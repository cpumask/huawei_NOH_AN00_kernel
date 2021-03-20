

#ifndef __OAL_PCIE_LINUX_H__
#define __OAL_PCIE_LINUX_H__

#include "oal_util.h"
#include "oal_pcie_host.h"
#ifdef CONFIG_ARCH_KIRIN_PCIE

#ifndef _PRE_HI375X_PCIE
#include <linux/hisi/pcie-kirin-api.h>
#endif

#endif

#define OAL_PCIE_MIN_MPS 128

#define OAL_CONTINUOUS_LINK_UP_SUCC_TIMES 3

typedef enum _PCIE_EP_IP_STATUS_ {
    PCIE_EP_IP_POWER_DOWN = 0,
    PCIE_EP_IP_POWER_UP
} PCIE_EP_IP_STATUS;

#define  PCIE_HISI_VENDOR_ID         (0x19e5)
#define  PCIE_HISI_DEVICE_ID_HI1103  (0x1103)
#define  PCIE_HISI_DEVICE_ID_HI1105  (0x1105)
#define  PCIE_HISI_DEVICE_ID_HI1106  (0x1106)


#define PCIE_DUAL_PCI_MASTER_FLAG  0x11060000
#define PCIE_DUAL_PCI_SLAVE_FLAG   0x11060001

typedef struct _oal_pcie_linux_res__ {
    uint32_t version;  /* pcie version */
    uint32_t revision; /* 1:4.7a  , 2:5.00a */
    uint32_t irq_stat; /* 0:enabled, 1:disabled */
    uint32_t l1_err_cnt;
    oal_spin_lock_stru st_lock;
    oal_pci_dev_stru *pst_pcie_dev; /* Linux PCIe device hander */
#ifdef _PRE_PLAT_FEATURE_HI110X_DUAL_PCIE
    oal_pci_dev_stru *pst_pcie_dev_dual; /* dual pcie, pcie1 */
#endif
    hcc_bus *pst_bus;
    oal_pcie_res *pst_pci_res;
    oal_pcie_msi_stru st_msi;
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    struct pci_saved_state *default_state;
    struct pci_saved_state *state;
#endif
#ifdef CONFIG_ARCH_KIRIN_PCIE

#ifndef _PRE_HI375X_PCIE
    struct kirin_pcie_register_event pcie_event;
#endif

#endif
#ifdef _PRE_PLAT_FEATURE_HI110X_DUAL_PCIE
    struct pci_saved_state *default_state_dual;
    struct pci_saved_state *state_dual;
#ifdef CONFIG_ARCH_KIRIN_PCIE
    struct kirin_pcie_register_event pcie_event_dual;
#endif
#endif
    oal_tasklet_stru st_rx_task;

    PCIE_EP_IP_STATUS power_status;
    oal_atomic st_pcie_wakeup_flag;

    oal_completion st_pcie_ready;
    oal_completion st_pcie_shutdown_response;

    oal_wakelock_stru st_sr_bugfix; /* 暂时规避PCIE S/R失败的问题 */
} oal_pcie_linux_res;

typedef struct _pcie_wlan_callback_group_ {
    pcie_callback_stru pcie_mac_2g_isr_callback;
    pcie_callback_stru pcie_mac_5g_isr_callback;
    pcie_callback_stru pcie_phy_2g_isr_callback;
    pcie_callback_stru pcie_phy_5g_isr_callback;
} pcie_wlan_callback_group_stru;

typedef struct _oal_pcie_bindcpu_cfg_ {
    uint8_t is_bind;    /* 自动绑核指令,需保证存放在最低位 */
    uint8_t irq_cpu;    /* 用户绑定硬中断命令 */
    uint8_t thread_cmd; /* 用户绑定线程命令 */
    oal_bool_enum_uint8 is_userctl; /* 是否根据用户命令绑核 */
}oal_pcie_bindcpu_cfg;

#ifdef _PRE_PLAT_FEATURE_HI110X_DUAL_PCIE
extern int32_t g_dual_pci_support;
#endif

#ifdef _PRE_PLAT_FEATURE_HI110X_PCIE
int32_t pcie_wlan_func_register(pcie_wlan_callback_group_stru *p_func);
#else
OAL_STATIC OAL_INLINE int32_t pcie_wlan_func_register(pcie_wlan_callback_group_stru *p_func)
{
    return OAL_SUCC;
}
#endif

int32_t oal_wifi_platform_load_pcie(void);
void oal_wifi_platform_unload_pcie(void);

int32_t oal_pcie_set_loglevel(int32_t loglevel);
int32_t oal_pcie_set_hi11xx_loglevel(int32_t loglevel);
int32_t oal_pcie_firmware_read(oal_pcie_linux_res *pst_pcie_lres, uint8_t *buff, int32_t len, uint32_t timeout);
int32_t oal_pcie_firmware_write(oal_pcie_linux_res *pst_pcie_lres, uint8_t *buff, int32_t len);
int32_t oal_disable_pcie_irq(oal_pcie_linux_res *pst_pci_lres);
int32_t oal_pcie_ip_factory_test(hcc_bus *pst_bus, int32_t test_count);
int32_t oal_pcie_ip_voltage_bias_init(hcc_bus *pst_bus);
int32_t oal_pcie_rc_slt_chip_transfer(hcc_bus *pst_bus, void *ddr_address,
                                      uint32_t data_size, int32_t direction);
int32_t oal_pcie_ip_init(hcc_bus *pst_bus);
int32_t oal_pcie_110x_working_check(void);

#ifdef _PRE_PRODUCT_HI1620S_KUNPENG
#undef CONFIG_ARCH_KIRIN_PCIE
#ifdef _PRE_CONFIG_ARCH_HI1620S_KUNPENG_PCIE
extern int pci_power_notify_register(struct pci_dev *pdev, int (*poweron)(void* data),
                                     int (*poweroff)(void* data), void* data);
extern int pci_rp_power_ctrl(struct pci_dev *pdev, int power_option);
extern int pci_dev_re_enumerate(struct pci_dev *pdev);
#endif
#endif

#ifdef _PRE_HI375X_PCIE
extern int hipcie_enumerate(u32 rc_idx);
extern u32 show_link_state(u32 rc_id);
extern int hipcie_pm_control(int resume_flag, u32 rc_idx);
extern int hipcie_lp_ctrl(u32 rc_idx, u32 enable);
extern int hipcie_power_notifiy_register(u32 rc_idx, int (*poweron)(void* data),
                                         int (*poweroff)(void* data), void* data);
extern int32_t g_kirin_rc_idx;
#define kirin_pcie_enumerate                hipcie_enumerate
#define kirin_pcie_pm_control               hipcie_pm_control
#define kirin_pcie_lp_ctrl                  hipcie_lp_ctrl
#define kirin_pcie_power_notifiy_register   hipcie_power_notifiy_register

#else
#ifdef CONFIG_ARCH_KIRIN_PCIE
extern int32_t g_kirin_rc_idx;
/* hisi kirin PCIe functions */
extern int kirin_pcie_enumerate(u32 rc_idx);
extern u32 show_link_state(u32 rc_id);
extern int kirin_pcie_pm_control(int resume_flag, u32 rc_idx);
extern int kirin_pcie_lp_ctrl(u32 rc_idx, u32 enable);
/* notify WiFi when RC PCIE power on/off */
extern int kirin_pcie_power_notifiy_register(u32 rc_idx, int (*poweron)(void *data),
                                             int (*poweroff)(void *data), void *data);
#endif
#endif

extern oal_pcie_linux_res *oal_get_pcie_linux_res(void);
#endif

