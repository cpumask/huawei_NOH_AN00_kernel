#ifndef __SOC_NPU_AICORE_INTERFACE_H__
#define __SOC_NPU_AICORE_INTERFACE_H__ 
#ifdef __cplusplus
    #if __cplusplus
        extern "C" {
    #endif
#endif
#ifndef __SOC_H_FOR_ASM__
#define SOC_NPU_AICORE_RUN_STALL_ADDR(base) ((base) + (0x00000000UL))
#define SOC_NPU_AICORE_DBG_CTRL_ADDR(base) ((base) + (0x00000008UL))
#define SOC_NPU_AICORE_RST_CTRL_ADDR(base) ((base) + (0x00000010UL))
#define SOC_NPU_AICORE_FAST_PATH_CTRL_ADDR(base) ((base) + (0x00000018UL))
#define SOC_NPU_AICORE_AI_CORE_INT_ADDR(base) ((base) + (0x00000020UL))
#define SOC_NPU_AICORE_AI_CORE_INT_MASK_ADDR(base) ((base) + (0x00000028UL))
#define SOC_NPU_AICORE_RST_CNT_ADDR(base) ((base) + (0x00000030UL))
#define SOC_NPU_AICORE_ECC_EN_ADDR(base) ((base) + (0x00000038UL))
#define SOC_NPU_AICORE_AXI_CLAMP_CTRL_ADDR(base) ((base) + (0x00000040UL))
#define SOC_NPU_AICORE_AXI_CLAMP_STATE_ADDR(base) ((base) + (0x00000048UL))
#define SOC_NPU_AICORE_CLK_GATE_MASK_ADDR(base) ((base) + (0x00000050UL))
#define SOC_NPU_AICORE_CLK_DELAY_CNT_ADDR(base) ((base) + (0x00000058UL))
#define SOC_NPU_AICORE_TOP_ICG_MBIST_ADDR(base) ((base) + (0x00000060UL))
#define SOC_NPU_AICORE_AXI_CLAMP_STATE_1_ADDR(base) ((base) + (0x00000068UL))
#define SOC_NPU_AICORE_LOCK_BYPASS_ADDR(base) ((base) + (0x00000070UL))
#define SOC_NPU_AICORE_SYSCTRL_LOCK_ADDR(base) ((base) + (0x00000078UL))
#define SOC_NPU_AICORE_PC_START_ADDR(base) ((base) + (0x00000080UL))
#define SOC_NPU_AICORE_PARA_BASE_ADDR(base) ((base) + (0x00000088UL))
#define SOC_NPU_AICORE_SMMU_SUBSTREAMID_ADDR(base) ((base) + (0x00000090UL))
#define SOC_NPU_AICORE_TASK_CFG_ADDR(base) ((base) + (0x00000098UL))
#define SOC_NPU_AICORE_DATA_MAIN_BASE_ADDR(base) ((base) + (0x000000A0UL))
#define SOC_NPU_AICORE_DATA_UB_BASE_ADDR(base) ((base) + (0x000000A8UL))
#define SOC_NPU_AICORE_DATA_SIZE_ADDR(base) ((base) + (0x000000B0UL))
#define SOC_NPU_AICORE_L2_REMAP_CFG0_ADDR(base) ((base) + (0x000000B8UL))
#define SOC_NPU_AICORE_L2_REMAP_CFG1_ADDR(base) ((base) + (0x000000C0UL))
#define SOC_NPU_AICORE_L2_REMAP_CFG2_ADDR(base) ((base) + (0x000000C8UL))
#define SOC_NPU_AICORE_L2_REMAP_CFG3_ADDR(base) ((base) + (0x000000D0UL))
#define SOC_NPU_AICORE_L2_REMAP_CFG4_ADDR(base) ((base) + (0x000000D8UL))
#define SOC_NPU_AICORE_L2_REMAP_CFG5_ADDR(base) ((base) + (0x000000E0UL))
#define SOC_NPU_AICORE_L2_REMAP_CFG6_ADDR(base) ((base) + (0x000000E8UL))
#define SOC_NPU_AICORE_L2_REMAP_CFG7_ADDR(base) ((base) + (0x000000F0UL))
#define SOC_NPU_AICORE_L2_VADDR_BASE_ADDR(base) ((base) + (0x000000F8UL))
#define SOC_NPU_AICORE_DBG_RW_CTRL_ADDR(base) ((base) + (0x000000100UL))
#define SOC_NPU_AICORE_DBG_ADDR_ADDR(base) ((base) + (0x000000108UL))
#define SOC_NPU_AICORE_DBG_DATA0_ADDR(base) ((base) + (0x000000110UL))
#define SOC_NPU_AICORE_DBG_DATA1_ADDR(base) ((base) + (0x000000118UL))
#define SOC_NPU_AICORE_DBG_DATA2_ADDR(base) ((base) + (0x000000120UL))
#define SOC_NPU_AICORE_DBG_DATA3_ADDR(base) ((base) + (0x000000128UL))
#define SOC_NPU_AICORE_DFX_DATA_ADDR(base) ((base) + (0x000000130UL))
#define SOC_NPU_AICORE_DBG_STATUS_ADDR(base) ((base) + (0x000000138UL))
#define SOC_NPU_AICORE_RESERVED_REG00_ADDR(base) ((base) + (0x000000140UL))
#define SOC_NPU_AICORE_RESERVED_REG01_ADDR(base) ((base) + (0x000000148UL))
#define SOC_NPU_AICORE_DJTAG_CLK_BYPASS_ADDR(base) ((base) + (0x00000150UL))
#define SOC_NPU_AICORE_PMU_CTRL_ADDR(base) ((base) + (0x000000200UL))
#define SOC_NPU_AICORE_PMU_MIN_OV_CNT_ADDR(base) ((base) + (0x000000208UL))
#define SOC_NPU_AICORE_PMU_CNT0_ADDR(base) ((base) + (0x000000210UL))
#define SOC_NPU_AICORE_PMU_CNT1_ADDR(base) ((base) + (0x000000218UL))
#define SOC_NPU_AICORE_PMU_CNT2_ADDR(base) ((base) + (0x000000220UL))
#define SOC_NPU_AICORE_PMU_CNT3_ADDR(base) ((base) + (0x000000228UL))
#define SOC_NPU_AICORE_PMU_CNT4_ADDR(base) ((base) + (0x000000230UL))
#define SOC_NPU_AICORE_PMU_CNT5_ADDR(base) ((base) + (0x000000238UL))
#define SOC_NPU_AICORE_PMU_CNT6_ADDR(base) ((base) + (0x000000240UL))
#define SOC_NPU_AICORE_PMU_CNT7_ADDR(base) ((base) + (0x000000248UL))
#define SOC_NPU_AICORE_PMU_TASK_CYC_CNT_ADDR(base) ((base) + (0x000000250UL))
#define SOC_NPU_AICORE_PMU_CNT0_IDX_ADDR(base) ((base) + (0x000000260UL))
#define SOC_NPU_AICORE_PMU_CNT1_IDX_ADDR(base) ((base) + (0x000000268UL))
#define SOC_NPU_AICORE_PMU_CNT2_IDX_ADDR(base) ((base) + (0x000000270UL))
#define SOC_NPU_AICORE_PMU_CNT3_IDX_ADDR(base) ((base) + (0x000000278UL))
#define SOC_NPU_AICORE_PMU_CNT4_IDX_ADDR(base) ((base) + (0x000000280UL))
#define SOC_NPU_AICORE_PMU_CNT5_IDX_ADDR(base) ((base) + (0x000000288UL))
#define SOC_NPU_AICORE_PMU_CNT6_IDX_ADDR(base) ((base) + (0x000000290UL))
#define SOC_NPU_AICORE_PMU_CNT7_IDX_ADDR(base) ((base) + (0x000000298UL))
#define SOC_NPU_AICORE_PMU_START_CNT_CYC_ADDR(base) ((base) + (0x0000002A0UL))
#define SOC_NPU_AICORE_PMU_STOP_CNT_CYC_ADDR(base) ((base) + (0x0000002A8UL))
#define SOC_NPU_AICORE_IFU_CTRL_ADDR(base) ((base) + (0x000000300UL))
#define SOC_NPU_AICORE_IC_INV_CTRL_ADDR(base) ((base) + (0x000000308UL))
#define SOC_NPU_AICORE_IC_INV_VA_ADDR(base) ((base) + (0x000000310UL))
#define SOC_NPU_AICORE_IC_INV_STATUS_ADDR(base) ((base) + (0x000000318UL))
#define SOC_NPU_AICORE_HW_BKPT_EN_ADDR(base) ((base) + (0x00000400UL))
#define SOC_NPU_AICORE_HW_BKPT0_PC_ADDR(base) ((base) + (0x00000408UL))
#define SOC_NPU_AICORE_HW_BKPT1_PC_ADDR(base) ((base) + (0x00000410UL))
#define SOC_NPU_AICORE_HW_BKPT2_PC_ADDR(base) ((base) + (0x00000418UL))
#define SOC_NPU_AICORE_HW_BKPT3_PC_ADDR(base) ((base) + (0x00000420UL))
#define SOC_NPU_AICORE_HW_BKPT4_PC_ADDR(base) ((base) + (0x00000428UL))
#define SOC_NPU_AICORE_HW_BKPT5_PC_ADDR(base) ((base) + (0x00000430UL))
#define SOC_NPU_AICORE_HW_BKPT6_PC_ADDR(base) ((base) + (0x00000438UL))
#define SOC_NPU_AICORE_HW_BKPT7_PC_ADDR(base) ((base) + (0x00000440UL))
#define SOC_NPU_AICORE_HW_BKPT8_PC_ADDR(base) ((base) + (0x00000448UL))
#define SOC_NPU_AICORE_HW_BKPT9_PC_ADDR(base) ((base) + (0x00000450UL))
#define SOC_NPU_AICORE_HW_BKPT10_PC_ADDR(base) ((base) + (0x00000458UL))
#define SOC_NPU_AICORE_HW_BKPT11_PC_ADDR(base) ((base) + (0x00000460UL))
#define SOC_NPU_AICORE_HW_BKPT12_PC_ADDR(base) ((base) + (0x00000468UL))
#define SOC_NPU_AICORE_HW_BKPT13_PC_ADDR(base) ((base) + (0x00000470UL))
#define SOC_NPU_AICORE_HW_BKPT14_PC_ADDR(base) ((base) + (0x00000478UL))
#define SOC_NPU_AICORE_HW_BKPT15_PC_ADDR(base) ((base) + (0x00000480UL))
#define SOC_NPU_AICORE_CCU_IQ_TIMEOUT_ADDR(base) ((base) + (0x00000488UL))
#define SOC_NPU_AICORE_CCU_CTRL_ADDR(base) ((base) + (0x00000490UL))
#define SOC_NPU_AICORE_SPR_STATUS_ADDR(base) ((base) + (0x000004C0UL))
#define SOC_NPU_AICORE_COND_SPR_ADDR(base) ((base) + (0x000004C8UL))
#define SOC_NPU_AICORE_CCU_REDIRECT_IFU_STATUS_ADDR(base) ((base) + (0x000004D0UL))
#define SOC_NPU_AICORE_BIU_CTRL0_ADDR(base) ((base) + (0x00000500UL))
#define SOC_NPU_AICORE_BIU_CTRL1_ADDR(base) ((base) + (0x00000508UL))
#define SOC_NPU_AICORE_BIU_CTRL2_ADDR(base) ((base) + (0x00000510UL))
#define SOC_NPU_AICORE_BIU_STATUS0_ADDR(base) ((base) + (0x00000518UL))
#define SOC_NPU_AICORE_BIU_STATUS1_ADDR(base) ((base) + (0x00000520UL))
#define SOC_NPU_AICORE_BIU_STATUS2_ADDR(base) ((base) + (0x00000528UL))
#define SOC_NPU_AICORE_BIU_STATUS3_ADDR(base) ((base) + (0x00000530UL))
#define SOC_NPU_AICORE_BIU_STATUS4_ADDR(base) ((base) + (0x00000538UL))
#define SOC_NPU_AICORE_BIU_STATUS5_ADDR(base) ((base) + (0x00000540UL))
#define SOC_NPU_AICORE_BIU_SMMU_STREAMID_ADDR(base) ((base) + (0x00000548UL))
#define SOC_NPU_AICORE_BIU_L2_REMAP_PADDR_BASE_ADDR(base) ((base) + (0x00000550UL))
#define SOC_NPU_AICORE_BIU_L2_SIZE_ADDR(base) ((base) + (0x00000558UL))
#define SOC_NPU_AICORE_BIU_L2_PAGE_SIZE_ADDR(base) ((base) + (0x00000560UL))
#define SOC_NPU_AICORE_BIU_CTRL6_ADDR(base) ((base) + (0x00000568UL))
#define SOC_NPU_AICORE_BIU_STATUS8_ADDR(base) ((base) + (0x00000570UL))
#define SOC_NPU_AICORE_BIU_STREAM_NS_ADDR(base) ((base) + (0x00000578UL))
#define SOC_NPU_AICORE_BIU_CTRL7_ADDR(base) ((base) + (0x00000580UL))
#define SOC_NPU_AICORE_BIU_CTRL8_ADDR(base) ((base) + (0x00000588UL))
#define SOC_NPU_AICORE_BIU_SMMU_CFG_ADDR(base) ((base) + (0x00000590UL))
#define SOC_NPU_AICORE_SMMU_SEC_STREAMID_ADDR(base) ((base) + (0x00000598UL))
#define SOC_NPU_AICORE_BIU_STATUS9_ADDR(base) ((base) + (0x000005C0UL))
#define SOC_NPU_AICORE_CORE_ID_ADDR(base) ((base) + (0x00000600UL))
#define SOC_NPU_AICORE_AIC_VER_ADDR(base) ((base) + (0x00000608UL))
#define SOC_NPU_AICORE_SMMU_VER_ADDR(base) ((base) + (0x00000610UL))
#define SOC_NPU_AICORE_DISPATCH_VER_ADDR(base) ((base) + (0x00000618UL))
#define SOC_NPU_AICORE_AA_VER_ADDR(base) ((base) + (0x00000620UL))
#define SOC_NPU_AICORE_CRG_VER_ADDR(base) ((base) + (0x00000628UL))
#define SOC_NPU_AICORE_POWER_MODE_CTRL_ADDR(base) ((base) + (0x00000630UL))
#define SOC_NPU_AICORE_MEM_TMOD_CTRL_ADDR(base) ((base) + (0x00000638UL))
#define SOC_NPU_AICORE_AIC_ERROR_ADDR(base) ((base) + (0x00000700UL))
#define SOC_NPU_AICORE_AIC_ERROR_MASK_ADDR(base) ((base) + (0x00000708UL))
#define SOC_NPU_AICORE_BIU_ERR_INFO_ADDR(base) ((base) + (0x00000710UL))
#define SOC_NPU_AICORE_CCU_ERR_INFO_ADDR(base) ((base) + (0x00000718UL))
#define SOC_NPU_AICORE_CUBE_ERR_INFO_ADDR(base) ((base) + (0x00000720UL))
#define SOC_NPU_AICORE_IFU_ERR_INFO_ADDR(base) ((base) + (0x00000728UL))
#define SOC_NPU_AICORE_MTE_ERR_INFO_ADDR(base) ((base) + (0x00000730UL))
#define SOC_NPU_AICORE_VEC_ERR_INFO_ADDR(base) ((base) + (0x00000738UL))
#define SOC_NPU_AICORE_MTE_ECC_1BIT_ERR_ADDR(base) ((base) + (0x00000740UL))
#define SOC_NPU_AICORE_VEC_CUBE_ECC_1BIT_ERR_ADDR(base) ((base) + (0x00000748UL))
#define SOC_NPU_AICORE_AIC_ERROR_FORCE_ADDR(base) ((base) + (0x00000750UL))
#define SOC_NPU_AICORE_AIC_ERROR_FORCE2_ADDR(base) ((base) + (0x00000758UL))
#define SOC_NPU_AICORE_AIC_ERROR2_ADDR(base) ((base) + (0x00000760UL))
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_ADDR(base) ((base) + (0x00000768UL))
#define SOC_NPU_AICORE_CCU_BUS_ERR_INFO_ADDR(base) ((base) + (0x00000770UL))
#define SOC_NPU_AICORE_AIC_ERROR1_ADDR(base) ((base) + (0x00000780UL))
#define SOC_NPU_AICORE_AIC_ERROR_MASK1_ADDR(base) ((base) + (0x00000788UL))
#define SOC_NPU_AICORE_PCT_CTRL_ADDR(base) ((base) + (0x00000800UL))
#define SOC_NPU_AICORE_PCT_STATUS_ADDR(base) ((base) + (0x00000808UL))
#define SOC_NPU_AICORE_PCT_NUM_ENTRIES_ADDR(base) ((base) + (0x00000810UL))
#define SOC_NPU_AICORE_PCT_START_CNT_CYC_ADDR(base) ((base) + (0x00000820UL))
#define SOC_NPU_AICORE_PCT_STOP_CNT_CYC_ADDR(base) ((base) + (0x00000828UL))
#define SOC_NPU_AICORE_PCT_OV_TIMESTAMP_ADDR(base) ((base) + (0x00000830UL))
#define SOC_NPU_AICORE_VEC_CTRL_ADDR(base) ((base) + (0x00000900UL))
#define SOC_NPU_AICORE_VEC_RESERVED_REG00_ADDR(base) ((base) + (0x000000a00UL))
#define SOC_NPU_AICORE_VEC_RESERVED_REG01_ADDR(base) ((base) + (0x000000A08UL))
#define SOC_NPU_AICORE_CUBE_RESERVED_REG00_ADDR(base) ((base) + (0x000000A10UL))
#define SOC_NPU_AICORE_CUBE_RESERVED_REG01_ADDR(base) ((base) + (0x000000A18UL))
#define SOC_NPU_AICORE_SC_RESERVED_REG00_ADDR(base) ((base) + (0x000000A20UL))
#define SOC_NPU_AICORE_SC_RESERVED_REG01_ADDR(base) ((base) + (0x000000A28UL))
#define SOC_NPU_AICORE_SC_RESERVED_RO_REG00_ADDR(base) ((base) + (0x000000A30UL))
#define SOC_NPU_AICORE_SC_RESERVED_RO_REG01_ADDR(base) ((base) + (0x000000A38UL))
#define SOC_NPU_AICORE_SC_RESERVED_REG02_ADDR(base) ((base) + (0x000000A40UL))
#define SOC_NPU_AICORE_SC_RESERVED_REG03_ADDR(base) ((base) + (0x000000A48UL))
#define SOC_NPU_AICORE_VEC_RESERVED_RO_REG00_ADDR(base) ((base) + (0x000000A50UL))
#define SOC_NPU_AICORE_VEC_RESERVED_RO_REG01_ADDR(base) ((base) + (0x000000A58UL))
#define SOC_NPU_AICORE_CUBE_RESERVED_RO_REG00_ADDR(base) ((base) + (0x000000A60UL))
#define SOC_NPU_AICORE_CUBE_RESERVED_RO_REG01_ADDR(base) ((base) + (0x000000A68UL))
#define SOC_NPU_AICORE_MTE_CTRL_ADDR(base) ((base) + (0x00000b00UL))
#define SOC_NPU_AICORE_CUBE_CTRL_ADDR(base) ((base) + (0x00000c00UL))
#define SOC_NPU_AICORE_BIU8_STATUS2_ADDR(base) ((base) + (0x00000D28UL))
#define SOC_NPU_AICORE_BIU8_STATUS3_ADDR(base) ((base) + (0x00000D30UL))
#define SOC_NPU_AICORE_BIU8_STATUS4_ADDR(base) ((base) + (0x00000D38UL))
#define SOC_NPU_AICORE_BIU8_SMMU_STREAMID_ADDR(base) ((base) + (0x00000D48UL))
#define SOC_NPU_AICORE_BIU8_L2_REMAP_PADDR_BASE_ADDR(base) ((base) + (0x00000D50UL))
#define SOC_NPU_AICORE_BIU8_L2_PAGE_SIZE_ADDR(base) ((base) + (0x00000D58UL))
#define SOC_NPU_AICORE_BIU8_L2_SIZE_ADDR(base) ((base) + (0x00000D60UL))
#define SOC_NPU_AICORE_BIU8_CTRL3_ADDR(base) ((base) + (0x00000D80UL))
#define SOC_NPU_AICORE_BIU8_CTRL4_ADDR(base) ((base) + (0x00000D88UL))
#define SOC_NPU_AICORE_BIU8_CTRL5_ADDR(base) ((base) + (0x00000D90UL))
#define SOC_NPU_AICORE_BIU8_STATUS6_ADDR(base) ((base) + (0x00000D98UL))
#define SOC_NPU_AICORE_BIU8_STATUS7_ADDR(base) ((base) + (0x00000DA0UL))
#define SOC_NPU_AICORE_CCU_TIME_COMPARE_COUNTER_CTRL_ADDR(base) ((base) + (0x00000E00UL))
#define SOC_NPU_AICORE_CCU_TIME_COMPARE_COUNTER_INT_ADDR(base) ((base) + (0x00000E08UL))
#define SOC_NPU_AICORE_AIC_SECURE_EN_ADDR(base) ((base) + (0x00000F00UL))
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_CTRL_ADDR(base) ((base) + (0x00001000UL))
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_STATUS_ADDR(base) ((base) + (0x00001008UL))
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_WDATA0_ADDR(base) ((base) + (0x00001010UL))
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_WDATA1_ADDR(base) ((base) + (0x00001018UL))
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_WDATA2_ADDR(base) ((base) + (0x00001020UL))
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_WDATA3_ADDR(base) ((base) + (0x00001028UL))
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_WDATA4_ADDR(base) ((base) + (0x00001030UL))
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_RDATA0_ADDR(base) ((base) + (0x00001038UL))
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_RDATA1_ADDR(base) ((base) + (0x00001040UL))
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_RDATA2_ADDR(base) ((base) + (0x00001048UL))
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_RDATA3_ADDR(base) ((base) + (0x00001050UL))
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_RDATA4_ADDR(base) ((base) + (0x00001058UL))
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_CTRL_ADDR(base) ((base) + (0x00001060UL))
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_STATUS_ADDR(base) ((base) + (0x00001068UL))
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_WDATA0_ADDR(base) ((base) + (0x00001070UL))
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_WDATA1_ADDR(base) ((base) + (0x00001078UL))
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_WDATA2_ADDR(base) ((base) + (0x00001080UL))
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_WDATA3_ADDR(base) ((base) + (0x00001088UL))
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_WDATA4_ADDR(base) ((base) + (0x00001090UL))
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_RDATA0_ADDR(base) ((base) + (0x00001098UL))
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_RDATA1_ADDR(base) ((base) + (0x000010A0UL))
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_RDATA2_ADDR(base) ((base) + (0x000010A8UL))
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_RDATA3_ADDR(base) ((base) + (0x000010B0UL))
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_RDATA4_ADDR(base) ((base) + (0x000010B8UL))
#else
#define SOC_NPU_AICORE_RUN_STALL_ADDR(base) ((base) + (0x00000000))
#define SOC_NPU_AICORE_DBG_CTRL_ADDR(base) ((base) + (0x00000008))
#define SOC_NPU_AICORE_RST_CTRL_ADDR(base) ((base) + (0x00000010))
#define SOC_NPU_AICORE_FAST_PATH_CTRL_ADDR(base) ((base) + (0x00000018))
#define SOC_NPU_AICORE_AI_CORE_INT_ADDR(base) ((base) + (0x00000020))
#define SOC_NPU_AICORE_AI_CORE_INT_MASK_ADDR(base) ((base) + (0x00000028))
#define SOC_NPU_AICORE_RST_CNT_ADDR(base) ((base) + (0x00000030))
#define SOC_NPU_AICORE_ECC_EN_ADDR(base) ((base) + (0x00000038))
#define SOC_NPU_AICORE_AXI_CLAMP_CTRL_ADDR(base) ((base) + (0x00000040))
#define SOC_NPU_AICORE_AXI_CLAMP_STATE_ADDR(base) ((base) + (0x00000048))
#define SOC_NPU_AICORE_CLK_GATE_MASK_ADDR(base) ((base) + (0x00000050))
#define SOC_NPU_AICORE_CLK_DELAY_CNT_ADDR(base) ((base) + (0x00000058))
#define SOC_NPU_AICORE_TOP_ICG_MBIST_ADDR(base) ((base) + (0x00000060))
#define SOC_NPU_AICORE_AXI_CLAMP_STATE_1_ADDR(base) ((base) + (0x00000068))
#define SOC_NPU_AICORE_LOCK_BYPASS_ADDR(base) ((base) + (0x00000070))
#define SOC_NPU_AICORE_SYSCTRL_LOCK_ADDR(base) ((base) + (0x00000078))
#define SOC_NPU_AICORE_PC_START_ADDR(base) ((base) + (0x00000080))
#define SOC_NPU_AICORE_PARA_BASE_ADDR(base) ((base) + (0x00000088))
#define SOC_NPU_AICORE_SMMU_SUBSTREAMID_ADDR(base) ((base) + (0x00000090))
#define SOC_NPU_AICORE_TASK_CFG_ADDR(base) ((base) + (0x00000098))
#define SOC_NPU_AICORE_DATA_MAIN_BASE_ADDR(base) ((base) + (0x000000A0))
#define SOC_NPU_AICORE_DATA_UB_BASE_ADDR(base) ((base) + (0x000000A8))
#define SOC_NPU_AICORE_DATA_SIZE_ADDR(base) ((base) + (0x000000B0))
#define SOC_NPU_AICORE_L2_REMAP_CFG0_ADDR(base) ((base) + (0x000000B8))
#define SOC_NPU_AICORE_L2_REMAP_CFG1_ADDR(base) ((base) + (0x000000C0))
#define SOC_NPU_AICORE_L2_REMAP_CFG2_ADDR(base) ((base) + (0x000000C8))
#define SOC_NPU_AICORE_L2_REMAP_CFG3_ADDR(base) ((base) + (0x000000D0))
#define SOC_NPU_AICORE_L2_REMAP_CFG4_ADDR(base) ((base) + (0x000000D8))
#define SOC_NPU_AICORE_L2_REMAP_CFG5_ADDR(base) ((base) + (0x000000E0))
#define SOC_NPU_AICORE_L2_REMAP_CFG6_ADDR(base) ((base) + (0x000000E8))
#define SOC_NPU_AICORE_L2_REMAP_CFG7_ADDR(base) ((base) + (0x000000F0))
#define SOC_NPU_AICORE_L2_VADDR_BASE_ADDR(base) ((base) + (0x000000F8))
#define SOC_NPU_AICORE_DBG_RW_CTRL_ADDR(base) ((base) + (0x000000100))
#define SOC_NPU_AICORE_DBG_ADDR_ADDR(base) ((base) + (0x000000108))
#define SOC_NPU_AICORE_DBG_DATA0_ADDR(base) ((base) + (0x000000110))
#define SOC_NPU_AICORE_DBG_DATA1_ADDR(base) ((base) + (0x000000118))
#define SOC_NPU_AICORE_DBG_DATA2_ADDR(base) ((base) + (0x000000120))
#define SOC_NPU_AICORE_DBG_DATA3_ADDR(base) ((base) + (0x000000128))
#define SOC_NPU_AICORE_DFX_DATA_ADDR(base) ((base) + (0x000000130))
#define SOC_NPU_AICORE_DBG_STATUS_ADDR(base) ((base) + (0x000000138))
#define SOC_NPU_AICORE_RESERVED_REG00_ADDR(base) ((base) + (0x000000140))
#define SOC_NPU_AICORE_RESERVED_REG01_ADDR(base) ((base) + (0x000000148))
#define SOC_NPU_AICORE_DJTAG_CLK_BYPASS_ADDR(base) ((base) + (0x00000150))
#define SOC_NPU_AICORE_PMU_CTRL_ADDR(base) ((base) + (0x000000200))
#define SOC_NPU_AICORE_PMU_MIN_OV_CNT_ADDR(base) ((base) + (0x000000208))
#define SOC_NPU_AICORE_PMU_CNT0_ADDR(base) ((base) + (0x000000210))
#define SOC_NPU_AICORE_PMU_CNT1_ADDR(base) ((base) + (0x000000218))
#define SOC_NPU_AICORE_PMU_CNT2_ADDR(base) ((base) + (0x000000220))
#define SOC_NPU_AICORE_PMU_CNT3_ADDR(base) ((base) + (0x000000228))
#define SOC_NPU_AICORE_PMU_CNT4_ADDR(base) ((base) + (0x000000230))
#define SOC_NPU_AICORE_PMU_CNT5_ADDR(base) ((base) + (0x000000238))
#define SOC_NPU_AICORE_PMU_CNT6_ADDR(base) ((base) + (0x000000240))
#define SOC_NPU_AICORE_PMU_CNT7_ADDR(base) ((base) + (0x000000248))
#define SOC_NPU_AICORE_PMU_TASK_CYC_CNT_ADDR(base) ((base) + (0x000000250))
#define SOC_NPU_AICORE_PMU_CNT0_IDX_ADDR(base) ((base) + (0x000000260))
#define SOC_NPU_AICORE_PMU_CNT1_IDX_ADDR(base) ((base) + (0x000000268))
#define SOC_NPU_AICORE_PMU_CNT2_IDX_ADDR(base) ((base) + (0x000000270))
#define SOC_NPU_AICORE_PMU_CNT3_IDX_ADDR(base) ((base) + (0x000000278))
#define SOC_NPU_AICORE_PMU_CNT4_IDX_ADDR(base) ((base) + (0x000000280))
#define SOC_NPU_AICORE_PMU_CNT5_IDX_ADDR(base) ((base) + (0x000000288))
#define SOC_NPU_AICORE_PMU_CNT6_IDX_ADDR(base) ((base) + (0x000000290))
#define SOC_NPU_AICORE_PMU_CNT7_IDX_ADDR(base) ((base) + (0x000000298))
#define SOC_NPU_AICORE_PMU_START_CNT_CYC_ADDR(base) ((base) + (0x0000002A0))
#define SOC_NPU_AICORE_PMU_STOP_CNT_CYC_ADDR(base) ((base) + (0x0000002A8))
#define SOC_NPU_AICORE_IFU_CTRL_ADDR(base) ((base) + (0x000000300))
#define SOC_NPU_AICORE_IC_INV_CTRL_ADDR(base) ((base) + (0x000000308))
#define SOC_NPU_AICORE_IC_INV_VA_ADDR(base) ((base) + (0x000000310))
#define SOC_NPU_AICORE_IC_INV_STATUS_ADDR(base) ((base) + (0x000000318))
#define SOC_NPU_AICORE_HW_BKPT_EN_ADDR(base) ((base) + (0x00000400))
#define SOC_NPU_AICORE_HW_BKPT0_PC_ADDR(base) ((base) + (0x00000408))
#define SOC_NPU_AICORE_HW_BKPT1_PC_ADDR(base) ((base) + (0x00000410))
#define SOC_NPU_AICORE_HW_BKPT2_PC_ADDR(base) ((base) + (0x00000418))
#define SOC_NPU_AICORE_HW_BKPT3_PC_ADDR(base) ((base) + (0x00000420))
#define SOC_NPU_AICORE_HW_BKPT4_PC_ADDR(base) ((base) + (0x00000428))
#define SOC_NPU_AICORE_HW_BKPT5_PC_ADDR(base) ((base) + (0x00000430))
#define SOC_NPU_AICORE_HW_BKPT6_PC_ADDR(base) ((base) + (0x00000438))
#define SOC_NPU_AICORE_HW_BKPT7_PC_ADDR(base) ((base) + (0x00000440))
#define SOC_NPU_AICORE_HW_BKPT8_PC_ADDR(base) ((base) + (0x00000448))
#define SOC_NPU_AICORE_HW_BKPT9_PC_ADDR(base) ((base) + (0x00000450))
#define SOC_NPU_AICORE_HW_BKPT10_PC_ADDR(base) ((base) + (0x00000458))
#define SOC_NPU_AICORE_HW_BKPT11_PC_ADDR(base) ((base) + (0x00000460))
#define SOC_NPU_AICORE_HW_BKPT12_PC_ADDR(base) ((base) + (0x00000468))
#define SOC_NPU_AICORE_HW_BKPT13_PC_ADDR(base) ((base) + (0x00000470))
#define SOC_NPU_AICORE_HW_BKPT14_PC_ADDR(base) ((base) + (0x00000478))
#define SOC_NPU_AICORE_HW_BKPT15_PC_ADDR(base) ((base) + (0x00000480))
#define SOC_NPU_AICORE_CCU_IQ_TIMEOUT_ADDR(base) ((base) + (0x00000488))
#define SOC_NPU_AICORE_CCU_CTRL_ADDR(base) ((base) + (0x00000490))
#define SOC_NPU_AICORE_SPR_STATUS_ADDR(base) ((base) + (0x000004C0))
#define SOC_NPU_AICORE_COND_SPR_ADDR(base) ((base) + (0x000004C8))
#define SOC_NPU_AICORE_CCU_REDIRECT_IFU_STATUS_ADDR(base) ((base) + (0x000004D0))
#define SOC_NPU_AICORE_BIU_CTRL0_ADDR(base) ((base) + (0x00000500))
#define SOC_NPU_AICORE_BIU_CTRL1_ADDR(base) ((base) + (0x00000508))
#define SOC_NPU_AICORE_BIU_CTRL2_ADDR(base) ((base) + (0x00000510))
#define SOC_NPU_AICORE_BIU_STATUS0_ADDR(base) ((base) + (0x00000518))
#define SOC_NPU_AICORE_BIU_STATUS1_ADDR(base) ((base) + (0x00000520))
#define SOC_NPU_AICORE_BIU_STATUS2_ADDR(base) ((base) + (0x00000528))
#define SOC_NPU_AICORE_BIU_STATUS3_ADDR(base) ((base) + (0x00000530))
#define SOC_NPU_AICORE_BIU_STATUS4_ADDR(base) ((base) + (0x00000538))
#define SOC_NPU_AICORE_BIU_STATUS5_ADDR(base) ((base) + (0x00000540))
#define SOC_NPU_AICORE_BIU_SMMU_STREAMID_ADDR(base) ((base) + (0x00000548))
#define SOC_NPU_AICORE_BIU_L2_REMAP_PADDR_BASE_ADDR(base) ((base) + (0x00000550))
#define SOC_NPU_AICORE_BIU_L2_SIZE_ADDR(base) ((base) + (0x00000558))
#define SOC_NPU_AICORE_BIU_L2_PAGE_SIZE_ADDR(base) ((base) + (0x00000560))
#define SOC_NPU_AICORE_BIU_CTRL6_ADDR(base) ((base) + (0x00000568))
#define SOC_NPU_AICORE_BIU_STATUS8_ADDR(base) ((base) + (0x00000570))
#define SOC_NPU_AICORE_BIU_STREAM_NS_ADDR(base) ((base) + (0x00000578))
#define SOC_NPU_AICORE_BIU_CTRL7_ADDR(base) ((base) + (0x00000580))
#define SOC_NPU_AICORE_BIU_CTRL8_ADDR(base) ((base) + (0x00000588))
#define SOC_NPU_AICORE_BIU_SMMU_CFG_ADDR(base) ((base) + (0x00000590))
#define SOC_NPU_AICORE_SMMU_SEC_STREAMID_ADDR(base) ((base) + (0x00000598))
#define SOC_NPU_AICORE_BIU_STATUS9_ADDR(base) ((base) + (0x000005C0))
#define SOC_NPU_AICORE_CORE_ID_ADDR(base) ((base) + (0x00000600))
#define SOC_NPU_AICORE_AIC_VER_ADDR(base) ((base) + (0x00000608))
#define SOC_NPU_AICORE_SMMU_VER_ADDR(base) ((base) + (0x00000610))
#define SOC_NPU_AICORE_DISPATCH_VER_ADDR(base) ((base) + (0x00000618))
#define SOC_NPU_AICORE_AA_VER_ADDR(base) ((base) + (0x00000620))
#define SOC_NPU_AICORE_CRG_VER_ADDR(base) ((base) + (0x00000628))
#define SOC_NPU_AICORE_POWER_MODE_CTRL_ADDR(base) ((base) + (0x00000630))
#define SOC_NPU_AICORE_MEM_TMOD_CTRL_ADDR(base) ((base) + (0x00000638))
#define SOC_NPU_AICORE_AIC_ERROR_ADDR(base) ((base) + (0x00000700))
#define SOC_NPU_AICORE_AIC_ERROR_MASK_ADDR(base) ((base) + (0x00000708))
#define SOC_NPU_AICORE_BIU_ERR_INFO_ADDR(base) ((base) + (0x00000710))
#define SOC_NPU_AICORE_CCU_ERR_INFO_ADDR(base) ((base) + (0x00000718))
#define SOC_NPU_AICORE_CUBE_ERR_INFO_ADDR(base) ((base) + (0x00000720))
#define SOC_NPU_AICORE_IFU_ERR_INFO_ADDR(base) ((base) + (0x00000728))
#define SOC_NPU_AICORE_MTE_ERR_INFO_ADDR(base) ((base) + (0x00000730))
#define SOC_NPU_AICORE_VEC_ERR_INFO_ADDR(base) ((base) + (0x00000738))
#define SOC_NPU_AICORE_MTE_ECC_1BIT_ERR_ADDR(base) ((base) + (0x00000740))
#define SOC_NPU_AICORE_VEC_CUBE_ECC_1BIT_ERR_ADDR(base) ((base) + (0x00000748))
#define SOC_NPU_AICORE_AIC_ERROR_FORCE_ADDR(base) ((base) + (0x00000750))
#define SOC_NPU_AICORE_AIC_ERROR_FORCE2_ADDR(base) ((base) + (0x00000758))
#define SOC_NPU_AICORE_AIC_ERROR2_ADDR(base) ((base) + (0x00000760))
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_ADDR(base) ((base) + (0x00000768))
#define SOC_NPU_AICORE_CCU_BUS_ERR_INFO_ADDR(base) ((base) + (0x00000770))
#define SOC_NPU_AICORE_AIC_ERROR1_ADDR(base) ((base) + (0x00000780))
#define SOC_NPU_AICORE_AIC_ERROR_MASK1_ADDR(base) ((base) + (0x00000788))
#define SOC_NPU_AICORE_PCT_CTRL_ADDR(base) ((base) + (0x00000800))
#define SOC_NPU_AICORE_PCT_STATUS_ADDR(base) ((base) + (0x00000808))
#define SOC_NPU_AICORE_PCT_NUM_ENTRIES_ADDR(base) ((base) + (0x00000810))
#define SOC_NPU_AICORE_PCT_START_CNT_CYC_ADDR(base) ((base) + (0x00000820))
#define SOC_NPU_AICORE_PCT_STOP_CNT_CYC_ADDR(base) ((base) + (0x00000828))
#define SOC_NPU_AICORE_PCT_OV_TIMESTAMP_ADDR(base) ((base) + (0x00000830))
#define SOC_NPU_AICORE_VEC_CTRL_ADDR(base) ((base) + (0x00000900))
#define SOC_NPU_AICORE_VEC_RESERVED_REG00_ADDR(base) ((base) + (0x000000a00))
#define SOC_NPU_AICORE_VEC_RESERVED_REG01_ADDR(base) ((base) + (0x000000A08))
#define SOC_NPU_AICORE_CUBE_RESERVED_REG00_ADDR(base) ((base) + (0x000000A10))
#define SOC_NPU_AICORE_CUBE_RESERVED_REG01_ADDR(base) ((base) + (0x000000A18))
#define SOC_NPU_AICORE_SC_RESERVED_REG00_ADDR(base) ((base) + (0x000000A20))
#define SOC_NPU_AICORE_SC_RESERVED_REG01_ADDR(base) ((base) + (0x000000A28))
#define SOC_NPU_AICORE_SC_RESERVED_RO_REG00_ADDR(base) ((base) + (0x000000A30))
#define SOC_NPU_AICORE_SC_RESERVED_RO_REG01_ADDR(base) ((base) + (0x000000A38))
#define SOC_NPU_AICORE_SC_RESERVED_REG02_ADDR(base) ((base) + (0x000000A40))
#define SOC_NPU_AICORE_SC_RESERVED_REG03_ADDR(base) ((base) + (0x000000A48))
#define SOC_NPU_AICORE_VEC_RESERVED_RO_REG00_ADDR(base) ((base) + (0x000000A50))
#define SOC_NPU_AICORE_VEC_RESERVED_RO_REG01_ADDR(base) ((base) + (0x000000A58))
#define SOC_NPU_AICORE_CUBE_RESERVED_RO_REG00_ADDR(base) ((base) + (0x000000A60))
#define SOC_NPU_AICORE_CUBE_RESERVED_RO_REG01_ADDR(base) ((base) + (0x000000A68))
#define SOC_NPU_AICORE_MTE_CTRL_ADDR(base) ((base) + (0x00000b00))
#define SOC_NPU_AICORE_CUBE_CTRL_ADDR(base) ((base) + (0x00000c00))
#define SOC_NPU_AICORE_BIU8_STATUS2_ADDR(base) ((base) + (0x00000D28))
#define SOC_NPU_AICORE_BIU8_STATUS3_ADDR(base) ((base) + (0x00000D30))
#define SOC_NPU_AICORE_BIU8_STATUS4_ADDR(base) ((base) + (0x00000D38))
#define SOC_NPU_AICORE_BIU8_SMMU_STREAMID_ADDR(base) ((base) + (0x00000D48))
#define SOC_NPU_AICORE_BIU8_L2_REMAP_PADDR_BASE_ADDR(base) ((base) + (0x00000D50))
#define SOC_NPU_AICORE_BIU8_L2_PAGE_SIZE_ADDR(base) ((base) + (0x00000D58))
#define SOC_NPU_AICORE_BIU8_L2_SIZE_ADDR(base) ((base) + (0x00000D60))
#define SOC_NPU_AICORE_BIU8_CTRL3_ADDR(base) ((base) + (0x00000D80))
#define SOC_NPU_AICORE_BIU8_CTRL4_ADDR(base) ((base) + (0x00000D88))
#define SOC_NPU_AICORE_BIU8_CTRL5_ADDR(base) ((base) + (0x00000D90))
#define SOC_NPU_AICORE_BIU8_STATUS6_ADDR(base) ((base) + (0x00000D98))
#define SOC_NPU_AICORE_BIU8_STATUS7_ADDR(base) ((base) + (0x00000DA0))
#define SOC_NPU_AICORE_CCU_TIME_COMPARE_COUNTER_CTRL_ADDR(base) ((base) + (0x00000E00))
#define SOC_NPU_AICORE_CCU_TIME_COMPARE_COUNTER_INT_ADDR(base) ((base) + (0x00000E08))
#define SOC_NPU_AICORE_AIC_SECURE_EN_ADDR(base) ((base) + (0x00000F00))
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_CTRL_ADDR(base) ((base) + (0x00001000))
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_STATUS_ADDR(base) ((base) + (0x00001008))
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_WDATA0_ADDR(base) ((base) + (0x00001010))
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_WDATA1_ADDR(base) ((base) + (0x00001018))
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_WDATA2_ADDR(base) ((base) + (0x00001020))
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_WDATA3_ADDR(base) ((base) + (0x00001028))
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_WDATA4_ADDR(base) ((base) + (0x00001030))
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_RDATA0_ADDR(base) ((base) + (0x00001038))
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_RDATA1_ADDR(base) ((base) + (0x00001040))
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_RDATA2_ADDR(base) ((base) + (0x00001048))
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_RDATA3_ADDR(base) ((base) + (0x00001050))
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_RDATA4_ADDR(base) ((base) + (0x00001058))
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_CTRL_ADDR(base) ((base) + (0x00001060))
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_STATUS_ADDR(base) ((base) + (0x00001068))
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_WDATA0_ADDR(base) ((base) + (0x00001070))
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_WDATA1_ADDR(base) ((base) + (0x00001078))
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_WDATA2_ADDR(base) ((base) + (0x00001080))
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_WDATA3_ADDR(base) ((base) + (0x00001088))
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_WDATA4_ADDR(base) ((base) + (0x00001090))
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_RDATA0_ADDR(base) ((base) + (0x00001098))
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_RDATA1_ADDR(base) ((base) + (0x000010A0))
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_RDATA2_ADDR(base) ((base) + (0x000010A8))
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_RDATA3_ADDR(base) ((base) + (0x000010B0))
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_RDATA4_ADDR(base) ((base) + (0x000010B8))
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long psq_stop : 1;
        unsigned long long pc_reload : 1;
        unsigned long long reserved : 62;
    } reg;
} SOC_NPU_AICORE_RUN_STALL_UNION;
#endif
#define SOC_NPU_AICORE_RUN_STALL_psq_stop_START (0)
#define SOC_NPU_AICORE_RUN_STALL_psq_stop_END (0)
#define SOC_NPU_AICORE_RUN_STALL_pc_reload_START (1)
#define SOC_NPU_AICORE_RUN_STALL_pc_reload_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long single_step : 1;
        unsigned long long ccu_stall : 1;
        unsigned long long resume_fetch_one : 1;
        unsigned long long resume_all : 1;
        unsigned long long ifu_manual_resume : 1;
        unsigned long long err_resp_en : 1;
        unsigned long long ccu_redirect_ifu : 1;
        unsigned long long dbg_ctrl_reserved : 57;
    } reg;
} SOC_NPU_AICORE_DBG_CTRL_UNION;
#endif
#define SOC_NPU_AICORE_DBG_CTRL_single_step_START (0)
#define SOC_NPU_AICORE_DBG_CTRL_single_step_END (0)
#define SOC_NPU_AICORE_DBG_CTRL_ccu_stall_START (1)
#define SOC_NPU_AICORE_DBG_CTRL_ccu_stall_END (1)
#define SOC_NPU_AICORE_DBG_CTRL_resume_fetch_one_START (2)
#define SOC_NPU_AICORE_DBG_CTRL_resume_fetch_one_END (2)
#define SOC_NPU_AICORE_DBG_CTRL_resume_all_START (3)
#define SOC_NPU_AICORE_DBG_CTRL_resume_all_END (3)
#define SOC_NPU_AICORE_DBG_CTRL_ifu_manual_resume_START (4)
#define SOC_NPU_AICORE_DBG_CTRL_ifu_manual_resume_END (4)
#define SOC_NPU_AICORE_DBG_CTRL_err_resp_en_START (5)
#define SOC_NPU_AICORE_DBG_CTRL_err_resp_en_END (5)
#define SOC_NPU_AICORE_DBG_CTRL_ccu_redirect_ifu_START (6)
#define SOC_NPU_AICORE_DBG_CTRL_ccu_redirect_ifu_END (6)
#define SOC_NPU_AICORE_DBG_CTRL_dbg_ctrl_reserved_START (7)
#define SOC_NPU_AICORE_DBG_CTRL_dbg_ctrl_reserved_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long rst_all : 1;
        unsigned long long reserved_0 : 7;
        unsigned long long rst_done : 1;
        unsigned long long ub_init_done : 1;
        unsigned long long mte_init_done : 1;
        unsigned long long cube_init_done : 1;
        unsigned long long ccu_init_done : 1;
        unsigned long long reserved_1 : 51;
    } reg;
} SOC_NPU_AICORE_RST_CTRL_UNION;
#endif
#define SOC_NPU_AICORE_RST_CTRL_rst_all_START (0)
#define SOC_NPU_AICORE_RST_CTRL_rst_all_END (0)
#define SOC_NPU_AICORE_RST_CTRL_rst_done_START (8)
#define SOC_NPU_AICORE_RST_CTRL_rst_done_END (8)
#define SOC_NPU_AICORE_RST_CTRL_ub_init_done_START (9)
#define SOC_NPU_AICORE_RST_CTRL_ub_init_done_END (9)
#define SOC_NPU_AICORE_RST_CTRL_mte_init_done_START (10)
#define SOC_NPU_AICORE_RST_CTRL_mte_init_done_END (10)
#define SOC_NPU_AICORE_RST_CTRL_cube_init_done_START (11)
#define SOC_NPU_AICORE_RST_CTRL_cube_init_done_END (11)
#define SOC_NPU_AICORE_RST_CTRL_ccu_init_done_START (12)
#define SOC_NPU_AICORE_RST_CTRL_ccu_init_done_END (12)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long fast_path_en : 1;
        unsigned long long fast_path_icache_inv_en : 1;
        unsigned long long fast_path_pipe_rst_en : 1;
        unsigned long long reserved : 61;
    } reg;
} SOC_NPU_AICORE_FAST_PATH_CTRL_UNION;
#endif
#define SOC_NPU_AICORE_FAST_PATH_CTRL_fast_path_en_START (0)
#define SOC_NPU_AICORE_FAST_PATH_CTRL_fast_path_en_END (0)
#define SOC_NPU_AICORE_FAST_PATH_CTRL_fast_path_icache_inv_en_START (1)
#define SOC_NPU_AICORE_FAST_PATH_CTRL_fast_path_icache_inv_en_END (1)
#define SOC_NPU_AICORE_FAST_PATH_CTRL_fast_path_pipe_rst_en_START (2)
#define SOC_NPU_AICORE_FAST_PATH_CTRL_fast_path_pipe_rst_en_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long blk_norm_int : 1;
        unsigned long long sw_bkpt_int : 1;
        unsigned long long ccu_timeout_int : 1;
        unsigned long long single_step_int : 1;
        unsigned long long ccu_stall_int : 1;
        unsigned long long hw_bkpt_int : 1;
        unsigned long long warn_as_exception_int : 1;
        unsigned long long blk_warn_done_int : 1;
        unsigned long long trap_int : 1;
        unsigned long long reserved : 55;
    } reg;
} SOC_NPU_AICORE_AI_CORE_INT_UNION;
#endif
#define SOC_NPU_AICORE_AI_CORE_INT_blk_norm_int_START (0)
#define SOC_NPU_AICORE_AI_CORE_INT_blk_norm_int_END (0)
#define SOC_NPU_AICORE_AI_CORE_INT_sw_bkpt_int_START (1)
#define SOC_NPU_AICORE_AI_CORE_INT_sw_bkpt_int_END (1)
#define SOC_NPU_AICORE_AI_CORE_INT_ccu_timeout_int_START (2)
#define SOC_NPU_AICORE_AI_CORE_INT_ccu_timeout_int_END (2)
#define SOC_NPU_AICORE_AI_CORE_INT_single_step_int_START (3)
#define SOC_NPU_AICORE_AI_CORE_INT_single_step_int_END (3)
#define SOC_NPU_AICORE_AI_CORE_INT_ccu_stall_int_START (4)
#define SOC_NPU_AICORE_AI_CORE_INT_ccu_stall_int_END (4)
#define SOC_NPU_AICORE_AI_CORE_INT_hw_bkpt_int_START (5)
#define SOC_NPU_AICORE_AI_CORE_INT_hw_bkpt_int_END (5)
#define SOC_NPU_AICORE_AI_CORE_INT_warn_as_exception_int_START (6)
#define SOC_NPU_AICORE_AI_CORE_INT_warn_as_exception_int_END (6)
#define SOC_NPU_AICORE_AI_CORE_INT_blk_warn_done_int_START (7)
#define SOC_NPU_AICORE_AI_CORE_INT_blk_warn_done_int_END (7)
#define SOC_NPU_AICORE_AI_CORE_INT_trap_int_START (8)
#define SOC_NPU_AICORE_AI_CORE_INT_trap_int_END (8)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long blk_norm_int_mask : 1;
        unsigned long long sw_bkpt_int_mask : 1;
        unsigned long long ccu_timeout_int_mask : 1;
        unsigned long long single_step_int_mask : 1;
        unsigned long long ccu_stall_int_mask : 1;
        unsigned long long hw_bkpt_int_mask : 1;
        unsigned long long warn_as_exception_int_mask : 1;
        unsigned long long blk_warn_done_int_mask : 1;
        unsigned long long trap_int_mask : 1;
        unsigned long long reserved : 55;
    } reg;
} SOC_NPU_AICORE_AI_CORE_INT_MASK_UNION;
#endif
#define SOC_NPU_AICORE_AI_CORE_INT_MASK_blk_norm_int_mask_START (0)
#define SOC_NPU_AICORE_AI_CORE_INT_MASK_blk_norm_int_mask_END (0)
#define SOC_NPU_AICORE_AI_CORE_INT_MASK_sw_bkpt_int_mask_START (1)
#define SOC_NPU_AICORE_AI_CORE_INT_MASK_sw_bkpt_int_mask_END (1)
#define SOC_NPU_AICORE_AI_CORE_INT_MASK_ccu_timeout_int_mask_START (2)
#define SOC_NPU_AICORE_AI_CORE_INT_MASK_ccu_timeout_int_mask_END (2)
#define SOC_NPU_AICORE_AI_CORE_INT_MASK_single_step_int_mask_START (3)
#define SOC_NPU_AICORE_AI_CORE_INT_MASK_single_step_int_mask_END (3)
#define SOC_NPU_AICORE_AI_CORE_INT_MASK_ccu_stall_int_mask_START (4)
#define SOC_NPU_AICORE_AI_CORE_INT_MASK_ccu_stall_int_mask_END (4)
#define SOC_NPU_AICORE_AI_CORE_INT_MASK_hw_bkpt_int_mask_START (5)
#define SOC_NPU_AICORE_AI_CORE_INT_MASK_hw_bkpt_int_mask_END (5)
#define SOC_NPU_AICORE_AI_CORE_INT_MASK_warn_as_exception_int_mask_START (6)
#define SOC_NPU_AICORE_AI_CORE_INT_MASK_warn_as_exception_int_mask_END (6)
#define SOC_NPU_AICORE_AI_CORE_INT_MASK_blk_warn_done_int_mask_START (7)
#define SOC_NPU_AICORE_AI_CORE_INT_MASK_blk_warn_done_int_mask_END (7)
#define SOC_NPU_AICORE_AI_CORE_INT_MASK_trap_int_mask_START (8)
#define SOC_NPU_AICORE_AI_CORE_INT_MASK_trap_int_mask_END (8)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long rst_cnt : 8;
        unsigned long long rst_done_cnt : 8;
        unsigned long long reserved : 48;
    } reg;
} SOC_NPU_AICORE_RST_CNT_UNION;
#endif
#define SOC_NPU_AICORE_RST_CNT_rst_cnt_START (0)
#define SOC_NPU_AICORE_RST_CNT_rst_cnt_END (7)
#define SOC_NPU_AICORE_RST_CNT_rst_done_cnt_START (8)
#define SOC_NPU_AICORE_RST_CNT_rst_done_cnt_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long l1_ecc_en_n : 1;
        unsigned long long l0a_ecc_en_n : 1;
        unsigned long long l0b_ecc_en_n : 1;
        unsigned long long l0c_ecc_en_n : 1;
        unsigned long long ub_ecc_en_n : 1;
        unsigned long long tlu_ecc_en_n : 1;
        unsigned long long reserved_0 : 2;
        unsigned long long l1_ecc_wb_en_n : 1;
        unsigned long long l0a_ecc_wb_en_n : 1;
        unsigned long long l0b_ecc_wb_en_n : 1;
        unsigned long long l0c_ecc_wb_en_n : 1;
        unsigned long long ub_ecc_wb_en_n : 1;
        unsigned long long tlu_ecc_wb_en_n : 1;
        unsigned long long ccu_ecc_en_n : 1;
        unsigned long long ccu_ecc_wb_en_n : 1;
        unsigned long long reserved_1 : 48;
    } reg;
} SOC_NPU_AICORE_ECC_EN_UNION;
#endif
#define SOC_NPU_AICORE_ECC_EN_l1_ecc_en_n_START (0)
#define SOC_NPU_AICORE_ECC_EN_l1_ecc_en_n_END (0)
#define SOC_NPU_AICORE_ECC_EN_l0a_ecc_en_n_START (1)
#define SOC_NPU_AICORE_ECC_EN_l0a_ecc_en_n_END (1)
#define SOC_NPU_AICORE_ECC_EN_l0b_ecc_en_n_START (2)
#define SOC_NPU_AICORE_ECC_EN_l0b_ecc_en_n_END (2)
#define SOC_NPU_AICORE_ECC_EN_l0c_ecc_en_n_START (3)
#define SOC_NPU_AICORE_ECC_EN_l0c_ecc_en_n_END (3)
#define SOC_NPU_AICORE_ECC_EN_ub_ecc_en_n_START (4)
#define SOC_NPU_AICORE_ECC_EN_ub_ecc_en_n_END (4)
#define SOC_NPU_AICORE_ECC_EN_tlu_ecc_en_n_START (5)
#define SOC_NPU_AICORE_ECC_EN_tlu_ecc_en_n_END (5)
#define SOC_NPU_AICORE_ECC_EN_l1_ecc_wb_en_n_START (8)
#define SOC_NPU_AICORE_ECC_EN_l1_ecc_wb_en_n_END (8)
#define SOC_NPU_AICORE_ECC_EN_l0a_ecc_wb_en_n_START (9)
#define SOC_NPU_AICORE_ECC_EN_l0a_ecc_wb_en_n_END (9)
#define SOC_NPU_AICORE_ECC_EN_l0b_ecc_wb_en_n_START (10)
#define SOC_NPU_AICORE_ECC_EN_l0b_ecc_wb_en_n_END (10)
#define SOC_NPU_AICORE_ECC_EN_l0c_ecc_wb_en_n_START (11)
#define SOC_NPU_AICORE_ECC_EN_l0c_ecc_wb_en_n_END (11)
#define SOC_NPU_AICORE_ECC_EN_ub_ecc_wb_en_n_START (12)
#define SOC_NPU_AICORE_ECC_EN_ub_ecc_wb_en_n_END (12)
#define SOC_NPU_AICORE_ECC_EN_tlu_ecc_wb_en_n_START (13)
#define SOC_NPU_AICORE_ECC_EN_tlu_ecc_wb_en_n_END (13)
#define SOC_NPU_AICORE_ECC_EN_ccu_ecc_en_n_START (14)
#define SOC_NPU_AICORE_ECC_EN_ccu_ecc_en_n_END (14)
#define SOC_NPU_AICORE_ECC_EN_ccu_ecc_wb_en_n_START (15)
#define SOC_NPU_AICORE_ECC_EN_ccu_ecc_wb_en_n_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long bus_axi_rst_ack : 1;
        unsigned long long cfg_force_req_ack : 1;
        unsigned long long bus_axi_rst_req : 1;
        unsigned long long bus_axi_rst_ack_1 : 1;
        unsigned long long bus_axi_rst_bypass : 1;
        unsigned long long reserved : 59;
    } reg;
} SOC_NPU_AICORE_AXI_CLAMP_CTRL_UNION;
#endif
#define SOC_NPU_AICORE_AXI_CLAMP_CTRL_bus_axi_rst_ack_START (0)
#define SOC_NPU_AICORE_AXI_CLAMP_CTRL_bus_axi_rst_ack_END (0)
#define SOC_NPU_AICORE_AXI_CLAMP_CTRL_cfg_force_req_ack_START (1)
#define SOC_NPU_AICORE_AXI_CLAMP_CTRL_cfg_force_req_ack_END (1)
#define SOC_NPU_AICORE_AXI_CLAMP_CTRL_bus_axi_rst_req_START (2)
#define SOC_NPU_AICORE_AXI_CLAMP_CTRL_bus_axi_rst_req_END (2)
#define SOC_NPU_AICORE_AXI_CLAMP_CTRL_bus_axi_rst_ack_1_START (3)
#define SOC_NPU_AICORE_AXI_CLAMP_CTRL_bus_axi_rst_ack_1_END (3)
#define SOC_NPU_AICORE_AXI_CLAMP_CTRL_bus_axi_rst_bypass_START (4)
#define SOC_NPU_AICORE_AXI_CLAMP_CTRL_bus_axi_rst_bypass_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long axi_softrst_state0 : 32;
        unsigned long long axi_softrst_state1 : 32;
    } reg;
} SOC_NPU_AICORE_AXI_CLAMP_STATE_UNION;
#endif
#define SOC_NPU_AICORE_AXI_CLAMP_STATE_axi_softrst_state0_START (0)
#define SOC_NPU_AICORE_AXI_CLAMP_STATE_axi_softrst_state0_END (31)
#define SOC_NPU_AICORE_AXI_CLAMP_STATE_axi_softrst_state1_START (32)
#define SOC_NPU_AICORE_AXI_CLAMP_STATE_axi_softrst_state1_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long clk_ccu_cube_en_mask : 1;
        unsigned long long clk_ccu_ex_en_mask : 1;
        unsigned long long clk_cube_l0a_en_mask : 1;
        unsigned long long clk_cube_l0b_en_mask : 1;
        unsigned long long clk_cube_l0c_en_mask : 1;
        unsigned long long clk_ccu_mte_en_mask : 1;
        unsigned long long clk_ccu_vec_en_mask : 1;
        unsigned long long clk_cube_pe_en_mask : 1;
        unsigned long long clk_ib_en_mask : 1;
        unsigned long long clk_ifu_en_mask : 1;
        unsigned long long clk_ccu_ub_en_mask : 1;
        unsigned long long clk_mte_sub_en_mask : 1;
        unsigned long long reserved_0 : 1;
        unsigned long long clk_vec_valu_cmp_conv_en_mask : 1;
        unsigned long long clk_vec_valu_grp_en_mask : 1;
        unsigned long long clk_vec_valu_imux_en_mask : 1;
        unsigned long long clk_vec_valu_lut_en_mask : 1;
        unsigned long long clk_vec_valu_mau_en_mask : 1;
        unsigned long long clk_vec_valu_rpn_en_mask : 1;
        unsigned long long clk_mte_aipp_scaler_en_mask : 2;
        unsigned long long reg_clk_sel : 1;
        unsigned long long clk_vec_valu_div_en_mask : 1;
        unsigned long long clk_mte_smask_en_mask : 1;
        unsigned long long clk_cube_fifo_en_mask : 1;
        unsigned long long reserved_1 : 37;
        unsigned long long debug_mode_en : 1;
        unsigned long long clk_global_en_mask : 1;
    } reg;
} SOC_NPU_AICORE_CLK_GATE_MASK_UNION;
#endif
#define SOC_NPU_AICORE_CLK_GATE_MASK_clk_ccu_cube_en_mask_START (0)
#define SOC_NPU_AICORE_CLK_GATE_MASK_clk_ccu_cube_en_mask_END (0)
#define SOC_NPU_AICORE_CLK_GATE_MASK_clk_ccu_ex_en_mask_START (1)
#define SOC_NPU_AICORE_CLK_GATE_MASK_clk_ccu_ex_en_mask_END (1)
#define SOC_NPU_AICORE_CLK_GATE_MASK_clk_cube_l0a_en_mask_START (2)
#define SOC_NPU_AICORE_CLK_GATE_MASK_clk_cube_l0a_en_mask_END (2)
#define SOC_NPU_AICORE_CLK_GATE_MASK_clk_cube_l0b_en_mask_START (3)
#define SOC_NPU_AICORE_CLK_GATE_MASK_clk_cube_l0b_en_mask_END (3)
#define SOC_NPU_AICORE_CLK_GATE_MASK_clk_cube_l0c_en_mask_START (4)
#define SOC_NPU_AICORE_CLK_GATE_MASK_clk_cube_l0c_en_mask_END (4)
#define SOC_NPU_AICORE_CLK_GATE_MASK_clk_ccu_mte_en_mask_START (5)
#define SOC_NPU_AICORE_CLK_GATE_MASK_clk_ccu_mte_en_mask_END (5)
#define SOC_NPU_AICORE_CLK_GATE_MASK_clk_ccu_vec_en_mask_START (6)
#define SOC_NPU_AICORE_CLK_GATE_MASK_clk_ccu_vec_en_mask_END (6)
#define SOC_NPU_AICORE_CLK_GATE_MASK_clk_cube_pe_en_mask_START (7)
#define SOC_NPU_AICORE_CLK_GATE_MASK_clk_cube_pe_en_mask_END (7)
#define SOC_NPU_AICORE_CLK_GATE_MASK_clk_ib_en_mask_START (8)
#define SOC_NPU_AICORE_CLK_GATE_MASK_clk_ib_en_mask_END (8)
#define SOC_NPU_AICORE_CLK_GATE_MASK_clk_ifu_en_mask_START (9)
#define SOC_NPU_AICORE_CLK_GATE_MASK_clk_ifu_en_mask_END (9)
#define SOC_NPU_AICORE_CLK_GATE_MASK_clk_ccu_ub_en_mask_START (10)
#define SOC_NPU_AICORE_CLK_GATE_MASK_clk_ccu_ub_en_mask_END (10)
#define SOC_NPU_AICORE_CLK_GATE_MASK_clk_mte_sub_en_mask_START (11)
#define SOC_NPU_AICORE_CLK_GATE_MASK_clk_mte_sub_en_mask_END (11)
#define SOC_NPU_AICORE_CLK_GATE_MASK_clk_vec_valu_cmp_conv_en_mask_START (13)
#define SOC_NPU_AICORE_CLK_GATE_MASK_clk_vec_valu_cmp_conv_en_mask_END (13)
#define SOC_NPU_AICORE_CLK_GATE_MASK_clk_vec_valu_grp_en_mask_START (14)
#define SOC_NPU_AICORE_CLK_GATE_MASK_clk_vec_valu_grp_en_mask_END (14)
#define SOC_NPU_AICORE_CLK_GATE_MASK_clk_vec_valu_imux_en_mask_START (15)
#define SOC_NPU_AICORE_CLK_GATE_MASK_clk_vec_valu_imux_en_mask_END (15)
#define SOC_NPU_AICORE_CLK_GATE_MASK_clk_vec_valu_lut_en_mask_START (16)
#define SOC_NPU_AICORE_CLK_GATE_MASK_clk_vec_valu_lut_en_mask_END (16)
#define SOC_NPU_AICORE_CLK_GATE_MASK_clk_vec_valu_mau_en_mask_START (17)
#define SOC_NPU_AICORE_CLK_GATE_MASK_clk_vec_valu_mau_en_mask_END (17)
#define SOC_NPU_AICORE_CLK_GATE_MASK_clk_vec_valu_rpn_en_mask_START (18)
#define SOC_NPU_AICORE_CLK_GATE_MASK_clk_vec_valu_rpn_en_mask_END (18)
#define SOC_NPU_AICORE_CLK_GATE_MASK_clk_mte_aipp_scaler_en_mask_START (19)
#define SOC_NPU_AICORE_CLK_GATE_MASK_clk_mte_aipp_scaler_en_mask_END (20)
#define SOC_NPU_AICORE_CLK_GATE_MASK_reg_clk_sel_START (21)
#define SOC_NPU_AICORE_CLK_GATE_MASK_reg_clk_sel_END (21)
#define SOC_NPU_AICORE_CLK_GATE_MASK_clk_vec_valu_div_en_mask_START (22)
#define SOC_NPU_AICORE_CLK_GATE_MASK_clk_vec_valu_div_en_mask_END (22)
#define SOC_NPU_AICORE_CLK_GATE_MASK_clk_mte_smask_en_mask_START (23)
#define SOC_NPU_AICORE_CLK_GATE_MASK_clk_mte_smask_en_mask_END (23)
#define SOC_NPU_AICORE_CLK_GATE_MASK_clk_cube_fifo_en_mask_START (24)
#define SOC_NPU_AICORE_CLK_GATE_MASK_clk_cube_fifo_en_mask_END (24)
#define SOC_NPU_AICORE_CLK_GATE_MASK_debug_mode_en_START (62)
#define SOC_NPU_AICORE_CLK_GATE_MASK_debug_mode_en_END (62)
#define SOC_NPU_AICORE_CLK_GATE_MASK_clk_global_en_mask_START (63)
#define SOC_NPU_AICORE_CLK_GATE_MASK_clk_global_en_mask_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long clk_ccu_vec_delay_cnt : 5;
        unsigned long long reserved_0 : 3;
        unsigned long long clk_ccu_mte_delay_cnt : 5;
        unsigned long long reserved_1 : 3;
        unsigned long long clk_ccu_cube_delay_cnt : 5;
        unsigned long long reserved_2 : 3;
        unsigned long long clk_ccu_ub_delay_cnt : 5;
        unsigned long long reserved_3 : 35;
    } reg;
} SOC_NPU_AICORE_CLK_DELAY_CNT_UNION;
#endif
#define SOC_NPU_AICORE_CLK_DELAY_CNT_clk_ccu_vec_delay_cnt_START (0)
#define SOC_NPU_AICORE_CLK_DELAY_CNT_clk_ccu_vec_delay_cnt_END (4)
#define SOC_NPU_AICORE_CLK_DELAY_CNT_clk_ccu_mte_delay_cnt_START (8)
#define SOC_NPU_AICORE_CLK_DELAY_CNT_clk_ccu_mte_delay_cnt_END (12)
#define SOC_NPU_AICORE_CLK_DELAY_CNT_clk_ccu_cube_delay_cnt_START (16)
#define SOC_NPU_AICORE_CLK_DELAY_CNT_clk_ccu_cube_delay_cnt_END (20)
#define SOC_NPU_AICORE_CLK_DELAY_CNT_clk_ccu_ub_delay_cnt_START (24)
#define SOC_NPU_AICORE_CLK_DELAY_CNT_clk_ccu_ub_delay_cnt_END (28)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long mbist2ub16mem_all_sel : 6;
        unsigned long long reserved_0 : 2;
        unsigned long long icg_en_smmu_trans : 1;
        unsigned long long icg_en_mbist : 1;
        unsigned long long reserved_1 : 54;
    } reg;
} SOC_NPU_AICORE_TOP_ICG_MBIST_UNION;
#endif
#define SOC_NPU_AICORE_TOP_ICG_MBIST_mbist2ub16mem_all_sel_START (0)
#define SOC_NPU_AICORE_TOP_ICG_MBIST_mbist2ub16mem_all_sel_END (5)
#define SOC_NPU_AICORE_TOP_ICG_MBIST_icg_en_smmu_trans_START (8)
#define SOC_NPU_AICORE_TOP_ICG_MBIST_icg_en_smmu_trans_END (8)
#define SOC_NPU_AICORE_TOP_ICG_MBIST_icg_en_mbist_START (9)
#define SOC_NPU_AICORE_TOP_ICG_MBIST_icg_en_mbist_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long axi_softrst_state0_1 : 32;
        unsigned long long axi_softrst_state1_1 : 32;
    } reg;
} SOC_NPU_AICORE_AXI_CLAMP_STATE_1_UNION;
#endif
#define SOC_NPU_AICORE_AXI_CLAMP_STATE_1_axi_softrst_state0_1_START (0)
#define SOC_NPU_AICORE_AXI_CLAMP_STATE_1_axi_softrst_state0_1_END (31)
#define SOC_NPU_AICORE_AXI_CLAMP_STATE_1_axi_softrst_state1_1_START (32)
#define SOC_NPU_AICORE_AXI_CLAMP_STATE_1_axi_softrst_state1_1_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long reserved_0 : 4;
        unsigned long long lock_bypass0_lpid : 3;
        unsigned long long lock_bypass0_srcid : 9;
        unsigned long long lock_bypass0_mask_n : 12;
        unsigned long long reserved_1 : 3;
        unsigned long long lock_bypass0_en : 1;
        unsigned long long reserved_2 : 4;
        unsigned long long lock_bypass1_lpid : 3;
        unsigned long long lock_bypass1_srcid : 9;
        unsigned long long lock_bypass1_mask_n : 12;
        unsigned long long reserved_3 : 3;
        unsigned long long lock_bypass1_en : 1;
    } reg;
} SOC_NPU_AICORE_LOCK_BYPASS_UNION;
#endif
#define SOC_NPU_AICORE_LOCK_BYPASS_lock_bypass0_lpid_START (4)
#define SOC_NPU_AICORE_LOCK_BYPASS_lock_bypass0_lpid_END (6)
#define SOC_NPU_AICORE_LOCK_BYPASS_lock_bypass0_srcid_START (7)
#define SOC_NPU_AICORE_LOCK_BYPASS_lock_bypass0_srcid_END (15)
#define SOC_NPU_AICORE_LOCK_BYPASS_lock_bypass0_mask_n_START (16)
#define SOC_NPU_AICORE_LOCK_BYPASS_lock_bypass0_mask_n_END (27)
#define SOC_NPU_AICORE_LOCK_BYPASS_lock_bypass0_en_START (31)
#define SOC_NPU_AICORE_LOCK_BYPASS_lock_bypass0_en_END (31)
#define SOC_NPU_AICORE_LOCK_BYPASS_lock_bypass1_lpid_START (36)
#define SOC_NPU_AICORE_LOCK_BYPASS_lock_bypass1_lpid_END (38)
#define SOC_NPU_AICORE_LOCK_BYPASS_lock_bypass1_srcid_START (39)
#define SOC_NPU_AICORE_LOCK_BYPASS_lock_bypass1_srcid_END (47)
#define SOC_NPU_AICORE_LOCK_BYPASS_lock_bypass1_mask_n_START (48)
#define SOC_NPU_AICORE_LOCK_BYPASS_lock_bypass1_mask_n_END (59)
#define SOC_NPU_AICORE_LOCK_BYPASS_lock_bypass1_en_START (63)
#define SOC_NPU_AICORE_LOCK_BYPASS_lock_bypass1_en_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long sysctrl_lock : 32;
        unsigned long long reserved : 32;
    } reg;
} SOC_NPU_AICORE_SYSCTRL_LOCK_UNION;
#endif
#define SOC_NPU_AICORE_SYSCTRL_LOCK_sysctrl_lock_START (0)
#define SOC_NPU_AICORE_SYSCTRL_LOCK_sysctrl_lock_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long reserved_0 : 2;
        unsigned long long va : 46;
        unsigned long long reserved_1 : 11;
        unsigned long long ifu_preload_cnt : 5;
    } reg;
} SOC_NPU_AICORE_PC_START_UNION;
#endif
#define SOC_NPU_AICORE_PC_START_va_START (2)
#define SOC_NPU_AICORE_PC_START_va_END (47)
#define SOC_NPU_AICORE_PC_START_ifu_preload_cnt_START (59)
#define SOC_NPU_AICORE_PC_START_ifu_preload_cnt_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long para_base : 64;
    } reg;
} SOC_NPU_AICORE_PARA_BASE_UNION;
#endif
#define SOC_NPU_AICORE_PARA_BASE_para_base_START (0)
#define SOC_NPU_AICORE_PARA_BASE_para_base_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long smmu_substreamid : 16;
        unsigned long long two_level_smmu_streamid : 16;
        unsigned long long two_level_en : 1;
        unsigned long long reserved : 31;
    } reg;
} SOC_NPU_AICORE_SMMU_SUBSTREAMID_UNION;
#endif
#define SOC_NPU_AICORE_SMMU_SUBSTREAMID_smmu_substreamid_START (0)
#define SOC_NPU_AICORE_SMMU_SUBSTREAMID_smmu_substreamid_END (15)
#define SOC_NPU_AICORE_SMMU_SUBSTREAMID_two_level_smmu_streamid_START (16)
#define SOC_NPU_AICORE_SMMU_SUBSTREAMID_two_level_smmu_streamid_END (31)
#define SOC_NPU_AICORE_SMMU_SUBSTREAMID_two_level_en_START (32)
#define SOC_NPU_AICORE_SMMU_SUBSTREAMID_two_level_en_END (32)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long block_id : 16;
        unsigned long long task_id : 6;
        unsigned long long reserved_0 : 10;
        unsigned long long block_dim : 16;
        unsigned long long l2_in_main : 8;
        unsigned long long reserved_1 : 3;
        unsigned long long biu_privilege : 1;
        unsigned long long biu_non_secure : 1;
        unsigned long long biu_instr : 1;
        unsigned long long icache_inv_before_startup : 1;
        unsigned long long pipe_rst_before_startup : 1;
    } reg;
} SOC_NPU_AICORE_TASK_CFG_UNION;
#endif
#define SOC_NPU_AICORE_TASK_CFG_block_id_START (0)
#define SOC_NPU_AICORE_TASK_CFG_block_id_END (15)
#define SOC_NPU_AICORE_TASK_CFG_task_id_START (16)
#define SOC_NPU_AICORE_TASK_CFG_task_id_END (21)
#define SOC_NPU_AICORE_TASK_CFG_block_dim_START (32)
#define SOC_NPU_AICORE_TASK_CFG_block_dim_END (47)
#define SOC_NPU_AICORE_TASK_CFG_l2_in_main_START (48)
#define SOC_NPU_AICORE_TASK_CFG_l2_in_main_END (55)
#define SOC_NPU_AICORE_TASK_CFG_biu_privilege_START (59)
#define SOC_NPU_AICORE_TASK_CFG_biu_privilege_END (59)
#define SOC_NPU_AICORE_TASK_CFG_biu_non_secure_START (60)
#define SOC_NPU_AICORE_TASK_CFG_biu_non_secure_END (60)
#define SOC_NPU_AICORE_TASK_CFG_biu_instr_START (61)
#define SOC_NPU_AICORE_TASK_CFG_biu_instr_END (61)
#define SOC_NPU_AICORE_TASK_CFG_icache_inv_before_startup_START (62)
#define SOC_NPU_AICORE_TASK_CFG_icache_inv_before_startup_END (62)
#define SOC_NPU_AICORE_TASK_CFG_pipe_rst_before_startup_START (63)
#define SOC_NPU_AICORE_TASK_CFG_pipe_rst_before_startup_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long data_main_base : 64;
    } reg;
} SOC_NPU_AICORE_DATA_MAIN_BASE_UNION;
#endif
#define SOC_NPU_AICORE_DATA_MAIN_BASE_data_main_base_START (0)
#define SOC_NPU_AICORE_DATA_MAIN_BASE_data_main_base_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long data_ub_base : 64;
    } reg;
} SOC_NPU_AICORE_DATA_UB_BASE_UNION;
#endif
#define SOC_NPU_AICORE_DATA_UB_BASE_data_ub_base_START (0)
#define SOC_NPU_AICORE_DATA_UB_BASE_data_ub_base_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long data_size : 32;
        unsigned long long reserved : 32;
    } reg;
} SOC_NPU_AICORE_DATA_SIZE_UNION;
#endif
#define SOC_NPU_AICORE_DATA_SIZE_data_size_START (0)
#define SOC_NPU_AICORE_DATA_SIZE_data_size_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long l2_remap_cfg0 : 64;
    } reg;
} SOC_NPU_AICORE_L2_REMAP_CFG0_UNION;
#endif
#define SOC_NPU_AICORE_L2_REMAP_CFG0_l2_remap_cfg0_START (0)
#define SOC_NPU_AICORE_L2_REMAP_CFG0_l2_remap_cfg0_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long l2_remap_cfg1 : 64;
    } reg;
} SOC_NPU_AICORE_L2_REMAP_CFG1_UNION;
#endif
#define SOC_NPU_AICORE_L2_REMAP_CFG1_l2_remap_cfg1_START (0)
#define SOC_NPU_AICORE_L2_REMAP_CFG1_l2_remap_cfg1_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long l2_remap_cfg2 : 64;
    } reg;
} SOC_NPU_AICORE_L2_REMAP_CFG2_UNION;
#endif
#define SOC_NPU_AICORE_L2_REMAP_CFG2_l2_remap_cfg2_START (0)
#define SOC_NPU_AICORE_L2_REMAP_CFG2_l2_remap_cfg2_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long l2_remap_cfg3 : 64;
    } reg;
} SOC_NPU_AICORE_L2_REMAP_CFG3_UNION;
#endif
#define SOC_NPU_AICORE_L2_REMAP_CFG3_l2_remap_cfg3_START (0)
#define SOC_NPU_AICORE_L2_REMAP_CFG3_l2_remap_cfg3_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long l2_remap_cfg4 : 64;
    } reg;
} SOC_NPU_AICORE_L2_REMAP_CFG4_UNION;
#endif
#define SOC_NPU_AICORE_L2_REMAP_CFG4_l2_remap_cfg4_START (0)
#define SOC_NPU_AICORE_L2_REMAP_CFG4_l2_remap_cfg4_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long l2_remap_cfg5 : 64;
    } reg;
} SOC_NPU_AICORE_L2_REMAP_CFG5_UNION;
#endif
#define SOC_NPU_AICORE_L2_REMAP_CFG5_l2_remap_cfg5_START (0)
#define SOC_NPU_AICORE_L2_REMAP_CFG5_l2_remap_cfg5_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long l2_remap_cfg6 : 64;
    } reg;
} SOC_NPU_AICORE_L2_REMAP_CFG6_UNION;
#endif
#define SOC_NPU_AICORE_L2_REMAP_CFG6_l2_remap_cfg6_START (0)
#define SOC_NPU_AICORE_L2_REMAP_CFG6_l2_remap_cfg6_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long l2_remap_cfg7 : 64;
    } reg;
} SOC_NPU_AICORE_L2_REMAP_CFG7_UNION;
#endif
#define SOC_NPU_AICORE_L2_REMAP_CFG7_l2_remap_cfg7_START (0)
#define SOC_NPU_AICORE_L2_REMAP_CFG7_l2_remap_cfg7_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long l2_vaddr_base : 64;
    } reg;
} SOC_NPU_AICORE_L2_VADDR_BASE_UNION;
#endif
#define SOC_NPU_AICORE_L2_VADDR_BASE_l2_vaddr_base_START (0)
#define SOC_NPU_AICORE_L2_VADDR_BASE_l2_vaddr_base_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long dbg_ctrl_wr : 1;
        unsigned long long dbg_ctrl_rd : 1;
        unsigned long long reserved_0 : 6;
        unsigned long long dbg_rw_done : 1;
        unsigned long long reserved_1 : 55;
    } reg;
} SOC_NPU_AICORE_DBG_RW_CTRL_UNION;
#endif
#define SOC_NPU_AICORE_DBG_RW_CTRL_dbg_ctrl_wr_START (0)
#define SOC_NPU_AICORE_DBG_RW_CTRL_dbg_ctrl_wr_END (0)
#define SOC_NPU_AICORE_DBG_RW_CTRL_dbg_ctrl_rd_START (1)
#define SOC_NPU_AICORE_DBG_RW_CTRL_dbg_ctrl_rd_END (1)
#define SOC_NPU_AICORE_DBG_RW_CTRL_dbg_rw_done_START (8)
#define SOC_NPU_AICORE_DBG_RW_CTRL_dbg_rw_done_END (8)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long dbg_addr : 48;
        unsigned long long dbg_sel : 15;
        unsigned long long dfx_en : 1;
    } reg;
} SOC_NPU_AICORE_DBG_ADDR_UNION;
#endif
#define SOC_NPU_AICORE_DBG_ADDR_dbg_addr_START (0)
#define SOC_NPU_AICORE_DBG_ADDR_dbg_addr_END (47)
#define SOC_NPU_AICORE_DBG_ADDR_dbg_sel_START (48)
#define SOC_NPU_AICORE_DBG_ADDR_dbg_sel_END (62)
#define SOC_NPU_AICORE_DBG_ADDR_dfx_en_START (63)
#define SOC_NPU_AICORE_DBG_ADDR_dfx_en_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long dbg_data0 : 64;
    } reg;
} SOC_NPU_AICORE_DBG_DATA0_UNION;
#endif
#define SOC_NPU_AICORE_DBG_DATA0_dbg_data0_START (0)
#define SOC_NPU_AICORE_DBG_DATA0_dbg_data0_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long dbg_data1 : 64;
    } reg;
} SOC_NPU_AICORE_DBG_DATA1_UNION;
#endif
#define SOC_NPU_AICORE_DBG_DATA1_dbg_data1_START (0)
#define SOC_NPU_AICORE_DBG_DATA1_dbg_data1_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long dbg_data2 : 64;
    } reg;
} SOC_NPU_AICORE_DBG_DATA2_UNION;
#endif
#define SOC_NPU_AICORE_DBG_DATA2_dbg_data2_START (0)
#define SOC_NPU_AICORE_DBG_DATA2_dbg_data2_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long dbg_data3 : 64;
    } reg;
} SOC_NPU_AICORE_DBG_DATA3_UNION;
#endif
#define SOC_NPU_AICORE_DBG_DATA3_dbg_data3_START (0)
#define SOC_NPU_AICORE_DBG_DATA3_dbg_data3_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long dfx_data : 8;
        unsigned long long reserved : 54;
        unsigned long long dbg_ifu_resume_all : 1;
        unsigned long long dbg_ifu_resume_fetch_one : 1;
    } reg;
} SOC_NPU_AICORE_DFX_DATA_UNION;
#endif
#define SOC_NPU_AICORE_DFX_DATA_dfx_data_START (0)
#define SOC_NPU_AICORE_DFX_DATA_dfx_data_END (7)
#define SOC_NPU_AICORE_DFX_DATA_dbg_ifu_resume_all_START (62)
#define SOC_NPU_AICORE_DFX_DATA_dbg_ifu_resume_all_END (62)
#define SOC_NPU_AICORE_DFX_DATA_dbg_ifu_resume_fetch_one_START (63)
#define SOC_NPU_AICORE_DFX_DATA_dbg_ifu_resume_fetch_one_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long dbg_cache_invalid : 1;
        unsigned long long dbg_write_fail : 1;
        unsigned long long dbg_read_err : 1;
        unsigned long long reserved : 61;
    } reg;
} SOC_NPU_AICORE_DBG_STATUS_UNION;
#endif
#define SOC_NPU_AICORE_DBG_STATUS_dbg_cache_invalid_START (0)
#define SOC_NPU_AICORE_DBG_STATUS_dbg_cache_invalid_END (0)
#define SOC_NPU_AICORE_DBG_STATUS_dbg_write_fail_START (1)
#define SOC_NPU_AICORE_DBG_STATUS_dbg_write_fail_END (1)
#define SOC_NPU_AICORE_DBG_STATUS_dbg_read_err_START (2)
#define SOC_NPU_AICORE_DBG_STATUS_dbg_read_err_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long mte2_norm_fuse_en_n : 1;
        unsigned long long mte2_cmplx_fuse_en_n : 1;
        unsigned long long mte3_all_fuse_en_n : 1;
        unsigned long long mte3_dm_erly_st_en_n : 1;
        unsigned long long mte3_fmc_erly_st_en_n : 1;
        unsigned long long mte2_fmd_erly_st_en_n : 1;
        unsigned long long mte2_uzp_erly_st_en_n : 1;
        unsigned long long scat_bp_en_n : 1;
        unsigned long long mte2_ub_erly_st_en_n : 1;
        unsigned long long mte2_l1_erly_st_en_n : 1;
        unsigned long long mte2_l0b_erly_st_en_n : 1;
        unsigned long long mte2_l0a_erly_st_en_n : 1;
        unsigned long long mte3_dm_fuse_en_n : 1;
        unsigned long long mte2_dm_fuse_en_n : 1;
        unsigned long long mte2_2d_fuse_en_n : 1;
        unsigned long long reserved : 49;
    } reg;
} SOC_NPU_AICORE_RESERVED_REG00_UNION;
#endif
#define SOC_NPU_AICORE_RESERVED_REG00_mte2_norm_fuse_en_n_START (0)
#define SOC_NPU_AICORE_RESERVED_REG00_mte2_norm_fuse_en_n_END (0)
#define SOC_NPU_AICORE_RESERVED_REG00_mte2_cmplx_fuse_en_n_START (1)
#define SOC_NPU_AICORE_RESERVED_REG00_mte2_cmplx_fuse_en_n_END (1)
#define SOC_NPU_AICORE_RESERVED_REG00_mte3_all_fuse_en_n_START (2)
#define SOC_NPU_AICORE_RESERVED_REG00_mte3_all_fuse_en_n_END (2)
#define SOC_NPU_AICORE_RESERVED_REG00_mte3_dm_erly_st_en_n_START (3)
#define SOC_NPU_AICORE_RESERVED_REG00_mte3_dm_erly_st_en_n_END (3)
#define SOC_NPU_AICORE_RESERVED_REG00_mte3_fmc_erly_st_en_n_START (4)
#define SOC_NPU_AICORE_RESERVED_REG00_mte3_fmc_erly_st_en_n_END (4)
#define SOC_NPU_AICORE_RESERVED_REG00_mte2_fmd_erly_st_en_n_START (5)
#define SOC_NPU_AICORE_RESERVED_REG00_mte2_fmd_erly_st_en_n_END (5)
#define SOC_NPU_AICORE_RESERVED_REG00_mte2_uzp_erly_st_en_n_START (6)
#define SOC_NPU_AICORE_RESERVED_REG00_mte2_uzp_erly_st_en_n_END (6)
#define SOC_NPU_AICORE_RESERVED_REG00_scat_bp_en_n_START (7)
#define SOC_NPU_AICORE_RESERVED_REG00_scat_bp_en_n_END (7)
#define SOC_NPU_AICORE_RESERVED_REG00_mte2_ub_erly_st_en_n_START (8)
#define SOC_NPU_AICORE_RESERVED_REG00_mte2_ub_erly_st_en_n_END (8)
#define SOC_NPU_AICORE_RESERVED_REG00_mte2_l1_erly_st_en_n_START (9)
#define SOC_NPU_AICORE_RESERVED_REG00_mte2_l1_erly_st_en_n_END (9)
#define SOC_NPU_AICORE_RESERVED_REG00_mte2_l0b_erly_st_en_n_START (10)
#define SOC_NPU_AICORE_RESERVED_REG00_mte2_l0b_erly_st_en_n_END (10)
#define SOC_NPU_AICORE_RESERVED_REG00_mte2_l0a_erly_st_en_n_START (11)
#define SOC_NPU_AICORE_RESERVED_REG00_mte2_l0a_erly_st_en_n_END (11)
#define SOC_NPU_AICORE_RESERVED_REG00_mte3_dm_fuse_en_n_START (12)
#define SOC_NPU_AICORE_RESERVED_REG00_mte3_dm_fuse_en_n_END (12)
#define SOC_NPU_AICORE_RESERVED_REG00_mte2_dm_fuse_en_n_START (13)
#define SOC_NPU_AICORE_RESERVED_REG00_mte2_dm_fuse_en_n_END (13)
#define SOC_NPU_AICORE_RESERVED_REG00_mte2_2d_fuse_en_n_START (14)
#define SOC_NPU_AICORE_RESERVED_REG00_mte2_2d_fuse_en_n_END (14)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long reserved : 64;
    } reg;
} SOC_NPU_AICORE_RESERVED_REG01_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long djtag_clk_bypass_en : 1;
        unsigned long long reserved : 63;
    } reg;
} SOC_NPU_AICORE_DJTAG_CLK_BYPASS_UNION;
#endif
#define SOC_NPU_AICORE_DJTAG_CLK_BYPASS_djtag_clk_bypass_en_START (0)
#define SOC_NPU_AICORE_DJTAG_CLK_BYPASS_djtag_clk_bypass_en_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long pmu_en : 1;
        unsigned long long user_profile_mode : 1;
        unsigned long long sample_profile_mode : 1;
        unsigned long long reserved : 5;
        unsigned long long pmu_overflow : 1;
        unsigned long long pmu_ctrl_reserved : 55;
    } reg;
} SOC_NPU_AICORE_PMU_CTRL_UNION;
#endif
#define SOC_NPU_AICORE_PMU_CTRL_pmu_en_START (0)
#define SOC_NPU_AICORE_PMU_CTRL_pmu_en_END (0)
#define SOC_NPU_AICORE_PMU_CTRL_user_profile_mode_START (1)
#define SOC_NPU_AICORE_PMU_CTRL_user_profile_mode_END (1)
#define SOC_NPU_AICORE_PMU_CTRL_sample_profile_mode_START (2)
#define SOC_NPU_AICORE_PMU_CTRL_sample_profile_mode_END (2)
#define SOC_NPU_AICORE_PMU_CTRL_pmu_overflow_START (8)
#define SOC_NPU_AICORE_PMU_CTRL_pmu_overflow_END (8)
#define SOC_NPU_AICORE_PMU_CTRL_pmu_ctrl_reserved_START (9)
#define SOC_NPU_AICORE_PMU_CTRL_pmu_ctrl_reserved_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long pmu_min_ov_cnt : 64;
    } reg;
} SOC_NPU_AICORE_PMU_MIN_OV_CNT_UNION;
#endif
#define SOC_NPU_AICORE_PMU_MIN_OV_CNT_pmu_min_ov_cnt_START (0)
#define SOC_NPU_AICORE_PMU_MIN_OV_CNT_pmu_min_ov_cnt_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long pmu_cnt0 : 32;
        unsigned long long reserved : 32;
    } reg;
} SOC_NPU_AICORE_PMU_CNT0_UNION;
#endif
#define SOC_NPU_AICORE_PMU_CNT0_pmu_cnt0_START (0)
#define SOC_NPU_AICORE_PMU_CNT0_pmu_cnt0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long pmu_cnt1 : 32;
        unsigned long long reserved : 32;
    } reg;
} SOC_NPU_AICORE_PMU_CNT1_UNION;
#endif
#define SOC_NPU_AICORE_PMU_CNT1_pmu_cnt1_START (0)
#define SOC_NPU_AICORE_PMU_CNT1_pmu_cnt1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long pmu_cnt2 : 32;
        unsigned long long reserved : 32;
    } reg;
} SOC_NPU_AICORE_PMU_CNT2_UNION;
#endif
#define SOC_NPU_AICORE_PMU_CNT2_pmu_cnt2_START (0)
#define SOC_NPU_AICORE_PMU_CNT2_pmu_cnt2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long pmu_cnt3 : 32;
        unsigned long long reserved : 32;
    } reg;
} SOC_NPU_AICORE_PMU_CNT3_UNION;
#endif
#define SOC_NPU_AICORE_PMU_CNT3_pmu_cnt3_START (0)
#define SOC_NPU_AICORE_PMU_CNT3_pmu_cnt3_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long pmu_cnt4 : 32;
        unsigned long long reserved : 32;
    } reg;
} SOC_NPU_AICORE_PMU_CNT4_UNION;
#endif
#define SOC_NPU_AICORE_PMU_CNT4_pmu_cnt4_START (0)
#define SOC_NPU_AICORE_PMU_CNT4_pmu_cnt4_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long pmu_cnt5 : 32;
        unsigned long long reserved : 32;
    } reg;
} SOC_NPU_AICORE_PMU_CNT5_UNION;
#endif
#define SOC_NPU_AICORE_PMU_CNT5_pmu_cnt5_START (0)
#define SOC_NPU_AICORE_PMU_CNT5_pmu_cnt5_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long pmu_cnt6 : 32;
        unsigned long long reserved : 32;
    } reg;
} SOC_NPU_AICORE_PMU_CNT6_UNION;
#endif
#define SOC_NPU_AICORE_PMU_CNT6_pmu_cnt6_START (0)
#define SOC_NPU_AICORE_PMU_CNT6_pmu_cnt6_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long pmu_cnt7 : 32;
        unsigned long long reserved : 32;
    } reg;
} SOC_NPU_AICORE_PMU_CNT7_UNION;
#endif
#define SOC_NPU_AICORE_PMU_CNT7_pmu_cnt7_START (0)
#define SOC_NPU_AICORE_PMU_CNT7_pmu_cnt7_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long pmu_task_cyc_cnt : 64;
    } reg;
} SOC_NPU_AICORE_PMU_TASK_CYC_CNT_UNION;
#endif
#define SOC_NPU_AICORE_PMU_TASK_CYC_CNT_pmu_task_cyc_cnt_START (0)
#define SOC_NPU_AICORE_PMU_TASK_CYC_CNT_pmu_task_cyc_cnt_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long pmu_cnt0_idx : 8;
        unsigned long long reserved : 56;
    } reg;
} SOC_NPU_AICORE_PMU_CNT0_IDX_UNION;
#endif
#define SOC_NPU_AICORE_PMU_CNT0_IDX_pmu_cnt0_idx_START (0)
#define SOC_NPU_AICORE_PMU_CNT0_IDX_pmu_cnt0_idx_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long pmu_cnt1_idx : 8;
        unsigned long long reserved : 56;
    } reg;
} SOC_NPU_AICORE_PMU_CNT1_IDX_UNION;
#endif
#define SOC_NPU_AICORE_PMU_CNT1_IDX_pmu_cnt1_idx_START (0)
#define SOC_NPU_AICORE_PMU_CNT1_IDX_pmu_cnt1_idx_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long pmu_cnt2_idx : 8;
        unsigned long long reserved : 56;
    } reg;
} SOC_NPU_AICORE_PMU_CNT2_IDX_UNION;
#endif
#define SOC_NPU_AICORE_PMU_CNT2_IDX_pmu_cnt2_idx_START (0)
#define SOC_NPU_AICORE_PMU_CNT2_IDX_pmu_cnt2_idx_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long pmu_cnt3_idx : 8;
        unsigned long long reserved : 56;
    } reg;
} SOC_NPU_AICORE_PMU_CNT3_IDX_UNION;
#endif
#define SOC_NPU_AICORE_PMU_CNT3_IDX_pmu_cnt3_idx_START (0)
#define SOC_NPU_AICORE_PMU_CNT3_IDX_pmu_cnt3_idx_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long pmu_cnt4_idx : 8;
        unsigned long long reserved : 56;
    } reg;
} SOC_NPU_AICORE_PMU_CNT4_IDX_UNION;
#endif
#define SOC_NPU_AICORE_PMU_CNT4_IDX_pmu_cnt4_idx_START (0)
#define SOC_NPU_AICORE_PMU_CNT4_IDX_pmu_cnt4_idx_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long pmu_cnt5_idx : 8;
        unsigned long long reserved : 56;
    } reg;
} SOC_NPU_AICORE_PMU_CNT5_IDX_UNION;
#endif
#define SOC_NPU_AICORE_PMU_CNT5_IDX_pmu_cnt5_idx_START (0)
#define SOC_NPU_AICORE_PMU_CNT5_IDX_pmu_cnt5_idx_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long pmu_cnt6_idx : 8;
        unsigned long long reserved : 56;
    } reg;
} SOC_NPU_AICORE_PMU_CNT6_IDX_UNION;
#endif
#define SOC_NPU_AICORE_PMU_CNT6_IDX_pmu_cnt6_idx_START (0)
#define SOC_NPU_AICORE_PMU_CNT6_IDX_pmu_cnt6_idx_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long pmu_cnt7_idx : 8;
        unsigned long long reserved : 56;
    } reg;
} SOC_NPU_AICORE_PMU_CNT7_IDX_UNION;
#endif
#define SOC_NPU_AICORE_PMU_CNT7_IDX_pmu_cnt7_idx_START (0)
#define SOC_NPU_AICORE_PMU_CNT7_IDX_pmu_cnt7_idx_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long pmu_start_cnt_cyc : 64;
    } reg;
} SOC_NPU_AICORE_PMU_START_CNT_CYC_UNION;
#endif
#define SOC_NPU_AICORE_PMU_START_CNT_CYC_pmu_start_cnt_cyc_START (0)
#define SOC_NPU_AICORE_PMU_START_CNT_CYC_pmu_start_cnt_cyc_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long pmu_stop_cnt_cyc : 64;
    } reg;
} SOC_NPU_AICORE_PMU_STOP_CNT_CYC_UNION;
#endif
#define SOC_NPU_AICORE_PMU_STOP_CNT_CYC_pmu_stop_cnt_cyc_START (0)
#define SOC_NPU_AICORE_PMU_STOP_CNT_CYC_pmu_stop_cnt_cyc_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long ifu_prefetch_en : 1;
        unsigned long long ifu_ic_en : 1;
        unsigned long long ifu_prefetch_en1 : 1;
        unsigned long long ifu_priority_push_en : 1;
        unsigned long long ifu_exception_back_en : 1;
        unsigned long long ifu_sw_stall : 1;
        unsigned long long ifu_sw_hw_stall_sel : 1;
        unsigned long long ifu_exit_from_except_stop : 1;
        unsigned long long ifu_reserved_reg : 56;
    } reg;
} SOC_NPU_AICORE_IFU_CTRL_UNION;
#endif
#define SOC_NPU_AICORE_IFU_CTRL_ifu_prefetch_en_START (0)
#define SOC_NPU_AICORE_IFU_CTRL_ifu_prefetch_en_END (0)
#define SOC_NPU_AICORE_IFU_CTRL_ifu_ic_en_START (1)
#define SOC_NPU_AICORE_IFU_CTRL_ifu_ic_en_END (1)
#define SOC_NPU_AICORE_IFU_CTRL_ifu_prefetch_en1_START (2)
#define SOC_NPU_AICORE_IFU_CTRL_ifu_prefetch_en1_END (2)
#define SOC_NPU_AICORE_IFU_CTRL_ifu_priority_push_en_START (3)
#define SOC_NPU_AICORE_IFU_CTRL_ifu_priority_push_en_END (3)
#define SOC_NPU_AICORE_IFU_CTRL_ifu_exception_back_en_START (4)
#define SOC_NPU_AICORE_IFU_CTRL_ifu_exception_back_en_END (4)
#define SOC_NPU_AICORE_IFU_CTRL_ifu_sw_stall_START (5)
#define SOC_NPU_AICORE_IFU_CTRL_ifu_sw_stall_END (5)
#define SOC_NPU_AICORE_IFU_CTRL_ifu_sw_hw_stall_sel_START (6)
#define SOC_NPU_AICORE_IFU_CTRL_ifu_sw_hw_stall_sel_END (6)
#define SOC_NPU_AICORE_IFU_CTRL_ifu_exit_from_except_stop_START (7)
#define SOC_NPU_AICORE_IFU_CTRL_ifu_exit_from_except_stop_END (7)
#define SOC_NPU_AICORE_IFU_CTRL_ifu_reserved_reg_START (8)
#define SOC_NPU_AICORE_IFU_CTRL_ifu_reserved_reg_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long ic_inv_one : 1;
        unsigned long long ic_inv_all : 1;
        unsigned long long ifu_preload_req : 1;
        unsigned long long ifu_preload_stop : 1;
        unsigned long long reserved : 60;
    } reg;
} SOC_NPU_AICORE_IC_INV_CTRL_UNION;
#endif
#define SOC_NPU_AICORE_IC_INV_CTRL_ic_inv_one_START (0)
#define SOC_NPU_AICORE_IC_INV_CTRL_ic_inv_one_END (0)
#define SOC_NPU_AICORE_IC_INV_CTRL_ic_inv_all_START (1)
#define SOC_NPU_AICORE_IC_INV_CTRL_ic_inv_all_END (1)
#define SOC_NPU_AICORE_IC_INV_CTRL_ifu_preload_req_START (2)
#define SOC_NPU_AICORE_IC_INV_CTRL_ifu_preload_req_END (2)
#define SOC_NPU_AICORE_IC_INV_CTRL_ifu_preload_stop_START (3)
#define SOC_NPU_AICORE_IC_INV_CTRL_ifu_preload_stop_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long reserved_0: 7;
        unsigned long long ic_inv_va : 41;
        unsigned long long reserved_1: 16;
    } reg;
} SOC_NPU_AICORE_IC_INV_VA_UNION;
#endif
#define SOC_NPU_AICORE_IC_INV_VA_ic_inv_va_START (7)
#define SOC_NPU_AICORE_IC_INV_VA_ic_inv_va_END (47)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long inv_done : 1;
        unsigned long long reserved : 63;
    } reg;
} SOC_NPU_AICORE_IC_INV_STATUS_UNION;
#endif
#define SOC_NPU_AICORE_IC_INV_STATUS_inv_done_START (0)
#define SOC_NPU_AICORE_IC_INV_STATUS_inv_done_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long hw_bkpt_en : 16;
        unsigned long long reserved : 48;
    } reg;
} SOC_NPU_AICORE_HW_BKPT_EN_UNION;
#endif
#define SOC_NPU_AICORE_HW_BKPT_EN_hw_bkpt_en_START (0)
#define SOC_NPU_AICORE_HW_BKPT_EN_hw_bkpt_en_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long reserved : 2;
        unsigned long long hw_bkpt0_pc : 46;
        unsigned long long hw_bkpt0_id : 16;
    } reg;
} SOC_NPU_AICORE_HW_BKPT0_PC_UNION;
#endif
#define SOC_NPU_AICORE_HW_BKPT0_PC_hw_bkpt0_pc_START (2)
#define SOC_NPU_AICORE_HW_BKPT0_PC_hw_bkpt0_pc_END (47)
#define SOC_NPU_AICORE_HW_BKPT0_PC_hw_bkpt0_id_START (48)
#define SOC_NPU_AICORE_HW_BKPT0_PC_hw_bkpt0_id_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long reserved : 2;
        unsigned long long hw_bkpt1_pc : 46;
        unsigned long long hw_bkpt1_id : 16;
    } reg;
} SOC_NPU_AICORE_HW_BKPT1_PC_UNION;
#endif
#define SOC_NPU_AICORE_HW_BKPT1_PC_hw_bkpt1_pc_START (2)
#define SOC_NPU_AICORE_HW_BKPT1_PC_hw_bkpt1_pc_END (47)
#define SOC_NPU_AICORE_HW_BKPT1_PC_hw_bkpt1_id_START (48)
#define SOC_NPU_AICORE_HW_BKPT1_PC_hw_bkpt1_id_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long reserved : 2;
        unsigned long long hw_bkpt2_pc : 46;
        unsigned long long hw_bkpt2_id : 16;
    } reg;
} SOC_NPU_AICORE_HW_BKPT2_PC_UNION;
#endif
#define SOC_NPU_AICORE_HW_BKPT2_PC_hw_bkpt2_pc_START (2)
#define SOC_NPU_AICORE_HW_BKPT2_PC_hw_bkpt2_pc_END (47)
#define SOC_NPU_AICORE_HW_BKPT2_PC_hw_bkpt2_id_START (48)
#define SOC_NPU_AICORE_HW_BKPT2_PC_hw_bkpt2_id_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long reserved : 2;
        unsigned long long hw_bkpt3_pc : 46;
        unsigned long long hw_bkpt3_id : 16;
    } reg;
} SOC_NPU_AICORE_HW_BKPT3_PC_UNION;
#endif
#define SOC_NPU_AICORE_HW_BKPT3_PC_hw_bkpt3_pc_START (2)
#define SOC_NPU_AICORE_HW_BKPT3_PC_hw_bkpt3_pc_END (47)
#define SOC_NPU_AICORE_HW_BKPT3_PC_hw_bkpt3_id_START (48)
#define SOC_NPU_AICORE_HW_BKPT3_PC_hw_bkpt3_id_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long reserved : 2;
        unsigned long long hw_bkpt4_pc : 46;
        unsigned long long hw_bkpt4_id : 16;
    } reg;
} SOC_NPU_AICORE_HW_BKPT4_PC_UNION;
#endif
#define SOC_NPU_AICORE_HW_BKPT4_PC_hw_bkpt4_pc_START (2)
#define SOC_NPU_AICORE_HW_BKPT4_PC_hw_bkpt4_pc_END (47)
#define SOC_NPU_AICORE_HW_BKPT4_PC_hw_bkpt4_id_START (48)
#define SOC_NPU_AICORE_HW_BKPT4_PC_hw_bkpt4_id_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long reserved : 2;
        unsigned long long hw_bkpt5_pc : 46;
        unsigned long long hw_bkpt5_id : 16;
    } reg;
} SOC_NPU_AICORE_HW_BKPT5_PC_UNION;
#endif
#define SOC_NPU_AICORE_HW_BKPT5_PC_hw_bkpt5_pc_START (2)
#define SOC_NPU_AICORE_HW_BKPT5_PC_hw_bkpt5_pc_END (47)
#define SOC_NPU_AICORE_HW_BKPT5_PC_hw_bkpt5_id_START (48)
#define SOC_NPU_AICORE_HW_BKPT5_PC_hw_bkpt5_id_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long reserved : 2;
        unsigned long long hw_bkpt6_pc : 46;
        unsigned long long hw_bkpt6_id : 16;
    } reg;
} SOC_NPU_AICORE_HW_BKPT6_PC_UNION;
#endif
#define SOC_NPU_AICORE_HW_BKPT6_PC_hw_bkpt6_pc_START (2)
#define SOC_NPU_AICORE_HW_BKPT6_PC_hw_bkpt6_pc_END (47)
#define SOC_NPU_AICORE_HW_BKPT6_PC_hw_bkpt6_id_START (48)
#define SOC_NPU_AICORE_HW_BKPT6_PC_hw_bkpt6_id_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long reserved : 2;
        unsigned long long hw_bkpt7_pc : 46;
        unsigned long long hw_bkpt7_id : 16;
    } reg;
} SOC_NPU_AICORE_HW_BKPT7_PC_UNION;
#endif
#define SOC_NPU_AICORE_HW_BKPT7_PC_hw_bkpt7_pc_START (2)
#define SOC_NPU_AICORE_HW_BKPT7_PC_hw_bkpt7_pc_END (47)
#define SOC_NPU_AICORE_HW_BKPT7_PC_hw_bkpt7_id_START (48)
#define SOC_NPU_AICORE_HW_BKPT7_PC_hw_bkpt7_id_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long reserved : 2;
        unsigned long long hw_bkpt8_pc : 46;
        unsigned long long hw_bkpt8_id : 16;
    } reg;
} SOC_NPU_AICORE_HW_BKPT8_PC_UNION;
#endif
#define SOC_NPU_AICORE_HW_BKPT8_PC_hw_bkpt8_pc_START (2)
#define SOC_NPU_AICORE_HW_BKPT8_PC_hw_bkpt8_pc_END (47)
#define SOC_NPU_AICORE_HW_BKPT8_PC_hw_bkpt8_id_START (48)
#define SOC_NPU_AICORE_HW_BKPT8_PC_hw_bkpt8_id_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long reserved : 2;
        unsigned long long hw_bkpt9_pc : 46;
        unsigned long long hw_bkpt9_id : 16;
    } reg;
} SOC_NPU_AICORE_HW_BKPT9_PC_UNION;
#endif
#define SOC_NPU_AICORE_HW_BKPT9_PC_hw_bkpt9_pc_START (2)
#define SOC_NPU_AICORE_HW_BKPT9_PC_hw_bkpt9_pc_END (47)
#define SOC_NPU_AICORE_HW_BKPT9_PC_hw_bkpt9_id_START (48)
#define SOC_NPU_AICORE_HW_BKPT9_PC_hw_bkpt9_id_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long reserved : 2;
        unsigned long long hw_bkpt10_pc : 46;
        unsigned long long hw_bkpt10_id : 16;
    } reg;
} SOC_NPU_AICORE_HW_BKPT10_PC_UNION;
#endif
#define SOC_NPU_AICORE_HW_BKPT10_PC_hw_bkpt10_pc_START (2)
#define SOC_NPU_AICORE_HW_BKPT10_PC_hw_bkpt10_pc_END (47)
#define SOC_NPU_AICORE_HW_BKPT10_PC_hw_bkpt10_id_START (48)
#define SOC_NPU_AICORE_HW_BKPT10_PC_hw_bkpt10_id_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long reserved : 2;
        unsigned long long hw_bkpt11_pc : 46;
        unsigned long long hw_bkpt11_id : 16;
    } reg;
} SOC_NPU_AICORE_HW_BKPT11_PC_UNION;
#endif
#define SOC_NPU_AICORE_HW_BKPT11_PC_hw_bkpt11_pc_START (2)
#define SOC_NPU_AICORE_HW_BKPT11_PC_hw_bkpt11_pc_END (47)
#define SOC_NPU_AICORE_HW_BKPT11_PC_hw_bkpt11_id_START (48)
#define SOC_NPU_AICORE_HW_BKPT11_PC_hw_bkpt11_id_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long reserved : 2;
        unsigned long long hw_bkpt12_pc : 46;
        unsigned long long hw_bkpt12_id : 16;
    } reg;
} SOC_NPU_AICORE_HW_BKPT12_PC_UNION;
#endif
#define SOC_NPU_AICORE_HW_BKPT12_PC_hw_bkpt12_pc_START (2)
#define SOC_NPU_AICORE_HW_BKPT12_PC_hw_bkpt12_pc_END (47)
#define SOC_NPU_AICORE_HW_BKPT12_PC_hw_bkpt12_id_START (48)
#define SOC_NPU_AICORE_HW_BKPT12_PC_hw_bkpt12_id_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long reserved : 2;
        unsigned long long hw_bkpt13_pc : 46;
        unsigned long long hw_bkpt13_id : 16;
    } reg;
} SOC_NPU_AICORE_HW_BKPT13_PC_UNION;
#endif
#define SOC_NPU_AICORE_HW_BKPT13_PC_hw_bkpt13_pc_START (2)
#define SOC_NPU_AICORE_HW_BKPT13_PC_hw_bkpt13_pc_END (47)
#define SOC_NPU_AICORE_HW_BKPT13_PC_hw_bkpt13_id_START (48)
#define SOC_NPU_AICORE_HW_BKPT13_PC_hw_bkpt13_id_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long reserved : 2;
        unsigned long long hw_bkpt14_pc : 46;
        unsigned long long hw_bkpt14_id : 16;
    } reg;
} SOC_NPU_AICORE_HW_BKPT14_PC_UNION;
#endif
#define SOC_NPU_AICORE_HW_BKPT14_PC_hw_bkpt14_pc_START (2)
#define SOC_NPU_AICORE_HW_BKPT14_PC_hw_bkpt14_pc_END (47)
#define SOC_NPU_AICORE_HW_BKPT14_PC_hw_bkpt14_id_START (48)
#define SOC_NPU_AICORE_HW_BKPT14_PC_hw_bkpt14_id_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long reserved : 2;
        unsigned long long hw_bkpt15_pc : 46;
        unsigned long long hw_bkpt15_id : 16;
    } reg;
} SOC_NPU_AICORE_HW_BKPT15_PC_UNION;
#endif
#define SOC_NPU_AICORE_HW_BKPT15_PC_hw_bkpt15_pc_START (2)
#define SOC_NPU_AICORE_HW_BKPT15_PC_hw_bkpt15_pc_END (47)
#define SOC_NPU_AICORE_HW_BKPT15_PC_hw_bkpt15_id_START (48)
#define SOC_NPU_AICORE_HW_BKPT15_PC_hw_bkpt15_id_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long ccu_iq_timeout : 32;
        unsigned long long reserved : 32;
    } reg;
} SOC_NPU_AICORE_CCU_IQ_TIMEOUT_UNION;
#endif
#define SOC_NPU_AICORE_CCU_IQ_TIMEOUT_ccu_iq_timeout_START (0)
#define SOC_NPU_AICORE_CCU_IQ_TIMEOUT_ccu_iq_timeout_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long ccu_single_issue_mode : 1;
        unsigned long long mte1_single_commit : 1;
        unsigned long long mte2_single_commit : 1;
        unsigned long long mte3_single_commit : 1;
        unsigned long long vec_single_commit : 1;
        unsigned long long cube_single_commit : 1;
        unsigned long long global_single_commit : 1;
        unsigned long long ccu_rff_bp_en : 1;
        unsigned long long mte4_single_commit : 1;
        unsigned long long mte5_single_commit : 1;
        unsigned long long ccu_dc_single_commit : 1;
        unsigned long long ccu_reserved_reg : 53;
    } reg;
} SOC_NPU_AICORE_CCU_CTRL_UNION;
#endif
#define SOC_NPU_AICORE_CCU_CTRL_ccu_single_issue_mode_START (0)
#define SOC_NPU_AICORE_CCU_CTRL_ccu_single_issue_mode_END (0)
#define SOC_NPU_AICORE_CCU_CTRL_mte1_single_commit_START (1)
#define SOC_NPU_AICORE_CCU_CTRL_mte1_single_commit_END (1)
#define SOC_NPU_AICORE_CCU_CTRL_mte2_single_commit_START (2)
#define SOC_NPU_AICORE_CCU_CTRL_mte2_single_commit_END (2)
#define SOC_NPU_AICORE_CCU_CTRL_mte3_single_commit_START (3)
#define SOC_NPU_AICORE_CCU_CTRL_mte3_single_commit_END (3)
#define SOC_NPU_AICORE_CCU_CTRL_vec_single_commit_START (4)
#define SOC_NPU_AICORE_CCU_CTRL_vec_single_commit_END (4)
#define SOC_NPU_AICORE_CCU_CTRL_cube_single_commit_START (5)
#define SOC_NPU_AICORE_CCU_CTRL_cube_single_commit_END (5)
#define SOC_NPU_AICORE_CCU_CTRL_global_single_commit_START (6)
#define SOC_NPU_AICORE_CCU_CTRL_global_single_commit_END (6)
#define SOC_NPU_AICORE_CCU_CTRL_ccu_rff_bp_en_START (7)
#define SOC_NPU_AICORE_CCU_CTRL_ccu_rff_bp_en_END (7)
#define SOC_NPU_AICORE_CCU_CTRL_mte4_single_commit_START (8)
#define SOC_NPU_AICORE_CCU_CTRL_mte4_single_commit_END (8)
#define SOC_NPU_AICORE_CCU_CTRL_mte5_single_commit_START (9)
#define SOC_NPU_AICORE_CCU_CTRL_mte5_single_commit_END (9)
#define SOC_NPU_AICORE_CCU_CTRL_ccu_dc_single_commit_START (10)
#define SOC_NPU_AICORE_CCU_CTRL_ccu_dc_single_commit_END (10)
#define SOC_NPU_AICORE_CCU_CTRL_ccu_reserved_reg_START (11)
#define SOC_NPU_AICORE_CCU_CTRL_ccu_reserved_reg_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long spr_status : 64;
    } reg;
} SOC_NPU_AICORE_SPR_STATUS_UNION;
#endif
#define SOC_NPU_AICORE_SPR_STATUS_spr_status_START (0)
#define SOC_NPU_AICORE_SPR_STATUS_spr_status_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long ccu_cond_spr : 64;
    } reg;
} SOC_NPU_AICORE_COND_SPR_UNION;
#endif
#define SOC_NPU_AICORE_COND_SPR_ccu_cond_spr_START (0)
#define SOC_NPU_AICORE_COND_SPR_ccu_cond_spr_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long redirect_ifu_status : 1;
        unsigned long long reserved : 63;
    } reg;
} SOC_NPU_AICORE_CCU_REDIRECT_IFU_STATUS_UNION;
#endif
#define SOC_NPU_AICORE_CCU_REDIRECT_IFU_STATUS_redirect_ifu_status_START (0)
#define SOC_NPU_AICORE_CCU_REDIRECT_IFU_STATUS_redirect_ifu_status_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long biu_ctrl0 : 64;
    } reg;
} SOC_NPU_AICORE_BIU_CTRL0_UNION;
#endif
#define SOC_NPU_AICORE_BIU_CTRL0_biu_ctrl0_START (0)
#define SOC_NPU_AICORE_BIU_CTRL0_biu_ctrl0_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long biu_ctrl1 : 64;
    } reg;
} SOC_NPU_AICORE_BIU_CTRL1_UNION;
#endif
#define SOC_NPU_AICORE_BIU_CTRL1_biu_ctrl1_START (0)
#define SOC_NPU_AICORE_BIU_CTRL1_biu_ctrl1_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long biu_ctrl2 : 64;
    } reg;
} SOC_NPU_AICORE_BIU_CTRL2_UNION;
#endif
#define SOC_NPU_AICORE_BIU_CTRL2_biu_ctrl2_START (0)
#define SOC_NPU_AICORE_BIU_CTRL2_biu_ctrl2_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long biu_status0 : 64;
    } reg;
} SOC_NPU_AICORE_BIU_STATUS0_UNION;
#endif
#define SOC_NPU_AICORE_BIU_STATUS0_biu_status0_START (0)
#define SOC_NPU_AICORE_BIU_STATUS0_biu_status0_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long biu_status1 : 64;
    } reg;
} SOC_NPU_AICORE_BIU_STATUS1_UNION;
#endif
#define SOC_NPU_AICORE_BIU_STATUS1_biu_status1_START (0)
#define SOC_NPU_AICORE_BIU_STATUS1_biu_status1_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long biu_status2 : 64;
    } reg;
} SOC_NPU_AICORE_BIU_STATUS2_UNION;
#endif
#define SOC_NPU_AICORE_BIU_STATUS2_biu_status2_START (0)
#define SOC_NPU_AICORE_BIU_STATUS2_biu_status2_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long biu_status3 : 64;
    } reg;
} SOC_NPU_AICORE_BIU_STATUS3_UNION;
#endif
#define SOC_NPU_AICORE_BIU_STATUS3_biu_status3_START (0)
#define SOC_NPU_AICORE_BIU_STATUS3_biu_status3_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long biu_status4 : 64;
    } reg;
} SOC_NPU_AICORE_BIU_STATUS4_UNION;
#endif
#define SOC_NPU_AICORE_BIU_STATUS4_biu_status4_START (0)
#define SOC_NPU_AICORE_BIU_STATUS4_biu_status4_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long biu_status5 : 64;
    } reg;
} SOC_NPU_AICORE_BIU_STATUS5_UNION;
#endif
#define SOC_NPU_AICORE_BIU_STATUS5_biu_status5_START (0)
#define SOC_NPU_AICORE_BIU_STATUS5_biu_status5_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long biu_smmu_streamid : 16;
        unsigned long long reserved : 48;
    } reg;
} SOC_NPU_AICORE_BIU_SMMU_STREAMID_UNION;
#endif
#define SOC_NPU_AICORE_BIU_SMMU_STREAMID_biu_smmu_streamid_START (0)
#define SOC_NPU_AICORE_BIU_SMMU_STREAMID_biu_smmu_streamid_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long biu_l2_paddr_base : 64;
    } reg;
} SOC_NPU_AICORE_BIU_L2_REMAP_PADDR_BASE_UNION;
#endif
#define SOC_NPU_AICORE_BIU_L2_REMAP_PADDR_BASE_biu_l2_paddr_base_START (0)
#define SOC_NPU_AICORE_BIU_L2_REMAP_PADDR_BASE_biu_l2_paddr_base_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long biu_l2_size : 4;
        unsigned long long reserved : 59;
        unsigned long long biu_l2_remap_en : 1;
    } reg;
} SOC_NPU_AICORE_BIU_L2_SIZE_UNION;
#endif
#define SOC_NPU_AICORE_BIU_L2_SIZE_biu_l2_size_START (0)
#define SOC_NPU_AICORE_BIU_L2_SIZE_biu_l2_size_END (3)
#define SOC_NPU_AICORE_BIU_L2_SIZE_biu_l2_remap_en_START (63)
#define SOC_NPU_AICORE_BIU_L2_SIZE_biu_l2_remap_en_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long biu_l2_page_size : 4;
        unsigned long long reserved : 60;
    } reg;
} SOC_NPU_AICORE_BIU_L2_PAGE_SIZE_UNION;
#endif
#define SOC_NPU_AICORE_BIU_L2_PAGE_SIZE_biu_l2_page_size_START (0)
#define SOC_NPU_AICORE_BIU_L2_PAGE_SIZE_biu_l2_page_size_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long biu_ctrl6 : 64;
    } reg;
} SOC_NPU_AICORE_BIU_CTRL6_UNION;
#endif
#define SOC_NPU_AICORE_BIU_CTRL6_biu_ctrl6_START (0)
#define SOC_NPU_AICORE_BIU_CTRL6_biu_ctrl6_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long biu_status8 : 64;
    } reg;
} SOC_NPU_AICORE_BIU_STATUS8_UNION;
#endif
#define SOC_NPU_AICORE_BIU_STATUS8_biu_status8_START (0)
#define SOC_NPU_AICORE_BIU_STATUS8_biu_status8_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long biu_stream_ns : 1;
        unsigned long long reserved : 63;
    } reg;
} SOC_NPU_AICORE_BIU_STREAM_NS_UNION;
#endif
#define SOC_NPU_AICORE_BIU_STREAM_NS_biu_stream_ns_START (0)
#define SOC_NPU_AICORE_BIU_STREAM_NS_biu_stream_ns_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long pqos_otsd0_lvl : 8;
        unsigned long long pqos_otsd1_lvl : 8;
        unsigned long long pqos_otsd2_lvl : 8;
        unsigned long long reserved : 40;
    } reg;
} SOC_NPU_AICORE_BIU_CTRL7_UNION;
#endif
#define SOC_NPU_AICORE_BIU_CTRL7_pqos_otsd0_lvl_START (0)
#define SOC_NPU_AICORE_BIU_CTRL7_pqos_otsd0_lvl_END (7)
#define SOC_NPU_AICORE_BIU_CTRL7_pqos_otsd1_lvl_START (8)
#define SOC_NPU_AICORE_BIU_CTRL7_pqos_otsd1_lvl_END (15)
#define SOC_NPU_AICORE_BIU_CTRL7_pqos_otsd2_lvl_START (16)
#define SOC_NPU_AICORE_BIU_CTRL7_pqos_otsd2_lvl_END (23)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long reserved_0 : 1;
        unsigned long long addr_intlv_cfg : 2;
        unsigned long long reserved_1 : 1;
        unsigned long long rosd_ctrl_en : 3;
        unsigned long long reserved_2 : 1;
        unsigned long long rosd_ifu_th : 3;
        unsigned long long reserved_3 : 1;
        unsigned long long rosd_dc_th : 2;
        unsigned long long reserved_4 : 2;
        unsigned long long rosd_mte_th : 7;
        unsigned long long reserved_5 : 1;
        unsigned long long wosd_ctrl_en : 2;
        unsigned long long reserved_6 : 2;
        unsigned long long wosd_dc_th : 2;
        unsigned long long reserved_7 : 2;
        unsigned long long wosd_mte_th : 6;
        unsigned long long reserved_8 : 26;
    } reg;
} SOC_NPU_AICORE_BIU_CTRL8_UNION;
#endif
#define SOC_NPU_AICORE_BIU_CTRL8_addr_intlv_cfg_START (1)
#define SOC_NPU_AICORE_BIU_CTRL8_addr_intlv_cfg_END (2)
#define SOC_NPU_AICORE_BIU_CTRL8_rosd_ctrl_en_START (4)
#define SOC_NPU_AICORE_BIU_CTRL8_rosd_ctrl_en_END (6)
#define SOC_NPU_AICORE_BIU_CTRL8_rosd_ifu_th_START (8)
#define SOC_NPU_AICORE_BIU_CTRL8_rosd_ifu_th_END (10)
#define SOC_NPU_AICORE_BIU_CTRL8_rosd_dc_th_START (12)
#define SOC_NPU_AICORE_BIU_CTRL8_rosd_dc_th_END (13)
#define SOC_NPU_AICORE_BIU_CTRL8_rosd_mte_th_START (16)
#define SOC_NPU_AICORE_BIU_CTRL8_rosd_mte_th_END (22)
#define SOC_NPU_AICORE_BIU_CTRL8_wosd_ctrl_en_START (24)
#define SOC_NPU_AICORE_BIU_CTRL8_wosd_ctrl_en_END (25)
#define SOC_NPU_AICORE_BIU_CTRL8_wosd_dc_th_START (28)
#define SOC_NPU_AICORE_BIU_CTRL8_wosd_dc_th_END (29)
#define SOC_NPU_AICORE_BIU_CTRL8_wosd_mte_th_START (32)
#define SOC_NPU_AICORE_BIU_CTRL8_wosd_mte_th_END (37)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long smmu_bp_sel : 2;
        unsigned long long smmu_ssidv_unbp : 1;
        unsigned long long smmu_ssidv_bp : 1;
        unsigned long long reserved_0 : 12;
        unsigned long long smmu_streamid_bp : 16;
        unsigned long long reserved_1 : 32;
    } reg;
} SOC_NPU_AICORE_BIU_SMMU_CFG_UNION;
#endif
#define SOC_NPU_AICORE_BIU_SMMU_CFG_smmu_bp_sel_START (0)
#define SOC_NPU_AICORE_BIU_SMMU_CFG_smmu_bp_sel_END (1)
#define SOC_NPU_AICORE_BIU_SMMU_CFG_smmu_ssidv_unbp_START (2)
#define SOC_NPU_AICORE_BIU_SMMU_CFG_smmu_ssidv_unbp_END (2)
#define SOC_NPU_AICORE_BIU_SMMU_CFG_smmu_ssidv_bp_START (3)
#define SOC_NPU_AICORE_BIU_SMMU_CFG_smmu_ssidv_bp_END (3)
#define SOC_NPU_AICORE_BIU_SMMU_CFG_smmu_streamid_bp_START (16)
#define SOC_NPU_AICORE_BIU_SMMU_CFG_smmu_streamid_bp_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long smmu_sec_strmid_unbp : 16;
        unsigned long long smmu_sec_strmid_bp : 16;
        unsigned long long reserved : 32;
    } reg;
} SOC_NPU_AICORE_SMMU_SEC_STREAMID_UNION;
#endif
#define SOC_NPU_AICORE_SMMU_SEC_STREAMID_smmu_sec_strmid_unbp_START (0)
#define SOC_NPU_AICORE_SMMU_SEC_STREAMID_smmu_sec_strmid_unbp_END (15)
#define SOC_NPU_AICORE_SMMU_SEC_STREAMID_smmu_sec_strmid_bp_START (16)
#define SOC_NPU_AICORE_SMMU_SEC_STREAMID_smmu_sec_strmid_bp_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long biu_status9 : 64;
    } reg;
} SOC_NPU_AICORE_BIU_STATUS9_UNION;
#endif
#define SOC_NPU_AICORE_BIU_STATUS9_biu_status9_START (0)
#define SOC_NPU_AICORE_BIU_STATUS9_biu_status9_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long core_id : 16;
        unsigned long long reserved : 48;
    } reg;
} SOC_NPU_AICORE_CORE_ID_UNION;
#endif
#define SOC_NPU_AICORE_CORE_ID_core_id_START (0)
#define SOC_NPU_AICORE_CORE_ID_core_id_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long aic_tag_ver : 32;
        unsigned long long core_ver : 16;
        unsigned long long isa_ver : 16;
    } reg;
} SOC_NPU_AICORE_AIC_VER_UNION;
#endif
#define SOC_NPU_AICORE_AIC_VER_aic_tag_ver_START (0)
#define SOC_NPU_AICORE_AIC_VER_aic_tag_ver_END (31)
#define SOC_NPU_AICORE_AIC_VER_core_ver_START (32)
#define SOC_NPU_AICORE_AIC_VER_core_ver_END (47)
#define SOC_NPU_AICORE_AIC_VER_isa_ver_START (48)
#define SOC_NPU_AICORE_AIC_VER_isa_ver_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long smmu_svn_ver : 64;
    } reg;
} SOC_NPU_AICORE_SMMU_VER_UNION;
#endif
#define SOC_NPU_AICORE_SMMU_VER_smmu_svn_ver_START (0)
#define SOC_NPU_AICORE_SMMU_VER_smmu_svn_ver_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long dispatch_svn_ver : 64;
    } reg;
} SOC_NPU_AICORE_DISPATCH_VER_UNION;
#endif
#define SOC_NPU_AICORE_DISPATCH_VER_dispatch_svn_ver_START (0)
#define SOC_NPU_AICORE_DISPATCH_VER_dispatch_svn_ver_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long aa_svn_ver : 64;
    } reg;
} SOC_NPU_AICORE_AA_VER_UNION;
#endif
#define SOC_NPU_AICORE_AA_VER_aa_svn_ver_START (0)
#define SOC_NPU_AICORE_AA_VER_aa_svn_ver_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long crg_svn_ver : 64;
    } reg;
} SOC_NPU_AICORE_CRG_VER_UNION;
#endif
#define SOC_NPU_AICORE_CRG_VER_crg_svn_ver_START (0)
#define SOC_NPU_AICORE_CRG_VER_crg_svn_ver_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long smmu_mem_power_mode : 6;
        unsigned long long reserved_0 : 2;
        unsigned long long aic_mem_power_mode : 6;
        unsigned long long reserved_1 : 48;
        unsigned long long sd_mode : 1;
        unsigned long long pudelay_aic : 1;
    } reg;
} SOC_NPU_AICORE_POWER_MODE_CTRL_UNION;
#endif
#define SOC_NPU_AICORE_POWER_MODE_CTRL_smmu_mem_power_mode_START (0)
#define SOC_NPU_AICORE_POWER_MODE_CTRL_smmu_mem_power_mode_END (5)
#define SOC_NPU_AICORE_POWER_MODE_CTRL_aic_mem_power_mode_START (8)
#define SOC_NPU_AICORE_POWER_MODE_CTRL_aic_mem_power_mode_END (13)
#define SOC_NPU_AICORE_POWER_MODE_CTRL_sd_mode_START (62)
#define SOC_NPU_AICORE_POWER_MODE_CTRL_sd_mode_END (62)
#define SOC_NPU_AICORE_POWER_MODE_CTRL_pudelay_aic_START (63)
#define SOC_NPU_AICORE_POWER_MODE_CTRL_pudelay_aic_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long smmu_tp_ram_tmod : 10;
        unsigned long long reserved_0 : 6;
        unsigned long long smmu_sp_ram_tmod : 10;
        unsigned long long reserved_1 : 6;
        unsigned long long aic_tp_ram_tmod : 10;
        unsigned long long reserved_2 : 6;
        unsigned long long aic_sp_ram_tmod : 10;
        unsigned long long reserved_3 : 6;
    } reg;
} SOC_NPU_AICORE_MEM_TMOD_CTRL_UNION;
#endif
#define SOC_NPU_AICORE_MEM_TMOD_CTRL_smmu_tp_ram_tmod_START (0)
#define SOC_NPU_AICORE_MEM_TMOD_CTRL_smmu_tp_ram_tmod_END (9)
#define SOC_NPU_AICORE_MEM_TMOD_CTRL_smmu_sp_ram_tmod_START (16)
#define SOC_NPU_AICORE_MEM_TMOD_CTRL_smmu_sp_ram_tmod_END (25)
#define SOC_NPU_AICORE_MEM_TMOD_CTRL_aic_tp_ram_tmod_START (32)
#define SOC_NPU_AICORE_MEM_TMOD_CTRL_aic_tp_ram_tmod_END (41)
#define SOC_NPU_AICORE_MEM_TMOD_CTRL_aic_sp_ram_tmod_START (48)
#define SOC_NPU_AICORE_MEM_TMOD_CTRL_aic_sp_ram_tmod_END (57)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long biu_l2_read_oob : 1;
        unsigned long long biu_l2_write_oob : 1;
        unsigned long long ccu_call_depth_ovrflw : 1;
        unsigned long long ccu_div0 : 1;
        unsigned long long ccu_illegal_instr : 1;
        unsigned long long ccu_loop_cnt_err : 1;
        unsigned long long ccu_loop_err : 1;
        unsigned long long ccu_neg_sqrt : 1;
        unsigned long long ccu_ub_ecc : 1;
        unsigned long long cube_invld_input : 1;
        unsigned long long cube_l0a_ecc : 1;
        unsigned long long cube_l0a_rdwr_cflt : 1;
        unsigned long long cube_l0a_wrap_around : 1;
        unsigned long long cube_l0b_ecc : 1;
        unsigned long long cube_l0b_rdwr_cflt : 1;
        unsigned long long cube_l0b_wrap_around : 1;
        unsigned long long cube_l0c_ecc : 1;
        unsigned long long cube_l0c_rdwr_cflt : 1;
        unsigned long long cube_l0c_self_rdwr_cflt : 1;
        unsigned long long cube_l0c_wrap_around : 1;
        unsigned long long ifu_bus_err : 1;
        unsigned long long mte_aipp_illegal_param : 1;
        unsigned long long mte_bas_raddr_obound : 1;
        unsigned long long mte_biu_rdwr_resp : 1;
        unsigned long long mte_cidx_overflow : 1;
        unsigned long long mte_decomp : 1;
        unsigned long long mte_f1wpos_larger_fsize : 1;
        unsigned long long mte_fmap_less_kernel : 1;
        unsigned long long mte_fmapwh_larger_l1size : 1;
        unsigned long long mte_fpos_larger_fsize : 1;
        unsigned long long mte_gdma_illegal_burst_len : 1;
        unsigned long long mte_gdma_illegal_burst_num : 1;
        unsigned long long mte_gdma_read_overflow : 1;
        unsigned long long mte_gdma_write_overflow : 1;
        unsigned long long mte_comp : 1;
        unsigned long long mte_illegal_fm_size : 1;
        unsigned long long mte_illegal_l1_3d_size : 1;
        unsigned long long mte_illegal_stride : 1;
        unsigned long long mte_l0a_rdwr_cflt : 1;
        unsigned long long mte_l0b_rdwr_cflt : 1;
        unsigned long long mte_l1_ecc : 1;
        unsigned long long mte_padding_cfg : 1;
        unsigned long long mte_read_overflow : 1;
        unsigned long long mte_rob_ecc : 1;
        unsigned long long mte_tlu_ecc : 1;
        unsigned long long mte_ub_ecc : 1;
        unsigned long long mte_unzip : 1;
        unsigned long long mte_write_3d_overflow : 1;
        unsigned long long mte_write_overflow : 1;
        unsigned long long vec_data_excp_ccu : 1;
        unsigned long long vec_data_excp_mte : 1;
        unsigned long long vec_data_excp_vec : 1;
        unsigned long long vec_div0 : 1;
        unsigned long long vec_illegal_mask : 1;
        unsigned long long vec_inf_nan : 1;
        unsigned long long vec_l0c_ecc : 1;
        unsigned long long vec_l0c_rdwr_cflt : 1;
        unsigned long long vec_neg_ln : 1;
        unsigned long long vec_neg_sqrt : 1;
        unsigned long long vec_same_blk_addr : 1;
        unsigned long long vec_ub_ecc : 1;
        unsigned long long vec_ub_self_rdwr_cflt : 1;
        unsigned long long vec_ub_wrap_around : 1;
        unsigned long long biu_dfx_err : 1;
    } reg;
} SOC_NPU_AICORE_AIC_ERROR_UNION;
#endif
#define SOC_NPU_AICORE_AIC_ERROR_biu_l2_read_oob_START (0)
#define SOC_NPU_AICORE_AIC_ERROR_biu_l2_read_oob_END (0)
#define SOC_NPU_AICORE_AIC_ERROR_biu_l2_write_oob_START (1)
#define SOC_NPU_AICORE_AIC_ERROR_biu_l2_write_oob_END (1)
#define SOC_NPU_AICORE_AIC_ERROR_ccu_call_depth_ovrflw_START (2)
#define SOC_NPU_AICORE_AIC_ERROR_ccu_call_depth_ovrflw_END (2)
#define SOC_NPU_AICORE_AIC_ERROR_ccu_div0_START (3)
#define SOC_NPU_AICORE_AIC_ERROR_ccu_div0_END (3)
#define SOC_NPU_AICORE_AIC_ERROR_ccu_illegal_instr_START (4)
#define SOC_NPU_AICORE_AIC_ERROR_ccu_illegal_instr_END (4)
#define SOC_NPU_AICORE_AIC_ERROR_ccu_loop_cnt_err_START (5)
#define SOC_NPU_AICORE_AIC_ERROR_ccu_loop_cnt_err_END (5)
#define SOC_NPU_AICORE_AIC_ERROR_ccu_loop_err_START (6)
#define SOC_NPU_AICORE_AIC_ERROR_ccu_loop_err_END (6)
#define SOC_NPU_AICORE_AIC_ERROR_ccu_neg_sqrt_START (7)
#define SOC_NPU_AICORE_AIC_ERROR_ccu_neg_sqrt_END (7)
#define SOC_NPU_AICORE_AIC_ERROR_ccu_ub_ecc_START (8)
#define SOC_NPU_AICORE_AIC_ERROR_ccu_ub_ecc_END (8)
#define SOC_NPU_AICORE_AIC_ERROR_cube_invld_input_START (9)
#define SOC_NPU_AICORE_AIC_ERROR_cube_invld_input_END (9)
#define SOC_NPU_AICORE_AIC_ERROR_cube_l0a_ecc_START (10)
#define SOC_NPU_AICORE_AIC_ERROR_cube_l0a_ecc_END (10)
#define SOC_NPU_AICORE_AIC_ERROR_cube_l0a_rdwr_cflt_START (11)
#define SOC_NPU_AICORE_AIC_ERROR_cube_l0a_rdwr_cflt_END (11)
#define SOC_NPU_AICORE_AIC_ERROR_cube_l0a_wrap_around_START (12)
#define SOC_NPU_AICORE_AIC_ERROR_cube_l0a_wrap_around_END (12)
#define SOC_NPU_AICORE_AIC_ERROR_cube_l0b_ecc_START (13)
#define SOC_NPU_AICORE_AIC_ERROR_cube_l0b_ecc_END (13)
#define SOC_NPU_AICORE_AIC_ERROR_cube_l0b_rdwr_cflt_START (14)
#define SOC_NPU_AICORE_AIC_ERROR_cube_l0b_rdwr_cflt_END (14)
#define SOC_NPU_AICORE_AIC_ERROR_cube_l0b_wrap_around_START (15)
#define SOC_NPU_AICORE_AIC_ERROR_cube_l0b_wrap_around_END (15)
#define SOC_NPU_AICORE_AIC_ERROR_cube_l0c_ecc_START (16)
#define SOC_NPU_AICORE_AIC_ERROR_cube_l0c_ecc_END (16)
#define SOC_NPU_AICORE_AIC_ERROR_cube_l0c_rdwr_cflt_START (17)
#define SOC_NPU_AICORE_AIC_ERROR_cube_l0c_rdwr_cflt_END (17)
#define SOC_NPU_AICORE_AIC_ERROR_cube_l0c_self_rdwr_cflt_START (18)
#define SOC_NPU_AICORE_AIC_ERROR_cube_l0c_self_rdwr_cflt_END (18)
#define SOC_NPU_AICORE_AIC_ERROR_cube_l0c_wrap_around_START (19)
#define SOC_NPU_AICORE_AIC_ERROR_cube_l0c_wrap_around_END (19)
#define SOC_NPU_AICORE_AIC_ERROR_ifu_bus_err_START (20)
#define SOC_NPU_AICORE_AIC_ERROR_ifu_bus_err_END (20)
#define SOC_NPU_AICORE_AIC_ERROR_mte_aipp_illegal_param_START (21)
#define SOC_NPU_AICORE_AIC_ERROR_mte_aipp_illegal_param_END (21)
#define SOC_NPU_AICORE_AIC_ERROR_mte_bas_raddr_obound_START (22)
#define SOC_NPU_AICORE_AIC_ERROR_mte_bas_raddr_obound_END (22)
#define SOC_NPU_AICORE_AIC_ERROR_mte_biu_rdwr_resp_START (23)
#define SOC_NPU_AICORE_AIC_ERROR_mte_biu_rdwr_resp_END (23)
#define SOC_NPU_AICORE_AIC_ERROR_mte_cidx_overflow_START (24)
#define SOC_NPU_AICORE_AIC_ERROR_mte_cidx_overflow_END (24)
#define SOC_NPU_AICORE_AIC_ERROR_mte_decomp_START (25)
#define SOC_NPU_AICORE_AIC_ERROR_mte_decomp_END (25)
#define SOC_NPU_AICORE_AIC_ERROR_mte_f1wpos_larger_fsize_START (26)
#define SOC_NPU_AICORE_AIC_ERROR_mte_f1wpos_larger_fsize_END (26)
#define SOC_NPU_AICORE_AIC_ERROR_mte_fmap_less_kernel_START (27)
#define SOC_NPU_AICORE_AIC_ERROR_mte_fmap_less_kernel_END (27)
#define SOC_NPU_AICORE_AIC_ERROR_mte_fmapwh_larger_l1size_START (28)
#define SOC_NPU_AICORE_AIC_ERROR_mte_fmapwh_larger_l1size_END (28)
#define SOC_NPU_AICORE_AIC_ERROR_mte_fpos_larger_fsize_START (29)
#define SOC_NPU_AICORE_AIC_ERROR_mte_fpos_larger_fsize_END (29)
#define SOC_NPU_AICORE_AIC_ERROR_mte_gdma_illegal_burst_len_START (30)
#define SOC_NPU_AICORE_AIC_ERROR_mte_gdma_illegal_burst_len_END (30)
#define SOC_NPU_AICORE_AIC_ERROR_mte_gdma_illegal_burst_num_START (31)
#define SOC_NPU_AICORE_AIC_ERROR_mte_gdma_illegal_burst_num_END (31)
#define SOC_NPU_AICORE_AIC_ERROR_mte_gdma_read_overflow_START (32)
#define SOC_NPU_AICORE_AIC_ERROR_mte_gdma_read_overflow_END (32)
#define SOC_NPU_AICORE_AIC_ERROR_mte_gdma_write_overflow_START (33)
#define SOC_NPU_AICORE_AIC_ERROR_mte_gdma_write_overflow_END (33)
#define SOC_NPU_AICORE_AIC_ERROR_mte_comp_START (34)
#define SOC_NPU_AICORE_AIC_ERROR_mte_comp_END (34)
#define SOC_NPU_AICORE_AIC_ERROR_mte_illegal_fm_size_START (35)
#define SOC_NPU_AICORE_AIC_ERROR_mte_illegal_fm_size_END (35)
#define SOC_NPU_AICORE_AIC_ERROR_mte_illegal_l1_3d_size_START (36)
#define SOC_NPU_AICORE_AIC_ERROR_mte_illegal_l1_3d_size_END (36)
#define SOC_NPU_AICORE_AIC_ERROR_mte_illegal_stride_START (37)
#define SOC_NPU_AICORE_AIC_ERROR_mte_illegal_stride_END (37)
#define SOC_NPU_AICORE_AIC_ERROR_mte_l0a_rdwr_cflt_START (38)
#define SOC_NPU_AICORE_AIC_ERROR_mte_l0a_rdwr_cflt_END (38)
#define SOC_NPU_AICORE_AIC_ERROR_mte_l0b_rdwr_cflt_START (39)
#define SOC_NPU_AICORE_AIC_ERROR_mte_l0b_rdwr_cflt_END (39)
#define SOC_NPU_AICORE_AIC_ERROR_mte_l1_ecc_START (40)
#define SOC_NPU_AICORE_AIC_ERROR_mte_l1_ecc_END (40)
#define SOC_NPU_AICORE_AIC_ERROR_mte_padding_cfg_START (41)
#define SOC_NPU_AICORE_AIC_ERROR_mte_padding_cfg_END (41)
#define SOC_NPU_AICORE_AIC_ERROR_mte_read_overflow_START (42)
#define SOC_NPU_AICORE_AIC_ERROR_mte_read_overflow_END (42)
#define SOC_NPU_AICORE_AIC_ERROR_mte_rob_ecc_START (43)
#define SOC_NPU_AICORE_AIC_ERROR_mte_rob_ecc_END (43)
#define SOC_NPU_AICORE_AIC_ERROR_mte_tlu_ecc_START (44)
#define SOC_NPU_AICORE_AIC_ERROR_mte_tlu_ecc_END (44)
#define SOC_NPU_AICORE_AIC_ERROR_mte_ub_ecc_START (45)
#define SOC_NPU_AICORE_AIC_ERROR_mte_ub_ecc_END (45)
#define SOC_NPU_AICORE_AIC_ERROR_mte_unzip_START (46)
#define SOC_NPU_AICORE_AIC_ERROR_mte_unzip_END (46)
#define SOC_NPU_AICORE_AIC_ERROR_mte_write_3d_overflow_START (47)
#define SOC_NPU_AICORE_AIC_ERROR_mte_write_3d_overflow_END (47)
#define SOC_NPU_AICORE_AIC_ERROR_mte_write_overflow_START (48)
#define SOC_NPU_AICORE_AIC_ERROR_mte_write_overflow_END (48)
#define SOC_NPU_AICORE_AIC_ERROR_vec_data_excp_ccu_START (49)
#define SOC_NPU_AICORE_AIC_ERROR_vec_data_excp_ccu_END (49)
#define SOC_NPU_AICORE_AIC_ERROR_vec_data_excp_mte_START (50)
#define SOC_NPU_AICORE_AIC_ERROR_vec_data_excp_mte_END (50)
#define SOC_NPU_AICORE_AIC_ERROR_vec_data_excp_vec_START (51)
#define SOC_NPU_AICORE_AIC_ERROR_vec_data_excp_vec_END (51)
#define SOC_NPU_AICORE_AIC_ERROR_vec_div0_START (52)
#define SOC_NPU_AICORE_AIC_ERROR_vec_div0_END (52)
#define SOC_NPU_AICORE_AIC_ERROR_vec_illegal_mask_START (53)
#define SOC_NPU_AICORE_AIC_ERROR_vec_illegal_mask_END (53)
#define SOC_NPU_AICORE_AIC_ERROR_vec_inf_nan_START (54)
#define SOC_NPU_AICORE_AIC_ERROR_vec_inf_nan_END (54)
#define SOC_NPU_AICORE_AIC_ERROR_vec_l0c_ecc_START (55)
#define SOC_NPU_AICORE_AIC_ERROR_vec_l0c_ecc_END (55)
#define SOC_NPU_AICORE_AIC_ERROR_vec_l0c_rdwr_cflt_START (56)
#define SOC_NPU_AICORE_AIC_ERROR_vec_l0c_rdwr_cflt_END (56)
#define SOC_NPU_AICORE_AIC_ERROR_vec_neg_ln_START (57)
#define SOC_NPU_AICORE_AIC_ERROR_vec_neg_ln_END (57)
#define SOC_NPU_AICORE_AIC_ERROR_vec_neg_sqrt_START (58)
#define SOC_NPU_AICORE_AIC_ERROR_vec_neg_sqrt_END (58)
#define SOC_NPU_AICORE_AIC_ERROR_vec_same_blk_addr_START (59)
#define SOC_NPU_AICORE_AIC_ERROR_vec_same_blk_addr_END (59)
#define SOC_NPU_AICORE_AIC_ERROR_vec_ub_ecc_START (60)
#define SOC_NPU_AICORE_AIC_ERROR_vec_ub_ecc_END (60)
#define SOC_NPU_AICORE_AIC_ERROR_vec_ub_self_rdwr_cflt_START (61)
#define SOC_NPU_AICORE_AIC_ERROR_vec_ub_self_rdwr_cflt_END (61)
#define SOC_NPU_AICORE_AIC_ERROR_vec_ub_wrap_around_START (62)
#define SOC_NPU_AICORE_AIC_ERROR_vec_ub_wrap_around_END (62)
#define SOC_NPU_AICORE_AIC_ERROR_biu_dfx_err_START (63)
#define SOC_NPU_AICORE_AIC_ERROR_biu_dfx_err_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long biu_l2_read_oob_mask : 1;
        unsigned long long biu_l2_write_oob_mask : 1;
        unsigned long long ccu_call_depth_ovrflw_mask : 1;
        unsigned long long ccu_div0_mask : 1;
        unsigned long long ccu_illegal_instr_mask : 1;
        unsigned long long ccu_loop_cnt_err_mask : 1;
        unsigned long long ccu_loop_err_mask : 1;
        unsigned long long ccu_neg_sqrt_mask : 1;
        unsigned long long ccu_ub_ecc_mask : 1;
        unsigned long long cube_invld_input_mask : 1;
        unsigned long long cube_l0a_ecc_mask : 1;
        unsigned long long cube_l0a_rdwr_cflt_mask : 1;
        unsigned long long cube_l0a_wrap_around_mask : 1;
        unsigned long long cube_l0b_ecc_mask : 1;
        unsigned long long cube_l0b_rdwr_cflt_mask : 1;
        unsigned long long cube_l0b_wrap_around_mask : 1;
        unsigned long long cube_l0c_ecc_mask : 1;
        unsigned long long cube_l0c_rdwr_cflt_mask : 1;
        unsigned long long cube_l0c_self_rdwr_cflt_mask : 1;
        unsigned long long cube_l0c_wrap_around_mask : 1;
        unsigned long long ifu_bus_err_mask : 1;
        unsigned long long mte_aipp_illegal_param_mask : 1;
        unsigned long long mte_bas_raddr_obound_mask : 1;
        unsigned long long mte_biu_rdwr_resp_mask : 1;
        unsigned long long mte_cidx_overflow_mask : 1;
        unsigned long long mte_decomp_mask : 1;
        unsigned long long mte_f1wpos_larger_fsize_mask : 1;
        unsigned long long mte_fmap_less_kernel_mask : 1;
        unsigned long long mte_fmapwh_larger_l1size_mask : 1;
        unsigned long long mte_fpos_larger_fsize_mask : 1;
        unsigned long long mte_gdma_illegal_burst_len_mask : 1;
        unsigned long long mte_gdma_illegal_burst_num_mask : 1;
        unsigned long long mte_gdma_read_overflow_mask : 1;
        unsigned long long mte_gdma_write_overflow_mask : 1;
        unsigned long long mte_comp_mask : 1;
        unsigned long long mte_illegal_fm_size_mask : 1;
        unsigned long long mte_illegal_l1_3d_size_mask : 1;
        unsigned long long mte_illegal_stride_mask : 1;
        unsigned long long mte_l0a_rdwr_cflt_mask : 1;
        unsigned long long mte_l0b_rdwr_cflt_mask : 1;
        unsigned long long mte_l1_ecc_mask : 1;
        unsigned long long mte_padding_cfg_mask : 1;
        unsigned long long mte_read_overflow_mask : 1;
        unsigned long long mte_rob_ecc_mask : 1;
        unsigned long long mte_tlu_ecc_mask : 1;
        unsigned long long mte_ub_ecc_mask : 1;
        unsigned long long mte_unzip_mask : 1;
        unsigned long long mte_write_3d_overflow_mask : 1;
        unsigned long long mte_write_overflow_mask : 1;
        unsigned long long vec_data_excp_ccu_mask : 1;
        unsigned long long vec_data_excp_mte_mask : 1;
        unsigned long long vec_data_excp_vec_mask : 1;
        unsigned long long vec_div0_mask : 1;
        unsigned long long vec_illegal_mask_mask : 1;
        unsigned long long vec_inf_nan_mask : 1;
        unsigned long long vec_l0c_ecc_mask : 1;
        unsigned long long vec_l0c_rdwr_cflt_mask : 1;
        unsigned long long vec_neg_ln_mask : 1;
        unsigned long long vec_neg_sqrt_mask : 1;
        unsigned long long vec_same_blk_addr_mask : 1;
        unsigned long long vec_ub_ecc_mask : 1;
        unsigned long long vec_ub_self_rdwr_cflt_mask : 1;
        unsigned long long vec_ub_wrap_around_mask : 1;
        unsigned long long biu_dfx_err_mask : 1;
    } reg;
} SOC_NPU_AICORE_AIC_ERROR_MASK_UNION;
#endif
#define SOC_NPU_AICORE_AIC_ERROR_MASK_biu_l2_read_oob_mask_START (0)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_biu_l2_read_oob_mask_END (0)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_biu_l2_write_oob_mask_START (1)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_biu_l2_write_oob_mask_END (1)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_ccu_call_depth_ovrflw_mask_START (2)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_ccu_call_depth_ovrflw_mask_END (2)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_ccu_div0_mask_START (3)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_ccu_div0_mask_END (3)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_ccu_illegal_instr_mask_START (4)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_ccu_illegal_instr_mask_END (4)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_ccu_loop_cnt_err_mask_START (5)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_ccu_loop_cnt_err_mask_END (5)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_ccu_loop_err_mask_START (6)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_ccu_loop_err_mask_END (6)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_ccu_neg_sqrt_mask_START (7)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_ccu_neg_sqrt_mask_END (7)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_ccu_ub_ecc_mask_START (8)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_ccu_ub_ecc_mask_END (8)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_cube_invld_input_mask_START (9)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_cube_invld_input_mask_END (9)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_cube_l0a_ecc_mask_START (10)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_cube_l0a_ecc_mask_END (10)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_cube_l0a_rdwr_cflt_mask_START (11)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_cube_l0a_rdwr_cflt_mask_END (11)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_cube_l0a_wrap_around_mask_START (12)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_cube_l0a_wrap_around_mask_END (12)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_cube_l0b_ecc_mask_START (13)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_cube_l0b_ecc_mask_END (13)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_cube_l0b_rdwr_cflt_mask_START (14)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_cube_l0b_rdwr_cflt_mask_END (14)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_cube_l0b_wrap_around_mask_START (15)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_cube_l0b_wrap_around_mask_END (15)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_cube_l0c_ecc_mask_START (16)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_cube_l0c_ecc_mask_END (16)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_cube_l0c_rdwr_cflt_mask_START (17)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_cube_l0c_rdwr_cflt_mask_END (17)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_cube_l0c_self_rdwr_cflt_mask_START (18)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_cube_l0c_self_rdwr_cflt_mask_END (18)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_cube_l0c_wrap_around_mask_START (19)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_cube_l0c_wrap_around_mask_END (19)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_ifu_bus_err_mask_START (20)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_ifu_bus_err_mask_END (20)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_aipp_illegal_param_mask_START (21)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_aipp_illegal_param_mask_END (21)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_bas_raddr_obound_mask_START (22)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_bas_raddr_obound_mask_END (22)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_biu_rdwr_resp_mask_START (23)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_biu_rdwr_resp_mask_END (23)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_cidx_overflow_mask_START (24)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_cidx_overflow_mask_END (24)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_decomp_mask_START (25)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_decomp_mask_END (25)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_f1wpos_larger_fsize_mask_START (26)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_f1wpos_larger_fsize_mask_END (26)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_fmap_less_kernel_mask_START (27)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_fmap_less_kernel_mask_END (27)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_fmapwh_larger_l1size_mask_START (28)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_fmapwh_larger_l1size_mask_END (28)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_fpos_larger_fsize_mask_START (29)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_fpos_larger_fsize_mask_END (29)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_gdma_illegal_burst_len_mask_START (30)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_gdma_illegal_burst_len_mask_END (30)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_gdma_illegal_burst_num_mask_START (31)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_gdma_illegal_burst_num_mask_END (31)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_gdma_read_overflow_mask_START (32)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_gdma_read_overflow_mask_END (32)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_gdma_write_overflow_mask_START (33)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_gdma_write_overflow_mask_END (33)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_comp_mask_START (34)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_comp_mask_END (34)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_illegal_fm_size_mask_START (35)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_illegal_fm_size_mask_END (35)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_illegal_l1_3d_size_mask_START (36)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_illegal_l1_3d_size_mask_END (36)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_illegal_stride_mask_START (37)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_illegal_stride_mask_END (37)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_l0a_rdwr_cflt_mask_START (38)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_l0a_rdwr_cflt_mask_END (38)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_l0b_rdwr_cflt_mask_START (39)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_l0b_rdwr_cflt_mask_END (39)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_l1_ecc_mask_START (40)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_l1_ecc_mask_END (40)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_padding_cfg_mask_START (41)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_padding_cfg_mask_END (41)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_read_overflow_mask_START (42)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_read_overflow_mask_END (42)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_rob_ecc_mask_START (43)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_rob_ecc_mask_END (43)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_tlu_ecc_mask_START (44)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_tlu_ecc_mask_END (44)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_ub_ecc_mask_START (45)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_ub_ecc_mask_END (45)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_unzip_mask_START (46)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_unzip_mask_END (46)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_write_3d_overflow_mask_START (47)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_write_3d_overflow_mask_END (47)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_write_overflow_mask_START (48)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_mte_write_overflow_mask_END (48)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_vec_data_excp_ccu_mask_START (49)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_vec_data_excp_ccu_mask_END (49)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_vec_data_excp_mte_mask_START (50)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_vec_data_excp_mte_mask_END (50)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_vec_data_excp_vec_mask_START (51)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_vec_data_excp_vec_mask_END (51)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_vec_div0_mask_START (52)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_vec_div0_mask_END (52)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_vec_illegal_mask_mask_START (53)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_vec_illegal_mask_mask_END (53)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_vec_inf_nan_mask_START (54)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_vec_inf_nan_mask_END (54)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_vec_l0c_ecc_mask_START (55)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_vec_l0c_ecc_mask_END (55)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_vec_l0c_rdwr_cflt_mask_START (56)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_vec_l0c_rdwr_cflt_mask_END (56)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_vec_neg_ln_mask_START (57)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_vec_neg_ln_mask_END (57)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_vec_neg_sqrt_mask_START (58)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_vec_neg_sqrt_mask_END (58)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_vec_same_blk_addr_mask_START (59)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_vec_same_blk_addr_mask_END (59)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_vec_ub_ecc_mask_START (60)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_vec_ub_ecc_mask_END (60)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_vec_ub_self_rdwr_cflt_mask_START (61)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_vec_ub_self_rdwr_cflt_mask_END (61)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_vec_ub_wrap_around_mask_START (62)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_vec_ub_wrap_around_mask_END (62)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_biu_dfx_err_mask_START (63)
#define SOC_NPU_AICORE_AIC_ERROR_MASK_biu_dfx_err_mask_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long biu_err_addr : 64;
    } reg;
} SOC_NPU_AICORE_BIU_ERR_INFO_UNION;
#endif
#define SOC_NPU_AICORE_BIU_ERR_INFO_biu_err_addr_START (0)
#define SOC_NPU_AICORE_BIU_ERR_INFO_biu_err_addr_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long ccu_err_pc : 8;
        unsigned long long ccu_err_addr : 15;
        unsigned long long reserved : 9;
        unsigned long long ccu_err_instr : 32;
    } reg;
} SOC_NPU_AICORE_CCU_ERR_INFO_UNION;
#endif
#define SOC_NPU_AICORE_CCU_ERR_INFO_ccu_err_pc_START (0)
#define SOC_NPU_AICORE_CCU_ERR_INFO_ccu_err_pc_END (7)
#define SOC_NPU_AICORE_CCU_ERR_INFO_ccu_err_addr_START (8)
#define SOC_NPU_AICORE_CCU_ERR_INFO_ccu_err_addr_END (22)
#define SOC_NPU_AICORE_CCU_ERR_INFO_ccu_err_instr_START (32)
#define SOC_NPU_AICORE_CCU_ERR_INFO_ccu_err_instr_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long cube_err_pc : 8;
        unsigned long long cube_err_addr : 9;
        unsigned long long reserved : 47;
    } reg;
} SOC_NPU_AICORE_CUBE_ERR_INFO_UNION;
#endif
#define SOC_NPU_AICORE_CUBE_ERR_INFO_cube_err_pc_START (0)
#define SOC_NPU_AICORE_CUBE_ERR_INFO_cube_err_pc_END (7)
#define SOC_NPU_AICORE_CUBE_ERR_INFO_cube_err_addr_START (8)
#define SOC_NPU_AICORE_CUBE_ERR_INFO_cube_err_addr_END (16)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long reserved_0 : 2;
        unsigned long long ifu_err_addr : 46;
        unsigned long long ifu_err_type : 3;
        unsigned long long reserved_1 : 13;
    } reg;
} SOC_NPU_AICORE_IFU_ERR_INFO_UNION;
#endif
#define SOC_NPU_AICORE_IFU_ERR_INFO_ifu_err_addr_START (2)
#define SOC_NPU_AICORE_IFU_ERR_INFO_ifu_err_addr_END (47)
#define SOC_NPU_AICORE_IFU_ERR_INFO_ifu_err_type_START (48)
#define SOC_NPU_AICORE_IFU_ERR_INFO_ifu_err_type_END (50)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long mte_err_pc : 8;
        unsigned long long mte_err_addr : 15;
        unsigned long long reserved_0 : 1;
        unsigned long long mte_err_type : 4;
        unsigned long long reserved_1 : 36;
    } reg;
} SOC_NPU_AICORE_MTE_ERR_INFO_UNION;
#endif
#define SOC_NPU_AICORE_MTE_ERR_INFO_mte_err_pc_START (0)
#define SOC_NPU_AICORE_MTE_ERR_INFO_mte_err_pc_END (7)
#define SOC_NPU_AICORE_MTE_ERR_INFO_mte_err_addr_START (8)
#define SOC_NPU_AICORE_MTE_ERR_INFO_mte_err_addr_END (22)
#define SOC_NPU_AICORE_MTE_ERR_INFO_mte_err_type_START (24)
#define SOC_NPU_AICORE_MTE_ERR_INFO_mte_err_type_END (27)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long vec_err_pc : 8;
        unsigned long long vec_err_rcnt : 8;
        unsigned long long vec_err_addr : 13;
        unsigned long long reserved : 35;
    } reg;
} SOC_NPU_AICORE_VEC_ERR_INFO_UNION;
#endif
#define SOC_NPU_AICORE_VEC_ERR_INFO_vec_err_pc_START (0)
#define SOC_NPU_AICORE_VEC_ERR_INFO_vec_err_pc_END (7)
#define SOC_NPU_AICORE_VEC_ERR_INFO_vec_err_rcnt_START (8)
#define SOC_NPU_AICORE_VEC_ERR_INFO_vec_err_rcnt_END (15)
#define SOC_NPU_AICORE_VEC_ERR_INFO_vec_err_addr_START (16)
#define SOC_NPU_AICORE_VEC_ERR_INFO_vec_err_addr_END (28)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long mte_warn_l1_ecc_1bit_addr : 15;
        unsigned long long reserved_0 : 1;
        unsigned long long mte_warn_rob_ecc_1bit_addr : 11;
        unsigned long long reserved_1 : 5;
        unsigned long long mte_warn_tlu_ecc_1bit_addr : 7;
        unsigned long long ccu_warn_sbuf_ecc_1bit_addr : 11;
        unsigned long long reserved_2 : 14;
    } reg;
} SOC_NPU_AICORE_MTE_ECC_1BIT_ERR_UNION;
#endif
#define SOC_NPU_AICORE_MTE_ECC_1BIT_ERR_mte_warn_l1_ecc_1bit_addr_START (0)
#define SOC_NPU_AICORE_MTE_ECC_1BIT_ERR_mte_warn_l1_ecc_1bit_addr_END (14)
#define SOC_NPU_AICORE_MTE_ECC_1BIT_ERR_mte_warn_rob_ecc_1bit_addr_START (16)
#define SOC_NPU_AICORE_MTE_ECC_1BIT_ERR_mte_warn_rob_ecc_1bit_addr_END (26)
#define SOC_NPU_AICORE_MTE_ECC_1BIT_ERR_mte_warn_tlu_ecc_1bit_addr_START (32)
#define SOC_NPU_AICORE_MTE_ECC_1BIT_ERR_mte_warn_tlu_ecc_1bit_addr_END (38)
#define SOC_NPU_AICORE_MTE_ECC_1BIT_ERR_ccu_warn_sbuf_ecc_1bit_addr_START (39)
#define SOC_NPU_AICORE_MTE_ECC_1BIT_ERR_ccu_warn_sbuf_ecc_1bit_addr_END (49)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long vec_warn_ub_ecc_1bit_addr : 13;
        unsigned long long reserved_0 : 3;
        unsigned long long vec_warn_l0c_ecc_1bit_addr : 10;
        unsigned long long reserved_1 : 6;
        unsigned long long cube_warn_l0a_ecc_1bit_addr : 7;
        unsigned long long cube_warn_l0b_ecc_1bit_addr : 7;
        unsigned long long reserved_2 : 2;
        unsigned long long cube_warn_l0c_ecc_1bit_addr : 9;
        unsigned long long reserved_3 : 7;
    } reg;
} SOC_NPU_AICORE_VEC_CUBE_ECC_1BIT_ERR_UNION;
#endif
#define SOC_NPU_AICORE_VEC_CUBE_ECC_1BIT_ERR_vec_warn_ub_ecc_1bit_addr_START (0)
#define SOC_NPU_AICORE_VEC_CUBE_ECC_1BIT_ERR_vec_warn_ub_ecc_1bit_addr_END (12)
#define SOC_NPU_AICORE_VEC_CUBE_ECC_1BIT_ERR_vec_warn_l0c_ecc_1bit_addr_START (16)
#define SOC_NPU_AICORE_VEC_CUBE_ECC_1BIT_ERR_vec_warn_l0c_ecc_1bit_addr_END (25)
#define SOC_NPU_AICORE_VEC_CUBE_ECC_1BIT_ERR_cube_warn_l0a_ecc_1bit_addr_START (32)
#define SOC_NPU_AICORE_VEC_CUBE_ECC_1BIT_ERR_cube_warn_l0a_ecc_1bit_addr_END (38)
#define SOC_NPU_AICORE_VEC_CUBE_ECC_1BIT_ERR_cube_warn_l0b_ecc_1bit_addr_START (39)
#define SOC_NPU_AICORE_VEC_CUBE_ECC_1BIT_ERR_cube_warn_l0b_ecc_1bit_addr_END (45)
#define SOC_NPU_AICORE_VEC_CUBE_ECC_1BIT_ERR_cube_warn_l0c_ecc_1bit_addr_START (48)
#define SOC_NPU_AICORE_VEC_CUBE_ECC_1BIT_ERR_cube_warn_l0c_ecc_1bit_addr_END (56)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long reserved_0 : 1;
        unsigned long long reserved_1 : 1;
        unsigned long long reserved_2 : 1;
        unsigned long long reserved_3 : 1;
        unsigned long long reserved_4 : 1;
        unsigned long long reserved_5 : 1;
        unsigned long long reserved_6 : 1;
        unsigned long long reserved_7 : 1;
        unsigned long long ccu_ub_ecc_force : 1;
        unsigned long long reserved_8 : 1;
        unsigned long long cube_l0a_ecc_force : 1;
        unsigned long long reserved_9 : 1;
        unsigned long long reserved_10 : 1;
        unsigned long long cube_l0b_ecc_force : 1;
        unsigned long long reserved_11 : 1;
        unsigned long long reserved_12 : 1;
        unsigned long long cube_l0c_ecc_force : 1;
        unsigned long long reserved_13 : 1;
        unsigned long long reserved_14 : 1;
        unsigned long long reserved_15 : 1;
        unsigned long long reserved_16 : 1;
        unsigned long long reserved_17 : 1;
        unsigned long long reserved_18 : 1;
        unsigned long long reserved_19 : 1;
        unsigned long long reserved_20 : 1;
        unsigned long long reserved_21 : 1;
        unsigned long long reserved_22 : 1;
        unsigned long long reserved_23 : 1;
        unsigned long long reserved_24 : 1;
        unsigned long long reserved_25 : 1;
        unsigned long long reserved_26 : 1;
        unsigned long long reserved_27 : 1;
        unsigned long long reserved_28 : 1;
        unsigned long long reserved_29 : 1;
        unsigned long long reserved_30 : 1;
        unsigned long long reserved_31 : 1;
        unsigned long long reserved_32 : 1;
        unsigned long long reserved_33 : 1;
        unsigned long long reserved_34 : 1;
        unsigned long long mte_l1_ecc_force : 1;
        unsigned long long reserved_35 : 1;
        unsigned long long reserved_36 : 1;
        unsigned long long mte_rob_ecc_force : 1;
        unsigned long long mte_tlu_ecc_force : 1;
        unsigned long long mte_ub_ecc_force : 1;
        unsigned long long reserved_37 : 1;
        unsigned long long reserved_38 : 1;
        unsigned long long reserved_39 : 1;
        unsigned long long reserved_40 : 1;
        unsigned long long reserved_41 : 1;
        unsigned long long reserved_42 : 1;
        unsigned long long reserved_43 : 1;
        unsigned long long reserved_44 : 1;
        unsigned long long vec_l0c_ecc_force : 1;
        unsigned long long reserved_45 : 1;
        unsigned long long reserved_46 : 1;
        unsigned long long reserved_47 : 1;
        unsigned long long reserved_48 : 1;
        unsigned long long vec_ub_ecc_force : 1;
        unsigned long long reserved_49 : 1;
        unsigned long long reserved_50 : 4;
    } reg;
} SOC_NPU_AICORE_AIC_ERROR_FORCE_UNION;
#endif
#define SOC_NPU_AICORE_AIC_ERROR_FORCE_ccu_ub_ecc_force_START (8)
#define SOC_NPU_AICORE_AIC_ERROR_FORCE_ccu_ub_ecc_force_END (8)
#define SOC_NPU_AICORE_AIC_ERROR_FORCE_cube_l0a_ecc_force_START (10)
#define SOC_NPU_AICORE_AIC_ERROR_FORCE_cube_l0a_ecc_force_END (10)
#define SOC_NPU_AICORE_AIC_ERROR_FORCE_cube_l0b_ecc_force_START (13)
#define SOC_NPU_AICORE_AIC_ERROR_FORCE_cube_l0b_ecc_force_END (13)
#define SOC_NPU_AICORE_AIC_ERROR_FORCE_cube_l0c_ecc_force_START (16)
#define SOC_NPU_AICORE_AIC_ERROR_FORCE_cube_l0c_ecc_force_END (16)
#define SOC_NPU_AICORE_AIC_ERROR_FORCE_mte_l1_ecc_force_START (39)
#define SOC_NPU_AICORE_AIC_ERROR_FORCE_mte_l1_ecc_force_END (39)
#define SOC_NPU_AICORE_AIC_ERROR_FORCE_mte_rob_ecc_force_START (42)
#define SOC_NPU_AICORE_AIC_ERROR_FORCE_mte_rob_ecc_force_END (42)
#define SOC_NPU_AICORE_AIC_ERROR_FORCE_mte_tlu_ecc_force_START (43)
#define SOC_NPU_AICORE_AIC_ERROR_FORCE_mte_tlu_ecc_force_END (43)
#define SOC_NPU_AICORE_AIC_ERROR_FORCE_mte_ub_ecc_force_START (44)
#define SOC_NPU_AICORE_AIC_ERROR_FORCE_mte_ub_ecc_force_END (44)
#define SOC_NPU_AICORE_AIC_ERROR_FORCE_vec_l0c_ecc_force_START (53)
#define SOC_NPU_AICORE_AIC_ERROR_FORCE_vec_l0c_ecc_force_END (53)
#define SOC_NPU_AICORE_AIC_ERROR_FORCE_vec_ub_ecc_force_START (58)
#define SOC_NPU_AICORE_AIC_ERROR_FORCE_vec_ub_ecc_force_END (58)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long ccu_sbuf_ecc_force : 1;
        unsigned long long reserved : 63;
    } reg;
} SOC_NPU_AICORE_AIC_ERROR_FORCE2_UNION;
#endif
#define SOC_NPU_AICORE_AIC_ERROR_FORCE2_ccu_sbuf_ecc_force_START (0)
#define SOC_NPU_AICORE_AIC_ERROR_FORCE2_ccu_sbuf_ecc_force_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long ccu_sbuf_ecc : 1;
        unsigned long long vec_col2img_rd_fm_addr_ovflow : 1;
        unsigned long long vec_col2img_rd_dfm_addr_ovfflow : 1;
        unsigned long long vec_col2img_illegal_fm_size : 1;
        unsigned long long vec_col2img_illegal_stride : 1;
        unsigned long long vec_col2img_illegal_1st_win_pos : 1;
        unsigned long long vec_col2img_illegal_fetch_pos : 1;
        unsigned long long vec_col2img_illegal_k_size : 1;
        unsigned long long ccu_inf_nan : 1;
        unsigned long long mte_illegal_schn_cfg : 1;
        unsigned long long mte_atm_addr_misalg : 1;
        unsigned long long vec_instr_addr_misalign : 1;
        unsigned long long vec_instr_illegal_cfg : 1;
        unsigned long long vec_instr_undef : 1;
        unsigned long long ccu_addr_err : 1;
        unsigned long long ccu_bus_err : 1;
        unsigned long long mte_err_addr_misalign : 1;
        unsigned long long mte_err_dw_pad_conf_err : 1;
        unsigned long long mte_err_dw_fmap_h_illegal : 1;
        unsigned long long mte_err_wino_l0b_write_overflow : 1;
        unsigned long long mte_err_wino_l0b_read_overflow : 1;
        unsigned long long mte_err_illegal_v_cov_pad_ctl : 1;
        unsigned long long mte_err_illegal_h_cov_pad_ctl : 1;
        unsigned long long mte_err_illegal_w_size : 1;
        unsigned long long mte_err_illegal_h_size : 1;
        unsigned long long mte_err_illegal_chn_size : 1;
        unsigned long long mte_err_illegal_k_m_ext_step : 1;
        unsigned long long mte_err_illegal_k_m_start_pos : 1;
        unsigned long long mte_err_illegal_smallk_cfg : 1;
        unsigned long long mte_err_read_3d_overflow : 1;
        unsigned long long reserved : 34;
    } reg;
} SOC_NPU_AICORE_AIC_ERROR2_UNION;
#endif
#define SOC_NPU_AICORE_AIC_ERROR2_ccu_sbuf_ecc_START (0)
#define SOC_NPU_AICORE_AIC_ERROR2_ccu_sbuf_ecc_END (0)
#define SOC_NPU_AICORE_AIC_ERROR2_vec_col2img_rd_fm_addr_ovflow_START (1)
#define SOC_NPU_AICORE_AIC_ERROR2_vec_col2img_rd_fm_addr_ovflow_END (1)
#define SOC_NPU_AICORE_AIC_ERROR2_vec_col2img_rd_dfm_addr_ovfflow_START (2)
#define SOC_NPU_AICORE_AIC_ERROR2_vec_col2img_rd_dfm_addr_ovfflow_END (2)
#define SOC_NPU_AICORE_AIC_ERROR2_vec_col2img_illegal_fm_size_START (3)
#define SOC_NPU_AICORE_AIC_ERROR2_vec_col2img_illegal_fm_size_END (3)
#define SOC_NPU_AICORE_AIC_ERROR2_vec_col2img_illegal_stride_START (4)
#define SOC_NPU_AICORE_AIC_ERROR2_vec_col2img_illegal_stride_END (4)
#define SOC_NPU_AICORE_AIC_ERROR2_vec_col2img_illegal_1st_win_pos_START (5)
#define SOC_NPU_AICORE_AIC_ERROR2_vec_col2img_illegal_1st_win_pos_END (5)
#define SOC_NPU_AICORE_AIC_ERROR2_vec_col2img_illegal_fetch_pos_START (6)
#define SOC_NPU_AICORE_AIC_ERROR2_vec_col2img_illegal_fetch_pos_END (6)
#define SOC_NPU_AICORE_AIC_ERROR2_vec_col2img_illegal_k_size_START (7)
#define SOC_NPU_AICORE_AIC_ERROR2_vec_col2img_illegal_k_size_END (7)
#define SOC_NPU_AICORE_AIC_ERROR2_ccu_inf_nan_START (8)
#define SOC_NPU_AICORE_AIC_ERROR2_ccu_inf_nan_END (8)
#define SOC_NPU_AICORE_AIC_ERROR2_mte_illegal_schn_cfg_START (9)
#define SOC_NPU_AICORE_AIC_ERROR2_mte_illegal_schn_cfg_END (9)
#define SOC_NPU_AICORE_AIC_ERROR2_mte_atm_addr_misalg_START (10)
#define SOC_NPU_AICORE_AIC_ERROR2_mte_atm_addr_misalg_END (10)
#define SOC_NPU_AICORE_AIC_ERROR2_vec_instr_addr_misalign_START (11)
#define SOC_NPU_AICORE_AIC_ERROR2_vec_instr_addr_misalign_END (11)
#define SOC_NPU_AICORE_AIC_ERROR2_vec_instr_illegal_cfg_START (12)
#define SOC_NPU_AICORE_AIC_ERROR2_vec_instr_illegal_cfg_END (12)
#define SOC_NPU_AICORE_AIC_ERROR2_vec_instr_undef_START (13)
#define SOC_NPU_AICORE_AIC_ERROR2_vec_instr_undef_END (13)
#define SOC_NPU_AICORE_AIC_ERROR2_ccu_addr_err_START (14)
#define SOC_NPU_AICORE_AIC_ERROR2_ccu_addr_err_END (14)
#define SOC_NPU_AICORE_AIC_ERROR2_ccu_bus_err_START (15)
#define SOC_NPU_AICORE_AIC_ERROR2_ccu_bus_err_END (15)
#define SOC_NPU_AICORE_AIC_ERROR2_mte_err_addr_misalign_START (16)
#define SOC_NPU_AICORE_AIC_ERROR2_mte_err_addr_misalign_END (16)
#define SOC_NPU_AICORE_AIC_ERROR2_mte_err_dw_pad_conf_err_START (17)
#define SOC_NPU_AICORE_AIC_ERROR2_mte_err_dw_pad_conf_err_END (17)
#define SOC_NPU_AICORE_AIC_ERROR2_mte_err_dw_fmap_h_illegal_START (18)
#define SOC_NPU_AICORE_AIC_ERROR2_mte_err_dw_fmap_h_illegal_END (18)
#define SOC_NPU_AICORE_AIC_ERROR2_mte_err_wino_l0b_write_overflow_START (19)
#define SOC_NPU_AICORE_AIC_ERROR2_mte_err_wino_l0b_write_overflow_END (19)
#define SOC_NPU_AICORE_AIC_ERROR2_mte_err_wino_l0b_read_overflow_START (20)
#define SOC_NPU_AICORE_AIC_ERROR2_mte_err_wino_l0b_read_overflow_END (20)
#define SOC_NPU_AICORE_AIC_ERROR2_mte_err_illegal_v_cov_pad_ctl_START (21)
#define SOC_NPU_AICORE_AIC_ERROR2_mte_err_illegal_v_cov_pad_ctl_END (21)
#define SOC_NPU_AICORE_AIC_ERROR2_mte_err_illegal_h_cov_pad_ctl_START (22)
#define SOC_NPU_AICORE_AIC_ERROR2_mte_err_illegal_h_cov_pad_ctl_END (22)
#define SOC_NPU_AICORE_AIC_ERROR2_mte_err_illegal_w_size_START (23)
#define SOC_NPU_AICORE_AIC_ERROR2_mte_err_illegal_w_size_END (23)
#define SOC_NPU_AICORE_AIC_ERROR2_mte_err_illegal_h_size_START (24)
#define SOC_NPU_AICORE_AIC_ERROR2_mte_err_illegal_h_size_END (24)
#define SOC_NPU_AICORE_AIC_ERROR2_mte_err_illegal_chn_size_START (25)
#define SOC_NPU_AICORE_AIC_ERROR2_mte_err_illegal_chn_size_END (25)
#define SOC_NPU_AICORE_AIC_ERROR2_mte_err_illegal_k_m_ext_step_START (26)
#define SOC_NPU_AICORE_AIC_ERROR2_mte_err_illegal_k_m_ext_step_END (26)
#define SOC_NPU_AICORE_AIC_ERROR2_mte_err_illegal_k_m_start_pos_START (27)
#define SOC_NPU_AICORE_AIC_ERROR2_mte_err_illegal_k_m_start_pos_END (27)
#define SOC_NPU_AICORE_AIC_ERROR2_mte_err_illegal_smallk_cfg_START (28)
#define SOC_NPU_AICORE_AIC_ERROR2_mte_err_illegal_smallk_cfg_END (28)
#define SOC_NPU_AICORE_AIC_ERROR2_mte_err_read_3d_overflow_START (29)
#define SOC_NPU_AICORE_AIC_ERROR2_mte_err_read_3d_overflow_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long ccu_sbuf_ecc_mask : 1;
        unsigned long long vec_col2img_rd_fm_addr_ovflow_mask : 1;
        unsigned long long vec_col2img_rd_dfm_addr_ovfflow_mask : 1;
        unsigned long long vec_col2img_illegal_fm_size_mask : 1;
        unsigned long long vec_col2img_illegal_stride_mask : 1;
        unsigned long long vec_col2img_illegal_1st_win_pos_mask : 1;
        unsigned long long vec_col2img_illegal_fetch_pos_mask : 1;
        unsigned long long vec_col2img_illegal_k_size_mask : 1;
        unsigned long long ccu_inf_nan_mask : 1;
        unsigned long long mte_illegal_schn_cfg_mask : 1;
        unsigned long long mte_atm_addr_misalg_mask : 1;
        unsigned long long vec_instr_addr_misalign_mask : 1;
        unsigned long long vec_instr_illegal_cfg_mask : 1;
        unsigned long long vec_instr_undef_mask : 1;
        unsigned long long ccu_addr_err_mask : 1;
        unsigned long long ccu_bus_err_mask : 1;
        unsigned long long mte_err_addr_misalign_mask : 1;
        unsigned long long mte_err_dw_pad_conf_err_mask : 1;
        unsigned long long mte_err_dw_fmap_h_illegal_mask : 1;
        unsigned long long mte_err_wino_l0b_write_overflow_mask : 1;
        unsigned long long mte_err_wino_l0b_read_overflow_mask : 1;
        unsigned long long mte_err_illegal_v_cov_pad_ctl_mask : 1;
        unsigned long long mte_err_illegal_h_cov_pad_ctl_mask : 1;
        unsigned long long mte_err_illegal_w_size_mask : 1;
        unsigned long long mte_err_illegal_h_size_mask : 1;
        unsigned long long mte_err_illegal_chn_size_mask : 1;
        unsigned long long mte_err_illegal_k_m_ext_step_mask : 1;
        unsigned long long mte_err_illegal_k_m_start_pos_mask : 1;
        unsigned long long mte_err_illegal_smallk_cfg_mask : 1;
        unsigned long long mte_err_read_3d_overflow_mask : 1;
        unsigned long long reserved : 34;
    } reg;
} SOC_NPU_AICORE_AIC_ERROR_MASK2_UNION;
#endif
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_ccu_sbuf_ecc_mask_START (0)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_ccu_sbuf_ecc_mask_END (0)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_vec_col2img_rd_fm_addr_ovflow_mask_START (1)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_vec_col2img_rd_fm_addr_ovflow_mask_END (1)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_vec_col2img_rd_dfm_addr_ovfflow_mask_START (2)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_vec_col2img_rd_dfm_addr_ovfflow_mask_END (2)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_vec_col2img_illegal_fm_size_mask_START (3)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_vec_col2img_illegal_fm_size_mask_END (3)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_vec_col2img_illegal_stride_mask_START (4)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_vec_col2img_illegal_stride_mask_END (4)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_vec_col2img_illegal_1st_win_pos_mask_START (5)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_vec_col2img_illegal_1st_win_pos_mask_END (5)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_vec_col2img_illegal_fetch_pos_mask_START (6)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_vec_col2img_illegal_fetch_pos_mask_END (6)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_vec_col2img_illegal_k_size_mask_START (7)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_vec_col2img_illegal_k_size_mask_END (7)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_ccu_inf_nan_mask_START (8)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_ccu_inf_nan_mask_END (8)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_mte_illegal_schn_cfg_mask_START (9)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_mte_illegal_schn_cfg_mask_END (9)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_mte_atm_addr_misalg_mask_START (10)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_mte_atm_addr_misalg_mask_END (10)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_vec_instr_addr_misalign_mask_START (11)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_vec_instr_addr_misalign_mask_END (11)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_vec_instr_illegal_cfg_mask_START (12)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_vec_instr_illegal_cfg_mask_END (12)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_vec_instr_undef_mask_START (13)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_vec_instr_undef_mask_END (13)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_ccu_addr_err_mask_START (14)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_ccu_addr_err_mask_END (14)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_ccu_bus_err_mask_START (15)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_ccu_bus_err_mask_END (15)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_mte_err_addr_misalign_mask_START (16)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_mte_err_addr_misalign_mask_END (16)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_mte_err_dw_pad_conf_err_mask_START (17)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_mte_err_dw_pad_conf_err_mask_END (17)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_mte_err_dw_fmap_h_illegal_mask_START (18)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_mte_err_dw_fmap_h_illegal_mask_END (18)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_mte_err_wino_l0b_write_overflow_mask_START (19)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_mte_err_wino_l0b_write_overflow_mask_END (19)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_mte_err_wino_l0b_read_overflow_mask_START (20)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_mte_err_wino_l0b_read_overflow_mask_END (20)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_mte_err_illegal_v_cov_pad_ctl_mask_START (21)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_mte_err_illegal_v_cov_pad_ctl_mask_END (21)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_mte_err_illegal_h_cov_pad_ctl_mask_START (22)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_mte_err_illegal_h_cov_pad_ctl_mask_END (22)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_mte_err_illegal_w_size_mask_START (23)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_mte_err_illegal_w_size_mask_END (23)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_mte_err_illegal_h_size_mask_START (24)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_mte_err_illegal_h_size_mask_END (24)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_mte_err_illegal_chn_size_mask_START (25)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_mte_err_illegal_chn_size_mask_END (25)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_mte_err_illegal_k_m_ext_step_mask_START (26)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_mte_err_illegal_k_m_ext_step_mask_END (26)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_mte_err_illegal_k_m_start_pos_mask_START (27)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_mte_err_illegal_k_m_start_pos_mask_END (27)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_mte_err_illegal_smallk_cfg_mask_START (28)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_mte_err_illegal_smallk_cfg_mask_END (28)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_mte_err_read_3d_overflow_mask_START (29)
#define SOC_NPU_AICORE_AIC_ERROR_MASK2_mte_err_read_3d_overflow_mask_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long ccu_bus_err_addr : 48;
        unsigned long long ccu_bus_err_type : 3;
        unsigned long long reserved : 13;
    } reg;
} SOC_NPU_AICORE_CCU_BUS_ERR_INFO_UNION;
#endif
#define SOC_NPU_AICORE_CCU_BUS_ERR_INFO_ccu_bus_err_addr_START (0)
#define SOC_NPU_AICORE_CCU_BUS_ERR_INFO_ccu_bus_err_addr_END (47)
#define SOC_NPU_AICORE_CCU_BUS_ERR_INFO_ccu_bus_err_type_START (48)
#define SOC_NPU_AICORE_CCU_BUS_ERR_INFO_ccu_bus_err_type_END (50)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long mte_timeout : 1;
        unsigned long long mte_ub_rd_cflt : 1;
        unsigned long long mte_ub_wr_cflt : 1;
        unsigned long long mte_ktable_wr_addr_overflow : 1;
        unsigned long long mte_ktable_rd_addr_overflow : 1;
        unsigned long long ccu_ub_rd_cflt : 1;
        unsigned long long ccu_ub_wr_cflt : 1;
        unsigned long long reserved : 57;
    } reg;
} SOC_NPU_AICORE_AIC_ERROR1_UNION;
#endif
#define SOC_NPU_AICORE_AIC_ERROR1_mte_timeout_START (0)
#define SOC_NPU_AICORE_AIC_ERROR1_mte_timeout_END (0)
#define SOC_NPU_AICORE_AIC_ERROR1_mte_ub_rd_cflt_START (1)
#define SOC_NPU_AICORE_AIC_ERROR1_mte_ub_rd_cflt_END (1)
#define SOC_NPU_AICORE_AIC_ERROR1_mte_ub_wr_cflt_START (2)
#define SOC_NPU_AICORE_AIC_ERROR1_mte_ub_wr_cflt_END (2)
#define SOC_NPU_AICORE_AIC_ERROR1_mte_ktable_wr_addr_overflow_START (3)
#define SOC_NPU_AICORE_AIC_ERROR1_mte_ktable_wr_addr_overflow_END (3)
#define SOC_NPU_AICORE_AIC_ERROR1_mte_ktable_rd_addr_overflow_START (4)
#define SOC_NPU_AICORE_AIC_ERROR1_mte_ktable_rd_addr_overflow_END (4)
#define SOC_NPU_AICORE_AIC_ERROR1_ccu_ub_rd_cflt_START (5)
#define SOC_NPU_AICORE_AIC_ERROR1_ccu_ub_rd_cflt_END (5)
#define SOC_NPU_AICORE_AIC_ERROR1_ccu_ub_wr_cflt_START (6)
#define SOC_NPU_AICORE_AIC_ERROR1_ccu_ub_wr_cflt_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long mte_timeout_mask : 1;
        unsigned long long mte_ub_rd_cflt_mask : 1;
        unsigned long long mte_ub_wr_cflt_mask : 1;
        unsigned long long mte_ktable_wr_addr_overflow_mask : 1;
        unsigned long long mte_ktable_rd_addr_overflow_mask : 1;
        unsigned long long ccu_ub_rd_cflt_mask : 1;
        unsigned long long ccu_ub_wr_cflt_mask : 1;
        unsigned long long reserved : 57;
    } reg;
} SOC_NPU_AICORE_AIC_ERROR_MASK1_UNION;
#endif
#define SOC_NPU_AICORE_AIC_ERROR_MASK1_mte_timeout_mask_START (0)
#define SOC_NPU_AICORE_AIC_ERROR_MASK1_mte_timeout_mask_END (0)
#define SOC_NPU_AICORE_AIC_ERROR_MASK1_mte_ub_rd_cflt_mask_START (1)
#define SOC_NPU_AICORE_AIC_ERROR_MASK1_mte_ub_rd_cflt_mask_END (1)
#define SOC_NPU_AICORE_AIC_ERROR_MASK1_mte_ub_wr_cflt_mask_START (2)
#define SOC_NPU_AICORE_AIC_ERROR_MASK1_mte_ub_wr_cflt_mask_END (2)
#define SOC_NPU_AICORE_AIC_ERROR_MASK1_mte_ktable_wr_addr_overflow_mask_START (3)
#define SOC_NPU_AICORE_AIC_ERROR_MASK1_mte_ktable_wr_addr_overflow_mask_END (3)
#define SOC_NPU_AICORE_AIC_ERROR_MASK1_mte_ktable_rd_addr_overflow_mask_START (4)
#define SOC_NPU_AICORE_AIC_ERROR_MASK1_mte_ktable_rd_addr_overflow_mask_END (4)
#define SOC_NPU_AICORE_AIC_ERROR_MASK1_ccu_ub_rd_cflt_mask_START (5)
#define SOC_NPU_AICORE_AIC_ERROR_MASK1_ccu_ub_rd_cflt_mask_END (5)
#define SOC_NPU_AICORE_AIC_ERROR_MASK1_ccu_ub_wr_cflt_mask_START (6)
#define SOC_NPU_AICORE_AIC_ERROR_MASK1_ccu_ub_wr_cflt_mask_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long pct_en : 1;
        unsigned long long pct_user_profile_mode : 1;
        unsigned long long pct_loop_instr_en : 1;
        unsigned long long pct_timestamp_gray : 1;
        unsigned long long pct_timestamp_en_dly : 4;
        unsigned long long pct_ctrl_reserved : 56;
    } reg;
} SOC_NPU_AICORE_PCT_CTRL_UNION;
#endif
#define SOC_NPU_AICORE_PCT_CTRL_pct_en_START (0)
#define SOC_NPU_AICORE_PCT_CTRL_pct_en_END (0)
#define SOC_NPU_AICORE_PCT_CTRL_pct_user_profile_mode_START (1)
#define SOC_NPU_AICORE_PCT_CTRL_pct_user_profile_mode_END (1)
#define SOC_NPU_AICORE_PCT_CTRL_pct_loop_instr_en_START (2)
#define SOC_NPU_AICORE_PCT_CTRL_pct_loop_instr_en_END (2)
#define SOC_NPU_AICORE_PCT_CTRL_pct_timestamp_gray_START (3)
#define SOC_NPU_AICORE_PCT_CTRL_pct_timestamp_gray_END (3)
#define SOC_NPU_AICORE_PCT_CTRL_pct_timestamp_en_dly_START (4)
#define SOC_NPU_AICORE_PCT_CTRL_pct_timestamp_en_dly_END (7)
#define SOC_NPU_AICORE_PCT_CTRL_pct_ctrl_reserved_START (8)
#define SOC_NPU_AICORE_PCT_CTRL_pct_ctrl_reserved_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long pct_overflow : 1;
        unsigned long long pct_done : 1;
        unsigned long long pct_buf_err : 1;
        unsigned long long pct_warning : 1;
        unsigned long long reserved : 60;
    } reg;
} SOC_NPU_AICORE_PCT_STATUS_UNION;
#endif
#define SOC_NPU_AICORE_PCT_STATUS_pct_overflow_START (0)
#define SOC_NPU_AICORE_PCT_STATUS_pct_overflow_END (0)
#define SOC_NPU_AICORE_PCT_STATUS_pct_done_START (1)
#define SOC_NPU_AICORE_PCT_STATUS_pct_done_END (1)
#define SOC_NPU_AICORE_PCT_STATUS_pct_buf_err_START (2)
#define SOC_NPU_AICORE_PCT_STATUS_pct_buf_err_END (2)
#define SOC_NPU_AICORE_PCT_STATUS_pct_warning_START (3)
#define SOC_NPU_AICORE_PCT_STATUS_pct_warning_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long pct_num_entries : 10;
        unsigned long long reserved : 54;
    } reg;
} SOC_NPU_AICORE_PCT_NUM_ENTRIES_UNION;
#endif
#define SOC_NPU_AICORE_PCT_NUM_ENTRIES_pct_num_entries_START (0)
#define SOC_NPU_AICORE_PCT_NUM_ENTRIES_pct_num_entries_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long pct_start_cnt_cyc : 64;
    } reg;
} SOC_NPU_AICORE_PCT_START_CNT_CYC_UNION;
#endif
#define SOC_NPU_AICORE_PCT_START_CNT_CYC_pct_start_cnt_cyc_START (0)
#define SOC_NPU_AICORE_PCT_START_CNT_CYC_pct_start_cnt_cyc_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long pct_stop_cnt_cyc : 64;
    } reg;
} SOC_NPU_AICORE_PCT_STOP_CNT_CYC_UNION;
#endif
#define SOC_NPU_AICORE_PCT_STOP_CNT_CYC_pct_stop_cnt_cyc_START (0)
#define SOC_NPU_AICORE_PCT_STOP_CNT_CYC_pct_stop_cnt_cyc_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long pct_ov_timestamp : 64;
    } reg;
} SOC_NPU_AICORE_PCT_OV_TIMESTAMP_UNION;
#endif
#define SOC_NPU_AICORE_PCT_OV_TIMESTAMP_pct_ov_timestamp_START (0)
#define SOC_NPU_AICORE_PCT_OV_TIMESTAMP_pct_ov_timestamp_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long vec_wb_mrg_disable : 1;
        unsigned long long vec_vms4_chicken_en : 1;
        unsigned long long vec_col2img_cflt_chk_en : 1;
        unsigned long long vec_instr_fusion_disable : 1;
        unsigned long long reserved : 60;
    } reg;
} SOC_NPU_AICORE_VEC_CTRL_UNION;
#endif
#define SOC_NPU_AICORE_VEC_CTRL_vec_wb_mrg_disable_START (0)
#define SOC_NPU_AICORE_VEC_CTRL_vec_wb_mrg_disable_END (0)
#define SOC_NPU_AICORE_VEC_CTRL_vec_vms4_chicken_en_START (1)
#define SOC_NPU_AICORE_VEC_CTRL_vec_vms4_chicken_en_END (1)
#define SOC_NPU_AICORE_VEC_CTRL_vec_col2img_cflt_chk_en_START (2)
#define SOC_NPU_AICORE_VEC_CTRL_vec_col2img_cflt_chk_en_END (2)
#define SOC_NPU_AICORE_VEC_CTRL_vec_instr_fusion_disable_START (3)
#define SOC_NPU_AICORE_VEC_CTRL_vec_instr_fusion_disable_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long vec_reserved_reg00 : 64;
    } reg;
} SOC_NPU_AICORE_VEC_RESERVED_REG00_UNION;
#endif
#define SOC_NPU_AICORE_VEC_RESERVED_REG00_vec_reserved_reg00_START (0)
#define SOC_NPU_AICORE_VEC_RESERVED_REG00_vec_reserved_reg00_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long vec_reserved_reg01 : 64;
    } reg;
} SOC_NPU_AICORE_VEC_RESERVED_REG01_UNION;
#endif
#define SOC_NPU_AICORE_VEC_RESERVED_REG01_vec_reserved_reg01_START (0)
#define SOC_NPU_AICORE_VEC_RESERVED_REG01_vec_reserved_reg01_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long cube_reserved_reg00 : 64;
    } reg;
} SOC_NPU_AICORE_CUBE_RESERVED_REG00_UNION;
#endif
#define SOC_NPU_AICORE_CUBE_RESERVED_REG00_cube_reserved_reg00_START (0)
#define SOC_NPU_AICORE_CUBE_RESERVED_REG00_cube_reserved_reg00_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long cube_reserved_reg01 : 64;
    } reg;
} SOC_NPU_AICORE_CUBE_RESERVED_REG01_UNION;
#endif
#define SOC_NPU_AICORE_CUBE_RESERVED_REG01_cube_reserved_reg01_START (0)
#define SOC_NPU_AICORE_CUBE_RESERVED_REG01_cube_reserved_reg01_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long eco_ver : 4;
        unsigned long long sc_reserved_reg00_7to4 : 4;
        unsigned long long sc_ifu_dual_issue_ec_mask : 1;
        unsigned long long sc_reserved_reg00 : 55;
    } reg;
} SOC_NPU_AICORE_SC_RESERVED_REG00_UNION;
#endif
#define SOC_NPU_AICORE_SC_RESERVED_REG00_eco_ver_START (0)
#define SOC_NPU_AICORE_SC_RESERVED_REG00_eco_ver_END (3)
#define SOC_NPU_AICORE_SC_RESERVED_REG00_sc_reserved_reg00_7to4_START (4)
#define SOC_NPU_AICORE_SC_RESERVED_REG00_sc_reserved_reg00_7to4_END (7)
#define SOC_NPU_AICORE_SC_RESERVED_REG00_sc_ifu_dual_issue_ec_mask_START (8)
#define SOC_NPU_AICORE_SC_RESERVED_REG00_sc_ifu_dual_issue_ec_mask_END (8)
#define SOC_NPU_AICORE_SC_RESERVED_REG00_sc_reserved_reg00_START (9)
#define SOC_NPU_AICORE_SC_RESERVED_REG00_sc_reserved_reg00_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long cube_l0c_mbist_all_mask_n : 4;
        unsigned long long cube_l0b_mbist_all_mask_n : 1;
        unsigned long long sc_reserved_reg01_7to5 : 3;
        unsigned long long mte_l0a_mbist_all_mask_n : 4;
        unsigned long long sc_reserved_reg01_15to12 : 4;
        unsigned long long mte_rob_mbist_all_mask_n : 4;
        unsigned long long mte_aipp_mbist_all_mask_n : 1;
        unsigned long long sc_reserved_reg01_27to21 : 7;
        unsigned long long ifu_mbist_all_mask_n : 1;
        unsigned long long sc_reserved_reg01_31to29 : 3;
        unsigned long long pct_mbist_all_mask_n : 1;
        unsigned long long sc_reserved_reg01_35to33 : 3;
        unsigned long long mte_l1_mbist_all_mask_n : 6;
        unsigned long long ccu_sbuf_mbist_all_mask_n : 1;
        unsigned long long sc_reserved_reg01 : 21;
    } reg;
} SOC_NPU_AICORE_SC_RESERVED_REG01_UNION;
#endif
#define SOC_NPU_AICORE_SC_RESERVED_REG01_cube_l0c_mbist_all_mask_n_START (0)
#define SOC_NPU_AICORE_SC_RESERVED_REG01_cube_l0c_mbist_all_mask_n_END (3)
#define SOC_NPU_AICORE_SC_RESERVED_REG01_cube_l0b_mbist_all_mask_n_START (4)
#define SOC_NPU_AICORE_SC_RESERVED_REG01_cube_l0b_mbist_all_mask_n_END (4)
#define SOC_NPU_AICORE_SC_RESERVED_REG01_sc_reserved_reg01_7to5_START (5)
#define SOC_NPU_AICORE_SC_RESERVED_REG01_sc_reserved_reg01_7to5_END (7)
#define SOC_NPU_AICORE_SC_RESERVED_REG01_mte_l0a_mbist_all_mask_n_START (8)
#define SOC_NPU_AICORE_SC_RESERVED_REG01_mte_l0a_mbist_all_mask_n_END (11)
#define SOC_NPU_AICORE_SC_RESERVED_REG01_sc_reserved_reg01_15to12_START (12)
#define SOC_NPU_AICORE_SC_RESERVED_REG01_sc_reserved_reg01_15to12_END (15)
#define SOC_NPU_AICORE_SC_RESERVED_REG01_mte_rob_mbist_all_mask_n_START (16)
#define SOC_NPU_AICORE_SC_RESERVED_REG01_mte_rob_mbist_all_mask_n_END (19)
#define SOC_NPU_AICORE_SC_RESERVED_REG01_mte_aipp_mbist_all_mask_n_START (20)
#define SOC_NPU_AICORE_SC_RESERVED_REG01_mte_aipp_mbist_all_mask_n_END (20)
#define SOC_NPU_AICORE_SC_RESERVED_REG01_sc_reserved_reg01_27to21_START (21)
#define SOC_NPU_AICORE_SC_RESERVED_REG01_sc_reserved_reg01_27to21_END (27)
#define SOC_NPU_AICORE_SC_RESERVED_REG01_ifu_mbist_all_mask_n_START (28)
#define SOC_NPU_AICORE_SC_RESERVED_REG01_ifu_mbist_all_mask_n_END (28)
#define SOC_NPU_AICORE_SC_RESERVED_REG01_sc_reserved_reg01_31to29_START (29)
#define SOC_NPU_AICORE_SC_RESERVED_REG01_sc_reserved_reg01_31to29_END (31)
#define SOC_NPU_AICORE_SC_RESERVED_REG01_pct_mbist_all_mask_n_START (32)
#define SOC_NPU_AICORE_SC_RESERVED_REG01_pct_mbist_all_mask_n_END (32)
#define SOC_NPU_AICORE_SC_RESERVED_REG01_sc_reserved_reg01_35to33_START (33)
#define SOC_NPU_AICORE_SC_RESERVED_REG01_sc_reserved_reg01_35to33_END (35)
#define SOC_NPU_AICORE_SC_RESERVED_REG01_mte_l1_mbist_all_mask_n_START (36)
#define SOC_NPU_AICORE_SC_RESERVED_REG01_mte_l1_mbist_all_mask_n_END (41)
#define SOC_NPU_AICORE_SC_RESERVED_REG01_ccu_sbuf_mbist_all_mask_n_START (42)
#define SOC_NPU_AICORE_SC_RESERVED_REG01_ccu_sbuf_mbist_all_mask_n_END (42)
#define SOC_NPU_AICORE_SC_RESERVED_REG01_sc_reserved_reg01_START (43)
#define SOC_NPU_AICORE_SC_RESERVED_REG01_sc_reserved_reg01_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long sc_reserved_ro_reg00 : 64;
    } reg;
} SOC_NPU_AICORE_SC_RESERVED_RO_REG00_UNION;
#endif
#define SOC_NPU_AICORE_SC_RESERVED_RO_REG00_sc_reserved_ro_reg00_START (0)
#define SOC_NPU_AICORE_SC_RESERVED_RO_REG00_sc_reserved_ro_reg00_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long sc_reserved_ro_reg01 : 64;
    } reg;
} SOC_NPU_AICORE_SC_RESERVED_RO_REG01_UNION;
#endif
#define SOC_NPU_AICORE_SC_RESERVED_RO_REG01_sc_reserved_ro_reg01_START (0)
#define SOC_NPU_AICORE_SC_RESERVED_RO_REG01_sc_reserved_ro_reg01_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long sc_reserved_reg02 : 64;
    } reg;
} SOC_NPU_AICORE_SC_RESERVED_REG02_UNION;
#endif
#define SOC_NPU_AICORE_SC_RESERVED_REG02_sc_reserved_reg02_START (0)
#define SOC_NPU_AICORE_SC_RESERVED_REG02_sc_reserved_reg02_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long sc_reserved_reg03 : 64;
    } reg;
} SOC_NPU_AICORE_SC_RESERVED_REG03_UNION;
#endif
#define SOC_NPU_AICORE_SC_RESERVED_REG03_sc_reserved_reg03_START (0)
#define SOC_NPU_AICORE_SC_RESERVED_REG03_sc_reserved_reg03_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long vec_reserved_ro_reg00 : 64;
    } reg;
} SOC_NPU_AICORE_VEC_RESERVED_RO_REG00_UNION;
#endif
#define SOC_NPU_AICORE_VEC_RESERVED_RO_REG00_vec_reserved_ro_reg00_START (0)
#define SOC_NPU_AICORE_VEC_RESERVED_RO_REG00_vec_reserved_ro_reg00_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long vec_reserved_ro_reg01 : 64;
    } reg;
} SOC_NPU_AICORE_VEC_RESERVED_RO_REG01_UNION;
#endif
#define SOC_NPU_AICORE_VEC_RESERVED_RO_REG01_vec_reserved_ro_reg01_START (0)
#define SOC_NPU_AICORE_VEC_RESERVED_RO_REG01_vec_reserved_ro_reg01_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long cube_reserved_ro_reg00 : 64;
    } reg;
} SOC_NPU_AICORE_CUBE_RESERVED_RO_REG00_UNION;
#endif
#define SOC_NPU_AICORE_CUBE_RESERVED_RO_REG00_cube_reserved_ro_reg00_START (0)
#define SOC_NPU_AICORE_CUBE_RESERVED_RO_REG00_cube_reserved_ro_reg00_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long cube_reserved_ro_reg01 : 64;
    } reg;
} SOC_NPU_AICORE_CUBE_RESERVED_RO_REG01_UNION;
#endif
#define SOC_NPU_AICORE_CUBE_RESERVED_RO_REG01_cube_reserved_ro_reg01_START (0)
#define SOC_NPU_AICORE_CUBE_RESERVED_RO_REG01_cube_reserved_ro_reg01_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long mte_fuse_en : 1;
        unsigned long long reserved_0 : 15;
        unsigned long long bwif_cfg : 8;
        unsigned long long brif_cfg : 8;
        unsigned long long mte_erly_pth_en_n : 1;
        unsigned long long mte_l1_wr_arb_cfg : 4;
        unsigned long long mte_aipp_rsv : 16;
        unsigned long long mte_ub_wr_comb_en : 1;
        unsigned long long reserved_1 : 10;
    } reg;
} SOC_NPU_AICORE_MTE_CTRL_UNION;
#endif
#define SOC_NPU_AICORE_MTE_CTRL_mte_fuse_en_START (0)
#define SOC_NPU_AICORE_MTE_CTRL_mte_fuse_en_END (0)
#define SOC_NPU_AICORE_MTE_CTRL_bwif_cfg_START (16)
#define SOC_NPU_AICORE_MTE_CTRL_bwif_cfg_END (23)
#define SOC_NPU_AICORE_MTE_CTRL_brif_cfg_START (24)
#define SOC_NPU_AICORE_MTE_CTRL_brif_cfg_END (31)
#define SOC_NPU_AICORE_MTE_CTRL_mte_erly_pth_en_n_START (32)
#define SOC_NPU_AICORE_MTE_CTRL_mte_erly_pth_en_n_END (32)
#define SOC_NPU_AICORE_MTE_CTRL_mte_l1_wr_arb_cfg_START (33)
#define SOC_NPU_AICORE_MTE_CTRL_mte_l1_wr_arb_cfg_END (36)
#define SOC_NPU_AICORE_MTE_CTRL_mte_aipp_rsv_START (37)
#define SOC_NPU_AICORE_MTE_CTRL_mte_aipp_rsv_END (52)
#define SOC_NPU_AICORE_MTE_CTRL_mte_ub_wr_comb_en_START (53)
#define SOC_NPU_AICORE_MTE_CTRL_mte_ub_wr_comb_en_END (53)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long cube_dummy_nop_en : 1;
        unsigned long long cube_dummy_nop_cycle : 3;
        unsigned long long cube_dummy_start_en : 1;
        unsigned long long cube_dummy_cfg_cycle : 4;
        unsigned long long cube_dummy_cfg_k : 5;
        unsigned long long cube_vdrop_cycle : 6;
        unsigned long long cube_vdrop_en : 1;
        unsigned long long cube_inst_interval_cycle : 6;
        unsigned long long cube_active_cfg_en : 1;
        unsigned long long cube_active_interval_cycle : 6;
        unsigned long long cube_dummy_interval_cycle : 6;
        unsigned long long cube_dummy_data0 : 8;
        unsigned long long cube_dummy_data1 : 8;
        unsigned long long reserved : 8;
    } reg;
} SOC_NPU_AICORE_CUBE_CTRL_UNION;
#endif
#define SOC_NPU_AICORE_CUBE_CTRL_cube_dummy_nop_en_START (0)
#define SOC_NPU_AICORE_CUBE_CTRL_cube_dummy_nop_en_END (0)
#define SOC_NPU_AICORE_CUBE_CTRL_cube_dummy_nop_cycle_START (1)
#define SOC_NPU_AICORE_CUBE_CTRL_cube_dummy_nop_cycle_END (3)
#define SOC_NPU_AICORE_CUBE_CTRL_cube_dummy_start_en_START (4)
#define SOC_NPU_AICORE_CUBE_CTRL_cube_dummy_start_en_END (4)
#define SOC_NPU_AICORE_CUBE_CTRL_cube_dummy_cfg_cycle_START (5)
#define SOC_NPU_AICORE_CUBE_CTRL_cube_dummy_cfg_cycle_END (8)
#define SOC_NPU_AICORE_CUBE_CTRL_cube_dummy_cfg_k_START (9)
#define SOC_NPU_AICORE_CUBE_CTRL_cube_dummy_cfg_k_END (13)
#define SOC_NPU_AICORE_CUBE_CTRL_cube_vdrop_cycle_START (14)
#define SOC_NPU_AICORE_CUBE_CTRL_cube_vdrop_cycle_END (19)
#define SOC_NPU_AICORE_CUBE_CTRL_cube_vdrop_en_START (20)
#define SOC_NPU_AICORE_CUBE_CTRL_cube_vdrop_en_END (20)
#define SOC_NPU_AICORE_CUBE_CTRL_cube_inst_interval_cycle_START (21)
#define SOC_NPU_AICORE_CUBE_CTRL_cube_inst_interval_cycle_END (26)
#define SOC_NPU_AICORE_CUBE_CTRL_cube_active_cfg_en_START (27)
#define SOC_NPU_AICORE_CUBE_CTRL_cube_active_cfg_en_END (27)
#define SOC_NPU_AICORE_CUBE_CTRL_cube_active_interval_cycle_START (28)
#define SOC_NPU_AICORE_CUBE_CTRL_cube_active_interval_cycle_END (33)
#define SOC_NPU_AICORE_CUBE_CTRL_cube_dummy_interval_cycle_START (34)
#define SOC_NPU_AICORE_CUBE_CTRL_cube_dummy_interval_cycle_END (39)
#define SOC_NPU_AICORE_CUBE_CTRL_cube_dummy_data0_START (40)
#define SOC_NPU_AICORE_CUBE_CTRL_cube_dummy_data0_END (47)
#define SOC_NPU_AICORE_CUBE_CTRL_cube_dummy_data1_START (48)
#define SOC_NPU_AICORE_CUBE_CTRL_cube_dummy_data1_END (55)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long biu8_status2 : 64;
    } reg;
} SOC_NPU_AICORE_BIU8_STATUS2_UNION;
#endif
#define SOC_NPU_AICORE_BIU8_STATUS2_biu8_status2_START (0)
#define SOC_NPU_AICORE_BIU8_STATUS2_biu8_status2_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long biu8_status3 : 64;
    } reg;
} SOC_NPU_AICORE_BIU8_STATUS3_UNION;
#endif
#define SOC_NPU_AICORE_BIU8_STATUS3_biu8_status3_START (0)
#define SOC_NPU_AICORE_BIU8_STATUS3_biu8_status3_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long biu8_status4 : 64;
    } reg;
} SOC_NPU_AICORE_BIU8_STATUS4_UNION;
#endif
#define SOC_NPU_AICORE_BIU8_STATUS4_biu8_status4_START (0)
#define SOC_NPU_AICORE_BIU8_STATUS4_biu8_status4_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long biu8_smmu_streamid : 16;
        unsigned long long reserved : 48;
    } reg;
} SOC_NPU_AICORE_BIU8_SMMU_STREAMID_UNION;
#endif
#define SOC_NPU_AICORE_BIU8_SMMU_STREAMID_biu8_smmu_streamid_START (0)
#define SOC_NPU_AICORE_BIU8_SMMU_STREAMID_biu8_smmu_streamid_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long biu8_l2_paddr_base : 64;
    } reg;
} SOC_NPU_AICORE_BIU8_L2_REMAP_PADDR_BASE_UNION;
#endif
#define SOC_NPU_AICORE_BIU8_L2_REMAP_PADDR_BASE_biu8_l2_paddr_base_START (0)
#define SOC_NPU_AICORE_BIU8_L2_REMAP_PADDR_BASE_biu8_l2_paddr_base_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long biu8_l2_page_size : 3;
        unsigned long long reserved : 61;
    } reg;
} SOC_NPU_AICORE_BIU8_L2_PAGE_SIZE_UNION;
#endif
#define SOC_NPU_AICORE_BIU8_L2_PAGE_SIZE_biu8_l2_page_size_START (0)
#define SOC_NPU_AICORE_BIU8_L2_PAGE_SIZE_biu8_l2_page_size_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct {
        unsigned long long biu8_l2_size : 3;
        unsigned long long reserved : 60;
        unsigned long long biu8_l2_remap_en : 1;
    } reg;
} SOC_NPU_AICORE_BIU8_L2_SIZE_UNION;
#endif
#define SOC_NPU_AICORE_BIU8_L2_SIZE_biu8_l2_size_START (0)
#define SOC_NPU_AICORE_BIU8_L2_SIZE_biu8_l2_size_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long biu8_ctrl3 : 64;
    } reg;
} SOC_NPU_AICORE_BIU8_CTRL3_UNION;
#endif
#define SOC_NPU_AICORE_BIU8_CTRL3_biu8_ctrl3_START (0)
#define SOC_NPU_AICORE_BIU8_CTRL3_biu8_ctrl3_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long biu8_ctrl4 : 64;
    } reg;
} SOC_NPU_AICORE_BIU8_CTRL4_UNION;
#endif
#define SOC_NPU_AICORE_BIU8_CTRL4_biu8_ctrl4_START (0)
#define SOC_NPU_AICORE_BIU8_CTRL4_biu8_ctrl4_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long biu8_ctrl5 : 64;
    } reg;
} SOC_NPU_AICORE_BIU8_CTRL5_UNION;
#endif
#define SOC_NPU_AICORE_BIU8_CTRL5_biu8_ctrl5_START (0)
#define SOC_NPU_AICORE_BIU8_CTRL5_biu8_ctrl5_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long biu8_status6 : 64;
    } reg;
} SOC_NPU_AICORE_BIU8_STATUS6_UNION;
#endif
#define SOC_NPU_AICORE_BIU8_STATUS6_biu8_status6_START (0)
#define SOC_NPU_AICORE_BIU8_STATUS6_biu8_status6_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long biu8_status7 : 64;
    } reg;
} SOC_NPU_AICORE_BIU8_STATUS7_UNION;
#endif
#define SOC_NPU_AICORE_BIU8_STATUS7_biu8_status7_START (0)
#define SOC_NPU_AICORE_BIU8_STATUS7_biu8_status7_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long tiny_except_compcnt : 63;
        unsigned long long tiny_except_compcnt_bypass : 1;
    } reg;
} SOC_NPU_AICORE_CCU_TIME_COMPARE_COUNTER_CTRL_UNION;
#endif
#define SOC_NPU_AICORE_CCU_TIME_COMPARE_COUNTER_CTRL_tiny_except_compcnt_START (0)
#define SOC_NPU_AICORE_CCU_TIME_COMPARE_COUNTER_CTRL_tiny_except_compcnt_END (62)
#define SOC_NPU_AICORE_CCU_TIME_COMPARE_COUNTER_CTRL_tiny_except_compcnt_bypass_START (63)
#define SOC_NPU_AICORE_CCU_TIME_COMPARE_COUNTER_CTRL_tiny_except_compcnt_bypass_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long tiny_except_compcnt_int : 1;
        unsigned long long reserved : 63;
    } reg;
} SOC_NPU_AICORE_CCU_TIME_COMPARE_COUNTER_INT_UNION;
#endif
#define SOC_NPU_AICORE_CCU_TIME_COMPARE_COUNTER_INT_tiny_except_compcnt_int_START (0)
#define SOC_NPU_AICORE_CCU_TIME_COMPARE_COUNTER_INT_tiny_except_compcnt_int_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long aic_sec_en : 1;
        unsigned long long reserved : 63;
    } reg;
} SOC_NPU_AICORE_AIC_SECURE_EN_UNION;
#endif
#define SOC_NPU_AICORE_AIC_SECURE_EN_aic_sec_en_START (0)
#define SOC_NPU_AICORE_AIC_SECURE_EN_aic_sec_en_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long dfx_mem_req : 1;
        unsigned long long dfx_mem_all : 1;
        unsigned long long reserved_0 : 2;
        unsigned long long dfx_mem_array : 8;
        unsigned long long reserved_1 : 4;
        unsigned long long dfx_mem_we : 1;
        unsigned long long dfx_mem_waddr : 9;
        unsigned long long dfx_mem_wbe : 4;
        unsigned long long reserved_2 : 2;
        unsigned long long dfx_mem_re : 1;
        unsigned long long dfx_mem_add_rd : 9;
        unsigned long long reserved_3 : 21;
        unsigned long long dfx_mem_access0 : 1;
    } reg;
} SOC_NPU_AICORE_DFX_MEM_ACCESS0_CTRL_UNION;
#endif
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_CTRL_dfx_mem_req_START (0)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_CTRL_dfx_mem_req_END (0)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_CTRL_dfx_mem_all_START (1)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_CTRL_dfx_mem_all_END (1)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_CTRL_dfx_mem_array_START (4)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_CTRL_dfx_mem_array_END (11)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_CTRL_dfx_mem_we_START (16)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_CTRL_dfx_mem_we_END (16)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_CTRL_dfx_mem_waddr_START (17)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_CTRL_dfx_mem_waddr_END (25)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_CTRL_dfx_mem_wbe_START (26)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_CTRL_dfx_mem_wbe_END (29)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_CTRL_dfx_mem_re_START (32)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_CTRL_dfx_mem_re_END (32)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_CTRL_dfx_mem_add_rd_START (33)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_CTRL_dfx_mem_add_rd_END (41)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_CTRL_dfx_mem_access0_START (63)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_CTRL_dfx_mem_access0_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long dfx_mem_wreq_ack : 1;
        unsigned long long dfx_mem_rready : 1;
        unsigned long long reserved : 62;
    } reg;
} SOC_NPU_AICORE_DFX_MEM_ACCESS0_STATUS_UNION;
#endif
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_STATUS_dfx_mem_wreq_ack_START (0)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_STATUS_dfx_mem_wreq_ack_END (0)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_STATUS_dfx_mem_rready_START (1)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_STATUS_dfx_mem_rready_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long dfx_mem_access0_wdata0 : 64;
    } reg;
} SOC_NPU_AICORE_DFX_MEM_ACCESS0_WDATA0_UNION;
#endif
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_WDATA0_dfx_mem_access0_wdata0_START (0)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_WDATA0_dfx_mem_access0_wdata0_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long dfx_mem_access0_wdata1 : 64;
    } reg;
} SOC_NPU_AICORE_DFX_MEM_ACCESS0_WDATA1_UNION;
#endif
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_WDATA1_dfx_mem_access0_wdata1_START (0)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_WDATA1_dfx_mem_access0_wdata1_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long dfx_mem_access0_wdata2 : 64;
    } reg;
} SOC_NPU_AICORE_DFX_MEM_ACCESS0_WDATA2_UNION;
#endif
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_WDATA2_dfx_mem_access0_wdata2_START (0)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_WDATA2_dfx_mem_access0_wdata2_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long dfx_mem_access0_wdata3 : 64;
    } reg;
} SOC_NPU_AICORE_DFX_MEM_ACCESS0_WDATA3_UNION;
#endif
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_WDATA3_dfx_mem_access0_wdata3_START (0)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_WDATA3_dfx_mem_access0_wdata3_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long dfx_mem_access0_wdata4 : 12;
        unsigned long long reserved : 52;
    } reg;
} SOC_NPU_AICORE_DFX_MEM_ACCESS0_WDATA4_UNION;
#endif
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_WDATA4_dfx_mem_access0_wdata4_START (0)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_WDATA4_dfx_mem_access0_wdata4_END (11)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long dfx_mem_access0_rdata0 : 64;
    } reg;
} SOC_NPU_AICORE_DFX_MEM_ACCESS0_RDATA0_UNION;
#endif
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_RDATA0_dfx_mem_access0_rdata0_START (0)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_RDATA0_dfx_mem_access0_rdata0_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long dfx_mem_access0_rdata1 : 64;
    } reg;
} SOC_NPU_AICORE_DFX_MEM_ACCESS0_RDATA1_UNION;
#endif
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_RDATA1_dfx_mem_access0_rdata1_START (0)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_RDATA1_dfx_mem_access0_rdata1_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long dfx_mem_access0_rdata2 : 64;
    } reg;
} SOC_NPU_AICORE_DFX_MEM_ACCESS0_RDATA2_UNION;
#endif
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_RDATA2_dfx_mem_access0_rdata2_START (0)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_RDATA2_dfx_mem_access0_rdata2_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long dfx_mem_access0_rdata3 : 64;
    } reg;
} SOC_NPU_AICORE_DFX_MEM_ACCESS0_RDATA3_UNION;
#endif
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_RDATA3_dfx_mem_access0_rdata3_START (0)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_RDATA3_dfx_mem_access0_rdata3_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long dfx_mem_access0_rdata4 : 12;
        unsigned long long reserved : 52;
    } reg;
} SOC_NPU_AICORE_DFX_MEM_ACCESS0_RDATA4_UNION;
#endif
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_RDATA4_dfx_mem_access0_rdata4_START (0)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS0_RDATA4_dfx_mem_access0_rdata4_END (11)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long dfx_mem_req1 : 1;
        unsigned long long dfx_mem_all1 : 1;
        unsigned long long reserved_0 : 2;
        unsigned long long dfx_mem_array1 : 6;
        unsigned long long reserved_1 : 6;
        unsigned long long dfx_mem_we1 : 1;
        unsigned long long dfx_mem_waddr1 : 9;
        unsigned long long reserved_2 : 6;
        unsigned long long dfx_mem_re1 : 1;
        unsigned long long dfx_mem_add_rd1 : 9;
        unsigned long long reserved_3 : 21;
        unsigned long long dfx_mem_access1 : 1;
    } reg;
} SOC_NPU_AICORE_DFX_MEM_ACCESS1_CTRL_UNION;
#endif
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_CTRL_dfx_mem_req1_START (0)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_CTRL_dfx_mem_req1_END (0)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_CTRL_dfx_mem_all1_START (1)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_CTRL_dfx_mem_all1_END (1)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_CTRL_dfx_mem_array1_START (4)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_CTRL_dfx_mem_array1_END (9)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_CTRL_dfx_mem_we1_START (16)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_CTRL_dfx_mem_we1_END (16)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_CTRL_dfx_mem_waddr1_START (17)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_CTRL_dfx_mem_waddr1_END (25)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_CTRL_dfx_mem_re1_START (32)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_CTRL_dfx_mem_re1_END (32)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_CTRL_dfx_mem_add_rd1_START (33)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_CTRL_dfx_mem_add_rd1_END (41)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_CTRL_dfx_mem_access1_START (63)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_CTRL_dfx_mem_access1_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long dfx_mem_wreq_ack1 : 1;
        unsigned long long dfx_mem_rready1 : 1;
        unsigned long long reserved : 62;
    } reg;
} SOC_NPU_AICORE_DFX_MEM_ACCESS1_STATUS_UNION;
#endif
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_STATUS_dfx_mem_wreq_ack1_START (0)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_STATUS_dfx_mem_wreq_ack1_END (0)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_STATUS_dfx_mem_rready1_START (1)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_STATUS_dfx_mem_rready1_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long dfx_mem_access1_wdata0 : 64;
    } reg;
} SOC_NPU_AICORE_DFX_MEM_ACCESS1_WDATA0_UNION;
#endif
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_WDATA0_dfx_mem_access1_wdata0_START (0)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_WDATA0_dfx_mem_access1_wdata0_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long dfx_mem_access1_wdata1 : 64;
    } reg;
} SOC_NPU_AICORE_DFX_MEM_ACCESS1_WDATA1_UNION;
#endif
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_WDATA1_dfx_mem_access1_wdata1_START (0)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_WDATA1_dfx_mem_access1_wdata1_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long dfx_mem_access1_wdata2 : 64;
    } reg;
} SOC_NPU_AICORE_DFX_MEM_ACCESS1_WDATA2_UNION;
#endif
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_WDATA2_dfx_mem_access1_wdata2_START (0)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_WDATA2_dfx_mem_access1_wdata2_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long dfx_mem_access1_wdata3 : 64;
    } reg;
} SOC_NPU_AICORE_DFX_MEM_ACCESS1_WDATA3_UNION;
#endif
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_WDATA3_dfx_mem_access1_wdata3_START (0)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_WDATA3_dfx_mem_access1_wdata3_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long dfx_mem_access1_wdata4 : 10;
        unsigned long long reserved : 54;
    } reg;
} SOC_NPU_AICORE_DFX_MEM_ACCESS1_WDATA4_UNION;
#endif
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_WDATA4_dfx_mem_access1_wdata4_START (0)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_WDATA4_dfx_mem_access1_wdata4_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long dfx_mem_access1_rdata0 : 64;
    } reg;
} SOC_NPU_AICORE_DFX_MEM_ACCESS1_RDATA0_UNION;
#endif
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_RDATA0_dfx_mem_access1_rdata0_START (0)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_RDATA0_dfx_mem_access1_rdata0_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long dfx_mem_access1_rdata1 : 64;
    } reg;
} SOC_NPU_AICORE_DFX_MEM_ACCESS1_RDATA1_UNION;
#endif
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_RDATA1_dfx_mem_access1_rdata1_START (0)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_RDATA1_dfx_mem_access1_rdata1_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long dfx_mem_access1_rdata2 : 64;
    } reg;
} SOC_NPU_AICORE_DFX_MEM_ACCESS1_RDATA2_UNION;
#endif
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_RDATA2_dfx_mem_access1_rdata2_START (0)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_RDATA2_dfx_mem_access1_rdata2_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long dfx_mem_access1_rdata3 : 64;
    } reg;
} SOC_NPU_AICORE_DFX_MEM_ACCESS1_RDATA3_UNION;
#endif
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_RDATA3_dfx_mem_access1_rdata3_START (0)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_RDATA3_dfx_mem_access1_rdata3_END (63)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned long long value;
    struct
    {
        unsigned long long dfx_mem_access1_rdata4 : 10;
        unsigned long long reserved : 54;
    } reg;
} SOC_NPU_AICORE_DFX_MEM_ACCESS1_RDATA4_UNION;
#endif
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_RDATA4_dfx_mem_access1_rdata4_START (0)
#define SOC_NPU_AICORE_DFX_MEM_ACCESS1_RDATA4_dfx_mem_access1_rdata4_END (9)
#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
#endif
