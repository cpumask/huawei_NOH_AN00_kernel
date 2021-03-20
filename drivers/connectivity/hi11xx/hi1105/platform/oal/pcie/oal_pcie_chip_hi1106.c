

#ifdef _PRE_PLAT_FEATURE_HI110X_PCIE
#define HI11XX_LOG_MODULE_NAME "[PCIEC]"
#define HISI_LOG_TAG           "[PCIEC]"
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

#include "hi1106/host_ctrl_rb_regs.h"
#include "hi1106/pcie_ctrl_rb_regs.h"

/* HI1106 Registers */
#define HI1106_PA_PCIE0_CTRL_BASE_ADDR      PCIE_CTRL_RB_BASE /* 对应PCIE_CTRL页 */
#define HI1106_PA_PCIE1_CTRL_BASE_ADDR      0x4010E000 /* 对应PCIE_CTRL页 */
#define HI1106_PA_PCIE0_DBI_BASE_ADDRESS    0x40107000
#define HI1106_PA_PCIE1_DBI_BASE_ADDRESS    0x4010D000
#define HI1106_PA_ETE_CTRL_BASE_ADDRESS    HOST_CTRL_RB_BASE
#define HI1106_PA_GLB_CTL_BASE_ADDR        0x40000000
#define HI1106_PA_PMU_CMU_CTL_BASE         0x40002000
#define HI1106_PA_PMU2_CMU_IR_BASE         0x4000E000
#define HI1106_PA_W_CTL_BASE               0x40105000

#define HI1106_DEV_VERSION_CPU_ADDR 0x0000003c

int32_t g_dual_pci_support = OAL_FALSE; /* 0 means don't support */
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
oal_debug_module_param(g_dual_pci_support, int, S_IRUGO | S_IWUSR);
#endif

void oal_pcie_chip_info_init_hi1106(oal_pcie_res *pst_pci_res)
{
    oal_print_hi11xx_log(HI11XX_LOG_INFO, "oal_pcie_chip_info_init_hi1106");
    pst_pci_res->chip_info.dual_pci_support = (uint32_t)g_dual_pci_support;
    pst_pci_res->chip_info.ete_support = OAL_TRUE;
    pst_pci_res->chip_info.membar_support = OAL_TRUE;
    pst_pci_res->chip_info.addr_info.pcie_ctrl = HI1106_PA_PCIE0_CTRL_BASE_ADDR;
    pst_pci_res->chip_info.addr_info.dbi = HI1106_PA_PCIE0_DBI_BASE_ADDRESS;
    pst_pci_res->chip_info.addr_info.ete_ctrl = HI1106_PA_ETE_CTRL_BASE_ADDRESS;
    pst_pci_res->chip_info.addr_info.glb_ctrl = HI1106_PA_GLB_CTL_BASE_ADDR;
    pst_pci_res->chip_info.addr_info.pmu_ctrl = HI1106_PA_PMU_CMU_CTL_BASE;
    pst_pci_res->chip_info.addr_info.pmu2_ctrl = HI1106_PA_PMU2_CMU_IR_BASE;
    pst_pci_res->chip_info.addr_info.boot_version = HI1106_DEV_VERSION_CPU_ADDR;
    pst_pci_res->chip_info.addr_info.sharemem_addr = PCIE_CTRL_RB_HOST_DEVICE_REG1_REG;
}

#endif
