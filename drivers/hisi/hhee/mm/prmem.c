/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Memory protection library - core functions
 * Author: Igor Stoppa <igor.stoppa@huawei.com>
 * Creator: security-ap
 * Date: 2020/04/15
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/slab.h>
#include <asm/cacheflush.h>
#include <linux/hisi/prmem.h>
#include <linux/mutex.h>
#include <linux/spinlock.h>
#include <linux/string.h>
#include <linux/vmalloc.h>
#include <linux/hisi/hisi_hkip.h>
#include <asm/barrier.h>
#include <linux/mutex.h>

#ifdef pr_fmt
#undef pr_fmt
#endif

#define pr_fmt(fmt) "prmem : " fmt

/*
 * Bypass mode for prmem
 *
 * If active, the bypass mode will prevent the write protection of any
 * memory allocated either for or by prmem.
 * Any write operation will be treated as a simple direct write.
 */

#define PRMEM_BYPASS 0

bool prmem_bypass_protection __ro_after_init;
EXPORT_SYMBOL(prmem_bypass_protection);

static DEFINE_MUTEX(pool_mutex_lock);

#ifdef CONFIG_HISI_PRMEM
#define MAX_ALIGN_ORDER (ilog2(sizeof(void *)))

/* Calculation of pool type from struct pool address */
extern char __end_data_ro_no_recl_prmem_pools[];
extern char __end_data_wr_no_recl_prmem_pools[];
extern char __end_data_start_wr_no_recl_prmem_pools[];
extern char __end_data_start_wr_recl_prmem_pools[];
extern char __end_data_wr_recl_prmem_pools[];
extern char __end_data_ro_recl_prmem_pools[];
extern char __end_data_rw_recl_prmem_pools[];

#define pool_range_entry(index, type)					\
	[index] = (unsigned long)__end_data_##type##_prmem_pools

unsigned long prmem_pools_ranges[] = {
	pool_range_entry(PRMEM_RO_NO_RECL, ro_no_recl),
	pool_range_entry(PRMEM_WR_NO_RECL, wr_no_recl),
	pool_range_entry(PRMEM_START_WR_NO_RECL, start_wr_no_recl),
	pool_range_entry(PRMEM_START_WR_RECL, start_wr_recl),
	pool_range_entry(PRMEM_WR_RECL, wr_recl),
	pool_range_entry(PRMEM_RO_RECL, ro_recl),
	pool_range_entry(PRMEM_RW_RECL, rw_recl),
};
#endif

/* Various helper functions. */

static inline size_t vmap_area_mem(struct vmap_area *va)
{
	return va->vm->nr_pages * PAGE_SIZE;
}

static inline size_t vmap_area_size(struct vmap_area *va)
{
	return va->va_end - va->va_start;
}

static inline unsigned long tag_vmap_area(struct vmap_area *va,
					  unsigned long tag)
{
	return va->vm->flags |= tag;
}

static inline unsigned long untag_vmap_area(struct vmap_area *va,
					    unsigned long tag)
{
	return va->vm->flags &= ~tag;
}

static inline void lock_vmap_area(int (*fn)(const void *base, size_t size),
				  struct vmap_area *va)
{
	if (!prmem_active())
		return;
	set_prmem_ro(va->va_start, va->vm->nr_pages);
	(*fn)((void *)(uintptr_t)va->va_start, vmap_area_mem(va));
}

static inline void unlock_vmap_area(int (*fn)(const void *base, size_t size),
				    struct vmap_area *va)
{
	if (!prmem_active())
		return;
	(*fn)((void *)(uintptr_t)va->va_start, vmap_area_mem(va));
	set_prmem_rw(va->va_start, va->vm->nr_pages);
}

static inline bool is_atomic(enum prmem_flags flags)
{
	return !!(flags & PRMEM_ATOMIC);
}

static inline int vmap_area_to_prmem_range_type(struct vmap_area *va)
{
	return prmem_get_type((void *)(uintptr_t)va->va_start, vmap_area_size(va));
}

static inline void protect_vmap_area(struct vmap_area *va,
				     enum prmem_pool_type type)
{
	switch (type) {
		case prmem_pool_ro_no_recl:
			lock_vmap_area(hkip_register_ro, va);
			break;
		case prmem_pool_start_wr_no_recl:
		case prmem_pool_wr_no_recl:
			lock_vmap_area(hkip_register_rowm, va);
			break;
		case prmem_pool_start_wr_recl:
		case prmem_pool_wr_recl:
			lock_vmap_area(hkip_register_rowm_mod, va);
			break;
		case prmem_pool_ro_recl:
			lock_vmap_area(hkip_register_ro_mod, va);
			break;
		case prmem_pool_rw_recl:
			break;
		default:
			WARN_ON("Unknown pool type.");
			return;
	}
	tag_vmap_area(va, VM_PRMEM_PROTECTED);
}

static inline void unprotect_vmap_area(struct vmap_area *va,
				       enum prmem_pool_type type)
{
	untag_vmap_area(va, VM_PRMEM_PROTECTED);
	if (is_pool_type_wr_recl(type))
		unlock_vmap_area(hkip_unregister_rowm_mod, va);
	else if (type == prmem_pool_ro_recl)
		unlock_vmap_area(hkip_unregister_ro_mod, va);
	memzero_explicit((void *)(uintptr_t)va->va_start, vmap_area_mem(va));
}

static inline bool empty(struct prmem_pool *pool)
{
	return unlikely(!pool->pmalloc_list);
}

/* Allocation from a protected area is allowed only for a START_WR pool. */
static inline bool unwritable(struct prmem_pool *pool)
{
	if (!(pool->pmalloc_list->va->vm->flags & VM_PRMEM_PROTECTED))
		return false;
	return !is_pool_type_start_wr(pool_to_prmem_range_type(pool));
}

static inline size_t get_alignment(struct prmem_pool *pool, size_t size,
				   int align_order)
{
	if (align_order < -1)
		return __roundup_pow_of_two(size);
	else if (align_order == -1)
		return pool->align;
	else
		return 1UL << align_order;
}

static inline bool exhausted(struct prmem_pool *pool, size_t size)
{
	/*
	 * Note: the test is done by offset vs size, without accounting for
	 * alignment, because the offset grows downward, in the direction
	 * of the beginning of the page(s) allocated, which is already page
	 * aligned. Page alignment is a superset of any reasonable
	 * alignment that might be required,up to PAGE_SIZE, included.
	 */
	return unlikely(pool->offset < size);
}

static inline bool space_needed(struct prmem_pool *pool, size_t size)
{
	return empty(pool) || unwritable(pool) || exhausted(pool, size);
}

static inline bool can_grow(struct prmem_pool *pool, size_t size)
{
	return ((pool->cap == PRMEM_NO_CAP) ||
		(pool->allocated + size <= pool->cap));
}

static inline struct vmap_area_node *get_vmap_area_node(void);
static inline int prmem_insert_vmap_area(struct vmap_area *va,
					 struct prmem_pool *pool,
					 struct vmap_area_node **list)
{
	struct vmap_area_node *node = NULL;

	node = get_vmap_area_node();
	if (WARN(node == NULL, "Failed to allocate vmap_area_node"))
		return -ENOMEM;
	va->pool = pool;
	va->node = node;
	wr_assign(node->va, va);
	wr_assign(node->next, (*list));
	wr_assign(node->va_start, va->va_start);
	wr_assign(node->va_size, va->vm->nr_pages * PAGE_SIZE);
	wr_assign((*list), node);
	return 0;
}

/* Support for linked list of vmap_area_node */

#define __roundup(a, b) ((((a) + (b) - 1) / (b)) * (b))

#define vmap_area_node_size_roundup					\
	__roundup(sizeof(struct vmap_area_node), sizeof(void *))

#define vmap_area_nodes_nr_roundup(nodes)				\
	(__roundup(nodes * vmap_area_node_size_roundup, PAGE_SIZE) /	\
	 vmap_area_node_size_roundup)

/*
 * Range of acceptable amount of nodes to preload in the vmap_nodes cache.
 * Max 4 pages seems reasonable, but there isn't really any hard evidence
 * that 4 is a good number. It should be enough, without taking too much
 * space, but maybe even 1 or 2 are sufficient in any real life case.
 */
#define MIN_VA_NODES_NR (PAGE_SIZE / vmap_area_node_size_roundup)
#define DEFAULT_VA_NODES_NR MIN_VA_NODES_NR
#define MAX_VA_NODES_NR (4 * MIN_VA_NODES_NR)
static unsigned int vmap_area_nodes_nr = DEFAULT_VA_NODES_NR;

static struct vmap_area_node *vmap_area_nodes_pool;

static inline bool validate_item_in_array(unsigned long start,
					  unsigned long end,
					  unsigned long size,
					  unsigned long item)
{
	return ((start <= item) && (item < end) &&
		(((item % PAGE_SIZE) % size) == 0));
}

bool is_vmap_area_node(struct vmap_area_node *node)
{
	unsigned long n = (unsigned long)(uintptr_t)node;
	unsigned long size = vmap_area_node_size_roundup;
	unsigned long start = (unsigned long)(uintptr_t)vmap_area_nodes_pool;
	unsigned long end = (start + vmap_area_nodes_nr * size);

	return validate_item_in_array(start, end, size, n);
}

#define vmap_area_node_order ilog2(__vmap_area_node_roundedup)
PRMEM_CACHE(vmap_area_nodes, NULL, sizeof(struct vmap_area_node),
	    sizeof(void *));

static inline struct vmap_area_node *get_vmap_area_node(void)
{
	struct vmap_area_node *node = NULL;

	node = prmem_cache_alloc(&vmap_area_nodes, PRMEM_ATOMIC);
	WARN_ONCE(!node, "Cache out of vmap_area_node elements!");
	return node;
}

static inline void put_vmap_area_node(struct vmap_area_node *node)
{
	prmem_cache_free(&vmap_area_nodes, node);
}

static inline bool is_prmem_vmap_area(enum prmem_flags flags)
{
	return !!(flags & PRMEM_VA);
}

static inline bool is_prmem_vmap_area_freeable(enum prmem_flags flags)
{
	return is_prmem_vmap_area(flags) && !!(flags & PRMEM_FREEABLE);
}

static struct vmap_area *prmem_vmap_area_prepare(enum prmem_pool_type type,
						 size_t min_size)
{
	struct vmap_area *new_area = NULL;
	unsigned long size;

	size = PAGE_ALIGN(min_size);
	new_area = prmem_vmap_area(size, (enum prmem_type)type);
	if (unlikely(!new_area))
		return NULL;
	tag_vmap_area(new_area, VM_PRMEM);
	memset((void *)(uintptr_t)new_area->va_start, 0, size);
	if (is_pool_type_start_wr(type))
		protect_vmap_area(new_area, (enum prmem_pool_type)type);
	return new_area;
}

static void pool_free_vmap_area(struct prmem_pool *pool,
				struct vmap_area *va);

static struct vmap_area *pool_add_vmap_area(struct prmem_pool *pool,
					    size_t size,
					    enum prmem_flags flags)
{
	struct vmap_area *va = NULL;
	struct vmap_area_node **list = NULL;
	enum prmem_pool_type type;

	type = pool_to_prmem_range_type(pool);
	if (is_prmem_vmap_area(flags)) {
		if (is_prmem_type_recl((enum prmem_type)type) &&
		    is_prmem_vmap_area_freeable(flags))
			list = &(pool->recl_va_list);
		else
			list = &(pool->no_recl_va_list);
	} else {
		size = (size > pool->refill) ? size : pool->refill;
		list = &(pool->pmalloc_list);
	}
	size = PAGE_ALIGN(size);
	if (!can_grow(pool, size)) {
		pr_err("Pool cap met.");
		return NULL;
	}
	va = prmem_vmap_area_prepare(type, size);
	if (WARN(!va, "Failed to allocate vmap_area"))
		return NULL;
	if (prmem_insert_vmap_area(va, pool, list) != 0) {
		pool_free_vmap_area(pool, va);
		return NULL;
	}
	if (*list != pool->pmalloc_list)
		goto out;
	if (is_prmem_type_start_wr((enum prmem_type)type)) {
		if (!pool->pmalloc_protected)
			wr_assign(pool->pmalloc_protected, true);
	} else {
		if (pool->pmalloc_protected)
			wr_assign(pool->pmalloc_protected, false);
	}
out:
	wr_add(pool->allocated, size);
	return va;
}

/*
 * prmem_vmap_area_alloc() - allocate one vmap_area of protected memory
 * @type: the type of memory to allocate
 * @min_size: the minimum size of the vmap_area to allocate
 *
 * The vmap_area is not associated to any pool and cannot be extended,
 * once allocated. It can only be released, if allowed by its type.
 *
 * Returns on success the vmap_area, NULL otherwise
 *
 * pmalloc and friends are not really meant for high-memory-pressure
 * usecases, so it is not particularly expensive to clear the memory,
 * before making it available for use.
 * Zeroing the memory ensures that it will not be possible to leak data
 * from previous users of a certain page.
 * Furhtermore, clearing the memory now, before it is possibly write
 * protected and handed down to its user, reduces the chances of having to
 * perform an ad-hoc HVC just for executing a wr_memset(0)
 */
struct vmap_area *prmem_vmap_area_alloc(struct prmem_pool *pool,
					size_t size,
					enum prmem_flags flags)
{
	unsigned long irq_flags;
	struct vmap_area *va = NULL;

	spin_lock_irqsave(pool->lock, irq_flags);
	va = pool_add_vmap_area(pool, size, flags | PRMEM_VA);
	spin_unlock_irqrestore(pool->lock, irq_flags);
	return va;
}
EXPORT_SYMBOL(prmem_vmap_area_alloc);

/*
 * prmap_vmap_area_alloc_addr() - allocate a vmap_area of protected memory
 * @type: the type of memory to allocate
 * @min_size: the minimum size of the vmap_area to allocate
 *
 * The vmap_area is not associated to any pool and cannot be extended,
 * once allocated. It can only be released, if allowed by its type.
 *
 * Returns on success the address of the allocation, NULL otherwise
 *
 * It is a wrapper for pvamp_alloc()
 */
void *prmem_vmap_area_alloc_addr(struct prmem_pool *pool, size_t size,
				 enum prmem_flags flags)
{
	struct vmap_area *va = NULL;

	va = prmem_vmap_area_alloc(pool, size, flags);
	if (unlikely(!va))
		return NULL;
	return (void *)(uintptr_t)va->va_start;
}
EXPORT_SYMBOL(prmem_vmap_area_alloc_addr);

#ifdef CONFIG_HISI_PRMEM
static inline bool is_pool_valid(struct prmem_pool *pool)
{
	unsigned long p = (unsigned long)(uintptr_t)pool;
	unsigned long start = (unsigned long)(uintptr_t)__start_data_prmem_pools;
	unsigned long end = (unsigned long)(uintptr_t)__end_data_prmem_pools;
	unsigned long size = __roundup(sizeof(struct prmem_pool),
				       sizeof(void *));

	return validate_item_in_array(start, end, size, p);
}
#else /* !defined(CONFIG_HISI_PRMEM) */
static inline bool is_pool_valid(struct prmem_pool *pool)
{
	return true;
}
#endif /* !defined(CONFIG_HISI_PRMEM) */

static inline bool is_vmap_area_in_list(struct vmap_area *va,
					struct vmap_area_node *node)
{
	while (node)
		if (node->va == va)
			return true;
		else
			node = node->next;
	return false;
}

static inline bool is_vmap_area_valid(struct vmap_area *va)
{
	if (unlikely(!(is_vmap_area_node(va->node) &&
		       (va->node->va == va) &&
		       vmap_area_is_prmem(va) &&
		       (va->va_start == va->node->va_start) &&
		       (va->vm->nr_pages * PAGE_SIZE == va->node->va_size))))
		return false;
	if (unlikely(!is_pool_valid(va->pool)))
		return false;
	if (unlikely(!(is_vmap_area_in_list(va, va->pool->recl_va_list) ||
		       is_vmap_area_in_list(va, va->pool->no_recl_va_list))))
		return false;
	return true;
}

/*
 * prmem_vmap_area_protect() - protect a vmap_area
 * @va: the vmap_area to protect
 *
 * Test that the area belongs to the PRMEM range and protects it
 */
void prmem_vmap_area_protect(struct vmap_area *va)
{
	unsigned long irq_flags;

	if (is_vmap_area_valid(va)) {
		spin_lock_irqsave(va->pool->lock, irq_flags);
		protect_vmap_area(va, vmap_area_to_prmem_range_type(va));
		spin_unlock_irqrestore(va->pool->lock, irq_flags);
	}
}
EXPORT_SYMBOL(prmem_vmap_area_protect);

/*
 * prmem_vmap_area_protect_addr() - protect a vmap_area, by address
 * @addr: the address at the beginning of the vmap_area to protect
 *
 * Test that the area belongs to the PRMEM range and protects it
 *
 * It is a wrapper for pvamp_protect()
 */
void prmem_vmap_area_protect_addr(void *addr)
{
	struct vmap_area *va = NULL;

	va = prmem_find_vmap_area((unsigned long)(uintptr_t)addr);
	if (likely(va != NULL))
		prmem_vmap_area_protect(va);
}
EXPORT_SYMBOL(prmem_vmap_area_protect_addr);

static void pool_free_vmap_area(struct prmem_pool *pool,
				struct vmap_area *va)
{
	struct vmap_area_node **n = &(pool->recl_va_list);

	while (*n) {
		struct vmap_area_node *tmp = *n;

		if ((*n)->va != va) {
			n = &((*n)->next);
			continue;
		}
		wr_assign(*n, (*n)->next);
		wr_sub(pool->allocated, PAGE_SIZE * va->vm->nr_pages);
		unprotect_vmap_area(va, vmap_area_to_prmem_range_type(va));
		untag_vmap_area(va, VM_PRMEM);
		pvfree((void *)(uintptr_t)tmp->va_start);
		put_vmap_area_node(tmp);
		return;
	}
}

/*
 * prmem_vmap_area_free() - free a previously allocated protected vmap_area
 * @va: the vmap_area to free
 *
 * Both the address range and the phisical pages are released.
 * The associated struct vmap_area_node is returned to the node cache.
 */
void prmem_vmap_area_free(struct vmap_area *va)
{
	unsigned long irq_flags;
	struct prmem_pool *pool = NULL;

	if (WARN(!vmap_area_is_reclaimable(va), "Unfreeable vmap_area!"))
		return;
	if (is_vmap_area_valid(va)) {
		pool = va->pool;
		spin_lock_irqsave(pool->lock, irq_flags);
		pool_free_vmap_area(pool, va);
		spin_unlock_irqrestore(pool->lock, irq_flags);
	}
}
EXPORT_SYMBOL(prmem_vmap_area_free);

/*
 * prmem_vmap_area_free_addr() - free a protected vmap_area, by address
 * @addr: the address at the beginning of the vmap_area to free
 *
 * Both the address range and the phisical pages are released
 *
 * It is a wrapper for pvamp_free()
 */
void prmem_vmap_area_free_addr(void *addr)
{
	struct vmap_area *va = NULL;

	va = prmem_find_vmap_area((unsigned long)(uintptr_t)addr);
	if (likely(va != NULL))
		prmem_vmap_area_free(va);
}
EXPORT_SYMBOL(prmem_vmap_area_free_addr);

static inline int grow(struct prmem_pool *pool, size_t size)
{
	struct vmap_area *va = NULL;

	va = pool_add_vmap_area(pool, size, PRMEM_NO_FLAGS);
	if (va != NULL)
		wr_assign(pool->offset, vmap_area_mem(va));
	return (va != NULL);
}

static inline bool space_available(struct prmem_pool *pool, size_t size,
				   enum prmem_flags flags)
{
	return ((!space_needed(pool, size)) ||
		((!is_atomic(flags)) && grow(pool, size)));
}

/*
 * prmem_pool_preload() - pre-allocate memory in a pool - can sleep
 * @pool: the pool to associate memory to
 * @size: the min size ofthe vmap_area to allocate
 *
 * Returns 0 in case of success, -ENOMEM otherwise
 */
int prmem_pool_preload(struct prmem_pool *pool, size_t size)
{
	bool ret = false;

	mutex_lock(&pool_mutex_lock);
	ret = space_available(pool, size, PRMEM_NO_FLAGS);
	mutex_unlock(&pool_mutex_lock);
	return ret;
}
EXPORT_SYMBOL(prmem_pool_preload);

/*
 * pmalloc_aligned() - allocate protectable memory from a pool
 * @pool: handle to the pool to be used for memory allocation
 * @size: amount of memory (in bytes) requested
 * @align_order: order of the alignment required.
 *               -1 : default value from pool settings
 *               < -1 : align to size, rounded up to power of 2
 * @flags: enum prmem_flags variable affecting the allocation
 *
 * Allocates memory from a pool.
 * If needed, the pool will automatically allocate enough memory to
 * either satisfy the request or meet the "refill" parameter received
 * upon creation.
 * New allocation can happen also if the current memory in the pool is
 * already write protected.
 * In case of concurrent allocations, competing users must take care of
 * ensuring serialization of the resource.
 *
 * Return:
 * * pointer to the memory requested	- success
 * * NULL				- error
 */
void *pmalloc_aligned(struct prmem_pool *pool, size_t size,
		      int align_order, enum prmem_flags flags)
{
	unsigned long irq_flags;
	size_t new_offset;
	void *ret = NULL;
	size_t alignment = get_alignment(pool, size, align_order);

	spin_lock_irqsave(pool->lock, irq_flags);
	if (!space_available(pool, size, flags))
		goto out;
	new_offset = round_down(pool->offset - size, alignment);
	wr_assign(pool->offset, new_offset);
	ret = (void *)(uintptr_t)(pool->pmalloc_list->va_start + new_offset);
out:
	spin_unlock_irqrestore(pool->lock, irq_flags);
	return ret;
}
EXPORT_SYMBOL(pmalloc_aligned);

/*
 * prmem_protect_pool() - write-protect the pmalloced memory in the pool
 * @pool: the pool with the pmalloced memory to write-protect
 *
 * Write-protects all the memory areas from the pool which are associated
 * to pmalloc.
 * This does not prevent further allocation of additional memory, that
 * can be initialized and protected.
 * The catch is that whatever spare memory might still be available in the
 * current va is forfaited.
 * This does not apply if the pool is of type "start wr".
 * Successive allocations will use a new va.
 */
void prmem_protect_pool(struct prmem_pool *pool)
{
	unsigned long irq_flags;
	struct vmap_area_node *node = NULL;
	enum prmem_pool_type type = pool_to_prmem_range_type(pool);

	if (unlikely(is_pool_type_start_wr(type)))
		return;
	spin_lock_irqsave(pool->lock, irq_flags);
	for (node = pool->pmalloc_list; node; node = node->next)
		protect_vmap_area(node->va, type);
	wr_assign(pool->pmalloc_protected, true);
	spin_unlock_irqrestore(pool->lock, irq_flags);
}
EXPORT_SYMBOL(prmem_protect_pool);

static void free_vmap_area_nodes_list(struct vmap_area_node **node)
{
	struct vmap_area_node *root = NULL;

	root = *node;
	wr_assign(*node, NULL);
	while(root) {
		struct vmap_area_node *tmp = root;

		pool_free_vmap_area(tmp->va->pool, tmp->va);
		root = root->next;
		put_vmap_area_node(tmp);
	}
}

/*
 * prmem_free_pool() - Releases all the memory associated to a pool
 * @pool: the pool whose memory must be reclaimed
 *
 * All the memory associated to the pool will be freed, if the pool
 * supports it. It has no effect on pools that prevent reclaiming memory.
 * Prior to altering the pool, all the vmap_areas are vetted for tampering.
 */
void prmem_free_pool(struct prmem_pool *pool)
{
	unsigned long irq_flags;

	if (WARN(!is_pool_recl(pool), "Cannot free pool!"))
	    return;
	spin_lock_irqsave(pool->lock, irq_flags);
	free_vmap_area_nodes_list(&(pool->pmalloc_list));
	free_vmap_area_nodes_list(&(pool->recl_va_list));
	free_vmap_area_nodes_list(&(pool->no_recl_va_list));
	wr_assign(pool->offset, 0);
	if (!is_pool_type_start_wr(pool_to_prmem_range_type(pool)))
		wr_assign(pool->pmalloc_protected, false);
	spin_unlock_irqrestore(pool->lock, irq_flags);
}
EXPORT_SYMBOL(prmem_free_pool);

static inline void inc_obj_cache_count(struct prmem_cache *cache)
{
	wr_inc(cache->count);
}

static inline void dec_obj_cache_count(struct prmem_cache *cache)
{
	wr_dec(cache->count);
}

/*
 * prmem_cache_alloc() - allocates an object from cache
 * @cache: the cache to allocate the object from
 * @flags: enum prmem_flags variable affecting the allocation
 *
 * Returns the requested object upon success, NULL otherwise
 */
void *prmem_cache_alloc(struct prmem_cache *cache, enum prmem_flags flags)
{
	unsigned long irq_flags;
	struct stack_node *node = NULL;

	spin_lock_irqsave(cache->lock, irq_flags);
	node = cache->node;
	if (likely(node != NULL)) {
		wr_assign(cache->node, cache->node->node);
		dec_obj_cache_count(cache);
		if (flags & PRMEM_ZERO)
			wr_memset(node, 0, cache->size);
	} else if (cache->pool != NULL) {
		node = pmalloc_aligned(cache->pool, cache->size,
				       cache->align_ord, flags);
	}
	spin_unlock_irqrestore(cache->lock, irq_flags);
	return (void *)node;
}
EXPORT_SYMBOL(prmem_cache_alloc);

static inline void prmem_cache_push(struct prmem_cache *cache, void *obj)
{
	struct stack_node *node = (struct stack_node *)obj;

	wr_assign(node->node, cache->node);
	wr_assign(cache->node, node);
	inc_obj_cache_count(cache);
}

/*
 * prmem_cache_free() - returns an object to the object cache
 * @cache: the cache into which the object must be pushed
 * @obj: the object to return
 */
void prmem_cache_free(struct prmem_cache *cache, void *obj)
{
	unsigned long irq_flags;

	spin_lock_irqsave(cache->lock, irq_flags);
	prmem_cache_push(cache, obj);
	spin_unlock_irqrestore(cache->lock, irq_flags);
}
EXPORT_SYMBOL(prmem_cache_free);

/*
 * prmem_cache_preload() - pre-populates an object cache
 * @cache: the cache to allocate the object from
 * @units: how many objects to push into the cache
 * @flags: enum prmem_flags variable affecting the allocation
 *
 * Returns the number of objects effectively pushed
 */
unsigned prmem_cache_preload(struct prmem_cache *cache,
			     unsigned units, enum prmem_flags flags)
{
	unsigned long irq_flags;
	unsigned i = 0;

	spin_lock_irqsave(cache->lock, irq_flags);
	if (cache->pool == NULL)
		goto out;
	for (i = 0; i < units; i++) {
		void *p = pmalloc_aligned(cache->pool, cache->size,
					  cache->align_ord, flags);

		if (unlikely(!p))
			goto out;
		prmem_cache_push(cache, p);
	}
out:
	spin_unlock_irqrestore(cache->lock, irq_flags);
	return i;
}
EXPORT_SYMBOL(prmem_cache_preload);

/* -------------------------- sysfs pools  ---------------------------- */

static struct kobject *prmem_create_dir(const char *name,
					struct kobject *parent);

static ssize_t prmem_pool_show_pmalloc_protected(struct kobject *dev,
						 struct kobj_attribute *attr,
						 char *buf)
{
	struct prmem_pool_sysfs_data *data = NULL;

	data = container_of(attr, struct prmem_pool_sysfs_data,
			    attr_pmalloc_protected);
	return snprintf(buf, PAGE_SIZE, "%s\n",
			(!data->pool->pmalloc_protected) ? "false" :
			(!hhee_is_present()) ? "fake_no_hhee" :
			prmem_bypass() ? "fake_bypass" : "true");
}

static ssize_t prmem_pool_show_allocated(struct kobject *dev,
				      struct kobj_attribute *attr,
				      char *buf)
{
	struct prmem_pool_sysfs_data *data = NULL;

	data = container_of(attr, struct prmem_pool_sysfs_data,
			    attr_allocated);
	return snprintf(buf, PAGE_SIZE, "%016lu\n", data->pool->allocated);
}

static ssize_t prmem_pool_show_cap(struct kobject *dev,
				   struct kobj_attribute *attr,
				   char *buf)
{
	struct prmem_pool_sysfs_data *data = NULL;

	data = container_of(attr, struct prmem_pool_sysfs_data,
			    attr_cap);
	return snprintf(buf, PAGE_SIZE, "%016lu\n", data->pool->cap);
}

static const char *prmem_pool_types_names[] = {
	[PRMEM_RO_NO_RECL] = "read only non reclaimable",
	[PRMEM_WR_NO_RECL] = "write rare non reclaimable",
	[PRMEM_START_WR_NO_RECL] =
		"pre-protected write rare non reclaimable",
	[PRMEM_START_WR_RECL] = "pre-protected write rare reclaimable",
	[PRMEM_WR_RECL] = "write rare reclaimable",
	[PRMEM_RO_RECL] = "read only reclaimable",
	[PRMEM_RW_RECL] = "read write reclaimable",
};

static ssize_t prmem_pool_show_allocation_type(struct kobject *dev,
					       struct kobj_attribute *attr,
					       char *buf)
{
	struct prmem_pool_sysfs_data *data = NULL;
	enum prmem_pool_type type;

	data = container_of(attr, struct prmem_pool_sysfs_data,
			    attr_allocation_type);
	type = (enum prmem_pool_type)pool_to_prmem_range_type(data->pool);
	return snprintf(buf, PAGE_SIZE, "%s\n",
			prmem_pool_types_names[type]);
}

#define prmem_sysfs_attr_init(data, attr_name)					\
do {										\
	sysfs_attr_init(&data->attr_##attr_name.attr);				\
	data->attr_##attr_name.attr.name = #attr_name;				\
	data->attr_##attr_name.attr.mode = VERIFY_OCTAL_PERMISSIONS(0640);	\
	data->attr_##attr_name.show = prmem_pool_show_##attr_name;		\
} while (0)

static struct kobject *create_prmem_sysfs_pool(struct prmem_pool *pool,
					       struct kobject *parent)
{
	struct kobject *root = NULL;
	struct prmem_pool_sysfs_data *data = NULL;

	root = prmem_create_dir(pool->name, parent);
	if (!root)
		return NULL;
	data = kzalloc(sizeof(*data), GFP_KERNEL);
	if (!data)
		return NULL;
	data->pool = pool;
	prmem_sysfs_attr_init(data, pmalloc_protected);
	prmem_sysfs_attr_init(data, allocated);
	prmem_sysfs_attr_init(data, cap);
	prmem_sysfs_attr_init(data, allocation_type);
	{
		const struct attribute *attrs[] = {
			&data->attr_pmalloc_protected.attr,
			&data->attr_allocated.attr,
			&data->attr_cap.attr,
			&data->attr_allocation_type.attr,
			NULL,
		};
		sysfs_create_files(root, attrs);
	}
	return root;
}

static void create_prmem_sysfs_pools(struct kobject *parent)
{
#ifdef CONFIG_HISI_PRMEM
	unsigned long start;
	unsigned long end;
	unsigned long size;
	unsigned long p;
	struct kobject *pools_kobj = NULL;
	struct kobject *pool = NULL;


	pools_kobj = prmem_create_dir("pools", parent);
	if (!pools_kobj)
		return;

	start = (unsigned long)__start_data_prmem_pools;
	end = (unsigned long)__end_data_prmem_pools;
	size = roundup(sizeof(struct prmem_pool), sizeof(void *));
	for (p = start; p < end; p += size) {

		pool = create_prmem_sysfs_pool((struct prmem_pool *)(uintptr_t)p,
					       pools_kobj);
		if (!pool)
			return;
	}
#endif
}

/* ------------------------------ init --------------------------------- */

static __init void preload_vmap_area_nodes_cache(void)
{
	int i;
	struct vmap_area *va = NULL;

	va = prmem_vmap_area_prepare(prmem_pool_start_wr_no_recl,
				     vmap_area_nodes_nr *
				     vmap_area_node_size_roundup);
	if (WARN(!va, "Unable to allocate vmap_area_nodes"))
		return;
	vmap_area_nodes_pool = (struct vmap_area_node *)(uintptr_t)(va->va_start);
	for (i = 0; i < vmap_area_nodes_nr; i++)
		prmem_cache_free(&vmap_area_nodes,
				 &vmap_area_nodes_pool[i]);
}

void mark_wr_data_wr(void);
void __init prmem_init(void)
{
	prmem_bypass_protection = PRMEM_BYPASS || !hhee_is_present();
	mark_wr_data_wr();
	preload_vmap_area_nodes_cache();
}

static __init int cmdline_read_nodes_nr(char *str)
{
	unsigned int nodes_nr;

	if (!sscanf(str, "%u", &nodes_nr) ||
	    nodes_nr < MIN_VA_NODES_NR || nodes_nr > MAX_VA_NODES_NR)
		vmap_area_nodes_nr = DEFAULT_VA_NODES_NR;
	else
		vmap_area_nodes_nr = vmap_area_nodes_nr_roundup(nodes_nr);
	return 1;
}
__setup("vmap_area_nodes_nr=", cmdline_read_nodes_nr);


/* -------------------------- common sysfs ----------------------------- */

extern struct kobject *kernel_kobj;

struct kobject *prmem_kobj;
EXPORT_SYMBOL(prmem_kobj);

static struct kobject *prmem_create_dir(const char *name,
					struct kobject *parent)
{
	struct kobject *dir = NULL;

	dir = kobject_create_and_add(name, parent);
	if (!dir)
		return NULL;
	dir->sd->mode &= 077000;
	dir->sd->mode |= 000750;
	return dir;
}

/* -------------------------- status sysfs ----------------------------- */

static ssize_t status_show(struct kobject *kobj,
			   struct kobj_attribute *attribute, char *buf)
{
	char *status = NULL;

	if (!prmem_enabled())
		status = "off";
	else if (!hhee_is_present())
		status = "no_hhee";
	else if (prmem_bypass())
		status = "bypass";
	else
		status = "enabled";
	return snprintf(buf, PAGE_SIZE, "%s", status);
}

static struct kobj_attribute *create_status_sysfs(void)
{
	struct kobj_attribute *attr = NULL;

	attr = kmalloc(sizeof(struct kobj_attribute), GFP_KERNEL);
	if (!attr)
		return NULL;
	sysfs_attr_init(&(attr->attr));

	attr->attr.name = "status";
	attr->attr.mode = 0440;
	attr->show = status_show;
	return attr;
}

static void create_prmem_sysfs_entries(void)
{
	struct kobj_attribute *status = NULL;

	prmem_kobj = prmem_create_dir("prmem", kernel_kobj);
	if (!prmem_kobj)
		goto err1;
	status = create_status_sysfs();
	if (!status)
		goto err2;
	sysfs_create_file(prmem_kobj,(const struct attribute *)status);
	create_prmem_sysfs_pools(prmem_kobj);
	return;
err2:
	kfree(prmem_kobj);
	prmem_kobj = NULL;
err1:
	pr_err("Error creating sysfs entries");
}

/* ------------------------------ init --------------------------------- */

static int __init prmem_late_init(void)
{
	create_prmem_sysfs_entries();
	return 0;
}
late_initcall(prmem_late_init);
