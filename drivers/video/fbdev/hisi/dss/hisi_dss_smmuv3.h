/* Copyright (c) 2019-2020, Hisilicon Tech. Co., Ltd. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */
#ifndef HISI_DSS_SMMUV3_H
#define HISI_DSS_SMMUV3_H

#define SMMU_TIMEOUT 1000   /* us */
#define DSS_TBU0_DTI_NUMS 0x17
#define DSS_TBU1_DTI_NUMS 0x0F
#define MDC_TBU_DTI_NUMS 0x0B
#define MDC_SWID_NUMS 33

#define HISI_DSS_SSID_MAX_NUM 3
#define HISI_PRIMARY_DSS_SSID_OFFSET 0
#define HISI_EXTERNAL_DSS_SSID_OFFSET 3
#define HISI_OFFLINE_SSID 16

enum smmu_event {
	TBU_DISCONNECT = 0x0,
	TBU_CONNECT = 0x1,
};

static inline int hisi_dss_get_ssid(int frame_no)
{
	return frame_no % HISI_DSS_SSID_MAX_NUM;
}

void hisi_dss_smmuv3_deinit(void);
void hisi_dss_smmuv3_init(void);

#endif
