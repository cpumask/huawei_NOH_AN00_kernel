/*
 * log_rdr_cfg.c
 *
 * for rdr log cfg api define
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
 */
#include "log_cfg_api.h"
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sysfs.h>
#include <linux/semaphore.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/stat.h>
#include <linux/uaccess.h>
#include <linux/syscalls.h>
#include <linux/slab.h>
#include <linux/hisi/rdr_pub.h>

#include <securec.h>
#include "tc_ns_log.h"
#include "tlogger.h"

#define TEEOS_MODID HISI_BB_MOD_TEE_START
#define TEEOS_MODID_END  HISI_BB_MOD_TEE_END

struct rdr_register_module_result g_mem_info;
static const u64 g_current_core_id = RDR_TEEOS;

static void tee_fn_dump(u32 modid, u32 etype, u64 coreid, char *pathname,
	pfn_cb_dump_done pfn_cb)
{
	u32 l_modid = 0;

	l_modid = modid;
	pfn_cb(l_modid, g_current_core_id);
}

static int tee_rdr_register_core(void)
{
	struct rdr_module_ops_pub s_module_ops = {
		.ops_dump = NULL,
		.ops_reset = NULL
	};
	int ret = -1;

	s_module_ops.ops_dump = tee_fn_dump;
	s_module_ops.ops_reset = NULL;
	ret = rdr_register_module_ops(g_current_core_id,
		&s_module_ops, &g_mem_info);
	if (ret != 0)
		tloge("register rdr mem failed\n");
	return ret;
}

void unregister_log_exception(void)
{
	int ret;
	ret = rdr_unregister_exception((unsigned int)TEEOS_MODID);
	if (ret != 0)
		tloge("unregister rdr exception error\n");
}

int register_log_exception(void)
{
	struct rdr_exception_info_s einfo;
	int ret = -1;
	unsigned int ret_rdr;
	errno_t ret_s;
	const char tee_module_name[] = "RDR_TEEOS";
	const char tee_module_desc[] = "RDR_TEEOS crash";

	ret_s = memset_s(&einfo, sizeof(struct rdr_exception_info_s),
		0, sizeof(struct rdr_exception_info_s));
	if (ret_s != EOK) {
		tloge("memset einfo failed\n");
		return ret_s;
	}

	einfo.e_modid = (unsigned int)TEEOS_MODID;
	einfo.e_modid_end = (unsigned int)TEEOS_MODID_END;
	einfo.e_process_priority = RDR_ERR;
	einfo.e_reboot_priority = RDR_REBOOT_WAIT;
	einfo.e_notify_core_mask = RDR_TEEOS | RDR_AP;
	einfo.e_reset_core_mask = RDR_TEEOS | RDR_AP;
	einfo.e_reentrant = (unsigned int)RDR_REENTRANT_ALLOW;
	einfo.e_exce_type = TEE_S_EXCEPTION;
	einfo.e_from_core = RDR_TEEOS;
	einfo.e_upload_flag = (unsigned int)RDR_UPLOAD_YES;
	ret_s = memcpy_s(einfo.e_from_module, sizeof(einfo.e_from_module),
		tee_module_name, sizeof(tee_module_name));
	if (ret_s != EOK) {
		tloge("memcpy einfo.e_from_module failed\n");
		return ret_s;
	}
	ret_s = memcpy_s(einfo.e_desc, sizeof(einfo.e_desc),
		tee_module_desc, sizeof(tee_module_desc));
	if (ret_s != EOK) {
		tloge("memcpy einfo.e_desc failed\n");
		return ret_s;
	}
	ret_rdr = rdr_register_exception(&einfo);
	if (ret_rdr == 0) {
		tloge("register exception mem failed\n");
		ret = -1;
	} else {
		ret = 0;
	}
	return ret;
}

/* Register log memory */
int register_log_mem(uint64_t *addr, uint32_t *len)
{
	int ret = 0;

	if (addr == NULL || len == NULL) {
		tloge("check addr or len is failed\n");
		return -1;
	}

	ret = tee_rdr_register_core();
	if (ret != 0)
		return ret;

	ret = register_mem_to_teeos(g_mem_info.log_addr, g_mem_info.log_len);
	if (ret != 0)
		return ret;

	*addr = g_mem_info.log_addr;
	*len = g_mem_info.log_len;
	return ret;
}

#define RDR_ERROR_ADDR  0x83000001
void report_log_system_error(void)
{
	rdr_system_error(RDR_ERROR_ADDR, 0, 0);
}

void ta_crash_report_log(void)
{
}

int *map_log_mem(uint64_t mem_addr, uint32_t mem_len)
{
	return (int *)hisi_bbox_map(mem_addr, mem_len);
}

void unmap_log_mem(int *log_buffer)
{
	hisi_bbox_unmap((void *)log_buffer);
	(void)rdr_unregister_module_ops(g_current_core_id);
}

#define ROOT_UID                0
#define SYSTEM_GID              1000
void get_log_chown(uid_t *user, gid_t *group)
{
	if (user == NULL || group == NULL) {
		tloge("user or group buffer is null\n");
		return;
	}

	*user = ROOT_UID;
	*group = SYSTEM_GID;
}
