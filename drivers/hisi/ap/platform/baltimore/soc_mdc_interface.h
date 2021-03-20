#ifndef __SOC_MDC_INTERFACE_H__
#define __SOC_MDC_INTERFACE_H__ 
#ifdef __cplusplus
    #if __cplusplus
        extern "C" {
    #endif
#endif
#ifndef __SOC_H_FOR_ASM__
#define SOC_MDC_DSS_TAG_ADDR(base) ((base) + (0x0000UL))
#define SOC_MDC_APB_CTL_ADDR(base) ((base) + (0x0004UL))
#define SOC_MDC_MMBUF_CFG_SECU_EN_ADDR(base) ((base) + (0x0008UL))
#define SOC_MDC_ASC_CFG_SECU_EN_ADDR(base) ((base) + (0x000CUL))
#define SOC_MDC_DSS_AXI_RST_EN_ADDR(base) ((base) + (0x118UL))
#define SOC_MDC_CPU_OFF_INTS_ADDR(base) ((base) + (0x234UL))
#define SOC_MDC_CPU_OFF_INT_MSK_ADDR(base) ((base) + (0x238UL))
#define SOC_MDC_CPU_OFF_CAM_INTS_ADDR(base) ((base) + (0x2ACUL))
#define SOC_MDC_CPU_OFF_CAM_INT_MSK_ADDR(base) ((base) + (0x2B0UL))
#define SOC_MDC_GLB_MODULE_CLK_SEL_ADDR(base) ((base) + (0x0300UL))
#define SOC_MDC_GLB_MODULE_CLK_EN_ADDR(base) ((base) + (0x304UL))
#define SOC_MDC_GLB0_DBG_SEL_ADDR(base) ((base) + (0x310UL))
#define SOC_MDC_GLB1_DBG_SEL_ADDR(base) ((base) + (0x314UL))
#define SOC_MDC_DBG_IRQ_CPU_ADDR(base) ((base) + (0x320UL))
#define SOC_MDC_DBG_IRQ_MCU_ADDR(base) ((base) + (0x324UL))
#define SOC_MDC_DSS_SPRAM_CTRL_ADDR(base) ((base) + (0x0600UL))
#define SOC_MDC_DSS_PM_CTRL_ADDR(base) ((base) + (0x0604UL))
#define SOC_MDC_GLB_DSLP_EN_ADDR(base) ((base) + (0x0608UL))
#define SOC_MDC_DSS_TPRAM_CTRL_ADDR(base) ((base) + (0x060CUL))
#else
#define SOC_MDC_DSS_TAG_ADDR(base) ((base) + (0x0000))
#define SOC_MDC_APB_CTL_ADDR(base) ((base) + (0x0004))
#define SOC_MDC_MMBUF_CFG_SECU_EN_ADDR(base) ((base) + (0x0008))
#define SOC_MDC_ASC_CFG_SECU_EN_ADDR(base) ((base) + (0x000C))
#define SOC_MDC_DSS_AXI_RST_EN_ADDR(base) ((base) + (0x118))
#define SOC_MDC_CPU_OFF_INTS_ADDR(base) ((base) + (0x234))
#define SOC_MDC_CPU_OFF_INT_MSK_ADDR(base) ((base) + (0x238))
#define SOC_MDC_CPU_OFF_CAM_INTS_ADDR(base) ((base) + (0x2AC))
#define SOC_MDC_CPU_OFF_CAM_INT_MSK_ADDR(base) ((base) + (0x2B0))
#define SOC_MDC_GLB_MODULE_CLK_SEL_ADDR(base) ((base) + (0x0300))
#define SOC_MDC_GLB_MODULE_CLK_EN_ADDR(base) ((base) + (0x304))
#define SOC_MDC_GLB0_DBG_SEL_ADDR(base) ((base) + (0x310))
#define SOC_MDC_GLB1_DBG_SEL_ADDR(base) ((base) + (0x314))
#define SOC_MDC_DBG_IRQ_CPU_ADDR(base) ((base) + (0x320))
#define SOC_MDC_DBG_IRQ_MCU_ADDR(base) ((base) + (0x324))
#define SOC_MDC_DSS_SPRAM_CTRL_ADDR(base) ((base) + (0x0600))
#define SOC_MDC_DSS_PM_CTRL_ADDR(base) ((base) + (0x0604))
#define SOC_MDC_GLB_DSLP_EN_ADDR(base) ((base) + (0x0608))
#define SOC_MDC_DSS_TPRAM_CTRL_ADDR(base) ((base) + (0x060C))
#endif
#ifndef __SOC_H_FOR_ASM__
#define SOC_MDC_AIF_CH_CTL_0_ADDR(base) ((base) + (0x0000UL))
#define SOC_MDC_AIF_CH_HS_0_ADDR(base) ((base) + (0x0004UL))
#define SOC_MDC_AIF_CH_LS_0_ADDR(base) ((base) + (0x0008UL))
#define SOC_MDC_AIF_CH_CTL_1_ADDR(base) ((base) + (0x0020UL))
#define SOC_MDC_AIF_CH_HS_1_ADDR(base) ((base) + (0x0024UL))
#define SOC_MDC_AIF_CH_LS_1_ADDR(base) ((base) + (0x0028UL))
#define SOC_MDC_AIF_CH_CTL_2_ADDR(base) ((base) + (0x0040UL))
#define SOC_MDC_AIF_CH_HS_2_ADDR(base) ((base) + (0x0044UL))
#define SOC_MDC_AIF_CH_LS_2_ADDR(base) ((base) + (0x0048UL))
#define SOC_MDC_AIF_CH_CTL_6_ADDR(base) ((base) + (0x00C0UL))
#define SOC_MDC_AIF_CH_HS_6_ADDR(base) ((base) + (0x00C4UL))
#define SOC_MDC_AIF_CH_LS_6_ADDR(base) ((base) + (0x00C8UL))
#define SOC_MDC_AIF_CH_CTL_8_ADDR(base) ((base) + (0x0100UL))
#define SOC_MDC_AIF_CH_HS_8_ADDR(base) ((base) + (0x0104UL))
#define SOC_MDC_AIF_CH_LS_8_ADDR(base) ((base) + (0x0108UL))
#define SOC_MDC_AIF_CH_CTL_9_ADDR(base) ((base) + (0x0120UL))
#define SOC_MDC_AIF_CH_HS_9_ADDR(base) ((base) + (0x0124UL))
#define SOC_MDC_AIF_CH_LS_9_ADDR(base) ((base) + (0x0128UL))
#define SOC_MDC_AIF_CH_CTL_10_ADDR(base) ((base) + (0x0140UL))
#define SOC_MDC_AIF_CH_HS_10_ADDR(base) ((base) + (0x0144UL))
#define SOC_MDC_AIF_CH_LS_10_ADDR(base) ((base) + (0x0148UL))
#define SOC_MDC_ARCACH2_ADDR_THRH_ADDR(base) ((base) + (0x0240UL))
#define SOC_MDC_ARCACH2_ADDR_THRL_ADDR(base) ((base) + (0x0244UL))
#define SOC_MDC_ARCACH2_VALUE0_ADDR(base) ((base) + (0x0248UL))
#define SOC_MDC_ARCACH2_VALUE1_ADDR(base) ((base) + (0x024CUL))
#define SOC_MDC_AWCACH2_ADDR_THRH_ADDR(base) ((base) + (0x0250UL))
#define SOC_MDC_AWCACH2_ADDR_THRL_ADDR(base) ((base) + (0x0254UL))
#define SOC_MDC_AWCACH2_VALUE0_ADDR(base) ((base) + (0x0258UL))
#define SOC_MDC_AWCACH2_VALUE1_ADDR(base) ((base) + (0x025CUL))
#define SOC_MDC_AIF_CH_MID_0_ADDR(base) ((base) + (0x0400UL))
#define SOC_MDC_AIF_CH_MID_1_ADDR(base) ((base) + (0x0404UL))
#define SOC_MDC_AIF_CH_MID_2_ADDR(base) ((base) + (0x0408UL))
#define SOC_MDC_AIF_CH_MID_6_ADDR(base) ((base) + (0x0418UL))
#define SOC_MDC_AIF_CH_MID_8_ADDR(base) ((base) + (0x0420UL))
#define SOC_MDC_AIF_CH_MID_9_ADDR(base) ((base) + (0x0424UL))
#define SOC_MDC_AIF_CH_MID_10_ADDR(base) ((base) + (0x0428UL))
#define SOC_MDC_AIF_CH_MID_CMD_ADDR(base) ((base) + (0x0434UL))
#define SOC_MDC_DSS0_MID_ADDR(base) ((base) + (0x0438UL))
#define SOC_MDC_DSS1_MID_ADDR(base) ((base) + (0x043CUL))
#define SOC_MDC_DSS2_MID_ADDR(base) ((base) + (0x0440UL))
#define SOC_MDC_DSS3_MID_ADDR(base) ((base) + (0x0444UL))
#define SOC_MDC_MMU_ID_ATTR_NS_0_ADDR(base) ((base) + (0x0580UL))
#define SOC_MDC_MMU_ID_ATTR_NS_1_ADDR(base) ((base) + (0x0584UL))
#define SOC_MDC_MMU_ID_ATTR_NS_2_ADDR(base) ((base) + (0x0588UL))
#define SOC_MDC_MMU_ID_ATTR_NS_6_ADDR(base) ((base) + (0x0598UL))
#define SOC_MDC_MMU_ID_ATTR_NS_8_ADDR(base) ((base) + (0x05A0UL))
#define SOC_MDC_MMU_ID_ATTR_NS_9_ADDR(base) ((base) + (0x05A4UL))
#define SOC_MDC_MMU_ID_ATTR_NS_10_ADDR(base) ((base) + (0x05A8UL))
#define SOC_MDC_MMU_ID_ATTR_NS_13_ADDR(base) ((base) + (0x05B4UL))
#define SOC_MDC_AXI2_RID_MSK0_ADDR(base) ((base) + (0x0720UL))
#define SOC_MDC_AXI2_RID_MSK1_ADDR(base) ((base) + (0x0724UL))
#define SOC_MDC_AXI2_WID_MSK_ADDR(base) ((base) + (0x0728UL))
#define SOC_MDC_AXI2_R_QOS_MAP_ADDR(base) ((base) + (0x072CUL))
#define SOC_MDC_AIF_CLK_SEL0_ADDR(base) ((base) + (0x0820UL))
#define SOC_MDC_AIF_CLK_SEL1_ADDR(base) ((base) + (0x0824UL))
#define SOC_MDC_AIF_CLK_EN0_ADDR(base) ((base) + (0x0828UL))
#define SOC_MDC_AIF_CLK_EN1_ADDR(base) ((base) + (0x082CUL))
#define SOC_MDC_AIF_CH_CTL_CMD_ADDR(base) ((base) + (0x0830UL))
#define SOC_MDC_MONITOR_TIMER_INI_ADDR(base) ((base) + (0x0834UL))
#define SOC_MDC_DEBUG_BUF_BASE_ADDR(base) ((base) + (0x0838UL))
#define SOC_MDC_DEBUG_CTRL_ADDR(base) ((base) + (0x083CUL))
#define SOC_MDC_AIF_SHADOW_READ_ADDR(base) ((base) + (0x0840UL))
#define SOC_MDC_AIF_MEM_CTRL_ADDR(base) ((base) + (0x0844UL))
#define SOC_MDC_MONITOR_EN_ADDR(base) ((base) + (0x0848UL))
#define SOC_MDC_MONITOR_CTRL_ADDR(base) ((base) + (0x084CUL))
#define SOC_MDC_MONITOR_SAMPLE_MUN_ADDR(base) ((base) + (0x0850UL))
#define SOC_MDC_MONITOR_SAMPLE_TIME_ADDR(base) ((base) + (0x0854UL))
#define SOC_MDC_MONITOR_SAMPLE_FLOW_ADDR(base) ((base) + (0x0858UL))
#define SOC_MDC_AIX_MODULE_CLK_SEL_ADDR(base) ((base) + (0xA04UL))
#define SOC_MDC_AIX_MODULE_CLK_EN_ADDR(base) ((base) + (0xA08UL))
#else
#define SOC_MDC_AIF_CH_CTL_0_ADDR(base) ((base) + (0x0000))
#define SOC_MDC_AIF_CH_HS_0_ADDR(base) ((base) + (0x0004))
#define SOC_MDC_AIF_CH_LS_0_ADDR(base) ((base) + (0x0008))
#define SOC_MDC_AIF_CH_CTL_1_ADDR(base) ((base) + (0x0020))
#define SOC_MDC_AIF_CH_HS_1_ADDR(base) ((base) + (0x0024))
#define SOC_MDC_AIF_CH_LS_1_ADDR(base) ((base) + (0x0028))
#define SOC_MDC_AIF_CH_CTL_2_ADDR(base) ((base) + (0x0040))
#define SOC_MDC_AIF_CH_HS_2_ADDR(base) ((base) + (0x0044))
#define SOC_MDC_AIF_CH_LS_2_ADDR(base) ((base) + (0x0048))
#define SOC_MDC_AIF_CH_CTL_6_ADDR(base) ((base) + (0x00C0))
#define SOC_MDC_AIF_CH_HS_6_ADDR(base) ((base) + (0x00C4))
#define SOC_MDC_AIF_CH_LS_6_ADDR(base) ((base) + (0x00C8))
#define SOC_MDC_AIF_CH_CTL_8_ADDR(base) ((base) + (0x0100))
#define SOC_MDC_AIF_CH_HS_8_ADDR(base) ((base) + (0x0104))
#define SOC_MDC_AIF_CH_LS_8_ADDR(base) ((base) + (0x0108))
#define SOC_MDC_AIF_CH_CTL_9_ADDR(base) ((base) + (0x0120))
#define SOC_MDC_AIF_CH_HS_9_ADDR(base) ((base) + (0x0124))
#define SOC_MDC_AIF_CH_LS_9_ADDR(base) ((base) + (0x0128))
#define SOC_MDC_AIF_CH_CTL_10_ADDR(base) ((base) + (0x0140))
#define SOC_MDC_AIF_CH_HS_10_ADDR(base) ((base) + (0x0144))
#define SOC_MDC_AIF_CH_LS_10_ADDR(base) ((base) + (0x0148))
#define SOC_MDC_ARCACH2_ADDR_THRH_ADDR(base) ((base) + (0x0240))
#define SOC_MDC_ARCACH2_ADDR_THRL_ADDR(base) ((base) + (0x0244))
#define SOC_MDC_ARCACH2_VALUE0_ADDR(base) ((base) + (0x0248))
#define SOC_MDC_ARCACH2_VALUE1_ADDR(base) ((base) + (0x024C))
#define SOC_MDC_AWCACH2_ADDR_THRH_ADDR(base) ((base) + (0x0250))
#define SOC_MDC_AWCACH2_ADDR_THRL_ADDR(base) ((base) + (0x0254))
#define SOC_MDC_AWCACH2_VALUE0_ADDR(base) ((base) + (0x0258))
#define SOC_MDC_AWCACH2_VALUE1_ADDR(base) ((base) + (0x025C))
#define SOC_MDC_AIF_CH_MID_0_ADDR(base) ((base) + (0x0400))
#define SOC_MDC_AIF_CH_MID_1_ADDR(base) ((base) + (0x0404))
#define SOC_MDC_AIF_CH_MID_2_ADDR(base) ((base) + (0x0408))
#define SOC_MDC_AIF_CH_MID_6_ADDR(base) ((base) + (0x0418))
#define SOC_MDC_AIF_CH_MID_8_ADDR(base) ((base) + (0x0420))
#define SOC_MDC_AIF_CH_MID_9_ADDR(base) ((base) + (0x0424))
#define SOC_MDC_AIF_CH_MID_10_ADDR(base) ((base) + (0x0428))
#define SOC_MDC_AIF_CH_MID_CMD_ADDR(base) ((base) + (0x0434))
#define SOC_MDC_DSS0_MID_ADDR(base) ((base) + (0x0438))
#define SOC_MDC_DSS1_MID_ADDR(base) ((base) + (0x043C))
#define SOC_MDC_DSS2_MID_ADDR(base) ((base) + (0x0440))
#define SOC_MDC_DSS3_MID_ADDR(base) ((base) + (0x0444))
#define SOC_MDC_MMU_ID_ATTR_NS_0_ADDR(base) ((base) + (0x0580))
#define SOC_MDC_MMU_ID_ATTR_NS_1_ADDR(base) ((base) + (0x0584))
#define SOC_MDC_MMU_ID_ATTR_NS_2_ADDR(base) ((base) + (0x0588))
#define SOC_MDC_MMU_ID_ATTR_NS_6_ADDR(base) ((base) + (0x0598))
#define SOC_MDC_MMU_ID_ATTR_NS_8_ADDR(base) ((base) + (0x05A0))
#define SOC_MDC_MMU_ID_ATTR_NS_9_ADDR(base) ((base) + (0x05A4))
#define SOC_MDC_MMU_ID_ATTR_NS_10_ADDR(base) ((base) + (0x05A8))
#define SOC_MDC_MMU_ID_ATTR_NS_13_ADDR(base) ((base) + (0x05B4))
#define SOC_MDC_AXI2_RID_MSK0_ADDR(base) ((base) + (0x0720))
#define SOC_MDC_AXI2_RID_MSK1_ADDR(base) ((base) + (0x0724))
#define SOC_MDC_AXI2_WID_MSK_ADDR(base) ((base) + (0x0728))
#define SOC_MDC_AXI2_R_QOS_MAP_ADDR(base) ((base) + (0x072C))
#define SOC_MDC_AIF_CLK_SEL0_ADDR(base) ((base) + (0x0820))
#define SOC_MDC_AIF_CLK_SEL1_ADDR(base) ((base) + (0x0824))
#define SOC_MDC_AIF_CLK_EN0_ADDR(base) ((base) + (0x0828))
#define SOC_MDC_AIF_CLK_EN1_ADDR(base) ((base) + (0x082C))
#define SOC_MDC_AIF_CH_CTL_CMD_ADDR(base) ((base) + (0x0830))
#define SOC_MDC_MONITOR_TIMER_INI_ADDR(base) ((base) + (0x0834))
#define SOC_MDC_DEBUG_BUF_BASE_ADDR(base) ((base) + (0x0838))
#define SOC_MDC_DEBUG_CTRL_ADDR(base) ((base) + (0x083C))
#define SOC_MDC_AIF_SHADOW_READ_ADDR(base) ((base) + (0x0840))
#define SOC_MDC_AIF_MEM_CTRL_ADDR(base) ((base) + (0x0844))
#define SOC_MDC_MONITOR_EN_ADDR(base) ((base) + (0x0848))
#define SOC_MDC_MONITOR_CTRL_ADDR(base) ((base) + (0x084C))
#define SOC_MDC_MONITOR_SAMPLE_MUN_ADDR(base) ((base) + (0x0850))
#define SOC_MDC_MONITOR_SAMPLE_TIME_ADDR(base) ((base) + (0x0854))
#define SOC_MDC_MONITOR_SAMPLE_FLOW_ADDR(base) ((base) + (0x0858))
#define SOC_MDC_AIX_MODULE_CLK_SEL_ADDR(base) ((base) + (0xA04))
#define SOC_MDC_AIX_MODULE_CLK_EN_ADDR(base) ((base) + (0xA08))
#endif
#ifndef __SOC_H_FOR_ASM__
#define SOC_MDC_MIF_CLK_CTL_ADDR(base) ((base) + (0x0000UL))
#define SOC_MDC_MIF_MEM_CTRL_ADDR(base) ((base) + (0x0004UL))
#define SOC_MDC_MIF_SHADOW_READ_ADDR(base) ((base) + (0x0008UL))
#define SOC_MDC_AIF_CMD_RELOAD_ADDR(base) ((base) + (0x000CUL))
#define SOC_MDC_MIF_CTRL_D0_ADDR(base) ((base) + (0x0010UL))
#define SOC_MDC_MIF_LEAD_CTRL_D0_ADDR(base) ((base) + (0x0014UL))
#define SOC_MDC_MIF_OS_CTRL_D0_ADDR(base) ((base) + (0x0018UL))
#define SOC_MDC_MIF_STAT_D0_ADDR(base) ((base) + (0x001CUL))
#define SOC_MDC_MIF_CTRL_D1_ADDR(base) ((base) + (0x0020UL))
#define SOC_MDC_MIF_LEAD_CTRL_D1_ADDR(base) ((base) + (0x0024UL))
#define SOC_MDC_MIF_OS_CTRL_D1_ADDR(base) ((base) + (0x0028UL))
#define SOC_MDC_MIF_STAT_D1_ADDR(base) ((base) + (0x002CUL))
#define SOC_MDC_MIF_CTRL_V0_ADDR(base) ((base) + (0x0030UL))
#define SOC_MDC_MIF_LEAD_CTRL_V0_ADDR(base) ((base) + (0x0034UL))
#define SOC_MDC_MIF_OS_CTRL_V0_ADDR(base) ((base) + (0x0038UL))
#define SOC_MDC_MIF_STAT_V0_ADDR(base) ((base) + (0x003CUL))
#define SOC_MDC_MIF_CTRL_D2_ADDR(base) ((base) + (0x0070UL))
#define SOC_MDC_MIF_LEAD_CTRL_D2_ADDR(base) ((base) + (0x0074UL))
#define SOC_MDC_MIF_OS_CTRL_D2_ADDR(base) ((base) + (0x0078UL))
#define SOC_MDC_MIF_STAT_D2_ADDR(base) ((base) + (0x007CUL))
#define SOC_MDC_MIF_CTRL_W0_ADDR(base) ((base) + (0x00A0UL))
#define SOC_MDC_MIF_LEAD_CTRL_W0_ADDR(base) ((base) + (0x00A4UL))
#define SOC_MDC_MIF_OS_CTRL_W0_ADDR(base) ((base) + (0x00A8UL))
#define SOC_MDC_MIF_STAT_W0_ADDR(base) ((base) + (0x00ACUL))
#define SOC_MDC_MIF_CTRL_W1_ADDR(base) ((base) + (0x00B0UL))
#define SOC_MDC_MIF_LEAD_CTRL_W1_ADDR(base) ((base) + (0x00B4UL))
#define SOC_MDC_MIF_OS_CTRL_W1_ADDR(base) ((base) + (0x00B8UL))
#define SOC_MDC_MIF_STAT_W1_ADDR(base) ((base) + (0x00BCUL))
#else
#define SOC_MDC_MIF_CLK_CTL_ADDR(base) ((base) + (0x0000))
#define SOC_MDC_MIF_MEM_CTRL_ADDR(base) ((base) + (0x0004))
#define SOC_MDC_MIF_SHADOW_READ_ADDR(base) ((base) + (0x0008))
#define SOC_MDC_AIF_CMD_RELOAD_ADDR(base) ((base) + (0x000C))
#define SOC_MDC_MIF_CTRL_D0_ADDR(base) ((base) + (0x0010))
#define SOC_MDC_MIF_LEAD_CTRL_D0_ADDR(base) ((base) + (0x0014))
#define SOC_MDC_MIF_OS_CTRL_D0_ADDR(base) ((base) + (0x0018))
#define SOC_MDC_MIF_STAT_D0_ADDR(base) ((base) + (0x001C))
#define SOC_MDC_MIF_CTRL_D1_ADDR(base) ((base) + (0x0020))
#define SOC_MDC_MIF_LEAD_CTRL_D1_ADDR(base) ((base) + (0x0024))
#define SOC_MDC_MIF_OS_CTRL_D1_ADDR(base) ((base) + (0x0028))
#define SOC_MDC_MIF_STAT_D1_ADDR(base) ((base) + (0x002C))
#define SOC_MDC_MIF_CTRL_V0_ADDR(base) ((base) + (0x0030))
#define SOC_MDC_MIF_LEAD_CTRL_V0_ADDR(base) ((base) + (0x0034))
#define SOC_MDC_MIF_OS_CTRL_V0_ADDR(base) ((base) + (0x0038))
#define SOC_MDC_MIF_STAT_V0_ADDR(base) ((base) + (0x003C))
#define SOC_MDC_MIF_CTRL_D2_ADDR(base) ((base) + (0x0070))
#define SOC_MDC_MIF_LEAD_CTRL_D2_ADDR(base) ((base) + (0x0074))
#define SOC_MDC_MIF_OS_CTRL_D2_ADDR(base) ((base) + (0x0078))
#define SOC_MDC_MIF_STAT_D2_ADDR(base) ((base) + (0x007C))
#define SOC_MDC_MIF_CTRL_W0_ADDR(base) ((base) + (0x00A0))
#define SOC_MDC_MIF_LEAD_CTRL_W0_ADDR(base) ((base) + (0x00A4))
#define SOC_MDC_MIF_OS_CTRL_W0_ADDR(base) ((base) + (0x00A8))
#define SOC_MDC_MIF_STAT_W0_ADDR(base) ((base) + (0x00AC))
#define SOC_MDC_MIF_CTRL_W1_ADDR(base) ((base) + (0x00B0))
#define SOC_MDC_MIF_LEAD_CTRL_W1_ADDR(base) ((base) + (0x00B4))
#define SOC_MDC_MIF_OS_CTRL_W1_ADDR(base) ((base) + (0x00B8))
#define SOC_MDC_MIF_STAT_W1_ADDR(base) ((base) + (0x00BC))
#endif
#ifndef __SOC_H_FOR_ASM__
#define SOC_MDC_ISPIF_EN_ADDR(base) ((base) + (0x000UL))
#define SOC_MDC_ISPIF_CLK_SEL_ADDR(base) ((base) + (0x004UL))
#define SOC_MDC_ISPIF_CLK_EN_ADDR(base) ((base) + (0x008UL))
#define SOC_MDC_ISPIF_IN_SIZE_ADDR(base) ((base) + (0x00CUL))
#define SOC_MDC_ISPIF_RD_SHADOW_ADDR(base) ((base) + (0x010UL))
#define SOC_MDC_ISPIF_DBG0_ADDR(base) ((base) + (0x014UL))
#define SOC_MDC_ISPIF_DBG1_ADDR(base) ((base) + (0x018UL))
#define SOC_MDC_ISPIF_DBG2_ADDR(base) ((base) + (0x01CUL))
#else
#define SOC_MDC_ISPIF_EN_ADDR(base) ((base) + (0x000))
#define SOC_MDC_ISPIF_CLK_SEL_ADDR(base) ((base) + (0x004))
#define SOC_MDC_ISPIF_CLK_EN_ADDR(base) ((base) + (0x008))
#define SOC_MDC_ISPIF_IN_SIZE_ADDR(base) ((base) + (0x00C))
#define SOC_MDC_ISPIF_RD_SHADOW_ADDR(base) ((base) + (0x010))
#define SOC_MDC_ISPIF_DBG0_ADDR(base) ((base) + (0x014))
#define SOC_MDC_ISPIF_DBG1_ADDR(base) ((base) + (0x018))
#define SOC_MDC_ISPIF_DBG2_ADDR(base) ((base) + (0x01C))
#endif
#ifndef __SOC_H_FOR_ASM__
#define SOC_MDC_CMDLIST_CH_PENDING_CLR_ADDR(base,i) ((base) + (0x0000+0x40*(i)))
#define SOC_MDC_CMDLIST_CH_CTRL_ADDR(base,i) ((base) + (0x0004+0x40*(i)))
#define SOC_MDC_CMDLIST_CH_STATUS_ADDR(base,i) ((base) + (0x0008+0x40*(i)))
#define SOC_MDC_CMDLIST_CH_STAAD_ADDR(base,i) ((base) + (0x000C+0x40*(i)))
#define SOC_MDC_CMDLIST_CH_CURAD_ADDR(base,i) ((base) + (0x0010+0x40*(i)))
#define SOC_MDC_CMDLIST_CH_INTE_ADDR(base,i) ((base) + (0x0014+0x40*(i)))
#define SOC_MDC_CMDLIST_CH_INTC_ADDR(base,i) ((base) + (0x0018+0x40*(i)))
#define SOC_MDC_CMDLIST_CH_INTS_ADDR(base,i) ((base) + (0x001C+0x40*(i)))
#define SOC_MDC_CMDLIST_CH_DBG_ADDR(base,i) ((base) + (0x0028+0x40*(i)))
#define SOC_MDC_CMDLIST_DBG_ADDR(base) ((base) + (0x0700UL))
#define SOC_MDC_BUF_DBG_EN_ADDR(base) ((base) + (0x0704UL))
#define SOC_MDC_BUF_DBG_CNT_CLR_ADDR(base) ((base) + (0x0708UL))
#define SOC_MDC_BUF_DBG_CNT_ADDR(base) ((base) + (0x070CUL))
#define SOC_MDC_CMDLIST_TIMEOUT_TH_ADDR(base) ((base) + (0x0710UL))
#define SOC_MDC_CMDLIST_START_ADDR(base) ((base) + (0x0714UL))
#define SOC_MDC_CMDLIST_ADDR_MASK_EN_ADDR(base) ((base) + (0x0718UL))
#define SOC_MDC_CMDLIST_ADDR_MASK_DIS_ADDR(base) ((base) + (0x071CUL))
#define SOC_MDC_CMDLIST_ADDR_MASK_STATUS_ADDR(base) ((base) + (0x0720UL))
#define SOC_MDC_CMDLIST_TASK_CONTINUE_ADDR(base) ((base) + (0x0724UL))
#define SOC_MDC_CMDLIST_TASK_STATUS_ADDR(base) ((base) + (0x0728UL))
#define SOC_MDC_CMDLIST_CTRL_ADDR(base) ((base) + (0x072CUL))
#define SOC_MDC_CMDLIST_SECU_ADDR(base) ((base) + (0x0730UL))
#define SOC_MDC_CMDLIST_INTS_ADDR(base) ((base) + (0x0734UL))
#define SOC_MDC_CMDLIST_SWRST_ADDR(base) ((base) + (0x0738UL))
#define SOC_MDC_CMD_MEM_CTRL_ADDR(base) ((base) + (0x073CUL))
#define SOC_MDC_CMD_CLK_SEL_ADDR(base) ((base) + (0x0740UL))
#define SOC_MDC_CMD_CLK_EN_ADDR(base) ((base) + (0x0744UL))
#else
#define SOC_MDC_CMDLIST_CH_PENDING_CLR_ADDR(base,i) ((base) + (0x0000+0x40*(i)))
#define SOC_MDC_CMDLIST_CH_CTRL_ADDR(base,i) ((base) + (0x0004+0x40*(i)))
#define SOC_MDC_CMDLIST_CH_STATUS_ADDR(base,i) ((base) + (0x0008+0x40*(i)))
#define SOC_MDC_CMDLIST_CH_STAAD_ADDR(base,i) ((base) + (0x000C+0x40*(i)))
#define SOC_MDC_CMDLIST_CH_CURAD_ADDR(base,i) ((base) + (0x0010+0x40*(i)))
#define SOC_MDC_CMDLIST_CH_INTE_ADDR(base,i) ((base) + (0x0014+0x40*(i)))
#define SOC_MDC_CMDLIST_CH_INTC_ADDR(base,i) ((base) + (0x0018+0x40*(i)))
#define SOC_MDC_CMDLIST_CH_INTS_ADDR(base,i) ((base) + (0x001C+0x40*(i)))
#define SOC_MDC_CMDLIST_CH_DBG_ADDR(base,i) ((base) + (0x0028+0x40*(i)))
#define SOC_MDC_CMDLIST_DBG_ADDR(base) ((base) + (0x0700))
#define SOC_MDC_BUF_DBG_EN_ADDR(base) ((base) + (0x0704))
#define SOC_MDC_BUF_DBG_CNT_CLR_ADDR(base) ((base) + (0x0708))
#define SOC_MDC_BUF_DBG_CNT_ADDR(base) ((base) + (0x070C))
#define SOC_MDC_CMDLIST_TIMEOUT_TH_ADDR(base) ((base) + (0x0710))
#define SOC_MDC_CMDLIST_START_ADDR(base) ((base) + (0x0714))
#define SOC_MDC_CMDLIST_ADDR_MASK_EN_ADDR(base) ((base) + (0x0718))
#define SOC_MDC_CMDLIST_ADDR_MASK_DIS_ADDR(base) ((base) + (0x071C))
#define SOC_MDC_CMDLIST_ADDR_MASK_STATUS_ADDR(base) ((base) + (0x0720))
#define SOC_MDC_CMDLIST_TASK_CONTINUE_ADDR(base) ((base) + (0x0724))
#define SOC_MDC_CMDLIST_TASK_STATUS_ADDR(base) ((base) + (0x0728))
#define SOC_MDC_CMDLIST_CTRL_ADDR(base) ((base) + (0x072C))
#define SOC_MDC_CMDLIST_SECU_ADDR(base) ((base) + (0x0730))
#define SOC_MDC_CMDLIST_INTS_ADDR(base) ((base) + (0x0734))
#define SOC_MDC_CMDLIST_SWRST_ADDR(base) ((base) + (0x0738))
#define SOC_MDC_CMD_MEM_CTRL_ADDR(base) ((base) + (0x073C))
#define SOC_MDC_CMD_CLK_SEL_ADDR(base) ((base) + (0x0740))
#define SOC_MDC_CMD_CLK_EN_ADDR(base) ((base) + (0x0744))
#endif
#ifndef __SOC_H_FOR_ASM__
#define SOC_MDC_RCH0_FLUSH_EN_ADDR(base) ((base) + (0x0100UL))
#define SOC_MDC_RCH1_FLUSH_EN_ADDR(base) ((base) + (0x0104UL))
#define SOC_MDC_RCH2_FLUSH_EN_ADDR(base) ((base) + (0x0108UL))
#define SOC_MDC_RCH6_FLUSH_EN_ADDR(base) ((base) + (0x0118UL))
#define SOC_MDC_WCH0_FLUSH_EN_ADDR(base) ((base) + (0x0120UL))
#define SOC_MDC_WCH1_FLUSH_EN_ADDR(base) ((base) + (0x0124UL))
#define SOC_MDC_OV2_FLUSH_EN_ADDR(base) ((base) + (0x0130UL))
#define SOC_MDC_ISPIF_FLUSH_EN_ADDR(base) ((base) + (0x0134UL))
#define SOC_MDC_RCH0_OV_OEN_ADDR(base) ((base) + (0x0160UL))
#define SOC_MDC_RCH1_OV_OEN_ADDR(base) ((base) + (0x0164UL))
#define SOC_MDC_RCH2_OV_OEN_ADDR(base) ((base) + (0x0168UL))
#define SOC_MDC_RCH6_OV_OEN_ADDR(base) ((base) + (0x0178UL))
#define SOC_MDC_RCH_OV2_SEL0_ADDR(base) ((base) + (0x0188UL))
#define SOC_MDC_RCH_OV3_SEL_ADDR(base) ((base) + (0x018CUL))
#define SOC_MDC_WCH0_OV_IEN_ADDR(base) ((base) + (0x01A0UL))
#define SOC_MDC_WCH1_OV_IEN_ADDR(base) ((base) + (0x01A4UL))
#define SOC_MDC_WCH_OV2_SEL_ADDR(base) ((base) + (0x01A8UL))
#define SOC_MDC_ISP_WCH_SEL_ADDR(base) ((base) + (0x01ACUL))
#define SOC_MDC_RCH0_STARTY_ADDR(base) ((base) + (0x01C0UL))
#define SOC_MDC_RCH1_STARTY_ADDR(base) ((base) + (0x01C4UL))
#define SOC_MDC_RCH2_STARTY_ADDR(base) ((base) + (0x01C8UL))
#define SOC_MDC_RCH6_STARTY_ADDR(base) ((base) + (0x01D8UL))
#define SOC_MDC_MCTL_CLK_SEL_ADDR(base) ((base) + (0x01F0UL))
#define SOC_MDC_MCTL_CLK_EN_ADDR(base) ((base) + (0x01F4UL))
#define SOC_MDC_MOD_CLK_SEL_ADDR(base) ((base) + (0x01F8UL))
#define SOC_MDC_MOD_CLK_EN_ADDR(base) ((base) + (0x01FCUL))
#define SOC_MDC_MOD0_DBG_ADDR(base) ((base) + (0x0200UL))
#define SOC_MDC_MOD1_DBG_ADDR(base) ((base) + (0x0204UL))
#define SOC_MDC_MOD2_DBG_ADDR(base) ((base) + (0x0208UL))
#define SOC_MDC_MOD6_DBG_ADDR(base) ((base) + (0x0218UL))
#define SOC_MDC_MOD8_DBG_ADDR(base) ((base) + (0x0220UL))
#define SOC_MDC_MOD9_DBG_ADDR(base) ((base) + (0x0224UL))
#define SOC_MDC_MOD12_DBG_ADDR(base) ((base) + (0x0230UL))
#define SOC_MDC_MOD13_DBG_ADDR(base) ((base) + (0x0234UL))
#define SOC_MDC_MOD17_DBG_ADDR(base) ((base) + (0x0244UL))
#define SOC_MDC_MOD18_DBG_ADDR(base) ((base) + (0x0248UL))
#define SOC_MDC_MOD0_STATUS_ADDR(base) ((base) + (0x0280UL))
#define SOC_MDC_MOD1_STATUS_ADDR(base) ((base) + (0x0284UL))
#define SOC_MDC_MOD2_STATUS_ADDR(base) ((base) + (0x0288UL))
#define SOC_MDC_MOD6_STATUS_ADDR(base) ((base) + (0x0298UL))
#define SOC_MDC_MOD8_STATUS_ADDR(base) ((base) + (0x02A0UL))
#define SOC_MDC_MOD9_STATUS_ADDR(base) ((base) + (0x02A4UL))
#define SOC_MDC_MOD12_STATUS_ADDR(base) ((base) + (0x02B0UL))
#define SOC_MDC_MOD13_STATUS_ADDR(base) ((base) + (0x02B4UL))
#define SOC_MDC_MOD17_STATUS_ADDR(base) ((base) + (0x02C4UL))
#define SOC_MDC_MOD18_STATUS_ADDR(base) ((base) + (0x02C8UL))
#define SOC_MDC_SW_DBG_ADDR(base) ((base) + (0x0300UL))
#define SOC_MDC_SW0_STATUS0_ADDR(base) ((base) + (0x0304UL))
#define SOC_MDC_SW1_STATUS_ADDR(base) ((base) + (0x0324UL))
#else
#define SOC_MDC_RCH0_FLUSH_EN_ADDR(base) ((base) + (0x0100))
#define SOC_MDC_RCH1_FLUSH_EN_ADDR(base) ((base) + (0x0104))
#define SOC_MDC_RCH2_FLUSH_EN_ADDR(base) ((base) + (0x0108))
#define SOC_MDC_RCH6_FLUSH_EN_ADDR(base) ((base) + (0x0118))
#define SOC_MDC_WCH0_FLUSH_EN_ADDR(base) ((base) + (0x0120))
#define SOC_MDC_WCH1_FLUSH_EN_ADDR(base) ((base) + (0x0124))
#define SOC_MDC_OV2_FLUSH_EN_ADDR(base) ((base) + (0x0130))
#define SOC_MDC_ISPIF_FLUSH_EN_ADDR(base) ((base) + (0x0134))
#define SOC_MDC_RCH0_OV_OEN_ADDR(base) ((base) + (0x0160))
#define SOC_MDC_RCH1_OV_OEN_ADDR(base) ((base) + (0x0164))
#define SOC_MDC_RCH2_OV_OEN_ADDR(base) ((base) + (0x0168))
#define SOC_MDC_RCH6_OV_OEN_ADDR(base) ((base) + (0x0178))
#define SOC_MDC_RCH_OV2_SEL0_ADDR(base) ((base) + (0x0188))
#define SOC_MDC_RCH_OV3_SEL_ADDR(base) ((base) + (0x018C))
#define SOC_MDC_WCH0_OV_IEN_ADDR(base) ((base) + (0x01A0))
#define SOC_MDC_WCH1_OV_IEN_ADDR(base) ((base) + (0x01A4))
#define SOC_MDC_WCH_OV2_SEL_ADDR(base) ((base) + (0x01A8))
#define SOC_MDC_ISP_WCH_SEL_ADDR(base) ((base) + (0x01AC))
#define SOC_MDC_RCH0_STARTY_ADDR(base) ((base) + (0x01C0))
#define SOC_MDC_RCH1_STARTY_ADDR(base) ((base) + (0x01C4))
#define SOC_MDC_RCH2_STARTY_ADDR(base) ((base) + (0x01C8))
#define SOC_MDC_RCH6_STARTY_ADDR(base) ((base) + (0x01D8))
#define SOC_MDC_MCTL_CLK_SEL_ADDR(base) ((base) + (0x01F0))
#define SOC_MDC_MCTL_CLK_EN_ADDR(base) ((base) + (0x01F4))
#define SOC_MDC_MOD_CLK_SEL_ADDR(base) ((base) + (0x01F8))
#define SOC_MDC_MOD_CLK_EN_ADDR(base) ((base) + (0x01FC))
#define SOC_MDC_MOD0_DBG_ADDR(base) ((base) + (0x0200))
#define SOC_MDC_MOD1_DBG_ADDR(base) ((base) + (0x0204))
#define SOC_MDC_MOD2_DBG_ADDR(base) ((base) + (0x0208))
#define SOC_MDC_MOD6_DBG_ADDR(base) ((base) + (0x0218))
#define SOC_MDC_MOD8_DBG_ADDR(base) ((base) + (0x0220))
#define SOC_MDC_MOD9_DBG_ADDR(base) ((base) + (0x0224))
#define SOC_MDC_MOD12_DBG_ADDR(base) ((base) + (0x0230))
#define SOC_MDC_MOD13_DBG_ADDR(base) ((base) + (0x0234))
#define SOC_MDC_MOD17_DBG_ADDR(base) ((base) + (0x0244))
#define SOC_MDC_MOD18_DBG_ADDR(base) ((base) + (0x0248))
#define SOC_MDC_MOD0_STATUS_ADDR(base) ((base) + (0x0280))
#define SOC_MDC_MOD1_STATUS_ADDR(base) ((base) + (0x0284))
#define SOC_MDC_MOD2_STATUS_ADDR(base) ((base) + (0x0288))
#define SOC_MDC_MOD6_STATUS_ADDR(base) ((base) + (0x0298))
#define SOC_MDC_MOD8_STATUS_ADDR(base) ((base) + (0x02A0))
#define SOC_MDC_MOD9_STATUS_ADDR(base) ((base) + (0x02A4))
#define SOC_MDC_MOD12_STATUS_ADDR(base) ((base) + (0x02B0))
#define SOC_MDC_MOD13_STATUS_ADDR(base) ((base) + (0x02B4))
#define SOC_MDC_MOD17_STATUS_ADDR(base) ((base) + (0x02C4))
#define SOC_MDC_MOD18_STATUS_ADDR(base) ((base) + (0x02C8))
#define SOC_MDC_SW_DBG_ADDR(base) ((base) + (0x0300))
#define SOC_MDC_SW0_STATUS0_ADDR(base) ((base) + (0x0304))
#define SOC_MDC_SW1_STATUS_ADDR(base) ((base) + (0x0324))
#endif
#ifndef __SOC_H_FOR_ASM__
#define SOC_MDC_CTL_EN_ADDR(base) ((base) + (0x0000UL))
#define SOC_MDC_CTL_MUTEX_ADDR(base) ((base) + (0x0004UL))
#define SOC_MDC_CTL_MUTEX_STATUS_ADDR(base) ((base) + (0x0008UL))
#define SOC_MDC_CTL_MUTEX_OV_ADDR(base) ((base) + (0x0018UL))
#define SOC_MDC_CTL_MUTEX_WCH0_ADDR(base) ((base) + (0x0020UL))
#define SOC_MDC_CTL_MUTEX_WCH1_ADDR(base) ((base) + (0x0024UL))
#define SOC_MDC_CTL_MUTEX_RCH0_ADDR(base) ((base) + (0x0030UL))
#define SOC_MDC_CTL_MUTEX_RCH1_ADDR(base) ((base) + (0x0034UL))
#define SOC_MDC_CTL_MUTEX_RCH2_ADDR(base) ((base) + (0x0038UL))
#define SOC_MDC_CTL_MUTEX_RCH6_ADDR(base) ((base) + (0x0048UL))
#define SOC_MDC_CTL_TOP_ADDR(base) ((base) + (0x0050UL))
#define SOC_MDC_CTL_FLUSH_STATUS_ADDR(base) ((base) + (0x0054UL))
#define SOC_MDC_CTL_CLEAR_ADDR(base) ((base) + (0x0058UL))
#define SOC_MDC_CTL_CACK_TOUT_ADDR(base) ((base) + (0x0060UL))
#define SOC_MDC_CTL_MUTEX_TOUT_ADDR(base) ((base) + (0x0064UL))
#define SOC_MDC_CTL_STATUS_ADDR(base) ((base) + (0x0068UL))
#define SOC_MDC_CTL_INTEN_ADDR(base) ((base) + (0x006CUL))
#define SOC_MDC_CTL_SW_ST_ADDR(base) ((base) + (0x0070UL))
#define SOC_MDC_CTL_ST_SEL_ADDR(base) ((base) + (0x0074UL))
#define SOC_MDC_CTL_END_SEL_ADDR(base) ((base) + (0x0078UL))
#define SOC_MDC_CTL_CLK_SEL_ADDR(base) ((base) + (0x0080UL))
#define SOC_MDC_CTL_CLK_EN_ADDR(base) ((base) + (0x0084UL))
#define SOC_MDC_CTL_DBG_ADDR(base) ((base) + (0x00E0UL))
#else
#define SOC_MDC_CTL_EN_ADDR(base) ((base) + (0x0000))
#define SOC_MDC_CTL_MUTEX_ADDR(base) ((base) + (0x0004))
#define SOC_MDC_CTL_MUTEX_STATUS_ADDR(base) ((base) + (0x0008))
#define SOC_MDC_CTL_MUTEX_OV_ADDR(base) ((base) + (0x0018))
#define SOC_MDC_CTL_MUTEX_WCH0_ADDR(base) ((base) + (0x0020))
#define SOC_MDC_CTL_MUTEX_WCH1_ADDR(base) ((base) + (0x0024))
#define SOC_MDC_CTL_MUTEX_RCH0_ADDR(base) ((base) + (0x0030))
#define SOC_MDC_CTL_MUTEX_RCH1_ADDR(base) ((base) + (0x0034))
#define SOC_MDC_CTL_MUTEX_RCH2_ADDR(base) ((base) + (0x0038))
#define SOC_MDC_CTL_MUTEX_RCH6_ADDR(base) ((base) + (0x0048))
#define SOC_MDC_CTL_TOP_ADDR(base) ((base) + (0x0050))
#define SOC_MDC_CTL_FLUSH_STATUS_ADDR(base) ((base) + (0x0054))
#define SOC_MDC_CTL_CLEAR_ADDR(base) ((base) + (0x0058))
#define SOC_MDC_CTL_CACK_TOUT_ADDR(base) ((base) + (0x0060))
#define SOC_MDC_CTL_MUTEX_TOUT_ADDR(base) ((base) + (0x0064))
#define SOC_MDC_CTL_STATUS_ADDR(base) ((base) + (0x0068))
#define SOC_MDC_CTL_INTEN_ADDR(base) ((base) + (0x006C))
#define SOC_MDC_CTL_SW_ST_ADDR(base) ((base) + (0x0070))
#define SOC_MDC_CTL_ST_SEL_ADDR(base) ((base) + (0x0074))
#define SOC_MDC_CTL_END_SEL_ADDR(base) ((base) + (0x0078))
#define SOC_MDC_CTL_CLK_SEL_ADDR(base) ((base) + (0x0080))
#define SOC_MDC_CTL_CLK_EN_ADDR(base) ((base) + (0x0084))
#define SOC_MDC_CTL_DBG_ADDR(base) ((base) + (0x00E0))
#endif
#ifndef __SOC_H_FOR_ASM__
#define SOC_MDC_DMA_OFT_X0_ADDR(base) ((base) + (0x0000UL))
#define SOC_MDC_DMA_OFT_Y0_ADDR(base) ((base) + (0x0004UL))
#define SOC_MDC_DMA_OFT_X1_ADDR(base) ((base) + (0x0008UL))
#define SOC_MDC_DMA_OFT_Y1_ADDR(base) ((base) + (0x000CUL))
#define SOC_MDC_DMA_MASK0_ADDR(base) ((base) + (0x0010UL))
#define SOC_MDC_DMA_MASK1_ADDR(base) ((base) + (0x0014UL))
#define SOC_MDC_DMA_STRETCH_SIZE_VRT_ADDR(base) ((base) + (0x0018UL))
#define SOC_MDC_DMA_CTRL_ADDR(base) ((base) + (0x001CUL))
#define SOC_MDC_DMA_TILE_SCRAM_ADDR(base) ((base) + (0x0020UL))
#define SOC_MDC_DMA_PULSE_ADDR(base) ((base) + (0x0028UL))
#define SOC_MDC_RWCH_CFG0_ADDR(base) ((base) + (0x0030UL))
#define SOC_MDC_DMA_DATA_ADDR0_ADDR(base) ((base) + (0x0060UL))
#define SOC_MDC_DMA_STRIDE0_ADDR(base) ((base) + (0x0064UL))
#define SOC_MDC_DMA_STRETCH_STRIDE0_ADDR(base) ((base) + (0x0068UL))
#define SOC_MDC_DMA_DATA_NUM0_ADDR(base) ((base) + (0x006CUL))
#define SOC_MDC_DMA_TEST0_ADDR(base) ((base) + (0x0070UL))
#define SOC_MDC_DMA_TEST1_ADDR(base) ((base) + (0x0074UL))
#define SOC_MDC_DMA_TEST3_ADDR(base) ((base) + (0x0078UL))
#define SOC_MDC_DMA_TEST4_ADDR(base) ((base) + (0x007CUL))
#define SOC_MDC_DMA_STATUS_Y_ADDR(base) ((base) + (0x0080UL))
#define SOC_MDC_DMA_DATA_ADDR1_ADDR(base) ((base) + (0x0084UL))
#define SOC_MDC_DMA_STRIDE1_ADDR(base) ((base) + (0x0088UL))
#define SOC_MDC_DMA_STRETCH_STRIDE1_ADDR(base) ((base) + (0x008CUL))
#define SOC_MDC_DMA_DATA_NUM1_ADDR(base) ((base) + (0x0090UL))
#define SOC_MDC_DMA_TEST0_U_ADDR(base) ((base) + (0x0094UL))
#define SOC_MDC_DMA_TEST1_U_ADDR(base) ((base) + (0x0098UL))
#define SOC_MDC_DMA_TEST3_U_ADDR(base) ((base) + (0x009CUL))
#define SOC_MDC_DMA_TEST4_U_ADDR(base) ((base) + (0x00A0UL))
#define SOC_MDC_DMA_STATUS_U_ADDR(base) ((base) + (0x00A4UL))
#define SOC_MDC_DMA_DATA_ADDR2_ADDR(base) ((base) + (0x00A8UL))
#define SOC_MDC_DMA_STRIDE2_ADDR(base) ((base) + (0x00ACUL))
#define SOC_MDC_DMA_STRETCH_STRIDE2_ADDR(base) ((base) + (0x00B0UL))
#define SOC_MDC_DMA_DATA_NUM2_ADDR(base) ((base) + (0x00B4UL))
#define SOC_MDC_DMA_TEST0_V_ADDR(base) ((base) + (0x00B8UL))
#define SOC_MDC_DMA_TEST1_V_ADDR(base) ((base) + (0x00BCUL))
#define SOC_MDC_DMA_TEST3_V_ADDR(base) ((base) + (0x00C0UL))
#define SOC_MDC_DMA_TEST4_V_ADDR(base) ((base) + (0x00C4UL))
#define SOC_MDC_DMA_STATUS_V_ADDR(base) ((base) + (0x00C8UL))
#define SOC_MDC_CH_RD_SHADOW_ADDR(base) ((base) + (0x00D0UL))
#define SOC_MDC_CH_CTL_ADDR(base) ((base) + (0x00D4UL))
#define SOC_MDC_CH_SECU_EN_ADDR(base) ((base) + (0x00D8UL))
#define SOC_MDC_CH_SW_END_REQ_ADDR(base) ((base) + (0x00DCUL))
#define SOC_MDC_CH_CLK_SEL_ADDR(base) ((base) + (0x00E0UL))
#define SOC_MDC_CH_CLK_EN_ADDR(base) ((base) + (0x00E4UL))
#define SOC_MDC_CH_DBG0_ADDR(base) ((base) + (0x00E8UL))
#define SOC_MDC_CH_DBG1_ADDR(base) ((base) + (0x00ECUL))
#define SOC_MDC_CH_DBG2_ADDR(base) ((base) + (0x00F0UL))
#define SOC_MDC_CH_DBG3_ADDR(base) ((base) + (0x00F4UL))
#define SOC_MDC_CH_DBG4_ADDR(base) ((base) + (0x00F8UL))
#define SOC_MDC_CH_DBG5_ADDR(base) ((base) + (0x00FCUL))
#define SOC_MDC_BITEXT_CTL_ADDR(base) ((base) + (0x0140UL))
#define SOC_MDC_BITEXT_REG_INI0_0_ADDR(base) ((base) + (0x0144UL))
#define SOC_MDC_BITEXT_REG_INI0_1_ADDR(base) ((base) + (0x0148UL))
#define SOC_MDC_BITEXT_REG_INI0_2_ADDR(base) ((base) + (0x014CUL))
#define SOC_MDC_BITEXT_REG_INI0_3_ADDR(base) ((base) + (0x0150UL))
#define SOC_MDC_BITEXT_REG_INI1_0_ADDR(base) ((base) + (0x0154UL))
#define SOC_MDC_BITEXT_REG_INI1_1_ADDR(base) ((base) + (0x0158UL))
#define SOC_MDC_BITEXT_REG_INI1_2_ADDR(base) ((base) + (0x015CUL))
#define SOC_MDC_BITEXT_REG_INI1_3_ADDR(base) ((base) + (0x0160UL))
#define SOC_MDC_BITEXT_REG_INI2_0_ADDR(base) ((base) + (0x0164UL))
#define SOC_MDC_BITEXT_REG_INI2_1_ADDR(base) ((base) + (0x0168UL))
#define SOC_MDC_BITEXT_REG_INI2_2_ADDR(base) ((base) + (0x016CUL))
#define SOC_MDC_BITEXT_REG_INI2_3_ADDR(base) ((base) + (0x0170UL))
#define SOC_MDC_BITEXT_POWER_CTRL_C_ADDR(base) ((base) + (0x0174UL))
#define SOC_MDC_BITEXT_POWER_CTRL_SHIFT_ADDR(base) ((base) + (0x0178UL))
#define SOC_MDC_BITEXT_FILT_00_ADDR(base) ((base) + (0x017CUL))
#define SOC_MDC_BITEXT_FILT_01_ADDR(base) ((base) + (0x0180UL))
#define SOC_MDC_BITEXT_FILT_02_ADDR(base) ((base) + (0x0184UL))
#define SOC_MDC_BITEXT_FILT_10_ADDR(base) ((base) + (0x0188UL))
#define SOC_MDC_BITEXT_FILT_11_ADDR(base) ((base) + (0x018CUL))
#define SOC_MDC_BITEXT_FILT_12_ADDR(base) ((base) + (0x0190UL))
#define SOC_MDC_BITEXT_FILT_20_ADDR(base) ((base) + (0x0194UL))
#define SOC_MDC_BITEXT_FILT_21_ADDR(base) ((base) + (0x01A0UL))
#define SOC_MDC_BITEXT_FILT_22_ADDR(base) ((base) + (0x01A4UL))
#define SOC_MDC_BITEXT_THD_R0_ADDR(base) ((base) + (0x01A8UL))
#define SOC_MDC_BITEXT_THD_R1_ADDR(base) ((base) + (0x01ACUL))
#define SOC_MDC_BITEXT_THD_G0_ADDR(base) ((base) + (0x01B0UL))
#define SOC_MDC_BITEXT_THD_G1_ADDR(base) ((base) + (0x01B4UL))
#define SOC_MDC_BITEXT_THD_B0_ADDR(base) ((base) + (0x01B8UL))
#define SOC_MDC_BITEXT_THD_B1_ADDR(base) ((base) + (0x01BCUL))
#define SOC_MDC_BITEXT0_DBG0_ADDR(base) ((base) + (0x01C0UL))
#define SOC_MDC_SCF_EN_HSCL_STR_ADDR(base) ((base) + (0x0200UL))
#define SOC_MDC_SCF_EN_VSCL_STR_ADDR(base) ((base) + (0x0204UL))
#define SOC_MDC_SCF_H_V_ORDER_ADDR(base) ((base) + (0x0208UL))
#define SOC_MDC_SCF_CH_CORE_GT_ADDR(base) ((base) + (0x020CUL))
#define SOC_MDC_SCF_INPUT_WIDTH_HEIGHT_ADDR(base) ((base) + (0x0210UL))
#define SOC_MDC_SCF_OUTPUT_WIDTH_HEIGHT_ADDR(base) ((base) + (0x0214UL))
#define SOC_MDC_SCF_COEF_MEM_CTRL_ADDR(base) ((base) + (0x0218UL))
#define SOC_MDC_SCF_EN_HSCL_ADDR(base) ((base) + (0x021CUL))
#define SOC_MDC_SCF_EN_VSCL_ADDR(base) ((base) + (0x0220UL))
#define SOC_MDC_SCF_ACC_HSCL_ADDR(base) ((base) + (0x0224UL))
#define SOC_MDC_SCF_ACC_HSCL1_ADDR(base) ((base) + (0x0228UL))
#define SOC_MDC_SCF_INC_HSCL_ADDR(base) ((base) + (0x0234UL))
#define SOC_MDC_SCF_ACC_VSCL_ADDR(base) ((base) + (0x0238UL))
#define SOC_MDC_SCF_ACC_VSCL1_ADDR(base) ((base) + (0x023CUL))
#define SOC_MDC_SCF_INC_VSCL_ADDR(base) ((base) + (0x0248UL))
#define SOC_MDC_SCF_EN_NONLINEAR_ADDR(base) ((base) + (0x024CUL))
#define SOC_MDC_SCF_EN_MMP_ADDR(base) ((base) + (0x027CUL))
#define SOC_MDC_SCF_DB_H0_ADDR(base) ((base) + (0x0280UL))
#define SOC_MDC_SCF_DB_H1_ADDR(base) ((base) + (0x0284UL))
#define SOC_MDC_SCF_DB_V0_ADDR(base) ((base) + (0x0288UL))
#define SOC_MDC_SCF_DB_V1_ADDR(base) ((base) + (0x028CUL))
#define SOC_MDC_SCF_LB_MEM_CTRL_ADDR(base) ((base) + (0x0290UL))
#define SOC_MDC_ARSR_INPUT_WIDTH_HEIGHT_ADDR(base) ((base) + (0x0300UL))
#define SOC_MDC_ARSR_OUTPUT_WIDTH_HEIGHT_ADDR(base) ((base) + (0x0304UL))
#define SOC_MDC_IHLEFT_ADDR(base) ((base) + (0x0308UL))
#define SOC_MDC_IHLEFT1_ADDR(base) ((base) + (0x030CUL))
#define SOC_MDC_IHRIGHT_ADDR(base) ((base) + (0x0310UL))
#define SOC_MDC_IHRIGHT1_ADDR(base) ((base) + (0x0314UL))
#define SOC_MDC_IVTOP_ADDR(base) ((base) + (0x0318UL))
#define SOC_MDC_IVBOTTOM_ADDR(base) ((base) + (0x031CUL))
#define SOC_MDC_IVBOTTOM1_ADDR(base) ((base) + (0x0320UL))
#define SOC_MDC_IHINC_ADDR(base) ((base) + (0x0324UL))
#define SOC_MDC_IVINC_ADDR(base) ((base) + (0x0328UL))
#define SOC_MDC_OFFSET_ADDR(base) ((base) + (0x032CUL))
#define SOC_MDC_MODE_ADDR(base) ((base) + (0x0330UL))
#define SOC_MDC_SKIN_THRES_Y_ADDR(base) ((base) + (0x0334UL))
#define SOC_MDC_SKIN_THRES_U_ADDR(base) ((base) + (0x0338UL))
#define SOC_MDC_SKIN_THRES_V_ADDR(base) ((base) + (0x033CUL))
#define SOC_MDC_SKIN_CFG0_ADDR(base) ((base) + (0x0340UL))
#define SOC_MDC_SKIN_CFG1_ADDR(base) ((base) + (0x0344UL))
#define SOC_MDC_SKIN_CFG2_ADDR(base) ((base) + (0x0348UL))
#define SOC_MDC_SHOOT_CFG1_ADDR(base) ((base) + (0x034CUL))
#define SOC_MDC_SHOOT_CFG2_ADDR(base) ((base) + (0x0350UL))
#define SOC_MDC_SHOOT_CFG3_ADDR(base) ((base) + (0x0354UL))
#define SOC_MDC_SHARP_CFG3_ADDR(base) ((base) + (0x0358UL))
#define SOC_MDC_SHARP_CFG4_ADDR(base) ((base) + (0x035CUL))
#define SOC_MDC_SHARP_CFG5_ADDR(base) ((base) + (0x0360UL))
#define SOC_MDC_SHARP_CFG6_ADDR(base) ((base) + (0x0364UL))
#define SOC_MDC_SHARP_CFG7_ADDR(base) ((base) + (0x0368UL))
#define SOC_MDC_SHARP_CFG8_ADDR(base) ((base) + (0x036CUL))
#define SOC_MDC_SHARPLEVEL_ADDR(base) ((base) + (0x0370UL))
#define SOC_MDC_SHPGAIN_LOW_ADDR(base) ((base) + (0x0374UL))
#define SOC_MDC_SHPGAIN_MID_ADDR(base) ((base) + (0x0378UL))
#define SOC_MDC_SHPGAIN_HIGH_ADDR(base) ((base) + (0x037CUL))
#define SOC_MDC_GAINCTRLSLOPH_MF_ADDR(base) ((base) + (0x0380UL))
#define SOC_MDC_GAINCTRLSLOPL_MF_ADDR(base) ((base) + (0x0384UL))
#define SOC_MDC_GAINCTRLSLOPH_HF_ADDR(base) ((base) + (0x0388UL))
#define SOC_MDC_GAINCTRLSLOPL_HF_ADDR(base) ((base) + (0x038CUL))
#define SOC_MDC_MF_LMT_ADDR(base) ((base) + (0x0390UL))
#define SOC_MDC_GAIN_MF_ADDR(base) ((base) + (0x0394UL))
#define SOC_MDC_MF_B_ADDR(base) ((base) + (0x0398UL))
#define SOC_MDC_HF_LMT_ADDR(base) ((base) + (0x039CUL))
#define SOC_MDC_GAIN_HF_ADDR(base) ((base) + (0x03A0UL))
#define SOC_MDC_HF_B_ADDR(base) ((base) + (0x03A4UL))
#define SOC_MDC_LF_CTRL_ADDR(base) ((base) + (0x03A8UL))
#define SOC_MDC_LF_VAR_ADDR(base) ((base) + (0x03ACUL))
#define SOC_MDC_LF_CTRL_SLOP_ADDR(base) ((base) + (0x03B0UL))
#define SOC_MDC_HF_SELECT_ADDR(base) ((base) + (0x03B4UL))
#define SOC_MDC_SHARP_CFG2_H_ADDR(base) ((base) + (0x03B8UL))
#define SOC_MDC_SHARP_CFG2_L_ADDR(base) ((base) + (0x03BCUL))
#define SOC_MDC_TEXTURW_ANALYSTS_ADDR(base) ((base) + (0x03D0UL))
#define SOC_MDC_INTPLSHOOTCTRL_ADDR(base) ((base) + (0x03D4UL))
#define SOC_MDC_ARSR2P_DEBUG0_ADDR(base) ((base) + (0x03D8UL))
#define SOC_MDC_ARSR2P_DEBUG1_ADDR(base) ((base) + (0x03DCUL))
#define SOC_MDC_ARSR2P_DEBUG2_ADDR(base) ((base) + (0x03E0UL))
#define SOC_MDC_ARSR2P_DEBUG3_ADDR(base) ((base) + (0x03E4UL))
#define SOC_MDC_ARSR2P_LB_MEM_CTRL_ADDR(base) ((base) + (0x03E8UL))
#define SOC_MDC_PCSC_IDC0_ADDR(base) ((base) + (0x0400UL))
#define SOC_MDC_PCSC_IDC2_ADDR(base) ((base) + (0x0404UL))
#define SOC_MDC_PCSC_ODC0_ADDR(base) ((base) + (0x0408UL))
#define SOC_MDC_PCSC_ODC2_ADDR(base) ((base) + (0x040CUL))
#define SOC_MDC_PCSC_P00_ADDR(base) ((base) + (0x0410UL))
#define SOC_MDC_PCSC_P01_ADDR(base) ((base) + (0x0414UL))
#define SOC_MDC_PCSC_P02_ADDR(base) ((base) + (0x0418UL))
#define SOC_MDC_PCSC_P10_ADDR(base) ((base) + (0x041CUL))
#define SOC_MDC_PCSC_P11_ADDR(base) ((base) + (0x0420UL))
#define SOC_MDC_PCSC_P12_ADDR(base) ((base) + (0x0424UL))
#define SOC_MDC_PCSC_P20_ADDR(base) ((base) + (0x0428UL))
#define SOC_MDC_PCSC_P21_ADDR(base) ((base) + (0x042CUL))
#define SOC_MDC_PCSC_P22_ADDR(base) ((base) + (0x0430UL))
#define SOC_MDC_PCSC_ICG_MODULE_ADDR(base) ((base) + (0x0434UL))
#define SOC_MDC_PCSC_MPREC_ADDR(base) ((base) + (0x0438UL))
#define SOC_MDC_POST_CLIP_DISP_SIZE_ADDR(base) ((base) + (0x0480UL))
#define SOC_MDC_POST_CLIP_CTL_HRZ_ADDR(base) ((base) + (0x0484UL))
#define SOC_MDC_POST_CLIP_CTL_VRZ_ADDR(base) ((base) + (0x0488UL))
#define SOC_MDC_POST_CLIP_EN_ADDR(base) ((base) + (0x048CUL))
#define SOC_MDC_CSC_IDC0_ADDR(base) ((base) + (0x0500UL))
#define SOC_MDC_CSC_IDC2_ADDR(base) ((base) + (0x0504UL))
#define SOC_MDC_CSC_ODC0_ADDR(base) ((base) + (0x0508UL))
#define SOC_MDC_CSC_ODC2_ADDR(base) ((base) + (0x050CUL))
#define SOC_MDC_CSC_P00_ADDR(base) ((base) + (0x0510UL))
#define SOC_MDC_CSC_P01_ADDR(base) ((base) + (0x0514UL))
#define SOC_MDC_CSC_P02_ADDR(base) ((base) + (0x0518UL))
#define SOC_MDC_CSC_P10_ADDR(base) ((base) + (0x051CUL))
#define SOC_MDC_CSC_P11_ADDR(base) ((base) + (0x0520UL))
#define SOC_MDC_CSC_P12_ADDR(base) ((base) + (0x0524UL))
#define SOC_MDC_CSC_P20_ADDR(base) ((base) + (0x0528UL))
#define SOC_MDC_CSC_P21_ADDR(base) ((base) + (0x052CUL))
#define SOC_MDC_CSC_P22_ADDR(base) ((base) + (0x0530UL))
#define SOC_MDC_CSC_ICG_MODULE_ADDR(base) ((base) + (0x0534UL))
#define SOC_MDC_CSC_MPREC_ADDR(base) ((base) + (0x0538UL))
#define SOC_MDC_CH_DEBUG_SEL_ADDR(base) ((base) + (0x600UL))
#define SOC_MDC_VPP_CTRL_ADDR(base) ((base) + (0x0700UL))
#define SOC_MDC_VPP_MEM_CTRL_ADDR(base) ((base) + (0x0704UL))
#define SOC_MDC_DMA_BUF_CTRL_ADDR(base) ((base) + (0x0800UL))
#define SOC_MDC_DMA_BUF_CPU_CTL_ADDR(base) ((base) + (0x0804UL))
#define SOC_MDC_DMA_BUF_CPU_START_ADDR(base) ((base) + (0x0808UL))
#define SOC_MDC_DMA_BUF_CPU_ADDR_ADDR(base) ((base) + (0x080CUL))
#define SOC_MDC_DMA_BUF_CPU_RDATA0_ADDR(base) ((base) + (0x0810UL))
#define SOC_MDC_DMA_BUF_CPU_RDATA1_ADDR(base) ((base) + (0x814UL))
#define SOC_MDC_DMA_BUF_CPU_RDATA2_ADDR(base) ((base) + (0x818UL))
#define SOC_MDC_DMA_BUF_CPU_RDATA3_ADDR(base) ((base) + (0x81CUL))
#define SOC_MDC_DMA_BUF_CPU_WDATA0_ADDR(base) ((base) + (0x820UL))
#define SOC_MDC_DMA_BUF_CPU_WDATA1_ADDR(base) ((base) + (0x824UL))
#define SOC_MDC_DMA_BUF_CPU_WDATA2_ADDR(base) ((base) + (0x828UL))
#define SOC_MDC_DMA_BUF_CPU_WDATA3_ADDR(base) ((base) + (0x82CUL))
#define SOC_MDC_DMA_REQ_END_ADDR(base) ((base) + (0x830UL))
#define SOC_MDC_DMA_BUF_DBGCFG_ADDR(base) ((base) + (0x834UL))
#define SOC_MDC_DMA_BUF_DBG0_ADDR(base) ((base) + (0x838UL))
#define SOC_MDC_DMA_BUF_DBG1_ADDR(base) ((base) + (0x83CUL))
#define SOC_MDC_DMA_BUF_DBG2_ADDR(base) ((base) + (0x840UL))
#define SOC_MDC_DMA_BUF_DBG3_ADDR(base) ((base) + (0x844UL))
#define SOC_MDC_DMA_BUF_DBG4_ADDR(base) ((base) + (0x848UL))
#define SOC_MDC_DMA_BUF_DBG5_ADDR(base) ((base) + (0x84CUL))
#define SOC_MDC_DMA_BUF_SIZE_ADDR(base) ((base) + (0x850UL))
#define SOC_MDC_DMA_BUF_MEM_CTRL_ADDR(base) ((base) + (0x0854UL))
#define SOC_MDC_AFBCD_HREG_HDR_PTR_LO_ADDR(base) ((base) + (0x0900UL))
#define SOC_MDC_AFBCD_HREG_PIC_WIDTH_ADDR(base) ((base) + (0x0904UL))
#define SOC_MDC_AFBCD_HREG_PIC_HEIGHT_ADDR(base) ((base) + (0x090CUL))
#define SOC_MDC_AFBCD_HREG_FORMAT_ADDR(base) ((base) + (0x0910UL))
#define SOC_MDC_AFBCD_CTL_ADDR(base) ((base) + (0x0914UL))
#define SOC_MDC_AFBCD_STR_ADDR(base) ((base) + (0x0918UL))
#define SOC_MDC_LINE_CROP_ADDR(base) ((base) + (0x091CUL))
#define SOC_MDC_INPUT_HEADER_STRIDE_ADDR(base) ((base) + (0x0920UL))
#define SOC_MDC_AFBCD_PAYLOAD_STRIDE_ADDR(base) ((base) + (0x0924UL))
#define SOC_MDC_MM_BASE_ADDR(base) ((base) + (0x0928UL))
#define SOC_MDC_AFBCD_PAYLOAD_POINTER_ADDR(base) ((base) + (0x0930UL))
#define SOC_MDC_HEIGHT_BF_STR_ADDR(base) ((base) + (0x0934UL))
#define SOC_MDC_OS_CFG_ADDR(base) ((base) + (0x0938UL))
#define SOC_MDC_AFBCD_MEM_CTRL_ADDR(base) ((base) + (0x093CUL))
#define SOC_MDC_AFBCD_SCRAMBLE_MODE_ADDR(base) ((base) + (0x0940UL))
#define SOC_MDC_AFBCD_HEADER_POINTER_OFFSET_ADDR(base) ((base) + (0x0944UL))
#define SOC_MDC_AFBCD_MONITOR_REG1_ADDR(base) ((base) + (0x0948UL))
#define SOC_MDC_AFBCD_MONITOR_REG2_ADDR(base) ((base) + (0x094CUL))
#define SOC_MDC_AFBCD_MONITOR_REG3_ADDR(base) ((base) + (0x0950UL))
#define SOC_MDC_AFBCD_DEBUG_REG0_ADDR(base) ((base) + (0x0954UL))
#define SOC_MDC_FBCD_CREG_FBCD_CTRL_MODE_ADDR(base) ((base) + (0x0960UL))
#define SOC_MDC_AFBCD_HREG_HDR_PTR_LO_1_ADDR(base) ((base) + (0x0964UL))
#define SOC_MDC_AFBCD_HREG_PLD_PTR_LO_1_ADDR(base) ((base) + (0x0968UL))
#define SOC_MDC_FBCD_HEADER_SRTIDE_1_ADDR(base) ((base) + (0x096CUL))
#define SOC_MDC_FBCD_PAYLOAD_STRIDE_1_ADDR(base) ((base) + (0x0970UL))
#define SOC_MDC_FBCD_BLOCK_TYPE_ADDR(base) ((base) + (0x0974UL))
#define SOC_MDC_MM_BASE_1_ADDR(base) ((base) + (0x0978UL))
#define SOC_MDC_MM_BASE_2_ADDR(base) ((base) + (0x097CUL))
#define SOC_MDC_MM_BASE_3_ADDR(base) ((base) + (0x0980UL))
#define SOC_MDC_HFBCD_MEM_CTRL_ADDR(base) ((base) + (0x0984UL))
#define SOC_MDC_HFBCD_MEM_CTRL_1_ADDR(base) ((base) + (0x0988UL))
#define SOC_MDC_FBCD_CREG_FBCD_CLK_GATE_ADDR(base) ((base) + (0x098CUL))
#define SOC_MDC_HFBCD_MONTIOR_REG1_ADDR(base) ((base) + (0x0990UL))
#define SOC_MDC_HFBCD_MONTIOR_REG2_ADDR(base) ((base) + (0x0994UL))
#define SOC_MDC_HFBCD_MONTIOR_REG3_ADDR(base) ((base) + (0x0998UL))
#define SOC_MDC_HFBCD_DEBUG_REG0_ADDR(base) ((base) + (0x099CUL))
#define SOC_MDC_AFBCD_MONTIOR_REG4_ADDR(base) ((base) + (0x09A0UL))
#define SOC_MDC_AFBCD_MONTIOR_REG5_ADDR(base) ((base) + (0x09A4UL))
#define SOC_MDC_HFBCD_MONTIOR_REG4_ADDR(base) ((base) + (0x09A8UL))
#define SOC_MDC_HFBCD_MONTIOR_REG5_ADDR(base) ((base) + (0x09ACUL))
#define SOC_MDC_HEBCD_OS_CFG_ADDR(base) ((base) + (0x09B0UL))
#define SOC_MDC_HEBCD_TAG_VALUE0_ADDR(base) ((base) + (0x09C0UL))
#define SOC_MDC_HEBCD_TAG_VALUE1_ADDR(base) ((base) + (0x09C4UL))
#define SOC_MDC_HEBCD_TAG_VALUE2_ADDR(base) ((base) + (0x09C8UL))
#define SOC_MDC_HEBCD_TAG_VALUE3_ADDR(base) ((base) + (0x09CCUL))
#define SOC_MDC_HEBCD_RESERVED0_ADDR(base) ((base) + (0x09D0UL))
#define SOC_MDC_HEBCD_RESERVED1_ADDR(base) ((base) + (0x09D4UL))
#define SOC_MDC_HEBCD_RESERVED2_ADDR(base) ((base) + (0x09D8UL))
#define SOC_MDC_HEBCD_RESERVED3_ADDR(base) ((base) + (0x09DCUL))
#define SOC_MDC_HEBCD_MONITOR_REG0_ADDR(base) ((base) + (0x09E0UL))
#define SOC_MDC_HEBCD_MONITOR_REG1_ADDR(base) ((base) + (0x09E4UL))
#define SOC_MDC_HEBCD_MONITOR_REG2_ADDR(base) ((base) + (0x09E8UL))
#define SOC_MDC_HEBCD_MONITOR_REG3_ADDR(base) ((base) + (0x09ECUL))
#define SOC_MDC_HEBCD_MEM_CTRL_ADDR(base) ((base) + (0x09F0UL))
#define SOC_MDC_HEBCD_CLK_GATE_ADDR(base) ((base) + (0x09F4UL))
#define SOC_MDC_REG_DEFAULT_ADDR(base) ((base) + (0x0A00UL))
#define SOC_MDC_DMA_ADDR_EXT_ADDR(base) ((base) + (0x0A40UL))
#define SOC_MDC_V0_SCF_VIDEO_6TAP_COEF_ADDR(base,l) ((base) + (0x1000+0x4*(l)))
#define SOC_MDC_V0_SCF_VIDEO_5TAP_COEF_ADDR(base,m) ((base) + (0x3000+0x4*(m)))
#define SOC_MDC_V0_SCF_VIDEO_4TAP_COEF_ADDR(base,n) ((base) + (0x3800+0x4*(n)))
#define SOC_MDC_COEFY_V_ADDR(base,o) ((base) + (0x5000+0x4*(o)))
#define SOC_MDC_COEFY_H_ADDR(base,p) ((base) + (0x5100+0x4*(p)))
#define SOC_MDC_COEFA_V_ADDR(base,o) ((base) + (0x5300+0x4*(o)))
#define SOC_MDC_COEFA_H_ADDR(base,p) ((base) + (0x5400+0x4*(p)))
#define SOC_MDC_COEFUV_V_ADDR(base,o) ((base) + (0x5600+0x4*(o)))
#define SOC_MDC_COEFUV_H_ADDR(base,p) ((base) + (0x5700+0x4*(p)))
#else
#define SOC_MDC_DMA_OFT_X0_ADDR(base) ((base) + (0x0000))
#define SOC_MDC_DMA_OFT_Y0_ADDR(base) ((base) + (0x0004))
#define SOC_MDC_DMA_OFT_X1_ADDR(base) ((base) + (0x0008))
#define SOC_MDC_DMA_OFT_Y1_ADDR(base) ((base) + (0x000C))
#define SOC_MDC_DMA_MASK0_ADDR(base) ((base) + (0x0010))
#define SOC_MDC_DMA_MASK1_ADDR(base) ((base) + (0x0014))
#define SOC_MDC_DMA_STRETCH_SIZE_VRT_ADDR(base) ((base) + (0x0018))
#define SOC_MDC_DMA_CTRL_ADDR(base) ((base) + (0x001C))
#define SOC_MDC_DMA_TILE_SCRAM_ADDR(base) ((base) + (0x0020))
#define SOC_MDC_DMA_PULSE_ADDR(base) ((base) + (0x0028))
#define SOC_MDC_RWCH_CFG0_ADDR(base) ((base) + (0x0030))
#define SOC_MDC_DMA_DATA_ADDR0_ADDR(base) ((base) + (0x0060))
#define SOC_MDC_DMA_STRIDE0_ADDR(base) ((base) + (0x0064))
#define SOC_MDC_DMA_STRETCH_STRIDE0_ADDR(base) ((base) + (0x0068))
#define SOC_MDC_DMA_DATA_NUM0_ADDR(base) ((base) + (0x006C))
#define SOC_MDC_DMA_TEST0_ADDR(base) ((base) + (0x0070))
#define SOC_MDC_DMA_TEST1_ADDR(base) ((base) + (0x0074))
#define SOC_MDC_DMA_TEST3_ADDR(base) ((base) + (0x0078))
#define SOC_MDC_DMA_TEST4_ADDR(base) ((base) + (0x007C))
#define SOC_MDC_DMA_STATUS_Y_ADDR(base) ((base) + (0x0080))
#define SOC_MDC_DMA_DATA_ADDR1_ADDR(base) ((base) + (0x0084))
#define SOC_MDC_DMA_STRIDE1_ADDR(base) ((base) + (0x0088))
#define SOC_MDC_DMA_STRETCH_STRIDE1_ADDR(base) ((base) + (0x008C))
#define SOC_MDC_DMA_DATA_NUM1_ADDR(base) ((base) + (0x0090))
#define SOC_MDC_DMA_TEST0_U_ADDR(base) ((base) + (0x0094))
#define SOC_MDC_DMA_TEST1_U_ADDR(base) ((base) + (0x0098))
#define SOC_MDC_DMA_TEST3_U_ADDR(base) ((base) + (0x009C))
#define SOC_MDC_DMA_TEST4_U_ADDR(base) ((base) + (0x00A0))
#define SOC_MDC_DMA_STATUS_U_ADDR(base) ((base) + (0x00A4))
#define SOC_MDC_DMA_DATA_ADDR2_ADDR(base) ((base) + (0x00A8))
#define SOC_MDC_DMA_STRIDE2_ADDR(base) ((base) + (0x00AC))
#define SOC_MDC_DMA_STRETCH_STRIDE2_ADDR(base) ((base) + (0x00B0))
#define SOC_MDC_DMA_DATA_NUM2_ADDR(base) ((base) + (0x00B4))
#define SOC_MDC_DMA_TEST0_V_ADDR(base) ((base) + (0x00B8))
#define SOC_MDC_DMA_TEST1_V_ADDR(base) ((base) + (0x00BC))
#define SOC_MDC_DMA_TEST3_V_ADDR(base) ((base) + (0x00C0))
#define SOC_MDC_DMA_TEST4_V_ADDR(base) ((base) + (0x00C4))
#define SOC_MDC_DMA_STATUS_V_ADDR(base) ((base) + (0x00C8))
#define SOC_MDC_CH_RD_SHADOW_ADDR(base) ((base) + (0x00D0))
#define SOC_MDC_CH_CTL_ADDR(base) ((base) + (0x00D4))
#define SOC_MDC_CH_SECU_EN_ADDR(base) ((base) + (0x00D8))
#define SOC_MDC_CH_SW_END_REQ_ADDR(base) ((base) + (0x00DC))
#define SOC_MDC_CH_CLK_SEL_ADDR(base) ((base) + (0x00E0))
#define SOC_MDC_CH_CLK_EN_ADDR(base) ((base) + (0x00E4))
#define SOC_MDC_CH_DBG0_ADDR(base) ((base) + (0x00E8))
#define SOC_MDC_CH_DBG1_ADDR(base) ((base) + (0x00EC))
#define SOC_MDC_CH_DBG2_ADDR(base) ((base) + (0x00F0))
#define SOC_MDC_CH_DBG3_ADDR(base) ((base) + (0x00F4))
#define SOC_MDC_CH_DBG4_ADDR(base) ((base) + (0x00F8))
#define SOC_MDC_CH_DBG5_ADDR(base) ((base) + (0x00FC))
#define SOC_MDC_BITEXT_CTL_ADDR(base) ((base) + (0x0140))
#define SOC_MDC_BITEXT_REG_INI0_0_ADDR(base) ((base) + (0x0144))
#define SOC_MDC_BITEXT_REG_INI0_1_ADDR(base) ((base) + (0x0148))
#define SOC_MDC_BITEXT_REG_INI0_2_ADDR(base) ((base) + (0x014C))
#define SOC_MDC_BITEXT_REG_INI0_3_ADDR(base) ((base) + (0x0150))
#define SOC_MDC_BITEXT_REG_INI1_0_ADDR(base) ((base) + (0x0154))
#define SOC_MDC_BITEXT_REG_INI1_1_ADDR(base) ((base) + (0x0158))
#define SOC_MDC_BITEXT_REG_INI1_2_ADDR(base) ((base) + (0x015C))
#define SOC_MDC_BITEXT_REG_INI1_3_ADDR(base) ((base) + (0x0160))
#define SOC_MDC_BITEXT_REG_INI2_0_ADDR(base) ((base) + (0x0164))
#define SOC_MDC_BITEXT_REG_INI2_1_ADDR(base) ((base) + (0x0168))
#define SOC_MDC_BITEXT_REG_INI2_2_ADDR(base) ((base) + (0x016C))
#define SOC_MDC_BITEXT_REG_INI2_3_ADDR(base) ((base) + (0x0170))
#define SOC_MDC_BITEXT_POWER_CTRL_C_ADDR(base) ((base) + (0x0174))
#define SOC_MDC_BITEXT_POWER_CTRL_SHIFT_ADDR(base) ((base) + (0x0178))
#define SOC_MDC_BITEXT_FILT_00_ADDR(base) ((base) + (0x017C))
#define SOC_MDC_BITEXT_FILT_01_ADDR(base) ((base) + (0x0180))
#define SOC_MDC_BITEXT_FILT_02_ADDR(base) ((base) + (0x0184))
#define SOC_MDC_BITEXT_FILT_10_ADDR(base) ((base) + (0x0188))
#define SOC_MDC_BITEXT_FILT_11_ADDR(base) ((base) + (0x018C))
#define SOC_MDC_BITEXT_FILT_12_ADDR(base) ((base) + (0x0190))
#define SOC_MDC_BITEXT_FILT_20_ADDR(base) ((base) + (0x0194))
#define SOC_MDC_BITEXT_FILT_21_ADDR(base) ((base) + (0x01A0))
#define SOC_MDC_BITEXT_FILT_22_ADDR(base) ((base) + (0x01A4))
#define SOC_MDC_BITEXT_THD_R0_ADDR(base) ((base) + (0x01A8))
#define SOC_MDC_BITEXT_THD_R1_ADDR(base) ((base) + (0x01AC))
#define SOC_MDC_BITEXT_THD_G0_ADDR(base) ((base) + (0x01B0))
#define SOC_MDC_BITEXT_THD_G1_ADDR(base) ((base) + (0x01B4))
#define SOC_MDC_BITEXT_THD_B0_ADDR(base) ((base) + (0x01B8))
#define SOC_MDC_BITEXT_THD_B1_ADDR(base) ((base) + (0x01BC))
#define SOC_MDC_BITEXT0_DBG0_ADDR(base) ((base) + (0x01C0))
#define SOC_MDC_SCF_EN_HSCL_STR_ADDR(base) ((base) + (0x0200))
#define SOC_MDC_SCF_EN_VSCL_STR_ADDR(base) ((base) + (0x0204))
#define SOC_MDC_SCF_H_V_ORDER_ADDR(base) ((base) + (0x0208))
#define SOC_MDC_SCF_CH_CORE_GT_ADDR(base) ((base) + (0x020C))
#define SOC_MDC_SCF_INPUT_WIDTH_HEIGHT_ADDR(base) ((base) + (0x0210))
#define SOC_MDC_SCF_OUTPUT_WIDTH_HEIGHT_ADDR(base) ((base) + (0x0214))
#define SOC_MDC_SCF_COEF_MEM_CTRL_ADDR(base) ((base) + (0x0218))
#define SOC_MDC_SCF_EN_HSCL_ADDR(base) ((base) + (0x021C))
#define SOC_MDC_SCF_EN_VSCL_ADDR(base) ((base) + (0x0220))
#define SOC_MDC_SCF_ACC_HSCL_ADDR(base) ((base) + (0x0224))
#define SOC_MDC_SCF_ACC_HSCL1_ADDR(base) ((base) + (0x0228))
#define SOC_MDC_SCF_INC_HSCL_ADDR(base) ((base) + (0x0234))
#define SOC_MDC_SCF_ACC_VSCL_ADDR(base) ((base) + (0x0238))
#define SOC_MDC_SCF_ACC_VSCL1_ADDR(base) ((base) + (0x023C))
#define SOC_MDC_SCF_INC_VSCL_ADDR(base) ((base) + (0x0248))
#define SOC_MDC_SCF_EN_NONLINEAR_ADDR(base) ((base) + (0x024C))
#define SOC_MDC_SCF_EN_MMP_ADDR(base) ((base) + (0x027C))
#define SOC_MDC_SCF_DB_H0_ADDR(base) ((base) + (0x0280))
#define SOC_MDC_SCF_DB_H1_ADDR(base) ((base) + (0x0284))
#define SOC_MDC_SCF_DB_V0_ADDR(base) ((base) + (0x0288))
#define SOC_MDC_SCF_DB_V1_ADDR(base) ((base) + (0x028C))
#define SOC_MDC_SCF_LB_MEM_CTRL_ADDR(base) ((base) + (0x0290))
#define SOC_MDC_ARSR_INPUT_WIDTH_HEIGHT_ADDR(base) ((base) + (0x0300))
#define SOC_MDC_ARSR_OUTPUT_WIDTH_HEIGHT_ADDR(base) ((base) + (0x0304))
#define SOC_MDC_IHLEFT_ADDR(base) ((base) + (0x0308))
#define SOC_MDC_IHLEFT1_ADDR(base) ((base) + (0x030C))
#define SOC_MDC_IHRIGHT_ADDR(base) ((base) + (0x0310))
#define SOC_MDC_IHRIGHT1_ADDR(base) ((base) + (0x0314))
#define SOC_MDC_IVTOP_ADDR(base) ((base) + (0x0318))
#define SOC_MDC_IVBOTTOM_ADDR(base) ((base) + (0x031C))
#define SOC_MDC_IVBOTTOM1_ADDR(base) ((base) + (0x0320))
#define SOC_MDC_IHINC_ADDR(base) ((base) + (0x0324))
#define SOC_MDC_IVINC_ADDR(base) ((base) + (0x0328))
#define SOC_MDC_OFFSET_ADDR(base) ((base) + (0x032C))
#define SOC_MDC_MODE_ADDR(base) ((base) + (0x0330))
#define SOC_MDC_SKIN_THRES_Y_ADDR(base) ((base) + (0x0334))
#define SOC_MDC_SKIN_THRES_U_ADDR(base) ((base) + (0x0338))
#define SOC_MDC_SKIN_THRES_V_ADDR(base) ((base) + (0x033C))
#define SOC_MDC_SKIN_CFG0_ADDR(base) ((base) + (0x0340))
#define SOC_MDC_SKIN_CFG1_ADDR(base) ((base) + (0x0344))
#define SOC_MDC_SKIN_CFG2_ADDR(base) ((base) + (0x0348))
#define SOC_MDC_SHOOT_CFG1_ADDR(base) ((base) + (0x034C))
#define SOC_MDC_SHOOT_CFG2_ADDR(base) ((base) + (0x0350))
#define SOC_MDC_SHOOT_CFG3_ADDR(base) ((base) + (0x0354))
#define SOC_MDC_SHARP_CFG3_ADDR(base) ((base) + (0x0358))
#define SOC_MDC_SHARP_CFG4_ADDR(base) ((base) + (0x035C))
#define SOC_MDC_SHARP_CFG5_ADDR(base) ((base) + (0x0360))
#define SOC_MDC_SHARP_CFG6_ADDR(base) ((base) + (0x0364))
#define SOC_MDC_SHARP_CFG7_ADDR(base) ((base) + (0x0368))
#define SOC_MDC_SHARP_CFG8_ADDR(base) ((base) + (0x036C))
#define SOC_MDC_SHARPLEVEL_ADDR(base) ((base) + (0x0370))
#define SOC_MDC_SHPGAIN_LOW_ADDR(base) ((base) + (0x0374))
#define SOC_MDC_SHPGAIN_MID_ADDR(base) ((base) + (0x0378))
#define SOC_MDC_SHPGAIN_HIGH_ADDR(base) ((base) + (0x037C))
#define SOC_MDC_GAINCTRLSLOPH_MF_ADDR(base) ((base) + (0x0380))
#define SOC_MDC_GAINCTRLSLOPL_MF_ADDR(base) ((base) + (0x0384))
#define SOC_MDC_GAINCTRLSLOPH_HF_ADDR(base) ((base) + (0x0388))
#define SOC_MDC_GAINCTRLSLOPL_HF_ADDR(base) ((base) + (0x038C))
#define SOC_MDC_MF_LMT_ADDR(base) ((base) + (0x0390))
#define SOC_MDC_GAIN_MF_ADDR(base) ((base) + (0x0394))
#define SOC_MDC_MF_B_ADDR(base) ((base) + (0x0398))
#define SOC_MDC_HF_LMT_ADDR(base) ((base) + (0x039C))
#define SOC_MDC_GAIN_HF_ADDR(base) ((base) + (0x03A0))
#define SOC_MDC_HF_B_ADDR(base) ((base) + (0x03A4))
#define SOC_MDC_LF_CTRL_ADDR(base) ((base) + (0x03A8))
#define SOC_MDC_LF_VAR_ADDR(base) ((base) + (0x03AC))
#define SOC_MDC_LF_CTRL_SLOP_ADDR(base) ((base) + (0x03B0))
#define SOC_MDC_HF_SELECT_ADDR(base) ((base) + (0x03B4))
#define SOC_MDC_SHARP_CFG2_H_ADDR(base) ((base) + (0x03B8))
#define SOC_MDC_SHARP_CFG2_L_ADDR(base) ((base) + (0x03BC))
#define SOC_MDC_TEXTURW_ANALYSTS_ADDR(base) ((base) + (0x03D0))
#define SOC_MDC_INTPLSHOOTCTRL_ADDR(base) ((base) + (0x03D4))
#define SOC_MDC_ARSR2P_DEBUG0_ADDR(base) ((base) + (0x03D8))
#define SOC_MDC_ARSR2P_DEBUG1_ADDR(base) ((base) + (0x03DC))
#define SOC_MDC_ARSR2P_DEBUG2_ADDR(base) ((base) + (0x03E0))
#define SOC_MDC_ARSR2P_DEBUG3_ADDR(base) ((base) + (0x03E4))
#define SOC_MDC_ARSR2P_LB_MEM_CTRL_ADDR(base) ((base) + (0x03E8))
#define SOC_MDC_PCSC_IDC0_ADDR(base) ((base) + (0x0400))
#define SOC_MDC_PCSC_IDC2_ADDR(base) ((base) + (0x0404))
#define SOC_MDC_PCSC_ODC0_ADDR(base) ((base) + (0x0408))
#define SOC_MDC_PCSC_ODC2_ADDR(base) ((base) + (0x040C))
#define SOC_MDC_PCSC_P00_ADDR(base) ((base) + (0x0410))
#define SOC_MDC_PCSC_P01_ADDR(base) ((base) + (0x0414))
#define SOC_MDC_PCSC_P02_ADDR(base) ((base) + (0x0418))
#define SOC_MDC_PCSC_P10_ADDR(base) ((base) + (0x041C))
#define SOC_MDC_PCSC_P11_ADDR(base) ((base) + (0x0420))
#define SOC_MDC_PCSC_P12_ADDR(base) ((base) + (0x0424))
#define SOC_MDC_PCSC_P20_ADDR(base) ((base) + (0x0428))
#define SOC_MDC_PCSC_P21_ADDR(base) ((base) + (0x042C))
#define SOC_MDC_PCSC_P22_ADDR(base) ((base) + (0x0430))
#define SOC_MDC_PCSC_ICG_MODULE_ADDR(base) ((base) + (0x0434))
#define SOC_MDC_PCSC_MPREC_ADDR(base) ((base) + (0x0438))
#define SOC_MDC_POST_CLIP_DISP_SIZE_ADDR(base) ((base) + (0x0480))
#define SOC_MDC_POST_CLIP_CTL_HRZ_ADDR(base) ((base) + (0x0484))
#define SOC_MDC_POST_CLIP_CTL_VRZ_ADDR(base) ((base) + (0x0488))
#define SOC_MDC_POST_CLIP_EN_ADDR(base) ((base) + (0x048C))
#define SOC_MDC_CSC_IDC0_ADDR(base) ((base) + (0x0500))
#define SOC_MDC_CSC_IDC2_ADDR(base) ((base) + (0x0504))
#define SOC_MDC_CSC_ODC0_ADDR(base) ((base) + (0x0508))
#define SOC_MDC_CSC_ODC2_ADDR(base) ((base) + (0x050C))
#define SOC_MDC_CSC_P00_ADDR(base) ((base) + (0x0510))
#define SOC_MDC_CSC_P01_ADDR(base) ((base) + (0x0514))
#define SOC_MDC_CSC_P02_ADDR(base) ((base) + (0x0518))
#define SOC_MDC_CSC_P10_ADDR(base) ((base) + (0x051C))
#define SOC_MDC_CSC_P11_ADDR(base) ((base) + (0x0520))
#define SOC_MDC_CSC_P12_ADDR(base) ((base) + (0x0524))
#define SOC_MDC_CSC_P20_ADDR(base) ((base) + (0x0528))
#define SOC_MDC_CSC_P21_ADDR(base) ((base) + (0x052C))
#define SOC_MDC_CSC_P22_ADDR(base) ((base) + (0x0530))
#define SOC_MDC_CSC_ICG_MODULE_ADDR(base) ((base) + (0x0534))
#define SOC_MDC_CSC_MPREC_ADDR(base) ((base) + (0x0538))
#define SOC_MDC_CH_DEBUG_SEL_ADDR(base) ((base) + (0x600))
#define SOC_MDC_VPP_CTRL_ADDR(base) ((base) + (0x0700))
#define SOC_MDC_VPP_MEM_CTRL_ADDR(base) ((base) + (0x0704))
#define SOC_MDC_DMA_BUF_CTRL_ADDR(base) ((base) + (0x0800))
#define SOC_MDC_DMA_BUF_CPU_CTL_ADDR(base) ((base) + (0x0804))
#define SOC_MDC_DMA_BUF_CPU_START_ADDR(base) ((base) + (0x0808))
#define SOC_MDC_DMA_BUF_CPU_ADDR_ADDR(base) ((base) + (0x080C))
#define SOC_MDC_DMA_BUF_CPU_RDATA0_ADDR(base) ((base) + (0x0810))
#define SOC_MDC_DMA_BUF_CPU_RDATA1_ADDR(base) ((base) + (0x814))
#define SOC_MDC_DMA_BUF_CPU_RDATA2_ADDR(base) ((base) + (0x818))
#define SOC_MDC_DMA_BUF_CPU_RDATA3_ADDR(base) ((base) + (0x81C))
#define SOC_MDC_DMA_BUF_CPU_WDATA0_ADDR(base) ((base) + (0x820))
#define SOC_MDC_DMA_BUF_CPU_WDATA1_ADDR(base) ((base) + (0x824))
#define SOC_MDC_DMA_BUF_CPU_WDATA2_ADDR(base) ((base) + (0x828))
#define SOC_MDC_DMA_BUF_CPU_WDATA3_ADDR(base) ((base) + (0x82C))
#define SOC_MDC_DMA_REQ_END_ADDR(base) ((base) + (0x830))
#define SOC_MDC_DMA_BUF_DBGCFG_ADDR(base) ((base) + (0x834))
#define SOC_MDC_DMA_BUF_DBG0_ADDR(base) ((base) + (0x838))
#define SOC_MDC_DMA_BUF_DBG1_ADDR(base) ((base) + (0x83C))
#define SOC_MDC_DMA_BUF_DBG2_ADDR(base) ((base) + (0x840))
#define SOC_MDC_DMA_BUF_DBG3_ADDR(base) ((base) + (0x844))
#define SOC_MDC_DMA_BUF_DBG4_ADDR(base) ((base) + (0x848))
#define SOC_MDC_DMA_BUF_DBG5_ADDR(base) ((base) + (0x84C))
#define SOC_MDC_DMA_BUF_SIZE_ADDR(base) ((base) + (0x850))
#define SOC_MDC_DMA_BUF_MEM_CTRL_ADDR(base) ((base) + (0x0854))
#define SOC_MDC_AFBCD_HREG_HDR_PTR_LO_ADDR(base) ((base) + (0x0900))
#define SOC_MDC_AFBCD_HREG_PIC_WIDTH_ADDR(base) ((base) + (0x0904))
#define SOC_MDC_AFBCD_HREG_PIC_HEIGHT_ADDR(base) ((base) + (0x090C))
#define SOC_MDC_AFBCD_HREG_FORMAT_ADDR(base) ((base) + (0x0910))
#define SOC_MDC_AFBCD_CTL_ADDR(base) ((base) + (0x0914))
#define SOC_MDC_AFBCD_STR_ADDR(base) ((base) + (0x0918))
#define SOC_MDC_LINE_CROP_ADDR(base) ((base) + (0x091C))
#define SOC_MDC_INPUT_HEADER_STRIDE_ADDR(base) ((base) + (0x0920))
#define SOC_MDC_AFBCD_PAYLOAD_STRIDE_ADDR(base) ((base) + (0x0924))
#define SOC_MDC_MM_BASE_ADDR(base) ((base) + (0x0928))
#define SOC_MDC_AFBCD_PAYLOAD_POINTER_ADDR(base) ((base) + (0x0930))
#define SOC_MDC_HEIGHT_BF_STR_ADDR(base) ((base) + (0x0934))
#define SOC_MDC_OS_CFG_ADDR(base) ((base) + (0x0938))
#define SOC_MDC_AFBCD_MEM_CTRL_ADDR(base) ((base) + (0x093C))
#define SOC_MDC_AFBCD_SCRAMBLE_MODE_ADDR(base) ((base) + (0x0940))
#define SOC_MDC_AFBCD_HEADER_POINTER_OFFSET_ADDR(base) ((base) + (0x0944))
#define SOC_MDC_AFBCD_MONITOR_REG1_ADDR(base) ((base) + (0x0948))
#define SOC_MDC_AFBCD_MONITOR_REG2_ADDR(base) ((base) + (0x094C))
#define SOC_MDC_AFBCD_MONITOR_REG3_ADDR(base) ((base) + (0x0950))
#define SOC_MDC_AFBCD_DEBUG_REG0_ADDR(base) ((base) + (0x0954))
#define SOC_MDC_FBCD_CREG_FBCD_CTRL_MODE_ADDR(base) ((base) + (0x0960))
#define SOC_MDC_AFBCD_HREG_HDR_PTR_LO_1_ADDR(base) ((base) + (0x0964))
#define SOC_MDC_AFBCD_HREG_PLD_PTR_LO_1_ADDR(base) ((base) + (0x0968))
#define SOC_MDC_FBCD_HEADER_SRTIDE_1_ADDR(base) ((base) + (0x096C))
#define SOC_MDC_FBCD_PAYLOAD_STRIDE_1_ADDR(base) ((base) + (0x0970))
#define SOC_MDC_FBCD_BLOCK_TYPE_ADDR(base) ((base) + (0x0974))
#define SOC_MDC_MM_BASE_1_ADDR(base) ((base) + (0x0978))
#define SOC_MDC_MM_BASE_2_ADDR(base) ((base) + (0x097C))
#define SOC_MDC_MM_BASE_3_ADDR(base) ((base) + (0x0980))
#define SOC_MDC_HFBCD_MEM_CTRL_ADDR(base) ((base) + (0x0984))
#define SOC_MDC_HFBCD_MEM_CTRL_1_ADDR(base) ((base) + (0x0988))
#define SOC_MDC_FBCD_CREG_FBCD_CLK_GATE_ADDR(base) ((base) + (0x098C))
#define SOC_MDC_HFBCD_MONTIOR_REG1_ADDR(base) ((base) + (0x0990))
#define SOC_MDC_HFBCD_MONTIOR_REG2_ADDR(base) ((base) + (0x0994))
#define SOC_MDC_HFBCD_MONTIOR_REG3_ADDR(base) ((base) + (0x0998))
#define SOC_MDC_HFBCD_DEBUG_REG0_ADDR(base) ((base) + (0x099C))
#define SOC_MDC_AFBCD_MONTIOR_REG4_ADDR(base) ((base) + (0x09A0))
#define SOC_MDC_AFBCD_MONTIOR_REG5_ADDR(base) ((base) + (0x09A4))
#define SOC_MDC_HFBCD_MONTIOR_REG4_ADDR(base) ((base) + (0x09A8))
#define SOC_MDC_HFBCD_MONTIOR_REG5_ADDR(base) ((base) + (0x09AC))
#define SOC_MDC_HEBCD_OS_CFG_ADDR(base) ((base) + (0x09B0))
#define SOC_MDC_HEBCD_TAG_VALUE0_ADDR(base) ((base) + (0x09C0))
#define SOC_MDC_HEBCD_TAG_VALUE1_ADDR(base) ((base) + (0x09C4))
#define SOC_MDC_HEBCD_TAG_VALUE2_ADDR(base) ((base) + (0x09C8))
#define SOC_MDC_HEBCD_TAG_VALUE3_ADDR(base) ((base) + (0x09CC))
#define SOC_MDC_HEBCD_RESERVED0_ADDR(base) ((base) + (0x09D0))
#define SOC_MDC_HEBCD_RESERVED1_ADDR(base) ((base) + (0x09D4))
#define SOC_MDC_HEBCD_RESERVED2_ADDR(base) ((base) + (0x09D8))
#define SOC_MDC_HEBCD_RESERVED3_ADDR(base) ((base) + (0x09DC))
#define SOC_MDC_HEBCD_MONITOR_REG0_ADDR(base) ((base) + (0x09E0))
#define SOC_MDC_HEBCD_MONITOR_REG1_ADDR(base) ((base) + (0x09E4))
#define SOC_MDC_HEBCD_MONITOR_REG2_ADDR(base) ((base) + (0x09E8))
#define SOC_MDC_HEBCD_MONITOR_REG3_ADDR(base) ((base) + (0x09EC))
#define SOC_MDC_HEBCD_MEM_CTRL_ADDR(base) ((base) + (0x09F0))
#define SOC_MDC_HEBCD_CLK_GATE_ADDR(base) ((base) + (0x09F4))
#define SOC_MDC_REG_DEFAULT_ADDR(base) ((base) + (0x0A00))
#define SOC_MDC_DMA_ADDR_EXT_ADDR(base) ((base) + (0x0A40))
#define SOC_MDC_V0_SCF_VIDEO_6TAP_COEF_ADDR(base,l) ((base) + (0x1000+0x4*(l)))
#define SOC_MDC_V0_SCF_VIDEO_5TAP_COEF_ADDR(base,m) ((base) + (0x3000+0x4*(m)))
#define SOC_MDC_V0_SCF_VIDEO_4TAP_COEF_ADDR(base,n) ((base) + (0x3800+0x4*(n)))
#define SOC_MDC_COEFY_V_ADDR(base,o) ((base) + (0x5000+0x4*(o)))
#define SOC_MDC_COEFY_H_ADDR(base,p) ((base) + (0x5100+0x4*(p)))
#define SOC_MDC_COEFA_V_ADDR(base,o) ((base) + (0x5300+0x4*(o)))
#define SOC_MDC_COEFA_H_ADDR(base,p) ((base) + (0x5400+0x4*(p)))
#define SOC_MDC_COEFUV_V_ADDR(base,o) ((base) + (0x5600+0x4*(o)))
#define SOC_MDC_COEFUV_H_ADDR(base,p) ((base) + (0x5700+0x4*(p)))
#endif
#ifndef __SOC_H_FOR_ASM__
#define SOC_MDC_CLD_EN_ADDR(base) ((base) + (0x0B00UL))
#define SOC_MDC_CLD_SIZE_VRT_ADDR(base) ((base) + (0x0B04UL))
#define SOC_MDC_CLD_SIZE_HRZ_ADDR(base) ((base) + (0x0B08UL))
#define SOC_MDC_CLD_RGB_ADDR(base) ((base) + (0x0B0CUL))
#else
#define SOC_MDC_CLD_EN_ADDR(base) ((base) + (0x0B00))
#define SOC_MDC_CLD_SIZE_VRT_ADDR(base) ((base) + (0x0B04))
#define SOC_MDC_CLD_SIZE_HRZ_ADDR(base) ((base) + (0x0B08))
#define SOC_MDC_CLD_RGB_ADDR(base) ((base) + (0x0B0C))
#endif
#ifndef __SOC_H_FOR_ASM__
#define SOC_MDC_DMA_FIFO_CLR_ADDR(base) ((base) + (0x0034UL))
#define SOC_MDC_DMA_COUNTER_DGB_ADDR(base) ((base) + (0x003CUL))
#define SOC_MDC_DMA_ROT_BURST4_ADDR(base) ((base) + (0x0040UL))
#define SOC_MDC_DMA_RSV1_ADDR(base) ((base) + (0x0044UL))
#define SOC_MDC_DMA_RSV2_ADDR(base) ((base) + (0x0048UL))
#define SOC_MDC_DMA_SW_MASK_EN_ADDR(base) ((base) + (0x004CUL))
#define SOC_MDC_DMA_START_MASK0_ADDR(base) ((base) + (0x0050UL))
#define SOC_MDC_DMA_END_MASK0_ADDR(base) ((base) + (0x0054UL))
#define SOC_MDC_DMA_START_MASK1_ADDR(base) ((base) + (0x0058UL))
#define SOC_MDC_DMA_END_MASK1_ADDR(base) ((base) + (0x005CUL))
#define SOC_MDC_WCH_CLK_SEL_ADDR(base) ((base) + (0x00E0UL))
#define SOC_MDC_WCH_CLK_EN_ADDR(base) ((base) + (0x00E4UL))
#define SOC_MDC_DFC_DISP_SIZE_ADDR(base) ((base) + (0x0100UL))
#define SOC_MDC_DFC_PIX_IN_NUM_ADDR(base) ((base) + (0x0104UL))
#define SOC_MDC_DFC_GLB_ALPHA01_ADDR(base) ((base) + (0x0108UL))
#define SOC_MDC_DFC_DISP_FMT_ADDR(base) ((base) + (0x010CUL))
#define SOC_MDC_DFC_CLIP_CTL_HRZ_ADDR(base) ((base) + (0x0110UL))
#define SOC_MDC_DFC_CLIP_CTL_VRZ_ADDR(base) ((base) + (0x0114UL))
#define SOC_MDC_DFC_CTL_CLIP_EN_ADDR(base) ((base) + (0x0118UL))
#define SOC_MDC_DFC_ICG_MODULE_ADDR(base) ((base) + (0x011CUL))
#define SOC_MDC_DFC_DITHER_ENABLE_ADDR(base) ((base) + (0x0120UL))
#define SOC_MDC_DFC_PADDING_CTL_ADDR(base) ((base) + (0x0124UL))
#define SOC_MDC_DFC_GLB_ALPHA23_ADDR(base) ((base) + (0x0128UL))
#define SOC_MDC_DFC_GLB_ALPHA_ADDR(base) ((base) + (0x012CUL))
#define SOC_MDC_DFC_ALPHA_CTL_ADDR(base) ((base) + (0x0130UL))
#define SOC_MDC_DFC_ALPHA_THD_ADDR(base) ((base) + (0x0134UL))
#define SOC_MDC_DITHER_CTL1_ADDR(base) ((base) + (0x01D0UL))
#define SOC_MDC_DITHER_TRI_THD10_ADDR(base) ((base) + (0x01DCUL))
#define SOC_MDC_DITHER_TRI_THD10_UNI_ADDR(base) ((base) + (0x01E8UL))
#define SOC_MDC_BAYER_CTL_ADDR(base) ((base) + (0x01ECUL))
#define SOC_MDC_BAYER_MATRIX_PART1_ADDR(base) ((base) + (0x01F4UL))
#define SOC_MDC_BAYER_MATRIX_PART0_ADDR(base) ((base) + (0x01F8UL))
#define SOC_MDC_ROT_FIRST_LNS_ADDR(base) ((base) + (0x0580UL))
#define SOC_MDC_ROT_STATE_ADDR(base) ((base) + (0x0584UL))
#define SOC_MDC_ROT_MEM_CTRL_ADDR(base) ((base) + (0x0588UL))
#define SOC_MDC_ROT_SIZE_ADDR(base) ((base) + (0x058CUL))
#define SOC_MDC_ROT_422_MODE_ADDR(base) ((base) + (0x0590UL))
#define SOC_MDC_ROT_CPU_START0_ADDR(base) ((base) + (0x0594UL))
#define SOC_MDC_ROT_CPU_ADDR0_ADDR(base) ((base) + (0x0598UL))
#define SOC_MDC_ROT_CPU_RDATA0_ADDR(base) ((base) + (0x059CUL))
#define SOC_MDC_ROT_CPU_RDATA1_ADDR(base) ((base) + (0x05A0UL))
#define SOC_MDC_ROT_CPU_WDATA0_ADDR(base) ((base) + (0x05A4UL))
#define SOC_MDC_ROT_CPU_WDATA1_ADDR(base) ((base) + (0x05A8UL))
#define SOC_MDC_ROT_CPU_CTL1_ADDR(base) ((base) + (0x05ACUL))
#define SOC_MDC_ROT_CPU_START1_ADDR(base) ((base) + (0x05B0UL))
#define SOC_MDC_ROT_CPU_ADDR1_ADDR(base) ((base) + (0x05B4UL))
#define SOC_MDC_ROT_CPU_RDATA2_ADDR(base) ((base) + (0x05B8UL))
#define SOC_MDC_ROT_CPU_RDATA3_ADDR(base) ((base) + (0x05BCUL))
#define SOC_MDC_ROT_CPU_WDATA2_ADDR(base) ((base) + (0x05C0UL))
#define SOC_MDC_ROT_CPU_WDATA3_ADDR(base) ((base) + (0x05C4UL))
#define SOC_MDC_AFBCE_HREG_PIC_BLKS_ADDR(base) ((base) + (0x0900UL))
#define SOC_MDC_AFBCE_HREG_FORMAT_ADDR(base) ((base) + (0x0904UL))
#define SOC_MDC_AFBCE_HREG_HDR_PTR_L0_ADDR(base) ((base) + (0x0908UL))
#define SOC_MDC_AFBCE_HREG_PLD_PTR_L0_ADDR(base) ((base) + (0x090CUL))
#define SOC_MDC_PICTURE_SIZE_ADDR(base) ((base) + (0x0910UL))
#define SOC_MDC_HEADER_SRTIDE_0_ADDR(base) ((base) + (0x0918UL))
#define SOC_MDC_PAYLOAD_STRIDE_0_ADDR(base) ((base) + (0x091CUL))
#define SOC_MDC_ENC_OS_CFG_ADDR(base) ((base) + (0x0920UL))
#define SOC_MDC_AFBCE_MEM_CTRL_ADDR(base) ((base) + (0x0924UL))
#define SOC_MDC_AFBCE_QOS_CFG_ADDR(base) ((base) + (0x0928UL))
#define SOC_MDC_FBCE_THRESHOLD_ADDR(base) ((base) + (0x092CUL))
#define SOC_MDC_FBCE_SCRAMBLE_MODE_ADDR(base) ((base) + (0x0930UL))
#define SOC_MDC_AFBCE_HEADER_POINTER_OFFSET_ADDR(base) ((base) + (0x0934UL))
#define SOC_MDC_AFBCE_MONITOR_REG1_ADDR(base) ((base) + (0x0938UL))
#define SOC_MDC_AFBCE_MONITOR_REG2_ADDR(base) ((base) + (0x093CUL))
#define SOC_MDC_AFBCE_MONITOR_REG3_ADDR(base) ((base) + (0x0940UL))
#define SOC_MDC_AFBCE_DEBUG_REG0_ADDR(base) ((base) + (0x0944UL))
#define SOC_MDC_FBCE_CREG_FBCE_CTRL_MODE_ADDR(base) ((base) + (0x0950UL))
#define SOC_MDC_AFBCE_HREG_HDR_PTR_L1_ADDR(base) ((base) + (0x0954UL))
#define SOC_MDC_AFBCE_HREG_PLD_PTR_L1_ADDR(base) ((base) + (0x0958UL))
#define SOC_MDC_FBCE_HEADER_SRTIDE_1_ADDR(base) ((base) + (0x095CUL))
#define SOC_MDC_FBCE_PAYLOAD_STRIDE_1_ADDR(base) ((base) + (0x0960UL))
#define SOC_MDC_FBCE_CREG_FBCE_CLK_GATE_ADDR(base) ((base) + (0x0964UL))
#define SOC_MDC_AFBCE_MEM_CTRL_1_ADDR(base) ((base) + (0x0968UL))
#define SOC_MDC_AFBCE_MONITOR_REG4_ADDR(base) ((base) + (0x096CUL))
#define SOC_MDC_AFBCE_MONITOR_REG5_ADDR(base) ((base) + (0x0970UL))
#define SOC_MDC_AFBCE_MONITOR_REG6_ADDR(base) ((base) + (0x0974UL))
#define SOC_MDC_HEBCE_SPBLOCK_MODE_ADDR(base) ((base) + (0x0978UL))
#define SOC_MDC_HEBCE_TAG_VALUE0_ADDR(base) ((base) + (0x097CUL))
#define SOC_MDC_HEBCE_TAG_VALUE1_ADDR(base) ((base) + (0x0980UL))
#define SOC_MDC_HEBCE_TAG_VALUE2_ADDR(base) ((base) + (0x0984UL))
#define SOC_MDC_HEBCE_TAG_VALUE3_ADDR(base) ((base) + (0x0988UL))
#define SOC_MDC_HEBCE_RESERVED0_ADDR(base) ((base) + (0x098CUL))
#define SOC_MDC_HEBCE_RESERVED1_ADDR(base) ((base) + (0x0990UL))
#define SOC_MDC_HEBCE_RESERVED2_ADDR(base) ((base) + (0x0994UL))
#define SOC_MDC_HEBCE_RESERVED3_ADDR(base) ((base) + (0x0998UL))
#define SOC_MDC_HEBCE_MONITOR_REG0_ADDR(base) ((base) + (0x099CUL))
#define SOC_MDC_HEBCE_MONITOR_REG1_ADDR(base) ((base) + (0x09A0UL))
#define SOC_MDC_HEBCE_MONITOR_REG2_ADDR(base) ((base) + (0x09A4UL))
#define SOC_MDC_HEBCE_MONITOR_REG3_ADDR(base) ((base) + (0x09A8UL))
#define SOC_MDC_HEBCE_MEM_CTRL_ADDR(base) ((base) + (0x09ACUL))
#define SOC_MDC_WCH_SCF_VIDEO_6TAP_COEF_ADDR(base,l) ((base) + (0x1000+0x4*(l)))
#define SOC_MDC_WCH_SCF_VIDEO_5TAP_COEF_ADDR(base,m) ((base) + (0x3000+0x4*(m)))
#define SOC_MDC_WCH_SCF_VIDEO_4TAP_COEF_ADDR(base,n) ((base) + (0x3800+0x4*(n)))
#define SOC_MDC_GMP_EN_ADDR(base) ((base) + (0x0B00UL))
#define SOC_MDC_GMP_MEM_CTRL_ADDR(base) ((base) + (0x0B04UL))
#define SOC_MDC_GMP_LUT_SEL_ADDR(base) ((base) + (0x0B08UL))
#define SOC_MDC_GMP_DBG_W0_ADDR(base) ((base) + (0x0B10UL))
#define SOC_MDC_GMP_DBG_R0_ADDR(base) ((base) + (0x0B14UL))
#define SOC_MDC_GMP_DBG_R1_ADDR(base) ((base) + (0x0B18UL))
#define SOC_MDC_GMP_DBG_R2_ADDR(base) ((base) + (0x0B1CUL))
#define SOC_MDC_GMP_DBG_R3_ADDR(base) ((base) + (0x0B20UL))
#else
#define SOC_MDC_DMA_FIFO_CLR_ADDR(base) ((base) + (0x0034))
#define SOC_MDC_DMA_COUNTER_DGB_ADDR(base) ((base) + (0x003C))
#define SOC_MDC_DMA_ROT_BURST4_ADDR(base) ((base) + (0x0040))
#define SOC_MDC_DMA_RSV1_ADDR(base) ((base) + (0x0044))
#define SOC_MDC_DMA_RSV2_ADDR(base) ((base) + (0x0048))
#define SOC_MDC_DMA_SW_MASK_EN_ADDR(base) ((base) + (0x004C))
#define SOC_MDC_DMA_START_MASK0_ADDR(base) ((base) + (0x0050))
#define SOC_MDC_DMA_END_MASK0_ADDR(base) ((base) + (0x0054))
#define SOC_MDC_DMA_START_MASK1_ADDR(base) ((base) + (0x0058))
#define SOC_MDC_DMA_END_MASK1_ADDR(base) ((base) + (0x005C))
#define SOC_MDC_WCH_CLK_SEL_ADDR(base) ((base) + (0x00E0))
#define SOC_MDC_WCH_CLK_EN_ADDR(base) ((base) + (0x00E4))
#define SOC_MDC_DFC_DISP_SIZE_ADDR(base) ((base) + (0x0100))
#define SOC_MDC_DFC_PIX_IN_NUM_ADDR(base) ((base) + (0x0104))
#define SOC_MDC_DFC_GLB_ALPHA01_ADDR(base) ((base) + (0x0108))
#define SOC_MDC_DFC_DISP_FMT_ADDR(base) ((base) + (0x010C))
#define SOC_MDC_DFC_CLIP_CTL_HRZ_ADDR(base) ((base) + (0x0110))
#define SOC_MDC_DFC_CLIP_CTL_VRZ_ADDR(base) ((base) + (0x0114))
#define SOC_MDC_DFC_CTL_CLIP_EN_ADDR(base) ((base) + (0x0118))
#define SOC_MDC_DFC_ICG_MODULE_ADDR(base) ((base) + (0x011C))
#define SOC_MDC_DFC_DITHER_ENABLE_ADDR(base) ((base) + (0x0120))
#define SOC_MDC_DFC_PADDING_CTL_ADDR(base) ((base) + (0x0124))
#define SOC_MDC_DFC_GLB_ALPHA23_ADDR(base) ((base) + (0x0128))
#define SOC_MDC_DFC_GLB_ALPHA_ADDR(base) ((base) + (0x012C))
#define SOC_MDC_DFC_ALPHA_CTL_ADDR(base) ((base) + (0x0130))
#define SOC_MDC_DFC_ALPHA_THD_ADDR(base) ((base) + (0x0134))
#define SOC_MDC_DITHER_CTL1_ADDR(base) ((base) + (0x01D0))
#define SOC_MDC_DITHER_TRI_THD10_ADDR(base) ((base) + (0x01DC))
#define SOC_MDC_DITHER_TRI_THD10_UNI_ADDR(base) ((base) + (0x01E8))
#define SOC_MDC_BAYER_CTL_ADDR(base) ((base) + (0x01EC))
#define SOC_MDC_BAYER_MATRIX_PART1_ADDR(base) ((base) + (0x01F4))
#define SOC_MDC_BAYER_MATRIX_PART0_ADDR(base) ((base) + (0x01F8))
#define SOC_MDC_ROT_FIRST_LNS_ADDR(base) ((base) + (0x0580))
#define SOC_MDC_ROT_STATE_ADDR(base) ((base) + (0x0584))
#define SOC_MDC_ROT_MEM_CTRL_ADDR(base) ((base) + (0x0588))
#define SOC_MDC_ROT_SIZE_ADDR(base) ((base) + (0x058C))
#define SOC_MDC_ROT_422_MODE_ADDR(base) ((base) + (0x0590))
#define SOC_MDC_ROT_CPU_START0_ADDR(base) ((base) + (0x0594))
#define SOC_MDC_ROT_CPU_ADDR0_ADDR(base) ((base) + (0x0598))
#define SOC_MDC_ROT_CPU_RDATA0_ADDR(base) ((base) + (0x059C))
#define SOC_MDC_ROT_CPU_RDATA1_ADDR(base) ((base) + (0x05A0))
#define SOC_MDC_ROT_CPU_WDATA0_ADDR(base) ((base) + (0x05A4))
#define SOC_MDC_ROT_CPU_WDATA1_ADDR(base) ((base) + (0x05A8))
#define SOC_MDC_ROT_CPU_CTL1_ADDR(base) ((base) + (0x05AC))
#define SOC_MDC_ROT_CPU_START1_ADDR(base) ((base) + (0x05B0))
#define SOC_MDC_ROT_CPU_ADDR1_ADDR(base) ((base) + (0x05B4))
#define SOC_MDC_ROT_CPU_RDATA2_ADDR(base) ((base) + (0x05B8))
#define SOC_MDC_ROT_CPU_RDATA3_ADDR(base) ((base) + (0x05BC))
#define SOC_MDC_ROT_CPU_WDATA2_ADDR(base) ((base) + (0x05C0))
#define SOC_MDC_ROT_CPU_WDATA3_ADDR(base) ((base) + (0x05C4))
#define SOC_MDC_AFBCE_HREG_PIC_BLKS_ADDR(base) ((base) + (0x0900))
#define SOC_MDC_AFBCE_HREG_FORMAT_ADDR(base) ((base) + (0x0904))
#define SOC_MDC_AFBCE_HREG_HDR_PTR_L0_ADDR(base) ((base) + (0x0908))
#define SOC_MDC_AFBCE_HREG_PLD_PTR_L0_ADDR(base) ((base) + (0x090C))
#define SOC_MDC_PICTURE_SIZE_ADDR(base) ((base) + (0x0910))
#define SOC_MDC_HEADER_SRTIDE_0_ADDR(base) ((base) + (0x0918))
#define SOC_MDC_PAYLOAD_STRIDE_0_ADDR(base) ((base) + (0x091C))
#define SOC_MDC_ENC_OS_CFG_ADDR(base) ((base) + (0x0920))
#define SOC_MDC_AFBCE_MEM_CTRL_ADDR(base) ((base) + (0x0924))
#define SOC_MDC_AFBCE_QOS_CFG_ADDR(base) ((base) + (0x0928))
#define SOC_MDC_FBCE_THRESHOLD_ADDR(base) ((base) + (0x092C))
#define SOC_MDC_FBCE_SCRAMBLE_MODE_ADDR(base) ((base) + (0x0930))
#define SOC_MDC_AFBCE_HEADER_POINTER_OFFSET_ADDR(base) ((base) + (0x0934))
#define SOC_MDC_AFBCE_MONITOR_REG1_ADDR(base) ((base) + (0x0938))
#define SOC_MDC_AFBCE_MONITOR_REG2_ADDR(base) ((base) + (0x093C))
#define SOC_MDC_AFBCE_MONITOR_REG3_ADDR(base) ((base) + (0x0940))
#define SOC_MDC_AFBCE_DEBUG_REG0_ADDR(base) ((base) + (0x0944))
#define SOC_MDC_FBCE_CREG_FBCE_CTRL_MODE_ADDR(base) ((base) + (0x0950))
#define SOC_MDC_AFBCE_HREG_HDR_PTR_L1_ADDR(base) ((base) + (0x0954))
#define SOC_MDC_AFBCE_HREG_PLD_PTR_L1_ADDR(base) ((base) + (0x0958))
#define SOC_MDC_FBCE_HEADER_SRTIDE_1_ADDR(base) ((base) + (0x095C))
#define SOC_MDC_FBCE_PAYLOAD_STRIDE_1_ADDR(base) ((base) + (0x0960))
#define SOC_MDC_FBCE_CREG_FBCE_CLK_GATE_ADDR(base) ((base) + (0x0964))
#define SOC_MDC_AFBCE_MEM_CTRL_1_ADDR(base) ((base) + (0x0968))
#define SOC_MDC_AFBCE_MONITOR_REG4_ADDR(base) ((base) + (0x096C))
#define SOC_MDC_AFBCE_MONITOR_REG5_ADDR(base) ((base) + (0x0970))
#define SOC_MDC_AFBCE_MONITOR_REG6_ADDR(base) ((base) + (0x0974))
#define SOC_MDC_HEBCE_SPBLOCK_MODE_ADDR(base) ((base) + (0x0978))
#define SOC_MDC_HEBCE_TAG_VALUE0_ADDR(base) ((base) + (0x097C))
#define SOC_MDC_HEBCE_TAG_VALUE1_ADDR(base) ((base) + (0x0980))
#define SOC_MDC_HEBCE_TAG_VALUE2_ADDR(base) ((base) + (0x0984))
#define SOC_MDC_HEBCE_TAG_VALUE3_ADDR(base) ((base) + (0x0988))
#define SOC_MDC_HEBCE_RESERVED0_ADDR(base) ((base) + (0x098C))
#define SOC_MDC_HEBCE_RESERVED1_ADDR(base) ((base) + (0x0990))
#define SOC_MDC_HEBCE_RESERVED2_ADDR(base) ((base) + (0x0994))
#define SOC_MDC_HEBCE_RESERVED3_ADDR(base) ((base) + (0x0998))
#define SOC_MDC_HEBCE_MONITOR_REG0_ADDR(base) ((base) + (0x099C))
#define SOC_MDC_HEBCE_MONITOR_REG1_ADDR(base) ((base) + (0x09A0))
#define SOC_MDC_HEBCE_MONITOR_REG2_ADDR(base) ((base) + (0x09A4))
#define SOC_MDC_HEBCE_MONITOR_REG3_ADDR(base) ((base) + (0x09A8))
#define SOC_MDC_HEBCE_MEM_CTRL_ADDR(base) ((base) + (0x09AC))
#define SOC_MDC_WCH_SCF_VIDEO_6TAP_COEF_ADDR(base,l) ((base) + (0x1000+0x4*(l)))
#define SOC_MDC_WCH_SCF_VIDEO_5TAP_COEF_ADDR(base,m) ((base) + (0x3000+0x4*(m)))
#define SOC_MDC_WCH_SCF_VIDEO_4TAP_COEF_ADDR(base,n) ((base) + (0x3800+0x4*(n)))
#define SOC_MDC_GMP_EN_ADDR(base) ((base) + (0x0B00))
#define SOC_MDC_GMP_MEM_CTRL_ADDR(base) ((base) + (0x0B04))
#define SOC_MDC_GMP_LUT_SEL_ADDR(base) ((base) + (0x0B08))
#define SOC_MDC_GMP_DBG_W0_ADDR(base) ((base) + (0x0B10))
#define SOC_MDC_GMP_DBG_R0_ADDR(base) ((base) + (0x0B14))
#define SOC_MDC_GMP_DBG_R1_ADDR(base) ((base) + (0x0B18))
#define SOC_MDC_GMP_DBG_R2_ADDR(base) ((base) + (0x0B1C))
#define SOC_MDC_GMP_DBG_R3_ADDR(base) ((base) + (0x0B20))
#endif
#ifndef __SOC_H_FOR_ASM__
#define SOC_MDC_U_MDC_GMP_COEF_ADDR(base,x) ((base) + (0(x)0000+0(x)4*(x)))
#else
#define SOC_MDC_U_MDC_GMP_COEF_ADDR(base,x) ((base) + (0(x)0000+0(x)4*(x)))
#endif
#ifndef __SOC_H_FOR_ASM__
#define SOC_MDC_OV_SIZE_ADDR(base) ((base) + (0x00UL))
#define SOC_MDC_OV_BG_COLOR_RGB_ADDR(base) ((base) + (0x04UL))
#define SOC_MDC_OV_BG_COLOR_A_ADDR(base) ((base) + (0x08UL))
#define SOC_MDC_DST_STARTPOS_ADDR(base) ((base) + (0x0CUL))
#define SOC_MDC_DST_ENDPOS_ADDR(base) ((base) + (0x10UL))
#define SOC_MDC_OV_GCFG_ADDR(base) ((base) + (0x14UL))
#define SOC_MDC_LAYER0_POS_ADDR(base) ((base) + (0x30UL))
#define SOC_MDC_LAYER0_SIZE_ADDR(base) ((base) + (0x34UL))
#define SOC_MDC_LAYER0_SRCLOKEY_ADDR(base) ((base) + (0x38UL))
#define SOC_MDC_LAYER0_SRCHIKEY_ADDR(base) ((base) + (0x3CUL))
#define SOC_MDC_LAYER0_DSTLOKEY_ADDR(base) ((base) + (0x40UL))
#define SOC_MDC_LAYER0_DSTHIKEY_ADDR(base) ((base) + (0x44UL))
#define SOC_MDC_LAYER0_PATTERN_RGB_ADDR(base) ((base) + (0x48UL))
#define SOC_MDC_LAYER0_PATTERN_A_ADDR(base) ((base) + (0x4CUL))
#define SOC_MDC_LAYER0_ALPHA_MODE_ADDR(base) ((base) + (0x50UL))
#define SOC_MDC_LAYER0_ALPHA_A_ADDR(base) ((base) + (0x54UL))
#define SOC_MDC_LAYER0_CFG_ADDR(base) ((base) + (0x58UL))
#define SOC_MDC_LAYER0_PSPOS_ADDR(base) ((base) + (0x5CUL))
#define SOC_MDC_LAYER0_PEPOS_ADDR(base) ((base) + (0x60UL))
#define SOC_MDC_LAYER0_INFO_ALPHA_ADDR(base) ((base) + (0x64UL))
#define SOC_MDC_LAYER0_INFO_SRCCOLOR_ADDR(base) ((base) + (0x68UL))
#define SOC_MDC_LAYER0_DBG_INFO_ADDR(base) ((base) + (0x6CUL))
#define SOC_MDC_LAYER1_POS_ADDR(base) ((base) + (0x90UL))
#define SOC_MDC_LAYER1_SIZE_ADDR(base) ((base) + (0x94UL))
#define SOC_MDC_LAYER1_SRCLOKEY_ADDR(base) ((base) + (0x98UL))
#define SOC_MDC_LAYER1_SRCHIKEY_ADDR(base) ((base) + (0x9CUL))
#define SOC_MDC_LAYER1_DSTLOKEY_ADDR(base) ((base) + (0xA0UL))
#define SOC_MDC_LAYER1_DSTHIKEY_ADDR(base) ((base) + (0xA4UL))
#define SOC_MDC_LAYER1_PATTERN_RGB_ADDR(base) ((base) + (0xA8UL))
#define SOC_MDC_LAYER1_PATTERN_A_ADDR(base) ((base) + (0xACUL))
#define SOC_MDC_LAYER1_ALPHA_MODE_ADDR(base) ((base) + (0xB0UL))
#define SOC_MDC_LAYER1_ALPHA_A_ADDR(base) ((base) + (0xB4UL))
#define SOC_MDC_LAYER1_CFG_ADDR(base) ((base) + (0xB8UL))
#define SOC_MDC_LAYER1_PSPOS_ADDR(base) ((base) + (0xBCUL))
#define SOC_MDC_LAYER1_PEPOS_ADDR(base) ((base) + (0xC0UL))
#define SOC_MDC_LAYER1_INFO_ALPHA_ADDR(base) ((base) + (0xC4UL))
#define SOC_MDC_LAYER1_INFO_SRCCOLOR_ADDR(base) ((base) + (0xC8UL))
#define SOC_MDC_LAYER1_DBG_INFO_ADDR(base) ((base) + (0xCCUL))
#define SOC_MDC_LAYER2_POS_ADDR(base) ((base) + (0xF0UL))
#define SOC_MDC_LAYER2_SIZE_ADDR(base) ((base) + (0xF4UL))
#define SOC_MDC_LAYER2_SRCLOKEY_ADDR(base) ((base) + (0xF8UL))
#define SOC_MDC_LAYER2_SRCHIKEY_ADDR(base) ((base) + (0xFCUL))
#define SOC_MDC_LAYER2_DSTLOKEY_ADDR(base) ((base) + (0x100UL))
#define SOC_MDC_LAYER2_DSTHIKEY_ADDR(base) ((base) + (0x104UL))
#define SOC_MDC_LAYER2_PATTERN_RGB_ADDR(base) ((base) + (0x108UL))
#define SOC_MDC_LAYER2_PATTERN_A_ADDR(base) ((base) + (0x10CUL))
#define SOC_MDC_LAYER2_ALPHA_MODE_ADDR(base) ((base) + (0x110UL))
#define SOC_MDC_LAYER2_ALPHA_A_ADDR(base) ((base) + (0x114UL))
#define SOC_MDC_LAYER2_CFG_ADDR(base) ((base) + (0x118UL))
#define SOC_MDC_LAYER2_PSPOS_ADDR(base) ((base) + (0x11CUL))
#define SOC_MDC_LAYER2_PEPOS_ADDR(base) ((base) + (0x120UL))
#define SOC_MDC_LAYER2_INFO_ALPHA_ADDR(base) ((base) + (0x124UL))
#define SOC_MDC_LAYER2_INFO_SRCCOLOR_ADDR(base) ((base) + (0x128UL))
#define SOC_MDC_LAYER2_DBG_INFO_ADDR(base) ((base) + (0x12CUL))
#define SOC_MDC_LAYER3_POS_ADDR(base) ((base) + (0x150UL))
#define SOC_MDC_LAYER3_SIZE_ADDR(base) ((base) + (0x154UL))
#define SOC_MDC_LAYER3_SRCLOKEY_ADDR(base) ((base) + (0x158UL))
#define SOC_MDC_LAYER3_SRCHIKEY_ADDR(base) ((base) + (0x15CUL))
#define SOC_MDC_LAYER3_DSTLOKEY_ADDR(base) ((base) + (0x160UL))
#define SOC_MDC_LAYER3_DSTHIKEY_ADDR(base) ((base) + (0x164UL))
#define SOC_MDC_LAYER3_PATTERN_RGB_ADDR(base) ((base) + (0x168UL))
#define SOC_MDC_LAYER3_PATTERN_A_ADDR(base) ((base) + (0x16CUL))
#define SOC_MDC_LAYER3_ALPHA_MODE_ADDR(base) ((base) + (0x170UL))
#define SOC_MDC_LAYER3_ALPHA_A_ADDR(base) ((base) + (0x174UL))
#define SOC_MDC_LAYER3_CFG_ADDR(base) ((base) + (0x178UL))
#define SOC_MDC_LAYER3_PSPOS_ADDR(base) ((base) + (0x17CUL))
#define SOC_MDC_LAYER3_PEPOS_ADDR(base) ((base) + (0x180UL))
#define SOC_MDC_LAYER3_INFO_ALPHA_ADDR(base) ((base) + (0x184UL))
#define SOC_MDC_LAYER3_INFO_SRCCOLOR_ADDR(base) ((base) + (0x188UL))
#define SOC_MDC_LAYER3_DBG_INFO_ADDR(base) ((base) + (0x18CUL))
#define SOC_MDC_LAYER4_POS_ADDR(base) ((base) + (0x1B0UL))
#define SOC_MDC_LAYER4_SIZE_ADDR(base) ((base) + (0x1B4UL))
#define SOC_MDC_LAYER4_SRCLOKEY_ADDR(base) ((base) + (0x1B8UL))
#define SOC_MDC_LAYER4_SRCHIKEY_ADDR(base) ((base) + (0x1BCUL))
#define SOC_MDC_LAYER4_DSTLOKEY_ADDR(base) ((base) + (0x1C0UL))
#define SOC_MDC_LAYER4_DSTHIKEY_ADDR(base) ((base) + (0x1C4UL))
#define SOC_MDC_LAYER4_PATTERN_RGB_ADDR(base) ((base) + (0x1C8UL))
#define SOC_MDC_LAYER4_PATTERN_A_ADDR(base) ((base) + (0x1CCUL))
#define SOC_MDC_LAYER4_ALPHA_MODE_ADDR(base) ((base) + (0x1D0UL))
#define SOC_MDC_LAYER4_ALPHA_A_ADDR(base) ((base) + (0x1D4UL))
#define SOC_MDC_LAYER4_CFG_ADDR(base) ((base) + (0x1D8UL))
#define SOC_MDC_LAYER4_PSPOS_ADDR(base) ((base) + (0x1DCUL))
#define SOC_MDC_LAYER4_PEPOS_ADDR(base) ((base) + (0x1E0UL))
#define SOC_MDC_LAYER4_INFO_ALPHA_ADDR(base) ((base) + (0x1E4UL))
#define SOC_MDC_LAYER4_INFO_SRCCOLOR_ADDR(base) ((base) + (0x1E8UL))
#define SOC_MDC_LAYER4_DBG_INFO_ADDR(base) ((base) + (0x1ECUL))
#define SOC_MDC_LAYER5_POS_ADDR(base) ((base) + (0x210UL))
#define SOC_MDC_LAYER5_SIZE_ADDR(base) ((base) + (0x214UL))
#define SOC_MDC_LAYER5_SRCLOKEY_ADDR(base) ((base) + (0x218UL))
#define SOC_MDC_LAYER5_SRCHIKEY_ADDR(base) ((base) + (0x21CUL))
#define SOC_MDC_LAYER5_DSTLOKEY_ADDR(base) ((base) + (0x220UL))
#define SOC_MDC_LAYER5_DSTHIKEY_ADDR(base) ((base) + (0x224UL))
#define SOC_MDC_LAYER5_PATTERN_RGB_ADDR(base) ((base) + (0x228UL))
#define SOC_MDC_LAYER5_PATTERN_A_ADDR(base) ((base) + (0x22CUL))
#define SOC_MDC_LAYER5_ALPHA_MODE_ADDR(base) ((base) + (0x230UL))
#define SOC_MDC_LAYER5_ALPHA_A_ADDR(base) ((base) + (0x234UL))
#define SOC_MDC_LAYER5_CFG_ADDR(base) ((base) + (0x238UL))
#define SOC_MDC_LAYER5_PSPOS_ADDR(base) ((base) + (0x23CUL))
#define SOC_MDC_LAYER5_PEPOS_ADDR(base) ((base) + (0x240UL))
#define SOC_MDC_LAYER5_INFO_ALPHA_ADDR(base) ((base) + (0x244UL))
#define SOC_MDC_LAYER5_INFO_SRCCOLOR_ADDR(base) ((base) + (0x248UL))
#define SOC_MDC_LAYER5_DBG_INFO_ADDR(base) ((base) + (0x24CUL))
#define SOC_MDC_LAYER6_POS_ADDR(base) ((base) + (0x270UL))
#define SOC_MDC_LAYER6_SIZE_ADDR(base) ((base) + (0x274UL))
#define SOC_MDC_LAYER6_SRCLOKEY_ADDR(base) ((base) + (0x278UL))
#define SOC_MDC_LAYER6_SRCHIKEY_ADDR(base) ((base) + (0x27CUL))
#define SOC_MDC_LAYER6_DSTLOKEY_ADDR(base) ((base) + (0x280UL))
#define SOC_MDC_LAYER6_DSTHIKEY_ADDR(base) ((base) + (0x284UL))
#define SOC_MDC_LAYER6_PATTERN_RGB_ADDR(base) ((base) + (0x288UL))
#define SOC_MDC_LAYER6_PATTERN_A_ADDR(base) ((base) + (0x28CUL))
#define SOC_MDC_LAYER6_ALPHA_MODE_ADDR(base) ((base) + (0x290UL))
#define SOC_MDC_LAYER6_ALPHA_A_ADDR(base) ((base) + (0x294UL))
#define SOC_MDC_LAYER6_CFG_ADDR(base) ((base) + (0x298UL))
#define SOC_MDC_LAYER6_PSPOS_ADDR(base) ((base) + (0x29CUL))
#define SOC_MDC_LAYER6_PEPOS_ADDR(base) ((base) + (0x2A0UL))
#define SOC_MDC_LAYER6_INFO_ALPHA_ADDR(base) ((base) + (0x2A4UL))
#define SOC_MDC_LAYER6_INFO_SRCCOLOR_ADDR(base) ((base) + (0x2A8UL))
#define SOC_MDC_LAYER6_DBG_INFO_ADDR(base) ((base) + (0x2ACUL))
#define SOC_MDC_LAYER7_POS_ADDR(base) ((base) + (0x2D0UL))
#define SOC_MDC_LAYER7_SIZE_ADDR(base) ((base) + (0x2D4UL))
#define SOC_MDC_LAYER7_SRCLOKEY_ADDR(base) ((base) + (0x2D8UL))
#define SOC_MDC_LAYER7_SRCHIKEY_ADDR(base) ((base) + (0x2DCUL))
#define SOC_MDC_LAYER7_DSTLOKEY_ADDR(base) ((base) + (0x2E0UL))
#define SOC_MDC_LAYER7_DSTHIKEY_ADDR(base) ((base) + (0x2E4UL))
#define SOC_MDC_LAYER7_PATTERN_RGB_ADDR(base) ((base) + (0x2E8UL))
#define SOC_MDC_LAYER7_PATTERN_A_ADDR(base) ((base) + (0x2ECUL))
#define SOC_MDC_LAYER7_ALPHA_MODE_ADDR(base) ((base) + (0x2F0UL))
#define SOC_MDC_LAYER7_ALPHA_A_ADDR(base) ((base) + (0x2F4UL))
#define SOC_MDC_LAYER7_CFG_ADDR(base) ((base) + (0x2F8UL))
#define SOC_MDC_LAYER7_PSPOS_ADDR(base) ((base) + (0x2FCUL))
#define SOC_MDC_LAYER7_PEPOS_ADDR(base) ((base) + (0x300UL))
#define SOC_MDC_LAYER7_INFO_ALPHA_ADDR(base) ((base) + (0x304UL))
#define SOC_MDC_LAYER7_INFO_SRCCOLOR_ADDR(base) ((base) + (0x308UL))
#define SOC_MDC_LAYER7_DBG_INFO_ADDR(base) ((base) + (0x30CUL))
#define SOC_MDC_BASE_DBG_INFO_ADDR(base) ((base) + (0x340UL))
#define SOC_MDC_OV_RD_SHADOW_SEL_ADDR(base) ((base) + (0x344UL))
#define SOC_MDC_OV_CLK_SEL_ADDR(base) ((base) + (0x348UL))
#define SOC_MDC_OV_CLK_EN_ADDR(base) ((base) + (0x34CUL))
#define SOC_MDC_BLOCK_SIZE_ADDR(base) ((base) + (0x350UL))
#define SOC_MDC_BLOCK_DBG_ADDR(base) ((base) + (0x354UL))
#define SOC_MDC_OV8_REG_DEFAULT_ADDR(base) ((base) + (0x358UL))
#else
#define SOC_MDC_OV_SIZE_ADDR(base) ((base) + (0x00))
#define SOC_MDC_OV_BG_COLOR_RGB_ADDR(base) ((base) + (0x04))
#define SOC_MDC_OV_BG_COLOR_A_ADDR(base) ((base) + (0x08))
#define SOC_MDC_DST_STARTPOS_ADDR(base) ((base) + (0x0C))
#define SOC_MDC_DST_ENDPOS_ADDR(base) ((base) + (0x10))
#define SOC_MDC_OV_GCFG_ADDR(base) ((base) + (0x14))
#define SOC_MDC_LAYER0_POS_ADDR(base) ((base) + (0x30))
#define SOC_MDC_LAYER0_SIZE_ADDR(base) ((base) + (0x34))
#define SOC_MDC_LAYER0_SRCLOKEY_ADDR(base) ((base) + (0x38))
#define SOC_MDC_LAYER0_SRCHIKEY_ADDR(base) ((base) + (0x3C))
#define SOC_MDC_LAYER0_DSTLOKEY_ADDR(base) ((base) + (0x40))
#define SOC_MDC_LAYER0_DSTHIKEY_ADDR(base) ((base) + (0x44))
#define SOC_MDC_LAYER0_PATTERN_RGB_ADDR(base) ((base) + (0x48))
#define SOC_MDC_LAYER0_PATTERN_A_ADDR(base) ((base) + (0x4C))
#define SOC_MDC_LAYER0_ALPHA_MODE_ADDR(base) ((base) + (0x50))
#define SOC_MDC_LAYER0_ALPHA_A_ADDR(base) ((base) + (0x54))
#define SOC_MDC_LAYER0_CFG_ADDR(base) ((base) + (0x58))
#define SOC_MDC_LAYER0_PSPOS_ADDR(base) ((base) + (0x5C))
#define SOC_MDC_LAYER0_PEPOS_ADDR(base) ((base) + (0x60))
#define SOC_MDC_LAYER0_INFO_ALPHA_ADDR(base) ((base) + (0x64))
#define SOC_MDC_LAYER0_INFO_SRCCOLOR_ADDR(base) ((base) + (0x68))
#define SOC_MDC_LAYER0_DBG_INFO_ADDR(base) ((base) + (0x6C))
#define SOC_MDC_LAYER1_POS_ADDR(base) ((base) + (0x90))
#define SOC_MDC_LAYER1_SIZE_ADDR(base) ((base) + (0x94))
#define SOC_MDC_LAYER1_SRCLOKEY_ADDR(base) ((base) + (0x98))
#define SOC_MDC_LAYER1_SRCHIKEY_ADDR(base) ((base) + (0x9C))
#define SOC_MDC_LAYER1_DSTLOKEY_ADDR(base) ((base) + (0xA0))
#define SOC_MDC_LAYER1_DSTHIKEY_ADDR(base) ((base) + (0xA4))
#define SOC_MDC_LAYER1_PATTERN_RGB_ADDR(base) ((base) + (0xA8))
#define SOC_MDC_LAYER1_PATTERN_A_ADDR(base) ((base) + (0xAC))
#define SOC_MDC_LAYER1_ALPHA_MODE_ADDR(base) ((base) + (0xB0))
#define SOC_MDC_LAYER1_ALPHA_A_ADDR(base) ((base) + (0xB4))
#define SOC_MDC_LAYER1_CFG_ADDR(base) ((base) + (0xB8))
#define SOC_MDC_LAYER1_PSPOS_ADDR(base) ((base) + (0xBC))
#define SOC_MDC_LAYER1_PEPOS_ADDR(base) ((base) + (0xC0))
#define SOC_MDC_LAYER1_INFO_ALPHA_ADDR(base) ((base) + (0xC4))
#define SOC_MDC_LAYER1_INFO_SRCCOLOR_ADDR(base) ((base) + (0xC8))
#define SOC_MDC_LAYER1_DBG_INFO_ADDR(base) ((base) + (0xCC))
#define SOC_MDC_LAYER2_POS_ADDR(base) ((base) + (0xF0))
#define SOC_MDC_LAYER2_SIZE_ADDR(base) ((base) + (0xF4))
#define SOC_MDC_LAYER2_SRCLOKEY_ADDR(base) ((base) + (0xF8))
#define SOC_MDC_LAYER2_SRCHIKEY_ADDR(base) ((base) + (0xFC))
#define SOC_MDC_LAYER2_DSTLOKEY_ADDR(base) ((base) + (0x100))
#define SOC_MDC_LAYER2_DSTHIKEY_ADDR(base) ((base) + (0x104))
#define SOC_MDC_LAYER2_PATTERN_RGB_ADDR(base) ((base) + (0x108))
#define SOC_MDC_LAYER2_PATTERN_A_ADDR(base) ((base) + (0x10C))
#define SOC_MDC_LAYER2_ALPHA_MODE_ADDR(base) ((base) + (0x110))
#define SOC_MDC_LAYER2_ALPHA_A_ADDR(base) ((base) + (0x114))
#define SOC_MDC_LAYER2_CFG_ADDR(base) ((base) + (0x118))
#define SOC_MDC_LAYER2_PSPOS_ADDR(base) ((base) + (0x11C))
#define SOC_MDC_LAYER2_PEPOS_ADDR(base) ((base) + (0x120))
#define SOC_MDC_LAYER2_INFO_ALPHA_ADDR(base) ((base) + (0x124))
#define SOC_MDC_LAYER2_INFO_SRCCOLOR_ADDR(base) ((base) + (0x128))
#define SOC_MDC_LAYER2_DBG_INFO_ADDR(base) ((base) + (0x12C))
#define SOC_MDC_LAYER3_POS_ADDR(base) ((base) + (0x150))
#define SOC_MDC_LAYER3_SIZE_ADDR(base) ((base) + (0x154))
#define SOC_MDC_LAYER3_SRCLOKEY_ADDR(base) ((base) + (0x158))
#define SOC_MDC_LAYER3_SRCHIKEY_ADDR(base) ((base) + (0x15C))
#define SOC_MDC_LAYER3_DSTLOKEY_ADDR(base) ((base) + (0x160))
#define SOC_MDC_LAYER3_DSTHIKEY_ADDR(base) ((base) + (0x164))
#define SOC_MDC_LAYER3_PATTERN_RGB_ADDR(base) ((base) + (0x168))
#define SOC_MDC_LAYER3_PATTERN_A_ADDR(base) ((base) + (0x16C))
#define SOC_MDC_LAYER3_ALPHA_MODE_ADDR(base) ((base) + (0x170))
#define SOC_MDC_LAYER3_ALPHA_A_ADDR(base) ((base) + (0x174))
#define SOC_MDC_LAYER3_CFG_ADDR(base) ((base) + (0x178))
#define SOC_MDC_LAYER3_PSPOS_ADDR(base) ((base) + (0x17C))
#define SOC_MDC_LAYER3_PEPOS_ADDR(base) ((base) + (0x180))
#define SOC_MDC_LAYER3_INFO_ALPHA_ADDR(base) ((base) + (0x184))
#define SOC_MDC_LAYER3_INFO_SRCCOLOR_ADDR(base) ((base) + (0x188))
#define SOC_MDC_LAYER3_DBG_INFO_ADDR(base) ((base) + (0x18C))
#define SOC_MDC_LAYER4_POS_ADDR(base) ((base) + (0x1B0))
#define SOC_MDC_LAYER4_SIZE_ADDR(base) ((base) + (0x1B4))
#define SOC_MDC_LAYER4_SRCLOKEY_ADDR(base) ((base) + (0x1B8))
#define SOC_MDC_LAYER4_SRCHIKEY_ADDR(base) ((base) + (0x1BC))
#define SOC_MDC_LAYER4_DSTLOKEY_ADDR(base) ((base) + (0x1C0))
#define SOC_MDC_LAYER4_DSTHIKEY_ADDR(base) ((base) + (0x1C4))
#define SOC_MDC_LAYER4_PATTERN_RGB_ADDR(base) ((base) + (0x1C8))
#define SOC_MDC_LAYER4_PATTERN_A_ADDR(base) ((base) + (0x1CC))
#define SOC_MDC_LAYER4_ALPHA_MODE_ADDR(base) ((base) + (0x1D0))
#define SOC_MDC_LAYER4_ALPHA_A_ADDR(base) ((base) + (0x1D4))
#define SOC_MDC_LAYER4_CFG_ADDR(base) ((base) + (0x1D8))
#define SOC_MDC_LAYER4_PSPOS_ADDR(base) ((base) + (0x1DC))
#define SOC_MDC_LAYER4_PEPOS_ADDR(base) ((base) + (0x1E0))
#define SOC_MDC_LAYER4_INFO_ALPHA_ADDR(base) ((base) + (0x1E4))
#define SOC_MDC_LAYER4_INFO_SRCCOLOR_ADDR(base) ((base) + (0x1E8))
#define SOC_MDC_LAYER4_DBG_INFO_ADDR(base) ((base) + (0x1EC))
#define SOC_MDC_LAYER5_POS_ADDR(base) ((base) + (0x210))
#define SOC_MDC_LAYER5_SIZE_ADDR(base) ((base) + (0x214))
#define SOC_MDC_LAYER5_SRCLOKEY_ADDR(base) ((base) + (0x218))
#define SOC_MDC_LAYER5_SRCHIKEY_ADDR(base) ((base) + (0x21C))
#define SOC_MDC_LAYER5_DSTLOKEY_ADDR(base) ((base) + (0x220))
#define SOC_MDC_LAYER5_DSTHIKEY_ADDR(base) ((base) + (0x224))
#define SOC_MDC_LAYER5_PATTERN_RGB_ADDR(base) ((base) + (0x228))
#define SOC_MDC_LAYER5_PATTERN_A_ADDR(base) ((base) + (0x22C))
#define SOC_MDC_LAYER5_ALPHA_MODE_ADDR(base) ((base) + (0x230))
#define SOC_MDC_LAYER5_ALPHA_A_ADDR(base) ((base) + (0x234))
#define SOC_MDC_LAYER5_CFG_ADDR(base) ((base) + (0x238))
#define SOC_MDC_LAYER5_PSPOS_ADDR(base) ((base) + (0x23C))
#define SOC_MDC_LAYER5_PEPOS_ADDR(base) ((base) + (0x240))
#define SOC_MDC_LAYER5_INFO_ALPHA_ADDR(base) ((base) + (0x244))
#define SOC_MDC_LAYER5_INFO_SRCCOLOR_ADDR(base) ((base) + (0x248))
#define SOC_MDC_LAYER5_DBG_INFO_ADDR(base) ((base) + (0x24C))
#define SOC_MDC_LAYER6_POS_ADDR(base) ((base) + (0x270))
#define SOC_MDC_LAYER6_SIZE_ADDR(base) ((base) + (0x274))
#define SOC_MDC_LAYER6_SRCLOKEY_ADDR(base) ((base) + (0x278))
#define SOC_MDC_LAYER6_SRCHIKEY_ADDR(base) ((base) + (0x27C))
#define SOC_MDC_LAYER6_DSTLOKEY_ADDR(base) ((base) + (0x280))
#define SOC_MDC_LAYER6_DSTHIKEY_ADDR(base) ((base) + (0x284))
#define SOC_MDC_LAYER6_PATTERN_RGB_ADDR(base) ((base) + (0x288))
#define SOC_MDC_LAYER6_PATTERN_A_ADDR(base) ((base) + (0x28C))
#define SOC_MDC_LAYER6_ALPHA_MODE_ADDR(base) ((base) + (0x290))
#define SOC_MDC_LAYER6_ALPHA_A_ADDR(base) ((base) + (0x294))
#define SOC_MDC_LAYER6_CFG_ADDR(base) ((base) + (0x298))
#define SOC_MDC_LAYER6_PSPOS_ADDR(base) ((base) + (0x29C))
#define SOC_MDC_LAYER6_PEPOS_ADDR(base) ((base) + (0x2A0))
#define SOC_MDC_LAYER6_INFO_ALPHA_ADDR(base) ((base) + (0x2A4))
#define SOC_MDC_LAYER6_INFO_SRCCOLOR_ADDR(base) ((base) + (0x2A8))
#define SOC_MDC_LAYER6_DBG_INFO_ADDR(base) ((base) + (0x2AC))
#define SOC_MDC_LAYER7_POS_ADDR(base) ((base) + (0x2D0))
#define SOC_MDC_LAYER7_SIZE_ADDR(base) ((base) + (0x2D4))
#define SOC_MDC_LAYER7_SRCLOKEY_ADDR(base) ((base) + (0x2D8))
#define SOC_MDC_LAYER7_SRCHIKEY_ADDR(base) ((base) + (0x2DC))
#define SOC_MDC_LAYER7_DSTLOKEY_ADDR(base) ((base) + (0x2E0))
#define SOC_MDC_LAYER7_DSTHIKEY_ADDR(base) ((base) + (0x2E4))
#define SOC_MDC_LAYER7_PATTERN_RGB_ADDR(base) ((base) + (0x2E8))
#define SOC_MDC_LAYER7_PATTERN_A_ADDR(base) ((base) + (0x2EC))
#define SOC_MDC_LAYER7_ALPHA_MODE_ADDR(base) ((base) + (0x2F0))
#define SOC_MDC_LAYER7_ALPHA_A_ADDR(base) ((base) + (0x2F4))
#define SOC_MDC_LAYER7_CFG_ADDR(base) ((base) + (0x2F8))
#define SOC_MDC_LAYER7_PSPOS_ADDR(base) ((base) + (0x2FC))
#define SOC_MDC_LAYER7_PEPOS_ADDR(base) ((base) + (0x300))
#define SOC_MDC_LAYER7_INFO_ALPHA_ADDR(base) ((base) + (0x304))
#define SOC_MDC_LAYER7_INFO_SRCCOLOR_ADDR(base) ((base) + (0x308))
#define SOC_MDC_LAYER7_DBG_INFO_ADDR(base) ((base) + (0x30C))
#define SOC_MDC_BASE_DBG_INFO_ADDR(base) ((base) + (0x340))
#define SOC_MDC_OV_RD_SHADOW_SEL_ADDR(base) ((base) + (0x344))
#define SOC_MDC_OV_CLK_SEL_ADDR(base) ((base) + (0x348))
#define SOC_MDC_OV_CLK_EN_ADDR(base) ((base) + (0x34C))
#define SOC_MDC_BLOCK_SIZE_ADDR(base) ((base) + (0x350))
#define SOC_MDC_BLOCK_DBG_ADDR(base) ((base) + (0x354))
#define SOC_MDC_OV8_REG_DEFAULT_ADDR(base) ((base) + (0x358))
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dss_tag : 32;
    } reg;
} soc_mdc_dss_tag_union;
#endif
#define SOC_MDC_DSS_TAG_DSS_TAG_START (0)
#define SOC_MDC_DSS_TAG_DSS_TAG_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int apb_cpu_force : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_apb_ctl_union;
#endif
#define SOC_MDC_APB_CTL_APB_CPU_FORCE_START (0)
#define SOC_MDC_APB_CTL_APB_CPU_FORCE_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int mmbuf_cfg_secu_n : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_mmbuf_cfg_secu_en_union;
#endif
#define SOC_MDC_MMBUF_CFG_SECU_EN_MMBUF_CFG_SECU_N_START (0)
#define SOC_MDC_MMBUF_CFG_SECU_EN_MMBUF_CFG_SECU_N_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int asc_cfg_secu_n : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_asc_cfg_secu_en_union;
#endif
#define SOC_MDC_ASC_CFG_SECU_EN_ASC_CFG_SECU_N_START (0)
#define SOC_MDC_ASC_CFG_SECU_EN_ASC_CFG_SECU_N_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dss_axi_rst_en : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_dss_axi_rst_en_union;
#endif
#define SOC_MDC_DSS_AXI_RST_EN_DSS_AXI_RST_EN_START (0)
#define SOC_MDC_DSS_AXI_RST_EN_DSS_AXI_RST_EN_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int cpu_off_ints : 32;
    } reg;
} soc_mdc_cpu_off_ints_union;
#endif
#define SOC_MDC_CPU_OFF_INTS_CPU_OFF_INTS_START (0)
#define SOC_MDC_CPU_OFF_INTS_CPU_OFF_INTS_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int cpu_off_int_msk : 32;
    } reg;
} soc_mdc_cpu_off_int_msk_union;
#endif
#define SOC_MDC_CPU_OFF_INT_MSK_CPU_OFF_INT_MSK_START (0)
#define SOC_MDC_CPU_OFF_INT_MSK_CPU_OFF_INT_MSK_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int cpu_off_cam_ints : 17;
        unsigned int reserved : 15;
    } reg;
} soc_mdc_cpu_off_cam_ints_union;
#endif
#define SOC_MDC_CPU_OFF_CAM_INTS_CPU_OFF_CAM_INTS_START (0)
#define SOC_MDC_CPU_OFF_CAM_INTS_CPU_OFF_CAM_INTS_END (16)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int cpu_off_cam_int_msk : 17;
        unsigned int reserved : 15;
    } reg;
} soc_mdc_cpu_off_cam_int_msk_union;
#endif
#define SOC_MDC_CPU_OFF_CAM_INT_MSK_CPU_OFF_CAM_INT_MSK_START (0)
#define SOC_MDC_CPU_OFF_CAM_INT_MSK_CPU_OFF_CAM_INT_MSK_END (16)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int module_clk_sel : 32;
    } reg;
} soc_mdc_glb_module_clk_sel_union;
#endif
#define SOC_MDC_GLB_MODULE_CLK_SEL_MODULE_CLK_SEL_START (0)
#define SOC_MDC_GLB_MODULE_CLK_SEL_MODULE_CLK_SEL_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int module_clk_en : 32;
    } reg;
} soc_mdc_glb_module_clk_en_union;
#endif
#define SOC_MDC_GLB_MODULE_CLK_EN_MODULE_CLK_EN_START (0)
#define SOC_MDC_GLB_MODULE_CLK_EN_MODULE_CLK_EN_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int glb0_core_dbg_sel : 4;
        unsigned int dss_sum_dbg_sel : 2;
        unsigned int dss_sum_dbg_en : 1;
        unsigned int reserved_0 : 1;
        unsigned int glb0_axi_dbg_sel : 2;
        unsigned int reserved_1 : 22;
    } reg;
} soc_mdc_glb0_dbg_sel_union;
#endif
#define SOC_MDC_GLB0_DBG_SEL_GLB0_CORE_DBG_SEL_START (0)
#define SOC_MDC_GLB0_DBG_SEL_GLB0_CORE_DBG_SEL_END (3)
#define SOC_MDC_GLB0_DBG_SEL_DSS_SUM_DBG_SEL_START (4)
#define SOC_MDC_GLB0_DBG_SEL_DSS_SUM_DBG_SEL_END (5)
#define SOC_MDC_GLB0_DBG_SEL_DSS_SUM_DBG_EN_START (6)
#define SOC_MDC_GLB0_DBG_SEL_DSS_SUM_DBG_EN_END (6)
#define SOC_MDC_GLB0_DBG_SEL_GLB0_AXI_DBG_SEL_START (8)
#define SOC_MDC_GLB0_DBG_SEL_GLB0_AXI_DBG_SEL_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int glb1_core_dbg_sel : 2;
        unsigned int glb1_clk_dbg_sel : 2;
        unsigned int reserved_0 : 4;
        unsigned int glb1_pxl0_dbg_sel : 2;
        unsigned int reserved_1 : 22;
    } reg;
} soc_mdc_glb1_dbg_sel_union;
#endif
#define SOC_MDC_GLB1_DBG_SEL_GLB1_CORE_DBG_SEL_START (0)
#define SOC_MDC_GLB1_DBG_SEL_GLB1_CORE_DBG_SEL_END (1)
#define SOC_MDC_GLB1_DBG_SEL_GLB1_CLK_DBG_SEL_START (2)
#define SOC_MDC_GLB1_DBG_SEL_GLB1_CLK_DBG_SEL_END (3)
#define SOC_MDC_GLB1_DBG_SEL_GLB1_PXL0_DBG_SEL_START (8)
#define SOC_MDC_GLB1_DBG_SEL_GLB1_PXL0_DBG_SEL_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dbg_irq_cpu : 3;
        unsigned int reserved : 29;
    } reg;
} soc_mdc_dbg_irq_cpu_union;
#endif
#define SOC_MDC_DBG_IRQ_CPU_DBG_IRQ_CPU_START (0)
#define SOC_MDC_DBG_IRQ_CPU_DBG_IRQ_CPU_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dbg_irq_mcu : 3;
        unsigned int reserved : 29;
    } reg;
} soc_mdc_dbg_irq_mcu_union;
#endif
#define SOC_MDC_DBG_IRQ_MCU_DBG_IRQ_MCU_START (0)
#define SOC_MDC_DBG_IRQ_MCU_DBG_IRQ_MCU_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dss_spram_ctrl : 32;
    } reg;
} soc_mdc_dss_spram_ctrl_union;
#endif
#define SOC_MDC_DSS_SPRAM_CTRL_DSS_SPRAM_CTRL_START (0)
#define SOC_MDC_DSS_SPRAM_CTRL_DSS_SPRAM_CTRL_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dss_pm_ctrl : 31;
        unsigned int reserved : 1;
    } reg;
} soc_mdc_dss_pm_ctrl_union;
#endif
#define SOC_MDC_DSS_PM_CTRL_DSS_PM_CTRL_START (0)
#define SOC_MDC_DSS_PM_CTRL_DSS_PM_CTRL_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int glb_dslp_en : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_glb_dslp_en_union;
#endif
#define SOC_MDC_GLB_DSLP_EN_GLB_DSLP_EN_START (0)
#define SOC_MDC_GLB_DSLP_EN_GLB_DSLP_EN_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dss_tpram_ctrl : 32;
    } reg;
} soc_mdc_dss_tpram_ctrl_union;
#endif
#define SOC_MDC_DSS_TPRAM_CTRL_DSS_TPRAM_CTRL_START (0)
#define SOC_MDC_DSS_TPRAM_CTRL_DSS_TPRAM_CTRL_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int axi_sel_0 : 2;
        unsigned int reserved_0 : 6;
        unsigned int credit_mode_0 : 3;
        unsigned int reserved_1 : 1;
        unsigned int max_cont_0 : 4;
        unsigned int reserved_2 : 16;
    } reg;
} soc_mdc_aif_ch_ctl_0_union;
#endif
#define SOC_MDC_AIF_CH_CTL_0_AXI_SEL_0_START (0)
#define SOC_MDC_AIF_CH_CTL_0_AXI_SEL_0_END (1)
#define SOC_MDC_AIF_CH_CTL_0_CREDIT_MODE_0_START (8)
#define SOC_MDC_AIF_CH_CTL_0_CREDIT_MODE_0_END (10)
#define SOC_MDC_AIF_CH_CTL_0_MAX_CONT_0_START (12)
#define SOC_MDC_AIF_CH_CTL_0_MAX_CONT_0_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int credit_en_0 : 1;
        unsigned int reserved_0 : 3;
        unsigned int credit_step_0 : 7;
        unsigned int reserved_1 : 1;
        unsigned int credit_uth_0 : 9;
        unsigned int credit_lth_0 : 9;
        unsigned int reserved_2 : 2;
    } reg;
} soc_mdc_aif_ch_hs_0_union;
#endif
#define SOC_MDC_AIF_CH_HS_0_CREDIT_EN_0_START (0)
#define SOC_MDC_AIF_CH_HS_0_CREDIT_EN_0_END (0)
#define SOC_MDC_AIF_CH_HS_0_CREDIT_STEP_0_START (4)
#define SOC_MDC_AIF_CH_HS_0_CREDIT_STEP_0_END (10)
#define SOC_MDC_AIF_CH_HS_0_CREDIT_UTH_0_START (12)
#define SOC_MDC_AIF_CH_HS_0_CREDIT_UTH_0_END (20)
#define SOC_MDC_AIF_CH_HS_0_CREDIT_LTH_0_START (21)
#define SOC_MDC_AIF_CH_HS_0_CREDIT_LTH_0_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int credit_en_lower_0 : 1;
        unsigned int reserved_0 : 3;
        unsigned int credit_step_lower_0 : 7;
        unsigned int reserved_1 : 1;
        unsigned int credit_uth_lower_0 : 9;
        unsigned int credit_lth_lower_0 : 9;
        unsigned int reserved_2 : 2;
    } reg;
} soc_mdc_aif_ch_ls_0_union;
#endif
#define SOC_MDC_AIF_CH_LS_0_CREDIT_EN_LOWER_0_START (0)
#define SOC_MDC_AIF_CH_LS_0_CREDIT_EN_LOWER_0_END (0)
#define SOC_MDC_AIF_CH_LS_0_CREDIT_STEP_LOWER_0_START (4)
#define SOC_MDC_AIF_CH_LS_0_CREDIT_STEP_LOWER_0_END (10)
#define SOC_MDC_AIF_CH_LS_0_CREDIT_UTH_LOWER_0_START (12)
#define SOC_MDC_AIF_CH_LS_0_CREDIT_UTH_LOWER_0_END (20)
#define SOC_MDC_AIF_CH_LS_0_CREDIT_LTH_LOWER_0_START (21)
#define SOC_MDC_AIF_CH_LS_0_CREDIT_LTH_LOWER_0_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int axi_sel_1 : 2;
        unsigned int reserved_0 : 6;
        unsigned int credit_mode_1 : 3;
        unsigned int reserved_1 : 1;
        unsigned int max_cont_1 : 4;
        unsigned int reserved_2 : 16;
    } reg;
} soc_mdc_aif_ch_ctl_1_union;
#endif
#define SOC_MDC_AIF_CH_CTL_1_AXI_SEL_1_START (0)
#define SOC_MDC_AIF_CH_CTL_1_AXI_SEL_1_END (1)
#define SOC_MDC_AIF_CH_CTL_1_CREDIT_MODE_1_START (8)
#define SOC_MDC_AIF_CH_CTL_1_CREDIT_MODE_1_END (10)
#define SOC_MDC_AIF_CH_CTL_1_MAX_CONT_1_START (12)
#define SOC_MDC_AIF_CH_CTL_1_MAX_CONT_1_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int credit_en_1 : 1;
        unsigned int reserved_0 : 3;
        unsigned int credit_step_1 : 7;
        unsigned int reserved_1 : 1;
        unsigned int credit_uth_1 : 9;
        unsigned int credit_lth_1 : 9;
        unsigned int reserved_2 : 2;
    } reg;
} soc_mdc_aif_ch_hs_1_union;
#endif
#define SOC_MDC_AIF_CH_HS_1_CREDIT_EN_1_START (0)
#define SOC_MDC_AIF_CH_HS_1_CREDIT_EN_1_END (0)
#define SOC_MDC_AIF_CH_HS_1_CREDIT_STEP_1_START (4)
#define SOC_MDC_AIF_CH_HS_1_CREDIT_STEP_1_END (10)
#define SOC_MDC_AIF_CH_HS_1_CREDIT_UTH_1_START (12)
#define SOC_MDC_AIF_CH_HS_1_CREDIT_UTH_1_END (20)
#define SOC_MDC_AIF_CH_HS_1_CREDIT_LTH_1_START (21)
#define SOC_MDC_AIF_CH_HS_1_CREDIT_LTH_1_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int credit_en_lower_1 : 1;
        unsigned int reserved_0 : 3;
        unsigned int credit_step_lower_1 : 7;
        unsigned int reserved_1 : 1;
        unsigned int credit_uth_lower_1 : 9;
        unsigned int credit_lth_lower_1 : 9;
        unsigned int reserved_2 : 2;
    } reg;
} soc_mdc_aif_ch_ls_1_union;
#endif
#define SOC_MDC_AIF_CH_LS_1_CREDIT_EN_LOWER_1_START (0)
#define SOC_MDC_AIF_CH_LS_1_CREDIT_EN_LOWER_1_END (0)
#define SOC_MDC_AIF_CH_LS_1_CREDIT_STEP_LOWER_1_START (4)
#define SOC_MDC_AIF_CH_LS_1_CREDIT_STEP_LOWER_1_END (10)
#define SOC_MDC_AIF_CH_LS_1_CREDIT_UTH_LOWER_1_START (12)
#define SOC_MDC_AIF_CH_LS_1_CREDIT_UTH_LOWER_1_END (20)
#define SOC_MDC_AIF_CH_LS_1_CREDIT_LTH_LOWER_1_START (21)
#define SOC_MDC_AIF_CH_LS_1_CREDIT_LTH_LOWER_1_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int axi_sel_2 : 2;
        unsigned int reserved_0 : 6;
        unsigned int credit_mode_2 : 3;
        unsigned int reserved_1 : 1;
        unsigned int max_cont_2 : 4;
        unsigned int reserved_2 : 16;
    } reg;
} soc_mdc_aif_ch_ctl_2_union;
#endif
#define SOC_MDC_AIF_CH_CTL_2_AXI_SEL_2_START (0)
#define SOC_MDC_AIF_CH_CTL_2_AXI_SEL_2_END (1)
#define SOC_MDC_AIF_CH_CTL_2_CREDIT_MODE_2_START (8)
#define SOC_MDC_AIF_CH_CTL_2_CREDIT_MODE_2_END (10)
#define SOC_MDC_AIF_CH_CTL_2_MAX_CONT_2_START (12)
#define SOC_MDC_AIF_CH_CTL_2_MAX_CONT_2_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int credit_en_2 : 1;
        unsigned int reserved_0 : 3;
        unsigned int credit_step_2 : 7;
        unsigned int reserved_1 : 1;
        unsigned int credit_uth_2 : 9;
        unsigned int credit_lth_2 : 9;
        unsigned int reserved_2 : 2;
    } reg;
} soc_mdc_aif_ch_hs_2_union;
#endif
#define SOC_MDC_AIF_CH_HS_2_CREDIT_EN_2_START (0)
#define SOC_MDC_AIF_CH_HS_2_CREDIT_EN_2_END (0)
#define SOC_MDC_AIF_CH_HS_2_CREDIT_STEP_2_START (4)
#define SOC_MDC_AIF_CH_HS_2_CREDIT_STEP_2_END (10)
#define SOC_MDC_AIF_CH_HS_2_CREDIT_UTH_2_START (12)
#define SOC_MDC_AIF_CH_HS_2_CREDIT_UTH_2_END (20)
#define SOC_MDC_AIF_CH_HS_2_CREDIT_LTH_2_START (21)
#define SOC_MDC_AIF_CH_HS_2_CREDIT_LTH_2_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int credit_en_lower_2 : 1;
        unsigned int reserved_0 : 3;
        unsigned int credit_step_lower_2 : 7;
        unsigned int reserved_1 : 1;
        unsigned int credit_uth_lower_2 : 9;
        unsigned int credit_lth_lower_2 : 9;
        unsigned int reserved_2 : 2;
    } reg;
} soc_mdc_aif_ch_ls_2_union;
#endif
#define SOC_MDC_AIF_CH_LS_2_CREDIT_EN_LOWER_2_START (0)
#define SOC_MDC_AIF_CH_LS_2_CREDIT_EN_LOWER_2_END (0)
#define SOC_MDC_AIF_CH_LS_2_CREDIT_STEP_LOWER_2_START (4)
#define SOC_MDC_AIF_CH_LS_2_CREDIT_STEP_LOWER_2_END (10)
#define SOC_MDC_AIF_CH_LS_2_CREDIT_UTH_LOWER_2_START (12)
#define SOC_MDC_AIF_CH_LS_2_CREDIT_UTH_LOWER_2_END (20)
#define SOC_MDC_AIF_CH_LS_2_CREDIT_LTH_LOWER_2_START (21)
#define SOC_MDC_AIF_CH_LS_2_CREDIT_LTH_LOWER_2_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int axi_sel_6 : 2;
        unsigned int reserved_0 : 6;
        unsigned int credit_mode_6 : 3;
        unsigned int reserved_1 : 1;
        unsigned int max_cont_6 : 4;
        unsigned int reserved_2 : 16;
    } reg;
} soc_mdc_aif_ch_ctl_6_union;
#endif
#define SOC_MDC_AIF_CH_CTL_6_AXI_SEL_6_START (0)
#define SOC_MDC_AIF_CH_CTL_6_AXI_SEL_6_END (1)
#define SOC_MDC_AIF_CH_CTL_6_CREDIT_MODE_6_START (8)
#define SOC_MDC_AIF_CH_CTL_6_CREDIT_MODE_6_END (10)
#define SOC_MDC_AIF_CH_CTL_6_MAX_CONT_6_START (12)
#define SOC_MDC_AIF_CH_CTL_6_MAX_CONT_6_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int credit_en_6 : 1;
        unsigned int reserved_0 : 3;
        unsigned int credit_step_6 : 7;
        unsigned int reserved_1 : 1;
        unsigned int credit_uth_6 : 9;
        unsigned int credit_lth_6 : 9;
        unsigned int reserved_2 : 2;
    } reg;
} soc_mdc_aif_ch_hs_6_union;
#endif
#define SOC_MDC_AIF_CH_HS_6_CREDIT_EN_6_START (0)
#define SOC_MDC_AIF_CH_HS_6_CREDIT_EN_6_END (0)
#define SOC_MDC_AIF_CH_HS_6_CREDIT_STEP_6_START (4)
#define SOC_MDC_AIF_CH_HS_6_CREDIT_STEP_6_END (10)
#define SOC_MDC_AIF_CH_HS_6_CREDIT_UTH_6_START (12)
#define SOC_MDC_AIF_CH_HS_6_CREDIT_UTH_6_END (20)
#define SOC_MDC_AIF_CH_HS_6_CREDIT_LTH_6_START (21)
#define SOC_MDC_AIF_CH_HS_6_CREDIT_LTH_6_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int credit_en_lower_6 : 1;
        unsigned int reserved_0 : 3;
        unsigned int credit_step_lower_6 : 7;
        unsigned int reserved_1 : 1;
        unsigned int credit_uth_lower_6 : 9;
        unsigned int credit_lth_lower_6 : 9;
        unsigned int reserved_2 : 2;
    } reg;
} soc_mdc_aif_ch_ls_6_union;
#endif
#define SOC_MDC_AIF_CH_LS_6_CREDIT_EN_LOWER_6_START (0)
#define SOC_MDC_AIF_CH_LS_6_CREDIT_EN_LOWER_6_END (0)
#define SOC_MDC_AIF_CH_LS_6_CREDIT_STEP_LOWER_6_START (4)
#define SOC_MDC_AIF_CH_LS_6_CREDIT_STEP_LOWER_6_END (10)
#define SOC_MDC_AIF_CH_LS_6_CREDIT_UTH_LOWER_6_START (12)
#define SOC_MDC_AIF_CH_LS_6_CREDIT_UTH_LOWER_6_END (20)
#define SOC_MDC_AIF_CH_LS_6_CREDIT_LTH_LOWER_6_START (21)
#define SOC_MDC_AIF_CH_LS_6_CREDIT_LTH_LOWER_6_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int axi_sel_8 : 2;
        unsigned int reserved_0 : 6;
        unsigned int credit_mode_8 : 3;
        unsigned int reserved_1 : 1;
        unsigned int max_cont_8 : 4;
        unsigned int reserved_2 : 16;
    } reg;
} soc_mdc_aif_ch_ctl_8_union;
#endif
#define SOC_MDC_AIF_CH_CTL_8_AXI_SEL_8_START (0)
#define SOC_MDC_AIF_CH_CTL_8_AXI_SEL_8_END (1)
#define SOC_MDC_AIF_CH_CTL_8_CREDIT_MODE_8_START (8)
#define SOC_MDC_AIF_CH_CTL_8_CREDIT_MODE_8_END (10)
#define SOC_MDC_AIF_CH_CTL_8_MAX_CONT_8_START (12)
#define SOC_MDC_AIF_CH_CTL_8_MAX_CONT_8_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int credit_en_8 : 1;
        unsigned int reserved_0 : 3;
        unsigned int credit_step_8 : 7;
        unsigned int reserved_1 : 1;
        unsigned int credit_uth_8 : 9;
        unsigned int credit_lth_8 : 9;
        unsigned int reserved_2 : 2;
    } reg;
} soc_mdc_aif_ch_hs_8_union;
#endif
#define SOC_MDC_AIF_CH_HS_8_CREDIT_EN_8_START (0)
#define SOC_MDC_AIF_CH_HS_8_CREDIT_EN_8_END (0)
#define SOC_MDC_AIF_CH_HS_8_CREDIT_STEP_8_START (4)
#define SOC_MDC_AIF_CH_HS_8_CREDIT_STEP_8_END (10)
#define SOC_MDC_AIF_CH_HS_8_CREDIT_UTH_8_START (12)
#define SOC_MDC_AIF_CH_HS_8_CREDIT_UTH_8_END (20)
#define SOC_MDC_AIF_CH_HS_8_CREDIT_LTH_8_START (21)
#define SOC_MDC_AIF_CH_HS_8_CREDIT_LTH_8_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int credit_en_lower_8 : 1;
        unsigned int reserved_0 : 3;
        unsigned int credit_step_lower_8 : 7;
        unsigned int reserved_1 : 1;
        unsigned int credit_uth_lower_8 : 9;
        unsigned int credit_lth_lower_8 : 9;
        unsigned int reserved_2 : 2;
    } reg;
} soc_mdc_aif_ch_ls_8_union;
#endif
#define SOC_MDC_AIF_CH_LS_8_CREDIT_EN_LOWER_8_START (0)
#define SOC_MDC_AIF_CH_LS_8_CREDIT_EN_LOWER_8_END (0)
#define SOC_MDC_AIF_CH_LS_8_CREDIT_STEP_LOWER_8_START (4)
#define SOC_MDC_AIF_CH_LS_8_CREDIT_STEP_LOWER_8_END (10)
#define SOC_MDC_AIF_CH_LS_8_CREDIT_UTH_LOWER_8_START (12)
#define SOC_MDC_AIF_CH_LS_8_CREDIT_UTH_LOWER_8_END (20)
#define SOC_MDC_AIF_CH_LS_8_CREDIT_LTH_LOWER_8_START (21)
#define SOC_MDC_AIF_CH_LS_8_CREDIT_LTH_LOWER_8_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int axi_sel_9 : 2;
        unsigned int reserved_0 : 6;
        unsigned int credit_mode_9 : 3;
        unsigned int reserved_1 : 1;
        unsigned int max_cont_9 : 4;
        unsigned int reserved_2 : 16;
    } reg;
} soc_mdc_aif_ch_ctl_9_union;
#endif
#define SOC_MDC_AIF_CH_CTL_9_AXI_SEL_9_START (0)
#define SOC_MDC_AIF_CH_CTL_9_AXI_SEL_9_END (1)
#define SOC_MDC_AIF_CH_CTL_9_CREDIT_MODE_9_START (8)
#define SOC_MDC_AIF_CH_CTL_9_CREDIT_MODE_9_END (10)
#define SOC_MDC_AIF_CH_CTL_9_MAX_CONT_9_START (12)
#define SOC_MDC_AIF_CH_CTL_9_MAX_CONT_9_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int credit_en_9 : 1;
        unsigned int reserved_0 : 3;
        unsigned int credit_step_9 : 7;
        unsigned int reserved_1 : 1;
        unsigned int credit_uth_9 : 9;
        unsigned int credit_lth_9 : 9;
        unsigned int reserved_2 : 2;
    } reg;
} soc_mdc_aif_ch_hs_9_union;
#endif
#define SOC_MDC_AIF_CH_HS_9_CREDIT_EN_9_START (0)
#define SOC_MDC_AIF_CH_HS_9_CREDIT_EN_9_END (0)
#define SOC_MDC_AIF_CH_HS_9_CREDIT_STEP_9_START (4)
#define SOC_MDC_AIF_CH_HS_9_CREDIT_STEP_9_END (10)
#define SOC_MDC_AIF_CH_HS_9_CREDIT_UTH_9_START (12)
#define SOC_MDC_AIF_CH_HS_9_CREDIT_UTH_9_END (20)
#define SOC_MDC_AIF_CH_HS_9_CREDIT_LTH_9_START (21)
#define SOC_MDC_AIF_CH_HS_9_CREDIT_LTH_9_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int credit_en_lower_9 : 1;
        unsigned int reserved_0 : 3;
        unsigned int credit_step_lower_9 : 7;
        unsigned int reserved_1 : 1;
        unsigned int credit_uth_lower_9 : 9;
        unsigned int credit_lth_lower_9 : 9;
        unsigned int reserved_2 : 2;
    } reg;
} soc_mdc_aif_ch_ls_9_union;
#endif
#define SOC_MDC_AIF_CH_LS_9_CREDIT_EN_LOWER_9_START (0)
#define SOC_MDC_AIF_CH_LS_9_CREDIT_EN_LOWER_9_END (0)
#define SOC_MDC_AIF_CH_LS_9_CREDIT_STEP_LOWER_9_START (4)
#define SOC_MDC_AIF_CH_LS_9_CREDIT_STEP_LOWER_9_END (10)
#define SOC_MDC_AIF_CH_LS_9_CREDIT_UTH_LOWER_9_START (12)
#define SOC_MDC_AIF_CH_LS_9_CREDIT_UTH_LOWER_9_END (20)
#define SOC_MDC_AIF_CH_LS_9_CREDIT_LTH_LOWER_9_START (21)
#define SOC_MDC_AIF_CH_LS_9_CREDIT_LTH_LOWER_9_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int axi_sel_10 : 2;
        unsigned int reserved_0 : 6;
        unsigned int credit_mode_10 : 3;
        unsigned int reserved_1 : 1;
        unsigned int max_cont_10 : 4;
        unsigned int reserved_2 : 16;
    } reg;
} soc_mdc_aif_ch_ctl_10_union;
#endif
#define SOC_MDC_AIF_CH_CTL_10_AXI_SEL_10_START (0)
#define SOC_MDC_AIF_CH_CTL_10_AXI_SEL_10_END (1)
#define SOC_MDC_AIF_CH_CTL_10_CREDIT_MODE_10_START (8)
#define SOC_MDC_AIF_CH_CTL_10_CREDIT_MODE_10_END (10)
#define SOC_MDC_AIF_CH_CTL_10_MAX_CONT_10_START (12)
#define SOC_MDC_AIF_CH_CTL_10_MAX_CONT_10_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int credit_en_10 : 1;
        unsigned int reserved_0 : 3;
        unsigned int credit_step_10 : 7;
        unsigned int reserved_1 : 1;
        unsigned int credit_uth_10 : 9;
        unsigned int credit_lth_10 : 9;
        unsigned int reserved_2 : 2;
    } reg;
} soc_mdc_aif_ch_hs_10_union;
#endif
#define SOC_MDC_AIF_CH_HS_10_CREDIT_EN_10_START (0)
#define SOC_MDC_AIF_CH_HS_10_CREDIT_EN_10_END (0)
#define SOC_MDC_AIF_CH_HS_10_CREDIT_STEP_10_START (4)
#define SOC_MDC_AIF_CH_HS_10_CREDIT_STEP_10_END (10)
#define SOC_MDC_AIF_CH_HS_10_CREDIT_UTH_10_START (12)
#define SOC_MDC_AIF_CH_HS_10_CREDIT_UTH_10_END (20)
#define SOC_MDC_AIF_CH_HS_10_CREDIT_LTH_10_START (21)
#define SOC_MDC_AIF_CH_HS_10_CREDIT_LTH_10_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int credit_en_lower_10 : 1;
        unsigned int reserved_0 : 3;
        unsigned int credit_step_lower_10 : 7;
        unsigned int reserved_1 : 1;
        unsigned int credit_uth_lower_10 : 9;
        unsigned int credit_lth_lower_10 : 9;
        unsigned int reserved_2 : 2;
    } reg;
} soc_mdc_aif_ch_ls_10_union;
#endif
#define SOC_MDC_AIF_CH_LS_10_CREDIT_EN_LOWER_10_START (0)
#define SOC_MDC_AIF_CH_LS_10_CREDIT_EN_LOWER_10_END (0)
#define SOC_MDC_AIF_CH_LS_10_CREDIT_STEP_LOWER_10_START (4)
#define SOC_MDC_AIF_CH_LS_10_CREDIT_STEP_LOWER_10_END (10)
#define SOC_MDC_AIF_CH_LS_10_CREDIT_UTH_LOWER_10_START (12)
#define SOC_MDC_AIF_CH_LS_10_CREDIT_UTH_LOWER_10_END (20)
#define SOC_MDC_AIF_CH_LS_10_CREDIT_LTH_LOWER_10_START (21)
#define SOC_MDC_AIF_CH_LS_10_CREDIT_LTH_LOWER_10_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int arcache2_addr_thrh : 32;
    } reg;
} soc_mdc_arcach2_addr_thrh_union;
#endif
#define SOC_MDC_ARCACH2_ADDR_THRH_ARCACHE2_ADDR_THRH_START (0)
#define SOC_MDC_ARCACH2_ADDR_THRH_ARCACHE2_ADDR_THRH_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int arcache2_addr_thrl : 32;
    } reg;
} soc_mdc_arcach2_addr_thrl_union;
#endif
#define SOC_MDC_ARCACH2_ADDR_THRL_ARCACHE2_ADDR_THRL_START (0)
#define SOC_MDC_ARCACH2_ADDR_THRL_ARCACHE2_ADDR_THRL_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int arcache2_value0 : 4;
        unsigned int arcache2_addr_thrh_h4bit : 4;
        unsigned int reserved : 24;
    } reg;
} soc_mdc_arcach2_value0_union;
#endif
#define SOC_MDC_ARCACH2_VALUE0_ARCACHE2_VALUE0_START (0)
#define SOC_MDC_ARCACH2_VALUE0_ARCACHE2_VALUE0_END (3)
#define SOC_MDC_ARCACH2_VALUE0_ARCACHE2_ADDR_THRH_H4BIT_START (4)
#define SOC_MDC_ARCACH2_VALUE0_ARCACHE2_ADDR_THRH_H4BIT_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int arcache2_value1 : 4;
        unsigned int arcache2_addr_thrl_h4bit : 4;
        unsigned int reserved : 24;
    } reg;
} soc_mdc_arcach2_value1_union;
#endif
#define SOC_MDC_ARCACH2_VALUE1_ARCACHE2_VALUE1_START (0)
#define SOC_MDC_ARCACH2_VALUE1_ARCACHE2_VALUE1_END (3)
#define SOC_MDC_ARCACH2_VALUE1_ARCACHE2_ADDR_THRL_H4BIT_START (4)
#define SOC_MDC_ARCACH2_VALUE1_ARCACHE2_ADDR_THRL_H4BIT_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int awcache2_addr_thrh : 32;
    } reg;
} soc_mdc_awcach2_addr_thrh_union;
#endif
#define SOC_MDC_AWCACH2_ADDR_THRH_AWCACHE2_ADDR_THRH_START (0)
#define SOC_MDC_AWCACH2_ADDR_THRH_AWCACHE2_ADDR_THRH_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int awcache2_addr_thrl : 32;
    } reg;
} soc_mdc_awcach2_addr_thrl_union;
#endif
#define SOC_MDC_AWCACH2_ADDR_THRL_AWCACHE2_ADDR_THRL_START (0)
#define SOC_MDC_AWCACH2_ADDR_THRL_AWCACHE2_ADDR_THRL_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int awcache2_value0 : 4;
        unsigned int awcache2_addr_thrh_h4bit : 4;
        unsigned int reserved : 24;
    } reg;
} soc_mdc_awcach2_value0_union;
#endif
#define SOC_MDC_AWCACH2_VALUE0_AWCACHE2_VALUE0_START (0)
#define SOC_MDC_AWCACH2_VALUE0_AWCACHE2_VALUE0_END (3)
#define SOC_MDC_AWCACH2_VALUE0_AWCACHE2_ADDR_THRH_H4BIT_START (4)
#define SOC_MDC_AWCACH2_VALUE0_AWCACHE2_ADDR_THRH_H4BIT_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int awcache2_value1 : 4;
        unsigned int awcache2_addr_thrl_h4bit : 4;
        unsigned int reserved : 24;
    } reg;
} soc_mdc_awcach2_value1_union;
#endif
#define SOC_MDC_AWCACH2_VALUE1_AWCACHE2_VALUE1_START (0)
#define SOC_MDC_AWCACH2_VALUE1_AWCACHE2_VALUE1_END (3)
#define SOC_MDC_AWCACH2_VALUE1_AWCACHE2_ADDR_THRL_H4BIT_START (4)
#define SOC_MDC_AWCACH2_VALUE1_AWCACHE2_ADDR_THRL_H4BIT_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int mid_0 : 4;
        unsigned int reserved : 28;
    } reg;
} soc_mdc_aif_ch_mid_0_union;
#endif
#define SOC_MDC_AIF_CH_MID_0_MID_0_START (0)
#define SOC_MDC_AIF_CH_MID_0_MID_0_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int mid_1 : 4;
        unsigned int reserved : 28;
    } reg;
} soc_mdc_aif_ch_mid_1_union;
#endif
#define SOC_MDC_AIF_CH_MID_1_MID_1_START (0)
#define SOC_MDC_AIF_CH_MID_1_MID_1_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int mid_2 : 4;
        unsigned int reserved : 28;
    } reg;
} soc_mdc_aif_ch_mid_2_union;
#endif
#define SOC_MDC_AIF_CH_MID_2_MID_2_START (0)
#define SOC_MDC_AIF_CH_MID_2_MID_2_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int mid_6 : 4;
        unsigned int reserved : 28;
    } reg;
} soc_mdc_aif_ch_mid_6_union;
#endif
#define SOC_MDC_AIF_CH_MID_6_MID_6_START (0)
#define SOC_MDC_AIF_CH_MID_6_MID_6_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int mid_8 : 4;
        unsigned int reserved : 28;
    } reg;
} soc_mdc_aif_ch_mid_8_union;
#endif
#define SOC_MDC_AIF_CH_MID_8_MID_8_START (0)
#define SOC_MDC_AIF_CH_MID_8_MID_8_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int mid_9 : 4;
        unsigned int reserved : 28;
    } reg;
} soc_mdc_aif_ch_mid_9_union;
#endif
#define SOC_MDC_AIF_CH_MID_9_MID_9_START (0)
#define SOC_MDC_AIF_CH_MID_9_MID_9_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int mid_10 : 4;
        unsigned int reserved : 28;
    } reg;
} soc_mdc_aif_ch_mid_10_union;
#endif
#define SOC_MDC_AIF_CH_MID_10_MID_10_START (0)
#define SOC_MDC_AIF_CH_MID_10_MID_10_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int mid_cmd : 4;
        unsigned int reserved : 28;
    } reg;
} soc_mdc_aif_ch_mid_cmd_union;
#endif
#define SOC_MDC_AIF_CH_MID_CMD_MID_CMD_START (0)
#define SOC_MDC_AIF_CH_MID_CMD_MID_CMD_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dss0_mid : 3;
        unsigned int reserved : 29;
    } reg;
} soc_mdc_dss0_mid_union;
#endif
#define SOC_MDC_DSS0_MID_DSS0_MID_START (0)
#define SOC_MDC_DSS0_MID_DSS0_MID_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dss1_mid : 3;
        unsigned int reserved : 29;
    } reg;
} soc_mdc_dss1_mid_union;
#endif
#define SOC_MDC_DSS1_MID_DSS1_MID_START (0)
#define SOC_MDC_DSS1_MID_DSS1_MID_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dss2_mid : 3;
        unsigned int reserved : 29;
    } reg;
} soc_mdc_dss2_mid_union;
#endif
#define SOC_MDC_DSS2_MID_DSS2_MID_START (0)
#define SOC_MDC_DSS2_MID_DSS2_MID_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dss3_mid : 3;
        unsigned int reserved : 29;
    } reg;
} soc_mdc_dss3_mid_union;
#endif
#define SOC_MDC_DSS3_MID_DSS3_MID_START (0)
#define SOC_MDC_DSS3_MID_DSS3_MID_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ch_sid_ns_0 : 8;
        unsigned int reserved_0 : 4;
        unsigned int ch_ssid_ns_0 : 8;
        unsigned int ch_ssidv_ns_0 : 1;
        unsigned int reserved_1 : 3;
        unsigned int ch_sc_hint_ns_0 : 4;
        unsigned int ch_sc_hint_en_ns_0 : 1;
        unsigned int ch_ptl_as_ful_ns_0 : 1;
        unsigned int reserved_2 : 2;
    } reg;
} soc_mdc_mmu_id_attr_ns_0_union;
#endif
#define SOC_MDC_MMU_ID_ATTR_NS_0_CH_SID_NS_0_START (0)
#define SOC_MDC_MMU_ID_ATTR_NS_0_CH_SID_NS_0_END (7)
#define SOC_MDC_MMU_ID_ATTR_NS_0_CH_SSID_NS_0_START (12)
#define SOC_MDC_MMU_ID_ATTR_NS_0_CH_SSID_NS_0_END (19)
#define SOC_MDC_MMU_ID_ATTR_NS_0_CH_SSIDV_NS_0_START (20)
#define SOC_MDC_MMU_ID_ATTR_NS_0_CH_SSIDV_NS_0_END (20)
#define SOC_MDC_MMU_ID_ATTR_NS_0_CH_SC_HINT_NS_0_START (24)
#define SOC_MDC_MMU_ID_ATTR_NS_0_CH_SC_HINT_NS_0_END (27)
#define SOC_MDC_MMU_ID_ATTR_NS_0_CH_SC_HINT_EN_NS_0_START (28)
#define SOC_MDC_MMU_ID_ATTR_NS_0_CH_SC_HINT_EN_NS_0_END (28)
#define SOC_MDC_MMU_ID_ATTR_NS_0_CH_PTL_AS_FUL_NS_0_START (29)
#define SOC_MDC_MMU_ID_ATTR_NS_0_CH_PTL_AS_FUL_NS_0_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ch_sid_ns_1 : 8;
        unsigned int reserved_0 : 4;
        unsigned int ch_ssid_ns_1 : 8;
        unsigned int ch_ssidv_ns_1 : 1;
        unsigned int reserved_1 : 3;
        unsigned int ch_sc_hint_ns_1 : 4;
        unsigned int ch_sc_hint_en_ns_1 : 1;
        unsigned int ch_ptl_as_ful_ns_1 : 1;
        unsigned int reserved_2 : 2;
    } reg;
} soc_mdc_mmu_id_attr_ns_1_union;
#endif
#define SOC_MDC_MMU_ID_ATTR_NS_1_CH_SID_NS_1_START (0)
#define SOC_MDC_MMU_ID_ATTR_NS_1_CH_SID_NS_1_END (7)
#define SOC_MDC_MMU_ID_ATTR_NS_1_CH_SSID_NS_1_START (12)
#define SOC_MDC_MMU_ID_ATTR_NS_1_CH_SSID_NS_1_END (19)
#define SOC_MDC_MMU_ID_ATTR_NS_1_CH_SSIDV_NS_1_START (20)
#define SOC_MDC_MMU_ID_ATTR_NS_1_CH_SSIDV_NS_1_END (20)
#define SOC_MDC_MMU_ID_ATTR_NS_1_CH_SC_HINT_NS_1_START (24)
#define SOC_MDC_MMU_ID_ATTR_NS_1_CH_SC_HINT_NS_1_END (27)
#define SOC_MDC_MMU_ID_ATTR_NS_1_CH_SC_HINT_EN_NS_1_START (28)
#define SOC_MDC_MMU_ID_ATTR_NS_1_CH_SC_HINT_EN_NS_1_END (28)
#define SOC_MDC_MMU_ID_ATTR_NS_1_CH_PTL_AS_FUL_NS_1_START (29)
#define SOC_MDC_MMU_ID_ATTR_NS_1_CH_PTL_AS_FUL_NS_1_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ch_sid_ns_2 : 8;
        unsigned int reserved_0 : 4;
        unsigned int ch_ssid_ns_2 : 8;
        unsigned int ch_ssidv_ns_2 : 1;
        unsigned int reserved_1 : 3;
        unsigned int ch_sc_hint_ns_2 : 4;
        unsigned int ch_sc_hint_en_ns_2 : 1;
        unsigned int ch_ptl_as_ful_ns_2 : 1;
        unsigned int reserved_2 : 2;
    } reg;
} soc_mdc_mmu_id_attr_ns_2_union;
#endif
#define SOC_MDC_MMU_ID_ATTR_NS_2_CH_SID_NS_2_START (0)
#define SOC_MDC_MMU_ID_ATTR_NS_2_CH_SID_NS_2_END (7)
#define SOC_MDC_MMU_ID_ATTR_NS_2_CH_SSID_NS_2_START (12)
#define SOC_MDC_MMU_ID_ATTR_NS_2_CH_SSID_NS_2_END (19)
#define SOC_MDC_MMU_ID_ATTR_NS_2_CH_SSIDV_NS_2_START (20)
#define SOC_MDC_MMU_ID_ATTR_NS_2_CH_SSIDV_NS_2_END (20)
#define SOC_MDC_MMU_ID_ATTR_NS_2_CH_SC_HINT_NS_2_START (24)
#define SOC_MDC_MMU_ID_ATTR_NS_2_CH_SC_HINT_NS_2_END (27)
#define SOC_MDC_MMU_ID_ATTR_NS_2_CH_SC_HINT_EN_NS_2_START (28)
#define SOC_MDC_MMU_ID_ATTR_NS_2_CH_SC_HINT_EN_NS_2_END (28)
#define SOC_MDC_MMU_ID_ATTR_NS_2_CH_PTL_AS_FUL_NS_2_START (29)
#define SOC_MDC_MMU_ID_ATTR_NS_2_CH_PTL_AS_FUL_NS_2_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ch_sid_ns_6 : 8;
        unsigned int reserved_0 : 4;
        unsigned int ch_ssid_ns_6 : 8;
        unsigned int ch_ssidv_ns_6 : 1;
        unsigned int reserved_1 : 3;
        unsigned int ch_sc_hint_ns_6 : 4;
        unsigned int ch_sc_hint_en_ns_6 : 1;
        unsigned int ch_ptl_as_ful_ns_6 : 1;
        unsigned int reserved_2 : 2;
    } reg;
} soc_mdc_mmu_id_attr_ns_6_union;
#endif
#define SOC_MDC_MMU_ID_ATTR_NS_6_CH_SID_NS_6_START (0)
#define SOC_MDC_MMU_ID_ATTR_NS_6_CH_SID_NS_6_END (7)
#define SOC_MDC_MMU_ID_ATTR_NS_6_CH_SSID_NS_6_START (12)
#define SOC_MDC_MMU_ID_ATTR_NS_6_CH_SSID_NS_6_END (19)
#define SOC_MDC_MMU_ID_ATTR_NS_6_CH_SSIDV_NS_6_START (20)
#define SOC_MDC_MMU_ID_ATTR_NS_6_CH_SSIDV_NS_6_END (20)
#define SOC_MDC_MMU_ID_ATTR_NS_6_CH_SC_HINT_NS_6_START (24)
#define SOC_MDC_MMU_ID_ATTR_NS_6_CH_SC_HINT_NS_6_END (27)
#define SOC_MDC_MMU_ID_ATTR_NS_6_CH_SC_HINT_EN_NS_6_START (28)
#define SOC_MDC_MMU_ID_ATTR_NS_6_CH_SC_HINT_EN_NS_6_END (28)
#define SOC_MDC_MMU_ID_ATTR_NS_6_CH_PTL_AS_FUL_NS_6_START (29)
#define SOC_MDC_MMU_ID_ATTR_NS_6_CH_PTL_AS_FUL_NS_6_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ch_sid_ns_8 : 8;
        unsigned int reserved_0 : 4;
        unsigned int ch_ssid_ns_8 : 8;
        unsigned int ch_ssidv_ns_8 : 1;
        unsigned int reserved_1 : 3;
        unsigned int ch_sc_hint_ns_8 : 4;
        unsigned int ch_sc_hint_en_ns_8 : 1;
        unsigned int ch_ptl_as_ful_ns_8 : 1;
        unsigned int reserved_2 : 2;
    } reg;
} soc_mdc_mmu_id_attr_ns_8_union;
#endif
#define SOC_MDC_MMU_ID_ATTR_NS_8_CH_SID_NS_8_START (0)
#define SOC_MDC_MMU_ID_ATTR_NS_8_CH_SID_NS_8_END (7)
#define SOC_MDC_MMU_ID_ATTR_NS_8_CH_SSID_NS_8_START (12)
#define SOC_MDC_MMU_ID_ATTR_NS_8_CH_SSID_NS_8_END (19)
#define SOC_MDC_MMU_ID_ATTR_NS_8_CH_SSIDV_NS_8_START (20)
#define SOC_MDC_MMU_ID_ATTR_NS_8_CH_SSIDV_NS_8_END (20)
#define SOC_MDC_MMU_ID_ATTR_NS_8_CH_SC_HINT_NS_8_START (24)
#define SOC_MDC_MMU_ID_ATTR_NS_8_CH_SC_HINT_NS_8_END (27)
#define SOC_MDC_MMU_ID_ATTR_NS_8_CH_SC_HINT_EN_NS_8_START (28)
#define SOC_MDC_MMU_ID_ATTR_NS_8_CH_SC_HINT_EN_NS_8_END (28)
#define SOC_MDC_MMU_ID_ATTR_NS_8_CH_PTL_AS_FUL_NS_8_START (29)
#define SOC_MDC_MMU_ID_ATTR_NS_8_CH_PTL_AS_FUL_NS_8_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ch_sid_ns_9 : 8;
        unsigned int reserved_0 : 4;
        unsigned int ch_ssid_ns_9 : 8;
        unsigned int ch_ssidv_ns_9 : 1;
        unsigned int reserved_1 : 3;
        unsigned int ch_sc_hint_ns_9 : 4;
        unsigned int ch_sc_hint_en_ns_9 : 1;
        unsigned int ch_ptl_as_ful_ns_9 : 1;
        unsigned int reserved_2 : 2;
    } reg;
} soc_mdc_mmu_id_attr_ns_9_union;
#endif
#define SOC_MDC_MMU_ID_ATTR_NS_9_CH_SID_NS_9_START (0)
#define SOC_MDC_MMU_ID_ATTR_NS_9_CH_SID_NS_9_END (7)
#define SOC_MDC_MMU_ID_ATTR_NS_9_CH_SSID_NS_9_START (12)
#define SOC_MDC_MMU_ID_ATTR_NS_9_CH_SSID_NS_9_END (19)
#define SOC_MDC_MMU_ID_ATTR_NS_9_CH_SSIDV_NS_9_START (20)
#define SOC_MDC_MMU_ID_ATTR_NS_9_CH_SSIDV_NS_9_END (20)
#define SOC_MDC_MMU_ID_ATTR_NS_9_CH_SC_HINT_NS_9_START (24)
#define SOC_MDC_MMU_ID_ATTR_NS_9_CH_SC_HINT_NS_9_END (27)
#define SOC_MDC_MMU_ID_ATTR_NS_9_CH_SC_HINT_EN_NS_9_START (28)
#define SOC_MDC_MMU_ID_ATTR_NS_9_CH_SC_HINT_EN_NS_9_END (28)
#define SOC_MDC_MMU_ID_ATTR_NS_9_CH_PTL_AS_FUL_NS_9_START (29)
#define SOC_MDC_MMU_ID_ATTR_NS_9_CH_PTL_AS_FUL_NS_9_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ch_sid_ns_10 : 8;
        unsigned int reserved_0 : 4;
        unsigned int ch_ssid_ns_10 : 8;
        unsigned int ch_ssidv_ns_10 : 1;
        unsigned int reserved_1 : 3;
        unsigned int ch_sc_hint_ns_10 : 4;
        unsigned int ch_sc_hint_en_ns_10 : 1;
        unsigned int ch_ptl_as_ful_ns_10 : 1;
        unsigned int reserved_2 : 2;
    } reg;
} soc_mdc_mmu_id_attr_ns_10_union;
#endif
#define SOC_MDC_MMU_ID_ATTR_NS_10_CH_SID_NS_10_START (0)
#define SOC_MDC_MMU_ID_ATTR_NS_10_CH_SID_NS_10_END (7)
#define SOC_MDC_MMU_ID_ATTR_NS_10_CH_SSID_NS_10_START (12)
#define SOC_MDC_MMU_ID_ATTR_NS_10_CH_SSID_NS_10_END (19)
#define SOC_MDC_MMU_ID_ATTR_NS_10_CH_SSIDV_NS_10_START (20)
#define SOC_MDC_MMU_ID_ATTR_NS_10_CH_SSIDV_NS_10_END (20)
#define SOC_MDC_MMU_ID_ATTR_NS_10_CH_SC_HINT_NS_10_START (24)
#define SOC_MDC_MMU_ID_ATTR_NS_10_CH_SC_HINT_NS_10_END (27)
#define SOC_MDC_MMU_ID_ATTR_NS_10_CH_SC_HINT_EN_NS_10_START (28)
#define SOC_MDC_MMU_ID_ATTR_NS_10_CH_SC_HINT_EN_NS_10_END (28)
#define SOC_MDC_MMU_ID_ATTR_NS_10_CH_PTL_AS_FUL_NS_10_START (29)
#define SOC_MDC_MMU_ID_ATTR_NS_10_CH_PTL_AS_FUL_NS_10_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ch_sid_ns_13 : 8;
        unsigned int reserved_0 : 4;
        unsigned int ch_ssid_ns_13 : 8;
        unsigned int ch_ssidv_ns_13 : 1;
        unsigned int reserved_1 : 3;
        unsigned int ch_sc_hint_ns_13 : 4;
        unsigned int ch_sc_hint_en_ns_13 : 1;
        unsigned int ch_ptl_as_ful_ns_13 : 1;
        unsigned int reserved_2 : 2;
    } reg;
} soc_mdc_mmu_id_attr_ns_13_union;
#endif
#define SOC_MDC_MMU_ID_ATTR_NS_13_CH_SID_NS_13_START (0)
#define SOC_MDC_MMU_ID_ATTR_NS_13_CH_SID_NS_13_END (7)
#define SOC_MDC_MMU_ID_ATTR_NS_13_CH_SSID_NS_13_START (12)
#define SOC_MDC_MMU_ID_ATTR_NS_13_CH_SSID_NS_13_END (19)
#define SOC_MDC_MMU_ID_ATTR_NS_13_CH_SSIDV_NS_13_START (20)
#define SOC_MDC_MMU_ID_ATTR_NS_13_CH_SSIDV_NS_13_END (20)
#define SOC_MDC_MMU_ID_ATTR_NS_13_CH_SC_HINT_NS_13_START (24)
#define SOC_MDC_MMU_ID_ATTR_NS_13_CH_SC_HINT_NS_13_END (27)
#define SOC_MDC_MMU_ID_ATTR_NS_13_CH_SC_HINT_EN_NS_13_START (28)
#define SOC_MDC_MMU_ID_ATTR_NS_13_CH_SC_HINT_EN_NS_13_END (28)
#define SOC_MDC_MMU_ID_ATTR_NS_13_CH_PTL_AS_FUL_NS_13_START (29)
#define SOC_MDC_MMU_ID_ATTR_NS_13_CH_PTL_AS_FUL_NS_13_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int axi2_rid_msk0 : 32;
    } reg;
} soc_mdc_axi2_rid_msk0_union;
#endif
#define SOC_MDC_AXI2_RID_MSK0_AXI2_RID_MSK0_START (0)
#define SOC_MDC_AXI2_RID_MSK0_AXI2_RID_MSK0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int axi2_rid_msk1 : 32;
    } reg;
} soc_mdc_axi2_rid_msk1_union;
#endif
#define SOC_MDC_AXI2_RID_MSK1_AXI2_RID_MSK1_START (0)
#define SOC_MDC_AXI2_RID_MSK1_AXI2_RID_MSK1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int axi2_wid_msk : 32;
    } reg;
} soc_mdc_axi2_wid_msk_union;
#endif
#define SOC_MDC_AXI2_WID_MSK_AXI2_WID_MSK_START (0)
#define SOC_MDC_AXI2_WID_MSK_AXI2_WID_MSK_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int axi2_rd_qos_map : 8;
        unsigned int axi2_wr_qos_map : 8;
        unsigned int axi2_rd_prs_map : 8;
        unsigned int axi2_wr_prs_map : 8;
    } reg;
} soc_mdc_axi2_r_qos_map_union;
#endif
#define SOC_MDC_AXI2_R_QOS_MAP_AXI2_RD_QOS_MAP_START (0)
#define SOC_MDC_AXI2_R_QOS_MAP_AXI2_RD_QOS_MAP_END (7)
#define SOC_MDC_AXI2_R_QOS_MAP_AXI2_WR_QOS_MAP_START (8)
#define SOC_MDC_AXI2_R_QOS_MAP_AXI2_WR_QOS_MAP_END (15)
#define SOC_MDC_AXI2_R_QOS_MAP_AXI2_RD_PRS_MAP_START (16)
#define SOC_MDC_AXI2_R_QOS_MAP_AXI2_RD_PRS_MAP_END (23)
#define SOC_MDC_AXI2_R_QOS_MAP_AXI2_WR_PRS_MAP_START (24)
#define SOC_MDC_AXI2_R_QOS_MAP_AXI2_WR_PRS_MAP_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int aif_clk_sel0 : 32;
    } reg;
} soc_mdc_aif_clk_sel0_union;
#endif
#define SOC_MDC_AIF_CLK_SEL0_AIF_CLK_SEL0_START (0)
#define SOC_MDC_AIF_CLK_SEL0_AIF_CLK_SEL0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int aif_clk_sel1 : 32;
    } reg;
} soc_mdc_aif_clk_sel1_union;
#endif
#define SOC_MDC_AIF_CLK_SEL1_AIF_CLK_SEL1_START (0)
#define SOC_MDC_AIF_CLK_SEL1_AIF_CLK_SEL1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int aif_clk_en0 : 32;
    } reg;
} soc_mdc_aif_clk_en0_union;
#endif
#define SOC_MDC_AIF_CLK_EN0_AIF_CLK_EN0_START (0)
#define SOC_MDC_AIF_CLK_EN0_AIF_CLK_EN0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int aif_clk_en1 : 32;
    } reg;
} soc_mdc_aif_clk_en1_union;
#endif
#define SOC_MDC_AIF_CLK_EN1_AIF_CLK_EN1_START (0)
#define SOC_MDC_AIF_CLK_EN1_AIF_CLK_EN1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int axi_sel_cmd : 2;
        unsigned int reserved : 30;
    } reg;
} soc_mdc_aif_ch_ctl_cmd_union;
#endif
#define SOC_MDC_AIF_CH_CTL_CMD_AXI_SEL_CMD_START (0)
#define SOC_MDC_AIF_CH_CTL_CMD_AXI_SEL_CMD_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int monitor_ini : 32;
    } reg;
} soc_mdc_monitor_timer_ini_union;
#endif
#define SOC_MDC_MONITOR_TIMER_INI_MONITOR_INI_START (0)
#define SOC_MDC_MONITOR_TIMER_INI_MONITOR_INI_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 4;
        unsigned int debug_base : 28;
    } reg;
} soc_mdc_debug_buf_base_union;
#endif
#define SOC_MDC_DEBUG_BUF_BASE_DEBUG_BASE_START (4)
#define SOC_MDC_DEBUG_BUF_BASE_DEBUG_BASE_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int qos_mode : 1;
        unsigned int flux_mode : 1;
        unsigned int credit_th_sel : 1;
        unsigned int addr_protect_en : 1;
        unsigned int reserved : 12;
        unsigned int debug_depth : 16;
    } reg;
} soc_mdc_debug_ctrl_union;
#endif
#define SOC_MDC_DEBUG_CTRL_QOS_MODE_START (0)
#define SOC_MDC_DEBUG_CTRL_QOS_MODE_END (0)
#define SOC_MDC_DEBUG_CTRL_FLUX_MODE_START (1)
#define SOC_MDC_DEBUG_CTRL_FLUX_MODE_END (1)
#define SOC_MDC_DEBUG_CTRL_CREDIT_TH_SEL_START (2)
#define SOC_MDC_DEBUG_CTRL_CREDIT_TH_SEL_END (2)
#define SOC_MDC_DEBUG_CTRL_ADDR_PROTECT_EN_START (3)
#define SOC_MDC_DEBUG_CTRL_ADDR_PROTECT_EN_END (3)
#define SOC_MDC_DEBUG_CTRL_DEBUG_DEPTH_START (16)
#define SOC_MDC_DEBUG_CTRL_DEBUG_DEPTH_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int aif_rd_shadow : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_aif_shadow_read_union;
#endif
#define SOC_MDC_AIF_SHADOW_READ_AIF_RD_SHADOW_START (0)
#define SOC_MDC_AIF_SHADOW_READ_AIF_RD_SHADOW_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int mem_ctrl : 16;
        unsigned int reserved : 16;
    } reg;
} soc_mdc_aif_mem_ctrl_union;
#endif
#define SOC_MDC_AIF_MEM_CTRL_MEM_CTRL_START (0)
#define SOC_MDC_AIF_MEM_CTRL_MEM_CTRL_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int monitor_en : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_monitor_en_union;
#endif
#define SOC_MDC_MONITOR_EN_MONITOR_EN_START (0)
#define SOC_MDC_MONITOR_EN_MONITOR_EN_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int mon_axi_sel_w : 2;
        unsigned int mon_axi_sel_r : 2;
        unsigned int sample_mode : 2;
        unsigned int reserved_0 : 2;
        unsigned int sample_cmd_type : 2;
        unsigned int reserved_1 : 2;
        unsigned int sample_mid : 5;
        unsigned int reserved_2 : 15;
    } reg;
} soc_mdc_monitor_ctrl_union;
#endif
#define SOC_MDC_MONITOR_CTRL_MON_AXI_SEL_W_START (0)
#define SOC_MDC_MONITOR_CTRL_MON_AXI_SEL_W_END (1)
#define SOC_MDC_MONITOR_CTRL_MON_AXI_SEL_R_START (2)
#define SOC_MDC_MONITOR_CTRL_MON_AXI_SEL_R_END (3)
#define SOC_MDC_MONITOR_CTRL_SAMPLE_MODE_START (4)
#define SOC_MDC_MONITOR_CTRL_SAMPLE_MODE_END (5)
#define SOC_MDC_MONITOR_CTRL_SAMPLE_CMD_TYPE_START (8)
#define SOC_MDC_MONITOR_CTRL_SAMPLE_CMD_TYPE_END (9)
#define SOC_MDC_MONITOR_CTRL_SAMPLE_MID_START (12)
#define SOC_MDC_MONITOR_CTRL_SAMPLE_MID_END (16)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int window_end_num : 16;
        unsigned int reserved : 16;
    } reg;
} soc_mdc_monitor_sample_mun_union;
#endif
#define SOC_MDC_MONITOR_SAMPLE_MUN_WINDOW_END_NUM_START (0)
#define SOC_MDC_MONITOR_SAMPLE_MUN_WINDOW_END_NUM_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int window_end_time : 32;
    } reg;
} soc_mdc_monitor_sample_time_union;
#endif
#define SOC_MDC_MONITOR_SAMPLE_TIME_WINDOW_END_TIME_START (0)
#define SOC_MDC_MONITOR_SAMPLE_TIME_WINDOW_END_TIME_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int window_end_flow : 32;
    } reg;
} soc_mdc_monitor_sample_flow_union;
#endif
#define SOC_MDC_MONITOR_SAMPLE_FLOW_WINDOW_END_FLOW_START (0)
#define SOC_MDC_MONITOR_SAMPLE_FLOW_WINDOW_END_FLOW_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int module_clk_sel : 32;
    } reg;
} soc_mdc_aix_module_clk_sel_union;
#endif
#define SOC_MDC_AIX_MODULE_CLK_SEL_MODULE_CLK_SEL_START (0)
#define SOC_MDC_AIX_MODULE_CLK_SEL_MODULE_CLK_SEL_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int module_clk_en : 32;
    } reg;
} soc_mdc_aix_module_clk_en_union;
#endif
#define SOC_MDC_AIX_MODULE_CLK_EN_MODULE_CLK_EN_START (0)
#define SOC_MDC_AIX_MODULE_CLK_EN_MODULE_CLK_EN_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int clk_en : 1;
        unsigned int clk_sel : 1;
        unsigned int reserved : 30;
    } reg;
} soc_mdc_mif_clk_ctl_union;
#endif
#define SOC_MDC_MIF_CLK_CTL_CLK_EN_START (0)
#define SOC_MDC_MIF_CLK_CTL_CLK_EN_END (0)
#define SOC_MDC_MIF_CLK_CTL_CLK_SEL_START (1)
#define SOC_MDC_MIF_CLK_CTL_CLK_SEL_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int mif_mem_ctrl : 4;
        unsigned int reserved : 28;
    } reg;
} soc_mdc_mif_mem_ctrl_union;
#endif
#define SOC_MDC_MIF_MEM_CTRL_MIF_MEM_CTRL_START (0)
#define SOC_MDC_MIF_MEM_CTRL_MIF_MEM_CTRL_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int mif_rd_shadow : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_mif_shadow_read_union;
#endif
#define SOC_MDC_MIF_SHADOW_READ_MIF_RD_SHADOW_START (0)
#define SOC_MDC_MIF_SHADOW_READ_MIF_RD_SHADOW_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int aif_cmd_reload : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_aif_cmd_reload_union;
#endif
#define SOC_MDC_AIF_CMD_RELOAD_AIF_CMD_RELOAD_START (0)
#define SOC_MDC_AIF_CMD_RELOAD_AIF_CMD_RELOAD_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pref_en_r0 : 1;
        unsigned int lock_en_r0 : 1;
        unsigned int delock_type_r0 : 1;
        unsigned int reserved : 29;
    } reg;
} soc_mdc_mif_ctrl_d0_union;
#endif
#define SOC_MDC_MIF_CTRL_D0_PREF_EN_R0_START (0)
#define SOC_MDC_MIF_CTRL_D0_PREF_EN_R0_END (0)
#define SOC_MDC_MIF_CTRL_D0_LOCK_EN_R0_START (1)
#define SOC_MDC_MIF_CTRL_D0_LOCK_EN_R0_END (1)
#define SOC_MDC_MIF_CTRL_D0_DELOCK_TYPE_R0_START (2)
#define SOC_MDC_MIF_CTRL_D0_DELOCK_TYPE_R0_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sub0_lead_num_r0 : 8;
        unsigned int sub1_lead_num_r0 : 8;
        unsigned int sub2_lead_num_r0 : 8;
        unsigned int sub3_lead_num_r0 : 8;
    } reg;
} soc_mdc_mif_lead_ctrl_d0_union;
#endif
#define SOC_MDC_MIF_LEAD_CTRL_D0_SUB0_LEAD_NUM_R0_START (0)
#define SOC_MDC_MIF_LEAD_CTRL_D0_SUB0_LEAD_NUM_R0_END (7)
#define SOC_MDC_MIF_LEAD_CTRL_D0_SUB1_LEAD_NUM_R0_START (8)
#define SOC_MDC_MIF_LEAD_CTRL_D0_SUB1_LEAD_NUM_R0_END (15)
#define SOC_MDC_MIF_LEAD_CTRL_D0_SUB2_LEAD_NUM_R0_START (16)
#define SOC_MDC_MIF_LEAD_CTRL_D0_SUB2_LEAD_NUM_R0_END (23)
#define SOC_MDC_MIF_LEAD_CTRL_D0_SUB3_LEAD_NUM_R0_START (24)
#define SOC_MDC_MIF_LEAD_CTRL_D0_SUB3_LEAD_NUM_R0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sub0_os_num_r0 : 8;
        unsigned int sub1_os_num_r0 : 8;
        unsigned int sub2_os_num_r0 : 8;
        unsigned int sub3_os_num_r0 : 8;
    } reg;
} soc_mdc_mif_os_ctrl_d0_union;
#endif
#define SOC_MDC_MIF_OS_CTRL_D0_SUB0_OS_NUM_R0_START (0)
#define SOC_MDC_MIF_OS_CTRL_D0_SUB0_OS_NUM_R0_END (7)
#define SOC_MDC_MIF_OS_CTRL_D0_SUB1_OS_NUM_R0_START (8)
#define SOC_MDC_MIF_OS_CTRL_D0_SUB1_OS_NUM_R0_END (15)
#define SOC_MDC_MIF_OS_CTRL_D0_SUB2_OS_NUM_R0_START (16)
#define SOC_MDC_MIF_OS_CTRL_D0_SUB2_OS_NUM_R0_END (23)
#define SOC_MDC_MIF_OS_CTRL_D0_SUB3_OS_NUM_R0_START (24)
#define SOC_MDC_MIF_OS_CTRL_D0_SUB3_OS_NUM_R0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sub0_pref_cnt_r0 : 8;
        unsigned int sub1_pref_cnt_r0 : 8;
        unsigned int sub2_pref_cnt_r0 : 8;
        unsigned int sub3_pref_cnt_r0 : 8;
    } reg;
} soc_mdc_mif_stat_d0_union;
#endif
#define SOC_MDC_MIF_STAT_D0_SUB0_PREF_CNT_R0_START (0)
#define SOC_MDC_MIF_STAT_D0_SUB0_PREF_CNT_R0_END (7)
#define SOC_MDC_MIF_STAT_D0_SUB1_PREF_CNT_R0_START (8)
#define SOC_MDC_MIF_STAT_D0_SUB1_PREF_CNT_R0_END (15)
#define SOC_MDC_MIF_STAT_D0_SUB2_PREF_CNT_R0_START (16)
#define SOC_MDC_MIF_STAT_D0_SUB2_PREF_CNT_R0_END (23)
#define SOC_MDC_MIF_STAT_D0_SUB3_PREF_CNT_R0_START (24)
#define SOC_MDC_MIF_STAT_D0_SUB3_PREF_CNT_R0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pref_en_r1 : 1;
        unsigned int lock_en_r1 : 1;
        unsigned int delock_type_r1 : 1;
        unsigned int reserved : 29;
    } reg;
} soc_mdc_mif_ctrl_d1_union;
#endif
#define SOC_MDC_MIF_CTRL_D1_PREF_EN_R1_START (0)
#define SOC_MDC_MIF_CTRL_D1_PREF_EN_R1_END (0)
#define SOC_MDC_MIF_CTRL_D1_LOCK_EN_R1_START (1)
#define SOC_MDC_MIF_CTRL_D1_LOCK_EN_R1_END (1)
#define SOC_MDC_MIF_CTRL_D1_DELOCK_TYPE_R1_START (2)
#define SOC_MDC_MIF_CTRL_D1_DELOCK_TYPE_R1_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sub3_lead_num_r1 : 8;
        unsigned int reserved : 24;
    } reg;
} soc_mdc_mif_lead_ctrl_d1_union;
#endif
#define SOC_MDC_MIF_LEAD_CTRL_D1_SUB3_LEAD_NUM_R1_START (0)
#define SOC_MDC_MIF_LEAD_CTRL_D1_SUB3_LEAD_NUM_R1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sub3_os_num_r1 : 8;
        unsigned int reserved : 24;
    } reg;
} soc_mdc_mif_os_ctrl_d1_union;
#endif
#define SOC_MDC_MIF_OS_CTRL_D1_SUB3_OS_NUM_R1_START (0)
#define SOC_MDC_MIF_OS_CTRL_D1_SUB3_OS_NUM_R1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sub3_pref_cnt_r1 : 8;
        unsigned int reserved : 24;
    } reg;
} soc_mdc_mif_stat_d1_union;
#endif
#define SOC_MDC_MIF_STAT_D1_SUB3_PREF_CNT_R1_START (0)
#define SOC_MDC_MIF_STAT_D1_SUB3_PREF_CNT_R1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pref_en_r2 : 1;
        unsigned int lock_en_r2 : 1;
        unsigned int delock_type_r2 : 1;
        unsigned int reserved : 29;
    } reg;
} soc_mdc_mif_ctrl_v0_union;
#endif
#define SOC_MDC_MIF_CTRL_V0_PREF_EN_R2_START (0)
#define SOC_MDC_MIF_CTRL_V0_PREF_EN_R2_END (0)
#define SOC_MDC_MIF_CTRL_V0_LOCK_EN_R2_START (1)
#define SOC_MDC_MIF_CTRL_V0_LOCK_EN_R2_END (1)
#define SOC_MDC_MIF_CTRL_V0_DELOCK_TYPE_R2_START (2)
#define SOC_MDC_MIF_CTRL_V0_DELOCK_TYPE_R2_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sub0_lead_num_r2 : 8;
        unsigned int sub1_lead_num_r2 : 8;
        unsigned int sub2_lead_num_r2 : 8;
        unsigned int sub3_lead_num_r2 : 8;
    } reg;
} soc_mdc_mif_lead_ctrl_v0_union;
#endif
#define SOC_MDC_MIF_LEAD_CTRL_V0_SUB0_LEAD_NUM_R2_START (0)
#define SOC_MDC_MIF_LEAD_CTRL_V0_SUB0_LEAD_NUM_R2_END (7)
#define SOC_MDC_MIF_LEAD_CTRL_V0_SUB1_LEAD_NUM_R2_START (8)
#define SOC_MDC_MIF_LEAD_CTRL_V0_SUB1_LEAD_NUM_R2_END (15)
#define SOC_MDC_MIF_LEAD_CTRL_V0_SUB2_LEAD_NUM_R2_START (16)
#define SOC_MDC_MIF_LEAD_CTRL_V0_SUB2_LEAD_NUM_R2_END (23)
#define SOC_MDC_MIF_LEAD_CTRL_V0_SUB3_LEAD_NUM_R2_START (24)
#define SOC_MDC_MIF_LEAD_CTRL_V0_SUB3_LEAD_NUM_R2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sub0_os_num_r2 : 8;
        unsigned int sub1_os_num_r2 : 8;
        unsigned int sub2_os_num_r2 : 8;
        unsigned int sub3_os_num_r2 : 8;
    } reg;
} soc_mdc_mif_os_ctrl_v0_union;
#endif
#define SOC_MDC_MIF_OS_CTRL_V0_SUB0_OS_NUM_R2_START (0)
#define SOC_MDC_MIF_OS_CTRL_V0_SUB0_OS_NUM_R2_END (7)
#define SOC_MDC_MIF_OS_CTRL_V0_SUB1_OS_NUM_R2_START (8)
#define SOC_MDC_MIF_OS_CTRL_V0_SUB1_OS_NUM_R2_END (15)
#define SOC_MDC_MIF_OS_CTRL_V0_SUB2_OS_NUM_R2_START (16)
#define SOC_MDC_MIF_OS_CTRL_V0_SUB2_OS_NUM_R2_END (23)
#define SOC_MDC_MIF_OS_CTRL_V0_SUB3_OS_NUM_R2_START (24)
#define SOC_MDC_MIF_OS_CTRL_V0_SUB3_OS_NUM_R2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sub0_pref_cnt_r2 : 8;
        unsigned int sub1_pref_cnt_r2 : 8;
        unsigned int sub2_pref_cnt_r2 : 8;
        unsigned int sub3_pref_cnt_r2 : 8;
    } reg;
} soc_mdc_mif_stat_v0_union;
#endif
#define SOC_MDC_MIF_STAT_V0_SUB0_PREF_CNT_R2_START (0)
#define SOC_MDC_MIF_STAT_V0_SUB0_PREF_CNT_R2_END (7)
#define SOC_MDC_MIF_STAT_V0_SUB1_PREF_CNT_R2_START (8)
#define SOC_MDC_MIF_STAT_V0_SUB1_PREF_CNT_R2_END (15)
#define SOC_MDC_MIF_STAT_V0_SUB2_PREF_CNT_R2_START (16)
#define SOC_MDC_MIF_STAT_V0_SUB2_PREF_CNT_R2_END (23)
#define SOC_MDC_MIF_STAT_V0_SUB3_PREF_CNT_R2_START (24)
#define SOC_MDC_MIF_STAT_V0_SUB3_PREF_CNT_R2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pref_en_r6 : 1;
        unsigned int lock_en_r6 : 1;
        unsigned int delock_type_r6 : 1;
        unsigned int reserved : 29;
    } reg;
} soc_mdc_mif_ctrl_d2_union;
#endif
#define SOC_MDC_MIF_CTRL_D2_PREF_EN_R6_START (0)
#define SOC_MDC_MIF_CTRL_D2_PREF_EN_R6_END (0)
#define SOC_MDC_MIF_CTRL_D2_LOCK_EN_R6_START (1)
#define SOC_MDC_MIF_CTRL_D2_LOCK_EN_R6_END (1)
#define SOC_MDC_MIF_CTRL_D2_DELOCK_TYPE_R6_START (2)
#define SOC_MDC_MIF_CTRL_D2_DELOCK_TYPE_R6_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sub3_lead_num_r6 : 8;
        unsigned int reserved : 24;
    } reg;
} soc_mdc_mif_lead_ctrl_d2_union;
#endif
#define SOC_MDC_MIF_LEAD_CTRL_D2_SUB3_LEAD_NUM_R6_START (0)
#define SOC_MDC_MIF_LEAD_CTRL_D2_SUB3_LEAD_NUM_R6_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sub3_os_num_r6 : 8;
        unsigned int reserved : 24;
    } reg;
} soc_mdc_mif_os_ctrl_d2_union;
#endif
#define SOC_MDC_MIF_OS_CTRL_D2_SUB3_OS_NUM_R6_START (0)
#define SOC_MDC_MIF_OS_CTRL_D2_SUB3_OS_NUM_R6_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sub3_pref_cnt_r6 : 8;
        unsigned int reserved : 24;
    } reg;
} soc_mdc_mif_stat_d2_union;
#endif
#define SOC_MDC_MIF_STAT_D2_SUB3_PREF_CNT_R6_START (0)
#define SOC_MDC_MIF_STAT_D2_SUB3_PREF_CNT_R6_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pref_en_w0 : 1;
        unsigned int lock_en_w0 : 1;
        unsigned int delock_type_w0 : 1;
        unsigned int reserved : 29;
    } reg;
} soc_mdc_mif_ctrl_w0_union;
#endif
#define SOC_MDC_MIF_CTRL_W0_PREF_EN_W0_START (0)
#define SOC_MDC_MIF_CTRL_W0_PREF_EN_W0_END (0)
#define SOC_MDC_MIF_CTRL_W0_LOCK_EN_W0_START (1)
#define SOC_MDC_MIF_CTRL_W0_LOCK_EN_W0_END (1)
#define SOC_MDC_MIF_CTRL_W0_DELOCK_TYPE_W0_START (2)
#define SOC_MDC_MIF_CTRL_W0_DELOCK_TYPE_W0_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sub0_lead_num_w0 : 8;
        unsigned int sub1_lead_num_w0 : 8;
        unsigned int sub2_lead_num_w0 : 8;
        unsigned int reserved : 8;
    } reg;
} soc_mdc_mif_lead_ctrl_w0_union;
#endif
#define SOC_MDC_MIF_LEAD_CTRL_W0_SUB0_LEAD_NUM_W0_START (0)
#define SOC_MDC_MIF_LEAD_CTRL_W0_SUB0_LEAD_NUM_W0_END (7)
#define SOC_MDC_MIF_LEAD_CTRL_W0_SUB1_LEAD_NUM_W0_START (8)
#define SOC_MDC_MIF_LEAD_CTRL_W0_SUB1_LEAD_NUM_W0_END (15)
#define SOC_MDC_MIF_LEAD_CTRL_W0_SUB2_LEAD_NUM_W0_START (16)
#define SOC_MDC_MIF_LEAD_CTRL_W0_SUB2_LEAD_NUM_W0_END (23)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sub0_os_num_w0 : 8;
        unsigned int sub1_os_num_w0 : 8;
        unsigned int sub2_os_num_w0 : 8;
        unsigned int reserved : 8;
    } reg;
} soc_mdc_mif_os_ctrl_w0_union;
#endif
#define SOC_MDC_MIF_OS_CTRL_W0_SUB0_OS_NUM_W0_START (0)
#define SOC_MDC_MIF_OS_CTRL_W0_SUB0_OS_NUM_W0_END (7)
#define SOC_MDC_MIF_OS_CTRL_W0_SUB1_OS_NUM_W0_START (8)
#define SOC_MDC_MIF_OS_CTRL_W0_SUB1_OS_NUM_W0_END (15)
#define SOC_MDC_MIF_OS_CTRL_W0_SUB2_OS_NUM_W0_START (16)
#define SOC_MDC_MIF_OS_CTRL_W0_SUB2_OS_NUM_W0_END (23)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sub0_pref_cnt_w0 : 8;
        unsigned int sub1_pref_cnt_w0 : 8;
        unsigned int sub2_pref_cnt_w0 : 8;
        unsigned int reserved : 8;
    } reg;
} soc_mdc_mif_stat_w0_union;
#endif
#define SOC_MDC_MIF_STAT_W0_SUB0_PREF_CNT_W0_START (0)
#define SOC_MDC_MIF_STAT_W0_SUB0_PREF_CNT_W0_END (7)
#define SOC_MDC_MIF_STAT_W0_SUB1_PREF_CNT_W0_START (8)
#define SOC_MDC_MIF_STAT_W0_SUB1_PREF_CNT_W0_END (15)
#define SOC_MDC_MIF_STAT_W0_SUB2_PREF_CNT_W0_START (16)
#define SOC_MDC_MIF_STAT_W0_SUB2_PREF_CNT_W0_END (23)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pref_en_w1 : 1;
        unsigned int lock_en_w1 : 1;
        unsigned int delock_type_w1 : 1;
        unsigned int reserved : 29;
    } reg;
} soc_mdc_mif_ctrl_w1_union;
#endif
#define SOC_MDC_MIF_CTRL_W1_PREF_EN_W1_START (0)
#define SOC_MDC_MIF_CTRL_W1_PREF_EN_W1_END (0)
#define SOC_MDC_MIF_CTRL_W1_LOCK_EN_W1_START (1)
#define SOC_MDC_MIF_CTRL_W1_LOCK_EN_W1_END (1)
#define SOC_MDC_MIF_CTRL_W1_DELOCK_TYPE_W1_START (2)
#define SOC_MDC_MIF_CTRL_W1_DELOCK_TYPE_W1_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sub0_lead_num_w1 : 8;
        unsigned int sub1_lead_num_w1 : 8;
        unsigned int sub2_lead_num_w1 : 8;
        unsigned int sub3_lead_num_w1 : 8;
    } reg;
} soc_mdc_mif_lead_ctrl_w1_union;
#endif
#define SOC_MDC_MIF_LEAD_CTRL_W1_SUB0_LEAD_NUM_W1_START (0)
#define SOC_MDC_MIF_LEAD_CTRL_W1_SUB0_LEAD_NUM_W1_END (7)
#define SOC_MDC_MIF_LEAD_CTRL_W1_SUB1_LEAD_NUM_W1_START (8)
#define SOC_MDC_MIF_LEAD_CTRL_W1_SUB1_LEAD_NUM_W1_END (15)
#define SOC_MDC_MIF_LEAD_CTRL_W1_SUB2_LEAD_NUM_W1_START (16)
#define SOC_MDC_MIF_LEAD_CTRL_W1_SUB2_LEAD_NUM_W1_END (23)
#define SOC_MDC_MIF_LEAD_CTRL_W1_SUB3_LEAD_NUM_W1_START (24)
#define SOC_MDC_MIF_LEAD_CTRL_W1_SUB3_LEAD_NUM_W1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sub0_os_num_w1 : 8;
        unsigned int sub1_os_num_w1 : 8;
        unsigned int sub2_os_num_w1 : 8;
        unsigned int sub3_os_num_w1 : 8;
    } reg;
} soc_mdc_mif_os_ctrl_w1_union;
#endif
#define SOC_MDC_MIF_OS_CTRL_W1_SUB0_OS_NUM_W1_START (0)
#define SOC_MDC_MIF_OS_CTRL_W1_SUB0_OS_NUM_W1_END (7)
#define SOC_MDC_MIF_OS_CTRL_W1_SUB1_OS_NUM_W1_START (8)
#define SOC_MDC_MIF_OS_CTRL_W1_SUB1_OS_NUM_W1_END (15)
#define SOC_MDC_MIF_OS_CTRL_W1_SUB2_OS_NUM_W1_START (16)
#define SOC_MDC_MIF_OS_CTRL_W1_SUB2_OS_NUM_W1_END (23)
#define SOC_MDC_MIF_OS_CTRL_W1_SUB3_OS_NUM_W1_START (24)
#define SOC_MDC_MIF_OS_CTRL_W1_SUB3_OS_NUM_W1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sub0_pref_cnt_w1 : 8;
        unsigned int sub1_pref_cnt_w1 : 8;
        unsigned int sub2_pref_cnt_w1 : 8;
        unsigned int sub3_pref_cnt_w1 : 8;
    } reg;
} soc_mdc_mif_stat_w1_union;
#endif
#define SOC_MDC_MIF_STAT_W1_SUB0_PREF_CNT_W1_START (0)
#define SOC_MDC_MIF_STAT_W1_SUB0_PREF_CNT_W1_END (7)
#define SOC_MDC_MIF_STAT_W1_SUB1_PREF_CNT_W1_START (8)
#define SOC_MDC_MIF_STAT_W1_SUB1_PREF_CNT_W1_END (15)
#define SOC_MDC_MIF_STAT_W1_SUB2_PREF_CNT_W1_START (16)
#define SOC_MDC_MIF_STAT_W1_SUB2_PREF_CNT_W1_END (23)
#define SOC_MDC_MIF_STAT_W1_SUB3_PREF_CNT_W1_START (24)
#define SOC_MDC_MIF_STAT_W1_SUB3_PREF_CNT_W1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ispif_en : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_ispif_en_union;
#endif
#define SOC_MDC_ISPIF_EN_ISPIF_EN_START (0)
#define SOC_MDC_ISPIF_EN_ISPIF_EN_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ispif_clk_sel : 3;
        unsigned int reserved : 29;
    } reg;
} soc_mdc_ispif_clk_sel_union;
#endif
#define SOC_MDC_ISPIF_CLK_SEL_ISPIF_CLK_SEL_START (0)
#define SOC_MDC_ISPIF_CLK_SEL_ISPIF_CLK_SEL_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ispif_clk_en : 3;
        unsigned int reserved : 29;
    } reg;
} soc_mdc_ispif_clk_en_union;
#endif
#define SOC_MDC_ISPIF_CLK_EN_ISPIF_CLK_EN_START (0)
#define SOC_MDC_ISPIF_CLK_EN_ISPIF_CLK_EN_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ispif_in_height : 13;
        unsigned int reserved_0 : 3;
        unsigned int ispif_in_width : 13;
        unsigned int reserved_1 : 3;
    } reg;
} soc_mdc_ispif_in_size_union;
#endif
#define SOC_MDC_ISPIF_IN_SIZE_ISPIF_IN_HEIGHT_START (0)
#define SOC_MDC_ISPIF_IN_SIZE_ISPIF_IN_HEIGHT_END (12)
#define SOC_MDC_ISPIF_IN_SIZE_ISPIF_IN_WIDTH_START (16)
#define SOC_MDC_ISPIF_IN_SIZE_ISPIF_IN_WIDTH_END (28)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ispif_rd_shadow : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_ispif_rd_shadow_union;
#endif
#define SOC_MDC_ISPIF_RD_SHADOW_ISPIF_RD_SHADOW_START (0)
#define SOC_MDC_ISPIF_RD_SHADOW_ISPIF_RD_SHADOW_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ispif_dbg0 : 32;
    } reg;
} soc_mdc_ispif_dbg0_union;
#endif
#define SOC_MDC_ISPIF_DBG0_ISPIF_DBG0_START (0)
#define SOC_MDC_ISPIF_DBG0_ISPIF_DBG0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ispif_dbg1 : 32;
    } reg;
} soc_mdc_ispif_dbg1_union;
#endif
#define SOC_MDC_ISPIF_DBG1_ISPIF_DBG1_START (0)
#define SOC_MDC_ISPIF_DBG1_ISPIF_DBG1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ispif_dbg2 : 32;
    } reg;
} soc_mdc_ispif_dbg2_union;
#endif
#define SOC_MDC_ISPIF_DBG2_ISPIF_DBG2_START (0)
#define SOC_MDC_ISPIF_DBG2_ISPIF_DBG2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ch_pending_clr : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_cmdlist_ch_pending_clr_union;
#endif
#define SOC_MDC_CMDLIST_CH_PENDING_CLR_CH_PENDING_CLR_START (0)
#define SOC_MDC_CMDLIST_CH_PENDING_CLR_CH_PENDING_CLR_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ch_enable : 1;
        unsigned int ch_auto_single : 1;
        unsigned int ch_start_sel : 3;
        unsigned int ch_pause : 1;
        unsigned int ch_scene_sel : 1;
        unsigned int reserved : 25;
    } reg;
} soc_mdc_cmdlist_ch_ctrl_union;
#endif
#define SOC_MDC_CMDLIST_CH_CTRL_CH_ENABLE_START (0)
#define SOC_MDC_CMDLIST_CH_CTRL_CH_ENABLE_END (0)
#define SOC_MDC_CMDLIST_CH_CTRL_CH_AUTO_SINGLE_START (1)
#define SOC_MDC_CMDLIST_CH_CTRL_CH_AUTO_SINGLE_END (1)
#define SOC_MDC_CMDLIST_CH_CTRL_CH_START_SEL_START (2)
#define SOC_MDC_CMDLIST_CH_CTRL_CH_START_SEL_END (4)
#define SOC_MDC_CMDLIST_CH_CTRL_CH_PAUSE_START (5)
#define SOC_MDC_CMDLIST_CH_CTRL_CH_PAUSE_END (5)
#define SOC_MDC_CMDLIST_CH_CTRL_CH_SCENE_SEL_START (6)
#define SOC_MDC_CMDLIST_CH_CTRL_CH_SCENE_SEL_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ch_ctl_fsm : 4;
        unsigned int reserved : 4;
        unsigned int ch_dma_fsm : 2;
        unsigned int ch_idle : 1;
        unsigned int oa_idle : 1;
        unsigned int ch_cmd_done_id : 10;
        unsigned int ch_cmd_id : 10;
    } reg;
} soc_mdc_cmdlist_ch_status_union;
#endif
#define SOC_MDC_CMDLIST_CH_STATUS_CH_CTL_FSM_START (0)
#define SOC_MDC_CMDLIST_CH_STATUS_CH_CTL_FSM_END (3)
#define SOC_MDC_CMDLIST_CH_STATUS_CH_DMA_FSM_START (8)
#define SOC_MDC_CMDLIST_CH_STATUS_CH_DMA_FSM_END (9)
#define SOC_MDC_CMDLIST_CH_STATUS_CH_IDLE_START (10)
#define SOC_MDC_CMDLIST_CH_STATUS_CH_IDLE_END (10)
#define SOC_MDC_CMDLIST_CH_STATUS_OA_IDLE_START (11)
#define SOC_MDC_CMDLIST_CH_STATUS_OA_IDLE_END (11)
#define SOC_MDC_CMDLIST_CH_STATUS_CH_CMD_DONE_ID_START (12)
#define SOC_MDC_CMDLIST_CH_STATUS_CH_CMD_DONE_ID_END (21)
#define SOC_MDC_CMDLIST_CH_STATUS_CH_CMD_ID_START (22)
#define SOC_MDC_CMDLIST_CH_STATUS_CH_CMD_ID_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 4;
        unsigned int ch_start_addr : 28;
    } reg;
} soc_mdc_cmdlist_ch_staad_union;
#endif
#define SOC_MDC_CMDLIST_CH_STAAD_CH_START_ADDR_START (4)
#define SOC_MDC_CMDLIST_CH_STAAD_CH_START_ADDR_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ch_cnt : 4;
        unsigned int ch_curr_addr : 28;
    } reg;
} soc_mdc_cmdlist_ch_curad_union;
#endif
#define SOC_MDC_CMDLIST_CH_CURAD_CH_CNT_START (0)
#define SOC_MDC_CMDLIST_CH_CURAD_CH_CNT_END (3)
#define SOC_MDC_CMDLIST_CH_CURAD_CH_CURR_ADDR_START (4)
#define SOC_MDC_CMDLIST_CH_CURAD_CH_CURR_ADDR_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ch_onedone_inte : 1;
        unsigned int ch_alldone_inte : 1;
        unsigned int ch_axi_err_inte : 1;
        unsigned int ch_pending_inte : 1;
        unsigned int ch_start_inte : 1;
        unsigned int ch_badcmd_inte : 1;
        unsigned int ch_timeout_inte : 1;
        unsigned int ch_taskdone_inte : 1;
        unsigned int reserved : 24;
    } reg;
} soc_mdc_cmdlist_ch_inte_union;
#endif
#define SOC_MDC_CMDLIST_CH_INTE_CH_ONEDONE_INTE_START (0)
#define SOC_MDC_CMDLIST_CH_INTE_CH_ONEDONE_INTE_END (0)
#define SOC_MDC_CMDLIST_CH_INTE_CH_ALLDONE_INTE_START (1)
#define SOC_MDC_CMDLIST_CH_INTE_CH_ALLDONE_INTE_END (1)
#define SOC_MDC_CMDLIST_CH_INTE_CH_AXI_ERR_INTE_START (2)
#define SOC_MDC_CMDLIST_CH_INTE_CH_AXI_ERR_INTE_END (2)
#define SOC_MDC_CMDLIST_CH_INTE_CH_PENDING_INTE_START (3)
#define SOC_MDC_CMDLIST_CH_INTE_CH_PENDING_INTE_END (3)
#define SOC_MDC_CMDLIST_CH_INTE_CH_START_INTE_START (4)
#define SOC_MDC_CMDLIST_CH_INTE_CH_START_INTE_END (4)
#define SOC_MDC_CMDLIST_CH_INTE_CH_BADCMD_INTE_START (5)
#define SOC_MDC_CMDLIST_CH_INTE_CH_BADCMD_INTE_END (5)
#define SOC_MDC_CMDLIST_CH_INTE_CH_TIMEOUT_INTE_START (6)
#define SOC_MDC_CMDLIST_CH_INTE_CH_TIMEOUT_INTE_END (6)
#define SOC_MDC_CMDLIST_CH_INTE_CH_TASKDONE_INTE_START (7)
#define SOC_MDC_CMDLIST_CH_INTE_CH_TASKDONE_INTE_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ch_onedone_intc : 1;
        unsigned int ch_alldone_intc : 1;
        unsigned int ch_axi_err_intc : 1;
        unsigned int ch_pending_intc : 1;
        unsigned int ch_start_intc : 1;
        unsigned int ch_badcmd_intc : 1;
        unsigned int ch_timeout_intc : 1;
        unsigned int ch_taskdone_intc : 1;
        unsigned int reserved : 24;
    } reg;
} soc_mdc_cmdlist_ch_intc_union;
#endif
#define SOC_MDC_CMDLIST_CH_INTC_CH_ONEDONE_INTC_START (0)
#define SOC_MDC_CMDLIST_CH_INTC_CH_ONEDONE_INTC_END (0)
#define SOC_MDC_CMDLIST_CH_INTC_CH_ALLDONE_INTC_START (1)
#define SOC_MDC_CMDLIST_CH_INTC_CH_ALLDONE_INTC_END (1)
#define SOC_MDC_CMDLIST_CH_INTC_CH_AXI_ERR_INTC_START (2)
#define SOC_MDC_CMDLIST_CH_INTC_CH_AXI_ERR_INTC_END (2)
#define SOC_MDC_CMDLIST_CH_INTC_CH_PENDING_INTC_START (3)
#define SOC_MDC_CMDLIST_CH_INTC_CH_PENDING_INTC_END (3)
#define SOC_MDC_CMDLIST_CH_INTC_CH_START_INTC_START (4)
#define SOC_MDC_CMDLIST_CH_INTC_CH_START_INTC_END (4)
#define SOC_MDC_CMDLIST_CH_INTC_CH_BADCMD_INTC_START (5)
#define SOC_MDC_CMDLIST_CH_INTC_CH_BADCMD_INTC_END (5)
#define SOC_MDC_CMDLIST_CH_INTC_CH_TIMEOUT_INTC_START (6)
#define SOC_MDC_CMDLIST_CH_INTC_CH_TIMEOUT_INTC_END (6)
#define SOC_MDC_CMDLIST_CH_INTC_CH_TASKDONE_INTC_START (7)
#define SOC_MDC_CMDLIST_CH_INTC_CH_TASKDONE_INTC_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ch_onedone_ints : 1;
        unsigned int ch_alldone_ints : 1;
        unsigned int ch_axi_err_ints : 1;
        unsigned int ch_pending_ints : 1;
        unsigned int ch_start_ints : 1;
        unsigned int ch_badcmd_ints : 1;
        unsigned int ch_timeout_ints : 1;
        unsigned int ch_taskdone_ints : 1;
        unsigned int reserved : 24;
    } reg;
} soc_mdc_cmdlist_ch_ints_union;
#endif
#define SOC_MDC_CMDLIST_CH_INTS_CH_ONEDONE_INTS_START (0)
#define SOC_MDC_CMDLIST_CH_INTS_CH_ONEDONE_INTS_END (0)
#define SOC_MDC_CMDLIST_CH_INTS_CH_ALLDONE_INTS_START (1)
#define SOC_MDC_CMDLIST_CH_INTS_CH_ALLDONE_INTS_END (1)
#define SOC_MDC_CMDLIST_CH_INTS_CH_AXI_ERR_INTS_START (2)
#define SOC_MDC_CMDLIST_CH_INTS_CH_AXI_ERR_INTS_END (2)
#define SOC_MDC_CMDLIST_CH_INTS_CH_PENDING_INTS_START (3)
#define SOC_MDC_CMDLIST_CH_INTS_CH_PENDING_INTS_END (3)
#define SOC_MDC_CMDLIST_CH_INTS_CH_START_INTS_START (4)
#define SOC_MDC_CMDLIST_CH_INTS_CH_START_INTS_END (4)
#define SOC_MDC_CMDLIST_CH_INTS_CH_BADCMD_INTS_START (5)
#define SOC_MDC_CMDLIST_CH_INTS_CH_BADCMD_INTS_END (5)
#define SOC_MDC_CMDLIST_CH_INTS_CH_TIMEOUT_INTS_START (6)
#define SOC_MDC_CMDLIST_CH_INTS_CH_TIMEOUT_INTS_END (6)
#define SOC_MDC_CMDLIST_CH_INTS_CH_TASKDONE_INTS_START (7)
#define SOC_MDC_CMDLIST_CH_INTS_CH_TASKDONE_INTS_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int cmd_dbg0 : 1;
        unsigned int cmd_dbg1 : 1;
        unsigned int cmd_dbg2 : 1;
        unsigned int cmd_dbg3 : 1;
        unsigned int cmd_dbg4 : 1;
        unsigned int cmd_dbg5 : 1;
        unsigned int cmd_dbg6 : 1;
        unsigned int cmd_dbg7 : 1;
        unsigned int cmd_dbg8 : 1;
        unsigned int cmd_dbg9 : 1;
        unsigned int cmd_dbg10 : 1;
        unsigned int cmd_dbg11 : 1;
        unsigned int reserved : 20;
    } reg;
} soc_mdc_cmdlist_ch_dbg_union;
#endif
#define SOC_MDC_CMDLIST_CH_DBG_CMD_DBG0_START (0)
#define SOC_MDC_CMDLIST_CH_DBG_CMD_DBG0_END (0)
#define SOC_MDC_CMDLIST_CH_DBG_CMD_DBG1_START (1)
#define SOC_MDC_CMDLIST_CH_DBG_CMD_DBG1_END (1)
#define SOC_MDC_CMDLIST_CH_DBG_CMD_DBG2_START (2)
#define SOC_MDC_CMDLIST_CH_DBG_CMD_DBG2_END (2)
#define SOC_MDC_CMDLIST_CH_DBG_CMD_DBG3_START (3)
#define SOC_MDC_CMDLIST_CH_DBG_CMD_DBG3_END (3)
#define SOC_MDC_CMDLIST_CH_DBG_CMD_DBG4_START (4)
#define SOC_MDC_CMDLIST_CH_DBG_CMD_DBG4_END (4)
#define SOC_MDC_CMDLIST_CH_DBG_CMD_DBG5_START (5)
#define SOC_MDC_CMDLIST_CH_DBG_CMD_DBG5_END (5)
#define SOC_MDC_CMDLIST_CH_DBG_CMD_DBG6_START (6)
#define SOC_MDC_CMDLIST_CH_DBG_CMD_DBG6_END (6)
#define SOC_MDC_CMDLIST_CH_DBG_CMD_DBG7_START (7)
#define SOC_MDC_CMDLIST_CH_DBG_CMD_DBG7_END (7)
#define SOC_MDC_CMDLIST_CH_DBG_CMD_DBG8_START (8)
#define SOC_MDC_CMDLIST_CH_DBG_CMD_DBG8_END (8)
#define SOC_MDC_CMDLIST_CH_DBG_CMD_DBG9_START (9)
#define SOC_MDC_CMDLIST_CH_DBG_CMD_DBG9_END (9)
#define SOC_MDC_CMDLIST_CH_DBG_CMD_DBG10_START (10)
#define SOC_MDC_CMDLIST_CH_DBG_CMD_DBG10_END (10)
#define SOC_MDC_CMDLIST_CH_DBG_CMD_DBG11_START (11)
#define SOC_MDC_CMDLIST_CH_DBG_CMD_DBG11_END (11)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int cmd_axi_dbg : 28;
        unsigned int reserved : 4;
    } reg;
} soc_mdc_cmdlist_dbg_union;
#endif
#define SOC_MDC_CMDLIST_DBG_CMD_AXI_DBG_START (0)
#define SOC_MDC_CMDLIST_DBG_CMD_AXI_DBG_END (27)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int buf_dbg_en : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_buf_dbg_en_union;
#endif
#define SOC_MDC_BUF_DBG_EN_BUF_DBG_EN_START (0)
#define SOC_MDC_BUF_DBG_EN_BUF_DBG_EN_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int buf_dbg_cnt_clr : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_buf_dbg_cnt_clr_union;
#endif
#define SOC_MDC_BUF_DBG_CNT_CLR_BUF_DBG_CNT_CLR_START (0)
#define SOC_MDC_BUF_DBG_CNT_CLR_BUF_DBG_CNT_CLR_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int buf_dbg_cnt_th : 8;
        unsigned int reserved : 24;
    } reg;
} soc_mdc_buf_dbg_cnt_union;
#endif
#define SOC_MDC_BUF_DBG_CNT_BUF_DBG_CNT_TH_START (0)
#define SOC_MDC_BUF_DBG_CNT_BUF_DBG_CNT_TH_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int timeout_th : 32;
    } reg;
} soc_mdc_cmdlist_timeout_th_union;
#endif
#define SOC_MDC_CMDLIST_TIMEOUT_TH_TIMEOUT_TH_START (0)
#define SOC_MDC_CMDLIST_TIMEOUT_TH_TIMEOUT_TH_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int start : 16;
        unsigned int reserved : 16;
    } reg;
} soc_mdc_cmdlist_start_union;
#endif
#define SOC_MDC_CMDLIST_START_START_START (0)
#define SOC_MDC_CMDLIST_START_START_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int addr_mask_en : 16;
        unsigned int reserved : 16;
    } reg;
} soc_mdc_cmdlist_addr_mask_en_union;
#endif
#define SOC_MDC_CMDLIST_ADDR_MASK_EN_ADDR_MASK_EN_START (0)
#define SOC_MDC_CMDLIST_ADDR_MASK_EN_ADDR_MASK_EN_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int addr_mask_dis : 16;
        unsigned int reserved : 16;
    } reg;
} soc_mdc_cmdlist_addr_mask_dis_union;
#endif
#define SOC_MDC_CMDLIST_ADDR_MASK_DIS_ADDR_MASK_DIS_START (0)
#define SOC_MDC_CMDLIST_ADDR_MASK_DIS_ADDR_MASK_DIS_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int addr_mask_status : 16;
        unsigned int reserved : 16;
    } reg;
} soc_mdc_cmdlist_addr_mask_status_union;
#endif
#define SOC_MDC_CMDLIST_ADDR_MASK_STATUS_ADDR_MASK_STATUS_START (0)
#define SOC_MDC_CMDLIST_ADDR_MASK_STATUS_ADDR_MASK_STATUS_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int task_continue : 16;
        unsigned int reserved : 16;
    } reg;
} soc_mdc_cmdlist_task_continue_union;
#endif
#define SOC_MDC_CMDLIST_TASK_CONTINUE_TASK_CONTINUE_START (0)
#define SOC_MDC_CMDLIST_TASK_CONTINUE_TASK_CONTINUE_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int task_status : 16;
        unsigned int reserved : 16;
    } reg;
} soc_mdc_cmdlist_task_status_union;
#endif
#define SOC_MDC_CMDLIST_TASK_STATUS_TASK_STATUS_START (0)
#define SOC_MDC_CMDLIST_TASK_STATUS_TASK_STATUS_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int axi_mid : 4;
        unsigned int axi_qos : 2;
        unsigned int axi_mainpress : 2;
        unsigned int axi_outstd_depth : 4;
        unsigned int buf0_outstd_depth : 2;
        unsigned int reserved_0 : 2;
        unsigned int reserved_1 : 2;
        unsigned int reserved_2 : 2;
        unsigned int reserved_3 : 2;
        unsigned int reserved_4 : 2;
        unsigned int reserved_5 : 8;
    } reg;
} soc_mdc_cmdlist_ctrl_union;
#endif
#define SOC_MDC_CMDLIST_CTRL_AXI_MID_START (0)
#define SOC_MDC_CMDLIST_CTRL_AXI_MID_END (3)
#define SOC_MDC_CMDLIST_CTRL_AXI_QOS_START (4)
#define SOC_MDC_CMDLIST_CTRL_AXI_QOS_END (5)
#define SOC_MDC_CMDLIST_CTRL_AXI_MAINPRESS_START (6)
#define SOC_MDC_CMDLIST_CTRL_AXI_MAINPRESS_END (7)
#define SOC_MDC_CMDLIST_CTRL_AXI_OUTSTD_DEPTH_START (8)
#define SOC_MDC_CMDLIST_CTRL_AXI_OUTSTD_DEPTH_END (11)
#define SOC_MDC_CMDLIST_CTRL_BUF0_OUTSTD_DEPTH_START (12)
#define SOC_MDC_CMDLIST_CTRL_BUF0_OUTSTD_DEPTH_END (13)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ch_secu : 16;
        unsigned int reserved : 16;
    } reg;
} soc_mdc_cmdlist_secu_union;
#endif
#define SOC_MDC_CMDLIST_SECU_CH_SECU_START (0)
#define SOC_MDC_CMDLIST_SECU_CH_SECU_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ch_ints : 16;
        unsigned int reserved : 16;
    } reg;
} soc_mdc_cmdlist_ints_union;
#endif
#define SOC_MDC_CMDLIST_INTS_CH_INTS_START (0)
#define SOC_MDC_CMDLIST_INTS_CH_INTS_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ch_swreset : 16;
        unsigned int reserved : 16;
    } reg;
} soc_mdc_cmdlist_swrst_union;
#endif
#define SOC_MDC_CMDLIST_SWRST_CH_SWRESET_START (0)
#define SOC_MDC_CMDLIST_SWRST_CH_SWRESET_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int cmd_mem_ctrl : 4;
        unsigned int reserved : 28;
    } reg;
} soc_mdc_cmd_mem_ctrl_union;
#endif
#define SOC_MDC_CMD_MEM_CTRL_CMD_MEM_CTRL_START (0)
#define SOC_MDC_CMD_MEM_CTRL_CMD_MEM_CTRL_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int cmd_clk_sel : 32;
    } reg;
} soc_mdc_cmd_clk_sel_union;
#endif
#define SOC_MDC_CMD_CLK_SEL_CMD_CLK_SEL_START (0)
#define SOC_MDC_CMD_CLK_SEL_CMD_CLK_SEL_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int cmd_clk_en : 32;
    } reg;
} soc_mdc_cmd_clk_en_union;
#endif
#define SOC_MDC_CMD_CLK_EN_CMD_CLK_EN_START (0)
#define SOC_MDC_CMD_CLK_EN_CMD_CLK_EN_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int rch0_flush_en : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_rch0_flush_en_union;
#endif
#define SOC_MDC_RCH0_FLUSH_EN_RCH0_FLUSH_EN_START (0)
#define SOC_MDC_RCH0_FLUSH_EN_RCH0_FLUSH_EN_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int rch1_flush_en : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_rch1_flush_en_union;
#endif
#define SOC_MDC_RCH1_FLUSH_EN_RCH1_FLUSH_EN_START (0)
#define SOC_MDC_RCH1_FLUSH_EN_RCH1_FLUSH_EN_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int rch2_flush_en : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_rch2_flush_en_union;
#endif
#define SOC_MDC_RCH2_FLUSH_EN_RCH2_FLUSH_EN_START (0)
#define SOC_MDC_RCH2_FLUSH_EN_RCH2_FLUSH_EN_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int rch6_flush_en : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_rch6_flush_en_union;
#endif
#define SOC_MDC_RCH6_FLUSH_EN_RCH6_FLUSH_EN_START (0)
#define SOC_MDC_RCH6_FLUSH_EN_RCH6_FLUSH_EN_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int wch0_flush_en : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_wch0_flush_en_union;
#endif
#define SOC_MDC_WCH0_FLUSH_EN_WCH0_FLUSH_EN_START (0)
#define SOC_MDC_WCH0_FLUSH_EN_WCH0_FLUSH_EN_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int wch1_flush_en : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_wch1_flush_en_union;
#endif
#define SOC_MDC_WCH1_FLUSH_EN_WCH1_FLUSH_EN_START (0)
#define SOC_MDC_WCH1_FLUSH_EN_WCH1_FLUSH_EN_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ov2_flush_en : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_ov2_flush_en_union;
#endif
#define SOC_MDC_OV2_FLUSH_EN_OV2_FLUSH_EN_START (0)
#define SOC_MDC_OV2_FLUSH_EN_OV2_FLUSH_EN_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ispif_flush_en : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_ispif_flush_en_union;
#endif
#define SOC_MDC_ISPIF_FLUSH_EN_ISPIF_FLUSH_EN_START (0)
#define SOC_MDC_ISPIF_FLUSH_EN_ISPIF_FLUSH_EN_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int rch0_ov_oen : 13;
        unsigned int reserved : 19;
    } reg;
} soc_mdc_rch0_ov_oen_union;
#endif
#define SOC_MDC_RCH0_OV_OEN_RCH0_OV_OEN_START (0)
#define SOC_MDC_RCH0_OV_OEN_RCH0_OV_OEN_END (12)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int rch1_ov_oen : 13;
        unsigned int reserved : 19;
    } reg;
} soc_mdc_rch1_ov_oen_union;
#endif
#define SOC_MDC_RCH1_OV_OEN_RCH1_OV_OEN_START (0)
#define SOC_MDC_RCH1_OV_OEN_RCH1_OV_OEN_END (12)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int rch2_ov_oen : 13;
        unsigned int reserved : 19;
    } reg;
} soc_mdc_rch2_ov_oen_union;
#endif
#define SOC_MDC_RCH2_OV_OEN_RCH2_OV_OEN_START (0)
#define SOC_MDC_RCH2_OV_OEN_RCH2_OV_OEN_END (12)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int rch6_ov_oen : 13;
        unsigned int reserved : 19;
    } reg;
} soc_mdc_rch6_ov_oen_union;
#endif
#define SOC_MDC_RCH6_OV_OEN_RCH6_OV_OEN_START (0)
#define SOC_MDC_RCH6_OV_OEN_RCH6_OV_OEN_END (12)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int rch_ov2_sel0 : 32;
    } reg;
} soc_mdc_rch_ov2_sel0_union;
#endif
#define SOC_MDC_RCH_OV2_SEL0_RCH_OV2_SEL0_START (0)
#define SOC_MDC_RCH_OV2_SEL0_RCH_OV2_SEL0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int rch_ov3_sel : 12;
        unsigned int reserved : 20;
    } reg;
} soc_mdc_rch_ov3_sel_union;
#endif
#define SOC_MDC_RCH_OV3_SEL_RCH_OV3_SEL_START (0)
#define SOC_MDC_RCH_OV3_SEL_RCH_OV3_SEL_END (11)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int wch0_ov_ien : 3;
        unsigned int reserved : 29;
    } reg;
} soc_mdc_wch0_ov_ien_union;
#endif
#define SOC_MDC_WCH0_OV_IEN_WCH0_OV_IEN_START (0)
#define SOC_MDC_WCH0_OV_IEN_WCH0_OV_IEN_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int wch1_ov_ien : 3;
        unsigned int reserved : 29;
    } reg;
} soc_mdc_wch1_ov_ien_union;
#endif
#define SOC_MDC_WCH1_OV_IEN_WCH1_OV_IEN_START (0)
#define SOC_MDC_WCH1_OV_IEN_WCH1_OV_IEN_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int wch_ov2_sel : 2;
        unsigned int reserved : 30;
    } reg;
} soc_mdc_wch_ov2_sel_union;
#endif
#define SOC_MDC_WCH_OV2_SEL_WCH_OV2_SEL_START (0)
#define SOC_MDC_WCH_OV2_SEL_WCH_OV2_SEL_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int isp_wch_sel : 2;
        unsigned int reserved : 30;
    } reg;
} soc_mdc_isp_wch_sel_union;
#endif
#define SOC_MDC_ISP_WCH_SEL_ISP_WCH_SEL_START (0)
#define SOC_MDC_ISP_WCH_SEL_ISP_WCH_SEL_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int rch0_starty : 15;
        unsigned int reserved_0 : 1;
        unsigned int rch0_thld : 15;
        unsigned int reserved_1 : 1;
    } reg;
} soc_mdc_rch0_starty_union;
#endif
#define SOC_MDC_RCH0_STARTY_RCH0_STARTY_START (0)
#define SOC_MDC_RCH0_STARTY_RCH0_STARTY_END (14)
#define SOC_MDC_RCH0_STARTY_RCH0_THLD_START (16)
#define SOC_MDC_RCH0_STARTY_RCH0_THLD_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int rch1_starty : 15;
        unsigned int reserved_0 : 1;
        unsigned int rch1_thld : 15;
        unsigned int reserved_1 : 1;
    } reg;
} soc_mdc_rch1_starty_union;
#endif
#define SOC_MDC_RCH1_STARTY_RCH1_STARTY_START (0)
#define SOC_MDC_RCH1_STARTY_RCH1_STARTY_END (14)
#define SOC_MDC_RCH1_STARTY_RCH1_THLD_START (16)
#define SOC_MDC_RCH1_STARTY_RCH1_THLD_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int rch2_starty : 15;
        unsigned int reserved_0 : 1;
        unsigned int rch2_thld : 15;
        unsigned int reserved_1 : 1;
    } reg;
} soc_mdc_rch2_starty_union;
#endif
#define SOC_MDC_RCH2_STARTY_RCH2_STARTY_START (0)
#define SOC_MDC_RCH2_STARTY_RCH2_STARTY_END (14)
#define SOC_MDC_RCH2_STARTY_RCH2_THLD_START (16)
#define SOC_MDC_RCH2_STARTY_RCH2_THLD_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int rch6_starty : 15;
        unsigned int reserved_0 : 1;
        unsigned int rch6_thld : 15;
        unsigned int reserved_1 : 1;
    } reg;
} soc_mdc_rch6_starty_union;
#endif
#define SOC_MDC_RCH6_STARTY_RCH6_STARTY_START (0)
#define SOC_MDC_RCH6_STARTY_RCH6_STARTY_END (14)
#define SOC_MDC_RCH6_STARTY_RCH6_THLD_START (16)
#define SOC_MDC_RCH6_STARTY_RCH6_THLD_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int mctl_clk_sel : 32;
    } reg;
} soc_mdc_mctl_clk_sel_union;
#endif
#define SOC_MDC_MCTL_CLK_SEL_MCTL_CLK_SEL_START (0)
#define SOC_MDC_MCTL_CLK_SEL_MCTL_CLK_SEL_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int mctl_clk_en : 32;
    } reg;
} soc_mdc_mctl_clk_en_union;
#endif
#define SOC_MDC_MCTL_CLK_EN_MCTL_CLK_EN_START (0)
#define SOC_MDC_MCTL_CLK_EN_MCTL_CLK_EN_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int mod_clk_sel : 32;
    } reg;
} soc_mdc_mod_clk_sel_union;
#endif
#define SOC_MDC_MOD_CLK_SEL_MOD_CLK_SEL_START (0)
#define SOC_MDC_MOD_CLK_SEL_MOD_CLK_SEL_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int mod_clk_en : 32;
    } reg;
} soc_mdc_mod_clk_en_union;
#endif
#define SOC_MDC_MOD_CLK_EN_MOD_CLK_EN_START (0)
#define SOC_MDC_MOD_CLK_EN_MOD_CLK_EN_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int rch0_dbg : 24;
        unsigned int reserved : 8;
    } reg;
} soc_mdc_mod0_dbg_union;
#endif
#define SOC_MDC_MOD0_DBG_RCH0_DBG_START (0)
#define SOC_MDC_MOD0_DBG_RCH0_DBG_END (23)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int rch1_dbg : 24;
        unsigned int reserved : 8;
    } reg;
} soc_mdc_mod1_dbg_union;
#endif
#define SOC_MDC_MOD1_DBG_RCH1_DBG_START (0)
#define SOC_MDC_MOD1_DBG_RCH1_DBG_END (23)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int rch2_dbg : 24;
        unsigned int reserved : 8;
    } reg;
} soc_mdc_mod2_dbg_union;
#endif
#define SOC_MDC_MOD2_DBG_RCH2_DBG_START (0)
#define SOC_MDC_MOD2_DBG_RCH2_DBG_END (23)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int rch6_dbg : 24;
        unsigned int reserved : 8;
    } reg;
} soc_mdc_mod6_dbg_union;
#endif
#define SOC_MDC_MOD6_DBG_RCH6_DBG_START (0)
#define SOC_MDC_MOD6_DBG_RCH6_DBG_END (23)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int wch0_dbg : 24;
        unsigned int reserved : 8;
    } reg;
} soc_mdc_mod8_dbg_union;
#endif
#define SOC_MDC_MOD8_DBG_WCH0_DBG_START (0)
#define SOC_MDC_MOD8_DBG_WCH0_DBG_END (23)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int wch1_dbg : 24;
        unsigned int reserved : 8;
    } reg;
} soc_mdc_mod9_dbg_union;
#endif
#define SOC_MDC_MOD9_DBG_WCH1_DBG_START (0)
#define SOC_MDC_MOD9_DBG_WCH1_DBG_END (23)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ov2_dbg : 24;
        unsigned int reserved : 8;
    } reg;
} soc_mdc_mod12_dbg_union;
#endif
#define SOC_MDC_MOD12_DBG_OV2_DBG_START (0)
#define SOC_MDC_MOD12_DBG_OV2_DBG_END (23)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ispif_dbg : 24;
        unsigned int reserved : 8;
    } reg;
} soc_mdc_mod13_dbg_union;
#endif
#define SOC_MDC_MOD13_DBG_ISPIF_DBG_START (0)
#define SOC_MDC_MOD13_DBG_ISPIF_DBG_END (23)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int itf0_dbg : 24;
        unsigned int reserved : 8;
    } reg;
} soc_mdc_mod17_dbg_union;
#endif
#define SOC_MDC_MOD17_DBG_ITF0_DBG_START (0)
#define SOC_MDC_MOD17_DBG_ITF0_DBG_END (23)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int itf1_dbg : 24;
        unsigned int reserved : 8;
    } reg;
} soc_mdc_mod18_dbg_union;
#endif
#define SOC_MDC_MOD18_DBG_ITF1_DBG_START (0)
#define SOC_MDC_MOD18_DBG_ITF1_DBG_END (23)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int rch0_status : 6;
        unsigned int reserved : 26;
    } reg;
} soc_mdc_mod0_status_union;
#endif
#define SOC_MDC_MOD0_STATUS_RCH0_STATUS_START (0)
#define SOC_MDC_MOD0_STATUS_RCH0_STATUS_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int rch1_status : 6;
        unsigned int reserved : 26;
    } reg;
} soc_mdc_mod1_status_union;
#endif
#define SOC_MDC_MOD1_STATUS_RCH1_STATUS_START (0)
#define SOC_MDC_MOD1_STATUS_RCH1_STATUS_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int rch2_status : 6;
        unsigned int reserved : 26;
    } reg;
} soc_mdc_mod2_status_union;
#endif
#define SOC_MDC_MOD2_STATUS_RCH2_STATUS_START (0)
#define SOC_MDC_MOD2_STATUS_RCH2_STATUS_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int rch6_status : 6;
        unsigned int reserved : 26;
    } reg;
} soc_mdc_mod6_status_union;
#endif
#define SOC_MDC_MOD6_STATUS_RCH6_STATUS_START (0)
#define SOC_MDC_MOD6_STATUS_RCH6_STATUS_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int wch0_status : 6;
        unsigned int reserved : 26;
    } reg;
} soc_mdc_mod8_status_union;
#endif
#define SOC_MDC_MOD8_STATUS_WCH0_STATUS_START (0)
#define SOC_MDC_MOD8_STATUS_WCH0_STATUS_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int wch1_status : 6;
        unsigned int reserved : 26;
    } reg;
} soc_mdc_mod9_status_union;
#endif
#define SOC_MDC_MOD9_STATUS_WCH1_STATUS_START (0)
#define SOC_MDC_MOD9_STATUS_WCH1_STATUS_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ov2_status : 6;
        unsigned int reserved : 26;
    } reg;
} soc_mdc_mod12_status_union;
#endif
#define SOC_MDC_MOD12_STATUS_OV2_STATUS_START (0)
#define SOC_MDC_MOD12_STATUS_OV2_STATUS_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ov3_status : 6;
        unsigned int reserved : 26;
    } reg;
} soc_mdc_mod13_status_union;
#endif
#define SOC_MDC_MOD13_STATUS_OV3_STATUS_START (0)
#define SOC_MDC_MOD13_STATUS_OV3_STATUS_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int itf0_status : 6;
        unsigned int reserved : 26;
    } reg;
} soc_mdc_mod17_status_union;
#endif
#define SOC_MDC_MOD17_STATUS_ITF0_STATUS_START (0)
#define SOC_MDC_MOD17_STATUS_ITF0_STATUS_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int itf1_status : 6;
        unsigned int reserved : 26;
    } reg;
} soc_mdc_mod18_status_union;
#endif
#define SOC_MDC_MOD18_STATUS_ITF1_STATUS_START (0)
#define SOC_MDC_MOD18_STATUS_ITF1_STATUS_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sw_dbg : 16;
        unsigned int reserved : 16;
    } reg;
} soc_mdc_sw_dbg_union;
#endif
#define SOC_MDC_SW_DBG_SW_DBG_START (0)
#define SOC_MDC_SW_DBG_SW_DBG_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sw0_status0 : 32;
    } reg;
} soc_mdc_sw0_status0_union;
#endif
#define SOC_MDC_SW0_STATUS0_SW0_STATUS0_START (0)
#define SOC_MDC_SW0_STATUS0_SW0_STATUS0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sw1_status : 32;
    } reg;
} soc_mdc_sw1_status_union;
#endif
#define SOC_MDC_SW1_STATUS_SW1_STATUS_START (0)
#define SOC_MDC_SW1_STATUS_SW1_STATUS_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ctl_en : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_ctl_en_union;
#endif
#define SOC_MDC_CTL_EN_CTL_EN_START (0)
#define SOC_MDC_CTL_EN_CTL_EN_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ctl_mutex_sw : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_ctl_mutex_union;
#endif
#define SOC_MDC_CTL_MUTEX_CTL_MUTEX_SW_START (0)
#define SOC_MDC_CTL_MUTEX_CTL_MUTEX_SW_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ctl_mutex_status : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_ctl_mutex_status_union;
#endif
#define SOC_MDC_CTL_MUTEX_STATUS_CTL_MUTEX_STATUS_START (0)
#define SOC_MDC_CTL_MUTEX_STATUS_CTL_MUTEX_STATUS_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ctl_mutex_ov : 4;
        unsigned int reserved : 28;
    } reg;
} soc_mdc_ctl_mutex_ov_union;
#endif
#define SOC_MDC_CTL_MUTEX_OV_CTL_MUTEX_OV_START (0)
#define SOC_MDC_CTL_MUTEX_OV_CTL_MUTEX_OV_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ctl_mutex_wch0 : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_ctl_mutex_wch0_union;
#endif
#define SOC_MDC_CTL_MUTEX_WCH0_CTL_MUTEX_WCH0_START (0)
#define SOC_MDC_CTL_MUTEX_WCH0_CTL_MUTEX_WCH0_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ctl_mutex_wch1 : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_ctl_mutex_wch1_union;
#endif
#define SOC_MDC_CTL_MUTEX_WCH1_CTL_MUTEX_WCH1_START (0)
#define SOC_MDC_CTL_MUTEX_WCH1_CTL_MUTEX_WCH1_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ctl_mutex_rch0 : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_ctl_mutex_rch0_union;
#endif
#define SOC_MDC_CTL_MUTEX_RCH0_CTL_MUTEX_RCH0_START (0)
#define SOC_MDC_CTL_MUTEX_RCH0_CTL_MUTEX_RCH0_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ctl_mutex_rch1 : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_ctl_mutex_rch1_union;
#endif
#define SOC_MDC_CTL_MUTEX_RCH1_CTL_MUTEX_RCH1_START (0)
#define SOC_MDC_CTL_MUTEX_RCH1_CTL_MUTEX_RCH1_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ctl_mutex_rch2 : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_ctl_mutex_rch2_union;
#endif
#define SOC_MDC_CTL_MUTEX_RCH2_CTL_MUTEX_RCH2_START (0)
#define SOC_MDC_CTL_MUTEX_RCH2_CTL_MUTEX_RCH2_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ctl_mutex_rch6 : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_ctl_mutex_rch6_union;
#endif
#define SOC_MDC_CTL_MUTEX_RCH6_CTL_MUTEX_RCH6_START (0)
#define SOC_MDC_CTL_MUTEX_RCH6_CTL_MUTEX_RCH6_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ctl_sof_sel : 2;
        unsigned int reserved_0 : 1;
        unsigned int ctl_block_mode : 1;
        unsigned int reserved_1 : 28;
    } reg;
} soc_mdc_ctl_top_union;
#endif
#define SOC_MDC_CTL_TOP_CTL_SOF_SEL_START (0)
#define SOC_MDC_CTL_TOP_CTL_SOF_SEL_END (1)
#define SOC_MDC_CTL_TOP_CTL_BLOCK_MODE_START (3)
#define SOC_MDC_CTL_TOP_CTL_BLOCK_MODE_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ctl_mod_flush_status : 32;
    } reg;
} soc_mdc_ctl_flush_status_union;
#endif
#define SOC_MDC_CTL_FLUSH_STATUS_CTL_MOD_FLUSH_STATUS_START (0)
#define SOC_MDC_CTL_FLUSH_STATUS_CTL_MOD_FLUSH_STATUS_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ctl_sw_clr : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_ctl_clear_union;
#endif
#define SOC_MDC_CTL_CLEAR_CTL_SW_CLR_START (0)
#define SOC_MDC_CTL_CLEAR_CTL_SW_CLR_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ctl_cack_tout : 16;
        unsigned int reserved : 16;
    } reg;
} soc_mdc_ctl_cack_tout_union;
#endif
#define SOC_MDC_CTL_CACK_TOUT_CTL_CACK_TOUT_START (0)
#define SOC_MDC_CTL_CACK_TOUT_CTL_CACK_TOUT_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ctl_mutex_tout : 16;
        unsigned int reserved : 16;
    } reg;
} soc_mdc_ctl_mutex_tout_union;
#endif
#define SOC_MDC_CTL_MUTEX_TOUT_CTL_MUTEX_TOUT_START (0)
#define SOC_MDC_CTL_MUTEX_TOUT_CTL_MUTEX_TOUT_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ctl_status : 4;
        unsigned int ctl_no_clr_ack : 1;
        unsigned int reserved : 27;
    } reg;
} soc_mdc_ctl_status_union;
#endif
#define SOC_MDC_CTL_STATUS_CTL_STATUS_START (0)
#define SOC_MDC_CTL_STATUS_CTL_STATUS_END (3)
#define SOC_MDC_CTL_STATUS_CTL_NO_CLR_ACK_START (4)
#define SOC_MDC_CTL_STATUS_CTL_NO_CLR_ACK_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ctl_inten : 4;
        unsigned int reserved : 28;
    } reg;
} soc_mdc_ctl_inten_union;
#endif
#define SOC_MDC_CTL_INTEN_CTL_INTEN_START (0)
#define SOC_MDC_CTL_INTEN_CTL_INTEN_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ctl_sw_st : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_ctl_sw_st_union;
#endif
#define SOC_MDC_CTL_SW_ST_CTL_SW_ST_START (0)
#define SOC_MDC_CTL_SW_ST_CTL_SW_ST_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ctl_st_sel : 1;
        unsigned int ctl_act_sel : 1;
        unsigned int reserved : 30;
    } reg;
} soc_mdc_ctl_st_sel_union;
#endif
#define SOC_MDC_CTL_ST_SEL_CTL_ST_SEL_START (0)
#define SOC_MDC_CTL_ST_SEL_CTL_ST_SEL_END (0)
#define SOC_MDC_CTL_ST_SEL_CTL_ACT_SEL_START (1)
#define SOC_MDC_CTL_ST_SEL_CTL_ACT_SEL_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ctl_end_sel : 32;
    } reg;
} soc_mdc_ctl_end_sel_union;
#endif
#define SOC_MDC_CTL_END_SEL_CTL_END_SEL_START (0)
#define SOC_MDC_CTL_END_SEL_CTL_END_SEL_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ctl_clk_sel : 32;
    } reg;
} soc_mdc_ctl_clk_sel_union;
#endif
#define SOC_MDC_CTL_CLK_SEL_CTL_CLK_SEL_START (0)
#define SOC_MDC_CTL_CLK_SEL_CTL_CLK_SEL_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ctl_clk_en : 32;
    } reg;
} soc_mdc_ctl_clk_en_union;
#endif
#define SOC_MDC_CTL_CLK_EN_CTL_CLK_EN_START (0)
#define SOC_MDC_CTL_CLK_EN_CTL_CLK_EN_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ctl_dbg : 24;
        unsigned int reserved : 8;
    } reg;
} soc_mdc_ctl_dbg_union;
#endif
#define SOC_MDC_CTL_DBG_CTL_DBG_START (0)
#define SOC_MDC_CTL_DBG_CTL_DBG_END (23)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_oft_x0 : 12;
        unsigned int reserved : 20;
    } reg;
} soc_mdc_dma_oft_x0_union;
#endif
#define SOC_MDC_DMA_OFT_X0_DMA_OFT_X0_START (0)
#define SOC_MDC_DMA_OFT_X0_DMA_OFT_X0_END (11)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_oft_y0 : 16;
        unsigned int reserved : 16;
    } reg;
} soc_mdc_dma_oft_y0_union;
#endif
#define SOC_MDC_DMA_OFT_Y0_DMA_OFT_Y0_START (0)
#define SOC_MDC_DMA_OFT_Y0_DMA_OFT_Y0_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_oft_x1 : 12;
        unsigned int reserved : 20;
    } reg;
} soc_mdc_dma_oft_x1_union;
#endif
#define SOC_MDC_DMA_OFT_X1_DMA_OFT_X1_START (0)
#define SOC_MDC_DMA_OFT_X1_DMA_OFT_X1_END (11)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_oft_y1 : 16;
        unsigned int reserved : 16;
    } reg;
} soc_mdc_dma_oft_y1_union;
#endif
#define SOC_MDC_DMA_OFT_Y1_DMA_OFT_Y1_START (0)
#define SOC_MDC_DMA_OFT_Y1_DMA_OFT_Y1_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_mask_y0 : 16;
        unsigned int dma_mask_x0 : 12;
        unsigned int reserved : 4;
    } reg;
} soc_mdc_dma_mask0_union;
#endif
#define SOC_MDC_DMA_MASK0_DMA_MASK_Y0_START (0)
#define SOC_MDC_DMA_MASK0_DMA_MASK_Y0_END (15)
#define SOC_MDC_DMA_MASK0_DMA_MASK_X0_START (16)
#define SOC_MDC_DMA_MASK0_DMA_MASK_X0_END (27)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_mask_y1 : 16;
        unsigned int dma_mask_x1 : 12;
        unsigned int reserved : 4;
    } reg;
} soc_mdc_dma_mask1_union;
#endif
#define SOC_MDC_DMA_MASK1_DMA_MASK_Y1_START (0)
#define SOC_MDC_DMA_MASK1_DMA_MASK_Y1_END (15)
#define SOC_MDC_DMA_MASK1_DMA_MASK_X1_START (16)
#define SOC_MDC_DMA_MASK1_DMA_MASK_X1_END (27)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_stretch_size_vrt : 13;
        unsigned int dma_line_skip_step : 6;
        unsigned int reserved : 13;
    } reg;
} soc_mdc_dma_stretch_size_vrt_union;
#endif
#define SOC_MDC_DMA_STRETCH_SIZE_VRT_DMA_STRETCH_SIZE_VRT_START (0)
#define SOC_MDC_DMA_STRETCH_SIZE_VRT_DMA_STRETCH_SIZE_VRT_END (12)
#define SOC_MDC_DMA_STRETCH_SIZE_VRT_DMA_LINE_SKIP_STEP_START (13)
#define SOC_MDC_DMA_STRETCH_SIZE_VRT_DMA_LINE_SKIP_STEP_END (18)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_page_8k : 1;
        unsigned int dma_tile_en : 1;
        unsigned int reserved_0 : 1;
        unsigned int dma_pix_fmt : 5;
        unsigned int dma_va_en : 1;
        unsigned int dma_mirror_hrz_en : 1;
        unsigned int dma_mirror_vrt_en : 1;
        unsigned int dma_rot : 1;
        unsigned int dma_stretch_en : 1;
        unsigned int reserved_1 : 4;
        unsigned int dma_mask_en : 1;
        unsigned int dma_phy_scrambl_4 : 1;
        unsigned int dma_phy_scrambl_3 : 1;
        unsigned int dma_phy_scrambl_2 : 1;
        unsigned int dma_phy_scrambl_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int dma_cross_128 : 1;
        unsigned int dma_tile_128byte_en : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 6;
    } reg;
} soc_mdc_dma_ctrl_union;
#endif
#define SOC_MDC_DMA_CTRL_DMA_PAGE_8K_START (0)
#define SOC_MDC_DMA_CTRL_DMA_PAGE_8K_END (0)
#define SOC_MDC_DMA_CTRL_DMA_TILE_EN_START (1)
#define SOC_MDC_DMA_CTRL_DMA_TILE_EN_END (1)
#define SOC_MDC_DMA_CTRL_DMA_PIX_FMT_START (3)
#define SOC_MDC_DMA_CTRL_DMA_PIX_FMT_END (7)
#define SOC_MDC_DMA_CTRL_DMA_VA_EN_START (8)
#define SOC_MDC_DMA_CTRL_DMA_VA_EN_END (8)
#define SOC_MDC_DMA_CTRL_DMA_MIRROR_HRZ_EN_START (9)
#define SOC_MDC_DMA_CTRL_DMA_MIRROR_HRZ_EN_END (9)
#define SOC_MDC_DMA_CTRL_DMA_MIRROR_VRT_EN_START (10)
#define SOC_MDC_DMA_CTRL_DMA_MIRROR_VRT_EN_END (10)
#define SOC_MDC_DMA_CTRL_DMA_ROT_START (11)
#define SOC_MDC_DMA_CTRL_DMA_ROT_END (11)
#define SOC_MDC_DMA_CTRL_DMA_STRETCH_EN_START (12)
#define SOC_MDC_DMA_CTRL_DMA_STRETCH_EN_END (12)
#define SOC_MDC_DMA_CTRL_DMA_MASK_EN_START (17)
#define SOC_MDC_DMA_CTRL_DMA_MASK_EN_END (17)
#define SOC_MDC_DMA_CTRL_DMA_PHY_SCRAMBL_4_START (18)
#define SOC_MDC_DMA_CTRL_DMA_PHY_SCRAMBL_4_END (18)
#define SOC_MDC_DMA_CTRL_DMA_PHY_SCRAMBL_3_START (19)
#define SOC_MDC_DMA_CTRL_DMA_PHY_SCRAMBL_3_END (19)
#define SOC_MDC_DMA_CTRL_DMA_PHY_SCRAMBL_2_START (20)
#define SOC_MDC_DMA_CTRL_DMA_PHY_SCRAMBL_2_END (20)
#define SOC_MDC_DMA_CTRL_DMA_PHY_SCRAMBL_1_START (21)
#define SOC_MDC_DMA_CTRL_DMA_PHY_SCRAMBL_1_END (21)
#define SOC_MDC_DMA_CTRL_DMA_CROSS_128_START (23)
#define SOC_MDC_DMA_CTRL_DMA_CROSS_128_END (23)
#define SOC_MDC_DMA_CTRL_DMA_TILE_128BYTE_EN_START (24)
#define SOC_MDC_DMA_CTRL_DMA_TILE_128BYTE_EN_END (24)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_tile_scrambling_en : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_dma_tile_scram_union;
#endif
#define SOC_MDC_DMA_TILE_SCRAM_DMA_TILE_SCRAMBLING_EN_START (0)
#define SOC_MDC_DMA_TILE_SCRAM_DMA_TILE_SCRAMBLING_EN_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_pulse : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_dma_pulse_union;
#endif
#define SOC_MDC_DMA_PULSE_DMA_PULSE_START (0)
#define SOC_MDC_DMA_PULSE_DMA_PULSE_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int rwch_cfg0 : 32;
    } reg;
} soc_mdc_rwch_cfg0_union;
#endif
#define SOC_MDC_RWCH_CFG0_RWCH_CFG0_START (0)
#define SOC_MDC_RWCH_CFG0_RWCH_CFG0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_addr0 : 32;
    } reg;
} soc_mdc_dma_data_addr0_union;
#endif
#define SOC_MDC_DMA_DATA_ADDR0_DMA_ADDR0_START (0)
#define SOC_MDC_DMA_DATA_ADDR0_DMA_ADDR0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_stride0 : 13;
        unsigned int reserved_0 : 3;
        unsigned int dma_l2t_interleave_n0 : 4;
        unsigned int reserved_1 : 12;
    } reg;
} soc_mdc_dma_stride0_union;
#endif
#define SOC_MDC_DMA_STRIDE0_DMA_STRIDE0_START (0)
#define SOC_MDC_DMA_STRIDE0_DMA_STRIDE0_END (12)
#define SOC_MDC_DMA_STRIDE0_DMA_L2T_INTERLEAVE_N0_START (16)
#define SOC_MDC_DMA_STRIDE0_DMA_L2T_INTERLEAVE_N0_END (19)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_stretch_stride0 : 19;
        unsigned int reserved : 13;
    } reg;
} soc_mdc_dma_stretch_stride0_union;
#endif
#define SOC_MDC_DMA_STRETCH_STRIDE0_DMA_STRETCH_STRIDE0_START (0)
#define SOC_MDC_DMA_STRETCH_STRIDE0_DMA_STRETCH_STRIDE0_END (18)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_data_num0 : 30;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_dma_data_num0_union;
#endif
#define SOC_MDC_DMA_DATA_NUM0_DMA_DATA_NUM0_START (0)
#define SOC_MDC_DMA_DATA_NUM0_DMA_DATA_NUM0_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_test0_y : 32;
    } reg;
} soc_mdc_dma_test0_union;
#endif
#define SOC_MDC_DMA_TEST0_DMA_TEST0_Y_START (0)
#define SOC_MDC_DMA_TEST0_DMA_TEST0_Y_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_test1_y : 32;
    } reg;
} soc_mdc_dma_test1_union;
#endif
#define SOC_MDC_DMA_TEST1_DMA_TEST1_Y_START (0)
#define SOC_MDC_DMA_TEST1_DMA_TEST1_Y_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_test3_y : 32;
    } reg;
} soc_mdc_dma_test3_union;
#endif
#define SOC_MDC_DMA_TEST3_DMA_TEST3_Y_START (0)
#define SOC_MDC_DMA_TEST3_DMA_TEST3_Y_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_test4_y : 32;
    } reg;
} soc_mdc_dma_test4_union;
#endif
#define SOC_MDC_DMA_TEST4_DMA_TEST4_Y_START (0)
#define SOC_MDC_DMA_TEST4_DMA_TEST4_Y_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_status_y : 32;
    } reg;
} soc_mdc_dma_status_y_union;
#endif
#define SOC_MDC_DMA_STATUS_Y_DMA_STATUS_Y_START (0)
#define SOC_MDC_DMA_STATUS_Y_DMA_STATUS_Y_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_addr1 : 32;
    } reg;
} soc_mdc_dma_data_addr1_union;
#endif
#define SOC_MDC_DMA_DATA_ADDR1_DMA_ADDR1_START (0)
#define SOC_MDC_DMA_DATA_ADDR1_DMA_ADDR1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_stride1 : 13;
        unsigned int reserved_0 : 3;
        unsigned int dma_l2t_interleave_n1 : 4;
        unsigned int reserved_1 : 12;
    } reg;
} soc_mdc_dma_stride1_union;
#endif
#define SOC_MDC_DMA_STRIDE1_DMA_STRIDE1_START (0)
#define SOC_MDC_DMA_STRIDE1_DMA_STRIDE1_END (12)
#define SOC_MDC_DMA_STRIDE1_DMA_L2T_INTERLEAVE_N1_START (16)
#define SOC_MDC_DMA_STRIDE1_DMA_L2T_INTERLEAVE_N1_END (19)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_stretch_stride1 : 19;
        unsigned int reserved : 13;
    } reg;
} soc_mdc_dma_stretch_stride1_union;
#endif
#define SOC_MDC_DMA_STRETCH_STRIDE1_DMA_STRETCH_STRIDE1_START (0)
#define SOC_MDC_DMA_STRETCH_STRIDE1_DMA_STRETCH_STRIDE1_END (18)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_data_num1 : 30;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_dma_data_num1_union;
#endif
#define SOC_MDC_DMA_DATA_NUM1_DMA_DATA_NUM1_START (0)
#define SOC_MDC_DMA_DATA_NUM1_DMA_DATA_NUM1_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_test0_u : 32;
    } reg;
} soc_mdc_dma_test0_u_union;
#endif
#define SOC_MDC_DMA_TEST0_U_DMA_TEST0_U_START (0)
#define SOC_MDC_DMA_TEST0_U_DMA_TEST0_U_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_test1_u : 32;
    } reg;
} soc_mdc_dma_test1_u_union;
#endif
#define SOC_MDC_DMA_TEST1_U_DMA_TEST1_U_START (0)
#define SOC_MDC_DMA_TEST1_U_DMA_TEST1_U_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_test3_u : 32;
    } reg;
} soc_mdc_dma_test3_u_union;
#endif
#define SOC_MDC_DMA_TEST3_U_DMA_TEST3_U_START (0)
#define SOC_MDC_DMA_TEST3_U_DMA_TEST3_U_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_test4_u : 32;
    } reg;
} soc_mdc_dma_test4_u_union;
#endif
#define SOC_MDC_DMA_TEST4_U_DMA_TEST4_U_START (0)
#define SOC_MDC_DMA_TEST4_U_DMA_TEST4_U_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_status_u : 32;
    } reg;
} soc_mdc_dma_status_u_union;
#endif
#define SOC_MDC_DMA_STATUS_U_DMA_STATUS_U_START (0)
#define SOC_MDC_DMA_STATUS_U_DMA_STATUS_U_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_addr2 : 32;
    } reg;
} soc_mdc_dma_data_addr2_union;
#endif
#define SOC_MDC_DMA_DATA_ADDR2_DMA_ADDR2_START (0)
#define SOC_MDC_DMA_DATA_ADDR2_DMA_ADDR2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_stride2 : 13;
        unsigned int reserved_0 : 3;
        unsigned int dma_l2t_interleave_n2 : 4;
        unsigned int reserved_1 : 12;
    } reg;
} soc_mdc_dma_stride2_union;
#endif
#define SOC_MDC_DMA_STRIDE2_DMA_STRIDE2_START (0)
#define SOC_MDC_DMA_STRIDE2_DMA_STRIDE2_END (12)
#define SOC_MDC_DMA_STRIDE2_DMA_L2T_INTERLEAVE_N2_START (16)
#define SOC_MDC_DMA_STRIDE2_DMA_L2T_INTERLEAVE_N2_END (19)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_stretch_stride2 : 19;
        unsigned int reserved : 13;
    } reg;
} soc_mdc_dma_stretch_stride2_union;
#endif
#define SOC_MDC_DMA_STRETCH_STRIDE2_DMA_STRETCH_STRIDE2_START (0)
#define SOC_MDC_DMA_STRETCH_STRIDE2_DMA_STRETCH_STRIDE2_END (18)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_data_num2 : 30;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_dma_data_num2_union;
#endif
#define SOC_MDC_DMA_DATA_NUM2_DMA_DATA_NUM2_START (0)
#define SOC_MDC_DMA_DATA_NUM2_DMA_DATA_NUM2_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_test0_v : 32;
    } reg;
} soc_mdc_dma_test0_v_union;
#endif
#define SOC_MDC_DMA_TEST0_V_DMA_TEST0_V_START (0)
#define SOC_MDC_DMA_TEST0_V_DMA_TEST0_V_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_test1_v : 32;
    } reg;
} soc_mdc_dma_test1_v_union;
#endif
#define SOC_MDC_DMA_TEST1_V_DMA_TEST1_V_START (0)
#define SOC_MDC_DMA_TEST1_V_DMA_TEST1_V_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_test3_v : 32;
    } reg;
} soc_mdc_dma_test3_v_union;
#endif
#define SOC_MDC_DMA_TEST3_V_DMA_TEST3_V_START (0)
#define SOC_MDC_DMA_TEST3_V_DMA_TEST3_V_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_test4_v : 32;
    } reg;
} soc_mdc_dma_test4_v_union;
#endif
#define SOC_MDC_DMA_TEST4_V_DMA_TEST4_V_START (0)
#define SOC_MDC_DMA_TEST4_V_DMA_TEST4_V_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_status_v : 32;
    } reg;
} soc_mdc_dma_status_v_union;
#endif
#define SOC_MDC_DMA_STATUS_V_DMA_STATUS_V_START (0)
#define SOC_MDC_DMA_STATUS_V_DMA_STATUS_V_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int rwch_rd_shadow : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_ch_rd_shadow_union;
#endif
#define SOC_MDC_CH_RD_SHADOW_RWCH_RD_SHADOW_START (0)
#define SOC_MDC_CH_RD_SHADOW_RWCH_RD_SHADOW_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ch_en : 1;
        unsigned int ch_version_sel : 1;
        unsigned int ch_afbcd_en : 1;
        unsigned int ch_block_en : 1;
        unsigned int reserved_0 : 4;
        unsigned int ch_dmac_en : 1;
        unsigned int reserved_1 : 3;
        unsigned int ch_frm_end_dly : 4;
        unsigned int ch_block_h : 13;
        unsigned int reserved_2 : 3;
    } reg;
} soc_mdc_ch_ctl_union;
#endif
#define SOC_MDC_CH_CTL_CH_EN_START (0)
#define SOC_MDC_CH_CTL_CH_EN_END (0)
#define SOC_MDC_CH_CTL_CH_VERSION_SEL_START (1)
#define SOC_MDC_CH_CTL_CH_VERSION_SEL_END (1)
#define SOC_MDC_CH_CTL_CH_AFBCD_EN_START (2)
#define SOC_MDC_CH_CTL_CH_AFBCD_EN_END (2)
#define SOC_MDC_CH_CTL_CH_BLOCK_EN_START (3)
#define SOC_MDC_CH_CTL_CH_BLOCK_EN_END (3)
#define SOC_MDC_CH_CTL_CH_DMAC_EN_START (8)
#define SOC_MDC_CH_CTL_CH_DMAC_EN_END (8)
#define SOC_MDC_CH_CTL_CH_FRM_END_DLY_START (12)
#define SOC_MDC_CH_CTL_CH_FRM_END_DLY_END (15)
#define SOC_MDC_CH_CTL_CH_BLOCK_H_START (16)
#define SOC_MDC_CH_CTL_CH_BLOCK_H_END (28)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ch_secu_en : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_ch_secu_en_union;
#endif
#define SOC_MDC_CH_SECU_EN_CH_SECU_EN_START (0)
#define SOC_MDC_CH_SECU_EN_CH_SECU_EN_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ch_sw_end_req : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_ch_sw_end_req_union;
#endif
#define SOC_MDC_CH_SW_END_REQ_CH_SW_END_REQ_START (0)
#define SOC_MDC_CH_SW_END_REQ_CH_SW_END_REQ_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ch_clk_sel : 32;
    } reg;
} soc_mdc_ch_clk_sel_union;
#endif
#define SOC_MDC_CH_CLK_SEL_CH_CLK_SEL_START (0)
#define SOC_MDC_CH_CLK_SEL_CH_CLK_SEL_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ch_clk_en : 32;
    } reg;
} soc_mdc_ch_clk_en_union;
#endif
#define SOC_MDC_CH_CLK_EN_CH_CLK_EN_START (0)
#define SOC_MDC_CH_CLK_EN_CH_CLK_EN_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ch_dbg0 : 32;
    } reg;
} soc_mdc_ch_dbg0_union;
#endif
#define SOC_MDC_CH_DBG0_CH_DBG0_START (0)
#define SOC_MDC_CH_DBG0_CH_DBG0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ch_dbg1 : 32;
    } reg;
} soc_mdc_ch_dbg1_union;
#endif
#define SOC_MDC_CH_DBG1_CH_DBG1_START (0)
#define SOC_MDC_CH_DBG1_CH_DBG1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ch_dbg2 : 32;
    } reg;
} soc_mdc_ch_dbg2_union;
#endif
#define SOC_MDC_CH_DBG2_CH_DBG2_START (0)
#define SOC_MDC_CH_DBG2_CH_DBG2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ch_dbg3 : 32;
    } reg;
} soc_mdc_ch_dbg3_union;
#endif
#define SOC_MDC_CH_DBG3_CH_DBG3_START (0)
#define SOC_MDC_CH_DBG3_CH_DBG3_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ch_dbg4 : 32;
    } reg;
} soc_mdc_ch_dbg4_union;
#endif
#define SOC_MDC_CH_DBG4_CH_DBG4_START (0)
#define SOC_MDC_CH_DBG4_CH_DBG4_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ch_dbg5 : 32;
    } reg;
} soc_mdc_ch_dbg5_union;
#endif
#define SOC_MDC_CH_DBG5_CH_DBG5_START (0)
#define SOC_MDC_CH_DBG5_CH_DBG5_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bitext_en : 1;
        unsigned int bitext_rgb_en : 1;
        unsigned int bitext_alpha_en : 1;
        unsigned int bitext_reg_ini_cfg_en : 1;
        unsigned int reserved : 28;
    } reg;
} soc_mdc_bitext_ctl_union;
#endif
#define SOC_MDC_BITEXT_CTL_BITEXT_EN_START (0)
#define SOC_MDC_BITEXT_CTL_BITEXT_EN_END (0)
#define SOC_MDC_BITEXT_CTL_BITEXT_RGB_EN_START (1)
#define SOC_MDC_BITEXT_CTL_BITEXT_RGB_EN_END (1)
#define SOC_MDC_BITEXT_CTL_BITEXT_ALPHA_EN_START (2)
#define SOC_MDC_BITEXT_CTL_BITEXT_ALPHA_EN_END (2)
#define SOC_MDC_BITEXT_CTL_BITEXT_REG_INI_CFG_EN_START (3)
#define SOC_MDC_BITEXT_CTL_BITEXT_REG_INI_CFG_EN_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bitext_reg_ini0_0 : 32;
    } reg;
} soc_mdc_bitext_reg_ini0_0_union;
#endif
#define SOC_MDC_BITEXT_REG_INI0_0_BITEXT_REG_INI0_0_START (0)
#define SOC_MDC_BITEXT_REG_INI0_0_BITEXT_REG_INI0_0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bitext_reg_ini0_1 : 32;
    } reg;
} soc_mdc_bitext_reg_ini0_1_union;
#endif
#define SOC_MDC_BITEXT_REG_INI0_1_BITEXT_REG_INI0_1_START (0)
#define SOC_MDC_BITEXT_REG_INI0_1_BITEXT_REG_INI0_1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bitext_reg_ini0_2 : 32;
    } reg;
} soc_mdc_bitext_reg_ini0_2_union;
#endif
#define SOC_MDC_BITEXT_REG_INI0_2_BITEXT_REG_INI0_2_START (0)
#define SOC_MDC_BITEXT_REG_INI0_2_BITEXT_REG_INI0_2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bitext_reg_ini0_3 : 31;
        unsigned int reserved : 1;
    } reg;
} soc_mdc_bitext_reg_ini0_3_union;
#endif
#define SOC_MDC_BITEXT_REG_INI0_3_BITEXT_REG_INI0_3_START (0)
#define SOC_MDC_BITEXT_REG_INI0_3_BITEXT_REG_INI0_3_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bitext_reg_ini1_0 : 32;
    } reg;
} soc_mdc_bitext_reg_ini1_0_union;
#endif
#define SOC_MDC_BITEXT_REG_INI1_0_BITEXT_REG_INI1_0_START (0)
#define SOC_MDC_BITEXT_REG_INI1_0_BITEXT_REG_INI1_0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bitext_reg_ini1_1 : 32;
    } reg;
} soc_mdc_bitext_reg_ini1_1_union;
#endif
#define SOC_MDC_BITEXT_REG_INI1_1_BITEXT_REG_INI1_1_START (0)
#define SOC_MDC_BITEXT_REG_INI1_1_BITEXT_REG_INI1_1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bitext_reg_ini1_2 : 32;
    } reg;
} soc_mdc_bitext_reg_ini1_2_union;
#endif
#define SOC_MDC_BITEXT_REG_INI1_2_BITEXT_REG_INI1_2_START (0)
#define SOC_MDC_BITEXT_REG_INI1_2_BITEXT_REG_INI1_2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bitext_reg_ini1_3 : 31;
        unsigned int reserved : 1;
    } reg;
} soc_mdc_bitext_reg_ini1_3_union;
#endif
#define SOC_MDC_BITEXT_REG_INI1_3_BITEXT_REG_INI1_3_START (0)
#define SOC_MDC_BITEXT_REG_INI1_3_BITEXT_REG_INI1_3_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bitext_reg_ini2_0 : 32;
    } reg;
} soc_mdc_bitext_reg_ini2_0_union;
#endif
#define SOC_MDC_BITEXT_REG_INI2_0_BITEXT_REG_INI2_0_START (0)
#define SOC_MDC_BITEXT_REG_INI2_0_BITEXT_REG_INI2_0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bitext_reg_ini2_1 : 32;
    } reg;
} soc_mdc_bitext_reg_ini2_1_union;
#endif
#define SOC_MDC_BITEXT_REG_INI2_1_BITEXT_REG_INI2_1_START (0)
#define SOC_MDC_BITEXT_REG_INI2_1_BITEXT_REG_INI2_1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bitext_reg_ini2_2 : 32;
    } reg;
} soc_mdc_bitext_reg_ini2_2_union;
#endif
#define SOC_MDC_BITEXT_REG_INI2_2_BITEXT_REG_INI2_2_START (0)
#define SOC_MDC_BITEXT_REG_INI2_2_BITEXT_REG_INI2_2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bitext_reg_ini2_3 : 31;
        unsigned int reserved : 1;
    } reg;
} soc_mdc_bitext_reg_ini2_3_union;
#endif
#define SOC_MDC_BITEXT_REG_INI2_3_BITEXT_REG_INI2_3_START (0)
#define SOC_MDC_BITEXT_REG_INI2_3_BITEXT_REG_INI2_3_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bitext_power_ctrl_c : 2;
        unsigned int reserved : 30;
    } reg;
} soc_mdc_bitext_power_ctrl_c_union;
#endif
#define SOC_MDC_BITEXT_POWER_CTRL_C_BITEXT_POWER_CTRL_C_START (0)
#define SOC_MDC_BITEXT_POWER_CTRL_C_BITEXT_POWER_CTRL_C_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bitext_power_ctrl_shift : 2;
        unsigned int reserved : 30;
    } reg;
} soc_mdc_bitext_power_ctrl_shift_union;
#endif
#define SOC_MDC_BITEXT_POWER_CTRL_SHIFT_BITEXT_POWER_CTRL_SHIFT_START (0)
#define SOC_MDC_BITEXT_POWER_CTRL_SHIFT_BITEXT_POWER_CTRL_SHIFT_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bitext_filt_00 : 5;
        unsigned int reserved : 27;
    } reg;
} soc_mdc_bitext_filt_00_union;
#endif
#define SOC_MDC_BITEXT_FILT_00_BITEXT_FILT_00_START (0)
#define SOC_MDC_BITEXT_FILT_00_BITEXT_FILT_00_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bitext_filt_01 : 5;
        unsigned int reserved : 27;
    } reg;
} soc_mdc_bitext_filt_01_union;
#endif
#define SOC_MDC_BITEXT_FILT_01_BITEXT_FILT_01_START (0)
#define SOC_MDC_BITEXT_FILT_01_BITEXT_FILT_01_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bitext_filt_02 : 5;
        unsigned int reserved : 27;
    } reg;
} soc_mdc_bitext_filt_02_union;
#endif
#define SOC_MDC_BITEXT_FILT_02_BITEXT_FILT_02_START (0)
#define SOC_MDC_BITEXT_FILT_02_BITEXT_FILT_02_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bitext_filt_10 : 5;
        unsigned int reserved : 27;
    } reg;
} soc_mdc_bitext_filt_10_union;
#endif
#define SOC_MDC_BITEXT_FILT_10_BITEXT_FILT_10_START (0)
#define SOC_MDC_BITEXT_FILT_10_BITEXT_FILT_10_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bitext_filt_11 : 5;
        unsigned int reserved : 27;
    } reg;
} soc_mdc_bitext_filt_11_union;
#endif
#define SOC_MDC_BITEXT_FILT_11_BITEXT_FILT_11_START (0)
#define SOC_MDC_BITEXT_FILT_11_BITEXT_FILT_11_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bitext_filt_12 : 5;
        unsigned int reserved : 27;
    } reg;
} soc_mdc_bitext_filt_12_union;
#endif
#define SOC_MDC_BITEXT_FILT_12_BITEXT_FILT_12_START (0)
#define SOC_MDC_BITEXT_FILT_12_BITEXT_FILT_12_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bitext_filt_20 : 5;
        unsigned int reserved : 27;
    } reg;
} soc_mdc_bitext_filt_20_union;
#endif
#define SOC_MDC_BITEXT_FILT_20_BITEXT_FILT_20_START (0)
#define SOC_MDC_BITEXT_FILT_20_BITEXT_FILT_20_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bitext_filt_21 : 5;
        unsigned int reserved : 27;
    } reg;
} soc_mdc_bitext_filt_21_union;
#endif
#define SOC_MDC_BITEXT_FILT_21_BITEXT_FILT_21_START (0)
#define SOC_MDC_BITEXT_FILT_21_BITEXT_FILT_21_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bitext_filt_22 : 5;
        unsigned int reserved : 27;
    } reg;
} soc_mdc_bitext_filt_22_union;
#endif
#define SOC_MDC_BITEXT_FILT_22_BITEXT_FILT_22_START (0)
#define SOC_MDC_BITEXT_FILT_22_BITEXT_FILT_22_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bitext_thd_r0 : 32;
    } reg;
} soc_mdc_bitext_thd_r0_union;
#endif
#define SOC_MDC_BITEXT_THD_R0_BITEXT_THD_R0_START (0)
#define SOC_MDC_BITEXT_THD_R0_BITEXT_THD_R0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bitext_thd_r1 : 16;
        unsigned int reserved : 16;
    } reg;
} soc_mdc_bitext_thd_r1_union;
#endif
#define SOC_MDC_BITEXT_THD_R1_BITEXT_THD_R1_START (0)
#define SOC_MDC_BITEXT_THD_R1_BITEXT_THD_R1_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bitext_thd_g0 : 32;
    } reg;
} soc_mdc_bitext_thd_g0_union;
#endif
#define SOC_MDC_BITEXT_THD_G0_BITEXT_THD_G0_START (0)
#define SOC_MDC_BITEXT_THD_G0_BITEXT_THD_G0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bitext_thd_g1 : 16;
        unsigned int reserved : 16;
    } reg;
} soc_mdc_bitext_thd_g1_union;
#endif
#define SOC_MDC_BITEXT_THD_G1_BITEXT_THD_G1_START (0)
#define SOC_MDC_BITEXT_THD_G1_BITEXT_THD_G1_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bitext_thd_b0 : 32;
    } reg;
} soc_mdc_bitext_thd_b0_union;
#endif
#define SOC_MDC_BITEXT_THD_B0_BITEXT_THD_B0_START (0)
#define SOC_MDC_BITEXT_THD_B0_BITEXT_THD_B0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bitext_thd_b1 : 16;
        unsigned int reserved : 16;
    } reg;
} soc_mdc_bitext_thd_b1_union;
#endif
#define SOC_MDC_BITEXT_THD_B1_BITEXT_THD_B1_START (0)
#define SOC_MDC_BITEXT_THD_B1_BITEXT_THD_B1_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bitext_dbg0 : 32;
    } reg;
} soc_mdc_bitext0_dbg0_union;
#endif
#define SOC_MDC_BITEXT0_DBG0_BITEXT_DBG0_START (0)
#define SOC_MDC_BITEXT0_DBG0_BITEXT_DBG0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int scf_en_hscl_str : 1;
        unsigned int scf_en_hscl_str_a : 1;
        unsigned int reserved : 30;
    } reg;
} soc_mdc_scf_en_hscl_str_union;
#endif
#define SOC_MDC_SCF_EN_HSCL_STR_SCF_EN_HSCL_STR_START (0)
#define SOC_MDC_SCF_EN_HSCL_STR_SCF_EN_HSCL_STR_END (0)
#define SOC_MDC_SCF_EN_HSCL_STR_SCF_EN_HSCL_STR_A_START (1)
#define SOC_MDC_SCF_EN_HSCL_STR_SCF_EN_HSCL_STR_A_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int scf_en_vscl_str : 1;
        unsigned int scf_en_vscl_str_a : 1;
        unsigned int reserved : 30;
    } reg;
} soc_mdc_scf_en_vscl_str_union;
#endif
#define SOC_MDC_SCF_EN_VSCL_STR_SCF_EN_VSCL_STR_START (0)
#define SOC_MDC_SCF_EN_VSCL_STR_SCF_EN_VSCL_STR_END (0)
#define SOC_MDC_SCF_EN_VSCL_STR_SCF_EN_VSCL_STR_A_START (1)
#define SOC_MDC_SCF_EN_VSCL_STR_SCF_EN_VSCL_STR_A_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int scf_h_v_order : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_scf_h_v_order_union;
#endif
#define SOC_MDC_SCF_H_V_ORDER_SCF_H_V_ORDER_START (0)
#define SOC_MDC_SCF_H_V_ORDER_SCF_H_V_ORDER_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int scf_a_core_gt : 1;
        unsigned int scf_r_core_gt : 1;
        unsigned int scf_g_core_gt : 1;
        unsigned int scf_b_core_gt : 1;
        unsigned int reserved : 28;
    } reg;
} soc_mdc_scf_ch_core_gt_union;
#endif
#define SOC_MDC_SCF_CH_CORE_GT_SCF_A_CORE_GT_START (0)
#define SOC_MDC_SCF_CH_CORE_GT_SCF_A_CORE_GT_END (0)
#define SOC_MDC_SCF_CH_CORE_GT_SCF_R_CORE_GT_START (1)
#define SOC_MDC_SCF_CH_CORE_GT_SCF_R_CORE_GT_END (1)
#define SOC_MDC_SCF_CH_CORE_GT_SCF_G_CORE_GT_START (2)
#define SOC_MDC_SCF_CH_CORE_GT_SCF_G_CORE_GT_END (2)
#define SOC_MDC_SCF_CH_CORE_GT_SCF_B_CORE_GT_START (3)
#define SOC_MDC_SCF_CH_CORE_GT_SCF_B_CORE_GT_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int scf_input_height : 13;
        unsigned int reserved_0 : 3;
        unsigned int scf_input_width : 13;
        unsigned int reserved_1 : 3;
    } reg;
} soc_mdc_scf_input_width_height_union;
#endif
#define SOC_MDC_SCF_INPUT_WIDTH_HEIGHT_SCF_INPUT_HEIGHT_START (0)
#define SOC_MDC_SCF_INPUT_WIDTH_HEIGHT_SCF_INPUT_HEIGHT_END (12)
#define SOC_MDC_SCF_INPUT_WIDTH_HEIGHT_SCF_INPUT_WIDTH_START (16)
#define SOC_MDC_SCF_INPUT_WIDTH_HEIGHT_SCF_INPUT_WIDTH_END (28)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int scf_output_height : 13;
        unsigned int reserved_0 : 3;
        unsigned int scf_output_width : 13;
        unsigned int reserved_1 : 3;
    } reg;
} soc_mdc_scf_output_width_height_union;
#endif
#define SOC_MDC_SCF_OUTPUT_WIDTH_HEIGHT_SCF_OUTPUT_HEIGHT_START (0)
#define SOC_MDC_SCF_OUTPUT_WIDTH_HEIGHT_SCF_OUTPUT_HEIGHT_END (12)
#define SOC_MDC_SCF_OUTPUT_WIDTH_HEIGHT_SCF_OUTPUT_WIDTH_START (16)
#define SOC_MDC_SCF_OUTPUT_WIDTH_HEIGHT_SCF_OUTPUT_WIDTH_END (28)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int scf_hcoef_mem_ctrl : 4;
        unsigned int scf_vcoef_mem_ctrl : 4;
        unsigned int reserved : 24;
    } reg;
} soc_mdc_scf_coef_mem_ctrl_union;
#endif
#define SOC_MDC_SCF_COEF_MEM_CTRL_SCF_HCOEF_MEM_CTRL_START (0)
#define SOC_MDC_SCF_COEF_MEM_CTRL_SCF_HCOEF_MEM_CTRL_END (3)
#define SOC_MDC_SCF_COEF_MEM_CTRL_SCF_VCOEF_MEM_CTRL_START (4)
#define SOC_MDC_SCF_COEF_MEM_CTRL_SCF_VCOEF_MEM_CTRL_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int scf_en_hscl_en : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_scf_en_hscl_union;
#endif
#define SOC_MDC_SCF_EN_HSCL_SCF_EN_HSCL_EN_START (0)
#define SOC_MDC_SCF_EN_HSCL_SCF_EN_HSCL_EN_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int scf_en_vscl_en : 1;
        unsigned int scf_out_buffer_en : 1;
        unsigned int reserved : 30;
    } reg;
} soc_mdc_scf_en_vscl_union;
#endif
#define SOC_MDC_SCF_EN_VSCL_SCF_EN_VSCL_EN_START (0)
#define SOC_MDC_SCF_EN_VSCL_SCF_EN_VSCL_EN_END (0)
#define SOC_MDC_SCF_EN_VSCL_SCF_OUT_BUFFER_EN_START (1)
#define SOC_MDC_SCF_EN_VSCL_SCF_OUT_BUFFER_EN_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int scf_acc_hscl : 30;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_scf_acc_hscl_union;
#endif
#define SOC_MDC_SCF_ACC_HSCL_SCF_ACC_HSCL_START (0)
#define SOC_MDC_SCF_ACC_HSCL_SCF_ACC_HSCL_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int scf_acc_hscl1 : 19;
        unsigned int reserved : 13;
    } reg;
} soc_mdc_scf_acc_hscl1_union;
#endif
#define SOC_MDC_SCF_ACC_HSCL1_SCF_ACC_HSCL1_START (0)
#define SOC_MDC_SCF_ACC_HSCL1_SCF_ACC_HSCL1_END (18)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int scf_inc_hscl : 24;
        unsigned int reserved : 8;
    } reg;
} soc_mdc_scf_inc_hscl_union;
#endif
#define SOC_MDC_SCF_INC_HSCL_SCF_INC_HSCL_START (0)
#define SOC_MDC_SCF_INC_HSCL_SCF_INC_HSCL_END (23)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int scf_acc_vscl : 30;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_scf_acc_vscl_union;
#endif
#define SOC_MDC_SCF_ACC_VSCL_SCF_ACC_VSCL_START (0)
#define SOC_MDC_SCF_ACC_VSCL_SCF_ACC_VSCL_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int scf_acc_vscl1 : 19;
        unsigned int reserved : 13;
    } reg;
} soc_mdc_scf_acc_vscl1_union;
#endif
#define SOC_MDC_SCF_ACC_VSCL1_SCF_ACC_VSCL1_START (0)
#define SOC_MDC_SCF_ACC_VSCL1_SCF_ACC_VSCL1_END (18)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int scf_inc_vscl : 24;
        unsigned int reserved : 8;
    } reg;
} soc_mdc_scf_inc_vscl_union;
#endif
#define SOC_MDC_SCF_INC_VSCL_SCF_INC_VSCL_START (0)
#define SOC_MDC_SCF_INC_VSCL_SCF_INC_VSCL_END (23)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int scf_sw_rst : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_scf_en_nonlinear_union;
#endif
#define SOC_MDC_SCF_EN_NONLINEAR_SCF_SW_RST_START (0)
#define SOC_MDC_SCF_EN_NONLINEAR_SCF_SW_RST_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int scf_en_mmp : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_scf_en_mmp_union;
#endif
#define SOC_MDC_SCF_EN_MMP_SCF_EN_MMP_START (0)
#define SOC_MDC_SCF_EN_MMP_SCF_EN_MMP_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int scf_debug_h0 : 32;
    } reg;
} soc_mdc_scf_db_h0_union;
#endif
#define SOC_MDC_SCF_DB_H0_SCF_DEBUG_H0_START (0)
#define SOC_MDC_SCF_DB_H0_SCF_DEBUG_H0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int scf_debug_h1 : 32;
    } reg;
} soc_mdc_scf_db_h1_union;
#endif
#define SOC_MDC_SCF_DB_H1_SCF_DEBUG_H1_START (0)
#define SOC_MDC_SCF_DB_H1_SCF_DEBUG_H1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int scf_debug_v0 : 32;
    } reg;
} soc_mdc_scf_db_v0_union;
#endif
#define SOC_MDC_SCF_DB_V0_SCF_DEBUG_V0_START (0)
#define SOC_MDC_SCF_DB_V0_SCF_DEBUG_V0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int scf_debug_v1 : 32;
    } reg;
} soc_mdc_scf_db_v1_union;
#endif
#define SOC_MDC_SCF_DB_V1_SCF_DEBUG_V1_START (0)
#define SOC_MDC_SCF_DB_V1_SCF_DEBUG_V1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int scf_lb_mem_ctrl : 4;
        unsigned int reserved : 28;
    } reg;
} soc_mdc_scf_lb_mem_ctrl_union;
#endif
#define SOC_MDC_SCF_LB_MEM_CTRL_SCF_LB_MEM_CTRL_START (0)
#define SOC_MDC_SCF_LB_MEM_CTRL_SCF_LB_MEM_CTRL_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int arsr_input_height : 13;
        unsigned int reserved_0 : 3;
        unsigned int arsr_input_width : 13;
        unsigned int reserved_1 : 3;
    } reg;
} soc_mdc_arsr_input_width_height_union;
#endif
#define SOC_MDC_ARSR_INPUT_WIDTH_HEIGHT_ARSR_INPUT_HEIGHT_START (0)
#define SOC_MDC_ARSR_INPUT_WIDTH_HEIGHT_ARSR_INPUT_HEIGHT_END (12)
#define SOC_MDC_ARSR_INPUT_WIDTH_HEIGHT_ARSR_INPUT_WIDTH_START (16)
#define SOC_MDC_ARSR_INPUT_WIDTH_HEIGHT_ARSR_INPUT_WIDTH_END (28)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int arsr_output_height : 13;
        unsigned int reserved_0 : 3;
        unsigned int arsr_output_width : 13;
        unsigned int reserved_1 : 3;
    } reg;
} soc_mdc_arsr_output_width_height_union;
#endif
#define SOC_MDC_ARSR_OUTPUT_WIDTH_HEIGHT_ARSR_OUTPUT_HEIGHT_START (0)
#define SOC_MDC_ARSR_OUTPUT_WIDTH_HEIGHT_ARSR_OUTPUT_HEIGHT_END (12)
#define SOC_MDC_ARSR_OUTPUT_WIDTH_HEIGHT_ARSR_OUTPUT_WIDTH_START (16)
#define SOC_MDC_ARSR_OUTPUT_WIDTH_HEIGHT_ARSR_OUTPUT_WIDTH_END (28)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ihleft : 29;
        unsigned int reserved : 3;
    } reg;
} soc_mdc_ihleft_union;
#endif
#define SOC_MDC_IHLEFT_IHLEFT_START (0)
#define SOC_MDC_IHLEFT_IHLEFT_END (28)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ihleft1 : 29;
        unsigned int reserved : 3;
    } reg;
} soc_mdc_ihleft1_union;
#endif
#define SOC_MDC_IHLEFT1_IHLEFT1_START (0)
#define SOC_MDC_IHLEFT1_IHLEFT1_END (28)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ihright : 29;
        unsigned int reserved : 3;
    } reg;
} soc_mdc_ihright_union;
#endif
#define SOC_MDC_IHRIGHT_IHRIGHT_START (0)
#define SOC_MDC_IHRIGHT_IHRIGHT_END (28)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ihright1 : 29;
        unsigned int reserved : 3;
    } reg;
} soc_mdc_ihright1_union;
#endif
#define SOC_MDC_IHRIGHT1_IHRIGHT1_START (0)
#define SOC_MDC_IHRIGHT1_IHRIGHT1_END (28)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ivtop : 29;
        unsigned int reserved : 3;
    } reg;
} soc_mdc_ivtop_union;
#endif
#define SOC_MDC_IVTOP_IVTOP_START (0)
#define SOC_MDC_IVTOP_IVTOP_END (28)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ivbottom : 29;
        unsigned int reserved : 3;
    } reg;
} soc_mdc_ivbottom_union;
#endif
#define SOC_MDC_IVBOTTOM_IVBOTTOM_START (0)
#define SOC_MDC_IVBOTTOM_IVBOTTOM_END (28)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ivbottom1 : 29;
        unsigned int reserved : 3;
    } reg;
} soc_mdc_ivbottom1_union;
#endif
#define SOC_MDC_IVBOTTOM1_IVBOTTOM1_START (0)
#define SOC_MDC_IVBOTTOM1_IVBOTTOM1_END (28)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ihinc : 22;
        unsigned int reserved : 10;
    } reg;
} soc_mdc_ihinc_union;
#endif
#define SOC_MDC_IHINC_IHINC_START (0)
#define SOC_MDC_IHINC_IHINC_END (21)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ivinc : 22;
        unsigned int reserved : 10;
    } reg;
} soc_mdc_ivinc_union;
#endif
#define SOC_MDC_IVINC_IVINC_START (0)
#define SOC_MDC_IVINC_IVINC_END (21)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int uv_voffset : 6;
        unsigned int reserved_0 : 10;
        unsigned int uv_hoffset : 6;
        unsigned int reserved_1 : 10;
    } reg;
} soc_mdc_offset_union;
#endif
#define SOC_MDC_OFFSET_UV_VOFFSET_START (0)
#define SOC_MDC_OFFSET_UV_VOFFSET_END (5)
#define SOC_MDC_OFFSET_UV_HOFFSET_START (16)
#define SOC_MDC_OFFSET_UV_HOFFSET_END (21)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bypass : 1;
        unsigned int sharpenen : 1;
        unsigned int shootdetecten : 1;
        unsigned int skinctrlen : 1;
        unsigned int textureanalysisen : 1;
        unsigned int diintplen : 1;
        unsigned int nearesten : 1;
        unsigned int prescaleren : 1;
        unsigned int nointplen : 1;
        unsigned int dbg_en : 3;
        unsigned int lowpower_en : 1;
        unsigned int reserved : 19;
    } reg;
} soc_mdc_mode_union;
#endif
#define SOC_MDC_MODE_BYPASS_START (0)
#define SOC_MDC_MODE_BYPASS_END (0)
#define SOC_MDC_MODE_SHARPENEN_START (1)
#define SOC_MDC_MODE_SHARPENEN_END (1)
#define SOC_MDC_MODE_SHOOTDETECTEN_START (2)
#define SOC_MDC_MODE_SHOOTDETECTEN_END (2)
#define SOC_MDC_MODE_SKINCTRLEN_START (3)
#define SOC_MDC_MODE_SKINCTRLEN_END (3)
#define SOC_MDC_MODE_TEXTUREANALYSISEN_START (4)
#define SOC_MDC_MODE_TEXTUREANALYSISEN_END (4)
#define SOC_MDC_MODE_DIINTPLEN_START (5)
#define SOC_MDC_MODE_DIINTPLEN_END (5)
#define SOC_MDC_MODE_NEARESTEN_START (6)
#define SOC_MDC_MODE_NEARESTEN_END (6)
#define SOC_MDC_MODE_PRESCALEREN_START (7)
#define SOC_MDC_MODE_PRESCALEREN_END (7)
#define SOC_MDC_MODE_NOINTPLEN_START (8)
#define SOC_MDC_MODE_NOINTPLEN_END (8)
#define SOC_MDC_MODE_DBG_EN_START (9)
#define SOC_MDC_MODE_DBG_EN_END (11)
#define SOC_MDC_MODE_LOWPOWER_EN_START (12)
#define SOC_MDC_MODE_LOWPOWER_EN_END (12)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ythresl : 10;
        unsigned int ythresh : 10;
        unsigned int yexpectvalue : 10;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_skin_thres_y_union;
#endif
#define SOC_MDC_SKIN_THRES_Y_YTHRESL_START (0)
#define SOC_MDC_SKIN_THRES_Y_YTHRESL_END (9)
#define SOC_MDC_SKIN_THRES_Y_YTHRESH_START (10)
#define SOC_MDC_SKIN_THRES_Y_YTHRESH_END (19)
#define SOC_MDC_SKIN_THRES_Y_YEXPECTVALUE_START (20)
#define SOC_MDC_SKIN_THRES_Y_YEXPECTVALUE_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int uthresl : 10;
        unsigned int uthresh : 10;
        unsigned int uexpectvalue : 10;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_skin_thres_u_union;
#endif
#define SOC_MDC_SKIN_THRES_U_UTHRESL_START (0)
#define SOC_MDC_SKIN_THRES_U_UTHRESL_END (9)
#define SOC_MDC_SKIN_THRES_U_UTHRESH_START (10)
#define SOC_MDC_SKIN_THRES_U_UTHRESH_END (19)
#define SOC_MDC_SKIN_THRES_U_UEXPECTVALUE_START (20)
#define SOC_MDC_SKIN_THRES_U_UEXPECTVALUE_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int vthresl : 10;
        unsigned int vthresh : 10;
        unsigned int vexpectvalue : 10;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_skin_thres_v_union;
#endif
#define SOC_MDC_SKIN_THRES_V_VTHRESL_START (0)
#define SOC_MDC_SKIN_THRES_V_VTHRESL_END (9)
#define SOC_MDC_SKIN_THRES_V_VTHRESH_START (10)
#define SOC_MDC_SKIN_THRES_V_VTHRESH_END (19)
#define SOC_MDC_SKIN_THRES_V_VEXPECTVALUE_START (20)
#define SOC_MDC_SKIN_THRES_V_VEXPECTVALUE_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int yslop : 13;
        unsigned int clipdata : 10;
        unsigned int reserved : 9;
    } reg;
} soc_mdc_skin_cfg0_union;
#endif
#define SOC_MDC_SKIN_CFG0_YSLOP_START (0)
#define SOC_MDC_SKIN_CFG0_YSLOP_END (12)
#define SOC_MDC_SKIN_CFG0_CLIPDATA_START (13)
#define SOC_MDC_SKIN_CFG0_CLIPDATA_END (22)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int uslop : 13;
        unsigned int reserved : 19;
    } reg;
} soc_mdc_skin_cfg1_union;
#endif
#define SOC_MDC_SKIN_CFG1_USLOP_START (0)
#define SOC_MDC_SKIN_CFG1_USLOP_END (12)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int vslop : 13;
        unsigned int reserved : 19;
    } reg;
} soc_mdc_skin_cfg2_union;
#endif
#define SOC_MDC_SKIN_CFG2_VSLOP_START (0)
#define SOC_MDC_SKIN_CFG2_VSLOP_END (12)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int shootslop1 : 15;
        unsigned int reserved_0 : 1;
        unsigned int shootgradalpha : 6;
        unsigned int reserved_1 : 10;
    } reg;
} soc_mdc_shoot_cfg1_union;
#endif
#define SOC_MDC_SHOOT_CFG1_SHOOTSLOP1_START (0)
#define SOC_MDC_SHOOT_CFG1_SHOOTSLOP1_END (14)
#define SOC_MDC_SHOOT_CFG1_SHOOTGRADALPHA_START (16)
#define SOC_MDC_SHOOT_CFG1_SHOOTGRADALPHA_END (21)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int shootgradsubthrl : 11;
        unsigned int reserved_0 : 5;
        unsigned int shootgradsubthrh : 11;
        unsigned int reserved_1 : 5;
    } reg;
} soc_mdc_shoot_cfg2_union;
#endif
#define SOC_MDC_SHOOT_CFG2_SHOOTGRADSUBTHRL_START (0)
#define SOC_MDC_SHOOT_CFG2_SHOOTGRADSUBTHRL_END (10)
#define SOC_MDC_SHOOT_CFG2_SHOOTGRADSUBTHRH_START (16)
#define SOC_MDC_SHOOT_CFG2_SHOOTGRADSUBTHRH_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int shootshpratio : 6;
        unsigned int reserved : 26;
    } reg;
} soc_mdc_shoot_cfg3_union;
#endif
#define SOC_MDC_SHOOT_CFG3_SHOOTSHPRATIO_START (0)
#define SOC_MDC_SHOOT_CFG3_SHOOTSHPRATIO_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int shptocuthigh0 : 10;
        unsigned int reserved_0 : 6;
        unsigned int shptocuthigh1 : 10;
        unsigned int reserved_1 : 6;
    } reg;
} soc_mdc_sharp_cfg3_union;
#endif
#define SOC_MDC_SHARP_CFG3_SHPTOCUTHIGH0_START (0)
#define SOC_MDC_SHARP_CFG3_SHPTOCUTHIGH0_END (9)
#define SOC_MDC_SHARP_CFG3_SHPTOCUTHIGH1_START (16)
#define SOC_MDC_SHARP_CFG3_SHPTOCUTHIGH1_END (25)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sharpshootctrll : 10;
        unsigned int reserved_0 : 6;
        unsigned int sharpshootctrlh : 10;
        unsigned int reserved_1 : 6;
    } reg;
} soc_mdc_sharp_cfg4_union;
#endif
#define SOC_MDC_SHARP_CFG4_SHARPSHOOTCTRLL_START (0)
#define SOC_MDC_SHARP_CFG4_SHARPSHOOTCTRLL_END (9)
#define SOC_MDC_SHARP_CFG4_SHARPSHOOTCTRLH_START (16)
#define SOC_MDC_SHARP_CFG4_SHARPSHOOTCTRLH_END (25)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} soc_mdc_sharp_cfg5_union;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int blendshootctrl : 10;
        unsigned int reserved_0 : 6;
        unsigned int sharpcoring : 10;
        unsigned int reserved_1 : 6;
    } reg;
} soc_mdc_sharp_cfg6_union;
#endif
#define SOC_MDC_SHARP_CFG6_BLENDSHOOTCTRL_START (0)
#define SOC_MDC_SHARP_CFG6_BLENDSHOOTCTRL_END (9)
#define SOC_MDC_SHARP_CFG6_SHARPCORING_START (16)
#define SOC_MDC_SHARP_CFG6_SHARPCORING_END (25)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int skinthresh : 5;
        unsigned int reserved_0 : 3;
        unsigned int skinthresl : 5;
        unsigned int reserved_1 : 19;
    } reg;
} soc_mdc_sharp_cfg7_union;
#endif
#define SOC_MDC_SHARP_CFG7_SKINTHRESH_START (0)
#define SOC_MDC_SHARP_CFG7_SKINTHRESH_END (4)
#define SOC_MDC_SHARP_CFG7_SKINTHRESL_START (8)
#define SOC_MDC_SHARP_CFG7_SKINTHRESL_END (12)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int linearshratiol : 8;
        unsigned int reserved_0 : 8;
        unsigned int linearshratioh : 8;
        unsigned int reserved_1 : 8;
    } reg;
} soc_mdc_sharp_cfg8_union;
#endif
#define SOC_MDC_SHARP_CFG8_LINEARSHRATIOL_START (0)
#define SOC_MDC_SHARP_CFG8_LINEARSHRATIOL_END (7)
#define SOC_MDC_SHARP_CFG8_LINEARSHRATIOH_START (16)
#define SOC_MDC_SHARP_CFG8_LINEARSHRATIOH_END (23)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sharplevel_mf : 4;
        unsigned int reserved_0 : 12;
        unsigned int sharplevel_hf : 4;
        unsigned int reserved_1 : 12;
    } reg;
} soc_mdc_sharplevel_union;
#endif
#define SOC_MDC_SHARPLEVEL_SHARPLEVEL_MF_START (0)
#define SOC_MDC_SHARPLEVEL_SHARPLEVEL_MF_END (3)
#define SOC_MDC_SHARPLEVEL_SHARPLEVEL_HF_START (16)
#define SOC_MDC_SHARPLEVEL_SHARPLEVEL_HF_END (19)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int shpgainlow_mf : 12;
        unsigned int reserved_0 : 4;
        unsigned int shpgainlow_hf : 12;
        unsigned int reserved_1 : 4;
    } reg;
} soc_mdc_shpgain_low_union;
#endif
#define SOC_MDC_SHPGAIN_LOW_SHPGAINLOW_MF_START (0)
#define SOC_MDC_SHPGAIN_LOW_SHPGAINLOW_MF_END (11)
#define SOC_MDC_SHPGAIN_LOW_SHPGAINLOW_HF_START (16)
#define SOC_MDC_SHPGAIN_LOW_SHPGAINLOW_HF_END (27)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int shpgainmid_mf : 12;
        unsigned int reserved_0 : 4;
        unsigned int shpgainmid_hf : 12;
        unsigned int reserved_1 : 4;
    } reg;
} soc_mdc_shpgain_mid_union;
#endif
#define SOC_MDC_SHPGAIN_MID_SHPGAINMID_MF_START (0)
#define SOC_MDC_SHPGAIN_MID_SHPGAINMID_MF_END (11)
#define SOC_MDC_SHPGAIN_MID_SHPGAINMID_HF_START (16)
#define SOC_MDC_SHPGAIN_MID_SHPGAINMID_HF_END (27)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int shpgainhigh_mf : 12;
        unsigned int reserved_0 : 4;
        unsigned int shpgainhigh_hf : 12;
        unsigned int reserved_1 : 4;
    } reg;
} soc_mdc_shpgain_high_union;
#endif
#define SOC_MDC_SHPGAIN_HIGH_SHPGAINHIGH_MF_START (0)
#define SOC_MDC_SHPGAIN_HIGH_SHPGAINHIGH_MF_END (11)
#define SOC_MDC_SHPGAIN_HIGH_SHPGAINHIGH_HF_START (16)
#define SOC_MDC_SHPGAIN_HIGH_SHPGAINHIGH_HF_END (27)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gainctrlsloph_mf : 22;
        unsigned int reserved : 10;
    } reg;
} soc_mdc_gainctrlsloph_mf_union;
#endif
#define SOC_MDC_GAINCTRLSLOPH_MF_GAINCTRLSLOPH_MF_START (0)
#define SOC_MDC_GAINCTRLSLOPH_MF_GAINCTRLSLOPH_MF_END (21)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gainctrlslopl_mf : 22;
        unsigned int reserved : 10;
    } reg;
} soc_mdc_gainctrlslopl_mf_union;
#endif
#define SOC_MDC_GAINCTRLSLOPL_MF_GAINCTRLSLOPL_MF_START (0)
#define SOC_MDC_GAINCTRLSLOPL_MF_GAINCTRLSLOPL_MF_END (21)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gainctrlsloph_hf : 22;
        unsigned int reserved : 10;
    } reg;
} soc_mdc_gainctrlsloph_hf_union;
#endif
#define SOC_MDC_GAINCTRLSLOPH_HF_GAINCTRLSLOPH_HF_START (0)
#define SOC_MDC_GAINCTRLSLOPH_HF_GAINCTRLSLOPH_HF_END (21)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gainctrlslopl_hf : 22;
        unsigned int reserved : 10;
    } reg;
} soc_mdc_gainctrlslopl_hf_union;
#endif
#define SOC_MDC_GAINCTRLSLOPL_HF_GAINCTRLSLOPL_HF_START (0)
#define SOC_MDC_GAINCTRLSLOPL_HF_GAINCTRLSLOPL_HF_END (21)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int mf_lmt : 10;
        unsigned int reserved : 22;
    } reg;
} soc_mdc_mf_lmt_union;
#endif
#define SOC_MDC_MF_LMT_MF_LMT_START (0)
#define SOC_MDC_MF_LMT_MF_LMT_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gain_mf_l : 12;
        unsigned int reserved_0: 4;
        unsigned int gain_mf_h : 12;
        unsigned int reserved_1: 4;
    } reg;
} soc_mdc_gain_mf_union;
#endif
#define SOC_MDC_GAIN_MF_GAIN_MF_L_START (0)
#define SOC_MDC_GAIN_MF_GAIN_MF_L_END (11)
#define SOC_MDC_GAIN_MF_GAIN_MF_H_START (16)
#define SOC_MDC_GAIN_MF_GAIN_MF_H_END (27)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int mf_b : 23;
        unsigned int reserved : 9;
    } reg;
} soc_mdc_mf_b_union;
#endif
#define SOC_MDC_MF_B_MF_B_START (0)
#define SOC_MDC_MF_B_MF_B_END (22)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hf_lmt : 10;
        unsigned int reserved : 22;
    } reg;
} soc_mdc_hf_lmt_union;
#endif
#define SOC_MDC_HF_LMT_HF_LMT_START (0)
#define SOC_MDC_HF_LMT_HF_LMT_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gain_hf_l : 12;
        unsigned int reserved_0: 4;
        unsigned int gain_hf_h : 12;
        unsigned int reserved_1: 4;
    } reg;
} soc_mdc_gain_hf_union;
#endif
#define SOC_MDC_GAIN_HF_GAIN_HF_L_START (0)
#define SOC_MDC_GAIN_HF_GAIN_HF_L_END (11)
#define SOC_MDC_GAIN_HF_GAIN_HF_H_START (16)
#define SOC_MDC_GAIN_HF_GAIN_HF_H_END (27)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hf_b : 23;
        unsigned int reserved : 9;
    } reg;
} soc_mdc_hf_b_union;
#endif
#define SOC_MDC_HF_B_HF_B_START (0)
#define SOC_MDC_HF_B_HF_B_END (22)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int lf_ctrl_l : 8;
        unsigned int reserved_0: 8;
        unsigned int lf_ctrl_h : 8;
        unsigned int reserved_1: 8;
    } reg;
} soc_mdc_lf_ctrl_union;
#endif
#define SOC_MDC_LF_CTRL_LF_CTRL_L_START (0)
#define SOC_MDC_LF_CTRL_LF_CTRL_L_END (7)
#define SOC_MDC_LF_CTRL_LF_CTRL_H_START (16)
#define SOC_MDC_LF_CTRL_LF_CTRL_H_END (23)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int lf_var_l : 10;
        unsigned int reserved_0: 6;
        unsigned int lf_var_h : 10;
        unsigned int reserved_1: 6;
    } reg;
} soc_mdc_lf_var_union;
#endif
#define SOC_MDC_LF_VAR_LF_VAR_L_START (0)
#define SOC_MDC_LF_VAR_LF_VAR_L_END (9)
#define SOC_MDC_LF_VAR_LF_VAR_H_START (16)
#define SOC_MDC_LF_VAR_LF_VAR_H_END (25)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int lf_ctrl_slop : 16;
        unsigned int reserved : 16;
    } reg;
} soc_mdc_lf_ctrl_slop_union;
#endif
#define SOC_MDC_LF_CTRL_SLOP_LF_CTRL_SLOP_START (0)
#define SOC_MDC_LF_CTRL_SLOP_LF_CTRL_SLOP_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hf_select : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_hf_select_union;
#endif
#define SOC_MDC_HF_SELECT_HF_SELECT_START (0)
#define SOC_MDC_HF_SELECT_HF_SELECT_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int shpgainvarhigh0 : 10;
        unsigned int reserved_0 : 6;
        unsigned int shpgainvarhigh1 : 10;
        unsigned int reserved_1 : 6;
    } reg;
} soc_mdc_sharp_cfg2_h_union;
#endif
#define SOC_MDC_SHARP_CFG2_H_SHPGAINVARHIGH0_START (0)
#define SOC_MDC_SHARP_CFG2_H_SHPGAINVARHIGH0_END (9)
#define SOC_MDC_SHARP_CFG2_H_SHPGAINVARHIGH1_START (16)
#define SOC_MDC_SHARP_CFG2_H_SHPGAINVARHIGH1_END (25)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int shpgainvarlow0 : 10;
        unsigned int reserved_0 : 6;
        unsigned int shpgainvarlow1 : 10;
        unsigned int reserved_1 : 6;
    } reg;
} soc_mdc_sharp_cfg2_l_union;
#endif
#define SOC_MDC_SHARP_CFG2_L_SHPGAINVARLOW0_START (0)
#define SOC_MDC_SHARP_CFG2_L_SHPGAINVARLOW0_END (9)
#define SOC_MDC_SHARP_CFG2_L_SHPGAINVARLOW1_START (16)
#define SOC_MDC_SHARP_CFG2_L_SHPGAINVARLOW1_END (25)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int difflow : 8;
        unsigned int reserved_0: 8;
        unsigned int diffhigh : 8;
        unsigned int reserved_1: 8;
    } reg;
} soc_mdc_texturw_analysts_union;
#endif
#define SOC_MDC_TEXTURW_ANALYSTS_DIFFLOW_START (0)
#define SOC_MDC_TEXTURW_ANALYSTS_DIFFLOW_END (7)
#define SOC_MDC_TEXTURW_ANALYSTS_DIFFHIGH_START (16)
#define SOC_MDC_TEXTURW_ANALYSTS_DIFFHIGH_END (23)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int intplshootctrl : 10;
        unsigned int reserved : 22;
    } reg;
} soc_mdc_intplshootctrl_union;
#endif
#define SOC_MDC_INTPLSHOOTCTRL_INTPLSHOOTCTRL_START (0)
#define SOC_MDC_INTPLSHOOTCTRL_INTPLSHOOTCTRL_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int arsr2p_debug_h0 : 32;
    } reg;
} soc_mdc_arsr2p_debug0_union;
#endif
#define SOC_MDC_ARSR2P_DEBUG0_ARSR2P_DEBUG_H0_START (0)
#define SOC_MDC_ARSR2P_DEBUG0_ARSR2P_DEBUG_H0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int arsr2p_debug_h1 : 32;
    } reg;
} soc_mdc_arsr2p_debug1_union;
#endif
#define SOC_MDC_ARSR2P_DEBUG1_ARSR2P_DEBUG_H1_START (0)
#define SOC_MDC_ARSR2P_DEBUG1_ARSR2P_DEBUG_H1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int arsr2p_debug_v0 : 32;
    } reg;
} soc_mdc_arsr2p_debug2_union;
#endif
#define SOC_MDC_ARSR2P_DEBUG2_ARSR2P_DEBUG_V0_START (0)
#define SOC_MDC_ARSR2P_DEBUG2_ARSR2P_DEBUG_V0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int arsr2p_debug_v1 : 32;
    } reg;
} soc_mdc_arsr2p_debug3_union;
#endif
#define SOC_MDC_ARSR2P_DEBUG3_ARSR2P_DEBUG_V1_START (0)
#define SOC_MDC_ARSR2P_DEBUG3_ARSR2P_DEBUG_V1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int arsr_lb_mem_ctrl : 4;
        unsigned int reserved : 28;
    } reg;
} soc_mdc_arsr2p_lb_mem_ctrl_union;
#endif
#define SOC_MDC_ARSR2P_LB_MEM_CTRL_ARSR_LB_MEM_CTRL_START (0)
#define SOC_MDC_ARSR2P_LB_MEM_CTRL_ARSR_LB_MEM_CTRL_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pcsc_idc0 : 11;
        unsigned int reserved_0: 5;
        unsigned int pcsc_idc1 : 11;
        unsigned int reserved_1: 5;
    } reg;
} soc_mdc_pcsc_idc0_union;
#endif
#define SOC_MDC_PCSC_IDC0_PCSC_IDC0_START (0)
#define SOC_MDC_PCSC_IDC0_PCSC_IDC0_END (10)
#define SOC_MDC_PCSC_IDC0_PCSC_IDC1_START (16)
#define SOC_MDC_PCSC_IDC0_PCSC_IDC1_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pcsc_idc2 : 11;
        unsigned int reserved : 21;
    } reg;
} soc_mdc_pcsc_idc2_union;
#endif
#define SOC_MDC_PCSC_IDC2_PCSC_IDC2_START (0)
#define SOC_MDC_PCSC_IDC2_PCSC_IDC2_END (10)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pcsc_odc0 : 11;
        unsigned int reserved_0: 5;
        unsigned int pcsc_odc1 : 11;
        unsigned int reserved_1: 5;
    } reg;
} soc_mdc_pcsc_odc0_union;
#endif
#define SOC_MDC_PCSC_ODC0_PCSC_ODC0_START (0)
#define SOC_MDC_PCSC_ODC0_PCSC_ODC0_END (10)
#define SOC_MDC_PCSC_ODC0_PCSC_ODC1_START (16)
#define SOC_MDC_PCSC_ODC0_PCSC_ODC1_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pcsc_odc2 : 11;
        unsigned int reserved : 21;
    } reg;
} soc_mdc_pcsc_odc2_union;
#endif
#define SOC_MDC_PCSC_ODC2_PCSC_ODC2_START (0)
#define SOC_MDC_PCSC_ODC2_PCSC_ODC2_END (10)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pcsc_p00 : 17;
        unsigned int reserved : 15;
    } reg;
} soc_mdc_pcsc_p00_union;
#endif
#define SOC_MDC_PCSC_P00_PCSC_P00_START (0)
#define SOC_MDC_PCSC_P00_PCSC_P00_END (16)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pcsc_p01 : 17;
        unsigned int reserved : 15;
    } reg;
} soc_mdc_pcsc_p01_union;
#endif
#define SOC_MDC_PCSC_P01_PCSC_P01_START (0)
#define SOC_MDC_PCSC_P01_PCSC_P01_END (16)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pcsc_p02 : 17;
        unsigned int reserved : 15;
    } reg;
} soc_mdc_pcsc_p02_union;
#endif
#define SOC_MDC_PCSC_P02_PCSC_P02_START (0)
#define SOC_MDC_PCSC_P02_PCSC_P02_END (16)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pcsc_p10 : 17;
        unsigned int reserved : 15;
    } reg;
} soc_mdc_pcsc_p10_union;
#endif
#define SOC_MDC_PCSC_P10_PCSC_P10_START (0)
#define SOC_MDC_PCSC_P10_PCSC_P10_END (16)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pcsc_p11 : 17;
        unsigned int reserved : 15;
    } reg;
} soc_mdc_pcsc_p11_union;
#endif
#define SOC_MDC_PCSC_P11_PCSC_P11_START (0)
#define SOC_MDC_PCSC_P11_PCSC_P11_END (16)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pcsc_p12 : 17;
        unsigned int reserved : 15;
    } reg;
} soc_mdc_pcsc_p12_union;
#endif
#define SOC_MDC_PCSC_P12_PCSC_P12_START (0)
#define SOC_MDC_PCSC_P12_PCSC_P12_END (16)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pcsc_p20 : 17;
        unsigned int reserved : 15;
    } reg;
} soc_mdc_pcsc_p20_union;
#endif
#define SOC_MDC_PCSC_P20_PCSC_P20_START (0)
#define SOC_MDC_PCSC_P20_PCSC_P20_END (16)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pcsc_p21 : 17;
        unsigned int reserved : 15;
    } reg;
} soc_mdc_pcsc_p21_union;
#endif
#define SOC_MDC_PCSC_P21_PCSC_P21_START (0)
#define SOC_MDC_PCSC_P21_PCSC_P21_END (16)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pcsc_p22 : 17;
        unsigned int reserved : 15;
    } reg;
} soc_mdc_pcsc_p22_union;
#endif
#define SOC_MDC_PCSC_P22_PCSC_P22_START (0)
#define SOC_MDC_PCSC_P22_PCSC_P22_END (16)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pcsc_module_en : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_pcsc_icg_module_union;
#endif
#define SOC_MDC_PCSC_ICG_MODULE_PCSC_MODULE_EN_START (0)
#define SOC_MDC_PCSC_ICG_MODULE_PCSC_MODULE_EN_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pcsc_mprec : 3;
        unsigned int reserved : 29;
    } reg;
} soc_mdc_pcsc_mprec_union;
#endif
#define SOC_MDC_PCSC_MPREC_PCSC_MPREC_START (0)
#define SOC_MDC_PCSC_MPREC_PCSC_MPREC_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int post_clip_size_vrt : 13;
        unsigned int reserved_0 : 3;
        unsigned int post_clip_size_hrz : 13;
        unsigned int reserved_1 : 3;
    } reg;
} soc_mdc_post_clip_disp_size_union;
#endif
#define SOC_MDC_POST_CLIP_DISP_SIZE_POST_CLIP_SIZE_VRT_START (0)
#define SOC_MDC_POST_CLIP_DISP_SIZE_POST_CLIP_SIZE_VRT_END (12)
#define SOC_MDC_POST_CLIP_DISP_SIZE_POST_CLIP_SIZE_HRZ_START (16)
#define SOC_MDC_POST_CLIP_DISP_SIZE_POST_CLIP_SIZE_HRZ_END (28)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int post_clip_right : 6;
        unsigned int reserved_0 : 10;
        unsigned int post_clip_left : 6;
        unsigned int reserved_1 : 10;
    } reg;
} soc_mdc_post_clip_ctl_hrz_union;
#endif
#define SOC_MDC_POST_CLIP_CTL_HRZ_POST_CLIP_RIGHT_START (0)
#define SOC_MDC_POST_CLIP_CTL_HRZ_POST_CLIP_RIGHT_END (5)
#define SOC_MDC_POST_CLIP_CTL_HRZ_POST_CLIP_LEFT_START (16)
#define SOC_MDC_POST_CLIP_CTL_HRZ_POST_CLIP_LEFT_END (21)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int post_clip_bot : 6;
        unsigned int reserved_0 : 10;
        unsigned int post_clip_top : 6;
        unsigned int reserved_1 : 10;
    } reg;
} soc_mdc_post_clip_ctl_vrz_union;
#endif
#define SOC_MDC_POST_CLIP_CTL_VRZ_POST_CLIP_BOT_START (0)
#define SOC_MDC_POST_CLIP_CTL_VRZ_POST_CLIP_BOT_END (5)
#define SOC_MDC_POST_CLIP_CTL_VRZ_POST_CLIP_TOP_START (16)
#define SOC_MDC_POST_CLIP_CTL_VRZ_POST_CLIP_TOP_END (21)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int post_clip_enable : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_post_clip_en_union;
#endif
#define SOC_MDC_POST_CLIP_EN_POST_CLIP_ENABLE_START (0)
#define SOC_MDC_POST_CLIP_EN_POST_CLIP_ENABLE_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int csc_idc0 : 11;
        unsigned int reserved_0: 5;
        unsigned int csc_idc1 : 11;
        unsigned int reserved_1: 5;
    } reg;
} soc_mdc_csc_idc0_union;
#endif
#define SOC_MDC_CSC_IDC0_CSC_IDC0_START (0)
#define SOC_MDC_CSC_IDC0_CSC_IDC0_END (10)
#define SOC_MDC_CSC_IDC0_CSC_IDC1_START (16)
#define SOC_MDC_CSC_IDC0_CSC_IDC1_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int csc_idc2 : 11;
        unsigned int reserved : 21;
    } reg;
} soc_mdc_csc_idc2_union;
#endif
#define SOC_MDC_CSC_IDC2_CSC_IDC2_START (0)
#define SOC_MDC_CSC_IDC2_CSC_IDC2_END (10)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int csc_odc0 : 11;
        unsigned int reserved_0: 5;
        unsigned int csc_odc1 : 11;
        unsigned int reserved_1: 5;
    } reg;
} soc_mdc_csc_odc0_union;
#endif
#define SOC_MDC_CSC_ODC0_CSC_ODC0_START (0)
#define SOC_MDC_CSC_ODC0_CSC_ODC0_END (10)
#define SOC_MDC_CSC_ODC0_CSC_ODC1_START (16)
#define SOC_MDC_CSC_ODC0_CSC_ODC1_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int csc_odc2 : 11;
        unsigned int reserved : 21;
    } reg;
} soc_mdc_csc_odc2_union;
#endif
#define SOC_MDC_CSC_ODC2_CSC_ODC2_START (0)
#define SOC_MDC_CSC_ODC2_CSC_ODC2_END (10)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int csc_p00 : 17;
        unsigned int reserved : 15;
    } reg;
} soc_mdc_csc_p00_union;
#endif
#define SOC_MDC_CSC_P00_CSC_P00_START (0)
#define SOC_MDC_CSC_P00_CSC_P00_END (16)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int csc_p01 : 17;
        unsigned int reserved : 15;
    } reg;
} soc_mdc_csc_p01_union;
#endif
#define SOC_MDC_CSC_P01_CSC_P01_START (0)
#define SOC_MDC_CSC_P01_CSC_P01_END (16)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int csc_p02 : 17;
        unsigned int reserved : 15;
    } reg;
} soc_mdc_csc_p02_union;
#endif
#define SOC_MDC_CSC_P02_CSC_P02_START (0)
#define SOC_MDC_CSC_P02_CSC_P02_END (16)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int csc_p10 : 17;
        unsigned int reserved : 15;
    } reg;
} soc_mdc_csc_p10_union;
#endif
#define SOC_MDC_CSC_P10_CSC_P10_START (0)
#define SOC_MDC_CSC_P10_CSC_P10_END (16)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int csc_p11 : 17;
        unsigned int reserved : 15;
    } reg;
} soc_mdc_csc_p11_union;
#endif
#define SOC_MDC_CSC_P11_CSC_P11_START (0)
#define SOC_MDC_CSC_P11_CSC_P11_END (16)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int csc_p12 : 17;
        unsigned int reserved : 15;
    } reg;
} soc_mdc_csc_p12_union;
#endif
#define SOC_MDC_CSC_P12_CSC_P12_START (0)
#define SOC_MDC_CSC_P12_CSC_P12_END (16)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int csc_p20 : 17;
        unsigned int reserved : 15;
    } reg;
} soc_mdc_csc_p20_union;
#endif
#define SOC_MDC_CSC_P20_CSC_P20_START (0)
#define SOC_MDC_CSC_P20_CSC_P20_END (16)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int csc_p21 : 17;
        unsigned int reserved : 15;
    } reg;
} soc_mdc_csc_p21_union;
#endif
#define SOC_MDC_CSC_P21_CSC_P21_START (0)
#define SOC_MDC_CSC_P21_CSC_P21_END (16)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int csc_p22 : 17;
        unsigned int reserved : 15;
    } reg;
} soc_mdc_csc_p22_union;
#endif
#define SOC_MDC_CSC_P22_CSC_P22_START (0)
#define SOC_MDC_CSC_P22_CSC_P22_END (16)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int csc_module_en : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_csc_icg_module_union;
#endif
#define SOC_MDC_CSC_ICG_MODULE_CSC_MODULE_EN_START (0)
#define SOC_MDC_CSC_ICG_MODULE_CSC_MODULE_EN_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int csc_mprec : 3;
        unsigned int reserved : 29;
    } reg;
} soc_mdc_csc_mprec_union;
#endif
#define SOC_MDC_CSC_MPREC_CSC_MPREC_START (0)
#define SOC_MDC_CSC_MPREC_CSC_MPREC_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ch_debug_sel : 4;
        unsigned int reserved : 28;
    } reg;
} soc_mdc_ch_debug_sel_union;
#endif
#define SOC_MDC_CH_DEBUG_SEL_CH_DEBUG_SEL_START (0)
#define SOC_MDC_CH_DEBUG_SEL_CH_DEBUG_SEL_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int vpp_vlpf_hlpf : 1;
        unsigned int vpp_en : 1;
        unsigned int reserved : 30;
    } reg;
} soc_mdc_vpp_ctrl_union;
#endif
#define SOC_MDC_VPP_CTRL_VPP_VLPF_HLPF_START (0)
#define SOC_MDC_VPP_CTRL_VPP_VLPF_HLPF_END (0)
#define SOC_MDC_VPP_CTRL_VPP_EN_START (1)
#define SOC_MDC_VPP_CTRL_VPP_EN_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int vpp_mem_ctrl : 4;
        unsigned int reserved : 28;
    } reg;
} soc_mdc_vpp_mem_ctrl_union;
#endif
#define SOC_MDC_VPP_MEM_CTRL_VPP_MEM_CTRL_START (0)
#define SOC_MDC_VPP_MEM_CTRL_VPP_MEM_CTRL_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_buf_os_cfg : 6;
        unsigned int reserved_0 : 2;
        unsigned int dma_buf_qos_mask : 1;
        unsigned int dma_buf_cont_mask : 1;
        unsigned int dma_buf_l_e_mask : 1;
        unsigned int reserved_1 : 21;
    } reg;
} soc_mdc_dma_buf_ctrl_union;
#endif
#define SOC_MDC_DMA_BUF_CTRL_DMA_BUF_OS_CFG_START (0)
#define SOC_MDC_DMA_BUF_CTRL_DMA_BUF_OS_CFG_END (5)
#define SOC_MDC_DMA_BUF_CTRL_DMA_BUF_QOS_MASK_START (8)
#define SOC_MDC_DMA_BUF_CTRL_DMA_BUF_QOS_MASK_END (8)
#define SOC_MDC_DMA_BUF_CTRL_DMA_BUF_CONT_MASK_START (9)
#define SOC_MDC_DMA_BUF_CTRL_DMA_BUF_CONT_MASK_END (9)
#define SOC_MDC_DMA_BUF_CTRL_DMA_BUF_L_E_MASK_START (10)
#define SOC_MDC_DMA_BUF_CTRL_DMA_BUF_L_E_MASK_END (10)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_buf_cpu_ctl : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_dma_buf_cpu_ctl_union;
#endif
#define SOC_MDC_DMA_BUF_CPU_CTL_DMA_BUF_CPU_CTL_START (0)
#define SOC_MDC_DMA_BUF_CPU_CTL_DMA_BUF_CPU_CTL_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_buf_cpu_start : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_dma_buf_cpu_start_union;
#endif
#define SOC_MDC_DMA_BUF_CPU_START_DMA_BUF_CPU_START_START (0)
#define SOC_MDC_DMA_BUF_CPU_START_DMA_BUF_CPU_START_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_buf_cpu_addr : 9;
        unsigned int reserved : 22;
        unsigned int dma_buf_cpu_rw : 1;
    } reg;
} soc_mdc_dma_buf_cpu_addr_union;
#endif
#define SOC_MDC_DMA_BUF_CPU_ADDR_DMA_BUF_CPU_ADDR_START (0)
#define SOC_MDC_DMA_BUF_CPU_ADDR_DMA_BUF_CPU_ADDR_END (8)
#define SOC_MDC_DMA_BUF_CPU_ADDR_DMA_BUF_CPU_RW_START (31)
#define SOC_MDC_DMA_BUF_CPU_ADDR_DMA_BUF_CPU_RW_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_buf_cpu_rdata0 : 32;
    } reg;
} soc_mdc_dma_buf_cpu_rdata0_union;
#endif
#define SOC_MDC_DMA_BUF_CPU_RDATA0_DMA_BUF_CPU_RDATA0_START (0)
#define SOC_MDC_DMA_BUF_CPU_RDATA0_DMA_BUF_CPU_RDATA0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_buf_cpu_rdata1 : 32;
    } reg;
} soc_mdc_dma_buf_cpu_rdata1_union;
#endif
#define SOC_MDC_DMA_BUF_CPU_RDATA1_DMA_BUF_CPU_RDATA1_START (0)
#define SOC_MDC_DMA_BUF_CPU_RDATA1_DMA_BUF_CPU_RDATA1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_buf_cpu_rdata2 : 32;
    } reg;
} soc_mdc_dma_buf_cpu_rdata2_union;
#endif
#define SOC_MDC_DMA_BUF_CPU_RDATA2_DMA_BUF_CPU_RDATA2_START (0)
#define SOC_MDC_DMA_BUF_CPU_RDATA2_DMA_BUF_CPU_RDATA2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_buf_cpu_rdata3 : 32;
    } reg;
} soc_mdc_dma_buf_cpu_rdata3_union;
#endif
#define SOC_MDC_DMA_BUF_CPU_RDATA3_DMA_BUF_CPU_RDATA3_START (0)
#define SOC_MDC_DMA_BUF_CPU_RDATA3_DMA_BUF_CPU_RDATA3_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_buf_cpu_wdata0 : 32;
    } reg;
} soc_mdc_dma_buf_cpu_wdata0_union;
#endif
#define SOC_MDC_DMA_BUF_CPU_WDATA0_DMA_BUF_CPU_WDATA0_START (0)
#define SOC_MDC_DMA_BUF_CPU_WDATA0_DMA_BUF_CPU_WDATA0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_buf_cpu_wdata1 : 32;
    } reg;
} soc_mdc_dma_buf_cpu_wdata1_union;
#endif
#define SOC_MDC_DMA_BUF_CPU_WDATA1_DMA_BUF_CPU_WDATA1_START (0)
#define SOC_MDC_DMA_BUF_CPU_WDATA1_DMA_BUF_CPU_WDATA1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_buf_cpu_wdata2 : 32;
    } reg;
} soc_mdc_dma_buf_cpu_wdata2_union;
#endif
#define SOC_MDC_DMA_BUF_CPU_WDATA2_DMA_BUF_CPU_WDATA2_START (0)
#define SOC_MDC_DMA_BUF_CPU_WDATA2_DMA_BUF_CPU_WDATA2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_buf_cpu_wdata3 : 32;
    } reg;
} soc_mdc_dma_buf_cpu_wdata3_union;
#endif
#define SOC_MDC_DMA_BUF_CPU_WDATA3_DMA_BUF_CPU_WDATA3_START (0)
#define SOC_MDC_DMA_BUF_CPU_WDATA3_DMA_BUF_CPU_WDATA3_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_req_end : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_dma_req_end_union;
#endif
#define SOC_MDC_DMA_REQ_END_DMA_REQ_END_START (0)
#define SOC_MDC_DMA_REQ_END_DMA_REQ_END_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_buf_dbgcfg : 32;
    } reg;
} soc_mdc_dma_buf_dbgcfg_union;
#endif
#define SOC_MDC_DMA_BUF_DBGCFG_DMA_BUF_DBGCFG_START (0)
#define SOC_MDC_DMA_BUF_DBGCFG_DMA_BUF_DBGCFG_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_buf_dbg0 : 32;
    } reg;
} soc_mdc_dma_buf_dbg0_union;
#endif
#define SOC_MDC_DMA_BUF_DBG0_DMA_BUF_DBG0_START (0)
#define SOC_MDC_DMA_BUF_DBG0_DMA_BUF_DBG0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_buf_dbg1 : 32;
    } reg;
} soc_mdc_dma_buf_dbg1_union;
#endif
#define SOC_MDC_DMA_BUF_DBG1_DMA_BUF_DBG1_START (0)
#define SOC_MDC_DMA_BUF_DBG1_DMA_BUF_DBG1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_buf_dbg2 : 32;
    } reg;
} soc_mdc_dma_buf_dbg2_union;
#endif
#define SOC_MDC_DMA_BUF_DBG2_DMA_BUF_DBG2_START (0)
#define SOC_MDC_DMA_BUF_DBG2_DMA_BUF_DBG2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_buf_dbg3 : 32;
    } reg;
} soc_mdc_dma_buf_dbg3_union;
#endif
#define SOC_MDC_DMA_BUF_DBG3_DMA_BUF_DBG3_START (0)
#define SOC_MDC_DMA_BUF_DBG3_DMA_BUF_DBG3_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_buf_dbg4 : 32;
    } reg;
} soc_mdc_dma_buf_dbg4_union;
#endif
#define SOC_MDC_DMA_BUF_DBG4_DMA_BUF_DBG4_START (0)
#define SOC_MDC_DMA_BUF_DBG4_DMA_BUF_DBG4_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_buf_dbg5 : 32;
    } reg;
} soc_mdc_dma_buf_dbg5_union;
#endif
#define SOC_MDC_DMA_BUF_DBG5_DMA_BUF_DBG5_START (0)
#define SOC_MDC_DMA_BUF_DBG5_DMA_BUF_DBG5_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_buf_img_width : 13;
        unsigned int reserved_0 : 3;
        unsigned int dma_buf_img_height : 13;
        unsigned int reserved_1 : 3;
    } reg;
} soc_mdc_dma_buf_size_union;
#endif
#define SOC_MDC_DMA_BUF_SIZE_DMA_BUF_IMG_WIDTH_START (0)
#define SOC_MDC_DMA_BUF_SIZE_DMA_BUF_IMG_WIDTH_END (12)
#define SOC_MDC_DMA_BUF_SIZE_DMA_BUF_IMG_HEIGHT_START (16)
#define SOC_MDC_DMA_BUF_SIZE_DMA_BUF_IMG_HEIGHT_END (28)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_buf_mem_ctrl : 4;
        unsigned int reserved : 28;
    } reg;
} soc_mdc_dma_buf_mem_ctrl_union;
#endif
#define SOC_MDC_DMA_BUF_MEM_CTRL_DMA_BUF_MEM_CTRL_START (0)
#define SOC_MDC_DMA_BUF_MEM_CTRL_DMA_BUF_MEM_CTRL_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int afbcd_hdr_ptr_0 : 32;
    } reg;
} soc_mdc_afbcd_hreg_hdr_ptr_lo_union;
#endif
#define SOC_MDC_AFBCD_HREG_HDR_PTR_LO_AFBCD_HDR_PTR_0_START (0)
#define SOC_MDC_AFBCD_HREG_HDR_PTR_LO_AFBCD_HDR_PTR_0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int afbcd_pic_width : 16;
        unsigned int reserved : 16;
    } reg;
} soc_mdc_afbcd_hreg_pic_width_union;
#endif
#define SOC_MDC_AFBCD_HREG_PIC_WIDTH_AFBCD_PIC_WIDTH_START (0)
#define SOC_MDC_AFBCD_HREG_PIC_WIDTH_AFBCD_PIC_WIDTH_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int afbcd_pic_height : 16;
        unsigned int reserved : 16;
    } reg;
} soc_mdc_afbcd_hreg_pic_height_union;
#endif
#define SOC_MDC_AFBCD_HREG_PIC_HEIGHT_AFBCD_PIC_HEIGHT_START (0)
#define SOC_MDC_AFBCD_HREG_PIC_HEIGHT_AFBCD_PIC_HEIGHT_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int afbcd_block_split : 1;
        unsigned int afbcd_half_block : 1;
        unsigned int reserved_0 : 14;
        unsigned int reserved_1 : 5;
        unsigned int afbcd_color_transform : 1;
        unsigned int reserved_2 : 10;
    } reg;
} soc_mdc_afbcd_hreg_format_union;
#endif
#define SOC_MDC_AFBCD_HREG_FORMAT_AFBCD_BLOCK_SPLIT_START (0)
#define SOC_MDC_AFBCD_HREG_FORMAT_AFBCD_BLOCK_SPLIT_END (0)
#define SOC_MDC_AFBCD_HREG_FORMAT_AFBCD_HALF_BLOCK_START (1)
#define SOC_MDC_AFBCD_HREG_FORMAT_AFBCD_HALF_BLOCK_END (1)
#define SOC_MDC_AFBCD_HREG_FORMAT_AFBCD_COLOR_TRANSFORM_START (21)
#define SOC_MDC_AFBCD_HREG_FORMAT_AFBCD_COLOR_TRANSFORM_END (21)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 6;
        unsigned int afbcd_half_block_mode : 2;
        unsigned int reserved_1 : 24;
    } reg;
} soc_mdc_afbcd_ctl_union;
#endif
#define SOC_MDC_AFBCD_CTL_AFBCD_HALF_BLOCK_MODE_START (6)
#define SOC_MDC_AFBCD_CTL_AFBCD_HALF_BLOCK_MODE_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int afbcd_stretch_inc : 8;
        unsigned int afbcd_stretch_acc : 4;
        unsigned int reserved_0 : 4;
        unsigned int hebcd_stretch_step : 12;
        unsigned int reserved_1 : 4;
    } reg;
} soc_mdc_afbcd_str_union;
#endif
#define SOC_MDC_AFBCD_STR_AFBCD_STRETCH_INC_START (0)
#define SOC_MDC_AFBCD_STR_AFBCD_STRETCH_INC_END (7)
#define SOC_MDC_AFBCD_STR_AFBCD_STRETCH_ACC_START (8)
#define SOC_MDC_AFBCD_STR_AFBCD_STRETCH_ACC_END (11)
#define SOC_MDC_AFBCD_STR_HEBCD_STRETCH_STEP_START (16)
#define SOC_MDC_AFBCD_STR_HEBCD_STRETCH_STEP_END (27)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int afbcd_bottom_crop_num : 8;
        unsigned int afbcd_top_crop_num : 8;
        unsigned int reserved : 16;
    } reg;
} soc_mdc_line_crop_union;
#endif
#define SOC_MDC_LINE_CROP_AFBCD_BOTTOM_CROP_NUM_START (0)
#define SOC_MDC_LINE_CROP_AFBCD_BOTTOM_CROP_NUM_END (7)
#define SOC_MDC_LINE_CROP_AFBCD_TOP_CROP_NUM_START (8)
#define SOC_MDC_LINE_CROP_AFBCD_TOP_CROP_NUM_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int afbcd_header_stride_0 : 14;
        unsigned int afbcd_header_start_pos : 2;
        unsigned int reserved : 16;
    } reg;
} soc_mdc_input_header_stride_union;
#endif
#define SOC_MDC_INPUT_HEADER_STRIDE_AFBCD_HEADER_STRIDE_0_START (0)
#define SOC_MDC_INPUT_HEADER_STRIDE_AFBCD_HEADER_STRIDE_0_END (13)
#define SOC_MDC_INPUT_HEADER_STRIDE_AFBCD_HEADER_START_POS_START (14)
#define SOC_MDC_INPUT_HEADER_STRIDE_AFBCD_HEADER_START_POS_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int afbcd_payload_stride_0 : 20;
        unsigned int reserved : 12;
    } reg;
} soc_mdc_afbcd_payload_stride_union;
#endif
#define SOC_MDC_AFBCD_PAYLOAD_STRIDE_AFBCD_PAYLOAD_STRIDE_0_START (0)
#define SOC_MDC_AFBCD_PAYLOAD_STRIDE_AFBCD_PAYLOAD_STRIDE_0_END (19)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int afbcd_mm_base_0 : 32;
    } reg;
} soc_mdc_mm_base_union;
#endif
#define SOC_MDC_MM_BASE_AFBCD_MM_BASE_0_START (0)
#define SOC_MDC_MM_BASE_AFBCD_MM_BASE_0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int afbcd_payload_ptr_0 : 32;
    } reg;
} soc_mdc_afbcd_payload_pointer_union;
#endif
#define SOC_MDC_AFBCD_PAYLOAD_POINTER_AFBCD_PAYLOAD_PTR_0_START (0)
#define SOC_MDC_AFBCD_PAYLOAD_POINTER_AFBCD_PAYLOAD_PTR_0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int afbcd_pic_height_bf_stretch : 16;
        unsigned int reserved : 16;
    } reg;
} soc_mdc_height_bf_str_union;
#endif
#define SOC_MDC_HEIGHT_BF_STR_AFBCD_PIC_HEIGHT_BF_STRETCH_START (0)
#define SOC_MDC_HEIGHT_BF_STR_AFBCD_PIC_HEIGHT_BF_STRETCH_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int afbcd_mmrd_os : 4;
        unsigned int afbcd_mmwr_os : 4;
        unsigned int afbcd_ddrr_os : 6;
        unsigned int hfbcd_mmrd_os : 4;
        unsigned int hfbcd_mmwr_os : 4;
        unsigned int hfbcd_ddrr_os : 6;
        unsigned int reserved : 4;
    } reg;
} soc_mdc_os_cfg_union;
#endif
#define SOC_MDC_OS_CFG_AFBCD_MMRD_OS_START (0)
#define SOC_MDC_OS_CFG_AFBCD_MMRD_OS_END (3)
#define SOC_MDC_OS_CFG_AFBCD_MMWR_OS_START (4)
#define SOC_MDC_OS_CFG_AFBCD_MMWR_OS_END (7)
#define SOC_MDC_OS_CFG_AFBCD_DDRR_OS_START (8)
#define SOC_MDC_OS_CFG_AFBCD_DDRR_OS_END (13)
#define SOC_MDC_OS_CFG_HFBCD_MMRD_OS_START (14)
#define SOC_MDC_OS_CFG_HFBCD_MMRD_OS_END (17)
#define SOC_MDC_OS_CFG_HFBCD_MMWR_OS_START (18)
#define SOC_MDC_OS_CFG_HFBCD_MMWR_OS_END (21)
#define SOC_MDC_OS_CFG_HFBCD_DDRR_OS_START (22)
#define SOC_MDC_OS_CFG_HFBCD_DDRR_OS_END (27)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int afbcd_mem_ctrl_0 : 4;
        unsigned int afbcd_mem_ctrl_1 : 4;
        unsigned int afbcd_mem_ctrl_2 : 4;
        unsigned int afbcd_mem_ctrl_3 : 4;
        unsigned int reserved : 16;
    } reg;
} soc_mdc_afbcd_mem_ctrl_union;
#endif
#define SOC_MDC_AFBCD_MEM_CTRL_AFBCD_MEM_CTRL_0_START (0)
#define SOC_MDC_AFBCD_MEM_CTRL_AFBCD_MEM_CTRL_0_END (3)
#define SOC_MDC_AFBCD_MEM_CTRL_AFBCD_MEM_CTRL_1_START (4)
#define SOC_MDC_AFBCD_MEM_CTRL_AFBCD_MEM_CTRL_1_END (7)
#define SOC_MDC_AFBCD_MEM_CTRL_AFBCD_MEM_CTRL_2_START (8)
#define SOC_MDC_AFBCD_MEM_CTRL_AFBCD_MEM_CTRL_2_END (11)
#define SOC_MDC_AFBCD_MEM_CTRL_AFBCD_MEM_CTRL_3_START (12)
#define SOC_MDC_AFBCD_MEM_CTRL_AFBCD_MEM_CTRL_3_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int afbcd_scramble_mode : 2;
        unsigned int hfbcd_scramble_mode : 4;
        unsigned int reserved_0 : 2;
        unsigned int hebcd_scramble_mode : 4;
        unsigned int reserved_1 : 20;
    } reg;
} soc_mdc_afbcd_scramble_mode_union;
#endif
#define SOC_MDC_AFBCD_SCRAMBLE_MODE_AFBCD_SCRAMBLE_MODE_START (0)
#define SOC_MDC_AFBCD_SCRAMBLE_MODE_AFBCD_SCRAMBLE_MODE_END (1)
#define SOC_MDC_AFBCD_SCRAMBLE_MODE_HFBCD_SCRAMBLE_MODE_START (2)
#define SOC_MDC_AFBCD_SCRAMBLE_MODE_HFBCD_SCRAMBLE_MODE_END (5)
#define SOC_MDC_AFBCD_SCRAMBLE_MODE_HEBCD_SCRAMBLE_MODE_START (8)
#define SOC_MDC_AFBCD_SCRAMBLE_MODE_HEBCD_SCRAMBLE_MODE_END (11)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int afbcd_header_pointer_offset : 32;
    } reg;
} soc_mdc_afbcd_header_pointer_offset_union;
#endif
#define SOC_MDC_AFBCD_HEADER_POINTER_OFFSET_AFBCD_HEADER_POINTER_OFFSET_START (0)
#define SOC_MDC_AFBCD_HEADER_POINTER_OFFSET_AFBCD_HEADER_POINTER_OFFSET_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int afbcd_monitor_reg1 : 32;
    } reg;
} soc_mdc_afbcd_monitor_reg1_union;
#endif
#define SOC_MDC_AFBCD_MONITOR_REG1_AFBCD_MONITOR_REG1_START (0)
#define SOC_MDC_AFBCD_MONITOR_REG1_AFBCD_MONITOR_REG1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int afbcd_monitor_reg2 : 32;
    } reg;
} soc_mdc_afbcd_monitor_reg2_union;
#endif
#define SOC_MDC_AFBCD_MONITOR_REG2_AFBCD_MONITOR_REG2_START (0)
#define SOC_MDC_AFBCD_MONITOR_REG2_AFBCD_MONITOR_REG2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int afbcd_monitor_reg3 : 32;
    } reg;
} soc_mdc_afbcd_monitor_reg3_union;
#endif
#define SOC_MDC_AFBCD_MONITOR_REG3_AFBCD_MONITOR_REG3_START (0)
#define SOC_MDC_AFBCD_MONITOR_REG3_AFBCD_MONITOR_REG3_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int afbcd_debug_reg0 : 32;
    } reg;
} soc_mdc_afbcd_debug_reg0_union;
#endif
#define SOC_MDC_AFBCD_DEBUG_REG0_AFBCD_DEBUG_REG0_START (0)
#define SOC_MDC_AFBCD_DEBUG_REG0_AFBCD_DEBUG_REG0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int fbcd_creg_fbcd_mode : 1;
        unsigned int fbcd_creg_hfbcd_noheader_mode : 1;
        unsigned int reserved_0 : 2;
        unsigned int hebcd_enable : 1;
        unsigned int hebcd_raw_mode : 1;
        unsigned int hebcd_blk4x4_en : 1;
        unsigned int reserved_1 : 25;
    } reg;
} soc_mdc_fbcd_creg_fbcd_ctrl_mode_union;
#endif
#define SOC_MDC_FBCD_CREG_FBCD_CTRL_MODE_FBCD_CREG_FBCD_MODE_START (0)
#define SOC_MDC_FBCD_CREG_FBCD_CTRL_MODE_FBCD_CREG_FBCD_MODE_END (0)
#define SOC_MDC_FBCD_CREG_FBCD_CTRL_MODE_FBCD_CREG_HFBCD_NOHEADER_MODE_START (1)
#define SOC_MDC_FBCD_CREG_FBCD_CTRL_MODE_FBCD_CREG_HFBCD_NOHEADER_MODE_END (1)
#define SOC_MDC_FBCD_CREG_FBCD_CTRL_MODE_HEBCD_ENABLE_START (4)
#define SOC_MDC_FBCD_CREG_FBCD_CTRL_MODE_HEBCD_ENABLE_END (4)
#define SOC_MDC_FBCD_CREG_FBCD_CTRL_MODE_HEBCD_RAW_MODE_START (5)
#define SOC_MDC_FBCD_CREG_FBCD_CTRL_MODE_HEBCD_RAW_MODE_END (5)
#define SOC_MDC_FBCD_CREG_FBCD_CTRL_MODE_HEBCD_BLK4X4_EN_START (6)
#define SOC_MDC_FBCD_CREG_FBCD_CTRL_MODE_HEBCD_BLK4X4_EN_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int fbcd_hdr_ptr_1 : 32;
    } reg;
} soc_mdc_afbcd_hreg_hdr_ptr_lo_1_union;
#endif
#define SOC_MDC_AFBCD_HREG_HDR_PTR_LO_1_FBCD_HDR_PTR_1_START (0)
#define SOC_MDC_AFBCD_HREG_HDR_PTR_LO_1_FBCD_HDR_PTR_1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int fbcd_payload_ptr_1 : 32;
    } reg;
} soc_mdc_afbcd_hreg_pld_ptr_lo_1_union;
#endif
#define SOC_MDC_AFBCD_HREG_PLD_PTR_LO_1_FBCD_PAYLOAD_PTR_1_START (0)
#define SOC_MDC_AFBCD_HREG_PLD_PTR_LO_1_FBCD_PAYLOAD_PTR_1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int fbcd_header_stride_1 : 14;
        unsigned int reserved : 18;
    } reg;
} soc_mdc_fbcd_header_srtide_1_union;
#endif
#define SOC_MDC_HEADER_SRTIDE_1_FBCD_HEADER_STRIDE_1_START (0)
#define SOC_MDC_HEADER_SRTIDE_1_FBCD_HEADER_STRIDE_1_END (13)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int fbcd_payload_stride_1 : 20;
        unsigned int reserved : 12;
    } reg;
} soc_mdc_fbcd_payload_stride_1_union;
#endif
#define SOC_MDC_PAYLOAD_STRIDE_1_FBCD_PAYLOAD_STRIDE_1_START (0)
#define SOC_MDC_PAYLOAD_STRIDE_1_FBCD_PAYLOAD_STRIDE_1_END (19)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int fbcd_block_type : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_fbcd_block_type_union;
#endif
#define SOC_MDC_FBCD_BLOCK_TYPE_FBCD_BLOCK_TYPE_START (0)
#define SOC_MDC_FBCD_BLOCK_TYPE_FBCD_BLOCK_TYPE_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int fbcd_mm_base_1 : 32;
    } reg;
} soc_mdc_mm_base_1_union;
#endif
#define SOC_MDC_MM_BASE_1_FBCD_MM_BASE_1_START (0)
#define SOC_MDC_MM_BASE_1_FBCD_MM_BASE_1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int fbcd_mm_base_2 : 32;
    } reg;
} soc_mdc_mm_base_2_union;
#endif
#define SOC_MDC_MM_BASE_2_FBCD_MM_BASE_2_START (0)
#define SOC_MDC_MM_BASE_2_FBCD_MM_BASE_2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int fbcd_mm_base_3 : 32;
    } reg;
} soc_mdc_mm_base_3_union;
#endif
#define SOC_MDC_MM_BASE_3_FBCD_MM_BASE_3_START (0)
#define SOC_MDC_MM_BASE_3_FBCD_MM_BASE_3_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hfbcd_mem_ctrl_0 : 4;
        unsigned int hfbcd_mem_ctrl_1 : 4;
        unsigned int hfbcd_mem_ctrl_2 : 4;
        unsigned int hfbcd_mem_ctrl_3 : 4;
        unsigned int hfbcd_mem_ctrl_4 : 4;
        unsigned int hfbcd_mem_ctrl_5 : 4;
        unsigned int hfbcd_mem_ctrl_6 : 4;
        unsigned int hfbcd_mem_ctrl_7 : 4;
    } reg;
} soc_mdc_hfbcd_mem_ctrl_union;
#endif
#define SOC_MDC_HFBCD_MEM_CTRL_HFBCD_MEM_CTRL_0_START (0)
#define SOC_MDC_HFBCD_MEM_CTRL_HFBCD_MEM_CTRL_0_END (3)
#define SOC_MDC_HFBCD_MEM_CTRL_HFBCD_MEM_CTRL_1_START (4)
#define SOC_MDC_HFBCD_MEM_CTRL_HFBCD_MEM_CTRL_1_END (7)
#define SOC_MDC_HFBCD_MEM_CTRL_HFBCD_MEM_CTRL_2_START (8)
#define SOC_MDC_HFBCD_MEM_CTRL_HFBCD_MEM_CTRL_2_END (11)
#define SOC_MDC_HFBCD_MEM_CTRL_HFBCD_MEM_CTRL_3_START (12)
#define SOC_MDC_HFBCD_MEM_CTRL_HFBCD_MEM_CTRL_3_END (15)
#define SOC_MDC_HFBCD_MEM_CTRL_HFBCD_MEM_CTRL_4_START (16)
#define SOC_MDC_HFBCD_MEM_CTRL_HFBCD_MEM_CTRL_4_END (19)
#define SOC_MDC_HFBCD_MEM_CTRL_HFBCD_MEM_CTRL_5_START (20)
#define SOC_MDC_HFBCD_MEM_CTRL_HFBCD_MEM_CTRL_5_END (23)
#define SOC_MDC_HFBCD_MEM_CTRL_HFBCD_MEM_CTRL_6_START (24)
#define SOC_MDC_HFBCD_MEM_CTRL_HFBCD_MEM_CTRL_6_END (27)
#define SOC_MDC_HFBCD_MEM_CTRL_HFBCD_MEM_CTRL_7_START (28)
#define SOC_MDC_HFBCD_MEM_CTRL_HFBCD_MEM_CTRL_7_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hfbcd_mem_ctrl_8 : 4;
        unsigned int hfbcd_mem_ctrl_9 : 4;
        unsigned int hfbcd_mem_ctrl_10 : 4;
        unsigned int reserved : 20;
    } reg;
} soc_mdc_hfbcd_mem_ctrl_1_union;
#endif
#define SOC_MDC_HFBCD_MEM_CTRL_1_HFBCD_MEM_CTRL_8_START (0)
#define SOC_MDC_HFBCD_MEM_CTRL_1_HFBCD_MEM_CTRL_8_END (3)
#define SOC_MDC_HFBCD_MEM_CTRL_1_HFBCD_MEM_CTRL_9_START (4)
#define SOC_MDC_HFBCD_MEM_CTRL_1_HFBCD_MEM_CTRL_9_END (7)
#define SOC_MDC_HFBCD_MEM_CTRL_1_HFBCD_MEM_CTRL_10_START (8)
#define SOC_MDC_HFBCD_MEM_CTRL_1_HFBCD_MEM_CTRL_10_END (11)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int fbcd_creg_clkgate_off : 1;
        unsigned int fbcd_creg_mem_sd_off : 1;
        unsigned int fbcd_creg_hfbcd_ip_auto_clk_gt_en : 1;
        unsigned int fbcd_creg_fbcd_auto_clk_gt_en : 1;
        unsigned int reserved : 28;
    } reg;
} soc_mdc_fbcd_creg_fbcd_clk_gate_union;
#endif
#define SOC_MDC_FBCD_CREG_FBCD_CLK_GATE_FBCD_CREG_CLKGATE_OFF_START (0)
#define SOC_MDC_FBCD_CREG_FBCD_CLK_GATE_FBCD_CREG_CLKGATE_OFF_END (0)
#define SOC_MDC_FBCD_CREG_FBCD_CLK_GATE_FBCD_CREG_MEM_SD_OFF_START (1)
#define SOC_MDC_FBCD_CREG_FBCD_CLK_GATE_FBCD_CREG_MEM_SD_OFF_END (1)
#define SOC_MDC_FBCD_CREG_FBCD_CLK_GATE_FBCD_CREG_HFBCD_IP_AUTO_CLK_GT_EN_START (2)
#define SOC_MDC_FBCD_CREG_FBCD_CLK_GATE_FBCD_CREG_HFBCD_IP_AUTO_CLK_GT_EN_END (2)
#define SOC_MDC_FBCD_CREG_FBCD_CLK_GATE_FBCD_CREG_FBCD_AUTO_CLK_GT_EN_START (3)
#define SOC_MDC_FBCD_CREG_FBCD_CLK_GATE_FBCD_CREG_FBCD_AUTO_CLK_GT_EN_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hfbcd_monitor_reg1 : 32;
    } reg;
} soc_mdc_hfbcd_montior_reg1_union;
#endif
#define SOC_MDC_HFBCD_MONTIOR_REG1_HFBCD_MONITOR_REG1_START (0)
#define SOC_MDC_HFBCD_MONTIOR_REG1_HFBCD_MONITOR_REG1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hfbcd_monitor_reg2 : 32;
    } reg;
} soc_mdc_hfbcd_montior_reg2_union;
#endif
#define SOC_MDC_HFBCD_MONTIOR_REG2_HFBCD_MONITOR_REG2_START (0)
#define SOC_MDC_HFBCD_MONTIOR_REG2_HFBCD_MONITOR_REG2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hfbcd_monitor_reg3 : 32;
    } reg;
} soc_mdc_hfbcd_montior_reg3_union;
#endif
#define SOC_MDC_HFBCD_MONTIOR_REG3_HFBCD_MONITOR_REG3_START (0)
#define SOC_MDC_HFBCD_MONTIOR_REG3_HFBCD_MONITOR_REG3_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hfbcd_debug_reg0 : 32;
    } reg;
} soc_mdc_hfbcd_debug_reg0_union;
#endif
#define SOC_MDC_HFBCD_DEBUG_REG0_HFBCD_DEBUG_REG0_START (0)
#define SOC_MDC_HFBCD_DEBUG_REG0_HFBCD_DEBUG_REG0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int afbcd_monitor_reg4 : 32;
    } reg;
} soc_mdc_afbcd_montior_reg4_union;
#endif
#define SOC_MDC_AFBCD_MONTIOR_REG4_AFBCD_MONITOR_REG4_START (0)
#define SOC_MDC_AFBCD_MONTIOR_REG4_AFBCD_MONITOR_REG4_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int afbcd_monitor_reg5 : 32;
    } reg;
} soc_mdc_afbcd_montior_reg5_union;
#endif
#define SOC_MDC_AFBCD_MONTIOR_REG5_AFBCD_MONITOR_REG5_START (0)
#define SOC_MDC_AFBCD_MONTIOR_REG5_AFBCD_MONITOR_REG5_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hfbcd_monitor_reg4 : 32;
    } reg;
} soc_mdc_hfbcd_montior_reg4_union;
#endif
#define SOC_MDC_HFBCD_MONTIOR_REG4_HFBCD_MONITOR_REG4_START (0)
#define SOC_MDC_HFBCD_MONTIOR_REG4_HFBCD_MONITOR_REG4_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hfbcd_monitor_reg5 : 32;
    } reg;
} soc_mdc_hfbcd_montior_reg5_union;
#endif
#define SOC_MDC_HFBCD_MONTIOR_REG5_HFBCD_MONITOR_REG5_START (0)
#define SOC_MDC_HFBCD_MONTIOR_REG5_HFBCD_MONITOR_REG5_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hebcd_mmrd_os : 8;
        unsigned int hebcd_mmwr_os : 8;
        unsigned int hebcd_ddrr_os : 8;
        unsigned int reserved : 8;
    } reg;
} soc_mdc_hebcd_os_cfg_union;
#endif
#define SOC_MDC_HEBCD_OS_CFG_HEBCD_MMRD_OS_START (0)
#define SOC_MDC_HEBCD_OS_CFG_HEBCD_MMRD_OS_END (7)
#define SOC_MDC_HEBCD_OS_CFG_HEBCD_MMWR_OS_START (8)
#define SOC_MDC_HEBCD_OS_CFG_HEBCD_MMWR_OS_END (15)
#define SOC_MDC_HEBCD_OS_CFG_HEBCD_DDRR_OS_START (16)
#define SOC_MDC_HEBCD_OS_CFG_HEBCD_DDRR_OS_END (23)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hebcd_tag_value0 : 32;
    } reg;
} soc_mdc_hebcd_tag_value0_union;
#endif
#define SOC_MDC_HEBCD_TAG_VALUE0_HEBCD_TAG_VALUE0_START (0)
#define SOC_MDC_HEBCD_TAG_VALUE0_HEBCD_TAG_VALUE0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hebcd_tag_value1 : 32;
    } reg;
} soc_mdc_hebcd_tag_value1_union;
#endif
#define SOC_MDC_HEBCD_TAG_VALUE1_HEBCD_TAG_VALUE1_START (0)
#define SOC_MDC_HEBCD_TAG_VALUE1_HEBCD_TAG_VALUE1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hebcd_tag_value2 : 32;
    } reg;
} soc_mdc_hebcd_tag_value2_union;
#endif
#define SOC_MDC_HEBCD_TAG_VALUE2_HEBCD_TAG_VALUE2_START (0)
#define SOC_MDC_HEBCD_TAG_VALUE2_HEBCD_TAG_VALUE2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hebcd_tag_value3 : 32;
    } reg;
} soc_mdc_hebcd_tag_value3_union;
#endif
#define SOC_MDC_HEBCD_TAG_VALUE3_HEBCD_TAG_VALUE3_START (0)
#define SOC_MDC_HEBCD_TAG_VALUE3_HEBCD_TAG_VALUE3_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hebcd_reserved0 : 32;
    } reg;
} soc_mdc_hebcd_reserved0_union;
#endif
#define SOC_MDC_HEBCD_RESERVED0_HEBCD_RESERVED0_START (0)
#define SOC_MDC_HEBCD_RESERVED0_HEBCD_RESERVED0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hebcd_reserved1 : 32;
    } reg;
} soc_mdc_hebcd_reserved1_union;
#endif
#define SOC_MDC_HEBCD_RESERVED1_HEBCD_RESERVED1_START (0)
#define SOC_MDC_HEBCD_RESERVED1_HEBCD_RESERVED1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hebcd_reserved2 : 32;
    } reg;
} soc_mdc_hebcd_reserved2_union;
#endif
#define SOC_MDC_HEBCD_RESERVED2_HEBCD_RESERVED2_START (0)
#define SOC_MDC_HEBCD_RESERVED2_HEBCD_RESERVED2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hebcd_reserved3 : 32;
    } reg;
} soc_mdc_hebcd_reserved3_union;
#endif
#define SOC_MDC_HEBCD_RESERVED3_HEBCD_RESERVED3_START (0)
#define SOC_MDC_HEBCD_RESERVED3_HEBCD_RESERVED3_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hebcd_monitor_reg0 : 32;
    } reg;
} soc_mdc_hebcd_monitor_reg0_union;
#endif
#define SOC_MDC_HEBCD_MONITOR_REG0_HEBCD_MONITOR_REG0_START (0)
#define SOC_MDC_HEBCD_MONITOR_REG0_HEBCD_MONITOR_REG0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hebcd_monitor_reg1 : 32;
    } reg;
} soc_mdc_hebcd_monitor_reg1_union;
#endif
#define SOC_MDC_HEBCD_MONITOR_REG1_HEBCD_MONITOR_REG1_START (0)
#define SOC_MDC_HEBCD_MONITOR_REG1_HEBCD_MONITOR_REG1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hebcd_monitor_reg2 : 32;
    } reg;
} soc_mdc_hebcd_monitor_reg2_union;
#endif
#define SOC_MDC_HEBCD_MONITOR_REG2_HEBCD_MONITOR_REG2_START (0)
#define SOC_MDC_HEBCD_MONITOR_REG2_HEBCD_MONITOR_REG2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hebcd_monitor_reg3 : 32;
    } reg;
} soc_mdc_hebcd_monitor_reg3_union;
#endif
#define SOC_MDC_HEBCD_MONITOR_REG3_HEBCD_MONITOR_REG3_START (0)
#define SOC_MDC_HEBCD_MONITOR_REG3_HEBCD_MONITOR_REG3_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hebcd_mem_ctrl_0 : 4;
        unsigned int hebcd_mem_ctrl_1 : 4;
        unsigned int reserved : 24;
    } reg;
} soc_mdc_hebcd_mem_ctrl_union;
#endif
#define SOC_MDC_HEBCD_MEM_CTRL_HEBCD_MEM_CTRL_0_START (0)
#define SOC_MDC_HEBCD_MEM_CTRL_HEBCD_MEM_CTRL_0_END (3)
#define SOC_MDC_HEBCD_MEM_CTRL_HEBCD_MEM_CTRL_1_START (4)
#define SOC_MDC_HEBCD_MEM_CTRL_HEBCD_MEM_CTRL_1_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hebcd_clk_gate0 : 1;
        unsigned int hebcd_clk_gate1 : 1;
        unsigned int hebcd_clk_gate2 : 1;
        unsigned int hebcd_clk_gate3 : 1;
        unsigned int reserved : 28;
    } reg;
} soc_mdc_hebcd_clk_gate_union;
#endif
#define SOC_MDC_HEBCD_CLK_GATE_HEBCD_CLK_GATE0_START (0)
#define SOC_MDC_HEBCD_CLK_GATE_HEBCD_CLK_GATE0_END (0)
#define SOC_MDC_HEBCD_CLK_GATE_HEBCD_CLK_GATE1_START (1)
#define SOC_MDC_HEBCD_CLK_GATE_HEBCD_CLK_GATE1_END (1)
#define SOC_MDC_HEBCD_CLK_GATE_HEBCD_CLK_GATE2_START (2)
#define SOC_MDC_HEBCD_CLK_GATE_HEBCD_CLK_GATE2_END (2)
#define SOC_MDC_HEBCD_CLK_GATE_HEBCD_CLK_GATE3_START (3)
#define SOC_MDC_HEBCD_CLK_GATE_HEBCD_CLK_GATE3_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reg_default : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_reg_default_union;
#endif
#define SOC_MDC_REG_DEFAULT_REG_DEFAULT_START (0)
#define SOC_MDC_REG_DEFAULT_REG_DEFAULT_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_addr_ext0 : 4;
        unsigned int reserved : 28;
    } reg;
} soc_mdc_dma_addr_ext_union;
#endif
#define SOC_MDC_DMA_ADDR_EXT_DMA_ADDR_EXT0_START (0)
#define SOC_MDC_DMA_ADDR_EXT_DMA_ADDR_EXT0_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int scf_video_6tap_coef : 32;
    } reg;
} soc_mdc_v0_scf_video_6tap_coef_union;
#endif
#define SOC_MDC_V0_SCF_VIDEO_6TAP_COEF_SCF_VIDEO_6TAP_COEF_START (0)
#define SOC_MDC_V0_SCF_VIDEO_6TAP_COEF_SCF_VIDEO_6TAP_COEF_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int scf_video_5tap_coef : 32;
    } reg;
} soc_mdc_v0_scf_video_5tap_coef_union;
#endif
#define SOC_MDC_V0_SCF_VIDEO_5TAP_COEF_SCF_VIDEO_5TAP_COEF_START (0)
#define SOC_MDC_V0_SCF_VIDEO_5TAP_COEF_SCF_VIDEO_5TAP_COEF_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int scf_video_4tap_coef : 32;
    } reg;
} soc_mdc_v0_scf_video_4tap_coef_union;
#endif
#define SOC_MDC_V0_SCF_VIDEO_4TAP_COEF_SCF_VIDEO_4TAP_COEF_START (0)
#define SOC_MDC_V0_SCF_VIDEO_4TAP_COEF_SCF_VIDEO_4TAP_COEF_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int coefy_v : 27;
        unsigned int reserved : 5;
    } reg;
} soc_mdc_coefy_v_union;
#endif
#define SOC_MDC_COEFY_V_COEFY_V_START (0)
#define SOC_MDC_COEFY_V_COEFY_V_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int coefy_h : 27;
        unsigned int reserved : 5;
    } reg;
} soc_mdc_coefy_h_union;
#endif
#define SOC_MDC_COEFY_H_COEFY_H_START (0)
#define SOC_MDC_COEFY_H_COEFY_H_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int coefa_v : 18;
        unsigned int reserved : 14;
    } reg;
} soc_mdc_coefa_v_union;
#endif
#define SOC_MDC_COEFA_V_COEFA_V_START (0)
#define SOC_MDC_COEFA_V_COEFA_V_END (17)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int coefa_h : 18;
        unsigned int reserved : 14;
    } reg;
} soc_mdc_coefa_h_union;
#endif
#define SOC_MDC_COEFA_H_COEFA_H_START (0)
#define SOC_MDC_COEFA_H_COEFA_H_END (17)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int coefuv_v : 18;
        unsigned int reserved : 14;
    } reg;
} soc_mdc_coefuv_v_union;
#endif
#define SOC_MDC_COEFUV_V_COEFUV_V_START (0)
#define SOC_MDC_COEFUV_V_COEFUV_V_END (17)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int coefuv_h : 18;
        unsigned int reserved : 14;
    } reg;
} soc_mdc_coefuv_h_union;
#endif
#define SOC_MDC_COEFUV_H_COEFUV_H_START (0)
#define SOC_MDC_COEFUV_H_COEFUV_H_END (17)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int cld_en : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_cld_en_union;
#endif
#define SOC_MDC_CLD_EN_CLD_EN_START (0)
#define SOC_MDC_CLD_EN_CLD_EN_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int cld_size_vrt : 13;
        unsigned int reserved : 19;
    } reg;
} soc_mdc_cld_size_vrt_union;
#endif
#define SOC_MDC_CLD_SIZE_VRT_CLD_SIZE_VRT_START (0)
#define SOC_MDC_CLD_SIZE_VRT_CLD_SIZE_VRT_END (12)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int cld_size_hrz : 13;
        unsigned int reserved : 19;
    } reg;
} soc_mdc_cld_size_hrz_union;
#endif
#define SOC_MDC_CLD_SIZE_HRZ_CLD_SIZE_HRZ_START (0)
#define SOC_MDC_CLD_SIZE_HRZ_CLD_SIZE_HRZ_END (12)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int cld_rgb : 24;
        unsigned int reserved : 8;
    } reg;
} soc_mdc_cld_rgb_union;
#endif
#define SOC_MDC_CLD_RGB_CLD_RGB_START (0)
#define SOC_MDC_CLD_RGB_CLD_RGB_END (23)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_fifo_clr : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_dma_fifo_clr_union;
#endif
#define SOC_MDC_DMA_FIFO_CLR_DMA_FIFO_CLR_START (0)
#define SOC_MDC_DMA_FIFO_CLR_DMA_FIFO_CLR_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_counter_en : 1;
        unsigned int dma_counter_inc_cnum : 4;
        unsigned int dma_counter_rst_cnum : 4;
        unsigned int wch_enc_slice_h : 8;
        unsigned int wch_enc_space : 4;
        unsigned int reserved : 11;
    } reg;
} soc_mdc_dma_counter_dgb_union;
#endif
#define SOC_MDC_DMA_COUNTER_DGB_DMA_COUNTER_EN_START (0)
#define SOC_MDC_DMA_COUNTER_DGB_DMA_COUNTER_EN_END (0)
#define SOC_MDC_DMA_COUNTER_DGB_DMA_COUNTER_INC_CNUM_START (1)
#define SOC_MDC_DMA_COUNTER_DGB_DMA_COUNTER_INC_CNUM_END (4)
#define SOC_MDC_DMA_COUNTER_DGB_DMA_COUNTER_RST_CNUM_START (5)
#define SOC_MDC_DMA_COUNTER_DGB_DMA_COUNTER_RST_CNUM_END (8)
#define SOC_MDC_DMA_COUNTER_DGB_WCH_ENC_SLICE_H_START (9)
#define SOC_MDC_DMA_COUNTER_DGB_WCH_ENC_SLICE_H_END (16)
#define SOC_MDC_DMA_COUNTER_DGB_WCH_ENC_SPACE_START (17)
#define SOC_MDC_DMA_COUNTER_DGB_WCH_ENC_SPACE_END (20)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_rot_burst4 : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_dma_rot_burst4_union;
#endif
#define SOC_MDC_DMA_ROT_BURST4_DMA_ROT_BURST4_START (0)
#define SOC_MDC_DMA_ROT_BURST4_DMA_ROT_BURST4_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_reload_rsv1_reg : 32;
    } reg;
} soc_mdc_dma_rsv1_union;
#endif
#define SOC_MDC_DMA_RSV1_DMA_RELOAD_RSV1_REG_START (0)
#define SOC_MDC_DMA_RSV1_DMA_RELOAD_RSV1_REG_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_rsv2_reg : 32;
    } reg;
} soc_mdc_dma_rsv2_union;
#endif
#define SOC_MDC_DMA_RSV2_DMA_RSV2_REG_START (0)
#define SOC_MDC_DMA_RSV2_DMA_RSV2_REG_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_sw_mask_en : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_dma_sw_mask_en_union;
#endif
#define SOC_MDC_DMA_SW_MASK_EN_DMA_SW_MASK_EN_START (0)
#define SOC_MDC_DMA_SW_MASK_EN_DMA_SW_MASK_EN_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_start_mask0 : 32;
    } reg;
} soc_mdc_dma_start_mask0_union;
#endif
#define SOC_MDC_DMA_START_MASK0_DMA_START_MASK0_START (0)
#define SOC_MDC_DMA_START_MASK0_DMA_START_MASK0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_end_mask0 : 32;
    } reg;
} soc_mdc_dma_end_mask0_union;
#endif
#define SOC_MDC_DMA_END_MASK0_DMA_END_MASK0_START (0)
#define SOC_MDC_DMA_END_MASK0_DMA_END_MASK0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_start_mask1 : 32;
    } reg;
} soc_mdc_dma_start_mask1_union;
#endif
#define SOC_MDC_DMA_START_MASK1_DMA_START_MASK1_START (0)
#define SOC_MDC_DMA_START_MASK1_DMA_START_MASK1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dma_end_mask1 : 32;
    } reg;
} soc_mdc_dma_end_mask1_union;
#endif
#define SOC_MDC_DMA_END_MASK1_DMA_END_MASK1_START (0)
#define SOC_MDC_DMA_END_MASK1_DMA_END_MASK1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int wch_clk_sel : 32;
    } reg;
} soc_mdc_wch_clk_sel_union;
#endif
#define SOC_MDC_WCH_CLK_SEL_WCH_CLK_SEL_START (0)
#define SOC_MDC_WCH_CLK_SEL_WCH_CLK_SEL_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int wch_clk_en : 32;
    } reg;
} soc_mdc_wch_clk_en_union;
#endif
#define SOC_MDC_WCH_CLK_EN_WCH_CLK_EN_START (0)
#define SOC_MDC_WCH_CLK_EN_WCH_CLK_EN_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dfc_size_vrt : 13;
        unsigned int reserved_0 : 3;
        unsigned int dfc_size_hrz : 13;
        unsigned int reserved_1 : 3;
    } reg;
} soc_mdc_dfc_disp_size_union;
#endif
#define SOC_MDC_DFC_DISP_SIZE_DFC_SIZE_VRT_START (0)
#define SOC_MDC_DFC_DISP_SIZE_DFC_SIZE_VRT_END (12)
#define SOC_MDC_DFC_DISP_SIZE_DFC_SIZE_HRZ_START (16)
#define SOC_MDC_DFC_DISP_SIZE_DFC_SIZE_HRZ_END (28)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dfc_pix_in_num : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_dfc_pix_in_num_union;
#endif
#define SOC_MDC_DFC_PIX_IN_NUM_DFC_PIX_IN_NUM_START (0)
#define SOC_MDC_DFC_PIX_IN_NUM_DFC_PIX_IN_NUM_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dfc_glb_alpha1 : 10;
        unsigned int reserved_0 : 6;
        unsigned int dfc_glb_alpha0 : 10;
        unsigned int reserved_1 : 6;
    } reg;
} soc_mdc_dfc_glb_alpha01_union;
#endif
#define SOC_MDC_DFC_GLB_ALPHA01_DFC_GLB_ALPHA1_START (0)
#define SOC_MDC_DFC_GLB_ALPHA01_DFC_GLB_ALPHA1_END (9)
#define SOC_MDC_DFC_GLB_ALPHA01_DFC_GLB_ALPHA0_START (16)
#define SOC_MDC_DFC_GLB_ALPHA01_DFC_GLB_ALPHA0_END (25)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dfc_ax_swap : 1;
        unsigned int dfc_img_fmt : 5;
        unsigned int dfc_uv_swap : 1;
        unsigned int dfc_rb_swap : 1;
        unsigned int reserved : 24;
    } reg;
} soc_mdc_dfc_disp_fmt_union;
#endif
#define SOC_MDC_DFC_DISP_FMT_DFC_AX_SWAP_START (0)
#define SOC_MDC_DFC_DISP_FMT_DFC_AX_SWAP_END (0)
#define SOC_MDC_DFC_DISP_FMT_DFC_IMG_FMT_START (1)
#define SOC_MDC_DFC_DISP_FMT_DFC_IMG_FMT_END (5)
#define SOC_MDC_DFC_DISP_FMT_DFC_UV_SWAP_START (6)
#define SOC_MDC_DFC_DISP_FMT_DFC_UV_SWAP_END (6)
#define SOC_MDC_DFC_DISP_FMT_DFC_RB_SWAP_START (7)
#define SOC_MDC_DFC_DISP_FMT_DFC_RB_SWAP_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dfc_right_clip : 6;
        unsigned int reserved_0 : 10;
        unsigned int dfc_left_clip : 6;
        unsigned int reserved_1 : 10;
    } reg;
} soc_mdc_dfc_clip_ctl_hrz_union;
#endif
#define SOC_MDC_DFC_CLIP_CTL_HRZ_DFC_RIGHT_CLIP_START (0)
#define SOC_MDC_DFC_CLIP_CTL_HRZ_DFC_RIGHT_CLIP_END (5)
#define SOC_MDC_DFC_CLIP_CTL_HRZ_DFC_LEFT_CLIP_START (16)
#define SOC_MDC_DFC_CLIP_CTL_HRZ_DFC_LEFT_CLIP_END (21)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dfc_bot_clip : 6;
        unsigned int reserved_0 : 10;
        unsigned int dfc_top_clip : 6;
        unsigned int reserved_1 : 10;
    } reg;
} soc_mdc_dfc_clip_ctl_vrz_union;
#endif
#define SOC_MDC_DFC_CLIP_CTL_VRZ_DFC_BOT_CLIP_START (0)
#define SOC_MDC_DFC_CLIP_CTL_VRZ_DFC_BOT_CLIP_END (5)
#define SOC_MDC_DFC_CLIP_CTL_VRZ_DFC_TOP_CLIP_START (16)
#define SOC_MDC_DFC_CLIP_CTL_VRZ_DFC_TOP_CLIP_END (21)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dfc_ctl_clip_enable : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_dfc_ctl_clip_en_union;
#endif
#define SOC_MDC_DFC_CTL_CLIP_EN_DFC_CTL_CLIP_ENABLE_START (0)
#define SOC_MDC_DFC_CTL_CLIP_EN_DFC_CTL_CLIP_ENABLE_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dfc_module_en : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_dfc_icg_module_union;
#endif
#define SOC_MDC_DFC_ICG_MODULE_DFC_MODULE_EN_START (0)
#define SOC_MDC_DFC_ICG_MODULE_DFC_MODULE_EN_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dfc_dither_en : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_dfc_dither_enable_union;
#endif
#define SOC_MDC_DFC_DITHER_ENABLE_DFC_DITHER_EN_START (0)
#define SOC_MDC_DFC_DITHER_ENABLE_DFC_DITHER_EN_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dfc_left_pad : 6;
        unsigned int reserved_0 : 2;
        unsigned int dfc_right_pad : 6;
        unsigned int reserved_1 : 2;
        unsigned int dfc_top_pad : 6;
        unsigned int reserved_2 : 2;
        unsigned int dfc_bot_pad : 6;
        unsigned int reserved_3 : 1;
        unsigned int dfc_ctl_pad_enable : 1;
    } reg;
} soc_mdc_dfc_padding_ctl_union;
#endif
#define SOC_MDC_DFC_PADDING_CTL_DFC_LEFT_PAD_START (0)
#define SOC_MDC_DFC_PADDING_CTL_DFC_LEFT_PAD_END (5)
#define SOC_MDC_DFC_PADDING_CTL_DFC_RIGHT_PAD_START (8)
#define SOC_MDC_DFC_PADDING_CTL_DFC_RIGHT_PAD_END (13)
#define SOC_MDC_DFC_PADDING_CTL_DFC_TOP_PAD_START (16)
#define SOC_MDC_DFC_PADDING_CTL_DFC_TOP_PAD_END (21)
#define SOC_MDC_DFC_PADDING_CTL_DFC_BOT_PAD_START (24)
#define SOC_MDC_DFC_PADDING_CTL_DFC_BOT_PAD_END (29)
#define SOC_MDC_DFC_PADDING_CTL_DFC_CTL_PAD_ENABLE_START (31)
#define SOC_MDC_DFC_PADDING_CTL_DFC_CTL_PAD_ENABLE_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dfc_glb_alpha3 : 10;
        unsigned int reserved_0 : 6;
        unsigned int dfc_glb_alpha2 : 10;
        unsigned int reserved_1 : 6;
    } reg;
} soc_mdc_dfc_glb_alpha23_union;
#endif
#define SOC_MDC_DFC_GLB_ALPHA23_DFC_GLB_ALPHA3_START (0)
#define SOC_MDC_DFC_GLB_ALPHA23_DFC_GLB_ALPHA3_END (9)
#define SOC_MDC_DFC_GLB_ALPHA23_DFC_GLB_ALPHA2_START (16)
#define SOC_MDC_DFC_GLB_ALPHA23_DFC_GLB_ALPHA2_END (25)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 16;
        unsigned int dfc_glb_alpha : 10;
        unsigned int reserved_1 : 6;
    } reg;
} soc_mdc_dfc_glb_alpha_union;
#endif
#define SOC_MDC_DFC_GLB_ALPHA_DFC_GLB_ALPHA_START (16)
#define SOC_MDC_DFC_GLB_ALPHA_DFC_GLB_ALPHA_END (25)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bayer_alpha_handle_mode : 2;
        unsigned int bayer_alpha_shift_mode : 2;
        unsigned int reserved : 28;
    } reg;
} soc_mdc_dfc_alpha_ctl_union;
#endif
#define SOC_MDC_DFC_ALPHA_CTL_BAYER_ALPHA_HANDLE_MODE_START (0)
#define SOC_MDC_DFC_ALPHA_CTL_BAYER_ALPHA_HANDLE_MODE_END (1)
#define SOC_MDC_DFC_ALPHA_CTL_BAYER_ALPHA_SHIFT_MODE_START (2)
#define SOC_MDC_DFC_ALPHA_CTL_BAYER_ALPHA_SHIFT_MODE_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bayer_alpha_thd : 30;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_dfc_alpha_thd_union;
#endif
#define SOC_MDC_DFC_ALPHA_THD_BAYER_ALPHA_THD_START (0)
#define SOC_MDC_DFC_ALPHA_THD_BAYER_ALPHA_THD_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dither_en : 1;
        unsigned int dither_hifreq_noise_mode : 2;
        unsigned int dither_rgb_shift_mode : 1;
        unsigned int dither_unifrom_en : 1;
        unsigned int bayer_rgb_en : 1;
        unsigned int reserved : 26;
    } reg;
} soc_mdc_dither_ctl1_union;
#endif
#define SOC_MDC_DITHER_CTL1_DITHER_EN_START (0)
#define SOC_MDC_DITHER_CTL1_DITHER_EN_END (0)
#define SOC_MDC_DITHER_CTL1_DITHER_HIFREQ_NOISE_MODE_START (1)
#define SOC_MDC_DITHER_CTL1_DITHER_HIFREQ_NOISE_MODE_END (2)
#define SOC_MDC_DITHER_CTL1_DITHER_RGB_SHIFT_MODE_START (3)
#define SOC_MDC_DITHER_CTL1_DITHER_RGB_SHIFT_MODE_END (3)
#define SOC_MDC_DITHER_CTL1_DITHER_UNIFROM_EN_START (4)
#define SOC_MDC_DITHER_CTL1_DITHER_UNIFROM_EN_END (4)
#define SOC_MDC_DITHER_CTL1_BAYER_RGB_EN_START (5)
#define SOC_MDC_DITHER_CTL1_BAYER_RGB_EN_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dither_tri_thd10_b : 10;
        unsigned int dither_tri_thd10_g : 10;
        unsigned int dither_tri_thd10_r : 10;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_dither_tri_thd10_union;
#endif
#define SOC_MDC_DITHER_TRI_THD10_DITHER_TRI_THD10_B_START (0)
#define SOC_MDC_DITHER_TRI_THD10_DITHER_TRI_THD10_B_END (9)
#define SOC_MDC_DITHER_TRI_THD10_DITHER_TRI_THD10_G_START (10)
#define SOC_MDC_DITHER_TRI_THD10_DITHER_TRI_THD10_G_END (19)
#define SOC_MDC_DITHER_TRI_THD10_DITHER_TRI_THD10_R_START (20)
#define SOC_MDC_DITHER_TRI_THD10_DITHER_TRI_THD10_R_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dither_tri_thd10_uni_b : 10;
        unsigned int dither_tri_thd10_uni_g : 10;
        unsigned int dither_tri_thd10_uni_r : 10;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_dither_tri_thd10_uni_union;
#endif
#define SOC_MDC_DITHER_TRI_THD10_UNI_DITHER_TRI_THD10_UNI_B_START (0)
#define SOC_MDC_DITHER_TRI_THD10_UNI_DITHER_TRI_THD10_UNI_B_END (9)
#define SOC_MDC_DITHER_TRI_THD10_UNI_DITHER_TRI_THD10_UNI_G_START (10)
#define SOC_MDC_DITHER_TRI_THD10_UNI_DITHER_TRI_THD10_UNI_G_END (19)
#define SOC_MDC_DITHER_TRI_THD10_UNI_DITHER_TRI_THD10_UNI_R_START (20)
#define SOC_MDC_DITHER_TRI_THD10_UNI_DITHER_TRI_THD10_UNI_R_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 16;
        unsigned int bayer_offset : 12;
        unsigned int reserved_1 : 4;
    } reg;
} soc_mdc_bayer_ctl_union;
#endif
#define SOC_MDC_BAYER_CTL_BAYER_OFFSET_START (16)
#define SOC_MDC_BAYER_CTL_BAYER_OFFSET_END (27)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bayer_matrix_part1 : 32;
    } reg;
} soc_mdc_bayer_matrix_part1_union;
#endif
#define SOC_MDC_BAYER_MATRIX_PART1_BAYER_MATRIX_PART1_START (0)
#define SOC_MDC_BAYER_MATRIX_PART1_BAYER_MATRIX_PART1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bayer_matrix_part0 : 32;
    } reg;
} soc_mdc_bayer_matrix_part0_union;
#endif
#define SOC_MDC_BAYER_MATRIX_PART0_BAYER_MATRIX_PART0_START (0)
#define SOC_MDC_BAYER_MATRIX_PART0_BAYER_MATRIX_PART0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int rot_first_lns_en : 1;
        unsigned int rot_first_lns : 5;
        unsigned int reserved : 26;
    } reg;
} soc_mdc_rot_first_lns_union;
#endif
#define SOC_MDC_ROT_FIRST_LNS_ROT_FIRST_LNS_EN_START (0)
#define SOC_MDC_ROT_FIRST_LNS_ROT_FIRST_LNS_EN_END (0)
#define SOC_MDC_ROT_FIRST_LNS_ROT_FIRST_LNS_START (1)
#define SOC_MDC_ROT_FIRST_LNS_ROT_FIRST_LNS_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int rot_state : 32;
    } reg;
} soc_mdc_rot_state_union;
#endif
#define SOC_MDC_ROT_STATE_ROT_STATE_START (0)
#define SOC_MDC_ROT_STATE_ROT_STATE_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int rot_mem_ctrl : 4;
        unsigned int rot_auto_lp_en : 1;
        unsigned int reserved : 27;
    } reg;
} soc_mdc_rot_mem_ctrl_union;
#endif
#define SOC_MDC_ROT_MEM_CTRL_ROT_MEM_CTRL_START (0)
#define SOC_MDC_ROT_MEM_CTRL_ROT_MEM_CTRL_END (3)
#define SOC_MDC_ROT_MEM_CTRL_ROT_AUTO_LP_EN_START (4)
#define SOC_MDC_ROT_MEM_CTRL_ROT_AUTO_LP_EN_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int rot_img_width : 13;
        unsigned int reserved_0 : 3;
        unsigned int rot_img_height : 13;
        unsigned int reserved_1 : 3;
    } reg;
} soc_mdc_rot_size_union;
#endif
#define SOC_MDC_ROT_SIZE_ROT_IMG_WIDTH_START (0)
#define SOC_MDC_ROT_SIZE_ROT_IMG_WIDTH_END (12)
#define SOC_MDC_ROT_SIZE_ROT_IMG_HEIGHT_START (16)
#define SOC_MDC_ROT_SIZE_ROT_IMG_HEIGHT_END (28)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int rot_yuv422_mode : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_rot_422_mode_union;
#endif
#define SOC_MDC_ROT_422_MODE_ROT_YUV422_MODE_START (0)
#define SOC_MDC_ROT_422_MODE_ROT_YUV422_MODE_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int rot_cpu_start0 : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_rot_cpu_start0_union;
#endif
#define SOC_MDC_ROT_CPU_START0_ROT_CPU_START0_START (0)
#define SOC_MDC_ROT_CPU_START0_ROT_CPU_START0_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int rot_cpu_addr0 : 12;
        unsigned int reserved : 19;
        unsigned int rot_cpu_rw0 : 1;
    } reg;
} soc_mdc_rot_cpu_addr0_union;
#endif
#define SOC_MDC_ROT_CPU_ADDR0_ROT_CPU_ADDR0_START (0)
#define SOC_MDC_ROT_CPU_ADDR0_ROT_CPU_ADDR0_END (11)
#define SOC_MDC_ROT_CPU_ADDR0_ROT_CPU_RW0_START (31)
#define SOC_MDC_ROT_CPU_ADDR0_ROT_CPU_RW0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int rot_cpu_rdata0 : 32;
    } reg;
} soc_mdc_rot_cpu_rdata0_union;
#endif
#define SOC_MDC_ROT_CPU_RDATA0_ROT_CPU_RDATA0_START (0)
#define SOC_MDC_ROT_CPU_RDATA0_ROT_CPU_RDATA0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int rot_cpu_rdata1 : 32;
    } reg;
} soc_mdc_rot_cpu_rdata1_union;
#endif
#define SOC_MDC_ROT_CPU_RDATA1_ROT_CPU_RDATA1_START (0)
#define SOC_MDC_ROT_CPU_RDATA1_ROT_CPU_RDATA1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int rot_cpu_wdata0 : 32;
    } reg;
} soc_mdc_rot_cpu_wdata0_union;
#endif
#define SOC_MDC_ROT_CPU_WDATA0_ROT_CPU_WDATA0_START (0)
#define SOC_MDC_ROT_CPU_WDATA0_ROT_CPU_WDATA0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int rot_cpu_wdata1 : 32;
    } reg;
} soc_mdc_rot_cpu_wdata1_union;
#endif
#define SOC_MDC_ROT_CPU_WDATA1_ROT_CPU_WDATA1_START (0)
#define SOC_MDC_ROT_CPU_WDATA1_ROT_CPU_WDATA1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int rot_cpu_ctl1 : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_rot_cpu_ctl1_union;
#endif
#define SOC_MDC_ROT_CPU_CTL1_ROT_CPU_CTL1_START (0)
#define SOC_MDC_ROT_CPU_CTL1_ROT_CPU_CTL1_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int rot_cpu_start1 : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_rot_cpu_start1_union;
#endif
#define SOC_MDC_ROT_CPU_START1_ROT_CPU_START1_START (0)
#define SOC_MDC_ROT_CPU_START1_ROT_CPU_START1_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int rot_cpu_addr1 : 12;
        unsigned int reserved : 19;
        unsigned int rot_cpu_rw1 : 1;
    } reg;
} soc_mdc_rot_cpu_addr1_union;
#endif
#define SOC_MDC_ROT_CPU_ADDR1_ROT_CPU_ADDR1_START (0)
#define SOC_MDC_ROT_CPU_ADDR1_ROT_CPU_ADDR1_END (11)
#define SOC_MDC_ROT_CPU_ADDR1_ROT_CPU_RW1_START (31)
#define SOC_MDC_ROT_CPU_ADDR1_ROT_CPU_RW1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int rot_cpu_rdata2 : 32;
    } reg;
} soc_mdc_rot_cpu_rdata2_union;
#endif
#define SOC_MDC_ROT_CPU_RDATA2_ROT_CPU_RDATA2_START (0)
#define SOC_MDC_ROT_CPU_RDATA2_ROT_CPU_RDATA2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int rot_cpu_rdata3 : 32;
    } reg;
} soc_mdc_rot_cpu_rdata3_union;
#endif
#define SOC_MDC_ROT_CPU_RDATA3_ROT_CPU_RDATA3_START (0)
#define SOC_MDC_ROT_CPU_RDATA3_ROT_CPU_RDATA3_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int rot_cpu_wdata2 : 32;
    } reg;
} soc_mdc_rot_cpu_wdata2_union;
#endif
#define SOC_MDC_ROT_CPU_WDATA2_ROT_CPU_WDATA2_START (0)
#define SOC_MDC_ROT_CPU_WDATA2_ROT_CPU_WDATA2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int rot_cpu_wdata3 : 32;
    } reg;
} soc_mdc_rot_cpu_wdata3_union;
#endif
#define SOC_MDC_ROT_CPU_WDATA3_ROT_CPU_WDATA3_START (0)
#define SOC_MDC_ROT_CPU_WDATA3_ROT_CPU_WDATA3_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int afbce_pic_blks : 24;
        unsigned int reserved : 8;
    } reg;
} soc_mdc_afbce_hreg_pic_blks_union;
#endif
#define SOC_MDC_AFBCE_HREG_PIC_BLKS_AFBCE_PIC_BLKS_START (0)
#define SOC_MDC_AFBCE_HREG_PIC_BLKS_AFBCE_PIC_BLKS_END (23)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 16;
        unsigned int reserved_1 : 5;
        unsigned int afbce_color_transform : 1;
        unsigned int hebce_blk4x4_en : 1;
        unsigned int reserved_2 : 9;
    } reg;
} soc_mdc_afbce_hreg_format_union;
#endif
#define SOC_MDC_AFBCE_HREG_FORMAT_AFBCE_COLOR_TRANSFORM_START (21)
#define SOC_MDC_AFBCE_HREG_FORMAT_AFBCE_COLOR_TRANSFORM_END (21)
#define SOC_MDC_AFBCE_HREG_FORMAT_HEBCE_BLK4X4_EN_START (22)
#define SOC_MDC_AFBCE_HREG_FORMAT_HEBCE_BLK4X4_EN_END (22)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int afbce_hdr_ptr_0 : 32;
    } reg;
} soc_mdc_afbce_hreg_hdr_ptr_l0_union;
#endif
#define SOC_MDC_AFBCE_HREG_HDR_PTR_L0_AFBCE_HDR_PTR_0_START (0)
#define SOC_MDC_AFBCE_HREG_HDR_PTR_L0_AFBCE_HDR_PTR_0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int afbce_payload_ptr_0 : 32;
    } reg;
} soc_mdc_afbce_hreg_pld_ptr_l0_union;
#endif
#define SOC_MDC_AFBCE_HREG_PLD_PTR_L0_AFBCE_PAYLOAD_PTR_0_START (0)
#define SOC_MDC_AFBCE_HREG_PLD_PTR_L0_AFBCE_PAYLOAD_PTR_0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int afbce_block_height : 16;
        unsigned int afbce_block_width : 10;
        unsigned int reserved : 6;
    } reg;
} soc_mdc_picture_size_union;
#endif
#define SOC_MDC_PICTURE_SIZE_AFBCE_BLOCK_HEIGHT_START (0)
#define SOC_MDC_PICTURE_SIZE_AFBCE_BLOCK_HEIGHT_END (15)
#define SOC_MDC_PICTURE_SIZE_AFBCE_BLOCK_WIDTH_START (16)
#define SOC_MDC_PICTURE_SIZE_AFBCE_BLOCK_WIDTH_END (25)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int afbce_header_stride_0 : 14;
        unsigned int reserved : 18;
    } reg;
} soc_mdc_header_srtide_0_union;
#endif
#define SOC_MDC_HEADER_SRTIDE_0_AFBCE_HEADER_STRIDE_0_START (0)
#define SOC_MDC_HEADER_SRTIDE_0_AFBCE_HEADER_STRIDE_0_END (13)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int afbce_payload_stride_0 : 20;
        unsigned int reserved : 12;
    } reg;
} soc_mdc_payload_stride_0_union;
#endif
#define SOC_MDC_PAYLOAD_STRIDE_0_AFBCE_PAYLOAD_STRIDE_0_START (0)
#define SOC_MDC_PAYLOAD_STRIDE_0_AFBCE_PAYLOAD_STRIDE_0_END (19)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int afbce_ddrw_os : 6;
        unsigned int hfbce_ddrw_os : 6;
        unsigned int hebce_ddrw_os : 6;
        unsigned int reserved : 14;
    } reg;
} soc_mdc_enc_os_cfg_union;
#endif
#define SOC_MDC_ENC_OS_CFG_AFBCE_DDRW_OS_START (0)
#define SOC_MDC_ENC_OS_CFG_AFBCE_DDRW_OS_END (5)
#define SOC_MDC_ENC_OS_CFG_HFBCE_DDRW_OS_START (6)
#define SOC_MDC_ENC_OS_CFG_HFBCE_DDRW_OS_END (11)
#define SOC_MDC_ENC_OS_CFG_HEBCE_DDRW_OS_START (12)
#define SOC_MDC_ENC_OS_CFG_HEBCE_DDRW_OS_END (17)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int afbce_mem_ctrl_0 : 4;
        unsigned int afbce_mem_ctrl_1 : 4;
        unsigned int afbce_mem_ctrl_2 : 4;
        unsigned int hfbce_mem_ctrl_0 : 4;
        unsigned int hfbce_mem_ctrl_1 : 4;
        unsigned int hfbce_mem_ctrl_2 : 4;
        unsigned int hfbce_mem_ctrl_3 : 4;
        unsigned int hfbce_mem_ctrl_4 : 4;
    } reg;
} soc_mdc_afbce_mem_ctrl_union;
#endif
#define SOC_MDC_AFBCE_MEM_CTRL_AFBCE_MEM_CTRL_0_START (0)
#define SOC_MDC_AFBCE_MEM_CTRL_AFBCE_MEM_CTRL_0_END (3)
#define SOC_MDC_AFBCE_MEM_CTRL_AFBCE_MEM_CTRL_1_START (4)
#define SOC_MDC_AFBCE_MEM_CTRL_AFBCE_MEM_CTRL_1_END (7)
#define SOC_MDC_AFBCE_MEM_CTRL_AFBCE_MEM_CTRL_2_START (8)
#define SOC_MDC_AFBCE_MEM_CTRL_AFBCE_MEM_CTRL_2_END (11)
#define SOC_MDC_AFBCE_MEM_CTRL_HFBCE_MEM_CTRL_0_START (12)
#define SOC_MDC_AFBCE_MEM_CTRL_HFBCE_MEM_CTRL_0_END (15)
#define SOC_MDC_AFBCE_MEM_CTRL_HFBCE_MEM_CTRL_1_START (16)
#define SOC_MDC_AFBCE_MEM_CTRL_HFBCE_MEM_CTRL_1_END (19)
#define SOC_MDC_AFBCE_MEM_CTRL_HFBCE_MEM_CTRL_2_START (20)
#define SOC_MDC_AFBCE_MEM_CTRL_HFBCE_MEM_CTRL_2_END (23)
#define SOC_MDC_AFBCE_MEM_CTRL_HFBCE_MEM_CTRL_3_START (24)
#define SOC_MDC_AFBCE_MEM_CTRL_HFBCE_MEM_CTRL_3_END (27)
#define SOC_MDC_AFBCE_MEM_CTRL_HFBCE_MEM_CTRL_4_START (28)
#define SOC_MDC_AFBCE_MEM_CTRL_HFBCE_MEM_CTRL_4_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int afbce_qos_cfg : 3;
        unsigned int reserved : 29;
    } reg;
} soc_mdc_afbce_qos_cfg_union;
#endif
#define SOC_MDC_AFBCE_QOS_CFG_AFBCE_QOS_CFG_START (0)
#define SOC_MDC_AFBCE_QOS_CFG_AFBCE_QOS_CFG_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int afbce_threshold : 3;
        unsigned int hfbce_threshold : 3;
        unsigned int hebce_threshold : 3;
        unsigned int reserved : 23;
    } reg;
} soc_mdc_fbce_threshold_union;
#endif
#define SOC_MDC_FBCE_THRESHOLD_AFBCE_THRESHOLD_START (0)
#define SOC_MDC_FBCE_THRESHOLD_AFBCE_THRESHOLD_END (2)
#define SOC_MDC_FBCE_THRESHOLD_HFBCE_THRESHOLD_START (3)
#define SOC_MDC_FBCE_THRESHOLD_HFBCE_THRESHOLD_END (5)
#define SOC_MDC_FBCE_THRESHOLD_HEBCE_THRESHOLD_START (6)
#define SOC_MDC_FBCE_THRESHOLD_HEBCE_THRESHOLD_END (8)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int afbce_scramble_mode : 2;
        unsigned int hfbce_scramble_mode : 4;
        unsigned int hebce_scramble_mode : 4;
        unsigned int reserved : 22;
    } reg;
} soc_mdc_fbce_scramble_mode_union;
#endif
#define SOC_MDC_FBCE_SCRAMBLE_MODE_AFBCE_SCRAMBLE_MODE_START (0)
#define SOC_MDC_FBCE_SCRAMBLE_MODE_AFBCE_SCRAMBLE_MODE_END (1)
#define SOC_MDC_FBCE_SCRAMBLE_MODE_HFBCE_SCRAMBLE_MODE_START (2)
#define SOC_MDC_FBCE_SCRAMBLE_MODE_HFBCE_SCRAMBLE_MODE_END (5)
#define SOC_MDC_FBCE_SCRAMBLE_MODE_HEBCE_SCRAMBLE_MODE_START (6)
#define SOC_MDC_FBCE_SCRAMBLE_MODE_HEBCE_SCRAMBLE_MODE_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int afbce_header_pointer_offset : 32;
    } reg;
} soc_mdc_afbce_header_pointer_offset_union;
#endif
#define SOC_MDC_AFBCE_HEADER_POINTER_OFFSET_AFBCE_HEADER_POINTER_OFFSET_START (0)
#define SOC_MDC_AFBCE_HEADER_POINTER_OFFSET_AFBCE_HEADER_POINTER_OFFSET_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int afbce_monitor_reg1 : 32;
    } reg;
} soc_mdc_afbce_monitor_reg1_union;
#endif
#define SOC_MDC_AFBCE_MONITOR_REG1_AFBCE_MONITOR_REG1_START (0)
#define SOC_MDC_AFBCE_MONITOR_REG1_AFBCE_MONITOR_REG1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int afbce_monitor_reg2 : 32;
    } reg;
} soc_mdc_afbce_monitor_reg2_union;
#endif
#define SOC_MDC_AFBCE_MONITOR_REG2_AFBCE_MONITOR_REG2_START (0)
#define SOC_MDC_AFBCE_MONITOR_REG2_AFBCE_MONITOR_REG2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int afbce_monitor_reg3 : 32;
    } reg;
} soc_mdc_afbce_monitor_reg3_union;
#endif
#define SOC_MDC_AFBCE_MONITOR_REG3_AFBCE_MONITOR_REG3_START (0)
#define SOC_MDC_AFBCE_MONITOR_REG3_AFBCE_MONITOR_REG3_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int afbce_debug_reg0 : 32;
    } reg;
} soc_mdc_afbce_debug_reg0_union;
#endif
#define SOC_MDC_AFBCE_DEBUG_REG0_AFBCE_DEBUG_REG0_START (0)
#define SOC_MDC_AFBCE_DEBUG_REG0_AFBCE_DEBUG_REG0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int fbce_creg_fbce_mode : 1;
        unsigned int fbce_creg_hfbce_bypass : 1;
        unsigned int reserved_0 : 2;
        unsigned int fbce_creg_hfbce_raw_en : 1;
        unsigned int hebce_enable : 1;
        unsigned int reserved_1 : 26;
    } reg;
} soc_mdc_fbce_creg_fbce_ctrl_mode_union;
#endif
#define SOC_MDC_FBCE_CREG_FBCE_CTRL_MODE_FBCE_CREG_FBCE_MODE_START (0)
#define SOC_MDC_FBCE_CREG_FBCE_CTRL_MODE_FBCE_CREG_FBCE_MODE_END (0)
#define SOC_MDC_FBCE_CREG_FBCE_CTRL_MODE_FBCE_CREG_HFBCE_BYPASS_START (1)
#define SOC_MDC_FBCE_CREG_FBCE_CTRL_MODE_FBCE_CREG_HFBCE_BYPASS_END (1)
#define SOC_MDC_FBCE_CREG_FBCE_CTRL_MODE_FBCE_CREG_HFBCE_RAW_EN_START (4)
#define SOC_MDC_FBCE_CREG_FBCE_CTRL_MODE_FBCE_CREG_HFBCE_RAW_EN_END (4)
#define SOC_MDC_FBCE_CREG_FBCE_CTRL_MODE_HEBCE_ENABLE_START (5)
#define SOC_MDC_FBCE_CREG_FBCE_CTRL_MODE_HEBCE_ENABLE_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int afbce_hdr_ptr_1 : 32;
    } reg;
} soc_mdc_afbce_hreg_hdr_ptr_l1_union;
#endif
#define SOC_MDC_AFBCE_HREG_HDR_PTR_L1_AFBCE_HDR_PTR_1_START (0)
#define SOC_MDC_AFBCE_HREG_HDR_PTR_L1_AFBCE_HDR_PTR_1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int afbce_payload_ptr_1 : 32;
    } reg;
} soc_mdc_afbce_hreg_pld_ptr_l1_union;
#endif
#define SOC_MDC_AFBCE_HREG_PLD_PTR_L1_AFBCE_PAYLOAD_PTR_1_START (0)
#define SOC_MDC_AFBCE_HREG_PLD_PTR_L1_AFBCE_PAYLOAD_PTR_1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int afbce_header_stride_1 : 14;
        unsigned int reserved : 18;
    } reg;
} soc_mdc_fbce_header_srtide_1_union;
#endif
#define SOC_MDC_HEADER_SRTIDE_1_AFBCE_HEADER_STRIDE_1_START (0)
#define SOC_MDC_HEADER_SRTIDE_1_AFBCE_HEADER_STRIDE_1_END (13)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int afbce_payload_stride_1 : 20;
        unsigned int reserved : 12;
    } reg;
} soc_mdc_fbce_payload_stride_1_union;
#endif
#define SOC_MDC_PAYLOAD_STRIDE_1_AFBCE_PAYLOAD_STRIDE_1_START (0)
#define SOC_MDC_PAYLOAD_STRIDE_1_AFBCE_PAYLOAD_STRIDE_1_END (19)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int fbce_creg_clkgate_off : 1;
        unsigned int fbce_creg_mem_sd_off : 1;
        unsigned int fbce_creg_hfbce_ip_auto_clk_gt_en : 1;
        unsigned int fbce_creg_fbce_auto_clk_gt_en : 1;
        unsigned int hebce_clkgate_off : 1;
        unsigned int hebce_mem_sd_off : 1;
        unsigned int reserved : 26;
    } reg;
} soc_mdc_fbce_creg_fbce_clk_gate_union;
#endif
#define SOC_MDC_FBCE_CREG_FBCE_CLK_GATE_FBCE_CREG_CLKGATE_OFF_START (0)
#define SOC_MDC_FBCE_CREG_FBCE_CLK_GATE_FBCE_CREG_CLKGATE_OFF_END (0)
#define SOC_MDC_FBCE_CREG_FBCE_CLK_GATE_FBCE_CREG_MEM_SD_OFF_START (1)
#define SOC_MDC_FBCE_CREG_FBCE_CLK_GATE_FBCE_CREG_MEM_SD_OFF_END (1)
#define SOC_MDC_FBCE_CREG_FBCE_CLK_GATE_FBCE_CREG_HFBCE_IP_AUTO_CLK_GT_EN_START (2)
#define SOC_MDC_FBCE_CREG_FBCE_CLK_GATE_FBCE_CREG_HFBCE_IP_AUTO_CLK_GT_EN_END (2)
#define SOC_MDC_FBCE_CREG_FBCE_CLK_GATE_FBCE_CREG_FBCE_AUTO_CLK_GT_EN_START (3)
#define SOC_MDC_FBCE_CREG_FBCE_CLK_GATE_FBCE_CREG_FBCE_AUTO_CLK_GT_EN_END (3)
#define SOC_MDC_FBCE_CREG_FBCE_CLK_GATE_HEBCE_CLKGATE_OFF_START (4)
#define SOC_MDC_FBCE_CREG_FBCE_CLK_GATE_HEBCE_CLKGATE_OFF_END (4)
#define SOC_MDC_FBCE_CREG_FBCE_CLK_GATE_HEBCE_MEM_SD_OFF_START (5)
#define SOC_MDC_FBCE_CREG_FBCE_CLK_GATE_HEBCE_MEM_SD_OFF_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hfbce_mem_ctrl_5 : 4;
        unsigned int hfbce_mem_ctrl_6 : 4;
        unsigned int reserved : 24;
    } reg;
} soc_mdc_afbce_mem_ctrl_1_union;
#endif
#define SOC_MDC_AFBCE_MEM_CTRL_1_HFBCE_MEM_CTRL_5_START (0)
#define SOC_MDC_AFBCE_MEM_CTRL_1_HFBCE_MEM_CTRL_5_END (3)
#define SOC_MDC_AFBCE_MEM_CTRL_1_HFBCE_MEM_CTRL_6_START (4)
#define SOC_MDC_AFBCE_MEM_CTRL_1_HFBCE_MEM_CTRL_6_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int afbce_monitor_reg4 : 32;
    } reg;
} soc_mdc_afbce_monitor_reg4_union;
#endif
#define SOC_MDC_AFBCE_MONITOR_REG4_AFBCE_MONITOR_REG4_START (0)
#define SOC_MDC_AFBCE_MONITOR_REG4_AFBCE_MONITOR_REG4_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int afbce_monitor_reg5 : 32;
    } reg;
} soc_mdc_afbce_monitor_reg5_union;
#endif
#define SOC_MDC_AFBCE_MONITOR_REG5_AFBCE_MONITOR_REG5_START (0)
#define SOC_MDC_AFBCE_MONITOR_REG5_AFBCE_MONITOR_REG5_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int afbce_monitor_reg6 : 32;
    } reg;
} soc_mdc_afbce_monitor_reg6_union;
#endif
#define SOC_MDC_AFBCE_MONITOR_REG6_AFBCE_MONITOR_REG6_START (0)
#define SOC_MDC_AFBCE_MONITOR_REG6_AFBCE_MONITOR_REG6_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hebce_spblk_layout : 2;
        unsigned int reserved_0 : 6;
        unsigned int hebce_spblk_compact_num : 5;
        unsigned int reserved_1 : 3;
        unsigned int hebce_spblk_layout_switch_en : 1;
        unsigned int reserved_2 : 15;
    } reg;
} soc_mdc_hebce_spblock_mode_union;
#endif
#define SOC_MDC_HEBCE_SPBLOCK_MODE_HEBCE_SPBLK_LAYOUT_START (0)
#define SOC_MDC_HEBCE_SPBLOCK_MODE_HEBCE_SPBLK_LAYOUT_END (1)
#define SOC_MDC_HEBCE_SPBLOCK_MODE_HEBCE_SPBLK_COMPACT_NUM_START (8)
#define SOC_MDC_HEBCE_SPBLOCK_MODE_HEBCE_SPBLK_COMPACT_NUM_END (12)
#define SOC_MDC_HEBCE_SPBLOCK_MODE_HEBCE_SPBLK_LAYOUT_SWITCH_EN_START (16)
#define SOC_MDC_HEBCE_SPBLOCK_MODE_HEBCE_SPBLK_LAYOUT_SWITCH_EN_END (16)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hebce_tag_value0 : 32;
    } reg;
} soc_mdc_hebce_tag_value0_union;
#endif
#define SOC_MDC_HEBCE_TAG_VALUE0_HEBCE_TAG_VALUE0_START (0)
#define SOC_MDC_HEBCE_TAG_VALUE0_HEBCE_TAG_VALUE0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hebce_tag_value1 : 32;
    } reg;
} soc_mdc_hebce_tag_value1_union;
#endif
#define SOC_MDC_HEBCE_TAG_VALUE1_HEBCE_TAG_VALUE1_START (0)
#define SOC_MDC_HEBCE_TAG_VALUE1_HEBCE_TAG_VALUE1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hebce_tag_value2 : 32;
    } reg;
} soc_mdc_hebce_tag_value2_union;
#endif
#define SOC_MDC_HEBCE_TAG_VALUE2_HEBCE_TAG_VALUE2_START (0)
#define SOC_MDC_HEBCE_TAG_VALUE2_HEBCE_TAG_VALUE2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hebce_tag_value3 : 32;
    } reg;
} soc_mdc_hebce_tag_value3_union;
#endif
#define SOC_MDC_HEBCE_TAG_VALUE3_HEBCE_TAG_VALUE3_START (0)
#define SOC_MDC_HEBCE_TAG_VALUE3_HEBCE_TAG_VALUE3_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hebce_reserved0 : 32;
    } reg;
} soc_mdc_hebce_reserved0_union;
#endif
#define SOC_MDC_HEBCE_RESERVED0_HEBCE_RESERVED0_START (0)
#define SOC_MDC_HEBCE_RESERVED0_HEBCE_RESERVED0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hebce_reserved1 : 32;
    } reg;
} soc_mdc_hebce_reserved1_union;
#endif
#define SOC_MDC_HEBCE_RESERVED1_HEBCE_RESERVED1_START (0)
#define SOC_MDC_HEBCE_RESERVED1_HEBCE_RESERVED1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hebce_reserved2 : 32;
    } reg;
} soc_mdc_hebce_reserved2_union;
#endif
#define SOC_MDC_HEBCE_RESERVED2_HEBCE_RESERVED2_START (0)
#define SOC_MDC_HEBCE_RESERVED2_HEBCE_RESERVED2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hebce_reserved3 : 32;
    } reg;
} soc_mdc_hebce_reserved3_union;
#endif
#define SOC_MDC_HEBCE_RESERVED3_HEBCE_RESERVED3_START (0)
#define SOC_MDC_HEBCE_RESERVED3_HEBCE_RESERVED3_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hebce_monitor_reg0 : 32;
    } reg;
} soc_mdc_hebce_monitor_reg0_union;
#endif
#define SOC_MDC_HEBCE_MONITOR_REG0_HEBCE_MONITOR_REG0_START (0)
#define SOC_MDC_HEBCE_MONITOR_REG0_HEBCE_MONITOR_REG0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hebce_monitor_reg1 : 32;
    } reg;
} soc_mdc_hebce_monitor_reg1_union;
#endif
#define SOC_MDC_HEBCE_MONITOR_REG1_HEBCE_MONITOR_REG1_START (0)
#define SOC_MDC_HEBCE_MONITOR_REG1_HEBCE_MONITOR_REG1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hebce_monitor_reg2 : 32;
    } reg;
} soc_mdc_hebce_monitor_reg2_union;
#endif
#define SOC_MDC_HEBCE_MONITOR_REG2_HEBCE_MONITOR_REG2_START (0)
#define SOC_MDC_HEBCE_MONITOR_REG2_HEBCE_MONITOR_REG2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hebce_monitor_reg3 : 32;
    } reg;
} soc_mdc_hebce_monitor_reg3_union;
#endif
#define SOC_MDC_HEBCE_MONITOR_REG3_HEBCE_MONITOR_REG3_START (0)
#define SOC_MDC_HEBCE_MONITOR_REG3_HEBCE_MONITOR_REG3_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hebce_mem_ctrl : 4;
        unsigned int reserved : 28;
    } reg;
} soc_mdc_hebce_mem_ctrl_union;
#endif
#define SOC_MDC_HEBCE_MEM_CTRL_HEBCE_MEM_CTRL_START (0)
#define SOC_MDC_HEBCE_MEM_CTRL_HEBCE_MEM_CTRL_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int scf_video_6tap_coef : 32;
    } reg;
} soc_mdc_wch_scf_video_6tap_coef_union;
#endif
#define SOC_MDC_WCH_SCF_VIDEO_6TAP_COEF_SCF_VIDEO_6TAP_COEF_START (0)
#define SOC_MDC_WCH_SCF_VIDEO_6TAP_COEF_SCF_VIDEO_6TAP_COEF_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int scf_video_5tap_coef : 32;
    } reg;
} soc_mdc_wch_scf_video_5tap_coef_union;
#endif
#define SOC_MDC_WCH_SCF_VIDEO_5TAP_COEF_SCF_VIDEO_5TAP_COEF_START (0)
#define SOC_MDC_WCH_SCF_VIDEO_5TAP_COEF_SCF_VIDEO_5TAP_COEF_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int scf_video_4tap_coef : 32;
    } reg;
} soc_mdc_wch_scf_video_4tap_coef_union;
#endif
#define SOC_MDC_WCH_SCF_VIDEO_4TAP_COEF_SCF_VIDEO_4TAP_COEF_START (0)
#define SOC_MDC_WCH_SCF_VIDEO_4TAP_COEF_SCF_VIDEO_4TAP_COEF_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gmp_en : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_gmp_en_union;
#endif
#define SOC_MDC_GMP_EN_GMP_EN_START (0)
#define SOC_MDC_GMP_EN_GMP_EN_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gmp_mem_ctrl : 4;
        unsigned int reserved : 28;
    } reg;
} soc_mdc_gmp_mem_ctrl_union;
#endif
#define SOC_MDC_GMP_MEM_CTRL_GMP_MEM_CTRL_START (0)
#define SOC_MDC_GMP_MEM_CTRL_GMP_MEM_CTRL_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gmp_lut_sel : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_gmp_lut_sel_union;
#endif
#define SOC_MDC_GMP_LUT_SEL_GMP_LUT_SEL_START (0)
#define SOC_MDC_GMP_LUT_SEL_GMP_LUT_SEL_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gmp_dbg_w0 : 32;
    } reg;
} soc_mdc_gmp_dbg_w0_union;
#endif
#define SOC_MDC_GMP_DBG_W0_GMP_DBG_W0_START (0)
#define SOC_MDC_GMP_DBG_W0_GMP_DBG_W0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gmp_dbg_r0 : 32;
    } reg;
} soc_mdc_gmp_dbg_r0_union;
#endif
#define SOC_MDC_GMP_DBG_R0_GMP_DBG_R0_START (0)
#define SOC_MDC_GMP_DBG_R0_GMP_DBG_R0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gmp_dbg_r1 : 32;
    } reg;
} soc_mdc_gmp_dbg_r1_union;
#endif
#define SOC_MDC_GMP_DBG_R1_GMP_DBG_R1_START (0)
#define SOC_MDC_GMP_DBG_R1_GMP_DBG_R1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gmp_dbg_r2 : 32;
    } reg;
} soc_mdc_gmp_dbg_r2_union;
#endif
#define SOC_MDC_GMP_DBG_R2_GMP_DBG_R2_START (0)
#define SOC_MDC_GMP_DBG_R2_GMP_DBG_R2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gmp_dbg_r3 : 32;
    } reg;
} soc_mdc_gmp_dbg_r3_union;
#endif
#define SOC_MDC_GMP_DBG_R3_GMP_DBG_R3_START (0)
#define SOC_MDC_GMP_DBG_R3_GMP_DBG_R3_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int u_mdc_gmp_coef : 32;
    } reg;
} soc_mdc_u_mdc_gmp_coef_union;
#endif
#define SOC_MDC_U_MDC_GMP_COEF_U_MDC_GMP_COEF_START (0)
#define SOC_MDC_U_MDC_GMP_COEF_U_MDC_GMP_COEF_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int img_width : 15;
        unsigned int reserved_0 : 1;
        unsigned int img_height : 15;
        unsigned int reserved_1 : 1;
    } reg;
} soc_mdc_ov_size_union;
#endif
#define SOC_MDC_OV_SIZE_IMG_WIDTH_START (0)
#define SOC_MDC_OV_SIZE_IMG_WIDTH_END (14)
#define SOC_MDC_OV_SIZE_IMG_HEIGHT_START (16)
#define SOC_MDC_OV_SIZE_IMG_HEIGHT_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bg_color_rgb : 30;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_ov_bg_color_rgb_union;
#endif
#define SOC_MDC_OV_BG_COLOR_RGB_BG_COLOR_RGB_START (0)
#define SOC_MDC_OV_BG_COLOR_RGB_BG_COLOR_RGB_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bg_color_a : 10;
        unsigned int reserved : 22;
    } reg;
} soc_mdc_ov_bg_color_a_union;
#endif
#define SOC_MDC_OV_BG_COLOR_A_BG_COLOR_A_START (0)
#define SOC_MDC_OV_BG_COLOR_A_BG_COLOR_A_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int blayer_startx : 15;
        unsigned int reserved_0 : 1;
        unsigned int blayer_starty : 15;
        unsigned int reserved_1 : 1;
    } reg;
} soc_mdc_dst_startpos_union;
#endif
#define SOC_MDC_DST_STARTPOS_BLAYER_STARTX_START (0)
#define SOC_MDC_DST_STARTPOS_BLAYER_STARTX_END (14)
#define SOC_MDC_DST_STARTPOS_BLAYER_STARTY_START (16)
#define SOC_MDC_DST_STARTPOS_BLAYER_STARTY_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int blayer_endx : 15;
        unsigned int reserved_0 : 1;
        unsigned int blayer_endy : 15;
        unsigned int reserved_1 : 1;
    } reg;
} soc_mdc_dst_endpos_union;
#endif
#define SOC_MDC_DST_ENDPOS_BLAYER_ENDX_START (0)
#define SOC_MDC_DST_ENDPOS_BLAYER_ENDX_END (14)
#define SOC_MDC_DST_ENDPOS_BLAYER_ENDY_START (16)
#define SOC_MDC_DST_ENDPOS_BLAYER_ENDY_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int overlay_enable : 1;
        unsigned int reserved_0 : 7;
        unsigned int reserved_1 : 3;
        unsigned int reserved_2 : 3;
        unsigned int reserved_3 : 2;
        unsigned int blayer_bgcolor_en : 1;
        unsigned int reserved_4 : 3;
        unsigned int p3fmt : 2;
        unsigned int reserved_5 : 2;
        unsigned int p3fmt_enable : 1;
        unsigned int reserved_6 : 7;
    } reg;
} soc_mdc_ov_gcfg_union;
#endif
#define SOC_MDC_OV_GCFG_OVERLAY_ENABLE_START (0)
#define SOC_MDC_OV_GCFG_OVERLAY_ENABLE_END (0)
#define SOC_MDC_OV_GCFG_BLAYER_BGCOLOR_EN_START (16)
#define SOC_MDC_OV_GCFG_BLAYER_BGCOLOR_EN_END (16)
#define SOC_MDC_OV_GCFG_P3FMT_START (20)
#define SOC_MDC_OV_GCFG_P3FMT_END (21)
#define SOC_MDC_OV_GCFG_P3FMT_ENABLE_START (24)
#define SOC_MDC_OV_GCFG_P3FMT_ENABLE_END (24)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer0_startx : 15;
        unsigned int reserved_0 : 1;
        unsigned int layer0_starty : 15;
        unsigned int reserved_1 : 1;
    } reg;
} soc_mdc_layer0_pos_union;
#endif
#define SOC_MDC_LAYER0_POS_LAYER0_STARTX_START (0)
#define SOC_MDC_LAYER0_POS_LAYER0_STARTX_END (14)
#define SOC_MDC_LAYER0_POS_LAYER0_STARTY_START (16)
#define SOC_MDC_LAYER0_POS_LAYER0_STARTY_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer0_endx : 15;
        unsigned int reserved_0 : 1;
        unsigned int layer0_endy : 15;
        unsigned int reserved_1 : 1;
    } reg;
} soc_mdc_layer0_size_union;
#endif
#define SOC_MDC_LAYER0_SIZE_LAYER0_ENDX_START (0)
#define SOC_MDC_LAYER0_SIZE_LAYER0_ENDX_END (14)
#define SOC_MDC_LAYER0_SIZE_LAYER0_ENDY_START (16)
#define SOC_MDC_LAYER0_SIZE_LAYER0_ENDY_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer0_src_locolorkey : 30;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_layer0_srclokey_union;
#endif
#define SOC_MDC_LAYER0_SRCLOKEY_LAYER0_SRC_LOCOLORKEY_START (0)
#define SOC_MDC_LAYER0_SRCLOKEY_LAYER0_SRC_LOCOLORKEY_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer0_src_hicolorkey : 30;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_layer0_srchikey_union;
#endif
#define SOC_MDC_LAYER0_SRCHIKEY_LAYER0_SRC_HICOLORKEY_START (0)
#define SOC_MDC_LAYER0_SRCHIKEY_LAYER0_SRC_HICOLORKEY_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer0_dst_locolorkey : 30;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_layer0_dstlokey_union;
#endif
#define SOC_MDC_LAYER0_DSTLOKEY_LAYER0_DST_LOCOLORKEY_START (0)
#define SOC_MDC_LAYER0_DSTLOKEY_LAYER0_DST_LOCOLORKEY_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer0_dst_hicolorkey : 30;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_layer0_dsthikey_union;
#endif
#define SOC_MDC_LAYER0_DSTHIKEY_LAYER0_DST_HICOLORKEY_START (0)
#define SOC_MDC_LAYER0_DSTHIKEY_LAYER0_DST_HICOLORKEY_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer0_pattern_rgb : 30;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_layer0_pattern_rgb_union;
#endif
#define SOC_MDC_LAYER0_PATTERN_RGB_LAYER0_PATTERN_RGB_START (0)
#define SOC_MDC_LAYER0_PATTERN_RGB_LAYER0_PATTERN_RGB_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer0_pattern_a : 10;
        unsigned int reserved : 22;
    } reg;
} soc_mdc_layer0_pattern_a_union;
#endif
#define SOC_MDC_LAYER0_PATTERN_A_LAYER0_PATTERN_A_START (0)
#define SOC_MDC_LAYER0_PATTERN_A_LAYER0_PATTERN_A_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 8;
        unsigned int layer0_fix_mode : 1;
        unsigned int layer0_dst_pmode : 1;
        unsigned int layer0_alpha_offdst : 2;
        unsigned int layer0_dst_gmode : 2;
        unsigned int layer0_dst_amode : 2;
        unsigned int reserved_1 : 7;
        unsigned int layer0_src_mmode : 1;
        unsigned int layer0_alpha_smode : 1;
        unsigned int layer0_src_pmode : 1;
        unsigned int layer0_src_lmode : 1;
        unsigned int layer0_alpha_offsrc : 1;
        unsigned int layer0_src_gmode : 2;
        unsigned int layer0_src_amode : 2;
    } reg;
} soc_mdc_layer0_alpha_mode_union;
#endif
#define SOC_MDC_LAYER0_ALPHA_MODE_LAYER0_FIX_MODE_START (8)
#define SOC_MDC_LAYER0_ALPHA_MODE_LAYER0_FIX_MODE_END (8)
#define SOC_MDC_LAYER0_ALPHA_MODE_LAYER0_DST_PMODE_START (9)
#define SOC_MDC_LAYER0_ALPHA_MODE_LAYER0_DST_PMODE_END (9)
#define SOC_MDC_LAYER0_ALPHA_MODE_LAYER0_ALPHA_OFFDST_START (10)
#define SOC_MDC_LAYER0_ALPHA_MODE_LAYER0_ALPHA_OFFDST_END (11)
#define SOC_MDC_LAYER0_ALPHA_MODE_LAYER0_DST_GMODE_START (12)
#define SOC_MDC_LAYER0_ALPHA_MODE_LAYER0_DST_GMODE_END (13)
#define SOC_MDC_LAYER0_ALPHA_MODE_LAYER0_DST_AMODE_START (14)
#define SOC_MDC_LAYER0_ALPHA_MODE_LAYER0_DST_AMODE_END (15)
#define SOC_MDC_LAYER0_ALPHA_MODE_LAYER0_SRC_MMODE_START (23)
#define SOC_MDC_LAYER0_ALPHA_MODE_LAYER0_SRC_MMODE_END (23)
#define SOC_MDC_LAYER0_ALPHA_MODE_LAYER0_ALPHA_SMODE_START (24)
#define SOC_MDC_LAYER0_ALPHA_MODE_LAYER0_ALPHA_SMODE_END (24)
#define SOC_MDC_LAYER0_ALPHA_MODE_LAYER0_SRC_PMODE_START (25)
#define SOC_MDC_LAYER0_ALPHA_MODE_LAYER0_SRC_PMODE_END (25)
#define SOC_MDC_LAYER0_ALPHA_MODE_LAYER0_SRC_LMODE_START (26)
#define SOC_MDC_LAYER0_ALPHA_MODE_LAYER0_SRC_LMODE_END (26)
#define SOC_MDC_LAYER0_ALPHA_MODE_LAYER0_ALPHA_OFFSRC_START (27)
#define SOC_MDC_LAYER0_ALPHA_MODE_LAYER0_ALPHA_OFFSRC_END (27)
#define SOC_MDC_LAYER0_ALPHA_MODE_LAYER0_SRC_GMODE_START (28)
#define SOC_MDC_LAYER0_ALPHA_MODE_LAYER0_SRC_GMODE_END (29)
#define SOC_MDC_LAYER0_ALPHA_MODE_LAYER0_SRC_AMODE_START (30)
#define SOC_MDC_LAYER0_ALPHA_MODE_LAYER0_SRC_AMODE_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer0_dst_global_alpha : 10;
        unsigned int reserved_0 : 6;
        unsigned int layer0_src_global_alpha : 10;
        unsigned int reserved_1 : 6;
    } reg;
} soc_mdc_layer0_alpha_a_union;
#endif
#define SOC_MDC_LAYER0_ALPHA_A_LAYER0_DST_GLOBAL_ALPHA_START (0)
#define SOC_MDC_LAYER0_ALPHA_A_LAYER0_DST_GLOBAL_ALPHA_END (9)
#define SOC_MDC_LAYER0_ALPHA_A_LAYER0_SRC_GLOBAL_ALPHA_START (16)
#define SOC_MDC_LAYER0_ALPHA_A_LAYER0_SRC_GLOBAL_ALPHA_END (25)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer0_enable : 1;
        unsigned int reserved_0 : 3;
        unsigned int layer0_trop_en : 1;
        unsigned int layer0_rop_en : 1;
        unsigned int reserved_1 : 2;
        unsigned int layer0_src_cfg : 1;
        unsigned int reserved_2 : 3;
        unsigned int reserved_3 : 3;
        unsigned int reserved_4 : 1;
        unsigned int layer0_auto_nosrc : 1;
        unsigned int reserved_5 : 3;
        unsigned int layer0_trop_code : 4;
        unsigned int layer0_rop_code : 8;
    } reg;
} soc_mdc_layer0_cfg_union;
#endif
#define SOC_MDC_LAYER0_CFG_LAYER0_ENABLE_START (0)
#define SOC_MDC_LAYER0_CFG_LAYER0_ENABLE_END (0)
#define SOC_MDC_LAYER0_CFG_LAYER0_TROP_EN_START (4)
#define SOC_MDC_LAYER0_CFG_LAYER0_TROP_EN_END (4)
#define SOC_MDC_LAYER0_CFG_LAYER0_ROP_EN_START (5)
#define SOC_MDC_LAYER0_CFG_LAYER0_ROP_EN_END (5)
#define SOC_MDC_LAYER0_CFG_LAYER0_SRC_CFG_START (8)
#define SOC_MDC_LAYER0_CFG_LAYER0_SRC_CFG_END (8)
#define SOC_MDC_LAYER0_CFG_LAYER0_AUTO_NOSRC_START (16)
#define SOC_MDC_LAYER0_CFG_LAYER0_AUTO_NOSRC_END (16)
#define SOC_MDC_LAYER0_CFG_LAYER0_TROP_CODE_START (20)
#define SOC_MDC_LAYER0_CFG_LAYER0_TROP_CODE_END (23)
#define SOC_MDC_LAYER0_CFG_LAYER0_ROP_CODE_START (24)
#define SOC_MDC_LAYER0_CFG_LAYER0_ROP_CODE_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer0_pstartx : 15;
        unsigned int reserved_0 : 1;
        unsigned int layer0_pstarty : 15;
        unsigned int reserved_1 : 1;
    } reg;
} soc_mdc_layer0_pspos_union;
#endif
#define SOC_MDC_LAYER0_PSPOS_LAYER0_PSTARTX_START (0)
#define SOC_MDC_LAYER0_PSPOS_LAYER0_PSTARTX_END (14)
#define SOC_MDC_LAYER0_PSPOS_LAYER0_PSTARTY_START (16)
#define SOC_MDC_LAYER0_PSPOS_LAYER0_PSTARTY_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer0_pendx : 15;
        unsigned int reserved_0 : 1;
        unsigned int layer0_pendy : 15;
        unsigned int reserved_1 : 1;
    } reg;
} soc_mdc_layer0_pepos_union;
#endif
#define SOC_MDC_LAYER0_PEPOS_LAYER0_PENDX_START (0)
#define SOC_MDC_LAYER0_PEPOS_LAYER0_PENDX_END (14)
#define SOC_MDC_LAYER0_PEPOS_LAYER0_PENDY_START (16)
#define SOC_MDC_LAYER0_PEPOS_LAYER0_PENDY_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer0_frame_end : 1;
        unsigned int layer0_nosrc : 1;
        unsigned int layer0_dstalpha_flag : 1;
        unsigned int layer0_srcalpha_flag : 1;
        unsigned int reserved_0 : 4;
        unsigned int layer0_dstalpha_info : 10;
        unsigned int layer0_srcalpha_info : 10;
        unsigned int reserved_1 : 4;
    } reg;
} soc_mdc_layer0_info_alpha_union;
#endif
#define SOC_MDC_LAYER0_INFO_ALPHA_LAYER0_FRAME_END_START (0)
#define SOC_MDC_LAYER0_INFO_ALPHA_LAYER0_FRAME_END_END (0)
#define SOC_MDC_LAYER0_INFO_ALPHA_LAYER0_NOSRC_START (1)
#define SOC_MDC_LAYER0_INFO_ALPHA_LAYER0_NOSRC_END (1)
#define SOC_MDC_LAYER0_INFO_ALPHA_LAYER0_DSTALPHA_FLAG_START (2)
#define SOC_MDC_LAYER0_INFO_ALPHA_LAYER0_DSTALPHA_FLAG_END (2)
#define SOC_MDC_LAYER0_INFO_ALPHA_LAYER0_SRCALPHA_FLAG_START (3)
#define SOC_MDC_LAYER0_INFO_ALPHA_LAYER0_SRCALPHA_FLAG_END (3)
#define SOC_MDC_LAYER0_INFO_ALPHA_LAYER0_DSTALPHA_INFO_START (8)
#define SOC_MDC_LAYER0_INFO_ALPHA_LAYER0_DSTALPHA_INFO_END (17)
#define SOC_MDC_LAYER0_INFO_ALPHA_LAYER0_SRCALPHA_INFO_START (18)
#define SOC_MDC_LAYER0_INFO_ALPHA_LAYER0_SRCALPHA_INFO_END (27)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer0_srccolor_flag : 1;
        unsigned int reserved : 1;
        unsigned int layer0_srccolor_info : 30;
    } reg;
} soc_mdc_layer0_info_srccolor_union;
#endif
#define SOC_MDC_LAYER0_INFO_SRCCOLOR_LAYER0_SRCCOLOR_FLAG_START (0)
#define SOC_MDC_LAYER0_INFO_SRCCOLOR_LAYER0_SRCCOLOR_FLAG_END (0)
#define SOC_MDC_LAYER0_INFO_SRCCOLOR_LAYER0_SRCCOLOR_INFO_START (2)
#define SOC_MDC_LAYER0_INFO_SRCCOLOR_LAYER0_SRCCOLOR_INFO_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer0_dbg_info : 32;
    } reg;
} soc_mdc_layer0_dbg_info_union;
#endif
#define SOC_MDC_LAYER0_DBG_INFO_LAYER0_DBG_INFO_START (0)
#define SOC_MDC_LAYER0_DBG_INFO_LAYER0_DBG_INFO_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer1_startx : 15;
        unsigned int reserved_0 : 1;
        unsigned int layer1_starty : 15;
        unsigned int reserved_1 : 1;
    } reg;
} soc_mdc_layer1_pos_union;
#endif
#define SOC_MDC_LAYER1_POS_LAYER1_STARTX_START (0)
#define SOC_MDC_LAYER1_POS_LAYER1_STARTX_END (14)
#define SOC_MDC_LAYER1_POS_LAYER1_STARTY_START (16)
#define SOC_MDC_LAYER1_POS_LAYER1_STARTY_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer1_endx : 15;
        unsigned int reserved_0 : 1;
        unsigned int layer1_endy : 15;
        unsigned int reserved_1 : 1;
    } reg;
} soc_mdc_layer1_size_union;
#endif
#define SOC_MDC_LAYER1_SIZE_LAYER1_ENDX_START (0)
#define SOC_MDC_LAYER1_SIZE_LAYER1_ENDX_END (14)
#define SOC_MDC_LAYER1_SIZE_LAYER1_ENDY_START (16)
#define SOC_MDC_LAYER1_SIZE_LAYER1_ENDY_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer1_src_locolorkey : 30;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_layer1_srclokey_union;
#endif
#define SOC_MDC_LAYER1_SRCLOKEY_LAYER1_SRC_LOCOLORKEY_START (0)
#define SOC_MDC_LAYER1_SRCLOKEY_LAYER1_SRC_LOCOLORKEY_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer1_src_hicolorkey : 30;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_layer1_srchikey_union;
#endif
#define SOC_MDC_LAYER1_SRCHIKEY_LAYER1_SRC_HICOLORKEY_START (0)
#define SOC_MDC_LAYER1_SRCHIKEY_LAYER1_SRC_HICOLORKEY_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer1_dst_locolorkey : 30;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_layer1_dstlokey_union;
#endif
#define SOC_MDC_LAYER1_DSTLOKEY_LAYER1_DST_LOCOLORKEY_START (0)
#define SOC_MDC_LAYER1_DSTLOKEY_LAYER1_DST_LOCOLORKEY_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer1_dst_hicolorkey : 30;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_layer1_dsthikey_union;
#endif
#define SOC_MDC_LAYER1_DSTHIKEY_LAYER1_DST_HICOLORKEY_START (0)
#define SOC_MDC_LAYER1_DSTHIKEY_LAYER1_DST_HICOLORKEY_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer1_pattern_rgb : 30;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_layer1_pattern_rgb_union;
#endif
#define SOC_MDC_LAYER1_PATTERN_RGB_LAYER1_PATTERN_RGB_START (0)
#define SOC_MDC_LAYER1_PATTERN_RGB_LAYER1_PATTERN_RGB_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer1_pattern_a : 10;
        unsigned int reserved : 22;
    } reg;
} soc_mdc_layer1_pattern_a_union;
#endif
#define SOC_MDC_LAYER1_PATTERN_A_LAYER1_PATTERN_A_START (0)
#define SOC_MDC_LAYER1_PATTERN_A_LAYER1_PATTERN_A_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 8;
        unsigned int layer1_fix_mode : 1;
        unsigned int layer1_dst_pmode : 1;
        unsigned int layer1_off_dst : 2;
        unsigned int layer1_dst_gmode : 2;
        unsigned int layer1_dst_amode : 2;
        unsigned int reserved_1 : 7;
        unsigned int layer1_src_mmode : 1;
        unsigned int layer1_alpha_smode : 1;
        unsigned int layer1_src_pmode : 1;
        unsigned int layer1_src_lmode : 1;
        unsigned int layer1_off_src : 1;
        unsigned int layer1_src_gmode : 2;
        unsigned int layer1_src_amode : 2;
    } reg;
} soc_mdc_layer1_alpha_mode_union;
#endif
#define SOC_MDC_LAYER1_ALPHA_MODE_LAYER1_FIX_MODE_START (8)
#define SOC_MDC_LAYER1_ALPHA_MODE_LAYER1_FIX_MODE_END (8)
#define SOC_MDC_LAYER1_ALPHA_MODE_LAYER1_DST_PMODE_START (9)
#define SOC_MDC_LAYER1_ALPHA_MODE_LAYER1_DST_PMODE_END (9)
#define SOC_MDC_LAYER1_ALPHA_MODE_LAYER1_OFF_DST_START (10)
#define SOC_MDC_LAYER1_ALPHA_MODE_LAYER1_OFF_DST_END (11)
#define SOC_MDC_LAYER1_ALPHA_MODE_LAYER1_DST_GMODE_START (12)
#define SOC_MDC_LAYER1_ALPHA_MODE_LAYER1_DST_GMODE_END (13)
#define SOC_MDC_LAYER1_ALPHA_MODE_LAYER1_DST_AMODE_START (14)
#define SOC_MDC_LAYER1_ALPHA_MODE_LAYER1_DST_AMODE_END (15)
#define SOC_MDC_LAYER1_ALPHA_MODE_LAYER1_SRC_MMODE_START (23)
#define SOC_MDC_LAYER1_ALPHA_MODE_LAYER1_SRC_MMODE_END (23)
#define SOC_MDC_LAYER1_ALPHA_MODE_LAYER1_ALPHA_SMODE_START (24)
#define SOC_MDC_LAYER1_ALPHA_MODE_LAYER1_ALPHA_SMODE_END (24)
#define SOC_MDC_LAYER1_ALPHA_MODE_LAYER1_SRC_PMODE_START (25)
#define SOC_MDC_LAYER1_ALPHA_MODE_LAYER1_SRC_PMODE_END (25)
#define SOC_MDC_LAYER1_ALPHA_MODE_LAYER1_SRC_LMODE_START (26)
#define SOC_MDC_LAYER1_ALPHA_MODE_LAYER1_SRC_LMODE_END (26)
#define SOC_MDC_LAYER1_ALPHA_MODE_LAYER1_OFF_SRC_START (27)
#define SOC_MDC_LAYER1_ALPHA_MODE_LAYER1_OFF_SRC_END (27)
#define SOC_MDC_LAYER1_ALPHA_MODE_LAYER1_SRC_GMODE_START (28)
#define SOC_MDC_LAYER1_ALPHA_MODE_LAYER1_SRC_GMODE_END (29)
#define SOC_MDC_LAYER1_ALPHA_MODE_LAYER1_SRC_AMODE_START (30)
#define SOC_MDC_LAYER1_ALPHA_MODE_LAYER1_SRC_AMODE_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer1_dst_global_alpha : 10;
        unsigned int reserved_0 : 6;
        unsigned int layer1_src_global_alpha : 10;
        unsigned int reserved_1 : 6;
    } reg;
} soc_mdc_layer1_alpha_a_union;
#endif
#define SOC_MDC_LAYER1_ALPHA_A_LAYER1_DST_GLOBAL_ALPHA_START (0)
#define SOC_MDC_LAYER1_ALPHA_A_LAYER1_DST_GLOBAL_ALPHA_END (9)
#define SOC_MDC_LAYER1_ALPHA_A_LAYER1_SRC_GLOBAL_ALPHA_START (16)
#define SOC_MDC_LAYER1_ALPHA_A_LAYER1_SRC_GLOBAL_ALPHA_END (25)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer1_enable : 1;
        unsigned int reserved_0 : 3;
        unsigned int layer1_trop_en : 1;
        unsigned int layer1_rop_en : 1;
        unsigned int reserved_1 : 2;
        unsigned int layer1_src_cfg : 1;
        unsigned int reserved_2 : 3;
        unsigned int reserved_3 : 3;
        unsigned int reserved_4 : 1;
        unsigned int layer1_auto_nosrc : 1;
        unsigned int reserved_5 : 3;
        unsigned int layer1_trop_code : 4;
        unsigned int layer1_rop_code : 8;
    } reg;
} soc_mdc_layer1_cfg_union;
#endif
#define SOC_MDC_LAYER1_CFG_LAYER1_ENABLE_START (0)
#define SOC_MDC_LAYER1_CFG_LAYER1_ENABLE_END (0)
#define SOC_MDC_LAYER1_CFG_LAYER1_TROP_EN_START (4)
#define SOC_MDC_LAYER1_CFG_LAYER1_TROP_EN_END (4)
#define SOC_MDC_LAYER1_CFG_LAYER1_ROP_EN_START (5)
#define SOC_MDC_LAYER1_CFG_LAYER1_ROP_EN_END (5)
#define SOC_MDC_LAYER1_CFG_LAYER1_SRC_CFG_START (8)
#define SOC_MDC_LAYER1_CFG_LAYER1_SRC_CFG_END (8)
#define SOC_MDC_LAYER1_CFG_LAYER1_AUTO_NOSRC_START (16)
#define SOC_MDC_LAYER1_CFG_LAYER1_AUTO_NOSRC_END (16)
#define SOC_MDC_LAYER1_CFG_LAYER1_TROP_CODE_START (20)
#define SOC_MDC_LAYER1_CFG_LAYER1_TROP_CODE_END (23)
#define SOC_MDC_LAYER1_CFG_LAYER1_ROP_CODE_START (24)
#define SOC_MDC_LAYER1_CFG_LAYER1_ROP_CODE_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer1_pstartx : 15;
        unsigned int reserved_0 : 1;
        unsigned int layer1_pstarty : 15;
        unsigned int reserved_1 : 1;
    } reg;
} soc_mdc_layer1_pspos_union;
#endif
#define SOC_MDC_LAYER1_PSPOS_LAYER1_PSTARTX_START (0)
#define SOC_MDC_LAYER1_PSPOS_LAYER1_PSTARTX_END (14)
#define SOC_MDC_LAYER1_PSPOS_LAYER1_PSTARTY_START (16)
#define SOC_MDC_LAYER1_PSPOS_LAYER1_PSTARTY_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer1_pendx : 15;
        unsigned int reserved_0 : 1;
        unsigned int layer1_pendy : 15;
        unsigned int reserved_1 : 1;
    } reg;
} soc_mdc_layer1_pepos_union;
#endif
#define SOC_MDC_LAYER1_PEPOS_LAYER1_PENDX_START (0)
#define SOC_MDC_LAYER1_PEPOS_LAYER1_PENDX_END (14)
#define SOC_MDC_LAYER1_PEPOS_LAYER1_PENDY_START (16)
#define SOC_MDC_LAYER1_PEPOS_LAYER1_PENDY_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer1_frame_end : 1;
        unsigned int layer1_nosrc : 1;
        unsigned int layer1_dstalpha_flag : 1;
        unsigned int layer1_srcalpha_flag : 1;
        unsigned int reserved_0 : 4;
        unsigned int layer1_dstalpha_info : 10;
        unsigned int layer1_srcalpha_info : 10;
        unsigned int reserved_1 : 4;
    } reg;
} soc_mdc_layer1_info_alpha_union;
#endif
#define SOC_MDC_LAYER1_INFO_ALPHA_LAYER1_FRAME_END_START (0)
#define SOC_MDC_LAYER1_INFO_ALPHA_LAYER1_FRAME_END_END (0)
#define SOC_MDC_LAYER1_INFO_ALPHA_LAYER1_NOSRC_START (1)
#define SOC_MDC_LAYER1_INFO_ALPHA_LAYER1_NOSRC_END (1)
#define SOC_MDC_LAYER1_INFO_ALPHA_LAYER1_DSTALPHA_FLAG_START (2)
#define SOC_MDC_LAYER1_INFO_ALPHA_LAYER1_DSTALPHA_FLAG_END (2)
#define SOC_MDC_LAYER1_INFO_ALPHA_LAYER1_SRCALPHA_FLAG_START (3)
#define SOC_MDC_LAYER1_INFO_ALPHA_LAYER1_SRCALPHA_FLAG_END (3)
#define SOC_MDC_LAYER1_INFO_ALPHA_LAYER1_DSTALPHA_INFO_START (8)
#define SOC_MDC_LAYER1_INFO_ALPHA_LAYER1_DSTALPHA_INFO_END (17)
#define SOC_MDC_LAYER1_INFO_ALPHA_LAYER1_SRCALPHA_INFO_START (18)
#define SOC_MDC_LAYER1_INFO_ALPHA_LAYER1_SRCALPHA_INFO_END (27)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer1_srccolor_flag : 1;
        unsigned int reserved : 1;
        unsigned int layer1_srccolor_info : 30;
    } reg;
} soc_mdc_layer1_info_srccolor_union;
#endif
#define SOC_MDC_LAYER1_INFO_SRCCOLOR_LAYER1_SRCCOLOR_FLAG_START (0)
#define SOC_MDC_LAYER1_INFO_SRCCOLOR_LAYER1_SRCCOLOR_FLAG_END (0)
#define SOC_MDC_LAYER1_INFO_SRCCOLOR_LAYER1_SRCCOLOR_INFO_START (2)
#define SOC_MDC_LAYER1_INFO_SRCCOLOR_LAYER1_SRCCOLOR_INFO_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer1_dbg_info : 32;
    } reg;
} soc_mdc_layer1_dbg_info_union;
#endif
#define SOC_MDC_LAYER1_DBG_INFO_LAYER1_DBG_INFO_START (0)
#define SOC_MDC_LAYER1_DBG_INFO_LAYER1_DBG_INFO_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer2_startx : 15;
        unsigned int reserved_0 : 1;
        unsigned int layer2_starty : 15;
        unsigned int reserved_1 : 1;
    } reg;
} soc_mdc_layer2_pos_union;
#endif
#define SOC_MDC_LAYER2_POS_LAYER2_STARTX_START (0)
#define SOC_MDC_LAYER2_POS_LAYER2_STARTX_END (14)
#define SOC_MDC_LAYER2_POS_LAYER2_STARTY_START (16)
#define SOC_MDC_LAYER2_POS_LAYER2_STARTY_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer2_endx : 15;
        unsigned int reserved_0 : 1;
        unsigned int layer2_endy : 15;
        unsigned int reserved_1 : 1;
    } reg;
} soc_mdc_layer2_size_union;
#endif
#define SOC_MDC_LAYER2_SIZE_LAYER2_ENDX_START (0)
#define SOC_MDC_LAYER2_SIZE_LAYER2_ENDX_END (14)
#define SOC_MDC_LAYER2_SIZE_LAYER2_ENDY_START (16)
#define SOC_MDC_LAYER2_SIZE_LAYER2_ENDY_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer2_src_locolorkey : 30;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_layer2_srclokey_union;
#endif
#define SOC_MDC_LAYER2_SRCLOKEY_LAYER2_SRC_LOCOLORKEY_START (0)
#define SOC_MDC_LAYER2_SRCLOKEY_LAYER2_SRC_LOCOLORKEY_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer2_src_hicolorkey : 30;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_layer2_srchikey_union;
#endif
#define SOC_MDC_LAYER2_SRCHIKEY_LAYER2_SRC_HICOLORKEY_START (0)
#define SOC_MDC_LAYER2_SRCHIKEY_LAYER2_SRC_HICOLORKEY_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer2_dst_locolorkey : 30;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_layer2_dstlokey_union;
#endif
#define SOC_MDC_LAYER2_DSTLOKEY_LAYER2_DST_LOCOLORKEY_START (0)
#define SOC_MDC_LAYER2_DSTLOKEY_LAYER2_DST_LOCOLORKEY_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer2_dst_hicolorkey : 30;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_layer2_dsthikey_union;
#endif
#define SOC_MDC_LAYER2_DSTHIKEY_LAYER2_DST_HICOLORKEY_START (0)
#define SOC_MDC_LAYER2_DSTHIKEY_LAYER2_DST_HICOLORKEY_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer2_pattern_rgb : 30;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_layer2_pattern_rgb_union;
#endif
#define SOC_MDC_LAYER2_PATTERN_RGB_LAYER2_PATTERN_RGB_START (0)
#define SOC_MDC_LAYER2_PATTERN_RGB_LAYER2_PATTERN_RGB_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer2_pattern_a : 10;
        unsigned int reserved : 22;
    } reg;
} soc_mdc_layer2_pattern_a_union;
#endif
#define SOC_MDC_LAYER2_PATTERN_A_LAYER2_PATTERN_A_START (0)
#define SOC_MDC_LAYER2_PATTERN_A_LAYER2_PATTERN_A_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 8;
        unsigned int layer2_fix_mode : 1;
        unsigned int layer2_dst_pmode : 1;
        unsigned int layer2_off_dst : 2;
        unsigned int layer2_dst_gmode : 2;
        unsigned int layer2_dst_amode : 2;
        unsigned int reserved_1 : 7;
        unsigned int layer2_src_mmode : 1;
        unsigned int layer2_alpha_smode : 1;
        unsigned int layer2_src_pmode : 1;
        unsigned int layer2_src_lmode : 1;
        unsigned int layer2_off_src : 1;
        unsigned int layer2_src_gmode : 2;
        unsigned int layer2_src_amode : 2;
    } reg;
} soc_mdc_layer2_alpha_mode_union;
#endif
#define SOC_MDC_LAYER2_ALPHA_MODE_LAYER2_FIX_MODE_START (8)
#define SOC_MDC_LAYER2_ALPHA_MODE_LAYER2_FIX_MODE_END (8)
#define SOC_MDC_LAYER2_ALPHA_MODE_LAYER2_DST_PMODE_START (9)
#define SOC_MDC_LAYER2_ALPHA_MODE_LAYER2_DST_PMODE_END (9)
#define SOC_MDC_LAYER2_ALPHA_MODE_LAYER2_OFF_DST_START (10)
#define SOC_MDC_LAYER2_ALPHA_MODE_LAYER2_OFF_DST_END (11)
#define SOC_MDC_LAYER2_ALPHA_MODE_LAYER2_DST_GMODE_START (12)
#define SOC_MDC_LAYER2_ALPHA_MODE_LAYER2_DST_GMODE_END (13)
#define SOC_MDC_LAYER2_ALPHA_MODE_LAYER2_DST_AMODE_START (14)
#define SOC_MDC_LAYER2_ALPHA_MODE_LAYER2_DST_AMODE_END (15)
#define SOC_MDC_LAYER2_ALPHA_MODE_LAYER2_SRC_MMODE_START (23)
#define SOC_MDC_LAYER2_ALPHA_MODE_LAYER2_SRC_MMODE_END (23)
#define SOC_MDC_LAYER2_ALPHA_MODE_LAYER2_ALPHA_SMODE_START (24)
#define SOC_MDC_LAYER2_ALPHA_MODE_LAYER2_ALPHA_SMODE_END (24)
#define SOC_MDC_LAYER2_ALPHA_MODE_LAYER2_SRC_PMODE_START (25)
#define SOC_MDC_LAYER2_ALPHA_MODE_LAYER2_SRC_PMODE_END (25)
#define SOC_MDC_LAYER2_ALPHA_MODE_LAYER2_SRC_LMODE_START (26)
#define SOC_MDC_LAYER2_ALPHA_MODE_LAYER2_SRC_LMODE_END (26)
#define SOC_MDC_LAYER2_ALPHA_MODE_LAYER2_OFF_SRC_START (27)
#define SOC_MDC_LAYER2_ALPHA_MODE_LAYER2_OFF_SRC_END (27)
#define SOC_MDC_LAYER2_ALPHA_MODE_LAYER2_SRC_GMODE_START (28)
#define SOC_MDC_LAYER2_ALPHA_MODE_LAYER2_SRC_GMODE_END (29)
#define SOC_MDC_LAYER2_ALPHA_MODE_LAYER2_SRC_AMODE_START (30)
#define SOC_MDC_LAYER2_ALPHA_MODE_LAYER2_SRC_AMODE_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer2_dst_global_alpha : 10;
        unsigned int reserved_0 : 6;
        unsigned int layer2_src_global_alpha : 10;
        unsigned int reserved_1 : 6;
    } reg;
} soc_mdc_layer2_alpha_a_union;
#endif
#define SOC_MDC_LAYER2_ALPHA_A_LAYER2_DST_GLOBAL_ALPHA_START (0)
#define SOC_MDC_LAYER2_ALPHA_A_LAYER2_DST_GLOBAL_ALPHA_END (9)
#define SOC_MDC_LAYER2_ALPHA_A_LAYER2_SRC_GLOBAL_ALPHA_START (16)
#define SOC_MDC_LAYER2_ALPHA_A_LAYER2_SRC_GLOBAL_ALPHA_END (25)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer2_enable : 1;
        unsigned int reserved_0 : 3;
        unsigned int layer2_trop_en : 1;
        unsigned int layer2_rop_en : 1;
        unsigned int reserved_1 : 2;
        unsigned int layer2_src_cfg : 1;
        unsigned int reserved_2 : 3;
        unsigned int reserved_3 : 3;
        unsigned int reserved_4 : 1;
        unsigned int layer2_auto_nosrc : 1;
        unsigned int reserved_5 : 3;
        unsigned int layer2_trop_code : 4;
        unsigned int layer2_rop_code : 8;
    } reg;
} soc_mdc_layer2_cfg_union;
#endif
#define SOC_MDC_LAYER2_CFG_LAYER2_ENABLE_START (0)
#define SOC_MDC_LAYER2_CFG_LAYER2_ENABLE_END (0)
#define SOC_MDC_LAYER2_CFG_LAYER2_TROP_EN_START (4)
#define SOC_MDC_LAYER2_CFG_LAYER2_TROP_EN_END (4)
#define SOC_MDC_LAYER2_CFG_LAYER2_ROP_EN_START (5)
#define SOC_MDC_LAYER2_CFG_LAYER2_ROP_EN_END (5)
#define SOC_MDC_LAYER2_CFG_LAYER2_SRC_CFG_START (8)
#define SOC_MDC_LAYER2_CFG_LAYER2_SRC_CFG_END (8)
#define SOC_MDC_LAYER2_CFG_LAYER2_AUTO_NOSRC_START (16)
#define SOC_MDC_LAYER2_CFG_LAYER2_AUTO_NOSRC_END (16)
#define SOC_MDC_LAYER2_CFG_LAYER2_TROP_CODE_START (20)
#define SOC_MDC_LAYER2_CFG_LAYER2_TROP_CODE_END (23)
#define SOC_MDC_LAYER2_CFG_LAYER2_ROP_CODE_START (24)
#define SOC_MDC_LAYER2_CFG_LAYER2_ROP_CODE_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer2_pstartx : 15;
        unsigned int reserved_0 : 1;
        unsigned int layer2_pstarty : 15;
        unsigned int reserved_1 : 1;
    } reg;
} soc_mdc_layer2_pspos_union;
#endif
#define SOC_MDC_LAYER2_PSPOS_LAYER2_PSTARTX_START (0)
#define SOC_MDC_LAYER2_PSPOS_LAYER2_PSTARTX_END (14)
#define SOC_MDC_LAYER2_PSPOS_LAYER2_PSTARTY_START (16)
#define SOC_MDC_LAYER2_PSPOS_LAYER2_PSTARTY_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer2_pendx : 15;
        unsigned int reserved_0 : 1;
        unsigned int layer2_pendy : 15;
        unsigned int reserved_1 : 1;
    } reg;
} soc_mdc_layer2_pepos_union;
#endif
#define SOC_MDC_LAYER2_PEPOS_LAYER2_PENDX_START (0)
#define SOC_MDC_LAYER2_PEPOS_LAYER2_PENDX_END (14)
#define SOC_MDC_LAYER2_PEPOS_LAYER2_PENDY_START (16)
#define SOC_MDC_LAYER2_PEPOS_LAYER2_PENDY_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer2_frame_end : 1;
        unsigned int layer2_nosrc : 1;
        unsigned int layer2_dstalpha_flag : 1;
        unsigned int layer2_srcalpha_flag : 1;
        unsigned int reserved_0 : 4;
        unsigned int layer2_dstalpha_info : 10;
        unsigned int layer2_srcalpha_info : 10;
        unsigned int reserved_1 : 4;
    } reg;
} soc_mdc_layer2_info_alpha_union;
#endif
#define SOC_MDC_LAYER2_INFO_ALPHA_LAYER2_FRAME_END_START (0)
#define SOC_MDC_LAYER2_INFO_ALPHA_LAYER2_FRAME_END_END (0)
#define SOC_MDC_LAYER2_INFO_ALPHA_LAYER2_NOSRC_START (1)
#define SOC_MDC_LAYER2_INFO_ALPHA_LAYER2_NOSRC_END (1)
#define SOC_MDC_LAYER2_INFO_ALPHA_LAYER2_DSTALPHA_FLAG_START (2)
#define SOC_MDC_LAYER2_INFO_ALPHA_LAYER2_DSTALPHA_FLAG_END (2)
#define SOC_MDC_LAYER2_INFO_ALPHA_LAYER2_SRCALPHA_FLAG_START (3)
#define SOC_MDC_LAYER2_INFO_ALPHA_LAYER2_SRCALPHA_FLAG_END (3)
#define SOC_MDC_LAYER2_INFO_ALPHA_LAYER2_DSTALPHA_INFO_START (8)
#define SOC_MDC_LAYER2_INFO_ALPHA_LAYER2_DSTALPHA_INFO_END (17)
#define SOC_MDC_LAYER2_INFO_ALPHA_LAYER2_SRCALPHA_INFO_START (18)
#define SOC_MDC_LAYER2_INFO_ALPHA_LAYER2_SRCALPHA_INFO_END (27)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer2_srccolor_flag : 1;
        unsigned int reserved : 1;
        unsigned int layer2_srccolor_info : 30;
    } reg;
} soc_mdc_layer2_info_srccolor_union;
#endif
#define SOC_MDC_LAYER2_INFO_SRCCOLOR_LAYER2_SRCCOLOR_FLAG_START (0)
#define SOC_MDC_LAYER2_INFO_SRCCOLOR_LAYER2_SRCCOLOR_FLAG_END (0)
#define SOC_MDC_LAYER2_INFO_SRCCOLOR_LAYER2_SRCCOLOR_INFO_START (2)
#define SOC_MDC_LAYER2_INFO_SRCCOLOR_LAYER2_SRCCOLOR_INFO_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer2_dbg_info : 32;
    } reg;
} soc_mdc_layer2_dbg_info_union;
#endif
#define SOC_MDC_LAYER2_DBG_INFO_LAYER2_DBG_INFO_START (0)
#define SOC_MDC_LAYER2_DBG_INFO_LAYER2_DBG_INFO_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer3_startx : 15;
        unsigned int reserved_0 : 1;
        unsigned int layer3_starty : 15;
        unsigned int reserved_1 : 1;
    } reg;
} soc_mdc_layer3_pos_union;
#endif
#define SOC_MDC_LAYER3_POS_LAYER3_STARTX_START (0)
#define SOC_MDC_LAYER3_POS_LAYER3_STARTX_END (14)
#define SOC_MDC_LAYER3_POS_LAYER3_STARTY_START (16)
#define SOC_MDC_LAYER3_POS_LAYER3_STARTY_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer3_endx : 15;
        unsigned int reserved_0 : 1;
        unsigned int layer3_endy : 15;
        unsigned int reserved_1 : 1;
    } reg;
} soc_mdc_layer3_size_union;
#endif
#define SOC_MDC_LAYER3_SIZE_LAYER3_ENDX_START (0)
#define SOC_MDC_LAYER3_SIZE_LAYER3_ENDX_END (14)
#define SOC_MDC_LAYER3_SIZE_LAYER3_ENDY_START (16)
#define SOC_MDC_LAYER3_SIZE_LAYER3_ENDY_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer3_src_locolorkey : 30;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_layer3_srclokey_union;
#endif
#define SOC_MDC_LAYER3_SRCLOKEY_LAYER3_SRC_LOCOLORKEY_START (0)
#define SOC_MDC_LAYER3_SRCLOKEY_LAYER3_SRC_LOCOLORKEY_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer3_src_hicolorkey : 30;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_layer3_srchikey_union;
#endif
#define SOC_MDC_LAYER3_SRCHIKEY_LAYER3_SRC_HICOLORKEY_START (0)
#define SOC_MDC_LAYER3_SRCHIKEY_LAYER3_SRC_HICOLORKEY_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer3_dst_locolorkey : 30;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_layer3_dstlokey_union;
#endif
#define SOC_MDC_LAYER3_DSTLOKEY_LAYER3_DST_LOCOLORKEY_START (0)
#define SOC_MDC_LAYER3_DSTLOKEY_LAYER3_DST_LOCOLORKEY_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer3_dst_hicolorkey : 30;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_layer3_dsthikey_union;
#endif
#define SOC_MDC_LAYER3_DSTHIKEY_LAYER3_DST_HICOLORKEY_START (0)
#define SOC_MDC_LAYER3_DSTHIKEY_LAYER3_DST_HICOLORKEY_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer3_pattern_rgb : 30;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_layer3_pattern_rgb_union;
#endif
#define SOC_MDC_LAYER3_PATTERN_RGB_LAYER3_PATTERN_RGB_START (0)
#define SOC_MDC_LAYER3_PATTERN_RGB_LAYER3_PATTERN_RGB_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer3_pattern_a : 10;
        unsigned int reserved : 22;
    } reg;
} soc_mdc_layer3_pattern_a_union;
#endif
#define SOC_MDC_LAYER3_PATTERN_A_LAYER3_PATTERN_A_START (0)
#define SOC_MDC_LAYER3_PATTERN_A_LAYER3_PATTERN_A_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 8;
        unsigned int layer3_fix_mode : 1;
        unsigned int layer3_dst_pmode : 1;
        unsigned int layer3_off_dst : 2;
        unsigned int layer3_dst_gmode : 2;
        unsigned int layer3_dst_amode : 2;
        unsigned int reserved_1 : 7;
        unsigned int layer3_src_mmode : 1;
        unsigned int layer3_alpha_smode : 1;
        unsigned int layer3_src_pmode : 1;
        unsigned int layer3_src_lmode : 1;
        unsigned int layer3_off_src : 1;
        unsigned int layer3_src_gmode : 2;
        unsigned int layer3_src_amode : 2;
    } reg;
} soc_mdc_layer3_alpha_mode_union;
#endif
#define SOC_MDC_LAYER3_ALPHA_MODE_LAYER3_FIX_MODE_START (8)
#define SOC_MDC_LAYER3_ALPHA_MODE_LAYER3_FIX_MODE_END (8)
#define SOC_MDC_LAYER3_ALPHA_MODE_LAYER3_DST_PMODE_START (9)
#define SOC_MDC_LAYER3_ALPHA_MODE_LAYER3_DST_PMODE_END (9)
#define SOC_MDC_LAYER3_ALPHA_MODE_LAYER3_OFF_DST_START (10)
#define SOC_MDC_LAYER3_ALPHA_MODE_LAYER3_OFF_DST_END (11)
#define SOC_MDC_LAYER3_ALPHA_MODE_LAYER3_DST_GMODE_START (12)
#define SOC_MDC_LAYER3_ALPHA_MODE_LAYER3_DST_GMODE_END (13)
#define SOC_MDC_LAYER3_ALPHA_MODE_LAYER3_DST_AMODE_START (14)
#define SOC_MDC_LAYER3_ALPHA_MODE_LAYER3_DST_AMODE_END (15)
#define SOC_MDC_LAYER3_ALPHA_MODE_LAYER3_SRC_MMODE_START (23)
#define SOC_MDC_LAYER3_ALPHA_MODE_LAYER3_SRC_MMODE_END (23)
#define SOC_MDC_LAYER3_ALPHA_MODE_LAYER3_ALPHA_SMODE_START (24)
#define SOC_MDC_LAYER3_ALPHA_MODE_LAYER3_ALPHA_SMODE_END (24)
#define SOC_MDC_LAYER3_ALPHA_MODE_LAYER3_SRC_PMODE_START (25)
#define SOC_MDC_LAYER3_ALPHA_MODE_LAYER3_SRC_PMODE_END (25)
#define SOC_MDC_LAYER3_ALPHA_MODE_LAYER3_SRC_LMODE_START (26)
#define SOC_MDC_LAYER3_ALPHA_MODE_LAYER3_SRC_LMODE_END (26)
#define SOC_MDC_LAYER3_ALPHA_MODE_LAYER3_OFF_SRC_START (27)
#define SOC_MDC_LAYER3_ALPHA_MODE_LAYER3_OFF_SRC_END (27)
#define SOC_MDC_LAYER3_ALPHA_MODE_LAYER3_SRC_GMODE_START (28)
#define SOC_MDC_LAYER3_ALPHA_MODE_LAYER3_SRC_GMODE_END (29)
#define SOC_MDC_LAYER3_ALPHA_MODE_LAYER3_SRC_AMODE_START (30)
#define SOC_MDC_LAYER3_ALPHA_MODE_LAYER3_SRC_AMODE_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer3_dst_global_alpha : 10;
        unsigned int reserved_0 : 6;
        unsigned int layer3_src_global_alpha : 10;
        unsigned int reserved_1 : 6;
    } reg;
} soc_mdc_layer3_alpha_a_union;
#endif
#define SOC_MDC_LAYER3_ALPHA_A_LAYER3_DST_GLOBAL_ALPHA_START (0)
#define SOC_MDC_LAYER3_ALPHA_A_LAYER3_DST_GLOBAL_ALPHA_END (9)
#define SOC_MDC_LAYER3_ALPHA_A_LAYER3_SRC_GLOBAL_ALPHA_START (16)
#define SOC_MDC_LAYER3_ALPHA_A_LAYER3_SRC_GLOBAL_ALPHA_END (25)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer3_enable : 1;
        unsigned int reserved_0 : 3;
        unsigned int layer3_trop_en : 1;
        unsigned int layer3_rop_en : 1;
        unsigned int reserved_1 : 2;
        unsigned int layer3_src_cfg : 1;
        unsigned int reserved_2 : 3;
        unsigned int reserved_3 : 3;
        unsigned int reserved_4 : 1;
        unsigned int layer3_auto_nosrc : 1;
        unsigned int reserved_5 : 3;
        unsigned int layer3_trop_code : 4;
        unsigned int layer3_rop_code : 8;
    } reg;
} soc_mdc_layer3_cfg_union;
#endif
#define SOC_MDC_LAYER3_CFG_LAYER3_ENABLE_START (0)
#define SOC_MDC_LAYER3_CFG_LAYER3_ENABLE_END (0)
#define SOC_MDC_LAYER3_CFG_LAYER3_TROP_EN_START (4)
#define SOC_MDC_LAYER3_CFG_LAYER3_TROP_EN_END (4)
#define SOC_MDC_LAYER3_CFG_LAYER3_ROP_EN_START (5)
#define SOC_MDC_LAYER3_CFG_LAYER3_ROP_EN_END (5)
#define SOC_MDC_LAYER3_CFG_LAYER3_SRC_CFG_START (8)
#define SOC_MDC_LAYER3_CFG_LAYER3_SRC_CFG_END (8)
#define SOC_MDC_LAYER3_CFG_LAYER3_AUTO_NOSRC_START (16)
#define SOC_MDC_LAYER3_CFG_LAYER3_AUTO_NOSRC_END (16)
#define SOC_MDC_LAYER3_CFG_LAYER3_TROP_CODE_START (20)
#define SOC_MDC_LAYER3_CFG_LAYER3_TROP_CODE_END (23)
#define SOC_MDC_LAYER3_CFG_LAYER3_ROP_CODE_START (24)
#define SOC_MDC_LAYER3_CFG_LAYER3_ROP_CODE_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer3_pstartx : 15;
        unsigned int reserved_0 : 1;
        unsigned int layer3_pstarty : 15;
        unsigned int reserved_1 : 1;
    } reg;
} soc_mdc_layer3_pspos_union;
#endif
#define SOC_MDC_LAYER3_PSPOS_LAYER3_PSTARTX_START (0)
#define SOC_MDC_LAYER3_PSPOS_LAYER3_PSTARTX_END (14)
#define SOC_MDC_LAYER3_PSPOS_LAYER3_PSTARTY_START (16)
#define SOC_MDC_LAYER3_PSPOS_LAYER3_PSTARTY_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer3_pendx : 15;
        unsigned int reserved_0 : 1;
        unsigned int layer3_pendy : 15;
        unsigned int reserved_1 : 1;
    } reg;
} soc_mdc_layer3_pepos_union;
#endif
#define SOC_MDC_LAYER3_PEPOS_LAYER3_PENDX_START (0)
#define SOC_MDC_LAYER3_PEPOS_LAYER3_PENDX_END (14)
#define SOC_MDC_LAYER3_PEPOS_LAYER3_PENDY_START (16)
#define SOC_MDC_LAYER3_PEPOS_LAYER3_PENDY_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer3_frame_end : 1;
        unsigned int layer3_nosrc : 1;
        unsigned int layer3_dstalpha_flag : 1;
        unsigned int layer3_srcalpha_flag : 1;
        unsigned int reserved_0 : 4;
        unsigned int layer3_dstalpha_info : 10;
        unsigned int layer3_srcalpha_info : 10;
        unsigned int reserved_1 : 4;
    } reg;
} soc_mdc_layer3_info_alpha_union;
#endif
#define SOC_MDC_LAYER3_INFO_ALPHA_LAYER3_FRAME_END_START (0)
#define SOC_MDC_LAYER3_INFO_ALPHA_LAYER3_FRAME_END_END (0)
#define SOC_MDC_LAYER3_INFO_ALPHA_LAYER3_NOSRC_START (1)
#define SOC_MDC_LAYER3_INFO_ALPHA_LAYER3_NOSRC_END (1)
#define SOC_MDC_LAYER3_INFO_ALPHA_LAYER3_DSTALPHA_FLAG_START (2)
#define SOC_MDC_LAYER3_INFO_ALPHA_LAYER3_DSTALPHA_FLAG_END (2)
#define SOC_MDC_LAYER3_INFO_ALPHA_LAYER3_SRCALPHA_FLAG_START (3)
#define SOC_MDC_LAYER3_INFO_ALPHA_LAYER3_SRCALPHA_FLAG_END (3)
#define SOC_MDC_LAYER3_INFO_ALPHA_LAYER3_DSTALPHA_INFO_START (8)
#define SOC_MDC_LAYER3_INFO_ALPHA_LAYER3_DSTALPHA_INFO_END (17)
#define SOC_MDC_LAYER3_INFO_ALPHA_LAYER3_SRCALPHA_INFO_START (18)
#define SOC_MDC_LAYER3_INFO_ALPHA_LAYER3_SRCALPHA_INFO_END (27)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer3_srccolor_flag : 1;
        unsigned int reserved : 1;
        unsigned int layer3_srccolor_info : 30;
    } reg;
} soc_mdc_layer3_info_srccolor_union;
#endif
#define SOC_MDC_LAYER3_INFO_SRCCOLOR_LAYER3_SRCCOLOR_FLAG_START (0)
#define SOC_MDC_LAYER3_INFO_SRCCOLOR_LAYER3_SRCCOLOR_FLAG_END (0)
#define SOC_MDC_LAYER3_INFO_SRCCOLOR_LAYER3_SRCCOLOR_INFO_START (2)
#define SOC_MDC_LAYER3_INFO_SRCCOLOR_LAYER3_SRCCOLOR_INFO_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer3_dbg_info : 32;
    } reg;
} soc_mdc_layer3_dbg_info_union;
#endif
#define SOC_MDC_LAYER3_DBG_INFO_LAYER3_DBG_INFO_START (0)
#define SOC_MDC_LAYER3_DBG_INFO_LAYER3_DBG_INFO_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer4_startx : 15;
        unsigned int reserved_0 : 1;
        unsigned int layer4_starty : 15;
        unsigned int reserved_1 : 1;
    } reg;
} soc_mdc_layer4_pos_union;
#endif
#define SOC_MDC_LAYER4_POS_LAYER4_STARTX_START (0)
#define SOC_MDC_LAYER4_POS_LAYER4_STARTX_END (14)
#define SOC_MDC_LAYER4_POS_LAYER4_STARTY_START (16)
#define SOC_MDC_LAYER4_POS_LAYER4_STARTY_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer4_endx : 15;
        unsigned int reserved_0 : 1;
        unsigned int layer4_endy : 15;
        unsigned int reserved_1 : 1;
    } reg;
} soc_mdc_layer4_size_union;
#endif
#define SOC_MDC_LAYER4_SIZE_LAYER4_ENDX_START (0)
#define SOC_MDC_LAYER4_SIZE_LAYER4_ENDX_END (14)
#define SOC_MDC_LAYER4_SIZE_LAYER4_ENDY_START (16)
#define SOC_MDC_LAYER4_SIZE_LAYER4_ENDY_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer4_src_locolorkey : 30;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_layer4_srclokey_union;
#endif
#define SOC_MDC_LAYER4_SRCLOKEY_LAYER4_SRC_LOCOLORKEY_START (0)
#define SOC_MDC_LAYER4_SRCLOKEY_LAYER4_SRC_LOCOLORKEY_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer4_src_hicolorkey : 30;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_layer4_srchikey_union;
#endif
#define SOC_MDC_LAYER4_SRCHIKEY_LAYER4_SRC_HICOLORKEY_START (0)
#define SOC_MDC_LAYER4_SRCHIKEY_LAYER4_SRC_HICOLORKEY_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer4_dst_locolorkey : 30;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_layer4_dstlokey_union;
#endif
#define SOC_MDC_LAYER4_DSTLOKEY_LAYER4_DST_LOCOLORKEY_START (0)
#define SOC_MDC_LAYER4_DSTLOKEY_LAYER4_DST_LOCOLORKEY_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer4_dst_hicolorkey : 30;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_layer4_dsthikey_union;
#endif
#define SOC_MDC_LAYER4_DSTHIKEY_LAYER4_DST_HICOLORKEY_START (0)
#define SOC_MDC_LAYER4_DSTHIKEY_LAYER4_DST_HICOLORKEY_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer4_pattern_rgb : 30;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_layer4_pattern_rgb_union;
#endif
#define SOC_MDC_LAYER4_PATTERN_RGB_LAYER4_PATTERN_RGB_START (0)
#define SOC_MDC_LAYER4_PATTERN_RGB_LAYER4_PATTERN_RGB_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer4_pattern_a : 10;
        unsigned int reserved : 22;
    } reg;
} soc_mdc_layer4_pattern_a_union;
#endif
#define SOC_MDC_LAYER4_PATTERN_A_LAYER4_PATTERN_A_START (0)
#define SOC_MDC_LAYER4_PATTERN_A_LAYER4_PATTERN_A_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 8;
        unsigned int layer4_fix_mode : 1;
        unsigned int layer4_dst_pmode : 1;
        unsigned int layer4_off_dst : 2;
        unsigned int layer4_dst_gmode : 2;
        unsigned int layer4_dst_amode : 2;
        unsigned int reserved_1 : 7;
        unsigned int layer4_src_mmode : 1;
        unsigned int layer4_alpha_smode : 1;
        unsigned int layer4_src_pmode : 1;
        unsigned int layer4_src_lmode : 1;
        unsigned int layer4_off_src : 1;
        unsigned int layer4_src_gmode : 2;
        unsigned int layer4_src_amode : 2;
    } reg;
} soc_mdc_layer4_alpha_mode_union;
#endif
#define SOC_MDC_LAYER4_ALPHA_MODE_LAYER4_FIX_MODE_START (8)
#define SOC_MDC_LAYER4_ALPHA_MODE_LAYER4_FIX_MODE_END (8)
#define SOC_MDC_LAYER4_ALPHA_MODE_LAYER4_DST_PMODE_START (9)
#define SOC_MDC_LAYER4_ALPHA_MODE_LAYER4_DST_PMODE_END (9)
#define SOC_MDC_LAYER4_ALPHA_MODE_LAYER4_OFF_DST_START (10)
#define SOC_MDC_LAYER4_ALPHA_MODE_LAYER4_OFF_DST_END (11)
#define SOC_MDC_LAYER4_ALPHA_MODE_LAYER4_DST_GMODE_START (12)
#define SOC_MDC_LAYER4_ALPHA_MODE_LAYER4_DST_GMODE_END (13)
#define SOC_MDC_LAYER4_ALPHA_MODE_LAYER4_DST_AMODE_START (14)
#define SOC_MDC_LAYER4_ALPHA_MODE_LAYER4_DST_AMODE_END (15)
#define SOC_MDC_LAYER4_ALPHA_MODE_LAYER4_SRC_MMODE_START (23)
#define SOC_MDC_LAYER4_ALPHA_MODE_LAYER4_SRC_MMODE_END (23)
#define SOC_MDC_LAYER4_ALPHA_MODE_LAYER4_ALPHA_SMODE_START (24)
#define SOC_MDC_LAYER4_ALPHA_MODE_LAYER4_ALPHA_SMODE_END (24)
#define SOC_MDC_LAYER4_ALPHA_MODE_LAYER4_SRC_PMODE_START (25)
#define SOC_MDC_LAYER4_ALPHA_MODE_LAYER4_SRC_PMODE_END (25)
#define SOC_MDC_LAYER4_ALPHA_MODE_LAYER4_SRC_LMODE_START (26)
#define SOC_MDC_LAYER4_ALPHA_MODE_LAYER4_SRC_LMODE_END (26)
#define SOC_MDC_LAYER4_ALPHA_MODE_LAYER4_OFF_SRC_START (27)
#define SOC_MDC_LAYER4_ALPHA_MODE_LAYER4_OFF_SRC_END (27)
#define SOC_MDC_LAYER4_ALPHA_MODE_LAYER4_SRC_GMODE_START (28)
#define SOC_MDC_LAYER4_ALPHA_MODE_LAYER4_SRC_GMODE_END (29)
#define SOC_MDC_LAYER4_ALPHA_MODE_LAYER4_SRC_AMODE_START (30)
#define SOC_MDC_LAYER4_ALPHA_MODE_LAYER4_SRC_AMODE_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer4_dst_global_alpha : 10;
        unsigned int reserved_0 : 6;
        unsigned int layer4_src_global_alpha : 10;
        unsigned int reserved_1 : 6;
    } reg;
} soc_mdc_layer4_alpha_a_union;
#endif
#define SOC_MDC_LAYER4_ALPHA_A_LAYER4_DST_GLOBAL_ALPHA_START (0)
#define SOC_MDC_LAYER4_ALPHA_A_LAYER4_DST_GLOBAL_ALPHA_END (9)
#define SOC_MDC_LAYER4_ALPHA_A_LAYER4_SRC_GLOBAL_ALPHA_START (16)
#define SOC_MDC_LAYER4_ALPHA_A_LAYER4_SRC_GLOBAL_ALPHA_END (25)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer4_enable : 1;
        unsigned int reserved_0 : 3;
        unsigned int layer4_trop_en : 1;
        unsigned int layer4_rop_en : 1;
        unsigned int reserved_1 : 2;
        unsigned int layer4_src_cfg : 1;
        unsigned int reserved_2 : 3;
        unsigned int reserved_3 : 3;
        unsigned int reserved_4 : 1;
        unsigned int layer4_auto_nosrc : 1;
        unsigned int reserved_5 : 3;
        unsigned int layer4_trop_code : 4;
        unsigned int layer4_rop_code : 8;
    } reg;
} soc_mdc_layer4_cfg_union;
#endif
#define SOC_MDC_LAYER4_CFG_LAYER4_ENABLE_START (0)
#define SOC_MDC_LAYER4_CFG_LAYER4_ENABLE_END (0)
#define SOC_MDC_LAYER4_CFG_LAYER4_TROP_EN_START (4)
#define SOC_MDC_LAYER4_CFG_LAYER4_TROP_EN_END (4)
#define SOC_MDC_LAYER4_CFG_LAYER4_ROP_EN_START (5)
#define SOC_MDC_LAYER4_CFG_LAYER4_ROP_EN_END (5)
#define SOC_MDC_LAYER4_CFG_LAYER4_SRC_CFG_START (8)
#define SOC_MDC_LAYER4_CFG_LAYER4_SRC_CFG_END (8)
#define SOC_MDC_LAYER4_CFG_LAYER4_AUTO_NOSRC_START (16)
#define SOC_MDC_LAYER4_CFG_LAYER4_AUTO_NOSRC_END (16)
#define SOC_MDC_LAYER4_CFG_LAYER4_TROP_CODE_START (20)
#define SOC_MDC_LAYER4_CFG_LAYER4_TROP_CODE_END (23)
#define SOC_MDC_LAYER4_CFG_LAYER4_ROP_CODE_START (24)
#define SOC_MDC_LAYER4_CFG_LAYER4_ROP_CODE_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer4_pstartx : 15;
        unsigned int reserved_0 : 1;
        unsigned int layer4_pstarty : 15;
        unsigned int reserved_1 : 1;
    } reg;
} soc_mdc_layer4_pspos_union;
#endif
#define SOC_MDC_LAYER4_PSPOS_LAYER4_PSTARTX_START (0)
#define SOC_MDC_LAYER4_PSPOS_LAYER4_PSTARTX_END (14)
#define SOC_MDC_LAYER4_PSPOS_LAYER4_PSTARTY_START (16)
#define SOC_MDC_LAYER4_PSPOS_LAYER4_PSTARTY_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer4_pendx : 15;
        unsigned int reserved_0 : 1;
        unsigned int layer4_pendy : 15;
        unsigned int reserved_1 : 1;
    } reg;
} soc_mdc_layer4_pepos_union;
#endif
#define SOC_MDC_LAYER4_PEPOS_LAYER4_PENDX_START (0)
#define SOC_MDC_LAYER4_PEPOS_LAYER4_PENDX_END (14)
#define SOC_MDC_LAYER4_PEPOS_LAYER4_PENDY_START (16)
#define SOC_MDC_LAYER4_PEPOS_LAYER4_PENDY_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer4_frame_end : 1;
        unsigned int layer4_nosrc : 1;
        unsigned int layer4_dstalpha_flag : 1;
        unsigned int layer4_srcalpha_flag : 1;
        unsigned int reserved_0 : 4;
        unsigned int layer4_dstalpha_info : 10;
        unsigned int layer4_srcalpha_info : 10;
        unsigned int reserved_1 : 4;
    } reg;
} soc_mdc_layer4_info_alpha_union;
#endif
#define SOC_MDC_LAYER4_INFO_ALPHA_LAYER4_FRAME_END_START (0)
#define SOC_MDC_LAYER4_INFO_ALPHA_LAYER4_FRAME_END_END (0)
#define SOC_MDC_LAYER4_INFO_ALPHA_LAYER4_NOSRC_START (1)
#define SOC_MDC_LAYER4_INFO_ALPHA_LAYER4_NOSRC_END (1)
#define SOC_MDC_LAYER4_INFO_ALPHA_LAYER4_DSTALPHA_FLAG_START (2)
#define SOC_MDC_LAYER4_INFO_ALPHA_LAYER4_DSTALPHA_FLAG_END (2)
#define SOC_MDC_LAYER4_INFO_ALPHA_LAYER4_SRCALPHA_FLAG_START (3)
#define SOC_MDC_LAYER4_INFO_ALPHA_LAYER4_SRCALPHA_FLAG_END (3)
#define SOC_MDC_LAYER4_INFO_ALPHA_LAYER4_DSTALPHA_INFO_START (8)
#define SOC_MDC_LAYER4_INFO_ALPHA_LAYER4_DSTALPHA_INFO_END (17)
#define SOC_MDC_LAYER4_INFO_ALPHA_LAYER4_SRCALPHA_INFO_START (18)
#define SOC_MDC_LAYER4_INFO_ALPHA_LAYER4_SRCALPHA_INFO_END (27)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer4_srccolor_flag : 1;
        unsigned int reserved : 1;
        unsigned int layer4_srccolor_info : 30;
    } reg;
} soc_mdc_layer4_info_srccolor_union;
#endif
#define SOC_MDC_LAYER4_INFO_SRCCOLOR_LAYER4_SRCCOLOR_FLAG_START (0)
#define SOC_MDC_LAYER4_INFO_SRCCOLOR_LAYER4_SRCCOLOR_FLAG_END (0)
#define SOC_MDC_LAYER4_INFO_SRCCOLOR_LAYER4_SRCCOLOR_INFO_START (2)
#define SOC_MDC_LAYER4_INFO_SRCCOLOR_LAYER4_SRCCOLOR_INFO_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer4_dbg_info : 32;
    } reg;
} soc_mdc_layer4_dbg_info_union;
#endif
#define SOC_MDC_LAYER4_DBG_INFO_LAYER4_DBG_INFO_START (0)
#define SOC_MDC_LAYER4_DBG_INFO_LAYER4_DBG_INFO_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer5_startx : 15;
        unsigned int reserved_0 : 1;
        unsigned int layer5_starty : 15;
        unsigned int reserved_1 : 1;
    } reg;
} soc_mdc_layer5_pos_union;
#endif
#define SOC_MDC_LAYER5_POS_LAYER5_STARTX_START (0)
#define SOC_MDC_LAYER5_POS_LAYER5_STARTX_END (14)
#define SOC_MDC_LAYER5_POS_LAYER5_STARTY_START (16)
#define SOC_MDC_LAYER5_POS_LAYER5_STARTY_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer5_endx : 15;
        unsigned int reserved_0 : 1;
        unsigned int layer5_endy : 15;
        unsigned int reserved_1 : 1;
    } reg;
} soc_mdc_layer5_size_union;
#endif
#define SOC_MDC_LAYER5_SIZE_LAYER5_ENDX_START (0)
#define SOC_MDC_LAYER5_SIZE_LAYER5_ENDX_END (14)
#define SOC_MDC_LAYER5_SIZE_LAYER5_ENDY_START (16)
#define SOC_MDC_LAYER5_SIZE_LAYER5_ENDY_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer5_src_locolorkey : 30;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_layer5_srclokey_union;
#endif
#define SOC_MDC_LAYER5_SRCLOKEY_LAYER5_SRC_LOCOLORKEY_START (0)
#define SOC_MDC_LAYER5_SRCLOKEY_LAYER5_SRC_LOCOLORKEY_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer5_src_hicolorkey : 30;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_layer5_srchikey_union;
#endif
#define SOC_MDC_LAYER5_SRCHIKEY_LAYER5_SRC_HICOLORKEY_START (0)
#define SOC_MDC_LAYER5_SRCHIKEY_LAYER5_SRC_HICOLORKEY_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer5_dst_locolorkey : 30;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_layer5_dstlokey_union;
#endif
#define SOC_MDC_LAYER5_DSTLOKEY_LAYER5_DST_LOCOLORKEY_START (0)
#define SOC_MDC_LAYER5_DSTLOKEY_LAYER5_DST_LOCOLORKEY_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer5_dst_hicolorkey : 30;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_layer5_dsthikey_union;
#endif
#define SOC_MDC_LAYER5_DSTHIKEY_LAYER5_DST_HICOLORKEY_START (0)
#define SOC_MDC_LAYER5_DSTHIKEY_LAYER5_DST_HICOLORKEY_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer5_pattern_rgb : 30;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_layer5_pattern_rgb_union;
#endif
#define SOC_MDC_LAYER5_PATTERN_RGB_LAYER5_PATTERN_RGB_START (0)
#define SOC_MDC_LAYER5_PATTERN_RGB_LAYER5_PATTERN_RGB_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer5_pattern_a : 10;
        unsigned int reserved : 22;
    } reg;
} soc_mdc_layer5_pattern_a_union;
#endif
#define SOC_MDC_LAYER5_PATTERN_A_LAYER5_PATTERN_A_START (0)
#define SOC_MDC_LAYER5_PATTERN_A_LAYER5_PATTERN_A_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 8;
        unsigned int layer5_fix_mode : 1;
        unsigned int layer5_dst_pmode : 1;
        unsigned int layer5_off_dst : 2;
        unsigned int layer5_dst_gmode : 2;
        unsigned int layer5_dst_amode : 2;
        unsigned int reserved_1 : 7;
        unsigned int layer5_src_mmode : 1;
        unsigned int layer5_alpha_smode : 1;
        unsigned int layer5_src_pmode : 1;
        unsigned int layer5_src_lmode : 1;
        unsigned int layer5_off_src : 1;
        unsigned int layer5_src_gmode : 2;
        unsigned int layer5_src_amode : 2;
    } reg;
} soc_mdc_layer5_alpha_mode_union;
#endif
#define SOC_MDC_LAYER5_ALPHA_MODE_LAYER5_FIX_MODE_START (8)
#define SOC_MDC_LAYER5_ALPHA_MODE_LAYER5_FIX_MODE_END (8)
#define SOC_MDC_LAYER5_ALPHA_MODE_LAYER5_DST_PMODE_START (9)
#define SOC_MDC_LAYER5_ALPHA_MODE_LAYER5_DST_PMODE_END (9)
#define SOC_MDC_LAYER5_ALPHA_MODE_LAYER5_OFF_DST_START (10)
#define SOC_MDC_LAYER5_ALPHA_MODE_LAYER5_OFF_DST_END (11)
#define SOC_MDC_LAYER5_ALPHA_MODE_LAYER5_DST_GMODE_START (12)
#define SOC_MDC_LAYER5_ALPHA_MODE_LAYER5_DST_GMODE_END (13)
#define SOC_MDC_LAYER5_ALPHA_MODE_LAYER5_DST_AMODE_START (14)
#define SOC_MDC_LAYER5_ALPHA_MODE_LAYER5_DST_AMODE_END (15)
#define SOC_MDC_LAYER5_ALPHA_MODE_LAYER5_SRC_MMODE_START (23)
#define SOC_MDC_LAYER5_ALPHA_MODE_LAYER5_SRC_MMODE_END (23)
#define SOC_MDC_LAYER5_ALPHA_MODE_LAYER5_ALPHA_SMODE_START (24)
#define SOC_MDC_LAYER5_ALPHA_MODE_LAYER5_ALPHA_SMODE_END (24)
#define SOC_MDC_LAYER5_ALPHA_MODE_LAYER5_SRC_PMODE_START (25)
#define SOC_MDC_LAYER5_ALPHA_MODE_LAYER5_SRC_PMODE_END (25)
#define SOC_MDC_LAYER5_ALPHA_MODE_LAYER5_SRC_LMODE_START (26)
#define SOC_MDC_LAYER5_ALPHA_MODE_LAYER5_SRC_LMODE_END (26)
#define SOC_MDC_LAYER5_ALPHA_MODE_LAYER5_OFF_SRC_START (27)
#define SOC_MDC_LAYER5_ALPHA_MODE_LAYER5_OFF_SRC_END (27)
#define SOC_MDC_LAYER5_ALPHA_MODE_LAYER5_SRC_GMODE_START (28)
#define SOC_MDC_LAYER5_ALPHA_MODE_LAYER5_SRC_GMODE_END (29)
#define SOC_MDC_LAYER5_ALPHA_MODE_LAYER5_SRC_AMODE_START (30)
#define SOC_MDC_LAYER5_ALPHA_MODE_LAYER5_SRC_AMODE_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer5_dst_global_alpha : 10;
        unsigned int reserved_0 : 6;
        unsigned int layer5_src_global_alpha : 10;
        unsigned int reserved_1 : 6;
    } reg;
} soc_mdc_layer5_alpha_a_union;
#endif
#define SOC_MDC_LAYER5_ALPHA_A_LAYER5_DST_GLOBAL_ALPHA_START (0)
#define SOC_MDC_LAYER5_ALPHA_A_LAYER5_DST_GLOBAL_ALPHA_END (9)
#define SOC_MDC_LAYER5_ALPHA_A_LAYER5_SRC_GLOBAL_ALPHA_START (16)
#define SOC_MDC_LAYER5_ALPHA_A_LAYER5_SRC_GLOBAL_ALPHA_END (25)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer5_enable : 1;
        unsigned int reserved_0 : 3;
        unsigned int layer5_trop_en : 1;
        unsigned int layer5_rop_en : 1;
        unsigned int reserved_1 : 2;
        unsigned int layer5_src_cfg : 1;
        unsigned int reserved_2 : 3;
        unsigned int reserved_3 : 3;
        unsigned int reserved_4 : 1;
        unsigned int layer5_auto_nosrc : 1;
        unsigned int reserved_5 : 3;
        unsigned int layer5_trop_code : 4;
        unsigned int layer5_rop_code : 8;
    } reg;
} soc_mdc_layer5_cfg_union;
#endif
#define SOC_MDC_LAYER5_CFG_LAYER5_ENABLE_START (0)
#define SOC_MDC_LAYER5_CFG_LAYER5_ENABLE_END (0)
#define SOC_MDC_LAYER5_CFG_LAYER5_TROP_EN_START (4)
#define SOC_MDC_LAYER5_CFG_LAYER5_TROP_EN_END (4)
#define SOC_MDC_LAYER5_CFG_LAYER5_ROP_EN_START (5)
#define SOC_MDC_LAYER5_CFG_LAYER5_ROP_EN_END (5)
#define SOC_MDC_LAYER5_CFG_LAYER5_SRC_CFG_START (8)
#define SOC_MDC_LAYER5_CFG_LAYER5_SRC_CFG_END (8)
#define SOC_MDC_LAYER5_CFG_LAYER5_AUTO_NOSRC_START (16)
#define SOC_MDC_LAYER5_CFG_LAYER5_AUTO_NOSRC_END (16)
#define SOC_MDC_LAYER5_CFG_LAYER5_TROP_CODE_START (20)
#define SOC_MDC_LAYER5_CFG_LAYER5_TROP_CODE_END (23)
#define SOC_MDC_LAYER5_CFG_LAYER5_ROP_CODE_START (24)
#define SOC_MDC_LAYER5_CFG_LAYER5_ROP_CODE_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer5_pstartx : 15;
        unsigned int reserved_0 : 1;
        unsigned int layer5_pstarty : 15;
        unsigned int reserved_1 : 1;
    } reg;
} soc_mdc_layer5_pspos_union;
#endif
#define SOC_MDC_LAYER5_PSPOS_LAYER5_PSTARTX_START (0)
#define SOC_MDC_LAYER5_PSPOS_LAYER5_PSTARTX_END (14)
#define SOC_MDC_LAYER5_PSPOS_LAYER5_PSTARTY_START (16)
#define SOC_MDC_LAYER5_PSPOS_LAYER5_PSTARTY_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer5_pendx : 15;
        unsigned int reserved_0 : 1;
        unsigned int layer5_pendy : 15;
        unsigned int reserved_1 : 1;
    } reg;
} soc_mdc_layer5_pepos_union;
#endif
#define SOC_MDC_LAYER5_PEPOS_LAYER5_PENDX_START (0)
#define SOC_MDC_LAYER5_PEPOS_LAYER5_PENDX_END (14)
#define SOC_MDC_LAYER5_PEPOS_LAYER5_PENDY_START (16)
#define SOC_MDC_LAYER5_PEPOS_LAYER5_PENDY_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer5_frame_end : 1;
        unsigned int layer5_nosrc : 1;
        unsigned int layer5_dstalpha_flag : 1;
        unsigned int layer5_srcalpha_flag : 1;
        unsigned int reserved_0 : 4;
        unsigned int layer5_dstalpha_info : 10;
        unsigned int layer5_srcalpha_info : 10;
        unsigned int reserved_1 : 4;
    } reg;
} soc_mdc_layer5_info_alpha_union;
#endif
#define SOC_MDC_LAYER5_INFO_ALPHA_LAYER5_FRAME_END_START (0)
#define SOC_MDC_LAYER5_INFO_ALPHA_LAYER5_FRAME_END_END (0)
#define SOC_MDC_LAYER5_INFO_ALPHA_LAYER5_NOSRC_START (1)
#define SOC_MDC_LAYER5_INFO_ALPHA_LAYER5_NOSRC_END (1)
#define SOC_MDC_LAYER5_INFO_ALPHA_LAYER5_DSTALPHA_FLAG_START (2)
#define SOC_MDC_LAYER5_INFO_ALPHA_LAYER5_DSTALPHA_FLAG_END (2)
#define SOC_MDC_LAYER5_INFO_ALPHA_LAYER5_SRCALPHA_FLAG_START (3)
#define SOC_MDC_LAYER5_INFO_ALPHA_LAYER5_SRCALPHA_FLAG_END (3)
#define SOC_MDC_LAYER5_INFO_ALPHA_LAYER5_DSTALPHA_INFO_START (8)
#define SOC_MDC_LAYER5_INFO_ALPHA_LAYER5_DSTALPHA_INFO_END (17)
#define SOC_MDC_LAYER5_INFO_ALPHA_LAYER5_SRCALPHA_INFO_START (18)
#define SOC_MDC_LAYER5_INFO_ALPHA_LAYER5_SRCALPHA_INFO_END (27)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer5_srccolor_flag : 1;
        unsigned int reserved : 1;
        unsigned int layer5_srccolor_info : 30;
    } reg;
} soc_mdc_layer5_info_srccolor_union;
#endif
#define SOC_MDC_LAYER5_INFO_SRCCOLOR_LAYER5_SRCCOLOR_FLAG_START (0)
#define SOC_MDC_LAYER5_INFO_SRCCOLOR_LAYER5_SRCCOLOR_FLAG_END (0)
#define SOC_MDC_LAYER5_INFO_SRCCOLOR_LAYER5_SRCCOLOR_INFO_START (2)
#define SOC_MDC_LAYER5_INFO_SRCCOLOR_LAYER5_SRCCOLOR_INFO_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer5_dbg_info : 32;
    } reg;
} soc_mdc_layer5_dbg_info_union;
#endif
#define SOC_MDC_LAYER5_DBG_INFO_LAYER5_DBG_INFO_START (0)
#define SOC_MDC_LAYER5_DBG_INFO_LAYER5_DBG_INFO_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer6_startx : 15;
        unsigned int reserved_0 : 1;
        unsigned int layer6_starty : 15;
        unsigned int reserved_1 : 1;
    } reg;
} soc_mdc_layer6_pos_union;
#endif
#define SOC_MDC_LAYER6_POS_LAYER6_STARTX_START (0)
#define SOC_MDC_LAYER6_POS_LAYER6_STARTX_END (14)
#define SOC_MDC_LAYER6_POS_LAYER6_STARTY_START (16)
#define SOC_MDC_LAYER6_POS_LAYER6_STARTY_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer6_endx : 15;
        unsigned int reserved_0 : 1;
        unsigned int layer6_endy : 15;
        unsigned int reserved_1 : 1;
    } reg;
} soc_mdc_layer6_size_union;
#endif
#define SOC_MDC_LAYER6_SIZE_LAYER6_ENDX_START (0)
#define SOC_MDC_LAYER6_SIZE_LAYER6_ENDX_END (14)
#define SOC_MDC_LAYER6_SIZE_LAYER6_ENDY_START (16)
#define SOC_MDC_LAYER6_SIZE_LAYER6_ENDY_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer6_src_locolorkey : 30;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_layer6_srclokey_union;
#endif
#define SOC_MDC_LAYER6_SRCLOKEY_LAYER6_SRC_LOCOLORKEY_START (0)
#define SOC_MDC_LAYER6_SRCLOKEY_LAYER6_SRC_LOCOLORKEY_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer6_src_hicolorkey : 30;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_layer6_srchikey_union;
#endif
#define SOC_MDC_LAYER6_SRCHIKEY_LAYER6_SRC_HICOLORKEY_START (0)
#define SOC_MDC_LAYER6_SRCHIKEY_LAYER6_SRC_HICOLORKEY_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer6_dst_locolorkey : 30;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_layer6_dstlokey_union;
#endif
#define SOC_MDC_LAYER6_DSTLOKEY_LAYER6_DST_LOCOLORKEY_START (0)
#define SOC_MDC_LAYER6_DSTLOKEY_LAYER6_DST_LOCOLORKEY_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer6_dst_hicolorkey : 30;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_layer6_dsthikey_union;
#endif
#define SOC_MDC_LAYER6_DSTHIKEY_LAYER6_DST_HICOLORKEY_START (0)
#define SOC_MDC_LAYER6_DSTHIKEY_LAYER6_DST_HICOLORKEY_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer6_pattern_rgb : 30;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_layer6_pattern_rgb_union;
#endif
#define SOC_MDC_LAYER6_PATTERN_RGB_LAYER6_PATTERN_RGB_START (0)
#define SOC_MDC_LAYER6_PATTERN_RGB_LAYER6_PATTERN_RGB_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer6_pattern_a : 10;
        unsigned int reserved : 22;
    } reg;
} soc_mdc_layer6_pattern_a_union;
#endif
#define SOC_MDC_LAYER6_PATTERN_A_LAYER6_PATTERN_A_START (0)
#define SOC_MDC_LAYER6_PATTERN_A_LAYER6_PATTERN_A_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 8;
        unsigned int layer6_fix_mode : 1;
        unsigned int layer6_dst_pmode : 1;
        unsigned int layer6_off_dst : 2;
        unsigned int layer6_dst_gmode : 2;
        unsigned int layer6_dst_amode : 2;
        unsigned int reserved_1 : 7;
        unsigned int layer6_src_mmode : 1;
        unsigned int layer6_alpha_smode : 1;
        unsigned int layer6_src_pmode : 1;
        unsigned int layer6_src_lmode : 1;
        unsigned int layer6_off_src : 1;
        unsigned int layer6_src_gmode : 2;
        unsigned int layer6_src_amode : 2;
    } reg;
} soc_mdc_layer6_alpha_mode_union;
#endif
#define SOC_MDC_LAYER6_ALPHA_MODE_LAYER6_FIX_MODE_START (8)
#define SOC_MDC_LAYER6_ALPHA_MODE_LAYER6_FIX_MODE_END (8)
#define SOC_MDC_LAYER6_ALPHA_MODE_LAYER6_DST_PMODE_START (9)
#define SOC_MDC_LAYER6_ALPHA_MODE_LAYER6_DST_PMODE_END (9)
#define SOC_MDC_LAYER6_ALPHA_MODE_LAYER6_OFF_DST_START (10)
#define SOC_MDC_LAYER6_ALPHA_MODE_LAYER6_OFF_DST_END (11)
#define SOC_MDC_LAYER6_ALPHA_MODE_LAYER6_DST_GMODE_START (12)
#define SOC_MDC_LAYER6_ALPHA_MODE_LAYER6_DST_GMODE_END (13)
#define SOC_MDC_LAYER6_ALPHA_MODE_LAYER6_DST_AMODE_START (14)
#define SOC_MDC_LAYER6_ALPHA_MODE_LAYER6_DST_AMODE_END (15)
#define SOC_MDC_LAYER6_ALPHA_MODE_LAYER6_SRC_MMODE_START (23)
#define SOC_MDC_LAYER6_ALPHA_MODE_LAYER6_SRC_MMODE_END (23)
#define SOC_MDC_LAYER6_ALPHA_MODE_LAYER6_ALPHA_SMODE_START (24)
#define SOC_MDC_LAYER6_ALPHA_MODE_LAYER6_ALPHA_SMODE_END (24)
#define SOC_MDC_LAYER6_ALPHA_MODE_LAYER6_SRC_PMODE_START (25)
#define SOC_MDC_LAYER6_ALPHA_MODE_LAYER6_SRC_PMODE_END (25)
#define SOC_MDC_LAYER6_ALPHA_MODE_LAYER6_SRC_LMODE_START (26)
#define SOC_MDC_LAYER6_ALPHA_MODE_LAYER6_SRC_LMODE_END (26)
#define SOC_MDC_LAYER6_ALPHA_MODE_LAYER6_OFF_SRC_START (27)
#define SOC_MDC_LAYER6_ALPHA_MODE_LAYER6_OFF_SRC_END (27)
#define SOC_MDC_LAYER6_ALPHA_MODE_LAYER6_SRC_GMODE_START (28)
#define SOC_MDC_LAYER6_ALPHA_MODE_LAYER6_SRC_GMODE_END (29)
#define SOC_MDC_LAYER6_ALPHA_MODE_LAYER6_SRC_AMODE_START (30)
#define SOC_MDC_LAYER6_ALPHA_MODE_LAYER6_SRC_AMODE_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer6_dst_global_alpha : 10;
        unsigned int reserved_0 : 6;
        unsigned int layer6_src_global_alpha : 10;
        unsigned int reserved_1 : 6;
    } reg;
} soc_mdc_layer6_alpha_a_union;
#endif
#define SOC_MDC_LAYER6_ALPHA_A_LAYER6_DST_GLOBAL_ALPHA_START (0)
#define SOC_MDC_LAYER6_ALPHA_A_LAYER6_DST_GLOBAL_ALPHA_END (9)
#define SOC_MDC_LAYER6_ALPHA_A_LAYER6_SRC_GLOBAL_ALPHA_START (16)
#define SOC_MDC_LAYER6_ALPHA_A_LAYER6_SRC_GLOBAL_ALPHA_END (25)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer6_enable : 1;
        unsigned int reserved_0 : 3;
        unsigned int layer6_trop_en : 1;
        unsigned int layer6_rop_en : 1;
        unsigned int reserved_1 : 2;
        unsigned int layer6_src_cfg : 1;
        unsigned int reserved_2 : 3;
        unsigned int reserved_3 : 3;
        unsigned int reserved_4 : 1;
        unsigned int layer6_auto_nosrc : 1;
        unsigned int reserved_5 : 3;
        unsigned int layer6_trop_code : 4;
        unsigned int layer6_rop_code : 8;
    } reg;
} soc_mdc_layer6_cfg_union;
#endif
#define SOC_MDC_LAYER6_CFG_LAYER6_ENABLE_START (0)
#define SOC_MDC_LAYER6_CFG_LAYER6_ENABLE_END (0)
#define SOC_MDC_LAYER6_CFG_LAYER6_TROP_EN_START (4)
#define SOC_MDC_LAYER6_CFG_LAYER6_TROP_EN_END (4)
#define SOC_MDC_LAYER6_CFG_LAYER6_ROP_EN_START (5)
#define SOC_MDC_LAYER6_CFG_LAYER6_ROP_EN_END (5)
#define SOC_MDC_LAYER6_CFG_LAYER6_SRC_CFG_START (8)
#define SOC_MDC_LAYER6_CFG_LAYER6_SRC_CFG_END (8)
#define SOC_MDC_LAYER6_CFG_LAYER6_AUTO_NOSRC_START (16)
#define SOC_MDC_LAYER6_CFG_LAYER6_AUTO_NOSRC_END (16)
#define SOC_MDC_LAYER6_CFG_LAYER6_TROP_CODE_START (20)
#define SOC_MDC_LAYER6_CFG_LAYER6_TROP_CODE_END (23)
#define SOC_MDC_LAYER6_CFG_LAYER6_ROP_CODE_START (24)
#define SOC_MDC_LAYER6_CFG_LAYER6_ROP_CODE_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer6_pstartx : 15;
        unsigned int reserved_0 : 1;
        unsigned int layer6_pstarty : 15;
        unsigned int reserved_1 : 1;
    } reg;
} soc_mdc_layer6_pspos_union;
#endif
#define SOC_MDC_LAYER6_PSPOS_LAYER6_PSTARTX_START (0)
#define SOC_MDC_LAYER6_PSPOS_LAYER6_PSTARTX_END (14)
#define SOC_MDC_LAYER6_PSPOS_LAYER6_PSTARTY_START (16)
#define SOC_MDC_LAYER6_PSPOS_LAYER6_PSTARTY_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer6_pendx : 15;
        unsigned int reserved_0 : 1;
        unsigned int layer6_pendy : 15;
        unsigned int reserved_1 : 1;
    } reg;
} soc_mdc_layer6_pepos_union;
#endif
#define SOC_MDC_LAYER6_PEPOS_LAYER6_PENDX_START (0)
#define SOC_MDC_LAYER6_PEPOS_LAYER6_PENDX_END (14)
#define SOC_MDC_LAYER6_PEPOS_LAYER6_PENDY_START (16)
#define SOC_MDC_LAYER6_PEPOS_LAYER6_PENDY_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer6_frame_end : 1;
        unsigned int layer6_nosrc : 1;
        unsigned int layer6_dstalpha_flag : 1;
        unsigned int layer6_srcalpha_flag : 1;
        unsigned int reserved_0 : 4;
        unsigned int layer6_dstalpha_info : 10;
        unsigned int layer6_srcalpha_info : 10;
        unsigned int reserved_1 : 4;
    } reg;
} soc_mdc_layer6_info_alpha_union;
#endif
#define SOC_MDC_LAYER6_INFO_ALPHA_LAYER6_FRAME_END_START (0)
#define SOC_MDC_LAYER6_INFO_ALPHA_LAYER6_FRAME_END_END (0)
#define SOC_MDC_LAYER6_INFO_ALPHA_LAYER6_NOSRC_START (1)
#define SOC_MDC_LAYER6_INFO_ALPHA_LAYER6_NOSRC_END (1)
#define SOC_MDC_LAYER6_INFO_ALPHA_LAYER6_DSTALPHA_FLAG_START (2)
#define SOC_MDC_LAYER6_INFO_ALPHA_LAYER6_DSTALPHA_FLAG_END (2)
#define SOC_MDC_LAYER6_INFO_ALPHA_LAYER6_SRCALPHA_FLAG_START (3)
#define SOC_MDC_LAYER6_INFO_ALPHA_LAYER6_SRCALPHA_FLAG_END (3)
#define SOC_MDC_LAYER6_INFO_ALPHA_LAYER6_DSTALPHA_INFO_START (8)
#define SOC_MDC_LAYER6_INFO_ALPHA_LAYER6_DSTALPHA_INFO_END (17)
#define SOC_MDC_LAYER6_INFO_ALPHA_LAYER6_SRCALPHA_INFO_START (18)
#define SOC_MDC_LAYER6_INFO_ALPHA_LAYER6_SRCALPHA_INFO_END (27)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer6_srccolor_flag : 1;
        unsigned int reserved : 1;
        unsigned int layer6_srccolor_info : 30;
    } reg;
} soc_mdc_layer6_info_srccolor_union;
#endif
#define SOC_MDC_LAYER6_INFO_SRCCOLOR_LAYER6_SRCCOLOR_FLAG_START (0)
#define SOC_MDC_LAYER6_INFO_SRCCOLOR_LAYER6_SRCCOLOR_FLAG_END (0)
#define SOC_MDC_LAYER6_INFO_SRCCOLOR_LAYER6_SRCCOLOR_INFO_START (2)
#define SOC_MDC_LAYER6_INFO_SRCCOLOR_LAYER6_SRCCOLOR_INFO_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer6_dbg_info : 32;
    } reg;
} soc_mdc_layer6_dbg_info_union;
#endif
#define SOC_MDC_LAYER6_DBG_INFO_LAYER6_DBG_INFO_START (0)
#define SOC_MDC_LAYER6_DBG_INFO_LAYER6_DBG_INFO_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer7_startx : 15;
        unsigned int reserved_0 : 1;
        unsigned int layer7_starty : 15;
        unsigned int reserved_1 : 1;
    } reg;
} soc_mdc_layer7_pos_union;
#endif
#define SOC_MDC_LAYER7_POS_LAYER7_STARTX_START (0)
#define SOC_MDC_LAYER7_POS_LAYER7_STARTX_END (14)
#define SOC_MDC_LAYER7_POS_LAYER7_STARTY_START (16)
#define SOC_MDC_LAYER7_POS_LAYER7_STARTY_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer7_endx : 15;
        unsigned int reserved_0 : 1;
        unsigned int layer7_endy : 15;
        unsigned int reserved_1 : 1;
    } reg;
} soc_mdc_layer7_size_union;
#endif
#define SOC_MDC_LAYER7_SIZE_LAYER7_ENDX_START (0)
#define SOC_MDC_LAYER7_SIZE_LAYER7_ENDX_END (14)
#define SOC_MDC_LAYER7_SIZE_LAYER7_ENDY_START (16)
#define SOC_MDC_LAYER7_SIZE_LAYER7_ENDY_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer7_src_locolorkey : 30;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_layer7_srclokey_union;
#endif
#define SOC_MDC_LAYER7_SRCLOKEY_LAYER7_SRC_LOCOLORKEY_START (0)
#define SOC_MDC_LAYER7_SRCLOKEY_LAYER7_SRC_LOCOLORKEY_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer7_src_hicolorkey : 30;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_layer7_srchikey_union;
#endif
#define SOC_MDC_LAYER7_SRCHIKEY_LAYER7_SRC_HICOLORKEY_START (0)
#define SOC_MDC_LAYER7_SRCHIKEY_LAYER7_SRC_HICOLORKEY_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer7_dst_locolorkey : 30;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_layer7_dstlokey_union;
#endif
#define SOC_MDC_LAYER7_DSTLOKEY_LAYER7_DST_LOCOLORKEY_START (0)
#define SOC_MDC_LAYER7_DSTLOKEY_LAYER7_DST_LOCOLORKEY_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer7_dst_hicolorkey : 30;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_layer7_dsthikey_union;
#endif
#define SOC_MDC_LAYER7_DSTHIKEY_LAYER7_DST_HICOLORKEY_START (0)
#define SOC_MDC_LAYER7_DSTHIKEY_LAYER7_DST_HICOLORKEY_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer7_pattern_rgb : 30;
        unsigned int reserved : 2;
    } reg;
} soc_mdc_layer7_pattern_rgb_union;
#endif
#define SOC_MDC_LAYER7_PATTERN_RGB_LAYER7_PATTERN_RGB_START (0)
#define SOC_MDC_LAYER7_PATTERN_RGB_LAYER7_PATTERN_RGB_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer7_pattern_a : 10;
        unsigned int reserved : 22;
    } reg;
} soc_mdc_layer7_pattern_a_union;
#endif
#define SOC_MDC_LAYER7_PATTERN_A_LAYER7_PATTERN_A_START (0)
#define SOC_MDC_LAYER7_PATTERN_A_LAYER7_PATTERN_A_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 8;
        unsigned int layer7_fix_mode : 1;
        unsigned int layer7_dst_pmode : 1;
        unsigned int layer7_off_dst : 2;
        unsigned int layer7_dst_gmode : 2;
        unsigned int layer7_dst_amode : 2;
        unsigned int reserved_1 : 7;
        unsigned int layer7_src_mmode : 1;
        unsigned int layer7_alpha_smode : 1;
        unsigned int layer7_src_pmode : 1;
        unsigned int layer7_src_lmode : 1;
        unsigned int layer7_off_src : 1;
        unsigned int layer7_src_gmode : 2;
        unsigned int layer7_src_amode : 2;
    } reg;
} soc_mdc_layer7_alpha_mode_union;
#endif
#define SOC_MDC_LAYER7_ALPHA_MODE_LAYER7_FIX_MODE_START (8)
#define SOC_MDC_LAYER7_ALPHA_MODE_LAYER7_FIX_MODE_END (8)
#define SOC_MDC_LAYER7_ALPHA_MODE_LAYER7_DST_PMODE_START (9)
#define SOC_MDC_LAYER7_ALPHA_MODE_LAYER7_DST_PMODE_END (9)
#define SOC_MDC_LAYER7_ALPHA_MODE_LAYER7_OFF_DST_START (10)
#define SOC_MDC_LAYER7_ALPHA_MODE_LAYER7_OFF_DST_END (11)
#define SOC_MDC_LAYER7_ALPHA_MODE_LAYER7_DST_GMODE_START (12)
#define SOC_MDC_LAYER7_ALPHA_MODE_LAYER7_DST_GMODE_END (13)
#define SOC_MDC_LAYER7_ALPHA_MODE_LAYER7_DST_AMODE_START (14)
#define SOC_MDC_LAYER7_ALPHA_MODE_LAYER7_DST_AMODE_END (15)
#define SOC_MDC_LAYER7_ALPHA_MODE_LAYER7_SRC_MMODE_START (23)
#define SOC_MDC_LAYER7_ALPHA_MODE_LAYER7_SRC_MMODE_END (23)
#define SOC_MDC_LAYER7_ALPHA_MODE_LAYER7_ALPHA_SMODE_START (24)
#define SOC_MDC_LAYER7_ALPHA_MODE_LAYER7_ALPHA_SMODE_END (24)
#define SOC_MDC_LAYER7_ALPHA_MODE_LAYER7_SRC_PMODE_START (25)
#define SOC_MDC_LAYER7_ALPHA_MODE_LAYER7_SRC_PMODE_END (25)
#define SOC_MDC_LAYER7_ALPHA_MODE_LAYER7_SRC_LMODE_START (26)
#define SOC_MDC_LAYER7_ALPHA_MODE_LAYER7_SRC_LMODE_END (26)
#define SOC_MDC_LAYER7_ALPHA_MODE_LAYER7_OFF_SRC_START (27)
#define SOC_MDC_LAYER7_ALPHA_MODE_LAYER7_OFF_SRC_END (27)
#define SOC_MDC_LAYER7_ALPHA_MODE_LAYER7_SRC_GMODE_START (28)
#define SOC_MDC_LAYER7_ALPHA_MODE_LAYER7_SRC_GMODE_END (29)
#define SOC_MDC_LAYER7_ALPHA_MODE_LAYER7_SRC_AMODE_START (30)
#define SOC_MDC_LAYER7_ALPHA_MODE_LAYER7_SRC_AMODE_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer7_dst_global_alpha : 10;
        unsigned int reserved_0 : 6;
        unsigned int layer7_src_global_alpha : 10;
        unsigned int reserved_1 : 6;
    } reg;
} soc_mdc_layer7_alpha_a_union;
#endif
#define SOC_MDC_LAYER7_ALPHA_A_LAYER7_DST_GLOBAL_ALPHA_START (0)
#define SOC_MDC_LAYER7_ALPHA_A_LAYER7_DST_GLOBAL_ALPHA_END (9)
#define SOC_MDC_LAYER7_ALPHA_A_LAYER7_SRC_GLOBAL_ALPHA_START (16)
#define SOC_MDC_LAYER7_ALPHA_A_LAYER7_SRC_GLOBAL_ALPHA_END (25)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer7_enable : 1;
        unsigned int reserved_0 : 3;
        unsigned int layer7_trop_en : 1;
        unsigned int layer7_rop_en : 1;
        unsigned int reserved_1 : 2;
        unsigned int layer7_src_cfg : 1;
        unsigned int reserved_2 : 3;
        unsigned int reserved_3 : 3;
        unsigned int reserved_4 : 1;
        unsigned int layer7_auto_nosrc : 1;
        unsigned int reserved_5 : 3;
        unsigned int layer7_trop_code : 4;
        unsigned int layer7_rop_code : 8;
    } reg;
} soc_mdc_layer7_cfg_union;
#endif
#define SOC_MDC_LAYER7_CFG_LAYER7_ENABLE_START (0)
#define SOC_MDC_LAYER7_CFG_LAYER7_ENABLE_END (0)
#define SOC_MDC_LAYER7_CFG_LAYER7_TROP_EN_START (4)
#define SOC_MDC_LAYER7_CFG_LAYER7_TROP_EN_END (4)
#define SOC_MDC_LAYER7_CFG_LAYER7_ROP_EN_START (5)
#define SOC_MDC_LAYER7_CFG_LAYER7_ROP_EN_END (5)
#define SOC_MDC_LAYER7_CFG_LAYER7_SRC_CFG_START (8)
#define SOC_MDC_LAYER7_CFG_LAYER7_SRC_CFG_END (8)
#define SOC_MDC_LAYER7_CFG_LAYER7_AUTO_NOSRC_START (16)
#define SOC_MDC_LAYER7_CFG_LAYER7_AUTO_NOSRC_END (16)
#define SOC_MDC_LAYER7_CFG_LAYER7_TROP_CODE_START (20)
#define SOC_MDC_LAYER7_CFG_LAYER7_TROP_CODE_END (23)
#define SOC_MDC_LAYER7_CFG_LAYER7_ROP_CODE_START (24)
#define SOC_MDC_LAYER7_CFG_LAYER7_ROP_CODE_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer7_pstartx : 15;
        unsigned int reserved_0 : 1;
        unsigned int layer7_pstarty : 15;
        unsigned int reserved_1 : 1;
    } reg;
} soc_mdc_layer7_pspos_union;
#endif
#define SOC_MDC_LAYER7_PSPOS_LAYER7_PSTARTX_START (0)
#define SOC_MDC_LAYER7_PSPOS_LAYER7_PSTARTX_END (14)
#define SOC_MDC_LAYER7_PSPOS_LAYER7_PSTARTY_START (16)
#define SOC_MDC_LAYER7_PSPOS_LAYER7_PSTARTY_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer7_pendx : 15;
        unsigned int reserved_0 : 1;
        unsigned int layer7_pendy : 15;
        unsigned int reserved_1 : 1;
    } reg;
} soc_mdc_layer7_pepos_union;
#endif
#define SOC_MDC_LAYER7_PEPOS_LAYER7_PENDX_START (0)
#define SOC_MDC_LAYER7_PEPOS_LAYER7_PENDX_END (14)
#define SOC_MDC_LAYER7_PEPOS_LAYER7_PENDY_START (16)
#define SOC_MDC_LAYER7_PEPOS_LAYER7_PENDY_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer7_frame_end : 1;
        unsigned int layer7_nosrc : 1;
        unsigned int layer7_dstalpha_flag : 1;
        unsigned int layer7_srcalpha_flag : 1;
        unsigned int reserved_0 : 4;
        unsigned int layer7_dstalpha_info : 10;
        unsigned int layer7_srcalpha_info : 10;
        unsigned int reserved_1 : 4;
    } reg;
} soc_mdc_layer7_info_alpha_union;
#endif
#define SOC_MDC_LAYER7_INFO_ALPHA_LAYER7_FRAME_END_START (0)
#define SOC_MDC_LAYER7_INFO_ALPHA_LAYER7_FRAME_END_END (0)
#define SOC_MDC_LAYER7_INFO_ALPHA_LAYER7_NOSRC_START (1)
#define SOC_MDC_LAYER7_INFO_ALPHA_LAYER7_NOSRC_END (1)
#define SOC_MDC_LAYER7_INFO_ALPHA_LAYER7_DSTALPHA_FLAG_START (2)
#define SOC_MDC_LAYER7_INFO_ALPHA_LAYER7_DSTALPHA_FLAG_END (2)
#define SOC_MDC_LAYER7_INFO_ALPHA_LAYER7_SRCALPHA_FLAG_START (3)
#define SOC_MDC_LAYER7_INFO_ALPHA_LAYER7_SRCALPHA_FLAG_END (3)
#define SOC_MDC_LAYER7_INFO_ALPHA_LAYER7_DSTALPHA_INFO_START (8)
#define SOC_MDC_LAYER7_INFO_ALPHA_LAYER7_DSTALPHA_INFO_END (17)
#define SOC_MDC_LAYER7_INFO_ALPHA_LAYER7_SRCALPHA_INFO_START (18)
#define SOC_MDC_LAYER7_INFO_ALPHA_LAYER7_SRCALPHA_INFO_END (27)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer7_srccolor_flag : 1;
        unsigned int reserved : 1;
        unsigned int layer7_srccolor_info : 30;
    } reg;
} soc_mdc_layer7_info_srccolor_union;
#endif
#define SOC_MDC_LAYER7_INFO_SRCCOLOR_LAYER7_SRCCOLOR_FLAG_START (0)
#define SOC_MDC_LAYER7_INFO_SRCCOLOR_LAYER7_SRCCOLOR_FLAG_END (0)
#define SOC_MDC_LAYER7_INFO_SRCCOLOR_LAYER7_SRCCOLOR_INFO_START (2)
#define SOC_MDC_LAYER7_INFO_SRCCOLOR_LAYER7_SRCCOLOR_INFO_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int layer7_dbg_info : 32;
    } reg;
} soc_mdc_layer7_dbg_info_union;
#endif
#define SOC_MDC_LAYER7_DBG_INFO_LAYER7_DBG_INFO_START (0)
#define SOC_MDC_LAYER7_DBG_INFO_LAYER7_DBG_INFO_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int base_dbg_info : 32;
    } reg;
} soc_mdc_base_dbg_info_union;
#endif
#define SOC_MDC_BASE_DBG_INFO_BASE_DBG_INFO_START (0)
#define SOC_MDC_BASE_DBG_INFO_BASE_DBG_INFO_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ov_rd_shadow_sel : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_ov_rd_shadow_sel_union;
#endif
#define SOC_MDC_OV_RD_SHADOW_SEL_OV_RD_SHADOW_SEL_START (0)
#define SOC_MDC_OV_RD_SHADOW_SEL_OV_RD_SHADOW_SEL_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ov_clk_sel : 32;
    } reg;
} soc_mdc_ov_clk_sel_union;
#endif
#define SOC_MDC_OV_CLK_SEL_OV_CLK_SEL_START (0)
#define SOC_MDC_OV_CLK_SEL_OV_CLK_SEL_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ov_clk_en : 32;
    } reg;
} soc_mdc_ov_clk_en_union;
#endif
#define SOC_MDC_OV_CLK_EN_OV_CLK_EN_START (0)
#define SOC_MDC_OV_CLK_EN_OV_CLK_EN_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 16;
        unsigned int block_height : 15;
        unsigned int reserved_1 : 1;
    } reg;
} soc_mdc_block_size_union;
#endif
#define SOC_MDC_BLOCK_SIZE_BLOCK_HEIGHT_START (16)
#define SOC_MDC_BLOCK_SIZE_BLOCK_HEIGHT_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int block_dbg : 32;
    } reg;
} soc_mdc_block_dbg_union;
#endif
#define SOC_MDC_BLOCK_DBG_BLOCK_DBG_START (0)
#define SOC_MDC_BLOCK_DBG_BLOCK_DBG_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reg_default : 1;
        unsigned int reserved : 31;
    } reg;
} soc_mdc_ov8_reg_default_union;
#endif
#define SOC_MDC_OV8_REG_DEFAULT_REG_DEFAULT_START (0)
#define SOC_MDC_OV8_REG_DEFAULT_REG_DEFAULT_END (0)
#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
#endif
