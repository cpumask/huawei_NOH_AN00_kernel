#ifndef __SOC_MEDIA1_CRG_INTERFACE_H__
#define __SOC_MEDIA1_CRG_INTERFACE_H__ 
#ifdef __cplusplus
    #if __cplusplus
        extern "C" {
    #endif
#endif
#ifndef __SOC_H_FOR_ASM__
#define SOC_MEDIA1_CRG_PEREN0_ADDR(base) ((base) + (0x000UL))
#define SOC_MEDIA1_CRG_PERDIS0_ADDR(base) ((base) + (0x004UL))
#define SOC_MEDIA1_CRG_PERCLKEN0_ADDR(base) ((base) + (0x008UL))
#define SOC_MEDIA1_CRG_PERSTAT0_ADDR(base) ((base) + (0x00CUL))
#define SOC_MEDIA1_CRG_PEREN1_ADDR(base) ((base) + (0x010UL))
#define SOC_MEDIA1_CRG_PERDIS1_ADDR(base) ((base) + (0x014UL))
#define SOC_MEDIA1_CRG_PERCLKEN1_ADDR(base) ((base) + (0x018UL))
#define SOC_MEDIA1_CRG_PERSTAT1_ADDR(base) ((base) + (0x01CUL))
#define SOC_MEDIA1_CRG_PEREN2_ADDR(base) ((base) + (0x020UL))
#define SOC_MEDIA1_CRG_PERDIS2_ADDR(base) ((base) + (0x024UL))
#define SOC_MEDIA1_CRG_PERCLKEN2_ADDR(base) ((base) + (0x028UL))
#define SOC_MEDIA1_CRG_PERSTAT2_ADDR(base) ((base) + (0x02CUL))
#define SOC_MEDIA1_CRG_PERRSTEN0_ADDR(base) ((base) + (0x030UL))
#define SOC_MEDIA1_CRG_PERRSTDIS0_ADDR(base) ((base) + (0x034UL))
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ADDR(base) ((base) + (0x038UL))
#define SOC_MEDIA1_CRG_PERRSTEN1_ADDR(base) ((base) + (0x03CUL))
#define SOC_MEDIA1_CRG_PERRSTDIS1_ADDR(base) ((base) + (0x040UL))
#define SOC_MEDIA1_CRG_PERRSTSTAT1_ADDR(base) ((base) + (0x044UL))
#define SOC_MEDIA1_CRG_CLKDIV0_ADDR(base) ((base) + (0x060UL))
#define SOC_MEDIA1_CRG_CLKDIV1_ADDR(base) ((base) + (0x064UL))
#define SOC_MEDIA1_CRG_CLKDIV2_ADDR(base) ((base) + (0x068UL))
#define SOC_MEDIA1_CRG_CLKDIV3_ADDR(base) ((base) + (0x06CUL))
#define SOC_MEDIA1_CRG_CLKDIV4_ADDR(base) ((base) + (0x070UL))
#define SOC_MEDIA1_CRG_CLKDIV5_ADDR(base) ((base) + (0x074UL))
#define SOC_MEDIA1_CRG_CLKDIV6_ADDR(base) ((base) + (0x078UL))
#define SOC_MEDIA1_CRG_CLKDIV7_ADDR(base) ((base) + (0x07CUL))
#define SOC_MEDIA1_CRG_CLKDIV8_ADDR(base) ((base) + (0x080UL))
#define SOC_MEDIA1_CRG_CLKDIV9_ADDR(base) ((base) + (0x084UL))
#define SOC_MEDIA1_CRG_CLKDIV10_ADDR(base) ((base) + (0x088UL))
#define SOC_MEDIA1_CRG_CLKDIV11_ADDR(base) ((base) + (0x08CUL))
#define SOC_MEDIA1_CRG_CLKDIV12_ADDR(base) ((base) + (0x090UL))
#define SOC_MEDIA1_CRG_CLKDIV13_ADDR(base) ((base) + (0x094UL))
#define SOC_MEDIA1_CRG_CLKDIV15_ADDR(base) ((base) + (0x098UL))
#define SOC_MEDIA1_CRG_CLKDIV16_ADDR(base) ((base) + (0x09CUL))
#define SOC_MEDIA1_CRG_PERI_STAT0_ADDR(base) ((base) + (0x0A0UL))
#define SOC_MEDIA1_CRG_PERI_STAT1_ADDR(base) ((base) + (0x0A4UL))
#define SOC_MEDIA1_CRG_PERI_STAT2_ADDR(base) ((base) + (0x0A8UL))
#define SOC_MEDIA1_CRG_ISOEN_ADDR(base) ((base) + (0x100UL))
#define SOC_MEDIA1_CRG_ISODIS_ADDR(base) ((base) + (0x104UL))
#define SOC_MEDIA1_CRG_ISOSTAT_ADDR(base) ((base) + (0x108UL))
#define SOC_MEDIA1_CRG_IPCLKRST_BYPASS_ADDR(base) ((base) + (0x130UL))
#define SOC_MEDIA1_CRG_ISPCPU_CLKEN_ADDR(base) ((base) + (0x140UL))
#define SOC_MEDIA1_CRG_ISPCPU_RSTEN_ADDR(base) ((base) + (0x144UL))
#define SOC_MEDIA1_CRG_ISPCPU_RSTDIS_ADDR(base) ((base) + (0x148UL))
#define SOC_MEDIA1_CRG_ISPCPU_RSTSTAT_ADDR(base) ((base) + (0x14CUL))
#define SOC_MEDIA1_CRG_ISP_CPU_STATE0_ADDR(base) ((base) + (0x150UL))
#define SOC_MEDIA1_CRG_ISP_CPU_STATE1_ADDR(base) ((base) + (0x154UL))
#define SOC_MEDIA1_CRG_PERI_AUTODIV0_ADDR(base) ((base) + (0x160UL))
#define SOC_MEDIA1_CRG_PERI_AUTODIV1_ADDR(base) ((base) + (0x164UL))
#define SOC_MEDIA1_CRG_PERI_AUTODIV2_ADDR(base) ((base) + (0x168UL))
#define SOC_MEDIA1_CRG_PERI_AUTODIV3_ADDR(base) ((base) + (0x16cUL))
#define SOC_MEDIA1_CRG_MEDIA_AUTODIV_STAT_ADDR(base) ((base) + (0x190UL))
#define SOC_MEDIA1_CRG_MMBUF_MEM_CTRL_ADDR(base) ((base) + (0x1C0UL))
#define SOC_MEDIA1_CRG_ISPTOMED1_RESERVE_OUT_ADDR(base) ((base) + (0x1C4UL))
#define SOC_MEDIA1_CRG_INTR_QIC_BUS_NONIDLE_PEND_ADDR(base) ((base) + (0x1D0UL))
#define SOC_MEDIA1_CRG_INTR_STAT_QIC_BUS_NONIDLE_PEND_ADDR(base) ((base) + (0x1D4UL))
#define SOC_MEDIA1_CRG_PCR_CG_BYPASS_CTRL0_ADDR(base) ((base) + (0x200UL))
#define SOC_MEDIA1_CRG_PCR_CG_PSTATE1_ISPFUNC3_ADDR(base) ((base) + (0x204UL))
#define SOC_MEDIA1_CRG_PCR_CG_PSTATE2_ISPFUNC3_ADDR(base) ((base) + (0x208UL))
#define SOC_MEDIA1_CRG_PCR_CG_PSTATE3_ISPFUNC3_ADDR(base) ((base) + (0x20CUL))
#define SOC_MEDIA1_CRG_PCR_CG_PSTATE4_ISPFUNC3_ADDR(base) ((base) + (0x210UL))
#define SOC_MEDIA1_CRG_PCR_CG_PSTATE1_ISPFUNC5_ADDR(base) ((base) + (0x214UL))
#define SOC_MEDIA1_CRG_PCR_CG_PSTATE2_ISPFUNC5_ADDR(base) ((base) + (0x218UL))
#define SOC_MEDIA1_CRG_PCR_CG_PSTATE3_ISPFUNC5_ADDR(base) ((base) + (0x21CUL))
#define SOC_MEDIA1_CRG_PCR_CG_PSTATE4_ISPFUNC5_ADDR(base) ((base) + (0x220UL))
#define SOC_MEDIA1_CRG_SC_PLL_CTRL0_ADDR(base) ((base) + (0x240UL))
#define SOC_MEDIA1_CRG_SC_PLL_CTRL_A_ADDR(base) ((base) + (0x244UL))
#define SOC_MEDIA1_CRG_SC_PLL_CTRL_B_ADDR(base) ((base) + (0x248UL))
#define SOC_MEDIA1_CRG_PLL_CLK_TP_SEL_ADDR(base) ((base) + (0x24CUL))
#define SOC_MEDIA1_CRG_ADPLLS_CFG0_ADDR(base) ((base) + (0x260UL))
#define SOC_MEDIA1_CRG_ADPLLS_CFG1_ADDR(base) ((base) + (0x264UL))
#define SOC_MEDIA1_CRG_ADPLLS_CFG2_ADDR(base) ((base) + (0x268UL))
#define SOC_MEDIA1_CRG_ADPLLS_CFG3_ADDR(base) ((base) + (0x26CUL))
#define SOC_MEDIA1_CRG_ADPLLS_CFG4_ADDR(base) ((base) + (0x270UL))
#define SOC_MEDIA1_CRG_ADPLLS_CFG5_ADDR(base) ((base) + (0x274UL))
#define SOC_MEDIA1_CRG_ADPLLS_CFG6_ADDR(base) ((base) + (0x278UL))
#define SOC_MEDIA1_CRG_ADPLLS_STATE0_ADDR(base) ((base) + (0x27CUL))
#define SOC_MEDIA1_CRG_ADPLLS_STATE1_ADDR(base) ((base) + (0x280UL))
#define SOC_MEDIA1_CRG_ADPLLS_STATE2_ADDR(base) ((base) + (0x284UL))
#define SOC_MEDIA1_CRG_QIC_INTR_BUS_ISP_ADDR(base) ((base) + (0x300UL))
#define SOC_MEDIA1_CRG_QIC_INTR_BUS_DSS_ADDR(base) ((base) + (0x304UL))
#define SOC_MEDIA1_CRG_QIC_INTR_BUS0_VIVO_ADDR(base) ((base) + (0x308UL))
#define SOC_MEDIA1_CRG_QIC_INTR_BUS1_VIVO_ADDR(base) ((base) + (0x30CUL))
#define SOC_MEDIA1_CRG_QIC_BUSY_ISP_ADDR(base) ((base) + (0x310UL))
#define SOC_MEDIA1_CRG_QIC_BUSY_DSS_ADDR(base) ((base) + (0x314UL))
#define SOC_MEDIA1_CRG_QIC_BUSY0_VIVO_ADDR(base) ((base) + (0x318UL))
#define SOC_MEDIA1_CRG_QIC_BUSY1_VIVO_ADDR(base) ((base) + (0x31CUL))
#define SOC_MEDIA1_CRG_PERRSTEN_ISP_SEC_ADDR(base) ((base) + (0x800UL))
#define SOC_MEDIA1_CRG_PERRSTDIS_ISP_SEC_ADDR(base) ((base) + (0x804UL))
#define SOC_MEDIA1_CRG_PERRSTSTAT_ISP_SEC_ADDR(base) ((base) + (0x808UL))
#define SOC_MEDIA1_CRG_ISPCPU_CTRL0_SEC_ADDR(base) ((base) + (0x810UL))
#define SOC_MEDIA1_CRG_ISP_ADB_LP_OUT_ADDR(base) ((base) + (0x814UL))
#define SOC_MEDIA1_CRG_ISP_ADB_LP_IN_ADDR(base) ((base) + (0x818UL))
#define SOC_MEDIA1_CRG_PERRSTEN_GENERAL_SEC_ADDR(base) ((base) + (0xA00UL))
#define SOC_MEDIA1_CRG_PERRSTDIS_GENERAL_SEC_ADDR(base) ((base) + (0xA04UL))
#define SOC_MEDIA1_CRG_PERRSTSTAT_GENERAL_SEC_ADDR(base) ((base) + (0xA08UL))
#define SOC_MEDIA1_CRG_PEREN0_GENERAL_SEC_ADDR(base) ((base) + (0xA30UL))
#define SOC_MEDIA1_CRG_PERDIS0_GENERAL_SEC_ADDR(base) ((base) + (0xA34UL))
#define SOC_MEDIA1_CRG_PERCLKEN0_GENERAL_SEC_ADDR(base) ((base) + (0xA38UL))
#define SOC_MEDIA1_CRG_PERSTAT0_GENERAL_SEC_ADDR(base) ((base) + (0xA3CUL))
#define SOC_MEDIA1_CRG_DECPROT0SET_ADDR(base) ((base) + (0xF00UL))
#else
#define SOC_MEDIA1_CRG_PEREN0_ADDR(base) ((base) + (0x000))
#define SOC_MEDIA1_CRG_PERDIS0_ADDR(base) ((base) + (0x004))
#define SOC_MEDIA1_CRG_PERCLKEN0_ADDR(base) ((base) + (0x008))
#define SOC_MEDIA1_CRG_PERSTAT0_ADDR(base) ((base) + (0x00C))
#define SOC_MEDIA1_CRG_PEREN1_ADDR(base) ((base) + (0x010))
#define SOC_MEDIA1_CRG_PERDIS1_ADDR(base) ((base) + (0x014))
#define SOC_MEDIA1_CRG_PERCLKEN1_ADDR(base) ((base) + (0x018))
#define SOC_MEDIA1_CRG_PERSTAT1_ADDR(base) ((base) + (0x01C))
#define SOC_MEDIA1_CRG_PEREN2_ADDR(base) ((base) + (0x020))
#define SOC_MEDIA1_CRG_PERDIS2_ADDR(base) ((base) + (0x024))
#define SOC_MEDIA1_CRG_PERCLKEN2_ADDR(base) ((base) + (0x028))
#define SOC_MEDIA1_CRG_PERSTAT2_ADDR(base) ((base) + (0x02C))
#define SOC_MEDIA1_CRG_PERRSTEN0_ADDR(base) ((base) + (0x030))
#define SOC_MEDIA1_CRG_PERRSTDIS0_ADDR(base) ((base) + (0x034))
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ADDR(base) ((base) + (0x038))
#define SOC_MEDIA1_CRG_PERRSTEN1_ADDR(base) ((base) + (0x03C))
#define SOC_MEDIA1_CRG_PERRSTDIS1_ADDR(base) ((base) + (0x040))
#define SOC_MEDIA1_CRG_PERRSTSTAT1_ADDR(base) ((base) + (0x044))
#define SOC_MEDIA1_CRG_CLKDIV0_ADDR(base) ((base) + (0x060))
#define SOC_MEDIA1_CRG_CLKDIV1_ADDR(base) ((base) + (0x064))
#define SOC_MEDIA1_CRG_CLKDIV2_ADDR(base) ((base) + (0x068))
#define SOC_MEDIA1_CRG_CLKDIV3_ADDR(base) ((base) + (0x06C))
#define SOC_MEDIA1_CRG_CLKDIV4_ADDR(base) ((base) + (0x070))
#define SOC_MEDIA1_CRG_CLKDIV5_ADDR(base) ((base) + (0x074))
#define SOC_MEDIA1_CRG_CLKDIV6_ADDR(base) ((base) + (0x078))
#define SOC_MEDIA1_CRG_CLKDIV7_ADDR(base) ((base) + (0x07C))
#define SOC_MEDIA1_CRG_CLKDIV8_ADDR(base) ((base) + (0x080))
#define SOC_MEDIA1_CRG_CLKDIV9_ADDR(base) ((base) + (0x084))
#define SOC_MEDIA1_CRG_CLKDIV10_ADDR(base) ((base) + (0x088))
#define SOC_MEDIA1_CRG_CLKDIV11_ADDR(base) ((base) + (0x08C))
#define SOC_MEDIA1_CRG_CLKDIV12_ADDR(base) ((base) + (0x090))
#define SOC_MEDIA1_CRG_CLKDIV13_ADDR(base) ((base) + (0x094))
#define SOC_MEDIA1_CRG_CLKDIV15_ADDR(base) ((base) + (0x098))
#define SOC_MEDIA1_CRG_CLKDIV16_ADDR(base) ((base) + (0x09C))
#define SOC_MEDIA1_CRG_PERI_STAT0_ADDR(base) ((base) + (0x0A0))
#define SOC_MEDIA1_CRG_PERI_STAT1_ADDR(base) ((base) + (0x0A4))
#define SOC_MEDIA1_CRG_PERI_STAT2_ADDR(base) ((base) + (0x0A8))
#define SOC_MEDIA1_CRG_ISOEN_ADDR(base) ((base) + (0x100))
#define SOC_MEDIA1_CRG_ISODIS_ADDR(base) ((base) + (0x104))
#define SOC_MEDIA1_CRG_ISOSTAT_ADDR(base) ((base) + (0x108))
#define SOC_MEDIA1_CRG_IPCLKRST_BYPASS_ADDR(base) ((base) + (0x130))
#define SOC_MEDIA1_CRG_ISPCPU_CLKEN_ADDR(base) ((base) + (0x140))
#define SOC_MEDIA1_CRG_ISPCPU_RSTEN_ADDR(base) ((base) + (0x144))
#define SOC_MEDIA1_CRG_ISPCPU_RSTDIS_ADDR(base) ((base) + (0x148))
#define SOC_MEDIA1_CRG_ISPCPU_RSTSTAT_ADDR(base) ((base) + (0x14C))
#define SOC_MEDIA1_CRG_ISP_CPU_STATE0_ADDR(base) ((base) + (0x150))
#define SOC_MEDIA1_CRG_ISP_CPU_STATE1_ADDR(base) ((base) + (0x154))
#define SOC_MEDIA1_CRG_PERI_AUTODIV0_ADDR(base) ((base) + (0x160))
#define SOC_MEDIA1_CRG_PERI_AUTODIV1_ADDR(base) ((base) + (0x164))
#define SOC_MEDIA1_CRG_PERI_AUTODIV2_ADDR(base) ((base) + (0x168))
#define SOC_MEDIA1_CRG_PERI_AUTODIV3_ADDR(base) ((base) + (0x16c))
#define SOC_MEDIA1_CRG_MEDIA_AUTODIV_STAT_ADDR(base) ((base) + (0x190))
#define SOC_MEDIA1_CRG_MMBUF_MEM_CTRL_ADDR(base) ((base) + (0x1C0))
#define SOC_MEDIA1_CRG_ISPTOMED1_RESERVE_OUT_ADDR(base) ((base) + (0x1C4))
#define SOC_MEDIA1_CRG_INTR_QIC_BUS_NONIDLE_PEND_ADDR(base) ((base) + (0x1D0))
#define SOC_MEDIA1_CRG_INTR_STAT_QIC_BUS_NONIDLE_PEND_ADDR(base) ((base) + (0x1D4))
#define SOC_MEDIA1_CRG_PCR_CG_BYPASS_CTRL0_ADDR(base) ((base) + (0x200))
#define SOC_MEDIA1_CRG_PCR_CG_PSTATE1_ISPFUNC3_ADDR(base) ((base) + (0x204))
#define SOC_MEDIA1_CRG_PCR_CG_PSTATE2_ISPFUNC3_ADDR(base) ((base) + (0x208))
#define SOC_MEDIA1_CRG_PCR_CG_PSTATE3_ISPFUNC3_ADDR(base) ((base) + (0x20C))
#define SOC_MEDIA1_CRG_PCR_CG_PSTATE4_ISPFUNC3_ADDR(base) ((base) + (0x210))
#define SOC_MEDIA1_CRG_PCR_CG_PSTATE1_ISPFUNC5_ADDR(base) ((base) + (0x214))
#define SOC_MEDIA1_CRG_PCR_CG_PSTATE2_ISPFUNC5_ADDR(base) ((base) + (0x218))
#define SOC_MEDIA1_CRG_PCR_CG_PSTATE3_ISPFUNC5_ADDR(base) ((base) + (0x21C))
#define SOC_MEDIA1_CRG_PCR_CG_PSTATE4_ISPFUNC5_ADDR(base) ((base) + (0x220))
#define SOC_MEDIA1_CRG_SC_PLL_CTRL0_ADDR(base) ((base) + (0x240))
#define SOC_MEDIA1_CRG_SC_PLL_CTRL_A_ADDR(base) ((base) + (0x244))
#define SOC_MEDIA1_CRG_SC_PLL_CTRL_B_ADDR(base) ((base) + (0x248))
#define SOC_MEDIA1_CRG_PLL_CLK_TP_SEL_ADDR(base) ((base) + (0x24C))
#define SOC_MEDIA1_CRG_ADPLLS_CFG0_ADDR(base) ((base) + (0x260))
#define SOC_MEDIA1_CRG_ADPLLS_CFG1_ADDR(base) ((base) + (0x264))
#define SOC_MEDIA1_CRG_ADPLLS_CFG2_ADDR(base) ((base) + (0x268))
#define SOC_MEDIA1_CRG_ADPLLS_CFG3_ADDR(base) ((base) + (0x26C))
#define SOC_MEDIA1_CRG_ADPLLS_CFG4_ADDR(base) ((base) + (0x270))
#define SOC_MEDIA1_CRG_ADPLLS_CFG5_ADDR(base) ((base) + (0x274))
#define SOC_MEDIA1_CRG_ADPLLS_CFG6_ADDR(base) ((base) + (0x278))
#define SOC_MEDIA1_CRG_ADPLLS_STATE0_ADDR(base) ((base) + (0x27C))
#define SOC_MEDIA1_CRG_ADPLLS_STATE1_ADDR(base) ((base) + (0x280))
#define SOC_MEDIA1_CRG_ADPLLS_STATE2_ADDR(base) ((base) + (0x284))
#define SOC_MEDIA1_CRG_QIC_INTR_BUS_ISP_ADDR(base) ((base) + (0x300))
#define SOC_MEDIA1_CRG_QIC_INTR_BUS_DSS_ADDR(base) ((base) + (0x304))
#define SOC_MEDIA1_CRG_QIC_INTR_BUS0_VIVO_ADDR(base) ((base) + (0x308))
#define SOC_MEDIA1_CRG_QIC_INTR_BUS1_VIVO_ADDR(base) ((base) + (0x30C))
#define SOC_MEDIA1_CRG_QIC_BUSY_ISP_ADDR(base) ((base) + (0x310))
#define SOC_MEDIA1_CRG_QIC_BUSY_DSS_ADDR(base) ((base) + (0x314))
#define SOC_MEDIA1_CRG_QIC_BUSY0_VIVO_ADDR(base) ((base) + (0x318))
#define SOC_MEDIA1_CRG_QIC_BUSY1_VIVO_ADDR(base) ((base) + (0x31C))
#define SOC_MEDIA1_CRG_PERRSTEN_ISP_SEC_ADDR(base) ((base) + (0x800))
#define SOC_MEDIA1_CRG_PERRSTDIS_ISP_SEC_ADDR(base) ((base) + (0x804))
#define SOC_MEDIA1_CRG_PERRSTSTAT_ISP_SEC_ADDR(base) ((base) + (0x808))
#define SOC_MEDIA1_CRG_ISPCPU_CTRL0_SEC_ADDR(base) ((base) + (0x810))
#define SOC_MEDIA1_CRG_ISP_ADB_LP_OUT_ADDR(base) ((base) + (0x814))
#define SOC_MEDIA1_CRG_ISP_ADB_LP_IN_ADDR(base) ((base) + (0x818))
#define SOC_MEDIA1_CRG_PERRSTEN_GENERAL_SEC_ADDR(base) ((base) + (0xA00))
#define SOC_MEDIA1_CRG_PERRSTDIS_GENERAL_SEC_ADDR(base) ((base) + (0xA04))
#define SOC_MEDIA1_CRG_PERRSTSTAT_GENERAL_SEC_ADDR(base) ((base) + (0xA08))
#define SOC_MEDIA1_CRG_PEREN0_GENERAL_SEC_ADDR(base) ((base) + (0xA30))
#define SOC_MEDIA1_CRG_PERDIS0_GENERAL_SEC_ADDR(base) ((base) + (0xA34))
#define SOC_MEDIA1_CRG_PERCLKEN0_GENERAL_SEC_ADDR(base) ((base) + (0xA38))
#define SOC_MEDIA1_CRG_PERSTAT0_GENERAL_SEC_ADDR(base) ((base) + (0xA3C))
#define SOC_MEDIA1_CRG_DECPROT0SET_ADDR(base) ((base) + (0xF00))
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int gt_atclk_trace : 1;
        unsigned int gt_pclk_dbg_trace : 1;
        unsigned int gt_pclk_media1_lm_lpm3 : 1;
        unsigned int gt_clk_ispfunc4 : 1;
        unsigned int reserved_2 : 1;
        unsigned int gt_pclk_media1_lm : 1;
        unsigned int reserved_3 : 1;
        unsigned int gt_clk_isp_i2c : 1;
        unsigned int reserved_4 : 1;
        unsigned int gt_clk_isp_sys : 1;
        unsigned int gt_clk_isp_cpu : 1;
        unsigned int gt_clk_ispfunc : 1;
        unsigned int gt_pclk_dss : 1;
        unsigned int gt_clk_edc0 : 1;
        unsigned int gt_clk_ispfunc5 : 1;
        unsigned int gt_clk_ldi1 : 1;
        unsigned int gt_clk_vivobus : 1;
        unsigned int gt_aclk_dss : 1;
        unsigned int gt_aclk_isp : 1;
        unsigned int gt_clk_ispfunc3 : 1;
        unsigned int gt_clk_ispfunc2 : 1;
        unsigned int gt_pclk_atgs_vivo : 1;
        unsigned int gt_pclk_atgm_dss : 1;
        unsigned int gt_pclk_atgm_isp : 1;
        unsigned int gt_clk_loadmonitor : 1;
        unsigned int gt_clk_dmss_vivo : 1;
        unsigned int gt_clk_tcu_vivo : 1;
        unsigned int gt_clk_brg : 1;
        unsigned int gt_clk_isp_cputocfg : 1;
        unsigned int reserved_5 : 1;
    } reg;
} SOC_MEDIA1_CRG_PEREN0_UNION;
#endif
#define SOC_MEDIA1_CRG_PEREN0_gt_atclk_trace_START (2)
#define SOC_MEDIA1_CRG_PEREN0_gt_atclk_trace_END (2)
#define SOC_MEDIA1_CRG_PEREN0_gt_pclk_dbg_trace_START (3)
#define SOC_MEDIA1_CRG_PEREN0_gt_pclk_dbg_trace_END (3)
#define SOC_MEDIA1_CRG_PEREN0_gt_pclk_media1_lm_lpm3_START (4)
#define SOC_MEDIA1_CRG_PEREN0_gt_pclk_media1_lm_lpm3_END (4)
#define SOC_MEDIA1_CRG_PEREN0_gt_clk_ispfunc4_START (5)
#define SOC_MEDIA1_CRG_PEREN0_gt_clk_ispfunc4_END (5)
#define SOC_MEDIA1_CRG_PEREN0_gt_pclk_media1_lm_START (7)
#define SOC_MEDIA1_CRG_PEREN0_gt_pclk_media1_lm_END (7)
#define SOC_MEDIA1_CRG_PEREN0_gt_clk_isp_i2c_START (9)
#define SOC_MEDIA1_CRG_PEREN0_gt_clk_isp_i2c_END (9)
#define SOC_MEDIA1_CRG_PEREN0_gt_clk_isp_sys_START (11)
#define SOC_MEDIA1_CRG_PEREN0_gt_clk_isp_sys_END (11)
#define SOC_MEDIA1_CRG_PEREN0_gt_clk_isp_cpu_START (12)
#define SOC_MEDIA1_CRG_PEREN0_gt_clk_isp_cpu_END (12)
#define SOC_MEDIA1_CRG_PEREN0_gt_clk_ispfunc_START (13)
#define SOC_MEDIA1_CRG_PEREN0_gt_clk_ispfunc_END (13)
#define SOC_MEDIA1_CRG_PEREN0_gt_pclk_dss_START (14)
#define SOC_MEDIA1_CRG_PEREN0_gt_pclk_dss_END (14)
#define SOC_MEDIA1_CRG_PEREN0_gt_clk_edc0_START (15)
#define SOC_MEDIA1_CRG_PEREN0_gt_clk_edc0_END (15)
#define SOC_MEDIA1_CRG_PEREN0_gt_clk_ispfunc5_START (16)
#define SOC_MEDIA1_CRG_PEREN0_gt_clk_ispfunc5_END (16)
#define SOC_MEDIA1_CRG_PEREN0_gt_clk_ldi1_START (17)
#define SOC_MEDIA1_CRG_PEREN0_gt_clk_ldi1_END (17)
#define SOC_MEDIA1_CRG_PEREN0_gt_clk_vivobus_START (18)
#define SOC_MEDIA1_CRG_PEREN0_gt_clk_vivobus_END (18)
#define SOC_MEDIA1_CRG_PEREN0_gt_aclk_dss_START (19)
#define SOC_MEDIA1_CRG_PEREN0_gt_aclk_dss_END (19)
#define SOC_MEDIA1_CRG_PEREN0_gt_aclk_isp_START (20)
#define SOC_MEDIA1_CRG_PEREN0_gt_aclk_isp_END (20)
#define SOC_MEDIA1_CRG_PEREN0_gt_clk_ispfunc3_START (21)
#define SOC_MEDIA1_CRG_PEREN0_gt_clk_ispfunc3_END (21)
#define SOC_MEDIA1_CRG_PEREN0_gt_clk_ispfunc2_START (22)
#define SOC_MEDIA1_CRG_PEREN0_gt_clk_ispfunc2_END (22)
#define SOC_MEDIA1_CRG_PEREN0_gt_pclk_atgs_vivo_START (23)
#define SOC_MEDIA1_CRG_PEREN0_gt_pclk_atgs_vivo_END (23)
#define SOC_MEDIA1_CRG_PEREN0_gt_pclk_atgm_dss_START (24)
#define SOC_MEDIA1_CRG_PEREN0_gt_pclk_atgm_dss_END (24)
#define SOC_MEDIA1_CRG_PEREN0_gt_pclk_atgm_isp_START (25)
#define SOC_MEDIA1_CRG_PEREN0_gt_pclk_atgm_isp_END (25)
#define SOC_MEDIA1_CRG_PEREN0_gt_clk_loadmonitor_START (26)
#define SOC_MEDIA1_CRG_PEREN0_gt_clk_loadmonitor_END (26)
#define SOC_MEDIA1_CRG_PEREN0_gt_clk_dmss_vivo_START (27)
#define SOC_MEDIA1_CRG_PEREN0_gt_clk_dmss_vivo_END (27)
#define SOC_MEDIA1_CRG_PEREN0_gt_clk_tcu_vivo_START (28)
#define SOC_MEDIA1_CRG_PEREN0_gt_clk_tcu_vivo_END (28)
#define SOC_MEDIA1_CRG_PEREN0_gt_clk_brg_START (29)
#define SOC_MEDIA1_CRG_PEREN0_gt_clk_brg_END (29)
#define SOC_MEDIA1_CRG_PEREN0_gt_clk_isp_cputocfg_START (30)
#define SOC_MEDIA1_CRG_PEREN0_gt_clk_isp_cputocfg_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int gt_atclk_trace : 1;
        unsigned int gt_pclk_dbg_trace : 1;
        unsigned int gt_pclk_media1_lm_lpm3 : 1;
        unsigned int gt_clk_ispfunc4 : 1;
        unsigned int reserved_2 : 1;
        unsigned int gt_pclk_media1_lm : 1;
        unsigned int reserved_3 : 1;
        unsigned int gt_clk_isp_i2c : 1;
        unsigned int reserved_4 : 1;
        unsigned int gt_clk_isp_sys : 1;
        unsigned int gt_clk_isp_cpu : 1;
        unsigned int gt_clk_ispfunc : 1;
        unsigned int gt_pclk_dss : 1;
        unsigned int gt_clk_edc0 : 1;
        unsigned int gt_clk_ispfunc5 : 1;
        unsigned int gt_clk_ldi1 : 1;
        unsigned int gt_clk_vivobus : 1;
        unsigned int gt_aclk_dss : 1;
        unsigned int gt_aclk_isp : 1;
        unsigned int gt_clk_ispfunc3 : 1;
        unsigned int gt_clk_ispfunc2 : 1;
        unsigned int gt_pclk_atgs_vivo : 1;
        unsigned int gt_pclk_atgm_dss : 1;
        unsigned int gt_pclk_atgm_isp : 1;
        unsigned int gt_clk_loadmonitor : 1;
        unsigned int gt_clk_dmss_vivo : 1;
        unsigned int gt_clk_tcu_vivo : 1;
        unsigned int gt_clk_brg : 1;
        unsigned int gt_clk_isp_cputocfg : 1;
        unsigned int reserved_5 : 1;
    } reg;
} SOC_MEDIA1_CRG_PERDIS0_UNION;
#endif
#define SOC_MEDIA1_CRG_PERDIS0_gt_atclk_trace_START (2)
#define SOC_MEDIA1_CRG_PERDIS0_gt_atclk_trace_END (2)
#define SOC_MEDIA1_CRG_PERDIS0_gt_pclk_dbg_trace_START (3)
#define SOC_MEDIA1_CRG_PERDIS0_gt_pclk_dbg_trace_END (3)
#define SOC_MEDIA1_CRG_PERDIS0_gt_pclk_media1_lm_lpm3_START (4)
#define SOC_MEDIA1_CRG_PERDIS0_gt_pclk_media1_lm_lpm3_END (4)
#define SOC_MEDIA1_CRG_PERDIS0_gt_clk_ispfunc4_START (5)
#define SOC_MEDIA1_CRG_PERDIS0_gt_clk_ispfunc4_END (5)
#define SOC_MEDIA1_CRG_PERDIS0_gt_pclk_media1_lm_START (7)
#define SOC_MEDIA1_CRG_PERDIS0_gt_pclk_media1_lm_END (7)
#define SOC_MEDIA1_CRG_PERDIS0_gt_clk_isp_i2c_START (9)
#define SOC_MEDIA1_CRG_PERDIS0_gt_clk_isp_i2c_END (9)
#define SOC_MEDIA1_CRG_PERDIS0_gt_clk_isp_sys_START (11)
#define SOC_MEDIA1_CRG_PERDIS0_gt_clk_isp_sys_END (11)
#define SOC_MEDIA1_CRG_PERDIS0_gt_clk_isp_cpu_START (12)
#define SOC_MEDIA1_CRG_PERDIS0_gt_clk_isp_cpu_END (12)
#define SOC_MEDIA1_CRG_PERDIS0_gt_clk_ispfunc_START (13)
#define SOC_MEDIA1_CRG_PERDIS0_gt_clk_ispfunc_END (13)
#define SOC_MEDIA1_CRG_PERDIS0_gt_pclk_dss_START (14)
#define SOC_MEDIA1_CRG_PERDIS0_gt_pclk_dss_END (14)
#define SOC_MEDIA1_CRG_PERDIS0_gt_clk_edc0_START (15)
#define SOC_MEDIA1_CRG_PERDIS0_gt_clk_edc0_END (15)
#define SOC_MEDIA1_CRG_PERDIS0_gt_clk_ispfunc5_START (16)
#define SOC_MEDIA1_CRG_PERDIS0_gt_clk_ispfunc5_END (16)
#define SOC_MEDIA1_CRG_PERDIS0_gt_clk_ldi1_START (17)
#define SOC_MEDIA1_CRG_PERDIS0_gt_clk_ldi1_END (17)
#define SOC_MEDIA1_CRG_PERDIS0_gt_clk_vivobus_START (18)
#define SOC_MEDIA1_CRG_PERDIS0_gt_clk_vivobus_END (18)
#define SOC_MEDIA1_CRG_PERDIS0_gt_aclk_dss_START (19)
#define SOC_MEDIA1_CRG_PERDIS0_gt_aclk_dss_END (19)
#define SOC_MEDIA1_CRG_PERDIS0_gt_aclk_isp_START (20)
#define SOC_MEDIA1_CRG_PERDIS0_gt_aclk_isp_END (20)
#define SOC_MEDIA1_CRG_PERDIS0_gt_clk_ispfunc3_START (21)
#define SOC_MEDIA1_CRG_PERDIS0_gt_clk_ispfunc3_END (21)
#define SOC_MEDIA1_CRG_PERDIS0_gt_clk_ispfunc2_START (22)
#define SOC_MEDIA1_CRG_PERDIS0_gt_clk_ispfunc2_END (22)
#define SOC_MEDIA1_CRG_PERDIS0_gt_pclk_atgs_vivo_START (23)
#define SOC_MEDIA1_CRG_PERDIS0_gt_pclk_atgs_vivo_END (23)
#define SOC_MEDIA1_CRG_PERDIS0_gt_pclk_atgm_dss_START (24)
#define SOC_MEDIA1_CRG_PERDIS0_gt_pclk_atgm_dss_END (24)
#define SOC_MEDIA1_CRG_PERDIS0_gt_pclk_atgm_isp_START (25)
#define SOC_MEDIA1_CRG_PERDIS0_gt_pclk_atgm_isp_END (25)
#define SOC_MEDIA1_CRG_PERDIS0_gt_clk_loadmonitor_START (26)
#define SOC_MEDIA1_CRG_PERDIS0_gt_clk_loadmonitor_END (26)
#define SOC_MEDIA1_CRG_PERDIS0_gt_clk_dmss_vivo_START (27)
#define SOC_MEDIA1_CRG_PERDIS0_gt_clk_dmss_vivo_END (27)
#define SOC_MEDIA1_CRG_PERDIS0_gt_clk_tcu_vivo_START (28)
#define SOC_MEDIA1_CRG_PERDIS0_gt_clk_tcu_vivo_END (28)
#define SOC_MEDIA1_CRG_PERDIS0_gt_clk_brg_START (29)
#define SOC_MEDIA1_CRG_PERDIS0_gt_clk_brg_END (29)
#define SOC_MEDIA1_CRG_PERDIS0_gt_clk_isp_cputocfg_START (30)
#define SOC_MEDIA1_CRG_PERDIS0_gt_clk_isp_cputocfg_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int gt_atclk_trace : 1;
        unsigned int gt_pclk_dbg_trace : 1;
        unsigned int gt_pclk_media1_lm_lpm3 : 1;
        unsigned int gt_clk_ispfunc4 : 1;
        unsigned int reserved_2 : 1;
        unsigned int gt_pclk_media1_lm : 1;
        unsigned int reserved_3 : 1;
        unsigned int gt_clk_isp_i2c : 1;
        unsigned int reserved_4 : 1;
        unsigned int gt_clk_isp_sys : 1;
        unsigned int gt_clk_isp_cpu : 1;
        unsigned int gt_clk_ispfunc : 1;
        unsigned int gt_pclk_dss : 1;
        unsigned int gt_clk_edc0 : 1;
        unsigned int gt_clk_ispfunc5 : 1;
        unsigned int gt_clk_ldi1 : 1;
        unsigned int gt_clk_vivobus : 1;
        unsigned int gt_aclk_dss : 1;
        unsigned int gt_aclk_isp : 1;
        unsigned int gt_clk_ispfunc3 : 1;
        unsigned int gt_clk_ispfunc2 : 1;
        unsigned int gt_pclk_atgs_vivo : 1;
        unsigned int gt_pclk_atgm_dss : 1;
        unsigned int gt_pclk_atgm_isp : 1;
        unsigned int gt_clk_loadmonitor : 1;
        unsigned int gt_clk_dmss_vivo : 1;
        unsigned int gt_clk_tcu_vivo : 1;
        unsigned int gt_clk_brg : 1;
        unsigned int gt_clk_isp_cputocfg : 1;
        unsigned int reserved_5 : 1;
    } reg;
} SOC_MEDIA1_CRG_PERCLKEN0_UNION;
#endif
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_atclk_trace_START (2)
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_atclk_trace_END (2)
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_pclk_dbg_trace_START (3)
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_pclk_dbg_trace_END (3)
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_pclk_media1_lm_lpm3_START (4)
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_pclk_media1_lm_lpm3_END (4)
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_clk_ispfunc4_START (5)
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_clk_ispfunc4_END (5)
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_pclk_media1_lm_START (7)
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_pclk_media1_lm_END (7)
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_clk_isp_i2c_START (9)
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_clk_isp_i2c_END (9)
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_clk_isp_sys_START (11)
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_clk_isp_sys_END (11)
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_clk_isp_cpu_START (12)
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_clk_isp_cpu_END (12)
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_clk_ispfunc_START (13)
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_clk_ispfunc_END (13)
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_pclk_dss_START (14)
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_pclk_dss_END (14)
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_clk_edc0_START (15)
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_clk_edc0_END (15)
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_clk_ispfunc5_START (16)
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_clk_ispfunc5_END (16)
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_clk_ldi1_START (17)
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_clk_ldi1_END (17)
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_clk_vivobus_START (18)
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_clk_vivobus_END (18)
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_aclk_dss_START (19)
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_aclk_dss_END (19)
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_aclk_isp_START (20)
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_aclk_isp_END (20)
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_clk_ispfunc3_START (21)
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_clk_ispfunc3_END (21)
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_clk_ispfunc2_START (22)
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_clk_ispfunc2_END (22)
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_pclk_atgs_vivo_START (23)
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_pclk_atgs_vivo_END (23)
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_pclk_atgm_dss_START (24)
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_pclk_atgm_dss_END (24)
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_pclk_atgm_isp_START (25)
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_pclk_atgm_isp_END (25)
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_clk_loadmonitor_START (26)
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_clk_loadmonitor_END (26)
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_clk_dmss_vivo_START (27)
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_clk_dmss_vivo_END (27)
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_clk_tcu_vivo_START (28)
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_clk_tcu_vivo_END (28)
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_clk_brg_START (29)
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_clk_brg_END (29)
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_clk_isp_cputocfg_START (30)
#define SOC_MEDIA1_CRG_PERCLKEN0_gt_clk_isp_cputocfg_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int st_atclk_trace : 1;
        unsigned int st_pclk_dbg_trace : 1;
        unsigned int st_pclk_media1_lm_lpm3 : 1;
        unsigned int st_clk_ispfunc4 : 1;
        unsigned int reserved_2 : 1;
        unsigned int st_pclk_media1_lm : 1;
        unsigned int reserved_3 : 1;
        unsigned int st_clk_isp_i2c : 1;
        unsigned int reserved_4 : 1;
        unsigned int st_clk_isp_sys : 1;
        unsigned int st_clk_isp_cpu : 1;
        unsigned int st_clk_ispfunc : 1;
        unsigned int st_pclk_dss : 1;
        unsigned int st_clk_edc0 : 1;
        unsigned int st_clk_ispfunc5 : 1;
        unsigned int st_clk_ldi1 : 1;
        unsigned int st_clk_vivobus : 1;
        unsigned int st_aclk_dss : 1;
        unsigned int st_aclk_isp : 1;
        unsigned int st_clk_ispfunc3 : 1;
        unsigned int st_clk_ispfunc2 : 1;
        unsigned int st_pclk_atgs_vivo : 1;
        unsigned int st_pclk_atgm_dss : 1;
        unsigned int st_pclk_atgm_isp : 1;
        unsigned int st_clk_loadmonitor : 1;
        unsigned int st_clk_dmss_vivo : 1;
        unsigned int st_clk_tcu_vivo : 1;
        unsigned int st_clk_brg : 1;
        unsigned int st_clk_isp_cputocfg : 1;
        unsigned int reserved_5 : 1;
    } reg;
} SOC_MEDIA1_CRG_PERSTAT0_UNION;
#endif
#define SOC_MEDIA1_CRG_PERSTAT0_st_atclk_trace_START (2)
#define SOC_MEDIA1_CRG_PERSTAT0_st_atclk_trace_END (2)
#define SOC_MEDIA1_CRG_PERSTAT0_st_pclk_dbg_trace_START (3)
#define SOC_MEDIA1_CRG_PERSTAT0_st_pclk_dbg_trace_END (3)
#define SOC_MEDIA1_CRG_PERSTAT0_st_pclk_media1_lm_lpm3_START (4)
#define SOC_MEDIA1_CRG_PERSTAT0_st_pclk_media1_lm_lpm3_END (4)
#define SOC_MEDIA1_CRG_PERSTAT0_st_clk_ispfunc4_START (5)
#define SOC_MEDIA1_CRG_PERSTAT0_st_clk_ispfunc4_END (5)
#define SOC_MEDIA1_CRG_PERSTAT0_st_pclk_media1_lm_START (7)
#define SOC_MEDIA1_CRG_PERSTAT0_st_pclk_media1_lm_END (7)
#define SOC_MEDIA1_CRG_PERSTAT0_st_clk_isp_i2c_START (9)
#define SOC_MEDIA1_CRG_PERSTAT0_st_clk_isp_i2c_END (9)
#define SOC_MEDIA1_CRG_PERSTAT0_st_clk_isp_sys_START (11)
#define SOC_MEDIA1_CRG_PERSTAT0_st_clk_isp_sys_END (11)
#define SOC_MEDIA1_CRG_PERSTAT0_st_clk_isp_cpu_START (12)
#define SOC_MEDIA1_CRG_PERSTAT0_st_clk_isp_cpu_END (12)
#define SOC_MEDIA1_CRG_PERSTAT0_st_clk_ispfunc_START (13)
#define SOC_MEDIA1_CRG_PERSTAT0_st_clk_ispfunc_END (13)
#define SOC_MEDIA1_CRG_PERSTAT0_st_pclk_dss_START (14)
#define SOC_MEDIA1_CRG_PERSTAT0_st_pclk_dss_END (14)
#define SOC_MEDIA1_CRG_PERSTAT0_st_clk_edc0_START (15)
#define SOC_MEDIA1_CRG_PERSTAT0_st_clk_edc0_END (15)
#define SOC_MEDIA1_CRG_PERSTAT0_st_clk_ispfunc5_START (16)
#define SOC_MEDIA1_CRG_PERSTAT0_st_clk_ispfunc5_END (16)
#define SOC_MEDIA1_CRG_PERSTAT0_st_clk_ldi1_START (17)
#define SOC_MEDIA1_CRG_PERSTAT0_st_clk_ldi1_END (17)
#define SOC_MEDIA1_CRG_PERSTAT0_st_clk_vivobus_START (18)
#define SOC_MEDIA1_CRG_PERSTAT0_st_clk_vivobus_END (18)
#define SOC_MEDIA1_CRG_PERSTAT0_st_aclk_dss_START (19)
#define SOC_MEDIA1_CRG_PERSTAT0_st_aclk_dss_END (19)
#define SOC_MEDIA1_CRG_PERSTAT0_st_aclk_isp_START (20)
#define SOC_MEDIA1_CRG_PERSTAT0_st_aclk_isp_END (20)
#define SOC_MEDIA1_CRG_PERSTAT0_st_clk_ispfunc3_START (21)
#define SOC_MEDIA1_CRG_PERSTAT0_st_clk_ispfunc3_END (21)
#define SOC_MEDIA1_CRG_PERSTAT0_st_clk_ispfunc2_START (22)
#define SOC_MEDIA1_CRG_PERSTAT0_st_clk_ispfunc2_END (22)
#define SOC_MEDIA1_CRG_PERSTAT0_st_pclk_atgs_vivo_START (23)
#define SOC_MEDIA1_CRG_PERSTAT0_st_pclk_atgs_vivo_END (23)
#define SOC_MEDIA1_CRG_PERSTAT0_st_pclk_atgm_dss_START (24)
#define SOC_MEDIA1_CRG_PERSTAT0_st_pclk_atgm_dss_END (24)
#define SOC_MEDIA1_CRG_PERSTAT0_st_pclk_atgm_isp_START (25)
#define SOC_MEDIA1_CRG_PERSTAT0_st_pclk_atgm_isp_END (25)
#define SOC_MEDIA1_CRG_PERSTAT0_st_clk_loadmonitor_START (26)
#define SOC_MEDIA1_CRG_PERSTAT0_st_clk_loadmonitor_END (26)
#define SOC_MEDIA1_CRG_PERSTAT0_st_clk_dmss_vivo_START (27)
#define SOC_MEDIA1_CRG_PERSTAT0_st_clk_dmss_vivo_END (27)
#define SOC_MEDIA1_CRG_PERSTAT0_st_clk_tcu_vivo_START (28)
#define SOC_MEDIA1_CRG_PERSTAT0_st_clk_tcu_vivo_END (28)
#define SOC_MEDIA1_CRG_PERSTAT0_st_clk_brg_START (29)
#define SOC_MEDIA1_CRG_PERSTAT0_st_clk_brg_END (29)
#define SOC_MEDIA1_CRG_PERSTAT0_st_clk_isp_cputocfg_START (30)
#define SOC_MEDIA1_CRG_PERSTAT0_st_clk_isp_cputocfg_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int gt_pclk_autodiv_vivobus : 1;
        unsigned int reserved_1 : 1;
        unsigned int gt_clk_qic_idi2axi : 1;
        unsigned int gt_pclk_autodiv_ispcpu : 1;
        unsigned int reserved_2 : 1;
        unsigned int gt_pclk_csi_wrapper_cfg : 1;
        unsigned int reserved_3 : 1;
        unsigned int gt_clk_isp_i3c : 1;
        unsigned int gt_clk_isp_ref32k : 1;
        unsigned int gt_clk_dss_ref32k : 1;
        unsigned int gt_clk_vivobus_ref32k : 1;
        unsigned int gt_clk_media_common : 1;
        unsigned int gt_clk_media_common_core : 1;
        unsigned int gt_clk_media_common_p2p_rx : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int gt_aclk_disp_qic_subsys : 1;
        unsigned int gt_pclk_disp_qic_subsys : 1;
        unsigned int gt_aclk_isp_qic_subsys : 1;
        unsigned int gt_pclk_isp_qic_subsys : 1;
        unsigned int gt_aclk_qic_dss : 1;
        unsigned int gt_pclk_qic_dss : 1;
        unsigned int gt_aclk_media_common : 1;
        unsigned int gt_pclk_media_common : 1;
        unsigned int gt_aclk_qic_isp : 1;
        unsigned int gt_pclk_qic_isp : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int reserved_8 : 3;
    } reg;
} SOC_MEDIA1_CRG_PEREN1_UNION;
#endif
#define SOC_MEDIA1_CRG_PEREN1_gt_pclk_autodiv_vivobus_START (1)
#define SOC_MEDIA1_CRG_PEREN1_gt_pclk_autodiv_vivobus_END (1)
#define SOC_MEDIA1_CRG_PEREN1_gt_clk_qic_idi2axi_START (3)
#define SOC_MEDIA1_CRG_PEREN1_gt_clk_qic_idi2axi_END (3)
#define SOC_MEDIA1_CRG_PEREN1_gt_pclk_autodiv_ispcpu_START (4)
#define SOC_MEDIA1_CRG_PEREN1_gt_pclk_autodiv_ispcpu_END (4)
#define SOC_MEDIA1_CRG_PEREN1_gt_pclk_csi_wrapper_cfg_START (6)
#define SOC_MEDIA1_CRG_PEREN1_gt_pclk_csi_wrapper_cfg_END (6)
#define SOC_MEDIA1_CRG_PEREN1_gt_clk_isp_i3c_START (8)
#define SOC_MEDIA1_CRG_PEREN1_gt_clk_isp_i3c_END (8)
#define SOC_MEDIA1_CRG_PEREN1_gt_clk_isp_ref32k_START (9)
#define SOC_MEDIA1_CRG_PEREN1_gt_clk_isp_ref32k_END (9)
#define SOC_MEDIA1_CRG_PEREN1_gt_clk_dss_ref32k_START (10)
#define SOC_MEDIA1_CRG_PEREN1_gt_clk_dss_ref32k_END (10)
#define SOC_MEDIA1_CRG_PEREN1_gt_clk_vivobus_ref32k_START (11)
#define SOC_MEDIA1_CRG_PEREN1_gt_clk_vivobus_ref32k_END (11)
#define SOC_MEDIA1_CRG_PEREN1_gt_clk_media_common_START (12)
#define SOC_MEDIA1_CRG_PEREN1_gt_clk_media_common_END (12)
#define SOC_MEDIA1_CRG_PEREN1_gt_clk_media_common_core_START (13)
#define SOC_MEDIA1_CRG_PEREN1_gt_clk_media_common_core_END (13)
#define SOC_MEDIA1_CRG_PEREN1_gt_clk_media_common_p2p_rx_START (14)
#define SOC_MEDIA1_CRG_PEREN1_gt_clk_media_common_p2p_rx_END (14)
#define SOC_MEDIA1_CRG_PEREN1_gt_aclk_disp_qic_subsys_START (17)
#define SOC_MEDIA1_CRG_PEREN1_gt_aclk_disp_qic_subsys_END (17)
#define SOC_MEDIA1_CRG_PEREN1_gt_pclk_disp_qic_subsys_START (18)
#define SOC_MEDIA1_CRG_PEREN1_gt_pclk_disp_qic_subsys_END (18)
#define SOC_MEDIA1_CRG_PEREN1_gt_aclk_isp_qic_subsys_START (19)
#define SOC_MEDIA1_CRG_PEREN1_gt_aclk_isp_qic_subsys_END (19)
#define SOC_MEDIA1_CRG_PEREN1_gt_pclk_isp_qic_subsys_START (20)
#define SOC_MEDIA1_CRG_PEREN1_gt_pclk_isp_qic_subsys_END (20)
#define SOC_MEDIA1_CRG_PEREN1_gt_aclk_qic_dss_START (21)
#define SOC_MEDIA1_CRG_PEREN1_gt_aclk_qic_dss_END (21)
#define SOC_MEDIA1_CRG_PEREN1_gt_pclk_qic_dss_START (22)
#define SOC_MEDIA1_CRG_PEREN1_gt_pclk_qic_dss_END (22)
#define SOC_MEDIA1_CRG_PEREN1_gt_aclk_media_common_START (23)
#define SOC_MEDIA1_CRG_PEREN1_gt_aclk_media_common_END (23)
#define SOC_MEDIA1_CRG_PEREN1_gt_pclk_media_common_START (24)
#define SOC_MEDIA1_CRG_PEREN1_gt_pclk_media_common_END (24)
#define SOC_MEDIA1_CRG_PEREN1_gt_aclk_qic_isp_START (25)
#define SOC_MEDIA1_CRG_PEREN1_gt_aclk_qic_isp_END (25)
#define SOC_MEDIA1_CRG_PEREN1_gt_pclk_qic_isp_START (26)
#define SOC_MEDIA1_CRG_PEREN1_gt_pclk_qic_isp_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int gt_pclk_autodiv_vivobus : 1;
        unsigned int reserved_1 : 1;
        unsigned int gt_clk_qic_idi2axi : 1;
        unsigned int gt_pclk_autodiv_ispcpu : 1;
        unsigned int reserved_2 : 1;
        unsigned int gt_pclk_csi_wrapper_cfg : 1;
        unsigned int reserved_3 : 1;
        unsigned int gt_clk_isp_i3c : 1;
        unsigned int gt_clk_isp_ref32k : 1;
        unsigned int gt_clk_dss_ref32k : 1;
        unsigned int gt_clk_vivobus_ref32k : 1;
        unsigned int gt_clk_media_common : 1;
        unsigned int gt_clk_media_common_core : 1;
        unsigned int gt_clk_media_common_p2p_rx : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int gt_aclk_disp_qic_subsys : 1;
        unsigned int gt_pclk_disp_qic_subsys : 1;
        unsigned int gt_aclk_isp_qic_subsys : 1;
        unsigned int gt_pclk_isp_qic_subsys : 1;
        unsigned int gt_aclk_qic_dss : 1;
        unsigned int gt_pclk_qic_dss : 1;
        unsigned int gt_aclk_media_common : 1;
        unsigned int gt_pclk_media_common : 1;
        unsigned int gt_aclk_qic_isp : 1;
        unsigned int gt_pclk_qic_isp : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int reserved_8 : 3;
    } reg;
} SOC_MEDIA1_CRG_PERDIS1_UNION;
#endif
#define SOC_MEDIA1_CRG_PERDIS1_gt_pclk_autodiv_vivobus_START (1)
#define SOC_MEDIA1_CRG_PERDIS1_gt_pclk_autodiv_vivobus_END (1)
#define SOC_MEDIA1_CRG_PERDIS1_gt_clk_qic_idi2axi_START (3)
#define SOC_MEDIA1_CRG_PERDIS1_gt_clk_qic_idi2axi_END (3)
#define SOC_MEDIA1_CRG_PERDIS1_gt_pclk_autodiv_ispcpu_START (4)
#define SOC_MEDIA1_CRG_PERDIS1_gt_pclk_autodiv_ispcpu_END (4)
#define SOC_MEDIA1_CRG_PERDIS1_gt_pclk_csi_wrapper_cfg_START (6)
#define SOC_MEDIA1_CRG_PERDIS1_gt_pclk_csi_wrapper_cfg_END (6)
#define SOC_MEDIA1_CRG_PERDIS1_gt_clk_isp_i3c_START (8)
#define SOC_MEDIA1_CRG_PERDIS1_gt_clk_isp_i3c_END (8)
#define SOC_MEDIA1_CRG_PERDIS1_gt_clk_isp_ref32k_START (9)
#define SOC_MEDIA1_CRG_PERDIS1_gt_clk_isp_ref32k_END (9)
#define SOC_MEDIA1_CRG_PERDIS1_gt_clk_dss_ref32k_START (10)
#define SOC_MEDIA1_CRG_PERDIS1_gt_clk_dss_ref32k_END (10)
#define SOC_MEDIA1_CRG_PERDIS1_gt_clk_vivobus_ref32k_START (11)
#define SOC_MEDIA1_CRG_PERDIS1_gt_clk_vivobus_ref32k_END (11)
#define SOC_MEDIA1_CRG_PERDIS1_gt_clk_media_common_START (12)
#define SOC_MEDIA1_CRG_PERDIS1_gt_clk_media_common_END (12)
#define SOC_MEDIA1_CRG_PERDIS1_gt_clk_media_common_core_START (13)
#define SOC_MEDIA1_CRG_PERDIS1_gt_clk_media_common_core_END (13)
#define SOC_MEDIA1_CRG_PERDIS1_gt_clk_media_common_p2p_rx_START (14)
#define SOC_MEDIA1_CRG_PERDIS1_gt_clk_media_common_p2p_rx_END (14)
#define SOC_MEDIA1_CRG_PERDIS1_gt_aclk_disp_qic_subsys_START (17)
#define SOC_MEDIA1_CRG_PERDIS1_gt_aclk_disp_qic_subsys_END (17)
#define SOC_MEDIA1_CRG_PERDIS1_gt_pclk_disp_qic_subsys_START (18)
#define SOC_MEDIA1_CRG_PERDIS1_gt_pclk_disp_qic_subsys_END (18)
#define SOC_MEDIA1_CRG_PERDIS1_gt_aclk_isp_qic_subsys_START (19)
#define SOC_MEDIA1_CRG_PERDIS1_gt_aclk_isp_qic_subsys_END (19)
#define SOC_MEDIA1_CRG_PERDIS1_gt_pclk_isp_qic_subsys_START (20)
#define SOC_MEDIA1_CRG_PERDIS1_gt_pclk_isp_qic_subsys_END (20)
#define SOC_MEDIA1_CRG_PERDIS1_gt_aclk_qic_dss_START (21)
#define SOC_MEDIA1_CRG_PERDIS1_gt_aclk_qic_dss_END (21)
#define SOC_MEDIA1_CRG_PERDIS1_gt_pclk_qic_dss_START (22)
#define SOC_MEDIA1_CRG_PERDIS1_gt_pclk_qic_dss_END (22)
#define SOC_MEDIA1_CRG_PERDIS1_gt_aclk_media_common_START (23)
#define SOC_MEDIA1_CRG_PERDIS1_gt_aclk_media_common_END (23)
#define SOC_MEDIA1_CRG_PERDIS1_gt_pclk_media_common_START (24)
#define SOC_MEDIA1_CRG_PERDIS1_gt_pclk_media_common_END (24)
#define SOC_MEDIA1_CRG_PERDIS1_gt_aclk_qic_isp_START (25)
#define SOC_MEDIA1_CRG_PERDIS1_gt_aclk_qic_isp_END (25)
#define SOC_MEDIA1_CRG_PERDIS1_gt_pclk_qic_isp_START (26)
#define SOC_MEDIA1_CRG_PERDIS1_gt_pclk_qic_isp_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int gt_pclk_autodiv_vivobus : 1;
        unsigned int reserved_1 : 1;
        unsigned int gt_clk_qic_idi2axi : 1;
        unsigned int gt_pclk_autodiv_ispcpu : 1;
        unsigned int reserved_2 : 1;
        unsigned int gt_pclk_csi_wrapper_cfg : 1;
        unsigned int reserved_3 : 1;
        unsigned int gt_clk_isp_i3c : 1;
        unsigned int gt_clk_isp_ref32k : 1;
        unsigned int gt_clk_dss_ref32k : 1;
        unsigned int gt_clk_vivobus_ref32k : 1;
        unsigned int gt_clk_media_common : 1;
        unsigned int gt_clk_media_common_core : 1;
        unsigned int gt_clk_media_common_p2p_rx : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int gt_aclk_disp_qic_subsys : 1;
        unsigned int gt_pclk_disp_qic_subsys : 1;
        unsigned int gt_aclk_isp_qic_subsys : 1;
        unsigned int gt_pclk_isp_qic_subsys : 1;
        unsigned int gt_aclk_qic_dss : 1;
        unsigned int gt_pclk_qic_dss : 1;
        unsigned int gt_aclk_media_common : 1;
        unsigned int gt_pclk_media_common : 1;
        unsigned int gt_aclk_qic_isp : 1;
        unsigned int gt_pclk_qic_isp : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int reserved_8 : 3;
    } reg;
} SOC_MEDIA1_CRG_PERCLKEN1_UNION;
#endif
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_pclk_autodiv_vivobus_START (1)
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_pclk_autodiv_vivobus_END (1)
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_clk_qic_idi2axi_START (3)
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_clk_qic_idi2axi_END (3)
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_pclk_autodiv_ispcpu_START (4)
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_pclk_autodiv_ispcpu_END (4)
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_pclk_csi_wrapper_cfg_START (6)
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_pclk_csi_wrapper_cfg_END (6)
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_clk_isp_i3c_START (8)
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_clk_isp_i3c_END (8)
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_clk_isp_ref32k_START (9)
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_clk_isp_ref32k_END (9)
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_clk_dss_ref32k_START (10)
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_clk_dss_ref32k_END (10)
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_clk_vivobus_ref32k_START (11)
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_clk_vivobus_ref32k_END (11)
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_clk_media_common_START (12)
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_clk_media_common_END (12)
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_clk_media_common_core_START (13)
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_clk_media_common_core_END (13)
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_clk_media_common_p2p_rx_START (14)
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_clk_media_common_p2p_rx_END (14)
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_aclk_disp_qic_subsys_START (17)
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_aclk_disp_qic_subsys_END (17)
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_pclk_disp_qic_subsys_START (18)
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_pclk_disp_qic_subsys_END (18)
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_aclk_isp_qic_subsys_START (19)
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_aclk_isp_qic_subsys_END (19)
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_pclk_isp_qic_subsys_START (20)
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_pclk_isp_qic_subsys_END (20)
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_aclk_qic_dss_START (21)
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_aclk_qic_dss_END (21)
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_pclk_qic_dss_START (22)
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_pclk_qic_dss_END (22)
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_aclk_media_common_START (23)
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_aclk_media_common_END (23)
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_pclk_media_common_START (24)
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_pclk_media_common_END (24)
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_aclk_qic_isp_START (25)
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_aclk_qic_isp_END (25)
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_pclk_qic_isp_START (26)
#define SOC_MEDIA1_CRG_PERCLKEN1_gt_pclk_qic_isp_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int st_pclk_autodiv_vivobus : 1;
        unsigned int reserved_1 : 1;
        unsigned int st_clk_qic_idi2axi : 1;
        unsigned int st_pclk_autodiv_ispcpu : 1;
        unsigned int reserved_2 : 1;
        unsigned int st_pclk_csi_wrapper_cfg : 1;
        unsigned int reserved_3 : 1;
        unsigned int st_clk_isp_i3c : 1;
        unsigned int st_clk_isp_ref32k : 1;
        unsigned int st_clk_dss_ref32k : 1;
        unsigned int st_clk_vivobus_ref32k : 1;
        unsigned int st_clk_media_common : 1;
        unsigned int st_clk_media_common_core : 1;
        unsigned int st_clk_media_common_p2p_rx : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int st_aclk_disp_qic_subsys : 1;
        unsigned int st_pclk_disp_qic_subsys : 1;
        unsigned int st_aclk_isp_qic_subsys : 1;
        unsigned int st_pclk_isp_qic_subsys : 1;
        unsigned int st_aclk_qic_dss : 1;
        unsigned int st_pclk_qic_dss : 1;
        unsigned int st_aclk_media_common : 1;
        unsigned int st_pclk_media_common : 1;
        unsigned int st_aclk_qic_isp : 1;
        unsigned int st_pclk_qic_isp : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int reserved_8 : 3;
    } reg;
} SOC_MEDIA1_CRG_PERSTAT1_UNION;
#endif
#define SOC_MEDIA1_CRG_PERSTAT1_st_pclk_autodiv_vivobus_START (1)
#define SOC_MEDIA1_CRG_PERSTAT1_st_pclk_autodiv_vivobus_END (1)
#define SOC_MEDIA1_CRG_PERSTAT1_st_clk_qic_idi2axi_START (3)
#define SOC_MEDIA1_CRG_PERSTAT1_st_clk_qic_idi2axi_END (3)
#define SOC_MEDIA1_CRG_PERSTAT1_st_pclk_autodiv_ispcpu_START (4)
#define SOC_MEDIA1_CRG_PERSTAT1_st_pclk_autodiv_ispcpu_END (4)
#define SOC_MEDIA1_CRG_PERSTAT1_st_pclk_csi_wrapper_cfg_START (6)
#define SOC_MEDIA1_CRG_PERSTAT1_st_pclk_csi_wrapper_cfg_END (6)
#define SOC_MEDIA1_CRG_PERSTAT1_st_clk_isp_i3c_START (8)
#define SOC_MEDIA1_CRG_PERSTAT1_st_clk_isp_i3c_END (8)
#define SOC_MEDIA1_CRG_PERSTAT1_st_clk_isp_ref32k_START (9)
#define SOC_MEDIA1_CRG_PERSTAT1_st_clk_isp_ref32k_END (9)
#define SOC_MEDIA1_CRG_PERSTAT1_st_clk_dss_ref32k_START (10)
#define SOC_MEDIA1_CRG_PERSTAT1_st_clk_dss_ref32k_END (10)
#define SOC_MEDIA1_CRG_PERSTAT1_st_clk_vivobus_ref32k_START (11)
#define SOC_MEDIA1_CRG_PERSTAT1_st_clk_vivobus_ref32k_END (11)
#define SOC_MEDIA1_CRG_PERSTAT1_st_clk_media_common_START (12)
#define SOC_MEDIA1_CRG_PERSTAT1_st_clk_media_common_END (12)
#define SOC_MEDIA1_CRG_PERSTAT1_st_clk_media_common_core_START (13)
#define SOC_MEDIA1_CRG_PERSTAT1_st_clk_media_common_core_END (13)
#define SOC_MEDIA1_CRG_PERSTAT1_st_clk_media_common_p2p_rx_START (14)
#define SOC_MEDIA1_CRG_PERSTAT1_st_clk_media_common_p2p_rx_END (14)
#define SOC_MEDIA1_CRG_PERSTAT1_st_aclk_disp_qic_subsys_START (17)
#define SOC_MEDIA1_CRG_PERSTAT1_st_aclk_disp_qic_subsys_END (17)
#define SOC_MEDIA1_CRG_PERSTAT1_st_pclk_disp_qic_subsys_START (18)
#define SOC_MEDIA1_CRG_PERSTAT1_st_pclk_disp_qic_subsys_END (18)
#define SOC_MEDIA1_CRG_PERSTAT1_st_aclk_isp_qic_subsys_START (19)
#define SOC_MEDIA1_CRG_PERSTAT1_st_aclk_isp_qic_subsys_END (19)
#define SOC_MEDIA1_CRG_PERSTAT1_st_pclk_isp_qic_subsys_START (20)
#define SOC_MEDIA1_CRG_PERSTAT1_st_pclk_isp_qic_subsys_END (20)
#define SOC_MEDIA1_CRG_PERSTAT1_st_aclk_qic_dss_START (21)
#define SOC_MEDIA1_CRG_PERSTAT1_st_aclk_qic_dss_END (21)
#define SOC_MEDIA1_CRG_PERSTAT1_st_pclk_qic_dss_START (22)
#define SOC_MEDIA1_CRG_PERSTAT1_st_pclk_qic_dss_END (22)
#define SOC_MEDIA1_CRG_PERSTAT1_st_aclk_media_common_START (23)
#define SOC_MEDIA1_CRG_PERSTAT1_st_aclk_media_common_END (23)
#define SOC_MEDIA1_CRG_PERSTAT1_st_pclk_media_common_START (24)
#define SOC_MEDIA1_CRG_PERSTAT1_st_pclk_media_common_END (24)
#define SOC_MEDIA1_CRG_PERSTAT1_st_aclk_qic_isp_START (25)
#define SOC_MEDIA1_CRG_PERSTAT1_st_aclk_qic_isp_END (25)
#define SOC_MEDIA1_CRG_PERSTAT1_st_pclk_qic_isp_START (26)
#define SOC_MEDIA1_CRG_PERSTAT1_st_pclk_qic_isp_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_mmbuf : 1;
        unsigned int gt_pclk_mmbuf : 1;
        unsigned int gt_aclk_mmbuf : 1;
        unsigned int gt_aclk_asc : 1;
        unsigned int gt_clk_dss_axi_mm : 1;
        unsigned int gt_pclk_mmbuf_cfg : 1;
        unsigned int gt_clk_mmbuf_sh : 1;
        unsigned int gt_aclk_dss_axi_mm_sh : 1;
        unsigned int gt_clk_edc0_sh : 1;
        unsigned int gt_clk_vivobus_sh : 1;
        unsigned int gt_aclk_disp_qic_subsys_sh : 1;
        unsigned int gt_aclk_dss_sh : 1;
        unsigned int gt_pclk_disp_qic_subsys_sh : 1;
        unsigned int gt_pclk_dss_sh : 1;
        unsigned int reserved : 18;
    } reg;
} SOC_MEDIA1_CRG_PEREN2_UNION;
#endif
#define SOC_MEDIA1_CRG_PEREN2_gt_clk_mmbuf_START (0)
#define SOC_MEDIA1_CRG_PEREN2_gt_clk_mmbuf_END (0)
#define SOC_MEDIA1_CRG_PEREN2_gt_pclk_mmbuf_START (1)
#define SOC_MEDIA1_CRG_PEREN2_gt_pclk_mmbuf_END (1)
#define SOC_MEDIA1_CRG_PEREN2_gt_aclk_mmbuf_START (2)
#define SOC_MEDIA1_CRG_PEREN2_gt_aclk_mmbuf_END (2)
#define SOC_MEDIA1_CRG_PEREN2_gt_aclk_asc_START (3)
#define SOC_MEDIA1_CRG_PEREN2_gt_aclk_asc_END (3)
#define SOC_MEDIA1_CRG_PEREN2_gt_clk_dss_axi_mm_START (4)
#define SOC_MEDIA1_CRG_PEREN2_gt_clk_dss_axi_mm_END (4)
#define SOC_MEDIA1_CRG_PEREN2_gt_pclk_mmbuf_cfg_START (5)
#define SOC_MEDIA1_CRG_PEREN2_gt_pclk_mmbuf_cfg_END (5)
#define SOC_MEDIA1_CRG_PEREN2_gt_clk_mmbuf_sh_START (6)
#define SOC_MEDIA1_CRG_PEREN2_gt_clk_mmbuf_sh_END (6)
#define SOC_MEDIA1_CRG_PEREN2_gt_aclk_dss_axi_mm_sh_START (7)
#define SOC_MEDIA1_CRG_PEREN2_gt_aclk_dss_axi_mm_sh_END (7)
#define SOC_MEDIA1_CRG_PEREN2_gt_clk_edc0_sh_START (8)
#define SOC_MEDIA1_CRG_PEREN2_gt_clk_edc0_sh_END (8)
#define SOC_MEDIA1_CRG_PEREN2_gt_clk_vivobus_sh_START (9)
#define SOC_MEDIA1_CRG_PEREN2_gt_clk_vivobus_sh_END (9)
#define SOC_MEDIA1_CRG_PEREN2_gt_aclk_disp_qic_subsys_sh_START (10)
#define SOC_MEDIA1_CRG_PEREN2_gt_aclk_disp_qic_subsys_sh_END (10)
#define SOC_MEDIA1_CRG_PEREN2_gt_aclk_dss_sh_START (11)
#define SOC_MEDIA1_CRG_PEREN2_gt_aclk_dss_sh_END (11)
#define SOC_MEDIA1_CRG_PEREN2_gt_pclk_disp_qic_subsys_sh_START (12)
#define SOC_MEDIA1_CRG_PEREN2_gt_pclk_disp_qic_subsys_sh_END (12)
#define SOC_MEDIA1_CRG_PEREN2_gt_pclk_dss_sh_START (13)
#define SOC_MEDIA1_CRG_PEREN2_gt_pclk_dss_sh_END (13)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_mmbuf : 1;
        unsigned int gt_pclk_mmbuf : 1;
        unsigned int gt_aclk_mmbuf : 1;
        unsigned int gt_aclk_asc : 1;
        unsigned int gt_clk_dss_axi_mm : 1;
        unsigned int gt_pclk_mmbuf_cfg : 1;
        unsigned int gt_clk_mmbuf_sh : 1;
        unsigned int gt_aclk_dss_axi_mm_sh : 1;
        unsigned int gt_clk_edc0_sh : 1;
        unsigned int gt_clk_vivobus_sh : 1;
        unsigned int gt_aclk_disp_qic_subsys_sh : 1;
        unsigned int gt_aclk_dss_sh : 1;
        unsigned int gt_pclk_disp_qic_subsys_sh : 1;
        unsigned int gt_pclk_dss_sh : 1;
        unsigned int reserved : 18;
    } reg;
} SOC_MEDIA1_CRG_PERDIS2_UNION;
#endif
#define SOC_MEDIA1_CRG_PERDIS2_gt_clk_mmbuf_START (0)
#define SOC_MEDIA1_CRG_PERDIS2_gt_clk_mmbuf_END (0)
#define SOC_MEDIA1_CRG_PERDIS2_gt_pclk_mmbuf_START (1)
#define SOC_MEDIA1_CRG_PERDIS2_gt_pclk_mmbuf_END (1)
#define SOC_MEDIA1_CRG_PERDIS2_gt_aclk_mmbuf_START (2)
#define SOC_MEDIA1_CRG_PERDIS2_gt_aclk_mmbuf_END (2)
#define SOC_MEDIA1_CRG_PERDIS2_gt_aclk_asc_START (3)
#define SOC_MEDIA1_CRG_PERDIS2_gt_aclk_asc_END (3)
#define SOC_MEDIA1_CRG_PERDIS2_gt_clk_dss_axi_mm_START (4)
#define SOC_MEDIA1_CRG_PERDIS2_gt_clk_dss_axi_mm_END (4)
#define SOC_MEDIA1_CRG_PERDIS2_gt_pclk_mmbuf_cfg_START (5)
#define SOC_MEDIA1_CRG_PERDIS2_gt_pclk_mmbuf_cfg_END (5)
#define SOC_MEDIA1_CRG_PERDIS2_gt_clk_mmbuf_sh_START (6)
#define SOC_MEDIA1_CRG_PERDIS2_gt_clk_mmbuf_sh_END (6)
#define SOC_MEDIA1_CRG_PERDIS2_gt_aclk_dss_axi_mm_sh_START (7)
#define SOC_MEDIA1_CRG_PERDIS2_gt_aclk_dss_axi_mm_sh_END (7)
#define SOC_MEDIA1_CRG_PERDIS2_gt_clk_edc0_sh_START (8)
#define SOC_MEDIA1_CRG_PERDIS2_gt_clk_edc0_sh_END (8)
#define SOC_MEDIA1_CRG_PERDIS2_gt_clk_vivobus_sh_START (9)
#define SOC_MEDIA1_CRG_PERDIS2_gt_clk_vivobus_sh_END (9)
#define SOC_MEDIA1_CRG_PERDIS2_gt_aclk_disp_qic_subsys_sh_START (10)
#define SOC_MEDIA1_CRG_PERDIS2_gt_aclk_disp_qic_subsys_sh_END (10)
#define SOC_MEDIA1_CRG_PERDIS2_gt_aclk_dss_sh_START (11)
#define SOC_MEDIA1_CRG_PERDIS2_gt_aclk_dss_sh_END (11)
#define SOC_MEDIA1_CRG_PERDIS2_gt_pclk_disp_qic_subsys_sh_START (12)
#define SOC_MEDIA1_CRG_PERDIS2_gt_pclk_disp_qic_subsys_sh_END (12)
#define SOC_MEDIA1_CRG_PERDIS2_gt_pclk_dss_sh_START (13)
#define SOC_MEDIA1_CRG_PERDIS2_gt_pclk_dss_sh_END (13)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_mmbuf : 1;
        unsigned int gt_pclk_mmbuf : 1;
        unsigned int gt_aclk_mmbuf : 1;
        unsigned int gt_aclk_asc : 1;
        unsigned int gt_clk_dss_axi_mm : 1;
        unsigned int gt_pclk_mmbuf_cfg : 1;
        unsigned int gt_clk_mmbuf_sh : 1;
        unsigned int gt_aclk_dss_axi_mm_sh : 1;
        unsigned int gt_clk_edc0_sh : 1;
        unsigned int gt_clk_vivobus_sh : 1;
        unsigned int gt_aclk_disp_qic_subsys_sh : 1;
        unsigned int gt_aclk_dss_sh : 1;
        unsigned int gt_pclk_disp_qic_subsys_sh : 1;
        unsigned int gt_pclk_dss_sh : 1;
        unsigned int reserved : 18;
    } reg;
} SOC_MEDIA1_CRG_PERCLKEN2_UNION;
#endif
#define SOC_MEDIA1_CRG_PERCLKEN2_gt_clk_mmbuf_START (0)
#define SOC_MEDIA1_CRG_PERCLKEN2_gt_clk_mmbuf_END (0)
#define SOC_MEDIA1_CRG_PERCLKEN2_gt_pclk_mmbuf_START (1)
#define SOC_MEDIA1_CRG_PERCLKEN2_gt_pclk_mmbuf_END (1)
#define SOC_MEDIA1_CRG_PERCLKEN2_gt_aclk_mmbuf_START (2)
#define SOC_MEDIA1_CRG_PERCLKEN2_gt_aclk_mmbuf_END (2)
#define SOC_MEDIA1_CRG_PERCLKEN2_gt_aclk_asc_START (3)
#define SOC_MEDIA1_CRG_PERCLKEN2_gt_aclk_asc_END (3)
#define SOC_MEDIA1_CRG_PERCLKEN2_gt_clk_dss_axi_mm_START (4)
#define SOC_MEDIA1_CRG_PERCLKEN2_gt_clk_dss_axi_mm_END (4)
#define SOC_MEDIA1_CRG_PERCLKEN2_gt_pclk_mmbuf_cfg_START (5)
#define SOC_MEDIA1_CRG_PERCLKEN2_gt_pclk_mmbuf_cfg_END (5)
#define SOC_MEDIA1_CRG_PERCLKEN2_gt_clk_mmbuf_sh_START (6)
#define SOC_MEDIA1_CRG_PERCLKEN2_gt_clk_mmbuf_sh_END (6)
#define SOC_MEDIA1_CRG_PERCLKEN2_gt_aclk_dss_axi_mm_sh_START (7)
#define SOC_MEDIA1_CRG_PERCLKEN2_gt_aclk_dss_axi_mm_sh_END (7)
#define SOC_MEDIA1_CRG_PERCLKEN2_gt_clk_edc0_sh_START (8)
#define SOC_MEDIA1_CRG_PERCLKEN2_gt_clk_edc0_sh_END (8)
#define SOC_MEDIA1_CRG_PERCLKEN2_gt_clk_vivobus_sh_START (9)
#define SOC_MEDIA1_CRG_PERCLKEN2_gt_clk_vivobus_sh_END (9)
#define SOC_MEDIA1_CRG_PERCLKEN2_gt_aclk_disp_qic_subsys_sh_START (10)
#define SOC_MEDIA1_CRG_PERCLKEN2_gt_aclk_disp_qic_subsys_sh_END (10)
#define SOC_MEDIA1_CRG_PERCLKEN2_gt_aclk_dss_sh_START (11)
#define SOC_MEDIA1_CRG_PERCLKEN2_gt_aclk_dss_sh_END (11)
#define SOC_MEDIA1_CRG_PERCLKEN2_gt_pclk_disp_qic_subsys_sh_START (12)
#define SOC_MEDIA1_CRG_PERCLKEN2_gt_pclk_disp_qic_subsys_sh_END (12)
#define SOC_MEDIA1_CRG_PERCLKEN2_gt_pclk_dss_sh_START (13)
#define SOC_MEDIA1_CRG_PERCLKEN2_gt_pclk_dss_sh_END (13)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int st_clk_mmbuf : 1;
        unsigned int st_pclk_mmbuf : 1;
        unsigned int st_aclk_mmbuf : 1;
        unsigned int st_aclk_asc : 1;
        unsigned int st_clk_dss_axi_mm : 1;
        unsigned int st_pclk_mmbuf_cfg : 1;
        unsigned int st_clk_mmbuf_sh : 1;
        unsigned int st_aclk_dss_axi_mm_sh : 1;
        unsigned int st_clk_edc0_sh : 1;
        unsigned int st_clk_vivobus_sh : 1;
        unsigned int st_aclk_disp_qic_subsys_sh : 1;
        unsigned int st_aclk_dss_sh : 1;
        unsigned int st_pclk_disp_qic_subsys_sh : 1;
        unsigned int st_pclk_dss_sh : 1;
        unsigned int reserved : 18;
    } reg;
} SOC_MEDIA1_CRG_PERSTAT2_UNION;
#endif
#define SOC_MEDIA1_CRG_PERSTAT2_st_clk_mmbuf_START (0)
#define SOC_MEDIA1_CRG_PERSTAT2_st_clk_mmbuf_END (0)
#define SOC_MEDIA1_CRG_PERSTAT2_st_pclk_mmbuf_START (1)
#define SOC_MEDIA1_CRG_PERSTAT2_st_pclk_mmbuf_END (1)
#define SOC_MEDIA1_CRG_PERSTAT2_st_aclk_mmbuf_START (2)
#define SOC_MEDIA1_CRG_PERSTAT2_st_aclk_mmbuf_END (2)
#define SOC_MEDIA1_CRG_PERSTAT2_st_aclk_asc_START (3)
#define SOC_MEDIA1_CRG_PERSTAT2_st_aclk_asc_END (3)
#define SOC_MEDIA1_CRG_PERSTAT2_st_clk_dss_axi_mm_START (4)
#define SOC_MEDIA1_CRG_PERSTAT2_st_clk_dss_axi_mm_END (4)
#define SOC_MEDIA1_CRG_PERSTAT2_st_pclk_mmbuf_cfg_START (5)
#define SOC_MEDIA1_CRG_PERSTAT2_st_pclk_mmbuf_cfg_END (5)
#define SOC_MEDIA1_CRG_PERSTAT2_st_clk_mmbuf_sh_START (6)
#define SOC_MEDIA1_CRG_PERSTAT2_st_clk_mmbuf_sh_END (6)
#define SOC_MEDIA1_CRG_PERSTAT2_st_aclk_dss_axi_mm_sh_START (7)
#define SOC_MEDIA1_CRG_PERSTAT2_st_aclk_dss_axi_mm_sh_END (7)
#define SOC_MEDIA1_CRG_PERSTAT2_st_clk_edc0_sh_START (8)
#define SOC_MEDIA1_CRG_PERSTAT2_st_clk_edc0_sh_END (8)
#define SOC_MEDIA1_CRG_PERSTAT2_st_clk_vivobus_sh_START (9)
#define SOC_MEDIA1_CRG_PERSTAT2_st_clk_vivobus_sh_END (9)
#define SOC_MEDIA1_CRG_PERSTAT2_st_aclk_disp_qic_subsys_sh_START (10)
#define SOC_MEDIA1_CRG_PERSTAT2_st_aclk_disp_qic_subsys_sh_END (10)
#define SOC_MEDIA1_CRG_PERSTAT2_st_aclk_dss_sh_START (11)
#define SOC_MEDIA1_CRG_PERSTAT2_st_aclk_dss_sh_END (11)
#define SOC_MEDIA1_CRG_PERSTAT2_st_pclk_disp_qic_subsys_sh_START (12)
#define SOC_MEDIA1_CRG_PERSTAT2_st_pclk_disp_qic_subsys_sh_END (12)
#define SOC_MEDIA1_CRG_PERSTAT2_st_pclk_dss_sh_START (13)
#define SOC_MEDIA1_CRG_PERSTAT2_st_pclk_dss_sh_END (13)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int ip_rst_dss : 1;
        unsigned int ip_prst_dss : 1;
        unsigned int reserved_6 : 1;
        unsigned int ip_prst_atgm_dss : 1;
        unsigned int ip_prst_atgm_isp : 1;
        unsigned int ip_rst_trace : 1;
        unsigned int reserved_7 : 1;
        unsigned int ip_prst_media1_lm : 1;
        unsigned int ip_rst_brg : 1;
        unsigned int reserved_8 : 1;
        unsigned int ip_prst_atgs_vivo : 1;
        unsigned int reserved_9 : 1;
        unsigned int ip_rst_media_common : 1;
        unsigned int ip_prst_media_common : 1;
        unsigned int ip_arst_media_common : 1;
        unsigned int reserved_10 : 1;
        unsigned int reserved_11 : 1;
        unsigned int reserved_12 : 1;
        unsigned int ip_rst_tcu_vivo : 1;
        unsigned int ip_rst_disp_subsys_cfg : 1;
        unsigned int ip_rst_isp_subsys_cfg : 1;
        unsigned int reserved_13 : 5;
    } reg;
} SOC_MEDIA1_CRG_PERRSTEN0_UNION;
#endif
#define SOC_MEDIA1_CRG_PERRSTEN0_ip_rst_dss_START (6)
#define SOC_MEDIA1_CRG_PERRSTEN0_ip_rst_dss_END (6)
#define SOC_MEDIA1_CRG_PERRSTEN0_ip_prst_dss_START (7)
#define SOC_MEDIA1_CRG_PERRSTEN0_ip_prst_dss_END (7)
#define SOC_MEDIA1_CRG_PERRSTEN0_ip_prst_atgm_dss_START (9)
#define SOC_MEDIA1_CRG_PERRSTEN0_ip_prst_atgm_dss_END (9)
#define SOC_MEDIA1_CRG_PERRSTEN0_ip_prst_atgm_isp_START (10)
#define SOC_MEDIA1_CRG_PERRSTEN0_ip_prst_atgm_isp_END (10)
#define SOC_MEDIA1_CRG_PERRSTEN0_ip_rst_trace_START (11)
#define SOC_MEDIA1_CRG_PERRSTEN0_ip_rst_trace_END (11)
#define SOC_MEDIA1_CRG_PERRSTEN0_ip_prst_media1_lm_START (13)
#define SOC_MEDIA1_CRG_PERRSTEN0_ip_prst_media1_lm_END (13)
#define SOC_MEDIA1_CRG_PERRSTEN0_ip_rst_brg_START (14)
#define SOC_MEDIA1_CRG_PERRSTEN0_ip_rst_brg_END (14)
#define SOC_MEDIA1_CRG_PERRSTEN0_ip_prst_atgs_vivo_START (16)
#define SOC_MEDIA1_CRG_PERRSTEN0_ip_prst_atgs_vivo_END (16)
#define SOC_MEDIA1_CRG_PERRSTEN0_ip_rst_media_common_START (18)
#define SOC_MEDIA1_CRG_PERRSTEN0_ip_rst_media_common_END (18)
#define SOC_MEDIA1_CRG_PERRSTEN0_ip_prst_media_common_START (19)
#define SOC_MEDIA1_CRG_PERRSTEN0_ip_prst_media_common_END (19)
#define SOC_MEDIA1_CRG_PERRSTEN0_ip_arst_media_common_START (20)
#define SOC_MEDIA1_CRG_PERRSTEN0_ip_arst_media_common_END (20)
#define SOC_MEDIA1_CRG_PERRSTEN0_ip_rst_tcu_vivo_START (24)
#define SOC_MEDIA1_CRG_PERRSTEN0_ip_rst_tcu_vivo_END (24)
#define SOC_MEDIA1_CRG_PERRSTEN0_ip_rst_disp_subsys_cfg_START (25)
#define SOC_MEDIA1_CRG_PERRSTEN0_ip_rst_disp_subsys_cfg_END (25)
#define SOC_MEDIA1_CRG_PERRSTEN0_ip_rst_isp_subsys_cfg_START (26)
#define SOC_MEDIA1_CRG_PERRSTEN0_ip_rst_isp_subsys_cfg_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int ip_rst_dss : 1;
        unsigned int ip_prst_dss : 1;
        unsigned int reserved_6 : 1;
        unsigned int ip_prst_atgm_dss : 1;
        unsigned int ip_prst_atgm_isp : 1;
        unsigned int ip_rst_trace : 1;
        unsigned int reserved_7 : 1;
        unsigned int ip_prst_media1_lm : 1;
        unsigned int ip_rst_brg : 1;
        unsigned int reserved_8 : 1;
        unsigned int ip_prst_atgs_vivo : 1;
        unsigned int reserved_9 : 1;
        unsigned int ip_rst_media_common : 1;
        unsigned int ip_prst_media_common : 1;
        unsigned int ip_arst_media_common : 1;
        unsigned int reserved_10 : 1;
        unsigned int reserved_11 : 1;
        unsigned int reserved_12 : 1;
        unsigned int ip_rst_tcu_vivo : 1;
        unsigned int ip_rst_disp_subsys_cfg : 1;
        unsigned int ip_rst_isp_subsys_cfg : 1;
        unsigned int reserved_13 : 5;
    } reg;
} SOC_MEDIA1_CRG_PERRSTDIS0_UNION;
#endif
#define SOC_MEDIA1_CRG_PERRSTDIS0_ip_rst_dss_START (6)
#define SOC_MEDIA1_CRG_PERRSTDIS0_ip_rst_dss_END (6)
#define SOC_MEDIA1_CRG_PERRSTDIS0_ip_prst_dss_START (7)
#define SOC_MEDIA1_CRG_PERRSTDIS0_ip_prst_dss_END (7)
#define SOC_MEDIA1_CRG_PERRSTDIS0_ip_prst_atgm_dss_START (9)
#define SOC_MEDIA1_CRG_PERRSTDIS0_ip_prst_atgm_dss_END (9)
#define SOC_MEDIA1_CRG_PERRSTDIS0_ip_prst_atgm_isp_START (10)
#define SOC_MEDIA1_CRG_PERRSTDIS0_ip_prst_atgm_isp_END (10)
#define SOC_MEDIA1_CRG_PERRSTDIS0_ip_rst_trace_START (11)
#define SOC_MEDIA1_CRG_PERRSTDIS0_ip_rst_trace_END (11)
#define SOC_MEDIA1_CRG_PERRSTDIS0_ip_prst_media1_lm_START (13)
#define SOC_MEDIA1_CRG_PERRSTDIS0_ip_prst_media1_lm_END (13)
#define SOC_MEDIA1_CRG_PERRSTDIS0_ip_rst_brg_START (14)
#define SOC_MEDIA1_CRG_PERRSTDIS0_ip_rst_brg_END (14)
#define SOC_MEDIA1_CRG_PERRSTDIS0_ip_prst_atgs_vivo_START (16)
#define SOC_MEDIA1_CRG_PERRSTDIS0_ip_prst_atgs_vivo_END (16)
#define SOC_MEDIA1_CRG_PERRSTDIS0_ip_rst_media_common_START (18)
#define SOC_MEDIA1_CRG_PERRSTDIS0_ip_rst_media_common_END (18)
#define SOC_MEDIA1_CRG_PERRSTDIS0_ip_prst_media_common_START (19)
#define SOC_MEDIA1_CRG_PERRSTDIS0_ip_prst_media_common_END (19)
#define SOC_MEDIA1_CRG_PERRSTDIS0_ip_arst_media_common_START (20)
#define SOC_MEDIA1_CRG_PERRSTDIS0_ip_arst_media_common_END (20)
#define SOC_MEDIA1_CRG_PERRSTDIS0_ip_rst_tcu_vivo_START (24)
#define SOC_MEDIA1_CRG_PERRSTDIS0_ip_rst_tcu_vivo_END (24)
#define SOC_MEDIA1_CRG_PERRSTDIS0_ip_rst_disp_subsys_cfg_START (25)
#define SOC_MEDIA1_CRG_PERRSTDIS0_ip_rst_disp_subsys_cfg_END (25)
#define SOC_MEDIA1_CRG_PERRSTDIS0_ip_rst_isp_subsys_cfg_START (26)
#define SOC_MEDIA1_CRG_PERRSTDIS0_ip_rst_isp_subsys_cfg_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int ip_rst_dss : 1;
        unsigned int ip_prst_dss : 1;
        unsigned int reserved_6 : 1;
        unsigned int ip_prst_atgm_dss : 1;
        unsigned int ip_prst_atgm_isp : 1;
        unsigned int ip_rst_trace : 1;
        unsigned int reserved_7 : 1;
        unsigned int ip_prst_media1_lm : 1;
        unsigned int ip_rst_brg : 1;
        unsigned int reserved_8 : 1;
        unsigned int ip_prst_atgs_vivo : 1;
        unsigned int reserved_9 : 1;
        unsigned int ip_rst_media_common : 1;
        unsigned int ip_prst_media_common : 1;
        unsigned int ip_arst_media_common : 1;
        unsigned int reserved_10 : 1;
        unsigned int reserved_11 : 1;
        unsigned int reserved_12 : 1;
        unsigned int ip_rst_tcu_vivo : 1;
        unsigned int ip_rst_disp_subsys_cfg : 1;
        unsigned int ip_rst_isp_subsys_cfg : 1;
        unsigned int reserved_13 : 5;
    } reg;
} SOC_MEDIA1_CRG_PERRSTSTAT0_UNION;
#endif
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_rst_dss_START (6)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_rst_dss_END (6)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_prst_dss_START (7)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_prst_dss_END (7)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_prst_atgm_dss_START (9)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_prst_atgm_dss_END (9)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_prst_atgm_isp_START (10)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_prst_atgm_isp_END (10)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_rst_trace_START (11)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_rst_trace_END (11)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_prst_media1_lm_START (13)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_prst_media1_lm_END (13)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_rst_brg_START (14)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_rst_brg_END (14)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_prst_atgs_vivo_START (16)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_prst_atgs_vivo_END (16)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_rst_media_common_START (18)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_rst_media_common_END (18)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_prst_media_common_START (19)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_prst_media_common_END (19)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_arst_media_common_START (20)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_arst_media_common_END (20)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_rst_tcu_vivo_START (24)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_rst_tcu_vivo_END (24)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_rst_disp_subsys_cfg_START (25)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_rst_disp_subsys_cfg_END (25)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_rst_isp_subsys_cfg_START (26)
#define SOC_MEDIA1_CRG_PERRSTSTAT0_ip_rst_isp_subsys_cfg_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int ip_rst_qic_isp : 1;
        unsigned int ip_rst_qic_isp_cfg : 1;
        unsigned int ip_rst_qic_dss : 1;
        unsigned int ip_rst_qic_dss_cfg : 1;
        unsigned int ip_prst_mmbuf : 1;
        unsigned int ip_prst_mmbuf_cfg : 1;
        unsigned int reserved_2 : 24;
    } reg;
} SOC_MEDIA1_CRG_PERRSTEN1_UNION;
#endif
#define SOC_MEDIA1_CRG_PERRSTEN1_ip_rst_qic_isp_START (2)
#define SOC_MEDIA1_CRG_PERRSTEN1_ip_rst_qic_isp_END (2)
#define SOC_MEDIA1_CRG_PERRSTEN1_ip_rst_qic_isp_cfg_START (3)
#define SOC_MEDIA1_CRG_PERRSTEN1_ip_rst_qic_isp_cfg_END (3)
#define SOC_MEDIA1_CRG_PERRSTEN1_ip_rst_qic_dss_START (4)
#define SOC_MEDIA1_CRG_PERRSTEN1_ip_rst_qic_dss_END (4)
#define SOC_MEDIA1_CRG_PERRSTEN1_ip_rst_qic_dss_cfg_START (5)
#define SOC_MEDIA1_CRG_PERRSTEN1_ip_rst_qic_dss_cfg_END (5)
#define SOC_MEDIA1_CRG_PERRSTEN1_ip_prst_mmbuf_START (6)
#define SOC_MEDIA1_CRG_PERRSTEN1_ip_prst_mmbuf_END (6)
#define SOC_MEDIA1_CRG_PERRSTEN1_ip_prst_mmbuf_cfg_START (7)
#define SOC_MEDIA1_CRG_PERRSTEN1_ip_prst_mmbuf_cfg_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int ip_rst_qic_isp : 1;
        unsigned int ip_rst_qic_isp_cfg : 1;
        unsigned int ip_rst_qic_dss : 1;
        unsigned int ip_rst_qic_dss_cfg : 1;
        unsigned int ip_prst_mmbuf : 1;
        unsigned int ip_prst_mmbuf_cfg : 1;
        unsigned int reserved_2 : 24;
    } reg;
} SOC_MEDIA1_CRG_PERRSTDIS1_UNION;
#endif
#define SOC_MEDIA1_CRG_PERRSTDIS1_ip_rst_qic_isp_START (2)
#define SOC_MEDIA1_CRG_PERRSTDIS1_ip_rst_qic_isp_END (2)
#define SOC_MEDIA1_CRG_PERRSTDIS1_ip_rst_qic_isp_cfg_START (3)
#define SOC_MEDIA1_CRG_PERRSTDIS1_ip_rst_qic_isp_cfg_END (3)
#define SOC_MEDIA1_CRG_PERRSTDIS1_ip_rst_qic_dss_START (4)
#define SOC_MEDIA1_CRG_PERRSTDIS1_ip_rst_qic_dss_END (4)
#define SOC_MEDIA1_CRG_PERRSTDIS1_ip_rst_qic_dss_cfg_START (5)
#define SOC_MEDIA1_CRG_PERRSTDIS1_ip_rst_qic_dss_cfg_END (5)
#define SOC_MEDIA1_CRG_PERRSTDIS1_ip_prst_mmbuf_START (6)
#define SOC_MEDIA1_CRG_PERRSTDIS1_ip_prst_mmbuf_END (6)
#define SOC_MEDIA1_CRG_PERRSTDIS1_ip_prst_mmbuf_cfg_START (7)
#define SOC_MEDIA1_CRG_PERRSTDIS1_ip_prst_mmbuf_cfg_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int ip_rst_qic_isp : 1;
        unsigned int ip_rst_qic_isp_cfg : 1;
        unsigned int ip_rst_qic_dss : 1;
        unsigned int ip_rst_qic_dss_cfg : 1;
        unsigned int ip_prst_mmbuf : 1;
        unsigned int ip_prst_mmbuf_cfg : 1;
        unsigned int reserved_2 : 24;
    } reg;
} SOC_MEDIA1_CRG_PERRSTSTAT1_UNION;
#endif
#define SOC_MEDIA1_CRG_PERRSTSTAT1_ip_rst_qic_isp_START (2)
#define SOC_MEDIA1_CRG_PERRSTSTAT1_ip_rst_qic_isp_END (2)
#define SOC_MEDIA1_CRG_PERRSTSTAT1_ip_rst_qic_isp_cfg_START (3)
#define SOC_MEDIA1_CRG_PERRSTSTAT1_ip_rst_qic_isp_cfg_END (3)
#define SOC_MEDIA1_CRG_PERRSTSTAT1_ip_rst_qic_dss_START (4)
#define SOC_MEDIA1_CRG_PERRSTSTAT1_ip_rst_qic_dss_END (4)
#define SOC_MEDIA1_CRG_PERRSTSTAT1_ip_rst_qic_dss_cfg_START (5)
#define SOC_MEDIA1_CRG_PERRSTSTAT1_ip_rst_qic_dss_cfg_END (5)
#define SOC_MEDIA1_CRG_PERRSTSTAT1_ip_prst_mmbuf_START (6)
#define SOC_MEDIA1_CRG_PERRSTSTAT1_ip_prst_mmbuf_END (6)
#define SOC_MEDIA1_CRG_PERRSTSTAT1_ip_prst_mmbuf_cfg_START (7)
#define SOC_MEDIA1_CRG_PERRSTSTAT1_ip_prst_mmbuf_cfg_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sel_ispfunc5_pll : 4;
        unsigned int div_ispfunc5 : 6;
        unsigned int sel_isp_cpu_pll : 4;
        unsigned int reserved : 2;
        unsigned int bitmasken : 16;
    } reg;
} SOC_MEDIA1_CRG_CLKDIV0_UNION;
#endif
#define SOC_MEDIA1_CRG_CLKDIV0_sel_ispfunc5_pll_START (0)
#define SOC_MEDIA1_CRG_CLKDIV0_sel_ispfunc5_pll_END (3)
#define SOC_MEDIA1_CRG_CLKDIV0_div_ispfunc5_START (4)
#define SOC_MEDIA1_CRG_CLKDIV0_div_ispfunc5_END (9)
#define SOC_MEDIA1_CRG_CLKDIV0_sel_isp_cpu_pll_START (10)
#define SOC_MEDIA1_CRG_CLKDIV0_sel_isp_cpu_pll_END (13)
#define SOC_MEDIA1_CRG_CLKDIV0_bitmasken_START (16)
#define SOC_MEDIA1_CRG_CLKDIV0_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int div_ldi1 : 6;
        unsigned int sel_ldi1_pll : 4;
        unsigned int div_isp_i3c : 6;
        unsigned int bitmasken : 16;
    } reg;
} SOC_MEDIA1_CRG_CLKDIV1_UNION;
#endif
#define SOC_MEDIA1_CRG_CLKDIV1_div_ldi1_START (0)
#define SOC_MEDIA1_CRG_CLKDIV1_div_ldi1_END (5)
#define SOC_MEDIA1_CRG_CLKDIV1_sel_ldi1_pll_START (6)
#define SOC_MEDIA1_CRG_CLKDIV1_sel_ldi1_pll_END (9)
#define SOC_MEDIA1_CRG_CLKDIV1_div_isp_i3c_START (10)
#define SOC_MEDIA1_CRG_CLKDIV1_div_isp_i3c_END (15)
#define SOC_MEDIA1_CRG_CLKDIV1_bitmasken_START (16)
#define SOC_MEDIA1_CRG_CLKDIV1_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int div_edc0 : 6;
        unsigned int sel_edc0_pll : 4;
        unsigned int div_idi2axi_csid : 6;
        unsigned int bitmasken : 16;
    } reg;
} SOC_MEDIA1_CRG_CLKDIV2_UNION;
#endif
#define SOC_MEDIA1_CRG_CLKDIV2_div_edc0_START (0)
#define SOC_MEDIA1_CRG_CLKDIV2_div_edc0_END (5)
#define SOC_MEDIA1_CRG_CLKDIV2_sel_edc0_pll_START (6)
#define SOC_MEDIA1_CRG_CLKDIV2_sel_edc0_pll_END (9)
#define SOC_MEDIA1_CRG_CLKDIV2_div_idi2axi_csid_START (10)
#define SOC_MEDIA1_CRG_CLKDIV2_div_idi2axi_csid_END (15)
#define SOC_MEDIA1_CRG_CLKDIV2_bitmasken_START (16)
#define SOC_MEDIA1_CRG_CLKDIV2_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int div_ispfunc : 6;
        unsigned int sel_ispfunc_pll : 4;
        unsigned int sel_ispfunc2_pll : 4;
        unsigned int reserved : 2;
        unsigned int bitmasken : 16;
    } reg;
} SOC_MEDIA1_CRG_CLKDIV3_UNION;
#endif
#define SOC_MEDIA1_CRG_CLKDIV3_div_ispfunc_START (0)
#define SOC_MEDIA1_CRG_CLKDIV3_div_ispfunc_END (5)
#define SOC_MEDIA1_CRG_CLKDIV3_sel_ispfunc_pll_START (6)
#define SOC_MEDIA1_CRG_CLKDIV3_sel_ispfunc_pll_END (9)
#define SOC_MEDIA1_CRG_CLKDIV3_sel_ispfunc2_pll_START (10)
#define SOC_MEDIA1_CRG_CLKDIV3_sel_ispfunc2_pll_END (13)
#define SOC_MEDIA1_CRG_CLKDIV3_bitmasken_START (16)
#define SOC_MEDIA1_CRG_CLKDIV3_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int div_isp_cpu : 6;
        unsigned int reserved_0 : 2;
        unsigned int reserved_1 : 2;
        unsigned int sel_ispfunc3_pll : 4;
        unsigned int reserved_2 : 2;
        unsigned int bitmasken : 16;
    } reg;
} SOC_MEDIA1_CRG_CLKDIV4_UNION;
#endif
#define SOC_MEDIA1_CRG_CLKDIV4_div_isp_cpu_START (0)
#define SOC_MEDIA1_CRG_CLKDIV4_div_isp_cpu_END (5)
#define SOC_MEDIA1_CRG_CLKDIV4_sel_ispfunc3_pll_START (10)
#define SOC_MEDIA1_CRG_CLKDIV4_sel_ispfunc3_pll_END (13)
#define SOC_MEDIA1_CRG_CLKDIV4_bitmasken_START (16)
#define SOC_MEDIA1_CRG_CLKDIV4_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int div_vivobus : 6;
        unsigned int sel_vivobus_pll : 4;
        unsigned int div_ispfunc4 : 6;
        unsigned int bitmasken : 16;
    } reg;
} SOC_MEDIA1_CRG_CLKDIV5_UNION;
#endif
#define SOC_MEDIA1_CRG_CLKDIV5_div_vivobus_START (0)
#define SOC_MEDIA1_CRG_CLKDIV5_div_vivobus_END (5)
#define SOC_MEDIA1_CRG_CLKDIV5_sel_vivobus_pll_START (6)
#define SOC_MEDIA1_CRG_CLKDIV5_sel_vivobus_pll_END (9)
#define SOC_MEDIA1_CRG_CLKDIV5_div_ispfunc4_START (10)
#define SOC_MEDIA1_CRG_CLKDIV5_div_ispfunc4_END (15)
#define SOC_MEDIA1_CRG_CLKDIV5_bitmasken_START (16)
#define SOC_MEDIA1_CRG_CLKDIV5_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int div_pclk_mmbuf : 2;
        unsigned int sel_isp_i2c : 1;
        unsigned int sel_ispfunc4_pll : 4;
        unsigned int reserved_0 : 2;
        unsigned int reserved_1 : 4;
        unsigned int reserved_2 : 3;
        unsigned int bitmasken : 16;
    } reg;
} SOC_MEDIA1_CRG_CLKDIV6_UNION;
#endif
#define SOC_MEDIA1_CRG_CLKDIV6_div_pclk_mmbuf_START (0)
#define SOC_MEDIA1_CRG_CLKDIV6_div_pclk_mmbuf_END (1)
#define SOC_MEDIA1_CRG_CLKDIV6_sel_isp_i2c_START (2)
#define SOC_MEDIA1_CRG_CLKDIV6_sel_isp_i2c_END (2)
#define SOC_MEDIA1_CRG_CLKDIV6_sel_ispfunc4_pll_START (3)
#define SOC_MEDIA1_CRG_CLKDIV6_sel_ispfunc4_pll_END (6)
#define SOC_MEDIA1_CRG_CLKDIV6_bitmasken_START (16)
#define SOC_MEDIA1_CRG_CLKDIV6_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 4;
        unsigned int reserved_1 : 6;
        unsigned int div_aclk_mmbuf : 6;
        unsigned int bitmasken : 16;
    } reg;
} SOC_MEDIA1_CRG_CLKDIV7_UNION;
#endif
#define SOC_MEDIA1_CRG_CLKDIV7_div_aclk_mmbuf_START (10)
#define SOC_MEDIA1_CRG_CLKDIV7_div_aclk_mmbuf_END (15)
#define SOC_MEDIA1_CRG_CLKDIV7_bitmasken_START (16)
#define SOC_MEDIA1_CRG_CLKDIV7_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 6;
        unsigned int div_media_common : 6;
        unsigned int sel_media_common_pll : 4;
        unsigned int bitmasken : 16;
    } reg;
} SOC_MEDIA1_CRG_CLKDIV8_UNION;
#endif
#define SOC_MEDIA1_CRG_CLKDIV8_div_media_common_START (6)
#define SOC_MEDIA1_CRG_CLKDIV8_div_media_common_END (11)
#define SOC_MEDIA1_CRG_CLKDIV8_sel_media_common_pll_START (12)
#define SOC_MEDIA1_CRG_CLKDIV8_sel_media_common_pll_END (15)
#define SOC_MEDIA1_CRG_CLKDIV8_bitmasken_START (16)
#define SOC_MEDIA1_CRG_CLKDIV8_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sc_gt_clk_ispfunc4 : 1;
        unsigned int sc_gt_clk_ispfunc3 : 1;
        unsigned int sc_gt_clk_ispfunc2 : 1;
        unsigned int sc_gt_clk_vivobus : 1;
        unsigned int sc_gt_clk_idi2axi_csid : 1;
        unsigned int sc_gt_clk_isp_cpu : 1;
        unsigned int sc_gt_clk_ispfunc : 1;
        unsigned int sc_gt_clk_edc0 : 1;
        unsigned int sc_gt_clk_ldi1 : 1;
        unsigned int sc_gt_clk_isp_i3c : 1;
        unsigned int sc_gt_clk_ispfunc5 : 1;
        unsigned int sc_gt_clk_media_common : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int sc_gt_aclk_mmbuf : 1;
        unsigned int sc_gt_pclk_mmbuf : 1;
        unsigned int bitmasken : 16;
    } reg;
} SOC_MEDIA1_CRG_CLKDIV9_UNION;
#endif
#define SOC_MEDIA1_CRG_CLKDIV9_sc_gt_clk_ispfunc4_START (0)
#define SOC_MEDIA1_CRG_CLKDIV9_sc_gt_clk_ispfunc4_END (0)
#define SOC_MEDIA1_CRG_CLKDIV9_sc_gt_clk_ispfunc3_START (1)
#define SOC_MEDIA1_CRG_CLKDIV9_sc_gt_clk_ispfunc3_END (1)
#define SOC_MEDIA1_CRG_CLKDIV9_sc_gt_clk_ispfunc2_START (2)
#define SOC_MEDIA1_CRG_CLKDIV9_sc_gt_clk_ispfunc2_END (2)
#define SOC_MEDIA1_CRG_CLKDIV9_sc_gt_clk_vivobus_START (3)
#define SOC_MEDIA1_CRG_CLKDIV9_sc_gt_clk_vivobus_END (3)
#define SOC_MEDIA1_CRG_CLKDIV9_sc_gt_clk_idi2axi_csid_START (4)
#define SOC_MEDIA1_CRG_CLKDIV9_sc_gt_clk_idi2axi_csid_END (4)
#define SOC_MEDIA1_CRG_CLKDIV9_sc_gt_clk_isp_cpu_START (5)
#define SOC_MEDIA1_CRG_CLKDIV9_sc_gt_clk_isp_cpu_END (5)
#define SOC_MEDIA1_CRG_CLKDIV9_sc_gt_clk_ispfunc_START (6)
#define SOC_MEDIA1_CRG_CLKDIV9_sc_gt_clk_ispfunc_END (6)
#define SOC_MEDIA1_CRG_CLKDIV9_sc_gt_clk_edc0_START (7)
#define SOC_MEDIA1_CRG_CLKDIV9_sc_gt_clk_edc0_END (7)
#define SOC_MEDIA1_CRG_CLKDIV9_sc_gt_clk_ldi1_START (8)
#define SOC_MEDIA1_CRG_CLKDIV9_sc_gt_clk_ldi1_END (8)
#define SOC_MEDIA1_CRG_CLKDIV9_sc_gt_clk_isp_i3c_START (9)
#define SOC_MEDIA1_CRG_CLKDIV9_sc_gt_clk_isp_i3c_END (9)
#define SOC_MEDIA1_CRG_CLKDIV9_sc_gt_clk_ispfunc5_START (10)
#define SOC_MEDIA1_CRG_CLKDIV9_sc_gt_clk_ispfunc5_END (10)
#define SOC_MEDIA1_CRG_CLKDIV9_sc_gt_clk_media_common_START (11)
#define SOC_MEDIA1_CRG_CLKDIV9_sc_gt_clk_media_common_END (11)
#define SOC_MEDIA1_CRG_CLKDIV9_sc_gt_aclk_mmbuf_START (14)
#define SOC_MEDIA1_CRG_CLKDIV9_sc_gt_aclk_mmbuf_END (14)
#define SOC_MEDIA1_CRG_CLKDIV9_sc_gt_pclk_mmbuf_START (15)
#define SOC_MEDIA1_CRG_CLKDIV9_sc_gt_pclk_mmbuf_END (15)
#define SOC_MEDIA1_CRG_CLKDIV9_bitmasken_START (16)
#define SOC_MEDIA1_CRG_CLKDIV9_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sel_mmbuf_pll : 4;
        unsigned int div_ispfunc2 : 6;
        unsigned int div_ispfunc3 : 6;
        unsigned int bitmasken : 16;
    } reg;
} SOC_MEDIA1_CRG_CLKDIV10_UNION;
#endif
#define SOC_MEDIA1_CRG_CLKDIV10_sel_mmbuf_pll_START (0)
#define SOC_MEDIA1_CRG_CLKDIV10_sel_mmbuf_pll_END (3)
#define SOC_MEDIA1_CRG_CLKDIV10_div_ispfunc2_START (4)
#define SOC_MEDIA1_CRG_CLKDIV10_div_ispfunc2_END (9)
#define SOC_MEDIA1_CRG_CLKDIV10_div_ispfunc3_START (10)
#define SOC_MEDIA1_CRG_CLKDIV10_div_ispfunc3_END (15)
#define SOC_MEDIA1_CRG_CLKDIV10_bitmasken_START (16)
#define SOC_MEDIA1_CRG_CLKDIV10_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int vdbat_drop_freq_down_isp_cpu_bypass : 1;
        unsigned int div_isp_cpu_vbat_drop_freq_down : 5;
        unsigned int vdbat_drop_freq_down_ispfunc_bypass : 1;
        unsigned int div_ispfunc_vbat_drop_freq_down : 6;
        unsigned int reserved : 3;
        unsigned int bitmasken : 16;
    } reg;
} SOC_MEDIA1_CRG_CLKDIV11_UNION;
#endif
#define SOC_MEDIA1_CRG_CLKDIV11_vdbat_drop_freq_down_isp_cpu_bypass_START (0)
#define SOC_MEDIA1_CRG_CLKDIV11_vdbat_drop_freq_down_isp_cpu_bypass_END (0)
#define SOC_MEDIA1_CRG_CLKDIV11_div_isp_cpu_vbat_drop_freq_down_START (1)
#define SOC_MEDIA1_CRG_CLKDIV11_div_isp_cpu_vbat_drop_freq_down_END (5)
#define SOC_MEDIA1_CRG_CLKDIV11_vdbat_drop_freq_down_ispfunc_bypass_START (6)
#define SOC_MEDIA1_CRG_CLKDIV11_vdbat_drop_freq_down_ispfunc_bypass_END (6)
#define SOC_MEDIA1_CRG_CLKDIV11_div_ispfunc_vbat_drop_freq_down_START (7)
#define SOC_MEDIA1_CRG_CLKDIV11_div_ispfunc_vbat_drop_freq_down_END (12)
#define SOC_MEDIA1_CRG_CLKDIV11_bitmasken_START (16)
#define SOC_MEDIA1_CRG_CLKDIV11_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int vdbat_drop_freq_down_ispfunc2_bypass : 1;
        unsigned int div_ispfunc2_vbat_drop_freq_down : 5;
        unsigned int vdbat_drop_freq_down_ispfunc3_bypass : 1;
        unsigned int div_ispfunc3_vbat_drop_freq_down : 6;
        unsigned int reserved : 3;
        unsigned int bitmasken : 16;
    } reg;
} SOC_MEDIA1_CRG_CLKDIV12_UNION;
#endif
#define SOC_MEDIA1_CRG_CLKDIV12_vdbat_drop_freq_down_ispfunc2_bypass_START (0)
#define SOC_MEDIA1_CRG_CLKDIV12_vdbat_drop_freq_down_ispfunc2_bypass_END (0)
#define SOC_MEDIA1_CRG_CLKDIV12_div_ispfunc2_vbat_drop_freq_down_START (1)
#define SOC_MEDIA1_CRG_CLKDIV12_div_ispfunc2_vbat_drop_freq_down_END (5)
#define SOC_MEDIA1_CRG_CLKDIV12_vdbat_drop_freq_down_ispfunc3_bypass_START (6)
#define SOC_MEDIA1_CRG_CLKDIV12_vdbat_drop_freq_down_ispfunc3_bypass_END (6)
#define SOC_MEDIA1_CRG_CLKDIV12_div_ispfunc3_vbat_drop_freq_down_START (7)
#define SOC_MEDIA1_CRG_CLKDIV12_div_ispfunc3_vbat_drop_freq_down_END (12)
#define SOC_MEDIA1_CRG_CLKDIV12_bitmasken_START (16)
#define SOC_MEDIA1_CRG_CLKDIV12_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int vdbat_drop_freq_down_ispfunc4_bypass : 1;
        unsigned int div_ispfunc4_vbat_drop_freq_down : 5;
        unsigned int reserved : 10;
        unsigned int bitmasken : 16;
    } reg;
} SOC_MEDIA1_CRG_CLKDIV13_UNION;
#endif
#define SOC_MEDIA1_CRG_CLKDIV13_vdbat_drop_freq_down_ispfunc4_bypass_START (0)
#define SOC_MEDIA1_CRG_CLKDIV13_vdbat_drop_freq_down_ispfunc4_bypass_END (0)
#define SOC_MEDIA1_CRG_CLKDIV13_div_ispfunc4_vbat_drop_freq_down_START (1)
#define SOC_MEDIA1_CRG_CLKDIV13_div_ispfunc4_vbat_drop_freq_down_END (5)
#define SOC_MEDIA1_CRG_CLKDIV13_bitmasken_START (16)
#define SOC_MEDIA1_CRG_CLKDIV13_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int vdbat_drop_freq_down_ispfunc5_bypass : 1;
        unsigned int div_ispfunc5_vbat_drop_freq_down : 5;
        unsigned int reserved : 10;
        unsigned int bitmasken : 16;
    } reg;
} SOC_MEDIA1_CRG_CLKDIV15_UNION;
#endif
#define SOC_MEDIA1_CRG_CLKDIV15_vdbat_drop_freq_down_ispfunc5_bypass_START (0)
#define SOC_MEDIA1_CRG_CLKDIV15_vdbat_drop_freq_down_ispfunc5_bypass_END (0)
#define SOC_MEDIA1_CRG_CLKDIV15_div_ispfunc5_vbat_drop_freq_down_START (1)
#define SOC_MEDIA1_CRG_CLKDIV15_div_ispfunc5_vbat_drop_freq_down_END (5)
#define SOC_MEDIA1_CRG_CLKDIV15_bitmasken_START (16)
#define SOC_MEDIA1_CRG_CLKDIV15_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sc_gt_clk_edc0_sh : 1;
        unsigned int sc_gt_aclk_mbuf_sh : 1;
        unsigned int sc_gt_clk_vivobus_sh : 1;
        unsigned int reserved : 13;
        unsigned int bitmasken : 16;
    } reg;
} SOC_MEDIA1_CRG_CLKDIV16_UNION;
#endif
#define SOC_MEDIA1_CRG_CLKDIV16_sc_gt_clk_edc0_sh_START (0)
#define SOC_MEDIA1_CRG_CLKDIV16_sc_gt_clk_edc0_sh_END (0)
#define SOC_MEDIA1_CRG_CLKDIV16_sc_gt_aclk_mbuf_sh_START (1)
#define SOC_MEDIA1_CRG_CLKDIV16_sc_gt_aclk_mbuf_sh_END (1)
#define SOC_MEDIA1_CRG_CLKDIV16_sc_gt_clk_vivobus_sh_START (2)
#define SOC_MEDIA1_CRG_CLKDIV16_sc_gt_clk_vivobus_sh_END (2)
#define SOC_MEDIA1_CRG_CLKDIV16_bitmasken_START (16)
#define SOC_MEDIA1_CRG_CLKDIV16_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int swdone_clk_ispfunc4_div : 1;
        unsigned int swdone_clk_ispfunc3_div : 1;
        unsigned int swdone_clk_ispfunc2_div : 1;
        unsigned int swdone_clk_vivobus_div : 1;
        unsigned int swdone_clk_idi2axi_csid_div : 1;
        unsigned int swdone_clk_isp_cpu_div : 1;
        unsigned int swdone_clk_ispfunc_div : 1;
        unsigned int swdone_clk_edc0_div : 1;
        unsigned int swdone_clk_ldi1_div : 1;
        unsigned int swdone_clk_isp_i3c_div : 1;
        unsigned int swdone_clk_ispfunc5_div : 1;
        unsigned int swdone_clk_media_common_div : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int swdone_aclk_mmbuf_div : 1;
        unsigned int swdone_pclk_mmbuf_div : 1;
        unsigned int reserved_2 : 16;
    } reg;
} SOC_MEDIA1_CRG_PERI_STAT0_UNION;
#endif
#define SOC_MEDIA1_CRG_PERI_STAT0_swdone_clk_ispfunc4_div_START (0)
#define SOC_MEDIA1_CRG_PERI_STAT0_swdone_clk_ispfunc4_div_END (0)
#define SOC_MEDIA1_CRG_PERI_STAT0_swdone_clk_ispfunc3_div_START (1)
#define SOC_MEDIA1_CRG_PERI_STAT0_swdone_clk_ispfunc3_div_END (1)
#define SOC_MEDIA1_CRG_PERI_STAT0_swdone_clk_ispfunc2_div_START (2)
#define SOC_MEDIA1_CRG_PERI_STAT0_swdone_clk_ispfunc2_div_END (2)
#define SOC_MEDIA1_CRG_PERI_STAT0_swdone_clk_vivobus_div_START (3)
#define SOC_MEDIA1_CRG_PERI_STAT0_swdone_clk_vivobus_div_END (3)
#define SOC_MEDIA1_CRG_PERI_STAT0_swdone_clk_idi2axi_csid_div_START (4)
#define SOC_MEDIA1_CRG_PERI_STAT0_swdone_clk_idi2axi_csid_div_END (4)
#define SOC_MEDIA1_CRG_PERI_STAT0_swdone_clk_isp_cpu_div_START (5)
#define SOC_MEDIA1_CRG_PERI_STAT0_swdone_clk_isp_cpu_div_END (5)
#define SOC_MEDIA1_CRG_PERI_STAT0_swdone_clk_ispfunc_div_START (6)
#define SOC_MEDIA1_CRG_PERI_STAT0_swdone_clk_ispfunc_div_END (6)
#define SOC_MEDIA1_CRG_PERI_STAT0_swdone_clk_edc0_div_START (7)
#define SOC_MEDIA1_CRG_PERI_STAT0_swdone_clk_edc0_div_END (7)
#define SOC_MEDIA1_CRG_PERI_STAT0_swdone_clk_ldi1_div_START (8)
#define SOC_MEDIA1_CRG_PERI_STAT0_swdone_clk_ldi1_div_END (8)
#define SOC_MEDIA1_CRG_PERI_STAT0_swdone_clk_isp_i3c_div_START (9)
#define SOC_MEDIA1_CRG_PERI_STAT0_swdone_clk_isp_i3c_div_END (9)
#define SOC_MEDIA1_CRG_PERI_STAT0_swdone_clk_ispfunc5_div_START (10)
#define SOC_MEDIA1_CRG_PERI_STAT0_swdone_clk_ispfunc5_div_END (10)
#define SOC_MEDIA1_CRG_PERI_STAT0_swdone_clk_media_common_div_START (11)
#define SOC_MEDIA1_CRG_PERI_STAT0_swdone_clk_media_common_div_END (11)
#define SOC_MEDIA1_CRG_PERI_STAT0_swdone_aclk_mmbuf_div_START (14)
#define SOC_MEDIA1_CRG_PERI_STAT0_swdone_aclk_mmbuf_div_END (14)
#define SOC_MEDIA1_CRG_PERI_STAT0_swdone_pclk_mmbuf_div_START (15)
#define SOC_MEDIA1_CRG_PERI_STAT0_swdone_pclk_mmbuf_div_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sw_ack_clk_ispfunc5_sw : 4;
        unsigned int sw_ack_clk_ldi1_sw : 4;
        unsigned int sw_ack_clk_edc0_sw : 4;
        unsigned int sw_ack_clk_ispfunc_sw : 4;
        unsigned int sw_ack_clk_vivobus_sw : 4;
        unsigned int sw_ack_clk_ispfunc3_sw : 4;
        unsigned int sw_ack_clk_ispfunc2_sw : 4;
        unsigned int sw_ack_clk_ispfunc4_sw : 4;
    } reg;
} SOC_MEDIA1_CRG_PERI_STAT1_UNION;
#endif
#define SOC_MEDIA1_CRG_PERI_STAT1_sw_ack_clk_ispfunc5_sw_START (0)
#define SOC_MEDIA1_CRG_PERI_STAT1_sw_ack_clk_ispfunc5_sw_END (3)
#define SOC_MEDIA1_CRG_PERI_STAT1_sw_ack_clk_ldi1_sw_START (4)
#define SOC_MEDIA1_CRG_PERI_STAT1_sw_ack_clk_ldi1_sw_END (7)
#define SOC_MEDIA1_CRG_PERI_STAT1_sw_ack_clk_edc0_sw_START (8)
#define SOC_MEDIA1_CRG_PERI_STAT1_sw_ack_clk_edc0_sw_END (11)
#define SOC_MEDIA1_CRG_PERI_STAT1_sw_ack_clk_ispfunc_sw_START (12)
#define SOC_MEDIA1_CRG_PERI_STAT1_sw_ack_clk_ispfunc_sw_END (15)
#define SOC_MEDIA1_CRG_PERI_STAT1_sw_ack_clk_vivobus_sw_START (16)
#define SOC_MEDIA1_CRG_PERI_STAT1_sw_ack_clk_vivobus_sw_END (19)
#define SOC_MEDIA1_CRG_PERI_STAT1_sw_ack_clk_ispfunc3_sw_START (20)
#define SOC_MEDIA1_CRG_PERI_STAT1_sw_ack_clk_ispfunc3_sw_END (23)
#define SOC_MEDIA1_CRG_PERI_STAT1_sw_ack_clk_ispfunc2_sw_START (24)
#define SOC_MEDIA1_CRG_PERI_STAT1_sw_ack_clk_ispfunc2_sw_END (27)
#define SOC_MEDIA1_CRG_PERI_STAT1_sw_ack_clk_ispfunc4_sw_START (28)
#define SOC_MEDIA1_CRG_PERI_STAT1_sw_ack_clk_ispfunc4_sw_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sw_ack_clk_isp_cpu_sw : 4;
        unsigned int sw_ack_clk_media_common_sw : 4;
        unsigned int sw_ack_aclk_mmbuf_sw : 4;
        unsigned int reserved_0 : 4;
        unsigned int reserved_1 : 4;
        unsigned int reserved_2 : 12;
    } reg;
} SOC_MEDIA1_CRG_PERI_STAT2_UNION;
#endif
#define SOC_MEDIA1_CRG_PERI_STAT2_sw_ack_clk_isp_cpu_sw_START (0)
#define SOC_MEDIA1_CRG_PERI_STAT2_sw_ack_clk_isp_cpu_sw_END (3)
#define SOC_MEDIA1_CRG_PERI_STAT2_sw_ack_clk_media_common_sw_START (4)
#define SOC_MEDIA1_CRG_PERI_STAT2_sw_ack_clk_media_common_sw_END (7)
#define SOC_MEDIA1_CRG_PERI_STAT2_sw_ack_aclk_mmbuf_sw_START (8)
#define SOC_MEDIA1_CRG_PERI_STAT2_sw_ack_aclk_mmbuf_sw_END (11)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0: 1;
        unsigned int reserved_1: 1;
        unsigned int reserved_2: 1;
        unsigned int reserved_3: 1;
        unsigned int reserved_4: 1;
        unsigned int reserved_5: 1;
        unsigned int reserved_6: 1;
        unsigned int reserved_7: 25;
    } reg;
} SOC_MEDIA1_CRG_ISOEN_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0: 1;
        unsigned int reserved_1: 1;
        unsigned int reserved_2: 1;
        unsigned int reserved_3: 1;
        unsigned int reserved_4: 1;
        unsigned int reserved_5: 1;
        unsigned int reserved_6: 1;
        unsigned int reserved_7: 25;
    } reg;
} SOC_MEDIA1_CRG_ISODIS_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0: 1;
        unsigned int reserved_1: 1;
        unsigned int reserved_2: 1;
        unsigned int reserved_3: 1;
        unsigned int reserved_4: 1;
        unsigned int reserved_5: 1;
        unsigned int reserved_6: 1;
        unsigned int reserved_7: 25;
    } reg;
} SOC_MEDIA1_CRG_ISOSTAT_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dss_clkrst_bypass : 1;
        unsigned int media_common_clkrst_bypass : 1;
        unsigned int media1_lm_clkrst_bypass : 1;
        unsigned int media1_trace_clkrst_bypass : 1;
        unsigned int csi_wrapper_cfg_clkrst_bypass : 1;
        unsigned int idi2axi_cfg_clkrst_bypass : 1;
        unsigned int tcu_cfg_clkrst_bypass : 1;
        unsigned int qic_clkrst_flag_vivo_bypass : 1;
        unsigned int qic_clkrst_flag_idi_bypass : 1;
        unsigned int qic_clkrst_flag_dmss_bypass : 1;
        unsigned int qic_clkrst_flag_isp_bypass : 1;
        unsigned int qic_clkrst_flag_dss_bypass : 1;
        unsigned int qic_clkrst_flag_cfg_dss_bypass : 1;
        unsigned int qic_clkrst_flag_cfg_isp_bypass : 1;
        unsigned int reserved : 18;
    } reg;
} SOC_MEDIA1_CRG_IPCLKRST_BYPASS_UNION;
#endif
#define SOC_MEDIA1_CRG_IPCLKRST_BYPASS_dss_clkrst_bypass_START (0)
#define SOC_MEDIA1_CRG_IPCLKRST_BYPASS_dss_clkrst_bypass_END (0)
#define SOC_MEDIA1_CRG_IPCLKRST_BYPASS_media_common_clkrst_bypass_START (1)
#define SOC_MEDIA1_CRG_IPCLKRST_BYPASS_media_common_clkrst_bypass_END (1)
#define SOC_MEDIA1_CRG_IPCLKRST_BYPASS_media1_lm_clkrst_bypass_START (2)
#define SOC_MEDIA1_CRG_IPCLKRST_BYPASS_media1_lm_clkrst_bypass_END (2)
#define SOC_MEDIA1_CRG_IPCLKRST_BYPASS_media1_trace_clkrst_bypass_START (3)
#define SOC_MEDIA1_CRG_IPCLKRST_BYPASS_media1_trace_clkrst_bypass_END (3)
#define SOC_MEDIA1_CRG_IPCLKRST_BYPASS_csi_wrapper_cfg_clkrst_bypass_START (4)
#define SOC_MEDIA1_CRG_IPCLKRST_BYPASS_csi_wrapper_cfg_clkrst_bypass_END (4)
#define SOC_MEDIA1_CRG_IPCLKRST_BYPASS_idi2axi_cfg_clkrst_bypass_START (5)
#define SOC_MEDIA1_CRG_IPCLKRST_BYPASS_idi2axi_cfg_clkrst_bypass_END (5)
#define SOC_MEDIA1_CRG_IPCLKRST_BYPASS_tcu_cfg_clkrst_bypass_START (6)
#define SOC_MEDIA1_CRG_IPCLKRST_BYPASS_tcu_cfg_clkrst_bypass_END (6)
#define SOC_MEDIA1_CRG_IPCLKRST_BYPASS_qic_clkrst_flag_vivo_bypass_START (7)
#define SOC_MEDIA1_CRG_IPCLKRST_BYPASS_qic_clkrst_flag_vivo_bypass_END (7)
#define SOC_MEDIA1_CRG_IPCLKRST_BYPASS_qic_clkrst_flag_idi_bypass_START (8)
#define SOC_MEDIA1_CRG_IPCLKRST_BYPASS_qic_clkrst_flag_idi_bypass_END (8)
#define SOC_MEDIA1_CRG_IPCLKRST_BYPASS_qic_clkrst_flag_dmss_bypass_START (9)
#define SOC_MEDIA1_CRG_IPCLKRST_BYPASS_qic_clkrst_flag_dmss_bypass_END (9)
#define SOC_MEDIA1_CRG_IPCLKRST_BYPASS_qic_clkrst_flag_isp_bypass_START (10)
#define SOC_MEDIA1_CRG_IPCLKRST_BYPASS_qic_clkrst_flag_isp_bypass_END (10)
#define SOC_MEDIA1_CRG_IPCLKRST_BYPASS_qic_clkrst_flag_dss_bypass_START (11)
#define SOC_MEDIA1_CRG_IPCLKRST_BYPASS_qic_clkrst_flag_dss_bypass_END (11)
#define SOC_MEDIA1_CRG_IPCLKRST_BYPASS_qic_clkrst_flag_cfg_dss_bypass_START (12)
#define SOC_MEDIA1_CRG_IPCLKRST_BYPASS_qic_clkrst_flag_cfg_dss_bypass_END (12)
#define SOC_MEDIA1_CRG_IPCLKRST_BYPASS_qic_clkrst_flag_cfg_isp_bypass_START (13)
#define SOC_MEDIA1_CRG_IPCLKRST_BYPASS_qic_clkrst_flag_cfg_isp_bypass_END (13)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ccpu_pd_clk_en : 1;
        unsigned int ccpu_core_clk_en : 2;
        unsigned int ccpu_dbg_clk_en : 2;
        unsigned int ccpu_etm_clk_en : 2;
        unsigned int ccpu_ct_clk_en : 1;
        unsigned int ccpu_atbclk_en : 1;
        unsigned int ccpu_dbgclk_en : 1;
        unsigned int ccpu_m0clk_divnum : 2;
        unsigned int reserved : 4;
        unsigned int bitmasken : 16;
    } reg;
} SOC_MEDIA1_CRG_ISPCPU_CLKEN_UNION;
#endif
#define SOC_MEDIA1_CRG_ISPCPU_CLKEN_ccpu_pd_clk_en_START (0)
#define SOC_MEDIA1_CRG_ISPCPU_CLKEN_ccpu_pd_clk_en_END (0)
#define SOC_MEDIA1_CRG_ISPCPU_CLKEN_ccpu_core_clk_en_START (1)
#define SOC_MEDIA1_CRG_ISPCPU_CLKEN_ccpu_core_clk_en_END (2)
#define SOC_MEDIA1_CRG_ISPCPU_CLKEN_ccpu_dbg_clk_en_START (3)
#define SOC_MEDIA1_CRG_ISPCPU_CLKEN_ccpu_dbg_clk_en_END (4)
#define SOC_MEDIA1_CRG_ISPCPU_CLKEN_ccpu_etm_clk_en_START (5)
#define SOC_MEDIA1_CRG_ISPCPU_CLKEN_ccpu_etm_clk_en_END (6)
#define SOC_MEDIA1_CRG_ISPCPU_CLKEN_ccpu_ct_clk_en_START (7)
#define SOC_MEDIA1_CRG_ISPCPU_CLKEN_ccpu_ct_clk_en_END (7)
#define SOC_MEDIA1_CRG_ISPCPU_CLKEN_ccpu_atbclk_en_START (8)
#define SOC_MEDIA1_CRG_ISPCPU_CLKEN_ccpu_atbclk_en_END (8)
#define SOC_MEDIA1_CRG_ISPCPU_CLKEN_ccpu_dbgclk_en_START (9)
#define SOC_MEDIA1_CRG_ISPCPU_CLKEN_ccpu_dbgclk_en_END (9)
#define SOC_MEDIA1_CRG_ISPCPU_CLKEN_ccpu_m0clk_divnum_START (10)
#define SOC_MEDIA1_CRG_ISPCPU_CLKEN_ccpu_m0clk_divnum_END (11)
#define SOC_MEDIA1_CRG_ISPCPU_CLKEN_bitmasken_START (16)
#define SOC_MEDIA1_CRG_ISPCPU_CLKEN_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ccpu_core_srst_req_n : 2;
        unsigned int ccpu_scu_srst_req_n : 1;
        unsigned int ccpu_ct_srst_req_n : 1;
        unsigned int ccpu_wdt_srst_req_n : 2;
        unsigned int ccpu_dbg_srst_req_n : 2;
        unsigned int ccpu_etm_srst_req_n : 2;
        unsigned int reserved_0 : 1;
        unsigned int ccpu_periph_srst_req_n : 1;
        unsigned int cs_ccpu_dbg_srst_req_n : 1;
        unsigned int ccpu_pd_srst_req_n : 1;
        unsigned int reserved_1 : 18;
    } reg;
} SOC_MEDIA1_CRG_ISPCPU_RSTEN_UNION;
#endif
#define SOC_MEDIA1_CRG_ISPCPU_RSTEN_ccpu_core_srst_req_n_START (0)
#define SOC_MEDIA1_CRG_ISPCPU_RSTEN_ccpu_core_srst_req_n_END (1)
#define SOC_MEDIA1_CRG_ISPCPU_RSTEN_ccpu_scu_srst_req_n_START (2)
#define SOC_MEDIA1_CRG_ISPCPU_RSTEN_ccpu_scu_srst_req_n_END (2)
#define SOC_MEDIA1_CRG_ISPCPU_RSTEN_ccpu_ct_srst_req_n_START (3)
#define SOC_MEDIA1_CRG_ISPCPU_RSTEN_ccpu_ct_srst_req_n_END (3)
#define SOC_MEDIA1_CRG_ISPCPU_RSTEN_ccpu_wdt_srst_req_n_START (4)
#define SOC_MEDIA1_CRG_ISPCPU_RSTEN_ccpu_wdt_srst_req_n_END (5)
#define SOC_MEDIA1_CRG_ISPCPU_RSTEN_ccpu_dbg_srst_req_n_START (6)
#define SOC_MEDIA1_CRG_ISPCPU_RSTEN_ccpu_dbg_srst_req_n_END (7)
#define SOC_MEDIA1_CRG_ISPCPU_RSTEN_ccpu_etm_srst_req_n_START (8)
#define SOC_MEDIA1_CRG_ISPCPU_RSTEN_ccpu_etm_srst_req_n_END (9)
#define SOC_MEDIA1_CRG_ISPCPU_RSTEN_ccpu_periph_srst_req_n_START (11)
#define SOC_MEDIA1_CRG_ISPCPU_RSTEN_ccpu_periph_srst_req_n_END (11)
#define SOC_MEDIA1_CRG_ISPCPU_RSTEN_cs_ccpu_dbg_srst_req_n_START (12)
#define SOC_MEDIA1_CRG_ISPCPU_RSTEN_cs_ccpu_dbg_srst_req_n_END (12)
#define SOC_MEDIA1_CRG_ISPCPU_RSTEN_ccpu_pd_srst_req_n_START (13)
#define SOC_MEDIA1_CRG_ISPCPU_RSTEN_ccpu_pd_srst_req_n_END (13)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ccpu_core_srst_req_n : 2;
        unsigned int ccpu_scu_srst_req_n : 1;
        unsigned int ccpu_ct_srst_req_n : 1;
        unsigned int ccpu_wdt_srst_req_n : 2;
        unsigned int ccpu_dbg_srst_req_n : 2;
        unsigned int ccpu_etm_srst_req_n : 2;
        unsigned int reserved_0 : 1;
        unsigned int ccpu_periph_srst_req_n : 1;
        unsigned int cs_ccpu_dbg_srst_req_n : 1;
        unsigned int ccpu_pd_srst_req_n : 1;
        unsigned int reserved_1 : 18;
    } reg;
} SOC_MEDIA1_CRG_ISPCPU_RSTDIS_UNION;
#endif
#define SOC_MEDIA1_CRG_ISPCPU_RSTDIS_ccpu_core_srst_req_n_START (0)
#define SOC_MEDIA1_CRG_ISPCPU_RSTDIS_ccpu_core_srst_req_n_END (1)
#define SOC_MEDIA1_CRG_ISPCPU_RSTDIS_ccpu_scu_srst_req_n_START (2)
#define SOC_MEDIA1_CRG_ISPCPU_RSTDIS_ccpu_scu_srst_req_n_END (2)
#define SOC_MEDIA1_CRG_ISPCPU_RSTDIS_ccpu_ct_srst_req_n_START (3)
#define SOC_MEDIA1_CRG_ISPCPU_RSTDIS_ccpu_ct_srst_req_n_END (3)
#define SOC_MEDIA1_CRG_ISPCPU_RSTDIS_ccpu_wdt_srst_req_n_START (4)
#define SOC_MEDIA1_CRG_ISPCPU_RSTDIS_ccpu_wdt_srst_req_n_END (5)
#define SOC_MEDIA1_CRG_ISPCPU_RSTDIS_ccpu_dbg_srst_req_n_START (6)
#define SOC_MEDIA1_CRG_ISPCPU_RSTDIS_ccpu_dbg_srst_req_n_END (7)
#define SOC_MEDIA1_CRG_ISPCPU_RSTDIS_ccpu_etm_srst_req_n_START (8)
#define SOC_MEDIA1_CRG_ISPCPU_RSTDIS_ccpu_etm_srst_req_n_END (9)
#define SOC_MEDIA1_CRG_ISPCPU_RSTDIS_ccpu_periph_srst_req_n_START (11)
#define SOC_MEDIA1_CRG_ISPCPU_RSTDIS_ccpu_periph_srst_req_n_END (11)
#define SOC_MEDIA1_CRG_ISPCPU_RSTDIS_cs_ccpu_dbg_srst_req_n_START (12)
#define SOC_MEDIA1_CRG_ISPCPU_RSTDIS_cs_ccpu_dbg_srst_req_n_END (12)
#define SOC_MEDIA1_CRG_ISPCPU_RSTDIS_ccpu_pd_srst_req_n_START (13)
#define SOC_MEDIA1_CRG_ISPCPU_RSTDIS_ccpu_pd_srst_req_n_END (13)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ccpu_core_srst_req_n : 2;
        unsigned int ccpu_scu_srst_req_n : 1;
        unsigned int ccpu_ct_srst_req_n : 1;
        unsigned int ccpu_wdt_srst_req_n : 2;
        unsigned int ccpu_dbg_srst_req_n : 2;
        unsigned int ccpu_etm_srst_req_n : 2;
        unsigned int reserved_0 : 1;
        unsigned int ccpu_periph_srst_req_n : 1;
        unsigned int cs_ccpu_dbg_srst_req_n : 1;
        unsigned int ccpu_pd_srst_req_n : 1;
        unsigned int reserved_1 : 18;
    } reg;
} SOC_MEDIA1_CRG_ISPCPU_RSTSTAT_UNION;
#endif
#define SOC_MEDIA1_CRG_ISPCPU_RSTSTAT_ccpu_core_srst_req_n_START (0)
#define SOC_MEDIA1_CRG_ISPCPU_RSTSTAT_ccpu_core_srst_req_n_END (1)
#define SOC_MEDIA1_CRG_ISPCPU_RSTSTAT_ccpu_scu_srst_req_n_START (2)
#define SOC_MEDIA1_CRG_ISPCPU_RSTSTAT_ccpu_scu_srst_req_n_END (2)
#define SOC_MEDIA1_CRG_ISPCPU_RSTSTAT_ccpu_ct_srst_req_n_START (3)
#define SOC_MEDIA1_CRG_ISPCPU_RSTSTAT_ccpu_ct_srst_req_n_END (3)
#define SOC_MEDIA1_CRG_ISPCPU_RSTSTAT_ccpu_wdt_srst_req_n_START (4)
#define SOC_MEDIA1_CRG_ISPCPU_RSTSTAT_ccpu_wdt_srst_req_n_END (5)
#define SOC_MEDIA1_CRG_ISPCPU_RSTSTAT_ccpu_dbg_srst_req_n_START (6)
#define SOC_MEDIA1_CRG_ISPCPU_RSTSTAT_ccpu_dbg_srst_req_n_END (7)
#define SOC_MEDIA1_CRG_ISPCPU_RSTSTAT_ccpu_etm_srst_req_n_START (8)
#define SOC_MEDIA1_CRG_ISPCPU_RSTSTAT_ccpu_etm_srst_req_n_END (9)
#define SOC_MEDIA1_CRG_ISPCPU_RSTSTAT_ccpu_periph_srst_req_n_START (11)
#define SOC_MEDIA1_CRG_ISPCPU_RSTSTAT_ccpu_periph_srst_req_n_END (11)
#define SOC_MEDIA1_CRG_ISPCPU_RSTSTAT_cs_ccpu_dbg_srst_req_n_START (12)
#define SOC_MEDIA1_CRG_ISPCPU_RSTSTAT_cs_ccpu_dbg_srst_req_n_END (12)
#define SOC_MEDIA1_CRG_ISPCPU_RSTSTAT_ccpu_pd_srst_req_n_START (13)
#define SOC_MEDIA1_CRG_ISPCPU_RSTSTAT_ccpu_pd_srst_req_n_END (13)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int c0_sys_halt_r : 1;
        unsigned int c0_sys_sleep_r : 1;
        unsigned int c1_sys_halt_r : 1;
        unsigned int c1_sys_sleep_r : 1;
        unsigned int c0_sys_tf_halt_r : 1;
        unsigned int c0_sys_sleep_mode : 3;
        unsigned int c1_sys_tf_halt_r : 1;
        unsigned int c1_sys_sleep_mode : 3;
        unsigned int c0_arc_halt_ack : 1;
        unsigned int c0_arc_run_ack : 1;
        unsigned int c1_arc_halt_ack : 1;
        unsigned int c1_arc_run_ack : 1;
        unsigned int reserved : 16;
    } reg;
} SOC_MEDIA1_CRG_ISP_CPU_STATE0_UNION;
#endif
#define SOC_MEDIA1_CRG_ISP_CPU_STATE0_c0_sys_halt_r_START (0)
#define SOC_MEDIA1_CRG_ISP_CPU_STATE0_c0_sys_halt_r_END (0)
#define SOC_MEDIA1_CRG_ISP_CPU_STATE0_c0_sys_sleep_r_START (1)
#define SOC_MEDIA1_CRG_ISP_CPU_STATE0_c0_sys_sleep_r_END (1)
#define SOC_MEDIA1_CRG_ISP_CPU_STATE0_c1_sys_halt_r_START (2)
#define SOC_MEDIA1_CRG_ISP_CPU_STATE0_c1_sys_halt_r_END (2)
#define SOC_MEDIA1_CRG_ISP_CPU_STATE0_c1_sys_sleep_r_START (3)
#define SOC_MEDIA1_CRG_ISP_CPU_STATE0_c1_sys_sleep_r_END (3)
#define SOC_MEDIA1_CRG_ISP_CPU_STATE0_c0_sys_tf_halt_r_START (4)
#define SOC_MEDIA1_CRG_ISP_CPU_STATE0_c0_sys_tf_halt_r_END (4)
#define SOC_MEDIA1_CRG_ISP_CPU_STATE0_c0_sys_sleep_mode_START (5)
#define SOC_MEDIA1_CRG_ISP_CPU_STATE0_c0_sys_sleep_mode_END (7)
#define SOC_MEDIA1_CRG_ISP_CPU_STATE0_c1_sys_tf_halt_r_START (8)
#define SOC_MEDIA1_CRG_ISP_CPU_STATE0_c1_sys_tf_halt_r_END (8)
#define SOC_MEDIA1_CRG_ISP_CPU_STATE0_c1_sys_sleep_mode_START (9)
#define SOC_MEDIA1_CRG_ISP_CPU_STATE0_c1_sys_sleep_mode_END (11)
#define SOC_MEDIA1_CRG_ISP_CPU_STATE0_c0_arc_halt_ack_START (12)
#define SOC_MEDIA1_CRG_ISP_CPU_STATE0_c0_arc_halt_ack_END (12)
#define SOC_MEDIA1_CRG_ISP_CPU_STATE0_c0_arc_run_ack_START (13)
#define SOC_MEDIA1_CRG_ISP_CPU_STATE0_c0_arc_run_ack_END (13)
#define SOC_MEDIA1_CRG_ISP_CPU_STATE0_c1_arc_halt_ack_START (14)
#define SOC_MEDIA1_CRG_ISP_CPU_STATE0_c1_arc_halt_ack_END (14)
#define SOC_MEDIA1_CRG_ISP_CPU_STATE0_c1_arc_run_ack_START (15)
#define SOC_MEDIA1_CRG_ISP_CPU_STATE0_c1_arc_run_ack_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_MEDIA1_CRG_ISP_CPU_STATE1_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_MEDIA1_CRG_PERI_AUTODIV0_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dmss_auto_waitcfg_in : 10;
        unsigned int dmss_auto_waitcfg_out : 10;
        unsigned int reserved : 12;
    } reg;
} SOC_MEDIA1_CRG_PERI_AUTODIV1_UNION;
#endif
#define SOC_MEDIA1_CRG_PERI_AUTODIV1_dmss_auto_waitcfg_in_START (0)
#define SOC_MEDIA1_CRG_PERI_AUTODIV1_dmss_auto_waitcfg_in_END (9)
#define SOC_MEDIA1_CRG_PERI_AUTODIV1_dmss_auto_waitcfg_out_START (10)
#define SOC_MEDIA1_CRG_PERI_AUTODIV1_dmss_auto_waitcfg_out_END (19)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int vivobus_dss_bypass : 1;
        unsigned int vivobus_vivo_bypass : 1;
        unsigned int vivobus_isprt_bypass : 1;
        unsigned int vivobus_ispnrt_bypass : 1;
        unsigned int vivobus_ispcnn_bypass : 1;
        unsigned int vivobus_isparc_bypass : 1;
        unsigned int reserved : 21;
        unsigned int vivobus_dss_cfg_bypass : 1;
        unsigned int vivobus_vivo_cfg_bypass : 1;
        unsigned int vivobus_vivo_tb_bypass : 1;
        unsigned int vivobus_isp_cfg_bypass : 1;
        unsigned int vivobus_perf_idle_bypass : 1;
    } reg;
} SOC_MEDIA1_CRG_PERI_AUTODIV2_UNION;
#endif
#define SOC_MEDIA1_CRG_PERI_AUTODIV2_vivobus_dss_bypass_START (0)
#define SOC_MEDIA1_CRG_PERI_AUTODIV2_vivobus_dss_bypass_END (0)
#define SOC_MEDIA1_CRG_PERI_AUTODIV2_vivobus_vivo_bypass_START (1)
#define SOC_MEDIA1_CRG_PERI_AUTODIV2_vivobus_vivo_bypass_END (1)
#define SOC_MEDIA1_CRG_PERI_AUTODIV2_vivobus_isprt_bypass_START (2)
#define SOC_MEDIA1_CRG_PERI_AUTODIV2_vivobus_isprt_bypass_END (2)
#define SOC_MEDIA1_CRG_PERI_AUTODIV2_vivobus_ispnrt_bypass_START (3)
#define SOC_MEDIA1_CRG_PERI_AUTODIV2_vivobus_ispnrt_bypass_END (3)
#define SOC_MEDIA1_CRG_PERI_AUTODIV2_vivobus_ispcnn_bypass_START (4)
#define SOC_MEDIA1_CRG_PERI_AUTODIV2_vivobus_ispcnn_bypass_END (4)
#define SOC_MEDIA1_CRG_PERI_AUTODIV2_vivobus_isparc_bypass_START (5)
#define SOC_MEDIA1_CRG_PERI_AUTODIV2_vivobus_isparc_bypass_END (5)
#define SOC_MEDIA1_CRG_PERI_AUTODIV2_vivobus_dss_cfg_bypass_START (27)
#define SOC_MEDIA1_CRG_PERI_AUTODIV2_vivobus_dss_cfg_bypass_END (27)
#define SOC_MEDIA1_CRG_PERI_AUTODIV2_vivobus_vivo_cfg_bypass_START (28)
#define SOC_MEDIA1_CRG_PERI_AUTODIV2_vivobus_vivo_cfg_bypass_END (28)
#define SOC_MEDIA1_CRG_PERI_AUTODIV2_vivobus_vivo_tb_bypass_START (29)
#define SOC_MEDIA1_CRG_PERI_AUTODIV2_vivobus_vivo_tb_bypass_END (29)
#define SOC_MEDIA1_CRG_PERI_AUTODIV2_vivobus_isp_cfg_bypass_START (30)
#define SOC_MEDIA1_CRG_PERI_AUTODIV2_vivobus_isp_cfg_bypass_END (30)
#define SOC_MEDIA1_CRG_PERI_AUTODIV2_vivobus_perf_idle_bypass_START (31)
#define SOC_MEDIA1_CRG_PERI_AUTODIV2_vivobus_perf_idle_bypass_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int vivobus_div_auto_reduce_bypass : 1;
        unsigned int vivobus_auto_waitcfg_in : 10;
        unsigned int vivobus_auto_waitcfg_out : 10;
        unsigned int vivobus_div_auto_cfg : 6;
        unsigned int reserved : 5;
    } reg;
} SOC_MEDIA1_CRG_PERI_AUTODIV3_UNION;
#endif
#define SOC_MEDIA1_CRG_PERI_AUTODIV3_vivobus_div_auto_reduce_bypass_START (0)
#define SOC_MEDIA1_CRG_PERI_AUTODIV3_vivobus_div_auto_reduce_bypass_END (0)
#define SOC_MEDIA1_CRG_PERI_AUTODIV3_vivobus_auto_waitcfg_in_START (1)
#define SOC_MEDIA1_CRG_PERI_AUTODIV3_vivobus_auto_waitcfg_in_END (10)
#define SOC_MEDIA1_CRG_PERI_AUTODIV3_vivobus_auto_waitcfg_out_START (11)
#define SOC_MEDIA1_CRG_PERI_AUTODIV3_vivobus_auto_waitcfg_out_END (20)
#define SOC_MEDIA1_CRG_PERI_AUTODIV3_vivobus_div_auto_cfg_START (21)
#define SOC_MEDIA1_CRG_PERI_AUTODIV3_vivobus_div_auto_cfg_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int vbat_drop_freq_down_isp_cpu_stat : 1;
        unsigned int vbat_drop_freq_down_ispfunc_stat : 1;
        unsigned int autodiv_vivobus_stat : 1;
        unsigned int vbat_drop_freq_down_ispfunc2_stat : 1;
        unsigned int reserved_1 : 1;
        unsigned int vbat_drop_freq_down_ispfunc3_stat : 1;
        unsigned int vbat_drop_freq_down_ispfunc4_stat : 1;
        unsigned int vbat_drop_freq_down_ispfunc5_stat : 1;
        unsigned int reserved_2 : 23;
    } reg;
} SOC_MEDIA1_CRG_MEDIA_AUTODIV_STAT_UNION;
#endif
#define SOC_MEDIA1_CRG_MEDIA_AUTODIV_STAT_vbat_drop_freq_down_isp_cpu_stat_START (1)
#define SOC_MEDIA1_CRG_MEDIA_AUTODIV_STAT_vbat_drop_freq_down_isp_cpu_stat_END (1)
#define SOC_MEDIA1_CRG_MEDIA_AUTODIV_STAT_vbat_drop_freq_down_ispfunc_stat_START (2)
#define SOC_MEDIA1_CRG_MEDIA_AUTODIV_STAT_vbat_drop_freq_down_ispfunc_stat_END (2)
#define SOC_MEDIA1_CRG_MEDIA_AUTODIV_STAT_autodiv_vivobus_stat_START (3)
#define SOC_MEDIA1_CRG_MEDIA_AUTODIV_STAT_autodiv_vivobus_stat_END (3)
#define SOC_MEDIA1_CRG_MEDIA_AUTODIV_STAT_vbat_drop_freq_down_ispfunc2_stat_START (4)
#define SOC_MEDIA1_CRG_MEDIA_AUTODIV_STAT_vbat_drop_freq_down_ispfunc2_stat_END (4)
#define SOC_MEDIA1_CRG_MEDIA_AUTODIV_STAT_vbat_drop_freq_down_ispfunc3_stat_START (6)
#define SOC_MEDIA1_CRG_MEDIA_AUTODIV_STAT_vbat_drop_freq_down_ispfunc3_stat_END (6)
#define SOC_MEDIA1_CRG_MEDIA_AUTODIV_STAT_vbat_drop_freq_down_ispfunc4_stat_START (7)
#define SOC_MEDIA1_CRG_MEDIA_AUTODIV_STAT_vbat_drop_freq_down_ispfunc4_stat_END (7)
#define SOC_MEDIA1_CRG_MEDIA_AUTODIV_STAT_vbat_drop_freq_down_ispfunc5_stat_START (8)
#define SOC_MEDIA1_CRG_MEDIA_AUTODIV_STAT_vbat_drop_freq_down_ispfunc5_stat_END (8)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int mmbuf_mem_ctrl_s : 26;
        unsigned int reserved : 6;
    } reg;
} SOC_MEDIA1_CRG_MMBUF_MEM_CTRL_UNION;
#endif
#define SOC_MEDIA1_CRG_MMBUF_MEM_CTRL_mmbuf_mem_ctrl_s_START (0)
#define SOC_MEDIA1_CRG_MMBUF_MEM_CTRL_mmbuf_mem_ctrl_s_END (25)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int isptomed1_reserve_stat : 8;
        unsigned int reserved : 24;
    } reg;
} SOC_MEDIA1_CRG_ISPTOMED1_RESERVE_OUT_UNION;
#endif
#define SOC_MEDIA1_CRG_ISPTOMED1_RESERVE_OUT_isptomed1_reserve_stat_START (0)
#define SOC_MEDIA1_CRG_ISPTOMED1_RESERVE_OUT_isptomed1_reserve_stat_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int intr_clr_mediabus_nonidle_pend : 1;
        unsigned int intr_mask_vivobus_nonidle_pend : 1;
        unsigned int intr_mask_dispbus_nonidle_pend : 1;
        unsigned int intr_mask_ispbus_nonidle_pend : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 11;
        unsigned int bitmasken : 16;
    } reg;
} SOC_MEDIA1_CRG_INTR_QIC_BUS_NONIDLE_PEND_UNION;
#endif
#define SOC_MEDIA1_CRG_INTR_QIC_BUS_NONIDLE_PEND_intr_clr_mediabus_nonidle_pend_START (0)
#define SOC_MEDIA1_CRG_INTR_QIC_BUS_NONIDLE_PEND_intr_clr_mediabus_nonidle_pend_END (0)
#define SOC_MEDIA1_CRG_INTR_QIC_BUS_NONIDLE_PEND_intr_mask_vivobus_nonidle_pend_START (1)
#define SOC_MEDIA1_CRG_INTR_QIC_BUS_NONIDLE_PEND_intr_mask_vivobus_nonidle_pend_END (1)
#define SOC_MEDIA1_CRG_INTR_QIC_BUS_NONIDLE_PEND_intr_mask_dispbus_nonidle_pend_START (2)
#define SOC_MEDIA1_CRG_INTR_QIC_BUS_NONIDLE_PEND_intr_mask_dispbus_nonidle_pend_END (2)
#define SOC_MEDIA1_CRG_INTR_QIC_BUS_NONIDLE_PEND_intr_mask_ispbus_nonidle_pend_START (3)
#define SOC_MEDIA1_CRG_INTR_QIC_BUS_NONIDLE_PEND_intr_mask_ispbus_nonidle_pend_END (3)
#define SOC_MEDIA1_CRG_INTR_QIC_BUS_NONIDLE_PEND_bitmasken_START (16)
#define SOC_MEDIA1_CRG_INTR_QIC_BUS_NONIDLE_PEND_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int intr_stat_vivobus_nonidle_pend : 1;
        unsigned int intr_stat_dispbus_nonidle_pend : 1;
        unsigned int intr_stat_ispbus_nonidle_pend : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 12;
        unsigned int reserved_2 : 16;
    } reg;
} SOC_MEDIA1_CRG_INTR_STAT_QIC_BUS_NONIDLE_PEND_UNION;
#endif
#define SOC_MEDIA1_CRG_INTR_STAT_QIC_BUS_NONIDLE_PEND_intr_stat_vivobus_nonidle_pend_START (0)
#define SOC_MEDIA1_CRG_INTR_STAT_QIC_BUS_NONIDLE_PEND_intr_stat_vivobus_nonidle_pend_END (0)
#define SOC_MEDIA1_CRG_INTR_STAT_QIC_BUS_NONIDLE_PEND_intr_stat_dispbus_nonidle_pend_START (1)
#define SOC_MEDIA1_CRG_INTR_STAT_QIC_BUS_NONIDLE_PEND_intr_stat_dispbus_nonidle_pend_END (1)
#define SOC_MEDIA1_CRG_INTR_STAT_QIC_BUS_NONIDLE_PEND_intr_stat_ispbus_nonidle_pend_START (2)
#define SOC_MEDIA1_CRG_INTR_STAT_QIC_BUS_NONIDLE_PEND_intr_stat_ispbus_nonidle_pend_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_pcr_cg_clk_ispfunc3_bypass : 1;
        unsigned int gt_pcr_cg_clk_ispfunc5_bypass : 1;
        unsigned int reserved : 14;
        unsigned int bitmasken : 16;
    } reg;
} SOC_MEDIA1_CRG_PCR_CG_BYPASS_CTRL0_UNION;
#endif
#define SOC_MEDIA1_CRG_PCR_CG_BYPASS_CTRL0_gt_pcr_cg_clk_ispfunc3_bypass_START (0)
#define SOC_MEDIA1_CRG_PCR_CG_BYPASS_CTRL0_gt_pcr_cg_clk_ispfunc3_bypass_END (0)
#define SOC_MEDIA1_CRG_PCR_CG_BYPASS_CTRL0_gt_pcr_cg_clk_ispfunc5_bypass_START (1)
#define SOC_MEDIA1_CRG_PCR_CG_BYPASS_CTRL0_gt_pcr_cg_clk_ispfunc5_bypass_END (1)
#define SOC_MEDIA1_CRG_PCR_CG_BYPASS_CTRL0_bitmasken_START (16)
#define SOC_MEDIA1_CRG_PCR_CG_BYPASS_CTRL0_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pcr_cg_pstate1_ispfunc3 : 32;
    } reg;
} SOC_MEDIA1_CRG_PCR_CG_PSTATE1_ISPFUNC3_UNION;
#endif
#define SOC_MEDIA1_CRG_PCR_CG_PSTATE1_ISPFUNC3_pcr_cg_pstate1_ispfunc3_START (0)
#define SOC_MEDIA1_CRG_PCR_CG_PSTATE1_ISPFUNC3_pcr_cg_pstate1_ispfunc3_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pcr_cg_pstate2_ispfunc3 : 32;
    } reg;
} SOC_MEDIA1_CRG_PCR_CG_PSTATE2_ISPFUNC3_UNION;
#endif
#define SOC_MEDIA1_CRG_PCR_CG_PSTATE2_ISPFUNC3_pcr_cg_pstate2_ispfunc3_START (0)
#define SOC_MEDIA1_CRG_PCR_CG_PSTATE2_ISPFUNC3_pcr_cg_pstate2_ispfunc3_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pcr_cg_pstate3_ispfunc3 : 32;
    } reg;
} SOC_MEDIA1_CRG_PCR_CG_PSTATE3_ISPFUNC3_UNION;
#endif
#define SOC_MEDIA1_CRG_PCR_CG_PSTATE3_ISPFUNC3_pcr_cg_pstate3_ispfunc3_START (0)
#define SOC_MEDIA1_CRG_PCR_CG_PSTATE3_ISPFUNC3_pcr_cg_pstate3_ispfunc3_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pcr_cg_pstate4_ispfunc3 : 32;
    } reg;
} SOC_MEDIA1_CRG_PCR_CG_PSTATE4_ISPFUNC3_UNION;
#endif
#define SOC_MEDIA1_CRG_PCR_CG_PSTATE4_ISPFUNC3_pcr_cg_pstate4_ispfunc3_START (0)
#define SOC_MEDIA1_CRG_PCR_CG_PSTATE4_ISPFUNC3_pcr_cg_pstate4_ispfunc3_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pcr_cg_pstate1_ispfunc5 : 32;
    } reg;
} SOC_MEDIA1_CRG_PCR_CG_PSTATE1_ISPFUNC5_UNION;
#endif
#define SOC_MEDIA1_CRG_PCR_CG_PSTATE1_ISPFUNC5_pcr_cg_pstate1_ispfunc5_START (0)
#define SOC_MEDIA1_CRG_PCR_CG_PSTATE1_ISPFUNC5_pcr_cg_pstate1_ispfunc5_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pcr_cg_pstate2_ispfunc5 : 32;
    } reg;
} SOC_MEDIA1_CRG_PCR_CG_PSTATE2_ISPFUNC5_UNION;
#endif
#define SOC_MEDIA1_CRG_PCR_CG_PSTATE2_ISPFUNC5_pcr_cg_pstate2_ispfunc5_START (0)
#define SOC_MEDIA1_CRG_PCR_CG_PSTATE2_ISPFUNC5_pcr_cg_pstate2_ispfunc5_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pcr_cg_pstate3_ispfunc5 : 32;
    } reg;
} SOC_MEDIA1_CRG_PCR_CG_PSTATE3_ISPFUNC5_UNION;
#endif
#define SOC_MEDIA1_CRG_PCR_CG_PSTATE3_ISPFUNC5_pcr_cg_pstate3_ispfunc5_START (0)
#define SOC_MEDIA1_CRG_PCR_CG_PSTATE3_ISPFUNC5_pcr_cg_pstate3_ispfunc5_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pcr_cg_pstate4_ispfunc5 : 32;
    } reg;
} SOC_MEDIA1_CRG_PCR_CG_PSTATE4_ISPFUNC5_UNION;
#endif
#define SOC_MEDIA1_CRG_PCR_CG_PSTATE4_ISPFUNC5_pcr_cg_pstate4_ispfunc5_START (0)
#define SOC_MEDIA1_CRG_PCR_CG_PSTATE4_ISPFUNC5_pcr_cg_pstate4_ispfunc5_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pll_en : 1;
        unsigned int pll_bp : 1;
        unsigned int pll_refdiv : 6;
        unsigned int pll_fbdiv : 12;
        unsigned int pll_postdiv1 : 3;
        unsigned int pll_postdiv2 : 3;
        unsigned int pll_int_mod : 1;
        unsigned int pll_cfg_vld : 1;
        unsigned int gt_clk_pll : 1;
        unsigned int pll_lock : 1;
        unsigned int reserved : 2;
    } reg;
} SOC_MEDIA1_CRG_SC_PLL_CTRL0_UNION;
#endif
#define SOC_MEDIA1_CRG_SC_PLL_CTRL0_pll_en_START (0)
#define SOC_MEDIA1_CRG_SC_PLL_CTRL0_pll_en_END (0)
#define SOC_MEDIA1_CRG_SC_PLL_CTRL0_pll_bp_START (1)
#define SOC_MEDIA1_CRG_SC_PLL_CTRL0_pll_bp_END (1)
#define SOC_MEDIA1_CRG_SC_PLL_CTRL0_pll_refdiv_START (2)
#define SOC_MEDIA1_CRG_SC_PLL_CTRL0_pll_refdiv_END (7)
#define SOC_MEDIA1_CRG_SC_PLL_CTRL0_pll_fbdiv_START (8)
#define SOC_MEDIA1_CRG_SC_PLL_CTRL0_pll_fbdiv_END (19)
#define SOC_MEDIA1_CRG_SC_PLL_CTRL0_pll_postdiv1_START (20)
#define SOC_MEDIA1_CRG_SC_PLL_CTRL0_pll_postdiv1_END (22)
#define SOC_MEDIA1_CRG_SC_PLL_CTRL0_pll_postdiv2_START (23)
#define SOC_MEDIA1_CRG_SC_PLL_CTRL0_pll_postdiv2_END (25)
#define SOC_MEDIA1_CRG_SC_PLL_CTRL0_pll_int_mod_START (26)
#define SOC_MEDIA1_CRG_SC_PLL_CTRL0_pll_int_mod_END (26)
#define SOC_MEDIA1_CRG_SC_PLL_CTRL0_pll_cfg_vld_START (27)
#define SOC_MEDIA1_CRG_SC_PLL_CTRL0_pll_cfg_vld_END (27)
#define SOC_MEDIA1_CRG_SC_PLL_CTRL0_gt_clk_pll_START (28)
#define SOC_MEDIA1_CRG_SC_PLL_CTRL0_gt_clk_pll_END (28)
#define SOC_MEDIA1_CRG_SC_PLL_CTRL0_pll_lock_START (29)
#define SOC_MEDIA1_CRG_SC_PLL_CTRL0_pll_lock_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 3;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 3;
        unsigned int pll_bwprog : 3;
        unsigned int pll_bwprogen : 1;
        unsigned int pll_foutvco_en : 1;
        unsigned int pll_offsetcalbyp : 1;
        unsigned int pll_offsetcalcnt : 3;
        unsigned int pll_offsetcalen : 1;
        unsigned int pll_offsetcalin : 12;
        unsigned int pll_offsetfastcal : 1;
        unsigned int reserved_4 : 1;
    } reg;
} SOC_MEDIA1_CRG_SC_PLL_CTRL_A_UNION;
#endif
#define SOC_MEDIA1_CRG_SC_PLL_CTRL_A_pll_bwprog_START (8)
#define SOC_MEDIA1_CRG_SC_PLL_CTRL_A_pll_bwprog_END (10)
#define SOC_MEDIA1_CRG_SC_PLL_CTRL_A_pll_bwprogen_START (11)
#define SOC_MEDIA1_CRG_SC_PLL_CTRL_A_pll_bwprogen_END (11)
#define SOC_MEDIA1_CRG_SC_PLL_CTRL_A_pll_foutvco_en_START (12)
#define SOC_MEDIA1_CRG_SC_PLL_CTRL_A_pll_foutvco_en_END (12)
#define SOC_MEDIA1_CRG_SC_PLL_CTRL_A_pll_offsetcalbyp_START (13)
#define SOC_MEDIA1_CRG_SC_PLL_CTRL_A_pll_offsetcalbyp_END (13)
#define SOC_MEDIA1_CRG_SC_PLL_CTRL_A_pll_offsetcalcnt_START (14)
#define SOC_MEDIA1_CRG_SC_PLL_CTRL_A_pll_offsetcalcnt_END (16)
#define SOC_MEDIA1_CRG_SC_PLL_CTRL_A_pll_offsetcalen_START (17)
#define SOC_MEDIA1_CRG_SC_PLL_CTRL_A_pll_offsetcalen_END (17)
#define SOC_MEDIA1_CRG_SC_PLL_CTRL_A_pll_offsetcalin_START (18)
#define SOC_MEDIA1_CRG_SC_PLL_CTRL_A_pll_offsetcalin_END (29)
#define SOC_MEDIA1_CRG_SC_PLL_CTRL_A_pll_offsetfastcal_START (30)
#define SOC_MEDIA1_CRG_SC_PLL_CTRL_A_pll_offsetfastcal_END (30)
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
} SOC_MEDIA1_CRG_SC_PLL_CTRL_B_UNION;
#endif
#define SOC_MEDIA1_CRG_SC_PLL_CTRL_B_pll_vcosel_START (0)
#define SOC_MEDIA1_CRG_SC_PLL_CTRL_B_pll_vcosel_END (1)
#define SOC_MEDIA1_CRG_SC_PLL_CTRL_B_pll_qpiprog_START (2)
#define SOC_MEDIA1_CRG_SC_PLL_CTRL_B_pll_qpiprog_END (4)
#define SOC_MEDIA1_CRG_SC_PLL_CTRL_B_pll_qpiprogen_START (5)
#define SOC_MEDIA1_CRG_SC_PLL_CTRL_B_pll_qpiprogen_END (5)
#define SOC_MEDIA1_CRG_SC_PLL_CTRL_B_pll_qpiskewprog_START (6)
#define SOC_MEDIA1_CRG_SC_PLL_CTRL_B_pll_qpiskewprog_END (8)
#define SOC_MEDIA1_CRG_SC_PLL_CTRL_B_pll_qpiskewprogen_START (9)
#define SOC_MEDIA1_CRG_SC_PLL_CTRL_B_pll_qpiskewprogen_END (9)
#define SOC_MEDIA1_CRG_SC_PLL_CTRL_B_pll_fprog_START (10)
#define SOC_MEDIA1_CRG_SC_PLL_CTRL_B_pll_fprog_END (12)
#define SOC_MEDIA1_CRG_SC_PLL_CTRL_B_pll_fprogen_START (13)
#define SOC_MEDIA1_CRG_SC_PLL_CTRL_B_pll_fprogen_END (13)
#define SOC_MEDIA1_CRG_SC_PLL_CTRL_B_pll_reserved_START (16)
#define SOC_MEDIA1_CRG_SC_PLL_CTRL_B_pll_reserved_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pll_clk_tp_sel : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_MEDIA1_CRG_PLL_CLK_TP_SEL_UNION;
#endif
#define SOC_MEDIA1_CRG_PLL_CLK_TP_SEL_pll_clk_tp_sel_START (0)
#define SOC_MEDIA1_CRG_PLL_CLK_TP_SEL_pll_clk_tp_sel_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pll_en : 1;
        unsigned int ref_div_ratio : 3;
        unsigned int fbdiv : 17;
        unsigned int ckr_invert_en : 1;
        unsigned int cvr : 1;
        unsigned int ref_clk_sel : 1;
        unsigned int integer_offset : 8;
    } reg;
} SOC_MEDIA1_CRG_ADPLLS_CFG0_UNION;
#endif
#define SOC_MEDIA1_CRG_ADPLLS_CFG0_pll_en_START (0)
#define SOC_MEDIA1_CRG_ADPLLS_CFG0_pll_en_END (0)
#define SOC_MEDIA1_CRG_ADPLLS_CFG0_ref_div_ratio_START (1)
#define SOC_MEDIA1_CRG_ADPLLS_CFG0_ref_div_ratio_END (3)
#define SOC_MEDIA1_CRG_ADPLLS_CFG0_fbdiv_START (4)
#define SOC_MEDIA1_CRG_ADPLLS_CFG0_fbdiv_END (20)
#define SOC_MEDIA1_CRG_ADPLLS_CFG0_ckr_invert_en_START (21)
#define SOC_MEDIA1_CRG_ADPLLS_CFG0_ckr_invert_en_END (21)
#define SOC_MEDIA1_CRG_ADPLLS_CFG0_cvr_START (22)
#define SOC_MEDIA1_CRG_ADPLLS_CFG0_cvr_END (22)
#define SOC_MEDIA1_CRG_ADPLLS_CFG0_ref_clk_sel_START (23)
#define SOC_MEDIA1_CRG_ADPLLS_CFG0_ref_clk_sel_END (23)
#define SOC_MEDIA1_CRG_ADPLLS_CFG0_integer_offset_START (24)
#define SOC_MEDIA1_CRG_ADPLLS_CFG0_integer_offset_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int frac : 15;
        unsigned int reserved_0 : 1;
        unsigned int dig_dly : 10;
        unsigned int en_dco_dly : 2;
        unsigned int ckr_clk_en : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
    } reg;
} SOC_MEDIA1_CRG_ADPLLS_CFG1_UNION;
#endif
#define SOC_MEDIA1_CRG_ADPLLS_CFG1_frac_START (0)
#define SOC_MEDIA1_CRG_ADPLLS_CFG1_frac_END (14)
#define SOC_MEDIA1_CRG_ADPLLS_CFG1_dig_dly_START (16)
#define SOC_MEDIA1_CRG_ADPLLS_CFG1_dig_dly_END (25)
#define SOC_MEDIA1_CRG_ADPLLS_CFG1_en_dco_dly_START (26)
#define SOC_MEDIA1_CRG_ADPLLS_CFG1_en_dco_dly_END (27)
#define SOC_MEDIA1_CRG_ADPLLS_CFG1_ckr_clk_en_START (28)
#define SOC_MEDIA1_CRG_ADPLLS_CFG1_ckr_clk_en_END (28)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int remedy_cnt : 3;
        unsigned int lock_dec_comp_value : 14;
        unsigned int calib_comp_value : 9;
        unsigned int reserved_1 : 1;
        unsigned int calib_autogt_bypass : 1;
        unsigned int pll_lock_sel : 1;
        unsigned int reserved_2 : 2;
    } reg;
} SOC_MEDIA1_CRG_ADPLLS_CFG2_UNION;
#endif
#define SOC_MEDIA1_CRG_ADPLLS_CFG2_remedy_cnt_START (1)
#define SOC_MEDIA1_CRG_ADPLLS_CFG2_remedy_cnt_END (3)
#define SOC_MEDIA1_CRG_ADPLLS_CFG2_lock_dec_comp_value_START (4)
#define SOC_MEDIA1_CRG_ADPLLS_CFG2_lock_dec_comp_value_END (17)
#define SOC_MEDIA1_CRG_ADPLLS_CFG2_calib_comp_value_START (18)
#define SOC_MEDIA1_CRG_ADPLLS_CFG2_calib_comp_value_END (26)
#define SOC_MEDIA1_CRG_ADPLLS_CFG2_calib_autogt_bypass_START (28)
#define SOC_MEDIA1_CRG_ADPLLS_CFG2_calib_autogt_bypass_END (28)
#define SOC_MEDIA1_CRG_ADPLLS_CFG2_pll_lock_sel_START (29)
#define SOC_MEDIA1_CRG_ADPLLS_CFG2_pll_lock_sel_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pfd_filter_en : 1;
        unsigned int dither_en : 1;
        unsigned int dsm_mash_sel : 1;
        unsigned int dco_dsm_en : 1;
        unsigned int large_phe_thr : 17;
        unsigned int calib_div_sel : 3;
        unsigned int reserved_0 : 2;
        unsigned int sel_clk_ref_lost : 2;
        unsigned int sel_clk_dsm : 2;
        unsigned int dco_inv_flag : 1;
        unsigned int reserved_1 : 1;
    } reg;
} SOC_MEDIA1_CRG_ADPLLS_CFG3_UNION;
#endif
#define SOC_MEDIA1_CRG_ADPLLS_CFG3_pfd_filter_en_START (0)
#define SOC_MEDIA1_CRG_ADPLLS_CFG3_pfd_filter_en_END (0)
#define SOC_MEDIA1_CRG_ADPLLS_CFG3_dither_en_START (1)
#define SOC_MEDIA1_CRG_ADPLLS_CFG3_dither_en_END (1)
#define SOC_MEDIA1_CRG_ADPLLS_CFG3_dsm_mash_sel_START (2)
#define SOC_MEDIA1_CRG_ADPLLS_CFG3_dsm_mash_sel_END (2)
#define SOC_MEDIA1_CRG_ADPLLS_CFG3_dco_dsm_en_START (3)
#define SOC_MEDIA1_CRG_ADPLLS_CFG3_dco_dsm_en_END (3)
#define SOC_MEDIA1_CRG_ADPLLS_CFG3_large_phe_thr_START (4)
#define SOC_MEDIA1_CRG_ADPLLS_CFG3_large_phe_thr_END (20)
#define SOC_MEDIA1_CRG_ADPLLS_CFG3_calib_div_sel_START (21)
#define SOC_MEDIA1_CRG_ADPLLS_CFG3_calib_div_sel_END (23)
#define SOC_MEDIA1_CRG_ADPLLS_CFG3_sel_clk_ref_lost_START (26)
#define SOC_MEDIA1_CRG_ADPLLS_CFG3_sel_clk_ref_lost_END (27)
#define SOC_MEDIA1_CRG_ADPLLS_CFG3_sel_clk_dsm_START (28)
#define SOC_MEDIA1_CRG_ADPLLS_CFG3_sel_clk_dsm_END (29)
#define SOC_MEDIA1_CRG_ADPLLS_CFG3_dco_inv_flag_START (30)
#define SOC_MEDIA1_CRG_ADPLLS_CFG3_dco_inv_flag_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int read_en : 1;
        unsigned int unlock_clear : 1;
        unsigned int ref_lost_clear : 1;
        unsigned int gt_clk_test : 1;
        unsigned int lock_cnt_thr : 9;
        unsigned int gt_clk_ref_lost : 1;
        unsigned int reserved_0 : 2;
        unsigned int refclk_lost_thr : 4;
        unsigned int reserved_1 : 2;
        unsigned int lpf_a_cfg : 5;
        unsigned int lpf_b_cfg : 5;
    } reg;
} SOC_MEDIA1_CRG_ADPLLS_CFG4_UNION;
#endif
#define SOC_MEDIA1_CRG_ADPLLS_CFG4_read_en_START (0)
#define SOC_MEDIA1_CRG_ADPLLS_CFG4_read_en_END (0)
#define SOC_MEDIA1_CRG_ADPLLS_CFG4_unlock_clear_START (1)
#define SOC_MEDIA1_CRG_ADPLLS_CFG4_unlock_clear_END (1)
#define SOC_MEDIA1_CRG_ADPLLS_CFG4_ref_lost_clear_START (2)
#define SOC_MEDIA1_CRG_ADPLLS_CFG4_ref_lost_clear_END (2)
#define SOC_MEDIA1_CRG_ADPLLS_CFG4_gt_clk_test_START (3)
#define SOC_MEDIA1_CRG_ADPLLS_CFG4_gt_clk_test_END (3)
#define SOC_MEDIA1_CRG_ADPLLS_CFG4_lock_cnt_thr_START (4)
#define SOC_MEDIA1_CRG_ADPLLS_CFG4_lock_cnt_thr_END (12)
#define SOC_MEDIA1_CRG_ADPLLS_CFG4_gt_clk_ref_lost_START (13)
#define SOC_MEDIA1_CRG_ADPLLS_CFG4_gt_clk_ref_lost_END (13)
#define SOC_MEDIA1_CRG_ADPLLS_CFG4_refclk_lost_thr_START (16)
#define SOC_MEDIA1_CRG_ADPLLS_CFG4_refclk_lost_thr_END (19)
#define SOC_MEDIA1_CRG_ADPLLS_CFG4_lpf_a_cfg_START (22)
#define SOC_MEDIA1_CRG_ADPLLS_CFG4_lpf_a_cfg_END (26)
#define SOC_MEDIA1_CRG_ADPLLS_CFG4_lpf_b_cfg_START (27)
#define SOC_MEDIA1_CRG_ADPLLS_CFG4_lpf_b_cfg_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int test_calib_mode : 1;
        unsigned int test_dco_mode : 1;
        unsigned int reserved_0 : 2;
        unsigned int reserved_1 : 7;
        unsigned int test_dco_abank : 9;
        unsigned int test_dco_pbank : 7;
        unsigned int test_data_sel : 3;
        unsigned int reserved_2 : 2;
    } reg;
} SOC_MEDIA1_CRG_ADPLLS_CFG5_UNION;
#endif
#define SOC_MEDIA1_CRG_ADPLLS_CFG5_test_calib_mode_START (0)
#define SOC_MEDIA1_CRG_ADPLLS_CFG5_test_calib_mode_END (0)
#define SOC_MEDIA1_CRG_ADPLLS_CFG5_test_dco_mode_START (1)
#define SOC_MEDIA1_CRG_ADPLLS_CFG5_test_dco_mode_END (1)
#define SOC_MEDIA1_CRG_ADPLLS_CFG5_test_dco_abank_START (11)
#define SOC_MEDIA1_CRG_ADPLLS_CFG5_test_dco_abank_END (19)
#define SOC_MEDIA1_CRG_ADPLLS_CFG5_test_dco_pbank_START (20)
#define SOC_MEDIA1_CRG_ADPLLS_CFG5_test_dco_pbank_END (26)
#define SOC_MEDIA1_CRG_ADPLLS_CFG5_test_data_sel_START (27)
#define SOC_MEDIA1_CRG_ADPLLS_CFG5_test_data_sel_END (29)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int postdiv_en : 1;
        unsigned int foutvco_en : 1;
        unsigned int bypass : 1;
        unsigned int reserved_0 : 1;
        unsigned int postdiv1 : 3;
        unsigned int postdiv2 : 3;
        unsigned int ibias_d : 8;
        unsigned int ac_buf_pd : 1;
        unsigned int idac_filter_done : 1;
        unsigned int idac_test : 1;
        unsigned int reserved_1 : 11;
    } reg;
} SOC_MEDIA1_CRG_ADPLLS_CFG6_UNION;
#endif
#define SOC_MEDIA1_CRG_ADPLLS_CFG6_postdiv_en_START (0)
#define SOC_MEDIA1_CRG_ADPLLS_CFG6_postdiv_en_END (0)
#define SOC_MEDIA1_CRG_ADPLLS_CFG6_foutvco_en_START (1)
#define SOC_MEDIA1_CRG_ADPLLS_CFG6_foutvco_en_END (1)
#define SOC_MEDIA1_CRG_ADPLLS_CFG6_bypass_START (2)
#define SOC_MEDIA1_CRG_ADPLLS_CFG6_bypass_END (2)
#define SOC_MEDIA1_CRG_ADPLLS_CFG6_postdiv1_START (4)
#define SOC_MEDIA1_CRG_ADPLLS_CFG6_postdiv1_END (6)
#define SOC_MEDIA1_CRG_ADPLLS_CFG6_postdiv2_START (7)
#define SOC_MEDIA1_CRG_ADPLLS_CFG6_postdiv2_END (9)
#define SOC_MEDIA1_CRG_ADPLLS_CFG6_ibias_d_START (10)
#define SOC_MEDIA1_CRG_ADPLLS_CFG6_ibias_d_END (17)
#define SOC_MEDIA1_CRG_ADPLLS_CFG6_ac_buf_pd_START (18)
#define SOC_MEDIA1_CRG_ADPLLS_CFG6_ac_buf_pd_END (18)
#define SOC_MEDIA1_CRG_ADPLLS_CFG6_idac_filter_done_START (19)
#define SOC_MEDIA1_CRG_ADPLLS_CFG6_idac_filter_done_END (19)
#define SOC_MEDIA1_CRG_ADPLLS_CFG6_idac_test_START (20)
#define SOC_MEDIA1_CRG_ADPLLS_CFG6_idac_test_END (20)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int test_data0 : 32;
    } reg;
} SOC_MEDIA1_CRG_ADPLLS_STATE0_UNION;
#endif
#define SOC_MEDIA1_CRG_ADPLLS_STATE0_test_data0_START (0)
#define SOC_MEDIA1_CRG_ADPLLS_STATE0_test_data0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int test_data1 : 32;
    } reg;
} SOC_MEDIA1_CRG_ADPLLS_STATE1_UNION;
#endif
#define SOC_MEDIA1_CRG_ADPLLS_STATE1_test_data1_START (0)
#define SOC_MEDIA1_CRG_ADPLLS_STATE1_test_data1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int test_data2 : 32;
    } reg;
} SOC_MEDIA1_CRG_ADPLLS_STATE2_UNION;
#endif
#define SOC_MEDIA1_CRG_ADPLLS_STATE2_test_data2_START (0)
#define SOC_MEDIA1_CRG_ADPLLS_STATE2_test_data2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int qic_intr_bus_isp : 32;
    } reg;
} SOC_MEDIA1_CRG_QIC_INTR_BUS_ISP_UNION;
#endif
#define SOC_MEDIA1_CRG_QIC_INTR_BUS_ISP_qic_intr_bus_isp_START (0)
#define SOC_MEDIA1_CRG_QIC_INTR_BUS_ISP_qic_intr_bus_isp_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int qic_intr_bus_dss : 32;
    } reg;
} SOC_MEDIA1_CRG_QIC_INTR_BUS_DSS_UNION;
#endif
#define SOC_MEDIA1_CRG_QIC_INTR_BUS_DSS_qic_intr_bus_dss_START (0)
#define SOC_MEDIA1_CRG_QIC_INTR_BUS_DSS_qic_intr_bus_dss_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int qic_intr_bus0_vivo : 32;
    } reg;
} SOC_MEDIA1_CRG_QIC_INTR_BUS0_VIVO_UNION;
#endif
#define SOC_MEDIA1_CRG_QIC_INTR_BUS0_VIVO_qic_intr_bus0_vivo_START (0)
#define SOC_MEDIA1_CRG_QIC_INTR_BUS0_VIVO_qic_intr_bus0_vivo_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int qic_intr_bus1_vivo : 32;
    } reg;
} SOC_MEDIA1_CRG_QIC_INTR_BUS1_VIVO_UNION;
#endif
#define SOC_MEDIA1_CRG_QIC_INTR_BUS1_VIVO_qic_intr_bus1_vivo_START (0)
#define SOC_MEDIA1_CRG_QIC_INTR_BUS1_VIVO_qic_intr_bus1_vivo_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int qic_busy_isp : 32;
    } reg;
} SOC_MEDIA1_CRG_QIC_BUSY_ISP_UNION;
#endif
#define SOC_MEDIA1_CRG_QIC_BUSY_ISP_qic_busy_isp_START (0)
#define SOC_MEDIA1_CRG_QIC_BUSY_ISP_qic_busy_isp_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int qic_busy_dss : 32;
    } reg;
} SOC_MEDIA1_CRG_QIC_BUSY_DSS_UNION;
#endif
#define SOC_MEDIA1_CRG_QIC_BUSY_DSS_qic_busy_dss_START (0)
#define SOC_MEDIA1_CRG_QIC_BUSY_DSS_qic_busy_dss_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int qic_busy0_vivo : 32;
    } reg;
} SOC_MEDIA1_CRG_QIC_BUSY0_VIVO_UNION;
#endif
#define SOC_MEDIA1_CRG_QIC_BUSY0_VIVO_qic_busy0_vivo_START (0)
#define SOC_MEDIA1_CRG_QIC_BUSY0_VIVO_qic_busy0_vivo_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int qic_busy1_vivo : 32;
    } reg;
} SOC_MEDIA1_CRG_QIC_BUSY1_VIVO_UNION;
#endif
#define SOC_MEDIA1_CRG_QIC_BUSY1_VIVO_qic_busy1_vivo_START (0)
#define SOC_MEDIA1_CRG_QIC_BUSY1_VIVO_qic_busy1_vivo_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_rst_isp : 1;
        unsigned int ip_arst_isp : 1;
        unsigned int reserved_0 : 1;
        unsigned int ip_rst_isp_cputocfg : 1;
        unsigned int ip_rst_isp_cpu : 1;
        unsigned int ip_rst_isp_sys : 1;
        unsigned int ip_rst_isp_i2c : 1;
        unsigned int ip_rst_isp_i3c : 1;
        unsigned int ip_rst_csi_wrapper_cfg : 1;
        unsigned int reserved_1 : 23;
    } reg;
} SOC_MEDIA1_CRG_PERRSTEN_ISP_SEC_UNION;
#endif
#define SOC_MEDIA1_CRG_PERRSTEN_ISP_SEC_ip_rst_isp_START (0)
#define SOC_MEDIA1_CRG_PERRSTEN_ISP_SEC_ip_rst_isp_END (0)
#define SOC_MEDIA1_CRG_PERRSTEN_ISP_SEC_ip_arst_isp_START (1)
#define SOC_MEDIA1_CRG_PERRSTEN_ISP_SEC_ip_arst_isp_END (1)
#define SOC_MEDIA1_CRG_PERRSTEN_ISP_SEC_ip_rst_isp_cputocfg_START (3)
#define SOC_MEDIA1_CRG_PERRSTEN_ISP_SEC_ip_rst_isp_cputocfg_END (3)
#define SOC_MEDIA1_CRG_PERRSTEN_ISP_SEC_ip_rst_isp_cpu_START (4)
#define SOC_MEDIA1_CRG_PERRSTEN_ISP_SEC_ip_rst_isp_cpu_END (4)
#define SOC_MEDIA1_CRG_PERRSTEN_ISP_SEC_ip_rst_isp_sys_START (5)
#define SOC_MEDIA1_CRG_PERRSTEN_ISP_SEC_ip_rst_isp_sys_END (5)
#define SOC_MEDIA1_CRG_PERRSTEN_ISP_SEC_ip_rst_isp_i2c_START (6)
#define SOC_MEDIA1_CRG_PERRSTEN_ISP_SEC_ip_rst_isp_i2c_END (6)
#define SOC_MEDIA1_CRG_PERRSTEN_ISP_SEC_ip_rst_isp_i3c_START (7)
#define SOC_MEDIA1_CRG_PERRSTEN_ISP_SEC_ip_rst_isp_i3c_END (7)
#define SOC_MEDIA1_CRG_PERRSTEN_ISP_SEC_ip_rst_csi_wrapper_cfg_START (8)
#define SOC_MEDIA1_CRG_PERRSTEN_ISP_SEC_ip_rst_csi_wrapper_cfg_END (8)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_rst_isp : 1;
        unsigned int ip_arst_isp : 1;
        unsigned int reserved_0 : 1;
        unsigned int ip_rst_isp_cputocfg : 1;
        unsigned int ip_rst_isp_cpu : 1;
        unsigned int ip_rst_isp_sys : 1;
        unsigned int ip_rst_isp_i2c : 1;
        unsigned int ip_rst_isp_i3c : 1;
        unsigned int ip_rst_csi_wrapper_cfg : 1;
        unsigned int reserved_1 : 23;
    } reg;
} SOC_MEDIA1_CRG_PERRSTDIS_ISP_SEC_UNION;
#endif
#define SOC_MEDIA1_CRG_PERRSTDIS_ISP_SEC_ip_rst_isp_START (0)
#define SOC_MEDIA1_CRG_PERRSTDIS_ISP_SEC_ip_rst_isp_END (0)
#define SOC_MEDIA1_CRG_PERRSTDIS_ISP_SEC_ip_arst_isp_START (1)
#define SOC_MEDIA1_CRG_PERRSTDIS_ISP_SEC_ip_arst_isp_END (1)
#define SOC_MEDIA1_CRG_PERRSTDIS_ISP_SEC_ip_rst_isp_cputocfg_START (3)
#define SOC_MEDIA1_CRG_PERRSTDIS_ISP_SEC_ip_rst_isp_cputocfg_END (3)
#define SOC_MEDIA1_CRG_PERRSTDIS_ISP_SEC_ip_rst_isp_cpu_START (4)
#define SOC_MEDIA1_CRG_PERRSTDIS_ISP_SEC_ip_rst_isp_cpu_END (4)
#define SOC_MEDIA1_CRG_PERRSTDIS_ISP_SEC_ip_rst_isp_sys_START (5)
#define SOC_MEDIA1_CRG_PERRSTDIS_ISP_SEC_ip_rst_isp_sys_END (5)
#define SOC_MEDIA1_CRG_PERRSTDIS_ISP_SEC_ip_rst_isp_i2c_START (6)
#define SOC_MEDIA1_CRG_PERRSTDIS_ISP_SEC_ip_rst_isp_i2c_END (6)
#define SOC_MEDIA1_CRG_PERRSTDIS_ISP_SEC_ip_rst_isp_i3c_START (7)
#define SOC_MEDIA1_CRG_PERRSTDIS_ISP_SEC_ip_rst_isp_i3c_END (7)
#define SOC_MEDIA1_CRG_PERRSTDIS_ISP_SEC_ip_rst_csi_wrapper_cfg_START (8)
#define SOC_MEDIA1_CRG_PERRSTDIS_ISP_SEC_ip_rst_csi_wrapper_cfg_END (8)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_rst_isp : 1;
        unsigned int ip_arst_isp : 1;
        unsigned int reserved_0 : 1;
        unsigned int ip_rst_isp_cputocfg : 1;
        unsigned int ip_rst_isp_cpu : 1;
        unsigned int ip_rst_isp_sys : 1;
        unsigned int ip_rst_isp_i2c : 1;
        unsigned int ip_rst_isp_i3c : 1;
        unsigned int ip_rst_csi_wrapper_cfg : 1;
        unsigned int reserved_1 : 23;
    } reg;
} SOC_MEDIA1_CRG_PERRSTSTAT_ISP_SEC_UNION;
#endif
#define SOC_MEDIA1_CRG_PERRSTSTAT_ISP_SEC_ip_rst_isp_START (0)
#define SOC_MEDIA1_CRG_PERRSTSTAT_ISP_SEC_ip_rst_isp_END (0)
#define SOC_MEDIA1_CRG_PERRSTSTAT_ISP_SEC_ip_arst_isp_START (1)
#define SOC_MEDIA1_CRG_PERRSTSTAT_ISP_SEC_ip_arst_isp_END (1)
#define SOC_MEDIA1_CRG_PERRSTSTAT_ISP_SEC_ip_rst_isp_cputocfg_START (3)
#define SOC_MEDIA1_CRG_PERRSTSTAT_ISP_SEC_ip_rst_isp_cputocfg_END (3)
#define SOC_MEDIA1_CRG_PERRSTSTAT_ISP_SEC_ip_rst_isp_cpu_START (4)
#define SOC_MEDIA1_CRG_PERRSTSTAT_ISP_SEC_ip_rst_isp_cpu_END (4)
#define SOC_MEDIA1_CRG_PERRSTSTAT_ISP_SEC_ip_rst_isp_sys_START (5)
#define SOC_MEDIA1_CRG_PERRSTSTAT_ISP_SEC_ip_rst_isp_sys_END (5)
#define SOC_MEDIA1_CRG_PERRSTSTAT_ISP_SEC_ip_rst_isp_i2c_START (6)
#define SOC_MEDIA1_CRG_PERRSTSTAT_ISP_SEC_ip_rst_isp_i2c_END (6)
#define SOC_MEDIA1_CRG_PERRSTSTAT_ISP_SEC_ip_rst_isp_i3c_START (7)
#define SOC_MEDIA1_CRG_PERRSTSTAT_ISP_SEC_ip_rst_isp_i3c_END (7)
#define SOC_MEDIA1_CRG_PERRSTSTAT_ISP_SEC_ip_rst_csi_wrapper_cfg_START (8)
#define SOC_MEDIA1_CRG_PERRSTSTAT_ISP_SEC_ip_rst_csi_wrapper_cfg_END (8)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int c0_arc_event_a : 1;
        unsigned int c1_arc_event_a : 1;
        unsigned int c0_arc_halt_req : 1;
        unsigned int c0_arc_run_req : 1;
        unsigned int c1_arc_halt_req : 1;
        unsigned int c1_arc_run_req : 1;
        unsigned int isp_arc_mem_ctrl_s : 26;
    } reg;
} SOC_MEDIA1_CRG_ISPCPU_CTRL0_SEC_UNION;
#endif
#define SOC_MEDIA1_CRG_ISPCPU_CTRL0_SEC_c0_arc_event_a_START (0)
#define SOC_MEDIA1_CRG_ISPCPU_CTRL0_SEC_c0_arc_event_a_END (0)
#define SOC_MEDIA1_CRG_ISPCPU_CTRL0_SEC_c1_arc_event_a_START (1)
#define SOC_MEDIA1_CRG_ISPCPU_CTRL0_SEC_c1_arc_event_a_END (1)
#define SOC_MEDIA1_CRG_ISPCPU_CTRL0_SEC_c0_arc_halt_req_START (2)
#define SOC_MEDIA1_CRG_ISPCPU_CTRL0_SEC_c0_arc_halt_req_END (2)
#define SOC_MEDIA1_CRG_ISPCPU_CTRL0_SEC_c0_arc_run_req_START (3)
#define SOC_MEDIA1_CRG_ISPCPU_CTRL0_SEC_c0_arc_run_req_END (3)
#define SOC_MEDIA1_CRG_ISPCPU_CTRL0_SEC_c1_arc_halt_req_START (4)
#define SOC_MEDIA1_CRG_ISPCPU_CTRL0_SEC_c1_arc_halt_req_END (4)
#define SOC_MEDIA1_CRG_ISPCPU_CTRL0_SEC_c1_arc_run_req_START (5)
#define SOC_MEDIA1_CRG_ISPCPU_CTRL0_SEC_c1_arc_run_req_END (5)
#define SOC_MEDIA1_CRG_ISPCPU_CTRL0_SEC_isp_arc_mem_ctrl_s_START (6)
#define SOC_MEDIA1_CRG_ISPCPU_CTRL0_SEC_isp_arc_mem_ctrl_s_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0: 3;
        unsigned int reserved_1: 3;
        unsigned int reserved_2: 3;
        unsigned int reserved_3: 23;
    } reg;
} SOC_MEDIA1_CRG_ISP_ADB_LP_OUT_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0: 3;
        unsigned int reserved_1: 29;
    } reg;
} SOC_MEDIA1_CRG_ISP_ADB_LP_IN_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_arst_asc : 1;
        unsigned int ip_rst_idi2axi_csid : 1;
        unsigned int ip_rst_fp_idi : 1;
        unsigned int reserved : 29;
    } reg;
} SOC_MEDIA1_CRG_PERRSTEN_GENERAL_SEC_UNION;
#endif
#define SOC_MEDIA1_CRG_PERRSTEN_GENERAL_SEC_ip_arst_asc_START (0)
#define SOC_MEDIA1_CRG_PERRSTEN_GENERAL_SEC_ip_arst_asc_END (0)
#define SOC_MEDIA1_CRG_PERRSTEN_GENERAL_SEC_ip_rst_idi2axi_csid_START (1)
#define SOC_MEDIA1_CRG_PERRSTEN_GENERAL_SEC_ip_rst_idi2axi_csid_END (1)
#define SOC_MEDIA1_CRG_PERRSTEN_GENERAL_SEC_ip_rst_fp_idi_START (2)
#define SOC_MEDIA1_CRG_PERRSTEN_GENERAL_SEC_ip_rst_fp_idi_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_arst_asc : 1;
        unsigned int ip_rst_idi2axi_csid : 1;
        unsigned int ip_rst_fp_idi : 1;
        unsigned int reserved : 29;
    } reg;
} SOC_MEDIA1_CRG_PERRSTDIS_GENERAL_SEC_UNION;
#endif
#define SOC_MEDIA1_CRG_PERRSTDIS_GENERAL_SEC_ip_arst_asc_START (0)
#define SOC_MEDIA1_CRG_PERRSTDIS_GENERAL_SEC_ip_arst_asc_END (0)
#define SOC_MEDIA1_CRG_PERRSTDIS_GENERAL_SEC_ip_rst_idi2axi_csid_START (1)
#define SOC_MEDIA1_CRG_PERRSTDIS_GENERAL_SEC_ip_rst_idi2axi_csid_END (1)
#define SOC_MEDIA1_CRG_PERRSTDIS_GENERAL_SEC_ip_rst_fp_idi_START (2)
#define SOC_MEDIA1_CRG_PERRSTDIS_GENERAL_SEC_ip_rst_fp_idi_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_arst_asc : 1;
        unsigned int ip_rst_idi2axi_csid : 1;
        unsigned int ip_rst_fp_idi : 1;
        unsigned int reserved : 29;
    } reg;
} SOC_MEDIA1_CRG_PERRSTSTAT_GENERAL_SEC_UNION;
#endif
#define SOC_MEDIA1_CRG_PERRSTSTAT_GENERAL_SEC_ip_arst_asc_START (0)
#define SOC_MEDIA1_CRG_PERRSTSTAT_GENERAL_SEC_ip_arst_asc_END (0)
#define SOC_MEDIA1_CRG_PERRSTSTAT_GENERAL_SEC_ip_rst_idi2axi_csid_START (1)
#define SOC_MEDIA1_CRG_PERRSTSTAT_GENERAL_SEC_ip_rst_idi2axi_csid_END (1)
#define SOC_MEDIA1_CRG_PERRSTSTAT_GENERAL_SEC_ip_rst_fp_idi_START (2)
#define SOC_MEDIA1_CRG_PERRSTSTAT_GENERAL_SEC_ip_rst_fp_idi_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_idi2axi_csid : 1;
        unsigned int gt_clk_fp_idi : 1;
        unsigned int reserved : 30;
    } reg;
} SOC_MEDIA1_CRG_PEREN0_GENERAL_SEC_UNION;
#endif
#define SOC_MEDIA1_CRG_PEREN0_GENERAL_SEC_gt_clk_idi2axi_csid_START (0)
#define SOC_MEDIA1_CRG_PEREN0_GENERAL_SEC_gt_clk_idi2axi_csid_END (0)
#define SOC_MEDIA1_CRG_PEREN0_GENERAL_SEC_gt_clk_fp_idi_START (1)
#define SOC_MEDIA1_CRG_PEREN0_GENERAL_SEC_gt_clk_fp_idi_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_idi2axi_csid : 1;
        unsigned int gt_clk_fp_idi : 1;
        unsigned int reserved : 30;
    } reg;
} SOC_MEDIA1_CRG_PERDIS0_GENERAL_SEC_UNION;
#endif
#define SOC_MEDIA1_CRG_PERDIS0_GENERAL_SEC_gt_clk_idi2axi_csid_START (0)
#define SOC_MEDIA1_CRG_PERDIS0_GENERAL_SEC_gt_clk_idi2axi_csid_END (0)
#define SOC_MEDIA1_CRG_PERDIS0_GENERAL_SEC_gt_clk_fp_idi_START (1)
#define SOC_MEDIA1_CRG_PERDIS0_GENERAL_SEC_gt_clk_fp_idi_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_idi2axi_csid : 1;
        unsigned int gt_clk_fp_idi : 1;
        unsigned int reserved : 30;
    } reg;
} SOC_MEDIA1_CRG_PERCLKEN0_GENERAL_SEC_UNION;
#endif
#define SOC_MEDIA1_CRG_PERCLKEN0_GENERAL_SEC_gt_clk_idi2axi_csid_START (0)
#define SOC_MEDIA1_CRG_PERCLKEN0_GENERAL_SEC_gt_clk_idi2axi_csid_END (0)
#define SOC_MEDIA1_CRG_PERCLKEN0_GENERAL_SEC_gt_clk_fp_idi_START (1)
#define SOC_MEDIA1_CRG_PERCLKEN0_GENERAL_SEC_gt_clk_fp_idi_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int st_clk_idi2axi_csid : 1;
        unsigned int st_clk_fp_idi : 1;
        unsigned int reserved : 30;
    } reg;
} SOC_MEDIA1_CRG_PERSTAT0_GENERAL_SEC_UNION;
#endif
#define SOC_MEDIA1_CRG_PERSTAT0_GENERAL_SEC_st_clk_idi2axi_csid_START (0)
#define SOC_MEDIA1_CRG_PERSTAT0_GENERAL_SEC_st_clk_idi2axi_csid_END (0)
#define SOC_MEDIA1_CRG_PERSTAT0_GENERAL_SEC_st_clk_fp_idi_START (1)
#define SOC_MEDIA1_CRG_PERSTAT0_GENERAL_SEC_st_clk_fp_idi_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int tz_secure_n_med1_lm : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_MEDIA1_CRG_DECPROT0SET_UNION;
#endif
#define SOC_MEDIA1_CRG_DECPROT0SET_tz_secure_n_med1_lm_START (0)
#define SOC_MEDIA1_CRG_DECPROT0SET_tz_secure_n_med1_lm_END (0)
#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
#endif
