

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

/* HI1103 & HI1105 Registers */
#define HI1103_PA_PCIE_DMA_CTRL_BASE_ADDR  0x40008000 /* 对应PCIE_DMA_CTRL页 */
#define HI1103_PA_PCIE_CTRL_BASE_ADDR      0x40007000 /* 对应PCIE_CTRL页 */
#define HI1103_PA_PCIE_CONFIG_BASE_ADDRESS 0x40102000
#define HI1103_PA_GLB_CTL_BASE_ADDR        0x50000000
#define HI1103_PA_PMU_CMU_CTL_BASE         0x50002000
#define HI1103_PA_PMU2_CMU_IR_BASE         0x50003000
#define HI1103_PA_W_CTL_BASE               0x40000000

#define HI1103_DEV_VERSION_CPU_ADDR       0x00002700
#define HI1103_DEV_VERSION_CPU_ADDR_PIOLT 0x00000180

void oal_pcie_chip_info_init_hi1103(oal_pcie_res *pst_pci_res)
{
    oal_print_hi11xx_log(HI11XX_LOG_INFO, "oal_pcie_chip_info_init_hi1103");
    pst_pci_res->chip_info.edma_support = OAL_TRUE;
    pst_pci_res->chip_info.addr_info.edma_ctrl = HI1103_PA_PCIE_DMA_CTRL_BASE_ADDR;
    pst_pci_res->chip_info.addr_info.pcie_ctrl = HI1103_PA_PCIE_CTRL_BASE_ADDR;
    pst_pci_res->chip_info.addr_info.dbi = HI1103_PA_PCIE_CONFIG_BASE_ADDRESS;
    pst_pci_res->chip_info.addr_info.glb_ctrl = HI1103_PA_GLB_CTL_BASE_ADDR;
    pst_pci_res->chip_info.addr_info.pmu_ctrl = HI1103_PA_PMU_CMU_CTL_BASE;
    pst_pci_res->chip_info.addr_info.pmu2_ctrl = HI1103_PA_PMU2_CMU_IR_BASE;
    pst_pci_res->chip_info.addr_info.sharemem_addr = HI1103_PA_PCIE_CTRL_BASE_ADDR + 0x2e4; /* 0x2e4 device reg1 */
    if (pst_pci_res->revision == PCIE_REVISION_5_00A) {
        pst_pci_res->chip_info.membar_support = OAL_TRUE;
        pst_pci_res->chip_info.addr_info.boot_version = HI1103_DEV_VERSION_CPU_ADDR_PIOLT;
    } else {
        pst_pci_res->chip_info.membar_support = OAL_FALSE;
        pst_pci_res->chip_info.addr_info.boot_version = HI1103_DEV_VERSION_CPU_ADDR;
    }
}
#endif
