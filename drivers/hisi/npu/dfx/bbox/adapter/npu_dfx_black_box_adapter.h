/*
 * npu_dfx_black_box_adapter.h
 *
 * about npu black box adapter
 *
 * Copyright (c) 2012-2020 Huawei Technologies Co., Ltd.
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
 * attention: start to use in baltimore, If there are differences in subsequent
 * platforms, the directory needs to be split.
 */
#ifndef __NPU_BLACK_BOX_ADAPTER_H
#define __NPU_BLACK_BOX_ADAPTER_H

#ifdef CONFIG_NPU_NOC
static struct noc_err_para_s npu_noc_para[] = {
	{
		.masterid = (u32)SOC_NPU_AICORE0_MID,
		.targetflow = TARGET_FLOW_DEFAULT,
		.bus = 1, /* NOC_ERRBUS_NPU */
	}, {
		.masterid = (u32)SOC_NPU_AICORE1_MID,
		.targetflow = TARGET_FLOW_DEFAULT,
		.bus = 1, /* NOC_ERRBUS_NPU */
	}, {
		.masterid = (u32)SOC_NPU_SYSDMA_1_MID,
		.targetflow = TARGET_FLOW_DEFAULT,
		.bus = 1, /* NOC_ERRBUS_NPU */
	}, {
		.masterid = (u32)SOC_NPU_TS_0_MID,
		.targetflow = TARGET_FLOW_DEFAULT,
		.bus = 1, /* NOC_ERRBUS_NPU */
	}, {
		.masterid = (u32)SOC_NPU_TS_1_MID,
		.targetflow = TARGET_FLOW_DEFAULT,
		.bus = 1, /* NOC_ERRBUS_NPU */
	}, {
		.masterid = (u32)SOC_NPU_TCU_MID,
		.targetflow = TARGET_FLOW_DEFAULT,
		.bus = 1, /* NOC_ERRBUS_NPU */
	},
};

u32 modid_array[] = {
	(u32)EXC_TYPE_TS_AICORE_EXCEPTION, (u32)EXC_TYPE_TS_AICORE_EXCEPTION,
	(u32)EXC_TYPE_TS_SDMA_EXCEPTION, (u32)RDR_EXC_TYPE_TS_RUNNING_EXCEPTION,
	(u32)RDR_EXC_TYPE_TS_RUNNING_EXCEPTION, (u32)RDR_EXC_TYPE_NPU_SMMU_EXCEPTION
};
#endif

#endif /* __NPU_BLACK_BOX_ADAPTER_H */
