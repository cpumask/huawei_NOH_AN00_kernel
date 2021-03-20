#ifndef __SOC_NPU_SCTRL_INTERFACE_H__
#define __SOC_NPU_SCTRL_INTERFACE_H__ 
#ifdef __cplusplus
    #if __cplusplus
        extern "C" {
    #endif
#endif
#ifndef __SOC_H_FOR_ASM__
#define SOC_NPU_SCTRL_npu_ctrl0_ADDR(base) ((base) + (0x000UL))
#define SOC_NPU_SCTRL_npu_ctrl1_ADDR(base) ((base) + (0x004UL))
#define SOC_NPU_SCTRL_npu_ctrl2_ADDR(base) ((base) + (0x008UL))
#define SOC_NPU_SCTRL_npu_ctrl3_ADDR(base) ((base) + (0x00CUL))
#define SOC_NPU_SCTRL_npu_ctrl4_ADDR(base) ((base) + (0x010UL))
#define SOC_NPU_SCTRL_npu_ctrl5_ADDR(base) ((base) + (0x014UL))
#define SOC_NPU_SCTRL_npu_ctrl6_ADDR(base) ((base) + (0x018UL))
#define SOC_NPU_SCTRL_npu_ctrl7_ADDR(base) ((base) + (0x01CUL))
#define SOC_NPU_SCTRL_npu_ctrl8_ADDR(base) ((base) + (0x020UL))
#define SOC_NPU_SCTRL_npu_ctrl9_ADDR(base) ((base) + (0x024UL))
#define SOC_NPU_SCTRL_npu_ctrl10_ADDR(base) ((base) + (0x028UL))
#define SOC_NPU_SCTRL_npu_ctrl11_ADDR(base) ((base) + (0x02CUL))
#define SOC_NPU_SCTRL_npu_ctrl12_ADDR(base) ((base) + (0x030UL))
#define SOC_NPU_SCTRL_npu_ctrl13_ADDR(base) ((base) + (0x034UL))
#define SOC_NPU_SCTRL_npu_ctrl14_ADDR(base) ((base) + (0x038UL))
#define SOC_NPU_SCTRL_npu_ctrl15_ADDR(base) ((base) + (0x03CUL))
#define SOC_NPU_SCTRL_npu_ctrl16_ADDR(base) ((base) + (0x040UL))
#define SOC_NPU_SCTRL_npu_ctrl17_ADDR(base) ((base) + (0x044UL))
#define SOC_NPU_SCTRL_npu_ctrl18_ADDR(base) ((base) + (0x048UL))
#define SOC_NPU_SCTRL_npu_ctrl19_ADDR(base) ((base) + (0x04CUL))
#define SOC_NPU_SCTRL_npu_ctrl20_ADDR(base) ((base) + (0x050UL))
#define SOC_NPU_SCTRL_npu_ctrl21_ADDR(base) ((base) + (0x054UL))
#define SOC_NPU_SCTRL_npu_ctrl22_ADDR(base) ((base) + (0x058UL))
#define SOC_NPU_SCTRL_npu_ctrl23_ADDR(base) ((base) + (0x05CUL))
#define SOC_NPU_SCTRL_npu_ctrl24_ADDR(base) ((base) + (0x060UL))
#define SOC_NPU_SCTRL_npu_ctrl25_ADDR(base) ((base) + (0x064UL))
#define SOC_NPU_SCTRL_npu_ctrl26_ADDR(base) ((base) + (0x068UL))
#define SOC_NPU_SCTRL_npu_ctrl27_ADDR(base) ((base) + (0x06CUL))
#define SOC_NPU_SCTRL_npu_ctrl28_ADDR(base) ((base) + (0x070UL))
#define SOC_NPU_SCTRL_npu_ctrl29_ADDR(base) ((base) + (0x074UL))
#define SOC_NPU_SCTRL_npu_ctrl30_ADDR(base) ((base) + (0x078UL))
#define SOC_NPU_SCTRL_npu_ctrl31_ADDR(base) ((base) + (0x07CUL))
#define SOC_NPU_SCTRL_npu_ctrl32_ADDR(base) ((base) + (0x080UL))
#define SOC_NPU_SCTRL_npu_ctrl33_ADDR(base) ((base) + (0x084UL))
#define SOC_NPU_SCTRL_npu_ctrl34_ADDR(base) ((base) + (0x08CUL))
#define SOC_NPU_SCTRL_npu_ctrl35_ADDR(base) ((base) + (0x090UL))
#define SOC_NPU_SCTRL_npu_ctrl36_ADDR(base) ((base) + (0x094UL))
#define SOC_NPU_SCTRL_npu_ctrl37_ADDR(base) ((base) + (0x09CUL))
#define SOC_NPU_SCTRL_npu_ctrl38_ADDR(base) ((base) + (0x100UL))
#define SOC_NPU_SCTRL_npu_ctrl39_ADDR(base) ((base) + (0x104UL))
#define SOC_NPU_SCTRL_npu_ctrl40_ADDR(base) ((base) + (0x108UL))
#define SOC_NPU_SCTRL_npu_stat0_ADDR(base) ((base) + (0x800UL))
#define SOC_NPU_SCTRL_npu_stat1_ADDR(base) ((base) + (0x804UL))
#define SOC_NPU_SCTRL_npu_stat2_ADDR(base) ((base) + (0x808UL))
#define SOC_NPU_SCTRL_npu_stat3_ADDR(base) ((base) + (0x80CUL))
#define SOC_NPU_SCTRL_npu_stat4_ADDR(base) ((base) + (0x810UL))
#define SOC_NPU_SCTRL_npu_stat5_ADDR(base) ((base) + (0x814UL))
#define SOC_NPU_SCTRL_npu_stat6_ADDR(base) ((base) + (0x818UL))
#define SOC_NPU_SCTRL_npu_stat7_ADDR(base) ((base) + (0x81CUL))
#define SOC_NPU_SCTRL_npu_stat8_ADDR(base) ((base) + (0x820UL))
#define SOC_NPU_SCTRL_npu_stat9_ADDR(base) ((base) + (0x824UL))
#define SOC_NPU_SCTRL_npu_stat10_ADDR(base) ((base) + (0x828UL))
#define SOC_NPU_SCTRL_npu_stat11_ADDR(base) ((base) + (0x82CUL))
#define SOC_NPU_SCTRL_npu_stat12_ADDR(base) ((base) + (0x830UL))
#define SOC_NPU_SCTRL_npu_stat13_ADDR(base) ((base) + (0x834UL))
#define SOC_NPU_SCTRL_npu_stat14_ADDR(base) ((base) + (0x838UL))
#define SOC_NPU_SCTRL_npu_stat15_ADDR(base) ((base) + (0x83CUL))
#define SOC_NPU_SCTRL_npu_stat16_ADDR(base) ((base) + (0x840UL))
#define SOC_NPU_SCTRL_npu_stat17_ADDR(base) ((base) + (0x844UL))
#define SOC_NPU_SCTRL_npu_stat18_ADDR(base) ((base) + (0x848UL))
#define SOC_NPU_SCTRL_npu_stat19_ADDR(base) ((base) + (0x84CUL))
#define SOC_NPU_SCTRL_npu_stat20_ADDR(base) ((base) + (0x850UL))
#define SOC_NPU_SCTRL_npu_stat21_ADDR(base) ((base) + (0x854UL))
#define SOC_NPU_SCTRL_npu_stat22_ADDR(base) ((base) + (0x858UL))
#define SOC_NPU_SCTRL_npu_stat23_ADDR(base) ((base) + (0x85CUL))
#define SOC_NPU_SCTRL_npu_stat24_ADDR(base) ((base) + (0x860UL))
#define SOC_NPU_SCTRL_npu_ovc_rint_ADDR(base) ((base) + (0x864UL))
#define SOC_NPU_SCTRL_npu_ovc_mint_ADDR(base) ((base) + (0x868UL))
#define SOC_NPU_SCTRL_npu_state23_ADDR(base) ((base) + (0x86CUL))
#define SOC_NPU_SCTRL_npu_state24_ADDR(base) ((base) + (0x870UL))
#define SOC_NPU_SCTRL_npu_state25_ADDR(base) ((base) + (0x874UL))
#else
#define SOC_NPU_SCTRL_npu_ctrl0_ADDR(base) ((base) + (0x000))
#define SOC_NPU_SCTRL_npu_ctrl1_ADDR(base) ((base) + (0x004))
#define SOC_NPU_SCTRL_npu_ctrl2_ADDR(base) ((base) + (0x008))
#define SOC_NPU_SCTRL_npu_ctrl3_ADDR(base) ((base) + (0x00C))
#define SOC_NPU_SCTRL_npu_ctrl4_ADDR(base) ((base) + (0x010))
#define SOC_NPU_SCTRL_npu_ctrl5_ADDR(base) ((base) + (0x014))
#define SOC_NPU_SCTRL_npu_ctrl6_ADDR(base) ((base) + (0x018))
#define SOC_NPU_SCTRL_npu_ctrl7_ADDR(base) ((base) + (0x01C))
#define SOC_NPU_SCTRL_npu_ctrl8_ADDR(base) ((base) + (0x020))
#define SOC_NPU_SCTRL_npu_ctrl9_ADDR(base) ((base) + (0x024))
#define SOC_NPU_SCTRL_npu_ctrl10_ADDR(base) ((base) + (0x028))
#define SOC_NPU_SCTRL_npu_ctrl11_ADDR(base) ((base) + (0x02C))
#define SOC_NPU_SCTRL_npu_ctrl12_ADDR(base) ((base) + (0x030))
#define SOC_NPU_SCTRL_npu_ctrl13_ADDR(base) ((base) + (0x034))
#define SOC_NPU_SCTRL_npu_ctrl14_ADDR(base) ((base) + (0x038))
#define SOC_NPU_SCTRL_npu_ctrl15_ADDR(base) ((base) + (0x03C))
#define SOC_NPU_SCTRL_npu_ctrl16_ADDR(base) ((base) + (0x040))
#define SOC_NPU_SCTRL_npu_ctrl17_ADDR(base) ((base) + (0x044))
#define SOC_NPU_SCTRL_npu_ctrl18_ADDR(base) ((base) + (0x048))
#define SOC_NPU_SCTRL_npu_ctrl19_ADDR(base) ((base) + (0x04C))
#define SOC_NPU_SCTRL_npu_ctrl20_ADDR(base) ((base) + (0x050))
#define SOC_NPU_SCTRL_npu_ctrl21_ADDR(base) ((base) + (0x054))
#define SOC_NPU_SCTRL_npu_ctrl22_ADDR(base) ((base) + (0x058))
#define SOC_NPU_SCTRL_npu_ctrl23_ADDR(base) ((base) + (0x05C))
#define SOC_NPU_SCTRL_npu_ctrl24_ADDR(base) ((base) + (0x060))
#define SOC_NPU_SCTRL_npu_ctrl25_ADDR(base) ((base) + (0x064))
#define SOC_NPU_SCTRL_npu_ctrl26_ADDR(base) ((base) + (0x068))
#define SOC_NPU_SCTRL_npu_ctrl27_ADDR(base) ((base) + (0x06C))
#define SOC_NPU_SCTRL_npu_ctrl28_ADDR(base) ((base) + (0x070))
#define SOC_NPU_SCTRL_npu_ctrl29_ADDR(base) ((base) + (0x074))
#define SOC_NPU_SCTRL_npu_ctrl30_ADDR(base) ((base) + (0x078))
#define SOC_NPU_SCTRL_npu_ctrl31_ADDR(base) ((base) + (0x07C))
#define SOC_NPU_SCTRL_npu_ctrl32_ADDR(base) ((base) + (0x080))
#define SOC_NPU_SCTRL_npu_ctrl33_ADDR(base) ((base) + (0x084))
#define SOC_NPU_SCTRL_npu_ctrl34_ADDR(base) ((base) + (0x08C))
#define SOC_NPU_SCTRL_npu_ctrl35_ADDR(base) ((base) + (0x090))
#define SOC_NPU_SCTRL_npu_ctrl36_ADDR(base) ((base) + (0x094))
#define SOC_NPU_SCTRL_npu_ctrl37_ADDR(base) ((base) + (0x09C))
#define SOC_NPU_SCTRL_npu_ctrl38_ADDR(base) ((base) + (0x100))
#define SOC_NPU_SCTRL_npu_ctrl39_ADDR(base) ((base) + (0x104))
#define SOC_NPU_SCTRL_npu_ctrl40_ADDR(base) ((base) + (0x108))
#define SOC_NPU_SCTRL_npu_stat0_ADDR(base) ((base) + (0x800))
#define SOC_NPU_SCTRL_npu_stat1_ADDR(base) ((base) + (0x804))
#define SOC_NPU_SCTRL_npu_stat2_ADDR(base) ((base) + (0x808))
#define SOC_NPU_SCTRL_npu_stat3_ADDR(base) ((base) + (0x80C))
#define SOC_NPU_SCTRL_npu_stat4_ADDR(base) ((base) + (0x810))
#define SOC_NPU_SCTRL_npu_stat5_ADDR(base) ((base) + (0x814))
#define SOC_NPU_SCTRL_npu_stat6_ADDR(base) ((base) + (0x818))
#define SOC_NPU_SCTRL_npu_stat7_ADDR(base) ((base) + (0x81C))
#define SOC_NPU_SCTRL_npu_stat8_ADDR(base) ((base) + (0x820))
#define SOC_NPU_SCTRL_npu_stat9_ADDR(base) ((base) + (0x824))
#define SOC_NPU_SCTRL_npu_stat10_ADDR(base) ((base) + (0x828))
#define SOC_NPU_SCTRL_npu_stat11_ADDR(base) ((base) + (0x82C))
#define SOC_NPU_SCTRL_npu_stat12_ADDR(base) ((base) + (0x830))
#define SOC_NPU_SCTRL_npu_stat13_ADDR(base) ((base) + (0x834))
#define SOC_NPU_SCTRL_npu_stat14_ADDR(base) ((base) + (0x838))
#define SOC_NPU_SCTRL_npu_stat15_ADDR(base) ((base) + (0x83C))
#define SOC_NPU_SCTRL_npu_stat16_ADDR(base) ((base) + (0x840))
#define SOC_NPU_SCTRL_npu_stat17_ADDR(base) ((base) + (0x844))
#define SOC_NPU_SCTRL_npu_stat18_ADDR(base) ((base) + (0x848))
#define SOC_NPU_SCTRL_npu_stat19_ADDR(base) ((base) + (0x84C))
#define SOC_NPU_SCTRL_npu_stat20_ADDR(base) ((base) + (0x850))
#define SOC_NPU_SCTRL_npu_stat21_ADDR(base) ((base) + (0x854))
#define SOC_NPU_SCTRL_npu_stat22_ADDR(base) ((base) + (0x858))
#define SOC_NPU_SCTRL_npu_stat23_ADDR(base) ((base) + (0x85C))
#define SOC_NPU_SCTRL_npu_stat24_ADDR(base) ((base) + (0x860))
#define SOC_NPU_SCTRL_npu_ovc_rint_ADDR(base) ((base) + (0x864))
#define SOC_NPU_SCTRL_npu_ovc_mint_ADDR(base) ((base) + (0x868))
#define SOC_NPU_SCTRL_npu_state23_ADDR(base) ((base) + (0x86C))
#define SOC_NPU_SCTRL_npu_state24_ADDR(base) ((base) + (0x870))
#define SOC_NPU_SCTRL_npu_state25_ADDR(base) ((base) + (0x874))
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int dpm0_freq_sel : 3;
        unsigned int dpm0_volt_sel : 3;
        unsigned int dpm1_freq_sel : 3;
        unsigned int dpm1_volt_sel : 3;
        unsigned int hard_volt_monitor : 1;
        unsigned int reserved : 19;
    } reg;
} SOC_NPU_SCTRL_npu_ctrl0_UNION;
#endif
#define SOC_NPU_SCTRL_npu_ctrl0_dpm0_freq_sel_START (0)
#define SOC_NPU_SCTRL_npu_ctrl0_dpm0_freq_sel_END (2)
#define SOC_NPU_SCTRL_npu_ctrl0_dpm0_volt_sel_START (3)
#define SOC_NPU_SCTRL_npu_ctrl0_dpm0_volt_sel_END (5)
#define SOC_NPU_SCTRL_npu_ctrl0_dpm1_freq_sel_START (6)
#define SOC_NPU_SCTRL_npu_ctrl0_dpm1_freq_sel_END (8)
#define SOC_NPU_SCTRL_npu_ctrl0_dpm1_volt_sel_START (9)
#define SOC_NPU_SCTRL_npu_ctrl0_dpm1_volt_sel_END (11)
#define SOC_NPU_SCTRL_npu_ctrl0_hard_volt_monitor_START (12)
#define SOC_NPU_SCTRL_npu_ctrl0_hard_volt_monitor_END (12)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int mem_ctrl_s_npu_cssys : 26;
        unsigned int reserved : 6;
    } reg;
} SOC_NPU_SCTRL_npu_ctrl1_UNION;
#endif
#define SOC_NPU_SCTRL_npu_ctrl1_mem_ctrl_s_npu_cssys_START (0)
#define SOC_NPU_SCTRL_npu_ctrl1_mem_ctrl_s_npu_cssys_END (25)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int mem_ctrl_s_aic0_top : 26;
        unsigned int reserved : 6;
    } reg;
} SOC_NPU_SCTRL_npu_ctrl2_UNION;
#endif
#define SOC_NPU_SCTRL_npu_ctrl2_mem_ctrl_s_aic0_top_START (0)
#define SOC_NPU_SCTRL_npu_ctrl2_mem_ctrl_s_aic0_top_END (25)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int mem_ctrl_d1w2r_aic0_top : 16;
        unsigned int reserved : 16;
    } reg;
} SOC_NPU_SCTRL_npu_ctrl3_UNION;
#endif
#define SOC_NPU_SCTRL_npu_ctrl3_mem_ctrl_d1w2r_aic0_top_START (0)
#define SOC_NPU_SCTRL_npu_ctrl3_mem_ctrl_d1w2r_aic0_top_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int mem_ctrl_s_aic1_top : 26;
        unsigned int reserved : 6;
    } reg;
} SOC_NPU_SCTRL_npu_ctrl4_UNION;
#endif
#define SOC_NPU_SCTRL_npu_ctrl4_mem_ctrl_s_aic1_top_START (0)
#define SOC_NPU_SCTRL_npu_ctrl4_mem_ctrl_s_aic1_top_END (25)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int mem_ctrl_d1w2r_aic1_top : 16;
        unsigned int reserved : 16;
    } reg;
} SOC_NPU_SCTRL_npu_ctrl5_UNION;
#endif
#define SOC_NPU_SCTRL_npu_ctrl5_mem_ctrl_d1w2r_aic1_top_START (0)
#define SOC_NPU_SCTRL_npu_ctrl5_mem_ctrl_d1w2r_aic1_top_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int mem_ctrl_s_tiny_top : 26;
        unsigned int reserved : 6;
    } reg;
} SOC_NPU_SCTRL_npu_ctrl6_UNION;
#endif
#define SOC_NPU_SCTRL_npu_ctrl6_mem_ctrl_s_tiny_top_START (0)
#define SOC_NPU_SCTRL_npu_ctrl6_mem_ctrl_s_tiny_top_END (25)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int mem_ctrl_d1w2r_tiny_top : 16;
        unsigned int reserved : 16;
    } reg;
} SOC_NPU_SCTRL_npu_ctrl7_UNION;
#endif
#define SOC_NPU_SCTRL_npu_ctrl7_mem_ctrl_d1w2r_tiny_top_START (0)
#define SOC_NPU_SCTRL_npu_ctrl7_mem_ctrl_d1w2r_tiny_top_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int l2buf_spram_mem_ctrl : 26;
        unsigned int reserved : 6;
    } reg;
} SOC_NPU_SCTRL_npu_ctrl8_UNION;
#endif
#define SOC_NPU_SCTRL_npu_ctrl8_l2buf_spram_mem_ctrl_START (0)
#define SOC_NPU_SCTRL_npu_ctrl8_l2buf_spram_mem_ctrl_END (25)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int mem_ctrl_s_sysdma : 26;
        unsigned int reserved : 6;
    } reg;
} SOC_NPU_SCTRL_npu_ctrl9_UNION;
#endif
#define SOC_NPU_SCTRL_npu_ctrl9_mem_ctrl_s_sysdma_START (0)
#define SOC_NPU_SCTRL_npu_ctrl9_mem_ctrl_s_sysdma_END (25)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int mem_ctrl_d1w2r_sysdma : 16;
        unsigned int reserved : 16;
    } reg;
} SOC_NPU_SCTRL_npu_ctrl10_UNION;
#endif
#define SOC_NPU_SCTRL_npu_ctrl10_mem_ctrl_d1w2r_sysdma_START (0)
#define SOC_NPU_SCTRL_npu_ctrl10_mem_ctrl_d1w2r_sysdma_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pcr_mem_ctrl_tp : 16;
        unsigned int reserved : 16;
    } reg;
} SOC_NPU_SCTRL_npu_ctrl11_UNION;
#endif
#define SOC_NPU_SCTRL_npu_ctrl11_pcr_mem_ctrl_tp_START (0)
#define SOC_NPU_SCTRL_npu_ctrl11_pcr_mem_ctrl_tp_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hpm0_clk_div : 6;
        unsigned int hpm0_en : 1;
        unsigned int hpmx0_en : 1;
        unsigned int hpm1_clk_div : 6;
        unsigned int hpm1_en : 1;
        unsigned int hpmx1_en : 1;
        unsigned int hpm2_clk_div : 6;
        unsigned int hpm2_en : 1;
        unsigned int hpmx2_en : 1;
        unsigned int hpm3_clk_div : 6;
        unsigned int hpm3_en : 1;
        unsigned int hpmx3_en : 1;
    } reg;
} SOC_NPU_SCTRL_npu_ctrl12_UNION;
#endif
#define SOC_NPU_SCTRL_npu_ctrl12_hpm0_clk_div_START (0)
#define SOC_NPU_SCTRL_npu_ctrl12_hpm0_clk_div_END (5)
#define SOC_NPU_SCTRL_npu_ctrl12_hpm0_en_START (6)
#define SOC_NPU_SCTRL_npu_ctrl12_hpm0_en_END (6)
#define SOC_NPU_SCTRL_npu_ctrl12_hpmx0_en_START (7)
#define SOC_NPU_SCTRL_npu_ctrl12_hpmx0_en_END (7)
#define SOC_NPU_SCTRL_npu_ctrl12_hpm1_clk_div_START (8)
#define SOC_NPU_SCTRL_npu_ctrl12_hpm1_clk_div_END (13)
#define SOC_NPU_SCTRL_npu_ctrl12_hpm1_en_START (14)
#define SOC_NPU_SCTRL_npu_ctrl12_hpm1_en_END (14)
#define SOC_NPU_SCTRL_npu_ctrl12_hpmx1_en_START (15)
#define SOC_NPU_SCTRL_npu_ctrl12_hpmx1_en_END (15)
#define SOC_NPU_SCTRL_npu_ctrl12_hpm2_clk_div_START (16)
#define SOC_NPU_SCTRL_npu_ctrl12_hpm2_clk_div_END (21)
#define SOC_NPU_SCTRL_npu_ctrl12_hpm2_en_START (22)
#define SOC_NPU_SCTRL_npu_ctrl12_hpm2_en_END (22)
#define SOC_NPU_SCTRL_npu_ctrl12_hpmx2_en_START (23)
#define SOC_NPU_SCTRL_npu_ctrl12_hpmx2_en_END (23)
#define SOC_NPU_SCTRL_npu_ctrl12_hpm3_clk_div_START (24)
#define SOC_NPU_SCTRL_npu_ctrl12_hpm3_clk_div_END (29)
#define SOC_NPU_SCTRL_npu_ctrl12_hpm3_en_START (30)
#define SOC_NPU_SCTRL_npu_ctrl12_hpm3_en_END (30)
#define SOC_NPU_SCTRL_npu_ctrl12_hpmx3_en_START (31)
#define SOC_NPU_SCTRL_npu_ctrl12_hpmx3_en_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int icfg_ostd_lvl : 8;
        unsigned int ostd_monitor_en : 1;
        unsigned int reserved : 23;
    } reg;
} SOC_NPU_SCTRL_npu_ctrl13_UNION;
#endif
#define SOC_NPU_SCTRL_npu_ctrl13_icfg_ostd_lvl_START (0)
#define SOC_NPU_SCTRL_npu_ctrl13_icfg_ostd_lvl_END (7)
#define SOC_NPU_SCTRL_npu_ctrl13_ostd_monitor_en_START (8)
#define SOC_NPU_SCTRL_npu_ctrl13_ostd_monitor_en_END (8)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int atgm_aicore_sel : 3;
        unsigned int reserved_0 : 1;
        unsigned int l2buf_intlv_gran : 2;
        unsigned int sc_monitor_rst_req : 1;
        unsigned int reserved_1 : 25;
    } reg;
} SOC_NPU_SCTRL_npu_ctrl14_UNION;
#endif
#define SOC_NPU_SCTRL_npu_ctrl14_atgm_aicore_sel_START (0)
#define SOC_NPU_SCTRL_npu_ctrl14_atgm_aicore_sel_END (2)
#define SOC_NPU_SCTRL_npu_ctrl14_l2buf_intlv_gran_START (4)
#define SOC_NPU_SCTRL_npu_ctrl14_l2buf_intlv_gran_END (5)
#define SOC_NPU_SCTRL_npu_ctrl14_sc_monitor_rst_req_START (6)
#define SOC_NPU_SCTRL_npu_ctrl14_sc_monitor_rst_req_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ts_rd_bwc_saturation : 14;
        unsigned int reserved_0 : 2;
        unsigned int ts_rd_bwc_bandwidth : 13;
        unsigned int reserved_1 : 2;
        unsigned int ts_rd_bwc_en : 1;
    } reg;
} SOC_NPU_SCTRL_npu_ctrl15_UNION;
#endif
#define SOC_NPU_SCTRL_npu_ctrl15_ts_rd_bwc_saturation_START (0)
#define SOC_NPU_SCTRL_npu_ctrl15_ts_rd_bwc_saturation_END (13)
#define SOC_NPU_SCTRL_npu_ctrl15_ts_rd_bwc_bandwidth_START (16)
#define SOC_NPU_SCTRL_npu_ctrl15_ts_rd_bwc_bandwidth_END (28)
#define SOC_NPU_SCTRL_npu_ctrl15_ts_rd_bwc_en_START (31)
#define SOC_NPU_SCTRL_npu_ctrl15_ts_rd_bwc_en_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ts_wr_bwc_saturation : 14;
        unsigned int reserved_0 : 2;
        unsigned int ts_wr_bwc_bandwidth : 13;
        unsigned int reserved_1 : 2;
        unsigned int ts_wr_bwc_en : 1;
    } reg;
} SOC_NPU_SCTRL_npu_ctrl16_UNION;
#endif
#define SOC_NPU_SCTRL_npu_ctrl16_ts_wr_bwc_saturation_START (0)
#define SOC_NPU_SCTRL_npu_ctrl16_ts_wr_bwc_saturation_END (13)
#define SOC_NPU_SCTRL_npu_ctrl16_ts_wr_bwc_bandwidth_START (16)
#define SOC_NPU_SCTRL_npu_ctrl16_ts_wr_bwc_bandwidth_END (28)
#define SOC_NPU_SCTRL_npu_ctrl16_ts_wr_bwc_en_START (31)
#define SOC_NPU_SCTRL_npu_ctrl16_ts_wr_bwc_en_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ts_icfg_ostd0_lvl : 8;
        unsigned int ts_icfg_ostd1_lvl : 8;
        unsigned int flux_en_aicore0 : 1;
        unsigned int ostd_en_aicore0 : 1;
        unsigned int flux_en_ts : 1;
        unsigned int ostd_en_ts : 1;
        unsigned int flux_en_tiny : 1;
        unsigned int ostd_en_tiny : 1;
        unsigned int flux_en_sdma : 1;
        unsigned int ostd_en_sdma : 1;
        unsigned int flux_en_aicore1 : 1;
        unsigned int ostd_en_aicore1 : 1;
        unsigned int reserved_0 : 4;
        unsigned int ts_icfg_ostd0_type : 1;
        unsigned int reserved_1 : 1;
    } reg;
} SOC_NPU_SCTRL_npu_ctrl17_UNION;
#endif
#define SOC_NPU_SCTRL_npu_ctrl17_ts_icfg_ostd0_lvl_START (0)
#define SOC_NPU_SCTRL_npu_ctrl17_ts_icfg_ostd0_lvl_END (7)
#define SOC_NPU_SCTRL_npu_ctrl17_ts_icfg_ostd1_lvl_START (8)
#define SOC_NPU_SCTRL_npu_ctrl17_ts_icfg_ostd1_lvl_END (15)
#define SOC_NPU_SCTRL_npu_ctrl17_flux_en_aicore0_START (16)
#define SOC_NPU_SCTRL_npu_ctrl17_flux_en_aicore0_END (16)
#define SOC_NPU_SCTRL_npu_ctrl17_ostd_en_aicore0_START (17)
#define SOC_NPU_SCTRL_npu_ctrl17_ostd_en_aicore0_END (17)
#define SOC_NPU_SCTRL_npu_ctrl17_flux_en_ts_START (18)
#define SOC_NPU_SCTRL_npu_ctrl17_flux_en_ts_END (18)
#define SOC_NPU_SCTRL_npu_ctrl17_ostd_en_ts_START (19)
#define SOC_NPU_SCTRL_npu_ctrl17_ostd_en_ts_END (19)
#define SOC_NPU_SCTRL_npu_ctrl17_flux_en_tiny_START (20)
#define SOC_NPU_SCTRL_npu_ctrl17_flux_en_tiny_END (20)
#define SOC_NPU_SCTRL_npu_ctrl17_ostd_en_tiny_START (21)
#define SOC_NPU_SCTRL_npu_ctrl17_ostd_en_tiny_END (21)
#define SOC_NPU_SCTRL_npu_ctrl17_flux_en_sdma_START (22)
#define SOC_NPU_SCTRL_npu_ctrl17_flux_en_sdma_END (22)
#define SOC_NPU_SCTRL_npu_ctrl17_ostd_en_sdma_START (23)
#define SOC_NPU_SCTRL_npu_ctrl17_ostd_en_sdma_END (23)
#define SOC_NPU_SCTRL_npu_ctrl17_flux_en_aicore1_START (24)
#define SOC_NPU_SCTRL_npu_ctrl17_flux_en_aicore1_END (24)
#define SOC_NPU_SCTRL_npu_ctrl17_ostd_en_aicore1_START (25)
#define SOC_NPU_SCTRL_npu_ctrl17_ostd_en_aicore1_END (25)
#define SOC_NPU_SCTRL_npu_ctrl17_ts_icfg_ostd0_type_START (30)
#define SOC_NPU_SCTRL_npu_ctrl17_ts_icfg_ostd0_type_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 32;
    } reg;
} SOC_NPU_SCTRL_npu_ctrl18_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int npu_wrap_pa_cfg_valid : 1;
        unsigned int npu_wrap_pa_en : 1;
        unsigned int reserved_0 : 2;
        unsigned int npu_wrap_pa_cmd : 2;
        unsigned int reserved_1 : 2;
        unsigned int npu_wrap_pa_ctrl : 6;
        unsigned int reserved_2 : 2;
        unsigned int npu_wrap_pa_msg : 16;
    } reg;
} SOC_NPU_SCTRL_npu_ctrl19_UNION;
#endif
#define SOC_NPU_SCTRL_npu_ctrl19_npu_wrap_pa_cfg_valid_START (0)
#define SOC_NPU_SCTRL_npu_ctrl19_npu_wrap_pa_cfg_valid_END (0)
#define SOC_NPU_SCTRL_npu_ctrl19_npu_wrap_pa_en_START (1)
#define SOC_NPU_SCTRL_npu_ctrl19_npu_wrap_pa_en_END (1)
#define SOC_NPU_SCTRL_npu_ctrl19_npu_wrap_pa_cmd_START (4)
#define SOC_NPU_SCTRL_npu_ctrl19_npu_wrap_pa_cmd_END (5)
#define SOC_NPU_SCTRL_npu_ctrl19_npu_wrap_pa_ctrl_START (8)
#define SOC_NPU_SCTRL_npu_ctrl19_npu_wrap_pa_ctrl_END (13)
#define SOC_NPU_SCTRL_npu_ctrl19_npu_wrap_pa_msg_START (16)
#define SOC_NPU_SCTRL_npu_ctrl19_npu_wrap_pa_msg_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int npu_wrap_tidm_en : 1;
        unsigned int reserved_0 : 3;
        unsigned int npu_wrap_tihpm_mode : 3;
        unsigned int reserved_1 : 1;
        unsigned int npu_wrap_tidm_div : 6;
        unsigned int reserved_2 : 18;
    } reg;
} SOC_NPU_SCTRL_npu_ctrl20_UNION;
#endif
#define SOC_NPU_SCTRL_npu_ctrl20_npu_wrap_tidm_en_START (0)
#define SOC_NPU_SCTRL_npu_ctrl20_npu_wrap_tidm_en_END (0)
#define SOC_NPU_SCTRL_npu_ctrl20_npu_wrap_tihpm_mode_START (4)
#define SOC_NPU_SCTRL_npu_ctrl20_npu_wrap_tihpm_mode_END (6)
#define SOC_NPU_SCTRL_npu_ctrl20_npu_wrap_tidm_div_START (8)
#define SOC_NPU_SCTRL_npu_ctrl20_npu_wrap_tidm_div_END (13)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int npu_pi_en : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_NPU_SCTRL_npu_ctrl21_UNION;
#endif
#define SOC_NPU_SCTRL_npu_ctrl21_npu_pi_en_START (0)
#define SOC_NPU_SCTRL_npu_ctrl21_npu_pi_en_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int npu_pi_prot_cnt_th : 16;
        unsigned int reserved : 16;
    } reg;
} SOC_NPU_SCTRL_npu_ctrl22_UNION;
#endif
#define SOC_NPU_SCTRL_npu_ctrl22_npu_pi_prot_cnt_th_START (0)
#define SOC_NPU_SCTRL_npu_ctrl22_npu_pi_prot_cnt_th_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int aicore0_cube_start_cfg : 16;
        unsigned int aicore1_cube_start_cfg : 16;
    } reg;
} SOC_NPU_SCTRL_npu_ctrl23_UNION;
#endif
#define SOC_NPU_SCTRL_npu_ctrl23_aicore0_cube_start_cfg_START (0)
#define SOC_NPU_SCTRL_npu_ctrl23_aicore0_cube_start_cfg_END (15)
#define SOC_NPU_SCTRL_npu_ctrl23_aicore1_cube_start_cfg_START (16)
#define SOC_NPU_SCTRL_npu_ctrl23_aicore1_cube_start_cfg_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int cube_start_pulse_width_cfg : 4;
        unsigned int reserved : 28;
    } reg;
} SOC_NPU_SCTRL_npu_ctrl24_UNION;
#endif
#define SOC_NPU_SCTRL_npu_ctrl24_cube_start_pulse_width_cfg_START (0)
#define SOC_NPU_SCTRL_npu_ctrl24_cube_start_pulse_width_cfg_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int npu_stat_bwc_saturation : 14;
        unsigned int reserved_0 : 2;
        unsigned int npu_stat_bwc_bandwidth : 13;
        unsigned int reserved_1 : 2;
        unsigned int npu_stat_bwc_en : 1;
    } reg;
} SOC_NPU_SCTRL_npu_ctrl25_UNION;
#endif
#define SOC_NPU_SCTRL_npu_ctrl25_npu_stat_bwc_saturation_START (0)
#define SOC_NPU_SCTRL_npu_ctrl25_npu_stat_bwc_saturation_END (13)
#define SOC_NPU_SCTRL_npu_ctrl25_npu_stat_bwc_bandwidth_START (16)
#define SOC_NPU_SCTRL_npu_ctrl25_npu_stat_bwc_bandwidth_END (28)
#define SOC_NPU_SCTRL_npu_ctrl25_npu_stat_bwc_en_START (31)
#define SOC_NPU_SCTRL_npu_ctrl25_npu_stat_bwc_en_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ovc_det_en0 : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_NPU_SCTRL_npu_ctrl26_UNION;
#endif
#define SOC_NPU_SCTRL_npu_ctrl26_ovc_det_en0_START (0)
#define SOC_NPU_SCTRL_npu_ctrl26_ovc_det_en0_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ovc_det_en1 : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_NPU_SCTRL_npu_ctrl27_UNION;
#endif
#define SOC_NPU_SCTRL_npu_ctrl27_ovc_det_en1_START (0)
#define SOC_NPU_SCTRL_npu_ctrl27_ovc_det_en1_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ovc_det_en2 : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_NPU_SCTRL_npu_ctrl28_UNION;
#endif
#define SOC_NPU_SCTRL_npu_ctrl28_ovc_det_en2_START (0)
#define SOC_NPU_SCTRL_npu_ctrl28_ovc_det_en2_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ovc_int_det_th : 32;
    } reg;
} SOC_NPU_SCTRL_npu_ctrl29_UNION;
#endif
#define SOC_NPU_SCTRL_npu_ctrl29_ovc_int_det_th_START (0)
#define SOC_NPU_SCTRL_npu_ctrl29_ovc_int_det_th_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ovc_pcr_det_th0 : 32;
    } reg;
} SOC_NPU_SCTRL_npu_ctrl30_UNION;
#endif
#define SOC_NPU_SCTRL_npu_ctrl30_ovc_pcr_det_th0_START (0)
#define SOC_NPU_SCTRL_npu_ctrl30_ovc_pcr_det_th0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ovc_pcr_det_th1 : 32;
    } reg;
} SOC_NPU_SCTRL_npu_ctrl31_UNION;
#endif
#define SOC_NPU_SCTRL_npu_ctrl31_ovc_pcr_det_th1_START (0)
#define SOC_NPU_SCTRL_npu_ctrl31_ovc_pcr_det_th1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ovc_pcr_det_th2 : 32;
    } reg;
} SOC_NPU_SCTRL_npu_ctrl32_UNION;
#endif
#define SOC_NPU_SCTRL_npu_ctrl32_ovc_pcr_det_th2_START (0)
#define SOC_NPU_SCTRL_npu_ctrl32_ovc_pcr_det_th2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ovc_pcr_det_th3 : 32;
    } reg;
} SOC_NPU_SCTRL_npu_ctrl33_UNION;
#endif
#define SOC_NPU_SCTRL_npu_ctrl33_ovc_pcr_det_th3_START (0)
#define SOC_NPU_SCTRL_npu_ctrl33_ovc_pcr_det_th3_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int map_sw_sample_th : 8;
        unsigned int reserved : 24;
    } reg;
} SOC_NPU_SCTRL_npu_ctrl34_UNION;
#endif
#define SOC_NPU_SCTRL_npu_ctrl34_map_sw_sample_th_START (0)
#define SOC_NPU_SCTRL_npu_ctrl34_map_sw_sample_th_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ovc_int_mask : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_NPU_SCTRL_npu_ctrl35_UNION;
#endif
#define SOC_NPU_SCTRL_npu_ctrl35_ovc_int_mask_START (0)
#define SOC_NPU_SCTRL_npu_ctrl35_ovc_int_mask_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ovc_int_clr : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_NPU_SCTRL_npu_ctrl36_UNION;
#endif
#define SOC_NPU_SCTRL_npu_ctrl36_ovc_int_clr_START (0)
#define SOC_NPU_SCTRL_npu_ctrl36_ovc_int_clr_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int easc0_int_msk : 1;
        unsigned int easc1_int_msk : 1;
        unsigned int easc2_int_msk : 1;
        unsigned int easc3_int_msk : 1;
        unsigned int idsc_int_msk : 1;
        unsigned int aicore0_rst_int_msk : 1;
        unsigned int aicore0_rst_rls_int_msk : 1;
        unsigned int aicore1_rst_int_msk : 1;
        unsigned int aicore1_rst_rls_int_msk : 1;
        unsigned int tiny_rst_int_msk : 1;
        unsigned int tiny_rst_rls_int_msk : 1;
        unsigned int wdog_res_ts_int_msk : 1;
        unsigned int easc0_res_int_msk : 1;
        unsigned int easc0_res_rls_int_msk : 1;
        unsigned int easc1_res_int_msk : 1;
        unsigned int easc1_res_rls_int_msk : 1;
        unsigned int easc2_res_int_msk : 1;
        unsigned int easc2_res_rls_int_msk : 1;
        unsigned int easc3_res_int_msk : 1;
        unsigned int easc3_res_rls_int_msk : 1;
        unsigned int idsc_res_int_msk : 1;
        unsigned int idsc_res_rls_int_msk : 1;
        unsigned int reserved : 10;
    } reg;
} SOC_NPU_SCTRL_npu_ctrl37_UNION;
#endif
#define SOC_NPU_SCTRL_npu_ctrl37_easc0_int_msk_START (0)
#define SOC_NPU_SCTRL_npu_ctrl37_easc0_int_msk_END (0)
#define SOC_NPU_SCTRL_npu_ctrl37_easc1_int_msk_START (1)
#define SOC_NPU_SCTRL_npu_ctrl37_easc1_int_msk_END (1)
#define SOC_NPU_SCTRL_npu_ctrl37_easc2_int_msk_START (2)
#define SOC_NPU_SCTRL_npu_ctrl37_easc2_int_msk_END (2)
#define SOC_NPU_SCTRL_npu_ctrl37_easc3_int_msk_START (3)
#define SOC_NPU_SCTRL_npu_ctrl37_easc3_int_msk_END (3)
#define SOC_NPU_SCTRL_npu_ctrl37_idsc_int_msk_START (4)
#define SOC_NPU_SCTRL_npu_ctrl37_idsc_int_msk_END (4)
#define SOC_NPU_SCTRL_npu_ctrl37_aicore0_rst_int_msk_START (5)
#define SOC_NPU_SCTRL_npu_ctrl37_aicore0_rst_int_msk_END (5)
#define SOC_NPU_SCTRL_npu_ctrl37_aicore0_rst_rls_int_msk_START (6)
#define SOC_NPU_SCTRL_npu_ctrl37_aicore0_rst_rls_int_msk_END (6)
#define SOC_NPU_SCTRL_npu_ctrl37_aicore1_rst_int_msk_START (7)
#define SOC_NPU_SCTRL_npu_ctrl37_aicore1_rst_int_msk_END (7)
#define SOC_NPU_SCTRL_npu_ctrl37_aicore1_rst_rls_int_msk_START (8)
#define SOC_NPU_SCTRL_npu_ctrl37_aicore1_rst_rls_int_msk_END (8)
#define SOC_NPU_SCTRL_npu_ctrl37_tiny_rst_int_msk_START (9)
#define SOC_NPU_SCTRL_npu_ctrl37_tiny_rst_int_msk_END (9)
#define SOC_NPU_SCTRL_npu_ctrl37_tiny_rst_rls_int_msk_START (10)
#define SOC_NPU_SCTRL_npu_ctrl37_tiny_rst_rls_int_msk_END (10)
#define SOC_NPU_SCTRL_npu_ctrl37_wdog_res_ts_int_msk_START (11)
#define SOC_NPU_SCTRL_npu_ctrl37_wdog_res_ts_int_msk_END (11)
#define SOC_NPU_SCTRL_npu_ctrl37_easc0_res_int_msk_START (12)
#define SOC_NPU_SCTRL_npu_ctrl37_easc0_res_int_msk_END (12)
#define SOC_NPU_SCTRL_npu_ctrl37_easc0_res_rls_int_msk_START (13)
#define SOC_NPU_SCTRL_npu_ctrl37_easc0_res_rls_int_msk_END (13)
#define SOC_NPU_SCTRL_npu_ctrl37_easc1_res_int_msk_START (14)
#define SOC_NPU_SCTRL_npu_ctrl37_easc1_res_int_msk_END (14)
#define SOC_NPU_SCTRL_npu_ctrl37_easc1_res_rls_int_msk_START (15)
#define SOC_NPU_SCTRL_npu_ctrl37_easc1_res_rls_int_msk_END (15)
#define SOC_NPU_SCTRL_npu_ctrl37_easc2_res_int_msk_START (16)
#define SOC_NPU_SCTRL_npu_ctrl37_easc2_res_int_msk_END (16)
#define SOC_NPU_SCTRL_npu_ctrl37_easc2_res_rls_int_msk_START (17)
#define SOC_NPU_SCTRL_npu_ctrl37_easc2_res_rls_int_msk_END (17)
#define SOC_NPU_SCTRL_npu_ctrl37_easc3_res_int_msk_START (18)
#define SOC_NPU_SCTRL_npu_ctrl37_easc3_res_int_msk_END (18)
#define SOC_NPU_SCTRL_npu_ctrl37_easc3_res_rls_int_msk_START (19)
#define SOC_NPU_SCTRL_npu_ctrl37_easc3_res_rls_int_msk_END (19)
#define SOC_NPU_SCTRL_npu_ctrl37_idsc_res_int_msk_START (20)
#define SOC_NPU_SCTRL_npu_ctrl37_idsc_res_int_msk_END (20)
#define SOC_NPU_SCTRL_npu_ctrl37_idsc_res_rls_int_msk_START (21)
#define SOC_NPU_SCTRL_npu_ctrl37_idsc_res_rls_int_msk_END (21)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int idsc_rst_int_clr : 1;
        unsigned int idsc_rst_rls_int_clr : 1;
        unsigned int easc3_rst_int_clr : 1;
        unsigned int easc3_rst_rls_int_clr : 1;
        unsigned int easc2_rst_int_clr : 1;
        unsigned int easc2_rst_rls_int_clr : 1;
        unsigned int easc1_rst_int_clr : 1;
        unsigned int easc1_rst_rls_int_clr : 1;
        unsigned int easc0_rst_int_clr : 1;
        unsigned int easc0_rst_rls_int_clr : 1;
        unsigned int aicore0_rst_int_clr : 1;
        unsigned int aicore0_rst_rls_int_clr : 1;
        unsigned int aicore1_rst_int_clr : 1;
        unsigned int aicore1_rst_rls_int_clr : 1;
        unsigned int tiny_rst_int_clr : 1;
        unsigned int tiny_rst_rls_int_clr : 1;
        unsigned int reserved : 16;
    } reg;
} SOC_NPU_SCTRL_npu_ctrl38_UNION;
#endif
#define SOC_NPU_SCTRL_npu_ctrl38_idsc_rst_int_clr_START (0)
#define SOC_NPU_SCTRL_npu_ctrl38_idsc_rst_int_clr_END (0)
#define SOC_NPU_SCTRL_npu_ctrl38_idsc_rst_rls_int_clr_START (1)
#define SOC_NPU_SCTRL_npu_ctrl38_idsc_rst_rls_int_clr_END (1)
#define SOC_NPU_SCTRL_npu_ctrl38_easc3_rst_int_clr_START (2)
#define SOC_NPU_SCTRL_npu_ctrl38_easc3_rst_int_clr_END (2)
#define SOC_NPU_SCTRL_npu_ctrl38_easc3_rst_rls_int_clr_START (3)
#define SOC_NPU_SCTRL_npu_ctrl38_easc3_rst_rls_int_clr_END (3)
#define SOC_NPU_SCTRL_npu_ctrl38_easc2_rst_int_clr_START (4)
#define SOC_NPU_SCTRL_npu_ctrl38_easc2_rst_int_clr_END (4)
#define SOC_NPU_SCTRL_npu_ctrl38_easc2_rst_rls_int_clr_START (5)
#define SOC_NPU_SCTRL_npu_ctrl38_easc2_rst_rls_int_clr_END (5)
#define SOC_NPU_SCTRL_npu_ctrl38_easc1_rst_int_clr_START (6)
#define SOC_NPU_SCTRL_npu_ctrl38_easc1_rst_int_clr_END (6)
#define SOC_NPU_SCTRL_npu_ctrl38_easc1_rst_rls_int_clr_START (7)
#define SOC_NPU_SCTRL_npu_ctrl38_easc1_rst_rls_int_clr_END (7)
#define SOC_NPU_SCTRL_npu_ctrl38_easc0_rst_int_clr_START (8)
#define SOC_NPU_SCTRL_npu_ctrl38_easc0_rst_int_clr_END (8)
#define SOC_NPU_SCTRL_npu_ctrl38_easc0_rst_rls_int_clr_START (9)
#define SOC_NPU_SCTRL_npu_ctrl38_easc0_rst_rls_int_clr_END (9)
#define SOC_NPU_SCTRL_npu_ctrl38_aicore0_rst_int_clr_START (10)
#define SOC_NPU_SCTRL_npu_ctrl38_aicore0_rst_int_clr_END (10)
#define SOC_NPU_SCTRL_npu_ctrl38_aicore0_rst_rls_int_clr_START (11)
#define SOC_NPU_SCTRL_npu_ctrl38_aicore0_rst_rls_int_clr_END (11)
#define SOC_NPU_SCTRL_npu_ctrl38_aicore1_rst_int_clr_START (12)
#define SOC_NPU_SCTRL_npu_ctrl38_aicore1_rst_int_clr_END (12)
#define SOC_NPU_SCTRL_npu_ctrl38_aicore1_rst_rls_int_clr_START (13)
#define SOC_NPU_SCTRL_npu_ctrl38_aicore1_rst_rls_int_clr_END (13)
#define SOC_NPU_SCTRL_npu_ctrl38_tiny_rst_int_clr_START (14)
#define SOC_NPU_SCTRL_npu_ctrl38_tiny_rst_int_clr_END (14)
#define SOC_NPU_SCTRL_npu_ctrl38_tiny_rst_rls_int_clr_START (15)
#define SOC_NPU_SCTRL_npu_ctrl38_tiny_rst_rls_int_clr_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hpm0_opc_min_en : 1;
        unsigned int hpmx0_opc_min_en : 1;
        unsigned int hpm1_opc_min_en : 1;
        unsigned int hpmx1_opc_min_en : 1;
        unsigned int hpm2_opc_min_en : 1;
        unsigned int hpmx2_opc_min_en : 1;
        unsigned int hpm3_opc_min_en : 1;
        unsigned int hpmx3_opc_min_en : 1;
        unsigned int reserved : 24;
    } reg;
} SOC_NPU_SCTRL_npu_ctrl39_UNION;
#endif
#define SOC_NPU_SCTRL_npu_ctrl39_hpm0_opc_min_en_START (0)
#define SOC_NPU_SCTRL_npu_ctrl39_hpm0_opc_min_en_END (0)
#define SOC_NPU_SCTRL_npu_ctrl39_hpmx0_opc_min_en_START (1)
#define SOC_NPU_SCTRL_npu_ctrl39_hpmx0_opc_min_en_END (1)
#define SOC_NPU_SCTRL_npu_ctrl39_hpm1_opc_min_en_START (2)
#define SOC_NPU_SCTRL_npu_ctrl39_hpm1_opc_min_en_END (2)
#define SOC_NPU_SCTRL_npu_ctrl39_hpmx1_opc_min_en_START (3)
#define SOC_NPU_SCTRL_npu_ctrl39_hpmx1_opc_min_en_END (3)
#define SOC_NPU_SCTRL_npu_ctrl39_hpm2_opc_min_en_START (4)
#define SOC_NPU_SCTRL_npu_ctrl39_hpm2_opc_min_en_END (4)
#define SOC_NPU_SCTRL_npu_ctrl39_hpmx2_opc_min_en_START (5)
#define SOC_NPU_SCTRL_npu_ctrl39_hpmx2_opc_min_en_END (5)
#define SOC_NPU_SCTRL_npu_ctrl39_hpm3_opc_min_en_START (6)
#define SOC_NPU_SCTRL_npu_ctrl39_hpm3_opc_min_en_END (6)
#define SOC_NPU_SCTRL_npu_ctrl39_hpmx3_opc_min_en_START (7)
#define SOC_NPU_SCTRL_npu_ctrl39_hpmx3_opc_min_en_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ec_cfg : 32;
    } reg;
} SOC_NPU_SCTRL_npu_ctrl40_UNION;
#endif
#define SOC_NPU_SCTRL_npu_ctrl40_ec_cfg_START (0)
#define SOC_NPU_SCTRL_npu_ctrl40_ec_cfg_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int adb_slv_gt_status_asi14 : 1;
        unsigned int adb_slv_gt_status_asi13 : 1;
        unsigned int adb_slv_gt_status_asi12 : 1;
        unsigned int adb_slv_gt_status_asi11 : 1;
        unsigned int atgs_lp_active_asi14 : 1;
        unsigned int atgs_lp_active_asi13 : 1;
        unsigned int atgs_lp_active_asi12 : 1;
        unsigned int atgs_lp_active_asi11 : 1;
        unsigned int etf_full_npu_cssys : 1;
        unsigned int reserved : 23;
    } reg;
} SOC_NPU_SCTRL_npu_stat0_UNION;
#endif
#define SOC_NPU_SCTRL_npu_stat0_adb_slv_gt_status_asi14_START (0)
#define SOC_NPU_SCTRL_npu_stat0_adb_slv_gt_status_asi14_END (0)
#define SOC_NPU_SCTRL_npu_stat0_adb_slv_gt_status_asi13_START (1)
#define SOC_NPU_SCTRL_npu_stat0_adb_slv_gt_status_asi13_END (1)
#define SOC_NPU_SCTRL_npu_stat0_adb_slv_gt_status_asi12_START (2)
#define SOC_NPU_SCTRL_npu_stat0_adb_slv_gt_status_asi12_END (2)
#define SOC_NPU_SCTRL_npu_stat0_adb_slv_gt_status_asi11_START (3)
#define SOC_NPU_SCTRL_npu_stat0_adb_slv_gt_status_asi11_END (3)
#define SOC_NPU_SCTRL_npu_stat0_atgs_lp_active_asi14_START (4)
#define SOC_NPU_SCTRL_npu_stat0_atgs_lp_active_asi14_END (4)
#define SOC_NPU_SCTRL_npu_stat0_atgs_lp_active_asi13_START (5)
#define SOC_NPU_SCTRL_npu_stat0_atgs_lp_active_asi13_END (5)
#define SOC_NPU_SCTRL_npu_stat0_atgs_lp_active_asi12_START (6)
#define SOC_NPU_SCTRL_npu_stat0_atgs_lp_active_asi12_END (6)
#define SOC_NPU_SCTRL_npu_stat0_atgs_lp_active_asi11_START (7)
#define SOC_NPU_SCTRL_npu_stat0_atgs_lp_active_asi11_END (7)
#define SOC_NPU_SCTRL_npu_stat0_etf_full_npu_cssys_START (8)
#define SOC_NPU_SCTRL_npu_stat0_etf_full_npu_cssys_END (8)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hpm0_opc : 10;
        unsigned int hpm0_opc_vld : 1;
        unsigned int reserved_0 : 5;
        unsigned int hpmx0_opc : 10;
        unsigned int hpmx0_opc_vld : 1;
        unsigned int reserved_1 : 5;
    } reg;
} SOC_NPU_SCTRL_npu_stat1_UNION;
#endif
#define SOC_NPU_SCTRL_npu_stat1_hpm0_opc_START (0)
#define SOC_NPU_SCTRL_npu_stat1_hpm0_opc_END (9)
#define SOC_NPU_SCTRL_npu_stat1_hpm0_opc_vld_START (10)
#define SOC_NPU_SCTRL_npu_stat1_hpm0_opc_vld_END (10)
#define SOC_NPU_SCTRL_npu_stat1_hpmx0_opc_START (16)
#define SOC_NPU_SCTRL_npu_stat1_hpmx0_opc_END (25)
#define SOC_NPU_SCTRL_npu_stat1_hpmx0_opc_vld_START (26)
#define SOC_NPU_SCTRL_npu_stat1_hpmx0_opc_vld_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hpm0_opc_min : 10;
        unsigned int reserved_0 : 6;
        unsigned int hpmx0_opc_min : 10;
        unsigned int reserved_1 : 6;
    } reg;
} SOC_NPU_SCTRL_npu_stat2_UNION;
#endif
#define SOC_NPU_SCTRL_npu_stat2_hpm0_opc_min_START (0)
#define SOC_NPU_SCTRL_npu_stat2_hpm0_opc_min_END (9)
#define SOC_NPU_SCTRL_npu_stat2_hpmx0_opc_min_START (16)
#define SOC_NPU_SCTRL_npu_stat2_hpmx0_opc_min_END (25)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hpm1_opc : 10;
        unsigned int hpm1_opc_vld : 1;
        unsigned int reserved_0 : 5;
        unsigned int hpmx1_opc : 10;
        unsigned int hpmx1_opc_vld : 1;
        unsigned int reserved_1 : 5;
    } reg;
} SOC_NPU_SCTRL_npu_stat3_UNION;
#endif
#define SOC_NPU_SCTRL_npu_stat3_hpm1_opc_START (0)
#define SOC_NPU_SCTRL_npu_stat3_hpm1_opc_END (9)
#define SOC_NPU_SCTRL_npu_stat3_hpm1_opc_vld_START (10)
#define SOC_NPU_SCTRL_npu_stat3_hpm1_opc_vld_END (10)
#define SOC_NPU_SCTRL_npu_stat3_hpmx1_opc_START (16)
#define SOC_NPU_SCTRL_npu_stat3_hpmx1_opc_END (25)
#define SOC_NPU_SCTRL_npu_stat3_hpmx1_opc_vld_START (26)
#define SOC_NPU_SCTRL_npu_stat3_hpmx1_opc_vld_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hpm1_opc_min : 10;
        unsigned int reserved_0 : 6;
        unsigned int hpmx1_opc_min : 10;
        unsigned int reserved_1 : 6;
    } reg;
} SOC_NPU_SCTRL_npu_stat4_UNION;
#endif
#define SOC_NPU_SCTRL_npu_stat4_hpm1_opc_min_START (0)
#define SOC_NPU_SCTRL_npu_stat4_hpm1_opc_min_END (9)
#define SOC_NPU_SCTRL_npu_stat4_hpmx1_opc_min_START (16)
#define SOC_NPU_SCTRL_npu_stat4_hpmx1_opc_min_END (25)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hpm2_opc : 10;
        unsigned int hpm2_opc_vld : 1;
        unsigned int reserved_0 : 5;
        unsigned int hpmx2_opc : 10;
        unsigned int hpmx2_opc_vld : 1;
        unsigned int reserved_1 : 5;
    } reg;
} SOC_NPU_SCTRL_npu_stat5_UNION;
#endif
#define SOC_NPU_SCTRL_npu_stat5_hpm2_opc_START (0)
#define SOC_NPU_SCTRL_npu_stat5_hpm2_opc_END (9)
#define SOC_NPU_SCTRL_npu_stat5_hpm2_opc_vld_START (10)
#define SOC_NPU_SCTRL_npu_stat5_hpm2_opc_vld_END (10)
#define SOC_NPU_SCTRL_npu_stat5_hpmx2_opc_START (16)
#define SOC_NPU_SCTRL_npu_stat5_hpmx2_opc_END (25)
#define SOC_NPU_SCTRL_npu_stat5_hpmx2_opc_vld_START (26)
#define SOC_NPU_SCTRL_npu_stat5_hpmx2_opc_vld_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hpm2_opc_min : 10;
        unsigned int reserved_0 : 6;
        unsigned int hpmx2_opc_min : 10;
        unsigned int reserved_1 : 6;
    } reg;
} SOC_NPU_SCTRL_npu_stat6_UNION;
#endif
#define SOC_NPU_SCTRL_npu_stat6_hpm2_opc_min_START (0)
#define SOC_NPU_SCTRL_npu_stat6_hpm2_opc_min_END (9)
#define SOC_NPU_SCTRL_npu_stat6_hpmx2_opc_min_START (16)
#define SOC_NPU_SCTRL_npu_stat6_hpmx2_opc_min_END (25)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hpm3_opc : 10;
        unsigned int hpm3_opc_vld : 1;
        unsigned int reserved_0 : 5;
        unsigned int hpmx3_opc : 10;
        unsigned int hpmx3_opc_vld : 1;
        unsigned int reserved_1 : 5;
    } reg;
} SOC_NPU_SCTRL_npu_stat7_UNION;
#endif
#define SOC_NPU_SCTRL_npu_stat7_hpm3_opc_START (0)
#define SOC_NPU_SCTRL_npu_stat7_hpm3_opc_END (9)
#define SOC_NPU_SCTRL_npu_stat7_hpm3_opc_vld_START (10)
#define SOC_NPU_SCTRL_npu_stat7_hpm3_opc_vld_END (10)
#define SOC_NPU_SCTRL_npu_stat7_hpmx3_opc_START (16)
#define SOC_NPU_SCTRL_npu_stat7_hpmx3_opc_END (25)
#define SOC_NPU_SCTRL_npu_stat7_hpmx3_opc_vld_START (26)
#define SOC_NPU_SCTRL_npu_stat7_hpmx3_opc_vld_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hpm3_opc_min : 10;
        unsigned int reserved_0 : 6;
        unsigned int hpmx3_opc_min : 10;
        unsigned int reserved_1 : 6;
    } reg;
} SOC_NPU_SCTRL_npu_stat8_UNION;
#endif
#define SOC_NPU_SCTRL_npu_stat8_hpm3_opc_min_START (0)
#define SOC_NPU_SCTRL_npu_stat8_hpm3_opc_min_END (9)
#define SOC_NPU_SCTRL_npu_stat8_hpmx3_opc_min_START (16)
#define SOC_NPU_SCTRL_npu_stat8_hpmx3_opc_min_END (25)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int npu_wrap0_pasensor_hci_a_out : 9;
        unsigned int reserved_0 : 3;
        unsigned int npu_wrap0_pasensor_hci_o_out : 9;
        unsigned int reserved_1 : 3;
        unsigned int npu_wrap0_pasensor_valid : 1;
        unsigned int reserved_2 : 3;
        unsigned int npu_wrap0_pasensor_cfg_ready : 1;
        unsigned int reserved_3 : 3;
    } reg;
} SOC_NPU_SCTRL_npu_stat9_UNION;
#endif
#define SOC_NPU_SCTRL_npu_stat9_npu_wrap0_pasensor_hci_a_out_START (0)
#define SOC_NPU_SCTRL_npu_stat9_npu_wrap0_pasensor_hci_a_out_END (8)
#define SOC_NPU_SCTRL_npu_stat9_npu_wrap0_pasensor_hci_o_out_START (12)
#define SOC_NPU_SCTRL_npu_stat9_npu_wrap0_pasensor_hci_o_out_END (20)
#define SOC_NPU_SCTRL_npu_stat9_npu_wrap0_pasensor_valid_START (24)
#define SOC_NPU_SCTRL_npu_stat9_npu_wrap0_pasensor_valid_END (24)
#define SOC_NPU_SCTRL_npu_stat9_npu_wrap0_pasensor_cfg_ready_START (28)
#define SOC_NPU_SCTRL_npu_stat9_npu_wrap0_pasensor_cfg_ready_END (28)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int npu_wrap0_pasensor_nbti_a_out : 9;
        unsigned int reserved_0 : 3;
        unsigned int npu_wrap0_pasensor_nbti_o_out : 9;
        unsigned int reserved_1 : 11;
    } reg;
} SOC_NPU_SCTRL_npu_stat10_UNION;
#endif
#define SOC_NPU_SCTRL_npu_stat10_npu_wrap0_pasensor_nbti_a_out_START (0)
#define SOC_NPU_SCTRL_npu_stat10_npu_wrap0_pasensor_nbti_a_out_END (8)
#define SOC_NPU_SCTRL_npu_stat10_npu_wrap0_pasensor_nbti_o_out_START (12)
#define SOC_NPU_SCTRL_npu_stat10_npu_wrap0_pasensor_nbti_o_out_END (20)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int npu_wrap1_pasensor_hci_a_out : 9;
        unsigned int reserved_0 : 3;
        unsigned int npu_wrap1_pasensor_hci_o_out : 9;
        unsigned int reserved_1 : 3;
        unsigned int npu_wrap1_pasensor_valid : 1;
        unsigned int reserved_2 : 3;
        unsigned int npu_wrap1_pasensor_cfg_ready : 1;
        unsigned int reserved_3 : 3;
    } reg;
} SOC_NPU_SCTRL_npu_stat11_UNION;
#endif
#define SOC_NPU_SCTRL_npu_stat11_npu_wrap1_pasensor_hci_a_out_START (0)
#define SOC_NPU_SCTRL_npu_stat11_npu_wrap1_pasensor_hci_a_out_END (8)
#define SOC_NPU_SCTRL_npu_stat11_npu_wrap1_pasensor_hci_o_out_START (12)
#define SOC_NPU_SCTRL_npu_stat11_npu_wrap1_pasensor_hci_o_out_END (20)
#define SOC_NPU_SCTRL_npu_stat11_npu_wrap1_pasensor_valid_START (24)
#define SOC_NPU_SCTRL_npu_stat11_npu_wrap1_pasensor_valid_END (24)
#define SOC_NPU_SCTRL_npu_stat11_npu_wrap1_pasensor_cfg_ready_START (28)
#define SOC_NPU_SCTRL_npu_stat11_npu_wrap1_pasensor_cfg_ready_END (28)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int npu_wrap1_pasensor_nbti_a_out : 9;
        unsigned int reserved_0 : 3;
        unsigned int npu_wrap1_pasensor_nbti_o_out : 9;
        unsigned int reserved_1 : 11;
    } reg;
} SOC_NPU_SCTRL_npu_stat12_UNION;
#endif
#define SOC_NPU_SCTRL_npu_stat12_npu_wrap1_pasensor_nbti_a_out_START (0)
#define SOC_NPU_SCTRL_npu_stat12_npu_wrap1_pasensor_nbti_a_out_END (8)
#define SOC_NPU_SCTRL_npu_stat12_npu_wrap1_pasensor_nbti_o_out_START (12)
#define SOC_NPU_SCTRL_npu_stat12_npu_wrap1_pasensor_nbti_o_out_END (20)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int npu_wrap2_pasensor_hci_a_out : 9;
        unsigned int reserved_0 : 3;
        unsigned int npu_wrap2_pasensor_hci_o_out : 9;
        unsigned int reserved_1 : 3;
        unsigned int npu_wrap2_pasensor_valid : 1;
        unsigned int reserved_2 : 3;
        unsigned int npu_wrap2_pasensor_cfg_ready : 1;
        unsigned int reserved_3 : 3;
    } reg;
} SOC_NPU_SCTRL_npu_stat13_UNION;
#endif
#define SOC_NPU_SCTRL_npu_stat13_npu_wrap2_pasensor_hci_a_out_START (0)
#define SOC_NPU_SCTRL_npu_stat13_npu_wrap2_pasensor_hci_a_out_END (8)
#define SOC_NPU_SCTRL_npu_stat13_npu_wrap2_pasensor_hci_o_out_START (12)
#define SOC_NPU_SCTRL_npu_stat13_npu_wrap2_pasensor_hci_o_out_END (20)
#define SOC_NPU_SCTRL_npu_stat13_npu_wrap2_pasensor_valid_START (24)
#define SOC_NPU_SCTRL_npu_stat13_npu_wrap2_pasensor_valid_END (24)
#define SOC_NPU_SCTRL_npu_stat13_npu_wrap2_pasensor_cfg_ready_START (28)
#define SOC_NPU_SCTRL_npu_stat13_npu_wrap2_pasensor_cfg_ready_END (28)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int npu_wrap2_pasensor_nbti_a_out : 9;
        unsigned int reserved_0 : 3;
        unsigned int npu_wrap2_pasensor_nbti_o_out : 9;
        unsigned int reserved_1 : 11;
    } reg;
} SOC_NPU_SCTRL_npu_stat14_UNION;
#endif
#define SOC_NPU_SCTRL_npu_stat14_npu_wrap2_pasensor_nbti_a_out_START (0)
#define SOC_NPU_SCTRL_npu_stat14_npu_wrap2_pasensor_nbti_a_out_END (8)
#define SOC_NPU_SCTRL_npu_stat14_npu_wrap2_pasensor_nbti_o_out_START (12)
#define SOC_NPU_SCTRL_npu_stat14_npu_wrap2_pasensor_nbti_o_out_END (20)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int npu_wrap3_pasensor_hci_a_out : 9;
        unsigned int reserved_0 : 3;
        unsigned int npu_wrap3_pasensor_hci_o_out : 9;
        unsigned int reserved_1 : 3;
        unsigned int npu_wrap3_pasensor_valid : 1;
        unsigned int reserved_2 : 3;
        unsigned int npu_wrap3_pasensor_cfg_ready : 1;
        unsigned int reserved_3 : 3;
    } reg;
} SOC_NPU_SCTRL_npu_stat15_UNION;
#endif
#define SOC_NPU_SCTRL_npu_stat15_npu_wrap3_pasensor_hci_a_out_START (0)
#define SOC_NPU_SCTRL_npu_stat15_npu_wrap3_pasensor_hci_a_out_END (8)
#define SOC_NPU_SCTRL_npu_stat15_npu_wrap3_pasensor_hci_o_out_START (12)
#define SOC_NPU_SCTRL_npu_stat15_npu_wrap3_pasensor_hci_o_out_END (20)
#define SOC_NPU_SCTRL_npu_stat15_npu_wrap3_pasensor_valid_START (24)
#define SOC_NPU_SCTRL_npu_stat15_npu_wrap3_pasensor_valid_END (24)
#define SOC_NPU_SCTRL_npu_stat15_npu_wrap3_pasensor_cfg_ready_START (28)
#define SOC_NPU_SCTRL_npu_stat15_npu_wrap3_pasensor_cfg_ready_END (28)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int npu_wrap3_pasensor_nbti_a_out : 9;
        unsigned int reserved_0 : 3;
        unsigned int npu_wrap3_pasensor_nbti_o_out : 9;
        unsigned int reserved_1 : 11;
    } reg;
} SOC_NPU_SCTRL_npu_stat16_UNION;
#endif
#define SOC_NPU_SCTRL_npu_stat16_npu_wrap3_pasensor_nbti_a_out_START (0)
#define SOC_NPU_SCTRL_npu_stat16_npu_wrap3_pasensor_nbti_a_out_END (8)
#define SOC_NPU_SCTRL_npu_stat16_npu_wrap3_pasensor_nbti_o_out_START (12)
#define SOC_NPU_SCTRL_npu_stat16_npu_wrap3_pasensor_nbti_o_out_END (20)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int npu_wrap0_tidm_hpm_code : 9;
        unsigned int npu_wrap0_tidm_hpmx_code : 9;
        unsigned int npu_wrap0_tidm_tihpm_code : 9;
        unsigned int reserved : 4;
        unsigned int npu_wrap0_tidm_finish_valid : 1;
    } reg;
} SOC_NPU_SCTRL_npu_stat17_UNION;
#endif
#define SOC_NPU_SCTRL_npu_stat17_npu_wrap0_tidm_hpm_code_START (0)
#define SOC_NPU_SCTRL_npu_stat17_npu_wrap0_tidm_hpm_code_END (8)
#define SOC_NPU_SCTRL_npu_stat17_npu_wrap0_tidm_hpmx_code_START (9)
#define SOC_NPU_SCTRL_npu_stat17_npu_wrap0_tidm_hpmx_code_END (17)
#define SOC_NPU_SCTRL_npu_stat17_npu_wrap0_tidm_tihpm_code_START (18)
#define SOC_NPU_SCTRL_npu_stat17_npu_wrap0_tidm_tihpm_code_END (26)
#define SOC_NPU_SCTRL_npu_stat17_npu_wrap0_tidm_finish_valid_START (31)
#define SOC_NPU_SCTRL_npu_stat17_npu_wrap0_tidm_finish_valid_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int npu_wrap1_tidm_hpm_code : 9;
        unsigned int npu_wrap1_tidm_hpmx_code : 9;
        unsigned int npu_wrap1_tidm_tihpm_code : 9;
        unsigned int reserved : 4;
        unsigned int npu_wrap1_tidm_finish_valid : 1;
    } reg;
} SOC_NPU_SCTRL_npu_stat18_UNION;
#endif
#define SOC_NPU_SCTRL_npu_stat18_npu_wrap1_tidm_hpm_code_START (0)
#define SOC_NPU_SCTRL_npu_stat18_npu_wrap1_tidm_hpm_code_END (8)
#define SOC_NPU_SCTRL_npu_stat18_npu_wrap1_tidm_hpmx_code_START (9)
#define SOC_NPU_SCTRL_npu_stat18_npu_wrap1_tidm_hpmx_code_END (17)
#define SOC_NPU_SCTRL_npu_stat18_npu_wrap1_tidm_tihpm_code_START (18)
#define SOC_NPU_SCTRL_npu_stat18_npu_wrap1_tidm_tihpm_code_END (26)
#define SOC_NPU_SCTRL_npu_stat18_npu_wrap1_tidm_finish_valid_START (31)
#define SOC_NPU_SCTRL_npu_stat18_npu_wrap1_tidm_finish_valid_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int npu_wrap2_tidm_hpm_code : 9;
        unsigned int npu_wrap2_tidm_hpmx_code : 9;
        unsigned int npu_wrap2_tidm_tihpm_code : 9;
        unsigned int reserved : 4;
        unsigned int npu_wrap2_tidm_finish_valid : 1;
    } reg;
} SOC_NPU_SCTRL_npu_stat19_UNION;
#endif
#define SOC_NPU_SCTRL_npu_stat19_npu_wrap2_tidm_hpm_code_START (0)
#define SOC_NPU_SCTRL_npu_stat19_npu_wrap2_tidm_hpm_code_END (8)
#define SOC_NPU_SCTRL_npu_stat19_npu_wrap2_tidm_hpmx_code_START (9)
#define SOC_NPU_SCTRL_npu_stat19_npu_wrap2_tidm_hpmx_code_END (17)
#define SOC_NPU_SCTRL_npu_stat19_npu_wrap2_tidm_tihpm_code_START (18)
#define SOC_NPU_SCTRL_npu_stat19_npu_wrap2_tidm_tihpm_code_END (26)
#define SOC_NPU_SCTRL_npu_stat19_npu_wrap2_tidm_finish_valid_START (31)
#define SOC_NPU_SCTRL_npu_stat19_npu_wrap2_tidm_finish_valid_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int npu_wrap3_tidm_hpm_code : 9;
        unsigned int npu_wrap3_tidm_hpmx_code : 9;
        unsigned int npu_wrap3_tidm_tihpm_code : 9;
        unsigned int reserved : 4;
        unsigned int npu_wrap3_tidm_finish_valid : 1;
    } reg;
} SOC_NPU_SCTRL_npu_stat20_UNION;
#endif
#define SOC_NPU_SCTRL_npu_stat20_npu_wrap3_tidm_hpm_code_START (0)
#define SOC_NPU_SCTRL_npu_stat20_npu_wrap3_tidm_hpm_code_END (8)
#define SOC_NPU_SCTRL_npu_stat20_npu_wrap3_tidm_hpmx_code_START (9)
#define SOC_NPU_SCTRL_npu_stat20_npu_wrap3_tidm_hpmx_code_END (17)
#define SOC_NPU_SCTRL_npu_stat20_npu_wrap3_tidm_tihpm_code_START (18)
#define SOC_NPU_SCTRL_npu_stat20_npu_wrap3_tidm_tihpm_code_END (26)
#define SOC_NPU_SCTRL_npu_stat20_npu_wrap3_tidm_finish_valid_START (31)
#define SOC_NPU_SCTRL_npu_stat20_npu_wrap3_tidm_finish_valid_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int peri_axi_dlock_id : 8;
        unsigned int peri_axi_dlock_irq : 1;
        unsigned int peri_axi_dlock_wr : 1;
        unsigned int peri_axi_dlock_mst : 2;
        unsigned int peri_axi_dlock_slv : 3;
        unsigned int reserved : 17;
    } reg;
} SOC_NPU_SCTRL_npu_stat21_UNION;
#endif
#define SOC_NPU_SCTRL_npu_stat21_peri_axi_dlock_id_START (0)
#define SOC_NPU_SCTRL_npu_stat21_peri_axi_dlock_id_END (7)
#define SOC_NPU_SCTRL_npu_stat21_peri_axi_dlock_irq_START (8)
#define SOC_NPU_SCTRL_npu_stat21_peri_axi_dlock_irq_END (8)
#define SOC_NPU_SCTRL_npu_stat21_peri_axi_dlock_wr_START (9)
#define SOC_NPU_SCTRL_npu_stat21_peri_axi_dlock_wr_END (9)
#define SOC_NPU_SCTRL_npu_stat21_peri_axi_dlock_mst_START (10)
#define SOC_NPU_SCTRL_npu_stat21_peri_axi_dlock_mst_END (11)
#define SOC_NPU_SCTRL_npu_stat21_peri_axi_dlock_slv_START (12)
#define SOC_NPU_SCTRL_npu_stat21_peri_axi_dlock_slv_END (14)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int arc_axi_dlock_id : 6;
        unsigned int arc_axi_dlock_irq : 1;
        unsigned int arc_axi_dlock_wr : 1;
        unsigned int arc_axi_dlock_mst : 1;
        unsigned int arc_axi_dlock_slv : 2;
        unsigned int reserved : 21;
    } reg;
} SOC_NPU_SCTRL_npu_stat22_UNION;
#endif
#define SOC_NPU_SCTRL_npu_stat22_arc_axi_dlock_id_START (0)
#define SOC_NPU_SCTRL_npu_stat22_arc_axi_dlock_id_END (5)
#define SOC_NPU_SCTRL_npu_stat22_arc_axi_dlock_irq_START (6)
#define SOC_NPU_SCTRL_npu_stat22_arc_axi_dlock_irq_END (6)
#define SOC_NPU_SCTRL_npu_stat22_arc_axi_dlock_wr_START (7)
#define SOC_NPU_SCTRL_npu_stat22_arc_axi_dlock_wr_END (7)
#define SOC_NPU_SCTRL_npu_stat22_arc_axi_dlock_mst_START (8)
#define SOC_NPU_SCTRL_npu_stat22_arc_axi_dlock_mst_END (8)
#define SOC_NPU_SCTRL_npu_stat22_arc_axi_dlock_slv_START (9)
#define SOC_NPU_SCTRL_npu_stat22_arc_axi_dlock_slv_END (10)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sdma0_axi_dlock_id : 12;
        unsigned int sdma0_axi_dlock_irq : 1;
        unsigned int sdma0_axi_dlock_wr : 1;
        unsigned int sdma0_axi_dlock_mst : 1;
        unsigned int sdma0_axi_dlock_slv : 1;
        unsigned int reserved : 16;
    } reg;
} SOC_NPU_SCTRL_npu_stat23_UNION;
#endif
#define SOC_NPU_SCTRL_npu_stat23_sdma0_axi_dlock_id_START (0)
#define SOC_NPU_SCTRL_npu_stat23_sdma0_axi_dlock_id_END (11)
#define SOC_NPU_SCTRL_npu_stat23_sdma0_axi_dlock_irq_START (12)
#define SOC_NPU_SCTRL_npu_stat23_sdma0_axi_dlock_irq_END (12)
#define SOC_NPU_SCTRL_npu_stat23_sdma0_axi_dlock_wr_START (13)
#define SOC_NPU_SCTRL_npu_stat23_sdma0_axi_dlock_wr_END (13)
#define SOC_NPU_SCTRL_npu_stat23_sdma0_axi_dlock_mst_START (14)
#define SOC_NPU_SCTRL_npu_stat23_sdma0_axi_dlock_mst_END (14)
#define SOC_NPU_SCTRL_npu_stat23_sdma0_axi_dlock_slv_START (15)
#define SOC_NPU_SCTRL_npu_stat23_sdma0_axi_dlock_slv_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sdma1_axi_dlock_id : 12;
        unsigned int sdma1_axi_dlock_irq : 1;
        unsigned int sdma1_axi_dlock_wr : 1;
        unsigned int sdma1_axi_dlock_mst : 1;
        unsigned int sdma1_axi_dlock_slv : 1;
        unsigned int reserved : 16;
    } reg;
} SOC_NPU_SCTRL_npu_stat24_UNION;
#endif
#define SOC_NPU_SCTRL_npu_stat24_sdma1_axi_dlock_id_START (0)
#define SOC_NPU_SCTRL_npu_stat24_sdma1_axi_dlock_id_END (11)
#define SOC_NPU_SCTRL_npu_stat24_sdma1_axi_dlock_irq_START (12)
#define SOC_NPU_SCTRL_npu_stat24_sdma1_axi_dlock_irq_END (12)
#define SOC_NPU_SCTRL_npu_stat24_sdma1_axi_dlock_wr_START (13)
#define SOC_NPU_SCTRL_npu_stat24_sdma1_axi_dlock_wr_END (13)
#define SOC_NPU_SCTRL_npu_stat24_sdma1_axi_dlock_mst_START (14)
#define SOC_NPU_SCTRL_npu_stat24_sdma1_axi_dlock_mst_END (14)
#define SOC_NPU_SCTRL_npu_stat24_sdma1_axi_dlock_slv_START (15)
#define SOC_NPU_SCTRL_npu_stat24_sdma1_axi_dlock_slv_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ovc_rint : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_NPU_SCTRL_npu_ovc_rint_UNION;
#endif
#define SOC_NPU_SCTRL_npu_ovc_rint_ovc_rint_START (0)
#define SOC_NPU_SCTRL_npu_ovc_rint_ovc_rint_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ovc_mint : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_NPU_SCTRL_npu_ovc_mint_UNION;
#endif
#define SOC_NPU_SCTRL_npu_ovc_mint_ovc_mint_START (0)
#define SOC_NPU_SCTRL_npu_ovc_mint_ovc_mint_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int npu_lite_aicore0_ds_state : 1;
        unsigned int npu_lite_aicore1_ds_state : 1;
        unsigned int npu_tiny_aicore_ds_state : 1;
        unsigned int reserved : 29;
    } reg;
} SOC_NPU_SCTRL_npu_state23_UNION;
#endif
#define SOC_NPU_SCTRL_npu_state23_npu_lite_aicore0_ds_state_START (0)
#define SOC_NPU_SCTRL_npu_state23_npu_lite_aicore0_ds_state_END (0)
#define SOC_NPU_SCTRL_npu_state23_npu_lite_aicore1_ds_state_START (1)
#define SOC_NPU_SCTRL_npu_state23_npu_lite_aicore1_ds_state_END (1)
#define SOC_NPU_SCTRL_npu_state23_npu_tiny_aicore_ds_state_START (2)
#define SOC_NPU_SCTRL_npu_state23_npu_tiny_aicore_ds_state_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int easc0_rst_rls_int_st : 1;
        unsigned int easc0_rst_int_st : 1;
        unsigned int easc0_int_st : 1;
        unsigned int easc1_rst_rls_int_st : 1;
        unsigned int easc1_rst_int_st : 1;
        unsigned int easc1_int_st : 1;
        unsigned int easc2_rst_rls_int_st : 1;
        unsigned int easc2_rst_int_st : 1;
        unsigned int easc2_int_st : 1;
        unsigned int easc3_rst_rls_int_st : 1;
        unsigned int easc3_rst_int_st : 1;
        unsigned int easc3_int_st : 1;
        unsigned int idsc_rst_rls_int_st : 1;
        unsigned int idsc_rst_int_st : 1;
        unsigned int idsc_int_st : 1;
        unsigned int aicore0_rst_int_st : 1;
        unsigned int aicore0_rst_rls_int_st : 1;
        unsigned int aicore1_rst_int_st : 1;
        unsigned int aicore1_rst_rls_int_st : 1;
        unsigned int tiny_rst_int_st : 1;
        unsigned int tiny_rst_rls_int_st : 1;
        unsigned int wdog_res_ts_int_st : 1;
        unsigned int reserved : 10;
    } reg;
} SOC_NPU_SCTRL_npu_state24_UNION;
#endif
#define SOC_NPU_SCTRL_npu_state24_easc0_rst_rls_int_st_START (0)
#define SOC_NPU_SCTRL_npu_state24_easc0_rst_rls_int_st_END (0)
#define SOC_NPU_SCTRL_npu_state24_easc0_rst_int_st_START (1)
#define SOC_NPU_SCTRL_npu_state24_easc0_rst_int_st_END (1)
#define SOC_NPU_SCTRL_npu_state24_easc0_int_st_START (2)
#define SOC_NPU_SCTRL_npu_state24_easc0_int_st_END (2)
#define SOC_NPU_SCTRL_npu_state24_easc1_rst_rls_int_st_START (3)
#define SOC_NPU_SCTRL_npu_state24_easc1_rst_rls_int_st_END (3)
#define SOC_NPU_SCTRL_npu_state24_easc1_rst_int_st_START (4)
#define SOC_NPU_SCTRL_npu_state24_easc1_rst_int_st_END (4)
#define SOC_NPU_SCTRL_npu_state24_easc1_int_st_START (5)
#define SOC_NPU_SCTRL_npu_state24_easc1_int_st_END (5)
#define SOC_NPU_SCTRL_npu_state24_easc2_rst_rls_int_st_START (6)
#define SOC_NPU_SCTRL_npu_state24_easc2_rst_rls_int_st_END (6)
#define SOC_NPU_SCTRL_npu_state24_easc2_rst_int_st_START (7)
#define SOC_NPU_SCTRL_npu_state24_easc2_rst_int_st_END (7)
#define SOC_NPU_SCTRL_npu_state24_easc2_int_st_START (8)
#define SOC_NPU_SCTRL_npu_state24_easc2_int_st_END (8)
#define SOC_NPU_SCTRL_npu_state24_easc3_rst_rls_int_st_START (9)
#define SOC_NPU_SCTRL_npu_state24_easc3_rst_rls_int_st_END (9)
#define SOC_NPU_SCTRL_npu_state24_easc3_rst_int_st_START (10)
#define SOC_NPU_SCTRL_npu_state24_easc3_rst_int_st_END (10)
#define SOC_NPU_SCTRL_npu_state24_easc3_int_st_START (11)
#define SOC_NPU_SCTRL_npu_state24_easc3_int_st_END (11)
#define SOC_NPU_SCTRL_npu_state24_idsc_rst_rls_int_st_START (12)
#define SOC_NPU_SCTRL_npu_state24_idsc_rst_rls_int_st_END (12)
#define SOC_NPU_SCTRL_npu_state24_idsc_rst_int_st_START (13)
#define SOC_NPU_SCTRL_npu_state24_idsc_rst_int_st_END (13)
#define SOC_NPU_SCTRL_npu_state24_idsc_int_st_START (14)
#define SOC_NPU_SCTRL_npu_state24_idsc_int_st_END (14)
#define SOC_NPU_SCTRL_npu_state24_aicore0_rst_int_st_START (15)
#define SOC_NPU_SCTRL_npu_state24_aicore0_rst_int_st_END (15)
#define SOC_NPU_SCTRL_npu_state24_aicore0_rst_rls_int_st_START (16)
#define SOC_NPU_SCTRL_npu_state24_aicore0_rst_rls_int_st_END (16)
#define SOC_NPU_SCTRL_npu_state24_aicore1_rst_int_st_START (17)
#define SOC_NPU_SCTRL_npu_state24_aicore1_rst_int_st_END (17)
#define SOC_NPU_SCTRL_npu_state24_aicore1_rst_rls_int_st_START (18)
#define SOC_NPU_SCTRL_npu_state24_aicore1_rst_rls_int_st_END (18)
#define SOC_NPU_SCTRL_npu_state24_tiny_rst_int_st_START (19)
#define SOC_NPU_SCTRL_npu_state24_tiny_rst_int_st_END (19)
#define SOC_NPU_SCTRL_npu_state24_tiny_rst_rls_int_st_START (20)
#define SOC_NPU_SCTRL_npu_state24_tiny_rst_rls_int_st_END (20)
#define SOC_NPU_SCTRL_npu_state24_wdog_res_ts_int_st_START (21)
#define SOC_NPU_SCTRL_npu_state24_wdog_res_ts_int_st_END (21)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ec_rpt : 32;
    } reg;
} SOC_NPU_SCTRL_npu_state25_UNION;
#endif
#define SOC_NPU_SCTRL_npu_state25_ec_rpt_START (0)
#define SOC_NPU_SCTRL_npu_state25_ec_rpt_END (31)
#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
#endif
