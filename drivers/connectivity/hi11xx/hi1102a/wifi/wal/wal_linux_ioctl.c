

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "oal_ext_if.h"
#include "oal_profiling.h"
#include "oal_kernel_file.h"
#include "oal_cfg80211.h"
#include "oal_main.h"
#include "plat_pm_wlan.h"

#include "oam_ext_if.h"
#include "frw_ext_if.h"

#include "wlan_types.h"

#include "mac_vap.h"
#include "mac_resource.h"
#include "mac_regdomain.h"
#include "mac_ie.h"

#include "hmac_ext_if.h"
#include "hmac_chan_mgmt.h"

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

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
#include "oal_hcc_host_if.h"
#include "plat_cali.h"
#endif

#ifdef _PRE_WLAN_FEATURE_ARP_OFFLOAD
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include <linux/notifier.h>
#include <linux/inetdevice.h>
#include <net/addrconf.h>
#endif
#include "hmac_arp_offload.h"
#endif

#include "hmac_statistic_data_flow.h"

#ifdef _PRE_WLAN_TCP_OPT
#include "hmac_tcp_opt.h"
#endif

#ifdef _PRE_WLAN_FEATURE_ROAM
#include "hmac_roam_main.h"
#endif  // _PRE_WLAN_FEATURE_ROAM
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
#include "hisi_customize_wifi.h"
#endif /* #ifdef _PRE_PLAT_FEATURE_CUSTOMIZE */

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE) && (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include "plat_pm_wlan.h"
#include "plat_firmware.h"
#endif
#ifdef _PRE_WLAN_FEATURE_MULTI_NETBUF_AMSDU
#include "hmac_tx_amsdu.h"
#endif

#include "hmac_tcp_ack_filter.h"
#include "hmac_auto_ddr_freq.h"
#include "hmac_tx_opt.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_WAL_LINUX_IOCTL_C

#define MAX_PRIV_CMD_SIZE         4096
#define WAL_MAX_SPE_PORT_NUM      8   /* SPE���˿ں�Ϊ8 */
#define WAL_MAX_SPE_PKT_NUM       512 /* SPE����ķ��ͺͽ����������Լ���Ӧ�İ��ĸ��� */
#define PPS_VALUE_MIN             0   /* ���ƻ��·���pps value������ˮ�� */
#define PPS_VALUE_MAX             16000
#define PRIV_CMD_STRU_HEAD_LEN    8 /* private cmd:l_total_len��l_used_len��ռ�ռ䳤�� */
#define OAL_AP_GET_ASSOC_LIST_LEN 1024

/*****************************************************************************
  2 �ṹ�嶨��
*****************************************************************************/
typedef struct {
    oal_int8 *pc_country;                    /* �����ַ��� */
    mac_dfs_domain_enum_uint8 en_dfs_domain; /* DFS �״��׼ */
} wal_dfs_domain_entry_stru;

typedef struct {
    oal_uint32 ul_ap_max_user;           /* ap����û��� */
    oal_int8 ac_ap_mac_filter_mode[257]; /* AP mac��ַ�����������,�256 */
    oal_int32 l_ap_power_flag;           /* AP�ϵ��־ */
} wal_ap_config_stru;

/* 110x ���ڽ��ϲ��·��ַ����������л� */
typedef struct {
    oal_uint8 *pc_priv_cmd;
    oal_uint32 ul_case_entry;
} wal_ioctl_priv_cmd_stru;

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
OAL_STATIC oal_proc_dir_entry_stru *g_pst_proc_entry;

OAL_STATIC wal_ap_config_stru g_st_ap_config_info = { 0 }; /* AP������Ϣ,��Ҫ��vap �������·��� */

#if (_PRE_OS_VERSION_WIN32 == _PRE_OS_VERSION)
/* Just For UT */
OAL_STATIC oal_uint32 g_wal_wid_queue_init_flag = OAL_FALSE;
#endif
OAL_STATIC wal_msg_queue g_wal_wid_msg_queue;

#ifdef _PRE_WLAN_FEATURE_TCP_ACK_BUFFER
mac_tcp_ack_buf_cfg_table_stru g_ast_hmac_tcp_ack_buf_cfg_table[] = {
    { "enable", MAC_TCP_ACK_BUF_ENABLE },
    { "timeout", MAC_TCP_ACK_BUF_TIMEOUT },
    { "count", MAC_TCP_ACK_BUF_MAX },
};
#endif

mac_ps_params_cfg_table_stru g_ast_hmac_ps_params_cfg_table[] = {
    { "timeout", MAC_PS_PARAMS_TIMEOUT },
    { "count", MAC_PS_PARAMS_RESTART_COUNT },
    { "all", MAC_PS_PARAMS_ALL },
};

/* hi1102-cb add sys for 51/02 */
#ifdef _PRE_CONFIG_CONN_HISI_SYSFS_SUPPORT
struct kobject *gp_sys_kobject;
#endif

#ifdef _PRE_WLAN_FEATURE_ARP_OFFLOAD
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
oal_int32 wal_hipriv_inetaddr_notifier_call(struct notifier_block *this, oal_uint event, oal_void *ptr);

OAL_STATIC struct notifier_block wal_hipriv_notifier = {
    .notifier_call = wal_hipriv_inetaddr_notifier_call
};

oal_int32 wal_hipriv_inet6addr_notifier_call(struct notifier_block *this, oal_uint event, oal_void *ptr);

OAL_STATIC struct notifier_block wal_hipriv_notifier_ipv6 = {
    .notifier_call = wal_hipriv_inet6addr_notifier_call
};
#endif
#endif

#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
/* ÿ�����µ�������vap��ɵĶ��ƻ�ֻ����һ�Σ�wlan p2p iface�����ظ����� */
OAL_STATIC oal_uint8 g_uc_cfg_once_flag = OAL_FALSE;
/* ֻ��staut��aput��һ���ϵ�ʱ��ini�����ļ��ж�ȡ���� */
OAL_STATIC oal_uint8 g_uc_cfg_flag = OAL_FALSE;
#endif /* #ifdef _PRE_PLAT_FEATURE_CUSTOMIZE */
#ifdef _PRE_WLAN_CFGID_DEBUG
extern OAL_CONST wal_hipriv_cmd_entry_stru g_ast_hipriv_cmd_debug[];
#endif
/* ��̬�������� */
OAL_STATIC oal_uint32 wal_hipriv_vap_log_level(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param);
oal_uint32 wal_hipriv_set_mcast_data_dscr_param(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param);

oal_uint32 wal_hipriv_setcountry(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param);
OAL_STATIC oal_uint32 wal_hipriv_getcountry(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param);
oal_uint32 wal_hipriv_set_bw(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param);
OAL_STATIC oal_uint32 wal_hipriv_set_pow_rf_ctl(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param);
#ifdef _PRE_WLAN_FEATURE_ALWAYS_TX
OAL_STATIC oal_uint32 wal_hipriv_always_tx_1102(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param);
OAL_STATIC oal_uint32 wal_hipriv_always_tx_num(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param);
#endif

OAL_STATIC oal_uint32 wal_hipriv_always_rx(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param);
OAL_STATIC oal_uint32 wal_hipriv_pcie_pm_level(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param);
OAL_STATIC oal_uint32 wal_hipriv_user_info(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param);
oal_uint32 wal_hipriv_add_vap(oal_net_device_stru *pst_cfg_net_dev, oal_int8 *pc_param);

#ifdef _PRE_WLAN_FEATURE_BTCOEX
OAL_STATIC oal_uint32 wal_hipriv_btcoex_status_print(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param);
#endif
#ifdef _PRE_WLAN_FEATURE_LTECOEX
OAL_STATIC oal_uint32 wal_ioctl_ltecoex_mode_set(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param);
#endif

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
OAL_STATIC oal_uint32 wal_hipriv_aifsn_cfg(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param);
OAL_STATIC oal_uint32 wal_hipriv_cw_cfg(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param);
#endif

OAL_STATIC oal_uint32 wal_hipriv_set_random_mac_addr_scan(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param);
oal_uint32 wal_hipriv_add_user(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param);
oal_uint32 wal_hipriv_del_user(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param);

OAL_STATIC oal_uint32 wal_hipriv_reg_info(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param);

#if (defined(_PRE_PRODUCT_ID_HI110X_DEV) || defined(_PRE_PRODUCT_ID_HI110X_HOST))
OAL_STATIC oal_uint32 wal_hipriv_sdio_flowctrl(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param);
#endif
OAL_STATIC oal_uint32 wal_hipriv_set_2040_coext_support(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param);
OAL_STATIC oal_uint32 wal_hipriv_rx_fcs_info(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param);
oal_int32 wal_netdev_open(oal_net_device_stru *pst_net_dev);
oal_int32 wal_net_device_ioctl(oal_net_device_stru *pst_net_dev, oal_ifreq_stru *pst_ifr, oal_int32 ul_cmd);
#if ((!defined(_PRE_PRODUCT_ID_HI110X_DEV)) && (!defined(_PRE_PRODUCT_ID_HI110X_HOST)))
OAL_STATIC oal_int32 wal_ioctl_get_assoc_req_ie(
    oal_net_device_stru *pst_net_dev, oal_net_dev_ioctl_data_stru *pst_ioctl_data);
OAL_STATIC oal_int32 wal_ioctl_set_auth_mode(
    oal_net_device_stru *pst_net_dev, oal_net_dev_ioctl_data_stru *pst_ioctl_data);
OAL_STATIC oal_int32 wal_ioctl_set_country_code(
    oal_net_device_stru *pst_net_dev, oal_net_dev_ioctl_data_stru *pst_ioctl_data);
OAL_STATIC oal_int32 wal_ioctl_set_max_user(
    oal_net_device_stru *pst_net_dev, oal_net_dev_ioctl_data_stru *pst_ioctl_data);
OAL_STATIC oal_int32 wal_ioctl_nl80211_priv_connect(
    oal_net_device_stru *pst_net_dev, oal_net_dev_ioctl_data_stru *pst_ioctl_data);
OAL_STATIC oal_int32 wal_ioctl_nl80211_priv_disconnect(
    oal_net_device_stru *pst_net_dev, oal_net_dev_ioctl_data_stru *pst_ioctl_data);
OAL_STATIC oal_int32 wal_ioctl_set_channel(
    oal_net_device_stru *pst_net_dev, oal_net_dev_ioctl_data_stru *pst_ioctl_data);
OAL_STATIC oal_int32 wal_ioctl_set_wps_ie(
    oal_net_device_stru *pst_net_dev, oal_net_dev_ioctl_data_stru *pst_ioctl_data);
OAL_STATIC oal_int32 wal_ioctl_set_frag(oal_net_device_stru *pst_net_dev, oal_net_dev_ioctl_data_stru *pst_ioctl_data);
OAL_STATIC oal_int32 wal_ioctl_set_rts(oal_net_device_stru *pst_net_dev, oal_net_dev_ioctl_data_stru *pst_ioctl_data);
oal_int32 wal_ioctl_set_ssid(oal_net_device_stru *pst_net_dev, oal_net_dev_ioctl_data_stru *pst_ioctl_data);
#endif
OAL_STATIC oal_net_device_stats_stru *wal_netdev_get_stats(oal_net_device_stru *pst_net_dev);
OAL_STATIC oal_int32 wal_netdev_set_mac_addr(oal_net_device_stru *pst_net_dev, void *p_addr);

OAL_STATIC oal_uint32 wal_hipriv_set_mode(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param);
OAL_STATIC oal_uint32 wal_hipriv_set_freq(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param);
#ifdef _PRE_BT_FITTING_DATA_COLLECT
OAL_STATIC oal_uint32 wal_hipriv_init_bt_env(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param);
OAL_STATIC oal_uint32 wal_hipriv_set_bt_freq(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param);
OAL_STATIC oal_uint32 wal_hipriv_set_bt_upc_by_freq(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param);
OAL_STATIC oal_uint32 wal_hipriv_print_bt_gm(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param);
#endif
OAL_STATIC oal_uint32 wal_hipriv_set_tx_ba_policy(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param);
OAL_STATIC oal_uint32 wal_hipriv_dscr_th_opt(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param);
OAL_STATIC oal_uint32 wal_hipriv_tcp_ack_filter(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param);

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
OAL_STATIC int wal_ioctl_get_mode(oal_net_device_stru *pst_net_dev,
                                  oal_iw_request_info_stru *pst_info,
                                  oal_iwreq_data_union *pst_wrqu,
                                  char *pc_param);

OAL_STATIC oal_uint32 wal_hipriv_set_regdomain_pwr(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param);
OAL_STATIC oal_uint32 wal_hipriv_reg_write(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param);
OAL_STATIC oal_uint32 wal_hipriv_tpc_log(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param);
OAL_STATIC oal_uint32 wal_hipriv_dump_all_rx_dscr(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param);

#ifdef _PRE_WLAN_FEATURE_EDCA_OPT_AP
OAL_STATIC oal_uint32 wal_hipriv_set_edca_opt_switch_sta(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param);
#endif

#ifdef _PRE_WLAN_FEATURE_UAPSD
OAL_STATIC oal_uint32 wal_hipriv_set_uapsd_cap(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param);
#endif

#ifdef _PRE_WLAN_FEATURE_STA_PM
OAL_STATIC oal_int32 wal_ioctl_set_sta_pm(oal_net_device_stru *pst_net_dev, mac_pm_switch_enum_uint8 en_pm_enable,
                                          mac_pm_ctrl_type_enum_uint8 en_pm_ctrl_type);
#endif
OAL_STATIC oal_uint32 wal_hipriv_bgscan_enable(oal_net_device_stru *pst_cfg_net_dev, oal_int8 *pc_param);
#ifdef _PRE_WLAN_FEATURE_STA_PM
OAL_STATIC oal_uint32 wal_hipriv_sta_ps_mode(oal_net_device_stru *pst_cfg_net_dev, oal_int8 *pc_param);
#ifdef _PRE_PSM_DEBUG_MODE
OAL_STATIC oal_uint32 wal_hipriv_sta_ps_info(oal_net_device_stru *pst_cfg_net_dev, oal_int8 *pc_param);
#endif
#endif

#ifdef _PRE_WLAN_FEATURE_STA_UAPSD
OAL_STATIC oal_uint32 wal_hipriv_set_uapsd_para(oal_net_device_stru *pst_cfg_net_dev, oal_int8 *pc_param);
#endif
#ifdef _PRE_CONFIG_CONN_HISI_SYSFS_SUPPORT
/* hi1102-cb add sys for 51/02 */
OAL_STATIC oal_ssize_t wal_hipriv_sys_write(
    struct kobject *dev, struct kobj_attribute *attr, const char *buf, oal_size_t count);
OAL_STATIC oal_ssize_t wal_hipriv_sys_read(struct kobject *dev, struct kobj_attribute *attr, char *buf);
OAL_STATIC struct kobj_attribute dev_attr_hipriv =
    __ATTR(hipriv, OAL_S_IRUGO | OAL_S_IWUSR, wal_hipriv_sys_read, wal_hipriv_sys_write);
OAL_STATIC struct attribute *hipriv_sysfs_entries[] = {
    &dev_attr_hipriv.attr,
    NULL
};
OAL_STATIC struct attribute_group hipriv_attribute_group = {
    .attrs = hipriv_sysfs_entries,
};
/* hi1102-cb add sys for 51/02 */
#endif

#ifdef _PRE_WLAN_FEATURE_P2P
OAL_STATIC oal_int32 wal_ioctl_set_p2p_miracast_status(oal_net_device_stru *pst_net_dev, oal_uint8 uc_param);
OAL_STATIC oal_int32 wal_ioctl_set_p2p_noa(
    oal_net_device_stru *pst_net_dev, mac_cfg_p2p_noa_param_stru *pst_p2p_noa_param);
OAL_STATIC oal_int32 wal_ioctl_reduce_sar(oal_net_device_stru *pst_net_dev, oal_uint16 us_tx_power);
OAL_STATIC oal_int32 wal_ioctl_set_p2p_ops(
    oal_net_device_stru *pst_net_dev, mac_cfg_p2p_ops_param_stru *pst_p2p_ops_param);

#endif /* _PRE_WLAN_FEATURE_P2P */

#ifdef _PRE_WLAN_FEATURE_VOWIFI
OAL_STATIC oal_int32 wal_ioctl_set_vowifi_param(
    oal_net_device_stru *pst_net_dev, oal_int8 *puc_command, wal_wifi_priv_cmd_stru st_priv_cmd);
OAL_STATIC oal_int32 wal_ioctl_get_vowifi_param(
    oal_net_device_stru *pst_net_dev, oal_int8 *puc_command, oal_int32 *pl_value);
#endif

#ifdef _PRE_WLAN_FEATURE_HS20
OAL_STATIC oal_int32 wal_ioctl_set_qos_map(oal_net_device_stru *pst_net_dev,
                                           hmac_cfg_qos_map_param_stru *pst_qos_map_param);
#endif /* #ifdef _PRE_WLAN_FEATURE_HS20 */

oal_int32 wal_ioctl_set_wps_p2p_ie(oal_net_device_stru *pst_net_dev,
                                   oal_uint8 *puc_buf,
                                   oal_uint32 ul_len,
                                   en_app_ie_type_uint8 en_type);

OAL_STATIC oal_int32 wal_set_ap_max_user(oal_net_device_stru *pst_net_dev, oal_uint32 ul_ap_max_user);
OAL_STATIC oal_void wal_config_mac_filter(oal_net_device_stru *pst_net_dev, oal_int8 *pc_command);
OAL_STATIC oal_int32 wal_kick_sta(oal_net_device_stru *pst_net_dev, oal_uint8 *auc_mac_addr, oal_uint8 uc_mac_addr_len);
OAL_STATIC int wal_ioctl_set_ap_config(
    oal_net_device_stru *pst_net_dev, oal_iw_request_info_stru *pst_info,
    oal_iwreq_data_union *pst_wrqu, char *pc_extra);
OAL_STATIC int wal_ioctl_get_assoc_list(
    oal_net_device_stru *pst_net_dev, oal_iw_request_info_stru *pst_info,
    oal_iwreq_data_union *pst_wrqu, char *pc_extra);
OAL_STATIC int wal_ioctl_set_mac_filters(
    oal_net_device_stru *pst_net_dev, oal_iw_request_info_stru *pst_info,
    oal_iwreq_data_union *pst_wrqu, char *pc_extra);
OAL_STATIC int wal_ioctl_set_ap_sta_disassoc(
    oal_net_device_stru *pst_net_dev, oal_iw_request_info_stru *pst_info,
    oal_iwreq_data_union *pst_wrqu, char *pc_extra);
OAL_STATIC oal_uint32 wal_get_parameter_from_cmd(
    oal_int8 *pc_cmd, oal_int8 *pc_arg, OAL_CONST oal_int8 *puc_token,
    oal_uint32 *pul_cmd_offset, oal_uint32 ul_param_max_len);
OAL_STATIC oal_int32 wal_ioctl_priv_cmd_set_ap_wps_p2p_ie(
    oal_net_device_stru *pst_net_dev, oal_int8 *pc_command, wal_wifi_priv_cmd_stru st_priv_cmd);

#if defined(_PRE_PRODUCT_ID_HI110X_HOST)
oal_int32 wal_init_wlan_vap(oal_net_device_stru *pst_net_dev);
oal_int32 wal_deinit_wlan_vap(oal_net_device_stru *pst_net_dev);
oal_int32 wal_start_vap(oal_net_device_stru *pst_net_dev);
oal_int32 wal_stop_vap(oal_net_device_stru *pst_net_dev);
oal_void wal_set_mac_addr(oal_net_device_stru *pst_net_dev);
oal_int32 wal_init_wlan_netdev(oal_wiphy_stru *pst_wiphy, char *dev_name);
oal_int32 wal_setup_ap(oal_net_device_stru *pst_net_dev);
#endif
#ifdef _PRE_WLAN_FEATURE_11K
OAL_STATIC oal_uint32 wal_hipriv_send_neighbor_req(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param);
OAL_STATIC oal_uint32 wal_hipriv_beacon_req_table_switch(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param);
#endif
OAL_STATIC oal_uint32 wal_hipriv_voe_enable(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param);
#ifdef _PRE_WLAN_FEATURE_APF
OAL_STATIC oal_uint32 wal_hipriv_apf_filter_list(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param);
#endif
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
OAL_STATIC oal_uint32 wal_hipriv_set_pm_debug_switch(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param);
OAL_STATIC oal_int32 wal_ioctl_get_wifi_priv_feature_cap_param(
    oal_net_device_stru *pst_net_dev, oal_int8 *puc_command, oal_int32 *pl_value);
#ifndef CONFIG_HAS_EARLYSUSPEND
OAL_STATIC oal_int32 wal_ioctl_set_suspend_mode(oal_net_device_stru *pst_net_dev, oal_uint8 uc_suspend);
#endif
#endif
#ifdef _PRE_WLAN_FEATURE_TCP_ACK_BUFFER
OAL_STATIC oal_uint32 wal_hipriv_tcp_ack_buf_cfg(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param);
#endif
OAL_STATIC oal_uint32 wal_hipriv_ps_params_cfg(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param);
OAL_STATIC oal_uint32 wal_hipriv_mcs_set_check_enable(oal_net_device_stru *pst_cfg_net_dev, oal_int8 *pc_param);
OAL_STATIC oal_uint32 wal_hipriv_set_sar_level(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param);
OAL_STATIC oal_uint32 wal_ioctl_force_pass_filter(oal_net_device_stru *pst_net_dev, oal_uint8 uc_param);
#ifdef _PRE_WLAN_FEATURE_BTCOEX
OAL_STATIC oal_uint32 wal_hipriv_set_btcoex_params(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param);
#endif
OAL_STATIC oal_uint32 wal_hipriv_set_tx_pow_param(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param);

#ifdef _PRE_WLAN_NARROW_BAND
oal_int32 wal_set_nbfh(oal_net_device_stru *pst_net_dev);
#endif
#ifdef _PRE_WLAN_FEATURE_NRCOEX
OAL_STATIC oal_uint32 wal_hipriv_nrcoex_cmd(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param);
OAL_STATIC oal_int32 wal_ioctl_process_nrcoex_priority(oal_net_device_stru *pst_net_dev,
                                                       oal_int8 *pc_command);
OAL_STATIC oal_int32 wal_ioctl_get_nrcoex_info(oal_net_device_stru *pst_net_dev,
                                               oal_ifreq_stru *pst_ifr,
                                               oal_int8 *pc_command);

OAL_STATIC oal_uint32 wal_hipriv_nrcoex_test_cmd(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param);

#endif

/*****************************************************************************
  ˽���������. ˽�������ʽ:
         �豸�� ������ ����
  hipriv "Hisilicon0 create vap0"
*****************************************************************************/
/* private command strings */
#define CMD_SET_AP_WPS_P2P_IE "SET_AP_WPS_P2P_IE"
#define CMD_P2P_SET_NOA       "P2P_SET_NOA"
#define CMD_P2P_SET_PS        "P2P_SET_PS"
#define CMD_COUNTRY           "COUNTRY"
#ifdef _PRE_WLAN_FEATURE_LTECOEX
#define CMD_LTECOEX_MODE "LTECOEX_MODE"
#endif
#ifdef _PRE_WLAN_FEATURE_P2P
#define CMD_SET_STA_PM_ON "SET_STA_PM_ON"
#endif
#define CMD_SET_QOS_MAP    "SET_QOS_MAP"
#define CMD_TX_POWER       "TX_POWER"
#define CMD_WPAS_GET_CUST  "WPAS_GET_CUST"
#define CMD_SET_HITALK_STR "SET_HITALK"

#ifdef _PRE_WLAN_FEATURE_VOWIFI

#define CMD_VOWIFI_SET_MODE           "VOWIFI_DETECT SET MODE"
#define CMD_VOWIFI_GET_MODE           "VOWIFI_DETECT GET MODE"
#define CMD_VOWIFI_SET_PERIOD         "VOWIFI_DETECT SET PERIOD"
#define CMD_VOWIFI_GET_PERIOD         "VOWIFI_DETECT GET PERIOD"
#define CMD_VOWIFI_SET_LOW_THRESHOLD  "VOWIFI_DETECT SET LOW_THRESHOLD"
#define CMD_VOWIFI_GET_LOW_THRESHOLD  "VOWIFI_DETECT GET LOW_THRESHOLD"
#define CMD_VOWIFI_SET_HIGH_THRESHOLD "VOWIFI_DETECT SET HIGH_THRESHOLD"
#define CMD_VOWIFI_GET_HIGH_THRESHOLD "VOWIFI_DETECT GET HIGH_THRESHOLD"
#define CMD_VOWIFI_SET_TRIGGER_COUNT  "VOWIFI_DETECT SET TRIGGER_COUNT"
#define CMD_VOWIFI_GET_TRIGGER_COUNT  "VOWIFI_DETECT GET TRIGGER_COUNT"

#define CMD_VOWIFI_SET_PARAM "VOWIFI_DETECT SET"
#define CMD_VOWIFI_GET_PARAM "VOWIFI_DETECT GET"

#define CMD_VOWIFI_IS_SUPPORT_REPLY "true"
#else
#define CMD_VOWIFI_IS_SUPPORT_REPLY "false"
#endif /* _PRE_WLAN_FEATURE_VOWIFI */
#define CMD_VOWIFI_IS_SUPPORT                "VOWIFI_DETECT VOWIFi_IS_SUPPORT"
#define CMD_GET_WIFI_PRIV_FEATURE_CAPABILITY "GET_WIFI_PRIV_FEATURE_CAPABILITY"

#ifdef _PRE_WLAN_FEATURE_IP_FILTER
#define CMD_SET_RX_FILTER_ENABLE    "set_rx_filter_enable"
#define CMD_ADD_RX_FILTER_ITEMS     "add_rx_filter_items"
#define CMD_CLEAR_RX_FILTERS        "clear_rx_filters"
#define CMD_GET_RX_FILTER_PKT_STATE "get_rx_filter_pkt_state"

#define CMD_FILTER_SWITCH "FILTER"
#endif /* _PRE_WLAN_FEATURE_IP_FILTER */

#define CMD_SETSUSPENDOPT  "SETSUSPENDOPT"
#define CMD_SETSUSPENDMODE "SETSUSPENDMODE"

#define CMD_RXFILTER_START "RXFILTER-START"
#define CMD_RXFILTER_STOP  "RXFILTER-STOP"
#define CMD_MIRACAST_START "MIRACAST 1"
#define CMD_MIRACAST_STOP  "MIRACAST 0"

#ifdef _PRE_WLAN_FEATURE_TAS_ANT_SWITCH
/* TAS�����л����� */
#define CMD_SET_ANT_CHANGE "SET_ANT_CHANGE"
/* �������� */
#define CMD_MEASURE_TAS_RSSI "SET_TAS_MEASURE"
/* ̧���� */
#define CMD_SET_TAS_TXPOWER "SET_TAS_TXPOWER"
/* ��ȡ���� */
#define CMD_TAS_GET_ANT "TAS_GET_ANT"
#endif
#define CMD_SET_NRCOEX_PRIOR "SET_NRCOEX_PRIOR"
#define CMD_GET_NRCOEX_INFO  "GET_NRCOEX_INFO"
#define CMD_GET_BEACON_CNT   "GET_BEACON_CNT"
#define CMD_GET_APF_PKTS_CNT "GET_APF_PKTS_CNT"
/* ˽��������Ʋ������Ⱥ� */
#define CMD_SET_AP_WPS_P2P_IE_LEN                (OAL_STRLEN(CMD_SET_AP_WPS_P2P_IE))
#define CMD_P2P_SET_NOA_LEN                      (OAL_STRLEN(CMD_P2P_SET_NOA))
#define CMD_P2P_SET_PS_LEN                       (OAL_STRLEN(CMD_P2P_SET_PS))
#define CMD_SET_STA_PM_ON_LEN                    (OAL_STRLEN(CMD_SET_STA_PM_ON))
#define CMD_SET_QOS_MAP_LEN                      (OAL_STRLEN(CMD_SET_QOS_MAP))
#define CMD_COUNTRY_LEN                          (OAL_STRLEN(CMD_COUNTRY))
#define CMD_LTECOEX_MODE_LEN                     (OAL_STRLEN(CMD_LTECOEX_MODE))
#define CMD_TX_POWER_LEN                         (OAL_STRLEN(CMD_TX_POWER))
#define CMD_WPAS_GET_CUST_LEN                    (OAL_STRLEN(CMD_WPAS_GET_CUST))
#define CMD_VOWIFI_SET_PARAM_LEN                 (OAL_STRLEN(CMD_VOWIFI_SET_PARAM))
#define CMD_VOWIFI_GET_PARAM_LEN                 (OAL_STRLEN(CMD_VOWIFI_GET_PARAM))
#define CMD_SETSUSPENDOPT_LEN                    (OAL_STRLEN(CMD_SETSUSPENDOPT))
#define CMD_SETSUSPENDMODE_LEN                   (OAL_STRLEN(CMD_SETSUSPENDMODE))
#define CMD_GET_WIFI_PRIV_FEATURE_CAPABILITY_LEN (OAL_STRLEN(CMD_GET_WIFI_PRIV_FEATURE_CAPABILITY))
#define CMD_VOWIFI_IS_SUPPORT_LEN                (OAL_STRLEN(CMD_VOWIFI_IS_SUPPORT))
#define CMD_VOWIFI_IS_SUPPORT_REPLY_LEN          (OAL_STRLEN(CMD_VOWIFI_IS_SUPPORT_REPLY))
#define CMD_FILTER_SWITCH_LEN                    (OAL_STRLEN(CMD_FILTER_SWITCH))
#define CMD_RXFILTER_START_LEN                   (OAL_STRLEN(CMD_RXFILTER_START))
#define CMD_RXFILTER_STOP_LEN                    (OAL_STRLEN(CMD_RXFILTER_STOP))
#define CMD_MIRACAST_START_LEN                   (OAL_STRLEN(CMD_MIRACAST_START))
#define CMD_MIRACAST_STOP_LEN                    (OAL_STRLEN(CMD_MIRACAST_STOP))
#define CMD_SET_TAS_TXPOWER_LEN                  (OAL_STRLEN(CMD_SET_TAS_TXPOWER))
#define CMD_MEASURE_TAS_RSSI_LEN                 (OAL_STRLEN(CMD_MEASURE_TAS_RSSI))
#define CMD_TAS_GET_ANT_LEN                      (OAL_STRLEN(CMD_TAS_GET_ANT))
#define CMD_SET_ANT_CHANGE_LEN                   (OAL_STRLEN(CMD_SET_ANT_CHANGE))
#define CMD_SET_NRCOEX_PRIOR_LEN                 (OAL_STRLEN(CMD_SET_NRCOEX_PRIOR))
#define CMD_GET_NRCOEX_INFO_LEN                  (OAL_STRLEN(CMD_GET_NRCOEX_INFO))
#define CMD_GET_BEACON_CNT_LEN                   (OAL_STRLEN(CMD_GET_BEACON_CNT))
#define CMD_GET_APF_PKTS_CNT_LEN                 (OAL_STRLEN(CMD_GET_APF_PKTS_CNT))
#ifdef _PRE_WLAN_FEATURE_VOWIFI
wal_ioctl_priv_cmd_stru g_ast_vowifi_cmd_table[VOWIFI_CMD_BUTT] = {
    { CMD_VOWIFI_SET_MODE,           VOWIFI_SET_MODE },
    { CMD_VOWIFI_GET_MODE,           VOWIFI_GET_MODE },
    { CMD_VOWIFI_SET_PERIOD,         VOWIFI_SET_PERIOD },
    { CMD_VOWIFI_GET_PERIOD,         VOWIFI_GET_PERIOD },
    { CMD_VOWIFI_SET_LOW_THRESHOLD,  VOWIFI_SET_LOW_THRESHOLD },
    { CMD_VOWIFI_GET_LOW_THRESHOLD,  VOWIFI_GET_LOW_THRESHOLD },
    { CMD_VOWIFI_SET_HIGH_THRESHOLD, VOWIFI_SET_HIGH_THRESHOLD },
    { CMD_VOWIFI_GET_HIGH_THRESHOLD, VOWIFI_GET_HIGH_THRESHOLD },
    { CMD_VOWIFI_SET_TRIGGER_COUNT,  VOWIFI_SET_TRIGGER_COUNT },
    { CMD_VOWIFI_GET_TRIGGER_COUNT,  VOWIFI_GET_TRIGGER_COUNT },
    { CMD_VOWIFI_IS_SUPPORT,         VOWIFI_SET_IS_SUPPORT },
};
#endif

OAL_STATIC OAL_CONST wal_hipriv_cmd_entry_stru g_ast_hipriv_cmd[] = {
    /************************���ö��ⷢ����˽������*******************/
    /* ��ӡvap�����в�����Ϣ: hipriv "vap0 info" */
    { "info", wal_hipriv_vap_info },
    /* ���ù��������� hipriv "Hisilicon0 setcountry param"paramȡֵΪ��д�Ĺ������֣����� CN US */
    { "setcountry", wal_hipriv_setcountry },
    /* ��ѯ���������� hipriv "Hisilicon0 getcountry" */
    { "getcountry", wal_hipriv_getcountry },
#ifdef _PRE_WLAN_FEATURE_IP_FILTER
    /*
        ip filter(���ܵ��Խӿ�)hipriv "wlan0 ip_filter cmd param0 param1 ...."
        ����:�������� "wlan0 ip_filter set_rx_filter_enable 1/0"
        ��պ����� "wlan0 ip_filter clear_rx_filters"
        ���ú����� "wlan0 ip_filter add_rx_filter_items ��Ŀ����(0/1/2...)
        ��������(protocol0 port0 protocol1 port1...)",Ŀǰ�õ��Խӿڽ�֧��20����Ŀ
 */
    { "ip_filter", wal_hipriv_set_ip_filter },
#endif  // _PRE_WLAN_FEATURE_IP_FILTER
    /* ��ӡָ��mac��ַuser�����в�����Ϣ: hipriv "vap0 userinfo XX XX XX XX XX XX(16����oal_strtohex)" */
    { "userinfo", wal_hipriv_user_info },
    /*
       ��ӡ�Ĵ�����Ϣ:
       hipriv "Hisilicon0 reginfo 16|32(51û��16λ�Ĵ�����ȡ����) regtype(soc/mac/phy) startaddr endaddr"
 */
    { "reginfo", wal_hipriv_reg_info },
    /* ����pcie�͹��ļ��� hipriv "Hisilicon0 pcie_pm_level level(01/2/3/4)" */
    { "pcie_pm_level", wal_hipriv_pcie_pm_level },
    /*
        ��ӡ�Ĵ�����Ϣ:
        hipriv "Hisilicon0 regwrite 32/16(51û��16λд�Ĵ�������) regtype(soc/mac/phy) addr val" addr����16����0x��ͷ
 */
    { "regwrite", wal_hipriv_reg_write },
    /* ��ӡ���еĽ���������, hipriv "Hisilicon0 dump_all_dscr" */
    { "dump_all_dscr", wal_hipriv_dump_all_rx_dscr },
    /* �������mac addrɨ�迪�أ�sh hipriv.sh "Hisilicon0 random_mac_addr_scan 0|1(��|�ر�)" */
    { "random_mac_addr_scan", wal_hipriv_set_random_mac_addr_scan },
    /*
        ɨ��ֹͣ�������� hipriv "Hisilicon0 bgscan_enable param1" param1ȡֵ'0' '1',
        ��Ӧ�رպʹ򿪱���ɨ��, '2' ��ʾ�ر�����ɨ��
 */
    { "bgscan_enable", wal_hipriv_bgscan_enable },
    /* ����20/40����ʹ��: hipriv "vap0 2040_coexistence 0|1" 0��ʾ20/40MHz����ʹ�ܣ�1��ʾ20/40MHz���治ʹ�� */
    { "2040_coexistence", wal_hipriv_set_2040_coext_support },
#ifdef _PRE_WLAN_FEATURE_BTCOEX
    /* ��ӡ����ά����Ϣ��sh hipriv.sh "coex_print" */
    { "coex_print", wal_hipriv_btcoex_status_print },
#endif
#ifdef _PRE_WLAN_FEATURE_STA_PM
    /* ����PSPOLL���� sh hipriv.sh 'wlan0 set_ps_mode 3 0' */
    { "set_ps_mode", wal_hipriv_sta_ps_mode },
#ifdef _PRE_PSM_DEBUG_MODE
    /* sta psm��ά��ͳ����Ϣ sh hipriv.sh 'wlan0 psm_info_debug 1' */
    { "psm_info_debug", wal_hipriv_sta_ps_info },
#endif
#endif
#ifdef _PRE_WLAN_FEATURE_UAPSD
    /* hipriv "vap0 uapsd_en_cap 0\1" */
    { "uapsd_en_cap", wal_hipriv_set_uapsd_cap },
#endif
#ifdef _PRE_WLAN_FEATURE_STA_UAPSD
    /* ����uapsd�Ĳ�����Ϣ sh hipriv.sh 'wlan0 set_uapsd_para 3 1 1 1 1 */
    { "set_uapsd_para", wal_hipriv_set_uapsd_para },
#endif
    /* ����vap˽������Ϊ: hipriv "Hisilicon0 create vap0 ap|sta" */
    { "create", wal_hipriv_add_vap },
#ifdef _PRE_WLAN_FEATURE_ALWAYS_TX
    /*
        ���ó���ģʽ:hipriv "vap0 al_tx <value: 0/1/2>  <len>" ����mac���ƣ�11a,b,g��ֻ֧��4095�������ݷ���,
        ����ʹ��set_mcast_data�����ʽ�������
 */
    { "al_tx_1102", wal_hipriv_always_tx_1102 },
    /* ���ó�����Ŀ:               hipriv "vap0 al_tx_num <value>" */
    { "al_tx_num", wal_hipriv_always_tx_num },
#endif
    /* ���ó���ģʽ:               hipriv "vap0 al_rx <value: 0/1/2>" */
    { "al_rx", wal_hipriv_always_rx },
    /* ����non-HTģʽ�µ�����:     hipriv "vap0 rate  <value>" */
    { "rate", wal_hipriv_set_rate },
    /* ����HTģʽ�µ�����:         hipriv "vap0 mcs   <value>" */
    { "mcs", wal_hipriv_set_mcs },
    /* ����VHTģʽ�µ�����:        hipriv "vap0 mcsac <value>" */
    { "mcsac", wal_hipriv_set_mcsac },
    /* ����AP �ŵ� */
    { "freq", wal_hipriv_set_freq },
    /* ����AP Э��ģʽ */
    { "mode", wal_hipriv_set_mode },
    /* ���ô���:                   hipriv "vap0 bw    <value>" */
    { "bw", wal_hipriv_set_bw },
    /* ��ӡ��������Ϣ: hipriv "vap0 set_mcast_data <param name> <value>" */
    { "set_mcast_data", wal_hipriv_set_mcast_data_dscr_param },
    /* ��ӡ����֡��FCS��ȷ�������Ϣ:hipriv "vap0 rx_fcs_info 0/1" 0/1  0���������1������� */
    { "rx_fcs_info", wal_hipriv_rx_fcs_info },
    /* ���ù���������͹��ʣ�hipriv "Hisilicon0 set_regdomain_pwr 20",��λdBm */
    { "set_regdomain_pwr", wal_hipriv_set_regdomain_pwr },
    /* ��������û�����������: hipriv "vap0 add_user xx xx xx xx xx xx(mac��ַ) 0 | 1(HT����λ)"���������ĳһ��VAP */
    { "add_user", wal_hipriv_add_user },
    /* ����ɾ���û�����������: hipriv "vap0 del_user xx xx xx xx xx xx(mac��ַ)" ���������ĳһ��VAP */
    { "del_user", wal_hipriv_del_user },
    /* �㷨��������: hipriv "vap0 alg_cfg sch_vi_limit 10" */
    { "alg_cfg", wal_hipriv_alg_cfg },
#ifdef _PRE_WLAN_FEATURE_EDCA_OPT_AP
    /* STA�Ƿ���˽��edca�����Ż����� */
    { "set_edca_switch_sta", wal_hipriv_set_edca_opt_switch_sta },
#endif
    { "alg_tpc_log", wal_hipriv_tpc_log }, /* tpc�㷨��־��������: */
#if (defined(_PRE_PRODUCT_ID_HI110X_DEV) || defined(_PRE_PRODUCT_ID_HI110X_HOST))
    { "sdio_flowctrl", wal_hipriv_sdio_flowctrl },
#endif
#ifdef _PRE_WLAN_FEATURE_TCP_ACK_BUFFER
    { "tcp_ack_buf", wal_hipriv_tcp_ack_buf_cfg }, /* tcp ack buf */
#endif
    { "ps_params", wal_hipriv_ps_params_cfg }, /* ps_params host�·��͹��Ĳ��� */
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    /* wfaʹ�ã��̶�ָ��AC��aifsnֵ, sh hipriv.sh "Hisilicon0 aifsn_cfg 0|1(�ָ�|����) 0|1|2|3(be-vo) val" */
    { "aifsn_cfg", wal_hipriv_aifsn_cfg },
    /* wfaʹ�ã��̶�ָ��AC��cwmaxminֵ, sh hipriv.sh "Hisilicon0 cw_cfg 0|1(�ָ�|����) 0|1|2|3(be-vo) val" */
    { "cw_cfg", wal_hipriv_cw_cfg },
#endif
#ifdef _PRE_WLAN_FEATURE_11K
    { "send_neighbor_req", wal_hipriv_send_neighbor_req }, /* sh hipriv.sh "wlan0 send_neighbor_req WiFi1" */
    /* sh hipriv.sh "wlan0 beacon_req_table_switch 0/1" */
    { "beacon_req_table_switch", wal_hipriv_beacon_req_table_switch },
#endif
    /*
        VOE����ʹ�ܿ��ƣ�Ĭ�Ϲر� sh hipriv.sh "wlan0 voe_enable 0/1"
        (Bit0:11r  Bit1:11V Bit2:11K Bit3:�Ƿ�ǿ�ư���IE70(voe ��֤��Ҫ),
        Bit4:11r��֤,B5-B6:11k auth operating class  Bit7:��ȡ��Ϣ)
 */
    { "voe_enable", wal_hipriv_voe_enable },
    /* VAP������־���� hipriv "VAPX log_level {1|2}"  Warning��Error������־��VAPΪά�� */
    { "log_level", wal_hipriv_vap_log_level },
#ifdef _PRE_WLAN_FEATURE_APF
    { "apf_filter_list", wal_hipriv_apf_filter_list },
#endif
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    { "pm_debug", wal_hipriv_set_pm_debug_switch }, /* �͹���debug���� */
#endif
    { "mcs_check_enable",  wal_hipriv_mcs_set_check_enable },
    { "set_sar_level",     wal_hipriv_set_sar_level },
    { "set_rf_ctl_enable", wal_hipriv_set_pow_rf_ctl },
    { "set_tx_ba_policy",  wal_hipriv_set_tx_ba_policy },
#ifdef _PRE_WLAN_FEATURE_BTCOEX
    { "set_btcoex_params", wal_hipriv_set_btcoex_params },
#endif
    /* ���÷��͹��ʲ���: hipriv "vap0 set_tx_pow <param name> <value>" */
    { "set_tx_pow", wal_hipriv_set_tx_pow_param },

#ifdef _PRE_BT_FITTING_DATA_COLLECT
    { "bt_init_env",   wal_hipriv_init_bt_env },
    { "bt_freq",   wal_hipriv_set_bt_freq },
    { "bt_upc_by_freq", wal_hipriv_set_bt_upc_by_freq },
    { "bt_print_gm",   wal_hipriv_print_bt_gm },
#endif
    /* ����host��rx�������Ż���������: "wlan0 dscr_th_opt <queue id> <start_th> <interval>" */
    { "dscr_th_opt", wal_hipriv_dscr_th_opt },
    /* ����tcp ack������ز���: "wlan0 tcp_ack_filter <0|1>(off/on) */
    {"tcp_ack_filter",           wal_hipriv_tcp_ack_filter},
#ifdef _PRE_WLAN_FEATURE_NRCOEX
    {"nrcoex_cmd",               wal_hipriv_nrcoex_cmd},   /* ����NRCOEX����:hipriv.sh "wlan0 nrcoex_cmd xxx xxx" */
    {"nrcoex_test",              wal_hipriv_nrcoex_test_cmd}  /* ��03���������һ��,NRCOEX�¼��·����� */
#endif
};

/*****************************************************************************
  net_device�Ϲҽӵ�net_device_ops����
*****************************************************************************/
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
oal_net_device_ops_stru g_st_wal_net_dev_ops = {
    .ndo_get_stats          = wal_netdev_get_stats,
    .ndo_open               = wal_netdev_open,
    .ndo_stop               = wal_netdev_stop,
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC != _PRE_MULTI_CORE_MODE)
    .ndo_start_xmit         = wal_vap_start_xmit,
#else
    .ndo_start_xmit         = wal_bridge_vap_xmit,
#endif
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,44))
#else
    .ndo_set_multicast_list = OAL_PTR_NULL,
#endif

    .ndo_do_ioctl           = wal_net_device_ioctl,
    .ndo_change_mtu         = oal_net_device_change_mtu,
    .ndo_init               = oal_net_device_init,

#if defined(_PRE_WLAN_FEATURE_OFFLOAD_FLOWCTL)
    .ndo_select_queue       = wal_netdev_select_queue,
#endif

    .ndo_set_mac_address    = wal_netdev_set_mac_addr
};
#elif (_PRE_OS_VERSION_WIN32 == _PRE_OS_VERSION)
oal_net_device_ops_stru g_st_wal_net_dev_ops = {
    oal_net_device_init,
    wal_netdev_open,
    wal_netdev_stop,
    wal_bridge_vap_xmit,
    OAL_PTR_NULL,
    wal_netdev_get_stats,
    wal_net_device_ioctl,
    oal_net_device_change_mtu,
    wal_netdev_set_mac_addr
};
#endif

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE) && (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)

oal_ethtool_ops_stru g_st_wal_ethtool_ops = { 0 };
#endif

/*****************************************************************************
  ��׼ioctl�������.
*****************************************************************************/
OAL_STATIC OAL_CONST oal_iw_handler g_ast_iw_handlers[] = {
    OAL_PTR_NULL,                               /* SIOCSIWCOMMIT, */
    (oal_iw_handler)wal_ioctl_get_iwname,       /* SIOCGIWNAME, */
    OAL_PTR_NULL,                               /* SIOCSIWNWID, */
    OAL_PTR_NULL,                               /* SIOCGIWNWID, */
    OAL_PTR_NULL,                               /* SIOCSIWFREQ, ����Ƶ��/�ŵ� */
    OAL_PTR_NULL,                               /* SIOCGIWFREQ, ��ȡƵ��/�ŵ� */
    OAL_PTR_NULL,                               /* SIOCSIWMODE, ����bss type */
    (oal_iw_handler)wal_ioctl_get_mode,         /* SIOCGIWMODE, ��ȡbss type */
    OAL_PTR_NULL,                               /* SIOCSIWSENS, */
    OAL_PTR_NULL,                               /* SIOCGIWSENS, */
    OAL_PTR_NULL,                               /* SIOCSIWRANGE, */ /* not used */
    OAL_PTR_NULL,                               /* SIOCGIWRANGE, */
    OAL_PTR_NULL,                               /* SIOCSIWPRIV, */  /* not used */
    OAL_PTR_NULL,                               /* SIOCGIWPRIV, */  /* kernel code */
    OAL_PTR_NULL,                               /* SIOCSIWSTATS, */ /* not used */
    OAL_PTR_NULL,                               /* SIOCGIWSTATS, */
    OAL_PTR_NULL,                               /* SIOCSIWSPY, */
    OAL_PTR_NULL,                               /* SIOCGIWSPY, */
    OAL_PTR_NULL,                               /* -- hole -- */
    OAL_PTR_NULL,                               /* -- hole -- */
    OAL_PTR_NULL,                               /* SIOCSIWAP, */
    (oal_iw_handler)wal_ioctl_get_apaddr,       /* SIOCGIWAP, */
    OAL_PTR_NULL,                               /* SIOCSIWMLME, */
    OAL_PTR_NULL,                               /* SIOCGIWAPLIST, */
    OAL_PTR_NULL,                               /* SIOCSIWSCAN, */
    OAL_PTR_NULL,                               /* SIOCGIWSCAN, */
    OAL_PTR_NULL,                               /* SIOCSIWESSID, ����ssid */
    (oal_iw_handler)wal_ioctl_get_essid,        /* SIOCGIWESSID, ��ȡssid */
    OAL_PTR_NULL,                               /* SIOCSIWNICKN */
    OAL_PTR_NULL,                               /* SIOCGIWNICKN */
    OAL_PTR_NULL,                               /* -- hole -- */
    OAL_PTR_NULL,                               /* -- hole -- */
    OAL_PTR_NULL,                               /* SIOCSIWRATE */
    OAL_PTR_NULL,                               /* SIOCGIWRATE  get_iwrate */
    OAL_PTR_NULL,                               /* SIOCSIWRTS */
    OAL_PTR_NULL,                               /* SIOCGIWRTS  get_rtsthres */
    OAL_PTR_NULL,                               /* SIOCSIWFRAG */
    OAL_PTR_NULL,                               /* SIOCGIWFRAG  get_fragthres */
    OAL_PTR_NULL,                               /* SIOCSIWTXPOW, ���ô��书������ */
    OAL_PTR_NULL,                               /* SIOCGIWTXPOW, ���ô��书������ */
    OAL_PTR_NULL,                               /* SIOCSIWRETRY */
    OAL_PTR_NULL,                               /* SIOCGIWRETRY */
    OAL_PTR_NULL,                               /* SIOCSIWENCODE */
    OAL_PTR_NULL,                               /* SIOCGIWENCODE  get_iwencode */
    OAL_PTR_NULL,                               /* SIOCSIWPOWER */
    OAL_PTR_NULL,                               /* SIOCGIWPOWER */
    OAL_PTR_NULL,                               /* -- hole -- */
    OAL_PTR_NULL,                               /* -- hole -- */
    OAL_PTR_NULL,                               /* SIOCSIWGENIE */
    OAL_PTR_NULL,                               /* SIOCGIWGENIE */
    OAL_PTR_NULL,                               /* SIOCSIWAUTH */
    OAL_PTR_NULL,                               /* SIOCGIWAUTH */
    OAL_PTR_NULL,                               /* SIOCSIWENCODEEXT */
    OAL_PTR_NULL                                /* SIOCGIWENCODEEXT */
};

/*****************************************************************************
  ˽��ioctl����������嶨��
*****************************************************************************/
OAL_STATIC OAL_CONST oal_iw_priv_args_stru g_ast_iw_priv_args[] = {
    /* OAL_IW_PRIV_TYPE_CHAR | 256��ʾ����������������ͼ�������Ϣ */
    {WAL_IOCTL_PRIV_SET_AP_CFG, OAL_IW_PRIV_TYPE_CHAR |  256, 0, "AP_SET_CFG"},
    /* OAL_IW_PRIV_TYPE_CHAR | 256��ʾ����������������ͼ�������Ϣ */
    {WAL_IOCTL_PRIV_AP_MAC_FLTR, OAL_IW_PRIV_TYPE_CHAR | 256, 
     OAL_IW_PRIV_TYPE_CHAR | OAL_IW_PRIV_SIZE_FIXED | 0, "AP_SET_MAC_FLTR"},
    {WAL_IOCTL_PRIV_AP_GET_STA_LIST, 0, OAL_IW_PRIV_TYPE_CHAR | OAL_AP_GET_ASSOC_LIST_LEN, "AP_GET_STA_LIST"},
    /* OAL_IW_PRIV_TYPE_CHAR | 256��ʾ����������������ͼ�������Ϣ */
    {WAL_IOCTL_PRIV_AP_STA_DISASSOC, OAL_IW_PRIV_TYPE_CHAR | 256, OAL_IW_PRIV_TYPE_CHAR | 0, "AP_STA_DISASSOC"},
};

/*****************************************************************************
  ˽��ioctl�������.
*****************************************************************************/
OAL_STATIC OAL_CONST oal_iw_handler g_ast_iw_priv_handlers[] = {
    OAL_PTR_NULL,                                       /* SIOCWFIRSTPRIV+0 */  /* sub-ioctl set ��� */
    OAL_PTR_NULL,                                       /* SIOCWFIRSTPRIV+1 */  /* sub-ioctl get ��� */
    OAL_PTR_NULL,                                       /* SIOCWFIRSTPRIV+2 */  /* setkey */
    OAL_PTR_NULL,                                       /* SIOCWFIRSTPRIV+3 */  /* setwmmparams */
    OAL_PTR_NULL,                                       /* SIOCWFIRSTPRIV+4 */  /* delkey */
    OAL_PTR_NULL,                                       /* SIOCWFIRSTPRIV+5 */  /* getwmmparams */
    OAL_PTR_NULL,                                       /* SIOCWFIRSTPRIV+6 */  /* setmlme */
    OAL_PTR_NULL,                                       /* SIOCWFIRSTPRIV+7 */  /* getchaninfo */
    OAL_PTR_NULL,                                       /* SIOCWFIRSTPRIV+8 */  /* setcountry */
    OAL_PTR_NULL,                                       /* SIOCWFIRSTPRIV+9 */  /* getcountry */
    OAL_PTR_NULL,                                       /* SIOCWFIRSTPRIV+10 */  /* addmac */
    OAL_PTR_NULL,                                       /* SIOCWFIRSTPRIV+11 */  /* getscanresults */
    OAL_PTR_NULL,                                       /* SIOCWFIRSTPRIV+12 */  /* delmac */
    OAL_PTR_NULL,                                       /* SIOCWFIRSTPRIV+13 */  /* getchanlist */
    OAL_PTR_NULL,                                       /* SIOCWFIRSTPRIV+14 */  /* setchanlist */
    OAL_PTR_NULL,                                       /* SIOCWFIRSTPRIV+15 */  /* kickmac */
    OAL_PTR_NULL,                                       /* SIOCWFIRSTPRIV+16 */  /* chanswitch */
    OAL_PTR_NULL,                                     /* SIOCWFIRSTPRIV+17 */  /* ��ȡģʽ, ��: iwpriv vapN get_mode */
    OAL_PTR_NULL,                                     /* SIOCWFIRSTPRIV+18 */  /* ����ģʽ, ��: iwpriv vapN mode 11g */
    OAL_PTR_NULL,                                     /* SIOCWFIRSTPRIV+19 */  /* getappiebuf */
    OAL_PTR_NULL,                                     /* SIOCWFIRSTPRIV+20 */  /* null */
    (oal_iw_handler)wal_ioctl_get_assoc_list,         /* SIOCWFIRSTPRIV+21 */  /* APUTȡ�ù���STA�б� */
    (oal_iw_handler)wal_ioctl_set_mac_filters,        /* SIOCWFIRSTPRIV+22 */  /* APUT����STA���� */
    (oal_iw_handler)wal_ioctl_set_ap_config,            /* SIOCWFIRSTPRIV+23 */  /* ����APUT���� */
    (oal_iw_handler)wal_ioctl_set_ap_sta_disassoc,      /* SIOCWFIRSTPRIV+24 */  /* APUTȥ����STA */
    OAL_PTR_NULL,                                       /* SIOCWFIRSTPRIV+25 */  /* getStatistics */
    OAL_PTR_NULL,                                       /* SIOCWFIRSTPRIV+26 */  /* sendmgmt */
    OAL_PTR_NULL,                                       /* SIOCWFIRSTPRIV+27 */  /* null  */
    OAL_PTR_NULL,                                       /* SIOCWFIRSTPRIV+28 */  /* null */
    OAL_PTR_NULL,                                       /* SIOCWFIRSTPRIV+29 */  /* getaclmac */
    OAL_PTR_NULL,                                       /* SIOCWFIRSTPRIV+30 */  /* sethbrparams */
    OAL_PTR_NULL,                                       /* SIOCWFIRSTPRIV+29 */  /* getaclmac */
    OAL_PTR_NULL,                                       /* SIOCWFIRSTPRIV+30 */  /* sethbrparams */
    OAL_PTR_NULL,                                       /* SIOCWFIRSTPRIV+31 */  /* setrxtimeout */
};

/*****************************************************************************
  ��������iw_handler_def����
*****************************************************************************/
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
oal_iw_handler_def_stru g_st_iw_handler_def = {
    .standard           = g_ast_iw_handlers,
    .num_standard       = OAL_ARRAY_SIZE(g_ast_iw_handlers),
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,59))
#ifdef CONFIG_WEXT_PRIV
    .private            = g_ast_iw_priv_handlers,
    .num_private        = OAL_ARRAY_SIZE(g_ast_iw_priv_handlers),
    .private_args       = g_ast_iw_priv_args,
    .num_private_args   = OAL_ARRAY_SIZE(g_ast_iw_priv_args),
#endif
#else
    .private            = g_ast_iw_priv_handlers,
    .num_private        = OAL_ARRAY_SIZE(g_ast_iw_priv_handlers),
    .private_args       = g_ast_iw_priv_args,
    .num_private_args   = OAL_ARRAY_SIZE(g_ast_iw_priv_args),
#endif
    .get_wireless_stats = OAL_PTR_NULL
};

#elif (_PRE_OS_VERSION_WIN32 == _PRE_OS_VERSION)
oal_iw_handler_def_stru g_st_iw_handler_def = {
    OAL_PTR_NULL,                      /* ��׼ioctl handler */
    0,
    OAL_ARRAY_SIZE(g_ast_iw_priv_handlers),
    {0, 0},                                 /* �ֽڶ��� */
    OAL_ARRAY_SIZE(g_ast_iw_priv_args),
    g_ast_iw_priv_handlers,                 /* ˽��ioctl handler */
    g_ast_iw_priv_args,
    OAL_PTR_NULL
};
#endif

/*****************************************************************************
  Э��ģʽ�ַ�������
*****************************************************************************/
OAL_CONST wal_ioctl_mode_map_stru g_ast_mode_map[] = {
    /* legacy */
    {"11a",         WLAN_LEGACY_11A_MODE,       WLAN_BAND_5G,   WLAN_BAND_WIDTH_20M},
    {"11b",         WLAN_LEGACY_11B_MODE,       WLAN_BAND_2G,   WLAN_BAND_WIDTH_20M},
    {"11bg",        WLAN_MIXED_ONE_11G_MODE,    WLAN_BAND_2G,   WLAN_BAND_WIDTH_20M},
    {"11g",         WLAN_MIXED_TWO_11G_MODE,    WLAN_BAND_2G,   WLAN_BAND_WIDTH_20M},

    /* 11n */
    {"11na20",      WLAN_HT_MODE,           WLAN_BAND_5G,   WLAN_BAND_WIDTH_20M},
    {"11ng20",      WLAN_HT_MODE,           WLAN_BAND_2G,   WLAN_BAND_WIDTH_20M},
    {"11na40plus",  WLAN_HT_MODE,           WLAN_BAND_5G,   WLAN_BAND_WIDTH_40PLUS},
    {"11na40minus", WLAN_HT_MODE,           WLAN_BAND_5G,   WLAN_BAND_WIDTH_40MINUS},
    {"11ng40plus",  WLAN_HT_MODE,           WLAN_BAND_2G,   WLAN_BAND_WIDTH_40PLUS},
    {"11ng40minus", WLAN_HT_MODE,           WLAN_BAND_2G,   WLAN_BAND_WIDTH_40MINUS},

    /* 11ac */
    {"11ac20",              WLAN_VHT_MODE,  WLAN_BAND_5G,   WLAN_BAND_WIDTH_20M},
    {"11ac40plus",          WLAN_VHT_MODE,  WLAN_BAND_5G,   WLAN_BAND_WIDTH_40PLUS},
    {"11ac40minus",         WLAN_VHT_MODE,  WLAN_BAND_5G,   WLAN_BAND_WIDTH_40MINUS},
    {"11ac80plusplus",      WLAN_VHT_MODE,  WLAN_BAND_5G,   WLAN_BAND_WIDTH_80PLUSPLUS},
    {"11ac80plusminus",     WLAN_VHT_MODE,  WLAN_BAND_5G,   WLAN_BAND_WIDTH_80PLUSMINUS},
    {"11ac80minusplus",     WLAN_VHT_MODE,  WLAN_BAND_5G,   WLAN_BAND_WIDTH_80MINUSPLUS},
    {"11ac80minusminus",    WLAN_VHT_MODE,  WLAN_BAND_5G,   WLAN_BAND_WIDTH_80MINUSMINUS},

    {"11ac2g20",            WLAN_VHT_MODE,  WLAN_BAND_2G,   WLAN_BAND_WIDTH_20M},
    {"11ac2g40plus",        WLAN_VHT_MODE,  WLAN_BAND_2G,   WLAN_BAND_WIDTH_40PLUS},
    {"11ac2g40minus",       WLAN_VHT_MODE,  WLAN_BAND_2G,   WLAN_BAND_WIDTH_40MINUS},
    /* 11n only and 11ac only, ����20M���� */
    {"11nonly2g",           WLAN_HT_ONLY_MODE,   WLAN_BAND_2G,   WLAN_BAND_WIDTH_20M},
    {"11nonly5g",           WLAN_HT_ONLY_MODE,   WLAN_BAND_5G,   WLAN_BAND_WIDTH_20M},
    {"11aconly",            WLAN_VHT_ONLY_MODE,  WLAN_BAND_5G,   WLAN_BAND_WIDTH_20M},

    {OAL_PTR_NULL}
};

OAL_CONST oal_int8   *pauc_tx_pow_param_name[WAL_TX_POW_PARAM_BUTT] = {
    "rf_reg_ctl",
    "mag_level",
    "ctl_level",
    "show_log",
};

/* ע��! ����Ĳ���������Ҫ�� g_dmac_config_set_dscr_param�еĺ���˳���ϸ�һ��! */
OAL_CONST oal_int8   *pauc_tx_dscr_param_name[WAL_DSCR_PARAM_BUTT] = {
    "fbm",
    "pgl",
    "mtpgl",
    "sae",
    "ta",
    "ra",
    "cc",
    "data0",
    "data1",
    "data2",
    "data3",
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
    "ch_bw_exist"
};

OAL_STATIC OAL_CONST oal_int8   *pauc_bw_tbl[WLAN_BAND_ASSEMBLE_AUTO] = {
    "20",
    "rsv1",
    "rsv2",
    "rsv3",
    "40",
    "d40",
    "rsv6",
    "rsv7",
    "80",
    "d80",
    "rsv10",
    "rsv11",
    "160",
    "d160",
    "rsv14",
    "80_80"
};

OAL_STATIC OAL_CONST oal_int8   *pauc_non_ht_rate_tbl[WLAN_LEGACY_RATE_VALUE_BUTT] = {
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
#ifdef _PRE_WLAN_FIT_BASED_REALTIME_CALI
OAL_CONST wal_ioctl_dyn_cali_stru g_ast_dyn_cali_cfg_map[] = {
    {"realtime_cali_adjust",    MAC_DYN_CALI_CFG_SET_EN_REALTIME_CALI_ADJUST},
    {"2g_dscr_interval",        MAC_DYN_CALI_CFG_SET_2G_DSCR_INT},
    {"5g_dscr_interval",        MAC_DYN_CALI_CFG_SET_5G_DSCR_INT},
    {"pdet_min_th",             MAC_DYN_CALI_CFG_SET_PDET_MIN_TH},
    {"pdet_max_th",             MAC_DYN_CALI_CFG_SET_PDET_MAX_TH},
    {OAL_PTR_NULL}
};
#endif

OAL_CONST wal_ioctl_alg_cfg_stru g_ast_alg_cfg_map[] = {
    {"sch_vi_ctrl_ena",         MAC_ALG_CFG_SCHEDULE_VI_CTRL_ENA},
    {"sch_bebk_minbw_ena",      MAC_ALG_CFG_SCHEDULE_BEBK_MIN_BW_ENA},
    {"sch_mvap_sch_ena",        MAC_ALG_CFG_SCHEDULE_MVAP_SCH_ENA},
    {"sch_vi_sch_ms",           MAC_ALG_CFG_SCHEDULE_VI_SCH_LIMIT},
    {"sch_vo_sch_ms",           MAC_ALG_CFG_SCHEDULE_VO_SCH_LIMIT},
    {"sch_vi_drop_ms",          MAC_ALG_CFG_SCHEDULE_VI_DROP_LIMIT},
    {"sch_vi_ctrl_ms",          MAC_ALG_CFG_SCHEDULE_VI_CTRL_MS},
    {"sch_vi_life_ms",          MAC_ALG_CFG_SCHEDULE_VI_MSDU_LIFE_MS},
    {"sch_vo_life_ms",          MAC_ALG_CFG_SCHEDULE_VO_MSDU_LIFE_MS},
    {"sch_be_life_ms",          MAC_ALG_CFG_SCHEDULE_BE_MSDU_LIFE_MS},
    {"sch_bk_life_ms",          MAC_ALG_CFG_SCHEDULE_BK_MSDU_LIFE_MS},
    {"sch_vi_low_delay",        MAC_ALG_CFG_SCHEDULE_VI_LOW_DELAY_MS},
    {"sch_vi_high_delay",       MAC_ALG_CFG_SCHEDULE_VI_HIGH_DELAY_MS},
    {"sch_cycle_ms",            MAC_ALG_CFG_SCHEDULE_SCH_CYCLE_MS},
    {"sch_ctrl_cycle_ms",       MAC_ALG_CFG_SCHEDULE_TRAFFIC_CTRL_CYCLE},
    {"sch_cir_nvip_kbps",       MAC_ALG_CFG_SCHEDULE_CIR_NVIP_KBPS},
    {"sch_cir_nvip_be",         MAC_ALG_CFG_SCHEDULE_CIR_NVIP_KBPS_BE},
    {"sch_cir_nvip_bk",         MAC_ALG_CFG_SCHEDULE_CIR_NVIP_KBPS_BK},
    {"sch_cir_vip_kbps",        MAC_ALG_CFG_SCHEDULE_CIR_VIP_KBPS},
    {"sch_cir_vip_be",          MAC_ALG_CFG_SCHEDULE_CIR_VIP_KBPS_BE},
    {"sch_cir_vip_bk",          MAC_ALG_CFG_SCHEDULE_CIR_VIP_KBPS_BK},
    {"sch_cir_vap_kbps",        MAC_ALG_CFG_SCHEDULE_CIR_VAP_KBPS},
    {"sch_sm_delay_ms",         MAC_ALG_CFG_SCHEDULE_SM_TRAIN_DELAY},
    {"sch_drop_pkt_limit",      MAC_ALG_CFG_VIDEO_DROP_PKT_LIMIT},
    {"sch_flowctl_ena",         MAC_ALG_CFG_FLOWCTRL_ENABLE_FLAG},
    {"sch_log_start",           MAC_ALG_CFG_SCHEDULE_LOG_START},
    {"sch_log_end",             MAC_ALG_CFG_SCHEDULE_LOG_END},
    {"sch_vap_prio",            MAC_ALG_CFG_SCHEDULE_VAP_SCH_PRIO},

    {"txbf_switch",             MAC_ALG_CFG_TXBF_MASTER_SWITCH},
    {"txbf_txmode_enb",         MAC_ALG_CFG_TXBF_TXMODE_ENABLE},
    {"txbf_bfer_enb",           MAC_ALG_CFG_TXBF_TXBFER_ENABLE},
    {"txbf_bfee_enb",           MAC_ALG_CFG_TXBF_TXBFEE_ENABLE},
    {"txbf_11nbfee_enb",        MAC_ALG_CFG_TXBF_11N_BFEE_ENABLE},
    {"txbf_txstbc_enb",         MAC_ALG_CFG_TXBF_TXSTBC_ENABLE},
    {"txbf_rxstbc_enb",         MAC_ALG_CFG_TXBF_RXSTBC_ENABLE},
    {"txbf_2g_bfer",            MAC_ALG_CFG_TXBF_2G_BFER_ENABLE},
    {"txbf_2nss_bfer",          MAC_ALG_CFG_TXBF_2NSS_BFER_ENABLE},
    {"txbf_fix_mode",           MAC_ALG_CFG_TXBF_FIX_MODE},
    {"txbf_fix_sound",          MAC_ALG_CFG_TXBF_FIX_SOUNDING},
    {"txbf_probe_int",          MAC_ALG_CFG_TXBF_PROBE_INT},
    {"txbf_rm_worst",           MAC_ALG_CFG_TXBF_REMOVE_WORST},
    {"txbf_stbl_num",           MAC_ALG_CFG_TXBF_STABLE_NUM},
    {"txbf_probe_cnt",          MAC_ALG_CFG_TXBF_PROBE_COUNT},
    {"txbf_log_enb",            MAC_ALG_CFG_TXBF_LOG_ENABLE},
    {"txbf_log_sta",            MAC_ALG_CFG_TXBF_RECORD_LOG_START},
    {"txbf_log_out",            MAC_ALG_CFG_TXBF_LOG_OUTPUT},
    /* ������ر���������Ӧ�㷨: sh hipriv.sh "vap0 alg_cfg ar_enable [1|0]" */
    {"ar_enable",               MAC_ALG_CFG_AUTORATE_ENABLE},
    /* ������ر�ʹ���������: sh hipriv.sh "vap0 alg_cfg ar_use_lowest [1|0]" */
    {"ar_use_lowest",           MAC_ALG_CFG_AUTORATE_USE_LOWEST_RATE},
    /* ���ö���ͳ�Ƶİ���Ŀ:sh hipriv.sh "vap0 alg_cfg ar_short_num [����Ŀ]" */
    {"ar_short_num",            MAC_ALG_CFG_AUTORATE_SHORT_STAT_NUM},
    /* ���ö���ͳ�Ƶİ�λ��ֵ:sh hipriv.sh "vap0 alg_cfg ar_short_shift [λ��ֵ]" */
    {"ar_short_shift",          MAC_ALG_CFG_AUTORATE_SHORT_STAT_SHIFT},
    /* ���ó���ͳ�Ƶİ���Ŀ:sh hipriv.sh "vap0 alg_cfg ar_long_num [����Ŀ]" */
    {"ar_long_num",             MAC_ALG_CFG_AUTORATE_LONG_STAT_NUM},
    /* ���ó���ͳ�Ƶİ�λ��ֵ:sh hipriv.sh "vap0 alg_cfg ar_long_shift [λ��ֵ]" */
    {"ar_long_shift",           MAC_ALG_CFG_AUTORATE_LONG_STAT_SHIFT},
    /* ������С����̽������:sh hipriv.sh "vap0 alg_cfg ar_min_probe_no_up [����Ŀ]" */
    {"ar_min_probe_no_up",      MAC_ALG_CFG_AUTORATE_MIN_PROBE_UP_INTVL_PKTNUM},
    /* ������С����̽������:sh hipriv.sh "vap0 alg_cfg ar_min_probe_no_down [����Ŀ]" */
    {"ar_min_probe_no_down",    MAC_ALG_CFG_AUTORATE_MIN_PROBE_DOWN_INTVL_PKTNUM},
    /* �������̽������:sh hipriv.sh "vap0 alg_cfg ar_max_probe_no [����Ŀ]" */
    {"ar_max_probe_no",         MAC_ALG_CFG_AUTORATE_MAX_PROBE_INTVL_PKTNUM},
    /* ����̽�������ִ���:sh hipriv.sh "vap0 alg_cfg ar_keep_times [����]" */
    {"ar_keep_times",           MAC_ALG_CFG_AUTORATE_PROBE_INTVL_KEEP_TIMES},
    /* ����goodputͻ������(ǧ�ֱȣ���300):sh hipriv.sh "vap0 alg_cfg ar_delta_ratio [ǧ�ֱ�]" */
    {"ar_delta_ratio",          MAC_ALG_CFG_AUTORATE_DELTA_GOODPUT_RATIO},
    /* ����vi��per����(ǧ�ֱȣ���300):sh hipriv.sh "vap0 alg_cfg ar_vi_per_limit [ǧ�ֱ�]" */
    {"ar_vi_per_limit",         MAC_ALG_CFG_AUTORATE_VI_PROBE_PER_LIMIT},
    /* ����vo��per����(ǧ�ֱȣ���300):sh hipriv.sh "vap0 alg_cfg ar_vo_per_limit [ǧ�ֱ�]" */
    {"ar_vo_per_limit",         MAC_ALG_CFG_AUTORATE_VO_PROBE_PER_LIMIT},
    /* ����ampdu��durattionֵ:sh hipriv.sh "vap0 alg_cfg ar_ampdu_time [ʱ��ֵ]" */
    {"ar_ampdu_time",           MAC_ALG_CFG_AUTORATE_AMPDU_DURATION},
    /* ����mcs0�Ĵ���ʧ������:sh hipriv.sh "vap0 alg_cfg ar_cont_loss_num [����Ŀ]" */
    {"ar_cont_loss_num",        MAC_ALG_CFG_AUTORATE_MCS0_CONT_LOSS_NUM},
    /* ��������11b��rssi����:sh hipriv.sh "vap0 alg_cfg ar_11b_diff_rssi [��ֵ]" */
    {"ar_11b_diff_rssi",        MAC_ALG_CFG_AUTORATE_UP_PROTOCOL_DIFF_RSSI},
    /*
        ����rtsģʽ:sh hipriv.sh "vap0 alg_cfg ar_rts_mode [0(������)|1(����)|
        2(rate[0]��̬RTS, rate[1..3]����RTS)|3(rate[0]����RTS, rate[1..3]����RTS)]"
    */
    {"ar_rts_mode",             MAC_ALG_CFG_AUTORATE_RTS_MODE},
    /* ����Legacy�װ�����������:sh hipriv.sh "vap0 alg_cfg ar_legacy_loss [��ֵ]" */
    {"ar_legacy_loss",          MAC_ALG_CFG_AUTORATE_LEGACY_1ST_LOSS_RATIO_TH},
    /* ����Legacy�װ�����������:sh hipriv.sh "vap0 alg_cfg ar_ht_vht_loss [��ֵ]" */
    {"ar_ht_vht_loss",          MAC_ALG_CFG_AUTORATE_HT_VHT_1ST_LOSS_RATIO_TH},
    /*
        ��ʼ����ͳ����־:sh hipriv.sh "vap0 alg_cfg ar_stat_log_do [mac��ַ] [ҵ�����] [����Ŀ]"
        ��: sh hipriv.sh "vap0 alg_cfg ar_stat_log_do 06:31:04:E3:81:02 1 1000"
    */
    {"ar_stat_log_do",          MAC_ALG_CFG_AUTORATE_STAT_LOG_START},
    /*
        ��ʼ����ѡ����־:sh hipriv.sh "vap0 alg_cfg ar_sel_log_do [mac��ַ] [ҵ�����] [����Ŀ]"
        ��: sh hipriv.sh "vap0 alg_cfg ar_sel_log_do 06:31:04:E3:81:02 1 200"
    */
    {"ar_sel_log_do",           MAC_ALG_CFG_AUTORATE_SELECTION_LOG_START},
    /*
        ��ʼ�̶�������־:sh hipriv.sh "vap0 alg_cfg ar_fix_log_do [mac��ַ] [tidno] [per����]"
        ��: sh hipriv.sh "vap0 alg_cfg ar_sel_log_do 06:31:04:E3:81:02 1 200"
    */
    {"ar_fix_log_do",           MAC_ALG_CFG_AUTORATE_FIX_RATE_LOG_START},
    /*
        ��ʼ�ۺ�����Ӧ��־:sh hipriv.sh "vap0 alg_cfg ar_fix_log_do [mac��ַ] [tidno]"
        ��: sh hipriv.sh "vap0 alg_cfg ar_sel_log_do 06:31:04:E3:81:02 1 "
    */
    {"ar_aggr_log_do",          MAC_ALG_CFG_AUTORATE_AGGR_STAT_LOG_START},
    /* ��ӡ����ͳ����־:sh hipriv.sh "vap0 alg_cfg ar_st_log_out 06:31:04:E3:81:02" */
    {"ar_st_log_out",           MAC_ALG_CFG_AUTORATE_STAT_LOG_WRITE},
    /* ��ӡ����ѡ����־:sh hipriv.sh "vap0 alg_cfg ar_sel_log_out 06:31:04:E3:81:02" */
    {"ar_sel_log_out",          MAC_ALG_CFG_AUTORATE_SELECTION_LOG_WRITE},
    /* ��ӡ�̶�������־:sh hipriv.sh "vap0 alg_cfg ar_fix_log_out 06:31:04:E3:81:02" */
    {"ar_fix_log_out",          MAC_ALG_CFG_AUTORATE_FIX_RATE_LOG_WRITE},
    /* ��ӡ�̶�������־:sh hipriv.sh "vap0 alg_cfg ar_fix_log_out 06:31:04:E3:81:02" */
    {"ar_aggr_log_out",         MAC_ALG_CFG_AUTORATE_AGGR_STAT_LOG_WRITE},
    /* ��ӡ���ʼ���:sh hipriv.sh "vap0 alg_cfg ar_disp_rateset 06:31:04:E3:81:02" */
    {"ar_disp_rateset",         MAC_ALG_CFG_AUTORATE_DISPLAY_RATE_SET},
    /* ���ù̶�����:sh hipriv.sh "vap0 alg_cfg ar_cfg_fix_rate 06:31:04:E3:81:02 0" */
    {"ar_cfg_fix_rate",         MAC_ALG_CFG_AUTORATE_CONFIG_FIX_RATE},
    /* ��ӡ�������ʼ���:sh hipriv.sh "vap0 alg_cfg ar_disp_rx_rate 06:31:04:E3:81:02" */
    {"ar_disp_rx_rate",         MAC_ALG_CFG_AUTORATE_DISPLAY_RX_RATE},
    /* ������ر���������Ӧ��־: sh hipriv.sh "vap0 alg_cfg ar_log_enable [1|0]" */
    {"ar_log_enable",           MAC_ALG_CFG_AUTORATE_LOG_ENABLE},
    /* ��������VO����: sh hipriv.sh "vap0 alg_cfg ar_max_vo_rate [����ֵ]" */
    {"ar_max_vo_rate",          MAC_ALG_CFG_AUTORATE_VO_RATE_LIMIT},
    /* ������˥����per����ֵ: sh hipriv.sh "vap0 alg_cfg ar_fading_per_th [per����ֵ(ǧ����)]" */
    {"ar_fading_per_th",        MAC_ALG_CFG_AUTORATE_JUDGE_FADING_PER_TH},
    /* ���þۺ�����Ӧ����: sh hipriv.sh "vap0 alg_cfg ar_aggr_opt [1|0]" */
    {"ar_aggr_opt",             MAC_ALG_CFG_AUTORATE_AGGR_OPT},
    /* ���þۺ�����Ӧ̽����: sh hipriv.sh "vap0 alg_cfg ar_aggr_pb_intvl [̽����]" */
    {"ar_aggr_pb_intvl",        MAC_ALG_CFG_AUTORATE_AGGR_PROBE_INTVL_NUM},
    /* ���þۺ�����Ӧͳ����λֵ: sh hipriv.sh "vap0 alg_cfg ar_aggr_st_shift [ͳ����λֵ]" */
    {"ar_aggr_st_shift",        MAC_ALG_CFG_AUTORATE_AGGR_STAT_SHIFT},
    /* ����DBACģʽ�µ����ۺ�ʱ��: sh hipriv.sh "vap0 alg_cfg ar_dbac_aggrtime [���ۺ�ʱ��(us)]" */
    {"ar_dbac_aggrtime",        MAC_ALG_CFG_AUTORATE_DBAC_AGGR_TIME},
    /* ���õ����õ�VI״̬: sh hipriv.sh "vap0 alg_cfg ar_dbg_vi_status [0/1/2]" */
    {"ar_dbg_vi_status",        MAC_ALG_CFG_AUTORATE_DBG_VI_STATUS},
    /* �ۺ�����Ӧlog����: sh hipriv.sh "vap0 alg_cfg ar_dbg_aggr_log [0/1]" */
    {"ar_dbg_aggr_log",         MAC_ALG_CFG_AUTORATE_DBG_AGGR_LOG},
    /* �������ʱ仯ʱ�����оۺ�̽��ı�����: sh hipriv.sh "vap0 alg_cfg ar_aggr_pck_num [������]" */
    {"ar_aggr_pck_num",         MAC_ALG_CFG_AUTORATE_AGGR_NON_PROBE_PCK_NUM},
    /* ��С�ۺ�ʱ������: sh hipriv.sh "vap0 alg_cfg ar_aggr_min_idx [����ֵ]" */
    {"ar_min_aggr_idx",         MAC_ALG_CFG_AUTORATE_AGGR_MIN_AGGR_TIME_IDX},
    /* �������ۺ���Ŀ: sh hipriv.sh "vap0 alg_cfg ar_max_aggr_num [�ۺ���Ŀ]" */
    {"ar_max_aggr_num",         MAC_ALG_CFG_AUTORATE_MAX_AGGR_NUM},
    /* ������ͽ�MCS���ƾۺ�Ϊ1��PER����: sh hipriv.sh "vap0 alg_cfg ar_1mpdu_per_th [per����ֵ(ǧ����)]" */
    {"ar_1mpdu_per_th",         MAC_ALG_CFG_AUTORATE_LIMIT_1MPDU_PER_TH},
    /* ������رչ���̽�����: sh hipriv.sh "vap0 alg_cfg ar_btcoxe_probe [1|0]" */
    {"ar_btcoxe_probe",         MAC_ALG_CFG_AUTORATE_BTCOEX_PROBE_ENABLE},
    /* ������رչ���ۺϻ���: sh hipriv.sh "vap0 alg_cfg ar_btcoxe_aggr [1|0]" */
    {"ar_btcoxe_aggr",          MAC_ALG_CFG_AUTORATE_BTCOEX_AGGR_ENABLE},
    /* ���ù���ͳ��ʱ��������: sh hipriv.sh "vap0 alg_cfg ar_coxe_intvl [ͳ������ms]" */
    {"ar_coxe_intvl",           MAC_ALG_CFG_AUTORATE_COEX_STAT_INTVL},
    /* ���ù���abort�ͱ������޲���: sh hipriv.sh "vap0 alg_cfg ar_coxe_low_th [ǧ����]" */
    {"ar_coxe_low_th",          MAC_ALG_CFG_AUTORATE_COEX_LOW_ABORT_TH},
    /* ���ù���abort�߱������޲���: sh hipriv.sh "vap0 alg_cfg ar_coxe_high_th [ǧ����]" */
    {"ar_coxe_high_th",         MAC_ALG_CFG_AUTORATE_COEX_HIGH_ABORT_TH},
    /* ���ù���ۺ���ĿΪ1�����޲���: sh hipriv.sh "vap0 alg_cfg ar_coxe_agrr_th [ǧ����]" */
    {"ar_coxe_agrr_th",         MAC_ALG_CFG_AUTORATE_COEX_AGRR_NUM_ONE_TH},
    /* ��̬��������ʹ�ܿ���: sh hipriv.sh "vap0 alg_cfg ar_dyn_bw_en [0/1]" */
    {"ar_dyn_bw_en",            MAC_ALG_CFG_AUTORATE_DYNAMIC_BW_ENABLE},
    /* �����������Ż�����: sh hipriv.sh "vap0 alg_cfg ar_thpt_wave_opt [0/1]" */
    {"ar_thpt_wave_opt",        MAC_ALG_CFG_AUTORATE_THRPT_WAVE_OPT},
    /*
        �����ж�������������goodput�����������(ǧ����):
        sh hipriv.sh "vap0 alg_cfg ar_gdpt_diff_th [goodput����������(ǧ����)]"
    */
    {"ar_gdpt_diff_th",         MAC_ALG_CFG_AUTORATE_GOODPUT_DIFF_TH},
    /* �����ж�������������PER��������(ǧ����): sh hipriv.sh "vap0 alg_cfg ar_per_worse_th [PER�������(ǧ����)]" */
    {"ar_per_worse_th",         MAC_ALG_CFG_AUTORATE_PER_WORSE_TH},
    /* ���÷�RTS�յ�CTS����DATA������BA�ķ�������жϴ�������: sh hipriv.sh "vap0 alg_cfg ar_cts_no_ba_num [����]" */
    {"ar_cts_no_ack_num",       MAC_ALG_CFG_AUTORATE_RX_CTS_NO_BA_NUM},
    /* �����Ƿ�֧��voiceҵ��ۺ�: sh hipriv.sh "vap0 alg_cfg ar_vo_aggr [0/1]" */
    {"ar_vo_aggr",              MAL_ALG_CFG_AUTORATE_VOICE_AGGR},
    /* ���ÿ���ƽ��ͳ�Ƶ�ƽ������ƫ����:sh hipriv.sh"vap0 alg_cfg ar_fast_smth_shft[ƫ����]"(ȡ255��ʾȡ������ƽ��) */
    {"ar_fast_smth_shft",       MAC_ALG_CFG_AUTORATE_FAST_SMOOTH_SHIFT},
    /* ���ÿ���ƽ��ͳ�Ƶ���С�ۺ���Ŀ����: sh hipriv.sh "vap0 alg_cfg ar_fast_smth_aggr_num [��С�ۺ���Ŀ]" */
    {"ar_fast_smth_aggr_num",   MAC_ALG_CFG_AUTORATE_FAST_SMOOTH_AGGR_NUM},
    /* ����short GI�ͷ���PER����ֵ(ǧ����): sh hipriv.sh "vap0 alg_cfg ar_sgi_punish_per [PER����ֵ(ǧ����)]" */
    {"ar_sgi_punish_per",       MAC_ALG_CFG_AUTORATE_SGI_PUNISH_PER},
    /* ����short GI�ͷ��ĵȴ�̽����Ŀ: sh hipriv.sh "vap0 alg_cfg ar_sgi_punish_num [�ȴ�̽����Ŀ]" */
    {"ar_sgi_punish_num",       MAC_ALG_CFG_AUTORATE_SGI_PUNISH_NUM},

    {"sm_train_num",            MAC_ALG_CFG_SMARTANT_TRAINING_PACKET_NUMBER},
    {"sm_change_ant",           MAC_ALG_CFG_SMARTANT_CHANGE_ANT},
    {"sm_enable",               MAC_ALG_CFG_SMARTANT_ENABLE},
    {"sm_certain_ant",          MAC_ALG_CFG_SMARTANT_CERTAIN_ANT},
    {"sm_start",                MAC_ALG_CFG_SMARTANT_START_TRAIN},
    {"sm_train_packet",         MAC_ALG_CFG_SMARTANT_SET_TRAINING_PACKET_NUMBER},
    {"sm_min_packet",           MAC_ALG_CFG_SMARTANT_SET_LEAST_PACKET_NUMBER},
    {"sm_ant_interval",         MAC_ALG_CFG_SMARTANT_SET_ANT_CHANGE_INTERVAL},
    {"sm_user_interval",        MAC_ALG_CFG_SMARTANT_SET_USER_CHANGE_INTERVAL},
    {"sm_max_period",           MAC_ALG_CFG_SMARTANT_SET_PERIOD_MAX_FACTOR},
    {"sm_change_freq",          MAC_ALG_CFG_SMARTANT_SET_ANT_CHANGE_FREQ},
    {"sm_change_th",            MAC_ALG_CFG_SMARTANT_SET_ANT_CHANGE_THRESHOLD},
    /* ������������non-directʹ��: sh hipriv.sh "vap0 alg_cfg anti_inf_imm_en 0|1" */
    {"anti_inf_imm_en",         MAC_ALG_CFG_ANTI_INTF_IMM_ENABLE},
    /* ������������dynamic unlockʹ��: sh hipriv.sh "vap0 alg_cfg anti_inf_unlock_en 0|1" */
    {"anti_inf_unlock_en",      MAC_ALG_CFG_ANTI_INTF_UNLOCK_ENABLE},
    /* ������������rssiͳ������: sh hipriv.sh "vap0 anti_inf_stat_time [time]" */
    {"anti_inf_stat_time",      MAC_ALG_CFG_ANTI_INTF_RSSI_STAT_CYCLE},
    /* ������������unlock�ر�����: sh hipriv.sh "vap0 anti_inf_off_time [time]" */
    {"anti_inf_off_time",       MAC_ALG_CFG_ANTI_INTF_UNLOCK_CYCLE},
    /* ������������unlock�رճ���ʱ��: sh hipriv.sh "vap0 anti_inf_off_dur [time]" */
    {"anti_inf_off_dur",        MAC_ALG_CFG_ANTI_INTF_UNLOCK_DUR_TIME},
    /* ������nav����ʹ��: sh hipriv.sh "vap0 alg_cfg anti_inf_nav_en 0|1" */
    {"anti_inf_nav_en",         MAC_ALG_CFG_ANTI_INTF_NAV_IMM_ENABLE},
    /* ����������goodput�½�����: sh hipriv.sh "vap0 alg_cfg anti_inf_gd_th [num]" */
    {"anti_inf_gd_th",          MAC_ALG_CFG_ANTI_INTF_GOODPUT_FALL_TH},
    /* ����������̽�Ᵽ�����������: sh hipriv.sh "vap0 alg_cfg anti_inf_keep_max [num]" */
    {"anti_inf_keep_max",       MAC_ALG_CFG_ANTI_INTF_KEEP_CYC_MAX_NUM},
    /* ����������̽�Ᵽ�����������: sh hipriv.sh "vap0 alg_cfg anti_inf_keep_min [num]" */
    {"anti_inf_keep_min",       MAC_ALG_CFG_ANTI_INTF_KEEP_CYC_MIN_NUM},
    /* �����������Ƿ�ʹ��tx time̽��: sh hipriv.sh "vap0 anti_inf_tx_pro_en 0|1" */
    {"anti_inf_per_pro_en",     MAC_ALG_CFG_ANTI_INTF_PER_PROBE_EN},
    /* tx time�½�����: sh hipriv.sh "vap0 alg_cfg anti_inf_txtime_th [val]" */
    {"anti_inf_txtime_th",      MAC_ALG_CFG_ANTI_INTF_TX_TIME_FALL_TH},
    /* per�½�����: sh hipriv.sh "vap0 alg_cfg anti_inf_per_th [val]" */
    {"anti_inf_per_th",         MAC_ALG_CFG_ANTI_INTF_PER_FALL_TH},
    /* goodput��������: sh hipriv.sh "vap0 alg_cfg anti_inf_gd_jitter_th [val]" */
    {"anti_inf_gd_jitter_th",   MAC_ALG_CFG_ANTI_INTF_GOODPUT_JITTER_TH},
    /* ����������debug�Ĵ�ӡ��Ϣ: sh hipriv.sh "vap0 alg_cfg anti_inf_debug_mode 0|1|2" */
    {"anti_inf_debug_mode",     MAC_ALG_CFG_ANTI_INTF_DEBUG_MODE},
    /* ͬƵ���ż������: sh hipriv.sh "vap0 alg_cfg edca_opt_co_ch_time [time]" */
    {"edca_opt_co_ch_time",     MAC_ALG_CFG_EDCA_OPT_CO_CH_DET_CYCLE},
    /* �̶�edca�Ż���edca��������ֹ�Ĵ���ˢ��:sh hipriv.sh "vap0 alg_cfg edca_opt_fix_param 0|1" */
    {"edca_opt_fix_param",      MAC_ALG_CFG_EDCA_FIX_PARAM_ENABLE},
    /* apģʽ��edca�Ż�ʹ��ģʽ: sh hipriv.sh "vap0 alg_cfg edca_opt_en_ap 0|1|2" */
    {"edca_opt_en_ap",          MAC_ALG_CFG_EDCA_OPT_AP_EN_MODE},
    /* staģʽ��edca�Ż�ʹ��ģʽ: sh hipriv.sh "vap0 alg_cfg edca_opt_en_sta 0|1" */
    {"edca_opt_en_sta",         MAC_ALG_CFG_EDCA_OPT_STA_EN},
    /* staģʽ��edca txop limit�Ż�ʹ��ģʽ: sh hipriv.sh "vap0 alg_cfg txop_limit_en_sta 0|1" */
    {"txop_limit_en_sta",       MAC_ALG_CFG_TXOP_LIMIT_STA_EN},
    /* staģʽ��edca�Ż���weightingϵ��: sh hipriv.sh "vap0 alg_cfg edca_opt_sta_weight 0~3" */
    {"edca_opt_sta_weight",     MAC_ALG_CFG_EDCA_OPT_STA_WEIGHT},
    /* non-direct��ռ�ձ����� sh hipriv.sh "vap0 alg_cfg edca_opt_nondir_th [val]" */
    {"edca_opt_nondir_th",      MAC_ALG_CFG_EDCA_OPT_NONDIR_TH},
    /* apģʽ��UDPҵ���Ӧ���б����� sh hipriv.sh "vap0 alg_cfg edca_opt_th_udp [val]" */
    {"edca_opt_th_udp",         MAC_ALG_CFG_EDCA_OPT_TH_UDP},
    /* apģʽ��tcPҵ���Ӧ���б����� sh hipriv.sh "vap0 alg_cfg edca_opt_th_tcp [val]" */
    {"edca_opt_th_tcp",         MAC_ALG_CFG_EDCA_OPT_TH_TCP},
    /* �Ƿ��ӡ�����Ϣ�������ڱ��ذ汾���� */
    {"edca_opt_debug_mode",     MAC_ALG_CFG_EDCA_OPT_DEBUG_MODE},
    /* ��԰��׿��Ż�����: sh hipriv.sh "vap0 alg_cfg edca_opt_fe_port_opt 0|1" */
    {"edca_opt_fe_port_opt",    MAC_ALG_CFG_EDCA_OPT_FE_PORT_OPT},
    /* ��԰��׿ڵ��Կ���: sh hipriv.sh "vap0 alg_cfg edca_opt_fe_port_dbg 0|1" */
    {"edca_opt_fe_port_dbg",    MAC_ALG_CFG_EDCA_OPT_FE_PORT_DBG},
    /* 0x21���������0x64��������MDPU���ٵı�������(%):sh hipriv.sh "vap0 alg_cfg edca_opt_mpdu_dec_ratio[ratio(%)]" */
    {"edca_opt_mpdu_dec_ratio", MAC_ALG_CFG_EDCA_OPT_MPDU_DEC_RATIO_TH},
    /* Ĭ�ϲ���0x64�·��ͳɹ���MPDU����: sh hipriv.sh "vap0 alg_cfg edca_opt_def_cw_mpdu_th [num]" */
    {"edca_opt_def_cw_mpdu_th", MAC_ALG_CFG_EDCA_OPT_DEFAULT_CW_MPDU_TH},
    /* CCA�Ż�����ʹ��: sh hipriv.sh "vap0 alg_cfg cca_opt_alg_en_mode 0|1" */
    {"cca_opt_alg_en_mode",         MAC_ALG_CFG_CCA_OPT_ALG_EN_MODE},
    /* CCA�Ż�DEBUGģʽ����: sh hipriv.sh "vap0 alg_cfg cca_opt_debug_mode 0|1" */
    {"cca_opt_debug_mode",          MAC_ALG_CFG_CCA_OPT_DEBUG_MODE},
    /* CCA�Ż�T1��ʱ����:sh hipriv.sh "vap0 alg_cfg cca_opt_set_t1_counter_time [time]" */
    {"cca_opt_set_t1_counter_time", MAC_ALG_CFG_CCA_OPT_SET_T1_COUNTER_TIME},
    /* CCA�Ż�T2��ʱ����:sh hipriv.sh "vap0 alg_cfg cca_opt_set_t2_counter_time [time]" */
    {"cca_opt_set_t2_counter_time", MAC_ALG_CFG_CCA_OPT_SET_T2_COUNTER_TIME},
    /* CCA�Ż��ж��Ƿ����ƽ��RSSI�Ŀ��й��ʷ�0ֵ�Ĵ�������:sh hipriv.sh "vap0 alg_cfg cca_opt_set_ilde_cnt_th[val]" */
    {"cca_opt_set_ilde_cnt_th",     MAC_ALG_CFG_CCA_OPT_SET_ILDE_CNT_TH},
    /* CCA�Ż��ж��Ƿ����ǿ��Ƶ����Ƶ���ŵķ�æ����ֵ:sh hipriv.sh "vap0 alg_cfg cca_opt_set_duty_cyc_th [val]" */
    {"cca_opt_set_duty_cyc_th",     MAC_ALG_CFG_CCA_OPT_SET_DUTY_CYC_TH},
    /* CCA�Ż��ж��Ƿ������Ƶ����Ƶ���ŵ�sync error��ֵ:sh hipriv.sh "vap0 alg_cfg cca_opt_set_aveg_rssi_th [val]" */
    {"cca_opt_set_aveg_rssi_th",    MAC_ALG_CFG_CCA_OPT_SET_AVEG_RSSI_TH},
    /*
        CCA�Ż��ж��Ƿ������Ƶ����Ƶ���ŵ�pri20/40/80�������ֵ:
        sh hipriv.sh "vap0 alg_cfg cca_opt_set_chn_scan_cyc [val]"
    */
    {"cca_opt_set_chn_scan_cyc",    MAC_ALG_CFG_CCA_OPT_SET_CHN_SCAN_CYC},
    /* CCA�Ż��ŵ�ɨ���ʱ��(ms):sh hipriv.sh "vap0 alg_cfg cca_opt_set_sync_err_th [time]" */
    {"cca_opt_set_sync_err_th",     MAC_ALG_CFG_CCA_OPT_SET_SYNC_ERR_TH},
    /* CCA�Ż��ŵ�ɨ���ʱ��(ms):sh hipriv.sh "vap0 alg_cfg cca_opt_set_sync_err_th [time]" */
    {"cca_opt_set_cca_th_debug",    MAC_ALG_CFG_CCA_OPT_SET_CCA_TH_DEBUG},
    /* CCA log���� sh hipriv.sh "vap0 alg_cfg cca_opt_log 0|1" */
    {"cca_opt_log",                 MAC_ALG_CFG_CCA_OPT_LOG},
    /* ��ʼͳ����־:sh hipriv.sh "vap0 alg_cca_opt_log cca_opt_stat_log_do [val]"  */
    {"cca_opt_stat_log_do",         MAC_ALG_CFG_CCA_OPT_STAT_LOG_START},
    /* ��ӡͳ����־:sh hipriv.sh "vap0 alg_cca_opt_log cca_opt_stat_log_out" */
    {"cca_opt_stat_log_out",        MAC_ALG_CFG_CCA_OPT_STAT_LOG_WRITE},
    /* CCA����������ײ������(ms):sh hipriv.sh "vap0 alg_cfg cca_opt_set_collision_ratio_th [val]" */
    {"cca_opt_set_collision_ratio_th", MAC_ALG_CFG_CCA_OPT_SET_COLLISION_RATIO_TH},
    /* CCA��������goddput����(ms):sh hipriv.sh "vap0 alg_cfg cca_opt_set_goodput_loss_th [val]" */
    {"cca_opt_set_goodput_loss_th", MAC_ALG_CFG_CCA_OPT_SET_GOODPUT_LOSS_TH},
    /* CCA�����������̽����(ms):sh hipriv.sh "vap0 alg_cfg cca_opt_set_max_intvl_num [val]" */
    {"cca_opt_set_max_intvl_num",   MAC_ALG_CFG_CCA_OPT_SET_MAX_INTVL_NUM},
    /* CCA�޸��ż���������(ms):sh hipriv.sh "vap0 alg_cfg cca_opt_set_non_intf_cyc_num [val]" */
    {"cca_opt_set_non_intf_cyc_num",       MAC_ALG_CFG_CCA_OPT_NON_INTF_CYCLE_NUM_TH},
    /* CCA�޸��ż���������(ms):sh hipriv.sh "vap0 alg_cfg cca_opt_set_non_intf_duty_cyc_th [val]" */
    {"cca_opt_set_non_intf_duty_cyc_th",   MAC_ALG_CFG_CCA_OPT_NON_INTF_DUTY_CYC_TH},

    {"tpc_mode",                MAC_ALG_CFG_TPC_MODE},                          /* ����TPC����ģʽ */
    {"tpc_dbg",                 MAC_ALG_CFG_TPC_DEBUG},                         /* ����TPC��debug���� */
    {"tpc_pow_lvl",             MAC_ALG_CFG_TPC_POWER_LEVEL},                   /* ����TPC���ʵȼ�(0,1,2,3), �ڹ̶�����ģʽ��ʹ�� */
    {"tpc_log",                 MAC_ALG_CFG_TPC_LOG}, /* ����TPC��log����:sh hipriv.sh "vap0 alg_cfg tpc_log 1 */
    /*
        ��ʼ����ͳ����־:sh hipriv.sh "vap0 alg_tpc_log tpc_stat_log_do [mac��ַ] [ҵ�����] [����Ŀ]"
        ��: sh hipriv.sh "vap0 alg_tpc_log tpc_stat_log_do 06:31:04:E3:81:02 1 1000"
 */
    { "tpc_stat_log_do", MAC_ALG_CFG_TPC_STAT_LOG_START },
    /* ��ӡ����ͳ����־:sh hipriv.sh "vap0 alg_tpc_log tpc_stat_log_out 06:31:04:E3:81:02" */
    { "tpc_stat_log_out", MAC_ALG_CFG_TPC_STAT_LOG_WRITE },
    /*
        ��ʼÿ��ͳ����־:sh hipriv.sh "vap0 alg_tpc_log tpc_pkt_log_do [mac��ַ] [ҵ�����] [����Ŀ]"
        ��: sh hipriv.sh "vap0 alg_tpc_log tpc_pkt_log_do 06:31:04:E3:81:02 1 1000"
 */
    { "tpc_pkt_log_do", MAC_ALG_CFG_TPC_PER_PKT_LOG_START },
    /* ��ȡ����֡����:sh hipriv.sh "vap0 alg_tpc_log tpc_get_frame_pow beacon_pow" */
    { "tpc_get_frame_pow",          MAC_ALG_CFG_TPC_GET_FRAME_POW },
    { "tpc_reset_stat",             MAC_ALG_CFG_TPC_RESET_STAT },                /* �ͷ�ͳ���ڴ� */
    { "tpc_reset_pkt",              MAC_ALG_CFG_TPC_RESET_PKT },                  /* �ͷ�ÿ���ڴ� */
    { "tpc_over_temp_th",           MAC_ALG_CFG_TPC_OVER_TMP_TH },             /* TPC�������� */
    { "tpc_dpd_enable_rate",        MAC_ALG_CFG_TPC_DPD_ENABLE_RATE },      /* ����DPD��Ч������INDEX */
    { "tpc_target_rate_11b",        MAC_ALG_CFG_TPC_TARGET_RATE_11B },      /* 11bĿ���������� */
    { "tpc_target_rate_11ag",       MAC_ALG_CFG_TPC_TARGET_RATE_11AG },    /* 11agĿ���������� */
    { "tpc_target_rate_11n20",      MAC_ALG_CFG_TPC_TARGET_RATE_HT40 },   /* 11n20Ŀ���������� */
    { "tpc_target_rate_11n40",      MAC_ALG_CFG_TPC_TARGET_RATE_HT40 },   /* 11n40Ŀ���������� */
    { "tpc_target_rate_11ac20",     MAC_ALG_CFG_TPC_TARGET_RATE_VHT20 }, /* 11ac20Ŀ���������� */
    { "tpc_target_rate_11ac40",     MAC_ALG_CFG_TPC_TARGET_RATE_VHT40 }, /* 11ac40Ŀ���������� */
    { "tpc_target_rate_11ac80",     MAC_ALG_CFG_TPC_TARGET_RATE_VHT80 }, /* 11ac80Ŀ���������� */
    /* ��ӡTPC����־��Ϣ:sh hipriv.sh "vap0 alg_cfg tpc_show_log_info */
    {"tpc_show_log_info",       MAC_ALG_CFG_TPC_SHOW_LOG_INFO},
    {"tpc_no_margin_pow",       MAC_ALG_CFG_TPC_NO_MARGIN_POW},                 /* 51����û���������� */
    {"tpc_power_amend",         MAC_ALG_CFG_TPC_POWER_AMEND},     /* tx power�ڴ��ڲ�ƽ̹��tpc���й���������Ĭ��Ϊ0 */
    {"temp_pro_get_info",       MAC_ALG_CFG_TEMP_PRO_GET_INFO},                 /* ���±�����ȡά����Ϣ */

    {OAL_PTR_NULL}
};

OAL_CONST wal_dfs_domain_entry_stru g_ast_dfs_domain_table[] = {
    {"AE", MAC_DFS_DOMAIN_ETSI},
    {"AL", MAC_DFS_DOMAIN_NULL},
    {"AM", MAC_DFS_DOMAIN_ETSI},
    {"AN", MAC_DFS_DOMAIN_ETSI},
    {"AR", MAC_DFS_DOMAIN_FCC},
    {"AT", MAC_DFS_DOMAIN_ETSI},
    {"AU", MAC_DFS_DOMAIN_FCC},
    {"AZ", MAC_DFS_DOMAIN_ETSI},
    {"BA", MAC_DFS_DOMAIN_ETSI},
    {"BE", MAC_DFS_DOMAIN_ETSI},
    {"BG", MAC_DFS_DOMAIN_ETSI},
    {"BH", MAC_DFS_DOMAIN_ETSI},
    {"BL", MAC_DFS_DOMAIN_NULL},
    {"BN", MAC_DFS_DOMAIN_ETSI},
    {"BO", MAC_DFS_DOMAIN_ETSI},
    {"BR", MAC_DFS_DOMAIN_FCC},
    {"BY", MAC_DFS_DOMAIN_ETSI},
    {"BZ", MAC_DFS_DOMAIN_ETSI},
    {"CA", MAC_DFS_DOMAIN_FCC},
    {"CH", MAC_DFS_DOMAIN_ETSI},
    {"CL", MAC_DFS_DOMAIN_NULL},
    {"CN", MAC_DFS_DOMAIN_NULL},
    {"CO", MAC_DFS_DOMAIN_FCC},
    {"CR", MAC_DFS_DOMAIN_FCC},
    {"CS", MAC_DFS_DOMAIN_ETSI},
    {"CY", MAC_DFS_DOMAIN_ETSI},
    {"CZ", MAC_DFS_DOMAIN_ETSI},
    {"DE", MAC_DFS_DOMAIN_ETSI},
    {"DK", MAC_DFS_DOMAIN_ETSI},
    {"DO", MAC_DFS_DOMAIN_FCC},
    {"DZ", MAC_DFS_DOMAIN_NULL},
    {"EC", MAC_DFS_DOMAIN_FCC},
    {"EE", MAC_DFS_DOMAIN_ETSI},
    {"EG", MAC_DFS_DOMAIN_ETSI},
    {"ES", MAC_DFS_DOMAIN_ETSI},
    {"FI", MAC_DFS_DOMAIN_ETSI},
    {"FR", MAC_DFS_DOMAIN_ETSI},
    {"GB", MAC_DFS_DOMAIN_ETSI},
    {"GE", MAC_DFS_DOMAIN_ETSI},
    {"GR", MAC_DFS_DOMAIN_ETSI},
    {"GT", MAC_DFS_DOMAIN_FCC},
    {"HK", MAC_DFS_DOMAIN_FCC},
    {"HN", MAC_DFS_DOMAIN_FCC},
    {"HR", MAC_DFS_DOMAIN_ETSI},
    {"HU", MAC_DFS_DOMAIN_ETSI},
    {"ID", MAC_DFS_DOMAIN_NULL},
    {"IE", MAC_DFS_DOMAIN_ETSI},
    {"IL", MAC_DFS_DOMAIN_ETSI},
    {"IN", MAC_DFS_DOMAIN_NULL},
    {"IQ", MAC_DFS_DOMAIN_NULL},
    {"IR", MAC_DFS_DOMAIN_NULL},
    {"IS", MAC_DFS_DOMAIN_ETSI},
    {"IT", MAC_DFS_DOMAIN_ETSI},
    {"JM", MAC_DFS_DOMAIN_FCC},
    {"JO", MAC_DFS_DOMAIN_ETSI},
    {"JP", MAC_DFS_DOMAIN_MKK},
    {"KP", MAC_DFS_DOMAIN_NULL},
    {"KR", MAC_DFS_DOMAIN_KOREA},
    {"KW", MAC_DFS_DOMAIN_ETSI},
    {"KZ", MAC_DFS_DOMAIN_NULL},
    {"LB", MAC_DFS_DOMAIN_NULL},
    {"LI", MAC_DFS_DOMAIN_ETSI},
    {"LK", MAC_DFS_DOMAIN_FCC},
    {"LT", MAC_DFS_DOMAIN_ETSI},
    {"LU", MAC_DFS_DOMAIN_ETSI},
    {"LV", MAC_DFS_DOMAIN_ETSI},
    {"MA", MAC_DFS_DOMAIN_NULL},
    {"MC", MAC_DFS_DOMAIN_ETSI},
    {"MK", MAC_DFS_DOMAIN_ETSI},
    {"MO", MAC_DFS_DOMAIN_FCC},
    {"MT", MAC_DFS_DOMAIN_ETSI},
    {"MX", MAC_DFS_DOMAIN_FCC},
    {"MY", MAC_DFS_DOMAIN_FCC},
    {"NG", MAC_DFS_DOMAIN_NULL},
    {"NL", MAC_DFS_DOMAIN_ETSI},
    {"NO", MAC_DFS_DOMAIN_ETSI},
    {"NP", MAC_DFS_DOMAIN_NULL},
    {"NZ", MAC_DFS_DOMAIN_FCC},
    {"OM", MAC_DFS_DOMAIN_FCC},
    {"PA", MAC_DFS_DOMAIN_FCC},
    {"PE", MAC_DFS_DOMAIN_FCC},
    {"PG", MAC_DFS_DOMAIN_FCC},
    {"PH", MAC_DFS_DOMAIN_FCC},
    {"PK", MAC_DFS_DOMAIN_NULL},
    {"PL", MAC_DFS_DOMAIN_ETSI},
    {"PR", MAC_DFS_DOMAIN_FCC},
    {"PT", MAC_DFS_DOMAIN_ETSI},
    {"QA", MAC_DFS_DOMAIN_NULL},
    {"RO", MAC_DFS_DOMAIN_ETSI},
    {"RU", MAC_DFS_DOMAIN_FCC},
    {"SA", MAC_DFS_DOMAIN_FCC},
    {"SE", MAC_DFS_DOMAIN_ETSI},
    {"SG", MAC_DFS_DOMAIN_NULL},
    {"SI", MAC_DFS_DOMAIN_ETSI},
    {"SK", MAC_DFS_DOMAIN_ETSI},
    {"SV", MAC_DFS_DOMAIN_FCC},
    {"SY", MAC_DFS_DOMAIN_NULL},
    {"TH", MAC_DFS_DOMAIN_FCC},
    {"TN", MAC_DFS_DOMAIN_ETSI},
    {"TR", MAC_DFS_DOMAIN_ETSI},
    {"TT", MAC_DFS_DOMAIN_FCC},
    {"TW", MAC_DFS_DOMAIN_NULL},
    {"UA", MAC_DFS_DOMAIN_NULL},
    {"US", MAC_DFS_DOMAIN_FCC},
    {"UY", MAC_DFS_DOMAIN_FCC},
    {"UZ", MAC_DFS_DOMAIN_FCC},
    {"VE", MAC_DFS_DOMAIN_FCC},
    {"VN", MAC_DFS_DOMAIN_ETSI},
    {"YE", MAC_DFS_DOMAIN_NULL},
    {"ZA", MAC_DFS_DOMAIN_FCC},
    {"ZW", MAC_DFS_DOMAIN_NULL},
};
#ifdef _PRE_WLAN_FEATURE_NRCOEX
OAL_CONST wal_ioctl_nrcoex_cmd_stru g_ast_nrcoex_cmd_map[] = {
    {"display_info",  WLAN_NRCOEX_CMD_DISPLAY_INFO},
    {"nrcoex_event",  WLAN_NRCOEX_CMD_NRCOEX_EVENT},
    {"modem_freq",    WLAN_NRCOEX_CMD_MODEM_FREQ},
    {"modem_bw",      WLAN_NRCOEX_CMD_MODEM_BW},
    {OAL_PTR_NULL}
};
#endif

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/

oal_uint32 wal_get_cmd_one_arg(oal_int8 *pc_cmd, oal_int8 *pc_arg, oal_uint32 ul_arg_len, oal_uint32 *pul_cmd_offset)
{
    oal_int8 *pc_cmd_copy = OAL_PTR_NULL;
    oal_uint32 ul_pos = 0;

    if (OAL_UNLIKELY((pc_cmd == OAL_PTR_NULL) || (pc_arg == OAL_PTR_NULL) || (pul_cmd_offset == OAL_PTR_NULL))) {
        OAM_ERROR_LOG3(0, OAM_SF_ANY, "wal_get_cmd_one_arg::pc_cmd/pc_arg/pul_cmd_offset null ptr error %x, %x, %x",
                       (uintptr_t)pc_cmd, (uintptr_t)pc_arg, (uintptr_t)pul_cmd_offset);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pc_cmd_copy = pc_cmd;

    /* ȥ���ַ�����ʼ�Ŀո� */
    while (*pc_cmd_copy == ' ') {
        ++pc_cmd_copy;
    }

    while ((*pc_cmd_copy != ' ') && (*pc_cmd_copy != '\0')) {
        pc_arg[ul_pos] = *pc_cmd_copy;
        ++ul_pos;
        ++pc_cmd_copy;

        if (OAL_UNLIKELY(ul_pos >= ul_arg_len)) {
            OAM_ERROR_LOG1(0, OAM_SF_ANY, "{wal_get_cmd_one_arg::ul_pos >= WAL_HIPRIV_CMD_NAME_MAX_LEN, ul_pos=%d!}",
                           ul_pos);
            return OAL_ERR_CODE_ARRAY_OVERFLOW;
        }
    }

    pc_arg[ul_pos] = '\0';

    /* �ַ�������β�����ش����� */
    if (ul_pos == 0) {
        OAM_INFO_LOG0(0, OAM_SF_ANY, "{wal_get_cmd_one_arg::return param pc_arg is null!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    *pul_cmd_offset = (oal_uint32)(pc_cmd_copy - pc_cmd);

    return OAL_SUCC;
}


oal_void wal_msg_queue_init(oal_void)
{
    memset_s((oal_void *)&g_wal_wid_msg_queue, OAL_SIZEOF(g_wal_wid_msg_queue), 0, OAL_SIZEOF(g_wal_wid_msg_queue));
    oal_dlist_init_head(&g_wal_wid_msg_queue.st_head);
    g_wal_wid_msg_queue.count = 0;
    oal_spin_lock_init(&g_wal_wid_msg_queue.st_lock);
    OAL_WAIT_QUEUE_INIT_HEAD(&g_wal_wid_msg_queue.st_wait_queue);
}

OAL_STATIC oal_void _wal_msg_request_add_queue_(wal_msg_request_stru *pst_msg)
{
    oal_dlist_add_tail(&pst_msg->pst_entry, &g_wal_wid_msg_queue.st_head);
    g_wal_wid_msg_queue.count++;
}


oal_uint32 wal_get_request_msg_count(oal_void)
{
    return g_wal_wid_msg_queue.count;
}

oal_uint32 wal_check_and_release_msg_resp(wal_msg_stru *pst_rsp_msg)
{
    wal_msg_write_rsp_stru *pst_write_rsp_msg = OAL_PTR_NULL;
    if (pst_rsp_msg != OAL_PTR_NULL) {
        oal_uint32 ul_err_code;
        wlan_cfgid_enum_uint16 en_wid;
        pst_write_rsp_msg = (wal_msg_write_rsp_stru *)(pst_rsp_msg->auc_msg_data);
        ul_err_code = pst_write_rsp_msg->ul_err_code;
        en_wid = pst_write_rsp_msg->en_wid;
        oal_free(pst_rsp_msg);

        if (ul_err_code != OAL_SUCC) {
            OAM_WARNING_LOG2(0, OAM_SF_SCAN, "{wal_check_and_release_msg_resp::detect err code:[%u], wid:[%u]}",
                             ul_err_code, en_wid);
            return ul_err_code;
        }
    }

    return OAL_SUCC;
}


oal_void wal_msg_request_add_queue(wal_msg_request_stru *pst_msg)
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

OAL_STATIC oal_void _wal_msg_request_remove_queue_(wal_msg_request_stru *pst_msg)
{
    g_wal_wid_msg_queue.count--;
    oal_dlist_delete_entry(&pst_msg->pst_entry);
}


oal_void wal_msg_request_remove_queue(wal_msg_request_stru *pst_msg)
{
    oal_spin_lock_bh(&g_wal_wid_msg_queue.st_lock);
    _wal_msg_request_remove_queue_(pst_msg);
    oal_spin_unlock_bh(&g_wal_wid_msg_queue.st_lock);
}


oal_int32 wal_set_msg_response_by_addr(oal_ulong addr, oal_void *pst_resp_mem, oal_uint32 ul_resp_ret,
                                       oal_uint32 uc_rsp_len)
{
    oal_int32 l_ret = -OAL_EINVAL;
    oal_dlist_head_stru *pst_pos = OAL_PTR_NULL;
    oal_dlist_head_stru *pst_entry_temp = OAL_PTR_NULL;
    wal_msg_request_stru *pst_request = NULL;

    oal_spin_lock_bh(&g_wal_wid_msg_queue.st_lock);
    OAL_DLIST_SEARCH_FOR_EACH_SAFE(pst_pos, pst_entry_temp, (&g_wal_wid_msg_queue.st_head))
    {
        pst_request = (wal_msg_request_stru *)OAL_DLIST_GET_ENTRY(pst_pos, wal_msg_request_stru,
                                                                  pst_entry);
        if (pst_request->ul_request_address == (oal_ulong)addr) {
            /* address match */
            if (OAL_UNLIKELY(pst_request->pst_resp_mem != NULL)) {
                OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_set_msg_response_by_addr::wal_set_msg_response_by_addr \
                    response had been set!");
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


oal_uint32 wal_alloc_cfg_event(oal_net_device_stru *pst_net_dev,
                               frw_event_mem_stru **ppst_event_mem,
                               oal_void *pst_resp_addr,
                               wal_msg_stru **ppst_cfg_msg,
                               oal_uint16 us_len)
{
    mac_vap_stru *pst_vap;
    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL;
    frw_event_stru *pst_event = OAL_PTR_NULL;
    oal_uint16 us_resp_len = 0;

    wal_msg_rep_hdr *pst_rep_hdr = NULL;

    pst_vap = OAL_NET_DEV_PRIV(pst_net_dev);
    if (OAL_UNLIKELY(pst_vap == OAL_PTR_NULL)) {
        /* ���wifi�ر�״̬�£��·�hipriv������ʾerror��־ */
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_alloc_cfg_event::OAL_NET_DEV_PRIV(pst_net_dev) is null ptr! \
            pst_net_dev=[%p]}", (uintptr_t)pst_net_dev);
        return OAL_ERR_CODE_PTR_NULL;
    }

    us_resp_len += OAL_SIZEOF(wal_msg_rep_hdr);

    us_len += us_resp_len;

    pst_event_mem = FRW_EVENT_ALLOC(us_len);
    if (OAL_UNLIKELY(pst_event_mem == OAL_PTR_NULL)) {
        OAM_ERROR_LOG2(pst_vap->uc_vap_id, OAM_SF_ANY, "{wal_alloc_cfg_event::pst_event_mem is null, \
            request size:us_len:%d, resp_len:%d}", us_len, us_resp_len);
        return OAL_ERR_CODE_PTR_NULL;
    }

    *ppst_event_mem = pst_event_mem; /* ���θ�ֵ */

    pst_event = (frw_event_stru *)pst_event_mem->puc_data;

    /* ��д�¼�ͷ */
    FRW_EVENT_HDR_INIT(&(pst_event->st_event_hdr),
                       FRW_EVENT_TYPE_HOST_CRX,
                       WAL_HOST_CRX_SUBTYPE_CFG,
                       us_len,
                       FRW_EVENT_PIPELINE_STAGE_0,
                       pst_vap->uc_chip_id,
                       pst_vap->uc_device_id,
                       pst_vap->uc_vap_id);

    /* fill the resp hdr */
    pst_rep_hdr = (wal_msg_rep_hdr *)pst_event->auc_event_data;
    if (pst_resp_addr == NULL) {
        /* no response */
        pst_rep_hdr->ul_request_address = (oal_ulong)(uintptr_t)0;
    } else {
        /* need response */
        pst_rep_hdr->ul_request_address = (oal_ulong)(uintptr_t)pst_resp_addr;
    }

    *ppst_cfg_msg = (wal_msg_stru *)((oal_uint8 *)pst_event->auc_event_data + us_resp_len); /* ���θ�ֵ */

    return OAL_SUCC;
}


OAL_STATIC OAL_INLINE oal_int32 wal_request_wait_event_condition(wal_msg_request_stru *pst_msg_stru)
{
    oal_int32 l_ret = OAL_FALSE;
    oal_spin_lock_bh(&g_wal_wid_msg_queue.st_lock);
    if ((pst_msg_stru->pst_resp_mem != NULL) || (pst_msg_stru->ul_ret != OAL_SUCC)) {
        l_ret = OAL_TRUE;
    }
    oal_spin_unlock_bh(&g_wal_wid_msg_queue.st_lock);
    return l_ret;
}

oal_void wal_cfg_msg_task_sched(oal_void)
{
    OAL_WAIT_QUEUE_WAKE_UP(&g_wal_wid_msg_queue.st_wait_queue);
}

#ifdef _PRE_WLAN_FEATURE_TAS_ANT_SWITCH
OAL_STATIC oal_int32 wal_ioctl_tas_rssi_access(oal_net_device_stru *pst_net_dev, oal_uint8 uc_coreindex)
{
    oal_int32 l_ret;
    wal_msg_write_stru st_write_msg;

    /* vapδ����ʱ��������supplicant���� */
    if (OAL_NET_DEV_PRIV(pst_net_dev) == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_CFG, "wal_ioctl_tas_rssi_access::vap not created yet, ignore the cmd!");
        return -OAL_EINVAL;
    }

    if (g_aen_tas_switch_en[WITP_RF_CHANNEL_ZERO] == OAL_FALSE) {
        /* ��ǰ���߲�֧��TAS�л����� */
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "wal_ioctl_tas_rssi_access::core[%d] is not supported!", uc_coreindex);
        return OAL_SUCC;
    }

    /***************************************************************************
                                 ���¼���wal�㴦��
     ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_TAS_RSSI_ACCESS, OAL_SIZEOF(oal_uint8));
    st_write_msg.auc_value[0] = WITP_RF_CHANNEL_ZERO;
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(oal_uint8),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "wal_ioctl_tas_rssi_access::wal_send_cfg_event_etc failed, error no[%d]!", l_ret);
        return l_ret;
    }

    return OAL_SUCC;
}
#endif


oal_int32 wal_send_cfg_event(oal_net_device_stru *pst_net_dev,
                             wal_msg_type_enum_uint8 en_msg_type,
                             oal_uint16 us_len,
                             oal_uint8 *puc_param,
                             oal_bool_enum_uint8 en_need_rsp,
                             wal_msg_stru **ppst_rsp_msg)
{
    wal_msg_stru *pst_cfg_msg = OAL_PTR_NULL;
    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;
    oal_uint32 ul_ret;
    oal_int32 l_ret;
    oal_uint8 *puc_msg_data = OAL_PTR_NULL;

    DECLARE_WAL_MSG_REQ_STRU(st_msg_request);

    WAL_MSG_REQ_STRU_INIT(st_msg_request);

    if (ppst_rsp_msg != NULL) {
        *ppst_rsp_msg = NULL;
    }

    if (OAL_WARN_ON((en_need_rsp == OAL_TRUE) && (ppst_rsp_msg == OAL_PTR_NULL))) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_send_cfg_event::OAL_PTR_NULL == ppst_rsp_msg!}\r\n");
        return -OAL_EINVAL;
    }

    /* �����¼� */
    ul_ret = wal_alloc_cfg_event(pst_net_dev, &pst_event_mem,
        ((en_need_rsp == OAL_TRUE) ? &st_msg_request : NULL), &pst_cfg_msg, WAL_MSG_WRITE_MSG_HDR_LENGTH + us_len);
    if (OAL_UNLIKELY(ul_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_send_cfg_event::wal_alloc_cfg_event return err code %d!}\r\n", ul_ret);
        return -OAL_ENOMEM;
    }

    /* ��д������Ϣ */
    WAL_CFG_MSG_HDR_INIT(&(pst_cfg_msg->st_msg_hdr), en_msg_type, us_len, WAL_GET_MSG_SN());

    /* ��дWID��Ϣ */
    puc_msg_data = pst_cfg_msg->auc_msg_data;
    if (memcpy_s(puc_msg_data, us_len, puc_param, us_len) != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "wal_send_cfg_event_etc::memcpy fail!");
        FRW_EVENT_FREE(pst_event_mem);
        return -OAL_EINVAL;
    }
    if (en_need_rsp == OAL_TRUE) {
        /* add queue before post event! */
        wal_msg_request_add_queue(&st_msg_request);
    }

    /* �ַ��¼� */
    frw_event_dispatch_event(pst_event_mem);
    FRW_EVENT_FREE(pst_event_mem);

    /* win32 UTģʽ������һ���¼����� */
#if (_PRE_OS_VERSION_WIN32 == _PRE_OS_VERSION) && (_PRE_TEST_MODE == _PRE_TEST_MODE_UT)
    frw_event_process_all_event(0);
#endif

    if (en_need_rsp == OAL_FALSE) {
        return OAL_SUCC;
    }

    /* context can't in interrupt */
    if (OAL_WARN_ON(oal_in_interrupt())) {
        DECLARE_DFT_TRACE_KEY_INFO("wal_cfg_in_interrupt", OAL_DFT_TRACE_EXCEP);
    }

    if (OAL_WARN_ON(oal_in_atomic())) {
        DECLARE_DFT_TRACE_KEY_INFO("wal_cfg_in_atomic", OAL_DFT_TRACE_EXCEP);
    }

    /***************************************************************************
        �ȴ��¼�����
    ***************************************************************************/
    wal_wake_lock();

    /*lint -e730*/ /* info, boolean argument to function */
    l_ret = OAL_WAIT_EVENT_TIMEOUT(g_wal_wid_msg_queue.st_wait_queue,
        OAL_TRUE == wal_request_wait_event_condition(&st_msg_request), 30 * OAL_TIME_HZ); /* ��ʱʱ��30 * OAL_TIME_HZ */
    /*lint +e730*/

    /* response had been set, remove it from the list */
    if (en_need_rsp == OAL_TRUE) {
        wal_msg_request_remove_queue(&st_msg_request);
    }

    if (OAL_WARN_ON(l_ret == 0)) {
        /* ��ʱ . */
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_send_cfg_event:: wait queue timeout, 30s!}\r\n");
        OAL_IO_PRINT("[E]timeout, request info:%p, ret=%u, addr:0x%lx\n", st_msg_request.pst_resp_mem,
                     st_msg_request.ul_ret,
                     st_msg_request.ul_request_address);
        if (st_msg_request.pst_resp_mem != NULL) {
            oal_free(st_msg_request.pst_resp_mem);
        }
        wal_wake_unlock();
        DECLARE_DFT_TRACE_KEY_INFO("wal_send_cfg_timeout", OAL_DFT_TRACE_FAIL);
        /* ��ӡCFG EVENT�ڴ棬���㶨λ��32��ʾgroupsize�Ĵ�С */
        oal_print_hex_dump((oal_uint8 *)pst_cfg_msg, (WAL_MSG_WRITE_MSG_HDR_LENGTH + us_len), 32, "cfg event: ");
        frw_event_queue_info();
        return -OAL_ETIMEDOUT;
    }
    /*lint +e774*/
    pst_rsp_msg = (wal_msg_stru *)(st_msg_request.pst_resp_mem);
    if (pst_rsp_msg == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_send_cfg_event:: msg mem null!}");
        /*lint -e613*/
        *ppst_rsp_msg = OAL_PTR_NULL;
        /*lint +e613*/
        wal_wake_unlock();
        return -OAL_EFAUL;
    }

    if (pst_rsp_msg->st_msg_hdr.us_msg_len == 0) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_send_cfg_event:: no msg resp!}");
        /*lint -e613*/
        *ppst_rsp_msg = OAL_PTR_NULL;
        /*lint +e613*/
        oal_free(pst_rsp_msg);
        wal_wake_unlock();
        return -OAL_EFAUL;
    }
    /* ���������¼����ص�״̬��Ϣ */
    /*lint -e613*/
    *ppst_rsp_msg = pst_rsp_msg;
    /*lint +e613*/
    wal_wake_unlock();
    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_P2P


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
#endif

oal_int32 wal_cfg_vap_h2d_event(oal_net_device_stru *pst_net_dev)
{
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    oal_wireless_dev_stru *pst_wdev;
    mac_wiphy_priv_stru *pst_wiphy_priv = OAL_PTR_NULL;
    hmac_vap_stru *pst_cfg_hmac_vap = OAL_PTR_NULL;
    mac_device_stru *pst_mac_device = OAL_PTR_NULL;
    oal_net_device_stru *pst_cfg_net_dev = OAL_PTR_NULL;

    oal_int32 l_ret;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;
    oal_uint32 ul_err_code;
    wal_msg_write_stru st_write_msg;

    pst_wdev = OAL_NETDEVICE_WDEV(pst_net_dev);
    if (pst_wdev == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_cfg_vap_h2d_event::pst_wdev is null!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_wiphy_priv = (mac_wiphy_priv_stru *)oal_wiphy_priv(pst_wdev->wiphy);
    if (pst_wiphy_priv == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_cfg_vap_h2d_event::pst_wiphy_priv is null!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_mac_device = pst_wiphy_priv->pst_mac_device;
    if (pst_mac_device == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_cfg_vap_h2d_event::pst_mac_device is null!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_cfg_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_device->uc_cfg_vap_id);
    if (pst_cfg_hmac_vap == OAL_PTR_NULL) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_cfg_vap_h2d_event::mac_res_get_hmac_vap fail.vap_id[%u]}",
                         pst_mac_device->uc_cfg_vap_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_cfg_net_dev = pst_cfg_hmac_vap->pst_net_device;
    if (pst_cfg_net_dev == NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_cfg_vap_h2d_event::pst_cfg_net_dev is null!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /***************************************************************************
    ���¼���wal�㴦��
    ***************************************************************************/
    /* ��д��Ϣ */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_CFG_VAP_H2D, OAL_SIZEOF(mac_cfg_vap_h2d_stru));
    ((mac_cfg_vap_h2d_stru *)st_write_msg.auc_value)->pst_net_dev = pst_cfg_net_dev;

    /* ������Ϣ */
    l_ret = wal_send_cfg_event(pst_cfg_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_vap_h2d_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_TRUE,
                               &pst_rsp_msg);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_cfg_vap_h2d_event::wal_alloc_cfg_event return err code %d!}\r\n", l_ret);
        return l_ret;
    }

    /* ��������Ϣ */
    ul_err_code = wal_check_and_release_msg_resp(pst_rsp_msg);
    if (ul_err_code != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_cfg_vap_h2d_event::hmac cfg vap h2d fail, err code[%u]\r\n", ul_err_code);
        return -OAL_EINVAL;
    }

#endif

    return OAL_SUCC;
}
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)

OAL_STATIC oal_int32 wal_host_dev_config(oal_net_device_stru *pst_net_dev, wlan_cfgid_enum_uint16 en_wid)
{
    oal_wireless_dev_stru *pst_wdev = OAL_PTR_NULL;
    mac_wiphy_priv_stru *pst_wiphy_priv = OAL_PTR_NULL;
    hmac_vap_stru *pst_cfg_hmac_vap = OAL_PTR_NULL;
    mac_device_stru *pst_mac_device = OAL_PTR_NULL;
    oal_net_device_stru *pst_cfg_net_dev = OAL_PTR_NULL;

    oal_int32 l_ret;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;
    oal_uint32 ul_err_code;
    wal_msg_write_stru st_write_msg;

    if (pst_net_dev == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_host_dev_config::pst_net_dev is null!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }
    pst_wdev = OAL_NETDEVICE_WDEV(pst_net_dev);
    if (pst_wdev == NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_host_dev_config::pst_wdev is null!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_wiphy_priv = (mac_wiphy_priv_stru *)oal_wiphy_priv(pst_wdev->wiphy);
    if (pst_wiphy_priv == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_host_dev_config::pst_mac_device is null!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }
    pst_mac_device = pst_wiphy_priv->pst_mac_device;
    if (pst_mac_device == NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_host_dev_config::pst_mac_device is null!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_cfg_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_device->uc_cfg_vap_id);
    if (pst_cfg_hmac_vap == NULL) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_host_dev_config::pst_cfg_hmac_vap is null vap_id:%d!}\r\n",
                         pst_mac_device->uc_cfg_vap_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_cfg_net_dev = pst_cfg_hmac_vap->pst_net_device;
    if (pst_cfg_net_dev == NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_host_dev_config::pst_cfg_net_dev is null!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /***************************************************************************
    ���¼���wal�㴦��
    ***************************************************************************/
    /* ��д��Ϣ */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, en_wid, 0);

    /* ������Ϣ */
    l_ret = wal_send_cfg_event(pst_cfg_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH,
                               (oal_uint8 *)&st_write_msg,
                               OAL_TRUE,
                               &pst_rsp_msg);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_cfg_vap_h2d_event::wal_alloc_cfg_event return err code %d!}\r\n", l_ret);
        return l_ret;
    }

    /* ��������Ϣ */
    ul_err_code = wal_check_and_release_msg_resp(pst_rsp_msg);
    if (ul_err_code != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_cfg_vap_h2d_event::hmac cfg vap h2d fail, err code[%u]\r\n", ul_err_code);
        return -OAL_EINVAL;
    }

    return OAL_SUCC;
}


oal_int32 wal_host_dev_init(oal_net_device_stru *pst_net_dev)
{
    return wal_host_dev_config(pst_net_dev, WLAN_CFGID_HOST_DEV_INIT);
}


oal_int32 wal_host_dev_exit(oal_net_device_stru *pst_net_dev)
{
    return wal_host_dev_config(pst_net_dev, WLAN_CFGID_HOST_DEV_EXIT);
}
#endif

#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE

oal_void hwifi_config_init_base_power_main(oal_net_device_stru *pst_net_dev)
{
    wlan_customize_power_params_stru *pst_nvram;
    wal_msg_write_stru st_write_msg;
    oal_int32 l_ret;

    pst_nvram = hwifi_get_nvram_params();

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_CUS_BASE_POWER, OAL_SIZEOF(oal_uint8) * CUS_BASE_POWER_NUM);
    if (memcpy_s(st_write_msg.auc_value, WAL_MSG_WRITE_MAX_LEN, pst_nvram->auc_base_power,
        OAL_SIZEOF(oal_uint8) * CUS_BASE_POWER_NUM) != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "hwifi_config_init_base_power_main:memcpy fail");
    }

    if (hitalk_status) {
        /* խ��ģʽ��2.4Gʹ��խ���������� */
        st_write_msg.auc_value[0] = pst_nvram->uc_nb_base_power;
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "hwifi_config_init_base_power_main:nb base power[%d]",
                         st_write_msg.auc_value[0]);
    }

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(oal_uint8) * CUS_BASE_POWER_NUM,
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "{hwifi_config_init_base_power_main::return err code [%d]!}\r\n", l_ret);
        return;
    }

    return;
}


oal_void hwifi_config_init_5g_high_band_max_power_main(oal_net_device_stru *pst_net_dev)
{
    wlan_customize_power_params_stru *pst_nvram;
    wal_msg_write_stru st_write_msg;
    oal_int32 l_ret;

    pst_nvram = hwifi_get_nvram_params();

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_5G_HIGH_BAND_MAX_POW, OAL_SIZEOF(oal_uint8));
    if (memcpy_s(st_write_msg.auc_value, OAL_SIZEOF(st_write_msg.auc_value),
        &pst_nvram->uc_5g_high_band_max_pow, OAL_SIZEOF(oal_uint8)) != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "hwifi_config_init_5g_high_band_max_power_main::memcpy fail");
    }

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(oal_uint8),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "hwifi_config_init_5g_high_band_max_power_main::return err code [%d]", l_ret);
        return;
    }

    return;
}


oal_void hwifi_config_init_fcc_ce_power_main(oal_net_device_stru *pst_net_dev)
{
    wlan_customize_power_params_stru *pst_nvram;
    wal_msg_write_stru st_write_msg;
    oal_int32 l_ret;
    regdomain_enum en_regdomain;

    en_regdomain = hwifi_get_regdomain_from_country_code(hwifi_get_country_code());
    if ((en_regdomain != REGDOMAIN_FCC) && (en_regdomain != REGDOMAIN_ETSI)) {
        OAM_WARNING_LOG0(0, 0, "hwifi_config_init_fcc_ce_power_main:the country is not a fcc or ce country");
        return;
    }
    pst_nvram = hwifi_get_nvram_params();
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_CUS_FCC_CE_POWER, OAL_SIZEOF(wlan_fcc_ce_power_limit_stru));
    if (memcpy_s(st_write_msg.auc_value, OAL_SIZEOF(st_write_msg.auc_value),
        &pst_nvram->st_fcc_ce_txpwer_limit, OAL_SIZEOF(wlan_fcc_ce_power_limit_stru))) {
        OAM_ERROR_LOG0(0, 0, "hwifi_config_init_fcc_ce_power_main:memcpy fail.");
    }
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(wlan_fcc_ce_power_limit_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (l_ret != OAL_SUCC) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "{hwifi_config_init_fcc_ce_power_main::return err code [%d]!}\r\n", l_ret);
        return;
    }
    return;
}

/*
 * �� �� ��  : hwifi_config_init_ext_fcc_ce_power
 * ��������  : �·��αߴ����ʿ���ֵ
 * �� �� ֵ  : OAL_SUCC �� ʧ�ܴ�����
 */
oal_void hwifi_config_init_ext_fcc_ce_power(oal_net_device_stru *pst_net_dev)
{
    wlan_customize_power_params_stru *pst_nvram;
    wal_msg_write_stru st_write_msg;
    oal_int32 l_ret;
    regdomain_enum en_regdomain;

    en_regdomain = hwifi_get_regdomain_from_country_code(hwifi_get_country_code());
    if ((en_regdomain != REGDOMAIN_FCC) && (en_regdomain != REGDOMAIN_ETSI)) {
        return;
    }
    pst_nvram = hwifi_get_nvram_params();
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_EXT_FCC_CE_POWER,
                           OAL_SIZEOF(wlan_ext_fcc_ce_power_limit_stru));
    l_ret = memcpy_s(st_write_msg.auc_value, WAL_MSG_WRITE_MAX_LEN, &pst_nvram->st_ext_fcc_ce_txpwer_limit,
                     OAL_SIZEOF(wlan_ext_fcc_ce_power_limit_stru));
    if (l_ret != EOK) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "hwifi_config_init_ext_fcc_ce_power:memcpy fail, l_ret=%d", l_ret);
        return;
    }
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(wlan_ext_fcc_ce_power_limit_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (l_ret != OAL_SUCC) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "{hwifi_config_init_ext_fcc_ce_power::return err code [%d]!}\r\n", l_ret);
    }
    return;
}


OAL_STATIC oal_int32 hwifi_force_refresh_rf_params(oal_net_device_stru *pst_net_dev)
{
    /* update params */
    if (hwifi_config_init(CUS_TAG_NV)) {
        return OAL_FAIL;
    }

    /* ����base power */
    hwifi_config_init_base_power_main(pst_net_dev);

    /* ����CE����Ҫ���5G��band��������ֵ,��CE�����·�Ĭ��ֵ */
    hwifi_config_init_5g_high_band_max_power_main(pst_net_dev);

    /* �����ƻ�������Ϣ�·���device */
    hwifi_config_init_fcc_ce_power_main(pst_net_dev);
    hwifi_config_init_ext_fcc_ce_power(pst_net_dev);
    /* send data to device */
    return hwifi_config_init_nvram_main(pst_net_dev);
}


OAL_STATIC oal_void hwifi_config_tcp_ack_ini_param(oal_void)
{
    oal_int32 l_val;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DEVICE_RX_TCP_ACK_FILTER);
    g_en_tcp_ack_filter_switch = (l_val >= 0) ? (oal_bool_enum_uint8)l_val : OAL_FALSE;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DEVICE_RX_TCP_ACK_2G_TH);
    g_ul_2G_tx_large_pps_th = (l_val >= 0) ? l_val : WLAN_DEVICE_RX_TCP_ACK_DEFAULT_2G_TH;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DEVICE_RX_TCP_ACK_5G_TH);
    g_ul_5G_tx_large_pps_th = (l_val >= 0) ? l_val : WLAN_DEVICE_RX_TCP_ACK_DEFAULT_5G_TH;
}


OAL_STATIC oal_void hwifi_config_ddr_cpu_freq_ini_param(oal_void)
{
    oal_int32 l_val;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_BINDCPU);
    g_st_thread_bindcpu.en_irq_bindcpu = (l_val > 0) ? OAL_TRUE : OAL_FALSE;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_BINDCPU_MASK);
    g_st_thread_bindcpu.uc_cpumask = (l_val > 0) ? (oal_uint8)l_val : WLAN_BINDCPU_DEFAULT_MASK;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_BINDCPU_PPSHIGH);
    g_st_thread_bindcpu.us_throughput_pps_irq_high = (l_val > 0) ? (oal_uint16)l_val : WLAN_BUSY_CPU_PPS_THROUGHT;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_BINDCPU_PPSLOW);
    g_st_thread_bindcpu.us_throughput_pps_irq_low = (l_val > 0) ? (oal_uint16)l_val : WLAN_IDLE_CPU_PPS_THROUGHT;
    OAL_IO_PRINT("bindcpu en[%d], high[%d], low[%d]\r\n",
                 g_st_thread_bindcpu.en_irq_bindcpu, g_st_thread_bindcpu.us_throughput_pps_irq_high,
                 g_st_thread_bindcpu.us_throughput_pps_irq_low);

    g_st_auto_ddr_freq_mgmt.ul_auto_ddr_freq_pps_th_high = g_st_thread_bindcpu.us_throughput_pps_irq_high;
    g_st_auto_ddr_freq_mgmt.ul_auto_ddr_freq_pps_th_low = g_st_thread_bindcpu.us_throughput_pps_irq_low;
}


OAL_STATIC oal_void hwifi_config_txopt_opt_ini_param(oal_void)
{
    oal_int32 l_val;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TX_OPT_SWITCH_STA);
    g_st_wifi_tx_opt.uc_tx_opt_switch_sta = (l_val >= 0) ? (oal_uint8)l_val : WLAN_TX_OPT_SWITCH_STA;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TX_OPT_SWITCH_AP);
    g_st_wifi_tx_opt.uc_tx_opt_switch_ap = (l_val >= 0) ? (oal_uint8)l_val : WLAN_TX_OPT_SWITCH_AP;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TX_OPT_PPS_HIGH);
    g_st_wifi_tx_opt.us_txopt_th_pps_high = (l_val >= 0) ? (oal_uint16)l_val : WLAN_TX_OPT_TH_HIGH;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TX_OPT_PPS_LOW);
    g_st_wifi_tx_opt.us_txopt_th_pps_low = (l_val >= 0) ? (oal_uint16)l_val : WLAN_TX_OPT_TH_LOW;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TX_OPT_CWMIN);
    g_st_wifi_tx_opt.uc_cwmin = (l_val >= 0) ? (oal_uint8)l_val : WLAN_TX_OPT_CWMIN;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TX_OPT_CWMAX);
    g_st_wifi_tx_opt.uc_cwmax = (l_val >= 0) ? (oal_uint8)l_val : WLAN_TX_OPT_CWMAX;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TX_OPT_TXOPLIMIT);
    g_st_wifi_tx_opt.us_txoplimit = (l_val >= 0) ? (oal_uint16)l_val : WLAN_TX_OPT_TXOPLIMIT;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TX_OPT_DYN_TXOPLIMIT);
    g_st_wifi_tx_opt.us_dyn_txoplimit = (l_val >= 0) ? (oal_uint16)l_val : WLAN_TX_OPT_DYN_TXOPLIMIT;

    OAL_IO_PRINT("hwifi_config_txopt_opt_ini_param::uc_pk_opt_switch:sta[%d]/ap[%d], pps_high[%d], pps_low[%d]\r\n",
                 g_st_wifi_tx_opt.uc_tx_opt_switch_sta, g_st_wifi_tx_opt.uc_tx_opt_switch_ap,
                 g_st_wifi_tx_opt.us_txopt_th_pps_high, g_st_wifi_tx_opt.us_txopt_th_pps_low);

    OAL_IO_PRINT("hwifi_config_txopt_opt_ini_param::cwmin[%d], cwmax[%d], txop[%d], dyn_txop[%d]\r\n",
                 g_st_wifi_tx_opt.uc_cwmin, g_st_wifi_tx_opt.uc_cwmax, g_st_wifi_tx_opt.us_txoplimit,
                 g_st_wifi_tx_opt.us_dyn_txoplimit);
}

oal_void hwifi_config_init_tcp_ack_buf_params(oal_void)
{
    oal_int32 l_val;
    mac_tcp_ack_buf_switch_stru *pst_tcp_ack_buf_switch = mac_get_tcp_ack_buf_switch_addr();

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TX_TCP_ACK_BUF);
    pst_tcp_ack_buf_switch->uc_ini_tcp_ack_buf_en = (l_val > 0) ? OAL_TRUE : OAL_FALSE;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TCP_ACK_BUF_HIGH);
    pst_tcp_ack_buf_switch->us_tcp_ack_buf_throughput_high =
        (l_val > 0) ? (oal_uint16)l_val : WLAN_TCP_ACK_BUF_THROUGHPUT_THRESHOLD_HIGH;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TCP_ACK_BUF_LOW);
    pst_tcp_ack_buf_switch->us_tcp_ack_buf_throughput_low =
        (l_val > 0) ? (oal_uint16)l_val : WLAN_TCP_ACK_BUF_THROUGHPUT_THRESHOLD_LOW;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TCP_ACK_BUF_HIGH_40M);
    pst_tcp_ack_buf_switch->us_tcp_ack_buf_throughput_high_40M =
        (l_val > 0) ? (oal_uint16)l_val : WLAN_TCP_ACK_BUF_THROUGHPUT_THRESHOLD_HIGH_40M;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TCP_ACK_BUF_LOW_40M);
    pst_tcp_ack_buf_switch->us_tcp_ack_buf_throughput_low_40M =
        (l_val > 0) ? (oal_uint16)l_val : WLAN_TCP_ACK_BUF_THROUGHPUT_THRESHOLD_LOW_40M;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TCP_ACK_BUF_HIGH_80M);
    pst_tcp_ack_buf_switch->us_tcp_ack_buf_throughput_high_80M =
        (l_val > 0) ? (oal_uint16)l_val : WLAN_TCP_ACK_BUF_THROUGHPUT_THRESHOLD_HIGH_80M;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TCP_ACK_BUF_LOW_80M);
    pst_tcp_ack_buf_switch->us_tcp_ack_buf_throughput_low_80M =
        (l_val > 0) ? (oal_uint16)l_val : WLAN_TCP_ACK_BUF_THROUGHPUT_THRESHOLD_LOW_80M;
    OAL_IO_PRINT("TCP ACK BUF en[%d], 20M_high[%d], 20M_low[%d], 40M_high[%d], 40M_low[%d], 80M_high[%d], 80M_low[%d]",
                 pst_tcp_ack_buf_switch->uc_ini_tcp_ack_buf_en,
                 pst_tcp_ack_buf_switch->us_tcp_ack_buf_throughput_high,
                 pst_tcp_ack_buf_switch->us_tcp_ack_buf_throughput_low,
                 pst_tcp_ack_buf_switch->us_tcp_ack_buf_throughput_high_40M,
                 pst_tcp_ack_buf_switch->us_tcp_ack_buf_throughput_low_40M,
                 pst_tcp_ack_buf_switch->us_tcp_ack_buf_throughput_high_80M,
                 pst_tcp_ack_buf_switch->us_tcp_ack_buf_throughput_low_80M);
}
oal_void hwifi_config_host_global_ini_roam(oal_void)
{
#ifdef _PRE_WLAN_FEATURE_ROAM
    oal_int32 l_val;
    /******************************************** ���� ********************************************/
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_ROAM_SWITCH);
    wlan_customize.uc_roam_switch = ((l_val == 0) || (l_val == 1)) ? (oal_uint8)l_val : wlan_customize.uc_roam_switch;
    wlan_customize.uc_roam_scan_band = (mac_get_band_5g_enabled()) ? (BIT0 | BIT1) : BIT0;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_SCAN_ORTHOGONAL);
    wlan_customize.uc_roam_scan_orthogonal = (l_val >= 1) ? (oal_uint8)l_val : wlan_customize.uc_roam_scan_orthogonal;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TRIGGER_B);
    wlan_customize.c_roam_trigger_b = (oal_int8)l_val;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TRIGGER_A);
    wlan_customize.c_roam_trigger_a = (oal_int8)l_val;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DELTA_B);
    wlan_customize.c_roam_delta_b = (oal_int8)l_val;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DELTA_A);
    wlan_customize.c_roam_delta_a = (oal_int8)l_val;
#endif /* #ifdef _PRE_WLAN_FEATURE_ROAM */
}
OAL_STATIC oal_void hwifi_config_lte_gpio_ini_param(oal_void)
{
    oal_int32 l_val;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_LTE_GPIO_CHECK_SWITCH);
    wlan_customize.ul_lte_gpio_check_switch = (oal_uint32) !!l_val;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_ATCMDSRV_LTE_ISM_PRIORITY);
    wlan_customize.ul_lte_ism_priority = (oal_uint32)l_val;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_ATCMDSRV_LTE_RX_ACT);
    wlan_customize.ul_lte_rx_act = (oal_uint32)l_val;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_ATCMDSRV_LTE_TX_ACT);
    wlan_customize.ul_lte_tx_act = (oal_uint32)l_val;
}
OAL_STATIC oal_void hwifi_config_multi_netbuf_amsdu_ini_param(oal_void)
{
#ifdef _PRE_WLAN_FEATURE_MULTI_NETBUF_AMSDU
    oal_int32 l_val;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_AMPDU_AMSDU_SKB);
    g_tx_large_amsdu.uc_tx_amsdu_ampdu_en = (l_val > 0) ? OAL_TRUE : OAL_FALSE;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_AMSDU_AMPDU_TH_HIGH);
    g_tx_large_amsdu.us_amsdu_ampdu_throughput_high =
        (l_val > 0) ? (oal_uint16)l_val : WLAN_AMSDU_AMPDU_THROUGHPUT_THRESHOLD_HIGH;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_AMSDU_AMPDU_TH_LOW);
    g_tx_large_amsdu.us_amsdu_ampdu_throughput_low =
        (l_val > 0) ? (oal_uint16)l_val : WLAN_AMSDU_AMPDU_THROUGHPUT_THRESHOLD_LOW;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RX_AMPDU_AMSDU_SKB);
    g_uc_host_rx_ampdu_amsdu = (l_val > 0) ? OAL_TRUE : OAL_FALSE;
#endif
}
OAL_STATIC oal_void hwifi_config_tcp_opt_ini_param(oal_void)
{
#ifdef _PRE_WLAN_TCP_OPT
    oal_int32 l_val;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TCP_ACK_FILTER);
    g_st_tcp_ack_opt_th_params.uc_tcp_ack_filter_en = (l_val > 0) ? OAL_TRUE : OAL_FALSE;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TCP_ACK_FILTER_TH_HIGH);
    g_st_tcp_ack_opt_th_params.us_rx_filter_throughput_high =
        (l_val > 0) ? (oal_uint16)l_val : WLAN_TCP_ACK_FILTER_TH_HIGH;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TCP_ACK_FILTER_TH_LOW);
    g_st_tcp_ack_opt_th_params.us_rx_filter_throughput_low =
        (l_val > 0) ? (oal_uint16)l_val : WLAN_TCP_ACK_FILTER_TH_LOW;
    OAL_IO_PRINT("tcp ack filter en[%d], high[%d], low[%d]\r\n",
                 g_st_tcp_ack_opt_th_params.uc_tcp_ack_filter_en,
                 g_st_tcp_ack_opt_th_params.us_rx_filter_throughput_high,
                 g_st_tcp_ack_opt_th_params.us_rx_filter_throughput_low);
#endif
}
OAL_STATIC oal_void hwifi_config_dyn_bypass_extlna_ini_param(oal_void)
{
    oal_int32 l_val;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RX_DYN_BYPASS_EXTLNA);
    g_st_rx_dyn_bypass_extlna_switch.uc_ini_en = (l_val > 0) ? OAL_TRUE : OAL_FALSE;
    g_st_rx_dyn_bypass_extlna_switch.uc_cur_status = OAL_TRUE; /* Ĭ�ϵ͹��ĳ��� */
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RX_DYN_BYPASS_EXTLNA_HIGH);
    g_st_rx_dyn_bypass_extlna_switch.us_throughput_high =
        (l_val > 0) ? (oal_uint16)l_val : WLAN_DYN_BYPASS_EXTLNA_THROUGHPUT_THRESHOLD_HIGH;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RX_DYN_BYPASS_EXTLNA_LOW);
    g_st_rx_dyn_bypass_extlna_switch.us_throughput_low =
        (l_val > 0) ? (oal_uint16)l_val : WLAN_DYN_BYPASS_EXTLNA_THROUGHPUT_THRESHOLD_LOW;
    OAL_IO_PRINT("DYN_BYPASS_EXTLNA SWITCH en[%d], high[%d], low[%d]\r\n",
                 g_st_rx_dyn_bypass_extlna_switch.uc_ini_en, g_st_rx_dyn_bypass_extlna_switch.us_throughput_high,
                 g_st_rx_dyn_bypass_extlna_switch.us_throughput_low);
}
OAL_STATIC oal_void hwifi_config_small_amsdu_ini_param(oal_void)
{
    oal_int32 l_val;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TX_SMALL_AMSDU);
    g_st_small_amsdu_switch.uc_ini_small_amsdu_en = (l_val > 0) ? OAL_TRUE : OAL_FALSE;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_SMALL_AMSDU_HIGH);
    g_st_small_amsdu_switch.us_small_amsdu_throughput_high =
        (l_val > 0) ? (oal_uint16)l_val : WLAN_SMALL_AMSDU_THROUGHPUT_THRESHOLD_HIGH;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_SMALL_AMSDU_LOW);
    g_st_small_amsdu_switch.us_small_amsdu_throughput_low =
        (l_val > 0) ? (oal_uint16)l_val : WLAN_SMALL_AMSDU_THROUGHPUT_THRESHOLD_LOW;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_SMALL_AMSDU_PPS_HIGH);
    g_st_small_amsdu_switch.us_small_amsdu_pps_high =
        (l_val > 0) ? (oal_uint16)l_val : WLAN_SMALL_AMSDU_PPS_THRESHOLD_HIGH;
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_SMALL_AMSDU_PPS_LOW);
    g_st_small_amsdu_switch.us_small_amsdu_pps_low =
        (l_val > 0) ? (oal_uint16)l_val : WLAN_SMALL_AMSDU_PPS_THRESHOLD_LOW;
    OAL_IO_PRINT("SMALL AMSDU SWITCH en[%d], high_throughput [%d], low_throughput [%d], pps_high [%d], pps_low [%d]...",
                 g_st_small_amsdu_switch.uc_ini_small_amsdu_en,
                 g_st_small_amsdu_switch.us_small_amsdu_throughput_high,
                 g_st_small_amsdu_switch.us_small_amsdu_throughput_low,
                 g_st_small_amsdu_switch.us_small_amsdu_pps_high, g_st_small_amsdu_switch.us_small_amsdu_pps_low);
}
OAL_STATIC oal_void hwifi_config_random_mac_addr_scan_ini_param(oal_void)
{
    oal_int32 l_val;
    /* ���MAC��ַ ɨ��,���� */
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RANDOM_MAC_ADDR_SCAN);
    wlan_customize.uc_random_mac_addr_scan = !!l_val;

    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RANDOM_MAC_ADDR_CONNECT);
    wlan_customize.en_random_mac_addr_connect = !!l_val;
}
OAL_STATIC oal_void hwifi_config_tas_switch_ini_param(oal_void)
{
    oal_int32 l_val;
    /* TAS�����л� */
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_TAS_ANT_SWITCH);
    g_aen_tas_switch_en[WITP_RF_CHANNEL_ZERO] = !!l_val;
    OAL_IO_PRINT("TAS SWITCH en[%d]\r\n", g_aen_tas_switch_en[WITP_RF_CHANNEL_ZERO]);
    /******************************************** CAPABILITY ********************************************/
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DISABLE_CAPAB_2GHT40);
    wlan_customize.uc_disable_capab_2ght40 = (oal_uint8) !!l_val;
}

OAL_STATIC oal_int32 hwifi_config_host_global_ini_param(oal_void)
{
    oal_int32 l_val;
#ifdef _PRE_WLAN_FEATURE_AUTO_FREQ
    oal_uint32 cfg_id;
    oal_int32 l_cfg_value;
    oal_int8 *pc_tmp;
    device_speed_freq_level_stru ast_device_speed_freq_level_tmp[4];
    host_speed_freq_level_stru ast_host_speed_freq_level_tmp[4];
    oal_uint8 uc_flag = OAL_FALSE;
    oal_uint8 uc_index;
    hmac_vap_stru *pst_cfg_hmac_vap;
    oal_net_device_stru *pst_cfg_net_dev;
    wal_msg_write_stru st_write_msg;
    oal_int32 l_ret;
#endif /* #ifdef _PRE_WLAN_FEATURE_AUTO_FREQ */
    device_speed_freq_level_stru *pst_device_speed_freq_level = hmac_wifi_get_device_speed_freq_level_addr();
    host_speed_freq_level_stru *pst_host_speed_freq_level = hmac_wifi_get_host_speed_freq_level_addr();
    /******************************************** ���� ********************************************/
    l_val = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_AMPDU_TX_MAX_NUM);
    wlan_customize.ul_ampdu_tx_max_num =
        ((l_val <= WLAN_AMPDU_TX_MAX_NUM) && (l_val >= 1)) ? (oal_uint32)l_val : wlan_customize.ul_ampdu_tx_max_num;
    OAL_IO_PRINT("hisi_customize_wifi::ampdu_tx_max_num:%d", wlan_customize.ul_ampdu_tx_max_num);
    hwifi_config_host_global_ini_roam();

#ifdef _PRE_WLAN_FEATURE_AUTO_FREQ
    /******************************************** �Զ���Ƶ ********************************************/
    /* config pps_value */
    uc_index = 0;
    for (cfg_id = WLAN_CFG_INIT_PPS_THRESHOLD_LEVEL_0; cfg_id <= WLAN_CFG_INIT_PPS_THRESHOLD_LEVEL_3; ++cfg_id) {
        l_cfg_value = hwifi_get_init_value(CUS_TAG_INI, cfg_id);
        if ((l_cfg_value >= PPS_VALUE_MIN) && (l_cfg_value <= PPS_VALUE_MAX)) {
            ast_host_speed_freq_level_tmp[uc_index].ul_speed_level = l_cfg_value;
            uc_index++;
        } else {
            uc_flag = OAL_TRUE;
            OAM_WARNING_LOG1(0, OAM_SF_CFG, "hwifi_config_host_global_ini_param :: The pps value [%d] is out of range",
                             l_cfg_value);
            break;
        }
    }

    /* config g_device_speed_freq_level */
    pc_tmp = (oal_int8 *)&ast_device_speed_freq_level_tmp;
    for (cfg_id = WLAN_CFG_INIT_DEVICE_TYPE_LEVEL_0; cfg_id <= WLAN_CFG_INIT_DEVICE_TYPE_LEVEL_3; ++cfg_id) {
        l_cfg_value = hwifi_get_init_value(CUS_TAG_INI, cfg_id);
        if ((l_cfg_value >= FREQ_IDLE) && (l_cfg_value <= FREQ_HIGHEST)) {
            *pc_tmp = l_cfg_value;
            pc_tmp += 4; /* pc_tmpָ��ini���ƻ���������al_host_init_params����4������ */
        } else {
            uc_flag = OAL_TRUE;
            break;
        }
    }

    if (!uc_flag) {
        /* ֻ�������������ṹ��������Ԫ�ض���ȷ����ֵ�ŻὫ���϶��ƻ���ֵ�䵽device�Զ���Ƶ�У����򱣳�Ĭ��ֵ */
        if (memcpy_s(pst_host_speed_freq_level, hmac_wifi_get_host_speed_freq_level_size(),
            &ast_host_speed_freq_level_tmp, hmac_wifi_get_host_speed_freq_level_size()) != EOK) {
            OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hwifi_config_host_global_ini_param::memcpy_s fail!}\r\n");
        }

        if (memcpy_s(pst_device_speed_freq_level, hmac_wifi_get_device_speed_freq_level_size(),
            &ast_device_speed_freq_level_tmp, hmac_wifi_get_device_speed_freq_level_size()) != EOK) {
            OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hwifi_config_host_global_ini_param::memcpy_s fail!!}\r\n");
        }

        for (uc_index = 0; uc_index < 4; uc_index++) { /* ast_host_speed_freq_level_tmp�ṹ�����鳤��Ϊ4 */
            OAM_WARNING_LOG2(0, OAM_SF_ANY, "hwifi_config_host_global_ini_param:uc_device_type=[%d], pps_value = [%d]}",
                             pst_device_speed_freq_level[uc_index].uc_device_type,
                             pst_host_speed_freq_level[uc_index].ul_speed_level);
        }

        /* �·����ƻ�������device */
        pst_cfg_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(0);
        if (pst_cfg_hmac_vap == OAL_PTR_NULL) {
            OAM_WARNING_LOG0(0, OAM_SF_ANY, "{hwifi_config_host_global_ini_param::pst_cfg_hmac_vap is null!}\r\n");
            return -OAL_EFAUL;
        }

        pst_cfg_net_dev = pst_cfg_hmac_vap->pst_net_device;
        if (pst_cfg_net_dev == OAL_PTR_NULL) {
            OAM_WARNING_LOG0(0, OAM_SF_ANY, "{hwifi_config_host_global_ini_param::pst_cfg_net_dev is null!}\r\n");
            return -OAL_EFAUL;
        }

        WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_DEVICE_FREQ_VALUE, OAL_SIZEOF(oal_int32));
        *((oal_int8 *)(st_write_msg.auc_value)) = OAL_TRUE;

        l_ret = wal_send_cfg_event(pst_cfg_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(oal_int32),
                                   (oal_uint8 *)&st_write_msg,
                                   OAL_FALSE, OAL_PTR_NULL);
        if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
            OAM_ERROR_LOG1(0, OAM_SF_ANY, "{hwifi_config_host_global_ini_param::return err code [%d]!}\r\n", l_ret);
        }
    }
#endif /* #ifdef _PRE_WLAN_FEATURE_AUTO_FREQ */
    hwifi_config_random_mac_addr_scan_ini_param();
    hwifi_config_tas_switch_ini_param();
    /********************************************factory_lte_gpio_check ********************************************/
    hwifi_config_lte_gpio_ini_param();
    hwifi_config_multi_netbuf_amsdu_ini_param();
    hwifi_config_tcp_opt_ini_param();
    hwifi_config_tcp_ack_ini_param();
    hwifi_config_ddr_cpu_freq_ini_param();
    hwifi_config_txopt_opt_ini_param();

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    hwifi_config_init_tcp_ack_buf_params();
    hwifi_config_dyn_bypass_extlna_ini_param();
    hwifi_config_small_amsdu_ini_param();
#endif

    return OAL_SUCC;
}

OAL_STATIC oal_void hwifi_config_init_ini_perf(oal_net_device_stru *pst_cfg_net_dev)
{
    wal_msg_write_stru st_write_msg;
    oal_int32 l_ret;
    oal_int8 pc_param[18] = { 0 };
    oal_int8 pc_tmp[8] = { 0 };
    oal_uint16 us_len;
    oal_uint8 uc_sdio_assem_h2d;
    oal_uint8 uc_sdio_assem_d2h;

    /* SDIO FLOWCTRL */
    // device�����Ϸ����ж�
    /* ����ת�ַ����������ַ�������Ϊ8 */
    oal_itoa(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_USED_MEM_FOR_START), pc_param, 8);
    /* ����ת�ַ����������ַ�������Ϊ8 */
    oal_itoa(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_USED_MEM_FOR_STOP), pc_tmp, 8);
    pc_param[OAL_STRLEN(pc_param)] = ' ';
    if (memcpy_s(pc_param + OAL_STRLEN(pc_param), OAL_SIZEOF(pc_param), pc_tmp, OAL_STRLEN(pc_tmp)) != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hwifi_config_init_ini_perf::memcpy_s fail!}\r\n");
        return;
    }

    if (memcpy_s(st_write_msg.auc_value, OAL_SIZEOF(st_write_msg.auc_value), pc_param, OAL_STRLEN(pc_param)) != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hwifi_config_init_ini_perf::memcpy_s fail!}\r\n");
        return;
    }
    st_write_msg.auc_value[OAL_STRLEN(pc_param)] = '\0';
    us_len = (oal_uint16)(OAL_STRLEN(pc_param) + 1);

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SDIO_FLOWCTRL, us_len);

    l_ret = wal_send_cfg_event(pst_cfg_net_dev, WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + us_len, (oal_uint8 *)&st_write_msg,
                               OAL_FALSE, OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "{hwifi_config_init_ini_perf::return err code [%d]!}\r\n", l_ret);
    }

    /* SDIO ASSEMBLE COUNT:H2D */
    uc_sdio_assem_h2d = (oal_uint8)hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_SDIO_H2D_ASSEMBLE_COUNT);
    // �ж�ֵ�ĺϷ���
    if ((uc_sdio_assem_h2d >= 1) && (uc_sdio_assem_h2d <= HISDIO_HOST2DEV_SCATT_MAX)) {
        hcc_set_assemble_count(uc_sdio_assem_h2d);
    } else {
        OAM_ERROR_LOG2(0, OAM_SF_ANY, "{hwifi_config_init_ini_perf::sdio_assem_h2d[%d] out of range(0, %d]",
                       uc_sdio_assem_h2d, HISDIO_HOST2DEV_SCATT_MAX);
    }

    /* SDIO ASSEMBLE COUNT:D2H */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_D2H_HCC_ASSEMBLE_CNT, OAL_SIZEOF(oal_int32));
    uc_sdio_assem_d2h = (oal_uint8)hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_SDIO_D2H_ASSEMBLE_COUNT);
    // �ж�ֵ�ĺϷ���
    if ((uc_sdio_assem_d2h >= 1) && (uc_sdio_assem_d2h <= HISDIO_DEV2HOST_SCATT_MAX)) {
        *((oal_int32 *)(st_write_msg.auc_value)) = uc_sdio_assem_d2h;
    } else {
        *((oal_int32 *)(st_write_msg.auc_value)) = HISDIO_DEV2HOST_SCATT_MAX;
        OAM_ERROR_LOG2(0, OAM_SF_ANY, "hwifi_config_init_ini_perf::sdio_assem_d2h[%d] out of range(0, %d]",
                       uc_sdio_assem_d2h, HISDIO_DEV2HOST_SCATT_MAX);
    }

    l_ret = wal_send_cfg_event(pst_cfg_net_dev, WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(oal_int32),
                               (oal_uint8 *)&st_write_msg, OAL_FALSE, OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "{hwifi_config_init_ini_perf::return err code [%d]!}\r\n", l_ret);
    }
}

OAL_STATIC oal_void hwifi_config_init_ini_linkloss(oal_net_device_stru *pst_cfg_net_dev)
{
    wal_msg_write_stru st_write_msg;
    oal_int32 l_ret;

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_LINKLOSS_THRESHOLD, OAL_SIZEOF(mac_cfg_linkloss_threshold));
    ((mac_cfg_linkloss_threshold *)(st_write_msg.auc_value))->uc_linkloss_threshold_wlan_bt =
        (oal_uint8)hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_LINK_LOSS_THRESHOLD_WLAN_BT);
    ((mac_cfg_linkloss_threshold *)(st_write_msg.auc_value))->uc_linkloss_threshold_wlan_dbac =
        (oal_uint8)hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_LINK_LOSS_THRESHOLD_WLAN_DBAC);
    ((mac_cfg_linkloss_threshold *)(st_write_msg.auc_value))->uc_linkloss_threshold_wlan_normal =
        (oal_uint8)hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_LINK_LOSS_THRESHOLD_WLAN_NORMAL);

    l_ret = wal_send_cfg_event(pst_cfg_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_linkloss_threshold),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "hwifi_config_init_ini_linkloss:wal_send_cfg_event return err code [%d]", l_ret);
    }
}

OAL_STATIC oal_void hwifi_config_init_ini_country(oal_net_device_stru *pst_cfg_net_dev)
{
    oal_int32 l_ret;

    l_ret = (oal_int32)wal_hipriv_setcountry(pst_cfg_net_dev, hwifi_get_country_code());
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "hwifi_config_init_ini_country::wal_send_cfg_event return err code [%d]", l_ret);
    }
}

OAL_STATIC oal_void hwifi_config_init_ini_pm(oal_net_device_stru *pst_cfg_net_dev)
{
    wal_msg_write_stru st_write_msg;
    oal_int32 l_ret;
    oal_int32 l_switch;

    /*************************** �͹���ps_mode���ƻ� *****************************/
    hmac_config_set_ps_mode(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_PS_MODE));
    /*************************** ˽�ж��ƻ� *******************************/
    /* ���� */
    l_switch = !!hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_POWERMGMT_SWITCH);

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_PM_SWITCH, OAL_SIZEOF(oal_int32));
    *((oal_int32 *)(st_write_msg.auc_value)) = l_switch;

    l_ret = wal_send_cfg_event(pst_cfg_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(oal_int32),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "{hwifi_config_init_ini_pm::return err code [%d]!}\r\n", l_ret);
    }
}

OAL_STATIC oal_void hwifi_config_init_interworking(oal_net_device_stru *pst_cfg_net_dev)
{
    wal_msg_write_stru st_write_msg;
    oal_int32 l_ret;
    oal_int32 l_switch;

    /* ���� */
    l_switch = !!hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_INTERWORKING_SWITCH);

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_INTERWORKING_SWITCH, OAL_SIZEOF(oal_int32));
    *((oal_int32 *)(st_write_msg.auc_value)) = l_switch;

    l_ret = wal_send_cfg_event(pst_cfg_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(oal_int32),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "{hwifi_config_init_interworking::return err code [%d]!}\r\n", l_ret);
    }
}

#ifdef _PRE_WLAN_FEATURE_NRCOEX

oal_void hwifi_config_init_nrcoex_params(oal_net_device_stru *pst_net_dev)
{
    wal_msg_write_stru st_write_msg;
    oal_uint32 l_ret;
    wlan_nrcoex_ini_stru *pst_nrcoex_ini;

    pst_nrcoex_ini = hwifi_get_nrcoex_ini();
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_NRCOEX_PARAMS, OAL_SIZEOF(wlan_nrcoex_ini_stru));
    /* ע�ⶨ�ƻ���������,����Ϣ�ܹ����ݵ�������ݳ���Ϊ524�ֽ�,�ٴ���Ҫ�����ڴ���д����� */
    l_ret = memcpy_s(st_write_msg.auc_value, WAL_MSG_WRITE_MAX_LEN, pst_nrcoex_ini, OAL_SIZEOF(wlan_nrcoex_ini_stru));
    if (l_ret != EOK) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "hwifi_config_init_nrcoex_params:memcpy fail, l_ret=%d", l_ret);
        return;
    }
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(wlan_nrcoex_ini_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (l_ret != OAL_SUCC) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "{hwifi_config_init_nrcoex_params_main::return err code [%d]!}\r\n", l_ret);
        return;
    }
    return;
}
#endif


OAL_STATIC oal_void hwifi_config_init_ini_log(oal_net_device_stru *pst_cfg_net_dev)
{
    wal_msg_write_stru st_write_msg;
    oal_int32 l_ret;
    oal_int32 l_loglevel;

    /* log_level */
    l_loglevel = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_LOGLEVEL);
    if ((l_loglevel < OAM_LOG_LEVEL_ERROR) ||
        (l_loglevel > OAM_LOG_LEVEL_INFO)) {
        OAM_ERROR_LOG3(0, OAM_SF_ANY, "hwifi_config_init_ini_clock::loglevel[%d] out of range[%d, %d]",
                       l_loglevel, OAM_LOG_LEVEL_ERROR, OAM_LOG_LEVEL_INFO);
        return;
    }

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_ALL_LOG_LEVEL, OAL_SIZEOF(oal_int32));
    *((oal_int32 *)(st_write_msg.auc_value)) = l_loglevel;
    l_ret = wal_send_cfg_event(pst_cfg_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(oal_int32),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "{hwifi_config_init_ini_log::return err code[%d]!}\r\n", l_ret);
    }
}

OAL_STATIC oal_void hwifi_config_init_cca(wlan_cfg_customize_rf *pst_customize_rf)
{
    oal_int8 c_delta_cca_ed_high_20th_2g =
        (oal_int8)hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DELTA_CCA_ED_HIGH_20TH_2G);
    oal_int8 c_delta_cca_ed_high_40th_2g =
        (oal_int8)hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DELTA_CCA_ED_HIGH_40TH_2G);
    oal_int8 c_delta_cca_ed_high_20th_5g =
        (oal_int8)hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DELTA_CCA_ED_HIGH_20TH_5G);
    oal_int8 c_delta_cca_ed_high_40th_5g =
        (oal_int8)hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DELTA_CCA_ED_HIGH_40TH_5G);
    /* ���ÿһ��ĵ��������Ƿ񳬳�������� */
    if (CUS_DELTA_CCA_ED_HIGH_TH_OUT_OF_RANGE(c_delta_cca_ed_high_20th_2g) ||
        CUS_DELTA_CCA_ED_HIGH_TH_OUT_OF_RANGE(c_delta_cca_ed_high_40th_2g) ||
        CUS_DELTA_CCA_ED_HIGH_TH_OUT_OF_RANGE(c_delta_cca_ed_high_20th_5g) ||
        CUS_DELTA_CCA_ED_HIGH_TH_OUT_OF_RANGE(c_delta_cca_ed_high_40th_5g)) {
        OAM_ERROR_LOG4(0, OAM_SF_ANY, "{hwifi_config_init_cca::one or more delta cca ed high threshold out of \
            range[delta_20th_2g=%d, delta_40th_2g=%d, delta_20th_5g=%d, delta_40th_5g=%d], please check the value",
                       c_delta_cca_ed_high_20th_2g, c_delta_cca_ed_high_40th_2g,
                       c_delta_cca_ed_high_20th_5g, c_delta_cca_ed_high_40th_5g);
        /* set 0 */
        pst_customize_rf->c_delta_cca_ed_high_20th_2g = 0;
        pst_customize_rf->c_delta_cca_ed_high_40th_2g = 0;
        pst_customize_rf->c_delta_cca_ed_high_20th_5g = 0;
        pst_customize_rf->c_delta_cca_ed_high_40th_5g = 0;
    } else {
        pst_customize_rf->c_delta_cca_ed_high_20th_2g = c_delta_cca_ed_high_20th_2g;
        pst_customize_rf->c_delta_cca_ed_high_40th_2g = c_delta_cca_ed_high_40th_2g;
        pst_customize_rf->c_delta_cca_ed_high_20th_5g = c_delta_cca_ed_high_20th_5g;
        pst_customize_rf->c_delta_cca_ed_high_40th_5g = c_delta_cca_ed_high_40th_5g;
    }
}


OAL_STATIC oal_void hwifi_config_init_ini_rf(oal_net_device_stru *pst_cfg_net_dev)
{
    wal_msg_write_stru st_write_msg;
    oal_int32 l_ret;
    oal_uint8 uc_idx; /* �ṹ�������±� */
    oal_uint8 uc_error_param = OAL_FALSE;
    wlan_cfg_customize_rf *pst_customize_rf;

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_CUS_RF, OAL_SIZEOF(wlan_cfg_customize_rf));

    pst_customize_rf = (wlan_cfg_customize_rf *)(st_write_msg.auc_value);

    /* ����: 2g rf */
    for (uc_idx = 0; uc_idx < MAC_NUM_2G_BAND; ++uc_idx) {
        /* ��ȡ��2p4g ��band 0.25db��0.1db���ȵ�����ֵ */
        oal_int8 c_mult4 =
            /* ѭ��ÿ����BANDX��BANDX+1��ÿ��BAND��MULT4��MULT10 2�� */
            (oal_int8)hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RF_LINE_TXRX_GAIN_DB_2G_BAND_START + 2 * uc_idx);
        oal_int8 c_mult10 =
            (oal_int8)hwifi_get_init_value(CUS_TAG_INI,
            /* ѭ��ÿ����BANDX��BANDX+1��ÿ��BAND��MULT4��MULT10 2�� */
                                           WLAN_CFG_INIT_RF_LINE_TXRX_GAIN_DB_2G_BAND_START + 2 * uc_idx + 1);
        if ((c_mult4 >= RF_LINE_TXRX_GAIN_DB_2G_MIN) && (c_mult4 <= 0) &&
            (c_mult10 >= RF_LINE_TXRX_GAIN_DB_2G_MIN) && (c_mult10 <= 0) && !uc_error_param) {
            pst_customize_rf->ac_gain_db_2g[uc_idx].c_rf_gain_db_mult4 = c_mult4;
            pst_customize_rf->ac_gain_db_2g[uc_idx].c_rf_gain_db_mult10 = c_mult10;
        } else {
            /* ֵ������Ч��Χ�������ΪTRUE */
            uc_error_param = OAL_TRUE;
            OAM_ERROR_LOG2(0, OAM_SF_ANY, "hwifi_config_init_ini_rf:[ini]value out of range, config id range[%d, %d]",
                           /* ѭ��ÿ����BANDX��BANDX+1��ÿ��BAND��MULT4��MULT10 2�� */
                           WLAN_CFG_INIT_RF_LINE_TXRX_GAIN_DB_2G_BAND_START + 2 * uc_idx,
                           /* ѭ��ÿ����BANDX��BANDX+1��ÿ��BAND��MULT4��MULT10 2�� */
                           WLAN_CFG_INIT_RF_LINE_TXRX_GAIN_DB_2G_BAND_START + 2 * uc_idx + 1);
        }
    }
    if (mac_get_band_5g_enabled()) {
        /* ����: 5g rf */
        /* ����: fem�ڵ����߿ڵĸ����� */
        for (uc_idx = 0; uc_idx < MAC_NUM_5G_BAND; ++uc_idx) {
            /* ��ȡ��5g ��band 0.25db��0.1db���ȵ�����ֵ */
            oal_int8 c_mult4 =
                (oal_int8)hwifi_get_init_value(CUS_TAG_INI,
                 /* ѭ��ÿ����BANDX��BANDX+1��ÿ��BAND��MULT4��MULT10 2�� */
                                               WLAN_CFG_INIT_RF_LINE_TXRX_GAIN_DB_5G_BAND_START + 2 * uc_idx);
            oal_int8 c_mult10 =
                (oal_int8)hwifi_get_init_value(CUS_TAG_INI,
                 /* ѭ��ÿ����BANDX��BANDX+1��ÿ��BAND��MULT4��MULT10 2�� */
                                               WLAN_CFG_INIT_RF_LINE_TXRX_GAIN_DB_5G_BAND_START + 2 * uc_idx + 1);
            if ((c_mult4 <= 0) && (c_mult10 <= 0)) {
                pst_customize_rf->ac_gain_db_5g[uc_idx].c_rf_gain_db_mult4 = c_mult4;
                pst_customize_rf->ac_gain_db_5g[uc_idx].c_rf_gain_db_mult10 = c_mult10;
            } else {
                /* ֵ������Ч��Χ�������ΪTRUE */
                uc_error_param = OAL_TRUE;
                OAM_ERROR_LOG2(0, OAM_SF_ANY, "{hwifi_config_init_ini_rf:value out of range, config id range[%d, %d]",
                               /* ѭ��ÿ����BANDX��BANDX+1��ÿ��BAND��MULT4��MULT10 2�� */
                               WLAN_CFG_INIT_RF_LINE_TXRX_GAIN_DB_5G_BAND_START + 2 * uc_idx,
                               /* ѭ��ÿ����BANDX��BANDX+1��ÿ��BAND��MULT4��MULT10 2�� */
                               WLAN_CFG_INIT_RF_LINE_TXRX_GAIN_DB_5G_BAND_START + 2 * uc_idx + 1);
            }
        }

        pst_customize_rf->c_rf_line_rx_gain_db_5g =
            (oal_int8)hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RF_LINE_RX_GAIN_DB_5G);
        pst_customize_rf->c_lna_gain_db_5g = (oal_int8)hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_LNA_GAIN_DB_5G);
        pst_customize_rf->c_rf_line_tx_gain_db_5g =
            (oal_int8)hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_RF_LINE_TX_GAIN_DB_5G);
        pst_customize_rf->uc_ext_switch_isexist_5g =
            (oal_uint8) !!hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_EXT_SWITCH_ISEXIST_5G);
        pst_customize_rf->uc_ext_pa_isexist_5g =
            (oal_uint8) !!hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_EXT_PA_ISEXIST_5G);
        pst_customize_rf->uc_ext_lna_isexist_5g =
            (oal_uint8) !!hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_EXT_LNA_ISEXIST_5G);
        pst_customize_rf->us_lna_on2off_time_ns_5g =
            (oal_uint16)hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_LNA_ON2OFF_TIME_NS_5G);
        pst_customize_rf->us_lna_off2on_time_ns_5g =
            (oal_uint16)hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_LNA_OFF2ON_TIME_NS_5G);
        if (!(((pst_customize_rf->c_rf_line_rx_gain_db_5g >= RF_LINE_TXRX_GAIN_DB_5G_MIN) &&
               (pst_customize_rf->c_rf_line_rx_gain_db_5g <= 0)) &&
              ((pst_customize_rf->c_rf_line_tx_gain_db_5g >= RF_LINE_TXRX_GAIN_DB_5G_MIN) &&
               (pst_customize_rf->c_rf_line_tx_gain_db_5g <= 0)) &&
              ((pst_customize_rf->c_lna_gain_db_5g >= LNA_GAIN_DB_MIN) &&
               (pst_customize_rf->c_lna_gain_db_5g <= LNA_GAIN_DB_MAX)))) {
            OAM_ERROR_LOG2(0, OAM_SF_ANY, "hwifi_config_init_ini_rf::cfg_id_range[%d, %d] value out of range",
                           WLAN_CFG_INIT_RF_LINE_RX_GAIN_DB_5G, WLAN_CFG_INIT_LNA_OFF2ON_TIME_NS_5G);
            /* ֵ������Ч��Χ�������ΪTRUE */
            uc_error_param = OAL_TRUE;
        }
    }
    pst_customize_rf->uc_far_dist_pow_gain_switch =
        (oal_uint8) !!hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_FAR_DIST_POW_GAIN_SWITCH);
    pst_customize_rf->uc_far_dist_dsss_scale_promote_switch =
        (oal_uint8) !!hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_FAR_DIST_DSSS_SCALE_PROMOTE_SWITCH);

    /* ����: cca�������޵���ֵ */
    hwifi_config_init_cca(pst_customize_rf);
    pst_customize_rf->c_delta_pwr_ref_2g_20m =
        (oal_int8)hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DELTA_PWR_REF_2G_20M);
    pst_customize_rf->c_delta_pwr_ref_2g_40m =
        (oal_int8)hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DELTA_PWR_REF_2G_40M);
    pst_customize_rf->c_delta_pwr_ref_5g_20m =
        (oal_int8)hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DELTA_PWR_REF_5G_20M);
    pst_customize_rf->c_delta_pwr_ref_5g_40m =
        (oal_int8)hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DELTA_PWR_REF_5G_40M);
    pst_customize_rf->c_delta_pwr_ref_5g_80m =
        (oal_int8)hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DELTA_PWR_REF_5G_80M);

    /* ��������������в���ȷ�ģ�ֱ�ӷ��� */
    if (uc_error_param) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "hwifi_config_init_ini_rf::one or more params have wrong value");
        return;
    }
    /* ������в���������Ч��Χ�ڣ����·�����ֵ */
    l_ret = wal_send_cfg_event(pst_cfg_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(wlan_cfg_customize_rf),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "hwifi_config_init_ini_rf:wal_send_cfg_event failed, return err code[%d]", l_ret);
    }
}
#ifdef _PRE_WLAN_DOWNLOAD_PM
OAL_STATIC oal_void hwifi_config_init_ini_download_pm(oal_net_device_stru *pst_cfg_net_dev)
{
    wal_msg_write_stru st_write_msg;
    oal_uint16 us_download_rate_limit_pps;
    oal_uint32 l_ret;

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_CUS_DOWNLOAD_RATE_LIMIT,
                           OAL_SIZEOF(us_download_rate_limit_pps));
    us_download_rate_limit_pps =
        (oal_uint16)hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_DOWNLOAD_RATE_LIMIT_PPS);
    *(oal_uint16 *)st_write_msg.auc_value = us_download_rate_limit_pps;

    l_ret = wal_send_cfg_event(pst_cfg_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(oal_uint16),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "{hwifi_config_init_ini_download_pm::return err code [%d]!}\r\n", l_ret);
    }
}
#endif

OAL_STATIC oal_void hwifi_config_cali_mask_enable(oal_int32 *pl_priv_value)
{
    if (wlan_cal_disable_1102a == OAL_TRUE) {
        *pl_priv_value = (oal_int32)(((oal_uint32)*pl_priv_value) | WAL_CALI_MASK_CLOSEALL);
    }
}
oal_void hwifi_config_feature_temp_protect(wlan_cfg_customize_priv *pst_customize_priv)
{
    oal_uint32 l_ret;
    oal_int32 l_priv_value = 0;
#ifdef _PRE_WLAN_FEATURE_TEMP_PROTECT
    l_ret = hwifi_get_init_priv_value(WLAN_CFG_PRIV_TEMP_PRO_EN, &l_priv_value);
    if (l_ret == OAL_SUCC) {
        pst_customize_priv->st_temp_pri_custom.uc_temp_pro_enable = (oal_uint8)l_priv_value;
    }

    l_ret = hwifi_get_init_priv_value(WLAN_CFG_PRIV_TEMP_PRO_REDUCE_PWR_EN, &l_priv_value);
    if (l_ret == OAL_SUCC) {
        pst_customize_priv->st_temp_pri_custom.uc_temp_pro_reduce_pwr_enable = (oal_uint8)l_priv_value;
    }

    l_ret = hwifi_get_init_priv_value(WLAN_CFG_PRIV_TEMP_PRO_SAFE_TH, &l_priv_value);
    if (l_ret == OAL_SUCC) {
        pst_customize_priv->st_temp_pri_custom.us_temp_pro_safe_th = (oal_uint16)l_priv_value;
    }

    l_ret = hwifi_get_init_priv_value(WLAN_CFG_PRIV_TEMP_PRO_OVER_TH, &l_priv_value);
    if (l_ret == OAL_SUCC) {
        pst_customize_priv->st_temp_pri_custom.us_temp_pro_over_th = (oal_uint16)l_priv_value;
    }

    l_ret = hwifi_get_init_priv_value(WLAN_CFG_PRIV_TEMP_PRO_PA_OFF_TH, &l_priv_value);
    if (l_ret == OAL_SUCC) {
        pst_customize_priv->st_temp_pri_custom.us_temp_pro_pa_off_th = (oal_uint16)l_priv_value;
    }
#endif
}

oal_int32 hwifi_config_init_ini_priv(oal_net_device_stru *pst_cfg_net_dev)
{
    wal_msg_write_stru st_write_msg;
    oal_uint32 l_ret;
    wlan_cfg_customize_priv *pst_customize_priv;
    oal_int32 l_priv_value = 0;
    mac_device_voe_custom_stru *pst_voe_custom_param = mac_get_voe_custom_param_addr();

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_CUS_PRIV, OAL_SIZEOF(wlan_cfg_customize_priv));
    pst_customize_priv = (wlan_cfg_customize_priv *)(st_write_msg.auc_value);
    l_ret = hwifi_get_init_priv_value(WLAN_CFG_PRIV_CALI_MASK, &l_priv_value);
    /* ǿ�ƹر�У׼ʹ�ã����߷�ֹ���� */
    hwifi_config_cali_mask_enable(&l_priv_value);

    if (l_ret == OAL_SUCC) {
        pst_customize_priv->us_cali_mask = (oal_uint16)l_priv_value;
    }

    l_ret = hwifi_get_init_priv_value(WLAN_CFG_PRIV_FAST_CHECK_CNT, &l_priv_value);
    if (l_ret == OAL_SUCC) {
        pst_customize_priv->uc_fast_check_cnt = (oal_uint8)l_priv_value;
        g_wlan_fast_check_cnt = (l_priv_value > 0) ? (oal_uint8)l_priv_value : HMAC_PSM_TIMER_MIDIUM_CNT;
    }
    l_ret = hwifi_get_init_priv_value(WLAN_CFG_PRIV_VOE_SWITCH, &l_priv_value);
    if (l_ret == OAL_SUCC) {
        pst_customize_priv->uc_voe_switch = (oal_uint8)l_priv_value;
        pst_voe_custom_param->en_11k = ((oal_uint32)l_priv_value & BIT0) ? OAL_TRUE : OAL_FALSE;
        pst_voe_custom_param->en_11v = ((oal_uint32)l_priv_value & BIT1) ? OAL_TRUE : OAL_FALSE;
        pst_voe_custom_param->en_11r = ((oal_uint32)l_priv_value & BIT2) ? OAL_TRUE : OAL_FALSE;
    }
    hwifi_config_feature_temp_protect(pst_customize_priv);
    l_ret = hwifi_get_init_priv_value(WLAN_DSSS2OFDM_DBB_PWR_BO_VAL, &l_priv_value);
    if (l_ret == OAL_SUCC) {
        pst_customize_priv->s_dsss2ofdm_dbb_pwr_bo_val = (oal_int16)l_priv_value;
    }
#ifdef _PRE_WLAN_FEATURE_DYN_BYPASS_EXTLNA
    l_ret = hwifi_get_init_priv_value(WLAN_CFG_PRIV_DYN_BYPASS_EXTLNA, &l_priv_value);
    if (l_ret == OAL_SUCC) {
        pst_customize_priv->uc_dyn_bypass_extlna_enable = (oal_uint8)l_priv_value;
        OAM_WARNING_LOG1(0, OAM_SF_CFG, "hwifi_config_init_ini_priv: EXTLNA[%d]",
                         pst_customize_priv->uc_dyn_bypass_extlna_enable);
    }
#endif
    l_ret = hwifi_get_init_priv_value(WLAN_CFG_PRIV_HCC_FLOWCTRL_TYPE, &l_priv_value);
    if (l_ret == OAL_SUCC) {
        pst_customize_priv->uc_hcc_flowctrl_type = (oal_uint8)l_priv_value;
        OAM_WARNING_LOG1(0, OAM_SF_CFG, "hwifi_config_init_ini_priv: Non high priority flowctrl type[%d]",
                         pst_customize_priv->uc_hcc_flowctrl_type);
    }
    l_ret = hwifi_get_init_priv_value(WLAN_CFG_PRIV_5G_EXT_FEM_TYPE, &l_priv_value);
    if (l_ret == OAL_SUCC) {
        pst_customize_priv->uc_5g_ext_fem_type = (oal_uint8)l_priv_value;
    }
    pst_customize_priv->uc_i3c_switch = hwifi_get_gnss_scan_by_i3c();

    /* ������в���������Ч��Χ�ڣ����·�����ֵ */
    l_ret = wal_send_cfg_event(pst_cfg_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(wlan_cfg_customize_priv),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "{hwifi_config_init_ini_rf::wal_send_cfg_event failed, err code [%d]", l_ret);
        return OAL_FAIL;
    }
    return OAL_SUCC;
}
#ifdef _PRE_WLAN_FIT_BASED_REALTIME_CALI


oal_int32 hwifi_cfg_init_line_params(wlan_cus_dy_cali_param_stru *puc_dyn_cali_param)
{
    oal_int32 l_ret;

    l_ret = memcpy_s(puc_dyn_cali_param->al_dy_cali_base_ratio_params,
                     OAL_SIZEOF(puc_dyn_cali_param->al_dy_cali_base_ratio_params),
                     as_pro_line_params, OAL_SIZEOF(puc_dyn_cali_param->al_dy_cali_base_ratio_params));
    l_ret += memcpy_s(puc_dyn_cali_param->al_dy_cali_base_ratio_ppa_params,
                      OAL_SIZEOF(puc_dyn_cali_param->al_dy_cali_base_ratio_ppa_params),
                      &as_pro_line_params[CUS_DY_CALI_PARAMS_NUM],
                      OAL_SIZEOF(puc_dyn_cali_param->al_dy_cali_base_ratio_ppa_params));
    // BT power fit params
    l_ret += memcpy_s(puc_dyn_cali_param->al_bt_power_fit_params,
                      OAL_SIZEOF(puc_dyn_cali_param->al_bt_power_fit_params),
                      &as_pro_line_params[CUS_DY_CALI_PARAMS_NUM + 1],
                      OAL_SIZEOF(puc_dyn_cali_param->al_bt_power_fit_params));
    // BT PPA Vdet fit params
    l_ret += memcpy_s(puc_dyn_cali_param->al_bt_ppavdet_fit_params,
                      OAL_SIZEOF(puc_dyn_cali_param->al_bt_ppavdet_fit_params),
                      &as_pro_line_params[CUS_DY_CALI_PARAMS_NUM + 2], /* ȡ���ⶨ�ƻ���������14 + 2Ԫ�صĵ�ַ */
                      OAL_SIZEOF(puc_dyn_cali_param->al_bt_ppavdet_fit_params));
    return l_ret;
}


OAL_STATIC oal_uint32 hwifi_cfg_init_cus_dyn_cali(wlan_cus_dy_cali_param_stru *puc_dyn_cali_param)
{
    oal_int32 l_val;
    oal_uint8 uc_idx = 0;
    oal_uint8 uc_dy_cal_param_idx;
    oal_uint8 uc_cfg_id = WLAN_CFG_DTS_2G_CORE0_DPN_CH1;
    oal_uint8 uc_gm_opt;
    oal_int32 l_ret;

    /* ��̬У׼������ϵ����μ�� */
    for (uc_dy_cal_param_idx = 0; uc_dy_cal_param_idx < DY_CALI_PARAMS_NUM; uc_dy_cal_param_idx++) {
        if (as_pro_line_params[uc_dy_cal_param_idx].l_pow_par2 == 0) {
            OAM_ERROR_LOG0(0, OAM_SF_CUSTOM, "{hwifi_cfg_init_cus_dyn_cali::unexpected val l_pow_par2[0]}");
            return OAL_FAIL;
        }
    }
    l_ret = memcpy_s(puc_dyn_cali_param->as_extre_point_val,
                     OAL_SIZEOF(puc_dyn_cali_param->as_extre_point_val),
                     gs_extre_point_vals, OAL_SIZEOF(puc_dyn_cali_param->as_extre_point_val));
    l_ret += hwifi_cfg_init_line_params(puc_dyn_cali_param);
    /* DPN */
    for (uc_idx = 0; uc_idx < MAC_2G_CHANNEL_NUM; uc_idx++) {
        l_val = hwifi_get_init_value(CUS_TAG_DTS, uc_cfg_id + uc_idx);
        l_ret += memcpy_s(puc_dyn_cali_param->ac_dy_cali_2g_dpn_params[uc_idx],
                          DY_CALI_DPN_PARAMS_NUM * OAL_SIZEOF(oal_int8),
                          &l_val, DY_CALI_DPN_PARAMS_NUM * OAL_SIZEOF(oal_int8));
    }

    uc_cfg_id += MAC_2G_CHANNEL_NUM;

    for (uc_idx = 0; uc_idx < CUS_DY_CALI_NUM_5G_BAND; uc_idx++) {
        l_val = hwifi_get_init_value(CUS_TAG_DTS, uc_cfg_id + uc_idx);
        l_ret += memcpy_s(puc_dyn_cali_param->ac_dy_cali_5g_dpn_params[uc_idx],
                          WLAN_BW_CAP_160M * OAL_SIZEOF(oal_int8),
                          &l_val, WLAN_BW_CAP_160M * OAL_SIZEOF(oal_int8));
    }
    uc_cfg_id += DY_CALI_NUM_5G_BAND;

    l_val = hwifi_get_init_value(CUS_TAG_DTS, WLAN_CFG_DTS_DYN_CALI_DSCR_ITERVL);
    puc_dyn_cali_param->aus_dyn_cali_dscr_interval[WLAN_BAND_2G] = (oal_uint16)((oal_uint32)l_val & 0x0000FFFF);

    if (mac_get_band_5g_enabled()) {
        puc_dyn_cali_param->aus_dyn_cali_dscr_interval[WLAN_BAND_5G] =
           (oal_uint16)(((oal_uint32)l_val & 0xFFFF0000) >> 16); /* 5G�����ȡ��̬У׼���أ�l_val��16λ�� */
    }

    /* en tx power pdebuf opt */
    l_val = hwifi_get_init_value(CUS_TAG_DTS, WLAN_CFG_DTS_DYN_CALI_OPT_SWITCH);
    uc_gm_opt = ((oal_uint32)l_val & BIT2) >> NUM_1_BITS;

    if (((oal_uint32)l_val & 0x3) >> 1) {
        /* ����Ӧѡ������в���У׼���Ͳ���gm���������û�в���У׼������gm���� */
        l_val = !hwifi_get_fact_cali_completed();
    } else {
        /* ��ȡ�·�ֵ */
        l_val = (oal_int32)((oal_uint32)l_val & BIT0);
    }

    puc_dyn_cali_param->uc_tx_power_pdbuf_opt = ((oal_uint8)l_val) | uc_gm_opt;

    /* g_as_gm0_dB10 */
    l_val = hwifi_get_init_value(CUS_TAG_DTS, WLAN_CFG_DTS_DYN_CALI_GM0_DB10_AMEND);
    puc_dyn_cali_param->s_gm0_dB10 = (oal_int16)l_val;

    /* 5G����У׼UPC����ֵ�����ƻ��·� */
    l_val = hwifi_get_init_value(CUS_TAG_DTS, WLAN_CFG_DTS_5G_UPC_UP_LIMIT);
    puc_dyn_cali_param->uc_5g_upc_upper_limit = (oal_uint8)l_val;

    /* 5G IQУ׼���˹��ʣ����ƻ��·� */
    l_val = hwifi_get_init_value(CUS_TAG_DTS, WLAN_CFG_DTS_5G_IQ_BACKOFF_POWER);
    puc_dyn_cali_param->c_5g_iq_cali_backoff_pow = (oal_int8)l_val;

    /* 5G IQУ׼�������ʣ����ƻ��·� */
    l_val = hwifi_get_init_value(CUS_TAG_DTS, WLAN_CFG_DTS_5G_IQ_CALI_POWER);
    puc_dyn_cali_param->us_5g_iq_cali_pow = (oal_uint16)l_val;
    if (l_ret != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_CUSTOM, "hwifi_cfg_init_cus_dyn_cali::memcpy fail!");
        return OAL_FAIL;
    }

    return OAL_SUCC;
}


OAL_STATIC oal_void hwifi_config_init_cus_dyn_cali(oal_net_device_stru *pst_cfg_net_dev)
{
    wal_msg_write_stru st_write_msg;
    oal_uint32 ul_ret;
    oal_uint32 ul_offset;
    wlan_cus_dy_cali_param_stru st_dy_cus_cali;
    oal_int32 l_ret;

    if (OAL_WARN_ON(pst_cfg_net_dev == OAL_PTR_NULL)) {
        return;
    }

    /* ���ö�̬У׼����TXPWR_PA_DC_REF */
    memset_s(&st_dy_cus_cali, OAL_SIZEOF(wlan_cus_dy_cali_param_stru), 0, OAL_SIZEOF(wlan_cus_dy_cali_param_stru));
    ul_ret = hwifi_cfg_init_cus_dyn_cali(&st_dy_cus_cali);
    if (OAL_UNLIKELY(ul_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_CFG, "{hwifi_config_init_cus_dyn_cali::init cus dyn cali failed ret[%d]!}", ul_ret);
        return;
    }

    /* ������в���������Ч��Χ�ڣ����·�����ֵ */
    l_ret = memcpy_s(st_write_msg.auc_value, OAL_SIZEOF(st_write_msg.auc_value),
                     (oal_int8 *)&st_dy_cus_cali, OAL_SIZEOF(st_dy_cus_cali));
    if (l_ret != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_CFG, "hwifi_config_init_cus_dyn_cali::memcpy fail!");
        return;
    }
    ul_offset = OAL_SIZEOF(st_dy_cus_cali);
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_CUS_DYN_CALI_PARAM, ul_offset);
    ul_ret = wal_send_cfg_event(pst_cfg_net_dev,
                                WAL_MSG_TYPE_WRITE,
                                WAL_MSG_WRITE_MSG_HDR_LENGTH + ul_offset,
                                (oal_uint8 *)&st_write_msg,
                                OAL_FALSE,
                                OAL_PTR_NULL);
    if (OAL_UNLIKELY(ul_ret != OAL_SUCC)) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "{hwifi_config_init_cus_dyn_cali::wal_send_cfg_event failed, ret[%d]!}", ul_ret);
        return;
    }
    return;
}
#endif
oal_void hwifi_config_init_dts_cali_set_bt(wlan_cus_cali_stru *pst_cus_cali)
{
    // BT Gm calibration related
    pst_cus_cali->uc_bt_gm_cali_en = (oal_uint8)hwifi_get_init_value(CUS_TAG_DTS, WLAN_CFG_DTS_BT_CALI_GM_CALI_EN);
    pst_cus_cali->s_bt_gm0_dB10 = (oal_int16)hwifi_get_init_value(CUS_TAG_DTS, WLAN_CFG_DTS_BT_CALI_GM0_DB10);
    pst_cus_cali->uc_bt_insertion_loss =
        (oal_uint8)hwifi_get_init_value(CUS_TAG_DTS, WLAN_CFG_DTS_BT_CALI_INSERTION_LOSS);
    // BT base power
    pst_cus_cali->uc_bt_base_power = (oal_int16)hwifi_get_init_value(CUS_TAG_DTS, WLAN_CFG_DTS_BT_CALI_BASE_POWER);
    // BT is DPN calculating
    pst_cus_cali->uc_bt_is_dpn_calc = (oal_int16)hwifi_get_init_value(CUS_TAG_DTS, WLAN_CFG_DTS_BT_CALI_IS_DPN_CALC);
}

oal_void hwifi_config_init_dts_cali_bt_fit(oal_int32 *pul_params, oal_uint32 ul_paralen,
                                           wlan_cus_cali_stru *pst_cus_cali)
{
#ifdef _PRE_BT_FITTING_DATA_COLLECT
    oal_uint8 uc_idx;
    // BT DPN values for Tx Power calibration
    hwifi_config_get_bt_dpn(pul_params, ul_paralen);
    for (uc_idx = 0; uc_idx < CUS_BT_FREQ_NUM; uc_idx++) {
        if ((pul_params[uc_idx] >= CALI_TXPWR_DPN_MIN) && (pul_params[uc_idx] <= CALI_TXPWR_DPN_MAX)) {
            pst_cus_cali->ac_cali_bt_txpwr_dpn_params[uc_idx] = pul_params[uc_idx];
        } else {
            OAM_ERROR_LOG1(0, OAM_SF_ANY, "{hwifi_config_init_dts_cali_bt_fit::[dts]value out of range, config id[%d],"
            /* ��WLAN_CFG_DTS_BT_CALI_TXPWR_DPN_BAND1 + uc_idx / 10��ֵ������Ч��Χ����error��ӡ�����ñ�־����֤���������� */
                           "check the value in dts file!}", WLAN_CFG_DTS_BT_CALI_TXPWR_DPN_BAND1 + uc_idx / 10);
        }
    }
#endif
}

OAL_STATIC oal_uint32 hwifi_config_init_dts_cali(oal_net_device_stru *pst_cfg_net_dev)
{
    wal_msg_write_stru st_write_msg;
    oal_int32 l_ret;
    wlan_cfg_customize_rf_reg st_rf_reg;
    wlan_cus_cali_stru st_cus_cali;
    oal_uint32 ul_cfg_id;
    oal_int8 c_cfg_value; /* ��ʱ��������Ӷ��ƻ�get�ӿ��л�ȡ��ֵ */
    oal_uint8 uc_error_param = OAL_FALSE;
    oal_uint8 uc_idx = 0; /* �ṹ�������±� */
    oal_uint32 ul_offset = 0;
    oal_uint16 *pus_rf_reg; /* ָ��rf���õĵ�һ���Ĵ��� */
#ifdef _PRE_BT_FITTING_DATA_COLLECT
    oal_int32 l_params[CUS_BT_FREQ_NUM] = { 0 };
#endif

    /** ����: TXPWR_PA_DC_REF **/
    /* 2G REF: ��13���ŵ� */
    for (uc_idx = 0; uc_idx < 13; ++uc_idx) {
        oal_int16 s_ref_val =
            (oal_int16)hwifi_get_init_value(CUS_TAG_DTS, WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_START + uc_idx);
        if ((s_ref_val >= 0) && (s_ref_val <= CALI_TXPWR_PA_DC_REF_MAX)) {
            st_cus_cali.aus_cali_txpwr_pa_dc_ref_2g_val[uc_idx] = s_ref_val;
        } else {
            /* ֵ������Ч��Χ�������ΪTRUE */
            uc_error_param = OAL_TRUE;
            OAM_ERROR_LOG1(0, OAM_SF_ANY, "{hwifi_config_init_dts_cali::[dts]2g ref value out of range, config id[%d], \
                check the value in dts file!}", WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_START + uc_idx);
        }
    }
    if (mac_get_band_5g_enabled()) {
        oal_int16 *ps_ref_5g = &st_cus_cali.us_cali_txpwr_pa_dc_ref_5g_val_band1;
        /* 5G REF: ��7��band */
        for (uc_idx = 0; uc_idx < 7; ++uc_idx) {
            oal_int16 s_ref_val =
                (oal_int16)hwifi_get_init_value(CUS_TAG_DTS, WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_5G_START + uc_idx);
            if ((s_ref_val >= 0) && (s_ref_val <= CALI_TXPWR_PA_DC_REF_MAX)) {
                *(ps_ref_5g + uc_idx) = s_ref_val;
            } else {
                /* ֵ������Ч��Χ�������ΪTRUE */
                uc_error_param = OAL_TRUE;
                OAM_ERROR_LOG1(0, OAM_SF_ANY, "{hwifi_config_init_dts_cali::[dts]5g ref value out of range, config \
                    id[%d], check the value in dts file!}\r\n", WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_5G_START + uc_idx);
            }
        }
    }

    /** ����: BAND 5G ENABLE **/
    st_cus_cali.uc_band_5g_enable = !!mac_get_band_5g_enabled();

    /** ����: �������ȵ�λ **/
    st_cus_cali.uc_tone_amp_grade = (oal_uint8)hwifi_get_init_value(CUS_TAG_DTS, WLAN_CFG_DTS_CALI_TONE_AMP_GRADE);
    hwifi_config_init_dts_cali_bt_fit(l_params, OAL_SIZEOF(l_params), &st_cus_cali);
    // BT frequency for Tx Power calibration
    st_cus_cali.s_cali_bt_txpwr_num = (oal_int16)hwifi_get_init_value(CUS_TAG_DTS, WLAN_CFG_DTS_BT_CALI_TXPWR_NUM);
    ul_cfg_id = WLAN_CFG_DTS_BT_CALI_TXPWR_FREQ1;
    for (uc_idx = 0; uc_idx < CUS_BT_TXPWR_FREQ_NUM_MAX; uc_idx++) {
        c_cfg_value = (oal_int8)hwifi_get_init_value(CUS_TAG_DTS, ul_cfg_id + uc_idx);
        if ((c_cfg_value >= CALI_TXPWR_FREQ_MIN) && (c_cfg_value <= CALI_TXPWR_FREQ_MAX)) {
            st_cus_cali.auc_cali_bt_txpwr_freq[uc_idx] = (oal_uint8)c_cfg_value;
        } else {
            /* ֵ������Ч��Χ����error��ӡ�����ñ�־����֤���������� */
            OAM_ERROR_LOG1(0, OAM_SF_ANY, "{hwifi_config_init_dts_cali::[dts]value out of range, config id[%d], \
                check the value in dts file!}\r\n", ul_cfg_id + uc_idx);
        }
    }
    hwifi_config_init_dts_cali_set_bt(&st_cus_cali);
    /** ����: RF�Ĵ��� **/
    pus_rf_reg = (oal_uint16 *)&st_rf_reg;
    for (ul_cfg_id = WLAN_CFG_DTS_RF_FIRST; ul_cfg_id <= WLAN_CFG_DTS_RF_LAST; ++ul_cfg_id) {
        oal_uint16 us_reg_val = (oal_uint16)hwifi_get_init_value(CUS_TAG_DTS, ul_cfg_id);
        *(pus_rf_reg + ul_cfg_id - WLAN_CFG_DTS_RF_FIRST) = us_reg_val;
    }

    /* ��������������в���ȷ�ģ�ֱ�ӷ��� */
    if (uc_error_param) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "hwifi_config_init_ini_rf::one or more params have wrong value");
        /* �ͷ�pst_band_edge_limit�ڴ� */
        return OAL_FAIL;
    }
    /* ������в���������Ч��Χ�ڣ����·�����ֵ */
    if (memcpy_s(st_write_msg.auc_value, OAL_SIZEOF(wlan_cus_cali_stru), (oal_int8 *)&st_cus_cali,
        OAL_SIZEOF(wlan_cus_cali_stru)) != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "hwifi_config_init_dts_cali::memcpy_s fail!");
        return OAL_FAIL;
    }
    ul_offset += OAL_SIZEOF(wlan_cus_cali_stru);
    if (memcpy_s(st_write_msg.auc_value + ul_offset, OAL_SIZEOF(wlan_cfg_customize_rf_reg), (oal_int8 *)&st_rf_reg,
        OAL_SIZEOF(wlan_cfg_customize_rf_reg)) != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "hwifi_config_init_dts_cali::memcpy_s fail!");
        return OAL_FAIL;
    }
    ul_offset += OAL_SIZEOF(wlan_cfg_customize_rf_reg);

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_CUS_DTS_CALI, ul_offset);
    l_ret = wal_send_cfg_event(pst_cfg_net_dev,
                               WAL_MSG_TYPE_WRITE, WAL_MSG_WRITE_MSG_HDR_LENGTH + ul_offset,
                               (oal_uint8 *)&st_write_msg, OAL_FALSE, OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "{hwifi_config_init_dts_cali::wal_send_cfg_event failed, error no[%d]!}", l_ret);
        return (oal_uint32)l_ret;
    }
    return OAL_SUCC;
}

OAL_STATIC oal_void hwifi_config_init_ini_main(oal_net_device_stru *pst_cfg_net_dev)
{
    /* ���� */
    hwifi_config_init_ini_perf(pst_cfg_net_dev);
    /* LINKLOSS */
    hwifi_config_init_ini_linkloss(pst_cfg_net_dev);
    /* ������ */
    hwifi_config_init_ini_country(pst_cfg_net_dev);
    /* �͹��� */
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    hwifi_config_init_ini_pm(pst_cfg_net_dev);
#endif
    /* ��ά�ɲ� */
    hwifi_config_init_ini_log(pst_cfg_net_dev);
    /* RF */
    hwifi_config_init_ini_rf(pst_cfg_net_dev);
#ifdef _PRE_WLAN_DOWNLOAD_PM
    hwifi_config_init_ini_download_pm(pst_cfg_net_dev);
#endif
#ifdef _PRE_WLAN_FEATURE_NRCOEX
    hwifi_config_init_nrcoex_params(pst_cfg_net_dev);
#endif
    /* INTERWORKING */
    hwifi_config_init_interworking(pst_cfg_net_dev);
}

oal_uint32 hwifi_config_init_nvram_main(oal_net_device_stru *pst_cfg_net_dev)
{
    wal_msg_write_stru st_write_msg;
    wlan_customize_power_params_stru *pst_nvram;
    oal_int32 l_ret;
    oal_uint32 ul_offset = NUM_OF_NV_MAX_TXPOWER * OAL_SIZEOF(oal_uint8);

    pst_nvram = hwifi_get_nvram_params();

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_CUS_NVRAM_PARAM, ul_offset);
    if (memcpy_s(st_write_msg.auc_value, ul_offset, pst_nvram->ac_delt_txpower, ul_offset) != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hwifi_config_init_nvram_main::memcpy fail!}");
        return OAL_FAIL;
    }

    l_ret = wal_send_cfg_event(pst_cfg_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + ul_offset,
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "{hwifi_config_init_nvram_main::return err code [%d]!}\r\n", l_ret);
        return OAL_FAIL;
    }
    return OAL_SUCC;
}

oal_uint32 hwifi_config_init_dts_main(oal_net_device_stru *pst_cfg_net_dev)
{
    /* �·�base power */
    hwifi_config_init_base_power_main(pst_cfg_net_dev);

    /* �·�5G��band����书�� */
    hwifi_config_init_5g_high_band_max_power_main(pst_cfg_net_dev);

#ifdef _PRE_WLAN_FIT_BASED_REALTIME_CALI
    /* �·���̬У׼���� */
    hwifi_config_init_cus_dyn_cali(pst_cfg_net_dev);
#endif
    /* У׼ */
    return hwifi_config_init_dts_cali(pst_cfg_net_dev);
    /* У׼�ŵ���һ������ */
}

OAL_STATIC oal_int32 hwifi_config_init_ini_wlan(oal_net_device_stru *pst_net_dev)
{
    return OAL_SUCC;
}

OAL_STATIC oal_int32 hwifi_config_init_ini_p2p(oal_net_device_stru *pst_net_dev)
{
    return OAL_SUCC;
}

oal_void hwifi_config_init_ini(oal_net_device_stru *pst_net_dev)
{
    oal_net_device_stru *pst_cfg_net_dev;
    mac_vap_stru *pst_mac_vap;
    oal_wireless_dev_stru *pst_wdev;
    mac_wiphy_priv_stru *pst_wiphy_priv;
    mac_vap_stru *pst_cfg_mac_vap;
    hmac_vap_stru *pst_cfg_hmac_vap;
    mac_device_stru *pst_mac_device;

    if (pst_net_dev == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{hwifi_config_init_ini::pst_net_dev is null!}\r\n");
        return;
    }
    pst_mac_vap = OAL_NET_DEV_PRIV(pst_net_dev);
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{hwifi_config_init_ini::pst_mac_vap is null}\r\n");
        return;
    }
    pst_wdev = OAL_NETDEVICE_WDEV(pst_net_dev);
    if (pst_wdev == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{hwifi_config_init_ini::pst_wdev is null!}\r\n");
        return;
    }
    pst_wiphy_priv = (mac_wiphy_priv_stru *)oal_wiphy_priv(pst_wdev->wiphy);
    if (pst_wiphy_priv == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{hwifi_config_init_ini::pst_wiphy_priv is null!}\r\n");
        return;
    }
    pst_mac_device = pst_wiphy_priv->pst_mac_device;
    if (pst_mac_device == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{hwifi_config_init_ini::pst_mac_device is null!}\r\n");
        return;
    }
    pst_cfg_mac_vap = (mac_vap_stru *)mac_res_get_mac_vap(pst_mac_device->uc_cfg_vap_id);
    if (pst_cfg_mac_vap == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(pst_mac_device->uc_cfg_vap_id, OAM_SF_ANY, "hwifi_config_init_ini::pst_cfg_mac_vap is null");
        return;
    }
    pst_cfg_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_device->uc_cfg_vap_id);
    if (pst_cfg_hmac_vap == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(pst_mac_device->uc_cfg_vap_id, OAM_SF_ANY, "hwifi_config_init_ini::pst_cfg_hmac_vap is null");
        return;
    }
    pst_cfg_net_dev = pst_cfg_hmac_vap->pst_net_device;
    if (pst_cfg_net_dev == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{hwifi_config_init_ini::pst_cfg_net_dev is null!}\r\n");
        return;
    }
    if ((pst_wdev->iftype == NL80211_IFTYPE_STATION) ||
        (pst_wdev->iftype == NL80211_IFTYPE_P2P_DEVICE) ||
        (pst_wdev->iftype == NL80211_IFTYPE_AP)) {
        if (!g_uc_cfg_once_flag) {
            hwifi_config_init_nvram_main(pst_net_dev);
            hwifi_config_init_fcc_ce_power_main(pst_net_dev);
            hwifi_config_init_ext_fcc_ce_power(pst_net_dev);
            hwifi_config_init_ini_main(pst_cfg_net_dev);
            g_uc_cfg_once_flag = OAL_TRUE;
        }
        if ((strcmp("wlan0", pst_net_dev->name)) == 0) {
            hwifi_config_init_ini_wlan(pst_net_dev);
        }
#ifdef _PRE_WLAN_FEATURE_P2P
        else if ((strcmp("p2p0", pst_net_dev->name)) == 0) {
            hwifi_config_init_ini_p2p(pst_net_dev);
        }
#endif
        else {
            OAM_WARNING_LOG0(0, OAM_SF_ANY, "{hwifi_config_init_ini::net_dev is not wlan0 or p2p0!}\r\n");
            return;
        }
    }
    return;
}

oal_void hwifi_config_init_force(oal_void)
{
    /* �����ϵ�ʱ��ΪFALSE */
    g_uc_cfg_once_flag = OAL_FALSE;
    if (!g_uc_cfg_flag) {
        hwifi_config_host_global_ini_param();
        g_uc_cfg_flag = OAL_TRUE;
    }
}

#endif /* #ifdef _PRE_PLAT_FEATURE_CUSTOMIZE */


#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE) && (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#ifdef _PRE_WLAN_FEATURE_DFR
OAL_STATIC oal_bool_enum_uint8 wal_dfr_recovery_check(oal_net_device_stru *pst_net_dev)
{
    oal_wireless_dev_stru *pst_wdev;
    mac_wiphy_priv_stru *pst_wiphy_priv;
    mac_device_stru *pst_mac_device;

    if (pst_net_dev == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_dfr_recovery_check::pst_net_dev is null!}\r\n");
        return OAL_FALSE;
    }

    pst_wdev = OAL_NETDEVICE_WDEV(pst_net_dev);
    if (pst_wdev == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_dfr_recovery_check::pst_wdev is null!}\r\n");
        return OAL_FALSE;
    }

    pst_wiphy_priv = (mac_wiphy_priv_stru *)oal_wiphy_priv(pst_wdev->wiphy);
    if (pst_wiphy_priv == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_dfr_recovery_check::pst_wiphy_priv is null!}\r\n");
        return OAL_FALSE;
    }

    pst_mac_device = pst_wiphy_priv->pst_mac_device;
    if (pst_mac_device == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_dfr_recovery_check::pst_mac_device is null!}\r\n");
        return OAL_FALSE;
    }

    OAM_WARNING_LOG2(0, OAM_SF_ANY, "{wal_dfr_recovery_check::recovery_flag:%d, uc_vap_num:%d.}\r\n",
                     g_st_dfr_info.bit_ready_to_recovery_flag, pst_mac_device->uc_vap_num);

    if ((g_st_dfr_info.bit_ready_to_recovery_flag == OAL_TRUE)
        && (!pst_mac_device->uc_vap_num)) {
        /* DFR�ָ�,�ڴ���ҵ��VAPǰ�·�У׼�Ȳ���,ֻ�·�һ�� */
        return OAL_TRUE;
    }

    return OAL_FALSE;
}
#endif /* #ifdef _PRE_WLAN_FEATURE_DFR */
#endif


OAL_STATIC oal_void wal_netdev_open_flush_p2p_random_mac(oal_net_device_stru *pst_net_dev)
{
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
    /* p2p interface mac��ַ����ֻ������add virtual������,
    ���ܱ�֤�ϲ��·������mac���µ�mib,�˴�ˢ��mac addr */
    if ((wlan_customize.en_random_mac_addr_connect == OAL_TRUE) &&
        (oal_strncmp("p2p-p2p0", pst_net_dev->name, OAL_STRLEN("p2p-p2p0")) == 0)) {
        wal_set_random_mac_to_mib(pst_net_dev);
    }
#endif
}

OAL_STATIC oal_int32 _wal_netdev_open(oal_net_device_stru *pst_net_dev, oal_int32 pm_open_ret)
{
    wal_msg_write_stru st_write_msg;
    oal_int32 l_ret;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;
    oal_uint32 ul_err_code;
#ifdef _PRE_WLAN_FEATURE_P2P
    wlan_p2p_mode_enum_uint8 en_p2p_mode;
    oal_wireless_dev_stru *pst_wdev;
#endif
#if (defined(_PRE_PRODUCT_ID_HI110X_HOST) && (_PRE_OS_VERSION_WIN32 != _PRE_OS_VERSION))
    oal_int32 ul_check_hw_status = 0;
#endif

    oal_netdev_priv_stru *pst_netdev_priv = OAL_PTR_NULL;

    OAL_IO_PRINT("wal_netdev_open, dev_name is:%.16s\n", pst_net_dev->name);
    OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_netdev_open::iftype:%d.!}\r\n", pst_net_dev->ieee80211_ptr->iftype);

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE) && (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    g_uc_netdev_is_open = OAL_TRUE;
    if (pm_open_ret != OAL_ERR_CODE_ALREADY_OPEN) {
#ifdef _PRE_WLAN_FEATURE_DFR
        wal_dfr_init_param();
#endif
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
        /* �����ϵ�ʱ��ΪFALSE */
        hwifi_config_init_force();
#endif
        // �����ϵ糡�����·�����VAP
        l_ret = wal_cfg_vap_h2d_event(pst_net_dev);
        if (l_ret != OAL_SUCC) {
            OAL_IO_PRINT("wal_cfg_vap_h2d_event FAIL %d \r\n", l_ret);
            return -OAL_EFAIL;
        }
        OAL_IO_PRINT("wal_cfg_vap_h2d_event succ \r\n");
    }
#ifdef _PRE_WLAN_FEATURE_DFR
    else if (wal_dfr_recovery_check(pst_net_dev)) {
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
        wlan_pm_set_custom_cali_done(OAL_TRUE);
        wal_priv_init_config();
        wlan_dfr_custom_cali();
        hwifi_config_init_force();
#endif
        // �����ϵ糡�����·�����VAP
        l_ret = wal_cfg_vap_h2d_event(pst_net_dev);
        if (l_ret != OAL_SUCC) {
            OAL_IO_PRINT("DFR:wal_cfg_vap_h2d_event FAIL %d \r\n", l_ret);
            return -OAL_EFAIL;
        }
        OAL_IO_PRINT("DFR:wal_cfg_vap_h2d_event succ \r\n");
    }
#endif /* #ifdef _PRE_WLAN_FEATURE_DFR */

#endif

    if (g_st_ap_config_info.l_ap_power_flag == OAL_TRUE) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{_wal_netdev_open_init::power state is on, in ap mode, start vap later.}\r\n");
        /* �˱�����ʱ��һ�Σ���ֹ framework����ģʽ�л�ǰ�·�����up���� */
        g_st_ap_config_info.l_ap_power_flag = OAL_FALSE;
        oal_net_tx_wake_all_queues(pst_net_dev); /* �������Ͷ��� */
        return OAL_SUCC;
    }

    /* �ϵ�host device_stru��ʼ�� */
    l_ret = wal_host_dev_init(pst_net_dev);
    if (l_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{_wal_netdev_open_init::wal_host_dev_init failed [%d].}\r\n", l_ret);
        return -OAL_EFAIL;
    }

    if (((pst_net_dev->ieee80211_ptr->iftype == NL80211_IFTYPE_STATION) ||
         (pst_net_dev->ieee80211_ptr->iftype == NL80211_IFTYPE_P2P_DEVICE)) &&
        (((oal_strcmp("wlan0", pst_net_dev->name)) == 0) || ((oal_strcmp("p2p0", pst_net_dev->name)) == 0))) {
        l_ret = wal_init_wlan_vap(pst_net_dev);
        if (l_ret != OAL_SUCC) {
#ifdef _PRE_WLAN_1102A_CHR
            CHR_EXCEPTION_REPORT(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_WIFI, CHR_LAYER_DRV,
                                 CHR_WIFI_DRV_EVENT_OPEN, CHR_WIFI_DRV_ERROR_POWER_ON);
#endif
            return -OAL_EFAIL;
        }
    } else if (pst_net_dev->ieee80211_ptr->iftype == NL80211_IFTYPE_AP) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_netdev_open::ap mode, no need to start vap.!}\r\n");
        oal_net_tx_wake_all_queues(pst_net_dev); /* �������Ͷ��� */
        return OAL_SUCC;
    }

#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
    hwifi_config_init_ini(pst_net_dev);

    wal_netdev_open_flush_p2p_random_mac(pst_net_dev);
#endif

    /***************************************************************************
        ���¼���wal�㴦��
    ***************************************************************************/
#ifdef _PRE_WLAN_NARROW_BAND
    if (hitalk_status & NBFH_ON_MASK) {
        wal_set_nbfh(pst_net_dev);
    }
#endif

    /* ��д��Ϣ */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_START_VAP, OAL_SIZEOF(mac_cfg_start_vap_param_stru));
    ((mac_cfg_start_vap_param_stru *)st_write_msg.auc_value)->pst_net_dev = pst_net_dev;
#ifdef _PRE_WLAN_FEATURE_P2P
    pst_wdev = pst_net_dev->ieee80211_ptr;
    en_p2p_mode = wal_wireless_iftype_to_mac_p2p_mode(pst_wdev->iftype);
    if (en_p2p_mode == WLAN_P2P_BUTT) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_netdev_open::wal_wireless_iftype_to_mac_p2p_mode return BUFF}\r\n");
#ifdef _PRE_WLAN_1102A_CHR
        CHR_EXCEPTION_REPORT(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_WIFI, CHR_LAYER_DRV,
                             CHR_WIFI_DRV_EVENT_OPEN, CHR_WIFI_DRV_ERROR_POWER_ON);
#endif
        return -OAL_EINVAL;
    }
    ((mac_cfg_start_vap_param_stru *)st_write_msg.auc_value)->en_p2p_mode = en_p2p_mode;
#endif
    ((mac_cfg_start_vap_param_stru *)st_write_msg.auc_value)->en_mgmt_rate_init_flag = OAL_TRUE;

    /* ������Ϣ */
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_start_vap_param_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_TRUE,
                               &pst_rsp_msg);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_netdev_open::wal_alloc_cfg_event return err code %d!}\r\n", l_ret);
        return l_ret;
    }

    /* ��������Ϣ */
    ul_err_code = wal_check_and_release_msg_resp(pst_rsp_msg);
    if (ul_err_code != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_netdev_open::hmac start vap fail, err code[%u]!}\r\n", ul_err_code);
        return -OAL_EINVAL;
    }

    if ((OAL_NETDEVICE_FLAGS(pst_net_dev) & OAL_IFF_RUNNING) == 0) {
        OAL_NETDEVICE_FLAGS(pst_net_dev) |= OAL_IFF_RUNNING;
    }

    pst_netdev_priv = (oal_netdev_priv_stru *)OAL_NET_DEV_WIRELESS_PRIV(pst_net_dev);
    if (pst_netdev_priv->uc_napi_enable
        && (!pst_netdev_priv->uc_state)
        && ((pst_net_dev->ieee80211_ptr->iftype == NL80211_IFTYPE_STATION)
                  || (pst_net_dev->ieee80211_ptr->iftype == NL80211_IFTYPE_P2P_DEVICE)
                  || (pst_net_dev->ieee80211_ptr->iftype == NL80211_IFTYPE_P2P_CLIENT))) {
        oal_napi_enable(&pst_netdev_priv->st_napi);
        pst_netdev_priv->uc_state = 1;
    }

    oal_net_tx_wake_all_queues(pst_net_dev); /* �������Ͷ��� */
#if (defined(_PRE_PRODUCT_ID_HI110X_HOST) && (_PRE_OS_VERSION_WIN32 != _PRE_OS_VERSION))
    /* 1102Ӳ��fem��DEVICE����HOST�麸lna�ջټ��,ֻ��wlan0ʱ��ӡ */
    if ((pst_net_dev->ieee80211_ptr->iftype == NL80211_IFTYPE_STATION)
        && ((strcmp("wlan0", pst_net_dev->name)) == 0)) {
        wal_atcmsrv_ioctl_get_fem_pa_status(pst_net_dev, &ul_check_hw_status);
    }
#endif

    return OAL_SUCC;
}

oal_int32 wal_netdev_open(oal_net_device_stru *pst_net_dev)
{
    oal_int32 ret;
    oal_int32 pm_open_ret = OAL_SUCC;

    if (pst_net_dev == OAL_PTR_NULL) {
        return -OAL_EFAUL;
    }

    if (OAL_NETDEVICE_FLAGS(pst_net_dev) & OAL_IFF_RUNNING) {
        return OAL_SUCC;
    }
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE) && (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#ifdef _PRE_WLAN_NARROW_BAND
    if (hitalk_status) {
        pm_open_ret = hitalk_pm_open();
    } else
#endif
    {
        pm_open_ret = wlan_pm_open();
    }

    if (pm_open_ret == OAL_FAIL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_netdev_open::wlan_pm_open Fail!}\r\n");
#ifdef _PRE_WLAN_1102A_CHR
        CHR_EXCEPTION_REPORT(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_WIFI, CHR_LAYER_DRV,
                             CHR_WIFI_DRV_EVENT_OPEN, CHR_WIFI_DRV_ERROR_POWER_ON);
#endif
        return -OAL_EFAIL;
    }
#endif
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    oal_mutex_lock(&g_st_dfr_mutex);
#endif
    wal_wake_lock();
    ret = _wal_netdev_open(pst_net_dev, pm_open_ret);
    wal_wake_unlock();
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    oal_mutex_unlock(&g_st_dfr_mutex);
#endif

#ifdef _PRE_WLAN_1102A_CHR
    if (ret != OAL_SUCC) {
        CHR_EXCEPTION_REPORT(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_WIFI, CHR_LAYER_DRV,
                             CHR_WIFI_DRV_EVENT_OPEN, CHR_WIFI_DRV_ERROR_POWER_ON);
    }
#endif
#if (defined(_PRE_PRODUCT_ID_HI110X_HOST) && (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 14, 0))) && \
    (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    /* ��¼wlan0 ����ʱ�� */
    if (oal_strncmp("wlan0", pst_net_dev->name, OAL_STRLEN("wlan0")) == 0) {
        g_st_wifi_radio_stat.ull_wifi_on_time_stamp = OAL_TIME_JIFFY;
    }
#endif
#ifdef _PRE_WLAN_FEATURE_DFR
    chr_dev_cali_excp_trigger_dfr();
#endif
    return ret;
}
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE


oal_uint32 wal_custom_cali(oal_void)
{
    oal_net_device_stru *pst_net_dev;

    pst_net_dev = oal_dev_get_by_name("Hisilicon0");
    if (pst_net_dev != OAL_PTR_NULL) {
        /* ����oal_dev_get_by_name�󣬱������oal_dev_putʹnet_dev�����ü�����һ */
        oal_dev_put(pst_net_dev);

        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{hisi_customize_wifi host_module_init::the net_device is already exist!}\r\n");
    } else {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hisi_customize_wifi host_module_init::Hisilicon0 do NOT exist!}\r\n");
        return OAL_FAIL;
    }

    if (wlan_pm_get_custom_cali_done() == OAL_TRUE) {
        /* У׼�����·� */
        wal_send_cali_data(pst_net_dev);
    } else {
        wlan_pm_set_custom_cali_done(OAL_TRUE);
    }

    wal_send_cali_matrix_data(pst_net_dev);

    /* �·����� */
    return hwifi_config_init_dts_main(pst_net_dev);
}


oal_uint32 wal_priv_init_config(oal_void)
{
    oal_net_device_stru *pst_net_dev = oal_dev_get_by_name("Hisilicon0");
    if (pst_net_dev != OAL_PTR_NULL) {
        /* ����oal_dev_get_by_name�󣬱������oal_dev_putʹnet_dev�����ü�����һ */
        oal_dev_put(pst_net_dev);
    } else {
        OAM_ERROR_LOG0(0, OAM_SF_CUSTOM, "{hisi_customize_wifi host_module_init::Hisilicon0 do NOT exist!}\r\n");
        return OAL_FAIL;
    }

    return (oal_uint32)hwifi_config_init_ini_priv(pst_net_dev);
}

oal_int32 wal_set_custom_process_func(custom_cali_func p_cus_fun, priv_ini_config_func p_priv_func)
{
    struct custom_process_func_handler *pst_custom_process_func_handler;
    pst_custom_process_func_handler = oal_get_custom_process_func();
    if (pst_custom_process_func_handler == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_set_custom_process_func:: get handler failed!}");
    } else {
        pst_custom_process_func_handler->p_custom_cali_func = p_cus_fun;
        pst_custom_process_func_handler->p_priv_ini_config_func = p_priv_func;
    }

    return OAL_SUCC;
}
#endif /* #ifdef _PRE_PLAT_FEATURE_CUSTOMIZE */
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE) && (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)

OAL_STATIC oal_int32 wal_set_power_on(oal_net_device_stru *pst_net_dev, oal_int32 power_flag)
{
    oal_int32 l_ret = 0;

    // ap���µ磬����VAP
    if (power_flag == 0) { // �µ�
        /* �µ�host device_struȥ��ʼ�� */
        wal_host_dev_exit(pst_net_dev);

        wal_wake_lock();

#ifdef _PRE_WLAN_NARROW_BAND
        if (hitalk_status) {
            hitalk_pm_close();
            hitalk_status = 0;
        } else
#endif
        {
            wlan_pm_close();
        }

        wal_wake_unlock();

        g_st_ap_config_info.l_ap_power_flag = OAL_FALSE;
    } else if (power_flag == 1) { // �ϵ�
        g_st_ap_config_info.l_ap_power_flag = OAL_TRUE;

        wal_wake_lock();
        l_ret = wlan_pm_open();
        wal_wake_unlock();
        if (l_ret == OAL_FAIL) {
            OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_set_power_on::wlan_pm_open Fail!}\r\n");
            return -OAL_EFAIL;
        } else if (l_ret != OAL_ERR_CODE_ALREADY_OPEN) {
#ifdef _PRE_WLAN_FEATURE_DFR
            wal_dfr_init_param();
#endif
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
            /* �����ϵ�ʱ��ΪFALSE */
            hwifi_config_init_force();
#endif
            // �����ϵ糡�����·�����VAP
            l_ret = wal_cfg_vap_h2d_event(pst_net_dev);
            if (l_ret != OAL_SUCC) {
                return -OAL_EFAIL;
            }
        }

        /* �ϵ�host device_stru��ʼ�� */
        l_ret = wal_host_dev_init(pst_net_dev);
        if (l_ret != OAL_SUCC) {
            OAL_IO_PRINT("wal_set_power_on FAIL %d \r\n", l_ret);
            return -OAL_EFAIL;
        }
    } else {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_set_power_on::pupower_flag:%d error.}\r\n", power_flag);
        return -OAL_EFAIL;
    }

    return OAL_SUCC;
}


OAL_STATIC oal_void wal_set_power_mgmt_on(oal_uint power_mgmt_flag)
{
    struct wlan_pm_s *pst_wlan_pm;
    pst_wlan_pm = wlan_pm_get_drv();
    if (pst_wlan_pm != NULL) {
        /* apģʽ�£��Ƿ������µ����,1:����,0:������ */
        pst_wlan_pm->ul_apmode_allow_pm_flag = power_mgmt_flag;
    } else {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_set_power_mgmt_on::wlan_pm_get_drv return null.");
    }
}

#endif /* (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)&&(_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION) */

#if defined(_PRE_PRODUCT_ID_HI110X_HOST)

oal_int32 wal_netdev_stop_ap(oal_net_device_stru *pst_net_dev)
{
    oal_int32 l_ret;
    oal_netdev_priv_stru *pst_netdev_priv;
    if (pst_net_dev->ieee80211_ptr->iftype != NL80211_IFTYPE_AP) {
        return OAL_SUCC;
    }

#ifdef _PRE_WLAN_FEATURE_DFR
    if (g_st_dfr_info.bit_device_reset_process_flag) {
        return OAL_FAIL;
    }
#endif  // _PRE_WLAN_FEATURE_DFR

    /* ����ɨ��,�Է���20/40Mɨ������йر�AP */
    wal_force_scan_complete(pst_net_dev, OAL_TRUE);
    /* AP�ر��л���STAģʽ,ɾ�����vap */
    if (wal_stop_vap(pst_net_dev) != OAL_SUCC) {
        OAM_WARNING_LOG0(0, OAM_SF_CFG, "{wal_netdev_stop_ap::wal_stop_vap enter a error.}");
    }
    l_ret = wal_deinit_wlan_vap(pst_net_dev);
    if (l_ret != OAL_SUCC) {
        OAM_WARNING_LOG0(0, OAM_SF_CFG, "{wal_netdev_stop_ap::wal_deinit_wlan_vap enter a error.}");
        return l_ret;
    }
    pst_netdev_priv = (oal_netdev_priv_stru *)OAL_NET_DEV_WIRELESS_PRIV(pst_net_dev);

    /* Del aput����Ҫ�л�netdev iftype״̬��station */
    pst_net_dev->ieee80211_ptr->iftype = NL80211_IFTYPE_STATION;

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE) && (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    /* aput�µ� */
    wal_set_power_mgmt_on(OAL_TRUE);
    l_ret = wal_set_power_on(pst_net_dev, OAL_FALSE);
    if (l_ret != OAL_SUCC) {
        OAM_ERROR_LOG1(0, OAM_SF_CFG, "{wal_netdev_stop_ap::wal_set_power_on fail [%d]!}", l_ret);
        return l_ret;
    }
#endif
    return OAL_SUCC;
}

OAL_STATIC oal_int32 wal_netdev_stop_sta_p2p(oal_net_device_stru *pst_net_dev)
{
    oal_int32 l_ret;
    mac_vap_stru *pst_mac_vap;
    mac_device_stru *pst_mac_device;

    /* wlan0/p2p0 downʱ ɾ��VAP */
    if (((pst_net_dev->ieee80211_ptr->iftype == NL80211_IFTYPE_STATION) ||
         (pst_net_dev->ieee80211_ptr->iftype == NL80211_IFTYPE_P2P_DEVICE))
        && ((oal_strcmp("wlan0", pst_net_dev->name) == 0) || (oal_strcmp("p2p0", pst_net_dev->name) == 0))) {
        /* ����ɾ��p2pС��,ֻ����p2p0ʱɾ��p2p�飬������p2p/sta����ʱ�������mac�ᵼ��wal_netdev_stopǶ�׵��ö����� */
        if (oal_strcmp("p2p0", pst_net_dev->name) == 0) {
            pst_mac_vap = OAL_NET_DEV_PRIV(pst_net_dev);
            if (pst_mac_vap == OAL_PTR_NULL) {
                OAM_ERROR_LOG0(0, OAM_SF_P2P, "{wal_netdev_stop_sta_p2p::pst_mac_vap is null, netdev released.}\r\n");
                return OAL_SUCC;
            }
            pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
            if (pst_mac_device != OAL_PTR_NULL) {
                wal_del_p2p_group(pst_mac_device);
            }
        }
        l_ret = wal_deinit_wlan_vap(pst_net_dev);
        if (l_ret != OAL_SUCC) {
            return l_ret;
        }
    }
    return OAL_SUCC;
}

#endif /* #if defined(_PRE_PRODUCT_ID_HI110X_HOST) */

OAL_STATIC oal_int32 _wal_netdev_stop(oal_net_device_stru *pst_net_dev)
{
    wal_msg_write_stru st_write_msg;
    oal_uint32 ul_err_code;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;
    oal_int32 l_ret;
#ifdef _PRE_WLAN_FEATURE_P2P
    wlan_p2p_mode_enum_uint8 en_p2p_mode;
    oal_wireless_dev_stru *pst_wdev;
#endif

    oal_netdev_priv_stru *pst_netdev_priv = OAL_PTR_NULL;

    if (OAL_UNLIKELY(pst_net_dev == OAL_PTR_NULL)) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_netdev_stop::pst_net_dev is null ptr!}\r\n");
        return -OAL_EFAUL;
    }

    /* stop the netdev's queues */
    oal_net_tx_stop_all_queues(pst_net_dev); /* ֹͣ���Ͷ��� */
    wal_force_scan_complete(pst_net_dev, OAL_TRUE);

#if defined(_PRE_PRODUCT_ID_HI110X_HOST)
    OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_netdev_stop::iftype:%d.!}\r\n", pst_net_dev->ieee80211_ptr->iftype);

    OAL_IO_PRINT("wal_netdev_stop, dev_name is:%.16s\n", pst_net_dev->name);

    if (pst_net_dev->ieee80211_ptr->iftype == NL80211_IFTYPE_AP) {
        l_ret = wal_netdev_stop_ap(pst_net_dev);
        return l_ret;
    }

#endif

    /* ���netdev����running״̬����ֱ�ӷ��سɹ� */
    if ((OAL_NETDEVICE_FLAGS(pst_net_dev) & OAL_IFF_RUNNING) == 0) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_netdev_stop::vap is already down!}\r\n");
        return OAL_SUCC;
    }

    /***************************************************************************
                           ���¼���wal�㴦��
    ***************************************************************************/
    /* ��дWID��Ϣ */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_DOWN_VAP, OAL_SIZEOF(mac_cfg_down_vap_param_stru));
    ((mac_cfg_down_vap_param_stru *)st_write_msg.auc_value)->pst_net_dev = pst_net_dev;
#ifdef _PRE_WLAN_FEATURE_P2P
    pst_wdev = pst_net_dev->ieee80211_ptr;
    en_p2p_mode = wal_wireless_iftype_to_mac_p2p_mode(pst_wdev->iftype);
    if (en_p2p_mode == WLAN_P2P_BUTT) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_netdev_stop::wal_wireless_iftype_to_mac_p2p_mode return BUFF}\r\n");
        return -OAL_EINVAL;
    }
    ((mac_cfg_start_vap_param_stru *)st_write_msg.auc_value)->en_p2p_mode = en_p2p_mode;
#endif

    /* ������Ϣ */
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_down_vap_param_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_TRUE,
                               &pst_rsp_msg);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        if (OAL_NET_DEV_PRIV(pst_net_dev) == OAL_PTR_NULL) {
            /* �ر�net_device���������Ӧvap ��null�����flags running��־ */
            OAL_NETDEVICE_FLAGS(pst_net_dev) &= (~OAL_IFF_RUNNING);
            OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_netdev_stop::net_device is null, set flag not running, if_idx:%d}",
                             pst_net_dev->ifindex);
        }
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_netdev_stop::wal_alloc_cfg_event return err code %d!}\r\n", l_ret);
        return l_ret;
    }

    /* ��������Ϣ */
    ul_err_code = wal_check_and_release_msg_resp(pst_rsp_msg);
    if (ul_err_code != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_netdev_stop::hmac stop vap fail!err code [%d]}\r\n", ul_err_code);
        return -OAL_EFAIL;
    }

#if defined(_PRE_PRODUCT_ID_HI110X_HOST)
    l_ret = wal_netdev_stop_sta_p2p(pst_net_dev);
    if (l_ret != OAL_SUCC) {
        return l_ret;
    }

#endif

    pst_netdev_priv = (oal_netdev_priv_stru *)OAL_NET_DEV_WIRELESS_PRIV(pst_net_dev);
    if (pst_netdev_priv->uc_napi_enable
        && ((pst_net_dev->ieee80211_ptr->iftype == NL80211_IFTYPE_STATION)
                  || (pst_net_dev->ieee80211_ptr->iftype == NL80211_IFTYPE_P2P_DEVICE)
                  || (pst_net_dev->ieee80211_ptr->iftype == NL80211_IFTYPE_P2P_CLIENT))) {
        oal_netbuf_queue_purge(&pst_netdev_priv->st_rx_netbuf_queue);
        oal_napi_disable(&pst_netdev_priv->st_napi);
        pst_netdev_priv->uc_state = 0;
    }

    return OAL_SUCC;
}

oal_int32 wal_netdev_stop(oal_net_device_stru *pst_net_dev)
{
    oal_int32 ret;
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    oal_mutex_lock(&g_st_dfr_mutex);
#endif
    wal_wake_lock();
    ret = _wal_netdev_stop(pst_net_dev);
    wal_wake_unlock();
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    oal_mutex_unlock(&g_st_dfr_mutex);
#endif
    return ret;
}


OAL_STATIC oal_net_device_stats_stru *wal_netdev_get_stats(oal_net_device_stru *pst_net_dev)
{
    oal_net_device_stats_stru *pst_stats = &(pst_net_dev->stats);
#if defined(_PRE_PRODUCT_ID_HI110X_HOST)
    mac_vap_stru *pst_mac_vap;
    oam_stat_info_stru *pst_oam_stat;

    oam_vap_stat_info_stru *pst_oam_vap_stat;

    pst_mac_vap = OAL_NET_DEV_PRIV(pst_net_dev);
    pst_oam_stat = OAM_STAT_GET_STAT_ALL();

    if (pst_mac_vap == NULL) {
        return pst_stats;
    }

    if (pst_mac_vap->uc_vap_id >= WLAN_VAP_SUPPORT_MAX_NUM_LIMIT) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "wal_netdev_get_stats error vap id %u", pst_mac_vap->uc_vap_id);
        return pst_stats;
    }

    pst_oam_vap_stat = &(pst_oam_stat->ast_vap_stat_info[pst_mac_vap->uc_vap_id]);

    /* ����ͳ����Ϣ��net_device */
    pst_stats->rx_packets = pst_oam_vap_stat->ul_rx_pkt_to_lan;
    pst_stats->rx_bytes = pst_oam_vap_stat->ul_rx_bytes_to_lan;

    pst_stats->tx_packets = pst_oam_vap_stat->ul_tx_pkt_num_from_lan;
    pst_stats->tx_bytes = pst_oam_vap_stat->ul_tx_bytes_from_lan;
#endif
    return pst_stats;
}


OAL_STATIC oal_int32 _wal_netdev_set_mac_addr(oal_net_device_stru *pst_net_dev, void *p_addr)
{
    oal_sockaddr_stru *pst_mac_addr = OAL_PTR_NULL;
    oal_wireless_dev_stru *pst_wdev = OAL_PTR_NULL; /* ����P2P ������p2p0 �� p2p-p2p0 MAC ��ַ��wlan0 ��ȡ */

#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
    if (wlan_customize.en_random_mac_addr_connect == OAL_FALSE) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_netdev_set_mac_addr::ini custom dont support random mac addr!}\r\n");
        return OAL_SUCC;
    }
#endif

    if (OAL_UNLIKELY((pst_net_dev == OAL_PTR_NULL) || (p_addr == OAL_PTR_NULL))) {
        OAM_ERROR_LOG2(0, OAM_SF_ANY, "{wal_netdev_set_mac_addr::pst_net_dev or p_addr null ptr error %x, %x!}\r\n",
                       (uintptr_t)pst_net_dev, (uintptr_t)p_addr);

        return -OAL_EFAUL;
    }

    if (oal_netif_running(pst_net_dev)) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_netdev_set_mac_addr::cannot set address; device running!}\r\n");

        return -OAL_EBUSY;
    }
    /*lint +e774*//*lint +e506*/
    pst_mac_addr = (oal_sockaddr_stru *)p_addr;

    if (ETHER_IS_MULTICAST((oal_uint8 *)pst_mac_addr->sa_data)) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_netdev_set_mac_addr::can not set group/broadcast addr!}\r\n");

        return -OAL_EINVAL;
    }

    oal_set_mac_addr ((oal_uint8 *)(pst_net_dev->dev_addr), (oal_uint8 *)(pst_mac_addr->sa_data));

    pst_wdev = (oal_wireless_dev_stru *)pst_net_dev->ieee80211_ptr;
    if (pst_wdev == OAL_PTR_NULL) {
        return -OAL_EFAUL;
    }
    OAM_WARNING_LOG4(0, OAM_SF_ANY, "{wal_netdev_set_mac_addr::iftype [%d], mac_addr[%.2x:xx:xx:xx:%.2x:%.2x]..}\r\n",
                     /* iftype��dev_addr��0��4��5byte��Ϊ���������ӡ */
                     pst_wdev->iftype, pst_net_dev->dev_addr[0], pst_net_dev->dev_addr[4], pst_net_dev->dev_addr[5]);

#if defined(_PRE_PRODUCT_ID_HI110X_HOST)
    return OAL_SUCC;
#endif
}

OAL_STATIC oal_int32 wal_netdev_set_mac_addr(oal_net_device_stru *pst_net_dev, void *p_addr)
{
    oal_int32 ret;
    wal_wake_lock();
    ret = _wal_netdev_set_mac_addr(pst_net_dev, p_addr);
    wal_wake_unlock();

    if (ret != OAL_SUCC) {
#ifdef _PRE_WLAN_1102A_CHR
        CHR_EXCEPTION_REPORT(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_WIFI, CHR_LAYER_DRV,
                             CHR_WIFI_DRV_EVENT_OPEN, CHR_WIFI_DRV_ERROR_POWER_ON_SET_MAC_ADDR);
#endif
    }

    return ret;
}


OAL_STATIC oal_uint32 wal_ioctl_force_pass_filter(oal_net_device_stru *pst_net_dev, oal_uint8 uc_param)
{
    wal_msg_write_stru st_write_msg;
    oal_int32 l_ret;
    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_FORCE_PASS_FILTER, OAL_SIZEOF(oal_uint8));

    st_write_msg.auc_value[0] = uc_param;

    /* ������Ϣ */
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(oal_uint8),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_CFG, "{wal_ioctl_force_stop_filter::return err code [%d]!}\r\n", l_ret);
        return (oal_uint32)l_ret;
    }

    OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_ioctl_force_stop_filter::uc_param[%d].}", uc_param);
    return OAL_SUCC;
}


OAL_STATIC oal_uint32 wal_ioctl_judge_input_param_length(
    wal_wifi_priv_cmd_stru st_priv_cmd, oal_uint32 ul_cmd_length, oal_uint16 us_adjust_val)
{
    /* ����+1Ϊ �ַ�����������������м�Ŀո��ַ� */
    if (st_priv_cmd.ul_total_len < ul_cmd_length + 1 + us_adjust_val) {
        /* ��γ��Ȳ�����Ҫ��, ������ڴ�pc_commandͳһ��������wal_vendor_priv_cmd���ͷ� */
        return OAL_FAIL;
    }
    return OAL_SUCC;
}

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
OAL_STATIC oal_int32 wal_ioctl_priv_cmd_tx_power(
    oal_net_device_stru *pst_net_dev, oal_int8 *pc_command, wal_wifi_priv_cmd_stru st_priv_cmd)
{
    oal_uint32 ul_skip = CMD_TX_POWER_LEN + 1;
    oal_uint16 us_txpwr;
    oal_int32 l_ret;

    l_ret = wal_ioctl_judge_input_param_length(st_priv_cmd, CMD_TX_POWER_LEN, 1);
    if (l_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_ioctl_priv_cmd_tx_power::length is too short! at least need [%d]!}",
                         CMD_TX_POWER_LEN + 2); /* �ַ�����������ΪCMD_TX_POWER_LEN + 2 */
        return -OAL_EFAIL;
    }

    us_txpwr = (oal_uint16)oal_atoi(pc_command + ul_skip);
    l_ret = wal_ioctl_reduce_sar(pst_net_dev, us_txpwr);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_ioctl_priv_cmd_tx_power::return err code [%d]!}\r\n", l_ret);
        /* ������ӡ�����룬���سɹ�����ֹsupplicant �ۼ�4�� ioctlʧ�ܵ���wifi�쳣���� */
        return OAL_SUCC;
    }
    return l_ret;
}
#endif

#ifdef _PRE_WLAN_FEATURE_LTECOEX
OAL_STATIC oal_int32 wal_ioctl_priv_cmd_ltecoex_mode(
    oal_net_device_stru *pst_net_dev, oal_int8 *pc_command, wal_wifi_priv_cmd_stru st_priv_cmd)
{
    oal_int32 l_ret;
    oal_int8 ltecoex_mode;

    /* ��ʽ:LTECOEX_MODE 1 or LTECOEX_MODE 0 */
    l_ret = wal_ioctl_judge_input_param_length(st_priv_cmd, CMD_LTECOEX_MODE_LEN, 1);
    if (l_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_ioctl_priv_cmd_ltecoex_mode::length is too short! at least need [%d]!}",
                         CMD_LTECOEX_MODE_LEN + 2); /* �ַ�����������ΪCMD_LTECOEX_MODE_LEN + 2 */
        return -OAL_EFAIL;
    }

    ltecoex_mode = oal_atoi(pc_command + CMD_LTECOEX_MODE_LEN + 1);

    OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_ioctl_priv_cmd_ltecoex_mode::CMD_LTECOEX_MODE command, ltecoex mode:%d}\r\n",
                     ltecoex_mode);

    l_ret = (oal_int32)wal_ioctl_ltecoex_mode_set(pst_net_dev, (oal_int8 *)&ltecoex_mode);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_ioctl_priv_cmd_ltecoex_mode::return err code [%d]!}\r\n", l_ret);
        return -OAL_EFAIL;
    }
    return l_ret;
}
#endif

OAL_STATIC oal_int32 wal_ioctl_priv_cmd_country(
    oal_net_device_stru *pst_net_dev, oal_int8 *pc_command, wal_wifi_priv_cmd_stru st_priv_cmd)
{
#ifdef _PRE_WLAN_FEATURE_11D
    const oal_int8 *country_code = OAL_PTR_NULL;
    oal_int8 auc_country_code[3] = { 0 };
    oal_int32 l_ret;

    /* ��ʽ:COUNTRY CN */
    l_ret = wal_ioctl_judge_input_param_length(st_priv_cmd, CMD_COUNTRY_LEN, 2); /* 2��ʾ����ֵ */
    if (l_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_ioctl_priv_cmd_country::length is too short! at least need [%d]!}",
                         CMD_COUNTRY_LEN + 3); /* �ַ�����������ΪCMD_COUNTRY_LEN + 3 */
        return -OAL_EFAIL;
    }

    country_code = pc_command + CMD_COUNTRY_LEN + 1;
    /* ����2���ַ���auc_country_code */
    if (memcpy_s(auc_country_code, OAL_SIZEOF(auc_country_code), country_code, 2) != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "wal_ioctl_priv_cmd_country::memcpy fail!");
        return -OAL_EFAIL;
    }

#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
    /* ��ini��coutry_code=99ʱ���������޸�country_code���������ʹ��ini��country_code */
    if (g_st_cust_country_code_ignore_flag.en_country_code_ingore_ini_flag == OAL_TRUE) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_ioctl_priv_cmd_country::wlan_pm_set_country is ignore, \
            ignore_ini_flag is [%d]", g_st_cust_country_code_ignore_flag.en_country_code_ingore_ini_flag);

        return OAL_SUCC;
    }

#endif

    l_ret = wal_regdomain_update(pst_net_dev, auc_country_code);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_androiwal_ioctl_priv_cmd_countryd_priv_cmd:return err code [%d]", l_ret);

        return -OAL_EFAIL;
    }
#else
    OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_ioctl_priv_cmd_country::_PRE_WLAN_FEATURE_11D is not define!}\r\n");
#endif

    return l_ret;
}

OAL_STATIC oal_int32 wal_ioctl_priv_cmd_set_ap_wps_p2p_ie(
    oal_net_device_stru *pst_net_dev, oal_int8 *pc_command, wal_wifi_priv_cmd_stru st_priv_cmd)
{
    oal_uint32 ul_skip = CMD_SET_AP_WPS_P2P_IE_LEN + 1;
    oal_app_ie_stru *pst_wps_p2p_ie = OAL_PTR_NULL;
    oal_int32 l_ret;

    /* �ⲿ��������жϣ��ⲿ�������ݳ��ȱ���Ҫ����oal_app_ie_stru�ṹ��ͷ����С */
    l_ret = wal_ioctl_judge_input_param_length(st_priv_cmd, CMD_SET_AP_WPS_P2P_IE_LEN,
                                               (OAL_SIZEOF(oal_app_ie_stru) -
                                                (OAL_SIZEOF(oal_uint8) * WLAN_WPS_IE_MAX_SIZE)));
    if (l_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_ioctl_priv_cmd_set_ap_wps_p2p_ie:length is too short!at least need[%d]",
                         (ul_skip + OAL_SIZEOF(oal_app_ie_stru) - (OAL_SIZEOF(oal_uint8) * WLAN_WPS_IE_MAX_SIZE)));
        return -OAL_EFAIL;
    }

    pst_wps_p2p_ie = (oal_app_ie_stru *)(pc_command + ul_skip);

    /*lint -e413*/
    if ((ul_skip + pst_wps_p2p_ie->ul_ie_len + OAL_OFFSET_OF(oal_app_ie_stru, auc_ie)) >
        (oal_uint32)st_priv_cmd.ul_total_len) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "{wal_ioctl_priv_cmd_set_ap_wap_p2p_ie::SET_AP_WPS_P2P_IE param len is \
            too short. need %d.}\r\n", (ul_skip + pst_wps_p2p_ie->ul_ie_len));
        return -OAL_EFAIL;
    }
    /*lint +e413*/
    l_ret = wal_ioctl_set_wps_p2p_ie(pst_net_dev,
                                     pst_wps_p2p_ie->auc_ie,
                                     pst_wps_p2p_ie->ul_ie_len,
                                     pst_wps_p2p_ie->en_app_ie_type);

    return l_ret;
}

oal_uint32 wal_vendor_priv_tas_cmd(oal_int8 *pc_command, oal_net_device_stru *pst_net_dev,
                                   wal_wifi_priv_cmd_stru st_priv_cmd)
{
    oal_int32 l_ret = OAL_SUCC;
#ifdef _PRE_WLAN_FEATURE_TAS_ANT_SWITCH
    oal_int32 l_param_1 = 0;

    /* tas��֧�ַ���succ */
    if (hwifi_get_tas_state() == OAL_FALSE) {
        return OAL_SUCC;
    }

    if (oal_strncasecmp(pc_command, CMD_SET_ANT_CHANGE, CMD_SET_ANT_CHANGE_LEN) == 0) {
        l_ret = wal_ioctl_judge_input_param_length(st_priv_cmd, CMD_SET_ANT_CHANGE_LEN, 1);
        if (l_ret != OAL_SUCC) {
            OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_vendor_priv_cmd::CMD_SET_ANT_CHANGE cmd len err.}");
            return -OAL_EFAIL;
        }

        /* 0:Ĭ��̬ 1:tas̬ */
        l_param_1 = oal_atoi(pc_command + CMD_SET_ANT_CHANGE_LEN + 1);
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_vendor_priv_cmd::CMD_SET_ANT_CHANGE antIndex[%d].}", l_param_1);
        l_ret = board_wifi_tas_set(l_param_1);
        if (l_ret != OAL_SUCC) { /* �л�TAS�ı�GPIOʧ�ܣ��ϱ�CHR�¼� */
            OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_vendor_priv_cmd::Switch TAS ANT FAIL");
#ifdef _PRE_WLAN_1102A_CHR
            CHR_EXCEPTION_REPORT(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_WIFI,
                                 CHR_LAYER_DEV, CHR_WIFI_DEV_EVENT_CHIP, CHR_WIFI_DEV_ERROR_TAS_GPIO_FAIL);
#endif
        }
    } else if (oal_strncasecmp(pc_command, CMD_TAS_GET_ANT, CMD_TAS_GET_ANT_LEN) == 0) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_vendor_priv_cmd:: CMD_TAS_GET_ANT ant TAS switch[%d]}",
                         board_get_wifi_tas_gpio_state());
        return board_get_wifi_tas_gpio_state();
    } else if (oal_strncasecmp(pc_command, CMD_MEASURE_TAS_RSSI, CMD_MEASURE_TAS_RSSI_LEN) == 0) {
        l_ret = wal_ioctl_judge_input_param_length(st_priv_cmd, CMD_MEASURE_TAS_RSSI_LEN, 1);
        if (l_ret != OAL_SUCC) {
            OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_vendor_priv_cmd::CMD_MEASURE_TAS_RSSI cmd len error}");
            return -OAL_EFAIL;
        }

        l_param_1 = !!oal_atoi(pc_command + CMD_MEASURE_TAS_RSSI_LEN + 1);
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_vendor_priv_cmd::CMD_MEASURE_TAS_RSSI coreIndex[%d].}", l_param_1);
        /* �������� */
        l_ret = wal_ioctl_tas_rssi_access(pst_net_dev, l_param_1);
    } else if (oal_strncasecmp(pc_command, CMD_SET_TAS_TXPOWER, CMD_SET_TAS_TXPOWER_LEN) == 0) {
        /* taş���ʲ�֧�� */
    }
#endif
    return l_ret;
}
OAL_STATIC oal_int32 wal_psm_query_wait_complete(hmac_psm_flt_stat_query_stru *pst_hmac_psm_query,
                                                 mac_psm_query_type_enum_uint8 en_query_type)
{
    pst_hmac_psm_query->auc_complete_flag[en_query_type] = OAL_FALSE;
    /*lint -e730 -e740 -e774*/
    return OAL_WAIT_EVENT_INTERRUPTIBLE_TIMEOUT(pst_hmac_psm_query->st_wait_queue,
                                                (pst_hmac_psm_query->auc_complete_flag[en_query_type] == OAL_TRUE),
                                                OAL_TIME_HZ);
    /*lint +e730 +e740 +e774*/
}
oal_void wal_get_psm_info_copy_to_user_fail(mac_psm_query_type_enum_uint8 en_query_type,
                                            oal_uint32 *pul_info, oal_uint32 ul_size)
{
    if (en_query_type == MAC_PSM_QUERY_BEACON_CNT) {
        OAM_WARNING_LOG4(0, OAM_SF_CFG,
                         "{wal_get_psm_info_copy fail::pst_ifr = null!bcn rcv[%d], tbtt[%d], tim err[%d], tim[%d]}",
                         pul_info[0], pul_info[1], pul_info[2], pul_info[3]); /* 0, 1, 2, 3Ϊ�±� */
    } else if (en_query_type == MAC_PSM_QUERY_FLT_STAT) {
        OAM_WARNING_LOG4(0, OAM_SF_CFG,
                         "{wal_get_psm_info_copy fail::pst_ifr = null!arpdrp[%d], arprsp[%d], apfdrp[%d], icmpdrp[%d]}",
                         pul_info[0], pul_info[1], pul_info[2], pul_info[3]); /* 0, 1, 2, 3Ϊ�±� */
    }
}

oal_uint32 wal_ioctl_get_psm_info(oal_net_device_stru *pst_net_dev,
    mac_psm_query_type_enum_uint8 en_query_type, oal_ifreq_stru *pst_ifr)
{
    mac_vap_stru *pst_mac_vap = OAL_NET_DEV_PRIV(pst_net_dev);
    hmac_device_stru *pst_hmac_device = OAL_PTR_NULL;
    wal_msg_write_stru st_write_msg;
    hmac_psm_flt_stat_query_stru *pst_hmac_psm_query = OAL_PTR_NULL;
    mac_psm_query_stat_stru  *pst_psm_stat = OAL_PTR_NULL;
    oal_int32 l_ret;

    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_CFG, "{wal_ioctl_get_psm_info::pst_mac_vap get from netdev or pst_ifr is null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    pst_hmac_device = hmac_res_get_mac_dev(pst_mac_vap->uc_device_id);
    if (pst_hmac_device == OAL_PTR_NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }
    if (en_query_type >= MAC_PSM_QUERY_TYPE_BUTT) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{wal_ioctl_get_psm_info::query type is not supported}");
        return OAL_FAIL;
    }

    /* ���¼���wal�㴦�� */
    memset_s(&st_write_msg, OAL_SIZEOF(st_write_msg), 0, OAL_SIZEOF(st_write_msg));
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_QUERY_PSM_STAT, OAL_SIZEOF(mac_psm_query_type_enum_uint8));
    /* �·���ѯ���� */
    *(mac_psm_query_type_enum_uint8*)(st_write_msg.auc_value) = en_query_type;
    l_ret = wal_send_cfg_event(pst_net_dev, WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_psm_query_type_enum_uint8),
                               (oal_uint8 *)&st_write_msg, OAL_FALSE, OAL_PTR_NULL);
    if (l_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{wal_ioctl_get_psm_info::send event fail[%d]}", l_ret);
        return OAL_FAIL;
    }

    /* �ȴ���ѯ���� */
    pst_hmac_psm_query = &pst_hmac_device->st_psm_flt_stat_query;
    l_ret = wal_psm_query_wait_complete(pst_hmac_psm_query, en_query_type);
    /* ��ʱ���쳣 */
    if (l_ret <= 0) {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{wal_ioctl_get_psm_info::fail! ret:%d}", l_ret);
        return OAL_FAIL;
    }

    pst_psm_stat = &pst_hmac_psm_query->ast_psm_stat[en_query_type];
    if (pst_psm_stat->ul_query_item > MAC_PSM_QUERY_MSG_MAX_STAT_ITEM) {
        OAM_ERROR_LOG1(0, OAM_SF_CFG, "{wal_ioctl_get_psm_info::query_item invalid[%d]}", pst_psm_stat->ul_query_item);
    }

    /* ��ʽ����ʱ�ò���Ϊnull */
    if (pst_ifr == OAL_PTR_NULL) {
        wal_get_psm_info_copy_to_user_fail(en_query_type, pst_psm_stat->aul_val, OAL_SIZEOF(pst_psm_stat->aul_val));
        return OAL_SUCC;
    }

    if (oal_copy_to_user(pst_ifr->ifr_data + 8, pst_psm_stat->aul_val, /* 8Ϊhdr */
        pst_psm_stat->ul_query_item * OAL_SIZEOF(oal_uint32))) {
        OAM_ERROR_LOG0(0, OAM_SF_CFG, "{wal_ioctl_get_psm_info::Failed to copy ioctl_data to user !}");
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

OAL_STATIC oal_int32 wal_vendor_priv_append_cmd(oal_net_device_stru *pst_net_dev,
                                                oal_int8 *pc_command, wal_wifi_priv_cmd_stru st_priv_cmd,
                                                oal_ifreq_stru *pst_ifr)
{
    /* �������ڲ�֧�ֵ�������سɹ��������ϲ�wpa_supplicant��Ϊioctlʧ�ܣ������쳣����wifi, Ĭ��ΪSUCC */
    oal_int32 l_ret = OAL_SUCC;

#ifdef _PRE_WLAN_FEATURE_P2P
    if (oal_strncmp(pc_command, CMD_MIRACAST_START, CMD_MIRACAST_START_LEN) == 0) {
        l_ret = wal_ioctl_set_p2p_miracast_status(pst_net_dev, OAL_TRUE);
    } else if (oal_strncmp(pc_command, CMD_MIRACAST_STOP, CMD_MIRACAST_STOP_LEN) == 0) {
        l_ret = wal_ioctl_set_p2p_miracast_status(pst_net_dev, OAL_FALSE);
    }
#endif
    else if (oal_strncasecmp(pc_command, CMD_GET_BEACON_CNT, CMD_GET_BEACON_CNT_LEN) == 0) {
        l_ret = wal_ioctl_get_psm_info(pst_net_dev, MAC_PSM_QUERY_BEACON_CNT, pst_ifr);
    } else if (oal_strncasecmp(pc_command, CMD_GET_APF_PKTS_CNT, CMD_GET_APF_PKTS_CNT_LEN) == 0) {
        l_ret = wal_ioctl_get_psm_info(pst_net_dev, MAC_PSM_QUERY_FLT_STAT, pst_ifr);
    } else {
        l_ret = wal_vendor_priv_tas_cmd(pc_command, pst_net_dev, st_priv_cmd);
    }
    return l_ret;
}

oal_int32 wal_vendor_get_priv_feature_cap(oal_net_device_stru *pst_net_dev,
                                          oal_ifreq_stru *pst_ifr,
                                          oal_int8 *pc_command,
                                          wal_wifi_priv_cmd_stru *pst_priv_cmd)
{
    oal_int32 l_value = 0;
    oal_int32 l_ret;
    oal_int32 *pl_value;

    l_ret = wal_ioctl_get_wifi_priv_feature_cap_param(pst_net_dev, pc_command, &l_value);
    if (l_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_vendor_priv_cmd::CMD_GET_WIFI_PRVI_FEATURE_CAPABILITY(%d) \
            return err code [%d]!}", l_ret);
        return -OAL_EFAIL;
    }
    /* ��buf���� */
    memset_s(pc_command, (pst_priv_cmd->ul_total_len + 1), 0, (pst_priv_cmd->ul_total_len + 1));
    pc_command[pst_priv_cmd->ul_total_len] = '\0';
    pl_value = (oal_int32 *)pc_command;
    *pl_value = l_value;
    OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_vendor_get_priv_feature_cap::CMD_GET_WIFI_PRVI_FEATURE_CAPABILITY = [%x]!",
                     l_value);
    if (oal_copy_to_user(pst_ifr->ifr_data + PRIV_CMD_STRU_HEAD_LEN, pc_command,
                         (oal_uint32)(pst_priv_cmd->ul_total_len))) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "wal_vendor_priv_cmd:CMD_GET_WIFI_PRVI_FEATURE_CAPABILITY Failed to \
            copy ioctl_data to user !");
        return -OAL_EFAIL;
    }
    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_VOWIFI
oal_int32 wal_ioctl_process_get_vowifi_params(oal_net_device_stru *pst_net_dev, oal_ifreq_stru *pst_ifr,
                                              oal_int8 *pc_command, wal_wifi_priv_cmd_stru *pst_priv_cmd)
{
    oal_int32 l_value = 0;
    oal_int32 l_ret;
    oal_int32 *pl_value;

    l_ret = wal_ioctl_get_vowifi_param(pst_net_dev, pc_command, &l_value);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_ioctl_process_get_vowifi_params:CMD_VOWIFI_GET_MODE(%d), err code [%d]",
                         l_ret);
        return -OAL_EFAIL;
    }
    /* ��buf���� */
    memset_s(pc_command, (oal_uint32)(pst_priv_cmd->ul_total_len + 1), 0, (oal_uint32)(pst_priv_cmd->ul_total_len + 1));
    pc_command[pst_priv_cmd->ul_total_len] = '\0';
    pl_value = (oal_int32 *)pc_command;
    *pl_value = l_value;
    if (oal_copy_to_user(pst_ifr->ifr_data + PRIV_CMD_STRU_HEAD_LEN,
                         pc_command, (oal_uint32)(pst_priv_cmd->ul_total_len))) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "wal_ioctl_process_get_vowifi_params:Failed to copy data to user");
        return -OAL_EFAIL;
    }
    return OAL_SUCC;
}
#endif
#ifdef _PRE_WLAN_FEATURE_IP_FILTER
#ifdef CONFIG_DOZE_FILTER

oal_int32 wal_vendor_config_filter_cmd(wal_wifi_priv_cmd_stru *pst_priv_cmd, oal_int8 *pc_command)
{
    oal_int32 l_on;
    oal_int32 l_ret;
    /* ��ʽ:FILTER 1 or FILTER 0 */
    l_ret = wal_ioctl_judge_input_param_length(*pst_priv_cmd, CMD_FILTER_SWITCH_LEN, 1);
    if (l_ret != OAL_SUCC) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "{wal_vendor_config_filter_cmd:: CMD_FILTER_SWITCH length is too short! \
        at least need [%d]", (CMD_FILTER_SWITCH_LEN + 2)); /* �ַ�����������ΪCMD_FILTER_SWITCH_LEN + 2 */
        oal_free(pc_command);
        return -OAL_EFAIL;
    }
    l_on = oal_atoi(pc_command + CMD_FILTER_SWITCH_LEN + 1);
    OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_vendor_config_filter_cmd::CMD_FILTER_SWITCH %d.}", l_on);
    /* �����ں˽ӿڵ��� gWlanFilterOps.set_filter_enable */
    l_ret = hw_set_net_filter_enable(l_on);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_vendor_config_filter_cmd::CMD_FILTER_SWITCH return err code [%d]", l_ret);
        oal_free(pc_command);
        return -OAL_EFAIL;
    }
    return OAL_SUCC;
}
#endif
#endif

oal_int32 wal_vendor_priv_cmd(oal_net_device_stru *pst_net_dev, oal_ifreq_stru *pst_ifr, oal_int32 ul_cmd)
{
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    wal_wifi_priv_cmd_stru st_priv_cmd;
    oal_int8 *pc_command = OAL_PTR_NULL;
    oal_int32 l_ret = 0;
    oal_int32 l_memcpy_ret = EOK;

    if (pst_ifr->ifr_data == OAL_PTR_NULL) {
        l_ret = -OAL_EINVAL;
        return l_ret;
    }
#ifdef _PRE_WLAN_FEATURE_DFR
    if (g_st_dfr_info.bit_device_reset_process_flag) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_vendor_priv_cmd::dfr_process_status[%d]!}",
                         g_st_dfr_info.bit_device_reset_process_flag);
        return OAL_SUCC;
    }
#endif  // #ifdef _PRE_WLAN_FEATURE_DFR
    if (oal_copy_from_user((oal_uint8 *)&st_priv_cmd, pst_ifr->ifr_data, sizeof(wal_wifi_priv_cmd_stru))) {
        l_ret = -OAL_EINVAL;
        return l_ret;
    }
    if ((st_priv_cmd.ul_total_len > MAX_PRIV_CMD_SIZE) || (st_priv_cmd.ul_total_len < 0)) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "wal_vendor_priv_cmd:too long priavte command.len:%d", st_priv_cmd.ul_total_len);
        l_ret = -OAL_EINVAL;
        return l_ret;
    }
    /* �����ڴ汣��wpa_supplicant �·������������ */
    pc_command = oal_memalloc((oal_uint32)(st_priv_cmd.ul_total_len + 1)); /* total len Ϊpriv cmd ����buffer ���� */
    if (pc_command == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_vendor_priv_cmd::mem alloc failed.}\r\n");
        l_ret = -OAL_ENOMEM;
        return l_ret;
    }
    /* ����wpa_supplicant ����ں�̬�� */
    memset_s (pc_command, (oal_uint32)(st_priv_cmd.ul_total_len + 1), 0, (oal_uint32)(st_priv_cmd.ul_total_len + 1));
    l_ret = (oal_int32)oal_copy_from_user(pc_command, pst_ifr->ifr_data + PRIV_CMD_STRU_HEAD_LEN,
                                          (oal_uint32)(st_priv_cmd.ul_total_len));
    if (l_ret != 0) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_vendor_priv_cmd::oal_copy_from_user: -OAL_EFAIL }\r\n");
        l_ret = -OAL_EFAIL;
        oal_free(pc_command);
        return l_ret;
    }
    pc_command[st_priv_cmd.ul_total_len] = '\0';
    OAM_WARNING_LOG2(0, OAM_SF_ANY, "{wal_vendor_priv_cmd::vendor private cmd total_len:%d, used_len:%d}\r\n",
                     st_priv_cmd.ul_total_len, st_priv_cmd.ul_used_len);
#ifdef _PRE_WLAN_NARROW_BAND
    if (oal_strncmp(pc_command, CMD_SET_HITALK_STR, OAL_STRLEN(CMD_SET_HITALK_STR)) == 0) {
        if (wlan_pm_is_poweron()) {
            OAM_ERROR_LOG0(0, OAM_SF_ANY, "wal_vendor_priv_cmd: set hitalk when pm open");
        } else {
            hitalk_status = NARROW_BAND_ON_MASK | NBFH_ON_MASK;
            OAM_WARNING_LOG0(0, OAM_SF_ANY, "wal_vendor_priv_cmd: set hitalk enable");
        }
    } else
#endif
        if (oal_strncmp(pc_command, CMD_SET_AP_WPS_P2P_IE, CMD_SET_AP_WPS_P2P_IE_LEN) == 0) {
            l_ret = wal_ioctl_priv_cmd_set_ap_wps_p2p_ie(pst_net_dev, pc_command, st_priv_cmd);
        } else if (oal_strncmp(pc_command, CMD_P2P_SET_NOA, CMD_P2P_SET_NOA_LEN) == 0) {
            oal_uint32 skip = CMD_P2P_SET_NOA_LEN + 1;
            mac_cfg_p2p_noa_param_stru st_p2p_noa_param;

            l_ret = wal_ioctl_judge_input_param_length(st_priv_cmd, CMD_P2P_SET_NOA_LEN, OAL_SIZEOF(st_p2p_noa_param));
            if (l_ret != OAL_SUCC) {
                OAM_ERROR_LOG1(0, OAM_SF_ANY, "{wal_vendor_priv_cmd:: CMD_P2P_SET_NOA length is too short! \
                at least need [%d]!}\r\n", skip + OAL_SIZEOF(st_p2p_noa_param));
                oal_free(pc_command);
                return -OAL_EFAIL;
            }
            l_memcpy_ret += memcpy_s(&st_p2p_noa_param, OAL_SIZEOF(mac_cfg_p2p_noa_param_stru),
                                     pc_command + skip, OAL_SIZEOF(mac_cfg_p2p_noa_param_stru));
            l_ret = wal_ioctl_set_p2p_noa(pst_net_dev, &st_p2p_noa_param);
        } else if (oal_strncmp(pc_command, CMD_P2P_SET_PS, CMD_P2P_SET_PS_LEN) == 0) {
            oal_uint32 skip = CMD_P2P_SET_PS_LEN + 1;
            mac_cfg_p2p_ops_param_stru st_p2p_ops_param;

            l_ret = wal_ioctl_judge_input_param_length(st_priv_cmd, CMD_P2P_SET_PS_LEN, OAL_SIZEOF(st_p2p_ops_param));
            if (l_ret != OAL_SUCC) {
                OAM_ERROR_LOG1(0, OAM_SF_ANY, "{wal_vendor_priv_cmd:: CMD_P2P_SET_PS length is too short! \
                at least need [%d]!}\r\n", skip + OAL_SIZEOF(st_p2p_ops_param));
                oal_free(pc_command);
                return -OAL_EFAIL;
            }
            l_memcpy_ret += memcpy_s(&st_p2p_ops_param, OAL_SIZEOF(mac_cfg_p2p_ops_param_stru),
                                     pc_command + skip, OAL_SIZEOF(mac_cfg_p2p_ops_param_stru));
            l_ret = wal_ioctl_set_p2p_ops(pst_net_dev, &st_p2p_ops_param);
        } else if (oal_strncmp(pc_command, CMD_SET_STA_PM_ON, CMD_SET_STA_PM_ON_LEN) == 0) {
            mac_pm_switch_enum_uint8 en_pm_enable;

            l_ret = wal_ioctl_judge_input_param_length(st_priv_cmd, CMD_SET_STA_PM_ON_LEN, 1);
            if (l_ret != OAL_SUCC) {
                OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_vendor_priv_cmd:: CMD_SET_STA_PM_ON length is too short! \
                at least need [%d]!}\r\n", (CMD_SET_STA_PM_ON_LEN + 2)); /* �ַ�����������ΪCMD_SET_STA_PM_ON_LEN + 2 */
                oal_free(pc_command);
                return -OAL_EFAIL;
            }
            en_pm_enable = *(pc_command + CMD_SET_STA_PM_ON_LEN + 1) - '0';
            l_ret = wal_ioctl_set_sta_pm(pst_net_dev, en_pm_enable, MAC_STA_PM_CTRL_TYPE_HOST);
        } else if (oal_strncmp(pc_command, CMD_SET_QOS_MAP, CMD_SET_QOS_MAP_LEN) == 0) {
            oal_uint32 skip = CMD_SET_QOS_MAP_LEN + 1;
            hmac_cfg_qos_map_param_stru st_qos_map_param;
            l_ret = wal_ioctl_judge_input_param_length(st_priv_cmd, CMD_SET_QOS_MAP_LEN, OAL_SIZEOF(st_qos_map_param));
            if (l_ret != OAL_SUCC) {
                OAM_WARNING_LOG1(0, OAM_SF_ANY, "{CMD_SET_QOS_MAP[%d]!}", (skip + OAL_SIZEOF(st_qos_map_param)));
                oal_free(pc_command);
                return -OAL_EFAIL;
            }
            l_memcpy_ret += memcpy_s(&st_qos_map_param, OAL_SIZEOF(hmac_cfg_qos_map_param_stru),
                pc_command + skip, OAL_SIZEOF(hmac_cfg_qos_map_param_stru));
            l_ret = wal_ioctl_set_qos_map(pst_net_dev, &st_qos_map_param);
        } else if (oal_strncmp(pc_command, CMD_COUNTRY, CMD_COUNTRY_LEN) == 0) {
            l_ret = wal_ioctl_priv_cmd_country(pst_net_dev, pc_command, st_priv_cmd);
        }
#ifdef _PRE_WLAN_FEATURE_LTECOEX
        else if (oal_strncmp(pc_command, CMD_LTECOEX_MODE, CMD_LTECOEX_MODE_LEN) == 0) {
            l_ret = wal_ioctl_priv_cmd_ltecoex_mode(pst_net_dev, pc_command, st_priv_cmd);
        }
#endif
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
        else if (oal_strncmp(pc_command, CMD_TX_POWER, CMD_TX_POWER_LEN) == 0) {
            l_ret = wal_ioctl_priv_cmd_tx_power(pst_net_dev, pc_command, st_priv_cmd);
        }
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
        else if (oal_strncmp(pc_command, CMD_WPAS_GET_CUST, CMD_WPAS_GET_CUST_LEN) == 0) {
            /* ��buf���� */
            memset_s(pc_command, st_priv_cmd.ul_total_len + 1, 0, st_priv_cmd.ul_total_len + 1);
            pc_command[st_priv_cmd.ul_total_len] = '\0';
            /* ��ȡȫ�����ƻ����ã���������ȡdisable_capab_ht40 */
            hwifi_config_init_force();
            /* ��ֵht40��ֹλ */
            *pc_command = wlan_customize.uc_disable_capab_2ght40;
            if (oal_copy_to_user(pst_ifr->ifr_data + PRIV_CMD_STRU_HEAD_LEN,
                                 pc_command, (oal_uint32)(st_priv_cmd.ul_total_len))) {
                OAM_ERROR_LOG0(0, OAM_SF_ANY, "wal_vendor_priv_cmd: Failed to copy ioctl_data to user !");
                oal_free(pc_command);
                /* ���ش���֪ͨsupplicant����ʧ�ܣ�supplicant���������������� */
                return -OAL_EFAIL;
            }
        }
#endif
#endif
#ifdef _PRE_WLAN_FEATURE_VOWIFI
        else if (oal_strncmp(pc_command, CMD_VOWIFI_SET_PARAM, CMD_VOWIFI_SET_PARAM_LEN) == 0) {
            l_ret = wal_ioctl_set_vowifi_param(pst_net_dev, pc_command, st_priv_cmd);
            if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
                OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_vendor_priv_cmd::VOWIFI_SET_PARAM return err code [%d]!}", l_ret);
                oal_free(pc_command);
                return -OAL_EFAIL;
            }
        } else if (oal_strncmp(pc_command, CMD_VOWIFI_GET_PARAM, CMD_VOWIFI_GET_PARAM_LEN) == 0) {
            l_ret = wal_ioctl_process_get_vowifi_params(pst_net_dev, pst_ifr, pc_command, &st_priv_cmd);
            if (l_ret != OAL_SUCC) {
                oal_free(pc_command);
                return l_ret;
            }
        }
#endif /* _PRE_WLAN_FEATURE_VOWIFI */
#ifndef CONFIG_HAS_EARLYSUSPEND
        else if (oal_strncmp(pc_command, CMD_SETSUSPENDMODE, CMD_SETSUSPENDMODE_LEN) == 0) {
            l_ret = wal_ioctl_judge_input_param_length(st_priv_cmd, CMD_SETSUSPENDMODE_LEN, 1);
            if (l_ret != OAL_SUCC) {
                OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_vendor_priv_cmd:CMD_SETSUSPENDMODE length too short, need=%d",
                                 (CMD_SETSUSPENDMODE_LEN + 2)); /* �ַ�����������ΪCMD_SETSUSPENDMODE_LEN + 2 */
                oal_free(pc_command);
                return -OAL_EFAIL;
            }
            l_ret = wal_ioctl_set_suspend_mode(pst_net_dev, *(pc_command + CMD_SETSUSPENDMODE_LEN + 1) - '0');
        }
#endif
        else if (oal_strncmp(pc_command, CMD_GET_WIFI_PRIV_FEATURE_CAPABILITY,
                             CMD_GET_WIFI_PRIV_FEATURE_CAPABILITY_LEN) == 0) {
            l_ret = wal_vendor_get_priv_feature_cap(pst_net_dev, pst_ifr, pc_command, &st_priv_cmd);
        } else if (oal_strncmp(pc_command, CMD_VOWIFI_IS_SUPPORT, CMD_VOWIFI_IS_SUPPORT_LEN) == 0) {
            if ((oal_uint32)st_priv_cmd.ul_total_len < CMD_VOWIFI_IS_SUPPORT_REPLY_LEN) {
                OAM_ERROR_LOG1(0, OAM_SF_ANY, "wal_vendor_priv_cmd:CMD_VOWIFI_IS_SUPPORT length is to short. need %d",
                               CMD_VOWIFI_IS_SUPPORT_REPLY_LEN);
                oal_free(pc_command);
                return -OAL_EFAIL;
            }
            /* ��buf���� */
            memset_s (pc_command, (oal_uint32)(st_priv_cmd.ul_total_len + 1), 0,
                      (oal_uint32)(st_priv_cmd.ul_total_len + 1));
            pc_command[st_priv_cmd.ul_total_len] = '\0';
            l_memcpy_ret += memcpy_s(pc_command, (oal_uint32)(st_priv_cmd.ul_total_len),
                                     CMD_VOWIFI_IS_SUPPORT_REPLY, CMD_VOWIFI_IS_SUPPORT_REPLY_LEN);
            if (oal_copy_to_user(pst_ifr->ifr_data + PRIV_CMD_STRU_HEAD_LEN, pc_command,
                                 CMD_VOWIFI_IS_SUPPORT_REPLY_LEN)) {
                OAM_ERROR_LOG0(0, OAM_SF_ANY, "wal_vendor_priv_cmd:CMD_VOWIFI_IS_SUPPORT Failed copy to user");
                oal_free(pc_command);
                return -OAL_EFAIL;
            }
        }
#ifdef _PRE_WLAN_FEATURE_IP_FILTER
        else if (oal_strncmp(pc_command, CMD_FILTER_SWITCH, CMD_FILTER_SWITCH_LEN) == 0) {
#ifdef CONFIG_DOZE_FILTER
            l_ret = wal_vendor_config_filter_cmd(&st_priv_cmd, pc_command);
            if (l_ret != OAL_SUCC) {
                return -OAL_EFAIL;
            }
#else
            OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_vendor_priv_cmd::Not support CMD_FILTER_SWITCH.}");
#endif
        }
#endif /* _PRE_WLAN_FEATURE_IP_FILTER */
        else if (oal_strncmp(pc_command, CMD_RXFILTER_START, CMD_RXFILTER_START_LEN) == 0) {
            wal_ioctl_force_pass_filter(pst_net_dev, OAL_TRUE);
        } else if (oal_strncmp(pc_command, CMD_RXFILTER_STOP, CMD_RXFILTER_STOP_LEN) == 0) {
            wal_ioctl_force_pass_filter(pst_net_dev, OAL_FALSE);
        }
#ifdef _PRE_WLAN_FEATURE_NRCOEX
        else if (oal_strncmp(pc_command, CMD_SET_NRCOEX_PRIOR, CMD_SET_NRCOEX_PRIOR_LEN) == 0) {
            l_ret = wal_ioctl_process_nrcoex_priority(pst_net_dev, pc_command);
        } else if (oal_strncmp(pc_command, CMD_GET_NRCOEX_INFO, CMD_GET_NRCOEX_INFO_LEN) == 0) {
            l_ret = wal_ioctl_get_nrcoex_info(pst_net_dev, pst_ifr, pc_command);
        }
#endif
        else {
            /* Ȧ���Ӷ����ƣ�������չ˽������ӿ� */
            l_ret = wal_vendor_priv_append_cmd(pst_net_dev, pc_command, st_priv_cmd, pst_ifr);
        }
    if (l_memcpy_ret != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "wal_vendor_priv_cmd::memcpy fail!");
    }
    oal_free(pc_command);
    return l_ret;
#else
    return OAL_SUCC;
#endif
}

oal_int32 wal_net_device_ioctl(oal_net_device_stru *pst_net_dev, oal_ifreq_stru *pst_ifr, oal_int32 ul_cmd)
{
    oal_int32 l_ret = 0;

    if ((pst_net_dev == OAL_PTR_NULL) || (pst_ifr == OAL_PTR_NULL)) {
        OAM_ERROR_LOG2(0, OAM_SF_ANY, "{wal_net_device_ioctl::pst_dev %p, pst_ifr %p!}\r\n",
                       (uintptr_t)pst_net_dev, (uintptr_t)pst_ifr);
        return -OAL_EFAUL;
    }

    if (pst_ifr->ifr_data == OAL_PTR_NULL) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "{wal_net_device_ioctl::pst_ifr->ifr_data is NULL, ul_cmd[0x%x]!}\r\n", ul_cmd);
        return -OAL_EFAUL;
    }

#if (_PRE_OS_VERSION == _PRE_OS_VERSION_LINUX)
    if (!capable(CAP_NET_ADMIN)) {
        return -EPERM;
    }
#endif

    /* 1102 wpa_supplicant ͨ��ioctl �·����� */
    if (ul_cmd == WAL_SIOCDEVPRIVATE + 1) {
        l_ret = wal_vendor_priv_cmd(pst_net_dev, pst_ifr, ul_cmd);
        return l_ret;
    }
#if (_PRE_OS_VERSION == _PRE_OS_VERSION_LINUX)
    /* atcmdsrv ͨ��ioctl�·����� */
    else if (ul_cmd == (WAL_SIOCDEVPRIVATE + 2)) { /* �ж�ul_cmd�Ƿ�Ϊ0x89F0 + 2 */
        if (hi11xx_get_os_build_variant() == HI1XX_OS_BUILD_VARIANT_ROOT) {
            wal_wake_lock();
            l_ret = wal_atcmdsrv_wifi_priv_cmd(pst_net_dev, pst_ifr, ul_cmd);
            wal_wake_unlock();
        }
        return l_ret;
    }
#endif
    else {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "{wal_net_device_ioctl::unrecognised cmd[0x%x]!}\r\n", ul_cmd);
        return OAL_SUCC;
    }
}


OAL_STATIC oal_uint32 wal_hipriv_set_mode(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    oal_int8 ac_mode_str[WAL_HIPRIV_CMD_NAME_MAX_LEN] = { 0 }; /* Ԥ��Э��ģʽ�ַ����ռ� */
    oal_uint8 uc_prot_idx;
    mac_cfg_mode_param_stru *pst_mode_param = OAL_PTR_NULL;
    wal_msg_write_stru st_write_msg;
    oal_uint32 ul_off_set;
    oal_uint32 ul_ret;
    oal_int32 l_ret;

    if (OAL_UNLIKELY((pst_net_dev == OAL_PTR_NULL) || (pc_param == OAL_PTR_NULL))) {
        OAM_ERROR_LOG2(0, OAM_SF_ANY, "{wal_hipriv_set_mode::pst_net_dev/p_param null ptr error %x, %x!}\r\n",
                       (uintptr_t)pst_net_dev, (uintptr_t)pc_param);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    /* pc_paramָ����ģʽ����, ����ȡ����ŵ�ac_mode_str�� */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_mode_str, OAL_SIZEOF(ac_mode_str), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_hipriv_set_mode::wal_get_cmd_one_arg vap name return err_code %d", ul_ret);
        return ul_ret;
    }

    ac_mode_str[OAL_SIZEOF(ac_mode_str) - 1] = '\0'; /* ȷ����null��β */

    for (uc_prot_idx = 0; g_ast_mode_map[uc_prot_idx].pc_name != OAL_PTR_NULL; uc_prot_idx++) {
        l_ret = strcmp(g_ast_mode_map[uc_prot_idx].pc_name, ac_mode_str);
        if (l_ret == 0) {
            break;
        }
    }

    if (g_ast_mode_map[uc_prot_idx].pc_name == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_hipriv_set_mode::unrecognized protocol string!}\r\n");
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    /***************************************************************************
        ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_MODE, OAL_SIZEOF(mac_cfg_mode_param_stru));

    pst_mode_param = (mac_cfg_mode_param_stru *)(st_write_msg.auc_value);
    pst_mode_param->en_protocol = g_ast_mode_map[uc_prot_idx].en_mode;
    pst_mode_param->en_band = g_ast_mode_map[uc_prot_idx].en_band;
    pst_mode_param->en_bandwidth = g_ast_mode_map[uc_prot_idx].en_bandwidth;

    OAM_INFO_LOG3(0, OAM_SF_CFG, "{wal_hipriv_set_mode::protocol[%d], band[%d], bandwidth[%d]!}\r\n",
                  pst_mode_param->en_protocol, pst_mode_param->en_band, pst_mode_param->en_bandwidth);

    /* ������Ϣ */
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_mode_param_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_set_mode::wal_alloc_cfg_event return err code %d!}\r\n", l_ret);
        return (oal_uint32)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC oal_uint32 wal_hipriv_set_freq(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    wal_msg_write_stru st_write_msg;
    oal_int32 l_freq;
    oal_uint32 ul_off_set;
    oal_int8 ac_freq[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    oal_uint32 ul_ret;
    oal_int32 l_ret;

    /* pc_paramָ���´�����net_device��name, ����ȡ����ŵ�ac_name�� */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_freq, OAL_SIZEOF(ac_freq), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_hipriv_set_freq:wal_get_cmd_one_arg vap name return err_code %d", ul_ret);
        return ul_ret;
    }

    l_freq = oal_atoi(ac_freq);
    OAM_INFO_LOG1(0, OAM_SF_ANY, "{wal_hipriv_set_freq::l_freq = %d!}\r\n", l_freq);

    /***************************************************************************
        ���¼���wal�㴦��
    ***************************************************************************/
    /* ��д��Ϣ */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_CURRENT_CHANEL, OAL_SIZEOF(oal_int32));
    *((oal_int32 *)(st_write_msg.auc_value)) = l_freq;

    /* ������Ϣ */
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(oal_int32),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_set_freq::return err code %d!}\r\n", l_ret);
        return (oal_uint32)l_ret;
    }

    return OAL_SUCC;
}

#ifdef _PRE_BT_FITTING_DATA_COLLECT
OAL_STATIC oal_uint32 wal_hipriv_init_bt_env(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    wal_msg_write_stru st_write_msg;
    oal_int32 l_ret;

    /***************************************************************************
        ���¼���wal�㴦��
    ***************************************************************************/
    /* ��д��Ϣ */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_INIT_BT_ENV, OAL_SIZEOF(oal_uint8));

    /* ������Ϣ */
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(oal_uint8),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_init_bt_env::return err code %d!}\r\n", l_ret);
        return (oal_uint32)l_ret;
    }

    return OAL_SUCC;
}

OAL_STATIC oal_uint32 wal_hipriv_set_bt_freq(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    wal_msg_write_stru st_write_msg;
    oal_int32 l_freq;
    oal_uint32 ul_off_set;
    oal_int8 ac_freq[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    oal_uint32 ul_ret;
    oal_int32 l_ret;

    /* pc_paramָ���´�����net_device��name, ����ȡ����ŵ�ac_name�� */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_freq, OAL_SIZEOF(ac_freq), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_hipriv_set_bt_freq:wal_get_cmd_one_arg fail, err_code %d", ul_ret);
        return ul_ret;
    }

    l_freq = oal_atoi(ac_freq);
    OAM_INFO_LOG1(0, OAM_SF_ANY, "{wal_hipriv_set_bt_freq::l_freq = %d!}\r\n", l_freq);

    /***************************************************************************
        ���¼���wal�㴦��
    ***************************************************************************/
    /* ��д��Ϣ */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_BT_FREQ, OAL_SIZEOF(oal_int32));
    *((oal_int32 *)(st_write_msg.auc_value)) = l_freq;

    /* ������Ϣ */
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(oal_int32),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_set_bt_freq::return err code %d!}\r\n", l_ret);
        return (oal_uint32)l_ret;
    }

    return OAL_SUCC;
}

OAL_STATIC oal_uint32 wal_hipriv_set_bt_upc_by_freq(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    wal_msg_write_stru st_write_msg;
    oal_int32 l_freq;
    oal_uint32 ul_off_set;
    oal_int8 ac_freq[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    oal_uint32 ul_ret;
    oal_int32 l_ret;

    /* pc_paramָ���´�����net_device��name, ����ȡ����ŵ�ac_name�� */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_freq, OAL_SIZEOF(ac_freq), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_hipriv_set_bt_upc_by_freq:wal_get_cmd_one_arg fail, err_code %d", ul_ret);
        return ul_ret;
    }

    l_freq = oal_atoi(ac_freq);
    OAM_INFO_LOG1(0, OAM_SF_ANY, "{wal_hipriv_set_bt_upc_by_freq::l_freq = %d!}\r\n", l_freq);

    /***************************************************************************
        ���¼���wal�㴦��
    ***************************************************************************/
    /* ��д��Ϣ */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_BT_UPC_BY_FREQ, OAL_SIZEOF(oal_int32));
    *((oal_int32 *)(st_write_msg.auc_value)) = l_freq;

    /* ������Ϣ */
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(oal_int32),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_set_bt_upc_by_freq::return err code %d!}\r\n", l_ret);
        return (oal_uint32)l_ret;
    }

    return OAL_SUCC;
}

OAL_STATIC oal_uint32 wal_hipriv_print_bt_gm(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    wal_msg_write_stru st_write_msg;
    oal_int32 l_ret;

    /***************************************************************************
        ���¼���wal�㴦��
    ***************************************************************************/
    /* ��д��Ϣ */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_PRINT_BT_GM, OAL_SIZEOF(oal_uint8));

    /* ������Ϣ */
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(oal_uint8),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_print_bt_gm::return err code %d!}\r\n", l_ret);
        return (oal_uint32)l_ret;
    }

    return OAL_SUCC;
}
#endif


OAL_STATIC oal_uint32 wal_hipriv_set_tx_ba_policy(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    wal_msg_write_stru st_write_msg;
    oal_uint32 ul_tx_ba_policy;
    oal_uint32 ul_off_set;
    oal_int8 ac_value[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    oal_uint32 ul_ret;
    oal_int32 l_ret;

    /* pc_paramָ���´�����net_device��name, ����ȡ����ŵ�ac_name�� */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_value, OAL_SIZEOF(ac_value), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_set_tx_ba_policy::wal_get_cmd_one_arg fail, err_code %d", ul_ret);
        return ul_ret;
    }

    ul_tx_ba_policy = (oal_uint32)oal_atoi(ac_value);
    OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_set_tx_ba_policy::ul_tx_ba_policy = %d!}\r\n", ul_tx_ba_policy);

    /***************************************************************************
        ���¼���wal�㴦��
    ***************************************************************************/
    /* ��д��Ϣ */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_TX_BA_POLICY, OAL_SIZEOF(oal_uint32));
    *((oal_uint32 *)(st_write_msg.auc_value)) = ul_tx_ba_policy;

    /* ������Ϣ */
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(oal_uint32),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_set_tx_ba_policy::return err code %d!}\r\n", l_ret);
        return (oal_uint32)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC oal_uint32 wal_hipriv_dscr_th_opt(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    mac_cfg_dscr_th_stru *pst_dscr_th = OAL_PTR_NULL;
    wal_msg_write_stru st_write_msg;
    oal_uint32 ul_queue_id;
    oal_uint32 ul_start_th;
    oal_uint32 ul_interval;
    oal_uint32 ul_off_set;
    oal_uint32 ul_ret;
    oal_int32 l_ret;
    oal_int8 ac_value[WAL_HIPRIV_CMD_NAME_MAX_LEN];

    /* ��ȡqueue id����(0:������� 1:С������) */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_value, OAL_SIZEOF(ac_value), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_CFG, "wal_hipriv_dscr_th_opt:wal_get_cmd_one_arg fail, err_code %d", ul_ret);
        return ul_ret;
    }

    ul_queue_id = (oal_uint32)oal_atoi(ac_value);
    if ((ul_queue_id != 0) && (ul_queue_id != 1)) {
        OAM_WARNING_LOG1(0, OAM_SF_CFG, "{wal_hipriv_dscr_th_opt::ul_queue_id[%d] error!}", ul_ret);
        return OAL_FAIL;
    }

    pc_param += ul_off_set;

    /* ��ȡstart threshold���� */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_value, OAL_SIZEOF(ac_value), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_CFG, "wal_hipriv_dscr_th_opt::wal_get_cmd_one_arg fail, err_code %d", ul_ret);
        return ul_ret;
    }

    ul_start_th = (oal_uint32)oal_atoi(ac_value);

    pc_param += ul_off_set;

    /* ��ȡinterval���� */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_value, OAL_SIZEOF(ac_value), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_CFG, "wal_hipriv_dscr_th_opt:wal_get_cmd_one_arg fail, err_code %d", ul_ret);
        return ul_ret;
    }

    ul_interval = (oal_uint32)oal_atoi(ac_value);

    /***************************************************************************
        ���¼���wal�㴦��
    ***************************************************************************/
    /* ��д��Ϣ */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_DSCR_TH_HOST, OAL_SIZEOF(mac_cfg_dscr_th_stru));

    pst_dscr_th = (mac_cfg_dscr_th_stru *)st_write_msg.auc_value;
    pst_dscr_th->ul_queue_id = ul_queue_id;
    pst_dscr_th->ul_start_th = ul_start_th;
    pst_dscr_th->ul_interval = ul_interval;

    OAM_WARNING_LOG3(0, OAM_SF_CFG, "{wal_hipriv_dscr_th_opt::ul_queue_id[%d], ul_start_th[%d], ul_interval[%d]}",
                     pst_dscr_th->ul_queue_id, pst_dscr_th->ul_start_th, pst_dscr_th->ul_interval);

    /* ������Ϣ */
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_dscr_th_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_dscr_th_opt::return err code[%d]}", l_ret);
        return (oal_uint32)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC oal_uint32 wal_hipriv_tcp_ack_filter(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    wal_msg_write_stru st_write_msg;
    oal_uint32 ul_ret;
    oal_uint32 ul_off_set;
    oal_uint32 ul_switch;
    oal_int32 l_ret;
    oal_int8 ac_value[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    mac_cfg_tcp_ack_filter *pst_tcp_ack_filter = OAL_PTR_NULL;

    /* ��ȡ���ܿ��ز���(0:���ܹ� 1:���ܿ�) */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_value, OAL_SIZEOF(ac_value), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_CFG, "wal_hipriv_tcp_ack_filter::wal_get_cmd_one_arg fail, err_code %d", ul_ret);
        return ul_ret;
    }

    ul_switch = (oal_uint32)oal_atoi(ac_value);
    if ((ul_switch != 0) && (ul_switch != 1)) {
        OAM_WARNING_LOG1(0, OAM_SF_CFG, "{wal_hipriv_tcp_ack_filter::ul_switch[%d] error!}", ul_switch);
        return OAL_FAIL;
    }

    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_TCP_ACK_FILTER, OAL_SIZEOF(mac_cfg_tcp_ack_filter));

    pst_tcp_ack_filter = (mac_cfg_tcp_ack_filter *)st_write_msg.auc_value;
    pst_tcp_ack_filter->ul_switch = ul_switch;

    OAM_WARNING_LOG1(0, OAM_SF_CFG, "{wal_hipriv_tcp_ack_filter::ul_switch[%d]}", pst_tcp_ack_filter->ul_switch);

    /* ������Ϣ */
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_dscr_th_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_tcp_ack_filter::return err code[%d]}", l_ret);
        return (oal_uint32)l_ret;
    }

    return OAL_SUCC;
}

/* ��ʹ��iwconfig iwpriv ������hipriv �ӿ� */

oal_uint32 wal_hipriv_set_essid(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    oal_uint8 uc_ssid_len;
    oal_int32 l_ret;
    wal_msg_write_stru st_write_msg;
    mac_cfg_ssid_param_stru *pst_param = OAL_PTR_NULL;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    oal_uint32 ul_off_set;
    oal_int8 *pc_ssid = OAL_PTR_NULL;
    oal_int8 ac_ssid[WLAN_SSID_MAX_LEN] = { 0 };
    oal_uint32 ul_ret;

    if ((pst_net_dev == OAL_PTR_NULL) || (pc_param == OAL_PTR_NULL)) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_hipriv_set_essid::param pst_net_dev or pc_param is null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_mac_vap = OAL_NET_DEV_PRIV(pst_net_dev);
    if (pst_mac_vap == NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_hipriv_set_essid::pst_mac_vap is null!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_AP) {
        /* �豸��up״̬����APʱ�����������ã�������down */
        if ((OAL_IFF_RUNNING & OAL_NETDEVICE_FLAGS(pst_net_dev)) != 0) {
            OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_hipriv_set_essid::device is busy, please down it \
                firste %d!}", OAL_NETDEVICE_FLAGS(pst_net_dev));
            return OAL_ERR_CODE_CONFIG_BUSY;
        }
    }

    /* pc_paramָ����ģʽ����, ����ȡ����ŵ�ac_mode_str�� */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_ssid, OAL_SIZEOF(ac_ssid), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_hipriv_set_essid:wal_get_cmd_one_arg fail, err_code %d", ul_ret);
        return ul_ret;
    }

    pc_ssid = ac_ssid;
    pc_ssid = oal_strim(ac_ssid); /* ȥ���ַ�����ʼ��β�Ŀո� */
    uc_ssid_len = (oal_uint8)OAL_STRLEN(pc_ssid);

    OAM_INFO_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_hipriv_set_essid:: ssid length %d!}\r\n", uc_ssid_len);

    if (uc_ssid_len > WLAN_SSID_MAX_LEN - 1) { /* -1Ϊ\0Ԥ���ռ� */
        uc_ssid_len = WLAN_SSID_MAX_LEN - 1;
    }

    OAM_INFO_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_hipriv_set_essid:: ssid length is %d!}\r\n", uc_ssid_len);
    /***************************************************************************
        ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SSID, OAL_SIZEOF(mac_cfg_ssid_param_stru));

    /* ��дWID��Ӧ�Ĳ��� */
    pst_param = (mac_cfg_ssid_param_stru *)(st_write_msg.auc_value);
    pst_param->uc_ssid_len = uc_ssid_len;
    if (memcpy_s(pst_param->ac_ssid, OAL_SIZEOF(pst_param->ac_ssid), pc_ssid, uc_ssid_len) != EOK) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "wal_hipriv_set_essid::memcpy fail!");
        return OAL_FAIL;
    }

    /* ������Ϣ */
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_ssid_param_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "wal_hipriv_set_essid:wal_alloc_cfg_event fail, ret=%d",
                         l_ret);
        return (oal_uint32)l_ret;
    }

    return OAL_SUCC;
}

/*
 * �� �� ��  : wal_octl_get_essid
 * ��������  : ��ȡssid
 */
OAL_STATIC int wal_ioctl_get_essid(oal_net_device_stru *pst_net_dev, oal_iw_request_info_stru *pst_info,
                                   oal_iwreq_data_union *pst_data, char *pc_ssid)
{
    oal_int32 l_ret;
    wal_msg_query_stru st_query_msg;
    mac_cfg_ssid_param_stru *pst_ssid = OAL_PTR_NULL;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;
    wal_msg_rsp_stru *pst_query_rsp_msg = OAL_PTR_NULL;
    oal_iw_point_stru *pst_essid = (oal_iw_point_stru *)pst_data;

    /* ���¼���wal�㴦�� */
    st_query_msg.en_wid = WLAN_CFGID_SSID;

    /* ������Ϣ */
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_QUERY,
                               WAL_MSG_WID_LENGTH,
                               (oal_uint8 *)&st_query_msg,
                               OAL_TRUE,
                               &pst_rsp_msg);
    if ((l_ret != OAL_SUCC) || (pst_rsp_msg == OAL_PTR_NULL)) {
        if (pst_rsp_msg != OAL_PTR_NULL) {
            oal_free(pst_rsp_msg);
        }
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_ioctl_get_essid:: wal_send_cfg_event return err code %d!}", l_ret);
        return -OAL_EFAIL;
    }

    /* ��������Ϣ */
    pst_query_rsp_msg = (wal_msg_rsp_stru *)(pst_rsp_msg->auc_msg_data);

    /* ҵ���� */
    pst_ssid = (mac_cfg_ssid_param_stru *)(pst_query_rsp_msg->auc_value);
    pst_essid->flags = 1; /* ���ó��α�־Ϊ��Ч */
    pst_essid->length = OAL_MIN(pst_ssid->uc_ssid_len, OAL_IEEE80211_MAX_SSID_LEN);
    if (memcpy_s(pc_ssid, OAL_IEEE80211_MAX_SSID_LEN, pst_ssid->ac_ssid, pst_essid->length) != EOK) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "{wal_ioctl_get_essid::memcpy fail, essid len[%d]!}", pst_essid->length);
        oal_free(pst_rsp_msg);
        return -OAL_EINVAL;
    }

    oal_free(pst_rsp_msg);
    return OAL_SUCC;
}

/*
 * �� �� ��  : wal_ioctl_get_mode
 * ��������  : ��ȡ��������
 */
OAL_STATIC int wal_ioctl_get_mode(oal_net_device_stru *pst_net_dev,
                                  oal_iw_request_info_stru *pst_info,
                                  oal_iwreq_data_union *pst_wrqu,
                                  char *pc_param)
{
    oal_wireless_dev_stru *pst_wdev = OAL_PTR_NULL;
    oal_uint32 *pul_mode = OAL_PTR_NULL;

    if ((pst_net_dev == OAL_PTR_NULL) || (pst_wrqu == OAL_PTR_NULL)) {
        OAM_ERROR_LOG2(0, OAM_SF_ANY, "{wal_ioctl_get_mode::param null, pst_net_dev = %p, pst_wrqu = %p.}",
                       (uintptr_t)pst_net_dev, (uintptr_t)pst_wrqu);
        return -OAL_EINVAL;
    }

    pst_wdev = pst_net_dev->ieee80211_ptr;
    if (pst_wdev == OAL_PTR_NULL) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_ioctl_get_mode:: wireless device is null. net device id [%d]}",
                         pst_net_dev->ifindex);

        return -OAL_ENODEV;
    }

    pul_mode = (oal_uint32 *)pst_wrqu;

    switch (pst_wdev->iftype) {
        case NL80211_IFTYPE_AP:
            *pul_mode = OAL_IW_MODE_MASTER;
            break;
        case NL80211_IFTYPE_STATION:
            *pul_mode = OAL_IW_MODE_INFRA;
            break;
        case NL80211_IFTYPE_ADHOC:
            *pul_mode = OAL_IW_MODE_ADHOC;
            break;
        case NL80211_IFTYPE_MONITOR:
            *pul_mode = OAL_IW_MODE_MONITOR;
            break;
        case NL80211_IFTYPE_WDS:
            *pul_mode = OAL_IW_MODE_REPEAT;
            break;
        case NL80211_IFTYPE_AP_VLAN:
            *pul_mode = OAL_IW_MODE_SECOND;
            break;
        default:
            *pul_mode = OAL_IW_MODE_AUTO;
            break;
    }
    return OAL_SUCC;
}


OAL_STATIC int wal_ioctl_get_apaddr(oal_net_device_stru *pst_net_dev,
                                    oal_iw_request_info_stru *pst_info,
                                    oal_iwreq_data_union *pst_wrqu,
                                    char *pc_extra)
{
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    oal_sockaddr_stru *pst_addr = (oal_sockaddr_stru *)pst_wrqu;
    oal_uint8 auc_zero_addr[WLAN_MAC_ADDR_LEN] = { 0 };

    if ((pst_net_dev == OAL_PTR_NULL) || (pst_addr == OAL_PTR_NULL)) {
        OAM_ERROR_LOG2(0, OAM_SF_ANY, "{wal_ioctl_get_apaddr::param null, pst_net_dev = %p, pst_addr = %p.}",
                       (uintptr_t)pst_net_dev, (uintptr_t)pst_addr);
        return -OAL_EINVAL;
    }

    pst_mac_vap = OAL_NET_DEV_PRIV(pst_net_dev);
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_ioctl_get_apaddr::pst_mac_vap is null!}\r\n");
        return -OAL_EFAUL;
    }

    if (pst_mac_vap->en_vap_state == MAC_VAP_STATE_UP) {
        oal_set_mac_addr((oal_uint8 *)pst_addr->sa_data, pst_mac_vap->auc_bssid);
    } else {
        oal_set_mac_addr((oal_uint8 *)pst_addr->sa_data, auc_zero_addr);
    }

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_11D

OAL_STATIC oal_bool_enum_uint8 wal_is_alpha_upper(oal_int8 c_letter)
{
    if (c_letter >= 'A' && c_letter <= 'Z') {
        return OAL_TRUE;
    }

    return OAL_FALSE;
}


oal_uint8 wal_regdomain_get_band(oal_uint32 ul_start_freq, oal_uint32 ul_end_freq)
{
    if ((ul_start_freq > 2400) && (ul_end_freq < 2500)) { /* 2400-2500hz,WLAN_BAND_2G */
        return MAC_RC_START_FREQ_2;
    } else if ((ul_start_freq > 5000) && (ul_end_freq < 5870)) { /* 5000-5870hz,WLAN_BAND_5G */
        return MAC_RC_START_FREQ_5;
    } else if ((ul_start_freq > 4900) && (ul_end_freq < 4999)) { /* 4900-4999hz,WLAN_BAND_5G */
        return MAC_RC_START_FREQ_5;
    } else {
        return MAC_RC_START_FREQ_BUTT;
    }
}


oal_uint8 wal_regdomain_get_bw(oal_uint8 uc_bw)
{
    oal_uint8 uc_bw_map;

    switch (uc_bw) {
        case 80: /* ����Ϊ80MHz */
            uc_bw_map = MAC_CH_SPACING_80MHZ;
            break;
        case 40: /* ����Ϊ40MHz */
            uc_bw_map = MAC_CH_SPACING_40MHZ;
            break;
        case 20: /* ����Ϊ20MHz */
            uc_bw_map = MAC_CH_SPACING_20MHZ;
            break;
        default:
            uc_bw_map = MAC_CH_SPACING_BUTT;
            break;
    };

    return uc_bw_map;
}


oal_uint32 wal_regdomain_get_channel_2g(oal_uint32 ul_start_freq, oal_uint32 ul_end_freq)
{
    oal_uint32 ul_freq;
    oal_uint32 ul_i;
    oal_uint32 ul_ch_bmap = 0;

    for (ul_freq = ul_start_freq + 10; ul_freq <= (ul_end_freq - 10); ul_freq++) { /* ��ʼƵ���10������Ƶ���10 */
        for (ul_i = 0; ul_i < MAC_CHANNEL_FREQ_2_BUTT; ul_i++) {
            if (ul_freq == g_ast_freq_map_2g[ul_i].us_freq) {
                ul_ch_bmap |= (1 << ul_i);
            }
        }
    }

    return ul_ch_bmap;
}


oal_uint32 wal_regdomain_get_channel_5g(oal_uint32 ul_start_freq, oal_uint32 ul_end_freq)
{
    oal_uint32 ul_freq;
    oal_uint32 ul_i;
    oal_uint32 ul_ch_bmap = 0;
    /* �ŵ���5gƵ���ϣ���ʼƵ���10������Ƶ���10��Ƶ��ÿ�μ���5 */
    for (ul_freq = ul_start_freq + 10; ul_freq <= (ul_end_freq - 10); ul_freq += 5) {
        for (ul_i = 0; ul_i < MAC_CHANNEL_FREQ_5_BUTT; ul_i++) {
            if (ul_freq == g_ast_freq_map_5g[ul_i].us_freq) {
                ul_ch_bmap |= (1 << ul_i);
            }
        }
    }

    return ul_ch_bmap;
}


oal_uint32 wal_regdomain_get_channel(oal_uint8 uc_band, oal_uint32 ul_start_freq, oal_uint32 ul_end_freq)
{
    oal_uint32 ul_ch_bmap = 0;
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
oal_uint32 wal_linux_update_wiphy_channel_list_num(oal_net_device_stru *pst_net_dev, oal_wiphy_stru *pst_wiphy)
{
    oal_uint16 us_len;
    oal_uint32 ul_ret;
    mac_vendor_cmd_channel_list_stru st_channel_list;
    mac_vap_stru *pst_mac_vap;

    if ((pst_wiphy == OAL_PTR_NULL) || (pst_net_dev == OAL_PTR_NULL)) {
        OAM_ERROR_LOG2(0, OAM_SF_ANY, "{wal_linux_update_wiphy_channel_list_num::wiphy %p, net_dev %p}",
                       (uintptr_t)pst_wiphy, (uintptr_t)pst_net_dev);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_mac_vap = OAL_NET_DEV_PRIV(pst_net_dev);
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_linux_update_wiphy_channel_list_num::NET_DEV_PRIV is NULL.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    ul_ret = hmac_config_vendor_cmd_get_channel_list(pst_mac_vap, &us_len, (oal_uint8 *)(&st_channel_list));
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_linux_update_wiphy_channel_list_num::get_channel_list fail. %d}", ul_ret);
        return ul_ret;
    }

    /* ֻ����2G�ŵ�������5G �ŵ����ڴ���DFS �����Ҵ�����㲢�����⣬����Ҫ�޸� */
    hi1151_band_2ghz.n_channels = st_channel_list.uc_channel_num_2g;

    OAM_WARNING_LOG2(0, OAM_SF_ANY, "{wal_linux_update_wiphy_channel_list_num::2g_channel_num=%d, 5g_channel_num=%d}",
                     st_channel_list.uc_channel_num_2g,
                     st_channel_list.uc_channel_num_5g);
    return OAL_SUCC;
}
#endif

OAL_STATIC OAL_INLINE oal_void wal_get_dfs_domain(
    mac_regdomain_info_stru *pst_mac_regdom, OAL_CONST oal_int8 *pc_country)
{
    oal_uint32 u_idx;
    oal_uint32 ul_size = OAL_ARRAY_SIZE(g_ast_dfs_domain_table);

    for (u_idx = 0; u_idx < ul_size; u_idx++) {
        if (strcmp(g_ast_dfs_domain_table[u_idx].pc_country, pc_country) == 0) {
            pst_mac_regdom->en_dfs_domain = g_ast_dfs_domain_table[u_idx].en_dfs_domain;

            return;
        }
    }

    pst_mac_regdom->en_dfs_domain = MAC_DFS_DOMAIN_NULL;
}


OAL_STATIC oal_void wal_regdomain_fill_info(
    OAL_CONST oal_ieee80211_regdomain_stru *pst_regdom, mac_regdomain_info_stru *pst_mac_regdom)
{
    oal_uint32 ul_i;
    oal_uint32 ul_start;
    oal_uint32 ul_end;
    oal_uint8 uc_band;
    oal_uint8 uc_bw;
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
    regdomain_enum en_regdomin;
#endif
    /* ���ƹ����ַ��� */
    pst_mac_regdom->ac_country[0] = pst_regdom->alpha2[0];
    pst_mac_regdom->ac_country[1] = pst_regdom->alpha2[1];
    pst_mac_regdom->ac_country[2] = 0;

    /* ��ȡDFS��֤��׼���� */
    wal_get_dfs_domain(pst_mac_regdom, pst_regdom->alpha2);

    /* ����������� */
    pst_mac_regdom->uc_regclass_num = (oal_uint8)pst_regdom->n_reg_rules;

    /* ����������Ϣ */
    for (ul_i = 0; ul_i < pst_regdom->n_reg_rules; ul_i++) {
        /* ��д�������Ƶ��(2.4G��5G) */
        ul_start = pst_regdom->reg_rules[ul_i].freq_range.start_freq_khz / 1000;
        ul_end = pst_regdom->reg_rules[ul_i].freq_range.end_freq_khz / 1000;
        uc_band = wal_regdomain_get_band(ul_start, ul_end);
        pst_mac_regdom->ast_regclass[ul_i].en_start_freq = uc_band;

        /* ��д����������������� */
        uc_bw = (oal_uint8)(pst_regdom->reg_rules[ul_i].freq_range.max_bandwidth_khz / 1000);
        pst_mac_regdom->ast_regclass[ul_i].en_ch_spacing = wal_regdomain_get_bw(uc_bw);

        /* ��д�������ŵ�λͼ */
        pst_mac_regdom->ast_regclass[ul_i].ul_channel_bmap = wal_regdomain_get_channel(uc_band, ul_start, ul_end);

        /* ��ǹ�������Ϊ */
        pst_mac_regdom->ast_regclass[ul_i].uc_behaviour_bmap = 0;

        if (pst_regdom->reg_rules[ul_i].flags & NL80211_RRF_DFS) {
            pst_mac_regdom->ast_regclass[ul_i].uc_behaviour_bmap |= MAC_RC_DFS;
        }

        /* ��串���������͹��� */
        pst_mac_regdom->ast_regclass[ul_i].uc_coverage_class = 0;
        pst_mac_regdom->ast_regclass[ul_i].uc_max_reg_tx_pwr =
           (oal_uint8)(pst_regdom->reg_rules[ul_i].power_rule.max_eirp / 100);
        pst_mac_regdom->ast_regclass[ul_i].uc_max_tx_pwr =
           (oal_uint8)(pst_regdom->reg_rules[ul_i].power_rule.max_eirp / 100);
    }
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
    en_regdomin = hwifi_get_regdomain_from_country_code(pst_mac_regdom->ac_country);
    /* ����Ƿ�ΪFCC��CE���� */
    pst_mac_regdom->uc_regdomain_type = en_regdomin;
#endif
}


oal_int32 wal_regdomain_update(oal_net_device_stru *pst_net_dev, oal_int8 *pc_country)
{
    oal_uint8 uc_dev_id;
    mac_device_stru *pst_device = OAL_PTR_NULL;
    mac_board_stru *pst_hmac_board = OAL_PTR_NULL;
    OAL_CONST oal_ieee80211_regdomain_stru *pst_regdom;
    oal_uint16 us_size;
    mac_regdomain_info_stru *pst_mac_regdom = OAL_PTR_NULL;
    wal_msg_write_stru st_write_msg;
    mac_cfg_country_stru *pst_param = OAL_PTR_NULL;
    oal_int32 l_ret;
#ifndef _PRE_PLAT_FEATURE_CUSTOMIZE
    oal_int8 *pc_current_country = OAL_PTR_NULL;
#endif
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;

#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
    oal_int8 old_pc_country[COUNTRY_CODE_LEN] = { '9', '9' };
    if (memcpy_s(old_pc_country, COUNTRY_CODE_LEN, hwifi_get_country_code(), COUNTRY_CODE_LEN) != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_regdomain_update::memcpy fail!}");
    }
    hwifi_set_country_code(pc_country, COUNTRY_CODE_LEN);

    /* ����µĹ�����;ɹ��Ҵ���һ��regdomain����ˢ��RF������ֻ���¹����� */
    if (hwifi_is_regdomain_changed(old_pc_country, pc_country) == OAL_TRUE) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_regdomain_update::regdomain changed, refresh rf params.!}\r\n");

        /* ˢ�²���ʧ�ܣ�Ϊ�˱�֤������͹��ʲ�����Ӧ */
        /* �����������ԭ���Ĺ����룬���θ���ʧ�� */
        if (hwifi_force_refresh_rf_params(pst_net_dev) != OAL_SUCC) {
            OAM_WARNING_LOG0(0, OAM_SF_ANY, "wal_regdomain_update:refresh rf params failed. Set country back");
            hwifi_set_country_code(old_pc_country, COUNTRY_CODE_LEN);
        }
    }
#endif

    if (!wal_is_alpha_upper(pc_country[0]) || !wal_is_alpha_upper(pc_country[1])) {
        if ((pc_country[0] != '9') || (pc_country[1] != '9')) {
            OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_regdomain_update::country str is invalid!}\r\n");
            return -OAL_EINVAL;
        }
    }

#ifndef _PRE_PLAT_FEATURE_CUSTOMIZE
    pc_current_country = mac_regdomain_get_country();
    /* ��ǰ��������Ҫ���õĹ�����һ�£�ֱ�ӷ��� */
    if ((pc_country[0] == pc_current_country[0])
        && (pc_country[1] == pc_current_country[1])) {
        return OAL_SUCC;
    }
#endif /* #ifndef _PRE_PLAT_FEATURE_CUSTOMIZE */

    pst_regdom = wal_regdb_find_db(pc_country);
    if (pst_regdom == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_regdomain_update::no regdomain db was found!}\r\n");
        return -OAL_EINVAL;
    }

    us_size = (oal_uint16)(OAL_SIZEOF(mac_regclass_info_stru) * pst_regdom->n_reg_rules + MAC_RD_INFO_LEN);

    /* �����ڴ��Ź�������Ϣ�����ڴ�ָ����Ϊ�¼�payload����ȥ */
    /* �˴�������ڴ����¼��������ͷ�(hmac_config_set_country) */
    pst_mac_regdom = OAL_MEM_ALLOC(OAL_MEM_POOL_ID_LOCAL, us_size, OAL_TRUE);
    if (pst_mac_regdom == OAL_PTR_NULL) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "wal_regdomain_update::alloc regdom mem fail(size:%d), return null ptr", us_size);
        return -OAL_ENOMEM;
    }

    wal_regdomain_fill_info(pst_regdom, pst_mac_regdom);

    /***************************************************************************
        ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_COUNTRY, OAL_SIZEOF(mac_cfg_country_stru));

    /* ��дWID��Ӧ�Ĳ��� */
    pst_param = (mac_cfg_country_stru *)(st_write_msg.auc_value);
    pst_param->p_mac_regdom = pst_mac_regdom;

    /* ������Ϣ */
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_country_stru),
                               (oal_uint8 *)&st_write_msg, OAL_TRUE, &pst_rsp_msg);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_regdomain_update::return err code %d!}\r\n", l_ret);
        if ((l_ret != -OAL_ETIMEDOUT) && (l_ret != -OAL_EFAUL)) {
            OAL_MEM_FREE(pst_mac_regdom, OAL_TRUE);
        }

        if (pst_rsp_msg != OAL_PTR_NULL) {
            oal_free(pst_rsp_msg);
        }
        return l_ret;
    }
    oal_free(pst_rsp_msg);

    /* ������֧��ACSʱ������hostapd��������Ϣ; �������֧��ACS������Ҫ���£�����hostapd�޷�����DFS�ŵ� */
    hmac_board_get_instance(&pst_hmac_board);
    uc_dev_id = pst_hmac_board->ast_chip[0].auc_device_id[0];
    pst_device = mac_res_get_dev(uc_dev_id);
    if ((pst_device != OAL_PTR_NULL) && (pst_device->pst_wiphy != OAL_PTR_NULL)) {
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 3, 0))
        
        wal_linux_update_wiphy_channel_list_num(pst_net_dev, pst_device->pst_wiphy);
#endif
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_regdomain_update::update regdom to kernel.}\r\n");
        wal_cfg80211_reset_bands();
        oal_wiphy_apply_custom_regulatory(pst_device->pst_wiphy, pst_regdom);
        
        wal_cfg80211_save_bands();
    }

    return OAL_SUCC;
}

oal_uint32 wal_regdomain_update_sta(oal_uint8 uc_vap_id)
{
    oal_int8 *pc_desired_country = OAL_PTR_NULL;

    oal_net_device_stru *pst_net_dev = OAL_PTR_NULL;
    oal_int32 l_ret;
    oal_bool_enum_uint8 us_updata_rd_by_ie_switch = OAL_FALSE;

    hmac_vap_get_updata_rd_by_ie_switch(uc_vap_id, &us_updata_rd_by_ie_switch);

    if (us_updata_rd_by_ie_switch == OAL_TRUE) {
        pc_desired_country = hmac_vap_get_desired_country(uc_vap_id);
        if (OAL_UNLIKELY(pc_desired_country == OAL_PTR_NULL)) {
            OAM_ERROR_LOG0(uc_vap_id, OAM_SF_ANY, "{wal_regdomain_update_sta::pc_desired_country is null ptr!}\r\n");
            return OAL_ERR_CODE_PTR_NULL;
        }

        /* �����Ĺ�����ȫΪ0����ʾ�Զ�AP�Ĺ����벻���ڣ�����sta��ǰĬ�ϵĹ����� */
        if ((pc_desired_country[0] == 0) && (pc_desired_country[1] == 0)) {
            OAM_INFO_LOG0(uc_vap_id, OAM_SF_ANY, "wal_regdomain_update_sta:ap does not have country ie, use default");
            return OAL_SUCC;
        }

        pst_net_dev = hmac_vap_get_net_device(uc_vap_id);
        if (pst_net_dev == OAL_PTR_NULL) {
            OAM_ERROR_LOG0(uc_vap_id, OAM_SF_ANY, "{wal_regdomain_update_sta::pst_net_dev is null ptr!}\r\n");
            return OAL_ERR_CODE_PTR_NULL;
        }
        l_ret = wal_regdomain_update(pst_net_dev, pc_desired_country);
        if (l_ret != OAL_SUCC) {
            OAM_WARNING_LOG1(uc_vap_id, OAM_SF_ANY, "{wal_regdomain_update_sta::wal_regdomain_update err code %d!}",
                             l_ret);
            return OAL_FAIL;
        }

        OAM_INFO_LOG2(uc_vap_id, OAM_SF_ANY, "{wal_regdomain_update_sta::country is %u, %u!}\r\n",
                      (oal_uint8)pc_desired_country[0], (oal_uint8)pc_desired_country[1]);
    } else {
        OAM_INFO_LOG0(uc_vap_id, OAM_SF_ANY, "{wal_regdomain_update_sta::us_updata_rd_by_ie_switch is OAL_FALSE!}");
    }
    return OAL_SUCC;
}


oal_int32 wal_regdomain_update_country_code(oal_net_device_stru *pst_net_dev, oal_int8 *pc_country)
{
    oal_int32 l_ret;

    if ((pst_net_dev == OAL_PTR_NULL) || (pc_country == OAL_PTR_NULL)) {
        OAM_ERROR_LOG2(0, OAM_SF_ANY, "{wal_regdomain_update_country_code::null ptr.net_dev %p, country %p!}",
                       (uintptr_t)pst_net_dev, (uintptr_t)pc_country);
        return -OAL_EFAIL;
    }
    l_ret = wal_regdomain_update(pst_net_dev, pc_country);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_regdomain_update_country_code::update return err code [%d]!}", l_ret);
        return -OAL_EFAIL;
    }
    return OAL_SUCC;
}

#endif

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)

oal_int32 wal_set_random_mac_to_mib(oal_net_device_stru *pst_net_dev)
{
    oal_uint32 ul_ret;
    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL;
    wal_msg_stru *pst_cfg_msg = OAL_PTR_NULL;
    wal_msg_write_stru *pst_write_msg = OAL_PTR_NULL;
    mac_cfg_staion_id_param_stru *pst_param = OAL_PTR_NULL;
    mac_vap_stru *pst_mac_vap;
    oal_uint8 *puc_mac_addr = OAL_PTR_NULL;
    oal_wireless_dev_stru *pst_wdev = OAL_PTR_NULL; /* ����P2P ������p2p0 �� p2p-p2p0 MAC ��ַ��wlan0 ��ȡ */
    wlan_p2p_mode_enum_uint8 en_p2p_mode = WLAN_LEGACY_VAP_MODE;
    oal_int32 l_ret = EOK;

    pst_mac_vap = OAL_NET_DEV_PRIV(pst_net_dev);
    if (pst_mac_vap == NULL) {
        /* ��ʱ��ӣ��Ժ�ɾ����add:20151116����Ч��1month */
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_set_random_mac_to_mib::pst_mac_vap NULL}");
        oal_msleep(500); /* ����500���� */
        pst_mac_vap = OAL_NET_DEV_PRIV(pst_net_dev);
        if (pst_mac_vap == NULL) {
            OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_set_random_mac_to_mib::pst_mac_vap NULL}");
            return OAL_FAIL;
        }
        /* ��ʱ��ӣ��Ժ�ɾ�� */
    }

    if (pst_mac_vap->pst_mib_info == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_set_random_mac_to_mib::vap->mib_info is NULL !}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_wdev = pst_net_dev->ieee80211_ptr;

    switch (pst_wdev->iftype) {
        case NL80211_IFTYPE_P2P_DEVICE:
            en_p2p_mode = WLAN_P2P_DEV_MODE;
            /* ����P2P device MAC ��ַ */
            l_ret += memcpy_s(pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.auc_p2p0_dot11StationID,
                WLAN_MAC_ADDR_LEN, OAL_NETDEVICE_MAC_ADDR(pst_net_dev), WLAN_MAC_ADDR_LEN);
            break;

        case NL80211_IFTYPE_P2P_CLIENT:
        case NL80211_IFTYPE_P2P_GO:
            en_p2p_mode = (pst_wdev->iftype == NL80211_IFTYPE_P2P_GO) ? WLAN_P2P_GO_MODE : WLAN_P2P_CL_MODE;
            /* �����ϲ��·�ֵ������P2P interface MAC ��ַ */
            /* �ϲ㲻�·���������mac��ַ,��wal_cfg80211_add_p2p_interface_init��ʼ�� */
            l_ret += memcpy_s(pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.auc_dot11StationID, WLAN_MAC_ADDR_LEN,
                OAL_NETDEVICE_MAC_ADDR(pst_net_dev), WLAN_MAC_ADDR_LEN);
            break;
        default:
            if ((oal_strcmp("p2p0", pst_net_dev->name)) == 0) {
                en_p2p_mode = WLAN_P2P_DEV_MODE;
                /* ����P2P device MAC ��ַ */
                l_ret += memcpy_s(pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.auc_p2p0_dot11StationID,
                    WLAN_MAC_ADDR_LEN, OAL_NETDEVICE_MAC_ADDR(pst_net_dev), WLAN_MAC_ADDR_LEN);
                break;
            }

            en_p2p_mode = WLAN_LEGACY_VAP_MODE;
            /* �����ϲ��·�ֵ������wlan0 MAC ��ַ */
            l_ret += memcpy_s(pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.auc_dot11StationID, WLAN_MAC_ADDR_LEN,
                OAL_NETDEVICE_MAC_ADDR(pst_net_dev), WLAN_MAC_ADDR_LEN);
            break;
    }
    OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_set_random_mac_to_mib::p2p mode [%d]}", en_p2p_mode);
    if (l_ret != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_set_random_mac_to_mib::memcpy fail.}");
    }
    /* send the random mac to dmac */
    /***************************************************************************
        ���¼���wal�㴦��   copy from wal_netdev_set_mac_addr()
        gong TBD : ��Ϊ����ͨ�õ�config�ӿ�
    ***************************************************************************/
    ul_ret = wal_alloc_cfg_event(pst_net_dev, &pst_event_mem, NULL, &pst_cfg_msg,
                                 (WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_staion_id_param_stru)));
    if (OAL_UNLIKELY(ul_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_set_random_mac_to_mib() fail; return %d!}\r\n", ul_ret);
        return -OAL_ENOMEM;
    }

    /* ��д������Ϣ */
    WAL_CFG_MSG_HDR_INIT(&(pst_cfg_msg->st_msg_hdr),
                         WAL_MSG_TYPE_WRITE,
                         WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_staion_id_param_stru),
                         WAL_GET_MSG_SN());

    /* ��дWID��Ϣ */
    pst_write_msg = (wal_msg_write_stru *)pst_cfg_msg->auc_msg_data;
    WAL_WRITE_MSG_HDR_INIT(pst_write_msg, WLAN_CFGID_STATION_ID, OAL_SIZEOF(mac_cfg_staion_id_param_stru));

    pst_param = (mac_cfg_staion_id_param_stru *)pst_write_msg->auc_value; /* ��дWID��Ӧ�Ĳ��� */
#ifdef _PRE_WLAN_FEATURE_P2P
    /* ���ʹ��P2P����Ҫ��netdevice ��Ӧ��P2P ģʽ�����ò��������õ�hmac ��dmac */
    /* �Ա�ײ�ʶ���䵽p2p0 ��p2p-p2p0 cl */
    pst_param->en_p2p_mode = en_p2p_mode;
    if (en_p2p_mode == WLAN_P2P_DEV_MODE) {
        puc_mac_addr = pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.auc_p2p0_dot11StationID;
    } else
#endif
    {
        puc_mac_addr = pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.auc_dot11StationID;
    }
    oal_set_mac_addr(pst_param->auc_station_id, puc_mac_addr);

    OAM_WARNING_LOG4(0, OAM_SF_ANY, "{wal_set_random_mac_to_mib [%.2x:%.2x:xx:xx:%.2x:%.2x]..}\r\n",
                     /* puc_mac_addr��0��1��4��5byte��Ϊ���������ӡ */
                     puc_mac_addr[0], puc_mac_addr[1], puc_mac_addr[4], puc_mac_addr[5]);

    frw_event_dispatch_event(pst_event_mem); /* �ַ��¼� */
    FRW_EVENT_FREE(pst_event_mem);

    return OAL_SUCC;
}
#endif


oal_uint32 wal_hipriv_setcountry(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
#ifdef _PRE_WLAN_FEATURE_11D
    oal_int32 l_ret;
    oal_uint32 ul_ret;
    oal_uint32 ul_off_set;
    oal_int8 ac_arg[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    oal_int8 *puc_para = OAL_PTR_NULL;

    /* �豸��up״̬���������ã�������down */
    if ((OAL_IFF_RUNNING & OAL_NETDEVICE_FLAGS(pst_net_dev)) != 0) {
        OAM_INFO_LOG1(0, OAM_SF_ANY, "{wal_hipriv_setcountry::net_dev flags: %d!}", OAL_NETDEVICE_FLAGS(pst_net_dev));
        return OAL_EBUSY;
    }
    /* ��ȡ�������ַ��� */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, OAL_SIZEOF(ac_arg), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_setcountry::wal_get_cmd_one_arg return err_code [%d]!}", ul_ret);
        return ul_ret;
    }
    puc_para = &ac_arg[0];
    l_ret = wal_regdomain_update(pst_net_dev, puc_para);
    if (l_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_setcountry::regdomain_update return err code %d!}\r\n", l_ret);
        return (oal_uint32)l_ret;
    }

#else
    OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_hipriv_setcountry::_PRE_WLAN_FEATURE_11D is not define!}\r\n");

#endif
    return OAL_SUCC;
}


OAL_STATIC oal_uint32 wal_hipriv_getcountry(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
#ifdef _PRE_WLAN_FEATURE_11D
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;
    wal_msg_rsp_stru *pst_query_rsp_msg = OAL_PTR_NULL;
    wal_msg_query_stru st_query_msg;
    oal_int8 ac_tmp_buff[OAM_PRINT_FORMAT_LENGTH];
    oal_int32 l_ret;

    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    st_query_msg.en_wid = WLAN_CFGID_COUNTRY;

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_QUERY,
                               WAL_MSG_WID_LENGTH,
                               (oal_uint8 *)&st_query_msg,
                               OAL_TRUE,
                               &pst_rsp_msg);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        if (pst_rsp_msg != OAL_PTR_NULL) {
            oal_free(pst_rsp_msg);
        }
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_getcountry::wal_alloc_cfg_event return err code %d!}\r\n", l_ret);
        return (oal_uint32)l_ret;
    }

    /* ��������Ϣ */
    pst_query_rsp_msg = (wal_msg_rsp_stru *)(pst_rsp_msg->auc_msg_data);

    snprintf_s(ac_tmp_buff, sizeof(ac_tmp_buff), sizeof(ac_tmp_buff) - 1, "getcountry code is : %c%c.\n",
        pst_query_rsp_msg->auc_value[0],
        pst_query_rsp_msg->auc_value[1]);

    OAM_WARNING_LOG3(0, OAM_SF_CFG, "{wal_hipriv_getcountry:: %c, %c, len %d}",
                     pst_query_rsp_msg->auc_value[0],
                     pst_query_rsp_msg->auc_value[1],
                     pst_query_rsp_msg->us_len);

    oal_free(pst_rsp_msg);

    oam_print(ac_tmp_buff);
#else
    OAM_INFO_LOG0(0, OAM_SF_ANY, "{wal_hipriv_getcountry::_PRE_WLAN_FEATURE_11D is not define!}\r\n");
#endif

    return OAL_SUCC;
}

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION) && (LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 34))

OAL_STATIC oal_void *wal_sta_info_seq_start(struct seq_file *f, loff_t *pos)
{
    if (*pos == 0) {
        return f->private;
    } else {
        return NULL;
    }
}
OAL_STATIC oal_void wal_sta_info_seq_print(oal_uint16 us_idx, mac_user_stru *pst_user_tmp,
    hmac_user_stru *pst_hmac_user_tmp, oam_stat_info_stru *pst_oam_stat)
{
#define TID_STAT_TO_USER(_stat) ((_stat[0]) + (_stat[1]) + (_stat[2]) + (_stat[3]) + (_stat[4]) + \
                                     (_stat[5]) + (_stat[6]) + (_stat[7]))
#define BW_ENUM_TO_NUMBER(_bw) ((_bw) == 0 ? 20 : (_bw) == 1 ? 40 : 80)
    oal_uint8 *puc_addr;
    oam_user_stat_info_stru *pst_oam_user_stat;
    pst_oam_user_stat = &(pst_oam_stat->ast_user_stat_info[pst_user_tmp->us_assoc_id]);
    puc_addr = pst_user_tmp->auc_user_mac_addr;
    oal_int8 *pac_protocol2string[] = {
        "11a", "11b", "11g", "11g", "11g", "11n", "11ac", "11n", "11ac", "11n", "error"
    };

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
        "    Tx Throughput: %u\n" /* ������ */
#endif
,
        us_idx,
        pst_user_tmp->us_assoc_id,
        /* puc_addr��0��1��2��3��4��5byte��ʾMAC ADDR */
        puc_addr[0], puc_addr[1], puc_addr[2], puc_addr[3], puc_addr[4], puc_addr[5],
        pst_user_tmp->en_user_asoc_state, BW_ENUM_TO_NUMBER(pst_user_tmp->en_avail_bandwidth),
        (pst_user_tmp->uc_avail_num_spatial_stream + 1),
        pst_hmac_user_tmp->c_rssi, pac_protocol2string[pst_user_tmp->en_avail_protocol_mode],
        pst_hmac_user_tmp->ul_tx_rate, pst_hmac_user_tmp->ul_rx_rate,
        pst_hmac_user_tmp->ul_rx_rate_min, pst_hmac_user_tmp->ul_rx_rate_max,
        (oal_uint32)OAL_TIME_GET_RUNTIME(pst_hmac_user_tmp->ul_first_add_time, ul_curr_time) / 1000, /* 1s����1000ms */
        TID_STAT_TO_USER(pst_oam_user_stat->aul_tx_mpdu_succ_num) +
        TID_STAT_TO_USER(pst_oam_user_stat->aul_tx_mpdu_in_ampdu),
        TID_STAT_TO_USER(pst_oam_user_stat->aul_tx_mpdu_fail_num) +
        TID_STAT_TO_USER(pst_oam_user_stat->aul_tx_mpdu_fail_in_ampdu),
        pst_oam_user_stat->ul_rx_mpdu_num, 0, 20, /* TX power: 20dBm */
        TID_STAT_TO_USER(pst_oam_user_stat->aul_tx_mpdu_bytes) +
        TID_STAT_TO_USER(pst_oam_user_stat->aul_tx_ampdu_bytes),
        pst_oam_user_stat->ul_rx_mpdu_bytes, pst_oam_user_stat->ul_tx_ppdu_retries
#ifdef _PRE_WLAN_DFT_STAT
, pst_hmac_user_tmp->uc_cur_per, pst_hmac_user_tmp->uc_bestrate_per, 0
#endif
        );
#undef TID_STAT_TO_USER
#undef BW_ENUM_TO_NUMBER
}


OAL_STATIC oal_int32 wal_sta_info_seq_show(struct seq_file *f, void *v)
{
    mac_vap_stru *pst_mac_vap = (mac_vap_stru *)v;
    hmac_vap_stru *pst_hmac_vap;
    oal_dlist_head_stru *pst_entry;
    oal_dlist_head_stru *pst_dlist_tmp;
    mac_user_stru *pst_user_tmp;
    hmac_user_stru *pst_hmac_user_tmp;
    oal_uint16 us_idx = 1;
    oam_stat_info_stru *pst_oam_stat;
    oal_uint32 ul_curr_time;
    wal_msg_write_stru st_write_msg;
    oal_int32 l_ret;
    oal_uint32 *pul_param;
    mac_cfg_query_rssi_stru *pst_query_rssi_param;
    mac_cfg_query_rate_stru *pst_query_rate_param;

    pst_hmac_vap = mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "wal_sta_info_seq_show: hmac vap is null");
        return 0;
    }

    /* step1. ͬ��Ҫ��ѯ��dmac��Ϣ */
    OAL_DLIST_SEARCH_FOR_EACH_SAFE(pst_entry, pst_dlist_tmp, &(pst_mac_vap->st_mac_user_list_head))
    {
        pst_user_tmp = OAL_DLIST_GET_ENTRY(pst_entry, mac_user_stru, st_user_dlist);
        if (pst_user_tmp == OAL_PTR_NULL) {
            continue;
        }

        pst_hmac_user_tmp = mac_res_get_hmac_user(pst_user_tmp->us_assoc_id);
        if (pst_hmac_user_tmp == OAL_PTR_NULL) {
            continue;
        }

        if (pst_dlist_tmp == OAL_PTR_NULL) {
            /* ��forѭ���̻߳���ͣ���ڼ����ɾ���û��¼��������pst_dlist_tmpΪ�ա�Ϊ��ʱֱ��������ȡdmac��Ϣ */
            break;
        }

        /***********************************************************************/
        /* ��ȡdmac user��RSSI��Ϣ */
        /***********************************************************************/
        memset_s(&st_write_msg, OAL_SIZEOF(st_write_msg), 0, OAL_SIZEOF(st_write_msg));
        WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_QUERY_RSSI, OAL_SIZEOF(mac_cfg_query_rssi_stru));
        pst_query_rssi_param = (mac_cfg_query_rssi_stru *)st_write_msg.auc_value;

        pst_query_rssi_param->us_user_id = pst_user_tmp->us_assoc_id; /* ���û���id����ȥ */

        l_ret = wal_send_cfg_event(pst_hmac_vap->pst_net_device,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_query_rssi_stru),
                                   (oal_uint8 *)&st_write_msg, OAL_FALSE,
                                   OAL_PTR_NULL);
        if (l_ret != OAL_SUCC) {
            OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "wal_sta_info_seq_show:send event fail, ret:%d",
                             l_ret);
        }

        pst_hmac_vap->station_info_query_completed_flag = OAL_FALSE;
        /*lint -e730*/
        l_ret = OAL_WAIT_EVENT_INTERRUPTIBLE_TIMEOUT(pst_hmac_vap->query_wait_q,
                                                     (pst_hmac_vap->station_info_query_completed_flag == OAL_TRUE),
                                                     5 * OAL_TIME_HZ); /* 5 * HZ,HZһ��Ϊ1000�������ʾ��ʱʱ��Ϊ5000ms */
        /*lint +e730*/
        if (l_ret <= 0) {
            OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "wal_sta_info_seq_show: query rssi timeout.ret:%d",
                             l_ret);
        }

        /***********************************************************************/
        /* ��ȡdmac user��������Ϣ */
        /***********************************************************************/
        memset_s(&st_write_msg, OAL_SIZEOF(st_write_msg), 0, OAL_SIZEOF(st_write_msg));
        WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_QUERY_RATE, OAL_SIZEOF(mac_cfg_query_rate_stru));
        pst_query_rate_param = (mac_cfg_query_rate_stru *)st_write_msg.auc_value;

        pst_query_rate_param->us_user_id = pst_user_tmp->us_assoc_id; /* ���û���id����ȥ */

        l_ret = wal_send_cfg_event(pst_hmac_vap->pst_net_device,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_query_rate_stru),
                                   (oal_uint8 *)&st_write_msg,
                                   OAL_FALSE,
                                   OAL_PTR_NULL);
        if (l_ret != OAL_SUCC) {
            OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "wal_sta_info_seq_show:send event fail ret:%d", l_ret);
        }

        pst_hmac_vap->station_info_query_completed_flag = OAL_FALSE;
        /*lint -e730*/
        l_ret = OAL_WAIT_EVENT_INTERRUPTIBLE_TIMEOUT(pst_hmac_vap->query_wait_q,
                                                     (pst_hmac_vap->station_info_query_completed_flag == OAL_TRUE),
                                                     5 * OAL_TIME_HZ); /* 5 * HZ,HZһ��Ϊ1000�������ʾ��ʱʱ��Ϊ5000ms */
        /*lint +e730*/
        if (l_ret <= 0) {
            OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "wal_sta_info_seq_show:query rate timeout.ret:%d",
                             l_ret);
        }
    }

    /* step2. proc�ļ�����û���Ϣ */
    seq_printf(f, "Total user nums: %d\n", pst_mac_vap->us_user_nums);
    seq_printf(f, "-- STA info table --\n");

    pst_oam_stat = OAM_STAT_GET_STAT_ALL();
    ul_curr_time = (oal_uint32)OAL_TIME_GET_STAMP_MS();

    OAL_DLIST_SEARCH_FOR_EACH_SAFE(pst_entry, pst_dlist_tmp, &(pst_mac_vap->st_mac_user_list_head))
    {
        pst_user_tmp = OAL_DLIST_GET_ENTRY(pst_entry, mac_user_stru, st_user_dlist);
        if (pst_user_tmp == OAL_PTR_NULL) {
            continue;
        }

        pst_hmac_user_tmp = mac_res_get_hmac_user(pst_user_tmp->us_assoc_id);
        if (pst_hmac_user_tmp == OAL_PTR_NULL) {
            continue;
        }
        wal_sta_info_seq_print(us_idx, pst_user_tmp, pst_hmac_user_tmp, pst_oam_stat);
        us_idx++;

        if (pst_dlist_tmp == OAL_PTR_NULL) {
            break;
        }
    }
    return 0;
}


OAL_STATIC oal_void *wal_sta_info_seq_next(struct seq_file *f, void *v, loff_t *pos)
{
    return NULL;
}


OAL_STATIC oal_void wal_sta_info_seq_stop(struct seq_file *f, void *v)
{
}

/*****************************************************************************
    dmac_sta_info_seq_ops: ����seq_file ops
*****************************************************************************/
OAL_STATIC OAL_CONST struct seq_operations wal_sta_info_seq_ops = {
    .start = wal_sta_info_seq_start,
    .next = wal_sta_info_seq_next,
    .stop = wal_sta_info_seq_stop,
    .show = wal_sta_info_seq_show
};


OAL_STATIC oal_int32 wal_sta_info_seq_open(struct inode *inode, struct file *filp)
{
    oal_int32 l_ret;
    struct seq_file *pst_seq_file;
    struct proc_dir_entry *pde = PDE(inode);

    l_ret = seq_open(filp, &wal_sta_info_seq_ops);
    if (l_ret == OAL_SUCC) {
        pst_seq_file = (struct seq_file *)filp->private_data;

        pst_seq_file->private = pde->data;
    }

    return l_ret;
}

/*****************************************************************************
    gst_sta_info_proc_fops: ����sta info proc fops
*****************************************************************************/
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
    oal_int32 l_ret;
    hmac_vap_stru *pst_hmac_vap;

    pst_hmac_vap = mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "wal_read_vap_info_proc: hmac vap is null. vap id:%d", pst_mac_vap->uc_vap_id);
        return 0;
    }
    /***********************************************************************/
    /* ��ȡdmac vap��ANI��Ϣ */
    /***********************************************************************/
    memset_s(&st_write_msg, OAL_SIZEOF(st_write_msg), 0, OAL_SIZEOF(st_write_msg));
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_QUERY_RSSI, OAL_SIZEOF(mac_cfg_query_ani_stru));
    pst_query_ani_param = (mac_cfg_query_ani_stru *)st_write_msg.auc_value;

    l_ret = wal_send_cfg_event(pst_hmac_vap->pst_net_device, WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_query_ani_stru),
                               (oal_uint8 *)&st_write_msg, OAL_FALSE,
                               OAL_PTR_NULL);
    if (l_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "wal_read_vap_info_proc:send event fail, ret:%d", l_ret);
    }

    pst_hmac_vap->station_info_query_completed_flag = OAL_FALSE;
    /*lint -e730*/
    l_ret = OAL_WAIT_EVENT_INTERRUPTIBLE_TIMEOUT(pst_hmac_vap->query_wait_q,
                                                 (pst_hmac_vap->station_info_query_completed_flag == OAL_TRUE),
                                                 5 * OAL_TIME_HZ); /* 5 * HZ,HZһ��Ϊ1000�������ʾ��ʱʱ��Ϊ5000ms */
    /*lint +e730*/
    if (l_ret <= 0) { /* �ȴ���ʱ���쳣 */
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "wal_read_vap_info_proc:query ani timeout.ret:%d", l_ret);
    }
#endif

    pst_oam_stat = OAM_STAT_GET_STAT_ALL();

    pst_oam_vap_stat = &(pst_oam_stat->ast_vap_stat_info[pst_mac_vap->uc_vap_id]);

    len = snprintf_s(page, PAGE_SIZE, PAGE_SIZE - 1, "vap stats:\n"
        "  TX bytes: %u\n"
        "  TX packets: %u\n"
        "  TX packets error: %u\n"
        "  TX packets discard: %u\n"
        "  TX unicast packets: %u\n"
        "  TX multicast packets: %u\n"
        "  TX broadcast packets: %u\n",
        pst_oam_vap_stat->ul_tx_bytes_from_lan, pst_oam_vap_stat->ul_tx_pkt_num_from_lan,
        pst_oam_vap_stat->ul_tx_abnormal_msdu_dropped +
        pst_oam_vap_stat->ul_tx_security_check_faild +
        pst_oam_vap_stat->ul_tx_abnormal_mpdu_dropped,
        pst_oam_vap_stat->ul_tx_uapsd_process_dropped +
        pst_oam_vap_stat->ul_tx_psm_process_dropped +
        pst_oam_vap_stat->ul_tx_alg_process_dropped,
        pst_oam_vap_stat->ul_tx_pkt_num_from_lan - pst_oam_vap_stat->ul_tx_m2u_mcast_cnt, 0,
        pst_oam_vap_stat->ul_tx_m2u_mcast_cnt);

    len += snprintf_s(page + len, PAGE_SIZE - len, PAGE_SIZE - len - 1, "  RX bytes: %u\n"
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
        pst_oam_vap_stat->ul_rx_bytes_to_lan, pst_oam_vap_stat->ul_rx_pkt_to_lan,
        pst_oam_vap_stat->ul_rx_defrag_process_dropped +
        pst_oam_vap_stat->ul_rx_alg_process_dropped +
        pst_oam_vap_stat->ul_rx_abnormal_dropped,
        pst_oam_vap_stat->ul_rx_no_buff_dropped +
        pst_oam_vap_stat->ul_rx_ta_check_dropped +
        pst_oam_vap_stat->ul_rx_da_check_dropped +
        pst_oam_vap_stat->ul_rx_replay_fail_dropped +
        pst_oam_vap_stat->ul_rx_key_search_fail_dropped,
        pst_oam_vap_stat->ul_rx_pkt_to_lan - pst_oam_vap_stat->ul_rx_mcast_cnt,
        0, pst_oam_vap_stat->ul_rx_mcast_cnt, 0
#ifdef _PRE_WLAN_DFT_STAT
,
        pst_mac_vap->st_curr_sup_rates.uc_br_rate_num, pst_mac_vap->st_curr_sup_rates.uc_nbr_rate_num,
        pst_mac_vap->st_curr_sup_rates.uc_max_rate, pst_mac_vap->st_curr_sup_rates.uc_min_rate,
        pst_mac_vap->st_channel.uc_chan_number, pst_hmac_vap->uc_device_distance,
        pst_hmac_vap->uc_intf_state_cca, pst_hmac_vap->uc_intf_state_co
#endif
);

    return len;
}


OAL_STATIC int wal_read_rf_info_proc(char *page, char **start, off_t off,
                                     int count, int *eof, void *data)
{
    int len;
    mac_vap_stru *pst_mac_vap = (mac_vap_stru *)data;

    len = snprintf_s(page, PAGE_SIZE, PAGE_SIZE - 1, "rf info:\n  channel_num: %d\n",
        pst_mac_vap->st_channel.uc_chan_number);

    return len;
}
#endif


OAL_STATIC oal_void wal_add_vap_proc_file(mac_vap_stru *pst_mac_vap, oal_int8 *pc_name)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION) && (LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 34))
    hmac_vap_stru *pst_hmac_vap;
    oal_proc_dir_entry_stru *pst_proc_dir;
    oal_proc_dir_entry_stru *pst_proc_vapinfo;
    oal_proc_dir_entry_stru *pst_proc_stainfo;
    oal_proc_dir_entry_stru *pst_proc_mibrf;

    pst_hmac_vap = mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "wal_add_vap_proc_file: pst_hmac_vap is null ptr!");
        return;
    }

    pst_proc_dir = proc_mkdir(pc_name, NULL);
    if (pst_proc_dir == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "wal_add_vap_proc_file: proc_mkdir return null");
        return;
    }
    /* 420ʮ���ƶ�Ӧ�˽�����0644 linuxģʽ���� S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH); */
    pst_proc_vapinfo = oal_create_proc_entry("ap_info", 420, pst_proc_dir);
    if (pst_proc_vapinfo == OAL_PTR_NULL) {
        oal_remove_proc_entry(pc_name, NULL);
        return;
    }
    /* 420ʮ���ƶ�Ӧ�˽�����0644 linuxģʽ���� S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH); */
    pst_proc_stainfo = oal_create_proc_entry("sta_info", 420, pst_proc_dir);
    if (pst_proc_stainfo == OAL_PTR_NULL) {
        oal_remove_proc_entry("ap_info", pst_proc_dir);
        oal_remove_proc_entry(pc_name, NULL);
        return;
    }
    /* 420ʮ���ƶ�Ӧ�˽�����0644 linuxģʽ���� S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH); */
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

    /* sta info�������ļ��Ƚϴ��proc file��ͨ��proc_fops�ķ�ʽ��� */
    pst_proc_stainfo->data = pst_mac_vap;
    pst_proc_stainfo->proc_fops = &gst_sta_info_proc_fops;

    /* rf info */
    pst_proc_mibrf->read_proc = wal_read_rf_info_proc;
    pst_proc_mibrf->data = pst_mac_vap;

    pst_hmac_vap->pst_proc_dir = pst_proc_dir;
#endif
}
oal_uint32 wal_vap_mode_check(oal_int8 ac_mode[], oal_uint32 ul_mode_len,
#ifdef _PRE_WLAN_FEATURE_P2P
    wlan_p2p_mode_enum_uint8 *pen_p2p_mode,
#endif
    wlan_vap_mode_enum_uint8 *pen_mode)
{
    /* ����ac_mode�ַ�����Ӧ��ģʽ */
    if ((strcmp("ap", ac_mode)) == 0) {
        *pen_mode = WLAN_VAP_MODE_BSS_AP;
    } else if ((strcmp("sta", ac_mode)) == 0) {
        *pen_mode = WLAN_VAP_MODE_BSS_STA;
    }
#ifdef _PRE_WLAN_FEATURE_P2P
    /* ����P2P ���VAP */
    else if ((strcmp("p2p_device", ac_mode)) == 0) {
        *pen_mode = WLAN_VAP_MODE_BSS_STA;
        *pen_p2p_mode = WLAN_P2P_DEV_MODE;
    } else if ((strcmp("p2p_cl", ac_mode)) == 0) {
        *pen_mode = WLAN_VAP_MODE_BSS_STA;
        *pen_p2p_mode = WLAN_P2P_CL_MODE;
    } else if ((strcmp("p2p_go", ac_mode)) == 0) {
        *pen_mode = WLAN_VAP_MODE_BSS_AP;
        *pen_p2p_mode = WLAN_P2P_GO_MODE;
    }
#endif /* _PRE_WLAN_FEATURE_P2P */
    else {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_vap_mode_check::the mode param is invalid!}\r\n");
        return OAL_ERR_CODE_INVALID_CONFIG;
    }
    return OAL_SUCC;
}

oal_void wal_netdev_and_wdev_init(oal_net_device_stru *pst_net_dev, oal_wireless_dev_stru *pst_wdev,
#ifdef _PRE_WLAN_FEATURE_P2P
    wlan_p2p_mode_enum_uint8 en_p2p_mode,
#endif
    mac_device_stru *pst_mac_device, wlan_vap_mode_enum_uint8 en_mode)
{
    memset_s(pst_wdev, OAL_SIZEOF(oal_wireless_dev_stru), 0, OAL_SIZEOF(oal_wireless_dev_stru));

    /* ��netdevice���и�ֵ */
#ifdef CONFIG_WIRELESS_EXT
    pst_net_dev->wireless_handlers = &g_st_iw_handler_def;
#endif
    pst_net_dev->netdev_ops = &g_st_wal_net_dev_ops;

    OAL_NETDEVICE_DESTRUCTOR(pst_net_dev) = oal_net_free_netdev;

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 10, 44))
    OAL_NETDEVICE_MASTER(pst_net_dev) = OAL_PTR_NULL;
#endif

    OAL_NETDEVICE_IFALIAS(pst_net_dev) = OAL_PTR_NULL;
    OAL_NETDEVICE_WATCHDOG_TIMEO(pst_net_dev) = 5;
    OAL_NETDEVICE_WDEV(pst_net_dev) = pst_wdev;
    OAL_NETDEVICE_QDISC(pst_net_dev, OAL_PTR_NULL);

    pst_wdev->netdev = pst_net_dev;

    if (en_mode == WLAN_VAP_MODE_BSS_AP) {
        pst_wdev->iftype = NL80211_IFTYPE_AP;
    } else if (en_mode == WLAN_VAP_MODE_BSS_STA) {
        pst_wdev->iftype = NL80211_IFTYPE_STATION;
    }
#ifdef _PRE_WLAN_FEATURE_P2P
    if (en_p2p_mode == WLAN_P2P_DEV_MODE) {
        pst_wdev->iftype = NL80211_IFTYPE_P2P_DEVICE;
    } else if (en_p2p_mode == WLAN_P2P_CL_MODE) {
        pst_wdev->iftype = NL80211_IFTYPE_P2P_CLIENT;
    } else if (en_p2p_mode == WLAN_P2P_GO_MODE) {
        pst_wdev->iftype = NL80211_IFTYPE_P2P_GO;
    }
#endif /* _PRE_WLAN_FEATURE_P2P */

    pst_wdev->wiphy = pst_mac_device->pst_wiphy;

    OAL_NETDEVICE_FLAGS(pst_net_dev) &= ~OAL_IFF_RUNNING; /* ��net device��flag��Ϊdown */
}

oal_void wal_write_msg_init(wal_msg_write_stru *pst_write_msg, oal_net_device_stru *pst_net_dev,
#ifdef _PRE_WLAN_FEATURE_P2P
    wlan_p2p_mode_enum_uint8 en_p2p_mode,
#endif
    wlan_vap_mode_enum_uint8 en_mode, mac_vap_stru *pst_cfg_mac_vap)
{
   /*
       st_write_msg�������������ֹ���ֳ�Ա��Ϊ��غ�û�п���û�и�ֵ�����ַ�0���쳣ֵ
       ����ṹ����P2P modeû����P2P�����������������Ϊ������û����ȷ��ֵ
    */
    memset_s(pst_write_msg, OAL_SIZEOF(wal_msg_write_stru), 0, OAL_SIZEOF(wal_msg_write_stru));

    /***************************************************************************
        ���¼���wal�㴦��
    ***************************************************************************/
    /* ��д��Ϣ */
    WAL_WRITE_MSG_HDR_INIT(pst_write_msg, WLAN_CFGID_ADD_VAP, OAL_SIZEOF(mac_cfg_add_vap_param_stru));
    ((mac_cfg_add_vap_param_stru *)pst_write_msg->auc_value)->pst_net_dev = pst_net_dev;
    ((mac_cfg_add_vap_param_stru *)pst_write_msg->auc_value)->en_vap_mode = en_mode;
    ((mac_cfg_add_vap_param_stru *)pst_write_msg->auc_value)->uc_cfg_vap_indx = pst_cfg_mac_vap->uc_vap_id;
#ifdef _PRE_WLAN_FEATURE_P2P
    ((mac_cfg_add_vap_param_stru *)pst_write_msg->auc_value)->en_p2p_mode = en_p2p_mode;
#endif
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
    ((mac_cfg_add_vap_param_stru *)pst_write_msg->auc_value)->bit_11ac2g_enable =
        (oal_uint8) !!hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_11AC2G_ENABLE);
    ((mac_cfg_add_vap_param_stru *)pst_write_msg->auc_value)->bit_disable_capab_2ght40 =
        wlan_customize.uc_disable_capab_2ght40;
#endif
}

oal_uint32 wal_hipriv_add_vap(oal_net_device_stru *pst_cfg_net_dev, oal_int8 *pc_param)
{
    oal_net_device_stru *pst_net_dev = OAL_PTR_NULL;
    wal_msg_write_stru st_write_msg;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;
    oal_uint32 ul_err_code;
    oal_uint32 ul_ret;
    oal_int32 l_ret;
    oal_uint32 ul_off_set;
    oal_int8 ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    oal_int8 ac_mode[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    wlan_vap_mode_enum_uint8 en_mode;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;

    oal_wireless_dev_stru *pst_wdev = OAL_PTR_NULL;

    mac_vap_stru *pst_cfg_mac_vap = OAL_PTR_NULL;
    mac_device_stru *pst_mac_device = OAL_PTR_NULL;
#ifdef _PRE_WLAN_FEATURE_P2P
    wlan_p2p_mode_enum_uint8 en_p2p_mode = WLAN_LEGACY_VAP_MODE;
#endif

    /* pc_paramָ���´�����net_device��name, ����ȡ����ŵ�ac_name�� */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, OAL_SIZEOF(ac_name), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_hipriv_add_vap::wal_get_cmd_one_arg vap name return err_code %d", ul_ret);
        return ul_ret;
    }

    /* ac_name length��Ӧ����OAL_IF_NAME_SIZE */
    if (OAL_STRLEN(ac_name) >= OAL_IF_NAME_SIZE) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_add_vap:: vap name overlength is %d!}\r\n", OAL_STRLEN(ac_name));
        /* ��������vap name��Ϣ��groupsize��С��32 */
        oal_print_hex_dump((oal_uint8 *)ac_name, OAL_IF_NAME_SIZE, 32, "vap name lengh is overlong:");
        return OAL_FAIL;
    }

    pc_param += ul_off_set;

    /* pc_param ָ��'ap|sta', ����ȡ���ŵ�ac_mode�� */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_mode, OAL_SIZEOF(ac_mode), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_hipriv_add_vap:wal_get_cmd_one_arg vap name return err_code %d", ul_ret);
        return (oal_uint32)ul_ret;
    }

    /* ����ac_mode�ַ�����Ӧ��ģʽ */
    ul_ret = wal_vap_mode_check(ac_mode, OAL_SIZEOF(ac_mode),
#ifdef _PRE_WLAN_FEATURE_P2P
        &en_p2p_mode,
#endif
        &en_mode);
    if (ul_ret != OAL_SUCC) {
        return ul_ret;
    }
    /* ���������net device�Ѿ����ڣ�ֱ�ӷ��� */
    /* ����dev_name�ҵ�dev */
    pst_net_dev = oal_dev_get_by_name(ac_name);
    if (pst_net_dev != OAL_PTR_NULL) {
        /* ����oal_dev_get_by_name�󣬱������oal_dev_putʹnet_dev�����ü�����һ */
        oal_dev_put(pst_net_dev);

        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_hipriv_add_vap::the net_device is already exist!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ��ȡmac device */
    pst_cfg_mac_vap = OAL_NET_DEV_PRIV(pst_cfg_net_dev);
    pst_mac_device = mac_res_get_dev(pst_cfg_mac_vap->uc_device_id);
    if (OAL_UNLIKELY(pst_mac_device == OAL_PTR_NULL)) {
        OAM_WARNING_LOG0(pst_cfg_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_hipriv_add_vap::pst_mac_device is null ptr!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

#if defined(_PRE_WLAN_FEATURE_OFFLOAD_FLOWCTL)
    pst_net_dev = oal_net_alloc_netdev_mqs(OAL_SIZEOF(oal_netdev_priv_stru), ac_name, oal_ether_setup,
                                           WLAN_NET_QUEUE_BUTT, 1);
#else
    pst_net_dev = oal_net_alloc_netdev(OAL_SIZEOF(oal_netdev_priv_stru), ac_name, oal_ether_setup);
#endif
    if (OAL_UNLIKELY(pst_net_dev == OAL_PTR_NULL)) {
        OAM_WARNING_LOG0(pst_cfg_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_hipriv_add_vap::pst_net_dev null ptr error!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_wdev =
        (oal_wireless_dev_stru *)OAL_MEM_ALLOC(OAL_MEM_POOL_ID_LOCAL, OAL_SIZEOF(oal_wireless_dev_stru), OAL_FALSE);
    if (OAL_UNLIKELY(pst_wdev == OAL_PTR_NULL)) {
        OAM_ERROR_LOG0(pst_cfg_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_hipriv_add_vap:alloc mem, pst_wdev is null ptr}");
        oal_net_free_netdev(pst_net_dev);
        return OAL_ERR_CODE_PTR_NULL;
    }

    wal_netdev_and_wdev_init(pst_net_dev, pst_wdev,
#ifdef _PRE_WLAN_FEATURE_P2P
        en_p2p_mode,
#endif
        pst_mac_device, en_mode);
    wal_write_msg_init(&st_write_msg, pst_net_dev,
#ifdef _PRE_WLAN_FEATURE_P2P
        en_p2p_mode,
#endif
        en_mode, pst_cfg_mac_vap);

    /* ������Ϣ */
    l_ret = wal_send_cfg_event(pst_cfg_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_add_vap_param_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_TRUE,
                               &pst_rsp_msg);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAL_MEM_FREE(pst_wdev, OAL_FALSE);
        oal_net_free_netdev(pst_net_dev);
        OAM_WARNING_LOG1(pst_cfg_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_hipriv_add_vap::return err code %d!}", l_ret);
        return (oal_uint32)l_ret;
    }

    /* ��ȡ���صĴ����� */
    ul_err_code = wal_check_and_release_msg_resp(pst_rsp_msg);
    if (ul_err_code != OAL_SUCC) {
        OAM_WARNING_LOG1(pst_cfg_mac_vap->uc_vap_id, OAM_SF_ANY, "wal_hipriv_add_vap::hmac add vap fail, err code[%u]",
                         ul_err_code);
        /* �쳣�����ͷ��ڴ� */
        OAL_MEM_FREE(pst_wdev, OAL_FALSE);
        oal_net_free_netdev(pst_net_dev);
        return ul_err_code;
    }

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)

    if ((en_p2p_mode == WLAN_LEGACY_VAP_MODE) && (pst_mac_device->st_p2p_info.pst_primary_net_device == OAL_PTR_NULL)) {
        /* �������wlan0�� �򱣴�wlan0 Ϊ��net_device,p2p0 ��p2p-p2p0 MAC ��ַ����netdevice ��ȡ */
        pst_mac_device->st_p2p_info.pst_primary_net_device = pst_net_dev;
    }

    if (wal_set_random_mac_to_mib(pst_net_dev) != OAL_SUCC) {
        /* �쳣�����ͷ��ڴ� */
        /* �쳣�����ͷ��ڴ� */
        OAL_MEM_FREE(pst_wdev, OAL_FALSE);
        oal_net_free_netdev(pst_net_dev);
        return OAL_ERR_CODE_PTR_NULL;
    } /* set random mac to mib ; for hi1102-cb */
#endif

    /* ����netdevice��MAC��ַ��MAC��ַ��HMAC�㱻��ʼ����MIB�� */
    pst_mac_vap = OAL_NET_DEV_PRIV(pst_net_dev);
#ifdef _PRE_WLAN_FEATURE_P2P
    if (en_p2p_mode == WLAN_P2P_DEV_MODE) {
        oal_set_mac_addr((oal_uint8 *)OAL_NETDEVICE_MAC_ADDR(pst_net_dev),
                         pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.auc_p2p0_dot11StationID);

        pst_mac_device->st_p2p_info.uc_p2p0_vap_idx = pst_mac_vap->uc_vap_id;
    } else
#endif
    {
        oal_set_mac_addr((oal_uint8 *)OAL_NETDEVICE_MAC_ADDR(pst_net_dev),
                         pst_mac_vap->pst_mib_info->st_wlan_mib_sta_config.auc_dot11StationID);
    }

    /* ע��net_device */
    ul_ret = (oal_uint32)oal_net_register_netdev(pst_net_dev);
    if (OAL_UNLIKELY(ul_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "wal_hipriv_add_vap:oal_net_register_netdev fai, ret= %d",
                         ul_ret);

        /* �쳣�����ͷ��ڴ� */
        /* ��ɾ��vap�¼��ͷŸ������vap */
        WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_DEL_VAP, OAL_SIZEOF(mac_cfg_del_vap_param_stru));

        l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_del_vap_param_stru),
                                   (oal_uint8 *)&st_write_msg,
                                   OAL_TRUE,
                                   &pst_rsp_msg);

        if (wal_check_and_release_msg_resp(pst_rsp_msg) != OAL_SUCC) {
            OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_hipriv_add_vap::wal_check_and_release_msg_resp fail.}");
        }
        if (l_ret != OAL_SUCC) {
            OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_add_vap::wal_send_cfg_event fail, err code %d!}\r\n", l_ret);
        }

        OAL_MEM_FREE(pst_wdev, OAL_FALSE);
        oal_net_free_netdev(pst_net_dev);
        return ul_ret;
    }

    /* ����VAP��Ӧ��proc�ļ� */
    wal_add_vap_proc_file(pst_mac_vap, ac_name);

    return OAL_SUCC;
}


OAL_STATIC oal_void wal_del_vap_proc_file(oal_net_device_stru *pst_net_dev)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION) && (LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 34))
    mac_vap_stru *pst_mac_vap;
    hmac_vap_stru *pst_hmac_vap;

    pst_mac_vap = OAL_NET_DEV_PRIV(pst_net_dev);
    if (OAL_UNLIKELY(pst_mac_vap == OAL_PTR_NULL)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_del_vap_proc_file: pst_mac_vap is null ptr! pst_net_dev:%x",
                         (oal_uint32)pst_net_dev);
        return;
    }

    pst_hmac_vap = mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "wal_del_vap_proc_file: pst_hmac_vap is null ptr. mac vap id:%d",
                       pst_mac_vap->uc_vap_id);
        return;
    }

    if (pst_hmac_vap->pst_proc_dir) {
        oal_remove_proc_entry("mib_rf", pst_hmac_vap->pst_proc_dir);
        oal_remove_proc_entry("sta_info", pst_hmac_vap->pst_proc_dir);
        oal_remove_proc_entry("ap_info", pst_hmac_vap->pst_proc_dir);
        oal_remove_proc_entry(pst_hmac_vap->auc_name, NULL);
        pst_hmac_vap->pst_proc_dir = OAL_PTR_NULL;
    }
#endif
}


oal_uint32 wal_hipriv_del_vap(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    wal_msg_write_stru st_write_msg;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;
    oal_int32 l_ret;
#ifdef _PRE_WLAN_FEATURE_P2P
    oal_wireless_dev_stru *pst_wdev;
    wlan_p2p_mode_enum_uint8 en_p2p_mode;
#endif

    if (OAL_UNLIKELY((pst_net_dev == OAL_PTR_NULL) || (pc_param == OAL_PTR_NULL))) {
        OAM_ERROR_LOG2(0, OAM_SF_ANY, "{wal_hipriv_del_vap::pst_net_dev or pc_param null ptr error %x, %x!}\r\n",
                       (uintptr_t)pst_net_dev, (uintptr_t)pc_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* �豸��up״̬������ɾ����������down */
    if (OAL_UNLIKELY((OAL_IFF_RUNNING & OAL_NETDEVICE_FLAGS(pst_net_dev)) != 0)) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "{wal_hipriv_del_vap::device is busy, please down it first %d!}\r\n",
                       OAL_NETDEVICE_FLAGS(pst_net_dev));
        return OAL_ERR_CODE_CONFIG_BUSY;
    }

    /* ɾ��vap��Ӧ��proc�ļ� */
    wal_del_vap_proc_file(pst_net_dev);

    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    // ɾ��vap ʱ��Ҫ��������ֵ��
    ((mac_cfg_del_vap_param_stru *)st_write_msg.auc_value)->pst_net_dev = pst_net_dev;
#ifdef _PRE_WLAN_FEATURE_P2P
    pst_wdev = pst_net_dev->ieee80211_ptr;
    en_p2p_mode = wal_wireless_iftype_to_mac_p2p_mode(pst_wdev->iftype);
    if (en_p2p_mode == WLAN_P2P_BUTT) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_hipriv_del_vap::wal_wireless_iftype_to_mac_p2p_mode return BUFF}\r\n");
        return OAL_ERR_CODE_INVALID_CONFIG;
    }
    ((mac_cfg_del_vap_param_stru *)st_write_msg.auc_value)->en_p2p_mode = en_p2p_mode;

#endif

    OAL_MEM_FREE(OAL_NETDEVICE_WDEV(pst_net_dev), OAL_TRUE);

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_DEL_VAP, OAL_SIZEOF(mac_cfg_del_vap_param_stru));

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_del_vap_param_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_TRUE,
                               &pst_rsp_msg);

    if (wal_check_and_release_msg_resp(pst_rsp_msg) != OAL_SUCC) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_hipriv_del_vap::wal_check_and_release_msg_resp fail}");
    }

    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_del_vap::return err code %d}\r\n", l_ret);
        /* ȥע�� */
        oal_net_unregister_netdev(pst_net_dev);
        return (oal_uint32)l_ret;
    }

    /* ȥע�� */
    oal_net_unregister_netdev(pst_net_dev);

    return OAL_SUCC;
}


oal_uint32 wal_hipriv_vap_info(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    wal_msg_write_stru st_write_msg;
    oal_int32 l_ret;

    /***************************************************************************
        ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_VAP_INFO, OAL_SIZEOF(oal_int32));

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(oal_int32),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_vap_info::return err code %d!}\r\n", l_ret);
        return (oal_uint32)l_ret;
    }

    return OAL_SUCC;
}
#ifdef _PRE_WLAN_FEATURE_IP_FILTER
#define MAX_HIPRIV_IP_FILTER_BTABLE_SIZE 129

oal_uint32 wal_hipriv_set_ip_filter(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    oal_int32 l_items_cnt;
    oal_int32 l_items_idx;
    oal_int32 l_enable;
    oal_uint32 ul_ret;
    oal_uint32 ul_off_set;

    oal_int8 ac_cmd[WAL_HIPRIV_CMD_NAME_MAX_LEN] = { 0 };
    oal_int8 ac_cmd_param[WAL_HIPRIV_CMD_NAME_MAX_LEN] = { 0 };
    wal_hw_wifi_filter_item ast_items[MAX_HIPRIV_IP_FILTER_BTABLE_SIZE];

    l_enable = 0;
    l_items_cnt = 0;
    memset_s((oal_uint8 *)ast_items, OAL_SIZEOF(wal_hw_wifi_filter_item) * MAX_HIPRIV_IP_FILTER_BTABLE_SIZE,
             0, OAL_SIZEOF(wal_hw_wifi_filter_item) * MAX_HIPRIV_IP_FILTER_BTABLE_SIZE);

    /* ��ȡ�������� */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_cmd, OAL_SIZEOF(ac_cmd), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_hipriv_set_ip_filter:wal_get_cmd_one_arg fail, err_code %d", ul_ret);
        return ul_ret;
    }

    if (oal_strncmp(ac_cmd, CMD_CLEAR_RX_FILTERS, OAL_STRLEN(CMD_CLEAR_RX_FILTERS)) == 0) {
        /* ����� */
        ul_ret = (oal_uint32)wal_clear_ip_filter();
        return ul_ret;
    }

    pc_param += ul_off_set;
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_cmd_param, OAL_SIZEOF(ac_cmd_param), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_set_ip_filter::get cmd_param return err_code %d!}", ul_ret);
        return ul_ret;
    }

    if (oal_strncmp(ac_cmd, CMD_SET_RX_FILTER_ENABLE, OAL_STRLEN(CMD_SET_RX_FILTER_ENABLE)) == 0) {
        /* ʹ��/�رչ��� */
        l_enable = oal_atoi(ac_cmd_param);
        ul_ret = (oal_uint32)wal_set_ip_filter_enable(l_enable);
        return ul_ret;
    } else if (oal_strncmp(ac_cmd, CMD_ADD_RX_FILTER_ITEMS, OAL_STRLEN(CMD_ADD_RX_FILTER_ITEMS)) == 0) {
        /* ���º����� */
        /* ��ȡ������Ŀ�� */
        l_items_cnt = oal_atoi(ac_cmd_param);
        l_items_cnt = OAL_MIN(MAX_HIPRIV_IP_FILTER_BTABLE_SIZE, l_items_cnt);

        /* ��ȡ������Ŀ */
        for (l_items_idx = 0; l_items_idx < l_items_cnt; l_items_idx++) {
            /* ��ȡprotocolX */
            pc_param += ul_off_set;
            ul_ret = wal_get_cmd_one_arg(pc_param, ac_cmd_param, OAL_SIZEOF(ac_cmd_param), &ul_off_set);
            if (ul_ret != OAL_SUCC) {
                OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_hipriv_set_ip_filter:get item_params return err_code %d", ul_ret);
                return ul_ret;
            }
            ast_items[l_items_idx].protocol = (oal_uint8)oal_atoi(ac_cmd_param);

            /* ��ȡportX */
            pc_param += ul_off_set;
            ul_ret = wal_get_cmd_one_arg(pc_param, ac_cmd_param, OAL_SIZEOF(ac_cmd_param), &ul_off_set);
            if (ul_ret != OAL_SUCC) {
                OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_hipriv_set_ip_filter:get item_params return err_code %d", ul_ret);
                return ul_ret;
            }
            ast_items[l_items_idx].port = (oal_uint16)oal_atoi(ac_cmd_param);
        }

        ul_ret = (oal_uint32)wal_add_ip_filter_items(ast_items, l_items_cnt);
        return ul_ret;
    } else {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_hipriv_set_ip_filter::cmd_one_arg no support!}");
        return OAL_FAIL;
    }
}

#endif  // _PRE_WLAN_FEATURE_IP_FILTER


OAL_STATIC oal_uint32 wal_hipriv_set_2040_coext_support(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    wal_msg_write_stru st_write_msg;
    oal_uint32 ul_off_set;
    oal_int8 ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    oal_uint32 ul_ret;
    oal_int32 l_ret;
    oal_uint8 uc_csp;

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, OAL_SIZEOF(ac_name), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_hipriv_set_2040_coext_support::wal_get_cmd_one_arg return err_code %d!",
                         ul_ret);
        return ul_ret;
    }

    if ((strcmp("0", ac_name)) == 0) {
        uc_csp = 0;
    } else if ((strcmp("1", ac_name)) == 0) {
        uc_csp = 1;
    } else {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_set_2040_coext_support:the 2040_coexistence command is erro %x}",
                         (uintptr_t)ac_name);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    /***************************************************************************
                             ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_2040_COEXISTENCE, OAL_SIZEOF(oal_int32));
    *((oal_int32 *)(st_write_msg.auc_value)) = uc_csp; /* ��������������� */

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(oal_int32),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_set_2040_coext_support::return err code %d!}\r\n", l_ret);
        return (oal_uint32)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC oal_uint32 wal_hipriv_rx_fcs_info(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    wal_msg_write_stru st_write_msg;
    oal_uint32 ul_off_set;
    oal_int8 ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    oal_int32 l_ret;
    oal_uint32 ul_ret;
    mac_cfg_rx_fcs_info_stru *pst_rx_fcs_info = OAL_PTR_NULL;
    mac_cfg_rx_fcs_info_stru st_rx_fcs_info; /* ��ʱ�����ȡ��use����Ϣ */

    /* ��ӡ����֡��FCS��ȷ�������Ϣ:sh hipriv.sh "vap0 rx_fcs_info 0/1 1-4" 0/1  0���������1������� */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, OAL_SIZEOF(ac_name), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_rx_fcs_info::wal_get_cmd_one_arg return err_code %d!}", ul_ret);
        return ul_ret;
    }

    st_rx_fcs_info.ul_data_op = (oal_uint32)oal_atoi(ac_name);

    if (st_rx_fcs_info.ul_data_op > 1) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_rx_fcs_info::the ul_data_op command is error %x!}",
                         (uintptr_t)ac_name);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    /* ƫ�ƣ�ȡ��һ������ */
    pc_param = pc_param + ul_off_set;

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, OAL_SIZEOF(ac_name), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_rx_fcs_info::wal_get_cmd_one_arg return err_code %d!}", ul_ret);
        return ul_ret;
    }

    st_rx_fcs_info.ul_print_info = (oal_uint32)oal_atoi(ac_name);
    /* ��ӡ��������:<0>�������� <1>��֡�� <2>self fcs correct, <3>other fcs correct, <4>fcs error */
    if (st_rx_fcs_info.ul_print_info > 4) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_rx_fcs_info::the ul_print_info command is error %x!}",
                         (uintptr_t)ac_name);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_RX_FCS_INFO, OAL_SIZEOF(mac_cfg_rx_fcs_info_stru));

    /* ��������������� */
    pst_rx_fcs_info = (mac_cfg_rx_fcs_info_stru *)(st_write_msg.auc_value);
    pst_rx_fcs_info->ul_data_op = st_rx_fcs_info.ul_data_op;
    pst_rx_fcs_info->ul_print_info = st_rx_fcs_info.ul_print_info;

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_rx_fcs_info_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_rx_fcs_info::return err code %d!}\r\n", l_ret);
        return (oal_uint32)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC oal_uint32 wal_hipriv_vap_log_level(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    mac_vap_stru *pst_mac_vap;
    oam_log_level_enum_uint8 en_level_val;
    oal_uint32 ul_off_set;
    oal_int8 ac_param[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    oal_uint32 ul_ret;

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    wal_msg_write_stru st_write_msg;
#endif

    /* OAM logģ��Ŀ��ص�����: hipriv "Hisilicon0[vapx] log_level {1/2}"
       1-2(error��warning)������־��vap����Ϊά�ȣ�
 */
    pst_mac_vap = OAL_NET_DEV_PRIV(pst_net_dev);
    if (OAL_UNLIKELY(pst_mac_vap == OAL_PTR_NULL)) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_hipriv_vap_log_level::null pointer.}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ��ȡ��־���� */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_param, OAL_SIZEOF(ac_param), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        return ul_ret;
    }

    en_level_val = (oam_log_level_enum_uint8)oal_atoi(ac_param);
    if ((en_level_val < OAM_LOG_LEVEL_ERROR) || (en_level_val > OAM_LOG_LEVEL_INFO)) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "{wal_hipriv_feature_log_level::invalid switch value[%d].}", en_level_val);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    ul_ret = oam_log_set_vap_level(pst_mac_vap->uc_vap_id, en_level_val);

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    // Ŀǰ֧��02 device ����log ���� ���������ĺϲ�����
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_LOG_LEVEL, OAL_SIZEOF(oal_int32));
    *((oal_int32 *)(st_write_msg.auc_value)) = en_level_val;
    ul_ret |= (oal_uint32)wal_send_cfg_event(pst_net_dev,
                                             WAL_MSG_TYPE_WRITE,
                                             WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(oal_int32),
                                             (oal_uint8 *)&st_write_msg,
                                             OAL_FALSE,
                                             OAL_PTR_NULL);
    if (OAL_UNLIKELY(ul_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_vap_log_level::return err code[%d]!}\r\n", ul_ret);
        return ul_ret;
    }

#endif
    return ul_ret;
}
#ifdef _PRE_WLAN_FEATURE_BTCOEX

OAL_STATIC oal_uint32 wal_hipriv_btcoex_status_print(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    wal_msg_write_stru st_write_msg;
    oal_int32 l_ret;

    memset_s((oal_uint8 *)&st_write_msg, OAL_SIZEOF(st_write_msg), 0, OAL_SIZEOF(st_write_msg));

    /* sh hipriv.sh "vap_name coex_print" */
    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_BTCOEX_STATUS_PRINT, OAL_SIZEOF(oal_uint32));

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(oal_uint32),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_TXOP, "{wal_hipriv_btcoex_status_print::return err code[%d]!}\r\n", l_ret);
        return (oal_uint32)l_ret;
    }

    return OAL_SUCC;
}
#endif

#ifdef _PRE_WLAN_FEATURE_LTECOEX

OAL_STATIC oal_uint32 wal_ioctl_ltecoex_mode_set(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    wal_msg_write_stru st_write_msg;
    oal_int32 l_ret;

    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_LTECOEX_MODE_SET, OAL_SIZEOF(oal_uint32));

    /* ��������������� */
    *((oal_uint8 *)(st_write_msg.auc_value)) = *pc_param;

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(oal_uint8),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_ioctl_ltecoex_mode_set::return err code[%d]!}\r\n", l_ret);
        return (oal_uint32)l_ret;
    }

    return OAL_SUCC;
}
#endif

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)

OAL_STATIC oal_uint32 wal_hipriv_aifsn_cfg(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    wal_msg_write_stru st_write_msg;
    oal_uint32 ul_off_set;
    oal_int8 ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    mac_edca_cfg_stru st_edca_cfg;
    oal_int32 l_ret;
    oal_uint32 ul_ret;

    memset_s(&st_edca_cfg, OAL_SIZEOF(st_edca_cfg), 0, OAL_SIZEOF(st_edca_cfg));

    /* ��ȡ���ÿ��� */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, OAL_SIZEOF(ac_name), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_aifsn_cfg::get wfa switch fail, return err_code[%d]!}", ul_ret);
        return ul_ret;
    }
    pc_param += ul_off_set;
    st_edca_cfg.en_switch = (oal_bool_enum_uint8)oal_atoi(ac_name);

    /* ��ȡac */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, OAL_SIZEOF(ac_name), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_aifsn_cfg::get wfa ac fail, return err_code[%d]!}", ul_ret);
        return ul_ret;
    }
    pc_param += ul_off_set;
    st_edca_cfg.en_ac = (wlan_wme_ac_type_enum_uint8)oal_atoi(ac_name);

    if (st_edca_cfg.en_switch == OAL_TRUE) {
        /* ��ȡ����ֵ */
        ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, OAL_SIZEOF(ac_name), &ul_off_set);
        if (ul_ret != OAL_SUCC) {
            OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_aifsn_cfg::get wfa val fail, return err_code[%d]!}", ul_ret);
            return ul_ret;
        }
        pc_param += ul_off_set;
        st_edca_cfg.us_val = (oal_uint16)oal_atoi(ac_name);
    }
    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_WFA_CFG_AIFSN, OAL_SIZEOF(st_edca_cfg));

    /* ��������������� */
    if (memcpy_s(st_write_msg.auc_value,
                 OAL_SIZEOF(st_write_msg.auc_value),
                 (const oal_void *)&st_edca_cfg,
                 OAL_SIZEOF(st_edca_cfg)) != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "wal_hipriv_aifsn_cfg::memcpy fail!");
        return OAL_FAIL;
    }

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(st_edca_cfg),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_aifsn_cfg::return err code[%d]!}", ul_ret);
        return (oal_uint32)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC oal_uint32 wal_hipriv_cw_cfg(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    wal_msg_write_stru st_write_msg;
    oal_uint32 ul_off_set;
    oal_int8 ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    mac_edca_cfg_stru st_edca_cfg;
    oal_int32 l_ret;
    oal_uint32 ul_ret;

    memset_s(&st_edca_cfg, OAL_SIZEOF(st_edca_cfg), 0, OAL_SIZEOF(st_edca_cfg));

    /* ��ȡ���ÿ��� */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, OAL_SIZEOF(ac_name), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_cw_cfg::get wfa switch fail, return err_code[%d]!}", ul_ret);
        return ul_ret;
    }
    pc_param += ul_off_set;
    st_edca_cfg.en_switch = (oal_bool_enum_uint8)oal_atoi(ac_name);

    /* ��ȡac */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, OAL_SIZEOF(ac_name), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_cw_cfg::get wfa ac fail, return err_code[%d]!}", ul_ret);
        return ul_ret;
    }
    pc_param += ul_off_set;
    st_edca_cfg.en_ac = (wlan_wme_ac_type_enum_uint8)oal_atoi(ac_name);

    if (st_edca_cfg.en_switch == OAL_TRUE) {
        /* ��ȡ����ֵ */
        ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, OAL_SIZEOF(ac_name), &ul_off_set);
        if (ul_ret != OAL_SUCC) {
            OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_cw_cfg::get wfa val fail, return err_code[%d]!}", ul_ret);
            return ul_ret;
        }
        pc_param += ul_off_set;
        st_edca_cfg.us_val = (oal_uint16)oal_strtol(ac_name, OAL_PTR_NULL, 0);
    }
    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_WFA_CFG_CW, OAL_SIZEOF(st_edca_cfg));

    /* ��������������� */
    if (memcpy_s(st_write_msg.auc_value,
                 OAL_SIZEOF(st_write_msg.auc_value),
                 (const oal_void *)&st_edca_cfg,
                 OAL_SIZEOF(st_edca_cfg)) != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "wal_hipriv_cw_cfg::memcpy fail!");
        return OAL_FAIL;
    }

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(st_edca_cfg),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_aifsn_cfg::return err code[%d]!}", ul_ret);
        return (oal_uint32)l_ret;
    }

    return OAL_SUCC;
}
#endif

OAL_STATIC oal_uint32 wal_hipriv_set_random_mac_addr_scan(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    wal_msg_write_stru st_write_msg;
    oal_uint32 ul_off_set;
    oal_int8 ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    oal_uint32 ul_ret;
    oal_int32 l_ret;
    oal_uint8 uc_rand_mac_addr_scan_switch;

    /* sh hipriv.sh "Hisilicon0 random_mac_addr_scan 0|1(����)" */
    /* ��ȡ֡���� */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, OAL_SIZEOF(ac_name), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_hipriv_set_random_mac_addr_scan:get switch return err_code[%d]", ul_ret);
        return ul_ret;
    }
    pc_param += ul_off_set;
    uc_rand_mac_addr_scan_switch = (oal_uint8)oal_atoi(ac_name);
    /* ���ص�ȡֵ��ΧΪ0|1,�������Ϸ����ж� */
    if (uc_rand_mac_addr_scan_switch > 1) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "{wal_hipriv_set_random_mac_addr_scan::param is error, switch_value[%d]!}",
                       uc_rand_mac_addr_scan_switch);
        return OAL_FAIL;
    }

    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_RANDOM_MAC_ADDR_SCAN, OAL_SIZEOF(oal_uint32));
    *((oal_int32 *)(st_write_msg.auc_value)) = (oal_uint32)uc_rand_mac_addr_scan_switch;

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(oal_uint32),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_set_random_mac_addr_scan::return err code[%d]!}", l_ret);
        return (oal_uint32)l_ret;
    }

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_UAPSD

OAL_STATIC oal_uint32 wal_hipriv_set_uapsd_cap(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    wal_msg_write_stru st_write_msg;
    oal_int32 l_tmp;
    oal_uint32 ul_off_set;
    oal_int8 ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    oal_int32 l_ret;
    oal_uint32 ul_ret;

    /* �˴���������"1"��"0"����ac_name */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, OAL_SIZEOF(ac_name), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_set_uapsd_cap::wal_get_cmd_one_arg return err_code[%d]}", ul_ret);
        return ul_ret;
    }

    /* ��Խ������Ĳ�ͬ�����UAPSD���ؽ��в�ͬ������ */
    if ((strcmp("0", ac_name)) == 0) {
        l_tmp = 0;
    } else if ((strcmp("1", ac_name)) == 0) {
        l_tmp = 1;
    } else {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_hipriv_set_uapsd_cap::the log switch command is error [%x]",
                         (uintptr_t)ac_name);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_UAPSD_EN, OAL_SIZEOF(oal_int32));
    *((oal_int32 *)(st_write_msg.auc_value)) = l_tmp; /* ��������������� */

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(oal_int32),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_event_switch::return err code [%d]!}\r\n", l_ret);
        return (oal_uint32)l_ret;
    }

    return OAL_SUCC;
}
#endif


oal_uint32 wal_hipriv_add_user(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    wal_msg_write_stru st_write_msg;
    oal_uint32 ul_off_set;
    oal_int8 ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    oal_uint32 ul_ret;
    oal_int32 l_ret;
    mac_cfg_add_user_param_stru *pst_add_user_param = OAL_PTR_NULL;
    mac_cfg_add_user_param_stru st_add_user_param; /* ��ʱ�����ȡ��use����Ϣ */
    oal_uint32 ul_get_addr_idx;

    /*
        ��������û�����������: hipriv "vap0 add_user xx xx xx xx xx xx(mac��ַ) 0 | 1(HT����λ) "
        ���������ĳһ��VAP
 */
    /* ��ȡmac��ַ */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, OAL_SIZEOF(ac_name), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_add_user::wal_get_cmd_one_arg return err_code [%d]!}", ul_ret);
        return ul_ret;
    }

    memset_s((oal_uint8 *)&st_add_user_param, OAL_SIZEOF(st_add_user_param), 0, OAL_SIZEOF(st_add_user_param));
    oal_strtoaddr(ac_name, st_add_user_param.auc_mac_addr);
    /* ƫ�ƣ�ȡ��һ������ */
    pc_param = pc_param + ul_off_set;

    /* ��ȡ�û���HT��ʶ */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, OAL_SIZEOF(ac_name), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_add_user::wal_get_cmd_one_arg return err_code [%d]!}", ul_ret);
        return ul_ret;
    }

    /* ��Խ������Ĳ�ͬ�����user��HT�ֶν��в�ͬ������ */
    if ((strcmp("0", ac_name)) == 0) {
        st_add_user_param.en_ht_cap = 0;
    } else if ((strcmp("1", ac_name)) == 0) {
        st_add_user_param.en_ht_cap = 1;
    } else {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_hipriv_add_user:the mod switch command is error[%x]", (uintptr_t)ac_name);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_ADD_USER, OAL_SIZEOF(mac_cfg_add_user_param_stru));

    /* ��������������� */
    pst_add_user_param = (mac_cfg_add_user_param_stru *)(st_write_msg.auc_value);
    for (ul_get_addr_idx = 0; ul_get_addr_idx < WLAN_MAC_ADDR_LEN; ul_get_addr_idx++) {
        pst_add_user_param->auc_mac_addr[ul_get_addr_idx] = st_add_user_param.auc_mac_addr[ul_get_addr_idx];
    }
    pst_add_user_param->en_ht_cap = st_add_user_param.en_ht_cap;

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_add_user_param_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_add_user::return err code[%d]!}\r\n", l_ret);
        return (oal_uint32)l_ret;
    }

    return OAL_SUCC;
}


oal_uint32 wal_hipriv_del_user(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    wal_msg_write_stru st_write_msg;
    oal_uint32 ul_off_set;
    oal_int8 ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    oal_uint32 ul_ret;
    oal_int32 l_ret;
    mac_cfg_del_user_param_stru *pst_del_user_param = OAL_PTR_NULL;
    mac_cfg_del_user_param_stru st_del_user_param; /* ��ʱ�����ȡ��use����Ϣ */
    oal_uint32 ul_get_addr_idx;

    /*
        ����ɾ���û�����������: hipriv "vap0 del_user xx xx xx xx xx xx(mac��ַ)"
        ���������ĳһ��VAP
 */
    /* ��ȡmac��ַ */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, OAL_SIZEOF(ac_name), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_del_user::wal_get_cmd_one_arg return err_code [%d]!}", ul_ret);
        return ul_ret;
    }

    memset_s((oal_uint8 *)&st_del_user_param, OAL_SIZEOF(st_del_user_param), 0, OAL_SIZEOF(st_del_user_param));
    oal_strtoaddr(ac_name, st_del_user_param.auc_mac_addr);
    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_DEL_USER, OAL_SIZEOF(mac_cfg_add_user_param_stru));

    /* ��������������� */
    pst_del_user_param = (mac_cfg_add_user_param_stru *)(st_write_msg.auc_value);
    for (ul_get_addr_idx = 0; ul_get_addr_idx < WLAN_MAC_ADDR_LEN; ul_get_addr_idx++) {
        pst_del_user_param->auc_mac_addr[ul_get_addr_idx] = st_del_user_param.auc_mac_addr[ul_get_addr_idx];
    }

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_add_user_param_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_del_user::return err code[%d]!}\r\n", l_ret);
        return (oal_uint32)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC oal_uint32 wal_hipriv_user_info(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    wal_msg_write_stru st_write_msg;
    oal_int32 l_ret;
    mac_cfg_user_info_param_stru *pst_user_info_param = OAL_PTR_NULL;
    oal_uint8 auc_mac_addr[6] = { 0 }; /* ��ʱ�����ȡ��use��mac��ַ��Ϣ */
    oal_uint8 uc_char_index;
    oal_uint16 us_user_idx;

    /* ȥ���ַ����Ŀո� */
    pc_param++;

    /* ��ȡmac��ַ,16����ת�� */
    for (uc_char_index = 0; uc_char_index < 12; uc_char_index++) { /* uc_char_indexС��12 */
        if (*pc_param == ':') {
            pc_param++;
            if (uc_char_index != 0) {
                uc_char_index--;
            }

            continue;
        }

        auc_mac_addr[uc_char_index / 2] = /* 2��16��Ϊ16����ת���������� */
            (oal_uint8)(auc_mac_addr[uc_char_index / 2] * 16 * (uc_char_index % 2) +
                        oal_strtohex(pc_param));
        pc_param++;
    }

    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_USER_INFO, OAL_SIZEOF(mac_cfg_user_info_param_stru));

    /* ����mac��ַ���û� */
    pst_mac_vap = OAL_NET_DEV_PRIV(pst_net_dev);

    l_ret = (oal_int32)mac_vap_find_user_by_macaddr(pst_mac_vap, auc_mac_addr, &us_user_idx);
    if (l_ret != OAL_SUCC) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_hipriv_user_info::no such user!}\r\n");
        return OAL_FAIL;
    }

    /* ��������������� */
    pst_user_info_param = (mac_cfg_user_info_param_stru *)(st_write_msg.auc_value);
    pst_user_info_param->us_user_idx = us_user_idx;

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_user_info_param_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_hipriv_user_info::return err code [%d]!}", l_ret);
        return (oal_uint32)l_ret;
    }

    return OAL_SUCC;
}


oal_uint32 wal_hipriv_set_mcast_data_dscr_param(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    wal_msg_write_stru st_write_msg;
    oal_uint32 ul_off_set;
    oal_uint32 ul_ret;
    oal_int32 l_ret;
    mac_cfg_set_dscr_param_stru *pst_set_dscr_param;
    wal_dscr_param_enum_uint8 en_param_index;
    oal_int8 ac_arg[WAL_HIPRIV_CMD_NAME_MAX_LEN];

    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_DSCR, OAL_SIZEOF(mac_cfg_set_dscr_param_stru));

    /* ��������������������� */
    pst_set_dscr_param = (mac_cfg_set_dscr_param_stru *)(st_write_msg.auc_value);

    /* ��ȡ�������ֶ����������ַ��� */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, OAL_SIZEOF(ac_arg), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_hipriv_set_mcast_data_dscr_param:wal_get_cmd_one_arg fail err_code [%d]",
                         ul_ret);
        return ul_ret;
    }
    pc_param += ul_off_set;

    /* ������������һ���ֶ� */
    for (en_param_index = 0; en_param_index < WAL_DSCR_PARAM_BUTT; en_param_index++) {
        if (!strcmp(pauc_tx_dscr_param_name[en_param_index], ac_arg)) {
            break;
        }
    }

    /* ��������Ƿ��� */
    if (en_param_index == WAL_DSCR_PARAM_BUTT) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_hipriv_set_mcast_data_dscr_param::no such param for tx dscr!}\r\n");
        return OAL_FAIL;
    }

    pst_set_dscr_param->uc_function_index = en_param_index;

    /* ����Ҫ����Ϊ����ֵ */
    pst_set_dscr_param->l_value = oal_strtol(pc_param, OAL_PTR_NULL, 0);

    /* �鲥����֡���������� tpye = MAC_VAP_CONFIG_MCAST_DATA */
    pst_set_dscr_param->en_type = MAC_VAP_CONFIG_MCAST_DATA;

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_set_dscr_param_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_set_mcast_data_dscr_param::return err code [%d]!}\r\n", l_ret);
        return (oal_uint32)l_ret;
    }

    return OAL_SUCC;
}


oal_uint32 wal_hipriv_set_rate(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    wal_msg_write_stru st_write_msg;
    oal_uint32 ul_off_set;
    oal_uint32 ul_ret;
    oal_int32 l_ret;
    mac_cfg_non_ht_rate_stru *pst_set_rate_param;
    wlan_legacy_rate_value_enum_uint8 en_rate_index;
    oal_int8 ac_arg[WAL_HIPRIV_CMD_NAME_MAX_LEN];

    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_RATE, OAL_SIZEOF(mac_cfg_non_ht_rate_stru));

    /* ��������������������� */
    pst_set_rate_param = (mac_cfg_non_ht_rate_stru *)(st_write_msg.auc_value);

    /* ��ȡ����ֵ�ַ��� */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, OAL_SIZEOF(ac_arg), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_set_rate::wal_get_cmd_one_arg return err_code [%d]!}", ul_ret);
        return ul_ret;
    }

    /* ����������Ϊ��һ������ */
    for (en_rate_index = 0; en_rate_index < WLAN_LEGACY_RATE_VALUE_BUTT; en_rate_index++) {
        if (!strcmp(pauc_non_ht_rate_tbl[en_rate_index], ac_arg)) {
            break;
        }
    }

    /* ������������TX�������е�Э��ģʽ */
    if (en_rate_index <= WLAN_SHORT_11b_11_M_BPS) {
        pst_set_rate_param->en_protocol_mode = WLAN_11B_PHY_PROTOCOL_MODE;
    } else if ((en_rate_index >= WLAN_LEGACY_OFDM_48M_BPS) && (en_rate_index <= WLAN_LEGACY_OFDM_9M_BPS)) {
        pst_set_rate_param->en_protocol_mode = WLAN_LEGACY_OFDM_PHY_PROTOCOL_MODE;
    } else {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_hipriv_set_rate::invalid rate!}\r\n");
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    /* ����Ҫ����Ϊ����ֵ */
    pst_set_rate_param->en_rate = en_rate_index;

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_non_ht_rate_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_set_rate::return err code [%d]!}\r\n", l_ret);
        return (oal_uint32)l_ret;
    }
    return OAL_SUCC;
}


oal_uint32 wal_hipriv_set_mcs(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    wal_msg_write_stru st_write_msg;
    oal_uint32 ul_off_set;
    oal_uint32 ul_ret;
    oal_int32 l_ret;
    mac_cfg_tx_comp_stru *pst_set_mcs_param;
    oal_int32 l_mcs;
    oal_int8 ac_arg[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    oal_int32 l_idx = 0;

    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_MCS, OAL_SIZEOF(mac_cfg_tx_comp_stru));

    /* ��������������������� */
    pst_set_mcs_param = (mac_cfg_tx_comp_stru *)(st_write_msg.auc_value);

    /* ��ȡ����ֵ�ַ��� */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, OAL_SIZEOF(ac_arg), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_set_mcs::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    /* ��������Ϸ��Լ�� */
    while (ac_arg[l_idx] != '\0') {
        if (isdigit(ac_arg[l_idx])) {
            l_idx++;
            continue;
        } else {
            l_idx++;
            OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_hipriv_set_mcs::input illegal!}\r\n");
            return OAL_ERR_CODE_INVALID_CONFIG;
        }
    }

    /* ����Ҫ����Ϊ����ֵ */
    l_mcs = oal_atoi(ac_arg);
    if ((l_mcs < WAL_HIPRIV_HT_MCS_MIN) || (l_mcs > WAL_HIPRIV_HT_MCS_MAX)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_set_mcs::input val out of range [%d]!}\r\n", l_mcs);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    pst_set_mcs_param->uc_param = (oal_uint8)l_mcs;
    pst_set_mcs_param->en_protocol_mode = WLAN_HT_PHY_PROTOCOL_MODE;

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_tx_comp_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_set_mcs::return err code [%d]!}\r\n", l_ret);
        return (oal_uint32)l_ret;
    }
    return OAL_SUCC;
}


oal_uint32 wal_hipriv_set_mcsac(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    wal_msg_write_stru st_write_msg;
    oal_uint32 ul_off_set;
    oal_uint32 ul_ret;
    oal_int32 l_ret;
    mac_cfg_tx_comp_stru *pst_set_mcs_param;
    oal_int32 l_mcs;
    oal_int8 ac_arg[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    oal_int32 l_idx = 0;

    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_MCSAC, OAL_SIZEOF(mac_cfg_tx_comp_stru));

    /* ��������������������� */
    pst_set_mcs_param = (mac_cfg_tx_comp_stru *)(st_write_msg.auc_value);

    /* ��ȡ����ֵ�ַ��� */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, OAL_SIZEOF(ac_arg), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_set_mcsac::wal_get_cmd_one_arg return err_code [%d]!}", ul_ret);
        return ul_ret;
    }

    /* ��������Ϸ��Լ�� */
    while (ac_arg[l_idx] != '\0') {
        if (isdigit(ac_arg[l_idx])) {
            l_idx++;
            continue;
        } else {
            l_idx++;
            OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_hipriv_set_mcsac::input illegal!}\r\n");
            return OAL_ERR_CODE_INVALID_CONFIG;
        }
    }

    /* ����Ҫ����Ϊ����ֵ */
    l_mcs = oal_atoi(ac_arg);
    if ((l_mcs < WAL_HIPRIV_VHT_MCS_MIN) || (l_mcs > WAL_HIPRIV_VHT_MCS_MAX)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_set_mcs::input val out of range [%d]!}\r\n", l_mcs);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    pst_set_mcs_param->uc_param = (oal_uint8)l_mcs;
    pst_set_mcs_param->en_protocol_mode = WLAN_VHT_PHY_PROTOCOL_MODE;

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_tx_comp_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_set_mcsac::return err code [%d]!}\r\n", l_ret);
        return (oal_uint32)l_ret;
    }
    return OAL_SUCC;
}


oal_uint32 wal_hipriv_set_bw(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    wal_msg_write_stru st_write_msg;
    oal_uint32 ul_off_set;
    oal_uint32 ul_ret;
    oal_int32 l_ret;
    mac_cfg_tx_comp_stru *pst_set_bw_param;
    hal_channel_assemble_enum_uint8 en_bw_index;
    oal_int8 ac_arg[WAL_HIPRIV_CMD_NAME_MAX_LEN];

    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_BW, OAL_SIZEOF(mac_cfg_tx_comp_stru));

    /* ��������������������� */
    pst_set_bw_param = (mac_cfg_tx_comp_stru *)(st_write_msg.auc_value);

    /* ��ȡ����ֵ�ַ��� */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, OAL_SIZEOF(ac_arg), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_set_bw::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    /* ����Ҫ����Ϊ����ֵ */
    for (en_bw_index = 0; en_bw_index < WLAN_BAND_ASSEMBLE_AUTO; en_bw_index++) {
        if (!strcmp(pauc_bw_tbl[en_bw_index], ac_arg)) {
            break;
        }
    }

    /* ��������Ƿ��� */
    if (en_bw_index >= WLAN_BAND_ASSEMBLE_AUTO) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_hipriv_set_bw::not support this bandwidth!}\r\n");
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    pst_set_bw_param->uc_param = (oal_uint8)(en_bw_index);

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_tx_comp_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_set_bw::return err code [%d]!}\r\n", l_ret);
        return (oal_uint32)l_ret;
    }
    return OAL_SUCC;
}


OAL_STATIC oal_uint32 wal_hipriv_set_pow_rf_ctl(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    wal_msg_write_stru st_write_msg;
    oal_uint32 ul_off_set;
    oal_uint32 ul_ret;
    oal_int32 l_ret;
    oal_bool_enum_uint8 en_rf_regctl = OAL_TRUE;
    oal_uint8 uc_arg_val = 0;
    oal_int8 ac_arg[WAL_HIPRIV_CMD_NAME_MAX_LEN];

    /* ��ȡ����ֵ�ַ��� */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, OAL_SIZEOF(ac_arg), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_hipriv_set_pow_rf_ctl::wal_get_cmd_one_arg return err_code [%d]", ul_ret);
        return ul_ret;
    }
    uc_arg_val = (oal_uint8)oal_atoi(ac_arg);    if (uc_arg_val != 0) {
        en_rf_regctl = OAL_TRUE;
    } else {
        en_rf_regctl = OAL_FALSE;
    }
    OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_hipriv_set_pow_rf_ctl:en_rf_regctl=%d", en_rf_regctl);
    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_POW_RF_CTL, OAL_SIZEOF(oal_bool_enum_uint8));
    st_write_msg.auc_value[0] = en_rf_regctl;
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(oal_bool_enum_uint8),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_set_pow_rf_ctl::return err code [%d]!}\r\n", l_ret);
        return (oal_uint32)l_ret;
    }
    return OAL_SUCC;
}


#ifdef _PRE_WLAN_FEATURE_ALWAYS_TX
OAL_STATIC oal_uint32 wal_hipriv_always_tx_1102(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    wal_msg_write_stru st_write_msg;
    oal_uint32 ul_off_set;
    oal_uint32 ul_ret;
    oal_int32 l_ret;
    mac_cfg_tx_comp_stru *pst_set_bcast_param;
    oal_int8 ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    oal_switch_enum_uint8 en_tx_flag;
    mac_rf_payload_enum_uint8 en_payload_flag = RF_PAYLOAD_ALL_ZERO;
    oal_uint32 ul_len = 0;
    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_ALWAYS_TX_1102, OAL_SIZEOF(mac_cfg_tx_comp_stru));

    /* ��������������������� */
    pst_set_bcast_param = (mac_cfg_tx_comp_stru *)(st_write_msg.auc_value);

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, OAL_SIZEOF(ac_name), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_hipriv_packet_xmit::wal_get_cmd_one_arg return err_code [%d]", ul_ret);
        return ul_ret;
    }
    en_tx_flag = (oal_uint8)oal_atoi(ac_name);
    pc_param = pc_param + ul_off_set;

    if (en_tx_flag != OAL_SWITCH_OFF) {
        ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, OAL_SIZEOF(ac_name), &ul_off_set);
        if (ul_ret != OAL_SUCC) {
            OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_hipriv_packet_xmit:wal_get_cmd_one_arg return err_code [%d]", ul_ret);
            return ul_ret;
        }
        pc_param = pc_param + ul_off_set;
        en_payload_flag = (oal_uint8)oal_atoi(ac_name);
        if (en_payload_flag >= RF_PAYLOAD_BUTT) {
            OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_always_tx::payload flag err[%d]!}\r\n", en_payload_flag);
            return OAL_ERR_CODE_INVALID_CONFIG;
        }

        ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, OAL_SIZEOF(ac_name), &ul_off_set);
        if (ul_ret != OAL_SUCC) {
            OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_hipriv_packet_xmit:wal_get_cmd_one_arg return err_code[%d]", ul_ret);
            return ul_ret;
        }
        ul_len = (oal_uint16)oal_atoi(ac_name);
        if (ul_len > 65535) { /* ����65535����� */
            OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_always_tx::len [%u] overflow!}\r\n", ul_len);
            return OAL_ERR_CODE_INVALID_CONFIG;
        }
        pc_param += ul_off_set;
    }

    /* �رյ�����²���Ҫ��������Ĳ��� */
    pst_set_bcast_param->en_payload_flag = en_payload_flag;
    pst_set_bcast_param->ul_payload_len = ul_len;
    pst_set_bcast_param->uc_param = en_tx_flag;

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_tx_comp_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_always_tx::return err code [%d]!}\r\n", l_ret);
        return (oal_uint32)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC oal_uint32 wal_hipriv_always_tx_num(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    wal_msg_write_stru st_write_msg;
    oal_uint32 *pul_num;
    oal_uint32 ul_ret;
    oal_int8 ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    oal_uint32 ul_off_set;
    oal_int32 l_ret;

    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_ALWAYS_TX_NUM, OAL_SIZEOF(oal_uint32));

    /* ��ȡ�������� */
    pul_num = (oal_uint32 *)(st_write_msg.auc_value);

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, OAL_SIZEOF(ac_name), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_hipriv_always_tx_num:wal_get_cmd_one_arg return err_code [%d]", ul_ret);
        return ul_ret;
    }
    *pul_num = (oal_uint32)oal_atoi(ac_name);

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(oal_uint32),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_always_tx_num::return err code [%d]!}\r\n", l_ret);
        return (oal_uint32)l_ret;
    }

    return OAL_SUCC;
}

#endif


OAL_STATIC oal_uint32 wal_hipriv_always_rx(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    wal_msg_write_stru st_write_msg;
    oal_uint32 ul_off_set;
    oal_uint32 ul_ret;
    oal_int32 l_ret;
    oal_int8 ac_arg[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    oal_uint8 uc_rx_flag;
    oal_int32 l_idx = 0;

    /* ��ȡ����ģʽ���ر�־ */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, OAL_SIZEOF(ac_arg), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_always_rx::wal_get_cmd_one_arg return err_code [%d]!}", ul_ret);
        return ul_ret;
    }

    /* ��������Ϸ��Լ�� */
    while (ac_arg[l_idx] != '\0') {
        if (isdigit(ac_arg[l_idx])) {
            l_idx++;
            continue;
        } else {
            l_idx++;
            OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_hipriv_always_rx::input illegal!}\r\n");
            return OAL_ERR_CODE_INVALID_CONFIG;
        }
    }

    /* ���������ֵ�ַ���ת��Ϊ���� */
    uc_rx_flag = (oal_uint8)oal_atoi(ac_arg);
    if (uc_rx_flag > HAL_ALWAYS_RX_RESERVED) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_hipriv_always_rx::input should be 0 or 1.}\r\n");
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    *(oal_uint8 *)(st_write_msg.auc_value) = uc_rx_flag;

    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_ALWAYS_RX, OAL_SIZEOF(oal_uint8));

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(oal_uint8),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_always_rx::return err code [%d]!}\r\n", l_ret);
        return (oal_uint32)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC oal_uint32 wal_hipriv_pcie_pm_level(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    wal_msg_write_stru st_write_msg;
    oal_int8 ac_arg[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    oal_int32 l_ret;
    oal_uint16 us_len;
    oal_uint32 ul_ret;
    oal_uint32 ul_off_set;
    mac_cfg_pcie_pm_level_stru *pst_pcie_pm_level;

    /* �����ʽ: hipriv "Hisilicon0 pcie_pm_level level(0/1/2/3/4)" */
    pst_pcie_pm_level = (mac_cfg_pcie_pm_level_stru *)st_write_msg.auc_value;

    /* ppm */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, OAL_SIZEOF(ac_arg), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_pcie_pm_level:wal_get_cmd_one_arg return err_code [%d]}", ul_ret);
        return ul_ret;
    }

    pst_pcie_pm_level->uc_pcie_pm_level = (oal_uint8)oal_atoi(ac_arg);
    if (pst_pcie_pm_level->uc_pcie_pm_level > 4) { /* pcie�͹��ļ���,0->normal;1->L0S;2->L1;3->L1PM;4->L1s */
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_pcie_pm_level::pcie pm level must in set(0/1/2/3/4);\r\n",
                         pst_pcie_pm_level->uc_pcie_pm_level);
        return ul_ret;
    }

    us_len = OAL_SIZEOF(mac_cfg_pcie_pm_level_stru);
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_PCIE_PM_LEVEL, us_len);

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + us_len,
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_pcie_pm_level::return err code [%d]!}\r\n", l_ret);
        return (oal_uint32)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC int wal_ioctl_get_iwname(oal_net_device_stru *pst_net_dev,
                                    oal_iw_request_info_stru *pst_info,
                                    oal_iwreq_data_union *pst_wrqu,
                                    char *pc_extra)
{
    oal_int8 ac_iwname[] = "IEEE 802.11";

    if ((pst_net_dev == OAL_PTR_NULL) || (pst_wrqu == OAL_PTR_NULL)) {
        OAM_ERROR_LOG2(0, OAM_SF_ANY, "{wal_ioctl_get_iwname::param null, pst_net_dev = %p, pc_name = %p.}",
                       (uintptr_t)pst_net_dev, (uintptr_t)pst_wrqu);
        return -OAL_EINVAL;
    }

    if (memcpy_s(pst_wrqu->name, OAL_SIZEOF(pst_wrqu->name),
                 ac_iwname, OAL_MIN(OAL_SIZEOF(ac_iwname), OAL_SIZEOF(pst_wrqu->name))) != EOK) { /*lint !e506*/
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_ioctl_get_iwname::memcpy fail.}");
        return -OAL_EINVAL;
    }

    return OAL_SUCC;
}


OAL_STATIC oal_uint32 wal_hipriv_find_cmd(
    oal_int8 *pc_cmd_name, oal_uint8 uc_cmd_name_len, wal_hipriv_cmd_entry_stru **pst_cmd_id)
{
    oal_uint32 en_cmd_idx;
    int l_ret;
    *pst_cmd_id = NULL;

    if (OAL_UNLIKELY((pc_cmd_name == OAL_PTR_NULL) || (pst_cmd_id == OAL_PTR_NULL))) {
        OAM_ERROR_LOG2(0, OAM_SF_ANY, "{wal_hipriv_find_cmd::pc_cmd_name/puc_cmd_id null ptr error [%x] [%x]!}\r\n",
                       (uintptr_t)pc_cmd_name, (uintptr_t)pst_cmd_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (uc_cmd_name_len > WAL_HIPRIV_CMD_NAME_MAX_LEN) {
        return OAL_FAIL;
    }

    for (en_cmd_idx = 0; en_cmd_idx < OAL_ARRAY_SIZE(g_ast_hipriv_cmd); en_cmd_idx++) {
        l_ret = strcmp(g_ast_hipriv_cmd[en_cmd_idx].pc_cmd_name, pc_cmd_name);
        if (l_ret == 0) {
            *pst_cmd_id = (wal_hipriv_cmd_entry_stru *)&g_ast_hipriv_cmd[en_cmd_idx];

            return OAL_SUCC;
        }
    }

#ifdef _PRE_WLAN_CFGID_DEBUG
    for (en_cmd_idx = 0; en_cmd_idx < wal_hipriv_get_debug_cmd_size(); en_cmd_idx++) {
        l_ret = strcmp(g_ast_hipriv_cmd_debug[en_cmd_idx].pc_cmd_name, pc_cmd_name);
        if (l_ret == 0) {
            *pst_cmd_id = (wal_hipriv_cmd_entry_stru *)&g_ast_hipriv_cmd_debug[en_cmd_idx];

            return OAL_SUCC;
        }
    }
#endif

    OAM_IO_PRINTK("cmd name[%s] is not exist. \r\n", pc_cmd_name);
    return OAL_FAIL;
}


OAL_STATIC oal_uint32 wal_hipriv_get_cmd_net_dev(
    oal_int8 *pc_cmd, oal_net_device_stru **ppst_net_dev, oal_uint32 *pul_off_set)
{
    oal_net_device_stru *pst_net_dev = OAL_PTR_NULL;
    oal_int8 ac_dev_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    oal_uint32 ul_ret;

    if (OAL_UNLIKELY((pc_cmd == OAL_PTR_NULL) || (ppst_net_dev == OAL_PTR_NULL) || (pul_off_set == OAL_PTR_NULL))) {
        OAM_ERROR_LOG3(0, OAM_SF_ANY, "wal_hipriv_get_cmd_net_dev:pc_cmd[%x]ppst_net_dev[%x]pul_off_set[%x]",
                       (uintptr_t)pc_cmd, (uintptr_t)ppst_net_dev, (uintptr_t)pul_off_set);
        return OAL_ERR_CODE_PTR_NULL;
    }

    ul_ret = wal_get_cmd_one_arg(pc_cmd, ac_dev_name, OAL_SIZEOF(ac_dev_name), pul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_hipriv_get_cmd_net_dev:wal_get_cmd_one_arg fai, err_code [%d]}", ul_ret);
        return ul_ret;
    }

    /* ����dev_name�ҵ�dev */
    pst_net_dev = oal_dev_get_by_name(ac_dev_name);
    if (pst_net_dev == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_hipriv_get_cmd_net_dev::oal_dev_get_by_name return null ptr!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ����oal_dev_get_by_name�󣬱������oal_dev_putʹnet_dev�����ü�����һ */
    oal_dev_put(pst_net_dev);

    *ppst_net_dev = pst_net_dev;

    return OAL_SUCC;
}


OAL_STATIC oal_uint32 wal_hipriv_get_cmd_id(
    oal_int8 *pc_cmd, wal_hipriv_cmd_entry_stru **pst_cmd_id, oal_uint32 *pul_off_set)
{
    oal_int8 ac_cmd_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    oal_uint32 ul_ret;

    if (OAL_UNLIKELY((pc_cmd == OAL_PTR_NULL) || (pst_cmd_id == OAL_PTR_NULL) || (pul_off_set == OAL_PTR_NULL))) {
        OAM_ERROR_LOG3(0, OAM_SF_ANY, "{wal_hipriv_get_cmd_id::pc_cmd/puc_cmd_id/pul_off_set is null  [%x] [%x] [%x]}",
                       (uintptr_t)pc_cmd, (uintptr_t)pst_cmd_id, (uintptr_t)pul_off_set);
        return OAL_ERR_CODE_PTR_NULL;
    }

    ul_ret = wal_get_cmd_one_arg(pc_cmd, ac_cmd_name, OAL_SIZEOF(ac_cmd_name), pul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_get_cmd_id::wal_get_cmd_one_arg return err_code [%d]!}", ul_ret);
        return ul_ret;
    }

    /* �����������ҵ�����ö�� */
    ul_ret = wal_hipriv_find_cmd(ac_cmd_name, OAL_SIZEOF(ac_cmd_name), pst_cmd_id);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_get_cmd_id::wal_hipriv_find_cmd return error cod [%d]!}", ul_ret);
        return ul_ret;
    }

    return OAL_SUCC;
}


oal_uint32 wal_hipriv_parse_cmd(oal_int8 *pc_cmd)
{
    oal_net_device_stru *pst_net_dev = OAL_PTR_NULL;
    wal_hipriv_cmd_entry_stru *pst_hipriv_cmd_entry = NULL;
    oal_uint32 ul_off_set = 0;
    oal_uint32 ul_ret;
    if (OAL_UNLIKELY(pc_cmd == OAL_PTR_NULL)) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_hipriv_parse_cmd::pc_cmd null ptr error!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /***************************************************************************
        cmd��ʽԼ��
        �����豸�� ����      ����   Hisilicon0 create vap0
        1~15Byte   1~15Byte
    **************************** ***********************************************/
    ul_ret = wal_hipriv_get_cmd_net_dev(pc_cmd, &pst_net_dev, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_hipriv_parse_cmd:wal_hipriv_get_cmd_net_dev fail, error code[%d]", ul_ret);
        return ul_ret;
    }

    pc_cmd += ul_off_set;
    ul_ret = wal_hipriv_get_cmd_id(pc_cmd, &pst_hipriv_cmd_entry, &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_hipriv_parse_cmd::wal_hipriv_get_cmd_id return error code [%d]", ul_ret);
        return ul_ret;
    }

    pc_cmd += ul_off_set;

    /* ���������Ӧ�ĺ��� */
    ul_ret = pst_hipriv_cmd_entry->p_func(pst_net_dev, pc_cmd);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_parse_cmd::g_ast_hipriv_cmd return error code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }

    return OAL_SUCC;
}

#ifdef _PRE_CONFIG_CONN_HISI_SYSFS_SUPPORT

OAL_STATIC oal_ssize_t wal_hipriv_sys_write(
    struct kobject *dev, struct kobj_attribute *attr, const char *pc_buffer, oal_size_t count)
{
    oal_int8 *pc_cmd;
    oal_uint32 ul_ret;
    oal_uint32 ul_len = (oal_uint32)count;

    if (ul_len > WAL_HIPRIV_CMD_MAX_LEN) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "{wal_hipriv_sys_write::ul_len>WAL_HIPRIV_CMD_MAX_LEN, ul_len [%d]!}", ul_len);
        return -OAL_EINVAL;
    }

    pc_cmd = OAL_MEM_ALLOC(OAL_MEM_POOL_ID_LOCAL, WAL_HIPRIV_CMD_MAX_LEN, OAL_TRUE);
    if (OAL_UNLIKELY(pc_cmd == OAL_PTR_NULL)) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_hipriv_proc_write::alloc mem return null ptr!}\r\n");
        return -OAL_ENOMEM;
    }

    memset_s(pc_cmd, WAL_HIPRIV_CMD_MAX_LEN, 0, WAL_HIPRIV_CMD_MAX_LEN);

    if (memcpy_s(pc_cmd, WAL_HIPRIV_CMD_MAX_LEN, pc_buffer, ul_len) != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "wal_hipriv_sys_write::memcpy fail!");
        OAL_MEM_FREE(pc_cmd, OAL_TRUE);
        return -OAL_EINVAL;
    }

    pc_cmd[ul_len - 1] = '\0';

    OAM_IO_PRINTK(" %s\n", pc_cmd);

    ul_ret = wal_hipriv_parse_cmd(pc_cmd);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_proc_write::parse cmd return err code[%d]!}\r\n", ul_ret);
    }

    OAL_MEM_FREE(pc_cmd, OAL_TRUE);

    return (oal_int32)ul_len;
}


#define SYS_READ_MAX_STRING_LEN (4096 - 40) /* ��ǰ�����ַ�����20�ֽ��ڣ�Ԥ��40��֤���ᳬ�� */
OAL_STATIC oal_ssize_t wal_hipriv_sys_read(struct kobject *dev, struct kobj_attribute *attr, char *pc_buffer)
{
    oal_uint32 ul_cmd_idx;
    oal_uint32 buff_index = 0;

    for (ul_cmd_idx = 0; ul_cmd_idx < OAL_ARRAY_SIZE(g_ast_hipriv_cmd); ul_cmd_idx++) {
        buff_index += snprintf_s(pc_buffer + buff_index, (SYS_READ_MAX_STRING_LEN - buff_index),
            (SYS_READ_MAX_STRING_LEN - buff_index) - 1, "\t%s\n", g_ast_hipriv_cmd[ul_cmd_idx].pc_cmd_name);
        if (buff_index > SYS_READ_MAX_STRING_LEN) {
            buff_index += snprintf_s(pc_buffer + buff_index, (SYS_READ_MAX_STRING_LEN - buff_index),
                (SYS_READ_MAX_STRING_LEN - buff_index) - 1, "\tmore...\n");
            break;
        }
    }
#ifdef _PRE_WLAN_CFGID_DEBUG
    for (ul_cmd_idx = 0; ul_cmd_idx < wal_hipriv_get_debug_cmd_size(); ul_cmd_idx++) {
        buff_index += snprintf_s(pc_buffer + buff_index, (SYS_READ_MAX_STRING_LEN - buff_index),
            (SYS_READ_MAX_STRING_LEN - buff_index) - 1, "\t%s\n", g_ast_hipriv_cmd_debug[ul_cmd_idx].pc_cmd_name);
        if (buff_index > SYS_READ_MAX_STRING_LEN) {
            buff_index += snprintf_s(pc_buffer + buff_index, (SYS_READ_MAX_STRING_LEN - buff_index),
                (SYS_READ_MAX_STRING_LEN - buff_index) - 1, "\tmore...\n");
            break;
        }
    }
#endif

    return buff_index;
}

#endif /* _PRE_OS_VERSION_LINUX */

oal_int32 wal_hipriv_proc_write(oal_file_stru *pst_file, const oal_int8 *pc_buffer, oal_uint32 ul_len, oal_void *p_data)
{
    oal_int8 *pc_cmd = OAL_PTR_NULL;
    oal_uint32 ul_ret;

    if (ul_len > WAL_HIPRIV_CMD_MAX_LEN) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "{wal_hipriv_proc_write::ul_len>WAL_HIPRIV_CMD_MAX_LEN, ul_len [%d]!}", ul_len);
        return -OAL_EINVAL;
    }

    pc_cmd = OAL_MEM_ALLOC(OAL_MEM_POOL_ID_LOCAL, WAL_HIPRIV_CMD_MAX_LEN, OAL_TRUE);
    if (OAL_UNLIKELY(pc_cmd == OAL_PTR_NULL)) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_hipriv_proc_write::alloc mem return null ptr!}\r\n");
        return -OAL_ENOMEM;
    }

    memset_s(pc_cmd, WAL_HIPRIV_CMD_MAX_LEN, 0, WAL_HIPRIV_CMD_MAX_LEN);

    ul_ret = oal_copy_from_user((oal_void *)pc_cmd, pc_buffer, ul_len);
    /* copy_from_user������Ŀ���Ǵ��û��ռ俽�����ݵ��ں˿ռ䣬ʧ�ܷ���û�б��������ֽ������ɹ�����0 */
    if (ul_ret > 0) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_proc_write::oal_copy_from_user return ul_ret[%d]!}\r\n", ul_ret);
        OAL_MEM_FREE(pc_cmd, OAL_TRUE);

        return -OAL_EFAUL;
    }

    pc_cmd[ul_len - 1] = '\0';

    ul_ret = wal_hipriv_parse_cmd(pc_cmd);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_proc_write::parse cmd return err code[%d]!}\r\n", ul_ret);
    }

    OAL_MEM_FREE(pc_cmd, OAL_TRUE);

    return (oal_int32)ul_len;
}


oal_uint32 wal_hipriv_create_proc(oal_void *p_proc_arg)
{
#ifdef _PRE_CONFIG_CONN_HISI_SYSFS_SUPPORT
    oal_uint32 ul_ret;
#endif

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 10, 44))
    g_pst_proc_entry = OAL_PTR_NULL;
#else

    /*
        S_IRUSR�ļ������߾߿ɶ�ȡȨ��, S_IWUSR�ļ������߾߿�д��Ȩ��,
        S_IRGRP�û���߿ɶ�ȡȨ��, S_IROTH�����û��߿ɶ�ȡȨ��
 */
    /* 420ʮ���ƶ�Ӧ�˽�����0644 linuxģʽ���� S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH); */
    g_pst_proc_entry = oal_create_proc_entry(WAL_HIPRIV_PROC_ENTRY_NAME, 420, NULL);
    if (g_pst_proc_entry == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_hipriv_create_proc::oal_create_proc_entry return null ptr!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    g_pst_proc_entry->data = p_proc_arg;
    g_pst_proc_entry->nlink = 1; /* linux����procĬ��ֵ */
    g_pst_proc_entry->read_proc = OAL_PTR_NULL;

    g_pst_proc_entry->write_proc = (write_proc_t *)wal_hipriv_proc_write;
#endif

    /* hi1102-cb add sys for 51/02 */
#ifdef _PRE_CONFIG_CONN_HISI_SYSFS_SUPPORT
    gp_sys_kobject = oal_get_sysfs_root_object();
    if (gp_sys_kobject == NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_hipriv_create_proc::get sysfs root object failed!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    ul_ret = (oal_uint32)oal_debug_sysfs_create_group(gp_sys_kobject, &hipriv_attribute_group);
    if (ul_ret) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_hipriv_create_proc::hipriv_attribute_group create failed!}");
        ul_ret = OAL_ERR_CODE_PTR_NULL;
        return ul_ret;
    }
#endif

    return OAL_SUCC;
}


oal_uint32 wal_hipriv_remove_proc(void)
{
    /* ж��ʱɾ��sysfs */
#ifdef _PRE_CONFIG_CONN_HISI_SYSFS_SUPPORT
    if (gp_sys_kobject != NULL) {
        oal_debug_sysfs_remove_group(gp_sys_kobject, &hipriv_attribute_group);
        gp_sys_kobject = NULL;
    }
#endif

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)

    oal_remove_proc_entry(WAL_HIPRIV_PROC_ENTRY_NAME, NULL);
#elif (_PRE_OS_VERSION_WIN32 == _PRE_OS_VERSION)

    oal_remove_proc_entry(WAL_HIPRIV_PROC_ENTRY_NAME, g_pst_proc_entry);
#endif
    return OAL_SUCC;
}


OAL_STATIC oal_uint32 wal_hipriv_reg_info(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    wal_msg_write_stru st_write_msg;
    oal_int32 l_ret;
    oal_uint16 us_len;

    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    if (memcpy_s(st_write_msg.auc_value, OAL_SIZEOF(st_write_msg.auc_value), pc_param, OAL_STRLEN(pc_param)) != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "wal_hipriv_reg_info::memcpy fail!");
        return OAL_FAIL;
    }

    st_write_msg.auc_value[OAL_STRLEN(pc_param)] = '\0';

    us_len = (oal_uint16)(OAL_STRLEN(pc_param) + 1);

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_REG_INFO, us_len);

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + us_len,
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_reg_info::return err code [%d]!}\r\n", l_ret);
        return (oal_uint32)l_ret;
    }

    return OAL_SUCC;
}

#if (defined(_PRE_PRODUCT_ID_HI110X_DEV) || defined(_PRE_PRODUCT_ID_HI110X_HOST))

OAL_STATIC oal_uint32 wal_hipriv_sdio_flowctrl(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    wal_msg_write_stru st_write_msg;
    oal_int32 l_ret;
    oal_uint16 us_len;

    if (OAL_UNLIKELY(OAL_STRLEN(pc_param) >= WAL_MSG_WRITE_MAX_LEN)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_sdio_flowctrl:pc_param overlength is %d}", OAL_STRLEN(pc_param));
        return OAL_FAIL;
    }

    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    if (memcpy_s(st_write_msg.auc_value, OAL_SIZEOF(st_write_msg.auc_value), pc_param, OAL_STRLEN(pc_param)) != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "wal_hipriv_sdio_flowctrl::memcpy fail!");
        return OAL_FAIL;
    }

    st_write_msg.auc_value[OAL_STRLEN(pc_param)] = '\0';

    us_len = (oal_uint16)(OAL_STRLEN(pc_param) + 1);

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SDIO_FLOWCTRL, us_len);

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + us_len,
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_sdio_flowctrl::return err code [%d]!}\r\n", l_ret);
        return (oal_uint32)l_ret;
    }

    return OAL_SUCC;
}
#endif


OAL_STATIC oal_uint32 wal_hipriv_set_regdomain_pwr(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    oal_uint32 ul_off_set;
    oal_int8 ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    oal_uint32 ul_ret;
    oal_int32 l_ret;
    oal_int32 l_pwr;
    wal_msg_write_stru st_write_msg;

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, OAL_SIZEOF(ac_name), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_CFG, "wal_hipriv_set_regdomain_pwr, get arg return err %d", ul_ret);
        return ul_ret;
    }

    l_pwr = oal_atoi(ac_name);
    if ((l_pwr <= 0) || (l_pwr > 100)) { /* ����100����Чֵ */
        OAM_WARNING_LOG1(0, OAM_SF_CFG, "invalid value, %d", l_pwr);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_REGDOMAIN_PWR, OAL_SIZEOF(mac_cfg_regdomain_max_pwr_stru));

    ((mac_cfg_regdomain_max_pwr_stru *)st_write_msg.auc_value)->uc_pwr = (oal_uint8)l_pwr;
    ((mac_cfg_regdomain_max_pwr_stru *)st_write_msg.auc_value)->en_exceed_reg = OAL_FALSE;

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(oal_int32),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (l_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_CFG, "{wal_hipriv_set_regdomain_pwr::wal_send_cfg_event fail.err code %d}", l_ret);
    }

    return (oal_uint32)l_ret;
}


OAL_STATIC oal_uint32 wal_hipriv_dump_all_rx_dscr(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    wal_msg_write_stru st_write_msg;
    oal_int32 l_ret;

    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_DUMP_ALL_RX_DSCR, OAL_SIZEOF(oal_int32));

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(oal_int32),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_dump_all_rx_dscr::return err code [%d]!}\r\n", l_ret);
        return (oal_uint32)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC oal_uint32 wal_hipriv_reg_write(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    wal_msg_write_stru st_write_msg;
    oal_int32 l_ret;
    oal_uint16 us_len;

    /***************************************************************************
                              ���¼���wal�㴦��
    ***************************************************************************/
    if (memcpy_s(st_write_msg.auc_value, OAL_SIZEOF(st_write_msg.auc_value), pc_param, OAL_STRLEN(pc_param)) != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "wal_hipriv_reg_write::memcpy fail!");
        return OAL_FAIL;
    }

    st_write_msg.auc_value[OAL_STRLEN(pc_param)] = '\0';

    us_len = (oal_uint16)(OAL_STRLEN(pc_param) + 1);

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_REG_WRITE, us_len);

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + us_len,
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_reg_write::return err code [%d]!}\r\n", l_ret);
        return (oal_uint32)l_ret;
    }

    return OAL_SUCC;
}


oal_uint32 wal_hipriv_alg_cfg(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    wal_msg_write_stru st_write_msg;
    oal_uint32 ul_off_set;
    oal_int8 ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    oal_uint32 ul_ret;
    mac_ioctl_alg_param_stru *pst_alg_param = OAL_PTR_NULL;
    wal_ioctl_alg_cfg_stru st_alg_cfg;
    oal_uint8 uc_map_index = 0;
    oal_int32 l_ret;

    pst_alg_param = (mac_ioctl_alg_param_stru *)(st_write_msg.auc_value);

    /* ��ȡ���ò������� */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, OAL_SIZEOF(ac_name), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_alg_cfg::wal_get_cmd_one_arg return err_code [%d]!}", ul_ret);
        return ul_ret;
    }

    /* Ѱ��ƥ������� */
    st_alg_cfg = g_ast_alg_cfg_map[0];
    while (st_alg_cfg.pc_name != OAL_PTR_NULL) {
        if (strcmp(st_alg_cfg.pc_name, ac_name) == 0) {
            break;
        }
        st_alg_cfg = g_ast_alg_cfg_map[++uc_map_index];
    }

    /* û���ҵ���Ӧ������򱨴� */
    if (st_alg_cfg.pc_name == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_hipriv_alg_cfg::invalid alg_cfg command!}\r\n");
        return OAL_FAIL;
    }

    /* ��¼�����Ӧ��ö��ֵ */
    pst_alg_param->en_alg_cfg = g_ast_alg_cfg_map[uc_map_index].en_alg_cfg;

    /* ��ȡ��������ֵ */
    ul_ret = wal_get_cmd_one_arg(pc_param + ul_off_set, ac_name, OAL_SIZEOF(ac_name), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_alg_cfg::wal_get_cmd_one_arg return err_code [%d]!}", ul_ret);
        return ul_ret;
    }

    /* ��¼��������ֵ */
    pst_alg_param->ul_value = (oal_uint32)oal_atoi(ac_name);

    /***************************************************************************
                             ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_ALG_PARAM, OAL_SIZEOF(mac_ioctl_alg_param_stru));

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_ioctl_alg_param_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        return (oal_uint32)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC oal_uint32 wal_hipriv_tpc_log(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    wal_msg_write_stru st_write_msg;
    oal_uint32 ul_off_set;
    oal_int8 ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    oal_uint32 ul_ret;
    oal_int32 l_ret;
    mac_ioctl_alg_tpc_log_param_stru *pst_alg_tpc_log_param = OAL_PTR_NULL;
    wal_ioctl_alg_cfg_stru st_alg_cfg;
    oal_uint8 uc_map_index = 0;
    oal_bool_enum_uint8 en_stop_flag = OAL_FALSE;

    pst_alg_tpc_log_param = (mac_ioctl_alg_tpc_log_param_stru *)(st_write_msg.auc_value);

    /* ��ȡ���ò������� */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, OAL_SIZEOF(ac_name), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_tpc_log::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    pc_param = pc_param + ul_off_set;

    /* Ѱ��ƥ������� */
    st_alg_cfg = g_ast_alg_cfg_map[0];
    while (st_alg_cfg.pc_name != OAL_PTR_NULL) {
        if (strcmp(st_alg_cfg.pc_name, ac_name) == 0) {
            break;
        }
        st_alg_cfg = g_ast_alg_cfg_map[++uc_map_index];
    }

    /* û���ҵ���Ӧ������򱨴� */
    if (st_alg_cfg.pc_name == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_hipriv_tpc_log::invalid alg_cfg command!}\r\n");
        return OAL_FAIL;
    }

    /* ��¼�����Ӧ��ö��ֵ */
    pst_alg_tpc_log_param->en_alg_cfg = g_ast_alg_cfg_map[uc_map_index].en_alg_cfg;

    /* ���ֻ�ȡ�ض�֡���ʺ�ͳ����־�����:��ȡ����ֻ���ȡ֡���� */
    if (pst_alg_tpc_log_param->en_alg_cfg == MAC_ALG_CFG_TPC_GET_FRAME_POW) {
        /* ��ȡ���ò������� */
        ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, OAL_SIZEOF(ac_name), &ul_off_set);
        if (ul_ret != OAL_SUCC) {
            OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_tpc_log::wal_get_cmd_one_arg return err_code [%d]!}", ul_ret);
            return ul_ret;
        }
        /* ��¼�����Ӧ��֡���� */
        pst_alg_tpc_log_param->pc_frame_name = ac_name;
        en_stop_flag = OAL_TRUE;
    } else {
        ul_ret = wal_hipriv_get_mac_addr(pc_param, pst_alg_tpc_log_param->auc_mac_addr, &ul_off_set);
        if (ul_ret != OAL_SUCC) {
            OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_hipriv_tpc_log::wal_hipriv_get_mac_addr failed!}\r\n");
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

        /* ��ȡҵ������ֵ */
        if (en_stop_flag != OAL_TRUE) {
            ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, OAL_SIZEOF(ac_name), &ul_off_set);
            if (ul_ret != OAL_SUCC) {
                OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_hipriv_tpc_log:wal_get_cmd_one_arg return err_code [%d]", ul_ret);
                return ul_ret;
            }

            pst_alg_tpc_log_param->uc_ac_no = (oal_uint8)oal_atoi(ac_name);
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
                /* ��ȡ��������ֵ */
                ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, OAL_SIZEOF(ac_name), &ul_off_set);
                if (ul_ret != OAL_SUCC) {
                    OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_tpc_log:wal_get_cmd_one_arg fail, ret[%d]!}", ul_ret);
                    return ul_ret;
                }

                /* ��¼��������ֵ */
                pst_alg_tpc_log_param->us_value = (oal_uint16)oal_atoi(ac_name);
            }
        }
    }

    /***************************************************************************
                             ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_ALG_PARAM, OAL_SIZEOF(mac_ioctl_alg_tpc_log_param_stru));

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_ioctl_alg_tpc_log_param_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        return (oal_uint32)l_ret;
    }

    return OAL_SUCC;
}

#if ((!defined(_PRE_PRODUCT_ID_HI110X_DEV)) && (!defined(_PRE_PRODUCT_ID_HI110X_HOST)))

OAL_STATIC oal_int32 wal_ioctl_set_auth_mode(
    oal_net_device_stru *pst_net_dev, oal_net_dev_ioctl_data_stru *pst_ioctl_data)
{
    mac_vap_stru *pst_mac_vap;
    oal_uint16 us_len = sizeof(oal_uint32);
    oal_uint32 ul_ret;

    pst_mac_vap = OAL_NET_DEV_PRIV(pst_net_dev);
    if (OAL_UNLIKELY(pst_mac_vap == OAL_PTR_NULL)) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_ioctl_set_auth_mode::OAL_NET_DEV_PRIV(pst_net_dev) is null ptr!}\r\n");
        return -OAL_EINVAL;
    }

    ul_ret = hmac_config_set_auth_mode(pst_mac_vap, us_len, (oal_uint8 *)&(pst_ioctl_data->pri_data.auth_params));
    if (OAL_UNLIKELY(ul_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_ioctl_set_auth_mode:return err code [%d]}", ul_ret);
        return -OAL_EFAIL;
    }

    return OAL_SUCC;
}


OAL_STATIC oal_int32 wal_ioctl_set_max_user(
    oal_net_device_stru *pst_net_dev, oal_net_dev_ioctl_data_stru *pst_ioctl_data)
{
    mac_vap_stru *pst_mac_vap;
    oal_uint16 us_len = sizeof(oal_uint32);
    oal_uint32 ul_ret;

    pst_mac_vap = OAL_NET_DEV_PRIV(pst_net_dev);
    if (OAL_UNLIKELY(pst_mac_vap == OAL_PTR_NULL)) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_ioctl_set_max_user::OAL_NET_DEV_PRIV(pst_net_dev) is null ptr!}\r\n");
        return -OAL_EINVAL;
    }

    ul_ret = hmac_config_set_max_user(pst_mac_vap, us_len, (pst_ioctl_data->pri_data.ul_vap_max_user));
    if (OAL_UNLIKELY(ul_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "wal_ioctl_set_max_user:set_max_user fail, ret[%d]",
                         ul_ret);
        return -OAL_EFAIL;
    }

    return OAL_SUCC;
}


oal_int32 wal_ioctl_set_ssid(oal_net_device_stru *pst_net_dev, oal_net_dev_ioctl_data_stru *pst_ioctl_data)
{
    mac_vap_stru *pst_mac_vap;
    oal_uint16 us_len;
    oal_uint32 ul_ret;
    mac_cfg_ssid_param_stru st_ssid_param;
    oal_uint8 uc_ssid_temp;

    pst_mac_vap = OAL_NET_DEV_PRIV(pst_net_dev);
    if (OAL_UNLIKELY(pst_mac_vap == OAL_PTR_NULL)) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_ioctl_set_ssid::OAL_NET_DEV_PRIV(pst_net_dev) is null ptr!}\r\n");
        return -OAL_EINVAL;
    }
    /* ���SSID����+3,��֤�����strlen������� */
    pst_ioctl_data->pri_data.ssid[OAL_IEEE80211_MAX_SSID_LEN + 3] = '\0';
    uc_ssid_temp = (oal_uint8)OAL_STRLEN((oal_int8 *)pst_ioctl_data->pri_data.ssid);
    us_len = OAL_MIN(uc_ssid_temp, OAL_IEEE80211_MAX_SSID_LEN);

    st_ssid_param.uc_ssid_len = (oal_uint8)us_len;
    if (memcpy_s(st_ssid_param.ac_ssid, WLAN_SSID_MAX_LEN, (const oal_void *)(pst_ioctl_data->pri_data.ssid),
                 (oal_uint32)us_len) != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_ioctl_set_ssid::memcpy fail!}");
        return -OAL_EFAIL;
    }
    ul_ret = hmac_config_set_ssid(pst_mac_vap, OAL_SIZEOF(st_ssid_param), (oal_uint8 *)&st_ssid_param);
    if (OAL_UNLIKELY(ul_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "wal_ioctl_set_ssid::hmac_config_set_ssid fail, ret[%d]",
                         ul_ret);
        return -OAL_EFAIL;
    }

    return OAL_SUCC;
}


oal_int32 wal_ioctl_set_country_code(oal_net_device_stru *pst_net_dev, oal_net_dev_ioctl_data_stru *pst_ioctl_data)
{
#ifdef _PRE_WLAN_FEATURE_11D
    oal_int8 auc_country_code[4] = { 0 };
    oal_int32 l_ret;

    if (memcpy_s(auc_country_code, OAL_SIZEOF(auc_country_code),
        pst_ioctl_data->pri_data.country_code.auc_country_code, 3) != EOK) { /* ����3���ַ���auc_country_code */
        OAM_ERROR_LOG0(0, OAM_SF_FTM, "{wal_ioctl_set_country_code::memcpy fail!}");
        return -OAL_EFAIL;
    }
    l_ret = wal_regdomain_update(pst_net_dev, auc_country_code);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_ioctl_set_country_code::return err code [%d]!}\r\n", l_ret);
        return -OAL_EFAIL;
    }

#else
    OAM_INFO_LOG0(0, OAM_SF_ANY, "{wal_ioctl_set_country_code::_PRE_WLAN_FEATURE_11D is not define!}\r\n");
#endif

    return OAL_SUCC;
}

oal_void wal_ioctl_set_crypto_params(
    mac_cfg80211_connect_param_stru *pst_mac_cfg80211_connect_param, oal_net_dev_ioctl_data_stru *pst_ioctl_data)
{
    oal_uint8 uc_loop;
    oal_uint8 uc_pairwise_cipher_num;
    oal_uint8 uc_akm_suite_num;

    /* ����WPA/WPA2 ������Ϣ */
    pst_mac_cfg80211_connect_param->st_crypto.wpa_versions =
        (oal_uint8)pst_ioctl_data->pri_data.cfg80211_connect_params.st_crypto.wpa_versions;
    pst_mac_cfg80211_connect_param->st_crypto.cipher_group =
        (oal_uint8)pst_ioctl_data->pri_data.cfg80211_connect_params.st_crypto.cipher_group;
    pst_mac_cfg80211_connect_param->st_crypto.n_ciphers_pairwise =
        (oal_uint8)pst_ioctl_data->pri_data.cfg80211_connect_params.st_crypto.n_ciphers_pairwise;
    pst_mac_cfg80211_connect_param->st_crypto.n_akm_suites =
        (oal_uint8)pst_ioctl_data->pri_data.cfg80211_connect_params.st_crypto.n_akm_suites;
    pst_mac_cfg80211_connect_param->st_crypto.control_port =
        (oal_uint8)pst_ioctl_data->pri_data.cfg80211_connect_params.st_crypto.control_port;
    uc_pairwise_cipher_num = pst_mac_cfg80211_connect_param->st_crypto.n_ciphers_pairwise;
    for (uc_loop = 0; uc_loop < uc_pairwise_cipher_num; uc_loop++) {
        pst_mac_cfg80211_connect_param->st_crypto.ciphers_pairwise[uc_loop] =
            (oal_uint8)pst_ioctl_data->pri_data.cfg80211_connect_params.st_crypto.ciphers_pairwise[uc_loop];
    }
    uc_akm_suite_num = pst_mac_cfg80211_connect_param->st_crypto.n_akm_suites;
    for (uc_loop = 0; uc_loop < uc_akm_suite_num; uc_loop++) {
        pst_mac_cfg80211_connect_param->st_crypto.akm_suites[uc_loop] =
            (oal_uint8)pst_ioctl_data->pri_data.cfg80211_connect_params.st_crypto.akm_suites[uc_loop];
    }
}


oal_int32 wal_ioctl_nl80211_priv_connect(oal_net_device_stru *pst_net_dev, oal_net_dev_ioctl_data_stru *pst_ioctl_data)
{
    mac_cfg80211_connect_param_stru st_mac_cfg80211_connect_param;
    oal_int32 l_ret;

    /* ��ʼ���������Ӳ��� */
    memset_s(&st_mac_cfg80211_connect_param, OAL_SIZEOF(mac_cfg80211_connect_param_stru),
             0, OAL_SIZEOF(mac_cfg80211_connect_param_stru));
    /* �����ں��·��� freq to channel_number eg.1,2,36,40... */
    st_mac_cfg80211_connect_param.uc_channel =
        (oal_uint8)oal_ieee80211_frequency_to_channel(pst_ioctl_data->pri_data.cfg80211_connect_params.l_freq);
    /* �����ں��·��� ssid */
    st_mac_cfg80211_connect_param.uc_ssid_len =
        OAL_MIN((oal_uint8)pst_ioctl_data->pri_data.cfg80211_connect_params.ssid_len, WLAN_SSID_MAX_LEN);
    if (memcpy_s(st_mac_cfg80211_connect_param.auc_ssid, WLAN_SSID_MAX_LEN,
        (oal_uint8 *)pst_ioctl_data->pri_data.cfg80211_connect_params.puc_ssid,
        st_mac_cfg80211_connect_param.uc_ssid_len) != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_FTM, "{wal_ioctl_nl80211_priv_connect::memcpy fail!}");
    }

    /* �����ں��·��� bssid */
    oal_set_mac_addr(st_mac_cfg80211_connect_param.auc_bssid,
                     (oal_uint8 *)pst_ioctl_data->pri_data.cfg80211_connect_params.puc_bssid);
    /* �����ں��·��İ�ȫ��ز��� */
    /* ��֤���� */
    st_mac_cfg80211_connect_param.en_auth_type = pst_ioctl_data->pri_data.cfg80211_connect_params.en_auth_type;
    /* �������� */
    st_mac_cfg80211_connect_param.en_privacy = pst_ioctl_data->pri_data.cfg80211_connect_params.en_privacy;
    /* IE�·� */
    st_mac_cfg80211_connect_param.puc_ie = pst_ioctl_data->pri_data.cfg80211_connect_params.puc_ie;
    st_mac_cfg80211_connect_param.ul_ie_len = (oal_uint32)pst_ioctl_data->pri_data.cfg80211_connect_params.ie_len;
    /* ���ü��ܲ��� */
    if (pst_ioctl_data->pri_data.cfg80211_connect_params.en_privacy != 0) {
        if ((pst_ioctl_data->pri_data.cfg80211_connect_params.uc_wep_key_len != 0) &&
            (pst_ioctl_data->pri_data.cfg80211_connect_params.st_crypto.n_akm_suites == 0)) {
            /* ����wep������Ϣ */
            st_mac_cfg80211_connect_param.puc_wep_key = pst_ioctl_data->pri_data.cfg80211_connect_params.puc_wep_key;
            st_mac_cfg80211_connect_param.uc_wep_key_len =
                pst_ioctl_data->pri_data.cfg80211_connect_params.uc_wep_key_len;
            st_mac_cfg80211_connect_param.uc_wep_key_index =
                pst_ioctl_data->pri_data.cfg80211_connect_params.uc_wep_key_index;
            st_mac_cfg80211_connect_param.st_crypto.cipher_group =
                (oal_uint8)pst_ioctl_data->pri_data.cfg80211_connect_params.st_crypto.cipher_group;
        } else if (pst_ioctl_data->pri_data.cfg80211_connect_params.st_crypto.n_akm_suites != 0) {
            wal_ioctl_set_crypto_params(&st_mac_cfg80211_connect_param, pst_ioctl_data);
        } else if (mac_find_vendor_ie(MAC_WLAN_OUI_MICROSOFT, MAC_WLAN_OUI_TYPE_MICROSOFT_WPS,
            st_mac_cfg80211_connect_param.puc_ie, (oal_int32)(st_mac_cfg80211_connect_param.ul_ie_len))) {
            OAM_WARNING_LOG0(0, OAM_SF_WPS, "wal_ioctl_nl80211_priv_connect::WPS ie is included in puc_ie! \n");
        } else {
            OAM_ERROR_LOG3(0, OAM_SF_ANY, "{set_key fail!wep_key_len[%d] akm_suites[%d]} puc_wep_key[0x%x]\r\n",
                           pst_ioctl_data->pri_data.cfg80211_connect_params.uc_wep_key_len,
                           pst_ioctl_data->pri_data.cfg80211_connect_params.st_crypto.n_akm_suites,
                           pst_ioctl_data->pri_data.cfg80211_connect_params.puc_wep_key);
            return -OAL_EFAIL;
        }
    }
    /* ���¼����������������� */
    l_ret = wal_cfg80211_start_connect(pst_net_dev, &st_mac_cfg80211_connect_param);
    if (l_ret != OAL_SUCC) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_ioctl_nl80211_priv_connect::wal_cfg80211_start_connect fail!}\r\n");
        return -OAL_EFAIL;
    }
    return OAL_SUCC;
}


oal_int32 wal_ioctl_nl80211_priv_disconnect(
    oal_net_device_stru *pst_net_dev, oal_net_dev_ioctl_data_stru *pst_ioctl_data)
{
    mac_cfg_kick_user_param_stru st_mac_cfg_kick_user_param;
    oal_int32 l_ret;
    mac_user_stru *pst_mac_user = OAL_PTR_NULL;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;

    /* �����ں��·���connect���� */
    memset_s(&st_mac_cfg_kick_user_param, OAL_SIZEOF(mac_cfg_kick_user_param_stru),
             0, OAL_SIZEOF(mac_cfg_kick_user_param_stru));

    /* �����ں��·���ȥ����ԭ�� */
    st_mac_cfg_kick_user_param.us_reason_code = pst_ioctl_data->pri_data.kick_user_params.us_reason_code;

    /* ��д��sta������ap mac ��ַ */
    pst_mac_vap = OAL_NET_DEV_PRIV(pst_net_dev);
    if (OAL_UNLIKELY(pst_mac_vap == OAL_PTR_NULL)) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "wal_ioctl_nl80211_priv_disconnect::OAL_NET_DEV_PRIV(pst_net_dev) is null");
        return -OAL_EINVAL;
    }
    pst_mac_user = mac_res_get_mac_user(pst_mac_vap->uc_assoc_vap_id);
    if (pst_mac_user == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "wal_ioctl_nl80211_priv_disconnect:pst_mac_user is nul");
        return OAL_SUCC;
    }

    memcpy_s(st_mac_cfg_kick_user_param.auc_mac_addr, WLAN_MAC_ADDR_LEN,
             pst_mac_user->auc_user_mac_addr, WLAN_MAC_ADDR_LEN);

    l_ret = wal_cfg80211_start_disconnect(pst_net_dev, &st_mac_cfg_kick_user_param);
    if (l_ret != OAL_SUCC) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_ioctl_nl80211_priv_disconnect:: \
            wal_cfg80211_start_disconnect fail!}");
        return -OAL_EFAIL;
    }

    return OAL_SUCC;
}

oal_int32 wal_ioctl_set_channel(oal_net_device_stru *pst_net_dev, oal_net_dev_ioctl_data_stru *pst_ioctl_data)
{
    mac_cfg_channel_param_stru *pst_channel_param = OAL_PTR_NULL;
    wal_msg_write_stru st_write_msg;
    mac_device_stru *pst_device = OAL_PTR_NULL;
    mac_vap_stru *pst_mac_vap;
    oal_wiphy_stru *pst_wiphy = OAL_PTR_NULL;
    oal_ieee80211_channel_stru *pst_channel = OAL_PTR_NULL;
    wlan_channel_bandwidth_enum_uint8 en_bandwidth;
    oal_int32 l_freq;
    oal_int32 l_channel;
    oal_int32 l_sec_channel_offset;
    oal_int32 l_center_freq1;
    oal_int32 l_bandwidth;
    oal_int32 l_ret;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;
    oal_uint32 ul_err_code;

    l_freq = pst_ioctl_data->pri_data.freq.l_freq;
    l_channel = pst_ioctl_data->pri_data.freq.l_channel;
    l_sec_channel_offset = pst_ioctl_data->pri_data.freq.l_sec_channel_offset;
    l_center_freq1 = pst_ioctl_data->pri_data.freq.l_center_freq1;
    l_bandwidth = pst_ioctl_data->pri_data.freq.l_bandwidth;

    pst_mac_vap = OAL_NET_DEV_PRIV(pst_net_dev);
    if (OAL_UNLIKELY(pst_mac_vap == OAL_PTR_NULL)) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_ioctl_set_channel::OAL_NET_DEV_PRIV(pst_net_dev) is null ptr.}\r\n");
        return -OAL_EINVAL;
    }

    pst_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (pst_device == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_ioctl_set_channel::pst_device is null!}\r\n");
        return -OAL_EINVAL;
    }

    pst_wiphy = pst_device->pst_wiphy;
    pst_channel = oal_ieee80211_get_channel(pst_wiphy, l_freq);
    l_channel = pst_channel->hw_value;

    /* �ж��ŵ��ڲ��ڹ������� */
    l_ret = (oal_int32)mac_is_channel_num_valid(pst_channel->band, (oal_uint8)l_channel);
    if (l_ret != OAL_SUCC) {
        OAM_ERROR_LOG2(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_ioctl_set_channel::channel num is invalid. band, \
            ch num [%d] [%d]!}", pst_channel->band, l_channel);
        return -OAL_EINVAL;
    }

    /* �����ں˴���ֵ��WITP ����ֵת�� */
    if (l_bandwidth == 80) { /* ����Ϊ80MHz */
        en_bandwidth =
            /* ����Ϊ80MHz������Ƶ�� (l_center_freq1 - 5000) / 5 */
            mac_get_bandwith_from_center_freq_seg0 ((oal_uint8)l_channel, (oal_uint8)((l_center_freq1 - 5000) / 5));
    } else if (l_bandwidth == 40) { /* ����Ϊ40MHz */
        switch (l_sec_channel_offset) {
            case -1: /* �¼�-1��ʾ��20�ŵ�-1 */
                en_bandwidth = WLAN_BAND_WIDTH_40MINUS;
                break;
            case 1: /* �¼�1��ʾ��20�ŵ�+1 */
                en_bandwidth = WLAN_BAND_WIDTH_40PLUS;
                break;
            default:
                en_bandwidth = WLAN_BAND_WIDTH_20M;
                break;
        }
    } else {
        en_bandwidth = WLAN_BAND_WIDTH_20M;
    }

    /***************************************************************************
        ���¼���wal�㴦��
    ***************************************************************************/
    /* ��д��Ϣ */
    pst_channel_param = (mac_cfg_channel_param_stru *)(st_write_msg.auc_value);
    pst_channel_param->uc_channel = (oal_uint8)pst_channel->hw_value;
    pst_channel_param->en_band = pst_channel->band;
    pst_channel_param->en_bandwidth = en_bandwidth;

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_CFG80211_SET_CHANNEL, OAL_SIZEOF(mac_cfg_channel_param_stru));

    /* ������Ϣ */
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_channel_param_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_TRUE,
                               &pst_rsp_msg);
    if ((l_ret != OAL_SUCC) || (pst_rsp_msg == OAL_PTR_NULL)) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "{wal_ioctl_set_channel:: wal_alloc_cfg_event return err code %d!}\r\n", l_ret);
        return l_ret;
    }

    /* ��ȡ���صĴ����� */
    ul_err_code = wal_check_and_release_msg_resp(pst_rsp_msg);
    if (ul_err_code != OAL_SUCC) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "{wal_ioctl_set_channel::wal_check_and_release_msg_resp fail, err code:[%d]!}",
                       ul_err_code);
        return -OAL_EFAIL;
    }

    return OAL_SUCC;
}
#endif


oal_uint32 wal_ioctl_parse_wps_p2p_ie(oal_app_ie_stru *pst_app_ie, oal_uint8 *puc_src, oal_uint32 ul_src_len)
{
    oal_uint8 *puc_ie = OAL_PTR_NULL;
    oal_uint32 ul_ie_len;
    oal_uint8 *puc_buf_remain = OAL_PTR_NULL;
    oal_uint32 ul_len_remain;

    if ((pst_app_ie == OAL_PTR_NULL) || (puc_src == OAL_PTR_NULL)) {
        OAM_ERROR_LOG2(0, OAM_SF_CFG, "{wal_ioctl_parse_wps_p2p_ie::param is NULL, pst_app_ie=[%p], puc_src=[%p]!}",
                       (uintptr_t)pst_app_ie, (uintptr_t)puc_src);
        return OAL_ERR_CODE_PTR_NULL;
    }

    if ((ul_src_len == 0) || (ul_src_len > WLAN_WPS_IE_MAX_SIZE)) {
        OAM_WARNING_LOG1(0, OAM_SF_CFG, "{wal_ioctl_parse_wps_p2p_ie::ul_src_len=[%d] is invailid!}\r\n", ul_src_len);
        return OAL_FAIL;
    }

    pst_app_ie->ul_ie_len = 0;
    puc_buf_remain = puc_src;
    ul_len_remain = ul_src_len;

    while (ul_len_remain > MAC_IE_HDR_LEN) {
        /* MAC_EID_WPS,MAC_EID_P2P ID��Ϊ221 */
        puc_ie = mac_find_ie(MAC_EID_P2P, puc_buf_remain, (oal_int32)ul_len_remain);
        if (puc_ie != OAL_PTR_NULL) {
            ul_ie_len = (oal_uint8)puc_ie[1] + MAC_IE_HDR_LEN;
            if ((ul_ie_len > (WLAN_WPS_IE_MAX_SIZE - pst_app_ie->ul_ie_len)) ||
                (ul_src_len < ((oal_uint32)(puc_ie - puc_src) + ul_ie_len))) {
                OAM_WARNING_LOG3(0, OAM_SF_CFG, "{wal_ioctl_parse_wps_p2p_ie::uc_ie_len=[%d], left buffer sieze=[%d], \
                    src_end_len=[%d], param invalid!}", ul_ie_len, WLAN_WPS_IE_MAX_SIZE - pst_app_ie->ul_ie_len,
                                 (oal_uint32)(puc_ie - puc_src) + ul_ie_len);
                return OAL_FAIL;
            }
            if (memcpy_s(&(pst_app_ie->auc_ie[pst_app_ie->ul_ie_len]),
                WLAN_WPS_IE_MAX_SIZE - pst_app_ie->ul_ie_len, puc_ie, ul_ie_len) != EOK) {
                OAM_ERROR_LOG0(0, OAM_SF_CFG, "wal_ioctl_parse_wps_p2p_ie::memcpy fail!");
                return OAL_FAIL;
            }
            pst_app_ie->ul_ie_len += ul_ie_len;
            puc_buf_remain = puc_ie + ul_ie_len;
            ul_len_remain = ul_src_len - (oal_uint32)(puc_buf_remain - puc_src);
        } else {
            break;
        }
    }

    if (pst_app_ie->ul_ie_len > 0) {
        return OAL_SUCC;
    }

    return OAL_FAIL;
}


oal_int32 wal_ioctl_set_wps_p2p_ie(oal_net_device_stru *pst_net_dev,
                                   oal_uint8 *puc_buf,
                                   oal_uint32 ul_len,
                                   en_app_ie_type_uint8 en_type)
{
    wal_msg_write_stru st_write_msg;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;
    oal_uint32 ul_err_code;
    oal_app_ie_stru st_wps_p2p_ie;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    oal_w2h_app_ie_stru *pst_w2h_wps_p2p_ie = OAL_PTR_NULL;
    oal_int32 l_ret;

    if (ul_len > WLAN_WPS_IE_MAX_SIZE) {
        OAM_ERROR_LOG1(0, OAM_SF_CFG, "{wal_ioctl_set_wps_p2p_ie:: wrong ul_len: [%u]!}\r\n",
                       ul_len);
        return -OAL_EFAIL;
    }

    pst_mac_vap = OAL_NET_DEV_PRIV(pst_net_dev);
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_CFG, "{wal_ioctl_set_wps_p2p_ie::pst_mac_vap is null}");
        return -OAL_EINVAL;
    }

    memset_s(&st_wps_p2p_ie, OAL_SIZEOF(st_wps_p2p_ie), 0, OAL_SIZEOF(st_wps_p2p_ie));
    switch (en_type) {
        case OAL_APP_BEACON_IE:
        case OAL_APP_PROBE_RSP_IE:
        case OAL_APP_ASSOC_RSP_IE:
            st_wps_p2p_ie.en_app_ie_type = en_type;
            break;
        default:
            OAM_ERROR_LOG1(0, OAM_SF_CFG, "{wal_ioctl_set_wps_p2p_ie:: wrong type: [%x]!}\r\n",
                           en_type);
            return -OAL_EFAIL;
    }

    if (IS_LEGACY_VAP(pst_mac_vap) == OAL_FALSE) {
        if (wal_ioctl_parse_wps_p2p_ie(&st_wps_p2p_ie, puc_buf, ul_len) != OAL_SUCC) {
            OAM_WARNING_LOG1(0, OAM_SF_CFG, "{wal_ioctl_set_wps_p2p_ie::Type=[%d], parse p2p ie fail, !}\r\n", en_type);
            return -OAL_EFAIL;
        }
    } else {
        if (memcpy_s(st_wps_p2p_ie.auc_ie, WLAN_WPS_IE_MAX_SIZE, puc_buf, ul_len) != EOK) {
            OAM_ERROR_LOG0(0, OAM_SF_CFG, "wal_ioctl_set_wps_p2p_ie::memcpy fail!");
            return -OAL_EFAIL;
        }
        st_wps_p2p_ie.ul_ie_len = ul_len;
    }

    /***************************************************************************
        ���¼���wal�㴦��
    ***************************************************************************/
    pst_w2h_wps_p2p_ie = (oal_w2h_app_ie_stru *)st_write_msg.auc_value;
    pst_w2h_wps_p2p_ie->en_app_ie_type = st_wps_p2p_ie.en_app_ie_type;
    pst_w2h_wps_p2p_ie->ul_ie_len = st_wps_p2p_ie.ul_ie_len;
    pst_w2h_wps_p2p_ie->puc_data_ie = st_wps_p2p_ie.auc_ie;

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_WPS_P2P_IE, OAL_SIZEOF(oal_w2h_app_ie_stru));

    /* ������Ϣ */
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(oal_w2h_app_ie_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_TRUE,
                               &pst_rsp_msg);
    if ((l_ret != OAL_SUCC) || (pst_rsp_msg == OAL_PTR_NULL)) {
        OAM_ERROR_LOG1(0, OAM_SF_P2P, "{wal_ioctl_set_wps_p2p_ie:: wal_alloc_cfg_event return err code %d!}", l_ret);
        return l_ret;
    }

    /* ��ȡ���صĴ����� */
    ul_err_code = wal_check_and_release_msg_resp(pst_rsp_msg);
    if (ul_err_code != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_P2P, "{wal_ioctl_set_wps_p2p_ie::wal_check_and_release_msg_resp fail, err code:[%d]",
                         ul_err_code);
        return -OAL_EFAIL;
    }

    return OAL_SUCC;
}
#ifdef _PRE_WLAN_FEATURE_P2P

OAL_STATIC oal_int32 wal_ioctl_set_p2p_miracast_status(oal_net_device_stru *pst_net_dev, oal_uint8 uc_param)
{
    wal_msg_write_stru st_write_msg;
    oal_int32 l_ret;

    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_P2P_MIRACAST_STATUS, OAL_SIZEOF(oal_uint8));
    st_write_msg.auc_value[0] = uc_param;

    /* ������Ϣ */
    l_ret = wal_send_cfg_event(pst_net_dev, WAL_MSG_TYPE_WRITE, WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(oal_uint8),
                               (oal_uint8 *)&st_write_msg, OAL_FALSE, OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_CFG, "{wal_ioctl_set_p2p_miracast_status::return err code [%d]!}\r\n", l_ret);
        return l_ret;
    }

    OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_ioctl_set_p2p_miracast_status::value[%d].}", st_write_msg.auc_value[0]);
    return OAL_SUCC;
}


OAL_STATIC oal_int32 wal_ioctl_set_p2p_noa(
    oal_net_device_stru *pst_net_dev, mac_cfg_p2p_noa_param_stru *pst_p2p_noa_param)
{
    wal_msg_write_stru st_write_msg;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;
    oal_uint32 ul_err_code;
    oal_int32 l_ret;

    if (memcpy_s(st_write_msg.auc_value, OAL_SIZEOF(mac_cfg_p2p_noa_param_stru),
                 pst_p2p_noa_param, OAL_SIZEOF(mac_cfg_p2p_noa_param_stru)) != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_P2P, "wal_ioctl_set_p2p_noa::memcpy fail!");
        return -OAL_EFAIL;
    }

    /***************************************************************************
        ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_P2P_PS_NOA, OAL_SIZEOF(mac_cfg_p2p_noa_param_stru));

    /* ������Ϣ */
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_p2p_noa_param_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_TRUE,
                               &pst_rsp_msg);
    if ((l_ret != OAL_SUCC) || (pst_rsp_msg == OAL_PTR_NULL)) {
        OAM_ERROR_LOG1(0, OAM_SF_P2P, "{wal_ioctl_set_p2p_noa:: wal_alloc_cfg_event return err code %d!}\r\n", l_ret);
        return l_ret;
    }

    /* ��ȡ���صĴ����� */
    ul_err_code = wal_check_and_release_msg_resp(pst_rsp_msg);
    if (ul_err_code != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_P2P, "wal_ioctl_set_p2p_noa:wal_check_and_release_msg_resp fail, err code:[%d]!",
                         ul_err_code);
        return -OAL_EFAIL;
    }

    return OAL_SUCC;
}


OAL_STATIC oal_int32 wal_ioctl_set_p2p_ops(
    oal_net_device_stru *pst_net_dev, mac_cfg_p2p_ops_param_stru *pst_p2p_ops_param)
{
    wal_msg_write_stru st_write_msg;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;
    oal_uint32 ul_err_code;
    oal_int32 l_ret;

    if (memcpy_s(st_write_msg.auc_value, OAL_SIZEOF(mac_cfg_p2p_ops_param_stru),
                 pst_p2p_ops_param, OAL_SIZEOF(mac_cfg_p2p_ops_param_stru)) != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_P2P, "wal_ioctl_set_p2p_ops::memcpy fail!");
        return -OAL_EFAIL;
    }

    /***************************************************************************
        ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_P2P_PS_OPS, OAL_SIZEOF(mac_cfg_p2p_ops_param_stru));

    /* ������Ϣ */
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_p2p_ops_param_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_TRUE,
                               &pst_rsp_msg);
    if ((l_ret != OAL_SUCC) || (pst_rsp_msg == OAL_PTR_NULL)) {
        OAM_ERROR_LOG1(0, OAM_SF_P2P, "{wal_ioctl_set_p2p_ops:: wal_alloc_cfg_event return err code %d!}\r\n", l_ret);
        return l_ret;
    }

    /* ��ȡ���صĴ����� */
    ul_err_code = wal_check_and_release_msg_resp(pst_rsp_msg);
    if (ul_err_code != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_P2P, "{wal_ioctl_set_p2p_ops::wal_check_and_release_msg_resp fail, err code:[%d]!}",
                         ul_err_code);
        return -OAL_EFAIL;
    }

    return OAL_SUCC;
}
#endif

#ifdef _PRE_WLAN_FEATURE_STA_PM
OAL_STATIC oal_int32 wal_ioctl_set_sta_pm(oal_net_device_stru *pst_net_dev, mac_pm_switch_enum_uint8 en_pm_enable,
                                          mac_pm_ctrl_type_enum_uint8 en_pm_ctrl_type)
{
    wal_msg_write_stru st_write_msg;
    mac_cfg_ps_open_stru *pst_sta_pm_open;
    oal_int32 l_ret;

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_STA_PM_ON, OAL_SIZEOF(mac_cfg_ps_open_stru));

    /* ��������������� */
    pst_sta_pm_open = (mac_cfg_ps_open_stru *)(st_write_msg.auc_value);
    /* MAC_STA_PM_SWITCH_ON / MAC_STA_PM_SWITCH_OFF */
    pst_sta_pm_open->uc_pm_enable = en_pm_enable;
    pst_sta_pm_open->uc_pm_ctrl_type = en_pm_ctrl_type;

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_ps_open_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_ioctl_set_sta_pm::CMD_SET_STA_PM_ON return err code [%d]!}\r\n", l_ret);
        return -OAL_EFAIL;
    }
    return OAL_SUCC;
}
#endif
#ifdef _PRE_WLAN_FEATURE_HS20

OAL_STATIC oal_int32 wal_ioctl_set_qos_map(
    oal_net_device_stru *pst_net_dev, hmac_cfg_qos_map_param_stru *pst_qos_map_param)
{
    wal_msg_write_stru st_write_msg;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;
    oal_uint32 ul_err_code;
    oal_int32 l_ret;

    if (memcpy_s(st_write_msg.auc_value, OAL_SIZEOF(hmac_cfg_qos_map_param_stru),
                 pst_qos_map_param, OAL_SIZEOF(hmac_cfg_qos_map_param_stru)) != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_HS20, "wal_ioctl_set_qos_map::memcpy fail!");
        return -OAL_EFAIL;
    }

    /***************************************************************************
        ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_QOS_MAP, OAL_SIZEOF(hmac_cfg_qos_map_param_stru));

    /* ������Ϣ */
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(hmac_cfg_qos_map_param_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_TRUE,
                               &pst_rsp_msg);
    if ((l_ret != OAL_SUCC) || (pst_rsp_msg == OAL_PTR_NULL)) {
        OAM_ERROR_LOG1(0, OAM_SF_HS20, "{wal_ioctl_set_qos_map:: wal_alloc_cfg_event return err code %d!}\r\n", l_ret);
        return l_ret;
    }

    /* ��ȡ���صĴ����� */
    ul_err_code = wal_check_and_release_msg_resp(pst_rsp_msg);
    if (ul_err_code != OAL_SUCC) {
        OAM_WARNING_LOG2(0, OAM_SF_HS20, "{wal_ioctl_set_qos_map::wal_check_and_release_msg_resp fail, \
            err code:[%x] [%x]!}", l_ret, ul_err_code);
        return -OAL_EFAIL;
    }
    return OAL_SUCC;
}
#endif  // _PRE_WLAN_FEATURE_HS20

#if ((!defined(_PRE_PRODUCT_ID_HI110X_DEV)) && (!defined(_PRE_PRODUCT_ID_HI110X_HOST)))

oal_int32 wal_ioctl_set_wps_ie(oal_net_device_stru *pst_net_dev, oal_net_dev_ioctl_data_stru *pst_ioctl_data)
{
    wal_msg_write_stru st_write_msg;
    oal_app_ie_stru st_wps_ie;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;
    oal_uint32 ul_err_code;
    oal_int32 l_ret;

    memset_s(&st_wps_ie, OAL_SIZEOF(oal_app_ie_stru), 0, OAL_SIZEOF(oal_app_ie_stru));
    st_wps_ie.ul_ie_len = pst_ioctl_data->pri_data.st_app_ie.ul_ie_len;
    st_wps_ie.en_app_ie_type = pst_ioctl_data->pri_data.st_app_ie.en_app_ie_type;
    l_ret = (oal_int32)oal_copy_from_user(st_wps_ie.auc_ie, pst_ioctl_data->pri_data.st_app_ie.auc_ie,
                                          st_wps_ie.ul_ie_len);
    /* copy_from_user������Ŀ���Ǵ��û��ռ俽�����ݵ��ں˿ռ䣬ʧ�ܷ���û�б��������ֽ������ɹ�����0 */
    if (l_ret != 0) {
        OAM_WARNING_LOG2(0, OAM_SF_CFG, "{wal_ioctl_set_wps_ie::copy app ie fail.ie_type[%d], ie_len[%d]}",
                         st_wps_ie.en_app_ie_type, st_wps_ie.ul_ie_len);
        return -OAL_EFAIL;
    }

    if (memcpy_s(st_write_msg.auc_value, OAL_SIZEOF(oal_app_ie_stru), &st_wps_ie, OAL_SIZEOF(oal_app_ie_stru)) !=
        EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_CFG, "wal_ioctl_set_wps_ie::memcpy fail!");
        return -OAL_EFAIL;
    }

    /***************************************************************************
        ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_WPS_IE, OAL_SIZEOF(oal_app_ie_stru));

    /* ������Ϣ */
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(oal_app_ie_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_TRUE,
                               &pst_rsp_msg);
    if ((l_ret != OAL_SUCC) || (pst_rsp_msg == OAL_PTR_NULL)) {
        OAM_ERROR_LOG1(0, OAM_SF_P2P, "{wal_ioctl_set_wps_ie:: wal_alloc_cfg_event return err code %d!}\r\n", l_ret);
        return l_ret;
    }

    /* ��ȡ���صĴ����� */
    ul_err_code = wal_check_and_release_msg_resp(pst_rsp_msg);
    if (ul_err_code != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_P2P, "{wal_ioctl_set_wps_ie::wal_check_and_release_msg_resp fail, err code:[%x]",
                         ul_err_code);
        return -OAL_EFAIL;
    }

    return OAL_SUCC;
}


oal_int32 wal_ioctl_set_frag(oal_net_device_stru *pst_net_dev, oal_net_dev_ioctl_data_stru *pst_ioctl_data)
{
    wal_msg_write_stru st_write_msg;
    mac_cfg_frag_threshold_stru *pst_threshold;
    oal_uint32 ul_threshold;
    oal_uint16 us_len;
    oal_int32 l_ret;

    /* ��ȡ��Ƭ���� */
    ul_threshold = (oal_uint32)pst_ioctl_data->pri_data.l_frag;

    pst_threshold = (mac_cfg_frag_threshold_stru *)(st_write_msg.auc_value);
    pst_threshold->ul_frag_threshold = ul_threshold;

    /***************************************************************************
                              ���¼���wal�㴦��
    ***************************************************************************/
    us_len = OAL_SIZEOF(mac_cfg_frag_threshold_stru);
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_FRAG_THRESHOLD_REG, us_len);

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + us_len,
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_ioctl_set_frag::return err code [%d]!}\r\n", l_ret);
        return l_ret;
    }

    return OAL_SUCC;
}


oal_int32 wal_ioctl_set_rts(oal_net_device_stru *pst_net_dev, oal_net_dev_ioctl_data_stru *pst_ioctl_data)
{
    wal_msg_write_stru st_write_msg;
    mac_cfg_rts_threshold_stru *pst_threshold;
    oal_uint32 ul_threshold;
    oal_uint16 us_len;
    oal_int32 l_ret;

    /* ��ȡ��Ƭ���� */
    ul_threshold = (oal_uint32)pst_ioctl_data->pri_data.l_rts;

    pst_threshold = (mac_cfg_rts_threshold_stru *)(st_write_msg.auc_value);
    pst_threshold->ul_rts_threshold = ul_threshold;

    OAM_INFO_LOG1(0, OAM_SF_CFG, "{wal_ioctl_set_rts::rts [%d]!}\r\n", ul_threshold);
    /***************************************************************************
                              ���¼���wal�㴦��
    ***************************************************************************/
    us_len = OAL_SIZEOF(mac_cfg_rts_threshold_stru);
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_RTS_THRESHHOLD, us_len);

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + us_len,
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_CFG, "{wal_ioctl_set_rts::return err code [%d]!}\r\n", l_ret);
        return l_ret;
    }

    return OAL_SUCC;
}
#endif

oal_uint8 *wal_get_reduce_sar_ctrl_params(oal_uint8 uc_tx_power_lvl)
{
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
    wlan_customize_power_params_stru *pst_cust_nv_params = hwifi_get_nvram_params(); /* ����͹��ʶ��ƻ����� */
    if ((uc_tx_power_lvl <= CUS_NUM_OF_SAR_LVL) && (uc_tx_power_lvl > 0)) {
        uc_tx_power_lvl--;
    } else {
        return OAL_PTR_NULL;
    }
    if (memcpy_s(pst_cust_nv_params->auc_sar_ctrl_params, CUS_SAR_NUM, auc_sar_params[uc_tx_power_lvl],
        OAL_SIZEOF(oal_uint8) * CUS_SAR_NUM) != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "wal_get_reduce_sar_ctrl_params:memcpy fail");
    }
    return pst_cust_nv_params->auc_sar_ctrl_params;
#else
    return OAL_PTR_NULL;
#endif
}


OAL_STATIC oal_int32 wal_ioctl_reduce_sar(oal_net_device_stru *pst_net_dev, oal_uint16 us_tx_power)
{
    oal_int32 l_ret;
    wal_msg_write_stru st_write_msg;
    oal_uint8 *puc_sar_ctrl_params = OAL_PTR_NULL;
    oal_uint8 auc_sar_ctrl_params[CUS_SAR_NUM];
    oal_uint8 uc_lvl_idx = 0;

    OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_ioctl_reduce_sar:supplicant set tx_power[%d] for reduce SAR", us_tx_power);
    /***************************************************************************
        ����10XX�����ϲ��·��Ľ�SAR��λ��
        ��ǰ��λ������"����WiFi���߽�SAR sensor��
        �������ֵ�WiFi������WiFi��Modemһ����"��Ԥ����
        ����Ҫ1001~1020����
        ����        ��λ          ����0        ����1    ����2��RPC��   ����3(Ant�Ƿ�SAR sensor����)
                              �Ƿ����Ƶͬ��                           Ant1 Ant3
        Head SAR    ��λ1001        N           CE��    receiver on     NA  NA
                    ��λ1002        Y           CE��    receiver on     NA  NA
                    ��λ1003        N           FCC��   receiver on     NA  NA
                    ��λ1004        Y           FCC��   receiver on     NA  NA
        -------------------------------------------------------------------------
        Body SAR    ��λ1005        N           CE��    receiver off    0   0
                    ��λ1006        N           CE��    receiver off    0   1
                    ��λ1007        N           CE��    receiver off    1   0
                    ��λ1008        N           CE��    receiver off    1   1
                    ��λ1009        Y           CE��    receiver off    0   0
                    ��λ1010        Y           CE��    receiver off    0   1
                    ��λ1011        Y           CE��    receiver off    1   0
                    ��λ1012        Y           CE��    receiver off    1   1
                    ��λ1013        N           FCC��   receiver off    0   0
                    ��λ1014        N           FCC��   receiver off    0   1
                    ��λ1015        N           FCC��   receiver off    1   0
                    ��λ1016        N           FCC��   receiver off    1   1
                    ��λ1017        Y           FCC��   receiver off    0   0
                    ��λ1018        Y           FCC��   receiver off    0   1
                    ��λ1019        Y           FCC��   receiver off    1   0
                    ��λ1020        Y           FCC��   receiver off    1   1
    ***************************************************************************/
    if ((us_tx_power >= 1001) && (us_tx_power <= 1020)) { /* ��λ��1001-1020֮�� */
        uc_lvl_idx = us_tx_power - 1000; /* us_tx_power - 1000��ʾ���ƻ���SAR��λ�� */
    }
    puc_sar_ctrl_params = wal_get_reduce_sar_ctrl_params(uc_lvl_idx);
    if (puc_sar_ctrl_params == OAL_PTR_NULL) {
        memset_s(auc_sar_ctrl_params, OAL_SIZEOF(oal_uint8) * CUS_SAR_NUM,
                 0xFF, OAL_SIZEOF(oal_uint8) * CUS_SAR_NUM);
        puc_sar_ctrl_params = auc_sar_ctrl_params;
    }
    /* vapδ����ʱ��������supplicant���� */
    if (OAL_NET_DEV_PRIV(pst_net_dev) == NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_CFG, "wal_ioctl_reduce_sar::vap not created yet, ignore the cmd!");
        return -OAL_EINVAL;
    }
    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_REDUCE_SAR, OAL_SIZEOF(oal_uint8) * CUS_SAR_NUM);
    if (memcpy_s(st_write_msg.auc_value, OAL_SIZEOF(oal_uint8) * CUS_SAR_NUM,
                 puc_sar_ctrl_params, OAL_SIZEOF(oal_uint8) * CUS_SAR_NUM) != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_CFG, "wal_ioctl_reduce_sar::memcpy fail!");
        return -OAL_EINVAL;
    }

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(oal_uint8) * CUS_SAR_NUM,
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "wal_ioctl_reduce_sar::wal_send_cfg_event failed, error no[%d]!\r\n", l_ret);
        return l_ret;
    }
    return OAL_SUCC;
}

OAL_STATIC oal_uint32 wal_get_parameter_from_cmd(oal_int8 *pc_cmd, oal_int8 *pc_arg, OAL_CONST oal_int8 *puc_token,
                                                 oal_uint32 *pul_cmd_offset, oal_uint32 ul_param_max_len)
{
    oal_int8 *pc_cmd_copy = OAL_PTR_NULL;
    oal_int8 ac_cmd_copy[WAL_IOCTL_PRIV_SUBCMD_MAX_LEN];
    oal_uint32 ul_pos = 0;
    oal_uint32 ul_arg_len;

    if (OAL_UNLIKELY((pc_cmd == OAL_PTR_NULL) || (pc_arg == OAL_PTR_NULL) || (pul_cmd_offset == OAL_PTR_NULL))) {
        OAM_ERROR_LOG3(0, OAM_SF_ANY, "{wal_get_parameter_from_cmd::pc_cmd/pc_arg/pul_cmd_offset null ptr error \
            %x, %x, %x, %d!}\r\n", (uintptr_t)pc_cmd, (uintptr_t)pc_arg, (uintptr_t)pul_cmd_offset);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pc_cmd_copy = pc_cmd;

    /* ȥ���ַ�����ʼ�Ķ��� */
    while (*pc_cmd_copy == ',') {
        ++pc_cmd_copy;
    }
    /* ȡ�ö���ǰ���ַ��� */
    while ((*pc_cmd_copy != ',') && (*pc_cmd_copy != '\0')) {
        ac_cmd_copy[ul_pos] = *pc_cmd_copy;
        ++ul_pos;
        ++pc_cmd_copy;

        if (OAL_UNLIKELY(ul_pos >= ul_param_max_len)) {
            OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_get_parameter_from_cmd::ul_pos >= WAL_HIPRIV_CMD_NAME_MAX_LEN, \
                ul_pos %d!}\r\n", ul_pos);
            return OAL_ERR_CODE_ARRAY_OVERFLOW;
        }
    }
    ac_cmd_copy[ul_pos] = '\0';
    /* �ַ�������β�����ش����� */
    if (ul_pos == 0) {
        OAM_INFO_LOG0(0, OAM_SF_ANY, "{wal_get_parameter_from_cmd::return param pc_arg is null!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }
    *pul_cmd_offset = (oal_uint32)(pc_cmd_copy - pc_cmd);

    /* ����ַ����Ƿ����������ǰ�������ַ� */
    if (memcmp(ac_cmd_copy, puc_token, OAL_STRLEN(puc_token)) != 0) {
        return OAL_FAIL;
    } else {
        /* �۳�ǰ�������ַ����ش����� */
        ul_arg_len = OAL_STRLEN(ac_cmd_copy) - OAL_STRLEN(puc_token);
        if (memcpy_s(pc_arg, ul_param_max_len, ac_cmd_copy + OAL_STRLEN(puc_token), ul_arg_len) != EOK) {
            OAM_ERROR_LOG0(0, OAM_SF_ANY, "wal_get_parameter_from_cmd::memcpy fail!");
            return OAL_FAIL;
        }
        pc_arg[ul_arg_len] = '\0';
    }
    return OAL_SUCC;
}


OAL_STATIC oal_int32 wal_set_ap_max_user(oal_net_device_stru *pst_net_dev, oal_uint32 ul_ap_max_user)
{
    wal_msg_write_stru st_write_msg;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;
    oal_uint32 ul_err_code;
    oal_int32 l_ret;

    OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_set_ap_max_user:: ap_max_user is : %u.}\r\n", ul_ap_max_user);

    if (ul_ap_max_user == 0) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_set_ap_max_user:invalid ap max user(%u), ignore this set", ul_ap_max_user);
        return OAL_SUCC;
    }

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_MAX_USER, OAL_SIZEOF(ul_ap_max_user));
    *((oal_uint32 *)st_write_msg.auc_value) = ul_ap_max_user;
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(ul_ap_max_user),
                               (oal_uint8 *)&st_write_msg,
                               OAL_TRUE,
                               &pst_rsp_msg);
    if ((l_ret != OAL_SUCC) || (pst_rsp_msg == OAL_PTR_NULL)) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "{wal_set_ap_max_user:: wal_send_cfg_event return err code %d!}\r\n", l_ret);

        return l_ret;
    }

    /* ��ȡ���صĴ����� */
    ul_err_code = wal_check_and_release_msg_resp(pst_rsp_msg);
    if (ul_err_code != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_CFG, "wal_set_ap_max_user:wal_check_and_release_msg_resp fail, err code:[%d]",
                         ul_err_code);
        return -OAL_EFAIL;
    }
    return l_ret;
}


OAL_STATIC oal_void wal_config_mac_filter(oal_net_device_stru *pst_net_dev, oal_int8 *pc_command)
{
    oal_int8 ac_parsed_command[WAL_IOCTL_PRIV_SUBCMD_MAX_LEN];
    oal_int8 *pc_parse_command = OAL_PTR_NULL;
    oal_uint32 ul_mac_mode;
    oal_uint32 ul_mac_cnt;
    oal_uint32 ul_i;
#ifdef _PRE_WLAN_FEATURE_CUSTOM_SECURITY
    wal_msg_write_stru st_write_msg;
    oal_uint16 us_len;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;
    oal_uint32 ul_err_code;
    oal_int32 l_ret = 0;
#endif
    oal_uint32 ul_ret;
    oal_uint32 ul_off_set;

    if (pc_command == OAL_PTR_NULL) {
        return;
    }
    pc_parse_command = pc_command;

    /* ����MAC_MODE */
    ul_ret = wal_get_parameter_from_cmd(pc_parse_command, ac_parsed_command, "MAC_MODE=", &ul_off_set,
                                        WAL_IOCTL_PRIV_SUBCMD_MAX_LEN);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_config_mac_filter:wal_get_parameter_from_cmd return err_code %u", ul_ret);
        return;
    }
    /* �������Ƿ�Ϸ� 0,1,2 */
    ul_mac_mode = (oal_uint32)oal_atoi(ac_parsed_command);
    if (ul_mac_mode > 2) { /* ����2�Ƿ�ֵ */
        OAM_WARNING_LOG4(0, OAM_SF_ANY, "{wal_config_mac_filter::invalid MAC_MODE[%c%c%c%c]!}\r\n",
                         (oal_uint8)ac_parsed_command[0],
                         (oal_uint8)ac_parsed_command[1],
                         (oal_uint8)ac_parsed_command[2], /* ac_parsed_command��2byte��Ϊ���������ӡ */
                         (oal_uint8)ac_parsed_command[3]); /* ac_parsed_command��3byte��Ϊ���������ӡ */
        return;
    }

    /* ���ù���ģʽ */
#ifdef _PRE_WLAN_FEATURE_CUSTOM_SECURITY
    ul_ret = wal_hipriv_send_cfg_uint32_data(pst_net_dev, ac_parsed_command, WLAN_CFGID_BLACKLIST_MODE);
    if (ul_ret != OAL_SUCC) {
        return;
    }
#endif
    /* ����MAC_CNT */
    pc_parse_command += ul_off_set;
    ul_ret = wal_get_parameter_from_cmd(pc_parse_command, ac_parsed_command, "MAC_CNT=", &ul_off_set,
                                        WAL_IOCTL_PRIV_SUBCMD_MAX_LEN);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_config_mac_filter:wal_get_parameter_from_cmd fai, ret[%u]", ul_ret);
        return;
    }
    ul_mac_cnt = (oal_uint32)oal_atoi(ac_parsed_command);

    for (ul_i = 0; ul_i < ul_mac_cnt; ul_i++) {
        pc_parse_command += ul_off_set;
        ul_ret = wal_get_parameter_from_cmd(pc_parse_command, ac_parsed_command, "MAC=", &ul_off_set,
                                            WAL_IOCTL_PRIV_SUBCMD_MAX_LEN);
        if (ul_ret != OAL_SUCC) {
            OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_config_mac_filter::wal_get_parameter_from_cmd return err_code [%u]",
                             ul_ret);
            return;
        }
        /* 5.1  �������Ƿ����MAC����(6 * 2) */
        if (OAL_STRLEN(ac_parsed_command) != WLAN_MAC_ADDR_LEN * 2) {
            OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_config_mac_filter::invalid MAC format}\r\n");
            return;
        }
        /* 6. ��ӹ����豸 */
#ifdef _PRE_WLAN_FEATURE_CUSTOM_SECURITY
        /***************************************************************************
                             ���¼���wal�㴦��
        ***************************************************************************/
        memset_s((oal_uint8 *)&st_write_msg, OAL_SIZEOF(st_write_msg), 0, OAL_SIZEOF(st_write_msg));
        oal_strtoaddr(ac_parsed_command, st_write_msg.auc_value); /* ���ַ� ac_name ת�������� mac_add[6] */
        us_len = OAL_MAC_ADDR_LEN;                                /* OAL_SIZEOF(oal_uint8); */

        if (ul_i == (ul_mac_cnt - 1)) {
            /* �����е�mac��ַ�������ɺ󣬲Ž��й����û�ȷ�ϣ��Ƿ���Ҫɾ�� */
            WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_ADD_BLACK_LIST, us_len);
        } else {
            WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_ADD_BLACK_LIST_ONLY, us_len);
        }

        /* 6.1  ������Ϣ */
        l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + us_len,
                                   (oal_uint8 *)&st_write_msg,
                                   OAL_TRUE,
                                   &pst_rsp_msg);
        if ((l_ret != OAL_SUCC) || (pst_rsp_msg == OAL_PTR_NULL)) {
            OAM_ERROR_LOG1(0, OAM_SF_ANY, "{wal_config_mac_filter:: wal_send_cfg_event return err code %d!}", l_ret);
            return;
        }

        /* 6.2  ��ȡ���صĴ����� */
        ul_err_code = wal_check_and_release_msg_resp(pst_rsp_msg);
        if (ul_err_code != OAL_SUCC) {
            OAM_WARNING_LOG1(0, OAM_SF_CFG, "wal_config_mac_filter:wal_check_and_release_msg_resp fail, err code:[%x]",
                             ul_err_code);
            return;
        }
#endif
    }

    /* ÿ���������mac��ַ���˺���մ��м���� */
    return;
}


OAL_STATIC oal_int32 wal_kick_sta(oal_net_device_stru *pst_net_dev, oal_uint8 *auc_mac_addr, oal_uint8 uc_mac_addr_len)
{
#ifdef _PRE_WLAN_FEATURE_CUSTOM_SECURITY
    wal_msg_write_stru st_write_msg;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;
    oal_uint32 ul_err_code;
    mac_cfg_kick_user_param_stru *pst_kick_user_param;
    oal_int32 l_ret;
#endif

    if ((auc_mac_addr == NULL) || (uc_mac_addr_len != WLAN_MAC_ADDR_LEN)) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_kick_sta::check para!\n");
        return -OAL_EFAIL;
    }

#ifdef _PRE_WLAN_FEATURE_CUSTOM_SECURITY

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_KICK_USER, OAL_SIZEOF(mac_cfg_kick_user_param_stru));

    pst_kick_user_param = (mac_cfg_kick_user_param_stru *)(st_write_msg.auc_value);
    oal_set_mac_addr(pst_kick_user_param->auc_mac_addr, auc_mac_addr);

    pst_kick_user_param->us_reason_code = MAC_AUTH_NOT_VALID;

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_kick_user_param_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_TRUE,
                               &pst_rsp_msg);
    if ((l_ret != OAL_SUCC) || (pst_rsp_msg == OAL_PTR_NULL)) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "{wal_kick_sta:: wal_send_cfg_event return err code %d!}\r\n", l_ret);
        return l_ret;
    }

    /* 4.4  ��ȡ���صĴ����� */
    ul_err_code = wal_check_and_release_msg_resp(pst_rsp_msg);
    if (ul_err_code != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_CFG, "{wal_kick_sta::wal_check_and_release_msg_resp fail return err code: [%x]!}",
                         ul_err_code);
        return -OAL_EFAIL;
    }
#endif

    return OAL_SUCC;
}


OAL_STATIC int wal_ioctl_set_ap_config(oal_net_device_stru *pst_net_dev, oal_iw_request_info_stru *pst_info,
                                       oal_iwreq_data_union *pst_wrqu, char *pc_extra)
{
    oal_int8 *pc_command = OAL_PTR_NULL;
    oal_int8 *pc_parse_command = OAL_PTR_NULL;
    oal_int32 l_ret = OAL_SUCC;
    oal_uint32 ul_ret;
    oal_int8 ac_parse_command[WAL_IOCTL_PRIV_SUBCMD_MAX_LEN];
    oal_uint32 ul_off_set;

    if (OAL_UNLIKELY((pst_net_dev == OAL_PTR_NULL) || (pst_wrqu == OAL_PTR_NULL))) {
        OAM_WARNING_LOG2(0, OAM_SF_ANY, "{wal_ioctl_set_mac_filters:: param is OAL_PTR_NULL , pst_net_dev = %p, \
            pst_wrqu = %p}", (uintptr_t)pst_net_dev, (uintptr_t)pst_wrqu);
        return -OAL_EFAIL;
    }

    /* 1. �����ڴ汣��netd �·������������ */
    pc_command = oal_memalloc((oal_int32)(pst_wrqu->data.length + 1));
    if (pc_command == OAL_PTR_NULL) {
        return -OAL_ENOMEM;
    }
    /* 2. ����netd ����ں�̬�� */
    memset_s (pc_command, (oal_uint32)(pst_wrqu->data.length + 1), 0, (oal_uint32)(pst_wrqu->data.length + 1));
    ul_ret = oal_copy_from_user(pc_command, pst_wrqu->data.pointer, (oal_uint32)(pst_wrqu->data.length));
    if (ul_ret != OAL_SUCC) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_ioctl_set_ap_config::oal_copy_from_user: -OAL_EFAIL }\r\n");
        oal_free(pc_command);
        return -OAL_EFAIL;
    }
    pc_command[pst_wrqu->data.length] = '\0';

    OAL_IO_PRINT("wal_ioctl_set_ap_config, data len:%u \n", (oal_uint32)pst_wrqu->data.length);

    pc_parse_command = pc_command;
    /* 3.   �������� */
    /* 3.1  ����ASCII_CMD */
    ul_ret = wal_get_parameter_from_cmd(pc_parse_command, ac_parse_command, "ASCII_CMD=", &ul_off_set,
                                        WAL_IOCTL_PRIV_SUBCMD_MAX_LEN);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_ioctl_set_ap_config:wal_get_parameter_from_cmd ASCII_CMD fail, ret=[%u]",
                         ul_ret);
        oal_free(pc_command);
        return -OAL_EINVAL;
    }

    if ((strcmp("AP_CFG", ac_parse_command) != 0)) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_ioctl_set_ap_config::sub_command != 'AP_CFG' }");
        OAL_IO_PRINT("{wal_ioctl_set_ap_config::sub_command %6s...!= 'AP_CFG' }", ac_parse_command);
        oal_free(pc_command);
        return -OAL_EINVAL;
    }

    /* 3.2  ����CHANNEL��Ŀǰ������netd�·���channel��Ϣ */
    pc_parse_command += ul_off_set;
    ul_ret = wal_get_parameter_from_cmd(pc_parse_command, ac_parse_command, "CHANNEL=", &ul_off_set,
                                        WAL_IOCTL_PRIV_SUBCMD_MAX_LEN);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_ioctl_set_ap_config:wal_get_parameter_from_cmd CHANNEL fai, ret=[%u]",
                         ul_ret);
        oal_free(pc_command);
        return -OAL_EINVAL;
    }

    /* 3.3  ����MAX_SCB */
    pc_parse_command += ul_off_set;
    ul_ret = wal_get_parameter_from_cmd(pc_parse_command, ac_parse_command, "MAX_SCB=", &ul_off_set,
                                        WAL_IOCTL_PRIV_SUBCMD_MAX_LEN);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_ioctl_set_ap_config::wal_get_parameter_from_cmd MAX_SCB fai, , ret=[%u]",
                         ul_ret);
        oal_free(pc_command);
        return -OAL_EINVAL;
    }

    g_st_ap_config_info.ul_ap_max_user = (oal_uint32)oal_atoi(ac_parse_command);

    if (OAL_NET_DEV_PRIV(pst_net_dev) != OAL_PTR_NULL) {
        l_ret = wal_set_ap_max_user(pst_net_dev, (oal_uint32)oal_atoi(ac_parse_command));
    }

    /* 5. �����ͷ��ڴ� */
    oal_free(pc_command);
    return l_ret;
}


OAL_STATIC int wal_ioctl_get_assoc_list(oal_net_device_stru *pst_net_dev, oal_iw_request_info_stru *pst_info,
                                        oal_iwreq_data_union *pst_wrqu, char *pc_extra)
{
    oal_int32 l_ret;
    wal_msg_query_stru st_query_msg;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;
    wal_msg_rsp_stru *pst_query_rsp_msg = OAL_PTR_NULL;
    oal_int8 *pc_sta_list = OAL_PTR_NULL;
    oal_netbuf_stru *pst_response_netbuf = OAL_PTR_NULL;
    oal_int32 l_memcpy_ret = EOK;

    if (OAL_UNLIKELY((pst_net_dev == OAL_PTR_NULL) || (pst_info == OAL_PTR_NULL) ||
                     (pst_wrqu == OAL_PTR_NULL) || (pc_extra == OAL_PTR_NULL))) {
        OAM_WARNING_LOG4(0, OAM_SF_ANY, "{wal_ioctl_get_assoc_list:: param is OAL_PTR_NULL , pst_net_dev = %p, \
            pst_info = %p, pst_wrqu = %p, pc_extra = %p}",
                         (uintptr_t)pst_net_dev, (uintptr_t)pst_info, (uintptr_t)pst_wrqu, (uintptr_t)pc_extra);
        return -OAL_EFAIL;
    }

    /* �ϲ����κ�ʱ�򶼿����·��������Ҫ���жϵ�ǰnetdev��״̬����ʱ���� */
    if (OAL_UNLIKELY(OAL_NET_DEV_PRIV(pst_net_dev) == OAL_PTR_NULL)) {
        return -OAL_EFAIL;
    }

    /***************************************************************************
        ���¼���wal�㴦��
    ***************************************************************************/
    st_query_msg.en_wid = WLAN_CFGID_GET_STA_LIST;

    /* ������Ϣ */
    l_ret = wal_send_cfg_event(pst_net_dev, WAL_MSG_TYPE_QUERY,
                               WAL_MSG_WID_LENGTH, (oal_uint8 *)&st_query_msg,
                               OAL_TRUE, &pst_rsp_msg);
    if ((l_ret != OAL_SUCC) || (pst_rsp_msg == OAL_PTR_NULL)) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "{wal_ioctl_get_assoc_list:: wal_alloc_cfg_event return err code %d!}", l_ret);
        return l_ret;
    }

    /* ��������Ϣ */
    pst_query_rsp_msg = (wal_msg_rsp_stru *)(pst_rsp_msg->auc_msg_data);
    /* ҵ���� */
    if (pst_query_rsp_msg->us_len >= OAL_SIZEOF(oal_netbuf_stru *)) {
        /* ��ȡhmac�����netbufָ�� */
        l_memcpy_ret += memcpy_s(&pst_response_netbuf, OAL_SIZEOF(oal_netbuf_stru *), pst_query_rsp_msg->auc_value,
                                 OAL_SIZEOF(oal_netbuf_stru *));
        if (pst_response_netbuf != NULL) {
            /* ����ap�����sta��ַ��Ϣ */
            pc_sta_list = (oal_int8 *)OAL_NETBUF_DATA(pst_response_netbuf);
            pst_wrqu->data.length = (oal_uint16)(OAL_NETBUF_LEN(pst_response_netbuf) + 1);
            l_memcpy_ret += memcpy_s(pc_extra, OAL_MIN(pst_wrqu->data.length, OAL_AP_GET_ASSOC_LIST_LEN),
                                     pc_sta_list, OAL_MIN(pst_wrqu->data.length, OAL_AP_GET_ASSOC_LIST_LEN));
            pc_extra[OAL_MIN(OAL_NETBUF_LEN(pst_response_netbuf), OAL_AP_GET_ASSOC_LIST_LEN)] = '\0';
            oal_netbuf_free(pst_response_netbuf);
        } else {
            l_ret = -OAL_ENOMEM;
        }
    } else {
        /* groupsize��СΪ32 */
        oal_print_hex_dump((oal_uint8 *)pst_rsp_msg->auc_msg_data, pst_query_rsp_msg->us_len, 32, "query msg: ");
        l_ret = -OAL_EINVAL;
    }
    if (l_memcpy_ret != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "wal_ioctl_get_assoc_list::memcpy fail!");
        oal_free(pst_rsp_msg);
        return -OAL_EINVAL;
    }

    if (l_ret != OAL_SUCC) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "{wal_ioctl_get_assoc_list::process failed, ret=%d}", l_ret);
    }

    oal_free(pst_rsp_msg);
    return l_ret;
}


OAL_STATIC int wal_ioctl_set_mac_filters(oal_net_device_stru *pst_net_dev, oal_iw_request_info_stru *pst_info,
                                         oal_iwreq_data_union *pst_wrqu, char *pc_extra)
{
    mac_vap_stru *pst_vap = OAL_PTR_NULL;
    oal_int8 *pc_command = OAL_PTR_NULL;
    oal_int32 l_ret = 0;
    oal_uint32 ul_ret;
    oal_int8 ac_parsed_command[WAL_IOCTL_PRIV_SUBCMD_MAX_LEN];
    oal_int8 *pc_parse_command = OAL_PTR_NULL;
    oal_uint32 ul_mac_mode;
    oal_uint32 ul_mac_cnt;
    oal_uint8 auc_mac_addr[WLAN_MAC_ADDR_LEN];
    oal_uint32 ul_off_set;

    if (OAL_UNLIKELY((pst_net_dev == OAL_PTR_NULL) || (pst_info == OAL_PTR_NULL) ||
                     (pst_wrqu == OAL_PTR_NULL) || (pc_extra == OAL_PTR_NULL))) {
        OAM_WARNING_LOG4(0, OAM_SF_ANY, "{param is NULL,net_dev = %p,info = %p,wrqu = %p,extra = %p}",
                         (uintptr_t)pst_net_dev, (uintptr_t)pst_info, (uintptr_t)pst_wrqu, (uintptr_t)pc_extra);
        return -OAL_EFAIL;
    }

    /* 1. �����ڴ汣��netd �·������������ */
    pc_command = oal_memalloc((oal_int32)(pst_wrqu->data.length + 1));
    if (pc_command == OAL_PTR_NULL) {
        return -OAL_ENOMEM;
    }

    /* 2. ����netd ����ں�̬�� */
    memset_s (pc_command, (oal_uint32)(pst_wrqu->data.length + 1), 0, (oal_uint32)(pst_wrqu->data.length + 1));
    ul_ret = oal_copy_from_user(pc_command, pst_wrqu->data.pointer, (oal_uint32)(pst_wrqu->data.length));
    if (ul_ret != OAL_SUCC) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_ioctl_set_mac_filters::oal_copy_from_user: -OAL_EFAIL }\r\n");
        oal_free(pc_command);
        return -OAL_EFAIL;
    }
    pc_command[pst_wrqu->data.length] = '\0';

    OAL_IO_PRINT("wal_ioctl_set_mac_filters, data len:%d \n", pst_wrqu->data.length);

    pc_parse_command = pc_command;

    memset_s(g_st_ap_config_info.ac_ap_mac_filter_mode, OAL_SIZEOF(g_st_ap_config_info.ac_ap_mac_filter_mode),
             0, OAL_SIZEOF(g_st_ap_config_info.ac_ap_mac_filter_mode));
    if (strncpy_s(g_st_ap_config_info.ac_ap_mac_filter_mode, OAL_SIZEOF(g_st_ap_config_info.ac_ap_mac_filter_mode),
        pc_command, OAL_SIZEOF(g_st_ap_config_info.ac_ap_mac_filter_mode) - 1) != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_ioctl_set_mac_filters::strncpy_s error.}");
    }

    pst_vap = OAL_NET_DEV_PRIV(pst_net_dev);
    if (pst_vap == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_ioctl_set_mac_filters::netdevice vap is null, just save it.}\r\n");
        oal_free(pc_command);
        return OAL_SUCC;
    }

    /* 3  ����MAC_MODE */
    ul_ret = wal_get_parameter_from_cmd(pc_parse_command, ac_parsed_command, "MAC_MODE=", &ul_off_set,
                                        WAL_IOCTL_PRIV_SUBCMD_MAX_LEN);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_ioctl_set_mac_filters::get_param_from_cmd return err_code [%u]", ul_ret);
        oal_free(pc_command);
        return -OAL_EINVAL;
    }
    /* 3.1 �������Ƿ�Ϸ� 0,1,2 */
    ul_mac_mode = (oal_uint32)oal_atoi(ac_parsed_command);
    if (ul_mac_mode > 2) { /* ����2�ǷǷ��� */
        OAM_WARNING_LOG4(0, OAM_SF_ANY, "{wal_ioctl_set_mac_filters::invalid MAC_MODE[%c%c%c%c]!}",
                         (oal_uint8)ac_parsed_command[0], (oal_uint8)ac_parsed_command[1],
                         /* ac_parsed_command��0��1��2��3byte��Ϊ���������ӡ */
                         (oal_uint8)ac_parsed_command[2], (oal_uint8)ac_parsed_command[3]);
        oal_free(pc_command);
        return -OAL_EINVAL;
    }

    /* 5 ����MAC_CNT */
    pc_parse_command += ul_off_set;
    ul_ret = wal_get_parameter_from_cmd(pc_parse_command, ac_parsed_command, "MAC_CNT=", &ul_off_set,
                                        WAL_IOCTL_PRIV_SUBCMD_MAX_LEN);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_ioctl_set_mac_filters::wal_get_parameter_from_cmd fail, ret=[%u]", ul_ret);
        oal_free(pc_command);
        return -OAL_EINVAL;
    }
    ul_mac_cnt = (oal_uint32)oal_atoi(ac_parsed_command);

    wal_config_mac_filter(pst_net_dev, pc_command);

    /* ����ǰ�����ģʽ��ul_mac_modeΪ2�������·�����MAC��ַΪ�գ����������κ��豸��������Ҫȥ���������Ѿ�������STA */
    if ((ul_mac_cnt == 0) && (ul_mac_mode == 2)) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_ioctl_set_mac_filters::delete all user!}");

        memset_s(auc_mac_addr, OAL_SIZEOF(auc_mac_addr), 0xff, OAL_ETH_ALEN);
        l_ret = wal_kick_sta(pst_net_dev, auc_mac_addr, OAL_SIZEOF(auc_mac_addr));
    }

    oal_free(pc_command);
    return l_ret;
}


OAL_STATIC int wal_ioctl_set_ap_sta_disassoc(oal_net_device_stru *pst_net_dev, oal_iw_request_info_stru *pst_info,
                                             oal_iwreq_data_union *pst_wrqu, char *pc_extra)
{
    oal_int8 *pc_command = OAL_PTR_NULL;
    oal_int32 l_ret;
    oal_uint32 ul_ret;
    oal_int8 ac_parsed_command[WAL_IOCTL_PRIV_SUBCMD_MAX_LEN] = { 0 };
    oal_uint8 auc_mac_addr[WLAN_MAC_ADDR_LEN] = { 0 };
    oal_uint32 ul_off_set;

    if (OAL_UNLIKELY((pst_net_dev == OAL_PTR_NULL) || (pst_wrqu == OAL_PTR_NULL))) {
        OAM_WARNING_LOG2(0, OAM_SF_ANY, "{wal_ioctl_set_mac_filters:: param is OAL_PTR_NULL , pst_net_dev = %p, \
            pst_wrqu = %p}", (uintptr_t)pst_net_dev, (uintptr_t)pst_wrqu);
        return -OAL_EFAIL;
    }

    /* 1. �����ڴ汣��netd �·������������ */
    pc_command = oal_memalloc((oal_int32)(pst_wrqu->data.length + 1));
    if (pc_command == OAL_PTR_NULL) {
        return -OAL_ENOMEM;
    }

    /* 2. ����netd ����ں�̬�� */
    memset_s (pc_command, (oal_uint32)(pst_wrqu->data.length + 1), 0, (oal_uint32)(pst_wrqu->data.length + 1));
    ul_ret = oal_copy_from_user(pc_command, pst_wrqu->data.pointer, (oal_uint32)(pst_wrqu->data.length));
    if (ul_ret != OAL_SUCC) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_ioctl_set_ap_sta_disassoc::oal_copy_from_user: -OAL_EFAIL }\r\n");
        oal_free(pc_command);
        return -OAL_EFAIL;
    }
    pc_command[pst_wrqu->data.length] = '\0';

    /* 3. ���������ȡMAC */
    ul_ret = wal_get_parameter_from_cmd(pc_command, ac_parsed_command, "MAC=", &ul_off_set,
                                        WAL_IOCTL_PRIV_SUBCMD_MAX_LEN);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_ioctl_set_ap_sta_disassoc::wal_get_parameter_from_cmd MAC fail, ret=[%u]",
                         ul_ret);
        oal_free(pc_command);
        return -OAL_EINVAL;
    }
    /* 3.1  �������Ƿ����MAC����(6 * 2) */
    if (OAL_STRLEN(ac_parsed_command) != WLAN_MAC_ADDR_LEN * 2) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_ioctl_set_ap_sta_disassoc::invalid MAC format}\r\n");
        oal_free(pc_command);
        return -OAL_EINVAL;
    }
    oal_strtoaddr(ac_parsed_command, auc_mac_addr); /* ���ַ� ac_name ת�������� mac_add[6] */
    OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_ioctl_set_ap_sta_disassoc::Geting CMD from APP to DISASSOC!!}");
    l_ret = wal_kick_sta(pst_net_dev, auc_mac_addr, OAL_SIZEOF(auc_mac_addr));

    /* 5. �����ͷ��ڴ� */
    oal_free(pc_command);
    return l_ret;
}


oal_uint32 wal_hipriv_get_mac_addr(oal_int8 *pc_param, oal_uint8 auc_mac_addr[], oal_uint32 *pul_total_offset)
{
    oal_uint32 ul_off_set = 0;
    oal_uint32 ul_ret;
    oal_int8 ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];

    /* ��ȡmac��ַ */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, OAL_SIZEOF(ac_name), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_hipriv_get_mac_addr::wal_get_cmd_one_arg return err_code [%d]", ul_ret);
        return ul_ret;
    }
    oal_strtoaddr(ac_name, auc_mac_addr);
    *pul_total_offset = ul_off_set;

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_EDCA_OPT_AP

OAL_STATIC oal_uint32 wal_hipriv_set_edca_opt_switch_sta(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    wal_msg_write_stru st_write_msg;
    oal_uint8 uc_flag;
    oal_uint8 *puc_value = 0;
    oal_uint32 ul_ret;
    oal_uint32 ul_off_set = 0;
    oal_int32 l_ret;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    oal_int8 ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];

    /* ��ȡmac_vap */
    pst_mac_vap = OAL_NET_DEV_PRIV(pst_net_dev);
    if (pst_mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_STA) {
        OAM_WARNING_LOG0(0, OAM_SF_EDCA, "{wal_hipriv_set_edca_opt_switch_sta:: only STA_MODE support}");
        return OAL_FAIL;
    }

    /* ��ȡ���ò��� */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, OAL_SIZEOF(ac_name), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_EDCA, "wal_hipriv_set_edca_opt_switch_sta:wal_get_cmd_one_arg fail, err_code [%d]",
                         ul_ret);
        return ul_ret;
    }

    uc_flag = (oal_uint8)oal_atoi(ac_name);
    /* �Ƿ����ò��� */
    if (uc_flag > 1) {
        OAM_WARNING_LOG0(0, OAM_SF_EDCA, "wal_hipriv_set_edca_opt_switch_sta, invalid config, should be 0 or 1");
        return OAL_SUCC;
    }

    /* �����¼��ڴ� */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_EDCA_OPT_SWITCH_STA, OAL_SIZEOF(oal_uint8));
    puc_value = (oal_uint8 *)(st_write_msg.auc_value);
    *puc_value = uc_flag;

    /***************************************************************************
                             ���¼���wal�㴦��
    ***************************************************************************/
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(oal_uint8),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_EDCA, "{wal_hipriv_set_edca_opt_switch_sta:: return err_code [%d]!}\r\n", l_ret);
        return (oal_uint32)l_ret;
    }

    return OAL_SUCC;
}
#endif

#ifdef _PRE_WLAN_FEATURE_CUSTOM_SECURITY

oal_uint32 wal_hipriv_send_cfg_uint32_data(oal_net_device_stru *pst_net_dev,
                                           oal_int8 *pc_param, wlan_cfgid_enum_uint16 cfgid)
{
    wal_msg_write_stru st_write_msg;
    oal_int32 l_ret;
    oal_uint16 us_len;
    oal_uint32 ul_ret;
    oal_int8 ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN] = { 0 };
    oal_uint32 ul_off_set = 0;
    oal_uint32 set_value;

    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, OAL_SIZEOF(ac_name), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_hipriv_send_cfg_uint32_data:wal_get_cmd_one_arg fail!}\r\n");
        return ul_ret;
    }

    pc_param += ul_off_set;
    set_value = (oal_uint32)oal_atoi((const oal_int8 *)ac_name);

    us_len = 4; /* 4��OAL_SIZEOF(oal_uint32) */
    *(oal_uint32 *)(st_write_msg.auc_value) = set_value;

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, cfgid, us_len);

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + us_len,
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_CFG, "{wal_hipriv_send_cfg_uint32_data:wal_send_cfg_event return [%d].}\r\n", l_ret);
        return (oal_uint32)l_ret;
    }

    return OAL_SUCC;
}
#endif /* _PRE_WLAN_FEATURE_CUSTOM_SECURITY */


OAL_STATIC oal_uint32 wal_hipriv_bgscan_enable(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    oal_uint32 ul_off_set;
    oal_int8 ac_stop[2];
    oal_uint32 ul_ret;
    oal_int32 l_ret;
    wal_msg_write_stru st_write_msg;
    oal_uint8 *pen_bgscan_enable_flag = OAL_PTR_NULL;

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_stop, OAL_SIZEOF(ac_stop), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG0(0, OAM_SF_SCAN, "wal_hipriv_bgscan_enable: get first arg fail.");
        return OAL_FAIL;
    }

    /***************************************************************************
                            ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFIGD_BGSCAN_ENABLE, OAL_SIZEOF(oal_bool_enum_uint8));

    /* ��������������� */
    pen_bgscan_enable_flag = (oal_uint8 *)(st_write_msg.auc_value);
    *pen_bgscan_enable_flag = (oal_uint8)oal_atoi(ac_stop);

    OAM_WARNING_LOG1(0, OAM_SF_SCAN, "wal_hipriv_bgscan_enable:: bgscan_enable_flag= %d.", *pen_bgscan_enable_flag);

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(oal_bool_enum_uint8),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (l_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_hipriv_bgscan_enable:wal_send_cfg_event fail.return err code [%d]", l_ret);
        return (oal_uint32)l_ret;
    }

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_STA_PM

OAL_STATIC oal_uint32 wal_hipriv_sta_ps_mode(oal_net_device_stru *pst_cfg_net_dev, oal_int8 *pc_param)
{
    wal_msg_write_stru st_write_msg;
    oal_uint32 ul_off_set;
    oal_int8 ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    oal_uint32 ul_ret;
    oal_int32 l_ret;
    oal_uint8 uc_vap_ps_mode;
    mac_cfg_ps_mode_param_stru *pst_ps_mode_param;

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, OAL_SIZEOF(ac_name), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_hipriv_sta_ps_enable:wal_get_cmd_one_arg return err_code [%d]", ul_ret);
        return ul_ret;
    }

    uc_vap_ps_mode = (oal_uint8)oal_atoi(ac_name);
    pc_param = pc_param + ul_off_set;

    /***************************************************************************
                             ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_PS_MODE, OAL_SIZEOF(mac_cfg_ps_mode_param_stru));

    /* ��������������� */
    pst_ps_mode_param = (mac_cfg_ps_mode_param_stru *)(st_write_msg.auc_value);
    pst_ps_mode_param->uc_vap_ps_mode = uc_vap_ps_mode;

    l_ret = wal_send_cfg_event(pst_cfg_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_ps_mode_param_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_PWR, "{wal_hipriv_sta_ps_enable::return err code [%d]!}\r\n", l_ret);
        return (oal_uint32)l_ret;
    }

    return OAL_SUCC;
}

#ifdef _PRE_PSM_DEBUG_MODE

OAL_STATIC oal_uint32 wal_hipriv_sta_ps_info(oal_net_device_stru *pst_cfg_net_dev, oal_int8 *pc_param)
{
    wal_msg_write_stru st_write_msg;
    oal_uint32 ul_off_set;
    oal_int8 ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    oal_uint32 ul_ret;
    oal_int32 l_ret;
    oal_uint8 uc_psm_info_enable;
    oal_uint8 uc_psm_debug_mode;
    mac_cfg_ps_info_stru *pst_ps_info;

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, OAL_SIZEOF(ac_name), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_sta_ps_info::wal_get_cmd_one_arg return err_code [%d]!}", ul_ret);
        return ul_ret;
    }

    uc_psm_info_enable = (oal_uint8)oal_atoi(ac_name);
    pc_param = pc_param + ul_off_set;

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, OAL_SIZEOF(ac_name), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_sta_ps_info::wal_get_cmd_one_arg return err_code [%d]!}", ul_ret);
        return ul_ret;
    }

    uc_psm_debug_mode = (oal_uint8)oal_atoi(ac_name);
    pc_param = pc_param + ul_off_set;

    /***************************************************************************
                             ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SHOW_PS_INFO, OAL_SIZEOF(mac_cfg_ps_info_stru));

    /* ��������������� */
    pst_ps_info = (mac_cfg_ps_info_stru *)(st_write_msg.auc_value);
    pst_ps_info->uc_psm_info_enable = uc_psm_info_enable;
    pst_ps_info->uc_psm_debug_mode = uc_psm_debug_mode;

    l_ret = wal_send_cfg_event(pst_cfg_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_ps_info_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_PWR, "{wal_hipriv_sta_ps_info::return err code [%d]!}\r\n", l_ret);
        return (oal_uint32)l_ret;
    }

    return OAL_SUCC;
}

#endif
#endif

#ifdef _PRE_WLAN_FEATURE_STA_UAPSD

OAL_STATIC oal_uint32 wal_hipriv_set_uapsd_para(oal_net_device_stru *pst_cfg_net_dev, oal_int8 *pc_param)
{
    wal_msg_write_stru st_write_msg;
    oal_uint32 ul_off_set;
    oal_int8 ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    oal_uint32 ul_ret;
    oal_int32 l_ret;
    mac_cfg_uapsd_sta_stru *pst_uapsd_param;
    oal_uint8 uc_max_sp_len;
    oal_uint8 uc_ac;
    oal_uint8 uc_delivery_enabled[WLAN_WME_AC_BUTT];
    oal_uint8 uc_trigger_enabled[WLAN_WME_AC_BUTT];

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, OAL_SIZEOF(ac_name), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_hipriv_set_uapsd_para:wal_get_cmd_one_arg fail, err_code [%d]!", ul_ret);
        return ul_ret;
    }

    uc_max_sp_len = (oal_uint8)oal_atoi(ac_name);

    for (uc_ac = 0; uc_ac < WLAN_WME_AC_BUTT; uc_ac++) {
        pc_param = pc_param + ul_off_set;
        ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, OAL_SIZEOF(ac_name), &ul_off_set);
        if (ul_ret != OAL_SUCC) {
            OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_hipriv_set_uapsd_para:wal_get_cmd_one_arg fail, err_code[%d]",
                             ul_ret);
            return ul_ret;
        }

        /* delivery_enabled�Ĳ������� */
        uc_delivery_enabled[uc_ac] = (oal_uint8)oal_atoi(ac_name);

        /* trigger_enabled ���������� */
        uc_trigger_enabled[uc_ac] = (oal_uint8)oal_atoi(ac_name);
    }
    /***************************************************************************
                             ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_UAPSD_PARA, OAL_SIZEOF(mac_cfg_uapsd_sta_stru));

    /* ��������������� */
    pst_uapsd_param = (mac_cfg_uapsd_sta_stru *)(st_write_msg.auc_value);
    pst_uapsd_param->uc_max_sp_len = uc_max_sp_len;
    for (uc_ac = 0; uc_ac < WLAN_WME_AC_BUTT; uc_ac++) {
        pst_uapsd_param->uc_delivery_enabled[uc_ac] = uc_delivery_enabled[uc_ac];
        pst_uapsd_param->uc_trigger_enabled[uc_ac] = uc_trigger_enabled[uc_ac];
    }

    l_ret = wal_send_cfg_event(pst_cfg_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_uapsd_sta_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_PWR, "{wal_hipriv_set_uapsd_para::return err code [%d]!}\r\n", l_ret);
        return (oal_uint32)l_ret;
    }

    return OAL_SUCC;
}
#endif

#if defined(_PRE_PRODUCT_ID_HI110X_HOST)
#ifdef _PRE_WLAN_NARROW_BAND

oal_int32 wal_set_nbfh(oal_net_device_stru *pst_net_dev)
{
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
    mac_cfg_nbfh_param_stru *pst_nbfh_param;
#endif
    wal_msg_write_stru st_write_msg;
    oal_int32 l_ret;

    if (OAL_UNLIKELY(pst_net_dev == OAL_PTR_NULL)) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_set_nbfh::pst_net_dev is null ptr!}\r\n");
        return -OAL_EFAUL;
    }

#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
    pst_nbfh_param = (mac_cfg_nbfh_param_stru *)(st_write_msg.auc_value);

    pst_nbfh_param->ul_nbfh_tbtt_offset = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_NBFH_TBTT_OFFSET);
    pst_nbfh_param->ul_nbfh_tbtt_sync_time = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_NBFH_TBTT_SYNC_TIME);
    pst_nbfh_param->ul_nbfh_dwell_time = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_NBFH_DWELL_TIME);
    pst_nbfh_param->ul_nbfh_beacon_time = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_NBFH_BEACON_TIME);
#endif

    /***************************************************************************
        ���¼���wal�㴦��
    ***************************************************************************/
    /* ��д��Ϣ */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_NBFH, OAL_SIZEOF(mac_cfg_nbfh_param_stru));

    /* ������Ϣ */
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_nbfh_param_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_set_nbfh::wal_alloc_cfg_event return err code %d!}\r\n", l_ret);
    }

    return l_ret;
}
#endif


oal_int32 wal_start_vap(oal_net_device_stru *pst_net_dev)
{
    wal_msg_write_stru st_write_msg;
    oal_int32 l_ret;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;
    oal_uint32 ul_err_code;
#ifdef _PRE_WLAN_FEATURE_P2P
    wlan_p2p_mode_enum_uint8 en_p2p_mode;
    oal_wireless_dev_stru *pst_wdev;
#endif

    if (OAL_UNLIKELY(pst_net_dev == OAL_PTR_NULL)) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_start_vap::pst_net_dev is null ptr!}\r\n");
        return -OAL_EFAUL;
    }

    OAL_IO_PRINT("wal_start_vap, dev_name is:%.16s\n", pst_net_dev->name);

    /***************************************************************************
        ���¼���wal�㴦��
    ***************************************************************************/
    /* ��д��Ϣ */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_START_VAP, OAL_SIZEOF(mac_cfg_start_vap_param_stru));
    ((mac_cfg_start_vap_param_stru *)st_write_msg.auc_value)->pst_net_dev = pst_net_dev;
#ifdef _PRE_WLAN_FEATURE_P2P
    pst_wdev = pst_net_dev->ieee80211_ptr;
    en_p2p_mode = wal_wireless_iftype_to_mac_p2p_mode(pst_wdev->iftype);
    if (en_p2p_mode == WLAN_P2P_BUTT) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_start_vap::wal_wireless_iftype_to_mac_p2p_mode return BUFF}\r\n");
        return -OAL_EINVAL;
    }
    ((mac_cfg_start_vap_param_stru *)st_write_msg.auc_value)->en_p2p_mode = en_p2p_mode;

    OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_start_vap::en_p2p_mode:%d}\r\n", en_p2p_mode);
#endif
    ((mac_cfg_start_vap_param_stru *)st_write_msg.auc_value)->en_mgmt_rate_init_flag = OAL_TRUE;

    /* ������Ϣ */
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_start_vap_param_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_TRUE, &pst_rsp_msg);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_start_vap::wal_alloc_cfg_event return err code %d!}\r\n", l_ret);
        return l_ret;
    }

    /* ��������Ϣ */
    ul_err_code = wal_check_and_release_msg_resp(pst_rsp_msg);
    if (ul_err_code != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_start_vap::hmac start vap fail, err code[%d]!}\r\n", ul_err_code);
        return -OAL_EINVAL;
    }

    if ((OAL_NETDEVICE_FLAGS(pst_net_dev) & OAL_IFF_RUNNING) == 0) {
        OAL_NETDEVICE_FLAGS(pst_net_dev) |= OAL_IFF_RUNNING;
    }

    /* APģʽ,����VAP��,�������Ͷ��� */
    oal_net_tx_wake_all_queues(pst_net_dev); /* �������Ͷ��� */

    return OAL_SUCC;
}


oal_int32 wal_stop_vap(oal_net_device_stru *pst_net_dev)
{
    wal_msg_write_stru st_write_msg;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;
    oal_int32 l_ret;
#ifdef _PRE_WLAN_FEATURE_P2P
    wlan_p2p_mode_enum_uint8 en_p2p_mode;
    oal_wireless_dev_stru *pst_wdev;
#endif

    if (OAL_UNLIKELY(pst_net_dev == OAL_PTR_NULL)) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_stop_vap::pst_net_dev is null ptr!}\r\n");
        return -OAL_EFAUL;
    }

    /* �������up״̬������ֱ�ӷ��سɹ�,��ֹnetdevice״̬��VAP״̬��һ�µ���� */
    if ((OAL_NETDEVICE_FLAGS(pst_net_dev) & OAL_IFF_RUNNING) == 0) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_stop_vap::vap is already down, continue to reset hmac vap state.}\r\n");
    }

    OAL_IO_PRINT("wal_stop_vap, dev_name is:%.16s\n", pst_net_dev->name);

    /***************************************************************************
                           ���¼���wal�㴦��
    ***************************************************************************/
    /* ��дWID��Ϣ */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_DOWN_VAP, OAL_SIZEOF(mac_cfg_down_vap_param_stru));
    ((mac_cfg_down_vap_param_stru *)st_write_msg.auc_value)->pst_net_dev = pst_net_dev;
#ifdef _PRE_WLAN_FEATURE_P2P
    pst_wdev = pst_net_dev->ieee80211_ptr;
    en_p2p_mode = wal_wireless_iftype_to_mac_p2p_mode(pst_wdev->iftype);
    if (en_p2p_mode == WLAN_P2P_BUTT) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_stop_vap::wal_wireless_iftype_to_mac_p2p_mode return BUFF}\r\n");
        return -OAL_EINVAL;
    }
    ((mac_cfg_start_vap_param_stru *)st_write_msg.auc_value)->en_p2p_mode = en_p2p_mode;

    OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_stop_vap::en_p2p_mode:%d}\r\n", en_p2p_mode);
#endif

    /* ������Ϣ */
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_down_vap_param_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_TRUE,
                               &pst_rsp_msg);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_stop_vap::wal_alloc_cfg_event return err code %d!}\r\n", l_ret);
        return l_ret;
    }

    if (wal_check_and_release_msg_resp(pst_rsp_msg) != OAL_SUCC) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "wal_stop_vap::wal_check_and_release_msg_resp fail");
    }

    return OAL_SUCC;
}


#ifdef _PRE_WLAN_FEATURE_TAS_ANT_SWITCH
oal_void wal_init_witas_state(oal_void)
{
    oal_int32 wifi_tas_state;

    if (g_aen_tas_switch_en[WITP_RF_CHANNEL_ZERO] == OAL_FALSE) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{not support WiTAS}\r\n");
        return;
    }

    if (board_get_wifi_support_tas() == OAL_FALSE) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{WiTAS disabled in DTS}\r\n");
        return;
    }

    if (board_get_wifi_tas_gpio_init_sts(&wifi_tas_state) == BOARD_FAIL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{get TAS init GPIO sts fail}\r\n");
        return;
    }

    if (board_get_wifi_tas_gpio_state() != wifi_tas_state) {
        OAM_WARNING_LOG2(0, OAM_SF_ANY, "{current WiTAS state is [%d], change to state [%d]!}\r\n",
                         board_get_wifi_tas_gpio_state(), wifi_tas_state);
        board_wifi_tas_set(wifi_tas_state);
        return;
    } else {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{current WiTAS state is right, no need to set again!}\r\n");
        return;
    }
}
#endif


oal_int32 wal_init_wlan_vap(oal_net_device_stru *pst_net_dev)
{
    oal_net_device_stru *pst_cfg_net_dev;
    wal_msg_write_stru st_write_msg;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;
    oal_uint32 ul_err_code;
    mac_vap_stru *pst_mac_vap;
    oal_wireless_dev_stru *pst_wdev;
    mac_wiphy_priv_stru *pst_wiphy_priv;
    mac_vap_stru *pst_cfg_mac_vap;
    hmac_vap_stru *pst_cfg_hmac_vap;
    mac_device_stru *pst_mac_device;
    oal_int32 l_ret;

    wlan_vap_mode_enum_uint8 en_vap_mode;
#ifdef _PRE_WLAN_FEATURE_P2P
    wlan_p2p_mode_enum_uint8 en_p2p_mode = WLAN_LEGACY_VAP_MODE;
#endif

    if (pst_net_dev == NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_init_wlan_vap::pst_net_dev is null!}\r\n");
        return -OAL_EINVAL;
    }

    pst_mac_vap = OAL_NET_DEV_PRIV(pst_net_dev);
    if (pst_mac_vap != NULL) {
        if (pst_mac_vap->en_vap_state != MAC_VAP_STATE_BUTT) {
            OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_init_wlan_vap::pst_mac_vap is already exist}\r\n");
            return OAL_SUCC;
        }
        /* netdev�µ�vap�Ѿ���ɾ������Ҫ���´����͹��� */
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "wal_init_wlan_vap::pst_mac_vap is already free");
        OAL_NET_DEV_PRIV(pst_net_dev) = OAL_PTR_NULL;
    }

    pst_wdev = OAL_NETDEVICE_WDEV(pst_net_dev);
    if (pst_wdev == NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_init_wlan_vap::pst_wdev is null!}\r\n");
        return -OAL_EFAUL;
    }

    pst_wiphy_priv = (mac_wiphy_priv_stru *)oal_wiphy_priv(pst_wdev->wiphy);
    if (pst_wiphy_priv == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_init_wlan_vap::pst_wiphy_priv is null!}\r\n");
        return -OAL_EFAUL;
    }
    pst_mac_device = pst_wiphy_priv->pst_mac_device;
    if (pst_mac_device == NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_init_wlan_vap::pst_mac_device is null!}\r\n");
        return -OAL_EFAUL;
    }

    pst_cfg_mac_vap = (mac_vap_stru *)mac_res_get_mac_vap(pst_mac_device->uc_cfg_vap_id);
    if (pst_cfg_mac_vap == NULL) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_init_wlan_vap::pst_cfg_mac_vap is null! vap_id:%d}\r\n",
                         pst_mac_device->uc_cfg_vap_id);
        return -OAL_EFAUL;
    }
    pst_cfg_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_device->uc_cfg_vap_id);
    if (pst_cfg_hmac_vap == NULL) {
        OAM_WARNING_LOG0(pst_mac_device->uc_cfg_vap_id, OAM_SF_ANY, "wal_init_wlan_vap:pst_cfg_hmac_vap is null");
        return -OAL_EFAUL;
    }
    pst_cfg_net_dev = pst_cfg_hmac_vap->pst_net_device;
    if (pst_cfg_net_dev == NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_init_wlan_vap::pst_cfg_net_dev is null!}\r\n");
        return -OAL_EFAUL;
    }
    /* ������WIFI��AP��ʱ����VAP */
    if ((pst_wdev->iftype == NL80211_IFTYPE_STATION) || (pst_wdev->iftype == NL80211_IFTYPE_P2P_DEVICE)) {
        if ((strcmp("wlan0", pst_net_dev->name)) == 0) {
            en_vap_mode = WLAN_VAP_MODE_BSS_STA;
        }
#ifdef _PRE_WLAN_FEATURE_P2P
        else if ((strcmp("p2p0", pst_net_dev->name)) == 0) {
            en_vap_mode = WLAN_VAP_MODE_BSS_STA;
            en_p2p_mode = WLAN_P2P_DEV_MODE;
        }
#endif
        else {
            OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_init_wlan_vap::net_dev is not wlan0 or p2p0!}\r\n");
            return OAL_SUCC;
        }
    } else if (pst_wdev->iftype == NL80211_IFTYPE_AP) {
        en_vap_mode = WLAN_VAP_MODE_BSS_AP;
    } else {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_init_wlan_vap::net_dev is not wlan0 or p2p0!}\r\n");
        return OAL_SUCC;
    }

#ifdef _PRE_WLAN_FEATURE_P2P
    OAM_WARNING_LOG2(0, OAM_SF_ANY, "{wal_init_wlan_vap::en_vap_mode:%d, en_p2p_mode:%d}", en_vap_mode, en_p2p_mode);
#endif

    /***************************************************************************
        ���¼���wal�㴦��
    ***************************************************************************/
    /* ��д��Ϣ */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_ADD_VAP, OAL_SIZEOF(mac_cfg_add_vap_param_stru));
    ((mac_cfg_add_vap_param_stru *)st_write_msg.auc_value)->pst_net_dev = pst_net_dev;
    ((mac_cfg_add_vap_param_stru *)st_write_msg.auc_value)->en_vap_mode = en_vap_mode;
    ((mac_cfg_add_vap_param_stru *)st_write_msg.auc_value)->uc_cfg_vap_indx = pst_cfg_mac_vap->uc_vap_id;
#ifdef _PRE_WLAN_FEATURE_P2P
    ((mac_cfg_add_vap_param_stru *)st_write_msg.auc_value)->en_p2p_mode = en_p2p_mode;
#endif
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
    ((mac_cfg_add_vap_param_stru *)st_write_msg.auc_value)->bit_11ac2g_enable =
        (oal_uint8) !!hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_11AC2G_ENABLE);
    ((mac_cfg_add_vap_param_stru *)st_write_msg.auc_value)->bit_disable_capab_2ght40 =
        wlan_customize.uc_disable_capab_2ght40;
#endif
    /* ������Ϣ */
    l_ret = wal_send_cfg_event(pst_cfg_net_dev, WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_add_vap_param_stru),
                               (oal_uint8 *)&st_write_msg, OAL_TRUE, &pst_rsp_msg);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(pst_cfg_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_init_wlan_vap::return err code %d!}\r\n", l_ret);
        return -OAL_EFAIL;
    }

    /* ��ȡ���صĴ����� */
    ul_err_code = wal_check_and_release_msg_resp(pst_rsp_msg);
    if (ul_err_code != OAL_SUCC) {
        OAM_WARNING_LOG1(pst_cfg_mac_vap->uc_vap_id, OAM_SF_ANY, "wal_init_wlan_vap:hmac add vap fail, err code[%u]",
                         ul_err_code);
        return -OAL_EFAIL;
    }

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
    if (wal_set_random_mac_to_mib(pst_net_dev) != OAL_SUCC) {
        OAM_WARNING_LOG0(pst_cfg_mac_vap->uc_vap_id, OAM_SF_ANY, "wal_init_wlan_vap::wal_set_random_mac_to_mib fail!");
        return -OAL_EFAUL;
    }
#endif

    /* ����netdevice��MAC��ַ��MAC��ַ��HMAC�㱻��ʼ����MIB�� */
    pst_mac_vap = OAL_NET_DEV_PRIV(pst_net_dev);
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_init_wlan_vap::OAL_NET_DEV_PRIV(pst_net_dev) is null ptr.}");
        return -OAL_EINVAL;
    }

#ifdef _PRE_WLAN_FEATURE_P2P
    if (en_p2p_mode == WLAN_P2P_DEV_MODE) {
        pst_mac_device->st_p2p_info.uc_p2p0_vap_idx = pst_mac_vap->uc_vap_id;
    }
#endif

    if (pst_wdev->iftype == NL80211_IFTYPE_AP) {
        /* APģʽ��ʼ������ʼ����������û�����mac��ַ����ģʽ */
        if (g_st_ap_config_info.ul_ap_max_user > 0) {
            wal_set_ap_max_user(pst_net_dev, g_st_ap_config_info.ul_ap_max_user);
        }

        if (OAL_STRLEN(g_st_ap_config_info.ac_ap_mac_filter_mode) > 0) {
            wal_config_mac_filter(pst_net_dev, g_st_ap_config_info.ac_ap_mac_filter_mode);
        }
    }
#ifdef _PRE_WLAN_FEATURE_TAS_ANT_SWITCH
    wal_init_witas_state();
#endif
    return OAL_SUCC;
}


oal_int32 wal_deinit_wlan_vap(oal_net_device_stru *pst_net_dev)
{
    wal_msg_write_stru st_write_msg;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;
    mac_vap_stru *pst_mac_vap;
    oal_int32 l_ret;
    oal_int32 l_del_vap_flag = OAL_TRUE;

#ifdef _PRE_WLAN_FEATURE_P2P
    wlan_p2p_mode_enum_uint8 en_p2p_mode = WLAN_LEGACY_VAP_MODE;
#endif

    if (OAL_UNLIKELY(pst_net_dev == OAL_PTR_NULL)) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_deinit_wlan_vap::pst_del_vap_param null ptr !}\r\n");
        return -OAL_EINVAL;
    }

    pst_mac_vap = OAL_NET_DEV_PRIV(pst_net_dev);
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_deinit_wlan_vap::pst_mac_vap is already null}\r\n");
        return OAL_SUCC;
    }

    /* ������WIFI��AP�ر�ʱɾ��VAP */
    if (((strcmp("wlan0", pst_net_dev->name)) != 0) && (0 != (strcmp("p2p0", pst_net_dev->name)))) {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_deinit_wlan_vap::net_dev is not wlan0 or p2p0!}");
        return OAL_SUCC;
    }

#ifdef _PRE_WLAN_FEATURE_P2P
    if (strcmp("p2p0", pst_net_dev->name) == 0) {
        en_p2p_mode = WLAN_P2P_DEV_MODE;
    }
#endif

#ifdef _PRE_WLAN_FEATURE_P2P
    OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_deinit_wlan_vap::en_p2p_mode:%d}\r\n", en_p2p_mode);
#endif

    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    // ɾ��vap ʱ��Ҫ��������ֵ��
    ((mac_cfg_del_vap_param_stru *)st_write_msg.auc_value)->pst_net_dev = pst_net_dev;
#ifdef _PRE_WLAN_FEATURE_P2P
    ((mac_cfg_del_vap_param_stru *)st_write_msg.auc_value)->en_p2p_mode = en_p2p_mode;
#endif

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_DEL_VAP, OAL_SIZEOF(mac_cfg_del_vap_param_stru));
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_del_vap_param_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_TRUE,
                               &pst_rsp_msg);

    if (wal_check_and_release_msg_resp(pst_rsp_msg) != OAL_SUCC) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "wal_deinit_wlan_vap::wal_check_and_release_msg_resp fail.");
        /* can't set net dev's vap ptr to null when
          del vap wid process failed! */
        l_del_vap_flag = OAL_FALSE;
    }

    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_deinit_wlan_vap::return error code %d}\r\n", l_ret);
        if ((l_ret == -OAL_ENOMEM) || (l_ret == -OAL_EFAIL)) {
            /* wid had't processed */
            l_del_vap_flag = OAL_FALSE;
        }
    }

    if (l_del_vap_flag == OAL_TRUE) {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_deinit_wlan_vap::clear netdev priv.}\r\n");
        OAL_NET_DEV_PRIV(pst_net_dev) = NULL;
    }

    return l_ret;
}


oal_void wal_set_mac_addr(oal_net_device_stru *pst_net_dev)
{
    oal_uint8 auc_primary_mac_addr[WLAN_MAC_ADDR_LEN] = { 0 }; /* MAC��ַ */
    oal_wireless_dev_stru *pst_wdev;
    mac_wiphy_priv_stru *pst_wiphy_priv;
    mac_device_stru *pst_mac_device;

    pst_wdev = OAL_NETDEVICE_WDEV(pst_net_dev);
    pst_wiphy_priv = (mac_wiphy_priv_stru *)(oal_wiphy_priv(pst_wdev->wiphy));
    pst_mac_device = pst_wiphy_priv->pst_mac_device;

    if (OAL_UNLIKELY(pst_mac_device->st_p2p_info.pst_primary_net_device == OAL_PTR_NULL)) {
        /* random mac will be used. hi1102-cb (#include <linux/etherdevice.h>) */
        oal_random_ether_addr(auc_primary_mac_addr);
        auc_primary_mac_addr[0] &= (~0x02);
        auc_primary_mac_addr[1] = 0x11;
        auc_primary_mac_addr[2] = 0x02; /* mac��ַ��0x02��ֵ��auc_primary_mac_addr��2byte */
    } else {
#ifndef _PRE_PC_LINT
        if (OAL_LIKELY(OAL_NETDEVICE_MAC_ADDR(pst_mac_device->st_p2p_info.pst_primary_net_device) != OAL_PTR_NULL)) {
            if (memcpy_s(auc_primary_mac_addr, WLAN_MAC_ADDR_LEN,
                         OAL_NETDEVICE_MAC_ADDR(pst_mac_device->st_p2p_info.pst_primary_net_device),
                         WLAN_MAC_ADDR_LEN) != EOK) {
                OAM_ERROR_LOG0(0, OAM_SF_ANY, "wal_set_mac_addr::memcpy fail!");
                return;
            }
        } else {
            OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_set_mac_addr() pst_primary_net_device; addr is null}\r\n");
            return;
        }
#endif
    }

    switch (pst_wdev->iftype) {
        case NL80211_IFTYPE_P2P_DEVICE: {
            /* ����P2P device MAC ��ַ��������mac ��ַbit ����Ϊ1 */
            auc_primary_mac_addr[0] |= 0x02;
            oal_set_mac_addr((oal_uint8 *)OAL_NETDEVICE_MAC_ADDR(pst_net_dev), auc_primary_mac_addr);

            break;
        }
        default:
        {
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
            hwifi_get_mac_addr(auc_primary_mac_addr);
            auc_primary_mac_addr[0] &= (~0x02);
#endif
            oal_set_mac_addr((oal_uint8 *)OAL_NETDEVICE_MAC_ADDR(pst_net_dev), auc_primary_mac_addr);
            break;
        }
    }

    return;
}


oal_int32 wal_init_wlan_netdev(oal_wiphy_stru *pst_wiphy, char *dev_name)
{
    oal_net_device_stru *pst_net_dev;
    oal_wireless_dev_stru *pst_wdev;
    mac_wiphy_priv_stru *pst_wiphy_priv;
    enum nl80211_iftype en_type;
    oal_int32 l_ret;

    oal_netdev_priv_stru *pst_netdev_priv;

    if ((pst_wiphy == NULL) || (dev_name == NULL)) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_init_wlan_netdev::pst_wiphy or dev_name is null!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    if ((strcmp("wlan0", dev_name)) == 0) {
        en_type = NL80211_IFTYPE_STATION;
    } else if ((strcmp("p2p0", dev_name)) == 0) {
        en_type = NL80211_IFTYPE_P2P_DEVICE;
    } else {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_init_wlan_netdev::dev name is not wlan0 or p2p0}\r\n");
        return OAL_SUCC;
    }
    OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_init_wlan_netdev::en_type is %d, [2:wlan0;10:p2p0]}\r\n", en_type);

    /* ���������net device�Ѿ����ڣ�ֱ�ӷ��� */
    /* ����dev_name�ҵ�dev */
    pst_net_dev = oal_dev_get_by_name(dev_name);
    if (pst_net_dev != OAL_PTR_NULL) {
        /* ����oal_dev_get_by_name�󣬱������oal_dev_putʹnet_dev�����ü�����һ */
        oal_dev_put(pst_net_dev);

        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_init_wlan_netdev::the net_device is already exist!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

#if defined(_PRE_WLAN_FEATURE_OFFLOAD_FLOWCTL)
    pst_net_dev = oal_net_alloc_netdev_mqs(OAL_SIZEOF(oal_netdev_priv_stru), dev_name, oal_ether_setup,
                                           WLAN_NET_QUEUE_BUTT, 1);
#else
    pst_net_dev = oal_net_alloc_netdev(OAL_SIZEOF(oal_netdev_priv_stru), dev_name, oal_ether_setup);
#endif
    if (OAL_UNLIKELY(pst_net_dev == OAL_PTR_NULL)) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_init_wlan_netdev::oal_net_alloc_netdev return null!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_wdev =
        (oal_wireless_dev_stru *)OAL_MEM_ALLOC(OAL_MEM_POOL_ID_LOCAL, OAL_SIZEOF(oal_wireless_dev_stru), OAL_FALSE);
    if (OAL_UNLIKELY(pst_wdev == OAL_PTR_NULL)) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_init_wlan_netdev::alloc mem, pst_wdev is null ptr!}\r\n");
        oal_net_free_netdev(pst_net_dev);
        return OAL_ERR_CODE_PTR_NULL;
    }

    memset_s(pst_wdev, OAL_SIZEOF(oal_wireless_dev_stru), 0, OAL_SIZEOF(oal_wireless_dev_stru));

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    pst_net_dev->features |= NETIF_F_SG;
    pst_net_dev->hw_features |= NETIF_F_SG;
#endif /* _PRE_OS_VERSION_LINUX == _PRE_OS_VERSION */

    /* ��netdevice���и�ֵ */
#ifdef CONFIG_WIRELESS_EXT
    pst_net_dev->wireless_handlers = &g_st_iw_handler_def;
#endif
    pst_net_dev->netdev_ops = &g_st_wal_net_dev_ops;

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE) && (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    pst_net_dev->ethtool_ops = &g_st_wal_ethtool_ops;
#endif

    OAL_NETDEVICE_DESTRUCTOR(pst_net_dev) = oal_net_free_netdev;

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 10, 44))
    OAL_NETDEVICE_MASTER(pst_net_dev) = OAL_PTR_NULL;
#endif

    OAL_NETDEVICE_IFALIAS(pst_net_dev) = OAL_PTR_NULL;
    OAL_NETDEVICE_WATCHDOG_TIMEO(pst_net_dev) = 5;
    OAL_NETDEVICE_WDEV(pst_net_dev) = pst_wdev;
    OAL_NETDEVICE_QDISC(pst_net_dev, OAL_PTR_NULL);

    pst_wdev->netdev = pst_net_dev;
    pst_wdev->iftype = en_type;
    pst_wdev->wiphy = pst_wiphy;
    pst_wiphy_priv = (mac_wiphy_priv_stru *)(oal_wiphy_priv(pst_wiphy));

#ifdef _PRE_WLAN_FEATURE_P2P
    if (en_type == NL80211_IFTYPE_STATION) {
        /* �������wlan0�� �򱣴�wlan0 Ϊ��net_device,p2p0 ��p2p-p2p0 MAC ��ַ����netdevice ��ȡ */
        pst_wiphy_priv->pst_mac_device->st_p2p_info.pst_primary_net_device = pst_net_dev;
    } else if (en_type == NL80211_IFTYPE_P2P_DEVICE) {
        pst_wiphy_priv->pst_mac_device->st_p2p_info.pst_p2p_net_device = pst_net_dev;
    }
#endif
    OAL_NETDEVICE_FLAGS(pst_net_dev) &= ~OAL_IFF_RUNNING; /* ��net device��flag��Ϊdown */

    wal_set_mac_addr(pst_net_dev);

    /* NAPI pri netdevע�������net_deviceǰ�� */
    pst_netdev_priv = (oal_netdev_priv_stru *)OAL_NET_DEV_WIRELESS_PRIV(pst_net_dev);
    pst_netdev_priv->uc_napi_enable = OAL_TRUE;
    pst_netdev_priv->uc_gro_enable = OAL_TRUE;
    pst_netdev_priv->uc_napi_weight = NAPI_POLL_WEIGHT_MAX;
    pst_netdev_priv->uc_napi_dyn_weight = OAL_TRUE;
    pst_netdev_priv->uc_state = 0;
    pst_netdev_priv->ul_queue_len_max = NAPI_NETDEV_PRIV_QUEUE_LEN_MAX;
    pst_netdev_priv->ul_period_pkts = 0;
    pst_netdev_priv->ul_period_start = 0;
    oal_netbuf_list_head_init(&pst_netdev_priv->st_rx_netbuf_queue);
    /* poll������� */
    oal_netif_napi_add(pst_net_dev, &pst_netdev_priv->st_napi, hmac_rxdata_polling, NAPI_POLL_WEIGHT_MAX);

    /* ע��net_device */
    l_ret = oal_net_register_netdev(pst_net_dev);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_init_wlan_netdev::oal_net_register_netdev return error code %d!}", l_ret);

        OAL_MEM_FREE(pst_wdev, OAL_FALSE);
        oal_net_free_netdev(pst_net_dev);

        return l_ret;
    }

    return OAL_SUCC;
}


oal_int32 wal_setup_ap(oal_net_device_stru *pst_net_dev)
{
    oal_int32 l_ret;
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE) && (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    wal_set_power_mgmt_on(OAL_FALSE);
    l_ret = wal_set_power_on(pst_net_dev, OAL_TRUE);
    if (l_ret != OAL_SUCC) {
        OAM_ERROR_LOG1(0, OAM_SF_CFG, "{wal_setup_ap::wal_set_power_on fail [%d]!}", l_ret);
        return l_ret;
    }
#endif
    if (OAL_NETDEVICE_FLAGS(pst_net_dev) & OAL_IFF_RUNNING) {
        /* �л���APǰ��������豸����UP״̬����Ҫ��down wlan0�����豸 */
        OAL_IO_PRINT("wal_setup_ap:stop netdevice:%.16s", pst_net_dev->name);
        wal_netdev_stop(pst_net_dev);
    }

    pst_net_dev->ieee80211_ptr->iftype = NL80211_IFTYPE_AP;

    l_ret = wal_init_wlan_vap(pst_net_dev);
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
    if (l_ret == OAL_SUCC) {
        hwifi_config_init_ini(pst_net_dev);
    }
#endif /* #ifdef _PRE_PLAT_FEATURE_CUSTOMIZE */
    return l_ret;
}

#endif

#ifdef _PRE_WLAN_FEATURE_ARP_OFFLOAD

oal_uint32 wal_hipriv_register_inetaddr_notifier(oal_void)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    if (register_inetaddr_notifier(&wal_hipriv_notifier) == 0) {
        return OAL_SUCC;
    }

    OAM_ERROR_LOG0(0, OAM_SF_PWR, "{wal_hipriv_register_inetaddr_notifier::register inetaddr notifier failed.}");
    return OAL_FAIL;
#else
    return OAL_SUCC;
#endif
}


oal_uint32 wal_hipriv_unregister_inetaddr_notifier(oal_void)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    if (unregister_inetaddr_notifier(&wal_hipriv_notifier) == 0) {
        return OAL_SUCC;
    }

    OAM_ERROR_LOG0(0, OAM_SF_PWR, "{wal_hipriv_unregister_inetaddr_notifier::hmac_unregister \
        inetaddr notifier failed.}");
    return OAL_FAIL;
#else
    return OAL_SUCC;
#endif
}


oal_uint32 wal_hipriv_register_inet6addr_notifier(oal_void)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    if (register_inet6addr_notifier(&wal_hipriv_notifier_ipv6) == 0) {
        return OAL_SUCC;
    }

    OAM_ERROR_LOG0(0, OAM_SF_PWR, "{wal_hipriv_register_inet6addr_notifier::register inetaddr6 notifier failed.}");
    return OAL_FAIL;
#else
    return OAL_SUCC;
#endif
}


oal_uint32 wal_hipriv_unregister_inet6addr_notifier(oal_void)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    if (unregister_inet6addr_notifier(&wal_hipriv_notifier_ipv6) == 0) {
        return OAL_SUCC;
    }

    OAM_ERROR_LOG0(0, OAM_SF_PWR, "{wal_hipriv_unregister_inet6addr_notifier::hmac_unregister \
        inetaddr6 notifier failed.}");
    return OAL_FAIL;
#else
    return OAL_SUCC;
#endif
}

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)

oal_int32 wal_hipriv_inetaddr_notifier_call(struct notifier_block *this, oal_uint event, oal_void *ptr)
{
    /*
     * Notification mechanism from kernel to our driver. This function is called by the Linux kernel
     * whenever there is an event related to an IP address.
     * ptr : kernel provided pointer to IP address that has changed
 */
    struct in_ifaddr *pst_ifa = (struct in_ifaddr *)ptr;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;

    if (OAL_UNLIKELY(pst_ifa == OAL_PTR_NULL)) {
        OAM_ERROR_LOG0(0, OAM_SF_PWR, "{wal_hipriv_inetaddr_notifier_call::pst_ifa is NULL.}");
        return NOTIFY_DONE;
    }
    if (OAL_UNLIKELY(pst_ifa->ifa_dev->dev == OAL_PTR_NULL)) {
        OAM_ERROR_LOG0(0, OAM_SF_PWR, "{wal_hipriv_inet6addr_notifier_call::pst_ifa->idev->dev is NULL.}");
        return NOTIFY_DONE;
    }

    /* Filter notifications meant for non Hislicon devices */
    if (pst_ifa->ifa_dev->dev->netdev_ops != &g_st_wal_net_dev_ops) {
        return NOTIFY_DONE;
    }

    pst_mac_vap = (mac_vap_stru *)OAL_NET_DEV_PRIV(pst_ifa->ifa_dev->dev);
    if (OAL_UNLIKELY(pst_mac_vap == NULL)) {
        OAM_WARNING_LOG1(0, OAM_SF_PWR, "{wal_hipriv_inetaddr_notifier_call::Get mac vap failed, \
            when %d(UP:1 DOWN:2 UNKNOWN:others) ipv4 address.}", event);
        return NOTIFY_DONE;
    }

    wal_wake_lock();

    switch (event) {
        case NETDEV_UP: {
            OAM_WARNING_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_PWR, "{wal_hipriv_inetaddr_notifier_call:: \
                Up IPv4[%d.X.X.%d], MASK[0x%08X], vap_mode[%d].}",
                             /* ifa_address��0��3byteΪ���������ӡ */
                             ((oal_uint8 *)&(pst_ifa->ifa_address))[0], ((oal_uint8 *)&(pst_ifa->ifa_address))[3],
                             pst_ifa->ifa_mask, pst_mac_vap->en_vap_mode);
            hmac_arp_offload_set_ip_addr(pst_mac_vap, DMAC_CONFIG_IPV4, DMAC_IP_ADDR_ADD, &(pst_ifa->ifa_address),
                                         &(pst_ifa->ifa_mask));

            if (pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_STA) {
                /* ��ȡ��IP��ַ��ʱ�����͹��� */
#if _PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE
                wlan_pm_set_timeout(WLAN_SLEEP_DEFAULT_CHECK_CNT);
#endif

                hmac_tx_opt_set_ip_addr(pst_mac_vap, &(pst_ifa->ifa_address));

                /* ��ȡ��IP��ַ��ʱ��֪ͨ���μ�ʱ */
#ifdef _PRE_WLAN_FEATURE_ROAM
                pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
                if (pst_hmac_vap == OAL_PTR_NULL) {
                    OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{wal_hipriv_inetaddr_notifier_call:: \
                        pst_hmac_vap null.uc_vap_id[%d]}", pst_mac_vap->uc_vap_id);

                    wal_wake_unlock();

                    return NOTIFY_DONE;
                }

                pst_hmac_vap->uc_ip_addr_obtained_num++;
                hmac_roam_wpas_connect_state_notify(pst_hmac_vap, WPAS_CONNECT_STATE_IPADDR_OBTAINED);
#endif
            }
            break;
        }

        case NETDEV_DOWN: {
#if _PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE
            wlan_pm_set_timeout(WLAN_SLEEP_LONG_CHECK_CNT);
#endif
            OAM_WARNING_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_PWR, "{wal_hipriv_inetaddr_notifier_call:: \
                Down IPv4[%d.X.X.%d], MASK[0x%08X] vap_mode[%d]..}",
                             /* ifa_address��0��3byteΪ���������ӡ */
                             ((oal_uint8 *)&(pst_ifa->ifa_address))[0], ((oal_uint8 *)&(pst_ifa->ifa_address))[3],
                             pst_ifa->ifa_mask, pst_mac_vap->en_vap_mode);
            hmac_arp_offload_set_ip_addr(pst_mac_vap, DMAC_CONFIG_IPV4, DMAC_IP_ADDR_DEL, &(pst_ifa->ifa_address),
                                         &(pst_ifa->ifa_mask));

            if (pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_STA) {
                hmac_tx_opt_del_ip_addr(pst_mac_vap, &(pst_ifa->ifa_address));

                /* ��ȡ��IP��ַ��ʱ��֪ͨ���μ�ʱ */
#ifdef _PRE_WLAN_FEATURE_ROAM
                pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
                if (pst_hmac_vap == OAL_PTR_NULL) {
                    OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{wal_hipriv_inetaddr_notifier_call:: \
                        pst_hmac_vap null.uc_vap_id[%d]}", pst_mac_vap->uc_vap_id);

                    wal_wake_unlock();

                    return NOTIFY_DONE;
                }

                if (pst_hmac_vap->uc_ip_addr_obtained_num != 0) {
                    pst_hmac_vap->uc_ip_addr_obtained_num--;
                } else {
                    /* �쳣ά�⣬aput�µ�ʱ��vap_mode��תΪSTA���յ�down�¼�����ʱuc_ip_addr_obtained_numΪ0 */
                    OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_PWR, "wal_hipriv_inetaddr_notifier_call:: \
                        ip_obtained_num is zero, p2p_mode[%d]", pst_mac_vap->en_p2p_mode);
                }

                if (pst_hmac_vap->uc_ip_addr_obtained_num == 0) {
                    hmac_roam_wpas_connect_state_notify(pst_hmac_vap, WPAS_CONNECT_STATE_IPADDR_REMOVED);
                }
#endif
            }
            break;
        }

        default: {
            OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_PWR, "{wal_hipriv_inetaddr_notifier_call::Unknown notifier \
                event[%d].}", event);
            break;
        }
    }
    wal_wake_unlock();

    return NOTIFY_DONE;
}


oal_int32 wal_hipriv_inet6addr_notifier_call(struct notifier_block *this, oal_uint event, oal_void *ptr)
{
    /*
     * Notification mechanism from kernel to our driver. This function is called by the Linux kernel
     * whenever there is an event related to an IP address.
     * ptr : kernel provided pointer to IP address that has changed
 */
    struct inet6_ifaddr *pst_ifa = (struct inet6_ifaddr *)ptr;
    mac_vap_stru *pst_mac_vap;

    if (OAL_UNLIKELY(pst_ifa == NULL)) {
        OAM_ERROR_LOG0(0, OAM_SF_PWR, "{wal_hipriv_inet6addr_notifier_call::pst_ifa is NULL.}");
        return NOTIFY_DONE;
    }
    if (OAL_UNLIKELY(pst_ifa->idev->dev == NULL)) {
        OAM_ERROR_LOG0(0, OAM_SF_PWR, "{wal_hipriv_inet6addr_notifier_call::pst_ifa->idev->dev is NULL.}");
        return NOTIFY_DONE;
    }

    /* Filter notifications meant for non Hislicon devices */
    if (pst_ifa->idev->dev->netdev_ops != &g_st_wal_net_dev_ops) {
        return NOTIFY_DONE;
    }

    pst_mac_vap = (mac_vap_stru *)OAL_NET_DEV_PRIV(pst_ifa->idev->dev);
    if (OAL_UNLIKELY(pst_mac_vap == NULL)) {
        OAM_WARNING_LOG1(0, OAM_SF_PWR, "{wal_hipriv_inet6addr_notifier_call::Get mac vap failed, \
            when %d(UP:1 DOWN:2 UNKNOWN:others) ipv6 address.}", event);
        return NOTIFY_DONE;
    }

    switch (event) {
        case NETDEV_UP: {
            OAM_WARNING_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_PWR, "{wal_hipriv_inet6addr_notifier_call::UP \
                IPv6[%04x:%04x:XXXX:XXXX:XXXX:XXXX:%04x:%04x].}",
                             OAL_NET2HOST_SHORT((pst_ifa->addr.s6_addr16)[0]),
                             OAL_NET2HOST_SHORT((pst_ifa->addr.s6_addr16)[1]),
                             OAL_NET2HOST_SHORT((pst_ifa->addr.s6_addr16)[6]), /* s6_addr16��6��7byteΪ���������ӡ */
                             OAL_NET2HOST_SHORT((pst_ifa->addr.s6_addr16)[7]));
            hmac_arp_offload_set_ip_addr(pst_mac_vap, DMAC_CONFIG_IPV6, DMAC_IP_ADDR_ADD,
                                         &(pst_ifa->addr), &(pst_ifa->addr));
            break;
        }

        case NETDEV_DOWN: {
            OAM_WARNING_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_PWR, "{wal_hipriv_inet6addr_notifier_call::DOWN \
                IPv6[%04x:%04x:XXXX:XXXX:XXXX:XXXX:%04x:%04x].}",
                             OAL_NET2HOST_SHORT((pst_ifa->addr.s6_addr16)[0]),
                             OAL_NET2HOST_SHORT((pst_ifa->addr.s6_addr16)[1]),
                             OAL_NET2HOST_SHORT((pst_ifa->addr.s6_addr16)[6]), /* s6_addr16��6��7byteΪ���������ӡ */
                             OAL_NET2HOST_SHORT((pst_ifa->addr.s6_addr16)[7]));
            hmac_arp_offload_set_ip_addr(pst_mac_vap, DMAC_CONFIG_IPV6, DMAC_IP_ADDR_DEL,
                                         &(pst_ifa->addr), &(pst_ifa->addr));
            break;
        }

        default: {
            OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_PWR, "{wal_hipriv_inet6addr_notifier_call:Unknown notifier \
                event[%d].}", event);
            break;
        }
    }

    return NOTIFY_DONE;
}
#endif /* #if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION) */
#endif /* #ifdef _PRE_WLAN_FEATURE_ARP_OFFLOAD */
#ifdef _PRE_WLAN_FEATURE_11K

OAL_STATIC oal_uint32 wal_hipriv_send_neighbor_req(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    wal_msg_write_stru st_write_msg;
    oal_uint32 ul_offset = 0;
    oal_uint32 ul_ret;
    oal_int32 l_ret;
    oal_int8 ac_arg[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    mac_cfg_ssid_param_stru *pst_ssid;
    oal_uint8 uc_str_len;

    uc_str_len = OS_STR_LEN(pc_param);
    uc_str_len = (uc_str_len > 1) ? uc_str_len - 1 : uc_str_len;

    /* ��ȡSSID�ַ��� */
    if (uc_str_len != 0) {
        ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, OAL_SIZEOF(ac_arg), &ul_offset);
        if (ul_ret != OAL_SUCC) {
            OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_hipriv_send_neighbor_req:wal_get_cmd_one_arg fail, ret=[%d]", ul_ret);
            return ul_ret;
        }
    }

    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SEND_NEIGHBOR_REQ, OAL_SIZEOF(mac_cfg_ssid_param_stru));
    pst_ssid = (mac_cfg_ssid_param_stru *)st_write_msg.auc_value;
    pst_ssid->uc_ssid_len = uc_str_len;
    if (memcpy_s(pst_ssid->ac_ssid, WLAN_SSID_MAX_LEN, ac_arg, pst_ssid->uc_ssid_len) != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "wal_hipriv_send_neighbor_req::memcpy fail!");
        return OAL_FAIL;
    }

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_ssid_param_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_send_neighbor_req::return err code [%d]!}\r\n", l_ret);
        return (oal_uint32)l_ret;
    }
    return OAL_SUCC;
}

OAL_STATIC oal_uint32 wal_hipriv_beacon_req_table_switch(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    wal_msg_write_stru st_write_msg;
    oal_uint32 ul_offset = 0;
    oal_uint32 ul_ret;
    oal_int32 l_ret;
    oal_int8 ac_arg[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    oal_uint8 uc_switch;

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, OAL_SIZEOF(ac_arg), &ul_offset);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_hipriv_beacon_req_table_switch:wal_get_cmd_one_arg return err_code [%d]}",
                         ul_ret);
        return ul_ret;
    }

    uc_switch = (oal_uint8)oal_atoi(ac_arg);

    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_BCN_TABLE_SWITCH, OAL_SIZEOF(oal_uint8));
    st_write_msg.auc_value[0] = uc_switch;
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(oal_uint8),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_beacon_req_table_switch::return err code [%d]!}\r\n", l_ret);
        return (oal_uint32)l_ret;
    }
    return OAL_SUCC;
}
#endif  // _PRE_WLAN_FEATURE_11K


OAL_STATIC oal_uint32 wal_hipriv_voe_enable(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    wal_msg_write_stru st_write_msg;
    oal_uint32 ul_offset = 0;
    oal_uint32 ul_ret;
    oal_int32 l_ret;
    oal_int8 ac_arg[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    oal_uint16 us_switch;
    oal_uint16 *pus_value = OAL_PTR_NULL;

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, OAL_SIZEOF(ac_arg), &ul_offset);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_voe_enable::wal_get_cmd_one_arg return err_code [%d]!}", ul_ret);
        return ul_ret;
    }

    us_switch = (oal_uint16)oal_atoi(ac_arg);
    us_switch = us_switch & 0xFFFF;

    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_VOE_ENABLE, OAL_SIZEOF(oal_uint16));
    pus_value = (oal_uint16 *)st_write_msg.auc_value;
    *pus_value = us_switch;
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(oal_uint16),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_voe_enable::return err code [%d]!}\r\n", l_ret);
        return (oal_uint32)l_ret;
    }
    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_VOWIFI

OAL_STATIC oal_int32 wal_ioctl_set_vowifi_param(
    oal_net_device_stru *pst_net_dev, oal_int8 *puc_command, wal_wifi_priv_cmd_stru st_priv_cmd)
{
    oal_int32 l_ret;
    oal_uint16 us_len;
    wal_msg_write_stru st_write_msg;
    mac_cfg_vowifi_stru *pst_cfg_vowifi;
    mac_vowifi_cmd_enum_uint8 en_vowifi_cmd_id;
    oal_uint8 uc_param;
    oal_uint8 ul_cfg_id;

    /* vapδ����ʱ��������supplicant���� */
    if (OAL_NET_DEV_PRIV(pst_net_dev) == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_CFG, "wal_ioctl_set_vowifi_param::vap not created yet, ignore the cmd!");
        return -OAL_EINVAL;
    }

    for (ul_cfg_id = VOWIFI_SET_MODE; ul_cfg_id < VOWIFI_CMD_BUTT; ul_cfg_id++) {
        if ((oal_strncmp(puc_command, g_ast_vowifi_cmd_table[ul_cfg_id].pc_priv_cmd,
                         OAL_STRLEN(g_ast_vowifi_cmd_table[ul_cfg_id].pc_priv_cmd)) == 0) &&
            (st_priv_cmd.ul_total_len > (OAL_STRLEN(g_ast_vowifi_cmd_table[ul_cfg_id].pc_priv_cmd) + 1))) {
            uc_param = (oal_uint8)oal_atoi((oal_int8 *)puc_command +
                                           OAL_STRLEN((oal_uint8 *)g_ast_vowifi_cmd_table[ul_cfg_id].pc_priv_cmd) + 1);
            en_vowifi_cmd_id = g_ast_vowifi_cmd_table[ul_cfg_id].ul_case_entry;
            break;
        }
    }
    if (ul_cfg_id >= VOWIFI_CMD_BUTT) {
        OAM_WARNING_LOG0(0, OAM_SF_CFG, "wal_ioctl_set_vowifi_param::invalid cmd!");
        return -OAL_EINVAL;
    }

    OAM_WARNING_LOG2(0, OAM_SF_ANY, "{wal_ioctl_set_vowifi_param::supplicant set VoWiFi_param cmd(%d), value[%d] }",
                     en_vowifi_cmd_id, uc_param);

    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    us_len = OAL_SIZEOF(mac_cfg_vowifi_stru);
    pst_cfg_vowifi = (mac_cfg_vowifi_stru *)(st_write_msg.auc_value);
    pst_cfg_vowifi->en_vowifi_cfg_cmd = en_vowifi_cmd_id;
    pst_cfg_vowifi->uc_value = uc_param;

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_VOWIFI_INFO, us_len);
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + us_len,
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "wal_ioctl_set_vowifi_param::wal_send_cfg_event failed, error no[%d]", l_ret);
        return l_ret;
    }
    return OAL_SUCC;
}

OAL_STATIC oal_int32 wal_ioctl_get_vowifi_param(
    oal_net_device_stru *pst_net_dev, oal_int8 *puc_command, oal_int32 *pl_value)
{
    mac_vap_stru *pst_mac_vap;

    /* vapδ����ʱ��������supplicant���� */
    if (OAL_NET_DEV_PRIV(pst_net_dev) == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_CFG, "wal_ioctl_get_vowifi_param::vap not created yet, ignore the cmd!");
        return -OAL_EINVAL;
    }

    /* ��ȡmac_vap */
    pst_mac_vap = OAL_NET_DEV_PRIV(pst_net_dev);
    if (pst_mac_vap->pst_vowifi_cfg_param == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_CFG, "{wal_ioctl_get_vowifi_param::pst_vowifi_cfg_param is null.}");
        return OAL_SUCC;
    }

    if (oal_strncmp(puc_command, CMD_VOWIFI_GET_MODE, OAL_STRLEN(CMD_VOWIFI_GET_MODE)) == 0) {
        *pl_value = (oal_int32)pst_mac_vap->pst_vowifi_cfg_param->en_vowifi_mode;
    } else if (oal_strncmp(puc_command, CMD_VOWIFI_GET_PERIOD, OAL_STRLEN(CMD_VOWIFI_GET_PERIOD)) == 0) {
        *pl_value = (oal_int32)pst_mac_vap->pst_vowifi_cfg_param->us_rssi_period_ms / 1000; /* 1s����1000ms */
    } else if (oal_strncmp(puc_command, CMD_VOWIFI_GET_LOW_THRESHOLD, OAL_STRLEN(CMD_VOWIFI_GET_LOW_THRESHOLD)) == 0) {
        *pl_value = (oal_int32)pst_mac_vap->pst_vowifi_cfg_param->c_rssi_low_thres;
    } else if (oal_strncmp(puc_command, CMD_VOWIFI_GET_HIGH_THRESHOLD, OAL_STRLEN(CMD_VOWIFI_GET_HIGH_THRESHOLD)) ==
    0) {
        *pl_value = (oal_int32)pst_mac_vap->pst_vowifi_cfg_param->c_rssi_high_thres;
    } else if (oal_strncmp(puc_command, CMD_VOWIFI_GET_TRIGGER_COUNT, OAL_STRLEN(CMD_VOWIFI_GET_TRIGGER_COUNT)) == 0) {
        *pl_value = (oal_int32)pst_mac_vap->pst_vowifi_cfg_param->uc_trigger_count_thres;
    } else {
        OAM_WARNING_LOG0(0, OAM_SF_CFG, "wal_ioctl_get_vowifi_param::invalid cmd!");
        *pl_value = 0xffffffff;
        return -OAL_EINVAL;
    }

    OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_ioctl_get_vowifi_param::supplicant get VoWiFi_param value[%d] }", *pl_value);

    return OAL_SUCC;
}

#endif /* _PRE_WLAN_FEATURE_VOWIFI */
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)

OAL_STATIC oal_int32 wal_ioctl_get_wifi_priv_feature_cap_param(
    oal_net_device_stru *pst_net_dev, oal_int8 *puc_command, oal_int32 *pl_value)
{
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    oal_uint32 ul_value = 0;

    /* vapδ����ʱ��������supplicant���� */
    if (OAL_NET_DEV_PRIV(pst_net_dev) == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_CFG, "wal_ioctl_get_wifi_priv_feature_cap_param:vap not created, ignore the cmd");
        return -OAL_EINVAL;
    }

    /* ��ȡmac_vap */
    pst_mac_vap = OAL_NET_DEV_PRIV(pst_net_dev);
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_CFG, "{wal_ioctl_get_wifi_priv_feature_cap_param::netdevice->mac_vap is null.}");
        return OAL_SUCC;
    }

    *pl_value = 0;
    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_CFG, "{wal_ioctl_get_wifi_priv_feature_cap_param::pst_hmac_vap is null.}");
        return OAL_SUCC;
    }

#if defined(_PRE_WLAN_FEATURE_11K) || defined(_PRE_WLAN_FEATURE_11R)
    /* 11k���� */
    if (pst_hmac_vap->bit_11k_enable == OAL_TRUE) {
        ul_value |= BIT(WAL_WIFI_FEATURE_SUPPORT_11K);
    }

    /* 11v���� */
    if (pst_hmac_vap->bit_11v_enable == OAL_TRUE) {
        ul_value |= BIT(WAL_WIFI_FEATURE_SUPPORT_11V);
    }

    /* 11r���� */
    if (pst_hmac_vap->bit_11r_enable == OAL_TRUE) {
        ul_value |= BIT(WAL_WIFI_FEATURE_SUPPORT_11R);
    }
#endif
    ul_value |= BIT(WAL_WIFI_FEATURE_SUPPORT_VOWIFI_NAT_KEEP_ALIVE);

    *pl_value = (oal_int32)ul_value;

    return OAL_SUCC;
}
#endif


oal_int32 wal_config_send_event(oal_net_device_stru *pst_net_dev, wlan_cfgid_enum_uint16 us_cfgid, oal_uint8 *puc_param,
                                oal_uint16 us_len)
{
    oal_int32 l_ret;
    wal_msg_write_stru st_write_msg = { 0 };

    /* ��д msg ��Ϣͷ */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, us_cfgid, us_len);

    /* �������netbuf�׵�ַ��д��msg��Ϣ���� */
    l_ret = memcpy_s(st_write_msg.auc_value, OAL_SIZEOF(st_write_msg.auc_value), puc_param, us_len);
    if (l_ret != EOK) {
        OAM_ERROR_LOG1(0, OAM_SF_CFG, "wal_config_send_event::memcpy fail! cfgid [%d]", us_cfgid);
        return OAL_FAIL;
    }

    /* ������Ϣ */
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + us_len,
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_ERROR_LOG2(0, OAM_SF_CFG, "{wal_config_send_event:wal_send_cfg_event failed, error no[%d], cfg_id [%d]!}",
                       l_ret, us_cfgid);
        return l_ret;
    }

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_IP_FILTER

oal_int32 wal_set_ip_filter_enable(oal_int32 l_on)
{
    oal_uint16 us_len;
    oal_int32 l_ret;
    oal_uint32 ul_netbuf_len;
    oal_netbuf_stru *pst_netbuf = OAL_PTR_NULL;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    oal_net_device_stru *pst_net_dev = OAL_PTR_NULL;
    wal_msg_write_stru st_write_msg;
    mac_ip_filter_cmd_stru st_ip_filter_cmd;
    mac_ip_filter_cmd_stru *pst_cmd_info = OAL_PTR_NULL;
    oal_int32 l_memcpy_ret;

#ifdef _PRE_WLAN_FEATURE_DFR
    if (g_st_dfr_info.bit_device_reset_process_flag) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_set_ip_filter_enable:: dfr_process_status[%d]!}",
                         g_st_dfr_info.bit_device_reset_process_flag);
        return -OAL_EFAIL;
    }
#endif  // #ifdef _PRE_WLAN_FEATURE_DFR

    if (l_on < 0) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "{wal_set_ip_filter_enable::Invalid input parameter, on/off %d!}", l_on);
        return -OAL_EINVAL;
    }

    pst_net_dev = oal_dev_get_by_name("wlan0");
    if (pst_net_dev == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_set_ip_filter_enable::wlan0 not exist!}");
        return -OAL_EINVAL;
    }
    /* ����oal_dev_get_by_name�󣬱������oal_dev_putʹnet_dev�����ü�����һ */
    oal_dev_put(pst_net_dev);

    /* vapδ����ʱ���������·������� */
    pst_mac_vap = OAL_NET_DEV_PRIV(pst_net_dev);
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_set_ip_filter_enable::vap not created yet, ignore the cmd!}");
        return -OAL_EINVAL;
    }

    if (pst_mac_vap->st_cap_flag.bit_ip_filter != OAL_TRUE) {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "wal_set_ip_filter_enable:Func not enable,ignore");
        return -OAL_EINVAL;
    }

    /* ׼���������� */
    ul_netbuf_len = OAL_SIZEOF(st_ip_filter_cmd);
    memset_s((oal_uint8 *)&st_ip_filter_cmd, ul_netbuf_len, 0, ul_netbuf_len);
    st_ip_filter_cmd.en_cmd = MAC_IP_FILTER_ENABLE;
    st_ip_filter_cmd.en_enable = (l_on > 0) ? OAL_TRUE : OAL_FALSE;

    OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_set_ip_filter_enable::IP_filter on/off(%d).}",
                     st_ip_filter_cmd.en_enable);

    /* ����ռ� ������˹��� */
    pst_netbuf = OAL_MEM_NETBUF_ALLOC(OAL_NORMAL_NETBUF, ul_netbuf_len, OAL_NETBUF_PRIORITY_MID);
    if (pst_netbuf == OAL_PTR_NULL) {
        OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_set_ip_filter_enable::netbuf alloc null, size %d.}",
                       ul_netbuf_len);
        return -OAL_EINVAL;
    }
    memset_s(((oal_uint8 *)OAL_NETBUF_DATA(pst_netbuf)), ul_netbuf_len, 0, ul_netbuf_len);
    pst_cmd_info = (mac_ip_filter_cmd_stru *)OAL_NETBUF_DATA(pst_netbuf);

    /* ��¼���˹��� */
    l_memcpy_ret = memcpy_s((oal_uint8 *)pst_cmd_info, ul_netbuf_len, (oal_uint8 *)(&st_ip_filter_cmd), ul_netbuf_len);
    oal_netbuf_put(pst_netbuf, ul_netbuf_len);

    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    us_len = OAL_SIZEOF(oal_netbuf_stru *);

    /* ��д msg ��Ϣͷ */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_IP_FILTER, us_len);

    /* �������netbuf�׵�ַ��д��msg��Ϣ���� */
    l_memcpy_ret += memcpy_s(st_write_msg.auc_value, us_len, (oal_uint8 *)&pst_netbuf, us_len);
    if (l_memcpy_ret != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "wal_set_ip_filter_enable::memcpy fail!");
        oal_netbuf_free(pst_netbuf);
        return -OAL_EINVAL;
    }

    /* ������Ϣ */
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + us_len,
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "wal_set_ip_filter_enable:wal_send_cfg_event failed, ret=%d",
                       l_ret);
        oal_netbuf_free(pst_netbuf);
        return l_ret;
    }

    return OAL_SUCC;
}

oal_int32 wal_set_assigned_filter_enable(oal_int32 l_filter_id, oal_int32 l_switch)
{
    oal_uint16 us_len;
    oal_net_device_stru *pst_net_dev = OAL_PTR_NULL;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    mac_assigned_filter_cmd_stru st_assigned_filter_cmd;

#ifdef _PRE_WLAN_FEATURE_DFR
    if (g_st_dfr_info.bit_device_reset_process_flag) {
        OAM_WARNING_LOG1(0, OAM_SF_CFG, "{wal_set_assigned_filter_enable:: dfr_process_status[%d]!}",
                         g_st_dfr_info.bit_device_reset_process_flag);
        return -OAL_EFAIL;
    }
#endif

    if ((l_filter_id < 0) || (l_switch < 0)) {
        OAM_WARNING_LOG2(0, OAM_SF_CFG, "{wal_set_assigned_filter_enable::params error! filter_id [%d], switch [%d]}",
                         l_filter_id, l_switch);
        return -OAL_EINVAL;
    }

    pst_net_dev = oal_dev_get_by_name("wlan0");
    if (pst_net_dev == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_CFG, "{wal_set_assigned_filter_enable::wlan0 not exist!}");
        return -OAL_EINVAL;
    }
    /* ����oal_dev_get_by_name��,�������oal_dev_putʹnet_dev�����ü���-1 */
    oal_dev_put(pst_net_dev);

    /* vapδ����ʱ,�������·������� */
    pst_mac_vap = OAL_NET_DEV_PRIV(pst_net_dev);
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_CFG, "{wal_set_assigned_filter_enable::vap not created yet, ignore the cmd!}");
        return -OAL_EINVAL;
    }

    if (pst_mac_vap->st_cap_flag.bit_icmp_filter != OAL_TRUE) {
        OAM_WARNING_LOG2(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{wal_set_assigned_filter_enable::vap not support, \
            ignore the cmd! vap mode [%d], p2p mode [%d].}", pst_mac_vap->en_vap_mode, pst_mac_vap->en_p2p_mode);
        return -OAL_EINVAL;
    }

    /* ׼���������� */
    us_len = OAL_SIZEOF(mac_assigned_filter_cmd_stru);
    memset_s((oal_uint8 *)&st_assigned_filter_cmd, us_len, 0, us_len);
    st_assigned_filter_cmd.uc_filter_id = (mac_assigned_filter_id_enum)l_filter_id;
    st_assigned_filter_cmd.en_enable = (l_switch > 0) ? OAL_TRUE : OAL_FALSE;

    OAM_WARNING_LOG2(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
                     "{wal_set_assigned_filter_enable::assigned_filter filter_id [%d], switch [%d].}",
                     l_filter_id, st_assigned_filter_cmd.en_enable);

    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    if (wal_config_send_event(pst_net_dev, WLAN_CFGID_ASSIGNED_FILTER,
                              (oal_uint8 *)&st_assigned_filter_cmd, us_len) != OAL_SUCC) {
        return -OAL_EINVAL;
    }

    return OAL_SUCC;
}


oal_int32 wal_add_ip_filter_items(wal_hw_wifi_filter_item *pst_items, oal_int32 l_count)
{
    oal_uint16 us_len;
    oal_int32 l_ret;
    oal_uint32 ul_netbuf_len;
    oal_uint32 ul_items_idx;
    oal_netbuf_stru *pst_netbuf = OAL_PTR_NULL;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    oal_net_device_stru *pst_net_dev = OAL_PTR_NULL;
    wal_msg_write_stru st_write_msg;
    mac_ip_filter_cmd_stru st_ip_filter_cmd;
    mac_ip_filter_cmd_stru *pst_cmd_info = OAL_PTR_NULL;
    oal_int32 l_memcpy_ret;

#ifdef _PRE_WLAN_FEATURE_DFR
    if (g_st_dfr_info.bit_device_reset_process_flag) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_add_ip_filter_items:: dfr_process_status[%d]!}",
                         g_st_dfr_info.bit_device_reset_process_flag);
        return -OAL_EFAIL;
    }
#endif  // #ifdef _PRE_WLAN_FEATURE_DFR

    if ((pst_items == OAL_PTR_NULL) || (l_count <= 0)) {
        OAM_ERROR_LOG2(0, OAM_SF_ANY, "{wal_add_ip_filter_items::Invalid input parameter, pst_items %p, l_count %d!}",
                       (uintptr_t)pst_items, l_count);
        return -OAL_EINVAL;
    }

    pst_net_dev = oal_dev_get_by_name("wlan0");
    if (pst_net_dev == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_add_ip_filter_items::wlan0 not exist!}");
        return -OAL_EINVAL;
    }
    /* ����oal_dev_get_by_name�󣬱������oal_dev_putʹnet_dev�����ü�����һ */
    oal_dev_put(pst_net_dev);

    /* vapδ����ʱ���������·������� */
    pst_mac_vap = OAL_NET_DEV_PRIV(pst_net_dev);
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_add_ip_filter_items::vap not created yet, ignore the cmd!.}");
        return -OAL_EINVAL;
    }

    if (pst_mac_vap->st_cap_flag.bit_ip_filter != OAL_TRUE) {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "wal_add_ip_filter_items:Func not enable, ignore the cmd");
        return -OAL_EINVAL;
    }

    /* ׼�������¼� */
    memset_s((oal_uint8 *)&st_ip_filter_cmd, OAL_SIZEOF(st_ip_filter_cmd), 0, OAL_SIZEOF(st_ip_filter_cmd));
    st_ip_filter_cmd.en_cmd = MAC_IP_FILTER_UPDATE_BTABLE;

    /* ���ڱ���������С���ƣ�ȡ�����ɵĹ�����Ŀ����Сֵ */
    st_ip_filter_cmd.uc_item_count =
        OAL_MIN((MAC_MAX_IP_FILTER_BTABLE_SIZE / OAL_SIZEOF(mac_ip_filter_item_stru)), l_count);
    if (st_ip_filter_cmd.uc_item_count < l_count) {
        OAM_WARNING_LOG2(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_add_ip_filter_items::Btable(%d) is too small to \
            store %d items!}", st_ip_filter_cmd.uc_item_count, l_count);
    }

    OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "wal_add_ip_filter_items:Start updating btable, items_cnt(%d)",
                     st_ip_filter_cmd.uc_item_count);

    /* ѡ�������¼��ռ�Ĵ�С */
    ul_netbuf_len =
        (st_ip_filter_cmd.uc_item_count * OAL_SIZEOF(mac_ip_filter_item_stru)) + OAL_SIZEOF(st_ip_filter_cmd);

    /* ����ռ� ������˹��� */
    pst_netbuf = OAL_MEM_NETBUF_ALLOC(OAL_NORMAL_NETBUF, ul_netbuf_len, OAL_NETBUF_PRIORITY_MID);
    if (pst_netbuf == OAL_PTR_NULL) {
        OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_add_ip_filter_items::netbuf alloc null, size %d.}",
                       ul_netbuf_len);
        return -OAL_EINVAL;
    }
    memset_s(((oal_uint8 *)OAL_NETBUF_DATA(pst_netbuf)), ul_netbuf_len, 0, ul_netbuf_len);
    pst_cmd_info = (mac_ip_filter_cmd_stru *)OAL_NETBUF_DATA(pst_netbuf);

    /* ��¼���˹��� */
    l_memcpy_ret = memcpy_s((oal_uint8 *)pst_cmd_info, ul_netbuf_len,
                            (oal_uint8 *)(&st_ip_filter_cmd), OAL_SIZEOF(st_ip_filter_cmd));
    oal_netbuf_put(pst_netbuf, ul_netbuf_len);

    for (ul_items_idx = 0; ul_items_idx < st_ip_filter_cmd.uc_item_count; ul_items_idx++) {
        pst_cmd_info->ast_filter_items_items[ul_items_idx].uc_protocol = (oal_uint8)pst_items[ul_items_idx].protocol;
        pst_cmd_info->ast_filter_items_items[ul_items_idx].us_port = (oal_uint16)pst_items[ul_items_idx].port;
    }

    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    us_len = OAL_SIZEOF(oal_netbuf_stru *);

    /* ��д msg ��Ϣͷ */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_IP_FILTER, us_len);

    /* �������netbuf�׵�ַ��д��msg��Ϣ���� */
    l_memcpy_ret += memcpy_s(st_write_msg.auc_value, us_len, (oal_uint8 *)&pst_netbuf, us_len);
    if (l_memcpy_ret != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "wal_add_ip_filter_items::memcpy fail!");
        oal_netbuf_free(pst_netbuf);
        return -OAL_EINVAL;
    }

    /* ������Ϣ */
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + us_len,
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "wal_add_ip_filter_items:wal_send_cfg_event failed, \
            l_ret=%d", l_ret);
        oal_netbuf_free(pst_netbuf);
        return l_ret;
    }

    return OAL_SUCC;
}

oal_int32 wal_clear_ip_filter()
{
    oal_uint16 us_len;
    oal_int32 l_ret;
    oal_uint32 ul_netbuf_len;
    oal_netbuf_stru *pst_netbuf = OAL_PTR_NULL;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    oal_net_device_stru *pst_net_dev;
    wal_msg_write_stru st_write_msg;
    mac_ip_filter_cmd_stru st_ip_filter_cmd;
    mac_ip_filter_cmd_stru *pst_cmd_info = OAL_PTR_NULL;
    oal_int32 l_memcpy_ret;

#ifdef _PRE_WLAN_FEATURE_DFR
    if (g_st_dfr_info.bit_device_reset_process_flag) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_clear_ip_filter:: dfr_process_status[%d]!}",
                         g_st_dfr_info.bit_device_reset_process_flag);
        return -OAL_EFAIL;
    }
#endif  // #ifdef _PRE_WLAN_FEATURE_DFR

    pst_net_dev = oal_dev_get_by_name("wlan0");
    if (pst_net_dev == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_clear_ip_filter::wlan0 not exist!}");
        return -OAL_EINVAL;
    }

    /* ����oal_dev_get_by_name�󣬱������oal_dev_putʹnet_dev�����ü�����һ */
    oal_dev_put(pst_net_dev);

    /* vapδ����ʱ���������·������� */
    pst_mac_vap = OAL_NET_DEV_PRIV(pst_net_dev);
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_clear_ip_filter::vap not created yet, ignore the cmd!.}");
        return -OAL_EINVAL;
    }

    if (pst_mac_vap->st_cap_flag.bit_ip_filter != OAL_TRUE) {
        OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "wal_clear_ip_filter::Func not enable, ignore the cmd!");
        return -OAL_EINVAL;
    }

    /* ��������� */
    memset_s((oal_uint8 *)&st_ip_filter_cmd, OAL_SIZEOF(st_ip_filter_cmd), 0, OAL_SIZEOF(st_ip_filter_cmd));
    st_ip_filter_cmd.en_cmd = MAC_IP_FILTER_CLEAR;

    OAM_WARNING_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_clear_ip_filter::Now start clearing the list.}");

    /* ѡ�������¼��ռ�Ĵ�С */
    ul_netbuf_len = OAL_SIZEOF(st_ip_filter_cmd);

    /* ����ռ� ������˹��� */
    pst_netbuf = OAL_MEM_NETBUF_ALLOC(OAL_NORMAL_NETBUF, ul_netbuf_len, OAL_NETBUF_PRIORITY_MID);
    if (pst_netbuf == OAL_PTR_NULL) {
        OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_clear_ip_filter::netbuf alloc null, size %d.}",
                       ul_netbuf_len);
        return -OAL_EINVAL;
    }
    memset_s(((oal_uint8 *)OAL_NETBUF_DATA(pst_netbuf)), ul_netbuf_len, 0, ul_netbuf_len);
    pst_cmd_info = (mac_ip_filter_cmd_stru *)OAL_NETBUF_DATA(pst_netbuf);

    /* ��¼���˹��� */
    l_memcpy_ret = memcpy_s((oal_uint8 *)pst_cmd_info, ul_netbuf_len, (oal_uint8 *)(&st_ip_filter_cmd), ul_netbuf_len);
    oal_netbuf_put(pst_netbuf, ul_netbuf_len);

    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    us_len = OAL_SIZEOF(oal_netbuf_stru *);

    /* ��д msg ��Ϣͷ */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_IP_FILTER, us_len);

    /* �������netbuf�׵�ַ��д��msg��Ϣ���� */
    l_memcpy_ret += memcpy_s(st_write_msg.auc_value, us_len, (oal_uint8 *)&pst_netbuf, us_len);
    if (l_memcpy_ret != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "wal_clear_ip_filter::memcpy fail!");
        oal_netbuf_free(pst_netbuf);
        return -OAL_EINVAL;
    }

    /* ������Ϣ */
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + us_len,
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "wal_clear_ip_filter:wal_send_cfg_event failed, l_ret=%d",
                       l_ret);
        oal_netbuf_free(pst_netbuf);
        return l_ret;
    }

    return OAL_SUCC;
}
oal_int32 wal_register_ip_filter(wal_hw_wlan_filter_ops *pst_ip_filter_ops)
{
#ifdef CONFIG_DOZE_FILTER
    if (pst_ip_filter_ops == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_register_ip_filter::pg_st_ip_filter_ops is null !}");
        return -OAL_EINVAL;
    }
    hw_register_wlan_filter(pst_ip_filter_ops);
#else
    OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_register_ip_filter:: Not support CONFIG_DOZE_FILTER!}");
#endif
    return OAL_SUCC;
}

oal_int32 wal_unregister_ip_filter()
{
#ifdef CONFIG_DOZE_FILTER
    hw_unregister_wlan_filter();
#else
    OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_unregister_ip_filter:: Not support CONFIG_DOZE_FILTER!}");
#endif
    return OAL_SUCC;
}

#else
oal_int32 wal_set_ip_filter_enable(oal_int32 l_on)
{
    OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_set_ip_filter_enable::Ip_filter not support!}");
    return -OAL_EFAIL;
}

oal_int32 wal_set_assigned_filter_enable(oal_int32 l_filter_id, oal_int32 l_switch)
{
    OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_set_assigned_filter_enable::Assigned_filter not support!}");
    return -OAL_EFAIL;
}

oal_int32 wal_add_ip_filter_items(wal_hw_wifi_filter_item *pst_items, oal_int32 l_count)
{
    OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_add_ip_filter_items::Ip_filter not support!}");
    return -OAL_EFAIL;
}

oal_int32 wal_clear_ip_filter()
{
    OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_clear_ip_filter::Ip_filter not support!}");

    return -OAL_EFAIL;
}

#endif  // _PRE_WLAN_FEATURE_IP_FILTER

/*lint -e19*/
oal_module_symbol(wal_hipriv_proc_write);
oal_module_symbol(wal_hipriv_get_mac_addr);
/*lint +e19*/
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)

OAL_STATIC oal_uint32 wal_hipriv_set_pm_debug_switch(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    wal_msg_write_stru st_write_msg;
    mac_vap_stru *pst_mac_vap;
    oal_uint32 ul_ret = 0;
    oal_int32 l_ret;
    oal_uint32 ul_offset;
    oal_int8 ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    oal_int8 ac_value[WAL_HIPRIV_CMD_VALUE_MAX_LEN];
    mac_pm_debug_cfg_stru *pst_pm_debug_cfg = OAL_PTR_NULL;
    oal_uint8 uc_switch;
    oal_bool_enum_uint8 en_cmd_updata = OAL_FALSE;
    oal_uint16 us_val = 0;

    /* sh hipriv.sh "wlan0 pm_debug srb " */
    pst_mac_vap = OAL_NET_DEV_PRIV(pst_net_dev);
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_hipriv_set_pm_debug_switch::pst_mac_vap is null!}");
        return OAL_FAIL;
    }

    pst_pm_debug_cfg = (mac_pm_debug_cfg_stru *)(st_write_msg.auc_value);

    memset_s(pst_pm_debug_cfg, OAL_SIZEOF(mac_pm_debug_cfg_stru), 0, OAL_SIZEOF(mac_pm_debug_cfg_stru));

    do {
        ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, OAL_SIZEOF(ac_name), &ul_offset);
        if ((ul_ret != OAL_SUCC) && (ul_offset != 0)) {
            OAM_WARNING_LOG1(0, OAM_SF_CFG, "{wal_hipriv_set_pm_debug_switch::cmd format err, ret:%d;!!}\r\n", ul_ret);
            return ul_ret;
        }
        pc_param += ul_offset;

        if (en_cmd_updata == OAL_FALSE) {
            en_cmd_updata = OAL_TRUE;
        } else if (ul_offset == 0) {
            break;
        }

        ul_ret = wal_get_cmd_one_arg(pc_param, ac_value, OAL_SIZEOF(ac_value), &ul_offset);
        if ((ul_ret != OAL_SUCC) || ((!isdigit(ac_value[0])) && (strcmp("help", ac_value) != 0))) {
            OAL_IO_PRINT("CMD format::sh hipriv.sh 'wlan0 pm_debug [srb 0|1]'\r\n");
            OAM_ERROR_LOG0(0, OAM_SF_CFG, "{CMD format::sh hipriv.sh 'wlan0 pm_debug [srb 0|1]'!!}\r\n");
            return ul_ret;
        }
        pc_param += ul_offset;
#ifdef _PRE_WLAN_FEATURE_APF
        if (strcmp("apf", ac_name) == 0) {
            if (strcmp("help", ac_value) == 0) {
                OAM_WARNING_LOG0(0, OAM_SF_CFG, "{CMD format::sh hipriv.sh 'wlan0 pm_debug apf [0|1]'}");
                return OAL_SUCC;
            }

            uc_switch = (oal_uint8)oal_atoi(ac_value);
            if (uc_switch > 1) {
                OAM_ERROR_LOG1(0, OAM_SF_CFG, "CMD format:hipriv.sh 'wlan0 pm_debug apf [0|1]', input[%d]", uc_switch);
                return OAL_FAIL;
            }
            pst_pm_debug_cfg->ul_cmd_bit_map |= BIT(MAC_PM_DEBUG_APF);
            pst_pm_debug_cfg->uc_apf_switch = uc_switch;

            OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{wal_hipriv_set_pm_debug_switch::apf switch[%d].}",
                             uc_switch);
        } else if (strcmp("ao", ac_name) == 0) {
            if (strcmp("help", ac_value) == 0) {
                OAM_WARNING_LOG0(0, OAM_SF_CFG, "{CMD format::sh hipriv.sh 'wlan0 pm_debug ao [0|1]'}");
                return OAL_SUCC;
            }

            uc_switch = (oal_uint8)oal_atoi(ac_value);
            if (uc_switch > 1) {
                OAM_ERROR_LOG1(0, OAM_SF_CFG, "{CMD format::sh hipriv.sh 'wlan0 pm_debug ao [0|1]', input[%d]!!}",
                               uc_switch);
                return OAL_FAIL;
            }
            pst_pm_debug_cfg->ul_cmd_bit_map |= BIT(MAC_PM_DEBUG_AO);
            pst_pm_debug_cfg->uc_ao_switch = uc_switch;

            OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "wal_hipriv_set_pm_debug_switch:arp offload switch=%d",
                             uc_switch);
        }
#endif
#ifdef _PRE_PM_DYN_SET_TBTT_OFFSET
        else if (strcmp("dto", ac_name) == 0) {
            if (strcmp("help", ac_value) == 0) {
                OAM_WARNING_LOG0(0, OAM_SF_CFG, "{CMD format::sh hipriv.sh 'wlan0 pm_debug dto [0|1]'}");
                return OAL_SUCC;
            }

            uc_switch = (oal_uint8)oal_atoi(ac_value);
            if (uc_switch > 1) {
                OAM_ERROR_LOG1(0, OAM_SF_CFG, "{CMD format::sh hipriv.sh 'wlan0 pm_debug dto [0|1]', input[%d]!!}",
                               uc_switch);
                return OAL_FAIL;
            }
            pst_pm_debug_cfg->ul_cmd_bit_map |= BIT(MAC_PM_DEBUG_DTO);
            pst_pm_debug_cfg->uc_dto_switch = uc_switch;

            OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "wal_hipriv_set_pm_debug_switch:dto switch[%d]",
                             uc_switch);
        } else if (strcmp("torv", ac_name) == 0) {
            if (strcmp("help", ac_value) == 0) {
                OAM_WARNING_LOG0(0, OAM_SF_CFG, "{CMD format::sh hipriv.sh 'wlan0 pm_debug torv [0~550]'}");
                return OAL_SUCC;
            }

            us_val = (oal_uint16)oal_atoi(ac_value);
            if (us_val > 550) { /* ����550�Ƿ� */
                OAM_ERROR_LOG1(0, OAM_SF_CFG, "CMD format:hipriv.sh 'wlan0 pm_debug torv [0~550]', intput[%d]", us_val);
                return OAL_FAIL;
            }
            pst_pm_debug_cfg->ul_cmd_bit_map |= BIT(MAC_PM_DEBUG_TORV);
            pst_pm_debug_cfg->us_torv_val = us_val;

            OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{wal_hipriv_set_pm_debug_switch::val[%d].}", us_val);
        } else {
            OAL_IO_PRINT("CMD format::sh hipriv.sh 'wlan0 pm_debug [srb 0|1] [dto 0|1] [nfi 0|1] [apf 0|1] \
                [ao 0|1] [dto 0|1] [torv 0~550]'");
            OAM_ERROR_LOG0(0, OAM_SF_CFG, "{CMD format::sh hipriv.sh 'wlan0 pm_debug [srb 0|1] [dto 0|1] \
                [nfi 0|1] [apf 0|1] [dto 0|1] [torv 0~550]'!!}");
            return OAL_FAIL;
        }
#endif
    } while (*pc_param != '\0');

    /***************************************************************************
                               ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_PM_DEBUG_SWITCH, OAL_SIZEOF(mac_pm_debug_cfg_stru));

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_pm_debug_cfg_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_set_dbdc_debug_switch::return err code[%d]!}", l_ret);
        return (oal_uint32)l_ret;
    }

    return OAL_SUCC;
}
#endif

#ifdef _PRE_WLAN_FEATURE_TCP_ACK_BUFFER

OAL_STATIC oal_uint32 wal_hipriv_tcp_ack_buf_cfg(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    wal_msg_write_stru st_write_msg;
    oal_uint32 ul_off_set;
    oal_int8 ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    oal_uint32 ul_ret;
    oal_int32 l_ret;
    oal_int32 l_tmp;
    oal_uint8 uc_idx;
    mac_cfg_tcp_ack_buf_stru *pst_tcp_ack_param;

    pst_tcp_ack_param = (mac_cfg_tcp_ack_buf_stru *)(st_write_msg.auc_value);
    memset_s((oal_void *)pst_tcp_ack_param, OAL_SIZEOF(mac_cfg_tcp_ack_buf_stru),
             0, OAL_SIZEOF(mac_cfg_tcp_ack_buf_stru));

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, OAL_SIZEOF(ac_name), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_CFG, "wal_hipriv_tcp_ack_buf_cfg:wal_get_cmd_one_arg return err_code [%d]", ul_ret);
        return ul_ret;
    }
    pc_param = pc_param + ul_off_set;

    for (uc_idx = 0; uc_idx < MAC_TCP_ACK_BUF_TYPE_BUTT; uc_idx++) {
        if (strcmp(ac_name, g_ast_hmac_tcp_ack_buf_cfg_table[uc_idx].puc_string) == 0) {
            break;
        }
    }
    if (uc_idx == MAC_TCP_ACK_BUF_TYPE_BUTT) {
        OAM_WARNING_LOG0(0, OAM_SF_CFG, "{wal_hipriv_tcp_ack_buf_cfg:: parameter error !\r\n");
        return uc_idx;
    }
    pst_tcp_ack_param->en_cmd = g_ast_hmac_tcp_ack_buf_cfg_table[uc_idx].en_tcp_ack_buf_cfg_id;

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, OAL_SIZEOF(ac_name), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_CFG, "wal_hipriv_tcp_ack_buf_cfg:wal_get_cmd_one_arg return err_code [%d]", ul_ret);
        return ul_ret;
    }

    l_tmp = (oal_int32)oal_atoi(ac_name);
    if ((l_tmp < 0) || (l_tmp > 10 * 1000)) { /* oal_atoi(ac_name)����С��0���ߴ���10 * 1000���ǷǷ���Ч�� */
        OAM_WARNING_LOG1(0, OAM_SF_CFG, "{wal_hipriv_tcp_ack_buf_cfg::  car param[%d] invalid! }", oal_atoi(ac_name));
        return OAL_FAIL;
    }

    if (pst_tcp_ack_param->en_cmd == MAC_TCP_ACK_BUF_ENABLE) {
        if (((oal_uint8)l_tmp == OAL_FALSE) || ((oal_uint8)l_tmp == OAL_TRUE)) {
            pst_tcp_ack_param->en_enable = (oal_uint8)l_tmp;
        } else {
            OAM_WARNING_LOG1(0, OAM_SF_CFG, "{wal_hipriv_tcp_ack_buf_cfg:: en_fast_aging_flag[%d] must be 0 or 1.}",
                             (oal_uint8)l_tmp);
            return OAL_FAIL;
        }
    }
    if (pst_tcp_ack_param->en_cmd == MAC_TCP_ACK_BUF_TIMEOUT) {
        if ((oal_uint8)l_tmp == 0) {
            OAM_WARNING_LOG0(0, OAM_SF_CFG, "{wal_hipriv_tcp_ack_buf_cfg:: timer_ms shoule not be 0.}");
            return OAL_FAIL;
        }

        pst_tcp_ack_param->uc_timeout_ms = (oal_uint8)l_tmp;
    }
    if (pst_tcp_ack_param->en_cmd == MAC_TCP_ACK_BUF_MAX) {
        if ((oal_uint8)l_tmp == 0) {
            OAM_WARNING_LOG0(0, OAM_SF_CFG, "{wal_hipriv_tcp_ack_buf_cfg:: uc_count_limit shoule not be 0.}");
            return OAL_FAIL;
        }

        pst_tcp_ack_param->uc_count_limit = (oal_uint8)l_tmp;
    }

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_TCP_ACK_BUF, OAL_SIZEOF(mac_cfg_tcp_ack_buf_stru));

    OAM_WARNING_LOG4(0, OAM_SF_CFG, "wal_hipriv_tcp_ack_buf_cfg::en_cmd[%d], en_enable[%d], uc_timeout_ms[%d] \
        uc_count_limit[%d]", pst_tcp_ack_param->en_cmd,
                     pst_tcp_ack_param->en_enable,
                     pst_tcp_ack_param->uc_timeout_ms,
                     pst_tcp_ack_param->uc_count_limit);

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_tcp_ack_buf_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_CFG, "{wal_hipriv_tcp_ack_buf_cfg::return err code [%d]!}\r\n", ul_ret);
        return (oal_uint32)ul_ret;
    }

    return OAL_SUCC;
}
#endif

OAL_STATIC oal_uint32 wal_hipriv_ps_params_cfg(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    wal_msg_write_stru st_write_msg;
    oal_uint32 ul_off_set;
    oal_int8 ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    oal_uint32 ul_ret;
    oal_int32 l_ret;
    oal_int32 l_tmp;
    oal_uint8 uc_idx;
    mac_cfg_ps_params_stru *pst_ps_params;

    pst_ps_params = (mac_cfg_ps_params_stru *)(st_write_msg.auc_value);
    memset_s((oal_void *)pst_ps_params, OAL_SIZEOF(mac_cfg_ps_params_stru), 0, OAL_SIZEOF(mac_cfg_ps_params_stru));

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, OAL_SIZEOF(ac_name), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_CFG, "wal_hipriv_ps_params_cfg:wal_get_cmd_one_arg return err_code [%d]", ul_ret);
        return ul_ret;
    }
    pc_param = pc_param + ul_off_set;

    for (uc_idx = 0; uc_idx < MAC_PS_PARAMS_TYPE_BUTT; uc_idx++) {
        if (strcmp(ac_name, g_ast_hmac_ps_params_cfg_table[uc_idx].puc_string) == 0) {
            break;
        }
    }
    if (uc_idx == MAC_PS_PARAMS_TYPE_BUTT) {
        OAM_WARNING_LOG0(0, OAM_SF_CFG, "{wal_hipriv_ps_params_cfg:: parameter error !\r\n");
        return uc_idx;
    }
    pst_ps_params->en_cmd = g_ast_hmac_ps_params_cfg_table[uc_idx].en_ps_params_cfg_id;

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, OAL_SIZEOF(ac_name), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_CFG, "wal_hipriv_ps_params_cfg:wal_get_cmd_one_arg return err_code [%d]}", ul_ret);
        return ul_ret;
    }

    l_tmp = (oal_int32)oal_atoi(ac_name);
    if ((l_tmp < 0) || (l_tmp > 255)) { /* oal_atoi(ac_name)����С��0���ߴ���255���ǷǷ���Ч�� */
        OAM_WARNING_LOG1(0, OAM_SF_CFG, "{wal_hipriv_ps_params_cfg::  car param[%d] invalid! }\r\n", oal_atoi(ac_name));
        return OAL_FAIL;
    }

    if (pst_ps_params->en_cmd == MAC_PS_PARAMS_TIMEOUT) {
        if ((oal_uint8)l_tmp == 0) {
            OAM_WARNING_LOG0(0, OAM_SF_CFG, "{wal_hipriv_ps_params_cfg:: timer_ms shoule not be 0.}");
            return OAL_FAIL;
        }

        pst_ps_params->uc_timeout_ms = (oal_uint8)l_tmp;
    }
    if (pst_ps_params->en_cmd == MAC_PS_PARAMS_RESTART_COUNT) {
        if ((oal_uint8)l_tmp == 0) {
            OAM_WARNING_LOG0(0, OAM_SF_CFG, "{wal_hipriv_ps_params_cfg:: uc_restart_count shoule not be 0.}");
            return OAL_FAIL;
        }

        pst_ps_params->uc_restart_count = (oal_uint8)l_tmp;
    }

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_PS_PARAMS, OAL_SIZEOF(mac_cfg_ps_params_stru));

    OAM_WARNING_LOG3(0, OAM_SF_CFG, "{wal_hipriv_ps_params_cfg::en_cmd[%d], uc_timeout_ms[%d] uc_restart_count[%d]!}",
                     pst_ps_params->en_cmd,
                     pst_ps_params->uc_timeout_ms,
                     pst_ps_params->uc_restart_count);

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_ps_params_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_CFG, "{wal_hipriv_ps_params_cfg::return err code [%d]!}\r\n", ul_ret);
        return (oal_uint32)ul_ret;
    }

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_APF

OAL_STATIC oal_uint32 wal_hipriv_apf_filter_list(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    wal_msg_write_stru st_write_msg;
    oal_int32 l_ret;
    mac_apf_filter_cmd_stru st_apf_filter_cmd;

    memset_s(&st_apf_filter_cmd, OAL_SIZEOF(mac_apf_filter_cmd_stru), 0, OAL_SIZEOF(mac_apf_filter_cmd_stru));
    st_apf_filter_cmd.en_cmd_type = APF_GET_FILTER_CMD;
    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_APF, OAL_SIZEOF(st_apf_filter_cmd));
    if (memcpy_s(st_write_msg.auc_value, OAL_SIZEOF(st_write_msg.auc_value),
                 &st_apf_filter_cmd, OAL_SIZEOF(st_apf_filter_cmd)) != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "wal_hipriv_apf_filter_list::memcpy fail!");
        return OAL_FAIL;
    }

    /* ������Ϣ */
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(st_apf_filter_cmd),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);

    return (oal_uint32)l_ret;
}
#endif

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
#ifndef CONFIG_HAS_EARLYSUSPEND

OAL_STATIC oal_int32 wal_ioctl_set_suspend_mode(oal_net_device_stru *pst_net_dev, oal_uint8 uc_suspend)
{
    wal_msg_write_stru st_write_msg;
    oal_int32 l_ret;

    if (OAL_UNLIKELY((pst_net_dev == OAL_PTR_NULL))) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_ioctl_set_suspend_mode::pst_net_dev null ptr error!}");
        return -OAL_EFAUL;
    }

    st_write_msg.auc_value[0] = uc_suspend;

    /***************************************************************************
        ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_SUSPEND_MODE, OAL_SIZEOF(uc_suspend));

    /* ������Ϣ */
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(uc_suspend),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    return l_ret;
}
#endif
#endif


OAL_STATIC oal_uint32 wal_hipriv_mcs_set_check_enable(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    oal_uint32 ul_off_set;
    oal_int8 ac_arg[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    oal_uint32 ul_ret;
    oal_int32 l_ret;
    wal_msg_write_stru st_write_msg;
    oal_bool_enum_uint8 *pen_mcs_set_check_enable = OAL_PTR_NULL;

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, OAL_SIZEOF(ac_arg), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "wal_hipriv_ht_check_stop: get first arg fail.");
        return OAL_FAIL;
    }

    /***************************************************************************
                            ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFIGD_MCS_SET_CHECK_ENABLE, OAL_SIZEOF(oal_bool_enum_uint8));

    /* ��������������� */
    pen_mcs_set_check_enable = (oal_bool_enum_uint8 *)(st_write_msg.auc_value);
    *pen_mcs_set_check_enable = (oal_bool_enum_uint8)oal_atoi(ac_arg);

    OAM_WARNING_LOG1(0, OAM_SF_SCAN, "wal_hipriv_mcs_set_check_enable_flag= %d.", *pen_mcs_set_check_enable);

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(oal_bool_enum_uint8),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (l_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_hipriv_mcs_set_check_enable:wal_send_cfg_event fail.err code [%d]", l_ret);
        return (oal_uint32)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC oal_uint32 wal_hipriv_set_tx_pow_param(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    wal_msg_write_stru st_write_msg;
    mac_cfg_set_tx_pow_param_stru *pst_set_tx_pow_param;
    oal_int8 ac_arg[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    oal_uint32 ul_off_set;
    oal_uint32 ul_ret;
    oal_int32 l_ret;
    wal_dscr_param_enum_uint8 en_param_index;
    oal_uint8 uc_value = 0;

    /* �����ʽ: hipriv "wlan0 set_tx_pow rf_reg_ctl 0/1" ,   0:��ʹ��, 1:ʹ�� */
    /* �����ʽ: hipriv "wlan0 set_tx_pow mag_level 0/1/2/3"  ���ù���֡���ʵȼ� */
    /* �����ʽ: hipriv "wlan0 set_tx_pow ctl_level 0/1/2/3"  ���ÿ���֡���ʵȼ� */
    /* �����ʽ: hipriv "wlan0 set_tx_pow show_log type (bw chn)" ��ʾ������־ */
    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_TX_POW, OAL_SIZEOF(mac_cfg_set_tx_pow_param_stru));

    /* ��������������������� */
    pst_set_tx_pow_param = (mac_cfg_set_tx_pow_param_stru *)(st_write_msg.auc_value);
    memset_s(pst_set_tx_pow_param, OAL_SIZEOF(mac_cfg_set_tx_pow_param_stru),
             0, OAL_SIZEOF(mac_cfg_set_tx_pow_param_stru));

    /* ��ȡ�������ֶ����������ַ��� */
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, OAL_SIZEOF(ac_arg), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_hipriv_set_tx_pow_param:wal_get_cmd_one_arg return err_code[%d]", ul_ret);
        return ul_ret;
    }

    /* ������������һ���ֶ� */
    for (en_param_index = 0; en_param_index < WAL_TX_POW_PARAM_BUTT; en_param_index++) {
        if (!strcmp(pauc_tx_pow_param_name[en_param_index], ac_arg)) {
            break;
        }
    }

    /* ��������Ƿ��� */
    if (en_param_index == WAL_TX_POW_PARAM_BUTT) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_hipriv_set_tx_pow_param::no such param for tx pow!}\r\n");
        return OAL_FAIL;
    }

    pst_set_tx_pow_param->en_type = en_param_index;

    /* ��ȡ��һ������ */
    pc_param += ul_off_set;
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, OAL_SIZEOF(ac_arg), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_hipriv_set_tx_pow_param:wal_get_cmd_one_arg return err_code[%d]!", ul_ret);
    } else {
        uc_value = (oal_uint8)oal_atoi(ac_arg);
    }
    pst_set_tx_pow_param->auc_value[0] = uc_value;

    /*lint -e571*/
    /* ����У�� */
    switch (en_param_index) {
        case WAL_TX_POW_PARAM_SET_RF_REG_CTL:
            if (uc_value >= 2) { /* oal_atoi(ac_arg)ֵ���ڵ���2��������Ч������ */
                return OAL_ERR_CODE_INVALID_CONFIG;
            }
            break;
        case WAL_TX_POW_PARAM_SET_MAG_LEVEL:
        case WAL_TX_POW_PARAM_SET_CTL_LEVEL:
            if (uc_value > 4) { /* oal_atoi(ac_arg)ֵ����4��������Ч������ */
                return OAL_ERR_CODE_INVALID_CONFIG;
            }
            break;
        case WAL_TX_POW_PARAM_SHOW_LOG:
            if (pst_set_tx_pow_param->auc_value[0] == 1) { // Ϊ0��ʾ��ѯgmֵ��Ϊ1��ʾ��ѯiq expect pow��صĻ�·����
                // evm iq cali expect pow bw & chn
                /* ��ȡ��һ������bw */
                pc_param += ul_off_set;
                ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, OAL_SIZEOF(ac_arg), &ul_off_set);
                if (ul_ret != OAL_SUCC) {
                    OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_hipriv_set_tx_pow_param::show log arg1 bw missing!}");
                    break;
                }
                pst_set_tx_pow_param->auc_value[1] = (oal_uint8)oal_atoi(ac_arg);

                /* ��ȡ��һ������chn_idx */
                pc_param += ul_off_set;
                ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, OAL_SIZEOF(ac_arg), &ul_off_set);
                if (ul_ret != OAL_SUCC) {
                    OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_hipriv_set_tx_pow_param::show log arg2 chn_indx missing!}");
                    break;
                }
                pst_set_tx_pow_param->auc_value[2] = (oal_uint8)oal_atoi(ac_arg); /* auc_value[2]��ȡchn_idxֵ */
            }
            break;
        default:
            OAM_WARNING_LOG0(0, OAM_SF_ANY, "{wal_hipriv_set_tx_pow_param::error input type!}\r\n");

            return OAL_FAIL;
    }
    /*lint +e571*/
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_set_tx_pow_param_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_set_tx_pow_param::return err code [%d]!}\r\n", l_ret);
        return (oal_uint32)l_ret;
    }

    return OAL_SUCC;
}


OAL_STATIC oal_uint32 wal_hipriv_set_sar_level(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    oal_uint32 ul_off_set;
    oal_int8 ac_arg[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    oal_uint32 ul_ret;
    oal_int32 l_ret;
    oal_uint8 uc_sar_level;
    oal_uint16 us_tx_power = 1000;  // ���ϲ��·��������룬�ο�03�����ϲ��·�����Ϊ1001~1003

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_arg, OAL_SIZEOF(ac_arg), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "wal_hipriv_set_sar_level: get first arg fail.");
        return OAL_FAIL;
    }
    uc_sar_level = (oal_uint8)oal_atoi(ac_arg);
    OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_hipriv_set_sar_level:sar level[%d]", uc_sar_level);
    /* SAR�ĵ�λ��1��20�� */
    if ((uc_sar_level > 20) || (uc_sar_level == 0)) {
        /* �ֶ���������������·���� */
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "wal_hipriv_set_sar_level:uc_sar_level[%d] is not in the range 1~20",
                       uc_sar_level);
        return OAL_FAIL;
    }

    us_tx_power += uc_sar_level;

    l_ret = wal_ioctl_reduce_sar(pst_net_dev, us_tx_power);
    if (l_ret != OAL_SUCC) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "wal_hipriv_set_sar_level: reduce sar fail, l_ret[%d]", l_ret);
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_BTCOEX

OAL_STATIC oal_uint32 wal_hipriv_set_btcoex_params(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    wal_msg_write_stru st_write_msg;
    oal_uint32 ul_off_set;
    oal_int8 ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    oal_uint32 ul_ret;
    oal_int32 l_ret;
    mac_btcoex_cfg_type_enum_uint8 en_cfg_type;
    oal_uint8 uc_cfg_value;
    mac_btcoex_cfg_stru *pst_btcoex_params;

    pst_btcoex_params = (mac_btcoex_cfg_stru *)(st_write_msg.auc_value);
    memset_s((oal_void *)pst_btcoex_params, OAL_SIZEOF(mac_btcoex_cfg_stru), 0, OAL_SIZEOF(mac_btcoex_cfg_stru));
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, OAL_SIZEOF(ac_name), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_CFG, "wal_hipriv_set_btcoex_params::get_cmd_one_arg return err_code [%d]", ul_ret);
        return ul_ret;
    }
    en_cfg_type = (oal_uint8)oal_atoi(ac_name);
    pc_param = pc_param + ul_off_set;
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, OAL_SIZEOF(ac_name), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_CFG, "wal_hipriv_set_btcoex_params:get_cmd_one_arg return err_code=%d", ul_ret);
        return ul_ret;
    }
    uc_cfg_value = (oal_uint8)oal_atoi(ac_name);
    pst_btcoex_params->en_cfg_type = en_cfg_type;
    pst_btcoex_params->uc_cfg_value = uc_cfg_value;
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_BTCOEX_PARAMS, OAL_SIZEOF(mac_btcoex_cfg_stru));
    OAM_WARNING_LOG2(0, OAM_SF_CFG, "{wal_hipriv_set_btcoex_params::en_cfg_type[%d], uc_cfg_value[%d]!}",
                     en_cfg_type, uc_cfg_value);
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_btcoex_cfg_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_CFG, "{wal_hipriv_set_btcoex_params::return err code [%d]!}\r\n", l_ret);
        return OAL_FAIL;
    }
    return OAL_SUCC;
}
#endif
#ifdef _PRE_WLAN_FEATURE_NRCOEX

oal_uint32 wal_ioctl_set_wifi_priority(oal_net_device_stru *pst_net_dev, hal_nrcoex_priority_enum_uint8 en_priority)
{
    wal_msg_write_stru st_write_msg;
    oal_int32 l_ret;

    if (en_priority >= HAL_NRCOEX_PRIORITY_BUTT) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "wal_ioctl_set_wifi_priority:priority[%d] is invalid", en_priority);
        return OAL_FAIL;
    }
    memset_s(&st_write_msg, OAL_SIZEOF(st_write_msg), 0, OAL_SIZEOF(st_write_msg));
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_WIFI_PRIORITY, OAL_SIZEOF(hal_nrcoex_priority_enum_uint8));
    st_write_msg.auc_value[0] = en_priority;
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(hal_nrcoex_priority_enum_uint8),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (l_ret != OAL_SUCC) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "wal_ioctl_set_wifi_priority:send event fail, l_ret=%d", l_ret);
        return OAL_FAIL;
    }
    return OAL_SUCC;
}


OAL_STATIC oal_int32 wal_ioctl_process_nrcoex_priority(oal_net_device_stru *pst_net_dev,
                                                       oal_int8 *pc_command)
{
    hal_nrcoex_priority_enum_uint8 en_priority;
    oal_uint32 ul_ret;
    /* �����ʽSET_NRCOEX_PRIOR 0/1/2 */
    if (OAL_STRLEN(pc_command) < (CMD_SET_NRCOEX_PRIOR_LEN + 2)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_ioctl_process_nrcoex_priority:CMD to short, len=%d",
                         OAL_STRLEN(pc_command));
        return -OAL_EFAIL;
    }
    en_priority = oal_atoi(pc_command + CMD_SET_NRCOEX_PRIOR_LEN + 1);
    ul_ret = wal_ioctl_set_wifi_priority(pst_net_dev, en_priority);
    if (ul_ret != OAL_SUCC) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "wal_ioctl_process_nrcoex_priority:ul_ret=%d", ul_ret);
        return -OAL_EFAIL;
    }
    return OAL_SUCC;
}


OAL_STATIC oal_int32 wal_ioctl_get_nrcoex_info(oal_net_device_stru *pst_net_dev,
                                               oal_ifreq_stru *pst_ifr,
                                               oal_int8 *pc_command)
{
    wal_msg_write_stru st_write_msg;
    oal_int32 l_ret;
    hmac_device_stru *pst_hmac_device;
    mac_vap_stru *pst_mac_vap;
    hmac_nrcoex_info_query_stru *pst_nrcoex_query;

    pst_mac_vap = OAL_NET_DEV_PRIV(pst_net_dev);
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "wal_ioctl_get_nrcoex_info:pst_mac_vap is null");
        return -OAL_EFAIL;
    }
    pst_hmac_device = hmac_res_get_mac_dev(pst_mac_vap->uc_device_id);
    if (pst_hmac_device == OAL_PTR_NULL) {
        OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "wal_ioctl_get_nrcoex_info:pst_hmac_device is null, \
            device id:%d", pst_mac_vap->uc_device_id);
        return -OAL_EFAIL;
    }
    memset_s(&st_write_msg, OAL_SIZEOF(st_write_msg), 0, OAL_SIZEOF(st_write_msg));
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_NRCOEX_INFO, 0);
    l_ret = wal_send_cfg_event(pst_net_dev, WAL_MSG_TYPE_WRITE, WAL_MSG_WRITE_MSG_HDR_LENGTH,
                               (oal_uint8 *)&st_write_msg, OAL_FALSE, OAL_PTR_NULL);
    if (l_ret != OAL_SUCC) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "wal_ioctl_get_nrcoex_info:send event fail, l_ret=%d", l_ret);
        return -OAL_EFAIL;
    }
    pst_nrcoex_query = &pst_hmac_device->st_nrcoex_query;
    pst_nrcoex_query->en_query_completed_flag = OAL_FALSE;
    /*lint -e774*/
    l_ret = OAL_WAIT_EVENT_INTERRUPTIBLE_TIMEOUT(pst_nrcoex_query->st_wait_queue,
                                                 (pst_nrcoex_query->en_query_completed_flag == OAL_TRUE),
                                                 5 * OAL_TIME_HZ); /* HZ��1000��5 * OAL_TIME_HZ������5000ms����˼ */
    /*lint +e774*/
    /* �ȴ���ʱ���쳣�򷵻� */
    if (l_ret <= 0) {
        OAM_ERROR_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "wal_ioctl_get_nrcoex_info:wait fail, l_ret=%d", l_ret);
        return -OAL_EFAIL;
    }
    if (oal_copy_to_user(pst_ifr->ifr_data + PRIV_CMD_STRU_HEAD_LEN, &pst_nrcoex_query->st_nrcoex_info,
                         OAL_SIZEOF(wlan_nrcoex_info_stru))) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "wal_ioctl_get_nrcoex_info:copy to user fail");
        return -OAL_EFAIL;
    }
    return OAL_SUCC;
}
OAL_STATIC oal_uint32 wal_find_nrcoex_cmd(wal_ioctl_nrcoex_cmd_stru *pst_nrcoex_cmd, oal_int8 *pc_name)
{
    oal_uint8 uc_map_index = 0;
    /* Ѱ��ƥ������� */
    *pst_nrcoex_cmd = g_ast_nrcoex_cmd_map[0];
    while (pst_nrcoex_cmd->pc_name != OAL_PTR_NULL) {
        if (oal_strcmp(pst_nrcoex_cmd->pc_name, pc_name) == 0) {
            return OAL_SUCC;
        }
        *pst_nrcoex_cmd = g_ast_nrcoex_cmd_map[++uc_map_index];
    }
    return OAL_FAIL;
}


OAL_STATIC oal_uint32 wal_hipriv_nrcoex_test_cmd(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    oal_int8 ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    oal_uint32 ul_off_set;
    oal_uint32 ul_ret;
    wlan_nrcoex_cmd_stru *pst_nrcoex_cmd;
    wal_msg_write_stru st_write_msg;
    oal_uint16 us_value;
    oal_int32 l_ret;

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, OAL_SIZEOF(ac_name), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "{wal_hipriv_nrcoex_cmd::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    us_value = (oal_uint16)oal_atoi(ac_name);
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_NRCOEX_CMD, OAL_SIZEOF(wlan_nrcoex_cmd_stru));
    pst_nrcoex_cmd = (wlan_nrcoex_cmd_stru *)st_write_msg.auc_value;
    pst_nrcoex_cmd->en_nrcoex_cmd = WLAN_NRCOEX_CMD_NRCOEX_EVENT;
    pst_nrcoex_cmd->us_value = us_value;
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(wlan_nrcoex_cmd_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (l_ret != OAL_SUCC) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "wal_hipriv_nrcoex_cmd:send event fail, l_ret=%d", l_ret);
        return OAL_FAIL;
    }
    return OAL_SUCC;
}

OAL_STATIC oal_uint32 wal_hipriv_nrcoex_cmd(oal_net_device_stru *pst_net_dev, oal_int8 *pc_param)
{
    oal_int8 ac_name[WAL_HIPRIV_CMD_NAME_MAX_LEN];
    oal_uint32 ul_off_set;
    oal_uint32 ul_ret;
    wal_ioctl_nrcoex_cmd_stru st_nrcoex_cmd;
    wlan_nrcoex_cmd_stru *pst_nrcoex_cmd;
    wal_msg_write_stru st_write_msg;
    oal_uint16 us_value;
    oal_int32 l_ret;

    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, OAL_SIZEOF(ac_name), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "{wal_hipriv_nrcoex_cmd::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    pc_param = pc_param + ul_off_set;
    /* Ѱ��ƥ������� */
    if (wal_find_nrcoex_cmd(&st_nrcoex_cmd, ac_name) != OAL_SUCC) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_hipriv_nrcoex_cmd::invalid nrcoex cmd!}\r\n");
        return OAL_FAIL;
    }
    ul_ret = wal_get_cmd_one_arg(pc_param, ac_name, OAL_SIZEOF(ac_name), &ul_off_set);
    if (ul_ret != OAL_SUCC) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "{wal_hipriv_nrcoex_cmd::wal_get_cmd_one_arg return err_code [%d]!}\r\n", ul_ret);
        return ul_ret;
    }
    us_value = (oal_uint16)oal_atoi(ac_name);
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_NRCOEX_CMD, OAL_SIZEOF(wlan_nrcoex_cmd_stru));
    pst_nrcoex_cmd = (wlan_nrcoex_cmd_stru *)st_write_msg.auc_value;
    pst_nrcoex_cmd->en_nrcoex_cmd = st_nrcoex_cmd.en_nrcoex_cmd;
    pst_nrcoex_cmd->us_value = us_value;
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(wlan_nrcoex_cmd_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (l_ret != OAL_SUCC) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "wal_hipriv_nrcoex_cmd:send event fail, l_ret=%d", l_ret);
        return OAL_FAIL;
    }
    return OAL_SUCC;
}
#endif
