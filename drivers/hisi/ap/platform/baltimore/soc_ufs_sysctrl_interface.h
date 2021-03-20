#ifndef __SOC_UFS_SYSCTRL_INTERFACE_H__
#define __SOC_UFS_SYSCTRL_INTERFACE_H__ 
#ifdef __cplusplus
    #if __cplusplus
        extern "C" {
    #endif
#endif
#ifndef __SOC_H_FOR_ASM__
#define SOC_UFS_Sysctrl_CRG_UFS_CFG_ADDR(base) ((base) + (0x000UL))
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_ADDR(base) ((base) + (0x004UL))
#define SOC_UFS_Sysctrl_UFS_RESET_CTRL_ADDR(base) ((base) + (0x008UL))
#define SOC_UFS_Sysctrl_UFSAXI_W_QOS_LMTR_ADDR(base) ((base) + (0x00CUL))
#define SOC_UFS_Sysctrl_UFS_UMECTRL_ufs_ies_en_mask_START (0)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG_ip_rst_ufs_START (0)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG_ip_arst_ufs_START (1)
#define SOC_UFS_Sysctrl_UFS_UMECTRL_ADDR(base) ((base) + (0x0064UL))
#define SOC_UFS_Sysctrl_UFSAXI_R_QOS_LMTR_ADDR(base) ((base) + (0x010UL))
#define SOC_UFS_Sysctrl_CRG_UFS_STAT_ADDR(base) ((base) + (0x014UL))
#define SOC_UFS_Sysctrl_UFS_AXI_CTRL_ADDR(base) ((base) + (0x018UL))
#define SOC_UFS_Sysctrl_MEMORY_CTRL_D1W2R_ADDR(base) ((base) + (0x01CUL))
#define SOC_UFS_Sysctrl_UFS_CLKRST_BYPASS_ADDR(base) ((base) + (0x020UL))
#define SOC_UFS_Sysctrl_AO_MEMORY_CTRL_ADDR(base) ((base) + (0x024UL))
#define SOC_UFS_Sysctrl_MEMORY_CTRL_ADDR(base) ((base) + (0x028UL))
#define SOC_UFS_Sysctrl_HIUFS_MPHY_CFG_ADDR(base) ((base) + (0x02CUL))
#define SOC_UFS_Sysctrl_HIUFS_DEBUG_ADDR(base) ((base) + (0x030UL))
#define SOC_UFS_Sysctrl_MPHYMCU_TEST_POINT_ADDR(base) ((base) + (0x034UL))
#define SOC_UFS_Sysctrl_UFS_ENHANCE_CTRL_ADDR(base) ((base) + (0xF00UL))
#else
#define SOC_UFS_Sysctrl_CRG_UFS_CFG_ADDR(base) ((base) + (0x000))
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_ADDR(base) ((base) + (0x004))
#define SOC_UFS_Sysctrl_UFS_RESET_CTRL_ADDR(base) ((base) + (0x008))
#define SOC_UFS_Sysctrl_UFSAXI_W_QOS_LMTR_ADDR(base) ((base) + (0x00C))
#define SOC_UFS_Sysctrl_UFSAXI_R_QOS_LMTR_ADDR(base) ((base) + (0x010))
#define SOC_UFS_Sysctrl_CRG_UFS_STAT_ADDR(base) ((base) + (0x014))
#define SOC_UFS_Sysctrl_UFS_AXI_CTRL_ADDR(base) ((base) + (0x018))
#define SOC_UFS_Sysctrl_MEMORY_CTRL_D1W2R_ADDR(base) ((base) + (0x01C))
#define SOC_UFS_Sysctrl_UFS_CLKRST_BYPASS_ADDR(base) ((base) + (0x020))
#define SOC_UFS_Sysctrl_AO_MEMORY_CTRL_ADDR(base) ((base) + (0x024))
#define SOC_UFS_Sysctrl_MEMORY_CTRL_ADDR(base) ((base) + (0x028))
#define SOC_UFS_Sysctrl_HIUFS_MPHY_CFG_ADDR(base) ((base) + (0x02C))
#define SOC_UFS_Sysctrl_HIUFS_DEBUG_ADDR(base) ((base) + (0x030))
#define SOC_UFS_Sysctrl_MPHYMCU_TEST_POINT_ADDR(base) ((base) + (0x034))
#define SOC_UFS_Sysctrl_UFS_ENHANCE_CTRL_ADDR(base) ((base) + (0xF00))
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int div_ufs_tick : 6;
        unsigned int reserved : 10;
        unsigned int clkdivmasken1 : 16;
    } reg;
} SOC_UFS_Sysctrl_CRG_UFS_CFG_UNION;
#endif
#define SOC_UFS_Sysctrl_CRG_UFS_CFG_div_ufs_tick_START (0)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG_div_ufs_tick_END (5)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG_clkdivmasken1_START (16)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG_clkdivmasken1_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_ufs : 1;
        unsigned int gt_hclk_ufs_top : 1;
        unsigned int gt_pclk_ufs_mphy_apb : 1;
        unsigned int gt_clk_ufsphy_cfg : 1;
        unsigned int gt_clk_ufs_mphymcu_bus : 1;
        unsigned int gt_clk_ufs_mphymcu : 1;
        unsigned int gt_pclk_ufs_debug : 1;
        unsigned int gt_pclk_ufs_djtag_s : 1;
        unsigned int gt_clk_ufs_32k : 1;
        unsigned int gt_clk_ufs_tick : 1;
        unsigned int sc_gt_clk_ufs_tick_div : 1;
        unsigned int sc_gt_debug_clk : 1;
        unsigned int reserved : 4;
        unsigned int clkdivmasken2 : 16;
    } reg;
} SOC_UFS_Sysctrl_CRG_UFS_CFG1_UNION;
#endif
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_gt_clk_ufs_START (0)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_gt_clk_ufs_END (0)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_gt_hclk_ufs_top_START (1)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_gt_hclk_ufs_top_END (1)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_gt_pclk_ufs_mphy_apb_START (2)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_gt_pclk_ufs_mphy_apb_END (2)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_gt_clk_ufsphy_cfg_START (3)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_gt_clk_ufsphy_cfg_END (3)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_gt_clk_ufs_mphymcu_bus_START (4)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_gt_clk_ufs_mphymcu_bus_END (4)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_gt_clk_ufs_mphymcu_START (5)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_gt_clk_ufs_mphymcu_END (5)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_gt_pclk_ufs_debug_START (6)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_gt_pclk_ufs_debug_END (6)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_gt_pclk_ufs_djtag_s_START (7)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_gt_pclk_ufs_djtag_s_END (7)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_gt_clk_ufs_32k_START (8)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_gt_clk_ufs_32k_END (8)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_gt_clk_ufs_tick_START (9)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_gt_clk_ufs_tick_END (9)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_sc_gt_clk_ufs_tick_div_START (10)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_sc_gt_clk_ufs_tick_div_END (10)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_sc_gt_debug_clk_START (11)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_sc_gt_debug_clk_END (11)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_clkdivmasken2_START (16)
#define SOC_UFS_Sysctrl_CRG_UFS_CFG1_clkdivmasken2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_rst_ufs : 1;
        unsigned int ip_arst_ufs : 1;
        unsigned int ip_prst_ufs_debug : 1;
        unsigned int ip_prst_dgtag_s : 1;
        unsigned int ip_rst_mphymcu_bus : 1;
        unsigned int ip_rst_mphymcu_arc4 : 1;
        unsigned int ip_rst_mphymcu_19p2 : 1;
        unsigned int ip_prst_mphy_apb : 1;
        unsigned int ip_hrst_ufs_dcdr : 1;
        unsigned int reserved : 6;
        unsigned int ufs_device_rst_n : 1;
        unsigned int clkdivmasken : 16;
    } reg;
} SOC_UFS_Sysctrl_UFS_RESET_CTRL_UNION;
#endif
#define SOC_UFS_Sysctrl_UFS_RESET_CTRL_ip_rst_ufs_START (0)
#define SOC_UFS_Sysctrl_UFS_RESET_CTRL_ip_rst_ufs_END (0)
#define SOC_UFS_Sysctrl_UFS_RESET_CTRL_ip_arst_ufs_START (1)
#define SOC_UFS_Sysctrl_UFS_RESET_CTRL_ip_arst_ufs_END (1)
#define SOC_UFS_Sysctrl_UFS_RESET_CTRL_ip_prst_ufs_debug_START (2)
#define SOC_UFS_Sysctrl_UFS_RESET_CTRL_ip_prst_ufs_debug_END (2)
#define SOC_UFS_Sysctrl_UFS_RESET_CTRL_ip_prst_dgtag_s_START (3)
#define SOC_UFS_Sysctrl_UFS_RESET_CTRL_ip_prst_dgtag_s_END (3)
#define SOC_UFS_Sysctrl_UFS_RESET_CTRL_ip_rst_mphymcu_bus_START (4)
#define SOC_UFS_Sysctrl_UFS_RESET_CTRL_ip_rst_mphymcu_bus_END (4)
#define SOC_UFS_Sysctrl_UFS_RESET_CTRL_ip_rst_mphymcu_arc4_START (5)
#define SOC_UFS_Sysctrl_UFS_RESET_CTRL_ip_rst_mphymcu_arc4_END (5)
#define SOC_UFS_Sysctrl_UFS_RESET_CTRL_ip_rst_mphymcu_19p2_START (6)
#define SOC_UFS_Sysctrl_UFS_RESET_CTRL_ip_rst_mphymcu_19p2_END (6)
#define SOC_UFS_Sysctrl_UFS_RESET_CTRL_ip_prst_mphy_apb_START (7)
#define SOC_UFS_Sysctrl_UFS_RESET_CTRL_ip_prst_mphy_apb_END (7)
#define SOC_UFS_Sysctrl_UFS_RESET_CTRL_ip_hrst_ufs_dcdr_START (8)
#define SOC_UFS_Sysctrl_UFS_RESET_CTRL_ip_hrst_ufs_dcdr_END (8)
#define SOC_UFS_Sysctrl_UFS_RESET_CTRL_ufs_device_rst_n_START (15)
#define SOC_UFS_Sysctrl_UFS_RESET_CTRL_ufs_device_rst_n_END (15)
#define SOC_UFS_Sysctrl_UFS_RESET_CTRL_clkdivmasken_START (16)
#define SOC_UFS_Sysctrl_UFS_RESET_CTRL_clkdivmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ufsaxi_w_lmtr_saturation : 14;
        unsigned int reserved_0 : 2;
        unsigned int ufsaxi_w_lmtr_bandwidth : 13;
        unsigned int reserved_1 : 2;
        unsigned int ufsaxi_w_lmtr_en : 1;
    } reg;
} SOC_UFS_Sysctrl_UFSAXI_W_QOS_LMTR_UNION;
#endif
#define SOC_UFS_Sysctrl_UFSAXI_W_QOS_LMTR_ufsaxi_w_lmtr_saturation_START (0)
#define SOC_UFS_Sysctrl_UFSAXI_W_QOS_LMTR_ufsaxi_w_lmtr_saturation_END (13)
#define SOC_UFS_Sysctrl_UFSAXI_W_QOS_LMTR_ufsaxi_w_lmtr_bandwidth_START (16)
#define SOC_UFS_Sysctrl_UFSAXI_W_QOS_LMTR_ufsaxi_w_lmtr_bandwidth_END (28)
#define SOC_UFS_Sysctrl_UFSAXI_W_QOS_LMTR_ufsaxi_w_lmtr_en_START (31)
#define SOC_UFS_Sysctrl_UFSAXI_W_QOS_LMTR_ufsaxi_w_lmtr_en_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ufsaxi_r_lmtr_saturation : 14;
        unsigned int reserved_0 : 2;
        unsigned int ufsaxi_r_lmtr_bandwidth : 13;
        unsigned int reserved_1 : 2;
        unsigned int ufsaxi_r_lmtr_en : 1;
    } reg;
} SOC_UFS_Sysctrl_UFSAXI_R_QOS_LMTR_UNION;
#endif
#define SOC_UFS_Sysctrl_UFSAXI_R_QOS_LMTR_ufsaxi_r_lmtr_saturation_START (0)
#define SOC_UFS_Sysctrl_UFSAXI_R_QOS_LMTR_ufsaxi_r_lmtr_saturation_END (13)
#define SOC_UFS_Sysctrl_UFSAXI_R_QOS_LMTR_ufsaxi_r_lmtr_bandwidth_START (16)
#define SOC_UFS_Sysctrl_UFSAXI_R_QOS_LMTR_ufsaxi_r_lmtr_bandwidth_END (28)
#define SOC_UFS_Sysctrl_UFSAXI_R_QOS_LMTR_ufsaxi_r_lmtr_en_START (31)
#define SOC_UFS_Sysctrl_UFSAXI_R_QOS_LMTR_ufsaxi_r_lmtr_en_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int crg_ufs_stat : 16;
        unsigned int reserved : 16;
    } reg;
} SOC_UFS_Sysctrl_CRG_UFS_STAT_UNION;
#endif
#define SOC_UFS_Sysctrl_CRG_UFS_STAT_crg_ufs_stat_START (0)
#define SOC_UFS_Sysctrl_CRG_UFS_STAT_crg_ufs_stat_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sc_axcache_mask : 2;
        unsigned int reserved : 30;
    } reg;
} SOC_UFS_Sysctrl_UFS_AXI_CTRL_UNION;
#endif
#define SOC_UFS_Sysctrl_UFS_AXI_CTRL_sc_axcache_mask_START (0)
#define SOC_UFS_Sysctrl_UFS_AXI_CTRL_sc_axcache_mask_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ufs_mem_ctrl_d1w2r : 16;
        unsigned int sc_ufspsw_mem_sdo : 1;
        unsigned int reserved : 15;
    } reg;
} SOC_UFS_Sysctrl_MEMORY_CTRL_D1W2R_UNION;
#endif
#define SOC_UFS_Sysctrl_MEMORY_CTRL_D1W2R_ufs_mem_ctrl_d1w2r_START (0)
#define SOC_UFS_Sysctrl_MEMORY_CTRL_D1W2R_ufs_mem_ctrl_d1w2r_END (15)
#define SOC_UFS_Sysctrl_MEMORY_CTRL_D1W2R_sc_ufspsw_mem_sdo_START (16)
#define SOC_UFS_Sysctrl_MEMORY_CTRL_D1W2R_sc_ufspsw_mem_sdo_END (16)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ufs_ufshc_unipro_cfg_clkrst_bypass : 1;
        unsigned int ufs_mphy_cfg_clkrst_bypass : 1;
        unsigned int ufs_ahbcfg_clkrst_bypass : 1;
        unsigned int ufs_trace_clkrst_bypass : 1;
        unsigned int reserved : 28;
    } reg;
} SOC_UFS_Sysctrl_UFS_CLKRST_BYPASS_UNION;
#endif
#define SOC_UFS_Sysctrl_UFS_CLKRST_BYPASS_ufs_ufshc_unipro_cfg_clkrst_bypass_START (0)
#define SOC_UFS_Sysctrl_UFS_CLKRST_BYPASS_ufs_ufshc_unipro_cfg_clkrst_bypass_END (0)
#define SOC_UFS_Sysctrl_UFS_CLKRST_BYPASS_ufs_mphy_cfg_clkrst_bypass_START (1)
#define SOC_UFS_Sysctrl_UFS_CLKRST_BYPASS_ufs_mphy_cfg_clkrst_bypass_END (1)
#define SOC_UFS_Sysctrl_UFS_CLKRST_BYPASS_ufs_ahbcfg_clkrst_bypass_START (2)
#define SOC_UFS_Sysctrl_UFS_CLKRST_BYPASS_ufs_ahbcfg_clkrst_bypass_END (2)
#define SOC_UFS_Sysctrl_UFS_CLKRST_BYPASS_ufs_trace_clkrst_bypass_START (3)
#define SOC_UFS_Sysctrl_UFS_CLKRST_BYPASS_ufs_trace_clkrst_bypass_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ufs_ao_mem_ctrl : 26;
        unsigned int reserved : 6;
    } reg;
} SOC_UFS_Sysctrl_AO_MEMORY_CTRL_UNION;
#endif
#define SOC_UFS_Sysctrl_AO_MEMORY_CTRL_ufs_ao_mem_ctrl_START (0)
#define SOC_UFS_Sysctrl_AO_MEMORY_CTRL_ufs_ao_mem_ctrl_END (25)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ufs_mem_ctrl : 26;
        unsigned int reserved : 6;
    } reg;
} SOC_UFS_Sysctrl_MEMORY_CTRL_UNION;
#endif
#define SOC_UFS_Sysctrl_MEMORY_CTRL_ufs_mem_ctrl_START (0)
#define SOC_UFS_Sysctrl_MEMORY_CTRL_ufs_mem_ctrl_END (25)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int mphy_reg_bypass_40to20 : 1;
        unsigned int mphy_ufs_en : 1;
        unsigned int mphy_i2c_sel : 1;
        unsigned int mphymcu_jtag_sel : 1;
        unsigned int reserved : 28;
    } reg;
} SOC_UFS_Sysctrl_HIUFS_MPHY_CFG_UNION;
#endif
#define SOC_UFS_Sysctrl_HIUFS_MPHY_CFG_mphy_reg_bypass_40to20_START (0)
#define SOC_UFS_Sysctrl_HIUFS_MPHY_CFG_mphy_reg_bypass_40to20_END (0)
#define SOC_UFS_Sysctrl_HIUFS_MPHY_CFG_mphy_ufs_en_START (1)
#define SOC_UFS_Sysctrl_HIUFS_MPHY_CFG_mphy_ufs_en_END (1)
#define SOC_UFS_Sysctrl_HIUFS_MPHY_CFG_mphy_i2c_sel_START (2)
#define SOC_UFS_Sysctrl_HIUFS_MPHY_CFG_mphy_i2c_sel_END (2)
#define SOC_UFS_Sysctrl_HIUFS_MPHY_CFG_mphymcu_jtag_sel_START (3)
#define SOC_UFS_Sysctrl_HIUFS_MPHY_CFG_mphymcu_jtag_sel_END (3)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int intr_remote_hibernate_exit : 1;
        unsigned int mphy_pll_lock : 1;
        unsigned int reserved : 30;
    } reg;
} SOC_UFS_Sysctrl_HIUFS_DEBUG_UNION;
#endif
#define SOC_UFS_Sysctrl_HIUFS_DEBUG_intr_remote_hibernate_exit_START (0)
#define SOC_UFS_Sysctrl_HIUFS_DEBUG_intr_remote_hibernate_exit_END (0)
#define SOC_UFS_Sysctrl_HIUFS_DEBUG_mphy_pll_lock_START (1)
#define SOC_UFS_Sysctrl_HIUFS_DEBUG_mphy_pll_lock_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int mphymcu_test_point : 8;
        unsigned int reserved : 24;
    } reg;
} SOC_UFS_Sysctrl_MPHYMCU_TEST_POINT_UNION;
#endif
#define SOC_UFS_Sysctrl_MPHYMCU_TEST_POINT_mphymcu_test_point_START (0)
#define SOC_UFS_Sysctrl_MPHYMCU_TEST_POINT_mphymcu_test_point_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sc_enhanced_op_en : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_UFS_Sysctrl_UFS_ENHANCE_CTRL_UNION;
#endif
#define SOC_UFS_Sysctrl_UFS_ENHANCE_CTRL_sc_enhanced_op_en_START (0)
#define SOC_UFS_Sysctrl_UFS_ENHANCE_CTRL_sc_enhanced_op_en_END (0)
#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
#endif
