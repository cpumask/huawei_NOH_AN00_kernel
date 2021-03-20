/*
 * hisi_vcodec_vdec_memory.h
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

#ifndef HISI_VCODEC_VDEC_ION_H
#define HISI_VCODEC_VDEC_ION_H

#include <linux/dma-mapping.h>
#include <linux/dma-buf.h>
#include <linux/device.h>
#include <linux/fb.h>
#include <linux/version.h>
#include <linux/hisi-iommu.h>
#include "hi_type.h"

hi_s32 vdec_mem_iommu_map(void *dev, hi_s32 share_fd, UADDR *iova, unsigned long *size);
hi_s32 vdec_mem_iommu_unmap(void *dev, hi_s32 share_fd, UADDR iova);
#endif
