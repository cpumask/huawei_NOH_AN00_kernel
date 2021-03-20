/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2008-2019. All rights reserved.
 * Description: The common data type defination
 * Author: z44949
 * Create: 2008-10-31
 */
#ifndef __HI_DRV_MEM_H__
#define __HI_DRV_MEM_H__

#include <linux/seq_file.h>
#include <linux/vmalloc.h>
#include <linux/hisi/hisi_ion.h>
#include <linux/dma-buf.h>
#include <linux/slab.h>

#include "hi_type.h"
#include "drv_venc_ioctl.h"
#include "drv_venc_osal.h"

#define MAX_MEM_NAME_LEN       15
#define MAX_KMALLOC_MEM_NODE   16    /* 1 channel need 2 node ,there is have max 8 channels */
#define MAX_ION_MEM_NODE       200
#define SMMU_RWERRADDR_SIZE    128

typedef struct {
	HI_U64 read_addr;
	HI_U64 write_addr;
	HI_U32 size;
} venc_smmu_err_add_t;

typedef struct {
	HI_VOID  *start_virt_addr;
	HI_U64   start_phys_addr;
	HI_U32   size;
	HI_U8    is_mapped;
	HI_S32   share_fd;
} mem_buffer_t;

typedef struct  {
	HI_CHAR            node_name[MAX_MEM_NAME_LEN];
	HI_CHAR            zone_name[MAX_MEM_NAME_LEN];
	HI_VOID           *virt_addr;
	HI_U64             phys_addr;
	HI_U32             size;
	HI_S32             shared_fd;
	struct ion_handle *handle;
	struct dmabuf    *dmabuf;
} venc_mem_buf_t;

struct mem_info {
	void *virt_addr;
	HI_U64 iova_addr;
};

struct mem_pool {
	spinlock_t *lock;
	struct device *dev;
	vedu_osal_event_t event;

	void *start_virt_addr;
	HI_U64 start_phy_addr;

	void *aligned_virt_addr;
	HI_U64 aligned_iova_addr;

	HI_U32 num;
	HI_U32 each_size;
	HI_U32 align_size;
	DECLARE_KFIFO_PTR(fifo, struct mem_info);
};

HI_S32 drv_mem_copy_from_user(HI_U32 cmd, const void __user *user_arg, HI_VOID **kernel_arg);
HI_S32 drv_mem_init(void);
HI_S32 drv_mem_exit(void);
HI_S32 drv_mem_kalloc(const HI_CHAR *bufname, const HI_CHAR *zone_name, mem_buffer_t *psmbuf);
HI_S32 drv_mem_kfree(const mem_buffer_t *psmbuf);
HI_S32 drv_mem_map_kernel(HI_S32 shared_fd, mem_buffer_t *psmbuf);
HI_S32 drv_mem_unmap_kernel(mem_buffer_t *psmbuf);
HI_S32 drv_mem_get_map_info(HI_S32 shared_fd, mem_buffer_t *psmbuf);
HI_S32 drv_mem_put_map_info(mem_buffer_t *psmbuf);
HI_S32 drv_mem_iommumap(venc_buffer_record_t *node, struct platform_device *pdev);
HI_S32 drv_mem_iommuunmap(HI_S32 shared_fd, HI_S32 phys_addr, struct platform_device *pdev);

struct mem_pool *drv_mem_create_pool(struct device *dev, HI_U32 each_size, HI_U32 num, HI_U32 align_size);
void drv_mem_destory_pool(struct mem_pool *pool);
void *drv_mem_pool_alloc(struct mem_pool *pool, HI_U64 *iova_addr);
void drv_mem_pool_free(struct mem_pool *pool, void *virt_addr, HI_U64 iova_addr);

#endif /* __HI_DRV_MEM_H__ */

