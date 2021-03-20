/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: ivp atf subsys function
 * Author: chenweiyu
 * Create: 2019-08-23
 */

#include "ivp_atf_subsys.h"
#include <asm/compiler.h>
#include <linux/compiler.h>
#include <linux/of.h>
#include <linux/dma-mapping.h>
#include <linux/clk.h>
#include "ivp.h"
#include "ivp_log.h"
#include "ivp_reg.h"
#include "ivp_platform.h"

static noinline void atfd_hisi_service_ivp_smc(u64 _function_id,
	u64 _arg0, u64 _arg1, u64 _arg2, u64 _arg3)
{
	register u64 function_id asm("x0") = _function_id;
	register u64 arg0 asm("x1") = _arg0;
	register u64 arg1 asm("x2") = _arg1;
	register u64 arg2 asm("x3") = _arg2;
	register u64 arg3 asm("x4") = _arg3;

	asm volatile(
		__asmeq("%0", "x0")
		__asmeq("%1", "x1")
		__asmeq("%2", "x2")
		__asmeq("%3", "x3")
		__asmeq("%4", "x4")
		"smc    #0\n"
		: "+r" (function_id)
		: "r" (arg0), "r" (arg1), "r" (arg2), "r" (arg3));
}

static int ivpatf_check_coreid(unsigned int core_id)
{
	if (core_id != IVP_CORE0_ID && core_id != IVP_CORE1_ID)
		return -EINVAL;
	return 0;
}

int ivpatf_change_slv_secmod(unsigned int core_id, unsigned int mode)
{
	int ret;

	ret = ivpatf_check_coreid(core_id);
	if (ret) {
		ivp_err("invalid core_id:%u", core_id);
		return ret;
	}
	ivp_dbg("change slv mode to %u", mode);
	atfd_hisi_service_ivp_smc(IVP_SLV_SECMODE, core_id, mode, 0, 0);
	return ret;
}

int ivpatf_change_mst_secmod(unsigned int core_id, unsigned int mode)
{
	int ret;

	ret = ivpatf_check_coreid(core_id);
	if (ret) {
		ivp_err("invalid core_id:%u", core_id);
		return ret;
	}
	ivp_dbg("change mst mode to %u", mode);
	atfd_hisi_service_ivp_smc(IVP_MST_SECMODE, core_id, mode, 0, 0);
	return ret;
}

int ivpatf_poweron_remap_secmod(unsigned int core_id, unsigned int ivp_addr,
	unsigned int len, unsigned long ddr_addr)
{
	int ret;

	ret = ivpatf_check_coreid(core_id);
	if (ret) {
		ivp_err("invalid core_id:%u", core_id);
		return ret;
	}

	if ((ivp_addr & MASK_1MB) != 0 ||
		(ddr_addr & MASK_1MB) != 0 ||
		len >= MAX_DDR_LEN * SIZE_1MB) {
		ivp_dbg("ivp_addr:0x%x, ddr_addr:0x%lx not aligned, len:0x%x err",
			ivp_addr, ddr_addr, len);
		return -EINVAL;
	}
	len = (len + SIZE_1MB - 1) / SIZE_1MB - 1;
	atfd_hisi_service_ivp_smc(IVP_REMAP_SECMODE, core_id,
		ivp_addr / SIZE_1MB, ddr_addr / SIZE_1MB, len);
	return ret;
}

int ivpatf_tbu_signal_cfg_secmod(unsigned int core_id,
	unsigned int sid, unsigned int ssid, unsigned int secmod)
{
	if (core_id != IVP_CORE0_ID && core_id != IVP_CORE1_ID) {
		ivp_err("invalid input core_id:%u", core_id);
		return -EINVAL;
	}
	if (secmod == SECURE_MODE) {
		if ((sid != MEDIA2_IVP_SEC_SID_VALUE) ||
				(ssid != MEDIA2_IVP_SEC_SSID_VALUE)) {
			ivp_err("invalid input sid:%u ssid:%u", sid, ssid);
			return -EINVAL;
		}
	} else {
		if ((sid != MEDIA2_IVP_SID_VALUE) ||
				((ssid != MEDIA2_IVP0_SSID_VALUE) &&
				 (ssid != MEDIA2_IVP1_SSID_VALUE))) {
			return -EINVAL;
		}
	}

	atfd_hisi_service_ivp_smc(IVP_TBU_CFG_SECMODE, core_id, sid, ssid, secmod);
	return 0;
}
