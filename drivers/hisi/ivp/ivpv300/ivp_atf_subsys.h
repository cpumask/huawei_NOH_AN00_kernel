/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: ivp atf subsys header file
 * Author: chenweiyu
 * Create: 2019-08-23
 */

#ifndef _IVP_ATF_SUBSYS_H_
#define _IVP_ATF_SUBSYS_H_

#define IVP_SLV_SECMODE            0xC500BB00
#define IVP_MST_SECMODE            0xC500BB01
#define IVP_REMAP_SECMODE          0xC500BB02
#define IVP_TBU_CFG_SECMODE        0xC500BB03

enum secmode {
	IVP_SEC = 0,
	IVP_NONSEC
};

int ivpatf_change_slv_secmod(unsigned int core_id, unsigned int mode);
int ivpatf_change_mst_secmod(unsigned int core_id, unsigned int mode);
int ivpatf_poweron_remap_secmod(unsigned int core_id, unsigned int ivp_addr,
	unsigned int len, unsigned long ddr_addr);
int ivpatf_tbu_signal_cfg_secmod(unsigned int core_id, unsigned int sid,
	unsigned int ssid, unsigned int secmod);

#endif /* _IVP_ATF_SUBSYS_H_ */
