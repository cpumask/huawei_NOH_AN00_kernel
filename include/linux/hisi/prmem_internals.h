/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Internal header for memory protection library
 * Author: Igor Stoppa <igor.stoppa@huawei.com>
 * Creator: security-ap
 * Date: 2020/04/15
 */

#ifndef _LINUX_PRMEM_INTERNALS_H
#define _LINUX_PRMEM_INTERNALS_H
#include <linux/hisi/prmem_cfg.h>
#include <linux/hisi/prmem_defs.h>
#include <linux/hisi/prmem_vmalloc.h>
#include <linux/kobject.h>

struct prmem_pool_sysfs_data {
	struct prmem_pool *pool;
	struct kobj_attribute attr_pmalloc_protected;
	struct kobj_attribute attr_allocated;
	struct kobj_attribute attr_cap;
	struct kobj_attribute attr_allocation_type;
};

/* ------------------------ Validation Macros ------------------------ */

/*
 * VALIDATE() - Generate a compile-time error if the condition is not met
 * @val: the expression to be evaluated - it must resolve at compile-time
 *       and it must not have side effects, if evaluated twice
 * @cond: macro to use for testing @val - it must take exactly 1 parameter
 *
 * It can be daunting to debug errors in parameters passed to macros that
 * declare complex data structures. The best way to catch such bugs is to
 * introduce a mechanism which works like ASSERT() and generates a
 * compile-time error, exactly on the spot of the offending parameter.
 * This also has the additional advantage of not introducing any overhead
 * during runtime.
 *
 * Returns @val if the test is successful, otherwise it will halt the build
 */
#define VALIDATE(val, cond) ((val) / !!cond(val))

/*
 * is_power_of_two() - returns true if the parameter is a power of 2
 *                     and this macro can be evaluated at compile time.
 * @n: the number to check if it's a power of 2
 *
 * Returns: true if @n is ap ower of 2, error otherwise
 */
#define is_power_of_two(n) (n && ((n & (n - 1)) == 0))

/*
 * test_pool_alignment() - checks if the alignment is a power of 2
 * @alignment: the value to validate as power of 2
 *
 * Returns true if @alignment is a power of 2, false otherwise
 */
#define test_pool_alignment(alignment) is_power_of_two(alignment)

/*
 * validate_pool_alignment() - Generate compile time error if the
 *                             alignemnt is not a power of 2
 * @alignment: the value to validate as power of 2
 *
 * Returns true if @alignment is a power of 2, error otherwise
 */
#define validate_pool_alignment(alignment)				\
	VALIDATE(alignment, test_pool_alignment)

/*
 * test_object_size() - check if the object can contain a pointer
 * @size: the size, in bytes, required for a single object in the cache
 *
 * The cache stores objects in the form of a linked list. Each object must
 * be large enough to container a pointer to the next.
 *
 * Returns true if the object is sufficiently large, false otherwise
 */
#define test_object_size(size) ((size) >= sizeof(void *))

/*
 * validate_object_size() - check if the object can contain a pointer
 * @size: the size, in bytes, required for a single object in the cache
 *
 * The cache stores objects in the form of a linked list. Each object must
 * be large enough to container a pointer to the next.
 *
 * Returns true if the object is sufficiently large, error otherwise
 */
#define validate_object_size(size) VALIDATE(size, test_object_size)

/*
 * test_cache_alignment() - checks the alignment of an object cache
 * @alignment: the value, in bytes, to test
 *
 * Returns true if value is valid, false otherwise
 * @alignment must be a power of 2 large enough to contain a pointer
 */
#define test_cache_alignment(alignment)					\
	(is_power_of_two(alignment) && test_object_size(alignment))

/*
 * validate_cache_alignment() - checks the alignment of an object cache
 * @alignment: the value, in bytes, to test
 *
 * Returns true if value is valid, error otherwise
 * @alignment must be a power of 2 large enough to contain a pointer
 */
#define validate_cache_alignment(alignment)				\
	VALIDATE(alignment, test_cache_alignment)

/* ------------------------ prmem sections --------------------------- */

#ifdef CONFIG_HISI_PRMEM
extern char __start_data_wr[], __end_data_wr[];
extern char __start_data_prmem_pools[], __end_data_prmem_pools[];
extern char __start_data_wr_after_init[], __end_data_wr_after_init[];
extern char __start_data_wr[], __end_data_wr[];
extern char __start_data_rw[], __end_data_rw[];
extern char __start_data_prmem[], __end_data_prmem[];
#endif /* !defined(CONFIG_HISI_PRMEM) */

/* --------------------- Range Validation Helpers --------------------- */

/*
 * is_static_wr() - is the buffer within static wr memory?
 * @p: beginning of the buffer
 * @n: size of the buffer
 *
 * Returns true/false
 */
#ifdef CONFIG_HISI_PRMEM
static __always_inline bool is_static_wr(void *p, size_t n)
{
	unsigned long low = (unsigned long)(uintptr_t)p;
	unsigned long high = low + n;
	unsigned long start = (unsigned long)(uintptr_t)__start_data_wr;
	unsigned long end = (unsigned long)(uintptr_t)__end_data_wr;

	return likely(start <= low && high <= end);
}
#else /* !defined(CONFIG_HISI_PRMEM) */
static __always_inline bool is_static_wr(void *p, size_t n)
{
	return true;
}
#endif /* !defined(CONFIG_HISI_PRMEM) */

/**
 * is_static_wr_after_init() - is the buffer within static wr after init?
 * @p: beginning of the buffer
 * @n: size of the buffer
 *
 * Returns true/false
 */
#ifdef CONFIG_HISI_PRMEM
static __always_inline bool is_static_wr_after_init(void *p, size_t n)
{
	unsigned long low = (unsigned long)(uintptr_t)p;
	unsigned long high = low + n;
	unsigned long start = (unsigned long)(uintptr_t)__start_data_wr_after_init;
	unsigned long end = (unsigned long)(uintptr_t)__end_data_wr_after_init;

	return likely(start <= low && high <= end);
}
#else /* !defined(CONFIG_HISI_PRMEM) */
static __always_inline bool is_static_wr_after_init(void *p, size_t n)
{
	return true;
}
#endif /* !defined(CONFIG_HISI_PRMEM) */

/*
 * is_wr() - tests if the buffer is write_rare
 * @p: beginning of the buffer
 * @n: size of the buffer
 *
 * The buffer can be either a statically allocated variable, or it can
 * belong to a protected allocation that is re-writable.
 *
 * Returns true/false
 */
static inline bool is_wr(void *p, size_t n)
{
	return (prmem_is_wr(p, n) || is_static_wr(p, n) ||
		is_static_wr_after_init(p, n));
}

/*
 * is_static_rw() - is the buffer within rw memory?
 * @p: beginning of the buffer
 * @n: size of the buffer
 *
 * Returns true/false
 */
#ifdef CONFIG_HISI_PRMEM
static __always_inline bool is_static_rw(void *p, size_t n)
{
	unsigned long low = (unsigned long)(uintptr_t)p;
	unsigned long high = low + n;
	unsigned long start = (unsigned long)(uintptr_t)__start_data_rw;
	unsigned long end = (unsigned long)(uintptr_t)__end_data_rw;

	return likely(start <= low && high <= end);
}
#else /* !defined(CONFIG_HISI_PRMEM) */
static __always_inline bool is_static_rw(void *p, size_t n)
{
	return true;
}
#endif /* !defined(CONFIG_HISI_PRMEM) */

/*
 * is_rw() - tests if the buffer is in the special read/write area
 * @p: beginning of the buffer
 * @n: size of the buffer
 *
 * The buffer can be either a statically allocated variable, or it can
 * belong to a protected allocation that is a special read/write
 * allocation
 *
 * Returns true/false
 */
static inline bool is_rw(void *p, size_t n)
{
	return (pmalloc_is_rw(p, n) || is_static_rw(p, n));
}

bool is_vmap_area_node(struct vmap_area_node *node);

/* -------------------- Runtime Allocator -------------------- */

/*
 * is_pool_type_recl - check if memory from the pool can be reclaimed
 * @type: the pool type
 *
 * Returns true if the memory type is reclaimable, false otherwise
 */
static inline bool is_pool_type_recl(enum prmem_pool_type type) {
	return (type == prmem_pool_start_wr_recl ||
		type == prmem_pool_wr_recl ||
		type == prmem_pool_ro_recl ||
		type == prmem_pool_rw_recl);
}

/*
 * is_pool_type_no_recl - check if memory from the pool cannot be reclaimed
 * @type: the pool type
 *
 * Returns false if the memory type is reclaimable, true otherwise
 */
static inline bool is_pool_type_no_recl(enum prmem_pool_type type) {
	return (type == prmem_pool_start_wr_no_recl ||
		type == prmem_pool_wr_no_recl ||
		type == prmem_pool_ro_no_recl);
}

/*
 * is_pool_type_wr_recl - is memory from the pool both rw and reclaimable
 * @type: the pool type
 *
 * Returns true if the memory type is both wr and reclaimable, else true
 */
static inline bool is_pool_type_wr_recl(enum prmem_pool_type type) {
	return (type == prmem_pool_start_wr_recl ||
		type == prmem_pool_wr_recl);
}

static inline bool is_pool_type_start_wr(enum prmem_pool_type type) {
	return (type == prmem_pool_start_wr_no_recl ||
		type == prmem_pool_start_wr_recl);
}

extern unsigned long prmem_pools_ranges[];

/*
 * pool_to_prmem_range_type() - Address range for a given pool
 * @pool: the pool for which to identify the vmalloc address range
 *
 * Returns the allocation type associated with the pool.
 *
 * Since some regions can have 0 size and collapse, the first one ending
 * at a certain address is the correct one, and the vector must be scanned
 * from its beginning.
 */
#ifdef CONFIG_HISI_PRMEM
static inline int pool_to_prmem_range_type(struct prmem_pool *pool)
{
	unsigned long p = (unsigned long)(uintptr_t)pool;
	unsigned long start = (unsigned long)(uintptr_t)__start_data_prmem_pools;
	unsigned long end = (unsigned long)(uintptr_t)__end_data_prmem_pools;
	unsigned long i;

	if (unlikely(p < start || end <= p))
		return -EINVAL;

	for (i = 0; i <= PRMEM_LAST_REGION; i++)
		if (p < prmem_pools_ranges[i])
			return i;
	return -EINVAL;
}
#else /* !defined(CONFIG_HISI_PRMEM) */
static inline int pool_to_prmem_range_type(struct prmem_pool *pool)
{
	return pool->pool_type;
}
#endif /* !defined(CONFIG_HISI_PRMEM) */

/*
 * is_mem_pmalloc() - is the buffer part of the pmalloc range?
 * @addr: the location of the buffer
 * @size: the size of the buffer
 *
 * Return true if the buffer is within pmalloc, false otherwise
 */
static inline bool is_mem_pmalloc(void *addr, size_t size)
{
	return prmem_in_range(addr, size, PRMEM_FULL);
}

/*
 * is_pool_recl() - is the memory associated to the pool reclaimable?
 * @pool: the pool to analyse
 *
 * Returns true if the memory associated to the pool can be released.
 */
static inline bool is_pool_recl(struct prmem_pool *pool)
{
	enum prmem_pool_type type = pool_to_prmem_range_type(pool);

	return (type == prmem_pool_ro_recl ||
		type == prmem_pool_wr_recl ||
		type == prmem_pool_start_wr_recl);
}

/*
 * is_pool_wr() - is the pool associated to protected rewritable memory?
 * @pool: the pool to analyse
 *
 * Returns true if the memory associated to the pool can be rewritten.
 */
static inline bool is_pool_wr(struct prmem_pool *pool)
{
	enum prmem_pool_type type = pool_to_prmem_range_type(pool);

	return (type == prmem_pool_start_wr_no_recl ||
		type == prmem_pool_start_wr_recl ||
		type == prmem_pool_wr_recl ||
		type == prmem_pool_wr_no_recl);
}

/*
 * is_pool_start_wr() - are pool allocations pre-protected and rewritable?
 * @pool: the pool to analyse
 *
 * Returns true if allocations are pre-protected and can be rewritten.
 */
static inline bool is_pool_start_wr(struct prmem_pool *pool)
{
	enum prmem_pool_type type = pool_to_prmem_range_type(pool);

	return (type == prmem_pool_start_wr_no_recl ||
		type == prmem_pool_start_wr_recl);
}
#endif
