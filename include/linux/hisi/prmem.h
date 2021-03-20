/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: HKIP prmem support
 * Author: Igor Stoppa <igor.stoppa@huawei.com>
 * Creator: security-ap
 * Date: 2020/04/15
 */

#ifndef _LINUX_PRMEM_H
#define _LINUX_PRMEM_H

#include <linux/string.h>
#include <linux/vmalloc.h>
#include <linux/hisi/prmem_vmalloc.h>
#include <linux/hisi/hisi_hkip_prmem.h>
#include <linux/mutex.h>
#include <linux/log2.h>
#include <linux/spinlock_types.h>
#include <linux/stringify.h>
#include <asm/sections.h>
#include <asm/sizes.h>
#include <linux/hisi/prmem_cfg.h>
#include <linux/hisi/prmem_internals.h>
#include <linux/hisi/prmem_defs.h>

/*
 * wr_memset() - sets n bytes of the destination p to the c value
 * @p: beginning of the memory to write to
 * @c: byte to replicate
 * @n: amount of bytes to copy
 *
 * Returns pointer to the destination
 *
 * The function is forced to be inlined to prevent it from becoming a
 * gadget in code flow attacks.
 */
#ifdef CONFIG_HISI_PRMEM
static __always_inline void *wr_memset(void *p, int c, size_t n)
{
	unsigned long dst = (unsigned long)(uintptr_t)p;
	/* Replicate the LSB of c across the 8 bytes in v */
	uint64_t v = 0x0101010101010101 * (0xFF & (unsigned int)c);

	if (unlikely(!prmem_active()) || unlikely(is_rw(p, n))) {
	    memset(p, c, n);
	    return p;
	}
	if (WARN(!is_wr(p, n), "Unsupported memory write"))
		return p;
	if (likely(n == 8) && likely(!(dst & 7)))
		hkip_write_rowm_64(p, v);
	else if (likely((n == 4) && likely(!(dst & 3))))
		hkip_write_rowm_32(p, (uint32_t)v);
	else if (likely((n == 2) && likely(!(dst & 1))))
		hkip_write_rowm_16(p, (uint16_t)v);
	else if (likely(n == 1))
		hkip_write_rowm_8(p, (uint8_t)v);
	else
		hkip_set_rowm(p, c, n);
	return p;
}
#else /* CONFIG_HISI_PRMEM */
static __always_inline void *wr_memset(void *p, int c, __kernel_size_t n)
{
	return memset(p, c, n);
}
#endif /* CONFIG_HISI_PRMEM */

/*
 * wr_memcpy() - copies n bytes from q to p
 * @p: beginning of the memory to write to
 * @q: beginning of the memory to read from
 * @n: amount of bytes to copy
 *
 * Returns pointer to the destination
 *
 * The function is forced to be inlined to prevent it from becoming a
 * gadget in code flow attacks.
 */
#ifdef CONFIG_HISI_PRMEM
static __always_inline void *wr_memcpy(void *p, const void *q, size_t n)
{
	unsigned long dst = (unsigned long)(uintptr_t)p;

	if (unlikely(!prmem_active()) || unlikely(is_rw(p, n))) {
	    memcpy(p, q, n);
	    return p;
	}
	if (WARN(!is_wr(p, n), "Unsupported memory write"))
		return p;
	if (likely(n == 8) && likely(!(dst & 7)))
		hkip_write_rowm_64(p, *(uint64_t *)q);
	else if (likely((n == 4) && likely(!(dst & 3))))
		hkip_write_rowm_32(p, *(uint32_t *)q);
	else if (likely((n == 2) && likely(!(dst & 1))))
		hkip_write_rowm_16(p, *(uint16_t *)q);
	else if (likely(n == 1))
		hkip_write_rowm_8(p, *(uint8_t *)q);
	else
		hkip_write_rowm(p, q, n);
	return p;
}
#else /* CONFIG_HISI_PRMEM */
static __always_inline void *wr_memcpy(void *p, const void *q,
				       __kernel_size_t n)
{
	return memcpy(p, q, n);
}
#endif /* CONFIG_HISI_PRMEM */

/*
 * wr_add() - increments a write-rare variable with a given value
 * @dst: the variable to add to
 * @val: the value to add
 *
 * Returns: the updated value of the variable
 */
#define wr_add(dst, val)						\
do {									\
	typeof((dst)) *dst_ptr = &(dst);				\
	typeof(*dst_ptr) tmp = (*dst_ptr) + (typeof(tmp))(val);		\
									\
	wr_memcpy(dst_ptr, &tmp, sizeof(tmp));				\
	*dst_ptr;							\
} while(0)

/*
 * wr_inc() - decrements a write-rare variable by one
 * @dst: the variable to decrement
 *
 * Returns: the updated value of the variable
 */
#define wr_inc(dst)  wr_add(dst, 1)

/*
 * wr_sub() - decrements a write-rare variable with a given value
 * @dst: the variable to subtract from
 * @val: the value to subtract
 *
 * Returns: the updated value of the variable
 */
#define wr_sub(dst, val)  wr_add(dst, -(val))

/*
 * wr_dec() - decrements a write-rare variable by one
 * @dst: the variable to decrement
 *
 * Returns: the updated value of the variable
 */
#define wr_dec(dst)  wr_sub(dst, 1)

/*
 * wr_assign() - sets a write-rare variable to a specified value
 * @dst: the variable to set
 * @val: the new value
 *
 * Returns: the updated value of the variable
 */
#define wr_assign(dst, val)						\
do {									\
	typeof((dst)) *dst_ptr = &(dst);				\
	typeof(*dst_ptr) tmp = (typeof(tmp))(val);			\
									\
	wr_memcpy(dst_ptr, &tmp, sizeof(tmp));				\
	*dst_ptr;							\
} while(0)

/*
 * wr_rcu_assign_pointer() - initialize a pointer in rcu mode
 * @p: the rcu pointer - it MUST be aligned to a machine word
 * @v: the new value
 *
 * Returns the value assigned to the rcu pointer.
 *
 * It is provided as macro, to match rcu_assign_pointer()
 * The rcu_assign_pointer() is implemented as equivalent of:
 *
 * smp_mb();
 * WRITE_ONCE();
 */
#define wr_rcu_assign_pointer(p, v)					\
do {									\
	smp_mb();							\
	wr_assign(p, (v));						\
	p;								\
} while(0)

/* -------------------- Runtime Allocator -------------------- */

#define kB(x) ((x) * SZ_1K)
#define MB(x) ((x) * SZ_1M)
#define PRMEM_DEFAULT_REFILL PAGE_SIZE
#define PRMEM_DEFAULT_ALIGN_ORD ilog2(ARCH_KMALLOC_MINALIGN)

#define PRMEM_NO_CAP 0

/*
 * Under normal circumstances, the address of the pool encodes its
 * properties, however in prmem-off mode, the pool is allocaetd just like
 * any other variable, and therefore it needs a backup mechanism for
 * tracking its properties, through an extra field.
 */
#ifdef CONFIG_HISI_PRMEM
#define set_pool_type(type)
#else
#define set_pool_type(type)	.pool_type = prmem_pool_##type
#endif

/*
 * PRMEM_POOL() - create an empty protected memory pool
 * @_name: the name of the pool to be created
 * @type: any of the prmem_pool_type values
 * @al: alignemnt to use when allocating memory
 * @refill_val: min memory for new allocations, roundedup by PAGE_SIZE
 * @cap_val: max memory allocated by the pool, roundedup by PAGE_SIZE
 *
 * NOTES:
 * - alignment must be a power of 2, if positive; negative or zero
 * values will default the alignment to ARCH_KMALLOC_MINALIGN
 * - refill_val set to 0 selects PRMEM_DEFAULT_REFILL
 * - cap_val set to PRMEM_NO_CAP disables the memory capping for the pool
 * - union fields used in off-mode are implicitly initialized by on-mode
 */
#define PRMEM_POOL(_name, _type, _al, _refill_val, _cap_val)		\
	;								\
	static DEFINE_SPINLOCK(_name##_pool_lock);			\
	static const char _name##_pool_name[] = __stringify(_name);	\
	struct prmem_pool _name __prmem_pool_type(_type) = {		\
		.name = _name##_pool_name,				\
		.pmalloc_list = NULL,					\
		.recl_va_list = NULL,					\
		.no_recl_va_list = NULL,				\
		.offset = 0,						\
		.align = (_al) <= 0 ? PRMEM_DEFAULT_ALIGN_ORD :		\
				     validate_pool_alignment(_al),	\
		.refill = (!(_refill_val)) ? PRMEM_DEFAULT_REFILL :	\
					    round_up((_refill_val),	\
						     PAGE_SIZE),	\
		.cap = round_up(_cap_val, PAGE_SIZE),			\
		.allocated = 0,						\
		.lock = &(_name##_pool_lock),				\
		.pmalloc_protected = false,				\
		set_pool_type(_type)					\
	}

int prmem_pool_preload(struct prmem_pool *pool, size_t size);

void *pmalloc_aligned(struct prmem_pool *pool, size_t size,
		      int align_order, enum prmem_flags flags);

/*
 * pzalloc_aligned() - allocates and zeroes memory aligned as requested
 * @pool: the pool to use for the allocation
 * @size: the minimum amount of memory to allocate
 * @alignment_order: minimum alignement for the allocation
 * @flags: enum prmem_flags variable affecting the allocation
 *
 * Returns on success the required amount of memory, zeroed, NULL on fail
 *
 * pzalloc_aligned is an alias for pmalloc_aligned because pmalloc already
 * zeroes the memory provided, by default.
 */
static inline void *pzalloc_aligned(struct prmem_pool *pool, size_t size,
				    int align_order, enum prmem_flags flags)
{
	return pmalloc_aligned(pool, size, align_order, flags);
}

/*
 * pmalloc() - allocates memory as requested
 * @pool: the pool to use for the allocation
 * @size: the minimum amount of memory to allocate
 * @flags: enum prmem_flags variable affecting the allocation
 *
 * Returns on success the required amount of memory, NULL on fail
 *
 * The alignment used will be the default one, taken from the pool setting
 * The memory is automatically zeroed by the pool, to avoid leaking data.
 */
static inline void *pmalloc(struct prmem_pool *pool, size_t size,
			    enum prmem_flags flags)
{
	return pmalloc_aligned(pool, size, -1, flags);
}

/*
 * pzalloc() - allocates and zeroes memory
 * @pool: the pool to use for the allocation
 * @size: the minimum amount of memory to allocate
 * @flags: enum prmem_flags variable affecting the allocation
 *
 * Returns on success the required amount of memory, zeroed, NULL on fail
 *
 * pzalloc is an alias for pmalloc because pmalloc already zeroes the
 * memory provided, by default.
 *
 * The alignment used will be the default one, taken from the pool setting
 * The memory is automatically zeroed by the pool, to avoid leaking data.
 */
static inline void *pzalloc(struct prmem_pool *pool, size_t size,
			    enum prmem_flags flags)
{
	return pmalloc(pool, size, flags);
}

/*
 * pcalloc() - allocates memory for an array of n elements of given size
 * @pool: the pool to use for the allocation
 * @n: the number of elements in the array
 * @size: the minimum amount of memory to allocate for one element
 * @flags: enum prmem_flags variable affecting the allocation
 *
 * The alignment used will be the one defined by the pool
 * The memory is pre-zeroed
 */
static inline void *pcalloc(struct prmem_pool *pool, size_t n,
			    size_t size, enum prmem_flags flags)
{
	return pmalloc_aligned(pool,
			       round_up(size, pool->align) * n,
			       ilog2(pool->align), flags);
}

void prmem_protect_pool(struct prmem_pool *pool);

static inline bool prmem_pool_pmalloc_protected(struct prmem_pool *pool)
{
	return pool->pmalloc_protected;
}

void prmem_free_pool(struct prmem_pool *pool);

struct vmap_area *prmem_vmap_area_alloc(struct prmem_pool *pool,
					size_t size,
					enum prmem_flags flags);
void prmem_vmap_area_protect(struct vmap_area *va);
void prmem_vmap_area_free(struct vmap_area *va);
void *prmem_vmap_area_alloc_addr(struct prmem_pool *pool, size_t size,
				 enum prmem_flags flags);
void prmem_vmap_area_protect_addr(void *addr);
void prmem_vmap_area_free_addr(void *addr);


/*
 * pstrdup - duplicate a string, using pmalloc as allocator
 * @pool: handler to the pool to be used for memory allocation
 * @s: string to duplicate
 * @flags: enum prmem_flags variable affecting the allocation
 *
 * Generates a copy of the given string, allocating sufficient memory
 * from the given pmalloc pool.
 *
 * Returns a pointer to the replica, NULL in case of error.
 */
#ifdef CONFIG_HISI_PRMEM
static inline char *pstrdup(struct prmem_pool *pool, const char *s,
			    enum prmem_flags flags)
{
	size_t len;
	char *buf = NULL;

	if (unlikely(!pool || !s))
		return NULL;

	len = strlen(s) + 1;
	buf = pmalloc(pool, len, flags);
	if (unlikely(!buf))
		return NULL;
	if (pmalloc_is_start_wr(buf, 1))
		wr_memcpy(buf, s, len);
	else
		strncpy(buf, s, len);
	return buf;
}
#else
static inline char *pstrdup(struct prmem_pool *pool, const char *s,
			    enum prmem_flags flags)
{
	return kstrdup(s, GFP_KERNEL);
}
#endif

/* -------------------------- Object Cache -------------------------- */
#define PRMEM_CACHE_ALIGN_AS_POOL   (-1)

/*
 * PRMEM_CACHE() - Declare a cache
 * @name: name of the cache
 * @_pool: the optional pool to allocate new objects from
 * @_size: the size, in bytes, of the objects to allocate
 * @_alignment: the alignment, in bytes, of the objects to allocate
 *
 * The macro declares 2 objects:
 * - a private (static) spinlock, referred only by the pointer inside the
 *   cache
 * - the cache itself
 *
 * Because of the semantics, it is not possible to use modifiers with the
 * cache. The lone semicolon at the beginning of the macro is meant to
 * trigger warnings, should any modifier be prepended to PRMEM_CACHE()
 * The warning triggered is:
 * warning: declaration does not declare anything [-Wmissing-declarations]
 *
 * If the cache is pre-loaded with statically allocated objects, the pool
 * is optional.
 */
#define PRMEM_CACHE(_name, _pool, _size, _alignment)				\
	;									\
	static DEFINE_SPINLOCK(_name##_cache_lock);				\
	static const char _name##_cache_name[] = __stringify(_name);		\
	struct prmem_cache _name __prmem_object_cache = {			\
		.name = _name##_cache_name,					\
		.node = NULL,							\
		.count = 0,							\
		.pool = (_pool),						\
		.size = validate_object_size(round_up(_size, _alignment)),	\
		.align_ord = ((_alignment) <= 0) ? -1 :				\
			      ilog2(validate_cache_alignment(_alignment)),	\
		.lock = &(_name##_cache_lock),					\
		.prot = false,							\
		.sysfs_data = NULL,						\
	}

void *prmem_cache_alloc(struct prmem_cache *cache, enum prmem_flags flags);

/*
 * prmem_cache_zalloc() - allocate object from the cache, zeroed
 * @cache: the object cache to allocate from
 * @flags: enum prmem_flags variable affecting the allocation
 *
 * Returns on success pointer to the object cache, NULL otherwise
 */
static inline void *prmem_cache_zalloc(struct prmem_cache *cache,
				       enum prmem_flags flags)
{
	return prmem_cache_alloc(cache, flags | PRMEM_ZERO);
}

void prmem_cache_free(struct prmem_cache *cache, void *obj);

unsigned prmem_cache_preload(struct prmem_cache *cache, unsigned units,
			     enum prmem_flags flags);

/*
 * pstrdup_cache - duplicate a string, using a pmalloc cache as allocator
 * @cache: handler to the pool to be used for memory allocation
 * @s: string to duplicate
 * @flags: enum prmem_flags variable affecting the allocation
 *
 * Generates a copy of the given string, allocating sufficient memory
 * from the given prmem pool.
 *
 * Returns a pointer to the replica, NULL in case of error.
 */
static inline char *pstrdup_cache(struct prmem_cache *cache, const char *s,
				  enum prmem_flags flags)
{
	size_t len;
	char *buf = NULL;

	if (unlikely(!cache || !s))
		return NULL;

	len = strlen(s) + 1;
	if (unlikely(len > cache->size))
		return NULL;

	buf = prmem_cache_alloc(cache, flags);
	if (unlikely(!buf))
		return NULL;
	wr_memcpy(buf, s, len); /* A pmalloc object cache is always wr */
	return buf;
}

/*
 * pfree - Dummy function for replacing calls to kfree() / vfree()
 */
#define pfree(no_op_1, no_op_2)

#endif
