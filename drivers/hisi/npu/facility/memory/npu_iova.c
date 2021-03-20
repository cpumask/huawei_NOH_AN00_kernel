/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2018. All rights reserved.
 * Description:
 */
#include "npu_iova.h"

#include <linux/err.h>
#include <linux/iommu.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/mutex.h>
#include <linux/hisi-iommu.h>
#include <linux/vmalloc.h>
#include <linux/scatterlist.h>
#include <linux/dma-buf.h>
#include <asm/cacheflush.h>

#include "npu_platform.h"
#include "npu_log.h"
#include "hisi_smmu.h"

struct t_sg_table_va_map {
	struct list_head list;
	struct sg_table *table;
	int nents;
	unsigned long long virt_addr;
	unsigned long size;
};

struct t_sg_table_va_map_manager {
	struct mutex lock;
	struct list_head sg_table_va_list;
	u8 is_init;
};

static struct t_sg_table_va_map_manager g_sg_table_va_map_manager = {
	.is_init = 0,
};

static int sg_table_va_map_init()
{
	// initial sa_table and g_sg_table_va_map_manager
	mutex_init(&g_sg_table_va_map_manager.lock);
	g_sg_table_va_map_manager.is_init = 1;
	INIT_LIST_HEAD(&g_sg_table_va_map_manager.sg_table_va_list);
	return 0;
}
static int sg_table_va_map_insert(struct sg_table *table, int nents, unsigned long long va, unsigned long size)
{
	struct t_sg_table_va_map *p_sg_table_va_map = NULL;

	if (!g_sg_table_va_map_manager.is_init)
		sg_table_va_map_init();
	p_sg_table_va_map = kmalloc(sizeof(struct t_sg_table_va_map), GFP_KERNEL);
	COND_RETURN_ERROR(p_sg_table_va_map == NULL, -ENOMEM, "no mem to alloc sg_table_va_map\n");

	p_sg_table_va_map->table = table;
	p_sg_table_va_map->nents = nents;
	p_sg_table_va_map->virt_addr = va;
	p_sg_table_va_map->size = size;
	mutex_lock(&g_sg_table_va_map_manager.lock);
	list_add_tail(&p_sg_table_va_map->list, &g_sg_table_va_map_manager.sg_table_va_list);
	mutex_unlock(&g_sg_table_va_map_manager.lock);
	return 0;
}

static int sg_table_va_map_remove(unsigned long long va)
{
	struct t_sg_table_va_map *p_sg_table_va_map = NULL;
	struct list_head *pos = NULL;
	struct list_head *n = NULL;
	int ret = -1;

	if (!g_sg_table_va_map_manager.is_init) {
		NPU_DRV_ERR("sg table & va map has not init(never alloc)\n");
		return -1;
	}
	mutex_lock(&g_sg_table_va_map_manager.lock);
	list_for_each_safe(pos, n, &g_sg_table_va_map_manager.sg_table_va_list) {
		p_sg_table_va_map = list_entry(pos, struct t_sg_table_va_map, list);
		if (p_sg_table_va_map == NULL) {
			NPU_DRV_ERR("p_sg_table_va_map is null\n");
			ret = -1;
			break;
		}
		if(p_sg_table_va_map->virt_addr == va) {
			list_del(&p_sg_table_va_map->list);
			kfree(p_sg_table_va_map);
			ret = 0;
			break;
		}
	}
	mutex_unlock(&g_sg_table_va_map_manager.lock);
	return ret;
}

static int sg_table_va_map_find(struct sg_table **table, int *nents, unsigned long *size, unsigned long long va)
{
	struct t_sg_table_va_map *p_sg_table_va_map = NULL;
	struct list_head *pos = NULL;
	struct list_head *n = NULL;
	int ret = -1;

	if (!g_sg_table_va_map_manager.is_init) {
		NPU_DRV_ERR("sg table & va map has not init(never alloc)\n");
		return -1;
	}
	mutex_lock(&g_sg_table_va_map_manager.lock);
	list_for_each_safe(pos, n, &g_sg_table_va_map_manager.sg_table_va_list) {
		p_sg_table_va_map = list_entry(pos, struct t_sg_table_va_map, list);
		if (p_sg_table_va_map == NULL) {
			NPU_DRV_ERR("p_sg_table_va_map is null\n");
			ret = -1;
			break;
		}
		if(p_sg_table_va_map->virt_addr == va) {
			if (table != NULL)
				*table = p_sg_table_va_map->table;
			if (nents != NULL)
				*nents = p_sg_table_va_map->nents;
			if (size != NULL)
				*size = p_sg_table_va_map->size;
			ret = 0;
			break;
		}
	}
	mutex_unlock(&g_sg_table_va_map_manager.lock);
	return ret;
}

int devdrv_hisi_npu_iova_alloc(unsigned long long *virt_addr, unsigned long size)
{
	int i;
	int nents;
	int ret;
	struct sg_table *table = NULL;
	struct scatterlist *sg = NULL;
	struct page *page = NULL;
	struct page **pages = NULL;

	NPU_DRV_DEBUG("devdrv_hisi_npu_iova_alloc start \n");
	COND_RETURN_ERROR(virt_addr == NULL || size == 0, -1, "invalid params!size=%lu", size);

	nents = PAGE_ALIGN(size) / PAGE_SIZE;
	pages = vmalloc(sizeof(struct page *) * nents);
	COND_RETURN_ERROR(pages == NULL, -1, "alloc pages failed!");

	table = kmalloc(sizeof(struct sg_table), GFP_KERNEL);
	COND_GOTO_ERROR(table == NULL, free_pages, ret, -1, "alloc table failed!");

	ret = sg_alloc_table(table, nents, GFP_KERNEL);
	COND_GOTO_ERROR(ret != 0 || table->sgl == NULL, free_table, ret, -1, "sg_alloc_table failed!");

	sg = table->sgl;
	for (i = 0; i < nents; i++) {
		page = alloc_page(GFP_KERNEL);
		COND_GOTO_ERROR(page == NULL, free_page, ret, -1, "alloc_page failed!");
		sg_set_page(sg, page, PAGE_SIZE, 0);
		sg = sg_next(sg);

		pages[i] = page;
	}

	sg = table->sgl;
	*virt_addr = (unsigned long long)(uintptr_t)vmap(pages, nents, VM_MAP, pgprot_writecombine(PAGE_KERNEL));

	NPU_DRV_DEBUG("%s: va 0x%lx\n", __func__, (uintptr_t)*virt_addr);
	if (*virt_addr) {
		// Ensure that any D-cache lines for the interval are invalidated.
		__inval_dcache_area((void *)(uintptr_t)(*virt_addr), PAGE_ALIGN(size));
		// Save the mapping table of sg_table and va, used to release memory
		ret = sg_table_va_map_insert(table, nents, *virt_addr, size);
		if (ret == 0) {
			goto free_pages;
		} else {
			NPU_DRV_ERR("sg_table_va_map_insert fail ret = %d\n", ret);
			vunmap((const void *)(uintptr_t)(*virt_addr));
		}
	} else {
		ret = -1;
	}

free_page:
	nents = i;
	for_each_sg(table->sgl, sg, nents, i) {
		page = sg_page(sg);
		__free_page(page);
	}
	sg_free_table(table);
free_table:
	kfree(table);
	table = NULL;
free_pages:
	vfree(pages);
	pages = NULL;
	return ret;
}

void devdrv_hisi_npu_iova_free(unsigned long long virt_addr)
{
	struct sg_table *table = NULL;
	struct scatterlist *sg = NULL;
	struct page *page = NULL;
	int num = 6;
	int nents;
	int ret;

	NPU_DRV_DEBUG("devdrv_hisi_npu_iova_free start \n");

	ret = sg_table_va_map_find(&table, &num, NULL, virt_addr);
	if (ret || !table) {
		NPU_DRV_ERR("your virt_addr should be called by devdrv_hisi_npu_iova_alloc\n");
		return;
	}

	sg = table->sgl;
	// 1. unmap va
	vunmap((const void *)(uintptr_t)virt_addr);

	// 2. free sg_table
	nents = num;
	for_each_sg(table->sgl, sg, nents, num) {
		page = sg_page(sg);
		__free_page(page);
	}
	sg_free_table(table);
	kfree(table);
	table = NULL;
	NPU_DRV_DEBUG("%s %d\n", __func__, __LINE__);
	// 3. delete map node
	sg_table_va_map_remove(virt_addr);
	return;
}

int devdrv_hisi_npu_iova_map(unsigned long long virt_addr, unsigned long *iova)
{
	struct sg_table *table = NULL;
	struct scatterlist *sg = NULL;
	unsigned long size;
	int ret;
	struct devdrv_platform_info *platform_info = NULL;

	NPU_DRV_DEBUG("devdrv_hisi_npu_iova_map start \n");
	if (iova == NULL) {
		NPU_DRV_ERR("iova is null\n");
		return -1;
	}

	platform_info = devdrv_plat_get_info();
	if (platform_info == NULL) {
		NPU_DRV_ERR("devdrv_plat_get_info failed\n");
		return -1;
	}

	NPU_DRV_DEBUG("%s %d\n", __func__, __LINE__);
	ret = sg_table_va_map_find(&table, NULL, &size, virt_addr);
	if (ret != 0 || table == NULL) {
		NPU_DRV_ERR("your virt_addr should be called by devdrv_hisi_npu_iova_alloc\n");
		return -1;
	}

	sg = table->sgl;

	*iova = hisi_iommu_map_sg(DEVDRV_PLAT_GET_PSMMUDEV(platform_info), sg, 0, &size);
	NPU_DRV_DEBUG("%s: iova 0x%lx, va 0x%llx\n", __func__, (uintptr_t)*iova, (uintptr_t)virt_addr);
	if (*iova == 0) {
		NPU_DRV_ERR("hisi_iommu_map_sg fail\n");
		return -1;
	}
	return 0;
}
int devdrv_hisi_npu_iova_unmap(unsigned long long virt_addr, unsigned long iova)
{
	struct sg_table *table = NULL;
	struct scatterlist *sg = NULL;
	int ret;
	struct devdrv_platform_info *platform_info = NULL;

	NPU_DRV_DEBUG("devdrv_hisi_npu_iova_unmap start \n");

	platform_info = devdrv_plat_get_info();
	if (platform_info == NULL) {
		NPU_DRV_ERR("devdrv_plat_get_info failed\n");
		return -1;
	}

	NPU_DRV_DEBUG("%s %d\n", __func__, __LINE__);
	ret = sg_table_va_map_find(&table, NULL, NULL, virt_addr);
	if (table == NULL) {
		NPU_DRV_ERR("your virt_addr should be called by devdrv_hisi_npu_iova_alloc\n");
		return -1;
	}

	sg = table->sgl;
	// 1.free iova
	ret = hisi_iommu_unmap_sg(DEVDRV_PLAT_GET_PSMMUDEV(platform_info), sg, iova);
	NPU_DRV_DEBUG("return %d after unmap sg.\n", ret);
	if (ret)
		NPU_DRV_ERR("iova unmap fail, ret = %d\n", ret);
	return ret;
}

int devdrv_hisi_npu_iova_map_ion(int fd, int prot, unsigned long *iova)
{
	unsigned long size = 0;
	struct dma_buf *dmabuf = NULL;
	struct devdrv_platform_info *platform_info = NULL;

	NPU_DRV_INFO("devdrv_hisi_npu_iova_map start \n");
	COND_RETURN_ERROR(fd < 0 || iova == NULL, -EINVAL, "invalid params\n");

	platform_info = devdrv_plat_get_info();
	COND_RETURN_ERROR(platform_info == NULL, -EINVAL, "devdrv_plat_get_info fail\n");

	dmabuf = dma_buf_get(fd);
	COND_RETURN_ERROR(IS_ERR_OR_NULL(dmabuf), -EINVAL, "get dmabuf fail\n");

	NPU_DRV_DEBUG("iova device ptr = %p, dmabuf = %p, fd = %d\n", DEVDRV_PLAT_GET_PSMMUDEV(platform_info), dmabuf, fd);
	*iova = hisi_iommu_map_dmabuf(DEVDRV_PLAT_GET_PSMMUDEV(platform_info), dmabuf, prot, &size);
	COND_RETURN_ERROR((*iova) == 0, -EINVAL, "hisi_iommu_map_dmabuf fail\n");

	dma_buf_put(dmabuf);
	NPU_DRV_INFO("iova map success\n");

	return 0;
}

int devdrv_hisi_npu_iova_unmap_ion(int fd, unsigned long iova)
{
	int ret;
	struct dma_buf *dmabuf = NULL;
	struct devdrv_platform_info *platform_info = NULL;

	NPU_DRV_INFO("devdrv_hisi_npu_iova_unmap start \n");
	COND_RETURN_ERROR(fd < 0 || iova == 0, -EINVAL, "invalid params\n");

	platform_info = devdrv_plat_get_info();
	COND_RETURN_ERROR(platform_info == NULL, -EINVAL, "devdrv_plat_get_info fail\n");

	dmabuf = dma_buf_get(fd);
	COND_RETURN_ERROR(IS_ERR_OR_NULL(dmabuf), -EINVAL, "get dmabuf fail\n");

	NPU_DRV_DEBUG("iova device ptr = %p, dmabuf = %p, fd = %d\n", DEVDRV_PLAT_GET_PSMMUDEV(platform_info), dmabuf, fd);
	ret = hisi_iommu_unmap_dmabuf(DEVDRV_PLAT_GET_PSMMUDEV(platform_info), dmabuf, iova);
	COND_RETURN_ERROR(ret != 0, -EINVAL, "hisi_iommu_unmap_dmabuf fail\n");

	dma_buf_put(dmabuf);
	NPU_DRV_INFO("iova unmap success\n");

	return 0;
}
