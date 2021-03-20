

#ifndef __WAL_LINUX_IOCTL_H__
#define __WAL_LINUX_IOCTL_H__

/* 1 其他头文件包含 */
#include "oal_ext_if.h"
#include "wlan_types.h"
#include "wlan_spec.h"
#include "mac_vap.h"
#include "hmac_ext_if.h"
#include "wal_ext_if.h"
#include "wal_config.h"

#ifdef CONFIG_DOZE_FILTER
#include <huawei_platform/power/wifi_filter/wifi_filter.h>
#endif /* CONFIG_DOZE_FILTER */

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_WAL_LINUX_IOCTL_H
/* 2 宏定义 */
#define WLAN1_NETDEV_NAME          "wlan1"
#define MAX_PRIV_CMD_SIZE          4096
#define NET_DEV_NAME_LEN           16
#define ENABLE 1
#define DISABLE 0
#define BUFF_SIZE 64
#define CMD_LENGTH 2

#ifdef _PRE_WLAN_RR_PERFORMANCE
#define ASPM_ENABLE                1
#define ASPM_DISABLE               0
#define FREQ_MAX_VALUE             30
#define FREQ_MAX_FLAG              0
#define FREQ_AUTO_FLAG             1
#endif /* _PRE_WLAN_RR_PERFORMANCE */

#define WAL_HIPRIV_CMD_MAX_LEN (WLAN_MEM_LOCAL_SIZE2 - 4) /* 私有配置命令字符串最大长度，对应本地内存池一级大小 */

#define WAL_HIPRIV_CMD_NAME_MAX_LEN  80 /* 字符串中每个单词的最大长度(原20) */
#define WAL_HIPRIV_CMD_VALUE_MAX_LEN 10 /* 字符串中某个对应变量取值的最大位数 */

#define WAL_HIPRIV_PROC_ENTRY_NAME "hipriv"

#define WAL_SIOCDEVPRIVATE 0x89F0 /* SIOCDEVPRIVATE */

#define WAL_HIPRIV_HT_MCS_MIN    0
#define WAL_HIPRIV_HT_MCS_MAX    32
#define WAL_HIPRIV_VHT_MCS_MIN   0
#define WAL_HIPRIV_VHT_MCS_MAX   11
#define WAL_HIPRIV_HE_MCS_MIN    0
#define WAL_HIPRIV_HE_MCS_MAX    11
#define WAL_HIPRIV_HE_ER_MCS_MIN 0
#define WAL_HIPRIV_HE_ER_MCS_MAX 2
#define WAL_HIPRIV_NSS_MIN       1
#define WAL_HIPRIV_NSS_MAX       4
#define WAL_HIPRIV_CH_NUM        4
#ifdef  _PRE_WLAN_FEATURE_11AX_ER_SU_DCM
#define WAL_HIPRIV_MCS_TYPE_NUM  (WAL_DSCR_PARAM_MCSAX_ER - WAL_DSCR_PARAM_MCS) + 1
#else
#define WAL_HIPRIV_MCS_TYPE_NUM  (WAL_DSCR_PARAM_MCSAX - WAL_DSCR_PARAM_MCS) + 1
#endif

#define WAL_HIPRIV_BOOL_NIM          0
#define WAL_HIPRIV_BOOL_MAX          1
#define WAL_HIPRIV_FREQ_SKEW_ARG_NUM 8
#define WAL_PHY_DEBUG_TEST_WORD_CNT  5 /* trailer上报个数 */

#define WAL_HIPRIV_MS_TO_S                1000   /* ms和s之间倍数差 */
#define WAL_HIPRIV_KEEPALIVE_INTERVAL_MIN 5000   /* 受默认老化计数器出发时间所限制 */
#define WAL_HIPRIV_KEEPALIVE_INTERVAL_MAX 0xffff /* timer间隔时间限制所致(oal_uin16) */

#define WAL_IWPRIV_CAP_NUM      14
#define WAL_IWPRIV_IGMP_MIN_LEN 50

/* 用户pwr ref reg的定制化保护阈值 */
#define WAL_HIPRIV_PWR_REF_DELTA_HI 40
#define WAL_HIPRIV_PWR_REF_DELTA_LO (-40)

/* IOCTL私有配置命令宏定义 */
#define WAL_IOCTL_PRIV_SETPARAM      (OAL_SIOCIWFIRSTPRIV + 0)
#define WAL_IOCTL_PRIV_GETPARAM      (OAL_SIOCIWFIRSTPRIV + 1)
#define WAL_IOCTL_PRIV_SET_WMM_PARAM (OAL_SIOCIWFIRSTPRIV + 3)
#define WAL_IOCTL_PRIV_GET_WMM_PARAM (OAL_SIOCIWFIRSTPRIV + 5)
#define WAL_IOCTL_PRIV_SET_COUNTRY   (OAL_SIOCIWFIRSTPRIV + 8)
#define WAL_IOCTL_PRIV_GET_COUNTRY   (OAL_SIOCIWFIRSTPRIV + 9)

#define WAL_IOCTL_PRIV_GET_MODE (OAL_SIOCIWFIRSTPRIV + 17) /* 读取模式 */
#define WAL_IOCTL_PRIV_SET_MODE (OAL_SIOCIWFIRSTPRIV + 13) /* 设置模式 包括协议 频段 带宽 */

#define WAL_IOCTL_PRIV_AP_GET_STA_LIST (OAL_SIOCIWFIRSTPRIV + 21)
#define WAL_IOCTL_PRIV_AP_MAC_FLTR     (OAL_SIOCIWFIRSTPRIV + 22)
/* netd将此配置命令作为GET方式下发，get方式命令用奇数，set用偶数 */
#define WAL_IOCTL_PRIV_SET_AP_CFG      (OAL_SIOCIWFIRSTPRIV + 23)
#define WAL_IOCTL_PRIV_AP_STA_DISASSOC (OAL_SIOCIWFIRSTPRIV + 24)

#define WAL_IOCTL_PRIV_SET_MGMT_FRAME_FILTERS (OAL_SIOCIWFIRSTPRIV + 28) /* 设置管理帧过滤 */

#define WAL_IOCTL_PRIV_GET_BLACKLIST (OAL_SIOCIWFIRSTPRIV + 27)

#ifdef _PRE_WLAN_FEATURE_DFS
#define WAL_IOCTL_PRIV_GET_DFS_CHN_STAT (OAL_SIOCIWFIRSTPRIV + 29)
#endif

#define WAL_IOCTL_PRIV_SUBCMD_MAX_LEN 20

#define EXT_PA_ISEXIST_5G_MASK   0x000F
#define EXT_FEM_LP_STATUS_MASK   0x00F0
#define EXT_FEM_FEM_SPEC_MASK    0xFF00
#define EXT_FEM_LP_STATUS_OFFSET 4
#define EXT_FEM_FEM_SPEC_OFFSET  8

#define WAL_IFR_DATA_OFFSET 8

#define WAL_VAP_FOREACH_USER(_pst_user, _pst_vap, _pst_list_pos)                          \
    for ((_pst_list_pos) = (_pst_vap)->st_mac_user_list_head.pst_next,                    \
         (_pst_user) = oal_dlist_get_entry((_pst_list_pos), mac_user_stru, st_user_dlist); \
         (_pst_list_pos) != &((_pst_vap)->st_mac_user_list_head);                         \
         (_pst_list_pos) = (_pst_list_pos)->pst_next,                                     \
         (_pst_user) = oal_dlist_get_entry((_pst_list_pos), mac_user_stru, st_user_dlist)) \
        if (OAL_PTR_NULL != (_pst_user))

/*
  私有命令函数表. 私有命令格式:
         设备名 命令名 参数
  hipriv "Hisilicon0 create vap0"
*/
/* private command strings */
#define CMD_SET_AP_WPS_P2P_IE         "SET_AP_WPS_P2P_IE"
#define CMD_SET_MLME_IE               "SET_MLME_IE"
#define CMD_P2P_SET_NOA               "P2P_SET_NOA"
#define CMD_P2P_SET_PS                "P2P_SET_PS"
#define CMD_SET_POWER_ON              "SET_POWER_ON"
#define CMD_SET_POWER_MGMT_ON         "SET_POWER_MGMT_ON"
#define CMD_COUNTRY                   "COUNTRY"
#define CMD_GET_CAPA_DBDC             "GET_CAPAB_RSDB"
#define CMD_CAPA_DBDC_SUPP            "RSDB:1"
#define CMD_CAPA_DBDC_NOT_SUPP        "RSDB:0"
#define CMD_SET_DC_STATE              "SET_DC_STATE"
#ifdef _PRE_WLAN_FEATURE_NRCOEX
#define CMD_SET_NRCOEX_PRIOR          "SET_NRCOEX_PRIOR"
#define CMD_GET_NRCOEX_INFO           "GET_NRCOEX_INFO"
#endif
#define CMD_SET_CALI_FEM_MODE         "SET_CALI_FEM_MODE"
#define CMD_SET_QOS_MAP               "SET_QOS_MAP"
#define CMD_TX_POWER                  "TX_POWER"
#define CMD_WPAS_GET_CUST             "WPAS_GET_CUST"
#define CMD_SET_STA_PM_ON             "SET_STA_PM_ON"
#define CMD_SET_DYNAMIC_DBAC_MODE     "SET_DYNAMIC_DBAC_MODE"

#ifdef _PRE_WLAN_FEATURE_TAS_ANT_SWITCH
/* TAS天线切换命令 */
#define CMD_SET_MEMO_CHANGE           "SET_ANT_CHANGE"
/* 测量天线 */
#define CMD_MEASURE_TAS_RSSI          "SET_TAS_MEASURE"
/* 抬功率 */
#define CMD_SET_TAS_TXPOWER           "SET_TAS_TXPOWER"
/* 获取天线 */
#define CMD_TAS_GET_ANT               "TAS_GET_ANT"
#endif

#define CMD_SET_CLOSE_GO_CAC            "SET_CLOSE_GO_CAC"
#define CMD_SET_CLOSE_GO_CAC_LEN        (OAL_STRLEN(CMD_SET_CLOSE_GO_CAC))
#define CMD_SET_P2P_SCENE               "CMD_SET_P2P_SCENES"
#define CMD_SET_P2P_SCENE_LEN           (OAL_STRLEN(CMD_SET_P2P_SCENE))
#define CMD_SET_GO_DETECT_RADAR         "CMD_SET_GO_DETECT_RADAR"
#define CMD_SET_GO_DETECT_RADAR_LEN     (OAL_STRLEN(CMD_SET_GO_DETECT_RADAR))
#define CMD_SET_CHANGE_GO_CHANNEL       "CMD_SET_CHANGE_GO_CHANNEL"
#define CMD_SET_CHANGE_GO_CHANNEL_LEN   (OAL_STRLEN(CMD_SET_CHANGE_GO_CHANNEL))

#ifdef _PRE_WLAN_FEATURE_M2S
#define CMD_SET_M2S_SWITCH            "SET_M2S_SWITCH"
#define CMD_SET_M2S_BLACKLIST         "SET_M2S_BLACKLIST"
#define CMD_SET_M2S_MODEM             "SET_M2S_MODEM"
#endif

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

#define CMD_VOWIFI_SET_PARAM          "VOWIFI_DETECT SET"
#define CMD_VOWIFI_GET_PARAM          "VOWIFI_DETECT GET"

#define CMD_VOWIFI_IS_SUPPORT_REPLY   "true"

#define CMD_VOWIFI_IS_SUPPORT         "VOWIFI_DETECT VOWIFI_IS_SUPPORT"

#define CMD_GET_WIFI_PRIV_FEATURE_CAPABILITY "GET_WIFI_PRIV_FEATURE_CAPABILITY"

#define CMD_SETSUSPENDOPT             "SETSUSPENDOPT"
#define CMD_SETSUSPENDMODE            "SETSUSPENDMODE"

#define CMD_SET_SOFTAP_MIMOMODE       "SET_AP_MODE"

#define CMD_GET_AP_BANDWIDTH          "GET_AP_BANDWIDTH"

#define CMD_GET_VHT160_SUPPORTED      "GET_VHT160_SUPPORTED"

#define CMD_SET_VHT160_FEM_LOWER      "SET_VHT160_FEM_LOWER"

#define CMD_PCIE_ASPM_STATE           "CMD_PCIE_ASPM_STATE"
#define CMD_WLAN_FREQ                 "CMD_WLAN_FREQ"
#define CMD_NAPI_STATE                "CMD_NAPI_STATE"
#define CMD_PM_STATE                  "CMD_PM_STATE"
#define CMD_GSO_STATE                 "CMD_GSO_STATE"
#define CMD_LOW_LATENCY_ON            "CMD_LOW_LATENCY_ON"
#define CMD_LOW_LATENCY_OFF           "CMD_LOW_LATENCY_OFF"
#define CMD_HID2D_PARAMS              "CMD_HID2D_PARAMS"

#define CMD_SET_RX_FILTER_ENABLE       "set_rx_filter_enable"
#define CMD_ADD_RX_FILTER_ITEMS        "add_rx_filter_items"
#define CMD_CLEAR_RX_FILTERS           "clear_rx_filters"
#define CMD_GET_RX_FILTER_PKT_STATE    "get_rx_filter_pkt_state"
#define CMD_FILTER_SWITCH              "FILTER"

#define CMD_GET_APF_PKTS_CNT           "GET_APF_PKTS_CNT"
#define CMD_GET_APF_PKTS_CNT_LEN       (OAL_STRLEN(CMD_GET_APF_PKTS_CNT))
#ifdef _PRE_WLAN_FEATURE_PSM_ABN_PKTS_STAT
#define CMD_GET_ABNORMAL_PKTS_CNT      "GET_ABNORMAL_PKTS_CNT"
#define CMD_GET_ABNORMAL_PKTS_CNT_LEN  (OAL_STRLEN(CMD_GET_ABNORMAL_PKTS_CNT))
#endif
#ifdef _PRE_WLAN_FEATURE_MONITOR
#define CMD_QUERY_SNIFFER      "CMD_QUERY_SNIFFER"
#define CMD_QUERY_SNIFFER_LEN  (OAL_STRLEN(CMD_QUERY_SNIFFER))
#endif
#ifdef _PRE_WLAN_FEATURE_CSI
#define CMD_QUERY_CSI      "CMD_QUERY_CSI"
#define CMD_QUERY_CSI_LEN  (OAL_STRLEN(CMD_QUERY_CSI))
#endif
#define CMD_SET_FASTSLEEP_SWITCH       "SET_FASTSLEEP_SWITCH "
#define CMD_SET_FASTSLEEP_SWITCH_LEN   (OAL_STRLEN(CMD_SET_FASTSLEEP_SWITCH))

#define CMD_GET_FAST_SLEEP_CNT         "GET_FAST_SLEEP_CNT"
#define CMD_GET_FAST_SLEEP_CNT_LEN     (OAL_STRLEN(CMD_GET_FAST_SLEEP_CNT))

#define CMD_GET_BEACON_CNT             "GET_BEACON_CNT"
#define CMD_GET_BEACON_CNT_LEN         (OAL_STRLEN(CMD_GET_BEACON_CNT))

#define CMD_SET_TCP_ACK_CTL             "SET_TCP_ACK_SWITCH "
#define CMD_SET_TCP_ACK_CTL_LEN        (OAL_STRLEN(CMD_SET_TCP_ACK_CTL))

#define CMD_GET_WIFI6_SUPPORT          "CMD_WIFI_CATEGORY"
#define CMD_GET_WIFI6_SUPPORT_LEN      (OAL_STRLEN(CMD_GET_WIFI6_SUPPORT))

#define CMD_SET_AX_CLOSE_HTC            "SET_AX_CLOSE_HTC"
#define CMD_SET_AX_CLOSE_HTC_LEN        (OAL_STRLEN(CMD_SET_AX_CLOSE_HTC))
#define CMD_SET_AX_BLACKLIST            "SET_AX_BLACKLIST"
#define CMD_SET_AX_BLACKLIST_LEN        (OAL_STRLEN(CMD_SET_AX_BLACKLIST))
#define CMD_SET_CLREAR_11N_BLACKLIST    "CMD_CLEAR_11N_BLACKLIST"
#define CMD_SET_CLREAR_11N_BLACKLIST_LEN (OAL_STRLEN(CMD_SET_CLREAR_11N_BLACKLIST))
#define CMD_GET_RADAR_RESULT           "GET_RADAR_RESULT"
#define CMD_GET_RADAR_RESULT_LEN       (OAL_STRLEN(CMD_GET_RADAR_RESULT))
#define CMD_GET_TB_FRAME_GAIN        "GET_TB_FRAME_GAIN"
#define CMD_GET_TB_FRAME_GAIN_LEN    (OAL_STRLEN(CMD_GET_TB_FRAME_GAIN))
#define CMD_RXFILTER_START             "RXFILTER-START"
#define CMD_RXFILTER_STOP              "RXFILTER-STOP"
#define CMD_MIRACAST_START             "MIRACAST 1"
#define CMD_MIRACAST_STOP              "MIRACAST 0"
#define PROTOCOL_DEFAULT               1
#define PROTOCOL_WIFI6_SUPPORT         2

#ifdef _PRE_WLAN_RX_LISTEN_POWER_SAVING
#define CMD_SET_RX_LISTEN_PS_SWITCH    "SET_RX_LISTEN_PS_SWITCH "
#define CMD_SET_RX_LISTEN_PS_SWITCH_LEN (OAL_STRLEN(CMD_SET_RX_LISTEN_PS_SWITCH))
#define CMD_GET_RX_LISTEN_STATE        "GET_RX_LISTEN_STATE"
#define CMD_GET_RX_LISTEN_STATE_LEN    (OAL_STRLEN(CMD_GET_RX_LISTEN_STATE))
#endif

#ifdef _PRE_WLAN_FEATURE_FTM
#define CMD_FTM_STARTRANGING "FTM_STARTRANGING " /* 终端接口下发FTM测量命令 */
#define CMD_FTM_STARTRANGING_LEN_MIN 44
#define CMD_FTM_STARTRANGING_LEN (OAL_STRLEN(CMD_FTM_STARTRANGING))
#define CMD_FTM_STARTRANGING_PARA_NUM 6
#endif

/* 私有命令参数长度宏 */
#define   CMD_SET_AP_WPS_P2P_IE_LEN                              (OAL_STRLEN(CMD_SET_AP_WPS_P2P_IE))
#define   CMD_P2P_SET_NOA_LEN                                    (OAL_STRLEN(CMD_P2P_SET_NOA))
#define   CMD_P2P_SET_PS_LEN                                     (OAL_STRLEN(CMD_P2P_SET_PS))
#define   CMD_SET_STA_PM_ON_LEN                                  (OAL_STRLEN(CMD_SET_STA_PM_ON))
#define   CMD_SET_QOS_MAP_LEN                                    (OAL_STRLEN(CMD_SET_QOS_MAP))
#define   CMD_COUNTRY_LEN                                        (OAL_STRLEN(CMD_COUNTRY))
#define   CMD_LTECOEX_MODE_LEN                                   (OAL_STRLEN(CMD_LTECOEX_MODE))
#define   CMD_TX_POWER_LEN                                       (OAL_STRLEN(CMD_TX_POWER))
#define   CMD_WPAS_GET_CUST_LEN                                  (OAL_STRLEN(CMD_WPAS_GET_CUST))
#define   CMD_VOWIFI_SET_PARAM_LEN                               (OAL_STRLEN(CMD_VOWIFI_SET_PARAM))
#define   CMD_VOWIFI_GET_PARAM_LEN                               (OAL_STRLEN(CMD_VOWIFI_GET_PARAM))
#define   CMD_SETSUSPENDOPT_LEN                                  (OAL_STRLEN(CMD_SETSUSPENDOPT))
#define   CMD_SETSUSPENDMODE_LEN                                 (OAL_STRLEN(CMD_SETSUSPENDMODE))
#define   CMD_GET_WIFI_PRIV_FEATURE_CAPABILITY_LEN               (OAL_STRLEN(CMD_GET_WIFI_PRIV_FEATURE_CAPABILITY))
#define   CMD_VOWIFI_IS_SUPPORT_LEN                              (OAL_STRLEN(CMD_VOWIFI_IS_SUPPORT))
#define   CMD_VOWIFI_IS_SUPPORT_REPLY_LEN                        (OAL_STRLEN(CMD_VOWIFI_IS_SUPPORT_REPLY))
#define   CMD_FILTER_SWITCH_LEN                                  (OAL_STRLEN(CMD_FILTER_SWITCH))
#define   CMD_RXFILTER_START_LEN                                 (OAL_STRLEN(CMD_RXFILTER_START))
#define   CMD_RXFILTER_STOP_LEN                                  (OAL_STRLEN(CMD_RXFILTER_STOP))
#define   CMD_SET_MLME_IE_LEN                                    (OAL_STRLEN(CMD_SET_MLME_IE))
#define   CMD_MIRACAST_START_LEN                                 (OAL_STRLEN(CMD_MIRACAST_START))
#define   CMD_MIRACAST_STOP_LEN                                  (OAL_STRLEN(CMD_MIRACAST_STOP))
#define   CMD_SET_POWER_ON_LEN                                   (OAL_STRLEN(CMD_SET_POWER_ON))
#define   CMD_SET_POWER_MGMT_ON_LEN                              (OAL_STRLEN(CMD_SET_POWER_MGMT_ON))
#define   CMD_GET_CAPA_DBDC_LEN                                  (OAL_STRLEN(CMD_GET_CAPA_DBDC))
#define   CMD_CAPA_DBDC_SUPP_LEN                                 (OAL_STRLEN(CMD_CAPA_DBDC_SUPP))
#define   CMD_CAPA_DBDC_NOT_SUPP_LEN                             (OAL_STRLEN(CMD_CAPA_DBDC_NOT_SUPP))
#define   CMD_SET_TAS_TXPOWER_LEN                                (OAL_STRLEN(CMD_SET_TAS_TXPOWER))
#define   CMD_MEASURE_TAS_RSSI_LEN                               (OAL_STRLEN(CMD_MEASURE_TAS_RSSI))
#define   CMD_TAS_GET_ANT_LEN                                    (OAL_STRLEN(CMD_TAS_GET_ANT))
#define   CMD_SET_MEMO_CHANGE_LEN                                (OAL_STRLEN(CMD_SET_MEMO_CHANGE))
#define   CMD_SET_M2S_BLACKLIST_LEN                              (OAL_STRLEN(CMD_SET_M2S_BLACKLIST))
#define   CMD_GET_AP_BANDWIDTH_LEN                               (OAL_STRLEN(CMD_GET_AP_BANDWIDTH))
#define   CMD_GET_VHT160_SUPPORTED_LEN                           (OAL_STRLEN(CMD_GET_VHT160_SUPPORTED))
#define   CMD_SET_VHT160_FEM_LOWER_LEN                           (OAL_STRLEN(CMD_SET_VHT160_FEM_LOWER))
#define   CMD_SET_DC_STATE_LEN                                   (OAL_STRLEN(CMD_SET_DC_STATE))
#define   CMD_PCIE_ASPM_STATE_LEN                                (OAL_STRLEN(CMD_PCIE_ASPM_STATE))
#define   CMD_WLAN_FREQ_LEN                                      (OAL_STRLEN(CMD_WLAN_FREQ))
#define   CMD_NAPI_STATE_LEN                                     (OAL_STRLEN(CMD_NAPI_STATE))
#define   CMD_PM_STATE_LEN                                       (OAL_STRLEN(CMD_PM_STATE))
#define   CMD_GSO_STATE_LEN                                      (OAL_STRLEN(CMD_GSO_STATE))
#define   CMD_LOW_LATENCY_ON_LEN                                 (OAL_STRLEN(CMD_LOW_LATENCY_ON))
#define   CMD_LOW_LATENCY_OFF_LEN                                (OAL_STRLEN(CMD_LOW_LATENCY_OFF))
#define   CMD_SET_DYNAMIC_DBAC_MODE_LEN                          (OAL_STRLEN(CMD_SET_DYNAMIC_DBAC_MODE))
#define   CMD_HID2D_PARAMS_LEN                                   (OAL_STRLEN(CMD_HID2D_PARAMS))

typedef uint32_t (*wal_hipriv_cmd_func)(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
typedef uint32_t (*wal_hid2d_vendor_cmd_func)(oal_net_device_stru *net_dev, int8_t *cmd);

typedef uint32_t (*pwal_get_cmd_one_arg)(int8_t *pc_cmd, int8_t *pc_arg, uint32_t ul_arg_len, uint32_t *pul_cmd_offset);

/* if TV && built-in, open wlan0 is early and WAL_WAIT_CALI_COMPLETE_TIME should be longer */
#if (defined _PRE_PRODUCT_HI3751V811) && (!defined CONFIG_HI110X_KERNEL_MODULES_BUILD_SUPPORT)
#define WAL_WAIT_CALI_COMPLETE_TIME  15000
#else
#define WAL_WAIT_CALI_COMPLETE_TIME  6000
#endif

#ifdef _PRE_LINUX_TEST
#define BOARD_SUCC OAL_SUCC
#endif

/* 3 枚举定义 */
typedef enum {
    WAL_DSCR_PARAM_PA_GAIN_LEVEL = 0, /* pa增益等级 */
    WAL_DSCR_PARAM_MICRO_TX_POWER_GAIN_LEVEL,
    WAL_DSCR_PARAM_TXRTS_ANTENNA,
    WAL_DSCR_PARAM_RXCTRL_ANTENNA,
    WAL_DSCR_PARAM_CHANNAL_CODE,
    WAL_DSCR_PARAM_POWER,
    WAL_DSCR_PARAM_SHORTGI,
    WAL_DSCR_PARAM_PREAMBLE_MODE,
    WAL_DSCR_PARAM_RTSCTS,
    WAL_DSCR_PARAM_LSIGTXOP,
    WAL_DSCR_PARAM_SMOOTH,
    WAL_DSCR_PARAM_SOUNDING,
    WAL_DSCR_PARAM_TXBF,
    WAL_DSCR_PARAM_STBC,
    WAL_DSCR_PARAM_GET_ESS,
    WAL_DSCR_PARAM_DYN_BW,
    WAL_DSCR_PARAM_DYN_BW_EXIST,
    WAL_DSCR_PARAM_CH_BW_EXIST,
    WAL_DSCR_PARAM_RATE,
    WAL_DSCR_PARAM_MCS,
    WAL_DSCR_PARAM_MCSAC,
    WAL_DSCR_PARAM_MCSAX,
    WAL_DSCR_PARAM_MCSAX_ER,
    WAL_DSCR_PARAM_NSS,
    WAL_DSCR_PARAM_BW,
    WAL_DSCR_PARAM_LTF,
    WLA_DSCR_PARAM_GI,
    WLA_DSCR_PARAM_TXCHAIN,
    WLA_DSCR_PARAM_DCM,
    WLA_DSCR_PARAM_PROTOCOL_MODE,

    WAL_DSCR_PARAM_BUTT
} wal_dscr_param_enum;

typedef uint8_t wal_dscr_param_enum_uint8;

/* rx ip数据包过滤功能和上层协定(格式)结构体，TBD 上层接口尚未明确，需澄清后修改 */
#ifdef CONFIG_DOZE_FILTER
typedef hw_wifi_filter_item wal_hw_wifi_filter_item;
typedef struct hw_wlan_filter_ops wal_hw_wlan_filter_ops;

#else
typedef struct {
    unsigned short protocol;  // 协议类型
    unsigned short port;      // 目的端口号
    unsigned int filter_cnt;  // 过滤报文数
} wal_hw_wifi_filter_item;

typedef struct {
    int (*set_filter_enable)(int);
    int (*set_filter_enable_ex)(int, int);
    int (*add_filter_items)(wal_hw_wifi_filter_item *, int);
    int (*clear_filters)(void);
    int (*get_filter_pkg_stat)(wal_hw_wifi_filter_item *, int, int *);
} wal_hw_wlan_filter_ops;
#endif

typedef enum {
    WAL_TX_POW_PARAM_SET_RF_REG_CTL = 0,
    WAL_TX_POW_PARAM_SET_FIX_LEVEL,
    WAL_TX_POW_PARAM_SET_MAG_LEVEL,
    WAL_TX_POW_PARAM_SET_CTL_LEVEL,
    WAL_TX_POW_PARAM_SET_AMEND,
    WAL_TX_POW_PARAM_SET_NO_MARGIN,
    WAL_TX_POW_PARAM_SET_SHOW_LOG,
    WAL_TX_POW_PARAM_SET_SAR_LEVEL,
#ifdef _PRE_WLAN_FEATURE_TAS_ANT_SWITCH
    WAL_TX_POW_PARAM_TAS_POW_CTRL,
    WAL_TX_POW_PARAM_TAS_RSSI_MEASURE,
    WAL_TX_POW_PARAM_TAS_ANT_SWITCH,
#endif
    WAL_TX_POW_PARAM_SHOW_TPC_TABLE_GAIN,
    WAL_TX_POW_POW_SAVE,
    WAL_TX_POW_GET_PD_INFO,
    WAL_TX_POW_SET_TPC_IDX,

    WAL_TX_POW_PARAM_BUTT
} wal_tx_pow_param_enum;
typedef uint8_t wal_tx_pow_param_enum_uint8;

typedef enum {
    WAL_AMPDU_DISABLE,
    WAL_AMPDU_ENABLE,

    WAL_AMPDU_CFG_BUTT
} wal_ampdu_idx_enum;

typedef uint8_t wal_ampdu_idx_enum_uint8;

/* wifi 能力开关状态枚举值，通知到上层使用 */
typedef enum {
    WAL_WIFI_FEATURE_SUPPORT_11K = 0,
    WAL_WIFI_FEATURE_SUPPORT_11V = 1,
    WAL_WIFI_FEATURE_SUPPORT_11R = 2,
    WAL_WIFI_FEATURE_SUPPORT_VOWIFI_NAT_KEEP_ALIVE = 3,
    WAL_WIFI_FEATURE_SUPPORT_NARROWBAND = 4,
    WAL_WIFI_FEATURE_SUPPORT_160M_STA = 5,
    WAL_WIFI_FEATURE_SUPPORT_160M_AP = 6,

    WAL_WIFI_FEATURE_SUPPORT_BUTT
} wal_wifi_feature_capbility_enum;

/* HiD2D APK命令枚举值，上层下发下来 */
typedef enum {
    WAL_HID2D_INIT_CMD = 0,
    WAL_HID2D_SCAN_START_CMD = 1,
    WAL_HID2D_CHAN_SWITCH_CMD = 2,
    WAL_HID2D_HIGH_BW_MCS_CMD = 3,
    WAL_HID2D_RTS_CMD = 4,
    WAL_HID2D_TPC_CMD = 5,
    WAL_HID2D_CCA_CMD = 6,
    WAL_HID2D_EDCA_CMD = 7,
    WAL_HID2D_ACS_CMD = 8,
    WAL_HID2D_ACS_GC_INFO_CMD = 9,

    WAL_HID2D_CMD_BUTT,
} wal_hid2d_cmd_enum;
typedef uint8_t wal_hid2d_cmd_enum_uint8;

#ifdef _PRE_WLAN_RR_PERFORMANCE
/* pc命令枚举类型 */
typedef enum {
    WAL_EXT_PRI_CMD_SET_DEVICE_FREQ_MAX,     // 0 将Device频率锁定到最高
    WAL_EXT_PRI_CMD_SET_DEVICE_FREQ_AUTO,    // 1 将Device频率设置成动态调频
    WAL_EXT_PRI_CMD_ENBLE_PCIE_ASPM,         // 2 打开PCIE低功耗
    WAL_EXT_PRI_CMD_DISABLE_PCIE_ASPM,       // 3 关闭PCIE低功耗

    WAL_EXT_PRI_CMD_BUFF
} wal_ext_pri_cmd_neum;
#endif /* _PRE_WLAN_RR_PERFORMANCE */
#define WAL_IOCTL_IS_INVALID_FIXED_RATE(_l_val, _pc_stu)           \
    ((((_l_val) == WAL_HIPRIV_RATE_INVALID) &&                      \
     ((_pc_stu)->uc_function_index >= WAL_DSCR_PARAM_RATE) &&   \
     ((_pc_stu)->uc_function_index <= WAL_DSCR_PARAM_MCSAC)) ||  \
     ((_l_val) == 0 && ((_pc_stu)->uc_function_index == WAL_DSCR_PARAM_RATE)))

/* 4 全局变量声明 */
extern oal_iw_handler_def_stru g_st_iw_handler_def;
extern oal_net_device_ops_stru g_st_wal_net_dev_ops;
extern oal_ethtool_ops_stru g_st_wal_ethtool_ops;
extern oal_completion g_wlan_cali_completed;
extern int8_t g_wait_wlan_power_on_cali;

/* 5 消息头定义 */
/* 6 消息定义 */
/* 7 STRUCT定义 */
/* 110x 用于将上层下发字符串命令序列化 */
typedef struct {
    uint8_t     *pc_priv_cmd;
    uint32_t     ul_case_entry;
} wal_ioctl_priv_cmd_stru;
/* 私有命令入口结构定义 */
typedef struct {
    int8_t *pc_cmd_name;      /* 命令字符串 */
    wal_hipriv_cmd_func p_func; /* 命令对应处理函数 */
} wal_hipriv_cmd_entry_stru;

/* VoWiFi命令的转换结构体 */
typedef struct {
    int8_t *pc_vowifi_cmd_name;            /* 命令字符串 */
    mac_vowifi_cmd_enum_uint8 en_vowifi_cmd; /* 命令对应类型 */
    uint8_t auc_resv[3];
} wal_vowifi_cmd_stru;

/* 协议模式与字符串映射 */
typedef struct {
    int8_t *pc_name;                              /* 模式名字符串 */
    wlan_protocol_enum_uint8 en_mode;               /* 协议模式 */
    wlan_channel_band_enum_uint8 en_band;           /* 频段 */
    wlan_channel_bandwidth_enum_uint8 en_bandwidth; /* 带宽 */
    uint8_t auc_resv[1];
} wal_ioctl_mode_map_stru;

/* 算法参数配置结构体 */
typedef struct {
    int8_t *pc_name;                           /* 配置命令字符串 */
    mac_dyn_cali_cfg_enum_uint8 en_dyn_cali_cfg; /* 配置命令对应的枚举值 */
    uint8_t auc_resv[3];                       /* 字节对齐 */
} wal_ioctl_dyn_cali_stru;

/* 算法参数配置结构体 */
typedef struct {
    int8_t *pc_name;                 /* 配置命令字符串 */
    mac_alg_cfg_enum_uint8 en_alg_cfg; /* 配置命令对应的枚举值 */
    uint8_t auc_resv[3];             /* 字节对齐 */
} wal_ioctl_alg_cfg_stru;

#ifdef _PRE_WLAN_RR_PERFORMANCE
typedef struct wal_wifi_pc_cmd {
    int32_t   l_verify;    /* 校验位 */
    uint32_t  ul_cmd;      /* 命令号 */
} wal_wifi_pc_cmd_stru;
#endif /* _PRE_WLAN_RR_PERFORMANCE */
typedef struct {
    int8_t *pc_name;                    /* 配置命令字符串 */
    wlan_cfgid_enum_uint16 en_tlv_cfg_id; /* 配置命令对应的枚举值 */
    uint8_t auc_resv[2];                /* 字节对齐 */
} wal_ioctl_tlv_stru;

typedef struct {
    int8_t *pc_name;                    /* 配置命令字符串 */
    wlan_cfgid_enum_uint16 en_tlv_cfg_id; /* 配置命令对应的枚举值 */
    wal_ioctl_tlv_stru *pst_cfg_table;
} wal_ioctl_str_stru;

/* HiD2D APK ALG命令配置结构体 */
typedef struct {
    wal_hid2d_cmd_enum_uint8 alg_cfg_type;  /* 配置命令类型 */
    int8_t *name;                           /* 配置命令对应的字符串 */
    uint8_t resv[3];                        /* 字节对齐 */
} wal_ioctl_hid2d_apk_cfg_stru;
/* HiD2D 命令配置结构体 */
typedef struct {
    wal_hid2d_cmd_enum_uint8 hid2d_cfg_type;  /* 配置命令类型 */
    wal_hid2d_vendor_cmd_func func;           /* 命令对应处理函数 */
} wal_hid2d_cmd_entry_stru;

typedef struct {
    uint32_t ul_ap_max_user;           /* ap最大用户数 */
    int8_t ac_ap_mac_filter_mode[257]; /* AP mac地址过滤命令参数,最长256 */
    int32_t l_ap_power_flag;           /* AP上电标志 */
} wal_ap_config_stru;

typedef struct {
    int8_t *pc_country;                    /* 国家字符串 */
    mac_dfs_domain_enum_uint8 en_dfs_domain; /* DFS 雷达标准 */
} wal_dfs_domain_entry_stru;

extern wal_ap_config_stru g_st_ap_config_info;
extern wal_ap_config_stru g_st_ap_config_info;
extern OAL_CONST wal_ioctl_str_stru g_ast_set_str_table[];
extern OAL_CONST wal_hipriv_cmd_entry_stru g_ast_hipriv_cmd[];
#ifdef _PRE_WLAN_FEATURE_TAS_ANT_SWITCH
extern oal_bool_enum_uint8 g_tas_switch_en[];
#endif

/* 8 UNION定义 */
/* 9 OTHERS定义 */
/* 10 函数声明 */
uint32_t wal_hipriv_ampdu_tx_on(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
uint32_t wal_hipriv_amsdu_tx_on(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
uint32_t wal_hipriv_set_rate(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
uint32_t wal_hipriv_set_mcs(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
uint32_t wal_hipriv_set_mcsac(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
#ifdef _PRE_WLAN_FEATURE_11AX
uint32_t wal_hipriv_set_mcsax(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
#ifdef  _PRE_WLAN_FEATURE_11AX_ER_SU_DCM
uint32_t _wal_hipriv_set_mcsax_er(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
#endif
#endif
uint32_t wal_hipriv_vap_info(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
#ifdef _PRE_WLAN_FEATURE_NEGTIVE_DET
uint32_t wal_hipriv_pk_mode_debug(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
#endif
uint32_t wal_hipriv_set_ip_filter(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
uint32_t wal_hipriv_create_proc(void *p_proc_arg);
uint32_t wal_hipriv_del_vap(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
uint32_t wal_hipriv_remove_proc(void);
uint32_t wal_alloc_cfg_event(oal_net_device_stru *pst_net_dev, frw_event_mem_stru **ppst_event_mem,
                                          void *pst_resp_addr, wal_msg_stru **ppst_cfg_msg, uint16_t us_len);
int32_t wal_send_cfg_event(oal_net_device_stru *pst_net_dev,
                                        wal_msg_type_enum_uint8 en_msg_type,
                                        uint16_t us_len,
                                        uint8_t *puc_param,
                                        oal_bool_enum_uint8 en_need_rsp,
                                        wal_msg_stru **ppst_rsp_msg);
uint32_t wal_hipriv_process_rate_params(oal_net_device_stru *pst_net_dev,
                                                 int8_t *pc_cmd,
                                                 mac_cfg_set_dscr_param_stru *pc_stu);

int32_t wal_start_vap(oal_net_device_stru *pst_net_dev);
int32_t wal_stop_vap(oal_net_device_stru *pst_net_dev);
int32_t wal_init_wlan_vap(oal_net_device_stru *pst_net_dev);
int32_t wal_deinit_wlan_vap(oal_net_device_stru *pst_net_dev);
int32_t wal_init_wlan_netdev(oal_wiphy_stru *pst_wiphy, const char *dev_name);
int32_t wal_setup_ap(oal_net_device_stru *pst_net_dev);
int32_t wal_host_dev_init(oal_net_device_stru *pst_net_dev);
#ifdef _PRE_WLAN_FEATURE_NAN
void wal_init_nan_netdev(oal_wiphy_stru *wiphy);
#endif

#ifdef _PRE_WLAN_FEATURE_11D
uint32_t wal_regdomain_update_sta(uint8_t uc_vap_id);
int32_t wal_regdomain_update_country_code(oal_net_device_stru *pst_net_dev, int8_t *pc_country);
#ifdef _PRE_WLAN_COUNTRYCODE_SELFSTUDY
int32_t wal_regdomain_update_selfstudy_country_code(oal_net_device_stru *pst_net_dev,
                                                                 int8_t *pc_country);
void wal_selfstudy_regdomain_update_by_ap(frw_event_stru *pst_event);
#endif
#endif

int32_t wal_set_random_mac_to_mib(oal_net_device_stru *pst_net_dev);

uint8_t *wal_get_reduce_sar_ctrl_params(uint8_t uc_tx_power_lvl);

wlan_p2p_mode_enum_uint8 wal_wireless_iftype_to_mac_p2p_mode(enum nl80211_iftype en_iftype);
uint32_t wal_hipriv_arp_offload_enable(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
uint32_t wal_hipriv_show_arpoffload_info(oal_net_device_stru *pst_net_dev, int8_t *pc_param);

#ifdef _PRE_WLAN_FEATURE_M2S_MSS
uint32_t wal_ioctl_set_m2s_mss(oal_net_device_stru *pst_net_dev, uint8_t uc_m2s_mode);
#endif
int32_t wal_cfg_vap_h2d_event(oal_net_device_stru *pst_net_dev);

uint32_t hwifi_config_init_dts_main(oal_net_device_stru *pst_cfg_net_dev);
int32_t wal_set_custom_process_func(void);
uint32_t hwifi_config_init_nvram_main(oal_net_device_stru *pst_cfg_net_dev);
uint32_t hwifi_config_init_nvram_main_1103(oal_net_device_stru *pst_cfg_net_dev);
uint32_t hwifi_config_init_nvram_main_1106(oal_net_device_stru *pst_cfg_net_dev);
void hwifi_1106_config_cpu_freq_ini_param(void);
void hwifi_config_cpu_freq_ini_param_1103(void);
void hwifi_config_host_global_ini_param_1103(void);
void hwifi_1106_config_host_global_ini_param(void);
uint32_t hwifi_config_init_dts_main_1103(oal_net_device_stru *pst_cfg_net_dev);
uint32_t hwifi_config_init_dts_main_1106(oal_net_device_stru *pst_cfg_net_dev);
uint32_t wal_custom_cali_1103(void);
uint32_t wal_1106_custom_cali(void);
uint32_t wal_custom_cali(void);
void hwifi_config_init_force(void);

void hwifi_config_host_global_ini_param(void);

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 10, 44))
uint32_t wal_hipriv_set_essid(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
int wal_ioctl_set_essid(oal_net_device_stru *pst_net_dev, oal_iw_request_info_stru *pst_info,
                                   oal_iwreq_data_union *pst_wrqu, char *pc_extra);
#endif
uint32_t wal_get_cmd_one_arg(int8_t *pc_cmd, int8_t *pc_arg,
                                          uint32_t ul_arg_len, uint32_t *pul_cmd_offset);
uint32_t wal_hipriv_send_cfg_uint32_data(oal_net_device_stru *pst_net_dev,
                                                      int8_t *pc_param, wlan_cfgid_enum_uint16 cfgid);
#ifdef _PRE_WLAN_CFGID_DEBUG
uint32_t wal_hipriv_get_debug_cmd_size(void);
#endif
uint32_t wal_hipriv_alg_cfg(oal_net_device_stru *pst_net_dev, int8_t *pc_param);

uint32_t wal_hipriv_parse_cmd(int8_t *pc_cmd);
int32_t wal_set_assigned_filter_enable(int32_t l_filter_id, int32_t l_on);
int32_t wal_set_ip_filter_enable(int32_t l_on);
int32_t wal_add_ip_filter_items(wal_hw_wifi_filter_item *pst_items, int32_t l_count);
int32_t wal_clear_ip_filter(void);
#ifdef _PRE_WLAN_FEATURE_M2S
uint32_t wal_hipriv_set_m2s_switch(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
#endif
int32_t wal_register_ip_filter(wal_hw_wlan_filter_ops *pg_st_ip_filter_ops);
int32_t wal_unregister_ip_filter(void);

int32_t wlan_enter_light_suspend(void);
int32_t wlan_exit_light_suspend(void);
void wal_register_lights_suspend(void);
void wal_unregister_lights_suspend(void);

#ifdef _PRE_WLAN_FIT_BASED_REALTIME_CALI
uint32_t wal_hipriv_dyn_cali_cfg(oal_net_device_stru *pst_net_dev, int8_t *puc_param);
#endif

uint32_t wal_ioctl_set_ap_mode(oal_net_device_stru *pst_net_dev, uint8_t uc_param);
uint32_t wal_hipriv_set_mcast_data_dscr_param(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
uint32_t wal_ioctl_get_psm_stat(oal_net_device_stru *pst_net_dev,
                                  mac_psm_query_type_enum_uint8 en_query_type,
                                  oal_ifreq_stru *pst_ifr);
uint32_t wal_hipriv_sta_pm_on(oal_net_device_stru *pst_cfg_net_dev, int8_t *pc_param);
uint32_t wal_hipriv_set_tlv_cmd(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
uint32_t wal_hipriv_set_val_cmd(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
uint32_t wal_hipriv_set_phy_debug_switch(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
void wal_vap_get_wlan_mode_para(wal_msg_write_stru *st_write_msg);
int32_t wal_host_dev_exit(oal_net_device_stru *pst_net_dev);
uint32_t wal_hipriv_setcountry(oal_net_device_stru *pst_net_dev, int8_t *pc_param);
uint32_t wal_ioctl_set_sta_ps_mode(oal_net_device_stru *pst_cfg_net_dev, int8_t uc_ps_mode);
uint32_t wal_ioctl_set_fast_sleep_para(oal_net_device_stru *pst_cfg_net_dev, int8_t *pc_param);
void wal_set_power_mgmt_on(uint8_t power_mgmt_flag);
int32_t wal_set_power_on(oal_net_device_stru *pst_net_dev, int32_t power_flag);
int32_t wal_ext_priv_cmd(oal_net_device_stru *pst_net_dev, oal_ifreq_stru *pst_ifr);
#ifdef _PRE_WLAN_FEATURE_CSI
uint32_t wal_ioctl_set_csi_switch(oal_net_device_stru *pst_cfg_net_dev, int8_t *pc_param);
#endif
uint32_t wal_ioctl_set_mode(oal_net_device_stru *net_dev, int8_t *pc_param);
uint32_t wal_ioctl_set_freq(oal_net_device_stru *net_dev, int8_t *pc_param);
#ifdef _PRE_WLAN_FEATURE_MONITOR
uint32_t wal_ioctl_set_sniffer(oal_net_device_stru *net_dev, int8_t *pc_param);
uint32_t wal_ioctl_set_monitor_mode(oal_net_device_stru *net_dev, int8_t *pc_param);
#endif
uint32_t wal_ioctl_set_al_rx(oal_net_device_stru *net_dev, int8_t *pc_param);
uint32_t wal_ioctl_set_pm_switch(mac_vap_stru *mac_vap, uint32_t *params);
#endif /* end of wal_linux_ioctl.h */
