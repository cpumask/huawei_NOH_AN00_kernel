/*
 * drivers/staging/android/uapi/ion.h
 *
 * Copyright (C) 2011 Google, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef _UAPI_LINUX_ION_H
#define _UAPI_LINUX_ION_H

#include <linux/ioctl.h>
#include <linux/types.h>

/**
 * enum ion_heap_types - list of all possible types of heaps
 * @ION_HEAP_TYPE_SYSTEM:	 memory allocated via vmalloc
 * @ION_HEAP_TYPE_SYSTEM_CONTIG: memory allocated via kmalloc
 * @ION_HEAP_TYPE_CARVEOUT:	 memory allocated from a prereserved
 *				 carveout heap, allocations are physically
 *				 contiguous
 * @ION_HEAP_TYPE_DMA:		 memory allocated via DMA API
 * @ION_NUM_HEAPS:		 helper for iterating over heaps, a bit mask
 *				 is used to identify the heaps, so only 32
 *				 total heap types are supported
 */
enum ion_heap_type {
	ION_HEAP_TYPE_SYSTEM,
	ION_HEAP_TYPE_SYSTEM_CONTIG,
	ION_HEAP_TYPE_CARVEOUT,
	ION_HEAP_TYPE_CHUNK,
	ION_HEAP_TYPE_DMA,
	ION_HEAP_TYPE_DMA_POOL,
	ION_HEAP_TYPE_CPUDRAW,
	ION_HEAP_TYPE_IOMMU,
	ION_HEAP_TYPE_SECCM,
	ION_HEAP_TYPE_SEC_CONTIG,
	ION_HEAP_TYPE_SECSG,
	ION_HEAP_TYPE_CPA,
	ION_HEAP_TYPE_CUSTOM, /*
			       * must be last so device specific heaps always
			       * are at the end of this enum
			       */
};

#define ION_HEAP_TYPE_DMA_POOL_MASK	(1 << ION_HEAP_TYPE_DMA_POOL)
#define ION_HEAP_CPUDRAW_MASK		(1 << ION_HEAP_TYPE_CPUDRAW)
#define ION_HEAP_TYPE_IOMMU_MASK	(1 << ION_HEAP_TYPE_IOMMU)

#define ION_NUM_HEAP_IDS		(sizeof(unsigned int) * 8)

/**
 * allocation flags - the lower 16 bits are used by core ion, the upper 16
 * bits are reserved for use by the heaps themselves.
 */

/*
 * mappings of this buffer should be cached, ion will do cache maintenance
 * when the buffer is mapped for dma
 */
#define ION_FLAG_CACHED 1

/*
 * mappings of this buffer will created at mmap time, if this is set
 * caches must be managed manually
 */
#define ION_FLAG_CACHED_NEEDS_SYNC (0x1 << 1)
#define ION_FLAG_NOT_ZERO_BUFFER (0x1 << 2)	 /* do not zero buffer*/
#define ION_FLAG_SECURE_BUFFER (0x1 << 3)
#define ION_FLAG_GRAPHIC_BUFFER (0x1 << 4)
#define ION_FLAG_GRAPHIC_GPU_BUFFER (0x1 << 5)
#define ION_FLAG_ALLOC_NOWARN_BUFFER (0x1 << 6)
#define ION_FLAG_NO_SHRINK_BUFFER (0x1 << 7)
#define ION_FLAG_SMMUV3_BUFFER (0x1 << 8)
#define ION_FLAG_DRM_HEAP_ONLY (0x1 << 9)
#ifdef CONFIG_HISI_LB
#define ION_FLAG_HISI_LB_PLC_S    (0x1UL << 12)
#define ION_FLAG_HISI_LB_PLC_E    (0x1UL << 19)
#define ION_FLAG_HISI_LB_SHIFT    12
#define ION_FLAG_HISI_LB_MASK     (0xffUL << 12)

#define ION_FLAG_2_PLC_ID(flags) \
	((flags & ION_FLAG_HISI_LB_MASK) >> ION_FLAG_HISI_LB_SHIFT)
#endif

/**
 * DOC: Ion Userspace API
 *
 * create a client by opening /dev/ion
 * most operations handled via following ioctls
 *
 */

/**
 * struct ion_allocation_data - metadata passed from userspace for allocations
 * @len:		size of the allocation
 * @heap_id_mask:	mask of heap ids to allocate from
 * @flags:		flags passed to heap
 * @handle:		pointer that will be populated with a cookie to use to
 *			refer to this allocation
 *
 * Provided by userspace as an argument to the ioctl
 */
struct ion_allocation_data {
	__u64 len;
	__u32 heap_id_mask;
	__u32 flags;
	__u32 fd;
	__u32 unused;
};

#define MAX_HEAP_NAME			32

/**
 * struct ion_heap_data - data about a heap
 * @name - first 32 characters of the heap name
 * @type - heap type
 * @heap_id - heap id for the heap
 */
struct ion_heap_data {
	char name[MAX_HEAP_NAME];
	__u32 type;
	__u32 heap_id;
	__u32 reserved0;
	__u32 reserved1;
	__u32 reserved2;
};

/**
 * struct ion_heap_query - collection of data about all heaps
 * @cnt - total number of heaps to be copied
 * @heaps - buffer to copy heap data
 */
struct ion_heap_query {
	__u32 cnt; /* Total number of heaps to be copied */
	__u32 reserved0; /* align to 64bits */
	__u64 heaps; /* buffer to be populated */
	__u32 reserved1;
	__u32 reserved2;
};

#define ION_IOC_MAGIC		'I'

/**
 * DOC: ION_IOC_ALLOC - allocate memory
 *
 * Takes an ion_allocation_data struct and returns it with the handle field
 * populated with the opaque handle for the allocation.
 */
#define ION_IOC_ALLOC		_IOWR(ION_IOC_MAGIC, 0, \
				      struct ion_allocation_data)

/**
 * DOC: ION_IOC_HEAP_QUERY - information about available heaps
 *
 * Takes an ion_heap_query structure and populates information about
 * available Ion heaps.
 */
#define ION_IOC_HEAP_QUERY     _IOWR(ION_IOC_MAGIC, 8, \
					struct ion_heap_query)

#endif /* _UAPI_LINUX_ION_H */
