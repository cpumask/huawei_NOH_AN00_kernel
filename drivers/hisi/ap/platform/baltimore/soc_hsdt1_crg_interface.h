#ifndef __SOC_HSDT1_CRG_INTERFACE_H__
#define __SOC_HSDT1_CRG_INTERFACE_H__ 
#ifdef __cplusplus
    #if __cplusplus
        extern "C" {
    #endif
#endif
#ifndef __SOC_H_FOR_ASM__
#define SOC_HSDT1_CRG_PEREN0_ADDR(base) ((base) + (0x000UL))
#define SOC_HSDT1_CRG_PERDIS0_ADDR(base) ((base) + (0x004UL))
#define SOC_HSDT1_CRG_PERCLKEN0_ADDR(base) ((base) + (0x008UL))
#define SOC_HSDT1_CRG_PERSTAT0_ADDR(base) ((base) + (0x00CUL))
#define SOC_HSDT1_CRG_PEREN1_ADDR(base) ((base) + (0x010UL))
#define SOC_HSDT1_CRG_PERDIS1_ADDR(base) ((base) + (0x014UL))
#define SOC_HSDT1_CRG_PERCLKEN1_ADDR(base) ((base) + (0x018UL))
#define SOC_HSDT1_CRG_PERSTAT1_ADDR(base) ((base) + (0x01CUL))
#define SOC_HSDT1_CRG_PERRSTEN0_ADDR(base) ((base) + (0x020UL))
#define SOC_HSDT1_CRG_PERRSTDIS0_ADDR(base) ((base) + (0x024UL))
#define SOC_HSDT1_CRG_PERRSTSTAT0_ADDR(base) ((base) + (0x028UL))
#define SOC_HSDT1_CRG_CLKDIV0_ADDR(base) ((base) + (0x0A8UL))
#define SOC_HSDT1_CRG_PERI_STAT0_ADDR(base) ((base) + (0xC8UL))
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_ADDR(base) ((base) + (0x100UL))
#define SOC_HSDT1_CRG_PCIEPLL_CTRL0_ADDR(base) ((base) + (0x198UL))
#define SOC_HSDT1_CRG_PCIEPLL_CTRL1_ADDR(base) ((base) + (0x19cUL))
#define SOC_HSDT1_CRG_PCIEPLL_CTRL_A_ADDR(base) ((base) + (0x200UL))
#define SOC_HSDT1_CRG_PCIEPLL_CTRL_B_ADDR(base) ((base) + (0x204UL))
#define SOC_HSDT1_CRG_PCIEPLL_STAT_ADDR(base) ((base) + (0x208UL))
#define SOC_HSDT1_CRG_FNPLL_STAT0_ADDR(base) ((base) + (0x20CUL))
#define SOC_HSDT1_CRG_FNPLL_STAT1_ADDR(base) ((base) + (0x210UL))
#define SOC_HSDT1_CRG_FNPLL_CFG0_ADDR(base) ((base) + (0x224UL))
#define SOC_HSDT1_CRG_FNPLL_CFG1_ADDR(base) ((base) + (0x228UL))
#define SOC_HSDT1_CRG_FNPLL_CFG2_ADDR(base) ((base) + (0x22CUL))
#define SOC_HSDT1_CRG_FNPLL_CFG3_ADDR(base) ((base) + (0x230UL))
#define SOC_HSDT1_CRG_FNPLL_CFG4_ADDR(base) ((base) + (0x234UL))
#define SOC_HSDT1_CRG_FNPLL_CFG5_ADDR(base) ((base) + (0x238UL))
#define SOC_HSDT1_CRG_FNPLL_CFG6_ADDR(base) ((base) + (0x23CUL))
#define SOC_HSDT1_CRG_FNPLL_CFG7_ADDR(base) ((base) + (0x240UL))
#define SOC_HSDT1_CRG_ISOEN_ADDR(base) ((base) + (0x280UL))
#define SOC_HSDT1_CRG_ISODIS_ADDR(base) ((base) + (0x284UL))
#define SOC_HSDT1_CRG_ISOSTAT_ADDR(base) ((base) + (0x288UL))
#define SOC_HSDT1_CRG_PCIECTRL1_ADDR(base) ((base) + (0x304UL))
#define SOC_HSDT1_CRG_FNPLL_BK_STAT0_ADDR(base) ((base) + (0x400UL))
#define SOC_HSDT1_CRG_FNPLL_BK_STAT1_ADDR(base) ((base) + (0x404UL))
#define SOC_HSDT1_CRG_INTR_MASK_NOCBUS_NONIDLE_PEND_ADDR(base) ((base) + (0x500UL))
#define SOC_HSDT1_CRG_INTR_CLR_NOCBUS_NONIDLE_PEND_ADDR(base) ((base) + (0x504UL))
#define SOC_HSDT1_CRG_INTR_STAT_NOCBUS_NONIDLE_PEND_ADDR(base) ((base) + (0x508UL))
#else
#define SOC_HSDT1_CRG_PEREN0_ADDR(base) ((base) + (0x000))
#define SOC_HSDT1_CRG_PERDIS0_ADDR(base) ((base) + (0x004))
#define SOC_HSDT1_CRG_PERCLKEN0_ADDR(base) ((base) + (0x008))
#define SOC_HSDT1_CRG_PERSTAT0_ADDR(base) ((base) + (0x00C))
#define SOC_HSDT1_CRG_PEREN1_ADDR(base) ((base) + (0x010))
#define SOC_HSDT1_CRG_PERDIS1_ADDR(base) ((base) + (0x014))
#define SOC_HSDT1_CRG_PERCLKEN1_ADDR(base) ((base) + (0x018))
#define SOC_HSDT1_CRG_PERSTAT1_ADDR(base) ((base) + (0x01C))
#define SOC_HSDT1_CRG_PERRSTEN0_ADDR(base) ((base) + (0x020))
#define SOC_HSDT1_CRG_PERRSTDIS0_ADDR(base) ((base) + (0x024))
#define SOC_HSDT1_CRG_PERRSTSTAT0_ADDR(base) ((base) + (0x028))
#define SOC_HSDT1_CRG_CLKDIV0_ADDR(base) ((base) + (0x0A8))
#define SOC_HSDT1_CRG_PERI_STAT0_ADDR(base) ((base) + (0xC8))
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_ADDR(base) ((base) + (0x100))
#define SOC_HSDT1_CRG_PCIEPLL_CTRL0_ADDR(base) ((base) + (0x198))
#define SOC_HSDT1_CRG_PCIEPLL_CTRL1_ADDR(base) ((base) + (0x19c))
#define SOC_HSDT1_CRG_PCIEPLL_CTRL_A_ADDR(base) ((base) + (0x200))
#define SOC_HSDT1_CRG_PCIEPLL_CTRL_B_ADDR(base) ((base) + (0x204))
#define SOC_HSDT1_CRG_PCIEPLL_STAT_ADDR(base) ((base) + (0x208))
#define SOC_HSDT1_CRG_FNPLL_STAT0_ADDR(base) ((base) + (0x20C))
#define SOC_HSDT1_CRG_FNPLL_STAT1_ADDR(base) ((base) + (0x210))
#define SOC_HSDT1_CRG_FNPLL_CFG0_ADDR(base) ((base) + (0x224))
#define SOC_HSDT1_CRG_FNPLL_CFG1_ADDR(base) ((base) + (0x228))
#define SOC_HSDT1_CRG_FNPLL_CFG2_ADDR(base) ((base) + (0x22C))
#define SOC_HSDT1_CRG_FNPLL_CFG3_ADDR(base) ((base) + (0x230))
#define SOC_HSDT1_CRG_FNPLL_CFG4_ADDR(base) ((base) + (0x234))
#define SOC_HSDT1_CRG_FNPLL_CFG5_ADDR(base) ((base) + (0x238))
#define SOC_HSDT1_CRG_FNPLL_CFG6_ADDR(base) ((base) + (0x23C))
#define SOC_HSDT1_CRG_FNPLL_CFG7_ADDR(base) ((base) + (0x240))
#define SOC_HSDT1_CRG_ISOEN_ADDR(base) ((base) + (0x280))
#define SOC_HSDT1_CRG_ISODIS_ADDR(base) ((base) + (0x284))
#define SOC_HSDT1_CRG_ISOSTAT_ADDR(base) ((base) + (0x288))
#define SOC_HSDT1_CRG_PCIECTRL1_ADDR(base) ((base) + (0x304))
#define SOC_HSDT1_CRG_FNPLL_BK_STAT0_ADDR(base) ((base) + (0x400))
#define SOC_HSDT1_CRG_FNPLL_BK_STAT1_ADDR(base) ((base) + (0x404))
#define SOC_HSDT1_CRG_INTR_MASK_NOCBUS_NONIDLE_PEND_ADDR(base) ((base) + (0x500))
#define SOC_HSDT1_CRG_INTR_CLR_NOCBUS_NONIDLE_PEND_ADDR(base) ((base) + (0x504))
#define SOC_HSDT1_CRG_INTR_STAT_NOCBUS_NONIDLE_PEND_ADDR(base) ((base) + (0x508))
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_hclk_sd : 1;
        unsigned int gt_pclk_hsdt1_sysctrl : 1;
        unsigned int gt_aclk_dpctrl : 1;
        unsigned int gt_pclk_dpctrl : 1;
        unsigned int gt_pclk_hsdt1_ioc : 1;
        unsigned int gt_hclk_usb3otg_misc : 1;
        unsigned int gt_aclk_usb3otg : 1;
        unsigned int gt_aclk_noc_dpctrl_asyncbrg : 1;
        unsigned int gt_aclk_noc_usb3otg_asyncbrg : 1;
        unsigned int gt_clk_pcie1aux : 1;
        unsigned int gt_clk_pcie1_hp : 1;
        unsigned int gt_clk_pcie1_debounce : 1;
        unsigned int gt_pclk_pcie1_phy : 1;
        unsigned int gt_pclk_pcie1_sys : 1;
        unsigned int gt_aclk_pcie1 : 1;
        unsigned int gt_clk_pcie1phy_ref_soft : 1;
        unsigned int gt_clk_pcie1io_soft : 1;
        unsigned int gt_aclk_noc_pcie1_asyncbrg : 1;
        unsigned int gt_clk_usb2phy_ref : 1;
        unsigned int gt_aclk_hsdt1_trace : 1;
        unsigned int gt_pclk_hsdt1_trace : 1;
        unsigned int gt_clk_hsdt1_tcu : 1;
        unsigned int gt_clk_hsdt1_tbu : 1;
        unsigned int gt_clk_hsdt1_eusb : 1;
        unsigned int gt_clk_ulpi_ref : 1;
        unsigned int gt_clk_usb2_ulpi : 1;
        unsigned int gt_clk_usb31phy_apb : 1;
        unsigned int reserved : 5;
    } reg;
} SOC_HSDT1_CRG_PEREN0_UNION;
#endif
#define SOC_HSDT1_CRG_PEREN0_gt_hclk_sd_START (0)
#define SOC_HSDT1_CRG_PEREN0_gt_hclk_sd_END (0)
#define SOC_HSDT1_CRG_PEREN0_gt_pclk_hsdt1_sysctrl_START (1)
#define SOC_HSDT1_CRG_PEREN0_gt_pclk_hsdt1_sysctrl_END (1)
#define SOC_HSDT1_CRG_PEREN0_gt_aclk_dpctrl_START (2)
#define SOC_HSDT1_CRG_PEREN0_gt_aclk_dpctrl_END (2)
#define SOC_HSDT1_CRG_PEREN0_gt_pclk_dpctrl_START (3)
#define SOC_HSDT1_CRG_PEREN0_gt_pclk_dpctrl_END (3)
#define SOC_HSDT1_CRG_PEREN0_gt_pclk_hsdt1_ioc_START (4)
#define SOC_HSDT1_CRG_PEREN0_gt_pclk_hsdt1_ioc_END (4)
#define SOC_HSDT1_CRG_PEREN0_gt_hclk_usb3otg_misc_START (5)
#define SOC_HSDT1_CRG_PEREN0_gt_hclk_usb3otg_misc_END (5)
#define SOC_HSDT1_CRG_PEREN0_gt_aclk_usb3otg_START (6)
#define SOC_HSDT1_CRG_PEREN0_gt_aclk_usb3otg_END (6)
#define SOC_HSDT1_CRG_PEREN0_gt_aclk_noc_dpctrl_asyncbrg_START (7)
#define SOC_HSDT1_CRG_PEREN0_gt_aclk_noc_dpctrl_asyncbrg_END (7)
#define SOC_HSDT1_CRG_PEREN0_gt_aclk_noc_usb3otg_asyncbrg_START (8)
#define SOC_HSDT1_CRG_PEREN0_gt_aclk_noc_usb3otg_asyncbrg_END (8)
#define SOC_HSDT1_CRG_PEREN0_gt_clk_pcie1aux_START (9)
#define SOC_HSDT1_CRG_PEREN0_gt_clk_pcie1aux_END (9)
#define SOC_HSDT1_CRG_PEREN0_gt_clk_pcie1_hp_START (10)
#define SOC_HSDT1_CRG_PEREN0_gt_clk_pcie1_hp_END (10)
#define SOC_HSDT1_CRG_PEREN0_gt_clk_pcie1_debounce_START (11)
#define SOC_HSDT1_CRG_PEREN0_gt_clk_pcie1_debounce_END (11)
#define SOC_HSDT1_CRG_PEREN0_gt_pclk_pcie1_phy_START (12)
#define SOC_HSDT1_CRG_PEREN0_gt_pclk_pcie1_phy_END (12)
#define SOC_HSDT1_CRG_PEREN0_gt_pclk_pcie1_sys_START (13)
#define SOC_HSDT1_CRG_PEREN0_gt_pclk_pcie1_sys_END (13)
#define SOC_HSDT1_CRG_PEREN0_gt_aclk_pcie1_START (14)
#define SOC_HSDT1_CRG_PEREN0_gt_aclk_pcie1_END (14)
#define SOC_HSDT1_CRG_PEREN0_gt_clk_pcie1phy_ref_soft_START (15)
#define SOC_HSDT1_CRG_PEREN0_gt_clk_pcie1phy_ref_soft_END (15)
#define SOC_HSDT1_CRG_PEREN0_gt_clk_pcie1io_soft_START (16)
#define SOC_HSDT1_CRG_PEREN0_gt_clk_pcie1io_soft_END (16)
#define SOC_HSDT1_CRG_PEREN0_gt_aclk_noc_pcie1_asyncbrg_START (17)
#define SOC_HSDT1_CRG_PEREN0_gt_aclk_noc_pcie1_asyncbrg_END (17)
#define SOC_HSDT1_CRG_PEREN0_gt_clk_usb2phy_ref_START (18)
#define SOC_HSDT1_CRG_PEREN0_gt_clk_usb2phy_ref_END (18)
#define SOC_HSDT1_CRG_PEREN0_gt_aclk_hsdt1_trace_START (19)
#define SOC_HSDT1_CRG_PEREN0_gt_aclk_hsdt1_trace_END (19)
#define SOC_HSDT1_CRG_PEREN0_gt_pclk_hsdt1_trace_START (20)
#define SOC_HSDT1_CRG_PEREN0_gt_pclk_hsdt1_trace_END (20)
#define SOC_HSDT1_CRG_PEREN0_gt_clk_hsdt1_tcu_START (21)
#define SOC_HSDT1_CRG_PEREN0_gt_clk_hsdt1_tcu_END (21)
#define SOC_HSDT1_CRG_PEREN0_gt_clk_hsdt1_tbu_START (22)
#define SOC_HSDT1_CRG_PEREN0_gt_clk_hsdt1_tbu_END (22)
#define SOC_HSDT1_CRG_PEREN0_gt_clk_hsdt1_eusb_START (23)
#define SOC_HSDT1_CRG_PEREN0_gt_clk_hsdt1_eusb_END (23)
#define SOC_HSDT1_CRG_PEREN0_gt_clk_ulpi_ref_START (24)
#define SOC_HSDT1_CRG_PEREN0_gt_clk_ulpi_ref_END (24)
#define SOC_HSDT1_CRG_PEREN0_gt_clk_usb2_ulpi_START (25)
#define SOC_HSDT1_CRG_PEREN0_gt_clk_usb2_ulpi_END (25)
#define SOC_HSDT1_CRG_PEREN0_gt_clk_usb31phy_apb_START (26)
#define SOC_HSDT1_CRG_PEREN0_gt_clk_usb31phy_apb_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_hclk_sd : 1;
        unsigned int gt_pclk_hsdt1_sysctrl : 1;
        unsigned int gt_aclk_dpctrl : 1;
        unsigned int gt_pclk_dpctrl : 1;
        unsigned int gt_pclk_hsdt1_ioc : 1;
        unsigned int gt_hclk_usb3otg_misc : 1;
        unsigned int gt_aclk_usb3otg : 1;
        unsigned int gt_aclk_noc_dpctrl_asyncbrg : 1;
        unsigned int gt_aclk_noc_usb3otg_asyncbrg : 1;
        unsigned int gt_clk_pcie1aux : 1;
        unsigned int gt_clk_pcie1_hp : 1;
        unsigned int gt_clk_pcie1_debounce : 1;
        unsigned int gt_pclk_pcie1_phy : 1;
        unsigned int gt_pclk_pcie1_sys : 1;
        unsigned int gt_aclk_pcie1 : 1;
        unsigned int gt_clk_pcie1phy_ref_soft : 1;
        unsigned int gt_clk_pcie1io_soft : 1;
        unsigned int gt_aclk_noc_pcie1_asyncbrg : 1;
        unsigned int gt_clk_usb2phy_ref : 1;
        unsigned int gt_aclk_hsdt1_trace : 1;
        unsigned int gt_pclk_hsdt1_trace : 1;
        unsigned int gt_clk_hsdt1_tcu : 1;
        unsigned int gt_clk_hsdt1_tbu : 1;
        unsigned int gt_clk_hsdt1_eusb : 1;
        unsigned int gt_clk_ulpi_ref : 1;
        unsigned int gt_clk_usb2_ulpi : 1;
        unsigned int gt_clk_usb31phy_apb : 1;
        unsigned int reserved : 5;
    } reg;
} SOC_HSDT1_CRG_PERDIS0_UNION;
#endif
#define SOC_HSDT1_CRG_PERDIS0_gt_hclk_sd_START (0)
#define SOC_HSDT1_CRG_PERDIS0_gt_hclk_sd_END (0)
#define SOC_HSDT1_CRG_PERDIS0_gt_pclk_hsdt1_sysctrl_START (1)
#define SOC_HSDT1_CRG_PERDIS0_gt_pclk_hsdt1_sysctrl_END (1)
#define SOC_HSDT1_CRG_PERDIS0_gt_aclk_dpctrl_START (2)
#define SOC_HSDT1_CRG_PERDIS0_gt_aclk_dpctrl_END (2)
#define SOC_HSDT1_CRG_PERDIS0_gt_pclk_dpctrl_START (3)
#define SOC_HSDT1_CRG_PERDIS0_gt_pclk_dpctrl_END (3)
#define SOC_HSDT1_CRG_PERDIS0_gt_pclk_hsdt1_ioc_START (4)
#define SOC_HSDT1_CRG_PERDIS0_gt_pclk_hsdt1_ioc_END (4)
#define SOC_HSDT1_CRG_PERDIS0_gt_hclk_usb3otg_misc_START (5)
#define SOC_HSDT1_CRG_PERDIS0_gt_hclk_usb3otg_misc_END (5)
#define SOC_HSDT1_CRG_PERDIS0_gt_aclk_usb3otg_START (6)
#define SOC_HSDT1_CRG_PERDIS0_gt_aclk_usb3otg_END (6)
#define SOC_HSDT1_CRG_PERDIS0_gt_aclk_noc_dpctrl_asyncbrg_START (7)
#define SOC_HSDT1_CRG_PERDIS0_gt_aclk_noc_dpctrl_asyncbrg_END (7)
#define SOC_HSDT1_CRG_PERDIS0_gt_aclk_noc_usb3otg_asyncbrg_START (8)
#define SOC_HSDT1_CRG_PERDIS0_gt_aclk_noc_usb3otg_asyncbrg_END (8)
#define SOC_HSDT1_CRG_PERDIS0_gt_clk_pcie1aux_START (9)
#define SOC_HSDT1_CRG_PERDIS0_gt_clk_pcie1aux_END (9)
#define SOC_HSDT1_CRG_PERDIS0_gt_clk_pcie1_hp_START (10)
#define SOC_HSDT1_CRG_PERDIS0_gt_clk_pcie1_hp_END (10)
#define SOC_HSDT1_CRG_PERDIS0_gt_clk_pcie1_debounce_START (11)
#define SOC_HSDT1_CRG_PERDIS0_gt_clk_pcie1_debounce_END (11)
#define SOC_HSDT1_CRG_PERDIS0_gt_pclk_pcie1_phy_START (12)
#define SOC_HSDT1_CRG_PERDIS0_gt_pclk_pcie1_phy_END (12)
#define SOC_HSDT1_CRG_PERDIS0_gt_pclk_pcie1_sys_START (13)
#define SOC_HSDT1_CRG_PERDIS0_gt_pclk_pcie1_sys_END (13)
#define SOC_HSDT1_CRG_PERDIS0_gt_aclk_pcie1_START (14)
#define SOC_HSDT1_CRG_PERDIS0_gt_aclk_pcie1_END (14)
#define SOC_HSDT1_CRG_PERDIS0_gt_clk_pcie1phy_ref_soft_START (15)
#define SOC_HSDT1_CRG_PERDIS0_gt_clk_pcie1phy_ref_soft_END (15)
#define SOC_HSDT1_CRG_PERDIS0_gt_clk_pcie1io_soft_START (16)
#define SOC_HSDT1_CRG_PERDIS0_gt_clk_pcie1io_soft_END (16)
#define SOC_HSDT1_CRG_PERDIS0_gt_aclk_noc_pcie1_asyncbrg_START (17)
#define SOC_HSDT1_CRG_PERDIS0_gt_aclk_noc_pcie1_asyncbrg_END (17)
#define SOC_HSDT1_CRG_PERDIS0_gt_clk_usb2phy_ref_START (18)
#define SOC_HSDT1_CRG_PERDIS0_gt_clk_usb2phy_ref_END (18)
#define SOC_HSDT1_CRG_PERDIS0_gt_aclk_hsdt1_trace_START (19)
#define SOC_HSDT1_CRG_PERDIS0_gt_aclk_hsdt1_trace_END (19)
#define SOC_HSDT1_CRG_PERDIS0_gt_pclk_hsdt1_trace_START (20)
#define SOC_HSDT1_CRG_PERDIS0_gt_pclk_hsdt1_trace_END (20)
#define SOC_HSDT1_CRG_PERDIS0_gt_clk_hsdt1_tcu_START (21)
#define SOC_HSDT1_CRG_PERDIS0_gt_clk_hsdt1_tcu_END (21)
#define SOC_HSDT1_CRG_PERDIS0_gt_clk_hsdt1_tbu_START (22)
#define SOC_HSDT1_CRG_PERDIS0_gt_clk_hsdt1_tbu_END (22)
#define SOC_HSDT1_CRG_PERDIS0_gt_clk_hsdt1_eusb_START (23)
#define SOC_HSDT1_CRG_PERDIS0_gt_clk_hsdt1_eusb_END (23)
#define SOC_HSDT1_CRG_PERDIS0_gt_clk_ulpi_ref_START (24)
#define SOC_HSDT1_CRG_PERDIS0_gt_clk_ulpi_ref_END (24)
#define SOC_HSDT1_CRG_PERDIS0_gt_clk_usb2_ulpi_START (25)
#define SOC_HSDT1_CRG_PERDIS0_gt_clk_usb2_ulpi_END (25)
#define SOC_HSDT1_CRG_PERDIS0_gt_clk_usb31phy_apb_START (26)
#define SOC_HSDT1_CRG_PERDIS0_gt_clk_usb31phy_apb_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_hclk_sd : 1;
        unsigned int gt_pclk_hsdt1_sysctrl : 1;
        unsigned int gt_aclk_dpctrl : 1;
        unsigned int gt_pclk_dpctrl : 1;
        unsigned int gt_pclk_hsdt1_ioc : 1;
        unsigned int gt_hclk_usb3otg_misc : 1;
        unsigned int gt_aclk_usb3otg : 1;
        unsigned int gt_aclk_noc_dpctrl_asyncbrg : 1;
        unsigned int gt_aclk_noc_usb3otg_asyncbrg : 1;
        unsigned int gt_clk_pcie1aux : 1;
        unsigned int gt_clk_pcie1_hp : 1;
        unsigned int gt_clk_pcie1_debounce : 1;
        unsigned int gt_pclk_pcie1_phy : 1;
        unsigned int gt_pclk_pcie1_sys : 1;
        unsigned int gt_aclk_pcie1 : 1;
        unsigned int gt_clk_pcie1phy_ref_soft : 1;
        unsigned int gt_clk_pcie1io_soft : 1;
        unsigned int gt_aclk_noc_pcie1_asyncbrg : 1;
        unsigned int gt_clk_usb2phy_ref : 1;
        unsigned int gt_aclk_hsdt1_trace : 1;
        unsigned int gt_pclk_hsdt1_trace : 1;
        unsigned int gt_clk_hsdt1_tcu : 1;
        unsigned int gt_clk_hsdt1_tbu : 1;
        unsigned int gt_clk_hsdt1_eusb : 1;
        unsigned int gt_clk_ulpi_ref : 1;
        unsigned int gt_clk_usb2_ulpi : 1;
        unsigned int gt_clk_usb31phy_apb : 1;
        unsigned int reserved : 5;
    } reg;
} SOC_HSDT1_CRG_PERCLKEN0_UNION;
#endif
#define SOC_HSDT1_CRG_PERCLKEN0_gt_hclk_sd_START (0)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_hclk_sd_END (0)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_pclk_hsdt1_sysctrl_START (1)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_pclk_hsdt1_sysctrl_END (1)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_aclk_dpctrl_START (2)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_aclk_dpctrl_END (2)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_pclk_dpctrl_START (3)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_pclk_dpctrl_END (3)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_pclk_hsdt1_ioc_START (4)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_pclk_hsdt1_ioc_END (4)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_hclk_usb3otg_misc_START (5)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_hclk_usb3otg_misc_END (5)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_aclk_usb3otg_START (6)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_aclk_usb3otg_END (6)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_aclk_noc_dpctrl_asyncbrg_START (7)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_aclk_noc_dpctrl_asyncbrg_END (7)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_aclk_noc_usb3otg_asyncbrg_START (8)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_aclk_noc_usb3otg_asyncbrg_END (8)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_clk_pcie1aux_START (9)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_clk_pcie1aux_END (9)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_clk_pcie1_hp_START (10)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_clk_pcie1_hp_END (10)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_clk_pcie1_debounce_START (11)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_clk_pcie1_debounce_END (11)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_pclk_pcie1_phy_START (12)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_pclk_pcie1_phy_END (12)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_pclk_pcie1_sys_START (13)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_pclk_pcie1_sys_END (13)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_aclk_pcie1_START (14)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_aclk_pcie1_END (14)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_clk_pcie1phy_ref_soft_START (15)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_clk_pcie1phy_ref_soft_END (15)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_clk_pcie1io_soft_START (16)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_clk_pcie1io_soft_END (16)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_aclk_noc_pcie1_asyncbrg_START (17)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_aclk_noc_pcie1_asyncbrg_END (17)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_clk_usb2phy_ref_START (18)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_clk_usb2phy_ref_END (18)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_aclk_hsdt1_trace_START (19)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_aclk_hsdt1_trace_END (19)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_pclk_hsdt1_trace_START (20)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_pclk_hsdt1_trace_END (20)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_clk_hsdt1_tcu_START (21)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_clk_hsdt1_tcu_END (21)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_clk_hsdt1_tbu_START (22)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_clk_hsdt1_tbu_END (22)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_clk_hsdt1_eusb_START (23)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_clk_hsdt1_eusb_END (23)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_clk_ulpi_ref_START (24)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_clk_ulpi_ref_END (24)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_clk_usb2_ulpi_START (25)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_clk_usb2_ulpi_END (25)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_clk_usb31phy_apb_START (26)
#define SOC_HSDT1_CRG_PERCLKEN0_gt_clk_usb31phy_apb_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int st_hclk_sd : 1;
        unsigned int st_pclk_hsdt1_sysctrl : 1;
        unsigned int st_aclk_dpctrl : 1;
        unsigned int st_pclk_dpctrl : 1;
        unsigned int st_pclk_hsdt1_ioc : 1;
        unsigned int st_hclk_usb3otg_misc : 1;
        unsigned int st_aclk_usb3otg : 1;
        unsigned int st_aclk_noc_dpctrl_asyncbrg : 1;
        unsigned int st_aclk_noc_usb3otg_asyncbrg : 1;
        unsigned int st_clk_pcie1aux : 1;
        unsigned int st_clk_pcie1_hp : 1;
        unsigned int st_clk_pcie1_debounce : 1;
        unsigned int st_pclk_pcie1_phy : 1;
        unsigned int st_pclk_pcie1_sys : 1;
        unsigned int st_aclk_pcie1 : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int st_aclk_noc_pcie1_asyncbrg : 1;
        unsigned int st_clk_usb2phy_ref : 1;
        unsigned int st_aclk_hsdt1_trace : 1;
        unsigned int st_pclk_hsdt1_trace : 1;
        unsigned int st_clk_hsdt1_tcu : 1;
        unsigned int st_clk_hsdt1_tbu : 1;
        unsigned int st_clk_hsdt1_eusb : 1;
        unsigned int st_clk_ulpi_ref : 1;
        unsigned int st_clk_usb2_ulpi : 1;
        unsigned int st_clk_usb31phy_apb : 1;
        unsigned int reserved_2 : 5;
    } reg;
} SOC_HSDT1_CRG_PERSTAT0_UNION;
#endif
#define SOC_HSDT1_CRG_PERSTAT0_st_hclk_sd_START (0)
#define SOC_HSDT1_CRG_PERSTAT0_st_hclk_sd_END (0)
#define SOC_HSDT1_CRG_PERSTAT0_st_pclk_hsdt1_sysctrl_START (1)
#define SOC_HSDT1_CRG_PERSTAT0_st_pclk_hsdt1_sysctrl_END (1)
#define SOC_HSDT1_CRG_PERSTAT0_st_aclk_dpctrl_START (2)
#define SOC_HSDT1_CRG_PERSTAT0_st_aclk_dpctrl_END (2)
#define SOC_HSDT1_CRG_PERSTAT0_st_pclk_dpctrl_START (3)
#define SOC_HSDT1_CRG_PERSTAT0_st_pclk_dpctrl_END (3)
#define SOC_HSDT1_CRG_PERSTAT0_st_pclk_hsdt1_ioc_START (4)
#define SOC_HSDT1_CRG_PERSTAT0_st_pclk_hsdt1_ioc_END (4)
#define SOC_HSDT1_CRG_PERSTAT0_st_hclk_usb3otg_misc_START (5)
#define SOC_HSDT1_CRG_PERSTAT0_st_hclk_usb3otg_misc_END (5)
#define SOC_HSDT1_CRG_PERSTAT0_st_aclk_usb3otg_START (6)
#define SOC_HSDT1_CRG_PERSTAT0_st_aclk_usb3otg_END (6)
#define SOC_HSDT1_CRG_PERSTAT0_st_aclk_noc_dpctrl_asyncbrg_START (7)
#define SOC_HSDT1_CRG_PERSTAT0_st_aclk_noc_dpctrl_asyncbrg_END (7)
#define SOC_HSDT1_CRG_PERSTAT0_st_aclk_noc_usb3otg_asyncbrg_START (8)
#define SOC_HSDT1_CRG_PERSTAT0_st_aclk_noc_usb3otg_asyncbrg_END (8)
#define SOC_HSDT1_CRG_PERSTAT0_st_clk_pcie1aux_START (9)
#define SOC_HSDT1_CRG_PERSTAT0_st_clk_pcie1aux_END (9)
#define SOC_HSDT1_CRG_PERSTAT0_st_clk_pcie1_hp_START (10)
#define SOC_HSDT1_CRG_PERSTAT0_st_clk_pcie1_hp_END (10)
#define SOC_HSDT1_CRG_PERSTAT0_st_clk_pcie1_debounce_START (11)
#define SOC_HSDT1_CRG_PERSTAT0_st_clk_pcie1_debounce_END (11)
#define SOC_HSDT1_CRG_PERSTAT0_st_pclk_pcie1_phy_START (12)
#define SOC_HSDT1_CRG_PERSTAT0_st_pclk_pcie1_phy_END (12)
#define SOC_HSDT1_CRG_PERSTAT0_st_pclk_pcie1_sys_START (13)
#define SOC_HSDT1_CRG_PERSTAT0_st_pclk_pcie1_sys_END (13)
#define SOC_HSDT1_CRG_PERSTAT0_st_aclk_pcie1_START (14)
#define SOC_HSDT1_CRG_PERSTAT0_st_aclk_pcie1_END (14)
#define SOC_HSDT1_CRG_PERSTAT0_st_aclk_noc_pcie1_asyncbrg_START (17)
#define SOC_HSDT1_CRG_PERSTAT0_st_aclk_noc_pcie1_asyncbrg_END (17)
#define SOC_HSDT1_CRG_PERSTAT0_st_clk_usb2phy_ref_START (18)
#define SOC_HSDT1_CRG_PERSTAT0_st_clk_usb2phy_ref_END (18)
#define SOC_HSDT1_CRG_PERSTAT0_st_aclk_hsdt1_trace_START (19)
#define SOC_HSDT1_CRG_PERSTAT0_st_aclk_hsdt1_trace_END (19)
#define SOC_HSDT1_CRG_PERSTAT0_st_pclk_hsdt1_trace_START (20)
#define SOC_HSDT1_CRG_PERSTAT0_st_pclk_hsdt1_trace_END (20)
#define SOC_HSDT1_CRG_PERSTAT0_st_clk_hsdt1_tcu_START (21)
#define SOC_HSDT1_CRG_PERSTAT0_st_clk_hsdt1_tcu_END (21)
#define SOC_HSDT1_CRG_PERSTAT0_st_clk_hsdt1_tbu_START (22)
#define SOC_HSDT1_CRG_PERSTAT0_st_clk_hsdt1_tbu_END (22)
#define SOC_HSDT1_CRG_PERSTAT0_st_clk_hsdt1_eusb_START (23)
#define SOC_HSDT1_CRG_PERSTAT0_st_clk_hsdt1_eusb_END (23)
#define SOC_HSDT1_CRG_PERSTAT0_st_clk_ulpi_ref_START (24)
#define SOC_HSDT1_CRG_PERSTAT0_st_clk_ulpi_ref_END (24)
#define SOC_HSDT1_CRG_PERSTAT0_st_clk_usb2_ulpi_START (25)
#define SOC_HSDT1_CRG_PERSTAT0_st_clk_usb2_ulpi_END (25)
#define SOC_HSDT1_CRG_PERSTAT0_st_clk_usb31phy_apb_START (26)
#define SOC_HSDT1_CRG_PERSTAT0_st_clk_usb31phy_apb_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_dp_dpi_syp0 : 1;
        unsigned int gt_clk_dp_dpi_syp1 : 1;
        unsigned int gt_clk_hsdt1_usbdp_mcu : 1;
        unsigned int gt_clk_hsdt1_pcie1_mcu : 1;
        unsigned int gt_clk_hsdt1_usbdp_mcu_32k : 1;
        unsigned int gt_clk_hsdt1_pcie1_mcu_32k : 1;
        unsigned int gt_clk_hsdt1_usbdp_mcu_19p2 : 1;
        unsigned int gt_clk_hsdt1_pcie1_mcu_19p2 : 1;
        unsigned int gt_clk_hsdt1_usbdp_mcu_bus : 1;
        unsigned int gt_clk_hsdt1_pcie1_mcu_bus : 1;
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
        unsigned int reserved_16 : 6;
    } reg;
} SOC_HSDT1_CRG_PEREN1_UNION;
#endif
#define SOC_HSDT1_CRG_PEREN1_gt_clk_dp_dpi_syp0_START (0)
#define SOC_HSDT1_CRG_PEREN1_gt_clk_dp_dpi_syp0_END (0)
#define SOC_HSDT1_CRG_PEREN1_gt_clk_dp_dpi_syp1_START (1)
#define SOC_HSDT1_CRG_PEREN1_gt_clk_dp_dpi_syp1_END (1)
#define SOC_HSDT1_CRG_PEREN1_gt_clk_hsdt1_usbdp_mcu_START (2)
#define SOC_HSDT1_CRG_PEREN1_gt_clk_hsdt1_usbdp_mcu_END (2)
#define SOC_HSDT1_CRG_PEREN1_gt_clk_hsdt1_pcie1_mcu_START (3)
#define SOC_HSDT1_CRG_PEREN1_gt_clk_hsdt1_pcie1_mcu_END (3)
#define SOC_HSDT1_CRG_PEREN1_gt_clk_hsdt1_usbdp_mcu_32k_START (4)
#define SOC_HSDT1_CRG_PEREN1_gt_clk_hsdt1_usbdp_mcu_32k_END (4)
#define SOC_HSDT1_CRG_PEREN1_gt_clk_hsdt1_pcie1_mcu_32k_START (5)
#define SOC_HSDT1_CRG_PEREN1_gt_clk_hsdt1_pcie1_mcu_32k_END (5)
#define SOC_HSDT1_CRG_PEREN1_gt_clk_hsdt1_usbdp_mcu_19p2_START (6)
#define SOC_HSDT1_CRG_PEREN1_gt_clk_hsdt1_usbdp_mcu_19p2_END (6)
#define SOC_HSDT1_CRG_PEREN1_gt_clk_hsdt1_pcie1_mcu_19p2_START (7)
#define SOC_HSDT1_CRG_PEREN1_gt_clk_hsdt1_pcie1_mcu_19p2_END (7)
#define SOC_HSDT1_CRG_PEREN1_gt_clk_hsdt1_usbdp_mcu_bus_START (8)
#define SOC_HSDT1_CRG_PEREN1_gt_clk_hsdt1_usbdp_mcu_bus_END (8)
#define SOC_HSDT1_CRG_PEREN1_gt_clk_hsdt1_pcie1_mcu_bus_START (9)
#define SOC_HSDT1_CRG_PEREN1_gt_clk_hsdt1_pcie1_mcu_bus_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_dp_dpi_syp0 : 1;
        unsigned int gt_clk_dp_dpi_syp1 : 1;
        unsigned int gt_clk_hsdt1_usbdp_mcu : 1;
        unsigned int gt_clk_hsdt1_pcie1_mcu : 1;
        unsigned int gt_clk_hsdt1_usbdp_mcu_32k : 1;
        unsigned int gt_clk_hsdt1_pcie1_mcu_32k : 1;
        unsigned int gt_clk_hsdt1_usbdp_mcu_19p2 : 1;
        unsigned int gt_clk_hsdt1_pcie1_mcu_19p2 : 1;
        unsigned int gt_clk_hsdt1_usbdp_mcu_bus : 1;
        unsigned int gt_clk_hsdt1_pcie1_mcu_bus : 1;
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
        unsigned int reserved_16 : 6;
    } reg;
} SOC_HSDT1_CRG_PERDIS1_UNION;
#endif
#define SOC_HSDT1_CRG_PERDIS1_gt_clk_dp_dpi_syp0_START (0)
#define SOC_HSDT1_CRG_PERDIS1_gt_clk_dp_dpi_syp0_END (0)
#define SOC_HSDT1_CRG_PERDIS1_gt_clk_dp_dpi_syp1_START (1)
#define SOC_HSDT1_CRG_PERDIS1_gt_clk_dp_dpi_syp1_END (1)
#define SOC_HSDT1_CRG_PERDIS1_gt_clk_hsdt1_usbdp_mcu_START (2)
#define SOC_HSDT1_CRG_PERDIS1_gt_clk_hsdt1_usbdp_mcu_END (2)
#define SOC_HSDT1_CRG_PERDIS1_gt_clk_hsdt1_pcie1_mcu_START (3)
#define SOC_HSDT1_CRG_PERDIS1_gt_clk_hsdt1_pcie1_mcu_END (3)
#define SOC_HSDT1_CRG_PERDIS1_gt_clk_hsdt1_usbdp_mcu_32k_START (4)
#define SOC_HSDT1_CRG_PERDIS1_gt_clk_hsdt1_usbdp_mcu_32k_END (4)
#define SOC_HSDT1_CRG_PERDIS1_gt_clk_hsdt1_pcie1_mcu_32k_START (5)
#define SOC_HSDT1_CRG_PERDIS1_gt_clk_hsdt1_pcie1_mcu_32k_END (5)
#define SOC_HSDT1_CRG_PERDIS1_gt_clk_hsdt1_usbdp_mcu_19p2_START (6)
#define SOC_HSDT1_CRG_PERDIS1_gt_clk_hsdt1_usbdp_mcu_19p2_END (6)
#define SOC_HSDT1_CRG_PERDIS1_gt_clk_hsdt1_pcie1_mcu_19p2_START (7)
#define SOC_HSDT1_CRG_PERDIS1_gt_clk_hsdt1_pcie1_mcu_19p2_END (7)
#define SOC_HSDT1_CRG_PERDIS1_gt_clk_hsdt1_usbdp_mcu_bus_START (8)
#define SOC_HSDT1_CRG_PERDIS1_gt_clk_hsdt1_usbdp_mcu_bus_END (8)
#define SOC_HSDT1_CRG_PERDIS1_gt_clk_hsdt1_pcie1_mcu_bus_START (9)
#define SOC_HSDT1_CRG_PERDIS1_gt_clk_hsdt1_pcie1_mcu_bus_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_dp_dpi_syp0 : 1;
        unsigned int gt_clk_dp_dpi_syp1 : 1;
        unsigned int gt_clk_hsdt1_usbdp_mcu : 1;
        unsigned int gt_clk_hsdt1_pcie1_mcu : 1;
        unsigned int gt_clk_hsdt1_usbdp_mcu_32k : 1;
        unsigned int gt_clk_hsdt1_pcie1_mcu_32k : 1;
        unsigned int gt_clk_hsdt1_usbdp_mcu_19p2 : 1;
        unsigned int gt_clk_hsdt1_pcie1_mcu_19p2 : 1;
        unsigned int gt_clk_hsdt1_usbdp_mcu_bus : 1;
        unsigned int gt_clk_hsdt1_pcie1_mcu_bus : 1;
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
        unsigned int reserved_16 : 6;
    } reg;
} SOC_HSDT1_CRG_PERCLKEN1_UNION;
#endif
#define SOC_HSDT1_CRG_PERCLKEN1_gt_clk_dp_dpi_syp0_START (0)
#define SOC_HSDT1_CRG_PERCLKEN1_gt_clk_dp_dpi_syp0_END (0)
#define SOC_HSDT1_CRG_PERCLKEN1_gt_clk_dp_dpi_syp1_START (1)
#define SOC_HSDT1_CRG_PERCLKEN1_gt_clk_dp_dpi_syp1_END (1)
#define SOC_HSDT1_CRG_PERCLKEN1_gt_clk_hsdt1_usbdp_mcu_START (2)
#define SOC_HSDT1_CRG_PERCLKEN1_gt_clk_hsdt1_usbdp_mcu_END (2)
#define SOC_HSDT1_CRG_PERCLKEN1_gt_clk_hsdt1_pcie1_mcu_START (3)
#define SOC_HSDT1_CRG_PERCLKEN1_gt_clk_hsdt1_pcie1_mcu_END (3)
#define SOC_HSDT1_CRG_PERCLKEN1_gt_clk_hsdt1_usbdp_mcu_32k_START (4)
#define SOC_HSDT1_CRG_PERCLKEN1_gt_clk_hsdt1_usbdp_mcu_32k_END (4)
#define SOC_HSDT1_CRG_PERCLKEN1_gt_clk_hsdt1_pcie1_mcu_32k_START (5)
#define SOC_HSDT1_CRG_PERCLKEN1_gt_clk_hsdt1_pcie1_mcu_32k_END (5)
#define SOC_HSDT1_CRG_PERCLKEN1_gt_clk_hsdt1_usbdp_mcu_19p2_START (6)
#define SOC_HSDT1_CRG_PERCLKEN1_gt_clk_hsdt1_usbdp_mcu_19p2_END (6)
#define SOC_HSDT1_CRG_PERCLKEN1_gt_clk_hsdt1_pcie1_mcu_19p2_START (7)
#define SOC_HSDT1_CRG_PERCLKEN1_gt_clk_hsdt1_pcie1_mcu_19p2_END (7)
#define SOC_HSDT1_CRG_PERCLKEN1_gt_clk_hsdt1_usbdp_mcu_bus_START (8)
#define SOC_HSDT1_CRG_PERCLKEN1_gt_clk_hsdt1_usbdp_mcu_bus_END (8)
#define SOC_HSDT1_CRG_PERCLKEN1_gt_clk_hsdt1_pcie1_mcu_bus_START (9)
#define SOC_HSDT1_CRG_PERCLKEN1_gt_clk_hsdt1_pcie1_mcu_bus_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int st_clk_dp_dpi_syp0 : 1;
        unsigned int st_clk_dp_dpi_syp1 : 1;
        unsigned int st_clk_hsdt1_usbdp_mcu : 1;
        unsigned int st_clk_hsdt1_pcie1_mcu : 1;
        unsigned int st_clk_hsdt1_usbdp_mcu_32k : 1;
        unsigned int st_clk_hsdt1_pcie1_mcu_32k : 1;
        unsigned int st_clk_hsdt1_usbdp_mcu_19p2 : 1;
        unsigned int st_clk_hsdt1_pcie1_mcu_19p2 : 1;
        unsigned int st_clk_hsdt1_usbdp_mcu_bus : 1;
        unsigned int st_clk_hsdt1_pcie1_mcu_bus : 1;
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
        unsigned int reserved_16 : 6;
    } reg;
} SOC_HSDT1_CRG_PERSTAT1_UNION;
#endif
#define SOC_HSDT1_CRG_PERSTAT1_st_clk_dp_dpi_syp0_START (0)
#define SOC_HSDT1_CRG_PERSTAT1_st_clk_dp_dpi_syp0_END (0)
#define SOC_HSDT1_CRG_PERSTAT1_st_clk_dp_dpi_syp1_START (1)
#define SOC_HSDT1_CRG_PERSTAT1_st_clk_dp_dpi_syp1_END (1)
#define SOC_HSDT1_CRG_PERSTAT1_st_clk_hsdt1_usbdp_mcu_START (2)
#define SOC_HSDT1_CRG_PERSTAT1_st_clk_hsdt1_usbdp_mcu_END (2)
#define SOC_HSDT1_CRG_PERSTAT1_st_clk_hsdt1_pcie1_mcu_START (3)
#define SOC_HSDT1_CRG_PERSTAT1_st_clk_hsdt1_pcie1_mcu_END (3)
#define SOC_HSDT1_CRG_PERSTAT1_st_clk_hsdt1_usbdp_mcu_32k_START (4)
#define SOC_HSDT1_CRG_PERSTAT1_st_clk_hsdt1_usbdp_mcu_32k_END (4)
#define SOC_HSDT1_CRG_PERSTAT1_st_clk_hsdt1_pcie1_mcu_32k_START (5)
#define SOC_HSDT1_CRG_PERSTAT1_st_clk_hsdt1_pcie1_mcu_32k_END (5)
#define SOC_HSDT1_CRG_PERSTAT1_st_clk_hsdt1_usbdp_mcu_19p2_START (6)
#define SOC_HSDT1_CRG_PERSTAT1_st_clk_hsdt1_usbdp_mcu_19p2_END (6)
#define SOC_HSDT1_CRG_PERSTAT1_st_clk_hsdt1_pcie1_mcu_19p2_START (7)
#define SOC_HSDT1_CRG_PERSTAT1_st_clk_hsdt1_pcie1_mcu_19p2_END (7)
#define SOC_HSDT1_CRG_PERSTAT1_st_clk_hsdt1_usbdp_mcu_bus_START (8)
#define SOC_HSDT1_CRG_PERSTAT1_st_clk_hsdt1_usbdp_mcu_bus_END (8)
#define SOC_HSDT1_CRG_PERSTAT1_st_clk_hsdt1_pcie1_mcu_bus_START (9)
#define SOC_HSDT1_CRG_PERSTAT1_st_clk_hsdt1_pcie1_mcu_bus_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_hrst_sd : 1;
        unsigned int ip_rst_sd : 1;
        unsigned int ip_prst_hsdt1_sysctrl : 1;
        unsigned int ip_rst_dpctrl : 1;
        unsigned int ip_prst_hsdt1_ioc : 1;
        unsigned int ip_hrst_usb3otg_misc : 1;
        unsigned int ip_rst_usb3otg_32k : 1;
        unsigned int ip_rst_pcie1 : 1;
        unsigned int ip_prst_pcie1_sys : 1;
        unsigned int ip_prst_pcie1_phy : 1;
        unsigned int ip_arst_pcie1_asyncbrg : 1;
        unsigned int ip_prst_hsdt1_trace : 1;
        unsigned int ip_rst_hsdt1_trace : 1;
        unsigned int ip_rst_hsdt1_tcu : 1;
        unsigned int ip_rst_hsdt1_tbu : 1;
        unsigned int ip_rst_hsdt1_eusb : 1;
        unsigned int ip_rst_dpctrl_hisi : 1;
        unsigned int ip_rst_hsdt1_usbdp_mcu : 1;
        unsigned int ip_rst_hsdt1_pcie1_mcu : 1;
        unsigned int ip_rst_hsdt1_usbdp_mcu_32k : 1;
        unsigned int ip_rst_hsdt1_pcie1_mcu_32k : 1;
        unsigned int ip_rst_hsdt1_usbdp_mcu_19p2 : 1;
        unsigned int ip_rst_hsdt1_pcie1_mcu_19p2 : 1;
        unsigned int ip_rst_hsdt1_usbdp_mcu_bus : 1;
        unsigned int ip_rst_hsdt1_pcie1_mcu_bus : 1;
        unsigned int reserved : 7;
    } reg;
} SOC_HSDT1_CRG_PERRSTEN0_UNION;
#endif
#define SOC_HSDT1_CRG_PERRSTEN0_ip_hrst_sd_START (0)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_hrst_sd_END (0)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_sd_START (1)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_sd_END (1)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_prst_hsdt1_sysctrl_START (2)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_prst_hsdt1_sysctrl_END (2)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_dpctrl_START (3)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_dpctrl_END (3)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_prst_hsdt1_ioc_START (4)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_prst_hsdt1_ioc_END (4)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_hrst_usb3otg_misc_START (5)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_hrst_usb3otg_misc_END (5)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_usb3otg_32k_START (6)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_usb3otg_32k_END (6)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_pcie1_START (7)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_pcie1_END (7)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_prst_pcie1_sys_START (8)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_prst_pcie1_sys_END (8)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_prst_pcie1_phy_START (9)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_prst_pcie1_phy_END (9)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_arst_pcie1_asyncbrg_START (10)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_arst_pcie1_asyncbrg_END (10)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_prst_hsdt1_trace_START (11)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_prst_hsdt1_trace_END (11)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_hsdt1_trace_START (12)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_hsdt1_trace_END (12)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_hsdt1_tcu_START (13)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_hsdt1_tcu_END (13)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_hsdt1_tbu_START (14)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_hsdt1_tbu_END (14)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_hsdt1_eusb_START (15)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_hsdt1_eusb_END (15)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_dpctrl_hisi_START (16)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_dpctrl_hisi_END (16)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_hsdt1_usbdp_mcu_START (17)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_hsdt1_usbdp_mcu_END (17)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_hsdt1_pcie1_mcu_START (18)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_hsdt1_pcie1_mcu_END (18)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_hsdt1_usbdp_mcu_32k_START (19)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_hsdt1_usbdp_mcu_32k_END (19)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_hsdt1_pcie1_mcu_32k_START (20)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_hsdt1_pcie1_mcu_32k_END (20)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_hsdt1_usbdp_mcu_19p2_START (21)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_hsdt1_usbdp_mcu_19p2_END (21)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_hsdt1_pcie1_mcu_19p2_START (22)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_hsdt1_pcie1_mcu_19p2_END (22)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_hsdt1_usbdp_mcu_bus_START (23)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_hsdt1_usbdp_mcu_bus_END (23)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_hsdt1_pcie1_mcu_bus_START (24)
#define SOC_HSDT1_CRG_PERRSTEN0_ip_rst_hsdt1_pcie1_mcu_bus_END (24)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_hrst_sd : 1;
        unsigned int ip_rst_sd : 1;
        unsigned int ip_prst_hsdt1_sysctrl : 1;
        unsigned int ip_rst_dpctrl : 1;
        unsigned int ip_prst_hsdt1_ioc : 1;
        unsigned int ip_hrst_usb3otg_misc : 1;
        unsigned int ip_rst_usb3otg_32k : 1;
        unsigned int ip_rst_pcie1 : 1;
        unsigned int ip_prst_pcie1_sys : 1;
        unsigned int ip_prst_pcie1_phy : 1;
        unsigned int ip_arst_pcie1_asyncbrg : 1;
        unsigned int ip_prst_hsdt1_trace : 1;
        unsigned int ip_rst_hsdt1_trace : 1;
        unsigned int ip_rst_hsdt1_tcu : 1;
        unsigned int ip_rst_hsdt1_tbu : 1;
        unsigned int ip_rst_hsdt1_eusb : 1;
        unsigned int ip_rst_dpctrl_hisi : 1;
        unsigned int ip_rst_hsdt1_usbdp_mcu : 1;
        unsigned int ip_rst_hsdt1_pcie1_mcu : 1;
        unsigned int ip_rst_hsdt1_usbdp_mcu_32k : 1;
        unsigned int ip_rst_hsdt1_pcie1_mcu_32k : 1;
        unsigned int ip_rst_hsdt1_usbdp_mcu_19p2 : 1;
        unsigned int ip_rst_hsdt1_pcie1_mcu_19p2 : 1;
        unsigned int ip_rst_hsdt1_usbdp_mcu_bus : 1;
        unsigned int ip_rst_hsdt1_pcie1_mcu_bus : 1;
        unsigned int reserved : 7;
    } reg;
} SOC_HSDT1_CRG_PERRSTDIS0_UNION;
#endif
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_hrst_sd_START (0)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_hrst_sd_END (0)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_sd_START (1)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_sd_END (1)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_prst_hsdt1_sysctrl_START (2)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_prst_hsdt1_sysctrl_END (2)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_dpctrl_START (3)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_dpctrl_END (3)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_prst_hsdt1_ioc_START (4)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_prst_hsdt1_ioc_END (4)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_hrst_usb3otg_misc_START (5)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_hrst_usb3otg_misc_END (5)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_usb3otg_32k_START (6)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_usb3otg_32k_END (6)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_pcie1_START (7)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_pcie1_END (7)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_prst_pcie1_sys_START (8)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_prst_pcie1_sys_END (8)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_prst_pcie1_phy_START (9)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_prst_pcie1_phy_END (9)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_arst_pcie1_asyncbrg_START (10)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_arst_pcie1_asyncbrg_END (10)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_prst_hsdt1_trace_START (11)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_prst_hsdt1_trace_END (11)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_hsdt1_trace_START (12)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_hsdt1_trace_END (12)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_hsdt1_tcu_START (13)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_hsdt1_tcu_END (13)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_hsdt1_tbu_START (14)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_hsdt1_tbu_END (14)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_hsdt1_eusb_START (15)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_hsdt1_eusb_END (15)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_dpctrl_hisi_START (16)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_dpctrl_hisi_END (16)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_hsdt1_usbdp_mcu_START (17)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_hsdt1_usbdp_mcu_END (17)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_hsdt1_pcie1_mcu_START (18)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_hsdt1_pcie1_mcu_END (18)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_hsdt1_usbdp_mcu_32k_START (19)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_hsdt1_usbdp_mcu_32k_END (19)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_hsdt1_pcie1_mcu_32k_START (20)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_hsdt1_pcie1_mcu_32k_END (20)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_hsdt1_usbdp_mcu_19p2_START (21)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_hsdt1_usbdp_mcu_19p2_END (21)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_hsdt1_pcie1_mcu_19p2_START (22)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_hsdt1_pcie1_mcu_19p2_END (22)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_hsdt1_usbdp_mcu_bus_START (23)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_hsdt1_usbdp_mcu_bus_END (23)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_hsdt1_pcie1_mcu_bus_START (24)
#define SOC_HSDT1_CRG_PERRSTDIS0_ip_rst_hsdt1_pcie1_mcu_bus_END (24)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_hrst_sd : 1;
        unsigned int ip_rst_sd : 1;
        unsigned int ip_prst_hsdt1_sysctrl : 1;
        unsigned int ip_rst_dpctrl : 1;
        unsigned int ip_prst_hsdt1_ioc : 1;
        unsigned int ip_hrst_usb3otg_misc : 1;
        unsigned int ip_rst_usb3otg_32k : 1;
        unsigned int ip_rst_pcie1 : 1;
        unsigned int ip_prst_pcie1_sys : 1;
        unsigned int ip_prst_pcie1_phy : 1;
        unsigned int ip_arst_pcie1_asyncbrg : 1;
        unsigned int ip_prst_hsdt1_trace : 1;
        unsigned int ip_rst_hsdt1_trace : 1;
        unsigned int ip_rst_hsdt1_tcu : 1;
        unsigned int ip_rst_hsdt1_tbu : 1;
        unsigned int ip_rst_hsdt1_eusb : 1;
        unsigned int ip_rst_dpctrl_hisi : 1;
        unsigned int ip_rst_hsdt1_usbdp_mcu : 1;
        unsigned int ip_rst_hsdt1_pcie1_mcu : 1;
        unsigned int ip_rst_hsdt1_usbdp_mcu_32k : 1;
        unsigned int ip_rst_hsdt1_pcie1_mcu_32k : 1;
        unsigned int ip_rst_hsdt1_usbdp_mcu_19p2 : 1;
        unsigned int ip_rst_hsdt1_pcie1_mcu_19p2 : 1;
        unsigned int ip_rst_hsdt1_usbdp_mcu_bus : 1;
        unsigned int ip_rst_hsdt1_pcie1_mcu_bus : 1;
        unsigned int reserved : 7;
    } reg;
} SOC_HSDT1_CRG_PERRSTSTAT0_UNION;
#endif
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_hrst_sd_START (0)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_hrst_sd_END (0)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_sd_START (1)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_sd_END (1)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_prst_hsdt1_sysctrl_START (2)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_prst_hsdt1_sysctrl_END (2)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_dpctrl_START (3)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_dpctrl_END (3)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_prst_hsdt1_ioc_START (4)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_prst_hsdt1_ioc_END (4)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_hrst_usb3otg_misc_START (5)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_hrst_usb3otg_misc_END (5)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_usb3otg_32k_START (6)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_usb3otg_32k_END (6)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_pcie1_START (7)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_pcie1_END (7)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_prst_pcie1_sys_START (8)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_prst_pcie1_sys_END (8)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_prst_pcie1_phy_START (9)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_prst_pcie1_phy_END (9)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_arst_pcie1_asyncbrg_START (10)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_arst_pcie1_asyncbrg_END (10)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_prst_hsdt1_trace_START (11)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_prst_hsdt1_trace_END (11)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_hsdt1_trace_START (12)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_hsdt1_trace_END (12)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_hsdt1_tcu_START (13)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_hsdt1_tcu_END (13)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_hsdt1_tbu_START (14)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_hsdt1_tbu_END (14)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_hsdt1_eusb_START (15)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_hsdt1_eusb_END (15)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_dpctrl_hisi_START (16)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_dpctrl_hisi_END (16)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_hsdt1_usbdp_mcu_START (17)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_hsdt1_usbdp_mcu_END (17)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_hsdt1_pcie1_mcu_START (18)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_hsdt1_pcie1_mcu_END (18)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_hsdt1_usbdp_mcu_32k_START (19)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_hsdt1_usbdp_mcu_32k_END (19)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_hsdt1_pcie1_mcu_32k_START (20)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_hsdt1_pcie1_mcu_32k_END (20)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_hsdt1_usbdp_mcu_19p2_START (21)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_hsdt1_usbdp_mcu_19p2_END (21)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_hsdt1_pcie1_mcu_19p2_START (22)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_hsdt1_pcie1_mcu_19p2_END (22)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_hsdt1_usbdp_mcu_bus_START (23)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_hsdt1_usbdp_mcu_bus_END (23)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_hsdt1_pcie1_mcu_bus_START (24)
#define SOC_HSDT1_CRG_PERRSTSTAT0_ip_rst_hsdt1_pcie1_mcu_bus_END (24)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sc_gt_clk_pciephy_trans : 1;
        unsigned int sel_usb2phy_ref : 1;
        unsigned int sc_gt_clk_usb2phy_ref : 1;
        unsigned int sel_hsdt1_eusb : 1;
        unsigned int sc_gt_clk_usb2_ulpi : 1;
        unsigned int div_clk_usb2_ulpi : 4;
        unsigned int sel_ulpi_ref : 1;
        unsigned int reserved_0 : 4;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int bitmasken : 16;
    } reg;
} SOC_HSDT1_CRG_CLKDIV0_UNION;
#endif
#define SOC_HSDT1_CRG_CLKDIV0_sc_gt_clk_pciephy_trans_START (0)
#define SOC_HSDT1_CRG_CLKDIV0_sc_gt_clk_pciephy_trans_END (0)
#define SOC_HSDT1_CRG_CLKDIV0_sel_usb2phy_ref_START (1)
#define SOC_HSDT1_CRG_CLKDIV0_sel_usb2phy_ref_END (1)
#define SOC_HSDT1_CRG_CLKDIV0_sc_gt_clk_usb2phy_ref_START (2)
#define SOC_HSDT1_CRG_CLKDIV0_sc_gt_clk_usb2phy_ref_END (2)
#define SOC_HSDT1_CRG_CLKDIV0_sel_hsdt1_eusb_START (3)
#define SOC_HSDT1_CRG_CLKDIV0_sel_hsdt1_eusb_END (3)
#define SOC_HSDT1_CRG_CLKDIV0_sc_gt_clk_usb2_ulpi_START (4)
#define SOC_HSDT1_CRG_CLKDIV0_sc_gt_clk_usb2_ulpi_END (4)
#define SOC_HSDT1_CRG_CLKDIV0_div_clk_usb2_ulpi_START (5)
#define SOC_HSDT1_CRG_CLKDIV0_div_clk_usb2_ulpi_END (8)
#define SOC_HSDT1_CRG_CLKDIV0_sel_ulpi_ref_START (9)
#define SOC_HSDT1_CRG_CLKDIV0_sel_ulpi_ref_END (9)
#define SOC_HSDT1_CRG_CLKDIV0_bitmasken_START (16)
#define SOC_HSDT1_CRG_CLKDIV0_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int st_clk_pcie1phy_ref : 1;
        unsigned int st_clk_pcie1io : 1;
        unsigned int st_pclk_pcie1sys_asyncbrg : 1;
        unsigned int reserved : 29;
    } reg;
} SOC_HSDT1_CRG_PERI_STAT0_UNION;
#endif
#define SOC_HSDT1_CRG_PERI_STAT0_st_clk_pcie1phy_ref_START (0)
#define SOC_HSDT1_CRG_PERI_STAT0_st_clk_pcie1phy_ref_END (0)
#define SOC_HSDT1_CRG_PERI_STAT0_st_clk_pcie1io_START (1)
#define SOC_HSDT1_CRG_PERI_STAT0_st_clk_pcie1io_END (1)
#define SOC_HSDT1_CRG_PERI_STAT0_st_pclk_pcie1sys_asyncbrg_START (2)
#define SOC_HSDT1_CRG_PERI_STAT0_st_pclk_pcie1sys_asyncbrg_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sd_clkrst_bypass : 1;
        unsigned int dpctrl_clkrst_bypass : 1;
        unsigned int hsdt1_sysctrl_clkrst_bypass : 1;
        unsigned int hsdt1_ioc_clkrst_bypass : 1;
        unsigned int usb3otg_misc_clkrst_bypass : 1;
        unsigned int pcie1phy_clkrst_bypass : 1;
        unsigned int pcie1ctrl_clkrst_bypass : 1;
        unsigned int hsdt1trace_clkrst_bypass : 1;
        unsigned int hsdt1tcu_clkrst_bypass : 1;
        unsigned int hsdt1tbu_clkrst_bypass : 1;
        unsigned int hsdt1_eusb_clkrst_bypass : 1;
        unsigned int dpctrl_hisi_clkrst_bypass : 1;
        unsigned int hsdt1_usbdp_mcu_bus_clkrst_bypass : 1;
        unsigned int hsdt1_pcie1_mcu_bus_clkrst_bypass : 1;
        unsigned int hsdt1_usb31phy_apb_clkrst_bypass : 1;
        unsigned int reserved : 17;
    } reg;
} SOC_HSDT1_CRG_IPCLKRST_BYPASS0_UNION;
#endif
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_sd_clkrst_bypass_START (0)
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_sd_clkrst_bypass_END (0)
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_dpctrl_clkrst_bypass_START (1)
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_dpctrl_clkrst_bypass_END (1)
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_hsdt1_sysctrl_clkrst_bypass_START (2)
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_hsdt1_sysctrl_clkrst_bypass_END (2)
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_hsdt1_ioc_clkrst_bypass_START (3)
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_hsdt1_ioc_clkrst_bypass_END (3)
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_usb3otg_misc_clkrst_bypass_START (4)
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_usb3otg_misc_clkrst_bypass_END (4)
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_pcie1phy_clkrst_bypass_START (5)
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_pcie1phy_clkrst_bypass_END (5)
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_pcie1ctrl_clkrst_bypass_START (6)
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_pcie1ctrl_clkrst_bypass_END (6)
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_hsdt1trace_clkrst_bypass_START (7)
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_hsdt1trace_clkrst_bypass_END (7)
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_hsdt1tcu_clkrst_bypass_START (8)
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_hsdt1tcu_clkrst_bypass_END (8)
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_hsdt1tbu_clkrst_bypass_START (9)
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_hsdt1tbu_clkrst_bypass_END (9)
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_hsdt1_eusb_clkrst_bypass_START (10)
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_hsdt1_eusb_clkrst_bypass_END (10)
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_dpctrl_hisi_clkrst_bypass_START (11)
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_dpctrl_hisi_clkrst_bypass_END (11)
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_hsdt1_usbdp_mcu_bus_clkrst_bypass_START (12)
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_hsdt1_usbdp_mcu_bus_clkrst_bypass_END (12)
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_hsdt1_pcie1_mcu_bus_clkrst_bypass_START (13)
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_hsdt1_pcie1_mcu_bus_clkrst_bypass_END (13)
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_hsdt1_usb31phy_apb_clkrst_bypass_START (14)
#define SOC_HSDT1_CRG_IPCLKRST_BYPASS0_hsdt1_usb31phy_apb_clkrst_bypass_END (14)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pciepll_en : 1;
        unsigned int pciepll_bp : 1;
        unsigned int pciepll_refdiv : 6;
        unsigned int pciepll_fbdiv : 12;
        unsigned int pciepll_postdiv1 : 3;
        unsigned int pciepll_postdiv2 : 3;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 5;
    } reg;
} SOC_HSDT1_CRG_PCIEPLL_CTRL0_UNION;
#endif
#define SOC_HSDT1_CRG_PCIEPLL_CTRL0_pciepll_en_START (0)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL0_pciepll_en_END (0)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL0_pciepll_bp_START (1)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL0_pciepll_bp_END (1)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL0_pciepll_refdiv_START (2)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL0_pciepll_refdiv_END (7)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL0_pciepll_fbdiv_START (8)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL0_pciepll_fbdiv_END (19)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL0_pciepll_postdiv1_START (20)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL0_pciepll_postdiv1_END (22)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL0_pciepll_postdiv2_START (23)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL0_pciepll_postdiv2_END (25)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pciepll_fracdiv : 24;
        unsigned int pciepll_int_mod : 1;
        unsigned int pciepll_cfg_vld : 1;
        unsigned int gt_clk_pciepll : 1;
        unsigned int pciepll_sel : 1;
        unsigned int reserved : 4;
    } reg;
} SOC_HSDT1_CRG_PCIEPLL_CTRL1_UNION;
#endif
#define SOC_HSDT1_CRG_PCIEPLL_CTRL1_pciepll_fracdiv_START (0)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL1_pciepll_fracdiv_END (23)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL1_pciepll_int_mod_START (24)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL1_pciepll_int_mod_END (24)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL1_pciepll_cfg_vld_START (25)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL1_pciepll_cfg_vld_END (25)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL1_gt_clk_pciepll_START (26)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL1_gt_clk_pciepll_END (26)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL1_pciepll_sel_START (27)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL1_pciepll_sel_END (27)
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
} SOC_HSDT1_CRG_PCIEPLL_CTRL_A_UNION;
#endif
#define SOC_HSDT1_CRG_PCIEPLL_CTRL_A_pll_bbdrv_START (0)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL_A_pll_bbdrv_END (2)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL_A_pll_bben_START (3)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL_A_pll_bben_END (3)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL_A_pll_bbenoverride_START (4)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL_A_pll_bbenoverride_END (4)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL_A_pll_bbpw_START (5)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL_A_pll_bbpw_END (7)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL_A_pll_bwprog_START (8)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL_A_pll_bwprog_END (10)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL_A_pll_bwprogen_START (11)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL_A_pll_bwprogen_END (11)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL_A_pll_foutvco_en_START (12)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL_A_pll_foutvco_en_END (12)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL_A_pll_offsetcalbyp_START (13)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL_A_pll_offsetcalbyp_END (13)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL_A_pll_offsetcalcnt_START (14)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL_A_pll_offsetcalcnt_END (16)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL_A_pll_offsetcalen_START (17)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL_A_pll_offsetcalen_END (17)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL_A_pll_offsetcalin_START (18)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL_A_pll_offsetcalin_END (29)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL_A_pll_offsetfastcal_START (30)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL_A_pll_offsetfastcal_END (30)
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
} SOC_HSDT1_CRG_PCIEPLL_CTRL_B_UNION;
#endif
#define SOC_HSDT1_CRG_PCIEPLL_CTRL_B_pll_vcosel_START (0)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL_B_pll_vcosel_END (1)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL_B_pll_qpiprog_START (2)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL_B_pll_qpiprog_END (4)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL_B_pll_qpiprogen_START (5)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL_B_pll_qpiprogen_END (5)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL_B_pll_qpiskewprog_START (6)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL_B_pll_qpiskewprog_END (8)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL_B_pll_qpiskewprogen_START (9)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL_B_pll_qpiskewprogen_END (9)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL_B_pll_fprog_START (10)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL_B_pll_fprog_END (12)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL_B_pll_fprogen_START (13)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL_B_pll_fprogen_END (13)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL_B_pll_reserved_START (16)
#define SOC_HSDT1_CRG_PCIEPLL_CTRL_B_pll_reserved_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pciepll_sc_lock : 1;
        unsigned int pciepll_sc_en_stat : 1;
        unsigned int pciepll_sc_bypass_stat : 1;
        unsigned int pciepll_sc_gt_stat : 1;
        unsigned int pciepll_fn_lock : 1;
        unsigned int pciepll_fn_en_stat : 1;
        unsigned int pciepll_fn_bypass_stat : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 24;
    } reg;
} SOC_HSDT1_CRG_PCIEPLL_STAT_UNION;
#endif
#define SOC_HSDT1_CRG_PCIEPLL_STAT_pciepll_sc_lock_START (0)
#define SOC_HSDT1_CRG_PCIEPLL_STAT_pciepll_sc_lock_END (0)
#define SOC_HSDT1_CRG_PCIEPLL_STAT_pciepll_sc_en_stat_START (1)
#define SOC_HSDT1_CRG_PCIEPLL_STAT_pciepll_sc_en_stat_END (1)
#define SOC_HSDT1_CRG_PCIEPLL_STAT_pciepll_sc_bypass_stat_START (2)
#define SOC_HSDT1_CRG_PCIEPLL_STAT_pciepll_sc_bypass_stat_END (2)
#define SOC_HSDT1_CRG_PCIEPLL_STAT_pciepll_sc_gt_stat_START (3)
#define SOC_HSDT1_CRG_PCIEPLL_STAT_pciepll_sc_gt_stat_END (3)
#define SOC_HSDT1_CRG_PCIEPLL_STAT_pciepll_fn_lock_START (4)
#define SOC_HSDT1_CRG_PCIEPLL_STAT_pciepll_fn_lock_END (4)
#define SOC_HSDT1_CRG_PCIEPLL_STAT_pciepll_fn_en_stat_START (5)
#define SOC_HSDT1_CRG_PCIEPLL_STAT_pciepll_fn_en_stat_END (5)
#define SOC_HSDT1_CRG_PCIEPLL_STAT_pciepll_fn_bypass_stat_START (6)
#define SOC_HSDT1_CRG_PCIEPLL_STAT_pciepll_fn_bypass_stat_END (6)
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
} SOC_HSDT1_CRG_FNPLL_STAT0_UNION;
#endif
#define SOC_HSDT1_CRG_FNPLL_STAT0_div2_pd_test_fnpll_1v2_START (11)
#define SOC_HSDT1_CRG_FNPLL_STAT0_div2_pd_test_fnpll_1v2_END (11)
#define SOC_HSDT1_CRG_FNPLL_STAT0_fbdiv_rst_n_test_fnpll_1v2_START (12)
#define SOC_HSDT1_CRG_FNPLL_STAT0_fbdiv_rst_n_test_fnpll_1v2_END (12)
#define SOC_HSDT1_CRG_FNPLL_STAT0_refdiv_rst_n_test_fnpll_1v2_START (13)
#define SOC_HSDT1_CRG_FNPLL_STAT0_refdiv_rst_n_test_fnpll_1v2_END (13)
#define SOC_HSDT1_CRG_FNPLL_STAT0_ac_buf_pd_START (14)
#define SOC_HSDT1_CRG_FNPLL_STAT0_ac_buf_pd_END (14)
#define SOC_HSDT1_CRG_FNPLL_STAT0_dc_buf_pd_START (15)
#define SOC_HSDT1_CRG_FNPLL_STAT0_dc_buf_pd_END (15)
#define SOC_HSDT1_CRG_FNPLL_STAT0_pll_lock_fnpll_1v2_START (16)
#define SOC_HSDT1_CRG_FNPLL_STAT0_pll_lock_fnpll_1v2_END (16)
#define SOC_HSDT1_CRG_FNPLL_STAT0_pll_lock_ate_fnpll_1v2_START (17)
#define SOC_HSDT1_CRG_FNPLL_STAT0_pll_lock_ate_fnpll_1v2_END (17)
#define SOC_HSDT1_CRG_FNPLL_STAT0_bbpd_calib_done_fnpll_1v2_START (18)
#define SOC_HSDT1_CRG_FNPLL_STAT0_bbpd_calib_done_fnpll_1v2_END (18)
#define SOC_HSDT1_CRG_FNPLL_STAT0_bbpd_calib_fail_fnpll_1v2_START (19)
#define SOC_HSDT1_CRG_FNPLL_STAT0_bbpd_calib_fail_fnpll_1v2_END (19)
#define SOC_HSDT1_CRG_FNPLL_STAT0_bbpd_calib_success_fnpll_1v2_START (20)
#define SOC_HSDT1_CRG_FNPLL_STAT0_bbpd_calib_success_fnpll_1v2_END (20)
#define SOC_HSDT1_CRG_FNPLL_STAT0_pll_unlock_fnpll_1v2_START (21)
#define SOC_HSDT1_CRG_FNPLL_STAT0_pll_unlock_fnpll_1v2_END (21)
#define SOC_HSDT1_CRG_FNPLL_STAT0_clock_lost_fnpll_1v2_START (22)
#define SOC_HSDT1_CRG_FNPLL_STAT0_clock_lost_fnpll_1v2_END (22)
#define SOC_HSDT1_CRG_FNPLL_STAT0_ref_lost_flag_fnpll_1v2_START (23)
#define SOC_HSDT1_CRG_FNPLL_STAT0_ref_lost_flag_fnpll_1v2_END (23)
#define SOC_HSDT1_CRG_FNPLL_STAT0_test_data_fnpll_1v2_START (24)
#define SOC_HSDT1_CRG_FNPLL_STAT0_test_data_fnpll_1v2_END (31)
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
} SOC_HSDT1_CRG_FNPLL_STAT1_UNION;
#endif
#define SOC_HSDT1_CRG_FNPLL_STAT1_unlock_count_fnpll_1v2_START (5)
#define SOC_HSDT1_CRG_FNPLL_STAT1_unlock_count_fnpll_1v2_END (8)
#define SOC_HSDT1_CRG_FNPLL_STAT1_unlock_flag_fnpll_1v2_START (9)
#define SOC_HSDT1_CRG_FNPLL_STAT1_unlock_flag_fnpll_1v2_END (9)
#define SOC_HSDT1_CRG_FNPLL_STAT1_lock_counter_fnpll_1v2_START (12)
#define SOC_HSDT1_CRG_FNPLL_STAT1_lock_counter_fnpll_1v2_END (26)
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
} SOC_HSDT1_CRG_FNPLL_CFG0_UNION;
#endif
#define SOC_HSDT1_CRG_FNPLL_CFG0_phe_sel_en_fnpll_1v2_START (0)
#define SOC_HSDT1_CRG_FNPLL_CFG0_phe_sel_en_fnpll_1v2_END (0)
#define SOC_HSDT1_CRG_FNPLL_CFG0_dtc_test_fnpll_1v2_START (1)
#define SOC_HSDT1_CRG_FNPLL_CFG0_dtc_test_fnpll_1v2_END (1)
#define SOC_HSDT1_CRG_FNPLL_CFG0_sw_dc_buf_en_START (2)
#define SOC_HSDT1_CRG_FNPLL_CFG0_sw_dc_buf_en_END (2)
#define SOC_HSDT1_CRG_FNPLL_CFG0_sw_ac_buf_en_START (3)
#define SOC_HSDT1_CRG_FNPLL_CFG0_sw_ac_buf_en_END (3)
#define SOC_HSDT1_CRG_FNPLL_CFG0_phe_sel_in_fnpll_1v2_START (4)
#define SOC_HSDT1_CRG_FNPLL_CFG0_phe_sel_in_fnpll_1v2_END (5)
#define SOC_HSDT1_CRG_FNPLL_CFG0_unlock_clear_START (6)
#define SOC_HSDT1_CRG_FNPLL_CFG0_unlock_clear_END (6)
#define SOC_HSDT1_CRG_FNPLL_CFG0_dc_ac_clk_en_START (7)
#define SOC_HSDT1_CRG_FNPLL_CFG0_dc_ac_clk_en_END (7)
#define SOC_HSDT1_CRG_FNPLL_CFG0_dtc_m_cfg_fnpll_1v2_START (8)
#define SOC_HSDT1_CRG_FNPLL_CFG0_dtc_m_cfg_fnpll_1v2_END (13)
#define SOC_HSDT1_CRG_FNPLL_CFG0_ref_lost_clear_fnpll_1v2_START (14)
#define SOC_HSDT1_CRG_FNPLL_CFG0_ref_lost_clear_fnpll_1v2_END (14)
#define SOC_HSDT1_CRG_FNPLL_CFG0_spread_en_fnpll_1v2_START (16)
#define SOC_HSDT1_CRG_FNPLL_CFG0_spread_en_fnpll_1v2_END (16)
#define SOC_HSDT1_CRG_FNPLL_CFG0_downspread_fnpll_1v2_START (17)
#define SOC_HSDT1_CRG_FNPLL_CFG0_downspread_fnpll_1v2_END (17)
#define SOC_HSDT1_CRG_FNPLL_CFG0_sel_extwave_fnpll_1v2_START (18)
#define SOC_HSDT1_CRG_FNPLL_CFG0_sel_extwave_fnpll_1v2_END (18)
#define SOC_HSDT1_CRG_FNPLL_CFG0_divval_fnpll_1v2_START (19)
#define SOC_HSDT1_CRG_FNPLL_CFG0_divval_fnpll_1v2_END (22)
#define SOC_HSDT1_CRG_FNPLL_CFG0_spread_fnpll_1v2_START (23)
#define SOC_HSDT1_CRG_FNPLL_CFG0_spread_fnpll_1v2_END (25)
#define SOC_HSDT1_CRG_FNPLL_CFG0_freq_threshold_START (26)
#define SOC_HSDT1_CRG_FNPLL_CFG0_freq_threshold_END (31)
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
} SOC_HSDT1_CRG_FNPLL_CFG1_UNION;
#endif
#define SOC_HSDT1_CRG_FNPLL_CFG1_ext_maxaddr_fnpll_1v2_START (0)
#define SOC_HSDT1_CRG_FNPLL_CFG1_ext_maxaddr_fnpll_1v2_END (7)
#define SOC_HSDT1_CRG_FNPLL_CFG1_extwaveval_fnpll_1v2_START (8)
#define SOC_HSDT1_CRG_FNPLL_CFG1_extwaveval_fnpll_1v2_END (15)
#define SOC_HSDT1_CRG_FNPLL_CFG1_input_cfg_en_fnpll_1v2_START (16)
#define SOC_HSDT1_CRG_FNPLL_CFG1_input_cfg_en_fnpll_1v2_END (16)
#define SOC_HSDT1_CRG_FNPLL_CFG1_test_mode_fnpll_1v2_START (17)
#define SOC_HSDT1_CRG_FNPLL_CFG1_test_mode_fnpll_1v2_END (17)
#define SOC_HSDT1_CRG_FNPLL_CFG1_lock_thr_fnpll_1v2_START (18)
#define SOC_HSDT1_CRG_FNPLL_CFG1_lock_thr_fnpll_1v2_END (19)
#define SOC_HSDT1_CRG_FNPLL_CFG1_refdiv_rst_n_fnpll_1v2_START (20)
#define SOC_HSDT1_CRG_FNPLL_CFG1_refdiv_rst_n_fnpll_1v2_END (20)
#define SOC_HSDT1_CRG_FNPLL_CFG1_fbdiv_rst_n_fnpll_1v2_START (21)
#define SOC_HSDT1_CRG_FNPLL_CFG1_fbdiv_rst_n_fnpll_1v2_END (21)
#define SOC_HSDT1_CRG_FNPLL_CFG1_div2_pd_fnpll_1v2_START (22)
#define SOC_HSDT1_CRG_FNPLL_CFG1_div2_pd_fnpll_1v2_END (22)
#define SOC_HSDT1_CRG_FNPLL_CFG1_lock_en_fnpll_1v2_START (23)
#define SOC_HSDT1_CRG_FNPLL_CFG1_lock_en_fnpll_1v2_END (23)
#define SOC_HSDT1_CRG_FNPLL_CFG1_fbdiv_delay_num_fnpll_1v2_START (24)
#define SOC_HSDT1_CRG_FNPLL_CFG1_fbdiv_delay_num_fnpll_1v2_END (30)
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
} SOC_HSDT1_CRG_FNPLL_CFG2_UNION;
#endif
#define SOC_HSDT1_CRG_FNPLL_CFG2_bbpd_calib_byp_fnpll_1v2_START (0)
#define SOC_HSDT1_CRG_FNPLL_CFG2_bbpd_calib_byp_fnpll_1v2_END (0)
#define SOC_HSDT1_CRG_FNPLL_CFG2_dtc_ctrl_inv_fnpll_1v2_START (1)
#define SOC_HSDT1_CRG_FNPLL_CFG2_dtc_ctrl_inv_fnpll_1v2_END (1)
#define SOC_HSDT1_CRG_FNPLL_CFG2_dll_force_en_fnpll_1v2_START (2)
#define SOC_HSDT1_CRG_FNPLL_CFG2_dll_force_en_fnpll_1v2_END (2)
#define SOC_HSDT1_CRG_FNPLL_CFG2_DLL_EN_FNPLL_1V2_START (3)
#define SOC_HSDT1_CRG_FNPLL_CFG2_DLL_EN_FNPLL_1V2_END (3)
#define SOC_HSDT1_CRG_FNPLL_CFG2_phe_code_a_fnpll_1v2_START (4)
#define SOC_HSDT1_CRG_FNPLL_CFG2_phe_code_a_fnpll_1v2_END (5)
#define SOC_HSDT1_CRG_FNPLL_CFG2_phe_code_b_fnpll_1v2_START (6)
#define SOC_HSDT1_CRG_FNPLL_CFG2_phe_code_b_fnpll_1v2_END (7)
#define SOC_HSDT1_CRG_FNPLL_CFG2_ctinue_lock_num_fnpll_1v2_START (8)
#define SOC_HSDT1_CRG_FNPLL_CFG2_ctinue_lock_num_fnpll_1v2_END (13)
#define SOC_HSDT1_CRG_FNPLL_CFG2_read_en_fnpll_1v2_START (16)
#define SOC_HSDT1_CRG_FNPLL_CFG2_read_en_fnpll_1v2_END (16)
#define SOC_HSDT1_CRG_FNPLL_CFG2_k_gain_cfg_en_fnpll_1v2_START (17)
#define SOC_HSDT1_CRG_FNPLL_CFG2_k_gain_cfg_en_fnpll_1v2_END (17)
#define SOC_HSDT1_CRG_FNPLL_CFG2_k_gain_av_thr_fnpll_1v2_START (20)
#define SOC_HSDT1_CRG_FNPLL_CFG2_k_gain_av_thr_fnpll_1v2_END (22)
#define SOC_HSDT1_CRG_FNPLL_CFG2_pll_unlock_clr_fnpll_1v2_START (23)
#define SOC_HSDT1_CRG_FNPLL_CFG2_pll_unlock_clr_fnpll_1v2_END (23)
#define SOC_HSDT1_CRG_FNPLL_CFG2_k_gain_cfg_fnpll_1v2_START (24)
#define SOC_HSDT1_CRG_FNPLL_CFG2_k_gain_cfg_fnpll_1v2_END (29)
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
} SOC_HSDT1_CRG_FNPLL_CFG3_UNION;
#endif
#define SOC_HSDT1_CRG_FNPLL_CFG3_dtc_o_cfg_fnpll_1v2_START (0)
#define SOC_HSDT1_CRG_FNPLL_CFG3_dtc_o_cfg_fnpll_1v2_END (5)
#define SOC_HSDT1_CRG_FNPLL_CFG3_bbpd_lock_num_fnpll_1v2_START (8)
#define SOC_HSDT1_CRG_FNPLL_CFG3_bbpd_lock_num_fnpll_1v2_END (15)
#define SOC_HSDT1_CRG_FNPLL_CFG3_lpf_bw_fnpll_1v2_START (16)
#define SOC_HSDT1_CRG_FNPLL_CFG3_lpf_bw_fnpll_1v2_END (18)
#define SOC_HSDT1_CRG_FNPLL_CFG3_lpf_bw1_fnpll_1v2_START (20)
#define SOC_HSDT1_CRG_FNPLL_CFG3_lpf_bw1_fnpll_1v2_END (22)
#define SOC_HSDT1_CRG_FNPLL_CFG3_calib_lpf_bw_fnpll_1v2_START (24)
#define SOC_HSDT1_CRG_FNPLL_CFG3_calib_lpf_bw_fnpll_1v2_END (26)
#define SOC_HSDT1_CRG_FNPLL_CFG3_calib_lpf_bw1_fnpll_1v2_START (28)
#define SOC_HSDT1_CRG_FNPLL_CFG3_calib_lpf_bw1_fnpll_1v2_END (30)
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
} SOC_HSDT1_CRG_FNPLL_CFG4_UNION;
#endif
#define SOC_HSDT1_CRG_FNPLL_CFG4_max_k_gain_fnpll_1v2_START (0)
#define SOC_HSDT1_CRG_FNPLL_CFG4_max_k_gain_fnpll_1v2_END (5)
#define SOC_HSDT1_CRG_FNPLL_CFG4_min_k_gain_fnpll_1v2_START (8)
#define SOC_HSDT1_CRG_FNPLL_CFG4_min_k_gain_fnpll_1v2_END (13)
#define SOC_HSDT1_CRG_FNPLL_CFG4_en_dac_test_fnpll_1v2_START (16)
#define SOC_HSDT1_CRG_FNPLL_CFG4_en_dac_test_fnpll_1v2_END (16)
#define SOC_HSDT1_CRG_FNPLL_CFG4_updn_sel_fnpll_1v2_START (17)
#define SOC_HSDT1_CRG_FNPLL_CFG4_updn_sel_fnpll_1v2_END (17)
#define SOC_HSDT1_CRG_FNPLL_CFG4_icp_ctrl_fnpll_1v2_START (18)
#define SOC_HSDT1_CRG_FNPLL_CFG4_icp_ctrl_fnpll_1v2_END (18)
#define SOC_HSDT1_CRG_FNPLL_CFG4_vdc_sel_fnpll_1v2_START (20)
#define SOC_HSDT1_CRG_FNPLL_CFG4_vdc_sel_fnpll_1v2_END (23)
#define SOC_HSDT1_CRG_FNPLL_CFG4_test_data_sel_fnpll_1v2_START (28)
#define SOC_HSDT1_CRG_FNPLL_CFG4_test_data_sel_fnpll_1v2_END (31)
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
} SOC_HSDT1_CRG_FNPLL_CFG5_UNION;
#endif
#define SOC_HSDT1_CRG_FNPLL_CFG5_ref_lost_thr_fnpll_1v2_START (0)
#define SOC_HSDT1_CRG_FNPLL_CFG5_ref_lost_thr_fnpll_1v2_END (7)
#define SOC_HSDT1_CRG_FNPLL_CFG5_pfd_div_ratio_START (8)
#define SOC_HSDT1_CRG_FNPLL_CFG5_pfd_div_ratio_END (11)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int fnpll_en : 1;
        unsigned int BYPASS_FNPLL_1V2 : 1;
        unsigned int PLL_MODE_FNPLL_1V2 : 1;
        unsigned int reserved_0 : 1;
        unsigned int REFDIV_FNPLL_1V2 : 6;
        unsigned int reserved_1 : 2;
        unsigned int POSTDIV1_FNPLL_1V2 : 3;
        unsigned int reserved_2 : 1;
        unsigned int FBDIV_FNPLL_1V2 : 12;
        unsigned int POSTDIV2_FNPLL_1V2 : 3;
        unsigned int fbdiv_cfg_vld : 1;
    } reg;
} SOC_HSDT1_CRG_FNPLL_CFG6_UNION;
#endif
#define SOC_HSDT1_CRG_FNPLL_CFG6_fnpll_en_START (0)
#define SOC_HSDT1_CRG_FNPLL_CFG6_fnpll_en_END (0)
#define SOC_HSDT1_CRG_FNPLL_CFG6_BYPASS_FNPLL_1V2_START (1)
#define SOC_HSDT1_CRG_FNPLL_CFG6_BYPASS_FNPLL_1V2_END (1)
#define SOC_HSDT1_CRG_FNPLL_CFG6_PLL_MODE_FNPLL_1V2_START (2)
#define SOC_HSDT1_CRG_FNPLL_CFG6_PLL_MODE_FNPLL_1V2_END (2)
#define SOC_HSDT1_CRG_FNPLL_CFG6_REFDIV_FNPLL_1V2_START (4)
#define SOC_HSDT1_CRG_FNPLL_CFG6_REFDIV_FNPLL_1V2_END (9)
#define SOC_HSDT1_CRG_FNPLL_CFG6_POSTDIV1_FNPLL_1V2_START (12)
#define SOC_HSDT1_CRG_FNPLL_CFG6_POSTDIV1_FNPLL_1V2_END (14)
#define SOC_HSDT1_CRG_FNPLL_CFG6_FBDIV_FNPLL_1V2_START (16)
#define SOC_HSDT1_CRG_FNPLL_CFG6_FBDIV_FNPLL_1V2_END (27)
#define SOC_HSDT1_CRG_FNPLL_CFG6_POSTDIV2_FNPLL_1V2_START (28)
#define SOC_HSDT1_CRG_FNPLL_CFG6_POSTDIV2_FNPLL_1V2_END (30)
#define SOC_HSDT1_CRG_FNPLL_CFG6_fbdiv_cfg_vld_START (31)
#define SOC_HSDT1_CRG_FNPLL_CFG6_fbdiv_cfg_vld_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int FRAC_FNPLL_1V2 : 24;
        unsigned int FOUTPOSTDIVPD_FNPLL_1V2 : 1;
        unsigned int FOUT4PHASEPD_FNPLL_1V2 : 1;
        unsigned int FOUT2XPD_FNPLL_1V2 : 1;
        unsigned int reserved : 4;
        unsigned int frac_cfg_vld : 1;
    } reg;
} SOC_HSDT1_CRG_FNPLL_CFG7_UNION;
#endif
#define SOC_HSDT1_CRG_FNPLL_CFG7_FRAC_FNPLL_1V2_START (0)
#define SOC_HSDT1_CRG_FNPLL_CFG7_FRAC_FNPLL_1V2_END (23)
#define SOC_HSDT1_CRG_FNPLL_CFG7_FOUTPOSTDIVPD_FNPLL_1V2_START (24)
#define SOC_HSDT1_CRG_FNPLL_CFG7_FOUTPOSTDIVPD_FNPLL_1V2_END (24)
#define SOC_HSDT1_CRG_FNPLL_CFG7_FOUT4PHASEPD_FNPLL_1V2_START (25)
#define SOC_HSDT1_CRG_FNPLL_CFG7_FOUT4PHASEPD_FNPLL_1V2_END (25)
#define SOC_HSDT1_CRG_FNPLL_CFG7_FOUT2XPD_FNPLL_1V2_START (26)
#define SOC_HSDT1_CRG_FNPLL_CFG7_FOUT2XPD_FNPLL_1V2_END (26)
#define SOC_HSDT1_CRG_FNPLL_CFG7_frac_cfg_vld_START (31)
#define SOC_HSDT1_CRG_FNPLL_CFG7_frac_cfg_vld_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int usbpcie_refclk_iso_en : 1;
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
        unsigned int reserved_23 : 1;
        unsigned int reserved_24 : 1;
        unsigned int reserved_25 : 1;
        unsigned int reserved_26 : 1;
        unsigned int reserved_27 : 1;
        unsigned int reserved_28 : 1;
        unsigned int reserved_29 : 1;
        unsigned int reserved_30 : 1;
    } reg;
} SOC_HSDT1_CRG_ISOEN_UNION;
#endif
#define SOC_HSDT1_CRG_ISOEN_usbpcie_refclk_iso_en_START (0)
#define SOC_HSDT1_CRG_ISOEN_usbpcie_refclk_iso_en_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int usbpcie_refclk_iso_en : 1;
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
        unsigned int reserved_23 : 1;
        unsigned int reserved_24 : 1;
        unsigned int reserved_25 : 1;
        unsigned int reserved_26 : 1;
        unsigned int reserved_27 : 1;
        unsigned int reserved_28 : 1;
        unsigned int reserved_29 : 1;
        unsigned int reserved_30 : 1;
    } reg;
} SOC_HSDT1_CRG_ISODIS_UNION;
#endif
#define SOC_HSDT1_CRG_ISODIS_usbpcie_refclk_iso_en_START (0)
#define SOC_HSDT1_CRG_ISODIS_usbpcie_refclk_iso_en_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int usbpcie_refclk_iso_en : 1;
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
        unsigned int reserved_23 : 1;
        unsigned int reserved_24 : 1;
        unsigned int reserved_25 : 1;
        unsigned int reserved_26 : 1;
        unsigned int reserved_27 : 1;
        unsigned int reserved_28 : 1;
        unsigned int reserved_29 : 1;
        unsigned int reserved_30 : 1;
    } reg;
} SOC_HSDT1_CRG_ISOSTAT_UNION;
#endif
#define SOC_HSDT1_CRG_ISOSTAT_usbpcie_refclk_iso_en_START (0)
#define SOC_HSDT1_CRG_ISOSTAT_usbpcie_refclk_iso_en_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_pcie1io_hard_bypass : 1;
        unsigned int gt_clk_pcie1phy_ref_hard_bypass : 1;
        unsigned int pcie1phy_debounce_waitcfg_out_h : 2;
        unsigned int pcie1phy_debounce_waitcfg_in_h : 2;
        unsigned int pcie1io_oe_en_soft : 1;
        unsigned int pcie1_oe_gt_mode : 2;
        unsigned int pcie1io_oe_polar : 1;
        unsigned int pcie1io_hard_ctrl_debounce_bypass : 1;
        unsigned int pcie1io_oe_en_hard_bypass : 1;
        unsigned int pcie1phy_debounce_waitcfg_out : 8;
        unsigned int pcie1phy_debounce_waitcfg_in : 7;
        unsigned int pcie1io_ie_en_hard_bypass : 1;
        unsigned int pcie1io_ie_en_soft : 1;
        unsigned int pcie1io_ie_polar : 1;
        unsigned int pcie1_mplla_force_en_sel : 1;
        unsigned int reserved : 1;
    } reg;
} SOC_HSDT1_CRG_PCIECTRL1_UNION;
#endif
#define SOC_HSDT1_CRG_PCIECTRL1_gt_clk_pcie1io_hard_bypass_START (0)
#define SOC_HSDT1_CRG_PCIECTRL1_gt_clk_pcie1io_hard_bypass_END (0)
#define SOC_HSDT1_CRG_PCIECTRL1_gt_clk_pcie1phy_ref_hard_bypass_START (1)
#define SOC_HSDT1_CRG_PCIECTRL1_gt_clk_pcie1phy_ref_hard_bypass_END (1)
#define SOC_HSDT1_CRG_PCIECTRL1_pcie1phy_debounce_waitcfg_out_h_START (2)
#define SOC_HSDT1_CRG_PCIECTRL1_pcie1phy_debounce_waitcfg_out_h_END (3)
#define SOC_HSDT1_CRG_PCIECTRL1_pcie1phy_debounce_waitcfg_in_h_START (4)
#define SOC_HSDT1_CRG_PCIECTRL1_pcie1phy_debounce_waitcfg_in_h_END (5)
#define SOC_HSDT1_CRG_PCIECTRL1_pcie1io_oe_en_soft_START (6)
#define SOC_HSDT1_CRG_PCIECTRL1_pcie1io_oe_en_soft_END (6)
#define SOC_HSDT1_CRG_PCIECTRL1_pcie1_oe_gt_mode_START (7)
#define SOC_HSDT1_CRG_PCIECTRL1_pcie1_oe_gt_mode_END (8)
#define SOC_HSDT1_CRG_PCIECTRL1_pcie1io_oe_polar_START (9)
#define SOC_HSDT1_CRG_PCIECTRL1_pcie1io_oe_polar_END (9)
#define SOC_HSDT1_CRG_PCIECTRL1_pcie1io_hard_ctrl_debounce_bypass_START (10)
#define SOC_HSDT1_CRG_PCIECTRL1_pcie1io_hard_ctrl_debounce_bypass_END (10)
#define SOC_HSDT1_CRG_PCIECTRL1_pcie1io_oe_en_hard_bypass_START (11)
#define SOC_HSDT1_CRG_PCIECTRL1_pcie1io_oe_en_hard_bypass_END (11)
#define SOC_HSDT1_CRG_PCIECTRL1_pcie1phy_debounce_waitcfg_out_START (12)
#define SOC_HSDT1_CRG_PCIECTRL1_pcie1phy_debounce_waitcfg_out_END (19)
#define SOC_HSDT1_CRG_PCIECTRL1_pcie1phy_debounce_waitcfg_in_START (20)
#define SOC_HSDT1_CRG_PCIECTRL1_pcie1phy_debounce_waitcfg_in_END (26)
#define SOC_HSDT1_CRG_PCIECTRL1_pcie1io_ie_en_hard_bypass_START (27)
#define SOC_HSDT1_CRG_PCIECTRL1_pcie1io_ie_en_hard_bypass_END (27)
#define SOC_HSDT1_CRG_PCIECTRL1_pcie1io_ie_en_soft_START (28)
#define SOC_HSDT1_CRG_PCIECTRL1_pcie1io_ie_en_soft_END (28)
#define SOC_HSDT1_CRG_PCIECTRL1_pcie1io_ie_polar_START (29)
#define SOC_HSDT1_CRG_PCIECTRL1_pcie1io_ie_polar_END (29)
#define SOC_HSDT1_CRG_PCIECTRL1_pcie1_mplla_force_en_sel_START (30)
#define SOC_HSDT1_CRG_PCIECTRL1_pcie1_mplla_force_en_sel_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved : 11;
        unsigned int div2_pd_test_fnpll_1v2_bk : 1;
        unsigned int fbdiv_rst_n_test_fnpll_1v2_bk : 1;
        unsigned int refdiv_rst_n_test_fnpll_1v2_bk : 1;
        unsigned int ac_buf_pd_fnpll_1v2_bk : 1;
        unsigned int dc_buf_pd_fnpll_1v2_bk : 1;
        unsigned int pll_lock_fnpll_1v2_bk : 1;
        unsigned int pll_lock_ate_fnpll_1v2_bk : 1;
        unsigned int bbpd_calib_done_fnpll_1v2_bk : 1;
        unsigned int bbpd_calib_fail_fnpll_1v2_bk : 1;
        unsigned int bbpd_calib_success_fnpll_1v2_bk : 1;
        unsigned int pll_unlock_fnpll_1v2_bk : 1;
        unsigned int clock_lost_fnpll_1v2_bk : 1;
        unsigned int ref_lost_flag_fnpll_1v2_bk : 1;
        unsigned int test_data_fnpll_1v2_bk : 8;
    } reg;
} SOC_HSDT1_CRG_FNPLL_BK_STAT0_UNION;
#endif
#define SOC_HSDT1_CRG_FNPLL_BK_STAT0_div2_pd_test_fnpll_1v2_bk_START (11)
#define SOC_HSDT1_CRG_FNPLL_BK_STAT0_div2_pd_test_fnpll_1v2_bk_END (11)
#define SOC_HSDT1_CRG_FNPLL_BK_STAT0_fbdiv_rst_n_test_fnpll_1v2_bk_START (12)
#define SOC_HSDT1_CRG_FNPLL_BK_STAT0_fbdiv_rst_n_test_fnpll_1v2_bk_END (12)
#define SOC_HSDT1_CRG_FNPLL_BK_STAT0_refdiv_rst_n_test_fnpll_1v2_bk_START (13)
#define SOC_HSDT1_CRG_FNPLL_BK_STAT0_refdiv_rst_n_test_fnpll_1v2_bk_END (13)
#define SOC_HSDT1_CRG_FNPLL_BK_STAT0_ac_buf_pd_fnpll_1v2_bk_START (14)
#define SOC_HSDT1_CRG_FNPLL_BK_STAT0_ac_buf_pd_fnpll_1v2_bk_END (14)
#define SOC_HSDT1_CRG_FNPLL_BK_STAT0_dc_buf_pd_fnpll_1v2_bk_START (15)
#define SOC_HSDT1_CRG_FNPLL_BK_STAT0_dc_buf_pd_fnpll_1v2_bk_END (15)
#define SOC_HSDT1_CRG_FNPLL_BK_STAT0_pll_lock_fnpll_1v2_bk_START (16)
#define SOC_HSDT1_CRG_FNPLL_BK_STAT0_pll_lock_fnpll_1v2_bk_END (16)
#define SOC_HSDT1_CRG_FNPLL_BK_STAT0_pll_lock_ate_fnpll_1v2_bk_START (17)
#define SOC_HSDT1_CRG_FNPLL_BK_STAT0_pll_lock_ate_fnpll_1v2_bk_END (17)
#define SOC_HSDT1_CRG_FNPLL_BK_STAT0_bbpd_calib_done_fnpll_1v2_bk_START (18)
#define SOC_HSDT1_CRG_FNPLL_BK_STAT0_bbpd_calib_done_fnpll_1v2_bk_END (18)
#define SOC_HSDT1_CRG_FNPLL_BK_STAT0_bbpd_calib_fail_fnpll_1v2_bk_START (19)
#define SOC_HSDT1_CRG_FNPLL_BK_STAT0_bbpd_calib_fail_fnpll_1v2_bk_END (19)
#define SOC_HSDT1_CRG_FNPLL_BK_STAT0_bbpd_calib_success_fnpll_1v2_bk_START (20)
#define SOC_HSDT1_CRG_FNPLL_BK_STAT0_bbpd_calib_success_fnpll_1v2_bk_END (20)
#define SOC_HSDT1_CRG_FNPLL_BK_STAT0_pll_unlock_fnpll_1v2_bk_START (21)
#define SOC_HSDT1_CRG_FNPLL_BK_STAT0_pll_unlock_fnpll_1v2_bk_END (21)
#define SOC_HSDT1_CRG_FNPLL_BK_STAT0_clock_lost_fnpll_1v2_bk_START (22)
#define SOC_HSDT1_CRG_FNPLL_BK_STAT0_clock_lost_fnpll_1v2_bk_END (22)
#define SOC_HSDT1_CRG_FNPLL_BK_STAT0_ref_lost_flag_fnpll_1v2_bk_START (23)
#define SOC_HSDT1_CRG_FNPLL_BK_STAT0_ref_lost_flag_fnpll_1v2_bk_END (23)
#define SOC_HSDT1_CRG_FNPLL_BK_STAT0_test_data_fnpll_1v2_bk_START (24)
#define SOC_HSDT1_CRG_FNPLL_BK_STAT0_test_data_fnpll_1v2_bk_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 5;
        unsigned int unlock_count_fnpll_1v2_bk : 4;
        unsigned int unlock_flag_fnpll_1v2_bk : 1;
        unsigned int reserved_1 : 2;
        unsigned int lock_counter_fnpll_1v2_bk : 15;
        unsigned int reserved_2 : 5;
    } reg;
} SOC_HSDT1_CRG_FNPLL_BK_STAT1_UNION;
#endif
#define SOC_HSDT1_CRG_FNPLL_BK_STAT1_unlock_count_fnpll_1v2_bk_START (5)
#define SOC_HSDT1_CRG_FNPLL_BK_STAT1_unlock_count_fnpll_1v2_bk_END (8)
#define SOC_HSDT1_CRG_FNPLL_BK_STAT1_unlock_flag_fnpll_1v2_bk_START (9)
#define SOC_HSDT1_CRG_FNPLL_BK_STAT1_unlock_flag_fnpll_1v2_bk_END (9)
#define SOC_HSDT1_CRG_FNPLL_BK_STAT1_lock_counter_fnpll_1v2_bk_START (12)
#define SOC_HSDT1_CRG_FNPLL_BK_STAT1_lock_counter_fnpll_1v2_bk_END (26)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int intr_mask_usb_nonidle_pend : 1;
        unsigned int intr_mask_pcie1_nonidle_pend : 1;
        unsigned int reserved : 14;
        unsigned int bitmasken : 16;
    } reg;
} SOC_HSDT1_CRG_INTR_MASK_NOCBUS_NONIDLE_PEND_UNION;
#endif
#define SOC_HSDT1_CRG_INTR_MASK_NOCBUS_NONIDLE_PEND_intr_mask_usb_nonidle_pend_START (0)
#define SOC_HSDT1_CRG_INTR_MASK_NOCBUS_NONIDLE_PEND_intr_mask_usb_nonidle_pend_END (0)
#define SOC_HSDT1_CRG_INTR_MASK_NOCBUS_NONIDLE_PEND_intr_mask_pcie1_nonidle_pend_START (1)
#define SOC_HSDT1_CRG_INTR_MASK_NOCBUS_NONIDLE_PEND_intr_mask_pcie1_nonidle_pend_END (1)
#define SOC_HSDT1_CRG_INTR_MASK_NOCBUS_NONIDLE_PEND_bitmasken_START (16)
#define SOC_HSDT1_CRG_INTR_MASK_NOCBUS_NONIDLE_PEND_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int intr_clr_usb_nonidle_pend : 1;
        unsigned int intr_clr_pcie1_nonidle_pend : 1;
        unsigned int reserved : 14;
        unsigned int bitmasken : 16;
    } reg;
} SOC_HSDT1_CRG_INTR_CLR_NOCBUS_NONIDLE_PEND_UNION;
#endif
#define SOC_HSDT1_CRG_INTR_CLR_NOCBUS_NONIDLE_PEND_intr_clr_usb_nonidle_pend_START (0)
#define SOC_HSDT1_CRG_INTR_CLR_NOCBUS_NONIDLE_PEND_intr_clr_usb_nonidle_pend_END (0)
#define SOC_HSDT1_CRG_INTR_CLR_NOCBUS_NONIDLE_PEND_intr_clr_pcie1_nonidle_pend_START (1)
#define SOC_HSDT1_CRG_INTR_CLR_NOCBUS_NONIDLE_PEND_intr_clr_pcie1_nonidle_pend_END (1)
#define SOC_HSDT1_CRG_INTR_CLR_NOCBUS_NONIDLE_PEND_bitmasken_START (16)
#define SOC_HSDT1_CRG_INTR_CLR_NOCBUS_NONIDLE_PEND_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int intr_stat_usb_nonidle_pend : 1;
        unsigned int intr_stat_pcie1_nonidle_pend : 1;
        unsigned int reserved : 30;
    } reg;
} SOC_HSDT1_CRG_INTR_STAT_NOCBUS_NONIDLE_PEND_UNION;
#endif
#define SOC_HSDT1_CRG_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_usb_nonidle_pend_START (0)
#define SOC_HSDT1_CRG_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_usb_nonidle_pend_END (0)
#define SOC_HSDT1_CRG_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_pcie1_nonidle_pend_START (1)
#define SOC_HSDT1_CRG_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_pcie1_nonidle_pend_END (1)
#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
#endif
