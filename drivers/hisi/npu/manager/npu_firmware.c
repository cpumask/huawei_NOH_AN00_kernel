/*
 * npu_firmware.c
 *
 * about npu firmware
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
#include "npu_firmware.h"

#include <securec.h>
#include <linux/io.h>
#include <linux/hisi/rdr_pub.h>

#include "npu_log.h"
#include "npu_common.h"
#include "npu_platform.h"
#include "bbox/npu_dfx_black_box.h"

#define NPU_FIRMWARE_GET_FILE_SIZE_FAIL  (-1)

static struct devdrv_mem_desc* fw_mem_desc[DEVDRV_FW_TYPE_MAX] = {0};

loff_t devdrv_get_file_size(const char *path)
{
	int error;
	loff_t filesize = -1;
	struct kstat statbuff;
	mm_segment_t old_fs;

	old_fs = get_fs();
	set_fs((mm_segment_t)KERNEL_DS);

	error = vfs_stat(path, &statbuff);
	if (error < 0) {
		NPU_DRV_ERR("vfs_stat failed ret = %d\n ", error);
		set_fs(old_fs);
		return filesize;
	} else {
		filesize = statbuff.size;
	}
	set_fs(old_fs);
	return filesize;
}

bool devdrv_load_firmware(const struct file *fp, loff_t fsize, loff_t pos, u32 type)
{
	int ret;
	char *fw_load_addr = NULL;
	u64 max_size = (fsize > TS_FW_REMAP_SIZE) ? fsize : TS_FW_REMAP_SIZE;
	struct devdrv_platform_info *plat_info = devdrv_plat_get_info();
	if (plat_info == NULL) {
		NPU_DRV_ERR("devdrv_plat_get_info failed\r\n");
		return false;
	}

	NPU_DRV_DEBUG("devdrv_load_firmware\n ");
	if (type == DEVDRV_FW_TYPE_AICPU) {
		max_size = fsize;
		fw_load_addr = ioremap_nocache(devdrv_get_firmware_phy_addr(DEVDRV_FW_TYPE_AICPU), max_size);
	} else {
		if (fsize > TS_FW_MAX_SIZE * 4) {
			NPU_DRV_ERR("ts firmware size %d is too large\r\n", (u32)fsize);
			return false;
		}
		fw_load_addr = ioremap_nocache(devdrv_get_firmware_phy_addr(DEVDRV_FW_TYPE_TS), max_size);
	}

	if (fw_load_addr == NULL) {
		NPU_DRV_ERR("ioremap_nocache failed\n");
		return false;
	}
	NPU_DRV_DEBUG("max_size = 0x%llx ,fw_load_addr = %pK , fw_phy_addr = 0x%llx\n ",
		max_size, fw_load_addr, devdrv_get_firmware_phy_addr(DEVDRV_FW_TYPE_TS));
	NPU_DRV_DEBUG("pos %d type %d \n ", (u32)pos, type);

	ret = DEVDRV_PLAT_GET_RES_FW_PROC(plat_info)(fp, fsize, pos, (u64)(uintptr_t)fw_load_addr);
	if (ret != true) {
		NPU_DRV_ERR("npu_fw_proc failed, fsize is %d\r\n", (u32)fsize);
		iounmap(fw_load_addr);
		return false;
	}

	iounmap(fw_load_addr);
	return true;
}

int devdrv_load_cpu_fw_by_type(const char *firmware_path, u32 type)
{
	int ret = -1;
	loff_t fsize;
	struct file *fp = NULL;

	NPU_DRV_DEBUG("firmware_path: %p, type = %d \n", firmware_path, type);

	if (firmware_path == NULL) {
		NPU_DRV_ERR("firmware path is null\n");
		return -1;
	}

	if (type > DEVDRV_FW_TYPE_TS) {
		NPU_DRV_ERR("firmware type is invalid\n");
		return -1;
	}

	fsize = devdrv_get_file_size(firmware_path);
	NPU_DRV_DEBUG("firmware size is : %lld\n", fsize);
	if (fsize == NPU_FIRMWARE_GET_FILE_SIZE_FAIL) {
		NPU_DRV_ERR("get file size failed\n");
		return -1;
	}

	fp = filp_open(firmware_path, O_RDONLY, 0);
	if (IS_ERR(fp)) {
		NPU_DRV_ERR("can't open firmware file\n");
		return -1;
	}
	// load img offset 0x80
	if (devdrv_load_firmware(fp, fsize, 0x80, type) == false) {
		NPU_DRV_ERR("can't load firmware file %d\n", type);
		filp_close(fp, NULL);
		return ret;
	}

	filp_close(fp, NULL);
	return 0;
}

int devdrv_load_cpu_fw(void)
{
	int ret;
	static u32 s_ts_fw_load_flag = 0;
	struct devdrv_platform_info *plat_info = devdrv_plat_get_info();
	if (plat_info == NULL) {
		NPU_DRV_ERR("devdrv_plat_get_info failed\n");
		return -1;
	}

	if (DEVDRV_PLAT_GET_AICPU_MAX(plat_info) != 0) {
		ret = devdrv_load_cpu_fw_by_type(DEVDRV_AICPU_BINARY_PATH, DEVDRV_FW_TYPE_AICPU);
		if (ret != 0) {
			NPU_DRV_ERR("load ai cpu fw failed\n");
			return ret;
		}
	}

	if (s_ts_fw_load_flag == 0) {
		NPU_DRV_BOOT_TIME_TAG("start LOAD TS FW \n");
		if (DEVDRV_PLAT_GET_FEAUTRE_SWITCH(plat_info, DEVDRV_FEATURE_HWTS) == 1
			&& strstr(DEVDRV_PLAT_GET_CHIPTYPE(plat_info), "_es") != 0) {
			NPU_DRV_DEBUG("chiptype = %s\n", DEVDRV_PLAT_GET_CHIPTYPE(plat_info));
			ret = devdrv_load_cpu_fw_by_type(DEVDRV_NPU_FW_ES_IMA_PATH, DEVDRV_FW_TYPE_TS);
		} else {
			ret = devdrv_load_cpu_fw_by_type(DEVDRV_NPU_FW_IMA_PATH, DEVDRV_FW_TYPE_TS);
		}
		NPU_DRV_BOOT_TIME_TAG("END LOAD TS FW \n");
		if (ret != 0) {
			NPU_DRV_ERR("load ts cpu fw failed\n");
			/* bbox : load ts cpu fw failed */
			if (npu_rdr_exception_is_count_exceeding(RDR_EXC_TYPE_TS_INIT_EXCEPTION) == 0)
				rdr_system_error((u32)RDR_EXC_TYPE_TS_INIT_EXCEPTION, 0, 0);
			return ret;
		}
	}

	return 0;
}

void devdrv_get_firmware_mem_desc(void)
{
	struct devdrv_platform_info* plat_info = NULL;

	plat_info = devdrv_plat_get_info();
	if (plat_info == NULL) {
		NPU_DRV_ERR("devdrv_plat_get_irq failed\n");
		return;
	}

	fw_mem_desc[DEVDRV_FW_TYPE_AICPU] = DEVDRV_PLAT_GET_AIFW_BUF(plat_info);
	fw_mem_desc[DEVDRV_FW_TYPE_TS] = DEVDRV_PLAT_GET_TSFW_BUF(plat_info);
	return;
}

u64 devdrv_get_firmware_phy_addr(int type)
{
	if (type >= DEVDRV_FW_TYPE_MAX) {
		NPU_DRV_ERR("devdrv_plat_get_irq failed\n");
		return 0;
	}

	if (fw_mem_desc[type] == NULL)
		devdrv_get_firmware_mem_desc();

	return (u64)fw_mem_desc[type]->base;
}
EXPORT_SYMBOL(devdrv_get_firmware_phy_addr);

