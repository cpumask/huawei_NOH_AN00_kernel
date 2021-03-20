/*
 * npu_dfx_profiling.c
 *
 * about npu dfx profiling
 *
 * Copyright (c) 2012-2019 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */
#include <linux/err.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <asm/barrier.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/errno.h>

#include "npu_log.h"
#include "npu_dfx.h"
#include "npu_cache.h"
#include "npu_manager_common.h"
#ifdef PROFILING_USE_RESERVED_MEMORY
#include <linux/io.h>
#endif
#include <securec.h>
#include "npu_platform_resource.h"
#include "npu_dfx_profiling.h"

char *get_prof_channel_data_addr (struct prof_buff_desc *prof_buff, u32 channel)
{
	COND_RETURN_ERROR(prof_buff == NULL, NULL, "prof_buff is null\n");
	switch (channel) {
	case PROF_CHANNEL_TSCPU:
		return (char *)(uintptr_t)(prof_buff->head.manager.ring_buff[PROF_CHANNEL_TSCPU].base_addr);
	case PROF_CHANNEL_HWTS_LOG:
		return (char *)(uintptr_t)(prof_buff->head.manager.ring_buff[PROF_CHANNEL_HWTS_LOG].base_addr);
	case PROF_CHANNEL_HWTS_PROFILING:
		return (char *)(uintptr_t)(prof_buff->head.manager.ring_buff[PROF_CHANNEL_HWTS_PROFILING].base_addr);
	default:
		NPU_DRV_ERR("channel: %u error\n", channel);
	}
	return NULL;
}

u32 get_prof_channel_data_size(struct prof_buff_desc *prof_buff, u32 channel)
{
	COND_RETURN_ERROR(prof_buff == NULL, 0, "prof_buff is null\n");
	switch (channel) {
	case PROF_CHANNEL_TSCPU:
		return PROF_TSCPU_DATA_SIZE;
	case PROF_CHANNEL_HWTS_LOG:
		return PROF_HWTS_LOG_SIZE;
	case PROF_CHANNEL_HWTS_PROFILING:
		return PROF_HWTS_PROFILING_SIZE;
	default:
		NPU_DRV_ERR("channel: %u error\n", channel);
	}
	return 0;
}

static int prof_buff_alloc(u32 len, vir_addr_t *virt_addr, unsigned long *iova)
{
	int ret;

	if (virt_addr == NULL || iova == NULL) {
		NPU_DRV_ERR("virt_addr or iova is null\n");
		return -1;
	}
	ret = devdrv_hisi_npu_iova_alloc(virt_addr, len);
	if (ret != 0) {
		NPU_DRV_ERR("prof_buff_alloc fail, ret=%d\n", ret);
		return ret;
	}
	ret = devdrv_hisi_npu_iova_map(*virt_addr, iova);
	if (ret != 0) {
		NPU_DRV_ERR("prof_buff_map fail, ret=%d\n", ret);
		devdrv_hisi_npu_iova_free(*virt_addr);
	}
	return ret;
}

void prof_buff_free(vir_addr_t virt_addr, unsigned long iova)
{
	int ret = 0;

	ret = devdrv_hisi_npu_iova_unmap(virt_addr, iova);
	if (ret != 0) {
		NPU_DRV_ERR("devdrv_hisi_npu_iova_unmap fail, ret=%d\n", ret);
		return;
	}
	devdrv_hisi_npu_iova_free(virt_addr);
	return;
}

int prof_buffer_init(struct prof_buff_desc *prof_buff)
{
	int ret;
	COND_RETURN_ERROR(prof_buff == NULL, PROF_ERROR, "prof buf nullptr\n");

	/* init tscpu data buffer */
	ret = prof_buff_alloc(PROF_TSCPU_DATA_SIZE, (vir_addr_t *)&(prof_buff->head.manager.ring_buff[PROF_CHANNEL_TSCPU].base_addr),
		(unsigned long *)&(prof_buff->head.manager.ring_buff[PROF_CHANNEL_TSCPU].iova_addr));

	if (ret != 0) {
		NPU_DRV_ERR("prof_buff_alloc fail,goal = tscpu_data,size = 0x%x\n", PROF_TSCPU_DATA_SIZE);
		NPU_DRV_DEBUG("addr=0x%lx\n", prof_buff->head.manager.ring_buff[PROF_CHANNEL_TSCPU].base_addr);
		return PROF_ERROR;
	}

	prof_buff->head.manager.ring_buff[PROF_CHANNEL_TSCPU].length = PROF_TSCPU_DATA_SIZE; /* no used but for consistency with other two buffer */

	/* init hwts log data buffer */
	ret = prof_buff_alloc(PROF_HWTS_LOG_SIZE, (vir_addr_t *)&(prof_buff->head.manager.ring_buff[PROF_CHANNEL_HWTS_LOG].base_addr),
		(unsigned long *)&(prof_buff->head.manager.ring_buff[PROF_CHANNEL_HWTS_LOG].iova_addr));

	if (ret != 0) {
		prof_buff_free(prof_buff->head.manager.ring_buff[PROF_CHANNEL_TSCPU].base_addr,
			prof_buff->head.manager.ring_buff[PROF_CHANNEL_TSCPU].iova_addr);
		NPU_DRV_ERR("prof_buff_alloc fail, goal = tscpu_data,size = 0x%x\n",
			PROF_HWTS_LOG_SIZE);
		NPU_DRV_DEBUG("addr=0x%lx\n",
			prof_buff->head.manager.ring_buff[PROF_CHANNEL_HWTS_LOG].base_addr);
		return PROF_ERROR;
	}

	prof_buff->head.manager.ring_buff[PROF_CHANNEL_HWTS_LOG].length = PROF_HWTS_LOG_SIZE / PROF_HWTS_LOG_FORMAT_SIZE; /* slot num for hwts global register config */

	/* init hwts profiling data buffer */
	ret = prof_buff_alloc(PROF_HWTS_PROFILING_SIZE, (vir_addr_t *)&(prof_buff->head.manager.ring_buff[PROF_CHANNEL_HWTS_PROFILING].base_addr),
		(unsigned long *)&(prof_buff->head.manager.ring_buff[PROF_CHANNEL_HWTS_PROFILING].iova_addr));

	if (ret != 0) {
		prof_buff_free(prof_buff->head.manager.ring_buff[PROF_CHANNEL_TSCPU].base_addr,
			prof_buff->head.manager.ring_buff[PROF_CHANNEL_TSCPU].iova_addr);
		prof_buff_free(prof_buff->head.manager.ring_buff[PROF_CHANNEL_HWTS_LOG].base_addr,
			prof_buff->head.manager.ring_buff[PROF_CHANNEL_HWTS_LOG].iova_addr);
		NPU_DRV_ERR("prof_buff_alloc fail,goal = tscpu_data,size = 0x%x\n",
			PROF_HWTS_PROFILING_SIZE);
		NPU_DRV_DEBUG("addr=0x%lx\n",
			prof_buff->head.manager.ring_buff[PROF_CHANNEL_HWTS_PROFILING].base_addr);
		return PROF_ERROR;
	}

	prof_buff->head.manager.ring_buff[PROF_CHANNEL_HWTS_PROFILING].length = PROF_HWTS_PROFILING_SIZE / PROF_HWTS_LOG_FORMAT_SIZE; /* slot num for hwts global register config */

	return PROF_OK;
}

int prof_buffer_release(struct prof_buff_desc *prof_buff)
{
	int i;
	COND_RETURN_ERROR(prof_buff == NULL, PROF_ERROR, "prof buf nullptr\n");

	for (i = PROF_CHANNEL_TSCPU; i < PROF_CHANNEL_MAX; i++) {
		prof_buff_free(prof_buff->head.manager.ring_buff[i].base_addr, prof_buff->head.manager.ring_buff[i].iova_addr);
		prof_buff->head.manager.ring_buff[i].base_addr = 0;
		prof_buff->head.manager.ring_buff[i].iova_addr = 0;
	}
	return PROF_OK;
}
