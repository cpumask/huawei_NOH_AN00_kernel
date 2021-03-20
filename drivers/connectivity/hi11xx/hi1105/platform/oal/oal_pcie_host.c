

#ifdef _PRE_PLAT_FEATURE_HI110X_PCIE
#define HI11XX_LOG_MODULE_NAME "[PCIE_H]"
#define HISI_LOG_TAG           "[PCIE]"
#include "oal_util.h"

#include "oal_net.h"
#include "oal_ext_if.h"

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/err.h>
#include <linux/workqueue.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/kthread.h>

#include "oal_thread.h"
#include "oam_ext_if.h"

#include "oal_pcie_host.h"
#include "oal_pcie_linux.h"
#include "oal_pcie_reg.h"
#include "oal_pci_if.h"

#include "oal_pcie_comm.h"
#include "oal_hcc_host_if.h"
#include "oal_kernel_file.h"

#include "oal_pcie_pm.h"
#include "plat_firmware.h"
#include "plat_pm_wlan.h"
#include "board.h"
#include "securec.h"
#include "oal_util.h"
#include "plat_pm.h"

#ifdef _PRE_WLAN_PKT_TIME_STAT
#include <hwnet/ipv4/wifi_delayst.h>
#endif
#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_OAL_PCIE_HOST_C

#define PCIE_MEM_SCAN_VERFIY_VAL_1 0x5a
#define PCIE_MEM_SCAN_VERFIY_VAL_2 0xa5

/* tcxo->640M */
#define HI1103_PA_GLB_CTL_BASE_ADDR 0x50000000
#define HI1103_PA_PMU_CMU_CTL_BASE  0x50002000
#define HI1103_PA_PMU2_CMU_IR_BASE  0x50003000
#define HI1103_PA_W_CTL_BASE        0x40000000

#define HI1103_GLB_CTL_BASE_ADDR pst_glb_ctrl

#define HI1103_PMU_CMU_BASE pst_pmu_cmu_ctrl

#define HI1103_PMU2_CMU_IR_BASE pst_pmu2_cmu_ir_ctrl

#define HI1103_PMU2_CMU_IR_PMU_LNALDO0_EN_DELAY_STS_OFFSET 1
#define HI1103_PMU2_CMU_IR_PMU_LNALDO0_EN_STS_OFFSET       0
#define HI1103_PMU2_CMU_IR_PMU_LNALDO0_EN_DELAY_STS_MSK ((0x1UL) << HI1103_PMU2_CMU_IR_PMU_LNALDO0_EN_DELAY_STS_OFFSET)
#define HI1103_PMU2_CMU_IR_PMU_LNALDO0_EN_STS_MSK       ((0x1UL) << HI1103_PMU2_CMU_IR_PMU_LNALDO0_EN_STS_OFFSET)

#define HI1103_PMU2_CMU_IR_FBDIV_FRAC_SLP_STS_LEN    1
#define HI1103_PMU2_CMU_IR_FBDIV_FRAC_SLP_STS_OFFSET 0
#define HI1103_PMU2_CMU_IR_FBDIV_FRAC_WKUP_MSK       ((0x1UL) << HI1103_PMU2_CMU_IR_FBDIV_FRAC_WKUP_OFFSET)
#define HI1103_PMU2_CMU_IR_FBDIV_FRAC_SLP_STS_MSK    ((0x1UL) << HI1103_PMU2_CMU_IR_FBDIV_FRAC_SLP_STS_OFFSET)

#define HI1103_PMU2_CMU_IR_PMU2_CMU_ABB_MAN_SEL_W_2_REG   (HI1103_PMU2_CMU_IR_BASE + 0x148) /* PMU2_CMU_ABB 手动选择 */
#define HI1103_PMU2_CMU_IR_PMU_RFLDO3_EN_MAN_W_SEL_OFFSET 6

#define HI1103_PMU2_CMU_IR_PMU2_CMU_ABB_STS_2_REG (HI1103_PMU2_CMU_IR_BASE + 0x15C) /* PMU2_CMU_ABB 实际状态 */

#define HI1103_PMU2_CMU_IR_PMU2_CMU_ABB_MAN_SEL_W_3_REG   (HI1103_PMU2_CMU_IR_BASE + 0x168) /* PMU2_CMU_ABB 手动选择 */
#define HI1103_PMU2_CMU_IR_PMU_RFLDO6_EN_MAN_W_SEL_OFFSET 4

#define HI1103_PMU2_CMU_IR_PMU2_CMU_ABB_STS_3_REG (HI1103_PMU2_CMU_IR_BASE + 0x17C) /* PMU2_CMU_ABB 实际状态 */

#define HI1103_PMU2_CMU_IR_PMU2_CMU_ABB_MAN_SEL_W_6_REG (HI1103_PMU2_CMU_IR_BASE + 0x1C8) /* PMU2_CMU_ABB 手动选择 */
#define HI1103_PMU2_CMU_IR_PMU_XLDO_EN_MAN_W_SEL_OFFSET 0

#define HI1103_PMU2_CMU_IR_PMU2_CMU_ABB_STS_6_REG (HI1103_PMU2_CMU_IR_BASE + 0x1DC) /* PMU2_CMU_ABB 实际状态 */

#define HI1103_PMU2_CMU_IR_PMU2_CMU_ABB_MAN_SEL_W_8_REG (HI1103_PMU2_CMU_IR_BASE + 0x208) /* PMU2_CMU_ABB 手动选择 */
#define HI1103_PMU2_CMU_IR_PMU2_CMU_ABB_MAN_STS_8_REG   (HI1103_PMU2_CMU_IR_BASE + 0x210) /* PMU2_CMU_ABB 手动状态 */
#define HI1103_PMU2_CMU_IR_PMU2_CMU_ABB_DBG_SEL_8_REG   (HI1103_PMU2_CMU_IR_BASE + 0x218) /* PMU2_CMU_ABB 调试选择 */

#define HI1103_PMU2_CMU_IR_REFDIV_REG    (HI1103_PMU2_CMU_IR_BASE + 0x300) /* CMU 分频比配置 */
#define HI1103_PMU2_CMU_IR_FBDIV_REG     (HI1103_PMU2_CMU_IR_BASE + 0x304) /* CMU 分频比配置 */
#define HI1103_PMU2_CMU_IR_FRAC_L_REG    (HI1103_PMU2_CMU_IR_BASE + 0x308) /* CMU 分频比配置 */
#define HI1103_PMU2_CMU_IR_FRAC_H_REG    (HI1103_PMU2_CMU_IR_BASE + 0x30C) /* CMU 分频比配置 */
#define HI1103_PMU2_CMU_IR_POSTDIV_REG   (HI1103_PMU2_CMU_IR_BASE + 0x310) /* CMU 分频比配置 */
#define HI1103_PMU2_CMU_IR_CMU_PD_REG    (HI1103_PMU2_CMU_IR_BASE + 0x314) /* CMU PD 控制 */
#define HI1103_PMU2_CMU_IR_CMU_GT_W_REG  (HI1103_PMU2_CMU_IR_BASE + 0x318) /* CMU GT 控制 */
#define HI1103_PMU2_CMU_IR_TCXO_GT_W_REG (HI1103_PMU2_CMU_IR_BASE + 0x31C) /* TCXO GT 控制 */

#define HI1103_PMU2_CMU_IR_AON_DIV_1_REG    (HI1103_PMU2_CMU_IR_BASE + 0x418)
#define HI1103_PMU2_CMU_IR_AON_CRG_CKEN_REG (HI1103_PMU2_CMU_IR_BASE + 0x420)
#define HI1103_PMU2_CMU_IR_CLK_SEL_REG      (HI1103_PMU2_CMU_IR_BASE + 0x430)

#define HI1103_PMU2_CMU_IR_FBDIV_FRAC_SLP_REG     (HI1103_PMU2_CMU_IR_BASE + 0x370) /* CMU 分频比控制 */
#define HI1103_PMU2_CMU_IR_FBDIV_FRAC_WKUP_OFFSET 1

#define HI1103_PMU2_CMU_IR_CMU_STATUS_GRM_REG (HI1103_PMU2_CMU_IR_BASE + 0x890) /* CMU状态查询 */

#define HI1103_W_CTL_BASE                   pst_wctrl
#define HI1103_W_CTL_WTOPCRG_SOFT_CLKEN_REG (HI1103_W_CTL_BASE + 0x40)
#define HI1103_W_CTL_W_TCXO_SEL_REG         (HI1103_W_CTL_BASE + 0x80) /* WLAN TCXO/PLL时钟模式选择寄存器 */
#define HI1103_W_CTL_CLKMUX_STS_REG         (HI1103_W_CTL_BASE + 0x88)

#define PCIE_TRANS_US_OFFSET_BITS 17 /* 推算出的，计算需要 */

OAL_STATIC oal_pcie_res *g_pci_res = NULL;

OAL_STATIC oal_kobject *g_conn_syfs_pci_object = NULL;

char *g_pcie_link_state_str[PCI_WLAN_LINK_BUTT + 1] = {
    [PCI_WLAN_LINK_DOWN] = "linkdown",
    [PCI_WLAN_LINK_DEEPSLEEP] = "deepsleep",
    [PCI_WLAN_LINK_UP] = "linkup",
    [PCI_WLAN_LINK_MEM_UP] = "memup",
    [PCI_WLAN_LINK_RES_UP] = "resup",
    [PCI_WLAN_LINK_WORK_UP] = "workup",
    [PCI_WLAN_LINK_BUTT] = "butt"
};

OAL_STATIC oal_pcie_bar_info g_en_bar_tab[] = {
    /*
     * 1103 4.7a 一个BAR [8MB]， 5.0a 为两个BAR[Bar0 8M  BAR1 16KB]
     * (因为1103 是64bit bar,所以对应bar index寄存器, 是对应bar index=2,
     *  参考 __pci_read_base 最后一行),
     * 第二个BAR 直接用MEM 方式 访问IATU表
     */
    {
        .bar_idx = OAL_PCI_BAR_0,
    },
};

int32_t g_hipcie_loglevel = PCI_LOG_INFO;
oal_debug_module_param(g_hipcie_loglevel, int, S_IRUGO | S_IWUSR);

int32_t g_pcie_soft_fifo_enable = 0;
oal_debug_module_param(g_pcie_soft_fifo_enable, int, S_IRUGO | S_IWUSR);

int32_t g_pcie_ringbuf_bugfix_enable = 1;
oal_debug_module_param(g_pcie_ringbuf_bugfix_enable, int, S_IRUGO | S_IWUSR);

int32_t g_pcie_dma_data_check_enable = 0; /* Wi-Fi关闭时可以修改此标记 */
oal_debug_module_param(g_pcie_dma_data_check_enable, int, S_IRUGO | S_IWUSR);

int32_t g_ft_pcie_wcpu_max_freq_bypass = 0;
oal_debug_module_param(g_ft_pcie_wcpu_max_freq_bypass, int, S_IRUGO | S_IWUSR);

int32_t g_ft_pcie_link_err_bypass = 0;
oal_debug_module_param(g_ft_pcie_link_err_bypass, int, S_IRUGO | S_IWUSR);

int32_t g_ft_pcie_wcpu_mem_check_times = 1;
oal_debug_module_param(g_ft_pcie_wcpu_mem_check_times, int, S_IRUGO | S_IWUSR);

int32_t g_ft_pcie_perf_runtime = 200;
oal_debug_module_param(g_ft_pcie_perf_runtime, int, S_IRUGO | S_IWUSR);

int32_t g_ft_pcie_perf_wr_bypass = 0;
oal_debug_module_param(g_ft_pcie_perf_wr_bypass, int, S_IRUGO | S_IWUSR);

int32_t g_ft_pcie_perf_rd_bypass = 1;
oal_debug_module_param(g_ft_pcie_perf_rd_bypass, int, S_IRUGO | S_IWUSR);

int32_t g_ft_pcie_wcpu_mem_check_burst_check = 0;
oal_debug_module_param(g_ft_pcie_wcpu_mem_check_burst_check, int, S_IRUGO | S_IWUSR);

int32_t g_ft_pcie_wcpu_mem_check_byword_bypass = 1;
oal_debug_module_param(g_ft_pcie_wcpu_mem_check_byword_bypass, int, S_IRUGO | S_IWUSR);

int32_t g_ft_pcie_write_address_bypass = 0;
oal_debug_module_param(g_ft_pcie_write_address_bypass, int, S_IRUGO | S_IWUSR);

int32_t g_pcie_ldo_phy_0v9_param = 0;
oal_debug_module_param(g_pcie_ldo_phy_0v9_param, int, S_IRUGO | S_IWUSR);

int32_t g_pcie_ldo_phy_1v8_param = 0;
oal_debug_module_param(g_pcie_ldo_phy_1v8_param, int, S_IRUGO | S_IWUSR);

/* 0 memcopy from kernel ,1 memcopy from self */
int32_t g_pcie_memcopy_type = 0;
oal_debug_module_param(g_pcie_memcopy_type, int, S_IRUGO | S_IWUSR);
EXPORT_SYMBOL_GPL(g_pcie_memcopy_type);

char *pci_loglevel_format[] = {
    KERN_ERR "[PCIE][ERR] ",
    KERN_WARNING "[PCIE][WARN]",
    KERN_INFO "[PCIE][INFO]",
    KERN_INFO "[PCIE][DBG] ",
};

/* factory test */
/* mpw2 */
OAL_STATIC uintptr_t g_mpw2_wmem_scan_array[][PCIE_MEM_MSG_SIZE] = {
    { 0x00004000, 0x0008BFFF }, /* itcm ram */
    { 0x20000000, 0x20067FFF }, /* dtcm */
    { 0x60000000, 0x6008FFFF }, /* share mem */
};

/* pilot */
OAL_STATIC uintptr_t g_pilot_wmem_scan_array[][PCIE_MEM_MSG_SIZE] = {
    { 0x00040000, 0x000A7FFF }, /* itcm ram */
    { 0x20018000, 0x2007FFFF }, /* dtcm */
    { 0x60000000, 0x6007FFFF }, /* share mem */
};

OAL_STATIC uintptr_t g_mpw2_bmem_scan_array[][PCIE_MEM_MSG_SIZE] = {
    { 0x80000000, 0x800FFFFF }, /* itcm */
    { 0x80100000, 0x801DFFFF }, /* dtcm */
};

/* pilot */
OAL_STATIC uintptr_t g_pilot_bmem_scan_array[][PCIE_MEM_MSG_SIZE] = {
    { 0x80040000, 0x8010FFFF }, /* itcm */
    { 0x80200000, 0x8030FFFF }, /* dtcm */
};

/* 1105 */
OAL_STATIC uintptr_t g_hi1105_asic_wmem_scan_array[][PCIE_MEM_MSG_SIZE] = {
    { 0x00040000, 0x000BFFFF }, /* itcm ram */
    { 0x20018000, 0x2007FFFF }, /* dtcm */
    { 0x60000000, 0x600FFFFF }, /* share mem */
};

OAL_STATIC uintptr_t g_hi1105_fpga_wmem_scan_array[][PCIE_MEM_MSG_SIZE] = {
    { 0x00010000, 0x00037FFF }, /* itcm rom */
    { 0x00040000, 0x000BFFFF }, /* itcm ram */
    { 0x20018000, 0x2007FFFF }, /* dtcm */
    { 0x60000000, 0x6007FFFF }, /* share mem */
};

OAL_STATIC uintptr_t g_hi1105_asic_bmem_scan_array[][PCIE_MEM_MSG_SIZE] = {
    { 0x80040000, 0x8014FFFF }, /* itcm */
    { 0x80200000, 0x8030FFFF }, /* dtcm */
};

typedef enum _HI1103_REGIONS_ {
    HI1103_REGION_WCPU_ITCM,
    HI1103_REGION_WCPU_DTCM,
    HI1103_REGION_W_PKTRAM,
    HI1103_REGION_W_PERP_AHB,
    HI1103_REGION_BCPU_ITCM,
    HI1103_REGION_BCPU_DTCM,
    HI1103_REGION_B_PERP_AHB,
    HI1103_REGION_AON_APB,
    HI1103_REGION_BUTT
} HI1103_REGIONS;

/* Region大小必须为4KB的倍数，iATU要求 */
/* 这里的分段都是对应iATU inbound */
OAL_STATIC oal_pcie_region g_hi1103_pcie_mpw2_regions[] = {
    {   .pci_start = 0x00000000,
        .pci_end = 0x0008BFFF,
        .cpu_start = 0x00000000,
        .cpu_end = 0x0008BFFF, /* 560KB */
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
        .flag = OAL_IORESOURCE_MEM,
#else
        .flag = OAL_IORESOURCE_REG,
#endif
        .name = oal_pcie_to_name(HI1103_REGION_WCPU_ITCM)
    },
    {   .pci_start = 0x20000000,
        .pci_end = 0x20067FFF,
        .cpu_start = 0x20000000,
        .cpu_end = 0x20067FFF, /* 416KB */
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
        .flag = OAL_IORESOURCE_MEM,
#else
        .flag = OAL_IORESOURCE_REG,
#endif
        .name = oal_pcie_to_name(HI1103_REGION_WCPU_DTCM)
    },
    {   .pci_start = 0x60000000,
        .pci_end = 0x6008FFFF,
        .cpu_start = 0x60000000,
        .cpu_end = 0x6008FFFF, /* 576KB */
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
        .flag = OAL_IORESOURCE_MEM,
#else
        .flag = OAL_IORESOURCE_REG,
#endif
        .name = oal_pcie_to_name(HI1103_REGION_W_PKTRAM)
    },
    {   .pci_start = 0x40000000,
        .pci_end = 0x40102FFF,
        .cpu_start = 0x40000000,
        .cpu_end = 0x40102FFF, /* 1036KB */
        .flag = OAL_IORESOURCE_REG,
        .name = oal_pcie_to_name(HI1103_REGION_W_PERP_AHB)
    },
    {   .pci_start = 0x80000000,
        .pci_end = 0x800FFFFF,
        .cpu_start = 0x80000000,
        .cpu_end = 0x800FFFFF, /* 1024KB */
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
        .flag = OAL_IORESOURCE_MEM,
#else
        .flag = OAL_IORESOURCE_REG,
#endif
        .name = oal_pcie_to_name(HI1103_REGION_BCPU_ITCM)
    },
    {   .pci_start = 0x80100000,
        .pci_end = 0x801DFFFF,
        .cpu_start = 0x80100000,
        .cpu_end = 0x801DFFFF, /* 896KB */
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
        .flag = OAL_IORESOURCE_MEM,
#else
        .flag = OAL_IORESOURCE_REG,
#endif
        .name = oal_pcie_to_name(HI1103_REGION_BCPU_DTCM)
    },
    {   .pci_start = 0x48000000,
        .pci_end = 0x48122FFF,
        .cpu_start = 0x48000000,
        .cpu_end = 0x48122FFF, /* 1164KB */
        .flag = OAL_IORESOURCE_REG,
        .name = oal_pcie_to_name(HI1103_REGION_B_PERP_AHB)
    },
    {   .pci_start = 0x50000000,
        .pci_end = 0x5000EDFF,
        .cpu_start = 0x50000000,
        .cpu_end = 0x5000EDFF, /* 59KB */
        .flag = OAL_IORESOURCE_REG,
        .name = oal_pcie_to_name(HI1103_REGION_AON_APB)
    },
};

OAL_STATIC oal_pcie_region g_hi1103_pcie_pilot_regions[] = {
    {   .pci_start = 0x00000000,
        .pci_end = 0x000A7FFF,
        .cpu_start = 0x00000000,
        .cpu_end = 0x000A7FFF, /* 592KB */
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
        .flag = OAL_IORESOURCE_MEM,
#else
        .flag = OAL_IORESOURCE_REG,
#endif
        .name = oal_pcie_to_name(HI1103_REGION_WCPU_ITCM)
    },
    {   .pci_start = 0x20018000,
        .pci_end = 0x2007FFFF,
        .cpu_start = 0x20018000,
        .cpu_end = 0x2007FFFF, /* 416KB */
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
        .flag = OAL_IORESOURCE_MEM,
#else
        .flag = OAL_IORESOURCE_REG,
#endif
        .name = oal_pcie_to_name(HI1103_REGION_WCPU_DTCM)
    },
    {   .pci_start = 0x60000000,
        .pci_end = 0x6007FFFF,
        .cpu_start = 0x60000000,
        .cpu_end = 0x6007FFFF, /* 512KB */
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
        .flag = OAL_IORESOURCE_MEM,
#else
        .flag = OAL_IORESOURCE_REG,
#endif
        .name = oal_pcie_to_name(HI1103_REGION_W_PKTRAM)
    },
    {   .pci_start = 0x40000000,
        .pci_end = 0x40107FFF,
        .cpu_start = 0x40000000,
        .cpu_end = 0x40107FFF,
        .flag = OAL_IORESOURCE_REG,
        .name = oal_pcie_to_name(HI1103_REGION_W_PERP_AHB)
    },
    {   .pci_start = 0x80040000,
        .pci_end = 0x8010FFFF,
        .cpu_start = 0x80040000,
        .cpu_end = 0x8010FFFF, /* 832KB */
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
        .flag = OAL_IORESOURCE_MEM,
#else
        .flag = OAL_IORESOURCE_REG,
#endif
        .name = oal_pcie_to_name(HI1103_REGION_BCPU_ITCM)
    },
    {   .pci_start = 0x80200000,
        .pci_end = 0x8030FFFF,
        .cpu_start = 0x80200000,
        .cpu_end = 0x8030FFFF, /* 1088KB */
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
        .flag = OAL_IORESOURCE_MEM,
#else
        .flag = OAL_IORESOURCE_REG,
#endif
        .name = oal_pcie_to_name(HI1103_REGION_BCPU_DTCM)
    },
    {   .pci_start = 0x48000000,
        .pci_end = 0x48122FFF,
        .cpu_start = 0x48000000,
        .cpu_end = 0x48122FFF, /* 1164KB */
        .flag = OAL_IORESOURCE_REG,
        .name = oal_pcie_to_name(HI1103_REGION_B_PERP_AHB)
    },
    {   .pci_start = 0x50000000,
        .pci_end = 0x5000EDFF,
        .cpu_start = 0x50000000,
        .cpu_end = 0x5000EDFF, /* 59KB */
        .flag = OAL_IORESOURCE_REG,
        .name = oal_pcie_to_name(HI1103_REGION_AON_APB)
    },
};

OAL_STATIC oal_pcie_region g_hi1105_pcie_asic_regions[] = {
    {   .pci_start = 0x00000000,
        .pci_end = 0x000BFFFF,
        .cpu_start = 0x00000000,
        .cpu_end = 0x000BFFFF,
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
        .flag = OAL_IORESOURCE_MEM,
#else
        .flag = OAL_IORESOURCE_REG,
#endif
        .name = oal_pcie_to_name(HI1103_REGION_WCPU_ITCM)
    },
    {   .pci_start = 0x20018000,
        .pci_end = 0x2007FFFF,
        .cpu_start = 0x20018000,
        .cpu_end = 0x2007FFFF, /* 416KB */
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
        .flag = OAL_IORESOURCE_MEM,
#else
        .flag = OAL_IORESOURCE_REG,
#endif
        .name = oal_pcie_to_name(HI1103_REGION_WCPU_DTCM)
    },
    {   .pci_start = 0x60000000,
        .pci_end = 0x600FFFFF,
        .cpu_start = 0x60000000,
        .cpu_end = 0x600FFFFF, /* 512*2KB */
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
        .flag = OAL_IORESOURCE_MEM,
#else
        .flag = OAL_IORESOURCE_REG,
#endif
        .name = oal_pcie_to_name(HI1103_REGION_W_PKTRAM)
    },
    {   .pci_start = 0x40000000,
        .pci_end = 0x40107FFF,
        .cpu_start = 0x40000000,
        .cpu_end = 0x40107FFF,
        .flag = OAL_IORESOURCE_REG,
        .name = oal_pcie_to_name(HI1103_REGION_W_PERP_AHB)
    },
    {   .pci_start = 0x80000000,
        .pci_end = 0x8014FFFF,
        .cpu_start = 0x80000000,
        .cpu_end = 0x8014FFFF,
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
        .flag = OAL_IORESOURCE_MEM,
#else
        .flag = OAL_IORESOURCE_REG,
#endif
        .name = oal_pcie_to_name(HI1103_REGION_BCPU_ITCM)
    },
    {   .pci_start = 0x80200000,
        .pci_end = 0x8030FFFF,
        .cpu_start = 0x80200000,
        .cpu_end = 0x8030FFFF, /* 1088KB */
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
        .flag = OAL_IORESOURCE_MEM,
#else
        .flag = OAL_IORESOURCE_REG,
#endif
        .name = oal_pcie_to_name(HI1103_REGION_BCPU_DTCM)
    },
    {   .pci_start = 0x48000000,
        .pci_end = 0x48001FFF,
        .cpu_start = 0x48000000,
        .cpu_end = 0x483FFFFF,
        .flag = OAL_IORESOURCE_REG,
        .name = oal_pcie_to_name(HI1103_REGION_B_PERP_AHB)
    },
    {   .pci_start = 0x50000000,
        .pci_end = 0x5000FFFF,
        .cpu_start = 0x50000000,
        .cpu_end = 0x5000FFFF,
        .flag = OAL_IORESOURCE_REG,
        .name = oal_pcie_to_name(HI1103_REGION_AON_APB)
    },
};

OAL_STATIC oal_pcie_region g_hi1105_pcie_fpga_regions[] = {
    {   .pci_start = 0x00000000,
        .pci_end = 0x000BFFFF,
        .cpu_start = 0x00000000,
        .cpu_end = 0x000BFFFF,
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
        .flag = OAL_IORESOURCE_MEM,
#else
        .flag = OAL_IORESOURCE_REG,
#endif
        .name = oal_pcie_to_name(HI1103_REGION_WCPU_ITCM)
    },
    {   .pci_start = 0x20018000,
        .pci_end = 0x2007FFFF,
        .cpu_start = 0x20018000,
        .cpu_end = 0x2007FFFF, /* 416KB */
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
        .flag = OAL_IORESOURCE_MEM,
#else
        .flag = OAL_IORESOURCE_REG,
#endif
        .name = oal_pcie_to_name(HI1103_REGION_WCPU_DTCM)
    },
    {   .pci_start = 0x60000000,
        .pci_end = 0x6007FFFF,
        .cpu_start = 0x60000000,
        .cpu_end = 0x6007FFFF, /* 512KB */
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
        .flag = OAL_IORESOURCE_MEM,
#else
        .flag = OAL_IORESOURCE_REG,
#endif
        .name = oal_pcie_to_name(HI1103_REGION_W_PKTRAM)
    },
    {   .pci_start = 0x40000000,
        .pci_end = 0x40107FFF,
        .cpu_start = 0x40000000,
        .cpu_end = 0x40107FFF,
        .flag = OAL_IORESOURCE_REG,
        .name = oal_pcie_to_name(HI1103_REGION_W_PERP_AHB)
    },
    {   .pci_start = 0x50000000,
        .pci_end = 0x5000FFFF,
        .cpu_start = 0x50000000,
        .cpu_end = 0x5000FFFF,
        .flag = OAL_IORESOURCE_REG,
        .name = oal_pcie_to_name(HI1103_REGION_AON_APB)
    },
};

OAL_STATIC oal_pcie_region g_hi1106_pcie_fpga_regions[] = {
    {   .pci_start = 0x00000000 + 0x3000000,  /* 0x3000000 : 外设ACP地址固定偏移 */
        .pci_end   = 0x00003FFF + 0x3000000,
        .cpu_start = 0x00000000,
        .cpu_end   = 0x00003FFF, /* 16K */
        .flag      = OAL_IORESOURCE_REG,
        .name      = "HI1106_BOOT"
    },
    {   .pci_start = 0x00040000 + 0x3000000,
        .pci_end   = 0x001bFFFF + 0x3000000,
        .cpu_start = 0x00040000,
        .cpu_end   = 0x001bFFFF, /* 1536KB */
        .flag      = OAL_IORESOURCE_REG,
        .name      = "HI1106_WRAM_NON_ACP"
    },
    {   .pci_start = 0x02000000 + 0x3000000,
        .pci_end   = 0x02089FFF + 0x3000000,
        .cpu_start = 0x02000000,
        .cpu_end   = 0x02089FFF, /* 552KB */
        .flag      = OAL_IORESOURCE_REG,
        /* 和芯片约束，PCIE不使用S7端口 */
        .name      = "HI1106_SHARE_MEM_S6_NON_ACP"
    },
    {   .pci_start = 0x40000000,
        .pci_end   = 0x40115FFF,
        .cpu_start = 0x40000000,
        .cpu_end   = 0x40115FFF, /* 1112KB */
        .flag      = OAL_IORESOURCE_REG,
        .name      = "HI1106_AON_APB"
    },
    {   .pci_start = 0x44000000,
        .pci_end   = 0x44121FFF,
        .cpu_start = 0x44000000,
        .cpu_end   = 0x44121FFF, /* 1160KB */
        .flag      = OAL_IORESOURCE_REG,
        .name      = "HI1106_BITCM"
    },
    {   .pci_start = 0x44200000,
        .pci_end   = 0x442FFFFF,
        .cpu_start = 0x44200000,
        .cpu_end   = 0x442FFFFF, /* 1024KB */
        .flag      = OAL_IORESOURCE_REG,
        .name      = "HI1106_BDTCM"
    },
    {   .pci_start = 0x44400000,
        .pci_end   = 0x4446FFFF,
        .cpu_start = 0x44400000,
        .cpu_end   = 0x4446FFFF, /* 448KB */
        .flag      = OAL_IORESOURCE_REG,
        .name      = "HI1106_GITCM"
    },
    {   .pci_start = 0x44500000,
        .pci_end   = 0x44601FFF,
        .cpu_start = 0x44500000,
        .cpu_end   = 0x44601FFF, /* 1032KB */
        .flag      = OAL_IORESOURCE_REG,
        .name      = "HI1106_GDTCM"
    },
};

OAL_STATIC oal_pcie_region g_hi1106_pcie_asic_regions[] = {
    {   .pci_start = 0x00000000 + 0x3000000,  /* 0x3000000 : 外设ACP地址固定偏移 */
        .pci_end   = 0x00003FFF + 0x3000000,
        .cpu_start = 0x00000000,
        .cpu_end   = 0x00003FFF, /* 16K */
        .flag      = OAL_IORESOURCE_REG,
        .name      = "HI1106_BOOT"
    },
    {   .pci_start = 0x00040000 + 0x3000000,
        .pci_end   = 0x001bFFFF + 0x3000000,
        .cpu_start = 0x00040000,
        .cpu_end   = 0x001bFFFF, /* 1536KB */
        .flag      = OAL_IORESOURCE_REG,
        .name      = "HI1106_WRAM_NON_ACP"
    },
    {   .pci_start = 0x02000000 + 0x3000000,
        .pci_end   = 0x02089FFF + 0x3000000,
        .cpu_start = 0x02000000,
        .cpu_end   = 0x02089FFF, /* 552KB */
        .flag      = OAL_IORESOURCE_REG,
        /* 和芯片约束，PCIE不使用S7端口 */
        .name      = "HI1106_SHARE_MEM_S6_NON_ACP"
    },
    {   .pci_start = 0x40000000,
        .pci_end   = 0x40115FFF,
        .cpu_start = 0x40000000,
        .cpu_end   = 0x40115FFF, /* 1112KB */
        .flag      = OAL_IORESOURCE_REG,
        .name      = "HI1106_AON_APB"
    },
    {   .pci_start = 0x44000000,
        .pci_end   = 0x44121FFF,
        .cpu_start = 0x44000000,
        .cpu_end   = 0x44121FFF, /* 1160KB */
        .flag      = OAL_IORESOURCE_REG,
        .name      = "HI1106_BITCM"
    },
    {   .pci_start = 0x44200000,
        .pci_end   = 0x442FFFFF,
        .cpu_start = 0x44200000,
        .cpu_end   = 0x442FFFFF, /* 1024KB */
        .flag      = OAL_IORESOURCE_REG,
        .name      = "HI1106_BDTCM"
    },
    {   .pci_start = 0x44400000,
        .pci_end   = 0x4446FFFF,
        .cpu_start = 0x44400000,
        .cpu_end   = 0x4446FFFF, /* 448KB */
        .flag      = OAL_IORESOURCE_REG,
        .name      = "HI1106_GITCM"
    },
    {   .pci_start = 0x44500000,
        .pci_end   = 0x44601FFF,
        .cpu_start = 0x44500000,
        .cpu_end   = 0x44601FFF, /* 1032KB */
        .flag      = OAL_IORESOURCE_REG,
        .name      = "HI1106_GDTCM"
    },
};

oal_reg_bits_stru g_pcie_phy_0v9_bits[] = {
    { 700, 0x0, "0.7v" },
    { 750, 0x1, "0.75v" },
    { 775, 0x2, "0.775v" },
    { 800, 0x3, "0.8v" },
    { 825, 0x4, "0.825v" },
    { 850, 0x5, "0.85v"},
    { 875, 0x6, "0.875v"},
    { 900,  0x7, "0.9v" },
    { 925,  0x8, "0.925v" },
    { 950,  0x9, "0.950v" },
    { 975,  0xa, "0.975v" },
    { 1000, 0xb, "1v" },
    { 1025, 0xc, "1.025v" },
    { 1050, 0xd, "1.050v" },
    { 1075, 0xe, "1.075v" },
    { 1100, 0xf, "1.1v" }
};

oal_reg_bits_stru g_pcie_phy_1v8_bits[] = {
    { 1600, 0x0, "1.6v" },
    { 1625, 0x1, "1.625v" },
    { 1650, 0x2, "1.650v" },
    { 1675, 0x3, "1.675v" },
    { 1700, 0x4, "1.700v" },
    { 1725, 0x5, "1.725v" },
    { 1750, 0x6, "1.750v" },
    { 1775, 0x7, "1.775v" },
    { 1800, 0x8, "1.800v" }
};

#ifdef _PRE_PLAT_FEATURE_HI110X_PCIE_H2D_BYPASS
uint32_t h2d_bypass_pkt_num = 0;
oal_netbuf_stru *g_h2d_pst_netbuf = NULL;
dma_addr_t g_h2d_pci_dma_addr = 0x0;
#endif

#ifdef _PRE_PLAT_FEATURE_HI110X_PCIE_D2H_BYPASS
uint32_t d2h_bypass_pkt_num = 0;
uint32_t d2h_bypass_total_pkt_num = 0;
oal_netbuf_stru *g_d2h_pst_netbuf = NULL;
dma_addr_t g_d2h_pci_dma_addr = 0x0;
oal_completion g_d2h_test_done;
#endif

/* Function Declare */
OAL_STATIC int32_t oal_pcie_h2d_doorbell(oal_pcie_res *pst_pci_res);
OAL_STATIC int32_t oal_pcie_d2h_doorbell(oal_pcie_res *pst_pci_res);
OAL_STATIC int32_t oal_pcie_d2h_ringbuf_rd_update(oal_pcie_res *pst_pci_res);
OAL_STATIC int32_t oal_pcie_d2h_ringbuf_wr_update(oal_pcie_res *pst_pci_res);
OAL_STATIC uint32_t oal_pcie_d2h_ringbuf_freecount(oal_pcie_res *pst_pci_res, int32_t is_sync);
OAL_STATIC int32_t oal_pcie_d2h_ringbuf_write(oal_pcie_res *pst_pci_res,
                                              pci_addr_map *pst_map,
                                              pcie_write_ringbuf_item *pst_item);
void oal_pcie_tx_netbuf_free(oal_pcie_res *pst_pci_res, oal_netbuf_stru *pst_netbuf);
OAL_STATIC void oal_pcie_print_debug_usages(void);
int32_t oal_pcie_ringbuf_read_rd(oal_pcie_res *pst_pci_res, PCIE_COMM_RINGBUF_TYPE type);
OAL_STATIC int32_t oal_pcie_ringbuf_read_wr(oal_pcie_res *pst_pci_res, PCIE_COMM_RINGBUF_TYPE type);
OAL_STATIC int32_t oal_pcie_ringbuf_read(oal_pcie_res *pst_pci_res, PCIE_COMM_RINGBUF_TYPE type,
                                         uint8_t *buf, uint32_t len);
int32_t oal_pcie_ringbuf_write(oal_pcie_res *pst_pci_res,
                               PCIE_COMM_RINGBUF_TYPE type, uint8_t *buf, uint32_t len);
uint32_t oal_pcie_comm_ringbuf_freecount(oal_pcie_res *pst_pci_res,
                                         PCIE_COMM_RINGBUF_TYPE type);
OAL_STATIC int32_t oal_pcie_device_check_alive(oal_pcie_res *pst_pci_res);

/* 函数定义 */
OAL_STATIC void oal_pcie_io_trans64_sub(uint64_t *dst, uint64_t *src, int32_t size)
{
    int32_t remain = size;

    forever_loop() {
        if (remain < 8) { /* 8bytes 最少传输字节数，剩余数据量不足时退出 */
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
            rmb();
            wmb(); /* dsb */
#endif
            break;
        }
        if (remain >= 128) { /* 128 bytes, 根据剩余数据量拷贝，一次性拷贝128字节 */
            *((volatile uint64_t *)dst++) = *((volatile uint64_t *)src++);
            *((volatile uint64_t *)dst++) = *((volatile uint64_t *)src++);
            *((volatile uint64_t *)dst++) = *((volatile uint64_t *)src++);
            *((volatile uint64_t *)dst++) = *((volatile uint64_t *)src++);
            *((volatile uint64_t *)dst++) = *((volatile uint64_t *)src++);
            *((volatile uint64_t *)dst++) = *((volatile uint64_t *)src++);
            *((volatile uint64_t *)dst++) = *((volatile uint64_t *)src++);
            *((volatile uint64_t *)dst++) = *((volatile uint64_t *)src++);
            *((volatile uint64_t *)dst++) = *((volatile uint64_t *)src++);
            *((volatile uint64_t *)dst++) = *((volatile uint64_t *)src++);
            *((volatile uint64_t *)dst++) = *((volatile uint64_t *)src++);
            *((volatile uint64_t *)dst++) = *((volatile uint64_t *)src++);
            *((volatile uint64_t *)dst++) = *((volatile uint64_t *)src++);
            *((volatile uint64_t *)dst++) = *((volatile uint64_t *)src++);
            *((volatile uint64_t *)dst++) = *((volatile uint64_t *)src++);
            *((volatile uint64_t *)dst++) = *((volatile uint64_t *)src++);
            remain -= 128; // 128 bytes
        } else if (remain >= 64) { /* 64bytes 根据剩余数据量拷贝，一次性拷贝64字节 */
            *((volatile uint64_t *)dst++) = *((volatile uint64_t *)src++);
            *((volatile uint64_t *)dst++) = *((volatile uint64_t *)src++);
            *((volatile uint64_t *)dst++) = *((volatile uint64_t *)src++);
            *((volatile uint64_t *)dst++) = *((volatile uint64_t *)src++);
            *((volatile uint64_t *)dst++) = *((volatile uint64_t *)src++);
            *((volatile uint64_t *)dst++) = *((volatile uint64_t *)src++);
            *((volatile uint64_t *)dst++) = *((volatile uint64_t *)src++);
            *((volatile uint64_t *)dst++) = *((volatile uint64_t *)src++);
            remain -= 64; // 64 bytes
        } else if (remain >= 32) { /* 32 bytes 根据剩余数据量拷贝，一次性拷贝32字节 */
            *((volatile uint64_t *)dst++) = *((volatile uint64_t *)src++);
            *((volatile uint64_t *)dst++) = *((volatile uint64_t *)src++);
            *((volatile uint64_t *)dst++) = *((volatile uint64_t *)src++);
            *((volatile uint64_t *)dst++) = *((volatile uint64_t *)src++);
            remain -= 32; // 32 bytes
        } else if (remain >= 16) { /* 16 bytes 根据剩余数据量拷贝，一次性拷贝16字节 */
            *((volatile uint64_t *)dst++) = *((volatile uint64_t *)src++);
            *((volatile uint64_t *)dst++) = *((volatile uint64_t *)src++);
            remain -= 16; // 16 bytes
        } else {
            *((volatile uint64_t *)dst++) = *((volatile uint64_t *)src++);
            remain -= 8; /* 剩余数据量为8到15字节时，只拷贝8字节 */
        }
    }
}

void oal_pcie_io_trans32(uint32_t *dst, uint32_t *src, int32_t size)
{
    int32_t remain = size;

    forever_loop() {
        if (remain < 4) { /* 4 bytes 最少传输字节数，剩余数据量不足时退出 */
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
            rmb();
            wmb(); /* dsb */
#endif
            break;
        }
        if (remain >= 64) { /* 64 bytes 根据剩余数据量拷贝，一次性拷贝64字节 */
            *((volatile uint32_t *)dst++) = *((volatile uint32_t *)src++);
            *((volatile uint32_t *)dst++) = *((volatile uint32_t *)src++);
            *((volatile uint32_t *)dst++) = *((volatile uint32_t *)src++);
            *((volatile uint32_t *)dst++) = *((volatile uint32_t *)src++);
            *((volatile uint32_t *)dst++) = *((volatile uint32_t *)src++);
            *((volatile uint32_t *)dst++) = *((volatile uint32_t *)src++);
            *((volatile uint32_t *)dst++) = *((volatile uint32_t *)src++);
            *((volatile uint32_t *)dst++) = *((volatile uint32_t *)src++);
            *((volatile uint32_t *)dst++) = *((volatile uint32_t *)src++);
            *((volatile uint32_t *)dst++) = *((volatile uint32_t *)src++);
            *((volatile uint32_t *)dst++) = *((volatile uint32_t *)src++);
            *((volatile uint32_t *)dst++) = *((volatile uint32_t *)src++);
            *((volatile uint32_t *)dst++) = *((volatile uint32_t *)src++);
            *((volatile uint32_t *)dst++) = *((volatile uint32_t *)src++);
            *((volatile uint32_t *)dst++) = *((volatile uint32_t *)src++);
            *((volatile uint32_t *)dst++) = *((volatile uint32_t *)src++);
            remain -= 64; // 64 bytes
        } else if (remain >= 32) { /* 32 bytes 根据剩余数据量拷贝，一次性拷贝32字节 */
            *((volatile uint32_t *)dst++) = *((volatile uint32_t *)src++);
            *((volatile uint32_t *)dst++) = *((volatile uint32_t *)src++);
            *((volatile uint32_t *)dst++) = *((volatile uint32_t *)src++);
            *((volatile uint32_t *)dst++) = *((volatile uint32_t *)src++);
            *((volatile uint32_t *)dst++) = *((volatile uint32_t *)src++);
            *((volatile uint32_t *)dst++) = *((volatile uint32_t *)src++);
            *((volatile uint32_t *)dst++) = *((volatile uint32_t *)src++);
            *((volatile uint32_t *)dst++) = *((volatile uint32_t *)src++);
            remain -= 32; // 32 bytes
        } else if (remain >= 16) { /* 16 bytes 根据剩余数据量拷贝，一次性拷贝16字节 */
            *((volatile uint32_t *)dst++) = *((volatile uint32_t *)src++);
            *((volatile uint32_t *)dst++) = *((volatile uint32_t *)src++);
            *((volatile uint32_t *)dst++) = *((volatile uint32_t *)src++);
            *((volatile uint32_t *)dst++) = *((volatile uint32_t *)src++);
            remain -= 16; // 16 bytes
        } else if (remain >= 8) { /* 8 bytes 根据剩余数据量拷贝，一次性拷贝8字节 */
            *((volatile uint32_t *)dst++) = *((volatile uint32_t *)src++);
            *((volatile uint32_t *)dst++) = *((volatile uint32_t *)src++);
            remain -= 8; // 8 bytes
        } else {
            *((volatile uint32_t *)dst++) = *((volatile uint32_t *)src++);
            remain -= 4; /* 4 bytes 剩余数据量为4到7字节时，只拷贝4字节 */
        }
    }
}

void oal_pcie_io_trans64(void *dst, void *src, int32_t size)
{
    uint32_t copy_size;
    int32_t remain = size;
    forever_loop() {
        if (remain < 4) { /* 4 bytes 最少传输字节数，剩余数据量不足时退出 */
            break;
        }

        if ((!((uintptr_t)src & 0x7)) && (remain >= 8)) { /* 8bytes */
            copy_size = OAL_ROUND_DOWN((uint32_t)remain, 8); /* 清除低3bit，保证8字节对齐 */
            remain -= copy_size;
            oal_pcie_io_trans64_sub(dst, src, (int32_t)copy_size);
            src += copy_size;
            dst += copy_size;
        } else if ((!((uintptr_t)src & 0x3)) && (remain >= 4)) { /* 4bytes */
            remain -= 4;                                         /* 拷贝4字节，则长度减少4 */
            *((volatile uint32_t *)dst) = *((volatile uint32_t *)src);
            /* 每次偏移4字节 */
            dst += sizeof(uint32_t);
            src += sizeof(uint32_t);
        } else {
            oal_print_hi11xx_log(HI11XX_LOG_ERR, "invalid argument, dst:%pK , src:%pK, remain:%d", dst, src, remain);
        }
    }
}

int32_t oal_pcie_memport_copy_test(void)
{
    unsigned long burst_size = 4096; // 4096 one mem page
    unsigned long timeout;
    unsigned long total_size;
    declare_time_cost_stru(cost);
    void *buff_src = NULL;
    void *buff_dst = NULL;
    uint64_t trans_size, us_to_s;

    buff_src = oal_memalloc(burst_size);
    if (buff_src == NULL) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "alloc %lu buff failed", burst_size);
        return -OAL_EFAIL;
    }

    buff_dst = oal_memalloc(burst_size);
    if (buff_dst == NULL) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "alloc %lu buff failed", burst_size);
        oal_free(buff_src);
        return -OAL_EFAIL;
    }

    oal_get_time_cost_start(cost);
    timeout = jiffies + oal_msecs_to_jiffies(2000); // 2000ms timeout
    total_size = 0;
    forever_loop() {
        if (oal_time_after(jiffies, timeout)) {
            break;
        }

        oal_pcie_io_trans((uintptr_t)buff_dst, (uintptr_t)buff_src, burst_size);
        total_size += burst_size;
    }

    oal_get_time_cost_end(cost);
    oal_calc_time_cost_sub(cost);

    us_to_s = time_cost_var_sub(cost);
    trans_size = ((total_size * 1000u * 1000u) >> PCIE_TRANS_US_OFFSET_BITS); // 1000 calc
    trans_size = div_u64(trans_size, us_to_s);

    oal_print_hi11xx_log(HI11XX_LOG_INFO, "memcopy: %llu Mbps, trans_time:%llu us, tran_size:%lu",
                         trans_size, us_to_s, total_size);

    oal_free(buff_src);
    oal_free(buff_dst);
    return 0;
}

#ifdef _PRE_PLAT_FEATURE_HI110X_PCIE_PERFORMANCE
OAL_STATIC ktime_t g_mips_arry_total[PCIE_MIPS_BUTT];
OAL_STATIC ktime_t g_mips_arry[PCIE_MIPS_BUTT];
void oal_pcie_mips_start(PCIE_MIPS_TYPE type)
{
    if (oal_warn_on(type >= PCIE_MIPS_BUTT)) {
        oal_io_print("%s error: type:%d >= limit :%d", __FUNCTION__, type, PCIE_MIPS_BUTT);
        return;
    }
    g_mips_arry[type] = ktime_get();
}

void oal_pcie_mips_end(PCIE_MIPS_TYPE type)
{
    ktime_t end = ktime_get();

    if (oal_warn_on(type >= PCIE_MIPS_BUTT)) {
        oal_io_print("%s error: type:%d >= limit :%d", __FUNCTION__, type, PCIE_MIPS_BUTT);
        return;
    }

    g_mips_arry_total[type] = ktime_add(g_mips_arry_total[type], ktime_sub(end, g_mips_arry[type]));
}

void oal_pcie_mips_clear(void)
{
    memset_s(g_mips_arry, sizeof(g_mips_arry), 0, sizeof(g_mips_arry));
    memset_s(g_mips_arry_total, sizeof(g_mips_arry_total), 0, sizeof(g_mips_arry_total));
}

void oal_pcie_mips_show(void)
{
    int32_t i;
    oal_int64 trans_us, total_us;
    total_us = 0;
    for (i = 0; i < PCIE_MIPS_BUTT; i++) {
        trans_us = (uint64_t)ktime_to_us(g_mips_arry_total[i]);
        total_us += trans_us;
        oal_io_print("mips type:%d , trans_us :%llu us\n", i, trans_us);
    }
    oal_io_print("total_us :%llu us \n", total_us);
}

#endif
oal_pcie_res *oal_get_default_pcie_handler(void)
{
    return g_pci_res;
}

/* edma functions */
OAL_STATIC int32_t oal_pcie_edma_get_read_done_fifo(oal_pcie_res *pst_pci_res, edma_paddr_t *addr,
                                                    uint32_t *count)
{
    /* 后续考虑优化， PCIE读内存比较耗时 */
    /* 先读FIFO0，分三次读走64bit 数据，读空 */
    uint32_t addr_low, addr_high;
    uint32_t trans_count;

    /* 处理fifo0 */
    trans_count = oal_readl(pst_pci_res->pst_pci_ctrl_base + PCIE_FIFO_REMOTE_READ_FIFO0_DATA_OFF);
    addr_low = oal_readl(pst_pci_res->pst_pci_ctrl_base + PCIE_FIFO_REMOTE_READ_FIFO0_DATA_OFF + 4); // 4B offset
    addr_high = oal_readl(pst_pci_res->pst_pci_ctrl_base + PCIE_FIFO_REMOTE_READ_FIFO0_DATA_OFF + 8); // 8B offset
    if (pci_dbg_condtion()) {
        addr_low = oal_readl(pst_pci_res->pst_pci_ctrl_base + PCIE_FIFO_REMOTE_READ_FIFO0_DATA_OFF + 4); // 4B offset
        addr_high = oal_readl(pst_pci_res->pst_pci_ctrl_base + PCIE_FIFO_REMOTE_READ_FIFO0_DATA_OFF + 8); // 8B offset
        pci_print_log(PCI_LOG_DBG, "read done fifo0 addr_low:0x%8x, addr_high:0x%8x, trans_count:0x%8x\n",
                      addr_low, addr_high, trans_count);
    }

    trans_count = trans_count >> 1; /* 一个数据包对应2个描述符 */
    addr->bits.low_addr = addr_low;
    addr->bits.high_addr = addr_high;
    *count = trans_count;

    /* 处理fifo1 */
    addr++;
    count++;

    trans_count = oal_readl(pst_pci_res->pst_pci_ctrl_base + PCIE_FIFO_REMOTE_READ_FIFO1_DATA_OFF);
    addr_low = oal_readl(pst_pci_res->pst_pci_ctrl_base + PCIE_FIFO_REMOTE_READ_FIFO1_DATA_OFF + 4); // 4B offset
    addr_high = oal_readl(pst_pci_res->pst_pci_ctrl_base + PCIE_FIFO_REMOTE_READ_FIFO1_DATA_OFF + 8); // 8B offset

    if (pci_dbg_condtion()) {
        addr_low = oal_readl(pst_pci_res->pst_pci_ctrl_base + PCIE_FIFO_REMOTE_READ_FIFO1_DATA_OFF + 4); // 4B offset
        addr_high = oal_readl(pst_pci_res->pst_pci_ctrl_base + PCIE_FIFO_REMOTE_READ_FIFO1_DATA_OFF + 8); // 8B offset
        pci_print_log(PCI_LOG_DBG, "read done fifo1 addr_low:0x%8x, addr_high:0x%8x, trans_count:0x%8x\n",
                      addr_low, addr_high, trans_count);
    }

    trans_count = trans_count >> 1; /* 一个数据包对应2个描述符 */
    addr->bits.low_addr = addr_low;
    addr->bits.high_addr = addr_high;
    *count = trans_count;

    return OAL_TRUE;
}

OAL_STATIC int32_t oal_pcie_edma_get_write_done_fifo(oal_pcie_res *pst_pci_res, edma_paddr_t *addr,
                                                     uint32_t *count)
{
    /* 后续考虑优化， PCIE读内存比较耗时 */
    /* 先读FIFO0，分三次读走64bit 数据，读空 */
    uint32_t addr_low, addr_high;
    uint32_t trans_count;

    /* 处理fifo0 */
    trans_count = oal_readl(pst_pci_res->pst_pci_ctrl_base + PCIE_FIFO_REMOTE_WRITE_FIFO0_DATA_OFF);
#ifdef _PRE_PLAT_FEATURE_HI110X_PCIE_FIFO_ADDRESS
    addr_low = oal_readl(pst_pci_res->pst_pci_ctrl_base + PCIE_FIFO_REMOTE_WRITE_FIFO0_DATA_OFF + 4); // 4B offset
    addr_high = oal_readl(pst_pci_res->pst_pci_ctrl_base + PCIE_FIFO_REMOTE_WRITE_FIFO0_DATA_OFF + 8); // 8B offset
#endif

    if (pci_dbg_condtion()) {
#ifndef _PRE_PLAT_FEATURE_HI110X_PCIE_FIFO_ADDRESS
        addr_low = oal_readl(pst_pci_res->pst_pci_ctrl_base + PCIE_FIFO_REMOTE_WRITE_FIFO0_DATA_OFF + 4); // 4B offset
        addr_high = oal_readl(pst_pci_res->pst_pci_ctrl_base + PCIE_FIFO_REMOTE_WRITE_FIFO0_DATA_OFF + 8); // 8B offset
#endif
        pci_print_log(PCI_LOG_DBG, "write done fifo0 addr_low:0x%8x, addr_high:0x%8x, trans_count:0x%8x\n",
                      addr_low, addr_high, trans_count);
    }

    trans_count = trans_count >> 1; /* 一个数据包对应2个描述符 */
#ifdef _PRE_PLAT_FEATURE_HI110X_PCIE_FIFO_ADDRESS
    addr->bits.low_addr = addr_low;
    addr->bits.high_addr = addr_high;
#endif
    *count = trans_count;

    /* 处理fifo1 */
    addr++;
    count++;

    trans_count = oal_readl(pst_pci_res->pst_pci_ctrl_base + PCIE_FIFO_REMOTE_WRITE_FIFO1_DATA_OFF);
#ifdef _PRE_PLAT_FEATURE_HI110X_PCIE_FIFO_ADDRESS
    addr_low = oal_readl(pst_pci_res->pst_pci_ctrl_base + PCIE_FIFO_REMOTE_WRITE_FIFO1_DATA_OFF + 4); // 4B offset
    addr_high = oal_readl(pst_pci_res->pst_pci_ctrl_base + PCIE_FIFO_REMOTE_WRITE_FIFO1_DATA_OFF + 8); // 8B offset
#endif

    if (pci_dbg_condtion()) {
#ifndef _PRE_PLAT_FEATURE_HI110X_PCIE_FIFO_ADDRESS
        addr_low = oal_readl(pst_pci_res->pst_pci_ctrl_base + PCIE_FIFO_REMOTE_WRITE_FIFO1_DATA_OFF + 4); // 4B offset
        addr_high = oal_readl(pst_pci_res->pst_pci_ctrl_base + PCIE_FIFO_REMOTE_WRITE_FIFO1_DATA_OFF + 8); // 8B offset
#endif
        pci_print_log(PCI_LOG_DBG, "write done fifo1 addr_low:0x%8x, addr_high:0x%8x, trans_count:0x%8x\n",
                      addr_low, addr_high, trans_count);
    }

    trans_count = trans_count >> 1; /* 一个数据包对应2个描述符 */
#ifdef _PRE_PLAT_FEATURE_HI110X_PCIE_FIFO_ADDRESS
    addr->bits.low_addr = addr_low;
    addr->bits.high_addr = addr_high;
#endif
    *count = trans_count;

    return OAL_TRUE;
}

/* 函数定义 */
int32_t oal_pcie_disable_regions(oal_pcie_res *pst_pci_res)
{
    if (oal_warn_on(pst_pci_res == NULL)) {
        return -OAL_ENODEV;
    }

    pst_pci_res->regions.inited = 0;
    pci_print_log(PCI_LOG_DBG, "disable_regions");
    return OAL_SUCC;
}

int32_t oal_pcie_enable_regions(oal_pcie_res *pst_pci_res)
{
    if (oal_warn_on(pst_pci_res == NULL)) {
        return -OAL_ENODEV;
    }

    pst_pci_res->regions.inited = 1;

    pci_print_log(PCI_LOG_DBG, "enable_regions");
    return OAL_SUCC;
}

OAL_STATIC void oal_pcie_iatu_reg_dump_by_viewport(oal_pcie_res *pst_pci_res)
{
    int32_t index;
    int32_t ret;
    uint32_t reg = 0;
    uint32_t region_num;
    IATU_VIEWPORT_OFF vp;
    oal_pcie_region *region_base;
    oal_pci_dev_stru *pst_pci_dev = pcie_res_to_dev(pst_pci_res);

    region_num = pst_pci_res->regions.region_nums;
    region_base = pst_pci_res->regions.pst_regions;
    for (index = 0; index < region_num; index++, region_base++) {
        vp.bits.region_dir = HI_PCI_IATU_INBOUND;
        vp.bits.region_index = index; /* iatu index */
        ret = oal_pci_write_config_dword(pst_pci_dev, HI_PCI_IATU_VIEWPORT_OFF, vp.AsDword);
        if (ret) {
            pci_print_log(PCI_LOG_ERR, "dump write [0x%8x:0x%8x] pcie failed, ret=%d\n",
                          HI_PCI_IATU_VIEWPORT_OFF, vp.AsDword, ret);
            break;
        }

        ret = oal_pci_read_config_dword(pst_pci_dev, HI_PCI_IATU_VIEWPORT_OFF, &reg);
        if (ret) {
            pci_print_log(PCI_LOG_ERR, "dump read [0x%8x] pcie failed, ret=%d\n", HI_PCI_IATU_VIEWPORT_OFF, ret);
            break;
        }

        pci_print_log(PCI_LOG_INFO, "INBOUND iatu index:%d 's register:\n", index);

        if (reg != vp.AsDword) {
            pci_print_log(PCI_LOG_ERR, "dump write [0x%8x:0x%8x] pcie viewport failed value still 0x%8x\n",
                          HI_PCI_IATU_VIEWPORT_OFF, vp.AsDword, reg);
            break;
        }

        print_pcie_config_reg(pst_pci_dev, HI_PCI_IATU_VIEWPORT_OFF);

        print_pcie_config_reg(pst_pci_dev, hi_pci_iatu_region_ctrl_1_off_inbound_i(HI_PCI_IATU_BOUND_BASE_OFF));

        print_pcie_config_reg(pst_pci_dev, hi_pci_iatu_region_ctrl_2_off_inbound_i(HI_PCI_IATU_BOUND_BASE_OFF));

        print_pcie_config_reg(pst_pci_dev, hi_pci_iatu_lwr_base_addr_off_inbound_i(HI_PCI_IATU_BOUND_BASE_OFF));

        print_pcie_config_reg(pst_pci_dev, hi_pci_iatu_upper_base_addr_off_inbound_i(HI_PCI_IATU_BOUND_BASE_OFF));

        print_pcie_config_reg(pst_pci_dev, hi_pci_iatu_limit_addr_off_inbound_i(HI_PCI_IATU_BOUND_BASE_OFF));

        print_pcie_config_reg(pst_pci_dev, hi_pci_iatu_lwr_target_addr_off_inbound_i(HI_PCI_IATU_BOUND_BASE_OFF));

        print_pcie_config_reg(pst_pci_dev, hi_pci_iatu_upper_target_addr_off_inbound_i(HI_PCI_IATU_BOUND_BASE_OFF));
    }
}

OAL_STATIC void oal_pcie_iatu_reg_dump_by_membar(oal_pcie_res *pst_pci_res)
{
    void *inbound_addr = NULL;

    int32_t index;
    uint32_t region_num;
    oal_pcie_region *region_base = NULL;

    if (pst_pci_res->st_iatu_bar.st_region.vaddr == NULL) {
        pci_print_log(PCI_LOG_ERR, "iatu bar1 vaddr is null");
        return;
    }

    inbound_addr = pst_pci_res->st_iatu_bar.st_region.vaddr;

    region_num = pst_pci_res->regions.region_nums;
    region_base = pst_pci_res->regions.pst_regions;
    for (index = 0; index < region_num; index++, region_base++) {
        if (index >= 16) { /* dump size为0x2000，一次偏移0x200，这里16代表dump了所有空间 */
            break;
        }

        pci_print_log(PCI_LOG_INFO, "INBOUND iatu index:%d 's register:\n", index);

        oal_pcie_print_config_reg_bar(pst_pci_res,
                                      hi_pci_iatu_region_ctrl_1_off_inbound_i(hi_pci_iatu_inbound_base_off(index)),
                                      "hi_pci_iatu_region_ctrl_1_off_inbound_i");

        oal_pcie_print_config_reg_bar(pst_pci_res,
                                      hi_pci_iatu_region_ctrl_2_off_inbound_i(hi_pci_iatu_inbound_base_off(index)),
                                      "hi_pci_iatu_region_ctrl_2_off_inbound_i");

        oal_pcie_print_config_reg_bar(pst_pci_res,
                                      hi_pci_iatu_lwr_base_addr_off_inbound_i(hi_pci_iatu_inbound_base_off(index)),
                                      "hi_pci_iatu_lwr_base_addr_off_inbound_i");

        oal_pcie_print_config_reg_bar(pst_pci_res,
                                      hi_pci_iatu_upper_base_addr_off_inbound_i(hi_pci_iatu_inbound_base_off(index)),
                                      "hi_pci_iatu_upper_base_addr_off_inbound_i");

        oal_pcie_print_config_reg_bar(pst_pci_res,
                                      hi_pci_iatu_limit_addr_off_inbound_i(hi_pci_iatu_inbound_base_off(index)),
                                      "hi_pci_iatu_limit_addr_off_inbound_i");

        oal_pcie_print_config_reg_bar(pst_pci_res,
                                      hi_pci_iatu_lwr_target_addr_off_inbound_i(hi_pci_iatu_inbound_base_off(index)),
                                      "hi_pci_iatu_lwr_target_addr_off_inbound_i");

        oal_pcie_print_config_reg_bar(pst_pci_res,
                                      hi_pci_iatu_upper_target_addr_off_inbound_i(hi_pci_iatu_inbound_base_off(index)),
                                      "hi_pci_iatu_upper_target_addr_off_inbound_i");
    }
}

/* bar and iATU table config */
/* set ep inbound, host->device */
OAL_STATIC void oal_pcie_iatu_reg_dump(oal_pcie_res *pst_pci_res)
{
    if (pst_pci_res->chip_info.membar_support == OAL_FALSE) {
        oal_pcie_iatu_reg_dump_by_viewport(pst_pci_res);
    } else {
        oal_pcie_iatu_reg_dump_by_membar(pst_pci_res);
    }
}

OAL_STATIC void oal_pcie_regions_info_dump(oal_pcie_res *pst_pci_res)
{
    int32_t index;
    uint32_t region_num;
    oal_pcie_region *region_base;

    region_num = pst_pci_res->regions.region_nums;
    region_base = pst_pci_res->regions.pst_regions;

    if (oal_warn_on(!pst_pci_res->regions.inited)) {
        return;
    }

    if (region_num) {
        oal_io_print("regions[%u] info dump\n", region_num);
    }

    for (index = 0; index < region_num; index++, region_base++) {
        oal_io_print("[%15s]va:0x%p, pa:0x%llx, [pci start:0x%8llx end:0x%8llx],[cpu start:0x%8llx end:0x%8llx],\
                     size:%u, flag:0x%x\n",
                     region_base->name,
                     region_base->vaddr,
                     region_base->paddr,
                     region_base->pci_start,
                     region_base->pci_end,
                     region_base->cpu_start,
                     region_base->cpu_end,
                     region_base->size,
                     region_base->flag);
    }
}

OAL_STATIC int32_t oal_pcie_set_inbound_by_viewport(oal_pcie_res *pst_pci_res)
{
    uint32_t reg = 0;
    uint32_t ret = OAL_SUCC;
    edma_paddr_t start, target, end;
    IATU_VIEWPORT_OFF vp;
    IATU_REGION_CTRL_2_OFF ctr2;
    int32_t index;
    uint32_t region_num;
    oal_pcie_region *region_base;
    oal_pci_dev_stru *pst_pci_dev = pcie_res_to_dev(pst_pci_res);
    region_num = pst_pci_res->regions.region_nums;
    region_base = pst_pci_res->regions.pst_regions;

    for (index = 0; index < region_num; index++, region_base++) {
        vp.bits.region_dir = HI_PCI_IATU_INBOUND;
        vp.bits.region_index = index; /* iatu index */
        ret = oal_pci_write_config_dword(pst_pci_dev, HI_PCI_IATU_VIEWPORT_OFF, vp.AsDword);
        if (ret) {
            pci_print_log(PCI_LOG_ERR, "write [0x%8x:0x%8x] pcie failed, ret=%u\n",
                          HI_PCI_IATU_VIEWPORT_OFF, vp.AsDword, ret);
            return -OAL_EIO;
        }

        /* 是否需要回读等待 */
        ret = oal_pci_read_config_dword(pst_pci_dev, HI_PCI_IATU_VIEWPORT_OFF, &reg);
        if (ret) {
            pci_print_log(PCI_LOG_ERR, "read [0x%8x] pcie failed, index:%d, ret=%u\n",
                          HI_PCI_IATU_VIEWPORT_OFF, index, ret);
            return -OAL_EIO;
        }

        if (reg != vp.AsDword) {
            /* 1.viewport 没有切换完成 2. iatu配置个数超过了Soc的最大个数 */
            pci_print_log(PCI_LOG_ERR,
                          "write [0x%8x:0x%8x] pcie viewport failed value still 0x%8x, region's index:%d\n",
                          HI_PCI_IATU_VIEWPORT_OFF, vp.AsDword, reg, index);
            return -OAL_EIO;
        }

        ret |= (uint32_t)oal_pci_write_config_dword(pst_pci_dev,
                                                    hi_pci_iatu_region_ctrl_1_off_inbound_i(
                                                        HI_PCI_IATU_BOUND_BASE_OFF),
                                                    0x0);

        ctr2.AsDword = 0;
        ctr2.bits.region_en = 1;
        ctr2.bits.bar_num = region_base->bar_info->bar_idx;
        ret |= (uint32_t)oal_pci_write_config_dword(pst_pci_dev,
                                                    hi_pci_iatu_region_ctrl_2_off_inbound_i(
                                                        HI_PCI_IATU_BOUND_BASE_OFF),
                                                    ctr2.AsDword);

        /* Host侧64位地址的低32位地址 */
        start.addr = region_base->bus_addr;
        pci_print_log(PCI_LOG_INFO, "PCIe inbound bus addr:0x%llx", start.addr);
        ret |= (uint32_t)oal_pci_write_config_dword(pst_pci_dev,
                                                    hi_pci_iatu_lwr_base_addr_off_inbound_i(
                                                        HI_PCI_IATU_BOUND_BASE_OFF),
                                                    start.bits.low_addr);
        ret |= (uint32_t)oal_pci_write_config_dword(pst_pci_dev,
                                                    hi_pci_iatu_upper_base_addr_off_inbound_i(
                                                        HI_PCI_IATU_BOUND_BASE_OFF),
                                                    start.bits.high_addr);

        end.addr = start.addr + region_base->size - 1;
        if (start.bits.high_addr != end.bits.high_addr) {
            /* 如果跨了4G地址应该多配置一个iatu表项，待增加 */
            pci_print_log(PCI_LOG_ERR, "iatu high 32 bits must same![start:0x%llx, end:0x%llx]", start.addr, end.addr);
            return -OAL_EIO;
        }
        ret |= (uint32_t)oal_pci_write_config_dword(pst_pci_dev,
                                                    hi_pci_iatu_limit_addr_off_inbound_i(HI_PCI_IATU_BOUND_BASE_OFF),
                                                    end.bits.low_addr);

        /* Device侧对应的地址(PCI看到的地址) */
        target.addr = region_base->pci_start;
        ret |= (uint32_t)oal_pci_write_config_dword(pst_pci_dev,
                                                    hi_pci_iatu_lwr_target_addr_off_inbound_i(
                                                        HI_PCI_IATU_BOUND_BASE_OFF), target.bits.low_addr);
        ret |= (uint32_t)oal_pci_write_config_dword(pst_pci_dev,
                                                    hi_pci_iatu_upper_target_addr_off_inbound_i(
                                                        HI_PCI_IATU_BOUND_BASE_OFF),
                                                    target.bits.high_addr);
    }

    /* 配置命令寄存器                                                                         */
    /* BIT0 = 1(I/O Space Enable), BIT1 = 1(Memory Space Enable), BIT2 = 1(Bus Master Enable) */
    ret |= (uint32_t)oal_pci_write_config_word(pst_pci_dev, 0x04, 0x7);
    if (ret) {
        pci_print_log(PCI_LOG_ERR, "pci write iatu config failed ret=%d\n", ret);
        return -OAL_EIO;
    }

    if (pci_dbg_condtion()) {
        oal_pcie_iatu_reg_dump(pst_pci_res);
    }
    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_set_inbound_by_membar(oal_pcie_res *pst_pci_res)
{
    void *inbound_addr = NULL;

    int32_t ret = OAL_SUCC;
    edma_paddr_t start, target, end;
    IATU_REGION_CTRL_2_OFF ctr2;
    int32_t index;
    uint32_t region_num;
    oal_pcie_region *region_base;
    oal_pci_dev_stru *pst_pci_dev = pcie_res_to_dev(pst_pci_res);
    region_num = pst_pci_res->regions.region_nums;
    region_base = pst_pci_res->regions.pst_regions;

    if (pst_pci_res->st_iatu_bar.st_region.vaddr == NULL) {
        pci_print_log(PCI_LOG_ERR, "iatu bar1 vaddr is null");
        return -OAL_ENOMEM;
    }

    inbound_addr = pst_pci_res->st_iatu_bar.st_region.vaddr;

    for (index = 0; index < region_num; index++, region_base++) {
        if (index >= 16) { /* 设置的大小为0x2000，一次偏移0x200，这里16代表设置了所有空间 */
            pci_print_log(PCI_LOG_ERR, "iatu regions too many, start:0x%llx", region_base->bar_info->start);
            break;
        }

        oal_writel(0x0, inbound_addr + hi_pci_iatu_region_ctrl_1_off_inbound_i(hi_pci_iatu_inbound_base_off(index)));

        ctr2.AsDword = 0;
        ctr2.bits.region_en = 1;
        ctr2.bits.bar_num = region_base->bar_info->bar_idx;
        oal_writel(ctr2.AsDword,
                   inbound_addr + hi_pci_iatu_region_ctrl_2_off_inbound_i(hi_pci_iatu_inbound_base_off(index)));

        /* Host侧64位地址的低32位地址 */
        start.addr = region_base->bus_addr;
        pci_print_log(PCI_LOG_INFO, "PCIe inbound bus addr:0x%llx", start.addr);
        oal_writel(start.bits.low_addr,
                   inbound_addr + hi_pci_iatu_lwr_base_addr_off_inbound_i(hi_pci_iatu_inbound_base_off(index)));
        oal_writel(start.bits.high_addr,
                   inbound_addr + hi_pci_iatu_upper_base_addr_off_inbound_i(hi_pci_iatu_inbound_base_off(index)));

        end.addr = start.addr + region_base->size - 1;
        if (start.bits.high_addr != end.bits.high_addr) {
            /* 如果跨了4G地址应该多配置一个iatu表项，待增加 */
            pci_print_log(PCI_LOG_ERR, "iatu high 32 bits must same![start:0x%llx, end:0x%llx]", start.addr, end.addr);
            return -OAL_EIO;
        }
        oal_writel(end.bits.low_addr,
                   inbound_addr + hi_pci_iatu_limit_addr_off_inbound_i(hi_pci_iatu_inbound_base_off(index)));

        /* Device侧对应的地址(PCI看到的地址) */
        target.addr = region_base->pci_start;
        oal_writel(target.bits.low_addr,
                   inbound_addr + hi_pci_iatu_lwr_target_addr_off_inbound_i(hi_pci_iatu_inbound_base_off(index)));
        oal_writel(target.bits.high_addr,
                   inbound_addr + hi_pci_iatu_upper_target_addr_off_inbound_i(hi_pci_iatu_inbound_base_off(index)));
    }

    if (index) {
        /* 回读可以保证之前的IATU立刻生效 */
        uint32_t callback_read;
        callback_read = oal_readl(inbound_addr +
                                  hi_pci_iatu_region_ctrl_1_off_inbound_i(hi_pci_iatu_inbound_base_off(0)));
        oal_reference(callback_read);
    }

    /* 配置命令寄存器                                                                         */
    /* BIT0 = 1(I/O Space Enable), BIT1 = 1(Memory Space Enable), BIT2 = 1(Bus Master Enable) */
    ret = oal_pci_write_config_word(pst_pci_dev, 0x04, 0x7);
    if (ret) {
        pci_print_log(PCI_LOG_ERR, "pci write iatu config failed ret=%d\n", ret);
        return -OAL_EIO;
    }

    if (pci_dbg_condtion()) {
        oal_pcie_iatu_reg_dump(pst_pci_res);
    }

    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_set_inbound(oal_pcie_res *pst_pci_res)
{
    if (pst_pci_res->chip_info.membar_support == OAL_FALSE) {
        return oal_pcie_set_inbound_by_viewport(pst_pci_res);
    } else {
        return oal_pcie_set_inbound_by_membar(pst_pci_res);
    }
}

void oal_pcie_iatu_outbound_dump_by_membar(const void *outbound_addr, uint32_t index)
{
    if (outbound_addr == NULL) {
        pci_print_log(PCI_LOG_ERR, "iatu bar1 vaddr is null");
        return;
    }

    pci_print_log(PCI_LOG_INFO, "OUTBOUND iatu index:%d 's register:\n", index);
    pci_print_log(PCI_LOG_INFO, "%s : 0x%x", "hi_pci_iatu_region_ctrl_1_off_outbound_i",
                  oal_readl(outbound_addr
                  + hi_pci_iatu_region_ctrl_1_off_outbound_i(hi_pci_iatu_outbound_base_off(index))));
    pci_print_log(PCI_LOG_INFO, "%s : 0x%x", "hi_pci_iatu_region_ctrl_2_off_outbound_i",
                  oal_readl(outbound_addr + hi_pci_iatu_region_ctrl_2_off_outbound_i(
                      hi_pci_iatu_outbound_base_off(index))));
    pci_print_log(PCI_LOG_INFO, "%s : 0x%x", "hi_pci_iatu_lwr_base_addr_off_outbound_i",
                  hi_pci_iatu_lwr_base_addr_off_outbound_i(hi_pci_iatu_outbound_base_off(index)));
    pci_print_log(PCI_LOG_INFO, "%s : 0x%x", "hi_pci_iatu_upper_base_addr_off_outbound_i",
                  hi_pci_iatu_upper_base_addr_off_outbound_i(hi_pci_iatu_outbound_base_off(index)));
    pci_print_log(PCI_LOG_INFO, "%s : 0x%x", "hi_pci_iatu_limit_addr_off_outbound_i",
                  hi_pci_iatu_limit_addr_off_outbound_i(hi_pci_iatu_outbound_base_off(index)));
    pci_print_log(PCI_LOG_INFO, "%s : 0x%x", "hi_pci_iatu_lwr_target_addr_off_outbound_i",
                  hi_pci_iatu_lwr_target_addr_off_outbound_i(hi_pci_iatu_outbound_base_off(index)));
    pci_print_log(PCI_LOG_INFO, "%s : 0x%x", "hi_pci_iatu_upper_target_addr_off_outbound_i",
                  hi_pci_iatu_upper_target_addr_off_outbound_i(hi_pci_iatu_outbound_base_off(index)));
}

int32_t oal_pcie_set_outbound_iatu_by_membar(void* pst_bar1_vaddr, uint32_t index, uint64_t src_addr,
                                             uint64_t dst_addr, uint64_t limit_size)
{
    /* IATU 对齐要求,开始结束地址按照4K对齐 */
    IATU_REGION_CTRL_1_OFF ctr1;
    IATU_REGION_CTRL_2_OFF ctr2;
    void *bound_addr = pst_bar1_vaddr;

    uint32_t src_addrl, src_addrh, value;
    uint32_t dst_addrl, dst_addrh;
    uint64_t limit_addr = src_addr + limit_size - 1;

    if (oal_warn_on(pst_bar1_vaddr == NULL)) {
        pci_print_log(PCI_LOG_ERR, "pst_bar1_vaddr is null");
        return -OAL_ENODEV;
    }

    src_addrl = (uint32_t)src_addr;
    src_addrh = (uint32_t)(src_addr >> 32); /* 32 high bits */
    dst_addrl = (uint32_t)dst_addr;
    dst_addrh = (uint32_t)(dst_addr >> 32); /* 32 high bits */

    ctr1.AsDword = 0;

    if (limit_addr >> 32) { // 32 hight bits
        ctr1.bits.inc_region_size = 1; /* more than 4G */
    }

    ctr2.AsDword = 0;
    ctr2.bits.region_en = 1;
    ctr2.bits.bar_num = 0x0;

    oal_writel(ctr1.AsDword, bound_addr +
               hi_pci_iatu_region_ctrl_1_off_outbound_i(hi_pci_iatu_outbound_base_off(index)));
    oal_writel(ctr2.AsDword, bound_addr +
               hi_pci_iatu_region_ctrl_2_off_outbound_i(hi_pci_iatu_outbound_base_off(index)));

    oal_writel(src_addrl, bound_addr +
               hi_pci_iatu_lwr_base_addr_off_outbound_i(hi_pci_iatu_outbound_base_off(index)));
    oal_writel(src_addrh, bound_addr +
               hi_pci_iatu_upper_base_addr_off_outbound_i(hi_pci_iatu_outbound_base_off(index)));

    oal_writel((uint32_t)limit_addr, bound_addr +
               hi_pci_iatu_limit_addr_off_outbound_i(hi_pci_iatu_outbound_base_off(index)));
    // 32 hight bits
    oal_writel((uint32_t)(limit_addr >> 32), bound_addr +
               hi_pci_iatu_uppr_limit_addr_off_outbound_i(hi_pci_iatu_outbound_base_off(index)));

    oal_writel(dst_addrl, bound_addr +
               hi_pci_iatu_lwr_target_addr_off_outbound_i(hi_pci_iatu_outbound_base_off(index)));
    oal_writel(dst_addrh,
               bound_addr + hi_pci_iatu_upper_target_addr_off_outbound_i(hi_pci_iatu_outbound_base_off(index)));

    value = oal_readl(bound_addr + hi_pci_iatu_lwr_base_addr_off_outbound_i(hi_pci_iatu_outbound_base_off(index)));

    if (value != src_addrl) {
        pci_print_log(PCI_LOG_ERR,
                      "callback read 0x%x failed src_addr=0x%llx, dst_addr=0x%llx limit=0x%llx, index = %u",
                      value, src_addr, dst_addr,  limit_addr, index);
        oal_pcie_iatu_outbound_dump_by_membar(pst_bar1_vaddr, index);
        return -OAL_EFAIL;
    } else {
        pci_print_log(PCI_LOG_INFO, "outbound  src_addr=0x%llx, dst_addr=0x%llx limit=0x%llx, index = %u",
                      src_addr, dst_addr, limit_addr, index);
        return OAL_SUCC;
    }
}

#define HISI_PCIE_SLAVE_START_ADDRESS        (0x80000000)
#define HISI_PCIE_SLAVE_END_ADDRESS          (0xFFFFFFFFFFFFFFFF)
#define HISI_PCIE_IP_REGION_SIZE             (HISI_PCIE_SLAVE_END_ADDRESS - HISI_PCIE_SLAVE_START_ADDRESS + 1)
#define HISI_PCIE_MASTER_START_ADDRESS       (0x0)
#define HISI_PCIE_MASTER_END_ADDRESS         (HISI_PCIE_MASTER_START_ADDRESS + HISI_PCIE_IP_REGION_SIZE - 1)
#define HISI_PCIE_IP_REGION_OFFSET           (HISI_PCIE_SLAVE_START_ADDRESS - HISI_PCIE_MASTER_START_ADDRESS)

int32_t oal_pcie_set_outbound_membar(oal_pcie_iatu_bar* pst_iatu_bar)
{
    int32_t ret = OAL_SUCC;
    ret = oal_pcie_set_outbound_iatu_by_membar(pst_iatu_bar->st_region.vaddr,
                                               0, HISI_PCIE_SLAVE_START_ADDRESS,
                                               HISI_PCIE_MASTER_START_ADDRESS,
                                               HISI_PCIE_IP_REGION_SIZE);
    if (ret) {
        return ret;
    }
    return OAL_SUCC;
}

int32_t oal_pcie_set_outbound_by_membar(oal_pcie_res *pst_pci_res)
{
    oal_pci_dev_stru *pst_pci_dev = pcie_res_to_dev(pst_pci_res);

    if (pst_pci_dev->device != PCIE_HISI_DEVICE_ID_HI1106) {
        return OAL_SUCC;
    }

    return oal_pcie_set_outbound_membar(&pst_pci_res->st_iatu_bar);
}

int32_t oal_pcie_host_slave_address_switch(oal_pcie_res *pst_pci_res, uint64_t src_addr,
                                           uint64_t* dst_addr, int32_t is_host_iova)
{
    if (is_host_iova == OAL_TRUE) {
        if (oal_likely((src_addr < (HISI_PCIE_MASTER_END_ADDRESS)))) {
            *dst_addr = src_addr + HISI_PCIE_IP_REGION_OFFSET;
            pci_print_log(PCI_LOG_DBG, "pcie_if_hostca_to_devva ok, hostca=0x%llx\n", *dst_addr);
            return OAL_SUCC;
        }
    } else {
        if (oal_likely((((src_addr >= HISI_PCIE_SLAVE_START_ADDRESS)
                       && (src_addr < (HISI_PCIE_SLAVE_END_ADDRESS)))))) {
            *dst_addr = src_addr - HISI_PCIE_IP_REGION_OFFSET;
            pci_print_log(PCI_LOG_DBG, "pcie_if_devva_to_hostca ok, devva=0x%llx\n", *dst_addr);
            return OAL_SUCC;
        }
    }

    pci_print_log(PCI_LOG_ERR, "pcie_slave_address_switch %s failed, src_addr=0x%llx\n",
                  (is_host_iova == OAL_TRUE) ? "iova->slave" : "slave->iova", src_addr);
    return -OAL_EFAIL;
}

#ifdef _PRE_SDIO_TAE_DEBUG
    extern int32_t g_sdio_tae_debug_enable;
#endif
hcc_bus * oal_pcie_get_current_bus(void)
{
    uint32_t dev_id = HCC_CHIP_110X_DEV;
    hcc_bus *pst_bus = NULL;
    hcc_bus_dev *pst_bus_dev = hcc_get_bus_dev(dev_id);
#ifdef _PRE_SDIO_TAE_DEBUG
    if (g_sdio_tae_debug_enable) {
        pst_bus = hcc_find_bus_by_devid(dev_id, HCC_BUS_PCIE);
    } else {
#endif
        if (pst_bus_dev == NULL) {
            return NULL;
        }
        pst_bus = pst_bus_dev->cur_bus;
#ifdef _PRE_SDIO_TAE_DEBUG
    }
#endif

    if (hcc_bus_isvalid(pst_bus) != OAL_SUCC) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "bus %d is not ready", HCC_BUS_PCIE);
        return NULL;
    }

    return pst_bus;
}

oal_bool_enum_uint8 oal_pcie_link_state_up(void)
{
    hcc_bus *hi_bus = oal_pcie_get_current_bus();
    oal_pcie_res *g_pci_res = NULL;
    oal_pcie_linux_res *pci_lres = NULL;

    if (hi_bus == NULL) {
        return OAL_FALSE;
    }

    pci_lres = (oal_pcie_linux_res *)hi_bus->data;
    if (pci_lres == NULL) {
        return OAL_FALSE;
    }

    g_pci_res = pci_lres->pst_pci_res;
    if (g_pci_res == NULL) {
        return OAL_FALSE;
    }

    return g_pci_res->link_state == PCI_WLAN_LINK_WORK_UP;
}
oal_module_symbol(oal_pcie_link_state_up);

/* PCIE EP Master 口访问,主芯片CPU访问片内地址空间(注意不能访问片内PCIE Slave口空间) for WiFi TAE
 *    将devcpu看到的地址转换成host侧看到的物理地址和虚拟地址 */
int32_t oal_pcie_devca_to_hostva(uint32_t ul_chip_id, uint64_t dev_cpuaddr, uint64_t *host_va)
{
    oal_reference(ul_chip_id);
    return hcc_bus_master_address_switch(oal_pcie_get_current_bus(), dev_cpuaddr, host_va, MASTER_WCPUADDR_TO_VA);
}
oal_module_symbol(oal_pcie_devca_to_hostva);

/* 虚拟地址转换成WCPU看到的地址 */
int32_t oal_pcie_get_dev_ca(uint32_t ul_chip_id, void *host_va, uint64_t* dev_cpuaddr)
{
    oal_reference(ul_chip_id);
    return hcc_bus_master_address_switch(oal_pcie_get_current_bus(),
                                         (uint64_t)(uintptr_t)host_va, dev_cpuaddr, MASTER_VA_TO_WCPUADDR);
}
oal_module_symbol(oal_pcie_get_dev_ca);

/* PCIE EP Slave 口看到的地址转换 --片内外设通过PCIE访问主芯片DDR空间 */
/* 地址转换主芯片的DDR设备地址转换成device的SLAVE口地址
 *  注意这里的DDR设备地址(iova)不是的直接物理地址，而是对应Kernel的DMA地址
 *  设备地址不能通过phys_to_virt/virt_to_phys直接转换
 */
int32_t pcie_if_hostca_to_devva(uint32_t ul_chip_id, uint64_t host_iova, uint64_t *addr)
{
    oal_reference(ul_chip_id);
    return hcc_bus_slave_address_switch(oal_pcie_get_current_bus(), host_iova, addr, SLAVE_IOVA_TO_PCI_SLV);
}
EXPORT_SYMBOL_GPL(pcie_if_hostca_to_devva);

/* 地址转换主芯片的DDR设备地址转换成device的SLAVE口地址 */
int32_t pcie_if_devva_to_hostca(uint32_t ul_chip_id, uint64_t devva, uint64_t *host_iova)
{
    oal_reference(ul_chip_id);
    return hcc_bus_slave_address_switch(oal_pcie_get_current_bus(), devva, host_iova, SLAVE_PCI_SLV_TO_IOVA);
}
EXPORT_SYMBOL_GPL(pcie_if_devva_to_hostca);

/* set ep outbound, device->host */
OAL_STATIC int32_t oal_pcie_set_outbound(oal_pcie_res *pst_pci_res)
{
    if (pst_pci_res->chip_info.membar_support == OAL_TRUE) {
        return oal_pcie_set_outbound_by_membar(pst_pci_res);
    }
    return OAL_SUCC;
}

OAL_STATIC void oal_pcie_iatu_exit(oal_pcie_res *pst_pci_res)
{
}

int32_t oal_pcie_iatu_init(oal_pcie_res *pst_pci_res)
{
    int32_t ret;

    if (pst_pci_res == NULL) {
        return -OAL_ENODEV;
    }

    if (!pst_pci_res->regions.inited) {
        pci_print_log(PCI_LOG_ERR, "pcie regions is disabled, iatu config failed");
        return -OAL_EIO;
    }

    ret = oal_pcie_set_inbound(pst_pci_res);
    if (ret != OAL_SUCC) {
        pci_print_log(PCI_LOG_ERR, "pcie inbound set failed ret=%d\n", ret);
        return ret;
    }

    ret = oal_pcie_set_outbound(pst_pci_res);
    if (ret != OAL_SUCC) {
        pci_print_log(PCI_LOG_ERR, "pcie outbound set failed ret=%d\n", ret);
        return ret;
    }

    /* mem方式访问使能 */
    oal_pcie_change_link_state(pst_pci_res, PCI_WLAN_LINK_MEM_UP);
    return OAL_SUCC;
}

OAL_STATIC void oal_pcie_ctrl_base_address_exit(oal_pcie_res *pst_pci_res)
{
    pst_pci_res->pst_pci_dma_ctrl_base = NULL;
    pst_pci_res->pst_pci_ctrl_base = NULL;
    pst_pci_res->pst_pci_dbi_base = NULL;
    pst_pci_res->pst_ete_base = NULL;
}

OAL_STATIC int32_t oal_pcie_ctrl_base_address_init(oal_pcie_res *pst_pci_res)
{
    int32_t ret;
    pci_addr_map st_map;

    ret = oal_pcie_inbound_ca_to_va(pst_pci_res, pst_pci_res->chip_info.addr_info.pcie_ctrl, &st_map);
    if (ret != OAL_SUCC) {
        pci_print_log(PCI_LOG_ERR, "get dev address 0x%x failed", pst_pci_res->chip_info.addr_info.pcie_ctrl);
        oal_pcie_regions_info_dump(pst_pci_res);
        return -OAL_EFAIL;
    }
    pst_pci_res->pst_pci_ctrl_base = (void *)st_map.va;


    if (pst_pci_res->chip_info.edma_support == OAL_TRUE) {
        ret = oal_pcie_inbound_ca_to_va(pst_pci_res, pst_pci_res->chip_info.addr_info.edma_ctrl, &st_map);
        if (ret != OAL_SUCC) {
            pci_print_log(PCI_LOG_ERR, "get dev address 0x%x failed", pst_pci_res->chip_info.addr_info.edma_ctrl);
            oal_pcie_regions_info_dump(pst_pci_res);
            oal_pcie_ctrl_base_address_exit(pst_pci_res);
            return -OAL_EFAIL;
        }
        pst_pci_res->pst_pci_dma_ctrl_base = (void *)st_map.va;
    }

    /* dbi base */
    ret = oal_pcie_inbound_ca_to_va(pst_pci_res, pst_pci_res->chip_info.addr_info.dbi, &st_map);
    if (ret != OAL_SUCC) {
        pci_print_log(PCI_LOG_ERR, "get dev address 0x%x failed", PCIE_CONFIG_BASE_ADDRESS);
        oal_pcie_regions_info_dump(pst_pci_res);
        oal_pcie_ctrl_base_address_exit(pst_pci_res);
        return -OAL_EFAIL;
    }
    pst_pci_res->pst_pci_dbi_base = (void *)st_map.va;

    if (pst_pci_res->chip_info.ete_support == OAL_TRUE) {
        /* ete base */
        ret = oal_pcie_inbound_ca_to_va(pst_pci_res, pst_pci_res->chip_info.addr_info.ete_ctrl, &st_map);
        if (ret != OAL_SUCC) {
            pci_print_log(PCI_LOG_ERR, "get dev address 0x%x failed", PCIE_CONFIG_BASE_ADDRESS);
            oal_pcie_regions_info_dump(pst_pci_res);
            oal_pcie_ctrl_base_address_exit(pst_pci_res);
            return -OAL_EFAIL;
        }
        pst_pci_res->pst_ete_base = (void *)st_map.va;
    }

    pci_print_log(PCI_LOG_DBG, "ctrl base addr init succ, pci va:0x%p, fifo va:0x%p, dbi va:0x%p, ete va:0x%p",
                  pst_pci_res->pst_pci_ctrl_base, pst_pci_res->pst_pci_dma_ctrl_base, pst_pci_res->pst_pci_dbi_base,
                  pst_pci_res->pst_ete_base);
    return OAL_SUCC;

}

OAL_STATIC void oal_pcie_regions_exit(oal_pcie_res *pst_pci_res)
{
    int32_t index;
    uint32_t region_num;
    oal_pcie_region *region_base = NULL;
    pci_print_log(PCI_LOG_INFO, "oal_pcie_regions_exit\n");
    pst_pci_res->regions.inited = 0;

    region_num = pst_pci_res->regions.region_nums;
    region_base = pst_pci_res->regions.pst_regions;

    /* 释放申请的地址空间 */
    for (index = 0; index < region_num; index++, region_base++) {
        if (region_base->vaddr != NULL) {
            oal_iounmap(region_base->vaddr);
            oal_release_mem_region(region_base->paddr, region_base->size);
            region_base->vaddr = NULL;
        }
    }
}

void oal_pcie_bar1_exit(oal_pcie_iatu_bar *pst_iatu_bar)
{
    if (pst_iatu_bar->st_region.vaddr == NULL) {
        return;
    }

    oal_iounmap(pst_iatu_bar->st_region.vaddr);
    oal_release_mem_region(pst_iatu_bar->st_region.paddr, pst_iatu_bar->st_region.size);
    pst_iatu_bar->st_region.vaddr = NULL;
}

OAL_STATIC void oal_pcie_iatu_bar_exit(oal_pcie_res *pst_pci_res)
{
    oal_pcie_bar1_exit(&pst_pci_res->st_iatu_bar);
}

int32_t oal_pcie_bar1_init(oal_pcie_iatu_bar* pst_iatu_bar)
{
    oal_resource *pst_res = NULL;
    oal_pcie_bar_info *bar_base = NULL;
    oal_pcie_region *region_base = NULL;

    if (pst_iatu_bar->st_bar_info.size == 0) {
        return OAL_SUCC;
    }

    bar_base = &pst_iatu_bar->st_bar_info;
    region_base = &pst_iatu_bar->st_region;

    /* Bar1 专门用于配置 iatu表 */
    region_base->vaddr = NULL;            /* remap 后的虚拟地址 */
    region_base->paddr = bar_base->start; /* Host CPU看到的物理地址 */
    region_base->bus_addr = 0x0;
    region_base->res = NULL;
    region_base->bar_info = bar_base;
    region_base->size = bar_base->size;
    region_base->name = "iatu_bar1";
    region_base->flag = OAL_IORESOURCE_REG;

    pst_res = oal_request_mem_region(region_base->paddr, region_base->size, region_base->name);
    if (pst_res == NULL) {
        goto failed_request_region;
    }

    /* remap */
    if (region_base->flag & OAL_IORESOURCE_REG) {
        /* 寄存器映射成非cache段, 不需要刷cache */
        region_base->vaddr = oal_ioremap_nocache(region_base->paddr, region_base->size);

    } else {
        /* cache 段，注意要刷cache */
        region_base->vaddr = oal_ioremap(region_base->paddr, region_base->size);
    }

    if (region_base->vaddr == NULL) {
        oal_release_mem_region(region_base->paddr, region_base->size);
        goto failed_remap;
    }

    /* remap and request succ. */
    region_base->res = pst_res;

    pci_print_log(PCI_LOG_INFO, "iatu bar1 virtual address:%p", region_base->vaddr);

    return OAL_SUCC;
failed_remap:
    oal_iounmap(region_base->vaddr);
    oal_release_mem_region(region_base->paddr, region_base->size);
    region_base->vaddr = NULL;
failed_request_region:
    return -OAL_ENOMEM;
}

OAL_STATIC int32_t oal_pcie_iatu_bar_init(oal_pcie_res *pst_pci_res)
{
    return oal_pcie_bar1_init(&pst_pci_res->st_iatu_bar);
}

OAL_STATIC int32_t oal_pcie_regions_init(oal_pcie_res *pst_pci_res)
{
    /* 初始化DEVICE 每个段分配的HOST物理地址，然后做remap */
    void *vaddr = NULL;
    int32_t index, region_idx, bar_used_size;
    uint32_t bar_num, region_num;
    oal_pcie_bar_info *bar_base = NULL;
    oal_pcie_region *region_base = NULL;
    oal_resource *pst_res = NULL;

    if (oal_warn_on(pst_pci_res->regions.inited)) {
        /* 不能重复初始化 */
        return -OAL_EBUSY;
    }

    bar_num = pst_pci_res->regions.bar_nums;
    region_num = pst_pci_res->regions.region_nums;

    bar_base = pst_pci_res->regions.pst_bars;
    region_base = pst_pci_res->regions.pst_regions;

    /* 清空regions的特定字段 */
    for (index = 0; index < region_num; index++, region_base++) {
        region_base->vaddr = NULL; /* remap 后的虚拟地址 */
        region_base->paddr = 0x0;  /* Host CPU看到的物理地址 */
        region_base->bus_addr = 0x0;
        region_base->res = NULL;
        region_base->bar_info = NULL;
        region_base->size = region_base->pci_end - region_base->pci_start + 1;
    }

    region_idx = 0;
    bar_used_size = 0;
    bar_base = pst_pci_res->regions.pst_bars;
    region_base = pst_pci_res->regions.pst_regions;

    for (index = 0; index < bar_num; index++, bar_base++) {
        for (; region_idx < region_num; region_idx++, region_base++) {
            /* BAR可用的起始地址 */
            if (bar_base->start + bar_used_size + region_base->size - 1 > bar_base->end) {
                /* 这个BAR地址空间不足 */
                pci_print_log(PCI_LOG_ERR,
                              "index:%d,region_idx:%d, start:0x%llx ,end:0x%llx, used_size:0x%x, region_size:%u\n",
                              index, region_idx, bar_base->start, bar_base->end, bar_used_size, region_base->size);
                break;
            }

            region_base->paddr = bar_base->start + bar_used_size;
            region_base->bus_addr = bar_base->bus_start + bar_used_size;
            bar_used_size += region_base->size;
            region_base->bar_info = bar_base;
            pci_print_log(PCI_LOG_INFO, "bar idx:%d, region idx:%d, region paddr:0x%llx, region_size:%u\n",
                          index, region_idx, region_base->paddr, region_base->size);
        }
    }

    if (region_idx < region_num) {
        /* 地址不够用 */
        pci_print_log(PCI_LOG_ERR, "bar address range is too small, region_idx %d < region_num %d\n",
                      region_idx, region_num);
        return -OAL_ENOMEM;
    }

    pci_print_log(PCI_LOG_INFO, "Total region num:%d, size:%d\n", region_num, bar_used_size);

    region_base = pst_pci_res->regions.pst_regions;
    for (index = 0; index < region_num; index++, region_base++) {
        if (!region_base->flag) {
            continue;
        }

        pst_res = oal_request_mem_region(region_base->paddr, region_base->size, region_base->name);
        if (pst_res == NULL) {
            goto failed_remap;
        }

        /* remap */
        if (region_base->flag & OAL_IORESOURCE_REG) {
            /* 寄存器映射成非cache段, 不需要刷cache */
            vaddr = oal_ioremap_nocache(region_base->paddr, region_base->size);

        } else {
            /* cache 段，注意要刷cache */
            vaddr = oal_ioremap(region_base->paddr, region_base->size);
        }

        if (vaddr == NULL) {
            oal_release_mem_region(region_base->paddr, region_base->size);
            goto failed_remap;
        }

        /* remap and request succ. */
        region_base->res = pst_res;
        region_base->vaddr = vaddr; /* Host Cpu 可以访问的虚拟地址 */
    }

    oal_pcie_enable_regions(pst_pci_res);

    pci_print_log(PCI_LOG_INFO, "oal_pcie_regions_init succ\n");
    return OAL_SUCC;
failed_remap:
    pci_print_log(PCI_LOG_ERR, "request mem region failed, addr:0x%llx, size:%u, name:%s\n",
                  region_base->paddr, region_base->size, region_base->name);
    oal_pcie_regions_exit(pst_pci_res);
    return -OAL_EIO;
}

OAL_STATIC void oal_pcie_bar_exit(oal_pcie_res *pst_pci_res)
{
    oal_pcie_iatu_exit(pst_pci_res);
    oal_pcie_regions_exit(pst_pci_res);
    oal_pcie_iatu_bar_exit(pst_pci_res);
}

OAL_STATIC int32_t oal_pcie_get_ca_by_pa(oal_pcie_res *pst_pci_res, uintptr_t paddr, uint64_t *cpuaddr)
{
    int32_t index;
    uint32_t region_num;
    uint64_t offset;
    oal_pcie_region *region_base;
    uintptr_t end;

    region_num = pst_pci_res->regions.region_nums;
    region_base = pst_pci_res->regions.pst_regions;

    if (cpuaddr == NULL) {
        return -OAL_EINVAL;
    }

    *cpuaddr = 0;

    if (oal_warn_on(!pst_pci_res->regions.inited)) {
        return -OAL_ENODEV;
    }

    for (index = 0; index < region_num; index++, region_base++) {
        if (region_base->vaddr == NULL) {
            continue;
        }

        end = (uintptr_t)region_base->paddr + region_base->size - 1;

        if ((paddr >= (uintptr_t)region_base->paddr) && (paddr <= end)) {
            /* 地址在范围内 */
            offset = paddr - (uintptr_t)region_base->paddr;
            *cpuaddr = region_base->cpu_start + offset;
            return OAL_SUCC;
        } else {
            continue;
        }
    }

    return -OAL_ENOMEM;
}

/*
 * 将Device Cpu看到的地址转换为 Host侧的虚拟地址,
 * 虚拟地址返回NULL为无效地址，Device Cpu地址有可能为0,
 * local ip inbound cpu address to host virtual address,
 * 函数返回非0为失败
 */
int32_t oal_pcie_inbound_ca_to_va(oal_pcie_res *pst_pci_res, uint64_t dev_cpuaddr,
                                  pci_addr_map *addr_map)
{
    int32_t index;
    uint32_t region_num;
    uint64_t offset;
    oal_pcie_region *region_base;

    region_num = pst_pci_res->regions.region_nums;
    region_base = pst_pci_res->regions.pst_regions;

    if (addr_map != NULL) {
        addr_map->pa = 0;
        addr_map->va = 0;
    }

    if (oal_unlikely(pst_pci_res->link_state < PCI_WLAN_LINK_MEM_UP)) {
        pci_print_log(PCI_LOG_WARN, "addr request 0x%llx failed, link_state:%s",
                      dev_cpuaddr, oal_pcie_get_link_state_str(pst_pci_res->link_state));
        return -OAL_EBUSY;
    }

    if (oal_warn_on(!pst_pci_res->regions.inited)) {
        return -OAL_ENODEV;
    }

    for (index = 0; index < region_num; index++, region_base++) {
        if (region_base->vaddr == NULL) {
            continue;
        }

        if ((dev_cpuaddr >= region_base->cpu_start) && (dev_cpuaddr <= region_base->cpu_end)) {
            /* 地址在范围内 */
            offset = dev_cpuaddr - region_base->cpu_start;
            if (addr_map != NULL) {
                /* 返回HOST虚拟地址 */
                addr_map->va = (uintptr_t)(region_base->vaddr + offset);
                /* 返回HOST物理地址 */
                addr_map->pa = (uintptr_t)(region_base->paddr + offset);
            }
            return OAL_SUCC;
        } else {
            continue;
        }
    }

    return -OAL_ENOMEM;
}

/* 地址转换均为线性映射，注意起始地址和大小 */
int32_t oal_pcie_inbound_va_to_ca(oal_pcie_res *pst_pci_res, uint64_t host_va, uint64_t *dev_cpuaddr)
{
    int32_t index;
    uint32_t region_num;
    uint64_t offset;
    oal_pcie_region *region_base;
    uint64_t end;
    uint64_t vaddr = host_va;

    region_num = pst_pci_res->regions.region_nums;
    region_base = pst_pci_res->regions.pst_regions;

    if (oal_warn_on(dev_cpuaddr == NULL)) {
        return -OAL_EINVAL;
    }

    *dev_cpuaddr = 0;

    if (oal_warn_on(!pst_pci_res->regions.inited)) {
        return -OAL_ENODEV;
    }

    for (index = 0; index < region_num; index++, region_base++) {
        if (region_base->vaddr == NULL) {
            continue;
        }

        end = (uintptr_t)region_base->vaddr + region_base->size - 1;

        if ((vaddr >= (uintptr_t)region_base->vaddr) && (vaddr <= end)) {
            /* 地址在范围内 */
            offset = vaddr - (uintptr_t)region_base->vaddr;
            *dev_cpuaddr = region_base->cpu_start + offset;
            return OAL_SUCC;
        } else {
            continue;
        }
    }

    return -OAL_ENOMEM;
}

/* 检查通过PCIE操作的HOST侧虚拟地址是否合法 ，是否映射过 */
int32_t oal_pcie_vaddr_isvalid(oal_pcie_res *pst_pci_res, const void *vaddr)
{
    int32_t index;
    uint32_t region_num;
    oal_pcie_region *region_base = NULL;
    if (oal_warn_on(!pst_pci_res->regions.inited)) {
        return OAL_FALSE;
    }

    region_num = pst_pci_res->regions.region_nums;
    region_base = pst_pci_res->regions.pst_regions;

    for (index = 0; index < region_num; index++, region_base++) {
        if (region_base->vaddr == NULL) {
            continue;
        }

        if (((uintptr_t)vaddr >= (uintptr_t)region_base->vaddr)
            && ((uintptr_t)vaddr < (uintptr_t)region_base->vaddr + region_base->size)) {
            return OAL_TRUE;
        }
    }

    return OAL_FALSE;
}

OAL_STATIC int32_t oal_pcie_get_bar_region_info(oal_pcie_res *pst_pci_res,
                                                oal_pcie_region **region_base, uint32_t *region_num)
{
    oal_reference(pst_pci_res);

    if (get_hi110x_subchip_type() == BOARD_VERSION_HI1105) {
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "1105 %s region map", hi110x_is_asic() ? "asic" : "fpga");
        if (hi110x_is_asic()) {
            /* 1105 Asic */
            *region_num = oal_array_size(g_hi1105_pcie_asic_regions);
            *region_base = &g_hi1105_pcie_asic_regions[0];
        } else {
            /* 1105 Fpga */
            *region_num = oal_array_size(g_hi1105_pcie_fpga_regions);
            *region_base = &g_hi1105_pcie_fpga_regions[0];
        }
    } else if (get_hi110x_subchip_type() == BOARD_VERSION_HI1106) {
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "1106 %s region map", hi110x_is_asic() ? "asic" : "fpga");
        if (hi110x_is_asic()) {
            /* 1106 Asic, need add bfg map */
            *region_num = oal_array_size(g_hi1106_pcie_asic_regions);
            *region_base = &g_hi1106_pcie_asic_regions[0];
        } else {
            /* 1106 Fpga */
            *region_num = oal_array_size(g_hi1106_pcie_fpga_regions);
            *region_base = &g_hi1106_pcie_fpga_regions[0];
        }
    } else {
        if (pst_pci_res->revision == PCIE_REVISION_4_70A) {
            *region_num = oal_array_size(g_hi1103_pcie_mpw2_regions);
            *region_base = &g_hi1103_pcie_mpw2_regions[0];
        } else if (pst_pci_res->revision == PCIE_REVISION_5_00A) {
            *region_num = oal_array_size(g_hi1103_pcie_pilot_regions);
            *region_base = &g_hi1103_pcie_pilot_regions[0];
        } else {
            pci_print_log(PCI_LOG_ERR, "unkown pcie ip revision :0x%x\n", pst_pci_res->revision);
            return -OAL_ENODEV;
        }
    }

    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_mem_bar_init(oal_pci_dev_stru *pst_pci_dev, oal_pcie_bar_info *bar_curr)
{
    /* Get Bar Address */
    bar_curr->size = oal_pci_resource_len(pst_pci_dev, PCIE_IATU_BAR_INDEX);
    if (bar_curr->size == 0) {
        pci_print_log(PCI_LOG_ERR, "bar 1 size is zero, start:0x%lx, end:0x%lx",
                      oal_pci_resource_start(pst_pci_dev, PCIE_IATU_BAR_INDEX),
                      oal_pci_resource_end(pst_pci_dev, PCIE_IATU_BAR_INDEX));
        return -OAL_EIO;
    }

    bar_curr->end = oal_pci_resource_end(pst_pci_dev, PCIE_IATU_BAR_INDEX);
    bar_curr->start = oal_pci_resource_start(pst_pci_dev, PCIE_IATU_BAR_INDEX);
    bar_curr->bus_start = oal_pci_bus_address(pst_pci_dev, PCIE_IATU_BAR_INDEX);
    bar_curr->bar_idx = PCIE_IATU_BAR_INDEX;

    pci_print_log(PCI_LOG_INFO,
                  "preapre for bar idx:%u, phy start:0x%llx, end:0x%llx, bus address 0x%lx size:0x%x, flags:0x%lx\n",
                  PCIE_IATU_BAR_INDEX,
                  bar_curr->start,
                  bar_curr->end,
                  (uintptr_t)bar_curr->bus_start,
                  bar_curr->size,
                  oal_pci_resource_flags(pst_pci_dev, PCIE_IATU_BAR_INDEX));

    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_bar_init(oal_pcie_res *pst_pci_res)
{
    int32_t index;
    int32_t ret;
    uint32_t bar_num, region_num;
    oal_pcie_bar_info *bar_base = NULL;
    oal_pcie_region *region_base = NULL;

    oal_pci_dev_stru *pst_pci_dev = pcie_res_to_dev(pst_pci_res);

    /* 暂时只考虑1103 */
    bar_num = oal_array_size(g_en_bar_tab);
    bar_base = &g_en_bar_tab[0];

    ret = oal_pcie_get_bar_region_info(pst_pci_res, &region_base, &region_num);
    if (ret != OAL_SUCC) {
        return ret;
    }

    pci_print_log(PCI_LOG_INFO, "bar_num:%u, region_num:%u\n", bar_num, region_num);

    pst_pci_res->regions.pst_bars = bar_base;
    pst_pci_res->regions.bar_nums = bar_num;

    pst_pci_res->regions.pst_regions = region_base;
    pst_pci_res->regions.region_nums = region_num;

    /* 这里不映射，iatu配置要和映射分段对应 */
    for (index = 0; index < bar_num; index++) {
        /*
         * 获取Host分配的硬件地址资源,1103为8M大小,
         * 1103 4.7a 对应一个BAR, 5.0a 对应2个bar,
         * 其中第二个bar用于配置iatu表
         */
        oal_pcie_bar_info *bar_curr = bar_base + index;
        uint8_t bar_idx = bar_curr->bar_idx;

        /* pci resource built in pci_read_bases kernel. */
        bar_curr->start = oal_pci_resource_start(pst_pci_dev, bar_idx);
        bar_curr->end = oal_pci_resource_end(pst_pci_dev, bar_idx);
        bar_curr->bus_start = oal_pci_bus_address(pst_pci_dev, bar_idx);
        bar_curr->size = oal_pci_resource_len(pst_pci_dev, bar_idx);

        pci_print_log(PCI_LOG_INFO,
                      "preapre for bar idx:%u, \
                      phy start:0x%llx, end:0x%llx, bus address 0x%lx size:0x%x, flags:0x%lx\n",
                      bar_idx,
                      bar_curr->start,
                      bar_curr->end,
                      (uintptr_t)bar_curr->bus_start,
                      bar_curr->size,
                      oal_pci_resource_flags(pst_pci_dev, bar_idx));
    }

    /* 是否支持BAR1 */
    if (pst_pci_res->chip_info.membar_support == OAL_TRUE) {
        /* Get Bar Address */
        oal_pcie_bar_info *bar_curr = &pst_pci_res->st_iatu_bar.st_bar_info;
        ret = oal_pcie_mem_bar_init(pst_pci_dev, bar_curr);
        if (ret != OAL_SUCC) {
            return ret;
        }
    }

    ret = oal_pcie_iatu_bar_init(pst_pci_res);
    if (ret != OAL_SUCC) {
        return ret;
    }

    ret = oal_pcie_regions_init(pst_pci_res);
    if (ret != OAL_SUCC) {
        oal_pcie_iatu_bar_exit(pst_pci_res);
        return ret;
    }

    ret = oal_pcie_iatu_init(pst_pci_res);
    if (ret != OAL_SUCC) {
        oal_pcie_regions_exit(pst_pci_res);
        oal_pcie_iatu_bar_exit(pst_pci_res);
        return ret;
    }
    pci_print_log(PCI_LOG_INFO, "bar init succ");
    return OAL_SUCC;
}

#ifdef _PRE_PLAT_FEATURE_HI110X_DUAL_PCIE
int32_t oal_pcie_dual_bar_init(oal_pcie_res *pst_pci_res, oal_pci_dev_stru *pst_pci_dev)
{

    return oal_pcie_mem_bar_init(pst_pci_dev, &pst_pci_res->st_iatu_dual_bar.st_bar_info);
}
#endif

OAL_STATIC int32_t oal_pcie_rx_ringbuf_supply(oal_pcie_res *pst_pci_res,
                                              int32_t is_sync,
                                              int32_t is_doorbell,
                                              uint32_t request_cnt,
                                              int32_t gflag,
                                              int32_t *ret)
{
    uint32_t i;
    uint32_t cnt = 0;
    unsigned long flags;
    oal_netbuf_stru *pst_netbuf = NULL;
    pcie_cb_dma_res *pst_cb_res = NULL;
    dma_addr_t pci_dma_addr;
    pcie_write_ringbuf_item st_write_item;
    oal_pci_dev_stru *pst_pci_dev = pcie_res_to_dev(pst_pci_res);
    *ret = OAL_SUCC;
    oal_pcie_mips_start(PCIE_MIPS_RX_NETBUF_SUPPLY);
    if (is_sync == OAL_TRUE) {
        /* 同步Dev2Host的读指针 */
        oal_pcie_d2h_ringbuf_rd_update(pst_pci_res);
    }

    for (i = 0; i < request_cnt; i++) {
        if (oal_pcie_d2h_ringbuf_freecount(pst_pci_res, OAL_FALSE) == 0) {
            break;
        }
        /* ringbuf 有空间 */
        /* 预申请netbuf都按照大包来申请 */
        oal_pcie_mips_start(PCIE_MIPS_RX_MEM_ALLOC);
        pst_netbuf = oal_pcie_rx_netbuf_alloc(HCC_HDR_TOTAL_LEN + PCIE_EDMA_TRANS_MAX_FRAME_LEN, gflag);
        if (pst_netbuf == NULL) {
            pst_pci_res->st_rx_res.stat.alloc_netbuf_failed++;
            *ret = -OAL_ENOMEM;
            oal_pcie_mips_end(PCIE_MIPS_RX_MEM_ALLOC);
            break;
        }
        oal_netbuf_put(pst_netbuf, (HCC_HDR_TOTAL_LEN + PCIE_EDMA_TRANS_MAX_FRAME_LEN));
        oal_pcie_mips_end(PCIE_MIPS_RX_MEM_ALLOC);

        if (g_pcie_dma_data_check_enable) {
            uint32_t value;
            /* 增加标记，判断DMA是否真的启动 */
            oal_writel(0xffffffff, (void *)oal_netbuf_data(pst_netbuf));
            value = (uint32_t)(uintptr_t)oal_netbuf_data(pst_netbuf) + HCC_HDR_TOTAL_LEN;
            oal_writel(value, ((void *)oal_netbuf_data(pst_netbuf) + HCC_HDR_TOTAL_LEN));
            pci_dma_addr = dma_map_single(&pst_pci_dev->dev, oal_netbuf_data(pst_netbuf),
                                          oal_netbuf_len(pst_netbuf), PCI_DMA_TODEVICE);
        } else {
            pci_dma_addr = dma_map_single(&pst_pci_dev->dev, oal_netbuf_data(pst_netbuf),
                                          oal_netbuf_len(pst_netbuf), PCI_DMA_FROMDEVICE);
        }

        if (dma_mapping_error(&pst_pci_dev->dev, pci_dma_addr)) {
            pst_pci_res->st_rx_res.stat.map_netbuf_failed++;
            pci_print_log(PCI_LOG_INFO, "rx dma map netbuf failed, len=%u,cnt:%u",
                          oal_netbuf_len(pst_netbuf),
                          pst_pci_res->st_rx_res.stat.map_netbuf_failed);
            oal_netbuf_free(pst_netbuf);
            break;
        }

        /* DMA地址填到CB中, CB首地址8字节对齐可以直接强转 */
        pst_cb_res = (pcie_cb_dma_res *)oal_netbuf_cb(pst_netbuf);
        pst_cb_res->paddr.addr = pci_dma_addr;
        pst_cb_res->len = oal_netbuf_len(pst_netbuf);

        st_write_item.buff_paddr.addr = pci_dma_addr;

        /* 入队 */

        oal_spin_lock_irq_save(&pst_pci_res->st_rx_res.lock, &flags);
        oal_netbuf_list_tail_nolock(&pst_pci_res->st_rx_res.rxq, pst_netbuf);
        oal_spin_unlock_irq_restore(&pst_pci_res->st_rx_res.lock, &flags);

        pci_print_log(PCI_LOG_DBG, "d2h ringbuf write [netbuf:0x%p, data:[va:0x%lx,pa:0x%llx]",
                      pst_netbuf, (uintptr_t)oal_netbuf_data(pst_netbuf), st_write_item.buff_paddr.addr);
        oal_pcie_mips_start(PCIE_MIPS_RX_RINGBUF_WRITE);
        if (oal_unlikely(oal_pcie_d2h_ringbuf_write(pst_pci_res,
            &pst_pci_res->st_rx_res.ringbuf_data_dma_addr,
            &st_write_item) == 0)) {
            oal_pcie_mips_end(PCIE_MIPS_RX_RINGBUF_WRITE);
            break;
        }
        oal_pcie_mips_end(PCIE_MIPS_RX_RINGBUF_WRITE);

        cnt++;
    }

    /* 这里需要考虑HOST/DEVICE的初始化顺序 */
    if (cnt && (is_doorbell == OAL_TRUE)) {
        oal_pcie_d2h_ringbuf_wr_update(pst_pci_res);
#ifdef _PRE_COMMENT_CODE_
        /* 暂时不需要敲铃，D2H Device是大循环 */
        oal_pcie_d2h_doorbell(pst_pci_res);
#endif
    }

    oal_pcie_mips_end(PCIE_MIPS_RX_NETBUF_SUPPLY);

    return cnt;
}

OAL_STATIC void oal_pcie_print_ringbuf_info(pcie_ringbuf *pst_ringbuf, PCI_LOG_TYPE level)
{
    if (oal_warn_on(pst_ringbuf == NULL)) {
        return;
    }

    /* dump the ringbuf info */
    pci_print_log(level, "ringbuf[0x%p] idx:%u, rd:%u, wr:%u, size:%u, item_len:%u, item_mask:0x%x, base_addr:0x%llx",
                  pst_ringbuf,
                  pst_ringbuf->idx,
                  pst_ringbuf->rd,
                  pst_ringbuf->wr,
                  pst_ringbuf->size,
                  pst_ringbuf->item_len,
                  pst_ringbuf->item_mask,
                  pst_ringbuf->base_addr);
}

/* 预先分配rx的接收buf */
int32_t oal_pcie_rx_ringbuf_build(oal_pcie_res *pst_pci_res)
{
    /* 走到这里要确保DEVICE ZI区已经初始化完成， 中断已经注册和使能 */
#ifdef _PRE_PLAT_FEATURE_HI110X_PCIE_D2H_BYPASS
#else
    int32_t ret;
    int32_t supply_num;
    supply_num = oal_pcie_rx_ringbuf_supply(pst_pci_res, OAL_TRUE, OAL_TRUE,
                                            PCIE_RX_RINGBUF_SUPPLY_ALL, GFP_KERNEL, &ret);
    if (supply_num == 0) {
        pci_print_log(PCI_LOG_WARN, "oal_pcie_rx_ringbuf_build can't get any netbufs!, rxq len:%u",
                      oal_netbuf_list_len(&pst_pci_res->st_rx_res.rxq));
        oal_pcie_print_ringbuf_info(&pst_pci_res->st_ringbuf.st_d2h_buf, PCI_LOG_WARN);
        return -OAL_ENOMEM;
    } else {
        pci_print_log(PCI_LOG_INFO, "oal_pcie_rx_ringbuf_build got %u netbufs!", supply_num);
    }
#endif
    return OAL_SUCC;
}

/* 释放RX通路的资源 */
OAL_STATIC void oal_pcie_rx_res_clean(oal_pcie_res *pst_pci_res)
{
    unsigned long flags;
    oal_netbuf_stru *pst_netbuf = NULL;

    /* 释放RX补充队列 */
    pci_print_log(PCI_LOG_INFO, "prepare free rxq len=%d", oal_netbuf_list_len(&pst_pci_res->st_rx_res.rxq));
    forever_loop() {
        oal_spin_lock_irq_save(&pst_pci_res->st_rx_res.lock, &flags);
        pst_netbuf = oal_netbuf_delist_nolock(&pst_pci_res->st_rx_res.rxq);
        oal_spin_unlock_irq_restore(&pst_pci_res->st_rx_res.lock, &flags);

        if (pst_netbuf == NULL) {
            break;
        }

        oal_pcie_release_rx_netbuf(pst_pci_res, pst_netbuf);
    }
}

/* 释放TX通路的资源 */
OAL_STATIC void oal_pcie_tx_res_clean(oal_pcie_res *pst_pci_res)
{
    int32_t i;
    unsigned long flags;
    oal_netbuf_stru *pst_netbuf = NULL;

    /* 释放待TX发送队列, Ringbuf 在DEV侧 直接下电 */
    for (i = 0; i < PCIE_H2D_QTYPE_BUTT; i++) {
        pci_print_log(PCI_LOG_INFO, "prepare free txq[%d] len=%d",
                      i, oal_netbuf_list_len(&pst_pci_res->st_tx_res[i].txq));
        forever_loop() {
            oal_spin_lock_irq_save(&pst_pci_res->st_tx_res[i].lock, &flags);
            pst_netbuf = oal_netbuf_delist_nolock(&pst_pci_res->st_tx_res[i].txq);
            oal_spin_unlock_irq_restore(&pst_pci_res->st_tx_res[i].lock, &flags);

            if (pst_netbuf == NULL) {
                break;
            }

            oal_pcie_tx_netbuf_free(pst_pci_res, pst_netbuf);
        }
        oal_atomic_set(&pst_pci_res->st_tx_res[i].tx_ringbuf_sync_cond, 0);
    }
}

OAL_STATIC int32_t oal_pcie_transfer_edma_res_init(oal_pcie_res *pst_pci_res)
{
    int32_t ret;

    /* 下载完PATCH才需要执行下面的操作, 片验证阶段通过SSI下载代码 */
    ret = oal_pcie_share_mem_res_map(pst_pci_res);
    if (ret != OAL_SUCC) {
        return ret;
    }

    ret = oal_pcie_ringbuf_res_map(pst_pci_res);
    if (ret != OAL_SUCC) {
        return ret;
    }

    oal_pcie_rx_res_clean(pst_pci_res);
    oal_pcie_tx_res_clean(pst_pci_res);

    ret = oal_pcie_rx_ringbuf_build(pst_pci_res);
    if (ret != OAL_SUCC) {
        return ret;
    }

    mutex_lock(&pst_pci_res->st_rx_mem_lock);
    oal_pcie_change_link_state(pst_pci_res, PCI_WLAN_LINK_RES_UP);
    mutex_unlock(&pst_pci_res->st_rx_mem_lock);

    return ret;
}

/* first power init */
int32_t oal_pcie_transfer_res_init(oal_pcie_res *pst_pci_res)
{
    int32_t ret = -OAL_ENODEV;

    if (pst_pci_res->chip_info.edma_support == OAL_TRUE) {
        ret = oal_pcie_transfer_edma_res_init(pst_pci_res);
    } else if (pst_pci_res->chip_info.ete_support == OAL_TRUE) {
        ret = oal_ete_transfer_res_init(pst_pci_res);
    }

    return ret;
}

void oal_pcie_transfer_edma_res_exit(oal_pcie_res *pst_pci_res)
{
    oal_pcie_change_link_state(pst_pci_res, PCI_WLAN_LINK_DOWN);

    oal_pcie_rx_res_clean(pst_pci_res);
    oal_pcie_tx_res_clean(pst_pci_res);

    oal_pcie_ringbuf_res_unmap(pst_pci_res);

    oal_pcie_share_mem_res_unmap(pst_pci_res);
}

void oal_pcie_transfer_res_exit(oal_pcie_res *pst_pci_res)
{
    if (pst_pci_res->chip_info.edma_support == OAL_TRUE) {
        oal_pcie_transfer_edma_res_exit(pst_pci_res);
    } else if (pst_pci_res->chip_info.ete_support == OAL_TRUE) {
        oal_ete_transfer_res_exit(pst_pci_res);
    }
}

/* 配置BAR,IATU等设备资源 */
int32_t oal_pcie_dev_init(oal_pcie_res *pst_pci_res)
{
    int32_t ret;
    oal_pci_dev_stru *pst_pci_dev = pcie_res_to_dev(pst_pci_res);

    if (oal_warn_on(pst_pci_dev == NULL)) {
        return -OAL_ENODEV;
    }

    ret = oal_pcie_bar_init(pst_pci_res);
    if (ret != OAL_SUCC) {
        return ret;
    }

    ret = oal_pcie_ctrl_base_address_init(pst_pci_res);
    if (ret != OAL_SUCC) {
        oal_pcie_bar_exit(pst_pci_res);
        return ret;
    }

    return OAL_SUCC;
}
void oal_pcie_dev_deinit(oal_pcie_res *pst_pci_res)
{
    oal_pci_dev_stru *pst_pci_dev = pcie_res_to_dev(pst_pci_res);
    if (oal_warn_on(pst_pci_dev == NULL)) {
        return;
    }
    oal_pcie_ctrl_base_address_exit(pst_pci_res);
    oal_pcie_bar_exit(pst_pci_res);
}

/* isr functions */
OAL_STATIC int32_t oal_pcie_tx_dma_addr_match(oal_netbuf_stru *pst_netbuf, edma_paddr_t dma_addr)
{
    /* dma_addr 存放在CB字段里 */
    pcie_cb_dma_res st_cb_dma;
    int32_t ret;

    /* 不是从CB的首地址开始，必须拷贝，对齐问题。 */
    ret = memcpy_s(&st_cb_dma, sizeof(pcie_cb_dma_res),
                   (uint8_t *)oal_netbuf_cb(pst_netbuf) + sizeof(struct hcc_tx_cb_stru),
                   sizeof(pcie_cb_dma_res));
    if (ret != EOK) {
        pci_print_log(PCI_LOG_ERR, "get tx dma addr failed");
        return OAL_FALSE;
    }

    pci_print_log(PCI_LOG_DBG, "tx dma addr match, cb's addr 0x%llx , dma_addr 0x%llx",
                  st_cb_dma.paddr.addr, dma_addr.addr);

    if (st_cb_dma.paddr.addr == dma_addr.addr) {
        return OAL_TRUE;
    }
    return OAL_FALSE;
}

OAL_STATIC int32_t oal_pcie_tx_dma_addr_match_low(oal_netbuf_stru *pst_netbuf, uint16_t dma_addr)
{
    /* dma_addr 存放在CB字段里 */
    pcie_cb_dma_res st_cb_dma;
    int32_t ret;

    /* 不是从CB的首地址开始，必须拷贝，对齐问题。 */
    ret = memcpy_s(&st_cb_dma, sizeof(pcie_cb_dma_res),
                   (uint8_t *)oal_netbuf_cb(pst_netbuf) + sizeof(struct hcc_tx_cb_stru),
                   sizeof(pcie_cb_dma_res));
    if (ret != EOK) {
        pci_print_log(PCI_LOG_ERR, "get tx dma addr failed");
        return OAL_FALSE;
    }

    pci_print_log(PCI_LOG_DBG, "tx dma addr match, cb's addr 0x%llx , dma_addr 0x%x", st_cb_dma.paddr.addr, dma_addr);

    if ((uint16_t)st_cb_dma.paddr.bits.low_addr == dma_addr) {
        return OAL_TRUE;
    }

    pci_print_log(PCI_LOG_WARN, "tx dma addr match, cb's addr 0x%llx , dma_addr 0x%x", st_cb_dma.paddr.addr, dma_addr);
    return OAL_FALSE;
}

OAL_STATIC int32_t oal_pcie_rx_dma_addr_match(oal_netbuf_stru *pst_netbuf, edma_paddr_t dma_addr)
{
    pcie_cb_dma_res *pst_cb_res;
    /* DMA地址填到CB中, CB首地址8字节对齐可以直接强转 */
    pst_cb_res = (pcie_cb_dma_res *)oal_netbuf_cb(pst_netbuf);
    if (pst_cb_res->paddr.addr == dma_addr.addr) {
        return OAL_TRUE;
    }
    return OAL_FALSE;
}

OAL_STATIC int32_t oal_pcie_rx_dma_addr_matchlow(oal_netbuf_stru *pst_netbuf, uint32_t dma_addr)
{
    pcie_cb_dma_res *pst_cb_res;
    /* DMA地址填到CB中, CB首地址8字节对齐可以直接强转 */
    pst_cb_res = (pcie_cb_dma_res *)oal_netbuf_cb(pst_netbuf);
    if (pst_cb_res->paddr.bits.low_addr == dma_addr) {
        return OAL_TRUE;
    }

    pci_print_log(PCI_LOG_WARN, "rx dma addr match, cb's addr 0x%x , dma_addr 0x%x",
                  pst_cb_res->paddr.bits.low_addr, dma_addr);
    return OAL_FALSE;
}

void oal_pcie_release_rx_netbuf(oal_pcie_res *pst_pci_res, oal_netbuf_stru *pst_netbuf)
{
    pcie_cb_dma_res *pst_cb_res = NULL;
    oal_pci_dev_stru *pst_pci_dev = NULL;
    if (oal_warn_on(pst_netbuf == NULL)) {
        return;
    }

    if (oal_warn_on(pst_pci_res == NULL)) {
        declare_dft_trace_key_info("pcie release rx netbuf", OAL_DFT_TRACE_FAIL);
        oal_netbuf_free(pst_netbuf);
        return;
    }

    pst_pci_dev = pcie_res_to_dev(pst_pci_res);
    if (oal_warn_on(pst_pci_dev == NULL)) {
        declare_dft_trace_key_info("pcie release rx netbuf", OAL_DFT_TRACE_FAIL);
        oal_netbuf_free(pst_netbuf);
        return;
    }

    pst_cb_res = (pcie_cb_dma_res *)oal_netbuf_cb(pst_netbuf);
    if (oal_likely((pst_cb_res->paddr.addr != 0) && (pst_cb_res->len != 0))) {
        dma_unmap_single(&pst_pci_dev->dev, (dma_addr_t)pst_cb_res->paddr.addr, pst_cb_res->len, PCI_DMA_FROMDEVICE);
    } else {
        declare_dft_trace_key_info("pcie release rx netbuf", OAL_DFT_TRACE_FAIL);
    }

    oal_netbuf_free(pst_netbuf);
}

/* 向Hcc层提交收到的netbuf */
void oal_pcie_rx_netbuf_submit(oal_pcie_res *pst_pci_res, oal_netbuf_stru *pst_netbuf)
{
    struct hcc_handler *hcc = NULL;
    oal_pci_dev_stru *pst_pci_dev = NULL;
    pcie_cb_dma_res *pst_cb_res = NULL;
    oal_pcie_linux_res *pst_pci_lres = NULL;

    pst_pci_dev = pcie_res_to_dev(pst_pci_res);
    if (oal_unlikely(pst_pci_dev == NULL)) {
        goto release_netbuf;
    }

    pst_pci_lres = (oal_pcie_linux_res *)oal_pci_get_drvdata(pst_pci_dev);
    if (oal_unlikely(pst_pci_lres == NULL)) {
        pci_print_log(PCI_LOG_ERR, "lres is null");
        goto release_netbuf;
    }

    if (oal_unlikely(pst_pci_lres->pst_bus == NULL)) {
        pci_print_log(PCI_LOG_ERR, "lres's bus is null");
        goto release_netbuf;
    }

    if (oal_unlikely(hbus_to_dev(pst_pci_lres->pst_bus) == NULL)) {
        pci_print_log(PCI_LOG_ERR, "lres's dev is null");
        goto release_netbuf;
    }

    hcc = hbus_to_hcc(pst_pci_lres->pst_bus);
    if (oal_unlikely(hcc == NULL)) {
        pci_print_log(PCI_LOG_ERR, "lres's hcc is null");
        goto release_netbuf;
    }

    pst_cb_res = (pcie_cb_dma_res *)oal_netbuf_cb(pst_netbuf);

    if (oal_unlikely((pst_cb_res->paddr.addr == 0) || (pst_cb_res->len == 0))) {
        goto release_netbuf;
    }

    /* unmap pcie dma addr */
    dma_unmap_single(&pst_pci_dev->dev, (dma_addr_t)pst_cb_res->paddr.addr, pst_cb_res->len, PCI_DMA_FROMDEVICE);
    if (g_pcie_dma_data_check_enable) {
        if (oal_unlikely(oal_readl(oal_netbuf_data(pst_netbuf)) == 0xffffffff)) {
            declare_dft_trace_key_info("invalid_pcie_rx_netbuf", OAL_DFT_TRACE_FAIL);
            oal_print_hex_dump((uint8_t *)(oal_netbuf_data(pst_netbuf)),
                               HCC_HDR_TOTAL_LEN, HEX_DUMP_GROUP_SIZE, "hdr ");
            oal_print_hex_dump((uint8_t *)(oal_netbuf_data(pst_netbuf) + HCC_HDR_TOTAL_LEN),
                               HCC_HDR_TOTAL_LEN / 2, HEX_DUMP_GROUP_SIZE, "payload "); /* 2 half mem dump */
            oal_netbuf_free(pst_netbuf);
            oal_disable_pcie_irq(pst_pci_lres);
            /* DFR trigger */
            oal_pcie_change_link_state(pst_pci_lres->pst_pci_res, PCI_WLAN_LINK_DOWN);
            hcc_bus_exception_submit(pst_pci_lres->pst_bus, WIFI_TRANS_FAIL);
        } else {
            hcc_rx_submit(hcc, pst_netbuf);
        }
    } else {
        hcc_rx_submit(hcc, pst_netbuf);
    }
    return;

release_netbuf:
    oal_pcie_release_rx_netbuf(pst_pci_res, pst_netbuf);
    declare_dft_trace_key_info("pcie release rx netbuf", OAL_DFT_TRACE_OTHER);
    return;
}

OAL_STATIC int32_t oal_pcie_unmap_tx_netbuf(oal_pcie_res *pst_pci_res, oal_netbuf_stru *pst_netbuf)
{
    /* dma_addr 存放在CB字段里 */
    pcie_cb_dma_res st_cb_dma;
    oal_pci_dev_stru *pst_pci_dev;
    int32_t ret;

    pst_pci_dev = pcie_res_to_dev(pst_pci_res);

    /* 不是从CB的首地址开始，必须拷贝，对齐问题。 */
    ret = memcpy_s(&st_cb_dma, sizeof(pcie_cb_dma_res),
                   (uint8_t *)oal_netbuf_cb(pst_netbuf) + sizeof(struct hcc_tx_cb_stru),
                   sizeof(pcie_cb_dma_res));
    if (ret != EOK) {
        pci_print_log(PCI_LOG_ERR, "get dma addr from cb filed failed");
        return -OAL_EFAIL;
    }
#ifdef _PRE_PLAT_FEATURE_PCIE_DEBUG
    /* Debug */
    memset_s((uint8_t *)oal_netbuf_cb(pst_netbuf) + sizeof(struct hcc_tx_cb_stru),
             oal_netbuf_cb_size() - sizeof(struct hcc_tx_cb_stru), 0, sizeof(st_cb_dma));
#endif

    /* unmap pcie dma addr */
    if (oal_likely((st_cb_dma.paddr.addr != 0) && (st_cb_dma.len != 0))) {
        dma_unmap_single(&pst_pci_dev->dev, (dma_addr_t)st_cb_dma.paddr.addr, st_cb_dma.len, PCI_DMA_TODEVICE);
    } else {
        declare_dft_trace_key_info("pcie tx netbuf free fail", OAL_DFT_TRACE_FAIL);
        oal_print_hex_dump((uint8_t *)oal_netbuf_cb(pst_netbuf), oal_netbuf_cb_size(),
                           HEX_DUMP_GROUP_SIZE, "invalid cb: ");
        return -OAL_EFAIL;
    }

    return OAL_SUCC;
}

void oal_pcie_tx_netbuf_free(oal_pcie_res *pst_pci_res, oal_netbuf_stru *pst_netbuf)
{
    oal_pcie_unmap_tx_netbuf(pst_pci_res, pst_netbuf);

    hcc_tx_netbuf_free(pst_netbuf);
}

#ifdef _PRE_PLAT_FEATURE_HI110X_PCIE_H2D_BYPASS
/* 发送完成中断 */
OAL_STATIC void oal_pcie_h2d_transfer_done(oal_pcie_res *pst_pci_res)
{
    unsigned long flags;
    int32_t j, flag, cnt, total_cnt;
    /* tx fifo中获取 发送完成的首地址,双通道，双地址，双count */
    edma_paddr_t addr[PCIE_EDMA_MAX_CHANNELS];
    uint32_t count[PCIE_EDMA_MAX_CHANNELS];

    pst_pci_res->stat.intx_tx_count++;

    if (oal_pcie_edma_get_read_done_fifo(pst_pci_res, addr, count) != OAL_TRUE) {
        /* 待增加维测计数 */
        return;
    }

    pci_print_log(PCI_LOG_DBG, "oal_pcie_h2d_transfer_done, cnt:%u", pst_pci_res->stat.intx_tx_count);

    /*
     * 获取到发送完成的DMA地址，遍历发送队列,
     * 先遍历第一个元素，正常应该队头就是发送完成的元素，
     * 如果不在队头说明丢中断了(有FIFO正常不会丢),需要释放元素之前的netbuf
     */
    flag = 0;
    total_cnt = 0;
#ifdef _PRE_COMMENT_CODE_
    for (i = 0; i < PCIE_H2D_QTYPE_BUTT; i++) {
#endif
        /* DMA双通道 */
        for (j = 0; j < PCIE_EDMA_MAX_CHANNELS; j++) {
            pci_print_log(PCI_LOG_DBG, "tx chan:%d pa 0x%llx, cnt:%d", j, addr[j].addr, count[j]);

            cnt = count[j]; /* 无效描述符时,count为0 */
            if (!cnt) {
                continue;
            }

            if (g_h2d_pst_netbuf == NULL) {
                pci_print_log(PCI_LOG_DBG, "g_h2d_pst_netbuf is null!");
                break;
            }
#ifdef _PRE_PLAT_FEATURE_HI110X_PCIE_FIFO_ADDRESS
            if ((uint64_t)g_h2d_pci_dma_addr != (uint64_t)addr[j].addr) {
                /* 地址不匹配，遍历下一个队列 */
                pci_print_log(PCI_LOG_ERR, "unkown bypass netbuf addr:0x%llu, should be :0x%llu",
                              (uint64_t)g_h2d_pci_dma_addr, addr[j]);
                break;
            }
#endif

            h2d_bypass_pkt_num += cnt;
            pst_pci_res->st_tx_res[PCIE_H2D_QTYPE_NORMAL].stat.tx_done_count++;
            total_cnt += cnt;
        }

        if (oal_unlikely(total_cnt > PCIE_EDMA_READ_BUSRT_COUNT)) {
            pst_pci_res->st_tx_res[PCIE_H2D_QTYPE_NORMAL].stat.tx_burst_cnt[0]++;
        } else {
            pst_pci_res->st_tx_res[PCIE_H2D_QTYPE_NORMAL].stat.tx_burst_cnt[total_cnt]++;
        }
#ifdef _PRE_COMMENT_CODE_
        /* 未匹配，遍历下一条队列 */
    }
#endif
}
#else
/* 发送完成中断 */
OAL_STATIC void oal_pcie_h2d_transfer_done(oal_pcie_res *pst_pci_res)
{
    unsigned long flags;
    int32_t i, j, flag, cnt, total_cnt, netbuf_cnt, curr_cnt;
    oal_netbuf_stru *pst_netbuf = NULL;
    oal_netbuf_head_stru *pst_txq = NULL;
    /* tx fifo中获取 发送完成的首地址,双通道，双地址，双count */
    edma_paddr_t addr[PCIE_EDMA_MAX_CHANNELS];
    uint32_t count[PCIE_EDMA_MAX_CHANNELS];

    pst_pci_res->stat.intx_tx_count++;

    if (oal_pcie_edma_get_read_done_fifo(pst_pci_res, addr, count) != OAL_TRUE) {
        /* 待增加维测计数 */
        return;
    }

    pci_print_log(PCI_LOG_DBG, "oal_pcie_h2d_transfer_done, cnt:%u", pst_pci_res->stat.intx_tx_count);

    /*
     * 获取到发送完成的DMA地址，遍历发送队列,
     * 先遍历第一个元素，正常应该队头就是发送完成的元素，
     * 如果不在队头说明丢中断了(有FIFO正常不会丢),需要释放元素之前的netbuf
     */
    flag = 0;
    netbuf_cnt = 0;

    for (i = 0; i < PCIE_H2D_QTYPE_BUTT; i++) {
        total_cnt = 0;

        pst_txq = &pst_pci_res->st_tx_res[i].txq;

        if (oal_netbuf_list_empty(pst_txq)) {
            /* next txq */
            continue;
        }

        /* DMA双通道 */
        for (j = 0; j < PCIE_EDMA_MAX_CHANNELS; j++) {
            if (oal_netbuf_list_empty(pst_txq)) {
                /* 队列为空 */
                break;
            }

            pci_print_log(PCI_LOG_DBG, "[q:%d]tx chan:%d pa 0x%llx, cnt:%d", i, j, addr[j].addr, count[j]);

            cnt = count[j]; /* 无效描述符时,count为0 */
            if (!cnt) {
#ifdef _PRE_COMMENT_CODE_
                if (addr[j].addr) {
                    /* cnt 为0 时，addr 是上一次的值 */
                    pci_print_log(PCI_LOG_DBG, "tx chan:%d get invalid dma pa 0x%llx", j, addr[j].addr);
                }
#endif
                continue;
            }

            /* 保证一个地方入队，这里出队 */
            pst_netbuf = (oal_netbuf_stru *)oal_netbuf_next(pst_txq);

            if (oal_pcie_tx_dma_addr_match(pst_netbuf, addr[j]) != OAL_TRUE) {
                /* 地址不匹配，遍历下一个队列 */
                pci_print_log(PCI_LOG_DBG, "[q:%d]tx chan:%d match failed, search next txq", i, j);
                break;
            }

            /* match succ
             * 找到地址，出队,先入先出，所以先检查通道0，再检查通道1,
             * 2个通道的地址 应该在同一个队列中
             */
            curr_cnt = oal_netbuf_list_len(pst_txq);
            if (oal_unlikely(cnt > curr_cnt)) {
                /* count 出错? */
                pci_print_log(PCI_LOG_ERR, "[q:%d]tx chan:%d tx done invalid count cnt %d ,list len %u", i, j,
                              cnt, curr_cnt);
                declare_dft_trace_key_info("pcie tx done count error", OAL_DFT_TRACE_EXCEP);
                flag = 0;
                goto done;
            }

            total_cnt += cnt;

            do {
                /* 这里的锁可以优化 */
                oal_spin_lock_irq_save(&pst_pci_res->st_tx_res[i].lock, &flags);
                /* 头部出队 */
                pst_netbuf = oal_netbuf_delist_nolock(pst_txq);
                oal_spin_unlock_irq_restore(&pst_pci_res->st_tx_res[i].lock, &flags);
                if (pst_netbuf == NULL) {
                    /* 不应该为空，count有可能有问题 */
                    pci_print_log(PCI_LOG_ERR, "[q:%d]tx chan:%d tx netbuf queue underflow[cnt:%d:%d, qlen:%d]", i, j,
                                  cnt, count[j], curr_cnt);
                    declare_dft_trace_key_info("pcie tx done count error2", OAL_DFT_TRACE_EXCEP);
                    flag = 0;
                    goto done;
                }
#ifdef _PRE_WLAN_PKT_TIME_STAT
                if (DELAY_STATISTIC_SWITCH_ON) {
                    delay_record_snd_combine(pst_netbuf);
                }
#endif
                /* unmap dma addr & free netbuf */
                pst_pci_res->st_tx_res[i].stat.tx_done_count++;
                pci_print_log(PCI_LOG_DBG, "[q:%d]tx chan:%d, send netbuf ok, va:0x%p, cnt:%u",
                              i, j, pst_netbuf, pst_pci_res->st_tx_res[i].stat.tx_done_count);
                oal_pcie_tx_netbuf_free(pst_pci_res, pst_netbuf);
            } while (--cnt);

            if (!cnt) {
                /* 一个通道的地址处理完成 */
                pci_print_log(PCI_LOG_DBG, "[q:%d]tx chan:%d all bus process done!", i, j);
                flag = 1;
            }
        }

        netbuf_cnt += total_cnt;

        if (oal_unlikely(total_cnt > PCIE_EDMA_READ_BUSRT_COUNT)) {
            pst_pci_res->st_tx_res[i].stat.tx_burst_cnt[0]++;
        } else {
            if (total_cnt) {
                pst_pci_res->st_tx_res[i].stat.tx_burst_cnt[total_cnt]++;
            }
        }

        pst_pci_res->st_tx_res[i].stat.tx_count += total_cnt;

        if (flag) {
            break;
        }

        /* 未匹配，遍历下一条队列 */
    }

    if (oal_unlikely(netbuf_cnt != (count[0] + count[1]))) {
        pci_print_log(PCI_LOG_WARN, "count error total cnt:%u != dev reg count[0]%u  count[1] %u",
                      netbuf_cnt, count[0], count[1]);
        oal_disable_pcie_irq((oal_pcie_linux_res *)oal_pci_get_drvdata(pcie_res_to_dev(pst_pci_res)));
        flag = 0;
        goto done;
    }

done:
    if (!flag) {
        /* 维测,未找到FIFO中的地址，地址有错，或者count有错,或者丢中断 这里应该触发DFR */
#ifdef _PRE_COMMENT_CODE_
        declare_dft_trace_key_info("pcie tx done addr error", OAL_DFT_TRACE_EXCEP);
#endif
        pci_print_log(PCI_LOG_ERR, "pcie tx done addr error");
    } else {
        oal_pcie_linux_res *pst_pci_lres = (oal_pcie_linux_res *)oal_pci_get_drvdata(pcie_res_to_dev(pst_pci_res));
        /* 发送完成,唤醒发送线程 */
        if (oal_likely(pst_pci_lres != NULL)) {
            pci_print_log(PCI_LOG_DBG, "pcie sched hcc thread, qid:%d", i);
            oal_atomic_set(&pst_pci_res->st_tx_res[i].tx_ringbuf_sync_cond, 1); /* 下半部刷新，保持一致性 */
            hcc_sched_transfer(hbus_to_hcc(pst_pci_lres->pst_bus));
        }
    }
}
#endif

#ifdef _PRE_PLAT_FEATURE_HI110X_PCIE_D2H_BYPASS
/* 接收完成中断 */
OAL_STATIC void oal_pcie_d2h_transfer_done(oal_pcie_res *pst_pci_res)
{
    unsigned long flags;
    int32_t i, flag, cnt, total_cnt;
    /* rx fifo中获取 发送完成的首地址,双通道，双地址，双count */
    edma_paddr_t addr[PCIE_EDMA_MAX_CHANNELS];
    uint32_t count[PCIE_EDMA_MAX_CHANNELS];

    pst_pci_res->stat.intx_rx_count++;

    oal_pcie_mips_start(PCIE_MIPS_RX_MSG_FIFO);
    if (oal_pcie_edma_get_write_done_fifo(pst_pci_res, addr, count) != OAL_TRUE) {
        /* 待增加维测计数 */
        return;
    }
    oal_pcie_mips_end(PCIE_MIPS_RX_MSG_FIFO);

    pci_print_log(PCI_LOG_DBG, "oal_pcie_d2h_transfer_done, cnt:%u", pst_pci_res->stat.intx_rx_count);

    /*
     * 获取到发送完成的DMA地址，遍历发送队列,
     * 先遍历第一个元素，正常应该队头就是发送完成的元素，
     * 如果不在队头说明丢中断了(有FIFO正常不会丢),需要释放元素之前的netbuf
     */
    flag = 0;
    total_cnt = 0;

    /* DMA双通道 */
    for (i = 0; i < PCIE_EDMA_MAX_CHANNELS; i++) {
        pci_print_log(PCI_LOG_DBG, "rx chan:%d pa 0x%llx, cnt:%d", i, addr[i].addr, count[i]);
        cnt = count[i]; /* 无效描述符时,count为0 */
        if (!cnt) {
#ifdef _PRE_COMMENT_CODE_
            if (addr[i].addr) {
                /* cnt 和 addr 应该同时为 0 */
                pci_print_log(PCI_LOG_DBG, "rx chan:%d get invalid dma pa 0x%llx", i, addr[i].addr);
            }
#endif
            continue;
        }

        /* 找到地址，出队,先入先出，所以先检查通道0，再检查通道1, 2个通道的地址 应该在同一个队列中 */
        cnt = count[i];
        pst_pci_res->st_rx_res.stat.rx_done_count++;
        d2h_bypass_pkt_num += cnt;
        total_cnt += cnt;
    }

    if (oal_unlikely(total_cnt > PCIE_EDMA_WRITE_BUSRT_COUNT)) {
        pst_pci_res->st_rx_res.stat.rx_burst_cnt[0]++;
    } else {
        pst_pci_res->st_rx_res.stat.rx_burst_cnt[total_cnt]++;
    }

    if (d2h_bypass_pkt_num >= d2h_bypass_total_pkt_num) {
        oal_complete(&g_d2h_test_done);
    }

    oal_pcie_rx_ringbuf_bypass_supply(pst_pci_res, OAL_TRUE, OAL_TRUE, PCIE_RX_RINGBUF_SUPPLY_ALL);
}
#else
uint32_t rx_addr_count_err_cnt = 0;
/* 接收完成中断 */
OAL_STATIC void oal_pcie_d2h_transfer_done(oal_pcie_res *pst_pci_res)
{
    unsigned long flags;
    int32_t i, flag, cnt, total_cnt;
    const uint32_t ul_max_dump_bytes = 128;
    oal_netbuf_stru *pst_netbuf = NULL;
    oal_netbuf_head_stru *pst_rxq = NULL;
    /* rx fifo中获取 发送完成的首地址,双通道，双地址，双count */
    edma_paddr_t addr[PCIE_EDMA_MAX_CHANNELS];
    uint32_t count[PCIE_EDMA_MAX_CHANNELS];

    pst_pci_res->stat.intx_rx_count++;

    oal_pcie_mips_start(PCIE_MIPS_RX_MSG_FIFO);
    if (oal_pcie_edma_get_write_done_fifo(pst_pci_res, addr, count) != OAL_TRUE) {
        /* 待增加维测计数 */
        return;
    }
    oal_pcie_mips_end(PCIE_MIPS_RX_MSG_FIFO);

    pci_print_log(PCI_LOG_DBG, "oal_pcie_d2h_transfer_done, cnt:%u", pst_pci_res->stat.intx_rx_count);

    /*
     * 获取到发送完成的DMA地址，遍历发送队列,
     * 先遍历第一个元素，正常应该队头就是发送完成的元素，
     * 如果不在队头说明丢中断了(有FIFO正常不会丢),需要释放元素之前的netbuf
     */
    flag = 0;
    total_cnt = 0;

    pst_rxq = &pst_pci_res->st_rx_res.rxq;

    if (oal_netbuf_list_empty(pst_rxq)) {
        /* next txq */
        declare_dft_trace_key_info("pcie rx done fifo error", OAL_DFT_TRACE_EXCEP);
        return;
    }

    /* DMA双通道 */
    for (i = 0; i < PCIE_EDMA_MAX_CHANNELS; i++) {
        if (oal_netbuf_list_empty(pst_rxq)) {
            /* 队列为空 */
            break;
        }

        pci_print_log(PCI_LOG_DBG, "rx chan:%d pa 0x%llx, cnt:%d", i, addr[i].addr, count[i]);
        cnt = count[i]; /* 无效描述符时,count为0 */
        if (!cnt) {
#ifdef _PRE_COMMENT_CODE_
            if (addr[i].addr) {
                /* cnt 和 addr 应该同时为 0 */
                pci_print_log(PCI_LOG_DBG, "rx chan:%d get invalid dma pa 0x%llx", i, addr[i].addr);
            }
#endif
            continue;
        }

        /* 保证一个地方入队，这里出队 */
        pst_netbuf = (oal_netbuf_stru *)oal_netbuf_next(pst_rxq);

        if (oal_unlikely(pst_netbuf == NULL)) {
            oal_print_hi11xx_log(HI11XX_LOG_ERR, "%s error: pst_netbuf is null", __FUNCTION__);
            continue;
        };

#ifdef _PRE_PLAT_FEATURE_HI110X_PCIE_FIFO_ADDRESS
        if (oal_pcie_rx_dma_addr_match(pst_netbuf, addr[i]) != OAL_TRUE) {
            rx_addr_count_err_cnt++;
            if (rx_addr_count_err_cnt >= 2) { // 2, err retry cnt
                /* 地址不匹配 重试一次 */
                pci_print_log(PCI_LOG_ERR, "rx chan:%d match failed, rx error, count[i]:%u, errcnt:%d",
                              i, count[i], rx_addr_count_err_cnt);
                pci_print_log(PCI_LOG_ERR, "count0:%u, count1:%u", count[0], count[1]);
                declare_dft_trace_key_info("pcie rx addr fatal error", OAL_DFT_TRACE_EXCEP);
                flag = 0;
                goto done;
            } else {
                /* 地址不匹配，出错 */
                pci_print_log(PCI_LOG_ERR, "rx chan:%d match failed, rx error, count[i]:%u, errcnt:%d",
                              i, count[i], rx_addr_count_err_cnt);
                pci_print_log(PCI_LOG_ERR, "count0:%u, count1:%u", count[0], count[1]);
                declare_dft_trace_key_info("pcie rx addr error,retry", OAL_DFT_TRACE_FAIL);
                flag = 1;
                goto done;
            }
        } else {
            rx_addr_count_err_cnt = 0;
        }
#endif

        /*
         * 找到地址，出队,先入先出，所以先检查通道0，再检查通道1,
         * 2个通道的地址 应该在同一个队列中
         */
        cnt = count[i];
        if (oal_unlikely(cnt > oal_netbuf_list_len(pst_rxq))) {
            pci_print_log(PCI_LOG_ERR, "rx chan:%d rx done invalid count cnt %d ,list len %u", i,
                          cnt, oal_netbuf_list_len(pst_rxq));
            declare_dft_trace_key_info("pcie rx done count error", OAL_DFT_TRACE_EXCEP);
            flag = 0;
            goto done;
        }
        total_cnt += cnt;

        oal_pcie_mips_start(PCIE_MIPS_RX_QUEUE_POP);
        do {
            /* 这里的锁可以优化 */
            oal_spin_lock_irq_save(&pst_pci_res->st_rx_res.lock, &flags);

            /* 头部出队 */
            pst_netbuf = oal_netbuf_delist_nolock(pst_rxq);
            oal_spin_unlock_irq_restore(&pst_pci_res->st_rx_res.lock, &flags);
            if (pst_netbuf == NULL) {
                oal_pcie_mips_end(PCIE_MIPS_RX_QUEUE_POP);

                /* 不应该为空，count有可能有问题 */
                pci_print_log(PCI_LOG_ERR, "rx chan:%d tx netbuf queue underflow[cnt:%d, qlen:%d]", i,
                              cnt, oal_netbuf_list_len(pst_rxq));
                declare_dft_trace_key_info("pcie rx done count error2", OAL_DFT_TRACE_EXCEP);
                flag = 0;
                goto done;
            }
            pst_pci_res->st_rx_res.stat.rx_done_count++;
            pci_print_log(PCI_LOG_DBG, "rx chan:%d, send netbuf ok, va:0x%p, cnt:%u",
                          i, pst_netbuf, pst_pci_res->st_rx_res.stat.rx_done_count);
            if (pci_dbg_condtion()) {
                oal_print_hex_dump(oal_netbuf_data(pst_netbuf),
                                   oal_netbuf_len(pst_netbuf) <
                                   ul_max_dump_bytes
                                   ? oal_netbuf_len(pst_netbuf)
                                   : ul_max_dump_bytes,
                                   HEX_DUMP_GROUP_SIZE, "d2h payload: ");
            }
            /* unmap dma addr & free netbuf */
            oal_pcie_rx_netbuf_submit(pst_pci_res, pst_netbuf);
        } while (--cnt);
        oal_pcie_mips_end(PCIE_MIPS_RX_QUEUE_POP);

        if (!cnt) {
            /* 一个通道的地址处理完成 */
            pci_print_log(PCI_LOG_DBG, "rx chan:%d all bus process done!", i);
            flag = 1;
        }
    }

    if (oal_unlikely(total_cnt > PCIE_EDMA_WRITE_BUSRT_COUNT)) {
        pst_pci_res->st_rx_res.stat.rx_burst_cnt[0]++;
    } else {
        pst_pci_res->st_rx_res.stat.rx_burst_cnt[total_cnt]++;
    }

    pst_pci_res->st_rx_res.stat.rx_count += total_cnt;

done:
    if (!flag) {
        /* 维测,未找到FIFO中的地址，地址有错，或者count有错,或者丢中断 这里应该触发DFR */
        oal_pcie_linux_res *pst_pci_lres = (oal_pcie_linux_res *)oal_pci_get_drvdata(pcie_res_to_dev(pst_pci_res));
        pci_print_log(PCI_LOG_ERR, "pcie rx done addr error");
        oal_disable_pcie_irq(pst_pci_lres);

        /* DFR trigger */
        oal_pcie_change_link_state(pst_pci_lres->pst_pci_res, PCI_WLAN_LINK_DOWN);
        hcc_bus_exception_submit(pst_pci_lres->pst_bus, WIFI_TRANS_FAIL);

        chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_PLAT, CHR_LAYER_DRV,
                             CHR_PLT_DRV_EVENT_PCIE, CHR_PLAT_DRV_ERROR_D2H_TRANSFER_PCIE_LINK_DOWN);
    } else {
        oal_pcie_linux_res *pst_pci_lres = NULL;
        pci_print_log(PCI_LOG_DBG, "d2h trigger hcc_sched_transfer, dev:%p, lres:%p",
                      pcie_res_to_dev(pst_pci_res), oal_pci_get_drvdata(pcie_res_to_dev(pst_pci_res)));

        pst_pci_lres = (oal_pcie_linux_res *)oal_pci_get_drvdata(pcie_res_to_dev(pst_pci_res));
        /* 发送完成,唤醒发送线程 */
        if (oal_likely(pst_pci_lres != NULL)) {
            if (oal_likely(pst_pci_lres->pst_bus)) {
                hcc_sched_transfer(hbus_to_hcc(pst_pci_lres->pst_bus));
            } else {
                pci_print_log(PCI_LOG_ERR, "lres's bus is null! %p", pst_pci_lres);
            }

            /* 通知线程，补充RX内存 */
            oal_pcie_shced_rx_hi_thread(pst_pci_res);
        }
    }
}
#endif

/* edma read isr */
OAL_STATIC void oal_pcie_h2d_edma_isr(oal_pcie_res *pst_pci_res)
{
    int32_t ret;
    unsigned long flags;
    int32_t i, cnt, total_cnt, netbuf_cnt, curr_cnt;
    oal_netbuf_stru *pst_netbuf = NULL;
    oal_netbuf_head_stru *pst_txq = NULL;
    pcie_dma_read_fifo_item soft_rd_item;
    oal_pcie_linux_res *pst_pci_lres = (oal_pcie_linux_res *)oal_pci_get_drvdata(pcie_res_to_dev(pst_pci_res));

    pci_print_log(PCI_LOG_DBG, "oal_pcie_h2d_edma_isr enter");

    /* 读空soft fifo */
    forever_loop() {
        if (oal_pcie_ringbuf_read_wr(pst_pci_res, PCIE_COMM_RINGBUF_DMA_READ_FIFO) != OAL_SUCC) {
            pci_print_log(PCI_LOG_WARN, "read dma read fifo ringbuf failed");
            break;
        }

        cnt = 0;

        forever_loop() {
            ret = oal_pcie_ringbuf_read(pst_pci_res, PCIE_COMM_RINGBUF_DMA_READ_FIFO,
                                        (uint8_t *)&soft_rd_item, sizeof(soft_rd_item));
            if (ret <= 0) {
                break;
            }

            pst_pci_res->stat.intx_tx_count++;

            pci_print_log(PCI_LOG_DBG, "oal_pcie_h2d_edma_isr, cnt:%u", pst_pci_res->stat.intx_tx_count);
            if ((uint16_t)pst_pci_res->stat.intx_tx_count != soft_rd_item.isr_count) {
                pci_print_log(PCI_LOG_WARN, "tx count:%u not equal to isr count:%u",
                              (uint16_t)pst_pci_res->stat.intx_tx_count, soft_rd_item.isr_count);
            }

            if (oal_unlikely(soft_rd_item.qid >= (uint16_t)PCIE_H2D_QTYPE_BUTT)) {
                pci_print_log(PCI_LOG_ERR, "invalid h2d qid:%u", soft_rd_item.qid);
                oal_print_hex_dump((uint8_t *)&soft_rd_item, OAL_SIZEOF(soft_rd_item),
                                   HEX_DUMP_GROUP_SIZE, "read item: ");
                break;
            }

            pst_txq = &pst_pci_res->st_tx_res[soft_rd_item.qid].txq;
            if (oal_unlikely(oal_netbuf_list_empty(pst_txq))) {
                /* 队列为空 */
                pci_print_log(PCI_LOG_ERR, "invalid read item,qid:%u is empty", soft_rd_item.qid);
                oal_print_hex_dump((uint8_t *)&soft_rd_item, OAL_SIZEOF(soft_rd_item),
                                   HEX_DUMP_GROUP_SIZE, "read item: ");
                break;
            }

            total_cnt = 0;

            for (i = 0; i < PCIE_EDMA_MAX_CHANNELS; i++) {
                total_cnt += soft_rd_item.count[i];
            }

            netbuf_cnt = oal_netbuf_list_len(pst_txq);

            if (oal_unlikely(netbuf_cnt < total_cnt)) {
                /* 队列为空 */
                pci_print_log(PCI_LOG_ERR, "invalid read item,qid:%u had %u pkts less than %u",
                              soft_rd_item.qid, netbuf_cnt, total_cnt);
                oal_print_hex_dump((uint8_t *)&soft_rd_item, OAL_SIZEOF(soft_rd_item),
                                   HEX_DUMP_GROUP_SIZE, "read item: ");
                break;
            }

            /* 保证一个地方入队，这里出队 */
            pst_netbuf = (oal_netbuf_stru *)oal_netbuf_next(pst_txq);
            if (oal_pcie_tx_dma_addr_match_low(pst_netbuf, soft_rd_item.address) != OAL_TRUE) {
                pci_print_log(PCI_LOG_ERR, "[q:%d]tx queue match failed", soft_rd_item.qid);
                oal_print_hex_dump((uint8_t *)&soft_rd_item, OAL_SIZEOF(soft_rd_item),
                                   HEX_DUMP_GROUP_SIZE, "read item: ");
                break;
            }

            curr_cnt = total_cnt;

            do {
                /* 头部出队 */
                oal_spin_lock_irq_save(&pst_pci_res->st_tx_res[soft_rd_item.qid].lock, &flags);
                pst_netbuf = oal_netbuf_delist_nolock(pst_txq);
                oal_spin_unlock_irq_restore(&pst_pci_res->st_tx_res[soft_rd_item.qid].lock, &flags);
                if (oal_unlikely(pst_netbuf == NULL)) {
                    /* 不应该为空，count有可能有问题 */
                    pci_print_log(PCI_LOG_ERR, "[q:%d]tx netbuf queue underflow[curr_cnt:%d:%d], qlen:%d",
                                  soft_rd_item.qid, curr_cnt, total_cnt, netbuf_cnt);
                    declare_dft_trace_key_info("pcie tx done count error", OAL_DFT_TRACE_EXCEP);
                    return;
                }

                /* unmap dma addr & free netbuf */
                pst_pci_res->st_tx_res[soft_rd_item.qid].stat.tx_done_count++;
                pci_print_log(PCI_LOG_DBG, "[q:%d]send netbuf ok, va:0x%p, cnt:%u",
                              soft_rd_item.qid, pst_netbuf,
                              pst_pci_res->st_tx_res[soft_rd_item.qid].stat.tx_done_count);
                oal_pcie_tx_netbuf_free(pst_pci_res, pst_netbuf);
            } while (--total_cnt);

            /* 发送完成,唤醒发送线程 */
            if (oal_likely(pst_pci_lres != NULL)) {
                pci_print_log(PCI_LOG_DBG, "pcie sched hcc thread, qid:%d", soft_rd_item.qid);

                /* 下半部刷新，保持一致性 */
                oal_atomic_set(&pst_pci_res->st_tx_res[soft_rd_item.qid].tx_ringbuf_sync_cond, 1);
                hcc_sched_transfer(hbus_to_hcc(pst_pci_lres->pst_bus));
            }
            /* get read item */
            cnt++;
        }

        if (cnt == 0) {
            pci_print_log(PCI_LOG_DBG, "h2d soft fifo is empty, break");
            break;
        }
    }
}

/* edam write isr */
OAL_STATIC void oal_pcie_d2h_edma_isr(oal_pcie_res *pst_pci_res)
{
    int32_t ret;
    unsigned long flags;
    int32_t i, flag, cnt, total_cnt, netbuf_cnt, curr_cnt;
    const uint32_t ul_max_dump_bytes = 128;
    oal_netbuf_stru *pst_netbuf = NULL;
    oal_netbuf_head_stru *pst_rxq = NULL;
    pcie_dma_write_fifo_item soft_wr_item;

    oal_pcie_linux_res *pst_pci_lres = (oal_pcie_linux_res *)oal_pci_get_drvdata(pcie_res_to_dev(pst_pci_res));

    flag = 0;
    total_cnt = 0;

    pst_rxq = &pst_pci_res->st_rx_res.rxq;

    pci_print_log(PCI_LOG_DBG, "oal_pcie_d2h_edma_isr enter");

    /* 读空soft write fifo */
    forever_loop() {
        if (oal_pcie_ringbuf_read_wr(pst_pci_res, PCIE_COMM_RINGBUF_DMA_WRITE_FIFO) != OAL_SUCC) {
            pci_print_log(PCI_LOG_WARN, "read dma write fifo ringbuf failed");
            break;
        }

        cnt = 0;
        forever_loop() {
            ret = oal_pcie_ringbuf_read(pst_pci_res, PCIE_COMM_RINGBUF_DMA_WRITE_FIFO,
                                        (uint8_t *)&soft_wr_item, sizeof(soft_wr_item));
            if (ret <= 0) {
                break;
            }

            pst_pci_res->stat.intx_rx_count++;
            pci_print_log(PCI_LOG_DBG, "oal_pcie_d2h_edma_isr, cnt:%u", pst_pci_res->stat.intx_rx_count);

            if ((uint16_t)pst_pci_res->stat.intx_rx_count != soft_wr_item.isr_count) {
                pci_print_log(PCI_LOG_WARN, "rx count:%u not equal to isr count:%u",
                              (uint16_t)pst_pci_res->stat.intx_rx_count, soft_wr_item.isr_count);
            }

            if (oal_netbuf_list_empty(pst_rxq)) {
                break;
            }

            total_cnt = 0;
            for (i = 0; i < PCIE_EDMA_MAX_CHANNELS; i++) {
                total_cnt += soft_wr_item.count[i];
            }

            netbuf_cnt = oal_netbuf_list_len(pst_rxq);

            if (oal_unlikely(netbuf_cnt < total_cnt)) {
                /* 队列为空 */
                pci_print_log(PCI_LOG_ERR, "invalid write item, had %u pkts less than %u", netbuf_cnt, total_cnt);
                oal_print_hex_dump((uint8_t *)&soft_wr_item, OAL_SIZEOF(soft_wr_item),
                                   HEX_DUMP_GROUP_SIZE, "write item: ");
                flag = 0;
                goto done;
            }

            /* 保证一个地方入队，这里出队 */
            pst_netbuf = (oal_netbuf_stru *)oal_netbuf_next(pst_rxq);
            if (oal_unlikely(pst_netbuf == NULL)) {
                pci_print_log(PCI_LOG_ERR, "%s error: pst_netbuf is null", __FUNCTION__);
                goto done;
            };

            if (OAL_TRUE != oal_pcie_rx_dma_addr_matchlow(pst_netbuf, soft_wr_item.address)) {
                pci_print_log(PCI_LOG_ERR, "rx queue match failed");
                oal_print_hex_dump((uint8_t *)&soft_wr_item, OAL_SIZEOF(soft_wr_item),
                                   HEX_DUMP_GROUP_SIZE, "write item: ");
                flag = 0;
                goto done;
            }

            curr_cnt = total_cnt;

            /* get the rx netbuf list */
            do {
                /* 这里的锁可以优化 */
                oal_spin_lock_irq_save(&pst_pci_res->st_rx_res.lock, &flags);
                /* 头部出队 */
                pst_netbuf = oal_netbuf_delist_nolock(pst_rxq);
                oal_spin_unlock_irq_restore(&pst_pci_res->st_rx_res.lock, &flags);
                if (pst_netbuf == NULL) {
                    /* 不应该为空，count有可能有问题 */
                    pci_print_log(PCI_LOG_ERR, "rx  netbuf queue underflow[netbuf_cnt:%d, total_count:%d]",
                                  netbuf_cnt, total_cnt);
                    declare_dft_trace_key_info("pcie rx edma done count error", OAL_DFT_TRACE_EXCEP);
                    flag = 0;
                    goto done;
                }
                pst_pci_res->st_rx_res.stat.rx_done_count++;
                pci_print_log(PCI_LOG_DBG, "recv netbuf ok, va:0x%p, cnt:%u",
                              pst_netbuf, pst_pci_res->st_rx_res.stat.rx_done_count);
                if (pci_dbg_condtion()) {
                    oal_print_hex_dump(oal_netbuf_data(pst_netbuf),
                                       oal_netbuf_len(pst_netbuf) <
                                       ul_max_dump_bytes
                                       ? oal_netbuf_len(pst_netbuf)
                                       : ul_max_dump_bytes,
                                       HEX_DUMP_GROUP_SIZE, "d2h payload: ");
                }
                /* unmap dma addr & free netbuf */
                oal_pcie_rx_netbuf_submit(pst_pci_res, pst_netbuf);
            } while (--curr_cnt);

            if (oal_unlikely(total_cnt > PCIE_EDMA_WRITE_BUSRT_COUNT)) {
                pst_pci_res->st_rx_res.stat.rx_burst_cnt[0]++;
            } else {
                pst_pci_res->st_rx_res.stat.rx_burst_cnt[total_cnt]++;
            }

            cnt++;
        }

        if (cnt == 0) {
            pci_print_log(PCI_LOG_DBG, "d2h soft fifo is empty, break");
            break;
        }
    }

    flag = 1;

done:
    if (!flag) {
        /* 维测,未找到FIFO中的地址，地址有错，或者count有错,或者丢中断 这里应该触发DFR */
        pci_print_log(PCI_LOG_ERR, "pcie rx done addr error");
        oal_disable_pcie_irq(pst_pci_lres);

        /* DFR trigger */
        oal_pcie_change_link_state(pst_pci_lres->pst_pci_res, PCI_WLAN_LINK_DOWN);
        hcc_bus_exception_submit(pst_pci_lres->pst_bus, WIFI_TRANS_FAIL);

        chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_PLAT, CHR_LAYER_DRV,
                             CHR_PLT_DRV_EVENT_PCIE, CHR_PLAT_DRV_ERROR_D2H_EDMA_PCIE_LINK_DOWN);
    } else {
        pci_print_log(PCI_LOG_DBG, "d2h trigger hcc_sched_transfer, dev:%p, lres:%p",
                      pcie_res_to_dev(pst_pci_res), oal_pci_get_drvdata(pcie_res_to_dev(pst_pci_res)));
        /* 发送完成,唤醒发送线程 */
        if (oal_likely(pst_pci_lres != NULL)) {
            if (oal_likely(pst_pci_lres->pst_bus)) {
                hcc_sched_transfer(hbus_to_hcc(pst_pci_lres->pst_bus));
            } else {
                pci_print_log(PCI_LOG_ERR, "lres's bus is null! %p", pst_pci_lres);
            }

            /* 通知线程，补充RX内存 */
            oal_pcie_shced_rx_hi_thread(pst_pci_res);
        }
    }
}

#ifdef _PRE_PLAT_FEATURE_PCIE_EDMA_ORI
/* 原生EDMA, Not Finish */
int32_t oal_pcie_transfer_done(oal_pcie_res *pst_pci_res)
{
    int32_t flag, trans_cnt;
    MSG_FIFO_STAT msg_fifo_stat;

    HOST_INTR_STATUS stat, mask;

    if (oal_unlikely(pst_pci_res == NULL)) {
        pci_print_log(PCI_LOG_ERR, "pst_pci_res is null!");
        return;
    }

    pst_pci_res->stat.intx_total_count++;
    pci_print_log(PCI_LOG_DBG, "intx int count:%u", pst_pci_res->stat.intx_total_count);

    /* Host收到intx中断,遍历TX/RX FIFO寄存器 */
    if (oal_unlikely(pst_pci_res->pst_pci_dma_ctrl_base == NULL)) {
        pci_print_log(PCI_LOG_ERR, "fifo base addr is null!");
        pst_pci_res->stat.done_err_cnt++;
        return;
    }

    mask.bits.pcie_edma_tx_intr_status = 1;
    mask.bits.pcie_edma_rx_intr_status = 1;

    trans_cnt = 0;

    do {
        stat.AsDword = oal_readl(pst_pci_res->pst_pci_ctrl_base + PCIE_HOST_INTR_STATUS_OFF);

        stat.AsDword &= mask.AsDword; /* Get mask int */

        if (stat.AsDword == 0) {
            break;
        }

        if (stat.bits.pcie_edma_rx_intr_status) {
            /* 获取当前接收的描述符个数,释放当前队头的netbuf */
#error
            /* device to host edma 传输完成, 触发h2d doorbell通知DEVICE 查中断 */
            oal_pcie_h2d_doorbell(pst_pci_res);
            trans_cnt++;
        }

        if (stat.bits.pcie_edma_tx_intr_status) {
            trans_cnt++;
        }

    } while (1);
}
#else
int32_t oal_pcie_transfer_done(oal_pcie_res *pst_pci_res)
{
    int32_t trans_cnt, old_cnt;
    MSG_FIFO_STAT msg_fifo_stat;

    /* 这里的mask 只是mask 状态位，并不是mask中断， 这里的mask只用来标记是否处理这个中断 */
    HOST_INTR_STATUS stat, mask;

    if (oal_unlikely(pst_pci_res == NULL)) {
        pci_print_log(PCI_LOG_ERR, "pst_pci_res is null!");
        return 0;
    }

    pst_pci_res->stat.intx_total_count++;
    pci_print_log(PCI_LOG_DBG, "intx int count:%u", pst_pci_res->stat.intx_total_count);

    /* Host收到intx中断,遍历TX/RX FIFO寄存器 */
    if (oal_unlikely(pst_pci_res->pst_pci_dma_ctrl_base == NULL)) {
        pci_print_log(PCI_LOG_ERR, "fifo base addr is null!");
        pst_pci_res->stat.done_err_cnt++;
        return 0;
    }

    if (oal_unlikely(pst_pci_res->link_state < PCI_WLAN_LINK_MEM_UP)) {
        pci_print_log(PCI_LOG_ERR, "d2h int link invaild, link_state:%s",
                      oal_pcie_get_link_state_str((pst_pci_res->link_state)));
        pst_pci_res->stat.done_err_cnt++;
        /* linkdown, can't clear intx, disable intx */
        return -OAL_EFAIL;
    }

    mask.AsDword = 0xc;

    trans_cnt = 0;
    old_cnt = trans_cnt;

    do {
        if (oal_unlikely(pst_pci_res->link_state < PCI_WLAN_LINK_MEM_UP)) {
            pci_print_log(PCI_LOG_INFO, "link state is disabled:%s, intx can't process!",
                          oal_pcie_get_link_state_str(pst_pci_res->link_state));
            declare_dft_trace_key_info("pcie mem is not init", OAL_DFT_TRACE_OTHER);
        }

        oal_pcie_mips_start(PCIE_MIPS_RX_INTR_PROCESS);
        stat.AsDword = oal_readl(pst_pci_res->pst_pci_ctrl_base + PCIE_HOST_INTR_STATUS_OFF);
        pci_print_log(PCI_LOG_DBG, "intr status host:0x%8x", stat.AsDword);

        /* Link down check */
        if (oal_unlikely(stat.AsDword == 0xFFFFFFFF)) {
            if (oal_pcie_check_link_state(pst_pci_res) == OAL_FALSE) {
                pci_print_log(PCI_LOG_ERR, "read transfer done int failed, linkdown..");
                return -OAL_EBUSY;
            }
        }

        stat.AsDword &= mask.AsDword; /* Get mask int */

        if (stat.AsDword == 0) {
            oal_pcie_mips_end(PCIE_MIPS_RX_INTR_PROCESS);
            break;
        }

        pci_print_log(PCI_LOG_DBG, "clear host:0x%8x", stat.AsDword);
        oal_writel(stat.AsDword, (pst_pci_res->pst_pci_ctrl_base + PCIE_HOST_INTR_CLR));
        if (g_pcie_ringbuf_bugfix_enable) {
            /* force read, util the clear intr effect */
            oal_readl(pst_pci_res->pst_pci_ctrl_base + PCIE_HOST_DEVICE_REG0);
        }
        oal_pcie_mips_end(PCIE_MIPS_RX_INTR_PROCESS);

        if ((pst_pci_res->revision >= PCIE_REVISION_5_00A) && (g_pcie_soft_fifo_enable)) {
            /* 读空Soft FIFO */
            if (stat.bits.pcie_hw_edma_tx_intr_status) {
                oal_pcie_h2d_edma_isr(pst_pci_res);
            }

            if (stat.bits.pcie_hw_edma_rx_intr_status) {
                oal_pcie_d2h_edma_isr(pst_pci_res);
            }
        } else {
            /* 读空Hardware FIFO */
            forever_loop() {
                old_cnt = trans_cnt;
                oal_pcie_mips_start(PCIE_MIPS_RX_FIFO_STATUS);
                msg_fifo_stat.AsDword = oal_readl(pst_pci_res->pst_pci_ctrl_base + PCIE_MSG_FIFO_STATUS);
                oal_pcie_mips_end(PCIE_MIPS_RX_FIFO_STATUS);
                pci_print_log(PCI_LOG_DBG, "msg_fifo_stat host:0x%8x", msg_fifo_stat.AsDword);

                /* Link down check */
                if (oal_unlikely(msg_fifo_stat.AsDword == 0xFFFFFFFF)) {
                    if (oal_pcie_check_link_state(pst_pci_res) == OAL_FALSE) {
                        pci_print_log(PCI_LOG_ERR, "read message fifo stat failed, linkdown..");
                        return -OAL_EBUSY;
                    }
                }

                if (msg_fifo_stat.bits.rx_msg_fifo0_empty == 0 && msg_fifo_stat.bits.rx_msg_fifo1_empty == 0) {
                    oal_pcie_d2h_transfer_done(pst_pci_res);
                    trans_cnt++;
                }

                if (msg_fifo_stat.bits.tx_msg_fifo0_empty == 0 && msg_fifo_stat.bits.tx_msg_fifo1_empty == 0) {
                    oal_pcie_h2d_transfer_done(pst_pci_res);
                    trans_cnt++;
                }

                if (old_cnt == trans_cnt) {
                    break;
                }
            }
        }

    } while (1);

    pci_print_log(PCI_LOG_DBG, "trans done process %u cnt data", trans_cnt);

    /* 相等说明已经读空 */
    return !(old_cnt == trans_cnt);
}
#endif

/* ringbuf functions */
OAL_STATIC uint32_t oal_pcie_ringbuf_freecount(pcie_ringbuf *pst_ringbuf)
{
    /* 无符号，已经考虑了翻转 */
    uint32_t len = pst_ringbuf->size - (pst_ringbuf->wr - pst_ringbuf->rd);
    if (len == 0) {
        return 0;
    }

    if (len % pst_ringbuf->item_len) {
        pci_print_log(PCI_LOG_ERR, "oal_pcie_ringbuf_freecount, size:%u, wr:%u, rd:%u",
                      pst_ringbuf->size,
                      pst_ringbuf->wr,
                      pst_ringbuf->rd);
        return 0;
    }

    if (pst_ringbuf->item_mask) {
        /* item len 如果是2的N次幂，则移位 */
        len = len >> pst_ringbuf->item_mask;
    } else {
        len /= pst_ringbuf->item_len;
    }
    return len;
}

OAL_STATIC uint32_t oal_pcie_ringbuf_is_empty(pcie_ringbuf *pst_ringbuf)
{
    if (pst_ringbuf->wr == pst_ringbuf->rd) {
        return OAL_TRUE;
    }
    return OAL_FALSE;
}

int32_t oal_pcie_check_link_state(oal_pcie_res *pst_pci_res)
{
    int32_t ret;
    pci_addr_map addr_map;
    pcie_dev_ptr share_mem_address; /* Device cpu地址 */
    oal_pci_dev_stru *pst_pci_dev;

    pst_pci_dev = pcie_res_to_dev(pst_pci_res);

    ret = oal_pcie_inbound_ca_to_va(pst_pci_res, pst_pci_res->chip_info.addr_info.sharemem_addr, &addr_map);
    if (oal_unlikely(ret != OAL_SUCC)) {
        /* share mem 地址未映射! */
        pci_print_log(PCI_LOG_ERR, "can not found mem map for dev cpu address 0x%x\n",
                      pst_pci_res->chip_info.addr_info.sharemem_addr);
        return OAL_FALSE;
    }

    /* Get sharemem's dev_cpu address */
    oal_pcie_io_trans((uintptr_t)&share_mem_address, addr_map.va, sizeof(share_mem_address));

    if (share_mem_address == 0xFFFFFFFF) {
        uint32_t version = 0;

        declare_dft_trace_key_info("pcie_detect_linkdown", OAL_DFT_TRACE_EXCEP);

        ret = oal_pci_read_config_dword(pcie_res_to_dev(pst_pci_res), 0x0, &version);

        /* if pci config succ, pcie link still up, but pci master is down,
         * rc mem port is issue or ep axi bus is gating
         * if pci config failed, pcie maybe link down */
        pci_print_log(PCI_LOG_INFO, "read pci version 0x%8x ret=%d, host wakeup dev gpio:%d", version, ret,
                      hcc_bus_get_sleep_state(((oal_pcie_linux_res *)oal_pci_get_drvdata(
                      pcie_res_to_dev(pst_pci_res)))->pst_bus));
        do {
            oal_pcie_linux_res *pst_pci_lres = (oal_pcie_linux_res *)oal_pci_get_drvdata(pcie_res_to_dev(pst_pci_res));

            oal_pcie_change_link_state(pst_pci_lres->pst_pci_res, PCI_WLAN_LINK_DOWN);

#if defined(_PRE_CONFIG_GPIO_TO_SSI_DEBUG)
            if (board_get_wlan_wkup_gpio_val() == 0) {
                (void)ssi_dump_err_regs(SSI_ERR_PCIE_CHECK_LINK_FAIL);
            } else {
                pci_print_log(PCI_LOG_INFO, "dev wakeup gpio is high, dev maybe panic");
            }
#endif
            oal_pci_disable_device(pcie_res_to_dev(pst_pci_res));
            oal_disable_pcie_irq(pst_pci_lres);
        } while (0);
        return OAL_FALSE;
    } else {
        return OAL_TRUE;
    }
}

void oal_pcie_share_mem_res_unmap(oal_pcie_res *pst_pci_res)
{
    memset_s((void *)&pst_pci_res->dev_share_mem, OAL_SIZEOF(pst_pci_res->dev_share_mem),
             0, OAL_SIZEOF(pst_pci_res->dev_share_mem));
}

/* 调用必须在iATU配置, pcie device 使能之后， */
int32_t oal_pcie_share_mem_res_map(oal_pcie_res *pst_pci_res)
{
    int32_t ret;
    void *pst_share_mem_vaddr = NULL;
    pcie_dev_ptr share_mem_address = 0xFFFFFFFF; /* Device cpu地址 */
    pci_addr_map addr_map, share_mem_map;
    unsigned long timeout, timeout1;
    oal_pci_dev_stru *pst_pci_dev = NULL;

    pst_pci_dev = pcie_res_to_dev(pst_pci_res);

    /* 忙等50ms 若超时 再等10S 超时 */
    timeout = jiffies + msecs_to_jiffies(50);     /* 50ms each timeout */
    timeout1 = jiffies + msecs_to_jiffies(10000); /* 10000ms total timeout */

    ret = oal_pcie_inbound_ca_to_va(pst_pci_res, pst_pci_res->chip_info.addr_info.sharemem_addr, &addr_map);
    if (ret != OAL_SUCC) {
        /* share mem 地址未映射! */
        pci_print_log(PCI_LOG_ERR, "can not found mem map for dev cpu address 0x%x\n",
                      pst_pci_res->chip_info.addr_info.sharemem_addr);
        return ret;
    }

    pst_share_mem_vaddr = (void *)addr_map.va;

    pci_print_log(PCI_LOG_DBG, "device address:0x%x = va:0x%lx",
                  pst_pci_res->chip_info.addr_info.sharemem_addr, addr_map.va);

    forever_loop() {
        /* Get sharemem's dev_cpu address */
        oal_pcie_io_trans((uintptr_t)&share_mem_address, (uintptr_t)pst_share_mem_vaddr, sizeof(share_mem_address));

        /* 通过检查地址转换可以判断读出的sharemem地址是否是有效值 */
        ret = oal_pcie_inbound_ca_to_va(pst_pci_res, share_mem_address, &share_mem_map);
        if (ret == OAL_SUCC) {
            /* Device 初始化完成  & PCIE 通信正常 */
            if (share_mem_address != 0) {
                if (0xFFFFFFFF != share_mem_address) {
                    pci_print_log(PCI_LOG_INFO, "share_mem_address 0x%x", share_mem_address);
                    ret = OAL_SUCC;
                    break;
                }
            }
        }

        if (!time_after(jiffies, timeout)) {
            cpu_relax();
            continue; /* 未超时，继续 */
        }

        /* 50ms 超时, 开始10S超时探测 */
        if (!time_after(jiffies, timeout1)) {
            oal_msleep(1);
            continue; /* 未超时，继续 */
        } else {
            /* 10s+50ms 超时，退出 */
            pci_print_log(PCI_LOG_ERR, "share_mem_address 0x%x, jiffies:0x%lx, timeout:0x%lx, timeout1:0x%lx",
                          share_mem_address, jiffies, timeout, timeout1);
            ret = -OAL_ETIMEDOUT;
            break;
        }
    }

    if (share_mem_map.va != 0 && (ret == OAL_SUCC)) {
        pst_pci_res->dev_share_mem.va = share_mem_map.va;
        pst_pci_res->dev_share_mem.pa = share_mem_map.pa;
        pci_print_log(PCI_LOG_DBG, "share mem va:0x%lx, pa:0x%lx",
                      pst_pci_res->dev_share_mem.va, pst_pci_res->dev_share_mem.pa);
        return OAL_SUCC;
    }

    pci_print_log(PCI_LOG_INFO, "wait device & PCIe boot timeout, 0x%x", share_mem_address);

    (void)ssi_dump_err_regs(SSI_ERR_PCIE_WAIT_BOOT_TIMEOUT);

    return -OAL_EFAIL;
}

/* device shared mem write */
OAL_STATIC int32_t oal_pcie_write_dsm32(oal_pcie_res *pst_pci_res, PCIE_SHARED_DEVICE_ADDR_TYPE type, uint32_t val)
{
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    oal_pci_dev_stru *pst_pci_dev;
    pst_pci_dev = pcie_res_to_dev(pst_pci_res);
#endif

    if (oal_warn_on((uint32_t)type >= (uint32_t)PCIE_SHARED_ADDR_BUTT)) {
        pci_print_log(PCI_LOG_WARN, "invalid device addr type:%d", type);
        return -OAL_EINVAL;
    }

    if (oal_warn_on(pst_pci_res == NULL)) {
        pci_print_log(PCI_LOG_ERR, "pci res is null");
        return -OAL_ENODEV;
    }

    if (oal_warn_on(pst_pci_res->st_device_shared_addr_map[type].va == 0)) {
        pci_print_log(PCI_LOG_ERR, "dsm type:%d va is null", type);
        return -OAL_ENODEV;
    }

    oal_writel(val, (void *)pst_pci_res->st_device_shared_addr_map[type].va);
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    if (pst_pci_dev != NULL) {
        oal_pci_cache_flush(pst_pci_dev, (void *)pst_pci_res->st_device_shared_addr_map[type].pa, sizeof(val));
    }
#endif

    return OAL_SUCC;
}

/* device shared mem read */
OAL_STATIC int32_t oal_pcie_read_dsm32(oal_pcie_res *pst_pci_res, PCIE_SHARED_DEVICE_ADDR_TYPE type, uint32_t *val)
{
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    oal_pci_dev_stru *pst_pci_dev;
    pst_pci_dev = pcie_res_to_dev(pst_pci_res);
#endif

    if (oal_warn_on((uint32_t)type >= (uint32_t)PCIE_SHARED_ADDR_BUTT)) {
        pci_print_log(PCI_LOG_WARN, "invalid device addr type:%d", type);
        return -OAL_EINVAL;
    }

    if (oal_warn_on(pst_pci_res == NULL)) {
        pci_print_log(PCI_LOG_ERR, "pci res is null");
        return -OAL_ENODEV;
    }

    if (oal_warn_on(pst_pci_res->st_device_shared_addr_map[type].va == 0)) {
        pci_print_log(PCI_LOG_ERR, "dsm type:%d va is null", type);
        return -OAL_ENODEV;
    }

#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    if (pst_pci_dev != NULL) {
        /* cache 无效化 */
        oal_pci_cache_inv(pst_pci_dev, (void *)pst_pci_res->st_device_shared_addr_map[type].pa, sizeof(*val));
    }
#endif

    *val = oal_readl((void *)pst_pci_res->st_device_shared_addr_map[type].va);

    return OAL_SUCC;
}

int32_t oal_pcie_set_device_soft_fifo_enable(oal_pcie_res *pst_pci_res)
{
    if (pst_pci_res->revision >= PCIE_REVISION_5_00A) {
        return oal_pcie_write_dsm32(pst_pci_res, PCIE_SHARED_SOFT_FIFO_ENABLE, !!g_pcie_soft_fifo_enable);
    } else {
        return OAL_SUCC;
    }
}

int32_t oal_pcie_set_device_ringbuf_bugfix_enable(oal_pcie_res *pst_pci_res)
{
    return oal_pcie_write_dsm32(pst_pci_res, PCIE_SHARED_RINGBUF_BUGFIX, !!g_pcie_ringbuf_bugfix_enable);
}

int32_t oal_pcie_set_device_aspm_dync_disable(oal_pcie_res *pst_pci_res, uint32_t disable)
{
    uint32_t value = 0;
    int32_t ret = oal_pcie_write_dsm32(pst_pci_res, PCIE_SHARED_ASPM_DYNC_CTL, disable);
    oal_pcie_read_dsm32(pst_pci_res, PCIE_SHARED_ASPM_DYNC_CTL, &value);
    oal_reference(value);
    return ret;
}

int32_t oal_pcie_set_device_dma_check_enable(oal_pcie_res *pst_pci_res)
{
    return oal_pcie_write_dsm32(pst_pci_res, PCIE_SHARED_SOFT_DMA_CHECK, !!g_pcie_dma_data_check_enable);
}

void oal_pcie_device_xfer_pending_sig_clr(oal_pcie_res *pst_pci_res, oal_bool_enum clear)
{
    uint32_t value;
    if (clear != 0) {
        /* clear xfer pending signal, allow L1 enter/exist */
        value = oal_readl(pst_pci_res->pst_pci_ctrl_base + PCIE_LOW_POWER_CFG_OFF);
        value &= ~(1 << 7);
        oal_writel(value, pst_pci_res->pst_pci_ctrl_base + PCIE_LOW_POWER_CFG_OFF);

    } else {
        /* set xfer pending signal, wakeup L1 & disallow enter/exist */
        value = oal_readl(pst_pci_res->pst_pci_ctrl_base + PCIE_LOW_POWER_CFG_OFF);
        value |= (1 << 7);
        oal_writel(value, pst_pci_res->pst_pci_ctrl_base + PCIE_LOW_POWER_CFG_OFF);
    }

    /* flush pcie bus */
    oal_readl(pst_pci_res->pst_pci_ctrl_base + PCIE_LOW_POWER_CFG_OFF);
}


int32_t oal_pcie_device_aspm_init(oal_pcie_res *pst_pci_res)
{
    uint32_t value;

    value = oal_readl(pst_pci_res->pst_pci_ctrl_base + PCIE_CFG_OFF);
    value |= (1 << 1);
    value |= (1 << 3);
    oal_writel(value, pst_pci_res->pst_pci_ctrl_base + PCIE_CFG_OFF);

    oal_pcie_device_xfer_pending_sig_clr(pst_pci_res, OAL_TRUE);

    oal_print_hi11xx_log(HI11XX_LOG_INFO, "cfg reg:0x%x, low_power_cfg reg:0x%x",
                         oal_readl(pst_pci_res->pst_pci_ctrl_base + PCIE_CFG_OFF),
                         oal_readl(pst_pci_res->pst_pci_ctrl_base + PCIE_LOW_POWER_CFG_OFF));

    return OAL_SUCC;
}

/*
 * Prototype    : oal_pcie_devce_l1_disable
 * Description  : 关闭L1
 * Input        : oal_pcie_res *
 * Output       : true means l1 disabled before we set
 */
OAL_STATIC oal_bool_enum oal_pcie_devce_l1_disable(oal_pcie_res *pst_pci_res)
{
    LOW_POWER_CFG_U low_power_cfg;

    low_power_cfg.AsDword = oal_readl(pst_pci_res->pst_pci_ctrl_base + PCIE_LOW_POWER_CFG_OFF);
    if (low_power_cfg.bits.soc_app_xfer_pending == 1) {
        /* l1 disabled */
        return OAL_TRUE;
    }

    low_power_cfg.bits.soc_app_xfer_pending = 1;
    oal_writel(low_power_cfg.AsDword, pst_pci_res->pst_pci_ctrl_base + PCIE_LOW_POWER_CFG_OFF);

    return OAL_FALSE;
}

/*
 * Prototype    : oal_pcie_devce_l1_restore
 * Description  : 恢复L1配置
 * Input        : oal_pcie_res *,  l1_disable : true means disable L1
 * Output       : void
 */
OAL_STATIC void oal_pcie_devce_l1_restore(oal_pcie_res *pst_pci_res, oal_bool_enum l1_disable)
{
    LOW_POWER_CFG_U low_power_cfg;

    low_power_cfg.AsDword = oal_readl(pst_pci_res->pst_pci_ctrl_base + PCIE_LOW_POWER_CFG_OFF);
    low_power_cfg.bits.soc_app_xfer_pending = (l1_disable == OAL_TRUE) ? 1 : 0;
    oal_writel(low_power_cfg.AsDword, pst_pci_res->pst_pci_ctrl_base + PCIE_LOW_POWER_CFG_OFF);
}

/*
 * Prototype    : oal_pcie_device_phy_addr_range_vaild
 * Description  : 判断pcie phy地址范围合法性,IP强相关
 * Input        : addr:phy地址，16bit位宽
 * Output       : oal_bool_enum true means vaild
 */
OAL_STATIC oal_bool_enum oal_pcie_device_phy_addr_range_vaild(uint16_t addr)
{
    if (addr <= PCIE_EP_PHY_SUP_ADDR_END) {
        return OAL_TRUE;
    }

    if ((addr >= PCIE_EP_PHY_LANEN_ADDR_BEGIN) && (addr <= PCIE_EP_PHY_LANEN_ADDR_END)) {
        return OAL_TRUE;
    }

    oal_print_hi11xx_log(HI11XX_LOG_ERR, "invaild device phy addr:0x%x", addr);

    return OAL_FALSE;
}

/*
 * Prototype    : oal_pcie_device_phy_write
 * Description  : 写pcie device phy 寄存器，必须保证PHY在上电状态
 * Input        : addr:phy地址，16bit位宽 , value:写入值
 * Output       : int32_t : OAL_TURE means write succ
 */
OAL_STATIC int32_t oal_pcie_device_phy_write(oal_pcie_res *pst_pci_res,
                                             uint16_t addr, uint16_t value)
{
    int32_t i;
    PCIE_PHY_CFG_U phy_cfg;
    PCIE_PHY_CFG_ADDR_U phy_cfg_addr;
    PCIE_PHY_WR_DATA_U phy_wr_data;

    if (oal_pcie_device_phy_addr_range_vaild(addr) != OAL_TRUE) {
        return -OAL_EINVAL;
    }

    /* set write address */
    phy_cfg_addr.AsDword = 0;
    phy_cfg_addr.bits.phy_cr_addr = (uint32_t)addr;
    oal_writel(phy_cfg_addr.AsDword, pst_pci_res->pst_pci_ctrl_base + PCIE_PHY_CFG_ADDR_OFF);

    /* set write value */
    phy_wr_data.AsDword = 0;
    phy_wr_data.bits.phy_cr_wr_data = (uint32_t)value;
    oal_writel(phy_wr_data.AsDword, pst_pci_res->pst_pci_ctrl_base + PCIE_PHY_WR_DATA_OFF);

    /* trigger write */
    phy_cfg.AsDword = 0;
    phy_cfg.bits.phy_write = 1;     /* 1 means write */
    phy_cfg.bits.phy_config_en = 1; /* pulse */
    oal_writel(phy_cfg.AsDword, pst_pci_res->pst_pci_ctrl_base + PCIE_PHY_CFG_OFF);

    /* check config ready */
    for (i = 0; i < PCIE_EP_PHY_RW_TIMEOUT_CNT; i++) {
        phy_cfg.AsDword = oal_readl(pst_pci_res->pst_pci_ctrl_base + PCIE_PHY_CFG_OFF);
        if (phy_cfg.bits.phy_config_rdy == 1) { /* write done */
            oal_print_hi11xx_log(HI11XX_LOG_INFO, "write phy done addr:0x%x value=0x%x loop=%d", addr, value, i);
            return OAL_SUCC;
        }
    }

    oal_print_hi11xx_log(HI11XX_LOG_ERR, "write phy addr:0x%x value=0x%x timeout=%d", addr, value, i);
    return -OAL_ETIMEDOUT;
}

/*
 * Prototype    : oal_pcie_device_phy_read
 * Description  : 读pcie device phy 寄存器，必须保证PHY在上电状态
 * Input        : addr:phy地址，16bit位宽
 * Output       : int32_t : OAL_TURE means write succ , *pst_value:回读值
 */
OAL_STATIC int32_t oal_pcie_device_phy_read(oal_pcie_res *pst_pci_res,
                                            uint16_t addr, uint16_t *pst_value)
{
    int32_t i;
    PCIE_PHY_CFG_U phy_cfg;
    PCIE_PHY_CFG_ADDR_U phy_cfg_addr;
    PCIE_PHY_RD_DATA_U phy_rd_data;

    if (oal_unlikely(pst_value == NULL)) {
        return -OAL_EINVAL;
    }

    if (oal_unlikely(oal_pcie_device_phy_addr_range_vaild(addr) != OAL_TRUE)) {
        return -OAL_EINVAL;
    }

    /* set read address */
    phy_cfg_addr.AsDword = 0;
    phy_cfg_addr.bits.phy_cr_addr = (uint32_t)addr;
    oal_writel(phy_cfg_addr.AsDword, pst_pci_res->pst_pci_ctrl_base + PCIE_PHY_CFG_ADDR_OFF);

    /* trigger read */
    phy_cfg.AsDword = 0;
    phy_cfg.bits.phy_write = 0;     /* 0 means read */
    phy_cfg.bits.phy_config_en = 1; /* pulse */
    oal_writel(phy_cfg.AsDword, pst_pci_res->pst_pci_ctrl_base + PCIE_PHY_CFG_OFF);

    /* check config ready */
    for (i = 0; i < PCIE_EP_PHY_RW_TIMEOUT_CNT; i++) {
        phy_cfg.AsDword = oal_readl(pst_pci_res->pst_pci_ctrl_base + PCIE_PHY_CFG_OFF);
        if (phy_cfg.bits.phy_config_rdy == 1) { /* read done */
            phy_rd_data.AsDword = oal_readl(pst_pci_res->pst_pci_ctrl_base + PCIE_PHY_RD_DATA_OFF);
            *pst_value = (uint16_t)phy_rd_data.bits.phy_cr_rd_data;
            oal_print_hi11xx_log(HI11XX_LOG_INFO, "read phy done addr:0x%x value=0x%x loop=%d", addr, *pst_value, i);
            return OAL_SUCC;
        }
    }

    oal_print_hi11xx_log(HI11XX_LOG_ERR, "read phy addr:0x%x timeout=%d", addr, i);
    return -OAL_ETIMEDOUT;
}

/*
* Prototype    : oal_pcie_device_phy_los_en_ovrd
* Description  : 设置PCIE PHY  LOS_EN=1, bypass rx_los_en信号
* Input        : pst_pci_res
* Output       : int32_t : OAL_TURE means set succ
*/
OAL_STATIC int32_t oal_pcie_device_phy_los_en_ovrd(oal_pcie_res *pst_pci_res)
{
    uint16_t value;
    int32_t ret;
    oal_bool_enum l1_disabled;

    /* wakeup phy */
    l1_disabled = oal_pcie_devce_l1_disable(pst_pci_res);

    ret = oal_pcie_device_phy_read(pst_pci_res, PCIE_PHY_LANEN_DIG_RX_OVRD_IN_LO_OFF, &value);
    if (ret != OAL_SUCC) {
        oal_pcie_devce_l1_restore(pst_pci_res, l1_disabled);
        return ret;
    }

    /* 13:RX_LOS_EN_OVRD  12:RX_LOS_EN */
    value &= ~((1 << 12) | (1 << 13));
    value |= ((1 << 12) | (1 << 13));

    ret = oal_pcie_device_phy_write(pst_pci_res, PCIE_PHY_LANEN_DIG_RX_OVRD_IN_LO_OFF, value);
    if (ret != OAL_SUCC) {
        oal_pcie_devce_l1_restore(pst_pci_res, l1_disabled);
        return ret;
    }

    ret = oal_pcie_device_phy_read(pst_pci_res, PCIE_PHY_LANEN_DIG_RX_OVRD_IN_LO_OFF, &value);
    if (ret == OAL_SUCC) {
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "PCIE_PHY_LANEN_DIG_RX_OVRD_IN_LO=0x%x", value);
    }

    oal_pcie_devce_l1_restore(pst_pci_res, l1_disabled);

    return OAL_SUCC;
}

/*
* Prototype    : oal_pcie_device_phy_config
* Description  : PCIE PHY配置
* Input        : pst_pci_res
* Output       : int32_t : OAL_TURE means set succ
*/
int32_t oal_pcie_device_phy_config(oal_pcie_res *pst_pci_res)
{
    int32_t ret;
    oal_pci_dev_stru *pst_pci_dev;

    pst_pci_dev = pcie_res_to_dev(pst_pci_res);

    if (pst_pci_dev->device == PCIE_DEVICE_ID_1105) { /* 1105 soc bugfix */
        /* 1105 phy issue bugfix */
        ret = oal_pcie_device_phy_los_en_ovrd(pst_pci_res);
        if (ret != OAL_SUCC) {
            oal_print_hi11xx_log(HI11XX_LOG_INFO, "pcie phy los en ovrd failed=%d", ret);
            return ret;
        }
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "pcie phy los en ovrd succ");
    }

    oal_print_hi11xx_log(HI11XX_LOG_DBG, "pcie device id:0x%x", pst_pci_dev->device);

    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_device_read32_timeout(void *pst_va, uint32_t target,
                                                  uint32_t mask, unsigned long timeout)
{
    uint32_t value = 0xdeadbeaf;
    unsigned long timeout_t;
    timeout_t = jiffies + oal_msecs_to_jiffies(timeout);
    forever_loop() {
        value = oal_readl(pst_va);
        if ((value & mask) == target) {
            return OAL_SUCC;
        }

        if (timeout == 0) {
            return -OAL_EFAIL;
        }

        if (oal_time_after(jiffies, timeout_t)) {
            break;
        }

        cpu_relax();
    }

    oal_print_hi11xx_log(HI11XX_LOG_WARN, "%p expect 0x%x , read 0x%x, mask 0x%x, timeout=%lu ms",
                         pst_va, target, value, mask, timeout);
    return -OAL_ETIMEDOUT;
}

OAL_STATIC int32_t oal_pcie_get_vol_reg_bits_value(uint32_t target_value, oal_reg_bits_stru *pst_reg_bits,
                                                   uint32_t nums, uint32_t *pst_value)
{
    int32_t i;
    oal_reg_bits_stru *pst_tmp = NULL;
    for (i = 0; i < nums; i++) {
        pst_tmp = pst_reg_bits + i;
        if (target_value == pst_tmp->flag) {
            *pst_value = pst_tmp->value;
            oal_print_hi11xx_log(HI11XX_LOG_INFO, "flag:%u matched value:0x%x , [%s]",
                                 target_value, *pst_value, pst_tmp->name);
            return OAL_SUCC;
        }
    }

    return -OAL_ENODEV;
}

int32_t oal_pcie_get_vol_reg_1v8_value(int32_t request_vol, uint32_t *pst_value)
{
    int32_t ret;
    ret = oal_pcie_get_vol_reg_bits_value(request_vol, g_pcie_phy_1v8_bits,
                                          oal_array_size(g_pcie_phy_1v8_bits), pst_value);
    if (ret != OAL_SUCC) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "invalid pcie ldo bias phy 1v8 param =%d mV", request_vol);
        return -OAL_EINVAL;
    }
    return OAL_SUCC;
}
int32_t oal_pcie_get_vol_reg_0v9_value(int32_t request_vol, uint32_t *pst_value)
{
    int32_t ret;
    ret = oal_pcie_get_vol_reg_bits_value(request_vol, g_pcie_phy_0v9_bits,
                                          oal_array_size(g_pcie_phy_0v9_bits), pst_value);
    if (ret != OAL_SUCC) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "invalid pcie ldo bias phy 0v9 param =%d mV", request_vol);
        return -OAL_EINVAL;
    }
    return OAL_SUCC;
}
void oal_pcie_set_voltage_bias_param(uint32_t phy_0v9_bias, uint32_t phy_1v8_bias)
{
    g_pcie_ldo_phy_0v9_param = phy_0v9_bias;
    g_pcie_ldo_phy_1v8_param = phy_1v8_bias;
    oal_print_hi11xx_log(HI11XX_LOG_INFO, "param 0v9=%u 1v8=%u", phy_0v9_bias, phy_1v8_bias);
}

/* 电压拉偏初始化 */
int32_t oal_pcie_voltage_bias_init(oal_pcie_res *pst_pci_res)
{
    /* vp,vptx,vph 降压 5% */
    int32_t ret;
    uint32_t value, phy_0v9_bias, phy_1v8_bias;
    pci_addr_map addr_map;
    void *pst_pmu_cmu_ctrl = NULL;     /* 0x50002000 */
    void *pst_pmu2_cmu_ir_ctrl = NULL; /* 0x50003000 */

    if (oal_warn_on(pst_pci_res == NULL)) {
        return -OAL_ENODEV;
    }

    if (g_pcie_ldo_phy_0v9_param == 0 ||
        g_pcie_ldo_phy_1v8_param == 0) {
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "phy 0v9=%u mV, 1v8=%u mV invaild, pcie vol bias bypass",
                             g_pcie_ldo_phy_0v9_param, g_pcie_ldo_phy_1v8_param);
        return OAL_SUCC;
    }

    ret = oal_pcie_get_vol_reg_bits_value(g_pcie_ldo_phy_0v9_param, g_pcie_phy_0v9_bits,
                                          oal_array_size(g_pcie_phy_0v9_bits), &phy_0v9_bias);
    if (ret != OAL_SUCC) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "invalid pcie ldo bias phy 0v9 param =%d mV", g_pcie_ldo_phy_0v9_param);
        return -OAL_EINVAL;
    }

    ret = oal_pcie_get_vol_reg_bits_value(g_pcie_ldo_phy_1v8_param, g_pcie_phy_1v8_bits,
                                          oal_array_size(g_pcie_phy_1v8_bits), &phy_1v8_bias);
    if (ret != OAL_SUCC) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "invalid pcie ldo bias phy 1v8 param =%d mV", g_pcie_ldo_phy_1v8_param);
        return -OAL_EINVAL;
    }

    ret = oal_pcie_inbound_ca_to_va(pst_pci_res, HI1103_PA_PMU_CMU_CTL_BASE, &addr_map);
    if (oal_unlikely(ret != OAL_SUCC)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "can not found mem map for dev cpu address 0x%x\n",
                             HI1103_PA_PMU_CMU_CTL_BASE);
        return -OAL_EFAIL;
    }

    pst_pmu_cmu_ctrl = (void *)addr_map.va;

    ret = oal_pcie_inbound_ca_to_va(pst_pci_res, HI1103_PA_PMU2_CMU_IR_BASE, &addr_map);
    if (oal_unlikely(ret != OAL_SUCC)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "can not found mem map for dev cpu address 0x%x\n",
                             HI1103_PA_PMU2_CMU_IR_BASE);
        return -OAL_EFAIL;
    }

    pst_pmu2_cmu_ir_ctrl = (void *)addr_map.va;

    /* PCIELDO0V9_CFG0 */
    value = oal_readl(pst_pmu_cmu_ctrl + 0x0128);
    value &= ~(0xf << 8); // 8, bit pos
    value &= ~(0xf << 12); // 12, bit pos
    value |= (phy_0v9_bias << 8);
    value |= (phy_0v9_bias << 12);
    oal_writel(value, (pst_pmu_cmu_ctrl + 0x0128));

    /* PCIELDO1V8_CFG0 */
    value = oal_readl(pst_pmu2_cmu_ir_ctrl + 0x0268);
    value &= ~(0xf << 8);
    value |= (phy_1v8_bias << 8);
    oal_writel(value, (pst_pmu2_cmu_ir_ctrl + 0x0268));

    oal_print_hi11xx_log(HI11XX_LOG_INFO, "pcieldo0v9 reg=0x%x, pcieldo1v8 reg=0x%x",
                         oal_readl(pst_pmu_cmu_ctrl + 0x0128), oal_readl(pst_pmu2_cmu_ir_ctrl + 0x0268));

    return OAL_SUCC;
}

int32_t oal_pcie_device_changeto_high_cpufreq(oal_pcie_res *pst_pci_res)
{
    int32_t ret;
    pci_addr_map addr_map;
    void *pst_glb_ctrl = NULL;         /* 0x50000000 */
    void *pst_pmu_cmu_ctrl = NULL;     /* 0x50002000 */
    void *pst_pmu2_cmu_ir_ctrl = NULL; /* 0x50003000 */
    void *pst_wctrl = NULL;
    const uint32_t ul_dev_read_timeout = 100;

    if (g_ft_pcie_wcpu_max_freq_bypass) {
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "pcie_wcpu_max_freq_bypass");
        return OAL_SUCC;
    }

    ret = oal_pcie_inbound_ca_to_va(pst_pci_res, HI1103_PA_GLB_CTL_BASE_ADDR, &addr_map);
    if (oal_unlikely(ret != OAL_SUCC)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "can not found mem map for dev cpu address 0x%x\n",
                             HI1103_PA_GLB_CTL_BASE_ADDR);
        return -OAL_EFAIL;
    }

    pst_glb_ctrl = (void *)addr_map.va;

    ret = oal_pcie_inbound_ca_to_va(pst_pci_res, HI1103_PA_PMU_CMU_CTL_BASE, &addr_map);
    if (oal_unlikely(ret != OAL_SUCC)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "can not found mem map for dev cpu address 0x%x\n",
                             HI1103_PA_PMU_CMU_CTL_BASE);
        return -OAL_EFAIL;
    }

    pst_pmu_cmu_ctrl = (void *)addr_map.va;

    ret = oal_pcie_inbound_ca_to_va(pst_pci_res, HI1103_PA_PMU2_CMU_IR_BASE, &addr_map);
    if (oal_unlikely(ret != OAL_SUCC)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "can not found mem map for dev cpu address 0x%x\n",
                             HI1103_PA_PMU2_CMU_IR_BASE);
        return -OAL_EFAIL;
    }

    pst_pmu2_cmu_ir_ctrl = (void *)addr_map.va;

    ret = oal_pcie_inbound_ca_to_va(pst_pci_res, HI1103_PA_W_CTL_BASE, &addr_map);
    if (oal_unlikely(ret != OAL_SUCC)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "can not found mem map for dev cpu address 0x%x\n", HI1103_PA_W_CTL_BASE);
        return -OAL_EFAIL;
    }

    pst_wctrl = (void *)addr_map.va;

    oal_print_hi11xx_log(HI11XX_LOG_INFO, "pst_pci_res->revision %u", pst_pci_res->revision);

    if (pst_pci_res->revision == PCIE_REVISION_4_70A) {
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "pcie 4.70a mpw2 freq");
        oal_setl_bit(HI1103_PMU2_CMU_IR_PMU2_CMU_ABB_MAN_SEL_W_6_REG, HI1103_PMU2_CMU_IR_PMU_XLDO_EN_MAN_W_SEL_OFFSET);
        /* open xldo */
        oal_pcie_device_read32_timeout(HI1103_PMU2_CMU_IR_PMU2_CMU_ABB_STS_6_REG,
                                       (HI1103_PMU2_CMU_IR_PMU_LNALDO0_EN_DELAY_STS_MSK
                                       | HI1103_PMU2_CMU_IR_PMU_LNALDO0_EN_STS_MSK),
                                       (HI1103_PMU2_CMU_IR_PMU_LNALDO0_EN_DELAY_STS_MSK
                                       | HI1103_PMU2_CMU_IR_PMU_LNALDO0_EN_STS_MSK),
                                       ul_dev_read_timeout);

        /* frac_slp_sts */
        oal_setl_bit(HI1103_PMU2_CMU_IR_FBDIV_FRAC_SLP_REG, HI1103_PMU2_CMU_IR_FBDIV_FRAC_WKUP_OFFSET);
        oal_pcie_device_read32_timeout(HI1103_PMU2_CMU_IR_FBDIV_FRAC_SLP_REG, 0x0,
                                       (HI1103_PMU2_CMU_IR_FBDIV_FRAC_WKUP_MSK
                                       | HI1103_PMU2_CMU_IR_FBDIV_FRAC_SLP_STS_MSK),
                                       ul_dev_read_timeout);

        /* default is 1 */
        oal_setl_bits(HI1103_PMU2_CMU_IR_REFDIV_REG, 0, 6, 0x1);

        oal_writel(0x32, HI1103_PMU2_CMU_IR_FBDIV_REG);
        oal_writel(0x0, HI1103_PMU2_CMU_IR_FRAC_H_REG);
        oal_writel(0x0, HI1103_PMU2_CMU_IR_FRAC_L_REG);
        oal_writel(0x11, HI1103_PMU2_CMU_IR_POSTDIV_REG);

        /* pull down FOUTVCOPD */
        oal_setl_bit(HI1103_PMU2_CMU_IR_PMU2_CMU_ABB_MAN_SEL_W_8_REG, 4);
        oal_clearl_bit(HI1103_PMU2_CMU_IR_PMU2_CMU_ABB_DBG_SEL_8_REG, 4);
        oal_msleep(1);

        oal_clearl_mask(HI1103_PMU2_CMU_IR_CMU_PD_REG, (BIT4 | BIT3 | BIT1));

        oal_msleep(1);

        /* wait lock */
        oal_pcie_device_read32_timeout(HI1103_PMU2_CMU_IR_CMU_STATUS_GRM_REG, 0x0, 0x1, ul_dev_read_timeout);

        oal_clearl_mask(HI1103_PMU2_CMU_IR_CMU_GT_W_REG, (BIT1 | BIT2 | BIT4 | BIT5));
        oal_clearl_mask(HI1103_PMU2_CMU_IR_CMU_GT_W_REG, (BIT0 | BIT3));

        oal_clearl_bit(HI1103_PMU2_CMU_IR_TCXO_GT_W_REG, 4);

        /* switch to 640M */
        oal_setl_mask(HI1103_W_CTL_WTOPCRG_SOFT_CLKEN_REG, (BIT9 | BIT8));

        /* select 640M */
        oal_setl_bit(HI1103_W_CTL_W_TCXO_SEL_REG, 0);

    } else if (pst_pci_res->revision == PCIE_REVISION_5_00A) {
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "pcie 5.00a pilot freq from 76.8M");
        /* tcxo to 38.4M */
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "tcxo div=0x%x", oal_readl(pst_pmu2_cmu_ir_ctrl + 0x338));

        /* sysldo power chagne from vddio to buck */
        oal_setl_bit(pst_pmu_cmu_ctrl + 0xc8, 8);

        oal_setl_bit(pst_pmu_cmu_ctrl + 0x88, 6);

        oal_setl_bit(pst_pmu_cmu_ctrl + 0x88, 12);
        oal_readl(pst_pmu_cmu_ctrl + 0x88);
        oal_udelay(30);

        oal_setl_bit(pst_pmu_cmu_ctrl + 0x88, 2);
        oal_readl(pst_pmu_cmu_ctrl + 0x88);
        oal_udelay(60);

        oal_setl_bit(pst_pmu_cmu_ctrl + 0x88, 1);
        oal_readl(pst_pmu_cmu_ctrl + 0x88);
        oal_udelay(30);

        oal_clearl_bit(pst_pmu_cmu_ctrl + 0x88, 12);

        oal_setl_bit(pst_pmu_cmu_ctrl + 0x88, 13);

        // clk_960m_sel :2->3, frq=640M
        oal_writel(0x31b, pst_pmu2_cmu_ir_ctrl + 0x328);

        // buck_clamp_sel: 1>3>2
        oal_setl_bit(pst_pmu_cmu_ctrl + 0x170, 0);

        /* open intldo1 */
        oal_setl_bit(pst_pmu2_cmu_ir_ctrl + 0x108, 2);

        oal_pcie_device_read32_timeout((pst_pmu2_cmu_ir_ctrl + 0x11c), 0xc, 0xc, ul_dev_read_timeout);

        /* open xldo */
        oal_setl_bit(pst_pmu2_cmu_ir_ctrl + 0x148, 14);

        oal_pcie_device_read32_timeout((pst_pmu2_cmu_ir_ctrl + 0x15c), 0xc000, 0xc000, ul_dev_read_timeout);

        oal_setl_bits(HI1103_PMU2_CMU_IR_REFDIV_REG, 0, 6, 0x1);
        oal_writel(0x19, HI1103_PMU2_CMU_IR_FBDIV_REG);
        oal_writel(0x0, HI1103_PMU2_CMU_IR_FRAC_H_REG);
        oal_writel(0x0, HI1103_PMU2_CMU_IR_FRAC_L_REG);
        oal_writel(0x11, HI1103_PMU2_CMU_IR_POSTDIV_REG);

        oal_value_mask(pst_pmu2_cmu_ir_ctrl + 0x168, BIT8, BIT9 | BIT8);
        oal_pcie_device_read32_timeout((pst_pmu2_cmu_ir_ctrl + 0x17c), 0x200, 0x200, ul_dev_read_timeout);

        oal_clearl_mask(HI1103_PMU2_CMU_IR_CMU_PD_REG, (BIT4 | BIT3 | BIT1));
        oal_readl(HI1103_PMU2_CMU_IR_CMU_PD_REG);
        oal_msleep(1);

        oal_pcie_device_read32_timeout(pst_pmu2_cmu_ir_ctrl + 0x880, 0x0, 0x1, ul_dev_read_timeout);

        oal_clearl_mask(HI1103_PMU2_CMU_IR_CMU_GT_W_REG, (BIT2 | BIT5));

        oal_clearl_mask(HI1103_PMU2_CMU_IR_CMU_GT_W_REG, (BIT3 | BIT0));

        oal_clearl_bit(HI1103_PMU2_CMU_IR_TCXO_GT_W_REG, 4);

        oal_setl_mask(HI1103_W_CTL_WTOPCRG_SOFT_CLKEN_REG, (BIT8));

        oal_print_hi11xx_log(HI11XX_LOG_INFO, "change 640M set");
        oal_setl_bit(HI1103_W_CTL_W_TCXO_SEL_REG, 0);

        oal_print_hi11xx_log(HI11XX_LOG_INFO, "change 640M wait start");
        oal_msleep(10); /* 防止这里高频切出问题，下面只回读一次 */
        {
            uint32_t value = oal_readl(HI1103_W_CTL_CLKMUX_STS_REG);
            if ((value & 0x2) != 0x2) {
                oal_print_hi11xx_log(HI11XX_LOG_ERR, "change 640M wait failed, clkmux=0x%x", value);
                ssi_dump_device_regs(SSI_MODULE_MASK_COMM);
#ifdef _PRE_COMMENT_CODE_
                return ret;
#endif
            } else {
                oal_print_hi11xx_log(HI11XX_LOG_INFO, "change 640M wait ok, clkmux=0x%x", value);
            }
        }
    } else {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "unsupport revision: %u", pst_pci_res->revision);
        return -OAL_ENODEV;
    }

    ret = oal_pcie_device_check_alive(pst_pci_res);
    if (ret) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "change 640M failed");
        ssi_dump_device_regs(SSI_MODULE_MASK_ARM_REG);
        return ret;
    }

    oal_print_hi11xx_log(HI11XX_LOG_INFO, "change 640M done");
    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_device_check_alive(oal_pcie_res *pst_pci_res)
{
    int32_t ret;
    uint32_t value;
    pci_addr_map addr_map;

    ret = oal_pcie_inbound_ca_to_va(pst_pci_res, 0x50000000, &addr_map);
    if (oal_unlikely(ret != OAL_SUCC)) {
        /* share mem 地址未映射! */
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "can not found mem map for dev cpu address 0x%x\n", 0x50000000);
        return -OAL_EFAIL;
    }

    value = oal_readl((void *)addr_map.va);
    if (value == 0x101) {
        return OAL_SUCC;
    } else {
        oal_print_hi11xx_log(HI11XX_LOG_WARN, "pcie maybe linkdown, glbctrl=0x%x", value);
        return -OAL_EFAIL;
    }
}

OAL_STATIC int32_t oal_pcie_device_scan_reg(oal_pcie_res *pst_pci_res)
{
    return OAL_SUCC;
}

int32_t oal_pcie_get_gen_mode(oal_pcie_res *pst_pci_res)
{
    uint32_t value = (uint32_t)oal_readl(pst_pci_res->pst_pci_ctrl_base + PCIE_STATUS2_OFF);
    value = ((value >> 9) & 0x1);
    if (value == 0) {
        return PCIE_GEN1;
    } else {
        return PCIE_GEN2;
    }
}

OAL_STATIC int32_t oal_pcie_print_device_aer_cap_reg(oal_pcie_res *pst_pci_res)
{
    int32_t  pos_cap_aer;
    uint32_t uncor = 0;
    uint32_t cor   = 0;
    oal_pci_dev_stru *pst_pci_dev;
    pst_pci_dev = pcie_res_to_dev(pst_pci_res);

    pos_cap_aer = oal_pci_find_ext_capability(pst_pci_dev, PCI_EXT_CAP_ID_ERR);
    if (pos_cap_aer == 0) {
        return -OAL_EFAIL;
    }

    /* 状态寄存器读清 */
    if (oal_pci_read_config_dword(pst_pci_dev, pos_cap_aer + PCI_ERR_UNCOR_STATUS, &uncor)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "PCI_ERR_UNCOR_STATUS: read fail");
        return -OAL_EFAIL;
    }
    oal_print_hi11xx_log(HI11XX_LOG_INFO, "PCI_ERR_UNCOR_STATUS: 0x%x", uncor);

    if (oal_pci_read_config_dword(pst_pci_dev, pos_cap_aer + PCI_ERR_COR_STATUS, &cor)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "PCI_ERR_COR_STATUS: read fail");
        return -OAL_EFAIL;
    }
    oal_print_hi11xx_log(HI11XX_LOG_INFO, "PCI_ERR_COR_STATUS: 0x%x", cor);
#ifdef _PRE_COMMENT_CODE_
    oal_pci_write_config_dword(pst_pci_dev, pos_cap_aer + PCI_ERR_UNCOR_STATUS, uncor);
    oal_pci_write_config_dword(pst_pci_dev, pos_cap_aer + PCI_ERR_COR_STATUS, cor);
#endif
    return OAL_SUCC;
}

int32_t oal_pcie_unmask_device_link_erros(oal_pcie_res *pst_pci_res)
{
    uint32_t ucor_mask = 0;
    int32_t pos_cap_aer;
    oal_pci_dev_stru *pst_pci_dev;
    pst_pci_dev = pcie_res_to_dev(pst_pci_res);

    pos_cap_aer = oal_pci_find_ext_capability(pst_pci_dev, PCI_EXT_CAP_ID_ERR);
    if (pos_cap_aer == 0) {
        return -OAL_EFAIL;
    }

    if (oal_pci_read_config_dword(pst_pci_dev, pos_cap_aer + PCI_ERR_UNCOR_MASK, &ucor_mask)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "PCI_ERR_UNCOR_MASK: read fail");
        return -OAL_EFAIL;
    }

    oal_print_hi11xx_log(HI11XX_LOG_INFO, "PCI_ERR_UNCOR_MASK: 0x%x", ucor_mask);

    /* bit 22, 26 unmask , vendor suggest */
    ucor_mask = 0x0;

    oal_pci_write_config_dword(pst_pci_dev, pos_cap_aer + PCI_ERR_UNCOR_MASK, ucor_mask);

    return OAL_SUCC;
}

int32_t oal_pcie_check_device_link_errors(oal_pcie_res *pst_pci_res)
{
    PCIE_MSG_INTR_STATUS msg_intr_status, msg_mask;

    msg_mask.AsDword = 0x0;
    msg_intr_status.bits.soc_pcie_send_nf_err_status = 1;
    msg_intr_status.bits.soc_pcie_send_f_err_status = 1;
    msg_intr_status.bits.soc_pcie_send_cor_err_status = 1;

    msg_intr_status.AsDword = oal_readl(pst_pci_res->pst_pci_ctrl_base + PCIE_MSG_INTR_STATUS_OFF);
    msg_intr_status.AsDword &= msg_intr_status.AsDword;

    if (msg_intr_status.bits.soc_pcie_send_f_err_status) {
        /* 链路信号极差 */
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "f_err found, intr_status=0x%8x", msg_intr_status.AsDword);
        oal_pcie_print_device_aer_cap_reg(pst_pci_res);
        /* Clear the int */
        oal_writel(msg_intr_status.AsDword, (pst_pci_res->pst_pci_ctrl_base + PCIE_MSG_INTR_CLR_OFF));
        declare_dft_trace_key_info("soc_pcie_send_f_err", OAL_DFT_TRACE_SUCC);
        if (g_ft_pcie_link_err_bypass) {
            oal_print_hi11xx_log(HI11XX_LOG_INFO, "pcie_link_err_bypass");
            return OAL_SUCC;
        } else {
            return -OAL_EFAIL;
        }
    }

    if (msg_intr_status.bits.soc_pcie_send_nf_err_status) {
        /* 链路信号差 */
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "nf_err found, intr_status=0x%8x", msg_intr_status.AsDword);
        oal_pcie_print_device_aer_cap_reg(pst_pci_res);
        /* Clear the int */
        oal_writel(msg_intr_status.AsDword, (pst_pci_res->pst_pci_ctrl_base + PCIE_MSG_INTR_CLR_OFF));
        declare_dft_trace_key_info("soc_pcie_send_nf_err", OAL_DFT_TRACE_SUCC);
        if (g_ft_pcie_link_err_bypass) {
            oal_print_hi11xx_log(HI11XX_LOG_INFO, "pcie_link_err_bypass");
            return OAL_SUCC;
        } else {
            return -OAL_EFAIL;
        }
    }

    if (msg_intr_status.bits.soc_pcie_send_cor_err_status) {
        /* 可忽略的错误 */
        oal_print_hi11xx_log(HI11XX_LOG_WARN, "cor_err found, intr_status=0x%8x", msg_intr_status.AsDword);
        oal_pcie_print_device_aer_cap_reg(pst_pci_res);
        declare_dft_trace_key_info("soc_pcie_send_cor_err", OAL_DFT_TRACE_SUCC);
    }

    /* Clear the int */
    oal_writel(msg_intr_status.AsDword, (pst_pci_res->pst_pci_ctrl_base + PCIE_MSG_INTR_CLR_OFF));

    /* pass */
    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_device_mem_check_burst(oal_pcie_res *pst_pci_res, uint32_t burst_size,
                                                   uintptr_t start, unsigned long length, uint8_t test_value)
{
    uint32_t i;
    int32_t ret;
    void *pst_ddr_buf = NULL;
    uint32_t verify, value;
    unsigned long remain_size, offset, copy_size, total_size;

    pci_addr_map addr_map;

    if ((length & 0x7) || (start & 0x7)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "length %lu, cpu address 0x%lx is invalid", length, start);
        return -OAL_EFAUL;
    }

    ret = oal_pcie_inbound_ca_to_va(pst_pci_res, start + length - 1, &addr_map);
    if (ret) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "cpu address start:0x%lx + length:0x%lx -1 invalid", start, length);
        return ret;
    }

    ret = oal_pcie_inbound_ca_to_va(pst_pci_res, start, &addr_map);
    if (ret) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "cpu address start:0x%lx", start);
        return ret;
    }

    pst_ddr_buf = oal_memalloc(burst_size);
    if (pst_ddr_buf == NULL) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "alloc mem %u bytes failed", burst_size);
        return -OAL_ENOMEM;
    }

    memset_s(pst_ddr_buf, burst_size, test_value, burst_size);

    /* 先连续写再连续读，连续写性能最优 */
    remain_size = length;
    offset = 0;
    total_size = 0;
    verify = (test_value | test_value << 8 | test_value << 16 | test_value << 24);

    forever_loop() {
        if (remain_size == 0) {
            break;
        }

        copy_size = oal_min(burst_size, remain_size);

        /* h2d write */
        oal_pcie_io_trans(addr_map.va + offset, (uintptr_t)pst_ddr_buf, copy_size);

        /* d2h read */
        oal_pcie_io_trans((uintptr_t)pst_ddr_buf, addr_map.va + offset, copy_size);

        /* memcheck */
        for (i = 0; i < copy_size; i += sizeof(uint32_t)) {
            value = *(uint32_t *)(pst_ddr_buf + i);
            if (value != verify) {
                oal_print_hi11xx_log(HI11XX_LOG_ERR,
                                     "mem check failed, cpu address : 0x%lx , \
                                     write 0x%x , read 0x%x, offset =%lu, i = %u",
                                     start + offset + i, verify, value, offset, i);
                oal_print_hex_dump((uint8_t *)pst_ddr_buf, copy_size, HEX_DUMP_GROUP_SIZE, "burst check ");
                oal_print_hex_dump((uint8_t *)(uintptr_t)(addr_map.va + offset),
                                   copy_size, HEX_DUMP_GROUP_SIZE, "bcpu check ");
                oal_free(pst_ddr_buf);
                return -OAL_EFAIL;
            }
        }

        offset += copy_size;
        total_size += copy_size;
        remain_size -= copy_size;
    }

    if (total_size != length) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "wrong total_size=%lu ,length=%lu", total_size, length);
        oal_free(pst_ddr_buf);
        return -OAL_EFAIL;
    }

    oal_free(pst_ddr_buf);
    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_device_mem_check_word(oal_pcie_res *pst_pci_res,
                                                  uintptr_t start, unsigned long length, uint8_t test_value)
{
    uint32_t i;
    int32_t ret;
    uint32_t verify, value;
    declare_time_cost_stru(cost);

    pci_addr_map addr_map;

    oal_get_time_cost_start(cost);

    if ((length & 0x7) || (start & 0x7)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "length %lu, cpu address 0x%lx is invalid", length, start);
        return -OAL_EFAUL;
    }

    ret = oal_pcie_inbound_ca_to_va(pst_pci_res, start + length - 1, &addr_map);
    if (ret) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "cpu address start:0x%lx + length:0x%lx -1 invalid", start, length);
        return ret;
    }

    ret = oal_pcie_inbound_ca_to_va(pst_pci_res, start, &addr_map);
    if (ret) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "cpu address start:0x%lx", start);
        return ret;
    }

    verify = (test_value | test_value << 8 | test_value << 16 | test_value << 24);

    for (i = 0; i < (int32_t)(uint32_t)length; i += sizeof(uint32_t)) { /* 每次偏移4字节 */
        oal_writel(verify, (void *)(addr_map.va + i));
        value = oal_readl((void *)(addr_map.va + i));
        if (oal_unlikely(verify != value)) {
            oal_print_hi11xx_log(HI11XX_LOG_ERR, "cpu addr:0x%lx check failed, read=0x%x write=0x%x",
                                 start + i, value, verify);
            return -OAL_EFAIL;
        }
    }

    oal_get_time_cost_end(cost);
    oal_calc_time_cost_sub(cost);
    oal_print_hi11xx_log(HI11XX_LOG_INFO, "memcheck_byword 0x%lx length %lu, cost %llu us",
                         start, length, time_cost_var_sub(cost));

    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_device_mem_check(oal_pcie_res *pst_pci_res, uintptr_t start, unsigned long length)
{
    int32_t ret;
    int32_t i;
    const uint32_t ul_burst_size = 4096;

    if (g_ft_pcie_wcpu_mem_check_burst_check) {
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "pcie_wcpu_mem_check_burst_check_bypass");
    } else {
        for (i = 0; i < g_ft_pcie_wcpu_mem_check_times; i++) {
            ret = oal_pcie_device_mem_check_burst(pst_pci_res, ul_burst_size, start,
                                                  length, PCIE_MEM_SCAN_VERFIY_VAL_1);
            if (ret) {
                return ret;
            }

            ret = oal_pcie_device_mem_check_burst(pst_pci_res, ul_burst_size, start,
                                                  length, PCIE_MEM_SCAN_VERFIY_VAL_2);
            if (ret) {
                return ret;
            }

            oal_schedule();
        }
    }

    if (g_ft_pcie_wcpu_mem_check_byword_bypass) {
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "pcie_wcpu_mem_check_byword_bypass");
    } else {
        for (i = 0; i < g_ft_pcie_wcpu_mem_check_times; i++) {
            ret = oal_pcie_device_mem_check_word(pst_pci_res, start, length, PCIE_MEM_SCAN_VERFIY_VAL_1);
            if (ret) {
                return ret;
            }

            ret = oal_pcie_device_mem_check_word(pst_pci_res, start, length, PCIE_MEM_SCAN_VERFIY_VAL_2);
            if (ret) {
                return ret;
            }

            oal_schedule();
        }
    }

    oal_print_hi11xx_log(HI11XX_LOG_INFO, "mem check address: 0x%lx , length: 0x%lx  pass!", start, length);

    return OAL_SUCC;
}

/* 一次性写入全部的地址值 */
OAL_STATIC int32_t oal_pcie_device_mem_write_address_onetime(oal_pcie_res *pst_pci_res, uintptr_t start,
                                                             unsigned long length)
{
    int32_t ret;
    int32_t i;
    unsigned long remain_size, offset, copy_size;
    void *pst_ddr_buf = NULL;

    pci_addr_map addr_map;

    if ((length & 0x7) || (start & 0x7)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "length %lu, cpu address 0x%lx is invalid", length, start);
        return -OAL_EFAUL;
    }

    ret = oal_pcie_inbound_ca_to_va(pst_pci_res, start + length - 1, &addr_map);
    if (ret) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "cpu address start:0x%lx + length:0x%lx -1 invalid", start, length);
        return ret;
    }

    ret = oal_pcie_inbound_ca_to_va(pst_pci_res, start, &addr_map);
    if (ret) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "cpu address start:0x%lx", start);
        return ret;
    }

    pst_ddr_buf = oal_memalloc(PAGE_SIZE);
    if (pst_ddr_buf == NULL) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "alloc mem %lu failed", PAGE_SIZE);
        return -OAL_ENOMEM;
    }

    remain_size = length;
    offset = 0;

    /* 4字节写 */
    forever_loop() {
        if (remain_size == 0) {
            break;
        }

        copy_size = oal_min(remain_size, PAGE_SIZE);

        for (i = 0; i < copy_size; i += sizeof(uint32_t)) {      /* 每次偏移4字节 */
            *(uint32_t *)(pst_ddr_buf + i) = start + offset + i; /* CPU地址 */
        }

        oal_pcie_io_trans(addr_map.va + offset, (uintptr_t)pst_ddr_buf, copy_size);

        offset += copy_size;

        remain_size -= copy_size;
    }

    oal_free(pst_ddr_buf);

    oal_print_hi11xx_log(HI11XX_LOG_INFO, "device_mem_write_address_onetime start:0x%lx length:%lu", start, length);

    oal_schedule();

    return OAL_SUCC;
}

/* 一次性读出全部的地址值并且校验 */
OAL_STATIC int32_t oal_pcie_device_mem_read_address_onetime(oal_pcie_res *pst_pci_res, uintptr_t start,
                                                            unsigned long length)
{
    int32_t ret;
    int32_t i;
    unsigned long remain_size, offset, copy_size;
    void *pst_ddr_buf = NULL;

    pci_addr_map addr_map;

    if ((length & 0x7) || (start & 0x7)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "length %lu, cpu address 0x%lx is invalid", length, start);
        return -OAL_EFAUL;
    }

    ret = oal_pcie_inbound_ca_to_va(pst_pci_res, start + length - 1, &addr_map);
    if (ret) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "cpu address start:0x%lx + length:0x%lx -1 invalid", start, length);
        return ret;
    }

    ret = oal_pcie_inbound_ca_to_va(pst_pci_res, start, &addr_map);
    if (ret) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "cpu address start:0x%lx", start);
        return ret;
    }

    pst_ddr_buf = oal_memalloc(PAGE_SIZE);
    if (pst_ddr_buf == NULL) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "alloc mem %lu failed", PAGE_SIZE);
        return -OAL_ENOMEM;
    }

    remain_size = length;
    offset = 0;

    /* 4字节写 */
    forever_loop() {
        if (remain_size == 0) {
            break;
        }

        copy_size = oal_min(remain_size, PAGE_SIZE);

        oal_pcie_io_trans((uintptr_t)pst_ddr_buf, addr_map.va + offset, copy_size);

        for (i = 0; i < copy_size; i += sizeof(uint32_t)) { /* 每次偏移4字节 */
            uint32_t value = *((uint32_t *)(pst_ddr_buf + i));
            uint32_t cpu_address = start + offset + i; /* CPU地址 */
            if (oal_unlikely(value != cpu_address)) {
                oal_print_hi11xx_log(HI11XX_LOG_ERR,
                                     "mem check address verify failed, [0x%lx--0x%lx] at 0x%x, write 0x%x read 0x%x",
                                     start, start + length - 1, cpu_address, cpu_address, value);
                oal_print_hex_dump((uint8_t *)pst_ddr_buf, copy_size, HEX_DUMP_GROUP_SIZE, "address check ");
                oal_print_hex_dump((uint8_t *)(uintptr_t)(addr_map.va + offset),
                                   copy_size, HEX_DUMP_GROUP_SIZE, "pcie mem ");
                oal_free(pst_ddr_buf);
                return -OAL_EFAIL;
            }
        }

        offset += copy_size;

        remain_size -= copy_size;
    }

    oal_free(pst_ddr_buf);

    oal_print_hi11xx_log(HI11XX_LOG_INFO, "device_mem_read_address_onetime start:0x%lx length:%lu", start, length);

    oal_schedule();

    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_device_mem_performance(oal_pcie_res *pst_pci_res, uintptr_t start,
                                                   unsigned long length, unsigned long runtime)
{
    int32_t ret;
    unsigned long timeout;
    unsigned long offset = 0;
    unsigned long remain_size, copy_size, total_size;
    unsigned long burst_size = 4096;
    uint64_t trans_size, us_to_s;
    declare_time_cost_stru(cost);

    pci_addr_map addr_map;

    void *pst_buf = NULL;

    if ((length & 0x7) || (start & 0x7)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "start: %lu , length %lu is invalid", start, length);
        return -OAL_EFAUL;
    }

    ret = oal_pcie_inbound_ca_to_va(pst_pci_res, start + length - 1, &addr_map);
    if (ret) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "cpu address start:0x%lx + length:0x%lx -1 invalid", start, length);
        return ret;
    }

    ret = oal_pcie_inbound_ca_to_va(pst_pci_res, start, &addr_map);
    if (ret) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "cpu address start:0x%lx", start);
        return ret;
    }

    pst_buf = oal_memalloc(burst_size);
    if (pst_buf == NULL) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "alloc perf mem %lu bytes failed", burst_size);
        return -OAL_ENOMEM;
    }

    if (g_ft_pcie_perf_wr_bypass) {
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "pcie_perf_wr_bypass");
    } else {
        oal_get_time_cost_start(cost);

        /* 写性能, 写可以覆盖读 */
        remain_size = 0;
        total_size = 0;

        timeout = jiffies + oal_msecs_to_jiffies(runtime);
        forever_loop() {
            if (oal_time_after(jiffies, timeout)) {
                break;
            }

            if (remain_size == 0) {
                /* repeat */
                remain_size = length;
                offset = 0;
            }

            copy_size = oal_min(burst_size, remain_size);

            oal_pcie_io_trans(addr_map.va + offset, (uintptr_t)pst_buf, copy_size);

            offset += copy_size;
            total_size += copy_size;

            remain_size -= copy_size;

            cpu_relax();
        }
        oal_get_time_cost_end(cost);
        oal_calc_time_cost_sub(cost);

        us_to_s = time_cost_var_sub(cost);
        trans_size = ((total_size * 1000u * 1000u) >> PCIE_TRANS_US_OFFSET_BITS);
        trans_size = div_u64(trans_size, us_to_s);

        oal_print_hi11xx_log(HI11XX_LOG_INFO,
                             "Write(H2D) Thoughtput: %llu Mbps, trans_time:%llu us, tran_size:%lu, address:0x%lx",
                             trans_size, us_to_s, total_size, start);
    }

    if (g_ft_pcie_perf_rd_bypass) {
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "pcie_perf_wr_bypass");
    } else {
        oal_get_time_cost_start(cost);

        /* 读性能, 写可以覆盖读 */
        remain_size = 0;
        total_size = 0;

        timeout = jiffies + oal_msecs_to_jiffies(runtime);
        forever_loop() {
            if (oal_time_after(jiffies, timeout)) {
                break;
            }

            if (remain_size == 0) {
                /* repeat */
                remain_size = length;
                offset = 0;
            }

            copy_size = oal_min(burst_size, remain_size);

            oal_pcie_io_trans((uintptr_t)pst_buf, addr_map.va + offset, copy_size);

            offset += copy_size;
            total_size += copy_size;

            remain_size -= copy_size;

            cpu_relax();
        }
        oal_get_time_cost_end(cost);
        oal_calc_time_cost_sub(cost);

        us_to_s = time_cost_var_sub(cost);
        trans_size = ((total_size * 1000u * 1000u) >> PCIE_TRANS_US_OFFSET_BITS);
        trans_size = div_u64(trans_size, us_to_s);

        oal_print_hi11xx_log(HI11XX_LOG_INFO,
                             "Read(D2H) Thoughtput: %llu Mbps, trans_time:%llu us, tran_size:%lu, address:0x%lx",
                             trans_size, us_to_s, total_size, start);
    }

    ret = oal_pcie_device_check_alive(pst_pci_res);
    if (ret) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "performance write failed!");
        oal_free(pst_buf);
        return ret;
    }

    oal_print_hi11xx_log(HI11XX_LOG_INFO, "mem performance done");

    oal_free(pst_buf);

    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_device_get_scan_wmem_info(oal_pcie_res *pst_pci_res, uintptr_t (**pst_scan_base)[],
                                                      uint32_t *pst_scan_nums)
{
    *pst_scan_base = NULL;
    *pst_scan_nums = 0;
    if (get_hi110x_subchip_type() == BOARD_VERSION_HI1105) {
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "1105 %s wmem scan", hi110x_is_asic() ? "asic" : "fpga");
        if (hi110x_is_asic()) {
            *pst_scan_nums = sizeof(g_hi1105_asic_wmem_scan_array) /
                             (PCIE_MEM_MSG_SIZE * sizeof(g_hi1105_asic_wmem_scan_array[0][0]));
            *pst_scan_base = g_hi1105_asic_wmem_scan_array;
        } else {
            *pst_scan_nums = sizeof(g_hi1105_fpga_wmem_scan_array) /
                             (PCIE_MEM_MSG_SIZE * sizeof(g_hi1105_fpga_wmem_scan_array[0][0]));
            *pst_scan_base = g_hi1105_fpga_wmem_scan_array;
        }
    } else {
        if (pst_pci_res->revision == PCIE_REVISION_4_70A) {
            *pst_scan_nums = sizeof(g_mpw2_wmem_scan_array) /
                             (PCIE_MEM_MSG_SIZE * sizeof(g_mpw2_wmem_scan_array[0][0]));
            *pst_scan_base = g_mpw2_wmem_scan_array;
        } else if (pst_pci_res->revision == PCIE_REVISION_5_00A) {
            *pst_scan_nums = sizeof(g_pilot_wmem_scan_array) /
                             (PCIE_MEM_MSG_SIZE * sizeof(g_pilot_wmem_scan_array[0][0]));
            *pst_scan_base = g_pilot_wmem_scan_array;
        } else {
            oal_print_hi11xx_log(HI11XX_LOG_ERR, "unkown pcie ip revision :0x%x", pst_pci_res->revision);
            return -OAL_ENODEV;
        }
    }

    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_device_scan_wmem(oal_pcie_res *pst_pci_res)
{
    uint32_t i;
    int32_t ret;
    unsigned long cpu_start, cpu_end, mem_len;
    uint32_t scan_nums;
    uintptr_t(*pst_scan_base)[PCIE_MEM_MSG_SIZE];

    declare_time_cost_stru(cost);

    oal_print_hi11xx_log(HI11XX_LOG_INFO, "start scan wmem..");

    ret = oal_pcie_device_get_scan_wmem_info(pst_pci_res, &pst_scan_base, &scan_nums);
    if (ret != OAL_SUCC) {
        /* bypass */
        return OAL_SUCC;
    }

    oal_get_time_cost_start(cost);

    for (i = 0; i < scan_nums; i++) {
        oal_print_hi11xx_log(HI11XX_LOG_DBG, "i:%u, %pK", i, pst_scan_base + i);
        cpu_start = pst_scan_base[i][0];
        cpu_end = pst_scan_base[i][1];
        if (cpu_end <= cpu_start) {
            oal_print_hi11xx_log(HI11XX_LOG_ERR, "invalid mem region, start=0x%lx, end=0x%lx, i:%d",
                                 cpu_start, cpu_end, i);
            return -OAL_EFAUL;
        }

        mem_len = cpu_end - cpu_start + 1;

        ret = oal_pcie_device_mem_check(pst_pci_res, cpu_start, mem_len);
        if (ret) {
            return ret;
        }
    }

    oal_get_time_cost_end(cost);
    oal_calc_time_cost_sub(cost);
    oal_print_hi11xx_log(HI11XX_LOG_INFO, "device wmem done cost %llu us, regions :%u",
                         time_cost_var_sub(cost), scan_nums);

    if (g_ft_pcie_write_address_bypass) {
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "wmem write_address_bypass");
    } else {
        oal_get_time_cost_start(cost);
        /* 连续写，连续读，Soc要求写入当前地址的值 */
        for (i = 0; i < scan_nums; i++) {
            cpu_start = pst_scan_base[i][0];
            cpu_end = pst_scan_base[i][1];

            mem_len = cpu_end - cpu_start + 1;

            ret = oal_pcie_device_mem_write_address_onetime(pst_pci_res, cpu_start, mem_len);
            if (ret) {
                return ret;
            }
        }

        for (i = 0; i < scan_nums; i++) {
            cpu_start = pst_scan_base[i][0];
            cpu_end = pst_scan_base[i][1];

            mem_len = cpu_end - cpu_start + 1;

            ret = oal_pcie_device_mem_read_address_onetime(pst_pci_res, cpu_start, mem_len);
            if (ret) {
                return ret;
            }
        }

        oal_get_time_cost_end(cost);
        oal_calc_time_cost_sub(cost);
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "wmem address check done cost %llu us", time_cost_var_sub(cost));
    }

    if (g_ft_pcie_perf_wr_bypass) {
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "pcie_perf_wr_bypass");
    } else {
        oal_get_time_cost_start(cost);
        for (i = 0; i < scan_nums; i++) {
            cpu_start = pst_scan_base[i][0];
            cpu_end = pst_scan_base[i][1];
            if (cpu_end <= cpu_start) {
                oal_print_hi11xx_log(HI11XX_LOG_ERR, "invalid mem region, start=0x%lx, end=0x%lx, i:%d",
                                     cpu_start, cpu_end, i);
                return -OAL_EFAUL;
            }

            mem_len = cpu_end - cpu_start + 1;

            ret = oal_pcie_device_mem_performance(pst_pci_res, cpu_start, mem_len, g_ft_pcie_perf_runtime);
            if (ret) {
                return ret;
            }
        }
        oal_get_time_cost_end(cost);
        oal_calc_time_cost_sub(cost);
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "mem performance done cost %llu us", time_cost_var_sub(cost));
    }

    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_device_dereset_bcpu(oal_pcie_res *pst_pci_res)
{
    int32_t ret;
    pci_addr_map addr_map;
    void *pst_glb_ctrl = NULL; /* 0x50000000 */

    ret = oal_pcie_inbound_ca_to_va(pst_pci_res, HI1103_PA_GLB_CTL_BASE_ADDR, &addr_map);
    if (oal_unlikely(ret != OAL_SUCC)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "can not found mem map for dev cpu address 0x%x\n",
                             HI1103_PA_GLB_CTL_BASE_ADDR);
        return -OAL_EFAIL;
    }

    pst_glb_ctrl = (void *)addr_map.va;

    oal_writel(0x1, pst_glb_ctrl + 0x94);

    oal_print_hi11xx_log(HI11XX_LOG_INFO, "bcpu dereset, reg=0x%x", oal_readl(pst_glb_ctrl + 0x94));

    /* bcpu mem解复位需要时间, 1103 32K计数, 230us */
    oal_msleep(1);

    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_device_get_scan_bmem_info(oal_pcie_res *pst_pci_res, uintptr_t (**pst_scan_base)[],
                                                      uint32_t *pst_scan_nums)
{
    *pst_scan_base = NULL;
    *pst_scan_nums = 0;
    if (get_hi110x_subchip_type() == BOARD_VERSION_HI1105) {
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "1105 %s bmem scan", hi110x_is_asic() ? "asic" : "fpga");
        if (hi110x_is_asic()) {
            *pst_scan_nums = sizeof(g_hi1105_asic_bmem_scan_array) /
                             (PCIE_MEM_MSG_SIZE * sizeof(g_hi1105_asic_bmem_scan_array[0][0]));
            *pst_scan_base = g_hi1105_asic_bmem_scan_array;
        } else {
            oal_print_hi11xx_log(HI11XX_LOG_INFO, "skip 1105 fpga bmem scan");
            return -OAL_ENOMEM;
        }
    } else {
        if (pst_pci_res->revision == PCIE_REVISION_4_70A) {
            *pst_scan_nums = sizeof(g_mpw2_bmem_scan_array) /
                             (PCIE_MEM_MSG_SIZE * sizeof(g_mpw2_bmem_scan_array[0][0]));
            *pst_scan_base = g_mpw2_bmem_scan_array;
        } else if (pst_pci_res->revision == PCIE_REVISION_5_00A) {
            *pst_scan_nums = sizeof(g_pilot_bmem_scan_array) /
                             (PCIE_MEM_MSG_SIZE * sizeof(g_pilot_bmem_scan_array[0][0]));
            *pst_scan_base = g_pilot_bmem_scan_array;
        } else {
            oal_print_hi11xx_log(HI11XX_LOG_ERR, "unkown pcie ip revision :0x%x", pst_pci_res->revision);
            return -OAL_ENODEV;
        }
    }

    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_device_scan_bmem(oal_pcie_res *pst_pci_res)
{
    uint32_t i;
    int32_t ret;
    unsigned long cpu_start, cpu_end, mem_len;
    uint32_t scan_nums;
    uintptr_t(*pst_scan_base)[PCIE_MEM_MSG_SIZE];

    declare_time_cost_stru(cost);

    oal_print_hi11xx_log(HI11XX_LOG_INFO, "start scan bmem..");

    ret = oal_pcie_device_get_scan_bmem_info(pst_pci_res, &pst_scan_base, &scan_nums);
    if (ret != OAL_SUCC) {
        /* bypass */
        return OAL_SUCC;
    }

    oal_get_time_cost_start(cost);

    ret = oal_pcie_device_dereset_bcpu(pst_pci_res);
    if (ret != OAL_SUCC) {
        return ret;
    }

    for (i = 0; i < scan_nums; i++) {
        oal_print_hi11xx_log(HI11XX_LOG_DBG, "i:%u, %pK", i, pst_scan_base + i);
        cpu_start = pst_scan_base[i][0];
        cpu_end = pst_scan_base[i][1];
        if (cpu_end <= cpu_start) {
            oal_print_hi11xx_log(HI11XX_LOG_ERR, "invalid mem region, start=0x%lx, end=0x%lx, i:%d",
                                 cpu_start, cpu_end, i);
            return -OAL_EFAUL;
        }

        mem_len = cpu_end - cpu_start + 1;

        ret = oal_pcie_device_mem_check(pst_pci_res, cpu_start, mem_len);
        if (ret) {
            return ret;
        }
    }

    if (g_ft_pcie_write_address_bypass) {
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "bmem write_address_bypass");
    } else {
        /* 连续写，连续读，Soc要求写入当前地址的值 */
        for (i = 0; i < scan_nums; i++) {
            cpu_start = pst_scan_base[i][0];
            cpu_end = pst_scan_base[i][1];

            mem_len = cpu_end - cpu_start + 1;

            ret = oal_pcie_device_mem_write_address_onetime(pst_pci_res, cpu_start, mem_len);
            if (ret) {
                return ret;
            }
        }

        for (i = 0; i < scan_nums; i++) {
            cpu_start = pst_scan_base[i][0];
            cpu_end = pst_scan_base[i][1];

            mem_len = cpu_end - cpu_start + 1;

            ret = oal_pcie_device_mem_read_address_onetime(pst_pci_res, cpu_start, mem_len);
            if (ret) {
                return ret;
            }
        }
    }

    oal_get_time_cost_end(cost);
    oal_calc_time_cost_sub(cost);
    oal_print_hi11xx_log(HI11XX_LOG_INFO, "device bmem done cost %llu us, regions :%u",
                         time_cost_var_sub(cost), scan_nums);

    return OAL_SUCC;
}

/* scan the mem */
int32_t oal_pcie_device_mem_scanall(oal_pcie_res *pst_pci_res)
{
    int32_t ret;

    ret = oal_pcie_device_scan_reg(pst_pci_res);
    if (ret) {
        return ret;
    }

    ret = oal_pcie_device_scan_wmem(pst_pci_res);
    if (ret) {
        return ret;
    }

    ret = oal_pcie_device_scan_bmem(pst_pci_res);
    if (ret) {
        return ret;
    }

    oal_print_hi11xx_log(HI11XX_LOG_INFO, "device_mem_scanall pass");
    return OAL_SUCC;
}

int32_t oal_pcie_copy_from_device_by_dword(oal_pcie_res *pst_pci_res,
                                           void *ddr_address,
                                           unsigned long start,
                                           uint32_t data_size)
{
    uint32_t i;
    int32_t ret;
    uint32_t value;
    unsigned long length;
    pci_addr_map addr_map;

    length = (unsigned long)data_size;

    if (oal_unlikely(((uintptr_t)ddr_address & 0x3) || (data_size & 0x3))) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "ddr address %lu, length 0x%lx is invalid",
                             (uintptr_t)ddr_address, length);
        return -OAL_EINVAL;
    }

    ret = oal_pcie_inbound_ca_to_va(pst_pci_res, start + length - 1, &addr_map);
    if (ret) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "cpu address start:0x%lx + length:0x%lx -1 invalid", start, length);
        return ret;
    }

    ret = oal_pcie_inbound_ca_to_va(pst_pci_res, start, &addr_map);
    if (ret) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "cpu address start:0x%lx", start);
        return ret;
    }

    for (i = 0; i < (uint32_t)length; i += sizeof(uint32_t)) { /* 每次偏移4字节 */
        value = oal_readl((void *)(addr_map.va + i));
        oal_writel(value, ddr_address + i);
    }

    return (int32_t)data_size;
}

unsigned long oal_pcie_get_deivce_dtcm_cpuaddr(oal_pcie_res *pst_pci_res)
{
    if (pst_pci_res->revision == PCIE_REVISION_4_70A) {
        return 0x20000000;
    } else if (pst_pci_res->revision == PCIE_REVISION_5_00A) {
        return 0x20018000;
    } else {
        return 0xffffffff;
    }
}

int32_t oal_pcie_copy_to_device_by_dword(oal_pcie_res *pst_pci_res,
                                         void *ddr_address,
                                         unsigned long start,
                                         uint32_t data_size)
{
    uint32_t i;
    int32_t ret;
    uint32_t value;
    unsigned long length;
    pci_addr_map addr_map;

    length = (unsigned long)data_size;

    if (oal_unlikely(((uintptr_t)ddr_address & 0x3) || (data_size & 0x3))) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "ddr address %lu, length 0x%lx is invalid",
                             (uintptr_t)ddr_address, length);
        return -OAL_EINVAL;
    }

    ret = oal_pcie_inbound_ca_to_va(pst_pci_res, start + length - 1, &addr_map);
    if (ret) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "cpu address start:0x%lx + length:0x%lx  invalid", start, length);
        return ret;
    }

    ret = oal_pcie_inbound_ca_to_va(pst_pci_res, start, &addr_map);
    if (ret) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "cpu address start:0x%lx", start);
        return ret;
    }

    for (i = 0; i < (uint32_t)length; i += sizeof(uint32_t)) { /* 每次偏移4字节 */
        value = oal_readl(ddr_address + i);
        oal_writel(value, (void *)(addr_map.va + i));
    }

    return (int32_t)data_size;
}

/* 时钟分频要在低功耗关闭下配置 */
int32_t oal_pcie_device_auxclk_init(oal_pcie_res *pst_pci_res)
{
    int32_t ret;
    uint32_t value;
    pci_addr_map st_map;

    value = oal_readl(pst_pci_res->pst_pci_dbi_base + PCIE_AUX_CLK_FREQ_OFF);
    /* aux_clk 1M, synophys set */
    value &= (~((1 << 10) - 1));
    value |= 0x1;
    oal_writel(value, pst_pci_res->pst_pci_dbi_base + PCIE_AUX_CLK_FREQ_OFF);

    /* tcxo 38.4M 39分频 = 0.98M 接近1M */
    ret = oal_pcie_inbound_ca_to_va(pst_pci_res, (0x50000000 + 0x2c), &st_map);
    if (ret != OAL_SUCC) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "wcpu address  convert failed, ret=%d", ret);
        return ret;
    }

    value = oal_readl((void *)st_map.va);
    value &= (~(((1 << 6) - 1) << 8));
    value |= (0x27 << 8);
    oal_writel(value, (void *)st_map.va);

    oal_print_hi11xx_log(HI11XX_LOG_INFO, "clk_freq reg:0x%x, freq_div reg:0x%x",
                         oal_readl(pst_pci_res->pst_pci_dbi_base + PCIE_AUX_CLK_FREQ_OFF),
                         oal_readl((void *)st_map.va));
    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_device_shared_addr_res_map(oal_pcie_res *pst_pci_res,
                                                       pcie_share_mem_stru *pst_share_mem)
{
    int32_t ret;
    int32_t i;

    pci_addr_map st_map;

    for (i = 0; i < PCIE_SHARED_ADDR_BUTT; i++) {
        if (pst_share_mem->device_addr[i] == 0) {
            pci_print_log(PCI_LOG_DBG, "type:%d , device addr is zero", i);
            continue;
        }

        ret = oal_pcie_inbound_ca_to_va(pst_pci_res, pst_share_mem->device_addr[i], &st_map);
        if (ret != OAL_SUCC) {
            pci_print_log(PCI_LOG_ERR, "convert device addr type:%d, addr:0x%x failed, ret=%d",
                          i, pst_share_mem->device_addr[i], ret);
            return -OAL_ENOMEM;
        }

        memcpy_s(&pst_pci_res->st_device_shared_addr_map[i], sizeof(pci_addr_map), &st_map, sizeof(pci_addr_map));
    }

    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_device_shared_addr_res_unmap(oal_pcie_res *pst_pci_res)
{
    memset_s(&pst_pci_res->st_device_shared_addr_map, OAL_SIZEOF(pst_pci_res->st_device_shared_addr_map),
             0, OAL_SIZEOF(pst_pci_res->st_device_shared_addr_map));
    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_comm_ringbuf_res_unmap(oal_pcie_res *pst_pci_res)
{
    int32_t i;
    for (i = 0; i < PCIE_COMM_RINGBUF_BUTT; i++) {
        memset_s(&pst_pci_res->st_ringbuf_res.comm_rb_res[i].ctrl_daddr,
                 OAL_SIZEOF(pst_pci_res->st_ringbuf_res.comm_rb_res[i].ctrl_daddr),
                 0, OAL_SIZEOF(pst_pci_res->st_ringbuf_res.comm_rb_res[i].ctrl_daddr));
        memset_s(&pst_pci_res->st_ringbuf_res.comm_rb_res[i].data_daddr,
                 OAL_SIZEOF(pst_pci_res->st_ringbuf_res.comm_rb_res[i].data_daddr),
                 0, OAL_SIZEOF(pst_pci_res->st_ringbuf_res.comm_rb_res[i].data_daddr));
    }

    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_comm_ringbuf_res_map(oal_pcie_res *pst_pci_res)
{
    int32_t i;
    int32_t ret;
    pci_addr_map st_map; /* DEVICE CPU地址 */

    for (i = 0; i < PCIE_COMM_RINGBUF_BUTT; i++) {
        if (pst_pci_res->st_ringbuf.st_ringbuf[i].base_addr == 0) {
            /* ringbuf invalid */
            continue;
        }

        /* get ringbuf base_addr */
        ret = oal_pcie_inbound_ca_to_va(pst_pci_res, pst_pci_res->st_ringbuf.st_ringbuf[i].base_addr, &st_map);
        if (ret != OAL_SUCC) {
            pci_print_log(PCI_LOG_ERR, "invalid comm ringbuf base address 0x%llx, rb id:%d map failed, ret=%d\n",
                          pst_pci_res->st_ringbuf.st_ringbuf[i].base_addr, i, ret);
            return -OAL_ENOMEM;
        }

        pci_print_log(PCI_LOG_DBG, "comm ringbuf %d base address is 0x%llx",
                      i, pst_pci_res->st_ringbuf.st_ringbuf[i].base_addr);

        /* comm ringbuf data 所在DMA地址 */
        memcpy_s((void *)&pst_pci_res->st_ringbuf_res.comm_rb_res[i].data_daddr,
                 sizeof(pci_addr_map), (void *)&st_map, sizeof(pci_addr_map));

        /* comm ringbuf ctrl address */
        pst_pci_res->st_ringbuf_res.comm_rb_res[i].ctrl_daddr.va = pst_pci_res->st_ringbuf_map.va +
                                                                   OAL_OFFSET_OF(pcie_ringbuf_res, st_ringbuf[i]);
        pst_pci_res->st_ringbuf_res.comm_rb_res[i].ctrl_daddr.pa = pst_pci_res->st_ringbuf_map.pa +
                                                                   OAL_OFFSET_OF(pcie_ringbuf_res, st_ringbuf[i]);
    }

    return OAL_SUCC;
}

void oal_pcie_ringbuf_res_unmap(oal_pcie_res *pst_pci_res)
{
    int32_t i;
    memset_s(&pst_pci_res->st_ringbuf_map, OAL_SIZEOF(pst_pci_res->st_ringbuf_map),
             0, OAL_SIZEOF(pst_pci_res->st_ringbuf_map));
    memset_s(&pst_pci_res->st_ringbuf, OAL_SIZEOF(pst_pci_res->st_ringbuf), 0, OAL_SIZEOF(pst_pci_res->st_ringbuf));
    memset_s(&pst_pci_res->st_rx_res.ringbuf_data_dma_addr, OAL_SIZEOF(pst_pci_res->st_rx_res.ringbuf_data_dma_addr),
             0, OAL_SIZEOF(pst_pci_res->st_rx_res.ringbuf_data_dma_addr));
    memset_s(&pst_pci_res->st_rx_res.ringbuf_ctrl_dma_addr, OAL_SIZEOF(pst_pci_res->st_rx_res.ringbuf_ctrl_dma_addr),
             0, OAL_SIZEOF(pst_pci_res->st_rx_res.ringbuf_ctrl_dma_addr));
    for (i = 0; i < PCIE_H2D_QTYPE_BUTT; i++) {
        memset_s(&pst_pci_res->st_tx_res[i].ringbuf_data_dma_addr,
                 OAL_SIZEOF(pst_pci_res->st_tx_res[i].ringbuf_data_dma_addr),
                 0, OAL_SIZEOF(pst_pci_res->st_tx_res[i].ringbuf_data_dma_addr));
        memset_s(&pst_pci_res->st_tx_res[i].ringbuf_ctrl_dma_addr,
                 OAL_SIZEOF(pst_pci_res->st_tx_res[i].ringbuf_ctrl_dma_addr),
                 0, OAL_SIZEOF(pst_pci_res->st_tx_res[i].ringbuf_ctrl_dma_addr));
    }

    memset_s(&pst_pci_res->st_message_res.d2h_res.ringbuf_ctrl_dma_addr,
             OAL_SIZEOF(pst_pci_res->st_message_res.d2h_res.ringbuf_ctrl_dma_addr),
             0, OAL_SIZEOF(pst_pci_res->st_message_res.d2h_res.ringbuf_ctrl_dma_addr));
    memset_s(&pst_pci_res->st_message_res.d2h_res.ringbuf_data_dma_addr,
             OAL_SIZEOF(pst_pci_res->st_message_res.d2h_res.ringbuf_data_dma_addr),
             0, OAL_SIZEOF(pst_pci_res->st_message_res.d2h_res.ringbuf_data_dma_addr));

    memset_s(&pst_pci_res->st_message_res.h2d_res.ringbuf_ctrl_dma_addr,
             OAL_SIZEOF(pst_pci_res->st_message_res.h2d_res.ringbuf_ctrl_dma_addr),
             0, OAL_SIZEOF(pst_pci_res->st_message_res.h2d_res.ringbuf_ctrl_dma_addr));
    memset_s(&pst_pci_res->st_message_res.h2d_res.ringbuf_data_dma_addr,
             OAL_SIZEOF(pst_pci_res->st_message_res.h2d_res.ringbuf_data_dma_addr),
             0, OAL_SIZEOF(pst_pci_res->st_message_res.h2d_res.ringbuf_data_dma_addr));

    memset_s(&pst_pci_res->st_device_stat, OAL_SIZEOF(pst_pci_res->st_device_stat),
             0, OAL_SIZEOF(pst_pci_res->st_device_stat));

    oal_pcie_device_shared_addr_res_unmap(pst_pci_res);

    oal_pcie_comm_ringbuf_res_unmap(pst_pci_res);
}

int32_t oal_pcie_ringbuf_h2d_refresh(oal_pcie_res *pst_pci_res)
{
    int32_t ret;
    int32_t i;
    pcie_share_mem_stru st_share_mem;
    pci_addr_map st_map; /* DEVICE CPU地址 */

    oal_pcie_io_trans((uintptr_t)&st_share_mem, pst_pci_res->dev_share_mem.va, sizeof(pcie_share_mem_stru));

    ret = oal_pcie_inbound_ca_to_va(pst_pci_res, st_share_mem.ringbuf_res_paddr, &st_map);
    if (ret != OAL_SUCC) {
        pci_print_log(PCI_LOG_ERR, "invalid ringbuf device address 0x%x, map failed\n",
                      st_share_mem.ringbuf_res_paddr);
        oal_print_hex_dump((uint8_t *)&st_share_mem, OAL_SIZEOF(st_share_mem),
                           HEX_DUMP_GROUP_SIZE, "st_share_mem: ");
        return -OAL_ENOMEM;
    }

    /* h->h */
    memcpy_s(&pst_pci_res->st_ringbuf_map, sizeof(pci_addr_map), &st_map, sizeof(pci_addr_map));

    /* device的ringbuf管理结构同步到Host */
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    oal_pci_cache_inv(pst_pci_dev, (void *)pst_pci_res->st_ringbuf_map.pa, sizeof(pst_pci_res->st_ringbuf));
#endif

    /* 这里重新刷新h2d ringbuf 指针 */
    oal_pcie_io_trans((uintptr_t)&pst_pci_res->st_ringbuf, (uintptr_t)pst_pci_res->st_ringbuf_map.va,
                      sizeof(pst_pci_res->st_ringbuf));

    /* 初始化RX BUFF */
    for (i = 0; i < PCIE_H2D_QTYPE_BUTT; i++) {
        unsigned long offset;
        ret = oal_pcie_inbound_ca_to_va(pst_pci_res, pst_pci_res->st_ringbuf.st_h2d_buf[i].base_addr, &st_map);
        if (ret != OAL_SUCC) {
            pci_print_log(PCI_LOG_ERR, "invalid d2h ringbuf[%d] base address 0x%llx, map failed, ret=%d\n",
                          i, pst_pci_res->st_ringbuf.st_h2d_buf[i].base_addr, ret);
            return -OAL_ENOMEM;
        }
        memcpy_s(&pst_pci_res->st_tx_res[i].ringbuf_data_dma_addr,
                 sizeof(pci_addr_map), &st_map, sizeof(pci_addr_map));
        offset = (uintptr_t)&pst_pci_res->st_ringbuf.st_h2d_buf[i] - (uintptr_t)&pst_pci_res->st_ringbuf;
        pst_pci_res->st_tx_res[i].ringbuf_ctrl_dma_addr.pa = pst_pci_res->st_ringbuf_map.pa + offset;
        pst_pci_res->st_tx_res[i].ringbuf_ctrl_dma_addr.va = pst_pci_res->st_ringbuf_map.va + offset;
    }

    return OAL_SUCC;
}

/* 初始化Host ringbuf 和 Device ringbuf 的映射 */
int32_t oal_pcie_ringbuf_res_map(oal_pcie_res *pst_pci_res)
{
    int32_t ret;
    int32_t i;
    uint8_t reg = 0;
    oal_pci_dev_stru *pst_pci_dev;
    pci_addr_map st_map; /* DEVICE CPU地址 */
    pcie_share_mem_stru st_share_mem;

    pst_pci_dev = pcie_res_to_dev(pst_pci_res);

#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    oal_pci_cache_inv(pst_pci_dev, (void *)pst_pci_res->dev_share_mem.pa, sizeof(pcie_share_mem_stru));
#endif

    oal_pci_read_config_byte(pst_pci_dev, PCI_CACHE_LINE_SIZE, &reg);
    pci_print_log(PCI_LOG_INFO, "L1_CACHE_BYTES: %d\n", reg);

    pci_print_log(PCI_LOG_INFO, "pst_pci_res->dev_share_mem.va:%lx", pst_pci_res->dev_share_mem.va);

    oal_pcie_io_trans((uintptr_t)&st_share_mem, pst_pci_res->dev_share_mem.va, sizeof(pcie_share_mem_stru));
    oal_print_hex_dump((uint8_t *)pst_pci_res->dev_share_mem.va, sizeof(pcie_share_mem_stru),
                       HEX_DUMP_GROUP_SIZE, "st_share_mem: ");
    pci_print_log(PCI_LOG_INFO, "st_share_mem.ringbuf_res_paddr :0x%x\n", st_share_mem.ringbuf_res_paddr);

    ret = oal_pcie_inbound_ca_to_va(pst_pci_res, st_share_mem.ringbuf_res_paddr, &st_map);
    if (ret != OAL_SUCC) {
        pci_print_log(PCI_LOG_ERR, "invalid ringbuf device address 0x%x, map failed\n",
                      st_share_mem.ringbuf_res_paddr);
        oal_print_hex_dump((uint8_t *)&st_share_mem, OAL_SIZEOF(st_share_mem),
                           HEX_DUMP_GROUP_SIZE, "st_share_mem: ");
        return -OAL_ENOMEM;
    }

    /* h->h */
    memcpy_s(&pst_pci_res->st_ringbuf_map, sizeof(pci_addr_map), &st_map, sizeof(pci_addr_map));

    /* device的ringbuf管理结构同步到Host */
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    oal_pci_cache_inv(pst_pci_dev, (void *)pst_pci_res->st_ringbuf_map.pa, sizeof(pst_pci_res->st_ringbuf));
#endif
    oal_pcie_io_trans((uintptr_t)&pst_pci_res->st_ringbuf, pst_pci_res->st_ringbuf_map.va,
                      sizeof(pst_pci_res->st_ringbuf));

    /* 初始化ringbuf 管理结构体的映射 */
    pst_pci_res->st_rx_res.ringbuf_ctrl_dma_addr.pa = pst_pci_res->st_ringbuf_map.pa +
                                                      OAL_OFFSET_OF(pcie_ringbuf_res, st_d2h_buf);
    pst_pci_res->st_rx_res.ringbuf_ctrl_dma_addr.va = pst_pci_res->st_ringbuf_map.va +
                                                      OAL_OFFSET_OF(pcie_ringbuf_res, st_d2h_buf);

    /* 初始化TX BUFF, 不考虑大小端，host/dev 都是小端，否者这里的base_addr需要转换 */
    ret = oal_pcie_inbound_ca_to_va(pst_pci_res, pst_pci_res->st_ringbuf.st_d2h_buf.base_addr, &st_map);
    if (ret != OAL_SUCC) {
        pci_print_log(PCI_LOG_ERR, "invalid h2d ringbuf base address 0x%llx, map failed\n",
                      pst_pci_res->st_ringbuf.st_d2h_buf.base_addr);
        return -OAL_ENOMEM;
    }

    memcpy_s((void *)&pst_pci_res->st_rx_res.ringbuf_data_dma_addr, sizeof(pci_addr_map),
             (void *)&st_map, sizeof(pci_addr_map));

    /* 初始化RX BUFF */
    for (i = 0; i < PCIE_H2D_QTYPE_BUTT; i++) {
        uintptr_t offset;
        ret = oal_pcie_inbound_ca_to_va(pst_pci_res, pst_pci_res->st_ringbuf.st_h2d_buf[i].base_addr, &st_map);
        if (ret != OAL_SUCC) {
            pci_print_log(PCI_LOG_ERR, "invalid d2h ringbuf[%d] base address 0x%llx, map failed, ret=%d\n",
                          i, pst_pci_res->st_ringbuf.st_h2d_buf[i].base_addr, ret);
            return -OAL_ENOMEM;
        }
        memcpy_s(&pst_pci_res->st_tx_res[i].ringbuf_data_dma_addr,
                 sizeof(pci_addr_map), &st_map, sizeof(pci_addr_map));
        offset = (uintptr_t)&pst_pci_res->st_ringbuf.st_h2d_buf[i] - (uintptr_t)&pst_pci_res->st_ringbuf;
        pst_pci_res->st_tx_res[i].ringbuf_ctrl_dma_addr.pa = pst_pci_res->st_ringbuf_map.pa + offset;
        pst_pci_res->st_tx_res[i].ringbuf_ctrl_dma_addr.va = pst_pci_res->st_ringbuf_map.va + offset;
    }

    /* 初始化消息TX RINGBUFF */
    ret = oal_pcie_inbound_ca_to_va(pst_pci_res, pst_pci_res->st_ringbuf.st_h2d_msg.base_addr, &st_map);
    if (ret != OAL_SUCC) {
        pci_print_log(PCI_LOG_ERR, "invalid h2d message ringbuf base address 0x%llx, map failed, ret=%d\n",
                      pst_pci_res->st_ringbuf.st_h2d_msg.base_addr, ret);
        return -OAL_ENOMEM;
    }

    /* h2d message data 所在DMA地址 */
    memcpy_s((void *)&pst_pci_res->st_message_res.h2d_res.ringbuf_data_dma_addr,
             sizeof(pci_addr_map), (void *)&st_map, sizeof(pci_addr_map));

    /* h2d message ctrl 结构体 所在DMA地址 */
    pst_pci_res->st_message_res.h2d_res.ringbuf_ctrl_dma_addr.va = pst_pci_res->st_ringbuf_map.va +
                                                                   OAL_OFFSET_OF(pcie_ringbuf_res, st_h2d_msg);
    pst_pci_res->st_message_res.h2d_res.ringbuf_ctrl_dma_addr.pa = pst_pci_res->st_ringbuf_map.pa +
                                                                   OAL_OFFSET_OF(pcie_ringbuf_res, st_h2d_msg);

    /* 初始化消息RX RINGBUFF */
    ret = oal_pcie_inbound_ca_to_va(pst_pci_res, pst_pci_res->st_ringbuf.st_d2h_msg.base_addr, &st_map);
    if (ret != OAL_SUCC) {
        pci_print_log(PCI_LOG_ERR, "invalid d2h message ringbuf base address 0x%llx, map failed, ret=%d\n",
                      pst_pci_res->st_ringbuf.st_d2h_msg.base_addr, ret);
        return -OAL_ENOMEM;
    }

    /* d2h message data 所在DMA地址 */
    memcpy_s((void *)&pst_pci_res->st_message_res.d2h_res.ringbuf_data_dma_addr,
             sizeof(pci_addr_map), (void *)&st_map, sizeof(pci_addr_map));

    /* d2h message ctrl 结构体 所在DMA地址 */
    pst_pci_res->st_message_res.d2h_res.ringbuf_ctrl_dma_addr.va = pst_pci_res->st_ringbuf_map.va +
                                                                   OAL_OFFSET_OF(pcie_ringbuf_res, st_d2h_msg);
    pst_pci_res->st_message_res.d2h_res.ringbuf_ctrl_dma_addr.pa = pst_pci_res->st_ringbuf_map.pa +
                                                                   OAL_OFFSET_OF(pcie_ringbuf_res, st_d2h_msg);

#ifdef _PRE_PLAT_FEATURE_PCIE_DEVICE_STAT
    ret = oal_pcie_inbound_ca_to_va(pst_pci_res, st_share_mem.device_stat_paddr, &st_map);
    if (ret != OAL_SUCC) {
        pci_print_log(PCI_LOG_ERR, "invalid device_stat_paddr  0x%x, map failed\n", st_share_mem.device_stat_paddr);
        return -OAL_ENOMEM;
    }

    /* h->h */
    memcpy_s(&pst_pci_res->st_device_stat_map, sizeof(pci_addr_map), &st_map, sizeof(pci_addr_map));
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    oal_pci_cache_inv(pst_pci_dev, (void *)pst_pci_res->st_device_stat_map.pa, sizeof(pst_pci_res->st_device_stat));
#endif

    oal_pcie_io_trans((uintptr_t)&pst_pci_res->st_device_stat, pst_pci_res->st_device_stat_map.va,
                      sizeof(pst_pci_res->st_device_stat));
#endif

    ret = oal_pcie_comm_ringbuf_res_map(pst_pci_res);
    if (ret != OAL_SUCC) {
        return ret;
    }

    ret = oal_pcie_device_shared_addr_res_map(pst_pci_res, &st_share_mem);
    if (ret != OAL_SUCC) {
        return ret;
    }

    pci_print_log(PCI_LOG_INFO, "oal_pcie_ringbuf_res_map succ");
    return OAL_SUCC;
}

/* edma read 对应device->host, ringbuf_write 指更新写指针 */
OAL_STATIC int32_t oal_pcie_d2h_ringbuf_write(oal_pcie_res *pst_pci_res,
                                              pci_addr_map *pst_map,
                                              pcie_write_ringbuf_item *pst_item)
{
    /* 不判断写指针，此函数只执行写操作 */
    oal_pci_dev_stru *pst_pci_dev;
    uint32_t real_wr;

    pcie_ringbuf *pst_ringbuf = &pst_pci_res->st_ringbuf.st_d2h_buf;
    pst_pci_dev = pcie_res_to_dev(pst_pci_res);

    /* Debug */
    if (oal_unlikely(pst_ringbuf->item_len != sizeof(pcie_write_ringbuf_item))) {
        pci_print_log(PCI_LOG_ERR, "[%s]invalid item_len [%u!=%lu]\n",
                      __FUNCTION__, pst_ringbuf->item_len, (unsigned long)sizeof(pcie_write_ringbuf_item));
        return 0;
    }

    if (oal_warn_on(pst_ringbuf->wr - pst_ringbuf->rd >= pst_ringbuf->size)) {
        /* never touch here */
        pci_print_log(PCI_LOG_ERR, "ringbuf full [wr:%u] [rd:%u] [size:%u]\n",
                      pst_ringbuf->wr, pst_ringbuf->rd, pst_ringbuf->size);
        return 0;
    }

    real_wr = pst_ringbuf->wr & (pst_ringbuf->size - 1);
    oal_pcie_io_trans(pst_map->va + real_wr, (uintptr_t)pst_item, pst_ringbuf->item_len);
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    oal_pci_cache_flush(pst_pci_dev, (void *)pst_map->pa, pst_ringbuf->item_len);
#endif
    pst_ringbuf->wr += pst_ringbuf->item_len;

    return 1;
}

OAL_STATIC uint32_t oal_pcie_d2h_ringbuf_freecount(oal_pcie_res *pst_pci_res, int32_t is_sync)
{
    pcie_ringbuf *pst_ringbuf = &pst_pci_res->st_ringbuf.st_d2h_buf;

    if (is_sync == OAL_TRUE) {
        /* 同步Dev2Host的读指针 */
        oal_pcie_d2h_ringbuf_rd_update(pst_pci_res);
    }

    return oal_pcie_ringbuf_freecount(pst_ringbuf);
}

OAL_STATIC int32_t oal_pcie_d2h_ringbuf_wr_update(oal_pcie_res *pst_pci_res)
{
    /* d2h方向，同步host的ringbuf管理结构体的写指针到DEVICE侧, 需要刷cache */
    pci_addr_map st_map;

#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    oal_pci_dev_stru *pst_pci_dev = pcie_res_to_dev(pst_pci_res);
#endif

    oal_pcie_mips_start(PCIE_MIPS_RX_RINGBUF_WR_UPDATE);
    st_map.va = pst_pci_res->st_rx_res.ringbuf_ctrl_dma_addr.va + OAL_OFFSET_OF(pcie_ringbuf, wr);
    st_map.pa = pst_pci_res->st_rx_res.ringbuf_ctrl_dma_addr.pa + OAL_OFFSET_OF(pcie_ringbuf, wr);
    pci_print_log(PCI_LOG_DBG, "d2h ringbuf wr upate wr:%u", pst_pci_res->st_ringbuf.st_d2h_buf.wr);
    oal_pcie_write_mem32(st_map.va, pst_pci_res->st_ringbuf.st_d2h_buf.wr);
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    oal_pci_cache_flush(pst_pci_dev, (void *)st_map.pa, sizeof(pst_pci_res->st_ringbuf.st_d2h_buf.wr));
#endif
    oal_pcie_mips_end(PCIE_MIPS_RX_RINGBUF_WR_UPDATE);

    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_d2h_ringbuf_rd_update(oal_pcie_res *pst_pci_res)
{
    /* d2h方向，同步device的读指针到HOST ringbuf管理结构体 */
    /* 需要刷cache */
    uint32_t rd;
    pci_addr_map st_map;
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    oal_pci_dev_stru *pst_pci_dev = pcie_res_to_dev(pst_pci_res);
#endif

    oal_pcie_mips_start(PCIE_MIPS_RX_RINGBUF_RD_UPDATE);
    st_map.va = pst_pci_res->st_rx_res.ringbuf_ctrl_dma_addr.va + OAL_OFFSET_OF(pcie_ringbuf, rd);
    st_map.pa = pst_pci_res->st_rx_res.ringbuf_ctrl_dma_addr.pa + OAL_OFFSET_OF(pcie_ringbuf, rd);

#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    /* 无效化cache */
    oal_pci_cache_inv(pst_pci_dev, (void *)st_map.pa, sizeof(pst_pci_res->st_ringbuf.st_d2h_buf.rd));
#endif

    rd = oal_pcie_read_mem32(st_map.va);
    if (oal_unlikely(rd == 0xFFFFFFFF)) {
        if (oal_pcie_check_link_state(pst_pci_res) == OAL_FALSE) {
            pci_print_log(PCI_LOG_ERR, "d2h ringbuf rd update: link down[va:0x%lx, pa:0x%lx]", st_map.va, st_map.pa);
            return -OAL_ENODEV;
        }
    }
    pci_print_log(PCI_LOG_DBG, "d2h ringbuf rd update:[0x%lx:rd:%u]", st_map.va, rd);
    if (oal_unlikely(rd < pst_pci_res->st_ringbuf.st_d2h_buf.rd)) {
        /* 判断rd 翻转 */
        pci_print_log(PCI_LOG_INFO, "d2h new rd %u over old rd %u, wr:%u",
                      rd, pst_pci_res->st_ringbuf.st_d2h_buf.rd,
                      pst_pci_res->st_ringbuf.st_d2h_buf.wr);
        declare_dft_trace_key_info("d2h_ringbuf_overrun", OAL_DFT_TRACE_SUCC);
    }
    pst_pci_res->st_ringbuf.st_d2h_buf.rd = rd;
    oal_pcie_mips_end(PCIE_MIPS_RX_RINGBUF_RD_UPDATE);

    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_h2d_ringbuf_write(oal_pcie_res *pst_pci_res,
                                              pci_addr_map *pst_map,
                                              PCIE_H2D_RINGBUF_QTYPE qtype,
                                              pcie_read_ringbuf_item *pst_item)
{
    /* 不判断写指针，此函数只执行写操作 */
    oal_pci_dev_stru *pst_pci_dev;
    uint32_t real_wr;

    pcie_ringbuf *pst_ringbuf = &pst_pci_res->st_ringbuf.st_h2d_buf[qtype];

    pst_pci_dev = pcie_res_to_dev(pst_pci_res);

    /* Debug */
    if (oal_unlikely(pst_pci_res->link_state < PCI_WLAN_LINK_WORK_UP)) {
        pci_print_log(PCI_LOG_WARN, "oal_pcie_h2d_ringbuf_write invaild, link_state:%s",
                      oal_pcie_get_link_state_str(pst_pci_res->link_state));
        return -OAL_ENODEV;
    }

    /* Debug */
    if (oal_unlikely(pst_ringbuf->item_len != sizeof(pcie_read_ringbuf_item))) {
        pci_print_log(PCI_LOG_ERR, "[%s]invalid item_len [%u!=%lu]\n",
                      __FUNCTION__, pst_ringbuf->item_len, (unsigned long)sizeof(pcie_read_ringbuf_item));
        return 0;
    }

    if (oal_warn_on(pst_ringbuf->wr - pst_ringbuf->rd >= pst_ringbuf->size)) {
        /* never touch here */
        pci_print_log(PCI_LOG_ERR, "ringbuf full [wr:%u] [rd:%u] [size:%u]\n",
                      pst_ringbuf->wr, pst_ringbuf->rd, pst_ringbuf->size);
        return 0;
    }

    real_wr = pst_ringbuf->wr & (pst_ringbuf->size - 1);
    oal_pcie_io_trans(pst_map->va + real_wr, (uintptr_t)pst_item, pst_ringbuf->item_len);
    if (pci_dbg_condtion()) {
        int32_t ret;
        uint64_t cpuaddr;
        ret = oal_pcie_get_ca_by_pa(pst_pci_res, pst_map->pa, &cpuaddr);
        if (ret == OAL_SUCC) {
            pci_print_log(PCI_LOG_DBG, "h2d ringbuf write ringbuf data cpu address:0x%llx", cpuaddr);
        } else {
            pci_print_log(PCI_LOG_DBG, "h2d ringbuf rd pa:0x%lx invaild", pst_map->pa);
        }
        oal_print_hex_dump((uint8_t *)pst_item, pst_ringbuf->item_len, pst_ringbuf->item_len, "ringbuf write: ");
    }
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    oal_pci_cache_flush(pst_pci_dev, (void *)pst_map->pa, pst_ringbuf->item_len);
#endif
    pst_ringbuf->wr += pst_ringbuf->item_len;

    return 1;
}

OAL_STATIC int32_t oal_pcie_h2d_ringbuf_wr_update(oal_pcie_res *pst_pci_res, PCIE_H2D_RINGBUF_QTYPE qtype)
{
    /* h2d方向，同步host的ringbuf管理结构体的写指针到DEVICE侧, 需要刷cache */
    pci_addr_map st_map;
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    oal_pci_dev_stru *pst_pci_dev = pcie_res_to_dev(pst_pci_res);
#endif

    st_map.va = pst_pci_res->st_tx_res[qtype].ringbuf_ctrl_dma_addr.va + OAL_OFFSET_OF(pcie_ringbuf, wr);
    st_map.pa = pst_pci_res->st_tx_res[qtype].ringbuf_ctrl_dma_addr.pa + OAL_OFFSET_OF(pcie_ringbuf, wr);
    pci_print_log(PCI_LOG_DBG, "h2d ringbuf wr upate wr:%u", pst_pci_res->st_ringbuf.st_h2d_buf[qtype].wr);

    if (oal_unlikely(pst_pci_res->link_state <= PCI_WLAN_LINK_DOWN)) {
        pci_print_log(PCI_LOG_WARN, "pcie is linkdown");
        return -OAL_ENODEV;
    }

    oal_pcie_write_mem32(st_map.va, pst_pci_res->st_ringbuf.st_h2d_buf[qtype].wr);
    if (pci_dbg_condtion()) {
        int32_t ret;
        uint64_t cpuaddr;
        ret = oal_pcie_get_ca_by_pa(pst_pci_res, st_map.pa, &cpuaddr);
        if (ret == OAL_SUCC) {
            pci_print_log(PCI_LOG_DBG, "h2d ringbuf wr cpu address:0x%llx", cpuaddr);
        } else {
            pci_print_log(PCI_LOG_DBG, "h2d ringbuf wr pa:0x%lx invaild", st_map.pa);
        }
    }

#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    oal_pci_cache_flush(pst_pci_dev, (void *)st_map.pa, sizeof(pst_pci_res->st_ringbuf.st_h2d_buf[qtype].wr));
#endif

    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_h2d_ringbuf_rd_update(oal_pcie_res *pst_pci_res, PCIE_H2D_RINGBUF_QTYPE qtype)
{
    /* h2d方向，同步device的读指针到HOST ringbuf管理结构体 */
    /* 需要刷cache */
    uint32_t value;
    pci_addr_map st_map;
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    oal_pci_dev_stru *pst_pci_dev = pcie_res_to_dev(pst_pci_res);
#endif

    st_map.va = pst_pci_res->st_tx_res[qtype].ringbuf_ctrl_dma_addr.va + OAL_OFFSET_OF(pcie_ringbuf, rd);
    st_map.pa = pst_pci_res->st_tx_res[qtype].ringbuf_ctrl_dma_addr.pa + OAL_OFFSET_OF(pcie_ringbuf, rd);

    if (pci_dbg_condtion()) {
        int32_t ret;
        uint64_t cpuaddr;
        ret = oal_pcie_get_ca_by_pa(pst_pci_res, st_map.pa, &cpuaddr);
        if (ret == OAL_SUCC) {
            pci_print_log(PCI_LOG_DBG, "h2d ringbuf rd cpu address:0x%llx", cpuaddr);
        } else {
            pci_print_log(PCI_LOG_DBG, "h2d ringbuf rd pa:0x%lx invaild", st_map.pa);
        }
    }

    if (oal_unlikely(pst_pci_res->link_state <= PCI_WLAN_LINK_DOWN)) {
        pci_print_log(PCI_LOG_WARN, "pcie is linkdown");
        return -OAL_ENODEV;
    }

#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    oal_pci_cache_inv(pst_pci_dev, (void *)st_map.pa, sizeof(pst_pci_res->st_ringbuf.st_h2d_buf[qtype].rd));
#endif
    value = oal_pcie_read_mem32(st_map.va);
    if (oal_unlikely(value == 0xFFFFFFFF)) {
        if (oal_pcie_check_link_state(pst_pci_res) == OAL_FALSE) {
            pci_print_log(PCI_LOG_ERR, "h2d ringbuf rd update: link down[va:0x%lx, pa:0x%lx]", st_map.va, st_map.pa);
            return -OAL_ENODEV;
        }
    }
    pci_print_log(PCI_LOG_DBG, "h2d ringbuf rd upate rd:%u, curr wr:%u",
                  value, pst_pci_res->st_ringbuf.st_h2d_buf[qtype].wr);
    if (oal_unlikely(value < pst_pci_res->st_ringbuf.st_h2d_buf[qtype].rd)) {
        /* 判断rd 翻转 */
        pci_print_log(PCI_LOG_INFO, "h2d qtype %d new rd %u over old rd %u, wr:%u", qtype,
                      value, pst_pci_res->st_ringbuf.st_h2d_buf[qtype].rd,
                      pst_pci_res->st_ringbuf.st_h2d_buf[qtype].wr);
        declare_dft_trace_key_info("h2d_ringbuf_overrun", OAL_DFT_TRACE_SUCC);
    }
    pst_pci_res->st_ringbuf.st_h2d_buf[qtype].rd = value;

    return OAL_SUCC;
}

/* 获取ringbuf剩余空间大小，is_sync为TRUE时 先从DEVICE同步读指针再判断 */
OAL_STATIC uint32_t oal_pcie_h2d_ringbuf_freecount(oal_pcie_res *pst_pci_res, PCIE_H2D_RINGBUF_QTYPE qtype,
                                                   int32_t is_sync)
{
    pcie_ringbuf *pst_ringbuf = &pst_pci_res->st_ringbuf.st_h2d_buf[qtype];

    if (is_sync == OAL_TRUE) {
        /* 同步Host2Dev的读指针 */
        oal_pcie_h2d_ringbuf_rd_update(pst_pci_res, qtype);
    }

    oal_pcie_print_ringbuf_info(pst_ringbuf, PCI_LOG_DBG);
    return oal_pcie_ringbuf_freecount(pst_ringbuf);
}

uint32_t oal_pcie_h2d_ringbuf_is_empty(oal_pcie_res *pst_pci_res, PCIE_H2D_RINGBUF_QTYPE qtype, int32_t is_sync)
{
    pcie_ringbuf *pst_ringbuf = &pst_pci_res->st_ringbuf.st_h2d_buf[qtype];

    if (is_sync == OAL_TRUE) {
        /* 同步Host2Dev的读指针 */
        oal_pcie_h2d_ringbuf_rd_update(pst_pci_res, qtype);
    }

    oal_pcie_print_ringbuf_info(pst_ringbuf, PCI_LOG_DBG);
    return oal_pcie_ringbuf_is_empty(pst_ringbuf);
}

OAL_STATIC int32_t oal_pcie_h2d_doorbell(oal_pcie_res *pst_pci_res)
{
    /* 敲铃,host->device ringbuf 有数据更新,2个队列共享一个中断 */
    pst_pci_res->stat.h2d_doorbell_cnt++;
    pci_print_log(PCI_LOG_DBG, "oal_pcie_h2d_doorbell,cnt:%u", pst_pci_res->stat.h2d_doorbell_cnt);
    if (oal_unlikely(pst_pci_res->link_state <= PCI_WLAN_LINK_DOWN)) {
        pci_print_log(PCI_LOG_WARN, "pcie is linkdown");
        return -OAL_ENODEV;
    }
    oal_writel(PCIE_H2D_DOORBELL_TRIGGER_VALUE, pst_pci_res->pst_pci_ctrl_base + PCIE_H2D_DOORBELL_OFF);
    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_d2h_doorbell(oal_pcie_res *pst_pci_res)
{
    /* 敲铃,host->device ringbuf 有数据更新,2个队列共享一个中断 */
    pst_pci_res->stat.d2h_doorbell_cnt++;
    pci_print_log(PCI_LOG_DBG, "oal_pcie_d2h_doorbell,cnt:%u", pst_pci_res->stat.d2h_doorbell_cnt);
    if (oal_unlikely(pst_pci_res->link_state <= PCI_WLAN_LINK_DOWN)) {
        pci_print_log(PCI_LOG_WARN, "pcie is linkdown");
        return -OAL_ENODEV;
    }
    oal_writel(PCIE_D2H_DOORBELL_TRIGGER_VALUE, pst_pci_res->pst_pci_ctrl_base + PCIE_D2H_DOORBELL_OFF);
    return OAL_SUCC;
}

/* 队列从队头出队 */
#ifdef _PRE_PLAT_FEATURE_HI110X_PCIE_H2D_BYPASS
int32_t oal_pcie_send_netbuf(oal_pcie_res *pst_pci_res, oal_netbuf_stru *pst_netbuf, PCIE_H2D_RINGBUF_QTYPE qtype)
{
    unsigned long flags;
    oal_pci_dev_stru *pst_pci_dev = NULL;
    pcie_read_ringbuf_item st_item;
    int32_t send_cnt, queue_cnt, total_cnt;

    if (oal_unlikely(pst_pci_res == NULL)) {
        return 0;
    }

    queue_cnt = 1;
    send_cnt = oal_pcie_h2d_ringbuf_freecount(pst_pci_res, qtype, OAL_FALSE);

    if (queue_cnt > send_cnt) {
        /* ringbuf 空间不够, 刷新rd指针，重新判断 */
        send_cnt = oal_pcie_h2d_ringbuf_freecount(pst_pci_res, qtype, OAL_TRUE);
    }

    if (send_cnt == 0) {
        /* ringbuf 为满 */
        return 0;
    }

    pci_print_log(PCI_LOG_DBG, "h2d_ringbuf freecount:%u, qlen:%u", send_cnt, queue_cnt);

    total_cnt = 0;
    pst_pci_dev = pcie_res_to_dev(pst_pci_res);

    /* 64bits 传输, 不考虑大小端 */
    st_item.buff_paddr.addr = (uint64_t)g_h2d_pci_dma_addr;

    /* 这里的长度包含64B的头 */
    if (oal_likely(oal_netbuf_len(pst_netbuf) > HCC_HDR_TOTAL_LEN)) {
        /* tx ringbuf中的长度不包含头,就算包含也只是多传输一个头的长度 */
        /* 计算4字节对齐后的长度，默认进位 */
        st_item.buf_len = padding_m((oal_netbuf_len(pst_netbuf) - HCC_HDR_TOTAL_LEN), 4);
    } else {
        pci_print_log(PCI_LOG_WARN, "tx netbuf too short %u < %u\n", oal_netbuf_len(pst_netbuf), HCC_HDR_TOTAL_LEN);
        declare_dft_trace_key_info("tx netbuf too short", OAL_DFT_TRACE_FAIL);
    }

    st_item.reserved0 = 0x4321;

    pci_print_log(PCI_LOG_DBG, "h2d ringbuf write 0x%llx, len:%u", st_item.buff_paddr.addr, st_item.buf_len);

    /* 这里直接写，上面已经判断过ringbuf有空间 */
    total_cnt += oal_pcie_h2d_ringbuf_write(pst_pci_res, &pst_pci_res->st_tx_res[qtype].ringbuf_data_dma_addr,
                                            qtype, &st_item);
    return total_cnt;
}
#endif

int32_t oal_pcie_edma_tx_is_idle(oal_pcie_res *pst_pci_res, PCIE_H2D_RINGBUF_QTYPE qtype)
{
    if (oal_warn_on(pst_pci_res == NULL)) {
        pci_print_log(PCI_LOG_WARN, "pci res is null");
        return OAL_FALSE;
    }

    /* pcie is link */
    if (oal_unlikely(pst_pci_res->link_state <= PCI_WLAN_LINK_DOWN)) {
        return OAL_FALSE;
    }

    return (oal_pcie_h2d_ringbuf_freecount(pst_pci_res, qtype, OAL_FALSE) ? OAL_TRUE : OAL_FALSE);
}

/* write message to ringbuf */
/* Returns, bytes we wrote to ringbuf */
OAL_STATIC int32_t oal_pcie_h2d_message_buf_write(oal_pcie_res *pst_pci_res, pcie_ringbuf *pst_ringbuf,
                                                  pci_addr_map *pst_ringbuf_base, uint32_t message)
{
    uint32_t real_wr;
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    oal_pci_dev_stru *pst_pci_dev = NULL;
#endif

    if (oal_unlikely(pst_ringbuf->item_len != (uint16_t)OAL_SIZEOF(message))) {
        pci_print_log(PCI_LOG_ERR, "[%s]invalid item_len [%u!=%lu]\n",
                      __FUNCTION__, pst_ringbuf->item_len, (unsigned long)OAL_SIZEOF(pcie_read_ringbuf_item));
        return 0;
    }

    if (oal_warn_on(pst_ringbuf->wr - pst_ringbuf->rd >= pst_ringbuf->size)) {
        /* never touch here */
        pci_print_log(PCI_LOG_ERR, "message ringbuf full [wr:%u] [rd:%u] [size:%u]\n",
                      pst_ringbuf->wr, pst_ringbuf->rd, pst_ringbuf->size);
        return 0;
    }

    real_wr = pst_ringbuf->wr & (pst_ringbuf->size - 1);
    oal_pcie_io_trans(pst_ringbuf_base->va + real_wr, (uintptr_t)&message, OAL_SIZEOF(message));

#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    oal_pci_cache_flush(pst_pci_dev, (void *)pst_ringbuf_base->pa, OAL_SIZEOF(message));
#endif

    pst_ringbuf->wr += OAL_SIZEOF(message);

    return 1;
}

OAL_STATIC int32_t oal_pcie_h2d_message_buf_rd_update(oal_pcie_res *pst_pci_res)
{
    /* 需要刷cache */
    /* h2d方向，同步device的读指针到HOST message ringbuf管理结构体 */
    uint32_t rd;
    pci_addr_map st_map;

#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    oal_pci_dev_stru *pst_pci_dev = pcie_res_to_dev(pst_pci_res);
#endif

    st_map.va = pst_pci_res->st_message_res.h2d_res.ringbuf_ctrl_dma_addr.va + OAL_OFFSET_OF(pcie_ringbuf, rd);
    st_map.pa = pst_pci_res->st_message_res.h2d_res.ringbuf_ctrl_dma_addr.pa + OAL_OFFSET_OF(pcie_ringbuf, rd);

#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    /* 无效化cache */
    oal_pci_cache_inv(pst_pci_dev, (void *)st_map.pa, sizeof(pst_pci_res->st_ringbuf.st_h2d_msg.rd));
#endif

    rd = oal_pcie_read_mem32(st_map.va);
    if (oal_unlikely(rd == 0xFFFFFFFF)) {
        if (oal_pcie_check_link_state(pst_pci_res) == OAL_FALSE) {
            pci_print_log(PCI_LOG_ERR, "h2d message ringbuf rd update: link down[va:0x%lx, pa:0x%lx]",
                          st_map.va, st_map.pa);
            return -OAL_ENODEV;
        }
    }
    pci_print_log(PCI_LOG_DBG, "h2d message ringbuf rd update:[0x%lx:rd:0x%x]", st_map.va, rd);
    pst_pci_res->st_ringbuf.st_h2d_msg.rd = rd;

    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_h2d_message_buf_wr_update(oal_pcie_res *pst_pci_res)
{
    /* 需要刷cache */
    /* h2d方向，同步device的读指针到HOST message ringbuf管理结构体 */
    uint32_t wr_back;
    pci_addr_map st_map;

#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    oal_pci_dev_stru *pst_pci_dev = pcie_res_to_dev(pst_pci_res);
#endif

    st_map.va = pst_pci_res->st_message_res.h2d_res.ringbuf_ctrl_dma_addr.va + OAL_OFFSET_OF(pcie_ringbuf, wr);
    st_map.pa = pst_pci_res->st_message_res.h2d_res.ringbuf_ctrl_dma_addr.pa + OAL_OFFSET_OF(pcie_ringbuf, wr);

    oal_pcie_write_mem32(st_map.va, pst_pci_res->st_ringbuf.st_h2d_msg.wr);

#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    /* Flush cache */
    oal_pci_cache_flush(pst_pci_dev, (void *)st_map.pa, sizeof(pst_pci_res->st_ringbuf.st_h2d_msg.wr));
#endif

    wr_back = oal_pcie_read_mem32(st_map.va);
    if (wr_back != pst_pci_res->st_ringbuf.st_h2d_msg.wr) {
        pci_print_log(PCI_LOG_ERR, "pcie h2d message wr write failed, wr_back=%u, host_wr=%u",
                      wr_back, pst_pci_res->st_ringbuf.st_h2d_msg.wr);
        declare_dft_trace_key_info("h2d_message_wr_update_failed", OAL_DFT_TRACE_FAIL);
    }

    return OAL_SUCC;
}

/* update wr pointer to host ,check the read space */
OAL_STATIC int32_t oal_pcie_d2h_message_buf_wr_update(oal_pcie_res *pst_pci_res)
{
    uint32_t wr;
    pci_addr_map st_map;
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    oal_pci_dev_stru *pst_pci_dev = pcie_res_to_dev(pst_pci_res);
#endif

    st_map.va = pst_pci_res->st_message_res.d2h_res.ringbuf_ctrl_dma_addr.va + OAL_OFFSET_OF(pcie_ringbuf, wr);
    st_map.pa = pst_pci_res->st_message_res.d2h_res.ringbuf_ctrl_dma_addr.pa + OAL_OFFSET_OF(pcie_ringbuf, wr);

#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    /* 无效化cache */
    oal_pci_cache_inv(pst_pci_dev, (void *)st_map.pa, sizeof(pst_pci_res->st_ringbuf.st_d2h_msg.wr));
#endif
    wr = oal_pcie_read_mem32(st_map.va);
    if (oal_unlikely(wr == 0xFFFFFFFF)) {
        if (oal_pcie_check_link_state(pst_pci_res) == OAL_FALSE) {
            pci_print_log(PCI_LOG_ERR, "d2h message ringbuf wr update: link down[va:0x%lx, pa:0x%lx]",
                          st_map.va, st_map.pa);
            return -OAL_ENODEV;
        }
    }

    pci_print_log(PCI_LOG_DBG, "d2h message ringbuf wr update:[0x%lx:wr:0x%x]", st_map.va, wr);
    pst_pci_res->st_ringbuf.st_d2h_msg.wr = wr;
    return OAL_SUCC;
}

/* update rd pointer to device */
OAL_STATIC int32_t oal_pcie_d2h_message_buf_rd_update(oal_pcie_res *pst_pci_res)
{
    pci_addr_map st_map;
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    oal_pci_dev_stru *pst_pci_dev = pcie_res_to_dev(pst_pci_res);
#endif
    st_map.va = pst_pci_res->st_message_res.d2h_res.ringbuf_ctrl_dma_addr.va + OAL_OFFSET_OF(pcie_ringbuf, rd);
    st_map.pa = pst_pci_res->st_message_res.d2h_res.ringbuf_ctrl_dma_addr.pa + OAL_OFFSET_OF(pcie_ringbuf, rd);

    oal_pcie_write_mem32(st_map.va, pst_pci_res->st_ringbuf.st_d2h_msg.rd);
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    /* Flush cache */
    oal_pci_cache_flush(pst_pci_dev, (void *)st_map.pa, sizeof(pst_pci_res->st_ringbuf.st_d2h_msg.rd));
#endif
    return OAL_SUCC;
}

/* Update rd pointer, Return the bytes we read */
OAL_STATIC int32_t oal_pcie_d2h_message_buf_read(oal_pcie_res *pst_pci_res, pcie_ringbuf *pst_ringbuf,
                                                 pci_addr_map *pst_ringbuf_base, uint32_t *message)
{
    uint32_t real_rd, wr, rd, data_size;
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    oal_pci_dev_stru *pst_pci_dev = pcie_res_to_dev(pst_pci_res);
#endif

    rd = pst_ringbuf->rd;
    wr = pst_ringbuf->wr;

    data_size = wr - rd;

    if (oal_unlikely((data_size < pst_ringbuf->item_len) || (pst_ringbuf->item_len != OAL_SIZEOF(uint32_t)))) {
        pci_print_log(PCI_LOG_ERR, "d2h message buf read failed, date_size[%d] < item_len:%d, wr:%u, rd:%u",
                      data_size, pst_ringbuf->item_len, wr, rd);
        return 0;
    }

    real_rd = rd & (pst_ringbuf->size - 1);

#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    /* 无效化cache */
    oal_pci_cache_inv(pst_pci_dev, (void *)pst_ringbuf_base->pa, pst_ringbuf->item_len);
#endif

    if (oal_pcie_check_link_state(pst_pci_res) == OAL_FALSE) {
        /* LinkDown */
        pci_print_log(PCI_LOG_ERR, "d2h message read detect linkdown.");
        return 0;
    }

    oal_pcie_io_trans((uintptr_t)(message), (uintptr_t)(pst_ringbuf_base->va + (unsigned long)real_rd),
                      pst_ringbuf->item_len);

    pst_ringbuf->rd += pst_ringbuf->item_len;

    /* Update device's read pointer */
    oal_pcie_d2h_message_buf_rd_update(pst_pci_res);

    return pst_ringbuf->item_len;
}

/* update rd pointer to device */
OAL_STATIC int32_t oal_pcie_ringbuf_write_rd(oal_pcie_res *pst_pci_res, PCIE_COMM_RINGBUF_TYPE type)
{
    pci_addr_map st_map;
    pcie_ringbuf *pst_ringbuf = NULL;
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    oal_pci_dev_stru *pst_pci_dev = pcie_res_to_dev(pst_pci_res);
#endif

    if (oal_unlikely(pst_pci_res->link_state < PCI_WLAN_LINK_RES_UP)) {
        pci_print_log(PCI_LOG_WARN, "comm ringbuf %d write rd failed, link_state:%s",
                      type, oal_pcie_get_link_state_str(pst_pci_res->link_state));
        return -OAL_ENODEV;
    }

    pst_ringbuf = &pst_pci_res->st_ringbuf.st_ringbuf[type];

    st_map.va = pst_pci_res->st_ringbuf_res.comm_rb_res[type].ctrl_daddr.va + OAL_OFFSET_OF(pcie_ringbuf, rd);
    st_map.pa = pst_pci_res->st_ringbuf_res.comm_rb_res[type].ctrl_daddr.pa + OAL_OFFSET_OF(pcie_ringbuf, rd);

    oal_pcie_write_mem32(st_map.va, pst_ringbuf->rd);
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    /* Flush cache */
    oal_pci_cache_flush(pst_pci_dev, (void *)st_map.pa, sizeof(pst_ringbuf->rd));
#endif
    return OAL_SUCC;
}

int32_t oal_pcie_ringbuf_write_wr(oal_pcie_res *pst_pci_res, PCIE_COMM_RINGBUF_TYPE type)
{
    pci_addr_map st_map;
    pcie_ringbuf *pst_ringbuf = NULL;

#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    oal_pci_dev_stru *pst_pci_dev = pcie_res_to_dev(pst_pci_res);
#endif

    if (oal_unlikely(pst_pci_res->link_state < PCI_WLAN_LINK_RES_UP)) {
        pci_print_log(PCI_LOG_WARN, "comm ringbuf %d write wr failed, link_state:%s",
                      type, oal_pcie_get_link_state_str(pst_pci_res->link_state));
        return -OAL_ENODEV;
    }

    pst_ringbuf = &pst_pci_res->st_ringbuf.st_ringbuf[type];

    st_map.va = pst_pci_res->st_ringbuf_res.comm_rb_res[type].ctrl_daddr.va + OAL_OFFSET_OF(pcie_ringbuf, wr);
    st_map.pa = pst_pci_res->st_ringbuf_res.comm_rb_res[type].ctrl_daddr.pa + OAL_OFFSET_OF(pcie_ringbuf, wr);

    oal_pcie_write_mem32(st_map.va, pst_ringbuf->wr);
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    /* Flush cache */
    oal_pci_cache_flush(pst_pci_dev, (void *)st_map.pa, sizeof(pst_ringbuf->wr));
#endif
    return OAL_SUCC;
}

int32_t oal_pcie_ringbuf_read_rd(oal_pcie_res *pst_pci_res, PCIE_COMM_RINGBUF_TYPE type)
{
    uint32_t rd;
    pci_addr_map st_map;
    pcie_ringbuf *pst_ringbuf = NULL;
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    oal_pci_dev_stru *pst_pci_dev = pcie_res_to_dev(pst_pci_res);
#endif

    if (oal_unlikely(pst_pci_res->link_state < PCI_WLAN_LINK_RES_UP)) {
        pci_print_log(PCI_LOG_WARN, "comm ringbuf %d read rd, link_state:%s",
                      type, oal_pcie_get_link_state_str(pst_pci_res->link_state));
        return -OAL_ENODEV;
    }

    pst_ringbuf = &pst_pci_res->st_ringbuf.st_ringbuf[type];

    st_map.va = pst_pci_res->st_ringbuf_res.comm_rb_res[type].ctrl_daddr.va + OAL_OFFSET_OF(pcie_ringbuf, rd);
    st_map.pa = pst_pci_res->st_ringbuf_res.comm_rb_res[type].ctrl_daddr.pa + OAL_OFFSET_OF(pcie_ringbuf, rd);

#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    /* 无效化cache */
    oal_pci_cache_inv(pst_pci_dev, (void *)st_map.pa, sizeof(pst_ringbuf->rd));
#endif

    rd = oal_pcie_read_mem32(st_map.va);
    if (oal_unlikely(rd == 0xFFFFFFFF)) {
        if (oal_pcie_check_link_state(pst_pci_res) == OAL_FALSE) {
            pci_print_log(PCI_LOG_ERR, "ringbuf %d read rd: link down[va:0x%lx, pa:0x%lx]",
                          type, st_map.va, st_map.pa);
            return -OAL_ENODEV;
        }
    }

    pci_print_log(PCI_LOG_DBG, "ringbuf %d read rd:[0x%lx:rd:0x%x]", type, st_map.va, rd);

    pst_ringbuf->rd = rd;

    return OAL_SUCC;
}

/* update wr pointer to device */
OAL_STATIC int32_t oal_pcie_ringbuf_read_wr(oal_pcie_res *pst_pci_res, PCIE_COMM_RINGBUF_TYPE type)
{
    uint32_t wr;
    pci_addr_map st_map;
    pcie_ringbuf *pst_ringbuf = NULL;
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    oal_pci_dev_stru *pst_pci_dev = pcie_res_to_dev(pst_pci_res);
#endif

    if (oal_unlikely(pst_pci_res->link_state < PCI_WLAN_LINK_RES_UP)) {
        pci_print_log(PCI_LOG_WARN, "comm ringbuf %d read wr failed, link_state:%s",
                      type, oal_pcie_get_link_state_str(pst_pci_res->link_state));
        return -OAL_ENODEV;
    }

    pst_ringbuf = &pst_pci_res->st_ringbuf.st_ringbuf[type];

    st_map.va = pst_pci_res->st_ringbuf_res.comm_rb_res[type].ctrl_daddr.va + OAL_OFFSET_OF(pcie_ringbuf, wr);
    st_map.pa = pst_pci_res->st_ringbuf_res.comm_rb_res[type].ctrl_daddr.pa + OAL_OFFSET_OF(pcie_ringbuf, wr);

#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    /* 无效化cache */
    oal_pci_cache_inv(pst_pci_dev, (void *)st_map.pa, sizeof(pst_ringbuf->wr));
#endif

    wr = oal_pcie_read_mem32(st_map.va);
    if (oal_unlikely(wr == 0xFFFFFFFF)) {
        if (oal_pcie_check_link_state(pst_pci_res) == OAL_FALSE) {
            pci_print_log(PCI_LOG_ERR, "ringbuf %d wr update: link down[va:0x%lx, pa:0x%lx]",
                          type, st_map.va, st_map.pa);
            return -OAL_ENODEV;
        }
    }

    pci_print_log(PCI_LOG_DBG, "ringbuf %d read wr:[0x%lx:wr:0x%x]", type, st_map.va, wr);
    pst_ringbuf->wr = wr;

    return OAL_SUCC;
}

/* Update rd pointer, Return the bytes we read */
OAL_STATIC int32_t oal_pcie_ringbuf_read(oal_pcie_res *pst_pci_res, PCIE_COMM_RINGBUF_TYPE type,
                                         uint8_t *buf, uint32_t len)
{
    uint32_t real_rd, wr, rd, data_size;
    pcie_ringbuf *pst_ringbuf = NULL;
    pci_addr_map *pst_ringbuf_base = &pst_pci_res->st_ringbuf_res.comm_rb_res[type].data_daddr;
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    oal_pci_dev_stru *pst_pci_dev = pcie_res_to_dev(pst_pci_res);
#endif

    if (oal_warn_on(pst_ringbuf_base->va == 0)) {
        pci_print_log(PCI_LOG_WARN, "comm ringbuf %d read failed, ringbuf base va is null", type);
        return -OAL_EINVAL;
    }

    if (oal_unlikely(pst_pci_res->link_state < PCI_WLAN_LINK_RES_UP)) {
        pci_print_log(PCI_LOG_WARN, "comm ringbuf %d read failed, link_state:%s",
                      type, oal_pcie_get_link_state_str(pst_pci_res->link_state));
        return -OAL_ENODEV;
    }

    pst_ringbuf = &pst_pci_res->st_ringbuf.st_ringbuf[type];

    if (oal_warn_on(len != pst_ringbuf->item_len)) {
        pci_print_log(PCI_LOG_WARN, "ringbuf %d read request len %u not equal to %u",
                      type, len, pst_ringbuf->item_len);
        return -OAL_EIO;
    }

    rd = pst_ringbuf->rd;
    wr = pst_ringbuf->wr;

    data_size = wr - rd;

    if ((data_size < pst_ringbuf->item_len)) {
        /* ringbuf is empty */
        return 0;
    }

    real_rd = rd & (pst_ringbuf->size - 1);

#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    /* 无效化cache */
    oal_pci_cache_inv(pst_pci_dev, (void *)pst_ringbuf_base->pa, pst_ringbuf->item_len);
#endif

    oal_pcie_io_trans((uintptr_t)(buf), (pst_ringbuf_base->va + (unsigned long)real_rd), pst_ringbuf->item_len);

    if (*buf == 0xff) {
        if (oal_pcie_check_link_state(pst_pci_res) == OAL_FALSE) {
            /* LinkDown */
            pci_print_log(PCI_LOG_ERR, "linkdown when comm ringbuf %d read", type);
            return -OAL_ENODEV;
        }
    }

    pst_ringbuf->rd += pst_ringbuf->item_len;

    /* Update device's read pointer */
    oal_pcie_ringbuf_write_rd(pst_pci_res, type);

    return pst_ringbuf->item_len;
}

int32_t oal_pcie_ringbuf_write(oal_pcie_res *pst_pci_res,
                               PCIE_COMM_RINGBUF_TYPE type, uint8_t *buf, uint32_t len)
{
    /* 不判断写指针，此函数只执行写操作 */
    oal_pci_dev_stru *pst_pci_dev;
    uint32_t real_wr;

    pcie_ringbuf *pst_ringbuf = &pst_pci_res->st_ringbuf.st_ringbuf[type];
    pci_addr_map *pst_ringbuf_base = &pst_pci_res->st_ringbuf_res.comm_rb_res[type].data_daddr;

    pst_pci_dev = pcie_res_to_dev(pst_pci_res);

    /* Debug */
    if (oal_unlikely(pst_pci_res->link_state < PCI_WLAN_LINK_RES_UP)) {
        pci_print_log(PCI_LOG_WARN, "comm ringbuf %d write failed, link_state:%s",
                      type, oal_pcie_get_link_state_str(pst_pci_res->link_state));
        return 0;
    }

    if (oal_warn_on(len != pst_ringbuf->item_len)) {
        pci_print_log(PCI_LOG_WARN, "ringbuf %d write request len %u not equal to %u",
                      type, len, pst_ringbuf->item_len);
        return 0;
    }

    if (oal_warn_on(pst_ringbuf->wr - pst_ringbuf->rd >= pst_ringbuf->size)) {
        /* never touch here */
        pci_print_log(PCI_LOG_ERR, "ringbuf %d full [wr:%u] [rd:%u] [size:%u]\n",
                      type, pst_ringbuf->wr, pst_ringbuf->rd, pst_ringbuf->size);
        return 0;
    }

    real_wr = pst_ringbuf->wr & (pst_ringbuf->size - 1);
    oal_pcie_io_trans(pst_ringbuf_base->va + real_wr, (uintptr_t)buf, pst_ringbuf->item_len);
    if (pci_dbg_condtion()) {
        int32_t ret;
        uint64_t cpuaddr;
        ret = oal_pcie_get_ca_by_pa(pst_pci_res, pst_ringbuf_base->pa, &cpuaddr);
        if (ret == OAL_SUCC) {
            pci_print_log(PCI_LOG_DBG, "ringbuf %d write ringbuf data cpu address:0x%llx", type, cpuaddr);
        } else {
            pci_print_log(PCI_LOG_DBG, "ringbuf %d rd pa:0x%lx invaild", type, pst_ringbuf_base->pa);
        }
        oal_print_hex_dump((uint8_t *)buf, pst_ringbuf->item_len, pst_ringbuf->item_len, "ringbuf write: ");
    }

#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    oal_pci_cache_flush(pst_pci_dev, (void *)pst_ringbuf_base->pa, pst_ringbuf->item_len);
#endif

    pst_ringbuf->wr += pst_ringbuf->item_len;

    return 1;
}

uint32_t oal_pcie_comm_ringbuf_freecount(oal_pcie_res *pst_pci_res,
                                         PCIE_COMM_RINGBUF_TYPE type)
{
    pcie_ringbuf *pst_ringbuf = &pst_pci_res->st_ringbuf.st_ringbuf[type];
    return oal_pcie_ringbuf_freecount(pst_ringbuf);
}

int32_t oal_pcie_read_d2h_message(oal_pcie_res *pst_pci_res, uint32_t *message)
{
    int32_t ret;
    uint32_t len;
    pcie_ringbuf *pst_ringbuf = NULL;
    if (oal_unlikely(pst_pci_res == NULL)) {
        return -OAL_EINVAL;
    }

    if (oal_unlikely(pst_pci_res->link_state < PCI_WLAN_LINK_RES_UP)) {
        pci_print_log(PCI_LOG_INFO, "link state is disabled:%s!",
                      oal_pcie_get_link_state_str(pst_pci_res->link_state));
        return -OAL_ENODEV;
    }

    pst_ringbuf = &pst_pci_res->st_ringbuf.st_d2h_msg;
    pci_print_log(PCI_LOG_DBG, "oal_pcie_read_d2h_message ++");

    len = pcie_ringbuf_len(pst_ringbuf);
    if (len == 0) {
        /* No Message, update wr pointer and retry */
        ret = oal_pcie_d2h_message_buf_wr_update(pst_pci_res);
        if (ret != OAL_SUCC) {
            return ret;
        }
        len = pcie_ringbuf_len(&pst_pci_res->st_ringbuf.st_d2h_msg);
    }

    if (len == 0) {
        return -OAL_ENODEV;
    }

    if (oal_pcie_d2h_message_buf_read(pst_pci_res, pst_ringbuf,
                                      &pst_pci_res->st_message_res.d2h_res.ringbuf_data_dma_addr, message)) {
        pci_print_log(PCI_LOG_DBG, "oal_pcie_read_d2h_message --");
        return OAL_SUCC;
    } else {
        pci_print_log(PCI_LOG_DBG, "oal_pcie_read_d2h_message ^^");
        return -OAL_EINVAL;
    }
}

void oal_pcie_trigger_message(oal_pcie_res *pst_pci_res)
{
    if (pst_pci_res->chip_info.edma_support == OAL_TRUE) {
        /* 触发h2d int */
        oal_writel(PCIE_H2D_TRIGGER_VALUE, pst_pci_res->pst_pci_ctrl_base + PCIE_D2H_DOORBELL_OFF);
    } else if (pst_pci_res->chip_info.ete_support == OAL_TRUE) {
        oal_pcie_h2d_int(pst_pci_res);
    } else {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "unkown dma");
    }
}

int32_t oal_pcie_send_message_to_dev(oal_pcie_res *pst_pci_res, uint32_t message)
{
    int32_t ret;
    uint32_t freecount;
    if (oal_warn_on(pst_pci_res == NULL)) {
        return -OAL_ENODEV;
    }

    if (oal_unlikely(!pst_pci_res->regions.inited)) {
        pci_print_log(PCI_LOG_ERR, "region is disabled!");
        return -OAL_EFAUL;
    }

    if (oal_unlikely(pst_pci_res->link_state < PCI_WLAN_LINK_RES_UP)) {
        pci_print_log(PCI_LOG_WARN, "send message link invaild, link_state:%s",
                      oal_pcie_get_link_state_str(pst_pci_res->link_state));
        return -OAL_EBUSY;
    }

    /* message ringbuf freecount */
    oal_spin_lock(&pst_pci_res->st_message_res.h2d_res.lock);
    freecount = oal_pcie_ringbuf_freecount(&pst_pci_res->st_ringbuf.st_h2d_msg);
    if (freecount == 0) {
        /* no space, sync rd pointer */
        oal_pcie_h2d_message_buf_rd_update(pst_pci_res);
        freecount = oal_pcie_ringbuf_freecount(&pst_pci_res->st_ringbuf.st_h2d_msg);
    }

    if (freecount == 0) {
        oal_spin_unlock(&pst_pci_res->st_message_res.h2d_res.lock);
        return -OAL_EBUSY;
    }

    /* write message to ringbuf */
    ret = oal_pcie_h2d_message_buf_write(pst_pci_res, &pst_pci_res->st_ringbuf.st_h2d_msg,
                                         &pst_pci_res->st_message_res.h2d_res.ringbuf_data_dma_addr,
                                         message);
    if (ret <= 0) {
        oal_spin_unlock(&pst_pci_res->st_message_res.h2d_res.lock);
        if (oal_pcie_check_link_state(pst_pci_res) == OAL_FALSE) {
            /* Should trigger DFR here */
            pci_print_log(PCI_LOG_ERR, "h2d message send failed: link down, ret=%d", ret);
        }
        return -OAL_EIO;
    }

    /* 更新写指针 */
    oal_pcie_h2d_message_buf_wr_update(pst_pci_res);

    /* 触发h2d int */
    oal_pcie_trigger_message(pst_pci_res);

    oal_spin_unlock(&pst_pci_res->st_message_res.h2d_res.lock);

    return OAL_SUCC;
}

int32_t oal_pcie_get_host_trans_count(oal_pcie_res *pst_pci_res, uint64_t *tx, uint64_t *rx)
{
    if (tx != NULL) {
        int32_t i;
        *tx = 0;
        for (i = 0; i < PCIE_H2D_QTYPE_BUTT; i++) {
            *tx += (uint64_t)pst_pci_res->st_tx_res[i].stat.tx_count;
        }
    }
    if (rx != NULL) {
        *rx = (uint64_t)pst_pci_res->st_rx_res.stat.rx_count;
    }
    return OAL_SUCC;
}

int32_t oal_pcie_edma_sleep_request_host_check(oal_pcie_res *pst_pci_res)
{
    int32_t i;
    uint32_t len = 0;
    uint32_t total_len = 0;

    /* 此时allow sleep 应该tx也被释放 */
    for (i = 0; i < PCIE_H2D_QTYPE_BUTT; i++) {
        len = oal_netbuf_list_len(&pst_pci_res->st_tx_res[i].txq);
        if (len) {
            pci_print_log(PCI_LOG_INFO, "txq:%d still had skb len:%u", i, len);
        }
        total_len += len;
    }

    return (total_len != 0) ? -OAL_EBUSY : OAL_SUCC;
}

static int32_t pcie_send_check_param(oal_pcie_res *pst_pci_res, oal_netbuf_head_stru *pst_head)
{
    if (oal_unlikely(pst_pci_res == NULL || pst_head == NULL)) {
        pci_print_log(PCI_LOG_ERR, "invalid input pst_pci_res is %pK, pst_head %pK", pst_pci_res, pst_head);
        return -OAL_EINVAL;
    }

    /* pcie is link */
    if (oal_unlikely(pst_pci_res->link_state <= PCI_WLAN_LINK_DOWN)) {
        pci_print_log(PCI_LOG_WARN, "linkdown hold the pkt, link_state:%s",
                      oal_pcie_get_link_state_str(pst_pci_res->link_state));
        return 0;
    }

    if (oal_unlikely(oal_netbuf_list_empty(pst_head))) {
        return 0;
    }

    return 1;
}

int32_t oal_pcie_send_netbuf_list(oal_pcie_res *pst_pci_res, oal_netbuf_head_stru *pst_head,
                                  PCIE_H2D_RINGBUF_QTYPE qtype)
{
    unsigned long flags;
    oal_pcie_linux_res *pst_pci_lres = NULL;
    oal_netbuf_stru *pst_netbuf = NULL;
    oal_pci_dev_stru *pst_pci_dev = NULL;
    dma_addr_t pci_dma_addr;
    pcie_read_ringbuf_item st_item;
    pcie_cb_dma_res st_cb_dma;
    int32_t send_cnt, queue_cnt, total_cnt;
    const uint32_t ul_max_dump_bytes = 128;
    int32_t ret;

    ret = pcie_send_check_param(pst_pci_res, pst_head);
    if (ret <= 0) {
        return ret;
    }

    pst_pci_dev = pcie_res_to_dev(pst_pci_res);
    pst_pci_lres = (oal_pcie_linux_res *)oal_pci_get_drvdata(pst_pci_dev);

    if (oal_unlikely(OAL_SUCC != hcc_bus_pm_wakeup_device(pst_pci_lres->pst_bus))) {
        oal_msleep(100); /* wait for a while retry */
        return -OAL_EIO;
    }

    if (oal_unlikely(pst_pci_res->link_state < PCI_WLAN_LINK_WORK_UP)) {
        pci_print_log(PCI_LOG_WARN, "send netbuf link invaild, link_state:%s",
                      oal_pcie_get_link_state_str(pst_pci_res->link_state));
        return -OAL_ENODEV;
    }

    queue_cnt = oal_netbuf_list_len(pst_head);
    send_cnt = oal_pcie_h2d_ringbuf_freecount(pst_pci_res, qtype, OAL_FALSE);

    if (queue_cnt > send_cnt) {
        /* ringbuf 空间不够, 刷新rd指针，重新判断 */
        send_cnt = oal_pcie_h2d_ringbuf_freecount(pst_pci_res, qtype, OAL_TRUE);
    }

    if (send_cnt == 0) {
        /* ringbuf 为满 */
        return 0;
    }

    pci_print_log(PCI_LOG_DBG, "[q:%d]h2d_ringbuf freecount:%u, qlen:%u", (int32_t)qtype, send_cnt, queue_cnt);

    total_cnt = 0;

    forever_loop() {
        /* 填充ringbuf */
        if (oal_pcie_h2d_ringbuf_freecount(pst_pci_res, qtype, OAL_FALSE) == 0) {
            break;
        }

        /* 取netbuf */
        pst_netbuf = oal_netbuf_delist(pst_head);
        if (pst_netbuf == NULL) {
            break;
        }

        /* Debug */
        if (oal_unlikely(pst_pci_res->link_state < PCI_WLAN_LINK_WORK_UP)) {
            pci_print_log(PCI_LOG_WARN, "oal_pcie_send_netbuf_list loop invaild, link_state:%s",
                          oal_pcie_get_link_state_str(pst_pci_res->link_state));
            hcc_tx_netbuf_free(pst_netbuf);
            return -OAL_ENODEV;
        }

        pci_dma_addr = dma_map_single(&pst_pci_dev->dev, oal_netbuf_data(pst_netbuf),
                                      oal_netbuf_len(pst_netbuf), PCI_DMA_TODEVICE);
        if (dma_mapping_error(&pst_pci_dev->dev, pci_dma_addr)) {
            /* 映射失败先简单处理丢掉netbuf, dma mask 范围内 这里只是刷Cache */
            declare_dft_trace_key_info("pcie tx map failed", OAL_DFT_TRACE_OTHER);
            oal_print_hi11xx_log(HI11XX_LOG_ERR, "edma tx map failed, va=%p, len=%d", oal_netbuf_data(pst_netbuf),
                                 oal_netbuf_len(pst_netbuf));
            hcc_tx_netbuf_free(pst_netbuf);
            continue;
        }

        pci_print_log(PCI_LOG_DBG, "send netbuf 0x%p, dma pa:0x%llx", pst_netbuf, (uint64_t)pci_dma_addr);
        if (pci_dbg_condtion()) {
            oal_print_hex_dump(oal_netbuf_data(pst_netbuf),
                               oal_netbuf_len(pst_netbuf) <
                               ul_max_dump_bytes
                               ? oal_netbuf_len(pst_netbuf)
                               : ul_max_dump_bytes,
                               HEX_DUMP_GROUP_SIZE, "netbuf: ");
        }

        /* 64bits 传输, 不考虑大小端 */
        st_item.buff_paddr.addr = (uint64_t)pci_dma_addr;

        /* 这里的长度包含64B的头 */
        if (oal_likely(oal_netbuf_len(pst_netbuf) >= HCC_HDR_TOTAL_LEN)) {
            /* tx ringbuf中的长度不包含头,就算包含也只是多传输一个头的长度 */
            /* 计算4字节对齐后的长度，默认进位 */
            st_item.buf_len = padding_m((oal_netbuf_len(pst_netbuf) - HCC_HDR_TOTAL_LEN), 4);
        } else {
            pci_print_log(PCI_LOG_WARN, "tx netbuf too short %u < %u\n",
                          oal_netbuf_len(pst_netbuf), HCC_HDR_TOTAL_LEN);
            declare_dft_trace_key_info("tx netbuf too short", OAL_DFT_TRACE_FAIL);
        }

        st_item.reserved0 = 0x4321;

        st_cb_dma.paddr.addr = (uint64_t)pci_dma_addr;
        st_cb_dma.len = oal_netbuf_len(pst_netbuf);

        /* dma地址和长度存在CB字段中，发送完成后释放DMA地址 */
        ret = memcpy_s((uint8_t *)oal_netbuf_cb(pst_netbuf) + sizeof(struct hcc_tx_cb_stru),
                       oal_netbuf_cb_size() - sizeof(struct hcc_tx_cb_stru), &st_cb_dma, sizeof(st_cb_dma));
        if (ret != EOK) {
            pci_print_log(PCI_LOG_ERR, "dma addr copy failed");
        }

        /* netbuf入队 */
        oal_spin_lock_irq_save(&pst_pci_res->st_tx_res[qtype].lock, &flags);
        oal_netbuf_list_tail_nolock(&pst_pci_res->st_tx_res[qtype].txq, pst_netbuf);
        oal_spin_unlock_irq_restore(&pst_pci_res->st_tx_res[qtype].lock, &flags);

        pci_print_log(PCI_LOG_DBG, "h2d ringbuf write 0x%llx, len:%u", st_item.buff_paddr.addr, st_item.buf_len);

        /* 这里直接写，上面已经判断过ringbuf有空间 */
        total_cnt += oal_pcie_h2d_ringbuf_write(pst_pci_res,
                                                &pst_pci_res->st_tx_res[qtype].ringbuf_data_dma_addr,
                                                qtype, &st_item);
    }

    if (total_cnt) {
        /* 更新device侧wr指针,刷ringbuf cache */
        oal_pcie_h2d_ringbuf_wr_update(pst_pci_res, qtype);

        /* tx doorbell */
        oal_pcie_h2d_doorbell(pst_pci_res);
    } else {
        declare_dft_trace_key_info("pcie send toal cnt error", OAL_DFT_TRACE_FAIL);
    }

    return total_cnt;
}

OAL_STATIC int32_t oal_pcie_print_pcie_regs(oal_pcie_res *pst_pci_res, uint32_t base, uint32_t size)
{
    int32_t i;
    uint32_t value;
    void *pst_mem = NULL;
    pci_addr_map addr_map;
    size = OAL_ROUND_UP(size, 4); /* 计算4字节对齐后的长度，默认进位 */
    if ((size == 0) || (size > 0xffff)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "size invalid %u\n", size);
        return -OAL_EFAIL;
    }

    if (oal_pcie_inbound_ca_to_va(pst_pci_res, base, &addr_map) != OAL_SUCC) {
        /* share mem 地址未映射! */
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "can not found mem map for dev cpu address 0x%x\n", base);
        return -OAL_EFAIL;
    }

    pst_mem = vmalloc(size);
    if (pst_mem == NULL) {
        pci_print_log(PCI_LOG_WARN, "vmalloc mem size %u failed", size);
    } else {
        memset_s(pst_mem, size, 0, size);
    }

    for (i = 0; i < size; i += sizeof(uint32_t)) { /* 每次偏移4字节 */
        value = oal_readl((void *)addr_map.va + i);
        if (value == 0xffffffff) {
            if (oal_pcie_device_check_alive(pst_pci_res) != OAL_SUCC) {
                if (pst_mem != NULL) {
                    vfree(pst_mem);
                }
                return -OAL_ENODEV;
            }
        }

        if (pst_mem != NULL) {
            oal_writel(value, pst_mem + i);
        } else {
            oal_io_print("%8x:%8x\n", base + i, value);
        }
    }

    if (pst_mem != NULL) {
        if (i) {
            pci_print_log(PCI_LOG_INFO, "dump regs base 0x%x", base);
#ifdef CONFIG_PRINTK
            /* print to kenrel msg */
            print_hex_dump(KERN_INFO, "pcie regs: ", DUMP_PREFIX_OFFSET, 32, 4,
                           pst_mem, i, false); /* 内核函数固定的传参 */
#endif
        }

        vfree(pst_mem);
    }

    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_print_device_transfer_info(oal_pcie_res *pst_pcie_res)
{
    int32_t ret;
    if (pst_pcie_res->st_device_stat_map.va != 0) {
        ret = oal_pcie_device_check_alive(pst_pcie_res);
        if (ret != OAL_SUCC) {
            return ret;
        }

        pci_print_log(PCI_LOG_INFO, "show device info:");

#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
        oal_pci_cache_inv(pst_pci_dev, (void *)pst_pcie_res->st_device_stat_map.pa,
                          sizeof(pst_pcie_res->st_device_stat));
#endif
        oal_pcie_io_trans((uintptr_t)&pst_pcie_res->st_device_stat,
                          pst_pcie_res->st_device_stat_map.va,
                          sizeof(pst_pcie_res->st_device_stat));

        pci_print_log(PCI_LOG_INFO, "d2h fifo_full:%u, fifo_notfull:%u ringbuf_hit:%u, ringbuf_miss:%u\n",
                      pst_pcie_res->st_device_stat.d2h_stats.stat.fifo_full,
                      pst_pcie_res->st_device_stat.d2h_stats.stat.fifo_notfull,
                      pst_pcie_res->st_device_stat.d2h_stats.stat.ringbuf_hit,
                      pst_pcie_res->st_device_stat.d2h_stats.stat.ringbuf_miss);
        pci_print_log(PCI_LOG_INFO, "d2h dma_busy:%u, dma_idle:%u fifo_ele_empty:%u doorbell count:%u\n",
                      pst_pcie_res->st_device_stat.d2h_stats.stat.fifo_dma_busy,
                      pst_pcie_res->st_device_stat.d2h_stats.stat.fifo_dma_idle,
                      pst_pcie_res->st_device_stat.d2h_stats.stat.fifo_ele_empty,
                      pst_pcie_res->st_device_stat.d2h_stats.stat.doorbell_isr_count);

        pci_print_log(PCI_LOG_INFO,
                      "d2h push_fifo_count:%u done_isr_count:%u dma_work_list_stat:%u \
                      dma_free_list_stat:%u dma_pending_list_stat:%u",
                      pst_pcie_res->st_device_stat.d2h_stats.stat.push_fifo_count,
                      pst_pcie_res->st_device_stat.d2h_stats.stat.done_isr_count,
                      pst_pcie_res->st_device_stat.d2h_stats.stat.dma_work_list_stat,
                      pst_pcie_res->st_device_stat.d2h_stats.stat.dma_free_list_stat,
                      pst_pcie_res->st_device_stat.d2h_stats.stat.dma_pending_list_stat);

        pci_print_log(PCI_LOG_INFO,
                      "h2d fifo_full:%u, fifo_notfull:%u ringbuf_hit:%u, ringbuf_miss:%u fifo_ele_empty:%u\n",
                      pst_pcie_res->st_device_stat.h2d_stats.stat.fifo_full,
                      pst_pcie_res->st_device_stat.h2d_stats.stat.fifo_notfull,
                      pst_pcie_res->st_device_stat.h2d_stats.stat.ringbuf_hit,
                      pst_pcie_res->st_device_stat.h2d_stats.stat.ringbuf_miss,
                      pst_pcie_res->st_device_stat.h2d_stats.stat.fifo_ele_empty);

        pci_print_log(PCI_LOG_INFO,
                      "h2d push_fifo_count:%u done_isr_count:%u dma_work_list_stat:%u dma_free_list_stat:%u \
                      dma_pending_list_stat:%u",
                      pst_pcie_res->st_device_stat.h2d_stats.stat.push_fifo_count,
                      pst_pcie_res->st_device_stat.h2d_stats.stat.done_isr_count,
                      pst_pcie_res->st_device_stat.h2d_stats.stat.dma_work_list_stat,
                      pst_pcie_res->st_device_stat.h2d_stats.stat.dma_free_list_stat,
                      pst_pcie_res->st_device_stat.h2d_stats.stat.dma_pending_list_stat);

        pci_print_log(PCI_LOG_INFO,
                      "comm_stat l1_wake_force_push_cnt:%u l1_wake_l1_hit:%u l1_wake_l1_miss:%u \
                      l1_wake_state_err_cnt:%u l1_wake_timeout_cnt:%u l1_wake_timeout_max_cnt:%u",
                      pst_pcie_res->st_device_stat.comm_stat.l1_wake_force_push_cnt,
                      pst_pcie_res->st_device_stat.comm_stat.l1_wake_l1_hit,
                      pst_pcie_res->st_device_stat.comm_stat.l1_wake_l1_miss,
                      pst_pcie_res->st_device_stat.comm_stat.l1_wake_state_err_cnt,
                      pst_pcie_res->st_device_stat.comm_stat.l1_wake_timeout_cnt,
                      pst_pcie_res->st_device_stat.comm_stat.l1_wake_timeout_max_cnt);
        if (pst_pcie_res->st_device_stat.comm_stat.l1_wake_force_push_cnt) {
            declare_dft_trace_key_info("l1_wake_force_push_error", OAL_DFT_TRACE_FAIL);
        }
        if (pst_pcie_res->st_device_stat.comm_stat.l1_wake_state_err_cnt) {
            declare_dft_trace_key_info("l1_wake_state_err", OAL_DFT_TRACE_FAIL);
        }
    } else {
        pci_print_log(PCI_LOG_INFO, "st_device_stat_map null:va:%lu, pa:%lu\n",
                      pst_pcie_res->st_device_stat_map.va, pst_pcie_res->st_device_stat_map.pa);
    }

    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_print_all_ringbuf_info(oal_pcie_res *pst_pci_res)
{
    int32_t i;
    oal_pcie_print_ringbuf_info(&pst_pci_res->st_ringbuf.st_d2h_buf, PCI_LOG_INFO);
    for (i = 0; i < PCIE_H2D_QTYPE_BUTT; i++) {
        oal_pcie_print_ringbuf_info(&pst_pci_res->st_ringbuf.st_h2d_buf[i], PCI_LOG_INFO);
    }
    oal_pcie_print_ringbuf_info(&pst_pci_res->st_ringbuf.st_d2h_msg, PCI_LOG_INFO);
    oal_pcie_print_ringbuf_info(&pst_pci_res->st_ringbuf.st_h2d_msg, PCI_LOG_INFO);

    /* soft ringbuf */
    return OAL_SUCC;
}

void oal_pcie_print_transfer_info(oal_pcie_res *pst_pci_res, uint64_t print_flag)
{
    int32_t i = 0;
    int32_t j = 0;
    uint32_t len = 0;
    uint32_t total_len = 0;

    if (pst_pci_res == NULL) {
        return;
    }

    pci_print_log(PCI_LOG_INFO, "pcie transfer info:");
    if (pst_pci_res->stat.intx_total_count) {
        pci_print_log(PCI_LOG_INFO, "intx_total_count:%u", pst_pci_res->stat.intx_total_count);
    }
    if (pst_pci_res->stat.intx_tx_count) {
        pci_print_log(PCI_LOG_INFO, "intx_tx_count:%u", pst_pci_res->stat.intx_tx_count);
    }
    if (pst_pci_res->stat.intx_rx_count) {
        pci_print_log(PCI_LOG_INFO, "intx_rx_count:%u", pst_pci_res->stat.intx_rx_count);
    }
    if (pst_pci_res->stat.done_err_cnt) {
        pci_print_log(PCI_LOG_INFO, "done_err_cnt:%u", pst_pci_res->stat.done_err_cnt);
    }
    if (pst_pci_res->stat.h2d_doorbell_cnt) {
        pci_print_log(PCI_LOG_INFO, "h2d_doorbell_cnt:%u", pst_pci_res->stat.h2d_doorbell_cnt);
    }
    if (pst_pci_res->stat.d2h_doorbell_cnt) {
        pci_print_log(PCI_LOG_INFO, "d2h_doorbell_cnt:%u", pst_pci_res->stat.d2h_doorbell_cnt);
    }
    if (pst_pci_res->st_rx_res.stat.rx_count) {
        pci_print_log(PCI_LOG_INFO, "rx_count:%u", pst_pci_res->st_rx_res.stat.rx_count);
    }
    if (pst_pci_res->st_rx_res.stat.rx_done_count) {
        pci_print_log(PCI_LOG_INFO, "rx_done_count:%u", pst_pci_res->st_rx_res.stat.rx_done_count);
    }
    if (pst_pci_res->st_rx_res.stat.alloc_netbuf_failed) {
        pci_print_log(PCI_LOG_INFO, "alloc_netbuf_failed:%u", pst_pci_res->st_rx_res.stat.alloc_netbuf_failed);
    }
    if (pst_pci_res->st_rx_res.stat.map_netbuf_failed) {
        pci_print_log(PCI_LOG_INFO, "map_netbuf_failed:%u", pst_pci_res->st_rx_res.stat.map_netbuf_failed);
    }

    /* tx info */
    for (i = 0; i < PCIE_H2D_QTYPE_BUTT; i++) {
        if (pst_pci_res->st_tx_res[i].stat.tx_count) {
            pci_print_log(PCI_LOG_INFO, "[qid:%d]tx_count:%u", i, pst_pci_res->st_tx_res[i].stat.tx_count);
        }
        if (pst_pci_res->st_tx_res[i].stat.tx_done_count) {
            pci_print_log(PCI_LOG_INFO, "[qid:%d]tx_done_count:%u", i,
                          pst_pci_res->st_tx_res[i].stat.tx_done_count);
        }
        len = oal_netbuf_list_len(&pst_pci_res->st_tx_res[i].txq);
        if (len) {
            pci_print_log(PCI_LOG_INFO, "[qid:%d]len=%d", i, len);
            total_len += len;
        }

        pci_print_log(PCI_LOG_INFO, "[qid:%d]tx ringbuf cond is %d",
                      i, oal_atomic_read(&pst_pci_res->st_tx_res[i].tx_ringbuf_sync_cond));
    }

    /* burst info */
    for (i = 0; i < PCIE_EDMA_WRITE_BUSRT_COUNT + 1; i++) {
        if (pst_pci_res->st_rx_res.stat.rx_burst_cnt[i]) {
            pci_print_log(PCI_LOG_INFO, "rx burst %d count:%u", i, pst_pci_res->st_rx_res.stat.rx_burst_cnt[i]);
        }
    }

    for (i = 0; i < PCIE_H2D_QTYPE_BUTT; i++) {
        for (j = 0; j < PCIE_EDMA_READ_BUSRT_COUNT + 1; j++) {
            if (pst_pci_res->st_tx_res[i].stat.tx_burst_cnt[j]) {
                pci_print_log(PCI_LOG_INFO, "tx qid %d burst %d count:%u",
                              i, j, pst_pci_res->st_tx_res[i].stat.tx_burst_cnt[j]);
            }
        }
    }

    oal_pcie_print_all_ringbuf_info(pst_pci_res);

    /* dump pcie hardware info */
    if (oal_unlikely(pst_pci_res->link_state >= PCI_WLAN_LINK_WORK_UP)) {
        if (board_get_host_wakeup_dev_stat() == 1) {
            /* gpio is high axi is alive */
            if (print_flag & HCC_PRINT_TRANS_FLAG_DEVICE_REGS) {
                oal_pcie_print_pcie_regs(pst_pci_res, PCIE_CTRL_BASE_ADDR, 0x4c8 + 0x4);
                oal_pcie_print_pcie_regs(pst_pci_res, PCIE_DMA_CTRL_BASE_ADDR, 0x30 + 0x4);
            }

            if (print_flag & HCC_PRINT_TRANS_FLAG_DEVICE_STAT) {
                /* dump pcie status */
                oal_pcie_print_device_transfer_info(pst_pci_res);
            }
        }
    } else {
        pci_print_log(PCI_LOG_INFO, "pcie is %s", g_pcie_link_state_str[pst_pci_res->link_state]);
    }
}

void oal_pcie_reset_transfer_info(oal_pcie_res *pst_pci_res)
{
    int32_t i = 0;

    if (pst_pci_res == NULL) {
        return;
    }

    pci_print_log(PCI_LOG_INFO, "reset transfer info");
    pst_pci_res->stat.intx_total_count = 0;
    pst_pci_res->stat.intx_tx_count = 0;
    pst_pci_res->stat.intx_rx_count = 0;
    pst_pci_res->stat.h2d_doorbell_cnt = 0;
    pst_pci_res->stat.d2h_doorbell_cnt = 0;
    pst_pci_res->st_rx_res.stat.rx_count = 0;
    pst_pci_res->st_rx_res.stat.rx_done_count = 0;

    for (i = 0; i < PCIE_H2D_QTYPE_BUTT; i++) {
        pst_pci_res->st_tx_res[i].stat.tx_count = 0;
        pst_pci_res->st_tx_res[i].stat.tx_done_count = 0;
        memset_s((void *)pst_pci_res->st_tx_res[i].stat.tx_burst_cnt,
                 sizeof(pst_pci_res->st_tx_res[i].stat.tx_burst_cnt), 0,
                 sizeof(pst_pci_res->st_tx_res[i].stat.tx_burst_cnt));
    }

    memset_s((void *)pst_pci_res->st_rx_res.stat.rx_burst_cnt,
             sizeof(pst_pci_res->st_rx_res.stat.rx_burst_cnt), 0,
             sizeof(pst_pci_res->st_rx_res.stat.rx_burst_cnt));
}

int32_t oal_pcie_host_pending_signal_check(oal_pcie_res *pst_pci_res)
{
    int32_t i = 0;
    for (i = 0; i < PCIE_H2D_QTYPE_BUTT; i++) {
        if (oal_atomic_read(&pst_pci_res->st_tx_res[i].tx_ringbuf_sync_cond)) {
            return OAL_TRUE;
        }
    }

    return OAL_FALSE;
}

int32_t oal_pcie_host_pending_signal_process(oal_pcie_res *pst_pci_res)
{
    int32_t i = 0;
    int32_t total_cnt = 0;
    oal_pcie_linux_res *pst_pci_lres;
    oal_pci_dev_stru *pst_pci_dev;
    pst_pci_dev = pcie_res_to_dev(pst_pci_res);
    pst_pci_lres = (oal_pcie_linux_res *)oal_pci_get_drvdata(pst_pci_dev);
    if (oal_unlikely(OAL_SUCC != hcc_bus_pm_wakeup_device(pst_pci_lres->pst_bus))) {
        oal_msleep(100); /* wait for a while retry */
        for (i = 0; i < PCIE_H2D_QTYPE_BUTT; i++) {
            oal_atomic_set(&pst_pci_res->st_tx_res[i].tx_ringbuf_sync_cond, 0);
        }
        return total_cnt;
    }

    for (i = 0; i < PCIE_H2D_QTYPE_BUTT; i++) {
        if (oal_atomic_read(&pst_pci_res->st_tx_res[i].tx_ringbuf_sync_cond)) {
            oal_atomic_set(&pst_pci_res->st_tx_res[i].tx_ringbuf_sync_cond, 0);
            if (oal_pcie_h2d_ringbuf_rd_update(pst_pci_res, (PCIE_H2D_RINGBUF_QTYPE)i) == OAL_SUCC) {
                total_cnt++;
            }
        }
    }

    return total_cnt;
}

/* Debug functions */
OAL_STATIC int32_t oal_pcie_dump_all_regions_mem(struct kobject *dev, struct kobj_attribute *attr,
                                                 const char *buf, oal_pcie_res *pst_pcie_res)
{
    int32_t index;
    uint32_t region_num;
    oal_pcie_region *region_base = NULL;
    oal_pci_dev_stru *pst_pci_dev = NULL;

    /* 1181 test , get pci cfg memory */
    if (pst_pcie_res == NULL) {
        pci_print_log(PCI_LOG_WARN, "pci not init!\n");
        return -OAL_EBUSY;
    }

    pst_pci_dev = pcie_res_to_dev(pst_pcie_res);
    if (pst_pci_dev == NULL) {
        return -OAL_ENODEV;
    }

    if (oal_warn_on(!pst_pcie_res->regions.inited)) {
        return -OAL_EBUSY;
    }

    region_num = pst_pcie_res->regions.region_nums;
    region_base = pst_pcie_res->regions.pst_regions;

    for (index = 0; index < region_num; index++, region_base++) {
        if (region_base->vaddr == NULL) {
            continue;
        }
        if (region_base->vaddr != NULL) {
            uint32_t size = region_base->size > 256 ? 256 : region_base->size; /* 表示最多打印256字节 */
            oal_io_print("dump region[%d],name:%s, cpu addr:0x%llx\n",
                         index, region_base->name, region_base->cpu_start);
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
            oal_pci_cache_inv(pst_pci_dev, (void *)(uintptr_t) region_base->paddr, size);
#endif
            oal_print_hex_dump((uint8_t *)region_base->vaddr, size, HEX_DUMP_GROUP_SIZE, "pci mem  ");
        }
    }

    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_dump_all_regions(struct kobject *dev, struct kobj_attribute *attr,
                                             const char *buf, oal_pcie_res *pst_pcie_res)
{
    if (pst_pcie_res == NULL) {
        pci_print_log(PCI_LOG_ERR, "pci not init!\n");
        return -OAL_EBUSY;
    }

    oal_pcie_regions_info_dump(pst_pcie_res);
    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_debug_read32(struct kobject *dev, struct kobj_attribute *attr,
                                         const char *buf, oal_pcie_res *pst_pcie_res)
{
    /* echo read32 address > debug */
    int32_t ret;
    pci_addr_map addr_map;
    uint32_t cpu_address = 0xFFFFFFFF;
    uint32_t value;
    oal_pci_dev_stru *pst_pci_dev = NULL;

    if ((sscanf_s(buf, "0x%x", &cpu_address) != 1)) {
        pci_print_log(PCI_LOG_ERR, "read32 argument invalid,[%s]", buf);
        return -OAL_EINVAL;
    }

    if (pst_pcie_res == NULL) {
        pci_print_log(PCI_LOG_ERR, "pst_pcie_res is null");
        return -OAL_EBUSY;
    }

    pst_pci_dev = pcie_res_to_dev(pst_pcie_res);
    if (pst_pci_dev == NULL) {
        pci_print_log(PCI_LOG_ERR, "pst_pci_dev is null");
        return -OAL_ENODEV;
    }

    ret = oal_pcie_inbound_ca_to_va(pst_pcie_res, cpu_address, &addr_map);
    if (ret != OAL_SUCC) {
        oal_io_print("pcie_read32 0x%8x unmap, read failed!\n", cpu_address);
        return -OAL_EBUSY;
    }
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    oal_pci_cache_inv(pst_pci_dev, (void *)addr_map.pa, sizeof(value));
#endif
    value = oal_readl((void *)addr_map.va);
    oal_pcie_print_bits(&value, sizeof(uint32_t));
    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_debug_write32(struct kobject *dev, struct kobj_attribute *attr,
                                          const char *buf, oal_pcie_res *pst_pcie_res)
{
    /* echo write32 address value > debug */
    int32_t ret;
    pci_addr_map addr_map;
    uint32_t cpu_address = 0xFFFFFFFF;
    uint32_t value, old;
    oal_pci_dev_stru *pst_pci_dev = NULL;
    if ((sscanf_s(buf, "0x%x 0x%x", &cpu_address, &value) != 2)) {
        return -OAL_EINVAL;
    }

    if (pst_pcie_res == NULL) {
        pci_print_log(PCI_LOG_ERR, "pst_pcie_res is null");
        return -OAL_EBUSY;
    }

    pst_pci_dev = pcie_res_to_dev(pst_pcie_res);
    if (pst_pci_dev == NULL) {
        pci_print_log(PCI_LOG_ERR, "pst_pci_dev is null");
        return -OAL_ENODEV;
    }

    ret = oal_pcie_inbound_ca_to_va(pst_pcie_res, cpu_address, &addr_map);
    if (ret != OAL_SUCC) {
        oal_io_print("pcie_write32 0x%8x unmap, read failed!\n", cpu_address);
        return -OAL_ENODEV;
    }
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    oal_pci_cache_inv(pst_pci_dev, (void *)addr_map.pa, sizeof(value));
#endif
    old = oal_readl((void *)addr_map.va);
    oal_writel(value, (void *)addr_map.va);
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    oal_pci_cache_flush(pst_pci_dev, (void *)addr_map.pa, sizeof(value));
    oal_pci_cache_inv(pst_pci_dev, (void *)addr_map.pa, sizeof(value));
#endif
    oal_io_print("pcie_write32  change 0x%8x from 0x%8x to 0x%8x callback-read= 0x%8x\n",
                 cpu_address, old, value, oal_readl((void *)addr_map.va));
    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_debug_read_dsm32(struct kobject *dev, struct kobj_attribute *attr,
                                             const char *buf, oal_pcie_res *pst_pcie_res)
{
    int32_t ret;
    uint32_t val, type;
    if ((sscanf_s(buf, "%u", &type) != 1)) {
        pci_print_log(PCI_LOG_ERR, "host read_dsm32 argument invalid,[%s]", buf);
        return -OAL_EINVAL;
    }

    ret = oal_pcie_read_dsm32(pst_pcie_res, type, &val);
    if (ret == OAL_SUCC) {
        pci_print_log(PCI_LOG_INFO, "read_dsm32 type=%u, val=%u", type, val);
    }

    return ret;
}

OAL_STATIC int32_t oal_pcie_debug_write_dsm32(struct kobject *dev, struct kobj_attribute *attr,
                                              const char *buf, oal_pcie_res *pst_pcie_res)
{
    int32_t ret;
    uint32_t val, type;
    if ((sscanf_s(buf, "%u %u", &type, &val) != 2)) {
        pci_print_log(PCI_LOG_ERR, "host write_dsm32 argument invalid,[%s]", buf);
        return -OAL_EINVAL;
    }

    ret = oal_pcie_write_dsm32(pst_pcie_res, type, val);
    if (ret == OAL_SUCC) {
        pci_print_log(PCI_LOG_INFO, "write_dsm32 type=%u, val=%u", type, val);
    }

    return ret;
}

OAL_STATIC int32_t oal_pcie_debug_host_read32(struct kobject *dev, struct kobj_attribute *attr,
                                              const char *buf, oal_pcie_res *pst_pcie_res)
{
    /* echo host_read32 address > debug */
    unsigned long cpu_address, align_addr;
    void *pst_vaddr = NULL;
    int32_t value;

    if ((sscanf_s(buf, "0x%lx", &cpu_address) != 1)) {
        pci_print_log(PCI_LOG_ERR, "host read32 argument invalid,[%s]", buf);
        return -OAL_EINVAL;
    }

    align_addr = OAL_ROUND_DOWN(cpu_address, PAGE_SIZE);

    pst_vaddr = oal_ioremap_nocache(align_addr, PAGE_SIZE);
    if (pst_vaddr == OAL_SUCC) {
        oal_io_print("pcie_host_read32 0x%lx map failed!\n", align_addr);
        return -OAL_EBUSY;
    }

    value = oal_readl(pst_vaddr + (cpu_address - align_addr));
    oal_pcie_print_bits(&value, sizeof(uint32_t));

    oal_iounmap(pst_vaddr);
    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_debug_host_write32(struct kobject *dev, struct kobj_attribute *attr,
                                               const char *buf, oal_pcie_res *pst_pcie_res)
{
    /* echo host_write32 address value > debug */
    unsigned long cpu_address, align_addr;
    void *pst_vaddr = NULL;
    void *pst_align_addr = NULL;
    uint32_t old, value;

    if ((sscanf_s(buf, "0x%lx 0x%x", &cpu_address, &value) != 2)) {
        return -OAL_EINVAL;
    }

    align_addr = OAL_ROUND_DOWN(cpu_address, PAGE_SIZE);

    pst_vaddr = oal_ioremap_nocache(align_addr, PAGE_SIZE);
    if (pst_vaddr == OAL_SUCC) {
        oal_io_print("pcie_host_write32 0x%lx map failed!\n", align_addr);
        return -OAL_EBUSY;
    }

    pst_align_addr = pst_vaddr + (cpu_address - align_addr);

    old = oal_readl((void *)pst_align_addr);
    oal_writel(value, (void *)pst_align_addr);
    oal_io_print("host_write32  change 0x%lx from 0x%8x to 0x%8x callback-read= 0x%8x\n",
                 cpu_address, old, value, oal_readl((void *)pst_align_addr));
    oal_iounmap(pst_vaddr);
    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_debug_read16(struct kobject *dev, struct kobj_attribute *attr,
                                         const char *buf, oal_pcie_res *pst_pcie_res)
{
    /* echo read32 address > debug */
    int32_t ret;
    pci_addr_map addr_map;
    uint32_t cpu_address = 0xFFFFFFFF;
    uint32_t value;
    oal_pci_dev_stru *pst_pci_dev = NULL;

    if ((sscanf_s(buf, "0x%x", &cpu_address) != 1)) {
        return -OAL_EINVAL;
    }

    if (pst_pcie_res == NULL) {
        pci_print_log(PCI_LOG_ERR, "pst_pcie_res is null");
        return -OAL_EBUSY;
    }

    pst_pci_dev = pcie_res_to_dev(pst_pcie_res);
    if (pst_pci_dev == NULL) {
        pci_print_log(PCI_LOG_ERR, "pst_pci_dev is null");
        return -OAL_ENODEV;
    }

    ret = oal_pcie_inbound_ca_to_va(pst_pcie_res, cpu_address, &addr_map);
    if (ret != OAL_SUCC) {
        oal_io_print("pcie_read16 0x%8x unmap, read failed!\n", cpu_address);
        return -OAL_ENODEV;
    }
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    oal_pci_cache_inv(pst_pci_dev, (void *)addr_map.pa, sizeof(value));
#endif
    value = readw((void *)addr_map.va);
    oal_pcie_print_bits(&value, sizeof(uint16_t));
    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_debug_write16(struct kobject *dev, struct kobj_attribute *attr,
                                          const char *buf, oal_pcie_res *pst_pcie_res)
{
    /* echo write32 address value > debug */
    int32_t ret;
    pci_addr_map addr_map;
    uint32_t cpu_address = 0xFFFFFFFF;
    uint32_t value, old;
    oal_pci_dev_stru *pst_pci_dev = NULL;
    if ((sscanf_s(buf, "0x%x 0x%x", &cpu_address, &value) != 2)) {
        return -OAL_EINVAL;
    }

    if (pst_pcie_res == NULL) {
        pci_print_log(PCI_LOG_ERR, "pst_pcie_res is null");
        return -OAL_EBUSY;
    }

    pst_pci_dev = pcie_res_to_dev(pst_pcie_res);
    if (pst_pci_dev == NULL) {
        pci_print_log(PCI_LOG_ERR, "pst_pci_dev is null");
        return -OAL_ENODEV;
    }

    ret = oal_pcie_inbound_ca_to_va(pst_pcie_res, cpu_address, &addr_map);
    if (ret != OAL_SUCC) {
        oal_io_print("pcie_write16 0x%8x unmap, read failed!\n", cpu_address);
        return -OAL_ENODEV;
    }
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    oal_pci_cache_inv(pst_pci_dev, (void *)addr_map.pa, sizeof(value));
#endif
    old = readw((void *)addr_map.va);
    writew((uint16_t)value, (void *)addr_map.va);
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
    oal_pci_cache_flush(pst_pci_dev, (void *)addr_map.pa, sizeof(value));
    oal_pci_cache_inv(pst_pci_dev, (void *)addr_map.pa, sizeof(value));
#endif
    oal_io_print("pcie_write16  change 0x%8x from 0x%4x to 0x%4x callback-read= 0x%4x\n",
                 cpu_address, old, (uint16_t)value, readw((void *)addr_map.va));
    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_saveconfigmem(oal_pcie_res *pst_pcie_res, const char *file_name,
                                          uint32_t cpu_address, uint32_t length)
{
    struct file *fp = NULL;
    uint32_t index;
    int32_t ret = OAL_SUCC;
    mm_segment_t fs;
    void *pst_buf = NULL;

    oal_pci_dev_stru *pst_pci_dev = NULL;

    if (pst_pcie_res == NULL) {
        pci_print_log(PCI_LOG_ERR, "pst_pcie_res is null");
        return -OAL_EBUSY;
    }

    pst_pci_dev = pcie_res_to_dev(pst_pcie_res);
    if (pst_pci_dev == NULL) {
        pci_print_log(PCI_LOG_ERR, "pst_pci_dev is null");
        return -OAL_ENODEV;
    }

    if (length == 0) { /* 0 is invalid */
        pci_print_log(PCI_LOG_ERR, "input len is 0");
        return -OAL_EINVAL;
    }

    length = padding_m(length, 4); /* 计算4字节对齐后的长度，默认进位 */

    pst_buf = vmalloc(length);
    if (pst_buf == NULL) {
        pci_print_log(PCI_LOG_ERR, "savemem pst_buf is null, vmalloc size %u failed!", length);
        return -OAL_ENOMEM;
    }

    fs = get_fs();
    set_fs(KERNEL_DS);
    fp = filp_open(file_name, O_RDWR | O_CREAT, 0664);
    if (oal_is_err_or_null(fp)) {
        pci_print_log(PCI_LOG_ERR, "create file error,fp = 0x%p, filename is [%s]\n", fp, file_name);
        vfree(pst_buf);
        set_fs(fs);
        return -OAL_EINVAL;
    }

    pci_print_log(PCI_LOG_DBG, "savemem cpu:0x%8x len:%u save_path:%s", cpu_address, length, file_name);

    oal_reference(ret);

    for (index = 0; index < length; index += sizeof(uint32_t)) { /* 每次偏移4字节 */
        uint32_t reg = 0;
        ret = oal_pci_read_config_dword(pst_pci_dev, cpu_address + index, &reg);
        if (ret) {
            pci_print_log(PCI_LOG_ERR, "read 0x%x failed, ret=%d", cpu_address + index, ret);
            break;
        }

        oal_writel(reg, pst_buf + index);
    }

    if (index) {
        pci_print_log(PCI_LOG_INFO, "savemem cpu:0x%8x len:%u save_path:%s done", cpu_address, index, file_name);
        ret = oal_kernel_file_write(fp, pst_buf, length);
        if (ret < 0) {
            pci_print_log(PCI_LOG_ERR, "vfs write failed!");
        }
    } else {
        pci_print_log(PCI_LOG_WARN, "savemem cpu:0x%8x len:%u save_path:%s failed!",
                      cpu_address, length, file_name);
        ret = -OAL_EINVAL;
    }

#if (KERNEL_VERSION(2, 6, 35) <= LINUX_VERSION_CODE)
    vfs_fsync(fp, 0);
#else
    vfs_fsync(fp, fp->f_path.dentry, 0);
#endif

    filp_close(fp, NULL);
    set_fs(fs);
    vfree(pst_buf);
    return ret;
}

OAL_STATIC int32_t oal_pcie_savemem(oal_pcie_res *pst_pcie_res, const char *file_name,
                                    uint32_t cpu_address, uint32_t length)
{
    struct file *fp = NULL;
    uint32_t index, value;
    int32_t ret = OAL_SUCC;
    pci_addr_map addr_map;
    mm_segment_t fs;
    void *pst_buf = NULL;

    oal_pci_dev_stru *pst_pci_dev = NULL;

    if (pst_pcie_res == NULL) {
        pci_print_log(PCI_LOG_ERR, "pst_pcie_res is null");
        return -OAL_EBUSY;
    }

    pst_pci_dev = pcie_res_to_dev(pst_pcie_res);
    if (pst_pci_dev == NULL) {
        pci_print_log(PCI_LOG_ERR, "pst_pci_dev is null");
        return -OAL_ENODEV;
    }

    if (length == 0) { /* 0 is invalid */
        pci_print_log(PCI_LOG_ERR, "input len is 0");
        return -OAL_EINVAL;
    }

    length = padding_m(length, 4); /* 计算4字节对齐后的长度，默认进位 */

    pst_buf = vmalloc(length);
    if (pst_buf == NULL) {
        pci_print_log(PCI_LOG_ERR, "savemem pst_buf is null, vmalloc size %u failed!", length);
        return -OAL_ENOMEM;
    }

    fs = get_fs();
    set_fs(KERNEL_DS);
    fp = filp_open(file_name, O_RDWR | O_CREAT, 0664);
    if (oal_is_err_or_null(fp)) {
        pci_print_log(PCI_LOG_ERR, "create file error,fp = 0x%p, filename is [%s]\n", fp, file_name);
        vfree(pst_buf);
        set_fs(fs);
        return -OAL_EINVAL;
    }

    for (index = 0; index < length; index += sizeof(uint32_t)) { /* 每次偏移4字节 */
        ret = oal_pcie_inbound_ca_to_va(pst_pcie_res, cpu_address + index, &addr_map);
        if (ret != OAL_SUCC) {
            pci_print_log(PCI_LOG_ERR, "savemem address 0x%8x invalid", cpu_address + index);
            break;
        }
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
        oal_pci_cache_inv(pst_pci_dev, (void *)addr_map.pa, sizeof(value));
#endif
        /* 这里有可能保存的是寄存器区域，按4字节对齐访问 */
        value = oal_readl((void *)addr_map.va);
        oal_writel(value, pst_buf + index);
    }

    if (index) {
        pci_print_log(PCI_LOG_INFO, "savemem cpu:0x%8x len:%u save_path:%s done", cpu_address, index, file_name);
        ret = oal_kernel_file_write(fp, pst_buf, length);
        if (ret < 0) {
            pci_print_log(PCI_LOG_ERR, "vfs write failed!");
        }
    } else {
        pci_print_log(PCI_LOG_WARN, "savemem cpu:0x%8x len:%u save_path:%s failed!",
                      cpu_address, length, file_name);
        ret = -OAL_EINVAL;
    }

#if (KERNEL_VERSION(2, 6, 35) <= LINUX_VERSION_CODE)
    vfs_fsync(fp, 0);
#else
    vfs_fsync(fp, fp->f_path.dentry, 0);
#endif

    filp_close(fp, NULL);
    set_fs(fs);
    vfree(pst_buf);
    return ret;
}

OAL_STATIC int32_t oal_pcie_save_hostmem(oal_pcie_res *pst_pcie_res, const char *file_name,
                                         uintptr_t host_address, uint32_t length)
{
    struct file *fp = NULL;
    uint32_t index, value;
    int32_t ret = OAL_SUCC;
    mm_segment_t fs;
    void *pst_buf = NULL;
    void *vaddr = NULL;

    oal_pci_dev_stru *pst_pci_dev = NULL;

    if (pst_pcie_res == NULL) {
        pci_print_log(PCI_LOG_ERR, "pst_pcie_res is null");
        return -OAL_EBUSY;
    }

    pst_pci_dev = pcie_res_to_dev(pst_pcie_res);
    if (pst_pci_dev == NULL) {
        pci_print_log(PCI_LOG_ERR, "pst_pci_dev is null");
        return -OAL_ENODEV;
    }

    if (length == 0) { /* 0 is invalid */
        pci_print_log(PCI_LOG_ERR, "input len is 0");
        return -OAL_EINVAL;
    }

    length = padding_m(length, 4); /* 计算4字节对齐后的长度，默认进位 */

    vaddr = oal_ioremap_nocache(host_address, length);
    if (vaddr == NULL) {
        pci_print_log(PCI_LOG_ERR, "ioremap %lx , len:%u ,fail", host_address, length);
        return -OAL_ENOMEM;
    }

    pst_buf = vmalloc(length);
    if (pst_buf == NULL) {
        pci_print_log(PCI_LOG_ERR, "savemem pst_buf is null, vmalloc size %u failed!", length);
        return -OAL_ENOMEM;
    }

    fs = get_fs();
    set_fs(KERNEL_DS);
    fp = filp_open(file_name, O_RDWR | O_CREAT, 0664);
    if (oal_is_err_or_null(fp)) {
        pci_print_log(PCI_LOG_ERR, "create file error,fp = 0x%p, filename is [%s]\n", fp, file_name);
        set_fs(fs);
        vfree(pst_buf);
        oal_iounmap(vaddr);
        return -OAL_EINVAL;
    }

    for (index = 0; index < length; index += sizeof(uint32_t)) {
        /* 这里有可能保存的是寄存器区域，按4字节对齐访问 */
        value = oal_readl(vaddr + index);
        oal_writel(value, pst_buf + index);
    }

    if (index) {
        pci_print_log(PCI_LOG_INFO, "save_hostmem cpu:0x%lx len:%u save_path:%s done",
                      host_address, index, file_name);
        ret = oal_kernel_file_write(fp, pst_buf, length);
        if (ret < 0) {
            pci_print_log(PCI_LOG_ERR, "vfs write failed!");
        }
    } else {
        pci_print_log(PCI_LOG_WARN, "save_hostmem cpu:0x%lx len:%u save_path:%s failed!",
                      host_address, length, file_name);
        ret = -OAL_EINVAL;
    }

#if (KERNEL_VERSION(2, 6, 35) <= LINUX_VERSION_CODE)
    vfs_fsync(fp, 0);
#else
    vfs_fsync(fp, fp->f_path.dentry, 0);
#endif

    filp_close(fp, NULL);
    set_fs(fs);
    vfree(pst_buf);
    oal_iounmap(vaddr);
    return ret;
}

OAL_STATIC int32_t oal_pcie_debug_savemem(struct kobject *dev, struct kobj_attribute *attr,
                                          const char *buf, oal_pcie_res *pst_pcie_res)
{
    char file_name[PCIE_DEBUG_MSG_LEN];
    uint32_t cpu_address, length;
    int32_t ret;

    if (strlen(buf) >= OAL_SIZEOF(file_name)) {
        oal_io_print("input illegal!%s\n", __FUNCTION__);
        return -OAL_EINVAL;
    }

    if ((sscanf_s(buf, "0x%x %u %s", &cpu_address, &length, file_name, sizeof(file_name)) != 3)) {
        return -OAL_EINVAL;
    }

    ret = oal_pcie_savemem(pst_pcie_res, file_name, cpu_address, length);
    return ret;
}

OAL_STATIC int32_t oal_pcie_debug_saveconfigmem(struct kobject *dev, struct kobj_attribute *attr,
                                                const char *buf, oal_pcie_res *pst_pcie_res)
{
    char file_name[PCIE_DEBUG_MSG_LEN];
    uint32_t cpu_address, length;
    int32_t ret;

    if (strlen(buf) >= OAL_SIZEOF(file_name)) {
        oal_io_print("input illegal!%s\n", __FUNCTION__);
        return -OAL_EINVAL;
    }

    if ((sscanf_s(buf, "0x%x %u %s", &cpu_address, &length, file_name, sizeof(file_name)) != 3)) {
        return -OAL_EINVAL;
    }

    ret = oal_pcie_saveconfigmem(pst_pcie_res, file_name, cpu_address, length);
    return ret;
}

OAL_STATIC int32_t oal_pcie_debug_save_hostmem(struct kobject *dev, struct kobj_attribute *attr,
                                               const char *buf, oal_pcie_res *pst_pcie_res)
{
    char file_name[PCIE_DEBUG_MSG_LEN];
    uintptr_t host_address;
    uint32_t length;
    int32_t ret;

    if (strlen(buf) >= OAL_SIZEOF(file_name)) {
        oal_io_print("input illegal!%s\n", __FUNCTION__);
        return -OAL_EINVAL;
    }

    if ((sscanf_s(buf, "0x%lx %u %s", &host_address, &length, file_name, sizeof(file_name)) != 3)) {
        return -OAL_EINVAL;
    }

    ret = oal_pcie_save_hostmem(pst_pcie_res, file_name, host_address, length);
    return ret;
}

int32_t oal_pcie_print_device_mem(oal_pcie_res *pst_pcie_res, uint32_t cpu_address, uint32_t length)
{
    /* echo readmem address length(hex) > debug */
    uint32_t index;
    int32_t ret;
    pci_addr_map addr_map;
    void *print_buf = NULL;
    oal_pci_dev_stru *pst_pci_dev = NULL;

    if (pst_pcie_res == NULL) {
        pci_print_log(PCI_LOG_ERR, "pst_pcie_res is null");
        return -OAL_EBUSY;
    }

    pst_pci_dev = pcie_res_to_dev(pst_pcie_res);
    if (pst_pci_dev == NULL) {
        pci_print_log(PCI_LOG_ERR, "pst_pci_dev is null");
        return -OAL_ENODEV;
    }

    if (length == 0) { /* 0 is invalid */
        pci_print_log(PCI_LOG_ERR, "input len is 0");
        return -OAL_EINVAL;
    }

    length = padding_m(length, 4); /* 计算4字节对齐后的长度，默认进位 */
    print_buf = vmalloc(length);
    if (print_buf == NULL) {
        return -OAL_EINVAL;
    }

    for (index = 0; index < length; index += sizeof(uint32_t)) { /* 每次偏移4字节 */
        ret = oal_pcie_inbound_ca_to_va(pst_pcie_res, cpu_address + index, &addr_map);
        if (ret != OAL_SUCC) {
            pci_print_log(PCI_LOG_ERR, "readmem address 0x%8x invalid", cpu_address + index);
            break;
        }
        *(uint32_t *)(print_buf + index) = oal_readl((void *)addr_map.va);
    }

    if (index) {
        pci_print_log(PCI_LOG_INFO, "readmem cpu:0x%8x len:%u, va:0x%lx, pa:0x%lx  done",
                      cpu_address, index, addr_map.va, addr_map.pa);
        print_hex_dump(KERN_INFO, "readmem: ", DUMP_PREFIX_OFFSET, 16, 4,
                       print_buf, length, true); /* 内核函数固定的传参 */
    } else {
        pci_print_log(PCI_LOG_WARN, "readmem cpu:0x%8x len:%u  failed!", cpu_address, length);
        vfree(print_buf);
        return -OAL_EINVAL;
    }

    vfree(print_buf);
    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_debug_readmem(struct kobject *dev, struct kobj_attribute *attr,
                                          const char *buf, oal_pcie_res *pst_pcie_res)
{
    /* echo readmem address length(hex) > debug */
    uint32_t cpu_address, length;

    if ((sscanf_s(buf, "0x%x %u", &cpu_address, &length) != 2)) {
        return -OAL_EINVAL;
    }

    (void)oal_pcie_print_device_mem(pst_pcie_res, cpu_address, length);

    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_debug_readmem_config(struct kobject *dev, struct kobj_attribute *attr,
                                                 const char *buf, oal_pcie_res *pst_pcie_res)
{
    /* echo readmem address length(hex) > debug */
    uint32_t cpu_address, length, index;
    int32_t ret;
    uint32_t reg = 0;
    void *print_buf = NULL;
    oal_pci_dev_stru *pst_pci_dev = NULL;

    if (pst_pcie_res == NULL) {
        pci_print_log(PCI_LOG_ERR, "pst_pcie_res is null");
        return -OAL_EBUSY;
    }

    pst_pci_dev = pcie_res_to_dev(pst_pcie_res);
    if (pst_pci_dev == NULL) {
        pci_print_log(PCI_LOG_ERR, "pst_pci_dev is null");
        return -OAL_ENODEV;
    }

    if ((sscanf_s(buf, "0x%x %u", &cpu_address, &length) != 2)) {
        return -OAL_EINVAL;
    }

    if (length == 0) { /* 0 is invalid */
        pci_print_log(PCI_LOG_ERR, "input len is 0");
        return -OAL_EINVAL;
    }

    length = padding_m(length, 4); /* 计算4字节对齐后的长度，默认进位 */
    print_buf = vmalloc(length);
    if (print_buf == NULL) {
        return -OAL_EINVAL;
    }

    for (index = 0; index < length; index += sizeof(uint32_t)) { /* 每次偏移4字节 */
        ret = oal_pci_read_config_dword(pst_pci_dev, cpu_address + index, &reg);
        if (ret) {
            pci_print_log(PCI_LOG_ERR, "read 0x%x failed, ret=%d", cpu_address + index, ret);
            break;
        }
        *(uint32_t *)(print_buf + index) = reg;
    }

    if (index) {
        pci_print_log(PCI_LOG_INFO, "readmem cpu:0x%8x len:%u  done", cpu_address, index);
        oal_print_hex_dump(print_buf, length, HEX_DUMP_GROUP_SIZE, "readmem: ");
    } else {
        pci_print_log(PCI_LOG_WARN, "readmem cpu:0x%8x len:%u  failed!", cpu_address, length);
        vfree(print_buf);
        return -OAL_EINVAL;
    }

    vfree(print_buf);
    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_send_test_pkt(int32_t num)
{
    uint32_t cmd_len = 1500;
    oal_netbuf_stru *pst_netbuf = NULL;
    struct hcc_transfer_param st_hcc_transfer_param = {0};
    struct hcc_handler *hcc = hcc_get_110x_handler();
    if (hcc == NULL) {
        return -OAL_EFAIL;
    }

    if (num == 0) {
        return -OAL_EINVAL;
    }

    do {
        pst_netbuf = hcc_netbuf_alloc(cmd_len);
        if (pst_netbuf == NULL) {
            oal_io_print("hwifi alloc skb fail.\n");
            return -OAL_EFAIL;
        }

        memset_s(oal_netbuf_put(pst_netbuf, cmd_len), cmd_len, 0x5a, cmd_len);

        hcc_hdr_param_init(&st_hcc_transfer_param,
                           HCC_ACTION_TYPE_TEST,
                           HCC_TEST_SUBTYPE_CMD,
                           0,
                           HCC_FC_WAIT,
                           0);
        if (OAL_SUCC != hcc_tx(hcc, pst_netbuf, &st_hcc_transfer_param)) {
            oal_netbuf_free(pst_netbuf);
            oal_io_print("hcc tx failed\n");
            return -OAL_EFAIL;
        }
    } while (--num);

    return OAL_SUCC;
}

OAL_STATIC void oal_pcie_outbound_test_init(oal_pcie_res *pst_pcie_res, oal_pci_dev_stru *pst_pci_dev,
                                            uint32_t outbound_size, dma_addr_t outbound_dma_addr)
{
    if (pst_pcie_res->chip_info.edma_support == OAL_TRUE) {
        void *inbound_addr = pst_pcie_res->st_iatu_bar.st_region.vaddr;
        if (pst_pcie_res->revision == PCIE_REVISION_4_70A) {
            oal_pci_write_config_dword(pst_pci_dev, 0x700 + 0x200, 0x1);        /* view index */
            oal_pci_write_config_dword(pst_pci_dev, 0x700 + 0x204, 0x0);        /* ctrl 1 */
            oal_pci_write_config_dword(pst_pci_dev, 0x700 + 0x208, 0x80000000); /* ctrl 2 */
            oal_pci_write_config_dword(pst_pci_dev, 0x700 + 0x20c, 0x80000000); /* base lower */
            oal_pci_write_config_dword(pst_pci_dev, 0x700 + 0x210, 0);          /* base upper */
            oal_pci_write_config_dword(pst_pci_dev, 0x700 + 0x214,
                                       0x70000000 + outbound_size - 1); /* limit */
            oal_pci_write_config_dword(pst_pci_dev, 0x700 + 0x218,
                                       (uint32_t)outbound_dma_addr); /* target lower */
            oal_pci_write_config_dword(pst_pci_dev, 0x700 + 0x21c, 0); /* target upper */
        } else if (pst_pcie_res->chip_info.membar_support == OAL_TRUE) {
            IATU_REGION_CTRL_2_OFF ctr2;
            uint32_t index = 0;
            if (inbound_addr == NULL) {
                pci_print_log(PCI_LOG_ERR, "inbound_addr is null");
                return;
            }
            pci_print_log(PCI_LOG_INFO, "outbound config");
            oal_writel(0x0,
                       inbound_addr +
                       hi_pci_iatu_region_ctrl_1_off_outbound_i(hi_pci_iatu_outbound_base_off(index)));
            ctr2.AsDword = 0;
            ctr2.bits.region_en = 1;
            ctr2.bits.bar_num = 0x0;
            oal_writel(ctr2.AsDword,
                       inbound_addr +
                       hi_pci_iatu_region_ctrl_2_off_outbound_i(hi_pci_iatu_outbound_base_off(index)));

            oal_writel(0x80000000,
                       inbound_addr +
                       hi_pci_iatu_lwr_base_addr_off_outbound_i(hi_pci_iatu_outbound_base_off(index)));
            oal_writel(0x0,
                       inbound_addr +
                       hi_pci_iatu_upper_base_addr_off_outbound_i(hi_pci_iatu_outbound_base_off(index)));
            oal_writel(0x80000000 + outbound_size - 1,
                       inbound_addr +
                       hi_pci_iatu_limit_addr_off_outbound_i(hi_pci_iatu_outbound_base_off(index)));

            oal_writel((uint32_t)outbound_dma_addr,
                       inbound_addr +
                       hi_pci_iatu_lwr_target_addr_off_outbound_i(hi_pci_iatu_outbound_base_off(index)));
            oal_writel((uint32_t)(outbound_dma_addr >> 32),
                       inbound_addr +
                       hi_pci_iatu_upper_target_addr_off_outbound_i(hi_pci_iatu_outbound_base_off(index)));
        } else {
            pci_print_log(PCI_LOG_ERR, "unkown pcie ip revision :0x%x", pst_pcie_res->revision);
        }
    }

    if (pst_pcie_res->chip_info.ete_support == OAL_TRUE) {
        int32_t ret = OAL_SUCC;
        uint64_t host_iova = 0;
        uint64_t devva = 0;
        ret = pcie_if_hostca_to_devva(0, (uint64_t)outbound_dma_addr, &devva);
        if (ret == OAL_SUCC) {
            oal_print_hi11xx_log(HI11XX_LOG_INFO, "pcie_if_hostca_to_devva iova=0x%llx -> devva=0x%llx",
                                 (uint64_t)outbound_dma_addr, devva);
            ret = pcie_if_devva_to_hostca(0, devva, &host_iova);
            if (ret == OAL_SUCC) {
                oal_print_hi11xx_log(HI11XX_LOG_INFO, "pcie_if_devva_to_hostca devva=0x%llx -> iova=0x%llx",
                                     devva, host_iova);
            } else {
                oal_print_hi11xx_log(HI11XX_LOG_ERR, "pcie_if_devva_to_hostca failed devva=0x%llx", devva);
            }
        }
    }
}

/*
 * 测试outbound是否生效，返回DDR地址，
 * 通过SSI或者WCPU 读写Device 侧PCIe Slave地址 查看DDR是否有改变,
 * 1103 Slave 空间为256M
 */
OAL_STATIC int32_t oal_pcie_outbound_test(oal_pcie_res *pst_pcie_res, const char *buf)
{
    OAL_STATIC dma_addr_t outbound_dma_addr = 0;
    uint32_t outbound_size = 4096;
    OAL_STATIC oal_uint *outbound_vaddr = NULL;
    char casename[PCIE_DEBUG_MSG_LEN] = {0};
    oal_pci_dev_stru *pst_pci_dev;

    pst_pci_dev = pcie_res_to_dev(pst_pcie_res);
    if (pst_pci_dev == NULL) {
        pci_print_log(PCI_LOG_ERR, "pst_pci_dev is null");
        return -OAL_ENODEV;
    }

    if (strlen(buf) >= OAL_SIZEOF(casename)) {
        oal_io_print("input illegal!%s\n", __FUNCTION__);
        return -OAL_EINVAL;
    }

    /* Just for debug */
    if ((sscanf_s(buf, "%s", casename, sizeof(casename)) != 1)) {
        pci_print_log(PCI_LOG_ERR, "invalid agrument");
        return -OAL_EINVAL;
    } else {
        /* dump */
        if (outbound_vaddr) {
            oal_print_hex_dump((uint8_t *)outbound_vaddr, outbound_size, HEX_DUMP_GROUP_SIZE, "outbound: ");
        }
    }
    pci_print_log(PCI_LOG_INFO, "outbound:[%s]", casename);
    if (!oal_strcmp(casename, "rebound")) {
        if (outbound_vaddr) {
            dma_free_coherent(&pst_pci_dev->dev, outbound_size, outbound_vaddr, outbound_dma_addr);
        }
        outbound_vaddr = dma_alloc_coherent(&pst_pci_dev->dev, outbound_size,
                                            &outbound_dma_addr,
                                            GFP_KERNEL);
        if (outbound_vaddr) {
            memset_s(outbound_vaddr, outbound_size, 0x38, outbound_size);
            /* set outbound */
            oal_io_print("host dma addr:0x%lx , vaddr: 0x%p\n", (uintptr_t)outbound_dma_addr, outbound_vaddr);
            oal_pcie_outbound_test_init(pst_pcie_res, pst_pci_dev, outbound_size, outbound_dma_addr);
        }
    }

    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_addr_switch_test(oal_pcie_res *pst_pcie_res, const char *buf)
{
    int32_t ret = OAL_SUCC;
    if (pst_pcie_res->chip_info.addr_info.glb_ctrl != 0) {
        uint64_t host_va = 0;
        uint64_t dev_cpuaddr = 0;
        ret = oal_pcie_devca_to_hostva(0, pst_pcie_res->chip_info.addr_info.glb_ctrl, &host_va);
        if (ret == OAL_SUCC) {
            oal_print_hi11xx_log(HI11XX_LOG_INFO, "oal_pcie_devca_to_hostva 0x%x -> 0x%llx, value=0x%x",
                                 pst_pcie_res->chip_info.addr_info.glb_ctrl, host_va,
                                 oal_readl((void*)(uintptr_t)host_va));
            /* master hostva->devca */
            ret = oal_pcie_get_dev_ca(0, (void*)(uintptr_t)host_va, &dev_cpuaddr);
            if (ret == OAL_SUCC) {
                oal_print_hi11xx_log(HI11XX_LOG_INFO, "oal_pcie_get_dev_ca 0x%llx -> 0x%x",
                                     host_va, (uint32_t)dev_cpuaddr);
            } else {
                oal_print_hi11xx_log(HI11XX_LOG_ERR, "oal_pcie_get_dev_ca 0x%llx  convert failed",
                                     host_va);
            }
        } else {
            oal_print_hi11xx_log(HI11XX_LOG_ERR, "oal_pcie_devca_to_hostva 0x%x  convert failed",
                                 pst_pcie_res->chip_info.addr_info.glb_ctrl);
        }
    } else {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "glb_ctrl is 0");
    }

    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_device_memcheck(oal_pcie_res *pst_pcie_res, uint32_t cpuaddr,
                                            uint32_t size, uint32_t data)
{
    volatile unsigned int data_rd;
    volatile unsigned int data_wt;
    unsigned int i = 0;
    unsigned int mode = 0;
    int32_t ret;
    pci_addr_map addr_map;
    oal_pci_dev_stru *pst_pci_dev;

    pst_pci_dev = pcie_res_to_dev(pst_pcie_res);
    if (pst_pci_dev == NULL) {
        pci_print_log(PCI_LOG_ERR, "pst_pci_dev is null");
        return -OAL_ENODEV;
    }

    if (cpuaddr & (0x3)) {
        pci_print_log(PCI_LOG_ERR, "invalid cpu address, must align to 4 bytes:%u", cpuaddr);
        return -OAL_EINVAL;
    }

    if (size & 0x3) {
        pci_print_log(PCI_LOG_ERR, "invalid size, must align to 4 bytes:%u", size);
        return -OAL_EINVAL;
    }

    ret = oal_pcie_inbound_ca_to_va(pst_pcie_res, cpuaddr + size - 1, &addr_map);
    if (ret != OAL_SUCC) {
        pci_print_log(PCI_LOG_ERR, "unmap device address 0x%x, size:%u", cpuaddr, size);
        return -OAL_EINVAL;
    }

    ret = oal_pcie_inbound_ca_to_va(pst_pcie_res, cpuaddr, &addr_map);
    if (ret != OAL_SUCC) {
        pci_print_log(PCI_LOG_ERR, "unmap device address 0x%x", cpuaddr);
        return -OAL_EINVAL;
    }

    for (i = 0, mode = 0; i < (size); i += sizeof(uint32_t), mode++) { /* 每次偏移4字节，直到超过size */
        if (mode % 4 < 2) { /* 两块4字节地址写data，两块4字节地址写~data，测试mem用 */
            data_wt = data;
        } else {
            data_wt = ~data;
        }

        oal_writel(data_wt, (void *)(addr_map.va + i));
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
        oal_pci_cache_flush(pst_pci_dev, (void *)addr_map.pa + i, sizeof(uint32_t));
        oal_pci_cache_inv(pst_pci_dev, (void *)addr_map.pa + i, sizeof(uint32_t));
#endif
        data_rd = oal_readl((void *)(addr_map.va + i));
        if (data_rd != data_wt) {
            pci_print_log(PCI_LOG_ERR, "bad address :0x%8x ,write: 0x%8x ,read: 0x%8x",
                          cpuaddr + i, data_wt, data_rd);
            return -OAL_EFAIL;
        }
    }
    return OAL_SUCC;
}

typedef struct _memcheck_item_ {
    uint32_t address; /* device cpu address */
    uint32_t size;    /* device cpu address */
} memcheck_item;

/* 需要WCPU代码在bootloader 阶段 */
OAL_STATIC int32_t oal_pcie_device_memcheck_auto(oal_pcie_res *pst_pcie_res)
{
    int32_t ret, i;
    memcheck_item test_address[] = {
        { 0x00004000, 544 * 1024 }, /* 544KB */
        { 0x20001c00, 409 * 1024 }, /* 409KB */
        { 0x60000000, 576 * 1024 }  /* 576KB */
    };

    pci_print_log(PCI_LOG_INFO, "memcheck start...");
    for (i = 0; i < sizeof(test_address) / sizeof(test_address[0]); i++) {
        pci_print_log(PCI_LOG_INFO, "Test address: 0x%8x ,size: 0x%8x start.",
                      test_address[i].address, test_address[i].size);
        ret = oal_pcie_device_memcheck(pst_pcie_res, test_address[i].address, test_address[i].size, 0xFFFFFFFF);
        if (ret != OAL_SUCC) {
            break;
        }

        ret = oal_pcie_device_memcheck(pst_pcie_res, test_address[i].address, test_address[i].size, 0x00000000);
        if (ret != OAL_SUCC) {
            break;
        }

        ret = oal_pcie_device_memcheck(pst_pcie_res, test_address[i].address, test_address[i].size, 0x5a5a5a5a);
        if (ret != OAL_SUCC) {
            break;
        }

        ret = oal_pcie_device_memcheck(pst_pcie_res, test_address[i].address, test_address[i].size, 0xa5a5a5a5);
        if (ret != OAL_SUCC) {
            break;
        }
        pci_print_log(PCI_LOG_INFO, "Test address: 0x%8x ,size: 0x%8x OK.",
                      test_address[i].address, test_address[i].size);
    }

    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_debug_wlan_power_on(struct kobject *dev, struct kobj_attribute *attr,
                                                const char *buf, oal_pcie_res *pst_pcie_res)
{
    oal_pci_dev_stru *pst_pci_dev = NULL;

    if (pst_pcie_res == NULL) {
        pci_print_log(PCI_LOG_ERR, "pst_pcie_res is null");
        return -OAL_EBUSY;
    }

    pst_pci_dev = pcie_res_to_dev(pst_pcie_res);
    if (pst_pci_dev == NULL) {
        pci_print_log(PCI_LOG_ERR, "pst_pci_dev is null");
        return -OAL_ENODEV;
    }
    wlan_power_on();
    return OAL_SUCC;
}

#ifdef CONFIG_ARCH_KIRIN_PCIE
OAL_STATIC int32_t oal_pcie_resume_handler(void *data)
{
    oal_reference(data);
    /* 这里保证解复位EP控制器时efuse已经稳定 */
    board_host_wakeup_dev_set(GPIO_HIGHLEVEL);
    oal_msleep(25); /* 这里要用GPIO 做ACK 延迟不可靠, MPW2 硬件唤醒15ms,软件6ms */
    pci_print_log(PCI_LOG_DBG, "oal_pcie_resume_handler, pull up gpio");
    return 0;
}
#endif

OAL_STATIC int32_t oal_pcie_testcase_transfer_res_init(oal_pcie_res *pst_pcie_res,
                                                       oal_pci_dev_stru *pst_pci_dev,
                                                       const char *buf)
{
    int32_t ret;

    ret = oal_pcie_transfer_res_init(pst_pcie_res);
    if (ret == OAL_SUCC) {
        pci_print_log(PCI_LOG_INFO, "oal_pcie_transfer_res_init:SUCC");
    } else {
        pci_print_log(PCI_LOG_ERR, "oal_pcie_transfer_res_init:FAIL");
    }
    return ret;
}

OAL_STATIC int32_t oal_pcie_testcase_send_test_pkt(oal_pcie_res *pst_pcie_res,
                                                   oal_pci_dev_stru *pst_pci_dev,
                                                   const char *buf)
{
    oal_pcie_send_test_pkt(1); /* 表示测试循环次数为1 */
    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_testcase_send_test_pkt2(oal_pcie_res *pst_pcie_res,
                                                    oal_pci_dev_stru *pst_pci_dev,
                                                    const char *buf)
{
    oal_pcie_send_test_pkt(2); /* 表示测试循环次数为2 */
    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_testcase_send_test_pkt3(oal_pcie_res *pst_pcie_res,
                                                    oal_pci_dev_stru *pst_pci_dev,
                                                    const char *buf)
{
    oal_pcie_send_test_pkt(3); /* 表示测试循环次数为3 */
    return OAL_SUCC;
}

#ifdef _PRE_PLAT_FEATURE_HI110X_PCIE_PERFORMANCE
OAL_STATIC int32_t oal_pcie_testcase_pcie_mips_show(oal_pcie_res *pst_pcie_res,
                                                    oal_pci_dev_stru *pst_pci_dev,
                                                    const char *buf)
{
    oal_pcie_mips_show();
    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_testcase_pcie_mips_clear(oal_pcie_res *pst_pcie_res,
                                                     oal_pci_dev_stru *pst_pci_dev,
                                                     const char *buf)
{
    oal_pcie_mips_clear();
    return OAL_SUCC;
}
#endif

OAL_STATIC int32_t oal_pcie_testcase_d2h_doorbell(oal_pcie_res *pst_pcie_res,
                                                  oal_pci_dev_stru *pst_pci_dev,
                                                  const char *buf)
{
    oal_pcie_d2h_doorbell(pst_pcie_res);
    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_testcase_h2d_doorbell(oal_pcie_res *pst_pcie_res,
                                                  oal_pci_dev_stru *pst_pci_dev,
                                                  const char *buf)
{
    oal_pcie_h2d_doorbell(pst_pcie_res);
    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_testcase_outbound_test(oal_pcie_res *pst_pcie_res,
                                                   oal_pci_dev_stru *pst_pci_dev,
                                                   const char *buf)
{
    buf = buf + OAL_STRLEN("outbound_test");
    for (; *buf == ' '; buf++);
    oal_pcie_outbound_test(pst_pcie_res, buf);
    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_testcase_addr_switch_test(oal_pcie_res *pst_pcie_res,
                                                      oal_pci_dev_stru *pst_pci_dev,
                                                      const char *buf)
{
    buf = buf + OAL_STRLEN("addr_switch_test");
    for (; *buf == ' '; buf++);
    oal_pcie_addr_switch_test(pst_pcie_res, buf);
    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_testcase_ram_memcheck(oal_pcie_res *pst_pcie_res,
                                                  oal_pci_dev_stru *pst_pci_dev,
                                                  const char *buf)
{
    /* 遍历itcm,dtcm,pktmem,扫内存 */
    oal_pcie_device_memcheck_auto(pst_pcie_res);
    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_testcase_pme_enable(oal_pcie_res *pst_pcie_res,
                                                oal_pci_dev_stru *pst_pci_dev,
                                                const char *buf)
{
    int32_t  ret;
    uint32_t pm;
    uint32_t reg = 0;
    /* Enable PME */
    pm = pci_find_capability(pst_pci_dev, PCI_CAP_ID_PM);
    if (!pm) {
        pci_print_log(PCI_LOG_ERR, "can't get PCI_CAP_ID_PM");
        return OAL_SUCC;
    }
    pci_print_log(PCI_LOG_INFO, "PME OFF : %u", pm);
    ret = oal_pci_read_config_dword(pst_pci_dev, pm + PCI_PM_CTRL, &reg);
    if (ret) {
        pci_print_log(PCI_LOG_ERR, "read %u failed", pm + PCI_PM_CTRL);
        return OAL_SUCC;
    }

    oal_io_print("read %u value:\n", pm + PCI_PM_CTRL);
    oal_pcie_print_bits(&reg, sizeof(reg));

    reg |= 0x100;

    ret = oal_pci_write_config_dword(pst_pci_dev, pm + PCI_PM_CTRL, reg);
    if (ret) {
        pci_print_log(PCI_LOG_ERR, "write %u failed", pm + PCI_PM_CTRL);
    } else {
        pci_print_log(PCI_LOG_INFO, "write %u ok", pm + PCI_PM_CTRL);
    }
    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_testcase_ringbuf_stat(oal_pcie_res *pst_pcie_res,
                                                  oal_pci_dev_stru *pst_pci_dev,
                                                  const char *buf)
{
    int32_t i;
    uint32_t freecount, usedcount;
    oal_io_print("h2d ringbuf info:\n");
    for (i = 0; i < PCIE_H2D_QTYPE_BUTT; i++) {
        freecount = oal_pcie_h2d_ringbuf_freecount(pst_pcie_res, (PCIE_H2D_RINGBUF_QTYPE)i, OAL_TRUE);
        usedcount = pcie_ringbuf_len(&pst_pcie_res->st_ringbuf.st_h2d_buf[i]);
        oal_io_print("qtype:%d , freecount:%u, used_count:%u \n",
                     i, freecount, usedcount);
    }
    oal_io_print("\nd2h ringbuf info:\n");
    freecount = oal_pcie_d2h_ringbuf_freecount(pst_pcie_res, OAL_TRUE);
    usedcount = pcie_ringbuf_len(&pst_pcie_res->st_ringbuf.st_d2h_buf);
    oal_io_print("freecount:%u, used_count:%u \n",
                 freecount, usedcount);
    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_testcase_clear_trans_info(oal_pcie_res *pst_pcie_res,
                                                      oal_pci_dev_stru *pst_pci_dev,
                                                      const char *buf)
{
    memset_s((void *)pst_pcie_res->st_tx_res[PCIE_H2D_QTYPE_NORMAL].stat.tx_burst_cnt,
             sizeof(pst_pcie_res->st_tx_res[PCIE_H2D_QTYPE_NORMAL].stat.tx_burst_cnt),
             0, sizeof(pst_pcie_res->st_tx_res[PCIE_H2D_QTYPE_NORMAL].stat.tx_burst_cnt));

    memset_s((void *)pst_pcie_res->st_rx_res.stat.rx_burst_cnt,
             sizeof(pst_pcie_res->st_rx_res.stat.rx_burst_cnt),
             0, sizeof(pst_pcie_res->st_rx_res.stat.rx_burst_cnt));
    oal_io_print("clear_trans_info done\n");
    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_testcase_show_trans_info(oal_pcie_res *pst_pcie_res,
                                                     oal_pci_dev_stru *pst_pci_dev,
                                                     const char *buf)
{
    int32_t i;
    oal_io_print("tx h2d trans info:\n");
    for (i = 0; i <
         sizeof(pst_pcie_res->st_tx_res[PCIE_H2D_QTYPE_NORMAL].stat.tx_burst_cnt) /
         sizeof(pst_pcie_res->st_tx_res[PCIE_H2D_QTYPE_NORMAL].stat.tx_burst_cnt[0]);
         i++) {
        oal_io_print("[%3d] burst cnt:%u\n", i,
                     pst_pcie_res->st_tx_res[PCIE_H2D_QTYPE_NORMAL].stat.tx_burst_cnt[i]);
    }

    oal_io_print("\nrx d2h trans info:\n");
    for (i = 0; i <
         sizeof(pst_pcie_res->st_rx_res.stat.rx_burst_cnt) / sizeof(pst_pcie_res->st_rx_res.stat.rx_burst_cnt[0]);
         i++) {
        oal_io_print("[%3d] burst cnt:%u\n", i,
                     pst_pcie_res->st_rx_res.stat.rx_burst_cnt[i]);
    }

    oal_pcie_print_device_transfer_info(pst_pcie_res);
    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_testcase_send_message(oal_pcie_res *pst_pcie_res,
                                                  oal_pci_dev_stru *pst_pci_dev,
                                                  const char *buf)
{
    int32_t ret;
    uint32_t message;

    buf = buf + OAL_STRLEN("send_message");
    for (; *buf == ' '; buf++);
    if ((sscanf_s(buf, "%u", &message) != 1)) {
        pci_print_log(PCI_LOG_ERR, "invalid agrument");
    } else {
        ret = oal_pcie_send_message_to_dev(pst_pcie_res, message);
        pci_print_log(PCI_LOG_INFO, "send pcie message %u to dev %s",
                      message, (ret == OAL_SUCC) ? "succ" : "failed");
    }
    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_testcase_sched_rx_task(oal_pcie_res *pst_pcie_res,
                                                   oal_pci_dev_stru *pst_pci_dev,
                                                   const char *buf)
{
    oal_pcie_linux_res *pst_pci_lres = (oal_pcie_linux_res *)oal_pci_get_drvdata(pst_pci_dev);
    if (pst_pci_lres != NULL && pst_pci_lres->pst_bus != NULL) {
        up(&pst_pci_lres->pst_bus->rx_sema);
    } else {
        pci_print_log(PCI_LOG_INFO, "sched failed!");
    }
    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_testcase_turnoff_message(oal_pcie_res *pst_pcie_res,
                                                     oal_pci_dev_stru *pst_pci_dev,
                                                     const char *buf)
{
#ifdef CONFIG_ARCH_KIRIN_PCIE
    /* 走到这里说明wakelock已经释放，WIFI已经深睡,通知RC/EP下电，发送TurnOff Message */
    /* 下电之前关闭 PCIE HOST 控制器 */
    pci_print_log(PCI_LOG_INFO, "turnoff_message host");
    kirin_pcie_power_notifiy_register(g_kirin_rc_idx, NULL, NULL, NULL);
    kirin_pcie_pm_control(0, g_kirin_rc_idx);
#endif
    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_testcase_suspend_test(oal_pcie_res *pst_pcie_res,
                                                  oal_pci_dev_stru *pst_pci_dev,
                                                  const char *buf)
{
#ifdef CONFIG_ARCH_KIRIN_PCIE

    int32_t ret__;
    /* 走到这里说明wakelock已经释放，WIFI已经深睡,通知RC/EP下电，发送TurnOff Message */
    /* 下电之前关闭 PCIE HOST 控制器 */
    pci_print_log(PCI_LOG_INFO, "suspend_test host");

    oal_pcie_change_link_state(pst_pcie_res, PCI_WLAN_LINK_DOWN);

    kirin_pcie_power_notifiy_register(g_kirin_rc_idx, NULL, NULL, NULL);
    ret__ = kirin_pcie_pm_control(0, g_kirin_rc_idx);
    board_host_wakeup_dev_set(0);
    if (ret__) {
        pci_print_log(PCI_LOG_ERR, "suspend test power on failed, ret=%d", ret__);
        declare_dft_trace_key_info("suspend test power on failed", OAL_DFT_TRACE_OTHER);
    }
#endif
    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_testcase_resume_test(oal_pcie_res *pst_pcie_res,
                                                 oal_pci_dev_stru *pst_pci_dev,
                                                 const char *buf)
{
#ifdef CONFIG_ARCH_KIRIN_PCIE
    int32_t ret__;
    /* 走到这里说明wakelock已经释放，WIFI已经深睡,通知RC/EP下电，发送TurnOff Message */
    /* 下电之前关闭 PCIE HOST 控制器 */
    pci_print_log(PCI_LOG_INFO, "resume_test host");
    kirin_pcie_power_notifiy_register(g_kirin_rc_idx, oal_pcie_resume_handler, NULL, NULL);
    ret__ = kirin_pcie_pm_control(1, g_kirin_rc_idx);
    oal_pcie_change_link_state(pst_pcie_res, PCI_WLAN_LINK_WORK_UP);
    if (ret__) {
        pci_print_log(PCI_LOG_ERR, "resume test power on failed, ret=%d", ret__);
        declare_dft_trace_key_info("resume test power on failed", OAL_DFT_TRACE_OTHER);
    }
#endif
    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_testcase_rc_send_polling(oal_pcie_res *pst_pcie_res,
                                                     oal_pci_dev_stru *pst_pci_dev,
                                                     const char *buf)
{
#ifdef CONFIG_ARCH_KIRIN_PCIE
    oal_pci_dev_stru *pst_rc_dev;
    pst_rc_dev = pci_upstream_bridge(pst_pci_dev);
    if (pst_rc_dev == NULL) {
        pci_print_log(PCI_LOG_ERR, "no upstream dev");
    } else {
        uint16_t val = 0;
        oal_pci_read_config_word(pst_rc_dev, oal_pci_pcie_cap(pst_rc_dev) + PCI_EXP_LNKCTL2, &val);
        pci_print_log(PCI_LOG_INFO, "rc polling read 0x%x , value:0x%x",
                      oal_pci_pcie_cap(pst_rc_dev) + PCI_EXP_LNKCTL2,
                      val);
        val |= (1 << 4);
        oal_pci_write_config_word(pst_rc_dev, oal_pci_pcie_cap(pst_rc_dev) + PCI_EXP_LNKCTL2, val);
    }
#endif
    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_testcase_ep_send_polling(oal_pcie_res *pst_pcie_res,
                                                     oal_pci_dev_stru *pst_pci_dev,
                                                     const char *buf)
{
    uint16_t val = 0;
    int32_t ret = oal_pci_read_config_word(pst_pci_dev, oal_pci_pcie_cap(pst_pci_dev) + PCI_EXP_LNKCTL2, &val);
    if (ret == 0) {
        pci_print_log(PCI_LOG_INFO, "ep polling read 0x%x , value:0x%x",
                      oal_pci_pcie_cap(pst_pci_dev) + PCI_EXP_LNKCTL2, val);
        val |= (1 << 4);
        ret = oal_pci_write_config_word(pst_pci_dev, oal_pci_pcie_cap(pst_pci_dev) + PCI_EXP_LNKCTL2, val);
        if (ret) {
            pci_print_log(PCI_LOG_WARN, "ep_send_polling write fail");
        }
    }
    return OAL_SUCC;
}

typedef struct _pcie_debug_testcase_ {
    char *name;
    int32_t (*testcase)(oal_pcie_res *pst_pcie_res, oal_pci_dev_stru *pst_pci_dev, const char *buf);
} pcie_debug_testcase;

OAL_STATIC pcie_debug_testcase g_pci_debug_testcases[] = {
    { "transfer_res_init", oal_pcie_testcase_transfer_res_init },
    { "send_test_pkt",     oal_pcie_testcase_send_test_pkt },
    { "send_test_pkt2",    oal_pcie_testcase_send_test_pkt2 },
    { "send_test_pkt3",    oal_pcie_testcase_send_test_pkt3 },
#ifdef _PRE_PLAT_FEATURE_HI110X_PCIE_PERFORMANCE
    { "pcie_mips_show", oal_pcie_testcase_pcie_mips_show },
    { "pcie_mips_clear", oal_pcie_testcase_pcie_mips_clear },
#endif
    { "d2h_doorbell",       oal_pcie_testcase_d2h_doorbell },
    { "h2d_doorbell",       oal_pcie_testcase_h2d_doorbell },
    { "outbound_test",      oal_pcie_testcase_outbound_test },
    { "addr_switch_test",   oal_pcie_testcase_addr_switch_test },
    { "ram_memcheck",       oal_pcie_testcase_ram_memcheck },
    { "PME_ENABLE",         oal_pcie_testcase_pme_enable },
    { "ringbuf_stat",       oal_pcie_testcase_ringbuf_stat },
    { "clear_trans_info",   oal_pcie_testcase_clear_trans_info },
    { "show_trans_info",    oal_pcie_testcase_show_trans_info },
    { "send_message",       oal_pcie_testcase_send_message },
    { "sched_rx_task",      oal_pcie_testcase_sched_rx_task },
    { "turnoff_message",    oal_pcie_testcase_turnoff_message },
    { "suspend_test",       oal_pcie_testcase_suspend_test },
    { "resume_test",        oal_pcie_testcase_resume_test },
    { "rc_send_polling",    oal_pcie_testcase_rc_send_polling },
    { "ep_send_polling",    oal_pcie_testcase_ep_send_polling },
};

OAL_STATIC int32_t oal_pcie_debug_testcase(struct kobject *dev, struct kobj_attribute *attr,
                                           const char *buf, oal_pcie_res *pst_pcie_res)
{
    /* echo readmem address length(hex) > debug */
    int32_t i;

    char casename[PCIE_DEBUG_MSG_LEN] = {0};
    oal_pci_dev_stru *pst_pci_dev = NULL;

    if (pst_pcie_res == NULL) {
        pci_print_log(PCI_LOG_ERR, "pst_pcie_res is null");
        return -OAL_EBUSY;
    }

    pst_pci_dev = pcie_res_to_dev(pst_pcie_res);
    if (pst_pci_dev == NULL) {
        pci_print_log(PCI_LOG_ERR, "pst_pci_dev is null");
        return -OAL_ENODEV;
    }

    if (strlen(buf) >= OAL_SIZEOF(casename)) {
        oal_io_print("input illegal!%s\n", __FUNCTION__);
        return -OAL_EINVAL;
    }

    /* Just for debug */
    if ((sscanf_s(buf, "%s", casename, sizeof(casename)) != 1)) {
        return -OAL_EINVAL;
    }

    for (i = 0; i < oal_array_size(g_pci_debug_testcases); i++) {
        if (g_pci_debug_testcases[i].name) {
            if (!oal_strcmp(g_pci_debug_testcases[i].name, casename)) {
                break;
            }
        }
    }

    if (i == oal_array_size(g_pci_debug_testcases)) {
        pci_print_log(PCI_LOG_WARN, "testcase: %s not found", casename);
        return -OAL_ENODEV;
    }

    if (g_pci_debug_testcases[i].testcase == NULL) {
        pci_print_log(PCI_LOG_WARN, "testcase: %s no func", casename);
        return -OAL_ENODEV;
    }

    pci_print_log(PCI_LOG_INFO, "testcase:[%s] run", casename);
    g_pci_debug_testcases[i].testcase(pst_pcie_res, pst_pci_dev, buf);

    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_performance_read(oal_pcie_res *pst_pcie_res, uint32_t cpu_address,
                                             uint32_t length, uint32_t times, uint32_t burst_size)
{
    int32_t i;
    int32_t ret;
    ktime_t start_time, last_time;
    ktime_t trans_time;
    uint64_t trans_us;
    uint64_t trans_size, us_to_s, total_size;
    uint32_t size, copy_size, remainder;
    pci_addr_map addr_map_start, addr_map_end;
    void *pst_burst_buf = NULL;
    oal_pci_dev_stru *pst_pci_dev = NULL;
    if (pst_pcie_res == NULL) {
        pci_print_log(PCI_LOG_ERR, "pst_pcie_res is null");
        return -OAL_EBUSY;
    }

    pst_pci_dev = pcie_res_to_dev(pst_pcie_res);
    if (pst_pci_dev == NULL) {
        pci_print_log(PCI_LOG_ERR, "pst_pci_dev is null");
        return -OAL_ENODEV;
    }

    if (burst_size > length) {
        pci_print_log(PCI_LOG_ERR, "burst_size  large %u than length %u", burst_size, length);
        return -OAL_EINVAL;
    }

    ret = oal_pcie_inbound_ca_to_va(pst_pcie_res, cpu_address, &addr_map_start);
    if (ret != OAL_SUCC) {
        pci_print_log(PCI_LOG_ERR, "invalid device cpu address 0x%x", cpu_address);
        return -OAL_EINVAL;
    }

    ret = oal_pcie_inbound_ca_to_va(pst_pcie_res, cpu_address + length - 1, &addr_map_end);
    if (ret != OAL_SUCC) {
        pci_print_log(PCI_LOG_ERR, "invalid device cpu address 0x%x and length %u", cpu_address, length);
        return -OAL_EINVAL;
    }

    pst_burst_buf = oal_memalloc(length);
    if (pst_burst_buf == NULL) {
        pci_print_log(PCI_LOG_ERR, "alloc burst buf failed, buf size:%u", length);
        return -OAL_ENOMEM;
    }

    trans_size = 0;
    start_time = ktime_get();
    for (i = 0; i < times; i++) {
        size = 0;
        remainder = length;
        forever_loop() {
            if (remainder == 0) {
                break;
            }

            copy_size = (remainder <= burst_size) ? remainder : burst_size;

#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
            oal_pci_cache_inv(pst_pci_dev, (void *)(addr_map_start.pa + size), copy_size);
#endif
            oal_pcie_io_trans((uintptr_t)pst_burst_buf, addr_map_start.va + size, copy_size);
            remainder -= copy_size;
            size += copy_size;
            trans_size += copy_size;
        }
    }
    total_size = trans_size;
    last_time = ktime_get();
    trans_time = ktime_sub(last_time, start_time);
    trans_us = (uint64_t)ktime_to_us(trans_time);

    if (trans_us == 0) {
        trans_us = 1;
    }

    trans_size = trans_size * 1000u;
    trans_size = trans_size * 1000u;
    trans_size = (trans_size >> PCIE_TRANS_US_OFFSET_BITS);
    trans_size = div_u64(trans_size, trans_us);
    us_to_s = trans_us;
    do_div(us_to_s, 1000000u);

    oal_io_print("red length:%u, total_size:%llu, burst_size:%u, thoughtput:%llu Mbps, trans_time:%llu us,  %llu s\n",
                 length, total_size, burst_size, trans_size, trans_us, us_to_s);

    oal_free(pst_burst_buf);

    return ret;
}

OAL_STATIC int32_t oal_pcie_performance_netbuf_alloc(oal_pcie_res *pst_pcie_res, uint32_t test_time)
{
    uint64_t count;
    int32_t ret = OAL_SUCC;
    oal_netbuf_stru *pst_netbuf = NULL;
    dma_addr_t pci_dma_addr;
    ktime_t start_time, last_time;
    ktime_t trans_time;
    unsigned long timeout;
    uint64_t trans_us;
    uint64_t trans_size, us_to_s, total_size;
    oal_pci_dev_stru *pst_pci_dev = NULL;
    pcie_cb_dma_res *pst_cb_res = NULL;
    if (pst_pcie_res == NULL) {
        pci_print_log(PCI_LOG_ERR, "pst_pcie_res is null");
        return -OAL_EBUSY;
    }

    pst_pci_dev = pcie_res_to_dev(pst_pcie_res);
    if (pst_pci_dev == NULL) {
        pci_print_log(PCI_LOG_ERR, "pst_pci_dev is null");
        return -OAL_ENODEV;
    }

    start_time = ktime_get();
    timeout = jiffies + oal_msecs_to_jiffies(test_time);
    count = 0;
    forever_loop() {
        if (oal_time_after(jiffies, timeout)) {
            break;
        }

        pst_netbuf = oal_pcie_rx_netbuf_alloc(HCC_HDR_TOTAL_LEN + PCIE_EDMA_TRANS_MAX_FRAME_LEN, GFP_ATOMIC);
        if (pst_netbuf == NULL) {
            pci_print_log(PCI_LOG_INFO, "alloc netbuf failed!");
            break;
        }

        oal_netbuf_put(pst_netbuf, (HCC_HDR_TOTAL_LEN + PCIE_EDMA_TRANS_MAX_FRAME_LEN));

        pci_dma_addr = dma_map_single(&pst_pci_dev->dev, oal_netbuf_data(pst_netbuf),
                                      oal_netbuf_len(pst_netbuf), PCI_DMA_FROMDEVICE);
        if (dma_mapping_error(&pst_pci_dev->dev, pci_dma_addr)) {
            pci_print_log(PCI_LOG_INFO, "rx dma map netbuf failed, len=%u",
                          oal_netbuf_len(pst_netbuf));
            oal_netbuf_free(pst_netbuf);
            break;
        }

        pst_cb_res = (pcie_cb_dma_res *)oal_netbuf_cb(pst_netbuf);
        pst_cb_res->paddr.addr = pci_dma_addr;
        pst_cb_res->len = oal_netbuf_len(pst_netbuf);

        /* 释放内存 */
        dma_unmap_single(&pst_pci_dev->dev, (dma_addr_t)pst_cb_res->paddr.addr, pst_cb_res->len, PCI_DMA_FROMDEVICE);
        oal_netbuf_free(pst_netbuf);

        count++;
    }

    total_size = (HCC_HDR_TOTAL_LEN + PCIE_EDMA_TRANS_MAX_FRAME_LEN) * count;
    trans_size = total_size;
    last_time = ktime_get();
    trans_time = ktime_sub(last_time, start_time);
    trans_us = (uint64_t)ktime_to_us(trans_time);

    if (trans_us == 0) {
        trans_us = 1;
    }

    trans_size = trans_size * 1000u;
    trans_size = trans_size * 1000u;
    trans_size = (trans_size >> PCIE_TRANS_US_OFFSET_BITS);
    trans_size = div_u64(trans_size, trans_us);
    us_to_s = trans_us;
    do_div(us_to_s, 1000000u);

    oal_io_print("total_size:%llu, thoughtput:%llu Mbps, trans_time:%llu us,  %llu s\n",
                 total_size, trans_size, trans_us, us_to_s);

    return ret;
}

OAL_STATIC int32_t oal_pcie_performance_netbuf_queue(oal_pcie_res *pst_pcie_res,
                                                     uint32_t test_time, uint32_t alloc_count)
{
    uint64_t count;
    int32_t i;
    int32_t ret = OAL_SUCC;
    oal_netbuf_stru *pst_netbuf = NULL;
    oal_netbuf_head_stru st_netbuf_queue;
    dma_addr_t pci_dma_addr;
    ktime_t start_time, last_time;
    ktime_t trans_time;
    unsigned long timeout;
    uint64_t trans_us;
    uint64_t trans_size, us_to_s, total_size;
    oal_pci_dev_stru *pst_pci_dev = NULL;
    pcie_cb_dma_res *pst_cb_res = NULL;
    if (pst_pcie_res == NULL) {
        pci_print_log(PCI_LOG_ERR, "pst_pcie_res is null");
        return -OAL_EBUSY;
    }

    pst_pci_dev = pcie_res_to_dev(pst_pcie_res);
    if (pst_pci_dev == NULL) {
        pci_print_log(PCI_LOG_ERR, "pst_pci_dev is null");
        return -OAL_ENODEV;
    }

    if (alloc_count == 0) {
        alloc_count = 1;
    }

    start_time = ktime_get();
    timeout = jiffies + oal_msecs_to_jiffies(test_time);
    count = 0;
    oal_netbuf_head_init(&st_netbuf_queue);
    forever_loop() {
        if (oal_time_after(jiffies, timeout)) {
            break;
        }

        for (i = 0; i < alloc_count; i++) {
            pst_netbuf = oal_pcie_rx_netbuf_alloc(HCC_HDR_TOTAL_LEN + PCIE_EDMA_TRANS_MAX_FRAME_LEN, GFP_ATOMIC);
            if (pst_netbuf == NULL) {
                pci_print_log(PCI_LOG_INFO, "alloc netbuf failed!");
                break;
            }

            oal_netbuf_put(pst_netbuf, (HCC_HDR_TOTAL_LEN + PCIE_EDMA_TRANS_MAX_FRAME_LEN));

            pci_dma_addr = dma_map_single(&pst_pci_dev->dev, oal_netbuf_data(pst_netbuf),
                                          oal_netbuf_len(pst_netbuf), PCI_DMA_FROMDEVICE);
            if (dma_mapping_error(&pst_pci_dev->dev, pci_dma_addr)) {
                pci_print_log(PCI_LOG_INFO, "rx dma map netbuf failed, len=%u",
                              oal_netbuf_len(pst_netbuf));
                oal_netbuf_free(pst_netbuf);
                break;
            }

            pst_cb_res = (pcie_cb_dma_res *)oal_netbuf_cb(pst_netbuf);
            pst_cb_res->paddr.addr = pci_dma_addr;
            pst_cb_res->len = oal_netbuf_len(pst_netbuf);

            oal_netbuf_list_tail_nolock(&st_netbuf_queue, pst_netbuf);
        }

        forever_loop() {
            pst_netbuf = oal_netbuf_delist_nolock(&st_netbuf_queue);
            if (pst_netbuf == NULL) {
                break;
            }

            pst_cb_res = (pcie_cb_dma_res *)oal_netbuf_cb(pst_netbuf);
            /* 释放内存 */
            dma_unmap_single(&pst_pci_dev->dev, (dma_addr_t)pst_cb_res->paddr.addr,
                             pst_cb_res->len, PCI_DMA_FROMDEVICE);
            oal_netbuf_free(pst_netbuf);
            count++;
        }
    }

    total_size = (HCC_HDR_TOTAL_LEN + PCIE_EDMA_TRANS_MAX_FRAME_LEN) * count;
    last_time = ktime_get();
    trans_time = ktime_sub(last_time, start_time);
    trans_us = (uint64_t)ktime_to_us(trans_time);

    if (trans_us == 0) {
        trans_us = 1;
    }
    trans_size = total_size;
    trans_size = trans_size * 1000u;
    trans_size = trans_size * 1000u;
    trans_size = (trans_size >> PCIE_TRANS_US_OFFSET_BITS);
    trans_size = div_u64(trans_size, trans_us);
    us_to_s = trans_us;
    do_div(us_to_s, 1000000u);

    oal_io_print("alloc_count:%u, thoughtput:%llu Mbps, trans_time:%llu us,  %llu s\n",
                 alloc_count, trans_size, trans_us, us_to_s);

    return ret;
}

OAL_STATIC int32_t oal_pcie_performance_cpu(oal_pcie_res *pst_pcie_res,
                                            uint32_t length, uint32_t times, uint32_t burst_size)
{
    int32_t i;
    int32_t ret = OAL_SUCC;
    ktime_t start_time, last_time;
    ktime_t trans_time;
    uint64_t trans_us;
    uint64_t trans_size, us_to_s, total_size;
    uint32_t size, copy_size, remainder;
    void *pst_burst_buf_src = NULL;
    void *pst_burst_buf_dst = NULL;
    oal_pci_dev_stru *pst_pci_dev = NULL;
    if (pst_pcie_res == NULL) {
        pci_print_log(PCI_LOG_ERR, "pst_pcie_res is null");
        return -OAL_EBUSY;
    }

    pst_pci_dev = pcie_res_to_dev(pst_pcie_res);
    if (pst_pci_dev == NULL) {
        pci_print_log(PCI_LOG_ERR, "pst_pci_dev is null");
        return -OAL_ENODEV;
    }

    if (burst_size > length) {
        pci_print_log(PCI_LOG_ERR, "burst_size  large %u than length %u", burst_size, length);
        return -OAL_EINVAL;
    }

    pst_burst_buf_dst = oal_memalloc(length);
    if (pst_burst_buf_dst == NULL) {
        pci_print_log(PCI_LOG_ERR, "alloc src burst buf failed, buf size:%u", length);
        return -OAL_ENOMEM;
    }

    pst_burst_buf_src = oal_memalloc(length);
    if (pst_burst_buf_src == NULL) {
        pci_print_log(PCI_LOG_ERR, "alloc dst burst buf failed, buf size:%u", length);
        oal_free(pst_burst_buf_dst);
        return -OAL_ENOMEM;
    }

    trans_size = 0;
    start_time = ktime_get();
    for (i = 0; i < times; i++) {
        size = 0;
        remainder = length;
        forever_loop() {
            if (remainder == 0) {
                break;
            }

            copy_size = (remainder <= burst_size) ? remainder : burst_size;

            if (memcpy_s(pst_burst_buf_dst, length, pst_burst_buf_src, copy_size) != EOK) {
                pci_print_log(PCI_LOG_ERR, "memcpy_s error, destlen=%u, srclen=%u\n ", length, copy_size);
                break;
            }

            remainder -= copy_size;
            size += copy_size;
            trans_size += copy_size;
        }
    }
    total_size = trans_size;
    last_time = ktime_get();
    trans_time = ktime_sub(last_time, start_time);
    trans_us = (uint64_t)ktime_to_us(trans_time);

    if (trans_us == 0) {
        trans_us = 1;
    }

    trans_size = trans_size * 1000u;
    trans_size = trans_size * 1000u;
    trans_size = (trans_size >> PCIE_TRANS_US_OFFSET_BITS);
    trans_size = div_u64(trans_size, trans_us);
    us_to_s = trans_us;
    do_div(us_to_s, 1000000u);

    oal_io_print("red length:%u, total_size:%llu, burst_size:%u, thoughtput:%llu Mbps, trans_time:%llu us,  %llu s\n",
                 length, total_size, burst_size, trans_size, trans_us, us_to_s);

    oal_free(pst_burst_buf_dst);
    oal_free(pst_burst_buf_src);

    return ret;
}

OAL_STATIC int32_t oal_pcie_performance_write(oal_pcie_res *pst_pcie_res, uint32_t cpu_address, uint32_t length,
                                              uint32_t times, uint32_t burst_size, uint32_t value)
{
    int32_t i;
    int32_t ret;
    ktime_t start_time, last_time;
    ktime_t trans_time;
    uint64_t trans_us;
    uint64_t trans_size, us_to_s, total_size;
    uint32_t size, copy_size, remainder;
    pci_addr_map addr_map_start, addr_map_end;
    void *pst_burst_buf = NULL;
    oal_pci_dev_stru *pst_pci_dev = NULL;
    if (pst_pcie_res == NULL) {
        pci_print_log(PCI_LOG_ERR, "pst_pcie_res is null");
        return -OAL_EBUSY;
    }

    pst_pci_dev = pcie_res_to_dev(pst_pcie_res);
    if (pst_pci_dev == NULL) {
        pci_print_log(PCI_LOG_ERR, "pst_pci_dev is null");
        return -OAL_ENODEV;
    }

    if (burst_size > length) {
        pci_print_log(PCI_LOG_ERR, "burst_size  large %u than length %u", burst_size, length);
        return -OAL_EINVAL;
    }

    ret = oal_pcie_inbound_ca_to_va(pst_pcie_res, cpu_address, &addr_map_start);
    if (ret != OAL_SUCC) {
        pci_print_log(PCI_LOG_ERR, "invalid device cpu address 0x%x", cpu_address);
        return -OAL_EINVAL;
    }

    ret = oal_pcie_inbound_ca_to_va(pst_pcie_res, cpu_address + length - 1, &addr_map_end);
    if (ret != OAL_SUCC) {
        pci_print_log(PCI_LOG_ERR, "invalid device cpu address 0x%x and length %u", cpu_address, length);
        return -OAL_EINVAL;
    }

    pst_burst_buf = oal_memalloc(length);
    if (pst_burst_buf == NULL) {
        pci_print_log(PCI_LOG_ERR, "alloc burst buf failed, buf size:%u", length);
        return -OAL_ENOMEM;
    }

    memset_s(pst_burst_buf, length, (int32_t)value, length);

    trans_size = 0;
    start_time = ktime_get();
    for (i = 0; i < times; i++) {
        size = 0;
        remainder = length;
        forever_loop() {
            if (remainder == 0) {
                break;
            }

            copy_size = (remainder <= burst_size) ? remainder : burst_size;
            oal_pcie_io_trans(addr_map_start.va + size, (uintptr_t)pst_burst_buf, copy_size);
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
            oal_pci_cache_flush(pst_pci_dev, (void *)(addr_map_start.pa + size), copy_size);
#endif
            remainder -= copy_size;
            size += copy_size;
            trans_size += copy_size;
        }
    }

    total_size = trans_size;
    last_time = ktime_get();
    trans_time = ktime_sub(last_time, start_time);
    trans_us = (uint64_t)ktime_to_us(trans_time);

    if (trans_us == 0) {
        trans_us = 1;
    }

    trans_size = trans_size * 1000u;
    trans_size = trans_size * 1000u;
    trans_size = (trans_size >> PCIE_TRANS_US_OFFSET_BITS);
    trans_size = div_u64(trans_size, trans_us);
    us_to_s = trans_us;
    do_div(us_to_s, 1000000u);

    oal_io_print(" write length:%u, total_size:%llu,burst_size:%u,value:0x%x thoughtput:%llu Mbps, \
                 trans_time:%llu us,  %llu s\n",
                 length, total_size, burst_size, value, trans_size, trans_us, us_to_s);

    oal_free(pst_burst_buf);

    return ret;
}

OAL_STATIC int32_t oal_pcie_debug_performance_read(struct kobject *dev, struct kobj_attribute *attr,
                                                   const char *buf, oal_pcie_res *pst_pcie_res)
{
    /* echo readmem address length(hex) > debug */
    int32_t ret;
    uint32_t cpu_address;
    uint32_t length, times, burst_size;

    /* Just for debug */
    if ((sscanf_s(buf, "0x%x %u %u %u", &cpu_address, &length, &times, &burst_size) != 4)) {
        return -OAL_EINVAL;
    }

    ret = oal_pcie_performance_read(pst_pcie_res, cpu_address, length, times, burst_size);
    return ret;
}

OAL_STATIC int32_t oal_pcie_debug_performance_cpu(struct kobject *dev, struct kobj_attribute *attr,
                                                  const char *buf, oal_pcie_res *pst_pcie_res)
{
    int32_t ret;
    uint32_t length, times, burst_size;

    /* Just for debug */
    if ((sscanf_s(buf, "%u %u %u", &length, &times, &burst_size) != 3)) {
        return -OAL_EINVAL;
    }

    ret = oal_pcie_performance_cpu(pst_pcie_res, length, times, burst_size);
    return ret;
}

OAL_STATIC int32_t oal_pcie_debug_performance_netbuf_alloc(struct kobject *dev, struct kobj_attribute *attr,
                                                           const char *buf, oal_pcie_res *pst_pcie_res)
{
    int32_t ret;
    uint32_t test_time;

    /* Just for debug */
    if ((sscanf_s(buf, "%u", &test_time) != 1)) {
        return -OAL_EINVAL;
    }

    ret = oal_pcie_performance_netbuf_alloc(pst_pcie_res, test_time);
    return ret;
}

OAL_STATIC int32_t oal_pcie_debug_performance_netbuf_queue(struct kobject *dev, struct kobj_attribute *attr,
                                                           const char *buf, oal_pcie_res *pst_pcie_res)
{
    int32_t ret;
    uint32_t test_time, alloc_count;

    /* Just for debug */
    if ((sscanf_s(buf, "%u %u", &test_time, &alloc_count) != 2)) {
        return -OAL_EINVAL;
    }

    ret = oal_pcie_performance_netbuf_queue(pst_pcie_res, test_time, alloc_count);
    return ret;
}

#ifdef _PRE_PLAT_FEATURE_HI110X_PCIE_H2D_BYPASS
int32_t oal_pcie_performance_h2d_bypass(oal_pcie_res *pst_pcie_res,
                                        uint32_t pkt_num, uint32_t pkt_len)
{
    /* bypass hcc */
    int32_t i;
    oal_pci_dev_stru *pst_pci_dev = NULL;
    pcie_cb_dma_res *pst_cb_res = NULL;
    unsigned long timeout;
    ktime_t start_time, last_time;
    ktime_t trans_time;
    int32_t flag = 0;
    uint64_t trans_us;
    uint64_t trans_size, us_to_s, total_size;

    if (pst_pcie_res == NULL) {
        pci_print_log(PCI_LOG_ERR, "pst_pcie_res is null");
        return -OAL_EBUSY;
    }

    pst_pci_dev = pcie_res_to_dev(pst_pcie_res);
    if (pst_pci_dev == NULL) {
        pci_print_log(PCI_LOG_ERR, "pst_pci_dev is null");
        return -OAL_ENODEV;
    }

    if (pkt_num < 1 || pkt_len > PCIE_EDMA_TRANS_MAX_FRAME_LEN) {
        oal_io_print("invalid argument, pkt_num:%u, pkt_len:%u\n", pkt_num, pkt_len);
        return -OAL_EINVAL;
    }

    h2d_bypass_pkt_num = 0;

    g_h2d_pst_netbuf = oal_pcie_rx_netbuf_alloc(HCC_HDR_TOTAL_LEN + pkt_len, GFP_ATOMIC);
    if (g_h2d_pst_netbuf == NULL) {
        pci_print_log(PCI_LOG_ERR, "alloc netbuf %u failed!", HCC_HDR_TOTAL_LEN + pkt_len);
        return -OAL_ENOMEM;
    }

    oal_netbuf_put(g_h2d_pst_netbuf, HCC_HDR_TOTAL_LEN + pkt_len);

    g_h2d_pci_dma_addr = dma_map_single(&pst_pci_dev->dev, oal_netbuf_data(g_h2d_pst_netbuf),
                                        oal_netbuf_len(g_h2d_pst_netbuf), PCI_DMA_FROMDEVICE);
    if (dma_mapping_error(&pst_pci_dev->dev, g_h2d_pci_dma_addr)) {
        pci_print_log(PCI_LOG_INFO, "h2d bypass dma map netbuf failed, len=%u",
                      oal_netbuf_len(g_h2d_pst_netbuf));
        oal_netbuf_free(g_h2d_pst_netbuf);
        g_h2d_pst_netbuf = NULL;
        return -OAL_ENOMEM;
    }

    pst_cb_res = (pcie_cb_dma_res *)oal_netbuf_cb(g_h2d_pst_netbuf);
    pst_cb_res->paddr.addr = g_h2d_pci_dma_addr;
    pst_cb_res->len = oal_netbuf_len(g_h2d_pst_netbuf);

    oal_io_print("performance_h2d_bypass cpu=%d\n", get_cpu());
    put_cpu();

    start_time = ktime_get();

    for (i = 0; i < pkt_num; i++) {
        forever_loop() {
            if (oal_pcie_send_netbuf(pst_pcie_res, g_h2d_pst_netbuf, PCIE_H2D_QTYPE_NORMAL) == 0) {
                if (flag) {
                    /* 更新device侧wr指针,刷ringbuf cache */
                    oal_pcie_h2d_ringbuf_wr_update(pst_pcie_res, PCIE_H2D_QTYPE_NORMAL);

                    /* tx doorbell */
                    oal_pcie_h2d_doorbell(pst_pcie_res);
                }
                flag = 0;
                cpu_relax();
            } else {
                flag = 1;
                break;
            }
        }

        /* 更新device侧wr指针,刷ringbuf cache */
        oal_pcie_h2d_ringbuf_wr_update(pst_pcie_res, PCIE_H2D_QTYPE_NORMAL);

        /* tx doorbell */
        oal_pcie_h2d_doorbell(pst_pcie_res);
    }

    /* 更新device侧wr指针,刷ringbuf cache */
    oal_pcie_h2d_ringbuf_wr_update(pst_pcie_res, PCIE_H2D_QTYPE_NORMAL);

    /* tx doorbell */
    oal_pcie_h2d_doorbell(pst_pcie_res);

    /* 等待 回来的 包个数 */
    timeout = jiffies + oal_msecs_to_jiffies(10000);
    forever_loop() {
        if (oal_time_after(jiffies, timeout)) {
            pci_print_log(PCI_LOG_ERR, "wait h2d bypass transfer done timeout, pkt num:%u, just came :%u",
                          pkt_num, h2d_bypass_pkt_num);
            break;
        }

        if (h2d_bypass_pkt_num >= pkt_num) {
            break;
        }
        cpu_relax();
    }

    last_time = ktime_get();
    trans_time = ktime_sub(last_time, start_time);
    trans_us = (uint64_t)ktime_to_us(trans_time);

    if (trans_us == 0) {
        trans_us = 1;
    }

    trans_size = (uint64_t)pkt_len * pkt_num;
    trans_size = trans_size * 1000u;
    trans_size = trans_size * 1000u;
    trans_size = (trans_size >> PCIE_TRANS_US_OFFSET_BITS);
    trans_size = div_u64(trans_size, trans_us);
    us_to_s = trans_us;
    do_div(us_to_s, 1000000u);

    oal_io_print("pkt_num:%u, thoughtput:%llu Mbps, trans_time:%llu us,  %llu s\n",
                 pkt_num, trans_size, trans_us, us_to_s);

    dma_unmap_single(&pst_pci_dev->dev, (dma_addr_t)g_h2d_pci_dma_addr,
                     oal_netbuf_len(g_h2d_pst_netbuf), PCI_DMA_FROMDEVICE);
    g_h2d_pci_dma_addr = NULL;
    oal_netbuf_free(g_h2d_pst_netbuf);
    g_h2d_pst_netbuf = NULL;
    return OAL_SUCC;
}

int32_t oal_pcie_debug_performance_h2d_bypass(struct kobject *dev, struct kobj_attribute *attr,
                                              const char *buf, oal_pcie_res *pst_pcie_res)
{
    int32_t ret;
    uint32_t pkt_num, pkt_len;

    /* Just for debug */
    if ((sscanf_s(buf, "%u %u", &pkt_num, &pkt_len) != 2)) {
        return -OAL_EINVAL;
    }

    ret = oal_pcie_performance_h2d_bypass(pst_pcie_res, pkt_num, pkt_len);
    return ret;
}
#endif

#ifdef _PRE_PLAT_FEATURE_HI110X_PCIE_D2H_BYPASS
int32_t oal_pcie_performance_d2h_bypass(oal_pcie_res *pst_pcie_res,
                                        uint32_t pkt_num, uint32_t pkt_len)
{
    /* bypass hcc */
    int32_t i;
    int32_t ret;
    oal_pci_dev_stru *pst_pci_dev = NULL;
    pcie_cb_dma_res *pst_cb_res = NULL;
    unsigned long timeout;
    ktime_t start_time, last_time;
    ktime_t trans_time;
    int32_t flag = 0;
    uint64_t trans_us;
    uint64_t trans_size, us_to_s, total_size;

    if (pst_pcie_res == NULL) {
        pci_print_log(PCI_LOG_ERR, "pst_pcie_res is null");
        return -OAL_EBUSY;
    }

    pst_pci_dev = pcie_res_to_dev(pst_pcie_res);
    if (pst_pci_dev == NULL) {
        pci_print_log(PCI_LOG_ERR, "pst_pci_dev is null");
        return -OAL_ENODEV;
    }

    /* pkt_num后续需要 | 一个寄存器的高16bit，所以要保证这里不大于2^16 */
    if (pkt_num < 1 || pkt_len > PCIE_EDMA_TRANS_MAX_FRAME_LEN || pkt_num > 65536) {
        oal_io_print("invalid argument, pkt_num:%u, pkt_len:%u\n", pkt_num, pkt_len);
        return -OAL_EINVAL;
    }

    d2h_bypass_pkt_num = 0;

    g_d2h_pst_netbuf = oal_pcie_rx_netbuf_alloc(HCC_HDR_TOTAL_LEN + PCIE_EDMA_TRANS_MAX_FRAME_LEN, GFP_ATOMIC);
    if (g_d2h_pst_netbuf == NULL) {
        pci_print_log(PCI_LOG_ERR, "alloc netbuf %u failed!", HCC_HDR_TOTAL_LEN + PCIE_EDMA_TRANS_MAX_FRAME_LEN);
        return -OAL_ENOMEM;
    }

    oal_netbuf_put(g_d2h_pst_netbuf, HCC_HDR_TOTAL_LEN + PCIE_EDMA_TRANS_MAX_FRAME_LEN);

    g_d2h_pci_dma_addr = dma_map_single(&pst_pci_dev->dev, oal_netbuf_data(g_d2h_pst_netbuf),
                                        oal_netbuf_len(g_d2h_pst_netbuf), PCI_DMA_FROMDEVICE);
    if (dma_mapping_error(&pst_pci_dev->dev, g_d2h_pci_dma_addr)) {
        pci_print_log(PCI_LOG_INFO, "h2d bypass dma map netbuf failed, len=%u",
                      oal_netbuf_len(g_d2h_pst_netbuf));
        oal_netbuf_free(g_d2h_pst_netbuf);
        g_d2h_pst_netbuf = NULL;
        return -OAL_ENOMEM;
    }

    pst_cb_res = (pcie_cb_dma_res *)oal_netbuf_cb(g_d2h_pst_netbuf);
    pst_cb_res->paddr.addr = g_d2h_pci_dma_addr;
    pst_cb_res->len = oal_netbuf_len(g_d2h_pst_netbuf);
    oal_reinit_completion(g_d2h_test_done);

    oal_pcie_rx_ringbuf_bypass_supply(pst_pcie_res, OAL_TRUE, OAL_TRUE, PCIE_RX_RINGBUF_SUPPLY_ALL);

    /* 启动RX */
    d2h_bypass_total_pkt_num = pkt_num;
    oal_writel((uint16_t)pkt_len << 16 | ((uint16_t)pkt_num),
               pst_pcie_res->pst_pci_ctrl_base + PCIE_HOST_DEVICE_REG0);
    oal_writel(PCIE_H2D_TRIGGER_VALUE, pst_pcie_res->pst_pci_ctrl_base + PCIE_D2H_DOORBELL_OFF);

    oal_pcie_mips_start(PCIE_MIPS_HCC_RX_TOTAL);
    start_time = ktime_get();
    /* 补充内存 */
    ret = wait_for_completion_interruptible(&g_d2h_test_done);
    if (ret < 0) {
        pci_print_log(PCI_LOG_INFO, "g_d2h_test_done wait interrupt!, rx cnt:%u", d2h_bypass_pkt_num);
    }

    last_time = ktime_get();
    oal_pcie_mips_end(PCIE_MIPS_HCC_RX_TOTAL);
    trans_time = ktime_sub(last_time, start_time);
    trans_us = (uint64_t)ktime_to_us(trans_time);

    if (trans_us == 0) {
        trans_us = 1;
    }

    trans_size = (uint64_t)pkt_len * d2h_bypass_pkt_num;
    trans_size = trans_size * 1000u;
    trans_size = trans_size * 1000u;
    trans_size = (trans_size >> PCIE_TRANS_US_OFFSET_BITS);
    trans_size = div_u64(trans_size, trans_us);
    us_to_s = trans_us;
    do_div(us_to_s, 1000000u);

    oal_io_print("pkt_num:%u, thoughtput:%llu Mbps, trans_time:%llu us,  %llu s\n",
                 d2h_bypass_pkt_num, trans_size, trans_us, us_to_s);
    oal_pcie_mips_show();

    dma_unmap_single(&pst_pci_dev->dev, (dma_addr_t)g_d2h_pci_dma_addr,
                     oal_netbuf_len(g_d2h_pst_netbuf), PCI_DMA_FROMDEVICE);
    g_d2h_pci_dma_addr = NULL;
    oal_netbuf_free(g_d2h_pst_netbuf);
    g_d2h_pst_netbuf = NULL;
    return OAL_SUCC;
}

int32_t oal_pcie_debug_performance_d2h_bypass(struct kobject *dev, struct kobj_attribute *attr,
                                              const char *buf, oal_pcie_res *pst_pcie_res)
{
    int32_t ret;
    uint32_t pkt_num, pkt_len;

    /* Just for debug */
    if ((sscanf_s(buf, "%u %u", &pkt_num, &pkt_len) != 2)) {
        return -OAL_EINVAL;
    }

    ret = oal_pcie_performance_d2h_bypass(pst_pcie_res, pkt_num, pkt_len);
    return ret;
}
#endif

OAL_STATIC int32_t oal_pcie_debug_performance_write(struct kobject *dev, struct kobj_attribute *attr,
                                                    const char *buf, oal_pcie_res *pst_pcie_res)
{
    /* echo readmem address length(hex) > debug */
    int32_t ret;
    uint32_t cpu_address;
    uint32_t length, times, burst_size, value;

    /* Just for debug */
    if ((sscanf_s(buf, "0x%x %u %u %u 0x%x", &cpu_address, &length, &times, &burst_size, &value) != 5)) {
        return -OAL_EINVAL;
    }

    ret = oal_pcie_performance_write(pst_pcie_res, cpu_address, length, times, burst_size, value);
    return ret;
}

OAL_STATIC int32_t oal_pcie_loadfile(oal_pcie_res *pst_pcie_res, char *file_name,
                                     uint32_t cpu_address, int32_t performance)
{
    /* echo loadfile address filename > debug */
    /* echo loadfile 0x600000 /tmp/readmem.bin */
    struct file *fp = NULL;

    int32_t ret, rlen, total_len;
    pci_addr_map addr_map;

    void *pst_buf = NULL;
    oal_pci_dev_stru *pst_pci_dev = NULL;
    ktime_t start_time, last_time;
    ktime_t trans_time;
    uint64_t us_to_s, trans_us, file_us;
    mm_segment_t fs;

    if (pst_pcie_res == NULL) {
        pci_print_log(PCI_LOG_ERR, "pst_pcie_res is null");
        return -OAL_EBUSY;
    }

    pst_pci_dev = pcie_res_to_dev(pst_pcie_res);
    if (pst_pci_dev == NULL) {
        pci_print_log(PCI_LOG_ERR, "pst_pci_dev is null");
        return -OAL_ENODEV;
    }

    pst_buf = oal_memalloc(PAGE_SIZE);
    if (pst_buf == NULL) {
        pci_print_log(PCI_LOG_ERR, "pst_buf is null");
        return -OAL_ENOMEM;
    }

    fs = get_fs();
    set_fs(KERNEL_DS);
    fp = filp_open(file_name, O_RDWR, 0664);
    set_fs(fs);
    if (oal_is_err_or_null(fp)) {
        pci_print_log(PCI_LOG_ERR, "open file error,fp = 0x%p, filename is [%s]\n", fp, file_name);
        oal_free(pst_buf);
        return -OAL_EINVAL;
    }

    pci_print_log(PCI_LOG_DBG, "loadfile cpu:0x%8x loadpath:%s", cpu_address, file_name);
    total_len = 0;
    file_us = 0;
    us_to_s = 0;
    trans_us = 0;

    if (performance) {
        start_time = ktime_get();
    }

    forever_loop() {
        rlen = oal_file_read_ext(fp, fp->f_pos, (void *)pst_buf, PAGE_SIZE);

        if (rlen <= 0) {
            break;
        }
        fp->f_pos += rlen;
        ret = oal_pcie_inbound_ca_to_va(pst_pcie_res, cpu_address + total_len, &addr_map);
        if (ret != OAL_SUCC) {
            pci_print_log(PCI_LOG_ERR, "loadfile address 0x%8x invalid", cpu_address + total_len);
            break;
        }
        total_len += rlen;
        oal_pcie_io_trans(addr_map.va, (uintptr_t)pst_buf, rlen);
#ifdef CONFIG_PCIE_MEM_WR_CACHE_ENABLE
        oal_pci_cache_flush(pst_pci_dev, (void *)addr_map.pa, rlen);
#endif
    }

    if (performance) {
        last_time = ktime_get();
        trans_time = ktime_sub(last_time, start_time);
        trans_us = (uint64_t)ktime_to_us(trans_time);

        if (trans_us == 0) {
            trans_us = 1;
        }

        us_to_s = trans_us;
        do_div(us_to_s, 1000000u);
    }

    if (total_len) {
        if (performance)
            pci_print_log(PCI_LOG_INFO,
                          "loadfile cpu:0x%8x len:%u loadpath:%s done, cost %llu us %llu s, file_us:%llu",
                          cpu_address, total_len, file_name, trans_us, us_to_s, file_us);
        else
            pci_print_log(PCI_LOG_INFO, "loadfile cpu:0x%8x len:%u loadpath:%s done",
                          cpu_address, total_len, file_name);
    } else {
        pci_print_log(PCI_LOG_INFO, "loadfile cpu:0x%8x len:%u loadpath:%s failed",
                      cpu_address, total_len, file_name);
    }
    oal_free(pst_buf);
    oal_file_close(fp);
    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_debug_loadfile(struct kobject *dev, struct kobj_attribute *attr,
                                           const char *buf, oal_pcie_res *pst_pcie_res)
{
    int32_t ret;
    char file_name[PCIE_DEBUG_MSG_LEN];
    uint32_t cpu_address;

    if (strlen(buf) >= OAL_SIZEOF(file_name)) {
        oal_io_print("input illegal!%s\n", __FUNCTION__);
        return -OAL_EINVAL;
    }

    if ((sscanf_s(buf, "0x%x %s", &cpu_address, file_name, sizeof(file_name)) != 2)) {
        pci_print_log(PCI_LOG_ERR,
                      "loadfile argument invalid,[%s], should be [echo writemem address  filename > debug]",
                      buf);
        return -OAL_EINVAL;
    }

    ret = oal_pcie_loadfile(pst_pcie_res, file_name, cpu_address, 1);

    return ret;
}

/* sysfs debug */
typedef struct _pcie_sysfs_debug_info_ {
    char *name;
    char *usage;
    int32_t (*debug)(struct kobject *dev, struct kobj_attribute *attr, const char *buf, oal_pcie_res *pst_pcie_res);
} pcie_sysfs_debug_info;

OAL_STATIC int32_t oal_pcie_print_debug_info(struct kobject *dev, struct kobj_attribute *attr,
                                             const char *buf, oal_pcie_res *pst_pcie_res)
{
    oal_pcie_print_debug_usages();
    return OAL_SUCC;
}

OAL_STATIC pcie_sysfs_debug_info g_pci_debug[] = {
    { "help",                     "",   oal_pcie_print_debug_info },
    { "dump_all_regions_mem",     "",   oal_pcie_dump_all_regions_mem },
    { "dump_all_regions_info",    "address(hex) value(hex)", oal_pcie_dump_all_regions },
    { "read32",                   "address(hex)",   oal_pcie_debug_read32 },
    { "write32",                  "address(hex) value(hex)", oal_pcie_debug_write32 },
    { "host_read32",              "address(hex)",  oal_pcie_debug_host_read32 },
    { "host_write32",             "address(hex) value(hex)",  oal_pcie_debug_host_write32 },
    { "read16",                   "address(hex)",    oal_pcie_debug_read16 },
    { "write16",                  "address(hex) value(hex)",  oal_pcie_debug_write16 },
    { "saveconfigmem",            "address(hex) length(decimal) filename", oal_pcie_debug_saveconfigmem },
    { "savemem",                  "address(hex) length(decimal) filename", oal_pcie_debug_savemem },
    { "save_hostmem",             "address(hex) length(decimal) filename", oal_pcie_debug_save_hostmem },
    { "loadfile",                 "address(hex) filename",  oal_pcie_debug_loadfile },
    { "readmem",                  "address(hex) length(decimal)", oal_pcie_debug_readmem },
    { "readconfigmem",            "address(hex) length(decimal)", oal_pcie_debug_readmem_config },
    { "performance_read",         "address(hex) length(decimal) times(decimal) burst_size(decimal)", oal_pcie_debug_performance_read },
    { "performance_cpu",          "length(decimal) times(decimal) burst_size(decimal)",                          oal_pcie_debug_performance_cpu },
    { "performance_write",        "address(hex) length(decimal) times(decimal) burst_size(decimal) value(hex) ", oal_pcie_debug_performance_write },
    { "performance_netbuf_alloc", "test_time(decimal msec)",  oal_pcie_debug_performance_netbuf_alloc },
    { "performance_netbuf_queue", "test_time(decimal msec) alloc_count(decimal)",  oal_pcie_debug_performance_netbuf_queue },
#ifdef _PRE_PLAT_FEATURE_HI110X_PCIE_H2D_BYPASS
    { "performance_h2d_bypass", "pkt_num(decimal) pkt_len(decimal)", oal_pcie_debug_performance_h2d_bypass },
#endif
#ifdef _PRE_PLAT_FEATURE_HI110X_PCIE_D2H_BYPASS
    { "performance_d2h_bypass", "pkt_num(decimal) pkt_len(decimal)", oal_pcie_debug_performance_d2h_bypass },
#endif
    { "testcase",          "casename(string)",                 oal_pcie_debug_testcase },
    { "wlan_power_on", "",                                 oal_pcie_debug_wlan_power_on },
    { "read_dsm32",        "dsm_type(decimal)",                oal_pcie_debug_read_dsm32 },
    { "write_dsm32",       "dsm_type(decimal) value(decimal)", oal_pcie_debug_write_dsm32 }
};

OAL_STATIC void oal_pcie_print_debug_usages(void)
{
    int32_t i;
    int32_t ret;
    void *buf = oal_memalloc(PAGE_SIZE);
    if (buf == NULL) {
        return;
    }

    for (i = 0; i < oal_array_size(g_pci_debug); i++) {
        ret = snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "echo %s %s > /sys/hisys/pci/pcie/debug\n",
                         g_pci_debug[i].name ? : "", g_pci_debug[i].usage ? : "");
        if (ret < 0) {
            oal_free(buf);
            pci_print_log(PCI_LOG_ERR, "log str format err line[%d]\n", __LINE__);
            return;
        }
        printk("%s", (char *)buf);
    }

    oal_free(buf);
}

OAL_STATIC void oal_pcie_print_debug_usage(int32_t i)
{
    int32_t ret;
    void *buf = oal_memalloc(PAGE_SIZE);
    if (buf == NULL) {
        return;
    }

    ret = snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "echo %s %s > /sys/hisys/pci/pcie/debug\n",
                     g_pci_debug[i].name ? : "", g_pci_debug[i].usage ? : "");
    if (ret < 0) {
        oal_free(buf);
        pci_print_log(PCI_LOG_ERR, "log str format err line[%d]\n", __LINE__);
        return;
    }
    printk("%s", (char *)buf);

    oal_free(buf);
}

OAL_STATIC ssize_t oal_pcie_get_debug_info(struct kobject *dev, struct kobj_attribute *attr, char *buf)
{
    int ret;
    int32_t i;
    int32_t count = 0;

    ret = snprintf_s(buf + count, PAGE_SIZE - count, PAGE_SIZE - count - 1, "pci debug cmds:\n");
    if (ret < 0) {
        pci_print_log(PCI_LOG_ERR, "log str format err line[%d]\n", __LINE__);
        return count;
    }
    count += ret;
    for (i = 0; i < oal_array_size(g_pci_debug); i++) {
        ret = snprintf_s(buf + count, PAGE_SIZE - count, PAGE_SIZE - count - 1, "%s\n", g_pci_debug[i].name);
        if (ret < 0) {
            pci_print_log(PCI_LOG_ERR, "log str format err line[%d]\n", __LINE__);
            return count;
        }
        count += ret;
    }
    return count;
}

OAL_STATIC ssize_t oal_pcie_set_debug_info(struct kobject *dev, struct kobj_attribute *attr,
                                           const char *buf, size_t count)
{
    int32_t i;
    oal_pcie_res *pst_pcie_res = oal_get_default_pcie_handler();

    if (buf[count] != '\0') { /* 确保传进来的buf是一个字符串, count不包含结束符 */
        pci_print_log(PCI_LOG_ERR, "invalid pci cmd\n");
        return 0;
    }

    for (i = 0; i < oal_array_size(g_pci_debug); i++) {
        if (g_pci_debug[i].name) {
            if ((count >= OAL_STRLEN(g_pci_debug[i].name)) &&
                !oal_memcmp(g_pci_debug[i].name, buf, OAL_STRLEN(g_pci_debug[i].name))) {
                /* 判断最后一个字符是回车还是空格 */
                char last_c = *(buf + OAL_STRLEN(g_pci_debug[i].name));
                if (last_c == '\n' || last_c == ' ' || last_c == '\0') {
                    break;
                }
            }
        }
    }

    if (i == oal_array_size(g_pci_debug)) {
        pci_print_log(PCI_LOG_ERR, "invalid pci cmd:%s\n", buf);
        oal_pcie_print_debug_usages();
        return count;
    }
#ifdef _PRE_COMMENT_CODE_
    oal_io_print("pcie cmd:%s process\n", g_pci_debug[i].name);
#endif
    buf += OAL_STRLEN(g_pci_debug[i].name);
    if (*buf != '\0') {  // count > OAL_STRLEN(g_pci_debug[i].name)
        buf += 1;        /* EOF */
    }

    for (; *buf == ' '; buf++);

    if (g_pci_debug[i].debug(dev, attr, (const char *)buf, pst_pcie_res) == -OAL_EINVAL) {
        oal_pcie_print_debug_usage(i);
    }
    return count;
}
OAL_STATIC struct kobj_attribute g_dev_attr_debug =
    __ATTR(debug, S_IRUGO | S_IWUSR, oal_pcie_get_debug_info, oal_pcie_set_debug_info);
OAL_STATIC struct attribute *g_hpci_sysfs_entries[] = {
    &g_dev_attr_debug.attr,
    NULL
};

OAL_STATIC struct attribute_group g_hpci_attribute_group = {
    .name = "pcie",
    .attrs = g_hpci_sysfs_entries,
};

OAL_STATIC int32_t oal_pcie_sysfs_init(oal_pcie_res *pst_pcie_res)
{
    int32_t ret;
    oal_kobject *pst_root_object = NULL;

    pst_root_object = oal_get_sysfs_root_object();
    if (pst_root_object == NULL) {
        pci_print_log(PCI_LOG_ERR, "[E]get pci root sysfs object failed!\n");
        return -OAL_EFAIL;
    }

    g_conn_syfs_pci_object = kobject_create_and_add("pci", pst_root_object);
    if (g_conn_syfs_pci_object == NULL) {
        ret = -OAL_ENODEV;
        pci_print_log(PCI_LOG_ERR, "sysfs create kobject_create_and_add pci fail\n");
        goto fail_g_conn_syfs_pci_object;
    }

    ret = oal_debug_sysfs_create_group(g_conn_syfs_pci_object, &g_hpci_attribute_group);
    if (ret) {
        ret = -OAL_ENOMEM;
        pci_print_log(PCI_LOG_ERR, "sysfs create g_hpci_attribute_group group fail.ret=%d\n", ret);
        goto fail_create_pci_group;
    }

    return OAL_SUCC;
fail_create_pci_group:
    kobject_put(g_conn_syfs_pci_object);
    g_conn_syfs_pci_object = NULL;
fail_g_conn_syfs_pci_object:
    return ret;
}

OAL_STATIC int32_t oal_pcie_rx_thread_condtion(oal_atomic *pst_ato)
{
    int32_t ret = oal_atomic_read(pst_ato);
    if (oal_likely(ret == 1)) {
        oal_atomic_set(pst_ato, 0);
    }

    return ret;
}

OAL_STATIC int32_t oal_pcie_rx_hi_thread(void *data)
{
    int32_t ret = OAL_SUCC;
    int32_t supply_num;
    oal_pcie_res *pst_pcie_res = (oal_pcie_res *)data;

    if (oal_warn_on(pst_pcie_res == NULL)) {
        pci_print_log(PCI_LOG_ERR, "%s error: pst_pcie_res is null", __FUNCTION__);
        return -EFAIL;
    };

    allow_signal(SIGTERM);

    forever_loop() {
        if (oal_unlikely(kthread_should_stop())) {
            break;
        }

        ret = oal_wait_event_interruptible_m(pst_pcie_res->st_rx_hi_wq,
                                             oal_pcie_rx_thread_condtion(&pst_pcie_res->rx_hi_cond));
        if (oal_unlikely(ret == -ERESTARTSYS)) {
            pci_print_log(PCI_LOG_INFO, "task %s was interrupted by a signal\n", oal_get_current_task_name());
            break;
        }

        mutex_lock(&pst_pcie_res->st_rx_mem_lock);
        if (oal_unlikely(pst_pcie_res->link_state < PCI_WLAN_LINK_WORK_UP || !pst_pcie_res->regions.inited)) {
            pci_print_log(PCI_LOG_WARN, "hi thread link invaild, stop supply mem, link_state:%s, region:%d",
                          oal_pcie_get_link_state_str(pst_pcie_res->link_state),
                          pst_pcie_res->regions.inited);
        } else {
            supply_num = oal_pcie_rx_ringbuf_supply(pst_pcie_res, OAL_TRUE, OAL_TRUE,
                                                    PCIE_RX_RINGBUF_SUPPLY_ALL,
                                                    GFP_ATOMIC | __GFP_NOWARN, &ret);
            if (ret != OAL_SUCC) {
                /* 补充内存失败，成功则忽略，有可能当前不需要补充内存也视为成功 */
                oal_pcie_shced_rx_normal_thread(pst_pcie_res);
            }
        }
        mutex_unlock(&pst_pcie_res->st_rx_mem_lock);
    }

    return 0;
}

OAL_STATIC int32_t oal_pcie_rx_normal_thread(void *data)
{
    int32_t resched = 0;
    int32_t ret = OAL_SUCC;
    int32_t supply_num;
    oal_pcie_res *pst_pcie_res = (oal_pcie_res *)data;

    if (oal_warn_on(pst_pcie_res == NULL)) {
        pci_print_log(PCI_LOG_ERR, "%s error: pst_pcie_res is null", __FUNCTION__);
        return -EFAIL;
    };

    allow_signal(SIGTERM);

    forever_loop() {
        resched = 0;
        if (oal_unlikely(kthread_should_stop())) {
            break;
        }

        ret = oal_wait_event_interruptible_m(pst_pcie_res->st_rx_normal_wq,
                                             oal_pcie_rx_thread_condtion(&pst_pcie_res->rx_normal_cond));
        if (oal_unlikely(ret == -ERESTARTSYS)) {
            pci_print_log(PCI_LOG_INFO, "task %s was interrupted by a signal\n", oal_get_current_task_name());
            break;
        }

        mutex_lock(&pst_pcie_res->st_rx_mem_lock);
        if (oal_unlikely(pst_pcie_res->link_state < PCI_WLAN_LINK_WORK_UP || !pst_pcie_res->regions.inited)) {
            pci_print_log(PCI_LOG_WARN, "hi thread link invaild, stop supply mem, link_state:%s, region:%d",
                          oal_pcie_get_link_state_str(pst_pcie_res->link_state),
                          pst_pcie_res->regions.inited);
        } else {
            supply_num = oal_pcie_rx_ringbuf_supply(pst_pcie_res, OAL_TRUE, OAL_TRUE,
                                                    PCIE_RX_RINGBUF_SUPPLY_ALL, GFP_KERNEL, &ret);
            if (ret != OAL_SUCC) {
                resched = 1;
            }
        }

        mutex_unlock(&pst_pcie_res->st_rx_mem_lock);

        if (resched) {
            /*
             * 补充内存失败，成功则忽略，有可能当前不需要补充内存也视为成功,
             * 如果GFP_KERNEL 方式补充失败，则启动轮询,循环申请
             */
            oal_schedule();
            oal_pcie_shced_rx_normal_thread(pst_pcie_res);
        }
    }

    return 0;
}

OAL_STATIC void oal_pcie_edma_task_exit(oal_pcie_res *pst_pcie_res)
{
    if (pst_pcie_res->chip_info.edma_support != OAL_TRUE) {
        return;
    }

    if (pst_pcie_res->pst_rx_normal_task != NULL) {
        oal_thread_stop(pst_pcie_res->pst_rx_normal_task, NULL);
    }

    if (pst_pcie_res->pst_rx_hi_task != NULL) {
        oal_thread_stop(pst_pcie_res->pst_rx_hi_task, NULL);
    }
}

OAL_STATIC int32_t oal_pcie_edma_task_init(oal_pcie_res *pst_pcie_res)
{
    if (pst_pcie_res->chip_info.edma_support != OAL_TRUE) {
        return OAL_SUCC;
    }
    oal_wait_queue_init_head(&pst_pcie_res->st_rx_hi_wq);
    oal_wait_queue_init_head(&pst_pcie_res->st_rx_normal_wq);

    oal_atomic_set(&pst_pcie_res->rx_hi_cond, 0);
    oal_atomic_set(&pst_pcie_res->rx_normal_cond, 0);

    /* 高优先级内存用于补充内存 低耗时 */
    pst_pcie_res->pst_rx_hi_task = oal_thread_create(oal_pcie_rx_hi_thread,
                                                     (void *)pst_pcie_res,
                                                     NULL,
                                                     "pci_rx_hi_task",
                                                     SCHED_RR,
                                                     OAL_BUS_TEST_INIT_PRIORITY,
                                                     -1);
    if (pst_pcie_res->pst_rx_hi_task == NULL) {
        pci_print_log(PCI_LOG_ERR, "pcie rx hi task create failed!");
        return -OAL_EFAIL;
    }

    /* 低优先级线程用于补充内存  高耗时，申请不到就轮询 */
    pst_pcie_res->pst_rx_normal_task = oal_thread_create(oal_pcie_rx_normal_thread,
                                                         (void *)pst_pcie_res,
                                                         NULL,
                                                         "pci_rx_normal_task",
                                                         SCHED_NORMAL,
                                                         MIN_NICE,
                                                         -1);
    if (pst_pcie_res->pst_rx_normal_task == NULL) {
        pci_print_log(PCI_LOG_ERR, "pcie rx normal task create failed!");
        oal_pcie_edma_task_exit(pst_pcie_res);
        return -OAL_EFAIL;
    }

    return OAL_SUCC;
}

OAL_STATIC void oal_pcie_sysfs_exit(oal_pcie_res *pst_pcie_res)
{
    if (g_conn_syfs_pci_object == NULL) {
        return;
    }
    oal_debug_sysfs_remove_group(g_conn_syfs_pci_object, &g_hpci_attribute_group);
    kobject_put(g_conn_syfs_pci_object);
    g_conn_syfs_pci_object = NULL;
}

/* 原生dma rx完成MSI中断 */
OAL_STATIC irqreturn_t oal_pcie_edma_rx_intr_status_handler(int irq, void *dev_id)
{
    /* log for msi bbit, del later */
    oal_io_print("pcie_edma_rx_intr_status come, irq:%d\n", irq);
    return IRQ_HANDLED;
}

OAL_STATIC irqreturn_t oal_pcie_edma_tx_intr_status_handler(int irq, void *dev_id)
{
    oal_io_print("pcie_edma_tx_intr_status come, irq:%d\n", irq);
    return IRQ_HANDLED;
}

OAL_STATIC void oal_pcie_hw_edma_intr_status_handler(oal_pcie_res *pst_pci_res)
{
    int32_t trans_cnt = 0;
    int32_t old_cnt;
    MSG_FIFO_STAT msg_fifo_stat;

    forever_loop() {
        if (oal_unlikely(pst_pci_res->link_state < PCI_WLAN_LINK_MEM_UP)) {
            pci_print_log(PCI_LOG_INFO, "link state is disabled:%d, msi can't process!", pst_pci_res->link_state);
            declare_dft_trace_key_info("pcie mem is not init", OAL_DFT_TRACE_OTHER);
        }

        old_cnt = trans_cnt;
        oal_pcie_mips_start(PCIE_MIPS_RX_FIFO_STATUS);
        msg_fifo_stat.AsDword = oal_readl(pst_pci_res->pst_pci_ctrl_base + PCIE_MSG_FIFO_STATUS);
        oal_pcie_mips_end(PCIE_MIPS_RX_FIFO_STATUS);
        pci_print_log(PCI_LOG_DBG, "msg_fifo_stat host:0x%8x", msg_fifo_stat.AsDword);

        if (msg_fifo_stat.bits.rx_msg_fifo0_empty == 0 && msg_fifo_stat.bits.rx_msg_fifo1_empty == 0) {
            oal_pcie_d2h_transfer_done(pst_pci_res);
            trans_cnt++;
        }

        if (msg_fifo_stat.bits.tx_msg_fifo0_empty == 0 && msg_fifo_stat.bits.tx_msg_fifo1_empty == 0) {
            oal_pcie_h2d_transfer_done(pst_pci_res);
            trans_cnt++;
        }

        if (old_cnt == trans_cnt) {
            break;
        }
    }
}

OAL_STATIC irqreturn_t oal_pcie_hw_edma_rx_intr_status_handler(int irq, void *dev_id)
{
    oal_pcie_res *pst_pci_res = (oal_pcie_res *)dev_id;
    pci_print_log(PCI_LOG_DBG, "oal_pcie_hw_edma_rx_intr_status_handler come, irq:%d\n", irq);
    oal_pcie_hw_edma_intr_status_handler(pst_pci_res);
    return IRQ_HANDLED;
}

OAL_STATIC irqreturn_t oal_pcie_hw_edma_tx_intr_status_handler(int irq, void *dev_id)
{
    oal_pcie_res *pst_pci_res = (oal_pcie_res *)dev_id;
    pci_print_log(PCI_LOG_DBG, "oal_pcie_hw_edma_tx_intr_status_handler come, irq:%d\n", irq);
    oal_pcie_hw_edma_intr_status_handler(pst_pci_res);
    return IRQ_HANDLED;
}

irqreturn_t oal_device2host_init_handler(int irq, void *dev_id)
{
    oal_io_print("oal_device2host_init_handler come, irq:%d\n", irq);
    return IRQ_HANDLED;
}

OAL_STATIC irqreturn_t oal_pcie_msg_irq_handler(int irq, void *dev_id)
{
    oal_io_print("oal_pcie_msg_irq_handler come, irq:%d\n", irq);
    return IRQ_HANDLED;
}

/* msi int callback, should move to oal_pcie_host.c */
OAL_STATIC oal_irq_handler_t g_msi_110x_callback[] = {
    oal_pcie_edma_rx_intr_status_handler,
    oal_pcie_edma_tx_intr_status_handler,
    oal_pcie_hw_edma_rx_intr_status_handler,
    oal_pcie_hw_edma_tx_intr_status_handler,
    oal_device2host_init_handler,
    oal_pcie_msg_irq_handler,
};

oal_pcie_res *oal_pcie_host_init(void *data, oal_pcie_msi_stru *pst_msi, uint32_t revision)
{
    int32_t i;
    int32_t ret;
    oal_pcie_res *pst_pci_res = NULL;
    if (g_pci_res) {
        pci_print_log(PCI_LOG_ERR, "PCIe host had already init!\n");
        return NULL;
    }

    if (oal_netbuf_cb_size() < sizeof(pcie_cb_dma_res) + sizeof(struct hcc_tx_cb_stru)) {
        pci_print_log(PCI_LOG_ERR, "pcie cb is too large,[cb %lu < pcie cb %lu + hcc cb %lu]\n",
                      (unsigned long)oal_netbuf_cb_size(),
                      (unsigned long)sizeof(pcie_cb_dma_res),
                      (unsigned long)sizeof(struct hcc_tx_cb_stru));
        return NULL;
    }

    pst_pci_res = (oal_pcie_res *)oal_memalloc(sizeof(oal_pcie_res));
    if (pst_pci_res == NULL) {
        pci_print_log(PCI_LOG_ERR, "alloc pcie res failed, size:%u\n", (uint32_t)sizeof(oal_pcie_res));
        return NULL;
    }

    memset_s((void *)pst_pci_res, sizeof(oal_pcie_res), 0, sizeof(oal_pcie_res));
    pst_pci_res->data = data;
    pst_pci_res->revision = revision;

    pst_msi->func = g_msi_110x_callback;
    pst_msi->msi_num = (int32_t)(OAL_SIZEOF(g_msi_110x_callback) / OAL_SIZEOF(oal_irq_handler_t));

    /* 初始化tx/rx队列 */
    oal_spin_lock_init(&pst_pci_res->st_rx_res.lock);
    oal_netbuf_list_head_init(&pst_pci_res->st_rx_res.rxq);

    for (i = 0; i < PCIE_H2D_QTYPE_BUTT; i++) {
        oal_atomic_set(&pst_pci_res->st_tx_res[i].tx_ringbuf_sync_cond, 0);
        oal_spin_lock_init(&pst_pci_res->st_tx_res[i].lock);
        oal_netbuf_list_head_init(&pst_pci_res->st_tx_res[i].txq);
    }

    oal_spin_lock_init(&pst_pci_res->st_message_res.d2h_res.lock);
    oal_spin_lock_init(&pst_pci_res->st_message_res.h2d_res.lock);

#ifdef _PRE_PLAT_FEATURE_HI110X_PCIE_D2H_BYPASS
    oal_init_completion(&g_d2h_test_done);
#endif

    ret = oal_pcie_sysfs_init(pst_pci_res);
    if (ret != OAL_SUCC) {
        pci_print_log(PCI_LOG_ERR, "oal_pcie_sysfs_init failed, ret=%d\n", ret);
        oal_free(pst_pci_res);
        return NULL;
    }

    ret = oal_pcie_chip_info_init(pst_pci_res);
    if (ret != OAL_SUCC) {
        oal_pcie_sysfs_exit(pst_pci_res);
        oal_free(pst_pci_res);
        return NULL;
    }

    mutex_init(&pst_pci_res->st_rx_mem_lock);

    ret = oal_pcie_edma_task_init(pst_pci_res);
    if (ret != OAL_SUCC) {
        oal_pcie_sysfs_exit(pst_pci_res);
        oal_free(pst_pci_res);
        return NULL;
    }

    oal_ete_host_init(pst_pci_res);

    oal_pcie_change_link_state(pst_pci_res, PCI_WLAN_LINK_UP);

    g_pci_res = pst_pci_res;

    return pst_pci_res;
}

void oal_pcie_host_exit(oal_pcie_res *pst_pci_res)
{
    g_pci_res = NULL;
    oal_ete_host_exit(pst_pci_res);
    oal_pcie_edma_task_exit(pst_pci_res);
    mutex_destroy(&pst_pci_res->st_rx_mem_lock);
    oal_pcie_sysfs_exit(pst_pci_res);
    oal_free(pst_pci_res);
}
#endif
