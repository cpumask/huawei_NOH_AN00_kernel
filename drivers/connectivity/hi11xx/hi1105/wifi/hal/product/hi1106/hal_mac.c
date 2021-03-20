

#include "hal_mac.h"
#include "oal_pcie_host.h"
#include "oal_util.h"
#include "oal_net.h"
#include "oal_ext_if.h"
#include "hal_ring.h"
#include "securec.h"
#include "hal_device.h"
#include "oal_kernel_file.h"
#include "oal_cfg80211.h"
#include "oam_ext_if.h"
#include "frw_ext_if.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HAL_HOST_MAC_C

#ifdef CONFIG_ARCH_KIRIN_PCIE
oal_void hi1106_get_host_phy_int_status(hal_host_device_stru *pst_hal_device, uint32_t *pul_status)
{
    *pul_status = oal_readl((oal_void*)(uintptr_t)pst_hal_device->phy_regs.irq_status_reg);
}


oal_void hi1106_get_host_phy_int_mask(hal_host_device_stru *hal_device, uint32_t *mask)
{
    *mask = oal_readl((oal_void*)(uintptr_t)hal_device->phy_regs.irq_status_mask_reg);
}


oal_void hi1106_clear_host_phy_int_status(hal_host_device_stru *hal_device, uint32_t status)
{
    oal_writel(status, (uintptr_t)hal_device->phy_regs.irq_clr_reg);
}



oal_void hi1106_get_host_mac_int_status(hal_host_device_stru *hal_device, uint32_t *status)
{
    *status = oal_readl((oal_void*)(uintptr_t)hal_device->mac_regs.irq_status_reg);
}


oal_void hi1106_get_host_mac_int_mask(hal_host_device_stru *hal_device, uint32_t *p_mask)
{
    *p_mask = oal_readl((uintptr_t)hal_device->mac_regs.irq_status_mask_reg);
}


oal_void hi1106_clear_host_mac_int_status(hal_host_device_stru *pst_hal_device, uint32_t status)
{
    oal_writel(status, (uintptr_t)pst_hal_device->mac_regs.irq_clr_reg);
}


oal_void  hi1106_host_mac_irq_unmask(hal_host_device_stru *hal_device, uint32_t irq)
{
    uint32_t intr_mask;

    intr_mask = oal_readl((oal_void*)(uintptr_t)hal_device->mac_regs.irq_status_mask_reg);
    intr_mask &= (~((uint32_t)1 << irq));

    oal_writel(intr_mask, (oal_void*)(uintptr_t)hal_device->mac_regs.irq_status_mask_reg);
}


oal_void  hi1106_host_mac_irq_mask(hal_host_device_stru *hal_device, uint32_t irq)
{
    uint32_t intr_mask;

    intr_mask = oal_readl((oal_void*)(uintptr_t)hal_device->mac_regs.irq_status_mask_reg);
    intr_mask |= (((uint32_t)1 << irq));
    oal_writel(intr_mask, (uintptr_t)hal_device->mac_regs.irq_status_mask_reg);
}

static uint32_t hi1106_regs_addr_transfer(hal_host_device_stru *hal_device, uint32_t reg_addr)
{
    uint64_t host_va = 0;

    if (OAL_SUCC != oal_pcie_devca_to_hostva(0, reg_addr, &host_va)) {
        oam_error_log1(0, OAM_SF_ANY, "{hal_mac_regs_addr_transfer::regaddr[%x] devca2hostva fail.}", reg_addr);
        return OAL_FAIL;
    }

    switch (reg_addr) {
        case HI1106_MAC_HOST_INTR_CLR_REG:
            hal_device->mac_regs.irq_clr_reg = host_va;
            break;
        case HI1106_MAC_HOST_INTR_MASK_REG:
            hal_device->mac_regs.irq_status_mask_reg = host_va;
            break;
        case HI1106_MAC_HOST_INTR_STATUS_REG:
            hal_device->mac_regs.irq_status_reg = host_va;
            break;
        case HI1106_DFX_REG_BANK_CFG_INTR_CLR_REG:
            hal_device->phy_regs.irq_clr_reg = host_va;
            break;
        case HI1106_DFX_REG_BANK_CFG_INTR_MASK_HOST_REG:
            hal_device->phy_regs.irq_status_mask_reg = host_va;
            break;
        case HI1106_DFX_REG_BANK_PHY_INTR_RPT_REG:
            hal_device->phy_regs.irq_status_reg = host_va;
            break;
        default:
            oam_warning_log1(0, OAM_SF_ANY, "{hal_mac_regs_addr_transfer::reg addr[%x] not used.}", reg_addr);
            return OAL_FAIL;
    }

    return OAL_SUCC;
}


int32_t hi1106_host_regs_addr_init(hal_host_device_stru *hal_device)
{
    uint32_t idx;
    uint32_t hal_regs[] = {
        HI1106_MAC_HOST_INTR_CLR_REG,
        HI1106_MAC_HOST_INTR_MASK_REG,
        HI1106_MAC_HOST_INTR_STATUS_REG,
        HI1106_DFX_REG_BANK_CFG_INTR_CLR_REG,
        HI1106_DFX_REG_BANK_CFG_INTR_MASK_HOST_REG,
        HI1106_DFX_REG_BANK_PHY_INTR_RPT_REG,
    };

    for (idx = 0; idx < sizeof(hal_regs) / sizeof(uint32_t); idx++) {
        if (OAL_SUCC != hi1106_regs_addr_transfer(hal_device, hal_regs[idx])) {
            return OAL_FAIL;
        }
    }
    oam_warning_log0(0, OAM_SF_ANY, "{hal_host_regs_addr_init::regs addr trans succ.}");
    return OAL_SUCC;
}

/* 功能描述 : 常收维测信息输出 */
void hi1106_host_al_rx_fcs_info(void)
{
    hal_host_device_stru *hal_device = hal_get_host_device(0);

    oam_warning_log3(0, OAM_SF_CFG, "host_al_rx_fcs_info:packets info, mpdu succ[%d], ampdu succ[%d],fail[%d]",
        hal_device->st_alrx.rx_normal_mdpu_succ_num,
        hal_device->st_alrx.rx_ampdu_succ_num,
        hal_device->st_alrx.rx_ppdu_fail_num);

    hal_device->st_alrx.rx_normal_mdpu_succ_num = 0;
    hal_device->st_alrx.rx_ampdu_succ_num = 0;
    hal_device->st_alrx.rx_ppdu_fail_num = 0;
    return;
}

#endif
