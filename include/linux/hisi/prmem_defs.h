/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Definitions and enums for memory protection library
 * Author: Igor Stoppa <igor.stoppa@huawei.com>
 * Creator: security-ap
 * Date: 2020/04/15
 */

#ifndef _LINUX_PRMEM_DEFS_H
#define _LINUX_PRMEM_DEFS_H

#include <asm/sizes.h>

/*
 * Optimizations on the order of the vmalloc regions:
 * - First the 3 regions which cannot be reclaimed, to have a simple test
 *   on free_vmap_area, thus making contiguous the reclaimable ranges
 * - The 3 zones which are write-rare compatible must be contiguous, to
 *   have one single test encompassing them all.
 *
 *   The resulting layout is:
 *   - READ-ONLY NOT RECLAIMABLE
 *   - WRITE-RARE NOT RECLAIMABLE
 *   - PRE-PROTECTED WRITE-RARE NOT RECLAIMABLE
 *   - PRE-PROTECTED WRITE-RARE RECLAIMABLE
 *   - WRITE-RARE RECLAIMABLE
 *   - READ-ONLY RECLAIMABLE
 *   - READ-WRITE RECLAIMABLE	<---- Not for use in production code!
 *
 *   NB: The purpose of the last region is to have prmem-like code which
 *   doesn't actually write-protect the associated memory.
 *   This can be useful both when in the process of converting non-prmem
 *   code to prmem and if in need to temporarily disable prmem protection
 *   on a specific pool.
 */

#define PRMEM_RO_NO_RECL_SIZE           SZ_128M
#define PRMEM_WR_NO_RECL_SIZE           SZ_128M
#define PRMEM_START_WR_NO_RECL_SIZE     SZ_128M
#define PRMEM_START_WR_RECL_SIZE        SZ_128M
#define PRMEM_WR_RECL_SIZE              SZ_128M
#define PRMEM_RO_RECL_SIZE              SZ_128M
#define PRMEM_RW_RECL_SIZE              SZ_128M

/*
 * The whole region reserved for PMALLOC addresses is located at the
 * beginning of the VMALLOC address space.
 *
 * The 6 basic protectable regions + the regular non protectable one: */
/* 1st region: Read-Only Non-Reclaimable */
#define PRMEM_RO_NO_RECL_START \
	(PRMEM_RO_NO_RECL_END - PRMEM_RO_NO_RECL_SIZE)
#define PRMEM_RO_NO_RECL_END   PRMEM_WR_NO_RECL_START

/* 2rd region: Write-rare Non-Reclaimable */
#define PRMEM_WR_NO_RECL_START \
	(PRMEM_WR_NO_RECL_END - PRMEM_WR_NO_RECL_SIZE)
#define PRMEM_WR_NO_RECL_END   PRMEM_START_WR_NO_RECL_START

/* 3nd region: Pre-protected Write-rare Non-Reclaimable */
#define PRMEM_START_WR_NO_RECL_START \
	(PRMEM_START_WR_NO_RECL_END - PRMEM_START_WR_NO_RECL_SIZE)
#define PRMEM_START_WR_NO_RECL_END   PRMEM_START_WR_RECL_START

/* 4th region: Pre-protected Write-rare Reclaimable */
#define PRMEM_START_WR_RECL_START \
	(PRMEM_START_WR_RECL_END - PRMEM_START_WR_RECL_SIZE)
#define PRMEM_START_WR_RECL_END   PRMEM_WR_RECL_START

/* 5th region: Write-rare Reclaimable */
#define PRMEM_WR_RECL_START \
	(PRMEM_WR_RECL_END - PRMEM_WR_RECL_SIZE)
#define PRMEM_WR_RECL_END   PRMEM_RO_RECL_START

/* 6th region: Read-Only Reclaimable */
#define PRMEM_RO_RECL_START \
	(PRMEM_RO_RECL_END - PRMEM_RO_RECL_SIZE)
#define PRMEM_RO_RECL_END   PRMEM_RW_RECL_START

/* 7th region: Read-Write, Reclaimable */
#define PRMEM_RW_RECL_START \
	(PRMEM_RW_RECL_END - PRMEM_RW_RECL_SIZE)
#define PRMEM_RW_RECL_END   PRMEM_END

/* The 4 combined regions (they partially overlap): */
/*
 * 1st combined region: Reclaimable
 * (PRMEM_START_WR_RECL + PRMEM_WR_RECL +
 *  PRMEM_RO_RECL + PRMEM_RW_RECL)
 */
#define PRMEM_RECL_START PRMEM_START_WR_RECL_START
#define PRMEM_RECL_END PRMEM_RW_RECL_END

/*
 * 2nd combined region: Non Reclaimable
 * (PRMEM_RO_NO_RECL + PRMEM_WR_NO_RECL + PRMEM_START_WR_NO_RECL)
 */
#define PRMEM_NO_RECL_START PRMEM_RO_NO_RECL_START
#define PRMEM_NO_RECL_END PRMEM_START_WR_NO_RECL_END

/*
 * 3rd combined region: Write-Rare
 * (PRMEM_WR_NO_RECL + PRMEM_START_WR_NO_RECL +
 *  PRMEM_START_WR_RECL + PRMEM_WR_RECL)
 */
#define PRMEM_WR_START PRMEM_WR_NO_RECL_START
#define PRMEM_WR_END PRMEM_WR_RECL_END

/*
 * 4th combined region: Start-Write-Rare
 * (PRMEM_START_WR_NO_RECL + PRMEM_START_WR_RECL)
 */
#define PRMEM_START_WR_START PRMEM_START_WR_NO_RECL_START
#define PRMEM_START_WR_END PRMEM_START_WR_RECL_END

/*
 * 5th combined region: all the protectable regions
 * PRMEM_START is defined by arch-specific code
 */
#ifdef CONFIG_HISI_PRMEM
#define PRMEM_START PRMEM_RO_NO_RECL_START
#else
#define PRMEM_START PRMEM_END
#endif
#define PRMEM_SIZE (PRMEM_END - PRMEM_START)

#define PRMEM_FULL_START PRMEM_START
#define PRMEM_FULL_END PRMEM_END

#ifndef __ASSEMBLY__

#include <linux/spinlock_types.h>

/* ---------------------------- Pool Types ---------------------------- */
/*
 * enum prmem_type - selects sub-ranges of VMALLOC addresses
 * @PRMEM_RO_NO_RECL: R/O allocations, non reclaimable
 * @PRMEM_WR_NO_RECL: W/R allocations, non reclaimable
 * @PRMEM_START_WR_NO_RECL: pre-protected W/R allocations, non reclaimable
 * @PRMEM_START_WR_RECL: pre-protected W/R allocations, reclaimable
 * @PRMEM_WR_RECL: W/R allocations, reclaimable
 * @PRMEM_RO_RECL: R/O allocations, reclaimable
 * @PRMEM_LAST_PROT_REGION: index of the last write-protected range
 * @PRMEM_RW_RECL: R/W allocations, reclaimable
 * @PRMEM_LAST_REGION: index of the last PMALLOC range
 * @PRMEM_NO_RECL: combined range of non reclaimable ranges
 * @PRMEM_RECL: combined range of reclaimable ranges
 * @PRMEM_WR: combined range of W/R ranges
 * @PRMEM_START_WR: combined range of start W/R ranges
 * @PRMEM_FULL: combined range of protected ranges
 * @PRMEM_INDEX_NUM: number of usable indexes
 *
 * The first 6 indexes refer to 6 memory areas used for allocating
 * protected memory
 * WARNING: this *MUST* be kept in sync with "prmem_ranges"
 */
 enum prmem_type {
	PRMEM_RO_NO_RECL,
	PRMEM_WR_NO_RECL,
	PRMEM_START_WR_NO_RECL,
	PRMEM_START_WR_RECL,
	PRMEM_WR_RECL,
	PRMEM_RO_RECL,
	PRMEM_LAST_PROTECTED_REGION = PRMEM_RO_RECL,
	PRMEM_RW_RECL,
	PRMEM_LAST_REGION = PRMEM_RW_RECL,
	/* The following indexes refer to unions of the above. */
	PRMEM_NO_RECL,
	PRMEM_RECL,
	PRMEM_WR,
	PRMEM_START_WR,
	PRMEM_FULL,
	PRMEM_INDEX_NUM,
};

/* ------------------------------ Pools ------------------------------ */
/*
 * enum prmem_pool_type - select type of allocation associated to a pool
 * @prmem_pool_ro_no_recl: non-rewritable, non-reclaimable
 * @prmem_pool_wr_no_recl: rewritable, non-reclaimable
 * @prmem_pool_start_wr_no_recl: start protected, rewritable, non-recl
 * @prmem_pool_start_wr_recl: start protected, rewritable, reclaimable
 * @prmem_pool_wr_recl: rewritable, reclaimable
 * @prmem_pool_ro_recl: non-rewritable, reclaimable
 * @prmem_pool_rw_recl: read-write, reclaimable
 * @prmem_pool_type_num: cap value, useful for iterating over the range
 *
 * These enums define in which segment to allocate a specific instance of
 * a pool structure. Depending on the segment choosen, certain properties
 * will be atuomatically associated to the memory allocated in relation to
 * the specific pool structure.
 * The initialization ensures that the enum indexes are matching the values
 * from the corresponding indexes declared in the enum prmem_type, in
 * the file prmem_vmalloc.h
 */
enum prmem_pool_type {
	prmem_pool_ro_no_recl = PRMEM_RO_NO_RECL,
	prmem_pool_wr_no_recl = PRMEM_WR_NO_RECL,
	prmem_pool_start_wr_no_recl = PRMEM_START_WR_NO_RECL,
	prmem_pool_start_wr_recl = PRMEM_START_WR_RECL,
	prmem_pool_wr_recl = PRMEM_WR_RECL,
	prmem_pool_ro_recl = PRMEM_RO_RECL,
	prmem_pool_rw_recl = PRMEM_RW_RECL,
	prmem_pool_type_num,
};

/*
 * struct vmap_area_node - hardened node for tracking pvamap allocations
 * @next: pointer to the next hardened node
 * @va: pointer to the va the node is tracking
 */
struct vmap_area_node {
	struct vmap_area_node *next;
	struct vmap_area *va;
	size_t va_start;
	size_t va_size;
};

/*
 * struct prmem_pool - Object tracking allocations
 * @name: pointer to a string with the name of the pool
 * @pmalloc_list: pointer to the first node of a list of va allocations
 * @off_pmalloc_list: list of va allocations used in prmem-off mode
 * @recl_va_list: list of reclaimable va in the pool (on-mode)
 * @no_recl_va_list: list of non-reclaimable va in the pool (on-mode)
 * @off_va_list: list of reclaimable va in the pool (off-mode)
 * @offset: amount of free memory available from the current va
 * @align: the minimum alignment required for allocations from this pool
 * @refill: the minimum amount of memory to allocate in a new vmap_area
 *
 * The pool defines - implicitly, see PRMEM_POOL() - the properties of
 * the allocations associated with it.
 * Once a pool is created, it can provide only one specific type of
 * memory. This limitation is intended to limit the attack surface against
 * ROP/JOP attempts to hijack a pool in a different way than the intended
 * one.
 * The field @pool_type is used only for prmem-off mode, when address of
 * the pool structure is not enforced by prmem.
 * Instances of this type of structure should be created only through
 * PRMEM_POOL(), rather than directly.
 */
struct prmem_pool {
	const char *name;
	struct vmap_area_node *pmalloc_list;
	struct vmap_area_node *recl_va_list;
	struct vmap_area_node *no_recl_va_list;
	size_t offset;
	size_t align;
	size_t refill;
	size_t cap;
	size_t allocated;
	spinlock_t *lock;
	bool pmalloc_protected;
	struct prmem_pool_sysfs_data *sysfs_data;
#ifndef CONFIG_HISI_PRMEM
	enum prmem_pool_type pool_type;
#endif
} __attribute__((aligned(sizeof(void *))));

/*
 * enum prmem_flags - set of options to use when allocating from prmem
 * @PRMEM_NO_FLAGS: alias for a 0 bitmap
 * @PRMEM_ZERO: memory must be zeroed
 * @PRMEM_ATOMIC: do not allocate a new va, rather return NULL
 * @PRMEM_FREEABLE: va can be disposed independently from the pool
 *
 * Atomic allocation satisfy the need for avoiding sleeping in atomic
 * context, but can be also useful to verify that whatever amount of
 * memory had been pre-allocated is sufficient.
 * It applies only to allocation operations based either on pmalloc or on
 * a cache.
 *
 * Freeable allocations are exclusively referring to va that are managed
 * independently from the pmalloc memory. Such va can be optionally
 * freed before the pool is trashed. It is still required that the
 * associated pool is made of reclaimable memory.
 */
enum prmem_flags {
	PRMEM_NO_FLAGS = (1UL << 0),
	PRMEM_ZERO = (1UL << 1),
	PRMEM_ATOMIC = (1UL << 2),
	/* The following values are for internal use. */
	PRMEM_VA = (1UL << 3),
	PRMEM_FREEABLE = (1UL << 4),
	PRMEM_FREEABLE_VA = PRMEM_VA | PRMEM_FREEABLE,
};

/* -------------------------- Object Cache -------------------------- */
/*
 * struct stack_node - structure used to stack items in an object cache
 * @node: the pointer to thenext object in the cache
 */
struct stack_node {
	struct stack_node *node;
};

/*
 * struct prmem_cache - cache of objects of wr memory
 * @node: pointer to the first object in the stack implementing the cache
 * @pool: pool from which to allocate additional memory, as needed
 * @size: the size of one of the objects in the cache
 * @align_ord: the minimum alignemnt order required - it's log2(alignment)
 * @lock: lock used when pushing/popping objects
 */
struct prmem_cache {
	const char *name;
	struct stack_node *node __attribute__((aligned(sizeof(void *))));
	size_t count;
	struct prmem_pool *pool __attribute__((aligned(sizeof(void *))));
	u16 size;
	u16 align_ord;
	u16 elements;
	spinlock_t *lock;
	bool prot;
	struct prmem_cache_sysfs_data *sysfs_data;
} __attribute__((aligned(sizeof(void *))));

#endif /* __ASSEMBLY__ */
#endif
