

#define HI11XX_LOG_MODULE_NAME     "[PCIEL]"
#define HI11XX_LOG_MODULE_NAME_VAR pciel_loglevel
#define HISI_LOG_TAG               "[PCIEL]"
#include "oal_util.h"

#include "oal_net.h"
#include "oal_ext_if.h"
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
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
#include <linux/pci.h>
#include "board.h"
#include "plat_pm.h"
#endif
#include "hisi_ini.h"
#include "oal_thread.h"
#include "oam_ext_if.h"

#include "oal_pcie_host.h"
#include "oal_pcie_linux.h"
#include "oal_pcie_pm.h"
#include "oal_hcc_host_if.h"
#include "oal_ete_host.h"

#ifdef CONFIG_KIRIN_PCIE_L1SS_IDLE_SLEEP
#include <linux/hisi/hisi_pcie_idle_sleep.h>
#endif

#ifdef _PRE_PLAT_FEATURE_HI110X_PCIE

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_OAL_PCIE_LINUX_C
OAL_STATIC oal_pcie_linux_res *pcie_linux_res = NULL;

oal_completion g_probe_complete;    /* ��ʼ���ź��� */
OAL_VOLATILE int32_t g_probe_ret; /* probe ����ֵ */

int32_t g_pcie_enum_fail_reg_dump_flag = 0;

/* 1103 MPW2 ��ʹ��INTX �ж� */
OAL_STATIC int32_t g_hipci_msi_enable = 0; /* 0 -intx 1-pci */
OAL_STATIC int32_t g_hipci_gen_select = PCIE_GEN2;
OAL_STATIC int32_t g_ft_pcie_aspm_check_bypass = 0;
OAL_STATIC int32_t g_ft_pcie_gen_check_bypass = 0;
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
oal_debug_module_param(g_hipci_msi_enable, int, S_IRUGO | S_IWUSR);
oal_debug_module_param(g_hipci_gen_select, int, S_IRUGO | S_IWUSR);
oal_debug_module_param(g_ft_pcie_aspm_check_bypass, int, S_IRUGO | S_IWUSR);
oal_debug_module_param(g_ft_pcie_gen_check_bypass, int, S_IRUGO | S_IWUSR);
#endif

#if defined(CONFIG_ARCH_KIRIN_PCIE) || defined(_PRE_CONFIG_ARCH_HI1620S_KUNPENG_PCIE)
int32_t g_kirin_rc_idx = 0;
oal_debug_module_param(g_kirin_rc_idx, int, S_IRUGO | S_IWUSR);
#ifdef _PRE_PLAT_FEATURE_HI110X_DUAL_PCIE
/* ˫PCIE��EP PCIE1��Ӧ��KIRIN PCIE */
int32_t g_kirin_rc_idx_dual = 0;
oal_debug_module_param(g_kirin_rc_idx_dual, int, S_IRUGO | S_IWUSR);
#endif
#endif

OAL_STATIC int32_t g_pcie_print_once = 0;
oal_debug_module_param(g_pcie_print_once, int, S_IRUGO | S_IWUSR);

OAL_STATIC int32_t g_pcie_aspm_enable = 1;
oal_debug_module_param(g_pcie_aspm_enable, int, S_IRUGO | S_IWUSR);

OAL_STATIC int32_t g_pcie_performance_mode = 0;
oal_debug_module_param(g_pcie_performance_mode, int, S_IRUGO | S_IWUSR);

/* WCPUоƬ���ѽ׶��Ƿ��Զ��ر�L1���͵����ӳ� */
OAL_STATIC int32_t g_pcie_auto_disable_aspm = 0;
oal_debug_module_param(g_pcie_auto_disable_aspm, int, S_IRUGO | S_IWUSR);

OAL_STATIC int32_t g_pcie_shutdown_panic = 0;
#ifdef PLATFORM_DEBUG_ENABLE
module_param(g_pcie_shutdown_panic, int, S_IRUGO | S_IWUSR);
#endif
/* Function Declare */
OAL_STATIC int32_t oal_pcie_probe(oal_pci_dev_stru *pst_pci_dev, OAL_CONST oal_pci_device_id_stru *pst_id);
OAL_STATIC hcc_bus *oal_pcie_bus_init(oal_pcie_linux_res *pst_pci_lres);
OAL_STATIC void oal_pcie_bus_exit(hcc_bus *pst_bus);
OAL_STATIC int32_t oal_enable_pcie_irq(oal_pcie_linux_res *pst_pci_lres);
OAL_STATIC int32_t oal_pcie_host_ip_init(oal_pcie_linux_res *pst_pci_lres);
OAL_STATIC int32_t oal_pcie_enable_device_func(oal_pcie_linux_res *pst_pci_lres);
OAL_STATIC int32_t oal_pcie_shutdown_pre_respone(void *data);

#ifdef _PRE_PLAT_FEATURE_HI110X_DUAL_PCIE
OAL_STATIC int32_t oal_pcie_dual_power_on_callback(void *data);
OAL_STATIC int32_t oal_pcie_dual_power_off_fail_callback(void *data);
OAL_STATIC int32_t wlan_dual_first_power_off_callback(void *data);
#endif

OAL_STATIC OAL_CONST oal_pci_device_id_stru g_pcie_id_tab[] = {
    { PCIE_HISI_VENDOR_ID, PCIE_HISI_DEVICE_ID_HI1103, PCI_ANY_ID, PCI_ANY_ID }, /* 1103 PCIE */
    { PCIE_HISI_VENDOR_ID, PCIE_HISI_DEVICE_ID_HI1105, PCI_ANY_ID, PCI_ANY_ID }, /* 1105 PCIE */
    { PCIE_HISI_VENDOR_ID, PCIE_HISI_DEVICE_ID_HI1106, PCI_ANY_ID, PCI_ANY_ID }, /* 1106 PCIE */
    { 0, }
};

int32_t oal_pcie_set_loglevel(int32_t loglevel)
{
    int32_t ret = g_hipcie_loglevel;
    g_hipcie_loglevel = loglevel;
    return ret;
}

int32_t oal_pcie_set_hi11xx_loglevel(int32_t loglevel)
{
    int32_t ret = HI11XX_LOG_MODULE_NAME_VAR;
    HI11XX_LOG_MODULE_NAME_VAR = loglevel;
    return ret;
}

int32_t oal_pcie_enumerate(int32_t rc_idx)
{
    int32_t ret = -OAL_ENODEV;
#if defined(CONFIG_ARCH_KIRIN_PCIE)
    oal_io_print("notify host to scan rc:%d\n", rc_idx);
    ret = kirin_pcie_enumerate(rc_idx);
#endif
    return ret;
}

OAL_STATIC int32_t oal_pcie_enable_single_device(oal_pci_dev_stru *pst_pcie_dev, struct pci_saved_state **default_state)
{
    u16 old_cmd = 0;
    int32_t ret;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 0, 0))

    if (oal_warn_on(*default_state == NULL)) {
        oal_io_print("pcie had't any saved state!\n");
        return -OAL_ENODEV;
    }

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 14, 0))
    /*
     * Updated with pci_load_and_free_saved_state to compatible
     * with kernel 3.14 or higher
     */
    pci_load_and_free_saved_state(pst_pcie_dev, default_state);

    /* Update default state */
    *default_state = pci_store_saved_state(pst_pcie_dev);
#else
    pci_load_saved_state(pst_pcie_dev, *default_state);
#endif /* (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 14, 0)) */
#endif /* (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 0, 0)) */

    pci_restore_state(pst_pcie_dev);

    ret = oal_pci_enable_device(pst_pcie_dev);
    if (ret) {
        oal_io_print("enable device failed!ret=%d\n", ret);
        pci_disable_device(pst_pcie_dev);
    } else {
        pci_set_master(pst_pcie_dev);
    }

    pci_read_config_word(pst_pcie_dev, PCI_COMMAND, &old_cmd);
    oal_io_print("PCI_COMMAND:0x%x\n", old_cmd);

    return ret;
}

OAL_STATIC int32_t oal_pcie_save_resource(oal_pci_dev_stru *pst_pcie_dev, struct pci_saved_state **default_state)
{
    int32_t ret = OAL_SUCC;

    pci_save_state(pst_pcie_dev);
    oal_io_print("save resource\n");
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 0, 0))
    if (*default_state != NULL) {
        /* There is already exist pcie state */
        struct pci_saved_state *pst_state = *default_state;
        *default_state = NULL;
        kfree(pst_state);
        oal_io_print("default state already exist, free first\n");
    }

    *default_state = pci_store_saved_state(pst_pcie_dev);
    if (oal_unlikely(*default_state == NULL)) {
        oal_pci_disable_device(pst_pcie_dev);
        return -OAL_ENOMEM;
    }
#endif
    return ret;
}

OAL_STATIC int32_t oal_pcie_enable_device(oal_pcie_linux_res *pst_pci_lres)
{
    oal_pci_dev_stru *pst_pcie_dev = pst_pci_lres->pst_pcie_dev;
    if (oal_warn_on(pst_pci_lres == NULL)) {
        return -OAL_ENOMEM;
    }

    if (oal_warn_on(pst_pcie_dev == NULL)) {
        return -OAL_ENODEV;
    }
    return oal_pcie_enable_single_device(pst_pcie_dev, &pst_pci_lres->default_state);
}

OAL_STATIC void oal_pcie_disable_device(oal_pcie_linux_res *pst_pci_lres)
{
    if (oal_warn_on(pst_pci_lres == NULL)) {
        return;
    }

    if (oal_warn_on(pst_pci_lres->pst_pcie_dev == NULL)) {
        return;
    }

    oal_pci_disable_device(pst_pci_lres->pst_pcie_dev);
}

OAL_STATIC int32_t oal_pcie_save_default_resource(oal_pcie_linux_res *pst_pci_lres)
{
    if (oal_warn_on(pst_pci_lres == NULL)) {
        return -OAL_EINVAL;
    }

    if (oal_warn_on(pst_pci_lres->pst_pcie_dev == NULL)) {
        return -OAL_ENODEV;
    }

    return oal_pcie_save_resource(pst_pci_lres->pst_pcie_dev, &pst_pci_lres->default_state);
}

#ifdef _PRE_PLAT_FEATURE_HI110X_DUAL_PCIE
int32_t oal_pcie_enable_dual_device(oal_pcie_linux_res *pst_pci_lres)
{
    oal_pci_dev_stru *pst_pcie_dev = pst_pci_lres->pst_pcie_dev_dual;
    if (oal_warn_on(pst_pci_lres == NULL)) {
        return -OAL_ENOMEM;
    }

    if (oal_warn_on(pst_pcie_dev == NULL)) {
        return -OAL_ENODEV;
    }

    return oal_pcie_enable_single_device(pst_pcie_dev, &pst_pci_lres->default_state_dual);
}

void oal_pcie_disable_dual_device(oal_pcie_linux_res *pst_pci_lres)
{
    if (oal_warn_on(pst_pci_lres == NULL)) {
        return;
    }

    if (oal_warn_on(pst_pci_lres->pst_pcie_dev_dual == NULL)) {
        return;
    }

    oal_pci_disable_device(pst_pci_lres->pst_pcie_dev_dual);
}

int32_t oal_pcie_save_default_dual_resource(oal_pcie_linux_res *pst_pci_lres)
{
    if (oal_warn_on(pst_pci_lres == NULL)) {
        return -OAL_EINVAL;
    }

    if (oal_warn_on(pst_pci_lres->pst_pcie_dev_dual == NULL)) {
        return -OAL_ENODEV;
    }

    return oal_pcie_save_resource(pst_pci_lres->pst_pcie_dev_dual, &pst_pci_lres->default_state_dual);
}
#endif

OAL_STATIC irqreturn_t oal_pcie_intx_edma_isr(int irq, void *dev_id)
{
    /*
     * �жϴ�������̫�࣬Ŀǰ�޷����ƣ���Ϊ�ж���Ҫÿ�ζ��� ���������ж϶��Ļ���
     * Ҫ����ס�жϣ������жϻ��
     */
    oal_pcie_linux_res *pst_pci_lres = (oal_pcie_linux_res *)dev_id;
    if (oal_unlikely(oal_pcie_transfer_done(pst_pci_lres->pst_pci_res) < 0)) {
        pci_print_log(PCI_LOG_ERR, "froce to disable pcie intx");
        declare_dft_trace_key_info("transfer_done_fail", OAL_DFT_TRACE_EXCEP);
        oal_disable_pcie_irq(pst_pci_lres);

        /* DFR trigger */
        oal_pcie_change_link_state(pst_pci_lres->pst_pci_res, PCI_WLAN_LINK_DOWN);
        hcc_bus_exception_submit(pst_pci_lres->pst_bus, WIFI_TRANS_FAIL);

        chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_PLAT, CHR_LAYER_DRV,
                             CHR_PLT_DRV_EVENT_PCIE, CHR_PLAT_DRV_ERROR_INTX_ISR_PCIE_LINK_DOWN);
        /* �رյ͹��� */
    }
    return IRQ_HANDLED;
}

irqreturn_t oal_pcie_intx_ete_isr(int irq, void *dev_id)
{
    /*
     * �жϴ�������̫�࣬Ŀǰ�޷����ƣ���Ϊ�ж���Ҫÿ�ζ��� ���������ж϶��Ļ���
     * Ҫ����ס�жϣ������жϻ��
     */
    oal_pcie_linux_res *pst_pci_lres = (oal_pcie_linux_res *)dev_id;
    if (oal_unlikely(oal_pcie_transfer_ete_done(pst_pci_lres->pst_pci_res) < 0)) {
        pci_print_log(PCI_LOG_ERR, "froce to disable pcie intx");
        declare_dft_trace_key_info("transfer_done_fail", OAL_DFT_TRACE_EXCEP);
        oal_disable_pcie_irq(pst_pci_lres);

        /* DFR trigger */
        oal_pcie_change_link_state(pst_pci_lres->pst_pci_res, PCI_WLAN_LINK_DOWN);
        hcc_bus_exception_submit(pst_pci_lres->pst_bus, WIFI_TRANS_FAIL);

        chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_PLAT, CHR_LAYER_DRV,
                             CHR_PLT_DRV_EVENT_PCIE, CHR_PLAT_DRV_ERROR_INTX_ISR_PCIE_LINK_DOWN);
        /* �رյ͹��� */
    }
    return IRQ_HANDLED;
}

OAL_STATIC void oal_pcie_intx_task(uintptr_t data)
{
    oal_pcie_linux_res *pst_pci_lres = (oal_pcie_linux_res *)data;
    oal_pcie_mips_start(PCIE_MIPS_HCC_RX_TOTAL);
    if (oal_pcie_transfer_done(pst_pci_lres->pst_pci_res)) {
        /* ��0˵������Ҫ���� */
        oal_pcie_mips_end(PCIE_MIPS_HCC_RX_TOTAL);
        oal_task_sched(&pst_pci_lres->st_rx_task);
    }
    oal_pcie_mips_end(PCIE_MIPS_HCC_RX_TOTAL);
}

#ifndef _PRE_HI375X_PCIE
#ifdef CONFIG_ARCH_KIRIN_PCIE
OAL_STATIC void oal_pcie_linkdown_callback(struct kirin_pcie_notify *noti)
{
    oal_pcie_linux_res *pst_pci_lres = NULL;
    oal_pci_dev_stru *pst_pci_dev = (oal_pci_dev_stru *)noti->user;
    if (pst_pci_dev == NULL) {
        oal_io_print(KERN_ERR "pcie linkdown, pci dev is null!!\n");
        return;
    }

    pst_pci_lres = (oal_pcie_linux_res *)oal_pci_get_drvdata(pst_pci_dev);
    if (pst_pci_lres == NULL) {
        oal_io_print(KERN_ERR "pcie linkdown, lres is null\n");
        return;
    }

    if (pst_pci_lres->pst_bus == NULL) {
        oal_io_print(KERN_ERR "pcie linkdown, pst_bus is null\n");
        return;
    }

    oal_io_print(KERN_ERR "pcie dev[%s] [%d:%d] linkdown\n",
                 dev_name(&pst_pci_dev->dev), pst_pci_dev->vendor, pst_pci_dev->device);
    declare_dft_trace_key_info("pcie_link_down", OAL_DFT_TRACE_EXCEP);

    oal_pcie_change_link_state(pst_pci_lres->pst_pci_res, PCI_WLAN_LINK_DOWN);

    if (board_get_wlan_wkup_gpio_val() == 0) {
        hcc_bus_exception_submit(pst_pci_lres->pst_bus, WIFI_TRANS_FAIL);
#ifdef CONFIG_HUAWEI_DSM
        hw_110x_dsm_client_notify(SYSTEM_TYPE_PLATFORM, DSM_WIFI_PCIE_LINKDOWN, "%s: pcie linkdown\n", __FUNCTION__);
#endif
        chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_PLAT, CHR_LAYER_DRV,
                             CHR_PLT_DRV_EVENT_PCIE, CHR_PLAT_DRV_ERROR_WKUP_GPIO_PCIE_LINK_DOWN);
    } else {
        pci_print_log(PCI_LOG_INFO, "dev wakeup gpio is high, dev maybe panic");
    }
}
#endif
#endif

OAL_STATIC void oal_pcie_unregister_msi_by_index(oal_pcie_linux_res *pst_pci_lres, int32_t range)
{
    int32_t i, j;
    oal_pci_dev_stru *pst_pci_dev;
    pst_pci_dev = pst_pci_lres->pst_pcie_dev;

    j = (range > pst_pci_lres->st_msi.msi_num) ? pst_pci_lres->st_msi.msi_num : range;

    if (pst_pci_lres->st_msi.func == NULL) {
        return;
    }

    for (i = 0; i < j; i++) {
        oal_irq_handler_t msi_handler = pst_pci_lres->st_msi.func[i];
        if (msi_handler != NULL) {
            free_irq(pst_pci_dev->irq + i, (void *)pst_pci_lres->pst_pci_res);
        }
    }
}

OAL_STATIC void oal_pcie_unregister_msi(oal_pcie_linux_res *pst_pci_lres)
{
    oal_pcie_unregister_msi_by_index(pst_pci_lres, pst_pci_lres->st_msi.msi_num);
}

OAL_STATIC int32_t oal_pcie_register_msi(oal_pcie_linux_res *pst_pci_lres)
{
    int32_t i;
    int32_t ret;
    oal_pci_dev_stru *pst_pci_dev = NULL;

    if (pst_pci_lres->st_msi.msi_num <= 0) {
        return -OAL_ENODEV;
    }

    if (pst_pci_lres->st_msi.func == NULL) {
        return -OAL_ENODEV;
    }

    pst_pci_dev = pst_pci_lres->pst_pcie_dev;

    for (i = 0; i < pst_pci_lres->st_msi.msi_num; i++) {
        oal_irq_handler_t msi_handler = pst_pci_lres->st_msi.func[i];
        if (msi_handler != NULL) {
            pci_print_log(PCI_LOG_DBG, "try to request msi irq:%d", pst_pci_dev->irq + i);
            ret = request_irq(pst_pci_dev->irq + i, msi_handler, IRQF_SHARED,
                              "hisi_pci_msi", (void *)pst_pci_lres);
            if (ret) {
                oal_io_print("msi request irq failed, base irq:%u, msi index:%d, ret=%d\n", pst_pci_dev->irq, i, ret);
                goto failed_request_msi;
            }
        }
    }

    return OAL_SUCC;
failed_request_msi:
    oal_pcie_unregister_msi_by_index(pst_pci_lres, i);
    return ret;
}

#ifdef CONFIG_ARCH_KIRIN_PCIE
OAL_STATIC void oal_kirin_pcie_deregister_event(oal_pcie_linux_res *pst_pci_lres)
{
#ifndef _PRE_HI375X_PCIE
    kirin_pcie_deregister_event(&pst_pci_lres->pcie_event);
#endif
    return;
}

OAL_STATIC void oal_kirin_pcie_register_event(oal_pci_dev_stru *pst_pci_dev, oal_pcie_linux_res *pst_pci_lres)
{
#ifndef _PRE_HI375X_PCIE
    pst_pci_lres->pcie_event.events = KIRIN_PCIE_EVENT_LINKDOWN;
    pst_pci_lres->pcie_event.user = pst_pci_dev;
    pst_pci_lres->pcie_event.mode = KIRIN_PCIE_TRIGGER_CALLBACK;
    pst_pci_lres->pcie_event.callback = oal_pcie_linkdown_callback;
    kirin_pcie_register_event(&pst_pci_lres->pcie_event);
#endif
    return;
}
#endif

OAL_STATIC int32_t oal_pcie_register_int(oal_pci_dev_stru *pst_pci_dev, oal_pcie_linux_res *pst_pci_lres)
{
    int32_t ret = -OAL_EFAIL;

    /* interrupt register */
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    oal_io_print("raw irq: %d\n", pst_pci_dev->irq);

    /* Read from DTS later */
    pst_pci_lres->st_msi.is_msi_support = g_hipci_msi_enable;

    if (pst_pci_lres->st_msi.is_msi_support) {
        /* Try to enable msi */
        ret = pci_enable_msi(pst_pci_dev);
        if (ret > 0) {
            /* ep support msi */
            oal_io_print("msi irq:%d, msi int nums:%d\n", pst_pci_dev->irq, ret);
            ret = oal_pcie_register_msi(pst_pci_lres);
            if (ret != OAL_SUCC) {
                goto failed_register_msi;
            } else {
                pst_pci_lres->irq_stat = 0;
            }
        } else {
            /* non msi drivers */
            oal_io_print("msi request failed, disable msi... ret=%d\n", ret);
            pst_pci_lres->st_msi.is_msi_support = 0;
        }
    }

    /* Try to register intx */
    if (!pst_pci_lres->st_msi.is_msi_support) {
        if (pst_pci_lres->pst_pci_res->chip_info.edma_support == OAL_TRUE) {
            ret = request_irq(pst_pci_dev->irq, oal_pcie_intx_edma_isr, IRQF_SHARED,
                              "hisi_edma_intx", (void *)pst_pci_lres);
        }
        if (pst_pci_lres->pst_pci_res->chip_info.ete_support == OAL_TRUE) {
            ret = request_irq(pst_pci_dev->irq, oal_pcie_intx_ete_isr, IRQF_SHARED,
                              "hisi_ete_intx", (void *)pst_pci_lres);
        }

        if (ret < 0) {
            oal_io_print("request intx failed, ret=%d\n", ret);
            goto failed_pci_intx_request;
        }

        pst_pci_lres->irq_stat = 0;
    }

#endif

    oal_io_print("request pcie intx irq %d succ\n", pst_pci_dev->irq);
    return OAL_SUCC;

    /* fail process */
failed_pci_intx_request:
    if (pst_pci_lres->st_msi.is_msi_support) {
        oal_pcie_unregister_msi(pst_pci_lres);
    }
failed_register_msi:
    return ret;
}

#ifdef _PRE_PLAT_FEATURE_HI110X_DUAL_PCIE
OAL_STATIC int32_t oal_pcie_dual_pci_init(oal_pci_dev_stru *pst_pci_dev, oal_pcie_linux_res *pst_pci_lres)
{
    int32_t ret;
    if (pst_pci_lres->pst_pci_res == NULL) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "pst_pci_res is null");
        return -OAL_ENODEV;
    }

    ret = oal_pci_enable_device(pst_pci_dev);
    if (ret) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "enable dual pcie failed, ret=%d", ret);
        return ret;
    }

    ret = oal_pcie_dual_bar_init(pst_pci_lres->pst_pci_res, pst_pci_dev);
    if (ret != OAL_SUCC) {
        return ret;
    }
    ret = oal_pcie_bar1_init(&pst_pci_lres->pst_pci_res->st_iatu_dual_bar);
    if (ret != OAL_SUCC) {
        return ret;
    }
    ret = oal_pcie_save_default_dual_resource(pst_pci_lres);
    if (ret != OAL_SUCC) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "save dual resource failed, ret=%d", ret);
        oal_pcie_bar1_exit(&pst_pci_lres->pst_pci_res->st_iatu_dual_bar);
        return ret;
    }
    return ret;
}

OAL_STATIC void oal_pcie_dual_pci_exit(oal_pcie_linux_res *pst_pci_lres)
{
    if (pst_pci_lres->pst_pci_res == NULL) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "pst_pci_res is null");
        return;
    }
    oal_pcie_bar1_exit(&pst_pci_lres->pst_pci_res->st_iatu_dual_bar);
}

OAL_STATIC int32_t oal_pcie_dual_pci_probe(oal_pci_dev_stru *pst_pci_dev, OAL_CONST oal_pci_device_id_stru *pst_id)
{
    int32_t ret;
    uint32_t dword = 0;
    if (g_dual_pci_support == OAL_FALSE) {
        if (pcie_linux_res) {
            oal_print_hi11xx_log(HI11XX_LOG_ERR, "pcie device init failed, already init!\n");
            return -OAL_EBUSY;
        }
    } else {
        /* dual pcie, pcie0&pcie1 init flow */
        ret = oal_pci_read_config_dword(pst_pci_dev, PCI_EXP_DEVICE2_DUAL_FLAG, &dword);
        if (ret) {
            oal_print_hi11xx_log(HI11XX_LOG_ERR, "read dual flag failed, ret=%d", ret);
            return ret;
        }

        oal_print_hi11xx_log(HI11XX_LOG_INFO, "dual pcie flag: 0x%x", dword);

        if (dword == PCIE_DUAL_PCI_MASTER_FLAG) {
            /* pcie0 */
            if (pcie_linux_res != NULL) {
                oal_print_hi11xx_log(HI11XX_LOG_ERR, "dual pcie must probe master ip first!");
                return -OAL_EBUSY;
            }
        }

        if (dword == PCIE_DUAL_PCI_SLAVE_FLAG) {
            /* pcie1 */
            if (pcie_linux_res == NULL) {
                oal_print_hi11xx_log(HI11XX_LOG_ERR, "dual pcie must probe master ip first!");
                return -OAL_EBUSY;
            } else {
                pcie_linux_res->pst_pcie_dev_dual = pst_pci_dev;
                pci_set_drvdata(pst_pci_dev, pcie_linux_res);
                /* dual  pcie probe init */
                ret = oal_pcie_dual_pci_init(pst_pci_dev, pcie_linux_res);
                if (ret != OAL_SUCC) {
                    return ret;
                }
            }
        }
    }

    return OAL_SUCC;
}
#else
OAL_STATIC int32_t oal_pcie_dual_pci_probe(oal_pci_dev_stru *pst_pci_dev, OAL_CONST oal_pci_device_id_stru *pst_id)
{
    if (pcie_linux_res) {
        oal_io_print("pcie device init failed, already init!\n");
        return -OAL_EBUSY;
    }
    return OAL_SUCC;
}
#endif

/* ̽�⵽һ��PCIE�豸, probe �������ܻᴥ����� */
OAL_STATIC int32_t oal_pcie_probe(oal_pci_dev_stru *pst_pci_dev, OAL_CONST oal_pci_device_id_stru *pst_id)
{
    uint8_t reg8 = 0;
    int32_t ret = -OAL_EFAIL;
    unsigned short device_id;
    hcc_bus *pst_bus = NULL;
    oal_pcie_linux_res *pst_pci_lres = NULL;

    if (oal_any_null_ptr2(pst_pci_dev, pst_id)) {
        /* never touch here */
        oal_io_print("oal_pcie_probe failed, pst_pci_dev:%p, pst_id:%p\n", pst_pci_dev, pst_id);
        g_probe_ret = -OAL_EIO;
        oal_complete(&g_probe_complete);
        return -OAL_EIO;
    }

    device_id = oal_pci_get_dev_id(pst_pci_dev);

    /* �豸ID �� ��ƷID */
    oal_print_hi11xx_log(HI11XX_LOG_DBG,
                         "[PCIe][%s]devfn:0x%x , vendor:0x%x , device:0x%x , subsystem_vendor:0x%x , subsystem_device:0x%x , class:0x%x \n",
                         dev_name(&pst_pci_dev->dev),
                         pst_pci_dev->devfn,
                         pst_pci_dev->vendor,
                         pst_pci_dev->device,
                         pst_pci_dev->subsystem_vendor,
                         pst_pci_dev->subsystem_device,
                         pst_pci_dev->class);

    ret = oal_pcie_dual_pci_probe(pst_pci_dev, pst_id);
    if (ret != OAL_SUCC) {
        g_probe_ret = ret;
        oal_complete(&g_probe_complete);
        return ret;
    }
    
    ret = oal_pci_enable_device(pst_pci_dev);
    if (ret) {
        oal_io_print("pci: pci_enable_device error ret=%d\n", ret);
        g_probe_ret = ret;
        oal_complete(&g_probe_complete);
        return ret;
    }

    /* If slave pcie, end the probe */
    if (pcie_linux_res != NULL) {
        g_probe_ret = OAL_SUCC;
        oal_complete(&g_probe_complete);
        return OAL_SUCC;
    }

    /* alloc pcie resources */
    pst_pci_lres = (oal_pcie_linux_res *)oal_memalloc(sizeof(oal_pcie_linux_res));
    if (pst_pci_lres == NULL) {
        ret = -OAL_ENOMEM;
        oal_io_print("%s alloc res failed,size:%lu\n", __FUNCTION__, (unsigned long)sizeof(oal_pcie_linux_res));
        goto failed_res_alloc;
    }

    memset_s((void *)pst_pci_lres, sizeof(oal_pcie_linux_res), 0, sizeof(oal_pcie_linux_res));

    pst_pci_lres->pst_pcie_dev = pst_pci_dev;

    pci_set_drvdata(pst_pci_dev, pst_pci_lres);

    oal_init_completion(&pst_pci_lres->st_pcie_ready);
    oal_atomic_set(&pst_pci_lres->st_pcie_wakeup_flag, 0);

    oal_task_init(&pst_pci_lres->st_rx_task, oal_pcie_intx_task, (uintptr_t)pst_pci_lres);

    oal_spin_lock_init(&pst_pci_lres->st_lock);

    oal_init_completion(&pst_pci_lres->st_pcie_shutdown_response);

    oal_wake_lock_init(&pst_pci_lres->st_sr_bugfix, "pcie_sr_bugfix");

    pst_pci_lres->version = (pst_pci_dev->vendor) | (pst_pci_dev->device << 16);

    ret = oal_pci_read_config_byte(pst_pci_dev, PCI_REVISION_ID, &reg8);
    if (ret) {
        oal_io_print("pci: read revision id  failed, ret=%d\n", ret);
        ret = -OAL_ENODEV;
        goto failed_pci_host_init;
    }

    pst_pci_lres->revision = (uint32_t)reg8;

    pci_print_log(PCI_LOG_INFO, "wifi pcie version:0x%8x, revision:%d", pst_pci_lres->version, pst_pci_lres->revision);

    /* soft resource init */
    pst_pci_lres->pst_pci_res = oal_pcie_host_init((void *)pst_pci_dev, &pst_pci_lres->st_msi, pst_pci_lres->revision);
    if (pst_pci_lres->pst_pci_res == NULL) {
        ret = -OAL_ENODEV;
        oal_io_print("pci: oal_pcie_host_init failed\n");
        goto failed_pci_host_init;
    }

    pst_bus = oal_pcie_bus_init(pst_pci_lres);
    if (pst_bus == NULL) {
        goto failed_pci_bus_init;
    }

    hcc_bus_message_register(pst_bus, D2H_MSG_SHUTDOWN_IP_PRE_RESPONSE,
                             oal_pcie_shutdown_pre_respone,
                             (void *)pst_pci_lres);

    /* Ӳ���豸��Դ��ʼ��, 5610+1103 FPGA û�����µ�ӿ� */
    ret = oal_pcie_dev_init(pst_pci_lres->pst_pci_res);
    if (ret != OAL_SUCC) {
        goto failed_pci_dev_init;
    }

    ret = oal_pcie_register_int(pst_pci_dev, pst_pci_lres);
    if (ret != OAL_SUCC) {
        goto failed_register_msi;
    }

#ifdef CONFIG_ARCH_KIRIN_PCIE
    oal_kirin_pcie_register_event(pst_pci_dev, pst_pci_lres);
#endif /* KIRIN_PCIE_LINKDOWN_RECOVERY */

    pcie_linux_res = pst_pci_lres;
    g_probe_ret = ret;
    oal_complete(&g_probe_complete);

    return ret;
failed_register_msi:
    oal_pcie_dev_deinit(pst_pci_lres->pst_pci_res);

failed_pci_dev_init:
    oal_pcie_bus_exit(pst_bus);
failed_pci_bus_init:
    oal_pcie_host_exit(pst_pci_lres->pst_pci_res);
failed_pci_host_init:
    pci_set_drvdata(pst_pci_dev, NULL);
    oal_wake_lock_exit(&pst_pci_lres->st_sr_bugfix);
    oal_free(pst_pci_lres);
failed_res_alloc:
    oal_pci_disable_device(pst_pci_dev);
    g_probe_ret = ret;
    oal_complete(&g_probe_complete);
    return ret;
}

OAL_STATIC void oal_pcie_remove(oal_pci_dev_stru *pst_pci_dev)
{
    unsigned short device_id;
    oal_pcie_linux_res *pst_pci_lres = NULL;

    device_id = oal_pci_get_dev_id(pst_pci_dev);

    oal_io_print("pcie driver remove 0x%x, name:%s\n", device_id, dev_name(&pst_pci_dev->dev));

    pst_pci_lres = (oal_pcie_linux_res *)oal_pci_get_drvdata(pst_pci_dev);
    if (pst_pci_lres == NULL) {
        oal_io_print("oal_pcie_remove lres is null\n");
        return;
    }

    pcie_linux_res = NULL;

#ifdef CONFIG_ARCH_KIRIN_PCIE
    oal_kirin_pcie_deregister_event(pst_pci_lres);
#endif

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    if (pst_pci_lres->st_msi.is_msi_support) {
        oal_pcie_unregister_msi(pst_pci_lres);
    } else {
        free_irq(pst_pci_dev->irq, pst_pci_lres);
    }

#endif
#ifdef _PRE_PLAT_FEATURE_HI110X_DUAL_PCIE
    oal_pcie_dual_pci_exit(pst_pci_lres);
#endif
    oal_wake_lock_exit(&pst_pci_lres->st_sr_bugfix);

    oal_task_kill(&pst_pci_lres->st_rx_task);

    oal_pcie_dev_deinit(pst_pci_lres->pst_pci_res);

    oal_pcie_host_exit(pst_pci_lres->pst_pci_res);

    pst_pci_lres->pst_pcie_dev = NULL;
    oal_free(pst_pci_lres);

    oal_pci_disable_device(pst_pci_dev);
}

#if defined(CONFIG_ARCH_KIRIN_PCIE) || defined(_PRE_CONFIG_ARCH_HI1620S_KUNPENG_PCIE)
OAL_STATIC int32_t oal_pcie_device_wakeup_handler(const void *data)
{
    oal_reference(data);
    /* ���ﱣ֤�⸴λEP������ʱefuse�Ѿ��ȶ� */
    board_host_wakeup_dev_set(1);
    pci_print_log(PCI_LOG_INFO, "pcie wakeup device control, pull up gpio");
    return 0;
}
#endif
#ifdef CONFIG_ARCH_KIRIN_PCIE
OAL_STATIC int32_t oal_pcie_device_wake_powerup_handler(void *data)
{
    uint32_t ul_ret;
    uint32_t ul_retry_cnt = 0;
    const uint32_t ul_max_retry_cnt = 3;
    struct wlan_pm_s *pst_wlan_pm = wlan_pm_get_drv();

    oal_reference(data);
    declare_dft_trace_key_info("pcie_resume_powerup", OAL_DFT_TRACE_SUCC);
retry:
    pci_print_log(PCI_LOG_INFO, "pcie_resume_powerup pull up gpio,ul_retry_cnt=%u", ul_retry_cnt);
    if (pst_wlan_pm != NULL) {
        oal_init_completion(&pst_wlan_pm->st_wifi_powerup_done);
    }

    oal_atomic_set(&g_bus_powerup_dev_wait_ack, 1);
    /* ���ﱣ֤�⸴λEP������ʱefuse�Ѿ��ȶ� */
    board_host_wakeup_dev_set(1);
    if (pst_wlan_pm != NULL) {
        ul_ret = oal_wait_for_completion_timeout(&pst_wlan_pm->st_wifi_powerup_done,
                                                 (uint32_t)oal_msecs_to_jiffies(2000));
        if (oal_unlikely(ul_ret == 0)) {
            /* ��ʱ��������������Խ��� */
            declare_dft_trace_key_info("pcie_resume_powerup ack timeout", OAL_DFT_TRACE_FAIL);
            (void)ssi_dump_err_regs(SSI_ERR_PCIE_SR_WAKEUP_FAIL);

            /* ��ೢ�ԵĴ�������������� */
            if (ul_retry_cnt++ < ul_max_retry_cnt) {
                pci_print_log(PCI_LOG_INFO, "pull down wakeup gpio and retry");
                board_host_wakeup_dev_set(0);
                oal_msleep(5);
                goto retry;
            } else {
                declare_dft_trace_key_info("pcie_resume_powerup_ack_timeout_retry_failed", OAL_DFT_TRACE_FAIL);
                (void)ssi_dump_err_regs(SSI_ERR_PCIE_SR_WAKEUP_RETRY_FAIL);
            }

        } else {
            pci_print_log(PCI_LOG_INFO, "powerup done");
        }
    } else {
        oal_msleep(100); /* ����Ҫ��GPIO ��ACK �ӳٲ��ɿ�, S/R ���� ʱ��ϳ� */
    }

    oal_atomic_set(&g_bus_powerup_dev_wait_ack, 0);

    return 0;
}

OAL_STATIC int32_t oal_pcie_device_suspend_handler(void *data)
{
    oal_pcie_linux_res *pst_pci_lres = (oal_pcie_linux_res *)data;
#ifdef _PRE_COMMENT_CODE_
    /* �ߵ�����˵��wakelock�Ѿ��ͷţ�WIFI�Ѿ���˯,֪ͨRC/EP�µ磬 ����TurnOff Message */
    /* �µ�֮ǰ�ر� PCIE HOST ������ */
    kirin_pcie_power_notifiy_register(g_kirin_rc_idx, NULL, NULL, NULL);
    kirin_pcie_pm_control(0, g_kirin_rc_idx);
#else
    /*
     * �µ�����������д���
     * kirin_pcie_suspend_noirq,
     * �޷��ж�turnoff �Ƿ�ɹ�����
     */
#endif

    /* �˴���һ��������µ��� */
    pst_pci_lres->power_status = PCIE_EP_IP_POWER_DOWN;

    oal_pcie_change_link_state(pst_pci_lres->pst_pci_res, PCI_WLAN_LINK_DEEPSLEEP);

    pci_print_log(PCI_LOG_INFO, "pcie_suspend_powerdown");
    declare_dft_trace_key_info("pcie_suspend_powerdown", OAL_DFT_TRACE_SUCC);
#ifdef CONFIG_ARCH_KIRIN_PCIE
    kirin_pcie_power_notifiy_register(g_kirin_rc_idx, oal_pcie_device_wake_powerup_handler, NULL, NULL);
#endif

    return 0;
}
#endif

OAL_STATIC int32_t oal_pcie_suspend(oal_pci_dev_stru *pst_pci_dev, oal_pm_message_t state)
{
    hcc_bus *pst_bus = NULL;
    struct hcc_handler *pst_hcc = NULL;
    oal_pcie_linux_res *pst_pci_lres = (oal_pcie_linux_res *)oal_pci_get_drvdata(pst_pci_dev);

    if (pst_pci_lres == NULL) {
        oal_io_print("pcie_suspend lres is null\n");
        return 0;
    }

    pst_bus = pst_pci_lres->pst_bus;
    if (pst_bus == NULL) {
        oal_io_print(KERN_ERR "enter oal_pcie_suspend\n");
        return -OAL_ENODEV;
    }

    if (hbus_to_dev(pst_bus) == NULL) {
        oal_io_print("suspend,pcie is not work...\n");
        return OAL_SUCC;
    }

    pst_hcc = hbus_to_hcc(pst_bus);
    if (pst_hcc == NULL) {
        pci_print_log(PCI_LOG_WARN, "pcie dev's hcc handler is null!");
        return OAL_SUCC;
    }

    if (pst_bus != hdev_to_hbus(hbus_to_dev(pst_bus))) {
        /* pcie�ǵ�ǰ�ӿ� */
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "pcie is not current bus, return");
        return OAL_SUCC;
    }

    if (oal_atomic_read(&pst_hcc->state) != HCC_ON) {
        pci_print_log(PCI_LOG_INFO, "wifi is closed");
        return OAL_SUCC;
    }

    pci_print_log(PCI_LOG_INFO, "oal_pcie_suspend");

    if (down_interruptible(&pst_bus->sr_wake_sema)) {
        oal_io_print(KERN_ERR "pcie_wake_sema down failed.");
        return -OAL_EFAIL;
    }

    if (hcc_bus_wakelock_active(pst_bus)) {
        /* has wake lock so stop controller's suspend,
         * otherwise controller maybe error while sdio reinit */
        oal_io_print(KERN_ERR "Already wake up");
        up(&pst_bus->sr_wake_sema);
        return -OAL_EFAIL;
    }

    wlan_pm_wkup_src_debug_set(OAL_TRUE);

    declare_dft_trace_key_info("pcie_os_suspend", OAL_DFT_TRACE_SUCC);

    oal_pcie_save_default_resource(pst_pci_lres);
#ifdef CONFIG_ARCH_KIRIN_PCIE
    kirin_pcie_power_notifiy_register(g_kirin_rc_idx, NULL, oal_pcie_device_suspend_handler, (void *)pst_pci_lres);
#endif
    return 0;
}

OAL_STATIC int32_t oal_pcie_resume(oal_pci_dev_stru *pst_pci_dev)
{
    int32_t ret;
    uint32_t version = 0x0;
    hcc_bus *pst_bus = NULL;
    struct hcc_handler *pst_hcc = NULL;
    oal_pcie_linux_res *pst_pci_lres = (oal_pcie_linux_res *)oal_pci_get_drvdata(pst_pci_dev);
    if (pst_pci_lres == NULL) {
        oal_io_print("pcie_resume lres is null\n");
        return 0;
    }

    pst_bus = pst_pci_lres->pst_bus;
    if (pst_bus == NULL) {
        oal_io_print(KERN_ERR "enter oal_pcie_suspend\n");
        return -OAL_ENODEV;
    }

    if (hbus_to_dev(pst_bus) == NULL) {
        oal_io_print("resume,pcie is not work...\n");
        return OAL_SUCC;
    }

    pst_hcc = hbus_to_hcc(pst_bus);
    if (pst_hcc == NULL) {
        pci_print_log(PCI_LOG_WARN, "pcie dev's hcc handler is null!");
        return OAL_SUCC;
    }

    if (pst_bus != hdev_to_hbus(hbus_to_dev(pst_bus))) {
        /* pcie�ǵ�ǰ�ӿ� */
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "pcie is not current bus, return");
        return OAL_SUCC;
    }

    if (oal_atomic_read(&pst_hcc->state) != HCC_ON) {
        pci_print_log(PCI_LOG_INFO, "wifi is closed");
        return OAL_SUCC;
    }

    declare_dft_trace_key_info("pcie_os_resume", OAL_DFT_TRACE_SUCC);

    if (pst_pci_lres->power_status != PCIE_EP_IP_POWER_DOWN) {
        pci_print_log(PCI_LOG_INFO, "oal_pcie_resume");
        up(&pst_bus->sr_wake_sema);
        return OAL_SUCC;
    }

    ret = oal_pci_read_config_dword(pst_pci_lres->pst_pcie_dev, 0x0, &version);
    if (ret) {
        pci_print_log(PCI_LOG_ERR, "read pci version failed ret=%d", ret);
        oal_msleep(1000);
        ret = oal_pci_read_config_dword(pst_pci_lres->pst_pcie_dev, 0x0, &version);
        if (ret) {
            pci_print_log(PCI_LOG_ERR, "read pci version failed ret=%d", ret);
        } else {
            pci_print_log(PCI_LOG_WARN, "version:0x%x is not match with:0x%x", version, pst_pci_lres->version);
        }
        up(&pst_bus->sr_wake_sema);
        hcc_bus_exception_submit(pst_pci_lres->pst_bus, WIFI_TRANS_FAIL);

        chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_PLAT, CHR_LAYER_DRV,
                             CHR_PLT_DRV_EVENT_PCIE, CHR_PLAT_DRV_ERROR_RESUME_FIRMWARE_DOWN);
        return OAL_SUCC;
    }

    if (version != pst_pci_lres->version) {
        pci_print_log(PCI_LOG_WARN, "version:0x%x is not match with:0x%x", version, pst_pci_lres->version);
    }

    board_host_wakeup_dev_set(0);
    up(&pst_bus->sr_wake_sema);
    return 0;
}

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
OAL_STATIC void oal_pcie_shutdown(oal_pci_dev_stru *pst_pcie_dev)
{
    declare_time_cost_stru(cost);

    oal_pcie_linux_res *pst_pci_lres = (oal_pcie_linux_res *)oal_pci_get_drvdata(pst_pcie_dev);
    g_chip_err_block = 0;
    if (pst_pci_lres == NULL) {
        return;
    }

    /* system shutdown, should't write sdt file */
    oam_set_output_type(OAM_OUTPUT_TYPE_CONSOLE);

    if (pst_pci_lres->pst_bus == NULL) {
        pci_print_log(PCI_LOG_INFO, "pcie bus is null");
        return;
    }
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0))
    time_cost_var_start(cost) = 0;
#else
    time_cost_var_start(cost) = (ktime_t) { .tv64 = 0 };
#endif

    oal_wlan_gpio_intr_enable(hbus_to_dev(pst_pci_lres->pst_bus), OAL_FALSE);
    oal_disable_pcie_irq(pst_pci_lres);

    /* power off wifi */
    if (g_pcie_shutdown_panic) {
        oal_get_time_cost_start(cost);
    }

    if (in_interrupt() || irqs_disabled() || in_atomic()) {
        pci_print_log(PCI_LOG_INFO, "no call wlan_pm_close_by_shutdown interrupt");
    } else {
        wlan_pm_close_by_shutdown();
    }

    if (g_pcie_shutdown_panic) {
        oal_get_time_cost_end(cost);
        oal_calc_time_cost_sub(cost);
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "pcie shutdown cost %llu us", time_cost_var_sub(cost));
#ifdef PLATFORM_DEBUG_ENABLE
        /* debug shutdown process when after wifi shutdown */
        if (time_cost_var_sub(cost) > (uint64_t)(uint32_t)g_pcie_shutdown_panic) {
            oal_print_hi11xx_log(HI11XX_LOG_INFO, "pcie shutdown panic");
            BUG_ON(1);
        }
#endif
    }
}

OAL_STATIC oal_pci_driver_stru g_pcie_drv = {
    .name = "hi110x_pci",
    .id_table = g_pcie_id_tab,
    .probe = oal_pcie_probe,
    .remove = oal_pcie_remove,
    .suspend = oal_pcie_suspend,
    .resume = oal_pcie_resume,
    .shutdown = oal_pcie_shutdown,
};
#elif (_PRE_OS_VERSION_WIN32 == _PRE_OS_VERSION)
OAL_STATIC oal_pci_driver_stru g_pcie_drv = {
    "hi110x_pci",
    g_pcie_id_tab,
    oal_pcie_probe,
    oal_pcie_remove
};
#endif

OAL_STATIC int32_t oal_pcie_get_state(hcc_bus *pst_bus, uint32_t mask)
{
    oal_pcie_linux_res *pst_pci_lres;

    pst_pci_lres = (oal_pcie_linux_res *)pst_bus->data;
    oal_reference(pst_pci_lres);

    return OAL_TRUE;
}

OAL_STATIC void oal_enable_pcie_state(hcc_bus *pst_bus, uint32_t mask)
{
    oal_pcie_linux_res *pst_pci_lres;

    pst_pci_lres = (oal_pcie_linux_res *)pst_bus->data;
    oal_reference(pst_pci_lres);
}

OAL_STATIC void oal_disable_pcie_state(hcc_bus *pst_bus, uint32_t mask)
{
    oal_pcie_linux_res *pst_pci_lres;

    pst_pci_lres = (oal_pcie_linux_res *)pst_bus->data;
    oal_reference(pst_pci_lres);
}

OAL_STATIC int32_t oal_pcie_rx_netbuf(hcc_bus *pst_bus, oal_netbuf_head_stru *pst_head)
{
    oal_pcie_linux_res *pst_pci_lres;

    pst_pci_lres = (oal_pcie_linux_res *)pst_bus->data;
    oal_reference(pst_pci_lres);
    return OAL_SUCC;
}

OAL_STATIC PCIE_H2D_RINGBUF_QTYPE g_pcie_hcc2edma_qmap[HCC_NETBUF_QUEUE_BUTT] = {
    [HCC_NETBUF_NORMAL_QUEUE] = PCIE_H2D_QTYPE_NORMAL,
    [HCC_NETBUF_HIGH_QUEUE] = PCIE_H2D_QTYPE_HIGH,
    [HCC_NETBUF_HIGH2_QUEUE] = PCIE_H2D_QTYPE_NORMAL,
};
OAL_STATIC PCIE_H2D_RINGBUF_QTYPE oal_pcie_hcc_qtype_to_edma_qtype(hcc_netbuf_queue_type qtype)
{
    return g_pcie_hcc2edma_qmap[qtype];
}

OAL_STATIC int32_t oal_pcie_check_tx_param(hcc_bus *pst_bus, oal_netbuf_head_stru *pst_head,
                                           hcc_netbuf_queue_type qtype)
{
    oal_pcie_linux_res *pst_pci_lres = NULL;
    struct hcc_handler *pst_hcc = NULL;

    pst_pci_lres = (oal_pcie_linux_res *)pst_bus->data;
    if (oal_warn_on(pst_pci_lres == NULL)) {
        oal_io_print("pcie tx netbuf failed, lres is null\n");
        hcc_tx_netbuf_list_free(pst_head);
        return -OAL_ENODEV;
    }
    pst_hcc = hbus_to_hcc(pst_bus);
    if (pst_hcc == NULL) {
        pci_print_log(PCI_LOG_WARN, "pcie dev's hcc handler is null!");
        hcc_tx_netbuf_list_free(pst_head);
        return -OAL_ENODEV;
    }

    if (oal_unlikely(oal_atomic_read(&pst_hcc->state) != HCC_ON)) {
        /* drop netbuf list, wlan close or exception */
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "drop pcie netbuflist %u", oal_netbuf_list_len(pst_head));
        hcc_tx_netbuf_list_free(pst_head);
        return -OAL_EFAIL;
    }

    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_edma_tx_netbuf(hcc_bus *pst_bus, oal_netbuf_head_stru *pst_head,
                                           hcc_netbuf_queue_type qtype)
{
    int32_t ret;
    oal_pcie_linux_res *pst_pci_lres = (oal_pcie_linux_res *)pst_bus->data;

    ret = oal_pcie_check_tx_param(pst_bus, pst_head, qtype);
    if (oal_unlikely(ret != OAL_SUCC)) {
        return 0;
    }

    ret = oal_pcie_send_netbuf_list(pst_pci_lres->pst_pci_res, pst_head, oal_pcie_hcc_qtype_to_edma_qtype(qtype));
    return ret;
}

OAL_STATIC int32_t oal_pcie_ete_tx_netbuf(hcc_bus *pst_bus, oal_netbuf_head_stru *pst_head, hcc_netbuf_queue_type qtype)
{
    int32_t ret;
    oal_pcie_linux_res *pst_pci_lres = (oal_pcie_linux_res *)pst_bus->data;

    ret = oal_pcie_check_tx_param(pst_bus, pst_head, qtype);
    if (oal_unlikely(ret != OAL_SUCC)) {
        return 0;
    }

    ret = oal_ete_send_netbuf_list(pst_pci_lres->pst_pci_res, pst_head, qtype);
    return ret;
}

OAL_STATIC int32_t oal_pcie_send_message2dev(oal_pcie_linux_res *pst_pci_lres, uint32_t message)
{
    hcc_bus *pst_bus;
    int32_t ret;
    pst_bus = pst_pci_lres->pst_bus;

    if (oal_warn_on(pst_bus == NULL)) {
        return -OAL_ENODEV;
    }

    pci_print_log(PCI_LOG_DBG, "send msg to device [0x%8x]\n", (uint32_t)message);

    hcc_bus_wake_lock(pst_bus);
    ret = oal_pcie_send_message_to_dev(pst_pci_lres->pst_pci_res, message);
    hcc_bus_wake_unlock(pst_bus);

    return ret;
}

OAL_STATIC int32_t oal_pcie_send_msg(hcc_bus *pst_bus, uint32_t val)
{
    oal_pcie_linux_res *pst_pci_lres;

    pst_pci_lres = (oal_pcie_linux_res *)pst_bus->data;

    if (oal_warn_on(pst_pci_lres == NULL)) {
        return -OAL_ENODEV;
    }

    if (val >= H2D_MSG_COUNT) {
        oal_io_print("[Error]invaild param[%u]!\n", val);
        return -OAL_EINVAL;
    }

    return oal_pcie_send_message2dev(pst_pci_lres, (uint32_t)val);
}
#ifdef _PRE_PRODUCT_HI1620S_KUNPENG
#else


OAL_STATIC void oal_pcie_bindcpu_autoctl(oal_pcie_linux_res *pci_lres,
                                         oal_pci_dev_stru *pci_dev,
                                         int32_t is_bind)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#ifdef _PRE_HISI_BINDCPU
    struct cpumask slow_cpus, fast_cpus;

    hisi_get_slow_cpus(&slow_cpus);
    hisi_get_fast_cpus(&fast_cpus);
#endif

    if (is_bind) {
#ifdef CONFIG_NR_CPUS
#if CONFIG_NR_CPUS > OAL_BUS_HPCPU_NUM
        oal_print_hi11xx_log(HI11XX_LOG_DBG, "pcie bindcpu irq =%u, bind to cpu %d", pci_dev->irq, OAL_BUS_HPCPU_NUM);
        irq_set_affinity_hint(pci_dev->irq, cpumask_of(OAL_BUS_HPCPU_NUM));
#else
        oal_print_hi11xx_log(HI11XX_LOG_DBG, "pcie bindcpu fail, cpu:%d irq:%u", CONFIG_NR_CPUS, pci_dev->irq);
#endif
#endif

#ifdef _PRE_HISI_BINDCPU
        if (pci_lres->pst_pci_res->pst_rx_hi_task) {
            /* �ж����ں˻�Ӱ���߳�Ǩ�� */
            cpumask_clear_cpu(OAL_BUS_HPCPU_NUM, &fast_cpus);
            set_cpus_allowed_ptr(pci_lres->pst_pci_res->pst_rx_hi_task, &fast_cpus);
        }
#endif
    } else {
#ifdef CONFIG_NR_CPUS
        oal_print_hi11xx_log(HI11XX_LOG_DBG, "pcie unbind cpu irq =%u", pci_dev->irq);
#if (KERNEL_VERSION(2, 6, 35) <= LINUX_VERSION_CODE)
        /* 2.6.35-rc1 */
        irq_set_affinity_hint(pci_dev->irq, cpumask_of(0));
#endif
#ifdef _PRE_HISI_BINDCPU
        if (pci_lres->pst_pci_res->pst_rx_hi_task) {
            set_cpus_allowed_ptr(pci_lres->pst_pci_res->pst_rx_hi_task, &slow_cpus);
        }
#endif
#endif
    }
#endif
}


OAL_STATIC void oal_pcie_bindcpu_userctl(oal_pcie_linux_res *pci_lres,
                                         oal_pci_dev_stru *pci_dev,
                                         uint8_t irq_cpu,
                                         uint8_t thread_cmd)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#ifdef _PRE_HISI_BINDCPU
    struct cpumask thread_cpu_mask;
#endif

#if defined(CONFIG_NR_CPUS) &&  defined(_PRE_HISI_BINDCPU)
    oal_print_hi11xx_log(HI11XX_LOG_DBG, "pcie bindcpu irq =%u, bind to cpu %d", pci_dev->irq, irq_cpu);
    irq_set_affinity_hint(pci_dev->irq, cpumask_of(irq_cpu));
#endif

#ifdef _PRE_HISI_BINDCPU
    if (pci_lres->pst_pci_res->pst_rx_hi_task) {
        thread_cpu_mask = *((struct cpumask *)&thread_cmd);
        if (irq_cpu >= OAL_BUS_HPCPU_NUM) {
            /* �ж����ں˻�Ӱ���߳�Ǩ�� */
            cpumask_clear_cpu(irq_cpu, &thread_cpu_mask);
        }
        oal_print_hi11xx_log(HI11XX_LOG_DBG, "pst_rx_hi_task bind to cpu[0x%x]", thread_cmd);
        set_cpus_allowed_ptr(pci_lres->pst_pci_res->pst_rx_hi_task, &thread_cpu_mask);
    }
#endif
#endif
}

OAL_STATIC int32_t oal_pcie_bindcpu(hcc_bus *pst_bus, uint32_t chan, int32_t is_bind)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    oal_pcie_bindcpu_cfg bindcpu_cfd = *(oal_pcie_bindcpu_cfg *)&is_bind;
    oal_pcie_linux_res *pst_pci_lres = NULL;
    oal_pci_dev_stru *pst_pci_dev = NULL;

    pst_pci_lres = (oal_pcie_linux_res *)pst_bus->data;
    if (pst_pci_lres == OAL_PTR_NULL) {
        return OAL_SUCC;
    }

    pst_pci_dev = pst_pci_lres->pst_pcie_dev;
    if (pst_pci_dev == OAL_PTR_NULL) {
        return OAL_SUCC;
    }

    if (bindcpu_cfd.is_userctl == OAL_TRUE) {
        /* �����û������CPU */
        oal_pcie_bindcpu_userctl(pst_pci_lres, pst_pci_dev, bindcpu_cfd.irq_cpu, bindcpu_cfd.thread_cmd);
    } else {
        /* ������������PPS�����Զ���CPU */
        oal_pcie_bindcpu_autoctl(pst_pci_lres, pst_pci_dev, bindcpu_cfd.is_bind);
    }
#endif
    return OAL_SUCC;
}
#endif
OAL_STATIC int32_t oal_pcie_get_trans_count(hcc_bus *hi_bus, uint64_t *tx, uint64_t *rx)
{
    oal_pcie_linux_res *pst_pci_lres = (oal_pcie_linux_res *)hi_bus->data;
    return oal_pcie_get_host_trans_count(pst_pci_lres->pst_pci_res, tx, rx);
}

OAL_STATIC int32_t oal_pcie_shutdown_pre_respone(void *data)
{
    oal_pcie_linux_res *pst_pci_lres = (oal_pcie_linux_res *)data;
    oal_io_print("oal_pcie_shutdown_pre_respone\n");
    oal_complete(&pst_pci_lres->st_pcie_shutdown_response);
    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_switch_clean_res(hcc_bus *pst_bus)
{
    int32_t ret;

    /*
     * ���PCIE ͨ����֪ͨDevice�رշ���ͨ����
     * �ȴ�DMA������д���󷵻�
     */
    oal_pcie_linux_res *pst_pci_lres = (oal_pcie_linux_res *)pst_bus->data;

    oal_init_completion(&pst_pci_lres->st_pcie_shutdown_response);

    ret = oal_pcie_send_msg(pst_bus, H2D_MSG_SHUTDOWN_IP_PRE);
    if (ret) {
        oal_io_print("shutdown pre message send failed=%d\n", ret);
        return ret;
    }

    /* wait shutdown response */
    ret = oal_wait_for_completion_timeout(&pst_pci_lres->st_pcie_shutdown_response,
                                          (uint32_t)oal_msecs_to_jiffies(10000));
    if (ret == 0) {
        oal_io_print("wait pcie shutdown response timeout\n");
        return -OAL_ETIMEDOUT;
    }

    return OAL_SUCC;
    ;
}

#ifdef _PRE_PLAT_FEATURE_HI110X_DUAL_PCIE
OAL_STATIC int32_t oal_pcie_dual_reinit(oal_pcie_linux_res *pst_pci_lres)
{
    int32_t ret;
    uint32_t version = 0x0;
    declare_time_cost_stru(reinit);

    if (pst_pci_lres->pst_pcie_dev_dual == NULL) {
        return OAL_SUCC; /* dual device not exist */
    }

    oal_get_time_cost_start(reinit);

    oal_print_hi11xx_log(HI11XX_LOG_INFO, "oal_pcie_dual_reinit");

    ret = oal_pcie_enable_dual_device(pst_pci_lres);
    if (ret != OAL_SUCC) {
        return ret;
    }

    /* check link status */
    ret = oal_pci_read_config_dword(pst_pci_lres->pst_pcie_dev_dual, 0x0, &version);
    if (ret) {
        pci_print_log(PCI_LOG_ERR, "read pci version failed, enable device failed, ret=%d, version=0x%x",
                      ret, version);
        declare_dft_trace_key_info("pcie_enable_dual_device_reinit_fail", OAL_DFT_TRACE_FAIL);
        return -OAL_ENODEV;
    }

    if (version != pst_pci_lres->version) {
        pci_print_log(PCI_LOG_WARN, "version:0x%x is not match with:0x%x", version, pst_pci_lres->version);
        return -OAL_ENODEV;
    } else {
        pci_print_log(PCI_LOG_INFO, "pcie enable device check succ");
    }

    /* ��ʼ��PCIE��Դ */
    ret = oal_pcie_set_outbound_membar(&pst_pci_lres->pst_pci_res->st_iatu_dual_bar);
    if (ret) {
        oal_io_print(KERN_ERR "enable regions failed, ret=%d\n", ret);
        return ret;
    }

    oal_get_time_cost_end(reinit);
    oal_calc_time_cost_sub(reinit);
    pci_print_log(PCI_LOG_INFO, "pcie dual reinit cost %llu us", time_cost_var_sub(reinit));
    return ret;
}
#endif

OAL_STATIC int32_t oal_pcie_deinit(hcc_bus *pst_bus)
{
    return OAL_SUCC;
}

/* reinit pcie ep control */
OAL_STATIC int32_t oal_pcie_reinit(hcc_bus *pst_bus)
{
    int32_t ret;
    uint32_t version = 0x0;
    oal_pcie_linux_res *pst_pci_lres = (oal_pcie_linux_res *)pst_bus->data;
    declare_time_cost_stru(reinit);

    oal_get_time_cost_start(reinit);

    oal_print_hi11xx_log(HI11XX_LOG_INFO, "wake_sema_count=%d", pst_bus->sr_wake_sema.count);
    sema_init(&pst_bus->sr_wake_sema, 1); /* S/R�ź��� */

    hcc_bus_disable_state(pst_bus, OAL_BUS_STATE_ALL);
    ret = oal_pcie_enable_device(pst_pci_lres);
    if (ret == OAL_SUCC) {
#ifdef _PRE_COMMENT_CODE_
        /* ��Ҫ�ڳ�ʼ����ɺ�� */
        hcc_bus_enable_state(pst_bus, OAL_BUS_STATE_ALL);
#endif
    }

    /* check link status */
    if (pst_pci_lres->pst_pcie_dev != NULL) {
        ret = oal_pci_read_config_dword(pst_pci_lres->pst_pcie_dev, 0x0, &version);
        if (ret) {
            pci_print_log(PCI_LOG_ERR, "read pci version failed, enable device failed, ret=%d, version=0x%x",
                          ret, version);
            declare_dft_trace_key_info("pcie_enable_device_reinit_fail", OAL_DFT_TRACE_FAIL);
            return -OAL_ENODEV;
            ;
        }

        if (version != pst_pci_lres->version) {
            pci_print_log(PCI_LOG_WARN, "version:0x%x is not match with:0x%x", version, pst_pci_lres->version);
        } else {
            pci_print_log(PCI_LOG_INFO, "pcie enable device check succ");
        }
    }

    /* ��ʼ��PCIE��Դ */
    ret = oal_pcie_enable_regions(pst_pci_lres->pst_pci_res);
    if (ret) {
        oal_io_print(KERN_ERR "enable regions failed, ret=%d\n", ret);
        return ret;
    }

    ret = oal_pcie_iatu_init(pst_pci_lres->pst_pci_res);
    if (ret) {
        oal_io_print(KERN_ERR "iatu init failed, ret=%d\n", ret);
        return ret;
    }

    oal_get_time_cost_end(reinit);
    oal_calc_time_cost_sub(reinit);
    pci_print_log(PCI_LOG_INFO, "pcie reinit cost %llu us", time_cost_var_sub(reinit));
#ifdef _PRE_PLAT_FEATURE_HI110X_DUAL_PCIE
    ret = oal_pcie_dual_reinit(pst_pci_lres);
#endif
    return ret;
}

OAL_STATIC int32_t oal_pcie_edma_tx_condition(hcc_bus *pst_bus, hcc_netbuf_queue_type qtype)
{
    oal_pcie_linux_res *pst_pci_lres;
    pst_pci_lres = (oal_pcie_linux_res *)pst_bus->data;
    if (oal_warn_on(pst_pci_lres == NULL)) {
        oal_io_print("pci linux res is null\n");
        return OAL_FALSE;
    }

    return oal_pcie_edma_tx_is_idle(pst_pci_lres->pst_pci_res, oal_pcie_hcc_qtype_to_edma_qtype(qtype));
}

OAL_STATIC int32_t oal_pcie_ete_tx_condition(hcc_bus *pst_bus, hcc_netbuf_queue_type qtype)
{
    oal_pcie_linux_res *pst_pci_lres;
    pst_pci_lres = (oal_pcie_linux_res *)pst_bus->data;
    if (oal_warn_on(pst_pci_lres == NULL)) {
        oal_io_print("pci linux res is null\n");
        return OAL_FALSE;
    }

    return oal_pcie_ete_tx_is_idle(pst_pci_lres->pst_pci_res, qtype);
}


OAL_STATIC int32_t oal_pcie_host_lock(hcc_bus *pst_bus)
{
    oal_pcie_linux_res *pst_pci_lres;
    pst_pci_lres = (oal_pcie_linux_res *)pst_bus->data;
    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_host_unlock(hcc_bus *pst_bus)
{
    oal_pcie_linux_res *pst_pci_lres;
    pst_pci_lres = (oal_pcie_linux_res *)pst_bus->data;
    return OAL_SUCC;
}

/* 1103 PCIE ͨ�� host_wakeup_dev gpio �����Ѻ�֪ͨWCPU˯�� */
OAL_STATIC int32_t oal_pcie_sleep_request(hcc_bus *pst_bus)
{
    oal_pcie_linux_res *pst_pci_lres;
    pst_pci_lres = (oal_pcie_linux_res *)pst_bus->data;

    oal_disable_pcie_irq(pst_pci_lres);

    if (g_pcie_auto_disable_aspm != 0) {
        /* enable L1 after wakeup */
        oal_pcie_set_device_aspm_dync_disable(pst_pci_lres->pst_pci_res, OAL_FALSE);
        oal_pcie_device_xfer_pending_sig_clr(pst_pci_lres->pst_pci_res, OAL_TRUE);
    }

    /* ����GPIO��PCIEֻ����system suspend��ʱ��Ż��µ� GPIO ����֮�� DEV ��ʱ���ܽ���˯����������ͨ��PCIE ���� */
    mutex_lock(&pst_pci_lres->pst_pci_res->st_rx_mem_lock);
    oal_pcie_change_link_state(pst_pci_lres->pst_pci_res, PCI_WLAN_LINK_UP);
    mutex_unlock(&pst_pci_lres->pst_pci_res->st_rx_mem_lock);

    pci_clear_master(pst_pci_lres->pst_pcie_dev);

    return board_host_wakeup_dev_set(0);
}

OAL_STATIC int32_t oal_pcie_edma_sleep_request_host(hcc_bus *pst_bus)
{
    /* check host sleep condtion */
    oal_pcie_linux_res *pst_pci_lres = (oal_pcie_linux_res *)pst_bus->data;
    return oal_pcie_edma_sleep_request_host_check(pst_pci_lres->pst_pci_res);
}

OAL_STATIC int32_t oal_pcie_ete_sleep_request_host(hcc_bus *pst_bus)
{
    oal_pcie_linux_res *pst_pci_lres = (oal_pcie_linux_res *)pst_bus->data;
    return oal_ete_sleep_request_host_check(pst_pci_lres->pst_pci_res);
}

OAL_STATIC void oal_pcie_log_print(void)
{
    if (oal_print_rate_limit(PRINT_RATE_SECOND)) { /* 1s��ӡһ�� */
        pci_print_log(PCI_LOG_WARN, "oal_print_rate_limit");
    }
}

void oal_pcie_log_print_hi1xx(void)
{
    oal_pcie_log_print();
    oal_msleep(1200);
    oal_pcie_log_print();
    oal_pcie_log_print();
    oal_msleep(2000);
    oal_pcie_log_print();
}
#ifdef _PRE_CONFIG_ARCH_HI1620S_KUNPENG_PCIE
OAL_STATIC int32_t oal_pcie_wakeup_request(hcc_bus *pst_bus)
{
    oal_pcie_linux_res *pst_pci_lres = NULL;

    if (pst_bus == NULL) {
        pci_print_log(PCI_LOG_WARN, "oal_kupeng_pcie_wakeup_request pst_bus is null.\n");
        return -OAL_ENODEV;
    }

    pst_pci_lres = (oal_pcie_linux_res *)pst_bus->data;
    oal_pcie_device_wakeup_handler(NULL);
    pci_set_master(pst_pci_lres->pst_pcie_dev);

    return OAL_SUCC;
}
#else
OAL_STATIC int32_t oal_pcie_wakeup_request(hcc_bus *pst_bus)
{
#ifdef CONFIG_ARCH_KIRIN_PCIE
    int32_t ret;
    oal_pcie_linux_res *pst_pci_lres;
    pst_pci_lres = (oal_pcie_linux_res *)pst_bus->data;

    // 1.���� Host WakeUp Device gpio
    // 2.����kirin_pcie_pm_control �ϵ�RC ��齨��
    // 3.restore state, load iatu config
    if (oal_unlikely(pst_pci_lres->pst_pci_res->link_state <= PCI_WLAN_LINK_DOWN)) {
        pci_print_log(PCI_LOG_WARN, "link invaild, wakeup failed, link_state:%s",
                      oal_pcie_get_link_state_str(pst_pci_lres->pst_pci_res->link_state));
        return -OAL_ENODEV;
    }

    if (pst_pci_lres->power_status == PCIE_EP_IP_POWER_DOWN) {
        uint32_t ul_ret;
        declare_time_cost_stru(cost);
        struct wlan_pm_s *pst_wlan_pm = wlan_pm_get_drv();
        oal_get_time_cost_start(cost);

        if (pst_wlan_pm != NULL) {
            oal_init_completion(&pst_wlan_pm->st_wifi_powerup_done);
        }

        oal_atomic_set(&g_bus_powerup_dev_wait_ack, 1);
#ifdef _PRE_COMMENT_CODE_
        /* suspend ���µ�PCIE����������Ѿ��µ���Ҫ���³�ʼ��PCIE,�ָ�iatu���� */
        ret = kirin_pcie_power_notifiy_register(g_kirin_rc_idx, oal_pcie_device_wakeup_handler,
                                                NULL, NULL);
        if (ret) {
            return ret;
        }

        ret = kirin_pcie_pm_control(1, g_kirin_rc_idx);
        if (ret) {
            /* �����������DFR���̣�����LINKUP ֻ��20ms�ĳ�ʱʱ�� */
            oal_io_print(KERN_ERR "pcie_pm_control 1 failed!ret=%d\n", ret);
            return ret;
        }
#endif
        oal_pcie_device_wakeup_handler(NULL);
        if (pst_wlan_pm != NULL) {
            ul_ret = oal_wait_for_completion_timeout(&pst_wlan_pm->st_wifi_powerup_done,
                                                     (uint32_t)oal_msecs_to_jiffies(2000));
            if (oal_unlikely(ul_ret == 0)) {
                /* ��ʱ��������������Խ��� */
                declare_dft_trace_key_info("pcie_powerup_wakeup ack timeout", OAL_DFT_TRACE_FAIL);
            }
        } else {
            oal_msleep(100);
        }

        oal_atomic_set(&g_bus_powerup_dev_wait_ack, 0);

        /* iatu */
        ret = oal_pcie_reinit(pst_bus);
        if (ret != OAL_SUCC) {
#ifdef _PRE_COMMENT_CODE_
            kirin_pcie_power_notifiy_register(g_kirin_rc_idx, NULL, NULL, NULL);
            kirin_pcie_pm_control(0, g_kirin_rc_idx);
#endif
            pci_print_log(PCI_LOG_ERR, "oal_pcie_reinit failed!ret=%d", ret);
            return ret;
        }

        oal_pcie_host_ip_init(pst_pci_lres);

        if (oal_pcie_check_link_state(pst_pci_lres->pst_pci_res) == OAL_FALSE) {
            uint32_t version = 0x0;
            ret = oal_pci_read_config_dword(pst_pci_lres->pst_pcie_dev, 0x0, &version);
            pci_print_log(PCI_LOG_ERR, "mem access error, maybe linkdown! config read version :0x%x, ret=%d",
                          version, ret);
            return -OAL_ENODEV;
        }

        /* �������̣�RES�Ѿ���ʼ�� */
        oal_pcie_change_link_state(pst_pci_lres->pst_pci_res, PCI_WLAN_LINK_RES_UP);

        oal_atomic_set(&pst_pci_lres->st_pcie_wakeup_flag, 1);
        ret = oal_pcie_enable_device_func(pst_pci_lres);
        oal_atomic_set(&pst_pci_lres->st_pcie_wakeup_flag, 0);
        if (ret != OAL_SUCC) {
            oal_pcie_disable_regions(pst_pci_lres->pst_pci_res);
#ifdef _PRE_COMMENT_CODE_
            kirin_pcie_power_notifiy_register(g_kirin_rc_idx, NULL, NULL, NULL);
            kirin_pcie_pm_control(0, g_kirin_rc_idx);
#endif
            return ret;
        }

        mutex_lock(&pst_pci_lres->pst_pci_res->st_rx_mem_lock);
        oal_pcie_change_link_state(pst_pci_lres->pst_pci_res, PCI_WLAN_LINK_WORK_UP);
        mutex_unlock(&pst_pci_lres->pst_pci_res->st_rx_mem_lock);

        hcc_bus_enable_state(pst_bus, OAL_BUS_STATE_ALL);
        pst_pci_lres->power_status = PCIE_EP_IP_POWER_UP;

        oal_get_time_cost_end(cost);
        oal_calc_time_cost_sub(cost);
        pci_print_log(PCI_LOG_INFO, "pcie power up init succ, cost %llu us\n", time_cost_var_sub(cost));

    } else {
        /* ������оƬ��������GPIO���� */
        oal_pcie_device_wakeup_handler(NULL);
        pci_set_master(pst_pci_lres->pst_pcie_dev);
    }

#endif
    return OAL_SUCC;
}
#endif

OAL_STATIC int32_t oal_pcie_get_sleep_state(hcc_bus *pst_bus)
{
    oal_pcie_linux_res *pst_pci_lres;
    pst_pci_lres = (oal_pcie_linux_res *)pst_bus->data;
    if (oal_warn_on(pst_pci_lres == NULL)) {
        oal_io_print("pst_pci_lres is null\n");
        return -OAL_EINVAL;
    }

    return (board_get_host_wakeup_dev_stat() == 1) ? DISALLOW_TO_SLEEP_VALUE : ALLOW_TO_SLEEP_VALUE;
}

OAL_STATIC int32_t oal_pcie_edma_wakeup_complete(hcc_bus *pst_bus)
{
    oal_pcie_linux_res *pst_pci_lres;
    pst_pci_lres = (oal_pcie_linux_res *)pst_bus->data;
    if (oal_warn_on(pst_pci_lres == NULL)) {
        oal_io_print("pst_pci_lres is null\n");
        return -OAL_EINVAL;
    }

    mutex_lock(&pst_pci_lres->pst_pci_res->st_rx_mem_lock);
    oal_pcie_change_link_state(pst_pci_lres->pst_pci_res, PCI_WLAN_LINK_WORK_UP);
    mutex_unlock(&pst_pci_lres->pst_pci_res->st_rx_mem_lock);

    if (g_pcie_auto_disable_aspm != 0) {
        /* disable L1 after wakeup */
        oal_pcie_set_device_aspm_dync_disable(pst_pci_lres->pst_pci_res, OAL_TRUE);
        oal_pcie_device_xfer_pending_sig_clr(pst_pci_lres->pst_pci_res, OAL_FALSE);
    }

    oal_enable_pcie_irq(pst_pci_lres);

    oal_pcie_shced_rx_hi_thread(pst_pci_lres->pst_pci_res);

    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_ete_wakeup_complete(hcc_bus *pst_bus)
{
    oal_pcie_linux_res *pst_pci_lres;
    pst_pci_lres = (oal_pcie_linux_res *)pst_bus->data;
    if (oal_warn_on(pst_pci_lres == NULL)) {
        oal_io_print("pst_pci_lres is null\n");
        return -OAL_EINVAL;
    }

    mutex_lock(&pst_pci_lres->pst_pci_res->st_rx_mem_lock);
    oal_pcie_change_link_state(pst_pci_lres->pst_pci_res, PCI_WLAN_LINK_WORK_UP);
    mutex_unlock(&pst_pci_lres->pst_pci_res->st_rx_mem_lock);

    if (g_pcie_auto_disable_aspm != 0) {
        /* disable L1 after wakeup */
        oal_pcie_set_device_aspm_dync_disable(pst_pci_lres->pst_pci_res, OAL_TRUE);
        oal_pcie_device_xfer_pending_sig_clr(pst_pci_lres->pst_pci_res, OAL_FALSE);
    }

    oal_enable_pcie_irq(pst_pci_lres);

    oal_ete_sched_rx_threads(pst_pci_lres->pst_pci_res);

    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_rx_int_mask(hcc_bus *pst_bus, int32_t is_mask)
{
    oal_pcie_linux_res *pst_pci_lres;
    pst_pci_lres = (oal_pcie_linux_res *)pst_bus->data;

    if (oal_warn_on(pst_pci_lres == NULL)) {
        oal_io_print("pst_pci_lres is null\n");
        return -OAL_EINVAL;
    }
    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_pm_control(struct pci_dev *pdev, u32 rc_idx, int power_option)
{
    int32_t ret = -OAL_EFAIL;

#ifdef _PRE_CONFIG_ARCH_HI1620S_KUNPENG_PCIE
    if (pdev == NULL) {
        return -OAL_EFAIL;
    }

    ret = pci_rp_power_ctrl(pdev, power_option);
    pci_print_log(PCI_LOG_INFO, "pci_rp_power_ctrl ret=%d\n", ret);
    return ret;
#endif

    if (power_option == 1) { // 1 means power on
        wlan_pm_idle_sleep_vote(DISALLOW_IDLESLEEP);
    }

#ifdef CONFIG_ARCH_KIRIN_PCIE
    ret = kirin_pcie_pm_control(power_option, rc_idx);
    pci_print_log(PCI_LOG_INFO, "host_pcie_pm_control ret=%d\n", ret);
    if ((power_option == 0) || (power_option == 2)) { // 0 or 2 means power off
        wlan_pm_idle_sleep_vote(ALLOW_IDLESLEEP);
    }
    return ret;
#endif
}

OAL_STATIC int32_t oal_pcie_power_notifiy_register(struct pci_dev *pdev, u32 rc_idx, int (*poweron)(void *data),
                                                   int (*poweroff)(void *data), void *data)
{
    int32_t ret = -OAL_EFAIL;

#ifdef _PRE_CONFIG_ARCH_HI1620S_KUNPENG_PCIE
    if (pdev == NULL) {
        return -OAL_EFAIL;
    }

    ret = pci_power_notify_register(pdev, poweron, poweroff, data);
    pci_print_log(PCI_LOG_INFO, "pci_power_notify_register ret=%d\n", ret);
    return ret;
#endif

#ifdef CONFIG_ARCH_KIRIN_PCIE
    ret = kirin_pcie_power_notifiy_register(rc_idx, poweron, poweroff, data);
    pci_print_log(PCI_LOG_INFO, "host_pcie_power_notifiy_register ret=%d\n", ret);
    return ret;
#endif
}

OAL_STATIC int32_t oal_pcie_power_ctrl(hcc_bus *hi_bus, HCC_BUS_POWER_CTRL_TYPE ctrl,
                                       int (*func)(void *data), void *data)
{
    int32_t ret = -OAL_EFAIL;

    oal_pcie_linux_res *pst_pci_lres = NULL;
    if (hi_bus == NULL) {
        return ret;
    }
    pst_pci_lres = (oal_pcie_linux_res *)hi_bus->data;

    if (ctrl == HCC_BUS_CTRL_POWER_UP) {
        ret = oal_pcie_power_notifiy_register(pst_pci_lres->pst_pcie_dev, g_kirin_rc_idx, func, NULL, data);
    }

    if (ctrl == HCC_BUS_CTRL_POWER_DOWN) {
        ret = oal_pcie_power_notifiy_register(pst_pci_lres->pst_pcie_dev, g_kirin_rc_idx, NULL, func, data);
    }

    return ret;
}

OAL_STATIC int32_t oal_pcie_enable_device_func(oal_pcie_linux_res *pst_pci_lres)
{
    int32_t i;
    int32_t ret;
    const int32_t retry = 2;
    hcc_bus *pst_bus = pst_pci_lres->pst_bus;

    if (pst_bus == NULL) {
        return -OAL_ENODEV;
    }

    for (i = 0; i < retry; i++) {
        oal_init_completion(&pst_pci_lres->st_pcie_ready);

        ret = oal_pcie_send_message2dev(pst_pci_lres, PCIE_H2D_MESSAGE_HOST_READY);
        if (ret != OAL_SUCC) {
            oal_io_print(KERN_ERR "oal_pcie_send_message2dev failed, ret=%d\n", ret);
            continue;
        }

        /* ��һ���ж��п������ж�ʹ��֮ǰ�ϱ���ǿ�Ƶ���һ��RX Thread */
        up(&pst_bus->rx_sema);

        if (oal_wait_for_completion_timeout(&pst_pci_lres->st_pcie_ready,
                                            (uint32_t)oal_msecs_to_jiffies(HOST_WAIT_BOTTOM_INIT_TIMEOUT)) == 0) {
            oal_io_print(KERN_ERR "wait pcie ready timeout... %d ms \n", HOST_WAIT_BOTTOM_INIT_TIMEOUT);
            up(&pst_bus->rx_sema);
            if (oal_wait_for_completion_timeout(&pst_pci_lres->st_pcie_ready,
                                                (uint32_t)oal_msecs_to_jiffies(5000)) == 0) {
                oal_io_print(KERN_ERR "pcie retry 5 second hold, still timeout");
                if (i == 0) {
                    declare_dft_trace_key_info("wait pcie ready when power on, retry", OAL_DFT_TRACE_FAIL);
                } else {
                    declare_dft_trace_key_info("wait pcie ready when power on, retry still failed", OAL_DFT_TRACE_FAIL);
                }
                continue;
            } else {
                /* ǿ�Ƶ��ȳɹ���˵���п�����GPIO�ж�δ��Ӧ */
                oal_io_print(KERN_WARNING "[E]retry succ, maybe gpio interrupt issue");
                declare_dft_trace_key_info("pcie gpio int issue", OAL_DFT_TRACE_FAIL);
                break;
            }
        } else {
            break;
        }
    }

    if (i >= retry) {
        return ret;
    }

    return OAL_SUCC;
}

/* ���жϴ�������ѯ��Ϣ */
OAL_STATIC int32_t oal_pcie_enable_device_func_polling(oal_pcie_linux_res *pst_pci_lres)
{
    int32_t ret;
    int32_t retry = 0;
    unsigned long timeout_jiffies;

    hcc_bus *pst_bus = pst_pci_lres->pst_bus;

    if (pst_bus == NULL) {
        pci_print_log(PCI_LOG_ERR, "pst bus is null");
        return -OAL_ENODEV;
    }

    oal_init_completion(&pst_pci_lres->st_pcie_ready);

    /* �ȴ�device��ʼ����� */
    /* ֪ͨDevice�Ѿ���ʼ����� */
    ret = oal_pcie_send_message2dev(pst_pci_lres, PCIE_H2D_MESSAGE_HOST_READY);
    if (ret != OAL_SUCC) {
        pci_print_log(PCI_LOG_ERR, "oal_pcie_send_message2dev failed!ret=%d\n", ret);
        return ret;
    }

    up(&pst_bus->rx_sema);

    timeout_jiffies = jiffies + oal_msecs_to_jiffies(10000);
    forever_loop() {
        if (try_wait_for_completion(&pst_pci_lres->st_pcie_ready)) {
            /* decrement succ */
            break;
        }

        if (time_after(jiffies, timeout_jiffies)) {
            if (retry) {
                pci_print_log(PCI_LOG_ERR, "retry wait for pcie dev ready, 0x%lx, 0x%lx", jiffies, timeout_jiffies);
                declare_dft_trace_key_info("retry wait pcie dev ready", OAL_DFT_TRACE_FAIL);
                return -OAL_ETIMEDOUT;
            } else {
                retry = 1;
                pci_print_log(PCI_LOG_ERR, "wait for pcie dev ready, 0x%lx, 0x%lx", jiffies, timeout_jiffies);
                declare_dft_trace_key_info("wait pcie dev ready", OAL_DFT_TRACE_FAIL);
                ret = oal_pcie_send_message2dev(pst_pci_lres, PCIE_H2D_MESSAGE_HOST_READY);
                if (ret != OAL_SUCC) {
                    pci_print_log(PCI_LOG_ERR, "oal_pcie_send_message2dev retry failed!ret=%d\n", ret);
                    return ret;
                }

                timeout_jiffies = jiffies + oal_msecs_to_jiffies(5000);
                continue;
            }
        }

        up(&pst_bus->rx_sema);
        oal_msleep(1);
    }

    mutex_lock(&pst_pci_lres->pst_pci_res->st_rx_mem_lock);
    oal_pcie_change_link_state(pst_pci_lres->pst_pci_res, PCI_WLAN_LINK_WORK_UP);
    mutex_unlock(&pst_pci_lres->pst_pci_res->st_rx_mem_lock);

    pci_print_log(PCI_LOG_DBG, "device all ready, pcie wakeup done.");

    return OAL_SUCC;
}

OAL_STATIC int32_t oal_enable_pcie_irq(oal_pcie_linux_res *pst_pci_lres)
{
    oal_uint flag;
    if (oal_warn_on(pst_pci_lres == NULL)) {
        return -OAL_ENODEV;
    }

    oal_spin_lock_irq_save(&pst_pci_lres->st_lock, &flag);
    if (pst_pci_lres->irq_stat == 1) {
        if (!pst_pci_lres->st_msi.is_msi_support) {
            /* intx */
            pci_print_log(PCI_LOG_DBG, "enable_pcie_irq");
            enable_irq(pst_pci_lres->pst_pcie_dev->irq);
        } else {
            /* msi */
        }
        pst_pci_lres->irq_stat = 0;
    }
    oal_spin_unlock_irq_restore(&pst_pci_lres->st_lock, &flag);

    return OAL_SUCC;
}

int32_t oal_disable_pcie_irq(oal_pcie_linux_res *pst_pci_lres)
{
    oal_uint flag;

    if (oal_warn_on(pst_pci_lres == NULL)) {
        return -OAL_ENODEV;
    }

    oal_spin_lock_irq_save(&pst_pci_lres->st_lock, &flag);
    if (pst_pci_lres->irq_stat == 0) {
        pst_pci_lres->irq_stat = 1;
        if (!pst_pci_lres->st_msi.is_msi_support) {
            /* intx */
            pci_print_log(PCI_LOG_DBG, "disable_pcie_irq");
            if (in_irq()) {
                disable_irq_nosync(pst_pci_lres->pst_pcie_dev->irq);
            } else {
                /* process context */
                disable_irq_nosync(pst_pci_lres->pst_pcie_dev->irq);
                oal_spin_unlock_irq_restore(&pst_pci_lres->st_lock, &flag);
                synchronize_irq(pst_pci_lres->pst_pcie_dev->irq);
                oal_spin_lock_irq_save(&pst_pci_lres->st_lock, &flag);
            }
        } else {
            /* msi */
        }
    }
    oal_spin_unlock_irq_restore(&pst_pci_lres->st_lock, &flag);

    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_host_aspm_init(oal_pcie_linux_res *pst_pci_lres)
{
#ifdef CONFIG_ARCH_KIRIN_PCIE
    uint16_t val = 0;
    uint32_t reg;
    oal_pci_dev_stru *pst_rc_dev;
    pst_rc_dev = pci_upstream_bridge(pst_pci_lres->pst_pcie_dev);

    /* ʹ��/ȥʹ��ASPM��RC & EP */
    kirin_pcie_lp_ctrl(g_kirin_rc_idx, 0);
#endif

    if (pst_pci_lres->pst_pci_res != NULL) {
        oal_pcie_device_phy_config(pst_pci_lres->pst_pci_res);
    }

#ifdef  CONFIG_ARCH_KIRIN_PCIE
    if (g_pcie_aspm_enable) {
        /* L1SS config */
        if ((pst_pci_lres->pst_pci_res != NULL) && (pst_pci_lres->pst_pci_res->pst_pci_dbi_base != NULL)) {
            reg = oal_readl(pst_pci_lres->pst_pci_res->pst_pci_dbi_base + PCIE_ACK_F_ASPM_CTRL_OFF);
#ifdef _PRE_COMMENT_CODE_
            reg &= ~((0x7 << 24) | (0x7 << 27));
#endif
            reg &= ~((0x7 << 27));
#ifdef _PRE_COMMENT_CODE_
            /* L0s 7us entry, L1 64us entery, tx & rx */
            reg |= (0x7 << 24) | (0x7 << 27);
#endif
            reg |= (0x3 << 27);
            oal_writel(reg, pst_pci_lres->pst_pci_res->pst_pci_dbi_base + PCIE_ACK_F_ASPM_CTRL_OFF);
            pci_print_log(PCI_LOG_INFO, "ack aspm ctrl val:0x%x", reg);
#ifdef _PRE_COMMENT_CODE_
            reg = oal_readl(pst_pci_lres->pst_pci_res->pst_pci_dbi_base + PCIE_AUX_CLK_FREQ_OFF);
            reg &= ~(0x3ff);
            reg |= (0x2 << 0); /* aux_clk 2m,actual 1.92M  38.4M/20 */
            oal_writel(reg, pst_pci_lres->pst_pci_res->pst_pci_dbi_base + PCIE_AUX_CLK_FREQ_OFF);
            pci_print_log(PCI_LOG_INFO, "aux_clk_freq val:0x%x", reg);
#endif
        } else {
            pci_print_log(PCI_LOG_ERR, "pci res null or ctrl_base is null");
        }

        kirin_pcie_lp_ctrl(g_kirin_rc_idx, 1);
    }
    pci_print_log(PCI_LOG_INFO, "g_pcie_aspm_enable:%d", g_pcie_aspm_enable);

    /* rc noc protect */
    if (pst_rc_dev != NULL) {
        oal_pci_read_config_word(pst_rc_dev, oal_pci_pcie_cap(pst_rc_dev) + PCI_EXP_DEVCTL2, &val);
#ifdef _PRE_COMMENT_CODE_
        /* ����rc pcie rsp ��ʱʱ�䣬��kirin noc��Ӱ�� */
        val |= (0xe << 0);
        oal_pci_write_config_word(pst_rc_dev, oal_pci_pcie_cap(pst_rc_dev) + PCI_EXP_DEVCTL2, val);
#endif
        pci_print_log(PCI_LOG_INFO, "devctrl:0x%x", val);
    }
#endif

    return OAL_SUCC;
}

#ifdef CONFIG_ARCH_KIRIN_PCIE
OAL_STATIC int32_t oal_pcie_get_mps(oal_pci_dev_stru *pst_pcie_dev)
{
    int32_t ret;
    int32_t pos;
    uint16_t reg16 = 0;

    pos = pci_find_capability(pst_pcie_dev, PCI_CAP_ID_EXP);
    if (!pos) {
        return -1;
    }

    ret = oal_pci_read_config_word(pst_pcie_dev, pos + PCI_EXP_DEVCAP, &reg16);
    if (ret) {
        pci_print_log(PCI_LOG_ERR, "pci read devcap failed ret=%d", ret);
        return -1;
    }

    return (int32_t)(reg16 & PCI_EXP_DEVCAP_PAYLOAD);
}
#endif

/* Max Payload Size Supported,  must config beofre pcie access */
OAL_STATIC int32_t oal_pcie_mps_init(oal_pcie_linux_res *pst_pci_lres)
{
#ifdef CONFIG_ARCH_KIRIN_PCIE
    int32_t rc_mps, ep_mps;
    uint32_t mps, mrq;
    oal_pci_dev_stru *pst_rc_dev = NULL;

    if (!g_pcie_performance_mode) {
        return OAL_SUCC;
    }

    pst_rc_dev = pci_upstream_bridge(pst_pci_lres->pst_pcie_dev);
    if (pst_rc_dev == NULL) {
        pci_print_log(PCI_LOG_ERR, "no upstream dev");
        return -OAL_ENODEV;
    }

    rc_mps = oal_pcie_get_mps(pst_rc_dev);
    ep_mps = oal_pcie_get_mps(pst_pci_lres->pst_pcie_dev);

    if (rc_mps < 0 || ep_mps < 0) {
        pci_print_log(PCI_LOG_ERR, "mps get failed, rc_mps:%d  , ep_mps:%d", rc_mps, ep_mps);
        return -OAL_EFAIL;
    }

    mps = (uint32_t)oal_min(rc_mps, ep_mps);
    mrq = mps;
    pci_print_log(PCI_LOG_INFO, "rc/ep max payload size, rc_mps:%d  , ep_mps:%d, select mps:%d bytes",
                  rc_mps, ep_mps, OAL_PCIE_MIN_MPS << mps);
    mps <<= 5;  /* PCI_EXP_DEVCTL_PAYLOAD = 0b1110 0000 */
    mrq <<= 12; /* PCI_EXP_DEVCTL_READRQ  = 0b0111 0000 0000 0000 */

    pcie_capability_clear_and_set_word(pst_pci_lres->pst_pcie_dev, PCI_EXP_DEVCTL,
                                       PCI_EXP_DEVCTL_READRQ, mrq);
    pcie_capability_clear_and_set_word(pst_pci_lres->pst_pcie_dev, PCI_EXP_DEVCTL,
                                       PCI_EXP_DEVCTL_PAYLOAD, mps);
    pcie_capability_clear_and_set_word(pst_rc_dev, PCI_EXP_DEVCTL,
                                       PCI_EXP_DEVCTL_PAYLOAD, mps);
    pcie_capability_clear_and_set_word(pst_rc_dev, PCI_EXP_DEVCTL,
                                       PCI_EXP_DEVCTL_READRQ, mrq);
#endif
    return OAL_SUCC;
}

OAL_STATIC oal_void oal_pcie_host_ip_edma_init(oal_pcie_linux_res *pst_pci_lres)
{
    if (pst_pci_lres->pst_pci_res->chip_info.edma_support != OAL_TRUE) {
        return;
    }
    oal_pcie_set_device_soft_fifo_enable(pst_pci_lres->pst_pci_res);
    oal_pcie_set_device_ringbuf_bugfix_enable(pst_pci_lres->pst_pci_res);
    oal_pcie_set_device_dma_check_enable(pst_pci_lres->pst_pci_res);
}

OAL_STATIC oal_void oal_pcie_host_ip_ete_init(oal_pcie_linux_res *pst_pci_lres)
{
    if (pst_pci_lres->pst_pci_res->chip_info.ete_support != OAL_TRUE) {
        return;
    }
    oal_print_hi11xx_log(HI11XX_LOG_INFO, "oal_pcie_host_ip_ete_init");
    (void)oal_ete_ip_init(pst_pci_lres->pst_pci_res);
}

OAL_STATIC oal_void oal_pcie_host_ip_ete_exit(oal_pcie_linux_res *pst_pci_lres)
{
    if (pst_pci_lres->pst_pci_res->chip_info.ete_support != OAL_TRUE) {
        return;
    }
    oal_print_hi11xx_log(HI11XX_LOG_INFO, "oal_pcie_host_ip_ete_exit");
    (void)oal_ete_ip_exit(pst_pci_lres->pst_pci_res);
}

OAL_STATIC int32_t oal_pcie_host_ip_init(oal_pcie_linux_res *pst_pci_lres)
{
    oal_pcie_mps_init(pst_pci_lres);
    oal_pcie_host_aspm_init(pst_pci_lres);
    oal_pcie_host_ip_edma_init(pst_pci_lres);
    oal_pcie_host_ip_ete_init(pst_pci_lres);
    return OAL_SUCC;
}

OAL_STATIC void oal_pcie_print_chip_info(oal_pcie_linux_res *pst_pci_lres, uint32_t is_full_log)
{
    /* L1 recovery count */
    uint32_t l1_err_cnt = oal_readl(pst_pci_lres->pst_pci_res->pst_pci_ctrl_base + PCIE_STAT_CNT_LTSSM_ENTER_RCVRY_OFF);
    uint32_t l1_recvy_cnt = oal_readl(pst_pci_lres->pst_pci_res->pst_pci_ctrl_base + PCIE_STAT_CNT_L1_ENTER_RCVRY_OFF);
    const uint32_t l1_max_err_cnt = 3;

    if (pst_pci_lres->l1_err_cnt == 0) {
        pst_pci_lres->l1_err_cnt = l1_err_cnt;
    }

    if (is_full_log) {
        if (l1_err_cnt > l1_max_err_cnt - 1) {
            oal_print_hi11xx_log(HI11XX_LOG_ERR, "not l1 recovry error count %u, link_state unstable", l1_err_cnt);
        } else {
            /* ���������л����2�� */
            oal_print_hi11xx_log(HI11XX_LOG_INFO, "link_state stable, l1 excp count:%u", l1_err_cnt);
        }

        if (l1_recvy_cnt) {
            oal_print_hi11xx_log(HI11XX_LOG_INFO, "l1 enter count is %u", l1_recvy_cnt);
        } else {
            oal_print_hi11xx_log(HI11XX_LOG_INFO, "l1 maybe not enable");
        }
    } else {
        if (l1_err_cnt > l1_max_err_cnt - 1) {
            if ((pst_pci_lres->l1_err_cnt == 0) || (l1_err_cnt > pst_pci_lres->l1_err_cnt)) {
                /* link state error */
                oal_print_hi11xx_log(HI11XX_LOG_ERR, "not l1 recovry error count %u, link_state unstable", l1_err_cnt);
                declare_dft_trace_key_info("pcie_link_state_unstable", OAL_DFT_TRACE_FAIL);
            } else {
                oal_print_hi11xx_log(HI11XX_LOG_DBG, "link_state stable, l1 excp count:%u", l1_err_cnt);
            }
        }
    }

    pst_pci_lres->l1_err_cnt = l1_err_cnt;
}

OAL_STATIC void oal_pcie_chip_info(hcc_bus *pst_bus, uint32_t is_need_wakeup, uint32_t is_full_log)
{
    oal_pcie_linux_res *pst_pci_lres;
    pst_pci_lres = (oal_pcie_linux_res *)pst_bus->data;
    if (oal_warn_on(pst_pci_lres == NULL)) {
        return;
    }

    if (oal_unlikely(pst_pci_lres->pst_pci_res->link_state <= PCI_WLAN_LINK_DOWN)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "link_state:%s",
                             oal_pcie_get_link_state_str(pst_pci_lres->pst_pci_res->link_state));
        return;
    }

    if (is_need_wakeup == OAL_TRUE) {
        if (pst_bus->bus_dev == NULL) {
            return;
        }
    }

    if (is_need_wakeup == OAL_TRUE) {
        hcc_tx_transfer_lock(pst_bus->bus_dev->hcc);
        if (oal_unlikely(hcc_bus_pm_wakeup_device(pst_pci_lres->pst_bus) != OAL_SUCC)) {
            oal_print_hi11xx_log(HI11XX_LOG_ERR, "wakeup device failed");
            hcc_tx_transfer_unlock(pst_bus->bus_dev->hcc);
            return;
        }
    }

    oal_pcie_print_chip_info(pst_pci_lres, is_full_log);

    if (is_need_wakeup == OAL_TRUE) {
        hcc_tx_transfer_unlock(pst_bus->bus_dev->hcc);
    }
}

OAL_STATIC int32_t oal_pcie_ip_l1pm_check(oal_pcie_linux_res *pst_pci_lres)
{
    oal_pcie_print_chip_info(pst_pci_lres, OAL_TRUE);

    oal_msleep(20); /* wait pcie enter L1.2 */

    if (g_ft_pcie_aspm_check_bypass) {
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "l1pm_check bypass");
        return OAL_SUCC;
    } else {
#ifdef CONFIG_ARCH_KIRIN_PCIE
#if defined(CONFIG_KIRIN_PCIE_HI3660)
        uint32_t value;
        void *__iomem pst_rc_ahb_reg = NULL;
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "pcie960 pcie test");
        pst_rc_ahb_reg = ioremap_nocache(0xff3fe000, 0x1000);
        if (pst_rc_ahb_reg == NULL) {
            oal_print_hi11xx_log(HI11XX_LOG_ERR, "pst_rc_ahb_reg request failed");
            return -OAL_ENOMEM;
        }

        value = oal_readl(pst_rc_ahb_reg + 0x414);
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "SOC_PCIECTRL_STATE4_ADDR reg=0x%x", value);

        if ((value & (1 << 14 | 1 << 15)) == ((1 << 14 | 1 << 15))) {
            return OAL_SUCC;
        } else {
            oal_print_hi11xx_log(HI11XX_LOG_ERR, "L1.2 check failed, reg=0x%x, bit 14 15 must be 1", value);
            return -OAL_EFAIL;
        }

#elif defined(CONFIG_KIRIN_PCIE_KIRIN970)
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "pcie970 not implemented");
        return OAL_ENODEV;
#else
        uint32_t value = show_link_state(g_kirin_rc_idx);
        if (value == 0xC000) {
            return OAL_SUCC;
        } else {
            oal_print_hi11xx_log(HI11XX_LOG_ERR, "L1.2 check failed");
            return -OAL_EFAIL;
        }
#endif
#else
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "to be implemented");
        return -OAL_ENODEV;
#endif
    }
}

OAL_STATIC int32_t oal_pcie_gen_mode_check(oal_pcie_linux_res *pst_pci_lres)
{
    /* ����ʵ�ʲ�Ʒ���жϵ�ǰ��·״̬�Ƿ����� */
    int32_t gen_select = oal_pcie_get_gen_mode(pst_pci_lres->pst_pci_res);

    if (g_ft_pcie_gen_check_bypass) {
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "pcie_gen_check_bypass");
    } else {
        if (g_hipci_gen_select != gen_select) {
            oal_print_hi11xx_log(HI11XX_LOG_ERR, "expect link mode is %d, but current is %d",
                                 g_hipci_gen_select, gen_select);
            return -OAL_EFAIL;
        }
    }

    oal_print_hi11xx_log(HI11XX_LOG_INFO, "current link is %s", (gen_select == PCIE_GEN2) ? "GEN2" : "GEN1");

    return OAL_SUCC;
}

int32_t oal_pcie_ip_init(hcc_bus *pst_bus)
{
    int32_t ret;
    oal_pcie_linux_res *pst_pci_lres = NULL;
    if (pst_bus == NULL) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "pst_bus is null");
        return -OAL_ENODEV;
    }

    pst_pci_lres = (oal_pcie_linux_res *)pst_bus->data;
    if (oal_warn_on(pst_pci_lres == NULL)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "pst_pci_lres is null");
        return -OAL_ENODEV;
    }

    ret = oal_pcie_device_auxclk_init(pst_pci_lres->pst_pci_res);
    if (ret) {
        return ret;
    }

    ret = oal_pcie_device_aspm_init(pst_pci_lres->pst_pci_res);
    if (ret) {
        return ret;
    }

    /* ʹ�ܵ͹��� */
    ret = oal_pcie_host_aspm_init(pst_pci_lres);
    if (ret) {
        return ret;
    }

    return OAL_SUCC;
}

int32_t oal_pcie_ip_voltage_bias_init(hcc_bus *pst_bus)
{
    oal_pcie_linux_res *pst_pci_lres;
    pst_pci_lres = (oal_pcie_linux_res *)pst_bus->data;
    if (oal_warn_on(pst_pci_lres == NULL)) {
        return -OAL_ENODEV;
    }
    return oal_pcie_voltage_bias_init(pst_pci_lres->pst_pci_res);
}

int32_t oal_pcie_ip_factory_test(hcc_bus *pst_bus, int32_t test_count)
{
    int32_t i;
    int32_t ret;
    oal_pcie_linux_res *pst_pci_lres;

    pst_pci_lres = (oal_pcie_linux_res *)pst_bus->data;
    if (oal_warn_on(pst_pci_lres == NULL)) {
        return -OAL_ENODEV;
    }

    ret = oal_pcie_ip_init(pst_bus);

    oal_msleep(5); /* wait pcie enter L1.2 */

    ret = oal_pcie_ip_l1pm_check(pst_pci_lres);
    if (ret) {
        return ret;
    }

    ret = oal_pcie_voltage_bias_init(pst_pci_lres->pst_pci_res);
    if (ret) {
        return ret;
    }

    ret = oal_pcie_device_changeto_high_cpufreq(pst_pci_lres->pst_pci_res);
    if (ret) {
        return ret;
    }

    ret = oal_pcie_gen_mode_check(pst_pci_lres);
    if (ret) {
        return ret;
    }

    ret = oal_pcie_unmask_device_link_erros(pst_pci_lres->pst_pci_res);
    if (ret) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "unmask device link err failed = %d", ret);
        return ret;
    }

    for (i = 0; i < test_count; i++) {
        /* memcheck */
        ret = oal_pcie_device_mem_scanall(pst_pci_lres->pst_pci_res);
        if (ret) {
            return ret;
        }

        oal_print_hi11xx_log(HI11XX_LOG_INFO, "scan all mem done , test %d times", i + 1);

        ret = oal_pcie_check_device_link_errors(pst_pci_lres->pst_pci_res);
        if (ret) {
            return ret;
        }

        ret = oal_pcie_gen_mode_check(pst_pci_lres);
        if (ret) {
            return ret;
        }
    }

    oal_msleep(5); /* wait pcie enter L1.2 */

    ret = oal_pcie_ip_l1pm_check(pst_pci_lres);
    if (ret) {
        return ret;
    }

    return OAL_SUCC;
}

int32_t oal_pcie_rc_slt_chip_transfer(hcc_bus *pst_bus, void *ddr_address,
                                      uint32_t data_size, int32_t direction)
{
    oal_pcie_linux_res *pst_pci_lres = NULL;
    if (pst_bus == NULL) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "pst_bus is null");
        return -OAL_ENODEV;
    }

    pst_pci_lres = (oal_pcie_linux_res *)pst_bus->data;
    if (oal_warn_on(pst_pci_lres == NULL)) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "pst_pci_lres is null");
        return -OAL_ENODEV;
    }

    if (direction == 1) {
        /* 1��ʾ Host to Device */
        return oal_pcie_copy_to_device_by_dword(pst_pci_lres->pst_pci_res, ddr_address,
                                                oal_pcie_get_deivce_dtcm_cpuaddr(pst_pci_lres->pst_pci_res),
                                                data_size);
    } else if (direction == 2) {
        /* 2��ʾ Device to Host */
        return oal_pcie_copy_from_device_by_dword(pst_pci_lres->pst_pci_res, ddr_address,
                                                  oal_pcie_get_deivce_dtcm_cpuaddr(pst_pci_lres->pst_pci_res),
                                                  data_size);
    } else {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "invaild direction:%d", direction);
        return -OAL_EINVAL;
    }
}

#ifdef _PRE_PLAT_FEATURE_HI110X_DUAL_PCIE
OAL_STATIC OAL_INLINE int32_t oal_ete_dual_pci_power_up(oal_pcie_linux_res *pst_pci_lres)
{
    if (pst_pci_lres->pst_pcie_dev_dual == NULL) {
        /* no dual pci */
        return OAL_SUCC;
    }

    oal_pcie_power_notifiy_register(pcie_linux_res->pst_pcie_dev_dual, g_kirin_rc_idx_dual, oal_pcie_dual_power_on_callback,
                                    NULL, NULL);
    return oal_pcie_pm_control(pcie_linux_res->pst_pcie_dev_dual, g_kirin_rc_idx_dual, 1);
}

OAL_STATIC OAL_INLINE int32_t oal_ete_dual_pci_power_down(oal_pcie_linux_res *pst_pci_lres)
{
    if (pst_pci_lres->pst_pcie_dev_dual == NULL) {
        /* no dual pci */
        return OAL_SUCC;
    }
    oal_pcie_power_notifiy_register(pcie_linux_res->pst_pcie_dev_dual, g_kirin_rc_idx_dual, NULL,
                                    wlan_dual_first_power_off_callback, NULL);
    return oal_pcie_pm_control(pcie_linux_res->pst_pcie_dev_dual, g_kirin_rc_idx_dual, 2);
}
#endif

#ifdef _PRE_CONFIG_PCIE_SHARED_INTX_IRQ
/* request ���������ڴ���������ﲻ�ܼ�����������;  �ú���ֻ�������µ���ã����������� */
OAL_STATIC int32_t oal_enable_pcie_irq_with_request(oal_pcie_linux_res *pst_pci_lres)
{
    int32_t ret = OAL_SUCC;
    if (oal_warn_on(pst_pci_lres == NULL)) {
        return -OAL_ENODEV;
    }

    if (pst_pci_lres->irq_stat == 1) {
        if (!pst_pci_lres->st_msi.is_msi_support) {
            /* intx */
            pci_print_log(PCI_LOG_INFO, "enable_pcie_irq with request_irq");
            ret = request_irq(pst_pci_lres->pst_pcie_dev->irq, oal_pcie_intx_edma_isr, IRQF_SHARED,
                "hisi_edma_intx", (void *)pst_pci_lres);
            if (ret < 0) {
                oal_io_print("enable_pcie_irq with request_irq error\n");
                oal_pcie_change_link_state(pst_pci_lres->pst_pci_res, PCI_WLAN_LINK_DOWN);
                return -OAL_EINTR;
            }
        }
        pst_pci_lres->irq_stat = 0;
    }

    return OAL_SUCC;
}

int32_t oal_disable_pcie_irq_with_free(oal_pcie_linux_res *pst_pci_lres)
{
    if (oal_warn_on(pst_pci_lres == NULL)) {
        return -OAL_ENODEV;
    }

    if (pst_pci_lres->irq_stat == 0) {
        if (!pst_pci_lres->st_msi.is_msi_support) {
            /* intx */
            pci_print_log(PCI_LOG_INFO, "disable_pcie_irq with free_irq");
            free_irq(pst_pci_lres->pst_pcie_dev->irq, pst_pci_lres);
            pst_pci_lres->irq_stat = 1;
        }
    }

    return OAL_SUCC;
}
#endif

OAL_STATIC void oal_pcie_power_down(hcc_bus *pst_bus)
{
    oal_pcie_linux_res *pst_pci_lres = (oal_pcie_linux_res *)pst_bus->data;
    PCIE_EP_IP_STATUS old_power_status = pst_pci_lres->power_status;
    int32_t ret;

    /* disable intx gpio... �ȴ��жϴ����� */
#ifdef _PRE_CONFIG_PCIE_SHARED_INTX_IRQ
    ret = oal_disable_pcie_irq_with_free(pst_pci_lres);
#else
    ret = oal_disable_pcie_irq(pst_pci_lres);
#endif
    if (ret != OAL_SUCC) {
        return;
    }

    pst_pci_lres->power_status = PCIE_EP_IP_POWER_DOWN;
    if (pst_pci_lres->pst_pci_res == NULL) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "pst_pci_res is null");
        return;
    }

    pst_pci_lres->l1_err_cnt = 0;

    mutex_lock(&pst_pci_lres->pst_pci_res->st_rx_mem_lock);
    oal_pcie_change_link_state(pst_pci_lres->pst_pci_res, PCI_WLAN_LINK_DOWN);
    mutex_unlock(&pst_pci_lres->pst_pci_res->st_rx_mem_lock);

    oal_task_kill(&pst_pci_lres->st_rx_task);

    mutex_lock(&pst_pci_lres->pst_pci_res->st_rx_mem_lock);
    oal_pcie_disable_regions(pst_pci_lres->pst_pci_res);
    oal_pcie_transfer_res_exit(pst_pci_lres->pst_pci_res);
    mutex_unlock(&pst_pci_lres->pst_pci_res->st_rx_mem_lock);

#ifdef CONFIG_ARCH_KIRIN_PCIE
    oal_kirin_pcie_deregister_event(pst_pci_lres);
#endif

    /* �µ�֮ǰ�ر� PCIE HOST ������ */
#ifdef _PRE_CONFIG_ARCH_HI1620S_KUNPENG_PCIE
    ret = oal_pcie_pm_control(pst_pci_lres->pst_pcie_dev, g_kirin_rc_idx, 0); /* power off with L3 message */
#elif defined(CONFIG_ARCH_KIRIN_PCIE)

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 4, 0))
    ret = oal_pcie_pm_control(pst_pci_lres->pst_pcie_dev, g_kirin_rc_idx, 2); /* power off without L3 message */
#else
    ret = oal_pcie_pm_control(pst_pci_lres->pst_pcie_dev, g_kirin_rc_idx, 0); /* power off with L3 message */
#endif

#endif

#if defined(CONFIG_ARCH_KIRIN_PCIE) || defined(_PRE_CONFIG_ARCH_HI1620S_KUNPENG_PCIE)
    if (old_power_status != PCIE_EP_IP_POWER_DOWN) {
        if (ret) {
            declare_dft_trace_key_info("pcie_power_down_fail", OAL_DFT_TRACE_EXCEP);
        }
    }

    oal_pcie_power_notifiy_register(pst_pci_lres->pst_pcie_dev, g_kirin_rc_idx, NULL, NULL, NULL);
#ifdef _PRE_PLAT_FEATURE_HI110X_DUAL_PCIE
    oal_ete_dual_pci_power_down(pst_pci_lres);
#endif
#endif
}

#ifndef _PRE_HI375X_PCIE
#ifdef CONFIG_ARCH_KIRIN_PCIE
/* chip test */
void pcie_power_down_test(void)
{
    oal_pcie_linux_res *pst_pci_lres = pcie_linux_res;
    if (pst_pci_lres == NULL) {
        pci_print_log(PCI_LOG_INFO, "pst_pci_lres is null");
        return;
    }
    kirin_pcie_power_notifiy_register(g_kirin_rc_idx, NULL, oal_pcie_device_suspend_handler, (void *)pst_pci_lres);
    oal_kirin_pcie_deregister_event(pst_pci_lres);
    oal_pcie_save_default_resource(pst_pci_lres);
    {
        int32_t ret;
        ret = kirin_pcie_pm_control(0, g_kirin_rc_idx);
        if (ret) {
            pci_print_log(PCI_LOG_INFO, "pcie_pm_controltest failed=%d", ret);
            declare_dft_trace_key_info("pcie_power_down_fail", OAL_DFT_TRACE_EXCEP);
            ssi_dump_device_regs(SSI_MODULE_MASK_COMM | SSI_MODULE_MASK_PCIE_CFG | SSI_MODULE_MASK_PCIE_DBI);
        }
    }
}

void pcie_power_down_test_wake(void)
{
    int32_t ret;
    ret = kirin_pcie_pm_control(1, g_kirin_rc_idx);
    if (ret) {
        pci_print_log(PCI_LOG_ERR, "ret = %d", ret);
    }
}
#endif
#endif

OAL_STATIC void pcie_bus_power_down_action(hcc_bus *pst_bus)
{
    /* �ص�LINKDOWNע��ص����� */
    hcc_disable(hbus_to_hcc(pst_bus), OAL_TRUE);
    oal_wlan_gpio_intr_enable(hbus_to_dev(pst_bus), OAL_FALSE);
    hcc_transfer_lock(hbus_to_hcc(pst_bus));
    oal_pcie_power_down(pst_bus);
    hcc_transfer_unlock(hbus_to_hcc(pst_bus));
    board_host_wakeup_dev_set(0);
}

OAL_STATIC void oal_pcie_power_action_tae(oal_pcie_linux_res *pst_pci_lres, HCC_BUS_POWER_ACTION_TYPE action)
{
#ifdef _PRE_SDIO_TAE_DEBUG
    if (action == HCC_BUS_SW_POWER_PCIE_TAE_INIT) {
        oal_io_print("power patch lauch\n");
        oal_ete_ip_init(pst_pci_lres->pst_pci_res);
        oal_enable_pcie_irq(pst_pci_lres);
    }
#endif
}

OAL_STATIC int32_t oal_pcie_edma_power_action(hcc_bus *pst_bus, HCC_BUS_POWER_ACTION_TYPE action)
{
    int32_t ret;
    oal_pcie_linux_res *pst_pci_lres = (oal_pcie_linux_res *)pst_bus->data;

    if (oal_warn_on(pst_pci_lres == NULL)) {
        return -OAL_ENODEV;
    }

    oal_print_inject_check_stack();

    if (action == HCC_BUS_POWER_DOWN) {
        pcie_bus_power_down_action(pst_bus);
    }

    if (action == HCC_BUS_SW_POWER_DOWN) {
        oal_pcie_power_down(pst_bus);
    }

    if (action == HCC_BUS_POWER_UP || action == HCC_BUS_SW_POWER_UP) {
        /* �ϵ�֮ǰ��PCIE HOST ������ */
#if defined(CONFIG_ARCH_KIRIN_PCIE) || defined(_PRE_CONFIG_ARCH_HI1620S_KUNPENG_PCIE)
        declare_time_cost_stru(cost);
        if (action == HCC_BUS_POWER_UP) {
            oal_get_time_cost_start(cost);
        }

        pst_pci_lres->l1_err_cnt = 0;

        ret = oal_pcie_pm_control(pst_pci_lres->pst_pcie_dev, g_kirin_rc_idx, 1);
        if (g_pcie_print_once) {
            g_pcie_print_once = 0;
            ssi_dump_device_regs(SSI_MODULE_MASK_COMM | SSI_MODULE_MASK_PCIE_CFG | SSI_MODULE_MASK_PCIE_DBI |
              SSI_MODULE_MASK_WCTRL | SSI_MODULE_MASK_BCTRL);
        }
        board_host_wakeup_dev_set(0);
        oal_pcie_power_notifiy_register(pst_pci_lres->pst_pcie_dev, g_kirin_rc_idx, NULL, NULL, NULL);
        if (ret) {
            oal_io_print(KERN_ERR "pcie power on and link failed, ret=%d\n", ret);
            (void)ssi_dump_err_regs(SSI_ERR_PCIE_POWER_UP_FAIL);
            return ret;
        }

#if !defined(_PRE_HI375X_PCIE) && !defined(_PRE_PRODUCT_HI1620S_KUNPENG)
        kirin_pcie_register_event(&pst_pci_lres->pcie_event);
#endif

        pst_pci_lres->power_status = PCIE_EP_IP_POWER_UP;
        if (pst_pci_lres->pst_pci_res) {
            mutex_lock(&pst_pci_lres->pst_pci_res->st_rx_mem_lock);
            oal_pcie_change_link_state(pst_pci_lres->pst_pci_res, PCI_WLAN_LINK_UP);
            mutex_unlock(&pst_pci_lres->pst_pci_res->st_rx_mem_lock);
        }

        if (action == HCC_BUS_POWER_UP) {
            oal_get_time_cost_end(cost);
            oal_calc_time_cost_sub(cost);
            pci_print_log(PCI_LOG_INFO, "pcie link cost %llu us", time_cost_var_sub(cost));
        }
#endif
    }

    if (action == HCC_BUS_POWER_PATCH_LOAD_PREPARE) {
        /* close hcc */
        hcc_disable(hbus_to_hcc(pst_bus), OAL_TRUE);
        oal_init_completion(&pst_bus->st_device_ready);
        oal_wlan_gpio_intr_enable(hbus_to_dev(pst_bus), OAL_FALSE);
    }

    if (action == HCC_BUS_SW_POWER_PATCH_LOAD_PREPARE) {
        oal_init_completion(&pst_bus->st_device_ready);
    }


    if (action == HCC_BUS_POWER_PATCH_LAUCH) {
        oal_io_print("power patch lauch\n");

        /* Patch������� ��ʼ��ͨ����Դ��Ȼ��ȴ�ҵ���ʼ����� */
        ret = oal_pcie_transfer_res_init(pst_pci_lres->pst_pci_res);
        if (ret) {
            oal_io_print(KERN_ERR "pcie_transfer_res_init failed, ret=%d\n", ret);
            return ret;
        }

        oal_pcie_host_ip_init(pst_pci_lres);

        oal_wlan_gpio_intr_enable(hbus_to_dev(pst_bus), OAL_TRUE);

#ifdef _PRE_CONFIG_PCIE_SHARED_INTX_IRQ
        oal_enable_pcie_irq_with_request(pst_pci_lres);
#else
        oal_enable_pcie_irq(pst_pci_lres);
#endif

        ret = oal_pcie_enable_device_func(pst_pci_lres);
        if (ret != OAL_SUCC) {
            oal_io_print("enable pcie device func failed, ret=%d\n", ret);
            return ret;
        }

        if (oal_wait_for_completion_timeout(&pst_bus->st_device_ready,
                                            (uint32_t)oal_msecs_to_jiffies(HOST_WAIT_BOTTOM_INIT_TIMEOUT)) == 0) {
            oal_io_print(KERN_ERR "wait device ready timeout... %d ms \n", HOST_WAIT_BOTTOM_INIT_TIMEOUT);
            up(&pst_bus->rx_sema);
            if (oal_wait_for_completion_timeout(&pst_bus->st_device_ready, (uint32_t)oal_msecs_to_jiffies(5000)) == 0) {
                oal_io_print(KERN_ERR "retry 5 second hold, still timeout");
                return -OAL_ETIMEDOUT;
            } else {
                /* ǿ�Ƶ��ȳɹ���˵���п�����GPIO�ж�δ��Ӧ */
                oal_io_print(KERN_WARNING "[E]retry succ, maybe gpio interrupt issue");
                declare_dft_trace_key_info("pcie gpio int issue", OAL_DFT_TRACE_FAIL);
            }
        }

        if (g_pcie_ringbuf_bugfix_enable == 0) {
            ret = oal_pcie_ringbuf_h2d_refresh(pst_pci_lres->pst_pci_res);
            if (ret != OAL_SUCC) {
                return ret;
            }
        }

        mutex_lock(&pst_pci_lres->pst_pci_res->st_rx_mem_lock);
        oal_pcie_change_link_state(pst_pci_lres->pst_pci_res, PCI_WLAN_LINK_WORK_UP);
        mutex_unlock(&pst_pci_lres->pst_pci_res->st_rx_mem_lock);

        hcc_enable(hbus_to_hcc(pst_bus), OAL_TRUE);
    }

    if (action == HCC_BUS_SW_POWER_PATCH_LAUCH) {
        oal_io_print("HCC_BUS_SW_POWER_PATCH_LAUCH\n");

        /* Patch������� ��ʼ��ͨ����Դ��Ȼ��ȴ�ҵ���ʼ����� */
        ret = oal_pcie_transfer_res_init(pst_pci_lres->pst_pci_res);
        if (ret) {
            oal_io_print(KERN_ERR "pcie_transfer_res_init failed, ret=%d\n", ret);
            return ret;
        }

        oal_pcie_host_ip_init(pst_pci_lres);

        oal_wlan_gpio_intr_enable(hbus_to_dev(pst_bus), OAL_TRUE);

        oal_enable_pcie_irq(pst_pci_lres);

        /* ��ʱGPIO����ʹ�ã�BUS��δ�л� */
        ret = oal_pcie_enable_device_func_polling(pst_pci_lres);
        if (ret != OAL_SUCC) {
            oal_io_print("enable pcie device func failed, ret=%d\n", ret);
            return ret;
        }
    }

    return OAL_SUCC;
}

OAL_STATIC OAL_INLINE void oal_ete_bus_power_down(oal_pcie_linux_res *pst_pci_lres, hcc_bus *pst_bus)
{
    /* should stop ete when wifi poweroff too */
    oal_pcie_host_ip_ete_exit(pst_pci_lres);
    pcie_bus_power_down_action(pst_bus);
}

OAL_STATIC OAL_INLINE int32_t oal_ete_bus_power_up(oal_pcie_linux_res *pst_pci_lres, hcc_bus *pst_bus)
{
    int32_t ret = OAL_SUCC;
    /* �ϵ�֮ǰ��PCIE HOST ������ */
#if defined(CONFIG_ARCH_KIRIN_PCIE) || defined(_PRE_CONFIG_ARCH_HI1620S_KUNPENG_PCIE)
    declare_time_cost_stru(cost);
    oal_get_time_cost_start(cost);

    pst_pci_lres->l1_err_cnt = 0;

    ret = oal_pcie_pm_control(pst_pci_lres->pst_pcie_dev, g_kirin_rc_idx, 1);
    if (g_pcie_print_once) {
        g_pcie_print_once = 0;
        ssi_dump_device_regs(SSI_MODULE_MASK_COMM |
                             SSI_MODULE_MASK_PCIE_CFG |
                             SSI_MODULE_MASK_PCIE_DBI |
                             SSI_MODULE_MASK_WCTRL |
                             SSI_MODULE_MASK_BCTRL);
    }
    board_host_wakeup_dev_set(0);
    oal_pcie_power_notifiy_register(pst_pci_lres->pst_pcie_dev, g_kirin_rc_idx, NULL, NULL, NULL);
    if (ret) {
        oal_io_print(KERN_ERR "pcie power on and link failed, ret=%d\n", ret);
        (void)ssi_dump_err_regs(SSI_ERR_PCIE_POWER_UP_FAIL);
        return ret;
    }

#if !defined(_PRE_HI375X_PCIE) && !defined(_PRE_PRODUCT_HI1620S_KUNPENG)
    kirin_pcie_register_event(&pst_pci_lres->pcie_event);
#endif
#ifdef _PRE_PLAT_FEATURE_HI110X_DUAL_PCIE
    ret = oal_ete_dual_pci_power_up(pst_pci_lres);
    if (ret) {
        return ret;
    }
#endif

    pst_pci_lres->power_status = PCIE_EP_IP_POWER_UP;
    if (pst_pci_lres->pst_pci_res) {
        mutex_lock(&pst_pci_lres->pst_pci_res->st_rx_mem_lock);
        oal_pcie_change_link_state(pst_pci_lres->pst_pci_res, PCI_WLAN_LINK_UP);
        mutex_unlock(&pst_pci_lres->pst_pci_res->st_rx_mem_lock);
    }

    oal_get_time_cost_end(cost);
    oal_calc_time_cost_sub(cost);
    pci_print_log(PCI_LOG_INFO, "pcie link cost %llu us", time_cost_var_sub(cost));
#endif
    return ret;
}

OAL_STATIC OAL_INLINE int32_t oal_ete_bus_power_patch_lauch(oal_pcie_linux_res *pst_pci_lres, hcc_bus *pst_bus)
{
    int32_t ret;
    oal_io_print("power patch lauch\n");

    /* Patch������� ��ʼ��ͨ����Դ��Ȼ��ȴ�ҵ���ʼ����� */
    ret = oal_pcie_transfer_res_init(pst_pci_lres->pst_pci_res);
    if (ret) {
        oal_io_print(KERN_ERR "pcie_transfer_res_init failed, ret=%d\n", ret);
        return ret;
    }

    oal_pcie_host_ip_init(pst_pci_lres);

    oal_wlan_gpio_intr_enable(hbus_to_dev(pst_bus), OAL_TRUE);

    oal_enable_pcie_irq(pst_pci_lres);

    ret = oal_pcie_enable_device_func(pst_pci_lres);
    if (ret != OAL_SUCC) {
        oal_io_print("enable pcie device func failed, ret=%d\n", ret);
        return ret;
    }

    if (oal_wait_for_completion_timeout(&pst_bus->st_device_ready,
                                        (uint32_t)oal_msecs_to_jiffies(HOST_WAIT_BOTTOM_INIT_TIMEOUT)) == 0) {
        oal_io_print(KERN_ERR "wait device ready timeout... %d ms \n", HOST_WAIT_BOTTOM_INIT_TIMEOUT);
        up(&pst_bus->rx_sema);
        if (oal_wait_for_completion_timeout(&pst_bus->st_device_ready,
                                            (uint32_t)oal_msecs_to_jiffies(5000)) == 0) {
            oal_io_print(KERN_ERR "retry 5 second hold, still timeout");
            return -OAL_ETIMEDOUT;
        } else {
            /* ǿ�Ƶ��ȳɹ���˵���п�����GPIO�ж�δ��Ӧ */
            oal_io_print(KERN_WARNING "[E]retry succ, maybe gpio interrupt issue");
            declare_dft_trace_key_info("pcie gpio int issue", OAL_DFT_TRACE_FAIL);
        }
    }

    /* ringbuf rebuild, after device ete late init! after ete clk en */
    ret = oal_ete_rx_ringbuf_build(pst_pci_lres->pst_pci_res);
    if (ret != OAL_SUCC) {
        declare_dft_trace_key_info("oal_ete_rx_ringbuf_build failed", OAL_DFT_TRACE_FAIL);
    }

    mutex_lock(&pst_pci_lres->pst_pci_res->st_rx_mem_lock);
    oal_pcie_change_link_state(pst_pci_lres->pst_pci_res, PCI_WLAN_LINK_WORK_UP);
    mutex_unlock(&pst_pci_lres->pst_pci_res->st_rx_mem_lock);

    hcc_enable(hbus_to_hcc(pst_bus), OAL_TRUE);

    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_ete_power_action(hcc_bus *pst_bus, HCC_BUS_POWER_ACTION_TYPE action)
{
    int32_t ret = OAL_SUCC;
    oal_pcie_linux_res *pst_pci_lres;

    pst_pci_lres = (oal_pcie_linux_res *)pst_bus->data;
    if (oal_warn_on(pst_pci_lres == NULL)) {
        return -OAL_ENODEV;
    }

    if (action == HCC_BUS_POWER_DOWN) {
        oal_ete_bus_power_down(pst_pci_lres, pst_bus);
    }

    if (action == HCC_BUS_POWER_UP) {
        ret = oal_ete_bus_power_up(pst_pci_lres, pst_bus);
    }

    if (action == HCC_BUS_POWER_PATCH_LOAD_PREPARE) {
        /* close hcc */
        hcc_disable(hbus_to_hcc(pst_bus), OAL_TRUE);
        oal_init_completion(&pst_bus->st_device_ready);
        oal_wlan_gpio_intr_enable(hbus_to_dev(pst_bus), OAL_FALSE);
    }

    if (action == HCC_BUS_SW_POWER_PATCH_LOAD_PREPARE) {
        oal_init_completion(&pst_bus->st_device_ready);
    }

#ifdef _PRE_SDIO_TAE_DEBUG
    oal_pcie_power_action_tae(pst_pci_lres, action);
#endif

    if (action == HCC_BUS_POWER_PATCH_LAUCH) {
        ret = oal_ete_bus_power_patch_lauch(pst_pci_lres, pst_bus);
    }

    return ret;
}

OAL_STATIC int32_t oal_pcie_patch_read(hcc_bus *pst_bus, uint8_t *buff, int32_t len, uint32_t timeout)
{
    int32_t ret;
    oal_pcie_linux_res *pst_pci_lres;

    pst_pci_lres = (oal_pcie_linux_res *)pst_bus->data;
    if (oal_warn_on(pst_pci_lres == NULL)) {
        return -OAL_ENODEV;
    }

    if (oal_likely(pst_pci_lres->pst_pci_res)) {
        mutex_lock(&pst_pci_lres->pst_pci_res->st_rx_mem_lock);
    }

    ret = oal_pcie_firmware_read(pst_pci_lres, buff, len, timeout);

    if (oal_likely(pst_pci_lres->pst_pci_res)) {
        mutex_unlock(&pst_pci_lres->pst_pci_res->st_rx_mem_lock);
    }

    return ret;
}

OAL_STATIC int32_t oal_pcie_patch_write(hcc_bus *pst_bus, uint8_t *buff, int32_t len)
{
    int32_t ret;
    oal_pcie_linux_res *pst_pci_lres;

    pst_pci_lres = (oal_pcie_linux_res *)pst_bus->data;
    if (oal_warn_on(pst_pci_lres == NULL)) {
        oal_io_print("oal_pcie_patch_write failed, lres is null\n");
        return -OAL_ENODEV;
    }

    if (oal_likely(pst_pci_lres->pst_pci_res)) {
        mutex_lock(&pst_pci_lres->pst_pci_res->st_rx_mem_lock);
    }
    ret = oal_pcie_firmware_write(pst_pci_lres, buff, len);
    if (oal_likely(pst_pci_lres->pst_pci_res)) {
        mutex_unlock(&pst_pci_lres->pst_pci_res->st_rx_mem_lock);
    }
    return ret;
}

OAL_STATIC int32_t oal_pcie_gpio_irq(hcc_bus *hi_bus, int32_t irq)
{
    oal_uint ul_state;
    oal_pcie_linux_res *pst_pci_lres;
    struct wlan_pm_s *pst_wlan_pm = wlan_pm_get_drv();
    pst_pci_lres = (oal_pcie_linux_res *)hi_bus->data;

    if (oal_unlikely(hi_bus == NULL)) {
        oal_io_print(KERN_ERR "pcie bus is null, irq:%d\n", irq);
        return -OAL_EINVAL;
    }

    if (!hi_bus->pst_pm_callback || !hi_bus->pst_pm_callback->pm_state_get) {
        oal_io_print("GPIO interrupt function param is NULL\r\n");
        return -OAL_EINVAL;
    }

    hi_bus->gpio_int_count++;

    if (oal_atomic_read(&g_wakeup_dev_wait_ack)) {
        pci_print_log(PCI_LOG_INFO, "pcie_gpio_irq wakeup dev ack");
        hi_bus->pst_pm_callback->pm_wakeup_dev_ack();
    }

    if (oal_atomic_read(&g_bus_powerup_dev_wait_ack)) {
        pci_print_log(PCI_LOG_INFO, "pcie_gpio_irq powerup dev ack");
        if (oal_likely(pst_wlan_pm != NULL)) {
            oal_complete(&pst_wlan_pm->st_wifi_powerup_done);
        } else {
            pci_print_log(PCI_LOG_INFO, "pst_wlan_pm is null");
        }
    }

    if (oal_unlikely(pst_pci_lres != NULL)) {
        if (oal_atomic_read(&pst_pci_lres->st_pcie_wakeup_flag)) {
            pci_print_log(PCI_LOG_INFO, "pcie_gpio_irq dev ready ack");
            if (oal_likely(pst_wlan_pm != NULL)) {
                oal_complete(&pst_wlan_pm->st_wifi_powerup_done);
                up(&hi_bus->rx_sema);
            } else {
                pci_print_log(PCI_LOG_INFO, "pst_wlan_pm is null");
            }
        }
    }

    ul_state = hi_bus->pst_pm_callback->pm_state_get();

    if (ul_state == 0) {
        /* 0==HOST_DISALLOW_TO_SLEEP��ʾ���������� */
        hi_bus->data_int_count++;
        /* PCIE message use gpio interrupt */
        pci_print_log(PCI_LOG_DBG, "pcie message come..");
        up(&hi_bus->rx_sema);

    } else {
        /* 1==HOST_ALLOW_TO_SLEEP��ʾ��ǰ�����ߣ�����host */
        if (oal_warn_on(!hi_bus->pst_pm_callback->pm_wakeup_host)) {
            pci_print_log(PCI_LOG_ERR, "%s error:hi_bus->pst_pm_callback->pm_wakeup_host is null", __FUNCTION__);
            return -OAL_FAIL;
        }

        hi_bus->wakeup_int_count++;
    pci_print_log(PCI_LOG_INFO, "trigger wakeup work...");
        hi_bus->pst_pm_callback->pm_wakeup_host();
    }
    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_gpio_rx_data(hcc_bus *hi_bus)
{
    int32_t ret;
    uint32_t message;
    oal_pcie_linux_res *pst_pci_lres = (oal_pcie_linux_res *)hi_bus->data;
    if (oal_warn_on(pst_pci_lres == NULL)) {
        return -OAL_EINVAL;
    }

    /* read message from device, read until ringbuf empty */
    forever_loop() {
        if (!pst_pci_lres->pst_pci_res->regions.inited) {
            oal_io_print("pcie rx data region is disabled!\n");
            return -OAL_ENODEV;
        }

        /* read one message */
        hcc_bus_wake_lock(hi_bus);
        mutex_lock(&pst_pci_lres->pst_pci_res->st_rx_mem_lock);
        ret = oal_pcie_read_d2h_message(pst_pci_lres->pst_pci_res, &message);
        mutex_unlock(&pst_pci_lres->pst_pci_res->st_rx_mem_lock);
        hcc_bus_wake_unlock(hi_bus);
        if (ret != OAL_SUCC) {
            break;
        }

        pci_print_log(PCI_LOG_DBG, "pci  message : %u", message);
        /* get message succ. */
        if (message < D2H_MSG_COUNT) {
            /* standard message */
            hi_bus->msg[message].count++;
            hi_bus->last_msg = message;
            hi_bus->msg[message].cpu_time = cpu_clock(UINT_MAX);
            if (hi_bus->msg[message].msg_rx) {
                hi_bus->msg[message].msg_rx(hi_bus->msg[message].data);
            } else {
                oal_io_print("pcie mssage :%u no callback functions\n", message);
            }
        } else {
            if (message == PCIE_D2H_MESSAGE_HOST_READY_ACK) {
                pci_print_log(PCI_LOG_INFO, "d2h host read ack");
                oal_complete(&pst_pci_lres->st_pcie_ready);
            }
        }
    }

    return OAL_SUCC;
}

OAL_STATIC void oal_pcie_print_trans_info(hcc_bus *hi_bus, uint64_t print_flag)
{
    oal_pcie_linux_res *pst_pci_lres = (oal_pcie_linux_res *)hi_bus->data;
    if (oal_warn_on(pst_pci_lres == NULL)) {
        return;
    }

    oal_pcie_print_transfer_info(pst_pci_lres->pst_pci_res, print_flag);
}

OAL_STATIC void oal_pcie_reset_trans_info(hcc_bus *hi_bus)
{
    oal_pcie_linux_res *pst_pci_lres = (oal_pcie_linux_res *)hi_bus->data;
    if (oal_warn_on(pst_pci_lres == NULL)) {
        return;
    }

    oal_pcie_reset_transfer_info(pst_pci_lres->pst_pci_res);
}

OAL_STATIC void oal_ete_print_trans_info(hcc_bus *hi_bus, uint64_t print_flag)
{
    oal_pcie_linux_res *pst_pci_lres = (oal_pcie_linux_res *)hi_bus->data;
    if (oal_warn_on(pst_pci_lres == NULL)) {
        return;
    }

    oal_ete_print_transfer_info(pst_pci_lres->pst_pci_res, print_flag);
}

OAL_STATIC void oal_ete_reset_trans_info(hcc_bus *hi_bus)
{
    oal_pcie_linux_res *pst_pci_lres = (oal_pcie_linux_res *)hi_bus->data;
    if (oal_warn_on(pst_pci_lres == NULL)) {
        return;
    }

    oal_ete_reset_transfer_info(pst_pci_lres->pst_pci_res);
}


OAL_STATIC int32_t oal_pcie_edma_pending_signal_check(hcc_bus *hi_bus)
{
    oal_pcie_linux_res *pst_pci_lres = (oal_pcie_linux_res *)hi_bus->data;
    if (oal_unlikely(pst_pci_lres == NULL)) {
        pci_print_log(PCI_LOG_INFO, "pst_pci_lres is null");
        return OAL_FALSE;
    }
    return oal_pcie_host_pending_signal_check(pst_pci_lres->pst_pci_res);
}

OAL_STATIC int32_t oal_pcie_edma_pending_signal_process(hcc_bus *hi_bus)
{
    int32_t ret = 0;
    oal_pcie_linux_res *pst_pci_lres = (oal_pcie_linux_res *)hi_bus->data;
    if (oal_unlikely(pst_pci_lres == NULL)) {
        pci_print_log(PCI_LOG_INFO, "pst_pci_lres is null");
        return 0;
    }

    if (OAL_TRUE == oal_pcie_host_pending_signal_check(pst_pci_lres->pst_pci_res)) {
        hcc_tx_transfer_lock(hi_bus->bus_dev->hcc);
        if (OAL_TRUE == oal_pcie_host_pending_signal_check(pst_pci_lres->pst_pci_res)) { /* for wlan power off */
            ret = oal_pcie_host_pending_signal_process(pst_pci_lres->pst_pci_res);
        } else {
            pci_print_log(PCI_LOG_INFO, "pcie tx pending signal was cleared");
        }
        hcc_tx_transfer_unlock(hi_bus->bus_dev->hcc);
    }
    return ret;
}


OAL_STATIC int32_t oal_pcie_ete_pending_signal_check(hcc_bus *hi_bus)
{
    oal_pcie_linux_res *pst_pci_lres = (oal_pcie_linux_res *)hi_bus->data;
    if (oal_unlikely(pst_pci_lres == NULL)) {
        pci_print_log(PCI_LOG_INFO, "pst_pci_lres is null");
        return OAL_FALSE;
    }
    return oal_ete_host_pending_signal_check(pst_pci_lres->pst_pci_res);
}

OAL_STATIC int32_t oal_pcie_ete_pending_signal_process(hcc_bus *hi_bus)
{
    int32_t ret = 0;
    oal_pcie_linux_res *pst_pci_lres = (oal_pcie_linux_res *)hi_bus->data;
    if (oal_unlikely(pst_pci_lres == NULL)) {
        pci_print_log(PCI_LOG_INFO, "pst_pci_lres is null");
        return 0;
    }

    if (OAL_TRUE == oal_ete_host_pending_signal_check(pst_pci_lres->pst_pci_res)) {
        hcc_tx_transfer_lock(hi_bus->bus_dev->hcc);
        if (OAL_TRUE == oal_ete_host_pending_signal_check(pst_pci_lres->pst_pci_res)) { /* for wlan power off */
            ret = oal_ete_host_pending_signal_process(pst_pci_lres->pst_pci_res);
        } else {
            pci_print_log(PCI_LOG_INFO, "pcie tx pending signal was cleared");
        }
        hcc_tx_transfer_unlock(hi_bus->bus_dev->hcc);
    }
    return ret;
}

OAL_STATIC int32_t oal_pcie_master_address_switch(hcc_bus *hi_bus, uint64_t src_addr,
                                                  uint64_t* dst_addr, int32_t is_host_va)
{
    int32_t ret;
    pci_addr_map addr_map;

    oal_pcie_res *pst_pci_res = NULL;
    oal_pcie_linux_res *pst_pci_lres = (oal_pcie_linux_res *)hi_bus->data;
    if (oal_unlikely(pst_pci_lres == NULL)) {
        pci_print_log(PCI_LOG_INFO, "pst_pci_lres is null");
        return -OAL_ENODEV;
    }

    pst_pci_res = pst_pci_lres->pst_pci_res;

    if (oal_unlikely(pst_pci_res == NULL)) {
        pci_print_log(PCI_LOG_INFO, "pst_pci_res is null");
        return -OAL_ENODEV;
    }

    if (oal_warn_on(dst_addr == NULL)) {
        return -OAL_EINVAL;
    }

    *dst_addr = 0x0;

    if (is_host_va == OAL_FALSE) {
        /* device cpu address -> host virtual address */
        ret = oal_pcie_inbound_ca_to_va(pst_pci_res, src_addr, &addr_map);
        if (ret == OAL_SUCC) {
            *dst_addr = (uint64_t)addr_map.va;
        }
        return ret;
    } else {
        /* host virtual address -> device cpu address */
        ret = oal_pcie_inbound_va_to_ca(pst_pci_res, src_addr, dst_addr);
        return ret;
    }
}

OAL_STATIC int32_t oal_pcie_slave_address_switch(hcc_bus *hi_bus, uint64_t src_addr,
                                                 uint64_t* dst_addr, int32_t is_host_iova)
{
    oal_pcie_res *pst_pci_res = NULL;
    oal_pcie_linux_res *pst_pci_lres = (oal_pcie_linux_res *)hi_bus->data;
    if (oal_unlikely(pst_pci_lres == NULL)) {
        pci_print_log(PCI_LOG_INFO, "pst_pci_lres is null");
        return -OAL_ENODEV;
    }

    pst_pci_res = pst_pci_lres->pst_pci_res;

    if (oal_unlikely(pst_pci_res == NULL)) {
        pci_print_log(PCI_LOG_INFO, "pst_pci_res is null");
        return -OAL_ENODEV;
    }

    if (oal_warn_on(dst_addr == NULL)) {
        return -OAL_EINVAL;
    }

    return oal_pcie_host_slave_address_switch(pst_pci_res, src_addr, dst_addr, is_host_iova);
}

#ifdef CONFIG_KIRIN_PCIE_L1SS_IDLE_SLEEP
OAL_STATIC void oal_pcie_wlan_pm_vote(hcc_bus *hi_bus, uint8_t uc_allow)
{
    /* PCIe 32K feature, control rc fnpll clock */
    /* 0 means allow to fnpll gating */
    /* 1 means disallow to fnpll gating */
    kirin_pcie_refclk_device_vote(PCIE_DEVICE_WLAN, g_kirin_rc_idx, (uc_allow == ALLOW_IDLESLEEP) ? 0 : 1);
}
#endif

OAL_STATIC hcc_bus_opt_ops g_pcie_edma_opt_ops = {
    .get_bus_state = oal_pcie_get_state,
    .disable_bus_state = oal_disable_pcie_state,
    .enable_bus_state = oal_enable_pcie_state,
    .rx_netbuf_list = oal_pcie_rx_netbuf,
    .tx_netbuf_list = oal_pcie_edma_tx_netbuf,
    .send_msg = oal_pcie_send_msg,
    .lock = oal_pcie_host_lock,
    .unlock = oal_pcie_host_unlock,
    .sleep_request = oal_pcie_sleep_request,
    .sleep_request_host = oal_pcie_edma_sleep_request_host,
    .wakeup_request = oal_pcie_wakeup_request,
    .get_sleep_state = oal_pcie_get_sleep_state,
    .wakeup_complete = oal_pcie_edma_wakeup_complete,
    .rx_int_mask = oal_pcie_rx_int_mask,
    .power_action = oal_pcie_edma_power_action,
    .power_ctrl = oal_pcie_power_ctrl,
    .wlan_gpio_handler = oal_pcie_gpio_irq,
    .wlan_gpio_rxdata_proc = oal_pcie_gpio_rx_data,
    .reinit = oal_pcie_reinit,
    .deinit = oal_pcie_deinit,
    .tx_condition = oal_pcie_edma_tx_condition,
    .patch_read = oal_pcie_patch_read,
    .patch_write = oal_pcie_patch_write,
#ifdef _PRE_PRODUCT_HI1620S_KUNPENG
    .bindcpu = NULL,
#else
    .bindcpu = oal_pcie_bindcpu,
#endif
    .switch_suspend_tx = NULL,
    .get_trans_count = oal_pcie_get_trans_count,
    .switch_clean_res = oal_pcie_switch_clean_res,
    .voltage_bias_init = oal_pcie_ip_voltage_bias_init,
    .chip_info = oal_pcie_chip_info,

    .print_trans_info = oal_pcie_print_trans_info,
    .reset_trans_info = oal_pcie_reset_trans_info,
    .pending_signal_check = oal_pcie_edma_pending_signal_check,
    .pending_signal_process = oal_pcie_edma_pending_signal_process,
    .master_address_switch = NULL,
    .slave_address_switch = NULL,
#ifdef CONFIG_KIRIN_PCIE_L1SS_IDLE_SLEEP
    .wlan_pm_vote = oal_pcie_wlan_pm_vote,
#endif
};

OAL_STATIC hcc_bus_opt_ops g_pcie_ete_opt_ops = {
    .get_bus_state = oal_pcie_get_state,
    .disable_bus_state = oal_disable_pcie_state,
    .enable_bus_state = oal_enable_pcie_state,
    .rx_netbuf_list = oal_pcie_rx_netbuf,
    .tx_netbuf_list = oal_pcie_ete_tx_netbuf,
    .send_msg = oal_pcie_send_msg,
    .lock = oal_pcie_host_lock,
    .unlock = oal_pcie_host_unlock,
    .sleep_request = oal_pcie_sleep_request,
    .sleep_request_host = oal_pcie_ete_sleep_request_host,
    .wakeup_request = oal_pcie_wakeup_request,
    .get_sleep_state = oal_pcie_get_sleep_state,
    .wakeup_complete = oal_pcie_ete_wakeup_complete,
    .rx_int_mask = oal_pcie_rx_int_mask,
    .power_action = oal_pcie_ete_power_action,
    .power_ctrl = oal_pcie_power_ctrl,
    .wlan_gpio_handler = oal_pcie_gpio_irq,
    .wlan_gpio_rxdata_proc = oal_pcie_gpio_rx_data,
    .reinit = oal_pcie_reinit,
    .deinit = oal_pcie_deinit,
    .tx_condition = oal_pcie_ete_tx_condition,
    .patch_read = oal_pcie_patch_read,
    .patch_write = oal_pcie_patch_write,
#ifdef _PRE_PRODUCT_HI1620S_KUNPENG
    .bindcpu = NULL,
#else
    .bindcpu = oal_pcie_bindcpu,
#endif
    .switch_suspend_tx = NULL,
    .get_trans_count = NULL,
    .switch_clean_res = NULL,
    .voltage_bias_init = NULL,
    .chip_info = NULL,

    .print_trans_info = oal_ete_print_trans_info,
    .reset_trans_info = oal_ete_reset_trans_info,
    .pending_signal_check = oal_pcie_ete_pending_signal_check,
    .pending_signal_process = oal_pcie_ete_pending_signal_process,
    .master_address_switch = oal_pcie_master_address_switch,
    .slave_address_switch = oal_pcie_slave_address_switch,
#ifdef CONFIG_KIRIN_PCIE_L1SS_IDLE_SLEEP
    .wlan_pm_vote = oal_pcie_wlan_pm_vote,
#endif
};


OAL_STATIC hcc_bus *oal_pcie_bus_init(oal_pcie_linux_res *pst_pci_lres)
{
    int32_t ret;
    hcc_bus *pst_bus = OAL_PTR_NULL;

    pst_bus = hcc_alloc_bus();
    if (pst_bus == OAL_PTR_NULL) {
        oal_io_print("alloc pcie hcc bus failed, size:%u\n", (uint32_t)OAL_SIZEOF(hcc_bus));
        return NULL;
    }

    pst_bus->bus_type = HCC_BUS_PCIE;
    pst_bus->bus_id = 0x0;
    pst_bus->dev_id = HCC_CHIP_110X_DEV; /* ������Ը��� vendor id ����110X ��118X */

    /* PCIE ֻ��Ҫ4�ֽڶ���, burst��С�����ܵ�Ӱ������ */
    pst_bus->cap.align_size[HCC_TX] = sizeof(uint32_t);
    pst_bus->cap.align_size[HCC_RX] = sizeof(uint32_t);
    pst_bus->cap.max_trans_size = 0x7fffffff;

    if (pst_pci_lres->pst_pci_res->chip_info.edma_support == OAL_TRUE) {
        pst_bus->opt_ops = &g_pcie_edma_opt_ops;
    } else if (pst_pci_lres->pst_pci_res->chip_info.ete_support == OAL_TRUE) {
        pst_bus->opt_ops = &g_pcie_ete_opt_ops;
    } else {
        pst_bus->opt_ops = NULL;
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "unsupport chip, bus init failed!\n");
        hcc_free_bus(pst_bus);
        return NULL;
    }

    pst_bus->data = (void *)pst_pci_lres;

    ret = hcc_add_bus(pst_bus, "pcie");
    if (ret) {
        oal_io_print("add pcie bus failed, ret=%d\n", ret);
        hcc_free_bus(pst_bus);
        return NULL;
    }

    pst_pci_lres->pst_bus = pst_bus;
    return pst_bus;
}

OAL_STATIC void oal_pcie_bus_exit(hcc_bus *pst_bus)
{
    hcc_remove_bus(pst_bus);
    hcc_free_bus(pst_bus);
}

#if defined(CONFIG_ARCH_KIRIN_PCIE) || defined(_PRE_CONFIG_ARCH_HI1620S_KUNPENG_PCIE)
int32_t wlan_first_power_on_callback(void *data)
{
    oal_reference(data);
    /* before wlan chip power up, rc clock must on. */
    /* bootloader had delay 20us before reset pcie, soc requet 10us delay, need't delay here */
    hi_wlan_power_set(1);
    return 0;
}

int32_t wlan_first_power_off_fail_callback(void *data)
{
    oal_reference(data);
    /* ��ֹ����ö��ʧ�ܺ��µ��ʱ�� */
    oal_io_print("wlan_first_power_off_fail_callback\n");
    g_pcie_enum_fail_reg_dump_flag = 1;
    (void)ssi_dump_err_regs(SSI_ERR_PCIE_FST_POWER_OFF_FAIL);
    oal_chip_error_task_block();
    return -1;
}

#ifdef _PRE_PLAT_FEATURE_HI110X_DUAL_PCIE
OAL_STATIC int32_t oal_pcie_dual_power_on_callback(void *data)
{
    oal_reference(data);
    oal_io_print("board_set_pcie_reset\n");
    board_set_pcie_reset(OAL_FALSE, OAL_TRUE);
    mdelay(10); /* need't delay */
    return 0;
}

OAL_STATIC int32_t oal_pcie_dual_power_off_fail_callback(void *data)
{
    oal_reference(data);
    /* ��ֹ����ö��ʧ�ܺ��µ��ʱ�� */
    oal_io_print("oal_pcie_dual_power_off_fail_callback\n");
    g_pcie_enum_fail_reg_dump_flag = 1;
    (void)ssi_dump_err_regs(SSI_ERR_PCIE_FST_POWER_OFF_FAIL);
    oal_chip_error_task_block();
    return -1;
}

OAL_STATIC int32_t wlan_dual_first_power_off_callback(void *data)
{
    oal_reference(data);
    board_set_pcie_reset(OAL_FALSE, OAL_FALSE);
    return 0;
}
#endif
int32_t wlan_first_power_off_callback(void *data)
{
    oal_reference(data);
    hi_wlan_power_set(0);
    return 0;
}
#endif

OAL_STATIC void oal_pcie_voltage_bias_param_init(void)
{
    char *token = NULL;
    const uint32_t bias_param_size = 100;
    const uint32_t param_buff_size = 10;
    char bias_param[bias_param_size];
    uint32_t param[param_buff_size];
    uint32_t param_nums;
    char *bias_buf = bias_param;

    memset_s(bias_param, sizeof(bias_param), 0, sizeof(bias_param));
    memset_s(param, sizeof(param), 0, sizeof(param));

    if (get_cust_conf_string(INI_MODU_PLAT, "pcie_vol_bias_param", bias_param, sizeof(bias_param) - 1) == INI_FAILED) {
        return;
    }

    param_nums = 0;
    forever_loop() {
        token = strsep(&bias_buf, ",");
        if (token == NULL) {
            break;
        }
        if (param_nums >= sizeof(param) / sizeof(uint32_t)) {
            pci_print_log(PCI_LOG_ERR, "too many bias param:%u", param_nums);
            return;
        }

        param[param_nums++] = oal_strtol(token, NULL, 10); /* 10���� */
        pci_print_log(PCI_LOG_INFO, "bias param %u , value is %u", param_nums, param[param_nums - 1]);
    }

    oal_pcie_set_voltage_bias_param(param[0], param[1]);
}

OAL_STATIC void oal_pcie_ringbuf_bugfix_init(void)
{
    char buff[100]; /* �����ԭ���ݲ�֧����const�������������С */

    memset_s(buff, sizeof(buff), 0, sizeof(buff));

    if (get_cust_conf_string(INI_MODU_PLAT, "pcie_ringbuf_bugfix", buff, sizeof(buff) - 1) == INI_FAILED) {
        pci_print_log(PCI_LOG_INFO, "can't found ini:pcie_ringbuf_bugfix, bugfix stat:%d", g_pcie_ringbuf_bugfix_enable);
        return;
    }

    if (!oal_strncmp("enable", buff, OAL_STRLEN("enable"))) {
        g_pcie_ringbuf_bugfix_enable = 1;
    } else if (!strncmp("disable", buff, OAL_STRLEN("disable"))) {
        g_pcie_ringbuf_bugfix_enable = 0;
    } else {
        pci_print_log(PCI_LOG_WARN, "unkdown ringbuf bugfix ini:%s", buff);
    }

    pci_print_log(PCI_LOG_INFO, "ringbuf bugfix %s", g_pcie_ringbuf_bugfix_enable ? "enable" : "disable");
}

#ifdef _PRE_PRODUCT_HI1620S_KUNPENG
OAL_STATIC int32_t oal_pcie_dts_init(void)
{
    return OAL_SUCC;
}
#else

OAL_STATIC int32_t oal_pcie_dts_init(void)
{
#ifndef _PRE_HI375X_PCIE
#ifdef _PRE_CONFIG_USE_DTS
    int ret;
    u32 pcie_rx_idx = 0;
    struct device_node *np = NULL;
    np = of_find_compatible_node(NULL, NULL, DTS_NODE_HI110X_WIFI);
    if (np == NULL) {
        oal_print_hi11xx_log(HI11XX_LOG_ERR, "can't find node [%s]", DTS_NODE_HI110X_WIFI);
        return -OAL_ENODEV;
    }

    ret = of_property_read_u32(np, DTS_PROP_HI110X_PCIE_RC_IDX, &pcie_rx_idx);
    if (ret) {
        oal_print_hi11xx_log(HI11XX_LOG_INFO, "read prop [%s] fail, ret=%d", DTS_PROP_HI110X_PCIE_RC_IDX, ret);
        return ret;
    }

    oal_print_hi11xx_log(HI11XX_LOG_INFO, "%s=%d", DTS_PROP_HI110X_PCIE_RC_IDX, pcie_rx_idx);
#ifdef CONFIG_ARCH_KIRIN_PCIE
    g_kirin_rc_idx = pcie_rx_idx;
#endif
#endif
#endif
    return OAL_SUCC;
}
#endif

OAL_STATIC int32_t oal_pci_resource_init(void)
{
    int32_t ret;
    oal_pcie_linux_res *pst_pci_lres = NULL;

    /* Power off Wlan Chip */
    if (pcie_linux_res) {
        hcc_bus_disable_state(pcie_linux_res->pst_bus, OAL_BUS_STATE_ALL);
#if defined(CONFIG_ARCH_KIRIN_PCIE) || defined(_PRE_CONFIG_ARCH_HI1620S_KUNPENG_PCIE)
        pst_pci_lres = (oal_pcie_linux_res *)pcie_linux_res->pst_bus->data;
#endif

        /* ����PCIE ���üĴ��� */
        ret = oal_pcie_save_default_resource(pcie_linux_res);
        if (ret != OAL_SUCC) {
            oal_pcie_disable_device(pcie_linux_res);
#ifdef _PRE_CONFIG_PCIE_SHARED_INTX_IRQ
            oal_disable_pcie_irq_with_free(pcie_linux_res);
#else
            oal_disable_pcie_irq(pcie_linux_res);
#endif
            oal_pci_unregister_driver(&g_pcie_drv);
            hi_wlan_power_set(0);
#if defined(CONFIG_ARCH_KIRIN_PCIE) || defined(_PRE_CONFIG_ARCH_HI1620S_KUNPENG_PCIE)
            oal_pcie_power_notifiy_register(pst_pci_lres->pst_pcie_dev, g_kirin_rc_idx, NULL, NULL, NULL);
#endif
            return ret;
        }

#if defined(CONFIG_ARCH_KIRIN_PCIE) || defined(_PRE_CONFIG_ARCH_HI1620S_KUNPENG_PCIE)
        oal_pcie_power_notifiy_register(pst_pci_lres->pst_pcie_dev, g_kirin_rc_idx, NULL,
                                        wlan_first_power_off_callback, NULL);
#endif
#ifdef _PRE_CONFIG_PCIE_SHARED_INTX_IRQ
        oal_disable_pcie_irq_with_free(pcie_linux_res);
#else
        oal_disable_pcie_irq(pcie_linux_res);
#endif
        oal_pcie_edma_power_action(pcie_linux_res->pst_bus, HCC_BUS_POWER_DOWN);
        hi_wlan_power_set(0);
        }else {
        oal_io_print("pcie_linux_res is null\n");
#ifdef CONFIG_ARCH_KIRIN_PCIE
        kirin_pcie_power_notifiy_register(g_kirin_rc_idx, NULL, NULL, NULL);
#endif
        return -OAL_ENODEV;
    }
    return OAL_SUCC;
}

OAL_STATIC void oal_pcie_110x_enum_fail_proc(void)
{
    if (!g_pcie_enum_fail_reg_dump_flag) {
        (void)ssi_dump_err_regs(SSI_ERR_PCIE_ENUM_FAIL);
    }
}

OAL_STATIC int32_t oal_wait_pcie_probe_complete(void)
{
    int32_t ret = OAL_SUCC;

    if (oal_wait_for_completion_timeout(&g_probe_complete, 10 * HZ)) {
        if (g_probe_ret != OAL_SUCC) {
            oal_io_print("pcie driver probe failed ret=%d, driname:%s\n", g_probe_ret, g_pcie_drv.name);
#ifdef CONFIG_HWCONNECTIVITY_PC
            ssi_dump_device_regs(SSI_MODULE_MASK_COMM | SSI_MODULE_MASK_PCIE_CFG | SSI_MODULE_MASK_PCIE_DBI |
                SSI_MODULE_MASK_WCTRL | SSI_MODULE_MASK_BCTRL);
            chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_PLAT, CHR_LAYER_DRV,
                CHR_PLT_DRV_EVENT_INIT, CHR_PLAT_DRV_ERROR_PCIE_PROBE_FAIL);
#endif
#ifdef CONFIG_ARCH_KIRIN_PCIE
            kirin_pcie_power_notifiy_register(g_kirin_rc_idx, NULL, NULL, NULL);
#endif
            return g_probe_ret;
        }
    } else {
        oal_io_print("pcie driver probe timeout  driname:%s\n", g_pcie_drv.name);
        oal_pcie_110x_enum_fail_proc();
#ifdef CONFIG_HWCONNECTIVITY_PC
        chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_PLAT, CHR_LAYER_DRV,
            CHR_PLT_DRV_EVENT_INIT, CHR_PLAT_DRV_ERROR_PCIE_PROBE_TIMEOUT);
#endif
#ifdef CONFIG_ARCH_KIRIN_PCIE
        kirin_pcie_power_notifiy_register(g_kirin_rc_idx, NULL, NULL, NULL);
#endif
        return -OAL_ETIMEDOUT;
    }
    return ret;
}

#ifdef _PRE_PLAT_FEATURE_HI110X_DUAL_PCIE
int32_t oal_pcie_hi110x_dual_init(void)
{
#ifdef CONFIG_ARCH_KIRIN_PCIE
    int32_t ret;
#endif
    if (g_dual_pci_support == OAL_FALSE) {
        oal_io_print("dual pcie not support\n");
        return OAL_SUCC;
    }

    g_probe_ret = -OAL_EFAUL;
    g_pcie_enum_fail_reg_dump_flag = 0;
    oal_init_completion(&g_probe_complete);
    oal_io_print("%s PCIe driver dual device start\n", g_pcie_drv.name);
#ifdef CONFIG_ARCH_KIRIN_PCIE
    /* �򿪲ο�ʱ�� */
    ret = kirin_pcie_power_notifiy_register(g_kirin_rc_idx_dual, oal_pcie_dual_power_on_callback,
                                            oal_pcie_dual_power_off_fail_callback, NULL);
    if (ret != OAL_SUCC) {
        oal_io_print("enumerate_prepare failed ret=%d\n", ret);
        return ret;
    }

    ret = oal_pcie_enumerate(g_kirin_rc_idx_dual);
    if (ret != OAL_SUCC) {
        oal_io_print("enumerate failed ret=%d\n", ret);
        kirin_pcie_power_notifiy_register(g_kirin_rc_idx_dual, NULL, NULL, NULL);
        return ret;
    }

    if (pcie_linux_res == NULL || pcie_linux_res->pst_pcie_dev_dual == NULL) {
        oal_io_print("no dual pcie device\n");
        return -OAL_ENODEV;
    }

    /* poweroff */
    oal_pcie_power_notifiy_register(pcie_linux_res->pst_pcie_dev_dual, g_kirin_rc_idx_dual, NULL,
                                    wlan_dual_first_power_off_callback, NULL);
    oal_pcie_pm_control(pcie_linux_res->pst_pcie_dev_dual, g_kirin_rc_idx_dual, 2); /* power off without L3 message */
#endif
    return OAL_SUCC;
}
#endif

static void oal_pcie_110x_pre_init(void)
{
    /* init host pcie */
    oal_io_print("%s PCIe driver register start\n", g_pcie_drv.name); /* Debug */
    oal_init_completion(&g_probe_complete);

    g_probe_ret = -OAL_EFAUL;

    g_pcie_enum_fail_reg_dump_flag = 0;

    oal_pcie_dts_init();
}

/* trigger pcie probe, alloc pcie resource */
int32_t oal_pcie_110x_init(void)
{
    int32_t ret;

    oal_pcie_110x_pre_init();

    ret = oal_pci_register_driver(&g_pcie_drv);
    if (ret) {
        oal_io_print("pcie driver register failed ret=%d, driname:%s\n", ret, g_pcie_drv.name);
        return ret;
    }

#ifdef CONFIG_ARCH_KIRIN_PCIE
    /* �򿪲ο�ʱ�� */
    ret = kirin_pcie_power_notifiy_register(g_kirin_rc_idx, wlan_first_power_on_callback,
                                            wlan_first_power_off_fail_callback, NULL);
    if (ret != OAL_SUCC) {
        oal_io_print("enumerate_prepare failed ret=%d\n", ret);
        oal_pci_unregister_driver(&g_pcie_drv);
        return ret;
    }

    ret = oal_pcie_enumerate(g_kirin_rc_idx);
    if (ret != OAL_SUCC) {
        oal_pcie_110x_enum_fail_proc();
        oal_io_print("enumerate failed ret=%d\n", ret);
        kirin_pcie_power_notifiy_register(g_kirin_rc_idx, NULL, NULL, NULL);
        oal_pci_unregister_driver(&g_pcie_drv);
        return ret;
    }
#endif

    ret = oal_wait_pcie_probe_complete();
    if (ret != OAL_SUCC) {
        return ret;
    }
#ifdef _PRE_PLAT_FEATURE_HI110X_DUAL_PCIE
    ret = oal_pcie_hi110x_dual_init();
    if (ret != OAL_SUCC) {
        return ret;
    }
#endif

    oal_pcie_voltage_bias_param_init();
    oal_pcie_ringbuf_bugfix_init();

    ret = oal_pci_resource_init();
    if (ret != OAL_SUCC) {
        oal_io_print("%s PCIe driver register not succ\n", g_pcie_drv.name);
#ifdef CONFIG_HWCONNECTIVITY_PC
        chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_PLAT, CHR_LAYER_DRV,
            CHR_PLT_DRV_EVENT_INIT, CHR_PLAT_DRV_ERROR_PCIE_DRV_REG_FAIL);
#endif
        return ret;
    }

    oal_io_print("%s PCIe driver register succ\n", g_pcie_drv.name);
    return OAL_SUCC;
}

void oal_pcie_110x_exit(void)
{
#ifdef CONFIG_ARCH_KIRIN_PCIE
    kirin_pcie_power_notifiy_register(g_kirin_rc_idx, NULL, NULL, NULL);
#endif
    oal_pci_unregister_driver(&g_pcie_drv);
}
#endif

int32_t oal_pcie_110x_working_check(void)
{
    hcc_bus_dev *pst_bus_dev;
    pst_bus_dev = hcc_get_bus_dev(HCC_CHIP_110X_DEV);

    if (pst_bus_dev == NULL) {
        return OAL_FALSE;
    }

    if (pst_bus_dev->bus_cap & HCC_BUS_PCIE_CAP) {
        return OAL_TRUE;
    } else {
        return OAL_FALSE;
    }
}

int32_t oal_wifi_platform_load_pcie(void)
{
    int32_t ret = OAL_SUCC;

    if (oal_pcie_110x_working_check() != OAL_TRUE) {
        /* pcie driver don't support */
        oal_io_print("pcie driver don't support\n");
        return OAL_SUCC;
    }

    /* WiFi оƬ�ϵ� + PCIE ö�� */
#ifdef _PRE_PLAT_FEATURE_HI110X_PCIE
    ret = oal_pcie_110x_init();
#endif
#if defined(CONFIG_PCIE_KIRIN_SLT_HI110X) && defined(CONFIG_HISI_DEBUG_FS)
    if (ret == OAL_SUCC) {
        hi1103_pcie_chip_rc_slt_register();
    }
#endif
    return ret;
}

void oal_wifi_platform_unload_pcie(void)
{
    if (oal_pcie_110x_working_check() != OAL_TRUE) {
        return;
    }

#if defined(CONFIG_PCIE_KIRIN_SLT_HI110X) && defined(CONFIG_HISI_DEBUG_FS)
    hi1103_pcie_chip_rc_slt_unregister();
#endif
}

#ifdef _PRE_PLAT_FEATURE_HI110X_PCIE
oal_pcie_linux_res *oal_get_pcie_linux_res(void)
{
    return pcie_linux_res;
}
#else
oal_pcie_linux_res *oal_get_pcie_linux_res(void)
{
    return NULL;
}
#endif
oal_module_symbol(oal_get_pcie_linux_res);
