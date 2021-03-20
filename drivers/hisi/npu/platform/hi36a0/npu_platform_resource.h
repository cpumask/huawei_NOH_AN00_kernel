/*
 * npu_platform_resource.h
 *
 * about npu platform resource
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
#ifndef __NPU_PLATFORM_RESOURCE_H
#define __NPU_PLATFORM_RESOURCE_H
#include "global_ddr_map.h"
#include "devdrv_user_common_model.h"

#define DEVDRV_PLAT_STREAM_MAX     280
#define DEVDRV_PLAT_EVENT_MAX      256
#define DEVDRV_PLAT_NOTIFY_MAX     256
#define DEVDRV_PLAT_MODEL_MAX      192
#define DEVDRV_PLAT_AICORE_MAX     2
#define DEVDRV_PLAT_AICPU_MAX      0
#define DEVDRV_PLAT_CALC_SQ_MAX    80
#define DEVDRV_PLAT_CALC_SQ_DEPTH  256
#define DEVDRV_PLAT_CALC_CQ_MAX    20
#define DEVDRV_PLAT_CALC_CQ_DEPTH  1024
#define DEVDRV_PLAT_DFX_SQ_MAX     4
#define DEVDRV_PLAT_DFX_CQ_MAX     10
#define DEVDRV_PLAT_DFX_SQCQ_DEPTH 1024
#define DEVDRV_PLAT_DOORBELL_STRIDE 4096  /* stride 4KB */
#define DEVDRV_PLAT_SYSCACHE_SIZE  0x800000 /* 8M */



#define NPU_NONSEC_RESERVED_DDR_BASE_ADDR  (HISI_RESERVED_NPU_AI_TS_FW_PHYMEM_BASE)
#define NPU_SEC_RESERVED_DDR_BASE_ADDR  (HISI_RESERVED_NPU_SEC_PHYMEM_BASE)

/* baltimore need recheck the size */
#define NPU_NONSEC_RESERVED_TSCPU_FW_SIZE       0x400000
#define NPU_NONSEC_RESERVED_CONTROL_INFO_SIZE   0x80000
#define NPU_NONSEC_RESERVED_LOG_SIZE            0x80000
#define NPU_NONSEC_RESERVED_BBOX_SIZE           0x40000
#define NPU_NONSEC_RESERVED_SQCQ_SIZE           0x200000

#define NPU_NONSEC_RESERVED_PROF_SIZE           0x1000


#define NPU_NONSEC_RESERVED_TSCPU_FW_BASE_ADDR	NPU_NONSEC_RESERVED_DDR_BASE_ADDR

#define NPU_NONSEC_RESERVED_CONTROL_INFO_BASE_ADDR	(NPU_NONSEC_RESERVED_TSCPU_FW_BASE_ADDR \
												+ NPU_NONSEC_RESERVED_TSCPU_FW_SIZE)

#define NPU_NONSEC_RESERVED_LOG_BASE_ADDR		(NPU_NONSEC_RESERVED_CONTROL_INFO_BASE_ADDR \
												+ NPU_NONSEC_RESERVED_CONTROL_INFO_SIZE)

#define NPU_NONSEC_RESERVED_BBOX_BASE_ADDR		(NPU_NONSEC_RESERVED_LOG_BASE_ADDR \
												+ NPU_NONSEC_RESERVED_LOG_SIZE)

#define NPU_NONSEC_RESERVED_SQCQ_BASE_ADDR		(NPU_NONSEC_RESERVED_BBOX_BASE_ADDR \
												+ NPU_NONSEC_RESERVED_SQCQ_SIZE)

#define NPU_NONSEC_RESERVED_PROF_BASE_ADDR		(NPU_NONSEC_RESERVED_CONTROL_INFO_BASE_ADDR \
												+ DEVDRV_PROFILINGL_INFO_OFFSET)


enum devdrv_dump_region_index {
	DEVDRV_DUMP_REGION_NPU_CRG,
	DEVDRV_DUMP_REGION_MAX
};
#endif
