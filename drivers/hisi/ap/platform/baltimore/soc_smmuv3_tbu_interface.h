#ifndef __SOC_SMMUV3_TBU_INTERFACE_H__
#define __SOC_SMMUV3_TBU_INTERFACE_H__ 
#ifdef __cplusplus
    #if __cplusplus
        extern "C" {
    #endif
#endif
#ifndef __SOC_H_FOR_ASM__
#define SOC_SMMUv3_TBU_SMMU_TBU_CR_ADDR(base) ((base) + (0x0000UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_CRACK_ADDR(base) ((base) + (0x0004UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_MEM_CTRL_S_ADDR(base) ((base) + (0x0008UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_MEM_CTRL_B_ADDR(base) ((base) + (0x000CUL))
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_MASK_NS_ADDR(base) ((base) + (0x0010UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_RAW_NS_ADDR(base) ((base) + (0x0014UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_STAT_NS_ADDR(base) ((base) + (0x0018UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_CLR_NS_ADDR(base) ((base) + (0x001CUL))
#define SOC_SMMUv3_TBU_SMMU_TBU_LOCK_CLR_ADDR(base) ((base) + (0x0020UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_TLB_INV_NS_ADDR(base) ((base) + (0x0024UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_SWID_CFG_ADDR(base,m) ((base) + ((m)*0x4+0x100UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PIDR4_ADDR(base) ((base) + (0x0FD0UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PIDR5_ADDR(base) ((base) + (0x0FD4UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PIDR6_ADDR(base) ((base) + (0x0FD8UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PIDR7_ADDR(base) ((base) + (0x0FDCUL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PIDR0_ADDR(base) ((base) + (0x0FE0UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PIDR1_ADDR(base) ((base) + (0x0FE4UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PIDR2_ADDR(base) ((base) + (0x0FE8UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PIDR3_ADDR(base) ((base) + (0x0FECUL))
#define SOC_SMMUv3_TBU_SMMU_TBU_CIDR0_ADDR(base) ((base) + (0x0FF0UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_CIDR1_ADDR(base) ((base) + (0x0FF4UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_CIDR2_ADDR(base) ((base) + (0x0FF8UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_CIDR3_ADDR(base) ((base) + (0x0FFCUL))
#define SOC_SMMUv3_TBU_SMMU_TBU_SCR_ADDR(base) ((base) + (0x1000UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_MASK_S_ADDR(base) ((base) + (0x1010UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_RAW_S_ADDR(base) ((base) + (0x1014UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_STAT_S_ADDR(base) ((base) + (0x1018UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_CLR_S_ADDR(base) ((base) + (0x101CUL))
#define SOC_SMMUv3_TBU_SMMU_TBU_DBG_PORT_IN_ADDR(base) ((base) + (0x1020UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_DBG_PORT_OUT_0_ADDR(base) ((base) + (0x1024UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_DBG_PORT_OUT_1_ADDR(base) ((base) + (0x1028UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_DBG_PORT_OUT_2_ADDR(base) ((base) + (0x102CUL))
#define SOC_SMMUv3_TBU_SMMU_TBU_DBG_PORT_OUT_3_ADDR(base) ((base) + (0x1030UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_TLB_INV_S_ADDR(base) ((base) + (0x1034UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PROT_EN_ADDR(base,m) ((base) + ((m)*0x4+0x1100UL))
#else
#define SOC_SMMUv3_TBU_SMMU_TBU_CR_ADDR(base) ((base) + (0x0000))
#define SOC_SMMUv3_TBU_SMMU_TBU_CRACK_ADDR(base) ((base) + (0x0004))
#define SOC_SMMUv3_TBU_SMMU_TBU_MEM_CTRL_S_ADDR(base) ((base) + (0x0008))
#define SOC_SMMUv3_TBU_SMMU_TBU_MEM_CTRL_B_ADDR(base) ((base) + (0x000C))
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_MASK_NS_ADDR(base) ((base) + (0x0010))
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_RAW_NS_ADDR(base) ((base) + (0x0014))
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_STAT_NS_ADDR(base) ((base) + (0x0018))
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_CLR_NS_ADDR(base) ((base) + (0x001C))
#define SOC_SMMUv3_TBU_SMMU_TBU_LOCK_CLR_ADDR(base) ((base) + (0x0020))
#define SOC_SMMUv3_TBU_SMMU_TBU_TLB_INV_NS_ADDR(base) ((base) + (0x0024))
#define SOC_SMMUv3_TBU_SMMU_TBU_SWID_CFG_ADDR(base,m) ((base) + ((m)*0x4+0x100))
#define SOC_SMMUv3_TBU_SMMU_TBU_PIDR4_ADDR(base) ((base) + (0x0FD0))
#define SOC_SMMUv3_TBU_SMMU_TBU_PIDR5_ADDR(base) ((base) + (0x0FD4))
#define SOC_SMMUv3_TBU_SMMU_TBU_PIDR6_ADDR(base) ((base) + (0x0FD8))
#define SOC_SMMUv3_TBU_SMMU_TBU_PIDR7_ADDR(base) ((base) + (0x0FDC))
#define SOC_SMMUv3_TBU_SMMU_TBU_PIDR0_ADDR(base) ((base) + (0x0FE0))
#define SOC_SMMUv3_TBU_SMMU_TBU_PIDR1_ADDR(base) ((base) + (0x0FE4))
#define SOC_SMMUv3_TBU_SMMU_TBU_PIDR2_ADDR(base) ((base) + (0x0FE8))
#define SOC_SMMUv3_TBU_SMMU_TBU_PIDR3_ADDR(base) ((base) + (0x0FEC))
#define SOC_SMMUv3_TBU_SMMU_TBU_CIDR0_ADDR(base) ((base) + (0x0FF0))
#define SOC_SMMUv3_TBU_SMMU_TBU_CIDR1_ADDR(base) ((base) + (0x0FF4))
#define SOC_SMMUv3_TBU_SMMU_TBU_CIDR2_ADDR(base) ((base) + (0x0FF8))
#define SOC_SMMUv3_TBU_SMMU_TBU_CIDR3_ADDR(base) ((base) + (0x0FFC))
#define SOC_SMMUv3_TBU_SMMU_TBU_SCR_ADDR(base) ((base) + (0x1000))
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_MASK_S_ADDR(base) ((base) + (0x1010))
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_RAW_S_ADDR(base) ((base) + (0x1014))
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_STAT_S_ADDR(base) ((base) + (0x1018))
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_CLR_S_ADDR(base) ((base) + (0x101C))
#define SOC_SMMUv3_TBU_SMMU_TBU_DBG_PORT_IN_ADDR(base) ((base) + (0x1020))
#define SOC_SMMUv3_TBU_SMMU_TBU_DBG_PORT_OUT_0_ADDR(base) ((base) + (0x1024))
#define SOC_SMMUv3_TBU_SMMU_TBU_DBG_PORT_OUT_1_ADDR(base) ((base) + (0x1028))
#define SOC_SMMUv3_TBU_SMMU_TBU_DBG_PORT_OUT_2_ADDR(base) ((base) + (0x102C))
#define SOC_SMMUv3_TBU_SMMU_TBU_DBG_PORT_OUT_3_ADDR(base) ((base) + (0x1030))
#define SOC_SMMUv3_TBU_SMMU_TBU_TLB_INV_S_ADDR(base) ((base) + (0x1034))
#define SOC_SMMUv3_TBU_SMMU_TBU_PROT_EN_ADDR(base,m) ((base) + ((m)*0x4+0x1100))
#endif
#ifndef __SOC_H_FOR_ASM__
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_EVCNTR_ADDR(base,k) ((base) + (0x0+(k)*4UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_EVTYPER0_ADDR(base) ((base) + (0x400UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_EVTYPER_ADDR(base,u) ((base) + (0x404+(u)*4UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_SVR_ADDR(base,k) ((base) + (0x600+(k)*4UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_SMR_ADDR(base,l) ((base) + (0xA00+(l)*4UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CNTENSET0_0_ADDR(base) ((base) + (0xC00UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CNTENSET0_1_ADDR(base) ((base) + (0xC04UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CNTENCLR0_0_ADDR(base) ((base) + (0xC20UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CNTENCLR0_1_ADDR(base) ((base) + (0xC24UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_INTENSET0_0_ADDR(base) ((base) + (0xC40UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_INTENSET0_1_ADDR(base) ((base) + (0xC44UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_INTENCLR0_0_ADDR(base) ((base) + (0xC60UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_INTENCLR0_1_ADDR(base) ((base) + (0xC64UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_OVSCLR0_0_ADDR(base) ((base) + (0xC80UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_OVSCLR0_1_ADDR(base) ((base) + (0xC84UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_OVSSET0_0_ADDR(base) ((base) + (0xCC0UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_OVSSET0_1_ADDR(base) ((base) + (0xCC4UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CAPR_ADDR(base) ((base) + (0xD88UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_SCR_ADDR(base) ((base) + (0xDF8UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CFGR_ADDR(base) ((base) + (0xE00UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CR_ADDR(base) ((base) + (0xE04UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CEID0_0_ADDR(base) ((base) + (0xE20UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CEID0_1_ADDR(base) ((base) + (0xE24UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CEID1_0_ADDR(base) ((base) + (0xE28UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CEID1_1_ADDR(base) ((base) + (0xE2CUL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_IRQ_CTRL_ADDR(base) ((base) + (0xE50UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_IRQ_CTRLACK_ADDR(base) ((base) + (0xE54UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_IRQ_CFG0_0_ADDR(base) ((base) + (0xE58UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_IRQ_CFG0_1_ADDR(base) ((base) + (0xE5CUL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_IRQ_CFG1_ADDR(base) ((base) + (0xE60UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_IRQ_CFG2_ADDR(base) ((base) + (0xE64UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_IRQ_STATUS_ADDR(base) ((base) + (0xE68UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_AIDR_ADDR(base) ((base) + (0xE70UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_PMAUTHSTATUS_ADDR(base) ((base) + (0xFB8UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_PIDR4_ADDR(base) ((base) + (0xFD0UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_PIDR5_ADDR(base) ((base) + (0xFD4UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_PIDR6_ADDR(base) ((base) + (0xFD8UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_PIDR7_ADDR(base) ((base) + (0xFDCUL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_PIDR0_ADDR(base) ((base) + (0xFE0UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_PIDR1_ADDR(base) ((base) + (0xFE4UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_PIDR2_ADDR(base) ((base) + (0xFE8UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_PIDR3_ADDR(base) ((base) + (0xFECUL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CIDR0_ADDR(base) ((base) + (0xFF0UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CIDR1_ADDR(base) ((base) + (0xFF4UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CIDR2_ADDR(base) ((base) + (0xFF8UL))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CIDR3_ADDR(base) ((base) + (0xFFCUL))
#else
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_EVCNTR_ADDR(base,k) ((base) + (0x0+(k)*4))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_EVTYPER0_ADDR(base) ((base) + (0x400))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_EVTYPER_ADDR(base,u) ((base) + (0x404+(u)*4))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_SVR_ADDR(base,k) ((base) + (0x600+(k)*4))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_SMR_ADDR(base,l) ((base) + (0xA00+(l)*4))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CNTENSET0_0_ADDR(base) ((base) + (0xC00))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CNTENSET0_1_ADDR(base) ((base) + (0xC04))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CNTENCLR0_0_ADDR(base) ((base) + (0xC20))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CNTENCLR0_1_ADDR(base) ((base) + (0xC24))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_INTENSET0_0_ADDR(base) ((base) + (0xC40))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_INTENSET0_1_ADDR(base) ((base) + (0xC44))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_INTENCLR0_0_ADDR(base) ((base) + (0xC60))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_INTENCLR0_1_ADDR(base) ((base) + (0xC64))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_OVSCLR0_0_ADDR(base) ((base) + (0xC80))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_OVSCLR0_1_ADDR(base) ((base) + (0xC84))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_OVSSET0_0_ADDR(base) ((base) + (0xCC0))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_OVSSET0_1_ADDR(base) ((base) + (0xCC4))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CAPR_ADDR(base) ((base) + (0xD88))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_SCR_ADDR(base) ((base) + (0xDF8))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CFGR_ADDR(base) ((base) + (0xE00))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CR_ADDR(base) ((base) + (0xE04))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CEID0_0_ADDR(base) ((base) + (0xE20))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CEID0_1_ADDR(base) ((base) + (0xE24))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CEID1_0_ADDR(base) ((base) + (0xE28))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CEID1_1_ADDR(base) ((base) + (0xE2C))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_IRQ_CTRL_ADDR(base) ((base) + (0xE50))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_IRQ_CTRLACK_ADDR(base) ((base) + (0xE54))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_IRQ_CFG0_0_ADDR(base) ((base) + (0xE58))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_IRQ_CFG0_1_ADDR(base) ((base) + (0xE5C))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_IRQ_CFG1_ADDR(base) ((base) + (0xE60))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_IRQ_CFG2_ADDR(base) ((base) + (0xE64))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_IRQ_STATUS_ADDR(base) ((base) + (0xE68))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_AIDR_ADDR(base) ((base) + (0xE70))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_PMAUTHSTATUS_ADDR(base) ((base) + (0xFB8))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_PIDR4_ADDR(base) ((base) + (0xFD0))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_PIDR5_ADDR(base) ((base) + (0xFD4))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_PIDR6_ADDR(base) ((base) + (0xFD8))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_PIDR7_ADDR(base) ((base) + (0xFDC))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_PIDR0_ADDR(base) ((base) + (0xFE0))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_PIDR1_ADDR(base) ((base) + (0xFE4))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_PIDR2_ADDR(base) ((base) + (0xFE8))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_PIDR3_ADDR(base) ((base) + (0xFEC))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CIDR0_ADDR(base) ((base) + (0xFF0))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CIDR1_ADDR(base) ((base) + (0xFF4))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CIDR2_ADDR(base) ((base) + (0xFF8))
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CIDR3_ADDR(base) ((base) + (0xFFC))
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int tbu_en_req : 1;
        unsigned int clk_gt_ctrl : 2;
        unsigned int pref_qos_level_en : 1;
        unsigned int pref_qos_level : 4;
        unsigned int max_tok_trans : 8;
        unsigned int fetchslot_full_level : 6;
        unsigned int reserved : 2;
        unsigned int prefslot_full_level : 6;
        unsigned int trans_hazard_size : 2;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_CR_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_CR_tbu_en_req_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_CR_tbu_en_req_END (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_CR_clk_gt_ctrl_START (1)
#define SOC_SMMUv3_TBU_SMMU_TBU_CR_clk_gt_ctrl_END (2)
#define SOC_SMMUv3_TBU_SMMU_TBU_CR_pref_qos_level_en_START (3)
#define SOC_SMMUv3_TBU_SMMU_TBU_CR_pref_qos_level_en_END (3)
#define SOC_SMMUv3_TBU_SMMU_TBU_CR_pref_qos_level_START (4)
#define SOC_SMMUv3_TBU_SMMU_TBU_CR_pref_qos_level_END (7)
#define SOC_SMMUv3_TBU_SMMU_TBU_CR_max_tok_trans_START (8)
#define SOC_SMMUv3_TBU_SMMU_TBU_CR_max_tok_trans_END (15)
#define SOC_SMMUv3_TBU_SMMU_TBU_CR_fetchslot_full_level_START (16)
#define SOC_SMMUv3_TBU_SMMU_TBU_CR_fetchslot_full_level_END (21)
#define SOC_SMMUv3_TBU_SMMU_TBU_CR_prefslot_full_level_START (24)
#define SOC_SMMUv3_TBU_SMMU_TBU_CR_prefslot_full_level_END (29)
#define SOC_SMMUv3_TBU_SMMU_TBU_CR_trans_hazard_size_START (30)
#define SOC_SMMUv3_TBU_SMMU_TBU_CR_trans_hazard_size_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int tbu_en_ack : 1;
        unsigned int tbu_connected : 1;
        unsigned int reserved_0 : 6;
        unsigned int tok_trans_gnt : 8;
        unsigned int reserved_1 : 16;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_CRACK_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_CRACK_tbu_en_ack_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_CRACK_tbu_en_ack_END (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_CRACK_tbu_connected_START (1)
#define SOC_SMMUv3_TBU_SMMU_TBU_CRACK_tbu_connected_END (1)
#define SOC_SMMUv3_TBU_SMMU_TBU_CRACK_tok_trans_gnt_START (8)
#define SOC_SMMUv3_TBU_SMMU_TBU_CRACK_tok_trans_gnt_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int mem_ctrl_s : 26;
        unsigned int reserved : 6;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_MEM_CTRL_S_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_MEM_CTRL_S_mem_ctrl_s_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_MEM_CTRL_S_mem_ctrl_s_END (25)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int mem_ctrl_bp_d1w2r : 16;
        unsigned int reserved : 16;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_MEM_CTRL_B_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_MEM_CTRL_B_mem_ctrl_bp_d1w2r_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_MEM_CTRL_B_mem_ctrl_bp_d1w2r_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int tbu_global_irpt_ns_mask : 1;
        unsigned int tbu_pmu_irpt_mask : 1;
        unsigned int tlb_inv_finish_ns_mask : 1;
        unsigned int tlb_inv_error_ns_mask : 1;
        unsigned int reserved : 28;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_IRPT_MASK_NS_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_MASK_NS_tbu_global_irpt_ns_mask_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_MASK_NS_tbu_global_irpt_ns_mask_END (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_MASK_NS_tbu_pmu_irpt_mask_START (1)
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_MASK_NS_tbu_pmu_irpt_mask_END (1)
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_MASK_NS_tlb_inv_finish_ns_mask_START (2)
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_MASK_NS_tlb_inv_finish_ns_mask_END (2)
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_MASK_NS_tlb_inv_error_ns_mask_START (3)
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_MASK_NS_tlb_inv_error_ns_mask_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int tbu_global_irpt_ns_raw : 1;
        unsigned int tbu_pmu_irpt_raw : 1;
        unsigned int tlb_inv_finish_ns_raw : 1;
        unsigned int tlb_inv_error_ns_raw : 1;
        unsigned int reserved : 28;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_IRPT_RAW_NS_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_RAW_NS_tbu_global_irpt_ns_raw_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_RAW_NS_tbu_global_irpt_ns_raw_END (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_RAW_NS_tbu_pmu_irpt_raw_START (1)
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_RAW_NS_tbu_pmu_irpt_raw_END (1)
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_RAW_NS_tlb_inv_finish_ns_raw_START (2)
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_RAW_NS_tlb_inv_finish_ns_raw_END (2)
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_RAW_NS_tlb_inv_error_ns_raw_START (3)
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_RAW_NS_tlb_inv_error_ns_raw_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int tbu_global_irpt_ns_stat : 1;
        unsigned int tbu_pmu_irpt_stat : 1;
        unsigned int tlb_inv_finish_ns_stat : 1;
        unsigned int tlb_inv_error_ns_stat : 1;
        unsigned int reserved : 28;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_IRPT_STAT_NS_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_STAT_NS_tbu_global_irpt_ns_stat_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_STAT_NS_tbu_global_irpt_ns_stat_END (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_STAT_NS_tbu_pmu_irpt_stat_START (1)
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_STAT_NS_tbu_pmu_irpt_stat_END (1)
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_STAT_NS_tlb_inv_finish_ns_stat_START (2)
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_STAT_NS_tlb_inv_finish_ns_stat_END (2)
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_STAT_NS_tlb_inv_error_ns_stat_START (3)
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_STAT_NS_tlb_inv_error_ns_stat_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int tbu_global_irpt_ns_clr : 1;
        unsigned int tbu_pmu_irpt_clr : 1;
        unsigned int tlb_inv_finish_ns_clr : 1;
        unsigned int tlb_inv_error_ns_clr : 1;
        unsigned int reserved : 28;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_IRPT_CLR_NS_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_CLR_NS_tbu_global_irpt_ns_clr_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_CLR_NS_tbu_global_irpt_ns_clr_END (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_CLR_NS_tbu_pmu_irpt_clr_START (1)
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_CLR_NS_tbu_pmu_irpt_clr_END (1)
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_CLR_NS_tlb_inv_finish_ns_clr_START (2)
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_CLR_NS_tlb_inv_finish_ns_clr_END (2)
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_CLR_NS_tlb_inv_error_ns_clr_START (3)
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_CLR_NS_tlb_inv_error_ns_clr_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int tlb_lock_clr : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_LOCK_CLR_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_LOCK_CLR_tlb_lock_clr_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_LOCK_CLR_tlb_lock_clr_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int tlb_inv_ns_op : 2;
        unsigned int reserved_0 : 6;
        unsigned int tlb_inv_ns_sid : 8;
        unsigned int tlb_inv_ns_ssid : 8;
        unsigned int tlb_inv_ns_range : 5;
        unsigned int reserved_1 : 2;
        unsigned int tlb_inv_ns_en : 1;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_TLB_INV_NS_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_TLB_INV_NS_tlb_inv_ns_op_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_TLB_INV_NS_tlb_inv_ns_op_END (1)
#define SOC_SMMUv3_TBU_SMMU_TBU_TLB_INV_NS_tlb_inv_ns_sid_START (8)
#define SOC_SMMUv3_TBU_SMMU_TBU_TLB_INV_NS_tlb_inv_ns_sid_END (15)
#define SOC_SMMUv3_TBU_SMMU_TBU_TLB_INV_NS_tlb_inv_ns_ssid_START (16)
#define SOC_SMMUv3_TBU_SMMU_TBU_TLB_INV_NS_tlb_inv_ns_ssid_END (23)
#define SOC_SMMUv3_TBU_SMMU_TBU_TLB_INV_NS_tlb_inv_ns_range_START (24)
#define SOC_SMMUv3_TBU_SMMU_TBU_TLB_INV_NS_tlb_inv_ns_range_END (28)
#define SOC_SMMUv3_TBU_SMMU_TBU_TLB_INV_NS_tlb_inv_ns_en_START (31)
#define SOC_SMMUv3_TBU_SMMU_TBU_TLB_INV_NS_tlb_inv_ns_en_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pref_num : 8;
        unsigned int reserved : 16;
        unsigned int pref_jump : 4;
        unsigned int syscache_hint_sel : 2;
        unsigned int pref_lock_mask : 1;
        unsigned int pref_en : 1;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_SWID_CFG_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_SWID_CFG_pref_num_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_SWID_CFG_pref_num_END (7)
#define SOC_SMMUv3_TBU_SMMU_TBU_SWID_CFG_pref_jump_START (24)
#define SOC_SMMUv3_TBU_SMMU_TBU_SWID_CFG_pref_jump_END (27)
#define SOC_SMMUv3_TBU_SMMU_TBU_SWID_CFG_syscache_hint_sel_START (28)
#define SOC_SMMUv3_TBU_SMMU_TBU_SWID_CFG_syscache_hint_sel_END (29)
#define SOC_SMMUv3_TBU_SMMU_TBU_SWID_CFG_pref_lock_mask_START (30)
#define SOC_SMMUv3_TBU_SMMU_TBU_SWID_CFG_pref_lock_mask_END (30)
#define SOC_SMMUv3_TBU_SMMU_TBU_SWID_CFG_pref_en_START (31)
#define SOC_SMMUv3_TBU_SMMU_TBU_SWID_CFG_pref_en_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int smmu_tbu_pidr4 : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PIDR4_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_PIDR4_smmu_tbu_pidr4_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_PIDR4_smmu_tbu_pidr4_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int smmu_tbu_pidr5 : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PIDR5_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_PIDR5_smmu_tbu_pidr5_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_PIDR5_smmu_tbu_pidr5_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int smmu_tbu_pidr6 : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PIDR6_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_PIDR6_smmu_tbu_pidr6_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_PIDR6_smmu_tbu_pidr6_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int smmu_tbu_pidr7 : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PIDR7_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_PIDR7_smmu_tbu_pidr7_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_PIDR7_smmu_tbu_pidr7_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int smmu_tbu_pidr0 : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PIDR0_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_PIDR0_smmu_tbu_pidr0_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_PIDR0_smmu_tbu_pidr0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int smmu_tbu_pidr1 : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PIDR1_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_PIDR1_smmu_tbu_pidr1_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_PIDR1_smmu_tbu_pidr1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int smmu_tbu_pidr2 : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PIDR2_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_PIDR2_smmu_tbu_pidr2_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_PIDR2_smmu_tbu_pidr2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int smmu_tbu_pidr3 : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PIDR3_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_PIDR3_smmu_tbu_pidr3_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_PIDR3_smmu_tbu_pidr3_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int smmu_tbu_cidr0 : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_CIDR0_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_CIDR0_smmu_tbu_cidr0_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_CIDR0_smmu_tbu_cidr0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int smmu_tbu_cidr1 : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_CIDR1_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_CIDR1_smmu_tbu_cidr1_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_CIDR1_smmu_tbu_cidr1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int smmu_tbu_cidr2 : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_CIDR2_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_CIDR2_smmu_tbu_cidr2_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_CIDR2_smmu_tbu_cidr2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int smmu_tbu_cidr3 : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_CIDR3_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_CIDR3_smmu_tbu_cidr3_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_CIDR3_smmu_tbu_cidr3_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ns_uarch : 1;
        unsigned int hazard_dis : 1;
        unsigned int mtlb_hitmap_dis : 1;
        unsigned int tbu_bypass : 1;
        unsigned int dummy_unlock_en : 1;
        unsigned int tlb_inv_sel : 1;
        unsigned int reserved : 26;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_SCR_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_SCR_ns_uarch_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_SCR_ns_uarch_END (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_SCR_hazard_dis_START (1)
#define SOC_SMMUv3_TBU_SMMU_TBU_SCR_hazard_dis_END (1)
#define SOC_SMMUv3_TBU_SMMU_TBU_SCR_mtlb_hitmap_dis_START (2)
#define SOC_SMMUv3_TBU_SMMU_TBU_SCR_mtlb_hitmap_dis_END (2)
#define SOC_SMMUv3_TBU_SMMU_TBU_SCR_tbu_bypass_START (3)
#define SOC_SMMUv3_TBU_SMMU_TBU_SCR_tbu_bypass_END (3)
#define SOC_SMMUv3_TBU_SMMU_TBU_SCR_dummy_unlock_en_START (4)
#define SOC_SMMUv3_TBU_SMMU_TBU_SCR_dummy_unlock_en_END (4)
#define SOC_SMMUv3_TBU_SMMU_TBU_SCR_tlb_inv_sel_START (5)
#define SOC_SMMUv3_TBU_SMMU_TBU_SCR_tlb_inv_sel_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int tbu_global_irpt_s_mask : 1;
        unsigned int reserved_0 : 1;
        unsigned int tlb_inv_finish_s_mask : 1;
        unsigned int tlb_inv_error_s_mask : 1;
        unsigned int reserved_1 : 28;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_IRPT_MASK_S_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_MASK_S_tbu_global_irpt_s_mask_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_MASK_S_tbu_global_irpt_s_mask_END (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_MASK_S_tlb_inv_finish_s_mask_START (2)
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_MASK_S_tlb_inv_finish_s_mask_END (2)
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_MASK_S_tlb_inv_error_s_mask_START (3)
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_MASK_S_tlb_inv_error_s_mask_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int tbu_global_irpt_s_raw : 1;
        unsigned int reserved_0 : 1;
        unsigned int tlb_inv_finish_s_raw : 1;
        unsigned int tlb_inv_error_s_raw : 1;
        unsigned int reserved_1 : 28;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_IRPT_RAW_S_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_RAW_S_tbu_global_irpt_s_raw_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_RAW_S_tbu_global_irpt_s_raw_END (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_RAW_S_tlb_inv_finish_s_raw_START (2)
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_RAW_S_tlb_inv_finish_s_raw_END (2)
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_RAW_S_tlb_inv_error_s_raw_START (3)
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_RAW_S_tlb_inv_error_s_raw_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int tbu_global_irpt_s_stat : 1;
        unsigned int reserved_0 : 1;
        unsigned int tlb_inv_finish_s_stat : 1;
        unsigned int tlb_inv_error_s_stat : 1;
        unsigned int reserved_1 : 28;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_IRPT_STAT_S_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_STAT_S_tbu_global_irpt_s_stat_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_STAT_S_tbu_global_irpt_s_stat_END (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_STAT_S_tlb_inv_finish_s_stat_START (2)
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_STAT_S_tlb_inv_finish_s_stat_END (2)
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_STAT_S_tlb_inv_error_s_stat_START (3)
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_STAT_S_tlb_inv_error_s_stat_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int tbu_global_irpt_s_clr : 1;
        unsigned int reserved_0 : 1;
        unsigned int tlb_inv_finish_s_clr : 1;
        unsigned int tlb_inv_error_s_clr : 1;
        unsigned int reserved_1 : 28;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_IRPT_CLR_S_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_CLR_S_tbu_global_irpt_s_clr_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_CLR_S_tbu_global_irpt_s_clr_END (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_CLR_S_tlb_inv_finish_s_clr_START (2)
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_CLR_S_tlb_inv_finish_s_clr_END (2)
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_CLR_S_tlb_inv_error_s_clr_START (3)
#define SOC_SMMUv3_TBU_SMMU_TBU_IRPT_CLR_S_tlb_inv_error_s_clr_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dbg_cfg_addr : 16;
        unsigned int dbg_cfg_rd : 1;
        unsigned int reserved : 15;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_DBG_PORT_IN_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_DBG_PORT_IN_dbg_cfg_addr_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_DBG_PORT_IN_dbg_cfg_addr_END (15)
#define SOC_SMMUv3_TBU_SMMU_TBU_DBG_PORT_IN_dbg_cfg_rd_START (16)
#define SOC_SMMUv3_TBU_SMMU_TBU_DBG_PORT_IN_dbg_cfg_rd_END (16)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dbg_cfg_rdata_0 : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_DBG_PORT_OUT_0_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_DBG_PORT_OUT_0_dbg_cfg_rdata_0_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_DBG_PORT_OUT_0_dbg_cfg_rdata_0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dbg_cfg_rdata_1 : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_DBG_PORT_OUT_1_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_DBG_PORT_OUT_1_dbg_cfg_rdata_1_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_DBG_PORT_OUT_1_dbg_cfg_rdata_1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dbg_cfg_rdata_2 : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_DBG_PORT_OUT_2_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_DBG_PORT_OUT_2_dbg_cfg_rdata_2_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_DBG_PORT_OUT_2_dbg_cfg_rdata_2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dbg_cfg_rdata_3 : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_DBG_PORT_OUT_3_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_DBG_PORT_OUT_3_dbg_cfg_rdata_3_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_DBG_PORT_OUT_3_dbg_cfg_rdata_3_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int tlb_inv_s_op : 2;
        unsigned int reserved_0 : 6;
        unsigned int tlb_inv_s_sid : 8;
        unsigned int tlb_inv_s_ssid : 8;
        unsigned int tlb_inv_s_range : 5;
        unsigned int reserved_1 : 2;
        unsigned int tlb_inv_s_en : 1;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_TLB_INV_S_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_TLB_INV_S_tlb_inv_s_op_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_TLB_INV_S_tlb_inv_s_op_END (1)
#define SOC_SMMUv3_TBU_SMMU_TBU_TLB_INV_S_tlb_inv_s_sid_START (8)
#define SOC_SMMUv3_TBU_SMMU_TBU_TLB_INV_S_tlb_inv_s_sid_END (15)
#define SOC_SMMUv3_TBU_SMMU_TBU_TLB_INV_S_tlb_inv_s_ssid_START (16)
#define SOC_SMMUv3_TBU_SMMU_TBU_TLB_INV_S_tlb_inv_s_ssid_END (23)
#define SOC_SMMUv3_TBU_SMMU_TBU_TLB_INV_S_tlb_inv_s_range_START (24)
#define SOC_SMMUv3_TBU_SMMU_TBU_TLB_INV_S_tlb_inv_s_range_END (28)
#define SOC_SMMUv3_TBU_SMMU_TBU_TLB_INV_S_tlb_inv_s_en_START (31)
#define SOC_SMMUv3_TBU_SMMU_TBU_TLB_INV_S_tlb_inv_s_en_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int protect_en : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PROT_EN_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_PROT_EN_protect_en_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_PROT_EN_protect_en_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int evcntr : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PMCG_EVCNTR_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_EVCNTR_evcntr_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_EVCNTR_evcntr_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int event : 8;
        unsigned int reserved : 21;
        unsigned int filter_sid_span : 1;
        unsigned int filter_sec_sid : 1;
        unsigned int ovfcap : 1;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PMCG_EVTYPER0_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_EVTYPER0_event_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_EVTYPER0_event_END (7)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_EVTYPER0_filter_sid_span_START (29)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_EVTYPER0_filter_sid_span_END (29)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_EVTYPER0_filter_sec_sid_START (30)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_EVTYPER0_filter_sec_sid_END (30)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_EVTYPER0_ovfcap_START (31)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_EVTYPER0_ovfcap_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int event : 8;
        unsigned int reserved : 23;
        unsigned int ovfcap : 1;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PMCG_EVTYPER_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_EVTYPER_event_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_EVTYPER_event_END (7)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_EVTYPER_ovfcap_START (31)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_EVTYPER_ovfcap_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int svr : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PMCG_SVR_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_SVR_svr_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_SVR_svr_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int streamid : 24;
        unsigned int reserved : 8;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PMCG_SMR_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_SMR_streamid_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_SMR_streamid_END (23)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int cnten : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CNTENSET0_0_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CNTENSET0_0_cnten_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CNTENSET0_0_cnten_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CNTENSET0_1_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int cntclr : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CNTENCLR0_0_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CNTENCLR0_0_cntclr_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CNTENCLR0_0_cntclr_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CNTENCLR0_1_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int inten : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PMCG_INTENSET0_0_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_INTENSET0_0_inten_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_INTENSET0_0_inten_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PMCG_INTENSET0_1_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int intclr : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PMCG_INTENCLR0_0_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_INTENCLR0_0_intclr_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_INTENCLR0_0_intclr_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PMCG_INTENCLR0_1_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ovsclr : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PMCG_OVSCLR0_0_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_OVSCLR0_0_ovsclr_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_OVSCLR0_0_ovsclr_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PMCG_OVSCLR0_1_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ovsset : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PMCG_OVSSET0_0_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_OVSSET0_0_ovsset_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_OVSSET0_0_ovsset_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PMCG_OVSSET0_1_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int capture : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CAPR_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CAPR_capture_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CAPR_capture_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int so : 1;
        unsigned int nsra : 1;
        unsigned int nsmsi : 1;
        unsigned int reserved : 28;
        unsigned int readasone : 1;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PMCG_SCR_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_SCR_so_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_SCR_so_END (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_SCR_nsra_START (1)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_SCR_nsra_END (1)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_SCR_nsmsi_START (2)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_SCR_nsmsi_END (2)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_SCR_readasone_START (31)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_SCR_readasone_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int nctr : 6;
        unsigned int reserved_0 : 2;
        unsigned int size : 6;
        unsigned int reserved_1 : 6;
        unsigned int reloc_ctrs : 1;
        unsigned int msi : 1;
        unsigned int capture : 1;
        unsigned int sid_filter_type : 1;
        unsigned int reserved_2 : 8;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CFGR_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CFGR_nctr_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CFGR_nctr_END (5)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CFGR_size_START (8)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CFGR_size_END (13)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CFGR_reloc_ctrs_START (20)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CFGR_reloc_ctrs_END (20)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CFGR_msi_START (21)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CFGR_msi_END (21)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CFGR_capture_START (22)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CFGR_capture_END (22)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CFGR_sid_filter_type_START (23)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CFGR_sid_filter_type_END (23)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int e : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CR_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CR_e_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CR_e_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int event_en : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CEID0_0_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CEID0_0_event_en_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CEID0_0_event_en_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int event_en : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CEID0_1_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CEID0_1_event_en_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CEID0_1_event_en_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int event_en : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CEID1_0_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CEID1_0_event_en_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CEID1_0_event_en_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int event_en : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CEID1_1_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CEID1_1_event_en_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CEID1_1_event_en_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int irqen : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PMCG_IRQ_CTRL_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_IRQ_CTRL_irqen_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_IRQ_CTRL_irqen_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int irqen : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PMCG_IRQ_CTRLACK_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_IRQ_CTRLACK_irqen_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_IRQ_CTRLACK_irqen_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PMCG_IRQ_CFG0_0_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PMCG_IRQ_CFG0_1_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PMCG_IRQ_CFG1_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PMCG_IRQ_CFG2_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PMCG_IRQ_STATUS_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int archminorrev : 4;
        unsigned int archmajorrev : 4;
        unsigned int reserved : 24;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PMCG_AIDR_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_AIDR_archminorrev_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_AIDR_archminorrev_END (3)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_AIDR_archmajorrev_START (4)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_AIDR_archmajorrev_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int smmu_pmcg_pmauthstatus : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PMCG_PMAUTHSTATUS_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_PMAUTHSTATUS_smmu_pmcg_pmauthstatus_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_PMAUTHSTATUS_smmu_pmcg_pmauthstatus_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int smmu_pmcg_pidr4 : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PMCG_PIDR4_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_PIDR4_smmu_pmcg_pidr4_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_PIDR4_smmu_pmcg_pidr4_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int smmu_pmcg_pidr5 : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PMCG_PIDR5_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_PIDR5_smmu_pmcg_pidr5_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_PIDR5_smmu_pmcg_pidr5_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int smmu_pmcg_pidr6 : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PMCG_PIDR6_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_PIDR6_smmu_pmcg_pidr6_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_PIDR6_smmu_pmcg_pidr6_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int smmu_pmcg_pidr7 : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PMCG_PIDR7_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_PIDR7_smmu_pmcg_pidr7_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_PIDR7_smmu_pmcg_pidr7_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int smmu_pmcg_pidr0 : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PMCG_PIDR0_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_PIDR0_smmu_pmcg_pidr0_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_PIDR0_smmu_pmcg_pidr0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int smmu_pmcg_pidr1 : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PMCG_PIDR1_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_PIDR1_smmu_pmcg_pidr1_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_PIDR1_smmu_pmcg_pidr1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int smmu_pmcg_pidr2 : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PMCG_PIDR2_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_PIDR2_smmu_pmcg_pidr2_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_PIDR2_smmu_pmcg_pidr2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int smmu_pmcg_pidr3 : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PMCG_PIDR3_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_PIDR3_smmu_pmcg_pidr3_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_PIDR3_smmu_pmcg_pidr3_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int smmu_pmcg_cidr0 : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CIDR0_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CIDR0_smmu_pmcg_cidr0_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CIDR0_smmu_pmcg_cidr0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int smmu_pmcg_cidr1 : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CIDR1_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CIDR1_smmu_pmcg_cidr1_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CIDR1_smmu_pmcg_cidr1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int smmu_pmcg_cidr2 : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CIDR2_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CIDR2_smmu_pmcg_cidr2_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CIDR2_smmu_pmcg_cidr2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int smmu_pmcg_cidr3 : 32;
    } reg;
} SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CIDR3_UNION;
#endif
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CIDR3_smmu_pmcg_cidr3_START (0)
#define SOC_SMMUv3_TBU_SMMU_TBU_PMCG_CIDR3_smmu_pmcg_cidr3_END (31)
#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
#endif
