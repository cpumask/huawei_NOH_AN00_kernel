#ifndef __SOC_USB_MISC_CTRL_INTERFACE_H__
#define __SOC_USB_MISC_CTRL_INTERFACE_H__ 
#ifdef __cplusplus
    #if __cplusplus
        extern "C" {
    #endif
#endif
#ifndef __SOC_H_FOR_ASM__
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl0_ADDR(base) ((base) + (0x00UL))
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl1_ADDR(base) ((base) + (0x04UL))
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl2_ADDR(base) ((base) + (0x08UL))
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl3_ADDR(base) ((base) + (0x0cUL))
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl4_ADDR(base) ((base) + (0x10UL))
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl5_ADDR(base) ((base) + (0x14UL))
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl6_ADDR(base) ((base) + (0x18UL))
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl7_ADDR(base) ((base) + (0x1cUL))
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl8_ADDR(base) ((base) + (0x20UL))
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl9_ADDR(base) ((base) + (0x24UL))
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_ADDR(base) ((base) + (0x70UL))
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_st0_ADDR(base) ((base) + (0x2cUL))
#define SOC_USB_MISC_CTRL_USBOTG3_CTRL1_ADDR(base) ((base) + (0x30UL))
#define SOC_USB_MISC_CTRL_USBOTG3_STS0_ADDR(base) ((base) + (0x34UL))
#define SOC_USB_MISC_CTRL_USBOTG3_STS1_ADDR(base) ((base) + (0x38UL))
#define SOC_USB_MISC_CTRL_USBOTG3_STS2_ADDR(base) ((base) + (0x3cUL))
#define SOC_USB_MISC_CTRL_USBOTG3_STS3_ADDR(base) ((base) + (0x40UL))
#define SOC_USB_MISC_CTRL_BC_CTRL0_ADDR(base) ((base) + (0x44UL))
#define SOC_USB_MISC_CTRL_BC_CTRL1_ADDR(base) ((base) + (0x48UL))
#define SOC_USB_MISC_CTRL_USB_MISC_CFG4c_ADDR(base) ((base) + (0x4cUL))
#define SOC_USB_MISC_CTRL_USBOTG3_STS4_ADDR(base) ((base) + (0x50UL))
#define SOC_USB_MISC_CTRL_USB_MISC_CFG54_ADDR(base) ((base) + (0x54UL))
#define SOC_USB_MISC_CTRL_USB_MISC_CFG58_ADDR(base) ((base) + (0x58UL))
#define SOC_USB_MISC_CTRL_USB_MISC_CFG5C_ADDR(base) ((base) + (0x5cUL))
#define SOC_USB_MISC_CTRL_USB_MISC_CFG60_ADDR(base) ((base) + (0x60UL))
#define SOC_USB_MISC_CTRL_USB_MISC_CFG64_ADDR(base) ((base) + (0x64UL))
#define SOC_USB_MISC_CTRL_USB_MISC_CFG68_ADDR(base) ((base) + (0x68UL))
#define SOC_USB_MISC_CTRL_USB_MISC_CFG6C_ADDR(base) ((base) + (0x6cUL))
#define SOC_USB_MISC_CTRL_USB_MISC_CFGA0_ADDR(base) ((base) + (0xA0UL))
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl11_ADDR(base) ((base) + (0x0074UL))
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl12_ADDR(base) ((base) + (0x0078UL))
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl13_ADDR(base) ((base) + (0x007cUL))
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl14_ADDR(base) ((base) + (0x0080UL))
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl15_ADDR(base) ((base) + (0x0084UL))
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl16_ADDR(base) ((base) + (0x0088UL))
#define SOC_USB_MISC_CTRL_usb_pipe_rst_n_ADDR(base) ((base) + (0x008cUL))
#define SOC_USB_MISC_CTRL_dwc_usb_ctrl0_ADDR(base) ((base) + (0x0090UL))
#define SOC_USB_MISC_CTRL_dwc_usb_ctrl1_ADDR(base) ((base) + (0x0094UL))
#define SOC_USB_MISC_CTRL_dwc_usb_ctrl2_ADDR(base) ((base) + (0x0098UL))
#else
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl0_ADDR(base) ((base) + (0x00))
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl1_ADDR(base) ((base) + (0x04))
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl2_ADDR(base) ((base) + (0x08))
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl3_ADDR(base) ((base) + (0x0c))
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl4_ADDR(base) ((base) + (0x10))
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl5_ADDR(base) ((base) + (0x14))
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl6_ADDR(base) ((base) + (0x18))
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl7_ADDR(base) ((base) + (0x1c))
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl8_ADDR(base) ((base) + (0x20))
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl9_ADDR(base) ((base) + (0x24))
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_ADDR(base) ((base) + (0x70))
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_st0_ADDR(base) ((base) + (0x2c))
#define SOC_USB_MISC_CTRL_USBOTG3_CTRL1_ADDR(base) ((base) + (0x30))
#define SOC_USB_MISC_CTRL_USBOTG3_STS0_ADDR(base) ((base) + (0x34))
#define SOC_USB_MISC_CTRL_USBOTG3_STS1_ADDR(base) ((base) + (0x38))
#define SOC_USB_MISC_CTRL_USBOTG3_STS2_ADDR(base) ((base) + (0x3c))
#define SOC_USB_MISC_CTRL_USBOTG3_STS3_ADDR(base) ((base) + (0x40))
#define SOC_USB_MISC_CTRL_BC_CTRL0_ADDR(base) ((base) + (0x44))
#define SOC_USB_MISC_CTRL_BC_CTRL1_ADDR(base) ((base) + (0x48))
#define SOC_USB_MISC_CTRL_USB_MISC_CFG4c_ADDR(base) ((base) + (0x4c))
#define SOC_USB_MISC_CTRL_USBOTG3_STS4_ADDR(base) ((base) + (0x50))
#define SOC_USB_MISC_CTRL_USB_MISC_CFG54_ADDR(base) ((base) + (0x54))
#define SOC_USB_MISC_CTRL_USB_MISC_CFG58_ADDR(base) ((base) + (0x58))
#define SOC_USB_MISC_CTRL_USB_MISC_CFG5C_ADDR(base) ((base) + (0x5c))
#define SOC_USB_MISC_CTRL_USB_MISC_CFG60_ADDR(base) ((base) + (0x60))
#define SOC_USB_MISC_CTRL_USB_MISC_CFG64_ADDR(base) ((base) + (0x64))
#define SOC_USB_MISC_CTRL_USB_MISC_CFG68_ADDR(base) ((base) + (0x68))
#define SOC_USB_MISC_CTRL_USB_MISC_CFG6C_ADDR(base) ((base) + (0x6c))
#define SOC_USB_MISC_CTRL_USB_MISC_CFGA0_ADDR(base) ((base) + (0xA0))
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl11_ADDR(base) ((base) + (0x0074))
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl12_ADDR(base) ((base) + (0x0078))
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl13_ADDR(base) ((base) + (0x007c))
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl14_ADDR(base) ((base) + (0x0080))
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl15_ADDR(base) ((base) + (0x0084))
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl16_ADDR(base) ((base) + (0x0088))
#define SOC_USB_MISC_CTRL_usb_pipe_rst_n_ADDR(base) ((base) + (0x008c))
#define SOC_USB_MISC_CTRL_dwc_usb_ctrl0_ADDR(base) ((base) + (0x0090))
#define SOC_USB_MISC_CTRL_dwc_usb_ctrl1_ADDR(base) ((base) + (0x0094))
#define SOC_USB_MISC_CTRL_dwc_usb_ctrl2_ADDR(base) ((base) + (0x0098))
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int phy_cfg_cr_clk_sel : 1;
        unsigned int phy_cfg_por_in_lx : 1;
        unsigned int phy_cfg_rcal_bypass : 1;
        unsigned int phy_cfg_rcal_code : 4;
        unsigned int phy_cfg_rptr_mode : 1;
        unsigned int phy_cfg_rx_hs_term_en : 1;
        unsigned int phy_cfg_tx_fsls_vreg_bypass : 1;
        unsigned int reserved : 22;
    } reg;
} SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl0_UNION;
#endif
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl0_phy_cfg_cr_clk_sel_START (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl0_phy_cfg_cr_clk_sel_END (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl0_phy_cfg_por_in_lx_START (1)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl0_phy_cfg_por_in_lx_END (1)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl0_phy_cfg_rcal_bypass_START (2)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl0_phy_cfg_rcal_bypass_END (2)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl0_phy_cfg_rcal_code_START (3)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl0_phy_cfg_rcal_code_END (6)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl0_phy_cfg_rptr_mode_START (7)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl0_phy_cfg_rptr_mode_END (7)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl0_phy_cfg_rx_hs_term_en_START (8)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl0_phy_cfg_rx_hs_term_en_END (8)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl0_phy_cfg_tx_fsls_vreg_bypass_START (9)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl0_phy_cfg_tx_fsls_vreg_bypass_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int phy_cfg_pll_cpbias_cntrl : 7;
        unsigned int phy_cfg_pll_fb_div : 12;
        unsigned int phy_cfg_pll_gmp_cntrl : 2;
        unsigned int phy_cfg_pll_int_cntrl : 6;
        unsigned int reserved : 5;
    } reg;
} SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl1_UNION;
#endif
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl1_phy_cfg_pll_cpbias_cntrl_START (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl1_phy_cfg_pll_cpbias_cntrl_END (6)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl1_phy_cfg_pll_fb_div_START (7)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl1_phy_cfg_pll_fb_div_END (18)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl1_phy_cfg_pll_gmp_cntrl_START (19)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl1_phy_cfg_pll_gmp_cntrl_END (20)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl1_phy_cfg_pll_int_cntrl_START (21)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl1_phy_cfg_pll_int_cntrl_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int phy_cfg_pll_prop_cntrl : 6;
        unsigned int phy_cfg_pll_ref_div : 4;
        unsigned int phy_cfg_pll_vco_cntrl : 3;
        unsigned int phy_cfg_pll_vref_tune : 2;
        unsigned int ref_freq_sel : 3;
        unsigned int reserved : 14;
    } reg;
} SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl2_UNION;
#endif
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl2_phy_cfg_pll_prop_cntrl_START (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl2_phy_cfg_pll_prop_cntrl_END (5)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl2_phy_cfg_pll_ref_div_START (6)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl2_phy_cfg_pll_ref_div_END (9)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl2_phy_cfg_pll_vco_cntrl_START (10)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl2_phy_cfg_pll_vco_cntrl_END (12)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl2_phy_cfg_pll_vref_tune_START (13)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl2_phy_cfg_pll_vref_tune_END (14)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl2_ref_freq_sel_START (15)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl2_ref_freq_sel_END (17)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int phy_cfg_rx_eq_ctle : 2;
        unsigned int phy_cfg_rx_hs_tune : 3;
        unsigned int reserved : 27;
    } reg;
} SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl3_UNION;
#endif
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl3_phy_cfg_rx_eq_ctle_START (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl3_phy_cfg_rx_eq_ctle_END (1)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl3_phy_cfg_rx_hs_tune_START (2)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl3_phy_cfg_rx_hs_tune_END (4)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int phy_cfg_tx_fsls_slew_rate_tune : 1;
        unsigned int phy_cfg_tx_fsls_vref_tune : 2;
        unsigned int phy_cfg_tx_hs_vref_tune : 3;
        unsigned int phy_cfg_tx_hs_xv_tune : 2;
        unsigned int phy_cfg_tx_preemp_tune : 3;
        unsigned int phy_cfg_tx_res_tune : 2;
        unsigned int phy_cfg_tx_rise_tune : 2;
        unsigned int reserved : 17;
    } reg;
} SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl4_UNION;
#endif
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl4_phy_cfg_tx_fsls_slew_rate_tune_START (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl4_phy_cfg_tx_fsls_slew_rate_tune_END (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl4_phy_cfg_tx_fsls_vref_tune_START (1)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl4_phy_cfg_tx_fsls_vref_tune_END (2)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl4_phy_cfg_tx_hs_vref_tune_START (3)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl4_phy_cfg_tx_hs_vref_tune_END (5)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl4_phy_cfg_tx_hs_xv_tune_START (6)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl4_phy_cfg_tx_hs_xv_tune_END (7)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl4_phy_cfg_tx_preemp_tune_START (8)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl4_phy_cfg_tx_preemp_tune_END (10)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl4_phy_cfg_tx_res_tune_START (11)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl4_phy_cfg_tx_res_tune_END (12)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl4_phy_cfg_tx_rise_tune_START (13)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl4_phy_cfg_tx_rise_tune_END (14)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int res_req_in : 1;
        unsigned int res_ack_in : 1;
        unsigned int test_burin : 1;
        unsigned int test_iddq : 1;
        unsigned int test_loopback_en : 1;
        unsigned int test_stop_clk_en : 1;
        unsigned int retenable_n : 1;
        unsigned int utmi_clk_force_en : 1;
        unsigned int utmi_txbitstuffen : 1;
        unsigned int reserved : 23;
    } reg;
} SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl5_UNION;
#endif
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl5_res_req_in_START (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl5_res_req_in_END (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl5_res_ack_in_START (1)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl5_res_ack_in_END (1)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl5_test_burin_START (2)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl5_test_burin_END (2)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl5_test_iddq_START (3)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl5_test_iddq_END (3)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl5_test_loopback_en_START (4)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl5_test_loopback_en_END (4)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl5_test_stop_clk_en_START (5)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl5_test_stop_clk_en_END (5)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl5_retenable_n_START (6)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl5_retenable_n_END (6)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl5_utmi_clk_force_en_START (7)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl5_utmi_clk_force_en_END (7)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl5_utmi_txbitstuffen_START (8)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl5_utmi_txbitstuffen_END (8)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int phy_reset : 1;
        unsigned int repeator_reset : 1;
        unsigned int ulpi_reset : 1;
        unsigned int reserved : 29;
    } reg;
} SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl6_UNION;
#endif
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl6_phy_reset_START (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl6_phy_reset_END (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl6_repeator_reset_START (1)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl6_repeator_reset_END (1)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl6_ulpi_reset_START (2)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl6_ulpi_reset_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int utmi_vbus_valid_ext : 1;
        unsigned int hsdt1_utmi_word_if : 1;
        unsigned int audio_utmi_word_if : 1;
        unsigned int asp_use_phy : 1;
        unsigned int dwc_bvalid : 1;
        unsigned int dwc_utmi_vbusvld_sel : 1;
        unsigned int combophy_ready : 1;
        unsigned int combophy_ready_sel : 1;
        unsigned int power_prsent_sel : 2;
        unsigned int reserved : 22;
    } reg;
} SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl7_UNION;
#endif
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl7_utmi_vbus_valid_ext_START (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl7_utmi_vbus_valid_ext_END (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl7_hsdt1_utmi_word_if_START (1)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl7_hsdt1_utmi_word_if_END (1)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl7_audio_utmi_word_if_START (2)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl7_audio_utmi_word_if_END (2)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl7_asp_use_phy_START (3)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl7_asp_use_phy_END (3)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl7_dwc_bvalid_START (4)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl7_dwc_bvalid_END (4)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl7_dwc_utmi_vbusvld_sel_START (5)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl7_dwc_utmi_vbusvld_sel_END (5)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl7_combophy_ready_START (6)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl7_combophy_ready_END (6)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl7_combophy_ready_sel_START (7)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl7_combophy_ready_sel_END (7)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl7_power_prsent_sel_START (8)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl7_power_prsent_sel_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int cfg_utmi_dppulldown : 1;
        unsigned int cfg_utmi_dmpulldown : 1;
        unsigned int reserved : 30;
    } reg;
} SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl8_UNION;
#endif
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl8_cfg_utmi_dppulldown_START (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl8_cfg_utmi_dppulldown_END (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl8_cfg_utmi_dmpulldown_START (1)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl8_cfg_utmi_dmpulldown_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int bvalid_cfg : 1;
        unsigned int sessend_cfg : 1;
        unsigned int vbusvalid_cfg : 1;
        unsigned int reserved_0 : 7;
        unsigned int logic_debug_trace_en : 1;
        unsigned int trace_sample_clear_start : 1;
        unsigned int reserved_1 : 2;
        unsigned int sample_ctrl : 4;
        unsigned int reserved_2 : 14;
    } reg;
} SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl9_UNION;
#endif
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl9_bvalid_cfg_START (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl9_bvalid_cfg_END (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl9_sessend_cfg_START (1)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl9_sessend_cfg_END (1)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl9_vbusvalid_cfg_START (2)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl9_vbusvalid_cfg_END (2)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl9_logic_debug_trace_en_START (10)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl9_logic_debug_trace_en_END (10)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl9_trace_sample_clear_start_START (11)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl9_trace_sample_clear_start_END (11)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl9_sample_ctrl_START (14)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl9_sample_ctrl_END (17)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int cfg_scan_apb_in : 1;
        unsigned int cfg_scan_mode : 1;
        unsigned int cfg_scan_ocla_clk : 1;
        unsigned int cfg_scan_ocla_in : 1;
        unsigned int cfg_scan_pclk_clk : 1;
        unsigned int cfg_scan_pclk_in : 17;
        unsigned int cfg_scan_pll_clk : 1;
        unsigned int cfg_scan_pll_in : 3;
        unsigned int cfg_scan_ref_clk : 1;
        unsigned int cfg_scan_ref_in : 1;
        unsigned int cfg_scan_sclk_clk : 1;
        unsigned int cfg_scan_set_rst : 1;
        unsigned int cfg_scan_shift : 1;
        unsigned int cfg_scan_shift_cg : 1;
    } reg;
} SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_UNION;
#endif
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_apb_in_START (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_apb_in_END (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_mode_START (1)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_mode_END (1)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_ocla_clk_START (2)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_ocla_clk_END (2)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_ocla_in_START (3)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_ocla_in_END (3)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_pclk_clk_START (4)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_pclk_clk_END (4)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_pclk_in_START (5)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_pclk_in_END (21)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_pll_clk_START (22)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_pll_clk_END (22)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_pll_in_START (23)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_pll_in_END (25)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_ref_clk_START (26)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_ref_clk_END (26)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_ref_in_START (27)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_ref_in_END (27)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_sclk_clk_START (28)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_sclk_clk_END (28)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_set_rst_START (29)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_set_rst_END (29)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_shift_START (30)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_shift_END (30)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_shift_cg_START (31)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl10_cfg_scan_shift_cg_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int phy_dtb_out : 2;
        unsigned int test_utmi_rx_data : 8;
        unsigned int reserved : 22;
    } reg;
} SOC_USB_MISC_CTRL_sc_usb20phy_misc_st0_UNION;
#endif
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_st0_phy_dtb_out_START (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_st0_phy_dtb_out_END (1)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_st0_test_utmi_rx_data_START (2)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_st0_test_utmi_rx_data_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 2;
        unsigned int hub_port_overcurrent : 2;
        unsigned int reserved_1 : 3;
        unsigned int xhc_bme : 1;
        unsigned int reserved_2 : 24;
    } reg;
} SOC_USB_MISC_CTRL_USBOTG3_CTRL1_UNION;
#endif
#define SOC_USB_MISC_CTRL_USBOTG3_CTRL1_hub_port_overcurrent_START (2)
#define SOC_USB_MISC_CTRL_USBOTG3_CTRL1_hub_port_overcurrent_END (3)
#define SOC_USB_MISC_CTRL_USBOTG3_CTRL1_xhc_bme_START (7)
#define SOC_USB_MISC_CTRL_USBOTG3_CTRL1_xhc_bme_END (7)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int debug : 32;
    } reg;
} SOC_USB_MISC_CTRL_USBOTG3_STS0_UNION;
#endif
#define SOC_USB_MISC_CTRL_USBOTG3_STS0_debug_START (0)
#define SOC_USB_MISC_CTRL_USBOTG3_STS0_debug_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int debugh : 32;
    } reg;
} SOC_USB_MISC_CTRL_USBOTG3_STS1_UNION;
#endif
#define SOC_USB_MISC_CTRL_USBOTG3_STS1_debugh_START (0)
#define SOC_USB_MISC_CTRL_USBOTG3_STS1_debugh_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int logic_analyzer_trace : 32;
    } reg;
} SOC_USB_MISC_CTRL_USBOTG3_STS2_UNION;
#endif
#define SOC_USB_MISC_CTRL_USBOTG3_STS2_logic_analyzer_trace_START (0)
#define SOC_USB_MISC_CTRL_USBOTG3_STS2_logic_analyzer_trace_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int logic_analyzer_traceh : 32;
    } reg;
} SOC_USB_MISC_CTRL_USBOTG3_STS3_UNION;
#endif
#define SOC_USB_MISC_CTRL_USBOTG3_STS3_logic_analyzer_traceh_START (0)
#define SOC_USB_MISC_CTRL_USBOTG3_STS3_logic_analyzer_traceh_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0: 2;
        unsigned int reserved_1: 2;
        unsigned int reserved_2: 1;
        unsigned int reserved_3: 1;
        unsigned int reserved_4: 1;
        unsigned int reserved_5: 1;
        unsigned int reserved_6: 1;
        unsigned int reserved_7: 2;
        unsigned int reserved_8: 21;
    } reg;
} SOC_USB_MISC_CTRL_BC_CTRL0_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0: 1;
        unsigned int reserved_1: 31;
    } reg;
} SOC_USB_MISC_CTRL_BC_CTRL1_UNION;
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int mem_ctrl : 16;
        unsigned int reserved : 16;
    } reg;
} SOC_USB_MISC_CTRL_USB_MISC_CFG4c_UNION;
#endif
#define SOC_USB_MISC_CTRL_USB_MISC_CFG4c_mem_ctrl_START (0)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG4c_mem_ctrl_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int host_legacy_smi_interrupt : 1;
        unsigned int reserved_0 : 3;
        unsigned int usb31_debug_i1 : 2;
        unsigned int sc_usb31_host_system_err : 1;
        unsigned int reserved_1 : 25;
    } reg;
} SOC_USB_MISC_CTRL_USBOTG3_STS4_UNION;
#endif
#define SOC_USB_MISC_CTRL_USBOTG3_STS4_host_legacy_smi_interrupt_START (0)
#define SOC_USB_MISC_CTRL_USBOTG3_STS4_host_legacy_smi_interrupt_END (0)
#define SOC_USB_MISC_CTRL_USBOTG3_STS4_usb31_debug_i1_START (4)
#define SOC_USB_MISC_CTRL_USBOTG3_STS4_usb31_debug_i1_END (5)
#define SOC_USB_MISC_CTRL_USBOTG3_STS4_sc_usb31_host_system_err_START (6)
#define SOC_USB_MISC_CTRL_USBOTG3_STS4_sc_usb31_host_system_err_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int host_legacy_smi_pci_cmd_reg_wr : 1;
        unsigned int host_legacy_smi_bar_wr : 1;
        unsigned int pme_en : 1;
        unsigned int reserved_0 : 1;
        unsigned int usb3_typec_flip_invert : 1;
        unsigned int reserved_1 : 18;
        unsigned int usb3_phy_test_powerdown : 1;
        unsigned int reserved_2 : 8;
    } reg;
} SOC_USB_MISC_CTRL_USB_MISC_CFG54_UNION;
#endif
#define SOC_USB_MISC_CTRL_USB_MISC_CFG54_host_legacy_smi_pci_cmd_reg_wr_START (0)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG54_host_legacy_smi_pci_cmd_reg_wr_END (0)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG54_host_legacy_smi_bar_wr_START (1)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG54_host_legacy_smi_bar_wr_END (1)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG54_pme_en_START (2)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG54_pme_en_END (2)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG54_usb3_typec_flip_invert_START (4)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG54_usb3_typec_flip_invert_END (4)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG54_usb3_phy_test_powerdown_START (23)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG54_usb3_phy_test_powerdown_END (23)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int usb3_upcs_pwr_stable : 1;
        unsigned int usb3_upcs_pwr_en : 1;
        unsigned int phy0_ana_pwr_en : 1;
        unsigned int phy0_ana_pwr_stable : 1;
        unsigned int phy0_pcs_pwr_en : 1;
        unsigned int phy0_pcs_pwr_stable : 1;
        unsigned int phy0_pma_pwr_en : 1;
        unsigned int phy0_pma_pwr_stable : 1;
        unsigned int usb31_pmgt_ref_clk_ok : 1;
        unsigned int usb31_pmgt_ref_clk_off : 1;
        unsigned int reserved : 22;
    } reg;
} SOC_USB_MISC_CTRL_USB_MISC_CFG58_UNION;
#endif
#define SOC_USB_MISC_CTRL_USB_MISC_CFG58_usb3_upcs_pwr_stable_START (0)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG58_usb3_upcs_pwr_stable_END (0)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG58_usb3_upcs_pwr_en_START (1)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG58_usb3_upcs_pwr_en_END (1)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG58_phy0_ana_pwr_en_START (2)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG58_phy0_ana_pwr_en_END (2)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG58_phy0_ana_pwr_stable_START (3)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG58_phy0_ana_pwr_stable_END (3)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG58_phy0_pcs_pwr_en_START (4)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG58_phy0_pcs_pwr_en_END (4)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG58_phy0_pcs_pwr_stable_START (5)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG58_phy0_pcs_pwr_stable_END (5)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG58_phy0_pma_pwr_en_START (6)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG58_phy0_pma_pwr_en_END (6)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG58_phy0_pma_pwr_stable_START (7)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG58_phy0_pma_pwr_stable_END (7)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG58_usb31_pmgt_ref_clk_ok_START (8)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG58_usb31_pmgt_ref_clk_ok_END (8)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG58_usb31_pmgt_ref_clk_off_START (9)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG58_usb31_pmgt_ref_clk_off_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sc_usb3_mem_pwr_ram0_half : 16;
        unsigned int reserved : 16;
    } reg;
} SOC_USB_MISC_CTRL_USB_MISC_CFG5C_UNION;
#endif
#define SOC_USB_MISC_CTRL_USB_MISC_CFG5C_sc_usb3_mem_pwr_ram0_half_START (0)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG5C_sc_usb3_mem_pwr_ram0_half_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sc_usb3_mem_pwr_ram0_half2 : 16;
        unsigned int reserved : 16;
    } reg;
} SOC_USB_MISC_CTRL_USB_MISC_CFG60_UNION;
#endif
#define SOC_USB_MISC_CTRL_USB_MISC_CFG60_sc_usb3_mem_pwr_ram0_half2_START (0)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG60_sc_usb3_mem_pwr_ram0_half2_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sc_usb3_mem_pwr_ram2 : 16;
        unsigned int reserved : 16;
    } reg;
} SOC_USB_MISC_CTRL_USB_MISC_CFG64_UNION;
#endif
#define SOC_USB_MISC_CTRL_USB_MISC_CFG64_sc_usb3_mem_pwr_ram2_START (0)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG64_sc_usb3_mem_pwr_ram2_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sc_usb3_mem_pwr_ram1_half : 16;
        unsigned int reserved : 16;
    } reg;
} SOC_USB_MISC_CTRL_USB_MISC_CFG68_UNION;
#endif
#define SOC_USB_MISC_CTRL_USB_MISC_CFG68_sc_usb3_mem_pwr_ram1_half_START (0)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG68_sc_usb3_mem_pwr_ram1_half_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sc_usb3_mem_pwr_ram1_half2 : 16;
        unsigned int reserved : 16;
    } reg;
} SOC_USB_MISC_CTRL_USB_MISC_CFG6C_UNION;
#endif
#define SOC_USB_MISC_CTRL_USB_MISC_CFG6C_sc_usb3_mem_pwr_ram1_half2_START (0)
#define SOC_USB_MISC_CTRL_USB_MISC_CFG6C_sc_usb3_mem_pwr_ram1_half2_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int usb2phy_por_n : 1;
        unsigned int usb3phy_reset_n : 1;
        unsigned int reserved_0 : 6;
        unsigned int usb31c_resetn : 1;
        unsigned int reserved_1 : 3;
        unsigned int tca_dp4_por : 1;
        unsigned int tca_usb_dev_por : 1;
        unsigned int tca_mux_enable : 1;
        unsigned int testpoint_mux : 2;
        unsigned int reserved_2 : 15;
    } reg;
} SOC_USB_MISC_CTRL_USB_MISC_CFGA0_UNION;
#endif
#define SOC_USB_MISC_CTRL_USB_MISC_CFGA0_usb2phy_por_n_START (0)
#define SOC_USB_MISC_CTRL_USB_MISC_CFGA0_usb2phy_por_n_END (0)
#define SOC_USB_MISC_CTRL_USB_MISC_CFGA0_usb3phy_reset_n_START (1)
#define SOC_USB_MISC_CTRL_USB_MISC_CFGA0_usb3phy_reset_n_END (1)
#define SOC_USB_MISC_CTRL_USB_MISC_CFGA0_usb31c_resetn_START (8)
#define SOC_USB_MISC_CTRL_USB_MISC_CFGA0_usb31c_resetn_END (8)
#define SOC_USB_MISC_CTRL_USB_MISC_CFGA0_tca_dp4_por_START (12)
#define SOC_USB_MISC_CTRL_USB_MISC_CFGA0_tca_dp4_por_END (12)
#define SOC_USB_MISC_CTRL_USB_MISC_CFGA0_tca_usb_dev_por_START (13)
#define SOC_USB_MISC_CTRL_USB_MISC_CFGA0_tca_usb_dev_por_END (13)
#define SOC_USB_MISC_CTRL_USB_MISC_CFGA0_tca_mux_enable_START (14)
#define SOC_USB_MISC_CTRL_USB_MISC_CFGA0_tca_mux_enable_END (14)
#define SOC_USB_MISC_CTRL_USB_MISC_CFGA0_testpoint_mux_START (15)
#define SOC_USB_MISC_CTRL_USB_MISC_CFGA0_testpoint_mux_END (16)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int cfg_scan_sclk_in : 7;
        unsigned int reserved : 24;
        unsigned int ate_apb_test : 1;
    } reg;
} SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl11_UNION;
#endif
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl11_cfg_scan_sclk_in_START (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl11_cfg_scan_sclk_in_END (6)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl11_ate_apb_test_START (31)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl11_ate_apb_test_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int cfg_eusb2_utmi_opmode : 2;
        unsigned int cfg_eusb2_utmi_termselect : 1;
        unsigned int cfg_eusb2_utmi_xcvrselect : 2;
        unsigned int cfg_eusb_utmi_portreset : 1;
        unsigned int cfg_eusb2_utmi_sleep_n : 1;
        unsigned int cfg_eusb2_utmi_suspend_n : 1;
        unsigned int cfg_eusb2_utmi_txvalid : 1;
        unsigned int cfg_eusb2_utmi_tx_data : 8;
        unsigned int reserved : 15;
    } reg;
} SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl12_UNION;
#endif
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl12_cfg_eusb2_utmi_opmode_START (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl12_cfg_eusb2_utmi_opmode_END (1)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl12_cfg_eusb2_utmi_termselect_START (2)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl12_cfg_eusb2_utmi_termselect_END (2)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl12_cfg_eusb2_utmi_xcvrselect_START (3)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl12_cfg_eusb2_utmi_xcvrselect_END (4)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl12_cfg_eusb_utmi_portreset_START (5)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl12_cfg_eusb_utmi_portreset_END (5)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl12_cfg_eusb2_utmi_sleep_n_START (6)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl12_cfg_eusb2_utmi_sleep_n_END (6)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl12_cfg_eusb2_utmi_suspend_n_START (7)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl12_cfg_eusb2_utmi_suspend_n_END (7)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl12_cfg_eusb2_utmi_txvalid_START (8)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl12_cfg_eusb2_utmi_txvalid_END (8)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl12_cfg_eusb2_utmi_tx_data_START (9)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl12_cfg_eusb2_utmi_tx_data_END (16)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int eusb_ram_clk_sel : 1;
        unsigned int eusb_read_en : 1;
        unsigned int eusb_read_addr : 8;
        unsigned int ram_ful_stop : 1;
        unsigned int ram_clear : 1;
        unsigned int reserved : 20;
    } reg;
} SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl13_UNION;
#endif
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl13_eusb_ram_clk_sel_START (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl13_eusb_ram_clk_sel_END (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl13_eusb_read_en_START (1)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl13_eusb_read_en_END (1)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl13_eusb_read_addr_START (2)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl13_eusb_read_addr_END (9)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl13_ram_ful_stop_START (10)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl13_ram_ful_stop_END (10)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl13_ram_clear_START (11)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl13_ram_clear_END (11)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ocla_mem_ctrl : 16;
        unsigned int reserved : 16;
    } reg;
} SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl14_UNION;
#endif
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl14_ocla_mem_ctrl_START (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl14_ocla_mem_ctrl_END (15)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ocla_ram_data_31_0 : 32;
    } reg;
} SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl15_UNION;
#endif
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl15_ocla_ram_data_31_0_START (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl15_ocla_ram_data_31_0_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ocla_ram_data_63_32 : 32;
    } reg;
} SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl16_UNION;
#endif
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl16_ocla_ram_data_63_32_START (0)
#define SOC_USB_MISC_CTRL_sc_usb20phy_misc_ctrl16_ocla_ram_data_63_32_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int usb_pipe_rst_n : 1;
        unsigned int usb_pipe_rst_sel : 1;
        unsigned int reserved : 30;
    } reg;
} SOC_USB_MISC_CTRL_usb_pipe_rst_n_UNION;
#endif
#define SOC_USB_MISC_CTRL_usb_pipe_rst_n_usb_pipe_rst_n_START (0)
#define SOC_USB_MISC_CTRL_usb_pipe_rst_n_usb_pipe_rst_n_END (0)
#define SOC_USB_MISC_CTRL_usb_pipe_rst_n_usb_pipe_rst_sel_START (1)
#define SOC_USB_MISC_CTRL_usb_pipe_rst_n_usb_pipe_rst_sel_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ls_eop_timer : 18;
        unsigned int reserved : 14;
    } reg;
} SOC_USB_MISC_CTRL_dwc_usb_ctrl0_UNION;
#endif
#define SOC_USB_MISC_CTRL_dwc_usb_ctrl0_ls_eop_timer_START (0)
#define SOC_USB_MISC_CTRL_dwc_usb_ctrl0_ls_eop_timer_END (17)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int nols_eop_timer : 18;
        unsigned int reserved : 14;
    } reg;
} SOC_USB_MISC_CTRL_dwc_usb_ctrl1_UNION;
#endif
#define SOC_USB_MISC_CTRL_dwc_usb_ctrl1_nols_eop_timer_START (0)
#define SOC_USB_MISC_CTRL_dwc_usb_ctrl1_nols_eop_timer_END (17)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int eop_timer_capture : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_USB_MISC_CTRL_dwc_usb_ctrl2_UNION;
#endif
#define SOC_USB_MISC_CTRL_dwc_usb_ctrl2_eop_timer_capture_START (0)
#define SOC_USB_MISC_CTRL_dwc_usb_ctrl2_eop_timer_capture_END (0)
#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
#endif
