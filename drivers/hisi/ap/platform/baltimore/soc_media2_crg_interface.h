#ifndef __SOC_MEDIA2_CRG_INTERFACE_H__
#define __SOC_MEDIA2_CRG_INTERFACE_H__ 
#ifdef __cplusplus
    #if __cplusplus
        extern "C" {
    #endif
#endif
#ifndef __SOC_H_FOR_ASM__
#define SOC_MEDIA2_CRG_PEREN0_ADDR(base) ((base) + (0x000UL))
#define SOC_MEDIA2_CRG_PERDIS0_ADDR(base) ((base) + (0x004UL))
#define SOC_MEDIA2_CRG_PERCLKEN0_ADDR(base) ((base) + (0x008UL))
#define SOC_MEDIA2_CRG_PERSTAT0_ADDR(base) ((base) + (0x00CUL))
#define SOC_MEDIA2_CRG_PEREN1_ADDR(base) ((base) + (0x010UL))
#define SOC_MEDIA2_CRG_PERDIS1_ADDR(base) ((base) + (0x014UL))
#define SOC_MEDIA2_CRG_PERCLKEN1_ADDR(base) ((base) + (0x018UL))
#define SOC_MEDIA2_CRG_PERSTAT1_ADDR(base) ((base) + (0x01CUL))
#define SOC_MEDIA2_CRG_PERRSTEN0_ADDR(base) ((base) + (0x030UL))
#define SOC_MEDIA2_CRG_PERRSTDIS0_ADDR(base) ((base) + (0x034UL))
#define SOC_MEDIA2_CRG_PERRSTSTAT0_ADDR(base) ((base) + (0x038UL))
#define SOC_MEDIA2_CRG_PERRSTEN1_ADDR(base) ((base) + (0x040UL))
#define SOC_MEDIA2_CRG_PERRSTDIS1_ADDR(base) ((base) + (0x044UL))
#define SOC_MEDIA2_CRG_PERRSTSTAT1_ADDR(base) ((base) + (0x048UL))
#define SOC_MEDIA2_CRG_CLKDIV0_ADDR(base) ((base) + (0x080UL))
#define SOC_MEDIA2_CRG_CLKDIV1_ADDR(base) ((base) + (0x084UL))
#define SOC_MEDIA2_CRG_CLKDIV2_ADDR(base) ((base) + (0x088UL))
#define SOC_MEDIA2_CRG_CLKDIV3_ADDR(base) ((base) + (0x08CUL))
#define SOC_MEDIA2_CRG_CLKDIV4_ADDR(base) ((base) + (0x090UL))
#define SOC_MEDIA2_CRG_CLKDIV5_ADDR(base) ((base) + (0x094UL))
#define SOC_MEDIA2_CRG_CLKDIV6_ADDR(base) ((base) + (0x098UL))
#define SOC_MEDIA2_CRG_CLKDIV7_ADDR(base) ((base) + (0x09CUL))
#define SOC_MEDIA2_CRG_CLKDIV8_ADDR(base) ((base) + (0x0A0UL))
#define SOC_MEDIA2_CRG_CLKDIV9_ADDR(base) ((base) + (0x0A4UL))
#define SOC_MEDIA2_CRG_CLKDIV10_ADDR(base) ((base) + (0x0A8UL))
#define SOC_MEDIA2_CRG_CLKDIV11_ADDR(base) ((base) + (0x0ACUL))
#define SOC_MEDIA2_CRG_CLKDIV12_ADDR(base) ((base) + (0x0B0UL))
#define SOC_MEDIA2_CRG_CLKDIV13_ADDR(base) ((base) + (0x0B4UL))
#define SOC_MEDIA2_CRG_CLKDIV14_ADDR(base) ((base) + (0x0B8UL))
#define SOC_MEDIA2_CRG_CLKDIV15_ADDR(base) ((base) + (0x0BCUL))
#define SOC_MEDIA2_CRG_PERI_STAT0_ADDR(base) ((base) + (0x120UL))
#define SOC_MEDIA2_CRG_PERI_STAT1_ADDR(base) ((base) + (0x124UL))
#define SOC_MEDIA2_CRG_PERI_STAT2_ADDR(base) ((base) + (0x128UL))
#define SOC_MEDIA2_CRG_PERI_AUTODIV0_ADDR(base) ((base) + (0x160UL))
#define SOC_MEDIA2_CRG_PERI_AUTODIV1_ADDR(base) ((base) + (0x164UL))
#define SOC_MEDIA2_CRG_PERI_AUTODIV2_ADDR(base) ((base) + (0x168UL))
#define SOC_MEDIA2_CRG_PERI_AUTODIV3_ADDR(base) ((base) + (0x16cUL))
#define SOC_MEDIA2_CRG_PERI_AUTODIV4_ADDR(base) ((base) + (0x170UL))
#define SOC_MEDIA2_CRG_PERI_AUTODIV5_ADDR(base) ((base) + (0x174UL))
#define SOC_MEDIA2_CRG_PERI_AUTODIV6_ADDR(base) ((base) + (0x178UL))
#define SOC_MEDIA2_CRG_PERI_AUTODIV_STAT_ADDR(base) ((base) + (0x190UL))
#define SOC_MEDIA2_CRG_INTR_BUS_NONIDLE_PEND_ADDR(base) ((base) + (0x1D0UL))
#define SOC_MEDIA2_CRG_INTR_STAT_BUS_NONIDLE_PEND_ADDR(base) ((base) + (0x1D4UL))
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_ADDR(base) ((base) + (0x1D8UL))
#define SOC_MEDIA2_CRG_PCR_CG_BYPASS_CTRL0_ADDR(base) ((base) + (0x200UL))
#define SOC_MEDIA2_CRG_PCR_CG_PSTATE1_VENC_ADDR(base) ((base) + (0x204UL))
#define SOC_MEDIA2_CRG_PCR_CG_PSTATE2_VENC_ADDR(base) ((base) + (0x208UL))
#define SOC_MEDIA2_CRG_PCR_CG_PSTATE3_VENC_ADDR(base) ((base) + (0x20CUL))
#define SOC_MEDIA2_CRG_PCR_CG_PSTATE4_VENC_ADDR(base) ((base) + (0x210UL))
#define SOC_MEDIA2_CRG_PCR_CG_PSTATE1_VDEC_ADDR(base) ((base) + (0x214UL))
#define SOC_MEDIA2_CRG_PCR_CG_PSTATE2_VDEC_ADDR(base) ((base) + (0x218UL))
#define SOC_MEDIA2_CRG_PCR_CG_PSTATE3_VDEC_ADDR(base) ((base) + (0x21CUL))
#define SOC_MEDIA2_CRG_PCR_CG_PSTATE4_VDEC_ADDR(base) ((base) + (0x220UL))
#define SOC_MEDIA2_CRG_PCR_CG_PSTATE1_JPG_FUNC_ADDR(base) ((base) + (0x224UL))
#define SOC_MEDIA2_CRG_PCR_CG_PSTATE2_JPG_FUNC_ADDR(base) ((base) + (0x228UL))
#define SOC_MEDIA2_CRG_PCR_CG_PSTATE3_JPG_FUNC_ADDR(base) ((base) + (0x22CUL))
#define SOC_MEDIA2_CRG_PCR_CG_PSTATE4_JPG_FUNC_ADDR(base) ((base) + (0x230UL))
#define SOC_MEDIA2_CRG_QIC_INTR_BUS_RT0_ADDR(base) ((base) + (0x300UL))
#define SOC_MEDIA2_CRG_QIC_INTR_BUS_RT1_ADDR(base) ((base) + (0x304UL))
#define SOC_MEDIA2_CRG_QIC_INTR_BUS_RT2_ADDR(base) ((base) + (0x308UL))
#define SOC_MEDIA2_CRG_QIC_INTR_BUS_VCODEC_ADDR(base) ((base) + (0x30CUL))
#define SOC_MEDIA2_CRG_QIC_INTR_BUS_VCODEC_WA0_ADDR(base) ((base) + (0x310UL))
#define SOC_MEDIA2_CRG_QIC_INTR_BUS_VCODEC_WA1_ADDR(base) ((base) + (0x314UL))
#define SOC_MEDIA2_CRG_QIC_INTR_BUS_BUSY0_ADDR(base) ((base) + (0x318UL))
#define SOC_MEDIA2_CRG_QIC_INTR_BUS_BUSY1_ADDR(base) ((base) + (0x31CUL))
#define SOC_MEDIA2_CRG_QIC_INTR_BUS_BUSY2_ADDR(base) ((base) + (0x320UL))
#define SOC_MEDIA2_CRG_PERRSTEN_SEC_IVP_IVP1_ADDR(base) ((base) + (0xB00UL))
#define SOC_MEDIA2_CRG_PERRSTDIS_SEC_IVP_IVP1_ADDR(base) ((base) + (0xB04UL))
#define SOC_MEDIA2_CRG_PERRSTSTAT_SEC_IVP_IVP1_ADDR(base) ((base) + (0xB08UL))
#define SOC_MEDIA2_CRG_PERRSTEN_SEC_ARPP_ADDR(base) ((base) + (0xC00UL))
#define SOC_MEDIA2_CRG_PERRSTDIS_SEC_ARPP_ADDR(base) ((base) + (0xC04UL))
#define SOC_MEDIA2_CRG_PERRSTSTAT_SEC_ARPP_ADDR(base) ((base) + (0xC08UL))
#define SOC_MEDIA2_CRG_PEREN0_SEC_HIEPS_ADDR(base) ((base) + (0xD00UL))
#define SOC_MEDIA2_CRG_PERDIS0_SEC_HIEPS_ADDR(base) ((base) + (0xD04UL))
#define SOC_MEDIA2_CRG_PERCLKEN0_SEC_HIEPS_ADDR(base) ((base) + (0xD08UL))
#define SOC_MEDIA2_CRG_PERSTAT0_SEC_HIEPS_ADDR(base) ((base) + (0xD0CUL))
#define SOC_MEDIA2_CRG_PERRSTEN0_SEC_HIEPS_ADDR(base) ((base) + (0xD30UL))
#define SOC_MEDIA2_CRG_PERRSTDIS0_SEC_HIEPS_ADDR(base) ((base) + (0xD34UL))
#define SOC_MEDIA2_CRG_PERRSTSTAT0_SEC_HIEPS_ADDR(base) ((base) + (0xD38UL))
#define SOC_MEDIA2_CRG_CLKDIV0_SEC_HIEPS_ADDR(base) ((base) + (0xD40UL))
#define SOC_MEDIA2_CRG_CLKDIV2_SEC_HIEPS_ADDR(base) ((base) + (0xD44UL))
#define SOC_MEDIA2_CRG_CLKDIV3_SEC_HIEPS_ADDR(base) ((base) + (0xD48UL))
#define SOC_MEDIA2_CRG_CLKDIV4_SEC_HIEPS_ADDR(base) ((base) + (0xD4CUL))
#define SOC_MEDIA2_CRG_PERI_STAT0_SEC_HIEPS_ADDR(base) ((base) + (0xD60UL))
#define SOC_MEDIA2_CRG_PERI_STAT1_SEC_HIEPS_ADDR(base) ((base) + (0xD64UL))
#define SOC_MEDIA2_CRG_PERRSTEN_SEC_IVP_ADDR(base) ((base) + (0xE00UL))
#define SOC_MEDIA2_CRG_PERRSTDIS_SEC_IVP_ADDR(base) ((base) + (0xE04UL))
#define SOC_MEDIA2_CRG_PERRSTSTAT_SEC_IVP_ADDR(base) ((base) + (0xE08UL))
#define SOC_MEDIA2_CRG_DECPROT0SET_ADDR(base) ((base) + (0xF00UL))
#else
#define SOC_MEDIA2_CRG_PEREN0_ADDR(base) ((base) + (0x000))
#define SOC_MEDIA2_CRG_PERDIS0_ADDR(base) ((base) + (0x004))
#define SOC_MEDIA2_CRG_PERCLKEN0_ADDR(base) ((base) + (0x008))
#define SOC_MEDIA2_CRG_PERSTAT0_ADDR(base) ((base) + (0x00C))
#define SOC_MEDIA2_CRG_PEREN1_ADDR(base) ((base) + (0x010))
#define SOC_MEDIA2_CRG_PERDIS1_ADDR(base) ((base) + (0x014))
#define SOC_MEDIA2_CRG_PERCLKEN1_ADDR(base) ((base) + (0x018))
#define SOC_MEDIA2_CRG_PERSTAT1_ADDR(base) ((base) + (0x01C))
#define SOC_MEDIA2_CRG_PERRSTEN0_ADDR(base) ((base) + (0x030))
#define SOC_MEDIA2_CRG_PERRSTDIS0_ADDR(base) ((base) + (0x034))
#define SOC_MEDIA2_CRG_PERRSTSTAT0_ADDR(base) ((base) + (0x038))
#define SOC_MEDIA2_CRG_PERRSTEN1_ADDR(base) ((base) + (0x040))
#define SOC_MEDIA2_CRG_PERRSTDIS1_ADDR(base) ((base) + (0x044))
#define SOC_MEDIA2_CRG_PERRSTSTAT1_ADDR(base) ((base) + (0x048))
#define SOC_MEDIA2_CRG_CLKDIV0_ADDR(base) ((base) + (0x080))
#define SOC_MEDIA2_CRG_CLKDIV1_ADDR(base) ((base) + (0x084))
#define SOC_MEDIA2_CRG_CLKDIV2_ADDR(base) ((base) + (0x088))
#define SOC_MEDIA2_CRG_CLKDIV3_ADDR(base) ((base) + (0x08C))
#define SOC_MEDIA2_CRG_CLKDIV4_ADDR(base) ((base) + (0x090))
#define SOC_MEDIA2_CRG_CLKDIV5_ADDR(base) ((base) + (0x094))
#define SOC_MEDIA2_CRG_CLKDIV6_ADDR(base) ((base) + (0x098))
#define SOC_MEDIA2_CRG_CLKDIV7_ADDR(base) ((base) + (0x09C))
#define SOC_MEDIA2_CRG_CLKDIV8_ADDR(base) ((base) + (0x0A0))
#define SOC_MEDIA2_CRG_CLKDIV9_ADDR(base) ((base) + (0x0A4))
#define SOC_MEDIA2_CRG_CLKDIV10_ADDR(base) ((base) + (0x0A8))
#define SOC_MEDIA2_CRG_CLKDIV11_ADDR(base) ((base) + (0x0AC))
#define SOC_MEDIA2_CRG_CLKDIV12_ADDR(base) ((base) + (0x0B0))
#define SOC_MEDIA2_CRG_CLKDIV13_ADDR(base) ((base) + (0x0B4))
#define SOC_MEDIA2_CRG_CLKDIV14_ADDR(base) ((base) + (0x0B8))
#define SOC_MEDIA2_CRG_CLKDIV15_ADDR(base) ((base) + (0x0BC))
#define SOC_MEDIA2_CRG_PERI_STAT0_ADDR(base) ((base) + (0x120))
#define SOC_MEDIA2_CRG_PERI_STAT1_ADDR(base) ((base) + (0x124))
#define SOC_MEDIA2_CRG_PERI_STAT2_ADDR(base) ((base) + (0x128))
#define SOC_MEDIA2_CRG_PERI_AUTODIV0_ADDR(base) ((base) + (0x160))
#define SOC_MEDIA2_CRG_PERI_AUTODIV1_ADDR(base) ((base) + (0x164))
#define SOC_MEDIA2_CRG_PERI_AUTODIV2_ADDR(base) ((base) + (0x168))
#define SOC_MEDIA2_CRG_PERI_AUTODIV3_ADDR(base) ((base) + (0x16c))
#define SOC_MEDIA2_CRG_PERI_AUTODIV4_ADDR(base) ((base) + (0x170))
#define SOC_MEDIA2_CRG_PERI_AUTODIV5_ADDR(base) ((base) + (0x174))
#define SOC_MEDIA2_CRG_PERI_AUTODIV6_ADDR(base) ((base) + (0x178))
#define SOC_MEDIA2_CRG_PERI_AUTODIV_STAT_ADDR(base) ((base) + (0x190))
#define SOC_MEDIA2_CRG_INTR_BUS_NONIDLE_PEND_ADDR(base) ((base) + (0x1D0))
#define SOC_MEDIA2_CRG_INTR_STAT_BUS_NONIDLE_PEND_ADDR(base) ((base) + (0x1D4))
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_ADDR(base) ((base) + (0x1D8))
#define SOC_MEDIA2_CRG_PCR_CG_BYPASS_CTRL0_ADDR(base) ((base) + (0x200))
#define SOC_MEDIA2_CRG_PCR_CG_PSTATE1_VENC_ADDR(base) ((base) + (0x204))
#define SOC_MEDIA2_CRG_PCR_CG_PSTATE2_VENC_ADDR(base) ((base) + (0x208))
#define SOC_MEDIA2_CRG_PCR_CG_PSTATE3_VENC_ADDR(base) ((base) + (0x20C))
#define SOC_MEDIA2_CRG_PCR_CG_PSTATE4_VENC_ADDR(base) ((base) + (0x210))
#define SOC_MEDIA2_CRG_PCR_CG_PSTATE1_VDEC_ADDR(base) ((base) + (0x214))
#define SOC_MEDIA2_CRG_PCR_CG_PSTATE2_VDEC_ADDR(base) ((base) + (0x218))
#define SOC_MEDIA2_CRG_PCR_CG_PSTATE3_VDEC_ADDR(base) ((base) + (0x21C))
#define SOC_MEDIA2_CRG_PCR_CG_PSTATE4_VDEC_ADDR(base) ((base) + (0x220))
#define SOC_MEDIA2_CRG_PCR_CG_PSTATE1_JPG_FUNC_ADDR(base) ((base) + (0x224))
#define SOC_MEDIA2_CRG_PCR_CG_PSTATE2_JPG_FUNC_ADDR(base) ((base) + (0x228))
#define SOC_MEDIA2_CRG_PCR_CG_PSTATE3_JPG_FUNC_ADDR(base) ((base) + (0x22C))
#define SOC_MEDIA2_CRG_PCR_CG_PSTATE4_JPG_FUNC_ADDR(base) ((base) + (0x230))
#define SOC_MEDIA2_CRG_QIC_INTR_BUS_RT0_ADDR(base) ((base) + (0x300))
#define SOC_MEDIA2_CRG_QIC_INTR_BUS_RT1_ADDR(base) ((base) + (0x304))
#define SOC_MEDIA2_CRG_QIC_INTR_BUS_RT2_ADDR(base) ((base) + (0x308))
#define SOC_MEDIA2_CRG_QIC_INTR_BUS_VCODEC_ADDR(base) ((base) + (0x30C))
#define SOC_MEDIA2_CRG_QIC_INTR_BUS_VCODEC_WA0_ADDR(base) ((base) + (0x310))
#define SOC_MEDIA2_CRG_QIC_INTR_BUS_VCODEC_WA1_ADDR(base) ((base) + (0x314))
#define SOC_MEDIA2_CRG_QIC_INTR_BUS_BUSY0_ADDR(base) ((base) + (0x318))
#define SOC_MEDIA2_CRG_QIC_INTR_BUS_BUSY1_ADDR(base) ((base) + (0x31C))
#define SOC_MEDIA2_CRG_QIC_INTR_BUS_BUSY2_ADDR(base) ((base) + (0x320))
#define SOC_MEDIA2_CRG_PERRSTEN_SEC_IVP_IVP1_ADDR(base) ((base) + (0xB00))
#define SOC_MEDIA2_CRG_PERRSTDIS_SEC_IVP_IVP1_ADDR(base) ((base) + (0xB04))
#define SOC_MEDIA2_CRG_PERRSTSTAT_SEC_IVP_IVP1_ADDR(base) ((base) + (0xB08))
#define SOC_MEDIA2_CRG_PERRSTEN_SEC_ARPP_ADDR(base) ((base) + (0xC00))
#define SOC_MEDIA2_CRG_PERRSTDIS_SEC_ARPP_ADDR(base) ((base) + (0xC04))
#define SOC_MEDIA2_CRG_PERRSTSTAT_SEC_ARPP_ADDR(base) ((base) + (0xC08))
#define SOC_MEDIA2_CRG_PEREN0_SEC_HIEPS_ADDR(base) ((base) + (0xD00))
#define SOC_MEDIA2_CRG_PERDIS0_SEC_HIEPS_ADDR(base) ((base) + (0xD04))
#define SOC_MEDIA2_CRG_PERCLKEN0_SEC_HIEPS_ADDR(base) ((base) + (0xD08))
#define SOC_MEDIA2_CRG_PERSTAT0_SEC_HIEPS_ADDR(base) ((base) + (0xD0C))
#define SOC_MEDIA2_CRG_PERRSTEN0_SEC_HIEPS_ADDR(base) ((base) + (0xD30))
#define SOC_MEDIA2_CRG_PERRSTDIS0_SEC_HIEPS_ADDR(base) ((base) + (0xD34))
#define SOC_MEDIA2_CRG_PERRSTSTAT0_SEC_HIEPS_ADDR(base) ((base) + (0xD38))
#define SOC_MEDIA2_CRG_CLKDIV0_SEC_HIEPS_ADDR(base) ((base) + (0xD40))
#define SOC_MEDIA2_CRG_CLKDIV2_SEC_HIEPS_ADDR(base) ((base) + (0xD44))
#define SOC_MEDIA2_CRG_CLKDIV3_SEC_HIEPS_ADDR(base) ((base) + (0xD48))
#define SOC_MEDIA2_CRG_CLKDIV4_SEC_HIEPS_ADDR(base) ((base) + (0xD4C))
#define SOC_MEDIA2_CRG_PERI_STAT0_SEC_HIEPS_ADDR(base) ((base) + (0xD60))
#define SOC_MEDIA2_CRG_PERI_STAT1_SEC_HIEPS_ADDR(base) ((base) + (0xD64))
#define SOC_MEDIA2_CRG_PERRSTEN_SEC_IVP_ADDR(base) ((base) + (0xE00))
#define SOC_MEDIA2_CRG_PERRSTDIS_SEC_IVP_ADDR(base) ((base) + (0xE04))
#define SOC_MEDIA2_CRG_PERRSTSTAT_SEC_IVP_ADDR(base) ((base) + (0xE08))
#define SOC_MEDIA2_CRG_DECPROT0SET_ADDR(base) ((base) + (0xF00))
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_vdec_32k : 1;
        unsigned int gt_venc_32k : 1;
        unsigned int gt_clk_vcodecbus2ddrc : 1;
        unsigned int gt_pclk_venc : 1;
        unsigned int gt_aclk_venc : 1;
        unsigned int gt_clk_venc : 1;
        unsigned int gt_pclk_vdec : 1;
        unsigned int gt_aclk_vdec : 1;
        unsigned int gt_clk_vdec : 1;
        unsigned int gt_clk_vcodecbus : 1;
        unsigned int gt_pclk_atgs_vcodec : 1;
        unsigned int gt_pclk_autodiv_vcodecbus : 1;
        unsigned int gt_pclk_autodiv_vdec : 1;
        unsigned int gt_pclk_autodiv_venc : 1;
        unsigned int gt_clk_vcodecbus_ini : 1;
        unsigned int gt_clk_dmss_vcodec : 1;
        unsigned int gt_clk_loadmonitor : 1;
        unsigned int gt_pclk_media2_lm : 1;
        unsigned int gt_pclk_media2_lm_lpm3 : 1;
        unsigned int gt_clk_ivp32dsp_tcxo : 1;
        unsigned int gt_pclk_autodiv_ivp : 1;
        unsigned int gt_pclk_atgm_ivp : 1;
        unsigned int gt_clk_qic_ivp32_cfg : 1;
        unsigned int gt_clk_ade_func : 1;
        unsigned int gt_clk_jpg_func : 1;
        unsigned int gt_pclk_jpg : 1;
        unsigned int gt_atclk_trace : 1;
        unsigned int gt_pclk_dbg_trace : 1;
        unsigned int gt_aclk_jpg : 1;
        unsigned int gt_clk_tcu_vcodec : 1;
        unsigned int gt_clk_ivp_qic_crg_ini : 1;
        unsigned int gt_clk_ivp_qic_peri_ini : 1;
    } reg;
} SOC_MEDIA2_CRG_PEREN0_UNION;
#endif
#define SOC_MEDIA2_CRG_PEREN0_gt_vdec_32k_START (0)
#define SOC_MEDIA2_CRG_PEREN0_gt_vdec_32k_END (0)
#define SOC_MEDIA2_CRG_PEREN0_gt_venc_32k_START (1)
#define SOC_MEDIA2_CRG_PEREN0_gt_venc_32k_END (1)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_vcodecbus2ddrc_START (2)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_vcodecbus2ddrc_END (2)
#define SOC_MEDIA2_CRG_PEREN0_gt_pclk_venc_START (3)
#define SOC_MEDIA2_CRG_PEREN0_gt_pclk_venc_END (3)
#define SOC_MEDIA2_CRG_PEREN0_gt_aclk_venc_START (4)
#define SOC_MEDIA2_CRG_PEREN0_gt_aclk_venc_END (4)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_venc_START (5)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_venc_END (5)
#define SOC_MEDIA2_CRG_PEREN0_gt_pclk_vdec_START (6)
#define SOC_MEDIA2_CRG_PEREN0_gt_pclk_vdec_END (6)
#define SOC_MEDIA2_CRG_PEREN0_gt_aclk_vdec_START (7)
#define SOC_MEDIA2_CRG_PEREN0_gt_aclk_vdec_END (7)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_vdec_START (8)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_vdec_END (8)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_vcodecbus_START (9)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_vcodecbus_END (9)
#define SOC_MEDIA2_CRG_PEREN0_gt_pclk_atgs_vcodec_START (10)
#define SOC_MEDIA2_CRG_PEREN0_gt_pclk_atgs_vcodec_END (10)
#define SOC_MEDIA2_CRG_PEREN0_gt_pclk_autodiv_vcodecbus_START (11)
#define SOC_MEDIA2_CRG_PEREN0_gt_pclk_autodiv_vcodecbus_END (11)
#define SOC_MEDIA2_CRG_PEREN0_gt_pclk_autodiv_vdec_START (12)
#define SOC_MEDIA2_CRG_PEREN0_gt_pclk_autodiv_vdec_END (12)
#define SOC_MEDIA2_CRG_PEREN0_gt_pclk_autodiv_venc_START (13)
#define SOC_MEDIA2_CRG_PEREN0_gt_pclk_autodiv_venc_END (13)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_vcodecbus_ini_START (14)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_vcodecbus_ini_END (14)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_dmss_vcodec_START (15)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_dmss_vcodec_END (15)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_loadmonitor_START (16)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_loadmonitor_END (16)
#define SOC_MEDIA2_CRG_PEREN0_gt_pclk_media2_lm_START (17)
#define SOC_MEDIA2_CRG_PEREN0_gt_pclk_media2_lm_END (17)
#define SOC_MEDIA2_CRG_PEREN0_gt_pclk_media2_lm_lpm3_START (18)
#define SOC_MEDIA2_CRG_PEREN0_gt_pclk_media2_lm_lpm3_END (18)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_ivp32dsp_tcxo_START (19)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_ivp32dsp_tcxo_END (19)
#define SOC_MEDIA2_CRG_PEREN0_gt_pclk_autodiv_ivp_START (20)
#define SOC_MEDIA2_CRG_PEREN0_gt_pclk_autodiv_ivp_END (20)
#define SOC_MEDIA2_CRG_PEREN0_gt_pclk_atgm_ivp_START (21)
#define SOC_MEDIA2_CRG_PEREN0_gt_pclk_atgm_ivp_END (21)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_qic_ivp32_cfg_START (22)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_qic_ivp32_cfg_END (22)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_ade_func_START (23)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_ade_func_END (23)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_jpg_func_START (24)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_jpg_func_END (24)
#define SOC_MEDIA2_CRG_PEREN0_gt_pclk_jpg_START (25)
#define SOC_MEDIA2_CRG_PEREN0_gt_pclk_jpg_END (25)
#define SOC_MEDIA2_CRG_PEREN0_gt_atclk_trace_START (26)
#define SOC_MEDIA2_CRG_PEREN0_gt_atclk_trace_END (26)
#define SOC_MEDIA2_CRG_PEREN0_gt_pclk_dbg_trace_START (27)
#define SOC_MEDIA2_CRG_PEREN0_gt_pclk_dbg_trace_END (27)
#define SOC_MEDIA2_CRG_PEREN0_gt_aclk_jpg_START (28)
#define SOC_MEDIA2_CRG_PEREN0_gt_aclk_jpg_END (28)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_tcu_vcodec_START (29)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_tcu_vcodec_END (29)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_ivp_qic_crg_ini_START (30)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_ivp_qic_crg_ini_END (30)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_ivp_qic_peri_ini_START (31)
#define SOC_MEDIA2_CRG_PEREN0_gt_clk_ivp_qic_peri_ini_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_vdec_32k : 1;
        unsigned int gt_venc_32k : 1;
        unsigned int gt_clk_vcodecbus2ddrc : 1;
        unsigned int gt_pclk_venc : 1;
        unsigned int gt_aclk_venc : 1;
        unsigned int gt_clk_venc : 1;
        unsigned int gt_pclk_vdec : 1;
        unsigned int gt_aclk_vdec : 1;
        unsigned int gt_clk_vdec : 1;
        unsigned int gt_clk_vcodecbus : 1;
        unsigned int gt_pclk_atgs_vcodec : 1;
        unsigned int gt_pclk_autodiv_vcodecbus : 1;
        unsigned int gt_pclk_autodiv_vdec : 1;
        unsigned int gt_pclk_autodiv_venc : 1;
        unsigned int gt_clk_vcodecbus_ini : 1;
        unsigned int gt_clk_dmss_vcodec : 1;
        unsigned int gt_clk_loadmonitor : 1;
        unsigned int gt_pclk_media2_lm : 1;
        unsigned int gt_pclk_media2_lm_lpm3 : 1;
        unsigned int gt_clk_ivp32dsp_tcxo : 1;
        unsigned int gt_pclk_autodiv_ivp : 1;
        unsigned int gt_pclk_atgm_ivp : 1;
        unsigned int gt_clk_qic_ivp32_cfg : 1;
        unsigned int gt_clk_ade_func : 1;
        unsigned int gt_clk_jpg_func : 1;
        unsigned int gt_pclk_jpg : 1;
        unsigned int gt_atclk_trace : 1;
        unsigned int gt_pclk_dbg_trace : 1;
        unsigned int gt_aclk_jpg : 1;
        unsigned int gt_clk_tcu_vcodec : 1;
        unsigned int gt_clk_ivp_qic_crg_ini : 1;
        unsigned int gt_clk_ivp_qic_peri_ini : 1;
    } reg;
} SOC_MEDIA2_CRG_PERDIS0_UNION;
#endif
#define SOC_MEDIA2_CRG_PERDIS0_gt_vdec_32k_START (0)
#define SOC_MEDIA2_CRG_PERDIS0_gt_vdec_32k_END (0)
#define SOC_MEDIA2_CRG_PERDIS0_gt_venc_32k_START (1)
#define SOC_MEDIA2_CRG_PERDIS0_gt_venc_32k_END (1)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_vcodecbus2ddrc_START (2)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_vcodecbus2ddrc_END (2)
#define SOC_MEDIA2_CRG_PERDIS0_gt_pclk_venc_START (3)
#define SOC_MEDIA2_CRG_PERDIS0_gt_pclk_venc_END (3)
#define SOC_MEDIA2_CRG_PERDIS0_gt_aclk_venc_START (4)
#define SOC_MEDIA2_CRG_PERDIS0_gt_aclk_venc_END (4)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_venc_START (5)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_venc_END (5)
#define SOC_MEDIA2_CRG_PERDIS0_gt_pclk_vdec_START (6)
#define SOC_MEDIA2_CRG_PERDIS0_gt_pclk_vdec_END (6)
#define SOC_MEDIA2_CRG_PERDIS0_gt_aclk_vdec_START (7)
#define SOC_MEDIA2_CRG_PERDIS0_gt_aclk_vdec_END (7)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_vdec_START (8)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_vdec_END (8)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_vcodecbus_START (9)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_vcodecbus_END (9)
#define SOC_MEDIA2_CRG_PERDIS0_gt_pclk_atgs_vcodec_START (10)
#define SOC_MEDIA2_CRG_PERDIS0_gt_pclk_atgs_vcodec_END (10)
#define SOC_MEDIA2_CRG_PERDIS0_gt_pclk_autodiv_vcodecbus_START (11)
#define SOC_MEDIA2_CRG_PERDIS0_gt_pclk_autodiv_vcodecbus_END (11)
#define SOC_MEDIA2_CRG_PERDIS0_gt_pclk_autodiv_vdec_START (12)
#define SOC_MEDIA2_CRG_PERDIS0_gt_pclk_autodiv_vdec_END (12)
#define SOC_MEDIA2_CRG_PERDIS0_gt_pclk_autodiv_venc_START (13)
#define SOC_MEDIA2_CRG_PERDIS0_gt_pclk_autodiv_venc_END (13)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_vcodecbus_ini_START (14)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_vcodecbus_ini_END (14)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_dmss_vcodec_START (15)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_dmss_vcodec_END (15)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_loadmonitor_START (16)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_loadmonitor_END (16)
#define SOC_MEDIA2_CRG_PERDIS0_gt_pclk_media2_lm_START (17)
#define SOC_MEDIA2_CRG_PERDIS0_gt_pclk_media2_lm_END (17)
#define SOC_MEDIA2_CRG_PERDIS0_gt_pclk_media2_lm_lpm3_START (18)
#define SOC_MEDIA2_CRG_PERDIS0_gt_pclk_media2_lm_lpm3_END (18)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_ivp32dsp_tcxo_START (19)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_ivp32dsp_tcxo_END (19)
#define SOC_MEDIA2_CRG_PERDIS0_gt_pclk_autodiv_ivp_START (20)
#define SOC_MEDIA2_CRG_PERDIS0_gt_pclk_autodiv_ivp_END (20)
#define SOC_MEDIA2_CRG_PERDIS0_gt_pclk_atgm_ivp_START (21)
#define SOC_MEDIA2_CRG_PERDIS0_gt_pclk_atgm_ivp_END (21)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_qic_ivp32_cfg_START (22)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_qic_ivp32_cfg_END (22)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_ade_func_START (23)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_ade_func_END (23)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_jpg_func_START (24)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_jpg_func_END (24)
#define SOC_MEDIA2_CRG_PERDIS0_gt_pclk_jpg_START (25)
#define SOC_MEDIA2_CRG_PERDIS0_gt_pclk_jpg_END (25)
#define SOC_MEDIA2_CRG_PERDIS0_gt_atclk_trace_START (26)
#define SOC_MEDIA2_CRG_PERDIS0_gt_atclk_trace_END (26)
#define SOC_MEDIA2_CRG_PERDIS0_gt_pclk_dbg_trace_START (27)
#define SOC_MEDIA2_CRG_PERDIS0_gt_pclk_dbg_trace_END (27)
#define SOC_MEDIA2_CRG_PERDIS0_gt_aclk_jpg_START (28)
#define SOC_MEDIA2_CRG_PERDIS0_gt_aclk_jpg_END (28)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_tcu_vcodec_START (29)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_tcu_vcodec_END (29)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_ivp_qic_crg_ini_START (30)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_ivp_qic_crg_ini_END (30)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_ivp_qic_peri_ini_START (31)
#define SOC_MEDIA2_CRG_PERDIS0_gt_clk_ivp_qic_peri_ini_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_vdec_32k : 1;
        unsigned int gt_venc_32k : 1;
        unsigned int gt_clk_vcodecbus2ddrc : 1;
        unsigned int gt_pclk_venc : 1;
        unsigned int gt_aclk_venc : 1;
        unsigned int gt_clk_venc : 1;
        unsigned int gt_pclk_vdec : 1;
        unsigned int gt_aclk_vdec : 1;
        unsigned int gt_clk_vdec : 1;
        unsigned int gt_clk_vcodecbus : 1;
        unsigned int gt_pclk_atgs_vcodec : 1;
        unsigned int gt_pclk_autodiv_vcodecbus : 1;
        unsigned int gt_pclk_autodiv_vdec : 1;
        unsigned int gt_pclk_autodiv_venc : 1;
        unsigned int gt_clk_vcodecbus_ini : 1;
        unsigned int gt_clk_dmss_vcodec : 1;
        unsigned int gt_clk_loadmonitor : 1;
        unsigned int gt_pclk_media2_lm : 1;
        unsigned int gt_pclk_media2_lm_lpm3 : 1;
        unsigned int gt_clk_ivp32dsp_tcxo : 1;
        unsigned int gt_pclk_autodiv_ivp : 1;
        unsigned int gt_pclk_atgm_ivp : 1;
        unsigned int gt_clk_qic_ivp32_cfg : 1;
        unsigned int gt_clk_ade_func : 1;
        unsigned int gt_clk_jpg_func : 1;
        unsigned int gt_pclk_jpg : 1;
        unsigned int gt_atclk_trace : 1;
        unsigned int gt_pclk_dbg_trace : 1;
        unsigned int gt_aclk_jpg : 1;
        unsigned int gt_clk_tcu_vcodec : 1;
        unsigned int gt_clk_ivp_qic_crg_ini : 1;
        unsigned int gt_clk_ivp_qic_peri_ini : 1;
    } reg;
} SOC_MEDIA2_CRG_PERCLKEN0_UNION;
#endif
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_vdec_32k_START (0)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_vdec_32k_END (0)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_venc_32k_START (1)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_venc_32k_END (1)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_vcodecbus2ddrc_START (2)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_vcodecbus2ddrc_END (2)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_pclk_venc_START (3)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_pclk_venc_END (3)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_aclk_venc_START (4)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_aclk_venc_END (4)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_venc_START (5)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_venc_END (5)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_pclk_vdec_START (6)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_pclk_vdec_END (6)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_aclk_vdec_START (7)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_aclk_vdec_END (7)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_vdec_START (8)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_vdec_END (8)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_vcodecbus_START (9)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_vcodecbus_END (9)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_pclk_atgs_vcodec_START (10)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_pclk_atgs_vcodec_END (10)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_pclk_autodiv_vcodecbus_START (11)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_pclk_autodiv_vcodecbus_END (11)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_pclk_autodiv_vdec_START (12)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_pclk_autodiv_vdec_END (12)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_pclk_autodiv_venc_START (13)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_pclk_autodiv_venc_END (13)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_vcodecbus_ini_START (14)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_vcodecbus_ini_END (14)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_dmss_vcodec_START (15)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_dmss_vcodec_END (15)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_loadmonitor_START (16)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_loadmonitor_END (16)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_pclk_media2_lm_START (17)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_pclk_media2_lm_END (17)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_pclk_media2_lm_lpm3_START (18)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_pclk_media2_lm_lpm3_END (18)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_ivp32dsp_tcxo_START (19)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_ivp32dsp_tcxo_END (19)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_pclk_autodiv_ivp_START (20)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_pclk_autodiv_ivp_END (20)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_pclk_atgm_ivp_START (21)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_pclk_atgm_ivp_END (21)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_qic_ivp32_cfg_START (22)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_qic_ivp32_cfg_END (22)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_ade_func_START (23)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_ade_func_END (23)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_jpg_func_START (24)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_jpg_func_END (24)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_pclk_jpg_START (25)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_pclk_jpg_END (25)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_atclk_trace_START (26)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_atclk_trace_END (26)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_pclk_dbg_trace_START (27)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_pclk_dbg_trace_END (27)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_aclk_jpg_START (28)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_aclk_jpg_END (28)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_tcu_vcodec_START (29)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_tcu_vcodec_END (29)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_ivp_qic_crg_ini_START (30)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_ivp_qic_crg_ini_END (30)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_ivp_qic_peri_ini_START (31)
#define SOC_MEDIA2_CRG_PERCLKEN0_gt_clk_ivp_qic_peri_ini_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int st_vdec_32k : 1;
        unsigned int st_venc_32k : 1;
        unsigned int st_clk_vcodecbus2ddrc : 1;
        unsigned int st_pclk_venc : 1;
        unsigned int st_aclk_venc : 1;
        unsigned int st_clk_venc : 1;
        unsigned int st_pclk_vdec : 1;
        unsigned int st_aclk_vdec : 1;
        unsigned int st_clk_vdec : 1;
        unsigned int st_clk_vcodecbus : 1;
        unsigned int st_pclk_atgs_vcodec : 1;
        unsigned int st_pclk_autodiv_vcodecbus : 1;
        unsigned int st_pclk_autodiv_vdec : 1;
        unsigned int st_pclk_autodiv_venc : 1;
        unsigned int st_clk_vcodecbus_ini : 1;
        unsigned int st_clk_dmss_vcodec : 1;
        unsigned int st_clk_loadmonitor : 1;
        unsigned int st_pclk_media2_lm : 1;
        unsigned int st_pclk_media2_lm_lpm3 : 1;
        unsigned int st_clk_ivp32dsp_tcxo : 1;
        unsigned int st_pclk_autodiv_ivp : 1;
        unsigned int st_pclk_atgm_ivp : 1;
        unsigned int st_clk_qic_ivp32_cfg : 1;
        unsigned int st_clk_ade_func : 1;
        unsigned int st_clk_jpg_func : 1;
        unsigned int st_pclk_jpg : 1;
        unsigned int st_atclk_trace : 1;
        unsigned int st_pclk_dbg_trace : 1;
        unsigned int st_aclk_jpg : 1;
        unsigned int st_clk_tcu_vcodec : 1;
        unsigned int st_clk_ivp_qic_crg_ini : 1;
        unsigned int st_clk_ivp_qic_peri_ini : 1;
    } reg;
} SOC_MEDIA2_CRG_PERSTAT0_UNION;
#endif
#define SOC_MEDIA2_CRG_PERSTAT0_st_vdec_32k_START (0)
#define SOC_MEDIA2_CRG_PERSTAT0_st_vdec_32k_END (0)
#define SOC_MEDIA2_CRG_PERSTAT0_st_venc_32k_START (1)
#define SOC_MEDIA2_CRG_PERSTAT0_st_venc_32k_END (1)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_vcodecbus2ddrc_START (2)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_vcodecbus2ddrc_END (2)
#define SOC_MEDIA2_CRG_PERSTAT0_st_pclk_venc_START (3)
#define SOC_MEDIA2_CRG_PERSTAT0_st_pclk_venc_END (3)
#define SOC_MEDIA2_CRG_PERSTAT0_st_aclk_venc_START (4)
#define SOC_MEDIA2_CRG_PERSTAT0_st_aclk_venc_END (4)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_venc_START (5)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_venc_END (5)
#define SOC_MEDIA2_CRG_PERSTAT0_st_pclk_vdec_START (6)
#define SOC_MEDIA2_CRG_PERSTAT0_st_pclk_vdec_END (6)
#define SOC_MEDIA2_CRG_PERSTAT0_st_aclk_vdec_START (7)
#define SOC_MEDIA2_CRG_PERSTAT0_st_aclk_vdec_END (7)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_vdec_START (8)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_vdec_END (8)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_vcodecbus_START (9)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_vcodecbus_END (9)
#define SOC_MEDIA2_CRG_PERSTAT0_st_pclk_atgs_vcodec_START (10)
#define SOC_MEDIA2_CRG_PERSTAT0_st_pclk_atgs_vcodec_END (10)
#define SOC_MEDIA2_CRG_PERSTAT0_st_pclk_autodiv_vcodecbus_START (11)
#define SOC_MEDIA2_CRG_PERSTAT0_st_pclk_autodiv_vcodecbus_END (11)
#define SOC_MEDIA2_CRG_PERSTAT0_st_pclk_autodiv_vdec_START (12)
#define SOC_MEDIA2_CRG_PERSTAT0_st_pclk_autodiv_vdec_END (12)
#define SOC_MEDIA2_CRG_PERSTAT0_st_pclk_autodiv_venc_START (13)
#define SOC_MEDIA2_CRG_PERSTAT0_st_pclk_autodiv_venc_END (13)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_vcodecbus_ini_START (14)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_vcodecbus_ini_END (14)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_dmss_vcodec_START (15)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_dmss_vcodec_END (15)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_loadmonitor_START (16)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_loadmonitor_END (16)
#define SOC_MEDIA2_CRG_PERSTAT0_st_pclk_media2_lm_START (17)
#define SOC_MEDIA2_CRG_PERSTAT0_st_pclk_media2_lm_END (17)
#define SOC_MEDIA2_CRG_PERSTAT0_st_pclk_media2_lm_lpm3_START (18)
#define SOC_MEDIA2_CRG_PERSTAT0_st_pclk_media2_lm_lpm3_END (18)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_ivp32dsp_tcxo_START (19)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_ivp32dsp_tcxo_END (19)
#define SOC_MEDIA2_CRG_PERSTAT0_st_pclk_autodiv_ivp_START (20)
#define SOC_MEDIA2_CRG_PERSTAT0_st_pclk_autodiv_ivp_END (20)
#define SOC_MEDIA2_CRG_PERSTAT0_st_pclk_atgm_ivp_START (21)
#define SOC_MEDIA2_CRG_PERSTAT0_st_pclk_atgm_ivp_END (21)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_qic_ivp32_cfg_START (22)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_qic_ivp32_cfg_END (22)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_ade_func_START (23)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_ade_func_END (23)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_jpg_func_START (24)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_jpg_func_END (24)
#define SOC_MEDIA2_CRG_PERSTAT0_st_pclk_jpg_START (25)
#define SOC_MEDIA2_CRG_PERSTAT0_st_pclk_jpg_END (25)
#define SOC_MEDIA2_CRG_PERSTAT0_st_atclk_trace_START (26)
#define SOC_MEDIA2_CRG_PERSTAT0_st_atclk_trace_END (26)
#define SOC_MEDIA2_CRG_PERSTAT0_st_pclk_dbg_trace_START (27)
#define SOC_MEDIA2_CRG_PERSTAT0_st_pclk_dbg_trace_END (27)
#define SOC_MEDIA2_CRG_PERSTAT0_st_aclk_jpg_START (28)
#define SOC_MEDIA2_CRG_PERSTAT0_st_aclk_jpg_END (28)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_tcu_vcodec_START (29)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_tcu_vcodec_END (29)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_ivp_qic_crg_ini_START (30)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_ivp_qic_crg_ini_END (30)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_ivp_qic_peri_ini_START (31)
#define SOC_MEDIA2_CRG_PERSTAT0_st_clk_ivp_qic_peri_ini_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_aclk_arpp_top : 1;
        unsigned int gt_pclk_arpp_top : 1;
        unsigned int gt_pclk_atgm_arpp : 1;
        unsigned int gt_clk_arpp_tcxo : 1;
        unsigned int gt_clk_arpp_top : 1;
        unsigned int gt_clk_ivp32dsp_tcxo_ivp1 : 1;
        unsigned int gt_pclk_autodiv_ivp_ivp1 : 1;
        unsigned int gt_pclk_atgm_ivp_ivp1 : 1;
        unsigned int gt_clk_qic_ivp32_cfg_ivp1 : 1;
        unsigned int gt_clk_ivp_qic_crg_ini_ivp1 : 1;
        unsigned int gt_clk_ivp_qic_peri_ini_ivp1 : 1;
        unsigned int gt_clk_vcodecbus_32k : 1;
        unsigned int gt_clk_arpp_ref32k : 1;
        unsigned int gt_clk_ipp_ref32k : 1;
        unsigned int gt_clk_ivp32dsp_ref32k_ivp1 : 1;
        unsigned int gt_clk_ivp32dsp_ref32k : 1;
        unsigned int gt_pclk_atgm_pac : 1;
        unsigned int gt_clk_pac_tcxo : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 13;
    } reg;
} SOC_MEDIA2_CRG_PEREN1_UNION;
#endif
#define SOC_MEDIA2_CRG_PEREN1_gt_aclk_arpp_top_START (0)
#define SOC_MEDIA2_CRG_PEREN1_gt_aclk_arpp_top_END (0)
#define SOC_MEDIA2_CRG_PEREN1_gt_pclk_arpp_top_START (1)
#define SOC_MEDIA2_CRG_PEREN1_gt_pclk_arpp_top_END (1)
#define SOC_MEDIA2_CRG_PEREN1_gt_pclk_atgm_arpp_START (2)
#define SOC_MEDIA2_CRG_PEREN1_gt_pclk_atgm_arpp_END (2)
#define SOC_MEDIA2_CRG_PEREN1_gt_clk_arpp_tcxo_START (3)
#define SOC_MEDIA2_CRG_PEREN1_gt_clk_arpp_tcxo_END (3)
#define SOC_MEDIA2_CRG_PEREN1_gt_clk_arpp_top_START (4)
#define SOC_MEDIA2_CRG_PEREN1_gt_clk_arpp_top_END (4)
#define SOC_MEDIA2_CRG_PEREN1_gt_clk_ivp32dsp_tcxo_ivp1_START (5)
#define SOC_MEDIA2_CRG_PEREN1_gt_clk_ivp32dsp_tcxo_ivp1_END (5)
#define SOC_MEDIA2_CRG_PEREN1_gt_pclk_autodiv_ivp_ivp1_START (6)
#define SOC_MEDIA2_CRG_PEREN1_gt_pclk_autodiv_ivp_ivp1_END (6)
#define SOC_MEDIA2_CRG_PEREN1_gt_pclk_atgm_ivp_ivp1_START (7)
#define SOC_MEDIA2_CRG_PEREN1_gt_pclk_atgm_ivp_ivp1_END (7)
#define SOC_MEDIA2_CRG_PEREN1_gt_clk_qic_ivp32_cfg_ivp1_START (8)
#define SOC_MEDIA2_CRG_PEREN1_gt_clk_qic_ivp32_cfg_ivp1_END (8)
#define SOC_MEDIA2_CRG_PEREN1_gt_clk_ivp_qic_crg_ini_ivp1_START (9)
#define SOC_MEDIA2_CRG_PEREN1_gt_clk_ivp_qic_crg_ini_ivp1_END (9)
#define SOC_MEDIA2_CRG_PEREN1_gt_clk_ivp_qic_peri_ini_ivp1_START (10)
#define SOC_MEDIA2_CRG_PEREN1_gt_clk_ivp_qic_peri_ini_ivp1_END (10)
#define SOC_MEDIA2_CRG_PEREN1_gt_clk_vcodecbus_32k_START (11)
#define SOC_MEDIA2_CRG_PEREN1_gt_clk_vcodecbus_32k_END (11)
#define SOC_MEDIA2_CRG_PEREN1_gt_clk_arpp_ref32k_START (12)
#define SOC_MEDIA2_CRG_PEREN1_gt_clk_arpp_ref32k_END (12)
#define SOC_MEDIA2_CRG_PEREN1_gt_clk_ipp_ref32k_START (13)
#define SOC_MEDIA2_CRG_PEREN1_gt_clk_ipp_ref32k_END (13)
#define SOC_MEDIA2_CRG_PEREN1_gt_clk_ivp32dsp_ref32k_ivp1_START (14)
#define SOC_MEDIA2_CRG_PEREN1_gt_clk_ivp32dsp_ref32k_ivp1_END (14)
#define SOC_MEDIA2_CRG_PEREN1_gt_clk_ivp32dsp_ref32k_START (15)
#define SOC_MEDIA2_CRG_PEREN1_gt_clk_ivp32dsp_ref32k_END (15)
#define SOC_MEDIA2_CRG_PEREN1_gt_pclk_atgm_pac_START (16)
#define SOC_MEDIA2_CRG_PEREN1_gt_pclk_atgm_pac_END (16)
#define SOC_MEDIA2_CRG_PEREN1_gt_clk_pac_tcxo_START (17)
#define SOC_MEDIA2_CRG_PEREN1_gt_clk_pac_tcxo_END (17)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_aclk_arpp_top : 1;
        unsigned int gt_pclk_arpp_top : 1;
        unsigned int gt_pclk_atgm_arpp : 1;
        unsigned int gt_clk_arpp_tcxo : 1;
        unsigned int gt_clk_arpp_top : 1;
        unsigned int gt_clk_ivp32dsp_tcxo_ivp1 : 1;
        unsigned int gt_pclk_autodiv_ivp_ivp1 : 1;
        unsigned int gt_pclk_atgm_ivp_ivp1 : 1;
        unsigned int gt_clk_qic_ivp32_cfg_ivp1 : 1;
        unsigned int gt_clk_ivp_qic_crg_ini_ivp1 : 1;
        unsigned int gt_clk_ivp_qic_peri_ini_ivp1 : 1;
        unsigned int gt_clk_vcodecbus_32k : 1;
        unsigned int gt_clk_arpp_ref32k : 1;
        unsigned int gt_clk_ipp_ref32k : 1;
        unsigned int gt_clk_ivp32dsp_ref32k_ivp1 : 1;
        unsigned int gt_clk_ivp32dsp_ref32k : 1;
        unsigned int gt_pclk_atgm_pac : 1;
        unsigned int gt_clk_pac_tcxo : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 13;
    } reg;
} SOC_MEDIA2_CRG_PERDIS1_UNION;
#endif
#define SOC_MEDIA2_CRG_PERDIS1_gt_aclk_arpp_top_START (0)
#define SOC_MEDIA2_CRG_PERDIS1_gt_aclk_arpp_top_END (0)
#define SOC_MEDIA2_CRG_PERDIS1_gt_pclk_arpp_top_START (1)
#define SOC_MEDIA2_CRG_PERDIS1_gt_pclk_arpp_top_END (1)
#define SOC_MEDIA2_CRG_PERDIS1_gt_pclk_atgm_arpp_START (2)
#define SOC_MEDIA2_CRG_PERDIS1_gt_pclk_atgm_arpp_END (2)
#define SOC_MEDIA2_CRG_PERDIS1_gt_clk_arpp_tcxo_START (3)
#define SOC_MEDIA2_CRG_PERDIS1_gt_clk_arpp_tcxo_END (3)
#define SOC_MEDIA2_CRG_PERDIS1_gt_clk_arpp_top_START (4)
#define SOC_MEDIA2_CRG_PERDIS1_gt_clk_arpp_top_END (4)
#define SOC_MEDIA2_CRG_PERDIS1_gt_clk_ivp32dsp_tcxo_ivp1_START (5)
#define SOC_MEDIA2_CRG_PERDIS1_gt_clk_ivp32dsp_tcxo_ivp1_END (5)
#define SOC_MEDIA2_CRG_PERDIS1_gt_pclk_autodiv_ivp_ivp1_START (6)
#define SOC_MEDIA2_CRG_PERDIS1_gt_pclk_autodiv_ivp_ivp1_END (6)
#define SOC_MEDIA2_CRG_PERDIS1_gt_pclk_atgm_ivp_ivp1_START (7)
#define SOC_MEDIA2_CRG_PERDIS1_gt_pclk_atgm_ivp_ivp1_END (7)
#define SOC_MEDIA2_CRG_PERDIS1_gt_clk_qic_ivp32_cfg_ivp1_START (8)
#define SOC_MEDIA2_CRG_PERDIS1_gt_clk_qic_ivp32_cfg_ivp1_END (8)
#define SOC_MEDIA2_CRG_PERDIS1_gt_clk_ivp_qic_crg_ini_ivp1_START (9)
#define SOC_MEDIA2_CRG_PERDIS1_gt_clk_ivp_qic_crg_ini_ivp1_END (9)
#define SOC_MEDIA2_CRG_PERDIS1_gt_clk_ivp_qic_peri_ini_ivp1_START (10)
#define SOC_MEDIA2_CRG_PERDIS1_gt_clk_ivp_qic_peri_ini_ivp1_END (10)
#define SOC_MEDIA2_CRG_PERDIS1_gt_clk_vcodecbus_32k_START (11)
#define SOC_MEDIA2_CRG_PERDIS1_gt_clk_vcodecbus_32k_END (11)
#define SOC_MEDIA2_CRG_PERDIS1_gt_clk_arpp_ref32k_START (12)
#define SOC_MEDIA2_CRG_PERDIS1_gt_clk_arpp_ref32k_END (12)
#define SOC_MEDIA2_CRG_PERDIS1_gt_clk_ipp_ref32k_START (13)
#define SOC_MEDIA2_CRG_PERDIS1_gt_clk_ipp_ref32k_END (13)
#define SOC_MEDIA2_CRG_PERDIS1_gt_clk_ivp32dsp_ref32k_ivp1_START (14)
#define SOC_MEDIA2_CRG_PERDIS1_gt_clk_ivp32dsp_ref32k_ivp1_END (14)
#define SOC_MEDIA2_CRG_PERDIS1_gt_clk_ivp32dsp_ref32k_START (15)
#define SOC_MEDIA2_CRG_PERDIS1_gt_clk_ivp32dsp_ref32k_END (15)
#define SOC_MEDIA2_CRG_PERDIS1_gt_pclk_atgm_pac_START (16)
#define SOC_MEDIA2_CRG_PERDIS1_gt_pclk_atgm_pac_END (16)
#define SOC_MEDIA2_CRG_PERDIS1_gt_clk_pac_tcxo_START (17)
#define SOC_MEDIA2_CRG_PERDIS1_gt_clk_pac_tcxo_END (17)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_aclk_arpp_top : 1;
        unsigned int gt_pclk_arpp_top : 1;
        unsigned int gt_pclk_atgm_arpp : 1;
        unsigned int gt_clk_arpp_tcxo : 1;
        unsigned int gt_clk_arpp_top : 1;
        unsigned int gt_clk_ivp32dsp_tcxo_ivp1 : 1;
        unsigned int gt_pclk_autodiv_ivp_ivp1 : 1;
        unsigned int gt_pclk_atgm_ivp_ivp1 : 1;
        unsigned int gt_clk_qic_ivp32_cfg_ivp1 : 1;
        unsigned int gt_clk_ivp_qic_crg_ini_ivp1 : 1;
        unsigned int gt_clk_ivp_qic_peri_ini_ivp1 : 1;
        unsigned int gt_clk_vcodecbus_32k : 1;
        unsigned int gt_clk_arpp_ref32k : 1;
        unsigned int gt_clk_ipp_ref32k : 1;
        unsigned int gt_clk_ivp32dsp_ref32k_ivp1 : 1;
        unsigned int gt_clk_ivp32dsp_ref32k : 1;
        unsigned int gt_pclk_atgm_pac : 1;
        unsigned int gt_clk_pac_tcxo : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 13;
    } reg;
} SOC_MEDIA2_CRG_PERCLKEN1_UNION;
#endif
#define SOC_MEDIA2_CRG_PERCLKEN1_gt_aclk_arpp_top_START (0)
#define SOC_MEDIA2_CRG_PERCLKEN1_gt_aclk_arpp_top_END (0)
#define SOC_MEDIA2_CRG_PERCLKEN1_gt_pclk_arpp_top_START (1)
#define SOC_MEDIA2_CRG_PERCLKEN1_gt_pclk_arpp_top_END (1)
#define SOC_MEDIA2_CRG_PERCLKEN1_gt_pclk_atgm_arpp_START (2)
#define SOC_MEDIA2_CRG_PERCLKEN1_gt_pclk_atgm_arpp_END (2)
#define SOC_MEDIA2_CRG_PERCLKEN1_gt_clk_arpp_tcxo_START (3)
#define SOC_MEDIA2_CRG_PERCLKEN1_gt_clk_arpp_tcxo_END (3)
#define SOC_MEDIA2_CRG_PERCLKEN1_gt_clk_arpp_top_START (4)
#define SOC_MEDIA2_CRG_PERCLKEN1_gt_clk_arpp_top_END (4)
#define SOC_MEDIA2_CRG_PERCLKEN1_gt_clk_ivp32dsp_tcxo_ivp1_START (5)
#define SOC_MEDIA2_CRG_PERCLKEN1_gt_clk_ivp32dsp_tcxo_ivp1_END (5)
#define SOC_MEDIA2_CRG_PERCLKEN1_gt_pclk_autodiv_ivp_ivp1_START (6)
#define SOC_MEDIA2_CRG_PERCLKEN1_gt_pclk_autodiv_ivp_ivp1_END (6)
#define SOC_MEDIA2_CRG_PERCLKEN1_gt_pclk_atgm_ivp_ivp1_START (7)
#define SOC_MEDIA2_CRG_PERCLKEN1_gt_pclk_atgm_ivp_ivp1_END (7)
#define SOC_MEDIA2_CRG_PERCLKEN1_gt_clk_qic_ivp32_cfg_ivp1_START (8)
#define SOC_MEDIA2_CRG_PERCLKEN1_gt_clk_qic_ivp32_cfg_ivp1_END (8)
#define SOC_MEDIA2_CRG_PERCLKEN1_gt_clk_ivp_qic_crg_ini_ivp1_START (9)
#define SOC_MEDIA2_CRG_PERCLKEN1_gt_clk_ivp_qic_crg_ini_ivp1_END (9)
#define SOC_MEDIA2_CRG_PERCLKEN1_gt_clk_ivp_qic_peri_ini_ivp1_START (10)
#define SOC_MEDIA2_CRG_PERCLKEN1_gt_clk_ivp_qic_peri_ini_ivp1_END (10)
#define SOC_MEDIA2_CRG_PERCLKEN1_gt_clk_vcodecbus_32k_START (11)
#define SOC_MEDIA2_CRG_PERCLKEN1_gt_clk_vcodecbus_32k_END (11)
#define SOC_MEDIA2_CRG_PERCLKEN1_gt_clk_arpp_ref32k_START (12)
#define SOC_MEDIA2_CRG_PERCLKEN1_gt_clk_arpp_ref32k_END (12)
#define SOC_MEDIA2_CRG_PERCLKEN1_gt_clk_ipp_ref32k_START (13)
#define SOC_MEDIA2_CRG_PERCLKEN1_gt_clk_ipp_ref32k_END (13)
#define SOC_MEDIA2_CRG_PERCLKEN1_gt_clk_ivp32dsp_ref32k_ivp1_START (14)
#define SOC_MEDIA2_CRG_PERCLKEN1_gt_clk_ivp32dsp_ref32k_ivp1_END (14)
#define SOC_MEDIA2_CRG_PERCLKEN1_gt_clk_ivp32dsp_ref32k_START (15)
#define SOC_MEDIA2_CRG_PERCLKEN1_gt_clk_ivp32dsp_ref32k_END (15)
#define SOC_MEDIA2_CRG_PERCLKEN1_gt_pclk_atgm_pac_START (16)
#define SOC_MEDIA2_CRG_PERCLKEN1_gt_pclk_atgm_pac_END (16)
#define SOC_MEDIA2_CRG_PERCLKEN1_gt_clk_pac_tcxo_START (17)
#define SOC_MEDIA2_CRG_PERCLKEN1_gt_clk_pac_tcxo_END (17)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int st_aclk_arpp_top : 1;
        unsigned int st_pclk_arpp_top : 1;
        unsigned int st_pclk_atgm_arpp : 1;
        unsigned int st_clk_arpp_tcxo : 1;
        unsigned int st_clk_arpp_top : 1;
        unsigned int st_clk_ivp32dsp_tcxo_ivp1 : 1;
        unsigned int st_pclk_autodiv_ivp_ivp1 : 1;
        unsigned int st_pclk_atgm_ivp_ivp1 : 1;
        unsigned int st_clk_qic_ivp32_cfg_ivp1 : 1;
        unsigned int st_clk_ivp_qic_crg_ini_ivp1 : 1;
        unsigned int st_clk_ivp_qic_peri_ini_ivp1 : 1;
        unsigned int st_clk_vcodecbus_32k : 1;
        unsigned int st_clk_arpp_ref32k : 1;
        unsigned int st_clk_ipp_ref32k : 1;
        unsigned int st_clk_ivp32dsp_ref32k_ivp1 : 1;
        unsigned int st_clk_ivp32dsp_ref32k : 1;
        unsigned int st_pclk_atgm_pac : 1;
        unsigned int st_clk_pac_tcxo : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 13;
    } reg;
} SOC_MEDIA2_CRG_PERSTAT1_UNION;
#endif
#define SOC_MEDIA2_CRG_PERSTAT1_st_aclk_arpp_top_START (0)
#define SOC_MEDIA2_CRG_PERSTAT1_st_aclk_arpp_top_END (0)
#define SOC_MEDIA2_CRG_PERSTAT1_st_pclk_arpp_top_START (1)
#define SOC_MEDIA2_CRG_PERSTAT1_st_pclk_arpp_top_END (1)
#define SOC_MEDIA2_CRG_PERSTAT1_st_pclk_atgm_arpp_START (2)
#define SOC_MEDIA2_CRG_PERSTAT1_st_pclk_atgm_arpp_END (2)
#define SOC_MEDIA2_CRG_PERSTAT1_st_clk_arpp_tcxo_START (3)
#define SOC_MEDIA2_CRG_PERSTAT1_st_clk_arpp_tcxo_END (3)
#define SOC_MEDIA2_CRG_PERSTAT1_st_clk_arpp_top_START (4)
#define SOC_MEDIA2_CRG_PERSTAT1_st_clk_arpp_top_END (4)
#define SOC_MEDIA2_CRG_PERSTAT1_st_clk_ivp32dsp_tcxo_ivp1_START (5)
#define SOC_MEDIA2_CRG_PERSTAT1_st_clk_ivp32dsp_tcxo_ivp1_END (5)
#define SOC_MEDIA2_CRG_PERSTAT1_st_pclk_autodiv_ivp_ivp1_START (6)
#define SOC_MEDIA2_CRG_PERSTAT1_st_pclk_autodiv_ivp_ivp1_END (6)
#define SOC_MEDIA2_CRG_PERSTAT1_st_pclk_atgm_ivp_ivp1_START (7)
#define SOC_MEDIA2_CRG_PERSTAT1_st_pclk_atgm_ivp_ivp1_END (7)
#define SOC_MEDIA2_CRG_PERSTAT1_st_clk_qic_ivp32_cfg_ivp1_START (8)
#define SOC_MEDIA2_CRG_PERSTAT1_st_clk_qic_ivp32_cfg_ivp1_END (8)
#define SOC_MEDIA2_CRG_PERSTAT1_st_clk_ivp_qic_crg_ini_ivp1_START (9)
#define SOC_MEDIA2_CRG_PERSTAT1_st_clk_ivp_qic_crg_ini_ivp1_END (9)
#define SOC_MEDIA2_CRG_PERSTAT1_st_clk_ivp_qic_peri_ini_ivp1_START (10)
#define SOC_MEDIA2_CRG_PERSTAT1_st_clk_ivp_qic_peri_ini_ivp1_END (10)
#define SOC_MEDIA2_CRG_PERSTAT1_st_clk_vcodecbus_32k_START (11)
#define SOC_MEDIA2_CRG_PERSTAT1_st_clk_vcodecbus_32k_END (11)
#define SOC_MEDIA2_CRG_PERSTAT1_st_clk_arpp_ref32k_START (12)
#define SOC_MEDIA2_CRG_PERSTAT1_st_clk_arpp_ref32k_END (12)
#define SOC_MEDIA2_CRG_PERSTAT1_st_clk_ipp_ref32k_START (13)
#define SOC_MEDIA2_CRG_PERSTAT1_st_clk_ipp_ref32k_END (13)
#define SOC_MEDIA2_CRG_PERSTAT1_st_clk_ivp32dsp_ref32k_ivp1_START (14)
#define SOC_MEDIA2_CRG_PERSTAT1_st_clk_ivp32dsp_ref32k_ivp1_END (14)
#define SOC_MEDIA2_CRG_PERSTAT1_st_clk_ivp32dsp_ref32k_START (15)
#define SOC_MEDIA2_CRG_PERSTAT1_st_clk_ivp32dsp_ref32k_END (15)
#define SOC_MEDIA2_CRG_PERSTAT1_st_pclk_atgm_pac_START (16)
#define SOC_MEDIA2_CRG_PERSTAT1_st_pclk_atgm_pac_END (16)
#define SOC_MEDIA2_CRG_PERSTAT1_st_clk_pac_tcxo_START (17)
#define SOC_MEDIA2_CRG_PERSTAT1_st_clk_pac_tcxo_END (17)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_rst_venc : 1;
        unsigned int ip_arst_venc : 1;
        unsigned int ip_prst_venc : 1;
        unsigned int ip_prst_atgm_pac : 1;
        unsigned int ip_prst_atgm_ivp_ivp1 : 1;
        unsigned int ip_prst_atgm_arpp : 1;
        unsigned int ip_rst_vdec : 1;
        unsigned int ip_prst_media2_lm : 1;
        unsigned int ip_prst_atgs_vcodec : 1;
        unsigned int ip_rst_tcu_vcodec : 1;
        unsigned int ip_prst_atgm_ivp : 1;
        unsigned int ip_rst_trace : 1;
        unsigned int ip_rst_ade_func : 1;
        unsigned int ip_rst_jpg_func : 1;
        unsigned int ip_prst_jpg : 1;
        unsigned int ip_arst_jpg : 1;
        unsigned int reserved : 16;
    } reg;
} SOC_MEDIA2_CRG_PERRSTEN0_UNION;
#endif
#define SOC_MEDIA2_CRG_PERRSTEN0_ip_rst_venc_START (0)
#define SOC_MEDIA2_CRG_PERRSTEN0_ip_rst_venc_END (0)
#define SOC_MEDIA2_CRG_PERRSTEN0_ip_arst_venc_START (1)
#define SOC_MEDIA2_CRG_PERRSTEN0_ip_arst_venc_END (1)
#define SOC_MEDIA2_CRG_PERRSTEN0_ip_prst_venc_START (2)
#define SOC_MEDIA2_CRG_PERRSTEN0_ip_prst_venc_END (2)
#define SOC_MEDIA2_CRG_PERRSTEN0_ip_prst_atgm_pac_START (3)
#define SOC_MEDIA2_CRG_PERRSTEN0_ip_prst_atgm_pac_END (3)
#define SOC_MEDIA2_CRG_PERRSTEN0_ip_prst_atgm_ivp_ivp1_START (4)
#define SOC_MEDIA2_CRG_PERRSTEN0_ip_prst_atgm_ivp_ivp1_END (4)
#define SOC_MEDIA2_CRG_PERRSTEN0_ip_prst_atgm_arpp_START (5)
#define SOC_MEDIA2_CRG_PERRSTEN0_ip_prst_atgm_arpp_END (5)
#define SOC_MEDIA2_CRG_PERRSTEN0_ip_rst_vdec_START (6)
#define SOC_MEDIA2_CRG_PERRSTEN0_ip_rst_vdec_END (6)
#define SOC_MEDIA2_CRG_PERRSTEN0_ip_prst_media2_lm_START (7)
#define SOC_MEDIA2_CRG_PERRSTEN0_ip_prst_media2_lm_END (7)
#define SOC_MEDIA2_CRG_PERRSTEN0_ip_prst_atgs_vcodec_START (8)
#define SOC_MEDIA2_CRG_PERRSTEN0_ip_prst_atgs_vcodec_END (8)
#define SOC_MEDIA2_CRG_PERRSTEN0_ip_rst_tcu_vcodec_START (9)
#define SOC_MEDIA2_CRG_PERRSTEN0_ip_rst_tcu_vcodec_END (9)
#define SOC_MEDIA2_CRG_PERRSTEN0_ip_prst_atgm_ivp_START (10)
#define SOC_MEDIA2_CRG_PERRSTEN0_ip_prst_atgm_ivp_END (10)
#define SOC_MEDIA2_CRG_PERRSTEN0_ip_rst_trace_START (11)
#define SOC_MEDIA2_CRG_PERRSTEN0_ip_rst_trace_END (11)
#define SOC_MEDIA2_CRG_PERRSTEN0_ip_rst_ade_func_START (12)
#define SOC_MEDIA2_CRG_PERRSTEN0_ip_rst_ade_func_END (12)
#define SOC_MEDIA2_CRG_PERRSTEN0_ip_rst_jpg_func_START (13)
#define SOC_MEDIA2_CRG_PERRSTEN0_ip_rst_jpg_func_END (13)
#define SOC_MEDIA2_CRG_PERRSTEN0_ip_prst_jpg_START (14)
#define SOC_MEDIA2_CRG_PERRSTEN0_ip_prst_jpg_END (14)
#define SOC_MEDIA2_CRG_PERRSTEN0_ip_arst_jpg_START (15)
#define SOC_MEDIA2_CRG_PERRSTEN0_ip_arst_jpg_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_rst_venc : 1;
        unsigned int ip_arst_venc : 1;
        unsigned int ip_prst_venc : 1;
        unsigned int ip_prst_atgm_pac : 1;
        unsigned int ip_prst_atgm_ivp_ivp1 : 1;
        unsigned int ip_prst_atgm_arpp : 1;
        unsigned int ip_rst_vdec : 1;
        unsigned int ip_prst_media2_lm : 1;
        unsigned int ip_prst_atgs_vcodec : 1;
        unsigned int ip_rst_tcu_vcodec : 1;
        unsigned int ip_prst_atgm_ivp : 1;
        unsigned int ip_rst_trace : 1;
        unsigned int ip_rst_ade_func : 1;
        unsigned int ip_rst_jpg_func : 1;
        unsigned int ip_prst_jpg : 1;
        unsigned int ip_arst_jpg : 1;
        unsigned int reserved : 16;
    } reg;
} SOC_MEDIA2_CRG_PERRSTDIS0_UNION;
#endif
#define SOC_MEDIA2_CRG_PERRSTDIS0_ip_rst_venc_START (0)
#define SOC_MEDIA2_CRG_PERRSTDIS0_ip_rst_venc_END (0)
#define SOC_MEDIA2_CRG_PERRSTDIS0_ip_arst_venc_START (1)
#define SOC_MEDIA2_CRG_PERRSTDIS0_ip_arst_venc_END (1)
#define SOC_MEDIA2_CRG_PERRSTDIS0_ip_prst_venc_START (2)
#define SOC_MEDIA2_CRG_PERRSTDIS0_ip_prst_venc_END (2)
#define SOC_MEDIA2_CRG_PERRSTDIS0_ip_prst_atgm_pac_START (3)
#define SOC_MEDIA2_CRG_PERRSTDIS0_ip_prst_atgm_pac_END (3)
#define SOC_MEDIA2_CRG_PERRSTDIS0_ip_prst_atgm_ivp_ivp1_START (4)
#define SOC_MEDIA2_CRG_PERRSTDIS0_ip_prst_atgm_ivp_ivp1_END (4)
#define SOC_MEDIA2_CRG_PERRSTDIS0_ip_prst_atgm_arpp_START (5)
#define SOC_MEDIA2_CRG_PERRSTDIS0_ip_prst_atgm_arpp_END (5)
#define SOC_MEDIA2_CRG_PERRSTDIS0_ip_rst_vdec_START (6)
#define SOC_MEDIA2_CRG_PERRSTDIS0_ip_rst_vdec_END (6)
#define SOC_MEDIA2_CRG_PERRSTDIS0_ip_prst_media2_lm_START (7)
#define SOC_MEDIA2_CRG_PERRSTDIS0_ip_prst_media2_lm_END (7)
#define SOC_MEDIA2_CRG_PERRSTDIS0_ip_prst_atgs_vcodec_START (8)
#define SOC_MEDIA2_CRG_PERRSTDIS0_ip_prst_atgs_vcodec_END (8)
#define SOC_MEDIA2_CRG_PERRSTDIS0_ip_rst_tcu_vcodec_START (9)
#define SOC_MEDIA2_CRG_PERRSTDIS0_ip_rst_tcu_vcodec_END (9)
#define SOC_MEDIA2_CRG_PERRSTDIS0_ip_prst_atgm_ivp_START (10)
#define SOC_MEDIA2_CRG_PERRSTDIS0_ip_prst_atgm_ivp_END (10)
#define SOC_MEDIA2_CRG_PERRSTDIS0_ip_rst_trace_START (11)
#define SOC_MEDIA2_CRG_PERRSTDIS0_ip_rst_trace_END (11)
#define SOC_MEDIA2_CRG_PERRSTDIS0_ip_rst_ade_func_START (12)
#define SOC_MEDIA2_CRG_PERRSTDIS0_ip_rst_ade_func_END (12)
#define SOC_MEDIA2_CRG_PERRSTDIS0_ip_rst_jpg_func_START (13)
#define SOC_MEDIA2_CRG_PERRSTDIS0_ip_rst_jpg_func_END (13)
#define SOC_MEDIA2_CRG_PERRSTDIS0_ip_prst_jpg_START (14)
#define SOC_MEDIA2_CRG_PERRSTDIS0_ip_prst_jpg_END (14)
#define SOC_MEDIA2_CRG_PERRSTDIS0_ip_arst_jpg_START (15)
#define SOC_MEDIA2_CRG_PERRSTDIS0_ip_arst_jpg_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_rst_venc : 1;
        unsigned int ip_arst_venc : 1;
        unsigned int ip_prst_venc : 1;
        unsigned int ip_prst_atgm_pac : 1;
        unsigned int ip_prst_atgm_ivp_ivp1 : 1;
        unsigned int ip_prst_atgm_arpp : 1;
        unsigned int ip_rst_vdec : 1;
        unsigned int ip_prst_media2_lm : 1;
        unsigned int ip_prst_atgs_vcodec : 1;
        unsigned int ip_rst_tcu_vcodec : 1;
        unsigned int ip_prst_atgm_ivp : 1;
        unsigned int ip_rst_trace : 1;
        unsigned int ip_rst_ade_func : 1;
        unsigned int ip_rst_jpg_func : 1;
        unsigned int ip_prst_jpg : 1;
        unsigned int ip_arst_jpg : 1;
        unsigned int reserved : 16;
    } reg;
} SOC_MEDIA2_CRG_PERRSTSTAT0_UNION;
#endif
#define SOC_MEDIA2_CRG_PERRSTSTAT0_ip_rst_venc_START (0)
#define SOC_MEDIA2_CRG_PERRSTSTAT0_ip_rst_venc_END (0)
#define SOC_MEDIA2_CRG_PERRSTSTAT0_ip_arst_venc_START (1)
#define SOC_MEDIA2_CRG_PERRSTSTAT0_ip_arst_venc_END (1)
#define SOC_MEDIA2_CRG_PERRSTSTAT0_ip_prst_venc_START (2)
#define SOC_MEDIA2_CRG_PERRSTSTAT0_ip_prst_venc_END (2)
#define SOC_MEDIA2_CRG_PERRSTSTAT0_ip_prst_atgm_pac_START (3)
#define SOC_MEDIA2_CRG_PERRSTSTAT0_ip_prst_atgm_pac_END (3)
#define SOC_MEDIA2_CRG_PERRSTSTAT0_ip_prst_atgm_ivp_ivp1_START (4)
#define SOC_MEDIA2_CRG_PERRSTSTAT0_ip_prst_atgm_ivp_ivp1_END (4)
#define SOC_MEDIA2_CRG_PERRSTSTAT0_ip_prst_atgm_arpp_START (5)
#define SOC_MEDIA2_CRG_PERRSTSTAT0_ip_prst_atgm_arpp_END (5)
#define SOC_MEDIA2_CRG_PERRSTSTAT0_ip_rst_vdec_START (6)
#define SOC_MEDIA2_CRG_PERRSTSTAT0_ip_rst_vdec_END (6)
#define SOC_MEDIA2_CRG_PERRSTSTAT0_ip_prst_media2_lm_START (7)
#define SOC_MEDIA2_CRG_PERRSTSTAT0_ip_prst_media2_lm_END (7)
#define SOC_MEDIA2_CRG_PERRSTSTAT0_ip_prst_atgs_vcodec_START (8)
#define SOC_MEDIA2_CRG_PERRSTSTAT0_ip_prst_atgs_vcodec_END (8)
#define SOC_MEDIA2_CRG_PERRSTSTAT0_ip_rst_tcu_vcodec_START (9)
#define SOC_MEDIA2_CRG_PERRSTSTAT0_ip_rst_tcu_vcodec_END (9)
#define SOC_MEDIA2_CRG_PERRSTSTAT0_ip_prst_atgm_ivp_START (10)
#define SOC_MEDIA2_CRG_PERRSTSTAT0_ip_prst_atgm_ivp_END (10)
#define SOC_MEDIA2_CRG_PERRSTSTAT0_ip_rst_trace_START (11)
#define SOC_MEDIA2_CRG_PERRSTSTAT0_ip_rst_trace_END (11)
#define SOC_MEDIA2_CRG_PERRSTSTAT0_ip_rst_ade_func_START (12)
#define SOC_MEDIA2_CRG_PERRSTSTAT0_ip_rst_ade_func_END (12)
#define SOC_MEDIA2_CRG_PERRSTSTAT0_ip_rst_jpg_func_START (13)
#define SOC_MEDIA2_CRG_PERRSTSTAT0_ip_rst_jpg_func_END (13)
#define SOC_MEDIA2_CRG_PERRSTSTAT0_ip_prst_jpg_START (14)
#define SOC_MEDIA2_CRG_PERRSTSTAT0_ip_prst_jpg_END (14)
#define SOC_MEDIA2_CRG_PERRSTSTAT0_ip_arst_jpg_START (15)
#define SOC_MEDIA2_CRG_PERRSTSTAT0_ip_arst_jpg_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_rst_ade_crg : 1;
        unsigned int reserved_0 : 15;
        unsigned int reserved_1 : 16;
    } reg;
} SOC_MEDIA2_CRG_PERRSTEN1_UNION;
#endif
#define SOC_MEDIA2_CRG_PERRSTEN1_ip_rst_ade_crg_START (0)
#define SOC_MEDIA2_CRG_PERRSTEN1_ip_rst_ade_crg_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_rst_ade_crg : 1;
        unsigned int reserved_0 : 15;
        unsigned int reserved_1 : 16;
    } reg;
} SOC_MEDIA2_CRG_PERRSTDIS1_UNION;
#endif
#define SOC_MEDIA2_CRG_PERRSTDIS1_ip_rst_ade_crg_START (0)
#define SOC_MEDIA2_CRG_PERRSTDIS1_ip_rst_ade_crg_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_rst_ade_crg : 1;
        unsigned int reserved_0 : 15;
        unsigned int reserved_1 : 16;
    } reg;
} SOC_MEDIA2_CRG_PERRSTSTAT1_UNION;
#endif
#define SOC_MEDIA2_CRG_PERRSTSTAT1_ip_rst_ade_crg_START (0)
#define SOC_MEDIA2_CRG_PERRSTSTAT1_ip_rst_ade_crg_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sel_vcodecbus_pll : 4;
        unsigned int sel_venc_pll : 4;
        unsigned int sel_vdec_pll : 4;
        unsigned int sel_arpp_pll : 4;
        unsigned int bitmasken : 16;
    } reg;
} SOC_MEDIA2_CRG_CLKDIV0_UNION;
#endif
#define SOC_MEDIA2_CRG_CLKDIV0_sel_vcodecbus_pll_START (0)
#define SOC_MEDIA2_CRG_CLKDIV0_sel_vcodecbus_pll_END (3)
#define SOC_MEDIA2_CRG_CLKDIV0_sel_venc_pll_START (4)
#define SOC_MEDIA2_CRG_CLKDIV0_sel_venc_pll_END (7)
#define SOC_MEDIA2_CRG_CLKDIV0_sel_vdec_pll_START (8)
#define SOC_MEDIA2_CRG_CLKDIV0_sel_vdec_pll_END (11)
#define SOC_MEDIA2_CRG_CLKDIV0_sel_arpp_pll_START (12)
#define SOC_MEDIA2_CRG_CLKDIV0_sel_arpp_pll_END (15)
#define SOC_MEDIA2_CRG_CLKDIV0_bitmasken_START (16)
#define SOC_MEDIA2_CRG_CLKDIV0_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sel_ivp32dsp_core_pll : 4;
        unsigned int reserved : 4;
        unsigned int sel_ade_func_pll : 4;
        unsigned int sel_jpg_func_pll : 4;
        unsigned int bitmasken : 16;
    } reg;
} SOC_MEDIA2_CRG_CLKDIV1_UNION;
#endif
#define SOC_MEDIA2_CRG_CLKDIV1_sel_ivp32dsp_core_pll_START (0)
#define SOC_MEDIA2_CRG_CLKDIV1_sel_ivp32dsp_core_pll_END (3)
#define SOC_MEDIA2_CRG_CLKDIV1_sel_ade_func_pll_START (8)
#define SOC_MEDIA2_CRG_CLKDIV1_sel_ade_func_pll_END (11)
#define SOC_MEDIA2_CRG_CLKDIV1_sel_jpg_func_pll_START (12)
#define SOC_MEDIA2_CRG_CLKDIV1_sel_jpg_func_pll_END (15)
#define SOC_MEDIA2_CRG_CLKDIV1_bitmasken_START (16)
#define SOC_MEDIA2_CRG_CLKDIV1_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sel_ivp32dsp_core_ivp1_pll : 4;
        unsigned int sel_vcodecbus_pll_syspll0 : 1;
        unsigned int reserved_0 : 7;
        unsigned int reserved_1 : 4;
        unsigned int bitmasken : 16;
    } reg;
} SOC_MEDIA2_CRG_CLKDIV2_UNION;
#endif
#define SOC_MEDIA2_CRG_CLKDIV2_sel_ivp32dsp_core_ivp1_pll_START (0)
#define SOC_MEDIA2_CRG_CLKDIV2_sel_ivp32dsp_core_ivp1_pll_END (3)
#define SOC_MEDIA2_CRG_CLKDIV2_sel_vcodecbus_pll_syspll0_START (4)
#define SOC_MEDIA2_CRG_CLKDIV2_sel_vcodecbus_pll_syspll0_END (4)
#define SOC_MEDIA2_CRG_CLKDIV2_bitmasken_START (16)
#define SOC_MEDIA2_CRG_CLKDIV2_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0: 4;
        unsigned int reserved_1: 4;
        unsigned int reserved_2: 4;
        unsigned int reserved_3: 4;
        unsigned int bitmasken : 16;
    } reg;
} SOC_MEDIA2_CRG_CLKDIV3_UNION;
#endif
#define SOC_MEDIA2_CRG_CLKDIV3_bitmasken_START (16)
#define SOC_MEDIA2_CRG_CLKDIV3_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0: 4;
        unsigned int reserved_1: 4;
        unsigned int reserved_2: 4;
        unsigned int reserved_3: 4;
        unsigned int bitmasken : 16;
    } reg;
} SOC_MEDIA2_CRG_CLKDIV4_UNION;
#endif
#define SOC_MEDIA2_CRG_CLKDIV4_bitmasken_START (16)
#define SOC_MEDIA2_CRG_CLKDIV4_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0: 4;
        unsigned int reserved_1: 4;
        unsigned int reserved_2: 4;
        unsigned int reserved_3: 4;
        unsigned int bitmasken : 16;
    } reg;
} SOC_MEDIA2_CRG_CLKDIV5_UNION;
#endif
#define SOC_MEDIA2_CRG_CLKDIV5_bitmasken_START (16)
#define SOC_MEDIA2_CRG_CLKDIV5_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int div_vdec : 6;
        unsigned int reserved_0: 2;
        unsigned int div_venc : 6;
        unsigned int reserved_1: 2;
        unsigned int bitmasken : 16;
    } reg;
} SOC_MEDIA2_CRG_CLKDIV6_UNION;
#endif
#define SOC_MEDIA2_CRG_CLKDIV6_div_vdec_START (0)
#define SOC_MEDIA2_CRG_CLKDIV6_div_vdec_END (5)
#define SOC_MEDIA2_CRG_CLKDIV6_div_venc_START (8)
#define SOC_MEDIA2_CRG_CLKDIV6_div_venc_END (13)
#define SOC_MEDIA2_CRG_CLKDIV6_bitmasken_START (16)
#define SOC_MEDIA2_CRG_CLKDIV6_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int div_arpp : 6;
        unsigned int reserved_0 : 2;
        unsigned int div_vcodecbus : 6;
        unsigned int reserved_1 : 2;
        unsigned int bitmasken : 16;
    } reg;
} SOC_MEDIA2_CRG_CLKDIV7_UNION;
#endif
#define SOC_MEDIA2_CRG_CLKDIV7_div_arpp_START (0)
#define SOC_MEDIA2_CRG_CLKDIV7_div_arpp_END (5)
#define SOC_MEDIA2_CRG_CLKDIV7_div_vcodecbus_START (8)
#define SOC_MEDIA2_CRG_CLKDIV7_div_vcodecbus_END (13)
#define SOC_MEDIA2_CRG_CLKDIV7_bitmasken_START (16)
#define SOC_MEDIA2_CRG_CLKDIV7_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int div_ade_func : 6;
        unsigned int reserved_0 : 2;
        unsigned int div_jpg_func : 6;
        unsigned int reserved_1 : 2;
        unsigned int bitmasken : 16;
    } reg;
} SOC_MEDIA2_CRG_CLKDIV8_UNION;
#endif
#define SOC_MEDIA2_CRG_CLKDIV8_div_ade_func_START (0)
#define SOC_MEDIA2_CRG_CLKDIV8_div_ade_func_END (5)
#define SOC_MEDIA2_CRG_CLKDIV8_div_jpg_func_START (8)
#define SOC_MEDIA2_CRG_CLKDIV8_div_jpg_func_END (13)
#define SOC_MEDIA2_CRG_CLKDIV8_bitmasken_START (16)
#define SOC_MEDIA2_CRG_CLKDIV8_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int div_ivp32dsp_core_ivp1 : 6;
        unsigned int div_ivp32dsp_core : 6;
        unsigned int reserved : 4;
        unsigned int bitmasken : 16;
    } reg;
} SOC_MEDIA2_CRG_CLKDIV9_UNION;
#endif
#define SOC_MEDIA2_CRG_CLKDIV9_div_ivp32dsp_core_ivp1_START (0)
#define SOC_MEDIA2_CRG_CLKDIV9_div_ivp32dsp_core_ivp1_END (5)
#define SOC_MEDIA2_CRG_CLKDIV9_div_ivp32dsp_core_START (6)
#define SOC_MEDIA2_CRG_CLKDIV9_div_ivp32dsp_core_END (11)
#define SOC_MEDIA2_CRG_CLKDIV9_bitmasken_START (16)
#define SOC_MEDIA2_CRG_CLKDIV9_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0: 6;
        unsigned int reserved_1: 2;
        unsigned int reserved_2: 6;
        unsigned int reserved_3: 2;
        unsigned int bitmasken : 16;
    } reg;
} SOC_MEDIA2_CRG_CLKDIV10_UNION;
#endif
#define SOC_MEDIA2_CRG_CLKDIV10_bitmasken_START (16)
#define SOC_MEDIA2_CRG_CLKDIV10_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0: 6;
        unsigned int reserved_1: 2;
        unsigned int reserved_2: 6;
        unsigned int reserved_3: 2;
        unsigned int bitmasken : 16;
    } reg;
} SOC_MEDIA2_CRG_CLKDIV11_UNION;
#endif
#define SOC_MEDIA2_CRG_CLKDIV11_bitmasken_START (16)
#define SOC_MEDIA2_CRG_CLKDIV11_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0: 6;
        unsigned int reserved_1: 2;
        unsigned int reserved_2: 6;
        unsigned int reserved_3: 2;
        unsigned int bitmasken : 16;
    } reg;
} SOC_MEDIA2_CRG_CLKDIV12_UNION;
#endif
#define SOC_MEDIA2_CRG_CLKDIV12_bitmasken_START (16)
#define SOC_MEDIA2_CRG_CLKDIV12_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0: 6;
        unsigned int reserved_1: 2;
        unsigned int reserved_2: 6;
        unsigned int reserved_3: 2;
        unsigned int bitmasken : 16;
    } reg;
} SOC_MEDIA2_CRG_CLKDIV13_UNION;
#endif
#define SOC_MEDIA2_CRG_CLKDIV13_bitmasken_START (16)
#define SOC_MEDIA2_CRG_CLKDIV13_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sc_gt_clk_vcodecbus : 1;
        unsigned int sc_gt_clk_ivp32dsp_core : 1;
        unsigned int sc_gt_clk_venc : 1;
        unsigned int sc_gt_clk_vdec : 1;
        unsigned int sc_gt_clk_arpp : 1;
        unsigned int sc_gt_clk_ivp32dsp_core_ivp1 : 1;
        unsigned int sc_gt_clk_jpg_func : 1;
        unsigned int sc_gt_clk_ade_func : 1;
        unsigned int reserved : 8;
        unsigned int bitmasken : 16;
    } reg;
} SOC_MEDIA2_CRG_CLKDIV14_UNION;
#endif
#define SOC_MEDIA2_CRG_CLKDIV14_sc_gt_clk_vcodecbus_START (0)
#define SOC_MEDIA2_CRG_CLKDIV14_sc_gt_clk_vcodecbus_END (0)
#define SOC_MEDIA2_CRG_CLKDIV14_sc_gt_clk_ivp32dsp_core_START (1)
#define SOC_MEDIA2_CRG_CLKDIV14_sc_gt_clk_ivp32dsp_core_END (1)
#define SOC_MEDIA2_CRG_CLKDIV14_sc_gt_clk_venc_START (2)
#define SOC_MEDIA2_CRG_CLKDIV14_sc_gt_clk_venc_END (2)
#define SOC_MEDIA2_CRG_CLKDIV14_sc_gt_clk_vdec_START (3)
#define SOC_MEDIA2_CRG_CLKDIV14_sc_gt_clk_vdec_END (3)
#define SOC_MEDIA2_CRG_CLKDIV14_sc_gt_clk_arpp_START (4)
#define SOC_MEDIA2_CRG_CLKDIV14_sc_gt_clk_arpp_END (4)
#define SOC_MEDIA2_CRG_CLKDIV14_sc_gt_clk_ivp32dsp_core_ivp1_START (5)
#define SOC_MEDIA2_CRG_CLKDIV14_sc_gt_clk_ivp32dsp_core_ivp1_END (5)
#define SOC_MEDIA2_CRG_CLKDIV14_sc_gt_clk_jpg_func_START (6)
#define SOC_MEDIA2_CRG_CLKDIV14_sc_gt_clk_jpg_func_END (6)
#define SOC_MEDIA2_CRG_CLKDIV14_sc_gt_clk_ade_func_START (7)
#define SOC_MEDIA2_CRG_CLKDIV14_sc_gt_clk_ade_func_END (7)
#define SOC_MEDIA2_CRG_CLKDIV14_bitmasken_START (16)
#define SOC_MEDIA2_CRG_CLKDIV14_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 16;
        unsigned int bitmasken : 16;
    } reg;
} SOC_MEDIA2_CRG_CLKDIV15_UNION;
#endif
#define SOC_MEDIA2_CRG_CLKDIV15_bitmasken_START (16)
#define SOC_MEDIA2_CRG_CLKDIV15_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int swdone_clk_vcodecbus_div : 1;
        unsigned int swdone_clk_clk_ivp32dsp_core_div : 1;
        unsigned int swdone_clk_venc_div : 1;
        unsigned int swdone_clk_vdec_div : 1;
        unsigned int swdone_clk_arpp_div : 1;
        unsigned int swdone_clk_hieps_arc_div : 1;
        unsigned int swdone_clk_jpg_func_div : 1;
        unsigned int swdone_clk_ade_func_div : 1;
        unsigned int swdone_clk_clk_ivp32dsp_core_ivp1_div : 1;
        unsigned int reserved : 23;
    } reg;
} SOC_MEDIA2_CRG_PERI_STAT0_UNION;
#endif
#define SOC_MEDIA2_CRG_PERI_STAT0_swdone_clk_vcodecbus_div_START (0)
#define SOC_MEDIA2_CRG_PERI_STAT0_swdone_clk_vcodecbus_div_END (0)
#define SOC_MEDIA2_CRG_PERI_STAT0_swdone_clk_clk_ivp32dsp_core_div_START (1)
#define SOC_MEDIA2_CRG_PERI_STAT0_swdone_clk_clk_ivp32dsp_core_div_END (1)
#define SOC_MEDIA2_CRG_PERI_STAT0_swdone_clk_venc_div_START (2)
#define SOC_MEDIA2_CRG_PERI_STAT0_swdone_clk_venc_div_END (2)
#define SOC_MEDIA2_CRG_PERI_STAT0_swdone_clk_vdec_div_START (3)
#define SOC_MEDIA2_CRG_PERI_STAT0_swdone_clk_vdec_div_END (3)
#define SOC_MEDIA2_CRG_PERI_STAT0_swdone_clk_arpp_div_START (4)
#define SOC_MEDIA2_CRG_PERI_STAT0_swdone_clk_arpp_div_END (4)
#define SOC_MEDIA2_CRG_PERI_STAT0_swdone_clk_hieps_arc_div_START (5)
#define SOC_MEDIA2_CRG_PERI_STAT0_swdone_clk_hieps_arc_div_END (5)
#define SOC_MEDIA2_CRG_PERI_STAT0_swdone_clk_jpg_func_div_START (6)
#define SOC_MEDIA2_CRG_PERI_STAT0_swdone_clk_jpg_func_div_END (6)
#define SOC_MEDIA2_CRG_PERI_STAT0_swdone_clk_ade_func_div_START (7)
#define SOC_MEDIA2_CRG_PERI_STAT0_swdone_clk_ade_func_div_END (7)
#define SOC_MEDIA2_CRG_PERI_STAT0_swdone_clk_clk_ivp32dsp_core_ivp1_div_START (8)
#define SOC_MEDIA2_CRG_PERI_STAT0_swdone_clk_clk_ivp32dsp_core_ivp1_div_END (8)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sw_ack_clk_vcodecbus_div : 4;
        unsigned int sw_ack_clk_clk_ivp32dsp_core_sw : 4;
        unsigned int sw_ack_clk_venc_sw : 4;
        unsigned int sw_ack_clk_vdec_sw : 4;
        unsigned int sw_ack_clk_arpp_sw : 4;
        unsigned int sw_ack_clk_hieps_arc_sw : 4;
        unsigned int sw_ack_clk_jpg_func_sw : 4;
        unsigned int sw_ack_clk_ade_func_sw : 4;
    } reg;
} SOC_MEDIA2_CRG_PERI_STAT1_UNION;
#endif
#define SOC_MEDIA2_CRG_PERI_STAT1_sw_ack_clk_vcodecbus_div_START (0)
#define SOC_MEDIA2_CRG_PERI_STAT1_sw_ack_clk_vcodecbus_div_END (3)
#define SOC_MEDIA2_CRG_PERI_STAT1_sw_ack_clk_clk_ivp32dsp_core_sw_START (4)
#define SOC_MEDIA2_CRG_PERI_STAT1_sw_ack_clk_clk_ivp32dsp_core_sw_END (7)
#define SOC_MEDIA2_CRG_PERI_STAT1_sw_ack_clk_venc_sw_START (8)
#define SOC_MEDIA2_CRG_PERI_STAT1_sw_ack_clk_venc_sw_END (11)
#define SOC_MEDIA2_CRG_PERI_STAT1_sw_ack_clk_vdec_sw_START (12)
#define SOC_MEDIA2_CRG_PERI_STAT1_sw_ack_clk_vdec_sw_END (15)
#define SOC_MEDIA2_CRG_PERI_STAT1_sw_ack_clk_arpp_sw_START (16)
#define SOC_MEDIA2_CRG_PERI_STAT1_sw_ack_clk_arpp_sw_END (19)
#define SOC_MEDIA2_CRG_PERI_STAT1_sw_ack_clk_hieps_arc_sw_START (20)
#define SOC_MEDIA2_CRG_PERI_STAT1_sw_ack_clk_hieps_arc_sw_END (23)
#define SOC_MEDIA2_CRG_PERI_STAT1_sw_ack_clk_jpg_func_sw_START (24)
#define SOC_MEDIA2_CRG_PERI_STAT1_sw_ack_clk_jpg_func_sw_END (27)
#define SOC_MEDIA2_CRG_PERI_STAT1_sw_ack_clk_ade_func_sw_START (28)
#define SOC_MEDIA2_CRG_PERI_STAT1_sw_ack_clk_ade_func_sw_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sw_ack_clk_clk_ivp32dsp_core_ivp1_sw : 4;
        unsigned int sw_ack_clk_vcodecbus_sw_syspll0 : 2;
        unsigned int reserved : 26;
    } reg;
} SOC_MEDIA2_CRG_PERI_STAT2_UNION;
#endif
#define SOC_MEDIA2_CRG_PERI_STAT2_sw_ack_clk_clk_ivp32dsp_core_ivp1_sw_START (0)
#define SOC_MEDIA2_CRG_PERI_STAT2_sw_ack_clk_clk_ivp32dsp_core_ivp1_sw_END (3)
#define SOC_MEDIA2_CRG_PERI_STAT2_sw_ack_clk_vcodecbus_sw_syspll0_START (4)
#define SOC_MEDIA2_CRG_PERI_STAT2_sw_ack_clk_vcodecbus_sw_syspll0_END (5)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int venc_div_auto_reduce_bypass : 1;
        unsigned int venc_auto_waitcfg_in : 10;
        unsigned int venc_auto_waitcfg_out : 10;
        unsigned int venc_div_auto_cfg : 6;
        unsigned int reserved : 5;
    } reg;
} SOC_MEDIA2_CRG_PERI_AUTODIV0_UNION;
#endif
#define SOC_MEDIA2_CRG_PERI_AUTODIV0_venc_div_auto_reduce_bypass_START (0)
#define SOC_MEDIA2_CRG_PERI_AUTODIV0_venc_div_auto_reduce_bypass_END (0)
#define SOC_MEDIA2_CRG_PERI_AUTODIV0_venc_auto_waitcfg_in_START (1)
#define SOC_MEDIA2_CRG_PERI_AUTODIV0_venc_auto_waitcfg_in_END (10)
#define SOC_MEDIA2_CRG_PERI_AUTODIV0_venc_auto_waitcfg_out_START (11)
#define SOC_MEDIA2_CRG_PERI_AUTODIV0_venc_auto_waitcfg_out_END (20)
#define SOC_MEDIA2_CRG_PERI_AUTODIV0_venc_div_auto_cfg_START (21)
#define SOC_MEDIA2_CRG_PERI_AUTODIV0_venc_div_auto_cfg_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int vdec_div_auto_reduce_bypass : 1;
        unsigned int vdec_auto_waitcfg_in : 10;
        unsigned int vdec_auto_waitcfg_out : 10;
        unsigned int vdec_div_auto_cfg : 6;
        unsigned int reserved : 5;
    } reg;
} SOC_MEDIA2_CRG_PERI_AUTODIV1_UNION;
#endif
#define SOC_MEDIA2_CRG_PERI_AUTODIV1_vdec_div_auto_reduce_bypass_START (0)
#define SOC_MEDIA2_CRG_PERI_AUTODIV1_vdec_div_auto_reduce_bypass_END (0)
#define SOC_MEDIA2_CRG_PERI_AUTODIV1_vdec_auto_waitcfg_in_START (1)
#define SOC_MEDIA2_CRG_PERI_AUTODIV1_vdec_auto_waitcfg_in_END (10)
#define SOC_MEDIA2_CRG_PERI_AUTODIV1_vdec_auto_waitcfg_out_START (11)
#define SOC_MEDIA2_CRG_PERI_AUTODIV1_vdec_auto_waitcfg_out_END (20)
#define SOC_MEDIA2_CRG_PERI_AUTODIV1_vdec_div_auto_cfg_START (21)
#define SOC_MEDIA2_CRG_PERI_AUTODIV1_vdec_div_auto_cfg_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int vcodecbus_div_auto_reduce_bypass : 1;
        unsigned int vcodecbus_auto_waitcfg_in : 10;
        unsigned int vcodecbus_auto_waitcfg_out : 8;
        unsigned int vcodecbus_ivp32_idle_bypass : 1;
        unsigned int vcodecbus_ivp32_ivp1_idle_bypass : 1;
        unsigned int vcodecbus_div_auto_cfg : 6;
        unsigned int vcodecbus_venc_idle_bypass : 1;
        unsigned int vcodecbus_vdec_idle_bypass : 1;
        unsigned int reserved : 3;
    } reg;
} SOC_MEDIA2_CRG_PERI_AUTODIV2_UNION;
#endif
#define SOC_MEDIA2_CRG_PERI_AUTODIV2_vcodecbus_div_auto_reduce_bypass_START (0)
#define SOC_MEDIA2_CRG_PERI_AUTODIV2_vcodecbus_div_auto_reduce_bypass_END (0)
#define SOC_MEDIA2_CRG_PERI_AUTODIV2_vcodecbus_auto_waitcfg_in_START (1)
#define SOC_MEDIA2_CRG_PERI_AUTODIV2_vcodecbus_auto_waitcfg_in_END (10)
#define SOC_MEDIA2_CRG_PERI_AUTODIV2_vcodecbus_auto_waitcfg_out_START (11)
#define SOC_MEDIA2_CRG_PERI_AUTODIV2_vcodecbus_auto_waitcfg_out_END (18)
#define SOC_MEDIA2_CRG_PERI_AUTODIV2_vcodecbus_ivp32_idle_bypass_START (19)
#define SOC_MEDIA2_CRG_PERI_AUTODIV2_vcodecbus_ivp32_idle_bypass_END (19)
#define SOC_MEDIA2_CRG_PERI_AUTODIV2_vcodecbus_ivp32_ivp1_idle_bypass_START (20)
#define SOC_MEDIA2_CRG_PERI_AUTODIV2_vcodecbus_ivp32_ivp1_idle_bypass_END (20)
#define SOC_MEDIA2_CRG_PERI_AUTODIV2_vcodecbus_div_auto_cfg_START (21)
#define SOC_MEDIA2_CRG_PERI_AUTODIV2_vcodecbus_div_auto_cfg_END (26)
#define SOC_MEDIA2_CRG_PERI_AUTODIV2_vcodecbus_venc_idle_bypass_START (27)
#define SOC_MEDIA2_CRG_PERI_AUTODIV2_vcodecbus_venc_idle_bypass_END (27)
#define SOC_MEDIA2_CRG_PERI_AUTODIV2_vcodecbus_vdec_idle_bypass_START (28)
#define SOC_MEDIA2_CRG_PERI_AUTODIV2_vcodecbus_vdec_idle_bypass_END (28)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int vcodecbus_arpp_bypass : 1;
        unsigned int vcodecbus_venc_bypass : 1;
        unsigned int vcodecbus_eps_bypass : 1;
        unsigned int vcodecbus_ipp_bypass : 1;
        unsigned int vcodecbus_vdec_bypass : 1;
        unsigned int vcodecbus_ivp_bypass : 1;
        unsigned int vocdecbus_ivp1_bypass : 1;
        unsigned int vcodecbus_tcu_bypass : 1;
        unsigned int vocdecbus_vcodec_bypass : 1;
        unsigned int reserved : 13;
        unsigned int vcodecbus_arpp_cfg_bypass : 1;
        unsigned int vcodecbus_venc_cfg_bypass : 1;
        unsigned int vcodecbus_eps_cfg_bypass : 1;
        unsigned int vcodecbus_ipp_cfg_bypass : 1;
        unsigned int vcodecbus_vdec_cfg_bypass : 1;
        unsigned int vcodecbus_ivp_cfg_bypass : 1;
        unsigned int vocdecbus_ivp1_cfg_bypass : 1;
        unsigned int vcodecbus_tcu_cfg_bypass : 1;
        unsigned int vocdecbus_vcodec_cfg_bypass : 1;
        unsigned int vcodecbus_perf_idle_bypass : 1;
    } reg;
} SOC_MEDIA2_CRG_PERI_AUTODIV3_UNION;
#endif
#define SOC_MEDIA2_CRG_PERI_AUTODIV3_vcodecbus_arpp_bypass_START (0)
#define SOC_MEDIA2_CRG_PERI_AUTODIV3_vcodecbus_arpp_bypass_END (0)
#define SOC_MEDIA2_CRG_PERI_AUTODIV3_vcodecbus_venc_bypass_START (1)
#define SOC_MEDIA2_CRG_PERI_AUTODIV3_vcodecbus_venc_bypass_END (1)
#define SOC_MEDIA2_CRG_PERI_AUTODIV3_vcodecbus_eps_bypass_START (2)
#define SOC_MEDIA2_CRG_PERI_AUTODIV3_vcodecbus_eps_bypass_END (2)
#define SOC_MEDIA2_CRG_PERI_AUTODIV3_vcodecbus_ipp_bypass_START (3)
#define SOC_MEDIA2_CRG_PERI_AUTODIV3_vcodecbus_ipp_bypass_END (3)
#define SOC_MEDIA2_CRG_PERI_AUTODIV3_vcodecbus_vdec_bypass_START (4)
#define SOC_MEDIA2_CRG_PERI_AUTODIV3_vcodecbus_vdec_bypass_END (4)
#define SOC_MEDIA2_CRG_PERI_AUTODIV3_vcodecbus_ivp_bypass_START (5)
#define SOC_MEDIA2_CRG_PERI_AUTODIV3_vcodecbus_ivp_bypass_END (5)
#define SOC_MEDIA2_CRG_PERI_AUTODIV3_vocdecbus_ivp1_bypass_START (6)
#define SOC_MEDIA2_CRG_PERI_AUTODIV3_vocdecbus_ivp1_bypass_END (6)
#define SOC_MEDIA2_CRG_PERI_AUTODIV3_vcodecbus_tcu_bypass_START (7)
#define SOC_MEDIA2_CRG_PERI_AUTODIV3_vcodecbus_tcu_bypass_END (7)
#define SOC_MEDIA2_CRG_PERI_AUTODIV3_vocdecbus_vcodec_bypass_START (8)
#define SOC_MEDIA2_CRG_PERI_AUTODIV3_vocdecbus_vcodec_bypass_END (8)
#define SOC_MEDIA2_CRG_PERI_AUTODIV3_vcodecbus_arpp_cfg_bypass_START (22)
#define SOC_MEDIA2_CRG_PERI_AUTODIV3_vcodecbus_arpp_cfg_bypass_END (22)
#define SOC_MEDIA2_CRG_PERI_AUTODIV3_vcodecbus_venc_cfg_bypass_START (23)
#define SOC_MEDIA2_CRG_PERI_AUTODIV3_vcodecbus_venc_cfg_bypass_END (23)
#define SOC_MEDIA2_CRG_PERI_AUTODIV3_vcodecbus_eps_cfg_bypass_START (24)
#define SOC_MEDIA2_CRG_PERI_AUTODIV3_vcodecbus_eps_cfg_bypass_END (24)
#define SOC_MEDIA2_CRG_PERI_AUTODIV3_vcodecbus_ipp_cfg_bypass_START (25)
#define SOC_MEDIA2_CRG_PERI_AUTODIV3_vcodecbus_ipp_cfg_bypass_END (25)
#define SOC_MEDIA2_CRG_PERI_AUTODIV3_vcodecbus_vdec_cfg_bypass_START (26)
#define SOC_MEDIA2_CRG_PERI_AUTODIV3_vcodecbus_vdec_cfg_bypass_END (26)
#define SOC_MEDIA2_CRG_PERI_AUTODIV3_vcodecbus_ivp_cfg_bypass_START (27)
#define SOC_MEDIA2_CRG_PERI_AUTODIV3_vcodecbus_ivp_cfg_bypass_END (27)
#define SOC_MEDIA2_CRG_PERI_AUTODIV3_vocdecbus_ivp1_cfg_bypass_START (28)
#define SOC_MEDIA2_CRG_PERI_AUTODIV3_vocdecbus_ivp1_cfg_bypass_END (28)
#define SOC_MEDIA2_CRG_PERI_AUTODIV3_vcodecbus_tcu_cfg_bypass_START (29)
#define SOC_MEDIA2_CRG_PERI_AUTODIV3_vcodecbus_tcu_cfg_bypass_END (29)
#define SOC_MEDIA2_CRG_PERI_AUTODIV3_vocdecbus_vcodec_cfg_bypass_START (30)
#define SOC_MEDIA2_CRG_PERI_AUTODIV3_vocdecbus_vcodec_cfg_bypass_END (30)
#define SOC_MEDIA2_CRG_PERI_AUTODIV3_vcodecbus_perf_idle_bypass_START (31)
#define SOC_MEDIA2_CRG_PERI_AUTODIV3_vcodecbus_perf_idle_bypass_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ivp_div_auto_reduce_bypass : 1;
        unsigned int ivp_auto_waitcfg_in : 10;
        unsigned int ivp_auto_waitcfg_out : 9;
        unsigned int reserved_0 : 1;
        unsigned int ivp_div_auto_cfg : 6;
        unsigned int ivp_pwaitmode_bypass : 1;
        unsigned int ivp_dw_axi_m1_st_bypass : 1;
        unsigned int ivp_dw_axi_m2_st_bypass : 1;
        unsigned int ivp_dw_axi_m3_st_bypass : 1;
        unsigned int reserved_1 : 1;
    } reg;
} SOC_MEDIA2_CRG_PERI_AUTODIV4_UNION;
#endif
#define SOC_MEDIA2_CRG_PERI_AUTODIV4_ivp_div_auto_reduce_bypass_START (0)
#define SOC_MEDIA2_CRG_PERI_AUTODIV4_ivp_div_auto_reduce_bypass_END (0)
#define SOC_MEDIA2_CRG_PERI_AUTODIV4_ivp_auto_waitcfg_in_START (1)
#define SOC_MEDIA2_CRG_PERI_AUTODIV4_ivp_auto_waitcfg_in_END (10)
#define SOC_MEDIA2_CRG_PERI_AUTODIV4_ivp_auto_waitcfg_out_START (11)
#define SOC_MEDIA2_CRG_PERI_AUTODIV4_ivp_auto_waitcfg_out_END (19)
#define SOC_MEDIA2_CRG_PERI_AUTODIV4_ivp_div_auto_cfg_START (21)
#define SOC_MEDIA2_CRG_PERI_AUTODIV4_ivp_div_auto_cfg_END (26)
#define SOC_MEDIA2_CRG_PERI_AUTODIV4_ivp_pwaitmode_bypass_START (27)
#define SOC_MEDIA2_CRG_PERI_AUTODIV4_ivp_pwaitmode_bypass_END (27)
#define SOC_MEDIA2_CRG_PERI_AUTODIV4_ivp_dw_axi_m1_st_bypass_START (28)
#define SOC_MEDIA2_CRG_PERI_AUTODIV4_ivp_dw_axi_m1_st_bypass_END (28)
#define SOC_MEDIA2_CRG_PERI_AUTODIV4_ivp_dw_axi_m2_st_bypass_START (29)
#define SOC_MEDIA2_CRG_PERI_AUTODIV4_ivp_dw_axi_m2_st_bypass_END (29)
#define SOC_MEDIA2_CRG_PERI_AUTODIV4_ivp_dw_axi_m3_st_bypass_START (30)
#define SOC_MEDIA2_CRG_PERI_AUTODIV4_ivp_dw_axi_m3_st_bypass_END (30)
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
} SOC_MEDIA2_CRG_PERI_AUTODIV5_UNION;
#endif
#define SOC_MEDIA2_CRG_PERI_AUTODIV5_dmss_auto_waitcfg_in_START (0)
#define SOC_MEDIA2_CRG_PERI_AUTODIV5_dmss_auto_waitcfg_in_END (9)
#define SOC_MEDIA2_CRG_PERI_AUTODIV5_dmss_auto_waitcfg_out_START (10)
#define SOC_MEDIA2_CRG_PERI_AUTODIV5_dmss_auto_waitcfg_out_END (19)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ivp_div_auto_reduce_bypass_ivp1 : 1;
        unsigned int ivp_auto_waitcfg_in_ivp1 : 10;
        unsigned int ivp_auto_waitcfg_out_ivp1 : 9;
        unsigned int reserved_0 : 1;
        unsigned int ivp_div_auto_cfg_ivp1 : 6;
        unsigned int ivp_pwaitmode_bypass_ivp1 : 1;
        unsigned int ivp_dw_axi_m1_st_bypass_ivp1 : 1;
        unsigned int ivp_dw_axi_m2_st_bypass_ivp1 : 1;
        unsigned int ivp_dw_axi_m3_st_bypass_ivp1 : 1;
        unsigned int reserved_1 : 1;
    } reg;
} SOC_MEDIA2_CRG_PERI_AUTODIV6_UNION;
#endif
#define SOC_MEDIA2_CRG_PERI_AUTODIV6_ivp_div_auto_reduce_bypass_ivp1_START (0)
#define SOC_MEDIA2_CRG_PERI_AUTODIV6_ivp_div_auto_reduce_bypass_ivp1_END (0)
#define SOC_MEDIA2_CRG_PERI_AUTODIV6_ivp_auto_waitcfg_in_ivp1_START (1)
#define SOC_MEDIA2_CRG_PERI_AUTODIV6_ivp_auto_waitcfg_in_ivp1_END (10)
#define SOC_MEDIA2_CRG_PERI_AUTODIV6_ivp_auto_waitcfg_out_ivp1_START (11)
#define SOC_MEDIA2_CRG_PERI_AUTODIV6_ivp_auto_waitcfg_out_ivp1_END (19)
#define SOC_MEDIA2_CRG_PERI_AUTODIV6_ivp_div_auto_cfg_ivp1_START (21)
#define SOC_MEDIA2_CRG_PERI_AUTODIV6_ivp_div_auto_cfg_ivp1_END (26)
#define SOC_MEDIA2_CRG_PERI_AUTODIV6_ivp_pwaitmode_bypass_ivp1_START (27)
#define SOC_MEDIA2_CRG_PERI_AUTODIV6_ivp_pwaitmode_bypass_ivp1_END (27)
#define SOC_MEDIA2_CRG_PERI_AUTODIV6_ivp_dw_axi_m1_st_bypass_ivp1_START (28)
#define SOC_MEDIA2_CRG_PERI_AUTODIV6_ivp_dw_axi_m1_st_bypass_ivp1_END (28)
#define SOC_MEDIA2_CRG_PERI_AUTODIV6_ivp_dw_axi_m2_st_bypass_ivp1_START (29)
#define SOC_MEDIA2_CRG_PERI_AUTODIV6_ivp_dw_axi_m2_st_bypass_ivp1_END (29)
#define SOC_MEDIA2_CRG_PERI_AUTODIV6_ivp_dw_axi_m3_st_bypass_ivp1_START (30)
#define SOC_MEDIA2_CRG_PERI_AUTODIV6_ivp_dw_axi_m3_st_bypass_ivp1_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int autodiv_ivp_stat : 1;
        unsigned int autodiv_vdec_stat : 1;
        unsigned int autodiv_venc_stat : 1;
        unsigned int autodiv_vcodecbus_stat : 1;
        unsigned int autodiv_ivp_stat_ivp1 : 1;
        unsigned int reserved : 27;
    } reg;
} SOC_MEDIA2_CRG_PERI_AUTODIV_STAT_UNION;
#endif
#define SOC_MEDIA2_CRG_PERI_AUTODIV_STAT_autodiv_ivp_stat_START (0)
#define SOC_MEDIA2_CRG_PERI_AUTODIV_STAT_autodiv_ivp_stat_END (0)
#define SOC_MEDIA2_CRG_PERI_AUTODIV_STAT_autodiv_vdec_stat_START (1)
#define SOC_MEDIA2_CRG_PERI_AUTODIV_STAT_autodiv_vdec_stat_END (1)
#define SOC_MEDIA2_CRG_PERI_AUTODIV_STAT_autodiv_venc_stat_START (2)
#define SOC_MEDIA2_CRG_PERI_AUTODIV_STAT_autodiv_venc_stat_END (2)
#define SOC_MEDIA2_CRG_PERI_AUTODIV_STAT_autodiv_vcodecbus_stat_START (3)
#define SOC_MEDIA2_CRG_PERI_AUTODIV_STAT_autodiv_vcodecbus_stat_END (3)
#define SOC_MEDIA2_CRG_PERI_AUTODIV_STAT_autodiv_ivp_stat_ivp1_START (4)
#define SOC_MEDIA2_CRG_PERI_AUTODIV_STAT_autodiv_ivp_stat_ivp1_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int intr_clr_media2bus_nonidle_pend : 1;
        unsigned int intr_mask_vcodecbus_nonidle_pend : 1;
        unsigned int intr_mask_epsbus_nonidle_pend : 1;
        unsigned int intr_mask_vdecbus_nonidle_pend : 1;
        unsigned int intr_mask_vencbus_nonidle_pend : 1;
        unsigned int intr_mask_ivpbus_nonidle_pend : 1;
        unsigned int intr_mask_ippbus_nonidle_pend : 1;
        unsigned int intr_mask_arppbus_nonidle_pend : 1;
        unsigned int intr_mask_ivp1bus_nonidle_pend : 1;
        unsigned int reserved : 7;
        unsigned int bitmasken : 16;
    } reg;
} SOC_MEDIA2_CRG_INTR_BUS_NONIDLE_PEND_UNION;
#endif
#define SOC_MEDIA2_CRG_INTR_BUS_NONIDLE_PEND_intr_clr_media2bus_nonidle_pend_START (0)
#define SOC_MEDIA2_CRG_INTR_BUS_NONIDLE_PEND_intr_clr_media2bus_nonidle_pend_END (0)
#define SOC_MEDIA2_CRG_INTR_BUS_NONIDLE_PEND_intr_mask_vcodecbus_nonidle_pend_START (1)
#define SOC_MEDIA2_CRG_INTR_BUS_NONIDLE_PEND_intr_mask_vcodecbus_nonidle_pend_END (1)
#define SOC_MEDIA2_CRG_INTR_BUS_NONIDLE_PEND_intr_mask_epsbus_nonidle_pend_START (2)
#define SOC_MEDIA2_CRG_INTR_BUS_NONIDLE_PEND_intr_mask_epsbus_nonidle_pend_END (2)
#define SOC_MEDIA2_CRG_INTR_BUS_NONIDLE_PEND_intr_mask_vdecbus_nonidle_pend_START (3)
#define SOC_MEDIA2_CRG_INTR_BUS_NONIDLE_PEND_intr_mask_vdecbus_nonidle_pend_END (3)
#define SOC_MEDIA2_CRG_INTR_BUS_NONIDLE_PEND_intr_mask_vencbus_nonidle_pend_START (4)
#define SOC_MEDIA2_CRG_INTR_BUS_NONIDLE_PEND_intr_mask_vencbus_nonidle_pend_END (4)
#define SOC_MEDIA2_CRG_INTR_BUS_NONIDLE_PEND_intr_mask_ivpbus_nonidle_pend_START (5)
#define SOC_MEDIA2_CRG_INTR_BUS_NONIDLE_PEND_intr_mask_ivpbus_nonidle_pend_END (5)
#define SOC_MEDIA2_CRG_INTR_BUS_NONIDLE_PEND_intr_mask_ippbus_nonidle_pend_START (6)
#define SOC_MEDIA2_CRG_INTR_BUS_NONIDLE_PEND_intr_mask_ippbus_nonidle_pend_END (6)
#define SOC_MEDIA2_CRG_INTR_BUS_NONIDLE_PEND_intr_mask_arppbus_nonidle_pend_START (7)
#define SOC_MEDIA2_CRG_INTR_BUS_NONIDLE_PEND_intr_mask_arppbus_nonidle_pend_END (7)
#define SOC_MEDIA2_CRG_INTR_BUS_NONIDLE_PEND_intr_mask_ivp1bus_nonidle_pend_START (8)
#define SOC_MEDIA2_CRG_INTR_BUS_NONIDLE_PEND_intr_mask_ivp1bus_nonidle_pend_END (8)
#define SOC_MEDIA2_CRG_INTR_BUS_NONIDLE_PEND_bitmasken_START (16)
#define SOC_MEDIA2_CRG_INTR_BUS_NONIDLE_PEND_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int intr_stat_vcodecbus_nonidle_pend : 1;
        unsigned int intr_stat_epsbus_nonidle_pend : 1;
        unsigned int intr_stat_vdecbus_nonidle_pend : 1;
        unsigned int intr_stat_vencbus_nonidle_pend : 1;
        unsigned int intr_stat_ivpbus_nonidle_pend : 1;
        unsigned int intr_stat_ippbus_nonidle_pend : 1;
        unsigned int intr_stat_arppbus_nonidle_pend : 1;
        unsigned int intr_stat_ivp1bus_nonidle_pend : 1;
        unsigned int reserved_0 : 8;
        unsigned int reserved_1 : 16;
    } reg;
} SOC_MEDIA2_CRG_INTR_STAT_BUS_NONIDLE_PEND_UNION;
#endif
#define SOC_MEDIA2_CRG_INTR_STAT_BUS_NONIDLE_PEND_intr_stat_vcodecbus_nonidle_pend_START (0)
#define SOC_MEDIA2_CRG_INTR_STAT_BUS_NONIDLE_PEND_intr_stat_vcodecbus_nonidle_pend_END (0)
#define SOC_MEDIA2_CRG_INTR_STAT_BUS_NONIDLE_PEND_intr_stat_epsbus_nonidle_pend_START (1)
#define SOC_MEDIA2_CRG_INTR_STAT_BUS_NONIDLE_PEND_intr_stat_epsbus_nonidle_pend_END (1)
#define SOC_MEDIA2_CRG_INTR_STAT_BUS_NONIDLE_PEND_intr_stat_vdecbus_nonidle_pend_START (2)
#define SOC_MEDIA2_CRG_INTR_STAT_BUS_NONIDLE_PEND_intr_stat_vdecbus_nonidle_pend_END (2)
#define SOC_MEDIA2_CRG_INTR_STAT_BUS_NONIDLE_PEND_intr_stat_vencbus_nonidle_pend_START (3)
#define SOC_MEDIA2_CRG_INTR_STAT_BUS_NONIDLE_PEND_intr_stat_vencbus_nonidle_pend_END (3)
#define SOC_MEDIA2_CRG_INTR_STAT_BUS_NONIDLE_PEND_intr_stat_ivpbus_nonidle_pend_START (4)
#define SOC_MEDIA2_CRG_INTR_STAT_BUS_NONIDLE_PEND_intr_stat_ivpbus_nonidle_pend_END (4)
#define SOC_MEDIA2_CRG_INTR_STAT_BUS_NONIDLE_PEND_intr_stat_ippbus_nonidle_pend_START (5)
#define SOC_MEDIA2_CRG_INTR_STAT_BUS_NONIDLE_PEND_intr_stat_ippbus_nonidle_pend_END (5)
#define SOC_MEDIA2_CRG_INTR_STAT_BUS_NONIDLE_PEND_intr_stat_arppbus_nonidle_pend_START (6)
#define SOC_MEDIA2_CRG_INTR_STAT_BUS_NONIDLE_PEND_intr_stat_arppbus_nonidle_pend_END (6)
#define SOC_MEDIA2_CRG_INTR_STAT_BUS_NONIDLE_PEND_intr_stat_ivp1bus_nonidle_pend_START (7)
#define SOC_MEDIA2_CRG_INTR_STAT_BUS_NONIDLE_PEND_intr_stat_ivp1bus_nonidle_pend_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int media2_lm_clkrst_bypass : 1;
        unsigned int media2_trace_clkrst_bypass : 1;
        unsigned int tcu_cfg_clkrst_bypass : 1;
        unsigned int qic_dlock_avoid_eps_bypass : 1;
        unsigned int qic_dlock_avoid_eps_cfg_bypass : 1;
        unsigned int qic_dlock_avoid_hieps2cfg_bypass : 1;
        unsigned int qic_dlock_avoid_ivp0_bypass : 1;
        unsigned int qic_dlock_avoid_ivp0_cfg_bypass : 1;
        unsigned int qic_dlock_avoid_vdec_bypass : 1;
        unsigned int qic_dlock_avoid_vdec_cfg_bypass : 1;
        unsigned int qic_dlock_avoid_venc_bypass : 1;
        unsigned int qic_dlock_avoid_venc_cfg_bypass : 1;
        unsigned int qic_dlock_avoid_arpp_bypass : 1;
        unsigned int qic_dlock_avoid_arpp_cfg_bypass : 1;
        unsigned int qic_dlock_avoid_ipp_bypass : 1;
        unsigned int qic_dlock_avoid_ipp_cfg_bypass : 1;
        unsigned int qic_dlock_avoid_ivp1_bypass : 1;
        unsigned int qic_dlock_avoid_ivp1_cfg_bypass : 1;
        unsigned int qic_dlock_avoid_vcodec_bypass : 1;
        unsigned int qic_dlock_avoid_vcodec2dmss_bypass : 1;
        unsigned int reserved : 12;
    } reg;
} SOC_MEDIA2_CRG_IPCLKRST_BYPASS_UNION;
#endif
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_media2_lm_clkrst_bypass_START (0)
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_media2_lm_clkrst_bypass_END (0)
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_media2_trace_clkrst_bypass_START (1)
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_media2_trace_clkrst_bypass_END (1)
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_tcu_cfg_clkrst_bypass_START (2)
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_tcu_cfg_clkrst_bypass_END (2)
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_qic_dlock_avoid_eps_bypass_START (3)
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_qic_dlock_avoid_eps_bypass_END (3)
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_qic_dlock_avoid_eps_cfg_bypass_START (4)
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_qic_dlock_avoid_eps_cfg_bypass_END (4)
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_qic_dlock_avoid_hieps2cfg_bypass_START (5)
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_qic_dlock_avoid_hieps2cfg_bypass_END (5)
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_qic_dlock_avoid_ivp0_bypass_START (6)
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_qic_dlock_avoid_ivp0_bypass_END (6)
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_qic_dlock_avoid_ivp0_cfg_bypass_START (7)
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_qic_dlock_avoid_ivp0_cfg_bypass_END (7)
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_qic_dlock_avoid_vdec_bypass_START (8)
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_qic_dlock_avoid_vdec_bypass_END (8)
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_qic_dlock_avoid_vdec_cfg_bypass_START (9)
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_qic_dlock_avoid_vdec_cfg_bypass_END (9)
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_qic_dlock_avoid_venc_bypass_START (10)
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_qic_dlock_avoid_venc_bypass_END (10)
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_qic_dlock_avoid_venc_cfg_bypass_START (11)
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_qic_dlock_avoid_venc_cfg_bypass_END (11)
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_qic_dlock_avoid_arpp_bypass_START (12)
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_qic_dlock_avoid_arpp_bypass_END (12)
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_qic_dlock_avoid_arpp_cfg_bypass_START (13)
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_qic_dlock_avoid_arpp_cfg_bypass_END (13)
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_qic_dlock_avoid_ipp_bypass_START (14)
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_qic_dlock_avoid_ipp_bypass_END (14)
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_qic_dlock_avoid_ipp_cfg_bypass_START (15)
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_qic_dlock_avoid_ipp_cfg_bypass_END (15)
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_qic_dlock_avoid_ivp1_bypass_START (16)
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_qic_dlock_avoid_ivp1_bypass_END (16)
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_qic_dlock_avoid_ivp1_cfg_bypass_START (17)
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_qic_dlock_avoid_ivp1_cfg_bypass_END (17)
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_qic_dlock_avoid_vcodec_bypass_START (18)
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_qic_dlock_avoid_vcodec_bypass_END (18)
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_qic_dlock_avoid_vcodec2dmss_bypass_START (19)
#define SOC_MEDIA2_CRG_IPCLKRST_BYPASS_qic_dlock_avoid_vcodec2dmss_bypass_END (19)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_pcr_cg_clk_jpg_func_bypass : 1;
        unsigned int gt_pcr_cg_clk_vdec_bypass : 1;
        unsigned int gt_pcr_cg_clk_venc_bypass : 1;
        unsigned int reserved : 13;
        unsigned int bitmasken : 16;
    } reg;
} SOC_MEDIA2_CRG_PCR_CG_BYPASS_CTRL0_UNION;
#endif
#define SOC_MEDIA2_CRG_PCR_CG_BYPASS_CTRL0_gt_pcr_cg_clk_jpg_func_bypass_START (0)
#define SOC_MEDIA2_CRG_PCR_CG_BYPASS_CTRL0_gt_pcr_cg_clk_jpg_func_bypass_END (0)
#define SOC_MEDIA2_CRG_PCR_CG_BYPASS_CTRL0_gt_pcr_cg_clk_vdec_bypass_START (1)
#define SOC_MEDIA2_CRG_PCR_CG_BYPASS_CTRL0_gt_pcr_cg_clk_vdec_bypass_END (1)
#define SOC_MEDIA2_CRG_PCR_CG_BYPASS_CTRL0_gt_pcr_cg_clk_venc_bypass_START (2)
#define SOC_MEDIA2_CRG_PCR_CG_BYPASS_CTRL0_gt_pcr_cg_clk_venc_bypass_END (2)
#define SOC_MEDIA2_CRG_PCR_CG_BYPASS_CTRL0_bitmasken_START (16)
#define SOC_MEDIA2_CRG_PCR_CG_BYPASS_CTRL0_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pcr_cg_pstate1_venc : 32;
    } reg;
} SOC_MEDIA2_CRG_PCR_CG_PSTATE1_VENC_UNION;
#endif
#define SOC_MEDIA2_CRG_PCR_CG_PSTATE1_VENC_pcr_cg_pstate1_venc_START (0)
#define SOC_MEDIA2_CRG_PCR_CG_PSTATE1_VENC_pcr_cg_pstate1_venc_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pcr_cg_pstate2_venc : 32;
    } reg;
} SOC_MEDIA2_CRG_PCR_CG_PSTATE2_VENC_UNION;
#endif
#define SOC_MEDIA2_CRG_PCR_CG_PSTATE2_VENC_pcr_cg_pstate2_venc_START (0)
#define SOC_MEDIA2_CRG_PCR_CG_PSTATE2_VENC_pcr_cg_pstate2_venc_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pcr_cg_pstate3_venc : 32;
    } reg;
} SOC_MEDIA2_CRG_PCR_CG_PSTATE3_VENC_UNION;
#endif
#define SOC_MEDIA2_CRG_PCR_CG_PSTATE3_VENC_pcr_cg_pstate3_venc_START (0)
#define SOC_MEDIA2_CRG_PCR_CG_PSTATE3_VENC_pcr_cg_pstate3_venc_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pcr_cg_pstate4_venc : 32;
    } reg;
} SOC_MEDIA2_CRG_PCR_CG_PSTATE4_VENC_UNION;
#endif
#define SOC_MEDIA2_CRG_PCR_CG_PSTATE4_VENC_pcr_cg_pstate4_venc_START (0)
#define SOC_MEDIA2_CRG_PCR_CG_PSTATE4_VENC_pcr_cg_pstate4_venc_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pcr_cg_pstate1_vdec : 32;
    } reg;
} SOC_MEDIA2_CRG_PCR_CG_PSTATE1_VDEC_UNION;
#endif
#define SOC_MEDIA2_CRG_PCR_CG_PSTATE1_VDEC_pcr_cg_pstate1_vdec_START (0)
#define SOC_MEDIA2_CRG_PCR_CG_PSTATE1_VDEC_pcr_cg_pstate1_vdec_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pcr_cg_pstate2_vdec : 32;
    } reg;
} SOC_MEDIA2_CRG_PCR_CG_PSTATE2_VDEC_UNION;
#endif
#define SOC_MEDIA2_CRG_PCR_CG_PSTATE2_VDEC_pcr_cg_pstate2_vdec_START (0)
#define SOC_MEDIA2_CRG_PCR_CG_PSTATE2_VDEC_pcr_cg_pstate2_vdec_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pcr_cg_pstate3_vdec : 32;
    } reg;
} SOC_MEDIA2_CRG_PCR_CG_PSTATE3_VDEC_UNION;
#endif
#define SOC_MEDIA2_CRG_PCR_CG_PSTATE3_VDEC_pcr_cg_pstate3_vdec_START (0)
#define SOC_MEDIA2_CRG_PCR_CG_PSTATE3_VDEC_pcr_cg_pstate3_vdec_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pcr_cg_pstate4_vdec : 32;
    } reg;
} SOC_MEDIA2_CRG_PCR_CG_PSTATE4_VDEC_UNION;
#endif
#define SOC_MEDIA2_CRG_PCR_CG_PSTATE4_VDEC_pcr_cg_pstate4_vdec_START (0)
#define SOC_MEDIA2_CRG_PCR_CG_PSTATE4_VDEC_pcr_cg_pstate4_vdec_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pcr_cg_pstate1_jpg_func : 32;
    } reg;
} SOC_MEDIA2_CRG_PCR_CG_PSTATE1_JPG_FUNC_UNION;
#endif
#define SOC_MEDIA2_CRG_PCR_CG_PSTATE1_JPG_FUNC_pcr_cg_pstate1_jpg_func_START (0)
#define SOC_MEDIA2_CRG_PCR_CG_PSTATE1_JPG_FUNC_pcr_cg_pstate1_jpg_func_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pcr_cg_pstate2_jpg_func : 32;
    } reg;
} SOC_MEDIA2_CRG_PCR_CG_PSTATE2_JPG_FUNC_UNION;
#endif
#define SOC_MEDIA2_CRG_PCR_CG_PSTATE2_JPG_FUNC_pcr_cg_pstate2_jpg_func_START (0)
#define SOC_MEDIA2_CRG_PCR_CG_PSTATE2_JPG_FUNC_pcr_cg_pstate2_jpg_func_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pcr_cg_pstate3_jpg_func : 32;
    } reg;
} SOC_MEDIA2_CRG_PCR_CG_PSTATE3_JPG_FUNC_UNION;
#endif
#define SOC_MEDIA2_CRG_PCR_CG_PSTATE3_JPG_FUNC_pcr_cg_pstate3_jpg_func_START (0)
#define SOC_MEDIA2_CRG_PCR_CG_PSTATE3_JPG_FUNC_pcr_cg_pstate3_jpg_func_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pcr_cg_pstate4_jpg_func : 32;
    } reg;
} SOC_MEDIA2_CRG_PCR_CG_PSTATE4_JPG_FUNC_UNION;
#endif
#define SOC_MEDIA2_CRG_PCR_CG_PSTATE4_JPG_FUNC_pcr_cg_pstate4_jpg_func_START (0)
#define SOC_MEDIA2_CRG_PCR_CG_PSTATE4_JPG_FUNC_pcr_cg_pstate4_jpg_func_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int qic_intr_bus_rt0 : 32;
    } reg;
} SOC_MEDIA2_CRG_QIC_INTR_BUS_RT0_UNION;
#endif
#define SOC_MEDIA2_CRG_QIC_INTR_BUS_RT0_qic_intr_bus_rt0_START (0)
#define SOC_MEDIA2_CRG_QIC_INTR_BUS_RT0_qic_intr_bus_rt0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int qic_intr_bus_rt1 : 32;
    } reg;
} SOC_MEDIA2_CRG_QIC_INTR_BUS_RT1_UNION;
#endif
#define SOC_MEDIA2_CRG_QIC_INTR_BUS_RT1_qic_intr_bus_rt1_START (0)
#define SOC_MEDIA2_CRG_QIC_INTR_BUS_RT1_qic_intr_bus_rt1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int qic_intr_bus_rt2 : 32;
    } reg;
} SOC_MEDIA2_CRG_QIC_INTR_BUS_RT2_UNION;
#endif
#define SOC_MEDIA2_CRG_QIC_INTR_BUS_RT2_qic_intr_bus_rt2_START (0)
#define SOC_MEDIA2_CRG_QIC_INTR_BUS_RT2_qic_intr_bus_rt2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int qic_intr_bus_vcodec : 32;
    } reg;
} SOC_MEDIA2_CRG_QIC_INTR_BUS_VCODEC_UNION;
#endif
#define SOC_MEDIA2_CRG_QIC_INTR_BUS_VCODEC_qic_intr_bus_vcodec_START (0)
#define SOC_MEDIA2_CRG_QIC_INTR_BUS_VCODEC_qic_intr_bus_vcodec_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int qic_intr_bus_vcodec_wa0 : 32;
    } reg;
} SOC_MEDIA2_CRG_QIC_INTR_BUS_VCODEC_WA0_UNION;
#endif
#define SOC_MEDIA2_CRG_QIC_INTR_BUS_VCODEC_WA0_qic_intr_bus_vcodec_wa0_START (0)
#define SOC_MEDIA2_CRG_QIC_INTR_BUS_VCODEC_WA0_qic_intr_bus_vcodec_wa0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int qic_intr_bus_vcodec_wa1 : 32;
    } reg;
} SOC_MEDIA2_CRG_QIC_INTR_BUS_VCODEC_WA1_UNION;
#endif
#define SOC_MEDIA2_CRG_QIC_INTR_BUS_VCODEC_WA1_qic_intr_bus_vcodec_wa1_START (0)
#define SOC_MEDIA2_CRG_QIC_INTR_BUS_VCODEC_WA1_qic_intr_bus_vcodec_wa1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int qic_intr_busy0 : 32;
    } reg;
} SOC_MEDIA2_CRG_QIC_INTR_BUS_BUSY0_UNION;
#endif
#define SOC_MEDIA2_CRG_QIC_INTR_BUS_BUSY0_qic_intr_busy0_START (0)
#define SOC_MEDIA2_CRG_QIC_INTR_BUS_BUSY0_qic_intr_busy0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int qic_intr_busy1 : 32;
    } reg;
} SOC_MEDIA2_CRG_QIC_INTR_BUS_BUSY1_UNION;
#endif
#define SOC_MEDIA2_CRG_QIC_INTR_BUS_BUSY1_qic_intr_busy1_START (0)
#define SOC_MEDIA2_CRG_QIC_INTR_BUS_BUSY1_qic_intr_busy1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int qic_intr_busy2 : 32;
    } reg;
} SOC_MEDIA2_CRG_QIC_INTR_BUS_BUSY2_UNION;
#endif
#define SOC_MEDIA2_CRG_QIC_INTR_BUS_BUSY2_qic_intr_busy2_START (0)
#define SOC_MEDIA2_CRG_QIC_INTR_BUS_BUSY2_qic_intr_busy2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_rst_ivp32dsp_subsys_crg_ivp1 : 1;
        unsigned int ip_rst_ivp32dsp_subsys_ivp1 : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 29;
    } reg;
} SOC_MEDIA2_CRG_PERRSTEN_SEC_IVP_IVP1_UNION;
#endif
#define SOC_MEDIA2_CRG_PERRSTEN_SEC_IVP_IVP1_ip_rst_ivp32dsp_subsys_crg_ivp1_START (0)
#define SOC_MEDIA2_CRG_PERRSTEN_SEC_IVP_IVP1_ip_rst_ivp32dsp_subsys_crg_ivp1_END (0)
#define SOC_MEDIA2_CRG_PERRSTEN_SEC_IVP_IVP1_ip_rst_ivp32dsp_subsys_ivp1_START (1)
#define SOC_MEDIA2_CRG_PERRSTEN_SEC_IVP_IVP1_ip_rst_ivp32dsp_subsys_ivp1_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_rst_ivp32dsp_subsys_crg_ivp1 : 1;
        unsigned int ip_rst_ivp32dsp_subsys_ivp1 : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 29;
    } reg;
} SOC_MEDIA2_CRG_PERRSTDIS_SEC_IVP_IVP1_UNION;
#endif
#define SOC_MEDIA2_CRG_PERRSTDIS_SEC_IVP_IVP1_ip_rst_ivp32dsp_subsys_crg_ivp1_START (0)
#define SOC_MEDIA2_CRG_PERRSTDIS_SEC_IVP_IVP1_ip_rst_ivp32dsp_subsys_crg_ivp1_END (0)
#define SOC_MEDIA2_CRG_PERRSTDIS_SEC_IVP_IVP1_ip_rst_ivp32dsp_subsys_ivp1_START (1)
#define SOC_MEDIA2_CRG_PERRSTDIS_SEC_IVP_IVP1_ip_rst_ivp32dsp_subsys_ivp1_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_rst_ivp32dsp_subsys_crg_ivp1 : 1;
        unsigned int ip_rst_ivp32dsp_subsys_ivp1 : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 29;
    } reg;
} SOC_MEDIA2_CRG_PERRSTSTAT_SEC_IVP_IVP1_UNION;
#endif
#define SOC_MEDIA2_CRG_PERRSTSTAT_SEC_IVP_IVP1_ip_rst_ivp32dsp_subsys_crg_ivp1_START (0)
#define SOC_MEDIA2_CRG_PERRSTSTAT_SEC_IVP_IVP1_ip_rst_ivp32dsp_subsys_crg_ivp1_END (0)
#define SOC_MEDIA2_CRG_PERRSTSTAT_SEC_IVP_IVP1_ip_rst_ivp32dsp_subsys_ivp1_START (1)
#define SOC_MEDIA2_CRG_PERRSTSTAT_SEC_IVP_IVP1_ip_rst_ivp32dsp_subsys_ivp1_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_rst_arpp_subsys_crg : 1;
        unsigned int ip_rst_arpp_subsys : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 29;
    } reg;
} SOC_MEDIA2_CRG_PERRSTEN_SEC_ARPP_UNION;
#endif
#define SOC_MEDIA2_CRG_PERRSTEN_SEC_ARPP_ip_rst_arpp_subsys_crg_START (0)
#define SOC_MEDIA2_CRG_PERRSTEN_SEC_ARPP_ip_rst_arpp_subsys_crg_END (0)
#define SOC_MEDIA2_CRG_PERRSTEN_SEC_ARPP_ip_rst_arpp_subsys_START (1)
#define SOC_MEDIA2_CRG_PERRSTEN_SEC_ARPP_ip_rst_arpp_subsys_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_rst_arpp_subsys_crg : 1;
        unsigned int ip_rst_arpp_subsys : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 29;
    } reg;
} SOC_MEDIA2_CRG_PERRSTDIS_SEC_ARPP_UNION;
#endif
#define SOC_MEDIA2_CRG_PERRSTDIS_SEC_ARPP_ip_rst_arpp_subsys_crg_START (0)
#define SOC_MEDIA2_CRG_PERRSTDIS_SEC_ARPP_ip_rst_arpp_subsys_crg_END (0)
#define SOC_MEDIA2_CRG_PERRSTDIS_SEC_ARPP_ip_rst_arpp_subsys_START (1)
#define SOC_MEDIA2_CRG_PERRSTDIS_SEC_ARPP_ip_rst_arpp_subsys_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_rst_arpp_subsys_crg : 1;
        unsigned int ip_rst_arpp_subsys : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 29;
    } reg;
} SOC_MEDIA2_CRG_PERRSTSTAT_SEC_ARPP_UNION;
#endif
#define SOC_MEDIA2_CRG_PERRSTSTAT_SEC_ARPP_ip_rst_arpp_subsys_crg_START (0)
#define SOC_MEDIA2_CRG_PERRSTSTAT_SEC_ARPP_ip_rst_arpp_subsys_crg_END (0)
#define SOC_MEDIA2_CRG_PERRSTSTAT_SEC_ARPP_ip_rst_arpp_subsys_START (1)
#define SOC_MEDIA2_CRG_PERRSTSTAT_SEC_ARPP_ip_rst_arpp_subsys_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_hieps_ahb_ckm : 1;
        unsigned int gt_clk_hieps_ahb : 1;
        unsigned int gt_clk_hieps_axi : 1;
        unsigned int gt_pclk_hieps : 1;
        unsigned int gt_aclk_hieps : 1;
        unsigned int gt_clk_hieps_tcxo : 1;
        unsigned int gt_atclk_hieps : 1;
        unsigned int gt_pclk_dbg_hieps : 1;
        unsigned int gt_clk_hieps_arc : 1;
        unsigned int gt_clk_hieps_ref32k : 1;
        unsigned int reserved : 22;
    } reg;
} SOC_MEDIA2_CRG_PEREN0_SEC_HIEPS_UNION;
#endif
#define SOC_MEDIA2_CRG_PEREN0_SEC_HIEPS_gt_clk_hieps_ahb_ckm_START (0)
#define SOC_MEDIA2_CRG_PEREN0_SEC_HIEPS_gt_clk_hieps_ahb_ckm_END (0)
#define SOC_MEDIA2_CRG_PEREN0_SEC_HIEPS_gt_clk_hieps_ahb_START (1)
#define SOC_MEDIA2_CRG_PEREN0_SEC_HIEPS_gt_clk_hieps_ahb_END (1)
#define SOC_MEDIA2_CRG_PEREN0_SEC_HIEPS_gt_clk_hieps_axi_START (2)
#define SOC_MEDIA2_CRG_PEREN0_SEC_HIEPS_gt_clk_hieps_axi_END (2)
#define SOC_MEDIA2_CRG_PEREN0_SEC_HIEPS_gt_pclk_hieps_START (3)
#define SOC_MEDIA2_CRG_PEREN0_SEC_HIEPS_gt_pclk_hieps_END (3)
#define SOC_MEDIA2_CRG_PEREN0_SEC_HIEPS_gt_aclk_hieps_START (4)
#define SOC_MEDIA2_CRG_PEREN0_SEC_HIEPS_gt_aclk_hieps_END (4)
#define SOC_MEDIA2_CRG_PEREN0_SEC_HIEPS_gt_clk_hieps_tcxo_START (5)
#define SOC_MEDIA2_CRG_PEREN0_SEC_HIEPS_gt_clk_hieps_tcxo_END (5)
#define SOC_MEDIA2_CRG_PEREN0_SEC_HIEPS_gt_atclk_hieps_START (6)
#define SOC_MEDIA2_CRG_PEREN0_SEC_HIEPS_gt_atclk_hieps_END (6)
#define SOC_MEDIA2_CRG_PEREN0_SEC_HIEPS_gt_pclk_dbg_hieps_START (7)
#define SOC_MEDIA2_CRG_PEREN0_SEC_HIEPS_gt_pclk_dbg_hieps_END (7)
#define SOC_MEDIA2_CRG_PEREN0_SEC_HIEPS_gt_clk_hieps_arc_START (8)
#define SOC_MEDIA2_CRG_PEREN0_SEC_HIEPS_gt_clk_hieps_arc_END (8)
#define SOC_MEDIA2_CRG_PEREN0_SEC_HIEPS_gt_clk_hieps_ref32k_START (9)
#define SOC_MEDIA2_CRG_PEREN0_SEC_HIEPS_gt_clk_hieps_ref32k_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_hieps_ahb_ckm : 1;
        unsigned int gt_clk_hieps_ahb : 1;
        unsigned int gt_clk_hieps_axi : 1;
        unsigned int gt_pclk_hieps : 1;
        unsigned int gt_aclk_hieps : 1;
        unsigned int gt_clk_hieps_tcxo : 1;
        unsigned int gt_atclk_hieps : 1;
        unsigned int gt_pclk_dbg_hieps : 1;
        unsigned int gt_clk_hieps_arc : 1;
        unsigned int gt_clk_hieps_ref32k : 1;
        unsigned int reserved : 22;
    } reg;
} SOC_MEDIA2_CRG_PERDIS0_SEC_HIEPS_UNION;
#endif
#define SOC_MEDIA2_CRG_PERDIS0_SEC_HIEPS_gt_clk_hieps_ahb_ckm_START (0)
#define SOC_MEDIA2_CRG_PERDIS0_SEC_HIEPS_gt_clk_hieps_ahb_ckm_END (0)
#define SOC_MEDIA2_CRG_PERDIS0_SEC_HIEPS_gt_clk_hieps_ahb_START (1)
#define SOC_MEDIA2_CRG_PERDIS0_SEC_HIEPS_gt_clk_hieps_ahb_END (1)
#define SOC_MEDIA2_CRG_PERDIS0_SEC_HIEPS_gt_clk_hieps_axi_START (2)
#define SOC_MEDIA2_CRG_PERDIS0_SEC_HIEPS_gt_clk_hieps_axi_END (2)
#define SOC_MEDIA2_CRG_PERDIS0_SEC_HIEPS_gt_pclk_hieps_START (3)
#define SOC_MEDIA2_CRG_PERDIS0_SEC_HIEPS_gt_pclk_hieps_END (3)
#define SOC_MEDIA2_CRG_PERDIS0_SEC_HIEPS_gt_aclk_hieps_START (4)
#define SOC_MEDIA2_CRG_PERDIS0_SEC_HIEPS_gt_aclk_hieps_END (4)
#define SOC_MEDIA2_CRG_PERDIS0_SEC_HIEPS_gt_clk_hieps_tcxo_START (5)
#define SOC_MEDIA2_CRG_PERDIS0_SEC_HIEPS_gt_clk_hieps_tcxo_END (5)
#define SOC_MEDIA2_CRG_PERDIS0_SEC_HIEPS_gt_atclk_hieps_START (6)
#define SOC_MEDIA2_CRG_PERDIS0_SEC_HIEPS_gt_atclk_hieps_END (6)
#define SOC_MEDIA2_CRG_PERDIS0_SEC_HIEPS_gt_pclk_dbg_hieps_START (7)
#define SOC_MEDIA2_CRG_PERDIS0_SEC_HIEPS_gt_pclk_dbg_hieps_END (7)
#define SOC_MEDIA2_CRG_PERDIS0_SEC_HIEPS_gt_clk_hieps_arc_START (8)
#define SOC_MEDIA2_CRG_PERDIS0_SEC_HIEPS_gt_clk_hieps_arc_END (8)
#define SOC_MEDIA2_CRG_PERDIS0_SEC_HIEPS_gt_clk_hieps_ref32k_START (9)
#define SOC_MEDIA2_CRG_PERDIS0_SEC_HIEPS_gt_clk_hieps_ref32k_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_hieps_ahb_ckm : 1;
        unsigned int gt_clk_hieps_ahb : 1;
        unsigned int gt_clk_hieps_axi : 1;
        unsigned int gt_pclk_hieps : 1;
        unsigned int gt_aclk_hieps : 1;
        unsigned int gt_clk_hieps_tcxo : 1;
        unsigned int gt_atclk_hieps : 1;
        unsigned int gt_pclk_dbg_hieps : 1;
        unsigned int gt_clk_hieps_arc : 1;
        unsigned int gt_clk_hieps_ref32k : 1;
        unsigned int reserved : 22;
    } reg;
} SOC_MEDIA2_CRG_PERCLKEN0_SEC_HIEPS_UNION;
#endif
#define SOC_MEDIA2_CRG_PERCLKEN0_SEC_HIEPS_gt_clk_hieps_ahb_ckm_START (0)
#define SOC_MEDIA2_CRG_PERCLKEN0_SEC_HIEPS_gt_clk_hieps_ahb_ckm_END (0)
#define SOC_MEDIA2_CRG_PERCLKEN0_SEC_HIEPS_gt_clk_hieps_ahb_START (1)
#define SOC_MEDIA2_CRG_PERCLKEN0_SEC_HIEPS_gt_clk_hieps_ahb_END (1)
#define SOC_MEDIA2_CRG_PERCLKEN0_SEC_HIEPS_gt_clk_hieps_axi_START (2)
#define SOC_MEDIA2_CRG_PERCLKEN0_SEC_HIEPS_gt_clk_hieps_axi_END (2)
#define SOC_MEDIA2_CRG_PERCLKEN0_SEC_HIEPS_gt_pclk_hieps_START (3)
#define SOC_MEDIA2_CRG_PERCLKEN0_SEC_HIEPS_gt_pclk_hieps_END (3)
#define SOC_MEDIA2_CRG_PERCLKEN0_SEC_HIEPS_gt_aclk_hieps_START (4)
#define SOC_MEDIA2_CRG_PERCLKEN0_SEC_HIEPS_gt_aclk_hieps_END (4)
#define SOC_MEDIA2_CRG_PERCLKEN0_SEC_HIEPS_gt_clk_hieps_tcxo_START (5)
#define SOC_MEDIA2_CRG_PERCLKEN0_SEC_HIEPS_gt_clk_hieps_tcxo_END (5)
#define SOC_MEDIA2_CRG_PERCLKEN0_SEC_HIEPS_gt_atclk_hieps_START (6)
#define SOC_MEDIA2_CRG_PERCLKEN0_SEC_HIEPS_gt_atclk_hieps_END (6)
#define SOC_MEDIA2_CRG_PERCLKEN0_SEC_HIEPS_gt_pclk_dbg_hieps_START (7)
#define SOC_MEDIA2_CRG_PERCLKEN0_SEC_HIEPS_gt_pclk_dbg_hieps_END (7)
#define SOC_MEDIA2_CRG_PERCLKEN0_SEC_HIEPS_gt_clk_hieps_arc_START (8)
#define SOC_MEDIA2_CRG_PERCLKEN0_SEC_HIEPS_gt_clk_hieps_arc_END (8)
#define SOC_MEDIA2_CRG_PERCLKEN0_SEC_HIEPS_gt_clk_hieps_ref32k_START (9)
#define SOC_MEDIA2_CRG_PERCLKEN0_SEC_HIEPS_gt_clk_hieps_ref32k_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int st_clk_hieps_ahb_ckm : 1;
        unsigned int st_clk_hieps_ahb : 1;
        unsigned int st_clk_hieps_axi : 1;
        unsigned int st_pclk_hieps : 1;
        unsigned int st_aclk_hieps : 1;
        unsigned int st_clk_hieps_tcxo : 1;
        unsigned int st_atclk_hieps : 1;
        unsigned int st_pclk_dbg_hieps : 1;
        unsigned int st_clk_hieps_arc : 1;
        unsigned int st_clk_hieps_ref32k : 1;
        unsigned int reserved : 22;
    } reg;
} SOC_MEDIA2_CRG_PERSTAT0_SEC_HIEPS_UNION;
#endif
#define SOC_MEDIA2_CRG_PERSTAT0_SEC_HIEPS_st_clk_hieps_ahb_ckm_START (0)
#define SOC_MEDIA2_CRG_PERSTAT0_SEC_HIEPS_st_clk_hieps_ahb_ckm_END (0)
#define SOC_MEDIA2_CRG_PERSTAT0_SEC_HIEPS_st_clk_hieps_ahb_START (1)
#define SOC_MEDIA2_CRG_PERSTAT0_SEC_HIEPS_st_clk_hieps_ahb_END (1)
#define SOC_MEDIA2_CRG_PERSTAT0_SEC_HIEPS_st_clk_hieps_axi_START (2)
#define SOC_MEDIA2_CRG_PERSTAT0_SEC_HIEPS_st_clk_hieps_axi_END (2)
#define SOC_MEDIA2_CRG_PERSTAT0_SEC_HIEPS_st_pclk_hieps_START (3)
#define SOC_MEDIA2_CRG_PERSTAT0_SEC_HIEPS_st_pclk_hieps_END (3)
#define SOC_MEDIA2_CRG_PERSTAT0_SEC_HIEPS_st_aclk_hieps_START (4)
#define SOC_MEDIA2_CRG_PERSTAT0_SEC_HIEPS_st_aclk_hieps_END (4)
#define SOC_MEDIA2_CRG_PERSTAT0_SEC_HIEPS_st_clk_hieps_tcxo_START (5)
#define SOC_MEDIA2_CRG_PERSTAT0_SEC_HIEPS_st_clk_hieps_tcxo_END (5)
#define SOC_MEDIA2_CRG_PERSTAT0_SEC_HIEPS_st_atclk_hieps_START (6)
#define SOC_MEDIA2_CRG_PERSTAT0_SEC_HIEPS_st_atclk_hieps_END (6)
#define SOC_MEDIA2_CRG_PERSTAT0_SEC_HIEPS_st_pclk_dbg_hieps_START (7)
#define SOC_MEDIA2_CRG_PERSTAT0_SEC_HIEPS_st_pclk_dbg_hieps_END (7)
#define SOC_MEDIA2_CRG_PERSTAT0_SEC_HIEPS_st_clk_hieps_arc_START (8)
#define SOC_MEDIA2_CRG_PERSTAT0_SEC_HIEPS_st_clk_hieps_arc_END (8)
#define SOC_MEDIA2_CRG_PERSTAT0_SEC_HIEPS_st_clk_hieps_ref32k_START (9)
#define SOC_MEDIA2_CRG_PERSTAT0_SEC_HIEPS_st_clk_hieps_ref32k_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 12;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int ip_rst_hieps_crg : 1;
        unsigned int ip_rst_hieps : 1;
        unsigned int reserved_3 : 16;
    } reg;
} SOC_MEDIA2_CRG_PERRSTEN0_SEC_HIEPS_UNION;
#endif
#define SOC_MEDIA2_CRG_PERRSTEN0_SEC_HIEPS_ip_rst_hieps_crg_START (14)
#define SOC_MEDIA2_CRG_PERRSTEN0_SEC_HIEPS_ip_rst_hieps_crg_END (14)
#define SOC_MEDIA2_CRG_PERRSTEN0_SEC_HIEPS_ip_rst_hieps_START (15)
#define SOC_MEDIA2_CRG_PERRSTEN0_SEC_HIEPS_ip_rst_hieps_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 12;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int ip_rst_hieps_crg : 1;
        unsigned int ip_rst_hieps : 1;
        unsigned int reserved_3 : 16;
    } reg;
} SOC_MEDIA2_CRG_PERRSTDIS0_SEC_HIEPS_UNION;
#endif
#define SOC_MEDIA2_CRG_PERRSTDIS0_SEC_HIEPS_ip_rst_hieps_crg_START (14)
#define SOC_MEDIA2_CRG_PERRSTDIS0_SEC_HIEPS_ip_rst_hieps_crg_END (14)
#define SOC_MEDIA2_CRG_PERRSTDIS0_SEC_HIEPS_ip_rst_hieps_START (15)
#define SOC_MEDIA2_CRG_PERRSTDIS0_SEC_HIEPS_ip_rst_hieps_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 12;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int ip_rst_hieps_crg : 1;
        unsigned int ip_rst_hieps : 1;
        unsigned int reserved_3 : 16;
    } reg;
} SOC_MEDIA2_CRG_PERRSTSTAT0_SEC_HIEPS_UNION;
#endif
#define SOC_MEDIA2_CRG_PERRSTSTAT0_SEC_HIEPS_ip_rst_hieps_crg_START (14)
#define SOC_MEDIA2_CRG_PERRSTSTAT0_SEC_HIEPS_ip_rst_hieps_crg_END (14)
#define SOC_MEDIA2_CRG_PERRSTSTAT0_SEC_HIEPS_ip_rst_hieps_START (15)
#define SOC_MEDIA2_CRG_PERRSTSTAT0_SEC_HIEPS_ip_rst_hieps_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sel_hieps_arc : 4;
        unsigned int sel_hieps_ahb : 4;
        unsigned int sel_hieps_axi : 4;
        unsigned int reserved : 4;
        unsigned int bitmasken : 16;
    } reg;
} SOC_MEDIA2_CRG_CLKDIV0_SEC_HIEPS_UNION;
#endif
#define SOC_MEDIA2_CRG_CLKDIV0_SEC_HIEPS_sel_hieps_arc_START (0)
#define SOC_MEDIA2_CRG_CLKDIV0_SEC_HIEPS_sel_hieps_arc_END (3)
#define SOC_MEDIA2_CRG_CLKDIV0_SEC_HIEPS_sel_hieps_ahb_START (4)
#define SOC_MEDIA2_CRG_CLKDIV0_SEC_HIEPS_sel_hieps_ahb_END (7)
#define SOC_MEDIA2_CRG_CLKDIV0_SEC_HIEPS_sel_hieps_axi_START (8)
#define SOC_MEDIA2_CRG_CLKDIV0_SEC_HIEPS_sel_hieps_axi_END (11)
#define SOC_MEDIA2_CRG_CLKDIV0_SEC_HIEPS_bitmasken_START (16)
#define SOC_MEDIA2_CRG_CLKDIV0_SEC_HIEPS_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int div_hieps_ahb : 6;
        unsigned int div_hieps_ahb_cmk : 2;
        unsigned int reserved : 2;
        unsigned int div_hieps_arc : 6;
        unsigned int bitmasken : 16;
    } reg;
} SOC_MEDIA2_CRG_CLKDIV2_SEC_HIEPS_UNION;
#endif
#define SOC_MEDIA2_CRG_CLKDIV2_SEC_HIEPS_div_hieps_ahb_START (0)
#define SOC_MEDIA2_CRG_CLKDIV2_SEC_HIEPS_div_hieps_ahb_END (5)
#define SOC_MEDIA2_CRG_CLKDIV2_SEC_HIEPS_div_hieps_ahb_cmk_START (6)
#define SOC_MEDIA2_CRG_CLKDIV2_SEC_HIEPS_div_hieps_ahb_cmk_END (7)
#define SOC_MEDIA2_CRG_CLKDIV2_SEC_HIEPS_div_hieps_arc_START (10)
#define SOC_MEDIA2_CRG_CLKDIV2_SEC_HIEPS_div_hieps_arc_END (15)
#define SOC_MEDIA2_CRG_CLKDIV2_SEC_HIEPS_bitmasken_START (16)
#define SOC_MEDIA2_CRG_CLKDIV2_SEC_HIEPS_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int div_hieps_axi : 6;
        unsigned int reserved : 10;
        unsigned int bitmasken : 16;
    } reg;
} SOC_MEDIA2_CRG_CLKDIV3_SEC_HIEPS_UNION;
#endif
#define SOC_MEDIA2_CRG_CLKDIV3_SEC_HIEPS_div_hieps_axi_START (0)
#define SOC_MEDIA2_CRG_CLKDIV3_SEC_HIEPS_div_hieps_axi_END (5)
#define SOC_MEDIA2_CRG_CLKDIV3_SEC_HIEPS_bitmasken_START (16)
#define SOC_MEDIA2_CRG_CLKDIV3_SEC_HIEPS_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sc_gt_clk_hieps_arc : 1;
        unsigned int sc_gt_clk_hieps_ahb : 1;
        unsigned int sc_gt_clk_hieps_axi : 1;
        unsigned int sc_gt_clk_hieps_ahb_ckm : 1;
        unsigned int reserved : 12;
        unsigned int bitmasken : 16;
    } reg;
} SOC_MEDIA2_CRG_CLKDIV4_SEC_HIEPS_UNION;
#endif
#define SOC_MEDIA2_CRG_CLKDIV4_SEC_HIEPS_sc_gt_clk_hieps_arc_START (0)
#define SOC_MEDIA2_CRG_CLKDIV4_SEC_HIEPS_sc_gt_clk_hieps_arc_END (0)
#define SOC_MEDIA2_CRG_CLKDIV4_SEC_HIEPS_sc_gt_clk_hieps_ahb_START (1)
#define SOC_MEDIA2_CRG_CLKDIV4_SEC_HIEPS_sc_gt_clk_hieps_ahb_END (1)
#define SOC_MEDIA2_CRG_CLKDIV4_SEC_HIEPS_sc_gt_clk_hieps_axi_START (2)
#define SOC_MEDIA2_CRG_CLKDIV4_SEC_HIEPS_sc_gt_clk_hieps_axi_END (2)
#define SOC_MEDIA2_CRG_CLKDIV4_SEC_HIEPS_sc_gt_clk_hieps_ahb_ckm_START (3)
#define SOC_MEDIA2_CRG_CLKDIV4_SEC_HIEPS_sc_gt_clk_hieps_ahb_ckm_END (3)
#define SOC_MEDIA2_CRG_CLKDIV4_SEC_HIEPS_bitmasken_START (16)
#define SOC_MEDIA2_CRG_CLKDIV4_SEC_HIEPS_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int swdone_clk_hieps_arc_div : 1;
        unsigned int swdone_clk_hieps_ahb_div : 1;
        unsigned int swdone_clk_hieps_axi_div : 1;
        unsigned int swdone_clk_hieps_ahb_cmk_div : 1;
        unsigned int reserved : 28;
    } reg;
} SOC_MEDIA2_CRG_PERI_STAT0_SEC_HIEPS_UNION;
#endif
#define SOC_MEDIA2_CRG_PERI_STAT0_SEC_HIEPS_swdone_clk_hieps_arc_div_START (0)
#define SOC_MEDIA2_CRG_PERI_STAT0_SEC_HIEPS_swdone_clk_hieps_arc_div_END (0)
#define SOC_MEDIA2_CRG_PERI_STAT0_SEC_HIEPS_swdone_clk_hieps_ahb_div_START (1)
#define SOC_MEDIA2_CRG_PERI_STAT0_SEC_HIEPS_swdone_clk_hieps_ahb_div_END (1)
#define SOC_MEDIA2_CRG_PERI_STAT0_SEC_HIEPS_swdone_clk_hieps_axi_div_START (2)
#define SOC_MEDIA2_CRG_PERI_STAT0_SEC_HIEPS_swdone_clk_hieps_axi_div_END (2)
#define SOC_MEDIA2_CRG_PERI_STAT0_SEC_HIEPS_swdone_clk_hieps_ahb_cmk_div_START (3)
#define SOC_MEDIA2_CRG_PERI_STAT0_SEC_HIEPS_swdone_clk_hieps_ahb_cmk_div_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sw_ack_clk_hieps_arc : 4;
        unsigned int sw_ack_clk_hieps_ahb : 4;
        unsigned int sw_ack_clk_hieps_axi : 4;
        unsigned int reserved : 20;
    } reg;
} SOC_MEDIA2_CRG_PERI_STAT1_SEC_HIEPS_UNION;
#endif
#define SOC_MEDIA2_CRG_PERI_STAT1_SEC_HIEPS_sw_ack_clk_hieps_arc_START (0)
#define SOC_MEDIA2_CRG_PERI_STAT1_SEC_HIEPS_sw_ack_clk_hieps_arc_END (3)
#define SOC_MEDIA2_CRG_PERI_STAT1_SEC_HIEPS_sw_ack_clk_hieps_ahb_START (4)
#define SOC_MEDIA2_CRG_PERI_STAT1_SEC_HIEPS_sw_ack_clk_hieps_ahb_END (7)
#define SOC_MEDIA2_CRG_PERI_STAT1_SEC_HIEPS_sw_ack_clk_hieps_axi_START (8)
#define SOC_MEDIA2_CRG_PERI_STAT1_SEC_HIEPS_sw_ack_clk_hieps_axi_END (11)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_rst_ivp32dsp_subsys_crg : 1;
        unsigned int ip_rst_ivp32dsp_subsys : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 29;
    } reg;
} SOC_MEDIA2_CRG_PERRSTEN_SEC_IVP_UNION;
#endif
#define SOC_MEDIA2_CRG_PERRSTEN_SEC_IVP_ip_rst_ivp32dsp_subsys_crg_START (0)
#define SOC_MEDIA2_CRG_PERRSTEN_SEC_IVP_ip_rst_ivp32dsp_subsys_crg_END (0)
#define SOC_MEDIA2_CRG_PERRSTEN_SEC_IVP_ip_rst_ivp32dsp_subsys_START (1)
#define SOC_MEDIA2_CRG_PERRSTEN_SEC_IVP_ip_rst_ivp32dsp_subsys_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_rst_ivp32dsp_subsys_crg : 1;
        unsigned int ip_rst_ivp32dsp_subsys : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 29;
    } reg;
} SOC_MEDIA2_CRG_PERRSTDIS_SEC_IVP_UNION;
#endif
#define SOC_MEDIA2_CRG_PERRSTDIS_SEC_IVP_ip_rst_ivp32dsp_subsys_crg_START (0)
#define SOC_MEDIA2_CRG_PERRSTDIS_SEC_IVP_ip_rst_ivp32dsp_subsys_crg_END (0)
#define SOC_MEDIA2_CRG_PERRSTDIS_SEC_IVP_ip_rst_ivp32dsp_subsys_START (1)
#define SOC_MEDIA2_CRG_PERRSTDIS_SEC_IVP_ip_rst_ivp32dsp_subsys_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_rst_ivp32dsp_subsys_crg : 1;
        unsigned int ip_rst_ivp32dsp_subsys : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 29;
    } reg;
} SOC_MEDIA2_CRG_PERRSTSTAT_SEC_IVP_UNION;
#endif
#define SOC_MEDIA2_CRG_PERRSTSTAT_SEC_IVP_ip_rst_ivp32dsp_subsys_crg_START (0)
#define SOC_MEDIA2_CRG_PERRSTSTAT_SEC_IVP_ip_rst_ivp32dsp_subsys_crg_END (0)
#define SOC_MEDIA2_CRG_PERRSTSTAT_SEC_IVP_ip_rst_ivp32dsp_subsys_START (1)
#define SOC_MEDIA2_CRG_PERRSTSTAT_SEC_IVP_ip_rst_ivp32dsp_subsys_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int tz_secure_n_med2_lm : 1;
        unsigned int tz_secure_n_hieps : 1;
        unsigned int tz_secure_n_arpp : 1;
        unsigned int reserved : 29;
    } reg;
} SOC_MEDIA2_CRG_DECPROT0SET_UNION;
#endif
#define SOC_MEDIA2_CRG_DECPROT0SET_tz_secure_n_med2_lm_START (0)
#define SOC_MEDIA2_CRG_DECPROT0SET_tz_secure_n_med2_lm_END (0)
#define SOC_MEDIA2_CRG_DECPROT0SET_tz_secure_n_hieps_START (1)
#define SOC_MEDIA2_CRG_DECPROT0SET_tz_secure_n_hieps_END (1)
#define SOC_MEDIA2_CRG_DECPROT0SET_tz_secure_n_arpp_START (2)
#define SOC_MEDIA2_CRG_DECPROT0SET_tz_secure_n_arpp_END (2)
#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
#endif
