/*
 * hisi_vcodec_vdec_memory.c
 *
 * This is for vdec memory
 *
 * Copyright (c) 2019-2020 Huawei Technologies CO., Ltd.
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

#include "hisi_vcodec_vdec_memory.h"
#include "hisi_vcodec_vdec_plat.h"
#include "vfmw_osal.h"

static DEFINE_MUTEX(g_mem_mutex);

hi_s32 vdec_mem_iommu_map(void *dev, hi_s32 share_fd, UADDR *iova, unsigned long *size)
{
	hi_u64 iova_addr;
	unsigned long phy_size;
	struct dma_buf *dmabuf = HI_NULL;

	*iova = 0;
	*size = 0;

	vdec_check_ret(share_fd >= 0, HI_FAILURE);

	vdec_mutex_lock(&g_mem_mutex);

	dmabuf = dma_buf_get(share_fd);
	if (IS_ERR(dmabuf)) {
		dprint(PRN_FATAL, "dma_buf_get failed");
		vdec_mutex_unlock(&g_mem_mutex);
		return HI_FAILURE;
	}

	iova_addr = hisi_iommu_map_dmabuf((struct device *)dev, dmabuf, 0, &phy_size);
	if (!iova_addr) {
		dprint(PRN_FATAL, "hisi_iommu_map_dmabuf failed");
		dma_buf_put(dmabuf);
		vdec_mutex_unlock(&g_mem_mutex);
		return HI_FAILURE;
	}

	*iova = iova_addr;
	*size = phy_size;

	dma_buf_put(dmabuf);
	vdec_mutex_unlock(&g_mem_mutex);

	return HI_SUCCESS;
}

hi_s32 vdec_mem_iommu_unmap(void *dev, hi_s32 share_fd, UADDR iova)
{
	struct dma_buf *dmabuf = HI_NULL;
	hi_s32 ret;

	vdec_check_ret(share_fd >= 0, HI_FAILURE);
	vdec_check_ret(iova != 0, HI_FAILURE);

	vdec_mutex_lock(&g_mem_mutex);

	dmabuf = dma_buf_get(share_fd);
	if (IS_ERR(dmabuf)) {
		dprint(PRN_FATAL, "dma buf get failed\n");
		vdec_mutex_unlock(&g_mem_mutex);
		return HI_FAILURE;
	}

	ret = hisi_iommu_unmap_dmabuf((struct device *)dev, dmabuf, iova);
	if (ret) {
		dprint(PRN_ERROR, "hisi iommu unmap dmabuf failed\n");
		ret = HI_FAILURE;
	}

	dma_buf_put(dmabuf);

	vdec_mutex_unlock(&g_mem_mutex);
	return ret;
}

