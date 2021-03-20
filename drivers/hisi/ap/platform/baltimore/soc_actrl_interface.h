#ifndef __SOC_ACTRL_INTERFACE_H__
#define __SOC_ACTRL_INTERFACE_H__ 
#ifdef __cplusplus
    #if __cplusplus
        extern "C" {
    #endif
#endif
#ifndef __SOC_H_FOR_ASM__
#define SOC_ACTRL_ISO_EN_GROUP0_PERI_ADDR(base) ((base) + (0x000UL))
#define SOC_ACTRL_ISO_EN_GROUP1_PERI_ADDR(base) ((base) + (0x004UL))
#define SOC_ACTRL_ISO_EN_GROUP2_PERI_ADDR(base) ((base) + (0x008UL))
#define SOC_ACTRL_ISO_EN_AO_ADDR(base) ((base) + (0x00CUL))
#define SOC_ACTRL_MTCMOS_EN_GROUP0_PERI_ADDR(base) ((base) + (0x010UL))
#define SOC_ACTRL_MTCMOS_EN_GROUP1_PERI_ADDR(base) ((base) + (0x014UL))
#define SOC_ACTRL_MTCMOS_EN_GROUP2_PERI_ADDR(base) ((base) + (0x018UL))
#define SOC_ACTRL_MTCMOS_EN_AO_ADDR(base) ((base) + (0x01CUL))
#define SOC_ACTRL_MTCMOS_ACK0_PERI_ADDR(base) ((base) + (0x020UL))
#define SOC_ACTRL_MTCMOS_ACK1_PERI_ADDR(base) ((base) + (0x024UL))
#define SOC_ACTRL_MTCMOS_ACK_AO_ADDR(base) ((base) + (0x028UL))
#define SOC_ACTRL_BITMSK_NONSEC_CTRL0_ADDR(base) ((base) + (0x02CUL))
#define SOC_ACTRL_NONSEC_STATUS0_ADDR(base) ((base) + (0x030UL))
#define SOC_ACTRL_AO_BISR_SPRAM_MEM_CTRL_ADDR(base) ((base) + (0x038UL))
#define SOC_ACTRL_AO_SPI_SRAM_MEM_CTRL_ADDR(base) ((base) + (0x03CUL))
#define SOC_ACTRL_BISR_MRB_BUSY_FLAG_STATUS0_ADDR(base) ((base) + (0x040UL))
#define SOC_ACTRL_BISR_MRB_BUSY_FLAG_STATUS1_ADDR(base) ((base) + (0x044UL))
#define SOC_ACTRL_BISR_MRB_BUSY_FLAG_STATUS2_ADDR(base) ((base) + (0x048UL))
#define SOC_ACTRL_BISR_REPAIR_DONE_STATUS0_ADDR(base) ((base) + (0x04CUL))
#define SOC_ACTRL_BISR_REPAIR_DONE_STATUS1_ADDR(base) ((base) + (0x050UL))
#define SOC_ACTRL_BISR_REPAIR_DONE_STATUS2_ADDR(base) ((base) + (0x054UL))
#define SOC_ACTRL_BISR_REPAIR_ACK_STATUS0_ADDR(base) ((base) + (0x058UL))
#define SOC_ACTRL_BISR_REPAIR_ACK_STATUS1_ADDR(base) ((base) + (0x05CUL))
#define SOC_ACTRL_BISR_REPAIR_ACK_STATUS2_ADDR(base) ((base) + (0x060UL))
#define SOC_ACTRL_BISR_DBG_INFO_AO_STAUS0_ADDR(base) ((base) + (0x064UL))
#define SOC_ACTRL_BISR_DBG_INFO_AO_STAUS1_ADDR(base) ((base) + (0x068UL))
#define SOC_ACTRL_BITMSK_NONSEC_CTRL2_ADDR(base) ((base) + (0x06CUL))
#define SOC_ACTRL_BITMSK_NONSEC_CTRL3_ADDR(base) ((base) + (0x070UL))
#define SOC_ACTRL_BITMSK_NONSEC_CTRL4_ADDR(base) ((base) + (0x074UL))
#define SOC_ACTRL_BITMSK_NONSEC_CTRL5_ADDR(base) ((base) + (0x078UL))
#define SOC_ACTRL_NONSEC_STATUS1_ADDR(base) ((base) + (0x07CUL))
#define SOC_ACTRL_NONSEC_STATUS2_ADDR(base) ((base) + (0x080UL))
#define SOC_ACTRL_BITMSK_NONSEC_CTRL1_ADDR(base) ((base) + (0x400UL))
#define SOC_ACTRL_IOMCU_MEM_CTRL0_ADDR(base) ((base) + (0x800UL))
#define SOC_ACTRL_IOMCU_MEM_CTRL1_ADDR(base) ((base) + (0x804UL))
#define SOC_ACTRL_IOMCU_MEM_CTRL3_ADDR(base) ((base) + (0x808UL))
#define SOC_ACTRL_AO_MEM_CTRL0_ADDR(base) ((base) + (0x80CUL))
#define SOC_ACTRL_AO_MEM_CTRL1_ADDR(base) ((base) + (0x810UL))
#define SOC_ACTRL_AO_MEM_CTRL2_ADDR(base) ((base) + (0x814UL))
#define SOC_ACTRL_AO_MEM_CTRL3_ADDR(base) ((base) + (0x818UL))
#define SOC_ACTRL_AO_MEM_CTRL4_ADDR(base) ((base) + (0x81CUL))
#define SOC_ACTRL_AO_MEM_CTRL5_ADDR(base) ((base) + (0x820UL))
#define SOC_ACTRL_AO_MEM_CTRL6_ADDR(base) ((base) + (0x824UL))
#define SOC_ACTRL_AO_MEM_CTRL7_ADDR(base) ((base) + (0x828UL))
#define SOC_ACTRL_AO_MEM_CTRL8_ADDR(base) ((base) + (0x82CUL))
#define SOC_ACTRL_AO_MEM_CTRL9_ADDR(base) ((base) + (0x830UL))
#define SOC_ACTRL_AO_MEM_STATUS_ADDR(base) ((base) + (0x834UL))
#define SOC_ACTRL_SEC_CTRL0_ADDR(base) ((base) + (0x838UL))
#define SOC_ACTRL_SEC_STATUS0_ADDR(base) ((base) + (0x844UL))
#define SOC_ACTRL_AO_MEM_CTRL10_ADDR(base) ((base) + (0x848UL))
#define SOC_ACTRL_BITMSK_SEC_CTRL1_ADDR(base) ((base) + (0x84CUL))
#define SOC_ACTRL_BITMSK_SEC_CTRL2_ADDR(base) ((base) + (0x850UL))
#define SOC_ACTRL_BITMSK_SEC_CTRL3_ADDR(base) ((base) + (0x854UL))
#define SOC_ACTRL_BITMSK_SEC_CTRL4_ADDR(base) ((base) + (0x858UL))
#define SOC_ACTRL_SEC_STATUS1_ADDR(base) ((base) + (0x85CUL))
#define SOC_ACTRL_SEC_STATUS2_ADDR(base) ((base) + (0x860UL))
#define SOC_ACTRL_BITMSK_SEC_CTRL0_ADDR(base) ((base) + (0xC00UL))
#define SOC_ACTRL_ISO_EN_GROUP_SEC_ADDR(base) ((base) + (0xF00UL))
#define SOC_ACTRL_MTCMOS_EN_GROUP_SEC_ADDR(base) ((base) + (0xF04UL))
#define SOC_ACTRL_MTCMOS_ACK_GROUP_SEC_ADDR(base) ((base) + (0xF08UL))
#define SOC_ACTRL_HIEPS_DCU_EN_ADDR(base) ((base) + (0xF0CUL))
#define SOC_ACTRL_HIEPS_CTRL_SEC_ADDR(base) ((base) + (0xF10UL))
#define SOC_ACTRL_HIEPS_STATE_COUNTER_INC_REG_ADDR(base) ((base) + (0xF14UL))
#define SOC_ACTRL_HIEPS_STATE_COUNTER_ADDR(base) ((base) + (0xF18UL))
#define SOC_ACTRL_HIEPS_CTRL0_ADDR(base) ((base) + (0xF1CUL))
#define SOC_ACTRL_HIEPS_CTRL1_ADDR(base) ((base) + (0xF20UL))
#define SOC_ACTRL_HIEPS_CTRL2_ADDR(base) ((base) + (0xF24UL))
#define SOC_ACTRL_HIEPS_CTRL3_ADDR(base) ((base) + (0xF28UL))
#define SOC_ACTRL_HIEPS_CTRL4_ADDR(base) ((base) + (0xF2CUL))
#define SOC_ACTRL_HIEPS_CTRL5_ADDR(base) ((base) + (0xF30UL))
#define SOC_ACTRL_HIEPS_CTRL6_ADDR(base) ((base) + (0xF34UL))
#define SOC_ACTRL_HIEPS_CTRL7_ADDR(base) ((base) + (0xF38UL))
#define SOC_ACTRL_HIEPS_CTRL8_ADDR(base) ((base) + (0xF3CUL))
#define SOC_ACTRL_HIEPS_CTRL9_ADDR(base) ((base) + (0xF40UL))
#define SOC_ACTRL_HIEPS_CTRL10_ADDR(base) ((base) + (0xF44UL))
#define SOC_ACTRL_HIEPS_CTRL11_ADDR(base) ((base) + (0xF48UL))
#define SOC_ACTRL_HIEPS_CTRL12_ADDR(base) ((base) + (0xF4CUL))
#define SOC_ACTRL_HIEPS_CTRL13_ADDR(base) ((base) + (0xF50UL))
#define SOC_ACTRL_HIEPS_CTRL14_ADDR(base) ((base) + (0xF54UL))
#define SOC_ACTRL_HIEPS_CTRL15_ADDR(base) ((base) + (0xF58UL))
#define SOC_ACTRL_ACTRL2EPS_REG0_ADDR(base) ((base) + (0xF5CUL))
#define SOC_ACTRL_ACTRL2EPS_REG1_ADDR(base) ((base) + (0xF60UL))
#define SOC_ACTRL_ACTRL2EPS_REG2_ADDR(base) ((base) + (0xF64UL))
#define SOC_ACTRL_ACTRL2EPS_REG3_ADDR(base) ((base) + (0xF68UL))
#define SOC_ACTRL_ACTRL2EPS_REG4_ADDR(base) ((base) + (0xF6CUL))
#define SOC_ACTRL_EPS2ACTRL_REG0_ADDR(base) ((base) + (0xF70UL))
#define SOC_ACTRL_EPS2ACTRL_REG1_ADDR(base) ((base) + (0xF74UL))
#define SOC_ACTRL_EPS2ACTRL_REG2_ADDR(base) ((base) + (0xF78UL))
#define SOC_ACTRL_EPS2ACTRL_REG3_ADDR(base) ((base) + (0xF7CUL))
#define SOC_ACTRL_EPS2ACTRL_REG4_ADDR(base) ((base) + (0xF80UL))
#define SOC_ACTRL_HIEPS_MEM_CTRL_ADDR(base) ((base) + (0xF84UL))
#else
#define SOC_ACTRL_ISO_EN_GROUP0_PERI_ADDR(base) ((base) + (0x000))
#define SOC_ACTRL_ISO_EN_GROUP1_PERI_ADDR(base) ((base) + (0x004))
#define SOC_ACTRL_ISO_EN_GROUP2_PERI_ADDR(base) ((base) + (0x008))
#define SOC_ACTRL_ISO_EN_AO_ADDR(base) ((base) + (0x00C))
#define SOC_ACTRL_MTCMOS_EN_GROUP0_PERI_ADDR(base) ((base) + (0x010))
#define SOC_ACTRL_MTCMOS_EN_GROUP1_PERI_ADDR(base) ((base) + (0x014))
#define SOC_ACTRL_MTCMOS_EN_GROUP2_PERI_ADDR(base) ((base) + (0x018))
#define SOC_ACTRL_MTCMOS_EN_AO_ADDR(base) ((base) + (0x01C))
#define SOC_ACTRL_MTCMOS_ACK0_PERI_ADDR(base) ((base) + (0x020))
#define SOC_ACTRL_MTCMOS_ACK1_PERI_ADDR(base) ((base) + (0x024))
#define SOC_ACTRL_MTCMOS_ACK_AO_ADDR(base) ((base) + (0x028))
#define SOC_ACTRL_BITMSK_NONSEC_CTRL0_ADDR(base) ((base) + (0x02C))
#define SOC_ACTRL_NONSEC_STATUS0_ADDR(base) ((base) + (0x030))
#define SOC_ACTRL_AO_BISR_SPRAM_MEM_CTRL_ADDR(base) ((base) + (0x038))
#define SOC_ACTRL_AO_SPI_SRAM_MEM_CTRL_ADDR(base) ((base) + (0x03C))
#define SOC_ACTRL_BISR_MRB_BUSY_FLAG_STATUS0_ADDR(base) ((base) + (0x040))
#define SOC_ACTRL_BISR_MRB_BUSY_FLAG_STATUS1_ADDR(base) ((base) + (0x044))
#define SOC_ACTRL_BISR_MRB_BUSY_FLAG_STATUS2_ADDR(base) ((base) + (0x048))
#define SOC_ACTRL_BISR_REPAIR_DONE_STATUS0_ADDR(base) ((base) + (0x04C))
#define SOC_ACTRL_BISR_REPAIR_DONE_STATUS1_ADDR(base) ((base) + (0x050))
#define SOC_ACTRL_BISR_REPAIR_DONE_STATUS2_ADDR(base) ((base) + (0x054))
#define SOC_ACTRL_BISR_REPAIR_ACK_STATUS0_ADDR(base) ((base) + (0x058))
#define SOC_ACTRL_BISR_REPAIR_ACK_STATUS1_ADDR(base) ((base) + (0x05C))
#define SOC_ACTRL_BISR_REPAIR_ACK_STATUS2_ADDR(base) ((base) + (0x060))
#define SOC_ACTRL_BISR_DBG_INFO_AO_STAUS0_ADDR(base) ((base) + (0x064))
#define SOC_ACTRL_BISR_DBG_INFO_AO_STAUS1_ADDR(base) ((base) + (0x068))
#define SOC_ACTRL_BITMSK_NONSEC_CTRL2_ADDR(base) ((base) + (0x06C))
#define SOC_ACTRL_BITMSK_NONSEC_CTRL3_ADDR(base) ((base) + (0x070))
#define SOC_ACTRL_BITMSK_NONSEC_CTRL4_ADDR(base) ((base) + (0x074))
#define SOC_ACTRL_BITMSK_NONSEC_CTRL5_ADDR(base) ((base) + (0x078))
#define SOC_ACTRL_NONSEC_STATUS1_ADDR(base) ((base) + (0x07C))
#define SOC_ACTRL_NONSEC_STATUS2_ADDR(base) ((base) + (0x080))
#define SOC_ACTRL_BITMSK_NONSEC_CTRL1_ADDR(base) ((base) + (0x400))
#define SOC_ACTRL_IOMCU_MEM_CTRL0_ADDR(base) ((base) + (0x800))
#define SOC_ACTRL_IOMCU_MEM_CTRL1_ADDR(base) ((base) + (0x804))
#define SOC_ACTRL_IOMCU_MEM_CTRL3_ADDR(base) ((base) + (0x808))
#define SOC_ACTRL_AO_MEM_CTRL0_ADDR(base) ((base) + (0x80C))
#define SOC_ACTRL_AO_MEM_CTRL1_ADDR(base) ((base) + (0x810))
#define SOC_ACTRL_AO_MEM_CTRL2_ADDR(base) ((base) + (0x814))
#define SOC_ACTRL_AO_MEM_CTRL3_ADDR(base) ((base) + (0x818))
#define SOC_ACTRL_AO_MEM_CTRL4_ADDR(base) ((base) + (0x81C))
#define SOC_ACTRL_AO_MEM_CTRL5_ADDR(base) ((base) + (0x820))
#define SOC_ACTRL_AO_MEM_CTRL6_ADDR(base) ((base) + (0x824))
#define SOC_ACTRL_AO_MEM_CTRL7_ADDR(base) ((base) + (0x828))
#define SOC_ACTRL_AO_MEM_CTRL8_ADDR(base) ((base) + (0x82C))
#define SOC_ACTRL_AO_MEM_CTRL9_ADDR(base) ((base) + (0x830))
#define SOC_ACTRL_AO_MEM_STATUS_ADDR(base) ((base) + (0x834))
#define SOC_ACTRL_SEC_CTRL0_ADDR(base) ((base) + (0x838))
#define SOC_ACTRL_SEC_STATUS0_ADDR(base) ((base) + (0x844))
#define SOC_ACTRL_AO_MEM_CTRL10_ADDR(base) ((base) + (0x848))
#define SOC_ACTRL_BITMSK_SEC_CTRL1_ADDR(base) ((base) + (0x84C))
#define SOC_ACTRL_BITMSK_SEC_CTRL2_ADDR(base) ((base) + (0x850))
#define SOC_ACTRL_BITMSK_SEC_CTRL3_ADDR(base) ((base) + (0x854))
#define SOC_ACTRL_BITMSK_SEC_CTRL4_ADDR(base) ((base) + (0x858))
#define SOC_ACTRL_SEC_STATUS1_ADDR(base) ((base) + (0x85C))
#define SOC_ACTRL_SEC_STATUS2_ADDR(base) ((base) + (0x860))
#define SOC_ACTRL_BITMSK_SEC_CTRL0_ADDR(base) ((base) + (0xC00))
#define SOC_ACTRL_ISO_EN_GROUP_SEC_ADDR(base) ((base) + (0xF00))
#define SOC_ACTRL_MTCMOS_EN_GROUP_SEC_ADDR(base) ((base) + (0xF04))
#define SOC_ACTRL_MTCMOS_ACK_GROUP_SEC_ADDR(base) ((base) + (0xF08))
#define SOC_ACTRL_HIEPS_DCU_EN_ADDR(base) ((base) + (0xF0C))
#define SOC_ACTRL_HIEPS_CTRL_SEC_ADDR(base) ((base) + (0xF10))
#define SOC_ACTRL_HIEPS_STATE_COUNTER_INC_REG_ADDR(base) ((base) + (0xF14))
#define SOC_ACTRL_HIEPS_STATE_COUNTER_ADDR(base) ((base) + (0xF18))
#define SOC_ACTRL_HIEPS_CTRL0_ADDR(base) ((base) + (0xF1C))
#define SOC_ACTRL_HIEPS_CTRL1_ADDR(base) ((base) + (0xF20))
#define SOC_ACTRL_HIEPS_CTRL2_ADDR(base) ((base) + (0xF24))
#define SOC_ACTRL_HIEPS_CTRL3_ADDR(base) ((base) + (0xF28))
#define SOC_ACTRL_HIEPS_CTRL4_ADDR(base) ((base) + (0xF2C))
#define SOC_ACTRL_HIEPS_CTRL5_ADDR(base) ((base) + (0xF30))
#define SOC_ACTRL_HIEPS_CTRL6_ADDR(base) ((base) + (0xF34))
#define SOC_ACTRL_HIEPS_CTRL7_ADDR(base) ((base) + (0xF38))
#define SOC_ACTRL_HIEPS_CTRL8_ADDR(base) ((base) + (0xF3C))
#define SOC_ACTRL_HIEPS_CTRL9_ADDR(base) ((base) + (0xF40))
#define SOC_ACTRL_HIEPS_CTRL10_ADDR(base) ((base) + (0xF44))
#define SOC_ACTRL_HIEPS_CTRL11_ADDR(base) ((base) + (0xF48))
#define SOC_ACTRL_HIEPS_CTRL12_ADDR(base) ((base) + (0xF4C))
#define SOC_ACTRL_HIEPS_CTRL13_ADDR(base) ((base) + (0xF50))
#define SOC_ACTRL_HIEPS_CTRL14_ADDR(base) ((base) + (0xF54))
#define SOC_ACTRL_HIEPS_CTRL15_ADDR(base) ((base) + (0xF58))
#define SOC_ACTRL_ACTRL2EPS_REG0_ADDR(base) ((base) + (0xF5C))
#define SOC_ACTRL_ACTRL2EPS_REG1_ADDR(base) ((base) + (0xF60))
#define SOC_ACTRL_ACTRL2EPS_REG2_ADDR(base) ((base) + (0xF64))
#define SOC_ACTRL_ACTRL2EPS_REG3_ADDR(base) ((base) + (0xF68))
#define SOC_ACTRL_ACTRL2EPS_REG4_ADDR(base) ((base) + (0xF6C))
#define SOC_ACTRL_EPS2ACTRL_REG0_ADDR(base) ((base) + (0xF70))
#define SOC_ACTRL_EPS2ACTRL_REG1_ADDR(base) ((base) + (0xF74))
#define SOC_ACTRL_EPS2ACTRL_REG2_ADDR(base) ((base) + (0xF78))
#define SOC_ACTRL_EPS2ACTRL_REG3_ADDR(base) ((base) + (0xF7C))
#define SOC_ACTRL_EPS2ACTRL_REG4_ADDR(base) ((base) + (0xF80))
#define SOC_ACTRL_HIEPS_MEM_CTRL_ADDR(base) ((base) + (0xF84))
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int mphy_top_iso_en : 1;
        unsigned int syscache1_iso_en : 1;
        unsigned int ivp32dsp1_iso_en : 1;
        unsigned int pcie0_iso_en : 1;
        unsigned int pcie1_iso_en : 1;
        unsigned int mphy_iso_en : 1;
        unsigned int peri_fix_iso_en : 1;
        unsigned int syscache0_iso_en : 1;
        unsigned int gpu_top_iso_en : 1;
        unsigned int reserved : 7;
        unsigned int iso_en_group0_peri_msk : 16;
    } reg;
} SOC_ACTRL_ISO_EN_GROUP0_PERI_UNION;
#endif
#define SOC_ACTRL_ISO_EN_GROUP0_PERI_mphy_top_iso_en_START (0)
#define SOC_ACTRL_ISO_EN_GROUP0_PERI_mphy_top_iso_en_END (0)
#define SOC_ACTRL_ISO_EN_GROUP0_PERI_syscache1_iso_en_START (1)
#define SOC_ACTRL_ISO_EN_GROUP0_PERI_syscache1_iso_en_END (1)
#define SOC_ACTRL_ISO_EN_GROUP0_PERI_ivp32dsp1_iso_en_START (2)
#define SOC_ACTRL_ISO_EN_GROUP0_PERI_ivp32dsp1_iso_en_END (2)
#define SOC_ACTRL_ISO_EN_GROUP0_PERI_pcie0_iso_en_START (3)
#define SOC_ACTRL_ISO_EN_GROUP0_PERI_pcie0_iso_en_END (3)
#define SOC_ACTRL_ISO_EN_GROUP0_PERI_pcie1_iso_en_START (4)
#define SOC_ACTRL_ISO_EN_GROUP0_PERI_pcie1_iso_en_END (4)
#define SOC_ACTRL_ISO_EN_GROUP0_PERI_mphy_iso_en_START (5)
#define SOC_ACTRL_ISO_EN_GROUP0_PERI_mphy_iso_en_END (5)
#define SOC_ACTRL_ISO_EN_GROUP0_PERI_peri_fix_iso_en_START (6)
#define SOC_ACTRL_ISO_EN_GROUP0_PERI_peri_fix_iso_en_END (6)
#define SOC_ACTRL_ISO_EN_GROUP0_PERI_syscache0_iso_en_START (7)
#define SOC_ACTRL_ISO_EN_GROUP0_PERI_syscache0_iso_en_END (7)
#define SOC_ACTRL_ISO_EN_GROUP0_PERI_gpu_top_iso_en_START (8)
#define SOC_ACTRL_ISO_EN_GROUP0_PERI_gpu_top_iso_en_END (8)
#define SOC_ACTRL_ISO_EN_GROUP0_PERI_iso_en_group0_peri_msk_START (16)
#define SOC_ACTRL_ISO_EN_GROUP0_PERI_iso_en_group0_peri_msk_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int nputop_iso_en : 1;
        unsigned int npu_tiny_iso_en : 1;
        unsigned int npu_tscpu_iso_en : 1;
        unsigned int npu_aicore0_iso_en : 1;
        unsigned int npu_aicore1_iso_en : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int media1_iso_en : 1;
        unsigned int reserved_3 : 1;
        unsigned int isp_top_iso_en : 1;
        unsigned int isp_cap_iso_en : 1;
        unsigned int isp_rtvid_iso_en : 1;
        unsigned int isp_cpu_iso_en : 1;
        unsigned int vdec_iso_en : 1;
        unsigned int venc_iso_en : 1;
        unsigned int iso_en_group1_peri_msk : 16;
    } reg;
} SOC_ACTRL_ISO_EN_GROUP1_PERI_UNION;
#endif
#define SOC_ACTRL_ISO_EN_GROUP1_PERI_nputop_iso_en_START (1)
#define SOC_ACTRL_ISO_EN_GROUP1_PERI_nputop_iso_en_END (1)
#define SOC_ACTRL_ISO_EN_GROUP1_PERI_npu_tiny_iso_en_START (2)
#define SOC_ACTRL_ISO_EN_GROUP1_PERI_npu_tiny_iso_en_END (2)
#define SOC_ACTRL_ISO_EN_GROUP1_PERI_npu_tscpu_iso_en_START (3)
#define SOC_ACTRL_ISO_EN_GROUP1_PERI_npu_tscpu_iso_en_END (3)
#define SOC_ACTRL_ISO_EN_GROUP1_PERI_npu_aicore0_iso_en_START (4)
#define SOC_ACTRL_ISO_EN_GROUP1_PERI_npu_aicore0_iso_en_END (4)
#define SOC_ACTRL_ISO_EN_GROUP1_PERI_npu_aicore1_iso_en_START (5)
#define SOC_ACTRL_ISO_EN_GROUP1_PERI_npu_aicore1_iso_en_END (5)
#define SOC_ACTRL_ISO_EN_GROUP1_PERI_media1_iso_en_START (8)
#define SOC_ACTRL_ISO_EN_GROUP1_PERI_media1_iso_en_END (8)
#define SOC_ACTRL_ISO_EN_GROUP1_PERI_isp_top_iso_en_START (10)
#define SOC_ACTRL_ISO_EN_GROUP1_PERI_isp_top_iso_en_END (10)
#define SOC_ACTRL_ISO_EN_GROUP1_PERI_isp_cap_iso_en_START (11)
#define SOC_ACTRL_ISO_EN_GROUP1_PERI_isp_cap_iso_en_END (11)
#define SOC_ACTRL_ISO_EN_GROUP1_PERI_isp_rtvid_iso_en_START (12)
#define SOC_ACTRL_ISO_EN_GROUP1_PERI_isp_rtvid_iso_en_END (12)
#define SOC_ACTRL_ISO_EN_GROUP1_PERI_isp_cpu_iso_en_START (13)
#define SOC_ACTRL_ISO_EN_GROUP1_PERI_isp_cpu_iso_en_END (13)
#define SOC_ACTRL_ISO_EN_GROUP1_PERI_vdec_iso_en_START (14)
#define SOC_ACTRL_ISO_EN_GROUP1_PERI_vdec_iso_en_END (14)
#define SOC_ACTRL_ISO_EN_GROUP1_PERI_venc_iso_en_START (15)
#define SOC_ACTRL_ISO_EN_GROUP1_PERI_venc_iso_en_END (15)
#define SOC_ACTRL_ISO_EN_GROUP1_PERI_iso_en_group1_peri_msk_START (16)
#define SOC_ACTRL_ISO_EN_GROUP1_PERI_iso_en_group1_peri_msk_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ivp32dsp0_iso_en : 1;
        unsigned int arpp_iso_en : 1;
        unsigned int ipp_iso_en : 1;
        unsigned int dmc_iso_en : 1;
        unsigned int modem4g_iso_en : 1;
        unsigned int modem5gbbp_iso_en : 1;
        unsigned int modem5gsoc_iso_en : 1;
        unsigned int bba_iso_en : 1;
        unsigned int g3d_iso_en : 1;
        unsigned int arpp_s0_iso_en : 1;
        unsigned int arpp_s1_iso_en : 1;
        unsigned int ipp_s0_iso_en : 1;
        unsigned int ipp_s1_iso_en : 1;
        unsigned int bbamtcmos_iso_en : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int iso_en_group2_peri_msk : 16;
    } reg;
} SOC_ACTRL_ISO_EN_GROUP2_PERI_UNION;
#endif
#define SOC_ACTRL_ISO_EN_GROUP2_PERI_ivp32dsp0_iso_en_START (0)
#define SOC_ACTRL_ISO_EN_GROUP2_PERI_ivp32dsp0_iso_en_END (0)
#define SOC_ACTRL_ISO_EN_GROUP2_PERI_arpp_iso_en_START (1)
#define SOC_ACTRL_ISO_EN_GROUP2_PERI_arpp_iso_en_END (1)
#define SOC_ACTRL_ISO_EN_GROUP2_PERI_ipp_iso_en_START (2)
#define SOC_ACTRL_ISO_EN_GROUP2_PERI_ipp_iso_en_END (2)
#define SOC_ACTRL_ISO_EN_GROUP2_PERI_dmc_iso_en_START (3)
#define SOC_ACTRL_ISO_EN_GROUP2_PERI_dmc_iso_en_END (3)
#define SOC_ACTRL_ISO_EN_GROUP2_PERI_modem4g_iso_en_START (4)
#define SOC_ACTRL_ISO_EN_GROUP2_PERI_modem4g_iso_en_END (4)
#define SOC_ACTRL_ISO_EN_GROUP2_PERI_modem5gbbp_iso_en_START (5)
#define SOC_ACTRL_ISO_EN_GROUP2_PERI_modem5gbbp_iso_en_END (5)
#define SOC_ACTRL_ISO_EN_GROUP2_PERI_modem5gsoc_iso_en_START (6)
#define SOC_ACTRL_ISO_EN_GROUP2_PERI_modem5gsoc_iso_en_END (6)
#define SOC_ACTRL_ISO_EN_GROUP2_PERI_bba_iso_en_START (7)
#define SOC_ACTRL_ISO_EN_GROUP2_PERI_bba_iso_en_END (7)
#define SOC_ACTRL_ISO_EN_GROUP2_PERI_g3d_iso_en_START (8)
#define SOC_ACTRL_ISO_EN_GROUP2_PERI_g3d_iso_en_END (8)
#define SOC_ACTRL_ISO_EN_GROUP2_PERI_arpp_s0_iso_en_START (9)
#define SOC_ACTRL_ISO_EN_GROUP2_PERI_arpp_s0_iso_en_END (9)
#define SOC_ACTRL_ISO_EN_GROUP2_PERI_arpp_s1_iso_en_START (10)
#define SOC_ACTRL_ISO_EN_GROUP2_PERI_arpp_s1_iso_en_END (10)
#define SOC_ACTRL_ISO_EN_GROUP2_PERI_ipp_s0_iso_en_START (11)
#define SOC_ACTRL_ISO_EN_GROUP2_PERI_ipp_s0_iso_en_END (11)
#define SOC_ACTRL_ISO_EN_GROUP2_PERI_ipp_s1_iso_en_START (12)
#define SOC_ACTRL_ISO_EN_GROUP2_PERI_ipp_s1_iso_en_END (12)
#define SOC_ACTRL_ISO_EN_GROUP2_PERI_bbamtcmos_iso_en_START (13)
#define SOC_ACTRL_ISO_EN_GROUP2_PERI_bbamtcmos_iso_en_END (13)
#define SOC_ACTRL_ISO_EN_GROUP2_PERI_iso_en_group2_peri_msk_START (16)
#define SOC_ACTRL_ISO_EN_GROUP2_PERI_iso_en_group2_peri_msk_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int iomcu_iso_en : 1;
        unsigned int audio_iso_en : 1;
        unsigned int swing_iso_en : 1;
        unsigned int reserved : 13;
        unsigned int iso_en_ao_msk : 16;
    } reg;
} SOC_ACTRL_ISO_EN_AO_UNION;
#endif
#define SOC_ACTRL_ISO_EN_AO_iomcu_iso_en_START (0)
#define SOC_ACTRL_ISO_EN_AO_iomcu_iso_en_END (0)
#define SOC_ACTRL_ISO_EN_AO_audio_iso_en_START (1)
#define SOC_ACTRL_ISO_EN_AO_audio_iso_en_END (1)
#define SOC_ACTRL_ISO_EN_AO_swing_iso_en_START (2)
#define SOC_ACTRL_ISO_EN_AO_swing_iso_en_END (2)
#define SOC_ACTRL_ISO_EN_AO_iso_en_ao_msk_START (16)
#define SOC_ACTRL_ISO_EN_AO_iso_en_ao_msk_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int ivp32dsp1_mtcmos_en : 1;
        unsigned int syscache1_mtcmos_en : 1;
        unsigned int gpu_top_mtcmos_en : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int reserved_8 : 1;
        unsigned int syscache0_mtcmos_en : 1;
        unsigned int media1_mtcmos_en : 1;
        unsigned int reserved_9 : 1;
        unsigned int isp_top_mtcmos_en : 1;
        unsigned int mtcmos_en_group0_peri_msk : 16;
    } reg;
} SOC_ACTRL_MTCMOS_EN_GROUP0_PERI_UNION;
#endif
#define SOC_ACTRL_MTCMOS_EN_GROUP0_PERI_ivp32dsp1_mtcmos_en_START (1)
#define SOC_ACTRL_MTCMOS_EN_GROUP0_PERI_ivp32dsp1_mtcmos_en_END (1)
#define SOC_ACTRL_MTCMOS_EN_GROUP0_PERI_syscache1_mtcmos_en_START (2)
#define SOC_ACTRL_MTCMOS_EN_GROUP0_PERI_syscache1_mtcmos_en_END (2)
#define SOC_ACTRL_MTCMOS_EN_GROUP0_PERI_gpu_top_mtcmos_en_START (3)
#define SOC_ACTRL_MTCMOS_EN_GROUP0_PERI_gpu_top_mtcmos_en_END (3)
#define SOC_ACTRL_MTCMOS_EN_GROUP0_PERI_syscache0_mtcmos_en_START (12)
#define SOC_ACTRL_MTCMOS_EN_GROUP0_PERI_syscache0_mtcmos_en_END (12)
#define SOC_ACTRL_MTCMOS_EN_GROUP0_PERI_media1_mtcmos_en_START (13)
#define SOC_ACTRL_MTCMOS_EN_GROUP0_PERI_media1_mtcmos_en_END (13)
#define SOC_ACTRL_MTCMOS_EN_GROUP0_PERI_isp_top_mtcmos_en_START (15)
#define SOC_ACTRL_MTCMOS_EN_GROUP0_PERI_isp_top_mtcmos_en_END (15)
#define SOC_ACTRL_MTCMOS_EN_GROUP0_PERI_mtcmos_en_group0_peri_msk_START (16)
#define SOC_ACTRL_MTCMOS_EN_GROUP0_PERI_mtcmos_en_group0_peri_msk_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int isp_cap_mtcmos_en : 1;
        unsigned int isp_rtvid_mtcmos_en : 1;
        unsigned int isp_cpu_mtcmos_en : 1;
        unsigned int vdec_mtcmos_en : 1;
        unsigned int venc_mtcmos_en : 1;
        unsigned int ivp32dsp0_mtcmos_en : 1;
        unsigned int arpp_mtcmos_en : 1;
        unsigned int ipp_mtcmos_en : 1;
        unsigned int aicore0_mtcmos_en : 1;
        unsigned int aicore1_mtcmos_en : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int npu_tscpu_mtcmos_en : 1;
        unsigned int npu_tiny_mtcmos_en : 1;
        unsigned int arpp_s0_mtcmos_en : 1;
        unsigned int arpp_s1_mtcmos_en : 1;
        unsigned int mtcmos_en_group1_peri_msk : 16;
    } reg;
} SOC_ACTRL_MTCMOS_EN_GROUP1_PERI_UNION;
#endif
#define SOC_ACTRL_MTCMOS_EN_GROUP1_PERI_isp_cap_mtcmos_en_START (0)
#define SOC_ACTRL_MTCMOS_EN_GROUP1_PERI_isp_cap_mtcmos_en_END (0)
#define SOC_ACTRL_MTCMOS_EN_GROUP1_PERI_isp_rtvid_mtcmos_en_START (1)
#define SOC_ACTRL_MTCMOS_EN_GROUP1_PERI_isp_rtvid_mtcmos_en_END (1)
#define SOC_ACTRL_MTCMOS_EN_GROUP1_PERI_isp_cpu_mtcmos_en_START (2)
#define SOC_ACTRL_MTCMOS_EN_GROUP1_PERI_isp_cpu_mtcmos_en_END (2)
#define SOC_ACTRL_MTCMOS_EN_GROUP1_PERI_vdec_mtcmos_en_START (3)
#define SOC_ACTRL_MTCMOS_EN_GROUP1_PERI_vdec_mtcmos_en_END (3)
#define SOC_ACTRL_MTCMOS_EN_GROUP1_PERI_venc_mtcmos_en_START (4)
#define SOC_ACTRL_MTCMOS_EN_GROUP1_PERI_venc_mtcmos_en_END (4)
#define SOC_ACTRL_MTCMOS_EN_GROUP1_PERI_ivp32dsp0_mtcmos_en_START (5)
#define SOC_ACTRL_MTCMOS_EN_GROUP1_PERI_ivp32dsp0_mtcmos_en_END (5)
#define SOC_ACTRL_MTCMOS_EN_GROUP1_PERI_arpp_mtcmos_en_START (6)
#define SOC_ACTRL_MTCMOS_EN_GROUP1_PERI_arpp_mtcmos_en_END (6)
#define SOC_ACTRL_MTCMOS_EN_GROUP1_PERI_ipp_mtcmos_en_START (7)
#define SOC_ACTRL_MTCMOS_EN_GROUP1_PERI_ipp_mtcmos_en_END (7)
#define SOC_ACTRL_MTCMOS_EN_GROUP1_PERI_aicore0_mtcmos_en_START (8)
#define SOC_ACTRL_MTCMOS_EN_GROUP1_PERI_aicore0_mtcmos_en_END (8)
#define SOC_ACTRL_MTCMOS_EN_GROUP1_PERI_aicore1_mtcmos_en_START (9)
#define SOC_ACTRL_MTCMOS_EN_GROUP1_PERI_aicore1_mtcmos_en_END (9)
#define SOC_ACTRL_MTCMOS_EN_GROUP1_PERI_npu_tscpu_mtcmos_en_START (12)
#define SOC_ACTRL_MTCMOS_EN_GROUP1_PERI_npu_tscpu_mtcmos_en_END (12)
#define SOC_ACTRL_MTCMOS_EN_GROUP1_PERI_npu_tiny_mtcmos_en_START (13)
#define SOC_ACTRL_MTCMOS_EN_GROUP1_PERI_npu_tiny_mtcmos_en_END (13)
#define SOC_ACTRL_MTCMOS_EN_GROUP1_PERI_arpp_s0_mtcmos_en_START (14)
#define SOC_ACTRL_MTCMOS_EN_GROUP1_PERI_arpp_s0_mtcmos_en_END (14)
#define SOC_ACTRL_MTCMOS_EN_GROUP1_PERI_arpp_s1_mtcmos_en_START (15)
#define SOC_ACTRL_MTCMOS_EN_GROUP1_PERI_arpp_s1_mtcmos_en_END (15)
#define SOC_ACTRL_MTCMOS_EN_GROUP1_PERI_mtcmos_en_group1_peri_msk_START (16)
#define SOC_ACTRL_MTCMOS_EN_GROUP1_PERI_mtcmos_en_group1_peri_msk_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ipp_s0_mtcmos_en : 1;
        unsigned int ipp_s1_mtcmos_en : 1;
        unsigned int modem_bbic_mtcmos_en : 1;
        unsigned int reserved : 13;
        unsigned int mtcmos_en_group2_peri_msk : 16;
    } reg;
} SOC_ACTRL_MTCMOS_EN_GROUP2_PERI_UNION;
#endif
#define SOC_ACTRL_MTCMOS_EN_GROUP2_PERI_ipp_s0_mtcmos_en_START (0)
#define SOC_ACTRL_MTCMOS_EN_GROUP2_PERI_ipp_s0_mtcmos_en_END (0)
#define SOC_ACTRL_MTCMOS_EN_GROUP2_PERI_ipp_s1_mtcmos_en_START (1)
#define SOC_ACTRL_MTCMOS_EN_GROUP2_PERI_ipp_s1_mtcmos_en_END (1)
#define SOC_ACTRL_MTCMOS_EN_GROUP2_PERI_modem_bbic_mtcmos_en_START (2)
#define SOC_ACTRL_MTCMOS_EN_GROUP2_PERI_modem_bbic_mtcmos_en_END (2)
#define SOC_ACTRL_MTCMOS_EN_GROUP2_PERI_mtcmos_en_group2_peri_msk_START (16)
#define SOC_ACTRL_MTCMOS_EN_GROUP2_PERI_mtcmos_en_group2_peri_msk_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int iomcu_mtcmos_en : 1;
        unsigned int audio_mtcmos_en : 1;
        unsigned int swing_mtcmos_en : 1;
        unsigned int reserved : 13;
        unsigned int mtcmos_en_ao_msk : 16;
    } reg;
} SOC_ACTRL_MTCMOS_EN_AO_UNION;
#endif
#define SOC_ACTRL_MTCMOS_EN_AO_iomcu_mtcmos_en_START (0)
#define SOC_ACTRL_MTCMOS_EN_AO_iomcu_mtcmos_en_END (0)
#define SOC_ACTRL_MTCMOS_EN_AO_audio_mtcmos_en_START (1)
#define SOC_ACTRL_MTCMOS_EN_AO_audio_mtcmos_en_END (1)
#define SOC_ACTRL_MTCMOS_EN_AO_swing_mtcmos_en_START (2)
#define SOC_ACTRL_MTCMOS_EN_AO_swing_mtcmos_en_END (2)
#define SOC_ACTRL_MTCMOS_EN_AO_mtcmos_en_ao_msk_START (16)
#define SOC_ACTRL_MTCMOS_EN_AO_mtcmos_en_ao_msk_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int ivp32dsp1_mtcmos_ack : 1;
        unsigned int syscache1_mtcmos_ack : 1;
        unsigned int gpu_top_mtcmos_ack : 1;
        unsigned int reserved_1 : 8;
        unsigned int syscache0_mtcmos_ack : 1;
        unsigned int media1_mtcmos_ack : 1;
        unsigned int reserved_2 : 1;
        unsigned int isp_top_mtcmos_ack : 1;
        unsigned int isp_cap_mtcmos_ack : 1;
        unsigned int isp_rtvid_mtcmos_ack : 1;
        unsigned int isp_cpu_mtcmos_ack : 1;
        unsigned int vdec_mtcmos_ack : 1;
        unsigned int venc_mtcmos_ack : 1;
        unsigned int ivp32dsp0_mtcmos_ack : 1;
        unsigned int arpp_mtcmos_ack : 1;
        unsigned int ipp_mtcmos_ack : 1;
        unsigned int aicore0_mtcmos_ack : 1;
        unsigned int aicore1_mtcmos_ack : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int npu_tscpu_mtcmos_ack : 1;
        unsigned int npu_tiny_mtcmos_ack : 1;
        unsigned int arpp_s0_mtcmos_ack : 1;
        unsigned int arpp_s1_mtcmos_ack : 1;
    } reg;
} SOC_ACTRL_MTCMOS_ACK0_PERI_UNION;
#endif
#define SOC_ACTRL_MTCMOS_ACK0_PERI_ivp32dsp1_mtcmos_ack_START (1)
#define SOC_ACTRL_MTCMOS_ACK0_PERI_ivp32dsp1_mtcmos_ack_END (1)
#define SOC_ACTRL_MTCMOS_ACK0_PERI_syscache1_mtcmos_ack_START (2)
#define SOC_ACTRL_MTCMOS_ACK0_PERI_syscache1_mtcmos_ack_END (2)
#define SOC_ACTRL_MTCMOS_ACK0_PERI_gpu_top_mtcmos_ack_START (3)
#define SOC_ACTRL_MTCMOS_ACK0_PERI_gpu_top_mtcmos_ack_END (3)
#define SOC_ACTRL_MTCMOS_ACK0_PERI_syscache0_mtcmos_ack_START (12)
#define SOC_ACTRL_MTCMOS_ACK0_PERI_syscache0_mtcmos_ack_END (12)
#define SOC_ACTRL_MTCMOS_ACK0_PERI_media1_mtcmos_ack_START (13)
#define SOC_ACTRL_MTCMOS_ACK0_PERI_media1_mtcmos_ack_END (13)
#define SOC_ACTRL_MTCMOS_ACK0_PERI_isp_top_mtcmos_ack_START (15)
#define SOC_ACTRL_MTCMOS_ACK0_PERI_isp_top_mtcmos_ack_END (15)
#define SOC_ACTRL_MTCMOS_ACK0_PERI_isp_cap_mtcmos_ack_START (16)
#define SOC_ACTRL_MTCMOS_ACK0_PERI_isp_cap_mtcmos_ack_END (16)
#define SOC_ACTRL_MTCMOS_ACK0_PERI_isp_rtvid_mtcmos_ack_START (17)
#define SOC_ACTRL_MTCMOS_ACK0_PERI_isp_rtvid_mtcmos_ack_END (17)
#define SOC_ACTRL_MTCMOS_ACK0_PERI_isp_cpu_mtcmos_ack_START (18)
#define SOC_ACTRL_MTCMOS_ACK0_PERI_isp_cpu_mtcmos_ack_END (18)
#define SOC_ACTRL_MTCMOS_ACK0_PERI_vdec_mtcmos_ack_START (19)
#define SOC_ACTRL_MTCMOS_ACK0_PERI_vdec_mtcmos_ack_END (19)
#define SOC_ACTRL_MTCMOS_ACK0_PERI_venc_mtcmos_ack_START (20)
#define SOC_ACTRL_MTCMOS_ACK0_PERI_venc_mtcmos_ack_END (20)
#define SOC_ACTRL_MTCMOS_ACK0_PERI_ivp32dsp0_mtcmos_ack_START (21)
#define SOC_ACTRL_MTCMOS_ACK0_PERI_ivp32dsp0_mtcmos_ack_END (21)
#define SOC_ACTRL_MTCMOS_ACK0_PERI_arpp_mtcmos_ack_START (22)
#define SOC_ACTRL_MTCMOS_ACK0_PERI_arpp_mtcmos_ack_END (22)
#define SOC_ACTRL_MTCMOS_ACK0_PERI_ipp_mtcmos_ack_START (23)
#define SOC_ACTRL_MTCMOS_ACK0_PERI_ipp_mtcmos_ack_END (23)
#define SOC_ACTRL_MTCMOS_ACK0_PERI_aicore0_mtcmos_ack_START (24)
#define SOC_ACTRL_MTCMOS_ACK0_PERI_aicore0_mtcmos_ack_END (24)
#define SOC_ACTRL_MTCMOS_ACK0_PERI_aicore1_mtcmos_ack_START (25)
#define SOC_ACTRL_MTCMOS_ACK0_PERI_aicore1_mtcmos_ack_END (25)
#define SOC_ACTRL_MTCMOS_ACK0_PERI_npu_tscpu_mtcmos_ack_START (28)
#define SOC_ACTRL_MTCMOS_ACK0_PERI_npu_tscpu_mtcmos_ack_END (28)
#define SOC_ACTRL_MTCMOS_ACK0_PERI_npu_tiny_mtcmos_ack_START (29)
#define SOC_ACTRL_MTCMOS_ACK0_PERI_npu_tiny_mtcmos_ack_END (29)
#define SOC_ACTRL_MTCMOS_ACK0_PERI_arpp_s0_mtcmos_ack_START (30)
#define SOC_ACTRL_MTCMOS_ACK0_PERI_arpp_s0_mtcmos_ack_END (30)
#define SOC_ACTRL_MTCMOS_ACK0_PERI_arpp_s1_mtcmos_ack_START (31)
#define SOC_ACTRL_MTCMOS_ACK0_PERI_arpp_s1_mtcmos_ack_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ipp_s0_mtcmos_ack : 1;
        unsigned int ipp_s1_mtcmos_ack : 1;
        unsigned int modem_bbic_mtcmos_ack : 1;
        unsigned int reserved : 29;
    } reg;
} SOC_ACTRL_MTCMOS_ACK1_PERI_UNION;
#endif
#define SOC_ACTRL_MTCMOS_ACK1_PERI_ipp_s0_mtcmos_ack_START (0)
#define SOC_ACTRL_MTCMOS_ACK1_PERI_ipp_s0_mtcmos_ack_END (0)
#define SOC_ACTRL_MTCMOS_ACK1_PERI_ipp_s1_mtcmos_ack_START (1)
#define SOC_ACTRL_MTCMOS_ACK1_PERI_ipp_s1_mtcmos_ack_END (1)
#define SOC_ACTRL_MTCMOS_ACK1_PERI_modem_bbic_mtcmos_ack_START (2)
#define SOC_ACTRL_MTCMOS_ACK1_PERI_modem_bbic_mtcmos_ack_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int iomcu_mtcmos_ack : 1;
        unsigned int audio_mtcmos_ack : 1;
        unsigned int swing_mtcmos_ack : 1;
        unsigned int reserved : 29;
    } reg;
} SOC_ACTRL_MTCMOS_ACK_AO_UNION;
#endif
#define SOC_ACTRL_MTCMOS_ACK_AO_iomcu_mtcmos_ack_START (0)
#define SOC_ACTRL_MTCMOS_ACK_AO_iomcu_mtcmos_ack_END (0)
#define SOC_ACTRL_MTCMOS_ACK_AO_audio_mtcmos_ack_START (1)
#define SOC_ACTRL_MTCMOS_ACK_AO_audio_mtcmos_ack_END (1)
#define SOC_ACTRL_MTCMOS_ACK_AO_swing_mtcmos_ack_START (2)
#define SOC_ACTRL_MTCMOS_ACK_AO_swing_mtcmos_ack_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bisr_dbg_sel_ao : 4;
        unsigned int intr_repair_err_msk_ao : 1;
        unsigned int bisr_rom_mem_ctrl : 8;
        unsigned int reserved : 3;
        unsigned int nonsec_ctrl0_msk : 16;
    } reg;
} SOC_ACTRL_BITMSK_NONSEC_CTRL0_UNION;
#endif
#define SOC_ACTRL_BITMSK_NONSEC_CTRL0_bisr_dbg_sel_ao_START (0)
#define SOC_ACTRL_BITMSK_NONSEC_CTRL0_bisr_dbg_sel_ao_END (3)
#define SOC_ACTRL_BITMSK_NONSEC_CTRL0_intr_repair_err_msk_ao_START (4)
#define SOC_ACTRL_BITMSK_NONSEC_CTRL0_intr_repair_err_msk_ao_END (4)
#define SOC_ACTRL_BITMSK_NONSEC_CTRL0_bisr_rom_mem_ctrl_START (5)
#define SOC_ACTRL_BITMSK_NONSEC_CTRL0_bisr_rom_mem_ctrl_END (12)
#define SOC_ACTRL_BITMSK_NONSEC_CTRL0_nonsec_ctrl0_msk_START (16)
#define SOC_ACTRL_BITMSK_NONSEC_CTRL0_nonsec_ctrl0_msk_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 10;
        unsigned int repair_start_point_err_ao : 1;
        unsigned int pack_is0_err_ao : 1;
        unsigned int reserved_1 : 20;
    } reg;
} SOC_ACTRL_NONSEC_STATUS0_UNION;
#endif
#define SOC_ACTRL_NONSEC_STATUS0_repair_start_point_err_ao_START (10)
#define SOC_ACTRL_NONSEC_STATUS0_repair_start_point_err_ao_END (10)
#define SOC_ACTRL_NONSEC_STATUS0_pack_is0_err_ao_START (11)
#define SOC_ACTRL_NONSEC_STATUS0_pack_is0_err_ao_END (11)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bisr_spram_mem_ctrl : 26;
        unsigned int reserved : 6;
    } reg;
} SOC_ACTRL_AO_BISR_SPRAM_MEM_CTRL_UNION;
#endif
#define SOC_ACTRL_AO_BISR_SPRAM_MEM_CTRL_bisr_spram_mem_ctrl_START (0)
#define SOC_ACTRL_AO_BISR_SPRAM_MEM_CTRL_bisr_spram_mem_ctrl_END (25)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ao_spi_tpram_mem_ctrl : 16;
        unsigned int reserved : 16;
    } reg;
} SOC_ACTRL_AO_SPI_SRAM_MEM_CTRL_UNION;
#endif
#define SOC_ACTRL_AO_SPI_SRAM_MEM_CTRL_ao_spi_tpram_mem_ctrl_START (0)
#define SOC_ACTRL_AO_SPI_SRAM_MEM_CTRL_ao_spi_tpram_mem_ctrl_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bisr_mrb_busy_flag_astatus0 : 32;
    } reg;
} SOC_ACTRL_BISR_MRB_BUSY_FLAG_STATUS0_UNION;
#endif
#define SOC_ACTRL_BISR_MRB_BUSY_FLAG_STATUS0_bisr_mrb_busy_flag_astatus0_START (0)
#define SOC_ACTRL_BISR_MRB_BUSY_FLAG_STATUS0_bisr_mrb_busy_flag_astatus0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bisr_mrb_busy_flag_astatus1 : 32;
    } reg;
} SOC_ACTRL_BISR_MRB_BUSY_FLAG_STATUS1_UNION;
#endif
#define SOC_ACTRL_BISR_MRB_BUSY_FLAG_STATUS1_bisr_mrb_busy_flag_astatus1_START (0)
#define SOC_ACTRL_BISR_MRB_BUSY_FLAG_STATUS1_bisr_mrb_busy_flag_astatus1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bisr_mrb_busy_flag_astatus2 : 32;
    } reg;
} SOC_ACTRL_BISR_MRB_BUSY_FLAG_STATUS2_UNION;
#endif
#define SOC_ACTRL_BISR_MRB_BUSY_FLAG_STATUS2_bisr_mrb_busy_flag_astatus2_START (0)
#define SOC_ACTRL_BISR_MRB_BUSY_FLAG_STATUS2_bisr_mrb_busy_flag_astatus2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bisr_repair_done_status0 : 32;
    } reg;
} SOC_ACTRL_BISR_REPAIR_DONE_STATUS0_UNION;
#endif
#define SOC_ACTRL_BISR_REPAIR_DONE_STATUS0_bisr_repair_done_status0_START (0)
#define SOC_ACTRL_BISR_REPAIR_DONE_STATUS0_bisr_repair_done_status0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bisr_repair_done_status1 : 32;
    } reg;
} SOC_ACTRL_BISR_REPAIR_DONE_STATUS1_UNION;
#endif
#define SOC_ACTRL_BISR_REPAIR_DONE_STATUS1_bisr_repair_done_status1_START (0)
#define SOC_ACTRL_BISR_REPAIR_DONE_STATUS1_bisr_repair_done_status1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bisr_repair_done_status2 : 32;
    } reg;
} SOC_ACTRL_BISR_REPAIR_DONE_STATUS2_UNION;
#endif
#define SOC_ACTRL_BISR_REPAIR_DONE_STATUS2_bisr_repair_done_status2_START (0)
#define SOC_ACTRL_BISR_REPAIR_DONE_STATUS2_bisr_repair_done_status2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bisr_repair_ack_status0 : 32;
    } reg;
} SOC_ACTRL_BISR_REPAIR_ACK_STATUS0_UNION;
#endif
#define SOC_ACTRL_BISR_REPAIR_ACK_STATUS0_bisr_repair_ack_status0_START (0)
#define SOC_ACTRL_BISR_REPAIR_ACK_STATUS0_bisr_repair_ack_status0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bisr_repair_ack_status1 : 32;
    } reg;
} SOC_ACTRL_BISR_REPAIR_ACK_STATUS1_UNION;
#endif
#define SOC_ACTRL_BISR_REPAIR_ACK_STATUS1_bisr_repair_ack_status1_START (0)
#define SOC_ACTRL_BISR_REPAIR_ACK_STATUS1_bisr_repair_ack_status1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bisr_repair_ack_status2 : 32;
    } reg;
} SOC_ACTRL_BISR_REPAIR_ACK_STATUS2_UNION;
#endif
#define SOC_ACTRL_BISR_REPAIR_ACK_STATUS2_bisr_repair_ack_status2_START (0)
#define SOC_ACTRL_BISR_REPAIR_ACK_STATUS2_bisr_repair_ack_status2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bisr_dbg_info_ao_astatus0 : 32;
    } reg;
} SOC_ACTRL_BISR_DBG_INFO_AO_STAUS0_UNION;
#endif
#define SOC_ACTRL_BISR_DBG_INFO_AO_STAUS0_bisr_dbg_info_ao_astatus0_START (0)
#define SOC_ACTRL_BISR_DBG_INFO_AO_STAUS0_bisr_dbg_info_ao_astatus0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bisr_dbg_info_ao_astatus1 : 32;
    } reg;
} SOC_ACTRL_BISR_DBG_INFO_AO_STAUS1_UNION;
#endif
#define SOC_ACTRL_BISR_DBG_INFO_AO_STAUS1_bisr_dbg_info_ao_astatus1_START (0)
#define SOC_ACTRL_BISR_DBG_INFO_AO_STAUS1_bisr_dbg_info_ao_astatus1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int actrl_bitmsk_ctrl2 : 16;
        unsigned int nonsec_ctrl2_msk : 16;
    } reg;
} SOC_ACTRL_BITMSK_NONSEC_CTRL2_UNION;
#endif
#define SOC_ACTRL_BITMSK_NONSEC_CTRL2_actrl_bitmsk_ctrl2_START (0)
#define SOC_ACTRL_BITMSK_NONSEC_CTRL2_actrl_bitmsk_ctrl2_END (15)
#define SOC_ACTRL_BITMSK_NONSEC_CTRL2_nonsec_ctrl2_msk_START (16)
#define SOC_ACTRL_BITMSK_NONSEC_CTRL2_nonsec_ctrl2_msk_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int actrl_bitmsk_ctrl3 : 16;
        unsigned int nonsec_ctrl3_msk : 16;
    } reg;
} SOC_ACTRL_BITMSK_NONSEC_CTRL3_UNION;
#endif
#define SOC_ACTRL_BITMSK_NONSEC_CTRL3_actrl_bitmsk_ctrl3_START (0)
#define SOC_ACTRL_BITMSK_NONSEC_CTRL3_actrl_bitmsk_ctrl3_END (15)
#define SOC_ACTRL_BITMSK_NONSEC_CTRL3_nonsec_ctrl3_msk_START (16)
#define SOC_ACTRL_BITMSK_NONSEC_CTRL3_nonsec_ctrl3_msk_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int actrl_bitmsk_ctrl4 : 16;
        unsigned int nonsec_ctrl4_msk : 16;
    } reg;
} SOC_ACTRL_BITMSK_NONSEC_CTRL4_UNION;
#endif
#define SOC_ACTRL_BITMSK_NONSEC_CTRL4_actrl_bitmsk_ctrl4_START (0)
#define SOC_ACTRL_BITMSK_NONSEC_CTRL4_actrl_bitmsk_ctrl4_END (15)
#define SOC_ACTRL_BITMSK_NONSEC_CTRL4_nonsec_ctrl4_msk_START (16)
#define SOC_ACTRL_BITMSK_NONSEC_CTRL4_nonsec_ctrl4_msk_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int actrl_bitmsk_ctrl5 : 16;
        unsigned int nonsec_ctrl5_msk : 16;
    } reg;
} SOC_ACTRL_BITMSK_NONSEC_CTRL5_UNION;
#endif
#define SOC_ACTRL_BITMSK_NONSEC_CTRL5_actrl_bitmsk_ctrl5_START (0)
#define SOC_ACTRL_BITMSK_NONSEC_CTRL5_actrl_bitmsk_ctrl5_END (15)
#define SOC_ACTRL_BITMSK_NONSEC_CTRL5_nonsec_ctrl5_msk_START (16)
#define SOC_ACTRL_BITMSK_NONSEC_CTRL5_nonsec_ctrl5_msk_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int actrl_status1 : 32;
    } reg;
} SOC_ACTRL_NONSEC_STATUS1_UNION;
#endif
#define SOC_ACTRL_NONSEC_STATUS1_actrl_status1_START (0)
#define SOC_ACTRL_NONSEC_STATUS1_actrl_status1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int actrl_status2 : 32;
    } reg;
} SOC_ACTRL_NONSEC_STATUS2_UNION;
#endif
#define SOC_ACTRL_NONSEC_STATUS2_actrl_status2_START (0)
#define SOC_ACTRL_NONSEC_STATUS2_actrl_status2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sleep_mode_to_hsdt1_bypass : 1;
        unsigned int sc_spi3_cs : 4;
        unsigned int sc_spi5_cs : 4;
        unsigned int spi5_clr_sel : 1;
        unsigned int ufs_device_rst_n : 1;
        unsigned int bbpdrx_hresp_ctrl : 1;
        unsigned int bbpon_ahb_buffable_en : 1;
        unsigned int sc_onewire1_en : 1;
        unsigned int reserved : 2;
        unsigned int nonsec_ctrl1_msk : 16;
    } reg;
} SOC_ACTRL_BITMSK_NONSEC_CTRL1_UNION;
#endif
#define SOC_ACTRL_BITMSK_NONSEC_CTRL1_sleep_mode_to_hsdt1_bypass_START (0)
#define SOC_ACTRL_BITMSK_NONSEC_CTRL1_sleep_mode_to_hsdt1_bypass_END (0)
#define SOC_ACTRL_BITMSK_NONSEC_CTRL1_sc_spi3_cs_START (1)
#define SOC_ACTRL_BITMSK_NONSEC_CTRL1_sc_spi3_cs_END (4)
#define SOC_ACTRL_BITMSK_NONSEC_CTRL1_sc_spi5_cs_START (5)
#define SOC_ACTRL_BITMSK_NONSEC_CTRL1_sc_spi5_cs_END (8)
#define SOC_ACTRL_BITMSK_NONSEC_CTRL1_spi5_clr_sel_START (9)
#define SOC_ACTRL_BITMSK_NONSEC_CTRL1_spi5_clr_sel_END (9)
#define SOC_ACTRL_BITMSK_NONSEC_CTRL1_ufs_device_rst_n_START (10)
#define SOC_ACTRL_BITMSK_NONSEC_CTRL1_ufs_device_rst_n_END (10)
#define SOC_ACTRL_BITMSK_NONSEC_CTRL1_bbpdrx_hresp_ctrl_START (11)
#define SOC_ACTRL_BITMSK_NONSEC_CTRL1_bbpdrx_hresp_ctrl_END (11)
#define SOC_ACTRL_BITMSK_NONSEC_CTRL1_bbpon_ahb_buffable_en_START (12)
#define SOC_ACTRL_BITMSK_NONSEC_CTRL1_bbpon_ahb_buffable_en_END (12)
#define SOC_ACTRL_BITMSK_NONSEC_CTRL1_sc_onewire1_en_START (13)
#define SOC_ACTRL_BITMSK_NONSEC_CTRL1_sc_onewire1_en_END (13)
#define SOC_ACTRL_BITMSK_NONSEC_CTRL1_nonsec_ctrl1_msk_START (16)
#define SOC_ACTRL_BITMSK_NONSEC_CTRL1_nonsec_ctrl1_msk_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int actrl_iomcu_mem_sd_in0 : 16;
        unsigned int iomcu_mem_ctrl0_msk : 16;
    } reg;
} SOC_ACTRL_IOMCU_MEM_CTRL0_UNION;
#endif
#define SOC_ACTRL_IOMCU_MEM_CTRL0_actrl_iomcu_mem_sd_in0_START (0)
#define SOC_ACTRL_IOMCU_MEM_CTRL0_actrl_iomcu_mem_sd_in0_END (15)
#define SOC_ACTRL_IOMCU_MEM_CTRL0_iomcu_mem_ctrl0_msk_START (16)
#define SOC_ACTRL_IOMCU_MEM_CTRL0_iomcu_mem_ctrl0_msk_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int actrl_iomcu_mem_ds_in0 : 16;
        unsigned int iomcu_mem_ctrl1_msk : 16;
    } reg;
} SOC_ACTRL_IOMCU_MEM_CTRL1_UNION;
#endif
#define SOC_ACTRL_IOMCU_MEM_CTRL1_actrl_iomcu_mem_ds_in0_START (0)
#define SOC_ACTRL_IOMCU_MEM_CTRL1_actrl_iomcu_mem_ds_in0_END (15)
#define SOC_ACTRL_IOMCU_MEM_CTRL1_iomcu_mem_ctrl1_msk_START (16)
#define SOC_ACTRL_IOMCU_MEM_CTRL1_iomcu_mem_ctrl1_msk_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int actrl_iomcu_mem_sd_in1 : 4;
        unsigned int actrl_iomcu_mem_ds_in1 : 4;
        unsigned int actrl_mem_ctrl_reserved0 : 8;
        unsigned int iomcu_mem_ctrl3_msk : 16;
    } reg;
} SOC_ACTRL_IOMCU_MEM_CTRL3_UNION;
#endif
#define SOC_ACTRL_IOMCU_MEM_CTRL3_actrl_iomcu_mem_sd_in1_START (0)
#define SOC_ACTRL_IOMCU_MEM_CTRL3_actrl_iomcu_mem_sd_in1_END (3)
#define SOC_ACTRL_IOMCU_MEM_CTRL3_actrl_iomcu_mem_ds_in1_START (4)
#define SOC_ACTRL_IOMCU_MEM_CTRL3_actrl_iomcu_mem_ds_in1_END (7)
#define SOC_ACTRL_IOMCU_MEM_CTRL3_actrl_mem_ctrl_reserved0_START (8)
#define SOC_ACTRL_IOMCU_MEM_CTRL3_actrl_mem_ctrl_reserved0_END (15)
#define SOC_ACTRL_IOMCU_MEM_CTRL3_iomcu_mem_ctrl3_msk_START (16)
#define SOC_ACTRL_IOMCU_MEM_CTRL3_iomcu_mem_ctrl3_msk_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int actrl_asp_mem_ctrl_sd_in : 1;
        unsigned int actrl_asp_ret_mem_lp : 6;
        unsigned int actrl_asp_mfcc_ret_mem_sd_in : 1;
        unsigned int actrl_asp_mfcc_ret_mem_ds_in : 1;
        unsigned int actrl_mfcc_memlp_sd_in : 1;
        unsigned int actrl_swing_sd_in : 1;
        unsigned int actrl_ao_spi3_sd_in : 1;
        unsigned int actrl_ao_spi5_sd_in : 1;
        unsigned int actrl_mad_sd_in : 1;
        unsigned int reserved : 2;
        unsigned int ao_mem_ctrl0_msk : 16;
    } reg;
} SOC_ACTRL_AO_MEM_CTRL0_UNION;
#endif
#define SOC_ACTRL_AO_MEM_CTRL0_actrl_asp_mem_ctrl_sd_in_START (0)
#define SOC_ACTRL_AO_MEM_CTRL0_actrl_asp_mem_ctrl_sd_in_END (0)
#define SOC_ACTRL_AO_MEM_CTRL0_actrl_asp_ret_mem_lp_START (1)
#define SOC_ACTRL_AO_MEM_CTRL0_actrl_asp_ret_mem_lp_END (6)
#define SOC_ACTRL_AO_MEM_CTRL0_actrl_asp_mfcc_ret_mem_sd_in_START (7)
#define SOC_ACTRL_AO_MEM_CTRL0_actrl_asp_mfcc_ret_mem_sd_in_END (7)
#define SOC_ACTRL_AO_MEM_CTRL0_actrl_asp_mfcc_ret_mem_ds_in_START (8)
#define SOC_ACTRL_AO_MEM_CTRL0_actrl_asp_mfcc_ret_mem_ds_in_END (8)
#define SOC_ACTRL_AO_MEM_CTRL0_actrl_mfcc_memlp_sd_in_START (9)
#define SOC_ACTRL_AO_MEM_CTRL0_actrl_mfcc_memlp_sd_in_END (9)
#define SOC_ACTRL_AO_MEM_CTRL0_actrl_swing_sd_in_START (10)
#define SOC_ACTRL_AO_MEM_CTRL0_actrl_swing_sd_in_END (10)
#define SOC_ACTRL_AO_MEM_CTRL0_actrl_ao_spi3_sd_in_START (11)
#define SOC_ACTRL_AO_MEM_CTRL0_actrl_ao_spi3_sd_in_END (11)
#define SOC_ACTRL_AO_MEM_CTRL0_actrl_ao_spi5_sd_in_START (12)
#define SOC_ACTRL_AO_MEM_CTRL0_actrl_ao_spi5_sd_in_END (12)
#define SOC_ACTRL_AO_MEM_CTRL0_actrl_mad_sd_in_START (13)
#define SOC_ACTRL_AO_MEM_CTRL0_actrl_mad_sd_in_END (13)
#define SOC_ACTRL_AO_MEM_CTRL0_ao_mem_ctrl0_msk_START (16)
#define SOC_ACTRL_AO_MEM_CTRL0_ao_mem_ctrl0_msk_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int actrl_swing_lb_sd_in : 8;
        unsigned int actrl_dmc_a_ret_mem_sd_in : 1;
        unsigned int actrl_dmc_b_ret_mem_sd_in : 1;
        unsigned int actrl_dmc_c_ret_mem_sd_in : 1;
        unsigned int actrl_dmc_d_ret_mem_sd_in : 1;
        unsigned int actrl_dmc_a_ret_mem_ds_in : 1;
        unsigned int actrl_dmc_b_ret_mem_ds_in : 1;
        unsigned int actrl_dmc_c_ret_mem_ds_in : 1;
        unsigned int actrl_dmc_d_ret_mem_ds_in : 1;
        unsigned int ao_mem_ctrl1_msk : 16;
    } reg;
} SOC_ACTRL_AO_MEM_CTRL1_UNION;
#endif
#define SOC_ACTRL_AO_MEM_CTRL1_actrl_swing_lb_sd_in_START (0)
#define SOC_ACTRL_AO_MEM_CTRL1_actrl_swing_lb_sd_in_END (7)
#define SOC_ACTRL_AO_MEM_CTRL1_actrl_dmc_a_ret_mem_sd_in_START (8)
#define SOC_ACTRL_AO_MEM_CTRL1_actrl_dmc_a_ret_mem_sd_in_END (8)
#define SOC_ACTRL_AO_MEM_CTRL1_actrl_dmc_b_ret_mem_sd_in_START (9)
#define SOC_ACTRL_AO_MEM_CTRL1_actrl_dmc_b_ret_mem_sd_in_END (9)
#define SOC_ACTRL_AO_MEM_CTRL1_actrl_dmc_c_ret_mem_sd_in_START (10)
#define SOC_ACTRL_AO_MEM_CTRL1_actrl_dmc_c_ret_mem_sd_in_END (10)
#define SOC_ACTRL_AO_MEM_CTRL1_actrl_dmc_d_ret_mem_sd_in_START (11)
#define SOC_ACTRL_AO_MEM_CTRL1_actrl_dmc_d_ret_mem_sd_in_END (11)
#define SOC_ACTRL_AO_MEM_CTRL1_actrl_dmc_a_ret_mem_ds_in_START (12)
#define SOC_ACTRL_AO_MEM_CTRL1_actrl_dmc_a_ret_mem_ds_in_END (12)
#define SOC_ACTRL_AO_MEM_CTRL1_actrl_dmc_b_ret_mem_ds_in_START (13)
#define SOC_ACTRL_AO_MEM_CTRL1_actrl_dmc_b_ret_mem_ds_in_END (13)
#define SOC_ACTRL_AO_MEM_CTRL1_actrl_dmc_c_ret_mem_ds_in_START (14)
#define SOC_ACTRL_AO_MEM_CTRL1_actrl_dmc_c_ret_mem_ds_in_END (14)
#define SOC_ACTRL_AO_MEM_CTRL1_actrl_dmc_d_ret_mem_ds_in_START (15)
#define SOC_ACTRL_AO_MEM_CTRL1_actrl_dmc_d_ret_mem_ds_in_END (15)
#define SOC_ACTRL_AO_MEM_CTRL1_ao_mem_ctrl1_msk_START (16)
#define SOC_ACTRL_AO_MEM_CTRL1_ao_mem_ctrl1_msk_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int actrl_ufs_psw_ret_mem_sd_in : 1;
        unsigned int actrl_ufs_psw_ret_mem_ds_in : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int actrl_mddrc_ret_mem_sd_in : 1;
        unsigned int actrl_mddrc_ret_mem_ds_in : 1;
        unsigned int actrl_syscache0_ret_sd_in : 9;
        unsigned int reserved_2 : 1;
        unsigned int ao_mem_ctrl2_msk : 16;
    } reg;
} SOC_ACTRL_AO_MEM_CTRL2_UNION;
#endif
#define SOC_ACTRL_AO_MEM_CTRL2_actrl_ufs_psw_ret_mem_sd_in_START (0)
#define SOC_ACTRL_AO_MEM_CTRL2_actrl_ufs_psw_ret_mem_sd_in_END (0)
#define SOC_ACTRL_AO_MEM_CTRL2_actrl_ufs_psw_ret_mem_ds_in_START (1)
#define SOC_ACTRL_AO_MEM_CTRL2_actrl_ufs_psw_ret_mem_ds_in_END (1)
#define SOC_ACTRL_AO_MEM_CTRL2_actrl_mddrc_ret_mem_sd_in_START (4)
#define SOC_ACTRL_AO_MEM_CTRL2_actrl_mddrc_ret_mem_sd_in_END (4)
#define SOC_ACTRL_AO_MEM_CTRL2_actrl_mddrc_ret_mem_ds_in_START (5)
#define SOC_ACTRL_AO_MEM_CTRL2_actrl_mddrc_ret_mem_ds_in_END (5)
#define SOC_ACTRL_AO_MEM_CTRL2_actrl_syscache0_ret_sd_in_START (6)
#define SOC_ACTRL_AO_MEM_CTRL2_actrl_syscache0_ret_sd_in_END (14)
#define SOC_ACTRL_AO_MEM_CTRL2_ao_mem_ctrl2_msk_START (16)
#define SOC_ACTRL_AO_MEM_CTRL2_ao_mem_ctrl2_msk_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int actrl_syscache1_ret_sd_in : 9;
        unsigned int actrl_mdm4g_ret_mem_ctrl2 : 4;
        unsigned int reserved : 3;
        unsigned int ao_mem_ctrl3_msk : 16;
    } reg;
} SOC_ACTRL_AO_MEM_CTRL3_UNION;
#endif
#define SOC_ACTRL_AO_MEM_CTRL3_actrl_syscache1_ret_sd_in_START (0)
#define SOC_ACTRL_AO_MEM_CTRL3_actrl_syscache1_ret_sd_in_END (8)
#define SOC_ACTRL_AO_MEM_CTRL3_actrl_mdm4g_ret_mem_ctrl2_START (9)
#define SOC_ACTRL_AO_MEM_CTRL3_actrl_mdm4g_ret_mem_ctrl2_END (12)
#define SOC_ACTRL_AO_MEM_CTRL3_ao_mem_ctrl3_msk_START (16)
#define SOC_ACTRL_AO_MEM_CTRL3_ao_mem_ctrl3_msk_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int actrl_mdm4g_ret_mem_ctrl1 : 16;
        unsigned int ao_mem_ctrl4_msk : 16;
    } reg;
} SOC_ACTRL_AO_MEM_CTRL4_UNION;
#endif
#define SOC_ACTRL_AO_MEM_CTRL4_actrl_mdm4g_ret_mem_ctrl1_START (0)
#define SOC_ACTRL_AO_MEM_CTRL4_actrl_mdm4g_ret_mem_ctrl1_END (15)
#define SOC_ACTRL_AO_MEM_CTRL4_ao_mem_ctrl4_msk_START (16)
#define SOC_ACTRL_AO_MEM_CTRL4_ao_mem_ctrl4_msk_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int actrl_mdm5g_ret_mem_ctrl0 : 16;
        unsigned int ao_mem_ctrl5_msk : 16;
    } reg;
} SOC_ACTRL_AO_MEM_CTRL5_UNION;
#endif
#define SOC_ACTRL_AO_MEM_CTRL5_actrl_mdm5g_ret_mem_ctrl0_START (0)
#define SOC_ACTRL_AO_MEM_CTRL5_actrl_mdm5g_ret_mem_ctrl0_END (15)
#define SOC_ACTRL_AO_MEM_CTRL5_ao_mem_ctrl5_msk_START (16)
#define SOC_ACTRL_AO_MEM_CTRL5_ao_mem_ctrl5_msk_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int actrl_mdm5g_ret_mem_ctrl1 : 16;
        unsigned int ao_mem_ctrl6_msk : 16;
    } reg;
} SOC_ACTRL_AO_MEM_CTRL6_UNION;
#endif
#define SOC_ACTRL_AO_MEM_CTRL6_actrl_mdm5g_ret_mem_ctrl1_START (0)
#define SOC_ACTRL_AO_MEM_CTRL6_actrl_mdm5g_ret_mem_ctrl1_END (15)
#define SOC_ACTRL_AO_MEM_CTRL6_ao_mem_ctrl6_msk_START (16)
#define SOC_ACTRL_AO_MEM_CTRL6_ao_mem_ctrl6_msk_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int actrl_mdm5g_ret_mem_ctrl2 : 16;
        unsigned int ao_mem_ctrl7_msk : 16;
    } reg;
} SOC_ACTRL_AO_MEM_CTRL7_UNION;
#endif
#define SOC_ACTRL_AO_MEM_CTRL7_actrl_mdm5g_ret_mem_ctrl2_START (0)
#define SOC_ACTRL_AO_MEM_CTRL7_actrl_mdm5g_ret_mem_ctrl2_END (15)
#define SOC_ACTRL_AO_MEM_CTRL7_ao_mem_ctrl7_msk_START (16)
#define SOC_ACTRL_AO_MEM_CTRL7_ao_mem_ctrl7_msk_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int actrl_mdm5g_ret_mem_ctrl3 : 16;
        unsigned int ao_mem_ctrl8_msk : 16;
    } reg;
} SOC_ACTRL_AO_MEM_CTRL8_UNION;
#endif
#define SOC_ACTRL_AO_MEM_CTRL8_actrl_mdm5g_ret_mem_ctrl3_START (0)
#define SOC_ACTRL_AO_MEM_CTRL8_actrl_mdm5g_ret_mem_ctrl3_END (15)
#define SOC_ACTRL_AO_MEM_CTRL8_ao_mem_ctrl8_msk_START (16)
#define SOC_ACTRL_AO_MEM_CTRL8_ao_mem_ctrl8_msk_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int actrl_mem_ctrl_reserved1 : 16;
        unsigned int ao_mem_ctrl9_msk : 16;
    } reg;
} SOC_ACTRL_AO_MEM_CTRL9_UNION;
#endif
#define SOC_ACTRL_AO_MEM_CTRL9_actrl_mem_ctrl_reserved1_START (0)
#define SOC_ACTRL_AO_MEM_CTRL9_actrl_mem_ctrl_reserved1_END (15)
#define SOC_ACTRL_AO_MEM_CTRL9_ao_mem_ctrl9_msk_START (16)
#define SOC_ACTRL_AO_MEM_CTRL9_ao_mem_ctrl9_msk_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int asp_sdo_astat : 1;
        unsigned int asp_dram_dso : 1;
        unsigned int asp_dram_sdo : 1;
        unsigned int asp_ocram_dso : 1;
        unsigned int asp_ocram_sdo : 1;
        unsigned int asp_iram_dso : 1;
        unsigned int asp_iram_sdo : 1;
        unsigned int asp_mfcc_mem_sd_out : 1;
        unsigned int asp_mfcc_mem_ds_out : 1;
        unsigned int asp_mfcc_memlp_sd_out : 1;
        unsigned int swing_sd_out : 1;
        unsigned int mad_sd_out : 1;
        unsigned int iomcu_sd_out : 1;
        unsigned int hifd_ul_lb_sd_out : 8;
        unsigned int reserved : 11;
    } reg;
} SOC_ACTRL_AO_MEM_STATUS_UNION;
#endif
#define SOC_ACTRL_AO_MEM_STATUS_asp_sdo_astat_START (0)
#define SOC_ACTRL_AO_MEM_STATUS_asp_sdo_astat_END (0)
#define SOC_ACTRL_AO_MEM_STATUS_asp_dram_dso_START (1)
#define SOC_ACTRL_AO_MEM_STATUS_asp_dram_dso_END (1)
#define SOC_ACTRL_AO_MEM_STATUS_asp_dram_sdo_START (2)
#define SOC_ACTRL_AO_MEM_STATUS_asp_dram_sdo_END (2)
#define SOC_ACTRL_AO_MEM_STATUS_asp_ocram_dso_START (3)
#define SOC_ACTRL_AO_MEM_STATUS_asp_ocram_dso_END (3)
#define SOC_ACTRL_AO_MEM_STATUS_asp_ocram_sdo_START (4)
#define SOC_ACTRL_AO_MEM_STATUS_asp_ocram_sdo_END (4)
#define SOC_ACTRL_AO_MEM_STATUS_asp_iram_dso_START (5)
#define SOC_ACTRL_AO_MEM_STATUS_asp_iram_dso_END (5)
#define SOC_ACTRL_AO_MEM_STATUS_asp_iram_sdo_START (6)
#define SOC_ACTRL_AO_MEM_STATUS_asp_iram_sdo_END (6)
#define SOC_ACTRL_AO_MEM_STATUS_asp_mfcc_mem_sd_out_START (7)
#define SOC_ACTRL_AO_MEM_STATUS_asp_mfcc_mem_sd_out_END (7)
#define SOC_ACTRL_AO_MEM_STATUS_asp_mfcc_mem_ds_out_START (8)
#define SOC_ACTRL_AO_MEM_STATUS_asp_mfcc_mem_ds_out_END (8)
#define SOC_ACTRL_AO_MEM_STATUS_asp_mfcc_memlp_sd_out_START (9)
#define SOC_ACTRL_AO_MEM_STATUS_asp_mfcc_memlp_sd_out_END (9)
#define SOC_ACTRL_AO_MEM_STATUS_swing_sd_out_START (10)
#define SOC_ACTRL_AO_MEM_STATUS_swing_sd_out_END (10)
#define SOC_ACTRL_AO_MEM_STATUS_mad_sd_out_START (11)
#define SOC_ACTRL_AO_MEM_STATUS_mad_sd_out_END (11)
#define SOC_ACTRL_AO_MEM_STATUS_iomcu_sd_out_START (12)
#define SOC_ACTRL_AO_MEM_STATUS_iomcu_sd_out_END (12)
#define SOC_ACTRL_AO_MEM_STATUS_hifd_ul_lb_sd_out_START (13)
#define SOC_ACTRL_AO_MEM_STATUS_hifd_ul_lb_sd_out_END (20)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int iomcu_tcp_mem_ctrl_num : 5;
        unsigned int iomcu_tcp_mem_ctrl : 2;
        unsigned int reserved : 25;
    } reg;
} SOC_ACTRL_SEC_CTRL0_UNION;
#endif
#define SOC_ACTRL_SEC_CTRL0_iomcu_tcp_mem_ctrl_num_START (0)
#define SOC_ACTRL_SEC_CTRL0_iomcu_tcp_mem_ctrl_num_END (4)
#define SOC_ACTRL_SEC_CTRL0_iomcu_tcp_mem_ctrl_START (5)
#define SOC_ACTRL_SEC_CTRL0_iomcu_tcp_mem_ctrl_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int asp_hifi_idle : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_ACTRL_SEC_STATUS0_UNION;
#endif
#define SOC_ACTRL_SEC_STATUS0_asp_hifi_idle_START (0)
#define SOC_ACTRL_SEC_STATUS0_asp_hifi_idle_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int actrl_npu_lb_ret_sd_in : 4;
        unsigned int actrl_npu_lb_ret_ds_in : 4;
        unsigned int reserved : 8;
        unsigned int ao_mem_ctrl10_msk : 16;
    } reg;
} SOC_ACTRL_AO_MEM_CTRL10_UNION;
#endif
#define SOC_ACTRL_AO_MEM_CTRL10_actrl_npu_lb_ret_sd_in_START (0)
#define SOC_ACTRL_AO_MEM_CTRL10_actrl_npu_lb_ret_sd_in_END (3)
#define SOC_ACTRL_AO_MEM_CTRL10_actrl_npu_lb_ret_ds_in_START (4)
#define SOC_ACTRL_AO_MEM_CTRL10_actrl_npu_lb_ret_ds_in_END (7)
#define SOC_ACTRL_AO_MEM_CTRL10_ao_mem_ctrl10_msk_START (16)
#define SOC_ACTRL_AO_MEM_CTRL10_ao_mem_ctrl10_msk_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int actrl_bitmsk_sec_ctrl1 : 16;
        unsigned int sec_ctrl1_msk : 16;
    } reg;
} SOC_ACTRL_BITMSK_SEC_CTRL1_UNION;
#endif
#define SOC_ACTRL_BITMSK_SEC_CTRL1_actrl_bitmsk_sec_ctrl1_START (0)
#define SOC_ACTRL_BITMSK_SEC_CTRL1_actrl_bitmsk_sec_ctrl1_END (15)
#define SOC_ACTRL_BITMSK_SEC_CTRL1_sec_ctrl1_msk_START (16)
#define SOC_ACTRL_BITMSK_SEC_CTRL1_sec_ctrl1_msk_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int actrl_bitmsk_sec_ctrl2 : 16;
        unsigned int sec_ctrl2_msk : 16;
    } reg;
} SOC_ACTRL_BITMSK_SEC_CTRL2_UNION;
#endif
#define SOC_ACTRL_BITMSK_SEC_CTRL2_actrl_bitmsk_sec_ctrl2_START (0)
#define SOC_ACTRL_BITMSK_SEC_CTRL2_actrl_bitmsk_sec_ctrl2_END (15)
#define SOC_ACTRL_BITMSK_SEC_CTRL2_sec_ctrl2_msk_START (16)
#define SOC_ACTRL_BITMSK_SEC_CTRL2_sec_ctrl2_msk_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int actrl_bitmsk_sec_ctrl3 : 16;
        unsigned int sec_ctrl3_msk : 16;
    } reg;
} SOC_ACTRL_BITMSK_SEC_CTRL3_UNION;
#endif
#define SOC_ACTRL_BITMSK_SEC_CTRL3_actrl_bitmsk_sec_ctrl3_START (0)
#define SOC_ACTRL_BITMSK_SEC_CTRL3_actrl_bitmsk_sec_ctrl3_END (15)
#define SOC_ACTRL_BITMSK_SEC_CTRL3_sec_ctrl3_msk_START (16)
#define SOC_ACTRL_BITMSK_SEC_CTRL3_sec_ctrl3_msk_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int actrl_bitmsk_sec_ctrl4 : 16;
        unsigned int sec_ctrl4_msk : 16;
    } reg;
} SOC_ACTRL_BITMSK_SEC_CTRL4_UNION;
#endif
#define SOC_ACTRL_BITMSK_SEC_CTRL4_actrl_bitmsk_sec_ctrl4_START (0)
#define SOC_ACTRL_BITMSK_SEC_CTRL4_actrl_bitmsk_sec_ctrl4_END (15)
#define SOC_ACTRL_BITMSK_SEC_CTRL4_sec_ctrl4_msk_START (16)
#define SOC_ACTRL_BITMSK_SEC_CTRL4_sec_ctrl4_msk_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int jtag_dfx_disable : 1;
        unsigned int actrl_sec_status1 : 31;
    } reg;
} SOC_ACTRL_SEC_STATUS1_UNION;
#endif
#define SOC_ACTRL_SEC_STATUS1_jtag_dfx_disable_START (0)
#define SOC_ACTRL_SEC_STATUS1_jtag_dfx_disable_END (0)
#define SOC_ACTRL_SEC_STATUS1_actrl_sec_status1_START (1)
#define SOC_ACTRL_SEC_STATUS1_actrl_sec_status1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int actrl_sec_status2 : 32;
    } reg;
} SOC_ACTRL_SEC_STATUS2_UNION;
#endif
#define SOC_ACTRL_SEC_STATUS2_actrl_sec_status2_START (0)
#define SOC_ACTRL_SEC_STATUS2_actrl_sec_status2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int noc_intrleave_gran : 2;
        unsigned int iomcu_arc_halt_req_a : 1;
        unsigned int iomcu_arc_run_req_a : 1;
        unsigned int reserved_2 : 10;
        unsigned int sec_ctrl0_msk : 16;
    } reg;
} SOC_ACTRL_BITMSK_SEC_CTRL0_UNION;
#endif
#define SOC_ACTRL_BITMSK_SEC_CTRL0_noc_intrleave_gran_START (2)
#define SOC_ACTRL_BITMSK_SEC_CTRL0_noc_intrleave_gran_END (3)
#define SOC_ACTRL_BITMSK_SEC_CTRL0_iomcu_arc_halt_req_a_START (4)
#define SOC_ACTRL_BITMSK_SEC_CTRL0_iomcu_arc_halt_req_a_END (4)
#define SOC_ACTRL_BITMSK_SEC_CTRL0_iomcu_arc_run_req_a_START (5)
#define SOC_ACTRL_BITMSK_SEC_CTRL0_iomcu_arc_run_req_a_END (5)
#define SOC_ACTRL_BITMSK_SEC_CTRL0_sec_ctrl0_msk_START (16)
#define SOC_ACTRL_BITMSK_SEC_CTRL0_sec_ctrl0_msk_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int media2_iso_en : 1;
        unsigned int reserved : 15;
        unsigned int iso_en_group_sec_msk : 16;
    } reg;
} SOC_ACTRL_ISO_EN_GROUP_SEC_UNION;
#endif
#define SOC_ACTRL_ISO_EN_GROUP_SEC_media2_iso_en_START (0)
#define SOC_ACTRL_ISO_EN_GROUP_SEC_media2_iso_en_END (0)
#define SOC_ACTRL_ISO_EN_GROUP_SEC_iso_en_group_sec_msk_START (16)
#define SOC_ACTRL_ISO_EN_GROUP_SEC_iso_en_group_sec_msk_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int media2_mtcmos_en : 1;
        unsigned int reserved : 15;
        unsigned int mtcmos_en_group_sec_msk : 16;
    } reg;
} SOC_ACTRL_MTCMOS_EN_GROUP_SEC_UNION;
#endif
#define SOC_ACTRL_MTCMOS_EN_GROUP_SEC_media2_mtcmos_en_START (0)
#define SOC_ACTRL_MTCMOS_EN_GROUP_SEC_media2_mtcmos_en_END (0)
#define SOC_ACTRL_MTCMOS_EN_GROUP_SEC_mtcmos_en_group_sec_msk_START (16)
#define SOC_ACTRL_MTCMOS_EN_GROUP_SEC_mtcmos_en_group_sec_msk_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int media2_mtcmos_ack : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_ACTRL_MTCMOS_ACK_GROUP_SEC_UNION;
#endif
#define SOC_ACTRL_MTCMOS_ACK_GROUP_SEC_media2_mtcmos_ack_START (0)
#define SOC_ACTRL_MTCMOS_ACK_GROUP_SEC_media2_mtcmos_ack_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hieps_dcu_en : 32;
    } reg;
} SOC_ACTRL_HIEPS_DCU_EN_UNION;
#endif
#define SOC_ACTRL_HIEPS_DCU_EN_hieps_dcu_en_START (0)
#define SOC_ACTRL_HIEPS_DCU_EN_hieps_dcu_en_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hieps_resume : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_ACTRL_HIEPS_CTRL_SEC_UNION;
#endif
#define SOC_ACTRL_HIEPS_CTRL_SEC_hieps_resume_START (0)
#define SOC_ACTRL_HIEPS_CTRL_SEC_hieps_resume_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hieps_stat_counter_inc_reg : 32;
    } reg;
} SOC_ACTRL_HIEPS_STATE_COUNTER_INC_REG_UNION;
#endif
#define SOC_ACTRL_HIEPS_STATE_COUNTER_INC_REG_hieps_stat_counter_inc_reg_START (0)
#define SOC_ACTRL_HIEPS_STATE_COUNTER_INC_REG_hieps_stat_counter_inc_reg_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hieps_state_counter : 32;
    } reg;
} SOC_ACTRL_HIEPS_STATE_COUNTER_UNION;
#endif
#define SOC_ACTRL_HIEPS_STATE_COUNTER_hieps_state_counter_START (0)
#define SOC_ACTRL_HIEPS_STATE_COUNTER_hieps_state_counter_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int jtag_dfx_disable_en : 1;
        unsigned int hieps_ctrl0 : 31;
    } reg;
} SOC_ACTRL_HIEPS_CTRL0_UNION;
#endif
#define SOC_ACTRL_HIEPS_CTRL0_jtag_dfx_disable_en_START (0)
#define SOC_ACTRL_HIEPS_CTRL0_jtag_dfx_disable_en_END (0)
#define SOC_ACTRL_HIEPS_CTRL0_hieps_ctrl0_START (1)
#define SOC_ACTRL_HIEPS_CTRL0_hieps_ctrl0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hieps_ctrl1 : 32;
    } reg;
} SOC_ACTRL_HIEPS_CTRL1_UNION;
#endif
#define SOC_ACTRL_HIEPS_CTRL1_hieps_ctrl1_START (0)
#define SOC_ACTRL_HIEPS_CTRL1_hieps_ctrl1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hieps_ctrl2 : 32;
    } reg;
} SOC_ACTRL_HIEPS_CTRL2_UNION;
#endif
#define SOC_ACTRL_HIEPS_CTRL2_hieps_ctrl2_START (0)
#define SOC_ACTRL_HIEPS_CTRL2_hieps_ctrl2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hieps_ctrl3 : 32;
    } reg;
} SOC_ACTRL_HIEPS_CTRL3_UNION;
#endif
#define SOC_ACTRL_HIEPS_CTRL3_hieps_ctrl3_START (0)
#define SOC_ACTRL_HIEPS_CTRL3_hieps_ctrl3_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hieps_ctrl4 : 32;
    } reg;
} SOC_ACTRL_HIEPS_CTRL4_UNION;
#endif
#define SOC_ACTRL_HIEPS_CTRL4_hieps_ctrl4_START (0)
#define SOC_ACTRL_HIEPS_CTRL4_hieps_ctrl4_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hieps_ctrl5 : 32;
    } reg;
} SOC_ACTRL_HIEPS_CTRL5_UNION;
#endif
#define SOC_ACTRL_HIEPS_CTRL5_hieps_ctrl5_START (0)
#define SOC_ACTRL_HIEPS_CTRL5_hieps_ctrl5_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hieps_ctrl6 : 32;
    } reg;
} SOC_ACTRL_HIEPS_CTRL6_UNION;
#endif
#define SOC_ACTRL_HIEPS_CTRL6_hieps_ctrl6_START (0)
#define SOC_ACTRL_HIEPS_CTRL6_hieps_ctrl6_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hieps_ctrl7 : 32;
    } reg;
} SOC_ACTRL_HIEPS_CTRL7_UNION;
#endif
#define SOC_ACTRL_HIEPS_CTRL7_hieps_ctrl7_START (0)
#define SOC_ACTRL_HIEPS_CTRL7_hieps_ctrl7_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hieps_ctrl8 : 32;
    } reg;
} SOC_ACTRL_HIEPS_CTRL8_UNION;
#endif
#define SOC_ACTRL_HIEPS_CTRL8_hieps_ctrl8_START (0)
#define SOC_ACTRL_HIEPS_CTRL8_hieps_ctrl8_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hieps_ctrl9 : 32;
    } reg;
} SOC_ACTRL_HIEPS_CTRL9_UNION;
#endif
#define SOC_ACTRL_HIEPS_CTRL9_hieps_ctrl9_START (0)
#define SOC_ACTRL_HIEPS_CTRL9_hieps_ctrl9_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hieps_ctrl10 : 32;
    } reg;
} SOC_ACTRL_HIEPS_CTRL10_UNION;
#endif
#define SOC_ACTRL_HIEPS_CTRL10_hieps_ctrl10_START (0)
#define SOC_ACTRL_HIEPS_CTRL10_hieps_ctrl10_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hieps_ctrl11 : 32;
    } reg;
} SOC_ACTRL_HIEPS_CTRL11_UNION;
#endif
#define SOC_ACTRL_HIEPS_CTRL11_hieps_ctrl11_START (0)
#define SOC_ACTRL_HIEPS_CTRL11_hieps_ctrl11_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hieps_ctrl12 : 32;
    } reg;
} SOC_ACTRL_HIEPS_CTRL12_UNION;
#endif
#define SOC_ACTRL_HIEPS_CTRL12_hieps_ctrl12_START (0)
#define SOC_ACTRL_HIEPS_CTRL12_hieps_ctrl12_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hieps_ctrl13 : 32;
    } reg;
} SOC_ACTRL_HIEPS_CTRL13_UNION;
#endif
#define SOC_ACTRL_HIEPS_CTRL13_hieps_ctrl13_START (0)
#define SOC_ACTRL_HIEPS_CTRL13_hieps_ctrl13_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hieps_ctrl14 : 32;
    } reg;
} SOC_ACTRL_HIEPS_CTRL14_UNION;
#endif
#define SOC_ACTRL_HIEPS_CTRL14_hieps_ctrl14_START (0)
#define SOC_ACTRL_HIEPS_CTRL14_hieps_ctrl14_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hieps_ctrl15 : 32;
    } reg;
} SOC_ACTRL_HIEPS_CTRL15_UNION;
#endif
#define SOC_ACTRL_HIEPS_CTRL15_hieps_ctrl15_START (0)
#define SOC_ACTRL_HIEPS_CTRL15_hieps_ctrl15_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int actrl2eps_reg0 : 32;
    } reg;
} SOC_ACTRL_ACTRL2EPS_REG0_UNION;
#endif
#define SOC_ACTRL_ACTRL2EPS_REG0_actrl2eps_reg0_START (0)
#define SOC_ACTRL_ACTRL2EPS_REG0_actrl2eps_reg0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int actrl2eps_reg1 : 32;
    } reg;
} SOC_ACTRL_ACTRL2EPS_REG1_UNION;
#endif
#define SOC_ACTRL_ACTRL2EPS_REG1_actrl2eps_reg1_START (0)
#define SOC_ACTRL_ACTRL2EPS_REG1_actrl2eps_reg1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int actrl2eps_reg2 : 32;
    } reg;
} SOC_ACTRL_ACTRL2EPS_REG2_UNION;
#endif
#define SOC_ACTRL_ACTRL2EPS_REG2_actrl2eps_reg2_START (0)
#define SOC_ACTRL_ACTRL2EPS_REG2_actrl2eps_reg2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int actrl2eps_reg3 : 32;
    } reg;
} SOC_ACTRL_ACTRL2EPS_REG3_UNION;
#endif
#define SOC_ACTRL_ACTRL2EPS_REG3_actrl2eps_reg3_START (0)
#define SOC_ACTRL_ACTRL2EPS_REG3_actrl2eps_reg3_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int actrl2eps_reg4 : 32;
    } reg;
} SOC_ACTRL_ACTRL2EPS_REG4_UNION;
#endif
#define SOC_ACTRL_ACTRL2EPS_REG4_actrl2eps_reg4_START (0)
#define SOC_ACTRL_ACTRL2EPS_REG4_actrl2eps_reg4_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int eps2actrl_reg0 : 32;
    } reg;
} SOC_ACTRL_EPS2ACTRL_REG0_UNION;
#endif
#define SOC_ACTRL_EPS2ACTRL_REG0_eps2actrl_reg0_START (0)
#define SOC_ACTRL_EPS2ACTRL_REG0_eps2actrl_reg0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int eps2actrl_reg1 : 32;
    } reg;
} SOC_ACTRL_EPS2ACTRL_REG1_UNION;
#endif
#define SOC_ACTRL_EPS2ACTRL_REG1_eps2actrl_reg1_START (0)
#define SOC_ACTRL_EPS2ACTRL_REG1_eps2actrl_reg1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int eps2actrl_reg2 : 32;
    } reg;
} SOC_ACTRL_EPS2ACTRL_REG2_UNION;
#endif
#define SOC_ACTRL_EPS2ACTRL_REG2_eps2actrl_reg2_START (0)
#define SOC_ACTRL_EPS2ACTRL_REG2_eps2actrl_reg2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int eps2actrl_reg3 : 32;
    } reg;
} SOC_ACTRL_EPS2ACTRL_REG3_UNION;
#endif
#define SOC_ACTRL_EPS2ACTRL_REG3_eps2actrl_reg3_START (0)
#define SOC_ACTRL_EPS2ACTRL_REG3_eps2actrl_reg3_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int eps2actrl_reg4 : 32;
    } reg;
} SOC_ACTRL_EPS2ACTRL_REG4_UNION;
#endif
#define SOC_ACTRL_EPS2ACTRL_REG4_eps2actrl_reg4_START (0)
#define SOC_ACTRL_EPS2ACTRL_REG4_eps2actrl_reg4_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int actrl_eps_ret_mem_sd_in : 1;
        unsigned int actrl_eps_ret_mem_ds_in : 1;
        unsigned int reserved : 14;
        unsigned int hieps_mem_ctrl_msk : 16;
    } reg;
} SOC_ACTRL_HIEPS_MEM_CTRL_UNION;
#endif
#define SOC_ACTRL_HIEPS_MEM_CTRL_actrl_eps_ret_mem_sd_in_START (0)
#define SOC_ACTRL_HIEPS_MEM_CTRL_actrl_eps_ret_mem_sd_in_END (0)
#define SOC_ACTRL_HIEPS_MEM_CTRL_actrl_eps_ret_mem_ds_in_START (1)
#define SOC_ACTRL_HIEPS_MEM_CTRL_actrl_eps_ret_mem_ds_in_END (1)
#define SOC_ACTRL_HIEPS_MEM_CTRL_hieps_mem_ctrl_msk_START (16)
#define SOC_ACTRL_HIEPS_MEM_CTRL_hieps_mem_ctrl_msk_END (31)
#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
#endif
