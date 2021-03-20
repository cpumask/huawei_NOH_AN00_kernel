#ifndef __SOC_CORECRG_L_L3_INTERFACE_H__
#define __SOC_CORECRG_L_L3_INTERFACE_H__ 
#ifdef __cplusplus
    #if __cplusplus
        extern "C" {
    #endif
#endif
#ifndef __SOC_H_FOR_ASM__
#define SOC_CORECRG_L_L3_PEREN0_ADDR(base) ((base) + (0x000UL))
#define SOC_CORECRG_L_L3_PERDIS0_ADDR(base) ((base) + (0x004UL))
#define SOC_CORECRG_L_L3_PERCLKEN0_ADDR(base) ((base) + (0x008UL))
#define SOC_CORECRG_L_L3_PERSTAT0_ADDR(base) ((base) + (0x00CUL))
#define SOC_CORECRG_L_L3_PERI_STAT0_ADDR(base) ((base) + (0x060UL))
#define SOC_CORECRG_L_L3_CLKDIV0_ADDR(base) ((base) + (0x080UL))
#define SOC_CORECRG_L_L3_VS_CTRL_EN_LITTLE_ADDR(base) ((base) + (0x100UL))
#define SOC_CORECRG_L_L3_VS_CTRL_BYPASS_LITTLE_ADDR(base) ((base) + (0x104UL))
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_ADDR(base) ((base) + (0x108UL))
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_ADDR(base) ((base) + (0x10CUL))
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL1_LITTLE_ADDR(base) ((base) + (0x110UL))
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_LITTLE_ADDR(base) ((base) + (0x114UL))
#define SOC_CORECRG_L_L3_VS_CPM_DATA_STAT_LITTLE_ADDR(base) ((base) + (0x118UL))
#define SOC_CORECRG_L_L3_VS_TEST_STAT_LITTLE_ADDR(base) ((base) + (0x11CUL))
#define SOC_CORECRG_L_L3_VS_CTRL_EN_L3_ADDR(base) ((base) + (0x130UL))
#define SOC_CORECRG_L_L3_VS_CTRL_BYPASS_L3_ADDR(base) ((base) + (0x134UL))
#define SOC_CORECRG_L_L3_VS_CTRL_L3_ADDR(base) ((base) + (0x138UL))
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_ADDR(base) ((base) + (0x13CUL))
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL1_L3_ADDR(base) ((base) + (0x140UL))
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_L3_ADDR(base) ((base) + (0x144UL))
#define SOC_CORECRG_L_L3_VS_CPM_DATA_STAT_L3_ADDR(base) ((base) + (0x148UL))
#define SOC_CORECRG_L_L3_VS_TEST_STAT_L3_ADDR(base) ((base) + (0x14CUL))
#define SOC_CORECRG_L_L3_APLL0SSCCTRL_ADDR(base) ((base) + (0x1E0UL))
#define SOC_CORECRG_L_L3_APLL1SSCCTRL_ADDR(base) ((base) + (0x1E4UL))
#define SOC_CORECRG_L_L3_FCM_CLKEN_ADDR(base) ((base) + (0x200UL))
#define SOC_CORECRG_L_L3_CPUL_L3_FNPLL_STAT_SEL_ADDR(base) ((base) + (0x204UL))
#define SOC_CORECRG_L_L3_FNPLL_CFG0_ADDR(base) ((base) + (0x240UL))
#define SOC_CORECRG_L_L3_FNPLL_CFG1_ADDR(base) ((base) + (0x244UL))
#define SOC_CORECRG_L_L3_FNPLL_CFG2_ADDR(base) ((base) + (0x248UL))
#define SOC_CORECRG_L_L3_FNPLL_CFG3_ADDR(base) ((base) + (0x24cUL))
#define SOC_CORECRG_L_L3_FNPLL_CFG4_ADDR(base) ((base) + (0x250UL))
#define SOC_CORECRG_L_L3_FNPLL_CFG5_ADDR(base) ((base) + (0x254UL))
#define SOC_CORECRG_L_L3_FNPLL_STATE0_ADDR(base) ((base) + (0x270UL))
#define SOC_CORECRG_L_L3_FNPLL_STATE1_ADDR(base) ((base) + (0x274UL))
#define SOC_CORECRG_L_L3_APLL0_EXCTRL_A_ADDR(base) ((base) + (0x400UL))
#define SOC_CORECRG_L_L3_APLL0_EXCTRL_B_ADDR(base) ((base) + (0x404UL))
#define SOC_CORECRG_L_L3_APLL1_EXCTRL_A_ADDR(base) ((base) + (0x408UL))
#define SOC_CORECRG_L_L3_APLL1_EXCTRL_B_ADDR(base) ((base) + (0x40CUL))
#else
#define SOC_CORECRG_L_L3_PEREN0_ADDR(base) ((base) + (0x000))
#define SOC_CORECRG_L_L3_PERDIS0_ADDR(base) ((base) + (0x004))
#define SOC_CORECRG_L_L3_PERCLKEN0_ADDR(base) ((base) + (0x008))
#define SOC_CORECRG_L_L3_PERSTAT0_ADDR(base) ((base) + (0x00C))
#define SOC_CORECRG_L_L3_PERI_STAT0_ADDR(base) ((base) + (0x060))
#define SOC_CORECRG_L_L3_CLKDIV0_ADDR(base) ((base) + (0x080))
#define SOC_CORECRG_L_L3_VS_CTRL_EN_LITTLE_ADDR(base) ((base) + (0x100))
#define SOC_CORECRG_L_L3_VS_CTRL_BYPASS_LITTLE_ADDR(base) ((base) + (0x104))
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_ADDR(base) ((base) + (0x108))
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_ADDR(base) ((base) + (0x10C))
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL1_LITTLE_ADDR(base) ((base) + (0x110))
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_LITTLE_ADDR(base) ((base) + (0x114))
#define SOC_CORECRG_L_L3_VS_CPM_DATA_STAT_LITTLE_ADDR(base) ((base) + (0x118))
#define SOC_CORECRG_L_L3_VS_TEST_STAT_LITTLE_ADDR(base) ((base) + (0x11C))
#define SOC_CORECRG_L_L3_VS_CTRL_EN_L3_ADDR(base) ((base) + (0x130))
#define SOC_CORECRG_L_L3_VS_CTRL_BYPASS_L3_ADDR(base) ((base) + (0x134))
#define SOC_CORECRG_L_L3_VS_CTRL_L3_ADDR(base) ((base) + (0x138))
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_ADDR(base) ((base) + (0x13C))
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL1_L3_ADDR(base) ((base) + (0x140))
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_L3_ADDR(base) ((base) + (0x144))
#define SOC_CORECRG_L_L3_VS_CPM_DATA_STAT_L3_ADDR(base) ((base) + (0x148))
#define SOC_CORECRG_L_L3_VS_TEST_STAT_L3_ADDR(base) ((base) + (0x14C))
#define SOC_CORECRG_L_L3_APLL0SSCCTRL_ADDR(base) ((base) + (0x1E0))
#define SOC_CORECRG_L_L3_APLL1SSCCTRL_ADDR(base) ((base) + (0x1E4))
#define SOC_CORECRG_L_L3_FCM_CLKEN_ADDR(base) ((base) + (0x200))
#define SOC_CORECRG_L_L3_CPUL_L3_FNPLL_STAT_SEL_ADDR(base) ((base) + (0x204))
#define SOC_CORECRG_L_L3_FNPLL_CFG0_ADDR(base) ((base) + (0x240))
#define SOC_CORECRG_L_L3_FNPLL_CFG1_ADDR(base) ((base) + (0x244))
#define SOC_CORECRG_L_L3_FNPLL_CFG2_ADDR(base) ((base) + (0x248))
#define SOC_CORECRG_L_L3_FNPLL_CFG3_ADDR(base) ((base) + (0x24c))
#define SOC_CORECRG_L_L3_FNPLL_CFG4_ADDR(base) ((base) + (0x250))
#define SOC_CORECRG_L_L3_FNPLL_CFG5_ADDR(base) ((base) + (0x254))
#define SOC_CORECRG_L_L3_FNPLL_STATE0_ADDR(base) ((base) + (0x270))
#define SOC_CORECRG_L_L3_FNPLL_STATE1_ADDR(base) ((base) + (0x274))
#define SOC_CORECRG_L_L3_APLL0_EXCTRL_A_ADDR(base) ((base) + (0x400))
#define SOC_CORECRG_L_L3_APLL0_EXCTRL_B_ADDR(base) ((base) + (0x404))
#define SOC_CORECRG_L_L3_APLL1_EXCTRL_A_ADDR(base) ((base) + (0x408))
#define SOC_CORECRG_L_L3_APLL1_EXCTRL_B_ADDR(base) ((base) + (0x40C))
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_apll0_sscg : 1;
        unsigned int gt_clk_apll1_sscg : 1;
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
        unsigned int reserved_15 : 1;
        unsigned int reserved_16 : 1;
        unsigned int reserved_17 : 1;
        unsigned int reserved_18 : 1;
        unsigned int reserved_19 : 1;
        unsigned int reserved_20 : 1;
        unsigned int reserved_21 : 1;
        unsigned int reserved_22 : 1;
        unsigned int reserved_23 : 7;
    } reg;
} SOC_CORECRG_L_L3_PEREN0_UNION;
#endif
#define SOC_CORECRG_L_L3_PEREN0_gt_clk_apll0_sscg_START (0)
#define SOC_CORECRG_L_L3_PEREN0_gt_clk_apll0_sscg_END (0)
#define SOC_CORECRG_L_L3_PEREN0_gt_clk_apll1_sscg_START (1)
#define SOC_CORECRG_L_L3_PEREN0_gt_clk_apll1_sscg_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_apll0_sscg : 1;
        unsigned int gt_clk_apll1_sscg : 1;
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
        unsigned int reserved_15 : 1;
        unsigned int reserved_16 : 1;
        unsigned int reserved_17 : 1;
        unsigned int reserved_18 : 1;
        unsigned int reserved_19 : 1;
        unsigned int reserved_20 : 1;
        unsigned int reserved_21 : 1;
        unsigned int reserved_22 : 1;
        unsigned int reserved_23 : 7;
    } reg;
} SOC_CORECRG_L_L3_PERDIS0_UNION;
#endif
#define SOC_CORECRG_L_L3_PERDIS0_gt_clk_apll0_sscg_START (0)
#define SOC_CORECRG_L_L3_PERDIS0_gt_clk_apll0_sscg_END (0)
#define SOC_CORECRG_L_L3_PERDIS0_gt_clk_apll1_sscg_START (1)
#define SOC_CORECRG_L_L3_PERDIS0_gt_clk_apll1_sscg_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_apll0_sscg : 1;
        unsigned int gt_clk_apll1_sscg : 1;
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
        unsigned int reserved_15 : 1;
        unsigned int reserved_16 : 1;
        unsigned int reserved_17 : 1;
        unsigned int reserved_18 : 1;
        unsigned int reserved_19 : 1;
        unsigned int reserved_20 : 1;
        unsigned int reserved_21 : 1;
        unsigned int reserved_22 : 1;
        unsigned int reserved_23 : 7;
    } reg;
} SOC_CORECRG_L_L3_PERCLKEN0_UNION;
#endif
#define SOC_CORECRG_L_L3_PERCLKEN0_gt_clk_apll0_sscg_START (0)
#define SOC_CORECRG_L_L3_PERCLKEN0_gt_clk_apll0_sscg_END (0)
#define SOC_CORECRG_L_L3_PERCLKEN0_gt_clk_apll1_sscg_START (1)
#define SOC_CORECRG_L_L3_PERCLKEN0_gt_clk_apll1_sscg_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int st_clk_apll0_sscg : 1;
        unsigned int st_clk_apll1_sscg : 1;
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
        unsigned int reserved_15 : 1;
        unsigned int reserved_16 : 1;
        unsigned int reserved_17 : 1;
        unsigned int reserved_18 : 1;
        unsigned int reserved_19 : 1;
        unsigned int reserved_20 : 1;
        unsigned int reserved_21 : 1;
        unsigned int reserved_22 : 1;
        unsigned int reserved_23 : 7;
    } reg;
} SOC_CORECRG_L_L3_PERSTAT0_UNION;
#endif
#define SOC_CORECRG_L_L3_PERSTAT0_st_clk_apll0_sscg_START (0)
#define SOC_CORECRG_L_L3_PERSTAT0_st_clk_apll0_sscg_END (0)
#define SOC_CORECRG_L_L3_PERSTAT0_st_clk_apll1_sscg_START (1)
#define SOC_CORECRG_L_L3_PERSTAT0_st_clk_apll1_sscg_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int st_clk_cpu_little : 1;
        unsigned int st_clk_cpu_l3 : 1;
        unsigned int reserved_0 : 2;
        unsigned int reserved_1 : 2;
        unsigned int reserved_2 : 26;
    } reg;
} SOC_CORECRG_L_L3_PERI_STAT0_UNION;
#endif
#define SOC_CORECRG_L_L3_PERI_STAT0_st_clk_cpu_little_START (0)
#define SOC_CORECRG_L_L3_PERI_STAT0_st_clk_cpu_little_END (0)
#define SOC_CORECRG_L_L3_PERI_STAT0_st_clk_cpu_l3_START (1)
#define SOC_CORECRG_L_L3_PERI_STAT0_st_clk_cpu_l3_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int fn_sc_apll0_sel : 1;
        unsigned int fn_sc_apll1_sel : 1;
        unsigned int reserved_0 : 12;
        unsigned int reserved_1 : 2;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CORECRG_L_L3_CLKDIV0_UNION;
#endif
#define SOC_CORECRG_L_L3_CLKDIV0_fn_sc_apll0_sel_START (0)
#define SOC_CORECRG_L_L3_CLKDIV0_fn_sc_apll0_sel_END (0)
#define SOC_CORECRG_L_L3_CLKDIV0_fn_sc_apll1_sel_START (1)
#define SOC_CORECRG_L_L3_CLKDIV0_fn_sc_apll1_sel_END (1)
#define SOC_CORECRG_L_L3_CLKDIV0_bitmasken_START (16)
#define SOC_CORECRG_L_L3_CLKDIV0_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int vs_ctrl_en_little : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_CORECRG_L_L3_VS_CTRL_EN_LITTLE_UNION;
#endif
#define SOC_CORECRG_L_L3_VS_CTRL_EN_LITTLE_vs_ctrl_en_little_START (0)
#define SOC_CORECRG_L_L3_VS_CTRL_EN_LITTLE_vs_ctrl_en_little_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int vs_ctrl_bypass_little : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_CORECRG_L_L3_VS_CTRL_BYPASS_LITTLE_UNION;
#endif
#define SOC_CORECRG_L_L3_VS_CTRL_BYPASS_LITTLE_vs_ctrl_bypass_little_START (0)
#define SOC_CORECRG_L_L3_VS_CTRL_BYPASS_LITTLE_vs_ctrl_bypass_little_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int little_cpu0_wfi_wfe_bypass : 1;
        unsigned int little_cpu1_wfi_wfe_bypass : 1;
        unsigned int little_cpu2_wfi_wfe_bypass : 1;
        unsigned int little_cpu3_wfi_wfe_bypass : 1;
        unsigned int little_l2_idle_div_mod : 2;
        unsigned int little_cfg_cnt_cpu_wake_quit : 16;
        unsigned int little_cpu_wake_up_mode : 2;
        unsigned int little_cpu_l2_idle_switch_bypass : 1;
        unsigned int little_cpu_l2_idle_gt_en : 1;
        unsigned int little_dvfs_div_en : 1;
        unsigned int reserved_1 : 1;
        unsigned int sel_ckmux_little_icg : 1;
        unsigned int cpu_clk_div_cfg_little : 2;
    } reg;
} SOC_CORECRG_L_L3_VS_CTRL_LITTLE_UNION;
#endif
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_little_cpu0_wfi_wfe_bypass_START (1)
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_little_cpu0_wfi_wfe_bypass_END (1)
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_little_cpu1_wfi_wfe_bypass_START (2)
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_little_cpu1_wfi_wfe_bypass_END (2)
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_little_cpu2_wfi_wfe_bypass_START (3)
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_little_cpu2_wfi_wfe_bypass_END (3)
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_little_cpu3_wfi_wfe_bypass_START (4)
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_little_cpu3_wfi_wfe_bypass_END (4)
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_little_l2_idle_div_mod_START (5)
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_little_l2_idle_div_mod_END (6)
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_little_cfg_cnt_cpu_wake_quit_START (7)
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_little_cfg_cnt_cpu_wake_quit_END (22)
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_little_cpu_wake_up_mode_START (23)
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_little_cpu_wake_up_mode_END (24)
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_little_cpu_l2_idle_switch_bypass_START (25)
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_little_cpu_l2_idle_switch_bypass_END (25)
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_little_cpu_l2_idle_gt_en_START (26)
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_little_cpu_l2_idle_gt_en_END (26)
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_little_dvfs_div_en_START (27)
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_little_dvfs_div_en_END (27)
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_sel_ckmux_little_icg_START (29)
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_sel_ckmux_little_icg_END (29)
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_cpu_clk_div_cfg_little_START (30)
#define SOC_CORECRG_L_L3_VS_CTRL_LITTLE_cpu_clk_div_cfg_little_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int svfd_data_limit_e_little : 1;
        unsigned int svfd_off_time_step_little : 1;
        unsigned int svfd_pulse_width_sel_little : 1;
        unsigned int reserved_0 : 1;
        unsigned int svfd_ulvt_ll_little : 4;
        unsigned int svfd_ulvt_sl_little : 4;
        unsigned int svfd_lvt_ll_little : 4;
        unsigned int svfd_lvt_sl_little : 4;
        unsigned int svfd_trim_little : 2;
        unsigned int svfd_cmp_data_clr_l3 : 1;
        unsigned int reserved_1 : 1;
        unsigned int svfd_d_rate_little : 2;
        unsigned int svfd_off_mode_little : 1;
        unsigned int svfd_div64_en_little : 1;
        unsigned int svfd_cpm_period_little : 1;
        unsigned int svfd_edge_sel_little : 1;
        unsigned int svfd_cmp_data_mode_little : 2;
    } reg;
} SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_UNION;
#endif
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_data_limit_e_little_START (0)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_data_limit_e_little_END (0)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_off_time_step_little_START (1)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_off_time_step_little_END (1)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_pulse_width_sel_little_START (2)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_pulse_width_sel_little_END (2)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_ulvt_ll_little_START (4)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_ulvt_ll_little_END (7)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_ulvt_sl_little_START (8)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_ulvt_sl_little_END (11)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_lvt_ll_little_START (12)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_lvt_ll_little_END (15)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_lvt_sl_little_START (16)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_lvt_sl_little_END (19)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_trim_little_START (20)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_trim_little_END (21)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_cmp_data_clr_l3_START (22)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_cmp_data_clr_l3_END (22)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_d_rate_little_START (24)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_d_rate_little_END (25)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_off_mode_little_START (26)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_off_mode_little_END (26)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_div64_en_little_START (27)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_div64_en_little_END (27)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_cpm_period_little_START (28)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_cpm_period_little_END (28)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_edge_sel_little_START (29)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_edge_sel_little_END (29)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_cmp_data_mode_little_START (30)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_LITTLE_svfd_cmp_data_mode_little_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int svfd_glitch_test_little : 1;
        unsigned int reserved_0 : 3;
        unsigned int svfd_test_ffs_little : 8;
        unsigned int svfd_test_cpm_little : 8;
        unsigned int reserved_1 : 12;
    } reg;
} SOC_CORECRG_L_L3_VS_SVFD_CTRL1_LITTLE_UNION;
#endif
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL1_LITTLE_svfd_glitch_test_little_START (0)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL1_LITTLE_svfd_glitch_test_little_END (0)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL1_LITTLE_svfd_test_ffs_little_START (4)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL1_LITTLE_svfd_test_ffs_little_END (11)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL1_LITTLE_svfd_test_cpm_little_START (12)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL1_LITTLE_svfd_test_cpm_little_END (19)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int svfd_enalbe_mode_little : 2;
        unsigned int svfs_cpm_threshold_r_little : 6;
        unsigned int svfs_cpm_threshold_f_little : 6;
        unsigned int vol_drop_en_little : 1;
        unsigned int reserved : 1;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CORECRG_L_L3_VS_SVFD_CTRL2_LITTLE_UNION;
#endif
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_LITTLE_svfd_enalbe_mode_little_START (0)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_LITTLE_svfd_enalbe_mode_little_END (1)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_LITTLE_svfs_cpm_threshold_r_little_START (2)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_LITTLE_svfs_cpm_threshold_r_little_END (7)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_LITTLE_svfs_cpm_threshold_f_little_START (8)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_LITTLE_svfs_cpm_threshold_f_little_END (13)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_LITTLE_vol_drop_en_little_START (14)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_LITTLE_vol_drop_en_little_END (14)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_LITTLE_bitmasken_START (16)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_LITTLE_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int svfd_cpm_data_r_little : 8;
        unsigned int svfd_cpm_data_f_little : 8;
        unsigned int svfd_dll_lock_little : 1;
        unsigned int svfd_cpm_data_norst_little : 6;
        unsigned int reserved : 9;
    } reg;
} SOC_CORECRG_L_L3_VS_CPM_DATA_STAT_LITTLE_UNION;
#endif
#define SOC_CORECRG_L_L3_VS_CPM_DATA_STAT_LITTLE_svfd_cpm_data_r_little_START (0)
#define SOC_CORECRG_L_L3_VS_CPM_DATA_STAT_LITTLE_svfd_cpm_data_r_little_END (7)
#define SOC_CORECRG_L_L3_VS_CPM_DATA_STAT_LITTLE_svfd_cpm_data_f_little_START (8)
#define SOC_CORECRG_L_L3_VS_CPM_DATA_STAT_LITTLE_svfd_cpm_data_f_little_END (15)
#define SOC_CORECRG_L_L3_VS_CPM_DATA_STAT_LITTLE_svfd_dll_lock_little_START (16)
#define SOC_CORECRG_L_L3_VS_CPM_DATA_STAT_LITTLE_svfd_dll_lock_little_END (16)
#define SOC_CORECRG_L_L3_VS_CPM_DATA_STAT_LITTLE_svfd_cpm_data_norst_little_START (17)
#define SOC_CORECRG_L_L3_VS_CPM_DATA_STAT_LITTLE_svfd_cpm_data_norst_little_END (22)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int svfd_test_out_ffs_little : 4;
        unsigned int svfd_test_out_cpm_little : 4;
        unsigned int svfd_glitch_result_little : 1;
        unsigned int idle_low_freq_en_little : 1;
        unsigned int vbat_drop_protect_ind_little : 1;
        unsigned int reserved : 21;
    } reg;
} SOC_CORECRG_L_L3_VS_TEST_STAT_LITTLE_UNION;
#endif
#define SOC_CORECRG_L_L3_VS_TEST_STAT_LITTLE_svfd_test_out_ffs_little_START (0)
#define SOC_CORECRG_L_L3_VS_TEST_STAT_LITTLE_svfd_test_out_ffs_little_END (3)
#define SOC_CORECRG_L_L3_VS_TEST_STAT_LITTLE_svfd_test_out_cpm_little_START (4)
#define SOC_CORECRG_L_L3_VS_TEST_STAT_LITTLE_svfd_test_out_cpm_little_END (7)
#define SOC_CORECRG_L_L3_VS_TEST_STAT_LITTLE_svfd_glitch_result_little_START (8)
#define SOC_CORECRG_L_L3_VS_TEST_STAT_LITTLE_svfd_glitch_result_little_END (8)
#define SOC_CORECRG_L_L3_VS_TEST_STAT_LITTLE_idle_low_freq_en_little_START (9)
#define SOC_CORECRG_L_L3_VS_TEST_STAT_LITTLE_idle_low_freq_en_little_END (9)
#define SOC_CORECRG_L_L3_VS_TEST_STAT_LITTLE_vbat_drop_protect_ind_little_START (10)
#define SOC_CORECRG_L_L3_VS_TEST_STAT_LITTLE_vbat_drop_protect_ind_little_END (10)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int vs_ctrl_en_l3 : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_CORECRG_L_L3_VS_CTRL_EN_L3_UNION;
#endif
#define SOC_CORECRG_L_L3_VS_CTRL_EN_L3_vs_ctrl_en_l3_START (0)
#define SOC_CORECRG_L_L3_VS_CTRL_EN_L3_vs_ctrl_en_l3_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int vs_ctrl_bypass_l3 : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_CORECRG_L_L3_VS_CTRL_BYPASS_L3_UNION;
#endif
#define SOC_CORECRG_L_L3_VS_CTRL_BYPASS_L3_vs_ctrl_bypass_l3_START (0)
#define SOC_CORECRG_L_L3_VS_CTRL_BYPASS_L3_vs_ctrl_bypass_l3_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int l3_cpu0_wfi_wfe_bypass : 1;
        unsigned int l3_cpu1_wfi_wfe_bypass : 1;
        unsigned int l3_cpu2_wfi_wfe_bypass : 1;
        unsigned int l3_cpu3_wfi_wfe_bypass : 1;
        unsigned int l3_l2_idle_div_mod : 2;
        unsigned int l3_cfg_cnt_cpu_wake_quit : 16;
        unsigned int l3_cpu_wake_up_mode : 2;
        unsigned int l3_cpu_l2_idle_switch_bypass : 1;
        unsigned int l3_cpu_l2_idle_gt_en : 1;
        unsigned int l3_dvfs_div_en : 1;
        unsigned int reserved_1 : 1;
        unsigned int sel_ckmux_l3_icg : 1;
        unsigned int cpu_clk_div_cfg_l3 : 2;
    } reg;
} SOC_CORECRG_L_L3_VS_CTRL_L3_UNION;
#endif
#define SOC_CORECRG_L_L3_VS_CTRL_L3_l3_cpu0_wfi_wfe_bypass_START (1)
#define SOC_CORECRG_L_L3_VS_CTRL_L3_l3_cpu0_wfi_wfe_bypass_END (1)
#define SOC_CORECRG_L_L3_VS_CTRL_L3_l3_cpu1_wfi_wfe_bypass_START (2)
#define SOC_CORECRG_L_L3_VS_CTRL_L3_l3_cpu1_wfi_wfe_bypass_END (2)
#define SOC_CORECRG_L_L3_VS_CTRL_L3_l3_cpu2_wfi_wfe_bypass_START (3)
#define SOC_CORECRG_L_L3_VS_CTRL_L3_l3_cpu2_wfi_wfe_bypass_END (3)
#define SOC_CORECRG_L_L3_VS_CTRL_L3_l3_cpu3_wfi_wfe_bypass_START (4)
#define SOC_CORECRG_L_L3_VS_CTRL_L3_l3_cpu3_wfi_wfe_bypass_END (4)
#define SOC_CORECRG_L_L3_VS_CTRL_L3_l3_l2_idle_div_mod_START (5)
#define SOC_CORECRG_L_L3_VS_CTRL_L3_l3_l2_idle_div_mod_END (6)
#define SOC_CORECRG_L_L3_VS_CTRL_L3_l3_cfg_cnt_cpu_wake_quit_START (7)
#define SOC_CORECRG_L_L3_VS_CTRL_L3_l3_cfg_cnt_cpu_wake_quit_END (22)
#define SOC_CORECRG_L_L3_VS_CTRL_L3_l3_cpu_wake_up_mode_START (23)
#define SOC_CORECRG_L_L3_VS_CTRL_L3_l3_cpu_wake_up_mode_END (24)
#define SOC_CORECRG_L_L3_VS_CTRL_L3_l3_cpu_l2_idle_switch_bypass_START (25)
#define SOC_CORECRG_L_L3_VS_CTRL_L3_l3_cpu_l2_idle_switch_bypass_END (25)
#define SOC_CORECRG_L_L3_VS_CTRL_L3_l3_cpu_l2_idle_gt_en_START (26)
#define SOC_CORECRG_L_L3_VS_CTRL_L3_l3_cpu_l2_idle_gt_en_END (26)
#define SOC_CORECRG_L_L3_VS_CTRL_L3_l3_dvfs_div_en_START (27)
#define SOC_CORECRG_L_L3_VS_CTRL_L3_l3_dvfs_div_en_END (27)
#define SOC_CORECRG_L_L3_VS_CTRL_L3_sel_ckmux_l3_icg_START (29)
#define SOC_CORECRG_L_L3_VS_CTRL_L3_sel_ckmux_l3_icg_END (29)
#define SOC_CORECRG_L_L3_VS_CTRL_L3_cpu_clk_div_cfg_l3_START (30)
#define SOC_CORECRG_L_L3_VS_CTRL_L3_cpu_clk_div_cfg_l3_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int svfd_data_limit_e_l3 : 1;
        unsigned int svfd_off_time_step_l3 : 1;
        unsigned int svfd_pulse_width_sel_l3 : 1;
        unsigned int reserved_0 : 1;
        unsigned int svfd_ulvt_ll_l3 : 4;
        unsigned int svfd_ulvt_sl_l3 : 4;
        unsigned int svfd_lvt_ll_l3 : 4;
        unsigned int svfd_lvt_sl_l3 : 4;
        unsigned int svfd_trim_l3 : 2;
        unsigned int svfd_cmp_data_clr_l3 : 1;
        unsigned int reserved_1 : 1;
        unsigned int svfd_d_rate_l3 : 2;
        unsigned int svfd_off_mode_l3 : 1;
        unsigned int svfd_div64_en_l3 : 1;
        unsigned int svfd_cpm_period_l3 : 1;
        unsigned int svfd_edge_sel_l3 : 1;
        unsigned int svfd_cmp_data_mode_l3 : 2;
    } reg;
} SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_UNION;
#endif
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_data_limit_e_l3_START (0)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_data_limit_e_l3_END (0)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_off_time_step_l3_START (1)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_off_time_step_l3_END (1)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_pulse_width_sel_l3_START (2)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_pulse_width_sel_l3_END (2)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_ulvt_ll_l3_START (4)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_ulvt_ll_l3_END (7)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_ulvt_sl_l3_START (8)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_ulvt_sl_l3_END (11)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_lvt_ll_l3_START (12)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_lvt_ll_l3_END (15)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_lvt_sl_l3_START (16)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_lvt_sl_l3_END (19)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_trim_l3_START (20)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_trim_l3_END (21)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_cmp_data_clr_l3_START (22)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_cmp_data_clr_l3_END (22)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_d_rate_l3_START (24)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_d_rate_l3_END (25)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_off_mode_l3_START (26)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_off_mode_l3_END (26)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_div64_en_l3_START (27)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_div64_en_l3_END (27)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_cpm_period_l3_START (28)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_cpm_period_l3_END (28)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_edge_sel_l3_START (29)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_edge_sel_l3_END (29)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_cmp_data_mode_l3_START (30)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL0_L3_svfd_cmp_data_mode_l3_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int svfd_glitch_test_little : 1;
        unsigned int reserved_0 : 3;
        unsigned int svfd_test_ffs_little : 8;
        unsigned int svfd_test_cpm_little : 8;
        unsigned int reserved_1 : 12;
    } reg;
} SOC_CORECRG_L_L3_VS_SVFD_CTRL1_L3_UNION;
#endif
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL1_L3_svfd_glitch_test_little_START (0)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL1_L3_svfd_glitch_test_little_END (0)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL1_L3_svfd_test_ffs_little_START (4)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL1_L3_svfd_test_ffs_little_END (11)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL1_L3_svfd_test_cpm_little_START (12)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL1_L3_svfd_test_cpm_little_END (19)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int svfd_enalbe_mode_l3 : 2;
        unsigned int svfs_cpm_threshold_r_l3 : 6;
        unsigned int svfs_cpm_threshold_f_l3 : 6;
        unsigned int vol_drop_en_l3 : 1;
        unsigned int reserved : 1;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CORECRG_L_L3_VS_SVFD_CTRL2_L3_UNION;
#endif
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_L3_svfd_enalbe_mode_l3_START (0)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_L3_svfd_enalbe_mode_l3_END (1)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_L3_svfs_cpm_threshold_r_l3_START (2)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_L3_svfs_cpm_threshold_r_l3_END (7)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_L3_svfs_cpm_threshold_f_l3_START (8)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_L3_svfs_cpm_threshold_f_l3_END (13)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_L3_vol_drop_en_l3_START (14)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_L3_vol_drop_en_l3_END (14)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_L3_bitmasken_START (16)
#define SOC_CORECRG_L_L3_VS_SVFD_CTRL2_L3_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int svfd_cpm_data_r_l3 : 8;
        unsigned int svfd_cpm_data_f_l3 : 8;
        unsigned int svfd_dll_lock_l3 : 1;
        unsigned int svfd_cpm_data_norst_l3 : 6;
        unsigned int reserved : 9;
    } reg;
} SOC_CORECRG_L_L3_VS_CPM_DATA_STAT_L3_UNION;
#endif
#define SOC_CORECRG_L_L3_VS_CPM_DATA_STAT_L3_svfd_cpm_data_r_l3_START (0)
#define SOC_CORECRG_L_L3_VS_CPM_DATA_STAT_L3_svfd_cpm_data_r_l3_END (7)
#define SOC_CORECRG_L_L3_VS_CPM_DATA_STAT_L3_svfd_cpm_data_f_l3_START (8)
#define SOC_CORECRG_L_L3_VS_CPM_DATA_STAT_L3_svfd_cpm_data_f_l3_END (15)
#define SOC_CORECRG_L_L3_VS_CPM_DATA_STAT_L3_svfd_dll_lock_l3_START (16)
#define SOC_CORECRG_L_L3_VS_CPM_DATA_STAT_L3_svfd_dll_lock_l3_END (16)
#define SOC_CORECRG_L_L3_VS_CPM_DATA_STAT_L3_svfd_cpm_data_norst_l3_START (17)
#define SOC_CORECRG_L_L3_VS_CPM_DATA_STAT_L3_svfd_cpm_data_norst_l3_END (22)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int svfd_test_out_ffs_l3 : 4;
        unsigned int svfd_test_out_cpm_l3 : 4;
        unsigned int svfd_glitch_result_l3 : 1;
        unsigned int idle_low_freq_en_l3 : 1;
        unsigned int vbat_drop_protect_ind_l3 : 1;
        unsigned int reserved : 21;
    } reg;
} SOC_CORECRG_L_L3_VS_TEST_STAT_L3_UNION;
#endif
#define SOC_CORECRG_L_L3_VS_TEST_STAT_L3_svfd_test_out_ffs_l3_START (0)
#define SOC_CORECRG_L_L3_VS_TEST_STAT_L3_svfd_test_out_ffs_l3_END (3)
#define SOC_CORECRG_L_L3_VS_TEST_STAT_L3_svfd_test_out_cpm_l3_START (4)
#define SOC_CORECRG_L_L3_VS_TEST_STAT_L3_svfd_test_out_cpm_l3_END (7)
#define SOC_CORECRG_L_L3_VS_TEST_STAT_L3_svfd_glitch_result_l3_START (8)
#define SOC_CORECRG_L_L3_VS_TEST_STAT_L3_svfd_glitch_result_l3_END (8)
#define SOC_CORECRG_L_L3_VS_TEST_STAT_L3_idle_low_freq_en_l3_START (9)
#define SOC_CORECRG_L_L3_VS_TEST_STAT_L3_idle_low_freq_en_l3_END (9)
#define SOC_CORECRG_L_L3_VS_TEST_STAT_L3_vbat_drop_protect_ind_l3_START (10)
#define SOC_CORECRG_L_L3_VS_TEST_STAT_L3_vbat_drop_protect_ind_l3_END (10)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int apll0_ssc_reset : 1;
        unsigned int apll0_ssc_disable : 1;
        unsigned int apll0_ssc_downspread : 1;
        unsigned int apll0_ssc_spread : 3;
        unsigned int reserved_0 : 2;
        unsigned int apll0_ssc_divval : 4;
        unsigned int reserved_1 : 2;
        unsigned int apll0_ssmod_rstnptr : 1;
        unsigned int apll0_ssc_self_vld : 1;
        unsigned int reserved_2 : 16;
    } reg;
} SOC_CORECRG_L_L3_APLL0SSCCTRL_UNION;
#endif
#define SOC_CORECRG_L_L3_APLL0SSCCTRL_apll0_ssc_reset_START (0)
#define SOC_CORECRG_L_L3_APLL0SSCCTRL_apll0_ssc_reset_END (0)
#define SOC_CORECRG_L_L3_APLL0SSCCTRL_apll0_ssc_disable_START (1)
#define SOC_CORECRG_L_L3_APLL0SSCCTRL_apll0_ssc_disable_END (1)
#define SOC_CORECRG_L_L3_APLL0SSCCTRL_apll0_ssc_downspread_START (2)
#define SOC_CORECRG_L_L3_APLL0SSCCTRL_apll0_ssc_downspread_END (2)
#define SOC_CORECRG_L_L3_APLL0SSCCTRL_apll0_ssc_spread_START (3)
#define SOC_CORECRG_L_L3_APLL0SSCCTRL_apll0_ssc_spread_END (5)
#define SOC_CORECRG_L_L3_APLL0SSCCTRL_apll0_ssc_divval_START (8)
#define SOC_CORECRG_L_L3_APLL0SSCCTRL_apll0_ssc_divval_END (11)
#define SOC_CORECRG_L_L3_APLL0SSCCTRL_apll0_ssmod_rstnptr_START (14)
#define SOC_CORECRG_L_L3_APLL0SSCCTRL_apll0_ssmod_rstnptr_END (14)
#define SOC_CORECRG_L_L3_APLL0SSCCTRL_apll0_ssc_self_vld_START (15)
#define SOC_CORECRG_L_L3_APLL0SSCCTRL_apll0_ssc_self_vld_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int apll1_ssc_reset : 1;
        unsigned int apll1_ssc_disable : 1;
        unsigned int apll1_ssc_downspread : 1;
        unsigned int apll1_ssc_spread : 3;
        unsigned int reserved_0 : 2;
        unsigned int apll1_ssc_divval : 4;
        unsigned int reserved_1 : 2;
        unsigned int apll1_ssmod_rstnptr : 1;
        unsigned int apll1_ssc_self_vld : 1;
        unsigned int reserved_2 : 16;
    } reg;
} SOC_CORECRG_L_L3_APLL1SSCCTRL_UNION;
#endif
#define SOC_CORECRG_L_L3_APLL1SSCCTRL_apll1_ssc_reset_START (0)
#define SOC_CORECRG_L_L3_APLL1SSCCTRL_apll1_ssc_reset_END (0)
#define SOC_CORECRG_L_L3_APLL1SSCCTRL_apll1_ssc_disable_START (1)
#define SOC_CORECRG_L_L3_APLL1SSCCTRL_apll1_ssc_disable_END (1)
#define SOC_CORECRG_L_L3_APLL1SSCCTRL_apll1_ssc_downspread_START (2)
#define SOC_CORECRG_L_L3_APLL1SSCCTRL_apll1_ssc_downspread_END (2)
#define SOC_CORECRG_L_L3_APLL1SSCCTRL_apll1_ssc_spread_START (3)
#define SOC_CORECRG_L_L3_APLL1SSCCTRL_apll1_ssc_spread_END (5)
#define SOC_CORECRG_L_L3_APLL1SSCCTRL_apll1_ssc_divval_START (8)
#define SOC_CORECRG_L_L3_APLL1SSCCTRL_apll1_ssc_divval_END (11)
#define SOC_CORECRG_L_L3_APLL1SSCCTRL_apll1_ssmod_rstnptr_START (14)
#define SOC_CORECRG_L_L3_APLL1SSCCTRL_apll1_ssmod_rstnptr_END (14)
#define SOC_CORECRG_L_L3_APLL1SSCCTRL_apll1_ssc_self_vld_START (15)
#define SOC_CORECRG_L_L3_APLL1SSCCTRL_apll1_ssc_self_vld_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_cpu_l3 : 1;
        unsigned int gt_clk_cpu_little : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 13;
        unsigned int bitmasken : 16;
    } reg;
} SOC_CORECRG_L_L3_FCM_CLKEN_UNION;
#endif
#define SOC_CORECRG_L_L3_FCM_CLKEN_gt_clk_cpu_l3_START (0)
#define SOC_CORECRG_L_L3_FCM_CLKEN_gt_clk_cpu_l3_END (0)
#define SOC_CORECRG_L_L3_FCM_CLKEN_gt_clk_cpu_little_START (1)
#define SOC_CORECRG_L_L3_FCM_CLKEN_gt_clk_cpu_little_END (1)
#define SOC_CORECRG_L_L3_FCM_CLKEN_bitmasken_START (16)
#define SOC_CORECRG_L_L3_FCM_CLKEN_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int cpul_l3_fnpll_stat_sel : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_CORECRG_L_L3_CPUL_L3_FNPLL_STAT_SEL_UNION;
#endif
#define SOC_CORECRG_L_L3_CPUL_L3_FNPLL_STAT_SEL_cpul_l3_fnpll_stat_sel_START (0)
#define SOC_CORECRG_L_L3_CPUL_L3_FNPLL_STAT_SEL_cpul_l3_fnpll_stat_sel_END (0)
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
} SOC_CORECRG_L_L3_FNPLL_CFG0_UNION;
#endif
#define SOC_CORECRG_L_L3_FNPLL_CFG0_phe_sel_en_fnpll_1v2_START (0)
#define SOC_CORECRG_L_L3_FNPLL_CFG0_phe_sel_en_fnpll_1v2_END (0)
#define SOC_CORECRG_L_L3_FNPLL_CFG0_dtc_test_fnpll_1v2_START (1)
#define SOC_CORECRG_L_L3_FNPLL_CFG0_dtc_test_fnpll_1v2_END (1)
#define SOC_CORECRG_L_L3_FNPLL_CFG0_sw_dc_buf_en_START (2)
#define SOC_CORECRG_L_L3_FNPLL_CFG0_sw_dc_buf_en_END (2)
#define SOC_CORECRG_L_L3_FNPLL_CFG0_sw_ac_buf_en_START (3)
#define SOC_CORECRG_L_L3_FNPLL_CFG0_sw_ac_buf_en_END (3)
#define SOC_CORECRG_L_L3_FNPLL_CFG0_phe_sel_in_fnpll_1v2_START (4)
#define SOC_CORECRG_L_L3_FNPLL_CFG0_phe_sel_in_fnpll_1v2_END (5)
#define SOC_CORECRG_L_L3_FNPLL_CFG0_unlock_clear_START (6)
#define SOC_CORECRG_L_L3_FNPLL_CFG0_unlock_clear_END (6)
#define SOC_CORECRG_L_L3_FNPLL_CFG0_dc_ac_clk_en_START (7)
#define SOC_CORECRG_L_L3_FNPLL_CFG0_dc_ac_clk_en_END (7)
#define SOC_CORECRG_L_L3_FNPLL_CFG0_dtc_m_cfg_fnpll_1v2_START (8)
#define SOC_CORECRG_L_L3_FNPLL_CFG0_dtc_m_cfg_fnpll_1v2_END (13)
#define SOC_CORECRG_L_L3_FNPLL_CFG0_ref_lost_clear_fnpll_1v2_START (14)
#define SOC_CORECRG_L_L3_FNPLL_CFG0_ref_lost_clear_fnpll_1v2_END (14)
#define SOC_CORECRG_L_L3_FNPLL_CFG0_spread_en_fnpll_1v2_START (16)
#define SOC_CORECRG_L_L3_FNPLL_CFG0_spread_en_fnpll_1v2_END (16)
#define SOC_CORECRG_L_L3_FNPLL_CFG0_downspread_fnpll_1v2_START (17)
#define SOC_CORECRG_L_L3_FNPLL_CFG0_downspread_fnpll_1v2_END (17)
#define SOC_CORECRG_L_L3_FNPLL_CFG0_sel_extwave_fnpll_1v2_START (18)
#define SOC_CORECRG_L_L3_FNPLL_CFG0_sel_extwave_fnpll_1v2_END (18)
#define SOC_CORECRG_L_L3_FNPLL_CFG0_divval_fnpll_1v2_START (19)
#define SOC_CORECRG_L_L3_FNPLL_CFG0_divval_fnpll_1v2_END (22)
#define SOC_CORECRG_L_L3_FNPLL_CFG0_spread_fnpll_1v2_START (23)
#define SOC_CORECRG_L_L3_FNPLL_CFG0_spread_fnpll_1v2_END (25)
#define SOC_CORECRG_L_L3_FNPLL_CFG0_freq_threshold_START (26)
#define SOC_CORECRG_L_L3_FNPLL_CFG0_freq_threshold_END (31)
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
} SOC_CORECRG_L_L3_FNPLL_CFG1_UNION;
#endif
#define SOC_CORECRG_L_L3_FNPLL_CFG1_ext_maxaddr_fnpll_1v2_START (0)
#define SOC_CORECRG_L_L3_FNPLL_CFG1_ext_maxaddr_fnpll_1v2_END (7)
#define SOC_CORECRG_L_L3_FNPLL_CFG1_extwaveval_fnpll_1v2_START (8)
#define SOC_CORECRG_L_L3_FNPLL_CFG1_extwaveval_fnpll_1v2_END (15)
#define SOC_CORECRG_L_L3_FNPLL_CFG1_input_cfg_en_fnpll_1v2_START (16)
#define SOC_CORECRG_L_L3_FNPLL_CFG1_input_cfg_en_fnpll_1v2_END (16)
#define SOC_CORECRG_L_L3_FNPLL_CFG1_test_mode_fnpll_1v2_START (17)
#define SOC_CORECRG_L_L3_FNPLL_CFG1_test_mode_fnpll_1v2_END (17)
#define SOC_CORECRG_L_L3_FNPLL_CFG1_lock_thr_fnpll_1v2_START (18)
#define SOC_CORECRG_L_L3_FNPLL_CFG1_lock_thr_fnpll_1v2_END (19)
#define SOC_CORECRG_L_L3_FNPLL_CFG1_refdiv_rst_n_fnpll_1v2_START (20)
#define SOC_CORECRG_L_L3_FNPLL_CFG1_refdiv_rst_n_fnpll_1v2_END (20)
#define SOC_CORECRG_L_L3_FNPLL_CFG1_fbdiv_rst_n_fnpll_1v2_START (21)
#define SOC_CORECRG_L_L3_FNPLL_CFG1_fbdiv_rst_n_fnpll_1v2_END (21)
#define SOC_CORECRG_L_L3_FNPLL_CFG1_div2_pd_fnpll_1v2_START (22)
#define SOC_CORECRG_L_L3_FNPLL_CFG1_div2_pd_fnpll_1v2_END (22)
#define SOC_CORECRG_L_L3_FNPLL_CFG1_lock_en_fnpll_1v2_START (23)
#define SOC_CORECRG_L_L3_FNPLL_CFG1_lock_en_fnpll_1v2_END (23)
#define SOC_CORECRG_L_L3_FNPLL_CFG1_fbdiv_delay_num_fnpll_1v2_START (24)
#define SOC_CORECRG_L_L3_FNPLL_CFG1_fbdiv_delay_num_fnpll_1v2_END (30)
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
} SOC_CORECRG_L_L3_FNPLL_CFG2_UNION;
#endif
#define SOC_CORECRG_L_L3_FNPLL_CFG2_bbpd_calib_byp_fnpll_1v2_START (0)
#define SOC_CORECRG_L_L3_FNPLL_CFG2_bbpd_calib_byp_fnpll_1v2_END (0)
#define SOC_CORECRG_L_L3_FNPLL_CFG2_dtc_ctrl_inv_fnpll_1v2_START (1)
#define SOC_CORECRG_L_L3_FNPLL_CFG2_dtc_ctrl_inv_fnpll_1v2_END (1)
#define SOC_CORECRG_L_L3_FNPLL_CFG2_dll_force_en_fnpll_1v2_START (2)
#define SOC_CORECRG_L_L3_FNPLL_CFG2_dll_force_en_fnpll_1v2_END (2)
#define SOC_CORECRG_L_L3_FNPLL_CFG2_DLL_EN_FNPLL_1V2_START (3)
#define SOC_CORECRG_L_L3_FNPLL_CFG2_DLL_EN_FNPLL_1V2_END (3)
#define SOC_CORECRG_L_L3_FNPLL_CFG2_phe_code_a_fnpll_1v2_START (4)
#define SOC_CORECRG_L_L3_FNPLL_CFG2_phe_code_a_fnpll_1v2_END (5)
#define SOC_CORECRG_L_L3_FNPLL_CFG2_phe_code_b_fnpll_1v2_START (6)
#define SOC_CORECRG_L_L3_FNPLL_CFG2_phe_code_b_fnpll_1v2_END (7)
#define SOC_CORECRG_L_L3_FNPLL_CFG2_ctinue_lock_num_fnpll_1v2_START (8)
#define SOC_CORECRG_L_L3_FNPLL_CFG2_ctinue_lock_num_fnpll_1v2_END (13)
#define SOC_CORECRG_L_L3_FNPLL_CFG2_read_en_fnpll_1v2_START (16)
#define SOC_CORECRG_L_L3_FNPLL_CFG2_read_en_fnpll_1v2_END (16)
#define SOC_CORECRG_L_L3_FNPLL_CFG2_k_gain_cfg_en_fnpll_1v2_START (17)
#define SOC_CORECRG_L_L3_FNPLL_CFG2_k_gain_cfg_en_fnpll_1v2_END (17)
#define SOC_CORECRG_L_L3_FNPLL_CFG2_k_gain_av_thr_fnpll_1v2_START (20)
#define SOC_CORECRG_L_L3_FNPLL_CFG2_k_gain_av_thr_fnpll_1v2_END (22)
#define SOC_CORECRG_L_L3_FNPLL_CFG2_pll_unlock_clr_fnpll_1v2_START (23)
#define SOC_CORECRG_L_L3_FNPLL_CFG2_pll_unlock_clr_fnpll_1v2_END (23)
#define SOC_CORECRG_L_L3_FNPLL_CFG2_k_gain_cfg_fnpll_1v2_START (24)
#define SOC_CORECRG_L_L3_FNPLL_CFG2_k_gain_cfg_fnpll_1v2_END (29)
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
} SOC_CORECRG_L_L3_FNPLL_CFG3_UNION;
#endif
#define SOC_CORECRG_L_L3_FNPLL_CFG3_dtc_o_cfg_fnpll_1v2_START (0)
#define SOC_CORECRG_L_L3_FNPLL_CFG3_dtc_o_cfg_fnpll_1v2_END (5)
#define SOC_CORECRG_L_L3_FNPLL_CFG3_bbpd_lock_num_fnpll_1v2_START (8)
#define SOC_CORECRG_L_L3_FNPLL_CFG3_bbpd_lock_num_fnpll_1v2_END (15)
#define SOC_CORECRG_L_L3_FNPLL_CFG3_lpf_bw_fnpll_1v2_START (16)
#define SOC_CORECRG_L_L3_FNPLL_CFG3_lpf_bw_fnpll_1v2_END (18)
#define SOC_CORECRG_L_L3_FNPLL_CFG3_lpf_bw1_fnpll_1v2_START (20)
#define SOC_CORECRG_L_L3_FNPLL_CFG3_lpf_bw1_fnpll_1v2_END (22)
#define SOC_CORECRG_L_L3_FNPLL_CFG3_calib_lpf_bw_fnpll_1v2_START (24)
#define SOC_CORECRG_L_L3_FNPLL_CFG3_calib_lpf_bw_fnpll_1v2_END (26)
#define SOC_CORECRG_L_L3_FNPLL_CFG3_calib_lpf_bw1_fnpll_1v2_START (28)
#define SOC_CORECRG_L_L3_FNPLL_CFG3_calib_lpf_bw1_fnpll_1v2_END (30)
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
} SOC_CORECRG_L_L3_FNPLL_CFG4_UNION;
#endif
#define SOC_CORECRG_L_L3_FNPLL_CFG4_max_k_gain_fnpll_1v2_START (0)
#define SOC_CORECRG_L_L3_FNPLL_CFG4_max_k_gain_fnpll_1v2_END (5)
#define SOC_CORECRG_L_L3_FNPLL_CFG4_min_k_gain_fnpll_1v2_START (8)
#define SOC_CORECRG_L_L3_FNPLL_CFG4_min_k_gain_fnpll_1v2_END (13)
#define SOC_CORECRG_L_L3_FNPLL_CFG4_en_dac_test_fnpll_1v2_START (16)
#define SOC_CORECRG_L_L3_FNPLL_CFG4_en_dac_test_fnpll_1v2_END (16)
#define SOC_CORECRG_L_L3_FNPLL_CFG4_updn_sel_fnpll_1v2_START (17)
#define SOC_CORECRG_L_L3_FNPLL_CFG4_updn_sel_fnpll_1v2_END (17)
#define SOC_CORECRG_L_L3_FNPLL_CFG4_icp_ctrl_fnpll_1v2_START (18)
#define SOC_CORECRG_L_L3_FNPLL_CFG4_icp_ctrl_fnpll_1v2_END (18)
#define SOC_CORECRG_L_L3_FNPLL_CFG4_vdc_sel_fnpll_1v2_START (20)
#define SOC_CORECRG_L_L3_FNPLL_CFG4_vdc_sel_fnpll_1v2_END (23)
#define SOC_CORECRG_L_L3_FNPLL_CFG4_test_data_sel_fnpll_1v2_START (28)
#define SOC_CORECRG_L_L3_FNPLL_CFG4_test_data_sel_fnpll_1v2_END (31)
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
} SOC_CORECRG_L_L3_FNPLL_CFG5_UNION;
#endif
#define SOC_CORECRG_L_L3_FNPLL_CFG5_ref_lost_thr_fnpll_1v2_START (0)
#define SOC_CORECRG_L_L3_FNPLL_CFG5_ref_lost_thr_fnpll_1v2_END (7)
#define SOC_CORECRG_L_L3_FNPLL_CFG5_pfd_div_ratio_START (8)
#define SOC_CORECRG_L_L3_FNPLL_CFG5_pfd_div_ratio_END (11)
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
} SOC_CORECRG_L_L3_FNPLL_STATE0_UNION;
#endif
#define SOC_CORECRG_L_L3_FNPLL_STATE0_div2_pd_test_fnpll_1v2_START (11)
#define SOC_CORECRG_L_L3_FNPLL_STATE0_div2_pd_test_fnpll_1v2_END (11)
#define SOC_CORECRG_L_L3_FNPLL_STATE0_fbdiv_rst_n_test_fnpll_1v2_START (12)
#define SOC_CORECRG_L_L3_FNPLL_STATE0_fbdiv_rst_n_test_fnpll_1v2_END (12)
#define SOC_CORECRG_L_L3_FNPLL_STATE0_refdiv_rst_n_test_fnpll_1v2_START (13)
#define SOC_CORECRG_L_L3_FNPLL_STATE0_refdiv_rst_n_test_fnpll_1v2_END (13)
#define SOC_CORECRG_L_L3_FNPLL_STATE0_ac_buf_pd_START (14)
#define SOC_CORECRG_L_L3_FNPLL_STATE0_ac_buf_pd_END (14)
#define SOC_CORECRG_L_L3_FNPLL_STATE0_dc_buf_pd_START (15)
#define SOC_CORECRG_L_L3_FNPLL_STATE0_dc_buf_pd_END (15)
#define SOC_CORECRG_L_L3_FNPLL_STATE0_pll_lock_fnpll_1v2_START (16)
#define SOC_CORECRG_L_L3_FNPLL_STATE0_pll_lock_fnpll_1v2_END (16)
#define SOC_CORECRG_L_L3_FNPLL_STATE0_pll_lock_ate_fnpll_1v2_START (17)
#define SOC_CORECRG_L_L3_FNPLL_STATE0_pll_lock_ate_fnpll_1v2_END (17)
#define SOC_CORECRG_L_L3_FNPLL_STATE0_bbpd_calib_done_fnpll_1v2_START (18)
#define SOC_CORECRG_L_L3_FNPLL_STATE0_bbpd_calib_done_fnpll_1v2_END (18)
#define SOC_CORECRG_L_L3_FNPLL_STATE0_bbpd_calib_fail_fnpll_1v2_START (19)
#define SOC_CORECRG_L_L3_FNPLL_STATE0_bbpd_calib_fail_fnpll_1v2_END (19)
#define SOC_CORECRG_L_L3_FNPLL_STATE0_bbpd_calib_success_fnpll_1v2_START (20)
#define SOC_CORECRG_L_L3_FNPLL_STATE0_bbpd_calib_success_fnpll_1v2_END (20)
#define SOC_CORECRG_L_L3_FNPLL_STATE0_pll_unlock_fnpll_1v2_START (21)
#define SOC_CORECRG_L_L3_FNPLL_STATE0_pll_unlock_fnpll_1v2_END (21)
#define SOC_CORECRG_L_L3_FNPLL_STATE0_clock_lost_fnpll_1v2_START (22)
#define SOC_CORECRG_L_L3_FNPLL_STATE0_clock_lost_fnpll_1v2_END (22)
#define SOC_CORECRG_L_L3_FNPLL_STATE0_ref_lost_flag_fnpll_1v2_START (23)
#define SOC_CORECRG_L_L3_FNPLL_STATE0_ref_lost_flag_fnpll_1v2_END (23)
#define SOC_CORECRG_L_L3_FNPLL_STATE0_test_data_fnpll_1v2_START (24)
#define SOC_CORECRG_L_L3_FNPLL_STATE0_test_data_fnpll_1v2_END (31)
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
} SOC_CORECRG_L_L3_FNPLL_STATE1_UNION;
#endif
#define SOC_CORECRG_L_L3_FNPLL_STATE1_unlock_count_fnpll_1v2_START (5)
#define SOC_CORECRG_L_L3_FNPLL_STATE1_unlock_count_fnpll_1v2_END (8)
#define SOC_CORECRG_L_L3_FNPLL_STATE1_unlock_flag_fnpll_1v2_START (9)
#define SOC_CORECRG_L_L3_FNPLL_STATE1_unlock_flag_fnpll_1v2_END (9)
#define SOC_CORECRG_L_L3_FNPLL_STATE1_lock_counter_fnpll_1v2_START (12)
#define SOC_CORECRG_L_L3_FNPLL_STATE1_lock_counter_fnpll_1v2_END (26)
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
} SOC_CORECRG_L_L3_APLL0_EXCTRL_A_UNION;
#endif
#define SOC_CORECRG_L_L3_APLL0_EXCTRL_A_pll_bbdrv_START (0)
#define SOC_CORECRG_L_L3_APLL0_EXCTRL_A_pll_bbdrv_END (2)
#define SOC_CORECRG_L_L3_APLL0_EXCTRL_A_pll_bben_START (3)
#define SOC_CORECRG_L_L3_APLL0_EXCTRL_A_pll_bben_END (3)
#define SOC_CORECRG_L_L3_APLL0_EXCTRL_A_pll_bbenoverride_START (4)
#define SOC_CORECRG_L_L3_APLL0_EXCTRL_A_pll_bbenoverride_END (4)
#define SOC_CORECRG_L_L3_APLL0_EXCTRL_A_pll_bbpw_START (5)
#define SOC_CORECRG_L_L3_APLL0_EXCTRL_A_pll_bbpw_END (7)
#define SOC_CORECRG_L_L3_APLL0_EXCTRL_A_pll_bwprog_START (8)
#define SOC_CORECRG_L_L3_APLL0_EXCTRL_A_pll_bwprog_END (10)
#define SOC_CORECRG_L_L3_APLL0_EXCTRL_A_pll_bwprogen_START (11)
#define SOC_CORECRG_L_L3_APLL0_EXCTRL_A_pll_bwprogen_END (11)
#define SOC_CORECRG_L_L3_APLL0_EXCTRL_A_pll_foutvco_en_START (12)
#define SOC_CORECRG_L_L3_APLL0_EXCTRL_A_pll_foutvco_en_END (12)
#define SOC_CORECRG_L_L3_APLL0_EXCTRL_A_pll_offsetcalbyp_START (13)
#define SOC_CORECRG_L_L3_APLL0_EXCTRL_A_pll_offsetcalbyp_END (13)
#define SOC_CORECRG_L_L3_APLL0_EXCTRL_A_pll_offsetcalcnt_START (14)
#define SOC_CORECRG_L_L3_APLL0_EXCTRL_A_pll_offsetcalcnt_END (16)
#define SOC_CORECRG_L_L3_APLL0_EXCTRL_A_pll_offsetcalen_START (17)
#define SOC_CORECRG_L_L3_APLL0_EXCTRL_A_pll_offsetcalen_END (17)
#define SOC_CORECRG_L_L3_APLL0_EXCTRL_A_pll_offsetcalin_START (18)
#define SOC_CORECRG_L_L3_APLL0_EXCTRL_A_pll_offsetcalin_END (29)
#define SOC_CORECRG_L_L3_APLL0_EXCTRL_A_pll_offsetfastcal_START (30)
#define SOC_CORECRG_L_L3_APLL0_EXCTRL_A_pll_offsetfastcal_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pll_vcosel_apll0 : 2;
        unsigned int pll_qpiprog : 3;
        unsigned int pll_qpiprogen : 1;
        unsigned int pll_qpiskewprog : 3;
        unsigned int pll_qpiskewprogen : 1;
        unsigned int pll_fprog : 3;
        unsigned int pll_fprogen : 1;
        unsigned int reserved : 2;
        unsigned int pll_reserved : 16;
    } reg;
} SOC_CORECRG_L_L3_APLL0_EXCTRL_B_UNION;
#endif
#define SOC_CORECRG_L_L3_APLL0_EXCTRL_B_pll_vcosel_apll0_START (0)
#define SOC_CORECRG_L_L3_APLL0_EXCTRL_B_pll_vcosel_apll0_END (1)
#define SOC_CORECRG_L_L3_APLL0_EXCTRL_B_pll_qpiprog_START (2)
#define SOC_CORECRG_L_L3_APLL0_EXCTRL_B_pll_qpiprog_END (4)
#define SOC_CORECRG_L_L3_APLL0_EXCTRL_B_pll_qpiprogen_START (5)
#define SOC_CORECRG_L_L3_APLL0_EXCTRL_B_pll_qpiprogen_END (5)
#define SOC_CORECRG_L_L3_APLL0_EXCTRL_B_pll_qpiskewprog_START (6)
#define SOC_CORECRG_L_L3_APLL0_EXCTRL_B_pll_qpiskewprog_END (8)
#define SOC_CORECRG_L_L3_APLL0_EXCTRL_B_pll_qpiskewprogen_START (9)
#define SOC_CORECRG_L_L3_APLL0_EXCTRL_B_pll_qpiskewprogen_END (9)
#define SOC_CORECRG_L_L3_APLL0_EXCTRL_B_pll_fprog_START (10)
#define SOC_CORECRG_L_L3_APLL0_EXCTRL_B_pll_fprog_END (12)
#define SOC_CORECRG_L_L3_APLL0_EXCTRL_B_pll_fprogen_START (13)
#define SOC_CORECRG_L_L3_APLL0_EXCTRL_B_pll_fprogen_END (13)
#define SOC_CORECRG_L_L3_APLL0_EXCTRL_B_pll_reserved_START (16)
#define SOC_CORECRG_L_L3_APLL0_EXCTRL_B_pll_reserved_END (31)
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
} SOC_CORECRG_L_L3_APLL1_EXCTRL_A_UNION;
#endif
#define SOC_CORECRG_L_L3_APLL1_EXCTRL_A_pll_bbdrv_START (0)
#define SOC_CORECRG_L_L3_APLL1_EXCTRL_A_pll_bbdrv_END (2)
#define SOC_CORECRG_L_L3_APLL1_EXCTRL_A_pll_bben_START (3)
#define SOC_CORECRG_L_L3_APLL1_EXCTRL_A_pll_bben_END (3)
#define SOC_CORECRG_L_L3_APLL1_EXCTRL_A_pll_bbenoverride_START (4)
#define SOC_CORECRG_L_L3_APLL1_EXCTRL_A_pll_bbenoverride_END (4)
#define SOC_CORECRG_L_L3_APLL1_EXCTRL_A_pll_bbpw_START (5)
#define SOC_CORECRG_L_L3_APLL1_EXCTRL_A_pll_bbpw_END (7)
#define SOC_CORECRG_L_L3_APLL1_EXCTRL_A_pll_bwprog_START (8)
#define SOC_CORECRG_L_L3_APLL1_EXCTRL_A_pll_bwprog_END (10)
#define SOC_CORECRG_L_L3_APLL1_EXCTRL_A_pll_bwprogen_START (11)
#define SOC_CORECRG_L_L3_APLL1_EXCTRL_A_pll_bwprogen_END (11)
#define SOC_CORECRG_L_L3_APLL1_EXCTRL_A_pll_foutvco_en_START (12)
#define SOC_CORECRG_L_L3_APLL1_EXCTRL_A_pll_foutvco_en_END (12)
#define SOC_CORECRG_L_L3_APLL1_EXCTRL_A_pll_offsetcalbyp_START (13)
#define SOC_CORECRG_L_L3_APLL1_EXCTRL_A_pll_offsetcalbyp_END (13)
#define SOC_CORECRG_L_L3_APLL1_EXCTRL_A_pll_offsetcalcnt_START (14)
#define SOC_CORECRG_L_L3_APLL1_EXCTRL_A_pll_offsetcalcnt_END (16)
#define SOC_CORECRG_L_L3_APLL1_EXCTRL_A_pll_offsetcalen_START (17)
#define SOC_CORECRG_L_L3_APLL1_EXCTRL_A_pll_offsetcalen_END (17)
#define SOC_CORECRG_L_L3_APLL1_EXCTRL_A_pll_offsetcalin_START (18)
#define SOC_CORECRG_L_L3_APLL1_EXCTRL_A_pll_offsetcalin_END (29)
#define SOC_CORECRG_L_L3_APLL1_EXCTRL_A_pll_offsetfastcal_START (30)
#define SOC_CORECRG_L_L3_APLL1_EXCTRL_A_pll_offsetfastcal_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pll_vcosel_apll1 : 2;
        unsigned int pll_qpiprog : 3;
        unsigned int pll_qpiprogen : 1;
        unsigned int pll_qpiskewprog : 3;
        unsigned int pll_qpiskewprogen : 1;
        unsigned int pll_fprog : 3;
        unsigned int pll_fprogen : 1;
        unsigned int reserved : 2;
        unsigned int pll_reserved : 16;
    } reg;
} SOC_CORECRG_L_L3_APLL1_EXCTRL_B_UNION;
#endif
#define SOC_CORECRG_L_L3_APLL1_EXCTRL_B_pll_vcosel_apll1_START (0)
#define SOC_CORECRG_L_L3_APLL1_EXCTRL_B_pll_vcosel_apll1_END (1)
#define SOC_CORECRG_L_L3_APLL1_EXCTRL_B_pll_qpiprog_START (2)
#define SOC_CORECRG_L_L3_APLL1_EXCTRL_B_pll_qpiprog_END (4)
#define SOC_CORECRG_L_L3_APLL1_EXCTRL_B_pll_qpiprogen_START (5)
#define SOC_CORECRG_L_L3_APLL1_EXCTRL_B_pll_qpiprogen_END (5)
#define SOC_CORECRG_L_L3_APLL1_EXCTRL_B_pll_qpiskewprog_START (6)
#define SOC_CORECRG_L_L3_APLL1_EXCTRL_B_pll_qpiskewprog_END (8)
#define SOC_CORECRG_L_L3_APLL1_EXCTRL_B_pll_qpiskewprogen_START (9)
#define SOC_CORECRG_L_L3_APLL1_EXCTRL_B_pll_qpiskewprogen_END (9)
#define SOC_CORECRG_L_L3_APLL1_EXCTRL_B_pll_fprog_START (10)
#define SOC_CORECRG_L_L3_APLL1_EXCTRL_B_pll_fprog_END (12)
#define SOC_CORECRG_L_L3_APLL1_EXCTRL_B_pll_fprogen_START (13)
#define SOC_CORECRG_L_L3_APLL1_EXCTRL_B_pll_fprogen_END (13)
#define SOC_CORECRG_L_L3_APLL1_EXCTRL_B_pll_reserved_START (16)
#define SOC_CORECRG_L_L3_APLL1_EXCTRL_B_pll_reserved_END (31)
#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
#endif
