/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2010-2019. All rights reserved.
 * Description: smmu description
 */

#ifndef __HIVDEC_SMMU_REGS_H__
#define __HIVDEC_SMMU_REGS_H__

/* SMMU TBU regs offset */
#define SMMU_TBU_CR               (0x0000)
#define SMMU_TBU_CRACK            (0x0004)
#define SMMU_TBU_IRPT_MASK_NS     (0x0010)
#define SMMU_TBU_IRPT_CLR_NS      (0x001C)
#define SMMU_TBU_SWID_CFG_N       (0x0100)
#define SMMU_TBU_SCR              (0x1000)
#define SMMU_TBU_IRPT_MASK_S      (0x1010)
#define SMMU_TBU_IRPT_CLR_S       (0x101C)
#define SMMU_TBU_PROT_EN_N        (0x1100)
#define SMMU_TBU_PMCG_EVCNTR      (0x2000)
#define SMMU_TBU_PMCG_EVTYPER_N   (0x2404)
#define SMMU_TBU_PMCG_SMR         (0x2A00)
#define SMMU_TBU_PMCG_CNTENSET0_0 (0x2C00)
#define SMMU_TBU_PMCG_CAPR        (0x2D88)
#define SMMU_TBU_PMCG_CR          (0x2E04)

#define SMMU_PRE_SID              (0x0004)
#define SMMU_PRE_SSID             (0x0008)
#define SMMU_PRE_SSIDV            (0x000C)

#endif
