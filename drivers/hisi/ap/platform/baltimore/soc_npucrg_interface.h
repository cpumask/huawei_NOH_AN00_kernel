#ifndef __SOC_NPUCRG_INTERFACE_H__
#define __SOC_NPUCRG_INTERFACE_H__ 
#ifdef __cplusplus
    #if __cplusplus
        extern "C" {
    #endif
#endif
#ifndef __SOC_H_FOR_ASM__
#define SOC_NPUCRG_PEREN0_ADDR(base) ((base) + (0x000UL))
#define SOC_NPUCRG_PERDIS0_ADDR(base) ((base) + (0x004UL))
#define SOC_NPUCRG_PERCLKEN0_ADDR(base) ((base) + (0x008UL))
#define SOC_NPUCRG_PERSTAT0_ADDR(base) ((base) + (0x00CUL))
#define SOC_NPUCRG_PEREN1_ADDR(base) ((base) + (0x010UL))
#define SOC_NPUCRG_PERDIS1_ADDR(base) ((base) + (0x014UL))
#define SOC_NPUCRG_PERCLKEN1_ADDR(base) ((base) + (0x018UL))
#define SOC_NPUCRG_PERSTAT1_ADDR(base) ((base) + (0x01CUL))
#define SOC_NPUCRG_PEREN2_ADDR(base) ((base) + (0x020UL))
#define SOC_NPUCRG_PERDIS2_ADDR(base) ((base) + (0x024UL))
#define SOC_NPUCRG_PERCLKEN2_ADDR(base) ((base) + (0x028UL))
#define SOC_NPUCRG_PERSTAT2_ADDR(base) ((base) + (0x02CUL))
#define SOC_NPUCRG_PEREN3_ADDR(base) ((base) + (0x030UL))
#define SOC_NPUCRG_PERDIS3_ADDR(base) ((base) + (0x034UL))
#define SOC_NPUCRG_PERCLKEN3_ADDR(base) ((base) + (0x038UL))
#define SOC_NPUCRG_PERSTAT3_ADDR(base) ((base) + (0x03CUL))
#define SOC_NPUCRG_PERRSTEN0_ADDR(base) ((base) + (0x040UL))
#define SOC_NPUCRG_PERRSTDIS0_ADDR(base) ((base) + (0x044UL))
#define SOC_NPUCRG_PERRSTSTAT0_ADDR(base) ((base) + (0x048UL))
#define SOC_NPUCRG_PERRSTEN1_ADDR(base) ((base) + (0x04cUL))
#define SOC_NPUCRG_PERRSTDIS1_ADDR(base) ((base) + (0x050UL))
#define SOC_NPUCRG_PERRSTSTAT1_ADDR(base) ((base) + (0x054UL))
#define SOC_NPUCRG_PERRSTEN2_ADDR(base) ((base) + (0x058UL))
#define SOC_NPUCRG_PERRSTDIS2_ADDR(base) ((base) + (0x05cUL))
#define SOC_NPUCRG_PERRSTSTAT2_ADDR(base) ((base) + (0x060UL))
#define SOC_NPUCRG_CLKDIV0_ADDR(base) ((base) + (0x070UL))
#define SOC_NPUCRG_CLKDIV1_ADDR(base) ((base) + (0x078UL))
#define SOC_NPUCRG_PERI_STAT0_ADDR(base) ((base) + (0x080UL))
#define SOC_NPUCRG_PERI_CTRL0_ADDR(base) ((base) + (0x084UL))
#define SOC_NPUCRG_IPCLKRST_BYPASS_ADDR(base) ((base) + (0x090UL))
#define SOC_NPUCRG_PERI_AUTODIV0_ADDR(base) ((base) + (0x0A0UL))
#define SOC_NPUCRG_PERI_AUTODIV1_ADDR(base) ((base) + (0x0A4UL))
#define SOC_NPUCRG_PERI_AUTODIV2_ADDR(base) ((base) + (0x0A8UL))
#define SOC_NPUCRG_PERI_AUTODIV_INUSE_STAT_ADDR(base) ((base) + (0x0ACUL))
#define SOC_NPUCRG_PERI_AUTODIV3_ADDR(base) ((base) + (0x0B0UL))
#define SOC_NPUCRG_PERI_AUTODIV4_ADDR(base) ((base) + (0x0B4UL))
#define SOC_NPUCRG_PERI_AUTODIV5_ADDR(base) ((base) + (0x0B8UL))
#define SOC_NPUCRG_PPLL5CTRL0_ADDR(base) ((base) + (0x198UL))
#define SOC_NPUCRG_PPLL5CTRL1_ADDR(base) ((base) + (0x19CUL))
#define SOC_NPUCRG_PPLL5_CTRL_A_ADDR(base) ((base) + (0x200UL))
#define SOC_NPUCRG_PPLL5_CTRL_B_ADDR(base) ((base) + (0x204UL))
#define SOC_NPUCRG_PPLL5SSCCTRL_ADDR(base) ((base) + (0x208UL))
#define SOC_NPUCRG_PLL_VOTE_STAT_ADDR(base) ((base) + (0x20CUL))
#define SOC_NPUCRG_PLL_EN_VOTE_CTRL_ADDR(base) ((base) + (0x210UL))
#define SOC_NPUCRG_PLL_GT_VOTE_CTRL_ADDR(base) ((base) + (0x214UL))
#define SOC_NPUCRG_PLL_BYPASS_VOTE_CTRL_ADDR(base) ((base) + (0x218UL))
#define SOC_NPUCRG_FNPLL_STAT0_ADDR(base) ((base) + (0x220UL))
#define SOC_NPUCRG_FNPLL_STAT1_ADDR(base) ((base) + (0x224UL))
#define SOC_NPUCRG_FNPLL_CFG0_ADDR(base) ((base) + (0x228UL))
#define SOC_NPUCRG_FNPLL_CFG1_ADDR(base) ((base) + (0x22CUL))
#define SOC_NPUCRG_FNPLL_CFG2_ADDR(base) ((base) + (0x230UL))
#define SOC_NPUCRG_FNPLL_CFG3_ADDR(base) ((base) + (0x234UL))
#define SOC_NPUCRG_FNPLL_CFG4_ADDR(base) ((base) + (0x238UL))
#define SOC_NPUCRG_FNPLL_CFG5_ADDR(base) ((base) + (0x23CUL))
#define SOC_NPUCRG_VS_CTRL_EN_NPU_ADDR(base) ((base) + (0x240UL))
#define SOC_NPUCRG_VS_CTRL_BYPASS_NPU_ADDR(base) ((base) + (0x244UL))
#define SOC_NPUCRG_VS_CTRL_NPU_ADDR(base) ((base) + (0x248UL))
#define SOC_NPUCRG_VS_SVFD_CTRL0_NPU_ADDR(base) ((base) + (0x24CUL))
#define SOC_NPUCRG_VS_SVFD_CTRL1_NPU_ADDR(base) ((base) + (0x250UL))
#define SOC_NPUCRG_VS_SVFD_CTRL2_NPU_ADDR(base) ((base) + (0x254UL))
#define SOC_NPUCRG_VS_CPM_DATA_STAT_NPU_ADDR(base) ((base) + (0x258UL))
#define SOC_NPUCRG_VS_TEST_STAT_NPU_ADDR(base) ((base) + (0x25CUL))
#define SOC_NPUCRG_PSTATE1_AICORE_ADDR(base) ((base) + (0x260UL))
#define SOC_NPUCRG_PSTATE2_AICORE_ADDR(base) ((base) + (0x264UL))
#define SOC_NPUCRG_PSTATE3_AICORE_ADDR(base) ((base) + (0x268UL))
#define SOC_NPUCRG_PSTATE4_AICORE_ADDR(base) ((base) + (0x26CUL))
#define SOC_NPUCRG_INTR_MASK_NOCBUS_NONIDLE_PEND_ADDR(base) ((base) + (0x500UL))
#define SOC_NPUCRG_INTR_CLR_NOCBUS_NONIDLE_PEND_ADDR(base) ((base) + (0x504UL))
#define SOC_NPUCRG_INTR_STAT_NOCBUS_NONIDLE_PEND_ADDR(base) ((base) + (0x508UL))
#else
#define SOC_NPUCRG_PEREN0_ADDR(base) ((base) + (0x000))
#define SOC_NPUCRG_PERDIS0_ADDR(base) ((base) + (0x004))
#define SOC_NPUCRG_PERCLKEN0_ADDR(base) ((base) + (0x008))
#define SOC_NPUCRG_PERSTAT0_ADDR(base) ((base) + (0x00C))
#define SOC_NPUCRG_PEREN1_ADDR(base) ((base) + (0x010))
#define SOC_NPUCRG_PERDIS1_ADDR(base) ((base) + (0x014))
#define SOC_NPUCRG_PERCLKEN1_ADDR(base) ((base) + (0x018))
#define SOC_NPUCRG_PERSTAT1_ADDR(base) ((base) + (0x01C))
#define SOC_NPUCRG_PEREN2_ADDR(base) ((base) + (0x020))
#define SOC_NPUCRG_PERDIS2_ADDR(base) ((base) + (0x024))
#define SOC_NPUCRG_PERCLKEN2_ADDR(base) ((base) + (0x028))
#define SOC_NPUCRG_PERSTAT2_ADDR(base) ((base) + (0x02C))
#define SOC_NPUCRG_PEREN3_ADDR(base) ((base) + (0x030))
#define SOC_NPUCRG_PERDIS3_ADDR(base) ((base) + (0x034))
#define SOC_NPUCRG_PERCLKEN3_ADDR(base) ((base) + (0x038))
#define SOC_NPUCRG_PERSTAT3_ADDR(base) ((base) + (0x03C))
#define SOC_NPUCRG_PERRSTEN0_ADDR(base) ((base) + (0x040))
#define SOC_NPUCRG_PERRSTDIS0_ADDR(base) ((base) + (0x044))
#define SOC_NPUCRG_PERRSTSTAT0_ADDR(base) ((base) + (0x048))
#define SOC_NPUCRG_PERRSTEN1_ADDR(base) ((base) + (0x04c))
#define SOC_NPUCRG_PERRSTDIS1_ADDR(base) ((base) + (0x050))
#define SOC_NPUCRG_PERRSTSTAT1_ADDR(base) ((base) + (0x054))
#define SOC_NPUCRG_PERRSTEN2_ADDR(base) ((base) + (0x058))
#define SOC_NPUCRG_PERRSTDIS2_ADDR(base) ((base) + (0x05c))
#define SOC_NPUCRG_PERRSTSTAT2_ADDR(base) ((base) + (0x060))
#define SOC_NPUCRG_CLKDIV0_ADDR(base) ((base) + (0x070))
#define SOC_NPUCRG_CLKDIV1_ADDR(base) ((base) + (0x078))
#define SOC_NPUCRG_PERI_STAT0_ADDR(base) ((base) + (0x080))
#define SOC_NPUCRG_PERI_CTRL0_ADDR(base) ((base) + (0x084))
#define SOC_NPUCRG_IPCLKRST_BYPASS_ADDR(base) ((base) + (0x090))
#define SOC_NPUCRG_PERI_AUTODIV0_ADDR(base) ((base) + (0x0A0))
#define SOC_NPUCRG_PERI_AUTODIV1_ADDR(base) ((base) + (0x0A4))
#define SOC_NPUCRG_PERI_AUTODIV2_ADDR(base) ((base) + (0x0A8))
#define SOC_NPUCRG_PERI_AUTODIV_INUSE_STAT_ADDR(base) ((base) + (0x0AC))
#define SOC_NPUCRG_PERI_AUTODIV3_ADDR(base) ((base) + (0x0B0))
#define SOC_NPUCRG_PERI_AUTODIV4_ADDR(base) ((base) + (0x0B4))
#define SOC_NPUCRG_PERI_AUTODIV5_ADDR(base) ((base) + (0x0B8))
#define SOC_NPUCRG_PPLL5CTRL0_ADDR(base) ((base) + (0x198))
#define SOC_NPUCRG_PPLL5CTRL1_ADDR(base) ((base) + (0x19C))
#define SOC_NPUCRG_PPLL5_CTRL_A_ADDR(base) ((base) + (0x200))
#define SOC_NPUCRG_PPLL5_CTRL_B_ADDR(base) ((base) + (0x204))
#define SOC_NPUCRG_PPLL5SSCCTRL_ADDR(base) ((base) + (0x208))
#define SOC_NPUCRG_PLL_VOTE_STAT_ADDR(base) ((base) + (0x20C))
#define SOC_NPUCRG_PLL_EN_VOTE_CTRL_ADDR(base) ((base) + (0x210))
#define SOC_NPUCRG_PLL_GT_VOTE_CTRL_ADDR(base) ((base) + (0x214))
#define SOC_NPUCRG_PLL_BYPASS_VOTE_CTRL_ADDR(base) ((base) + (0x218))
#define SOC_NPUCRG_FNPLL_STAT0_ADDR(base) ((base) + (0x220))
#define SOC_NPUCRG_FNPLL_STAT1_ADDR(base) ((base) + (0x224))
#define SOC_NPUCRG_FNPLL_CFG0_ADDR(base) ((base) + (0x228))
#define SOC_NPUCRG_FNPLL_CFG1_ADDR(base) ((base) + (0x22C))
#define SOC_NPUCRG_FNPLL_CFG2_ADDR(base) ((base) + (0x230))
#define SOC_NPUCRG_FNPLL_CFG3_ADDR(base) ((base) + (0x234))
#define SOC_NPUCRG_FNPLL_CFG4_ADDR(base) ((base) + (0x238))
#define SOC_NPUCRG_FNPLL_CFG5_ADDR(base) ((base) + (0x23C))
#define SOC_NPUCRG_VS_CTRL_EN_NPU_ADDR(base) ((base) + (0x240))
#define SOC_NPUCRG_VS_CTRL_BYPASS_NPU_ADDR(base) ((base) + (0x244))
#define SOC_NPUCRG_VS_CTRL_NPU_ADDR(base) ((base) + (0x248))
#define SOC_NPUCRG_VS_SVFD_CTRL0_NPU_ADDR(base) ((base) + (0x24C))
#define SOC_NPUCRG_VS_SVFD_CTRL1_NPU_ADDR(base) ((base) + (0x250))
#define SOC_NPUCRG_VS_SVFD_CTRL2_NPU_ADDR(base) ((base) + (0x254))
#define SOC_NPUCRG_VS_CPM_DATA_STAT_NPU_ADDR(base) ((base) + (0x258))
#define SOC_NPUCRG_VS_TEST_STAT_NPU_ADDR(base) ((base) + (0x25C))
#define SOC_NPUCRG_PSTATE1_AICORE_ADDR(base) ((base) + (0x260))
#define SOC_NPUCRG_PSTATE2_AICORE_ADDR(base) ((base) + (0x264))
#define SOC_NPUCRG_PSTATE3_AICORE_ADDR(base) ((base) + (0x268))
#define SOC_NPUCRG_PSTATE4_AICORE_ADDR(base) ((base) + (0x26C))
#define SOC_NPUCRG_INTR_MASK_NOCBUS_NONIDLE_PEND_ADDR(base) ((base) + (0x500))
#define SOC_NPUCRG_INTR_CLR_NOCBUS_NONIDLE_PEND_ADDR(base) ((base) + (0x504))
#define SOC_NPUCRG_INTR_STAT_NOCBUS_NONIDLE_PEND_ADDR(base) ((base) + (0x508))
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_ai_core0 : 1;
        unsigned int gt_atclk_npu : 1;
        unsigned int gt_atclk_ai_core0_brg : 1;
        unsigned int gt_pclkdbg_ai_core0 : 1;
        unsigned int gt_clk_npu2dmss_adb_slv : 1;
        unsigned int reserved_0 : 1;
        unsigned int gt_atclk_sysdma_brg : 1;
        unsigned int reserved_1 : 1;
        unsigned int gt_pclkdbg_sysdma : 1;
        unsigned int gt_clk_aicore0_smmu : 1;
        unsigned int gt_clk_npu_sysdma_tiny : 1;
        unsigned int gt_clk_sysdma_smmu0 : 1;
        unsigned int gt_clk_sysdma_bus_monitor : 1;
        unsigned int gt_clk_npu_sysdma_lite : 1;
        unsigned int gt_clk_npu_l2buf : 1;
        unsigned int gt_clk_npu_tcu : 1;
        unsigned int gt_clk_aicore0_bus_monitor : 1;
        unsigned int gt_clk_aicore0_hpm : 1;
        unsigned int gt_clk_aicore0_tidm : 1;
        unsigned int gt_clk_aicore0_pa : 1;
        unsigned int reserved_2 : 1;
        unsigned int gt_clk_noc_tiny : 1;
        unsigned int gt_clk_noc_ts_cfg : 1;
        unsigned int gt_clk_noc_ts : 1;
        unsigned int gt_clk_noc_tcu : 1;
        unsigned int gt_clk_noc_l2buf : 1;
        unsigned int gt_clk_noc_sysdma_tiny : 1;
        unsigned int gt_clk_noc_sysdma_lite : 1;
        unsigned int gt_clk_noc_aicore0 : 1;
        unsigned int gt_clk_npu_tidm : 1;
        unsigned int gt_clk_npu_pa : 1;
        unsigned int gt_clk_npu_hpm : 1;
    } reg;
} SOC_NPUCRG_PEREN0_UNION;
#endif
#define SOC_NPUCRG_PEREN0_gt_clk_ai_core0_START (0)
#define SOC_NPUCRG_PEREN0_gt_clk_ai_core0_END (0)
#define SOC_NPUCRG_PEREN0_gt_atclk_npu_START (1)
#define SOC_NPUCRG_PEREN0_gt_atclk_npu_END (1)
#define SOC_NPUCRG_PEREN0_gt_atclk_ai_core0_brg_START (2)
#define SOC_NPUCRG_PEREN0_gt_atclk_ai_core0_brg_END (2)
#define SOC_NPUCRG_PEREN0_gt_pclkdbg_ai_core0_START (3)
#define SOC_NPUCRG_PEREN0_gt_pclkdbg_ai_core0_END (3)
#define SOC_NPUCRG_PEREN0_gt_clk_npu2dmss_adb_slv_START (4)
#define SOC_NPUCRG_PEREN0_gt_clk_npu2dmss_adb_slv_END (4)
#define SOC_NPUCRG_PEREN0_gt_atclk_sysdma_brg_START (6)
#define SOC_NPUCRG_PEREN0_gt_atclk_sysdma_brg_END (6)
#define SOC_NPUCRG_PEREN0_gt_pclkdbg_sysdma_START (8)
#define SOC_NPUCRG_PEREN0_gt_pclkdbg_sysdma_END (8)
#define SOC_NPUCRG_PEREN0_gt_clk_aicore0_smmu_START (9)
#define SOC_NPUCRG_PEREN0_gt_clk_aicore0_smmu_END (9)
#define SOC_NPUCRG_PEREN0_gt_clk_npu_sysdma_tiny_START (10)
#define SOC_NPUCRG_PEREN0_gt_clk_npu_sysdma_tiny_END (10)
#define SOC_NPUCRG_PEREN0_gt_clk_sysdma_smmu0_START (11)
#define SOC_NPUCRG_PEREN0_gt_clk_sysdma_smmu0_END (11)
#define SOC_NPUCRG_PEREN0_gt_clk_sysdma_bus_monitor_START (12)
#define SOC_NPUCRG_PEREN0_gt_clk_sysdma_bus_monitor_END (12)
#define SOC_NPUCRG_PEREN0_gt_clk_npu_sysdma_lite_START (13)
#define SOC_NPUCRG_PEREN0_gt_clk_npu_sysdma_lite_END (13)
#define SOC_NPUCRG_PEREN0_gt_clk_npu_l2buf_START (14)
#define SOC_NPUCRG_PEREN0_gt_clk_npu_l2buf_END (14)
#define SOC_NPUCRG_PEREN0_gt_clk_npu_tcu_START (15)
#define SOC_NPUCRG_PEREN0_gt_clk_npu_tcu_END (15)
#define SOC_NPUCRG_PEREN0_gt_clk_aicore0_bus_monitor_START (16)
#define SOC_NPUCRG_PEREN0_gt_clk_aicore0_bus_monitor_END (16)
#define SOC_NPUCRG_PEREN0_gt_clk_aicore0_hpm_START (17)
#define SOC_NPUCRG_PEREN0_gt_clk_aicore0_hpm_END (17)
#define SOC_NPUCRG_PEREN0_gt_clk_aicore0_tidm_START (18)
#define SOC_NPUCRG_PEREN0_gt_clk_aicore0_tidm_END (18)
#define SOC_NPUCRG_PEREN0_gt_clk_aicore0_pa_START (19)
#define SOC_NPUCRG_PEREN0_gt_clk_aicore0_pa_END (19)
#define SOC_NPUCRG_PEREN0_gt_clk_noc_tiny_START (21)
#define SOC_NPUCRG_PEREN0_gt_clk_noc_tiny_END (21)
#define SOC_NPUCRG_PEREN0_gt_clk_noc_ts_cfg_START (22)
#define SOC_NPUCRG_PEREN0_gt_clk_noc_ts_cfg_END (22)
#define SOC_NPUCRG_PEREN0_gt_clk_noc_ts_START (23)
#define SOC_NPUCRG_PEREN0_gt_clk_noc_ts_END (23)
#define SOC_NPUCRG_PEREN0_gt_clk_noc_tcu_START (24)
#define SOC_NPUCRG_PEREN0_gt_clk_noc_tcu_END (24)
#define SOC_NPUCRG_PEREN0_gt_clk_noc_l2buf_START (25)
#define SOC_NPUCRG_PEREN0_gt_clk_noc_l2buf_END (25)
#define SOC_NPUCRG_PEREN0_gt_clk_noc_sysdma_tiny_START (26)
#define SOC_NPUCRG_PEREN0_gt_clk_noc_sysdma_tiny_END (26)
#define SOC_NPUCRG_PEREN0_gt_clk_noc_sysdma_lite_START (27)
#define SOC_NPUCRG_PEREN0_gt_clk_noc_sysdma_lite_END (27)
#define SOC_NPUCRG_PEREN0_gt_clk_noc_aicore0_START (28)
#define SOC_NPUCRG_PEREN0_gt_clk_noc_aicore0_END (28)
#define SOC_NPUCRG_PEREN0_gt_clk_npu_tidm_START (29)
#define SOC_NPUCRG_PEREN0_gt_clk_npu_tidm_END (29)
#define SOC_NPUCRG_PEREN0_gt_clk_npu_pa_START (30)
#define SOC_NPUCRG_PEREN0_gt_clk_npu_pa_END (30)
#define SOC_NPUCRG_PEREN0_gt_clk_npu_hpm_START (31)
#define SOC_NPUCRG_PEREN0_gt_clk_npu_hpm_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_ai_core0 : 1;
        unsigned int gt_atclk_npu : 1;
        unsigned int gt_atclk_ai_core0_brg : 1;
        unsigned int gt_pclkdbg_ai_core0 : 1;
        unsigned int gt_clk_npu2dmss_adb_slv : 1;
        unsigned int reserved_0 : 1;
        unsigned int gt_atclk_sysdma_brg : 1;
        unsigned int reserved_1 : 1;
        unsigned int gt_pclkdbg_sysdma : 1;
        unsigned int gt_clk_aicore0_smmu : 1;
        unsigned int gt_clk_npu_sysdma_tiny : 1;
        unsigned int gt_clk_sysdma_smmu0 : 1;
        unsigned int gt_clk_sysdma_bus_monitor : 1;
        unsigned int gt_clk_npu_sysdma_lite : 1;
        unsigned int gt_clk_npu_l2buf : 1;
        unsigned int gt_clk_npu_tcu : 1;
        unsigned int gt_clk_aicore0_bus_monitor : 1;
        unsigned int gt_clk_aicore0_hpm : 1;
        unsigned int gt_clk_aicore0_tidm : 1;
        unsigned int gt_clk_aicore0_pa : 1;
        unsigned int reserved_2 : 1;
        unsigned int gt_clk_noc_tiny : 1;
        unsigned int gt_clk_noc_ts_cfg : 1;
        unsigned int gt_clk_noc_ts : 1;
        unsigned int gt_clk_noc_tcu : 1;
        unsigned int gt_clk_noc_l2buf : 1;
        unsigned int gt_clk_noc_sysdma_tiny : 1;
        unsigned int gt_clk_noc_sysdma_lite : 1;
        unsigned int gt_clk_noc_aicore0 : 1;
        unsigned int gt_clk_npu_tidm : 1;
        unsigned int gt_clk_npu_pa : 1;
        unsigned int gt_clk_npu_hpm : 1;
    } reg;
} SOC_NPUCRG_PERDIS0_UNION;
#endif
#define SOC_NPUCRG_PERDIS0_gt_clk_ai_core0_START (0)
#define SOC_NPUCRG_PERDIS0_gt_clk_ai_core0_END (0)
#define SOC_NPUCRG_PERDIS0_gt_atclk_npu_START (1)
#define SOC_NPUCRG_PERDIS0_gt_atclk_npu_END (1)
#define SOC_NPUCRG_PERDIS0_gt_atclk_ai_core0_brg_START (2)
#define SOC_NPUCRG_PERDIS0_gt_atclk_ai_core0_brg_END (2)
#define SOC_NPUCRG_PERDIS0_gt_pclkdbg_ai_core0_START (3)
#define SOC_NPUCRG_PERDIS0_gt_pclkdbg_ai_core0_END (3)
#define SOC_NPUCRG_PERDIS0_gt_clk_npu2dmss_adb_slv_START (4)
#define SOC_NPUCRG_PERDIS0_gt_clk_npu2dmss_adb_slv_END (4)
#define SOC_NPUCRG_PERDIS0_gt_atclk_sysdma_brg_START (6)
#define SOC_NPUCRG_PERDIS0_gt_atclk_sysdma_brg_END (6)
#define SOC_NPUCRG_PERDIS0_gt_pclkdbg_sysdma_START (8)
#define SOC_NPUCRG_PERDIS0_gt_pclkdbg_sysdma_END (8)
#define SOC_NPUCRG_PERDIS0_gt_clk_aicore0_smmu_START (9)
#define SOC_NPUCRG_PERDIS0_gt_clk_aicore0_smmu_END (9)
#define SOC_NPUCRG_PERDIS0_gt_clk_npu_sysdma_tiny_START (10)
#define SOC_NPUCRG_PERDIS0_gt_clk_npu_sysdma_tiny_END (10)
#define SOC_NPUCRG_PERDIS0_gt_clk_sysdma_smmu0_START (11)
#define SOC_NPUCRG_PERDIS0_gt_clk_sysdma_smmu0_END (11)
#define SOC_NPUCRG_PERDIS0_gt_clk_sysdma_bus_monitor_START (12)
#define SOC_NPUCRG_PERDIS0_gt_clk_sysdma_bus_monitor_END (12)
#define SOC_NPUCRG_PERDIS0_gt_clk_npu_sysdma_lite_START (13)
#define SOC_NPUCRG_PERDIS0_gt_clk_npu_sysdma_lite_END (13)
#define SOC_NPUCRG_PERDIS0_gt_clk_npu_l2buf_START (14)
#define SOC_NPUCRG_PERDIS0_gt_clk_npu_l2buf_END (14)
#define SOC_NPUCRG_PERDIS0_gt_clk_npu_tcu_START (15)
#define SOC_NPUCRG_PERDIS0_gt_clk_npu_tcu_END (15)
#define SOC_NPUCRG_PERDIS0_gt_clk_aicore0_bus_monitor_START (16)
#define SOC_NPUCRG_PERDIS0_gt_clk_aicore0_bus_monitor_END (16)
#define SOC_NPUCRG_PERDIS0_gt_clk_aicore0_hpm_START (17)
#define SOC_NPUCRG_PERDIS0_gt_clk_aicore0_hpm_END (17)
#define SOC_NPUCRG_PERDIS0_gt_clk_aicore0_tidm_START (18)
#define SOC_NPUCRG_PERDIS0_gt_clk_aicore0_tidm_END (18)
#define SOC_NPUCRG_PERDIS0_gt_clk_aicore0_pa_START (19)
#define SOC_NPUCRG_PERDIS0_gt_clk_aicore0_pa_END (19)
#define SOC_NPUCRG_PERDIS0_gt_clk_noc_tiny_START (21)
#define SOC_NPUCRG_PERDIS0_gt_clk_noc_tiny_END (21)
#define SOC_NPUCRG_PERDIS0_gt_clk_noc_ts_cfg_START (22)
#define SOC_NPUCRG_PERDIS0_gt_clk_noc_ts_cfg_END (22)
#define SOC_NPUCRG_PERDIS0_gt_clk_noc_ts_START (23)
#define SOC_NPUCRG_PERDIS0_gt_clk_noc_ts_END (23)
#define SOC_NPUCRG_PERDIS0_gt_clk_noc_tcu_START (24)
#define SOC_NPUCRG_PERDIS0_gt_clk_noc_tcu_END (24)
#define SOC_NPUCRG_PERDIS0_gt_clk_noc_l2buf_START (25)
#define SOC_NPUCRG_PERDIS0_gt_clk_noc_l2buf_END (25)
#define SOC_NPUCRG_PERDIS0_gt_clk_noc_sysdma_tiny_START (26)
#define SOC_NPUCRG_PERDIS0_gt_clk_noc_sysdma_tiny_END (26)
#define SOC_NPUCRG_PERDIS0_gt_clk_noc_sysdma_lite_START (27)
#define SOC_NPUCRG_PERDIS0_gt_clk_noc_sysdma_lite_END (27)
#define SOC_NPUCRG_PERDIS0_gt_clk_noc_aicore0_START (28)
#define SOC_NPUCRG_PERDIS0_gt_clk_noc_aicore0_END (28)
#define SOC_NPUCRG_PERDIS0_gt_clk_npu_tidm_START (29)
#define SOC_NPUCRG_PERDIS0_gt_clk_npu_tidm_END (29)
#define SOC_NPUCRG_PERDIS0_gt_clk_npu_pa_START (30)
#define SOC_NPUCRG_PERDIS0_gt_clk_npu_pa_END (30)
#define SOC_NPUCRG_PERDIS0_gt_clk_npu_hpm_START (31)
#define SOC_NPUCRG_PERDIS0_gt_clk_npu_hpm_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_ai_core0 : 1;
        unsigned int gt_atclk_npu : 1;
        unsigned int gt_atclk_ai_core0_brg : 1;
        unsigned int gt_pclkdbg_ai_core0 : 1;
        unsigned int gt_clk_npu2dmss_adb_slv : 1;
        unsigned int reserved_0 : 1;
        unsigned int gt_atclk_sysdma_brg : 1;
        unsigned int reserved_1 : 1;
        unsigned int gt_pclkdbg_sysdma : 1;
        unsigned int gt_clk_aicore0_smmu : 1;
        unsigned int gt_clk_npu_sysdma_tiny : 1;
        unsigned int gt_clk_sysdma_smmu0 : 1;
        unsigned int gt_clk_sysdma_bus_monitor : 1;
        unsigned int gt_clk_npu_sysdma_lite : 1;
        unsigned int gt_clk_npu_l2buf : 1;
        unsigned int gt_clk_npu_tcu : 1;
        unsigned int gt_clk_aicore0_bus_monitor : 1;
        unsigned int gt_clk_aicore0_hpm : 1;
        unsigned int gt_clk_aicore0_tidm : 1;
        unsigned int gt_clk_aicore0_pa : 1;
        unsigned int reserved_2 : 1;
        unsigned int gt_clk_noc_tiny : 1;
        unsigned int gt_clk_noc_ts_cfg : 1;
        unsigned int gt_clk_noc_ts : 1;
        unsigned int gt_clk_noc_tcu : 1;
        unsigned int gt_clk_noc_l2buf : 1;
        unsigned int gt_clk_noc_sysdma_tiny : 1;
        unsigned int gt_clk_noc_sysdma_lite : 1;
        unsigned int gt_clk_noc_aicore0 : 1;
        unsigned int gt_clk_npu_tidm : 1;
        unsigned int gt_clk_npu_pa : 1;
        unsigned int gt_clk_npu_hpm : 1;
    } reg;
} SOC_NPUCRG_PERCLKEN0_UNION;
#endif
#define SOC_NPUCRG_PERCLKEN0_gt_clk_ai_core0_START (0)
#define SOC_NPUCRG_PERCLKEN0_gt_clk_ai_core0_END (0)
#define SOC_NPUCRG_PERCLKEN0_gt_atclk_npu_START (1)
#define SOC_NPUCRG_PERCLKEN0_gt_atclk_npu_END (1)
#define SOC_NPUCRG_PERCLKEN0_gt_atclk_ai_core0_brg_START (2)
#define SOC_NPUCRG_PERCLKEN0_gt_atclk_ai_core0_brg_END (2)
#define SOC_NPUCRG_PERCLKEN0_gt_pclkdbg_ai_core0_START (3)
#define SOC_NPUCRG_PERCLKEN0_gt_pclkdbg_ai_core0_END (3)
#define SOC_NPUCRG_PERCLKEN0_gt_clk_npu2dmss_adb_slv_START (4)
#define SOC_NPUCRG_PERCLKEN0_gt_clk_npu2dmss_adb_slv_END (4)
#define SOC_NPUCRG_PERCLKEN0_gt_atclk_sysdma_brg_START (6)
#define SOC_NPUCRG_PERCLKEN0_gt_atclk_sysdma_brg_END (6)
#define SOC_NPUCRG_PERCLKEN0_gt_pclkdbg_sysdma_START (8)
#define SOC_NPUCRG_PERCLKEN0_gt_pclkdbg_sysdma_END (8)
#define SOC_NPUCRG_PERCLKEN0_gt_clk_aicore0_smmu_START (9)
#define SOC_NPUCRG_PERCLKEN0_gt_clk_aicore0_smmu_END (9)
#define SOC_NPUCRG_PERCLKEN0_gt_clk_npu_sysdma_tiny_START (10)
#define SOC_NPUCRG_PERCLKEN0_gt_clk_npu_sysdma_tiny_END (10)
#define SOC_NPUCRG_PERCLKEN0_gt_clk_sysdma_smmu0_START (11)
#define SOC_NPUCRG_PERCLKEN0_gt_clk_sysdma_smmu0_END (11)
#define SOC_NPUCRG_PERCLKEN0_gt_clk_sysdma_bus_monitor_START (12)
#define SOC_NPUCRG_PERCLKEN0_gt_clk_sysdma_bus_monitor_END (12)
#define SOC_NPUCRG_PERCLKEN0_gt_clk_npu_sysdma_lite_START (13)
#define SOC_NPUCRG_PERCLKEN0_gt_clk_npu_sysdma_lite_END (13)
#define SOC_NPUCRG_PERCLKEN0_gt_clk_npu_l2buf_START (14)
#define SOC_NPUCRG_PERCLKEN0_gt_clk_npu_l2buf_END (14)
#define SOC_NPUCRG_PERCLKEN0_gt_clk_npu_tcu_START (15)
#define SOC_NPUCRG_PERCLKEN0_gt_clk_npu_tcu_END (15)
#define SOC_NPUCRG_PERCLKEN0_gt_clk_aicore0_bus_monitor_START (16)
#define SOC_NPUCRG_PERCLKEN0_gt_clk_aicore0_bus_monitor_END (16)
#define SOC_NPUCRG_PERCLKEN0_gt_clk_aicore0_hpm_START (17)
#define SOC_NPUCRG_PERCLKEN0_gt_clk_aicore0_hpm_END (17)
#define SOC_NPUCRG_PERCLKEN0_gt_clk_aicore0_tidm_START (18)
#define SOC_NPUCRG_PERCLKEN0_gt_clk_aicore0_tidm_END (18)
#define SOC_NPUCRG_PERCLKEN0_gt_clk_aicore0_pa_START (19)
#define SOC_NPUCRG_PERCLKEN0_gt_clk_aicore0_pa_END (19)
#define SOC_NPUCRG_PERCLKEN0_gt_clk_noc_tiny_START (21)
#define SOC_NPUCRG_PERCLKEN0_gt_clk_noc_tiny_END (21)
#define SOC_NPUCRG_PERCLKEN0_gt_clk_noc_ts_cfg_START (22)
#define SOC_NPUCRG_PERCLKEN0_gt_clk_noc_ts_cfg_END (22)
#define SOC_NPUCRG_PERCLKEN0_gt_clk_noc_ts_START (23)
#define SOC_NPUCRG_PERCLKEN0_gt_clk_noc_ts_END (23)
#define SOC_NPUCRG_PERCLKEN0_gt_clk_noc_tcu_START (24)
#define SOC_NPUCRG_PERCLKEN0_gt_clk_noc_tcu_END (24)
#define SOC_NPUCRG_PERCLKEN0_gt_clk_noc_l2buf_START (25)
#define SOC_NPUCRG_PERCLKEN0_gt_clk_noc_l2buf_END (25)
#define SOC_NPUCRG_PERCLKEN0_gt_clk_noc_sysdma_tiny_START (26)
#define SOC_NPUCRG_PERCLKEN0_gt_clk_noc_sysdma_tiny_END (26)
#define SOC_NPUCRG_PERCLKEN0_gt_clk_noc_sysdma_lite_START (27)
#define SOC_NPUCRG_PERCLKEN0_gt_clk_noc_sysdma_lite_END (27)
#define SOC_NPUCRG_PERCLKEN0_gt_clk_noc_aicore0_START (28)
#define SOC_NPUCRG_PERCLKEN0_gt_clk_noc_aicore0_END (28)
#define SOC_NPUCRG_PERCLKEN0_gt_clk_npu_tidm_START (29)
#define SOC_NPUCRG_PERCLKEN0_gt_clk_npu_tidm_END (29)
#define SOC_NPUCRG_PERCLKEN0_gt_clk_npu_pa_START (30)
#define SOC_NPUCRG_PERCLKEN0_gt_clk_npu_pa_END (30)
#define SOC_NPUCRG_PERCLKEN0_gt_clk_npu_hpm_START (31)
#define SOC_NPUCRG_PERCLKEN0_gt_clk_npu_hpm_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int st_clk_ai_core0 : 1;
        unsigned int st_atclk_npu : 1;
        unsigned int st_atclk_ai_core0_brg : 1;
        unsigned int st_pclkdbg_ai_core0 : 1;
        unsigned int st_clk_npu2dmss_adb_slv : 1;
        unsigned int reserved_0 : 1;
        unsigned int st_atclk_sysdma_brg : 1;
        unsigned int reserved_1 : 1;
        unsigned int st_pclkdbg_sysdma : 1;
        unsigned int st_clk_aicore0_smmu : 1;
        unsigned int reserved_2 : 1;
        unsigned int st_clk_sysdma_smmu0 : 1;
        unsigned int st_clk_sysdma_bus_monitor : 1;
        unsigned int st_clk_npu_sysdma : 1;
        unsigned int st_clk_npu_l2buf : 1;
        unsigned int st_clk_npu_tcu : 1;
        unsigned int st_clk_aicore0_bus_monitor : 1;
        unsigned int st_clk_aicore0_hpm : 1;
        unsigned int st_clk_aicore0_tidm : 1;
        unsigned int st_clk_aicore0_pa : 1;
        unsigned int reserved_3 : 1;
        unsigned int st_clk_noc_tiny : 1;
        unsigned int st_clk_noc_ts_cfg : 1;
        unsigned int st_clk_noc_ts : 1;
        unsigned int st_clk_noc_tcu : 1;
        unsigned int st_clk_noc_l2buf : 1;
        unsigned int reserved_4 : 1;
        unsigned int st_clk_noc_sysdma : 1;
        unsigned int st_clk_noc_aicore0 : 1;
        unsigned int st_clk_npu_tidm : 1;
        unsigned int st_clk_npu_pa : 1;
        unsigned int st_clk_npu_hpm : 1;
    } reg;
} SOC_NPUCRG_PERSTAT0_UNION;
#endif
#define SOC_NPUCRG_PERSTAT0_st_clk_ai_core0_START (0)
#define SOC_NPUCRG_PERSTAT0_st_clk_ai_core0_END (0)
#define SOC_NPUCRG_PERSTAT0_st_atclk_npu_START (1)
#define SOC_NPUCRG_PERSTAT0_st_atclk_npu_END (1)
#define SOC_NPUCRG_PERSTAT0_st_atclk_ai_core0_brg_START (2)
#define SOC_NPUCRG_PERSTAT0_st_atclk_ai_core0_brg_END (2)
#define SOC_NPUCRG_PERSTAT0_st_pclkdbg_ai_core0_START (3)
#define SOC_NPUCRG_PERSTAT0_st_pclkdbg_ai_core0_END (3)
#define SOC_NPUCRG_PERSTAT0_st_clk_npu2dmss_adb_slv_START (4)
#define SOC_NPUCRG_PERSTAT0_st_clk_npu2dmss_adb_slv_END (4)
#define SOC_NPUCRG_PERSTAT0_st_atclk_sysdma_brg_START (6)
#define SOC_NPUCRG_PERSTAT0_st_atclk_sysdma_brg_END (6)
#define SOC_NPUCRG_PERSTAT0_st_pclkdbg_sysdma_START (8)
#define SOC_NPUCRG_PERSTAT0_st_pclkdbg_sysdma_END (8)
#define SOC_NPUCRG_PERSTAT0_st_clk_aicore0_smmu_START (9)
#define SOC_NPUCRG_PERSTAT0_st_clk_aicore0_smmu_END (9)
#define SOC_NPUCRG_PERSTAT0_st_clk_sysdma_smmu0_START (11)
#define SOC_NPUCRG_PERSTAT0_st_clk_sysdma_smmu0_END (11)
#define SOC_NPUCRG_PERSTAT0_st_clk_sysdma_bus_monitor_START (12)
#define SOC_NPUCRG_PERSTAT0_st_clk_sysdma_bus_monitor_END (12)
#define SOC_NPUCRG_PERSTAT0_st_clk_npu_sysdma_START (13)
#define SOC_NPUCRG_PERSTAT0_st_clk_npu_sysdma_END (13)
#define SOC_NPUCRG_PERSTAT0_st_clk_npu_l2buf_START (14)
#define SOC_NPUCRG_PERSTAT0_st_clk_npu_l2buf_END (14)
#define SOC_NPUCRG_PERSTAT0_st_clk_npu_tcu_START (15)
#define SOC_NPUCRG_PERSTAT0_st_clk_npu_tcu_END (15)
#define SOC_NPUCRG_PERSTAT0_st_clk_aicore0_bus_monitor_START (16)
#define SOC_NPUCRG_PERSTAT0_st_clk_aicore0_bus_monitor_END (16)
#define SOC_NPUCRG_PERSTAT0_st_clk_aicore0_hpm_START (17)
#define SOC_NPUCRG_PERSTAT0_st_clk_aicore0_hpm_END (17)
#define SOC_NPUCRG_PERSTAT0_st_clk_aicore0_tidm_START (18)
#define SOC_NPUCRG_PERSTAT0_st_clk_aicore0_tidm_END (18)
#define SOC_NPUCRG_PERSTAT0_st_clk_aicore0_pa_START (19)
#define SOC_NPUCRG_PERSTAT0_st_clk_aicore0_pa_END (19)
#define SOC_NPUCRG_PERSTAT0_st_clk_noc_tiny_START (21)
#define SOC_NPUCRG_PERSTAT0_st_clk_noc_tiny_END (21)
#define SOC_NPUCRG_PERSTAT0_st_clk_noc_ts_cfg_START (22)
#define SOC_NPUCRG_PERSTAT0_st_clk_noc_ts_cfg_END (22)
#define SOC_NPUCRG_PERSTAT0_st_clk_noc_ts_START (23)
#define SOC_NPUCRG_PERSTAT0_st_clk_noc_ts_END (23)
#define SOC_NPUCRG_PERSTAT0_st_clk_noc_tcu_START (24)
#define SOC_NPUCRG_PERSTAT0_st_clk_noc_tcu_END (24)
#define SOC_NPUCRG_PERSTAT0_st_clk_noc_l2buf_START (25)
#define SOC_NPUCRG_PERSTAT0_st_clk_noc_l2buf_END (25)
#define SOC_NPUCRG_PERSTAT0_st_clk_noc_sysdma_START (27)
#define SOC_NPUCRG_PERSTAT0_st_clk_noc_sysdma_END (27)
#define SOC_NPUCRG_PERSTAT0_st_clk_noc_aicore0_START (28)
#define SOC_NPUCRG_PERSTAT0_st_clk_noc_aicore0_END (28)
#define SOC_NPUCRG_PERSTAT0_st_clk_npu_tidm_START (29)
#define SOC_NPUCRG_PERSTAT0_st_clk_npu_tidm_END (29)
#define SOC_NPUCRG_PERSTAT0_st_clk_npu_pa_START (30)
#define SOC_NPUCRG_PERSTAT0_st_clk_npu_pa_END (30)
#define SOC_NPUCRG_PERSTAT0_st_clk_npu_hpm_START (31)
#define SOC_NPUCRG_PERSTAT0_st_clk_npu_hpm_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_ppll5_sscg : 1;
        unsigned int gt_pclk_aicore0_smmu : 1;
        unsigned int reserved_0 : 1;
        unsigned int gt_pclk_npu_l2buf : 1;
        unsigned int gt_pclk_npu_uart : 1;
        unsigned int gt_pclk_npu_ipc_ns : 1;
        unsigned int gt_pclk_npu_ipc_s : 1;
        unsigned int gt_pclk_npu_hw_exp_irq : 1;
        unsigned int gt_pclk_autodiv_npubus : 1;
        unsigned int gt_pclk_autodiv_npubus_cfg : 1;
        unsigned int reserved_1 : 1;
        unsigned int gt_pclk_autodiv_npubus_aicore : 1;
        unsigned int gt_pclk_npu_easc3 : 1;
        unsigned int gt_pclk_npu_easc2 : 1;
        unsigned int gt_pclk_atgs : 1;
        unsigned int gt_pclk_atgm : 1;
        unsigned int gt_pclk_npu_sysdma_tiny : 1;
        unsigned int gt_pclk_npu_sysdma_lite : 1;
        unsigned int gt_pclk_sysdma_smmu0 : 1;
        unsigned int gt_pclk_npu_easc1 : 1;
        unsigned int gt_pclk_npu_tcu : 1;
        unsigned int gt_pclk_npu_pcr : 1;
        unsigned int gt_pclk_npu_dpm0 : 1;
        unsigned int gt_pclk_npu_dpm1 : 1;
        unsigned int gt_clk_ppll5_npu_tp : 1;
        unsigned int gt_clk_tcxo : 1;
        unsigned int gt_pclk_npu_easc0 : 1;
        unsigned int gt_clk_npu_loadmonitor : 1;
        unsigned int gt_pclk_npu_loadmonitor : 1;
        unsigned int gt_pclk_npu_idsc : 1;
        unsigned int gt_pclk_aicore1_smmu : 1;
        unsigned int gt_pclk_npu_hw_exp_irq_ns : 1;
    } reg;
} SOC_NPUCRG_PEREN1_UNION;
#endif
#define SOC_NPUCRG_PEREN1_gt_clk_ppll5_sscg_START (0)
#define SOC_NPUCRG_PEREN1_gt_clk_ppll5_sscg_END (0)
#define SOC_NPUCRG_PEREN1_gt_pclk_aicore0_smmu_START (1)
#define SOC_NPUCRG_PEREN1_gt_pclk_aicore0_smmu_END (1)
#define SOC_NPUCRG_PEREN1_gt_pclk_npu_l2buf_START (3)
#define SOC_NPUCRG_PEREN1_gt_pclk_npu_l2buf_END (3)
#define SOC_NPUCRG_PEREN1_gt_pclk_npu_uart_START (4)
#define SOC_NPUCRG_PEREN1_gt_pclk_npu_uart_END (4)
#define SOC_NPUCRG_PEREN1_gt_pclk_npu_ipc_ns_START (5)
#define SOC_NPUCRG_PEREN1_gt_pclk_npu_ipc_ns_END (5)
#define SOC_NPUCRG_PEREN1_gt_pclk_npu_ipc_s_START (6)
#define SOC_NPUCRG_PEREN1_gt_pclk_npu_ipc_s_END (6)
#define SOC_NPUCRG_PEREN1_gt_pclk_npu_hw_exp_irq_START (7)
#define SOC_NPUCRG_PEREN1_gt_pclk_npu_hw_exp_irq_END (7)
#define SOC_NPUCRG_PEREN1_gt_pclk_autodiv_npubus_START (8)
#define SOC_NPUCRG_PEREN1_gt_pclk_autodiv_npubus_END (8)
#define SOC_NPUCRG_PEREN1_gt_pclk_autodiv_npubus_cfg_START (9)
#define SOC_NPUCRG_PEREN1_gt_pclk_autodiv_npubus_cfg_END (9)
#define SOC_NPUCRG_PEREN1_gt_pclk_autodiv_npubus_aicore_START (11)
#define SOC_NPUCRG_PEREN1_gt_pclk_autodiv_npubus_aicore_END (11)
#define SOC_NPUCRG_PEREN1_gt_pclk_npu_easc3_START (12)
#define SOC_NPUCRG_PEREN1_gt_pclk_npu_easc3_END (12)
#define SOC_NPUCRG_PEREN1_gt_pclk_npu_easc2_START (13)
#define SOC_NPUCRG_PEREN1_gt_pclk_npu_easc2_END (13)
#define SOC_NPUCRG_PEREN1_gt_pclk_atgs_START (14)
#define SOC_NPUCRG_PEREN1_gt_pclk_atgs_END (14)
#define SOC_NPUCRG_PEREN1_gt_pclk_atgm_START (15)
#define SOC_NPUCRG_PEREN1_gt_pclk_atgm_END (15)
#define SOC_NPUCRG_PEREN1_gt_pclk_npu_sysdma_tiny_START (16)
#define SOC_NPUCRG_PEREN1_gt_pclk_npu_sysdma_tiny_END (16)
#define SOC_NPUCRG_PEREN1_gt_pclk_npu_sysdma_lite_START (17)
#define SOC_NPUCRG_PEREN1_gt_pclk_npu_sysdma_lite_END (17)
#define SOC_NPUCRG_PEREN1_gt_pclk_sysdma_smmu0_START (18)
#define SOC_NPUCRG_PEREN1_gt_pclk_sysdma_smmu0_END (18)
#define SOC_NPUCRG_PEREN1_gt_pclk_npu_easc1_START (19)
#define SOC_NPUCRG_PEREN1_gt_pclk_npu_easc1_END (19)
#define SOC_NPUCRG_PEREN1_gt_pclk_npu_tcu_START (20)
#define SOC_NPUCRG_PEREN1_gt_pclk_npu_tcu_END (20)
#define SOC_NPUCRG_PEREN1_gt_pclk_npu_pcr_START (21)
#define SOC_NPUCRG_PEREN1_gt_pclk_npu_pcr_END (21)
#define SOC_NPUCRG_PEREN1_gt_pclk_npu_dpm0_START (22)
#define SOC_NPUCRG_PEREN1_gt_pclk_npu_dpm0_END (22)
#define SOC_NPUCRG_PEREN1_gt_pclk_npu_dpm1_START (23)
#define SOC_NPUCRG_PEREN1_gt_pclk_npu_dpm1_END (23)
#define SOC_NPUCRG_PEREN1_gt_clk_ppll5_npu_tp_START (24)
#define SOC_NPUCRG_PEREN1_gt_clk_ppll5_npu_tp_END (24)
#define SOC_NPUCRG_PEREN1_gt_clk_tcxo_START (25)
#define SOC_NPUCRG_PEREN1_gt_clk_tcxo_END (25)
#define SOC_NPUCRG_PEREN1_gt_pclk_npu_easc0_START (26)
#define SOC_NPUCRG_PEREN1_gt_pclk_npu_easc0_END (26)
#define SOC_NPUCRG_PEREN1_gt_clk_npu_loadmonitor_START (27)
#define SOC_NPUCRG_PEREN1_gt_clk_npu_loadmonitor_END (27)
#define SOC_NPUCRG_PEREN1_gt_pclk_npu_loadmonitor_START (28)
#define SOC_NPUCRG_PEREN1_gt_pclk_npu_loadmonitor_END (28)
#define SOC_NPUCRG_PEREN1_gt_pclk_npu_idsc_START (29)
#define SOC_NPUCRG_PEREN1_gt_pclk_npu_idsc_END (29)
#define SOC_NPUCRG_PEREN1_gt_pclk_aicore1_smmu_START (30)
#define SOC_NPUCRG_PEREN1_gt_pclk_aicore1_smmu_END (30)
#define SOC_NPUCRG_PEREN1_gt_pclk_npu_hw_exp_irq_ns_START (31)
#define SOC_NPUCRG_PEREN1_gt_pclk_npu_hw_exp_irq_ns_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_ppll5_sscg : 1;
        unsigned int gt_pclk_aicore0_smmu : 1;
        unsigned int reserved_0 : 1;
        unsigned int gt_pclk_npu_l2buf : 1;
        unsigned int gt_pclk_npu_uart : 1;
        unsigned int gt_pclk_npu_ipc_ns : 1;
        unsigned int gt_pclk_npu_ipc_s : 1;
        unsigned int gt_pclk_npu_hw_exp_irq : 1;
        unsigned int gt_pclk_autodiv_npubus : 1;
        unsigned int gt_pclk_autodiv_npubus_cfg : 1;
        unsigned int reserved_1 : 1;
        unsigned int gt_pclk_autodiv_npubus_aicore : 1;
        unsigned int gt_pclk_npu_easc3 : 1;
        unsigned int gt_pclk_npu_easc2 : 1;
        unsigned int gt_pclk_atgs : 1;
        unsigned int gt_pclk_atgm : 1;
        unsigned int gt_pclk_npu_sysdma_tiny : 1;
        unsigned int gt_pclk_npu_sysdma_lite : 1;
        unsigned int gt_pclk_sysdma_smmu0 : 1;
        unsigned int gt_pclk_npu_easc1 : 1;
        unsigned int gt_pclk_npu_tcu : 1;
        unsigned int gt_pclk_npu_pcr : 1;
        unsigned int gt_pclk_npu_dpm0 : 1;
        unsigned int gt_pclk_npu_dpm1 : 1;
        unsigned int gt_clk_ppll5_npu_tp : 1;
        unsigned int gt_clk_tcxo : 1;
        unsigned int gt_pclk_npu_easc0 : 1;
        unsigned int gt_clk_npu_loadmonitor : 1;
        unsigned int gt_pclk_npu_loadmonitor : 1;
        unsigned int gt_pclk_npu_idsc : 1;
        unsigned int gt_pclk_aicore1_smmu : 1;
        unsigned int gt_pclk_npu_hw_exp_irq_ns : 1;
    } reg;
} SOC_NPUCRG_PERDIS1_UNION;
#endif
#define SOC_NPUCRG_PERDIS1_gt_clk_ppll5_sscg_START (0)
#define SOC_NPUCRG_PERDIS1_gt_clk_ppll5_sscg_END (0)
#define SOC_NPUCRG_PERDIS1_gt_pclk_aicore0_smmu_START (1)
#define SOC_NPUCRG_PERDIS1_gt_pclk_aicore0_smmu_END (1)
#define SOC_NPUCRG_PERDIS1_gt_pclk_npu_l2buf_START (3)
#define SOC_NPUCRG_PERDIS1_gt_pclk_npu_l2buf_END (3)
#define SOC_NPUCRG_PERDIS1_gt_pclk_npu_uart_START (4)
#define SOC_NPUCRG_PERDIS1_gt_pclk_npu_uart_END (4)
#define SOC_NPUCRG_PERDIS1_gt_pclk_npu_ipc_ns_START (5)
#define SOC_NPUCRG_PERDIS1_gt_pclk_npu_ipc_ns_END (5)
#define SOC_NPUCRG_PERDIS1_gt_pclk_npu_ipc_s_START (6)
#define SOC_NPUCRG_PERDIS1_gt_pclk_npu_ipc_s_END (6)
#define SOC_NPUCRG_PERDIS1_gt_pclk_npu_hw_exp_irq_START (7)
#define SOC_NPUCRG_PERDIS1_gt_pclk_npu_hw_exp_irq_END (7)
#define SOC_NPUCRG_PERDIS1_gt_pclk_autodiv_npubus_START (8)
#define SOC_NPUCRG_PERDIS1_gt_pclk_autodiv_npubus_END (8)
#define SOC_NPUCRG_PERDIS1_gt_pclk_autodiv_npubus_cfg_START (9)
#define SOC_NPUCRG_PERDIS1_gt_pclk_autodiv_npubus_cfg_END (9)
#define SOC_NPUCRG_PERDIS1_gt_pclk_autodiv_npubus_aicore_START (11)
#define SOC_NPUCRG_PERDIS1_gt_pclk_autodiv_npubus_aicore_END (11)
#define SOC_NPUCRG_PERDIS1_gt_pclk_npu_easc3_START (12)
#define SOC_NPUCRG_PERDIS1_gt_pclk_npu_easc3_END (12)
#define SOC_NPUCRG_PERDIS1_gt_pclk_npu_easc2_START (13)
#define SOC_NPUCRG_PERDIS1_gt_pclk_npu_easc2_END (13)
#define SOC_NPUCRG_PERDIS1_gt_pclk_atgs_START (14)
#define SOC_NPUCRG_PERDIS1_gt_pclk_atgs_END (14)
#define SOC_NPUCRG_PERDIS1_gt_pclk_atgm_START (15)
#define SOC_NPUCRG_PERDIS1_gt_pclk_atgm_END (15)
#define SOC_NPUCRG_PERDIS1_gt_pclk_npu_sysdma_tiny_START (16)
#define SOC_NPUCRG_PERDIS1_gt_pclk_npu_sysdma_tiny_END (16)
#define SOC_NPUCRG_PERDIS1_gt_pclk_npu_sysdma_lite_START (17)
#define SOC_NPUCRG_PERDIS1_gt_pclk_npu_sysdma_lite_END (17)
#define SOC_NPUCRG_PERDIS1_gt_pclk_sysdma_smmu0_START (18)
#define SOC_NPUCRG_PERDIS1_gt_pclk_sysdma_smmu0_END (18)
#define SOC_NPUCRG_PERDIS1_gt_pclk_npu_easc1_START (19)
#define SOC_NPUCRG_PERDIS1_gt_pclk_npu_easc1_END (19)
#define SOC_NPUCRG_PERDIS1_gt_pclk_npu_tcu_START (20)
#define SOC_NPUCRG_PERDIS1_gt_pclk_npu_tcu_END (20)
#define SOC_NPUCRG_PERDIS1_gt_pclk_npu_pcr_START (21)
#define SOC_NPUCRG_PERDIS1_gt_pclk_npu_pcr_END (21)
#define SOC_NPUCRG_PERDIS1_gt_pclk_npu_dpm0_START (22)
#define SOC_NPUCRG_PERDIS1_gt_pclk_npu_dpm0_END (22)
#define SOC_NPUCRG_PERDIS1_gt_pclk_npu_dpm1_START (23)
#define SOC_NPUCRG_PERDIS1_gt_pclk_npu_dpm1_END (23)
#define SOC_NPUCRG_PERDIS1_gt_clk_ppll5_npu_tp_START (24)
#define SOC_NPUCRG_PERDIS1_gt_clk_ppll5_npu_tp_END (24)
#define SOC_NPUCRG_PERDIS1_gt_clk_tcxo_START (25)
#define SOC_NPUCRG_PERDIS1_gt_clk_tcxo_END (25)
#define SOC_NPUCRG_PERDIS1_gt_pclk_npu_easc0_START (26)
#define SOC_NPUCRG_PERDIS1_gt_pclk_npu_easc0_END (26)
#define SOC_NPUCRG_PERDIS1_gt_clk_npu_loadmonitor_START (27)
#define SOC_NPUCRG_PERDIS1_gt_clk_npu_loadmonitor_END (27)
#define SOC_NPUCRG_PERDIS1_gt_pclk_npu_loadmonitor_START (28)
#define SOC_NPUCRG_PERDIS1_gt_pclk_npu_loadmonitor_END (28)
#define SOC_NPUCRG_PERDIS1_gt_pclk_npu_idsc_START (29)
#define SOC_NPUCRG_PERDIS1_gt_pclk_npu_idsc_END (29)
#define SOC_NPUCRG_PERDIS1_gt_pclk_aicore1_smmu_START (30)
#define SOC_NPUCRG_PERDIS1_gt_pclk_aicore1_smmu_END (30)
#define SOC_NPUCRG_PERDIS1_gt_pclk_npu_hw_exp_irq_ns_START (31)
#define SOC_NPUCRG_PERDIS1_gt_pclk_npu_hw_exp_irq_ns_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_ppll5_sscg : 1;
        unsigned int gt_pclk_aicore0_smmu : 1;
        unsigned int reserved_0 : 1;
        unsigned int gt_pclk_npu_l2buf : 1;
        unsigned int gt_pclk_npu_uart : 1;
        unsigned int gt_pclk_npu_ipc_ns : 1;
        unsigned int gt_pclk_npu_ipc_s : 1;
        unsigned int gt_pclk_npu_hw_exp_irq : 1;
        unsigned int gt_pclk_autodiv_npubus : 1;
        unsigned int gt_pclk_autodiv_npubus_cfg : 1;
        unsigned int reserved_1 : 1;
        unsigned int gt_pclk_autodiv_npubus_aicore : 1;
        unsigned int gt_pclk_npu_easc3 : 1;
        unsigned int gt_pclk_npu_easc2 : 1;
        unsigned int gt_pclk_atgs : 1;
        unsigned int gt_pclk_atgm : 1;
        unsigned int gt_pclk_npu_sysdma_tiny : 1;
        unsigned int gt_pclk_npu_sysdma_lite : 1;
        unsigned int gt_pclk_sysdma_smmu0 : 1;
        unsigned int gt_pclk_npu_easc1 : 1;
        unsigned int gt_pclk_npu_tcu : 1;
        unsigned int gt_pclk_npu_pcr : 1;
        unsigned int gt_pclk_npu_dpm0 : 1;
        unsigned int gt_pclk_npu_dpm1 : 1;
        unsigned int gt_clk_ppll5_npu_tp : 1;
        unsigned int gt_clk_tcxo : 1;
        unsigned int gt_pclk_npu_easc0 : 1;
        unsigned int gt_clk_npu_loadmonitor : 1;
        unsigned int gt_pclk_npu_loadmonitor : 1;
        unsigned int gt_pclk_npu_idsc : 1;
        unsigned int gt_pclk_aicore1_smmu : 1;
        unsigned int gt_pclk_npu_hw_exp_irq_ns : 1;
    } reg;
} SOC_NPUCRG_PERCLKEN1_UNION;
#endif
#define SOC_NPUCRG_PERCLKEN1_gt_clk_ppll5_sscg_START (0)
#define SOC_NPUCRG_PERCLKEN1_gt_clk_ppll5_sscg_END (0)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_aicore0_smmu_START (1)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_aicore0_smmu_END (1)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_npu_l2buf_START (3)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_npu_l2buf_END (3)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_npu_uart_START (4)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_npu_uart_END (4)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_npu_ipc_ns_START (5)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_npu_ipc_ns_END (5)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_npu_ipc_s_START (6)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_npu_ipc_s_END (6)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_npu_hw_exp_irq_START (7)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_npu_hw_exp_irq_END (7)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_autodiv_npubus_START (8)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_autodiv_npubus_END (8)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_autodiv_npubus_cfg_START (9)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_autodiv_npubus_cfg_END (9)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_autodiv_npubus_aicore_START (11)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_autodiv_npubus_aicore_END (11)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_npu_easc3_START (12)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_npu_easc3_END (12)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_npu_easc2_START (13)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_npu_easc2_END (13)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_atgs_START (14)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_atgs_END (14)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_atgm_START (15)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_atgm_END (15)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_npu_sysdma_tiny_START (16)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_npu_sysdma_tiny_END (16)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_npu_sysdma_lite_START (17)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_npu_sysdma_lite_END (17)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_sysdma_smmu0_START (18)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_sysdma_smmu0_END (18)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_npu_easc1_START (19)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_npu_easc1_END (19)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_npu_tcu_START (20)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_npu_tcu_END (20)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_npu_pcr_START (21)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_npu_pcr_END (21)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_npu_dpm0_START (22)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_npu_dpm0_END (22)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_npu_dpm1_START (23)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_npu_dpm1_END (23)
#define SOC_NPUCRG_PERCLKEN1_gt_clk_ppll5_npu_tp_START (24)
#define SOC_NPUCRG_PERCLKEN1_gt_clk_ppll5_npu_tp_END (24)
#define SOC_NPUCRG_PERCLKEN1_gt_clk_tcxo_START (25)
#define SOC_NPUCRG_PERCLKEN1_gt_clk_tcxo_END (25)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_npu_easc0_START (26)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_npu_easc0_END (26)
#define SOC_NPUCRG_PERCLKEN1_gt_clk_npu_loadmonitor_START (27)
#define SOC_NPUCRG_PERCLKEN1_gt_clk_npu_loadmonitor_END (27)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_npu_loadmonitor_START (28)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_npu_loadmonitor_END (28)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_npu_idsc_START (29)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_npu_idsc_END (29)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_aicore1_smmu_START (30)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_aicore1_smmu_END (30)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_npu_hw_exp_irq_ns_START (31)
#define SOC_NPUCRG_PERCLKEN1_gt_pclk_npu_hw_exp_irq_ns_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int st_clk_ppll5_sscg : 1;
        unsigned int st_pclk_aicore0_smmu : 1;
        unsigned int reserved_0 : 1;
        unsigned int st_pclk_npu_l2buf : 1;
        unsigned int st_pclk_npu_uart : 1;
        unsigned int st_pclk_npu_ipc_ns : 1;
        unsigned int st_pclk_npu_ipc_s : 1;
        unsigned int st_pclk_npu_hw_exp_irq : 1;
        unsigned int st_pclk_autodiv_npubus : 1;
        unsigned int st_pclk_autodiv_npubus_cfg : 1;
        unsigned int reserved_1 : 1;
        unsigned int st_pclk_autodiv_npubus_aicore : 1;
        unsigned int st_pclk_npu_easc3 : 1;
        unsigned int st_pclk_npu_easc2 : 1;
        unsigned int st_pclk_atgs : 1;
        unsigned int st_pclk_atgm : 1;
        unsigned int st_pclk_npu_sysdma : 1;
        unsigned int reserved_2 : 1;
        unsigned int st_pclk_sysdma_smmu0 : 1;
        unsigned int st_pclk_npu_easc1 : 1;
        unsigned int st_pclk_npu_tcu : 1;
        unsigned int st_pclk_npu_pcr : 1;
        unsigned int st_pclk_npu_dpm0 : 1;
        unsigned int st_pclk_npu_dpm1 : 1;
        unsigned int st_clk_ppll5_npu_tp : 1;
        unsigned int st_clk_tcxo : 1;
        unsigned int st_pclk_npu_easc0 : 1;
        unsigned int st_clk_npu_loadmonitor : 1;
        unsigned int st_pclk_npu_loadmonitor : 1;
        unsigned int st_pclk_npu_idsc : 1;
        unsigned int st_pclk_aicore1_smmu : 1;
        unsigned int st_pclk_npu_hw_exp_irq_ns : 1;
    } reg;
} SOC_NPUCRG_PERSTAT1_UNION;
#endif
#define SOC_NPUCRG_PERSTAT1_st_clk_ppll5_sscg_START (0)
#define SOC_NPUCRG_PERSTAT1_st_clk_ppll5_sscg_END (0)
#define SOC_NPUCRG_PERSTAT1_st_pclk_aicore0_smmu_START (1)
#define SOC_NPUCRG_PERSTAT1_st_pclk_aicore0_smmu_END (1)
#define SOC_NPUCRG_PERSTAT1_st_pclk_npu_l2buf_START (3)
#define SOC_NPUCRG_PERSTAT1_st_pclk_npu_l2buf_END (3)
#define SOC_NPUCRG_PERSTAT1_st_pclk_npu_uart_START (4)
#define SOC_NPUCRG_PERSTAT1_st_pclk_npu_uart_END (4)
#define SOC_NPUCRG_PERSTAT1_st_pclk_npu_ipc_ns_START (5)
#define SOC_NPUCRG_PERSTAT1_st_pclk_npu_ipc_ns_END (5)
#define SOC_NPUCRG_PERSTAT1_st_pclk_npu_ipc_s_START (6)
#define SOC_NPUCRG_PERSTAT1_st_pclk_npu_ipc_s_END (6)
#define SOC_NPUCRG_PERSTAT1_st_pclk_npu_hw_exp_irq_START (7)
#define SOC_NPUCRG_PERSTAT1_st_pclk_npu_hw_exp_irq_END (7)
#define SOC_NPUCRG_PERSTAT1_st_pclk_autodiv_npubus_START (8)
#define SOC_NPUCRG_PERSTAT1_st_pclk_autodiv_npubus_END (8)
#define SOC_NPUCRG_PERSTAT1_st_pclk_autodiv_npubus_cfg_START (9)
#define SOC_NPUCRG_PERSTAT1_st_pclk_autodiv_npubus_cfg_END (9)
#define SOC_NPUCRG_PERSTAT1_st_pclk_autodiv_npubus_aicore_START (11)
#define SOC_NPUCRG_PERSTAT1_st_pclk_autodiv_npubus_aicore_END (11)
#define SOC_NPUCRG_PERSTAT1_st_pclk_npu_easc3_START (12)
#define SOC_NPUCRG_PERSTAT1_st_pclk_npu_easc3_END (12)
#define SOC_NPUCRG_PERSTAT1_st_pclk_npu_easc2_START (13)
#define SOC_NPUCRG_PERSTAT1_st_pclk_npu_easc2_END (13)
#define SOC_NPUCRG_PERSTAT1_st_pclk_atgs_START (14)
#define SOC_NPUCRG_PERSTAT1_st_pclk_atgs_END (14)
#define SOC_NPUCRG_PERSTAT1_st_pclk_atgm_START (15)
#define SOC_NPUCRG_PERSTAT1_st_pclk_atgm_END (15)
#define SOC_NPUCRG_PERSTAT1_st_pclk_npu_sysdma_START (16)
#define SOC_NPUCRG_PERSTAT1_st_pclk_npu_sysdma_END (16)
#define SOC_NPUCRG_PERSTAT1_st_pclk_sysdma_smmu0_START (18)
#define SOC_NPUCRG_PERSTAT1_st_pclk_sysdma_smmu0_END (18)
#define SOC_NPUCRG_PERSTAT1_st_pclk_npu_easc1_START (19)
#define SOC_NPUCRG_PERSTAT1_st_pclk_npu_easc1_END (19)
#define SOC_NPUCRG_PERSTAT1_st_pclk_npu_tcu_START (20)
#define SOC_NPUCRG_PERSTAT1_st_pclk_npu_tcu_END (20)
#define SOC_NPUCRG_PERSTAT1_st_pclk_npu_pcr_START (21)
#define SOC_NPUCRG_PERSTAT1_st_pclk_npu_pcr_END (21)
#define SOC_NPUCRG_PERSTAT1_st_pclk_npu_dpm0_START (22)
#define SOC_NPUCRG_PERSTAT1_st_pclk_npu_dpm0_END (22)
#define SOC_NPUCRG_PERSTAT1_st_pclk_npu_dpm1_START (23)
#define SOC_NPUCRG_PERSTAT1_st_pclk_npu_dpm1_END (23)
#define SOC_NPUCRG_PERSTAT1_st_clk_ppll5_npu_tp_START (24)
#define SOC_NPUCRG_PERSTAT1_st_clk_ppll5_npu_tp_END (24)
#define SOC_NPUCRG_PERSTAT1_st_clk_tcxo_START (25)
#define SOC_NPUCRG_PERSTAT1_st_clk_tcxo_END (25)
#define SOC_NPUCRG_PERSTAT1_st_pclk_npu_easc0_START (26)
#define SOC_NPUCRG_PERSTAT1_st_pclk_npu_easc0_END (26)
#define SOC_NPUCRG_PERSTAT1_st_clk_npu_loadmonitor_START (27)
#define SOC_NPUCRG_PERSTAT1_st_clk_npu_loadmonitor_END (27)
#define SOC_NPUCRG_PERSTAT1_st_pclk_npu_loadmonitor_START (28)
#define SOC_NPUCRG_PERSTAT1_st_pclk_npu_loadmonitor_END (28)
#define SOC_NPUCRG_PERSTAT1_st_pclk_npu_idsc_START (29)
#define SOC_NPUCRG_PERSTAT1_st_pclk_npu_idsc_END (29)
#define SOC_NPUCRG_PERSTAT1_st_pclk_aicore1_smmu_START (30)
#define SOC_NPUCRG_PERSTAT1_st_pclk_aicore1_smmu_END (30)
#define SOC_NPUCRG_PERSTAT1_st_pclk_npu_hw_exp_irq_ns_START (31)
#define SOC_NPUCRG_PERSTAT1_st_pclk_npu_hw_exp_irq_ns_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_npu_easc3 : 1;
        unsigned int gt_clk_npu_easc2 : 1;
        unsigned int gt_clk_npu_easc1 : 1;
        unsigned int gt_clk_npu_easc0 : 1;
        unsigned int gt_clk_npu_idsc : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int reserved_8 : 1;
        unsigned int reserved_9 : 1;
        unsigned int reserved_10 : 1;
        unsigned int reserved_11 : 1;
        unsigned int reserved_12 : 1;
        unsigned int reserved_13 : 1;
        unsigned int reserved_14 : 1;
        unsigned int reserved_15 : 12;
    } reg;
} SOC_NPUCRG_PEREN2_UNION;
#endif
#define SOC_NPUCRG_PEREN2_gt_clk_npu_easc3_START (0)
#define SOC_NPUCRG_PEREN2_gt_clk_npu_easc3_END (0)
#define SOC_NPUCRG_PEREN2_gt_clk_npu_easc2_START (1)
#define SOC_NPUCRG_PEREN2_gt_clk_npu_easc2_END (1)
#define SOC_NPUCRG_PEREN2_gt_clk_npu_easc1_START (2)
#define SOC_NPUCRG_PEREN2_gt_clk_npu_easc1_END (2)
#define SOC_NPUCRG_PEREN2_gt_clk_npu_easc0_START (3)
#define SOC_NPUCRG_PEREN2_gt_clk_npu_easc0_END (3)
#define SOC_NPUCRG_PEREN2_gt_clk_npu_idsc_START (4)
#define SOC_NPUCRG_PEREN2_gt_clk_npu_idsc_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_npu_easc3 : 1;
        unsigned int gt_clk_npu_easc2 : 1;
        unsigned int gt_clk_npu_easc1 : 1;
        unsigned int gt_clk_npu_easc0 : 1;
        unsigned int gt_clk_npu_idsc : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int reserved_8 : 1;
        unsigned int reserved_9 : 1;
        unsigned int reserved_10 : 1;
        unsigned int reserved_11 : 1;
        unsigned int reserved_12 : 1;
        unsigned int reserved_13 : 1;
        unsigned int reserved_14 : 1;
        unsigned int reserved_15 : 12;
    } reg;
} SOC_NPUCRG_PERDIS2_UNION;
#endif
#define SOC_NPUCRG_PERDIS2_gt_clk_npu_easc3_START (0)
#define SOC_NPUCRG_PERDIS2_gt_clk_npu_easc3_END (0)
#define SOC_NPUCRG_PERDIS2_gt_clk_npu_easc2_START (1)
#define SOC_NPUCRG_PERDIS2_gt_clk_npu_easc2_END (1)
#define SOC_NPUCRG_PERDIS2_gt_clk_npu_easc1_START (2)
#define SOC_NPUCRG_PERDIS2_gt_clk_npu_easc1_END (2)
#define SOC_NPUCRG_PERDIS2_gt_clk_npu_easc0_START (3)
#define SOC_NPUCRG_PERDIS2_gt_clk_npu_easc0_END (3)
#define SOC_NPUCRG_PERDIS2_gt_clk_npu_idsc_START (4)
#define SOC_NPUCRG_PERDIS2_gt_clk_npu_idsc_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_npu_easc3 : 1;
        unsigned int gt_clk_npu_easc2 : 1;
        unsigned int gt_clk_npu_easc1 : 1;
        unsigned int gt_clk_npu_easc0 : 1;
        unsigned int gt_clk_npu_idsc : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int reserved_8 : 1;
        unsigned int reserved_9 : 1;
        unsigned int reserved_10 : 1;
        unsigned int reserved_11 : 1;
        unsigned int reserved_12 : 1;
        unsigned int reserved_13 : 1;
        unsigned int reserved_14 : 1;
        unsigned int reserved_15 : 12;
    } reg;
} SOC_NPUCRG_PERCLKEN2_UNION;
#endif
#define SOC_NPUCRG_PERCLKEN2_gt_clk_npu_easc3_START (0)
#define SOC_NPUCRG_PERCLKEN2_gt_clk_npu_easc3_END (0)
#define SOC_NPUCRG_PERCLKEN2_gt_clk_npu_easc2_START (1)
#define SOC_NPUCRG_PERCLKEN2_gt_clk_npu_easc2_END (1)
#define SOC_NPUCRG_PERCLKEN2_gt_clk_npu_easc1_START (2)
#define SOC_NPUCRG_PERCLKEN2_gt_clk_npu_easc1_END (2)
#define SOC_NPUCRG_PERCLKEN2_gt_clk_npu_easc0_START (3)
#define SOC_NPUCRG_PERCLKEN2_gt_clk_npu_easc0_END (3)
#define SOC_NPUCRG_PERCLKEN2_gt_clk_npu_idsc_START (4)
#define SOC_NPUCRG_PERCLKEN2_gt_clk_npu_idsc_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int st_clk_npu_easc3 : 1;
        unsigned int st_clk_npu_easc2 : 1;
        unsigned int st_clk_npu_easc1 : 1;
        unsigned int st_clk_npu_easc0 : 1;
        unsigned int st_clk_npu_idsc : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int reserved_8 : 1;
        unsigned int reserved_9 : 1;
        unsigned int reserved_10 : 1;
        unsigned int reserved_11 : 1;
        unsigned int reserved_12 : 1;
        unsigned int reserved_13 : 1;
        unsigned int reserved_14 : 1;
        unsigned int reserved_15 : 12;
    } reg;
} SOC_NPUCRG_PERSTAT2_UNION;
#endif
#define SOC_NPUCRG_PERSTAT2_st_clk_npu_easc3_START (0)
#define SOC_NPUCRG_PERSTAT2_st_clk_npu_easc3_END (0)
#define SOC_NPUCRG_PERSTAT2_st_clk_npu_easc2_START (1)
#define SOC_NPUCRG_PERSTAT2_st_clk_npu_easc2_END (1)
#define SOC_NPUCRG_PERSTAT2_st_clk_npu_easc1_START (2)
#define SOC_NPUCRG_PERSTAT2_st_clk_npu_easc1_END (2)
#define SOC_NPUCRG_PERSTAT2_st_clk_npu_easc0_START (3)
#define SOC_NPUCRG_PERSTAT2_st_clk_npu_easc0_END (3)
#define SOC_NPUCRG_PERSTAT2_st_clk_npu_idsc_START (4)
#define SOC_NPUCRG_PERSTAT2_st_clk_npu_idsc_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_atclk_ts_cpu_brg : 1;
        unsigned int gt_pclkdbg_ts_cpu : 1;
        unsigned int gt_clk_ts_tidm : 1;
        unsigned int gt_clk_ts_its : 1;
        unsigned int gt_clk_ts_smmu : 1;
        unsigned int gt_clk_ts_apb : 1;
        unsigned int reserved_0 : 1;
        unsigned int gt_clk_ts_dw_bus : 1;
        unsigned int gt_clk_ts_cpu : 1;
        unsigned int gt_clk_ai_core1 : 1;
        unsigned int gt_clk_aicore1_hpm : 1;
        unsigned int gt_clk_aicore1_tidm : 1;
        unsigned int gt_clk_aicore1_pa : 1;
        unsigned int gt_atclk_ai_core1_brg : 1;
        unsigned int gt_pclkdbg_ai_core1 : 1;
        unsigned int gt_clk_noc_aicore1 : 1;
        unsigned int gt_clk_aicore1_bus_monitor : 1;
        unsigned int gt_clk_aicore1_smmu : 1;
        unsigned int gt_atclk_tiny_core_brg : 1;
        unsigned int gt_pclkdbg_tiny_core : 1;
        unsigned int gt_clk_tiny_bus_monitor : 1;
        unsigned int gt_clk_tiny_hpm : 1;
        unsigned int gt_clk_tiny_tidm : 1;
        unsigned int gt_clk_tiny_pa : 1;
        unsigned int gt_clk_tiny_apb : 1;
        unsigned int gt_clk_tiny_core : 1;
        unsigned int reserved_1 : 6;
    } reg;
} SOC_NPUCRG_PEREN3_UNION;
#endif
#define SOC_NPUCRG_PEREN3_gt_atclk_ts_cpu_brg_START (0)
#define SOC_NPUCRG_PEREN3_gt_atclk_ts_cpu_brg_END (0)
#define SOC_NPUCRG_PEREN3_gt_pclkdbg_ts_cpu_START (1)
#define SOC_NPUCRG_PEREN3_gt_pclkdbg_ts_cpu_END (1)
#define SOC_NPUCRG_PEREN3_gt_clk_ts_tidm_START (2)
#define SOC_NPUCRG_PEREN3_gt_clk_ts_tidm_END (2)
#define SOC_NPUCRG_PEREN3_gt_clk_ts_its_START (3)
#define SOC_NPUCRG_PEREN3_gt_clk_ts_its_END (3)
#define SOC_NPUCRG_PEREN3_gt_clk_ts_smmu_START (4)
#define SOC_NPUCRG_PEREN3_gt_clk_ts_smmu_END (4)
#define SOC_NPUCRG_PEREN3_gt_clk_ts_apb_START (5)
#define SOC_NPUCRG_PEREN3_gt_clk_ts_apb_END (5)
#define SOC_NPUCRG_PEREN3_gt_clk_ts_dw_bus_START (7)
#define SOC_NPUCRG_PEREN3_gt_clk_ts_dw_bus_END (7)
#define SOC_NPUCRG_PEREN3_gt_clk_ts_cpu_START (8)
#define SOC_NPUCRG_PEREN3_gt_clk_ts_cpu_END (8)
#define SOC_NPUCRG_PEREN3_gt_clk_ai_core1_START (9)
#define SOC_NPUCRG_PEREN3_gt_clk_ai_core1_END (9)
#define SOC_NPUCRG_PEREN3_gt_clk_aicore1_hpm_START (10)
#define SOC_NPUCRG_PEREN3_gt_clk_aicore1_hpm_END (10)
#define SOC_NPUCRG_PEREN3_gt_clk_aicore1_tidm_START (11)
#define SOC_NPUCRG_PEREN3_gt_clk_aicore1_tidm_END (11)
#define SOC_NPUCRG_PEREN3_gt_clk_aicore1_pa_START (12)
#define SOC_NPUCRG_PEREN3_gt_clk_aicore1_pa_END (12)
#define SOC_NPUCRG_PEREN3_gt_atclk_ai_core1_brg_START (13)
#define SOC_NPUCRG_PEREN3_gt_atclk_ai_core1_brg_END (13)
#define SOC_NPUCRG_PEREN3_gt_pclkdbg_ai_core1_START (14)
#define SOC_NPUCRG_PEREN3_gt_pclkdbg_ai_core1_END (14)
#define SOC_NPUCRG_PEREN3_gt_clk_noc_aicore1_START (15)
#define SOC_NPUCRG_PEREN3_gt_clk_noc_aicore1_END (15)
#define SOC_NPUCRG_PEREN3_gt_clk_aicore1_bus_monitor_START (16)
#define SOC_NPUCRG_PEREN3_gt_clk_aicore1_bus_monitor_END (16)
#define SOC_NPUCRG_PEREN3_gt_clk_aicore1_smmu_START (17)
#define SOC_NPUCRG_PEREN3_gt_clk_aicore1_smmu_END (17)
#define SOC_NPUCRG_PEREN3_gt_atclk_tiny_core_brg_START (18)
#define SOC_NPUCRG_PEREN3_gt_atclk_tiny_core_brg_END (18)
#define SOC_NPUCRG_PEREN3_gt_pclkdbg_tiny_core_START (19)
#define SOC_NPUCRG_PEREN3_gt_pclkdbg_tiny_core_END (19)
#define SOC_NPUCRG_PEREN3_gt_clk_tiny_bus_monitor_START (20)
#define SOC_NPUCRG_PEREN3_gt_clk_tiny_bus_monitor_END (20)
#define SOC_NPUCRG_PEREN3_gt_clk_tiny_hpm_START (21)
#define SOC_NPUCRG_PEREN3_gt_clk_tiny_hpm_END (21)
#define SOC_NPUCRG_PEREN3_gt_clk_tiny_tidm_START (22)
#define SOC_NPUCRG_PEREN3_gt_clk_tiny_tidm_END (22)
#define SOC_NPUCRG_PEREN3_gt_clk_tiny_pa_START (23)
#define SOC_NPUCRG_PEREN3_gt_clk_tiny_pa_END (23)
#define SOC_NPUCRG_PEREN3_gt_clk_tiny_apb_START (24)
#define SOC_NPUCRG_PEREN3_gt_clk_tiny_apb_END (24)
#define SOC_NPUCRG_PEREN3_gt_clk_tiny_core_START (25)
#define SOC_NPUCRG_PEREN3_gt_clk_tiny_core_END (25)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_atclk_ts_cpu_brg : 1;
        unsigned int gt_pclkdbg_ts_cpu : 1;
        unsigned int gt_clk_ts_tidm : 1;
        unsigned int gt_clk_ts_its : 1;
        unsigned int gt_clk_ts_smmu : 1;
        unsigned int gt_clk_ts_apb : 1;
        unsigned int reserved_0 : 1;
        unsigned int gt_clk_ts_dw_bus : 1;
        unsigned int gt_clk_ts_cpu : 1;
        unsigned int gt_clk_ai_core1 : 1;
        unsigned int gt_clk_aicore1_hpm : 1;
        unsigned int gt_clk_aicore1_tidm : 1;
        unsigned int gt_clk_aicore1_pa : 1;
        unsigned int gt_atclk_ai_core1_brg : 1;
        unsigned int gt_pclkdbg_ai_core1 : 1;
        unsigned int gt_clk_noc_aicore1 : 1;
        unsigned int gt_clk_aicore1_bus_monitor : 1;
        unsigned int gt_clk_aicore1_smmu : 1;
        unsigned int gt_atclk_tiny_core_brg : 1;
        unsigned int gt_pclkdbg_tiny_core : 1;
        unsigned int gt_clk_tiny_bus_monitor : 1;
        unsigned int gt_clk_tiny_hpm : 1;
        unsigned int gt_clk_tiny_tidm : 1;
        unsigned int gt_clk_tiny_pa : 1;
        unsigned int gt_clk_tiny_apb : 1;
        unsigned int gt_clk_tiny_core : 1;
        unsigned int reserved_1 : 6;
    } reg;
} SOC_NPUCRG_PERDIS3_UNION;
#endif
#define SOC_NPUCRG_PERDIS3_gt_atclk_ts_cpu_brg_START (0)
#define SOC_NPUCRG_PERDIS3_gt_atclk_ts_cpu_brg_END (0)
#define SOC_NPUCRG_PERDIS3_gt_pclkdbg_ts_cpu_START (1)
#define SOC_NPUCRG_PERDIS3_gt_pclkdbg_ts_cpu_END (1)
#define SOC_NPUCRG_PERDIS3_gt_clk_ts_tidm_START (2)
#define SOC_NPUCRG_PERDIS3_gt_clk_ts_tidm_END (2)
#define SOC_NPUCRG_PERDIS3_gt_clk_ts_its_START (3)
#define SOC_NPUCRG_PERDIS3_gt_clk_ts_its_END (3)
#define SOC_NPUCRG_PERDIS3_gt_clk_ts_smmu_START (4)
#define SOC_NPUCRG_PERDIS3_gt_clk_ts_smmu_END (4)
#define SOC_NPUCRG_PERDIS3_gt_clk_ts_apb_START (5)
#define SOC_NPUCRG_PERDIS3_gt_clk_ts_apb_END (5)
#define SOC_NPUCRG_PERDIS3_gt_clk_ts_dw_bus_START (7)
#define SOC_NPUCRG_PERDIS3_gt_clk_ts_dw_bus_END (7)
#define SOC_NPUCRG_PERDIS3_gt_clk_ts_cpu_START (8)
#define SOC_NPUCRG_PERDIS3_gt_clk_ts_cpu_END (8)
#define SOC_NPUCRG_PERDIS3_gt_clk_ai_core1_START (9)
#define SOC_NPUCRG_PERDIS3_gt_clk_ai_core1_END (9)
#define SOC_NPUCRG_PERDIS3_gt_clk_aicore1_hpm_START (10)
#define SOC_NPUCRG_PERDIS3_gt_clk_aicore1_hpm_END (10)
#define SOC_NPUCRG_PERDIS3_gt_clk_aicore1_tidm_START (11)
#define SOC_NPUCRG_PERDIS3_gt_clk_aicore1_tidm_END (11)
#define SOC_NPUCRG_PERDIS3_gt_clk_aicore1_pa_START (12)
#define SOC_NPUCRG_PERDIS3_gt_clk_aicore1_pa_END (12)
#define SOC_NPUCRG_PERDIS3_gt_atclk_ai_core1_brg_START (13)
#define SOC_NPUCRG_PERDIS3_gt_atclk_ai_core1_brg_END (13)
#define SOC_NPUCRG_PERDIS3_gt_pclkdbg_ai_core1_START (14)
#define SOC_NPUCRG_PERDIS3_gt_pclkdbg_ai_core1_END (14)
#define SOC_NPUCRG_PERDIS3_gt_clk_noc_aicore1_START (15)
#define SOC_NPUCRG_PERDIS3_gt_clk_noc_aicore1_END (15)
#define SOC_NPUCRG_PERDIS3_gt_clk_aicore1_bus_monitor_START (16)
#define SOC_NPUCRG_PERDIS3_gt_clk_aicore1_bus_monitor_END (16)
#define SOC_NPUCRG_PERDIS3_gt_clk_aicore1_smmu_START (17)
#define SOC_NPUCRG_PERDIS3_gt_clk_aicore1_smmu_END (17)
#define SOC_NPUCRG_PERDIS3_gt_atclk_tiny_core_brg_START (18)
#define SOC_NPUCRG_PERDIS3_gt_atclk_tiny_core_brg_END (18)
#define SOC_NPUCRG_PERDIS3_gt_pclkdbg_tiny_core_START (19)
#define SOC_NPUCRG_PERDIS3_gt_pclkdbg_tiny_core_END (19)
#define SOC_NPUCRG_PERDIS3_gt_clk_tiny_bus_monitor_START (20)
#define SOC_NPUCRG_PERDIS3_gt_clk_tiny_bus_monitor_END (20)
#define SOC_NPUCRG_PERDIS3_gt_clk_tiny_hpm_START (21)
#define SOC_NPUCRG_PERDIS3_gt_clk_tiny_hpm_END (21)
#define SOC_NPUCRG_PERDIS3_gt_clk_tiny_tidm_START (22)
#define SOC_NPUCRG_PERDIS3_gt_clk_tiny_tidm_END (22)
#define SOC_NPUCRG_PERDIS3_gt_clk_tiny_pa_START (23)
#define SOC_NPUCRG_PERDIS3_gt_clk_tiny_pa_END (23)
#define SOC_NPUCRG_PERDIS3_gt_clk_tiny_apb_START (24)
#define SOC_NPUCRG_PERDIS3_gt_clk_tiny_apb_END (24)
#define SOC_NPUCRG_PERDIS3_gt_clk_tiny_core_START (25)
#define SOC_NPUCRG_PERDIS3_gt_clk_tiny_core_END (25)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_atclk_ts_cpu_brg : 1;
        unsigned int gt_pclkdbg_ts_cpu : 1;
        unsigned int gt_clk_ts_tidm : 1;
        unsigned int gt_clk_ts_its : 1;
        unsigned int gt_clk_ts_smmu : 1;
        unsigned int gt_clk_ts_apb : 1;
        unsigned int reserved_0 : 1;
        unsigned int gt_clk_ts_dw_bus : 1;
        unsigned int gt_clk_ts_cpu : 1;
        unsigned int gt_clk_ai_core1 : 1;
        unsigned int gt_clk_aicore1_hpm : 1;
        unsigned int gt_clk_aicore1_tidm : 1;
        unsigned int gt_clk_aicore1_pa : 1;
        unsigned int gt_atclk_ai_core1_brg : 1;
        unsigned int gt_pclkdbg_ai_core1 : 1;
        unsigned int gt_clk_noc_aicore1 : 1;
        unsigned int gt_clk_aicore1_bus_monitor : 1;
        unsigned int gt_clk_aicore1_smmu : 1;
        unsigned int gt_atclk_tiny_core_brg : 1;
        unsigned int gt_pclkdbg_tiny_core : 1;
        unsigned int gt_clk_tiny_bus_monitor : 1;
        unsigned int gt_clk_tiny_hpm : 1;
        unsigned int gt_clk_tiny_tidm : 1;
        unsigned int gt_clk_tiny_pa : 1;
        unsigned int gt_clk_tiny_apb : 1;
        unsigned int gt_clk_tiny_core : 1;
        unsigned int reserved_1 : 6;
    } reg;
} SOC_NPUCRG_PERCLKEN3_UNION;
#endif
#define SOC_NPUCRG_PERCLKEN3_gt_atclk_ts_cpu_brg_START (0)
#define SOC_NPUCRG_PERCLKEN3_gt_atclk_ts_cpu_brg_END (0)
#define SOC_NPUCRG_PERCLKEN3_gt_pclkdbg_ts_cpu_START (1)
#define SOC_NPUCRG_PERCLKEN3_gt_pclkdbg_ts_cpu_END (1)
#define SOC_NPUCRG_PERCLKEN3_gt_clk_ts_tidm_START (2)
#define SOC_NPUCRG_PERCLKEN3_gt_clk_ts_tidm_END (2)
#define SOC_NPUCRG_PERCLKEN3_gt_clk_ts_its_START (3)
#define SOC_NPUCRG_PERCLKEN3_gt_clk_ts_its_END (3)
#define SOC_NPUCRG_PERCLKEN3_gt_clk_ts_smmu_START (4)
#define SOC_NPUCRG_PERCLKEN3_gt_clk_ts_smmu_END (4)
#define SOC_NPUCRG_PERCLKEN3_gt_clk_ts_apb_START (5)
#define SOC_NPUCRG_PERCLKEN3_gt_clk_ts_apb_END (5)
#define SOC_NPUCRG_PERCLKEN3_gt_clk_ts_dw_bus_START (7)
#define SOC_NPUCRG_PERCLKEN3_gt_clk_ts_dw_bus_END (7)
#define SOC_NPUCRG_PERCLKEN3_gt_clk_ts_cpu_START (8)
#define SOC_NPUCRG_PERCLKEN3_gt_clk_ts_cpu_END (8)
#define SOC_NPUCRG_PERCLKEN3_gt_clk_ai_core1_START (9)
#define SOC_NPUCRG_PERCLKEN3_gt_clk_ai_core1_END (9)
#define SOC_NPUCRG_PERCLKEN3_gt_clk_aicore1_hpm_START (10)
#define SOC_NPUCRG_PERCLKEN3_gt_clk_aicore1_hpm_END (10)
#define SOC_NPUCRG_PERCLKEN3_gt_clk_aicore1_tidm_START (11)
#define SOC_NPUCRG_PERCLKEN3_gt_clk_aicore1_tidm_END (11)
#define SOC_NPUCRG_PERCLKEN3_gt_clk_aicore1_pa_START (12)
#define SOC_NPUCRG_PERCLKEN3_gt_clk_aicore1_pa_END (12)
#define SOC_NPUCRG_PERCLKEN3_gt_atclk_ai_core1_brg_START (13)
#define SOC_NPUCRG_PERCLKEN3_gt_atclk_ai_core1_brg_END (13)
#define SOC_NPUCRG_PERCLKEN3_gt_pclkdbg_ai_core1_START (14)
#define SOC_NPUCRG_PERCLKEN3_gt_pclkdbg_ai_core1_END (14)
#define SOC_NPUCRG_PERCLKEN3_gt_clk_noc_aicore1_START (15)
#define SOC_NPUCRG_PERCLKEN3_gt_clk_noc_aicore1_END (15)
#define SOC_NPUCRG_PERCLKEN3_gt_clk_aicore1_bus_monitor_START (16)
#define SOC_NPUCRG_PERCLKEN3_gt_clk_aicore1_bus_monitor_END (16)
#define SOC_NPUCRG_PERCLKEN3_gt_clk_aicore1_smmu_START (17)
#define SOC_NPUCRG_PERCLKEN3_gt_clk_aicore1_smmu_END (17)
#define SOC_NPUCRG_PERCLKEN3_gt_atclk_tiny_core_brg_START (18)
#define SOC_NPUCRG_PERCLKEN3_gt_atclk_tiny_core_brg_END (18)
#define SOC_NPUCRG_PERCLKEN3_gt_pclkdbg_tiny_core_START (19)
#define SOC_NPUCRG_PERCLKEN3_gt_pclkdbg_tiny_core_END (19)
#define SOC_NPUCRG_PERCLKEN3_gt_clk_tiny_bus_monitor_START (20)
#define SOC_NPUCRG_PERCLKEN3_gt_clk_tiny_bus_monitor_END (20)
#define SOC_NPUCRG_PERCLKEN3_gt_clk_tiny_hpm_START (21)
#define SOC_NPUCRG_PERCLKEN3_gt_clk_tiny_hpm_END (21)
#define SOC_NPUCRG_PERCLKEN3_gt_clk_tiny_tidm_START (22)
#define SOC_NPUCRG_PERCLKEN3_gt_clk_tiny_tidm_END (22)
#define SOC_NPUCRG_PERCLKEN3_gt_clk_tiny_pa_START (23)
#define SOC_NPUCRG_PERCLKEN3_gt_clk_tiny_pa_END (23)
#define SOC_NPUCRG_PERCLKEN3_gt_clk_tiny_apb_START (24)
#define SOC_NPUCRG_PERCLKEN3_gt_clk_tiny_apb_END (24)
#define SOC_NPUCRG_PERCLKEN3_gt_clk_tiny_core_START (25)
#define SOC_NPUCRG_PERCLKEN3_gt_clk_tiny_core_END (25)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int st_atclk_ts_cpu_brg : 1;
        unsigned int st_pclkdbg_ts_cpu : 1;
        unsigned int st_clk_ts_tidm : 1;
        unsigned int st_clk_ts_its : 1;
        unsigned int st_clk_ts_smmu : 1;
        unsigned int st_clk_ts_apb : 1;
        unsigned int reserved_0 : 1;
        unsigned int st_clk_ts_dw_bus : 1;
        unsigned int st_clk_ts_cpu : 1;
        unsigned int st_clk_ai_core1 : 1;
        unsigned int st_clk_aicore1_hpm : 1;
        unsigned int st_clk_aicore1_tidm : 1;
        unsigned int st_clk_aicore1_pa : 1;
        unsigned int st_atclk_ai_core1_brg : 1;
        unsigned int st_pclkdbg_ai_core1 : 1;
        unsigned int st_clk_noc_aicore1 : 1;
        unsigned int st_clk_aicore1_bus_monitor : 1;
        unsigned int st_clk_aicore1_smmu : 1;
        unsigned int st_atclk_tiny_core_brg : 1;
        unsigned int st_pclkdbg_tiny_core : 1;
        unsigned int st_clk_tiny_bus_monitor : 1;
        unsigned int st_clk_tiny_hpm : 1;
        unsigned int st_clk_tiny_tidm : 1;
        unsigned int st_clk_tiny_pa : 1;
        unsigned int st_clk_tiny_apb : 1;
        unsigned int st_clk_tiny_core : 1;
        unsigned int reserved_1 : 6;
    } reg;
} SOC_NPUCRG_PERSTAT3_UNION;
#endif
#define SOC_NPUCRG_PERSTAT3_st_atclk_ts_cpu_brg_START (0)
#define SOC_NPUCRG_PERSTAT3_st_atclk_ts_cpu_brg_END (0)
#define SOC_NPUCRG_PERSTAT3_st_pclkdbg_ts_cpu_START (1)
#define SOC_NPUCRG_PERSTAT3_st_pclkdbg_ts_cpu_END (1)
#define SOC_NPUCRG_PERSTAT3_st_clk_ts_tidm_START (2)
#define SOC_NPUCRG_PERSTAT3_st_clk_ts_tidm_END (2)
#define SOC_NPUCRG_PERSTAT3_st_clk_ts_its_START (3)
#define SOC_NPUCRG_PERSTAT3_st_clk_ts_its_END (3)
#define SOC_NPUCRG_PERSTAT3_st_clk_ts_smmu_START (4)
#define SOC_NPUCRG_PERSTAT3_st_clk_ts_smmu_END (4)
#define SOC_NPUCRG_PERSTAT3_st_clk_ts_apb_START (5)
#define SOC_NPUCRG_PERSTAT3_st_clk_ts_apb_END (5)
#define SOC_NPUCRG_PERSTAT3_st_clk_ts_dw_bus_START (7)
#define SOC_NPUCRG_PERSTAT3_st_clk_ts_dw_bus_END (7)
#define SOC_NPUCRG_PERSTAT3_st_clk_ts_cpu_START (8)
#define SOC_NPUCRG_PERSTAT3_st_clk_ts_cpu_END (8)
#define SOC_NPUCRG_PERSTAT3_st_clk_ai_core1_START (9)
#define SOC_NPUCRG_PERSTAT3_st_clk_ai_core1_END (9)
#define SOC_NPUCRG_PERSTAT3_st_clk_aicore1_hpm_START (10)
#define SOC_NPUCRG_PERSTAT3_st_clk_aicore1_hpm_END (10)
#define SOC_NPUCRG_PERSTAT3_st_clk_aicore1_tidm_START (11)
#define SOC_NPUCRG_PERSTAT3_st_clk_aicore1_tidm_END (11)
#define SOC_NPUCRG_PERSTAT3_st_clk_aicore1_pa_START (12)
#define SOC_NPUCRG_PERSTAT3_st_clk_aicore1_pa_END (12)
#define SOC_NPUCRG_PERSTAT3_st_atclk_ai_core1_brg_START (13)
#define SOC_NPUCRG_PERSTAT3_st_atclk_ai_core1_brg_END (13)
#define SOC_NPUCRG_PERSTAT3_st_pclkdbg_ai_core1_START (14)
#define SOC_NPUCRG_PERSTAT3_st_pclkdbg_ai_core1_END (14)
#define SOC_NPUCRG_PERSTAT3_st_clk_noc_aicore1_START (15)
#define SOC_NPUCRG_PERSTAT3_st_clk_noc_aicore1_END (15)
#define SOC_NPUCRG_PERSTAT3_st_clk_aicore1_bus_monitor_START (16)
#define SOC_NPUCRG_PERSTAT3_st_clk_aicore1_bus_monitor_END (16)
#define SOC_NPUCRG_PERSTAT3_st_clk_aicore1_smmu_START (17)
#define SOC_NPUCRG_PERSTAT3_st_clk_aicore1_smmu_END (17)
#define SOC_NPUCRG_PERSTAT3_st_atclk_tiny_core_brg_START (18)
#define SOC_NPUCRG_PERSTAT3_st_atclk_tiny_core_brg_END (18)
#define SOC_NPUCRG_PERSTAT3_st_pclkdbg_tiny_core_START (19)
#define SOC_NPUCRG_PERSTAT3_st_pclkdbg_tiny_core_END (19)
#define SOC_NPUCRG_PERSTAT3_st_clk_tiny_bus_monitor_START (20)
#define SOC_NPUCRG_PERSTAT3_st_clk_tiny_bus_monitor_END (20)
#define SOC_NPUCRG_PERSTAT3_st_clk_tiny_hpm_START (21)
#define SOC_NPUCRG_PERSTAT3_st_clk_tiny_hpm_END (21)
#define SOC_NPUCRG_PERSTAT3_st_clk_tiny_tidm_START (22)
#define SOC_NPUCRG_PERSTAT3_st_clk_tiny_tidm_END (22)
#define SOC_NPUCRG_PERSTAT3_st_clk_tiny_pa_START (23)
#define SOC_NPUCRG_PERSTAT3_st_clk_tiny_pa_END (23)
#define SOC_NPUCRG_PERSTAT3_st_clk_tiny_apb_START (24)
#define SOC_NPUCRG_PERSTAT3_st_clk_tiny_apb_END (24)
#define SOC_NPUCRG_PERSTAT3_st_clk_tiny_core_START (25)
#define SOC_NPUCRG_PERSTAT3_st_clk_tiny_core_END (25)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_rst_ai_core0 : 1;
        unsigned int reserved_0 : 1;
        unsigned int ip_rst_pdbg_ts_cpu : 1;
        unsigned int reserved_1 : 1;
        unsigned int ip_rst_aicore0_smmu : 1;
        unsigned int reserved_2 : 1;
        unsigned int ip_rst_pdbg_sysdma : 1;
        unsigned int ip_rst_npu2dmss_adb_slv : 1;
        unsigned int ip_rst_pdbg_ai_core0 : 1;
        unsigned int reserved_3 : 1;
        unsigned int ip_rst_npu_sysdma : 1;
        unsigned int ip_rst_npu_tcu : 1;
        unsigned int reserved_4 : 1;
        unsigned int ip_rst_sysdma_smmu0 : 1;
        unsigned int ip_rst_ts_tidm : 1;
        unsigned int ip_rst_ts_its : 1;
        unsigned int reserved_5 : 1;
        unsigned int ip_rst_ts_subsys : 1;
        unsigned int ip_rst_ts_crg : 1;
        unsigned int ip_rst_ts_smmu : 1;
        unsigned int ip_rst_noc_tiny : 1;
        unsigned int ip_rst_npu_l2buf : 1;
        unsigned int ip_rst_noc_ts_cfg : 1;
        unsigned int ip_rst_noc_ts : 1;
        unsigned int ip_rst_npu_cssys : 1;
        unsigned int ip_rst_noc_l2buf : 1;
        unsigned int ip_rst_noc_tcu : 1;
        unsigned int ip_rst_noc_sysdma : 1;
        unsigned int ip_rst_noc_aicore0 : 1;
        unsigned int ip_rst_npu_hpm : 1;
        unsigned int ip_rst_npu_tidm : 1;
        unsigned int ip_rst_npu_pa : 1;
    } reg;
} SOC_NPUCRG_PERRSTEN0_UNION;
#endif
#define SOC_NPUCRG_PERRSTEN0_ip_rst_ai_core0_START (0)
#define SOC_NPUCRG_PERRSTEN0_ip_rst_ai_core0_END (0)
#define SOC_NPUCRG_PERRSTEN0_ip_rst_pdbg_ts_cpu_START (2)
#define SOC_NPUCRG_PERRSTEN0_ip_rst_pdbg_ts_cpu_END (2)
#define SOC_NPUCRG_PERRSTEN0_ip_rst_aicore0_smmu_START (4)
#define SOC_NPUCRG_PERRSTEN0_ip_rst_aicore0_smmu_END (4)
#define SOC_NPUCRG_PERRSTEN0_ip_rst_pdbg_sysdma_START (6)
#define SOC_NPUCRG_PERRSTEN0_ip_rst_pdbg_sysdma_END (6)
#define SOC_NPUCRG_PERRSTEN0_ip_rst_npu2dmss_adb_slv_START (7)
#define SOC_NPUCRG_PERRSTEN0_ip_rst_npu2dmss_adb_slv_END (7)
#define SOC_NPUCRG_PERRSTEN0_ip_rst_pdbg_ai_core0_START (8)
#define SOC_NPUCRG_PERRSTEN0_ip_rst_pdbg_ai_core0_END (8)
#define SOC_NPUCRG_PERRSTEN0_ip_rst_npu_sysdma_START (10)
#define SOC_NPUCRG_PERRSTEN0_ip_rst_npu_sysdma_END (10)
#define SOC_NPUCRG_PERRSTEN0_ip_rst_npu_tcu_START (11)
#define SOC_NPUCRG_PERRSTEN0_ip_rst_npu_tcu_END (11)
#define SOC_NPUCRG_PERRSTEN0_ip_rst_sysdma_smmu0_START (13)
#define SOC_NPUCRG_PERRSTEN0_ip_rst_sysdma_smmu0_END (13)
#define SOC_NPUCRG_PERRSTEN0_ip_rst_ts_tidm_START (14)
#define SOC_NPUCRG_PERRSTEN0_ip_rst_ts_tidm_END (14)
#define SOC_NPUCRG_PERRSTEN0_ip_rst_ts_its_START (15)
#define SOC_NPUCRG_PERRSTEN0_ip_rst_ts_its_END (15)
#define SOC_NPUCRG_PERRSTEN0_ip_rst_ts_subsys_START (17)
#define SOC_NPUCRG_PERRSTEN0_ip_rst_ts_subsys_END (17)
#define SOC_NPUCRG_PERRSTEN0_ip_rst_ts_crg_START (18)
#define SOC_NPUCRG_PERRSTEN0_ip_rst_ts_crg_END (18)
#define SOC_NPUCRG_PERRSTEN0_ip_rst_ts_smmu_START (19)
#define SOC_NPUCRG_PERRSTEN0_ip_rst_ts_smmu_END (19)
#define SOC_NPUCRG_PERRSTEN0_ip_rst_noc_tiny_START (20)
#define SOC_NPUCRG_PERRSTEN0_ip_rst_noc_tiny_END (20)
#define SOC_NPUCRG_PERRSTEN0_ip_rst_npu_l2buf_START (21)
#define SOC_NPUCRG_PERRSTEN0_ip_rst_npu_l2buf_END (21)
#define SOC_NPUCRG_PERRSTEN0_ip_rst_noc_ts_cfg_START (22)
#define SOC_NPUCRG_PERRSTEN0_ip_rst_noc_ts_cfg_END (22)
#define SOC_NPUCRG_PERRSTEN0_ip_rst_noc_ts_START (23)
#define SOC_NPUCRG_PERRSTEN0_ip_rst_noc_ts_END (23)
#define SOC_NPUCRG_PERRSTEN0_ip_rst_npu_cssys_START (24)
#define SOC_NPUCRG_PERRSTEN0_ip_rst_npu_cssys_END (24)
#define SOC_NPUCRG_PERRSTEN0_ip_rst_noc_l2buf_START (25)
#define SOC_NPUCRG_PERRSTEN0_ip_rst_noc_l2buf_END (25)
#define SOC_NPUCRG_PERRSTEN0_ip_rst_noc_tcu_START (26)
#define SOC_NPUCRG_PERRSTEN0_ip_rst_noc_tcu_END (26)
#define SOC_NPUCRG_PERRSTEN0_ip_rst_noc_sysdma_START (27)
#define SOC_NPUCRG_PERRSTEN0_ip_rst_noc_sysdma_END (27)
#define SOC_NPUCRG_PERRSTEN0_ip_rst_noc_aicore0_START (28)
#define SOC_NPUCRG_PERRSTEN0_ip_rst_noc_aicore0_END (28)
#define SOC_NPUCRG_PERRSTEN0_ip_rst_npu_hpm_START (29)
#define SOC_NPUCRG_PERRSTEN0_ip_rst_npu_hpm_END (29)
#define SOC_NPUCRG_PERRSTEN0_ip_rst_npu_tidm_START (30)
#define SOC_NPUCRG_PERRSTEN0_ip_rst_npu_tidm_END (30)
#define SOC_NPUCRG_PERRSTEN0_ip_rst_npu_pa_START (31)
#define SOC_NPUCRG_PERRSTEN0_ip_rst_npu_pa_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_rst_ai_core0 : 1;
        unsigned int reserved_0 : 1;
        unsigned int ip_rst_pdbg_ts_cpu : 1;
        unsigned int reserved_1 : 1;
        unsigned int ip_rst_aicore0_smmu : 1;
        unsigned int reserved_2 : 1;
        unsigned int ip_rst_pdbg_sysdma : 1;
        unsigned int ip_rst_npu2dmss_adb_slv : 1;
        unsigned int ip_rst_pdbg_ai_core0 : 1;
        unsigned int reserved_3 : 1;
        unsigned int ip_rst_npu_sysdma : 1;
        unsigned int ip_rst_npu_tcu : 1;
        unsigned int reserved_4 : 1;
        unsigned int ip_rst_sysdma_smmu0 : 1;
        unsigned int ip_rst_ts_tidm : 1;
        unsigned int ip_rst_ts_its : 1;
        unsigned int reserved_5 : 1;
        unsigned int ip_rst_ts_subsys : 1;
        unsigned int ip_rst_ts_crg : 1;
        unsigned int ip_rst_ts_smmu : 1;
        unsigned int ip_rst_noc_tiny : 1;
        unsigned int ip_rst_npu_l2buf : 1;
        unsigned int ip_rst_noc_ts_cfg : 1;
        unsigned int ip_rst_noc_ts : 1;
        unsigned int ip_rst_npu_cssys : 1;
        unsigned int ip_rst_noc_l2buf : 1;
        unsigned int ip_rst_noc_tcu : 1;
        unsigned int ip_rst_noc_sysdma : 1;
        unsigned int ip_rst_noc_aicore0 : 1;
        unsigned int ip_rst_npu_hpm : 1;
        unsigned int ip_rst_npu_tidm : 1;
        unsigned int ip_rst_npu_pa : 1;
    } reg;
} SOC_NPUCRG_PERRSTDIS0_UNION;
#endif
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_ai_core0_START (0)
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_ai_core0_END (0)
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_pdbg_ts_cpu_START (2)
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_pdbg_ts_cpu_END (2)
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_aicore0_smmu_START (4)
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_aicore0_smmu_END (4)
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_pdbg_sysdma_START (6)
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_pdbg_sysdma_END (6)
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_npu2dmss_adb_slv_START (7)
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_npu2dmss_adb_slv_END (7)
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_pdbg_ai_core0_START (8)
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_pdbg_ai_core0_END (8)
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_npu_sysdma_START (10)
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_npu_sysdma_END (10)
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_npu_tcu_START (11)
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_npu_tcu_END (11)
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_sysdma_smmu0_START (13)
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_sysdma_smmu0_END (13)
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_ts_tidm_START (14)
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_ts_tidm_END (14)
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_ts_its_START (15)
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_ts_its_END (15)
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_ts_subsys_START (17)
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_ts_subsys_END (17)
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_ts_crg_START (18)
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_ts_crg_END (18)
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_ts_smmu_START (19)
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_ts_smmu_END (19)
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_noc_tiny_START (20)
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_noc_tiny_END (20)
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_npu_l2buf_START (21)
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_npu_l2buf_END (21)
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_noc_ts_cfg_START (22)
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_noc_ts_cfg_END (22)
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_noc_ts_START (23)
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_noc_ts_END (23)
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_npu_cssys_START (24)
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_npu_cssys_END (24)
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_noc_l2buf_START (25)
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_noc_l2buf_END (25)
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_noc_tcu_START (26)
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_noc_tcu_END (26)
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_noc_sysdma_START (27)
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_noc_sysdma_END (27)
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_noc_aicore0_START (28)
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_noc_aicore0_END (28)
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_npu_hpm_START (29)
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_npu_hpm_END (29)
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_npu_tidm_START (30)
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_npu_tidm_END (30)
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_npu_pa_START (31)
#define SOC_NPUCRG_PERRSTDIS0_ip_rst_npu_pa_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_rst_ai_core0 : 1;
        unsigned int reserved_0 : 1;
        unsigned int ip_rst_pdbg_ts_cpu : 1;
        unsigned int reserved_1 : 1;
        unsigned int ip_rst_aicore0_smmu : 1;
        unsigned int reserved_2 : 1;
        unsigned int ip_rst_pdbg_sysdma : 1;
        unsigned int ip_rst_npu2dmss_adb_slv : 1;
        unsigned int ip_rst_pdbg_ai_core0 : 1;
        unsigned int reserved_3 : 1;
        unsigned int ip_rst_npu_sysdma : 1;
        unsigned int ip_rst_npu_tcu : 1;
        unsigned int reserved_4 : 1;
        unsigned int ip_rst_sysdma_smmu0 : 1;
        unsigned int ip_rst_ts_tidm : 1;
        unsigned int ip_rst_ts_its : 1;
        unsigned int reserved_5 : 1;
        unsigned int ip_rst_ts_subsys : 1;
        unsigned int ip_rst_ts_crg : 1;
        unsigned int ip_rst_ts_smmu : 1;
        unsigned int ip_rst_noc_tiny : 1;
        unsigned int ip_rst_npu_l2buf : 1;
        unsigned int ip_rst_noc_ts_cfg : 1;
        unsigned int ip_rst_noc_ts : 1;
        unsigned int ip_rst_npu_cssys : 1;
        unsigned int ip_rst_noc_l2buf : 1;
        unsigned int ip_rst_noc_tcu : 1;
        unsigned int ip_rst_noc_sysdma : 1;
        unsigned int ip_rst_noc_aicore0 : 1;
        unsigned int ip_rst_npu_hpm : 1;
        unsigned int ip_rst_npu_tidm : 1;
        unsigned int ip_rst_npu_pa : 1;
    } reg;
} SOC_NPUCRG_PERRSTSTAT0_UNION;
#endif
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_ai_core0_START (0)
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_ai_core0_END (0)
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_pdbg_ts_cpu_START (2)
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_pdbg_ts_cpu_END (2)
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_aicore0_smmu_START (4)
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_aicore0_smmu_END (4)
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_pdbg_sysdma_START (6)
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_pdbg_sysdma_END (6)
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_npu2dmss_adb_slv_START (7)
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_npu2dmss_adb_slv_END (7)
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_pdbg_ai_core0_START (8)
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_pdbg_ai_core0_END (8)
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_npu_sysdma_START (10)
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_npu_sysdma_END (10)
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_npu_tcu_START (11)
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_npu_tcu_END (11)
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_sysdma_smmu0_START (13)
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_sysdma_smmu0_END (13)
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_ts_tidm_START (14)
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_ts_tidm_END (14)
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_ts_its_START (15)
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_ts_its_END (15)
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_ts_subsys_START (17)
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_ts_subsys_END (17)
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_ts_crg_START (18)
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_ts_crg_END (18)
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_ts_smmu_START (19)
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_ts_smmu_END (19)
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_noc_tiny_START (20)
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_noc_tiny_END (20)
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_npu_l2buf_START (21)
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_npu_l2buf_END (21)
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_noc_ts_cfg_START (22)
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_noc_ts_cfg_END (22)
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_noc_ts_START (23)
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_noc_ts_END (23)
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_npu_cssys_START (24)
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_npu_cssys_END (24)
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_noc_l2buf_START (25)
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_noc_l2buf_END (25)
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_noc_tcu_START (26)
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_noc_tcu_END (26)
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_noc_sysdma_START (27)
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_noc_sysdma_END (27)
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_noc_aicore0_START (28)
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_noc_aicore0_END (28)
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_npu_hpm_START (29)
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_npu_hpm_END (29)
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_npu_tidm_START (30)
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_npu_tidm_END (30)
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_npu_pa_START (31)
#define SOC_NPUCRG_PERRSTSTAT0_ip_rst_npu_pa_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_prst_npu_dpm0 : 1;
        unsigned int ip_prst_npu_easc1 : 1;
        unsigned int ip_prst_npu_l2buf : 1;
        unsigned int ip_prst_npu_uart : 1;
        unsigned int ip_prst_npu_ipc_ns : 1;
        unsigned int ip_prst_npu_ipc_s : 1;
        unsigned int ip_prst_npu_loadmonitor : 1;
        unsigned int ip_prst_npu_easc0 : 1;
        unsigned int ip_prst_npu_pcr : 1;
        unsigned int ip_prst_npu_tcu : 1;
        unsigned int ip_prst_npu_easc2 : 1;
        unsigned int ip_prst_sysdma_smmu0 : 1;
        unsigned int ip_prst_npu_easc3 : 1;
        unsigned int ip_prst_npu_sysdma : 1;
        unsigned int ip_prst_aicore0_smmu : 1;
        unsigned int ip_prst_atgs : 1;
        unsigned int ip_prst_atgm : 1;
        unsigned int ip_prst_npu_hw_exp_irq_ns : 1;
        unsigned int ip_prst_npu_hw_exp_irq : 1;
        unsigned int ip_prst_npu_idsc : 1;
        unsigned int ip_rst_ai_core1 : 1;
        unsigned int ip_rst_pdbg_ai_core1 : 1;
        unsigned int ip_rst_noc_aicore1 : 1;
        unsigned int ip_prst_aicore1_smmu : 1;
        unsigned int ip_rst_aicore1_smmu : 1;
        unsigned int ip_prst_npu_dpm1 : 1;
        unsigned int reserved : 6;
    } reg;
} SOC_NPUCRG_PERRSTEN1_UNION;
#endif
#define SOC_NPUCRG_PERRSTEN1_ip_prst_npu_dpm0_START (0)
#define SOC_NPUCRG_PERRSTEN1_ip_prst_npu_dpm0_END (0)
#define SOC_NPUCRG_PERRSTEN1_ip_prst_npu_easc1_START (1)
#define SOC_NPUCRG_PERRSTEN1_ip_prst_npu_easc1_END (1)
#define SOC_NPUCRG_PERRSTEN1_ip_prst_npu_l2buf_START (2)
#define SOC_NPUCRG_PERRSTEN1_ip_prst_npu_l2buf_END (2)
#define SOC_NPUCRG_PERRSTEN1_ip_prst_npu_uart_START (3)
#define SOC_NPUCRG_PERRSTEN1_ip_prst_npu_uart_END (3)
#define SOC_NPUCRG_PERRSTEN1_ip_prst_npu_ipc_ns_START (4)
#define SOC_NPUCRG_PERRSTEN1_ip_prst_npu_ipc_ns_END (4)
#define SOC_NPUCRG_PERRSTEN1_ip_prst_npu_ipc_s_START (5)
#define SOC_NPUCRG_PERRSTEN1_ip_prst_npu_ipc_s_END (5)
#define SOC_NPUCRG_PERRSTEN1_ip_prst_npu_loadmonitor_START (6)
#define SOC_NPUCRG_PERRSTEN1_ip_prst_npu_loadmonitor_END (6)
#define SOC_NPUCRG_PERRSTEN1_ip_prst_npu_easc0_START (7)
#define SOC_NPUCRG_PERRSTEN1_ip_prst_npu_easc0_END (7)
#define SOC_NPUCRG_PERRSTEN1_ip_prst_npu_pcr_START (8)
#define SOC_NPUCRG_PERRSTEN1_ip_prst_npu_pcr_END (8)
#define SOC_NPUCRG_PERRSTEN1_ip_prst_npu_tcu_START (9)
#define SOC_NPUCRG_PERRSTEN1_ip_prst_npu_tcu_END (9)
#define SOC_NPUCRG_PERRSTEN1_ip_prst_npu_easc2_START (10)
#define SOC_NPUCRG_PERRSTEN1_ip_prst_npu_easc2_END (10)
#define SOC_NPUCRG_PERRSTEN1_ip_prst_sysdma_smmu0_START (11)
#define SOC_NPUCRG_PERRSTEN1_ip_prst_sysdma_smmu0_END (11)
#define SOC_NPUCRG_PERRSTEN1_ip_prst_npu_easc3_START (12)
#define SOC_NPUCRG_PERRSTEN1_ip_prst_npu_easc3_END (12)
#define SOC_NPUCRG_PERRSTEN1_ip_prst_npu_sysdma_START (13)
#define SOC_NPUCRG_PERRSTEN1_ip_prst_npu_sysdma_END (13)
#define SOC_NPUCRG_PERRSTEN1_ip_prst_aicore0_smmu_START (14)
#define SOC_NPUCRG_PERRSTEN1_ip_prst_aicore0_smmu_END (14)
#define SOC_NPUCRG_PERRSTEN1_ip_prst_atgs_START (15)
#define SOC_NPUCRG_PERRSTEN1_ip_prst_atgs_END (15)
#define SOC_NPUCRG_PERRSTEN1_ip_prst_atgm_START (16)
#define SOC_NPUCRG_PERRSTEN1_ip_prst_atgm_END (16)
#define SOC_NPUCRG_PERRSTEN1_ip_prst_npu_hw_exp_irq_ns_START (17)
#define SOC_NPUCRG_PERRSTEN1_ip_prst_npu_hw_exp_irq_ns_END (17)
#define SOC_NPUCRG_PERRSTEN1_ip_prst_npu_hw_exp_irq_START (18)
#define SOC_NPUCRG_PERRSTEN1_ip_prst_npu_hw_exp_irq_END (18)
#define SOC_NPUCRG_PERRSTEN1_ip_prst_npu_idsc_START (19)
#define SOC_NPUCRG_PERRSTEN1_ip_prst_npu_idsc_END (19)
#define SOC_NPUCRG_PERRSTEN1_ip_rst_ai_core1_START (20)
#define SOC_NPUCRG_PERRSTEN1_ip_rst_ai_core1_END (20)
#define SOC_NPUCRG_PERRSTEN1_ip_rst_pdbg_ai_core1_START (21)
#define SOC_NPUCRG_PERRSTEN1_ip_rst_pdbg_ai_core1_END (21)
#define SOC_NPUCRG_PERRSTEN1_ip_rst_noc_aicore1_START (22)
#define SOC_NPUCRG_PERRSTEN1_ip_rst_noc_aicore1_END (22)
#define SOC_NPUCRG_PERRSTEN1_ip_prst_aicore1_smmu_START (23)
#define SOC_NPUCRG_PERRSTEN1_ip_prst_aicore1_smmu_END (23)
#define SOC_NPUCRG_PERRSTEN1_ip_rst_aicore1_smmu_START (24)
#define SOC_NPUCRG_PERRSTEN1_ip_rst_aicore1_smmu_END (24)
#define SOC_NPUCRG_PERRSTEN1_ip_prst_npu_dpm1_START (25)
#define SOC_NPUCRG_PERRSTEN1_ip_prst_npu_dpm1_END (25)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_prst_npu_dpm0 : 1;
        unsigned int ip_prst_npu_easc1 : 1;
        unsigned int ip_prst_npu_l2buf : 1;
        unsigned int ip_prst_npu_uart : 1;
        unsigned int ip_prst_npu_ipc_ns : 1;
        unsigned int ip_prst_npu_ipc_s : 1;
        unsigned int ip_prst_npu_loadmonitor : 1;
        unsigned int ip_prst_npu_easc0 : 1;
        unsigned int ip_prst_npu_pcr : 1;
        unsigned int ip_prst_npu_tcu : 1;
        unsigned int ip_prst_npu_easc2 : 1;
        unsigned int ip_prst_sysdma_smmu0 : 1;
        unsigned int ip_prst_npu_easc3 : 1;
        unsigned int ip_prst_npu_sysdma : 1;
        unsigned int ip_prst_aicore0_smmu : 1;
        unsigned int ip_prst_atgs : 1;
        unsigned int ip_prst_atgm : 1;
        unsigned int ip_prst_npu_hw_exp_irq_ns : 1;
        unsigned int ip_prst_npu_hw_exp_irq : 1;
        unsigned int ip_prst_npu_idsc : 1;
        unsigned int ip_rst_ai_core1 : 1;
        unsigned int ip_rst_pdbg_ai_core1 : 1;
        unsigned int ip_rst_noc_aicore1 : 1;
        unsigned int ip_prst_aicore1_smmu : 1;
        unsigned int ip_rst_aicore1_smmu : 1;
        unsigned int ip_prst_npu_dpm1 : 1;
        unsigned int reserved : 6;
    } reg;
} SOC_NPUCRG_PERRSTDIS1_UNION;
#endif
#define SOC_NPUCRG_PERRSTDIS1_ip_prst_npu_dpm0_START (0)
#define SOC_NPUCRG_PERRSTDIS1_ip_prst_npu_dpm0_END (0)
#define SOC_NPUCRG_PERRSTDIS1_ip_prst_npu_easc1_START (1)
#define SOC_NPUCRG_PERRSTDIS1_ip_prst_npu_easc1_END (1)
#define SOC_NPUCRG_PERRSTDIS1_ip_prst_npu_l2buf_START (2)
#define SOC_NPUCRG_PERRSTDIS1_ip_prst_npu_l2buf_END (2)
#define SOC_NPUCRG_PERRSTDIS1_ip_prst_npu_uart_START (3)
#define SOC_NPUCRG_PERRSTDIS1_ip_prst_npu_uart_END (3)
#define SOC_NPUCRG_PERRSTDIS1_ip_prst_npu_ipc_ns_START (4)
#define SOC_NPUCRG_PERRSTDIS1_ip_prst_npu_ipc_ns_END (4)
#define SOC_NPUCRG_PERRSTDIS1_ip_prst_npu_ipc_s_START (5)
#define SOC_NPUCRG_PERRSTDIS1_ip_prst_npu_ipc_s_END (5)
#define SOC_NPUCRG_PERRSTDIS1_ip_prst_npu_loadmonitor_START (6)
#define SOC_NPUCRG_PERRSTDIS1_ip_prst_npu_loadmonitor_END (6)
#define SOC_NPUCRG_PERRSTDIS1_ip_prst_npu_easc0_START (7)
#define SOC_NPUCRG_PERRSTDIS1_ip_prst_npu_easc0_END (7)
#define SOC_NPUCRG_PERRSTDIS1_ip_prst_npu_pcr_START (8)
#define SOC_NPUCRG_PERRSTDIS1_ip_prst_npu_pcr_END (8)
#define SOC_NPUCRG_PERRSTDIS1_ip_prst_npu_tcu_START (9)
#define SOC_NPUCRG_PERRSTDIS1_ip_prst_npu_tcu_END (9)
#define SOC_NPUCRG_PERRSTDIS1_ip_prst_npu_easc2_START (10)
#define SOC_NPUCRG_PERRSTDIS1_ip_prst_npu_easc2_END (10)
#define SOC_NPUCRG_PERRSTDIS1_ip_prst_sysdma_smmu0_START (11)
#define SOC_NPUCRG_PERRSTDIS1_ip_prst_sysdma_smmu0_END (11)
#define SOC_NPUCRG_PERRSTDIS1_ip_prst_npu_easc3_START (12)
#define SOC_NPUCRG_PERRSTDIS1_ip_prst_npu_easc3_END (12)
#define SOC_NPUCRG_PERRSTDIS1_ip_prst_npu_sysdma_START (13)
#define SOC_NPUCRG_PERRSTDIS1_ip_prst_npu_sysdma_END (13)
#define SOC_NPUCRG_PERRSTDIS1_ip_prst_aicore0_smmu_START (14)
#define SOC_NPUCRG_PERRSTDIS1_ip_prst_aicore0_smmu_END (14)
#define SOC_NPUCRG_PERRSTDIS1_ip_prst_atgs_START (15)
#define SOC_NPUCRG_PERRSTDIS1_ip_prst_atgs_END (15)
#define SOC_NPUCRG_PERRSTDIS1_ip_prst_atgm_START (16)
#define SOC_NPUCRG_PERRSTDIS1_ip_prst_atgm_END (16)
#define SOC_NPUCRG_PERRSTDIS1_ip_prst_npu_hw_exp_irq_ns_START (17)
#define SOC_NPUCRG_PERRSTDIS1_ip_prst_npu_hw_exp_irq_ns_END (17)
#define SOC_NPUCRG_PERRSTDIS1_ip_prst_npu_hw_exp_irq_START (18)
#define SOC_NPUCRG_PERRSTDIS1_ip_prst_npu_hw_exp_irq_END (18)
#define SOC_NPUCRG_PERRSTDIS1_ip_prst_npu_idsc_START (19)
#define SOC_NPUCRG_PERRSTDIS1_ip_prst_npu_idsc_END (19)
#define SOC_NPUCRG_PERRSTDIS1_ip_rst_ai_core1_START (20)
#define SOC_NPUCRG_PERRSTDIS1_ip_rst_ai_core1_END (20)
#define SOC_NPUCRG_PERRSTDIS1_ip_rst_pdbg_ai_core1_START (21)
#define SOC_NPUCRG_PERRSTDIS1_ip_rst_pdbg_ai_core1_END (21)
#define SOC_NPUCRG_PERRSTDIS1_ip_rst_noc_aicore1_START (22)
#define SOC_NPUCRG_PERRSTDIS1_ip_rst_noc_aicore1_END (22)
#define SOC_NPUCRG_PERRSTDIS1_ip_prst_aicore1_smmu_START (23)
#define SOC_NPUCRG_PERRSTDIS1_ip_prst_aicore1_smmu_END (23)
#define SOC_NPUCRG_PERRSTDIS1_ip_rst_aicore1_smmu_START (24)
#define SOC_NPUCRG_PERRSTDIS1_ip_rst_aicore1_smmu_END (24)
#define SOC_NPUCRG_PERRSTDIS1_ip_prst_npu_dpm1_START (25)
#define SOC_NPUCRG_PERRSTDIS1_ip_prst_npu_dpm1_END (25)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_prst_npu_dpm0 : 1;
        unsigned int ip_prst_npu_easc1 : 1;
        unsigned int ip_prst_npu_l2buf : 1;
        unsigned int ip_prst_npu_uart : 1;
        unsigned int ip_prst_npu_ipc_ns : 1;
        unsigned int ip_prst_npu_ipc_s : 1;
        unsigned int ip_prst_npu_loadmonitor : 1;
        unsigned int ip_prst_npu_easc0 : 1;
        unsigned int ip_prst_npu_pcr : 1;
        unsigned int ip_prst_npu_tcu : 1;
        unsigned int ip_prst_npu_easc2 : 1;
        unsigned int ip_prst_sysdma_smmu0 : 1;
        unsigned int ip_prst_npu_easc3 : 1;
        unsigned int ip_prst_npu_sysdma : 1;
        unsigned int ip_prst_aicore0_smmu : 1;
        unsigned int ip_prst_atgs : 1;
        unsigned int ip_prst_atgm : 1;
        unsigned int ip_prst_npu_hw_exp_irq_ns : 1;
        unsigned int ip_prst_npu_hw_exp_irq : 1;
        unsigned int ip_prst_npu_idsc : 1;
        unsigned int ip_rst_ai_core1 : 1;
        unsigned int ip_rst_pdbg_ai_core1 : 1;
        unsigned int ip_rst_noc_aicore1 : 1;
        unsigned int ip_prst_aicore1_smmu : 1;
        unsigned int ip_rst_aicore1_smmu : 1;
        unsigned int ip_prst_npu_dpm1 : 1;
        unsigned int reserved : 6;
    } reg;
} SOC_NPUCRG_PERRSTSTAT1_UNION;
#endif
#define SOC_NPUCRG_PERRSTSTAT1_ip_prst_npu_dpm0_START (0)
#define SOC_NPUCRG_PERRSTSTAT1_ip_prst_npu_dpm0_END (0)
#define SOC_NPUCRG_PERRSTSTAT1_ip_prst_npu_easc1_START (1)
#define SOC_NPUCRG_PERRSTSTAT1_ip_prst_npu_easc1_END (1)
#define SOC_NPUCRG_PERRSTSTAT1_ip_prst_npu_l2buf_START (2)
#define SOC_NPUCRG_PERRSTSTAT1_ip_prst_npu_l2buf_END (2)
#define SOC_NPUCRG_PERRSTSTAT1_ip_prst_npu_uart_START (3)
#define SOC_NPUCRG_PERRSTSTAT1_ip_prst_npu_uart_END (3)
#define SOC_NPUCRG_PERRSTSTAT1_ip_prst_npu_ipc_ns_START (4)
#define SOC_NPUCRG_PERRSTSTAT1_ip_prst_npu_ipc_ns_END (4)
#define SOC_NPUCRG_PERRSTSTAT1_ip_prst_npu_ipc_s_START (5)
#define SOC_NPUCRG_PERRSTSTAT1_ip_prst_npu_ipc_s_END (5)
#define SOC_NPUCRG_PERRSTSTAT1_ip_prst_npu_loadmonitor_START (6)
#define SOC_NPUCRG_PERRSTSTAT1_ip_prst_npu_loadmonitor_END (6)
#define SOC_NPUCRG_PERRSTSTAT1_ip_prst_npu_easc0_START (7)
#define SOC_NPUCRG_PERRSTSTAT1_ip_prst_npu_easc0_END (7)
#define SOC_NPUCRG_PERRSTSTAT1_ip_prst_npu_pcr_START (8)
#define SOC_NPUCRG_PERRSTSTAT1_ip_prst_npu_pcr_END (8)
#define SOC_NPUCRG_PERRSTSTAT1_ip_prst_npu_tcu_START (9)
#define SOC_NPUCRG_PERRSTSTAT1_ip_prst_npu_tcu_END (9)
#define SOC_NPUCRG_PERRSTSTAT1_ip_prst_npu_easc2_START (10)
#define SOC_NPUCRG_PERRSTSTAT1_ip_prst_npu_easc2_END (10)
#define SOC_NPUCRG_PERRSTSTAT1_ip_prst_sysdma_smmu0_START (11)
#define SOC_NPUCRG_PERRSTSTAT1_ip_prst_sysdma_smmu0_END (11)
#define SOC_NPUCRG_PERRSTSTAT1_ip_prst_npu_easc3_START (12)
#define SOC_NPUCRG_PERRSTSTAT1_ip_prst_npu_easc3_END (12)
#define SOC_NPUCRG_PERRSTSTAT1_ip_prst_npu_sysdma_START (13)
#define SOC_NPUCRG_PERRSTSTAT1_ip_prst_npu_sysdma_END (13)
#define SOC_NPUCRG_PERRSTSTAT1_ip_prst_aicore0_smmu_START (14)
#define SOC_NPUCRG_PERRSTSTAT1_ip_prst_aicore0_smmu_END (14)
#define SOC_NPUCRG_PERRSTSTAT1_ip_prst_atgs_START (15)
#define SOC_NPUCRG_PERRSTSTAT1_ip_prst_atgs_END (15)
#define SOC_NPUCRG_PERRSTSTAT1_ip_prst_atgm_START (16)
#define SOC_NPUCRG_PERRSTSTAT1_ip_prst_atgm_END (16)
#define SOC_NPUCRG_PERRSTSTAT1_ip_prst_npu_hw_exp_irq_ns_START (17)
#define SOC_NPUCRG_PERRSTSTAT1_ip_prst_npu_hw_exp_irq_ns_END (17)
#define SOC_NPUCRG_PERRSTSTAT1_ip_prst_npu_hw_exp_irq_START (18)
#define SOC_NPUCRG_PERRSTSTAT1_ip_prst_npu_hw_exp_irq_END (18)
#define SOC_NPUCRG_PERRSTSTAT1_ip_prst_npu_idsc_START (19)
#define SOC_NPUCRG_PERRSTSTAT1_ip_prst_npu_idsc_END (19)
#define SOC_NPUCRG_PERRSTSTAT1_ip_rst_ai_core1_START (20)
#define SOC_NPUCRG_PERRSTSTAT1_ip_rst_ai_core1_END (20)
#define SOC_NPUCRG_PERRSTSTAT1_ip_rst_pdbg_ai_core1_START (21)
#define SOC_NPUCRG_PERRSTSTAT1_ip_rst_pdbg_ai_core1_END (21)
#define SOC_NPUCRG_PERRSTSTAT1_ip_rst_noc_aicore1_START (22)
#define SOC_NPUCRG_PERRSTSTAT1_ip_rst_noc_aicore1_END (22)
#define SOC_NPUCRG_PERRSTSTAT1_ip_prst_aicore1_smmu_START (23)
#define SOC_NPUCRG_PERRSTSTAT1_ip_prst_aicore1_smmu_END (23)
#define SOC_NPUCRG_PERRSTSTAT1_ip_rst_aicore1_smmu_START (24)
#define SOC_NPUCRG_PERRSTSTAT1_ip_rst_aicore1_smmu_END (24)
#define SOC_NPUCRG_PERRSTSTAT1_ip_prst_npu_dpm1_START (25)
#define SOC_NPUCRG_PERRSTSTAT1_ip_prst_npu_dpm1_END (25)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_rst_svfd_cpm_npu : 1;
        unsigned int ip_rst_svfd_ffs_npu : 1;
        unsigned int ip_rst_vsensorc_npu : 1;
        unsigned int ip_rst_npu_easc3 : 1;
        unsigned int ip_rst_npu_easc2 : 1;
        unsigned int ip_rst_npu_easc1 : 1;
        unsigned int ip_rst_npu_easc0 : 1;
        unsigned int ip_rst_npu_idsc : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int ip_rst_tiny_hpm : 1;
        unsigned int ip_rst_tiny_tidm : 1;
        unsigned int ip_rst_tiny_pa : 1;
        unsigned int ip_rst_tiny_apb : 1;
        unsigned int ip_rst_tiny_core : 1;
        unsigned int ip_rst_tiny_subsys : 1;
        unsigned int ip_rst_pdbg_tiny_core : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int ip_rst_aicore1_hpm : 1;
        unsigned int ip_rst_aicore1_tidm : 1;
        unsigned int ip_rst_aicore1_pa : 1;
        unsigned int ip_rst_aicore0_hpm : 1;
        unsigned int ip_rst_aicore0_tidm : 1;
        unsigned int ip_rst_aicore0_pa : 1;
        unsigned int ip_rst_npu_pcr : 1;
        unsigned int reserved_8 : 2;
    } reg;
} SOC_NPUCRG_PERRSTEN2_UNION;
#endif
#define SOC_NPUCRG_PERRSTEN2_ip_rst_svfd_cpm_npu_START (0)
#define SOC_NPUCRG_PERRSTEN2_ip_rst_svfd_cpm_npu_END (0)
#define SOC_NPUCRG_PERRSTEN2_ip_rst_svfd_ffs_npu_START (1)
#define SOC_NPUCRG_PERRSTEN2_ip_rst_svfd_ffs_npu_END (1)
#define SOC_NPUCRG_PERRSTEN2_ip_rst_vsensorc_npu_START (2)
#define SOC_NPUCRG_PERRSTEN2_ip_rst_vsensorc_npu_END (2)
#define SOC_NPUCRG_PERRSTEN2_ip_rst_npu_easc3_START (3)
#define SOC_NPUCRG_PERRSTEN2_ip_rst_npu_easc3_END (3)
#define SOC_NPUCRG_PERRSTEN2_ip_rst_npu_easc2_START (4)
#define SOC_NPUCRG_PERRSTEN2_ip_rst_npu_easc2_END (4)
#define SOC_NPUCRG_PERRSTEN2_ip_rst_npu_easc1_START (5)
#define SOC_NPUCRG_PERRSTEN2_ip_rst_npu_easc1_END (5)
#define SOC_NPUCRG_PERRSTEN2_ip_rst_npu_easc0_START (6)
#define SOC_NPUCRG_PERRSTEN2_ip_rst_npu_easc0_END (6)
#define SOC_NPUCRG_PERRSTEN2_ip_rst_npu_idsc_START (7)
#define SOC_NPUCRG_PERRSTEN2_ip_rst_npu_idsc_END (7)
#define SOC_NPUCRG_PERRSTEN2_ip_rst_tiny_hpm_START (10)
#define SOC_NPUCRG_PERRSTEN2_ip_rst_tiny_hpm_END (10)
#define SOC_NPUCRG_PERRSTEN2_ip_rst_tiny_tidm_START (11)
#define SOC_NPUCRG_PERRSTEN2_ip_rst_tiny_tidm_END (11)
#define SOC_NPUCRG_PERRSTEN2_ip_rst_tiny_pa_START (12)
#define SOC_NPUCRG_PERRSTEN2_ip_rst_tiny_pa_END (12)
#define SOC_NPUCRG_PERRSTEN2_ip_rst_tiny_apb_START (13)
#define SOC_NPUCRG_PERRSTEN2_ip_rst_tiny_apb_END (13)
#define SOC_NPUCRG_PERRSTEN2_ip_rst_tiny_core_START (14)
#define SOC_NPUCRG_PERRSTEN2_ip_rst_tiny_core_END (14)
#define SOC_NPUCRG_PERRSTEN2_ip_rst_tiny_subsys_START (15)
#define SOC_NPUCRG_PERRSTEN2_ip_rst_tiny_subsys_END (15)
#define SOC_NPUCRG_PERRSTEN2_ip_rst_pdbg_tiny_core_START (16)
#define SOC_NPUCRG_PERRSTEN2_ip_rst_pdbg_tiny_core_END (16)
#define SOC_NPUCRG_PERRSTEN2_ip_rst_aicore1_hpm_START (23)
#define SOC_NPUCRG_PERRSTEN2_ip_rst_aicore1_hpm_END (23)
#define SOC_NPUCRG_PERRSTEN2_ip_rst_aicore1_tidm_START (24)
#define SOC_NPUCRG_PERRSTEN2_ip_rst_aicore1_tidm_END (24)
#define SOC_NPUCRG_PERRSTEN2_ip_rst_aicore1_pa_START (25)
#define SOC_NPUCRG_PERRSTEN2_ip_rst_aicore1_pa_END (25)
#define SOC_NPUCRG_PERRSTEN2_ip_rst_aicore0_hpm_START (26)
#define SOC_NPUCRG_PERRSTEN2_ip_rst_aicore0_hpm_END (26)
#define SOC_NPUCRG_PERRSTEN2_ip_rst_aicore0_tidm_START (27)
#define SOC_NPUCRG_PERRSTEN2_ip_rst_aicore0_tidm_END (27)
#define SOC_NPUCRG_PERRSTEN2_ip_rst_aicore0_pa_START (28)
#define SOC_NPUCRG_PERRSTEN2_ip_rst_aicore0_pa_END (28)
#define SOC_NPUCRG_PERRSTEN2_ip_rst_npu_pcr_START (29)
#define SOC_NPUCRG_PERRSTEN2_ip_rst_npu_pcr_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_rst_svfd_cpm_npu : 1;
        unsigned int ip_rst_svfd_ffs_npu : 1;
        unsigned int ip_rst_vsensorc_npu : 1;
        unsigned int ip_rst_npu_easc3 : 1;
        unsigned int ip_rst_npu_easc2 : 1;
        unsigned int ip_rst_npu_easc1 : 1;
        unsigned int ip_rst_npu_easc0 : 1;
        unsigned int ip_rst_npu_idsc : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int ip_rst_tiny_hpm : 1;
        unsigned int ip_rst_tiny_tidm : 1;
        unsigned int ip_rst_tiny_pa : 1;
        unsigned int ip_rst_tiny_apb : 1;
        unsigned int ip_rst_tiny_core : 1;
        unsigned int ip_rst_tiny_subsys : 1;
        unsigned int ip_rst_pdbg_tiny_core : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int ip_rst_aicore1_hpm : 1;
        unsigned int ip_rst_aicore1_tidm : 1;
        unsigned int ip_rst_aicore1_pa : 1;
        unsigned int ip_rst_aicore0_hpm : 1;
        unsigned int ip_rst_aicore0_tidm : 1;
        unsigned int ip_rst_aicore0_pa : 1;
        unsigned int ip_rst_npu_pcr : 1;
        unsigned int reserved_8 : 2;
    } reg;
} SOC_NPUCRG_PERRSTDIS2_UNION;
#endif
#define SOC_NPUCRG_PERRSTDIS2_ip_rst_svfd_cpm_npu_START (0)
#define SOC_NPUCRG_PERRSTDIS2_ip_rst_svfd_cpm_npu_END (0)
#define SOC_NPUCRG_PERRSTDIS2_ip_rst_svfd_ffs_npu_START (1)
#define SOC_NPUCRG_PERRSTDIS2_ip_rst_svfd_ffs_npu_END (1)
#define SOC_NPUCRG_PERRSTDIS2_ip_rst_vsensorc_npu_START (2)
#define SOC_NPUCRG_PERRSTDIS2_ip_rst_vsensorc_npu_END (2)
#define SOC_NPUCRG_PERRSTDIS2_ip_rst_npu_easc3_START (3)
#define SOC_NPUCRG_PERRSTDIS2_ip_rst_npu_easc3_END (3)
#define SOC_NPUCRG_PERRSTDIS2_ip_rst_npu_easc2_START (4)
#define SOC_NPUCRG_PERRSTDIS2_ip_rst_npu_easc2_END (4)
#define SOC_NPUCRG_PERRSTDIS2_ip_rst_npu_easc1_START (5)
#define SOC_NPUCRG_PERRSTDIS2_ip_rst_npu_easc1_END (5)
#define SOC_NPUCRG_PERRSTDIS2_ip_rst_npu_easc0_START (6)
#define SOC_NPUCRG_PERRSTDIS2_ip_rst_npu_easc0_END (6)
#define SOC_NPUCRG_PERRSTDIS2_ip_rst_npu_idsc_START (7)
#define SOC_NPUCRG_PERRSTDIS2_ip_rst_npu_idsc_END (7)
#define SOC_NPUCRG_PERRSTDIS2_ip_rst_tiny_hpm_START (10)
#define SOC_NPUCRG_PERRSTDIS2_ip_rst_tiny_hpm_END (10)
#define SOC_NPUCRG_PERRSTDIS2_ip_rst_tiny_tidm_START (11)
#define SOC_NPUCRG_PERRSTDIS2_ip_rst_tiny_tidm_END (11)
#define SOC_NPUCRG_PERRSTDIS2_ip_rst_tiny_pa_START (12)
#define SOC_NPUCRG_PERRSTDIS2_ip_rst_tiny_pa_END (12)
#define SOC_NPUCRG_PERRSTDIS2_ip_rst_tiny_apb_START (13)
#define SOC_NPUCRG_PERRSTDIS2_ip_rst_tiny_apb_END (13)
#define SOC_NPUCRG_PERRSTDIS2_ip_rst_tiny_core_START (14)
#define SOC_NPUCRG_PERRSTDIS2_ip_rst_tiny_core_END (14)
#define SOC_NPUCRG_PERRSTDIS2_ip_rst_tiny_subsys_START (15)
#define SOC_NPUCRG_PERRSTDIS2_ip_rst_tiny_subsys_END (15)
#define SOC_NPUCRG_PERRSTDIS2_ip_rst_pdbg_tiny_core_START (16)
#define SOC_NPUCRG_PERRSTDIS2_ip_rst_pdbg_tiny_core_END (16)
#define SOC_NPUCRG_PERRSTDIS2_ip_rst_aicore1_hpm_START (23)
#define SOC_NPUCRG_PERRSTDIS2_ip_rst_aicore1_hpm_END (23)
#define SOC_NPUCRG_PERRSTDIS2_ip_rst_aicore1_tidm_START (24)
#define SOC_NPUCRG_PERRSTDIS2_ip_rst_aicore1_tidm_END (24)
#define SOC_NPUCRG_PERRSTDIS2_ip_rst_aicore1_pa_START (25)
#define SOC_NPUCRG_PERRSTDIS2_ip_rst_aicore1_pa_END (25)
#define SOC_NPUCRG_PERRSTDIS2_ip_rst_aicore0_hpm_START (26)
#define SOC_NPUCRG_PERRSTDIS2_ip_rst_aicore0_hpm_END (26)
#define SOC_NPUCRG_PERRSTDIS2_ip_rst_aicore0_tidm_START (27)
#define SOC_NPUCRG_PERRSTDIS2_ip_rst_aicore0_tidm_END (27)
#define SOC_NPUCRG_PERRSTDIS2_ip_rst_aicore0_pa_START (28)
#define SOC_NPUCRG_PERRSTDIS2_ip_rst_aicore0_pa_END (28)
#define SOC_NPUCRG_PERRSTDIS2_ip_rst_npu_pcr_START (29)
#define SOC_NPUCRG_PERRSTDIS2_ip_rst_npu_pcr_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_rst_svfd_cpm_npu : 1;
        unsigned int ip_rst_svfd_ffs_npu : 1;
        unsigned int ip_rst_vsensorc_npu : 1;
        unsigned int ip_rst_npu_easc3 : 1;
        unsigned int ip_rst_npu_easc2 : 1;
        unsigned int ip_rst_npu_easc1 : 1;
        unsigned int ip_rst_npu_easc0 : 1;
        unsigned int ip_rst_npu_idsc : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int ip_rst_tiny_hpm : 1;
        unsigned int ip_rst_tiny_tidm : 1;
        unsigned int ip_rst_tiny_pa : 1;
        unsigned int ip_rst_tiny_apb : 1;
        unsigned int ip_rst_tiny_core : 1;
        unsigned int ip_rst_tiny_subsys : 1;
        unsigned int ip_rst_pdbg_tiny_core : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int ip_rst_aicore1_hpm : 1;
        unsigned int ip_rst_aicore1_tidm : 1;
        unsigned int ip_rst_aicore1_pa : 1;
        unsigned int ip_rst_aicore0_hpm : 1;
        unsigned int ip_rst_aicore0_tidm : 1;
        unsigned int ip_rst_aicore0_pa : 1;
        unsigned int ip_rst_npu_pcr : 1;
        unsigned int reserved_8 : 2;
    } reg;
} SOC_NPUCRG_PERRSTSTAT2_UNION;
#endif
#define SOC_NPUCRG_PERRSTSTAT2_ip_rst_svfd_cpm_npu_START (0)
#define SOC_NPUCRG_PERRSTSTAT2_ip_rst_svfd_cpm_npu_END (0)
#define SOC_NPUCRG_PERRSTSTAT2_ip_rst_svfd_ffs_npu_START (1)
#define SOC_NPUCRG_PERRSTSTAT2_ip_rst_svfd_ffs_npu_END (1)
#define SOC_NPUCRG_PERRSTSTAT2_ip_rst_vsensorc_npu_START (2)
#define SOC_NPUCRG_PERRSTSTAT2_ip_rst_vsensorc_npu_END (2)
#define SOC_NPUCRG_PERRSTSTAT2_ip_rst_npu_easc3_START (3)
#define SOC_NPUCRG_PERRSTSTAT2_ip_rst_npu_easc3_END (3)
#define SOC_NPUCRG_PERRSTSTAT2_ip_rst_npu_easc2_START (4)
#define SOC_NPUCRG_PERRSTSTAT2_ip_rst_npu_easc2_END (4)
#define SOC_NPUCRG_PERRSTSTAT2_ip_rst_npu_easc1_START (5)
#define SOC_NPUCRG_PERRSTSTAT2_ip_rst_npu_easc1_END (5)
#define SOC_NPUCRG_PERRSTSTAT2_ip_rst_npu_easc0_START (6)
#define SOC_NPUCRG_PERRSTSTAT2_ip_rst_npu_easc0_END (6)
#define SOC_NPUCRG_PERRSTSTAT2_ip_rst_npu_idsc_START (7)
#define SOC_NPUCRG_PERRSTSTAT2_ip_rst_npu_idsc_END (7)
#define SOC_NPUCRG_PERRSTSTAT2_ip_rst_tiny_hpm_START (10)
#define SOC_NPUCRG_PERRSTSTAT2_ip_rst_tiny_hpm_END (10)
#define SOC_NPUCRG_PERRSTSTAT2_ip_rst_tiny_tidm_START (11)
#define SOC_NPUCRG_PERRSTSTAT2_ip_rst_tiny_tidm_END (11)
#define SOC_NPUCRG_PERRSTSTAT2_ip_rst_tiny_pa_START (12)
#define SOC_NPUCRG_PERRSTSTAT2_ip_rst_tiny_pa_END (12)
#define SOC_NPUCRG_PERRSTSTAT2_ip_rst_tiny_apb_START (13)
#define SOC_NPUCRG_PERRSTSTAT2_ip_rst_tiny_apb_END (13)
#define SOC_NPUCRG_PERRSTSTAT2_ip_rst_tiny_core_START (14)
#define SOC_NPUCRG_PERRSTSTAT2_ip_rst_tiny_core_END (14)
#define SOC_NPUCRG_PERRSTSTAT2_ip_rst_tiny_subsys_START (15)
#define SOC_NPUCRG_PERRSTSTAT2_ip_rst_tiny_subsys_END (15)
#define SOC_NPUCRG_PERRSTSTAT2_ip_rst_pdbg_tiny_core_START (16)
#define SOC_NPUCRG_PERRSTSTAT2_ip_rst_pdbg_tiny_core_END (16)
#define SOC_NPUCRG_PERRSTSTAT2_ip_rst_aicore1_hpm_START (23)
#define SOC_NPUCRG_PERRSTSTAT2_ip_rst_aicore1_hpm_END (23)
#define SOC_NPUCRG_PERRSTSTAT2_ip_rst_aicore1_tidm_START (24)
#define SOC_NPUCRG_PERRSTSTAT2_ip_rst_aicore1_tidm_END (24)
#define SOC_NPUCRG_PERRSTSTAT2_ip_rst_aicore1_pa_START (25)
#define SOC_NPUCRG_PERRSTSTAT2_ip_rst_aicore1_pa_END (25)
#define SOC_NPUCRG_PERRSTSTAT2_ip_rst_aicore0_hpm_START (26)
#define SOC_NPUCRG_PERRSTSTAT2_ip_rst_aicore0_hpm_END (26)
#define SOC_NPUCRG_PERRSTSTAT2_ip_rst_aicore0_tidm_START (27)
#define SOC_NPUCRG_PERRSTSTAT2_ip_rst_aicore0_tidm_END (27)
#define SOC_NPUCRG_PERRSTSTAT2_ip_rst_aicore0_pa_START (28)
#define SOC_NPUCRG_PERRSTSTAT2_ip_rst_aicore0_pa_END (28)
#define SOC_NPUCRG_PERRSTSTAT2_ip_rst_npu_pcr_START (29)
#define SOC_NPUCRG_PERRSTSTAT2_ip_rst_npu_pcr_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int div_clk_ts_dw_bus : 2;
        unsigned int div_clk_ts_apb : 3;
        unsigned int div_atclk_npu : 3;
        unsigned int div_clk_ai_core : 4;
        unsigned int div_clk_tscpu_ppll6 : 4;
        unsigned int bitmasken : 16;
    } reg;
} SOC_NPUCRG_CLKDIV0_UNION;
#endif
#define SOC_NPUCRG_CLKDIV0_div_clk_ts_dw_bus_START (0)
#define SOC_NPUCRG_CLKDIV0_div_clk_ts_dw_bus_END (1)
#define SOC_NPUCRG_CLKDIV0_div_clk_ts_apb_START (2)
#define SOC_NPUCRG_CLKDIV0_div_clk_ts_apb_END (4)
#define SOC_NPUCRG_CLKDIV0_div_atclk_npu_START (5)
#define SOC_NPUCRG_CLKDIV0_div_atclk_npu_END (7)
#define SOC_NPUCRG_CLKDIV0_div_clk_ai_core_START (8)
#define SOC_NPUCRG_CLKDIV0_div_clk_ai_core_END (11)
#define SOC_NPUCRG_CLKDIV0_div_clk_tscpu_ppll6_START (12)
#define SOC_NPUCRG_CLKDIV0_div_clk_tscpu_ppll6_END (15)
#define SOC_NPUCRG_CLKDIV0_bitmasken_START (16)
#define SOC_NPUCRG_CLKDIV0_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sc_gt_clk_ts_dw_bus : 1;
        unsigned int sc_gt_clk_ts_apb : 1;
        unsigned int sc_gt_atclk_npu : 1;
        unsigned int sc_gt_clk_tscpu_ppll6 : 1;
        unsigned int reserved_0 : 1;
        unsigned int sc_gt_clk_ppll5_npu_tp : 1;
        unsigned int div_clk_ppll5_npu_tp : 6;
        unsigned int sc_gt_clk_npu_loadmonitor : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 2;
        unsigned int bitmasken : 16;
    } reg;
} SOC_NPUCRG_CLKDIV1_UNION;
#endif
#define SOC_NPUCRG_CLKDIV1_sc_gt_clk_ts_dw_bus_START (0)
#define SOC_NPUCRG_CLKDIV1_sc_gt_clk_ts_dw_bus_END (0)
#define SOC_NPUCRG_CLKDIV1_sc_gt_clk_ts_apb_START (1)
#define SOC_NPUCRG_CLKDIV1_sc_gt_clk_ts_apb_END (1)
#define SOC_NPUCRG_CLKDIV1_sc_gt_atclk_npu_START (2)
#define SOC_NPUCRG_CLKDIV1_sc_gt_atclk_npu_END (2)
#define SOC_NPUCRG_CLKDIV1_sc_gt_clk_tscpu_ppll6_START (3)
#define SOC_NPUCRG_CLKDIV1_sc_gt_clk_tscpu_ppll6_END (3)
#define SOC_NPUCRG_CLKDIV1_sc_gt_clk_ppll5_npu_tp_START (5)
#define SOC_NPUCRG_CLKDIV1_sc_gt_clk_ppll5_npu_tp_END (5)
#define SOC_NPUCRG_CLKDIV1_div_clk_ppll5_npu_tp_START (6)
#define SOC_NPUCRG_CLKDIV1_div_clk_ppll5_npu_tp_END (11)
#define SOC_NPUCRG_CLKDIV1_sc_gt_clk_npu_loadmonitor_START (12)
#define SOC_NPUCRG_CLKDIV1_sc_gt_clk_npu_loadmonitor_END (12)
#define SOC_NPUCRG_CLKDIV1_bitmasken_START (16)
#define SOC_NPUCRG_CLKDIV1_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int swdone_clk_ts_dw_bus_div : 1;
        unsigned int swdone_clk_ts_apb_div : 1;
        unsigned int swdone_atclk_npu_div : 1;
        unsigned int swdone_clk_tscpu_ppll6_div : 1;
        unsigned int sw_ack_clk_ts_cpu_sw : 2;
        unsigned int swdone_clk_ai_core_div : 1;
        unsigned int reserved_0 : 1;
        unsigned int st_clk_ai_core_pcr : 1;
        unsigned int sw_ack_atclk_npu_sw : 2;
        unsigned int st_clk_ppll5_gt : 1;
        unsigned int st_clk_vdm_to_npu_aicore_gt : 1;
        unsigned int reserved_1 : 19;
    } reg;
} SOC_NPUCRG_PERI_STAT0_UNION;
#endif
#define SOC_NPUCRG_PERI_STAT0_swdone_clk_ts_dw_bus_div_START (0)
#define SOC_NPUCRG_PERI_STAT0_swdone_clk_ts_dw_bus_div_END (0)
#define SOC_NPUCRG_PERI_STAT0_swdone_clk_ts_apb_div_START (1)
#define SOC_NPUCRG_PERI_STAT0_swdone_clk_ts_apb_div_END (1)
#define SOC_NPUCRG_PERI_STAT0_swdone_atclk_npu_div_START (2)
#define SOC_NPUCRG_PERI_STAT0_swdone_atclk_npu_div_END (2)
#define SOC_NPUCRG_PERI_STAT0_swdone_clk_tscpu_ppll6_div_START (3)
#define SOC_NPUCRG_PERI_STAT0_swdone_clk_tscpu_ppll6_div_END (3)
#define SOC_NPUCRG_PERI_STAT0_sw_ack_clk_ts_cpu_sw_START (4)
#define SOC_NPUCRG_PERI_STAT0_sw_ack_clk_ts_cpu_sw_END (5)
#define SOC_NPUCRG_PERI_STAT0_swdone_clk_ai_core_div_START (6)
#define SOC_NPUCRG_PERI_STAT0_swdone_clk_ai_core_div_END (6)
#define SOC_NPUCRG_PERI_STAT0_st_clk_ai_core_pcr_START (8)
#define SOC_NPUCRG_PERI_STAT0_st_clk_ai_core_pcr_END (8)
#define SOC_NPUCRG_PERI_STAT0_sw_ack_atclk_npu_sw_START (9)
#define SOC_NPUCRG_PERI_STAT0_sw_ack_atclk_npu_sw_END (10)
#define SOC_NPUCRG_PERI_STAT0_st_clk_ppll5_gt_START (11)
#define SOC_NPUCRG_PERI_STAT0_st_clk_ppll5_gt_END (11)
#define SOC_NPUCRG_PERI_STAT0_st_clk_vdm_to_npu_aicore_gt_START (12)
#define SOC_NPUCRG_PERI_STAT0_st_clk_vdm_to_npu_aicore_gt_END (12)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pcr_gt_aicore_bypass : 1;
        unsigned int fn_sc_ppll5_sel : 1;
        unsigned int reserved_0 : 1;
        unsigned int sel_ts_cpu_sw : 1;
        unsigned int reserved_1 : 1;
        unsigned int sel_atclk_npu_sw : 1;
        unsigned int reserved_2 : 10;
        unsigned int bitmasken : 16;
    } reg;
} SOC_NPUCRG_PERI_CTRL0_UNION;
#endif
#define SOC_NPUCRG_PERI_CTRL0_pcr_gt_aicore_bypass_START (0)
#define SOC_NPUCRG_PERI_CTRL0_pcr_gt_aicore_bypass_END (0)
#define SOC_NPUCRG_PERI_CTRL0_fn_sc_ppll5_sel_START (1)
#define SOC_NPUCRG_PERI_CTRL0_fn_sc_ppll5_sel_END (1)
#define SOC_NPUCRG_PERI_CTRL0_sel_ts_cpu_sw_START (3)
#define SOC_NPUCRG_PERI_CTRL0_sel_ts_cpu_sw_END (3)
#define SOC_NPUCRG_PERI_CTRL0_sel_atclk_npu_sw_START (5)
#define SOC_NPUCRG_PERI_CTRL0_sel_atclk_npu_sw_END (5)
#define SOC_NPUCRG_PERI_CTRL0_bitmasken_START (16)
#define SOC_NPUCRG_PERI_CTRL0_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sysdma_smmu0_clkrst_bypass : 1;
        unsigned int aicore0_smmu_clkrst_bypass : 1;
        unsigned int npu_sysdma_clkrst_bypass : 1;
        unsigned int npu_l2buf_clkrst_bypass : 1;
        unsigned int npu_uart_clkrst_bypass : 1;
        unsigned int npu_ipc_ns_clkrst_bypass : 1;
        unsigned int npu_ipc_s_clkrst_bypass : 1;
        unsigned int npu_dpm0_clkrst_bypass : 1;
        unsigned int npu_irq_clkrst_bypass : 1;
        unsigned int aicore1_smmu_clkrst_bypass : 1;
        unsigned int npu_dpm1_clkrst_bypass : 1;
        unsigned int irq_ns_clkrst_bypass : 1;
        unsigned int idsc_clkrst_bypass : 1;
        unsigned int easc3_clkrst_bypass : 1;
        unsigned int easc2_clkrst_bypass : 1;
        unsigned int easc1_clkrst_bypass : 1;
        unsigned int easc0_clkrst_bypass : 1;
        unsigned int pcr_clkrst_bypass : 1;
        unsigned int npu_tcu_clkrst_bypass : 1;
        unsigned int loadmonitor_clkrst_bypass : 1;
        unsigned int reserved : 12;
    } reg;
} SOC_NPUCRG_IPCLKRST_BYPASS_UNION;
#endif
#define SOC_NPUCRG_IPCLKRST_BYPASS_sysdma_smmu0_clkrst_bypass_START (0)
#define SOC_NPUCRG_IPCLKRST_BYPASS_sysdma_smmu0_clkrst_bypass_END (0)
#define SOC_NPUCRG_IPCLKRST_BYPASS_aicore0_smmu_clkrst_bypass_START (1)
#define SOC_NPUCRG_IPCLKRST_BYPASS_aicore0_smmu_clkrst_bypass_END (1)
#define SOC_NPUCRG_IPCLKRST_BYPASS_npu_sysdma_clkrst_bypass_START (2)
#define SOC_NPUCRG_IPCLKRST_BYPASS_npu_sysdma_clkrst_bypass_END (2)
#define SOC_NPUCRG_IPCLKRST_BYPASS_npu_l2buf_clkrst_bypass_START (3)
#define SOC_NPUCRG_IPCLKRST_BYPASS_npu_l2buf_clkrst_bypass_END (3)
#define SOC_NPUCRG_IPCLKRST_BYPASS_npu_uart_clkrst_bypass_START (4)
#define SOC_NPUCRG_IPCLKRST_BYPASS_npu_uart_clkrst_bypass_END (4)
#define SOC_NPUCRG_IPCLKRST_BYPASS_npu_ipc_ns_clkrst_bypass_START (5)
#define SOC_NPUCRG_IPCLKRST_BYPASS_npu_ipc_ns_clkrst_bypass_END (5)
#define SOC_NPUCRG_IPCLKRST_BYPASS_npu_ipc_s_clkrst_bypass_START (6)
#define SOC_NPUCRG_IPCLKRST_BYPASS_npu_ipc_s_clkrst_bypass_END (6)
#define SOC_NPUCRG_IPCLKRST_BYPASS_npu_dpm0_clkrst_bypass_START (7)
#define SOC_NPUCRG_IPCLKRST_BYPASS_npu_dpm0_clkrst_bypass_END (7)
#define SOC_NPUCRG_IPCLKRST_BYPASS_npu_irq_clkrst_bypass_START (8)
#define SOC_NPUCRG_IPCLKRST_BYPASS_npu_irq_clkrst_bypass_END (8)
#define SOC_NPUCRG_IPCLKRST_BYPASS_aicore1_smmu_clkrst_bypass_START (9)
#define SOC_NPUCRG_IPCLKRST_BYPASS_aicore1_smmu_clkrst_bypass_END (9)
#define SOC_NPUCRG_IPCLKRST_BYPASS_npu_dpm1_clkrst_bypass_START (10)
#define SOC_NPUCRG_IPCLKRST_BYPASS_npu_dpm1_clkrst_bypass_END (10)
#define SOC_NPUCRG_IPCLKRST_BYPASS_irq_ns_clkrst_bypass_START (11)
#define SOC_NPUCRG_IPCLKRST_BYPASS_irq_ns_clkrst_bypass_END (11)
#define SOC_NPUCRG_IPCLKRST_BYPASS_idsc_clkrst_bypass_START (12)
#define SOC_NPUCRG_IPCLKRST_BYPASS_idsc_clkrst_bypass_END (12)
#define SOC_NPUCRG_IPCLKRST_BYPASS_easc3_clkrst_bypass_START (13)
#define SOC_NPUCRG_IPCLKRST_BYPASS_easc3_clkrst_bypass_END (13)
#define SOC_NPUCRG_IPCLKRST_BYPASS_easc2_clkrst_bypass_START (14)
#define SOC_NPUCRG_IPCLKRST_BYPASS_easc2_clkrst_bypass_END (14)
#define SOC_NPUCRG_IPCLKRST_BYPASS_easc1_clkrst_bypass_START (15)
#define SOC_NPUCRG_IPCLKRST_BYPASS_easc1_clkrst_bypass_END (15)
#define SOC_NPUCRG_IPCLKRST_BYPASS_easc0_clkrst_bypass_START (16)
#define SOC_NPUCRG_IPCLKRST_BYPASS_easc0_clkrst_bypass_END (16)
#define SOC_NPUCRG_IPCLKRST_BYPASS_pcr_clkrst_bypass_START (17)
#define SOC_NPUCRG_IPCLKRST_BYPASS_pcr_clkrst_bypass_END (17)
#define SOC_NPUCRG_IPCLKRST_BYPASS_npu_tcu_clkrst_bypass_START (18)
#define SOC_NPUCRG_IPCLKRST_BYPASS_npu_tcu_clkrst_bypass_END (18)
#define SOC_NPUCRG_IPCLKRST_BYPASS_loadmonitor_clkrst_bypass_START (19)
#define SOC_NPUCRG_IPCLKRST_BYPASS_loadmonitor_clkrst_bypass_END (19)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int npubus_noc_ddrc_bypass : 1;
        unsigned int npubus_noc_tcu_bypass : 1;
        unsigned int npubus_noc_sysdma_mst_bypass : 1;
        unsigned int npubus_noc_ts_mst1_bypass : 1;
        unsigned int npubus_noc_tiny_bypass : 1;
        unsigned int npubus_noc_aicore1_mst_bypass : 1;
        unsigned int npubus_noc_aicore0_mst_bypass : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int reserved_8 : 1;
        unsigned int reserved_9 : 16;
    } reg;
} SOC_NPUCRG_PERI_AUTODIV0_UNION;
#endif
#define SOC_NPUCRG_PERI_AUTODIV0_npubus_noc_ddrc_bypass_START (0)
#define SOC_NPUCRG_PERI_AUTODIV0_npubus_noc_ddrc_bypass_END (0)
#define SOC_NPUCRG_PERI_AUTODIV0_npubus_noc_tcu_bypass_START (1)
#define SOC_NPUCRG_PERI_AUTODIV0_npubus_noc_tcu_bypass_END (1)
#define SOC_NPUCRG_PERI_AUTODIV0_npubus_noc_sysdma_mst_bypass_START (2)
#define SOC_NPUCRG_PERI_AUTODIV0_npubus_noc_sysdma_mst_bypass_END (2)
#define SOC_NPUCRG_PERI_AUTODIV0_npubus_noc_ts_mst1_bypass_START (3)
#define SOC_NPUCRG_PERI_AUTODIV0_npubus_noc_ts_mst1_bypass_END (3)
#define SOC_NPUCRG_PERI_AUTODIV0_npubus_noc_tiny_bypass_START (4)
#define SOC_NPUCRG_PERI_AUTODIV0_npubus_noc_tiny_bypass_END (4)
#define SOC_NPUCRG_PERI_AUTODIV0_npubus_noc_aicore1_mst_bypass_START (5)
#define SOC_NPUCRG_PERI_AUTODIV0_npubus_noc_aicore1_mst_bypass_END (5)
#define SOC_NPUCRG_PERI_AUTODIV0_npubus_noc_aicore0_mst_bypass_START (6)
#define SOC_NPUCRG_PERI_AUTODIV0_npubus_noc_aicore0_mst_bypass_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int npubus_cfg_div_auto_reduce_bypass : 1;
        unsigned int npubus_cfg_auto_waitcfg_in : 10;
        unsigned int npubus_cfg_auto_waitcfg_out : 10;
        unsigned int npubus_cfg_div_auto_cfg : 6;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 4;
    } reg;
} SOC_NPUCRG_PERI_AUTODIV1_UNION;
#endif
#define SOC_NPUCRG_PERI_AUTODIV1_npubus_cfg_div_auto_reduce_bypass_START (0)
#define SOC_NPUCRG_PERI_AUTODIV1_npubus_cfg_div_auto_reduce_bypass_END (0)
#define SOC_NPUCRG_PERI_AUTODIV1_npubus_cfg_auto_waitcfg_in_START (1)
#define SOC_NPUCRG_PERI_AUTODIV1_npubus_cfg_auto_waitcfg_in_END (10)
#define SOC_NPUCRG_PERI_AUTODIV1_npubus_cfg_auto_waitcfg_out_START (11)
#define SOC_NPUCRG_PERI_AUTODIV1_npubus_cfg_auto_waitcfg_out_END (20)
#define SOC_NPUCRG_PERI_AUTODIV1_npubus_cfg_div_auto_cfg_START (21)
#define SOC_NPUCRG_PERI_AUTODIV1_npubus_cfg_div_auto_cfg_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int npubus_div_auto_reduce_bypass : 1;
        unsigned int npubus_auto_waitcfg_in : 10;
        unsigned int npubus_auto_waitcfg_out : 10;
        unsigned int npubus_div_auto_cfg : 6;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 4;
    } reg;
} SOC_NPUCRG_PERI_AUTODIV2_UNION;
#endif
#define SOC_NPUCRG_PERI_AUTODIV2_npubus_div_auto_reduce_bypass_START (0)
#define SOC_NPUCRG_PERI_AUTODIV2_npubus_div_auto_reduce_bypass_END (0)
#define SOC_NPUCRG_PERI_AUTODIV2_npubus_auto_waitcfg_in_START (1)
#define SOC_NPUCRG_PERI_AUTODIV2_npubus_auto_waitcfg_in_END (10)
#define SOC_NPUCRG_PERI_AUTODIV2_npubus_auto_waitcfg_out_START (11)
#define SOC_NPUCRG_PERI_AUTODIV2_npubus_auto_waitcfg_out_END (20)
#define SOC_NPUCRG_PERI_AUTODIV2_npubus_div_auto_cfg_START (21)
#define SOC_NPUCRG_PERI_AUTODIV2_npubus_div_auto_cfg_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int autodiv_npubus_stat : 1;
        unsigned int autodiv_npubus_cfg_stat : 1;
        unsigned int autodiv_npubus_aicore_stat : 1;
        unsigned int reserved : 29;
    } reg;
} SOC_NPUCRG_PERI_AUTODIV_INUSE_STAT_UNION;
#endif
#define SOC_NPUCRG_PERI_AUTODIV_INUSE_STAT_autodiv_npubus_stat_START (0)
#define SOC_NPUCRG_PERI_AUTODIV_INUSE_STAT_autodiv_npubus_stat_END (0)
#define SOC_NPUCRG_PERI_AUTODIV_INUSE_STAT_autodiv_npubus_cfg_stat_START (1)
#define SOC_NPUCRG_PERI_AUTODIV_INUSE_STAT_autodiv_npubus_cfg_stat_END (1)
#define SOC_NPUCRG_PERI_AUTODIV_INUSE_STAT_autodiv_npubus_aicore_stat_START (2)
#define SOC_NPUCRG_PERI_AUTODIV_INUSE_STAT_autodiv_npubus_aicore_stat_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int npubus_cfg_sysdma_mst_bypass : 1;
        unsigned int npubus_cfg_ts_mst0_bypass : 1;
        unsigned int npubus_cfg_tiny_bypass : 1;
        unsigned int npubus_cfg_hwts_cfg_bypass : 1;
        unsigned int npubus_cfg_sys2npubus_cfg_bypass : 1;
        unsigned int npubus_cfg_ts_cfg_bypass : 1;
        unsigned int npubus_cfg_tiny_cfg_bypass : 1;
        unsigned int npubus_cfg_npu2sysbus_cfg_bypass : 1;
        unsigned int npubus_cfg_npubus_cfg_bypass : 1;
        unsigned int npubus_cfg_aicore1_cfg_bypass : 1;
        unsigned int npubus_cfg_aicore0_cfg_bypass : 1;
        unsigned int npubus_cfg_aicore1_mst_bypass : 1;
        unsigned int npubus_cfg_aicore0_mst_bypass : 1;
        unsigned int npubus_cfg_ts_mst1_bypass : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 16;
    } reg;
} SOC_NPUCRG_PERI_AUTODIV3_UNION;
#endif
#define SOC_NPUCRG_PERI_AUTODIV3_npubus_cfg_sysdma_mst_bypass_START (0)
#define SOC_NPUCRG_PERI_AUTODIV3_npubus_cfg_sysdma_mst_bypass_END (0)
#define SOC_NPUCRG_PERI_AUTODIV3_npubus_cfg_ts_mst0_bypass_START (1)
#define SOC_NPUCRG_PERI_AUTODIV3_npubus_cfg_ts_mst0_bypass_END (1)
#define SOC_NPUCRG_PERI_AUTODIV3_npubus_cfg_tiny_bypass_START (2)
#define SOC_NPUCRG_PERI_AUTODIV3_npubus_cfg_tiny_bypass_END (2)
#define SOC_NPUCRG_PERI_AUTODIV3_npubus_cfg_hwts_cfg_bypass_START (3)
#define SOC_NPUCRG_PERI_AUTODIV3_npubus_cfg_hwts_cfg_bypass_END (3)
#define SOC_NPUCRG_PERI_AUTODIV3_npubus_cfg_sys2npubus_cfg_bypass_START (4)
#define SOC_NPUCRG_PERI_AUTODIV3_npubus_cfg_sys2npubus_cfg_bypass_END (4)
#define SOC_NPUCRG_PERI_AUTODIV3_npubus_cfg_ts_cfg_bypass_START (5)
#define SOC_NPUCRG_PERI_AUTODIV3_npubus_cfg_ts_cfg_bypass_END (5)
#define SOC_NPUCRG_PERI_AUTODIV3_npubus_cfg_tiny_cfg_bypass_START (6)
#define SOC_NPUCRG_PERI_AUTODIV3_npubus_cfg_tiny_cfg_bypass_END (6)
#define SOC_NPUCRG_PERI_AUTODIV3_npubus_cfg_npu2sysbus_cfg_bypass_START (7)
#define SOC_NPUCRG_PERI_AUTODIV3_npubus_cfg_npu2sysbus_cfg_bypass_END (7)
#define SOC_NPUCRG_PERI_AUTODIV3_npubus_cfg_npubus_cfg_bypass_START (8)
#define SOC_NPUCRG_PERI_AUTODIV3_npubus_cfg_npubus_cfg_bypass_END (8)
#define SOC_NPUCRG_PERI_AUTODIV3_npubus_cfg_aicore1_cfg_bypass_START (9)
#define SOC_NPUCRG_PERI_AUTODIV3_npubus_cfg_aicore1_cfg_bypass_END (9)
#define SOC_NPUCRG_PERI_AUTODIV3_npubus_cfg_aicore0_cfg_bypass_START (10)
#define SOC_NPUCRG_PERI_AUTODIV3_npubus_cfg_aicore0_cfg_bypass_END (10)
#define SOC_NPUCRG_PERI_AUTODIV3_npubus_cfg_aicore1_mst_bypass_START (11)
#define SOC_NPUCRG_PERI_AUTODIV3_npubus_cfg_aicore1_mst_bypass_END (11)
#define SOC_NPUCRG_PERI_AUTODIV3_npubus_cfg_aicore0_mst_bypass_START (12)
#define SOC_NPUCRG_PERI_AUTODIV3_npubus_cfg_aicore0_mst_bypass_END (12)
#define SOC_NPUCRG_PERI_AUTODIV3_npubus_cfg_ts_mst1_bypass_START (13)
#define SOC_NPUCRG_PERI_AUTODIV3_npubus_cfg_ts_mst1_bypass_END (13)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int npubus_aicore_noc_ddrc_bypass : 1;
        unsigned int npubus_aicore_noc_l2buffer_bypass : 1;
        unsigned int npubus_aicore_noc_tcu_bypass : 1;
        unsigned int npubus_aicore_noc_sysdma_mst_bypass : 1;
        unsigned int npubus_aicore_noc_ts_mst1_bypass : 1;
        unsigned int npubus_aicore_noc_ts_mst0_bypass : 1;
        unsigned int npubus_aicore_noc_tiny_bypass : 1;
        unsigned int npubus_aicore_noc_aicore1_mst_bypass : 1;
        unsigned int npubus_aicore_noc_aicore0_mst_bypass : 1;
        unsigned int npubus_aicore_noc_sys2npubus_cfg_bypass : 1;
        unsigned int npubus_aicore_noc_aicore1_cfg_bypass : 1;
        unsigned int npubus_aicore_noc_aicore0_cfg_bypass : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 16;
    } reg;
} SOC_NPUCRG_PERI_AUTODIV4_UNION;
#endif
#define SOC_NPUCRG_PERI_AUTODIV4_npubus_aicore_noc_ddrc_bypass_START (0)
#define SOC_NPUCRG_PERI_AUTODIV4_npubus_aicore_noc_ddrc_bypass_END (0)
#define SOC_NPUCRG_PERI_AUTODIV4_npubus_aicore_noc_l2buffer_bypass_START (1)
#define SOC_NPUCRG_PERI_AUTODIV4_npubus_aicore_noc_l2buffer_bypass_END (1)
#define SOC_NPUCRG_PERI_AUTODIV4_npubus_aicore_noc_tcu_bypass_START (2)
#define SOC_NPUCRG_PERI_AUTODIV4_npubus_aicore_noc_tcu_bypass_END (2)
#define SOC_NPUCRG_PERI_AUTODIV4_npubus_aicore_noc_sysdma_mst_bypass_START (3)
#define SOC_NPUCRG_PERI_AUTODIV4_npubus_aicore_noc_sysdma_mst_bypass_END (3)
#define SOC_NPUCRG_PERI_AUTODIV4_npubus_aicore_noc_ts_mst1_bypass_START (4)
#define SOC_NPUCRG_PERI_AUTODIV4_npubus_aicore_noc_ts_mst1_bypass_END (4)
#define SOC_NPUCRG_PERI_AUTODIV4_npubus_aicore_noc_ts_mst0_bypass_START (5)
#define SOC_NPUCRG_PERI_AUTODIV4_npubus_aicore_noc_ts_mst0_bypass_END (5)
#define SOC_NPUCRG_PERI_AUTODIV4_npubus_aicore_noc_tiny_bypass_START (6)
#define SOC_NPUCRG_PERI_AUTODIV4_npubus_aicore_noc_tiny_bypass_END (6)
#define SOC_NPUCRG_PERI_AUTODIV4_npubus_aicore_noc_aicore1_mst_bypass_START (7)
#define SOC_NPUCRG_PERI_AUTODIV4_npubus_aicore_noc_aicore1_mst_bypass_END (7)
#define SOC_NPUCRG_PERI_AUTODIV4_npubus_aicore_noc_aicore0_mst_bypass_START (8)
#define SOC_NPUCRG_PERI_AUTODIV4_npubus_aicore_noc_aicore0_mst_bypass_END (8)
#define SOC_NPUCRG_PERI_AUTODIV4_npubus_aicore_noc_sys2npubus_cfg_bypass_START (9)
#define SOC_NPUCRG_PERI_AUTODIV4_npubus_aicore_noc_sys2npubus_cfg_bypass_END (9)
#define SOC_NPUCRG_PERI_AUTODIV4_npubus_aicore_noc_aicore1_cfg_bypass_START (10)
#define SOC_NPUCRG_PERI_AUTODIV4_npubus_aicore_noc_aicore1_cfg_bypass_END (10)
#define SOC_NPUCRG_PERI_AUTODIV4_npubus_aicore_noc_aicore0_cfg_bypass_START (11)
#define SOC_NPUCRG_PERI_AUTODIV4_npubus_aicore_noc_aicore0_cfg_bypass_END (11)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int npubus_aicore_div_auto_reduce_bypass_lpm3 : 1;
        unsigned int npubus_aicore_auto_waitcfg_in : 10;
        unsigned int npubus_aicore_auto_waitcfg_out : 10;
        unsigned int npubus_aicore_div_auto_cfg : 6;
        unsigned int npubus_aicore_div_auto_reduce_bypass_acpu : 1;
        unsigned int npubus_aicore_div_auto_reduce_bypass_mcpu : 1;
        unsigned int reserved : 3;
    } reg;
} SOC_NPUCRG_PERI_AUTODIV5_UNION;
#endif
#define SOC_NPUCRG_PERI_AUTODIV5_npubus_aicore_div_auto_reduce_bypass_lpm3_START (0)
#define SOC_NPUCRG_PERI_AUTODIV5_npubus_aicore_div_auto_reduce_bypass_lpm3_END (0)
#define SOC_NPUCRG_PERI_AUTODIV5_npubus_aicore_auto_waitcfg_in_START (1)
#define SOC_NPUCRG_PERI_AUTODIV5_npubus_aicore_auto_waitcfg_in_END (10)
#define SOC_NPUCRG_PERI_AUTODIV5_npubus_aicore_auto_waitcfg_out_START (11)
#define SOC_NPUCRG_PERI_AUTODIV5_npubus_aicore_auto_waitcfg_out_END (20)
#define SOC_NPUCRG_PERI_AUTODIV5_npubus_aicore_div_auto_cfg_START (21)
#define SOC_NPUCRG_PERI_AUTODIV5_npubus_aicore_div_auto_cfg_END (26)
#define SOC_NPUCRG_PERI_AUTODIV5_npubus_aicore_div_auto_reduce_bypass_acpu_START (27)
#define SOC_NPUCRG_PERI_AUTODIV5_npubus_aicore_div_auto_reduce_bypass_acpu_END (27)
#define SOC_NPUCRG_PERI_AUTODIV5_npubus_aicore_div_auto_reduce_bypass_mcpu_START (28)
#define SOC_NPUCRG_PERI_AUTODIV5_npubus_aicore_div_auto_reduce_bypass_mcpu_END (28)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ppll5_en : 1;
        unsigned int ppll5_bp : 1;
        unsigned int ppll5_refdiv : 6;
        unsigned int ppll5_fbdiv : 12;
        unsigned int ppll5_postdiv1 : 3;
        unsigned int ppll5_postdiv2 : 3;
        unsigned int ppll5_lock : 1;
        unsigned int reserved : 5;
    } reg;
} SOC_NPUCRG_PPLL5CTRL0_UNION;
#endif
#define SOC_NPUCRG_PPLL5CTRL0_ppll5_en_START (0)
#define SOC_NPUCRG_PPLL5CTRL0_ppll5_en_END (0)
#define SOC_NPUCRG_PPLL5CTRL0_ppll5_bp_START (1)
#define SOC_NPUCRG_PPLL5CTRL0_ppll5_bp_END (1)
#define SOC_NPUCRG_PPLL5CTRL0_ppll5_refdiv_START (2)
#define SOC_NPUCRG_PPLL5CTRL0_ppll5_refdiv_END (7)
#define SOC_NPUCRG_PPLL5CTRL0_ppll5_fbdiv_START (8)
#define SOC_NPUCRG_PPLL5CTRL0_ppll5_fbdiv_END (19)
#define SOC_NPUCRG_PPLL5CTRL0_ppll5_postdiv1_START (20)
#define SOC_NPUCRG_PPLL5CTRL0_ppll5_postdiv1_END (22)
#define SOC_NPUCRG_PPLL5CTRL0_ppll5_postdiv2_START (23)
#define SOC_NPUCRG_PPLL5CTRL0_ppll5_postdiv2_END (25)
#define SOC_NPUCRG_PPLL5CTRL0_ppll5_lock_START (26)
#define SOC_NPUCRG_PPLL5CTRL0_ppll5_lock_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ppll5_fracdiv : 24;
        unsigned int ppll5_int_mod : 1;
        unsigned int ppll5_cfg_vld : 1;
        unsigned int gt_clk_ppll5 : 1;
        unsigned int reserved : 5;
    } reg;
} SOC_NPUCRG_PPLL5CTRL1_UNION;
#endif
#define SOC_NPUCRG_PPLL5CTRL1_ppll5_fracdiv_START (0)
#define SOC_NPUCRG_PPLL5CTRL1_ppll5_fracdiv_END (23)
#define SOC_NPUCRG_PPLL5CTRL1_ppll5_int_mod_START (24)
#define SOC_NPUCRG_PPLL5CTRL1_ppll5_int_mod_END (24)
#define SOC_NPUCRG_PPLL5CTRL1_ppll5_cfg_vld_START (25)
#define SOC_NPUCRG_PPLL5CTRL1_ppll5_cfg_vld_END (25)
#define SOC_NPUCRG_PPLL5CTRL1_gt_clk_ppll5_START (26)
#define SOC_NPUCRG_PPLL5CTRL1_gt_clk_ppll5_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pll_bbdrv : 3;
        unsigned int pll_bben : 1;
        unsigned int pll_bbenoverride : 1;
        unsigned int pll_bbpw : 3;
        unsigned int pll_bwprog : 3;
        unsigned int pll_bwprogen : 1;
        unsigned int pll_foutvco_en : 1;
        unsigned int pll_offsetcalbyp : 1;
        unsigned int pll_offsetcalcnt : 3;
        unsigned int pll_offsetcalen : 1;
        unsigned int pll_offsetcalin : 12;
        unsigned int pll_offsetfastcal : 1;
        unsigned int reserved : 1;
    } reg;
} SOC_NPUCRG_PPLL5_CTRL_A_UNION;
#endif
#define SOC_NPUCRG_PPLL5_CTRL_A_pll_bbdrv_START (0)
#define SOC_NPUCRG_PPLL5_CTRL_A_pll_bbdrv_END (2)
#define SOC_NPUCRG_PPLL5_CTRL_A_pll_bben_START (3)
#define SOC_NPUCRG_PPLL5_CTRL_A_pll_bben_END (3)
#define SOC_NPUCRG_PPLL5_CTRL_A_pll_bbenoverride_START (4)
#define SOC_NPUCRG_PPLL5_CTRL_A_pll_bbenoverride_END (4)
#define SOC_NPUCRG_PPLL5_CTRL_A_pll_bbpw_START (5)
#define SOC_NPUCRG_PPLL5_CTRL_A_pll_bbpw_END (7)
#define SOC_NPUCRG_PPLL5_CTRL_A_pll_bwprog_START (8)
#define SOC_NPUCRG_PPLL5_CTRL_A_pll_bwprog_END (10)
#define SOC_NPUCRG_PPLL5_CTRL_A_pll_bwprogen_START (11)
#define SOC_NPUCRG_PPLL5_CTRL_A_pll_bwprogen_END (11)
#define SOC_NPUCRG_PPLL5_CTRL_A_pll_foutvco_en_START (12)
#define SOC_NPUCRG_PPLL5_CTRL_A_pll_foutvco_en_END (12)
#define SOC_NPUCRG_PPLL5_CTRL_A_pll_offsetcalbyp_START (13)
#define SOC_NPUCRG_PPLL5_CTRL_A_pll_offsetcalbyp_END (13)
#define SOC_NPUCRG_PPLL5_CTRL_A_pll_offsetcalcnt_START (14)
#define SOC_NPUCRG_PPLL5_CTRL_A_pll_offsetcalcnt_END (16)
#define SOC_NPUCRG_PPLL5_CTRL_A_pll_offsetcalen_START (17)
#define SOC_NPUCRG_PPLL5_CTRL_A_pll_offsetcalen_END (17)
#define SOC_NPUCRG_PPLL5_CTRL_A_pll_offsetcalin_START (18)
#define SOC_NPUCRG_PPLL5_CTRL_A_pll_offsetcalin_END (29)
#define SOC_NPUCRG_PPLL5_CTRL_A_pll_offsetfastcal_START (30)
#define SOC_NPUCRG_PPLL5_CTRL_A_pll_offsetfastcal_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pll_vcosel : 2;
        unsigned int pll_qpiprog : 3;
        unsigned int pll_qpiprogen : 1;
        unsigned int pll_qpiskewprog : 3;
        unsigned int pll_qpiskewprogen : 1;
        unsigned int pll_fprog : 3;
        unsigned int pll_fprogen : 1;
        unsigned int reserved : 2;
        unsigned int pll_reserved : 16;
    } reg;
} SOC_NPUCRG_PPLL5_CTRL_B_UNION;
#endif
#define SOC_NPUCRG_PPLL5_CTRL_B_pll_vcosel_START (0)
#define SOC_NPUCRG_PPLL5_CTRL_B_pll_vcosel_END (1)
#define SOC_NPUCRG_PPLL5_CTRL_B_pll_qpiprog_START (2)
#define SOC_NPUCRG_PPLL5_CTRL_B_pll_qpiprog_END (4)
#define SOC_NPUCRG_PPLL5_CTRL_B_pll_qpiprogen_START (5)
#define SOC_NPUCRG_PPLL5_CTRL_B_pll_qpiprogen_END (5)
#define SOC_NPUCRG_PPLL5_CTRL_B_pll_qpiskewprog_START (6)
#define SOC_NPUCRG_PPLL5_CTRL_B_pll_qpiskewprog_END (8)
#define SOC_NPUCRG_PPLL5_CTRL_B_pll_qpiskewprogen_START (9)
#define SOC_NPUCRG_PPLL5_CTRL_B_pll_qpiskewprogen_END (9)
#define SOC_NPUCRG_PPLL5_CTRL_B_pll_fprog_START (10)
#define SOC_NPUCRG_PPLL5_CTRL_B_pll_fprog_END (12)
#define SOC_NPUCRG_PPLL5_CTRL_B_pll_fprogen_START (13)
#define SOC_NPUCRG_PPLL5_CTRL_B_pll_fprogen_END (13)
#define SOC_NPUCRG_PPLL5_CTRL_B_pll_reserved_START (16)
#define SOC_NPUCRG_PPLL5_CTRL_B_pll_reserved_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ppll5_ssc_reset : 1;
        unsigned int ppll5_ssc_disable : 1;
        unsigned int ppll5_ssc_downspread : 1;
        unsigned int ppll5_ssc_spread : 3;
        unsigned int reserved_0 : 2;
        unsigned int ppll5_ssc_divval : 4;
        unsigned int reserved_1 : 2;
        unsigned int ppll5_ssmod_rstnptr : 1;
        unsigned int ppll5_ssc_self_vld : 1;
        unsigned int reserved_2 : 16;
    } reg;
} SOC_NPUCRG_PPLL5SSCCTRL_UNION;
#endif
#define SOC_NPUCRG_PPLL5SSCCTRL_ppll5_ssc_reset_START (0)
#define SOC_NPUCRG_PPLL5SSCCTRL_ppll5_ssc_reset_END (0)
#define SOC_NPUCRG_PPLL5SSCCTRL_ppll5_ssc_disable_START (1)
#define SOC_NPUCRG_PPLL5SSCCTRL_ppll5_ssc_disable_END (1)
#define SOC_NPUCRG_PPLL5SSCCTRL_ppll5_ssc_downspread_START (2)
#define SOC_NPUCRG_PPLL5SSCCTRL_ppll5_ssc_downspread_END (2)
#define SOC_NPUCRG_PPLL5SSCCTRL_ppll5_ssc_spread_START (3)
#define SOC_NPUCRG_PPLL5SSCCTRL_ppll5_ssc_spread_END (5)
#define SOC_NPUCRG_PPLL5SSCCTRL_ppll5_ssc_divval_START (8)
#define SOC_NPUCRG_PPLL5SSCCTRL_ppll5_ssc_divval_END (11)
#define SOC_NPUCRG_PPLL5SSCCTRL_ppll5_ssmod_rstnptr_START (14)
#define SOC_NPUCRG_PPLL5SSCCTRL_ppll5_ssmod_rstnptr_END (14)
#define SOC_NPUCRG_PPLL5SSCCTRL_ppll5_ssc_self_vld_START (15)
#define SOC_NPUCRG_PPLL5SSCCTRL_ppll5_ssc_self_vld_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ppll5_gt_stat : 1;
        unsigned int ppll5_bypass : 1;
        unsigned int ppll5_en_stat : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 26;
    } reg;
} SOC_NPUCRG_PLL_VOTE_STAT_UNION;
#endif
#define SOC_NPUCRG_PLL_VOTE_STAT_ppll5_gt_stat_START (0)
#define SOC_NPUCRG_PLL_VOTE_STAT_ppll5_gt_stat_END (0)
#define SOC_NPUCRG_PLL_VOTE_STAT_ppll5_bypass_START (1)
#define SOC_NPUCRG_PLL_VOTE_STAT_ppll5_bypass_END (1)
#define SOC_NPUCRG_PLL_VOTE_STAT_ppll5_en_stat_START (2)
#define SOC_NPUCRG_PLL_VOTE_STAT_ppll5_en_stat_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ppll5_en_vote : 5;
        unsigned int reserved_0 : 5;
        unsigned int reserved_1 : 5;
        unsigned int reserved_2 : 1;
        unsigned int bitmasken : 16;
    } reg;
} SOC_NPUCRG_PLL_EN_VOTE_CTRL_UNION;
#endif
#define SOC_NPUCRG_PLL_EN_VOTE_CTRL_ppll5_en_vote_START (0)
#define SOC_NPUCRG_PLL_EN_VOTE_CTRL_ppll5_en_vote_END (4)
#define SOC_NPUCRG_PLL_EN_VOTE_CTRL_bitmasken_START (16)
#define SOC_NPUCRG_PLL_EN_VOTE_CTRL_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ppll5_gt_vote : 5;
        unsigned int reserved_0 : 5;
        unsigned int reserved_1 : 5;
        unsigned int reserved_2 : 1;
        unsigned int bitmasken : 16;
    } reg;
} SOC_NPUCRG_PLL_GT_VOTE_CTRL_UNION;
#endif
#define SOC_NPUCRG_PLL_GT_VOTE_CTRL_ppll5_gt_vote_START (0)
#define SOC_NPUCRG_PLL_GT_VOTE_CTRL_ppll5_gt_vote_END (4)
#define SOC_NPUCRG_PLL_GT_VOTE_CTRL_bitmasken_START (16)
#define SOC_NPUCRG_PLL_GT_VOTE_CTRL_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ppll5_bypass_vote : 5;
        unsigned int reserved_0 : 5;
        unsigned int reserved_1 : 5;
        unsigned int reserved_2 : 1;
        unsigned int bitmasken : 16;
    } reg;
} SOC_NPUCRG_PLL_BYPASS_VOTE_CTRL_UNION;
#endif
#define SOC_NPUCRG_PLL_BYPASS_VOTE_CTRL_ppll5_bypass_vote_START (0)
#define SOC_NPUCRG_PLL_BYPASS_VOTE_CTRL_ppll5_bypass_vote_END (4)
#define SOC_NPUCRG_PLL_BYPASS_VOTE_CTRL_bitmasken_START (16)
#define SOC_NPUCRG_PLL_BYPASS_VOTE_CTRL_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 11;
        unsigned int div2_pd_test_fnpll_1v2 : 1;
        unsigned int fbdiv_rst_n_test_fnpll_1v2 : 1;
        unsigned int refdiv_rst_n_test_fnpll_1v2 : 1;
        unsigned int ac_buf_pd : 1;
        unsigned int dc_buf_pd : 1;
        unsigned int pll_lock_fnpll_1v2 : 1;
        unsigned int pll_lock_ate_fnpll_1v2 : 1;
        unsigned int bbpd_calib_done_fnpll_1v2 : 1;
        unsigned int bbpd_calib_fail_fnpll_1v2 : 1;
        unsigned int bbpd_calib_success_fnpll_1v2 : 1;
        unsigned int pll_unlock_fnpll_1v2 : 1;
        unsigned int clock_lost_fnpll_1v2 : 1;
        unsigned int ref_lost_flag_fnpll_1v2 : 1;
        unsigned int test_data_fnpll_1v2 : 8;
    } reg;
} SOC_NPUCRG_FNPLL_STAT0_UNION;
#endif
#define SOC_NPUCRG_FNPLL_STAT0_div2_pd_test_fnpll_1v2_START (11)
#define SOC_NPUCRG_FNPLL_STAT0_div2_pd_test_fnpll_1v2_END (11)
#define SOC_NPUCRG_FNPLL_STAT0_fbdiv_rst_n_test_fnpll_1v2_START (12)
#define SOC_NPUCRG_FNPLL_STAT0_fbdiv_rst_n_test_fnpll_1v2_END (12)
#define SOC_NPUCRG_FNPLL_STAT0_refdiv_rst_n_test_fnpll_1v2_START (13)
#define SOC_NPUCRG_FNPLL_STAT0_refdiv_rst_n_test_fnpll_1v2_END (13)
#define SOC_NPUCRG_FNPLL_STAT0_ac_buf_pd_START (14)
#define SOC_NPUCRG_FNPLL_STAT0_ac_buf_pd_END (14)
#define SOC_NPUCRG_FNPLL_STAT0_dc_buf_pd_START (15)
#define SOC_NPUCRG_FNPLL_STAT0_dc_buf_pd_END (15)
#define SOC_NPUCRG_FNPLL_STAT0_pll_lock_fnpll_1v2_START (16)
#define SOC_NPUCRG_FNPLL_STAT0_pll_lock_fnpll_1v2_END (16)
#define SOC_NPUCRG_FNPLL_STAT0_pll_lock_ate_fnpll_1v2_START (17)
#define SOC_NPUCRG_FNPLL_STAT0_pll_lock_ate_fnpll_1v2_END (17)
#define SOC_NPUCRG_FNPLL_STAT0_bbpd_calib_done_fnpll_1v2_START (18)
#define SOC_NPUCRG_FNPLL_STAT0_bbpd_calib_done_fnpll_1v2_END (18)
#define SOC_NPUCRG_FNPLL_STAT0_bbpd_calib_fail_fnpll_1v2_START (19)
#define SOC_NPUCRG_FNPLL_STAT0_bbpd_calib_fail_fnpll_1v2_END (19)
#define SOC_NPUCRG_FNPLL_STAT0_bbpd_calib_success_fnpll_1v2_START (20)
#define SOC_NPUCRG_FNPLL_STAT0_bbpd_calib_success_fnpll_1v2_END (20)
#define SOC_NPUCRG_FNPLL_STAT0_pll_unlock_fnpll_1v2_START (21)
#define SOC_NPUCRG_FNPLL_STAT0_pll_unlock_fnpll_1v2_END (21)
#define SOC_NPUCRG_FNPLL_STAT0_clock_lost_fnpll_1v2_START (22)
#define SOC_NPUCRG_FNPLL_STAT0_clock_lost_fnpll_1v2_END (22)
#define SOC_NPUCRG_FNPLL_STAT0_ref_lost_flag_fnpll_1v2_START (23)
#define SOC_NPUCRG_FNPLL_STAT0_ref_lost_flag_fnpll_1v2_END (23)
#define SOC_NPUCRG_FNPLL_STAT0_test_data_fnpll_1v2_START (24)
#define SOC_NPUCRG_FNPLL_STAT0_test_data_fnpll_1v2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 5;
        unsigned int unlock_count_fnpll_1v2 : 4;
        unsigned int unlock_flag_fnpll_1v2 : 1;
        unsigned int reserved_1 : 2;
        unsigned int lock_counter_fnpll_1v2 : 15;
        unsigned int reserved_2 : 5;
    } reg;
} SOC_NPUCRG_FNPLL_STAT1_UNION;
#endif
#define SOC_NPUCRG_FNPLL_STAT1_unlock_count_fnpll_1v2_START (5)
#define SOC_NPUCRG_FNPLL_STAT1_unlock_count_fnpll_1v2_END (8)
#define SOC_NPUCRG_FNPLL_STAT1_unlock_flag_fnpll_1v2_START (9)
#define SOC_NPUCRG_FNPLL_STAT1_unlock_flag_fnpll_1v2_END (9)
#define SOC_NPUCRG_FNPLL_STAT1_lock_counter_fnpll_1v2_START (12)
#define SOC_NPUCRG_FNPLL_STAT1_lock_counter_fnpll_1v2_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int phe_sel_en_fnpll_1v2 : 1;
        unsigned int dtc_test_fnpll_1v2 : 1;
        unsigned int sw_dc_buf_en : 1;
        unsigned int sw_ac_buf_en : 1;
        unsigned int phe_sel_in_fnpll_1v2 : 2;
        unsigned int unlock_clear : 1;
        unsigned int dc_ac_clk_en : 1;
        unsigned int dtc_m_cfg_fnpll_1v2 : 6;
        unsigned int ref_lost_clear_fnpll_1v2 : 1;
        unsigned int reserved : 1;
        unsigned int spread_en_fnpll_1v2 : 1;
        unsigned int downspread_fnpll_1v2 : 1;
        unsigned int sel_extwave_fnpll_1v2 : 1;
        unsigned int divval_fnpll_1v2 : 4;
        unsigned int spread_fnpll_1v2 : 3;
        unsigned int freq_threshold : 6;
    } reg;
} SOC_NPUCRG_FNPLL_CFG0_UNION;
#endif
#define SOC_NPUCRG_FNPLL_CFG0_phe_sel_en_fnpll_1v2_START (0)
#define SOC_NPUCRG_FNPLL_CFG0_phe_sel_en_fnpll_1v2_END (0)
#define SOC_NPUCRG_FNPLL_CFG0_dtc_test_fnpll_1v2_START (1)
#define SOC_NPUCRG_FNPLL_CFG0_dtc_test_fnpll_1v2_END (1)
#define SOC_NPUCRG_FNPLL_CFG0_sw_dc_buf_en_START (2)
#define SOC_NPUCRG_FNPLL_CFG0_sw_dc_buf_en_END (2)
#define SOC_NPUCRG_FNPLL_CFG0_sw_ac_buf_en_START (3)
#define SOC_NPUCRG_FNPLL_CFG0_sw_ac_buf_en_END (3)
#define SOC_NPUCRG_FNPLL_CFG0_phe_sel_in_fnpll_1v2_START (4)
#define SOC_NPUCRG_FNPLL_CFG0_phe_sel_in_fnpll_1v2_END (5)
#define SOC_NPUCRG_FNPLL_CFG0_unlock_clear_START (6)
#define SOC_NPUCRG_FNPLL_CFG0_unlock_clear_END (6)
#define SOC_NPUCRG_FNPLL_CFG0_dc_ac_clk_en_START (7)
#define SOC_NPUCRG_FNPLL_CFG0_dc_ac_clk_en_END (7)
#define SOC_NPUCRG_FNPLL_CFG0_dtc_m_cfg_fnpll_1v2_START (8)
#define SOC_NPUCRG_FNPLL_CFG0_dtc_m_cfg_fnpll_1v2_END (13)
#define SOC_NPUCRG_FNPLL_CFG0_ref_lost_clear_fnpll_1v2_START (14)
#define SOC_NPUCRG_FNPLL_CFG0_ref_lost_clear_fnpll_1v2_END (14)
#define SOC_NPUCRG_FNPLL_CFG0_spread_en_fnpll_1v2_START (16)
#define SOC_NPUCRG_FNPLL_CFG0_spread_en_fnpll_1v2_END (16)
#define SOC_NPUCRG_FNPLL_CFG0_downspread_fnpll_1v2_START (17)
#define SOC_NPUCRG_FNPLL_CFG0_downspread_fnpll_1v2_END (17)
#define SOC_NPUCRG_FNPLL_CFG0_sel_extwave_fnpll_1v2_START (18)
#define SOC_NPUCRG_FNPLL_CFG0_sel_extwave_fnpll_1v2_END (18)
#define SOC_NPUCRG_FNPLL_CFG0_divval_fnpll_1v2_START (19)
#define SOC_NPUCRG_FNPLL_CFG0_divval_fnpll_1v2_END (22)
#define SOC_NPUCRG_FNPLL_CFG0_spread_fnpll_1v2_START (23)
#define SOC_NPUCRG_FNPLL_CFG0_spread_fnpll_1v2_END (25)
#define SOC_NPUCRG_FNPLL_CFG0_freq_threshold_START (26)
#define SOC_NPUCRG_FNPLL_CFG0_freq_threshold_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ext_maxaddr_fnpll_1v2 : 8;
        unsigned int extwaveval_fnpll_1v2 : 8;
        unsigned int input_cfg_en_fnpll_1v2 : 1;
        unsigned int test_mode_fnpll_1v2 : 1;
        unsigned int lock_thr_fnpll_1v2 : 2;
        unsigned int refdiv_rst_n_fnpll_1v2 : 1;
        unsigned int fbdiv_rst_n_fnpll_1v2 : 1;
        unsigned int div2_pd_fnpll_1v2 : 1;
        unsigned int lock_en_fnpll_1v2 : 1;
        unsigned int fbdiv_delay_num_fnpll_1v2 : 7;
        unsigned int reserved : 1;
    } reg;
} SOC_NPUCRG_FNPLL_CFG1_UNION;
#endif
#define SOC_NPUCRG_FNPLL_CFG1_ext_maxaddr_fnpll_1v2_START (0)
#define SOC_NPUCRG_FNPLL_CFG1_ext_maxaddr_fnpll_1v2_END (7)
#define SOC_NPUCRG_FNPLL_CFG1_extwaveval_fnpll_1v2_START (8)
#define SOC_NPUCRG_FNPLL_CFG1_extwaveval_fnpll_1v2_END (15)
#define SOC_NPUCRG_FNPLL_CFG1_input_cfg_en_fnpll_1v2_START (16)
#define SOC_NPUCRG_FNPLL_CFG1_input_cfg_en_fnpll_1v2_END (16)
#define SOC_NPUCRG_FNPLL_CFG1_test_mode_fnpll_1v2_START (17)
#define SOC_NPUCRG_FNPLL_CFG1_test_mode_fnpll_1v2_END (17)
#define SOC_NPUCRG_FNPLL_CFG1_lock_thr_fnpll_1v2_START (18)
#define SOC_NPUCRG_FNPLL_CFG1_lock_thr_fnpll_1v2_END (19)
#define SOC_NPUCRG_FNPLL_CFG1_refdiv_rst_n_fnpll_1v2_START (20)
#define SOC_NPUCRG_FNPLL_CFG1_refdiv_rst_n_fnpll_1v2_END (20)
#define SOC_NPUCRG_FNPLL_CFG1_fbdiv_rst_n_fnpll_1v2_START (21)
#define SOC_NPUCRG_FNPLL_CFG1_fbdiv_rst_n_fnpll_1v2_END (21)
#define SOC_NPUCRG_FNPLL_CFG1_div2_pd_fnpll_1v2_START (22)
#define SOC_NPUCRG_FNPLL_CFG1_div2_pd_fnpll_1v2_END (22)
#define SOC_NPUCRG_FNPLL_CFG1_lock_en_fnpll_1v2_START (23)
#define SOC_NPUCRG_FNPLL_CFG1_lock_en_fnpll_1v2_END (23)
#define SOC_NPUCRG_FNPLL_CFG1_fbdiv_delay_num_fnpll_1v2_START (24)
#define SOC_NPUCRG_FNPLL_CFG1_fbdiv_delay_num_fnpll_1v2_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bbpd_calib_byp_fnpll_1v2 : 1;
        unsigned int dtc_ctrl_inv_fnpll_1v2 : 1;
        unsigned int dll_force_en_fnpll_1v2 : 1;
        unsigned int DLL_EN_FNPLL_1V2 : 1;
        unsigned int phe_code_a_fnpll_1v2 : 2;
        unsigned int phe_code_b_fnpll_1v2 : 2;
        unsigned int ctinue_lock_num_fnpll_1v2 : 6;
        unsigned int reserved_0 : 2;
        unsigned int read_en_fnpll_1v2 : 1;
        unsigned int k_gain_cfg_en_fnpll_1v2 : 1;
        unsigned int reserved_1 : 2;
        unsigned int k_gain_av_thr_fnpll_1v2 : 3;
        unsigned int pll_unlock_clr_fnpll_1v2 : 1;
        unsigned int k_gain_cfg_fnpll_1v2 : 6;
        unsigned int reserved_2 : 2;
    } reg;
} SOC_NPUCRG_FNPLL_CFG2_UNION;
#endif
#define SOC_NPUCRG_FNPLL_CFG2_bbpd_calib_byp_fnpll_1v2_START (0)
#define SOC_NPUCRG_FNPLL_CFG2_bbpd_calib_byp_fnpll_1v2_END (0)
#define SOC_NPUCRG_FNPLL_CFG2_dtc_ctrl_inv_fnpll_1v2_START (1)
#define SOC_NPUCRG_FNPLL_CFG2_dtc_ctrl_inv_fnpll_1v2_END (1)
#define SOC_NPUCRG_FNPLL_CFG2_dll_force_en_fnpll_1v2_START (2)
#define SOC_NPUCRG_FNPLL_CFG2_dll_force_en_fnpll_1v2_END (2)
#define SOC_NPUCRG_FNPLL_CFG2_DLL_EN_FNPLL_1V2_START (3)
#define SOC_NPUCRG_FNPLL_CFG2_DLL_EN_FNPLL_1V2_END (3)
#define SOC_NPUCRG_FNPLL_CFG2_phe_code_a_fnpll_1v2_START (4)
#define SOC_NPUCRG_FNPLL_CFG2_phe_code_a_fnpll_1v2_END (5)
#define SOC_NPUCRG_FNPLL_CFG2_phe_code_b_fnpll_1v2_START (6)
#define SOC_NPUCRG_FNPLL_CFG2_phe_code_b_fnpll_1v2_END (7)
#define SOC_NPUCRG_FNPLL_CFG2_ctinue_lock_num_fnpll_1v2_START (8)
#define SOC_NPUCRG_FNPLL_CFG2_ctinue_lock_num_fnpll_1v2_END (13)
#define SOC_NPUCRG_FNPLL_CFG2_read_en_fnpll_1v2_START (16)
#define SOC_NPUCRG_FNPLL_CFG2_read_en_fnpll_1v2_END (16)
#define SOC_NPUCRG_FNPLL_CFG2_k_gain_cfg_en_fnpll_1v2_START (17)
#define SOC_NPUCRG_FNPLL_CFG2_k_gain_cfg_en_fnpll_1v2_END (17)
#define SOC_NPUCRG_FNPLL_CFG2_k_gain_av_thr_fnpll_1v2_START (20)
#define SOC_NPUCRG_FNPLL_CFG2_k_gain_av_thr_fnpll_1v2_END (22)
#define SOC_NPUCRG_FNPLL_CFG2_pll_unlock_clr_fnpll_1v2_START (23)
#define SOC_NPUCRG_FNPLL_CFG2_pll_unlock_clr_fnpll_1v2_END (23)
#define SOC_NPUCRG_FNPLL_CFG2_k_gain_cfg_fnpll_1v2_START (24)
#define SOC_NPUCRG_FNPLL_CFG2_k_gain_cfg_fnpll_1v2_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dtc_o_cfg_fnpll_1v2 : 6;
        unsigned int reserved_0 : 2;
        unsigned int bbpd_lock_num_fnpll_1v2 : 8;
        unsigned int lpf_bw_fnpll_1v2 : 3;
        unsigned int reserved_1 : 1;
        unsigned int lpf_bw1_fnpll_1v2 : 3;
        unsigned int reserved_2 : 1;
        unsigned int calib_lpf_bw_fnpll_1v2 : 3;
        unsigned int reserved_3 : 1;
        unsigned int calib_lpf_bw1_fnpll_1v2 : 3;
        unsigned int reserved_4 : 1;
    } reg;
} SOC_NPUCRG_FNPLL_CFG3_UNION;
#endif
#define SOC_NPUCRG_FNPLL_CFG3_dtc_o_cfg_fnpll_1v2_START (0)
#define SOC_NPUCRG_FNPLL_CFG3_dtc_o_cfg_fnpll_1v2_END (5)
#define SOC_NPUCRG_FNPLL_CFG3_bbpd_lock_num_fnpll_1v2_START (8)
#define SOC_NPUCRG_FNPLL_CFG3_bbpd_lock_num_fnpll_1v2_END (15)
#define SOC_NPUCRG_FNPLL_CFG3_lpf_bw_fnpll_1v2_START (16)
#define SOC_NPUCRG_FNPLL_CFG3_lpf_bw_fnpll_1v2_END (18)
#define SOC_NPUCRG_FNPLL_CFG3_lpf_bw1_fnpll_1v2_START (20)
#define SOC_NPUCRG_FNPLL_CFG3_lpf_bw1_fnpll_1v2_END (22)
#define SOC_NPUCRG_FNPLL_CFG3_calib_lpf_bw_fnpll_1v2_START (24)
#define SOC_NPUCRG_FNPLL_CFG3_calib_lpf_bw_fnpll_1v2_END (26)
#define SOC_NPUCRG_FNPLL_CFG3_calib_lpf_bw1_fnpll_1v2_START (28)
#define SOC_NPUCRG_FNPLL_CFG3_calib_lpf_bw1_fnpll_1v2_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int max_k_gain_fnpll_1v2 : 6;
        unsigned int reserved_0 : 2;
        unsigned int min_k_gain_fnpll_1v2 : 6;
        unsigned int reserved_1 : 2;
        unsigned int en_dac_test_fnpll_1v2 : 1;
        unsigned int updn_sel_fnpll_1v2 : 1;
        unsigned int icp_ctrl_fnpll_1v2 : 1;
        unsigned int reserved_2 : 1;
        unsigned int vdc_sel_fnpll_1v2 : 4;
        unsigned int reserved_3 : 4;
        unsigned int test_data_sel_fnpll_1v2 : 4;
    } reg;
} SOC_NPUCRG_FNPLL_CFG4_UNION;
#endif
#define SOC_NPUCRG_FNPLL_CFG4_max_k_gain_fnpll_1v2_START (0)
#define SOC_NPUCRG_FNPLL_CFG4_max_k_gain_fnpll_1v2_END (5)
#define SOC_NPUCRG_FNPLL_CFG4_min_k_gain_fnpll_1v2_START (8)
#define SOC_NPUCRG_FNPLL_CFG4_min_k_gain_fnpll_1v2_END (13)
#define SOC_NPUCRG_FNPLL_CFG4_en_dac_test_fnpll_1v2_START (16)
#define SOC_NPUCRG_FNPLL_CFG4_en_dac_test_fnpll_1v2_END (16)
#define SOC_NPUCRG_FNPLL_CFG4_updn_sel_fnpll_1v2_START (17)
#define SOC_NPUCRG_FNPLL_CFG4_updn_sel_fnpll_1v2_END (17)
#define SOC_NPUCRG_FNPLL_CFG4_icp_ctrl_fnpll_1v2_START (18)
#define SOC_NPUCRG_FNPLL_CFG4_icp_ctrl_fnpll_1v2_END (18)
#define SOC_NPUCRG_FNPLL_CFG4_vdc_sel_fnpll_1v2_START (20)
#define SOC_NPUCRG_FNPLL_CFG4_vdc_sel_fnpll_1v2_END (23)
#define SOC_NPUCRG_FNPLL_CFG4_test_data_sel_fnpll_1v2_START (28)
#define SOC_NPUCRG_FNPLL_CFG4_test_data_sel_fnpll_1v2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ref_lost_thr_fnpll_1v2 : 8;
        unsigned int pfd_div_ratio : 4;
        unsigned int reserved : 20;
    } reg;
} SOC_NPUCRG_FNPLL_CFG5_UNION;
#endif
#define SOC_NPUCRG_FNPLL_CFG5_ref_lost_thr_fnpll_1v2_START (0)
#define SOC_NPUCRG_FNPLL_CFG5_ref_lost_thr_fnpll_1v2_END (7)
#define SOC_NPUCRG_FNPLL_CFG5_pfd_div_ratio_START (8)
#define SOC_NPUCRG_FNPLL_CFG5_pfd_div_ratio_END (11)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int vs_ctrl_en_npu : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_NPUCRG_VS_CTRL_EN_NPU_UNION;
#endif
#define SOC_NPUCRG_VS_CTRL_EN_NPU_vs_ctrl_en_npu_START (0)
#define SOC_NPUCRG_VS_CTRL_EN_NPU_vs_ctrl_en_npu_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int vs_ctrl_bypass_npu : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_NPUCRG_VS_CTRL_BYPASS_NPU_UNION;
#endif
#define SOC_NPUCRG_VS_CTRL_BYPASS_NPU_vs_ctrl_bypass_npu_START (0)
#define SOC_NPUCRG_VS_CTRL_BYPASS_NPU_vs_ctrl_bypass_npu_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int npu_cpu0_wfi_wfe_bypass : 1;
        unsigned int npu_cpu1_wfi_wfe_bypass : 1;
        unsigned int npu_cpu2_wfi_wfe_bypass : 1;
        unsigned int npu_cpu3_wfi_wfe_bypass : 1;
        unsigned int npu_l2_idle_div_mod : 2;
        unsigned int npu_cfg_cnt_cpu_l2_idle_quit : 16;
        unsigned int npu_cpu_wake_up_mode : 2;
        unsigned int npu_cpu_l2_idle_switch_bypass : 1;
        unsigned int npu_cpu_l2_idle_gt_en : 1;
        unsigned int npu_dvfs_div_en : 1;
        unsigned int reserved_1 : 1;
        unsigned int sel_ckmux_npu_icg : 1;
        unsigned int cpu_clk_div_cfg_npu : 2;
    } reg;
} SOC_NPUCRG_VS_CTRL_NPU_UNION;
#endif
#define SOC_NPUCRG_VS_CTRL_NPU_npu_cpu0_wfi_wfe_bypass_START (1)
#define SOC_NPUCRG_VS_CTRL_NPU_npu_cpu0_wfi_wfe_bypass_END (1)
#define SOC_NPUCRG_VS_CTRL_NPU_npu_cpu1_wfi_wfe_bypass_START (2)
#define SOC_NPUCRG_VS_CTRL_NPU_npu_cpu1_wfi_wfe_bypass_END (2)
#define SOC_NPUCRG_VS_CTRL_NPU_npu_cpu2_wfi_wfe_bypass_START (3)
#define SOC_NPUCRG_VS_CTRL_NPU_npu_cpu2_wfi_wfe_bypass_END (3)
#define SOC_NPUCRG_VS_CTRL_NPU_npu_cpu3_wfi_wfe_bypass_START (4)
#define SOC_NPUCRG_VS_CTRL_NPU_npu_cpu3_wfi_wfe_bypass_END (4)
#define SOC_NPUCRG_VS_CTRL_NPU_npu_l2_idle_div_mod_START (5)
#define SOC_NPUCRG_VS_CTRL_NPU_npu_l2_idle_div_mod_END (6)
#define SOC_NPUCRG_VS_CTRL_NPU_npu_cfg_cnt_cpu_l2_idle_quit_START (7)
#define SOC_NPUCRG_VS_CTRL_NPU_npu_cfg_cnt_cpu_l2_idle_quit_END (22)
#define SOC_NPUCRG_VS_CTRL_NPU_npu_cpu_wake_up_mode_START (23)
#define SOC_NPUCRG_VS_CTRL_NPU_npu_cpu_wake_up_mode_END (24)
#define SOC_NPUCRG_VS_CTRL_NPU_npu_cpu_l2_idle_switch_bypass_START (25)
#define SOC_NPUCRG_VS_CTRL_NPU_npu_cpu_l2_idle_switch_bypass_END (25)
#define SOC_NPUCRG_VS_CTRL_NPU_npu_cpu_l2_idle_gt_en_START (26)
#define SOC_NPUCRG_VS_CTRL_NPU_npu_cpu_l2_idle_gt_en_END (26)
#define SOC_NPUCRG_VS_CTRL_NPU_npu_dvfs_div_en_START (27)
#define SOC_NPUCRG_VS_CTRL_NPU_npu_dvfs_div_en_END (27)
#define SOC_NPUCRG_VS_CTRL_NPU_sel_ckmux_npu_icg_START (29)
#define SOC_NPUCRG_VS_CTRL_NPU_sel_ckmux_npu_icg_END (29)
#define SOC_NPUCRG_VS_CTRL_NPU_cpu_clk_div_cfg_npu_START (30)
#define SOC_NPUCRG_VS_CTRL_NPU_cpu_clk_div_cfg_npu_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int svfd_data_limit_e_npu : 1;
        unsigned int svfd_off_time_step_npu : 1;
        unsigned int svfd_pulse_width_sel_npu : 1;
        unsigned int reserved_0 : 1;
        unsigned int svfd_ulvt_ll_npu : 4;
        unsigned int svfd_ulvt_sl_npu : 4;
        unsigned int svfd_lvt_ll_npu : 4;
        unsigned int svfd_lvt_sl_npu : 4;
        unsigned int svfd_trim_npu : 2;
        unsigned int svfd_cmp_data_clr_npu : 1;
        unsigned int reserved_1 : 1;
        unsigned int svfd_d_rate_npu : 2;
        unsigned int svfd_off_mode_npu : 1;
        unsigned int svfd_div64_en_npu : 1;
        unsigned int svfd_cpm_period_npu : 1;
        unsigned int svfd_edge_sel_npu : 1;
        unsigned int svfd_cmp_data_mode_npu : 2;
    } reg;
} SOC_NPUCRG_VS_SVFD_CTRL0_NPU_UNION;
#endif
#define SOC_NPUCRG_VS_SVFD_CTRL0_NPU_svfd_data_limit_e_npu_START (0)
#define SOC_NPUCRG_VS_SVFD_CTRL0_NPU_svfd_data_limit_e_npu_END (0)
#define SOC_NPUCRG_VS_SVFD_CTRL0_NPU_svfd_off_time_step_npu_START (1)
#define SOC_NPUCRG_VS_SVFD_CTRL0_NPU_svfd_off_time_step_npu_END (1)
#define SOC_NPUCRG_VS_SVFD_CTRL0_NPU_svfd_pulse_width_sel_npu_START (2)
#define SOC_NPUCRG_VS_SVFD_CTRL0_NPU_svfd_pulse_width_sel_npu_END (2)
#define SOC_NPUCRG_VS_SVFD_CTRL0_NPU_svfd_ulvt_ll_npu_START (4)
#define SOC_NPUCRG_VS_SVFD_CTRL0_NPU_svfd_ulvt_ll_npu_END (7)
#define SOC_NPUCRG_VS_SVFD_CTRL0_NPU_svfd_ulvt_sl_npu_START (8)
#define SOC_NPUCRG_VS_SVFD_CTRL0_NPU_svfd_ulvt_sl_npu_END (11)
#define SOC_NPUCRG_VS_SVFD_CTRL0_NPU_svfd_lvt_ll_npu_START (12)
#define SOC_NPUCRG_VS_SVFD_CTRL0_NPU_svfd_lvt_ll_npu_END (15)
#define SOC_NPUCRG_VS_SVFD_CTRL0_NPU_svfd_lvt_sl_npu_START (16)
#define SOC_NPUCRG_VS_SVFD_CTRL0_NPU_svfd_lvt_sl_npu_END (19)
#define SOC_NPUCRG_VS_SVFD_CTRL0_NPU_svfd_trim_npu_START (20)
#define SOC_NPUCRG_VS_SVFD_CTRL0_NPU_svfd_trim_npu_END (21)
#define SOC_NPUCRG_VS_SVFD_CTRL0_NPU_svfd_cmp_data_clr_npu_START (22)
#define SOC_NPUCRG_VS_SVFD_CTRL0_NPU_svfd_cmp_data_clr_npu_END (22)
#define SOC_NPUCRG_VS_SVFD_CTRL0_NPU_svfd_d_rate_npu_START (24)
#define SOC_NPUCRG_VS_SVFD_CTRL0_NPU_svfd_d_rate_npu_END (25)
#define SOC_NPUCRG_VS_SVFD_CTRL0_NPU_svfd_off_mode_npu_START (26)
#define SOC_NPUCRG_VS_SVFD_CTRL0_NPU_svfd_off_mode_npu_END (26)
#define SOC_NPUCRG_VS_SVFD_CTRL0_NPU_svfd_div64_en_npu_START (27)
#define SOC_NPUCRG_VS_SVFD_CTRL0_NPU_svfd_div64_en_npu_END (27)
#define SOC_NPUCRG_VS_SVFD_CTRL0_NPU_svfd_cpm_period_npu_START (28)
#define SOC_NPUCRG_VS_SVFD_CTRL0_NPU_svfd_cpm_period_npu_END (28)
#define SOC_NPUCRG_VS_SVFD_CTRL0_NPU_svfd_edge_sel_npu_START (29)
#define SOC_NPUCRG_VS_SVFD_CTRL0_NPU_svfd_edge_sel_npu_END (29)
#define SOC_NPUCRG_VS_SVFD_CTRL0_NPU_svfd_cmp_data_mode_npu_START (30)
#define SOC_NPUCRG_VS_SVFD_CTRL0_NPU_svfd_cmp_data_mode_npu_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int svfd_glitch_test_npu : 1;
        unsigned int reserved_0 : 3;
        unsigned int svfd_test_ffs_npu : 8;
        unsigned int svfd_test_cpm_npu : 8;
        unsigned int reserved_1 : 12;
    } reg;
} SOC_NPUCRG_VS_SVFD_CTRL1_NPU_UNION;
#endif
#define SOC_NPUCRG_VS_SVFD_CTRL1_NPU_svfd_glitch_test_npu_START (0)
#define SOC_NPUCRG_VS_SVFD_CTRL1_NPU_svfd_glitch_test_npu_END (0)
#define SOC_NPUCRG_VS_SVFD_CTRL1_NPU_svfd_test_ffs_npu_START (4)
#define SOC_NPUCRG_VS_SVFD_CTRL1_NPU_svfd_test_ffs_npu_END (11)
#define SOC_NPUCRG_VS_SVFD_CTRL1_NPU_svfd_test_cpm_npu_START (12)
#define SOC_NPUCRG_VS_SVFD_CTRL1_NPU_svfd_test_cpm_npu_END (19)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int svfd_enalbe_mode_npu : 2;
        unsigned int svfs_cpm_threshold_r_npu : 6;
        unsigned int svfs_cpm_threshold_f_npu : 6;
        unsigned int vol_drop_en_npu : 1;
        unsigned int reserved : 1;
        unsigned int vs_svfd_ctrl2_npu_msk : 16;
    } reg;
} SOC_NPUCRG_VS_SVFD_CTRL2_NPU_UNION;
#endif
#define SOC_NPUCRG_VS_SVFD_CTRL2_NPU_svfd_enalbe_mode_npu_START (0)
#define SOC_NPUCRG_VS_SVFD_CTRL2_NPU_svfd_enalbe_mode_npu_END (1)
#define SOC_NPUCRG_VS_SVFD_CTRL2_NPU_svfs_cpm_threshold_r_npu_START (2)
#define SOC_NPUCRG_VS_SVFD_CTRL2_NPU_svfs_cpm_threshold_r_npu_END (7)
#define SOC_NPUCRG_VS_SVFD_CTRL2_NPU_svfs_cpm_threshold_f_npu_START (8)
#define SOC_NPUCRG_VS_SVFD_CTRL2_NPU_svfs_cpm_threshold_f_npu_END (13)
#define SOC_NPUCRG_VS_SVFD_CTRL2_NPU_vol_drop_en_npu_START (14)
#define SOC_NPUCRG_VS_SVFD_CTRL2_NPU_vol_drop_en_npu_END (14)
#define SOC_NPUCRG_VS_SVFD_CTRL2_NPU_vs_svfd_ctrl2_npu_msk_START (16)
#define SOC_NPUCRG_VS_SVFD_CTRL2_NPU_vs_svfd_ctrl2_npu_msk_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int svfd_cpm_data_r_npu : 8;
        unsigned int svfd_cpm_data_f_npu : 8;
        unsigned int svfd_dll_lock_npu : 1;
        unsigned int svfd_cpm_data_norst_npu : 6;
        unsigned int reserved : 9;
    } reg;
} SOC_NPUCRG_VS_CPM_DATA_STAT_NPU_UNION;
#endif
#define SOC_NPUCRG_VS_CPM_DATA_STAT_NPU_svfd_cpm_data_r_npu_START (0)
#define SOC_NPUCRG_VS_CPM_DATA_STAT_NPU_svfd_cpm_data_r_npu_END (7)
#define SOC_NPUCRG_VS_CPM_DATA_STAT_NPU_svfd_cpm_data_f_npu_START (8)
#define SOC_NPUCRG_VS_CPM_DATA_STAT_NPU_svfd_cpm_data_f_npu_END (15)
#define SOC_NPUCRG_VS_CPM_DATA_STAT_NPU_svfd_dll_lock_npu_START (16)
#define SOC_NPUCRG_VS_CPM_DATA_STAT_NPU_svfd_dll_lock_npu_END (16)
#define SOC_NPUCRG_VS_CPM_DATA_STAT_NPU_svfd_cpm_data_norst_npu_START (17)
#define SOC_NPUCRG_VS_CPM_DATA_STAT_NPU_svfd_cpm_data_norst_npu_END (22)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int svfd_test_out_ffs_npu : 4;
        unsigned int svfd_test_out_cpm_npu : 4;
        unsigned int svfd_glitch_result_npu : 1;
        unsigned int idle_low_freq_en_npu : 1;
        unsigned int vbat_drop_protect_ind_npu : 1;
        unsigned int reserved : 21;
    } reg;
} SOC_NPUCRG_VS_TEST_STAT_NPU_UNION;
#endif
#define SOC_NPUCRG_VS_TEST_STAT_NPU_svfd_test_out_ffs_npu_START (0)
#define SOC_NPUCRG_VS_TEST_STAT_NPU_svfd_test_out_ffs_npu_END (3)
#define SOC_NPUCRG_VS_TEST_STAT_NPU_svfd_test_out_cpm_npu_START (4)
#define SOC_NPUCRG_VS_TEST_STAT_NPU_svfd_test_out_cpm_npu_END (7)
#define SOC_NPUCRG_VS_TEST_STAT_NPU_svfd_glitch_result_npu_START (8)
#define SOC_NPUCRG_VS_TEST_STAT_NPU_svfd_glitch_result_npu_END (8)
#define SOC_NPUCRG_VS_TEST_STAT_NPU_idle_low_freq_en_npu_START (9)
#define SOC_NPUCRG_VS_TEST_STAT_NPU_idle_low_freq_en_npu_END (9)
#define SOC_NPUCRG_VS_TEST_STAT_NPU_vbat_drop_protect_ind_npu_START (10)
#define SOC_NPUCRG_VS_TEST_STAT_NPU_vbat_drop_protect_ind_npu_END (10)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pstate1_aicore : 32;
    } reg;
} SOC_NPUCRG_PSTATE1_AICORE_UNION;
#endif
#define SOC_NPUCRG_PSTATE1_AICORE_pstate1_aicore_START (0)
#define SOC_NPUCRG_PSTATE1_AICORE_pstate1_aicore_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pstate2_aicore : 32;
    } reg;
} SOC_NPUCRG_PSTATE2_AICORE_UNION;
#endif
#define SOC_NPUCRG_PSTATE2_AICORE_pstate2_aicore_START (0)
#define SOC_NPUCRG_PSTATE2_AICORE_pstate2_aicore_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pstate3_aicore : 32;
    } reg;
} SOC_NPUCRG_PSTATE3_AICORE_UNION;
#endif
#define SOC_NPUCRG_PSTATE3_AICORE_pstate3_aicore_START (0)
#define SOC_NPUCRG_PSTATE3_AICORE_pstate3_aicore_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pstate4_aicore : 32;
    } reg;
} SOC_NPUCRG_PSTATE4_AICORE_UNION;
#endif
#define SOC_NPUCRG_PSTATE4_AICORE_pstate4_aicore_START (0)
#define SOC_NPUCRG_PSTATE4_AICORE_pstate4_aicore_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int intr_mask_aicore0_nonidle_pend : 1;
        unsigned int intr_mask_aicore1_nonidle_pend : 1;
        unsigned int intr_mask_sysdma_nonidle_pend : 1;
        unsigned int intr_mask_tcu_nonidle_pend : 1;
        unsigned int intr_mask_l2buf_nonidle_pend : 1;
        unsigned int intr_mask_ts_nonidle_pend : 1;
        unsigned int intr_mask_tiny_nonidle_pend : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 8;
        unsigned int bitmasken : 16;
    } reg;
} SOC_NPUCRG_INTR_MASK_NOCBUS_NONIDLE_PEND_UNION;
#endif
#define SOC_NPUCRG_INTR_MASK_NOCBUS_NONIDLE_PEND_intr_mask_aicore0_nonidle_pend_START (0)
#define SOC_NPUCRG_INTR_MASK_NOCBUS_NONIDLE_PEND_intr_mask_aicore0_nonidle_pend_END (0)
#define SOC_NPUCRG_INTR_MASK_NOCBUS_NONIDLE_PEND_intr_mask_aicore1_nonidle_pend_START (1)
#define SOC_NPUCRG_INTR_MASK_NOCBUS_NONIDLE_PEND_intr_mask_aicore1_nonidle_pend_END (1)
#define SOC_NPUCRG_INTR_MASK_NOCBUS_NONIDLE_PEND_intr_mask_sysdma_nonidle_pend_START (2)
#define SOC_NPUCRG_INTR_MASK_NOCBUS_NONIDLE_PEND_intr_mask_sysdma_nonidle_pend_END (2)
#define SOC_NPUCRG_INTR_MASK_NOCBUS_NONIDLE_PEND_intr_mask_tcu_nonidle_pend_START (3)
#define SOC_NPUCRG_INTR_MASK_NOCBUS_NONIDLE_PEND_intr_mask_tcu_nonidle_pend_END (3)
#define SOC_NPUCRG_INTR_MASK_NOCBUS_NONIDLE_PEND_intr_mask_l2buf_nonidle_pend_START (4)
#define SOC_NPUCRG_INTR_MASK_NOCBUS_NONIDLE_PEND_intr_mask_l2buf_nonidle_pend_END (4)
#define SOC_NPUCRG_INTR_MASK_NOCBUS_NONIDLE_PEND_intr_mask_ts_nonidle_pend_START (5)
#define SOC_NPUCRG_INTR_MASK_NOCBUS_NONIDLE_PEND_intr_mask_ts_nonidle_pend_END (5)
#define SOC_NPUCRG_INTR_MASK_NOCBUS_NONIDLE_PEND_intr_mask_tiny_nonidle_pend_START (6)
#define SOC_NPUCRG_INTR_MASK_NOCBUS_NONIDLE_PEND_intr_mask_tiny_nonidle_pend_END (6)
#define SOC_NPUCRG_INTR_MASK_NOCBUS_NONIDLE_PEND_bitmasken_START (16)
#define SOC_NPUCRG_INTR_MASK_NOCBUS_NONIDLE_PEND_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int intr_clr_aicore0_nonidle_pend : 1;
        unsigned int intr_clr_aicore1_nonidle_pend : 1;
        unsigned int intr_clr_sysdma_nonidle_pend : 1;
        unsigned int intr_clr_tcu_nonidle_pend : 1;
        unsigned int intr_clr_l2buf_nonidle_pend : 1;
        unsigned int intr_clr_ts_nonidle_pend : 1;
        unsigned int intr_clr_tiny_nonidle_pend : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 8;
        unsigned int bitmasken : 16;
    } reg;
} SOC_NPUCRG_INTR_CLR_NOCBUS_NONIDLE_PEND_UNION;
#endif
#define SOC_NPUCRG_INTR_CLR_NOCBUS_NONIDLE_PEND_intr_clr_aicore0_nonidle_pend_START (0)
#define SOC_NPUCRG_INTR_CLR_NOCBUS_NONIDLE_PEND_intr_clr_aicore0_nonidle_pend_END (0)
#define SOC_NPUCRG_INTR_CLR_NOCBUS_NONIDLE_PEND_intr_clr_aicore1_nonidle_pend_START (1)
#define SOC_NPUCRG_INTR_CLR_NOCBUS_NONIDLE_PEND_intr_clr_aicore1_nonidle_pend_END (1)
#define SOC_NPUCRG_INTR_CLR_NOCBUS_NONIDLE_PEND_intr_clr_sysdma_nonidle_pend_START (2)
#define SOC_NPUCRG_INTR_CLR_NOCBUS_NONIDLE_PEND_intr_clr_sysdma_nonidle_pend_END (2)
#define SOC_NPUCRG_INTR_CLR_NOCBUS_NONIDLE_PEND_intr_clr_tcu_nonidle_pend_START (3)
#define SOC_NPUCRG_INTR_CLR_NOCBUS_NONIDLE_PEND_intr_clr_tcu_nonidle_pend_END (3)
#define SOC_NPUCRG_INTR_CLR_NOCBUS_NONIDLE_PEND_intr_clr_l2buf_nonidle_pend_START (4)
#define SOC_NPUCRG_INTR_CLR_NOCBUS_NONIDLE_PEND_intr_clr_l2buf_nonidle_pend_END (4)
#define SOC_NPUCRG_INTR_CLR_NOCBUS_NONIDLE_PEND_intr_clr_ts_nonidle_pend_START (5)
#define SOC_NPUCRG_INTR_CLR_NOCBUS_NONIDLE_PEND_intr_clr_ts_nonidle_pend_END (5)
#define SOC_NPUCRG_INTR_CLR_NOCBUS_NONIDLE_PEND_intr_clr_tiny_nonidle_pend_START (6)
#define SOC_NPUCRG_INTR_CLR_NOCBUS_NONIDLE_PEND_intr_clr_tiny_nonidle_pend_END (6)
#define SOC_NPUCRG_INTR_CLR_NOCBUS_NONIDLE_PEND_bitmasken_START (16)
#define SOC_NPUCRG_INTR_CLR_NOCBUS_NONIDLE_PEND_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int intr_stat_aicore0_nonidle_pend : 1;
        unsigned int intr_stat_aicore1_nonidle_pend : 1;
        unsigned int intr_stat_sysdma_nonidle_pend : 1;
        unsigned int intr_stat_tcu_nonidle_pend : 1;
        unsigned int intr_stat_l2buf_nonidle_pend : 1;
        unsigned int intr_stat_ts_nonidle_pend : 1;
        unsigned int intr_stat_tiny_nonidle_pend : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 24;
    } reg;
} SOC_NPUCRG_INTR_STAT_NOCBUS_NONIDLE_PEND_UNION;
#endif
#define SOC_NPUCRG_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_aicore0_nonidle_pend_START (0)
#define SOC_NPUCRG_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_aicore0_nonidle_pend_END (0)
#define SOC_NPUCRG_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_aicore1_nonidle_pend_START (1)
#define SOC_NPUCRG_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_aicore1_nonidle_pend_END (1)
#define SOC_NPUCRG_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_sysdma_nonidle_pend_START (2)
#define SOC_NPUCRG_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_sysdma_nonidle_pend_END (2)
#define SOC_NPUCRG_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_tcu_nonidle_pend_START (3)
#define SOC_NPUCRG_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_tcu_nonidle_pend_END (3)
#define SOC_NPUCRG_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_l2buf_nonidle_pend_START (4)
#define SOC_NPUCRG_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_l2buf_nonidle_pend_END (4)
#define SOC_NPUCRG_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_ts_nonidle_pend_START (5)
#define SOC_NPUCRG_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_ts_nonidle_pend_END (5)
#define SOC_NPUCRG_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_tiny_nonidle_pend_START (6)
#define SOC_NPUCRG_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_tiny_nonidle_pend_END (6)
#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
#endif
