/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: prmem-specific page management
 * Author: Igor Stoppa <igor.stoppa@huawei.com>
 * Creator: security-ap
 * Date: 2020/04/15
 */

#ifndef _LINUX_PRMEM_VMALLOC_H
#define _LINUX_PRMEM_VMALLOC_H

#include <linux/vmalloc.h>
#include <linux/hisi/prmem_cfg.h>
#include <linux/hisi/prmem_defs.h>
#include <linux/errno.h>

/*
 * is_prmem_type_recl - true if the memory type is reclaimable
 * @type - one of the possible memory types from enum prmem_types
 *
 * Returns true if the type provided is reclaimable
 */
static inline bool is_prmem_type_recl(enum prmem_type type)
{
	return (type == PRMEM_START_WR_RECL || type == PRMEM_WR_RECL ||
		type == PRMEM_RO_RECL || type == PRMEM_RECL);
}

/*
 * is_prmem_type_start_wr - true if the memory type starts as write-rare
 * @type - one of the possible memory types from enum prmem_types
 *
 * Returns true if the type provided is start_wr
 */
static inline bool is_prmem_type_start_wr(enum prmem_type type)
{
	return (type == PRMEM_START_WR_NO_RECL ||
		type == PRMEM_START_WR_RECL || type == PRMEM_START_WR);
}

/*
 * is_prmem_type_wr - true if the memory type supports write-rare
 * @type - one of the possible memory types from enum prmem_types
 *
 * Returns true if the type provided is write-rare
 */
static inline bool is_prmem_type_wr(enum prmem_type type)
{
	return (type == PRMEM_WR_NO_RECL ||
		type == PRMEM_START_WR_NO_RECL ||
		type == PRMEM_START_WR_RECL || type == PRMEM_WR_RECL ||
		type == PRMEM_WR);
}

/*
 * struct prmem_range - description of one of prmem protected subranges
 * @start: the address at which the associated range starts
 * @end: the address at which the associated range ends
 * @range_name: string (max 31 characters) associated with the range
 */
struct prmem_range {
	unsigned long start;
	unsigned long end;
	char range_name[32];
};

extern const struct prmem_range prmem_ranges[PRMEM_INDEX_NUM];

#ifdef CONFIG_HISI_PRMEM
static inline bool prmem_in_range(void *start, unsigned long size,
				  enum prmem_type type)
{
	unsigned long _start = (unsigned long)(uintptr_t)start;
	unsigned long end = _start + size;

	if (unlikely(type < 0 || type >= PRMEM_INDEX_NUM))
		return false;

	return ((prmem_ranges[type].start <= _start) &&
		(_start < end) && (end <= prmem_ranges[type].end));
}
#else /* !defined(CONFIG_HISI_PRMEM) */
static inline bool prmem_in_range(void *start, unsigned long size,
				  enum prmem_type type)
{
	return true;
}
#endif /* !defined(CONFIG_HISI_PRMEM) */

#ifdef CONFIG_HISI_PRMEM
struct vmap_area *prmem_find_vmap_area(unsigned long addr);
#else /* !defined(CONFIG_HISI_PRMEM) */
static inline struct vmap_area *prmem_find_vmap_area(unsigned long addr)
{
	return find_vmap_area(addr);
}
#endif /* !defined(CONFIG_HISI_PRMEM) */

/*
 * vmap_area_in_range() - is the vma within the specified PMALLOC range?
 * @va: pointer to the vma structure
 * @type: reference to the specified PRMEM range
 *
 * Returns true if the vma is contained within the range, false otherwise
 */
static inline bool vmap_area_in_range(struct vmap_area *va,
				      enum  prmem_type type)
{
	return prmem_in_range((void *)(uintptr_t)va->va_start,
			      va->va_end - va->va_start, type);
}

#ifdef CONFIG_HISI_PRMEM
static inline int prmem_get_type(void *start, unsigned long size)
{
	int i;

	for (i = 0; i <= PRMEM_LAST_REGION; i++)
		if (prmem_in_range(start, size, i))
			return i;
	return -EINVAL;
}
#else /* !defined(CONFIG_HISI_PRMEM) */
static inline int prmem_get_type(void *start, unsigned long size)
{
	struct vmap_area *va = NULL;

	va = find_vmap_area((unsigned long)(uintptr_t)start);
	return va->pool->pool_type;
}
#endif /* !defined(CONFIG_HISI_PRMEM) */

/*
 * pmalloc_is_reclaimable() - can the memory buffer be freed?
 * @start: address of the buffer
 * @size: dimension of the buffer
 *
 * Returns true if the memory can be released, false otherwise
 */
static inline bool pmalloc_is_reclaimable(void *start, size_t size)
{
	return prmem_in_range(start, size, PRMEM_RECL);
}

/*
 * vmap_area_is_reclaimable() - can the vma be freed?
 * @va: pointer to the vma structure
 *
 * Returns true if the vma can be released, false otherwise
 */
static inline bool vmap_area_is_reclaimable(struct vmap_area *va)
{
	return vmap_area_in_range(va, PRMEM_RECL);
}

/*
 * prmem_is_wr() - is the memory buffer wr?
 * @start: address of the buffer
 * @size: dimension of the buffer
 *
 * Returns true if the memory is wr, false otherwise
 */
static inline bool prmem_is_wr(struct vmap_area *start, size_t size)
{
	return prmem_in_range(start, size, PRMEM_WR);
}

/*
 * vmap_area_is_wr() - is the vma wr?
 * @va: pointer to the vma structure
 *
 * Returns true if the vma is wr, false otherwise
 */
static inline bool vmap_area_is_wr(struct vmap_area *va)
{
	return vmap_area_in_range(va, PRMEM_WR);
}

/*
 * pmalloc_is_start_wr() - is the buffer wr since its allocation?
 * @start: address of the buffer
 * @size: dimension of the buffer
 *
 * Returns true if the memory was created as wr, false otherwise
 */
static inline bool pmalloc_is_start_wr(void *start, size_t size)
{
	return prmem_in_range(start, size, PRMEM_START_WR);
}

/*
 * vmap_area_is_start_wr() - is the vma wr since its allocation?
 * @va: pointer to the vma structure
 *
 * Returns true if the vma was created as wr, false otherwise
 */
static inline bool vmap_area_is_start_wr(struct vmap_area *va)
{
	return vmap_area_in_range(va, PRMEM_START_WR);
}

/*
 * pmalloc_is_rw() - is the memory buffer rw?
 * @start: address of the buffer
 * @size: dimension of the buffer
 *
 * Returns true if the memory is rw, false otherwise
 */
static inline bool pmalloc_is_rw(void *start, size_t size)
{
	return prmem_in_range(start, size, PRMEM_RW_RECL);
}

/*
 * vmap_area_is_rw() - is the vma rw?
 * @va: pointer to the vma structure
 *
 * Returns true if the vma is rw, false otherwise
 */
static inline bool vmap_area_is_rw(struct vmap_area *va)
{
	return vmap_area_in_range(va, PRMEM_RW_RECL);
}

/*
 * mem_is_pmalloc() - can the memory buffer be protected?
 * @start: address of the buffer
 * @size: dimension of the buffer
 *
 * Returns true if the memory can be protected, false otherwise
 */
static inline bool mem_is_pmalloc(void *start, size_t size)
{
	return prmem_in_range(start, size, PRMEM_FULL);
}

/*
 * vmap_area_is_prmem() - can the vma be protected?
 * @va: pointer to the vma structure
 *
 * Returns true if the vma can be protected, false otherwise
 */
static inline bool vmap_area_is_prmem(struct vmap_area *va)
{
	return vmap_area_in_range(va, PRMEM_FULL);
}

#ifdef CONFIG_HISI_PRMEM
struct vmap_area *prmem_vmap_area(unsigned long size, enum prmem_type type);
void pvfree(const void *addr);
int set_prmem_ro(unsigned long addr, int numpages);
int set_prmem_rw(unsigned long addr, int numpages);
#else
/*
 * The custom backend of prmem is nothing more than vmalloc() / vfree()
 * in a custom sub address range.
 * Taking that away, defaults to the basic version of each function.
 */
static inline struct vmap_area *prmem_vmap_area(unsigned long size,
						enum prmem_type type)
{
	unsigned long addr;

	addr = (unsigned long)(uintptr_t)vmalloc(size);
	if (!addr)
		return NULL;
	return find_vmap_area(addr);
}

static inline void pvfree(const void *addr)
{
	vfree(addr);
}
static inline int set_prmem_ro(unsigned long addr, int numpages)
{
	return numpages;
}

static inline int set_prmem_rw(unsigned long addr, int numpages)
{
	return numpages;
}
#endif

#endif
