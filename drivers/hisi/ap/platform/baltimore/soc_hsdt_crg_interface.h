#ifndef __SOC_HSDT_CRG_INTERFACE_H__
#define __SOC_HSDT_CRG_INTERFACE_H__ 
#ifdef __cplusplus
    #if __cplusplus
        extern "C" {
    #endif
#endif
#ifndef __SOC_H_FOR_ASM__
#define SOC_HSDT_CRG_PEREN0_ADDR(base) ((base) + (0x000UL))
#define SOC_HSDT_CRG_PERDIS0_ADDR(base) ((base) + (0x004UL))
#define SOC_HSDT_CRG_PERCLKEN0_ADDR(base) ((base) + (0x008UL))
#define SOC_HSDT_CRG_PERSTAT0_ADDR(base) ((base) + (0x00CUL))
#define SOC_HSDT_CRG_PEREN1_ADDR(base) ((base) + (0x010UL))
#define SOC_HSDT_CRG_PERDIS1_ADDR(base) ((base) + (0x014UL))
#define SOC_HSDT_CRG_PERCLKEN1_ADDR(base) ((base) + (0x018UL))
#define SOC_HSDT_CRG_PERSTAT1_ADDR(base) ((base) + (0x01CUL))
#define SOC_HSDT_CRG_PERRSTEN0_ADDR(base) ((base) + (0x060UL))
#define SOC_HSDT_CRG_PERRSTDIS0_ADDR(base) ((base) + (0x064UL))
#define SOC_HSDT_CRG_PERRSTSTAT0_ADDR(base) ((base) + (0x068UL))
#define SOC_HSDT_CRG_CLKDIV0_ADDR(base) ((base) + (0x0A8UL))
#define SOC_HSDT_CRG_CLKDIV1_ADDR(base) ((base) + (0x0ACUL))
#define SOC_HSDT_CRG_CLKDIV2_ADDR(base) ((base) + (0x0B0UL))
#define SOC_HSDT_CRG_PERI_STAT0_ADDR(base) ((base) + (0x100UL))
#define SOC_HSDT_CRG_PERI_STAT1_ADDR(base) ((base) + (0x104UL))
#define SOC_HSDT_CRG_PERI_STAT2_ADDR(base) ((base) + (0x108UL))
#define SOC_HSDT_CRG_PCIEPLL_CTRL0_ADDR(base) ((base) + (0x188UL))
#define SOC_HSDT_CRG_PCIEPLL_CTRL1_ADDR(base) ((base) + (0x18cUL))
#define SOC_HSDT_CRG_PCIEPLL_CTRL_A_ADDR(base) ((base) + (0x190UL))
#define SOC_HSDT_CRG_PCIEPLL_CTRL_B_ADDR(base) ((base) + (0x194UL))
#define SOC_HSDT_CRG_PCIEPLL_STAT_ADDR(base) ((base) + (0x204UL))
#define SOC_HSDT_CRG_FNPLL_STAT0_ADDR(base) ((base) + (0x208UL))
#define SOC_HSDT_CRG_FNPLL_STAT1_ADDR(base) ((base) + (0x20CUL))
#define SOC_HSDT_CRG_JITTER_MONITOR_STAT0_ADDR(base) ((base) + (0x210UL))
#define SOC_HSDT_CRG_JITTER_MONITOR_STAT1_ADDR(base) ((base) + (0x214UL))
#define SOC_HSDT_CRG_JITTER_MONITOR_CFG0_ADDR(base) ((base) + (0x21CUL))
#define SOC_HSDT_CRG_JITTER_MONITOR_CFG1_ADDR(base) ((base) + (0x220UL))
#define SOC_HSDT_CRG_JITTER_MONITOR_CFG2_ADDR(base) ((base) + (0x224UL))
#define SOC_HSDT_CRG_FNPLL_CFG0_ADDR(base) ((base) + (0x228UL))
#define SOC_HSDT_CRG_FNPLL_CFG1_ADDR(base) ((base) + (0x22CUL))
#define SOC_HSDT_CRG_FNPLL_CFG2_ADDR(base) ((base) + (0x230UL))
#define SOC_HSDT_CRG_FNPLL_CFG3_ADDR(base) ((base) + (0x234UL))
#define SOC_HSDT_CRG_FNPLL_CFG4_ADDR(base) ((base) + (0x238UL))
#define SOC_HSDT_CRG_FNPLL_CFG5_ADDR(base) ((base) + (0x23CUL))
#define SOC_HSDT_CRG_FNPLL_CFG6_ADDR(base) ((base) + (0x240UL))
#define SOC_HSDT_CRG_FNPLL_CFG7_ADDR(base) ((base) + (0x244UL))
#define SOC_HSDT_CRG_PCIECTRL0_ADDR(base) ((base) + (0x300UL))
#define SOC_HSDT_CRG_IPCLKRST_BYPASS0_ADDR(base) ((base) + (0x318UL))
#define SOC_HSDT_CRG_PERI_AUTODIV_INUSE_STAT_ADDR(base) ((base) + (0x34CUL))
#define SOC_HSDT_CRG_PERI_AUTODIV0_ADDR(base) ((base) + (0x350UL))
#define SOC_HSDT_CRG_PERI_AUTODIV1_ADDR(base) ((base) + (0x354UL))
#define SOC_HSDT_CRG_PEREN_CC712_SEC_ADDR(base) ((base) + (0xD10UL))
#define SOC_HSDT_CRG_PERDIS_CC712_SEC_ADDR(base) ((base) + (0xD14UL))
#define SOC_HSDT_CRG_PERCLKEN_CC712_SEC_ADDR(base) ((base) + (0xD18UL))
#define SOC_HSDT_CRG_PERSTAT_CC712_SEC_ADDR(base) ((base) + (0xD1CUL))
#define SOC_HSDT_CRG_INTR_MASK_NOCBUS_NONIDLE_PEND_ADDR(base) ((base) + (0x500UL))
#define SOC_HSDT_CRG_INTR_CLR_NOCBUS_NONIDLE_PEND_ADDR(base) ((base) + (0x504UL))
#define SOC_HSDT_CRG_INTR_STAT_NOCBUS_NONIDLE_PEND_ADDR(base) ((base) + (0x508UL))
#define SOC_HSDT_CRG_PERRSTEN_CC712_SEC_ADDR(base) ((base) + (0xD30UL))
#define SOC_HSDT_CRG_PERRSTDIS_CC712_SEC_ADDR(base) ((base) + (0xD34UL))
#define SOC_HSDT_CRG_PERRSTSTAT_CC712_SEC_ADDR(base) ((base) + (0xD38UL))
#define SOC_HSDT_CRG_CLKDIV_CC712_SEC_ADDR(base) ((base) + (0xD3CUL))
#define SOC_HSDT_CRG_PERI_CTRL_CC712_SEC_ADDR(base) ((base) + (0xD40UL))
#else
#define SOC_HSDT_CRG_PEREN0_ADDR(base) ((base) + (0x000))
#define SOC_HSDT_CRG_PERDIS0_ADDR(base) ((base) + (0x004))
#define SOC_HSDT_CRG_PERCLKEN0_ADDR(base) ((base) + (0x008))
#define SOC_HSDT_CRG_PERSTAT0_ADDR(base) ((base) + (0x00C))
#define SOC_HSDT_CRG_PEREN1_ADDR(base) ((base) + (0x010))
#define SOC_HSDT_CRG_PERDIS1_ADDR(base) ((base) + (0x014))
#define SOC_HSDT_CRG_PERCLKEN1_ADDR(base) ((base) + (0x018))
#define SOC_HSDT_CRG_PERSTAT1_ADDR(base) ((base) + (0x01C))
#define SOC_HSDT_CRG_PERRSTEN0_ADDR(base) ((base) + (0x060))
#define SOC_HSDT_CRG_PERRSTDIS0_ADDR(base) ((base) + (0x064))
#define SOC_HSDT_CRG_PERRSTSTAT0_ADDR(base) ((base) + (0x068))
#define SOC_HSDT_CRG_CLKDIV0_ADDR(base) ((base) + (0x0A8))
#define SOC_HSDT_CRG_CLKDIV1_ADDR(base) ((base) + (0x0AC))
#define SOC_HSDT_CRG_CLKDIV2_ADDR(base) ((base) + (0x0B0))
#define SOC_HSDT_CRG_PERI_STAT0_ADDR(base) ((base) + (0x100))
#define SOC_HSDT_CRG_PERI_STAT1_ADDR(base) ((base) + (0x104))
#define SOC_HSDT_CRG_PERI_STAT2_ADDR(base) ((base) + (0x108))
#define SOC_HSDT_CRG_PCIEPLL_CTRL0_ADDR(base) ((base) + (0x188))
#define SOC_HSDT_CRG_PCIEPLL_CTRL1_ADDR(base) ((base) + (0x18c))
#define SOC_HSDT_CRG_PCIEPLL_CTRL_A_ADDR(base) ((base) + (0x190))
#define SOC_HSDT_CRG_PCIEPLL_CTRL_B_ADDR(base) ((base) + (0x194))
#define SOC_HSDT_CRG_PCIEPLL_STAT_ADDR(base) ((base) + (0x204))
#define SOC_HSDT_CRG_FNPLL_STAT0_ADDR(base) ((base) + (0x208))
#define SOC_HSDT_CRG_FNPLL_STAT1_ADDR(base) ((base) + (0x20C))
#define SOC_HSDT_CRG_JITTER_MONITOR_STAT0_ADDR(base) ((base) + (0x210))
#define SOC_HSDT_CRG_JITTER_MONITOR_STAT1_ADDR(base) ((base) + (0x214))
#define SOC_HSDT_CRG_JITTER_MONITOR_CFG0_ADDR(base) ((base) + (0x21C))
#define SOC_HSDT_CRG_JITTER_MONITOR_CFG1_ADDR(base) ((base) + (0x220))
#define SOC_HSDT_CRG_JITTER_MONITOR_CFG2_ADDR(base) ((base) + (0x224))
#define SOC_HSDT_CRG_FNPLL_CFG0_ADDR(base) ((base) + (0x228))
#define SOC_HSDT_CRG_FNPLL_CFG1_ADDR(base) ((base) + (0x22C))
#define SOC_HSDT_CRG_FNPLL_CFG2_ADDR(base) ((base) + (0x230))
#define SOC_HSDT_CRG_FNPLL_CFG3_ADDR(base) ((base) + (0x234))
#define SOC_HSDT_CRG_FNPLL_CFG4_ADDR(base) ((base) + (0x238))
#define SOC_HSDT_CRG_FNPLL_CFG5_ADDR(base) ((base) + (0x23C))
#define SOC_HSDT_CRG_FNPLL_CFG6_ADDR(base) ((base) + (0x240))
#define SOC_HSDT_CRG_FNPLL_CFG7_ADDR(base) ((base) + (0x244))
#define SOC_HSDT_CRG_PCIECTRL0_ADDR(base) ((base) + (0x300))
#define SOC_HSDT_CRG_IPCLKRST_BYPASS0_ADDR(base) ((base) + (0x318))
#define SOC_HSDT_CRG_PERI_AUTODIV_INUSE_STAT_ADDR(base) ((base) + (0x34C))
#define SOC_HSDT_CRG_PERI_AUTODIV0_ADDR(base) ((base) + (0x350))
#define SOC_HSDT_CRG_PERI_AUTODIV1_ADDR(base) ((base) + (0x354))
#define SOC_HSDT_CRG_PEREN_CC712_SEC_ADDR(base) ((base) + (0xD10))
#define SOC_HSDT_CRG_PERDIS_CC712_SEC_ADDR(base) ((base) + (0xD14))
#define SOC_HSDT_CRG_PERCLKEN_CC712_SEC_ADDR(base) ((base) + (0xD18))
#define SOC_HSDT_CRG_PERSTAT_CC712_SEC_ADDR(base) ((base) + (0xD1C))
#define SOC_HSDT_CRG_INTR_MASK_NOCBUS_NONIDLE_PEND_ADDR(base) ((base) + (0x500))
#define SOC_HSDT_CRG_INTR_CLR_NOCBUS_NONIDLE_PEND_ADDR(base) ((base) + (0x504))
#define SOC_HSDT_CRG_INTR_STAT_NOCBUS_NONIDLE_PEND_ADDR(base) ((base) + (0x508))
#define SOC_HSDT_CRG_PERRSTEN_CC712_SEC_ADDR(base) ((base) + (0xD30))
#define SOC_HSDT_CRG_PERRSTDIS_CC712_SEC_ADDR(base) ((base) + (0xD34))
#define SOC_HSDT_CRG_PERRSTSTAT_CC712_SEC_ADDR(base) ((base) + (0xD38))
#define SOC_HSDT_CRG_CLKDIV_CC712_SEC_ADDR(base) ((base) + (0xD3C))
#define SOC_HSDT_CRG_PERI_CTRL_CC712_SEC_ADDR(base) ((base) + (0xD40))
#endif
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int gt_pclk_hsdt_ioc : 1;
        unsigned int gt_pclk_autodiv_hsdtbus : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int gt_pclk_pcie_trace : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int gt_pclk_hsdt_sysctrl : 1;
        unsigned int reserved_8 : 1;
        unsigned int reserved_9 : 1;
        unsigned int reserved_10 : 1;
        unsigned int reserved_11 : 17;
    } reg;
} SOC_HSDT_CRG_PEREN0_UNION;
#endif
#define SOC_HSDT_CRG_PEREN0_gt_pclk_hsdt_ioc_START (2)
#define SOC_HSDT_CRG_PEREN0_gt_pclk_hsdt_ioc_END (2)
#define SOC_HSDT_CRG_PEREN0_gt_pclk_autodiv_hsdtbus_START (3)
#define SOC_HSDT_CRG_PEREN0_gt_pclk_autodiv_hsdtbus_END (3)
#define SOC_HSDT_CRG_PEREN0_gt_pclk_pcie_trace_START (6)
#define SOC_HSDT_CRG_PEREN0_gt_pclk_pcie_trace_END (6)
#define SOC_HSDT_CRG_PEREN0_gt_pclk_hsdt_sysctrl_START (11)
#define SOC_HSDT_CRG_PEREN0_gt_pclk_hsdt_sysctrl_END (11)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int gt_pclk_hsdt_ioc : 1;
        unsigned int gt_pclk_autodiv_hsdtbus : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int gt_pclk_pcie_trace : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int gt_pclk_hsdt_sysctrl : 1;
        unsigned int reserved_8 : 1;
        unsigned int reserved_9 : 1;
        unsigned int reserved_10 : 1;
        unsigned int reserved_11 : 17;
    } reg;
} SOC_HSDT_CRG_PERDIS0_UNION;
#endif
#define SOC_HSDT_CRG_PERDIS0_gt_pclk_hsdt_ioc_START (2)
#define SOC_HSDT_CRG_PERDIS0_gt_pclk_hsdt_ioc_END (2)
#define SOC_HSDT_CRG_PERDIS0_gt_pclk_autodiv_hsdtbus_START (3)
#define SOC_HSDT_CRG_PERDIS0_gt_pclk_autodiv_hsdtbus_END (3)
#define SOC_HSDT_CRG_PERDIS0_gt_pclk_pcie_trace_START (6)
#define SOC_HSDT_CRG_PERDIS0_gt_pclk_pcie_trace_END (6)
#define SOC_HSDT_CRG_PERDIS0_gt_pclk_hsdt_sysctrl_START (11)
#define SOC_HSDT_CRG_PERDIS0_gt_pclk_hsdt_sysctrl_END (11)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int gt_pclk_hsdt_ioc : 1;
        unsigned int gt_pclk_autodiv_hsdtbus : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int gt_pclk_pcie_trace : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int gt_pclk_hsdt_sysctrl : 1;
        unsigned int reserved_8 : 1;
        unsigned int reserved_9 : 1;
        unsigned int reserved_10 : 1;
        unsigned int reserved_11 : 17;
    } reg;
} SOC_HSDT_CRG_PERCLKEN0_UNION;
#endif
#define SOC_HSDT_CRG_PERCLKEN0_gt_pclk_hsdt_ioc_START (2)
#define SOC_HSDT_CRG_PERCLKEN0_gt_pclk_hsdt_ioc_END (2)
#define SOC_HSDT_CRG_PERCLKEN0_gt_pclk_autodiv_hsdtbus_START (3)
#define SOC_HSDT_CRG_PERCLKEN0_gt_pclk_autodiv_hsdtbus_END (3)
#define SOC_HSDT_CRG_PERCLKEN0_gt_pclk_pcie_trace_START (6)
#define SOC_HSDT_CRG_PERCLKEN0_gt_pclk_pcie_trace_END (6)
#define SOC_HSDT_CRG_PERCLKEN0_gt_pclk_hsdt_sysctrl_START (11)
#define SOC_HSDT_CRG_PERCLKEN0_gt_pclk_hsdt_sysctrl_END (11)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int st_pclk_hsdt_ioc : 1;
        unsigned int st_pclk_autodiv_hsdtbus : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int st_pclk_pcie_trace : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int st_pclk_hsdt_sysctrl : 1;
        unsigned int reserved_8 : 1;
        unsigned int reserved_9 : 1;
        unsigned int reserved_10 : 1;
        unsigned int reserved_11 : 17;
    } reg;
} SOC_HSDT_CRG_PERSTAT0_UNION;
#endif
#define SOC_HSDT_CRG_PERSTAT0_st_pclk_hsdt_ioc_START (2)
#define SOC_HSDT_CRG_PERSTAT0_st_pclk_hsdt_ioc_END (2)
#define SOC_HSDT_CRG_PERSTAT0_st_pclk_autodiv_hsdtbus_START (3)
#define SOC_HSDT_CRG_PERSTAT0_st_pclk_autodiv_hsdtbus_END (3)
#define SOC_HSDT_CRG_PERSTAT0_st_pclk_pcie_trace_START (6)
#define SOC_HSDT_CRG_PERSTAT0_st_pclk_pcie_trace_END (6)
#define SOC_HSDT_CRG_PERSTAT0_st_pclk_hsdt_sysctrl_START (11)
#define SOC_HSDT_CRG_PERSTAT0_st_pclk_hsdt_sysctrl_END (11)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int gt_clk_pcie_hp : 1;
        unsigned int gt_clk_pcie_debounce : 1;
        unsigned int gt_pclk_pcie_phy : 1;
        unsigned int gt_pclk_pcie_sys : 1;
        unsigned int gt_aclk_pcie : 1;
        unsigned int gt_clk_pciephy_ref_soft : 1;
        unsigned int gt_clk_pcieio_soft : 1;
        unsigned int gt_aclk_noc_pcie_asyncbrg : 1;
        unsigned int gt_aclk_pcie_trace : 1;
        unsigned int gt_clk_hsdt_tcu : 1;
        unsigned int gt_clk_hsdt_tbu : 1;
        unsigned int gt_aclk_noc_trace_asyncbrg : 1;
        unsigned int gt_clk_noc_ufs_asyncbrg : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int reserved_8 : 11;
    } reg;
} SOC_HSDT_CRG_PEREN1_UNION;
#endif
#define SOC_HSDT_CRG_PEREN1_gt_clk_pcie_hp_START (1)
#define SOC_HSDT_CRG_PEREN1_gt_clk_pcie_hp_END (1)
#define SOC_HSDT_CRG_PEREN1_gt_clk_pcie_debounce_START (2)
#define SOC_HSDT_CRG_PEREN1_gt_clk_pcie_debounce_END (2)
#define SOC_HSDT_CRG_PEREN1_gt_pclk_pcie_phy_START (3)
#define SOC_HSDT_CRG_PEREN1_gt_pclk_pcie_phy_END (3)
#define SOC_HSDT_CRG_PEREN1_gt_pclk_pcie_sys_START (4)
#define SOC_HSDT_CRG_PEREN1_gt_pclk_pcie_sys_END (4)
#define SOC_HSDT_CRG_PEREN1_gt_aclk_pcie_START (5)
#define SOC_HSDT_CRG_PEREN1_gt_aclk_pcie_END (5)
#define SOC_HSDT_CRG_PEREN1_gt_clk_pciephy_ref_soft_START (6)
#define SOC_HSDT_CRG_PEREN1_gt_clk_pciephy_ref_soft_END (6)
#define SOC_HSDT_CRG_PEREN1_gt_clk_pcieio_soft_START (7)
#define SOC_HSDT_CRG_PEREN1_gt_clk_pcieio_soft_END (7)
#define SOC_HSDT_CRG_PEREN1_gt_aclk_noc_pcie_asyncbrg_START (8)
#define SOC_HSDT_CRG_PEREN1_gt_aclk_noc_pcie_asyncbrg_END (8)
#define SOC_HSDT_CRG_PEREN1_gt_aclk_pcie_trace_START (9)
#define SOC_HSDT_CRG_PEREN1_gt_aclk_pcie_trace_END (9)
#define SOC_HSDT_CRG_PEREN1_gt_clk_hsdt_tcu_START (10)
#define SOC_HSDT_CRG_PEREN1_gt_clk_hsdt_tcu_END (10)
#define SOC_HSDT_CRG_PEREN1_gt_clk_hsdt_tbu_START (11)
#define SOC_HSDT_CRG_PEREN1_gt_clk_hsdt_tbu_END (11)
#define SOC_HSDT_CRG_PEREN1_gt_aclk_noc_trace_asyncbrg_START (12)
#define SOC_HSDT_CRG_PEREN1_gt_aclk_noc_trace_asyncbrg_END (12)
#define SOC_HSDT_CRG_PEREN1_gt_clk_noc_ufs_asyncbrg_START (13)
#define SOC_HSDT_CRG_PEREN1_gt_clk_noc_ufs_asyncbrg_END (13)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int gt_clk_pcie_hp : 1;
        unsigned int gt_clk_pcie_debounce : 1;
        unsigned int gt_pclk_pcie_phy : 1;
        unsigned int gt_pclk_pcie_sys : 1;
        unsigned int gt_aclk_pcie : 1;
        unsigned int gt_clk_pciephy_ref_soft : 1;
        unsigned int gt_clk_pcieio_soft : 1;
        unsigned int gt_aclk_noc_pcie_asyncbrg : 1;
        unsigned int gt_aclk_pcie_trace : 1;
        unsigned int gt_clk_hsdt_tcu : 1;
        unsigned int gt_clk_hsdt_tbu : 1;
        unsigned int gt_aclk_noc_trace_asyncbrg : 1;
        unsigned int gt_clk_noc_ufs_asyncbrg : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int reserved_8 : 11;
    } reg;
} SOC_HSDT_CRG_PERDIS1_UNION;
#endif
#define SOC_HSDT_CRG_PERDIS1_gt_clk_pcie_hp_START (1)
#define SOC_HSDT_CRG_PERDIS1_gt_clk_pcie_hp_END (1)
#define SOC_HSDT_CRG_PERDIS1_gt_clk_pcie_debounce_START (2)
#define SOC_HSDT_CRG_PERDIS1_gt_clk_pcie_debounce_END (2)
#define SOC_HSDT_CRG_PERDIS1_gt_pclk_pcie_phy_START (3)
#define SOC_HSDT_CRG_PERDIS1_gt_pclk_pcie_phy_END (3)
#define SOC_HSDT_CRG_PERDIS1_gt_pclk_pcie_sys_START (4)
#define SOC_HSDT_CRG_PERDIS1_gt_pclk_pcie_sys_END (4)
#define SOC_HSDT_CRG_PERDIS1_gt_aclk_pcie_START (5)
#define SOC_HSDT_CRG_PERDIS1_gt_aclk_pcie_END (5)
#define SOC_HSDT_CRG_PERDIS1_gt_clk_pciephy_ref_soft_START (6)
#define SOC_HSDT_CRG_PERDIS1_gt_clk_pciephy_ref_soft_END (6)
#define SOC_HSDT_CRG_PERDIS1_gt_clk_pcieio_soft_START (7)
#define SOC_HSDT_CRG_PERDIS1_gt_clk_pcieio_soft_END (7)
#define SOC_HSDT_CRG_PERDIS1_gt_aclk_noc_pcie_asyncbrg_START (8)
#define SOC_HSDT_CRG_PERDIS1_gt_aclk_noc_pcie_asyncbrg_END (8)
#define SOC_HSDT_CRG_PERDIS1_gt_aclk_pcie_trace_START (9)
#define SOC_HSDT_CRG_PERDIS1_gt_aclk_pcie_trace_END (9)
#define SOC_HSDT_CRG_PERDIS1_gt_clk_hsdt_tcu_START (10)
#define SOC_HSDT_CRG_PERDIS1_gt_clk_hsdt_tcu_END (10)
#define SOC_HSDT_CRG_PERDIS1_gt_clk_hsdt_tbu_START (11)
#define SOC_HSDT_CRG_PERDIS1_gt_clk_hsdt_tbu_END (11)
#define SOC_HSDT_CRG_PERDIS1_gt_aclk_noc_trace_asyncbrg_START (12)
#define SOC_HSDT_CRG_PERDIS1_gt_aclk_noc_trace_asyncbrg_END (12)
#define SOC_HSDT_CRG_PERDIS1_gt_clk_noc_ufs_asyncbrg_START (13)
#define SOC_HSDT_CRG_PERDIS1_gt_clk_noc_ufs_asyncbrg_END (13)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int gt_clk_pcie_hp : 1;
        unsigned int gt_clk_pcie_debounce : 1;
        unsigned int gt_pclk_pcie_phy : 1;
        unsigned int gt_pclk_pcie_sys : 1;
        unsigned int gt_aclk_pcie : 1;
        unsigned int gt_clk_pciephy_ref_soft : 1;
        unsigned int gt_clk_pcieio_soft : 1;
        unsigned int gt_aclk_noc_pcie_asyncbrg : 1;
        unsigned int gt_aclk_pcie_trace : 1;
        unsigned int gt_clk_hsdt_tcu : 1;
        unsigned int gt_clk_hsdt_tbu : 1;
        unsigned int gt_aclk_noc_trace_asyncbrg : 1;
        unsigned int gt_clk_noc_ufs_asyncbrg : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int reserved_8 : 11;
    } reg;
} SOC_HSDT_CRG_PERCLKEN1_UNION;
#endif
#define SOC_HSDT_CRG_PERCLKEN1_gt_clk_pcie_hp_START (1)
#define SOC_HSDT_CRG_PERCLKEN1_gt_clk_pcie_hp_END (1)
#define SOC_HSDT_CRG_PERCLKEN1_gt_clk_pcie_debounce_START (2)
#define SOC_HSDT_CRG_PERCLKEN1_gt_clk_pcie_debounce_END (2)
#define SOC_HSDT_CRG_PERCLKEN1_gt_pclk_pcie_phy_START (3)
#define SOC_HSDT_CRG_PERCLKEN1_gt_pclk_pcie_phy_END (3)
#define SOC_HSDT_CRG_PERCLKEN1_gt_pclk_pcie_sys_START (4)
#define SOC_HSDT_CRG_PERCLKEN1_gt_pclk_pcie_sys_END (4)
#define SOC_HSDT_CRG_PERCLKEN1_gt_aclk_pcie_START (5)
#define SOC_HSDT_CRG_PERCLKEN1_gt_aclk_pcie_END (5)
#define SOC_HSDT_CRG_PERCLKEN1_gt_clk_pciephy_ref_soft_START (6)
#define SOC_HSDT_CRG_PERCLKEN1_gt_clk_pciephy_ref_soft_END (6)
#define SOC_HSDT_CRG_PERCLKEN1_gt_clk_pcieio_soft_START (7)
#define SOC_HSDT_CRG_PERCLKEN1_gt_clk_pcieio_soft_END (7)
#define SOC_HSDT_CRG_PERCLKEN1_gt_aclk_noc_pcie_asyncbrg_START (8)
#define SOC_HSDT_CRG_PERCLKEN1_gt_aclk_noc_pcie_asyncbrg_END (8)
#define SOC_HSDT_CRG_PERCLKEN1_gt_aclk_pcie_trace_START (9)
#define SOC_HSDT_CRG_PERCLKEN1_gt_aclk_pcie_trace_END (9)
#define SOC_HSDT_CRG_PERCLKEN1_gt_clk_hsdt_tcu_START (10)
#define SOC_HSDT_CRG_PERCLKEN1_gt_clk_hsdt_tcu_END (10)
#define SOC_HSDT_CRG_PERCLKEN1_gt_clk_hsdt_tbu_START (11)
#define SOC_HSDT_CRG_PERCLKEN1_gt_clk_hsdt_tbu_END (11)
#define SOC_HSDT_CRG_PERCLKEN1_gt_aclk_noc_trace_asyncbrg_START (12)
#define SOC_HSDT_CRG_PERCLKEN1_gt_aclk_noc_trace_asyncbrg_END (12)
#define SOC_HSDT_CRG_PERCLKEN1_gt_clk_noc_ufs_asyncbrg_START (13)
#define SOC_HSDT_CRG_PERCLKEN1_gt_clk_noc_ufs_asyncbrg_END (13)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int st_clk_pcie_hp : 1;
        unsigned int st_clk_pcie_debounce : 1;
        unsigned int st_pclk_pcie_phy : 1;
        unsigned int st_pclk_pcie_sys : 1;
        unsigned int st_aclk_pcie : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int st_aclk_noc_pcie_asyncbrg : 1;
        unsigned int st_aclk_pcie_trace : 1;
        unsigned int st_clk_hsdt_tcu : 1;
        unsigned int st_clk_hsdt_tbu : 1;
        unsigned int st_aclk_noc_trace_asyncbrg : 1;
        unsigned int st_clk_noc_ufs_asyncbrg : 1;
        unsigned int reserved_3 : 1;
        unsigned int reserved_4 : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int reserved_8 : 1;
        unsigned int reserved_9 : 1;
        unsigned int reserved_10 : 11;
    } reg;
} SOC_HSDT_CRG_PERSTAT1_UNION;
#endif
#define SOC_HSDT_CRG_PERSTAT1_st_clk_pcie_hp_START (1)
#define SOC_HSDT_CRG_PERSTAT1_st_clk_pcie_hp_END (1)
#define SOC_HSDT_CRG_PERSTAT1_st_clk_pcie_debounce_START (2)
#define SOC_HSDT_CRG_PERSTAT1_st_clk_pcie_debounce_END (2)
#define SOC_HSDT_CRG_PERSTAT1_st_pclk_pcie_phy_START (3)
#define SOC_HSDT_CRG_PERSTAT1_st_pclk_pcie_phy_END (3)
#define SOC_HSDT_CRG_PERSTAT1_st_pclk_pcie_sys_START (4)
#define SOC_HSDT_CRG_PERSTAT1_st_pclk_pcie_sys_END (4)
#define SOC_HSDT_CRG_PERSTAT1_st_aclk_pcie_START (5)
#define SOC_HSDT_CRG_PERSTAT1_st_aclk_pcie_END (5)
#define SOC_HSDT_CRG_PERSTAT1_st_aclk_noc_pcie_asyncbrg_START (8)
#define SOC_HSDT_CRG_PERSTAT1_st_aclk_noc_pcie_asyncbrg_END (8)
#define SOC_HSDT_CRG_PERSTAT1_st_aclk_pcie_trace_START (9)
#define SOC_HSDT_CRG_PERSTAT1_st_aclk_pcie_trace_END (9)
#define SOC_HSDT_CRG_PERSTAT1_st_clk_hsdt_tcu_START (10)
#define SOC_HSDT_CRG_PERSTAT1_st_clk_hsdt_tcu_END (10)
#define SOC_HSDT_CRG_PERSTAT1_st_clk_hsdt_tbu_START (11)
#define SOC_HSDT_CRG_PERSTAT1_st_clk_hsdt_tbu_END (11)
#define SOC_HSDT_CRG_PERSTAT1_st_aclk_noc_trace_asyncbrg_START (12)
#define SOC_HSDT_CRG_PERSTAT1_st_aclk_noc_trace_asyncbrg_END (12)
#define SOC_HSDT_CRG_PERSTAT1_st_clk_noc_ufs_asyncbrg_START (13)
#define SOC_HSDT_CRG_PERSTAT1_st_clk_noc_ufs_asyncbrg_END (13)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_rst_noc_ufs_asyncbrg : 1;
        unsigned int reserved_0 : 1;
        unsigned int ip_prst_hsdt_ioc : 1;
        unsigned int ip_rst_hsdt_tcu : 1;
        unsigned int ip_arst_pcie0_asyncbrg : 1;
        unsigned int ip_rst_hsdt_tbu : 1;
        unsigned int ip_prst_hsdt_sysctrl : 1;
        unsigned int ip_rst_pcie : 1;
        unsigned int ip_prst_pcie_sys : 1;
        unsigned int ip_prst_pcie_phy : 1;
        unsigned int ip_rst_pcie_trace : 1;
        unsigned int ip_prst_pcie_trace : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 19;
    } reg;
} SOC_HSDT_CRG_PERRSTEN0_UNION;
#endif
#define SOC_HSDT_CRG_PERRSTEN0_ip_rst_noc_ufs_asyncbrg_START (0)
#define SOC_HSDT_CRG_PERRSTEN0_ip_rst_noc_ufs_asyncbrg_END (0)
#define SOC_HSDT_CRG_PERRSTEN0_ip_prst_hsdt_ioc_START (2)
#define SOC_HSDT_CRG_PERRSTEN0_ip_prst_hsdt_ioc_END (2)
#define SOC_HSDT_CRG_PERRSTEN0_ip_rst_hsdt_tcu_START (3)
#define SOC_HSDT_CRG_PERRSTEN0_ip_rst_hsdt_tcu_END (3)
#define SOC_HSDT_CRG_PERRSTEN0_ip_arst_pcie0_asyncbrg_START (4)
#define SOC_HSDT_CRG_PERRSTEN0_ip_arst_pcie0_asyncbrg_END (4)
#define SOC_HSDT_CRG_PERRSTEN0_ip_rst_hsdt_tbu_START (5)
#define SOC_HSDT_CRG_PERRSTEN0_ip_rst_hsdt_tbu_END (5)
#define SOC_HSDT_CRG_PERRSTEN0_ip_prst_hsdt_sysctrl_START (6)
#define SOC_HSDT_CRG_PERRSTEN0_ip_prst_hsdt_sysctrl_END (6)
#define SOC_HSDT_CRG_PERRSTEN0_ip_rst_pcie_START (7)
#define SOC_HSDT_CRG_PERRSTEN0_ip_rst_pcie_END (7)
#define SOC_HSDT_CRG_PERRSTEN0_ip_prst_pcie_sys_START (8)
#define SOC_HSDT_CRG_PERRSTEN0_ip_prst_pcie_sys_END (8)
#define SOC_HSDT_CRG_PERRSTEN0_ip_prst_pcie_phy_START (9)
#define SOC_HSDT_CRG_PERRSTEN0_ip_prst_pcie_phy_END (9)
#define SOC_HSDT_CRG_PERRSTEN0_ip_rst_pcie_trace_START (10)
#define SOC_HSDT_CRG_PERRSTEN0_ip_rst_pcie_trace_END (10)
#define SOC_HSDT_CRG_PERRSTEN0_ip_prst_pcie_trace_START (11)
#define SOC_HSDT_CRG_PERRSTEN0_ip_prst_pcie_trace_END (11)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_rst_noc_ufs_asyncbrg : 1;
        unsigned int reserved_0 : 1;
        unsigned int ip_prst_hsdt_ioc : 1;
        unsigned int ip_rst_hsdt_tcu : 1;
        unsigned int ip_arst_pcie0_asyncbrg : 1;
        unsigned int ip_rst_hsdt_tbu : 1;
        unsigned int ip_prst_hsdt_sysctrl : 1;
        unsigned int ip_rst_pcie : 1;
        unsigned int ip_prst_pcie_sys : 1;
        unsigned int ip_prst_pcie_phy : 1;
        unsigned int ip_rst_pcie_trace : 1;
        unsigned int ip_prst_pcie_trace : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 19;
    } reg;
} SOC_HSDT_CRG_PERRSTDIS0_UNION;
#endif
#define SOC_HSDT_CRG_PERRSTDIS0_ip_rst_noc_ufs_asyncbrg_START (0)
#define SOC_HSDT_CRG_PERRSTDIS0_ip_rst_noc_ufs_asyncbrg_END (0)
#define SOC_HSDT_CRG_PERRSTDIS0_ip_prst_hsdt_ioc_START (2)
#define SOC_HSDT_CRG_PERRSTDIS0_ip_prst_hsdt_ioc_END (2)
#define SOC_HSDT_CRG_PERRSTDIS0_ip_rst_hsdt_tcu_START (3)
#define SOC_HSDT_CRG_PERRSTDIS0_ip_rst_hsdt_tcu_END (3)
#define SOC_HSDT_CRG_PERRSTDIS0_ip_arst_pcie0_asyncbrg_START (4)
#define SOC_HSDT_CRG_PERRSTDIS0_ip_arst_pcie0_asyncbrg_END (4)
#define SOC_HSDT_CRG_PERRSTDIS0_ip_rst_hsdt_tbu_START (5)
#define SOC_HSDT_CRG_PERRSTDIS0_ip_rst_hsdt_tbu_END (5)
#define SOC_HSDT_CRG_PERRSTDIS0_ip_prst_hsdt_sysctrl_START (6)
#define SOC_HSDT_CRG_PERRSTDIS0_ip_prst_hsdt_sysctrl_END (6)
#define SOC_HSDT_CRG_PERRSTDIS0_ip_rst_pcie_START (7)
#define SOC_HSDT_CRG_PERRSTDIS0_ip_rst_pcie_END (7)
#define SOC_HSDT_CRG_PERRSTDIS0_ip_prst_pcie_sys_START (8)
#define SOC_HSDT_CRG_PERRSTDIS0_ip_prst_pcie_sys_END (8)
#define SOC_HSDT_CRG_PERRSTDIS0_ip_prst_pcie_phy_START (9)
#define SOC_HSDT_CRG_PERRSTDIS0_ip_prst_pcie_phy_END (9)
#define SOC_HSDT_CRG_PERRSTDIS0_ip_rst_pcie_trace_START (10)
#define SOC_HSDT_CRG_PERRSTDIS0_ip_rst_pcie_trace_END (10)
#define SOC_HSDT_CRG_PERRSTDIS0_ip_prst_pcie_trace_START (11)
#define SOC_HSDT_CRG_PERRSTDIS0_ip_prst_pcie_trace_END (11)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_rst_noc_ufs_asyncbrg : 1;
        unsigned int reserved_0 : 1;
        unsigned int ip_prst_hsdt_ioc : 1;
        unsigned int ip_rst_hsdt_tcu : 1;
        unsigned int ip_arst_pcie0_asyncbrg : 1;
        unsigned int ip_rst_hsdt_tbu : 1;
        unsigned int ip_prst_hsdt_sysctrl : 1;
        unsigned int ip_rst_pcie : 1;
        unsigned int ip_prst_pcie_sys : 1;
        unsigned int ip_prst_pcie_phy : 1;
        unsigned int ip_rst_pcie_trace : 1;
        unsigned int ip_prst_pcie_trace : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 19;
    } reg;
} SOC_HSDT_CRG_PERRSTSTAT0_UNION;
#endif
#define SOC_HSDT_CRG_PERRSTSTAT0_ip_rst_noc_ufs_asyncbrg_START (0)
#define SOC_HSDT_CRG_PERRSTSTAT0_ip_rst_noc_ufs_asyncbrg_END (0)
#define SOC_HSDT_CRG_PERRSTSTAT0_ip_prst_hsdt_ioc_START (2)
#define SOC_HSDT_CRG_PERRSTSTAT0_ip_prst_hsdt_ioc_END (2)
#define SOC_HSDT_CRG_PERRSTSTAT0_ip_rst_hsdt_tcu_START (3)
#define SOC_HSDT_CRG_PERRSTSTAT0_ip_rst_hsdt_tcu_END (3)
#define SOC_HSDT_CRG_PERRSTSTAT0_ip_arst_pcie0_asyncbrg_START (4)
#define SOC_HSDT_CRG_PERRSTSTAT0_ip_arst_pcie0_asyncbrg_END (4)
#define SOC_HSDT_CRG_PERRSTSTAT0_ip_rst_hsdt_tbu_START (5)
#define SOC_HSDT_CRG_PERRSTSTAT0_ip_rst_hsdt_tbu_END (5)
#define SOC_HSDT_CRG_PERRSTSTAT0_ip_prst_hsdt_sysctrl_START (6)
#define SOC_HSDT_CRG_PERRSTSTAT0_ip_prst_hsdt_sysctrl_END (6)
#define SOC_HSDT_CRG_PERRSTSTAT0_ip_rst_pcie_START (7)
#define SOC_HSDT_CRG_PERRSTSTAT0_ip_rst_pcie_END (7)
#define SOC_HSDT_CRG_PERRSTSTAT0_ip_prst_pcie_sys_START (8)
#define SOC_HSDT_CRG_PERRSTSTAT0_ip_prst_pcie_sys_END (8)
#define SOC_HSDT_CRG_PERRSTSTAT0_ip_prst_pcie_phy_START (9)
#define SOC_HSDT_CRG_PERRSTSTAT0_ip_prst_pcie_phy_END (9)
#define SOC_HSDT_CRG_PERRSTSTAT0_ip_rst_pcie_trace_START (10)
#define SOC_HSDT_CRG_PERRSTSTAT0_ip_rst_pcie_trace_END (10)
#define SOC_HSDT_CRG_PERRSTSTAT0_ip_prst_pcie_trace_START (11)
#define SOC_HSDT_CRG_PERRSTSTAT0_ip_prst_pcie_trace_END (11)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 4;
        unsigned int reserved_1 : 1;
        unsigned int div_hsdt_subsys : 4;
        unsigned int div_hsdt_subsys_auto : 4;
        unsigned int reserved_2 : 2;
        unsigned int reserved_3 : 1;
        unsigned int bitmasken : 16;
    } reg;
} SOC_HSDT_CRG_CLKDIV0_UNION;
#endif
#define SOC_HSDT_CRG_CLKDIV0_div_hsdt_subsys_START (5)
#define SOC_HSDT_CRG_CLKDIV0_div_hsdt_subsys_END (8)
#define SOC_HSDT_CRG_CLKDIV0_div_hsdt_subsys_auto_START (9)
#define SOC_HSDT_CRG_CLKDIV0_div_hsdt_subsys_auto_END (12)
#define SOC_HSDT_CRG_CLKDIV0_bitmasken_START (16)
#define SOC_HSDT_CRG_CLKDIV0_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int sc_gt_clk_hsdt_subsys : 1;
        unsigned int sc_gt_clk_pciephy_trans : 1;
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
        unsigned int bitmasken : 16;
    } reg;
} SOC_HSDT_CRG_CLKDIV1_UNION;
#endif
#define SOC_HSDT_CRG_CLKDIV1_sc_gt_clk_hsdt_subsys_START (2)
#define SOC_HSDT_CRG_CLKDIV1_sc_gt_clk_hsdt_subsys_END (2)
#define SOC_HSDT_CRG_CLKDIV1_sc_gt_clk_pciephy_trans_START (3)
#define SOC_HSDT_CRG_CLKDIV1_sc_gt_clk_pciephy_trans_END (3)
#define SOC_HSDT_CRG_CLKDIV1_bitmasken_START (16)
#define SOC_HSDT_CRG_CLKDIV1_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0: 4;
        unsigned int reserved_1: 1;
        unsigned int reserved_2: 4;
        unsigned int reserved_3: 4;
        unsigned int reserved_4: 1;
        unsigned int reserved_5: 2;
        unsigned int bitmasken : 16;
    } reg;
} SOC_HSDT_CRG_CLKDIV2_UNION;
#endif
#define SOC_HSDT_CRG_CLKDIV2_bitmasken_START (16)
#define SOC_HSDT_CRG_CLKDIV2_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int swdone_clk_sec_div : 1;
        unsigned int swdone_clk_hsdt_subsys_div : 1;
        unsigned int swdone_clk_autodiv_hsdt_subsys_div : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 27;
    } reg;
} SOC_HSDT_CRG_PERI_STAT0_UNION;
#endif
#define SOC_HSDT_CRG_PERI_STAT0_swdone_clk_sec_div_START (0)
#define SOC_HSDT_CRG_PERI_STAT0_swdone_clk_sec_div_END (0)
#define SOC_HSDT_CRG_PERI_STAT0_swdone_clk_hsdt_subsys_div_START (1)
#define SOC_HSDT_CRG_PERI_STAT0_swdone_clk_hsdt_subsys_div_END (1)
#define SOC_HSDT_CRG_PERI_STAT0_swdone_clk_autodiv_hsdt_subsys_div_START (2)
#define SOC_HSDT_CRG_PERI_STAT0_swdone_clk_autodiv_hsdt_subsys_div_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sw_ack_clk_sec_sw : 2;
        unsigned int reserved_0 : 2;
        unsigned int reserved_1 : 28;
    } reg;
} SOC_HSDT_CRG_PERI_STAT1_UNION;
#endif
#define SOC_HSDT_CRG_PERI_STAT1_sw_ack_clk_sec_sw_START (0)
#define SOC_HSDT_CRG_PERI_STAT1_sw_ack_clk_sec_sw_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int st_clk_pciephy_ref : 1;
        unsigned int st_clk_pcieio : 1;
        unsigned int st_clk_noc_cc712 : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 1;
        unsigned int st_clk_noc_hsdt2ufs : 1;
        unsigned int reserved_4 : 1;
        unsigned int st_pclk_pciephy_asyncbrg : 1;
        unsigned int st_pclk_pciesys_asyncbrg : 1;
        unsigned int reserved_5 : 1;
        unsigned int reserved_6 : 1;
        unsigned int reserved_7 : 1;
        unsigned int reserved_8 : 18;
    } reg;
} SOC_HSDT_CRG_PERI_STAT2_UNION;
#endif
#define SOC_HSDT_CRG_PERI_STAT2_st_clk_pciephy_ref_START (0)
#define SOC_HSDT_CRG_PERI_STAT2_st_clk_pciephy_ref_END (0)
#define SOC_HSDT_CRG_PERI_STAT2_st_clk_pcieio_START (1)
#define SOC_HSDT_CRG_PERI_STAT2_st_clk_pcieio_END (1)
#define SOC_HSDT_CRG_PERI_STAT2_st_clk_noc_cc712_START (2)
#define SOC_HSDT_CRG_PERI_STAT2_st_clk_noc_cc712_END (2)
#define SOC_HSDT_CRG_PERI_STAT2_st_clk_noc_hsdt2ufs_START (7)
#define SOC_HSDT_CRG_PERI_STAT2_st_clk_noc_hsdt2ufs_END (7)
#define SOC_HSDT_CRG_PERI_STAT2_st_pclk_pciephy_asyncbrg_START (9)
#define SOC_HSDT_CRG_PERI_STAT2_st_pclk_pciephy_asyncbrg_END (9)
#define SOC_HSDT_CRG_PERI_STAT2_st_pclk_pciesys_asyncbrg_START (10)
#define SOC_HSDT_CRG_PERI_STAT2_st_pclk_pciesys_asyncbrg_END (10)
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
} SOC_HSDT_CRG_PCIEPLL_CTRL0_UNION;
#endif
#define SOC_HSDT_CRG_PCIEPLL_CTRL0_pciepll_en_START (0)
#define SOC_HSDT_CRG_PCIEPLL_CTRL0_pciepll_en_END (0)
#define SOC_HSDT_CRG_PCIEPLL_CTRL0_pciepll_bp_START (1)
#define SOC_HSDT_CRG_PCIEPLL_CTRL0_pciepll_bp_END (1)
#define SOC_HSDT_CRG_PCIEPLL_CTRL0_pciepll_refdiv_START (2)
#define SOC_HSDT_CRG_PCIEPLL_CTRL0_pciepll_refdiv_END (7)
#define SOC_HSDT_CRG_PCIEPLL_CTRL0_pciepll_fbdiv_START (8)
#define SOC_HSDT_CRG_PCIEPLL_CTRL0_pciepll_fbdiv_END (19)
#define SOC_HSDT_CRG_PCIEPLL_CTRL0_pciepll_postdiv1_START (20)
#define SOC_HSDT_CRG_PCIEPLL_CTRL0_pciepll_postdiv1_END (22)
#define SOC_HSDT_CRG_PCIEPLL_CTRL0_pciepll_postdiv2_START (23)
#define SOC_HSDT_CRG_PCIEPLL_CTRL0_pciepll_postdiv2_END (25)
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
} SOC_HSDT_CRG_PCIEPLL_CTRL1_UNION;
#endif
#define SOC_HSDT_CRG_PCIEPLL_CTRL1_pciepll_fracdiv_START (0)
#define SOC_HSDT_CRG_PCIEPLL_CTRL1_pciepll_fracdiv_END (23)
#define SOC_HSDT_CRG_PCIEPLL_CTRL1_pciepll_int_mod_START (24)
#define SOC_HSDT_CRG_PCIEPLL_CTRL1_pciepll_int_mod_END (24)
#define SOC_HSDT_CRG_PCIEPLL_CTRL1_pciepll_cfg_vld_START (25)
#define SOC_HSDT_CRG_PCIEPLL_CTRL1_pciepll_cfg_vld_END (25)
#define SOC_HSDT_CRG_PCIEPLL_CTRL1_gt_clk_pciepll_START (26)
#define SOC_HSDT_CRG_PCIEPLL_CTRL1_gt_clk_pciepll_END (26)
#define SOC_HSDT_CRG_PCIEPLL_CTRL1_pciepll_sel_START (27)
#define SOC_HSDT_CRG_PCIEPLL_CTRL1_pciepll_sel_END (27)
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
} SOC_HSDT_CRG_PCIEPLL_CTRL_A_UNION;
#endif
#define SOC_HSDT_CRG_PCIEPLL_CTRL_A_pll_bbdrv_START (0)
#define SOC_HSDT_CRG_PCIEPLL_CTRL_A_pll_bbdrv_END (2)
#define SOC_HSDT_CRG_PCIEPLL_CTRL_A_pll_bben_START (3)
#define SOC_HSDT_CRG_PCIEPLL_CTRL_A_pll_bben_END (3)
#define SOC_HSDT_CRG_PCIEPLL_CTRL_A_pll_bbenoverride_START (4)
#define SOC_HSDT_CRG_PCIEPLL_CTRL_A_pll_bbenoverride_END (4)
#define SOC_HSDT_CRG_PCIEPLL_CTRL_A_pll_bbpw_START (5)
#define SOC_HSDT_CRG_PCIEPLL_CTRL_A_pll_bbpw_END (7)
#define SOC_HSDT_CRG_PCIEPLL_CTRL_A_pll_bwprog_START (8)
#define SOC_HSDT_CRG_PCIEPLL_CTRL_A_pll_bwprog_END (10)
#define SOC_HSDT_CRG_PCIEPLL_CTRL_A_pll_bwprogen_START (11)
#define SOC_HSDT_CRG_PCIEPLL_CTRL_A_pll_bwprogen_END (11)
#define SOC_HSDT_CRG_PCIEPLL_CTRL_A_pll_foutvco_en_START (12)
#define SOC_HSDT_CRG_PCIEPLL_CTRL_A_pll_foutvco_en_END (12)
#define SOC_HSDT_CRG_PCIEPLL_CTRL_A_pll_offsetcalbyp_START (13)
#define SOC_HSDT_CRG_PCIEPLL_CTRL_A_pll_offsetcalbyp_END (13)
#define SOC_HSDT_CRG_PCIEPLL_CTRL_A_pll_offsetcalcnt_START (14)
#define SOC_HSDT_CRG_PCIEPLL_CTRL_A_pll_offsetcalcnt_END (16)
#define SOC_HSDT_CRG_PCIEPLL_CTRL_A_pll_offsetcalen_START (17)
#define SOC_HSDT_CRG_PCIEPLL_CTRL_A_pll_offsetcalen_END (17)
#define SOC_HSDT_CRG_PCIEPLL_CTRL_A_pll_offsetcalin_START (18)
#define SOC_HSDT_CRG_PCIEPLL_CTRL_A_pll_offsetcalin_END (29)
#define SOC_HSDT_CRG_PCIEPLL_CTRL_A_pll_offsetfastcal_START (30)
#define SOC_HSDT_CRG_PCIEPLL_CTRL_A_pll_offsetfastcal_END (30)
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
} SOC_HSDT_CRG_PCIEPLL_CTRL_B_UNION;
#endif
#define SOC_HSDT_CRG_PCIEPLL_CTRL_B_pll_vcosel_START (0)
#define SOC_HSDT_CRG_PCIEPLL_CTRL_B_pll_vcosel_END (1)
#define SOC_HSDT_CRG_PCIEPLL_CTRL_B_pll_qpiprog_START (2)
#define SOC_HSDT_CRG_PCIEPLL_CTRL_B_pll_qpiprog_END (4)
#define SOC_HSDT_CRG_PCIEPLL_CTRL_B_pll_qpiprogen_START (5)
#define SOC_HSDT_CRG_PCIEPLL_CTRL_B_pll_qpiprogen_END (5)
#define SOC_HSDT_CRG_PCIEPLL_CTRL_B_pll_qpiskewprog_START (6)
#define SOC_HSDT_CRG_PCIEPLL_CTRL_B_pll_qpiskewprog_END (8)
#define SOC_HSDT_CRG_PCIEPLL_CTRL_B_pll_qpiskewprogen_START (9)
#define SOC_HSDT_CRG_PCIEPLL_CTRL_B_pll_qpiskewprogen_END (9)
#define SOC_HSDT_CRG_PCIEPLL_CTRL_B_pll_fprog_START (10)
#define SOC_HSDT_CRG_PCIEPLL_CTRL_B_pll_fprog_END (12)
#define SOC_HSDT_CRG_PCIEPLL_CTRL_B_pll_fprogen_START (13)
#define SOC_HSDT_CRG_PCIEPLL_CTRL_B_pll_fprogen_END (13)
#define SOC_HSDT_CRG_PCIEPLL_CTRL_B_pll_reserved_START (16)
#define SOC_HSDT_CRG_PCIEPLL_CTRL_B_pll_reserved_END (31)
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
} SOC_HSDT_CRG_PCIEPLL_STAT_UNION;
#endif
#define SOC_HSDT_CRG_PCIEPLL_STAT_pciepll_sc_lock_START (0)
#define SOC_HSDT_CRG_PCIEPLL_STAT_pciepll_sc_lock_END (0)
#define SOC_HSDT_CRG_PCIEPLL_STAT_pciepll_sc_en_stat_START (1)
#define SOC_HSDT_CRG_PCIEPLL_STAT_pciepll_sc_en_stat_END (1)
#define SOC_HSDT_CRG_PCIEPLL_STAT_pciepll_sc_bypass_stat_START (2)
#define SOC_HSDT_CRG_PCIEPLL_STAT_pciepll_sc_bypass_stat_END (2)
#define SOC_HSDT_CRG_PCIEPLL_STAT_pciepll_sc_gt_stat_START (3)
#define SOC_HSDT_CRG_PCIEPLL_STAT_pciepll_sc_gt_stat_END (3)
#define SOC_HSDT_CRG_PCIEPLL_STAT_pciepll_fn_lock_START (4)
#define SOC_HSDT_CRG_PCIEPLL_STAT_pciepll_fn_lock_END (4)
#define SOC_HSDT_CRG_PCIEPLL_STAT_pciepll_fn_en_stat_START (5)
#define SOC_HSDT_CRG_PCIEPLL_STAT_pciepll_fn_en_stat_END (5)
#define SOC_HSDT_CRG_PCIEPLL_STAT_pciepll_fn_bypass_stat_START (6)
#define SOC_HSDT_CRG_PCIEPLL_STAT_pciepll_fn_bypass_stat_END (6)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int lock_counter_fnpll_1v2 : 11;
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
        unsigned int reserved : 1;
        unsigned int test_data_fnpll_1v2 : 8;
    } reg;
} SOC_HSDT_CRG_FNPLL_STAT0_UNION;
#endif
#define SOC_HSDT_CRG_FNPLL_STAT0_lock_counter_fnpll_1v2_START (0)
#define SOC_HSDT_CRG_FNPLL_STAT0_lock_counter_fnpll_1v2_END (10)
#define SOC_HSDT_CRG_FNPLL_STAT0_div2_pd_test_fnpll_1v2_START (11)
#define SOC_HSDT_CRG_FNPLL_STAT0_div2_pd_test_fnpll_1v2_END (11)
#define SOC_HSDT_CRG_FNPLL_STAT0_fbdiv_rst_n_test_fnpll_1v2_START (12)
#define SOC_HSDT_CRG_FNPLL_STAT0_fbdiv_rst_n_test_fnpll_1v2_END (12)
#define SOC_HSDT_CRG_FNPLL_STAT0_refdiv_rst_n_test_fnpll_1v2_START (13)
#define SOC_HSDT_CRG_FNPLL_STAT0_refdiv_rst_n_test_fnpll_1v2_END (13)
#define SOC_HSDT_CRG_FNPLL_STAT0_ac_buf_pd_START (14)
#define SOC_HSDT_CRG_FNPLL_STAT0_ac_buf_pd_END (14)
#define SOC_HSDT_CRG_FNPLL_STAT0_dc_buf_pd_START (15)
#define SOC_HSDT_CRG_FNPLL_STAT0_dc_buf_pd_END (15)
#define SOC_HSDT_CRG_FNPLL_STAT0_pll_lock_fnpll_1v2_START (16)
#define SOC_HSDT_CRG_FNPLL_STAT0_pll_lock_fnpll_1v2_END (16)
#define SOC_HSDT_CRG_FNPLL_STAT0_pll_lock_ate_fnpll_1v2_START (17)
#define SOC_HSDT_CRG_FNPLL_STAT0_pll_lock_ate_fnpll_1v2_END (17)
#define SOC_HSDT_CRG_FNPLL_STAT0_bbpd_calib_done_fnpll_1v2_START (18)
#define SOC_HSDT_CRG_FNPLL_STAT0_bbpd_calib_done_fnpll_1v2_END (18)
#define SOC_HSDT_CRG_FNPLL_STAT0_bbpd_calib_fail_fnpll_1v2_START (19)
#define SOC_HSDT_CRG_FNPLL_STAT0_bbpd_calib_fail_fnpll_1v2_END (19)
#define SOC_HSDT_CRG_FNPLL_STAT0_bbpd_calib_success_fnpll_1v2_START (20)
#define SOC_HSDT_CRG_FNPLL_STAT0_bbpd_calib_success_fnpll_1v2_END (20)
#define SOC_HSDT_CRG_FNPLL_STAT0_pll_unlock_fnpll_1v2_START (21)
#define SOC_HSDT_CRG_FNPLL_STAT0_pll_unlock_fnpll_1v2_END (21)
#define SOC_HSDT_CRG_FNPLL_STAT0_clock_lost_fnpll_1v2_START (22)
#define SOC_HSDT_CRG_FNPLL_STAT0_clock_lost_fnpll_1v2_END (22)
#define SOC_HSDT_CRG_FNPLL_STAT0_test_data_fnpll_1v2_START (24)
#define SOC_HSDT_CRG_FNPLL_STAT0_test_data_fnpll_1v2_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int reserved_0 : 5;
        unsigned int unlock_count_fnpll_1v2 : 4;
        unsigned int unlock_flag_fnpll_1v2 : 1;
        unsigned int reserved_1 : 22;
    } reg;
} SOC_HSDT_CRG_FNPLL_STAT1_UNION;
#endif
#define SOC_HSDT_CRG_FNPLL_STAT1_unlock_count_fnpll_1v2_START (5)
#define SOC_HSDT_CRG_FNPLL_STAT1_unlock_count_fnpll_1v2_END (8)
#define SOC_HSDT_CRG_FNPLL_STAT1_unlock_flag_fnpll_1v2_START (9)
#define SOC_HSDT_CRG_FNPLL_STAT1_unlock_flag_fnpll_1v2_END (9)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int jm_code_b : 8;
        unsigned int jm_code_a : 8;
        unsigned int jm_code_d : 8;
        unsigned int jm_code_c : 8;
    } reg;
} SOC_HSDT_CRG_JITTER_MONITOR_STAT0_UNION;
#endif
#define SOC_HSDT_CRG_JITTER_MONITOR_STAT0_jm_code_b_START (0)
#define SOC_HSDT_CRG_JITTER_MONITOR_STAT0_jm_code_b_END (7)
#define SOC_HSDT_CRG_JITTER_MONITOR_STAT0_jm_code_a_START (8)
#define SOC_HSDT_CRG_JITTER_MONITOR_STAT0_jm_code_a_END (15)
#define SOC_HSDT_CRG_JITTER_MONITOR_STAT0_jm_code_d_START (16)
#define SOC_HSDT_CRG_JITTER_MONITOR_STAT0_jm_code_d_END (23)
#define SOC_HSDT_CRG_JITTER_MONITOR_STAT0_jm_code_c_START (24)
#define SOC_HSDT_CRG_JITTER_MONITOR_STAT0_jm_code_c_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int jm_dtc_mea_done : 1;
        unsigned int test_comp : 1;
        unsigned int jm_underflow : 1;
        unsigned int jm_overflow : 1;
        unsigned int test_dtc_curr_st : 3;
        unsigned int reserved : 1;
        unsigned int jm_code_e : 8;
        unsigned int jm_code_min : 8;
        unsigned int jm_code_max : 8;
    } reg;
} SOC_HSDT_CRG_JITTER_MONITOR_STAT1_UNION;
#endif
#define SOC_HSDT_CRG_JITTER_MONITOR_STAT1_jm_dtc_mea_done_START (0)
#define SOC_HSDT_CRG_JITTER_MONITOR_STAT1_jm_dtc_mea_done_END (0)
#define SOC_HSDT_CRG_JITTER_MONITOR_STAT1_test_comp_START (1)
#define SOC_HSDT_CRG_JITTER_MONITOR_STAT1_test_comp_END (1)
#define SOC_HSDT_CRG_JITTER_MONITOR_STAT1_jm_underflow_START (2)
#define SOC_HSDT_CRG_JITTER_MONITOR_STAT1_jm_underflow_END (2)
#define SOC_HSDT_CRG_JITTER_MONITOR_STAT1_jm_overflow_START (3)
#define SOC_HSDT_CRG_JITTER_MONITOR_STAT1_jm_overflow_END (3)
#define SOC_HSDT_CRG_JITTER_MONITOR_STAT1_test_dtc_curr_st_START (4)
#define SOC_HSDT_CRG_JITTER_MONITOR_STAT1_test_dtc_curr_st_END (6)
#define SOC_HSDT_CRG_JITTER_MONITOR_STAT1_jm_code_e_START (8)
#define SOC_HSDT_CRG_JITTER_MONITOR_STAT1_jm_code_e_END (15)
#define SOC_HSDT_CRG_JITTER_MONITOR_STAT1_jm_code_min_START (16)
#define SOC_HSDT_CRG_JITTER_MONITOR_STAT1_jm_code_min_END (23)
#define SOC_HSDT_CRG_JITTER_MONITOR_STAT1_jm_code_max_START (24)
#define SOC_HSDT_CRG_JITTER_MONITOR_STAT1_jm_code_max_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sc_jm_clk_en : 1;
        unsigned int reserved_0 : 1;
        unsigned int sc_jm_rst_n : 1;
        unsigned int sc_one2two_cyc : 1;
        unsigned int sc_timing_ctrl_en : 1;
        unsigned int sc_dtc_start : 1;
        unsigned int sc_fdtc_rev : 1;
        unsigned int sc_cdtc_rev : 1;
        unsigned int sc_max2min_cyc : 5;
        unsigned int reserved_1 : 3;
        unsigned int sc_dtc_margin : 10;
        unsigned int reserved_2 : 1;
        unsigned int sc_cdtc_gap : 5;
    } reg;
} SOC_HSDT_CRG_JITTER_MONITOR_CFG0_UNION;
#endif
#define SOC_HSDT_CRG_JITTER_MONITOR_CFG0_sc_jm_clk_en_START (0)
#define SOC_HSDT_CRG_JITTER_MONITOR_CFG0_sc_jm_clk_en_END (0)
#define SOC_HSDT_CRG_JITTER_MONITOR_CFG0_sc_jm_rst_n_START (2)
#define SOC_HSDT_CRG_JITTER_MONITOR_CFG0_sc_jm_rst_n_END (2)
#define SOC_HSDT_CRG_JITTER_MONITOR_CFG0_sc_one2two_cyc_START (3)
#define SOC_HSDT_CRG_JITTER_MONITOR_CFG0_sc_one2two_cyc_END (3)
#define SOC_HSDT_CRG_JITTER_MONITOR_CFG0_sc_timing_ctrl_en_START (4)
#define SOC_HSDT_CRG_JITTER_MONITOR_CFG0_sc_timing_ctrl_en_END (4)
#define SOC_HSDT_CRG_JITTER_MONITOR_CFG0_sc_dtc_start_START (5)
#define SOC_HSDT_CRG_JITTER_MONITOR_CFG0_sc_dtc_start_END (5)
#define SOC_HSDT_CRG_JITTER_MONITOR_CFG0_sc_fdtc_rev_START (6)
#define SOC_HSDT_CRG_JITTER_MONITOR_CFG0_sc_fdtc_rev_END (6)
#define SOC_HSDT_CRG_JITTER_MONITOR_CFG0_sc_cdtc_rev_START (7)
#define SOC_HSDT_CRG_JITTER_MONITOR_CFG0_sc_cdtc_rev_END (7)
#define SOC_HSDT_CRG_JITTER_MONITOR_CFG0_sc_max2min_cyc_START (8)
#define SOC_HSDT_CRG_JITTER_MONITOR_CFG0_sc_max2min_cyc_END (12)
#define SOC_HSDT_CRG_JITTER_MONITOR_CFG0_sc_dtc_margin_START (16)
#define SOC_HSDT_CRG_JITTER_MONITOR_CFG0_sc_dtc_margin_END (25)
#define SOC_HSDT_CRG_JITTER_MONITOR_CFG0_sc_cdtc_gap_START (27)
#define SOC_HSDT_CRG_JITTER_MONITOR_CFG0_sc_cdtc_gap_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sc_fdtc_code_init : 8;
        unsigned int sc_cdtc_code_init : 8;
        unsigned int sc_fdtc_cyc : 5;
        unsigned int reserved_0 : 3;
        unsigned int sc_cdtc_cyc : 5;
        unsigned int reserved_1 : 3;
    } reg;
} SOC_HSDT_CRG_JITTER_MONITOR_CFG1_UNION;
#endif
#define SOC_HSDT_CRG_JITTER_MONITOR_CFG1_sc_fdtc_code_init_START (0)
#define SOC_HSDT_CRG_JITTER_MONITOR_CFG1_sc_fdtc_code_init_END (7)
#define SOC_HSDT_CRG_JITTER_MONITOR_CFG1_sc_cdtc_code_init_START (8)
#define SOC_HSDT_CRG_JITTER_MONITOR_CFG1_sc_cdtc_code_init_END (15)
#define SOC_HSDT_CRG_JITTER_MONITOR_CFG1_sc_fdtc_cyc_START (16)
#define SOC_HSDT_CRG_JITTER_MONITOR_CFG1_sc_fdtc_cyc_END (20)
#define SOC_HSDT_CRG_JITTER_MONITOR_CFG1_sc_cdtc_cyc_START (24)
#define SOC_HSDT_CRG_JITTER_MONITOR_CFG1_sc_cdtc_cyc_END (28)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int sc_fdtc_step : 7;
        unsigned int reserved_0 : 1;
        unsigned int sc_cdtc_step : 6;
        unsigned int reserved_1 : 2;
        unsigned int sc_fdtc_length : 8;
        unsigned int sc_cdtc_length : 8;
    } reg;
} SOC_HSDT_CRG_JITTER_MONITOR_CFG2_UNION;
#endif
#define SOC_HSDT_CRG_JITTER_MONITOR_CFG2_sc_fdtc_step_START (0)
#define SOC_HSDT_CRG_JITTER_MONITOR_CFG2_sc_fdtc_step_END (6)
#define SOC_HSDT_CRG_JITTER_MONITOR_CFG2_sc_cdtc_step_START (8)
#define SOC_HSDT_CRG_JITTER_MONITOR_CFG2_sc_cdtc_step_END (13)
#define SOC_HSDT_CRG_JITTER_MONITOR_CFG2_sc_fdtc_length_START (16)
#define SOC_HSDT_CRG_JITTER_MONITOR_CFG2_sc_fdtc_length_END (23)
#define SOC_HSDT_CRG_JITTER_MONITOR_CFG2_sc_cdtc_length_START (24)
#define SOC_HSDT_CRG_JITTER_MONITOR_CFG2_sc_cdtc_length_END (31)
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
        unsigned int cfg0_vld : 1;
        unsigned int spread_en_fnpll_1v2 : 1;
        unsigned int downspread_fnpll_1v2 : 1;
        unsigned int sel_extwave_fnpll_1v2 : 1;
        unsigned int divval_fnpll_1v2 : 4;
        unsigned int spread_fnpll_1v2 : 3;
        unsigned int freq_threshold : 6;
    } reg;
} SOC_HSDT_CRG_FNPLL_CFG0_UNION;
#endif
#define SOC_HSDT_CRG_FNPLL_CFG0_phe_sel_en_fnpll_1v2_START (0)
#define SOC_HSDT_CRG_FNPLL_CFG0_phe_sel_en_fnpll_1v2_END (0)
#define SOC_HSDT_CRG_FNPLL_CFG0_dtc_test_fnpll_1v2_START (1)
#define SOC_HSDT_CRG_FNPLL_CFG0_dtc_test_fnpll_1v2_END (1)
#define SOC_HSDT_CRG_FNPLL_CFG0_sw_dc_buf_en_START (2)
#define SOC_HSDT_CRG_FNPLL_CFG0_sw_dc_buf_en_END (2)
#define SOC_HSDT_CRG_FNPLL_CFG0_sw_ac_buf_en_START (3)
#define SOC_HSDT_CRG_FNPLL_CFG0_sw_ac_buf_en_END (3)
#define SOC_HSDT_CRG_FNPLL_CFG0_phe_sel_in_fnpll_1v2_START (4)
#define SOC_HSDT_CRG_FNPLL_CFG0_phe_sel_in_fnpll_1v2_END (5)
#define SOC_HSDT_CRG_FNPLL_CFG0_unlock_clear_START (6)
#define SOC_HSDT_CRG_FNPLL_CFG0_unlock_clear_END (6)
#define SOC_HSDT_CRG_FNPLL_CFG0_dc_ac_clk_en_START (7)
#define SOC_HSDT_CRG_FNPLL_CFG0_dc_ac_clk_en_END (7)
#define SOC_HSDT_CRG_FNPLL_CFG0_dtc_m_cfg_fnpll_1v2_START (8)
#define SOC_HSDT_CRG_FNPLL_CFG0_dtc_m_cfg_fnpll_1v2_END (13)
#define SOC_HSDT_CRG_FNPLL_CFG0_ref_lost_clear_fnpll_1v2_START (14)
#define SOC_HSDT_CRG_FNPLL_CFG0_ref_lost_clear_fnpll_1v2_END (14)
#define SOC_HSDT_CRG_FNPLL_CFG0_cfg0_vld_START (15)
#define SOC_HSDT_CRG_FNPLL_CFG0_cfg0_vld_END (15)
#define SOC_HSDT_CRG_FNPLL_CFG0_spread_en_fnpll_1v2_START (16)
#define SOC_HSDT_CRG_FNPLL_CFG0_spread_en_fnpll_1v2_END (16)
#define SOC_HSDT_CRG_FNPLL_CFG0_downspread_fnpll_1v2_START (17)
#define SOC_HSDT_CRG_FNPLL_CFG0_downspread_fnpll_1v2_END (17)
#define SOC_HSDT_CRG_FNPLL_CFG0_sel_extwave_fnpll_1v2_START (18)
#define SOC_HSDT_CRG_FNPLL_CFG0_sel_extwave_fnpll_1v2_END (18)
#define SOC_HSDT_CRG_FNPLL_CFG0_divval_fnpll_1v2_START (19)
#define SOC_HSDT_CRG_FNPLL_CFG0_divval_fnpll_1v2_END (22)
#define SOC_HSDT_CRG_FNPLL_CFG0_spread_fnpll_1v2_START (23)
#define SOC_HSDT_CRG_FNPLL_CFG0_spread_fnpll_1v2_END (25)
#define SOC_HSDT_CRG_FNPLL_CFG0_freq_threshold_START (26)
#define SOC_HSDT_CRG_FNPLL_CFG0_freq_threshold_END (31)
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
} SOC_HSDT_CRG_FNPLL_CFG1_UNION;
#endif
#define SOC_HSDT_CRG_FNPLL_CFG1_ext_maxaddr_fnpll_1v2_START (0)
#define SOC_HSDT_CRG_FNPLL_CFG1_ext_maxaddr_fnpll_1v2_END (7)
#define SOC_HSDT_CRG_FNPLL_CFG1_extwaveval_fnpll_1v2_START (8)
#define SOC_HSDT_CRG_FNPLL_CFG1_extwaveval_fnpll_1v2_END (15)
#define SOC_HSDT_CRG_FNPLL_CFG1_input_cfg_en_fnpll_1v2_START (16)
#define SOC_HSDT_CRG_FNPLL_CFG1_input_cfg_en_fnpll_1v2_END (16)
#define SOC_HSDT_CRG_FNPLL_CFG1_test_mode_fnpll_1v2_START (17)
#define SOC_HSDT_CRG_FNPLL_CFG1_test_mode_fnpll_1v2_END (17)
#define SOC_HSDT_CRG_FNPLL_CFG1_lock_thr_fnpll_1v2_START (18)
#define SOC_HSDT_CRG_FNPLL_CFG1_lock_thr_fnpll_1v2_END (19)
#define SOC_HSDT_CRG_FNPLL_CFG1_refdiv_rst_n_fnpll_1v2_START (20)
#define SOC_HSDT_CRG_FNPLL_CFG1_refdiv_rst_n_fnpll_1v2_END (20)
#define SOC_HSDT_CRG_FNPLL_CFG1_fbdiv_rst_n_fnpll_1v2_START (21)
#define SOC_HSDT_CRG_FNPLL_CFG1_fbdiv_rst_n_fnpll_1v2_END (21)
#define SOC_HSDT_CRG_FNPLL_CFG1_div2_pd_fnpll_1v2_START (22)
#define SOC_HSDT_CRG_FNPLL_CFG1_div2_pd_fnpll_1v2_END (22)
#define SOC_HSDT_CRG_FNPLL_CFG1_lock_en_fnpll_1v2_START (23)
#define SOC_HSDT_CRG_FNPLL_CFG1_lock_en_fnpll_1v2_END (23)
#define SOC_HSDT_CRG_FNPLL_CFG1_fbdiv_delay_num_fnpll_1v2_START (24)
#define SOC_HSDT_CRG_FNPLL_CFG1_fbdiv_delay_num_fnpll_1v2_END (30)
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
} SOC_HSDT_CRG_FNPLL_CFG2_UNION;
#endif
#define SOC_HSDT_CRG_FNPLL_CFG2_bbpd_calib_byp_fnpll_1v2_START (0)
#define SOC_HSDT_CRG_FNPLL_CFG2_bbpd_calib_byp_fnpll_1v2_END (0)
#define SOC_HSDT_CRG_FNPLL_CFG2_dtc_ctrl_inv_fnpll_1v2_START (1)
#define SOC_HSDT_CRG_FNPLL_CFG2_dtc_ctrl_inv_fnpll_1v2_END (1)
#define SOC_HSDT_CRG_FNPLL_CFG2_dll_force_en_fnpll_1v2_START (2)
#define SOC_HSDT_CRG_FNPLL_CFG2_dll_force_en_fnpll_1v2_END (2)
#define SOC_HSDT_CRG_FNPLL_CFG2_DLL_EN_FNPLL_1V2_START (3)
#define SOC_HSDT_CRG_FNPLL_CFG2_DLL_EN_FNPLL_1V2_END (3)
#define SOC_HSDT_CRG_FNPLL_CFG2_phe_code_a_fnpll_1v2_START (4)
#define SOC_HSDT_CRG_FNPLL_CFG2_phe_code_a_fnpll_1v2_END (5)
#define SOC_HSDT_CRG_FNPLL_CFG2_phe_code_b_fnpll_1v2_START (6)
#define SOC_HSDT_CRG_FNPLL_CFG2_phe_code_b_fnpll_1v2_END (7)
#define SOC_HSDT_CRG_FNPLL_CFG2_ctinue_lock_num_fnpll_1v2_START (8)
#define SOC_HSDT_CRG_FNPLL_CFG2_ctinue_lock_num_fnpll_1v2_END (13)
#define SOC_HSDT_CRG_FNPLL_CFG2_read_en_fnpll_1v2_START (16)
#define SOC_HSDT_CRG_FNPLL_CFG2_read_en_fnpll_1v2_END (16)
#define SOC_HSDT_CRG_FNPLL_CFG2_k_gain_cfg_en_fnpll_1v2_START (17)
#define SOC_HSDT_CRG_FNPLL_CFG2_k_gain_cfg_en_fnpll_1v2_END (17)
#define SOC_HSDT_CRG_FNPLL_CFG2_k_gain_av_thr_fnpll_1v2_START (20)
#define SOC_HSDT_CRG_FNPLL_CFG2_k_gain_av_thr_fnpll_1v2_END (22)
#define SOC_HSDT_CRG_FNPLL_CFG2_pll_unlock_clr_fnpll_1v2_START (23)
#define SOC_HSDT_CRG_FNPLL_CFG2_pll_unlock_clr_fnpll_1v2_END (23)
#define SOC_HSDT_CRG_FNPLL_CFG2_k_gain_cfg_fnpll_1v2_START (24)
#define SOC_HSDT_CRG_FNPLL_CFG2_k_gain_cfg_fnpll_1v2_END (29)
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
} SOC_HSDT_CRG_FNPLL_CFG3_UNION;
#endif
#define SOC_HSDT_CRG_FNPLL_CFG3_dtc_o_cfg_fnpll_1v2_START (0)
#define SOC_HSDT_CRG_FNPLL_CFG3_dtc_o_cfg_fnpll_1v2_END (5)
#define SOC_HSDT_CRG_FNPLL_CFG3_bbpd_lock_num_fnpll_1v2_START (8)
#define SOC_HSDT_CRG_FNPLL_CFG3_bbpd_lock_num_fnpll_1v2_END (15)
#define SOC_HSDT_CRG_FNPLL_CFG3_lpf_bw_fnpll_1v2_START (16)
#define SOC_HSDT_CRG_FNPLL_CFG3_lpf_bw_fnpll_1v2_END (18)
#define SOC_HSDT_CRG_FNPLL_CFG3_lpf_bw1_fnpll_1v2_START (20)
#define SOC_HSDT_CRG_FNPLL_CFG3_lpf_bw1_fnpll_1v2_END (22)
#define SOC_HSDT_CRG_FNPLL_CFG3_calib_lpf_bw_fnpll_1v2_START (24)
#define SOC_HSDT_CRG_FNPLL_CFG3_calib_lpf_bw_fnpll_1v2_END (26)
#define SOC_HSDT_CRG_FNPLL_CFG3_calib_lpf_bw1_fnpll_1v2_START (28)
#define SOC_HSDT_CRG_FNPLL_CFG3_calib_lpf_bw1_fnpll_1v2_END (30)
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
} SOC_HSDT_CRG_FNPLL_CFG4_UNION;
#endif
#define SOC_HSDT_CRG_FNPLL_CFG4_max_k_gain_fnpll_1v2_START (0)
#define SOC_HSDT_CRG_FNPLL_CFG4_max_k_gain_fnpll_1v2_END (5)
#define SOC_HSDT_CRG_FNPLL_CFG4_min_k_gain_fnpll_1v2_START (8)
#define SOC_HSDT_CRG_FNPLL_CFG4_min_k_gain_fnpll_1v2_END (13)
#define SOC_HSDT_CRG_FNPLL_CFG4_en_dac_test_fnpll_1v2_START (16)
#define SOC_HSDT_CRG_FNPLL_CFG4_en_dac_test_fnpll_1v2_END (16)
#define SOC_HSDT_CRG_FNPLL_CFG4_updn_sel_fnpll_1v2_START (17)
#define SOC_HSDT_CRG_FNPLL_CFG4_updn_sel_fnpll_1v2_END (17)
#define SOC_HSDT_CRG_FNPLL_CFG4_icp_ctrl_fnpll_1v2_START (18)
#define SOC_HSDT_CRG_FNPLL_CFG4_icp_ctrl_fnpll_1v2_END (18)
#define SOC_HSDT_CRG_FNPLL_CFG4_vdc_sel_fnpll_1v2_START (20)
#define SOC_HSDT_CRG_FNPLL_CFG4_vdc_sel_fnpll_1v2_END (23)
#define SOC_HSDT_CRG_FNPLL_CFG4_test_data_sel_fnpll_1v2_START (28)
#define SOC_HSDT_CRG_FNPLL_CFG4_test_data_sel_fnpll_1v2_END (31)
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
} SOC_HSDT_CRG_FNPLL_CFG5_UNION;
#endif
#define SOC_HSDT_CRG_FNPLL_CFG5_ref_lost_thr_fnpll_1v2_START (0)
#define SOC_HSDT_CRG_FNPLL_CFG5_ref_lost_thr_fnpll_1v2_END (7)
#define SOC_HSDT_CRG_FNPLL_CFG5_pfd_div_ratio_START (8)
#define SOC_HSDT_CRG_FNPLL_CFG5_pfd_div_ratio_END (11)
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
} SOC_HSDT_CRG_FNPLL_CFG6_UNION;
#endif
#define SOC_HSDT_CRG_FNPLL_CFG6_fnpll_en_START (0)
#define SOC_HSDT_CRG_FNPLL_CFG6_fnpll_en_END (0)
#define SOC_HSDT_CRG_FNPLL_CFG6_BYPASS_FNPLL_1V2_START (1)
#define SOC_HSDT_CRG_FNPLL_CFG6_BYPASS_FNPLL_1V2_END (1)
#define SOC_HSDT_CRG_FNPLL_CFG6_PLL_MODE_FNPLL_1V2_START (2)
#define SOC_HSDT_CRG_FNPLL_CFG6_PLL_MODE_FNPLL_1V2_END (2)
#define SOC_HSDT_CRG_FNPLL_CFG6_REFDIV_FNPLL_1V2_START (4)
#define SOC_HSDT_CRG_FNPLL_CFG6_REFDIV_FNPLL_1V2_END (9)
#define SOC_HSDT_CRG_FNPLL_CFG6_POSTDIV1_FNPLL_1V2_START (12)
#define SOC_HSDT_CRG_FNPLL_CFG6_POSTDIV1_FNPLL_1V2_END (14)
#define SOC_HSDT_CRG_FNPLL_CFG6_FBDIV_FNPLL_1V2_START (16)
#define SOC_HSDT_CRG_FNPLL_CFG6_FBDIV_FNPLL_1V2_END (27)
#define SOC_HSDT_CRG_FNPLL_CFG6_POSTDIV2_FNPLL_1V2_START (28)
#define SOC_HSDT_CRG_FNPLL_CFG6_POSTDIV2_FNPLL_1V2_END (30)
#define SOC_HSDT_CRG_FNPLL_CFG6_fbdiv_cfg_vld_START (31)
#define SOC_HSDT_CRG_FNPLL_CFG6_fbdiv_cfg_vld_END (31)
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
} SOC_HSDT_CRG_FNPLL_CFG7_UNION;
#endif
#define SOC_HSDT_CRG_FNPLL_CFG7_FRAC_FNPLL_1V2_START (0)
#define SOC_HSDT_CRG_FNPLL_CFG7_FRAC_FNPLL_1V2_END (23)
#define SOC_HSDT_CRG_FNPLL_CFG7_FOUTPOSTDIVPD_FNPLL_1V2_START (24)
#define SOC_HSDT_CRG_FNPLL_CFG7_FOUTPOSTDIVPD_FNPLL_1V2_END (24)
#define SOC_HSDT_CRG_FNPLL_CFG7_FOUT4PHASEPD_FNPLL_1V2_START (25)
#define SOC_HSDT_CRG_FNPLL_CFG7_FOUT4PHASEPD_FNPLL_1V2_END (25)
#define SOC_HSDT_CRG_FNPLL_CFG7_FOUT2XPD_FNPLL_1V2_START (26)
#define SOC_HSDT_CRG_FNPLL_CFG7_FOUT2XPD_FNPLL_1V2_END (26)
#define SOC_HSDT_CRG_FNPLL_CFG7_frac_cfg_vld_START (31)
#define SOC_HSDT_CRG_FNPLL_CFG7_frac_cfg_vld_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_pcieio_hard_bypass : 1;
        unsigned int gt_clk_pciephy_ref_hard_bypass : 1;
        unsigned int pciephy_debounce_waitcfg_out_h : 2;
        unsigned int pciephy_debounce_waitcfg_in_h : 2;
        unsigned int pcieio_oe_en_soft : 1;
        unsigned int pcie_oe_gt_mode : 2;
        unsigned int pcieio_oe_polar : 1;
        unsigned int pcieio_hard_ctrl_debounce_bypass : 1;
        unsigned int pcieio_oe_en_hard_bypass : 1;
        unsigned int pciephy_debounce_waitcfg_out : 8;
        unsigned int pciephy_debounce_waitcfg_in : 7;
        unsigned int pcieio_ie_en_hard_bypass : 1;
        unsigned int pcieio_ie_en_soft : 1;
        unsigned int pcieio_ie_polar : 1;
        unsigned int pcie_mplla_force_en_sel : 1;
        unsigned int reserved : 1;
    } reg;
} SOC_HSDT_CRG_PCIECTRL0_UNION;
#endif
#define SOC_HSDT_CRG_PCIECTRL0_gt_clk_pcieio_hard_bypass_START (0)
#define SOC_HSDT_CRG_PCIECTRL0_gt_clk_pcieio_hard_bypass_END (0)
#define SOC_HSDT_CRG_PCIECTRL0_gt_clk_pciephy_ref_hard_bypass_START (1)
#define SOC_HSDT_CRG_PCIECTRL0_gt_clk_pciephy_ref_hard_bypass_END (1)
#define SOC_HSDT_CRG_PCIECTRL0_pciephy_debounce_waitcfg_out_h_START (2)
#define SOC_HSDT_CRG_PCIECTRL0_pciephy_debounce_waitcfg_out_h_END (3)
#define SOC_HSDT_CRG_PCIECTRL0_pciephy_debounce_waitcfg_in_h_START (4)
#define SOC_HSDT_CRG_PCIECTRL0_pciephy_debounce_waitcfg_in_h_END (5)
#define SOC_HSDT_CRG_PCIECTRL0_pcieio_oe_en_soft_START (6)
#define SOC_HSDT_CRG_PCIECTRL0_pcieio_oe_en_soft_END (6)
#define SOC_HSDT_CRG_PCIECTRL0_pcie_oe_gt_mode_START (7)
#define SOC_HSDT_CRG_PCIECTRL0_pcie_oe_gt_mode_END (8)
#define SOC_HSDT_CRG_PCIECTRL0_pcieio_oe_polar_START (9)
#define SOC_HSDT_CRG_PCIECTRL0_pcieio_oe_polar_END (9)
#define SOC_HSDT_CRG_PCIECTRL0_pcieio_hard_ctrl_debounce_bypass_START (10)
#define SOC_HSDT_CRG_PCIECTRL0_pcieio_hard_ctrl_debounce_bypass_END (10)
#define SOC_HSDT_CRG_PCIECTRL0_pcieio_oe_en_hard_bypass_START (11)
#define SOC_HSDT_CRG_PCIECTRL0_pcieio_oe_en_hard_bypass_END (11)
#define SOC_HSDT_CRG_PCIECTRL0_pciephy_debounce_waitcfg_out_START (12)
#define SOC_HSDT_CRG_PCIECTRL0_pciephy_debounce_waitcfg_out_END (19)
#define SOC_HSDT_CRG_PCIECTRL0_pciephy_debounce_waitcfg_in_START (20)
#define SOC_HSDT_CRG_PCIECTRL0_pciephy_debounce_waitcfg_in_END (26)
#define SOC_HSDT_CRG_PCIECTRL0_pcieio_ie_en_hard_bypass_START (27)
#define SOC_HSDT_CRG_PCIECTRL0_pcieio_ie_en_hard_bypass_END (27)
#define SOC_HSDT_CRG_PCIECTRL0_pcieio_ie_en_soft_START (28)
#define SOC_HSDT_CRG_PCIECTRL0_pcieio_ie_en_soft_END (28)
#define SOC_HSDT_CRG_PCIECTRL0_pcieio_ie_polar_START (29)
#define SOC_HSDT_CRG_PCIECTRL0_pcieio_ie_polar_END (29)
#define SOC_HSDT_CRG_PCIECTRL0_pcie_mplla_force_en_sel_START (30)
#define SOC_HSDT_CRG_PCIECTRL0_pcie_mplla_force_en_sel_END (30)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int pciephy_clkrst_bypass : 1;
        unsigned int pciectrl_clkrst_bypass : 1;
        unsigned int pcietrace_clkrst_bypass : 1;
        unsigned int hsdttcu_clkrst_bypass : 1;
        unsigned int hsdttbu_clkrst_bypass : 1;
        unsigned int reserved_0 : 1;
        unsigned int hsdt_sysctrl_clkrst_bypass : 1;
        unsigned int reserved_1 : 1;
        unsigned int hsdt_ioc_clkrst_bypass : 1;
        unsigned int reserved_2 : 1;
        unsigned int reserved_3 : 22;
    } reg;
} SOC_HSDT_CRG_IPCLKRST_BYPASS0_UNION;
#endif
#define SOC_HSDT_CRG_IPCLKRST_BYPASS0_pciephy_clkrst_bypass_START (0)
#define SOC_HSDT_CRG_IPCLKRST_BYPASS0_pciephy_clkrst_bypass_END (0)
#define SOC_HSDT_CRG_IPCLKRST_BYPASS0_pciectrl_clkrst_bypass_START (1)
#define SOC_HSDT_CRG_IPCLKRST_BYPASS0_pciectrl_clkrst_bypass_END (1)
#define SOC_HSDT_CRG_IPCLKRST_BYPASS0_pcietrace_clkrst_bypass_START (2)
#define SOC_HSDT_CRG_IPCLKRST_BYPASS0_pcietrace_clkrst_bypass_END (2)
#define SOC_HSDT_CRG_IPCLKRST_BYPASS0_hsdttcu_clkrst_bypass_START (3)
#define SOC_HSDT_CRG_IPCLKRST_BYPASS0_hsdttcu_clkrst_bypass_END (3)
#define SOC_HSDT_CRG_IPCLKRST_BYPASS0_hsdttbu_clkrst_bypass_START (4)
#define SOC_HSDT_CRG_IPCLKRST_BYPASS0_hsdttbu_clkrst_bypass_END (4)
#define SOC_HSDT_CRG_IPCLKRST_BYPASS0_hsdt_sysctrl_clkrst_bypass_START (6)
#define SOC_HSDT_CRG_IPCLKRST_BYPASS0_hsdt_sysctrl_clkrst_bypass_END (6)
#define SOC_HSDT_CRG_IPCLKRST_BYPASS0_hsdt_ioc_clkrst_bypass_START (8)
#define SOC_HSDT_CRG_IPCLKRST_BYPASS0_hsdt_ioc_clkrst_bypass_END (8)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int autodiv_hsdtip_stat : 1;
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
        unsigned int reserved_17 : 14;
    } reg;
} SOC_HSDT_CRG_PERI_AUTODIV_INUSE_STAT_UNION;
#endif
#define SOC_HSDT_CRG_PERI_AUTODIV_INUSE_STAT_autodiv_hsdtip_stat_START (0)
#define SOC_HSDT_CRG_PERI_AUTODIV_INUSE_STAT_autodiv_hsdtip_stat_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hsdtip_div_auto_reduce_bypass : 1;
        unsigned int hsdtip_auto_waitcfg_in : 10;
        unsigned int hsdtip_auto_waitcfg_out : 10;
        unsigned int hsdtip_div_auto_cfg : 4;
        unsigned int reserved : 2;
        unsigned int hsdtip_pcie0_bypass : 1;
        unsigned int hsdtip_trace_bypass : 1;
        unsigned int hsdtip_sec_bypass : 1;
        unsigned int hsdtip_ufs_bypass : 1;
        unsigned int hsdtip_pcie0_tcu_bypass : 1;
    } reg;
} SOC_HSDT_CRG_PERI_AUTODIV0_UNION;
#endif
#define SOC_HSDT_CRG_PERI_AUTODIV0_hsdtip_div_auto_reduce_bypass_START (0)
#define SOC_HSDT_CRG_PERI_AUTODIV0_hsdtip_div_auto_reduce_bypass_END (0)
#define SOC_HSDT_CRG_PERI_AUTODIV0_hsdtip_auto_waitcfg_in_START (1)
#define SOC_HSDT_CRG_PERI_AUTODIV0_hsdtip_auto_waitcfg_in_END (10)
#define SOC_HSDT_CRG_PERI_AUTODIV0_hsdtip_auto_waitcfg_out_START (11)
#define SOC_HSDT_CRG_PERI_AUTODIV0_hsdtip_auto_waitcfg_out_END (20)
#define SOC_HSDT_CRG_PERI_AUTODIV0_hsdtip_div_auto_cfg_START (21)
#define SOC_HSDT_CRG_PERI_AUTODIV0_hsdtip_div_auto_cfg_END (24)
#define SOC_HSDT_CRG_PERI_AUTODIV0_hsdtip_pcie0_bypass_START (27)
#define SOC_HSDT_CRG_PERI_AUTODIV0_hsdtip_pcie0_bypass_END (27)
#define SOC_HSDT_CRG_PERI_AUTODIV0_hsdtip_trace_bypass_START (28)
#define SOC_HSDT_CRG_PERI_AUTODIV0_hsdtip_trace_bypass_END (28)
#define SOC_HSDT_CRG_PERI_AUTODIV0_hsdtip_sec_bypass_START (29)
#define SOC_HSDT_CRG_PERI_AUTODIV0_hsdtip_sec_bypass_END (29)
#define SOC_HSDT_CRG_PERI_AUTODIV0_hsdtip_ufs_bypass_START (30)
#define SOC_HSDT_CRG_PERI_AUTODIV0_hsdtip_ufs_bypass_END (30)
#define SOC_HSDT_CRG_PERI_AUTODIV0_hsdtip_pcie0_tcu_bypass_START (31)
#define SOC_HSDT_CRG_PERI_AUTODIV0_hsdtip_pcie0_tcu_bypass_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int hsdtip_apb_cfg_bypass : 1;
        unsigned int hsdtip_pcie0_cfg_bypass : 1;
        unsigned int hsdtip_ahb_cfg_bypass : 1;
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
    } reg;
} SOC_HSDT_CRG_PERI_AUTODIV1_UNION;
#endif
#define SOC_HSDT_CRG_PERI_AUTODIV1_hsdtip_apb_cfg_bypass_START (0)
#define SOC_HSDT_CRG_PERI_AUTODIV1_hsdtip_apb_cfg_bypass_END (0)
#define SOC_HSDT_CRG_PERI_AUTODIV1_hsdtip_pcie0_cfg_bypass_START (1)
#define SOC_HSDT_CRG_PERI_AUTODIV1_hsdtip_pcie0_cfg_bypass_END (1)
#define SOC_HSDT_CRG_PERI_AUTODIV1_hsdtip_ahb_cfg_bypass_START (2)
#define SOC_HSDT_CRG_PERI_AUTODIV1_hsdtip_ahb_cfg_bypass_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_ao_cc : 1;
        unsigned int gt_clk_cc712 : 1;
        unsigned int gt_clk_sec_pre_random : 1;
        unsigned int reserved : 29;
    } reg;
} SOC_HSDT_CRG_PEREN_CC712_SEC_UNION;
#endif
#define SOC_HSDT_CRG_PEREN_CC712_SEC_gt_clk_ao_cc_START (0)
#define SOC_HSDT_CRG_PEREN_CC712_SEC_gt_clk_ao_cc_END (0)
#define SOC_HSDT_CRG_PEREN_CC712_SEC_gt_clk_cc712_START (1)
#define SOC_HSDT_CRG_PEREN_CC712_SEC_gt_clk_cc712_END (1)
#define SOC_HSDT_CRG_PEREN_CC712_SEC_gt_clk_sec_pre_random_START (2)
#define SOC_HSDT_CRG_PEREN_CC712_SEC_gt_clk_sec_pre_random_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_ao_cc : 1;
        unsigned int gt_clk_cc712 : 1;
        unsigned int gt_clk_sec_pre_random : 1;
        unsigned int reserved : 29;
    } reg;
} SOC_HSDT_CRG_PERDIS_CC712_SEC_UNION;
#endif
#define SOC_HSDT_CRG_PERDIS_CC712_SEC_gt_clk_ao_cc_START (0)
#define SOC_HSDT_CRG_PERDIS_CC712_SEC_gt_clk_ao_cc_END (0)
#define SOC_HSDT_CRG_PERDIS_CC712_SEC_gt_clk_cc712_START (1)
#define SOC_HSDT_CRG_PERDIS_CC712_SEC_gt_clk_cc712_END (1)
#define SOC_HSDT_CRG_PERDIS_CC712_SEC_gt_clk_sec_pre_random_START (2)
#define SOC_HSDT_CRG_PERDIS_CC712_SEC_gt_clk_sec_pre_random_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int gt_clk_ao_cc : 1;
        unsigned int gt_clk_cc712 : 1;
        unsigned int gt_clk_sec_pre_random : 1;
        unsigned int reserved : 29;
    } reg;
} SOC_HSDT_CRG_PERCLKEN_CC712_SEC_UNION;
#endif
#define SOC_HSDT_CRG_PERCLKEN_CC712_SEC_gt_clk_ao_cc_START (0)
#define SOC_HSDT_CRG_PERCLKEN_CC712_SEC_gt_clk_ao_cc_END (0)
#define SOC_HSDT_CRG_PERCLKEN_CC712_SEC_gt_clk_cc712_START (1)
#define SOC_HSDT_CRG_PERCLKEN_CC712_SEC_gt_clk_cc712_END (1)
#define SOC_HSDT_CRG_PERCLKEN_CC712_SEC_gt_clk_sec_pre_random_START (2)
#define SOC_HSDT_CRG_PERCLKEN_CC712_SEC_gt_clk_sec_pre_random_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int st_clk_ao_cc : 1;
        unsigned int st_clk_cc712 : 1;
        unsigned int st_clk_sec_pre_random : 1;
        unsigned int reserved : 29;
    } reg;
} SOC_HSDT_CRG_PERSTAT_CC712_SEC_UNION;
#endif
#define SOC_HSDT_CRG_PERSTAT_CC712_SEC_st_clk_ao_cc_START (0)
#define SOC_HSDT_CRG_PERSTAT_CC712_SEC_st_clk_ao_cc_END (0)
#define SOC_HSDT_CRG_PERSTAT_CC712_SEC_st_clk_cc712_START (1)
#define SOC_HSDT_CRG_PERSTAT_CC712_SEC_st_clk_cc712_END (1)
#define SOC_HSDT_CRG_PERSTAT_CC712_SEC_st_clk_sec_pre_random_START (2)
#define SOC_HSDT_CRG_PERSTAT_CC712_SEC_st_clk_sec_pre_random_END (2)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int intr_mask_pcie0_nonidle_pend : 1;
        unsigned int intr_mask_ufs_nonidle_pend : 1;
        unsigned int reserved : 14;
        unsigned int bitmasken : 16;
    } reg;
} SOC_HSDT_CRG_INTR_MASK_NOCBUS_NONIDLE_PEND_UNION;
#endif
#define SOC_HSDT_CRG_INTR_MASK_NOCBUS_NONIDLE_PEND_intr_mask_pcie0_nonidle_pend_START (0)
#define SOC_HSDT_CRG_INTR_MASK_NOCBUS_NONIDLE_PEND_intr_mask_pcie0_nonidle_pend_END (0)
#define SOC_HSDT_CRG_INTR_MASK_NOCBUS_NONIDLE_PEND_intr_mask_ufs_nonidle_pend_START (1)
#define SOC_HSDT_CRG_INTR_MASK_NOCBUS_NONIDLE_PEND_intr_mask_ufs_nonidle_pend_END (1)
#define SOC_HSDT_CRG_INTR_MASK_NOCBUS_NONIDLE_PEND_bitmasken_START (16)
#define SOC_HSDT_CRG_INTR_MASK_NOCBUS_NONIDLE_PEND_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int intr_clr_pcie0_nonidle_pend : 1;
        unsigned int intr_clr_ufs_nonidle_pend : 1;
        unsigned int reserved : 14;
        unsigned int bitmasken : 16;
    } reg;
} SOC_HSDT_CRG_INTR_CLR_NOCBUS_NONIDLE_PEND_UNION;
#endif
#define SOC_HSDT_CRG_INTR_CLR_NOCBUS_NONIDLE_PEND_intr_clr_pcie0_nonidle_pend_START (0)
#define SOC_HSDT_CRG_INTR_CLR_NOCBUS_NONIDLE_PEND_intr_clr_pcie0_nonidle_pend_END (0)
#define SOC_HSDT_CRG_INTR_CLR_NOCBUS_NONIDLE_PEND_intr_clr_ufs_nonidle_pend_START (1)
#define SOC_HSDT_CRG_INTR_CLR_NOCBUS_NONIDLE_PEND_intr_clr_ufs_nonidle_pend_END (1)
#define SOC_HSDT_CRG_INTR_CLR_NOCBUS_NONIDLE_PEND_bitmasken_START (16)
#define SOC_HSDT_CRG_INTR_CLR_NOCBUS_NONIDLE_PEND_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int intr_stat_pcie0_nonidle_pend : 1;
        unsigned int intr_stat_ufs_nonidle_pend : 1;
        unsigned int reserved : 30;
    } reg;
} SOC_HSDT_CRG_INTR_STAT_NOCBUS_NONIDLE_PEND_UNION;
#endif
#define SOC_HSDT_CRG_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_pcie0_nonidle_pend_START (0)
#define SOC_HSDT_CRG_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_pcie0_nonidle_pend_END (0)
#define SOC_HSDT_CRG_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_ufs_nonidle_pend_START (1)
#define SOC_HSDT_CRG_INTR_STAT_NOCBUS_NONIDLE_PEND_intr_stat_ufs_nonidle_pend_END (1)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_rst_cc712 : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_HSDT_CRG_PERRSTEN_CC712_SEC_UNION;
#endif
#define SOC_HSDT_CRG_PERRSTEN_CC712_SEC_ip_rst_cc712_START (0)
#define SOC_HSDT_CRG_PERRSTEN_CC712_SEC_ip_rst_cc712_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_rst_cc712 : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_HSDT_CRG_PERRSTDIS_CC712_SEC_UNION;
#endif
#define SOC_HSDT_CRG_PERRSTDIS_CC712_SEC_ip_rst_cc712_START (0)
#define SOC_HSDT_CRG_PERRSTDIS_CC712_SEC_ip_rst_cc712_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int ip_rst_cc712 : 1;
        unsigned int reserved : 31;
    } reg;
} SOC_HSDT_CRG_PERRSTSTAT_CC712_SEC_UNION;
#endif
#define SOC_HSDT_CRG_PERRSTSTAT_CC712_SEC_ip_rst_cc712_START (0)
#define SOC_HSDT_CRG_PERRSTSTAT_CC712_SEC_ip_rst_cc712_END (0)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int div_sec : 5;
        unsigned int sc_gt_clk_sec : 1;
        unsigned int reserved_0 : 3;
        unsigned int reserved_1 : 4;
        unsigned int reserved_2 : 3;
        unsigned int bitmasken : 16;
    } reg;
} SOC_HSDT_CRG_CLKDIV_CC712_SEC_UNION;
#endif
#define SOC_HSDT_CRG_CLKDIV_CC712_SEC_div_sec_START (0)
#define SOC_HSDT_CRG_CLKDIV_CC712_SEC_div_sec_END (4)
#define SOC_HSDT_CRG_CLKDIV_CC712_SEC_sc_gt_clk_sec_START (5)
#define SOC_HSDT_CRG_CLKDIV_CC712_SEC_sc_gt_clk_sec_END (5)
#define SOC_HSDT_CRG_CLKDIV_CC712_SEC_bitmasken_START (16)
#define SOC_HSDT_CRG_CLKDIV_CC712_SEC_bitmasken_END (31)
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int value;
    struct
    {
        unsigned int cc712_clkrst_bypass : 1;
        unsigned int reserved_0 : 1;
        unsigned int reserved_1 : 1;
        unsigned int reserved_2 : 29;
    } reg;
} SOC_HSDT_CRG_PERI_CTRL_CC712_SEC_UNION;
#endif
#define SOC_HSDT_CRG_PERI_CTRL_CC712_SEC_cc712_clkrst_bypass_START (0)
#define SOC_HSDT_CRG_PERI_CTRL_CC712_SEC_cc712_clkrst_bypass_END (0)
#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
#endif
