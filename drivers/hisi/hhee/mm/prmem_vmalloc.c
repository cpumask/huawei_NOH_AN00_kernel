/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: prmem-specific page management
 *
 * The code is almost entirely lifted from vmalloc.c and then symbols
 * naming is adjusted to reflect its limited scope (and avoid conflicts)
 * The deferred/lazy freeing paths are removed, because protected memory
 * is not very often released. But when the release happens, it is better
 * to not leave any widow of opportunity by delaying its processing.
 * Copyright of this code belongs to the copyright holders of the original
 * code.
 *
 * Creator: security-ap
 * Date: 2020/04/15
 */

#include <linux/mm.h>
#include <linux/module.h>
#include <linux/highmem.h>
#include <linux/sched/signal.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <linux/seq_file.h>
#include <linux/kallsyms.h>
#include <linux/list.h>
#include <linux/notifier.h>
#include <linux/rbtree.h>
#include <linux/radix-tree.h>
#include <linux/rcupdate.h>
#include <linux/pfn.h>
#include <linux/atomic.h>
#include <linux/compiler.h>
#include <linux/llist.h>
#include <linux/bitops.h>
#include <chipset_common/security/kshield.h>
#include <linux/hisi/prmem_vmalloc.h>
#include <linux/uaccess.h>
#include <asm/tlbflush.h>
#include <asm/shmparam.h>

#include "internal.h"
#include <linux/gfp.h>
#include <linux/stringify.h>
#include <asm/pgtable.h>

/*** Page table manipulation functions ***/

static void vunmap_pte_range(pmd_t *pmd, unsigned long addr, unsigned long end)
{
	pte_t *pte = NULL;

	pte = pte_offset_kernel(pmd, addr);
	do {
		pte_t ptent = ptep_get_and_clear(&init_mm, addr, pte);
		WARN_ON(!pte_none(ptent) && !pte_present(ptent));
	} while (pte++, addr += PAGE_SIZE, addr != end);
}

static void vunmap_pmd_range(pud_t *pud, unsigned long addr, unsigned long end)
{
	pmd_t *pmd = NULL;
	unsigned long next;

	pmd = pmd_offset(pud, addr);
	do {
		next = pmd_addr_end(addr, end);
		if (pmd_clear_huge(pmd))
			continue;
		if (pmd_none_or_clear_bad(pmd))
			continue;
		vunmap_pte_range(pmd, addr, next);
	} while (pmd++, addr = next, addr != end);
}

static void vunmap_pud_range(p4d_t *p4d, unsigned long addr, unsigned long end)
{
	pud_t *pud = NULL;
	unsigned long next;

	pud = pud_offset(p4d, addr);
	do {
		next = pud_addr_end(addr, end);
		if (pud_clear_huge(pud))
			continue;
		if (pud_none_or_clear_bad(pud))
			continue;
		vunmap_pmd_range(pud, addr, next);
	} while (pud++, addr = next, addr != end);
}

static void vunmap_p4d_range(pgd_t *pgd, unsigned long addr, unsigned long end)
{
	p4d_t *p4d = NULL;
	unsigned long next;

	p4d = p4d_offset(pgd, addr);
	do {
		next = p4d_addr_end(addr, end);
		if (p4d_clear_huge(p4d))
			continue;
		if (p4d_none_or_clear_bad(p4d))
			continue;
		vunmap_pud_range(p4d, addr, next);
	} while (p4d++, addr = next, addr != end);
}

static void vunmap_page_range(unsigned long addr, unsigned long end)
{
	pgd_t *pgd = NULL;
	unsigned long next;

	BUG_ON(addr >= end);
	pgd = pgd_offset_k(addr);
	do {
		next = pgd_addr_end(addr, end);
		if (pgd_none_or_clear_bad(pgd))
			continue;
		vunmap_p4d_range(pgd, addr, next);
	} while (pgd++, addr = next, addr != end);
}

static int vmap_pte_range(pmd_t *pmd, unsigned long addr,
		unsigned long end, pgprot_t prot, struct page **pages, int *nr)
{
	pte_t *pte = NULL;

	/*
	 * nr is a running index into the array which helps higher level
	 * callers keep track of where we're up to.
	 */

	pte = pte_alloc_kernel(pmd, addr);
	if (!pte)
		return -ENOMEM;
	do {
		struct page *page = pages[*nr];

		if (WARN_ON(!pte_none(*pte)))
			return -EBUSY;
		if (WARN_ON(!page))
			return -ENOMEM;
		set_pte_at(&init_mm, addr, pte, mk_pte(page, prot));
		(*nr)++;
	} while (pte++, addr += PAGE_SIZE, addr != end);
	return 0;
}

static int vmap_pmd_range(pud_t *pud, unsigned long addr,
		unsigned long end, pgprot_t prot, struct page **pages, int *nr)
{
	pmd_t *pmd = NULL;
	unsigned long next;

	pmd = pmd_alloc(&init_mm, pud, addr);
	if (!pmd)
		return -ENOMEM;
	do {
		next = pmd_addr_end(addr, end);
		if (vmap_pte_range(pmd, addr, next, prot, pages, nr))
			return -ENOMEM;
	} while (pmd++, addr = next, addr != end);
	return 0;
}

static int vmap_pud_range(p4d_t *p4d, unsigned long addr,
		unsigned long end, pgprot_t prot, struct page **pages, int *nr)
{
	pud_t *pud = NULL;
	unsigned long next;

	pud = pud_alloc(&init_mm, p4d, addr);
	if (!pud)
		return -ENOMEM;
	do {
		next = pud_addr_end(addr, end);
		if (vmap_pmd_range(pud, addr, next, prot, pages, nr))
			return -ENOMEM;
	} while (pud++, addr = next, addr != end);
	return 0;
}

static int vmap_p4d_range(pgd_t *pgd, unsigned long addr,
		unsigned long end, pgprot_t prot, struct page **pages, int *nr)
{
	p4d_t *p4d = NULL;
	unsigned long next;

	p4d = p4d_alloc(&init_mm, pgd, addr);
	if (!p4d)
		return -ENOMEM;
	do {
		next = p4d_addr_end(addr, end);
		if (vmap_pud_range(p4d, addr, next, prot, pages, nr))
			return -ENOMEM;
	} while (p4d++, addr = next, addr != end);
	return 0;
}

/*
 * Set up page tables in kva (addr, end). The ptes shall have prot "prot", and
 * will have pfns corresponding to the "pages" array.
 *
 * Ie. pte at addr+N*PAGE_SIZE shall point to pfn corresponding to pages[N]
 */
static int vmap_page_range_noflush(unsigned long start, unsigned long end,
				   pgprot_t prot, struct page **pages)
{
	pgd_t *pgd = NULL;
	unsigned long next;
	unsigned long addr = start;
	int err = 0;
	int nr = 0;

	BUG_ON(addr >= end);
	pgd = pgd_offset_k(addr);
	do {
		next = pgd_addr_end(addr, end);
		err = vmap_p4d_range(pgd, addr, next, prot, pages, &nr);
		if (err)
			return err;
	} while (pgd++, addr = next, addr != end);

	return nr;
}

static int vmap_page_range(unsigned long start, unsigned long end,
			   pgprot_t prot, struct page **pages)
{
	int ret;

	ret = vmap_page_range_noflush(start, end, prot, pages);
	flush_cache_vmap(start, end);
	return ret;
}

#define VM_LAZY_FREE	0x02
#define VM_VM_AREA	0x04

static DEFINE_SPINLOCK(vmap_area_lock);
/* Export for kexec only */
LIST_HEAD(prmem_vmap_area_list);
static LLIST_HEAD(vmap_purge_list);
static struct rb_root vmap_area_root = RB_ROOT;

static struct vmap_area *__find_vmap_area(unsigned long addr)
{
	struct rb_node *n = vmap_area_root.rb_node;

	while (n) {
		struct vmap_area *va;

		va = rb_entry(n, struct vmap_area, rb_node);
		if (addr < va->va_start)
			n = n->rb_left;
		else if (addr >= va->va_end)
			n = n->rb_right;
		else
			return va;
	}
	return NULL;
}

static void __insert_vmap_area(struct vmap_area *va)
{
	struct rb_node **p = &vmap_area_root.rb_node;
	struct rb_node *parent = NULL;
	struct rb_node *tmp = NULL;

	while (*p) {
		struct vmap_area *tmp_va;

		parent = *p;
		tmp_va = rb_entry(parent, struct vmap_area, rb_node);
		if (va->va_start < tmp_va->va_end)
			p = &(*p)->rb_left;
		else if (va->va_end > tmp_va->va_start)
			p = &(*p)->rb_right;
		else
			BUG();
	}

	rb_link_node(&va->rb_node, parent, p);
	rb_insert_color(&va->rb_node, &vmap_area_root);

	/* address-sort this list */
	tmp = rb_prev(&va->rb_node);
	if (tmp) {
		struct vmap_area *prev;
		prev = rb_entry(tmp, struct vmap_area, rb_node);
		list_add_rcu(&va->list, &prev->list);
	} else
		list_add_rcu(&va->list, &prmem_vmap_area_list);
}

static void purge_vmap_area_lazy(void);

static BLOCKING_NOTIFIER_HEAD(vmap_notify_list);

#define addr_is_vmalloc(addr) 0

/*
 * Allocate a region of KVA of the specified size and alignment, within the
 * vstart and vend.
 */
static struct vmap_area *alloc_vmap_area(unsigned long size,
				unsigned long align,
				unsigned long vstart, unsigned long vend,
				int node, gfp_t gfp_mask)
{
	struct vmap_area *va = NULL;
	struct rb_node *n = NULL;
	unsigned long addr;
	int purged = 0;
	struct vmap_area *first = NULL;

	BUG_ON(!size);
	BUG_ON(offset_in_page(size));
	BUG_ON(!is_power_of_2(align));

	might_sleep();

	va = kmalloc_node(sizeof(struct vmap_area),
			gfp_mask & GFP_RECLAIM_MASK, node);
	if (unlikely(!va))
		return ERR_PTR(-ENOMEM);

retry:
	spin_lock(&vmap_area_lock);
	/* find starting point for our search */
	addr = ALIGN(vstart, align);
	if (addr + size < addr)
		goto overflow;

	n = vmap_area_root.rb_node;
	first = NULL;

	while (n) {
		struct vmap_area *tmp;
		tmp = rb_entry(n, struct vmap_area, rb_node);
		if (tmp->va_end >= addr) {
			first = tmp;
			if (tmp->va_start <= addr)
				break;
			n = n->rb_left;
		} else
			n = n->rb_right;
	}

	if (!first)
		goto found;

	/* from the starting point, walk areas until a suitable hole is found */
	while (addr + size > first->va_start && addr + size <= vend) {
		addr = ALIGN(first->va_end, align);
		if (addr + size < addr)
			goto overflow;

		if (list_is_last(&first->list, &prmem_vmap_area_list))
			goto found;

		first = list_next_entry(first, list);
	}

found:
	/*
	 * Check also calculated address against the vstart,
	 * because it can be 0 because of big align request.
	 */
	if (addr + size > vend || addr < vstart)
		goto overflow;

	va->va_start = addr;
	va->va_end = addr + size;
	va->flags = 0;
	__insert_vmap_area(va);
	spin_unlock(&vmap_area_lock);

	BUG_ON(!IS_ALIGNED(va->va_start, align));
	BUG_ON(va->va_start < vstart);
	BUG_ON(va->va_end > vend);

	return va;

overflow:
	spin_unlock(&vmap_area_lock);
	if (!purged) {
		purge_vmap_area_lazy();
		purged = 1;
		goto retry;
	}

	if (gfpflags_allow_blocking(gfp_mask)) {
		unsigned long freed = 0;
		blocking_notifier_call_chain(&vmap_notify_list, 0, &freed);
		if (freed > 0) {
			purged = 0;
			goto retry;
		}
	}

	if (!(gfp_mask & __GFP_NOWARN) && printk_ratelimit())
		pr_warn("vmap allocation for size %lu failed: use vmalloc=<size> to increase size\n",
			size);
	kfree(va);
	return ERR_PTR(-EBUSY);
}

static void __free_vmap_area(struct vmap_area *va)
{
	BUG_ON(RB_EMPTY_NODE(&va->rb_node));

	rb_erase(&va->rb_node, &vmap_area_root);
	RB_CLEAR_NODE(&va->rb_node);
	list_del_rcu(&va->list);
	kfree_rcu(va, rcu_head);
}

/*
 * Clear the pagetable entries of a given vmap_area
 */
static void unmap_vmap_area(struct vmap_area *va)
{
	vunmap_page_range(va->va_start, va->va_end);
}

static void vmap_debug_free_range(unsigned long start, unsigned long end)
{
	/*
	 * Unmap page tables and force a TLB flush immediately if pagealloc
	 * debugging is enabled.  This catches use after free bugs similarly to
	 * those in linear kernel virtual address space after a page has been
	 * freed.
	 *
	 * All the lazy freeing logic is still retained, in order to minimise
	 * intrusiveness of this debugging feature.
	 *
	 * This is going to be *slow* (linear kernel virtual address debugging
	 * doesn't do a broadcast TLB flush so it is a lot faster).
	 */
	if (debug_pagealloc_enabled()) {
		vunmap_page_range(start, end);
		flush_tlb_kernel_range(start, end);
	}
}

/*
 * lazy_max_pages is the maximum amount of virtual address space we gather up
 * before attempting to purge with a TLB flush.
 *
 * There is a tradeoff here: a larger number will cover more kernel page tables
 * and take slightly longer to purge, but it will linearly reduce the number of
 * global TLB flushes that must be performed. It would seem natural to scale
 * this number up linearly with the number of CPUs (because vmapping activity
 * could also scale linearly with the number of CPUs), however it is likely
 * that in practice, workloads might be constrained in other ways that mean
 * vmap activity will not scale linearly with CPUs. Also, I want to be
 * conservative and not introduce a big latency on huge systems, so go with
 * a less aggressive log scale. It will still be an improvement over the old
 * code, and it will be simple to change the scale factor if we find that it
 * becomes a problem on bigger systems.
 */
static unsigned long lazy_max_pages(void)
{
	unsigned int log;

	log = fls(num_online_cpus());

	return log * (32UL * 1024 * 1024 / PAGE_SIZE);
}

static atomic_t vmap_lazy_nr = ATOMIC_INIT(0);

/*
 * Serialize vmap purging.  There is no actual criticial section protected
 * by this look, but we want to avoid concurrent calls for performance
 * reasons and to make the pcpu_get_vm_areas more deterministic.
 */
static DEFINE_MUTEX(vmap_purge_lock);

/*
 * Purges all lazily-freed vmap areas.
 */
static bool __purge_vmap_area_lazy(unsigned long start, unsigned long end)
{
	struct llist_node *valist = NULL;
	struct vmap_area *va = NULL;
	struct vmap_area *n_va = NULL;
	bool do_free = false;

	lockdep_assert_held(&vmap_purge_lock);

	valist = llist_del_all(&vmap_purge_list);
	llist_for_each_entry(va, valist, purge_list) {
		if (va->va_start < start)
			start = va->va_start;
		if (va->va_end > end)
			end = va->va_end;
		do_free = true;
	}

	if (!do_free)
		return false;

	flush_tlb_kernel_range(start, end);

	spin_lock(&vmap_area_lock);
	llist_for_each_entry_safe(va, n_va, valist, purge_list) {
		int nr = (va->va_end - va->va_start) >> PAGE_SHIFT;

		__free_vmap_area(va);
		atomic_sub(nr, &vmap_lazy_nr);
		cond_resched_lock(&vmap_area_lock);
	}
	spin_unlock(&vmap_area_lock);
	return true;
}

/*
 * Kick off a purge of the outstanding lazy areas. Don't bother if somebody
 * is already purging.
 */
static void try_purge_vmap_area_lazy(void)
{
	if (mutex_trylock(&vmap_purge_lock)) {
		__purge_vmap_area_lazy(ULONG_MAX, 0);
		mutex_unlock(&vmap_purge_lock);
	}
}

/*
 * Kick off a purge of the outstanding lazy areas.
 */
static void purge_vmap_area_lazy(void)
{
	mutex_lock(&vmap_purge_lock);
	__purge_vmap_area_lazy(ULONG_MAX, 0);
	mutex_unlock(&vmap_purge_lock);
}

/*
 * Free a vmap_area, caller ensuring that the area has been unmapped
 * and flush_cache_vunmap had been called for the correct range
 * previously.
 */
static void free_vmap_area_noflush(struct vmap_area *va)
{
	int nr_lazy;

	nr_lazy = atomic_add_return((va->va_end - va->va_start) >> PAGE_SHIFT,
				    &vmap_lazy_nr);

	/* After this point, we may free va at any time */
	llist_add(&va->purge_list, &vmap_purge_list);

	if (unlikely(nr_lazy > lazy_max_pages()))
		try_purge_vmap_area_lazy();
}

/*
 * Free and unmap a vmap_area
 */
static void free_unmap_vmap_area(struct vmap_area *va)
{
	flush_cache_vunmap(va->va_start, va->va_end);
	unmap_vmap_area(va);
	free_vmap_area_noflush(va);
}

/*
 * prmem_find_vmap_area - locate the vmap_area from a prmem address
 * @addr: the address associated to the vmap_area to locate
 */
struct vmap_area *prmem_find_vmap_area(unsigned long addr)
{
	struct vmap_area *va = NULL;

	spin_lock(&vmap_area_lock);
	va = __find_vmap_area(addr);
	spin_unlock(&vmap_area_lock);

	return va;
}

static int map_pvm_area(struct vm_struct *area, pgprot_t prot, struct page **pages)
{
	unsigned long addr = (unsigned long)(uintptr_t)area->addr;
	unsigned long end = addr + get_vm_area_size(area);
	int err;

	err = vmap_page_range(addr, end, prot, pages);

	return err > 0 ? 0 : err;
}

static void setup_pmalloc_vm(struct vm_struct *vm, struct vmap_area *va,
			      unsigned long flags, const void *caller)
{
	spin_lock(&vmap_area_lock);
	vm->flags = flags;
	vm->addr = (void *)(uintptr_t)va->va_start;
	vm->size = va->va_end - va->va_start;
	vm->caller = caller;
	va->vm = vm;
	va->flags |= VM_VM_AREA;
	spin_unlock(&vmap_area_lock);
}

static void clear_vm_uninitialized_flag(struct vm_struct *vm)
{
	/*
	 * Before removing VM_UNINITIALIZED,
	 * we should make sure that vm has proper values.
	 * Pair with smp_rmb() in show_numa_info().
	 */
	smp_wmb();
	vm->flags &= ~VM_UNINITIALIZED;
}

static struct vm_struct *__get_vm_area_node(unsigned long size,
		unsigned long align, unsigned long flags, unsigned long start,
		unsigned long end, int node, gfp_t gfp_mask, const void *caller)
{
	struct vmap_area *va = NULL;
	struct vm_struct *area = NULL;

	BUG_ON(in_interrupt());
	size = PAGE_ALIGN(size);
	if (unlikely(!size))
		return NULL;

	if (flags & VM_IOREMAP)
		align = 1ul << clamp_t(int, get_count_order_long(size),
				       PAGE_SHIFT, IOREMAP_MAX_ORDER);

	area = kzalloc_node(sizeof(*area), gfp_mask & GFP_RECLAIM_MASK, node);
	if (unlikely(!area))
		return NULL;

	if (!(flags & VM_NO_GUARD))
		size += PAGE_SIZE;

	va = alloc_vmap_area(size, align, start, end, node, gfp_mask);
	if (IS_ERR(va)) {
		kfree(area);
		return NULL;
	}

	setup_pmalloc_vm(area, va, flags, caller);

	return area;
}

/*
 *	remove_vm_area  -  find and remove a continuous kernel virtual area
 *	@addr:		base address
 *
 *	Search for the kernel VM area starting at @addr, and remove it.
 *	This function returns the found VM area, but using it is NOT safe
 *	on SMP machines, except for its size or flags.
 */
struct vm_struct *remove_pvm_area(const void *addr)
{
	struct vmap_area *va = NULL;

	might_sleep();

	va = __find_vmap_area((unsigned long)(uintptr_t)addr);
	if (va && va->flags & VM_VM_AREA) {
		struct vm_struct *vm = va->vm;

		spin_lock(&vmap_area_lock);
		va->vm = NULL;
		va->flags &= ~VM_VM_AREA;
		va->flags |= VM_LAZY_FREE;
		spin_unlock(&vmap_area_lock);

		vmap_debug_free_range(va->va_start, va->va_end);
		kasan_free_shadow(vm);
		free_unmap_vmap_area(va);

		return vm;
	}
	return NULL;
}

static void __vunmap(const void *addr, int deallocate_pages)
{
	struct vm_struct *area = NULL;

	if (!addr)
		return;

	if (WARN(!PAGE_ALIGNED(addr), "Trying to pvfree() bad address (%p)\n",
			addr))
		return;

	area = __find_vmap_area((unsigned long)(uintptr_t)addr)->vm;
	if (unlikely(!area)) {
		WARN(1, KERN_ERR "Trying to pvfree() nonexistent vm area (%p)\n",
				addr);
		return;
	}

	debug_check_no_locks_freed(addr, get_vm_area_size(area));
	debug_check_no_obj_freed(addr, get_vm_area_size(area));

	remove_pvm_area(addr);
	if (deallocate_pages) {
		int i;

		for (i = 0; i < area->nr_pages; i++) {
			struct page *page = area->pages[i];

			BUG_ON(!page);
			__free_pages(page, 0);
		}

		kvfree(area->pages);
	}

	kfree(area);
	return;
}

/*
 *	pvfree  -  release memory allocated by pvmalloc()
 *	@addr:		memory base address
 *
 *	Free the virtually continuous memory area starting at @addr, as
 *	obtained from vmalloc(), vmalloc_32() or __vmalloc(). If @addr is
 *	NULL, no operation is performed.
 *
 *	Must not be called in NMI context (strictly speaking, only if we don't
 *	have CONFIG_ARCH_HAVE_NMI_SAFE_CMPXCHG, but making the calling
 *	conventions for vfree() arch-depenedent would be a really bad idea)
 *
 *	NOTE: assumes that the object at @addr has a size >= sizeof(llist_node)
 */
void pvfree(const void *addr)
{
	BUG_ON(in_nmi());

	if (!addr)
		return;
	__vunmap(addr, 1);
}
EXPORT_SYMBOL(pvfree);

static void *__pvmalloc_node(unsigned long size, unsigned long align,
			    gfp_t gfp_mask, pgprot_t prot,
			    int node, const void *caller);
static void *__pvmalloc_area_node(struct vm_struct *area, gfp_t gfp_mask,
				 pgprot_t prot, int node)
{
	const int order = 0;
	struct page **pages = NULL;
	unsigned int nr_pages, array_size, i;
	const gfp_t nested_gfp = (gfp_mask & GFP_RECLAIM_MASK) | __GFP_ZERO;
	const gfp_t alloc_mask = gfp_mask | __GFP_NOWARN;
	const gfp_t highmem_mask = (gfp_mask & (GFP_DMA | GFP_DMA32)) ?
					0 :
					__GFP_HIGHMEM;

	nr_pages = get_vm_area_size(area) >> PAGE_SHIFT;
	array_size = (nr_pages * sizeof(struct page *));

	area->nr_pages = nr_pages;
	/* Please note that the recursion is strictly bounded. */
	if (array_size > PAGE_SIZE) {
		pages = __pvmalloc_node(array_size, 1, nested_gfp|highmem_mask,
				PAGE_KERNEL, node, area->caller);
	} else {
		pages = kmalloc_node(array_size, nested_gfp, node);
	}
	area->pages = pages;
	if (!area->pages) {
		remove_pvm_area(area->addr);
		kfree(area);
		return NULL;
	}

	for (i = 0; i < area->nr_pages; i++) {
		struct page *page = NULL;

		if (node == NUMA_NO_NODE)
			page = alloc_page(alloc_mask|highmem_mask);
		else
			page = alloc_pages_node(node, alloc_mask|highmem_mask, order);

		if (unlikely(!page)) {
			/* Successfully allocated i pages, free them in __vunmap() */
			area->nr_pages = i;
			goto fail;
		}
		area->pages[i] = page;
		page_tracker_set_type(page, TRACK_VMALLOC, 0);
		if (gfpflags_allow_blocking(gfp_mask|highmem_mask))
			cond_resched();
	}

	if (map_pvm_area(area, prot, pages))
		goto fail;
	return area->addr;

fail:
	warn_alloc(gfp_mask, NULL,
			  "pvmalloc: allocation failure, allocated %ld of %ld bytes",
			  (area->nr_pages*PAGE_SIZE), area->size);
	pvfree(area->addr);
	return NULL;
}

/*
 *	__vmalloc_node_range  -  allocate virtually contiguous memory
 *	@size:		allocation size
 *	@align:		desired alignment
 *	@start:		vm area range start
 *	@end:		vm area range end
 *	@gfp_mask:	flags for the page level allocator
 *	@prot:		protection mask for the allocated pages
 *	@vm_flags:	additional vm area flags (e.g. %VM_NO_GUARD)
 *	@node:		node to use for allocation or NUMA_NO_NODE
 *	@caller:	caller's return address
 *
 *	Allocate enough pages to cover @size from the page level
 *	allocator with @gfp_mask flags.  Map them into contiguous
 *	kernel virtual space, using a pagetable protection of @prot.
 */
void *__pvmalloc_node_range(unsigned long size, unsigned long align,
			unsigned long start, unsigned long end, gfp_t gfp_mask,
			pgprot_t prot, unsigned long vm_flags, int node,
			const void *caller)
{
	struct vm_struct *area = NULL;
	void *addr = NULL;
	unsigned long real_size = size;

	size = PAGE_ALIGN(size);
	if (!size || (size >> PAGE_SHIFT) > totalram_pages)
		goto fail;

	area = __get_vm_area_node(size, align, VM_ALLOC | VM_UNINITIALIZED |
				vm_flags, start, end, node, gfp_mask, caller);
	if (!area)
		goto fail;

	addr = __pvmalloc_area_node(area, gfp_mask, prot, node);
	if (!addr)
		return NULL;

	/*
	 * In this function, newly allocated vm_struct has VM_UNINITIALIZED
	 * flag. It means that vm_struct is not fully initialized.
	 * Now, it is fully initialized, so remove this flag here.
	 */
	clear_vm_uninitialized_flag(area);
	return addr;

fail:
	warn_alloc(gfp_mask, NULL,
			  "vmalloc: allocation failure: %lu bytes", real_size);
	return NULL;
}

/*
 *	__pvmalloc_node  -  allocate virtually contiguous memory
 *	@size:		allocation size
 *	@align:		desired alignment
 *	@gfp_mask:	flags for the page level allocator
 *	@prot:		protection mask for the allocated pages
 *	@node:		node to use for allocation or NUMA_NO_NODE
 *	@caller:	caller's return address
 *
 *	Allocate enough pages to cover @size from the page level
 *	allocator with @gfp_mask flags.  Map them into contiguous
 *	kernel virtual space, using a pagetable protection of @prot.
 *
 *	Reclaim modifiers in @gfp_mask - __GFP_NORETRY, __GFP_RETRY_MAYFAIL
 *	and __GFP_NOFAIL are not supported
 *
 *	Any use of gfp flags outside of GFP_KERNEL should be consulted
 *	with mm people.
 *
 */
static void *__pvmalloc_node(unsigned long size, unsigned long align,
			    gfp_t gfp_mask, pgprot_t prot,
			    int node, const void *caller)
{
	return __pvmalloc_node_range(size, align, PRMEM_START, PRMEM_END,
				gfp_mask, prot, 0, node, caller);
}

#ifndef PAGE_KERNEL_EXEC
# define PAGE_KERNEL_EXEC PAGE_KERNEL
#endif

#if   defined(CONFIG_64BIT) && defined(CONFIG_ZONE_DMA)
#define GFP_VMALLOC32 (GFP_DMA | GFP_KERNEL)
#else
/*
 * 64b systems should always have either DMA or DMA32 zones. For others
 * GFP_DMA32 should do the right thing and use the normal zone.
 */
#define GFP_VMALLOC32 GFP_DMA32 | GFP_KERNEL
#endif

/*-------------------------- set prmem ro & rw -------------------------*/

struct page_change_data {
        pgprot_t set_mask;
        pgprot_t clear_mask;
};

static int change_prmem_page_range(pte_t *ptep, pgtable_t token,
				   unsigned long addr, void *data)
{
	struct page_change_data *cdata = data;
	pte_t pte = *ptep;

	pte = clear_pte_bit(pte, cdata->clear_mask);
	pte = set_pte_bit(pte, cdata->set_mask);

	set_pte(ptep, pte);
	return 0;
}

/* This function assumes that the range is mapped with PAGE_SIZE pages. */
static int __change_prmem_common(unsigned long start, unsigned long size,
			  pgprot_t set_mask, pgprot_t clear_mask)
{
	struct page_change_data data;
	int ret;

	if (kshield_chk_change_mem(start, size, set_mask, clear_mask))
		return -EINVAL;

	data.set_mask = set_mask;
	data.clear_mask = clear_mask;

	ret = apply_to_page_range(&init_mm, start, size,
				  change_prmem_page_range, &data);

	flush_tlb_kernel_range(start, start + size);
	return ret;

}

/*
 * Since prmem uses a separate vmap_area_root, it cannot use the regular
 * vmalloc-oriented find_vma_area(), it needs to use the prmem variant
 * prmem_find_vm_area()
 */
static int change_prmem_common(unsigned long addr, int numpages,
			       pgprot_t set_mask, pgprot_t clear_mask)
{
	unsigned long start = addr;
	unsigned long size = PAGE_SIZE * numpages;
	unsigned long end = start + size;
	struct vmap_area *area = NULL;

	if (!numpages)
		return 0;

	if (!PAGE_ALIGNED(addr)) {
		start &= PAGE_MASK;
		end = start + size;
		WARN_ON_ONCE(1);
	}

	area = __find_vmap_area(addr);
	if (!area || end > area->va_end || !(area->vm->flags & VM_ALLOC))
		return -EINVAL;
	return __change_prmem_common(start, size, set_mask, clear_mask);
}

/*
 * set_prmem_ro - set ro a number of pages in prmem address space
 * @addr: the address of the location to write protect
 * @numpages: number of pages to write protect
 *
 * Returns the amount of pages protected.
 */
int set_prmem_ro(unsigned long addr, int numpages)
{
	if (unlikely(!prmem_active()))
		return numpages;
	return change_prmem_common(addr, numpages,
				   __pgprot(PTE_RDONLY),
				   __pgprot(PTE_WRITE));
}
EXPORT_SYMBOL(set_prmem_ro);

/*
 * set_prmem_rw - set rw a number of pages in prmem address space
 * @addr: the address of the location to unprotect
 * @numpages: number of pages to make writeable
 *
 * Returns the amount of pages unprotected.
 */
int set_prmem_rw(unsigned long addr, int numpages)
{
	if (unlikely(!prmem_active()))
		return numpages;
	return change_prmem_common(addr, numpages,
				   __pgprot(PTE_WRITE),
				   __pgprot(PTE_RDONLY));
}
EXPORT_SYMBOL(set_prmem_rw);


/*-------------------------- New Functionality -------------------------*/

/*
 * declare_prmem_range() - instantiates a specific pmalloc range
 * @name: the name of the range
 *
 * To avoid the tedious - and error-prone - work of populating the data
 * with the description of each pmalloc range, it is more appropriate to
 * have a macro pulling all the related information together.
 * WARNING: this *MUST* be kept in sync with "enum prmem_type"
 */
#define declare_prmem_range(name)					\
	[PRMEM_##name] = {						\
		.start = PRMEM_##name##_START,				\
		.end = PRMEM_##name##_END,				\
		.range_name = __stringify(name),			\
	}

const struct prmem_range prmem_ranges[PRMEM_INDEX_NUM] = {
	declare_prmem_range(RO_NO_RECL),
	declare_prmem_range(WR_NO_RECL),
	declare_prmem_range(START_WR_NO_RECL),
	declare_prmem_range(START_WR_RECL),
	declare_prmem_range(WR_RECL),
	declare_prmem_range(RO_RECL),
	declare_prmem_range(RW_RECL),
	declare_prmem_range(NO_RECL),
	declare_prmem_range(RECL),
	declare_prmem_range(WR),
	declare_prmem_range(START_WR),
	declare_prmem_range(FULL),
};
EXPORT_SYMBOL(prmem_ranges);

/*
 * prmem_vmap_area() - prepares a vma of the requested type
 * @size: the minimum size of the vma
 * @type: the type indicating the properties required
 *
 * Returns a pointer to the new vma on success, NULL otherwise
 */
struct vmap_area *prmem_vmap_area(unsigned long size, enum prmem_type type)
{
	void *addr = NULL;
	struct vmap_area *va = NULL;

	addr = __pvmalloc_node_range(size, 1, prmem_ranges[type].start,
				    prmem_ranges[type].end,
				    GFP_KERNEL | __GFP_HIGHMEM,
				    PAGE_KERNEL, 0, NUMA_NO_NODE,
				    __builtin_return_address(0));
	if (unlikely(!addr))
		return NULL;
	va = __find_vmap_area((unsigned long)(uintptr_t)addr);
	if (!va || unlikely(!prmem_in_range(addr, va->vm->nr_pages * PAGE_SIZE,
				     type)))
		return NULL;
	return va;
}
EXPORT_SYMBOL(prmem_vmap_area);
