

#ifdef _PRE_PLAT_FEATURE_HI110X_PCIE
#define HI11XX_LOG_MODULE_NAME     "[PCIEC]"
#define HI11XX_LOG_MODULE_NAME_VAR pciec_loglevel
#define HISI_LOG_TAG               "[PCIEC]"
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

extern void oal_pcie_chip_info_init_hi1103(oal_pcie_res *pst_pci_res);
extern void oal_pcie_chip_info_init_hi1106(oal_pcie_res *pst_pci_res);

int32_t oal_pcie_chip_info_init(oal_pcie_res *pst_pci_res)
{
    oal_pci_dev_stru *pst_pci_dev = pcie_res_to_dev(pst_pci_res);

    size_t len = sizeof(pst_pci_res->chip_info.addr_info);

    (void)memset_s(&pst_pci_res->chip_info.addr_info, len, 0, len);

    switch (pst_pci_dev->device) {
        case PCIE_HISI_DEVICE_ID_HI1106:
            oal_pcie_chip_info_init_hi1106(pst_pci_res);
            break;
        case PCIE_HISI_DEVICE_ID_HI1103:
        case PCIE_HISI_DEVICE_ID_HI1105:
            oal_pcie_chip_info_init_hi1103(pst_pci_res);
            break;

        default:
            return -OAL_ENODEV;
            break;
    }

    return OAL_SUCC;
}

#endif
