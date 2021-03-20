

#include "oal_ext_if.h"
#include "oal_kernel_file.h"
#include "oal_cfg80211.h"

#include "oam_ext_if.h"
#include "frw_ext_if.h"
#if (_PRE_FRW_FEATURE_PROCCESS_ENTITY_TYPE == _PRE_FRW_FEATURE_PROCCESS_ENTITY_THREAD)
#ifndef CONFIG_HI110X_KERNEL_MODULES_BUILD_SUPPORT
#include "frw_task.h"
#endif
#endif

#include "wlan_spec.h"
#include "wlan_types.h"
#include "wlan_chip_i.h"
#include "mac_vap.h"
#include "mac_resource.h"
#include "mac_regdomain.h"
#include "mac_ie.h"
#include "mac_device.h"

#include "hmac_ext_if.h"
#include "hmac_chan_mgmt.h"
#include "hmac_tx_data.h"
#include "hmac_config.h"
#include "wal_main.h"
#include "wal_ext_if.h"
#include "wal_config.h"
#include "wal_regdb.h"
#include "wal_linux_scan.h"
#include "wal_linux_ioctl.h"
#include "wal_linux_bridge.h"
#include "wal_linux_flowctl.h"
#include "wal_linux_atcmdsrv.h"
#include "wal_linux_event.h"
#include "hmac_resource.h"
#include "hmac_p2p.h"

#include "wal_linux_cfg80211.h"
#include "wal_linux_cfgvendor.h"
#include "wal_dfx.h"

#include "oal_hcc_host_if.h"
#include "plat_cali.h"

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include <linux/notifier.h>
#include <linux/inetdevice.h>
#include <net/addrconf.h>
#endif
#include "hmac_arp_offload.h"

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 10, 59)) && (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include "../fs/proc/internal.h"
#endif

#include "hmac_auto_adjust_freq.h"
#ifdef _PRE_WLAN_FEATURE_MULTI_NETBUF_AMSDU
#include "hmac_tx_amsdu.h"
#endif

#include "hmac_roam_main.h"
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
#include "oal_sdio_comm.h"
#include "oal_net.h"
#include "hisi_customize_wifi.h"
#ifdef _PRE_WLAN_COUNTRYCODE_SELFSTUDY
#include "hisi_customize_wifi_hi110x.h"
#endif
#endif /* #ifdef _PRE_PLAT_FEATURE_CUSTOMIZE */

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include "plat_pm_wlan.h"
#include "plat_firmware.h"
#endif

#ifdef _PRE_WLAN_FEATURE_DFS
#include "hmac_dfs.h"
#endif

#include "wal_linux_ioctl.h"
#ifdef _PRE_WLAN_FEATURE_11AX
#include "hmac_wifi6_self_cure.h"
#endif

#ifdef _PRE_PRODUCT_HI3751V811
/* 熄屏状态下，仅响应永久组内成员的probe request帧，该功能需要包含注册屏幕状态相关钩子函数的头文件 */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 9, 0)) && (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include "drv_pmoc_ext.h"
#include "hi_module.h"
#endif
#endif

#include "wlan_mib.h"
#ifdef _PRE_WLAN_FEATURE_HIEX
#include "mac_hiex.h"
#include "hmac_hiex.h"
#endif
#include "hmac_tx_data.h"

#ifdef _PRE_WLAN_FEATURE_FTM
#include "mac_ftm.h"
#endif
#include "wal_cfg_ioctl.h"
#undef THIS_FILE_ID
#define THIS_FILE_ID      OAM_FILE_ID_WAL_LINUX_IOCTL_C

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include "board.h"
#endif
#if defined(WIN32)
#include "hisi_customize_wifi_hi110x.h"
#endif
#include "securec.h"
#include "securectype.h"

#ifdef _PRE_WLAN_RR_PERFORMANCE
#include "plat_debug.h"
#include "plat_firmware.h"
#include "hmac_auto_adjust_freq.h"
#include "wlan_mib.h"
#include "hmac_config.h"
#include "oal_hcc_bus.h"
#endif /* _PRE_WLAN_RR_PERFORMANCE */
#include "wal_linux_vendor.h"
#include "wal_linux_netdev_ops.h"
#include "wal_linux_customize.h"

extern uint32_t wal_hipriv_fem_lowpower(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
#ifdef CONFIG_HISI_CMDLINE_PARSE
extern unsigned int get_boot_into_recovery_flag(void);
#endif

/* 2 结构体定义 */

/* 3 全局变量定义 */
mac_rssi_cfg_table_stru g_ast_mac_rssi_config_table[] = {
    { "show_info", MAC_RSSI_LIMIT_SHOW_INFO },
    { "enable",    MAC_RSSI_LIMIT_ENABLE },
    { "delta",     MAC_RSSI_LIMIT_DELTA },
    { "threshold", MAC_RSSI_LIMIT_THRESHOLD }
};

#ifdef _PRE_WLAN_FEATURE_TCP_ACK_BUFFER
mac_tcp_ack_buf_cfg_table_stru g_ast_hmac_tcp_ack_buf_cfg_table[] = {
    { "enable",  MAC_TCP_ACK_BUF_ENABLE },
    { "timeout", MAC_TCP_ACK_BUF_TIMEOUT },
    { "count",   MAC_TCP_ACK_BUF_MAX },
};
#endif

OAL_STATIC oal_proc_dir_entry_stru *g_pst_proc_entry = OAL_PTR_NULL;


#if (_PRE_OS_VERSION_WIN32 == _PRE_OS_VERSION)
/* Just For UT */
OAL_STATIC uint32_t g_wal_wid_queue_init_flag = OAL_FALSE;
#endif
OAL_STATIC wal_msg_queue g_wal_wid_msg_queue;

wal_ap_config_stru g_st_ap_config_info = { 0 }; /* AP配置信息,需要在vap 创建后下发的 */

/* hi1102-cb add sys for 51/02 */
#ifdef _PRE_CONFIG_CONN_HISI_SYSFS_SUPPORT
struct kobject *gp_sys_kobject;
#endif

oal_completion g_wlan_cali_completed;
int8_t g_wait_wlan_power_on_cali = OAL_TRUE;
uint8_t g_go_cac = OAL_TRUE;

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
int32_t wal_hipriv_inetaddr_notifier_call(struct notifier_block *this, unsigned long event, void *ptr);

OAL_STATIC struct notifier_block wal_hipriv_notifier = {
    .notifier_call = wal_hipriv_inetaddr_notifier_call
};

int32_t wal_hipriv_inet6addr_notifier_call(struct notifier_block *this, unsigned long event, void *ptr);

OAL_STATIC struct notifier_block wal_hipriv_notifier_ipv6 = {
    .notifier_call = wal_hipriv_inet6addr_notifier_call
};
#endif

#ifdef _PRE_WLAN_FEATURE_DFR
extern hmac_dfr_info_stru g_st_dfr_info;
#endif  /* _PRE_WLAN_FEATURE_DFR */

#ifdef _PRE_WLAN_CFGID_DEBUG
extern OAL_CONST wal_hipriv_cmd_entry_stru g_ast_hipriv_cmd_debug[];
#endif
/* 静态函数声明 */
OAL_STATIC uint32_t wal_hipriv_vap_log_level(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
OAL_STATIC uint32_t wal_hipriv_getcountry(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
OAL_STATIC uint32_t wal_hipriv_set_bw(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
OAL_STATIC uint32_t wal_hipriv_always_tx(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
OAL_STATIC uint32_t wal_hipriv_always_tx_num(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
OAL_STATIC uint32_t wal_hipriv_always_tx_aggr_num(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
OAL_STATIC uint32_t wal_hipriv_set_ru_index(oal_net_device_stru *net_dev, int8_t *param);
OAL_STATIC uint32_t wal_hipriv_always_rx(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
#ifdef _PRE_WLAN_FEATURE_MONITOR
OAL_STATIC uint32_t wal_hipriv_set_sniffer(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
OAL_STATIC uint32_t wal_hipriv_enable_monitor_mode(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
#endif
OAL_STATIC uint32_t wal_hipriv_user_info(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
OAL_STATIC uint32_t wal_hipriv_add_vap(oal_net_device_stru *pst_cfg_net_dev, int8_t *pc_param);
OAL_STATIC uint32_t wal_hipriv_set_random_mac_addr_scan(oal_net_device_stru *pst_net_dev,
    int8_t *pc_param);
OAL_STATIC uint32_t wal_hipriv_add_user(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
OAL_STATIC uint32_t wal_hipriv_del_user(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
#ifdef PLATFORM_DEBUG_ENABLE
OAL_STATIC uint32_t wal_hipriv_reg_info(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
#endif
OAL_STATIC uint32_t wal_hipriv_sdio_flowctrl(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
#ifdef _PRE_WLAN_DELAY_STATISTIC
OAL_STATIC uint32_t wal_hipriv_pkt_time_switch(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
#endif
OAL_STATIC uint32_t wal_hipriv_set_2040_coext_support(oal_net_device_stru *pst_net_dev,
    int8_t *pc_param);
OAL_STATIC uint32_t wal_hipriv_rx_fcs_info(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
OAL_STATIC uint32_t wal_hipriv_set_mode(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
OAL_STATIC uint32_t wal_hipriv_set_freq(oal_net_device_stru *pst_net_dev, int8_t *pc_param);

#ifdef _PRE_WLAN_FEATURE_TCP_ACK_BUFFER
OAL_STATIC uint32_t wal_hipriv_tcp_ack_buf_cfg(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
#endif

int32_t wal_ioctl_get_blkwhtlst(oal_net_device_stru *pst_net_dev, oal_net_dev_ioctl_data_stru *pst_ioctl_data);
OAL_STATIC int wal_ioctl_get_essid(oal_net_device_stru *pst_net_dev,
                                   oal_iw_request_info_stru *pst_info,
                                   oal_iwreq_data_union *pst_wrqu,
                                   char *pc_param);
OAL_STATIC int wal_ioctl_get_apaddr(oal_net_device_stru *pst_net_dev,
                                    oal_iw_request_info_stru *pst_info,
                                    oal_iwreq_data_union *pst_wrqu,
                                    char *pc_extra);
OAL_STATIC int wal_ioctl_get_iwname(oal_net_device_stru *pst_net_dev,
                                    oal_iw_request_info_stru *pst_info,
                                    oal_iwreq_data_union *pst_wrqu,
                                    char *pc_extra);
#ifdef PLATFORM_DEBUG_ENABLE
OAL_STATIC uint32_t wal_hipriv_reg_write(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
#endif
OAL_STATIC uint32_t wal_hipriv_tpc_log(oal_net_device_stru *pst_net_dev, int8_t *pc_param);

OAL_STATIC uint32_t wal_hipriv_set_uapsd_cap(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
OAL_STATIC uint32_t wal_hipriv_sta_ps_mode(oal_net_device_stru *pst_cfg_net_dev, int8_t *pc_param);
OAL_STATIC uint32_t wal_hipriv_sta_ps_info(oal_net_device_stru *pst_cfg_net_dev, int8_t *pc_param);
OAL_STATIC uint32_t wal_hipriv_set_fasts_sleep_para(oal_net_device_stru *pst_cfg_net_dev,
    int8_t *pc_param);

OAL_STATIC uint32_t wal_hipriv_set_uapsd_para(oal_net_device_stru *pst_cfg_net_dev, int8_t *pc_param);
#ifdef _PRE_CONFIG_CONN_HISI_SYSFS_SUPPORT
/* hi1102-cb add sys for 51/02 */
OAL_STATIC oal_ssize_t wal_hipriv_sys_write(struct kobject *dev, struct kobj_attribute *attr,
    const char *buf, oal_size_t count);
OAL_STATIC oal_ssize_t wal_hipriv_sys_read(struct kobject *dev, struct kobj_attribute *attr, char *buf);
OAL_STATIC struct kobj_attribute dev_attr_hipriv =
    __ATTR(hipriv, (OAL_S_IRUGO | OAL_S_IWUSR), wal_hipriv_sys_read, wal_hipriv_sys_write);
OAL_STATIC struct attribute *hipriv_sysfs_entries[] = {
    &dev_attr_hipriv.attr,
    NULL
};
OAL_STATIC struct attribute_group hipriv_attribute_group = {
    .attrs = hipriv_sysfs_entries,
};
/* hi1102-cb add sys for 51/02 */
#endif

#ifdef _PRE_WLAN_FEATURE_M2S_MSS
OAL_STATIC uint32_t wal_ioctl_set_m2s_blacklist(oal_net_device_stru *pst_net_dev,
    uint8_t *puc_buf, uint32_t ul_buf_len, uint8_t uc_m2s_blacklist_cnt);
uint32_t wal_ioctl_set_m2s_mss(oal_net_device_stru *pst_net_dev, uint8_t uc_m2s_mode);
#endif

#ifdef _PRE_WLAN_FEATURE_HS20
OAL_STATIC int32_t wal_ioctl_set_qos_map(oal_net_device_stru *pst_net_dev,
                                           hmac_cfg_qos_map_param_stru *pst_qos_map_param);
#endif /* #ifdef _PRE_WLAN_FEATURE_HS20 */

OAL_STATIC int32_t wal_set_ap_max_user(oal_net_device_stru *pst_net_dev, uint32_t ul_ap_max_user);
OAL_STATIC void wal_config_mac_filter(oal_net_device_stru *pst_net_dev, int8_t *pc_command);
OAL_STATIC int32_t wal_kick_sta(oal_net_device_stru *pst_net_dev,
    uint8_t *auc_mac_addr, uint8_t uc_mac_addr_len, uint16_t us_reason_code);
OAL_STATIC int wal_ioctl_set_ap_config(oal_net_device_stru *pst_net_dev,
    oal_iw_request_info_stru *pst_info, oal_iwreq_data_union *pst_wrqu, char *pc_extra);
OAL_STATIC int wal_ioctl_set_mac_filters(oal_net_device_stru *pst_net_dev,
    oal_iw_request_info_stru *pst_info, oal_iwreq_data_union *pst_wrqu, char *pc_extra);
OAL_STATIC int wal_ioctl_get_assoc_list(oal_net_device_stru *pst_net_dev,
    oal_iw_request_info_stru *pst_info, oal_iwreq_data_union *pst_wrqu, char *pc_extra);
OAL_STATIC int wal_ioctl_set_ap_sta_disassoc(oal_net_device_stru *pst_net_dev,
    oal_iw_request_info_stru *pst_info, oal_iwreq_data_union *pst_wrqu, char *pc_extra);
OAL_STATIC uint32_t wal_get_parameter_from_cmd(int8_t *pc_cmd,
    int8_t *pc_arg, OAL_CONST int8_t *puc_token, uint32_t *pul_cmd_offset, uint32_t ul_param_max_len);

int32_t wal_init_wlan_vap(oal_net_device_stru *pst_net_dev);
int32_t wal_deinit_wlan_vap(oal_net_device_stru *pst_net_dev);
int32_t wal_start_vap(oal_net_device_stru *pst_net_dev);
int32_t wal_stop_vap(oal_net_device_stru *pst_net_dev);
OAL_STATIC int32_t wal_set_mac_addr(oal_net_device_stru *pst_net_dev);
int32_t wal_init_wlan_netdev(oal_wiphy_stru *pst_wiphy, const char *dev_name);
int32_t wal_setup_ap(oal_net_device_stru *pst_net_dev);

#ifdef _PRE_WLAN_FEATURE_11D
int32_t wal_regdomain_update_for_dfs(oal_net_device_stru *pst_net_dev, int8_t *pc_country);
int32_t wal_regdomain_update(oal_net_device_stru *pst_net_dev, int8_t *pc_country);
#endif


#ifdef _PRE_WLAN_FEATURE_11K
OAL_STATIC uint32_t wal_hipriv_send_neighbor_req(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
OAL_STATIC uint32_t wal_hipriv_beacon_req_table_switch(oal_net_device_stru *pst_net_dev,
    int8_t *pc_param);
#endif
OAL_STATIC uint32_t wal_hipriv_voe_enable(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
#ifdef _PRE_WLAN_FEATURE_APF
OAL_STATIC uint32_t wal_hipriv_apf_filter_list(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
#endif
OAL_STATIC uint32_t wal_hipriv_remove_app_ie(oal_net_device_stru *pst_net_dev, int8_t *pc_param);

#ifdef _PRE_WLAN_RR_PERFORMANCE
OAL_STATIC int32_t wal_ext_priv_cmd_dev_freq_check_para(oal_net_device_stru *pst_net_dev);
OAL_STATIC int32_t wal_priv_cmd_dev_freq_para(mac_vap_stru *mac_vap, mac_cfg_set_tlv_stru *pst_config_para,
                                                    uint8_t uc_cmd_type, uint8_t ul_value);
OAL_STATIC int32_t wal_ext_priv_cmd_set_dev_freq(oal_net_device_stru *pst_net_dev, uint8_t uc_flag);
OAL_STATIC int32_t wal_ext_priv_cmd_pcie_aspm(int32_t aspm);
#endif /* _PRE_WLAN_RR_PERFORMANCE */
#ifdef _PRE_WLAN_FEATURE_HID2D
OAL_STATIC uint32_t wal_hipriv_set_hid2d_acs_mode(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
#endif
extern uint8_t wal_cfg80211_convert_value_to_vht_width(int32_t l_channel_value);
extern uint32_t wal_hipriv_set_rfch(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
extern uint32_t wal_hipriv_set_rxch(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
extern uint32_t wal_hipriv_set_nss(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
extern uint32_t wal_hipriv_send_cw_signal(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
extern uint32_t wal_hipriv_adjust_ppm(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
extern uint32_t wal_hipriv_chip_check(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
extern uint32_t wal_hipriv_set_txpower(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
OAL_STATIC uint32_t wal_hipriv_set_p2p_scenes(oal_net_device_stru *p_net_dev, int8_t *p_param);
OAL_STATIC uint32_t wal_hipriv_cali_debug(oal_net_device_stru *pst_net_dev, int8_t *pc_param);


int8_t *g_pac_mib_operation_cmd_name[] = {
    "smps_vap_mode",
    "info",
    "2040_coexistence",
    "freq",
    "mode",
    "set_mcast_data",
    "set_edca_switch_sta",
    "beacon_offload_test",
    "auto_ba",
    "reassoc_req",
    "coex_preempt_type",
    "protocol_debug",
    "start_scan",
    "start_join",
    "kick_user",
    "ampdu_tx_on",
    "amsdu_tx_on",
    "ampdu_amsdu",
    "frag_threshold",
    "wmm_switch",
    "hide_ssid",
    "set_stbc_cap",
    "set_ldpc_cap",
    "set_psm_para",
    "set_sta_pm_on",
    "enable_pmf",
    "send_pspoll",
    "send_nulldata",
    "set_default_key",
    "add_key",
    "auto_protection",
    "send_2040_coext",
    "set_opmode_notify",
    "m2u_igmp_pkt_xmit",
    "smps_info",
    "vap_classify_tid",
    "essid",
    "bintval",
    "ampdu_mmss",
    "set_tx_classify_switch",
    "custom_info",
    "11v_cfg_wl_mgmt",
    "11v_cfg_bsst",
    "send_radio_meas_req",
    "11k_switch"
};

OAL_CONST wal_hipriv_cmd_entry_stru  g_ast_hipriv_cmd[] = {
    /* 商用对外发布的私有命令 */
    /* 打印vap的所有参数信息: hipriv "vap0 info" */
    { "info",                 wal_hipriv_vap_info },
    /* 设置国家码命令 hipriv "Hisilicon0 setcountry param "param取值为大写的国家码字，例如 CN US */
    { "setcountry",           wal_hipriv_setcountry },
    /* 查询国家码命令 hipriv "Hisilicon0 getcountry" */
    { "getcountry",           wal_hipriv_getcountry },
    /* ip filter(功能调试接口)hipriv "wlan0 ip_filter cmd param0 param1 ...."
       举例:启动功能 "wlan0 ip_filter set_rx_filter_enable 1/0"
       清空黑名单 "wlan0 ip_filter clear_rx_filters"
       设置黑名单 "wlan0 ip_filter add_rx_filter_items 条目个数(0/1/2...)
       名单内容(protocol0 port0 protocol1 port1...)",目前该调试接口仅支持20对条目
     */
    { "ip_filter",            wal_hipriv_set_ip_filter },
    /* 打印指定mac地址user的所有参数信息: hipriv "vap0 userinfo XX XX XX XX XX XX(16进制oal_strtohex)" */
    { "userinfo",             wal_hipriv_user_info },
#ifdef PLATFORM_DEBUG_ENABLE
    /* 打印寄存器信息: hipriv "Hisilicon0 reginfo 16|32 regtype(soc/mac/phy) startaddr endaddr" */
    { "reginfo",              wal_hipriv_reg_info },
    /* 写寄存器: hipriv "Hisilicon0 regwrite 32/16(51没有16位写寄存器功能) regtype(soc/mac/phy) addr val"
     * addr val必须都是16进制0x开头
     */
    { "regwrite",             wal_hipriv_reg_write },
#endif
    /* 设置随机mac addr扫描开关，sh hipriv.sh "Hisilicon0 random_mac_addr_scan 0|1(打开|关闭)" */
    { "random_mac_addr_scan", wal_hipriv_set_random_mac_addr_scan },
    /* 设置20/40共存使能: hipriv "vap0 2040_coexistence 0|1" 0表示20/40MHz共存使能，1表示20/40MHz共存不使能 */
    { "2040_coexistence",     wal_hipriv_set_2040_coext_support },

    { "set_ps_mode",          wal_hipriv_sta_ps_mode }, /* 设置PSPOLL能力 sh hipriv.sh 'wlan0 set_ps_mode 3' */
    { "psm_info_debug",       wal_hipriv_sta_ps_info }, /* sta psm的统计信息 sh hipriv.sh 'wlan0 psm_info_debug 1' */
    { "set_fast_sleep_para",  wal_hipriv_set_fasts_sleep_para },
    { "uapsd_en_cap",         wal_hipriv_set_uapsd_cap }, /* hipriv "vap0 uapsd_en_cap 0\1" */
    /* 设置uapsd的参数信息 sh hipriv.sh 'wlan0 set_uapsd_para 3 1 1 1 1 */
    { "set_uapsd_para",       wal_hipriv_set_uapsd_para },
    { "create",               wal_hipriv_add_vap }, /* 创建vap私有命令为: hipriv "Hisilicon0 create vap0 ap|sta" */
    /* 设置常发模式:hipriv "vap0 al_tx <value: 0/1/2>  <len>" 由于mac限制，
       11a,b,g下只支持4095以下数据发送,可以使用set_mcast_data对速率进行设置 */
    { "al_tx",                wal_hipriv_always_tx },
    { "al_tx_1102",           wal_hipriv_always_tx },        /* al_tx_02命令先保留 */
    { "al_tx_num",            wal_hipriv_always_tx_num },    /* 设置常发数目:       hipriv "vap0 al_tx_num <value>" */
    { "al_tx_aggr_num",       wal_hipriv_always_tx_aggr_num },
    { "al_ru_index",          wal_hipriv_set_ru_index }, /* 设置ru index: hipriv "vap0 al_ru_index 106tone 0 1" */
    { "al_rx",                wal_hipriv_always_rx },  /* 设置常收模式: hipriv "vap0 al_rx <value: 0/1/2>" */
#ifdef _PRE_WLAN_FEATURE_MONITOR
    { "sniffer",              wal_hipriv_set_sniffer }, /* 设置抓包模式: sh hipriv.sh "wlan0 sniffer <value: 0/1>" */
    { "monitor",              wal_hipriv_enable_monitor_mode }, /* sh hipriv.sh "wlan0 monitor <value: 0/1>" */
#endif
    { "rate",  wal_hipriv_set_rate },    /* 设置non-HT模式下的速率: hipriv "vap0 rate  <value>" */
    { "mcs",   wal_hipriv_set_mcs },     /* 设置HT模式下的速率: hipriv "vap0 mcs   <value>" */
    { "mcsac", wal_hipriv_set_mcsac },   /* 设置VHT模式下的速率: hipriv "vap0 mcsac <value>" */
#ifdef _PRE_WLAN_FEATURE_11AX
    { "mcsax", wal_hipriv_set_mcsax },       /* 设置HE模式下的速率:  hipriv "vap0 mcsax <value>" */
#ifdef  _PRE_WLAN_FEATURE_11AX_ER_SU_DCM
    { "mcsax_er", _wal_hipriv_set_mcsax_er }, /* 设置HE ER模式下的速率:      hipriv "vap0 mcsax_er <value>" */
#endif
#endif
    { "p2p_scenes",        wal_hipriv_set_p2p_scenes},
    { "freq",              wal_hipriv_set_freq },   /* 设置AP 信道 */
    { "mode",              wal_hipriv_set_mode },   /* 设置AP 协议模式 */
    { "bw",                wal_hipriv_set_bw },     /* 设置带宽: hipriv "vap0 bw    <value>" */
    /* 打印描述符信息: hipriv "vap0 set_mcast_data <param name> <value>" */
    { "set_mcast_data",    wal_hipriv_set_mcast_data_dscr_param },
    /* 打印接收帧的FCS正确与错误信息:hipriv "vap0 rx_fcs_info 0/1 1/2/3/4" 0/1  0代表不清除，1代表清除 */
    { "rx_fcs_info",       wal_hipriv_rx_fcs_info },
    /* 设置添加用户的配置命令: hipriv "vap0 add_user xx xx xx xx xx xx(mac地址) 0|1(HT能力位) " 该命令针对某一个VAP */
    { "add_user",          wal_hipriv_add_user },
    /* 设置删除用户的配置命令: hipriv "vap0 del_user xx xx xx xx xx xx(mac地址)" 该命令针对某一个VAP */
    { "del_user",          wal_hipriv_del_user },
    { "alg_cfg",           wal_hipriv_alg_cfg }, /* 算法参数配置: hipriv "vap0 alg_cfg sch_vi_limit 10" */
    { "alg_tpc_log", wal_hipriv_tpc_log }, /* tpc算法日志参数配置: */
    { "sdio_flowctrl", wal_hipriv_sdio_flowctrl },
#ifdef _PRE_WLAN_DELAY_STATISTIC
    /* 报文时间戳调试开关: hipriv "Hisilicon0 pkt_time_switch on |off */
    { "pkt_time_switch", wal_hipriv_pkt_time_switch },
#endif
#ifdef _PRE_WLAN_FEATURE_11K
    { "send_neighbor_req", wal_hipriv_send_neighbor_req },  /* sh hipriv.sh "wlan0 send_neighbor_req WiFi1" */
    /* sh hipriv.sh "wlan0 beacon_req_table_switch 0/1" */
    { "beacon_req_table_switch", wal_hipriv_beacon_req_table_switch },
#endif
    /* VOE功能使能控制，默认关闭 sh hipriv.sh "wlan0 voe_enable 0/1"
    (Bit0:11r  Bit1:11V Bit2:11K Bit3:是否强制包含IE70(voe 认证需要),
     Bit4:11r认证,B5-B6:11k auth operating class  Bit7:读取信息) */
    { "voe_enable", wal_hipriv_voe_enable },
    /* VAP级别日志级别 hipriv "VAPX log_level {1|2}"  Warning与Error级别日志以VAP为维度 */
    { "log_level", wal_hipriv_vap_log_level },

#ifdef _PRE_WLAN_FEATURE_TCP_ACK_BUFFER
    { "tcp_ack_buf", wal_hipriv_tcp_ack_buf_cfg }, /* 快速老化 */
#endif

#ifdef _PRE_WLAN_FEATURE_NEGTIVE_DET
    /* pkmode功能的门限调整接口 hipriv "wlan0 pk_mode_debug 0/1(high/low) 0/1/2/3/4(BW) 0/1/2/3(protocol) 吞吐门限" */
    { "pk_mode_debug", wal_hipriv_pk_mode_debug },
#endif
#ifdef _PRE_WLAN_FEATURE_APF
    { "apf_filter_list", wal_hipriv_apf_filter_list },
#endif
    /* 通过eid移除用户态下发的某个IE hipriv "wlan0 remove_app_ie 0/1 eid" 0恢复该ie,1屏蔽该ie */
    { "remove_app_ie", wal_hipriv_remove_app_ie },
#ifdef _PRE_WLAN_FEATURE_HID2D
    { "set_hid2d_acs_mode", wal_hipriv_set_hid2d_acs_mode },
#ifdef _PRE_WLAN_FEATURE_HID2D_PRESENTATION
    { "switch_chan", wal_hipriv_hid2d_switch_channel },
    { "scan_chan", wal_hipriv_hid2d_scan_channel },
#endif
#endif
    { "cali",   wal_hipriv_cali_debug },
};

/* net_device上挂接的net_device_ops函数 */
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
oal_net_device_ops_stru g_st_wal_net_dev_ops = {
    .ndo_get_stats = wal_netdev_get_stats,
    .ndo_open = wal_netdev_open_ext,
    .ndo_stop = wal_netdev_stop,
    .ndo_start_xmit = wal_bridge_vap_xmit,

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 10, 44))
    /* Temp blank */
#else
    .ndo_set_multicast_list = OAL_PTR_NULL,
#endif

    .ndo_do_ioctl = wal_net_device_ioctl,
    .ndo_change_mtu = oal_net_device_change_mtu,
    .ndo_init = oal_net_device_init,

    .ndo_select_queue = wal_netdev_select_queue,

    .ndo_set_mac_address = wal_netdev_set_mac_addr
};

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 10, 44))
/* 不支持ethtool_ops 相关函数操作,需要显示定义空的结构体，否则将采用内核默认的ethtool_ops会导致空指针异常 */
oal_ethtool_ops_stru g_st_wal_ethtool_ops = { 0 };
#endif

#ifdef _PRE_WLAN_FEATURE_NAN
/* xmit回调必须注册，NAN注册一个空函数。其他接口不需要注册。 */
OAL_STATIC oal_net_dev_tx_enum wal_nan_xmit(oal_netbuf_stru *buf, oal_net_device_stru *dev)
{
    oal_netbuf_free(buf);
    return NETDEV_TX_OK;
}

OAL_STATIC oal_net_device_ops_stru g_nan_net_dev_ops = {
    .ndo_start_xmit = wal_nan_xmit,
};
#endif

#elif (_PRE_OS_VERSION_WIN32 == _PRE_OS_VERSION)
oal_net_device_ops_stru g_st_wal_net_dev_ops = {
    oal_net_device_init,
    wal_netdev_open_ext,
    wal_netdev_stop,
    wal_bridge_vap_xmit,
    OAL_PTR_NULL,
    wal_netdev_get_stats,
    wal_net_device_ioctl,
    oal_net_device_change_mtu,
    wal_netdev_set_mac_addr
};
oal_ethtool_ops_stru g_st_wal_ethtool_ops = { 0 };
#endif

/* 标准ioctl命令函数表 */
OAL_STATIC OAL_CONST oal_iw_handler g_ast_iw_handlers[] = {
    OAL_PTR_NULL,                         /* SIOCSIWCOMMIT, */
    (oal_iw_handler)wal_ioctl_get_iwname, /* SIOCGIWNAME, */
    OAL_PTR_NULL,                         /* SIOCSIWNWID, */
    OAL_PTR_NULL,                         /* SIOCGIWNWID, */
    OAL_PTR_NULL,                         /* SIOCSIWFREQ, 设置频点信道 */
    OAL_PTR_NULL,                         /* SIOCGIWFREQ, 获取频点信道 */
    OAL_PTR_NULL,                         /* SIOCSIWMODE, 设置bss type */
    OAL_PTR_NULL,                         /* SIOCGIWMODE, 获取bss type */
    OAL_PTR_NULL,                         /* SIOCSIWSENS, */
    OAL_PTR_NULL,                         /* SIOCGIWSENS, */
    OAL_PTR_NULL, /* SIOCSIWRANGE, */     /* not used */
    OAL_PTR_NULL,                         /* SIOCGIWRANGE, */
    OAL_PTR_NULL, /* SIOCSIWPRIV, */      /* not used */
    OAL_PTR_NULL, /* SIOCGIWPRIV, */      /* kernel code */
    OAL_PTR_NULL, /* SIOCSIWSTATS, */     /* not used */
    OAL_PTR_NULL,                         /* SIOCGIWSTATS, */
    OAL_PTR_NULL,                         /* SIOCSIWSPY, */
    OAL_PTR_NULL,                         /* SIOCGIWSPY, */
    OAL_PTR_NULL,                         /* -- hole -- */
    OAL_PTR_NULL,                         /* -- hole -- */
    OAL_PTR_NULL,                         /* SIOCSIWAP, */
    (oal_iw_handler)wal_ioctl_get_apaddr, /* SIOCGIWAP, */
    OAL_PTR_NULL,                         /* SIOCSIWMLME, */
    OAL_PTR_NULL,                         /* SIOCGIWAPLIST, */
    OAL_PTR_NULL,                         /* SIOCSIWSCAN, */
    OAL_PTR_NULL,                         /* SIOCGIWSCAN, */
    OAL_PTR_NULL,                         /* SIOCSIWESSID, 设置ssid */
    (oal_iw_handler)wal_ioctl_get_essid,  /* SIOCGIWESSID, 获取ssid */
    OAL_PTR_NULL,                         /* SIOCSIWNICKN */
    OAL_PTR_NULL,                         /* SIOCGIWNICKN */
    OAL_PTR_NULL,                         /* -- hole -- */
    OAL_PTR_NULL,                         /* -- hole -- */
    OAL_PTR_NULL,                         /* SIOCSIWRATE */
    OAL_PTR_NULL,                         /* SIOCGIWRATE  get_iwrate */
    OAL_PTR_NULL,                         /* SIOCSIWRTS */
    OAL_PTR_NULL,                         /* SIOCGIWRTS  get_rtsthres */
    OAL_PTR_NULL,                         /* SIOCSIWFRAG */
    OAL_PTR_NULL,                         /* SIOCGIWFRAG  get_fragthres */
    OAL_PTR_NULL,                         /* SIOCSIWTXPOW, 设置传输功率限制 */
    OAL_PTR_NULL,                         /* SIOCGIWTXPOW, 设置传输功率限制 */
    OAL_PTR_NULL,                         /* SIOCSIWRETRY */
    OAL_PTR_NULL,                         /* SIOCGIWRETRY */
    OAL_PTR_NULL,                         /* SIOCSIWENCODE */
    OAL_PTR_NULL,                         /* SIOCGIWENCODE  get_iwencode */
    OAL_PTR_NULL,                         /* SIOCSIWPOWER */
    OAL_PTR_NULL,                         /* SIOCGIWPOWER */
    OAL_PTR_NULL,                         /* -- hole -- */
    OAL_PTR_NULL,                         /* -- hole -- */
    OAL_PTR_NULL,                         /* SIOCSIWGENIE */
    OAL_PTR_NULL,                         /* SIOCGIWGENIE */
    OAL_PTR_NULL,                         /* SIOCSIWAUTH */
    OAL_PTR_NULL,                         /* SIOCGIWAUTH */
    OAL_PTR_NULL,                         /* SIOCSIWENCODEEXT */
    OAL_PTR_NULL                          /* SIOCGIWENCODEEXT */
};

/* 私有ioctl命令参数定义 */
OAL_STATIC OAL_CONST oal_iw_priv_args_stru g_ast_iw_priv_args[] = {
    { WAL_IOCTL_PRIV_SET_AP_CFG,  OAL_IW_PRIV_TYPE_CHAR | OAL_IW_PRIV_TYPE256, 0, "AP_SET_CFG" },
    { WAL_IOCTL_PRIV_AP_MAC_FLTR, OAL_IW_PRIV_TYPE_CHAR | OAL_IW_PRIV_TYPE256,
      OAL_IW_PRIV_TYPE_CHAR | OAL_IW_PRIV_SIZE_FIXED | 0,         "AP_SET_MAC_FLTR" },
    { WAL_IOCTL_PRIV_AP_GET_STA_LIST, 0, OAL_IW_PRIV_TYPE_CHAR | OAL_IW_PRIV_TYPE1024, "AP_GET_STA_LIST" },
    { WAL_IOCTL_PRIV_AP_STA_DISASSOC, OAL_IW_PRIV_TYPE_CHAR | OAL_IW_PRIV_TYPE256,
      OAL_IW_PRIV_TYPE_CHAR | 0, "AP_STA_DISASSOC" },

    { WLAN_CFGID_ADD_BLACK_LIST, OAL_IW_PRIV_TYPE_CHAR | OAL_IW_PRIV_TYPE40,        0,    "blkwhtlst_add" },
    { WLAN_CFGID_DEL_BLACK_LIST, OAL_IW_PRIV_TYPE_CHAR | OAL_IW_PRIV_TYPE40,        0,    "blkwhtlst_del" },
    { WLAN_CFGID_CLR_BLACK_LIST, OAL_IW_PRIV_TYPE_CHAR | OAL_IW_PRIV_TYPE40,        0,    "blkwhtlst_clr" },
    /* 配置黑名单模式 */
    { WLAN_CFGID_BLACKLIST_MODE, OAL_IW_PRIV_TYPE_INT | OAL_IW_PRIV_SIZE_FIXED | 1, 0,    "blkwhtlst_mode" },
    /* 黑名单模式打印 */
    { WLAN_CFGID_BLACKLIST_MODE, 0, OAL_IW_PRIV_TYPE_INT | OAL_IW_PRIV_SIZE_FIXED | 1,    "get_blkwhtmode" },

    { WLAN_CFGID_GET_2040BSS_SW, 0, OAL_IW_PRIV_TYPE_INT | OAL_IW_PRIV_SIZE_FIXED | 1,    "get_obss_sw" },
    { WLAN_CFGID_2040BSS_ENABLE, OAL_IW_PRIV_TYPE_INT | OAL_IW_PRIV_SIZE_FIXED | 1, 0,    "set_obss_sw" },
};

/* 私有ioctl命令函数表 */
OAL_STATIC OAL_CONST oal_iw_handler g_ast_iw_priv_handlers[] = {
    OAL_PTR_NULL, /* SIOCWFIRSTPRIV+0 */                                   /* sub-ioctl set 入口 */
    OAL_PTR_NULL, /* SIOCWFIRSTPRIV+1 */                                   /* sub-ioctl get 入口 */
    OAL_PTR_NULL, /* SIOCWFIRSTPRIV+2 */                                   /* setkey */
    OAL_PTR_NULL, /* SIOCWFIRSTPRIV+3 */                                   /* setwmmparams */
    OAL_PTR_NULL, /* SIOCWFIRSTPRIV+4 */                                   /* delkey */
    OAL_PTR_NULL, /* SIOCWFIRSTPRIV+5 */                                   /* getwmmparams */
    OAL_PTR_NULL, /* SIOCWFIRSTPRIV+6 */                                   /* setmlme */
    OAL_PTR_NULL, /* SIOCWFIRSTPRIV+7 */                                   /* getchaninfo */
    OAL_PTR_NULL, /* SIOCWFIRSTPRIV+8 */                                   /* setcountry */
    OAL_PTR_NULL, /* SIOCWFIRSTPRIV+9 */                                   /* getcountry */
    OAL_PTR_NULL, /* SIOCWFIRSTPRIV+10 */                                  /* addmac */
    OAL_PTR_NULL, /* SIOCWFIRSTPRIV+11 */                                  /* getscanresults */
    OAL_PTR_NULL, /* SIOCWFIRSTPRIV+12 */                                  /* delmac */
    OAL_PTR_NULL, /* SIOCWFIRSTPRIV+13 */                                  /* getchanlist */
    OAL_PTR_NULL, /* SIOCWFIRSTPRIV+14 */                                  /* setchanlist */
    OAL_PTR_NULL, /* SIOCWFIRSTPRIV+15 */                                  /* setmac */
    OAL_PTR_NULL, /* SIOCWFIRSTPRIV+16 */                                  /* chanswitch */
    OAL_PTR_NULL, /* SIOCWFIRSTPRIV+17 */                                  /* 获取模式, 例: iwpriv vapN get_mode */
    OAL_PTR_NULL, /* SIOCWFIRSTPRIV+18 */                                  /* 设置模式, 例: iwpriv vapN mode 11g */
    OAL_PTR_NULL, /* SIOCWFIRSTPRIV+19 */                                  /* getappiebuf */
    OAL_PTR_NULL, /* SIOCWFIRSTPRIV+20 */                                  /* null */
    (oal_iw_handler)wal_ioctl_get_assoc_list, /* SIOCWFIRSTPRIV+21 */      /* APUT取得关联STA列表 */
    (oal_iw_handler)wal_ioctl_set_mac_filters, /* SIOCWFIRSTPRIV+22 */     /* APUT设置STA过滤 */
    (oal_iw_handler)wal_ioctl_set_ap_config, /* SIOCWFIRSTPRIV+23 */       /* 设置APUT参数 */
    (oal_iw_handler)wal_ioctl_set_ap_sta_disassoc, /* SIOCWFIRSTPRIV+24 */ /* APUT去关联STA */
    OAL_PTR_NULL, /* SIOCWFIRSTPRIV+25 */                                  /* getStatistics */
    OAL_PTR_NULL, /* SIOCWFIRSTPRIV+26 */                                  /* sendmgmt */
    OAL_PTR_NULL, /* SIOCWFIRSTPRIV+27 */                                  /* null */
    OAL_PTR_NULL, /* SIOCWFIRSTPRIV+28 */                                  /* null */
    OAL_PTR_NULL, /* SIOCWFIRSTPRIV+29 */                                  /* null */
    OAL_PTR_NULL, /* SIOCWFIRSTPRIV+30 */                                  /* sethbrparams */

    OAL_PTR_NULL,
    /* SIOCWFIRSTPRIV+31 */ /* setrxtimeout */
};

/* 无线配置iw_handler_def定义 */
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
oal_iw_handler_def_stru g_st_iw_handler_def = {
    .standard = g_ast_iw_handlers,
    .num_standard = oal_array_size(g_ast_iw_handlers),
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 10, 59))
#ifdef CONFIG_WEXT_PRIV
    .private = g_ast_iw_priv_handlers,
    .num_private = oal_array_size(g_ast_iw_priv_handlers),
    .private_args = g_ast_iw_priv_args,
    .num_private_args = oal_array_size(g_ast_iw_priv_args),
#endif
#else
    .private = g_ast_iw_priv_handlers,
    .num_private = oal_array_size(g_ast_iw_priv_handlers),
    .private_args = g_ast_iw_priv_args,
    .num_private_args = oal_array_size(g_ast_iw_priv_args),
#endif
    .get_wireless_stats = OAL_PTR_NULL
};

#elif (_PRE_OS_VERSION_WIN32 == _PRE_OS_VERSION)
oal_iw_handler_def_stru g_st_iw_handler_def = {
    OAL_PTR_NULL, /* 标准ioctl handler */
    0,
    oal_array_size(g_ast_iw_priv_handlers),
    { 0, 0 }, /* 字节对齐 */
    oal_array_size(g_ast_iw_priv_args),
    g_ast_iw_priv_handlers, /* 私有ioctl handler */
    g_ast_iw_priv_args,
    OAL_PTR_NULL
};
#endif

/* 协议模式字符串定义 */
OAL_CONST wal_ioctl_mode_map_stru g_ast_mode_map[] = {
    /* legacy */
    { "11a",  WLAN_LEGACY_11A_MODE,    WLAN_BAND_5G, WLAN_BAND_WIDTH_20M },
    { "11b",  WLAN_LEGACY_11B_MODE,    WLAN_BAND_2G, WLAN_BAND_WIDTH_20M },
    { "11bg", WLAN_MIXED_ONE_11G_MODE, WLAN_BAND_2G, WLAN_BAND_WIDTH_20M },
    { "11g",  WLAN_MIXED_TWO_11G_MODE, WLAN_BAND_2G, WLAN_BAND_WIDTH_20M },

    /* 11n */
    { "11na20",      WLAN_HT_MODE, WLAN_BAND_5G, WLAN_BAND_WIDTH_20M },
    { "11ng20",      WLAN_HT_MODE, WLAN_BAND_2G, WLAN_BAND_WIDTH_20M },
    { "11na40plus",  WLAN_HT_MODE, WLAN_BAND_5G, WLAN_BAND_WIDTH_40PLUS },
    { "11na40minus", WLAN_HT_MODE, WLAN_BAND_5G, WLAN_BAND_WIDTH_40MINUS },
    { "11ng40plus",  WLAN_HT_MODE, WLAN_BAND_2G, WLAN_BAND_WIDTH_40PLUS },
    { "11ng40minus", WLAN_HT_MODE, WLAN_BAND_2G, WLAN_BAND_WIDTH_40MINUS },

    /* 11ac */
    { "11ac20",           WLAN_VHT_MODE, WLAN_BAND_5G, WLAN_BAND_WIDTH_20M },
    { "11ac40plus",       WLAN_VHT_MODE, WLAN_BAND_5G, WLAN_BAND_WIDTH_40PLUS },
    { "11ac40minus",      WLAN_VHT_MODE, WLAN_BAND_5G, WLAN_BAND_WIDTH_40MINUS },
    { "11ac80plusplus",   WLAN_VHT_MODE, WLAN_BAND_5G, WLAN_BAND_WIDTH_80PLUSPLUS },
    { "11ac80plusminus",  WLAN_VHT_MODE, WLAN_BAND_5G, WLAN_BAND_WIDTH_80PLUSMINUS },
    { "11ac80minusplus",  WLAN_VHT_MODE, WLAN_BAND_5G, WLAN_BAND_WIDTH_80MINUSPLUS },
    { "11ac80minusminus", WLAN_VHT_MODE, WLAN_BAND_5G, WLAN_BAND_WIDTH_80MINUSMINUS },
#ifdef _PRE_WLAN_FEATURE_160M
    { "11ac160plusplusplus",    WLAN_VHT_MODE, WLAN_BAND_5G, WLAN_BAND_WIDTH_160PLUSPLUSPLUS },
    { "11ac160plusplusminus",   WLAN_VHT_MODE, WLAN_BAND_5G, WLAN_BAND_WIDTH_160PLUSPLUSMINUS },
    { "11ac160plusminusplus",   WLAN_VHT_MODE, WLAN_BAND_5G, WLAN_BAND_WIDTH_160PLUSMINUSPLUS },
    { "11ac160plusminusminus",  WLAN_VHT_MODE, WLAN_BAND_5G, WLAN_BAND_WIDTH_160PLUSMINUSMINUS },
    { "11ac160minusplusplus",   WLAN_VHT_MODE, WLAN_BAND_5G, WLAN_BAND_WIDTH_160MINUSPLUSPLUS },
    { "11ac160minusplusminus",  WLAN_VHT_MODE, WLAN_BAND_5G, WLAN_BAND_WIDTH_160MINUSPLUSMINUS },
    { "11ac160minusminusplus",  WLAN_VHT_MODE, WLAN_BAND_5G, WLAN_BAND_WIDTH_160MINUSMINUSPLUS },
    { "11ac160minusminusminus", WLAN_VHT_MODE, WLAN_BAND_5G, WLAN_BAND_WIDTH_160MINUSMINUSMINUS },
#endif

    { "11ac2g20",      WLAN_VHT_MODE, WLAN_BAND_2G, WLAN_BAND_WIDTH_20M },
    { "11ac2g40plus",  WLAN_VHT_MODE, WLAN_BAND_2G, WLAN_BAND_WIDTH_40PLUS },
    { "11ac2g40minus", WLAN_VHT_MODE, WLAN_BAND_2G, WLAN_BAND_WIDTH_40MINUS },
    /* 11n only and 11ac only, 都是20M带宽 */
    { "11nonly2g", WLAN_HT_ONLY_MODE,  WLAN_BAND_2G, WLAN_BAND_WIDTH_20M },
    { "11nonly5g", WLAN_HT_ONLY_MODE,  WLAN_BAND_5G, WLAN_BAND_WIDTH_20M },
    { "11aconly",  WLAN_VHT_ONLY_MODE, WLAN_BAND_5G, WLAN_BAND_WIDTH_20M },

    /* 1151产测以及ONT网页设置协议模式时不会携带带宽扩展方向信息 */
    { "11ng40",   WLAN_HT_MODE,  WLAN_BAND_2G, WLAN_BAND_WIDTH_40M },
    { "11ac2g40", WLAN_VHT_MODE, WLAN_BAND_2G, WLAN_BAND_WIDTH_40M },
    { "11na40",   WLAN_HT_MODE,  WLAN_BAND_5G, WLAN_BAND_WIDTH_40M },
    { "11ac40",   WLAN_VHT_MODE, WLAN_BAND_5G, WLAN_BAND_WIDTH_40M },
    { "11ac80",   WLAN_VHT_MODE, WLAN_BAND_5G, WLAN_BAND_WIDTH_80M },

#ifdef _PRE_WLAN_FEATURE_11AX
    /* 11ax */
    { "11ax2g20",           WLAN_HE_MODE, WLAN_BAND_2G, WLAN_BAND_WIDTH_20M },
    { "11ax2g40plus",       WLAN_HE_MODE, WLAN_BAND_2G, WLAN_BAND_WIDTH_40PLUS },
    { "11ax2g40minus",      WLAN_HE_MODE, WLAN_BAND_2G, WLAN_BAND_WIDTH_40MINUS },
    { "11ax5g20",           WLAN_HE_MODE, WLAN_BAND_5G, WLAN_BAND_WIDTH_20M },
    { "11ax5g40plus",       WLAN_HE_MODE, WLAN_BAND_5G, WLAN_BAND_WIDTH_40PLUS },
    { "11ax5g40minus",      WLAN_HE_MODE, WLAN_BAND_5G, WLAN_BAND_WIDTH_40MINUS },
    { "11ax5g80plusplus",   WLAN_HE_MODE, WLAN_BAND_5G, WLAN_BAND_WIDTH_80PLUSPLUS },
    { "11ax5g80plusminus",  WLAN_HE_MODE, WLAN_BAND_5G, WLAN_BAND_WIDTH_80PLUSMINUS },
    { "11ax5g80minusplus",  WLAN_HE_MODE, WLAN_BAND_5G, WLAN_BAND_WIDTH_80MINUSPLUS },
    { "11ax5g80minusminus", WLAN_HE_MODE, WLAN_BAND_5G, WLAN_BAND_WIDTH_80MINUSMINUS },

#ifdef _PRE_WLAN_FEATURE_160M
    { "11ax5g160plusplusplus",    WLAN_HE_MODE, WLAN_BAND_5G, WLAN_BAND_WIDTH_160PLUSPLUSPLUS },
    { "11ax5g160plusplusminus",   WLAN_HE_MODE, WLAN_BAND_5G, WLAN_BAND_WIDTH_160PLUSPLUSMINUS },
    { "11ax5g160plusminusplus",   WLAN_HE_MODE, WLAN_BAND_5G, WLAN_BAND_WIDTH_160PLUSMINUSPLUS },
    { "11ax5g160plusminusminus",  WLAN_HE_MODE, WLAN_BAND_5G, WLAN_BAND_WIDTH_160PLUSMINUSMINUS },
    { "11ax5g160minusplusplus",   WLAN_HE_MODE, WLAN_BAND_5G, WLAN_BAND_WIDTH_160MINUSPLUSPLUS },
    { "11ax5g160minusplusminus",  WLAN_HE_MODE, WLAN_BAND_5G, WLAN_BAND_WIDTH_160MINUSPLUSMINUS },
    { "11ax5g160minusminusplus",  WLAN_HE_MODE, WLAN_BAND_5G, WLAN_BAND_WIDTH_160MINUSMINUSPLUS },
    { "11ax5g160minusminusminus", WLAN_HE_MODE, WLAN_BAND_5G, WLAN_BAND_WIDTH_160MINUSMINUSMINUS },
#endif
#endif

    { OAL_PTR_NULL }
};

/* 注意! 这里的参数定义需要与 g_dmac_config_set_dscr_param中的函数顺序严格一致! */
OAL_CONST int8_t *pauc_tx_dscr_param_name[WAL_DSCR_PARAM_BUTT] = {
    "pgl",
    "mtpgl",
    "ta",
    "ra",
    "cc",
    "power",
    "shortgi",
    "preamble",
    "rtscts",
    "lsigtxop",
    "smooth",
    "snding",
    "txbf",
    "stbc",
    "rd_ess",
    "dyn_bw",
    "dyn_bw_exist",
    "ch_bw_exist",
    "rate",
    "mcs",
    "mcsac",
    "mcsax",
    "mcsax_er",
    "nss",
    "bw",
    "ltf",
    "gi",
    "txchain",
    "dcm",
    "protocol_mode"
};

OAL_CONST int8_t *pauc_tx_pow_param_name[WAL_TX_POW_PARAM_BUTT] = {
    "rf_reg_ctl",
    "fix_level",
    "mag_level",
    "ctl_level",
    "amend",
    "no_margin",
    "show_log",
    "sar_level",
#ifdef _PRE_WLAN_FEATURE_TAS_ANT_SWITCH
    "tas_pwr_ctrl",
    "tas_rssi_measure",
    "tas_ant_switch",
#endif
    "show_tpc_tbl_gain",
    "pow_save",
#ifdef _PRE_WLAN_FEATURE_FULL_QUAN_PROD_CAL
    "pdinfo",
#endif
    "tpc_idx",
};

OAL_CONST int8_t *g_ru_size_tbl[WLAN_HE_RU_SIZE_BUTT] = {
    "26tone",
    "52tone",
    "106tone",
    "242tone",
    "484tone",
    "996tone",
    "1992tone"
};

OAL_STATIC OAL_CONST int8_t *pauc_tx_dscr_nss_tbl[WLAN_NSS_LIMIT] = {
    "1",
    "2",
    "3",
    "4"
};

OAL_CONST int8_t *pauc_bw_tbl[WLAN_BANDWITH_CAP_BUTT] = {
    "20",
    "40",
    "d40",
    "80",
    "d80",
    "160",
    "d160",
    "80_80",
#ifdef  _PRE_WLAN_FEATURE_11AX_ER_SU_DCM
    "242tone",
    "106tone",
#endif
};

OAL_STATIC OAL_CONST int8_t *pauc_non_ht_rate_tbl[WLAN_LEGACY_RATE_VALUE_BUTT] = {
    "1",
    "2",
    "5.5",
    "11",
    "rsv0",
    "rsv1",
    "rsv2",
    "rsv3",
    "48",
    "24",
    "12",
    "6",
    "54",
    "36",
    "18",
    "9"
};

OAL_CONST wal_ioctl_dyn_cali_stru g_ast_dyn_cali_cfg_map[] = {
    { "realtime_cali_adjust", MAC_DYN_CALI_CFG_SET_EN_REALTIME_CALI_ADJUST },
    { "2g_dscr_interval",     MAC_DYN_CALI_CFG_SET_2G_DSCR_INT },
    { "5g_dscr_interval",     MAC_DYN_CALI_CFG_SET_5G_DSCR_INT },
    { "chain_interval",       MAC_DYN_CALI_CFG_SET_CHAIN_INT },
    { "pdet_min_th",          MAC_DYN_CALI_CFG_SET_PDET_MIN_TH },
    { "pdet_max_th",          MAC_DYN_CALI_CFG_SET_PDET_MAX_TH },
    { OAL_PTR_NULL }
};

OAL_CONST wal_ioctl_alg_cfg_stru g_ast_alg_cfg_map[] = {
    { "sch_vi_ctrl_ena",    MAC_ALG_CFG_SCHEDULE_VI_CTRL_ENA },
    { "sch_bebk_minbw_ena", MAC_ALG_CFG_SCHEDULE_BEBK_MIN_BW_ENA },
    { "sch_mvap_sch_ena",   MAC_ALG_CFG_SCHEDULE_MVAP_SCH_ENA },
    { "sch_vi_sch_ms",      MAC_ALG_CFG_SCHEDULE_VI_SCH_LIMIT },
    { "sch_vo_sch_ms",      MAC_ALG_CFG_SCHEDULE_VO_SCH_LIMIT },
    { "sch_vi_drop_ms",     MAC_ALG_CFG_SCHEDULE_VI_DROP_LIMIT },
    { "sch_vi_ctrl_ms",     MAC_ALG_CFG_SCHEDULE_VI_CTRL_MS },
    { "sch_vi_life_ms",     MAC_ALG_CFG_SCHEDULE_VI_MSDU_LIFE_MS },
    { "sch_vo_life_ms",     MAC_ALG_CFG_SCHEDULE_VO_MSDU_LIFE_MS },
    { "sch_be_life_ms",     MAC_ALG_CFG_SCHEDULE_BE_MSDU_LIFE_MS },
    { "sch_bk_life_ms",     MAC_ALG_CFG_SCHEDULE_BK_MSDU_LIFE_MS },
    { "sch_vi_low_delay",   MAC_ALG_CFG_SCHEDULE_VI_LOW_DELAY_MS },
    { "sch_vi_high_delay",  MAC_ALG_CFG_SCHEDULE_VI_HIGH_DELAY_MS },
    { "sch_cycle_ms",       MAC_ALG_CFG_SCHEDULE_SCH_CYCLE_MS },
    { "sch_ctrl_cycle_ms",  MAC_ALG_CFG_SCHEDULE_TRAFFIC_CTRL_CYCLE },
    { "sch_cir_nvip_kbps",  MAC_ALG_CFG_SCHEDULE_CIR_NVIP_KBPS },
    { "sch_cir_nvip_be",    MAC_ALG_CFG_SCHEDULE_CIR_NVIP_KBPS_BE },
    { "sch_cir_nvip_bk",    MAC_ALG_CFG_SCHEDULE_CIR_NVIP_KBPS_BK },
    { "sch_cir_vip_kbps",   MAC_ALG_CFG_SCHEDULE_CIR_VIP_KBPS },
    { "sch_cir_vip_be",     MAC_ALG_CFG_SCHEDULE_CIR_VIP_KBPS_BE },
    { "sch_cir_vip_bk",     MAC_ALG_CFG_SCHEDULE_CIR_VIP_KBPS_BK },
    { "sch_cir_vap_kbps",   MAC_ALG_CFG_SCHEDULE_CIR_VAP_KBPS },
    { "sch_sm_delay_ms",    MAC_ALG_CFG_SCHEDULE_SM_TRAIN_DELAY },
    { "sch_drop_pkt_limit", MAC_ALG_CFG_VIDEO_DROP_PKT_LIMIT },
    { "sch_flowctl_ena",    MAC_ALG_CFG_FLOWCTRL_ENABLE_FLAG },
    { "sch_log_start",      MAC_ALG_CFG_SCHEDULE_LOG_START },
    { "sch_log_end",        MAC_ALG_CFG_SCHEDULE_LOG_END },
    { "sch_method",         MAC_ALG_CFG_SCHEDULE_SCH_METHOD },
    { "sch_fix_mode",       MAC_ALG_CFG_SCHEDULE_FIX_SCH_MODE },
    { "sch_vap_prio",       MAC_ALG_CFG_SCHEDULE_VAP_SCH_PRIO },

    { "txbf_switch",      MAC_ALG_CFG_TXBF_MASTER_SWITCH },
    { "txbf_txmode_enb",  MAC_ALG_CFG_TXBF_TXMODE_ENABLE },
    { "txbf_11nbfee_enb", MAC_ALG_CFG_TXBF_11N_BFEE_ENABLE },
    { "txbf_2g_bfer",     MAC_ALG_CFG_TXBF_2G_BFER_ENABLE },
    { "txbf_2nss_bfer",   MAC_ALG_CFG_TXBF_2NSS_BFER_ENABLE },
    { "txbf_fix_mode",    MAC_ALG_CFG_TXBF_FIX_MODE },
    { "txbf_fix_sound",   MAC_ALG_CFG_TXBF_FIX_SOUNDING },
    { "txbf_probe_int",   MAC_ALG_CFG_TXBF_PROBE_INT },
    { "txbf_rm_worst",    MAC_ALG_CFG_TXBF_REMOVE_WORST },
    { "txbf_stbl_num",    MAC_ALG_CFG_TXBF_STABLE_NUM },
    { "txbf_probe_cnt",   MAC_ALG_CFG_TXBF_PROBE_COUNT },

    /* 开启或关闭速率自适应算法: sh hipriv.sh "vap0 alg_cfg ar_enable [1|0]" */
    { "ar_enable",            MAC_ALG_CFG_AUTORATE_ENABLE },
    /* 开启或关闭使用最低速率: sh hipriv.sh "vap0 alg_cfg ar_use_lowest [1|0]" */
    { "ar_use_lowest",        MAC_ALG_CFG_AUTORATE_USE_LOWEST_RATE },
    /* 设置短期统计的包数目:sh hipriv.sh "vap0 alg_cfg ar_short_num [包数目]" */
    { "ar_short_num",         MAC_ALG_CFG_AUTORATE_SHORT_STAT_NUM },
    /* 设置短期统计的包位移值:sh hipriv.sh "vap0 alg_cfg ar_short_shift [位移值]" */
    { "ar_short_shift",       MAC_ALG_CFG_AUTORATE_SHORT_STAT_SHIFT },
    /* 设置长期统计的包数目:sh hipriv.sh "vap0 alg_cfg ar_long_num [包数目]" */
    { "ar_long_num",          MAC_ALG_CFG_AUTORATE_LONG_STAT_NUM },
    /* 设置长期统计的包位移值:sh hipriv.sh "vap0 alg_cfg ar_long_shift [位移值]" */
    { "ar_long_shift",        MAC_ALG_CFG_AUTORATE_LONG_STAT_SHIFT },
    /* 设置最小探测包间隔:sh hipriv.sh "vap0 alg_cfg ar_min_probe_no [包数目]" */
    { "ar_min_probe_up_no",   MAC_ALG_CFG_AUTORATE_MIN_PROBE_UP_INTVL_PKTNUM },
    /* 设置最小探测包间隔:sh hipriv.sh "vap0 alg_cfg ar_min_probe_no [包数目]" */
    { "ar_min_probe_down_no", MAC_ALG_CFG_AUTORATE_MIN_PROBE_DOWN_INTVL_PKTNUM },
    /* 设置最大探测包间隔:sh hipriv.sh "vap0 alg_cfg ar_max_probe_no [包数目]" */
    { "ar_max_probe_no",      MAC_ALG_CFG_AUTORATE_MAX_PROBE_INTVL_PKTNUM },
    /* 设置探测间隔保持次数:sh hipriv.sh "vap0 alg_cfg ar_keep_times [次数]" */
    { "ar_keep_times",        MAC_ALG_CFG_AUTORATE_PROBE_INTVL_KEEP_TIMES },
    /* 设置goodput突变门限(千分比，如300):sh hipriv.sh "vap0 alg_cfg ar_delta_ratio [千分比]" */
    { "ar_delta_ratio",       MAC_ALG_CFG_AUTORATE_DELTA_GOODPUT_RATIO },
    /* 设置vi的per门限(千分比，如300):sh hipriv.sh "vap0 alg_cfg ar_vi_per_limit [千分比]" */
    { "ar_vi_per_limit",      MAC_ALG_CFG_AUTORATE_VI_PROBE_PER_LIMIT },
    /* 设置vo的per门限(千分比，如300):sh hipriv.sh "vap0 alg_cfg ar_vo_per_limit [千分比]" */
    { "ar_vo_per_limit",      MAC_ALG_CFG_AUTORATE_VO_PROBE_PER_LIMIT },
    /* 设置ampdu的durattion值:sh hipriv.sh "vap0 alg_cfg ar_ampdu_time [时间值]" */
    { "ar_ampdu_time",        MAC_ALG_CFG_AUTORATE_AMPDU_DURATION },
    /* 设置mcs0的传输失败门限:sh hipriv.sh "vap0 alg_cfg ar_cont_loss_num [包数目]" */
    { "ar_cont_loss_num",     MAC_ALG_CFG_AUTORATE_MCS0_CONT_LOSS_NUM },
    /* 设置升回11b的rssi门限:sh hipriv.sh "vap0 alg_cfg ar_11b_diff_rssi [数值]" */
    { "ar_11b_diff_rssi",     MAC_ALG_CFG_AUTORATE_UP_PROTOCOL_DIFF_RSSI },
    /* 设置rts模式:sh hipriv.sh "vap0 alg_cfg ar_rts_mode
       [0(都不开)|1(都开)|2(rate[0]动态RTS, rate[1..3]都开RTS)|3(rate[0]不开RTS, rate[1..3]都开RTS)]" */
    { "ar_rts_mode",          MAC_ALG_CFG_AUTORATE_RTS_MODE },
    /* 设置Legacy首包错误率门限:sh hipriv.sh "vap0 alg_cfg ar_legacy_loss [数值]" */
    { "ar_legacy_loss",       MAC_ALG_CFG_AUTORATE_LEGACY_1ST_LOSS_RATIO_TH },
    /* 设置Legacy首包错误率门限:sh hipriv.sh "vap0 alg_cfg ar_ht_vht_loss [数值]" */
    { "ar_ht_vht_loss",       MAC_ALG_CFG_AUTORATE_HT_VHT_1ST_LOSS_RATIO_TH },
    /* 开始速率统计日志:sh hipriv.sh "vap0 alg_cfg ar_stat_log_do [mac地址] [业务类别] [包数目]"
       如: sh hipriv.sh "vap0 alg_cfg ar_stat_log_do 06:31:04:E3:81:02 1 1000" */
    { "ar_stat_log_do",       MAC_ALG_CFG_AUTORATE_STAT_LOG_START },
    /* 开始速率选择日志:sh hipriv.sh "vap0 alg_cfg ar_sel_log_do [mac地址] [业务类别] [包数目]"
       如: sh hipriv.sh "vap0 alg_cfg ar_sel_log_do 06:31:04:E3:81:02 1 200" */
    { "ar_sel_log_do",        MAC_ALG_CFG_AUTORATE_SELECTION_LOG_START },
    /* 开始固定速率日志:sh hipriv.sh "vap0 alg_cfg ar_fix_log_do [mac地址] [tidno] [per门限]"
       如: sh hipriv.sh "vap0 alg_cfg ar_sel_log_do 06:31:04:E3:81:02 1 200" */
    { "ar_fix_log_do",        MAC_ALG_CFG_AUTORATE_FIX_RATE_LOG_START },
    /* 开始聚合自适应日志:sh hipriv.sh "vap0 alg_cfg ar_fix_log_do [mac地址] [tidno]"
       如: sh hipriv.sh "vap0 alg_cfg ar_sel_log_do 06:31:04:E3:81:02 1 " */
    { "ar_aggr_log_do",       MAC_ALG_CFG_AUTORATE_AGGR_STAT_LOG_START },
    /* 打印速率统计日志:sh hipriv.sh "vap0 alg_cfg ar_st_log_out 06:31:04:E3:81:02" */
    { "ar_st_log_out",        MAC_ALG_CFG_AUTORATE_STAT_LOG_WRITE },
    /* 打印速率选择日志:sh hipriv.sh "vap0 alg_cfg ar_sel_log_out 06:31:04:E3:81:02" */
    { "ar_sel_log_out",       MAC_ALG_CFG_AUTORATE_SELECTION_LOG_WRITE },
    /* 打印固定速率日志:sh hipriv.sh "vap0 alg_cfg ar_fix_log_out 06:31:04:E3:81:02" */
    { "ar_fix_log_out",       MAC_ALG_CFG_AUTORATE_FIX_RATE_LOG_WRITE },
    /* 打印固定速率日志:sh hipriv.sh "vap0 alg_cfg ar_fix_log_out 06:31:04:E3:81:02" */
    { "ar_aggr_log_out",      MAC_ALG_CFG_AUTORATE_AGGR_STAT_LOG_WRITE },
    /* 打印速率集合:sh hipriv.sh "vap0 alg_cfg ar_disp_rateset 06:31:04:E3:81:02" */
    { "ar_disp_rateset",      MAC_ALG_CFG_AUTORATE_DISPLAY_RATE_SET },
    /* 配置固定速率:sh hipriv.sh "vap0 alg_cfg ar_cfg_fix_rate 06:31:04:E3:81:02 0" */
    { "ar_cfg_fix_rate",      MAC_ALG_CFG_AUTORATE_CONFIG_FIX_RATE },
    /* 打印接收速率集合:sh hipriv.sh "vap0 alg_cfg ar_disp_rx_rate 06:31:04:E3:81:02" */
    { "ar_disp_rx_rate",      MAC_ALG_CFG_AUTORATE_DISPLAY_RX_RATE },
    /* 开启或关闭速率自适应日志: sh hipriv.sh "vap0 alg_cfg ar_log_enable [1|0]" */
    { "ar_log_enable",        MAC_ALG_CFG_AUTORATE_LOG_ENABLE },
    /* 设置最大的VO速率: sh hipriv.sh "vap0 alg_cfg ar_max_vo_rate [速率值]" */
    { "ar_max_vo_rate",       MAC_ALG_CFG_AUTORATE_VO_RATE_LIMIT },
    /* 设置深衰弱的per门限值: sh hipriv.sh "vap0 alg_cfg ar_fading_per_th [per门限值(千分数)]" */
    { "ar_fading_per_th",     MAC_ALG_CFG_AUTORATE_JUDGE_FADING_PER_TH },
    /* 设置聚合自适应开关: sh hipriv.sh "vap0 alg_cfg ar_aggr_opt [1|0]" */
    { "ar_aggr_opt",          MAC_ALG_CFG_AUTORATE_AGGR_OPT },
    /* 设置聚合自适应探测间隔: sh hipriv.sh "vap0 alg_cfg ar_aggr_pb_intvl [探测间隔]" */
    { "ar_aggr_pb_intvl",     MAC_ALG_CFG_AUTORATE_AGGR_PROBE_INTVL_NUM },
    /* 设置聚合自适应统计移位值: sh hipriv.sh "vap0 alg_cfg ar_aggr_st_shift [统计移位值]" */
    { "ar_aggr_st_shift",     MAC_ALG_CFG_AUTORATE_AGGR_STAT_SHIFT },
    /* 设置DBAC模式下的最大聚合时间: sh hipriv.sh "vap0 alg_cfg ar_dbac_aggrtime [最大聚合时间(us)]" */
    { "ar_dbac_aggrtime",     MAC_ALG_CFG_AUTORATE_DBAC_AGGR_TIME },
    /* 设置调试用的VI状态: sh hipriv.sh "vap0 alg_cfg ar_dbg_vi_status [0/1/2]" */
    { "ar_dbg_vi_status",     MAC_ALG_CFG_AUTORATE_DBG_VI_STATUS },
    /* 聚合自适应log开关: sh hipriv.sh "vap0 alg_cfg ar_dbg_aggr_log [0/1]" */
    { "ar_dbg_aggr_log",      MAC_ALG_CFG_AUTORATE_DBG_AGGR_LOG },
    /* 最优速率变化时不进行聚合探测的报文数: sh hipriv.sh "vap0 alg_cfg ar_aggr_pck_num [报文数]" */
    { "ar_aggr_pck_num",      MAC_ALG_CFG_AUTORATE_AGGR_NON_PROBE_PCK_NUM },
    /* 最小聚合时间索引: sh hipriv.sh "vap0 alg_cfg ar_aggr_min_idx [索引值]" */
    { "ar_min_aggr_idx",      MAC_ALG_CFG_AUTORATE_AGGR_MIN_AGGR_TIME_IDX },
    /* 设置聚合250us向上的deltaPER门限: sh hipriv.sh "vap0 alg_cfg ar_250us_dper_th [门限值]" */
    { "ar_250us_dper_th",     MAC_ALG_CFG_AUTORATE_AGGR_250US_DELTA_PER_TH },
    /* 设置最大聚合数目: sh hipriv.sh "vap0 alg_cfg ar_max_aggr_num [聚合数目]" */
    { "ar_max_aggr_num",      MAC_ALG_CFG_AUTORATE_MAX_AGGR_NUM },
    /* 设置最低阶MCS限制聚合为1的PER门限: sh hipriv.sh "vap0 alg_cfg ar_1mpdu_per_th [per门限值(千分数)]" */
    { "ar_1mpdu_per_th",      MAC_ALG_CFG_AUTORATE_LIMIT_1MPDU_PER_TH },
    /* 开启或关闭共存探测机制: sh hipriv.sh "vap0 alg_cfg ar_btcoxe_probe [1|0]" */
    { "ar_btcoxe_probe", MAC_ALG_CFG_AUTORATE_BTCOEX_PROBE_ENABLE },
    /* 开启或关闭共存聚合机制: sh hipriv.sh "vap0 alg_cfg ar_btcoxe_aggr [1|0]" */
    { "ar_btcoxe_aggr",  MAC_ALG_CFG_AUTORATE_BTCOEX_AGGR_ENABLE },
    /* 设置共存统计时间间隔参数: sh hipriv.sh "vap0 alg_cfg ar_coxe_intvl [统计周期ms]" */
    { "ar_coxe_intvl",   MAC_ALG_CFG_AUTORATE_COEX_STAT_INTVL },
    /* 设置共存abort低比例门限参数: sh hipriv.sh "vap0 alg_cfg ar_coxe_low_th [千分数]" */
    { "ar_coxe_low_th",  MAC_ALG_CFG_AUTORATE_COEX_LOW_ABORT_TH },
    /* 设置共存abort高比例门限参数: sh hipriv.sh "vap0 alg_cfg ar_coxe_high_th [千分数]" */
    { "ar_coxe_high_th", MAC_ALG_CFG_AUTORATE_COEX_HIGH_ABORT_TH },
    /* 设置共存聚合数目为1的门限参数: sh hipriv.sh "vap0 alg_cfg ar_coxe_agrr_th [千分数]" */
    { "ar_coxe_agrr_th", MAC_ALG_CFG_AUTORATE_COEX_AGRR_NUM_ONE_TH },
    /* 动态带宽特性使能开关: sh hipriv.sh "vap0 alg_cfg ar_dyn_bw_en [0/1]" */
    { "ar_dyn_bw_en",          MAC_ALG_CFG_AUTORATE_DYNAMIC_BW_ENABLE },
    /* 吞吐量波动优化开关: sh hipriv.sh "vap0 alg_cfg ar_thpt_wave_opt [0/1]" */
    { "ar_thpt_wave_opt",      MAC_ALG_CFG_AUTORATE_THRPT_WAVE_OPT },
    /* 设置判断吞吐量波动的goodput差异比例门限(千分数):
       sh hipriv.sh "vap0 alg_cfg ar_gdpt_diff_th [goodput相差比例门限(千分数)]" */
    { "ar_gdpt_diff_th",       MAC_ALG_CFG_AUTORATE_GOODPUT_DIFF_TH },
    /* 设置判断吞吐量波动的PER变差的门限(千分数): sh hipriv.sh "vap0 alg_cfg ar_per_worse_th [PER变差门限(千分数)]" */
    { "ar_per_worse_th",       MAC_ALG_CFG_AUTORATE_PER_WORSE_TH },
    /* 设置发RTS收到CTS但发DATA都不回BA的发送完成中断次数门限: sh hipriv.sh "vap0 alg_cfg ar_cts_no_ba_num [次数]" */
    { "ar_cts_no_ack_num",     MAC_ALG_CFG_AUTORATE_RX_CTS_NO_BA_NUM },
    /* 设置是否支持voice业务聚合: sh hipriv.sh "vap0 alg_cfg ar_vo_aggr [0/1]" */
    { "ar_vo_aggr",            MAL_ALG_CFG_AUTORATE_VOICE_AGGR },
    /* 设置快速平滑统计的平滑因子偏移量: sh hipriv.sh "vap0 alg_cfg ar_fast_smth_shft [偏移量]"(255表示取消快速平滑) */
    { "ar_fast_smth_shft",     MAC_ALG_CFG_AUTORATE_FAST_SMOOTH_SHIFT },
    /* 设置快速平滑统计的最小聚合数目门限: sh hipriv.sh "vap0 alg_cfg ar_fast_smth_aggr_num [最小聚合数目]" */
    { "ar_fast_smth_aggr_num", MAC_ALG_CFG_AUTORATE_FAST_SMOOTH_AGGR_NUM },
    /* 设置short GI惩罚的PER门限值(千分数): sh hipriv.sh "vap0 alg_cfg ar_sgi_punish_per [PER门限值(千分数)]" */
    { "ar_sgi_punish_per",     MAC_ALG_CFG_AUTORATE_SGI_PUNISH_PER },
    /* 设置short GI惩罚的等待探测数目: sh hipriv.sh "vap0 alg_cfg ar_sgi_punish_num [等待探测数目]" */
    { "ar_sgi_punish_num",     MAC_ALG_CFG_AUTORATE_SGI_PUNISH_NUM },
#ifdef _PRE_WLAN_FEATURE_MWO_DET
    /* 设置第四速率等级，在微波炉周期的8ms 发射期用 */
    { "ar_fourth_rate",   MAL_ALG_CFG_AUTORATE_LAST_RATE_RANK_INDEX },
    /* 设置autorate内微波炉检测算法的维测信息的打印开关 */
    { "ar_mwo_det_debug", MAL_ALG_CFG_AUTORATE_MWO_DET_DEBUG },
    /* 设置微波炉检测状态下per统计的时长，默认统计时间为3000ms,单位ms */
    { "ar_mwo_per_log",   MAL_ALG_CFG_AUTORATE_MWO_DET_PER_LOG },
#endif
    /* 设置接收通道AGC优化使能开关: sh hipriv.sh "vap0 alg_cfg ar_rxch_agc_opt [1/0]" */
    { "ar_rxch_agc_opt",     MAC_ALG_CFG_AUTORATE_RXCH_AGC_OPT },
    /* 设置接收通道AGC优化日志开关: sh hipriv.sh "vap0 alg_cfg ar_rxch_agc_log [1/0]" */
    { "ar_rxch_agc_log",     MAC_ALG_CFG_AUTORATE_RXCH_AGC_LOG },
    /* 设置接收通道AGC优化的弱信号RSSI门限: sh hipriv.sh "vap0 alg_cfg ar_weak_rssi_th [RSSI绝对值]"
       (例如: RSSI为-90dBm, 则参数值为90) */
    { "ar_weak_rssi_th",     MAC_ALG_CFG_AUTORATE_WEAK_RSSI_TH },
    /* 设置接收通道的统计周期(间隔报文数): sh hipriv.sh "vap0 alg_cfg ar_weak_rssi_th [报文数目]" */
    { "ar_rxch_stat_period", MAC_ALG_CFG_AUTORATE_RXCH_STAT_PERIOD },
    /* 设置RTS针对单用户TCP优化的调试开关: sh hipriv.sh "vap0 alg_cfg ar_rts_one_tcp_dbg [1/0]" */
    { "ar_rts_one_tcp_dbg",  MAC_ALG_CFG_AUTORATE_RTS_ONE_TCP_DBG },
    /* 设置针对扫描用户的优化: sh hipriv.sh "vap0 alg_cfg ar_scan_user_opt [1/0]" */
    { "ar_scan_user_opt",    MAC_ALG_CFG_AUTORATE_SCAN_USER_OPT },
    /* 设置每个速率等级的最大传输次数: sh hipriv.sh "vap0 alg_cfg ar_max_tx_cnt [传输次数]" */
    { "ar_max_tx_cnt",       MAC_ALG_CFG_AUTORATE_MAX_TX_COUNT },
    /* 设置是否开启80M 切40M的开关  sh hipriv.sh "vap0 alg_cfg ar_80M_40M_switch [1/0]" */
    { "ar_80M_40M_switch",   MAC_ALG_CFG_AUTORATE_80M_40M_SWITCH },
    /* 设置80M 切40M mcs门限  sh hipriv.sh "vap0 alg_cfg ar_40M_switch_thr [mcs]" */
    { "ar_40M_switch_thr",   MAC_ALG_CFG_AUTORATE_40M_SWITCH_THR },
    { "ar_collision_det",    MAC_ALG_CFG_AUTORATE_COLLISION_DET_EN },
    { "ar_switch_11b",       MAC_ALG_CFG_AUTORATE_SWITCH_11B },
    { "ar_rom_nss_auth",     MAC_ALG_CFG_AUTORATE_ROM_NSS_AUTH },
    { "sm_train_num",        MAC_ALG_CFG_SMARTANT_TRAINING_PACKET_NUMBER },
    { "sm_change_ant",       MAC_ALG_CFG_SMARTANT_CHANGE_ANT },
    { "sm_enable",           MAC_ALG_CFG_SMARTANT_ENABLE },
    { "sm_certain_ant",      MAC_ALG_CFG_SMARTANT_CERTAIN_ANT },
    { "sm_start",            MAC_ALG_CFG_SMARTANT_START_TRAIN },
    { "sm_train_packet",     MAC_ALG_CFG_SMARTANT_SET_TRAINING_PACKET_NUMBER },
    { "sm_min_packet",       MAC_ALG_CFG_SMARTANT_SET_LEAST_PACKET_NUMBER },
    { "sm_ant_interval",     MAC_ALG_CFG_SMARTANT_SET_ANT_CHANGE_INTERVAL },
    { "sm_user_interval",    MAC_ALG_CFG_SMARTANT_SET_USER_CHANGE_INTERVAL },
    { "sm_max_period",       MAC_ALG_CFG_SMARTANT_SET_PERIOD_MAX_FACTOR },
    { "sm_change_freq",      MAC_ALG_CFG_SMARTANT_SET_ANT_CHANGE_FREQ },
    { "sm_change_th",        MAC_ALG_CFG_SMARTANT_SET_ANT_CHANGE_THRESHOLD },
    /* 弱干扰免疫中non-direct使能: sh hipriv.sh "vap0 alg_cfg anti_inf_imm_en 0|1" */
    { "anti_inf_imm_en",       MAC_ALG_CFG_ANTI_INTF_IMM_ENABLE },
    /* 弱干扰免疫中dynamic unlock使能: sh hipriv.sh "vap0 alg_cfg anti_inf_unlock_en 0|1" */
    { "anti_inf_unlock_en",    MAC_ALG_CFG_ANTI_INTF_UNLOCK_ENABLE },
    /* 弱干扰免疫中rssi统计周期: sh hipriv.sh "vap0 anti_inf_stat_time [time]" */
    { "anti_inf_stat_time",    MAC_ALG_CFG_ANTI_INTF_RSSI_STAT_CYCLE },
    /* 弱干扰免疫中unlock关闭周期: sh hipriv.sh "vap0 anti_inf_off_time [time]" */
    { "anti_inf_off_time",     MAC_ALG_CFG_ANTI_INTF_UNLOCK_CYCLE },
    /* 弱干扰免疫中unlock关闭持续时间: sh hipriv.sh "vap0 anti_inf_off_dur [time]" */
    { "anti_inf_off_dur",      MAC_ALG_CFG_ANTI_INTF_UNLOCK_DUR_TIME },
    /* 抗干扰nav免疫使能: sh hipriv.sh "vap0 alg_cfg anti_inf_nav_en 0|1" */
    { "anti_inf_nav_en",       MAC_ALG_CFG_ANTI_INTF_NAV_IMM_ENABLE },
    /* 弱干扰免疫goodput下降门限: sh hipriv.sh "vap0 alg_cfg anti_inf_gd_th [num]" */
    { "anti_inf_gd_th",        MAC_ALG_CFG_ANTI_INTF_GOODPUT_FALL_TH },
    /* 弱干扰免疫探测保持最大周期数: sh hipriv.sh "vap0 alg_cfg anti_inf_keep_max [num]" */
    { "anti_inf_keep_max",     MAC_ALG_CFG_ANTI_INTF_KEEP_CYC_MAX_NUM },
    /* 弱干扰免疫探测保持最大周期数: sh hipriv.sh "vap0 alg_cfg anti_inf_keep_min [num]" */
    { "anti_inf_keep_min",     MAC_ALG_CFG_ANTI_INTF_KEEP_CYC_MIN_NUM },
    /* 弱干扰免疫是否使能tx time探测: sh hipriv.sh "vap0 anti_inf_tx_pro_en 0|1" */
    { "anti_inf_per_pro_en",   MAC_ALG_CFG_ANTI_INTF_PER_PROBE_EN },
    /* tx time下降门限: sh hipriv.sh "vap0 alg_cfg anti_inf_txtime_th [val]" */
    { "anti_inf_txtime_th",    MAC_ALG_CFG_ANTI_INTF_TX_TIME_FALL_TH },
    /* per下降门限: sh hipriv.sh "vap0 alg_cfg anti_inf_per_th [val]" */
    { "anti_inf_per_th",       MAC_ALG_CFG_ANTI_INTF_PER_FALL_TH },
    /* goodput抖动门限: sh hipriv.sh "vap0 alg_cfg anti_inf_gd_jitter_th [val]" */
    { "anti_inf_gd_jitter_th", MAC_ALG_CFG_ANTI_INTF_GOODPUT_JITTER_TH },
    /* 弱干扰免疫debug的打印信息: sh hipriv.sh "vap0 alg_cfg anti_inf_debug_mode 0|1|2" */
    { "anti_inf_debug_mode",   MAC_ALG_CFG_ANTI_INTF_DEBUG_MODE },
    /* 设置干扰检测周期(ms):sh hipriv.sh "vap0 alg_cfg intf_det_cycle [val]" */
    { "intf_det_cycle",          MAC_ALG_CFG_INTF_DET_CYCLE },
    /* 设置干扰检测模式(同频开/邻频叠频开/都开):sh hipriv.sh "vap0 alg_cfg intf_det_mode 0|1" */
    { "intf_det_mode",           MAC_ALG_CFG_INTF_DET_MODE },
    /* 设置干扰检测debug模式(每个bit表示一类):sh hipriv.sh "vap0 alg_cfg intf_det_debug 0|1" */
    { "intf_det_debug",          MAC_ALG_CFG_INTF_DET_DEBUG },
    /* 设置干扰检测sta阈值(千分之x):sh hipriv.sh "vap0 alg_cfg intf_det_cothr_sta [val]" */
    { "intf_det_cothr_sta",      MAC_ALG_CFG_INTF_DET_COCH_THR_STA },
    /* 设置ACI干扰检测高门限:sh hipriv.sh "vap0 alg_cfg aci_high_th [val]" */
    { "aci_high_th",      MAC_ALG_CFG_INTF_DET_ACI_HIGH_TH },
    /* 设置ACI干扰检测高门限:sh hipriv.sh "vap0 alg_cfg aci_low_th [val]" */
    { "aci_low_th",      MAC_ALG_CFG_INTF_DET_ACI_LOW_TH },
    { "aci_sync_ratio_th",      MAC_ALG_CFG_INTF_DET_ACI_SYNC_TH },
    /* 设置干扰检测sta无干扰阈值(千分之x):sh hipriv.sh "vap0 alg_cfg intf_det_nointf_thr_sta [val]" */
    { "intf_det_nointf_thr_sta", MAC_ALG_CFG_INTF_DET_COCH_NOINTF_STA },
    /* 设置干扰检测ap udp阈值(千分之x):sh hipriv.sh "vap0 alg_cfg intf_det_cothr_udp [val]" */
    { "intf_det_cothr_udp",      MAC_ALG_CFG_INTF_DET_COCH_THR_UDP },
    /* 设置干扰检测ap tcp阈值(千分之x):sh hipriv.sh "vap0 alg_cfg intf_det_cothr_tcp [val]" */
    { "intf_det_cothr_tcp",      MAC_ALG_CFG_INTF_DET_COCH_THR_TCP },
    /* 设置干扰检测邻频干扰扫描周期:sh hipriv.sh "vap0 alg_cfg intf_det_adjscan_cyc [val]" */
    { "intf_det_adjscan_cyc",    MAC_ALG_CFG_INTF_DET_ADJCH_SCAN_CYC },
    /* 设置干扰检测邻频叠频干扰繁忙度阈值(千分之x):sh hipriv.sh "vap0 alg_cfg intf_det_adjratio_thr [val]" */
    { "intf_det_adjratio_thr",   MAC_ALG_CFG_INTF_DET_ADJRATIO_THR },
    /* 设置干扰检测邻频叠频干扰sync error阈值(千分之x):sh hipriv.sh "vap0 alg_cfg intf_det_sync_th [val]" */
    { "intf_det_sync_th",        MAC_ALG_CFG_INTF_DET_SYNC_THR },
    /* 设置干扰检测零频叠频干扰平均rssi阈值(千分之x):sh hipriv.sh "vap0 alg_cfg intf_det_ave_rssi [val]" */
    { "intf_det_ave_rssi",       MAC_ALG_CFG_INTF_DET_AVE_RSSI },
    /* 设置干扰检测非邻/叠频干扰繁忙度阈值(千分之x):sh hipriv.sh "vap0 alg_cfg intf_det_no_adjratio_th [val]" */
    { "intf_det_no_adjratio_th", MAC_ALG_CFG_INTF_DET_NO_ADJRATIO_TH },
    /* 设置干扰检测非邻/叠频干扰计数阈值:sh hipriv.sh "vap0 alg_cfg intf_det_no_adjcyc_th [val]" */
    { "intf_det_no_adjcyc_th",   MAC_ALG_CFG_INTF_DET_NO_ADJCYC_TH },
    /* 设置干扰检测非邻/叠频干扰计数阈值:sh hipriv.sh "vap0 alg_cfg intf_det_collision_th [val] */
    { "intf_det_collision_th",   MAC_ALG_CFG_INTF_DET_COLLISION_TH },
    /* 设置检测到负增益后不探测阈值:sh hipriv.sh "vap0 alg_cfg neg_det_noprobe_th [val]" */
    { "neg_det_noprobe_th",      MAC_ALG_CFG_NEG_DET_NONPROBE_TH },
    /* 开始统计日志:sh hipriv.sh "vap0 alg_intf_det_log intf_det_stat_log_do [val]" */
    { "intf_det_stat_log_do",    MAC_ALG_CFG_INTF_DET_STAT_LOG_START },
    /* 打印统计日志:sh hipriv.sh "vap0 alg_intf_det_log intf_det_stat_log_out" */
    { "intf_det_stat_log_out",   MAC_ALG_CFG_INTF_DET_STAT_LOG_WRITE },
    /* ap模式下edca优化使能模式: sh hipriv.sh "vap0 alg_cfg edca_opt_en_ap 0|1|2" */
    { "edca_opt_en_ap",          MAC_ALG_CFG_EDCA_OPT_AP_EN_MODE },
    /* sta模式下edca优化使能模式: sh hipriv.sh "vap0 alg_cfg edca_opt_en_sta 0|1" */
    { "edca_opt_en_sta",         MAC_ALG_CFG_EDCA_OPT_STA_EN },
    /* sta模式下edca txop limit优化使能模式: sh hipriv.sh "vap0 alg_cfg txop_limit_en_sta 0|1" */
    { "txop_limit_en_sta",       MAC_ALG_CFG_TXOP_LIMIT_STA_EN },
    /* sta模式下edca优化的weighting系数: sh hipriv.sh "vap0 alg_cfg edca_opt_sta_weight 0~3" */
    { "edca_opt_sta_weight",         MAC_ALG_CFG_EDCA_OPT_STA_WEIGHT },
    /* 是否打印相关信息，仅用于本地版本调试 */
    { "edca_opt_debug_mode",         MAC_ALG_CFG_EDCA_OPT_DEBUG_MODE },
    /* 针对单用户BE TCP业务的优化开关: sh hipriv.sh "vap0 alg_cfg edca_opt_one_tcp_opt 0|1" */
    { "edca_opt_one_tcp_opt",        MAC_ALG_CFG_EDCA_ONE_BE_TCP_OPT },
    /* 针对单用户BE TCP业务的调试开关: sh hipriv.sh "vap0 alg_cfg edca_opt_one_tcp_dbg 0|1" */
    { "edca_opt_one_tcp_dbg",        MAC_ALG_CFG_EDCA_ONE_BE_TCP_DBG },
    /* 针对单用户BE TCP业务的EDCA参数无干扰选择阈值: sh hipriv.sh "vap0 alg_cfg edca_opt_one_tcp_th_no_intf [val]" */
    { "edca_opt_one_tcp_th_no_intf", MAC_ALG_CFG_EDCA_ONE_BE_TCP_TH_NO_INTF },
    /* 针对单用户BE TCP业务的EDCA参数干扰选择阈值: sh hipriv.sh "vap0 alg_cfg edca_opt_one_tcp_th_intf [val]" */
    { "edca_opt_one_tcp_th_intf",    MAC_ALG_CFG_EDCA_ONE_BE_TCP_TH_INTF },
    /* CCA优化功能使能: sh hipriv.sh "vap0 alg_cfg cca_opt_alg_en_mode 0|1" */
    { "cca_opt_alg_en_mode",      MAC_ALG_CFG_CCA_OPT_ALG_EN_MODE },
    /* CCA优化DEBUG模式启动: sh hipriv.sh "vap0 alg_cfg cca_opt_debug_mode 0|1" */
    { "cca_opt_debug_mode",       MAC_ALG_CFG_CCA_OPT_DEBUG_MODE },
    /* CCA优化信道扫描的时间(ms):sh hipriv.sh "vap0 alg_cfg cca_opt_set_sync_err_th [time]" */
    { "cca_opt_set_cca_th_debug", MAC_ALG_CFG_CCA_OPT_SET_CCA_TH_DEBUG },
    /* CCA log开关 sh hipriv.sh "vap0 alg_cfg cca_opt_log 0|1" */
    { "cca_opt_log",            MAC_ALG_CFG_CCA_OPT_LOG },
    { "tpc_mode",               MAC_ALG_CFG_TPC_MODE },              /* 设置TPC工作模式 */
    { "tpc_dbg",                MAC_ALG_CFG_TPC_DEBUG },             /* 设置TPC的debug开关 */
    /* 设置TPC的log开关:sh hipriv.sh "vap0 alg_cfg tpc_log 1 */
    { "tpc_log",                MAC_ALG_CFG_TPC_LOG },
    /* 开始功率统计日志:sh hipriv.sh "vap0 alg_tpc_log tpc_stat_log_do [mac地址] [业务类别] [包数目]"
       如: sh hipriv.sh "vap0 alg_tpc_log tpc_stat_log_do 06:31:04:E3:81:02 1 1000" */
    { "tpc_stat_log_do",        MAC_ALG_CFG_TPC_STAT_LOG_START },
    /* 打印功率统计日志:sh hipriv.sh "vap0 alg_tpc_log tpc_stat_log_out 06:31:04:E3:81:02" */
    { "tpc_stat_log_out",       MAC_ALG_CFG_TPC_STAT_LOG_WRITE },
    /* 开始每包统计日志:sh hipriv.sh "vap0 alg_tpc_log tpc_pkt_log_do [mac地址] [业务类别] [包数目]"
       如: sh hipriv.sh "vap0 alg_tpc_log tpc_pkt_log_do 06:31:04:E3:81:02 1 1000" */
    { "tpc_pkt_log_do",         MAC_ALG_CFG_TPC_PER_PKT_LOG_START },
    /* 获取特殊帧功率:sh hipriv.sh "vap0 alg_tpc_log tpc_get_frame_pow beacon_pow" */
    { "tpc_get_frame_pow",      MAC_ALG_CFG_TPC_GET_FRAME_POW },
    { "tpc_reset_stat",         MAC_ALG_CFG_TPC_RESET_STAT },        /* 释放统计内存 */
    { "tpc_reset_pkt",          MAC_ALG_CFG_TPC_RESET_PKT },         /* 释放每包内存 */
    { "tpc_over_temp_th",       MAC_ALG_CFG_TPC_OVER_TMP_TH },       /* TPC过温门限 */
    { "tpc_dpd_enable_rate",    MAC_ALG_CFG_TPC_DPD_ENABLE_RATE },   /* 配置DPD生效的速率INDEX */
    { "tpc_target_rate_11b",    MAC_ALG_CFG_TPC_TARGET_RATE_11B },   /* 11b目标速率设置 */
    { "tpc_target_rate_11ag",   MAC_ALG_CFG_TPC_TARGET_RATE_11AG },  /* 11ag目标速率设置 */
    { "tpc_target_rate_11n20",  MAC_ALG_CFG_TPC_TARGET_RATE_HT40 },  /* 11n20目标速率设置 */
    { "tpc_target_rate_11n40",  MAC_ALG_CFG_TPC_TARGET_RATE_HT40 },  /* 11n40目标速率设置 */
    { "tpc_target_rate_11ac20", MAC_ALG_CFG_TPC_TARGET_RATE_VHT20 }, /* 11ac20目标速率设置 */
    { "tpc_target_rate_11ac40", MAC_ALG_CFG_TPC_TARGET_RATE_VHT40 }, /* 11ac40目标速率设置 */
    { "tpc_target_rate_11ac80", MAC_ALG_CFG_TPC_TARGET_RATE_VHT80 }, /* 11ac80目标速率设置 */

#ifdef _PRE_WLAN_FEATURE_MU_TRAFFIC_CTL
    { "traffic_ctl_enable",       MAC_ALG_CFG_TRAFFIC_CTL_ENABLE },
    { "traffic_ctl_timeout",      MAC_ALG_CFG_TRAFFIC_CTL_TIMEOUT },
    { "traffic_ctl_min_thres",    MAC_ALG_CFG_TRAFFIC_CTL_MIN_THRESHOLD },
    { "traffic_ctl_log_debug",    MAC_ALG_CFG_TRAFFIC_CTL_LOG_DEBUG },
    { "traffic_ctl_buf_thres",    MAC_ALG_CFG_TRAFFIC_CTL_BUF_THRESHOLD },
    { "traffic_ctl_buf_adj_enb",  MAC_ALG_CFG_TRAFFIC_CTL_BUF_ADJ_ENABLE },
    { "traffic_ctl_buf_adj_num",  MAC_ALG_CFG_TRAFFIC_CTL_BUF_ADJ_NUM },
    { "traffic_ctl_buf_adj_cyc",  MAC_ALG_CFG_TRAFFIC_CTL_BUF_ADJ_CYCLE },
    { "traffic_ctl_rx_rst_enb",   MAC_ALG_CFG_TRAFFIC_CTL_RX_RESTORE_ENABLE },
    { "traffic_ctl_rx_rst_num",   MAC_ALG_CFG_TRAFFIC_RX_RESTORE_NUM },
    { "traffic_ctl_rx_rst_thres", MAC_ALG_CFG_TRAFFIC_RX_RESTORE_THRESHOLD },

    { "rx_dscr_ctl_enable", MAC_ALG_CFG_RX_DSCR_CTL_ENABLE },
    { "rx_dscr_ctl_log_debug", MAC_ALG_CFG_RX_DSCR_CTL_LOG_DEBUG },
#endif

#ifdef _PRE_WLAN_FEATURE_MWO_DET
    /* 微波炉检测使能信号 */
    { "mwo_det_enable",        MAC_ALG_CFG_MWO_DET_ENABLE },
    /* 停止微波炉信号发送时间计时的天线口功率门限（单位dBm） */
    { "mwo_det_end_rssi_th",   MAC_ALG_CFG_MWO_DET_END_RSSI_TH },
    /* 启动微波炉信号发送时间计时的天线口功率门限（单位dBm） */
    { "mwo_det_start_rssi_th", MAC_ALG_CFG_MWO_DET_START_RSSI_TH },
    /* 发送描述符中anti_intf_1thr,c2 大于本门限选择无干扰速率，c2小于此门限选择有干扰速率 */
    { "mwo_det_debug",         MAC_ALG_CFG_MWO_DET_DEBUG },
#endif
#ifdef _PRE_WLAN_FEATURE_HID2D
    {"hid2d_debug_log_enable",        MAC_ALG_CFG_HID2D_DEBUG_ENABLE},  /* Debug 使能开关 */
    {"hid2d_always_rts",              MAC_ALG_CFG_HID2D_RTS_ENABLE},  /* 投屏模式强制开启RTS */
    {"hid2d_disable_1024qam",         MAC_ALG_CFG_HID2D_HIGH_BW_MCS_DISABLE},  /* 投屏模式关闭高性能特性 */
    {"hid2d_set_high_txpower",        MAC_ALG_CFG_HID2D_HIGH_TXPOWER_ENABLE},  /* 投屏模式保持高传输功率 */
#ifdef _PRE_WLAN_FEATURE_HID2D_PRESENTATION
    {"hid2d_set_cca",                 MAC_ALG_CFG_HID2D_SET_APK_CCA_TH}, /* 发布会场景下设置CCA门限 */
#endif
#endif
    { OAL_PTR_NULL }
};

OAL_CONST wal_ioctl_tlv_stru g_ast_set_tlv_table[] = {
    /* cmd: wlan0 set_tlv xx xx */
    { "tx_pkts_stat", WLAN_CFGID_SET_DEVICE_PKT_STAT },
    { "auto_freq", WLAN_CFGID_SET_DEVICE_FREQ },

    { "set_adc_dac_freq", WLAN_CFGID_SET_ADC_DAC_FREQ }, /* 设置ADC DAC频率 */
    { "set_mac_freq", WLAN_CFGID_SET_MAC_FREQ },         /* 设MAC频率 */

    { "rx_ampdu_num", WLAN_CFGID_SET_ADDBA_RSP_BUFFER },
    { "data_collect", WLAN_CFGID_DATA_COLLECT },    /* 第一个参数为模式，第二个参数为点数 */
    /* cmd: wlan0 set_val xx */
    { "tx_ampdu_type",  WLAN_CFGID_SET_TX_AMPDU_TYPE },  /* 设置聚合类型的开关 */
    { "tx_ampdu_amsdu", WLAN_CFGID_AMSDU_AMPDU_SWITCH }, /* 设置tx amsdu ampdu联合聚合功能的开关 */
    { "rx_ampdu_amsdu", WLAN_CFGID_SET_RX_AMPDU_AMSDU }, /* 设置rx ampdu amsdu 联合聚合功能的开关 */

    { "sk_pacing_shift", WLAN_CFGID_SET_SK_PACING_SHIFT },

    { "trx_stat_log_en", WLAN_CFGID_SET_TRX_STAT_LOG }, /* 设置tcp ack缓存时吞吐量统计维测开关，其他模块可参考 */
    { "mimo_blacklist", WLAN_CFGID_MIMO_BLACKLIST },    /* 设置探测MIMO黑名单机制开关 */
    /* cmd: wlan0 set_val dfs_debug 0|1 */
    { "dfs_debug", WLAN_CFGID_SET_DFS_MODE },           /* 设置dfs是否为检测率模式的开关 */
#ifdef _PRE_WLAN_FEATURE_HID2D_TX_DROP
    { "hid2d_debug_info", WLAN_CFGID_HID2D_DEBUG_MODE },           /* 设置dfs是否为检测率模式的开关 */
#endif
    { "warning_mode", WLAN_CFGID_SET_WARNING_MODE },    /* 设置部分WARNING是否为测试模式的开关 */

    { "chr_mode", WLAN_CFGID_SET_CHR_MODE },            /* 设置chr模式的开关 */
    {"linkloss_csa_dis", WLAN_CFGID_SET_LINKLOSS_DISABLE_CSA},    /* 配置linkloss是否禁止csa */
#if defined(_PRE_WLAN_FEATURE_HID2D) && defined(_PRE_WLAN_FEATURE_HID2D_PRESENTATION)
    { "hid2d_presentation", WLAN_CFGID_HID2D_PRESENTATION_MODE }, /* 配置为发布会场景 */
#endif
    { OAL_PTR_NULL }
};

#ifdef _PRE_WLAN_FEATURE_11AX
OAL_CONST wal_ioctl_tlv_stru g_ast_11ax_debug_table[] = {
    /* cmd: wlan0 xxx  2  xxx 1 xxx 0 */
    { "print_log",        MAC_VAP_11AX_DEBUG_PRINT_LOG },
    { "tid",              MAC_VAP_11AX_DEBUG_HE_TB_PPDU_TID_NUM },
    { "htc_order",        MAC_VAP_11AX_DEBUG_HE_TB_PPDU_HTC_ORGER },
    { "htc_val",          MAC_VAP_11AX_DEBUG_HE_TB_PPDU_HTC_VALUE },
    { "fix_power",        MAC_VAP_11AX_DEBUG_HE_TB_PPDU_FIX_POWER },
    { "power_val",        MAC_VAP_11AX_DEBUG_HE_TB_PPDU_POWER_VALUE },
    { "disable_ba_check", MAC_VAP_11AX_DEBUG_HE_TB_PPDU_DISABLE_BA_CHECK },
    { "disable_mu_edca",  MAC_VAP_11AX_DEBUG_DISABLE_MU_EDCA },
    { "manual_cfo",       MAC_VAP_11AX_DEBUG_MANUAL_CFO },
    { "bsrp",             MAC_VAP_11AX_DEBUG_BSRP_CFG },
    { "bsrp_tid",         MAC_VAP_11AX_DEBUG_BSRP_TID },
    { "bsrp_size",        MAC_VAP_11AX_DEBUG_BSRP_QUEUE_SIZE },
    { "mpad_dur",         MAC_VAP_11AX_DEBUG_MAC_PADDING },
    { "tb_pwr_test",      MAC_VAP_11AX_DEBUG_POW_TEST },
    { "tb_ppdu_len",      MAC_VAP_11AX_DEBUG_TB_PPDU_LEN },
    { "tb_ppdu_ac",       MAC_VAP_11AX_DEBUG_TB_PPDU_AC },
    { "print_rx_trig",    MAC_VAP_11AX_DEBUG_PRINT_RX_TRIG_FRAME },
    { "om_auth_flag",     MAC_VAP_11AX_DEBUG_OM_AUTH_FLAG },
    { "tom_bw",           MAC_VAP_11AX_DEBUG_TOM_BW_FLAG },
    { "tom_nss",          MAC_VAP_11AX_DEBUG_TOM_NSS_FLAG },
    { "tom_ul_mu_disable", MAC_VAP_11AX_DEBUG_TOM_UL_MU_DISABLE_FLAG },
    { "uora_ocw",         MAC_VAP_11AX_DEBUG_UORA_OCW_UPDATE},
    { "uora_obo_bypass",  MAC_VAP_11AX_DEBUG_UORA_OBO_BYPASS},
    { "bss_color",        MAC_VAP_11AX_DEBUG_BSS_COLOR },

    { OAL_PTR_NULL }
};
#endif

/* MAC TX RX common info report命令表格 */
OAL_CONST wal_ioctl_tlv_stru g_ast_mac_tx_report_debug_table[] = {
    /* cmd: sh hipriv.sh "wlan0 set_str mac_report_tx XXX(命令参数个数) index XXX status XXX bw XXX protocol XXX
     *                    fr_type XXX sub_type XXX ampdu XXX psdu XXX hw_retry XXX"
     * MAC TX common命令举例:
     * 1.统计TB QoS NULL: sh hipriv.sh "wlan0 set_str mac_report_tx 4 index 0 protocol 11 fr_type 2 sub_type 12"
     * 2.清除统计寄存器配置信息:sh hipriv.sh "wlan0 set_str mac_report_tx 2 index 0 clear 1"
     * 注意:(1)tx只有index0支持status的配置，其他index不支持 (2)清除统计后重新统计需要写0之后才能正常计数
     *      (3)在一条命令中配置多个选项可能导致命令长度超过限制，可以分次进行配置
     * MAC寄存器CFG_TX_COMMON_CNT_CTRL各bit位配置详见寄存器表单
     */
    { "index",  MAC_TX_COMMON_REPORT_INDEX },            /* tx rx分别有8个 index:0-7 */
    { "status", MAC_TX_COMMON_REPORT_STATUS },           /* 0：发送成功 1：响应帧错误 2：响应帧超时 3：发送异常结束 */
    { "bw",     MAC_TX_COMMON_REPORT_BW_MODE },          /* bandwidth 0:20M 1:40M 2:80M 3:160M */
    /* 0000: The data rate is 11b type                    0001: The data rate is legacy OFDM type
     * 0010: The data rate is HT Mixed mode Frame type    0011: The data rate is HT Green Field Frame type
     * 0100: The data rate is VHT type                    0101~0111：reserved
     * 1000：The data rate is HE  SU Format type          1001：The data rate is HE  MU Format type
     * 1010：The data rate is HE  EXT SU Format type      1011：The data rate is HE  TRIG Format type
     * 1100~1111:reserved
     */
    { "protocol", MAC_TX_COMMON_REPORT_PROTOCOL_MODE },  /* 协议模式 */
    { "fr_type",  MAC_TX_COMMON_REPORT_FRAME_TYPE },     /* 帧类型 */
    { "sub_type", MAC_TX_COMMON_REPORT_SUB_TYPE },       /* 子类型 */
    { "ampdu",    MAC_TX_COMMON_REPORT_APMDU },          /* 0：非ampdu时统计 1：ampdu时统计 */
    { "psdu",     MAC_TX_COMMON_REPORT_PSDU },           /* 按psdu统计还是按mpdu统计：0：按mpdu统计 1：按psdu统计 */
    { "hw_retry", MAC_TX_COMMON_REPORT_HW_RETRY },       /* 0：非hw retry帧时统计 1：hw retry帧时统计 */
    { "clear",    MAC_TX_COMMON_REPORT_CTRL_REG_CLEAR }, /* 清除对应index的寄存器配置 */

    { OAL_PTR_NULL }
};

OAL_CONST wal_ioctl_tlv_stru g_ast_mac_rx_report_debug_table[] = {
    /* cmd: sh hipriv.sh "wlan0 set_str mac_report_rx 11(命令参数个数) index 5 status 1 bw 2 protocol 1 fr_type 2
     *                     sub_type 4 ampdu 1 psdu 1 vap_mode 3 bss 1 direct 1"
     *  clear命令: sh hipriv.sh "wlan0 set_str mac_report_rx index 5 clear 1"
     *  注意：清除统计后重新统计需要写0之后才能正常计数
     */
    /* 0：Invalid  1：RX successful
     * 2：Duplicate detected 3：FCS check failed
     * 4：Key search failed 5：MIC check failed
     * 6：ICV failed        others：Reserved
     */
    { "status", MAC_RX_COMMON_REPORT_STATUS },           /* 接收状态 */
    { "bw", MAC_RX_COMMON_REPORT_BW_MODE },              /* bandwidth 0:20M 1:40M 2:80M 3:160M */
    /* 0000: The data rate is 11b type                    0001: The data rate is legacy OFDM type
       0010: The data rate is HT Mixed mode Frame type    0011: The data rate is HT Green Field Frame type
       0100: The data rate is VHT type                    0101~0111：reserved
       1000：The data rate is HE  SU Format type          1001：The data rate is HE  MU Format type
       1010：The data rate is HE  EXT SU Format type      1011：The data rate is HE  TRIG Format type
       1100~1111:reserved */
    { "protocol", MAC_RX_COMMON_REPORT_PROTOCOL_MODE },  /* 协议模式 */
    { "fr_type",  MAC_RX_COMMON_REPORT_FRAME_TYPE },     /* 帧类型 */
    { "sub_type", MAC_RX_COMMON_REPORT_SUB_TYPE },       /* 子类型 */
    { "ampdu",    MAC_RX_COMMON_REPORT_APMDU },          /* 0：非ampdu时统计 1：ampdu时统计 */
    { "psdu",     MAC_RX_COMMON_REPORT_PSDU },           /* 按psdu统计还是按mpdu统计：0：按mpdu统计 1：按psdu统计 */
    { "vap_mode", MAC_RX_COMMON_REPORT_VAP_CHK },        /* 是否按vap统计:0~4：接收到vap0~4计数 5~6:resv 7:不区分vap */
    { "bss",      MAC_RX_COMMON_REPORT_BSS_CHK },        /* 0：非本bss时统计 1：本bss时统计 */
    { "direct",   MAC_RX_COMMON_REPORT_DIRECT_CHK },     /* 0：非direct帧时统计 1：direct帧时统计 */
    { "clear",    MAC_RX_COMMON_REPORT_CTRL_REG_CLEAR }, /* 清除对应index的寄存器配置 */

    { OAL_PTR_NULL }
};

OAL_CONST wal_ioctl_tlv_stru g_ast_common_debug_table[] = {
    { "pg_switch", PG_EFFICIENCY_STATISTICS_ENABLE },
    { "pg_info", PG_EFFICIENCY_INFO },

    { "hw_txq", MAC_VAP_COMMON_SET_TXQ },
    { "tx_info", MAC_VAP_COMMON_TX_INFO },

#ifdef _PRE_WLAN_FEATURE_MBO
    { "mbo_switch",                  MBO_SWITCH },        /* 测试用例1:MBO特性开关， out-of-the-box test */
    { "mbo_cell_capa",               MBO_CELL_CAP },      /* 测试用例2:MBO Capability Indication test  */
    { "mbo_assoc_disallowed_switch", MBO_ASSOC_DISALLOWED_SWITCH }, /* 测试用例6 */
#endif

#ifdef _PRE_WLAN_FEATURE_HIEX
    { "set_dev_hiex",                 HIEX_DEV_CAP },     /* mac device hiex cap cmd */
    { "hiex_debug_switch",            HIEX_DEBUG}, /* hiex debug */
#endif

    { "rifs_en",                      RIFS_ENABLE },
    { "greenfield_en",                GREENFIELD_ENABLE },
    { "protect_info",                 PROTECTION_LOG_SWITCH},

#ifdef _PRE_WLAN_FEATURE_11AX
    {"rx_frame_cnt",                 DMAC_RX_FRAME_STATISTICS}, /* 接收帧统计 */
    {"tx_frame_cnt",                 DMAC_TX_FRAME_STATISTICS}, /* 发送帧统计 */
    {"su_1xltf_0.8us_gi",            SU_PPDU_1xLTF_08US_GI_SWITCH},
#endif
    {"auth_rsp_timeout",             AUTH_RSP_TIMEOUT},
    {"forbid_open_auth",             FORBIT_OPEN_AUTH},

#ifdef _PRE_WLAN_RX_LISTEN_POWER_SAVING
    {"hal_ps_debug_switch",          HAL_PS_DEBUG_SWITCH},
#endif
    {"ht_self_cure",                 HT_SELF_CURE_DEBUG},
    {"set_bindcpu",                  USERCTL_BINDCPU},
    { OAL_PTR_NULL }
};

OAL_STATIC OAL_CONST wal_ioctl_tlv_stru g_ast_debug_log_table[] = {
    { "encap_ampdu", MAC_LOG_ENCAP_AMPDU },
#ifdef _PRE_WLAN_FEATURE_MONITOR
    { "monitor_ota", MAC_LOG_MONITOR_OTA_RPT },
#endif
    { OAL_PTR_NULL }
};

/* cmd: wlan0 set_str xxx  2  xxx 1 xxx 0 */
OAL_CONST wal_ioctl_str_stru g_ast_set_str_table[] = {
#ifdef _PRE_WLAN_FEATURE_11AX
    { "11ax_debug", WLAN_CFGID_11AX_DEBUG, (wal_ioctl_tlv_stru *)&g_ast_11ax_debug_table },
#endif

    { "mac_report_tx", WLAN_CFGID_MAC_TX_COMMON_REPORT, (wal_ioctl_tlv_stru *)&g_ast_mac_tx_report_debug_table },
    { "mac_report_rx", WLAN_CFGID_MAC_RX_COMMON_REPORT, (wal_ioctl_tlv_stru *)&g_ast_mac_rx_report_debug_table },
    { "common_debug",  WLAN_CFGID_COMMON_DEBUG,         (wal_ioctl_tlv_stru *)&g_ast_common_debug_table },
    { "debug_log",     WLAN_CFGID_LOG_DEBUG,         (wal_ioctl_tlv_stru *)&g_ast_debug_log_table },

    { OAL_PTR_NULL }
};

OAL_CONST wal_dfs_domain_entry_stru g_ast_dfs_domain_table[] = {
    { "AE", MAC_DFS_DOMAIN_ETSI },
    { "AL", MAC_DFS_DOMAIN_NULL },
    { "AM", MAC_DFS_DOMAIN_ETSI },
    { "AN", MAC_DFS_DOMAIN_ETSI },
    { "AR", MAC_DFS_DOMAIN_FCC },
    { "AT", MAC_DFS_DOMAIN_ETSI },
    { "AU", MAC_DFS_DOMAIN_FCC },
    { "AZ", MAC_DFS_DOMAIN_ETSI },
    { "BA", MAC_DFS_DOMAIN_ETSI },
    { "BE", MAC_DFS_DOMAIN_ETSI },
    { "BG", MAC_DFS_DOMAIN_ETSI },
    { "BH", MAC_DFS_DOMAIN_ETSI },
    { "BL", MAC_DFS_DOMAIN_NULL },
    { "BN", MAC_DFS_DOMAIN_ETSI },
    { "BO", MAC_DFS_DOMAIN_ETSI },
    { "BR", MAC_DFS_DOMAIN_FCC },
    { "BY", MAC_DFS_DOMAIN_ETSI },
    { "BZ", MAC_DFS_DOMAIN_ETSI },
    { "CA", MAC_DFS_DOMAIN_FCC },
    { "CH", MAC_DFS_DOMAIN_ETSI },
    { "CL", MAC_DFS_DOMAIN_NULL },
    { "CN", MAC_DFS_DOMAIN_CN },
    { "CO", MAC_DFS_DOMAIN_FCC },
    { "CR", MAC_DFS_DOMAIN_FCC },
    { "CS", MAC_DFS_DOMAIN_ETSI },
    { "CY", MAC_DFS_DOMAIN_ETSI },
    { "CZ", MAC_DFS_DOMAIN_ETSI },
    { "DE", MAC_DFS_DOMAIN_ETSI },
    { "DK", MAC_DFS_DOMAIN_ETSI },
    { "DO", MAC_DFS_DOMAIN_FCC },
    { "DZ", MAC_DFS_DOMAIN_NULL },
    { "EC", MAC_DFS_DOMAIN_FCC },
    { "EE", MAC_DFS_DOMAIN_ETSI },
    { "EG", MAC_DFS_DOMAIN_ETSI },
    { "ES", MAC_DFS_DOMAIN_ETSI },
    { "FI", MAC_DFS_DOMAIN_ETSI },
    { "FR", MAC_DFS_DOMAIN_ETSI },
    { "GB", MAC_DFS_DOMAIN_ETSI },
    { "GE", MAC_DFS_DOMAIN_ETSI },
    { "GR", MAC_DFS_DOMAIN_ETSI },
    { "GT", MAC_DFS_DOMAIN_FCC },
    { "HK", MAC_DFS_DOMAIN_FCC },
    { "HN", MAC_DFS_DOMAIN_FCC },
    { "HR", MAC_DFS_DOMAIN_ETSI },
    { "HU", MAC_DFS_DOMAIN_ETSI },
    { "ID", MAC_DFS_DOMAIN_NULL },
    { "IE", MAC_DFS_DOMAIN_ETSI },
    { "IL", MAC_DFS_DOMAIN_ETSI },
    { "IN", MAC_DFS_DOMAIN_NULL },
    { "IQ", MAC_DFS_DOMAIN_NULL },
    { "IR", MAC_DFS_DOMAIN_NULL },
    { "IS", MAC_DFS_DOMAIN_ETSI },
    { "IT", MAC_DFS_DOMAIN_ETSI },
    { "JM", MAC_DFS_DOMAIN_FCC },
    { "JO", MAC_DFS_DOMAIN_ETSI },
    { "JP", MAC_DFS_DOMAIN_MKK },
    { "KP", MAC_DFS_DOMAIN_NULL },
    { "KR", MAC_DFS_DOMAIN_KOREA },
    { "KW", MAC_DFS_DOMAIN_ETSI },
    { "KZ", MAC_DFS_DOMAIN_NULL },
    { "LB", MAC_DFS_DOMAIN_NULL },
    { "LI", MAC_DFS_DOMAIN_ETSI },
    { "LK", MAC_DFS_DOMAIN_FCC },
    { "LT", MAC_DFS_DOMAIN_ETSI },
    { "LU", MAC_DFS_DOMAIN_ETSI },
    { "LV", MAC_DFS_DOMAIN_ETSI },
    { "MA", MAC_DFS_DOMAIN_NULL },
    { "MC", MAC_DFS_DOMAIN_ETSI },
    { "MK", MAC_DFS_DOMAIN_ETSI },
    { "MO", MAC_DFS_DOMAIN_FCC },
    { "MT", MAC_DFS_DOMAIN_ETSI },
    { "MX", MAC_DFS_DOMAIN_FCC },
    { "MY", MAC_DFS_DOMAIN_FCC },
    { "NG", MAC_DFS_DOMAIN_NULL },
    { "NL", MAC_DFS_DOMAIN_ETSI },
    { "NO", MAC_DFS_DOMAIN_ETSI },
    { "NP", MAC_DFS_DOMAIN_NULL },
    { "NZ", MAC_DFS_DOMAIN_FCC },
    { "OM", MAC_DFS_DOMAIN_FCC },
    { "PA", MAC_DFS_DOMAIN_FCC },
    { "PE", MAC_DFS_DOMAIN_FCC },
    { "PG", MAC_DFS_DOMAIN_FCC },
    { "PH", MAC_DFS_DOMAIN_FCC },
    { "PK", MAC_DFS_DOMAIN_NULL },
    { "PL", MAC_DFS_DOMAIN_ETSI },
    { "PR", MAC_DFS_DOMAIN_FCC },
    { "PT", MAC_DFS_DOMAIN_ETSI },
    { "QA", MAC_DFS_DOMAIN_NULL },
    { "RO", MAC_DFS_DOMAIN_ETSI },
    { "RU", MAC_DFS_DOMAIN_FCC },
    { "SA", MAC_DFS_DOMAIN_FCC },
    { "SE", MAC_DFS_DOMAIN_ETSI },
    { "SG", MAC_DFS_DOMAIN_NULL },
    { "SI", MAC_DFS_DOMAIN_ETSI },
    { "SK", MAC_DFS_DOMAIN_ETSI },
    { "SV", MAC_DFS_DOMAIN_FCC },
    { "SY", MAC_DFS_DOMAIN_NULL },
    { "TH", MAC_DFS_DOMAIN_FCC },
    { "TN", MAC_DFS_DOMAIN_ETSI },
    { "TR", MAC_DFS_DOMAIN_ETSI },
    { "TT", MAC_DFS_DOMAIN_FCC },
    { "TW", MAC_DFS_DOMAIN_NULL },
    { "UA", MAC_DFS_DOMAIN_NULL },
    { "US", MAC_DFS_DOMAIN_FCC },
    { "UY", MAC_DFS_DOMAIN_FCC },
    { "UZ", MAC_DFS_DOMAIN_FCC },
    { "VE", MAC_DFS_DOMAIN_FCC },
    { "VN", MAC_DFS_DOMAIN_ETSI },
    { "YE", MAC_DFS_DOMAIN_NULL },
    { "ZA", MAC_DFS_DOMAIN_FCC },
    { "ZW", MAC_DFS_DOMAIN_NULL },
};
/* 解圈复杂度需要，wal_hipriv_process_rate_params建立的2个数组 */
OAL_STATIC int32_t g_al_mcs_min_table[WAL_HIPRIV_MCS_TYPE_NUM] = {
    WAL_HIPRIV_HT_MCS_MIN,
    WAL_HIPRIV_VHT_MCS_MIN,
    WAL_HIPRIV_HE_MCS_MIN,
#ifdef  _PRE_WLAN_FEATURE_11AX_ER_SU_DCM
    WAL_HIPRIV_HE_ER_MCS_MIN
#endif
};

OAL_STATIC int32_t g_al_mcs_max_table[WAL_HIPRIV_MCS_TYPE_NUM] = {
    WAL_HIPRIV_HT_MCS_MAX,
    WAL_HIPRIV_VHT_MCS_MAX,
    WAL_HIPRIV_HE_MCS_MAX,
#ifdef  _PRE_WLAN_FEATURE_11AX_ER_SU_DCM
    WAL_HIPRIV_HE_ER_MCS_MAX
#endif
};


uint32_t wal_get_cmd_one_arg(int8_t *pc_cmd, int8_t *pc_arg,
    uint32_t ul_arg_len, uint32_t *pul_cmd_offset)
{
    int8_t *pc_cmd_copy = OAL_PTR_NULL;
    uint32_t ul_pos = 0;

    if (oal_unlikely(oal_any_null_ptr3(pc_cmd, pc_arg, pul_cmd_offset))) {
        oam_error_log3(0, OAM_SF_ANY, "{wal_get_cmd_one_arg::pc_cmd/pc_arg/pul_cmd_offset null ptr err %x/%x/%x!}",
            (uintptr_t)pc_cmd, (uintptr_t)pc_arg, (uintptr_t)pul_cmd_offset);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pc_cmd_copy = pc_cmd;

    /* 去掉字符串开始的空格 */
    while (*pc_cmd_copy == ' ') {
        ++pc_cmd_copy;
    }

    while ((*pc_cmd_copy != ' ') && (*pc_cmd_copy != '\0')) {
        pc_arg[ul_pos] = *pc_cmd_copy;
        ++ul_pos;
        ++pc_cmd_copy;

        if (oal_unlikely(ul_pos >= ul_arg_len)) {
            oam_warning_log1(0, OAM_SF_ANY, "{wal_get_cmd_one_arg::pos>=HIPRIV_CMD_NAME_MAX_LEN, pos:%d}", ul_pos);
            return OAL_ERR_CODE_ARRAY_OVERFLOW;
        }
    }

    pc_arg[ul_pos] = '\0';

    /* 字符串到结尾，返回错误码(命令码之后无) */
    if (ul_pos == 0) {
        oam_info_log0(0, OAM_SF_ANY, "{wal_get_cmd_one_arg::return param pc_arg is null!}");
        return OAL_ERR_CODE_CONFIG_ARGS_OVER;
    }

    *pul_cmd_offset = (uint32_t)(pc_cmd_copy - pc_cmd);

    return OAL_SUCC;
}


uint8_t *wal_get_reduce_sar_ctrl_params(uint8_t uc_tx_power_lvl)
{
#if defined(_PRE_WLAN_FEATURE_TPC_OPT) && defined(_PRE_PLAT_FEATURE_CUSTOMIZE)
    /* 最大发送功率定制化数组 */
    wlan_init_cust_nvram_params *pst_cust_nv_params = hwifi_get_init_nvram_params();
    if ((uc_tx_power_lvl <= CUS_NUM_OF_SAR_LVL) && (uc_tx_power_lvl > 0)) {
        uc_tx_power_lvl--;
    } else {
        return OAL_PTR_NULL;
    }

    return (uint8_t *)pst_cust_nv_params->st_sar_ctrl_params[uc_tx_power_lvl];
#else
    return OAL_PTR_NULL;
#endif
}


uint32_t wal_hipriv_set_fix_rate_pre_config(oal_net_device_stru *pst_net_dev,
    oal_bool_enum_uint8 en_fix_rate_enable, mac_cfg_set_dscr_param_stru *pc_stu)
{
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    uint32_t ul_ret;
    uint8_t uc_ampdu_cfg_idx;
    int8_t ac_sw_ampdu_cmd[WAL_AMPDU_CFG_BUTT][WAL_HIPRIV_CMD_NAME_MAX_LEN] = { { "0" }, { "1" } };

    pst_mac_vap = oal_net_dev_priv(pst_net_dev);
    if (oal_unlikely(pst_mac_vap == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_hipriv_set_fix_rate_pre_config::(pst_net_dev)null ptr.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 配置无效固定速率时，开启autorate，恢复聚合开关值 */
    if (en_fix_rate_enable == OAL_FALSE) {
        /* 开启autorate算法 */
        ul_ret = wal_hipriv_alg_cfg(pst_net_dev, "ar_enable 1");
        if (ul_ret != OAL_SUCC) {
            oam_error_log1(pst_mac_vap->uc_vap_id, 0, "{wal_hipriv_set_fix_rate_pre_config:ar_enable fail:d%}", ul_ret);
            return OAL_FAIL;
        }

        if (pst_mac_vap->st_fix_rate_pre_para.en_rate_cfg_tag == OAL_TRUE) {
            /*  恢复ampdu聚合  */
            uc_ampdu_cfg_idx = pst_mac_vap->st_fix_rate_pre_para.en_tx_ampdu_last;
            ul_ret = wal_hipriv_ampdu_tx_on(pst_net_dev, ac_sw_ampdu_cmd[uc_ampdu_cfg_idx]);
            if (ul_ret != OAL_SUCC) {
                oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
                    "{wal_hipriv_set_fix_rate_pre_config::ampdu cmd cfg fail[d%]}", ul_ret);
                return OAL_FAIL;
            }

            /* 记录固定速率配置标记为未配置状态 */
            pst_mac_vap->st_fix_rate_pre_para.en_rate_cfg_tag = OAL_FALSE;
        }

        return OAL_SUCC;
    }

    /*  关闭autorate算法  */
    ul_ret = wal_hipriv_alg_cfg(pst_net_dev, "ar_enable 0");
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(pst_mac_vap->uc_vap_id, 0, "{wal_hipriv_set_fix_rate_pre_config::ar disable fail[d%]}", ul_ret);
        return OAL_FAIL;
    }

    /* 11abg模式下配置固定速率前关闭ampdu聚合 */
    if (pc_stu->uc_function_index == WAL_DSCR_PARAM_RATE) {
        if (pst_mac_vap->st_fix_rate_pre_para.en_rate_cfg_tag != OAL_TRUE) {
            /* 记录ampdu开关状态 */
            pst_mac_vap->st_fix_rate_pre_para.en_tx_ampdu_last = mac_mib_get_CfgAmpduTxAtive(pst_mac_vap);
            /* 记录固定速率配置标记为已配置状态 */
            pst_mac_vap->st_fix_rate_pre_para.en_rate_cfg_tag = OAL_TRUE;
        }

        /*  关闭ampdu聚合  */
        ul_ret = wal_hipriv_ampdu_tx_on(pst_net_dev, ac_sw_ampdu_cmd[WAL_AMPDU_DISABLE]);
        if (ul_ret != OAL_SUCC) {
            oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
                "{wal_hipriv_set_fix_rate_pre_config::disable ampdu fail:%d}", ul_ret);
            return OAL_FAIL;
        }
    }

    return OAL_SUCC;
}

OAL_STATIC uint32_t wal_hipriv_set_non_ht_rate(mac_cfg_non_ht_rate_stru *pst_set_non_ht_rate_param,
    wal_dscr_param_enum_uint8 en_param_index)
{
    if (en_param_index <= WLAN_SHORT_11b_11_M_BPS) {
        pst_set_non_ht_rate_param->en_protocol_mode = WLAN_11B_PHY_PROTOCOL_MODE;
    } else if (en_param_index >= WLAN_LEGACY_OFDM_48M_BPS && en_param_index <= WLAN_LEGACY_OFDM_9M_BPS) {
        pst_set_non_ht_rate_param->en_protocol_mode = WLAN_LEGACY_OFDM_PHY_PROTOCOL_MODE;
    } else {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_set_non_ht_rate::invalid rate!}");
        return OAL_ERR_CODE_INVALID_CONFIG;
    }
    return OAL_SUCC;
}

OAL_STATIC uint32_t wal_hipriv_process_rate_params_rate(oal_net_device_stru *pst_net_dev,
    int8_t *ac_arg, mac_cfg_set_dscr_param_stru *pc_stu)
{
    wal_dscr_param_enum_uint8 en_param_index;
    mac_cfg_non_ht_rate_stru *pst_set_non_ht_rate_param = OAL_PTR_NULL;

    pst_set_non_ht_rate_param = (mac_cfg_non_ht_rate_stru *)(&(pc_stu->l_value));
    /* 解析参数 */
    for (en_param_index = 0; en_param_index < WLAN_LEGACY_RATE_VALUE_BUTT; en_param_index++) {
        if (!oal_strcmp(pauc_non_ht_rate_tbl[en_param_index], ac_arg)) {
            pst_set_non_ht_rate_param->en_rate = en_param_index;
            break;
        }
    }

    /* 根据速率配置TX描述符中的协议模式 ; 对于05或者03 ram 会在dmac 根据速率刷新协议 */
    if (OAL_SUCC != wal_hipriv_set_non_ht_rate(pst_set_non_ht_rate_param, en_param_index)) {
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    if (OAL_SUCC != wal_hipriv_set_fix_rate_pre_config(pst_net_dev, OAL_TRUE, pc_stu)) {
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

OAL_STATIC uint32_t wal_hipriv_process_rate_params_mcs(oal_net_device_stru *pst_net_dev, int8_t *ac_arg,
    mac_cfg_set_dscr_param_stru *pc_stu)
{
    int32_t l_val;
    uint8_t uc_mcs_index;
    uint32_t ul_ret;

    l_val = oal_strtol(ac_arg, OAL_PTR_NULL, 0);
    uc_mcs_index = pc_stu->uc_function_index - WAL_DSCR_PARAM_MCS;
    if (l_val < g_al_mcs_min_table[uc_mcs_index] || l_val > g_al_mcs_max_table[uc_mcs_index]) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_process_rate_params::input mcs out of range[%d]}", l_val);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }
    pc_stu->l_value = l_val;
    ul_ret = wal_hipriv_set_fix_rate_pre_config(pst_net_dev, OAL_TRUE, pc_stu);
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(0, OAM_SF_ANY, "{wal_hipriv_process_rate_params:cfg fixed rate hdl fail[%d]}", ul_ret);
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

OAL_STATIC uint32_t wal_hipriv_process_rate_params_nss(int8_t *ac_arg,
    mac_cfg_set_dscr_param_stru *pc_stu)
{
    wal_dscr_param_enum_uint8 en_param_index;

    for (en_param_index = 0; en_param_index < WLAN_NSS_LIMIT; en_param_index++) {
        if (!oal_strcmp(pauc_tx_dscr_nss_tbl[en_param_index], ac_arg)) {
            pc_stu->l_value = en_param_index;
            break;
        }
    }
    if (en_param_index == WAL_DSCR_PARAM_BUTT) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_process_rate_params::invalid param for nss!}");
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    return OAL_SUCC;
}

OAL_STATIC uint32_t wal_hipriv_process_rate_params_bw(int8_t *ac_arg,
    mac_cfg_set_dscr_param_stru *pc_stu)
{
    wal_dscr_param_enum_uint8 en_param_index;

    for (en_param_index = 0; en_param_index < WLAN_BANDWITH_CAP_BUTT; en_param_index++) {
        if (!oal_strcmp(pauc_bw_tbl[en_param_index], ac_arg)) {
            pc_stu->l_value = en_param_index;
            break;
        }
    }
    if (en_param_index >= WLAN_BANDWITH_CAP_BUTT) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_process_rate_params::invalid param for bandwidth!}");
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    return OAL_SUCC;
}

OAL_STATIC uint32_t wal_hipriv_process_rate_params_by_dscr_func(oal_net_device_stru *pst_net_dev,
    int8_t *ac_arg, mac_cfg_set_dscr_param_stru *pc_stu, uint8_t ac_arg_len)
{
    uint32_t ul_ret;

    if (g_wlan_spec_cfg->feature_11ax_er_su_dcm_is_open
#ifdef  _PRE_WLAN_FEATURE_11AX_ER_SU_DCM
        && (pc_stu->uc_function_index == ac_arg_len)
#endif
        ) {
        ul_ret = wal_hipriv_process_rate_params_mcs(pst_net_dev, ac_arg, pc_stu);
    } else {
        switch (pc_stu->uc_function_index) {
            case WAL_DSCR_PARAM_RATE:
                ul_ret = wal_hipriv_process_rate_params_rate(pst_net_dev, ac_arg, pc_stu);
                break;
            case WAL_DSCR_PARAM_MCS:
            case WAL_DSCR_PARAM_MCSAC:
            case WAL_DSCR_PARAM_MCSAX:
                ul_ret = wal_hipriv_process_rate_params_mcs(pst_net_dev, ac_arg, pc_stu);
                break;

            case WAL_DSCR_PARAM_NSS:
                ul_ret = wal_hipriv_process_rate_params_nss(ac_arg, pc_stu);
                break;

            case WAL_DSCR_PARAM_BW:
                ul_ret = wal_hipriv_process_rate_params_bw(ac_arg, pc_stu);
                break;

            default:
                oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_process_rate_params::invalid cmd!}");
                return OAL_ERR_CODE_INVALID_CONFIG;
        }
    }

    if (ul_ret != OAL_SUCC) {
        return ul_ret;
    }

    return OAL_SUCC;
}


uint32_t wal_hipriv_process_rate_params(oal_net_device_stru *pst_net_dev,
    int8_t *pc_cmd, mac_cfg_set_dscr_param_stru *pc_stu)
{
    uint32_t ul_ret, ul_off_set;
    int32_t l_val;
    int8_t ac_arg[WAL_HIPRIV_CMD_NAME_MAX_LEN];

    ul_ret = wal_get_cmd_one_arg(pc_cmd, ac_arg, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        return ul_ret;
    }

    /*  配置无效速率值255时恢复自动速率 */
    l_val = oal_strtol(ac_arg, OAL_PTR_NULL, 0);
    if (WAL_IOCTL_IS_INVALID_FIXED_RATE(l_val, pc_stu)) {
        ul_ret = wal_hipriv_set_fix_rate_pre_config(pst_net_dev, OAL_FALSE, pc_stu);
        if (ul_ret != OAL_SUCC) {
            return OAL_FAIL;
        }
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    ul_ret = wal_hipriv_process_rate_params_by_dscr_func(pst_net_dev, ac_arg, pc_stu, WAL_HIPRIV_CMD_NAME_MAX_LEN);
    if (ul_ret != OAL_SUCC) {
        return ul_ret;
    }

    return OAL_SUCC;
}


void wal_msg_queue_init(void)
{
    memset_s((void *)&g_wal_wid_msg_queue, OAL_SIZEOF(g_wal_wid_msg_queue), 0, OAL_SIZEOF(g_wal_wid_msg_queue));
    oal_dlist_init_head(&g_wal_wid_msg_queue.st_head);
    g_wal_wid_msg_queue.count = 0;
    oal_spin_lock_init(&g_wal_wid_msg_queue.st_lock);
    oal_wait_queue_init_head(&g_wal_wid_msg_queue.st_wait_queue);
}

OAL_STATIC void _wal_msg_request_add_queue_(wal_msg_request_stru *pst_msg)
{
    oal_dlist_add_tail(&pst_msg->pst_entry, &g_wal_wid_msg_queue.st_head);
    g_wal_wid_msg_queue.count++;
}


uint32_t wal_get_request_msg_count(void)
{
    return g_wal_wid_msg_queue.count;
}

uint32_t wal_check_and_release_msg_resp(wal_msg_stru *pst_rsp_msg)
{
    wal_msg_write_rsp_stru *pst_write_rsp_msg = OAL_PTR_NULL;
    if (pst_rsp_msg != OAL_PTR_NULL) {
        uint32_t ul_err_code;
        wlan_cfgid_enum_uint16 en_wid;
        pst_write_rsp_msg = (wal_msg_write_rsp_stru *)(pst_rsp_msg->auc_msg_data);
        ul_err_code = pst_write_rsp_msg->ul_err_code;
        en_wid = pst_write_rsp_msg->en_wid;
        oal_free(pst_rsp_msg);

        if (ul_err_code != OAL_SUCC) {
            oam_warning_log2(0, OAM_SF_SCAN,
                "{wal_check_and_release_msg_resp::detect err code:[%u],wid:[%u]}",
                ul_err_code, en_wid);
            return ul_err_code;
        }
    }

    return OAL_SUCC;
}


void wal_msg_request_add_queue(wal_msg_request_stru *pst_msg)
{
#if (_PRE_OS_VERSION_WIN32 == _PRE_OS_VERSION)
    if (g_wal_wid_queue_init_flag == OAL_FALSE) {
        wal_msg_queue_init();
        g_wal_wid_queue_init_flag = OAL_TRUE;
    }
#endif
    oal_spin_lock_bh(&g_wal_wid_msg_queue.st_lock);
    _wal_msg_request_add_queue_(pst_msg);
    oal_spin_unlock_bh(&g_wal_wid_msg_queue.st_lock);
}

OAL_STATIC void _wal_msg_request_remove_queue_(wal_msg_request_stru *pst_msg)
{
    g_wal_wid_msg_queue.count--;
    oal_dlist_delete_entry(&pst_msg->pst_entry);
}


void wal_msg_request_remove_queue(wal_msg_request_stru *pst_msg)
{
    oal_spin_lock_bh(&g_wal_wid_msg_queue.st_lock);
    _wal_msg_request_remove_queue_(pst_msg);
    oal_spin_unlock_bh(&g_wal_wid_msg_queue.st_lock);
}


int32_t wal_set_msg_response_by_addr(uintptr_t addr,
    void *pst_resp_mem, uint32_t ul_resp_ret, uint32_t uc_rsp_len)
{
    int32_t l_ret = -OAL_EINVAL;
    oal_dlist_head_stru *pst_pos = OAL_PTR_NULL;
    oal_dlist_head_stru *pst_entry_temp = OAL_PTR_NULL;
    wal_msg_request_stru *pst_request = NULL;

    oal_spin_lock_bh(&g_wal_wid_msg_queue.st_lock);
    oal_dlist_search_for_each_safe(pst_pos, pst_entry_temp, (&g_wal_wid_msg_queue.st_head))
    {
        pst_request = (wal_msg_request_stru *)oal_dlist_get_entry(pst_pos, wal_msg_request_stru, pst_entry);
        if (pst_request->ul_request_address == addr) {
            /* address match */
            if (oal_unlikely(pst_request->pst_resp_mem != NULL)) {
                oam_error_log0(0, OAM_SF_ANY,
                    "{wal_set_msg_response_by_addr::wal_set_msg_response_by_addr rsp had set!");
            }
            pst_request->pst_resp_mem = pst_resp_mem;
            pst_request->ul_ret = ul_resp_ret;
            pst_request->ul_resp_len = uc_rsp_len;
            l_ret = OAL_SUCC;
            break;
        }
    }

    oal_spin_unlock_bh(&g_wal_wid_msg_queue.st_lock);

    return l_ret;
}


uint32_t wal_alloc_cfg_event(oal_net_device_stru *pst_net_dev,
    frw_event_mem_stru **ppst_event_mem, void *pst_resp_addr,
    wal_msg_stru **ppst_cfg_msg, uint16_t us_len)
{
    mac_vap_stru       *pst_vap       = OAL_PTR_NULL;
    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL;
    frw_event_stru     *pst_event     = OAL_PTR_NULL;
    uint16_t          us_resp_len   = 0;
    wal_msg_rep_hdr    *pst_rep_hdr   = NULL;

    pst_vap = oal_net_dev_priv(pst_net_dev);
    if (oal_unlikely(pst_vap == OAL_PTR_NULL)) {
        /* 规避wifi关闭状态下，下发hipriv命令显示error日志 */
        oam_warning_log1(0, OAM_SF_ANY,
            "{wal_alloc_cfg_event::OAL_NET_DEV_PRIV(pst_net_dev) null! pst_net_dev[%p]}", (uintptr_t)pst_net_dev);
        return OAL_ERR_CODE_PTR_NULL;
    }

    us_resp_len += OAL_SIZEOF(wal_msg_rep_hdr);

    us_len += us_resp_len;

    pst_event_mem = frw_event_alloc_m(us_len);
    if (oal_unlikely(pst_event_mem == OAL_PTR_NULL)) {
        oam_error_log2(pst_vap->uc_vap_id, OAM_SF_ANY,
            "{wal_alloc_cfg_event::pst_event_mem null ptr error,request size:us_len:%d,resp_len:%d}",
            us_len, us_resp_len);
        return OAL_ERR_CODE_PTR_NULL;
    }

    *ppst_event_mem = pst_event_mem; /* 出参赋值 */

    pst_event = frw_get_event_stru(pst_event_mem);

    /* 填写事件头 */
    frw_event_hdr_init(&(pst_event->st_event_hdr), FRW_EVENT_TYPE_HOST_CRX,
                       WAL_HOST_CRX_SUBTYPE_CFG, us_len, FRW_EVENT_PIPELINE_STAGE_0,
                       pst_vap->uc_chip_id, pst_vap->uc_device_id, pst_vap->uc_vap_id);

    /* fill the resp hdr */
    pst_rep_hdr = (wal_msg_rep_hdr *)pst_event->auc_event_data;
    if (pst_resp_addr == NULL) {
        /* no response */
        pst_rep_hdr->ul_request_address = (uintptr_t)0;
    } else {
        /* need response */
        pst_rep_hdr->ul_request_address = (uintptr_t)pst_resp_addr;
    }

    *ppst_cfg_msg = (wal_msg_stru *)((uint8_t *)pst_event->auc_event_data + us_resp_len);

    return OAL_SUCC;
}


OAL_STATIC OAL_INLINE int32_t wal_request_wait_event_condition(wal_msg_request_stru *pst_msg_stru)
{
    int32_t l_ret = OAL_FALSE;
    oal_spin_lock_bh(&g_wal_wid_msg_queue.st_lock);
    if ((pst_msg_stru->pst_resp_mem != NULL) || (pst_msg_stru->ul_ret != OAL_SUCC)) {
        l_ret = OAL_TRUE;
    }
    oal_spin_unlock_bh(&g_wal_wid_msg_queue.st_lock);
    return l_ret;
}

void wal_cfg_msg_task_sched(void)
{
    oal_wait_queue_wake_up(&g_wal_wid_msg_queue.st_wait_queue);
}


int32_t wal_send_cfg_event(oal_net_device_stru *pst_net_dev,
    wal_msg_type_enum_uint8 en_msg_type, uint16_t us_len,
    uint8_t *puc_param, oal_bool_enum_uint8 en_need_rsp, wal_msg_stru **ppst_rsp_msg)
{
    wal_msg_stru       *pst_cfg_msg = OAL_PTR_NULL;
    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL;
    wal_msg_stru       *pst_rsp_msg = OAL_PTR_NULL;
    uint32_t          ul_ret;
    int32_t           l_ret;
#ifdef _PRE_WLAN_FEATURE_SMP_SUPPORT
    mac_vap_stru       *pst_mac_vap;
#endif

    DECLARE_WAL_MSG_REQ_STRU(st_msg_request);
    WAL_MSG_REQ_STRU_INIT(st_msg_request);

    if (ppst_rsp_msg != NULL) {
        *ppst_rsp_msg = NULL;
    }

    if (oal_warn_on((en_need_rsp == OAL_TRUE) && (ppst_rsp_msg == OAL_PTR_NULL))) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_send_cfg_event::OAL_PTR_NULL == ppst_rsp_msg!}");
        return -OAL_EINVAL;
    }

    /* 申请事件 */
    ul_ret = wal_alloc_cfg_event(pst_net_dev, &pst_event_mem, ((en_need_rsp == OAL_TRUE) ? &st_msg_request : NULL),
                                 &pst_cfg_msg, WAL_MSG_WRITE_MSG_HDR_LENGTH + us_len);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_send_cfg_event::wal_alloc_cfg_event err %d!}", ul_ret);
        return -OAL_ENOMEM;
    }

    /* 填写配置消息 */
    WAL_CFG_MSG_HDR_INIT(&(pst_cfg_msg->st_msg_hdr), en_msg_type, us_len, WAL_GET_MSG_SN());

    /* 填写WID消息 */
    if (EOK != memcpy_s(pst_cfg_msg->auc_msg_data, us_len, puc_param, us_len)) {
        oam_error_log0(0, OAM_SF_ANY, "wal_send_cfg_event::memcpy fail!");
        frw_event_free_m(pst_event_mem);
        return -OAL_EINVAL;
    }

    if (en_need_rsp == OAL_TRUE) {
        /* add queue before post event! */
        wal_msg_request_add_queue(&st_msg_request);
    }

    /* 分发事件 */
#ifdef _PRE_WLAN_FEATURE_SMP_SUPPORT
    pst_mac_vap = oal_net_dev_priv(pst_net_dev);
    if (oal_unlikely(pst_mac_vap == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_send_cfg_event::OAL_NET_DEV_PRIV(pst_net_dev) null.}");
        frw_event_free_m(pst_event_mem);
        return -OAL_EINVAL;
    }

    frw_event_post_event(pst_event_mem, pst_mac_vap->ul_core_id);
#else
    frw_event_dispatch_event(pst_event_mem);
#endif
    frw_event_free_m(pst_event_mem);

    /* win32 UT模式，触发一次事件调度 */
#if (_PRE_OS_VERSION_WIN32 == _PRE_OS_VERSION) && (_PRE_TEST_MODE == _PRE_TEST_MODE_UT)
    frw_event_process_all_event(0);
#endif

    if (en_need_rsp == OAL_FALSE) {
        return OAL_SUCC;
    }

    /* context can't in interrupt */
    if (oal_warn_on(oal_in_interrupt())) {
        declare_dft_trace_key_info("wal_cfg_in_interrupt", OAL_DFT_TRACE_EXCEP);
    }

    if (oal_warn_on(oal_in_atomic())) {
        declare_dft_trace_key_info("wal_cfg_in_atomic", OAL_DFT_TRACE_EXCEP);
    }

    /* 等待事件返回 */
    wal_wake_lock();

    /* lint -e730 */ /* lint -e666 */ /* info, boolean argument to function */
    l_ret = oal_wait_event_timeout_m((g_wal_wid_msg_queue.st_wait_queue),
                                   (oal_bool_enum_uint8)(OAL_TRUE == wal_request_wait_event_condition(&st_msg_request)),
                                   (HMAC_WAIT_EVENT_RSP_TIME * OAL_TIME_HZ));
    /* lint +e730 */ /* lint +e666 */
    /* response had been set, remove it from the list */
    if (en_need_rsp == OAL_TRUE) {
        wal_msg_request_remove_queue(&st_msg_request);
    }

    if (oal_warn_on(l_ret == 0)) {
        /* 超时 */
        oam_error_log1(0, OAM_SF_ANY, "{wal_send_cfg_event:: wait queue timeout,%ds!}", HMAC_WAIT_EVENT_RSP_TIME);
        oal_io_print("[E]timeout,request info:%p,ret=%u,addr:0x%lx\n", st_msg_request.pst_resp_mem,
                     st_msg_request.ul_ret,
                     st_msg_request.ul_request_address);
        WAL_MSG_REQ_RESP_MEM_FREE(st_msg_request);
        wal_wake_unlock();
        declare_dft_trace_key_info("wal_send_cfg_timeout", OAL_DFT_TRACE_FAIL);
        /* 打印CFG EVENT内存，方便定位 */
        oal_print_hex_dump((uint8_t *)pst_cfg_msg, (WAL_MSG_WRITE_MSG_HDR_LENGTH + us_len), 32, "cfg event: ");
        /* 上行小包100%跑流，cpu 100%会触发打印，串口打印太多，取消打印 */
#if (_PRE_FRW_FEATURE_PROCCESS_ENTITY_TYPE == _PRE_FRW_FEATURE_PROCCESS_ENTITY_THREAD)
#ifndef CONFIG_HI110X_KERNEL_MODULES_BUILD_SUPPORT
        if (g_st_event_task[0].pst_event_kthread) {
            sched_show_task(g_st_event_task[0].pst_event_kthread);
        }
#endif
#endif
        return -OAL_ETIMEDOUT;
    }
    /*lint +e774*/
    pst_rsp_msg = (wal_msg_stru *)(st_msg_request.pst_resp_mem);
    if (pst_rsp_msg == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_send_cfg_event:: msg mem null!}");
        /*lint -e613*/
        // tscancode-suppress *
        *ppst_rsp_msg = OAL_PTR_NULL;
        /*lint +e613*/
        wal_wake_unlock();
        return -OAL_EFAUL;
    }

    if (pst_rsp_msg->st_msg_hdr.us_msg_len == 0) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_send_cfg_event:: no msg resp!}");
        /*lint -e613*/
        *ppst_rsp_msg = OAL_PTR_NULL;
        /*lint +e613*/
        oal_free(pst_rsp_msg);
        wal_wake_unlock();
        return -OAL_EFAUL;
    }
    /* 发送配置事件返回的状态信息 */
    /*lint -e613*/
    *ppst_rsp_msg = pst_rsp_msg;
    /*lint +e613*/
    wal_wake_unlock();
    return OAL_SUCC;
}



wlan_p2p_mode_enum_uint8 wal_wireless_iftype_to_mac_p2p_mode(enum nl80211_iftype en_iftype)
{
    wlan_p2p_mode_enum_uint8 en_p2p_mode = WLAN_LEGACY_VAP_MODE;

    switch (en_iftype) {
        case NL80211_IFTYPE_P2P_CLIENT:
            en_p2p_mode = WLAN_P2P_CL_MODE;
            break;
        case NL80211_IFTYPE_P2P_GO:
            en_p2p_mode = WLAN_P2P_GO_MODE;
            break;
        case NL80211_IFTYPE_P2P_DEVICE:
            en_p2p_mode = WLAN_P2P_DEV_MODE;
            break;
        case NL80211_IFTYPE_AP:
        case NL80211_IFTYPE_STATION:
            en_p2p_mode = WLAN_LEGACY_VAP_MODE;
            break;
        default:
            en_p2p_mode = WLAN_P2P_BUTT;
    }
    return en_p2p_mode;
}

int32_t wal_cfg_vap_h2d_event(oal_net_device_stru *pst_net_dev)
{
    oal_wireless_dev_stru *pst_wdev = OAL_PTR_NULL;
    mac_wiphy_priv_stru *pst_wiphy_priv = OAL_PTR_NULL;
    hmac_vap_stru *pst_cfg_hmac_vap = OAL_PTR_NULL;
    mac_device_stru *pst_mac_device = OAL_PTR_NULL;
    oal_net_device_stru *pst_cfg_net_dev = OAL_PTR_NULL;

    int32_t l_ret;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;
    uint32_t ul_err_code;
    wal_msg_write_stru st_write_msg;

    pst_wdev = oal_netdevice_wdev(pst_net_dev);
    if (pst_wdev == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_cfg_vap_h2d_event::wdev is null!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_wiphy_priv = (mac_wiphy_priv_stru *)oal_wiphy_priv(pst_wdev->wiphy);
    if (pst_wiphy_priv == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_cfg_vap_h2d_event::wiphy_priv is null!}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    pst_mac_device = pst_wiphy_priv->pst_mac_device;
    if (pst_mac_device == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_cfg_vap_h2d_event::mac_device is null!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_cfg_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_device->uc_cfg_vap_id);
    if (pst_cfg_hmac_vap == OAL_PTR_NULL) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_cfg_vap_h2d_event::mac_res_get_hmac_vap fail.vap_id[%u]}",
            pst_mac_device->uc_cfg_vap_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_cfg_net_dev = pst_cfg_hmac_vap->pst_net_device;
    if (pst_cfg_net_dev == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_cfg_vap_h2d_event::pst_cfg_net_dev is null!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 抛事件到wal层处理 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_CFG_VAP_H2D, OAL_SIZEOF(mac_cfg_vap_h2d_stru));
    ((mac_cfg_vap_h2d_stru *)st_write_msg.auc_value)->pst_net_dev = pst_cfg_net_dev;

    /* 发送消息 */
    l_ret = wal_send_cfg_event(pst_cfg_net_dev, WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_vap_h2d_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_TRUE, &pst_rsp_msg);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_cfg_vap_h2d_event::wal_alloc_cfg_event err %d!}", l_ret);
        return l_ret;
    }

    /* 处理返回消息 */
    ul_err_code = wal_check_and_release_msg_resp(pst_rsp_msg);
    if (ul_err_code != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_cfg_vap_h2d_event::hmac cfg vap h2d fail,err code[%u]", ul_err_code);
        return -OAL_EINVAL;
    }

    return OAL_SUCC;
}

OAL_STATIC int32_t wal_host_dev_config(oal_net_device_stru *pst_net_dev, wlan_cfgid_enum_uint16 en_wid)
{
    oal_wireless_dev_stru *pst_wdev = OAL_PTR_NULL;
    mac_wiphy_priv_stru *pst_wiphy_priv = OAL_PTR_NULL;
    hmac_vap_stru *pst_cfg_hmac_vap = OAL_PTR_NULL;
    mac_device_stru *pst_mac_device = OAL_PTR_NULL;
    oal_net_device_stru *pst_cfg_net_dev = OAL_PTR_NULL;

    int32_t l_ret;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;
    uint32_t ul_err_code;
    wal_msg_write_stru st_write_msg;

    pst_wdev = oal_netdevice_wdev(pst_net_dev);
    if (pst_wdev == NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_host_dev_config::pst_wdev is null!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_wiphy_priv = (mac_wiphy_priv_stru *)oal_wiphy_priv(pst_wdev->wiphy);
    if (pst_wiphy_priv == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_host_dev_config::pst_mac_device is null!}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    pst_mac_device = pst_wiphy_priv->pst_mac_device;
    if (pst_mac_device == NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_host_dev_config::pst_mac_device is null!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_cfg_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_device->uc_cfg_vap_id);
    if (pst_cfg_hmac_vap == NULL) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_host_dev_config::pst_cfg_hmac_vap is null vap_id:%d!}",
            pst_mac_device->uc_cfg_vap_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_cfg_net_dev = pst_cfg_hmac_vap->pst_net_device;
    if (pst_cfg_net_dev == NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_host_dev_config::pst_cfg_net_dev is null!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 抛事件到wal层处理 */
    /* 填写消息 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, en_wid, 0);

    /* 发送消息 */
    l_ret = wal_send_cfg_event(pst_cfg_net_dev, WAL_MSG_TYPE_WRITE, WAL_MSG_WRITE_MSG_HDR_LENGTH,
                               (uint8_t *)&st_write_msg, OAL_TRUE, &pst_rsp_msg);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_host_dev_config::wal_alloc_cfg_event err %d!}", l_ret);
        return l_ret;
    }

    /* 处理返回消息 */
    ul_err_code = wal_check_and_release_msg_resp(pst_rsp_msg);
    if (ul_err_code != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_host_dev_config::hmac cfg vap h2d fail,err code[%u]", ul_err_code);
        return -OAL_EINVAL;
    }

    return OAL_SUCC;
}


int32_t wal_host_dev_init(oal_net_device_stru *pst_net_dev)
{
    return wal_host_dev_config(pst_net_dev, WLAN_CFGID_HOST_DEV_INIT);
}


int32_t wal_host_dev_exit(oal_net_device_stru *pst_net_dev)
{
    return wal_host_dev_config(pst_net_dev, WLAN_CFGID_HOST_DEV_EXIT);
}


int32_t wal_set_power_on(oal_net_device_stru *pst_net_dev, int32_t power_flag)
{
    int32_t l_ret = 0;

    // ap上下电，配置VAP
    if (power_flag == 0) { // 下电
        /* 下电host device_stru去初始化 */
        wal_host_dev_exit(pst_net_dev);

        wal_wake_lock();
        wlan_pm_close();
        wal_wake_unlock();

        g_st_ap_config_info.l_ap_power_flag = OAL_FALSE;
    } else if (power_flag == 1) {  // 上电
        g_st_ap_config_info.l_ap_power_flag = OAL_TRUE;

        wal_wake_lock();
        l_ret = wlan_pm_open();
        wal_wake_unlock();
        if (l_ret == OAL_FAIL) {
            oam_error_log0(0, OAM_SF_ANY, "{wal_set_power_on::wlan_pm_open Fail!}");
            return -OAL_EFAIL;
        } else if (l_ret != OAL_ERR_CODE_ALREADY_OPEN) {
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
            /* 重新上电时置为FALSE */
            hwifi_config_init_force();
#endif
            // 重新上电场景，下发配置VAP
            l_ret = wal_cfg_vap_h2d_event(pst_net_dev);
            if (l_ret != OAL_SUCC) {
                return -OAL_EFAIL;
            }
        }

        /* 上电host device_stru初始化 */
        l_ret = wal_host_dev_init(pst_net_dev);
        if (l_ret != OAL_SUCC) {
            oal_io_print("wal_set_power_on FAIL %d \r\n", l_ret);
            return -OAL_EFAIL;
        }
    } else {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_set_power_on::pupower_flag:%d error.}", power_flag);
        return -OAL_EFAIL;
    }

    return OAL_SUCC;
}


void wal_set_power_mgmt_on(uint8_t power_mgmt_flag)
{
    struct wlan_pm_s *pst_wlan_pm;
    pst_wlan_pm = wlan_pm_get_drv();
    if (pst_wlan_pm != NULL) {
        /* ap模式下，是否允许下电操作,1:允许,0:不允许 */
        pst_wlan_pm->ul_apmode_allow_pm_flag = power_mgmt_flag;
    } else {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_set_power_mgmt_on::wlan_pm_get_drv return null.");
    }
}

OAL_STATIC uint32_t wal_hipriv_set_p2p_scenes(oal_net_device_stru *p_net_dev, int8_t *p_param)
{
    int32_t ret;
    uint8_t value;
    uint32_t offSet;
    int8_t valueStr[WAL_HIPRIV_CMD_NAME_MAX_LEN] = {0};
    wal_msg_write_stru writeMsg;

    ret = wal_get_cmd_one_arg(p_param, valueStr, WAL_HIPRIV_CMD_NAME_MAX_LEN, &offSet);
    if (ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_p2p_scenes::wal_get_cmd_one_arg vap name err %d!}", ret);
        return ret;
    }

    value = (oal_uint8)oal_atoi(valueStr);

    WAL_WRITE_MSG_HDR_INIT(&writeMsg, WALN_CFGID_SET_P2P_SCENES, OAL_SIZEOF(uint8_t));
    *(uint8_t *)(writeMsg.auc_value) = value;

    ret = wal_send_cfg_event(p_net_dev, WAL_MSG_TYPE_WRITE, WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(uint8_t),
        (oal_uint8 *)&writeMsg, OAL_FALSE, OAL_PTR_NULL);
    if (oal_unlikely(ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_M2S, "{wal_hipriv_set_p2p_scenes::returnerr code = [%d].}\r\n", ret);
        return (uint32_t)ret;
    }

    return OAL_SUCC;
}
#ifdef _PRE_WLAN_RR_PERFORMANCE

OAL_STATIC int32_t wal_ext_priv_cmd_dev_freq_check_para(oal_net_device_stru *pst_net_dev)
{
    if (oal_any_null_ptr1(pst_net_dev)) {
        oam_error_log1(0, OAM_SF_CFG, "wal_ext_priv_cmd_dev_freq_check_para::null_ptr. net_dev %p",
                       (uintptr_t)pst_net_dev);
        return -OAL_EFAIL;
    }

    return OAL_SUCC;
}


OAL_STATIC int32_t wal_priv_cmd_dev_freq_para(mac_vap_stru *mac_vap, mac_cfg_set_tlv_stru *pst_config_para,
                                                    uint8_t uc_cmd_type, uint8_t ul_value)
{
    int32_t     result;
    pst_config_para->uc_cmd_type = uc_cmd_type;
    pst_config_para->ul_value    = ul_value;
    pst_config_para->uc_len      = OAL_SIZEOF(mac_cfg_set_tlv_stru);
    pst_config_para->us_cfg_id   = WLAN_CFGID_SET_DEVICE_FREQ;

    result = hmac_config_set_tlv_cmd(mac_vap, 0, (uint8_t *)pst_config_para);
    if (result != OAL_SUCC) {
        oam_error_log2(0, OAM_SF_CFG, "hmac_config_set_tlv_cmd[%d,%d] fail!\n", uc_cmd_type, ul_value);
        return result;
    }

    oam_warning_log2(0, OAM_SF_CFG, "hmac_config_set_tlv_cmd[%d,%d] succ!\n", uc_cmd_type, ul_value);
    return OAL_SUCC;
}


OAL_STATIC int32_t wal_ext_priv_cmd_set_dev_freq(oal_net_device_stru *pst_net_dev, uint8_t uc_flag)
{
    mac_cfg_set_tlv_stru *pst_config_para = OAL_PTR_NULL;
    mac_vap_stru *mac_vap = OAL_PTR_NULL;
    int32_t result;

    result = wal_ext_priv_cmd_dev_freq_check_para(pst_net_dev);
    if (result != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_CFG, "wal_ext_priv_cmd_set_dev_freq::wal_ext_priv_cmd_dev_freq_check_para fail!\n");
        return result;
    }
    mac_vap = oal_net_dev_priv(pst_net_dev);
    if (mac_vap == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_CFG, "{wal_ext_priv_cmd_set_dev_freq::vap is null.}");
        return -OAL_EINVAL;
    }

    /* 申请内存 */
    pst_config_para = (mac_cfg_set_tlv_stru *)oal_memalloc(OAL_SIZEOF(mac_cfg_set_tlv_stru));
    if (oal_unlikely(pst_config_para == NULL)) {
        oam_error_log0(0, OAM_SF_CFG, "wal_ext_priv_cmd_set_dev_freq::pst_config_para malloc fail!\n");
        return -OAL_EFAIL;
    }
    memset_s((void *)pst_config_para, OAL_SIZEOF(mac_cfg_set_tlv_stru), 0, OAL_SIZEOF(mac_cfg_set_tlv_stru));

    if (uc_flag == FREQ_AUTO_FLAG) {
        result = wal_priv_cmd_dev_freq_para(mac_vap, pst_config_para, CMD_SET_AUTO_FREQ_ENDABLE, FREQ_LOCK_ENABLE);
        if (result != OAL_SUCC) {
            oal_free(pst_config_para);
            pst_config_para = NULL;
            oam_error_log0(0, OAM_SF_CFG, "wal_ext_priv_cmd_set_dev_freq::wal_ext_priv_cmd_fill_para fail!\n");
            return result;
        }
    } else if (uc_flag == FREQ_MAX_FLAG) {
        result = wal_priv_cmd_dev_freq_para(mac_vap, pst_config_para, CMD_SET_AUTO_FREQ_ENDABLE, FREQ_LOCK_DISABLE);
        if (result != OAL_SUCC) {
            oal_free(pst_config_para);
            pst_config_para = NULL;
            oam_error_log0(0, OAM_SF_CFG, "wal_ext_priv_cmd_set_dev_freq::wal_ext_priv_cmd_fill_para fail!\n");
            return result;
        }

        result = wal_priv_cmd_dev_freq_para(mac_vap, pst_config_para, CMD_SET_DEVICE_FREQ_VALUE, FREQ_MAX_VALUE);
        if (result != OAL_SUCC) {
            oal_free(pst_config_para);
            pst_config_para = NULL;
            oam_error_log0(0, OAM_SF_CFG, "wal_ext_priv_cmd_set_dev_freq::wal_ext_priv_cmd_fill_para fail!\n");
            return result;
        }
    }

    oal_free(pst_config_para);
    pst_config_para = NULL;
    oam_warning_log0(0, OAM_SF_CFG, "wal_ext_priv_cmd_set_dev_freq::success!\n");
    return OAL_SUCC;
}


OAL_STATIC int32_t wal_ext_priv_cmd_pcie_aspm(int32_t aspm)
{
    return hi110x_hcc_ip_pm_ctrl(aspm);
}


int32_t wal_ext_priv_cmd(oal_net_device_stru *pst_net_dev, oal_ifreq_stru *pst_ifr)
{
    wal_wifi_pc_cmd_stru  st_priv_cmd;
    int32_t             l_ret = OAL_SUCC;

    if (oal_any_null_ptr2(pst_ifr->ifr_data,pst_net_dev)) {
        l_ret = -OAL_EINVAL;
        return l_ret;
    }
    /*将用户态数据拷贝到内核态*/
    if (oal_copy_from_user(&st_priv_cmd, pst_ifr->ifr_data, sizeof(wal_wifi_pc_cmd_stru))) {
        l_ret = -OAL_EINVAL;
        return l_ret;
    }

    switch (st_priv_cmd.ul_cmd) {
        case WAL_EXT_PRI_CMD_SET_DEVICE_FREQ_MAX:
             l_ret = wal_ext_priv_cmd_set_dev_freq(pst_net_dev, FREQ_MAX_FLAG);
             break;
        case WAL_EXT_PRI_CMD_SET_DEVICE_FREQ_AUTO:
             l_ret = wal_ext_priv_cmd_set_dev_freq(pst_net_dev, FREQ_AUTO_FLAG);
             break;
        case WAL_EXT_PRI_CMD_ENBLE_PCIE_ASPM:
             l_ret = wal_ext_priv_cmd_pcie_aspm(ASPM_ENABLE);
             oam_warning_log2(0, OAM_SF_CFG, "wal_ext_priv_cmd_pcie_aspm::cmd = %d, l_ret = %d\n", ASPM_ENABLE, l_ret);
             break;
        case WAL_EXT_PRI_CMD_DISABLE_PCIE_ASPM:
             l_ret = wal_ext_priv_cmd_pcie_aspm(ASPM_DISABLE);
             oam_warning_log2(0, OAM_SF_CFG, "wal_ext_priv_cmd_pcie_aspm::cmd = %d, l_ret = %d\n", ASPM_DISABLE, l_ret);
             break;
        default:
             break;
    }
    chr_exception_p(CHR_WIFI_IWARE_RR_EVENTID, (uint8_t *)&st_priv_cmd.ul_cmd, sizeof(uint32_t));
    return l_ret;
}

#endif /* _PRE_WLAN_RR_PERFORMANCE */


OAL_STATIC uint32_t wal_hipriv_set_mode_config(oal_net_device_stru *net_dev, uint8_t uc_prot_idx)
{
    mac_cfg_mode_param_stru *pst_mode_param = NULL;
    wal_msg_stru *rsp_msg = NULL;
    wal_msg_write_stru write_msg;
    int32_t ret;

    /* 抛事件到wal层处理 */
    WAL_WRITE_MSG_HDR_INIT(&write_msg, WLAN_CFGID_MODE, OAL_SIZEOF(mac_cfg_mode_param_stru));

    pst_mode_param = (mac_cfg_mode_param_stru *)(write_msg.auc_value);
    pst_mode_param->en_protocol = g_ast_mode_map[uc_prot_idx].en_mode;
    pst_mode_param->en_band = g_ast_mode_map[uc_prot_idx].en_band;
    pst_mode_param->en_bandwidth = g_ast_mode_map[uc_prot_idx].en_bandwidth;

    oam_warning_log3(0, OAM_SF_CFG, "{wal_hipriv_set_mode_config::protocol[%d],band[%d],bandwidth[%d]!}",
                     pst_mode_param->en_protocol, pst_mode_param->en_band, pst_mode_param->en_bandwidth);

    /* 发送消息 */
    ret = wal_send_cfg_event(net_dev, WAL_MSG_TYPE_WRITE,
                             WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_mode_param_stru),
                             (uint8_t *)&write_msg, OAL_TRUE, &rsp_msg);

    if (oal_unlikely(ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_mode_config::wal_alloc_cfg_event err %d!}", ret);
        return (uint32_t)ret;
    }

    /* 读取返回的错误码 */
    ret = wal_check_and_release_msg_resp(rsp_msg);
    if (ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_DFR, "{wal_hipriv_set_mode_config fail, err code[%u]!}", ret);
        return ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_set_mode(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    return wal_ioctl_set_mode(pst_net_dev, pc_param);
}

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 10, 44))

uint32_t wal_hipriv_set_essid(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    uint8_t uc_ssid_len;
    int32_t l_ret;
    wal_msg_write_stru st_write_msg;
    mac_cfg_ssid_param_stru *pst_param;
    mac_vap_stru *pst_mac_vap;
    uint32_t ul_off_set;
    int8_t *pc_ssid;
    int8_t ac_ssid[WLAN_SSID_MAX_LEN] = { 0 };
    uint32_t ul_ret;

    pst_mac_vap = oal_net_dev_priv(pst_net_dev);
    if (pst_mac_vap == NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_set_essid::pst_mac_vap is null!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_AP) {
        /* 设备在up状态且是AP时，不允许配置，必须先down */
        if (0 != (OAL_IFF_RUNNING & oal_netdevice_flags(pst_net_dev))) {
            oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                "{wal_hipriv_set_essid::dev  busy, please down it first:%d}", oal_netdevice_flags(pst_net_dev));
            return -OAL_EBUSY;
        }
    }

    /* pc_param指向传入模式参数, 将其取出存放到ac_mode_str中 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_ssid, WLAN_SSID_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_essid::wal_get_cmd_one_arg vap name err %d}", ul_ret);
        return ul_ret;
    }

    pc_ssid = ac_ssid;
    pc_ssid = oal_strim(ac_ssid); /* 去掉字符串开始结尾的空格 */
    uc_ssid_len = (uint8_t)OAL_STRLEN(pc_ssid);

    oam_info_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_hipriv_set_essid:: ssid length %d!}", uc_ssid_len);

    if (uc_ssid_len > WLAN_SSID_MAX_LEN - 1) { /* -1为\0预留空间 */
        uc_ssid_len = WLAN_SSID_MAX_LEN - 1;
    }

    oam_info_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_hipriv_set_essid:: ssid length is %d!}", uc_ssid_len);
    /* 抛事件到wal层处理 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SSID, OAL_SIZEOF(mac_cfg_ssid_param_stru));

    /* 填写WID对应的参数 */
    pst_param = (mac_cfg_ssid_param_stru *)(st_write_msg.auc_value);
    pst_param->uc_ssid_len = uc_ssid_len;
    if (EOK != memcpy_s(pst_param->ac_ssid, OAL_SIZEOF(pst_param->ac_ssid), pc_ssid, uc_ssid_len)) {
        oam_error_log0(0, OAM_SF_ANY, "wal_hipriv_set_essid::memcpy fail!");
        return OAL_FAIL;
    }

    /* 发送消息 */
    l_ret = wal_send_cfg_event(pst_net_dev, WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_ssid_param_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE, OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(pst_mac_vap->uc_vap_id,
            OAM_SF_ANY, "{wal_hipriv_set_essid::alloc_cfg_event err:%d}", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}
#endif
/*
 * 函 数 名  : wal_octl_get_essid
 * 功能描述  : 获取ssid
 */
OAL_STATIC int wal_ioctl_get_essid(oal_net_device_stru *pst_net_dev, oal_iw_request_info_stru *pst_info,
                                   oal_iwreq_data_union *pst_data, char *pc_ssid)
{
    int32_t l_ret;
    wal_msg_query_stru st_query_msg;
    mac_cfg_ssid_param_stru *pst_ssid = OAL_PTR_NULL;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;
    wal_msg_rsp_stru *pst_query_rsp_msg = OAL_PTR_NULL;
    oal_iw_point_stru *pst_essid = (oal_iw_point_stru *)pst_data;

    /* 抛事件到wal层处理 */
    st_query_msg.en_wid = WLAN_CFGID_SSID;

    /* 发送消息 */
    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_QUERY,
                                   WAL_MSG_WID_LENGTH,
                                   (uint8_t *)&st_query_msg,
                                   OAL_TRUE,
                                   &pst_rsp_msg);

    if (l_ret != OAL_SUCC || pst_rsp_msg == OAL_PTR_NULL) {
        if (pst_rsp_msg != OAL_PTR_NULL) {
            oal_free(pst_rsp_msg);
        }
        oam_warning_log1(0, OAM_SF_ANY, "{wal_ioctl_get_essid:: wal_send_cfg_event err %d!}", l_ret);
        return -OAL_EFAIL;
    }

    /* 处理返回消息 */
    pst_query_rsp_msg = (wal_msg_rsp_stru *)(pst_rsp_msg->auc_msg_data);

    /* 业务处理 */
    pst_ssid = (mac_cfg_ssid_param_stru *)(pst_query_rsp_msg->auc_value);
    pst_essid->flags = 1; /* 设置出参标志为有效 */
    pst_essid->length = oal_min(pst_ssid->uc_ssid_len, OAL_IEEE80211_MAX_SSID_LEN);
    if (EOK != memcpy_s(pc_ssid, pst_essid->length, pst_ssid->ac_ssid, pst_essid->length)) {
        oam_warning_log1(0, OAM_SF_ANY, "wal_ioctl_get_essid::memcpy fail! pst_essid->length[%d]", pst_essid->length);
        oal_free(pst_rsp_msg);
        return -OAL_EINVAL;
    }

    oal_free(pst_rsp_msg);
    return OAL_SUCC;
}


OAL_STATIC int wal_ioctl_get_apaddr(oal_net_device_stru *pst_net_dev,
                                    oal_iw_request_info_stru *pst_info,
                                    oal_iwreq_data_union *pst_wrqu,
                                    char *pc_extra)
{
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    oal_sockaddr_stru *pst_addr = (oal_sockaddr_stru *)pst_wrqu;
    uint8_t auc_zero_addr[WLAN_MAC_ADDR_LEN] = { 0 };

    if ((pst_net_dev == OAL_PTR_NULL) || (pst_addr == OAL_PTR_NULL)) {
        oam_error_log2(0, OAM_SF_ANY, "{wal_ioctl_get_apaddr::param null, pst_net_dev = %p, pst_addr = %p.}",
                       (uintptr_t)pst_net_dev, (uintptr_t)pst_addr);
        return -OAL_EINVAL;
    }

    pst_mac_vap = oal_net_dev_priv(pst_net_dev);
    if (pst_mac_vap == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_ioctl_get_apaddr::pst_mac_vap is null!}");
        return -OAL_EFAUL;
    }

    if (pst_mac_vap->en_vap_state == MAC_VAP_STATE_UP) {
        oal_set_mac_addr((uint8_t *)pst_addr->sa_data, pst_mac_vap->auc_bssid);
    } else {
        oal_set_mac_addr((uint8_t *)pst_addr->sa_data, auc_zero_addr);
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_set_freq(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    return wal_ioctl_set_freq(pst_net_dev, pc_param);
}

#ifdef _PRE_WLAN_FEATURE_11D

OAL_STATIC oal_bool_enum_uint8 wal_is_alpha_upper(int8_t c_letter)
{
    if (c_letter >= 'A' && c_letter <= 'Z') {
        return OAL_TRUE;
    }

    return OAL_FALSE;
}


uint8_t wal_regdomain_get_band(uint32_t ul_start_freq, uint32_t ul_end_freq)
{
    if (ul_start_freq > 2400 && ul_end_freq < 2500) {
        return MAC_RC_START_FREQ_2;
    } else if (ul_start_freq > 5000 && ul_end_freq < 5870) {
        return MAC_RC_START_FREQ_5;
    } else if (ul_start_freq > 4900 && ul_end_freq < 4999) {
        return MAC_RC_START_FREQ_5;
    } else {
        return MAC_RC_START_FREQ_BUTT;
    }
}


uint8_t wal_regdomain_get_bw(uint8_t uc_bw)
{
    uint8_t uc_bw_map;

    switch (uc_bw) {
        case 80:
            uc_bw_map = MAC_CH_SPACING_80MHZ;
            break;
        case 40:
            uc_bw_map = MAC_CH_SPACING_40MHZ;
            break;
        case 20:
            uc_bw_map = MAC_CH_SPACING_20MHZ;
            break;
        default:
            uc_bw_map = MAC_CH_SPACING_BUTT;
            break;
    };

    return uc_bw_map;
}


uint32_t wal_regdomain_get_channel_2g(uint32_t ul_start_freq, uint32_t ul_end_freq)
{
    uint32_t ul_freq;
    uint32_t ul_i;
    uint32_t ul_ch_bmap = 0;

    for (ul_freq = ul_start_freq + 10; ul_freq <= (ul_end_freq - 10); ul_freq++) {
        for (ul_i = 0; ul_i < MAC_CHANNEL_FREQ_2_BUTT; ul_i++) {
            if (ul_freq == g_ast_freq_map_2g[ul_i].us_freq) {
                ul_ch_bmap |= (1 << ul_i);
            }
        }
    }

    return ul_ch_bmap;
}


uint32_t wal_regdomain_get_channel_5g(uint32_t ul_start_freq, uint32_t ul_end_freq)
{
    uint32_t ul_freq;
    uint32_t ul_i;
    uint32_t ul_ch_bmap = 0;

    for (ul_freq = ul_start_freq + 10; ul_freq <= (ul_end_freq - 10); ul_freq += 5) {
        for (ul_i = 0; ul_i < MAC_CHANNEL_FREQ_5_BUTT; ul_i++) {
            if (ul_freq == g_ast_freq_map_5g[ul_i].us_freq) {
                ul_ch_bmap |= (1 << ul_i);
            }
        }
    }

    return ul_ch_bmap;
}


uint32_t wal_regdomain_get_channel(uint8_t uc_band,
    uint32_t ul_start_freq, uint32_t ul_end_freq)
{
    uint32_t ul_ch_bmap = 0;
    ;

    switch (uc_band) {
        case MAC_RC_START_FREQ_2:
            ul_ch_bmap = wal_regdomain_get_channel_2g(ul_start_freq, ul_end_freq);
            break;

        case MAC_RC_START_FREQ_5:
            ul_ch_bmap = wal_regdomain_get_channel_5g(ul_start_freq, ul_end_freq);
            break;

        default:
            break;
    }

    return ul_ch_bmap;
}

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 3, 0))
extern oal_ieee80211_supported_band g_st_supported_band_2ghz_info;
uint32_t wal_linux_update_wiphy_channel_list_num(oal_net_device_stru *pst_net_dev,
    oal_wiphy_stru *pst_wiphy)
{
    uint16_t us_len;
    uint32_t ul_ret;
    mac_vendor_cmd_channel_list_stru st_channel_list;
    mac_vap_stru *pst_mac_vap;

    if (oal_any_null_ptr2(pst_wiphy, pst_net_dev)) {
        oam_error_log2(0, OAM_SF_ANY, "{wal_linux_update_wiphy_channel_list_num::wiphy %p, net_dev %p}",
            (uintptr_t)pst_wiphy, (uintptr_t)pst_net_dev);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_mac_vap = oal_net_dev_priv(pst_net_dev);
    if (pst_mac_vap == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_linux_update_wiphy_channel_list_num::NET_DEV_PRIV is NULL.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    ul_ret = hmac_config_vendor_cmd_get_channel_list(pst_mac_vap, &us_len, (uint8_t *)(&st_channel_list));
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_linux_update_wiphy_channel_list_num::get chan list fail:%d}", ul_ret);
        return ul_ret;
    }

    /* 只更新2G信道个数，5G信道由于存在DFS 区域，且带宽计算并无问题,不需要修改 */
    g_st_supported_band_2ghz_info.n_channels = st_channel_list.uc_channel_num_2g;

    oam_warning_log2(0, OAM_SF_ANY, "{wal_linux_update_wiphy_channel_list_num::2g_chan_num:%d, 5g_chan_num:%d}",
                     st_channel_list.uc_channel_num_2g,
                     st_channel_list.uc_channel_num_5g);
    return OAL_SUCC;
}
#endif


OAL_STATIC OAL_INLINE void wal_get_dfs_domain(mac_regdomain_info_stru *pst_mac_regdom,
    OAL_CONST int8_t *pc_country)
{
    uint32_t u_idx;
    uint32_t ul_size = oal_array_size(g_ast_dfs_domain_table);

    for (u_idx = 0; u_idx < ul_size; u_idx++) {
        if (0 == oal_strcmp(g_ast_dfs_domain_table[u_idx].pc_country, pc_country)) {
            pst_mac_regdom->en_dfs_domain = g_ast_dfs_domain_table[u_idx].en_dfs_domain;

            return;
        }
    }

    pst_mac_regdom->en_dfs_domain = MAC_DFS_DOMAIN_NULL;
}

OAL_STATIC void wal_regdomain_fill_info(OAL_CONST oal_ieee80211_regdomain_stru *pst_regdom,
    mac_regdomain_info_stru *pst_mac_regdom)
{
    uint32_t ul_i;
    uint32_t ul_start;
    uint32_t ul_end;
    uint8_t uc_band;
    uint8_t uc_bw;

    /* 复制国家字符串 */
    pst_mac_regdom->ac_country[0] = pst_regdom->alpha2[0];
    pst_mac_regdom->ac_country[1] = pst_regdom->alpha2[1];
    pst_mac_regdom->ac_country[2] = 0;

    /* 获取DFS认证标准类型 */
    wal_get_dfs_domain(pst_mac_regdom, pst_regdom->alpha2);

    /* 填充管制类个数 */
    pst_mac_regdom->uc_regclass_num = (uint8_t)pst_regdom->n_reg_rules;

    /* 填充管制类信息 */
    for (ul_i = 0; ul_i < pst_regdom->n_reg_rules; ul_i++) {
        /* 填写管制类的频段(2.4G或5G) */
        ul_start = pst_regdom->reg_rules[ul_i].freq_range.start_freq_khz / 1000;
        ul_end = pst_regdom->reg_rules[ul_i].freq_range.end_freq_khz / 1000;
        uc_band = wal_regdomain_get_band(ul_start, ul_end);
        pst_mac_regdom->ast_regclass[ul_i].en_start_freq = uc_band;

        /* 填写管制类允许的最大带宽 */
        uc_bw = (uint8_t)(pst_regdom->reg_rules[ul_i].freq_range.max_bandwidth_khz / 1000);
        pst_mac_regdom->ast_regclass[ul_i].en_ch_spacing = wal_regdomain_get_bw(uc_bw);

        /* 填写管制类信道位图 */
        pst_mac_regdom->ast_regclass[ul_i].ul_channel_bmap = wal_regdomain_get_channel(uc_band, ul_start, ul_end);

        /* 标记管制类行为 */
        pst_mac_regdom->ast_regclass[ul_i].uc_behaviour_bmap = 0;

        if (pst_regdom->reg_rules[ul_i].flags & NL80211_RRF_DFS) {
            pst_mac_regdom->ast_regclass[ul_i].uc_behaviour_bmap |= MAC_RC_DFS;
        }

        if (pst_regdom->reg_rules[ul_i].flags & NL80211_RRF_NO_INDOOR) {
            pst_mac_regdom->ast_regclass[ul_i].uc_behaviour_bmap |= MAC_RC_NO_INDOOR;
        }

        if (pst_regdom->reg_rules[ul_i].flags & NL80211_RRF_NO_OUTDOOR) {
            pst_mac_regdom->ast_regclass[ul_i].uc_behaviour_bmap |= MAC_RC_NO_OUTDOOR;
        }
        /* 填充覆盖类和最大发送功率 */
        pst_mac_regdom->ast_regclass[ul_i].uc_coverage_class = 0;
        pst_mac_regdom->ast_regclass[ul_i].uc_max_reg_tx_pwr =
            (uint8_t)(pst_regdom->reg_rules[ul_i].power_rule.max_eirp / 100);
        pst_mac_regdom->ast_regclass[ul_i].us_max_tx_pwr =
            (uint16_t)(pst_regdom->reg_rules[ul_i].power_rule.max_eirp / 10);
    }
}


int32_t wal_regdomain_update(oal_net_device_stru *pst_net_dev, int8_t *pc_country)
{

    uint8_t uc_dev_id;
    mac_device_stru *pst_device = OAL_PTR_NULL;

    OAL_CONST oal_ieee80211_regdomain_stru *pst_regdom;
    uint16_t us_size;
    mac_regdomain_info_stru *pst_mac_regdom = OAL_PTR_NULL;
    wal_msg_write_stru st_write_msg;
    mac_cfg_country_stru *pst_param = OAL_PTR_NULL;
    int32_t l_ret;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
    int8_t old_pc_country[COUNTRY_CODE_LEN] = { '9', '9' };
#endif

    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;

#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
    if (EOK != memcpy_s(old_pc_country, COUNTRY_CODE_LEN, hwifi_get_country_code(), COUNTRY_CODE_LEN)) {
        oam_error_log0(0, OAM_SF_ANY, "wal_regdomain_update::memcpy fail!");
        return -OAL_EINVAL;
    }
    hwifi_set_country_code(pc_country, COUNTRY_CODE_LEN);
    /* 如果新的国家码和旧国家处于一个regdomain，不刷新RF参数，只更新国家码 */
    if (OAL_TRUE == hwifi_is_regdomain_changed((uint8_t *)old_pc_country, (uint8_t *)pc_country)) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_regdomain_update::regdomain changed, refresh rf param!}");

        /* 刷新参数失败，为了保证国家码和功率参数对应 */
        /* 将国家码设回原来的国家码，本次更新失败 */
        if (hwifi_force_refresh_rf_params(pst_net_dev) != OAL_SUCC) {
            oam_warning_log0(0, OAM_SF_ANY,
                "{wal_regdomain_update::refresh rf(max_txpwr & dbb scale) params failed. Set country back.!}");
            hwifi_set_country_code(old_pc_country, COUNTRY_CODE_LEN);
        }
    }
#endif

    if (!wal_is_alpha_upper(pc_country[0]) || !wal_is_alpha_upper(pc_country[1])) {
        if ((pc_country[0] == '9') && (pc_country[1] == '9')) {
            oam_info_log0(0, OAM_SF_ANY, "{wal_regdomain_update::set regdomain to 99!}");
        } else {
            oam_warning_log0(0, OAM_SF_ANY, "{wal_regdomain_update::country str invalid!}");
            return -OAL_EINVAL;
        }
    }

    pst_regdom = wal_regdb_find_db(pc_country);
    if (pst_regdom == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_regdomain_update::no regdomain db was found!}");
        return -OAL_EINVAL;
    }

    us_size = (uint16_t)(OAL_SIZEOF(mac_regclass_info_stru) * pst_regdom->n_reg_rules + MAC_RD_INFO_LEN);

    /* 申请内存存放管制域信息，将内存指针作为事件payload抛下去 */
    /* 此处申请的内存在事件处理函数释放(hmac_config_set_country) */
    pst_mac_regdom = oal_mem_alloc_m(OAL_MEM_POOL_ID_LOCAL, us_size, OAL_TRUE);
    if (pst_mac_regdom == OAL_PTR_NULL) {
        oam_error_log1(0, OAM_SF_ANY, "{wal_regdomain_update::alloc regdom mem fail(%d),null ptr!}", us_size);
        return -OAL_ENOMEM;
    }

    wal_regdomain_fill_info(pst_regdom, pst_mac_regdom);

    /* 抛事件到wal层处理 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_COUNTRY, OAL_SIZEOF(mac_cfg_country_stru));

    /* 填写WID对应的参数 */
    pst_mac_regdom->en_regdomain = hwifi_get_regdomain_from_country_code(pc_country);
    pst_param = (mac_cfg_country_stru *)(st_write_msg.auc_value);
    pst_param->p_mac_regdom = pst_mac_regdom;

    /* 发送消息 */
    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_country_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_TRUE,
                                   &pst_rsp_msg);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_regdomain_update::err %d!}", l_ret);
        if (oal_value_ne_all2(l_ret, -OAL_ETIMEDOUT, -OAL_EFAUL)) {
            oal_mem_free_m(pst_mac_regdom, OAL_TRUE);
            pst_mac_regdom = OAL_PTR_NULL;
        }
        if (pst_rsp_msg != OAL_PTR_NULL) {
            oal_free(pst_rsp_msg);
        }
        return l_ret;
    }
    oal_free(pst_rsp_msg);

    /* 驱动不支持ACS时，更新hostapd管制域信息; 如果驱动支持ACS，则不需要更新，否则hostapd无法配置DFS信道 */
    pst_mac_vap = (mac_vap_stru *)oal_net_dev_priv(pst_net_dev);
    if (oal_unlikely(pst_mac_vap == OAL_PTR_NULL)) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_regdomain_update::OAL_NET_DEV_PRIV(pst_net_dev) null.}");
        return -OAL_FAIL;
    }

    uc_dev_id = pst_mac_vap->uc_device_id;
    pst_device = mac_res_get_dev(uc_dev_id);

    if ((pst_device != OAL_PTR_NULL) && (pst_device->pst_wiphy != OAL_PTR_NULL)) {
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 3, 0))
        
        wal_linux_update_wiphy_channel_list_num(pst_net_dev, pst_device->pst_wiphy);
#endif

        oam_warning_log0(0, OAM_SF_ANY, "{wal_regdomain_update::update regdom to kernel.}");

        wal_cfg80211_reset_bands(uc_dev_id);
        oal_wiphy_apply_custom_regulatory(pst_device->pst_wiphy, pst_regdom);
        
        wal_cfg80211_save_bands(uc_dev_id);
    }
    return OAL_SUCC;
}

#ifdef _PRE_WLAN_COUNTRYCODE_SELFSTUDY
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE

OAL_STATIC void wal_regdomain_update_rf_param(oal_net_device_stru *pst_net_dev, int8_t *pc_country,
                                                      int8_t *old_pc_country)
{
    if (EOK != memcpy_s(old_pc_country, COUNTRY_CODE_LEN, hwifi_get_country_code(), COUNTRY_CODE_LEN)) {
        oam_error_log0(0, OAM_SF_ANY, "wal_regdomain_update_rf_param::memcpy fail!");
        return;
    }

    hwifi_set_country_code(pc_country, COUNTRY_CODE_LEN);
    /* 如果新的国家码和旧国家处于一个regdomain，不刷新RF参数，只更新国家码 */
    if (OAL_TRUE == hwifi_is_regdomain_changed((uint8_t *)old_pc_country, (uint8_t *)pc_country)) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_regdomain_update_rf_param::regdomain changed, refresh rf param!}");

        /* 刷新参数失败，为了保证国家码和功率参数对应 */
        /* 将国家码设回原来的国家码，本次更新失败 */
        if (hwifi_force_refresh_rf_params(pst_net_dev) != OAL_SUCC) {
            oam_warning_log0(0, OAM_SF_ANY,
                "{wal_regdomain_update_rf_param::refresh rf params failed. Set country back.!}");
            hwifi_set_country_code(old_pc_country, COUNTRY_CODE_LEN);
        }
    }
}
#endif

OAL_STATIC int32_t wal_regdomain_confirm(int8_t *pc_country, mac_regdomain_info_stru *pst_mac_regdom)
{
    if (!wal_is_alpha_upper(pc_country[0]) || !wal_is_alpha_upper(pc_country[1])) {
        if ((pc_country[0] == '9') && (pc_country[1] == '9')) {
            oam_info_log0(0, OAM_SF_ANY, "{wal_regdomain_confirm::set regdomain to 99!}");
        } else {
            oam_warning_log0(0, OAM_SF_ANY, "{wal_regdomain_confirm::country str invalid!}");
            return -OAL_EINVAL;
        }
    }
    return OAL_SUCC;
}


OAL_STATIC void wal_regdomain_update_hostapd_param(OAL_CONST oal_ieee80211_regdomain_stru *pst_regdom,
                                                           oal_net_device_stru *pst_net_dev)
{
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    uint8_t uc_dev_id;
    mac_device_stru *pst_device = OAL_PTR_NULL;

    pst_mac_vap = (mac_vap_stru *)oal_net_dev_priv(pst_net_dev);
    if (oal_unlikely(pst_mac_vap == OAL_PTR_NULL)) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_regdomain_update_hostapd_param::OAL_NET_DEV_PRIV null.}");
        return;
    }

    uc_dev_id = pst_mac_vap->uc_device_id;
    pst_device = mac_res_get_dev(uc_dev_id);

    if ((pst_device != OAL_PTR_NULL) && (pst_device->pst_wiphy != OAL_PTR_NULL)) {
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 3, 0))
        
        wal_linux_update_wiphy_channel_list_num(pst_net_dev, pst_device->pst_wiphy);
#endif

        oam_warning_log0(0, OAM_SF_ANY, "{wal_regdomain_update_hostapd_param::update regdom to kernel.}");

        wal_cfg80211_reset_bands(uc_dev_id);
        oal_wiphy_apply_custom_regulatory(pst_device->pst_wiphy, pst_regdom);
        
        wal_cfg80211_save_bands(uc_dev_id);
    }
}


int32_t wal_selfstdy_regdomain_update(oal_net_device_stru *pst_net_dev, int8_t *pc_country)
{
    OAL_CONST oal_ieee80211_regdomain_stru *pst_regdom;
    uint16_t us_size;
    mac_regdomain_info_stru *pst_mac_regdom = OAL_PTR_NULL;
    wal_msg_write_stru st_write_msg;
    mac_cfg_country_stru *pst_param = OAL_PTR_NULL;
    int32_t l_ret;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
    int8_t old_pc_country[COUNTRY_CODE_LEN] = { '9', '9' };

    wal_regdomain_update_rf_param(pst_net_dev, pc_country, old_pc_country);
#endif

    l_ret = wal_regdomain_confirm(pc_country, pst_mac_regdom);

    pst_regdom = wal_regdb_find_db(pc_country);
    if (pst_regdom == OAL_PTR_NULL) {
        return -OAL_EINVAL;
    }
    us_size = (uint16_t)(OAL_SIZEOF(mac_regclass_info_stru) * pst_regdom->n_reg_rules + MAC_RD_INFO_LEN);

    /* 申请内存存放管制域信息，将内存指针作为事件payload抛下去 */
    /* 此处申请的内存在事件处理函数释放(hmac_config_set_country) */
    pst_mac_regdom = oal_mem_alloc_m(OAL_MEM_POOL_ID_LOCAL, us_size, OAL_TRUE);
    if (pst_mac_regdom == OAL_PTR_NULL) {
        return -OAL_ENOMEM;
    }

    wal_regdomain_fill_info(pst_regdom, pst_mac_regdom);

    /* 抛事件到wal层处理 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_COUNTRY, OAL_SIZEOF(mac_cfg_country_stru));

    /* 填写WID对应的参数 */
    pst_mac_regdom->en_regdomain = hwifi_get_regdomain_from_country_code(pc_country);
    pst_param = (mac_cfg_country_stru *)(st_write_msg.auc_value);
    pst_param->p_mac_regdom = pst_mac_regdom;

    /* 发送消息 */
    l_ret = wal_send_cfg_event(pst_net_dev, WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_country_stru),
                                   (uint8_t *)&st_write_msg, OAL_FALSE, &pst_rsp_msg);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        if (oal_value_ne_all2(l_ret, -OAL_ETIMEDOUT, -OAL_EFAUL)) {
            oal_mem_free_m(pst_mac_regdom, OAL_TRUE);
            pst_mac_regdom = OAL_PTR_NULL;
        }
        if (pst_rsp_msg != OAL_PTR_NULL) {
            oal_free(pst_rsp_msg);
        }
        return l_ret;
    }
    oal_free(pst_rsp_msg);

    /* 驱动不支持ACS时，更新hostapd管制域信息; 如果驱动支持ACS，则不需要更新，否则hostapd无法配置DFS信道 */
    wal_regdomain_update_hostapd_param(pst_regdom, pst_net_dev);
    return OAL_SUCC;
}
#endif

int32_t wal_regdomain_update_for_dfs(oal_net_device_stru *pst_net_dev, int8_t *pc_country)
{
    OAL_CONST oal_ieee80211_regdomain_stru *pst_regdom;
    uint16_t us_size;
    mac_regdomain_info_stru *pst_mac_regdom = OAL_PTR_NULL;
    wal_msg_write_stru st_write_msg;
    mac_dfs_domain_enum_uint8 *pst_param = OAL_PTR_NULL;
    int32_t l_ret;

    if (!wal_is_alpha_upper(pc_country[0]) || !wal_is_alpha_upper(pc_country[1])) {
        if ((pc_country[0] == '9') && (pc_country[1] == '9')) {
            oam_info_log0(0, OAM_SF_ANY, "{wal_regdomain_update_for_dfs::set regdomain to 99!}");
        } else {
            oam_warning_log0(0, OAM_SF_ANY, "{wal_regdomain_update_for_dfs::country str is invalid!}");
            return -OAL_EINVAL;
        }
    }

    pst_regdom = wal_regdb_find_db(pc_country);
    if (pst_regdom == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_regdomain_update_for_dfs::no regdomain db was found!}");
        return -OAL_EINVAL;
    }

    us_size = (uint16_t)(OAL_SIZEOF(mac_regclass_info_stru) * pst_regdom->n_reg_rules + MAC_RD_INFO_LEN);

    /* 申请内存存放管制域信息,在本函数结束后释放 */
    pst_mac_regdom = oal_mem_alloc_m(OAL_MEM_POOL_ID_LOCAL, us_size, OAL_TRUE);
    if (pst_mac_regdom == OAL_PTR_NULL) {
        oam_error_log1(0, OAM_SF_ANY, "{wal_regdomain_update_for_dfs::alloc regdom mem fail,size[%d]}", us_size);
        return -OAL_ENOMEM;
    }

    wal_regdomain_fill_info(pst_regdom, pst_mac_regdom);

    /* 抛事件到wal层处理 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_COUNTRY_FOR_DFS, OAL_SIZEOF(mac_dfs_domain_enum_uint8));

    /* 填写WID对应的参数 */
    pst_param = (mac_dfs_domain_enum_uint8 *)(st_write_msg.auc_value);
    *pst_param = pst_mac_regdom->en_dfs_domain;

    /* 发送消息 */
    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_dfs_domain_enum_uint8),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        /* pst_mac_regdom内存，此处释放 */
        oal_mem_free_m(pst_mac_regdom, OAL_TRUE);
        oam_warning_log1(0, OAM_SF_ANY, "{wal_regdomain_update_for_dfs::err %d!}", l_ret);
        return l_ret;
    }
    /* pst_mac_regdom内存，此处释放 */
    oal_mem_free_m(pst_mac_regdom, OAL_TRUE);

    return OAL_SUCC;
}


uint32_t wal_regdomain_update_sta(uint8_t uc_vap_id)
{
    int8_t *pc_desired_country = OAL_PTR_NULL;

    oal_net_device_stru *pst_net_dev = OAL_PTR_NULL;
    int32_t l_ret;
    oal_bool_enum_uint8 us_updata_rd_by_ie_switch = OAL_FALSE;

    hmac_vap_get_updata_rd_by_ie_switch(uc_vap_id, &us_updata_rd_by_ie_switch);

    if (us_updata_rd_by_ie_switch == OAL_TRUE) {
        pc_desired_country = hmac_vap_get_desired_country(uc_vap_id);

        if (oal_unlikely(pc_desired_country == OAL_PTR_NULL)) {
            oam_error_log0(uc_vap_id, OAM_SF_ANY, "{wal_regdomain_update_sta::pc_desired_country null!}");
            return OAL_ERR_CODE_PTR_NULL;
        }

        /* 期望的国家码全为0，表示对端AP的国家码不存在，采用sta当前默认的国家码 */
        if ((pc_desired_country[0] == 0) && (pc_desired_country[1] == 0)) {
            oam_info_log0(uc_vap_id, OAM_SF_ANY, "{wal_regdomain_update_sta::ap has no country ie,use default}");
            return OAL_SUCC;
        }

        pst_net_dev = hmac_vap_get_net_device(uc_vap_id);
        if (oal_unlikely(pst_net_dev == OAL_PTR_NULL)) {
            oam_error_log0(uc_vap_id, OAM_SF_ANY, "{wal_regdomain_update_sta::pst_net_dev null!}");
            return OAL_ERR_CODE_PTR_NULL;
        }

        l_ret = wal_regdomain_update_for_dfs(pst_net_dev, pc_desired_country);
        if (l_ret != OAL_SUCC) {
            oam_warning_log1(uc_vap_id, OAM_SF_ANY, "{wal_regdomain_update_sta::regdomain_update err:%d!}", l_ret);
            return OAL_FAIL;
        }

        l_ret = wal_regdomain_update(pst_net_dev, pc_desired_country);
        if (l_ret != OAL_SUCC) {
            oam_warning_log1(uc_vap_id, OAM_SF_ANY, "{wal_regdomain_update_sta::regdomain_update err:%d}", l_ret);
            return OAL_FAIL;
        }

    } else {
        oam_info_log0(uc_vap_id, OAM_SF_ANY, "{wal_regdomain_update_sta::us_updata_rd_by_ie_switch is fail!}");
    }
    return OAL_SUCC;
}


int32_t wal_regdomain_update_country_code(oal_net_device_stru *pst_net_dev, int8_t *pc_country)
{
    int32_t l_ret;

    if (pst_net_dev == OAL_PTR_NULL || pc_country == OAL_PTR_NULL) {
        oam_error_log1(0, OAM_SF_ANY, "{wal_regdomain_update_country_code::null ptr.net_dev %p!}",
                       (uintptr_t)pst_net_dev);
        return -OAL_EFAIL;
    }

    /* 设置国家码到wifi 驱动 */
    l_ret = wal_regdomain_update_for_dfs(pst_net_dev, pc_country);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_regdomain_update_country_code::update_for_dfs err [%d]!}", l_ret);
        return -OAL_EFAIL;
    }

    l_ret = wal_regdomain_update(pst_net_dev, pc_country);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_regdomain_update_country_code::update err [%d]!}", l_ret);
        return -OAL_EFAIL;
    }
    return OAL_SUCC;
}
#ifdef _PRE_WLAN_COUNTRYCODE_SELFSTUDY

int32_t wal_regdomain_update_selfstudy_country_code(oal_net_device_stru *pst_net_dev, int8_t *pc_country)
{
    int32_t   l_ret;

    if (pst_net_dev == OAL_PTR_NULL || pc_country == OAL_PTR_NULL) {
        oam_error_log1(0, OAM_SF_ANY,
            "{wal_regdomain_update_selfstudy_country_code::null ptr.net_dev %p!}", (uintptr_t)pst_net_dev);
        return -OAL_EFAIL;
    }

    /* 设置国家码到wifi 驱动 */
    l_ret = wal_regdomain_update_for_dfs(pst_net_dev, pc_country);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY,
            "{wal_regdomain_update_selfstudy_country_code::update_for_dfs return err code [%d]!}\r\n", l_ret);
        return -OAL_EFAIL;
    }

    l_ret = wal_selfstdy_regdomain_update(pst_net_dev, pc_country);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY,
            "{wal_regdomain_update_selfstudy_country_code::update return err code [%d]!}\r\n", l_ret);
        return -OAL_EFAIL;
    }
    return OAL_SUCC;
}

OAL_STATIC void wal_regdomain_update_by_ap(uint8_t uc_vap_id)
{
    int8_t *pc_desired_country = OAL_PTR_NULL;
    oal_net_device_stru *pst_net_dev = OAL_PTR_NULL;
    int32_t l_ret;
    oal_bool_enum_uint8 us_updata_rd_by_ie_switch = OAL_FALSE;

    /* 关联成功后，停止国家码自学习 */
    g_country_code_self_study_flag = OAL_FALSE;

    hmac_vap_get_updata_rd_by_ie_switch(uc_vap_id, &us_updata_rd_by_ie_switch);
    pc_desired_country = hmac_vap_get_desired_country(uc_vap_id);
    /* 若国家码为99，则以关联AP的国家码为准 */
    if (g_country_code_result[0] == '9' && g_country_code_result[1] == '9') {
        if (oal_unlikely(pc_desired_country == OAL_PTR_NULL)) {
            oam_error_log0(uc_vap_id, OAM_SF_ANY, "{wal_regdomain_update_sta::pc_desired_country null!}");
            return;
        }

        /* 期望的国家码全为0，表示对端AP的国家码不存在，采用sta当前默认的国家码 */
        if ((pc_desired_country[0] == 0) && (pc_desired_country[1] == 0)) {
            oam_info_log0(uc_vap_id, OAM_SF_ANY, "{wal_regdomain_update_sta::ap has no country ie,use default}");
        }

        pst_net_dev = hmac_vap_get_net_device(uc_vap_id);
        if (oal_unlikely(pst_net_dev == OAL_PTR_NULL)) {
            oam_error_log0(uc_vap_id, OAM_SF_ANY, "{wal_regdomain_update_sta::pst_net_dev null!}");
        }

        l_ret = wal_regdomain_update_selfstudy_country_code(pst_net_dev, pc_desired_country);
    }
}

void wal_selfstudy_regdomain_update_by_ap(frw_event_stru *pst_event)
{
    if (g_cust_country_code_ignore_flag.en_country_code_study_ingore_hipriv_flag == OAL_FALSE) {
        wal_regdomain_update_by_ap(pst_event->st_event_hdr.uc_vap_id);
    }
}
#endif
#endif

int32_t wal_set_random_mac_to_mib(oal_net_device_stru *pst_net_dev)
{
    uint32_t ul_ret;
    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL;
    wal_msg_stru *pst_cfg_msg = OAL_PTR_NULL;
    wal_msg_write_stru *pst_write_msg = OAL_PTR_NULL;
    mac_cfg_staion_id_param_stru *pst_param = OAL_PTR_NULL;
    mac_vap_stru *pst_mac_vap;
    uint8_t *puc_mac_addr = OAL_PTR_NULL;
    oal_wireless_dev_stru *pst_wdev = OAL_PTR_NULL; /* 对于P2P 场景，p2p0 和 p2p-p2p0 MAC 地址从wlan0 获取 */
    wlan_p2p_mode_enum_uint8 en_p2p_mode = WLAN_LEGACY_VAP_MODE;
    uint8_t *auc_wlan_addr = OAL_PTR_NULL;
    uint8_t *auc_p2p0_addr = OAL_PTR_NULL;
    int32_t l_ret = EOK;
    pst_mac_vap = oal_net_dev_priv(pst_net_dev);
    if (pst_mac_vap == NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_set_random_mac_to_mib::pst_mac_vap NULL}");
        return OAL_FAIL;
    }

    if (pst_mac_vap->pst_mib_info == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_set_random_mac_to_mib::vap->mib_info is NULL !}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    auc_wlan_addr = mac_mib_get_StationID(pst_mac_vap);
    auc_p2p0_addr = mac_mib_get_p2p0_dot11StationID(pst_mac_vap);

    pst_wdev = pst_net_dev->ieee80211_ptr;
    switch (pst_wdev->iftype) {
        case NL80211_IFTYPE_P2P_DEVICE:
            en_p2p_mode = WLAN_P2P_DEV_MODE;
            /* 产生P2P device MAC 地址，将本地mac 地址bit 设置为1 */
            l_ret += memcpy_s(auc_p2p0_addr, WLAN_MAC_ADDR_LEN, oal_netdevice_mac_addr(pst_net_dev), WLAN_MAC_ADDR_LEN);

            oam_warning_log3(0, OAM_SF_ANY, "wal_set_random_mac_to_mib:p2p0 mac is:%02X:XX:XX:XX:%02X:%02X\n",
                auc_p2p0_addr[0], auc_p2p0_addr[4], auc_p2p0_addr[5]); /* 4 5 macaddr */
            break;
        case NL80211_IFTYPE_P2P_CLIENT:
        case NL80211_IFTYPE_P2P_GO:
            en_p2p_mode = (pst_wdev->iftype == NL80211_IFTYPE_P2P_GO) ? WLAN_P2P_GO_MODE : WLAN_P2P_CL_MODE;
            /* 根据上层下发值，产生P2P interface MAC 地址 */
            /* 上层不下发，跟随主mac地址,在wal_cfg80211_add_p2p_interface_init初始化 */
            l_ret += memcpy_s(auc_wlan_addr, WLAN_MAC_ADDR_LEN,
                              oal_netdevice_mac_addr(pst_net_dev), WLAN_MAC_ADDR_LEN);
            break;
        default:
            if (0 == (oal_strcmp("p2p0", pst_net_dev->name))) {
                en_p2p_mode = WLAN_P2P_DEV_MODE;
                /* 产生P2P device MAC 地址，将本地mac 地址bit 设置为1 */
                l_ret += memcpy_s(auc_p2p0_addr, WLAN_MAC_ADDR_LEN, oal_netdevice_mac_addr(pst_net_dev),
                    WLAN_MAC_ADDR_LEN);
                break;
            }

            en_p2p_mode = WLAN_LEGACY_VAP_MODE;
            l_ret += memcpy_s(auc_wlan_addr, WLAN_MAC_ADDR_LEN,
                              oal_netdevice_mac_addr(pst_net_dev), WLAN_MAC_ADDR_LEN);
            break;
    }
    oam_warning_log1(0, OAM_SF_ANY, "{wal_set_random_mac_to_mib::p2p mode [%d]}", en_p2p_mode);
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "wal_set_random_mac_to_mib::memcpy fail!");
        return OAL_FAIL;
    }

    /* send the random mac to dmac */
    /* 抛事件到wal层处理   copy from wal_netdev_set_mac_addr()
       gong TBD : 改为调用通用的config接口
    */
    ul_ret = wal_alloc_cfg_event(pst_net_dev, &pst_event_mem, NULL, &pst_cfg_msg,
        (WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_staion_id_param_stru))); /* 申请事件 */
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_set_random_mac_to_mib() fail; return %d!}", ul_ret);
        return -OAL_ENOMEM;
    }

    /* 填写配置消息 */
    WAL_CFG_MSG_HDR_INIT(&(pst_cfg_msg->st_msg_hdr),
                         WAL_MSG_TYPE_WRITE,
                         WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_staion_id_param_stru),
                         WAL_GET_MSG_SN());

    /* 填写WID消息 */
    pst_write_msg = (wal_msg_write_stru *)pst_cfg_msg->auc_msg_data;
    WAL_WRITE_MSG_HDR_INIT(pst_write_msg, WLAN_CFGID_STATION_ID, OAL_SIZEOF(mac_cfg_staion_id_param_stru));

    pst_param = (mac_cfg_staion_id_param_stru *)pst_write_msg->auc_value; /* 填写WID对应的参数 */
    /* 如果使能P2P，需要将netdevice 对应的P2P 模式在配置参数中配置到hmac 和dmac */
    /* 以便底层识别配到p2p0 或p2p-p2p0 cl */
    pst_param->en_p2p_mode = en_p2p_mode;
    if (en_p2p_mode == WLAN_P2P_DEV_MODE) {
        puc_mac_addr = mac_mib_get_p2p0_dot11StationID(pst_mac_vap);
    } else
    {
        puc_mac_addr = mac_mib_get_StationID(pst_mac_vap);
    }
    oal_set_mac_addr(pst_param->auc_station_id, puc_mac_addr);

    frw_event_dispatch_event(pst_event_mem); /* 分发事件 */
    frw_event_free_m(pst_event_mem);

    return OAL_SUCC;
}


uint32_t wal_hipriv_setcountry(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
#ifdef _PRE_WLAN_FEATURE_11D
    int32_t l_ret;
    uint32_t ul_ret;
    uint32_t ul_off_set;
    int8_t ac_arg[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    int8_t *puc_para = OAL_PTR_NULL;

    /* 设备在up状态不允许配置，必须先down */
    if (0 != (OAL_IFF_RUNNING & oal_netdevice_flags(pst_net_dev))) {
        oam_info_log1(0, OAM_SF_ANY, "{wal_hipriv_setcountry::net_dev flags: %d!}", oal_netdevice_flags(pst_net_dev));
        return OAL_EBUSY;
    }

    /* 获取国家码字符串 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_setcountry::get cmd one arg err[%d]!}", ul_ret);
        return ul_ret;
    }
    puc_para = &ac_arg[0];

    l_ret = wal_regdomain_update_for_dfs(pst_net_dev, puc_para);
    if (l_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_setcountry::regdomain_update err %d!}", l_ret);
        return (uint32_t)l_ret;
    }
    l_ret = wal_regdomain_update(pst_net_dev, puc_para);
    if (l_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_setcountry::regdomain_update err %d!}", l_ret);
        return (uint32_t)l_ret;
    }
#else
    oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_setcountry::_PRE_WLAN_FEATURE_11D is not define!}");
#endif
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
    g_cust_country_code_ignore_flag.en_country_code_ingore_hipriv_flag = OAL_TRUE;
#endif

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_getcountry(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
#ifdef _PRE_WLAN_FEATURE_11D
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;
    wal_msg_rsp_stru *pst_query_rsp_msg = OAL_PTR_NULL;
    wal_msg_query_stru st_query_msg;
    int8_t ac_tmp_buff[OAM_PRINT_FORMAT_LENGTH];
    int32_t l_ret;

    /* 抛事件到wal层处理 */
    st_query_msg.en_wid = WLAN_CFGID_COUNTRY;

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_QUERY,
                                   WAL_MSG_WID_LENGTH,
                                   (uint8_t *)&st_query_msg,
                                   OAL_TRUE,
                                   &pst_rsp_msg);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        if (pst_rsp_msg != OAL_PTR_NULL) {
            oal_free(pst_rsp_msg);
        }

        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_getcountry::wal_alloc_cfg_event err %d!}", l_ret);
        return (uint32_t)l_ret;
    }

    /* 处理返回消息 */
    pst_query_rsp_msg = (wal_msg_rsp_stru *)(pst_rsp_msg->auc_msg_data);

    l_ret = snprintf_s(ac_tmp_buff, sizeof(ac_tmp_buff), sizeof(ac_tmp_buff) - 1,
                       "getcountry code is : %c%c.\n",
                       pst_query_rsp_msg->auc_value[0],
                       pst_query_rsp_msg->auc_value[1]);
    if (l_ret < 0) {
        oam_error_log0(0, OAM_SF_CFG, "hmac_config_list_sta::snprintf_s error!");
        oal_free(pst_rsp_msg);
        oam_print(ac_tmp_buff);
        return OAL_FAIL;
    }

    oam_warning_log3(0, OAM_SF_CFG, "{wal_hipriv_getcountry:: %c, %c, len %d}",
                     pst_query_rsp_msg->auc_value[0],
                     pst_query_rsp_msg->auc_value[1],
                     pst_query_rsp_msg->us_len);

    oal_free(pst_rsp_msg);
    oam_print(ac_tmp_buff);

#else
    oam_info_log0(0, OAM_SF_ANY, "{wal_hipriv_getcountry::_PRE_WLAN_FEATURE_11D is not define!}");
#endif

    return OAL_SUCC;
}

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION) && (LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 34))

OAL_STATIC void *wal_sta_info_seq_start(struct seq_file *f, loff_t *pos)
{
    if (*pos == 0) {
        return f->private;
    } else {
        return NULL;
    }
}


OAL_STATIC int32_t wal_sta_info_seq_show(struct seq_file *f, void *v)
{
#define TID_STAT_TO_USER(_stat) \
        ((_stat[0]) + (_stat[1]) + (_stat[2]) + (_stat[3]) + (_stat[4]) + (_stat[5]) + (_stat[6]) + (_stat[7]))
#define BW_ENUM_TO_NUMBER(_bw)  ((_bw) == 0 ? 20 : (_bw) == 1 ? 40 : 80)

    mac_vap_stru *pst_mac_vap = (mac_vap_stru *)v;
    hmac_vap_stru *pst_hmac_vap;
    oal_dlist_head_stru *pst_entry;
    oal_dlist_head_stru *pst_dlist_tmp;
    mac_user_stru *pst_user_tmp;
    hmac_user_stru *pst_hmac_user_tmp;
    uint8_t *puc_addr;
    uint16_t us_idx = 1;
    oam_stat_info_stru *pst_oam_stat;
    oam_user_stat_info_stru *pst_oam_user_stat;
    uint32_t ul_curr_time;
    int8_t *pac_protocol2string[] = {
        "11a", "11b", "11g", "11g", "11g", "11n", "11ac", "11n", "11ac", "11n", "error" };
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;
    mac_cfg_query_rssi_stru *pst_query_rssi_param;
    mac_cfg_query_rate_stru *pst_query_rate_param;

    pst_hmac_vap = mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log1(0, OAM_SF_ANY, "wal_sta_info_seq_show:hmac vap[%d] null", pst_mac_vap->uc_vap_id);
        return 0;
    }

    /* step1. 同步要查询的dmac信息 */
    oal_dlist_search_for_each_safe(pst_entry, pst_dlist_tmp, &(pst_mac_vap->st_mac_user_list_head))
    {
        pst_user_tmp = oal_dlist_get_entry(pst_entry, mac_user_stru, st_user_dlist);
        if (pst_user_tmp == OAL_PTR_NULL) {
            continue;
        }

        pst_hmac_user_tmp = mac_res_get_hmac_user(pst_user_tmp->us_assoc_id);
        if (pst_hmac_user_tmp == OAL_PTR_NULL) {
            continue;
        }

        if (pst_dlist_tmp == OAL_PTR_NULL) {
            /* 此for循环线程会暂停，期间会有删除用户事件，会出现pst_dlist_tmp为空。为空时直接跳过获取dmac信息 */
            break;
        }

        /* 获取dmac user的RSSI信息 */
        memset_s(&st_write_msg, OAL_SIZEOF(st_write_msg), 0, OAL_SIZEOF(st_write_msg));
        WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_QUERY_RSSI, OAL_SIZEOF(mac_cfg_query_rssi_stru));
        pst_query_rssi_param = (mac_cfg_query_rssi_stru *)st_write_msg.auc_value;

        pst_query_rssi_param->us_user_id = pst_user_tmp->us_assoc_id; /* 将用户的id传下去 */

        l_ret = wal_send_cfg_event(pst_hmac_vap->pst_net_device,
            WAL_MSG_TYPE_WRITE, WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_query_rssi_stru),
            (uint8_t *)&st_write_msg, OAL_FALSE, OAL_PTR_NULL);

        if (l_ret != OAL_SUCC) {
            oam_warning_log1(pst_mac_vap->uc_vap_id, 0, "wal_sta_info_seq_show:send query rssi event fail:%d", l_ret);
        }

        pst_hmac_vap->station_info_query_completed_flag = OAL_FALSE;
        /*lint -e730*/
        l_ret = oal_wait_event_interruptible_timeout_m(pst_hmac_vap->query_wait_q,
            (OAL_TRUE == pst_hmac_vap->station_info_query_completed_flag), 5 * OAL_TIME_HZ); /* 5 querytime */
        /*lint +e730*/
        if (l_ret <= 0) { /* 等待超时或异常 */
            oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "wal_sta_info_seq_show:query rssi timeout:%d", l_ret);
        }

        /* 获取dmac user的速率信息 */
        memset_s(&st_write_msg, OAL_SIZEOF(st_write_msg), 0, OAL_SIZEOF(st_write_msg));
        WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_QUERY_RATE, OAL_SIZEOF(mac_cfg_query_rate_stru));
        pst_query_rate_param = (mac_cfg_query_rate_stru *)st_write_msg.auc_value;

        pst_query_rate_param->us_user_id = pst_user_tmp->us_assoc_id; /* 将用户的id传下去 */

        l_ret = wal_send_cfg_event(pst_hmac_vap->pst_net_device, WAL_MSG_TYPE_WRITE,
            WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_query_rate_stru),
            (uint8_t *)&st_write_msg, OAL_FALSE, OAL_PTR_NULL);

        if (l_ret != OAL_SUCC) {
            oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "wal_sta_info_seq_show:SendQueryRate fail:%d", l_ret);
        }

        pst_hmac_vap->station_info_query_completed_flag = OAL_FALSE;
        /*lint -e730*/
        l_ret = oal_wait_event_interruptible_timeout_m(pst_hmac_vap->query_wait_q,
            (OAL_TRUE == pst_hmac_vap->station_info_query_completed_flag), 5 * OAL_TIME_HZ); /* 5 querytime */
        /*lint +e730*/
        if (l_ret <= 0) { /* 等待超时或异常 */
            oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "wal_sta_info_seq_show:query rate timeout:%d", l_ret);
        }
    }

    /* step2. proc文件输出用户信息 */
    seq_printf(f, "Total user nums: %d\n", pst_mac_vap->us_user_nums);
    seq_printf(f, "-- STA info table --\n");

    pst_oam_stat = oam_stat_get_stat_all();
    ul_curr_time = (uint32_t)oal_time_get_stamp_ms();

    oal_dlist_search_for_each_safe(pst_entry, pst_dlist_tmp, &(pst_mac_vap->st_mac_user_list_head))
    {
        pst_user_tmp = oal_dlist_get_entry(pst_entry, mac_user_stru, st_user_dlist);
        if (pst_user_tmp == OAL_PTR_NULL) {
            continue;
        }

        pst_hmac_user_tmp = mac_res_get_hmac_user(pst_user_tmp->us_assoc_id);
        if (pst_hmac_user_tmp == OAL_PTR_NULL) {
            continue;
        }

        pst_oam_user_stat = &(pst_oam_stat->ast_user_stat_info[pst_user_tmp->us_assoc_id]);
        puc_addr = pst_user_tmp->auc_user_mac_addr;

        seq_printf(f, "%2d: aid: %d\n"
                   "    MAC ADDR: %02X:%02X:%02X:%02X:%02X:%02X\n"
                   "    status: %d\n"
                   "    BW: %d\n"
                   "    NSS: %d\n"
                   "    RSSI: %d\n"
                   "    phy type: %s\n"
                   "    TX rate: %dkbps\n"
                   "    RX rate: %dkbps\n"
                   "    RX rate_min: %dkbps\n"
                   "    RX rate_max: %dkbps\n"
                   "    user online time: %us\n"
                   "    TX packets succ: %u\n"
                   "    TX packets fail: %u\n"
                   "    RX packets succ: %u\n"
                   "    RX packets fail: %u\n"
                   "    TX power: %ddBm\n"
                   "    TX bytes: %u\n"
                   "    RX bytes: %u\n"
                   "    TX retries: %u\n"
#ifdef _PRE_WLAN_DFT_STAT
                   "    Curr_rate PER: %u\n"
                   "    Best_rate PER: %u\n"
                   "    Tx Throughput: %u\n" /* 待补充 */
#endif
                   ,
                   us_idx, pst_user_tmp->us_assoc_id,
                   puc_addr[0], puc_addr[1], puc_addr[2], puc_addr[3], puc_addr[4], puc_addr[5],
                   pst_user_tmp->en_user_asoc_state, /* status */
                   BW_ENUM_TO_NUMBER(pst_user_tmp->en_avail_bandwidth),
                   /* NSS,加1是为了方便用户查看，软件处理都是0表示单流 */
                   (pst_user_tmp->en_avail_num_spatial_stream + 1),
                   pst_hmac_user_tmp->c_rssi,
                   pac_protocol2string[pst_user_tmp->en_avail_protocol_mode],
                   pst_hmac_user_tmp->ul_tx_rate,
                   pst_hmac_user_tmp->ul_rx_rate,
                   pst_hmac_user_tmp->ul_rx_rate_min,
                   pst_hmac_user_tmp->ul_rx_rate_max,
                   (uint32_t)oal_time_get_runtime(pst_hmac_user_tmp->ul_first_add_time, ul_curr_time) / 1000,
                   TID_STAT_TO_USER(pst_oam_user_stat->aul_tx_mpdu_succ_num) +
                        TID_STAT_TO_USER(pst_oam_user_stat->aul_tx_mpdu_in_ampdu),
                   TID_STAT_TO_USER(pst_oam_user_stat->aul_tx_mpdu_fail_num) +
                        TID_STAT_TO_USER(pst_oam_user_stat->aul_tx_mpdu_fail_in_ampdu),
                   pst_oam_user_stat->ul_rx_mpdu_num, /* RX packets succ */
                   0,
                   20, /* TX power, 暂不使用 后续调用tpc获取tx_power接口 */
                   TID_STAT_TO_USER(pst_oam_user_stat->aul_tx_mpdu_bytes) +
                        TID_STAT_TO_USER(pst_oam_user_stat->aul_tx_ampdu_bytes),
                   pst_oam_user_stat->ul_rx_mpdu_bytes,  /* RX bytes */
                   pst_oam_user_stat->ul_tx_ppdu_retries /* TX retries */
#ifdef _PRE_WLAN_DFT_STAT
                   , pst_hmac_user_tmp->uc_cur_per,
                   pst_hmac_user_tmp->uc_bestrate_per,
                   0
#endif
                );

        us_idx++;

        if (pst_dlist_tmp == OAL_PTR_NULL) {
            break;
        }
    }

#undef TID_STAT_TO_USER
#undef BW_ENUM_TO_NUMBER
    return 0;
}


OAL_STATIC void *wal_sta_info_seq_next(struct seq_file *f, void *v, loff_t *pos)
{
    return NULL;
}


OAL_STATIC void wal_sta_info_seq_stop(struct seq_file *f, void *v)
{
}

/* dmac_sta_info_seq_ops: 定义seq_file ops */
OAL_STATIC OAL_CONST struct seq_operations wal_sta_info_seq_ops = {
    .start = wal_sta_info_seq_start,
    .next = wal_sta_info_seq_next,
    .stop = wal_sta_info_seq_stop,
    .show = wal_sta_info_seq_show
};


OAL_STATIC int32_t wal_sta_info_seq_open(struct inode *inode, struct file *filp)
{
    int32_t l_ret;
    struct seq_file *pst_seq_file;
    struct proc_dir_entry *pde = PDE(inode);

    l_ret = seq_open(filp, &wal_sta_info_seq_ops);
    if (l_ret == OAL_SUCC) {
        pst_seq_file = (struct seq_file *)filp->private_data;

        pst_seq_file->private = pde->data;
    }

    return l_ret;
}

/* gst_sta_info_proc_fops: 定义sta info proc fops */
OAL_STATIC OAL_CONST struct file_operations gst_sta_info_proc_fops = {
    .owner = THIS_MODULE,
    .open = wal_sta_info_seq_open,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = seq_release
};


OAL_STATIC int wal_read_vap_info_proc(char *page, char **start, off_t off,
                                      int count, int *eof, void *data)
{
    int len;
    mac_vap_stru *pst_mac_vap = (mac_vap_stru *)data;
    oam_stat_info_stru *pst_oam_stat;
    oam_vap_stat_info_stru *pst_oam_vap_stat;

#ifdef _PRE_WLAN_DFT_STAT
    mac_cfg_query_ani_stru *pst_query_ani_param;
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;
    hmac_vap_stru *pst_hmac_vap;

    pst_hmac_vap = mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log1(0, OAM_SF_ANY, "wal_read_vap_info_proc:hmac vap null.vap:%d", pst_mac_vap->uc_vap_id);
        return 0;
    }
    /* 获取dmac vap的ANI信息 */
    memset_s(&st_write_msg, OAL_SIZEOF(st_write_msg), 0, OAL_SIZEOF(st_write_msg));
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_QUERY_RSSI, OAL_SIZEOF(mac_cfg_query_ani_stru));
    pst_query_ani_param = (mac_cfg_query_ani_stru *)st_write_msg.auc_value;

    l_ret = wal_send_cfg_event(pst_hmac_vap->pst_net_device, WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_query_ani_stru),
                                   (uint8_t *)&st_write_msg, OAL_FALSE, OAL_PTR_NULL);
    if (l_ret != OAL_SUCC) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, 0, "wal_read_vap_info_proc:send query ani cfg event:%d", l_ret);
    }

    pst_hmac_vap->station_info_query_completed_flag = OAL_FALSE;
    /*lint -e730*/
    l_ret = oal_wait_event_interruptible_timeout_m(pst_hmac_vap->query_wait_q,
        (OAL_TRUE == pst_hmac_vap->station_info_query_completed_flag), 5 * OAL_TIME_HZ); /* 5 querytime */
    /*lint +e730*/
    if (l_ret <= 0) { /* 等待超时或异常 */
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "wal_read_vap_info_proc:query ani timeout:%d", l_ret);
    }
#endif

    pst_oam_stat = oam_stat_get_stat_all();

    pst_oam_vap_stat = &(pst_oam_stat->ast_vap_stat_info[pst_mac_vap->uc_vap_id]);

    len = snprintf_s(page, PAGE_SIZE, PAGE_SIZE - 1,
                     "vap stats:\n"
                     "  TX bytes: %u\n"
                     "  TX packets: %u\n"
                     "  TX packets error: %u\n"
                     "  TX packets discard: %u\n"
                     "  TX unicast packets: %u\n"
                     "  TX multicast packets: %u\n"
                     "  TX broadcast packets: %u\n",
                     pst_oam_vap_stat->ul_tx_bytes_from_lan,
                     pst_oam_vap_stat->ul_tx_pkt_num_from_lan,
                     pst_oam_vap_stat->ul_tx_abnormal_msdu_dropped +
                     pst_oam_vap_stat->ul_tx_security_check_faild +
                     pst_oam_vap_stat->ul_tx_abnormal_mpdu_dropped,
                     pst_oam_vap_stat->ul_tx_uapsd_process_dropped +
                     pst_oam_vap_stat->ul_tx_psm_process_dropped +
                     pst_oam_vap_stat->ul_tx_alg_process_dropped,
                     pst_oam_vap_stat->ul_tx_pkt_num_from_lan - pst_oam_vap_stat->ul_tx_m2u_mcast_cnt,
                     0,
                     pst_oam_vap_stat->ul_tx_m2u_mcast_cnt);

    if (len >= PAGE_SIZE || len < 0) {
        oam_warning_log2(pst_mac_vap->uc_vap_id, 0, "wal_read_vap_info_proc:len:%d,page size: %d", len, PAGE_SIZE);
        return (PAGE_SIZE - 1);
    }
    len += snprintf_s(page + len, PAGE_SIZE - len, PAGE_SIZE - len - 1,
                      "  RX bytes: %u\n"
                      "  RX packets: %u\n"
                      "  RX packets error: %u\n"
                      "  RX packets discard: %u\n"
                      "  RX unicast packets: %u\n"
                      "  RX multicast packets: %u\n"
                      "  RX broadcast packets: %u\n"
                      "  RX unhnown protocol packets: %u\n"
#ifdef _PRE_WLAN_DFT_STAT
                      "  Br_rate_num: %u\n"
                      "  Nbr_rate_num: %u\n"
                      "  Max_rate: %u\n"
                      "  Min_rate: %u\n"
                      "  Channel num: %d\n"
                      "  ANI:\n"
                      "    dmac_device_distance: %d\n"
                      "    cca_intf_state: %d\n"
                      "    co_intf_state: %d\n"
#endif
                      ,
                      pst_oam_vap_stat->ul_rx_bytes_to_lan,
                      pst_oam_vap_stat->ul_rx_pkt_to_lan,
                      pst_oam_vap_stat->ul_rx_defrag_process_dropped +
                      pst_oam_vap_stat->ul_rx_alg_process_dropped +
                      pst_oam_vap_stat->ul_rx_abnormal_dropped,
                      pst_oam_vap_stat->ul_rx_no_buff_dropped +
                      pst_oam_vap_stat->ul_rx_ta_check_dropped +
                      pst_oam_vap_stat->ul_rx_da_check_dropped +
                      pst_oam_vap_stat->ul_rx_replay_fail_dropped +
                      pst_oam_vap_stat->ul_rx_key_search_fail_dropped,
                      pst_oam_vap_stat->ul_rx_pkt_to_lan - pst_oam_vap_stat->ul_rx_mcast_cnt,
                      0,
                      pst_oam_vap_stat->ul_rx_mcast_cnt,
                      0
#ifdef _PRE_WLAN_DFT_STAT
                      , pst_mac_vap->st_curr_sup_rates.uc_br_rate_num,
                      pst_mac_vap->st_curr_sup_rates.uc_nbr_rate_num,
                      pst_mac_vap->st_curr_sup_rates.uc_max_rate,
                      pst_mac_vap->st_curr_sup_rates.uc_min_rate,
                      pst_mac_vap->st_channel.uc_chan_number,
                      pst_hmac_vap->uc_device_distance,
                      pst_hmac_vap->uc_intf_state_cca,
                      pst_hmac_vap->uc_intf_state_co
#endif
                    );

    if (len >= PAGE_SIZE) {
        oam_warning_log2(pst_mac_vap->uc_vap_id, 0, "wal_read_vap_info_proc:len:%d  page size:%d", len, PAGE_SIZE);
        return (PAGE_SIZE - 1);
    }
    return len;
}


OAL_STATIC int wal_read_rf_info_proc(char *page, char **start, off_t off,
                                     int count, int *eof, void *data)
{
    int len;
    mac_vap_stru *pst_mac_vap = (mac_vap_stru *)data;

    len = snprintf_s(page, PAGE_SIZE, PAGE_SIZE - 1, "rf info:\n  channel_num: %d\n",
                     pst_mac_vap->st_channel.uc_chan_number);
    if (len >= PAGE_SIZE) {
        len = PAGE_SIZE - 1;
    }
    return len;
}


OAL_STATIC void wal_add_vap_proc_file(mac_vap_stru *pst_mac_vap, int8_t *pc_name)
{
    hmac_vap_stru *pst_hmac_vap;
    oal_proc_dir_entry_stru *pst_proc_dir;
    oal_proc_dir_entry_stru *pst_proc_vapinfo;
    oal_proc_dir_entry_stru *pst_proc_stainfo;
    oal_proc_dir_entry_stru *pst_proc_mibrf;

    pst_hmac_vap = mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "wal_add_vap_proc_file: pst_hmac_vap null!");
        return;
    }

    pst_proc_dir = proc_mkdir(pc_name, NULL);
    if (pst_proc_dir == OAL_PTR_NULL) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "wal_add_vap_proc_file: proc_mkdir return null");
        return;
    }

    pst_proc_vapinfo = oal_create_proc_entry("ap_info", 420, pst_proc_dir);
    if (pst_proc_vapinfo == OAL_PTR_NULL) {
        oal_remove_proc_entry(pc_name, NULL);
        return;
    }

    pst_proc_stainfo = oal_create_proc_entry("sta_info", 420, pst_proc_dir);
    if (pst_proc_stainfo == OAL_PTR_NULL) {
        oal_remove_proc_entry("ap_info", pst_proc_dir);
        oal_remove_proc_entry(pc_name, NULL);
        return;
    }

    pst_proc_mibrf = oal_create_proc_entry("mib_rf", 420, pst_proc_dir);
    if (pst_proc_mibrf == OAL_PTR_NULL) {
        oal_remove_proc_entry("ap_info", pst_proc_dir);
        oal_remove_proc_entry("sta_info", pst_proc_dir);
        oal_remove_proc_entry(pc_name, NULL);
        return;
    }

    /* vap info */
    pst_proc_vapinfo->read_proc = wal_read_vap_info_proc;
    pst_proc_vapinfo->data = pst_mac_vap;

    /* sta info，对于文件比较大的proc file，通过proc_fops的方式输出 */
    pst_proc_stainfo->data = pst_mac_vap;
    pst_proc_stainfo->proc_fops = &gst_sta_info_proc_fops;

    /* rf info */
    pst_proc_mibrf->read_proc = wal_read_rf_info_proc;
    pst_proc_mibrf->data = pst_mac_vap;

    pst_hmac_vap->pst_proc_dir = pst_proc_dir;
}


OAL_STATIC void wal_del_vap_proc_file(oal_net_device_stru *pst_net_dev)
{
    mac_vap_stru *pst_mac_vap;
    hmac_vap_stru *pst_hmac_vap;

    pst_mac_vap = oal_net_dev_priv(pst_net_dev);
    if (oal_unlikely(pst_mac_vap == OAL_PTR_NULL)) {
        oam_warning_log1(0, OAM_SF_ANY, "wal_del_vap_proc_file:mac_vap null! net_dev:%x", (uintptr_t)pst_net_dev);
        return;
    }

    pst_hmac_vap = mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log1(0, OAM_SF_ANY, "wal_del_vap_proc_file:hmac_vap null. mac vap id:%d", pst_mac_vap->uc_vap_id);
        return;
    }

    if (pst_hmac_vap->pst_proc_dir) {
        oal_remove_proc_entry("mib_rf", pst_hmac_vap->pst_proc_dir);
        oal_remove_proc_entry("sta_info", pst_hmac_vap->pst_proc_dir);
        oal_remove_proc_entry("ap_info", pst_hmac_vap->pst_proc_dir);
        oal_remove_proc_entry(pst_hmac_vap->auc_name, NULL);
        pst_hmac_vap->pst_proc_dir = OAL_PTR_NULL;
    }
}
#elif (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION) && (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 10, 59))

OAL_STATIC int32_t wal_sta_info_seq_show_sec(struct seq_file *f, void *v)
{
#define TID_STAT_TO_USER(_stat) \
    ((_stat[0]) + (_stat[1]) + (_stat[2]) + (_stat[3]) + (_stat[4]) + (_stat[5]) + (_stat[6]) + (_stat[7]))
#define BW_ENUM_TO_NUMBER(_bw)  ((_bw) == 0 ? 20 : (_bw) == 1 ? 40 : 80)

    mac_vap_stru *pst_mac_vap = (mac_vap_stru *)(f->private);
    hmac_vap_stru *pst_hmac_vap;
    oal_dlist_head_stru *pst_entry;
    oal_dlist_head_stru *pst_dlist_tmp;
    mac_user_stru *pst_user_tmp;
    hmac_user_stru *pst_hmac_user_tmp;
    uint8_t *puc_addr;
    uint16_t us_idx = 1;
    oam_stat_info_stru *pst_oam_stat;
    oam_user_stat_info_stru *pst_oam_user_stat;
    uint32_t ul_curr_time;
    int8_t *pac_protocol2string[] = {
        "11a", "11b", "11g", "11g", "11g", "11n", "11ac", "11n", "11ac", "11n", "error" };
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;
    mac_cfg_query_rssi_stru *pst_query_rssi_param;
    mac_cfg_query_rate_stru *pst_query_rate_param;

    pst_hmac_vap = mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "wal_sta_info_seq_show: hmac vap null.");
        return 0;
    }
    /* step1. 同步要查询的dmac信息 */
    oal_dlist_search_for_each_safe(pst_entry, pst_dlist_tmp, &(pst_mac_vap->st_mac_user_list_head))
    {
        pst_user_tmp = oal_dlist_get_entry(pst_entry, mac_user_stru, st_user_dlist);

        pst_hmac_user_tmp = mac_res_get_hmac_user(pst_user_tmp->us_assoc_id);
        if (pst_hmac_user_tmp == OAL_PTR_NULL) {
            continue;
        }

        if (pst_dlist_tmp == OAL_PTR_NULL) {
            /* 此for循环线程会暂停，期间会有删除用户事件，会出现pst_dlist_tmp为空。为空时直接跳过获取dmac信息 */
            break;
        }

        /* 获取dmac user的RSSI信息 */
        memset_s(&st_write_msg, OAL_SIZEOF(st_write_msg), 0, OAL_SIZEOF(st_write_msg));
        WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_QUERY_RSSI, OAL_SIZEOF(mac_cfg_query_rssi_stru));
        pst_query_rssi_param = (mac_cfg_query_rssi_stru *)st_write_msg.auc_value;

        pst_query_rssi_param->us_user_id = pst_user_tmp->us_assoc_id; /* 将用户的id传下去 */

        l_ret = wal_send_cfg_event(pst_hmac_vap->pst_net_device, WAL_MSG_TYPE_WRITE,
            WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_query_rssi_stru),
            (uint8_t *)&st_write_msg, OAL_FALSE, OAL_PTR_NULL);
        if (l_ret != OAL_SUCC) {
            oam_warning_log1(pst_mac_vap->uc_vap_id, 0, "wal_sta_info_seq_show:send query rssi cfg event:%d", l_ret);
        }

        pst_hmac_vap->station_info_query_completed_flag = OAL_FALSE;
        /*lint -save -e774 */
        /*lint -e730*/
        l_ret = oal_wait_event_interruptible_timeout_m(pst_hmac_vap->query_wait_q,
            (OAL_TRUE == pst_hmac_vap->station_info_query_completed_flag), 5 * OAL_TIME_HZ); /* 5 querytime */
        /*lint +e730*/
        /*lint -restore */
        if (l_ret <= 0) { /* 等待超时或异常 */
            oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "wal_sta_info_seq_show:query rssi timeout:%d", l_ret);
        }

        /* 获取dmac user的速率信息 */
        memset_s(&st_write_msg, OAL_SIZEOF(st_write_msg), 0, OAL_SIZEOF(st_write_msg));
        WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_QUERY_RATE, OAL_SIZEOF(mac_cfg_query_rate_stru));
        pst_query_rate_param = (mac_cfg_query_rate_stru *)st_write_msg.auc_value;

        pst_query_rate_param->us_user_id = pst_user_tmp->us_assoc_id; /* 将用户的id传下去 */

        l_ret = wal_send_cfg_event(pst_hmac_vap->pst_net_device, WAL_MSG_TYPE_WRITE,
                                       WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_query_rate_stru),
                                       (uint8_t *)&st_write_msg, OAL_FALSE, OAL_PTR_NULL);
        if (l_ret != OAL_SUCC) {
            oam_warning_log1(pst_mac_vap->uc_vap_id, 0, "wal_sta_info_seq_show:send query rate cfg fail:%d", l_ret);
        }

        pst_hmac_vap->station_info_query_completed_flag = OAL_FALSE;
        /*lint -save -e774 */
        /*lint -e730*/
        l_ret = oal_wait_event_interruptible_timeout_m(pst_hmac_vap->query_wait_q,
            (OAL_TRUE == pst_hmac_vap->station_info_query_completed_flag), 5 * OAL_TIME_HZ); /* 5 querytime */
        /*lint +e730*/
        /*lint -restore */
        if (l_ret <= 0) { /* 等待超时或异常 */
            oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "wal_sta_info_seq_show: query rate timeout:%d", l_ret);
        }
    }

    /* step2. proc文件输出用户信息 */
    seq_printf(f, "Total user nums: %d\n", pst_mac_vap->us_user_nums);
    seq_printf(f, "-- STA info table --\n");

    pst_oam_stat = oam_stat_get_stat_all();
    ul_curr_time = (uint32_t)oal_time_get_stamp_ms();

    oal_dlist_search_for_each_safe(pst_entry, pst_dlist_tmp, &(pst_mac_vap->st_mac_user_list_head))
    {
        pst_user_tmp = oal_dlist_get_entry(pst_entry, mac_user_stru, st_user_dlist);

        pst_hmac_user_tmp = mac_res_get_hmac_user(pst_user_tmp->us_assoc_id);
        if (pst_hmac_user_tmp == OAL_PTR_NULL) {
            continue;
        }

        pst_oam_user_stat = &(pst_oam_stat->ast_user_stat_info[pst_user_tmp->us_assoc_id]);
        puc_addr = pst_user_tmp->auc_user_mac_addr;

        seq_printf(f, "%2d: aid: %d\n"
                   "    MAC ADDR: %02X:%02X:%02X:%02X:%02X:%02X\n"
                   "    status: %d\n"
                   "    BW: %d\n"
                   "    NSS: %d\n"
                   "    RSSI: %d\n"
                   "    phy type: %s\n"
                   "    TX rate: %dkbps\n"
                   "    RX rate: %dkbps\n"
                   "    RX rate_min: %dkbps\n"
                   "    RX rate_max: %dkbps\n"
                   "    user online time: %us\n"
                   "    TX packets succ: %u\n"
                   "    TX packets fail: %u\n"
                   "    RX packets succ: %u\n"
                   "    RX packets fail: %u\n"
                   "    TX power: %ddBm\n"
                   "    TX bytes: %u\n"
                   "    RX bytes: %u\n"
                   "    TX retries: %u\n"
#ifdef _PRE_WLAN_DFT_STAT
                   "    Curr_rate PER: %u\n"
                   "    Best_rate PER: %u\n"
                   "    Tx Throughput: %u\n" /* 待补充 */
#endif
                   ,
                   us_idx, pst_user_tmp->us_assoc_id,
                   puc_addr[0], puc_addr[1], puc_addr[2], puc_addr[3], puc_addr[4], puc_addr[5],
                   pst_user_tmp->en_user_asoc_state, /* status */
                   BW_ENUM_TO_NUMBER(pst_user_tmp->en_avail_bandwidth),
                   (pst_user_tmp->en_avail_num_spatial_stream + 1), /* NSS */
                   pst_hmac_user_tmp->c_rssi,
                   pac_protocol2string[pst_user_tmp->en_avail_protocol_mode],
                   pst_hmac_user_tmp->ul_tx_rate,
                   pst_hmac_user_tmp->ul_rx_rate,
                   pst_hmac_user_tmp->ul_rx_rate_min,
                   pst_hmac_user_tmp->ul_rx_rate_max,
                   (uint32_t)oal_time_get_runtime(pst_hmac_user_tmp->ul_first_add_time, ul_curr_time) / 1000,
                   TID_STAT_TO_USER(pst_oam_user_stat->aul_tx_mpdu_succ_num) +
                        TID_STAT_TO_USER(pst_oam_user_stat->aul_tx_mpdu_in_ampdu),
                   TID_STAT_TO_USER(pst_oam_user_stat->aul_tx_mpdu_fail_num) +
                        TID_STAT_TO_USER(pst_oam_user_stat->aul_tx_mpdu_fail_in_ampdu),
                   pst_oam_user_stat->ul_rx_mpdu_num, /* RX packets succ */
                   0,
                   20, /* TX power, 暂不使用 后续调用tpc获取tx_power接口 */
                   TID_STAT_TO_USER(pst_oam_user_stat->aul_tx_mpdu_bytes) +
                        TID_STAT_TO_USER(pst_oam_user_stat->aul_tx_ampdu_bytes),
                   pst_oam_user_stat->ul_rx_mpdu_bytes, /* RX bytes */
                   pst_oam_user_stat->ul_tx_ppdu_retries /* TX retries */
#ifdef _PRE_WLAN_DFT_STAT
                   , pst_hmac_user_tmp->uc_cur_per,
                   pst_hmac_user_tmp->uc_bestrate_per,
                   0
#endif
                );

        us_idx++;

        if (pst_dlist_tmp == OAL_PTR_NULL) {
            break;
        }
    }

#undef TID_STAT_TO_USER
#undef BW_ENUM_TO_NUMBER
    return 0;
}

OAL_STATIC void wal_vap_stat_info(struct seq_file *f, oam_vap_stat_info_stru *pst_oam_vap_stat)
{
    seq_printf(f, "vap stats:\n"
               "  TX bytes: %u\n"
               "  TX packets: %u\n"
               "  TX packets error: %u\n"
               "  TX packets discard: %u\n"
               "  TX unicast packets: %u\n"
               "  TX multicast packets: %u\n"
               "  TX broadcast packets: %u\n",
               pst_oam_vap_stat->ul_tx_bytes_from_lan,
               pst_oam_vap_stat->ul_tx_pkt_num_from_lan,
               pst_oam_vap_stat->ul_tx_abnormal_msdu_dropped + pst_oam_vap_stat->ul_tx_security_check_faild +
                    pst_oam_vap_stat->ul_tx_abnormal_mpdu_dropped,
               pst_oam_vap_stat->ul_tx_uapsd_process_dropped + pst_oam_vap_stat->ul_tx_psm_process_dropped +
                    pst_oam_vap_stat->ul_tx_alg_process_dropped,
               pst_oam_vap_stat->ul_tx_pkt_num_from_lan - pst_oam_vap_stat->ul_tx_m2u_mcast_cnt,
               0,
               pst_oam_vap_stat->ul_tx_m2u_mcast_cnt);

}


OAL_STATIC int32_t wal_vap_info_seq_show_sec(struct seq_file *f, void *v)
{
    mac_vap_stru *pst_mac_vap = (mac_vap_stru *)(f->private);
    oam_stat_info_stru *pst_oam_stat;
    oam_vap_stat_info_stru *pst_oam_vap_stat;

#ifdef _PRE_WLAN_DFT_STAT
    mac_cfg_query_ani_stru *pst_query_ani_param;
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;
    hmac_vap_stru *pst_hmac_vap;

    pst_hmac_vap = mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "wal_read_vap_info_proc:hmac vap null.");
        return 0;
    }
    /* 获取dmac vap的ANI信息 */
    memset_s(&st_write_msg, OAL_SIZEOF(st_write_msg), 0, OAL_SIZEOF(st_write_msg));
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_QUERY_RSSI, OAL_SIZEOF(mac_cfg_query_ani_stru));
    pst_query_ani_param = (mac_cfg_query_ani_stru *)st_write_msg.auc_value;

    l_ret = wal_send_cfg_event(pst_hmac_vap->pst_net_device, WAL_MSG_TYPE_WRITE,
        WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_query_ani_stru),
        (uint8_t *)&st_write_msg, OAL_FALSE, OAL_PTR_NULL);
    if (l_ret != OAL_SUCC) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, 0, "wal_read_vap_info_proc:send query ani cfg event:%d", l_ret);
    }

    pst_hmac_vap->station_info_query_completed_flag = OAL_FALSE;
    /*lint -save -e774 */
    /*lint -e730*/
    l_ret = oal_wait_event_interruptible_timeout_m(pst_hmac_vap->query_wait_q,
        (OAL_TRUE == pst_hmac_vap->station_info_query_completed_flag), 5 * OAL_TIME_HZ); /* 5 querytime */
    /*lint +e730*/
    /*lint -restore */
    if (l_ret <= 0) { /* 等待超时或异常 */
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "wal_read_vap_info_proc: query ani timeout:%d", l_ret);
    }
#endif

    pst_oam_stat = oam_stat_get_stat_all();

    pst_oam_vap_stat = &(pst_oam_stat->ast_vap_stat_info[pst_mac_vap->uc_vap_id]);
    wal_vap_stat_info(f, pst_oam_vap_stat);
    seq_printf(f, "  RX bytes: %u\n"
               "  RX packets: %u\n"
               "  RX packets error: %u\n"
               "  RX packets discard: %u\n"
               "  RX unicast packets: %u\n"
               "  RX multicast packets: %u\n"
               "  RX broadcast packets: %u\n"
               "  RX unhnown protocol packets: %u\n"
#ifdef _PRE_WLAN_DFT_STAT
               "  Br_rate_num: %u\n"
               "  Nbr_rate_num: %u\n"
               "  Max_rate: %u\n"
               "  Min_rate: %u\n"
               "  Channel num: %d\n"
               "  ANI:\n"
               "    dmac_device_distance: %d\n"
               "    cca_intf_state: %d\n"
               "    co_intf_state: %d\n"
#endif
               ,
               pst_oam_vap_stat->ul_rx_bytes_to_lan,
               pst_oam_vap_stat->ul_rx_pkt_to_lan,
               pst_oam_vap_stat->ul_rx_defrag_process_dropped + pst_oam_vap_stat->ul_rx_alg_process_dropped +
                    pst_oam_vap_stat->ul_rx_abnormal_dropped,
               pst_oam_vap_stat->ul_rx_no_buff_dropped + pst_oam_vap_stat->ul_rx_ta_check_dropped +
                    pst_oam_vap_stat->ul_rx_da_check_dropped + pst_oam_vap_stat->ul_rx_replay_fail_dropped +
                    pst_oam_vap_stat->ul_rx_key_search_fail_dropped,
               pst_oam_vap_stat->ul_rx_pkt_to_lan - pst_oam_vap_stat->ul_rx_mcast_cnt,
               0,
               pst_oam_vap_stat->ul_rx_mcast_cnt,
               0
#ifdef _PRE_WLAN_DFT_STAT
               , pst_mac_vap->st_curr_sup_rates.uc_br_rate_num,
               pst_mac_vap->st_curr_sup_rates.uc_nbr_rate_num,
               pst_mac_vap->st_curr_sup_rates.uc_max_rate,
               pst_mac_vap->st_curr_sup_rates.uc_min_rate,
               pst_mac_vap->st_channel.uc_chan_number,
               pst_hmac_vap->uc_device_distance,
               pst_hmac_vap->uc_intf_state_cca,
               pst_hmac_vap->uc_intf_state_co
#endif
            );

    return 0;
}

OAL_STATIC int32_t wal_sta_info_proc_open(struct inode *inode, struct file *filp)
{
    int32_t l_ret;
    struct proc_dir_entry *pde = PDE(inode);
    l_ret = single_open(filp, wal_sta_info_seq_show_sec, pde->data);

    return l_ret;
}

OAL_STATIC int32_t wal_vap_info_proc_open(struct inode *inode, struct file *filp)
{
    int32_t l_ret;
    struct proc_dir_entry *pde = PDE(inode);
    l_ret = single_open(filp, wal_vap_info_seq_show_sec, pde->data);

    return l_ret;
}

static ssize_t wal_sta_info_proc_write(struct file *filp, const char __user *buffer, size_t len, loff_t *off)
{
    char mode;
    if (len < 1) {
        return -EINVAL;
    }

    if (copy_from_user(&mode, buffer, sizeof(mode))) {
        return -EFAULT;
    }

    return len;
}

static ssize_t wal_vap_info_proc_write(struct file *filp, const char __user *buffer, size_t len, loff_t *off)
{
    char mode;
    if (len < 1) {
        return -EINVAL;
    }

    if (copy_from_user(&mode, buffer, sizeof(mode))) {
        return -EFAULT;
    }

    return len;
}

/* gst_sta_info_proc_fops_sec: 定义sta info proc fops */
OAL_STATIC OAL_CONST struct file_operations gst_sta_info_proc_fops_sec = {
    .owner = THIS_MODULE,
    .open = wal_sta_info_proc_open,
    .read = seq_read,
    .write = wal_sta_info_proc_write,
    .llseek = seq_lseek,
    .release = single_release,
};

/* gst_vap_info_proc_fops_sec: 定义vap info proc fops */
OAL_STATIC OAL_CONST struct file_operations gst_vap_info_proc_fops_sec = {
    .owner = THIS_MODULE,
    .open = wal_vap_info_proc_open,
    .read = seq_read,
    .write = wal_vap_info_proc_write,
    .llseek = seq_lseek,
    .release = single_release,
};


OAL_STATIC void wal_add_vap_proc_file_sec(mac_vap_stru *pst_mac_vap, int8_t *pc_name)
{
    hmac_vap_stru           *pst_hmac_vap;
    oal_proc_dir_entry_stru *pst_proc_dir;
    oal_proc_dir_entry_stru *pst_proc_vapinfo;
    oal_proc_dir_entry_stru *pst_proc_stainfo;

    pst_hmac_vap = mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "wal_add_vap_proc_file:hmac_vap null!");
        return;
    }

    pst_proc_dir = proc_mkdir(pc_name, NULL);
    if (pst_proc_dir == OAL_PTR_NULL) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "wal_add_vap_proc_file:proc_mkdir return null");
        return;
    }

    pst_proc_vapinfo = proc_create("ap_info", 420, pst_proc_dir, &gst_vap_info_proc_fops_sec);
    if (pst_proc_vapinfo == OAL_PTR_NULL) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "wal_add_vap_proc_file:proc_create return null");
        oal_remove_proc_entry(pc_name, NULL);
        return;
    }

    pst_proc_stainfo = proc_create("sta_info", 420, pst_proc_dir, &gst_sta_info_proc_fops_sec);
    if (pst_proc_stainfo == OAL_PTR_NULL) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "wal_add_vap_proc_file:proc_create return null");
        oal_remove_proc_entry("ap_info", pst_proc_dir);
        oal_remove_proc_entry(pc_name, NULL);
        return;
    }

    pst_proc_vapinfo->data = pst_mac_vap;
    pst_proc_stainfo->data = pst_mac_vap;

    pst_hmac_vap->pst_proc_dir = pst_proc_dir;
}


OAL_STATIC void wal_del_vap_proc_file_sec(oal_net_device_stru *pst_net_dev)
{
    mac_vap_stru *pst_mac_vap;
    hmac_vap_stru *pst_hmac_vap;

    pst_mac_vap = oal_net_dev_priv(pst_net_dev);
    if (oal_unlikely(pst_mac_vap == OAL_PTR_NULL)) {
        return;
    }

    pst_hmac_vap = mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log1(0, OAM_SF_ANY, "wal_del_vap_proc_file:hmac_vap null. vap:%d", pst_mac_vap->uc_vap_id);
        return;
    }

    if (pst_hmac_vap->pst_proc_dir) {
        oal_remove_proc_entry("sta_info", pst_hmac_vap->pst_proc_dir);
        oal_remove_proc_entry("ap_info", pst_hmac_vap->pst_proc_dir);
        oal_remove_proc_entry(pst_hmac_vap->auc_name, NULL);
        pst_hmac_vap->pst_proc_dir = OAL_PTR_NULL;
    }
}
#endif
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
void wal_vap_get_wlan_mode_para(wal_msg_write_stru *st_write_msg)
{
    mac_cfg_add_vap_param_stru *vap_param = (mac_cfg_add_vap_param_stru *)st_write_msg->auc_value;

    vap_param->bit_11ac2g_enable = wlan_chip_get_11ac2g_enable();

    vap_param->bit_disable_capab_2ght40 = g_wlan_cust->uc_disable_capab_2ght40;
}
#endif

OAL_STATIC uint32_t wal_hipriv_add_vap(oal_net_device_stru *pst_cfg_net_dev, int8_t *pc_param)
{
    oal_net_device_stru *pst_net_dev = OAL_PTR_NULL;
    wal_msg_write_stru st_write_msg;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;
    uint32_t ul_err_code;
    uint32_t ul_ret;
    int32_t l_ret;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    int8_t ac_mode[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    wlan_vap_mode_enum_uint8 en_mode;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;

    oal_wireless_dev_stru *pst_wdev = OAL_PTR_NULL;

    mac_vap_stru *pst_cfg_mac_vap = OAL_PTR_NULL;
    mac_device_stru *pst_mac_device = OAL_PTR_NULL;

    wlan_p2p_mode_enum_uint8 en_p2p_mode = WLAN_LEGACY_VAP_MODE;

    /* pc_param指向新创建的net_device的name, 将其取出存放到ac_name中 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_add_vap::wal_get_cmd_one_arg err_code %d!}", ul_ret);
        return ul_ret;
    }

    /* ac_name length不应超过OAL_IF_NAME_SIZE */
    if (OAL_IF_NAME_SIZE <= OAL_STRLEN(ac_name)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_add_vap:: vap name overlength is %d!}", OAL_STRLEN(ac_name));
        /* 输出错误的vap name信息 */
        oal_print_hex_dump((uint8_t *)ac_name, OAL_IF_NAME_SIZE, 32, "vap name lengh is overlong:");
        return OAL_FAIL;
    }

    pc_param += ul_off_set;

    /* pc_param 指向'ap|sta', 将其取出放到ac_mode中 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_mode, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        return (uint32_t)ul_ret;
    }

    /* 解析ac_mode字符串对应的模式 */
    if (0 == (oal_strcmp("ap", ac_mode))) {
        en_mode = WLAN_VAP_MODE_BSS_AP;
    } else if (0 == (oal_strcmp("sta", ac_mode))) {
        en_mode = WLAN_VAP_MODE_BSS_STA;
    }
    /* 创建P2P 相关VAP */
    else if (0 == (oal_strcmp("p2p_device", ac_mode))) {
        en_mode = WLAN_VAP_MODE_BSS_STA;
        en_p2p_mode = WLAN_P2P_DEV_MODE;
    } else if (0 == (oal_strcmp("p2p_cl", ac_mode))) {
        en_mode = WLAN_VAP_MODE_BSS_STA;
        en_p2p_mode = WLAN_P2P_CL_MODE;
    } else if (0 == (oal_strcmp("p2p_go", ac_mode))) {
        en_mode = WLAN_VAP_MODE_BSS_AP;
        en_p2p_mode = WLAN_P2P_GO_MODE;
    }
    else {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_add_vap::the mode param is invalid!}");
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    /* 如果创建的net device已经存在，直接返回 */
    /* 根据dev_name找到dev */
    pst_net_dev = wal_config_get_netdev(ac_name, OAL_STRLEN(ac_name));
    if (pst_net_dev != OAL_PTR_NULL) {
        /* 调用oal_dev_get_by_name后，必须调用oal_dev_put使net_dev的引用计数减一 */
        oal_dev_put(pst_net_dev);

        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_add_vap::the net_device is already exist!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 获取mac device */
    pst_cfg_mac_vap = oal_net_dev_priv(pst_cfg_net_dev);
    pst_mac_device = mac_res_get_dev(pst_cfg_mac_vap->uc_device_id);
    if (oal_unlikely(pst_mac_device == OAL_PTR_NULL)) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_net_dev = oal_net_alloc_netdev_mqs(OAL_SIZEOF(oal_netdev_priv_stru), ac_name,
        oal_ether_setup, WLAN_NET_QUEUE_BUTT, 1); /* 此函数第一个入参代表私有长度，此处不涉及为0 */

    if (oal_unlikely(pst_net_dev == OAL_PTR_NULL)) {
        oam_warning_log0(pst_cfg_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_hipriv_add_vap::pst_net_dev null ptr error!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_wdev = (oal_wireless_dev_stru *)oal_mem_alloc_m(OAL_MEM_POOL_ID_LOCAL,
        OAL_SIZEOF(oal_wireless_dev_stru), OAL_FALSE);
    if (oal_unlikely(pst_wdev == OAL_PTR_NULL)) {
        oam_error_log0(pst_cfg_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_hipriv_add_vap::alloc mem, pst_wdev null!}");
        oal_net_free_netdev(pst_net_dev);
        return OAL_ERR_CODE_PTR_NULL;
    }

    memset_s(pst_wdev, OAL_SIZEOF(oal_wireless_dev_stru), 0, OAL_SIZEOF(oal_wireless_dev_stru));

    /* 对netdevice进行赋值 */
#ifdef CONFIG_WIRELESS_EXT
    pst_net_dev->wireless_handlers = &g_st_iw_handler_def;
#endif /* CONFIG_WIRELESS_EXT */
    /* oal_netdevice_ops(pst_net_dev)             = &g_st_wal_net_dev_ops; */
    pst_net_dev->netdev_ops = &g_st_wal_net_dev_ops;

    oal_netdevice_destructor(pst_net_dev) = oal_net_free_netdev;

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 10, 44))
    oal_netdevice_master(pst_net_dev) = OAL_PTR_NULL;
#endif

    oal_netdevice_ifalias(pst_net_dev) = OAL_PTR_NULL;
    oal_netdevice_watchdog_timeo(pst_net_dev) = 5;
    oal_netdevice_wdev(pst_net_dev) = pst_wdev;
    oal_netdevice_qdisc(pst_net_dev, OAL_PTR_NULL);
#ifdef _PRE_WLAN_FEATURE_SMP_SUPPORT
    //    oal_netdevice_tx_queue_len(pst_net_dev) = 0;
#endif

    pst_wdev->netdev = pst_net_dev;

    if (en_mode == WLAN_VAP_MODE_BSS_AP) {
        pst_wdev->iftype = NL80211_IFTYPE_AP;
    } else if (en_mode == WLAN_VAP_MODE_BSS_STA) {
        pst_wdev->iftype = NL80211_IFTYPE_STATION;
    }
    if (WLAN_P2P_DEV_MODE == en_p2p_mode) {
        pst_wdev->iftype = NL80211_IFTYPE_P2P_DEVICE;
    } else if (WLAN_P2P_CL_MODE == en_p2p_mode) {
        pst_wdev->iftype = NL80211_IFTYPE_P2P_CLIENT;
    } else if (WLAN_P2P_GO_MODE == en_p2p_mode) {
        pst_wdev->iftype = NL80211_IFTYPE_P2P_GO;
    }

    pst_wdev->wiphy = pst_mac_device->pst_wiphy;

    oal_netdevice_flags(pst_net_dev) &= ~OAL_IFF_RUNNING; /* 将net device的flag设为down */

    /* st_write_msg作清零操作，防止部分成员因为相关宏没有开而没有赋值，出现非0的异常值，
       例如结构体中P2P mode没有用P2P宏包起来，这里又因为包起来没有正确赋值 */
    memset_s(&st_write_msg, OAL_SIZEOF(wal_msg_write_stru), 0, OAL_SIZEOF(wal_msg_write_stru));

    /* 抛事件到wal层处理 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_ADD_VAP, OAL_SIZEOF(mac_cfg_add_vap_param_stru));
    ((mac_cfg_add_vap_param_stru *)st_write_msg.auc_value)->pst_net_dev = pst_net_dev;
    ((mac_cfg_add_vap_param_stru *)st_write_msg.auc_value)->en_vap_mode = en_mode;
    ((mac_cfg_add_vap_param_stru *)st_write_msg.auc_value)->uc_cfg_vap_indx = pst_cfg_mac_vap->uc_vap_id;

    ((mac_cfg_add_vap_param_stru *)st_write_msg.auc_value)->en_p2p_mode = en_p2p_mode;
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
    wal_vap_get_wlan_mode_para(&st_write_msg);
#endif
    /* 发送消息 */
    l_ret = wal_send_cfg_event(pst_cfg_net_dev, WAL_MSG_TYPE_WRITE,
        WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_add_vap_param_stru),
        (uint8_t *)&st_write_msg, OAL_TRUE, &pst_rsp_msg);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oal_mem_free_m(pst_wdev, OAL_FALSE);
        oal_net_free_netdev(pst_net_dev);
        oam_warning_log1(pst_cfg_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_hipriv_add_vap::err %d!}", l_ret);
        return (uint32_t)l_ret;
    }

    /* 读取返回的错误码 */
    ul_err_code = wal_check_and_release_msg_resp(pst_rsp_msg);
    if (ul_err_code != OAL_SUCC) {
        oam_warning_log1(pst_cfg_mac_vap->uc_vap_id, OAM_SF_ANY,
            "{wal_hipriv_add_vap::hmac add vap fail[%u]!}", ul_err_code);
        /* 异常处理，释放内存 */
        oal_mem_free_m(pst_wdev, OAL_FALSE);
        oal_net_free_netdev(pst_net_dev);
        return ul_err_code;
    }

    if ((en_p2p_mode == WLAN_LEGACY_VAP_MODE) && (pst_mac_device->st_p2p_info.pst_primary_net_device == OAL_PTR_NULL)) {
        /* 如果创建wlan0， 则保存wlan0 为主net_device,p2p0 和p2p-p2p0 MAC 地址从主netdevice 获取 */
        pst_mac_device->st_p2p_info.pst_primary_net_device = pst_net_dev;
    }

    if (OAL_SUCC != wal_set_random_mac_to_mib(pst_net_dev)) {
        /* 异常处理，释放内存 */
        /* 异常处理，释放内存 */
        oal_mem_free_m(pst_wdev, OAL_FALSE);
        oal_net_free_netdev(pst_net_dev);
        return OAL_ERR_CODE_PTR_NULL;
    } /* set random mac to mib ; for hi1102-cb */

    /* 设置netdevice的MAC地址，MAC地址在HMAC层被初始化到MIB中 */
    pst_mac_vap = oal_net_dev_priv(pst_net_dev);
    if (WLAN_P2P_CL_MODE != en_p2p_mode) {
        pst_mac_vap->en_vap_state = MAC_VAP_STATE_INIT;
    }

    if (en_p2p_mode == WLAN_P2P_DEV_MODE) {
        oal_set_mac_addr((uint8_t *)oal_netdevice_mac_addr(pst_net_dev),
            mac_mib_get_p2p0_dot11StationID(pst_mac_vap));

        pst_mac_device->st_p2p_info.uc_p2p0_vap_idx = pst_mac_vap->uc_vap_id;
    } else
    {
        oal_set_mac_addr((uint8_t *)oal_netdevice_mac_addr(pst_net_dev), mac_mib_get_StationID(pst_mac_vap));
    }

    /* 注册net_device */
    ul_ret = (uint32_t)hmac_net_register_netdev(pst_net_dev);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
            "{wal_hipriv_add_vap::hmac_net_register_netdev return error code %d!}", ul_ret);

        /* 异常处理，释放内存 */
        /* 抛删除vap事件释放刚申请的vap  */
        WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_DEL_VAP, OAL_SIZEOF(mac_cfg_del_vap_param_stru));

        l_ret = wal_send_cfg_event(pst_net_dev, WAL_MSG_TYPE_WRITE,
            WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_del_vap_param_stru),
            (uint8_t *)&st_write_msg, OAL_TRUE, &pst_rsp_msg);

        if (OAL_SUCC != wal_check_and_release_msg_resp(pst_rsp_msg)) {
            oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_add_vap::wal_check_and_release_msg_resp fail.}");
        }
        if (l_ret != OAL_SUCC) {
            oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_add_vap::wal_send_cfg_event fail,err code %d!}", l_ret);
        }

        oal_mem_free_m(pst_wdev, OAL_FALSE);
        oal_net_free_netdev(pst_net_dev);
        return ul_ret;
    }

    /* 创建VAP对应的proc文件 */
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION) && (LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 34))
    wal_add_vap_proc_file(pst_mac_vap, ac_name);
#elif (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION) && (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 10, 59))
    wal_add_vap_proc_file_sec(pst_mac_vap, ac_name);
#endif

    return OAL_SUCC;
}


uint32_t wal_hipriv_del_vap(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;
    int32_t l_ret;
    oal_wireless_dev_stru *pst_wdev = OAL_PTR_NULL;
    wlan_p2p_mode_enum_uint8 en_p2p_mode;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    if (oal_unlikely(oal_any_null_ptr2(pst_net_dev, pc_param))) {
        // 访问网络接口的模块可能不止一个,需要上层保证可靠删除
        oam_warning_log2(0, OAM_SF_ANY, "{wal_hipriv_del_vap::pst_net_dev or pc_param null ptr error %x, %x!}",
                         (uintptr_t)pst_net_dev, (uintptr_t)pc_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 设备在up状态不允许删除，必须先down */
    if (oal_unlikely(0 != (OAL_IFF_RUNNING & oal_netdevice_flags(pst_net_dev)))) {
        oam_error_log1(0, OAM_SF_ANY, "{wal_hipriv_del_vap::dev busy, down it first:%d}",
            oal_netdevice_flags(pst_net_dev));
        return OAL_ERR_CODE_CONFIG_BUSY;
    }

    pst_mac_vap = oal_net_dev_priv(pst_net_dev);
    if (oal_unlikely(pst_mac_vap == NULL)) {
        oam_error_log0(0, OAM_SF_CFG, "{wal_hipriv_del_vap::can't get mac vap from netdevice priv data!}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    if (pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_CONFIG) {
        oam_warning_log0(0, OAM_SF_CFG, "{wal_hipriv_del_vap::invalid para,cfg vap!}");
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    /* 设备在up状态不允许删除，必须先down */
    if ((pst_mac_vap->en_vap_state != MAC_VAP_STATE_INIT)
        && (pst_mac_vap->en_p2p_mode != WLAN_P2P_CL_MODE))
    {
        oam_error_log1(0, OAM_SF_ANY, "{wal_hipriv_del_vap::device busy, down it first %d!}",
            pst_mac_vap->en_vap_state);
        return OAL_ERR_CODE_CONFIG_BUSY;
    }

    /* 删除vap对应的proc文件 */
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION) && (LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 34))
    wal_del_vap_proc_file(pst_net_dev);
#elif (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION) && (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 10, 59))
    wal_del_vap_proc_file_sec(pst_net_dev);
#endif

    memset_s(&st_write_msg, OAL_SIZEOF(wal_msg_write_stru), 0, OAL_SIZEOF(wal_msg_write_stru));

    ((mac_cfg_del_vap_param_stru *)st_write_msg.auc_value)->pst_net_dev = pst_net_dev;
    pst_wdev = pst_net_dev->ieee80211_ptr;
    en_p2p_mode = wal_wireless_iftype_to_mac_p2p_mode(pst_wdev->iftype);
    if (WLAN_P2P_BUTT == en_p2p_mode) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_hipriv_del_vap::wal_wireless_iftype_to_mac_p2p_mode return BUFF}");
        return OAL_ERR_CODE_INVALID_CONFIG;
    }
    ((mac_cfg_del_vap_param_stru *)st_write_msg.auc_value)->en_p2p_mode = en_p2p_mode;

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_DEL_VAP, OAL_SIZEOF(mac_cfg_del_vap_param_stru));

    l_ret = wal_send_cfg_event(pst_net_dev, WAL_MSG_TYPE_WRITE,
        WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_del_vap_param_stru),
        (uint8_t *)&st_write_msg, OAL_TRUE, &pst_rsp_msg);

    if (OAL_SUCC != wal_check_and_release_msg_resp(pst_rsp_msg)) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_del_vap::wal_check_and_release_msg_resp fail}");
    }

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_del_vap::err %d}", l_ret);
        /* 去注册 */
        oal_net_unregister_netdev(pst_net_dev);
        oal_mem_free_m(pst_wdev, OAL_TRUE);
        return (uint32_t)l_ret;
    }

    /* 去注册 */
    oal_net_unregister_netdev(pst_net_dev);
    oal_mem_free_m(pst_wdev, OAL_TRUE);

    return OAL_SUCC;
}


uint32_t wal_hipriv_vap_info(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_VAP_INFO, OAL_SIZEOF(int32_t));

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(int32_t),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_vap_info::err %d!}", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}
#ifdef _PRE_WLAN_FEATURE_NEGTIVE_DET

uint32_t wal_hipriv_pk_mode_debug(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    /* pkmode门限调整接口 hipriv "wlan0 pk_mode_debug 0/1(high/low) 0/1/2/3/4(BW) 0/1/2/3(protocol) 吞吐门限值" */
    /*
        BW:20M     40M    80M   160M   80+80M
        protocol:lagency: HT: VHT: HE:
    */
    /*
    PK模式门限基数:
    {(单位Mbps)  20M     40M    80M   160M   80+80M
    lagency:    {valid, valid, valid, valid, valid},   (基础协议模式没有pk mode )
    HT:         {72,    150,   valid, valid, valid},
    VHT:        {86,    200,   433,   866,   866},
    HE:         {valid, valid, valid, valid, valid},   (暂不支持11ax的pk mode)
    };

    PK模式二级门限:
    高档位门限: g_st_pk_mode_high_th_table = PK模式门限基数 * 70% *1024 *1024 /8  (单位字节)
    低档位门限: g_st_pk_mode_low_th_table  = PK模式门限基数 * 20% *1024 *1024 /8  (单位字节)

    */
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;
    uint32_t ul_ret;
    uint32_t ul_off_set = 0;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN] = { 0 };
    uint32_t auc_pk_mode_param[4] = { 0 };

    /* st_write_msg作清零操作 */
    memset_s(&st_write_msg, OAL_SIZEOF(wal_msg_write_stru), 0, OAL_SIZEOF(wal_msg_write_stru));

    /* 0.获取第0个参数: 高门限/第门限 */
    ul_ret = wal_get_cmd_one_arg((int8_t *)pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_M2S, "{wal_hipriv_pk_mode_debug::wal_get_cmd_one_arg1  err_code [%d]!}", ul_ret);
        return ul_ret;
    }

    auc_pk_mode_param[0] = (uint32_t)oal_atoi(ac_name);
    if (auc_pk_mode_param[0] > 3) {
        oam_warning_log1(0, OAM_SF_M2S, "{wal_hipriv_pk_mode_debug::high/low param ERROR[%d]!}", auc_pk_mode_param[0]);
        return OAL_FAIL;
    }

    pc_param = pc_param + ul_off_set;

    /* 1.获取第一个参数: BW */
    ul_ret = wal_get_cmd_one_arg((int8_t *)pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_M2S, "{wal_hipriv_pk_mode_debug::wal_get_cmd_one_arg1  err_code [%d]!}", ul_ret);
        return ul_ret;
    }

    auc_pk_mode_param[1] = (uint32_t)oal_atoi(ac_name);
    if (auc_pk_mode_param[1] >= WLAN_BW_CAP_BUTT) {
        oam_warning_log1(0, OAM_SF_M2S, "{wal_hipriv_pk_mode_debug::BW param ERROR [%d]!}", auc_pk_mode_param[1]);
        return OAL_FAIL;
    }

    pc_param = pc_param + ul_off_set;

    /* 2.获取第二个参数: protocol */
    ul_ret = wal_get_cmd_one_arg((int8_t *)pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_M2S, "{wal_hipriv_pk_mode_debug::wal_get_cmd_one_arg1  err_code [%d]!}", ul_ret);
        return ul_ret;
    }

    auc_pk_mode_param[2] = (uint32_t)oal_atoi(ac_name);
    if (auc_pk_mode_param[2] >= WLAN_PROTOCOL_CAP_BUTT) {
        oam_warning_log1(0, 0, "{wal_hipriv_pk_mode_debug::protocol param drr[%d]!}", auc_pk_mode_param[2]); /* 2 cmd */
        return OAL_FAIL;
    }

    pc_param = pc_param + ul_off_set;

    /* 3.获取第三个参数: 门限值 */
    ul_ret = wal_get_cmd_one_arg((int8_t *)pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_M2S, "{wal_hipriv_pk_mode_debug::wal_get_cmd_one_arg1 err_code [%d]!}", ul_ret);
        return ul_ret;
    }

    auc_pk_mode_param[3] = (uint32_t)oal_atoi(ac_name);

    pc_param = pc_param + ul_off_set;

    /* 设置配置命令参数 */
    st_write_msg.auc_value[0] = auc_pk_mode_param[0];
    st_write_msg.auc_value[1] = auc_pk_mode_param[1];
    st_write_msg.auc_value[2] = auc_pk_mode_param[2];
    st_write_msg.auc_value[3] = auc_pk_mode_param[3];

    /* 抛事件到wal层处理 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SYNC_PK_MODE, OAL_SIZEOF(auc_pk_mode_param));

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(auc_pk_mode_param),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_pk_mode_debug::err %d!}", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}
#endif
#define MAX_HIPRIV_IP_FILTER_BTABLE_SIZE 129

OAL_STATIC uint32_t wal_ipriv_ip_filter_items(wal_hw_wifi_filter_item *pst_items,
    uint32_t ul_item_size, int32_t l_count)
{
    if (ul_item_size > sizeof(wal_hw_wifi_filter_item) * MAX_HIPRIV_IP_FILTER_BTABLE_SIZE) {
        return OAL_FAIL;
    }

    return (uint32_t)wal_add_ip_filter_items(pst_items, l_count);
}


uint32_t wal_hipriv_set_ip_filter(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    int32_t l_items_cnt;
    int32_t l_items_idx;
    int32_t l_enable;
    uint32_t ul_ret;
    uint32_t ul_off_set;

    int8_t ac_cmd[WAL_HIPRIV_CMD_NAME_MAX_LEN] = { 0 };
    int8_t ac_cmd_param[WAL_HIPRIV_CMD_NAME_MAX_LEN] = { 0 };
    wal_hw_wifi_filter_item ast_items[MAX_HIPRIV_IP_FILTER_BTABLE_SIZE];

    l_enable = 0;
    l_items_cnt = 0;
    memset_s((uint8_t *)ast_items, OAL_SIZEOF(wal_hw_wifi_filter_item) * MAX_HIPRIV_IP_FILTER_BTABLE_SIZE,
             0, OAL_SIZEOF(wal_hw_wifi_filter_item) * MAX_HIPRIV_IP_FILTER_BTABLE_SIZE);

    /* 其取出子命令 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_cmd, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_ip_filter::wal_get_cmd_one_arg err_code:%d}", ul_ret);
        return ul_ret;
    }

    if (0 == oal_strncmp(ac_cmd, CMD_CLEAR_RX_FILTERS, OAL_STRLEN(CMD_CLEAR_RX_FILTERS))) {
        /* 清理表单 */
        ul_ret = (uint32_t)wal_clear_ip_filter();
        return ul_ret;
    }

    pc_param += ul_off_set;
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_cmd_param, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_ip_filter::get cmd_param err %d!}", ul_ret);
        return ul_ret;
    }

    if (0 == oal_strncmp(ac_cmd, CMD_SET_RX_FILTER_ENABLE, OAL_STRLEN(CMD_SET_RX_FILTER_ENABLE))) {
        /* 使能/关闭功能 */
        l_enable = oal_atoi(ac_cmd_param);
        ul_ret = (uint32_t)wal_set_ip_filter_enable(l_enable);
        return ul_ret;

    } else if (0 == oal_strncmp(ac_cmd, CMD_ADD_RX_FILTER_ITEMS, OAL_STRLEN(CMD_ADD_RX_FILTER_ITEMS))) {
        /* 更新黑名单 */
        /* 获取名单条目数 */
        l_items_cnt = oal_atoi(ac_cmd_param);
        l_items_cnt = oal_min(MAX_HIPRIV_IP_FILTER_BTABLE_SIZE, l_items_cnt);

        /* 获取名单条目 */
        for (l_items_idx = 0; l_items_idx < l_items_cnt; l_items_idx++) {
            /* 获取protocol X */
            pc_param += ul_off_set;
            ul_ret = wal_get_cmd_one_arg(pc_param, ac_cmd_param, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
            if (ul_ret != OAL_SUCC) {
                oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_ip_filter::get item_params err:%d!}", ul_ret);
                return ul_ret;
            }
            ast_items[l_items_idx].protocol = (uint8_t)oal_atoi(ac_cmd_param);

            /* 获取portX */
            pc_param += ul_off_set;
            ul_ret = wal_get_cmd_one_arg(pc_param, ac_cmd_param, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
            if (ul_ret != OAL_SUCC) {
                oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_ip_filter::get item_params  err: %d!}", ul_ret);
                return ul_ret;
            }
            ast_items[l_items_idx].port = (uint16_t)oal_atoi(ac_cmd_param);
        }

        ul_ret = wal_ipriv_ip_filter_items(ast_items, sizeof(ast_items), l_items_cnt);
        return ul_ret;
    } else {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_set_ip_filter::cmd_one_arg no support!}");
        return OAL_FAIL;
    }
}


OAL_STATIC uint32_t wal_hipriv_set_2040_coext_support(oal_net_device_stru *pst_net_dev,
    int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;
    int32_t l_ret;
    uint8_t uc_csp;

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_2040_coext_support::wal_get_cmd_one_arg err:%d}", ul_ret);
        return ul_ret;
    }

    if (0 == (oal_strcmp("0", ac_name))) {
        uc_csp = 0;
    } else if (0 == (oal_strcmp("1", ac_name))) {
        uc_csp = 1;
    } else {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_2040_coext_support::2040coex cmderr:%x}", (uintptr_t)ac_name);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    /* 抛事件到wal层处理 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_2040_COEXISTENCE, OAL_SIZEOF(int32_t));
    *((int32_t *)(st_write_msg.auc_value)) = uc_csp; /* 设置配置命令参数 */

    l_ret = wal_send_cfg_event(pst_net_dev, WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(int32_t),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE, OAL_PTR_NULL);
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_2040_coext_support::err %d!}", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_rx_fcs_info(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    int32_t l_ret;
    uint32_t ul_ret;
    mac_cfg_rx_fcs_info_stru *pst_rx_fcs_info = OAL_PTR_NULL;
    mac_cfg_rx_fcs_info_stru st_rx_fcs_info; /* 临时保存获取的use的信息 */

    /* 打印接收帧的FCS正确与错误信息:sh hipriv.sh "vap0 rx_fcs_info 0/1 1-4" 0/1  0代表不清除，1代表清除 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_rx_fcs_info::wal_get_cmd_one_arg  err_code %d!}", ul_ret);
        return ul_ret;
    }

    st_rx_fcs_info.ul_data_op = (uint32_t)oal_atoi(ac_name);

    if (st_rx_fcs_info.ul_data_op > 1) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_rx_fcs_info:: ul_data_op cmd is error %x!}", (uintptr_t)ac_name);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    /* 偏移，取下一个参数 */
    pc_param = pc_param + ul_off_set;

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_rx_fcs_info::get cmd one arg err%d!}", ul_ret);
        return ul_ret;
    }

    st_rx_fcs_info.ul_print_info = (uint32_t)oal_atoi(ac_name);

    if (st_rx_fcs_info.ul_print_info > 4) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_rx_fcs_info:: print_info cmd is error %x!}", (uintptr_t)ac_name);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    /* 抛事件到wal层处理 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_RX_FCS_INFO, OAL_SIZEOF(mac_cfg_rx_fcs_info_stru));

    /* 设置配置命令参数 */
    pst_rx_fcs_info = (mac_cfg_rx_fcs_info_stru *)(st_write_msg.auc_value);
    pst_rx_fcs_info->ul_data_op = st_rx_fcs_info.ul_data_op;
    pst_rx_fcs_info->ul_print_info = st_rx_fcs_info.ul_print_info;

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_rx_fcs_info_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_rx_fcs_info::err %d!}", l_ret);
        return (uint32_t)l_ret;
    }
    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_vap_log_level(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    oam_log_level_enum_uint8 en_level_val;
    uint32_t ul_off_set;
    int8_t ac_param[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;

    wal_msg_write_stru st_write_msg;

    /* OAM log模块的开关的命令: hipriv "Hisilicon0[vapx] log_level {1/2}"
       1-2(error与warning)级别日志以vap级别为维度；
    */

    pst_mac_vap = oal_net_dev_priv(pst_net_dev);
    if (oal_unlikely(pst_mac_vap == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_hipriv_vap_log_level::null pointer.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 获取日志级别 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_param, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        return ul_ret;
    }

    en_level_val = (oam_log_level_enum_uint8)oal_atoi(ac_param);
    if ((en_level_val < OAM_LOG_LEVEL_ERROR) || (en_level_val > OAM_LOG_LEVEL_INFO)) {
        oam_error_log1(0, OAM_SF_ANY, "{wal_hipriv_feature_log_level::invalid switch value[%d].}", en_level_val);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    /* hipriv "Hisilicon0 log_level 1|2|3" 设置所有vip id的log */
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
    if (0 == oal_strcmp("Hisilicon0", pst_net_dev->name)) {
        hmac_config_set_all_log_level(pst_mac_vap, OAL_SIZEOF(oam_log_level_enum_uint8),
            (uint8_t *)&en_level_val);
        return OAL_SUCC;
    }
#endif

    ul_ret = oam_log_set_vap_level(pst_mac_vap->uc_vap_id, en_level_val);

    // 目前支持02 device 设置log 级别， 遗留后续的合并问题
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_LOG_LEVEL, OAL_SIZEOF(int32_t));
    *((int32_t *)(st_write_msg.auc_value)) = en_level_val;
    ul_ret |= (uint32_t)wal_send_cfg_event(pst_net_dev,
                                                 WAL_MSG_TYPE_WRITE,
                                                 WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(int32_t),
                                                 (uint8_t *)&st_write_msg,
                                                 OAL_FALSE, OAL_PTR_NULL);

    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_vap_log_level::err[%d]!}", ul_ret);
        return ul_ret;
    }

    return ul_ret;
}

OAL_STATIC uint32_t wal_hipriv_set_random_mac_addr_scan(oal_net_device_stru *pst_net_dev,
    int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;
    int32_t l_ret;
    uint8_t uc_rand_mac_addr_scan_switch;

    /* sh hipriv.sh "Hisilicon0 random_mac_addr_scan 0|1(开关)" */
    /* 获取帧方向 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_random_mac_addr_scan::get switch err[%d]!}", ul_ret);
        return ul_ret;
    }
    pc_param += ul_off_set;
    uc_rand_mac_addr_scan_switch = (uint8_t)oal_atoi(ac_name);

    /* 开关的取值范围为0|1,做参数合法性判断 */
    if (uc_rand_mac_addr_scan_switch > 1) {
        oam_error_log1(0, OAM_SF_ANY, "{wal_hipriv_set_random_mac_addr_scan::param is error, switch_value[%d]!}",
                       uc_rand_mac_addr_scan_switch);
        return OAL_FAIL;
    }

    /* 抛事件到wal层处理 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_RANDOM_MAC_ADDR_SCAN, OAL_SIZEOF(uint32_t));
    *((int32_t *)(st_write_msg.auc_value)) = (uint32_t)uc_rand_mac_addr_scan_switch;

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(uint32_t),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_random_mac_addr_scan::err[%d]!}", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_set_uapsd_cap(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_tmp;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    int32_t l_ret;
    uint32_t ul_ret;

    /* 此处将解析出"1"或"0"存入ac_name */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_uapsd_cap::wal_get_cmd_one_arg err[%d]}", ul_ret);
        return ul_ret;
    }

    /* 针对解析出的不同命令，对UAPSD开关进行不同的设置 */
    if (0 == (oal_strcmp("0", ac_name))) {
        l_tmp = 0;
    } else if (0 == (oal_strcmp("1", ac_name))) {
        l_tmp = 1;
    } else {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_uapsd_cap::log switch cmd error[%x]!}", (uintptr_t)ac_name);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    /* 抛事件到wal层处理 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_UAPSD_EN, OAL_SIZEOF(int32_t));
    *((int32_t *)(st_write_msg.auc_value)) = l_tmp; /* 设置配置命令参数 */

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(int32_t),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_event_switch::err [%d]!}", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_add_user(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;
    int32_t l_ret;
    mac_cfg_add_user_param_stru *pst_add_user_param = OAL_PTR_NULL;
    mac_cfg_add_user_param_stru st_add_user_param; /* 临时保存获取的use的信息 */
    uint32_t ul_get_addr_idx;

    /*
        设置添加用户的配置命令: hipriv "vap0 add_user xx xx xx xx xx xx(mac地址) 0 | 1(HT能力位) "
        该命令针对某一个VAP
    */
    /* 获取mac地址 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_add_user::get cmd one arg err[%d]!}", ul_ret);
        return ul_ret;
    }

    memset_s((uint8_t *)&st_add_user_param, OAL_SIZEOF(st_add_user_param), 0, OAL_SIZEOF(st_add_user_param));
    oal_strtoaddr(ac_name, OAL_SIZEOF(ac_name), st_add_user_param.auc_mac_addr, WLAN_MAC_ADDR_LEN);
    /* 偏移，取下一个参数 */
    pc_param = pc_param + ul_off_set;

    /* 获取用户的HT标识 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_add_user::get cmd one arg err[%d]!}", ul_ret);
        return ul_ret;
    }

    /* 针对解析出的不同命令，对user的HT字段进行不同的设置 */
    if (0 == (oal_strcmp("0", ac_name))) {
        st_add_user_param.en_ht_cap = 0;
    } else if (0 == (oal_strcmp("1", ac_name))) {
        st_add_user_param.en_ht_cap = 1;
    } else {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_add_user::the mod switch cmd error [%x]!}", (uintptr_t)ac_name);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    /* 抛事件到wal层处理 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_ADD_USER, OAL_SIZEOF(mac_cfg_add_user_param_stru));

    /* 设置配置命令参数 */
    pst_add_user_param = (mac_cfg_add_user_param_stru *)(st_write_msg.auc_value);
    for (ul_get_addr_idx = 0; ul_get_addr_idx < WLAN_MAC_ADDR_LEN; ul_get_addr_idx++) {
        pst_add_user_param->auc_mac_addr[ul_get_addr_idx] = st_add_user_param.auc_mac_addr[ul_get_addr_idx];
    }
    pst_add_user_param->en_ht_cap = st_add_user_param.en_ht_cap;

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_add_user_param_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_add_user::err[%d]!}", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_del_user(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;
    int32_t l_ret;
    mac_cfg_del_user_param_stru *pst_del_user_param = OAL_PTR_NULL;
    mac_cfg_del_user_param_stru st_del_user_param; /* 临时保存获取的use的信息 */
    uint32_t ul_get_addr_idx;

    /*
        设置删除用户的配置命令: hipriv "vap0 del_user xx xx xx xx xx xx(mac地址)"
        该命令针对某一个VAP
    */
    /* 获取mac地址 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_del_user::get cmd one arg err[%d]!}", ul_ret);
        return ul_ret;
    }

    memset_s((uint8_t *)&st_del_user_param, OAL_SIZEOF(st_del_user_param), 0, OAL_SIZEOF(st_del_user_param));
    oal_strtoaddr(ac_name, OAL_SIZEOF(ac_name), st_del_user_param.auc_mac_addr, WLAN_MAC_ADDR_LEN);

    /* 抛事件到wal层处理 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_DEL_USER, OAL_SIZEOF(mac_cfg_add_user_param_stru));

    /* 设置配置命令参数 */
    pst_del_user_param = (mac_cfg_add_user_param_stru *)(st_write_msg.auc_value);
    for (ul_get_addr_idx = 0; ul_get_addr_idx < WLAN_MAC_ADDR_LEN; ul_get_addr_idx++) {
        pst_del_user_param->auc_mac_addr[ul_get_addr_idx] = st_del_user_param.auc_mac_addr[ul_get_addr_idx];
    }

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_add_user_param_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_del_user::err[%d]!}", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_user_info(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;
    mac_cfg_user_info_param_stru *pst_user_info_param = OAL_PTR_NULL;
    uint8_t auc_mac_addr[6] = { 0 }; /* 临时保存获取的use的mac地址信息 */
    uint8_t uc_char_index;
    uint16_t us_user_idx;

    /* 去除字符串的空格 */
    pc_param++;

    /* 获取mac地址,16进制转换 */
    for (uc_char_index = 0; uc_char_index < 12; uc_char_index++) {
        if (*pc_param == ':') {
            pc_param++;
            if (uc_char_index != 0) {
                uc_char_index--;
            }

            continue;
        }

        auc_mac_addr[uc_char_index / 2] =
            (uint8_t)(auc_mac_addr[uc_char_index / 2] * 16 * (uc_char_index % 2) +
                        oal_strtohex(pc_param));
        pc_param++;
    }

    /* 抛事件到wal层处理 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_USER_INFO, OAL_SIZEOF(mac_cfg_user_info_param_stru));

    /* 根据mac地址找用户 */
    pst_mac_vap = oal_net_dev_priv(pst_net_dev);

    l_ret = (int32_t)mac_vap_find_user_by_macaddr(pst_mac_vap, auc_mac_addr, &us_user_idx);
    if (l_ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_user_info::no such user!}");
        return OAL_FAIL;
    }

    /* 设置配置命令参数 */
    pst_user_info_param = (mac_cfg_user_info_param_stru *)(st_write_msg.auc_value);
    pst_user_info_param->us_user_idx = us_user_idx;

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_user_info_param_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_hipriv_user_info::err [%d]!}", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


uint32_t wal_hipriv_set_mcast_data_dscr_param(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    uint32_t ul_ret;
    int32_t l_ret;
    mac_cfg_set_dscr_param_stru *pst_set_dscr_param = OAL_PTR_NULL;
    wal_dscr_param_enum_uint8 en_param_index;
    int8_t ac_arg[WAL_HIPRIV_CMD_NAME_MAX_LEN];

    /* 抛事件到wal层处理 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_DSCR, OAL_SIZEOF(mac_cfg_set_dscr_param_stru));

    /* 解析并设置配置命令参数 */
    pst_set_dscr_param = (mac_cfg_set_dscr_param_stru *)(st_write_msg.auc_value);

    /* 获取描述符字段设置命令字符串 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_mcast_data_dscr_param::get cmd one arg err[%d]!}", ul_ret);
        return ul_ret;
    }
    pc_param += ul_off_set;

    /* 解析是设置哪一个字段 */
    for (en_param_index = 0; en_param_index < WAL_DSCR_PARAM_BUTT; en_param_index++) {
        if (!oal_strcmp(pauc_tx_dscr_param_name[en_param_index], ac_arg)) {
            break;
        }
    }

    /* 检查命令是否打错 */
    if (en_param_index == WAL_DSCR_PARAM_BUTT) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_set_mcast_data_dscr_param::no such param for tx dscr!}");
        return OAL_FAIL;
    }

    pst_set_dscr_param->uc_function_index = en_param_index;

    /* 配置速率、空间流数、带宽 */
    if (en_param_index >= WAL_DSCR_PARAM_RATE && en_param_index <= WAL_DSCR_PARAM_BW) {
        ul_ret = wal_hipriv_process_rate_params(pst_net_dev, pc_param, pst_set_dscr_param);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_mcast_data_dscr_param::ucast_params err[%d]}", ul_ret);
            return ul_ret;
        }
    } else {
        /* 解析要设置为多大的值 */
        ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_mcast_data_dscr_param::get cmd one arg err[%d]}", ul_ret);
            return ul_ret;
        }
        pc_param += ul_off_set;
        pst_set_dscr_param->l_value = oal_strtol(ac_arg, OAL_PTR_NULL, 0);
    }

    /* 组播数据帧描述符设置 tpye = MAC_VAP_CONFIG_MCAST_DATA */
    pst_set_dscr_param->en_type = MAC_VAP_CONFIG_MCAST_DATA;

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_set_dscr_param_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE, OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_mcast_data_dscr_param::err [%d]!}", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


uint32_t wal_hipriv_set_rate(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    uint32_t ul_ret;
    int32_t l_ret;
    mac_cfg_non_ht_rate_stru *pst_set_rate_param = OAL_PTR_NULL;
    wlan_legacy_rate_value_enum_uint8 en_rate_index;
    int8_t ac_arg[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;

    /* 抛事件到wal层处理 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_RATE, OAL_SIZEOF(mac_cfg_non_ht_rate_stru));

    /* 解析并设置配置命令参数 */
    pst_set_rate_param = (mac_cfg_non_ht_rate_stru *)(st_write_msg.auc_value);

    /* 获取速率值字符串 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_rate::get cmd one arg err[%d]!}", ul_ret);
        return ul_ret;
    }

    /* 解析是设置为哪一级速率 */
    for (en_rate_index = 0; en_rate_index < WLAN_LEGACY_RATE_VALUE_BUTT; en_rate_index++) {
        if (!oal_strcmp(pauc_non_ht_rate_tbl[en_rate_index], ac_arg)) {
            break;
        }
    }

    /* 解析要设置为多大的值 */
    pst_set_rate_param->en_rate = en_rate_index;

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_non_ht_rate_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_TRUE,
                                   &pst_rsp_msg);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_rate::err [%d]!}", l_ret);
        return (uint32_t)l_ret;
    }
    /* 读取返回的错误码 */
    ul_ret = wal_check_and_release_msg_resp(pst_rsp_msg);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_DFR, "{wal_hipriv_set_rate fail, err code[%u]!}", ul_ret);
        return ul_ret;
    }
    return OAL_SUCC;
}


uint32_t wal_hipriv_set_mcs(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    uint32_t ul_ret;
    int32_t l_ret;
    mac_cfg_tx_comp_stru *pst_set_mcs_param = OAL_PTR_NULL;
    int32_t l_mcs;
    int8_t ac_arg[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    int32_t l_idx = 0;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;

    /* 抛事件到wal层处理 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_MCS, OAL_SIZEOF(mac_cfg_tx_comp_stru));

    /* 解析并设置配置命令参数 */
    pst_set_mcs_param = (mac_cfg_tx_comp_stru *)(st_write_msg.auc_value);

    /* 获取速率值字符串 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_mcs::get cmd one arg err[%d]!}", ul_ret);
        return ul_ret;
    }

    /* 输入命令合法性检测 */
    while ('\0' != ac_arg[l_idx]) {
        if (isdigit(ac_arg[l_idx])) {
            l_idx++;
            continue;
        } else {
            l_idx++;
            oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_set_mcs::input illegal!}");
            return OAL_ERR_CODE_INVALID_CONFIG;
        }
    }

    /* 解析要设置为多大的值 */
    l_mcs = oal_atoi(ac_arg);

    if (l_mcs < WAL_HIPRIV_HT_MCS_MIN || l_mcs > WAL_HIPRIV_HT_MCS_MAX) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_mcs::input val out of range [%d]!}", l_mcs);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    pst_set_mcs_param->uc_param = (uint8_t)l_mcs;

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_tx_comp_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_TRUE,
                                   &pst_rsp_msg);
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_mcs::err [%d]!}", l_ret);
        return (uint32_t)l_ret;
    }
    /* 读取返回的错误码 */
    ul_ret = wal_check_and_release_msg_resp(pst_rsp_msg);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_DFR, "{wal_hipriv_set_mcs fail, err code[%u]!}", ul_ret);
        return ul_ret;
    }
    return OAL_SUCC;
}


uint32_t wal_hipriv_set_mcsac(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    uint32_t ul_ret;
    int32_t l_ret;
    mac_cfg_tx_comp_stru *pst_set_mcs_param = OAL_PTR_NULL;
    int32_t l_mcs;
    int8_t ac_arg[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    int32_t l_idx = 0;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;

    /* 抛事件到wal层处理 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_MCSAC, OAL_SIZEOF(mac_cfg_tx_comp_stru));

    /* 解析并设置配置命令参数 */
    pst_set_mcs_param = (mac_cfg_tx_comp_stru *)(st_write_msg.auc_value);

    /* 获取速率值字符串 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_mcsac::get cmd one arg err[%d]!}", ul_ret);
        return ul_ret;
    }

    /* 输入命令合法性检测 */
    while ('\0' != ac_arg[l_idx]) {
        if (isdigit(ac_arg[l_idx])) {
            l_idx++;
            continue;
        } else {
            l_idx++;
            oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_set_mcsac::input illegal!}");
            return OAL_ERR_CODE_INVALID_CONFIG;
        }
    }

    /* 解析要设置为多大的值 */
    l_mcs = oal_atoi(ac_arg);

    if (l_mcs < WAL_HIPRIV_VHT_MCS_MIN || l_mcs > WAL_HIPRIV_VHT_MCS_MAX) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_mcs::input val out of range [%d]!}", l_mcs);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    pst_set_mcs_param->uc_param = (uint8_t)l_mcs;

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_tx_comp_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_TRUE,
                                   &pst_rsp_msg);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_mcsac::err [%d]!}", l_ret);
        return (uint32_t)l_ret;
    }
    /* 读取返回的错误码 */
    ul_ret = wal_check_and_release_msg_resp(pst_rsp_msg);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_DFR, "{wal_hipriv_set_mcsac fail, err code[%u]!}", ul_ret);
        return ul_ret;
    }

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_11AX

uint32_t wal_hipriv_set_mcsax(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    uint32_t ul_ret;
    int32_t l_ret;
    mac_cfg_tx_comp_stru *pst_set_mcs_param;
    int32_t l_mcs;
    int8_t ac_arg[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    int32_t l_idx = 0;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;

    /* 抛事件到wal层处理 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_MCSAX, OAL_SIZEOF(mac_cfg_tx_comp_stru));

    /* 解析并设置配置命令参数 */
    pst_set_mcs_param = (mac_cfg_tx_comp_stru *)(st_write_msg.auc_value);

    /* 获取速率值字符串 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_mcsax::get cmd one arg err[%d]!}", ul_ret);
        return ul_ret;
    }

    /* 输入命令合法性检测 */
    while ('\0' != ac_arg[l_idx]) {
        if (isdigit(ac_arg[l_idx])) {
            l_idx++;
            continue;
        } else {
            l_idx++;
            oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_set_mcsax::input illegal!}");
            return OAL_ERR_CODE_INVALID_CONFIG;
        }
    }

    /* 解析要设置为多大的值 */
    l_mcs = oal_atoi(ac_arg);

    if (l_mcs < WAL_HIPRIV_HE_MCS_MIN || l_mcs > WAL_HIPRIV_HE_MCS_MAX) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_mcsax::input val out of range [%d]!}", l_mcs);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    pst_set_mcs_param->uc_param = (uint8_t)l_mcs;

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_tx_comp_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_TRUE,
                                   &pst_rsp_msg);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_mcsax::err [%d]!}", l_ret);
        return (uint32_t)l_ret;
    }
    /* 读取返回的错误码 */
    ul_ret = wal_check_and_release_msg_resp(pst_rsp_msg);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_DFR, "{wal_hipriv_set_mcsax fail, err code[%u]!}", ul_ret);
        return ul_ret;
    }

    return OAL_SUCC;
}
#ifdef  _PRE_WLAN_FEATURE_11AX_ER_SU_DCM

uint32_t wal_hipriv_set_mcsax_er(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    uint32_t ul_ret;
    int32_t l_ret;
    mac_cfg_tx_comp_stru *pst_set_mcs_param;
    int32_t l_mcs;
    int8_t ac_arg[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    int32_t l_idx = 0;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;

    /* 抛事件到wal层处理 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_MCSAX_ER, OAL_SIZEOF(mac_cfg_tx_comp_stru));

    /* 解析并设置配置命令参数 */
    pst_set_mcs_param = (mac_cfg_tx_comp_stru *)(st_write_msg.auc_value);

    /* 获取速率值字符串 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_mcsax_er::get cmd one arg err[%d]!}", ul_ret);
        return ul_ret;
    }

    /* 输入命令合法性检测 */
    while ('\0' != ac_arg[l_idx]) {
        if (isdigit(ac_arg[l_idx])) {
            l_idx++;
            continue;
        } else {
            l_idx++;
            oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_set_mcsax_er::input illegal!}");
            return OAL_ERR_CODE_INVALID_CONFIG;
        }
    }

    /* 解析要设置为多大的值 */
    l_mcs = oal_atoi(ac_arg);
    if (l_mcs < WAL_HIPRIV_HE_ER_MCS_MIN || l_mcs > WAL_HIPRIV_HE_ER_MCS_MAX) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_mcsax_er::input val out of range [%d]!}", l_mcs);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    pst_set_mcs_param->uc_param = (uint8_t)l_mcs;

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_tx_comp_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_TRUE,
                                   &pst_rsp_msg);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_mcsax_er::err [%d]!}", l_ret);
        return (uint32_t)l_ret;
    }
    /* 读取返回的错误码 */
    ul_ret = wal_check_and_release_msg_resp(pst_rsp_msg);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_DFR, "{wal_hipriv_set_mcsax_er fail, err code[%u]!}", ul_ret);
        return ul_ret;
    }

    return OAL_SUCC;
}

uint32_t _wal_hipriv_set_mcsax_er(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    if (g_wlan_spec_cfg->feature_11ax_er_su_dcm_is_open) {
        return wal_hipriv_set_mcsax_er(pst_net_dev, pc_param);
    }

    return OAL_SUCC;
}
#endif
#endif


OAL_STATIC uint32_t wal_hipriv_set_bw(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    uint32_t ul_ret;
    int32_t l_ret;
    mac_cfg_tx_comp_stru *pst_set_bw_param = OAL_PTR_NULL;
    wlan_bandwith_cap_enum_uint8 en_bw_index;
    int8_t ac_arg[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;

    /* 抛事件到wal层处理 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_BW, OAL_SIZEOF(mac_cfg_tx_comp_stru));

    /* 解析并设置配置命令参数 */
    pst_set_bw_param = (mac_cfg_tx_comp_stru *)(st_write_msg.auc_value);

    /* 获取带宽值字符串 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_bw::get cmd one arg err[%d]!}", ul_ret);
        return ul_ret;
    }

    /* 解析要设置为多大的值 */
    for (en_bw_index = 0; en_bw_index < WLAN_BANDWITH_CAP_BUTT; en_bw_index++) {
        if (!oal_strcmp(pauc_bw_tbl[en_bw_index], ac_arg)) {
            break;
        }
    }

    /* 检查命令是否打错 */
    if (en_bw_index >= WLAN_BANDWITH_CAP_BUTT) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_set_bw::not support this bandwidth!}");
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    pst_set_bw_param->uc_param = (uint8_t)(en_bw_index);

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_tx_comp_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_TRUE,
                                   &pst_rsp_msg);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_bw::err [%d]!}", l_ret);
        return (uint32_t)l_ret;
    }
    /* 读取返回的错误码 */
    ul_ret = wal_check_and_release_msg_resp(pst_rsp_msg);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_DFR, "{wal_hipriv_set_bw fail, err code[%u]!}", ul_ret);
        return ul_ret;
    }
    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_always_tx(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    uint32_t ul_ret;
    int32_t l_ret;
    mac_cfg_tx_comp_stru *pst_set_bcast_param;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint8_t en_tx_flag;
    mac_rf_payload_enum_uint8 en_payload_flag = RF_PAYLOAD_RAND;
    uint32_t ul_len = 2000;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;

    /* 抛事件到wal层处理 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_ALWAYS_TX, OAL_SIZEOF(mac_cfg_tx_comp_stru));

    /* 解析并设置配置命令参数 */
    pst_set_bcast_param = (mac_cfg_tx_comp_stru *)(st_write_msg.auc_value);

    /* 获取常发模式开关标志 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_always_tx::get cmd one arg err[%d]!}", ul_ret);
        return ul_ret;
    }
    en_tx_flag = (uint8_t)oal_atoi(ac_name);
    if (en_tx_flag > HAL_ALWAYS_TX_MPDU) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_always_tx::input should be 0 or 1.}");
        return OAL_ERR_CODE_INVALID_CONFIG;
    }
    /* 关闭的情况下不需要解析后面的参数 */
    if (en_tx_flag != HAL_ALWAYS_TX_DISABLE) {
        /* ack_policy参数后续扩充 */
        /* 获取payload_flag参数 */
        pc_param = pc_param + ul_off_set;
        ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
        /* 若后面payload_flag和len参数没有设置，采用默认RF_PAYLOAD_RAND 2000 */
        if (ul_ret == OAL_SUCC) {
            en_payload_flag = (uint8_t)oal_atoi(ac_name);
            if (en_payload_flag >= RF_PAYLOAD_BUTT) {
                oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_always_tx::payload flag err[%d]!}", en_payload_flag);
                return OAL_ERR_CODE_INVALID_CONFIG;
            }

            /* 获取len参数 */
            pc_param = pc_param + ul_off_set;
            ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
            if (ul_ret != OAL_SUCC) {
                oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_always_tx::get cmd one arg err[%d]!}", ul_ret);
                return ul_ret;
            }
            ul_len = (uint16_t)oal_atoi(ac_name);
            if (ul_len > 65535) {
                oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_always_tx::len [%u] overflow!}", ul_len);
                return OAL_ERR_CODE_INVALID_CONFIG;
            }
        }
    }

    pst_set_bcast_param->en_payload_flag = en_payload_flag;
    pst_set_bcast_param->ul_payload_len = ul_len;
    pst_set_bcast_param->uc_param = en_tx_flag;
    pst_set_bcast_param->write_index = 0;
    oam_warning_log2(0, OAM_SF_ANY, "{wal_hipriv_always_tx::tx_flag[%d],len[%d]!}", en_tx_flag, ul_len);
    l_ret = wal_send_cfg_event(pst_net_dev, WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_tx_comp_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_TRUE,
                                   &pst_rsp_msg);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_always_tx::err [%d]!}", l_ret);
        return (uint32_t)l_ret;
    }

    /* 读取返回的错误码 */
    ul_ret = wal_check_and_release_msg_resp(pst_rsp_msg);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_DFR, "{wal_hipriv_always_tx fail, err code[%u]!}", ul_ret);
        return ul_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_always_tx_aggr_num(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t *pul_num;
    uint32_t ul_ret;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_off_set;
    int32_t l_ret;

    /* 抛事件到wal层处理 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_ALWAYS_TX_AGGR_NUM, OAL_SIZEOF(uint32_t));

    /* 获取参数配置 */
    pul_num = (uint32_t *)(st_write_msg.auc_value);

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_always_tx_aggr_num::get cmd one arg err[%d]!}", ul_ret);
        return ul_ret;
    }
    *pul_num = (uint32_t)oal_atoi(ac_name);
    oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_always_tx_aggr_num::altx aggr num[%d]!}", *pul_num);
    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(uint32_t),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_always_tx_aggr_num::err [%d]!}", l_ret);
        return (uint32_t)l_ret;
    }
    return OAL_SUCC;
}

OAL_STATIC uint32_t wal_hipriv_always_tx_num(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t *pul_num;
    uint32_t ul_ret;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_off_set;
    int32_t l_ret;

    /* 抛事件到wal层处理 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_ALWAYS_TX_NUM, OAL_SIZEOF(uint32_t));
    /* 获取参数配置 */
    pul_num = (uint32_t *)(st_write_msg.auc_value);

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_packet_xmit::get cmd one arg err[%d]!}", ul_ret);
        return ul_ret;
    }
    *pul_num = (uint32_t)oal_atoi(ac_name);

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(uint32_t),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_always_tx::err [%d]!}", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_set_ru_index(oal_net_device_stru *net_dev, int8_t *param)
{
    wal_msg_write_stru write_msg;
    uint32_t off_set;
    uint32_t ret;
    mac_cfg_tx_comp_stru *set_param;
    int8_t name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    wal_msg_stru *rsp_msg = OAL_PTR_NULL;

    /* 抛事件到wal层处理 */
    WAL_WRITE_MSG_HDR_INIT(&write_msg, WLAN_CFGID_SET_RU_INDEX, OAL_SIZEOF(mac_cfg_tx_comp_stru));
    /* 解析并设置配置命令参数 */
    set_param = (mac_cfg_tx_comp_stru *)(write_msg.auc_value);

    ret = wal_get_cmd_one_arg(param, name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &off_set);
    if (ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_ru_index::get cmd one arg err[%d]!}", ret);
        return ret;
    }

    for (set_param->uc_param = 0; set_param->uc_param < WLAN_HE_RU_SIZE_BUTT; set_param->uc_param++) {
        if (!oal_strcmp(g_ru_size_tbl[set_param->uc_param], name)) {
            break;
        }
    }
    /* 检查命令是否打错 */
    if (set_param->uc_param >= WLAN_HE_RU_SIZE_BUTT) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_set_ru_index::not support this ru size!}");
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    param = param + off_set;
    ret = wal_get_cmd_one_arg(param, name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &off_set);
    if (ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_ru_index::get cmd one arg err[%d]!}", ret);
        return ret;
    }
    set_param->en_payload_flag = (uint8_t)oal_atoi(name);

    param = param + off_set;
    ret = wal_get_cmd_one_arg(param, name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &off_set);
    if (ret != OAL_SUCC) {
        return ret;
    }
    set_param->en_protocol_mode = (uint8_t)oal_atoi(name);

    ret = wal_send_cfg_event(net_dev, WAL_MSG_TYPE_WRITE, WAL_MSG_WRITE_MSG_HDR_LENGTH +
        OAL_SIZEOF(mac_cfg_tx_comp_stru), (uint8_t *)&write_msg, OAL_TRUE, &rsp_msg);

    if (oal_unlikely(ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_ru_index::err [%d]!}", ret);
        return (uint32_t)ret;
    }

    /* 读取返回的错误码 */
    ret = wal_check_and_release_msg_resp(rsp_msg);
    if (ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_DFR, "{wal_hipriv_set_ru_index fail, err code[%u]!}", ret);
        return ret;
    }

    return OAL_SUCC;
}



OAL_STATIC uint32_t wal_hipriv_always_rx(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    return wal_ioctl_set_al_rx(pst_net_dev, pc_param);
}
#ifdef _PRE_WLAN_FEATURE_MONITOR

OAL_STATIC uint32_t wal_hipriv_set_sniffer(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    return wal_ioctl_set_sniffer(pst_net_dev, pc_param);
}


OAL_STATIC uint32_t wal_hipriv_enable_monitor_mode(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    return wal_ioctl_set_monitor_mode(pst_net_dev, pc_param);
}
#endif


OAL_STATIC int wal_ioctl_get_iwname(oal_net_device_stru *pst_net_dev,
                                    oal_iw_request_info_stru *pst_info,
                                    oal_iwreq_data_union *pst_wrqu,
                                    char *pc_extra)
{
    int8_t ac_iwname[] = "IEEE 802.11";

    if ((pst_net_dev == OAL_PTR_NULL) || (pst_wrqu == OAL_PTR_NULL)) {
        oam_error_log2(0, OAM_SF_ANY, "{wal_ioctl_get_iwname::param null, pst_net_dev = %p, pc_name = %p.}",
                       (uintptr_t)pst_net_dev, (uintptr_t)pst_wrqu);
        return -OAL_EINVAL;
    }

    if (EOK != memcpy_s(pst_wrqu->name, OAL_IF_NAME_SIZE, ac_iwname, OAL_SIZEOF(ac_iwname))) {
        oam_error_log0(0, OAM_SF_ANY, "wal_ioctl_get_iwname::memcpy fail!");
        return -OAL_EINVAL;
    }
    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_find_cmd(int8_t *pc_cmd_name,
    uint8_t uc_cmd_name_len, wal_hipriv_cmd_entry_stru **pst_cmd_id)
{
    uint32_t en_cmd_idx;
    int l_ret;
    *pst_cmd_id = NULL;

    if (oal_unlikely(oal_any_null_ptr2(pc_cmd_name, pst_cmd_id))) {
        oam_error_log2(0, OAM_SF_ANY, "{wal_hipriv_find_cmd::pc_cmd_name/puc_cmd_id null ptr error [%x] [%x]!}",
            (uintptr_t)pc_cmd_name, (uintptr_t)pst_cmd_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (uc_cmd_name_len > WAL_HIPRIV_CMD_NAME_MAX_LEN) {
        return OAL_FAIL;
    }

    for (en_cmd_idx = 0; en_cmd_idx < oal_array_size(g_ast_hipriv_cmd); en_cmd_idx++) {
        l_ret = oal_strcmp(g_ast_hipriv_cmd[en_cmd_idx].pc_cmd_name, pc_cmd_name);

        if (l_ret == 0) {
            *pst_cmd_id = (wal_hipriv_cmd_entry_stru *)&g_ast_hipriv_cmd[en_cmd_idx];
            oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_find_cmd::wal_hipriv_find_cmd en_cmd_idx = %d.}", en_cmd_idx);

            return OAL_SUCC;
        }
    }

#ifdef _PRE_WLAN_CFGID_DEBUG
    for (en_cmd_idx = 0; en_cmd_idx < wal_hipriv_get_debug_cmd_size(); en_cmd_idx++) {
        l_ret = oal_strcmp(g_ast_hipriv_cmd_debug[en_cmd_idx].pc_cmd_name, pc_cmd_name);

        if (l_ret == 0) {
            *pst_cmd_id = (wal_hipriv_cmd_entry_stru *)&g_ast_hipriv_cmd_debug[en_cmd_idx];
            oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_find_cmd::wal_hipriv_find_cmd en_cmd_idx:%d.}", en_cmd_idx);

            return OAL_SUCC;
        }
    }
#endif

    oam_io_printk("cmd name[%s] is not exist. \r\n", pc_cmd_name);
    return OAL_FAIL;
}


OAL_STATIC uint32_t wal_hipriv_get_cmd_net_dev(int8_t *pc_cmd,
    oal_net_device_stru **ppst_net_dev, uint32_t *pul_off_set)
{
    oal_net_device_stru *pst_net_dev = OAL_PTR_NULL;
    int8_t ac_dev_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;

    if (oal_any_null_ptr3(pc_cmd, ppst_net_dev, pul_off_set)) {
        oam_error_log3(0, OAM_SF_ANY, "{wal_hipriv_get_cmd_net_dev::cmd/ppst_net_dev/pul_off_set null[%x][%x][%x]}",
            (uintptr_t)pc_cmd, (uintptr_t)ppst_net_dev, (uintptr_t)pul_off_set);
        return OAL_ERR_CODE_PTR_NULL;
    }

    ul_ret = wal_get_cmd_one_arg(pc_cmd, ac_dev_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, pul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_get_cmd_net_dev::get cmd one arg err[%d]!}", ul_ret);
        return ul_ret;
    }

    /* 根据dev_name找到dev */
    pst_net_dev = wal_config_get_netdev(ac_dev_name, OAL_STRLEN(ac_dev_name));
    if (pst_net_dev == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_get_cmd_net_dev::wal_config_get_netdev return null ptr!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 调用oal_dev_get_by_name后，必须调用oal_dev_put使net_dev的引用计数减一 */
    oal_dev_put(pst_net_dev);

    *ppst_net_dev = pst_net_dev;

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_get_cmd_id(int8_t *pc_cmd,
    wal_hipriv_cmd_entry_stru **pst_cmd_id, uint32_t *pul_off_set)
{
    int8_t ac_cmd_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;

    if (oal_unlikely(oal_any_null_ptr3(pc_cmd, pst_cmd_id, pul_off_set))) {
        oam_error_log3(0, OAM_SF_ANY,
            "{wal_hipriv_get_cmd_id::pc_cmd/puc_cmd_id/pul_off_set null ptr error [%x] [%x] [%x]!}",
            (uintptr_t)pc_cmd, (uintptr_t)pst_cmd_id, (uintptr_t)pul_off_set);
        return OAL_ERR_CODE_PTR_NULL;
    }

    ul_ret = wal_get_cmd_one_arg(pc_cmd, ac_cmd_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, pul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_get_cmd_id::get cmd one arg err[%d]!}", ul_ret);
        return ul_ret;
    }

    /* 根据命令名找到命令枚举 */
    ul_ret = wal_hipriv_find_cmd(ac_cmd_name, sizeof(ac_cmd_name), pst_cmd_id);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_get_cmd_id::wal_hipriv_find_cmd return error cod [%d]!}", ul_ret);
        return ul_ret;
    }

    return OAL_SUCC;
}


uint32_t wal_hipriv_parse_cmd(int8_t *pc_cmd)
{
    oal_net_device_stru *pst_net_dev = OAL_PTR_NULL;
    wal_hipriv_cmd_entry_stru *pst_hipriv_cmd_entry = NULL;
    uint32_t ul_off_set = 0;
    uint32_t ul_ret;
    if (oal_unlikely(pc_cmd == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_hipriv_parse_cmd::pc_cmd null ptr error!}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    if (wal_process_cmd(pc_cmd) == OAL_SUCC) {
        return OAL_SUCC;
    }
    /*
        cmd格式约束
        网络设备名 命令      参数   Hisilicon0 create vap0
        1~15Byte   1~15Byte
    */
    ul_ret = wal_hipriv_get_cmd_net_dev(pc_cmd, &pst_net_dev, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_parse_cmd::wal_hipriv_get_cmd_net_dev error[%d]!}", ul_ret);
        return ul_ret;
    }

    pc_cmd += ul_off_set;
    ul_ret = wal_hipriv_get_cmd_id(pc_cmd, &pst_hipriv_cmd_entry, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_parse_cmd::wal_hipriv_get_cmd_id  error[%d]!}", ul_ret);
        return ul_ret;
    }

    pc_cmd += ul_off_set;
    /* 调用命令对应的函数 */
    ul_ret = pst_hipriv_cmd_entry->p_func(pst_net_dev, pc_cmd);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_parse_cmd::g_ast_hipriv_cmd  error[%d]!}", ul_ret);
        return ul_ret;
    }

    return OAL_SUCC;
}

#ifdef _PRE_CONFIG_CONN_HISI_SYSFS_SUPPORT

OAL_STATIC oal_ssize_t wal_hipriv_sys_write(struct kobject *dev, struct kobj_attribute *attr,
    const char *pc_buffer, oal_size_t count)
{
    int8_t *pc_cmd;
    uint32_t ul_ret;
    uint32_t ul_len = (uint32_t)count;

    if (ul_len > WAL_HIPRIV_CMD_MAX_LEN) {
        oam_error_log1(0, OAM_SF_ANY, "{wal_hipriv_sys_write::ul_len>WAL_HIPRIV_CMD_MAX_LEN, ul_len [%d]!}", ul_len);
        return -OAL_EINVAL;
    }

    pc_cmd = oal_mem_alloc_m(OAL_MEM_POOL_ID_LOCAL, WAL_HIPRIV_CMD_MAX_LEN, OAL_TRUE);
    if (oal_unlikely(pc_cmd == OAL_PTR_NULL)) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_proc_write::alloc mem return null ptr!}");
        return -OAL_ENOMEM;
    }

    memset_s(pc_cmd, WAL_HIPRIV_CMD_MAX_LEN, 0, WAL_HIPRIV_CMD_MAX_LEN);

    if (EOK != memcpy_s(pc_cmd, WAL_HIPRIV_CMD_MAX_LEN, pc_buffer, ul_len)) {
        oam_error_log0(0, OAM_SF_ANY, "wal_hipriv_sys_write::memcpy fail!");
        oal_mem_free_m(pc_cmd, OAL_TRUE);
        return -OAL_EINVAL;
    }

    pc_cmd[ul_len - 1] = '\0';

    ul_ret = wal_hipriv_parse_cmd(pc_cmd);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_proc_write::parse cmd return err code[%d]!}", ul_ret);
    }

    oal_mem_free_m(pc_cmd, OAL_TRUE);

    return (int32_t)ul_len;
}


#define SYS_READ_MAX_STRING_LEN (4096 - 40) /* 当前命令字符长度20字节内，预留40保证不会超出 */
OAL_STATIC oal_ssize_t wal_hipriv_sys_read(struct kobject *dev, struct kobj_attribute *attr, char *pc_buffer)
{
    uint32_t ul_cmd_idx;
    uint32_t buff_index = 0;

    for (ul_cmd_idx = 0; ul_cmd_idx < oal_array_size(g_ast_hipriv_cmd); ul_cmd_idx++) {
        buff_index += snprintf_s(pc_buffer + buff_index, (SYS_READ_MAX_STRING_LEN - buff_index),
                                 (SYS_READ_MAX_STRING_LEN - buff_index) - 1,
                                 "\t%s\n", g_ast_hipriv_cmd[ul_cmd_idx].pc_cmd_name);

        if (buff_index > SYS_READ_MAX_STRING_LEN) {
            oam_error_log0(0, OAM_SF_ANY, "wal_hipriv_sys_read::snprintf_s error!");
            return buff_index;
        }
    }
#ifdef _PRE_WLAN_CFGID_DEBUG
    for (ul_cmd_idx = 0; ul_cmd_idx < wal_hipriv_get_debug_cmd_size(); ul_cmd_idx++) {
        buff_index += snprintf_s(pc_buffer + buff_index, (SYS_READ_MAX_STRING_LEN - buff_index),
                                 (SYS_READ_MAX_STRING_LEN - buff_index) - 1,
                                 "\t%s\n", g_ast_hipriv_cmd_debug[ul_cmd_idx].pc_cmd_name);

        if (buff_index > SYS_READ_MAX_STRING_LEN) {
            oam_error_log0(0, OAM_SF_ANY, "wal_hipriv_sys_read::snprintf_s error!");
            break;
        }
    }
#endif

    return buff_index;
}

#endif /* _PRE_OS_VERSION_LINUX */

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 10, 44))
// use sys filesystem instead
#else

OAL_STATIC int32_t wal_hipriv_proc_write(oal_file_stru *pst_file,
    const int8_t *pc_buffer, uint32_t ul_len, void *p_data)
{
    int8_t *pc_cmd = OAL_PTR_NULL;
    uint32_t ul_ret;

    if (ul_len > WAL_HIPRIV_CMD_MAX_LEN) {
        oam_error_log1(0, OAM_SF_ANY, "{wal_hipriv_proc_write::ul_len>WAL_HIPRIV_CMD_MAX_LEN, ul_len [%d]!}", ul_len);
        return -OAL_EINVAL;
    }

    pc_cmd = oal_mem_alloc_m(OAL_MEM_POOL_ID_LOCAL, WAL_HIPRIV_CMD_MAX_LEN, OAL_TRUE);
    if (oal_unlikely(pc_cmd == OAL_PTR_NULL)) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_proc_write::alloc mem return null ptr!}");
        return -OAL_ENOMEM;
    }

    memset_s(pc_cmd, WAL_HIPRIV_CMD_MAX_LEN, 0, WAL_HIPRIV_CMD_MAX_LEN);

    ul_ret = oal_copy_from_user((void *)pc_cmd, pc_buffer, ul_len);

    /* copy_from_user函数的目的是从用户空间拷贝数据到内核空间，失败返回没有被拷贝的字节数，成功返回0 */
    if (ul_ret > 0) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_proc_write::oal_copy_from_user return ul_ret[%d]!}", ul_ret);
        oal_mem_free_m(pc_cmd, OAL_TRUE);

        return -OAL_EFAUL;
    }

    pc_cmd[ul_len - 1] = '\0';

    ul_ret = wal_hipriv_parse_cmd(pc_cmd);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_proc_write::parse cmd return err code[%d]!}", ul_ret);
    }

    oal_mem_free_m(pc_cmd, OAL_TRUE);

    return (int32_t)ul_len;
}
#endif

uint32_t wal_hipriv_create_proc(void *p_proc_arg)
{
#ifdef _PRE_CONFIG_CONN_HISI_SYSFS_SUPPORT
    uint32_t ul_ret;
#endif

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 10, 44))
    /* TBD */
    g_pst_proc_entry = OAL_PTR_NULL;
#else

    /* 420十进制对应八进制是0644 linux模式定义 S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH); */
    /* S_IRUSR文件所有者具可读取权限, S_IWUSR文件所有者具可写入权限, S_IRGRP用户组具可读取权限,
       S_IROTH其他用户具可读取权限 */
    g_pst_proc_entry = oal_create_proc_entry(WAL_HIPRIV_PROC_ENTRY_NAME, 420, NULL);
    if (g_pst_proc_entry == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_create_proc::oal_create_proc_entry return null ptr!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    g_pst_proc_entry->data = p_proc_arg;
    g_pst_proc_entry->nlink = 1; /* linux创建proc默认值 */
    g_pst_proc_entry->read_proc = OAL_PTR_NULL;

    g_pst_proc_entry->write_proc = (write_proc_t *)wal_hipriv_proc_write;
#endif

    /* hi1102-cb add sys for 51/02 */
#ifdef _PRE_CONFIG_CONN_HISI_SYSFS_SUPPORT
    gp_sys_kobject = oal_get_sysfs_root_object();
    if (gp_sys_kobject == NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_hipriv_create_proc::get sysfs root object failed!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    ul_ret = (uint32_t)oal_debug_sysfs_create_group(gp_sys_kobject, &hipriv_attribute_group);
    if (ul_ret) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_hipriv_create_proc::hipriv_attribute_group create failed!}");
        ul_ret = OAL_ERR_CODE_PTR_NULL;
        return ul_ret;
    }
#endif

    return OAL_SUCC;
}


uint32_t wal_hipriv_remove_proc(void)
{
    /* 卸载时删除sysfs */
#ifdef _PRE_CONFIG_CONN_HISI_SYSFS_SUPPORT
    if (gp_sys_kobject != NULL) {
        oal_debug_sysfs_remove_group(gp_sys_kobject, &hipriv_attribute_group);
        kobject_del(gp_sys_kobject);
        gp_sys_kobject = NULL;
    }
    oal_conn_sysfs_root_obj_exit();
    oal_conn_sysfs_root_boot_obj_exit();
#endif

    if (g_pst_proc_entry) {
        oal_remove_proc_entry(WAL_HIPRIV_PROC_ENTRY_NAME, NULL);
        g_pst_proc_entry = OAL_PTR_NULL;
    }

    return OAL_SUCC;
}
#ifdef PLATFORM_DEBUG_ENABLE

OAL_STATIC uint32_t wal_hipriv_reg_info(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;
    uint16_t us_len;

    /* 抛事件到wal层处理 */
    l_ret = memcpy_s(st_write_msg.auc_value, OAL_STRLEN(pc_param), pc_param, OAL_STRLEN(pc_param));
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "wal_hipriv_reg_info::memcpy fail!");
        return OAL_FAIL;
    }

    st_write_msg.auc_value[OAL_STRLEN(pc_param)] = '\0';

    us_len = (uint16_t)(OAL_STRLEN(pc_param) + 1);

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_REG_INFO, us_len);

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + us_len,
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_reg_info::return err code [%d]!}", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}
#endif

OAL_STATIC uint32_t wal_hipriv_sdio_flowctrl(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;
    uint16_t us_len;

    if (oal_unlikely(WAL_MSG_WRITE_MAX_LEN <= OAL_STRLEN(pc_param))) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_sdio_flowctrl:: pc_param overlength:%d}", OAL_STRLEN(pc_param));
        return OAL_FAIL;
    }

    /* 抛事件到wal层处理 */
    l_ret = memcpy_s(st_write_msg.auc_value, OAL_STRLEN(pc_param), pc_param, OAL_STRLEN(pc_param));
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "wal_hipriv_sdio_flowctrl::memcpy fail!");
        return OAL_FAIL;
    }

    st_write_msg.auc_value[OAL_STRLEN(pc_param)] = '\0';

    us_len = (uint16_t)(OAL_STRLEN(pc_param) + 1);

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SDIO_FLOWCTRL, us_len);

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + us_len,
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_sdio_flowctrl::return err code [%d]!}", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}
#ifdef PLATFORM_DEBUG_ENABLE

OAL_STATIC uint32_t wal_hipriv_reg_write(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;
    uint16_t us_len;

    /* 抛事件到wal层处理 */
    l_ret = memcpy_s(st_write_msg.auc_value, OAL_STRLEN(pc_param), pc_param, OAL_STRLEN(pc_param));
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "wal_hipriv_reg_write::memcpy fail!");
        return OAL_FAIL;
    }

    st_write_msg.auc_value[OAL_STRLEN(pc_param)] = '\0';

    us_len = (uint16_t)(OAL_STRLEN(pc_param) + 1);

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_REG_WRITE, us_len);

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + us_len,
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_reg_write::return err code [%d]!}", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}
#endif

uint32_t wal_hipriv_alg_cfg(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;
    mac_ioctl_alg_param_stru *pst_alg_param = OAL_PTR_NULL;
    wal_ioctl_alg_cfg_stru st_alg_cfg;
    uint8_t uc_map_index = 0;
    int32_t l_ret;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;

    pst_alg_param = (mac_ioctl_alg_param_stru *)(st_write_msg.auc_value);

    /* 获取配置参数名称 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_alg_cfg::get cmd one arg err[%d]!}", ul_ret);
        return ul_ret;
    }

    /* 寻找匹配的命令 */
    st_alg_cfg = g_ast_alg_cfg_map[0];
    while (st_alg_cfg.pc_name != OAL_PTR_NULL) {
        if (0 == oal_strcmp(st_alg_cfg.pc_name, ac_name)) {
            break;
        }
        st_alg_cfg = g_ast_alg_cfg_map[++uc_map_index];
    }

    oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_alg_cfg:: alg_cfg cmd[%d]!}", st_alg_cfg.en_alg_cfg);

    /* 没有找到对应的命令，则报错 */
    if (st_alg_cfg.pc_name == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_alg_cfg::invalid alg_cfg cmd!}");
        return OAL_FAIL;
    }

    /* 记录命令对应的枚举值 */
    pst_alg_param->en_alg_cfg = g_ast_alg_cfg_map[uc_map_index].en_alg_cfg;

    /* 获取参数配置值 */
    ul_ret = wal_get_cmd_one_arg(pc_param + ul_off_set, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_alg_cfg::get cmd one arg err[%d]!}", ul_ret);
        return ul_ret;
    }

    /* 记录参数配置值 */
    pst_alg_param->ul_value = (uint32_t)oal_atoi(ac_name);

    /* 抛事件到wal层处理 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_ALG_PARAM, OAL_SIZEOF(mac_ioctl_alg_param_stru));

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_ioctl_alg_param_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_TRUE,
                                   &pst_rsp_msg);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        return (uint32_t)l_ret;
    }
    /* 读取返回的错误码 */
    ul_ret = wal_check_and_release_msg_resp(pst_rsp_msg);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_DFR, "{wal_hipriv_alg_cfg fail, err code[%u]!}", ul_ret);
        return ul_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_tpc_log(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;
    int32_t l_ret;
    mac_ioctl_alg_tpc_log_param_stru *pst_alg_tpc_log_param = OAL_PTR_NULL;
    wal_ioctl_alg_cfg_stru st_alg_cfg;
    uint8_t uc_map_index = 0;
    oal_bool_enum_uint8 en_stop_flag = OAL_FALSE;

    pst_alg_tpc_log_param = (mac_ioctl_alg_tpc_log_param_stru *)(st_write_msg.auc_value);

    /* 获取配置参数名称 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_tpc_log::get cmd one arg err[%d]!}", ul_ret);
        return ul_ret;
    }
    pc_param = pc_param + ul_off_set;

    /* 寻找匹配的命令 */
    st_alg_cfg = g_ast_alg_cfg_map[0];
    while (st_alg_cfg.pc_name != OAL_PTR_NULL) {
        if (0 == oal_strcmp(st_alg_cfg.pc_name, ac_name)) {
            break;
        }
        st_alg_cfg = g_ast_alg_cfg_map[++uc_map_index];
    }

    /* 没有找到对应的命令，则报错 */
    if (st_alg_cfg.pc_name == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_tpc_log::invalid alg_cfg cmd!}");
        return OAL_FAIL;
    }

    /* 记录命令对应的枚举值 */
    pst_alg_tpc_log_param->en_alg_cfg = g_ast_alg_cfg_map[uc_map_index].en_alg_cfg;

    /* 区分获取特定帧功率和统计日志命令处理:获取功率只需获取帧名字 */
    if (pst_alg_tpc_log_param->en_alg_cfg == MAC_ALG_CFG_TPC_GET_FRAME_POW) {
        /* 获取配置参数名称 */
        ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_tpc_log::get cmd one arg err[%d]!}", ul_ret);
            return ul_ret;
        }
        /* 记录命令对应的帧名字 */
        pst_alg_tpc_log_param->pc_frame_name = ac_name;
    } else {
        ul_ret = wal_hipriv_get_mac_addr(pc_param, pst_alg_tpc_log_param->auc_mac_addr, &ul_off_set);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_tpc_log::wal_hipriv_get_mac_addr failed!}");
            return ul_ret;
        }
        pc_param += ul_off_set;

        while ((*pc_param == ' ') || (*pc_param == '\0')) {
            if (*pc_param == '\0') {
                en_stop_flag = OAL_TRUE;
                break;
            }
            ++pc_param;
        }

        /* 获取业务类型值 */
        if (en_stop_flag != OAL_TRUE) {
            ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
            if (ul_ret != OAL_SUCC) {
                oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_tpc_log::get cmd one arg err[%d]!}", ul_ret);
                return ul_ret;
            }

            pst_alg_tpc_log_param->uc_ac_no = (uint8_t)oal_atoi(ac_name);
            pc_param = pc_param + ul_off_set;

            en_stop_flag = OAL_FALSE;
            while ((*pc_param == ' ') || (*pc_param == '\0')) {
                if (*pc_param == '\0') {
                    en_stop_flag = OAL_TRUE;
                    break;
                }
                ++pc_param;
            }

            if (en_stop_flag != OAL_TRUE) {
                /* 获取参数配置值 */
                ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
                if (ul_ret != OAL_SUCC) {
                    oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_tpc_log::get cmd one arg err[%d]!}", ul_ret);
                    return ul_ret;
                }

                /* 记录参数配置值 */
                pst_alg_tpc_log_param->us_value = (uint16_t)oal_atoi(ac_name);
            }
        }
    }

    /* 抛事件到wal层处理 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_ALG_PARAM, OAL_SIZEOF(mac_ioctl_alg_tpc_log_param_stru));

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_ioctl_alg_tpc_log_param_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_HS20

OAL_STATIC int32_t wal_ioctl_set_qos_map(oal_net_device_stru *pst_net_dev,
    hmac_cfg_qos_map_param_stru *pst_qos_map_param)
{
    wal_msg_write_stru st_write_msg;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;
    uint32_t ul_err_code;
    int32_t l_ret;

    l_ret = memcpy_s(st_write_msg.auc_value, OAL_SIZEOF(hmac_cfg_qos_map_param_stru),
                     pst_qos_map_param, OAL_SIZEOF(hmac_cfg_qos_map_param_stru));
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_HS20, "wal_ioctl_set_qos_map::memcpy fail!");
        return -OAL_EFAIL;
    }

    /* 抛事件到wal层处理 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_QOS_MAP, OAL_SIZEOF(hmac_cfg_qos_map_param_stru));

    /* 发送消息 */
    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(hmac_cfg_qos_map_param_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_TRUE,
                                   &pst_rsp_msg);
    if ((l_ret != OAL_SUCC) || (pst_rsp_msg == OAL_PTR_NULL)) {
        oam_error_log1(0, OAM_SF_HS20, "{wal_ioctl_set_qos_map:: wal_alloc_cfg_event return err code %d!}", l_ret);
        return l_ret;
    }

    /* 读取返回的错误码 */
    ul_err_code = wal_check_and_release_msg_resp(pst_rsp_msg);
    if (ul_err_code != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_HS20, "{wal_ioctl_set_qos_map::wal_check_and_release_msg_resp fail[%x!}",
                         ul_err_code);
        return -OAL_EFAIL;
    }
    return OAL_SUCC;
}
#endif  // _PRE_WLAN_FEATURE_HS20


OAL_STATIC uint32_t wal_get_parameter_from_cmd(int8_t *pc_cmd, int8_t *pc_arg,
    OAL_CONST int8_t *puc_token, uint32_t *pul_cmd_offset, uint32_t ul_param_max_len)
{
    int8_t *pc_cmd_copy = OAL_PTR_NULL;
    int8_t ac_cmd_copy[WAL_IOCTL_PRIV_SUBCMD_MAX_LEN];
    uint32_t ul_pos = 0;
    uint32_t ul_arg_len;
    int8_t *pc_cmd_tmp = OAL_PTR_NULL;

    if (oal_unlikely(oal_any_null_ptr3(pc_cmd, pc_arg, pul_cmd_offset))) {
        oam_error_log3(0, OAM_SF_ANY, "{wal_get_parameter_from_cmd::pc_cmd/pc_arg/pul_cmd_offset null:%x,%x,%x,%x}",
            (uintptr_t)pc_cmd, (uintptr_t)pc_arg, (uintptr_t)pul_cmd_offset);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pc_cmd_copy = pc_cmd;

    /* 去掉字符串开始的逗号 */
    while (',' == *pc_cmd_copy) {
        ++pc_cmd_copy;
    }
    /* 取得逗号前的字符串 */
    while ((',' != *pc_cmd_copy) && ('\0' != *pc_cmd_copy)) {
        ac_cmd_copy[ul_pos] = *pc_cmd_copy;
        ++ul_pos;
        ++pc_cmd_copy;

        if (oal_unlikely(ul_pos >= ul_param_max_len)) {
            oam_warning_log1(0, OAM_SF_ANY, "{wal_get_parameter_from_cmd::pos>=CMD_NAME_MAX_LEN, pos %d!}", ul_pos);
            return OAL_ERR_CODE_ARRAY_OVERFLOW;
        }
    }
    ac_cmd_copy[ul_pos] = '\0';
    /* 字符串到结尾，返回错误码 */
    if (0 == ul_pos) {
        oam_info_log0(0, OAM_SF_ANY, "{wal_get_parameter_from_cmd::return param pc_arg is null!}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    *pul_cmd_offset = (uint32_t)(pc_cmd_copy - pc_cmd);

    /* 检查字符串是否包含期望的前置命令字符 */
    pc_cmd_tmp = &ac_cmd_copy[0];
    if (0 != oal_memcmp(pc_cmd_tmp, puc_token, OAL_STRLEN(puc_token))) {
        return OAL_FAIL;
    } else {
        /* 扣除前置命令字符，回传参数 */
        ul_arg_len = OAL_STRLEN(ac_cmd_copy) - OAL_STRLEN(puc_token);
        if (EOK != memcpy_s(pc_arg, ul_param_max_len, ac_cmd_copy + OAL_STRLEN(puc_token), ul_arg_len)) {
            oam_warning_log0(0, OAM_SF_ANY, "wal_get_parameter_from_cmd::memcpy fail!");
        }
        pc_arg[ul_arg_len] = '\0';
    }
    return OAL_SUCC;
}


OAL_STATIC int32_t wal_set_ap_max_user(oal_net_device_stru *pst_net_dev, uint32_t ul_ap_max_user)
{
    wal_msg_write_stru st_write_msg;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;
    uint32_t ul_err_code;
    int32_t l_ret;

    oam_warning_log1(0, OAM_SF_ANY, "{wal_set_ap_max_user::set AP max user:%u.}", ul_ap_max_user);

    if (ul_ap_max_user == 0) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_set_ap_max_user::invalid max user(%u),ignore it}", ul_ap_max_user);
        return OAL_SUCC;
    }

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_MAX_USER, OAL_SIZEOF(ul_ap_max_user));
    *((uint32_t *)st_write_msg.auc_value) = ul_ap_max_user;
    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(ul_ap_max_user),
                                   (uint8_t *)&st_write_msg,
                                   OAL_TRUE,
                                   &pst_rsp_msg);

    if ((l_ret != OAL_SUCC) || (pst_rsp_msg == OAL_PTR_NULL)) {
        oam_error_log1(0, OAM_SF_ANY, "{wal_set_ap_max_user:: wal_send_cfg_event return err code %d!}", l_ret);

        return l_ret;
    }

    /* 读取返回的错误码 */
    ul_err_code = wal_check_and_release_msg_resp(pst_rsp_msg);
    if (ul_err_code != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_CFG, "{wal_set_ap_max_user::wal_check_and_release_msg_resp fail[%d]!}",
                         ul_err_code);
        return -OAL_EFAIL;
    }
    /* 每次设置最大用户数完成后，都清空为非法值0 **/
    /* g_st_ap_config_info.ul_ap_max_user 0 */
    return l_ret;
}


OAL_STATIC void wal_config_mac_filter(oal_net_device_stru *pst_net_dev, int8_t *pc_command)
{
    int8_t ac_parsed_command[WAL_IOCTL_PRIV_SUBCMD_MAX_LEN];
    int8_t *pc_parse_command = OAL_PTR_NULL;
    uint32_t ul_mac_mode;
    uint32_t ul_mac_cnt;
    uint32_t ul_i;
    wal_msg_write_stru st_write_msg;
    uint16_t us_len;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;
    uint32_t ul_err_code;
    int32_t l_ret = 0;
    mac_blacklist_stru *pst_blklst = OAL_PTR_NULL;
    uint32_t ul_ret;
    uint32_t ul_off_set;

    if (pc_command == OAL_PTR_NULL) {
        return;
    }
    pc_parse_command = pc_command;

    /* 解析MAC_MODE */
    ul_ret = wal_get_parameter_from_cmd(pc_parse_command, ac_parsed_command, "MAC_MODE=",
        &ul_off_set, WAL_IOCTL_PRIV_SUBCMD_MAX_LEN);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_config_mac_filter::wal_get_parameter_from_cmd err %u.}", ul_ret);
        return;
    }
    /* 检查参数是否合法 0,1,2 */
    ul_mac_mode = (uint32_t)oal_atoi(ac_parsed_command);
    if (ul_mac_mode > 2) {
        oam_warning_log4(0, OAM_SF_ANY, "{wal_config_mac_filter::invalid MAC_MODE[%c%c%c%c]!}",
            (uint8_t)ac_parsed_command[0], (uint8_t)ac_parsed_command[1],
            (uint8_t)ac_parsed_command[2], (uint8_t)ac_parsed_command[3]); /* 2 3 cmd para */
        return;
    }

    /* 设置过滤模式 */
    ul_ret = wal_hipriv_send_cfg_uint32_data(pst_net_dev, ac_parsed_command, WLAN_CFGID_BLACKLIST_MODE);
    if (ul_ret != OAL_SUCC) {
        return;
    }
    /* 解析MAC_CNT */
    pc_parse_command += ul_off_set;
    ul_ret = wal_get_parameter_from_cmd(pc_parse_command, ac_parsed_command, "MAC_CNT=",
        &ul_off_set, WAL_IOCTL_PRIV_SUBCMD_MAX_LEN);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_config_mac_filter::wal_get_parameter_from_cmd err [%u]!}", ul_ret);
        return;
    }
    ul_mac_cnt = (uint32_t)oal_atoi(ac_parsed_command);

    for (ul_i = 0; ul_i < ul_mac_cnt; ul_i++) {
        pc_parse_command += ul_off_set;
        ul_ret = wal_get_parameter_from_cmd(pc_parse_command, ac_parsed_command, "MAC=",
            &ul_off_set, WAL_IOCTL_PRIV_SUBCMD_MAX_LEN);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log1(0, OAM_SF_ANY, "{wal_config_mac_filter::wal_get_parameter_from_cmd err [%u]!}", ul_ret);
            return;
        }
        /* 5.1  检查参数是否符合MAC长度 */
        if (WLAN_MAC_ADDR_LEN * 2 != OAL_STRLEN(ac_parsed_command)) {
            oam_warning_log0(0, OAM_SF_ANY, "{wal_config_mac_filter::invalid MAC format}");
            return;
        }
        /* 6. 添加过滤设备 */
        /* 抛事件到wal层处理 */
        memset_s((uint8_t *)&st_write_msg, OAL_SIZEOF(st_write_msg), 0, OAL_SIZEOF(st_write_msg));
        pst_blklst = (mac_blacklist_stru *)(st_write_msg.auc_value);
        /* 将字符 ac_name 转换成数组 mac_add[6] */
        oal_strtoaddr(ac_parsed_command, OAL_SIZEOF(ac_parsed_command), pst_blklst->auc_mac_addr, WLAN_MAC_ADDR_LEN);

        us_len = OAL_SIZEOF(mac_blacklist_stru);
        if (ul_i == (ul_mac_cnt - 1)) {
            /* 等所有的mac地址都添加完成后，才进行关联用户确认，是否需要删除 */
            WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_ADD_BLACK_LIST, us_len);
        } else {
            WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_ADD_BLACK_LIST_ONLY, us_len);
        }

        /* 6.1  发送消息 */
        l_ret = wal_send_cfg_event(pst_net_dev, WAL_MSG_TYPE_WRITE,
            WAL_MSG_WRITE_MSG_HDR_LENGTH + us_len, (uint8_t *)&st_write_msg, OAL_TRUE, &pst_rsp_msg);
        if ((l_ret != OAL_SUCC) || (pst_rsp_msg == OAL_PTR_NULL)) {
            oam_error_log1(0, OAM_SF_ANY, "{wal_config_mac_filter:: wal_send_cfg_event err %d!}", l_ret);
            return;
        }

        /* 6.2  读取返回的错误码 */
        ul_err_code = wal_check_and_release_msg_resp(pst_rsp_msg);
        if (ul_err_code != OAL_SUCC) {
            oam_warning_log1(0, OAM_SF_ANY, "{wal_config_mac_filter::wal_check_and_release_msg_resp fail[%x]!}",
                             ul_err_code);
            return;
        }
    }
    /* 每次设置完成mac地址过滤后，清空此中间变量 */
    return;
}


OAL_STATIC int32_t wal_kick_sta(oal_net_device_stru *pst_net_dev,
    uint8_t *auc_mac_addr, uint8_t uc_mac_addr_len, uint16_t us_reason_code)
{
    wal_msg_write_stru st_write_msg;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;
    uint32_t ul_err_code;
    mac_cfg_kick_user_param_stru *pst_kick_user_param = OAL_PTR_NULL;
    int32_t l_ret;

    if ((auc_mac_addr == NULL) || (uc_mac_addr_len != WLAN_MAC_ADDR_LEN)) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_kick_sta::check para! .\n");
        return -OAL_EFAIL;
    }

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_KICK_USER, OAL_SIZEOF(mac_cfg_kick_user_param_stru));

    pst_kick_user_param = (mac_cfg_kick_user_param_stru *)(st_write_msg.auc_value);
    oal_set_mac_addr(pst_kick_user_param->auc_mac_addr, auc_mac_addr);

    pst_kick_user_param->us_reason_code = us_reason_code;

    l_ret = wal_send_cfg_event(pst_net_dev, WAL_MSG_TYPE_WRITE,
        WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_kick_user_param_stru),
        (uint8_t *)&st_write_msg, OAL_TRUE, &pst_rsp_msg);

    if ((l_ret != OAL_SUCC) || (pst_rsp_msg == OAL_PTR_NULL)) {
        oam_error_log1(0, OAM_SF_ANY, "{wal_kick_sta:: wal_send_cfg_event return err code %d!}", l_ret);
        return l_ret;
    }

    /* 4.4  读取返回的错误码 */
    ul_err_code = wal_check_and_release_msg_resp(pst_rsp_msg);
    if (ul_err_code != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_CFG, "{wal_kick_sta::wal_check_and_release_msg_resp return err code: [%x]!}",
                         ul_err_code);
        return -OAL_EFAIL;
    }

    return OAL_SUCC;
}


OAL_STATIC int wal_ioctl_set_ap_config(oal_net_device_stru *pst_net_dev,
    oal_iw_request_info_stru *pst_info, oal_iwreq_data_union *pst_wrqu, char *pc_extra)
{
    int8_t *pc_command = OAL_PTR_NULL;
    int8_t *pc_parse_cmd = OAL_PTR_NULL;
    int32_t l_ret = OAL_SUCC;
    uint32_t ul_ret, ul_off_set;
    int8_t ac_cmd[WAL_IOCTL_PRIV_SUBCMD_MAX_LEN];

    if (oal_unlikely(oal_any_null_ptr2(pst_net_dev, pst_wrqu))) {
        oam_warning_log2(0, 0, "{wal_ioctl_set_ap_config:netd:%p,wrq:%p}", (uintptr_t)pst_net_dev, (uintptr_t)pst_wrqu);
        return -OAL_EFAIL;
    }

    /* 1. 申请内存保存netd 下发的命令和数据 */
    pc_command = oal_memalloc((int32_t)(pst_wrqu->data.length + 1));
    if (pc_command == OAL_PTR_NULL) {
        return -OAL_ENOMEM;
    }
    /* 2. 拷贝netd 命令到内核态中 */
    memset_s(pc_command, (uint32_t)(pst_wrqu->data.length + 1), 0, (uint32_t)(pst_wrqu->data.length + 1));
    ul_ret = oal_copy_from_user(pc_command, pst_wrqu->data.pointer, (uint32_t)(pst_wrqu->data.length));
    if (ul_ret != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_ioctl_set_ap_config::oal_copy_from_user: -OAL_EFAIL }");
        oal_free(pc_command);
        return -OAL_EFAIL;
    }
    pc_command[pst_wrqu->data.length] = '\0';

    oal_io_print("wal_ioctl_set_ap_config,data len:%u \n", (uint32_t)pst_wrqu->data.length);

    pc_parse_cmd = pc_command;
    /* 3.   解析参数 */
    /* 3.1  解析ASCII_CMD */
    ul_ret = wal_get_parameter_from_cmd(pc_parse_cmd, ac_cmd, "ASCII_CMD=", &ul_off_set, WAL_IOCTL_PRIV_SUBCMD_MAX_LEN);
    if (ul_ret != OAL_SUCC) {
        oal_free(pc_command);
        return -OAL_EINVAL;
    }

    if ((0 != oal_strcmp("AP_CFG", ac_cmd))) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_ioctl_set_ap_config::sub_command != 'AP_CFG' }");
        oal_free(pc_command);
        return -OAL_EINVAL;
    }

    /* 3.2  解析CHANNEL，目前不处理netd下发的channel信息 */
    pc_parse_cmd += ul_off_set;
    ul_ret = wal_get_parameter_from_cmd(pc_parse_cmd, ac_cmd, "CHANNEL=", &ul_off_set, WAL_IOCTL_PRIV_SUBCMD_MAX_LEN);
    if (ul_ret != OAL_SUCC) {
        oal_free(pc_command);
        return -OAL_EINVAL;
    }

    /* 3.3  解析MAX_SCB */
    pc_parse_cmd += ul_off_set;
    ul_ret = wal_get_parameter_from_cmd(pc_parse_cmd, ac_cmd, "MAX_SCB=", &ul_off_set, WAL_IOCTL_PRIV_SUBCMD_MAX_LEN);
    if (ul_ret != OAL_SUCC) {
        oal_free(pc_command);
        return -OAL_EINVAL;
    }

    g_st_ap_config_info.ul_ap_max_user = (uint32_t)oal_atoi(ac_cmd);
    if (oal_net_dev_priv(pst_net_dev) != OAL_PTR_NULL) {
        l_ret = wal_set_ap_max_user(pst_net_dev, (uint32_t)oal_atoi(ac_cmd));
    }

    /* 5. 结束释放内存 */
    oal_free(pc_command);
    return l_ret;
}


OAL_STATIC int wal_ioctl_get_assoc_list(oal_net_device_stru *pst_net_dev,
    oal_iw_request_info_stru *pst_info, oal_iwreq_data_union *pst_wrqu, char *pc_extra)
{
    int32_t l_ret;
    int32_t l_mret = EOK;
    wal_msg_query_stru st_query_msg;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;
    wal_msg_rsp_stru *pst_query_rsp_msg = OAL_PTR_NULL;
    int8_t *pc_sta_list = OAL_PTR_NULL;
    oal_netbuf_stru *pst_rsp_netbuf = OAL_PTR_NULL;

    if (oal_unlikely(oal_any_null_ptr4(pst_net_dev, pst_info, pst_wrqu, pc_extra))) {
        oam_warning_log4(0, OAM_SF_ANY, "{wal_ioctl_get_assoc_list::para null:netdev=%p,info=%p,pst_wrqu=%p,extra=%p}",
                         (uintptr_t)pst_net_dev, (uintptr_t)pst_info, (uintptr_t)pst_wrqu, (uintptr_t)pc_extra);
        return -OAL_EFAIL;
    }

    /* 上层在任何时候都可能下发此命令，需要先判断当前netdev的状态并及时返回 */
    if (oal_unlikely(oal_net_dev_priv(pst_net_dev) == OAL_PTR_NULL)) {
        return -OAL_EFAIL;
    }

    /* 抛事件到wal层处理 */
    st_query_msg.en_wid = WLAN_CFGID_GET_STA_LIST;

    /* 发送消息 */
    l_ret = wal_send_cfg_event(pst_net_dev, WAL_MSG_TYPE_QUERY,
        WAL_MSG_WID_LENGTH, (uint8_t *)&st_query_msg, OAL_TRUE, &pst_rsp_msg);

    if ((l_ret != OAL_SUCC) || (pst_rsp_msg == OAL_PTR_NULL)) {
        return l_ret;
    }

    /* 处理返回消息 */
    pst_query_rsp_msg = (wal_msg_rsp_stru *)(pst_rsp_msg->auc_msg_data);
    /* 业务处理 */
    if (pst_query_rsp_msg->us_len >= OAL_SIZEOF(oal_netbuf_stru *)) {
        /* 获取hmac保存的netbuf指针 */
        l_mret = memcpy_s(&pst_rsp_netbuf, OAL_SIZEOF(oal_netbuf_stru *),
                                pst_query_rsp_msg->auc_value, OAL_SIZEOF(oal_netbuf_stru *));
        if (pst_rsp_netbuf != NULL) {
            /* 保存ap保存的sta地址信息 */
            pc_sta_list = (int8_t *)oal_netbuf_data(pst_rsp_netbuf);
            pst_wrqu->data.length = (uint16_t)(oal_netbuf_len(pst_rsp_netbuf) + 1);
            l_mret += memcpy_s(pc_extra, pst_wrqu->data.length, pc_sta_list, pst_wrqu->data.length);
            pc_extra[oal_netbuf_len(pst_rsp_netbuf)] = '\0';
            oal_netbuf_free(pst_rsp_netbuf);
        } else {
            l_ret = -OAL_ENOMEM;
        }
    } else {
        oal_print_hex_dump((uint8_t *)pst_rsp_msg->auc_msg_data, pst_query_rsp_msg->us_len, 32, "query msg: ");
        l_ret = -OAL_EINVAL;
    }

    if ((l_ret != OAL_SUCC) || (l_mret != EOK)) {
        oam_error_log2(0, OAM_SF_ANY, "{wal_ioctl_get_assoc_list::process failed,ret:%d, mret:%d}", l_ret, l_mret);
        l_ret = -OAL_EINVAL;
    }

    oal_free(pst_rsp_msg);
    return l_ret;
}


OAL_STATIC int wal_ioctl_set_mac_filters(oal_net_device_stru *pst_net_dev,
    oal_iw_request_info_stru *pst_info, oal_iwreq_data_union *pst_wrqu, char *pc_extra)
{
    mac_vap_stru *pst_vap = OAL_PTR_NULL;
    int8_t  *pc_command = OAL_PTR_NULL;
    int32_t  l_ret = 0;
    uint32_t ul_ret;
    int8_t   ac_parsed_command[WAL_IOCTL_PRIV_SUBCMD_MAX_LEN];
    int8_t  *pc_parse_command = OAL_PTR_NULL;
    uint32_t ul_mac_mode, ul_mac_cnt, ul_off_set;
    uint8_t  auc_mac_addr[WLAN_MAC_ADDR_LEN];

    if (oal_unlikely(oal_any_null_ptr4(pst_net_dev, pst_info, pst_wrqu, pc_extra))) {
        oam_warning_log4(0, 0, "{wal_ioctl_set_mac_filters::param null,net_dev:%p,info:%p,wrqu:%p,pc_extra:%p}",
            (uintptr_t)pst_net_dev, (uintptr_t)pst_info, (uintptr_t)pst_wrqu, (uintptr_t)pc_extra);
        return -OAL_EFAIL;
    }

    /* 1. 申请内存保存netd 下发的命令和数据 */
    pc_command = oal_memalloc((int32_t)(pst_wrqu->data.length + 1));
    if (pc_command == OAL_PTR_NULL) {
        return -OAL_ENOMEM;
    }

    /* 2. 拷贝netd 命令到内核态中 */
    memset_s(pc_command, (uint32_t)(pst_wrqu->data.length + 1), 0, (uint32_t)(pst_wrqu->data.length + 1));
    ul_ret = oal_copy_from_user(pc_command, pst_wrqu->data.pointer, (uint32_t)(pst_wrqu->data.length));
    if (ul_ret != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_ioctl_set_mac_filters::oal_copy_from_user: -OAL_EFAIL }");
        oal_free(pc_command);
        return -OAL_EFAIL;
    }
    pc_command[pst_wrqu->data.length] = '\0';

    oal_io_print("wal_ioctl_set_mac_filters,data len:%d \n", pst_wrqu->data.length);

    pc_parse_command = pc_command;

    memset_s(g_st_ap_config_info.ac_ap_mac_filter_mode,
             OAL_SIZEOF(g_st_ap_config_info.ac_ap_mac_filter_mode), 0,
             OAL_SIZEOF(g_st_ap_config_info.ac_ap_mac_filter_mode));
    strncpy_s(g_st_ap_config_info.ac_ap_mac_filter_mode, OAL_SIZEOF(g_st_ap_config_info.ac_ap_mac_filter_mode),
              pc_command, OAL_SIZEOF(g_st_ap_config_info.ac_ap_mac_filter_mode) - 1);

    pst_vap = oal_net_dev_priv(pst_net_dev);
    if (pst_vap == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_ioctl_set_mac_filters::netdevice vap is null,just save it.}");
        oal_free(pc_command);
        return OAL_SUCC;
    }

    /* 3  解析MAC_MODE */
    ul_ret = wal_get_parameter_from_cmd(pc_parse_command, ac_parsed_command, "MAC_MODE=",
        &ul_off_set, WAL_IOCTL_PRIV_SUBCMD_MAX_LEN);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_ioctl_set_mac_filters::wal_get_parameter_from_cmd err [%u]!}", ul_ret);
        oal_free(pc_command);
        return -OAL_EINVAL;
    }
    /* 3.1 检查参数是否合法 0,1,2 */
    ul_mac_mode = (uint32_t)oal_atoi(ac_parsed_command);
    if (ul_mac_mode > 2) {
        oam_warning_log4(0, OAM_SF_ANY, "{wal_ioctl_set_mac_filters::invalid MAC_MODE[%c%c%c%c]!}",
            (uint8_t)ac_parsed_command[0], (uint8_t)ac_parsed_command[1],
            (uint8_t)ac_parsed_command[2], (uint8_t)ac_parsed_command[3]); /* 2 3 cmd para */
        oal_free(pc_command);
        return -OAL_EINVAL;
    }

    /* 5 解析MAC_CNT */
    pc_parse_command += ul_off_set;
    ul_ret = wal_get_parameter_from_cmd(pc_parse_command, ac_parsed_command, "MAC_CNT=",
        &ul_off_set, WAL_IOCTL_PRIV_SUBCMD_MAX_LEN);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_ioctl_set_mac_filters::wal_get_parameter_from_cmd err [%u]!}", ul_ret);
        oal_free(pc_command);
        return -OAL_EINVAL;
    }
    ul_mac_cnt = (uint32_t)oal_atoi(ac_parsed_command);

    wal_config_mac_filter(pst_net_dev, pc_command);

    /* 如果是白名单模式，且下发允许MAC地址为空，即不允许任何设备关联，需要去关联所有已经关联的STA */
    if ((ul_mac_cnt == 0) && (ul_mac_mode == 2)) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_ioctl_set_mac_filters::delete all user!}");

        memset_s(auc_mac_addr, OAL_ETH_ALEN, 0xff, OAL_ETH_ALEN);
        l_ret = wal_kick_sta(pst_net_dev, auc_mac_addr, sizeof(auc_mac_addr), MAC_AUTH_NOT_VALID);
    }

    oal_free(pc_command);
    return l_ret;
}


OAL_STATIC int wal_ioctl_set_ap_sta_disassoc(oal_net_device_stru *pst_net_dev,
    oal_iw_request_info_stru *pst_info, oal_iwreq_data_union *pst_wrqu, char *pc_extra)
{
    int8_t  *pc_command = OAL_PTR_NULL;
    int32_t  l_ret;
    uint32_t ul_ret;
    int8_t   ac_parsed_command[WAL_IOCTL_PRIV_SUBCMD_MAX_LEN] = { 0 };
    uint8_t  auc_mac_addr[WLAN_MAC_ADDR_LEN] = { 0 };
    uint32_t ul_off_set;

    if (oal_unlikely(oal_any_null_ptr2(pst_net_dev, pst_wrqu))) {
        oam_warning_log2(0, OAM_SF_ANY, "{wal_ioctl_set_ap_sta_disassoc:: param is null, net_dev:%p, pst_wrqu:%p}",
                         (uintptr_t)pst_net_dev, (uintptr_t)pst_wrqu);
        return -OAL_EFAIL;
    }

    /* 1. 申请内存保存netd 下发的命令和数据 */
    pc_command = oal_memalloc((int32_t)(pst_wrqu->data.length + 1));
    if (pc_command == OAL_PTR_NULL) {
        return -OAL_ENOMEM;
    }

    /* 2. 拷贝netd 命令到内核态中 */
    memset_s(pc_command, (uint32_t)(pst_wrqu->data.length + 1), 0, (uint32_t)(pst_wrqu->data.length + 1));
    ul_ret = oal_copy_from_user(pc_command, pst_wrqu->data.pointer, (uint32_t)(pst_wrqu->data.length));
    if (ul_ret != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_ioctl_set_ap_sta_disassoc::oal_copy_from_user: -OAL_EFAIL }");
        oal_free(pc_command);
        return -OAL_EFAIL;
    }
    pc_command[pst_wrqu->data.length] = '\0';

    /* 3. 解析命令获取MAC */
    ul_ret = wal_get_parameter_from_cmd(pc_command, ac_parsed_command, "MAC=",
        &ul_off_set, WAL_IOCTL_PRIV_SUBCMD_MAX_LEN);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_ioctl_set_ap_sta_disassoc::get_parameter_from_cmd MAC err[%u]}", ul_ret);
        oal_free(pc_command);
        return -OAL_EINVAL;
    }
    /* 3.1  检查参数是否符合MAC长度 */
    if (WLAN_MAC_ADDR_LEN * 2 != OAL_STRLEN(ac_parsed_command)) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_ioctl_set_ap_sta_disassoc::invalid MAC format}");
        oal_free(pc_command);
        return -OAL_EINVAL;
    }

    /* 将字符 ac_name 转换成数组 mac_add[6] */
    oal_strtoaddr(ac_parsed_command, OAL_SIZEOF(ac_parsed_command), auc_mac_addr, WLAN_MAC_ADDR_LEN);

    oam_warning_log0(0, OAM_SF_ANY, "{wal_ioctl_set_ap_sta_disassoc::Geting CMD from APP to DISASSOC!!}");
    l_ret = wal_kick_sta(pst_net_dev, auc_mac_addr, sizeof(auc_mac_addr), MAC_AUTH_NOT_VALID);

    /* 5. 结束释放内存 */
    oal_free(pc_command);
    return l_ret;
}

uint32_t wal_hipriv_get_mac_addr(int8_t *pc_param,
    uint8_t auc_mac_addr[], uint32_t *pul_total_offset)
{
    uint32_t ul_off_set = 0;
    uint32_t ul_ret ;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];

    /* 获取mac地址 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_get_mac_addr::get cmd one arg err[%d]!}", ul_ret);
        return ul_ret;
    }
    oal_strtoaddr(ac_name, OAL_SIZEOF(ac_name), auc_mac_addr, WLAN_MAC_ADDR_LEN);

    *pul_total_offset = ul_off_set;

    return OAL_SUCC;
}


uint32_t wal_hipriv_send_cfg_uint32_data(oal_net_device_stru *pst_net_dev,
                                               int8_t *pc_param, wlan_cfgid_enum_uint16 cfgid)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;
    uint16_t us_len;
    uint32_t ul_ret;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN] = { 0 };
    uint32_t ul_off_set = 0;
    uint32_t set_value;

    /* 抛事件到wal层处理 */
    memset_s(&st_write_msg, OAL_SIZEOF(wal_msg_write_stru), 0, OAL_SIZEOF(wal_msg_write_stru));
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_send_cfg_uint32_data:wal_get_cmd_one_arg fail!}");
        return ul_ret;
    }

    pc_param += ul_off_set;
    set_value = (uint32_t)oal_atoi((const int8_t *)ac_name);

    us_len = 4; /* OAL_SIZEOF(uint32_t) */
    *(uint32_t *)(st_write_msg.auc_value) = set_value;

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, cfgid, us_len);

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + us_len,
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_CFG, "{wal_hipriv_send_cfg_uint32_data:send_cfg_event return[%d]}", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


uint32_t wal_ioctl_set_sta_ps_mode(oal_net_device_stru *pst_cfg_net_dev,
    int8_t uc_ps_mode)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;
    mac_cfg_ps_mode_param_stru *pst_ps_mode_param;

    /* 抛事件到wal层处理 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_PS_MODE, OAL_SIZEOF(mac_cfg_ps_mode_param_stru));

    /* 设置配置命令参数 */
    pst_ps_mode_param = (mac_cfg_ps_mode_param_stru *)(st_write_msg.auc_value);
    pst_ps_mode_param->uc_vap_ps_mode = uc_ps_mode;

    l_ret = wal_send_cfg_event(pst_cfg_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_ps_mode_param_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_PWR, "{wal_hipriv_sta_ps_enable::return err code[%d]!}", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t wal_hipriv_sta_ps_mode(oal_net_device_stru *pst_cfg_net_dev, int8_t *pc_param)
{
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;
    uint8_t uc_vap_ps_mode;

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_sta_ps_enable::get cmd one arg err[%d]!}", ul_ret);
        return ul_ret;
    }

    uc_vap_ps_mode = (uint8_t)oal_atoi(ac_name);
    pc_param = pc_param + ul_off_set;

    return wal_ioctl_set_sta_ps_mode(pst_cfg_net_dev, uc_vap_ps_mode);
}


OAL_STATIC uint32_t wal_hipriv_sta_ps_info(oal_net_device_stru *pst_cfg_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;
    int32_t l_ret;
    uint8_t uc_psm_info_enable;
    uint8_t uc_psm_debug_mode;
    mac_cfg_ps_info_stru *pst_ps_info = NULL;

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_sta_ps_info::get cmd one arg err[%d]!}", ul_ret);
        return ul_ret;
    }

    uc_psm_info_enable = (uint8_t)oal_atoi(ac_name);
    pc_param = pc_param + ul_off_set;

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_sta_ps_info::get cmd one arg err[%d]!}", ul_ret);
        return ul_ret;
    }

    uc_psm_debug_mode = (uint8_t)oal_atoi(ac_name);
    pc_param = pc_param + ul_off_set;

    /* 抛事件到wal层处理 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SHOW_PS_INFO, OAL_SIZEOF(mac_cfg_ps_info_stru));

    /* 设置配置命令参数 */
    pst_ps_info = (mac_cfg_ps_info_stru *)(st_write_msg.auc_value);
    pst_ps_info->uc_psm_info_enable = uc_psm_info_enable;
    pst_ps_info->uc_psm_debug_mode = uc_psm_debug_mode;

    l_ret = wal_send_cfg_event(pst_cfg_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_ps_info_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_PWR, "{wal_hipriv_sta_ps_info::return err code [%d]!}", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


uint32_t wal_ioctl_set_fast_sleep_para(oal_net_device_stru *pst_cfg_net_dev,
    int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;

    /* 抛事件到wal层处理 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_FASTSLEEP_PARA, 4 * OAL_SIZEOF(uint8_t));

    /* 设置配置命令参数 */
    if (EOK != memcpy_s(st_write_msg.auc_value, 4 * OAL_SIZEOF(uint8_t), pc_param, 4 * OAL_SIZEOF(uint8_t))) {
        oam_error_log0(0, OAM_SF_PWR, "wal_ioctl_set_fast_sleep_para::memcpy fail!");
        return OAL_FAIL;
    }

    l_ret = wal_send_cfg_event(pst_cfg_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + 4 * OAL_SIZEOF(uint8_t),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_PWR, "{wal_ioctl_set_fast_sleep_para::return err code [%d]!}", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}

OAL_STATIC uint32_t wal_hipriv_set_fasts_sleep_para(oal_net_device_stru *pst_cfg_net_dev,
    int8_t *pc_param)
{
    uint32_t ul_para_cnt;
    uint8_t auc_para_val[4];
    uint8_t auc_para_str_tmp[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t pul_cmd_offset;
    uint32_t ul_ret;

    /* 获取携带的4个参数<min listen时间><max listen时间><亮屏收发包门限><暗屏收发包门限> */
    for (ul_para_cnt = 0; ul_para_cnt < 4; ul_para_cnt++) {
        ul_ret = wal_get_cmd_one_arg(pc_param, auc_para_str_tmp, WAL_HIPRIV_CMD_NAME_MAX_LEN, &pul_cmd_offset);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log1(0, OAM_SF_CFG, "{wal_hipriv_set_fasts_sleep_para::get para fail, err [%d]!}", ul_ret);
            return ul_ret;
        }
        auc_para_val[ul_para_cnt] = (uint8_t)oal_atoi(auc_para_str_tmp);
        pc_param += pul_cmd_offset;
    }

    return wal_ioctl_set_fast_sleep_para(pst_cfg_net_dev, auc_para_val);
}


OAL_STATIC uint32_t wal_hipriv_set_uapsd_para(oal_net_device_stru *pst_cfg_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;
    int32_t l_ret;
    mac_cfg_uapsd_sta_stru *pst_uapsd_param = OAL_PTR_NULL;
    uint8_t uc_max_sp_len;
    uint8_t uc_ac;
    uint8_t uc_delivery_enabled[WLAN_WME_AC_BUTT];
    uint8_t uc_trigger_enabled[WLAN_WME_AC_BUTT];

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_uapsd_para::get cmd one arg err[%d]!}", ul_ret);
        return ul_ret;
    }

    uc_max_sp_len = (uint8_t)oal_atoi(ac_name);

    for (uc_ac = 0; uc_ac < WLAN_WME_AC_BUTT; uc_ac++) {
        pc_param = pc_param + ul_off_set;
        ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_uapsd_para::get cmd one arg err[%d]!}", ul_ret);
            return ul_ret;
        }

        /* delivery_enabled的参数设置 */
        uc_delivery_enabled[uc_ac] = (uint8_t)oal_atoi(ac_name);

        /* trigger_enabled 参数的设置 */
        uc_trigger_enabled[uc_ac] = (uint8_t)oal_atoi(ac_name);
    }
    /* 抛事件到wal层处理 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_UAPSD_PARA, OAL_SIZEOF(mac_cfg_uapsd_sta_stru));

    /* 设置配置命令参数 */
    pst_uapsd_param = (mac_cfg_uapsd_sta_stru *)(st_write_msg.auc_value);
    pst_uapsd_param->uc_max_sp_len = uc_max_sp_len;
    for (uc_ac = 0; uc_ac < WLAN_WME_AC_BUTT; uc_ac++) {
        pst_uapsd_param->uc_delivery_enabled[uc_ac] = uc_delivery_enabled[uc_ac];
        pst_uapsd_param->uc_trigger_enabled[uc_ac] = uc_trigger_enabled[uc_ac];
    }

    l_ret = wal_send_cfg_event(pst_cfg_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_uapsd_sta_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_PWR, "{wal_hipriv_set_uapsd_para::return err code [%d]!}", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


int32_t wal_start_vap(oal_net_device_stru *pst_net_dev)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;
    uint32_t ul_err_code;
    wlan_p2p_mode_enum_uint8 en_p2p_mode;
    oal_wireless_dev_stru *pst_wdev = NULL;

    if (oal_unlikely(pst_net_dev == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_start_vap::pst_net_dev null!}");
        return -OAL_EFAUL;
    }

    oal_io_print("wal_start_vap,dev_name is:%s\n", pst_net_dev->name);

    /* 抛事件到wal层处理 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_START_VAP, OAL_SIZEOF(mac_cfg_start_vap_param_stru));
    ((mac_cfg_start_vap_param_stru *)st_write_msg.auc_value)->pst_net_dev = pst_net_dev;
    pst_wdev = pst_net_dev->ieee80211_ptr;
    en_p2p_mode = wal_wireless_iftype_to_mac_p2p_mode(pst_wdev->iftype);
    if (WLAN_P2P_BUTT == en_p2p_mode) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_start_vap::wal_wireless_iftype_to_mac_p2p_mode return BUFF}");
        return -OAL_EINVAL;
    }
    ((mac_cfg_start_vap_param_stru *)st_write_msg.auc_value)->en_p2p_mode = en_p2p_mode;

    oam_warning_log1(0, OAM_SF_ANY, "{wal_start_vap::en_p2p_mode:%d}", en_p2p_mode);
    ((mac_cfg_start_vap_param_stru *)st_write_msg.auc_value)->en_mgmt_rate_init_flag = OAL_TRUE;

    /* 发送消息 */
    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_start_vap_param_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_TRUE,
                                   &pst_rsp_msg);
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_start_vap::wal_alloc_cfg_event return err code %d!}", l_ret);
        return l_ret;
    }
    /* 处理返回消息 */
    ul_err_code = wal_check_and_release_msg_resp(pst_rsp_msg);
    if (ul_err_code != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_start_vap::hmac start vap fail, err code[%d]!}", ul_err_code);
        return -OAL_EINVAL;
    }
    if ((oal_netdevice_flags(pst_net_dev) & OAL_IFF_RUNNING) == 0) {
        oal_netdevice_flags(pst_net_dev) |= OAL_IFF_RUNNING;
    }
    /* AP模式,启动VAP后,启动发送队列 */
    oal_net_tx_wake_all_queues(pst_net_dev); /* 启动发送队列 */

    return OAL_SUCC;
}


int32_t wal_stop_vap(oal_net_device_stru *pst_net_dev)
{
    wal_msg_write_stru st_write_msg;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;
    int32_t l_ret;
    wlan_p2p_mode_enum_uint8 en_p2p_mode;
    oal_wireless_dev_stru *pst_wdev = NULL;

    if (oal_unlikely(pst_net_dev == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_stop_vap::pst_net_dev null!}");
        return -OAL_EFAUL;
    }

    if ((oal_netdevice_flags(pst_net_dev) & OAL_IFF_RUNNING) == 0) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_stop_vap::vap is already down,continue to reset hmac vap state.}");
    }

    oal_io_print("wal_stop_vap,dev_name is:%s\n", pst_net_dev->name);

    /* 抛事件到wal层处理 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_DOWN_VAP, OAL_SIZEOF(mac_cfg_down_vap_param_stru));
    ((mac_cfg_down_vap_param_stru *)st_write_msg.auc_value)->pst_net_dev = pst_net_dev;
    pst_wdev = pst_net_dev->ieee80211_ptr;
    en_p2p_mode = wal_wireless_iftype_to_mac_p2p_mode(pst_wdev->iftype);
    if (WLAN_P2P_BUTT == en_p2p_mode) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_stop_vap::wal_wireless_iftype_to_mac_p2p_mode return BUFF}");
        return -OAL_EINVAL;
    }
    ((mac_cfg_start_vap_param_stru *)st_write_msg.auc_value)->en_p2p_mode = en_p2p_mode;

    oam_warning_log1(0, OAM_SF_ANY, "{wal_stop_vap::en_p2p_mode:%d}", en_p2p_mode);

    /* 发送消息 */
    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_down_vap_param_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_TRUE,
                                   &pst_rsp_msg);
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_stop_vap::wal_alloc_cfg_event return err code %d!}", l_ret);
        return l_ret;
    }

    if (OAL_SUCC != wal_check_and_release_msg_resp(pst_rsp_msg)) {
        oam_warning_log0(0, OAM_SF_ANY, "wal_stop_vap::wal_check_and_release_msg_resp fail");
    }

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_TAS_ANT_SWITCH
int32_t wal_init_WiTAS_state(void)
{
#ifdef _PRE_CONFIG_USE_DTS
    int32_t wifi_tas_state = 0;
    int32_t ret;
    struct device_node *np = NULL;

    if ((g_tas_switch_en[WLAN_RF_CHANNEL_ZERO] == OAL_FALSE) &&
        (g_tas_switch_en[WLAN_RF_CHANNEL_ONE] == OAL_FALSE)) {
        oam_warning_log0(0, OAM_SF_ANY, "{not support WiTAS}");
        return BOARD_SUCC;
    }
    ret = get_board_dts_node(&np, DTS_NODE_HI110X_WIFI);
    if (ret != BOARD_SUCC) {
        oam_error_log0(0, OAM_SF_ANY, "{DTS read node hisi_wifi fail!}");
        return BOARD_FAIL;
    }
    ret = of_property_read_u32(np, DTS_PROP_HI110X_WIFI_TAS_STATE, &wifi_tas_state);
    if (ret) {
        oam_warning_log0(0, OAM_SF_ANY, "{read prop gpio_wifi_tas_state fail!}");
        return BOARD_SUCC;
    }
    if (board_get_wifi_tas_gpio_state() != wifi_tas_state) {
        return board_wifi_tas_set(wifi_tas_state);
    } else {
        oam_warning_log0(0, OAM_SF_ANY, "{current WiTAS state is right, no need to set again!}");
        return BOARD_SUCC;
    }
#else
    return BOARD_SUCC;
#endif
}
#endif

#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE

OAL_STATIC void wal_init_probe_resp_status(mac_cfg_add_vap_param_stru *write_msg)
{
    int32_t ini_probe_resp_mode;
    oal_bool_enum_uint8 probe_resp_enable;
    mac_probe_resp_mode_enum_uint8 probe_resp_status;

    if (hmac_get_feature_switch(HMAC_MIRACAST_SINK_SWITCH)) {
        ini_probe_resp_mode = wlan_chip_get_probe_resp_mode();

        probe_resp_enable  = !!(oal_mask(4, 4) & (uint32_t)ini_probe_resp_mode);
        probe_resp_status  = (oal_mask(4, 0) & (uint32_t)ini_probe_resp_mode);

        write_msg->probe_resp_enable = probe_resp_enable;
        if (probe_resp_status >= MAC_PROBE_RESP_MODE_BUTT) {
            oam_warning_log1(0, OAM_SF_ANY, "{wal_init_probe_resp_status::read ini->probe_resp_status error[%d]}",
                probe_resp_status);
            probe_resp_status = MAC_PROBE_RESP_MODE_ACTIVE;
        }
        write_msg->probe_resp_status = probe_resp_status;
    }
}
#endif


int32_t wal_init_wlan_vap(oal_net_device_stru *pst_net_dev)
{
    oal_net_device_stru *pst_cfg_net_dev = NULL;
    wal_msg_write_stru st_write_msg;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;
    uint32_t ul_err_code;
    mac_vap_stru *pst_mac_vap = NULL;
    oal_wireless_dev_stru *pst_wdev = NULL;
    mac_wiphy_priv_stru *pst_wiphy_priv = NULL;
    mac_vap_stru *pst_cfg_mac_vap = NULL;
    hmac_vap_stru *pst_cfg_hmac_vap = NULL;
    mac_device_stru *pst_mac_device = NULL;
    int8_t ac_wlan_netdev_name[MAC_NET_DEVICE_NAME_LENGTH];
    int8_t ac_p2p_netdev_name[MAC_NET_DEVICE_NAME_LENGTH];
    int32_t l_ret;
    mac_cfg_add_vap_param_stru *add_vap_param = NULL;

    pst_mac_vap = oal_net_dev_priv(pst_net_dev);
    if (pst_mac_vap != NULL) {
        if (pst_mac_vap->en_vap_state != MAC_VAP_STATE_BUTT) {
            oam_warning_log0(0, OAM_SF_ANY, "{wal_init_wlan_vap::pst_mac_vap is already exist}");
            return OAL_SUCC;
        }
        /* netdev下的vap已经被删除，需要重新创建和挂载 */
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_init_wlan_vap::pst_mac_vap is already free!}");
        oal_net_dev_priv(pst_net_dev) = OAL_PTR_NULL;
    }

    pst_wdev = oal_netdevice_wdev(pst_net_dev);
    if (pst_wdev == NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_init_wlan_vap::pst_wdev is null!}");
        return -OAL_EFAUL;
    }

    pst_wiphy_priv = (mac_wiphy_priv_stru *)oal_wiphy_priv(pst_wdev->wiphy);
    pst_mac_device = pst_wiphy_priv->pst_mac_device;
    if (pst_mac_device == NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_init_wlan_vap::pst_mac_device is null!}");
        return -OAL_EFAUL;
    }

    /* 通过device id获取wlanX和 p2pX的netdev名 */
    snprintf_s(ac_wlan_netdev_name, NET_DEV_NAME_LEN, NET_DEV_NAME_LEN - 1, "wlan%d", pst_mac_device->uc_device_id);
    snprintf_s(ac_p2p_netdev_name, NET_DEV_NAME_LEN, NET_DEV_NAME_LEN - 1, "p2p%d", pst_mac_device->uc_device_id);

    pst_cfg_mac_vap = (mac_vap_stru *)mac_res_get_mac_vap(pst_mac_device->uc_cfg_vap_id);
    pst_cfg_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_device->uc_cfg_vap_id);
    if (oal_any_null_ptr2(pst_cfg_mac_vap, pst_cfg_hmac_vap)) {
        oam_warning_log4(0, OAM_SF_ANY,
            "{wal_init_wlan_vap::pst_cfg_mac_vap[%p] or pst_cfg_hmac_vap[%p] is null! vap_id:%d,deviceid[%d]}",
            (uintptr_t)pst_cfg_mac_vap, (uintptr_t)pst_cfg_hmac_vap,
            pst_mac_device->uc_cfg_vap_id, pst_mac_device->uc_device_id);
        return -OAL_EFAUL;
    }

    pst_cfg_net_dev = pst_cfg_hmac_vap->pst_net_device;
    if (pst_cfg_net_dev == NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_init_wlan_vap::pst_cfg_net_dev is null!}");
        return -OAL_EFAUL;
    }
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_ADD_VAP, OAL_SIZEOF(mac_cfg_add_vap_param_stru));
    add_vap_param = (mac_cfg_add_vap_param_stru *)st_write_msg.auc_value;
    add_vap_param->en_p2p_mode = WLAN_LEGACY_VAP_MODE;
    /* 仅用于WIFI和AP打开时创建VAP */
    if ((NL80211_IFTYPE_STATION == pst_wdev->iftype) || (NL80211_IFTYPE_P2P_DEVICE == pst_wdev->iftype)) {
        if (oal_any_zero_value2(oal_strcmp(ac_wlan_netdev_name, pst_net_dev->name),
            oal_strcmp(WLAN1_NETDEV_NAME, pst_net_dev->name))) {
            add_vap_param->en_vap_mode = WLAN_VAP_MODE_BSS_STA;
        }
        else if (0 == (oal_strcmp(ac_p2p_netdev_name, pst_net_dev->name))) {
            add_vap_param->en_vap_mode = WLAN_VAP_MODE_BSS_STA;
            add_vap_param->en_p2p_mode = WLAN_P2P_DEV_MODE;
        }
        else {
            oam_warning_log1(0, OAM_SF_ANY, "{wal_init_wlan_vap::net_dev is not wlan or p2p[%d]!}",
                pst_mac_device->uc_device_id);
            return OAL_SUCC;
        }
    } else if (NL80211_IFTYPE_AP == pst_wdev->iftype) {
        add_vap_param->en_vap_mode = WLAN_VAP_MODE_BSS_AP;
    } else {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_init_wlan_vap::net_dev is not wlan0 or p2p0!}");
        return OAL_SUCC;
    }

    oam_warning_log2(0, OAM_SF_ANY, "{wal_init_wlan_vap::vap_mode:%d, p2p_mode:%d}",
        add_vap_param->en_vap_mode, add_vap_param->en_p2p_mode);

    if (oal_strcmp(ac_wlan_netdev_name, pst_net_dev->name) == 0) {
        add_vap_param->is_primary_vap = 1;
#ifdef _PRE_WLAN_FEATURE_TAS_ANT_SWITCH
        l_ret = wal_init_WiTAS_state();
        if (l_ret != BOARD_SUCC) {
            oam_warning_log0(0, OAM_SF_ANY, "{wal_init_WiTAS_state ini WiTAS state fail!}");
        }
#endif
    } else {
        add_vap_param->is_primary_vap = 0;
    }
    /* 抛事件到wal层处理 */
    add_vap_param->pst_net_dev = pst_net_dev;
    add_vap_param->uc_cfg_vap_indx = pst_cfg_mac_vap->uc_vap_id;
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
    wal_vap_get_wlan_mode_para(&st_write_msg);
    wal_init_probe_resp_status((mac_cfg_add_vap_param_stru *)st_write_msg.auc_value);
#endif
    /* 发送消息 */
    l_ret = wal_send_cfg_event(pst_cfg_net_dev, WAL_MSG_TYPE_WRITE,
        WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_add_vap_param_stru),
        (uint8_t *)&st_write_msg, OAL_TRUE, &pst_rsp_msg);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(pst_cfg_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_init_wlan_vap::return err:%d!}", l_ret);
        return -OAL_EFAIL;
    }

    /* 读取返回的错误码 */
    ul_err_code = wal_check_and_release_msg_resp(pst_rsp_msg);
    if (ul_err_code != OAL_SUCC) {
        oam_warning_log1(pst_cfg_mac_vap->uc_vap_id, 0, "{wal_init_wlan_vap::hmac add vap fail[%u]}", ul_err_code);
        return -OAL_EFAIL;
    }

    if (OAL_SUCC != wal_set_random_mac_to_mib(pst_net_dev)) {
        oam_warning_log0(pst_cfg_mac_vap->uc_vap_id, 0, "{wal_init_wlan_vap::set_random_mac_to_mib fail}");
        return -OAL_EFAUL;
    }

    /* 设置netdevice的MAC地址，MAC地址在HMAC层被初始化到MIB中 */
    pst_mac_vap = oal_net_dev_priv(pst_net_dev);
    if (pst_mac_vap == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_init_wlan_vap::OAL_NET_DEV_PRIV(pst_net_dev) null.}");
        return -OAL_EINVAL;
    }

    if (add_vap_param->en_p2p_mode == WLAN_P2P_DEV_MODE) {
        pst_mac_device->st_p2p_info.uc_p2p0_vap_idx = pst_mac_vap->uc_vap_id;
    }

    if (NL80211_IFTYPE_AP == pst_wdev->iftype) {
        /* AP模式初始化，初始化配置最大用户数和mac地址过滤模式 */
        if (g_st_ap_config_info.ul_ap_max_user > 0) {
            wal_set_ap_max_user(pst_net_dev, g_st_ap_config_info.ul_ap_max_user);
        }

        if (OAL_STRLEN(g_st_ap_config_info.ac_ap_mac_filter_mode) > 0) {
            wal_config_mac_filter(pst_net_dev, g_st_ap_config_info.ac_ap_mac_filter_mode);
        }
    }

    return OAL_SUCC;
}


int32_t wal_deinit_wlan_vap(oal_net_device_stru *pst_net_dev)
{
    wal_msg_write_stru st_write_msg;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;
    mac_vap_stru *pst_mac_vap = NULL;
    int32_t l_ret;
    int32_t l_del_vap_flag = OAL_TRUE;
    int8_t ac_wlan_netdev_name[MAC_NET_DEVICE_NAME_LENGTH];
    int8_t ac_p2p_netdev_name[MAC_NET_DEVICE_NAME_LENGTH];
    mac_device_stru *pst_mac_dev = NULL;
    wlan_p2p_mode_enum_uint8 en_p2p_mode = WLAN_LEGACY_VAP_MODE;

    if (oal_unlikely(pst_net_dev == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_deinit_wlan_vap::pst_del_vap_param null ptr !}");
        return -OAL_EINVAL;
    }

    pst_mac_vap = oal_net_dev_priv(pst_net_dev);
    if (pst_mac_vap == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_deinit_wlan_vap::pst_mac_vap is already null}");
        return OAL_SUCC;
    }

    pst_mac_dev = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (pst_mac_dev == OAL_PTR_NULL) {
        oam_warning_log1(0, OAM_SF_ANY, "wal_deinit_wlan_vap::get mac dev id[%d] FAIL", pst_mac_vap->uc_device_id);
        return -OAL_EFAIL;
    }
    /* 通过device id获取netdev名字 */
    snprintf_s(ac_wlan_netdev_name, MAC_NET_DEVICE_NAME_LENGTH, MAC_NET_DEVICE_NAME_LENGTH - 1,
               "wlan%d", pst_mac_dev->uc_device_id);
    snprintf_s(ac_p2p_netdev_name, MAC_NET_DEVICE_NAME_LENGTH, MAC_NET_DEVICE_NAME_LENGTH - 1,
               "p2p%d", pst_mac_dev->uc_device_id);

    /* 仅用于WIFI和AP关闭时删除VAP */
    if ((0 != (oal_strcmp(ac_wlan_netdev_name, pst_net_dev->name)))
        && (0 != (oal_strcmp(ac_p2p_netdev_name, pst_net_dev->name)))
        && (0 != (oal_strcmp(WLAN1_NETDEV_NAME, pst_net_dev->name)))) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_deinit_wlan_vap::net_dev is not wlan or p2p!}");
        return OAL_SUCC;
    }

    if (0 == oal_strcmp(ac_p2p_netdev_name, pst_net_dev->name)) {
        en_p2p_mode = WLAN_P2P_DEV_MODE;
    }

    oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_deinit_wlan_vap::en_p2p_mode:%d}", en_p2p_mode);

    /* 抛事件到wal层处理 */
    memset_s(&st_write_msg, OAL_SIZEOF(wal_msg_write_stru), 0, OAL_SIZEOF(wal_msg_write_stru));

    ((mac_cfg_del_vap_param_stru *)st_write_msg.auc_value)->pst_net_dev = pst_net_dev;
    ((mac_cfg_del_vap_param_stru *)st_write_msg.auc_value)->en_p2p_mode = en_p2p_mode;

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_DEL_VAP, OAL_SIZEOF(mac_cfg_del_vap_param_stru));
    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_del_vap_param_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_TRUE,
                                   &pst_rsp_msg);

    if (OAL_SUCC != wal_check_and_release_msg_resp(pst_rsp_msg)) {
        oam_warning_log0(0, OAM_SF_ANY, "wal_deinit_wlan_vap::wal_check_and_release_msg_resp fail.");
        /* can't set net dev's vap ptr to null when
          del vap wid process failed! */
        l_del_vap_flag = OAL_FALSE;
    }

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_deinit_wlan_vap::return error code %d}", l_ret);
        if (l_ret == -OAL_ENOMEM || l_ret == -OAL_EFAIL) {
            /* wid had't processed */
            l_del_vap_flag = OAL_FALSE;
        }
    }

    if (l_del_vap_flag == OAL_TRUE) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_deinit_wlan_vap::clear netdev priv.}");
        oal_net_dev_priv(pst_net_dev) = NULL;
    }

    return l_ret;
}


OAL_STATIC int32_t wal_set_mac_addr(oal_net_device_stru *pst_net_dev)
{
    uint8_t auc_primary_mac_addr[WLAN_MAC_ADDR_LEN] = { 0 }; /* MAC地址 */
    oal_wireless_dev_stru *pst_wdev;
    mac_wiphy_priv_stru *pst_wiphy_priv;
    mac_device_stru *pst_mac_device;

    pst_wdev = oal_netdevice_wdev(pst_net_dev);
    pst_wiphy_priv = (mac_wiphy_priv_stru *)(oal_wiphy_priv(pst_wdev->wiphy));
    pst_mac_device = pst_wiphy_priv->pst_mac_device;

    if (oal_unlikely(pst_mac_device->st_p2p_info.pst_primary_net_device == OAL_PTR_NULL)) {
        /* random mac will be used. hi1102-cb (#include <linux/etherdevice.h>)    */
        oal_random_ether_addr(auc_primary_mac_addr);
        auc_primary_mac_addr[0] &= (~0x02);
        auc_primary_mac_addr[1] = 0x11;
        auc_primary_mac_addr[2] = 0x02;
    } else {
#ifndef _PRE_PC_LINT
        if (oal_likely(OAL_PTR_NULL != oal_netdevice_mac_addr(pst_mac_device->st_p2p_info.pst_primary_net_device))) {
            if (EOK != memcpy_s(auc_primary_mac_addr, WLAN_MAC_ADDR_LEN,
                                oal_netdevice_mac_addr(pst_mac_device->st_p2p_info.pst_primary_net_device), WLAN_MAC_ADDR_LEN)) {
                oam_error_log0(0, OAM_SF_ANY, "wal_set_mac_addr::memcpy fail!");
                return OAL_FAIL;
            }
        } else {
            oam_warning_log0(0, OAM_SF_ANY, "{wal_get_mac_addr() pst_primary_net_device; addr is null}");
            return OAL_FAIL;
        }
#endif
    }

    switch (pst_wdev->iftype) {
        case NL80211_IFTYPE_P2P_DEVICE: {
            /* 产生P2P device MAC 地址，将本地mac 地址bit 设置为1 */
            auc_primary_mac_addr[0] |= 0x02;
            oal_set_mac_addr((uint8_t *)oal_netdevice_mac_addr(pst_net_dev), auc_primary_mac_addr);
            break;
        }
        default: {
            if (oal_strcmp(WLAN1_NETDEV_NAME, pst_net_dev->name) == 0) {
                auc_primary_mac_addr[3] ^= 0x80;
            } else {
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
                hwifi_get_mac_addr(auc_primary_mac_addr);
                auc_primary_mac_addr[0] &= (~0x02);
#else
                oal_random_ether_addr(auc_primary_mac_addr);
                auc_primary_mac_addr[0] &= (~0x02);
                auc_primary_mac_addr[1] = 0x11;
                auc_primary_mac_addr[2] = 0x02;
#endif /* #ifdef _PRE_PLAT_FEATURE_CUSTOMIZE */
            }
            oal_set_mac_addr((uint8_t *)oal_netdevice_mac_addr(pst_net_dev), auc_primary_mac_addr);
            break;
        }
    }

    return OAL_SUCC;
}

OAL_STATIC void wal_set_netdev_priv(oal_netdev_priv_stru *pst_netdev_priv)
{
#ifdef _PRE_CONFIG_HISI_110X_NAPI_DISABLE
    pst_netdev_priv->uc_napi_enable     = OAL_FALSE;
    pst_netdev_priv->uc_gro_enable      = OAL_FALSE;
#else
    pst_netdev_priv->uc_napi_enable = OAL_TRUE;
    pst_netdev_priv->uc_gro_enable = OAL_TRUE;
#endif
    pst_netdev_priv->uc_napi_weight = NAPI_POLL_WEIGHT_LEV1;
    pst_netdev_priv->ul_queue_len_max = NAPI_NETDEV_PRIV_QUEUE_LEN_MAX;
    pst_netdev_priv->uc_state = 0;
    pst_netdev_priv->ul_period_pkts = 0;
    pst_netdev_priv->ul_period_start = 0;
}


OAL_STATIC void wal_init_wlan_netdev_set_netdev(oal_net_device_stru *pst_net_dev, oal_wireless_dev_stru *pst_wdev)
{
#ifdef _PRE_WLAN_FEATURE_GSO
    oam_warning_log0(0, OAM_SF_ANY, "{wal_init_wlan_netdev::[GSO] add NETIF_F_SG}");
    pst_net_dev->features |= NETIF_F_SG;
    pst_net_dev->hw_features |= NETIF_F_SG;
#endif
 /* 对netdevice进行赋值 */
#ifdef CONFIG_WIRELESS_EXT
    pst_net_dev->wireless_handlers = &g_st_iw_handler_def;
#endif
    pst_net_dev->netdev_ops = &g_st_wal_net_dev_ops;
    pst_net_dev->ethtool_ops = &g_st_wal_ethtool_ops;
    oal_netdevice_destructor(pst_net_dev) = oal_net_free_netdev;

    /* host发送报文使能csum硬化功能 */
    if (g_tx_switch.tx_switch == HOST_TX) {
        pst_net_dev->features    |= NETIF_F_IP_CSUM | NETIF_F_IPV6_CSUM;
        pst_net_dev->hw_features |= NETIF_F_IP_CSUM | NETIF_F_IPV6_CSUM;
    }

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 10, 44))
    oal_netdevice_master(pst_net_dev) = OAL_PTR_NULL;
#endif

    oal_netdevice_ifalias(pst_net_dev) = OAL_PTR_NULL;
    oal_netdevice_watchdog_timeo(pst_net_dev) = 5;
    oal_netdevice_wdev(pst_net_dev) = pst_wdev;
    oal_netdevice_qdisc(pst_net_dev, OAL_PTR_NULL);
}


int32_t wal_init_wlan_netdev(oal_wiphy_stru *pst_wiphy, const char *dev_name)
{
    mac_device_stru *pst_mac_device = NULL;
    int8_t ac_wlan_netdev_name[MAC_NET_DEVICE_NAME_LENGTH];
    int8_t ac_p2p_netdev_name[MAC_NET_DEVICE_NAME_LENGTH];
    oal_net_device_stru *pst_net_dev = NULL;
    oal_wireless_dev_stru *pst_wdev = NULL;
    mac_wiphy_priv_stru *pst_wiphy_priv = NULL;
    enum nl80211_iftype en_type;
    int32_t l_ret;
    oal_netdev_priv_stru *pst_netdev_priv = NULL;

    if (oal_any_null_ptr2(pst_wiphy, dev_name)) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_init_wlan_netdev::pst_wiphy or dev_name is null!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_wiphy_priv = (mac_wiphy_priv_stru *)(oal_wiphy_priv(pst_wiphy));
    if (pst_wiphy_priv == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_init_wlan_netdev::pst_wiphy_priv is null!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_mac_device = pst_wiphy_priv->pst_mac_device;
    if (pst_mac_device == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_init_wlan_netdev::pst_wiphy_priv is null!}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    /* 通过device id获取netdev名字 */
    snprintf_s(ac_wlan_netdev_name, MAC_NET_DEVICE_NAME_LENGTH, MAC_NET_DEVICE_NAME_LENGTH - 1,
               "wlan%d", pst_mac_device->uc_device_id);
    snprintf_s(ac_p2p_netdev_name, MAC_NET_DEVICE_NAME_LENGTH, MAC_NET_DEVICE_NAME_LENGTH - 1,
               "p2p%d", pst_mac_device->uc_device_id);

    /* 信道跟随--添加hwlan name判断 */
    if (0 == (oal_strcmp(ac_wlan_netdev_name, dev_name)) || 0 == (oal_strcmp(WLAN1_NETDEV_NAME, dev_name))) {
        en_type = NL80211_IFTYPE_STATION;
    } else if (0 == (oal_strcmp(ac_p2p_netdev_name, dev_name))) {
        en_type = NL80211_IFTYPE_P2P_DEVICE;
    } else {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_init_wlan_netdev::dev name is not wlan or p2p}");
        return OAL_SUCC;
    }
    oam_warning_log1(0, OAM_SF_ANY, "{wal_init_wlan_netdev::en_type is %d}", en_type);

    /* 如果创建的net device已经存在，直接返回 */
    /* 根据dev_name找到dev */
    pst_net_dev = wal_config_get_netdev(dev_name, OAL_STRLEN(dev_name));
    if (pst_net_dev != OAL_PTR_NULL) {
        /* 调用oal_dev_get_by_name后，必须调用oal_dev_put使net_dev的引用计数减一 */
        oal_dev_put(pst_net_dev);

        oam_warning_log0(0, OAM_SF_ANY, "{wal_init_wlan_netdev::the net_device is already exist!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 此函数第一个入参代表私有长度，此处不涉及为0 */
    pst_net_dev = oal_net_alloc_netdev_mqs(OAL_SIZEOF(oal_netdev_priv_stru),
        dev_name, oal_ether_setup, WLAN_NET_QUEUE_BUTT, 1);

    if (oal_unlikely(pst_net_dev == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_init_wlan_netdev::oal_net_alloc_netdev return null!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_wdev = (oal_wireless_dev_stru *)oal_mem_alloc_m(OAL_MEM_POOL_ID_LOCAL,
        OAL_SIZEOF(oal_wireless_dev_stru), OAL_FALSE);
    if (oal_unlikely(pst_wdev == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_init_wlan_netdev::alloc mem, pst_wdev null!}");
        oal_net_free_netdev(pst_net_dev);
        return OAL_ERR_CODE_PTR_NULL;
    }

    memset_s(pst_wdev, OAL_SIZEOF(oal_wireless_dev_stru), 0, OAL_SIZEOF(oal_wireless_dev_stru));
    wal_init_wlan_netdev_set_netdev(pst_net_dev, pst_wdev);
    pst_wdev->netdev = pst_net_dev;
    pst_wdev->iftype = en_type;
    pst_wdev->wiphy = pst_wiphy;
    pst_wiphy_priv = (mac_wiphy_priv_stru *)(oal_wiphy_priv(pst_wiphy));

    /* 信道跟随--add wlan name 判断 */
    if ((NL80211_IFTYPE_STATION == en_type) && (0 == (oal_strcmp(ac_wlan_netdev_name, dev_name)))) {
        /* 如果创建wlan0， 则保存wlan0 为主net_device,p2p0 和p2p-p2p0 MAC 地址从主netdevice 获取 */
        pst_wiphy_priv->pst_mac_device->st_p2p_info.pst_primary_net_device = pst_net_dev;
    } else if ((NL80211_IFTYPE_STATION == en_type) && (0 == (oal_strcmp(WLAN1_NETDEV_NAME, dev_name)))) {
        /* netdevice 指针暂时挂载，未使用 */
        pst_wiphy_priv->pst_mac_device->st_p2p_info.pst_second_net_device = pst_net_dev;
    } else if (NL80211_IFTYPE_P2P_DEVICE == en_type) {
        pst_wiphy_priv->pst_mac_device->st_p2p_info.pst_p2p_net_device = pst_net_dev;
    }
    oal_netdevice_flags(pst_net_dev) &= ~OAL_IFF_RUNNING; /* 将net device的flag设为down */

    if (OAL_SUCC != wal_set_mac_addr(pst_net_dev)) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_init_wlan_netdev::set mac addr fail}");
    }

    pst_netdev_priv = (oal_netdev_priv_stru *)oal_net_dev_wireless_priv(pst_net_dev);
    wal_set_netdev_priv(pst_netdev_priv);

    oal_netbuf_list_head_init(&pst_netdev_priv->st_rx_netbuf_queue);
    oal_netif_napi_add(pst_net_dev, &pst_netdev_priv->st_napi, hmac_rxdata_polling, NAPI_POLL_WEIGHT_LEV1);
    /* 注册net_device */
    l_ret = hmac_net_register_netdev(pst_net_dev);
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_init_wlan_netdev::hmac_net_register_netdev err:%d!}", l_ret);

        oal_mem_free_m(pst_wdev, OAL_FALSE);
        oal_net_free_netdev(pst_net_dev);

        return l_ret;
    }

    return OAL_SUCC;
}


int32_t wal_setup_ap(oal_net_device_stru *pst_net_dev)
{
    int32_t l_ret;
    mac_vap_stru *pst_mac_vap = NULL;
    wal_set_power_mgmt_on(OAL_FALSE);
    l_ret = wal_set_power_on(pst_net_dev, OAL_TRUE);
    if (l_ret != OAL_SUCC) {
        oam_error_log1(0, OAM_SF_CFG, "{wal_setup_ap::wal_set_power_on fail [%d]!}", l_ret);
        return l_ret;
    }
    if (oal_netdevice_flags(pst_net_dev) & OAL_IFF_RUNNING) {
        /* 切换到AP前如果网络设备处于UP状态，需要先down wlan0网络设备 */
        oal_io_print("wal_setup_ap:stop netdevice:%.16s", pst_net_dev->name);
        oam_warning_log0(0, OAM_SF_CFG, "{wal_setup_ap:: oal_iff_running! now, stop netdevice}");
        wal_netdev_stop(pst_net_dev);
    }

    pst_mac_vap = oal_net_dev_priv(pst_net_dev);
    if (pst_mac_vap) {
        if (pst_mac_vap->en_vap_state != MAC_VAP_STATE_INIT) {
            /* 切换到AP前如果网络设备处于UP状态，需要先down wlan0网络设备 */
            oal_io_print("wal_setup_ap:stop netdevice:%s", pst_net_dev->name);
            wal_netdev_stop(pst_net_dev);
        }
    }

    pst_net_dev->ieee80211_ptr->iftype = NL80211_IFTYPE_AP;

    l_ret = wal_init_wlan_vap(pst_net_dev);

#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
    if (l_ret == OAL_SUCC) {
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
        /* 低功耗定制化开关 */
        (g_wlan_host_pm_switch == OAL_TRUE) ? wlan_pm_enable() : wlan_pm_disable();
#endif
    }
#endif /* #ifdef _PRE_PLAT_FEATURE_CUSTOMIZE */
    return l_ret;
}
#ifdef _PRE_WLAN_FEATURE_NAN
OAL_STATIC void wal_nan_netdev_setup(oal_net_device_stru *net_dev, oal_wireless_dev_stru *wdev,
    oal_wiphy_stru *wiphy)
{
    memset_s(wdev, OAL_SIZEOF(oal_wireless_dev_stru), 0, OAL_SIZEOF(oal_wireless_dev_stru));
    wdev->netdev = net_dev;
    wdev->iftype = NL80211_IFTYPE_STATION;
    wdev->wiphy = wiphy;

    net_dev->netdev_ops = &g_nan_net_dev_ops;
    net_dev->ethtool_ops = &g_st_wal_ethtool_ops;
    oal_netdevice_destructor(net_dev) = oal_net_free_netdev;
    oal_netdevice_ifalias(net_dev) = OAL_PTR_NULL;
    oal_netdevice_watchdog_timeo(net_dev) = 5;
    oal_netdevice_wdev(net_dev) = wdev;
    oal_netdevice_qdisc(net_dev, OAL_PTR_NULL);
    /* 将net device的flag设为down */
    oal_netdevice_flags(net_dev) &= ~OAL_IFF_RUNNING;
}


void wal_init_nan_netdev(oal_wiphy_stru *wiphy)
{
    oal_net_device_stru *net_dev = NULL;
    oal_wireless_dev_stru *wdev = NULL;
    int32_t l_ret;
    const char *dev_name = "nan0";

    if ((g_optimized_feature_switch_bitmap & BIT(CUSTOM_OPTIMIZE_FEATURE_NAN)) == 0) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_cfg80211_init_nan_netdev::NAN unsupported, Not create nan netdev!}");
        return;
    }
#ifdef CONFIG_HISI_CMDLINE_PARSE
    if (get_boot_into_recovery_flag()) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_cfg80211_init_nan_netdev::recovery mode, Not create nan netdev!}");
        return;
    }
#endif
    /* 如果创建的net device已经存在，直接返回 */
    net_dev = wal_config_get_netdev(dev_name, OAL_STRLEN(dev_name));
    if (net_dev != OAL_PTR_NULL) {
        /* 调用oal_dev_get_by_name后，必须调用oal_dev_put使net_dev的引用计数减一 */
        oal_dev_put(net_dev);
        oam_warning_log0(0, OAM_SF_ANY, "{wal_cfg80211_init_nan_netdev::nan net_device is already exist!}");
        return;
    }

    /* 此函数第一个入参代表私有长度，此处不涉及为0 */
    net_dev = oal_net_alloc_netdev(0, dev_name, oal_ether_setup);
    if (oal_unlikely(net_dev == OAL_PTR_NULL)) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_cfg80211_init_nan_netdev::oal_net_alloc_netdev return null!}");
        return;
    }

    wdev = (oal_wireless_dev_stru *)oal_mem_alloc_m(OAL_MEM_POOL_ID_LOCAL,
        OAL_SIZEOF(oal_wireless_dev_stru), OAL_FALSE);
    if (oal_unlikely(wdev == OAL_PTR_NULL)) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_cfg80211_init_nan_netdev::alloc wdev fail!}");
        oal_net_free_netdev(net_dev);
        return;
    }

    wal_nan_netdev_setup(net_dev, wdev, wiphy);

    /* 注册net_device */
    l_ret = hmac_net_register_netdev(net_dev);
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_cfg80211_init_nan_netdev::hmac_net_register_netdev err:%d!}", l_ret);
        oal_mem_free_m(wdev, OAL_FALSE);
        oal_net_free_netdev(net_dev);
        return;
    }
    oam_warning_log0(0, OAM_SF_ANY, "{wal_cfg80211_init_nan_netdev::add nan netdev succ!}");
}
#endif


uint32_t wal_hipriv_register_inetaddr_notifier(void)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    if (0 == register_inetaddr_notifier(&wal_hipriv_notifier)) {
        return OAL_SUCC;
    }

    oam_error_log0(0, OAM_SF_PWR, "{wal_hipriv_register_inetaddr_notifier::register inetaddr notifier failed.}");
    return OAL_FAIL;

#else
    return OAL_SUCC;
#endif
}


uint32_t wal_hipriv_unregister_inetaddr_notifier(void)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    if (0 == unregister_inetaddr_notifier(&wal_hipriv_notifier)) {
        return OAL_SUCC;
    }

    oam_error_log0(0, 0, "{wal_hipriv_unregister_inetaddr_notifier::hmac_unregister inetaddr notifier fail}");
    return OAL_FAIL;

#else
    return OAL_SUCC;
#endif
}


uint32_t wal_hipriv_register_inet6addr_notifier(void)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    if (0 == register_inet6addr_notifier(&wal_hipriv_notifier_ipv6)) {
        return OAL_SUCC;
    }

    oam_error_log0(0, OAM_SF_PWR, "{wal_hipriv_register_inet6addr_notifier::register inetaddr6 notifier failed.}");
    return OAL_FAIL;

#else
    return OAL_SUCC;
#endif
}


uint32_t wal_hipriv_unregister_inet6addr_notifier(void)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    if (0 == unregister_inet6addr_notifier(&wal_hipriv_notifier_ipv6)) {
        return OAL_SUCC;
    }

    oam_error_log0(0, OAM_SF_PWR, "{wal_hipriv_unregister_inet6addr_notifier::unreg inetaddr6 notifier fail}");
    return OAL_FAIL;

#else
    return OAL_SUCC;
#endif
}

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)

int32_t wal_hipriv_inetaddr_notifier_call(struct notifier_block *this, unsigned long event, void *ptr)
{
    /*
     * Notification mechanism from kernel to our driver. This function is called by the Linux kernel
     * whenever there is an event related to an IP address.
     * ptr : kernel provided pointer to IP address that has changed
     */
    struct in_ifaddr *pst_ifa = (struct in_ifaddr *)ptr;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;

    if (oal_unlikely(pst_ifa == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_PWR, "{wal_hipriv_inetaddr_notifier_call::pst_ifa is NULL.}");
        return NOTIFY_DONE;
    }
    if (oal_unlikely(pst_ifa->ifa_dev->dev == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_PWR, "{wal_hipriv_inet6addr_notifier_call::pst_ifa->idev->dev is NULL.}");
        return NOTIFY_DONE;
    }

    /* Filter notifications meant for non Hislicon devices */
    if (pst_ifa->ifa_dev->dev->netdev_ops != &g_st_wal_net_dev_ops) {
        return NOTIFY_DONE;
    }

    pst_mac_vap = (mac_vap_stru *)oal_net_dev_priv(pst_ifa->ifa_dev->dev);
    if (oal_unlikely(pst_mac_vap == NULL)) {
        oam_warning_log1(0, OAM_SF_PWR, "{wal_hipriv_inetaddr_notifier_call::Get mac vap failed, \
            when %d(UP:1 DOWN:2 UNKNOWN:others) ipv4 address.}", event);
        return NOTIFY_DONE;
    }

    if (ipv4_is_linklocal_169(pst_ifa->ifa_address)) {
        oam_warning_log3(pst_mac_vap->uc_vap_id, OAM_SF_PWR, "{wal_hipriv_inetaddr_notifier_call:: \
            Invalid IPv4[%d.X.X.%d], MASK[0x%08X].}", ((uint8_t *)&(pst_ifa->ifa_address))[0],
            ((uint8_t *)&(pst_ifa->ifa_address))[3], pst_ifa->ifa_mask); /* 3 netaddr */
        return NOTIFY_DONE;
    }

    wal_wake_lock();

    switch (event) {
        case NETDEV_UP: {
            oam_warning_log3(pst_mac_vap->uc_vap_id, OAM_SF_PWR, "{wal_hipriv_inetaddr_notifier_call:: \
                Up IPv4[%d.X.X.%d], MASK[0x%08X].}", ((uint8_t *)&(pst_ifa->ifa_address))[0],
                ((uint8_t *)&(pst_ifa->ifa_address))[3], pst_ifa->ifa_mask); /* 3 netaddr */
            hmac_arp_offload_set_ip_addr(pst_mac_vap, DMAC_CONFIG_IPV4,
                DMAC_IP_ADDR_ADD, &(pst_ifa->ifa_address), &(pst_ifa->ifa_mask));

            if (pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_STA) {
                pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
                if (pst_hmac_vap == OAL_PTR_NULL) {
                    oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
                        "{wal_hipriv_inetaddr_notifier_call::hmac_vap null.vap[%d]}", pst_mac_vap->uc_vap_id);
                    break;
                }
                /* 获取到IP地址的时候开启低功耗 */
                if ((pst_hmac_vap->uc_ps_mode == MAX_FAST_PS) || (pst_hmac_vap->uc_ps_mode == AUTO_FAST_PS)) {
                    wlan_pm_set_timeout((g_wlan_min_fast_ps_idle > 1) ?
                        (g_wlan_min_fast_ps_idle - 1) : g_wlan_min_fast_ps_idle);
                } else {
                    wlan_pm_set_timeout(WLAN_SLEEP_DEFAULT_CHECK_CNT);
                }
                pst_hmac_vap->ipaddr_obtained = OAL_TRUE;
                /* 获取到IP地址的时候通知漫游计时 */
                hmac_roam_wpas_connect_state_notify(pst_hmac_vap, WPAS_CONNECT_STATE_IPADDR_OBTAINED);
            }
            break;
        }

        case NETDEV_DOWN: {
            wlan_pm_set_timeout(WLAN_SLEEP_LONG_CHECK_CNT);
            oam_warning_log3(pst_mac_vap->uc_vap_id, OAM_SF_PWR, "{wal_hipriv_inetaddr_notifier_call::\
                Down IPv4[%d.X.X.%d], MASK[0x%08X]..}", ((uint8_t *)&(pst_ifa->ifa_address))[0],
                ((uint8_t *)&(pst_ifa->ifa_address))[3], pst_ifa->ifa_mask); /* 3 netaddr */
            hmac_arp_offload_set_ip_addr(pst_mac_vap, DMAC_CONFIG_IPV4, DMAC_IP_ADDR_DEL,
                &(pst_ifa->ifa_address), &(pst_ifa->ifa_mask));

            if (pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_STA) {
                /* 获取到IP地址的时候通知漫游计时 */
                pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
                if (pst_hmac_vap == OAL_PTR_NULL) {
                    oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
                        "{wal_hipriv_inetaddr_notifier_call::hmac_vap null.[%d]}", pst_mac_vap->uc_vap_id);
                    break;
                }
                pst_hmac_vap->ipaddr_obtained = OAL_FALSE;
                hmac_roam_wpas_connect_state_notify(pst_hmac_vap, WPAS_CONNECT_STATE_IPADDR_REMOVED);
            }
            break;
        }

        default: {
            oam_error_log1(pst_mac_vap->uc_vap_id, 0, "{wal_hipriv_inetaddr_notifier_call::Unknown event[%d]}", event);
            break;
        }
    }
    wal_wake_unlock();

    return NOTIFY_DONE;
}


int32_t wal_hipriv_inet6addr_notifier_call(struct notifier_block *this, unsigned long event, void *ptr)
{
    /*
     * Notification mechanism from kernel to our driver. This function is called by the Linux kernel
     * whenever there is an event related to an IP address.
     * ptr : kernel provided pointer to IP address that has changed
     */
    struct inet6_ifaddr *pst_ifa = (struct inet6_ifaddr *)ptr;
    mac_vap_stru *pst_mac_vap = NULL;

    if (oal_unlikely(pst_ifa == NULL)) {
        oam_error_log0(0, OAM_SF_PWR, "{wal_hipriv_inet6addr_notifier_call::pst_ifa is NULL.}");
        return NOTIFY_DONE;
    }
    if (oal_unlikely(pst_ifa->idev->dev == NULL)) {
        oam_error_log0(0, OAM_SF_PWR, "{wal_hipriv_inet6addr_notifier_call::pst_ifa->idev->dev is NULL.}");
        return NOTIFY_DONE;
    }

    /* Filter notifications meant for non Hislicon devices */
    if (pst_ifa->idev->dev->netdev_ops != &g_st_wal_net_dev_ops) {
        return NOTIFY_DONE;
    }

    pst_mac_vap = (mac_vap_stru *)oal_net_dev_priv(pst_ifa->idev->dev);
    if (oal_unlikely(pst_mac_vap == NULL)) {
        oam_warning_log1(0, OAM_SF_PWR,
            "{wal_hipriv_inet6addr_notifier_call::vap null,when %d(UP:1 DOWN:2 UNKNOWN:others) ipv6 addr}", event);
        return NOTIFY_DONE;
    }

    switch (event) {
        case NETDEV_UP: {
            oam_warning_log4(pst_mac_vap->uc_vap_id, OAM_SF_PWR,
                "{wal_hipriv_inet6addr_notifier_call::UP IPv6[%04x:%04x:XXXX:XXXX:XXXX:XXXX:%04x:%04x].}",
                oal_net2host_short((pst_ifa->addr.s6_addr16)[0]), oal_net2host_short((pst_ifa->addr.s6_addr16)[1]),
                oal_net2host_short((pst_ifa->addr.s6_addr16)[6]),  /* 6 net6addr */
                oal_net2host_short((pst_ifa->addr.s6_addr16)[7])); /* 7 net6addr */
            hmac_arp_offload_set_ip_addr(pst_mac_vap, DMAC_CONFIG_IPV6, DMAC_IP_ADDR_ADD,
                &(pst_ifa->addr), &(pst_ifa->addr));
            break;
        }

        case NETDEV_DOWN: {
            oam_warning_log4(pst_mac_vap->uc_vap_id, OAM_SF_PWR,
                "{wal_hipriv_inet6addr_notifier_call::DOWN IPv6[%04x:%04x:XXXX:XXXX:XXXX:XXXX:%04x:%04x].}",
                oal_net2host_short((pst_ifa->addr.s6_addr16)[0]), oal_net2host_short((pst_ifa->addr.s6_addr16)[1]),
                oal_net2host_short((pst_ifa->addr.s6_addr16)[6]),  /* 6 net6addr */
                oal_net2host_short((pst_ifa->addr.s6_addr16)[7])); /* 7 net6addr */
            hmac_arp_offload_set_ip_addr(pst_mac_vap, DMAC_CONFIG_IPV6, DMAC_IP_ADDR_DEL,
                &(pst_ifa->addr), &(pst_ifa->addr));
            break;
        }

        default: {
            oam_error_log1(pst_mac_vap->uc_vap_id, 0, "{wal_hipriv_inet6addr_notifier_call::event:%d}", event);
            break;
        }
    }

    return NOTIFY_DONE;
}
#endif /* #if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION) */
#ifdef _PRE_WLAN_FEATURE_11K

OAL_STATIC uint32_t wal_hipriv_send_neighbor_req(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_offset = 0;
    uint32_t ul_ret;
    int32_t l_ret;
    int8_t ac_arg[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    mac_cfg_ssid_param_stru *pst_ssid = NULL;
    uint8_t uc_str_len;

    uc_str_len = os_str_len(pc_param);
    uc_str_len = (uc_str_len > 1) ? uc_str_len - 1 : uc_str_len;

    /* 获取SSID字符串 */
    if (uc_str_len != 0) {
        ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_offset);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_send_neighbor_req::get cmd one arg err[%d]!}", ul_ret);
            return ul_ret;
        }
    }

    /* 抛事件到wal层处理 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SEND_NEIGHBOR_REQ, OAL_SIZEOF(mac_cfg_ssid_param_stru));
    pst_ssid = (mac_cfg_ssid_param_stru *)st_write_msg.auc_value;
    pst_ssid->uc_ssid_len = uc_str_len;
    l_ret = memcpy_s(pst_ssid->ac_ssid, WLAN_SSID_MAX_LEN, ac_arg, pst_ssid->uc_ssid_len);
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "wal_hipriv_send_neighbor_req::memcpy fail!");
        return OAL_FAIL;
    }

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_ssid_param_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_send_neighbor_req::return err code [%d]!}", l_ret);
        return (uint32_t)l_ret;
    }
    return OAL_SUCC;
}

OAL_STATIC uint32_t wal_hipriv_beacon_req_table_switch(oal_net_device_stru *pst_net_dev,
    int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_offset = 0;
    uint32_t ul_ret;
    int32_t l_ret;
    int8_t ac_arg[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint8_t uc_switch;

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_offset);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_beacon_req_table_switch::get cmd one arg err[%d]!}", ul_ret);
        return ul_ret;
    }

    uc_switch = (uint8_t)oal_atoi(ac_arg);

    /* 抛事件到wal层处理 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_BCN_TABLE_SWITCH, OAL_SIZEOF(uint8_t));
    st_write_msg.auc_value[0] = uc_switch;
    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(uint8_t),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_beacon_req_table_switch::return err code [%d]!}", l_ret);
        return (uint32_t)l_ret;
    }
    return OAL_SUCC;
}
#endif  // _PRE_WLAN_FEATURE_11K


OAL_STATIC uint32_t wal_hipriv_voe_enable(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_offset = 0;
    uint32_t ul_ret;
    int32_t l_ret;
    int8_t ac_arg[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint16_t us_switch;
    uint16_t *ps_value = OAL_PTR_NULL;

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_offset);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_voe_enable::get cmd one arg err[%d]!}", ul_ret);
        return ul_ret;
    }

    us_switch = (uint16_t)oal_atoi(ac_arg);
    us_switch = us_switch & 0xFFFF;

    /* 抛事件到wal层处理 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_VOE_ENABLE, OAL_SIZEOF(uint16_t));
    ps_value = (uint16_t *)st_write_msg.auc_value;
    *ps_value = us_switch;
    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(uint16_t),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_voe_enable::return err code [%d]!}", l_ret);
        return (uint32_t)l_ret;
    }
    return OAL_SUCC;
}


int32_t wal_set_assigned_filter_enable(int32_t l_filter_id, int32_t l_on)
{
    uint16_t us_len;
    int32_t l_ret;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    oal_net_device_stru *pst_net_dev = OAL_PTR_NULL;
    wal_msg_write_stru st_write_msg;
    mac_assigned_filter_cmd_stru st_assigned_filter_cmd;

#ifdef _PRE_WLAN_FEATURE_DFR
    if (g_st_dfr_info.bit_device_reset_process_flag) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_set_assigned_filter_enable:: dfr_process_status[%d]!}",
                         g_st_dfr_info.bit_device_reset_process_flag);
        return -OAL_EFAIL;
    }
#endif  // #ifdef _PRE_WLAN_FEATURE_DFR

    if (l_on < 0) {
        oam_error_log1(0, OAM_SF_ANY, "{wal_set_assigned_filter_enable::Invalid input, on/off %d!}", l_on);
        return -OAL_EINVAL;
    }

    pst_net_dev = wal_config_get_netdev("wlan0", OAL_STRLEN("wlan0"));
    if (pst_net_dev == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_set_assigned_filter_enable::wlan0 not exist!}");
        return -OAL_EINVAL;
    }
    /* 调用oal_dev_get_by_name后，必须调用oal_dev_put使net_dev的引用计数减一 */
    oal_dev_put(pst_net_dev);

    /* vap未创建时，不处理下发的命令 */
    pst_mac_vap = oal_net_dev_priv(pst_net_dev);
    if (pst_mac_vap == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_set_assigned_filter_enable::vap not created yet, ignore the cmd!}");
        return -OAL_EINVAL;
    }

    if (pst_mac_vap->st_cap_flag.bit_icmp_filter != OAL_TRUE) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_set_assigned_filter_enable::Func not enable!}");
        return -OAL_EINVAL;
    }

    /* 准备配置命令 */
    us_len = OAL_SIZEOF(st_assigned_filter_cmd);
    memset_s((uint8_t *)&st_assigned_filter_cmd, us_len, 0, us_len);
    st_assigned_filter_cmd.en_filter_id = (mac_assigned_filter_id_enum)l_filter_id;
    st_assigned_filter_cmd.en_enable = (l_on > 0) ? OAL_TRUE : OAL_FALSE;

    oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
        "{wal_set_assigned_filter_enable::assigned_filter on/off(%d)}", st_assigned_filter_cmd.en_enable);

    /* 抛事件到wal层处理 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_ASSIGNED_FILTER, us_len);

    /* 将申请的netbuf首地址填写到msg消息体内 */
    l_ret = memcpy_s(st_write_msg.auc_value, us_len, (uint8_t *)&st_assigned_filter_cmd, us_len);
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "wal_set_assigned_filter_enable::memcpy fail!");
        return OAL_FAIL;
    }

    /* 发送消息 */
    l_ret = wal_send_cfg_event(pst_net_dev,
                                WAL_MSG_TYPE_WRITE,
                                WAL_MSG_WRITE_MSG_HDR_LENGTH + us_len,
                                (uint8_t *)&st_write_msg,
                                OAL_FALSE, OAL_PTR_NULL);
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
            "{wal_set_assigned_filter_enable::send cfg event fail[%d]!}", l_ret);
        return l_ret;
    }

    return OAL_SUCC;
}


int32_t wal_set_ip_filter_enable(int32_t l_on)
{
    uint16_t us_len;
    int32_t l_ret;
    uint32_t ul_netbuf_len;
    oal_netbuf_stru *pst_netbuf = OAL_PTR_NULL;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    oal_net_device_stru *pst_net_dev = OAL_PTR_NULL;
    wal_msg_write_stru st_write_msg;
    mac_ip_filter_cmd_stru st_ip_filter_cmd;
    mac_ip_filter_cmd_stru *pst_cmd_info = OAL_PTR_NULL;

#ifdef _PRE_WLAN_FEATURE_DFR
    if (g_st_dfr_info.bit_device_reset_process_flag) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_set_ip_filter_enable:: dfr_process_status[%d]!}",
                         g_st_dfr_info.bit_device_reset_process_flag);
        return -OAL_EFAIL;
    }
#endif  // #ifdef _PRE_WLAN_FEATURE_DFR

    if (l_on < 0) {
        oam_error_log1(0, OAM_SF_ANY, "{wal_set_ip_filter_enable::Invalid input parameter, on/off %d!}", l_on);
        return -OAL_EINVAL;
    }

    pst_net_dev = wal_config_get_netdev("wlan0", OAL_STRLEN("wlan0"));
    if (pst_net_dev == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_set_ip_filter_enable::wlan0 not exist!}");
        return -OAL_EINVAL;
    }
    /* 调用oal_dev_get_by_name后，必须调用oal_dev_put使net_dev的引用计数减一 */
    oal_dev_put(pst_net_dev);

    /* vap未创建时，不处理下发的命令 */
    pst_mac_vap = oal_net_dev_priv(pst_net_dev);
    if (pst_mac_vap == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_set_ip_filter_enable::vap not created yet, ignore the cmd!}");
        return -OAL_EINVAL;
    }

    if (pst_mac_vap->st_cap_flag.bit_ip_filter != OAL_TRUE) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_set_ip_filter_enable::Func not enable,ignore!}");
        return -OAL_EINVAL;
    }

    /* 准备配置命令 */
    ul_netbuf_len = OAL_SIZEOF(st_ip_filter_cmd);
    memset_s((uint8_t *)&st_ip_filter_cmd, ul_netbuf_len, 0, ul_netbuf_len);
    st_ip_filter_cmd.en_cmd = MAC_IP_FILTER_ENABLE;
    st_ip_filter_cmd.en_enable = (l_on > 0) ? OAL_TRUE : OAL_FALSE;

    oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_set_ip_filter_enable::IP_filter on/off(%d).}",
                     st_ip_filter_cmd.en_enable);

    /* 申请空间 缓存过滤规则 */
    pst_netbuf = oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF, ul_netbuf_len, OAL_NETBUF_PRIORITY_MID);
    if (pst_netbuf == OAL_PTR_NULL) {
        oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
            "{wal_set_ip_filter_enable::netbuf alloc null,size %d.}", ul_netbuf_len);
        return -OAL_EINVAL;
    }
    memset_s(((uint8_t *)oal_netbuf_data(pst_netbuf)), ul_netbuf_len, 0, ul_netbuf_len);
    pst_cmd_info = (mac_ip_filter_cmd_stru *)oal_netbuf_data(pst_netbuf);

    /* 记录过滤规则 */
    l_ret = memcpy_s((uint8_t *)pst_cmd_info, ul_netbuf_len, (uint8_t *)(&st_ip_filter_cmd), ul_netbuf_len);
    oal_netbuf_put(pst_netbuf, ul_netbuf_len);

    /* 抛事件到wal层处理 */
    us_len = OAL_SIZEOF(oal_netbuf_stru *);

    /* 填写 msg 消息头 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_IP_FILTER, us_len);

    /* 将申请的netbuf首地址填写到msg消息体内 */
    l_ret += memcpy_s(st_write_msg.auc_value, us_len, (uint8_t *)&pst_netbuf, us_len);
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "wal_set_ip_filter_enable::memcpy fail!");
        oal_netbuf_free(pst_netbuf);
        return -OAL_EINVAL;
    }

    /* 发送消息 */
    l_ret = wal_send_cfg_event(pst_net_dev, WAL_MSG_TYPE_WRITE,
        WAL_MSG_WRITE_MSG_HDR_LENGTH + us_len, (uint8_t *)&st_write_msg, OAL_FALSE, OAL_PTR_NULL);
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_error_log1(pst_mac_vap->uc_vap_id, 0, "{wal_set_ip_filter_enable::send_cfg_event fail:%d}", l_ret);
        oal_netbuf_free(pst_netbuf);
        return l_ret;
    }

    return OAL_SUCC;
}

int32_t wal_add_ip_filter_items(wal_hw_wifi_filter_item *pst_items, int32_t l_count)
{
    uint16_t us_len;
    int32_t l_ret;
    uint32_t ul_netbuf_len;
    uint32_t ul_items_idx;
    oal_netbuf_stru *pst_netbuf = OAL_PTR_NULL;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    oal_net_device_stru *pst_net_dev = OAL_PTR_NULL;
    wal_msg_write_stru st_write_msg;
    mac_ip_filter_cmd_stru st_ip_filter_cmd;
    mac_ip_filter_cmd_stru *pst_cmd_info = OAL_PTR_NULL;

#ifdef _PRE_WLAN_FEATURE_DFR
    if (g_st_dfr_info.bit_device_reset_process_flag) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_add_ip_filter_items:: dfr_process_status[%d]!}",
                         g_st_dfr_info.bit_device_reset_process_flag);
        return -OAL_EFAIL;
    }
#endif  // #ifdef _PRE_WLAN_FEATURE_DFR

    if ((pst_items == OAL_PTR_NULL) || (l_count <= 0)) {
        oam_error_log2(0, OAM_SF_ANY, "{wal_add_ip_filter_items::Invalid input para, pst_items %p, l_count %d!}",
            (uintptr_t)pst_items, l_count);
        return -OAL_EINVAL;
    }

    pst_net_dev = wal_config_get_netdev("wlan0", OAL_STRLEN("wlan0"));
    if (pst_net_dev == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_add_ip_filter_items::wlan0 not exist!}");
        return -OAL_EINVAL;
    }
    /* 调用oal_dev_get_by_name后，必须调用oal_dev_put使net_dev的引用计数减一 */
    oal_dev_put(pst_net_dev);

    /* vap未创建时，不处理下发的命令 */
    pst_mac_vap = oal_net_dev_priv(pst_net_dev);
    if (pst_mac_vap == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_add_ip_filter_items::vap not created yet, ignore the cmd!.}");
        return -OAL_EINVAL;
    }

    if (pst_mac_vap->st_cap_flag.bit_ip_filter != OAL_TRUE) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_add_ip_filter_items::Func disable, ignore cmd}");
        return -OAL_EINVAL;
    }

    /* 准备配置事件 */
    memset_s((uint8_t *)&st_ip_filter_cmd, OAL_SIZEOF(st_ip_filter_cmd), 0, OAL_SIZEOF(st_ip_filter_cmd));
    st_ip_filter_cmd.en_cmd = MAC_IP_FILTER_UPDATE_BTABLE;

    /* 由于本地名单大小限制，取能收纳的规则条目数最小值 */
    st_ip_filter_cmd.uc_item_count =
        oal_min((MAC_MAX_IP_FILTER_BTABLE_SIZE / OAL_SIZEOF(mac_ip_filter_item_stru)), l_count);
    if (st_ip_filter_cmd.uc_item_count < l_count) {
        oam_warning_log2(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
            "{wal_add_ip_filter_items::Btable(%d) is too small to store %d items!}",
            st_ip_filter_cmd.uc_item_count, l_count);
    }

    oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
        "{wal_add_ip_filter_items::Start updating btable, items_cnt(%d).}",
        st_ip_filter_cmd.uc_item_count);

    /* 选择申请事件空间的大小 */
    ul_netbuf_len = (st_ip_filter_cmd.uc_item_count * OAL_SIZEOF(mac_ip_filter_item_stru)) +
        OAL_SIZEOF(st_ip_filter_cmd);

    /* 申请空间 缓存过滤规则 */
    pst_netbuf = oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF, ul_netbuf_len, OAL_NETBUF_PRIORITY_MID);
    if (pst_netbuf == OAL_PTR_NULL) {
        oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
            "{wal_add_ip_filter_items::netbuf alloc null,size %d.}", ul_netbuf_len);
        return -OAL_EINVAL;
    }
    memset_s(((uint8_t *)oal_netbuf_data(pst_netbuf)), ul_netbuf_len, 0, ul_netbuf_len);
    pst_cmd_info = (mac_ip_filter_cmd_stru *)oal_netbuf_data(pst_netbuf);

    /* 记录过滤规则 */
    l_ret = memcpy_s((uint8_t *)pst_cmd_info, ul_netbuf_len,
                     (uint8_t *)(&st_ip_filter_cmd), OAL_SIZEOF(st_ip_filter_cmd));
    oal_netbuf_put(pst_netbuf, ul_netbuf_len);

    for (ul_items_idx = 0; ul_items_idx < st_ip_filter_cmd.uc_item_count; ul_items_idx++) {
        pst_cmd_info->ast_filter_items[ul_items_idx].uc_protocol = (uint8_t)pst_items[ul_items_idx].protocol;
        pst_cmd_info->ast_filter_items[ul_items_idx].us_port = (uint16_t)pst_items[ul_items_idx].port;
    }

    /* 抛事件到wal层处理 */
    us_len = OAL_SIZEOF(oal_netbuf_stru *);

    /* 填写 msg 消息头 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_IP_FILTER, us_len);

    /* 将申请的netbuf首地址填写到msg消息体内 */
    l_ret += memcpy_s(st_write_msg.auc_value, us_len, (uint8_t *)&pst_netbuf, us_len);
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "hwifi_config_init_fcc_ce_txpwr_nvram::memcpy fail!");
        oal_netbuf_free(pst_netbuf);
        return -OAL_EINVAL;
    }

    /* 发送消息 */
    l_ret = wal_send_cfg_event(pst_net_dev, WAL_MSG_TYPE_WRITE,
        WAL_MSG_WRITE_MSG_HDR_LENGTH + us_len, (uint8_t *)&st_write_msg, OAL_FALSE, OAL_PTR_NULL);
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
            "{wal_add_ip_filter_items::wal_send_cfg_event fail[%d]!}", l_ret);
        oal_netbuf_free(pst_netbuf);
        return l_ret;
    }

    return OAL_SUCC;
}

int32_t wal_clear_ip_filter()
{
    uint16_t us_len;
    int32_t l_ret;
    uint32_t ul_netbuf_len;
    oal_netbuf_stru *pst_netbuf = OAL_PTR_NULL;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    oal_net_device_stru *pst_net_dev;
    wal_msg_write_stru st_write_msg;
    mac_ip_filter_cmd_stru st_ip_filter_cmd;
    mac_ip_filter_cmd_stru *pst_cmd_info = OAL_PTR_NULL;

#ifdef _PRE_WLAN_FEATURE_DFR
    if (g_st_dfr_info.bit_device_reset_process_flag) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_clear_ip_filter:: dfr_process_status[%d]!}",
                         g_st_dfr_info.bit_device_reset_process_flag);
        return -OAL_EFAIL;
    }
#endif  // #ifdef _PRE_WLAN_FEATURE_DFR

    pst_net_dev = wal_config_get_netdev("wlan0", OAL_STRLEN("wlan0"));
    if (pst_net_dev == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_clear_ip_filter::wlan0 not exist!}");
        return -OAL_EINVAL;
    }

    /* 调用oal_dev_get_by_name后，必须调用oal_dev_put使net_dev的引用计数减一 */
    oal_dev_put(pst_net_dev);

    /* vap未创建时，不处理下发的命令 */
    pst_mac_vap = oal_net_dev_priv(pst_net_dev);
    if (pst_mac_vap == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_clear_ip_filter::vap not created yet, ignore the cmd!.}");
        return -OAL_EINVAL;
    }

    if (pst_mac_vap->st_cap_flag.bit_ip_filter != OAL_TRUE) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_clear_ip_filter::Func disable,ignore the cmd}");
        return -OAL_EINVAL;
    }

    /* 清理黑名单 */
    memset_s((uint8_t *)&st_ip_filter_cmd, OAL_SIZEOF(st_ip_filter_cmd), 0, OAL_SIZEOF(st_ip_filter_cmd));
    st_ip_filter_cmd.en_cmd = MAC_IP_FILTER_CLEAR;

    oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_clear_ip_filter::Now start clearing the list.}");

    /* 选择申请事件空间的大小 */
    ul_netbuf_len = OAL_SIZEOF(st_ip_filter_cmd);

    /* 申请空间 缓存过滤规则 */
    pst_netbuf = oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF, ul_netbuf_len, OAL_NETBUF_PRIORITY_MID);
    if (pst_netbuf == OAL_PTR_NULL) {
        oam_error_log1(pst_mac_vap->uc_vap_id, 0, "{wal_clear_ip_filter::netbuf[%d] alloc fail}", ul_netbuf_len);
        return -OAL_EINVAL;
    }
    memset_s(((uint8_t *)oal_netbuf_data(pst_netbuf)), ul_netbuf_len, 0, ul_netbuf_len);
    pst_cmd_info = (mac_ip_filter_cmd_stru *)oal_netbuf_data(pst_netbuf);

    /* 记录过滤规则 */
    l_ret = memcpy_s((uint8_t *)pst_cmd_info, ul_netbuf_len, (uint8_t *)(&st_ip_filter_cmd), ul_netbuf_len);
    oal_netbuf_put(pst_netbuf, ul_netbuf_len);

    /* 抛事件到wal层处理 */
    us_len = OAL_SIZEOF(oal_netbuf_stru *);

    /* 填写 msg 消息头 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_IP_FILTER, us_len);

    /* 将申请的netbuf首地址填写到msg消息体内 */
    l_ret += memcpy_s(st_write_msg.auc_value, us_len, (uint8_t *)&pst_netbuf, us_len);
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "wal_clear_ip_filter::memcpy fail!");
        oal_netbuf_free(pst_netbuf);
        return -OAL_EINVAL;
    }

    /* 发送消息 */
    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + us_len,
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_clear_ip_filter::send cfg event fail:%d}", l_ret);
        oal_netbuf_free(pst_netbuf);
        return l_ret;
    }

    return OAL_SUCC;
}
int32_t wal_register_ip_filter(wal_hw_wlan_filter_ops *pst_ip_filter_ops)
{
#ifdef CONFIG_DOZE_FILTER
    if (pst_ip_filter_ops == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_register_ip_filter::pg_st_ip_filter_ops is null !}");
        return -OAL_EINVAL;
    }
    hw_register_wlan_filter(pst_ip_filter_ops);
#else
    oam_warning_log0(0, OAM_SF_ANY, "{wal_register_ip_filter:: Not support CONFIG_DOZE_FILTER!}");
#endif
    return OAL_SUCC;
}

int32_t wal_unregister_ip_filter()
{
#ifdef CONFIG_DOZE_FILTER
    hw_unregister_wlan_filter();
#else
    oam_warning_log0(0, OAM_SF_ANY, "{wal_unregister_ip_filter:: Not support CONFIG_DOZE_FILTER!}");
#endif
    return OAL_SUCC;
}


OAL_STATIC int32_t wal_ioctl_set_probe_resp_mode(oal_bool_enum_uint8 en_light_suspend)
{
    uint16_t us_len;
    int32_t l_ret;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    oal_net_device_stru *pst_net_dev;
    wal_msg_write_stru st_write_msg;
    mac_probe_resp_mode_enum_uint8 en_probe_resp_status;

    en_probe_resp_status = (en_light_suspend == OAL_TRUE) ? MAC_PROBE_RESP_MODE_PASSIVE : MAC_PROBE_RESP_MODE_ACTIVE;

    /* 查找p2p dev */
    pst_net_dev = wal_config_get_netdev("p2p0", OAL_STRLEN("p2p0"));
    if (pst_net_dev == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_ioctl_set_probe_resp_mode::p2p0 not exist!}");
        return -OAL_EINVAL;
    }
    /* 调用oal_dev_get_by_name后，必须调用oal_dev_put使net_dev的引用计数减一 */
    oal_dev_put(pst_net_dev);

    /* vap未创建时，不处理下发的命令 */
    pst_mac_vap = oal_net_dev_priv(pst_net_dev);
    if (pst_mac_vap == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_ioctl_set_probe_resp_mode::vap not created yet, ignore the cmd!}");
        return -OAL_EINVAL;
    }

    oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_ioctl_set_probe_resp_mode::set probe_resp_status[%d]!}",
        en_probe_resp_status);

    /* 抛事件到wal层处理 */
    us_len = OAL_SIZEOF(en_probe_resp_status);

    /* 填写 msg 消息头 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_PROBE_PESP_MODE, us_len);

    /* 将申请的netbuf首地址填写到msg消息体内 */
    if (EOK != memcpy_s(st_write_msg.auc_value, us_len, (uint8_t *)&en_probe_resp_status, us_len)) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_ioctl_set_probe_resp_mode::memcpy_s failed}");
        return -OAL_EINVAL;
    }

    /* 发送消息 */
    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + us_len,
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
            "{wal_ioctl_set_probe_resp_mode::send cfg event fail[%d]!}", l_ret);
        return l_ret;
    }

    return OAL_SUCC;
}

int32_t wlan_enter_light_suspend()
{
    /* 进入熄屏，待处理事件: 1.设置P2P dev Listen状态回复probe pesp模式为PASSIVE */
    if (OAL_SUCC != wal_ioctl_set_probe_resp_mode(OAL_TRUE)) {
        oam_error_log0(0, OAM_SF_ANY, "{wlan_enter_light_suspend::light_suspend set_probe_resp_mode fail}");
    }

    oam_warning_log0(0, OAM_SF_ANY, "{wlan_enter_lights_suspend::light_suspend enter}");
    return OAL_SUCC;
}

int32_t wlan_exit_light_suspend()
{
    /* 结束熄屏，待处理事件: 1.设置P2P dev Listen状态回复probe pesp模式为ACTIVE */
    if (OAL_SUCC != wal_ioctl_set_probe_resp_mode(OAL_FALSE)) {
        oam_error_log0(0, OAM_SF_ANY, "{wlan_exit_light_suspend::light_suspend set_probe_resp_mode fail}");
    }

    oam_error_log0(0, OAM_SF_ANY, "{wlan_exit_light_suspend::light_suspend exit}");
    return OAL_SUCC;
}

#ifdef _PRE_PRODUCT_HI3751_PLATO
#define PMOC_LIGHT_SUSPEND_CB_FUNC_S pmoc_light_suspend_cb_func
#define u32ModuleID module_id
#define sModuleName s_module_name
#define EnterLightSuspendFunc enter_light_suspend_func
#define ExitLightSuspendFunc exit_light_suspend_func
#define PMOC_DRV_LightSuspendCallbackRegister pmoc_drv_light_suspend_callback_register
#define PMOC_DRV_LightSuspendCallbackUnregister pmoc_drv_light_suspend_callback_unregister
#endif

void wal_register_lights_suspend()
{
#ifdef _PRE_PRODUCT_HI3751V811
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 9, 0)) && (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    PMOC_LIGHT_SUSPEND_CB_FUNC_S stLightSuspendCb;

    stLightSuspendCb.u32ModuleID = (uint32_t)HI_ID_USER;
    stLightSuspendCb.sModuleName = "HI_WIFI_1103";
    stLightSuspendCb.EnterLightSuspendFunc = wlan_enter_light_suspend;
    stLightSuspendCb.ExitLightSuspendFunc  = wlan_exit_light_suspend;

    /* 大屏熄屏亮屏的钩子注册操作 */
    if (PMOC_DRV_LightSuspendCallbackRegister(&stLightSuspendCb) != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_register_lights_suspend::PMOC_DRV_LightSuspendCallbackRegister failed!}");
        PMOC_DRV_LightSuspendCallbackUnregister((HI_U32)HI_ID_USER);
    }
    oam_warning_log0(0, OAM_SF_ANY, "{wal_register_lights_suspend::PMOC_DRV_LightSuspendCallbackRegister  success!}");
#endif // #if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 9, 0))
#else
    oam_warning_log0(0, OAM_SF_ANY, "{wal_register_lights_suspend::no register LightSuspendCallback hook}");
#endif
    return;
}
void wal_unregister_lights_suspend()
{
#ifdef _PRE_PRODUCT_HI3751V811
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 9, 0)) && (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    PMOC_DRV_LightSuspendCallbackUnregister((uint32_t)HI_ID_USER);
#endif // #if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 9, 0))
    oam_warning_log0(0, OAM_SF_ANY, "{wal_register_lights_suspend::PMOC_DRV_LightSuspendCallbackUnregister done!}");
#else
    oam_warning_log0(0, OAM_SF_ANY, "{wal_unregister_lights_suspend::no reg LightSuspendCallbackUnregister hook}");
#endif
    return;
}

/*lint -e19*/
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION) && (LINUX_VERSION_CODE < KERNEL_VERSION(3, 10, 44))
oal_module_symbol(wal_hipriv_proc_write);
#endif
oal_module_symbol(wal_hipriv_get_mac_addr); /*lint +e19*/

#ifdef _PRE_WLAN_FEATURE_TCP_ACK_BUFFER
OAL_STATIC uint32_t wal_hipriv_tcp_ack_buf_cfg(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t ul_ret;
    int32_t l_ret;
    int32_t l_tmp;
    uint8_t uc_idx;
    mac_cfg_tcp_ack_buf_stru *pst_tcp_ack_param;

    pst_tcp_ack_param = (mac_cfg_tcp_ack_buf_stru *)(st_write_msg.auc_value);
    memset_s((void *)pst_tcp_ack_param, OAL_SIZEOF(mac_cfg_tcp_ack_buf_stru),
             0, OAL_SIZEOF(mac_cfg_tcp_ack_buf_stru));

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_CFG, "{wal_hipriv_tcp_ack_buf_cfg::get cmd one arg err[%d]!}", ul_ret);
        return ul_ret;
    }
    pc_param = pc_param + ul_off_set;

    for (uc_idx = 0; uc_idx < MAC_TCP_ACK_BUF_TYPE_BUTT; uc_idx++) {
        if (0 == oal_strcmp(ac_name, g_ast_hmac_tcp_ack_buf_cfg_table[uc_idx].puc_string)) {
            break;
        }
    }
    if (uc_idx == MAC_TCP_ACK_BUF_TYPE_BUTT) {
        oam_warning_log0(0, OAM_SF_CFG, "{wal_hipriv_tcp_ack_buf_cfg:: parameter error !");
        return uc_idx;
    }
    pst_tcp_ack_param->en_cmd = g_ast_hmac_tcp_ack_buf_cfg_table[uc_idx].en_tcp_ack_buf_cfg_id;

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_CFG, "{wal_hipriv_tcp_ack_buf_cfg::get cmd one arg err[%d]!}", ul_ret);
        return ul_ret;
    }

    l_tmp = (int32_t)oal_atoi(ac_name);
    if ((l_tmp < 0) || (l_tmp > 10 * 1000)) {
        oam_warning_log1(0, OAM_SF_CFG, "{wal_hipriv_tcp_ack_buf_cfg::  car param[%d] invalid! }", oal_atoi(ac_name));
        return OAL_FAIL;
    }

    if (pst_tcp_ack_param->en_cmd == MAC_TCP_ACK_BUF_ENABLE) {
        if (((uint8_t)l_tmp == OAL_FALSE) || ((uint8_t)l_tmp == OAL_TRUE)) {
            pst_tcp_ack_param->en_enable = (uint8_t)l_tmp;
        } else {
            oam_warning_log1(0, OAM_SF_CFG, "{wal_hipriv_tcp_ack_buf_cfg::fast_aging_flag[%d]:0/1}", (uint8_t)l_tmp);
            return OAL_FAIL;
        }
    }
    if (pst_tcp_ack_param->en_cmd == MAC_TCP_ACK_BUF_TIMEOUT) {
        if ((uint8_t)l_tmp == 0) {
            oam_warning_log0(0, OAM_SF_CFG, "{wal_hipriv_tcp_ack_buf_cfg:: timer_ms shoule not be 0.}");
            return OAL_FAIL;
        }

        pst_tcp_ack_param->uc_timeout_ms = (uint8_t)l_tmp;
    }
    if (pst_tcp_ack_param->en_cmd == MAC_TCP_ACK_BUF_MAX) {
        if ((uint8_t)l_tmp == 0) {
            oam_warning_log0(0, OAM_SF_CFG, "{wal_hipriv_tcp_ack_buf_cfg:: uc_count_limit shoule not be 0.}");
            return OAL_FAIL;
        }

        pst_tcp_ack_param->uc_count_limit = (uint8_t)l_tmp;
    }

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_TCP_ACK_BUF, OAL_SIZEOF(mac_cfg_tcp_ack_buf_stru));

    oam_warning_log4(0, OAM_SF_CFG, "{wal_hipriv_tcp_ack_buf_cfg::cmd[%d],enable[%d],timeout_ms[%d] count_limit[%d]!}",
        pst_tcp_ack_param->en_cmd, pst_tcp_ack_param->en_enable,
        pst_tcp_ack_param->uc_timeout_ms, pst_tcp_ack_param->uc_count_limit);

    l_ret = wal_send_cfg_event(pst_net_dev, WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_tcp_ack_buf_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE, OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_CFG, "{wal_hipriv_tcp_ack_buf_cfg::return err code [%d]!}", ul_ret);
        return (uint32_t)ul_ret;
    }

    return OAL_SUCC;
}
#endif

#ifdef _PRE_WLAN_FEATURE_M2S_MSS

OAL_STATIC uint32_t wal_ioctl_set_m2s_blacklist(oal_net_device_stru *pst_net_dev,
    uint8_t *puc_buf, uint32_t ul_buf_len, uint8_t uc_m2s_blacklist_cnt)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;
    mac_m2s_ie_stru *pst_m2s_ie;

    /* st_write_msg作清零操作 */
    memset_s(&st_write_msg, OAL_SIZEOF(wal_msg_write_stru), 0, OAL_SIZEOF(wal_msg_write_stru));

    if (uc_m2s_blacklist_cnt > WLAN_M2S_BLACKLIST_MAX_NUM) {
        oam_warning_log1(0, OAM_SF_M2S, "{wal_ioctl_set_m2s_blacklist::cnt[%d] beyond scope.}", uc_m2s_blacklist_cnt);
        return OAL_FAIL;
    }

    if (ul_buf_len < uc_m2s_blacklist_cnt * OAL_SIZEOF(wlan_m2s_mgr_vap_stru)) {
        oam_warning_log1(0, OAM_SF_M2S, "{wal_ioctl_set_m2s_blacklist::src buff len[%d] not enough.}", ul_buf_len);
        return OAL_FAIL;
    }

    /* 抛事件到wal层处理 */
    pst_m2s_ie = (mac_m2s_ie_stru *)st_write_msg.auc_value;

    pst_m2s_ie->uc_blacklist_cnt = uc_m2s_blacklist_cnt;

    l_ret = memcpy_s(pst_m2s_ie->ast_m2s_blacklist, WLAN_M2S_BLACKLIST_MAX_NUM * OAL_SIZEOF(wlan_m2s_mgr_vap_stru),
                     puc_buf, uc_m2s_blacklist_cnt * OAL_SIZEOF(wlan_m2s_mgr_vap_stru));
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_M2S, "wal_ioctl_set_m2s_blacklist::memcpy fail!");
        return OAL_FAIL;
    }

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_M2S_BLACKLIST, OAL_SIZEOF(mac_m2s_ie_stru));

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_m2s_ie_stru),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_M2S, "{wal_ioctl_set_m2s_blacklist::wal_send_cfg_event err[%d]}", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}


uint32_t wal_ioctl_set_m2s_mss(oal_net_device_stru *pst_net_dev, uint8_t uc_m2s_mode)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;

    /* st_write_msg作清零操作 */
    memset_s(&st_write_msg, OAL_SIZEOF(wal_msg_write_stru), 0, OAL_SIZEOF(wal_msg_write_stru));

    /* 抛事件到wal层处理 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_M2S_MSS, OAL_SIZEOF(int32_t));
    *((int32_t *)(st_write_msg.auc_value)) = uc_m2s_mode; /* 设置配置命令参数 */

    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(int32_t),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_M2S, "{wal_ioctl_set_m2s_mss::wal_send_cfg_event err:[%d]}", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}

#endif

#ifdef _PRE_WLAN_FEATURE_APF

OAL_STATIC uint32_t wal_hipriv_apf_filter_list(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;
    mac_apf_filter_cmd_stru st_apf_filter_cmd;

    memset_s(&st_apf_filter_cmd, OAL_SIZEOF(mac_apf_filter_cmd_stru), 0, OAL_SIZEOF(mac_apf_filter_cmd_stru));
    st_apf_filter_cmd.en_cmd_type = APF_GET_FILTER_CMD;

    /* 抛事件到wal层处理 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_APF_FILTER, OAL_SIZEOF(st_apf_filter_cmd));
    l_ret = memcpy_s(st_write_msg.auc_value, OAL_SIZEOF(st_apf_filter_cmd),
                     &st_apf_filter_cmd, OAL_SIZEOF(st_apf_filter_cmd));
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "wal_hipriv_apf_filter_list::memcpy fail!");
        return OAL_FAIL;
    }

    /* 发送消息 */
    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(st_apf_filter_cmd),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_80m_rts_debug::return err code [%d]!}", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}
#endif


OAL_STATIC uint32_t wal_hipriv_remove_app_ie(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;
    uint32_t ul_ret;
    uint32_t ul_off_set;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    const uint8_t uc_remove_ie_len = 2; /* 0/1 eid */

    /*  抛事件到wal层处理 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_REMOVE_APP_IE, uc_remove_ie_len);

    /* 获取移除的类型，类型暂只支持0/1 移除或者恢复 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_remove_app_ie::wal_get_cmd_one_arg err:%d!}", ul_ret);
        return ul_ret;
    }
    st_write_msg.auc_value[0] = (uint8_t)oal_atoi(ac_name);
    /* 获取操作的EID */
    pc_param += ul_off_set;
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_remove_app_ie::wal_get_cmd_one_arg eid err:%d!}", ul_ret);
        return ul_ret;
    }
    st_write_msg.auc_value[1] = (uint8_t)oal_atoi(ac_name);

    /* 发送消息 */
    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + uc_remove_ie_len,
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_CFG, "{wal_hipriv_remove_app_ie::return err code [%d]!}", l_ret);
        return (uint32_t)l_ret;
    }

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_DELAY_STATISTIC

OAL_STATIC uint32_t wal_hipriv_pkt_time_switch(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    uint32_t ul_off_set;
    uint32_t ul_ret;
    int8_t ac_arg[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    wal_msg_write_stru st_write_msg;
    user_delay_switch_stru st_switch_cmd;
    int32_t l_ret;

    /* 获取命令字符串 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_pkt_time_switch::wal_get_cmd_one_arg err[%d]!}", ul_ret);
        return ul_ret;
    }

    memset_s(&st_switch_cmd, OAL_SIZEOF(user_delay_switch_stru), 0, OAL_SIZEOF(user_delay_switch_stru));
    if (0 == (oal_strcmp("on", ac_arg))) {
        st_switch_cmd.dmac_stat_enable = 1;

        /* 第二个参数，统计数据帧的数量 */
        pc_param += ul_off_set;
        ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_pkt_time_switch::wal_get_cmd_one_arg 2nd err:%d}", ul_ret);
            return ul_ret;
        }
        st_switch_cmd.dmac_packet_count_num = (uint32_t)oal_atoi(ac_arg);

        /* 第三个参数，上报间隔 */
        pc_param += ul_off_set;
        ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_pkt_time_switch::wal_get_cmd_one_arg 2nd err:%d}", ul_ret);
            return ul_ret;
        }
        st_switch_cmd.dmac_report_interval = (uint32_t)oal_atoi(ac_arg);

    } else if (0 == (oal_strcmp("off", ac_arg))) {
        st_switch_cmd.dmac_stat_enable = 0;
        st_switch_cmd.dmac_packet_count_num = 0;
        st_switch_cmd.dmac_report_interval = 0;
    } else {
        oam_error_log0(0, OAM_SF_CFG, "{wal_hipriv_pkt_time_switch::invalid parameter!}");
        return OAL_FAIL;
    }
    l_ret = memcpy_s(st_write_msg.auc_value, OAL_SIZEOF(st_switch_cmd), &st_switch_cmd, OAL_SIZEOF(st_switch_cmd));
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_CFG, "wal_hipriv_pkt_time_switch::memcpy fail!");
        return OAL_FAIL;
    }

    /* 抛事件到wal层处理 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_PKT_TIME_SWITCH, OAL_SIZEOF(st_switch_cmd));
    ul_ret = (uint32_t)wal_send_cfg_event(pst_net_dev, WAL_MSG_TYPE_WRITE,
                                                WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(st_switch_cmd),
                                                (uint8_t *)&st_write_msg,
                                                OAL_FALSE, OAL_PTR_NULL);

    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_CFG, "{wal_hipriv_pkt_time_switch::return err code %d!}", ul_ret);
    }
    return ul_ret;
}
#endif


uint32_t wal_ioctl_set_ap_mode(oal_net_device_stru *pst_net_dev, uint8_t uc_param)
{
    wal_msg_write_stru st_write_msg;
    int32_t l_ret;

    /* 抛事件到wal层处理 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SOFTAP_MIMO_MODE, OAL_SIZEOF(uint8_t));
    st_write_msg.auc_value[0] = uc_param;

    /* 发送消息 */
    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(uint8_t),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_CFG, "{wal_ioctl_set_ap_mode::return err code [%d]!}", l_ret);
        return (uint32_t)l_ret;
    }

    oam_warning_log1(0, OAM_SF_ANY, "{wal_ioctl_set_ap_mode::uc_param[%d].}", uc_param);
    return OAL_SUCC;
}

OAL_STATIC int32_t wal_psm_query_wait_complete(hmac_psm_flt_stat_query_stru *pst_hmac_psm_query,
                                                 mac_psm_query_type_enum_uint8 en_query_type)
{
    pst_hmac_psm_query->auc_complete_flag[en_query_type] = OAL_FALSE;
    /*lint -e730 -e740 -e774*/
    return oal_wait_event_interruptible_timeout_m(pst_hmac_psm_query->st_wait_queue,
             (pst_hmac_psm_query->auc_complete_flag[en_query_type] == OAL_TRUE),
             OAL_TIME_HZ);
    /*lint +e730 +e740 +e774*/
}

uint32_t wal_ioctl_get_psm_stat(oal_net_device_stru *pst_net_dev,
    mac_psm_query_type_enum_uint8 en_query_type, oal_ifreq_stru *pst_ifr)
{
    mac_vap_stru *pst_mac_vap = NULL;
    hmac_device_stru *pst_hmac_device = NULL;
    wal_msg_write_stru st_write_msg;
    hmac_psm_flt_stat_query_stru *pst_hmac_psm_query = NULL;
    mac_psm_query_stat_stru  *pst_psm_stat = NULL;
    int32_t l_ret;

    pst_mac_vap = oal_net_dev_priv(pst_net_dev);
    if (pst_mac_vap == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_CFG, "{wal_ioctl_get_psm_stat::pst_mac_vap get from netdev is null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    pst_hmac_device = hmac_res_get_mac_dev(pst_mac_vap->uc_device_id);
    if (pst_hmac_device == OAL_PTR_NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }
    if (en_query_type >= MAC_PSM_QUERY_TYPE_BUTT) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{wal_ioctl_get_psm_stat::query type is not supported}");
        return OAL_FAIL;
    }

    /* 抛事件到wal层处理 */
    memset_s(&st_write_msg, OAL_SIZEOF(st_write_msg), 0, OAL_SIZEOF(st_write_msg));
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_QUERY_PSM_STAT, OAL_SIZEOF(mac_psm_query_type_enum_uint8));
    /* 下发查询命令 */
    *(mac_psm_query_type_enum_uint8*)(st_write_msg.auc_value) = en_query_type;
    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH+OAL_SIZEOF(mac_psm_query_type_enum_uint8),
                                   (uint8_t *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);
    if (l_ret != OAL_SUCC) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{wal_ioctl_get_psm_stat::send event fail[%d]}", l_ret);
        return OAL_FAIL;
    }

    /* 等待查询返回 */
    pst_hmac_psm_query = &pst_hmac_device->st_psm_flt_stat_query;
    l_ret = wal_psm_query_wait_complete(pst_hmac_psm_query, en_query_type);
    /* 超时或异常 */
    if (l_ret <= 0) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{wal_ioctl_get_psm_stat::fail! ret:%d}", l_ret);
        return OAL_FAIL;
    }

    pst_psm_stat = &pst_hmac_psm_query->ast_psm_stat[en_query_type];
    if (pst_psm_stat->ul_query_item > MAC_PSM_QUERY_MSG_MAX_STAT_ITEM) {
        oam_error_log1(0, OAM_SF_CFG, "{wal_ioctl_get_psm_stat::query_item invalid[%d]}", pst_psm_stat->ul_query_item);
    }

    /* 调式命令时该参数为null */
    if (pst_ifr == OAL_PTR_NULL) {
        return OAL_SUCC;
    }
    if (oal_copy_to_user(pst_ifr->ifr_data + 8, pst_psm_stat->aul_val, /* 8为hdr */
        pst_psm_stat->ul_query_item * OAL_SIZEOF(uint32_t))) {
        oam_error_log0(0, OAM_SF_CFG, "{wal_ioctl_get_psm_stat::Failed to copy ioctl_data to user !}");
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_HID2D

OAL_STATIC uint32_t wal_hipriv_set_hid2d_acs_mode(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru              st_write_msg;
    uint32_t                        ul_off_set;
    int8_t                          ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t                        ul_ret;
    int32_t                         l_ret;
    uint32_t                        ul_value;

    /* 获取参数 */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        return ul_ret;
    }
    pc_param += ul_off_set;
    ul_value = (oal_bool_enum_uint8)oal_atoi(ac_name);

    /* 抛事件到wal层处理 */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_HID2D_ACS_MODE, OAL_SIZEOF(oal_uint32));

    /* 设置配置命令参数 */
    *((uint32_t *)(st_write_msg.auc_value)) = ul_value;

    l_ret = wal_send_cfg_event(pst_net_dev,
        WAL_MSG_TYPE_WRITE,
        WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(uint32_t),
        (uint8_t *)&st_write_msg,
        OAL_FALSE,
        OAL_PTR_NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_error_log1(0, OAM_SF_ANY, "{HiD2D::return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }
    return OAL_SUCC;
}

#endif
#ifdef _PRE_WLAN_FEATURE_CSI
uint32_t wal_ioctl_set_csi_switch_config(oal_net_device_stru *pst_cfg_net_dev,
    mac_cfg_csi_param_stru *cfg_csi_param_in)
{
    int32_t l_ret;
    wal_msg_write_stru st_write_msg;
    mac_cfg_csi_param_stru *cfg_csi_param = NULL;

    /***************************************************************************
                             抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_CSI, sizeof(mac_cfg_csi_param_stru));

    /* 设置配置命令参数 */
    cfg_csi_param = (mac_cfg_csi_param_stru *)(st_write_msg.auc_value);
    memcpy_s(cfg_csi_param, sizeof(mac_cfg_csi_param_stru), cfg_csi_param_in, sizeof(mac_cfg_csi_param_stru));
    l_ret = wal_send_cfg_event(pst_cfg_net_dev, WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + sizeof(mac_cfg_csi_param_stru),
                               (uint8_t *)&st_write_msg,
                               OAL_FALSE,
                               NULL);

    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_csi::return err code [%d]!}\r\n", l_ret);
        return (uint32_t)l_ret;
    }
    return OAL_SUCC;
}

uint32_t wal_ioctl_set_csi_switch(oal_net_device_stru *pst_cfg_net_dev, int8_t *pc_param)
{
    uint32_t ul_off_set, ul_ret;
    int8_t ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    mac_cfg_csi_param_stru st_cfg_csi_param = {{0}};

    if (oal_unlikely(oal_any_null_ptr2(pst_cfg_net_dev, pc_param))) {
        oam_error_log0(0, OAM_SF_ANY, "{wal_hipriv_set_csi::pst_cfg_net_dev or pc_param null ptr!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* PSD设置开关的命令: hipriv "vap0 set_csi ta ta_check csi_en"
       TA:被测量CSI的mac地址，为0表示不使能
       TA_check: 为1时，TA有效，表示每次采集CSI信息时需比对ta。
       csi_en:   为1时，表示使能CSI采集
    */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        return ul_ret;
    }
    oal_strtoaddr(ac_name, sizeof(ac_name), st_cfg_csi_param.auc_mac_addr, WLAN_MAC_ADDR_LEN);

    /* 偏移，取下一个参数 */
    pc_param = pc_param + ul_off_set;
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        return ul_ret;
    }
    st_cfg_csi_param.en_cfg_csi_on = (uint8_t)oal_atoi(ac_name);

    /* 偏移，取下一个参数 */
    pc_param = pc_param + ul_off_set;
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        return ul_ret;
    }
    st_cfg_csi_param.uc_csi_bw = (uint8_t)oal_atoi(ac_name);

    /* 偏移，取下一个参数 */
    pc_param = pc_param + ul_off_set;
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        return ul_ret;
    }
    st_cfg_csi_param.uc_csi_frame_type = (uint8_t)oal_atoi(ac_name);

    /* 偏移，取下一个参数 */
    pc_param = pc_param + ul_off_set;
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        return ul_ret;
    }
    st_cfg_csi_param.uc_csi_sample_period  = (uint8_t)oal_atoi(ac_name);

    /* 偏移，取下一个参数 */
    pc_param = pc_param + ul_off_set;
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        return ul_ret;
    }
    st_cfg_csi_param.uc_csi_phy_report_mode = (uint8_t)oal_atoi(ac_name);
    ul_ret = wal_ioctl_set_csi_switch_config(pst_cfg_net_dev, &st_cfg_csi_param);
    if (ul_ret != OAL_SUCC) {
        return ul_ret;
    }

    return OAL_SUCC;
}

#endif


OAL_STATIC uint32_t wal_hipriv_cali_debug(oal_net_device_stru *pst_net_dev, int8_t *pc_param)
{
    wal_msg_write_stru       st_write_msg;
    uint32_t                 ul_offset;
    int8_t                   ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t                 ul_ret;
    mac_cfg_cali_param_stru *cali_cfg_para = OAL_PTR_NULL;

    uint8_t                  arg_idx = 0;
    uint8_t                  src_str_len;
    uint16_t                 para_len;
    uint16_t                 curr_pos = 0;

    /* 设置配置命令参数 */
    cali_cfg_para = (mac_cfg_cali_param_stru *)(st_write_msg.auc_value);

    para_len = (oal_uint16)OAL_STRLEN(pc_param) + 1;
    while (curr_pos < para_len) {
        /* 获取参数 */
        ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &ul_offset);
        if (ul_ret == OAL_ERR_CODE_CONFIG_ARGS_OVER) {
            break;
        }

        if (ul_ret != OAL_SUCC) {
            oam_error_log0(0, OAM_SF_CFG, "wal_hipriv_cali_debug::wal_get_cmd_one_arg get fail");
            return ul_ret;
        }

        src_str_len = OAL_STRLEN(ac_name) + 1;
        if (src_str_len > WLAN_CALI_CFG_CMD_MAX_LEN) {
            oam_warning_log1(0, OAM_SF_CFG, "wal_hipriv_cali_debug::args len [%d] overflow", src_str_len);
            return OAL_ERR_CODE_ARRAY_OVERFLOW;
        }

        if (memcpy_s((oal_int8*)(cali_cfg_para->input_args[arg_idx]), WLAN_CALI_CFG_CMD_MAX_LEN, ac_name, src_str_len)
            != EOK) {
            return OAL_FAIL;
        }
        pc_param += ul_offset;
        curr_pos += ul_offset;
        arg_idx++;
    }
    cali_cfg_para->input_args_num = arg_idx;

    /***************************************************************************
                             抛事件到wal层处理
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_CALI_CFG, OAL_SIZEOF(mac_cfg_cali_param_stru));
    ul_ret = (oal_uint32)wal_send_cfg_event(pst_net_dev,
                                            WAL_MSG_TYPE_WRITE,
                                            WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_cali_param_stru),
                                            (oal_uint8 *)&st_write_msg,
                                            OAL_FALSE,
                                            OAL_PTR_NULL);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_CFG, "{wal_hipriv_cali_debug::return err code %d!}\r\n", ul_ret);
    }

    return ul_ret;
}
uint32_t wal_ioctl_set_mode(oal_net_device_stru *net_dev, int8_t *param)
{
    int8_t mode_str[WAL_HIPRIV_CMD_NAME_MAX_LEN] = { 0 }; /* 预留协议模式字符串空间 */
    uint8_t prot_idx;
    uint32_t off_set = 0;
    uint32_t result;
    int32_t ret;

    if (oal_unlikely(oal_any_null_ptr2(net_dev, param))) {
        oam_error_log2(0, OAM_SF_ANY, "{wal_hipriv_set_mode::net_dev/p_param null ptr error %x!}",
            (uintptr_t)net_dev, (uintptr_t)param);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    /* param指向传入模式参数, 将其取出存放到mode_str中 */
    result = wal_get_cmd_one_arg(param, mode_str, WAL_HIPRIV_CMD_NAME_MAX_LEN, &off_set);
    if (result != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_mode::wal_get_cmd_one_arg vap name err %d!}", result);
        return result;
    }

    mode_str[OAL_SIZEOF(mode_str) - 1] = '\0'; /* 确保以null结尾 */

    for (prot_idx = 0; g_ast_mode_map[prot_idx].pc_name != NULL; prot_idx++) {
        ret = oal_strcmp(g_ast_mode_map[prot_idx].pc_name, mode_str);
        if (ret == 0) {
            break;
        }
    }

    if (g_ast_mode_map[prot_idx].pc_name == NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_set_mode::unrecognized protocol string!}");
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    /* 发送消息到wal层处理 */
    ret = wal_hipriv_set_mode_config(net_dev, prot_idx);
    if (ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_set_mode::send cfg event fail!}");
        return ret;
    }
    return OAL_SUCC;
}

uint32_t wal_ioctl_set_freq(oal_net_device_stru *net_dev, int8_t *pc_param)
{
    wal_msg_write_stru write_msg = {0};
    int32_t l_freq;
    uint32_t off_set = 0;
    int8_t ac_freq[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint32_t result;
    int32_t ret;
    wal_msg_stru *rsp_msg = NULL;

    /* pc_param指向新创建的net_device的name, 将其取出存放到ac_name中 */
    result = wal_get_cmd_one_arg(pc_param, ac_freq, WAL_HIPRIV_CMD_NAME_MAX_LEN, &off_set);
    if (result != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_freq::wal_get_cmd_one_arg vap name err %d!}", result);
        return result;
    }

    l_freq = oal_atoi(ac_freq);
    oam_info_log1(0, OAM_SF_ANY, "{wal_hipriv_set_freq::l_freq = %d!}", l_freq);

    /* 抛事件到wal层处理 */
    WAL_WRITE_MSG_HDR_INIT(&write_msg, WLAN_CFGID_CURRENT_CHANEL, OAL_SIZEOF(int32_t));
    *((int32_t *)(write_msg.auc_value)) = l_freq;

    /* 发送消息 */
    ret = wal_send_cfg_event(net_dev,
                             WAL_MSG_TYPE_WRITE,
                             WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(int32_t),
                             (uint8_t *)&write_msg,
                             OAL_TRUE,
                             &rsp_msg);
    if (oal_unlikely(ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_freq::err %d!}", ret);
        return (uint32_t)ret;
    }
    /* 读取返回的错误码 */
    result = wal_check_and_release_msg_resp(rsp_msg);
    if (result != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_DFR, "{wal_hipriv_set_freq fail, err code[%u]!}", result);
        return result;
    }

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_MONITOR
uint32_t wal_ioctl_set_sniffer_config(oal_net_device_stru *net_dev, mac_cfg_sniffer_param_stru *cfg_sniffer_param)
{
    mac_cfg_sniffer_param_stru *pst_cfg_sniffer_param = NULL;
    wal_msg_write_stru write_msg = {0};
    uint32_t ul_mac_address_index, result;
    int32_t ret;
    wal_msg_stru *rsp_msg = NULL;

    /* 抛事件到wal层处理 */
    WAL_WRITE_MSG_HDR_INIT(&write_msg, WLAN_CFGID_SNIFFER, OAL_SIZEOF(mac_cfg_sniffer_param_stru));

    /* 设置配置命令参数 */
    pst_cfg_sniffer_param = (mac_cfg_sniffer_param_stru *)(write_msg.auc_value);
    for (ul_mac_address_index = 0; ul_mac_address_index < WLAN_MAC_ADDR_LEN; ul_mac_address_index++) {
        pst_cfg_sniffer_param->auc_mac_addr[ul_mac_address_index] =
            cfg_sniffer_param->auc_mac_addr[ul_mac_address_index];
    }
    pst_cfg_sniffer_param->uc_sniffer_mode = cfg_sniffer_param->uc_sniffer_mode;

    ret = wal_send_cfg_event(net_dev,
                             WAL_MSG_TYPE_WRITE,
                             WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_sniffer_param_stru),
                             (uint8_t *)&write_msg,
                             OAL_TRUE, &rsp_msg);

    if (oal_unlikely(ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_sniffer::err [%d]!}", ret);
        return (uint32_t)ret;
    }
    /* 读取返回的错误码 */
    result = wal_check_and_release_msg_resp(rsp_msg);
    if (result != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_DFR, "{wal_hipriv_set_sniffer fail, err code[%u]!}", result);
        return result;
    }

    return OAL_SUCC;
}

uint32_t wal_ioctl_set_sniffer(oal_net_device_stru *net_dev, int8_t *pc_param)
{
    uint32_t off_set = 0;
    int8_t ac_arg[WAL_HIPRIV_CMD_NAME_MAX_LEN], ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint8_t uc_sniffer_mode;
    int32_t l_idx = 0;
    mac_cfg_sniffer_param_stru st_cfg_sniffer_param; /* 临时保存sniffer配置信息 */
    uint32_t result;

    memset_s((uint8_t *)&st_cfg_sniffer_param, OAL_SIZEOF(st_cfg_sniffer_param), 0, OAL_SIZEOF(st_cfg_sniffer_param));

    /* 获取sniffer开关标志 */
    result = wal_get_cmd_one_arg(pc_param, ac_arg, WAL_HIPRIV_CMD_NAME_MAX_LEN, &off_set);
    if (result != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_sniffer::get cmd one arg err[%d]!}", result);
        return result;
    }

    /* 输入命令合法性检测 */
    while (ac_arg[l_idx] != '\0') {
        if (isdigit(ac_arg[l_idx])) {
            l_idx++;
            continue;
        } else {
            l_idx++;
            oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_set_sniffer::input illegal!}");
            return OAL_ERR_CODE_INVALID_CONFIG;
        }
    }
    /* 将命令参数值字符串转化为整数 */
    uc_sniffer_mode = (uint8_t)oal_atoi(ac_arg);

    if (uc_sniffer_mode > WLAN_SNIFFER_STATE_BUTT) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_set_sniffer::sniffer mode invalid.}");
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    st_cfg_sniffer_param.uc_sniffer_mode = uc_sniffer_mode;
    /* 偏移，取下一个参数 */
    pc_param = pc_param + off_set;

    /* 获取mac地址 */
    result = wal_get_cmd_one_arg(pc_param, ac_name, WAL_HIPRIV_CMD_NAME_MAX_LEN, &off_set);
    if (result != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_sniffer::get cmd one arg err[%d]!}", result);
        return result;
    }

    oal_strtoaddr(ac_name, OAL_SIZEOF(ac_name), st_cfg_sniffer_param.auc_mac_addr, WLAN_MAC_ADDR_LEN);

    oam_warning_log4(0, OAM_SF_ANY, "{wal_hipriv_set_sniffer::uc_sniffer_mode [%d],mac[3-4]=[%x:%x:%x].}",
                     uc_sniffer_mode, st_cfg_sniffer_param.auc_mac_addr[3],
                     st_cfg_sniffer_param.auc_mac_addr[4], st_cfg_sniffer_param.auc_mac_addr[5]);
    result = wal_ioctl_set_sniffer_config(net_dev, &st_cfg_sniffer_param);
    if (result != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_set_sniffer::wal_ioctl_set_sniffer_config err[%d]!}", result);
        return result;
    }
    if (oal_value_eq_any2(uc_sniffer_mode, WLAN_SNIFFER_TRAVEL_CAP_ON, WLAN_SINFFER_ON)) {
        wal_clear_continuous_disable_enable_wifi_cnt(); // 开启sniffer后将计数清零
    }

    return OAL_SUCC;
}


uint32_t wal_ioctl_set_monitor_mode(oal_net_device_stru *net_dev, int8_t *pc_param)
{
    wal_msg_write_stru write_msg = {0};
    uint32_t off_set = 0;
    uint32_t result;
    int32_t ret;
    int8_t ac_arg[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint8_t uc_monitor_mode;
    int32_t l_idx = 0;
    wal_msg_stru *rsp_msg = NULL;

    /* 获取monitor mode开关标志 */
    result = wal_get_cmd_one_arg(pc_param, ac_arg, WAL_HIPRIV_CMD_NAME_MAX_LEN, &off_set);
    if (result != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_enable_monitor_mode::get cmd one arg err[%d]!}", result);
        return result;
    }

    /* 输入命令合法性检测 */
    while (ac_arg[l_idx] != '\0') {
        if (isdigit(ac_arg[l_idx])) {
            l_idx++;
            continue;
        } else {
            l_idx++;
            oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_enable_monitor_mode::input illegal!}");
            return OAL_ERR_CODE_INVALID_CONFIG;
        }
    }

    /* 将命令参数值字符串转化为整数 */
    uc_monitor_mode = (uint8_t)oal_atoi(ac_arg);

    if (uc_monitor_mode > WLAN_MONITOR_STATE_BUTT) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_enable_monitor_mode::input should be 0 or 1.}");
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    *(uint8_t *)(write_msg.auc_value) = uc_monitor_mode;

    oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_enable_monitor_mode::uc_sniffer_mode [%d].}", uc_monitor_mode);

    /* 抛事件到wal层处理 */
    WAL_WRITE_MSG_HDR_INIT(&write_msg, WLAN_CFGID_MONITOR_MODE, OAL_SIZEOF(uint8_t));

    ret = wal_send_cfg_event(net_dev,
                             WAL_MSG_TYPE_WRITE,
                             WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(uint8_t),
                             (uint8_t *)&write_msg,
                             OAL_TRUE,
                             &rsp_msg);

    if (oal_unlikely(ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_enable_monitor_mode::err [%d]!}", ret);
        return (uint32_t)ret;
    }

    /* 读取返回的错误码 */
    result = wal_check_and_release_msg_resp(rsp_msg);
    if (result != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_DFR, "{wal_hipriv_enable_monitor_mode fail, err code[%u]!}", result);
        return result;
    }

    return OAL_SUCC;
}


#endif

uint32_t wal_ioctl_set_al_rx(oal_net_device_stru *net_dev, int8_t *pc_param)
{
    wal_msg_write_stru write_msg = {0};
    uint32_t off_set = 0;
    uint32_t result;
    int32_t ret;
    int8_t ac_arg[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    uint8_t uc_rx_flag;
    int32_t l_idx = 0;
    wal_msg_stru *rsp_msg = NULL;

    /* 获取常收模式开关标志 */
    result = wal_get_cmd_one_arg(pc_param, ac_arg, WAL_HIPRIV_CMD_NAME_MAX_LEN, &off_set);
    if (result != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_always_rx::get cmd one arg err[%d]!}", result);
        return result;
    }

    /* 输入命令合法性检测 */
    while ('\0' != ac_arg[l_idx]) {
        if (isdigit(ac_arg[l_idx])) {
            l_idx++;
            continue;
        } else {
            l_idx++;
            oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_always_rx::input illegal!}");
            return OAL_ERR_CODE_INVALID_CONFIG;
        }
    }

    /* 将命令参数值字符串转化为整数 */
    uc_rx_flag = (uint8_t)oal_atoi(ac_arg);

    if (uc_rx_flag > HAL_ALWAYS_RX_RESERVED) {
        oam_warning_log0(0, OAM_SF_ANY, "{wal_hipriv_always_rx::input should be 0 or 1.}");
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    *(uint8_t *)(write_msg.auc_value) = uc_rx_flag;


    WAL_WRITE_MSG_HDR_INIT(&write_msg, WLAN_CFGID_SET_ALWAYS_RX, OAL_SIZEOF(uint8_t));

    ret = wal_send_cfg_event(net_dev,
                             WAL_MSG_TYPE_WRITE,
                             WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(uint8_t),
                             (uint8_t *)&write_msg,
                             OAL_TRUE,
                             &rsp_msg);

    if (oal_unlikely(ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_ANY, "{wal_hipriv_always_rx::err [%d]!}", ret);
        return (uint32_t)ret;
    }
    /* 读取返回的错误码 */
    result = wal_check_and_release_msg_resp(rsp_msg);
    if (result != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_DFR, "{wal_hipriv_always_rx fail, err code[%u]!}", result);
        return result;
    }

    return OAL_SUCC;
}


uint32_t wal_ioctl_set_pm_switch(mac_vap_stru *mac_vap, uint32_t *params)
{
    uint8_t switch_pm = (uint8_t)params[0];

    oam_warning_log1(0, OAM_SF_PWR, "{wal_ioctl_set_pm_switch:[%d]}\r\n", switch_pm);

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    if ((switch_pm == 1) || (switch_pm == 4)) { /* 1, 4 关闭pm */
        g_wlan_host_pm_switch = OAL_TRUE;
        wlan_pm_enable();
    } else {
        wlan_pm_disable();
        g_wlan_host_pm_switch = OAL_FALSE;
    }
#endif

#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
    g_wlan_device_pm_switch = switch_pm;
#endif
    return OAL_SUCC;
}


