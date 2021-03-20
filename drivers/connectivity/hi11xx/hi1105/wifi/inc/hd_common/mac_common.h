

#ifndef __MAC_COMMON_H__
#define __MAC_COMMON_H__
// 此处不加extern "C" UT编译不过
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 1 其他头文件包含 */
#include "oal_types.h"
#include "oal_ext_if.h"
#include "oam_ext_if.h"
#include "oal_mm.h"
#include "oal_net.h"
#include "frw_ext_if.h"
#include "wlan_types.h"
#include "hal_common.h"
#include "mac_frame.h"
#include "mac_device_common.h"
#include "mac_user_common.h"
#include "mac_vap_common.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_MAC_COMMON_H

#define DMAC_BA_SEQNO_MASK                0x0FFF /* max sequece number */
#define DMAC_BA_MAX_SEQNO_BY_TWO          2048
#define DMAC_BA_GREATER_THAN_SEQHI  1
#define DMAC_BA_BETWEEN_SEQLO_SEQHI 2
#define MAC_TX_CTL_SIZE oal_netbuf_cb_size()
#define DMAC_BA_SEQ_ADD(_seq1, _seq2)   (((_seq1) + (_seq2)) & DMAC_BA_SEQNO_MASK)
#define DMAC_BA_SEQ_SUB(_seq1, _seq2)   (((_seq1) - (_seq2)) & DMAC_BA_SEQNO_MASK)
#define DMAC_BA_SEQNO_ADD(_seq1, _seq2) (((_seq1) + (_seq2)) & DMAC_BA_SEQNO_MASK)
#define DMAC_BA_SEQNO_SUB(_seq1, _seq2) (((_seq1) - (_seq2)) & DMAC_BA_SEQNO_MASK)
#define MAC_INVALID_RX_BA_LUT_INDEX HAL_MAX_RX_BA_LUT_SIZE
/* 发送BA窗口记录seq number的最大个数，必须是2的整数次幂 */
#define DMAC_TID_MAX_BUFS 128
/* 发送BA窗口记录seq number的bitmap所使用的类型长度 */
#define DMAC_TX_BUF_BITMAP_WORD_SIZE 32
/* 发送BA窗口记录seq number的bit map的长度 */
#define DMAC_TX_BUF_BITMAP_WORDS \
    ((DMAC_TID_MAX_BUFS + DMAC_TX_BUF_BITMAP_WORD_SIZE - 1) / DMAC_TX_BUF_BITMAP_WORD_SIZE)
#define IS_RW_RING_EMPTY(read, write) \
    ((read)->us_rw_ptr == (write)->us_rw_ptr)

#define IS_RW_RING_FULL(read, write)                                    \
    (((read)->st_rw_ptr.bit_rw_ptr == (write)->st_rw_ptr.bit_rw_ptr) && \
        ((read)->st_rw_ptr.bit_wrap_flag != (write)->st_rw_ptr.bit_wrap_flag))

#define IS_RING_WRAP_AROUND(read, write) \
    ((read)->st_rw_ptr.bit_wrap_flag != (write)->st_rw_ptr.bit_wrap_flag)

/* 抛往dmac侧消息头的长度 */
#define CUSTOM_MSG_DATA_HDR_LEN (OAL_SIZEOF(custom_cfgid_enum_uint32) + OAL_SIZEOF(uint32_t))

#ifdef _PRE_WLAN_FEATURE_NRCOEX
#define DMAC_WLAN_NRCOEX_INTERFERE_RULE_NUM (4) /* 5gnr共存干扰参数表，目前共4组 */
#endif

/* 3 枚举定义 */
/*****************************************************************************
  枚举名  : dmac_tx_host_drx_subtype_enum_uint8
  协议表格:
  枚举说明: HOST DRX事件子类型定义
*****************************************************************************/
/* WLAN_CRX子类型定义 */
typedef enum {
    DMAC_TX_HOST_DRX = 0,
    HMAC_TX_HOST_DRX = 1,

    DMAC_TX_HOST_DRX_BUTT
} dmac_tx_host_drx_subtype_enum;
typedef uint8_t dmac_tx_host_drx_subtype_enum_uint8;

/*****************************************************************************
  枚举名  : dmac_tx_wlan_dtx_subtype_enum_uint8
  协议表格:
  枚举说明: WLAN DTX事件子类型定义
*****************************************************************************/
typedef enum {
    DMAC_TX_WLAN_DTX = 0,

    DMAC_TX_WLAN_DTX_BUTT
} dmac_tx_wlan_dtx_subtype_enum;
typedef uint8_t dmac_tx_wlan_dtx_subtype_enum_uint8;

/*****************************************************************************
  枚举名  : dmac_wlan_ctx_event_sub_type_enum_uint8
  协议表格:
  枚举说明: WLAN CTX事件子类型定义
*****************************************************************************/
typedef enum {
    DMAC_WLAN_CTX_EVENT_SUB_TYPE_MGMT = 0, /* 管理帧处理 */
    DMAC_WLAN_CTX_EVENT_SUB_TYPE_ADD_USER,
    DMAC_WLAN_CTX_EVENT_SUB_TYPE_NOTIFY_ALG_ADD_USER,
    DMAC_WLAN_CTX_EVENT_SUB_TYPE_DEL_USER,

    DMAC_WLAN_CTX_EVENT_SUB_TYPE_BA_SYNC,  /* 收到wlan的Delba和addba rsp用于到dmac的同步 */
    DMAC_WLAN_CTX_EVENT_SUB_TYPE_PRIV_REQ, /* 11N自定义的请求的事件类型 */

    DMAC_WLAN_CTX_EVENT_SUB_TYPE_SCAN_REQ,       /* 扫描请求 */
    DMAC_WLAN_CTX_EVENT_SUB_TYPE_SCHED_SCAN_REQ, /* PNO调度扫描请求 */
    DMAC_WLAN_CTX_EVENT_SUB_TYPE_RESET_PSM,      /* 收到认证请求 关联请求，复位用户的节能状态 */

    DMAC_WLAN_CTX_EVENT_SUB_TYPE_JOIN_SET_REG,
    DMAC_WLAN_CTX_EVENT_SUB_TYPE_JOIN_DTIM_TSF_REG,
    DMAC_WLAN_CTX_EVENT_SUB_TYPE_CONN_RESULT, /* 关联结果 */

    DMAC_WLAN_CTX_EVENT_SUB_TYPE_ASOC_WRITE_REG, /* AP侧处理关联时，修改SEQNUM_DUPDET_CTRL寄存器 */

    DMAC_WLAN_CTX_EVENT_SUB_TYPE_STA_SET_EDCA_REG,       /* STA收到beacon和assoc rsp时，更新EDCA寄存器 */

    DMAC_WLAN_CTX_EVENT_SUB_TYPE_SWITCH_TO_NEW_CHAN, /* 切换至新信道事件 */
    DMAC_WALN_CTX_EVENT_SUB_TYPR_SELECT_CHAN,        /* 设置信道事件 */
    DMAC_WALN_CTX_EVENT_SUB_TYPR_DISABLE_TX,         /* 禁止硬件发送 */
    DMAC_WALN_CTX_EVENT_SUB_TYPR_ENABLE_TX,          /* 恢复硬件发送 */
    DMAC_WLAN_CTX_EVENT_SUB_TYPR_RESTART_NETWORK,    /* 切换信道后，恢复BSS的运行 */
#ifdef _PRE_WLAN_FEATURE_DFS
#ifdef _PRE_WLAN_FEATURE_OFFCHAN_CAC
    DMAC_WLAN_CTX_EVENT_SUB_TYPR_SWITCH_TO_OFF_CHAN,  /* 切换到offchan做off-chan cac检测 */
    DMAC_WLAN_CTX_EVENT_SUB_TYPR_SWITCH_TO_HOME_CHAN, /* 切回home chan */
#endif
    DMAC_WLAN_CTX_EVENT_SUB_TYPR_DFS_TEST,
    DMAC_WALN_CTX_EVENT_SUB_TYPR_DFS_CAC_CTRL_TX, /* DFS 1min CAC把vap状态位置为pause或者up,同时禁止或者开启硬件发送 */
#endif

#ifdef _PRE_WLAN_FEATURE_EDCA_OPT_AP
    DMAC_WLAN_CTX_EVENT_SUB_TYPR_EDCA_OPT, /* edca优化中业务识别通知事件 */
#endif
    DMAC_WLAN_CTX_EVENT_SUB_TYPE_CALI_HMAC2DMAC,
    DMAC_WLAN_CTX_EVENT_SUB_TYPE_DSCR_OPT,
    DMAC_WLAN_CTX_EVENT_SUB_TYPE_CALI_MATRIX_HMAC2DMAC,
    DMAC_WLAN_CTX_EVENT_SUB_TYPE_APP_IE_H2D,
    DMAC_WLAN_CTX_EVENT_SUB_TYPE_IP_FILTER,
#ifdef _PRE_WLAN_FEATURE_APF
    DMAC_WLAN_CTX_EVENT_SUB_TYPE_APF_CMD,
#endif

#ifdef _PRE_WLAN_FEATURE_11AX
    DMAC_WLAN_CTX_EVENT_SUB_TYPE_STA_SET_MU_EDCA_REG, /* STA收到beacon和assoc rsp时，更新MU EDCA寄存器 */
    DMAC_WLAN_CTX_EVENT_SUB_TYPE_STA_SET_SPATIAL_REUSE_REG,
    DMAC_WLAN_CTX_EVENT_SUB_TYPE_UPDATE_TWT,                      /* STA收到twt 时，更新寄存器 */
    /* STA收到beacon和assoc rsp时，更新NDP Feedback report寄存器 */
    DMAC_WLAN_CTX_EVENT_SUB_TYPE_STA_SET_NDP_FEEDBACK_REPORT_REG,
#endif

    DMAC_WLAN_CTX_EVENT_SUB_TYPE_HIEX_H2D_MSG,

    DMAC_WLAN_CTX_EVENT_SUB_TYPE_CUST_HMAC2DMAC,

    DMAC_WLAN_CTX_EVENT_SUB_TYPE_BUTT
} dmac_wlan_ctx_event_sub_type_enum;
typedef uint8_t dmac_wlan_ctx_event_sub_type_enum_uint8;

/* DMAC模块 WLAN_DRX子类型定义 */
typedef enum {
    DMAC_WLAN_DRX_EVENT_SUB_TYPE_RX_DATA,        /* AP模式: DMAC WLAN DRX 流程 */
    DMAC_WLAN_DRX_EVENT_SUB_TYPE_TKIP_MIC_FAILE, /* DMAC tkip mic faile 上报给HMAC */
    DMAC_WLAN_DRX_EVENT_SUB_TYPE_WOW_RX_DATA,    /* WOW DMAC WLAN DRX 流程 */

    DMAC_WLAN_DRX_EVENT_SUB_TYPE_BUTT
} dmac_wlan_drx_event_sub_type_enum;
typedef uint8_t dmac_wlan_drx_event_sub_type_enum_uint8;

/* DMAC模块 WLAN_CRX子类型定义 */
typedef enum {
    DMAC_WLAN_CRX_EVENT_SUB_TYPE_INIT,              /* DMAC 给 HMAC的初始化参数 */
    DMAC_WLAN_CRX_EVENT_SUB_TYPE_RX,                /* DMAC WLAN CRX 流程 */
    DMAC_WLAN_CRX_EVENT_SUB_TYPE_DELBA,             /* DMAC自身产生的DELBA帧 */
    DMAC_WLAN_CRX_EVENT_SUB_TYPE_EVERY_SCAN_RESULT, /* 扫描到一个bss信息，上报结果 */
    DMAC_WLAN_CRX_EVENT_SUB_TYPE_SCAN_COMP,         /* DMAC扫描完成上报给HMAC */
    DMAC_WLAN_CRX_EVENT_SUB_TYPE_OBSS_SCAN_COMP,    /* DMAC OBSS扫描完成上报给HMAC */
    DMAC_WLAN_CRX_EVENT_SUB_TYPE_CHAN_RESULT,       /* 上报一个信道的扫描结果 */
    DMAC_WLAN_CRX_EVENT_SUB_TYPE_ACS_RESP,          /* DMAC ACS 回复应用层命令执行结果给HMAC */
    DMAC_WLAN_CRX_EVENT_SUB_TYPE_DISASSOC,          /* DMAC上报去关联事件到HMAC, HMAC会删除用户 */
    DMAC_WLAN_CRX_EVENT_SUB_TYPE_DEAUTH,            /* DMAC上报去认证事件到HMAC */

    DMAC_WLAN_CRX_EVENT_SUB_TYPR_CH_SWITCH_COMPLETE, /* 信道切换完成事件 */
    DMAC_WLAN_CRX_EVENT_SUB_TYPR_DBAC,               /* DBAC enable/disable事件 */

    DMAC_WLAN_CRX_EVENT_SUB_TYPE_HIEX_D2H_MSG,
    DMAC_WLAN_CRX_EVENT_SUB_TYPE_RX_WOW,
    DMAC_WLAN_CRX_EVENT_SUB_TYPE_EVERY_SCAN_RESULT_RING, /* ring方式接收的扫描结果报文上报 */

    DMAC_WLAN_CRX_EVENT_SUB_TYPE_TX_RING_ADDR,

    DMAC_WLAN_CRX_EVENT_SUB_TYPE_BUTT
} dmac_wlan_crx_event_sub_type_enum;
typedef uint8_t dmac_wlan_crx_event_sub_type_enum_uint8;

/* 发向HOST侧的配置事件 */
typedef enum {
    DMAC_TO_HMAC_SYN_UP_REG_VAL = 1,  // FRW_SDT_REG_EVENT_LOG_SYN_SUB_TYPE = 0
    DMAC_TO_HMAC_CREATE_BA = 2,
    DMAC_TO_HMAC_DEL_BA = 3,
    DMAC_TO_HMAC_SYN_CFG = 4,

    DMAC_TO_HMAC_ALG_INFO_SYN = 6,
    DMAC_TO_HMAC_VOICE_AGGR = 7,
#if defined(_PRE_WLAN_FEATURE_DATA_SAMPLE) || defined(_PRE_WLAN_FEATURE_PSD_ANALYSIS)
    DMAC_TO_HMAC_SYN_UP_SAMPLE_DATA = 8,
#endif

#ifdef _PRE_WLAN_FEATURE_M2S
    DMAC_TO_HMAC_M2S_DATA = 10,
#endif

    DMAC_TO_HMAC_BANDWIDTH_INFO_SYN = 11,  /* dmac向hmac同步带宽的信息 */
    DMAC_TO_HMAC_PROTECTION_INFO_SYN = 12, /* dmac向hmac同步保护mib信息 */
    DMAC_TO_HMAC_CH_STATUS_INFO_SYN = 13,  /* dmac向hmac同步可用信道列表 */

    /* 事件注册时候需要枚举值列出来，防止出现device侧和host特性宏打开不一致，
       造成出现同步事件未注册问题，后续各子特性人注意宏打开的一致性
    */
    DMAC_TO_HMAC_SYN_BUTT
} dmac_to_hmac_syn_type_enum;

/* hmac to dmac定制化配置同步消息结构 */
typedef enum {
    CUSTOM_CFGID_NV_ID = 0,
    CUSTOM_CFGID_INI_ID,
    CUSTOM_CFGID_DTS_ID,
    CUSTOM_CFGID_PRIV_INI_ID,
    CUSTOM_CFGID_CAP_ID,

    CUSTOM_CFGID_BUTT,
} custom_cfgid_enum;
typedef unsigned int custom_cfgid_enum_uint32;

typedef enum {
    CUSTOM_CFGID_INI_ENDING_ID = 0,
    CUSTOM_CFGID_INI_FREQ_ID,
    CUSTOM_CFGID_INI_PERF_ID,
    CUSTOM_CFGID_INI_LINKLOSS_ID,
    CUSTOM_CFGID_INI_PM_SWITCH_ID,
    CUSTOM_CFGID_INI_PS_FAST_CHECK_CNT_ID,
    CUSTOM_CFGID_INI_FAST_MODE,

    /* 私有定制 */
    CUSTOM_CFGID_PRIV_INI_RADIO_CAP_ID,
    CUSTOM_CFGID_PRIV_FASTSCAN_SWITCH_ID,
    CUSTOM_CFGID_PRIV_ANT_SWITCH_ID,
    CUSTOM_CFGID_PRIV_LINKLOSS_THRESHOLD_FIXED_ID,
    CUSTOM_CFGID_PRIV_RADAR_ISR_FORBID_ID,
    CUSTOM_CFGID_PRIV_INI_BW_MAX_WITH_ID,
    CUSTOM_CFGID_PRIV_INI_LDPC_CODING_ID,
    CUSTOM_CFGID_PRIV_INI_RX_STBC_ID,
    CUSTOM_CFGID_PRIV_INI_TX_STBC_ID,
    CUSTOM_CFGID_PRIV_INI_SU_BFER_ID,
    CUSTOM_CFGID_PRIV_INI_SU_BFEE_ID,
    CUSTOM_CFGID_PRIV_INI_MU_BFER_ID,
    CUSTOM_CFGID_PRIV_INI_MU_BFEE_ID,
    CUSTOM_CFGID_PRIV_INI_11N_TXBF_ID,
    CUSTOM_CFGID_PRIV_INI_1024_QAM_ID,

    CUSTOM_CFGID_PRIV_INI_CALI_MASK_ID,
    CUSTOM_CFGID_PRIV_CALI_DATA_MASK_ID,
    CUSTOM_CFGID_PRIV_INI_AUTOCALI_MASK_ID,
    CUSTOM_CFGID_PRIV_INI_DOWNLOAD_RATELIMIT_PPS,
    CUSTOM_CFGID_PRIV_INI_TXOPPS_SWITCH_ID,

    CUSTOM_CFGID_PRIV_INI_OVER_TEMPER_PROTECT_THRESHOLD_ID,
    CUSTOM_CFGID_PRIV_INI_TEMP_PRO_ENABLE_ID,
    CUSTOM_CFGID_PRIV_INI_TEMP_PRO_REDUCE_PWR_ENABLE_ID,
    CUSTOM_CFGID_PRIV_INI_TEMP_PRO_SAFE_TH_ID,
    CUSTOM_CFGID_PRIV_INI_TEMP_PRO_OVER_TH_ID,
    CUSTOM_CFGID_PRIV_INI_TEMP_PRO_PA_OFF_TH_ID,

    CUSTOM_CFGID_PRIV_INI_DSSS2OFDM_DBB_PWR_BO_VAL_ID,
    CUSTOM_CFGID_PRIV_INI_EVM_PLL_REG_FIX_ID,
    CUSTOM_CFGID_PRIV_INI_VOE_SWITCH_ID,
    CUSTOM_CFGID_PRIV_COUNTRYCODE_SELFSTUDY_CFG_ID,
    CUSTOM_CFGID_PRIV_M2S_FUNCTION_EXT_MASK_ID,
    CUSTOM_CFGID_PRIV_M2S_FUNCTION_MASK_ID,
    CUSTOM_CFGID_PRIV_MCM_CUSTOM_FUNCTION_MASK_ID,
    CUSTOM_CFGID_PRIV_MCM_FUNCTION_MASK_ID,
    CUSTOM_CFGID_PRIV_INI_11AX_SWITCH_ID,
    CUSTOM_CFGID_PRIV_INI_HTC_SWITCH_ID,
    CUSTOM_CFGID_PRIV_INI_AC_SUSPEND_ID,
    CUSTOM_CFGID_PRIV_INI_MBSSID_SWITCH_ID,
    CUSTOM_CFGID_PRIV_DYN_BYPASS_EXTLNA_ID,
    CUSTOM_CFGID_PRIV_CTRL_FRAME_TX_CHAIN_ID,

    CUSTOM_CFGID_PRIV_CTRL_UPC_FOR_18DBM_C0_ID,
    CUSTOM_CFGID_PRIV_CTRL_UPC_FOR_18DBM_C1_ID,
    CUSTOM_CFGID_PRIV_CTRL_11B_DOUBLE_CHAIN_BO_POW_ID,
    CUSTOM_CFGID_PRIV_INI_LDAC_M2S_TH_ID,
    CUSTOM_CFGID_PRIV_INI_BTCOEX_MCM_TH_ID,
    CUSTOM_CFGID_PRIV_INI_NRCOEX_ID,
    CUSTOM_CFGID_PRIV_INI_HCC_FLOWCTRL_TYPE_ID,
    CUSTOM_CFGID_PRIV_INI_MBO_SWITCH_ID,

    CUSTOM_CFGID_PRIV_INI_DYNAMIC_DBAC_ID,
    CUSTOM_CFGID_PRIV_INI_PHY_CAP_SWITCH_ID,
    CUSTOM_CFGID_PRIV_DC_FLOWCTRL_ID,
    CUSTOM_CFGID_PRIV_INI_HAL_PS_RSSI_ID,
    CUSTOM_CFGID_PRIV_INI_HAL_PS_PPS_ID,
    CUSTOM_CFGID_PRIV_INI_OPTIMIZED_FEATURE_SWITCH_ID,
    CUSTOM_CFGID_PRIV_DDR_SWITCH_ID,
    CUSTOM_CFGID_PRIV_FEM_POW_CTL_ID,
    CUSTOM_CFGID_PRIV_INI_HIEX_CAP_ID,
    CUSTOM_CFGID_PRIV_INI_TX_SWITCH_ID,
    CUSTOM_CFGID_PRIV_INI_FTM_CAP_ID,

    CUSTOM_CFGID_INI_BUTT,
} custom_cfgid_h2d_ini_enum;
typedef unsigned int custom_cfgid_h2d_ini_enum_uint32;

enum custom_optimize_feature {
    CUSTOM_OPTIMIZE_FEATURE_RA_FLT = 0,
    CUSTOM_OPTIMIZE_FEATURE_NAN = 1,
    CUSTOM_OPTIMIZE_FEATURE_CERTIFY_MODE = 2,
    CUSTOM_OPTIMIZE_TXOP_COT = 3,
    CUSTOM_OPTIMIZE_FEATURE_BUTT,
};

typedef struct {
    custom_cfgid_enum_uint32 en_syn_id; /* 同步配置ID */
    uint32_t ul_len;                  /* DATA payload长度 */
    uint8_t auc_msg_body[4];          /* DATA payload */
} hmac_to_dmac_cfg_custom_data_stru;

/* MISC杂散事件 */
typedef enum {
    DMAC_MISC_SUB_TYPE_RADAR_DETECT,
    DMAC_MISC_SUB_TYPE_DISASOC,
    DMAC_MISC_SUB_TYPE_CALI_TO_HMAC,
    DMAC_MISC_SUB_TYPE_HMAC_TO_CALI,

    DMAC_MISC_SUB_TYPE_ROAM_TRIGGER,

#ifdef _PRE_WLAN_ONLINE_DPD
    DMAC_TO_HMAC_DPD,
#endif

#ifdef _PRE_WLAN_FEATURE_APF
    DMAC_MISC_SUB_TYPE_APF_REPORT,
#endif
    HAL_EVENT_HAL_RX_RESET_SMAC,
    DMAC_MISC_SUB_TYPE_BUTT
} dmac_misc_sub_type_enum;

typedef enum {
    DMAC_DISASOC_MISC_LINKLOSS = 0,
    DMAC_DISASOC_MISC_KEEPALIVE = 1,
    DMAC_DISASOC_MISC_CHANNEL_MISMATCH = 2,
    DMAC_DISASOC_MISC_LOW_RSSI = 3,
    DMAC_DISASOC_MISC_GET_CHANNEL_IDX_FAIL = 5,
    DMAC_DISASOC_MISC_BUTT
} dmac_disasoc_misc_reason_enum;
typedef uint16_t dmac_disasoc_misc_reason_enum_uint16;

/* HMAC to DMAC同步类型 */
typedef enum {
    HMAC_TO_DMAC_SYN_INIT,
    HMAC_TO_DMAC_SYN_CREATE_CFG_VAP,
    HMAC_TO_DMAC_SYN_CFG,
    HMAC_TO_DMAC_SYN_ALG,
    HMAC_TO_DMAC_SYN_REG,
#if defined(_PRE_WLAN_FEATURE_DATA_SAMPLE) || defined(_PRE_WLAN_FEATURE_PSD_ANALYSIS)
    HMAC_TO_DMAC_SYN_SAMPLE,
#endif

    HMAC_TO_DMAC_SYN_BUTT
} hmac_to_dmac_syn_type_enum;
typedef uint8_t hmac_to_dmac_syn_type_enum_uint8;

/* 算法的报文探测标志 (注:对于1102该枚举只允许使用3bit空间, 因此有效枚举值最大为7) */
typedef enum {
    DMAC_USER_ALG_NON_PROBE = 0,               /* 非探测报文 */
    DMAC_USER_ALG_TXBF_SOUNDING = 1,           /* TxBf sounding报文 */
    DMAC_USER_ALG_AUOTRATE_PROBE = 2,          /* Autorate探测报文 */
    DMAC_USER_ALG_AGGR_PROBE = 3,              /* 聚合探测报文 */
    DMAC_USER_ALG_TPC_PROBE = 4,               /* TPC探测报文 */
    DMAC_USER_ALG_TX_MODE_PROBE = 5,           /* 发送模式探测报文(TxBf, STBC, Chain) */
    DMAC_USER_ALG_SMARTANT_NULLDATA_PROBE = 6, /* 智能天线NullData训练报文 */
    DMAC_USER_ALG_SMARTANT_DATA_PROBE = 7,     /* 智能天线Data训练报文 */

    DMAC_USER_ALG_PROBE_BUTT
} dmac_user_alg_probe_enum;
typedef uint8_t dmac_user_alg_probe_enum_uint8;

/* BA会话的状态枚举 */
typedef enum {
    DMAC_BA_INIT = 0,   /* BA会话未建立 */
    DMAC_BA_INPROGRESS, /* BA会话建立过程中 */
    DMAC_BA_COMPLETE,   /* BA会话建立完成 */
    DMAC_BA_HALTED,     /* BA会话节能暂停 */
    DMAC_BA_FAILED,     /* BA会话建立失败 */

    DMAC_BA_BUTT
} dmac_ba_conn_status_enum;
typedef uint8_t dmac_ba_conn_status_enum_uint8;

/* 专用于CB字段自定义帧类型、帧子类型枚举值 */
typedef enum {
    WLAN_CB_FRAME_TYPE_START = 0,  /* cb默认初始化为0 */
    WLAN_CB_FRAME_TYPE_ACTION = 1, /* action帧 */
    WLAN_CB_FRAME_TYPE_DATA = 2,   /* 数据帧 */
    WLAN_CB_FRAME_TYPE_MGMT = 3,   /* 管理帧，用于p2p等需要上报host */

    WLAN_CB_FRAME_TYPE_BUTT
} wlan_cb_frame_type_enum;
typedef uint8_t wlan_cb_frame_type_enum_uint8;

/* cb字段action帧子类型枚举定义 */
typedef enum {
    WLAN_ACTION_BA_ADDBA_REQ = 0, /* 聚合action */
    WLAN_ACTION_BA_ADDBA_RSP = 1,
    WLAN_ACTION_BA_DELBA = 2,
#ifdef _PRE_WLAN_FEATURE_WMMAC
    WLAN_ACTION_BA_WMMAC_ADDTS_REQ = 3,
    WLAN_ACTION_BA_WMMAC_ADDTS_RSP = 4,
    WLAN_ACTION_BA_WMMAC_DELTS = 5,
#endif
    WLAN_ACTION_SMPS_FRAME_SUBTYPE = 6,   /* SMPS节能action */
    WLAN_ACTION_OPMODE_FRAME_SUBTYPE = 7, /* 工作模式通知action */
    WLAN_ACTION_P2PGO_FRAME_SUBTYPE = 8,  /* host发送的P2P go帧，主要是discoverability request */

#ifdef _PRE_WLAN_FEATURE_TWT
    WLAN_ACTION_TWT_SETUP_REQ = 9,
    WLAN_ACTION_TWT_TEARDOWN_REQ = 10,
#endif
#ifdef _PRE_WLAN_FEATURE_HIEX
    WLAN_ACTION_HIEX = 11,
#endif
#ifdef _PRE_WLAN_FEATURE_NAN
    WLAN_ACTION_NAN_PUBLISH = 12,
    WLAN_ACTION_NAN_FLLOWUP = 13,
    WLAN_ACTION_NAN_SUBSCRIBE = 14,
#endif
    WLAN_FRAME_TYPE_ACTION_BUTT
} wlan_cb_action_subtype_enum;
typedef uint8_t wlan_cb_frame_subtype_enum_uint8;

/* 配置命令带宽能力枚举 */
typedef enum {
    WLAN_BANDWITH_CAP_20M,
    WLAN_BANDWITH_CAP_40M,
    WLAN_BANDWITH_CAP_40M_DUP,
    WLAN_BANDWITH_CAP_80M,
    WLAN_BANDWITH_CAP_80M_DUP,
    WLAN_BANDWITH_CAP_160M,
    WLAN_BANDWITH_CAP_160M_DUP,
    WLAN_BANDWITH_CAP_80PLUS80,
#ifdef  _PRE_WLAN_FEATURE_11AX_ER_SU_DCM
    WLAN_BANDWITH_CAP_ER_242_TONE,
    WLAN_BANDWITH_CAP_ER_106_TONE,
#endif
    WLAN_BANDWITH_CAP_BUTT
} wlan_bandwith_cap_enum;
typedef uint8_t wlan_bandwith_cap_enum_uint8;

/* 4 全局变量声明 */
/* 5 消息头定义 */
/* 6 消息定义 */
/* 7 STRUCT定义 */
/* DMAC_WLAN_CRX_EVENT_SUB_TYPE_SCAN_COMP */
typedef struct {
    dmac_disasoc_misc_reason_enum_uint16 en_disasoc_reason;
    uint16_t us_user_idx;
} dmac_disasoc_misc_stru;
/* DMAC与HMAC模块共用的WLAN DRX事件结构体 */
typedef struct {
    oal_netbuf_stru *pst_netbuf; /* netbuf链表一个元素 */
    uint16_t us_netbuf_num;    /* netbuf链表的个数 */
    uint8_t auc_resv[2];       /* 字节对齐 */
} dmac_wlan_drx_event_stru;

/* DMAC与HMAC模块共用的WLAN CRX事件结构体 */
typedef struct {
    oal_netbuf_stru *pst_netbuf; /* 指向管理帧对应的netbuf */
    //    uint8_t              *puc_chtxt;          /* Shared Key认证用的challenge text */
} dmac_wlan_crx_event_stru;

typedef struct {
    uint16_t us_user_id;
    uint8_t uc_ba_size;
    oal_bool_enum_uint8 en_need_delba;
} dmac_to_hmac_btcoex_rx_delba_trigger_event_stru;

typedef struct {
    uint16_t us_user_id;
    oal_bool_enum_uint8 en_rx_amsdu;
    uint8_t uc_ps_type;
} dmac_to_hmac_ps_rx_delba_event_stru;

typedef struct {
    uint16_t us_user_id;
    oal_bool_enum_uint8 en_arp_probe;
    uint8_t uc_ps_type;
} dmac_to_hmac_ps_arp_probe_event_stru;

typedef struct {
    uint16_t us_user_index;
    uint8_t uc_tid;
    uint8_t uc_vap_id;
    uint8_t uc_cur_protocol;
    uint8_t auc_reserve[3];
} dmac_to_hmac_ctx_event_stru;

typedef struct {
    uint16_t us_user_index;
    uint8_t uc_cur_bandwidth;
    uint8_t uc_cur_protocol;
} dmac_to_hmac_syn_info_event_stru;

typedef struct {
    uint8_t uc_vap_id;
    uint8_t en_voice_aggr; /* 是否支持Voice聚合 */
    uint8_t auc_resv[2];
} dmac_to_hmac_voice_aggr_event_stru;

typedef struct {
    uint8_t uc_device_id;
    wlan_nss_enum_uint8 en_m2s_nss;
    wlan_m2s_type_enum_uint8 en_m2s_type; /* 0:软切换 1:硬切换 */
    uint8_t auc_reserve[1];
} dmac_to_hmac_m2s_event_stru;

/* d2h hal vap信息同步 */
typedef struct {
    uint8_t hal_dev_id;
    uint8_t mac_vap_id;
    uint8_t hal_vap_id;
    uint8_t valid;
} d2h_hal_vap_info_syn_event;

/* d2h lut信息同步 */
typedef struct {
    uint8_t  lut_idx;
    uint16_t usr_idx;
    uint8_t  hal_dev_id;
    uint8_t  valid;
} d2h_usr_lut_info_syn_event;

/* mic攻击 */
typedef struct {
    uint8_t auc_user_mac[WLAN_MAC_ADDR_LEN];
    uint8_t auc_reserve[2];
    oal_nl80211_key_type en_key_type;
    int32_t l_key_id;
} dmac_to_hmac_mic_event_stru;

/* DMAC与HMAC模块共用的DTX事件结构体 */
typedef struct {
    oal_netbuf_stru *pst_netbuf; /* netbuf链表一个元素 */
    uint16_t us_frame_len;
    uint16_t us_remain;
} dmac_tx_event_stru;
/* 存在MIMO兼容性AP，需抛事件通知hmac重关联，事件结构体 */
typedef struct {
    uint16_t us_user_id;
    mac_ap_type_enum_uint16 en_ap_type;
    uint8_t auc_mac_addr[WLAN_MAC_ADDR_LEN];
    uint8_t auc_resv[2];
} dmac_to_hmac_mimo_compatibility_event_stru;

typedef struct {
    mac_channel_stru st_channel;
    mac_ch_switch_info_stru st_ch_switch_info;

    oal_bool_enum_uint8 en_switch_immediately; /* 1 - 马上切换  0 - 暂不切换, 推迟到tbtt中切换 */
    oal_bool_enum_uint8 en_check_cac;
    oal_bool_enum_uint8 en_dot11FortyMHzIntolerant;
    uint8_t auc_resv[1];
} dmac_set_chan_stru;

typedef struct {
    wlan_ch_switch_status_enum_uint8 en_ch_switch_status;     /* 信道切换状态 */
    uint8_t uc_announced_channel;                           /* 新信道号 */
    wlan_channel_bandwidth_enum_uint8 en_announced_bandwidth; /* 新带宽模式 */
    uint8_t uc_ch_switch_cnt;                               /* 信道切换计数 */
    oal_bool_enum_uint8 en_csa_present_in_bcn;                /* Beacon帧中是否包含CSA IE */
    uint8_t uc_csa_vap_cnt;                                 /* 需要发送csa的vap个数 */
    wlan_csa_mode_tx_enum_uint8 en_csa_mode;
} dmac_set_ch_switch_info_stru;

typedef struct {
    uint8_t uc_cac_machw_en; /* 1min cac 发送队列开关状态 */
} dmac_set_cac_machw_info_stru;

#ifdef _PRE_WLAN_FEATURE_WMMAC
/* 挂载在dmac_ctx_action_event_stru.uc_resv[2]传递给dmac */
typedef struct {
    uint8_t uc_ac;
    uint8_t bit_psb : 1;
    uint8_t bit_direction : 7;
} dmac_addts_info_stru;
#endif

/*
 *  (1)DMAC与HMAC模块共用的CTX子类型ACTION对应的事件的结构体
 *  (2)当DMAC自身产生DELBA帧时，使用该结构体向HMAC模块抛事件
 */
typedef struct {
    mac_category_enum_uint8 en_action_category; /* ACTION帧的类型 */
    uint8_t uc_action;                        /* 不同ACTION类下的子帧类型 */
    uint16_t us_user_idx;
    uint16_t us_frame_len; /* 帧长度 */
    uint8_t uc_hdr_len;    /* 帧头长度 */
    uint8_t uc_tidno;      /* tidno，部分action帧使用 */
    uint8_t uc_initiator;  /* 触发端方向 */

    /* 以下为接收到req帧，发送rsp帧后，需要同步到dmac的内容 */
    uint8_t uc_status;                       /* rsp帧中的状态 */
    uint16_t us_baw_start;                   /* 窗口开始序列号 */
    uint16_t us_baw_size;                    /* 窗口大小 */
    uint8_t uc_ampdu_max_num;                /* BA会话下的最多聚合的AMPDU的个数 */
    oal_bool_enum_uint8 en_amsdu_supp;         /* 是否支持AMSDU */
    uint16_t us_ba_timeout;                  /* BA会话交互超时时间 */
    mac_back_variant_enum_uint8 en_back_var;   /* BA会话的变体 */
    uint8_t uc_dialog_token;                 /* ADDBA交互帧的dialog token */
    uint8_t uc_ba_policy;                    /* Immediate=1 Delayed=0 */
    uint8_t uc_lut_index;                    /* LUT索引 */
    uint8_t auc_mac_addr[WLAN_MAC_ADDR_LEN]; /* 用于DELBA/DELTS查找HMAC用户 */
#ifdef _PRE_WLAN_FEATURE_WMMAC
    uint8_t uc_tsid;            /* TS相关Action帧中的tsid值 */
    uint8_t uc_ts_dialog_token; /* ADDTS交互帧的dialog token */
    dmac_addts_info_stru st_addts_info;
#endif
} dmac_ctx_action_event_stru;

#ifdef _PRE_WLAN_FEATURE_TWT
typedef struct {
    mac_category_enum_uint8 en_action_category; /* ACTION帧的类型 */
    uint8_t uc_action;                        /* 不同ACTION类下的子帧类型 */
    uint16_t us_user_idx;
    mac_cfg_twt_stru st_twt_cfg;
} dmac_ctx_update_twt_stru;
#endif

/* 添加用户事件payload结构体 */
typedef struct {
    uint16_t us_user_idx; /* 用户index */
    uint8_t auc_user_mac_addr[WLAN_MAC_ADDR_LEN];
    uint16_t us_sta_aid;
    uint8_t auc_bssid[WLAN_MAC_ADDR_LEN];

    mac_vht_hdl_stru st_vht_hdl;
    mac_user_ht_hdl_stru st_ht_hdl;
    mac_ap_type_enum_uint16 en_ap_type;
    int8_t c_rssi; /* 用户bss的信号强度 */
    uint8_t lut_index;
} dmac_ctx_add_user_stru;

/* 删除用户事件结构体 */
typedef struct {
    uint16_t us_user_idx; /* 用户index */
    uint8_t auc_user_mac_addr[WLAN_MAC_ADDR_LEN];
    mac_ap_type_enum_uint16 en_ap_type;
    uint8_t auc_resv[2];
} dmac_ctx_del_user_stru;

typedef struct {
    uint8_t uc_scan_idx;
    uint8_t auc_resv[3];
    wlan_scan_chan_stats_stru st_chan_result;
} dmac_crx_chan_result_stru;

/* Update join req 需要配置的速率集参数 */
typedef struct {
    union {
        uint8_t uc_value;
        struct {
            uint8_t bit_support_11b : 1;  /* 该AP是否支持11b */
            uint8_t bit_support_11ag : 1; /* 该AP是否支持11ag */
            uint8_t bit_ht_capable : 1;   /* 是否支持ht */
            uint8_t bit_vht_capable : 1;  /* 是否支持vht */
            uint8_t bit_reserved : 4;
        } st_capable; /* 与dmac层wlan_phy_protocol_enum对应 */
    } un_capable_flag;
    uint8_t uc_min_rate[2]; /* 第一个存储11b协议对应的速率，第二个存储11ag协议对应的速率 */
    uint8_t uc_reserved;
} dmac_set_rate_stru;

/* Update join req 参数写寄存器的结构体定义 */
typedef struct {
    uint8_t auc_bssid[WLAN_MAC_ADDR_LEN]; /* 加入的AP的BSSID  */
    uint16_t us_beacon_period;
    mac_channel_stru st_current_channel;                      /* 要切换的信道信息 */
    uint32_t ul_beacon_filter;                              /* 过滤beacon帧的滤波器开启标识位 */
    uint32_t ul_non_frame_filter;                           /* 过滤no_frame帧的滤波器开启标识位 */
    uint8_t auc_ssid[WLAN_SSID_MAX_LEN];                    /* 加入的AP的SSID  */
    uint8_t uc_dtim_period;                                 /* dtim period      */
    oal_bool_enum_uint8 en_dot11FortyMHzOperationImplemented; /* dot11FortyMHzOperationImplemented */
    uint8_t   bit_SetHeCap : 1,
              bit_SetVHTCap : 1,
              bit_rsv : 6;
    dmac_set_rate_stru st_min_rate; /* Update join req 需要配置的速率集参数 */
    mac_multi_bssid_info st_mbssid_info;
    mac_ap_type_enum_uint16 en_ap_type;
} dmac_ctx_join_req_set_reg_stru;

/* wait join写寄存器参数的结构体定义 */
typedef struct {
    uint32_t ul_dtim_period;              /* dtim period */
    uint32_t ul_dtim_cnt;                 /* dtim count  */
    uint8_t auc_bssid[WLAN_MAC_ADDR_LEN]; /* 加入的AP的BSSID  */
    uint16_t us_tsf_bit0;                 /* tsf bit0  */
} dmac_ctx_set_dtim_tsf_reg_stru;
/* wait join写寄存器参数的结构体定义 */
typedef struct {
    uint16_t uc_user_index; /* user index */
    uint8_t auc_resv[2];
} dmac_ctx_asoc_set_reg_stru;

/* sta更新edca参数寄存器的结构体定义 */
typedef struct {
    uint8_t uc_vap_id;
    mac_wmm_set_param_type_enum_uint8 en_set_param_type;
    uint16_t us_user_index;
    wlan_mib_Dot11QAPEDCAEntry_stru ast_wlan_mib_qap_edac[WLAN_WME_AC_BUTT];
} dmac_ctx_sta_asoc_set_edca_reg_stru;
/* sta更新edca参数寄存器的结构体定义 */
/* sta更新sr参数寄存器的结构体定义 */
#ifdef _PRE_WLAN_FEATURE_11AX
typedef struct {
    mac_frame_he_spatial_reuse_parameter_set_ie_stru st_sr_ie;
} dmac_ctx_sta_asoc_set_sr_reg_stru;
#endif

typedef hal_rx_ctl_stru mac_rx_ctl_stru;

/* DMAC模块接收流程控制信息结构，存放于对应的netbuf的CB字段中，最大值为48字节,
   如果修改，一定要通知sdt同步修改，否则解析会有错误!!!!!!!!!!!!!!!!!!!!!!!!! */
typedef struct {
    hal_rx_ctl_stru st_rx_info;            /* dmac需要传给hmac的数据信息 */
    hal_rx_status_stru st_rx_status;       /* 保存加密类型及帧长信息 */
    hal_rx_statistic_stru st_rx_statistic; /* 保存接收描述符的统计信息 */
} dmac_rx_ctl_stru;

/* hmac to dmac配置同步消息结构 */
typedef struct {
    wlan_cfgid_enum_uint16 en_syn_id; /* 同步事件ID */
    uint16_t us_len;                /* 事件payload长度 */
    uint8_t auc_msg_body[4];        /* 事件payload */
} hmac_to_dmac_cfg_msg_stru;
typedef hmac_to_dmac_cfg_msg_stru dmac_to_hmac_cfg_msg_stru;

/* 1字节对齐 */
#pragma pack(push, 1)
typedef struct {
    mac_ieee80211_frame_stru *pst_frame_header; /* 该MPDU的帧头指针 */
    uint16_t us_seqnum;                       /* 记录软件分配的seqnum */
    wlan_frame_type_enum_uint8 en_frame_type;   /* 该帧是控制针、管理帧、数据帧 */
    uint8_t bit_80211_mac_head_type : 1;      /* 0: 802.11 mac头不在skb中,另外申请了内存存放;1:802.11 mac头在skb中 */
    uint8_t en_res : 7;                       /* 是否使用4地址，由WDS特性决定 */
} mac_tx_expand_cb_stru;

/* 裸系统cb字段 只有20字节可用, 当前使用19字节; HCC[8]+PAD[1]+CB[19]+MAC HEAD[36] */
struct mac_tx_ctl {
    /* byte1 */
    /* 取值:FRW_EVENT_TYPE_WLAN_DTX和FRW_EVENT_TYPE_HOST_DRX，作用:在释放时区分是内存池的netbuf还是原生态的 */
    frw_event_type_enum_uint8 bit_event_type : 5;
    uint8_t bit_event_sub_type : 3;
    /* byte2-3 */
    wlan_cb_frame_type_enum_uint8 uc_frame_type;       /* 自定义帧类型 */
    wlan_cb_frame_subtype_enum_uint8 uc_frame_subtype; /* 自定义帧子类型 */
    /* byte4 */
    uint8_t bit_mpdu_num : 7;   /* ampdu中包含的MPDU个数,实际描述符填写的值为此值-1 */
    uint8_t bit_netbuf_num : 1; /* 每个MPDU占用的netbuf数目 */
    /* 在每个MPDU的第一个NETBUF中有效 */
    /* byte5-6 */
    uint16_t us_mpdu_payload_len; /* 每个MPDU的长度不包括mac header length */
    /* byte7 */
    uint8_t bit_frame_header_length : 6; /* 51四地址32 */ /* 该MPDU的802.11头长度 */
    uint8_t bit_is_amsdu : 1;                             /* 是否AMSDU: OAL_FALSE不是，OAL_TRUE是 */
    uint8_t bit_is_first_msdu : 1;                        /* 是否是第一个子帧，OAL_FALSE不是 OAL_TRUE是 */
    /* byte8 */
    uint8_t bit_tid : 4;                  /* dmac tx 到 tx complete 传递的user结构体，目标用户地址 */
    wlan_wme_ac_type_enum_uint8 bit_ac : 3; /* ac */
    uint8_t bit_ismcast : 1;              /* 该MPDU是单播还是多播:OAL_FALSE单播，OAL_TRUE多播 */
    /* byte9 */
    uint8_t bit_retried_num : 4;   /* 重传次数 */
    uint8_t bit_mgmt_frame_id : 4; /* wpas 发送管理帧的frame id */
    /* byte10 */
    uint8_t bit_tx_user_idx : 6;          /* 比描述符中userindex多一个bit用于标识无效index */
    uint8_t bit_roam_data : 1;            /* 漫游期间帧发送标记 */
    uint8_t bit_is_get_from_ps_queue : 1; /* 节能特性用，标识一个MPDU是否从节能队列中取出来的 */
    /* byte11 */
    uint8_t bit_tx_vap_index : 3;
    wlan_tx_ack_policy_enum_uint8 en_ack_policy : 3;
    uint8_t bit_is_needretry : 1;
    uint8_t bit_need_rsp : 1; /* WPAS send mgmt,need dmac response tx status */
    /* byte12 */
    dmac_user_alg_probe_enum_uint8 en_is_probe_data : 3; /* 是否探测帧 */
    uint8_t bit_is_eapol_key_ptk : 1;                  /* 4 次握手过程中设置单播密钥EAPOL KEY 帧标识 */
    uint8_t bit_is_m2u_data : 1;                       /* 是否是组播转单播的数据 */
    uint8_t bit_is_tcp_ack : 1;                        /* 用于标记tcp ack */
    uint8_t bit_is_rtsp : 1;
    uint8_t en_use_4_addr : 1; /* 是否使用4地址，由WDS特性决定 */
    /* byte13-16 */
    uint16_t us_timestamp_ms;   /* 维测使用入TID队列的时间戳, 单位1ms精度 */
    uint8_t bit_is_qosnull : 1; /* 用于标记qos null帧 */
    uint8_t bit_is_himit : 1; /* 用于标记himit帧 */
    uint8_t bit_hiex_traced : 1; /* 用于标记hiex帧 */
    uint8_t bit_is_game_marked : 1; /* 用于标记游戏帧 */
    uint8_t bit_is_hid2d_pkt : 1; /* 用于标记hid2d投屏数据帧 */
    uint8_t bit_is_802_3_snap : 1; /* 是否是802.3 snap */
    uint8_t uc_reserved1 : 2;
    uint8_t uc_data_type; /* 数据帧类型, ring tx使用, 对应data_type_enum */
    /* byte17-18 */
    uint8_t uc_alg_pktno;     /* 算法用到的字段，唯一标示该报文 */
    uint8_t uc_alg_frame_tag : 2; /* 用于算法对帧进行标记 */
    uint8_t uc_hid2d_tx_delay_time : 6; /* 用于保存hid2d数据帧在dmac的允许传输时间 */
    /* byte19 */
    uint8_t bit_large_amsdu_level : 2;  /* offload下大包AMSDU聚合度 */
    uint8_t bit_align_padding : 2;      /* SKB 头部包含ETHER HEADER时,4字节对齐需要的padding */
    uint8_t bit_msdu_num_in_amsdu : 2;  /* 大包聚合时每个AMSDU子帧数 */
    uint8_t bit_is_large_skb_amsdu : 1; /* 是否是多子帧大包聚合 */
    uint8_t bit_htc_flag : 1;           /* 用于标记htc */

#ifndef _PRE_PRODUCT_ID_HI110X_DEV
    /* OFFLOAD架构下，HOST相对DEVICE的CB增量 */
    mac_tx_expand_cb_stru st_expand_cb;
#endif
} __OAL_DECLARE_PACKED;
typedef struct mac_tx_ctl mac_tx_ctl_stru;
#pragma pack(pop)

typedef struct {
    void *pst_ba;
    uint8_t uc_tid;
    mac_delba_initiator_enum_uint8 en_direction;
    uint16_t us_mac_user_idx;
    uint8_t uc_vap_id;
    uint16_t us_timeout_times;
    uint8_t uc_resv[1];
} dmac_ba_alarm_stru;

typedef struct {
    uint16_t us_baw_start;                          /* 第一个未确认的MPDU的序列号 */
    uint16_t us_last_seq_num;                       /* 最后一个发送的MPDU的序列号 */
    uint16_t us_baw_size;                           /* Block_Ack会话的buffer size大小 */
    uint16_t us_baw_head;                           /* bitmap中记录的第一个未确认的包的位置 */
    uint16_t us_baw_tail;                           /* bitmap中下一个未使用的位置 */
    oal_bool_enum_uint8 en_is_ba;                     /* Session Valid Flag */
    dmac_ba_conn_status_enum_uint8 en_ba_conn_status; /* BA会话的状态 */
    mac_back_variant_enum_uint8 en_back_var;          /* BA会话的变体 */
    uint8_t uc_dialog_token;                        /* ADDBA交互帧的dialog token */
    uint8_t uc_ba_policy;                           /* Immediate=1 Delayed=0 */
    oal_bool_enum_uint8 en_amsdu_supp;                /* BLOCK ACK支持AMSDU的标识 */
    uint8_t *puc_dst_addr;                          /* BA会话接收端地址 */
    uint16_t us_ba_timeout;                         /* BA会话交互超时时间 */
    uint8_t uc_ampdu_max_num;                       /* BA会话下，能够聚合的最大的mpdu的个数 */
    uint8_t uc_tx_ba_lut;                           /* BA会话LUT session index */
    uint16_t us_mac_user_idx;
#ifdef _PRE_WLAN_FEATURE_DFR
    uint16_t us_pre_baw_start;    /* 记录前一次判断ba窗是否卡死时的ssn */
    uint16_t us_pre_last_seq_num; /* 记录前一次判断ba窗是否卡死时的lsn */
    uint16_t us_ba_jamed_cnt;     /* BA窗卡死统计次数 */
#else
    uint8_t auc_resv[2];
#endif
    uint32_t aul_tx_buf_bitmap[DMAC_TX_BUF_BITMAP_WORDS];
} dmac_ba_tx_stru;

typedef struct {
    uint8_t uc_fem_delt_pow;
    uint8_t uc_tpc_adj_pow_start_idx;
    uint8_t auc_resv[2];
} dmac_ax_fem_pow_ctl_stru;

/* 处理MPDU的MSDU的处理状态的结构体的定义 */
typedef struct {
    oal_netbuf_stru *pst_curr_netbuf;     /* 当前处理的netbuf指针 */
    uint8_t *puc_curr_netbuf_data;      /* 当前处理的netbuf的data指针 */
    uint16_t us_submsdu_offset;         /* 当前处理的submsdu的偏移量,   */
    uint8_t uc_msdu_nums_in_netbuf;     /* 当前netbuf包含的总的msdu数目 */
    uint8_t uc_procd_msdu_in_netbuf;    /* 当前netbuf中已处理的msdu数目 */
    uint8_t uc_netbuf_nums_in_mpdu;     /* 当前MPDU的中的总的netbuf的数目 */
    uint8_t uc_procd_netbuf_nums;       /* 当前MPDU中已处理的netbuf的数目 */
    uint8_t uc_procd_msdu_nums_in_mpdu; /* 当前MPDU中已处理的MSDU数目 */

    uint8_t uc_flag;
} dmac_msdu_proc_state_stru;

/* 每一个MSDU包含的内容的结构体的定义 */
typedef struct {
    uint8_t auc_sa[WLAN_MAC_ADDR_LEN]; /* MSDU发送的源地址 */
    uint8_t auc_da[WLAN_MAC_ADDR_LEN]; /* MSDU接收的目的地址 */
    uint8_t auc_ta[WLAN_MAC_ADDR_LEN]; /* MSDU接收的发送地址 */
    uint8_t uc_priority;
    uint8_t auc_resv[1];

    oal_netbuf_stru *pst_netbuf; /* MSDU对应的netbuf指针(可以使clone的netbuf) */
} dmac_msdu_stru;

typedef struct {
    uint8_t uc_reason;
    uint8_t uc_event;
    uint8_t auc_des_addr[WLAN_MAC_ADDR_LEN];
} dmac_diasoc_deauth_event;

typedef enum {
    OAL_QUERY_STATION_INFO_EVENT = 1,
    OAL_ATCMDSRV_DBB_NUM_INFO_EVENT = 2,
    OAL_ATCMDSRV_FEM_PA_INFO_EVENT = 3,
    OAL_ATCMDSRV_GET_RX_PKCG = 4,
    OAL_ATCMDSRV_LTE_GPIO_CHECK = 5,
    OAL_ATCMDSRV_GET_ANT = 6,
    OAL_QUERY_EVNET_BUTT
} oal_query_event_id_enum;

typedef struct {
    uint8_t query_event;
    uint8_t auc_query_sta_addr[WLAN_MAC_ADDR_LEN];
} dmac_query_request_event;
typedef struct {
    uint32_t ul_event_para; /* 消息传输的数据 */
    uint32_t ul_fail_num;
    int16_t s_always_rx_rssi;
    uint8_t uc_event_id; /* 消息号 */
    uint8_t uc_reserved;
} dmac_atcmdsrv_atcmd_response_event;
typedef struct {
    uint8_t query_event;
    uint8_t auc_query_sta_addr[WLAN_MAC_ADDR_LEN]; /* sta mac地址 */
} dmac_query_station_info_request_event;

typedef struct {
    uint8_t flags;
    uint8_t mcs;
    uint16_t legacy;
    uint8_t nss;
    uint8_t bw;
    uint8_t rsv[1];
} tx_rate_info_stru;

typedef union {
    uint16_t rw_ptr; /* write index */
    struct {
        uint16_t bit_rw_ptr    : 15,
                 bit_wrap_flag : 1;
    } st_rw_ptr;
} un_rw_ptr;

typedef enum {
    TID_CMDID_CREATE                 = 0,
    TID_CMDID_DEL,
    TID_CMDID_ENQUE,
    TID_CMDID_DEQUE,

    TID_CMDID_BUTT,
} tid_cmd_enum;
typedef uint8_t tid_cmd_enum_uint8;

typedef enum {
    TID_STATE_INIT                 = 0,
    TID_STATE_UP,
    TID_STATE_PAUSE,

    TID_STATE_BUTT,
}tid_state_enum;
typedef uint8_t tid_state_enum_uint8;

typedef enum {
    _80211_TX_MODE_NORMAL  = 0,
    _80211_TX_MODE_RIFS    = 1,
    _80211_TX_MODE_AGGR    = 2,
    _80211_TX_MODE_BUTT
}txmode_enum;
typedef uint8_t txmode_enum_uint8;

/* Host Device公用部分 */
typedef struct {
    uint8_t size;
    uint8_t offload;
    uint8_t max_amsdu_num;
    uint8_t reserved[1];
    uint32_t base_addr_lsb;
    uint32_t base_addr_msb;
    uint16_t read_index;
    uint16_t write_index;
} msdu_info_ring_stru; /* hal_common.h? */

typedef struct {
    uint32_t msdu_addr_lsb;
    uint32_t msdu_addr_msb;
    uint32_t msdu_len      :12;
    uint32_t data_type     :4;
    uint32_t frag_num      :4;
    uint32_t to_ds         :1;
    uint32_t from_ds       :1;
    uint32_t more_frag     :1;
    uint32_t reserved      :1;
    uint32_t aggr_msdu_num :4;
    uint32_t first_msdu    :1;
    uint32_t csum_type     :3;
} msdu_info_stru;

/* TID事件用于更新指针和调度信息 */
typedef struct {
    tid_cmd_enum_uint8 cmd;        /* 0-creat tid; 1-del tid; 2-tid enqueue; 3-tid dequeue; */
    uint16_t user_id;
    uint8_t reserved[1];
    uint8_t tid_no;                /* TID属性，no大于64时，说明是建立了ba会话 */
    uint8_t max_retries;           /* TID属性，软件重传次数 */
    uint8_t ampdu_num;             /* TID属性，ampdu聚合个数 */
    uint8_t amsdu_num;             /* TID属性，amsdu聚合个数 */
    uint32_t cur_msdus;            /* TID状态，队列中的msdu个数 */
    uint32_t cur_bytes;            /* TID状态，队列中的字节数 */
    msdu_info_ring_stru msdu_ring; /* TID属性，msdu_info_ring参数 */
    uint32_t timestamp_us;         /* MSG属性，此MSG生成的时间搓 */
} tid_update_stru;

typedef struct {
    uint16_t user_id;    /* 用户AID */
    uint16_t read_index; /* RING读指针 */
    uint8_t tid;         /* 用户TID */
    uint8_t ba_info_vld; /* 有效标记 */
} mac_tx_ba_info_stru;

typedef struct {
    mac_tx_ba_info_stru tx_ba_info[WLAN_TIDNO_BUTT];
} mac_d2h_tx_ba_complete_info_stru;

typedef struct {
    uint16_t user_aid;
    uint16_t read_index;
    uint8_t tid;
} mac_d2h_tx_ring_info_stru;

typedef struct {
    uint32_t tx_ring_base_addr;
} mac_d2h_tx_ring_base_addr_stru;

typedef struct {
    tx_rate_info_stru tx_rate;
    uint16_t        rx_rate_legacy;
} txrx_rate_info_stru;

typedef struct {
    uint8_t query_event;         /* 消息号 */
    int8_t c_signal;             /* 信号强度 */
    uint16_t uplink_dealy;       /* 上行时延 */
    uint32_t ul_rx_packets;      /* total packets received */
    uint32_t ul_tx_packets;      /* total packets transmitted */
    uint32_t ul_rx_bytes;        /* total bytes received     */
    uint32_t ul_tx_bytes;        /* total bytes transmitted  */
    uint32_t ul_tx_retries;      /* 发送重传次数 */
    uint32_t ul_rx_dropped_misc; /* 接收失败次数 */
    uint32_t ul_tx_failed;       /* 发送失败次数 */
    int16_t s_free_power;        /* 底噪 */
    uint16_t s_chload;           /* 信道繁忙程度 */
    station_info_extend_stru st_station_info_extend;
    union {
        mac_rate_info_stru  tx_rate;   /* ROM部分使用，不可更改，新增函数使用txrx_rate */
        txrx_rate_info_stru txrx_rate;
    } rate_info;
} dmac_query_station_info_response_event;

typedef struct {
    uint32_t ul_cycles;                   /* 统计间隔时钟周期数 */
    uint32_t ul_sw_tx_succ_num;           /* 软件统计发送成功ppdu个数 */
    uint32_t ul_sw_tx_fail_num;           /* 软件统计发送失败ppdu个数 */
    uint32_t ul_sw_rx_ampdu_succ_num;     /* 软件统计接收成功的ampdu个数 */
    uint32_t ul_sw_rx_mpdu_succ_num;      /* 软件统计接收成功的mpdu个数 */
    uint32_t ul_sw_rx_ppdu_fail_num;      /* 软件统计接收失败的ppdu个数 */
    uint32_t ul_hw_rx_ampdu_fcs_fail_num; /* 硬件统计接收ampdu fcs校验失败个数 */
    uint32_t ul_hw_rx_mpdu_fcs_fail_num;  /* 硬件统计接收mpdu fcs校验失败个数 */
} dmac_thruput_info_sync_stru;

typedef struct {
    uint32_t uc_dscr_status;
    uint8_t mgmt_frame_id;
    uint8_t auc_resv[1];
    uint16_t us_user_idx;
} dmac_crx_mgmt_tx_status_stru;

#ifdef _PRE_WLAN_FEATURE_M2S
typedef struct {
    wlan_m2s_mgr_vap_stru ast_m2s_comp_vap[WLAN_SERVICE_STA_MAX_NUM_PER_DEVICE];
    uint8_t uc_m2s_state; /* 当前m2s状态 */
    union {
        struct {
            oal_bool_enum_uint8 en_m2s_result;
            uint8_t uc_m2s_mode; /* 当前切换业务 */
            uint8_t uc_vap_num;
        } mss_result;

        struct {
            oal_bool_enum_uint8 en_arp_detect_on;
        } arp_detect_result;
    } pri_data;
} dmac_m2s_complete_syn_stru;
#endif

#ifdef _PRE_WLAN_FEATURE_TAS_ANT_SWITCH
typedef struct {
    int32_t l_core_idx;
    int32_t l_rssi;
    int32_t aul_rsv[4];
} dmac_tas_rssi_notify_stru;
#endif

typedef enum {
    DMAC_CONFIG_IPV4 = 0, /* 配置IPv4地址 */
    DMAC_CONFIG_IPV6,     /* 配置IPv6地址 */
    DMAC_CONFIG_BUTT
} dmac_ip_type;
typedef uint8_t dmac_ip_type_enum_uint8;

typedef enum {
    DMAC_IP_ADDR_ADD = 0, /* 增加IP地址 */
    DMAC_IP_ADDR_DEL,     /* 删除IP地址 */
    DMAC_IP_OPER_BUTT
} dmac_ip_oper;
typedef uint8_t dmac_ip_oper_enum_uint8;

typedef struct {
    dmac_ip_type_enum_uint8 en_type;
    dmac_ip_oper_enum_uint8 en_oper;

    uint8_t auc_resv[2];

    uint8_t auc_ip_addr[OAL_IP_ADDR_MAX_SIZE];
    uint8_t auc_mask_addr[OAL_IP_ADDR_MAX_SIZE];
} dmac_ip_addr_config_stru;

#if defined(_PRE_WLAN_FEATURE_DATA_SAMPLE) || defined(_PRE_WLAN_FEATURE_PSD_ANALYSIS)
typedef struct {
    uint32_t ul_type;
    uint32_t ul_reg_num;
    uint32_t ul_count;
    uint32_t ul_event_data;
} dmac_sdt_sample_frame_stru;
#endif

#ifdef _PRE_WLAN_FEATURE_APF
typedef struct {
    void *p_program; /* netbuf链表一个元素 */
    uint16_t us_program_len;
} dmac_apf_report_event_stru;
#endif

#ifdef _PRE_WLAN_FEATURE_NRCOEX
typedef struct {
    uint32_t ul_freq;
    uint32_t ul_40m_20m_gap0;
    uint32_t ul_160m_80m_gap0;
    uint32_t ul_40m_20m_gap1;
    uint32_t ul_160m_80m_gap1;
    uint32_t ul_40m_20m_gap2;
    uint32_t ul_160m_80m_gap2;
    uint32_t ul_smallgap0_act;
    uint32_t ul_gap01_act;
    uint32_t ul_gap12_act;
    int32_t l_rxslot_rssi;
} nrcoex_rule_stru;

typedef struct {
    uint32_t us_freq_low_limit : 16,
               us_freq_high_limit : 16;
    uint32_t us_20m_w2m_gap0 : 16,
               us_40m_w2m_gap0 : 16;
    uint32_t us_80m_w2m_gap0 : 16,
               us_160m_w2m_gap0 : 16;
    uint32_t us_20m_w2m_gap1 : 16,
               us_40m_w2m_gap1 : 16;
    uint32_t us_80m_w2m_gap1 : 16,
               us_160m_w2m_gap1 : 16;
    uint32_t us_20m_w2m_gap2 : 16,
               us_40m_w2m_gap2 : 16;
    uint32_t us_80m_w2m_gap2 : 16,
               us_160m_w2m_gap2 : 16;
    uint32_t uc_20m_smallgap0_act : 8,
               uc_40m_smallgap0_act : 8,
               uc_80m_smallgap0_act : 8,
               uc_160m_smallgap0_act : 8;
    uint32_t uc_20m_gap01_act : 8,
               uc_40m_gap01_act : 8,
               uc_80m_gap01_act : 8,
               uc_160m_gap01_act : 8;
    uint32_t uc_20m_gap12_act : 8,
               uc_40m_gap12_act : 8,
               uc_80m_gap12_act : 8,
               uc_160m_gap12_act : 8;
    uint32_t uc_20m_rxslot_rssi : 8,
               uc_40m_rxslot_rssi : 8,
               uc_80m_rxslot_rssi : 8,
               uc_160m_rxslot_rssi : 8;
} nrcoex_rule_detail_stru;

typedef union {
    nrcoex_rule_stru st_nrcoex_rule_data;
    nrcoex_rule_detail_stru st_nrcoex_rule_detail_data;
} nrcoex_rule_data_union;

typedef struct {
    uint8_t uc_nrcoex_enable;
    uint8_t rsv[3];  /* 保留字段:3字节 */
    nrcoex_rule_data_union un_nrcoex_rule_data[DMAC_WLAN_NRCOEX_INTERFERE_RULE_NUM];
} nrcoex_cfg_info_stru;
#endif
#ifdef _PRE_WLAN_FEATURE_NAN
typedef enum {
    NAN_CFG_TYPE_SET_PARAM = 0,
    NAN_CFG_TYPE_SET_TX_MGMT,

    NAN_CFG_TYPE_BUTT
} mac_nan_cfg_type_enum;
typedef uint8_t mac_nan_cfg_type_enum_uint8;

typedef enum {
    NAN_RSP_TYPE_SET_PARAM = 0,
    NAN_RSP_TYPE_SET_TX_MGMT,
    NAN_RSP_TYPE_CANCEL_TX,
    NAN_RSP_TYPE_ERROR,

    NAN_RSP_TYPE_BUTT
} dmac_nan_rsp_type_enum;
typedef uint8_t dmac_nan_rsp_type_enum_uint8;

typedef struct {
    uint32_t duration;
    uint32_t period;
    uint8_t type;
    uint8_t band;
    uint8_t channel;
} mac_nan_param_stru;

typedef struct {
    uint8_t action;
    uint8_t periodic;
    uint16_t len;
    uint8_t *data;
} mac_nan_mgmt_info_stru;

typedef struct {
    mac_nan_cfg_type_enum_uint8 type;
    uint16_t transaction_id;
    mac_nan_param_stru param;
    mac_nan_mgmt_info_stru mgmt_info;
} mac_nan_cfg_msg_stru;

typedef struct {
    uint16_t transaction;
    uint8_t type;
    uint8_t status;
    uint8_t action; /* nan action类型 */
} mac_nan_rsp_msg_stru;

typedef enum {
    NAN_OK = 0,
    NAN_ERR
} mac_nan_status_enum;
#endif

#ifdef _PRE_WLAN_FEATURE_HIEX
typedef struct {
    uint8_t tx_pwr_detail_2g[2]; // 2.4G 2字节功率信息(su功率，tb功率)
    uint8_t tx_pwr_detail_5g[4][2]; // 5G 4个band, 每个band 2字节功率信息(su功率，tb功率)
} mac_sta_max_tx_power_info_stru;
#endif

typedef struct {
    oal_uint8 cfg_type;
    oal_bool_enum_uint8 need_w4_dev_return;
    oal_uint8 resv[2]; /* 2 预留，4 字节对齐 */
    oal_uint32 dev_ret;
} mac_cfg_cali_hdr_stru;

typedef struct {
    mac_cfg_cali_hdr_stru cali_cfg_hdr;

    oal_int8  input_args[WLAN_CALI_CFG_MAX_ARGS_NUM][WLAN_CALI_CFG_CMD_MAX_LEN];
    oal_uint8 input_args_num;
    oal_uint8 resv1[3]; /* 3 预留，4 字节对齐 */

    oal_int32 al_output_param[WLAN_CFG_MAX_ARGS_NUM];
    oal_uint8 uc_output_param_num;
    oal_uint8 resv2[3]; /* 3 预留，4 字节对齐 */
} mac_cfg_cali_param_stru;

#define TID_AGGR_UPDATE_NUM 64
#define RING_DEPTH(SIZE)  ((128) << (SIZE))
/* 不区分offload架构的CB字段 */
#define MAC_GET_CB_IS_4ADDRESS(_pst_tx_ctrl)        ((_pst_tx_ctrl)->en_use_4_addr)
/* 标记是否是小包AMSDU帧 */
#define MAC_GET_CB_IS_AMSDU(_pst_tx_ctrl)           ((_pst_tx_ctrl)->bit_is_amsdu)
/* 自适应算法决定的聚合度 */
#define MAC_GET_CB_AMSDU_LEVEL(_pst_tx_ctrl)        ((_pst_tx_ctrl)->bit_large_amsdu_level)
/* MSDU帧头部对齐字节 */
#define MAC_GET_CB_ETHER_HEAD_PADDING(_pst_tx_ctrl) ((_pst_tx_ctrl)->bit_align_padding)
/* 实际组成的大包AMSDU帧包含的子帧数 */
#define MAC_GET_CB_HAS_MSDU_NUMBER(_pst_tx_ctrl)    ((_pst_tx_ctrl)->bit_msdu_num_in_amsdu)
/* 标记是否组成了大包AMSDU帧 */
#define MAC_GET_CB_IS_LARGE_SKB_AMSDU(_pst_tx_ctrl) ((_pst_tx_ctrl)->bit_is_large_skb_amsdu)
#define MAC_GET_CB_IS_FIRST_MSDU(_pst_tx_ctrl)      ((_pst_tx_ctrl)->bit_is_first_msdu)
#define MAC_GET_CB_IS_NEED_RESP(_pst_tx_ctrl)       ((_pst_tx_ctrl)->bit_need_rsp)
#define MAC_GET_CB_IS_EAPOL_KEY_PTK(_pst_tx_ctrl)   ((_pst_tx_ctrl)->bit_is_eapol_key_ptk)
#define MAC_GET_CB_IS_ROAM_DATA(_pst_tx_ctrl)       ((_pst_tx_ctrl)->bit_roam_data)
#define MAC_GET_CB_IS_FROM_PS_QUEUE(_pst_tx_ctrl)   ((_pst_tx_ctrl)->bit_is_get_from_ps_queue)
#define MAC_GET_CB_IS_MCAST(_pst_tx_ctrl)           ((_pst_tx_ctrl)->bit_ismcast)
#define MAC_GET_CB_IS_NEEDRETRY(_pst_tx_ctrl)       ((_pst_tx_ctrl)->bit_is_needretry)
#define MAC_GET_CB_IS_PROBE_DATA(_pst_tx_ctrl)      ((_pst_tx_ctrl)->en_is_probe_data)
#define MAC_GET_CB_IS_RTSP(_pst_tx_ctrl)            ((_pst_tx_ctrl)->bit_is_rtsp)
#define MAC_GET_CB_ALG_TAGS(_pst_tx_ctrl)           ((_pst_tx_ctrl)->uc_alg_frame_tag)

#define MAC_GET_CB_MGMT_FRAME_ID(_pst_tx_ctrl) ((_pst_tx_ctrl)->bit_mgmt_frame_id)
#define MAC_GET_CB_MPDU_LEN(_pst_tx_ctrl)      ((_pst_tx_ctrl)->us_mpdu_payload_len)
#define MAC_GET_CB_FRAME_TYPE(_pst_tx_ctrl)    ((_pst_tx_ctrl)->uc_frame_type)
#define MAC_GET_CB_FRAME_SUBTYPE(_pst_tx_ctrl) ((_pst_tx_ctrl)->uc_frame_subtype)
#define MAC_GET_CB_DATA_TYPE(_pst_tx_ctrl)     ((_pst_tx_ctrl)->uc_data_type)
#define MAC_GET_CB_IS_802_3_SNAP(_pst_tx_ctrl)    ((_pst_tx_ctrl)->bit_is_802_3_snap)
#define MAC_GET_CB_TIMESTAMP(_pst_tx_ctrl)     ((_pst_tx_ctrl)->us_timestamp_ms)
#define MAC_GET_CB_IS_QOSNULL(_pst_tx_ctrl)    ((_pst_tx_ctrl)->bit_is_qosnull)
#ifdef _PRE_WLAN_FEATURE_HIEX
#define MAC_GET_CB_HIEX_TRACED(_pst_tx_ctrl) ((_pst_tx_ctrl)->bit_hiex_traced)
#define MAC_GET_RX_CB_HIEX_TRACED(_pst_rx_ctl) ((_pst_rx_ctl)->bit_hiex_traced)
#endif
/* VIP数据帧 */
#define MAC_GET_CB_IS_EAPOL(ptx_ctrl)                             \
    ((MAC_GET_CB_FRAME_TYPE(ptx_ctrl) == WLAN_CB_FRAME_TYPE_DATA) && \
        (MAC_GET_CB_FRAME_SUBTYPE(ptx_ctrl) == MAC_DATA_EAPOL))
#define MAC_GET_CB_IS_ARP(_pst_tx_ctrl)                             \
        ((MAC_GET_CB_FRAME_TYPE(_pst_tx_ctrl) == WLAN_CB_FRAME_TYPE_DATA) && \
            ((MAC_GET_CB_FRAME_SUBTYPE(_pst_tx_ctrl) == MAC_DATA_ARP_REQ) || \
            (MAC_GET_CB_FRAME_SUBTYPE(_pst_tx_ctrl) == MAC_DATA_ARP_RSP)))
#define MAC_GET_CB_IS_VIPFRAME(_pst_tx_ctrl)                             \
    ((WLAN_CB_FRAME_TYPE_DATA == MAC_GET_CB_FRAME_TYPE(_pst_tx_ctrl)) && \
        (MAC_DATA_VIP_FRAME >= MAC_GET_CB_FRAME_SUBTYPE(_pst_tx_ctrl)))

#define MAC_GET_CB_IS_SMPS_FRAME(_pst_tx_ctrl)                             \
    ((WLAN_CB_FRAME_TYPE_ACTION == MAC_GET_CB_FRAME_TYPE(_pst_tx_ctrl)) && \
        (WLAN_ACTION_SMPS_FRAME_SUBTYPE == MAC_GET_CB_FRAME_SUBTYPE(_pst_tx_ctrl)))
#define MAC_GET_CB_IS_OPMODE_FRAME(_pst_tx_ctrl)                           \
    ((WLAN_CB_FRAME_TYPE_ACTION == MAC_GET_CB_FRAME_TYPE(_pst_tx_ctrl)) && \
        (WLAN_ACTION_OPMODE_FRAME_SUBTYPE == MAC_GET_CB_FRAME_SUBTYPE(_pst_tx_ctrl)))
#define MAC_GET_CB_IS_P2PGO_FRAME(_pst_tx_ctrl)                          \
    ((WLAN_CB_FRAME_TYPE_MGMT == MAC_GET_CB_FRAME_TYPE(_pst_tx_ctrl)) && \
        (WLAN_ACTION_P2PGO_FRAME_SUBTYPE == MAC_GET_CB_FRAME_SUBTYPE(_pst_tx_ctrl)))
#ifdef _PRE_WLAN_FEATURE_TWT
#define MAC_GET_CB_IS_TWT_SETUP_FRAME(_pst_tx_ctrl)                        \
    ((WLAN_CB_FRAME_TYPE_ACTION == MAC_GET_CB_FRAME_TYPE(_pst_tx_ctrl)) && \
        (WLAN_ACTION_TWT_SETUP_REQ == MAC_GET_CB_FRAME_SUBTYPE(_pst_tx_ctrl)))
#endif

#define mac_dbdc_send_m2s_action(mac_device, tx_ctl) \
    (mac_is_dbdc_running((mac_device)) && (MAC_GET_CB_IS_SMPS_FRAME((tx_ctl)) || MAC_GET_CB_IS_OPMODE_FRAME((tx_ctl))))

/* 模块发送流程控制信息结构体的信息元素获取 */
#define MAC_GET_CB_MPDU_NUM(_pst_tx_ctrl)            ((_pst_tx_ctrl)->bit_mpdu_num)
#define MAC_GET_CB_NETBUF_NUM(_pst_tx_ctrl)          ((_pst_tx_ctrl)->bit_netbuf_num)
#define MAC_GET_CB_FRAME_HEADER_LENGTH(_pst_tx_ctrl) ((_pst_tx_ctrl)->bit_frame_header_length)
#define MAC_GET_CB_ACK_POLACY(_pst_tx_ctrl)          ((_pst_tx_ctrl)->en_ack_policy)
#define MAC_GET_CB_TX_VAP_INDEX(_pst_tx_ctrl)        ((_pst_tx_ctrl)->bit_tx_vap_index)
#define MAC_GET_CB_TX_USER_IDX(_pst_tx_ctrl)         ((_pst_tx_ctrl)->bit_tx_user_idx)
#define MAC_GET_CB_WME_AC_TYPE(_pst_tx_ctrl)         ((_pst_tx_ctrl)->bit_ac)
#define MAC_GET_CB_WME_TID_TYPE(_pst_tx_ctrl)        ((_pst_tx_ctrl)->bit_tid)
#define MAC_GET_CB_EVENT_TYPE(_pst_tx_ctrl)          ((_pst_tx_ctrl)->bit_event_type)
#define MAC_GET_CB_EVENT_SUBTYPE(_pst_tx_ctrl)       ((_pst_tx_ctrl)->bit_event_sub_type)
#define MAC_GET_CB_RETRIED_NUM(_pst_tx_ctrl)         ((_pst_tx_ctrl)->bit_retried_num)
#define MAC_GET_CB_ALG_PKTNO(_pst_tx_ctrl)           ((_pst_tx_ctrl)->uc_alg_pktno)
#define MAC_GET_CB_TCP_ACK(_pst_tx_ctrl)             ((_pst_tx_ctrl)->bit_is_tcp_ack)
#define MAC_GET_CB_HTC_FLAG(_pst_tx_ctrl)            ((_pst_tx_ctrl)->bit_htc_flag)
#ifdef _PRE_WLAN_FEATURE_HIEX
#define MAC_GET_CB_IS_HIMIT_FRAME(_pst_tx_ctrl)      ((_pst_tx_ctrl)->bit_is_himit)
#define MAC_GET_CB_IS_GAME_MARKED_FRAME(_pst_tx_ctrl)((_pst_tx_ctrl)->bit_is_game_marked)
#endif
#ifdef _PRE_WLAN_FEATURE_HID2D_TX_DROP
#define MAC_GET_CB_IS_HID2D_FRAME(_pst_tx_ctrl)((_pst_tx_ctrl)->bit_is_hid2d_pkt)
#define MAC_GET_CB_HID2D_TX_DELAY_TIME(_pst_tx_ctrl)((_pst_tx_ctrl)->uc_hid2d_tx_delay_time)
#endif
#define MAC_GET_CB_IS_DATA_FRAME(_pst_tx_ctrl)                                \
    ((MAC_GET_CB_WLAN_FRAME_TYPE(_pst_tx_ctrl) == WLAN_DATA_BASICTYPE) &&     \
        ((MAC_GET_CB_WLAN_FRAME_SUBTYPE(_pst_tx_ctrl) == WLAN_DATA) ||  \
            (MAC_GET_CB_WLAN_FRAME_SUBTYPE(_pst_tx_ctrl) == WLAN_QOS_DATA) ||  \
            (MAC_GET_CB_WLAN_FRAME_SUBTYPE(_pst_tx_ctrl) == WLAN_QOS_NULL_FRAME)))

#if defined(_PRE_PRODUCT_ID_HI110X_DEV)
#define MAC_GET_CB_SEQ_NUM(_pst_tx_ctrl)             \
    (((mac_ieee80211_frame_stru *)((uint8_t *)_pst_tx_ctrl + OAL_MAX_CB_LEN))->bit_seq_num)
#define MAC_SET_CB_80211_MAC_HEAD_TYPE(_pst_tx_ctrl, _flag)
#define MAC_GET_CB_80211_MAC_HEAD_TYPE(_pst_tx_ctrl) 1 /* offload架构,MAC HEAD由netbuf index管理,不需要释放 */

#define MAC_GET_CB_WLAN_FRAME_TYPE(_pst_tx_ctrl) \
    (((mac_ieee80211_frame_stru *)((uint8_t *)(_pst_tx_ctrl) + OAL_MAX_CB_LEN))->st_frame_control.bit_type)
#define MAC_GET_CB_WLAN_FRAME_SUBTYPE(_pst_tx_ctrl) \
    (((mac_ieee80211_frame_stru *)((uint8_t *)(_pst_tx_ctrl) + OAL_MAX_CB_LEN))->st_frame_control.bit_sub_type)
#ifdef _PRE_WLAN_FEATURE_TXBF_HW
#define MAC_FRAME_TYPE_IS_VHT_NDPA(_uc_type, _uc_sub_type) \
    ((WLAN_CONTROL == (_uc_type)) && (WLAN_VHT_NDPA == (_uc_sub_type)))
#endif

#define MAC_SET_CB_FRAME_HEADER_ADDR(_pst_tx_ctrl, _addr)
#define MAC_GET_CB_FRAME_HEADER_ADDR(_pst_tx_ctrl) \
    ((mac_ieee80211_frame_stru *)((uint8_t *)_pst_tx_ctrl + OAL_MAX_CB_LEN))

#define MAC_SET_CB_IS_QOS_DATA(_pst_tx_ctrl, _flag)
#define MAC_GET_CB_IS_QOS_DATA(_pst_tx_ctrl) \
    ((WLAN_DATA_BASICTYPE == MAC_GET_CB_WLAN_FRAME_TYPE(_pst_tx_ctrl)) &&  \
        ((WLAN_QOS_DATA == MAC_GET_CB_WLAN_FRAME_SUBTYPE(_pst_tx_ctrl)) ||  \
            (WLAN_QOS_NULL_FRAME == MAC_GET_CB_WLAN_FRAME_SUBTYPE(_pst_tx_ctrl))))

#define MAC_SET_CB_IS_BAR(_pst_tx_ctrl, _flag)
#define MAC_GET_CB_IS_BAR(_pst_tx_ctrl) \
    ((WLAN_CONTROL == MAC_GET_CB_WLAN_FRAME_TYPE(_pst_tx_ctrl)) && \
        (WLAN_BLOCKACK_REQ == MAC_GET_CB_WLAN_FRAME_SUBTYPE(_pst_tx_ctrl)))

/* OFFLOAD 架构不需要 */
#define MAC_SET_CB_BAR_DSCR_ADDR(_pst_tx_ctrl, _addr)
#define MAC_GET_CB_BAR_DSCR_ADDR(_pst_tx_ctrl) OAL_PTR_NULL
#else
#define MAC_GET_CB_WLAN_FRAME_TYPE(_pst_tx_ctrl)            ((_pst_tx_ctrl)->st_expand_cb.en_frame_type)
#define MAC_GET_CB_WLAN_FRAME_SUBTYPE(_pst_tx_ctrl)         \
    (((_pst_tx_ctrl)->pst_frame_header)->st_frame_control.bit_sub_type)
#define MAC_GET_CB_SEQ_NUM(_pst_tx_ctrl)                    ((_pst_tx_ctrl)->st_expand_cb.us_seqnum)
#define MAC_SET_CB_FRAME_HEADER_ADDR(_pst_tx_ctrl, _addr)   ((_pst_tx_ctrl)->st_expand_cb.pst_frame_header = (_addr))
#define MAC_GET_CB_FRAME_HEADER_ADDR(_pst_tx_ctrl)          ((_pst_tx_ctrl)->st_expand_cb.pst_frame_header)
#define MAC_SET_CB_80211_MAC_HEAD_TYPE(_pst_tx_ctrl, _flag) \
    ((_pst_tx_ctrl)->st_expand_cb.bit_80211_mac_head_type = (_flag))
#define MAC_GET_CB_80211_MAC_HEAD_TYPE(_pst_tx_ctrl)        ((_pst_tx_ctrl)->st_expand_cb.bit_80211_mac_head_type)
#define MAC_SET_CB_IS_QOS_DATA(_pst_tx_ctrl, _flag)
#define MAC_GET_CB_IS_QOS_DATA(_pst_tx_ctrl)                OAL_FALSE
#endif  //#if defined(_PRE_PRODUCT_ID_HI110X_DEV)

#define MAC_GET_RX_CB_FRAME_LEN(_pst_rx_ctl)         ((_pst_rx_ctl)->us_frame_len)
#define MAC_GET_RX_CB_MAC_HEADER_LEN(_pst_rx_ctl)    ((_pst_rx_ctl)->uc_mac_header_len)
#define MAC_GET_RX_CB_MAC_VAP_ID(_pst_rx_ctl)        ((_pst_rx_ctl)->uc_mac_vap_id)
#define MAC_GET_RX_CB_HAL_VAP_IDX(_pst_rx_ctl)       ((_pst_rx_ctl)->bit_vap_id)

#define MAC_GET_RX_CB_MAC_VAP_ID(_pst_rx_ctl)        ((_pst_rx_ctl)->uc_mac_vap_id)
#define MAC_GET_RX_CB_HAL_VAP_IDX(_pst_rx_ctl)       ((_pst_rx_ctl)->bit_vap_id)
#define MAC_RXCB_VAP_ID(_pst_rx_ctl)                 ((_pst_rx_ctl)->uc_mac_vap_id)

#define MAC_GET_RX_CB_SEQ_NUM(_pst_rx_ctl)           ((_pst_rx_ctl)->us_seq_num)
#define MAC_GET_RX_CB_TID(_pst_rx_ctl)               ((_pst_rx_ctl)->rx_tid)
#define MAC_GET_RX_CB_PROC_FLAG(_pst_rx_ctl)         ((_pst_rx_ctl)->bit_process_flag)
#define MAC_GET_RX_CB_REL_IS_VALID(_pst_rx_ctl)      ((_pst_rx_ctl)->bit_release_valid)
#define MAC_GET_RX_CB_REL_END_SEQNUM(_pst_rx_ctl)    ((_pst_rx_ctl)->bit_release_end_sn)
#define MAC_GET_RX_CB_SSN(_pst_rx_ctl)               ((_pst_rx_ctl)->bit_start_seqnum)
#define MAC_GET_RX_CB_REL_START_SEQNUM(_pst_rx_ctl)  ((_pst_rx_ctl)->bit_release_start_sn)
#define MAC_GET_RX_CB_DST_IS_VALID(_pst_rx_ctl)      ((_pst_rx_ctl)->bit_dst_is_valid)
#define MAC_GET_RX_DST_USER_ID(_pst_rx_ctl)          ((_pst_rx_ctl)->dst_user_id)

#define MAC_RXCB_IS_AMSDU(_pst_rx_ctl)               ((_pst_rx_ctl)->bit_amsdu_enable)
#define MAC_RX_CB_IS_DEST_CURR_BSS(_pst_rx_ctl)     \
    (((_pst_rx_ctl)->bit_dst_is_valid) && \
    (((_pst_rx_ctl)->bit_band_id) == ((_pst_rx_ctl)->bit_dst_band_id)) && \
    (((_pst_rx_ctl)->bit_vap_id) == ((_pst_rx_ctl)->dst_hal_vap_id)))


/* DMAC模块接收流程控制信息结构体的信息元素获取 */
#define MAC_RXCB_TA_USR_ID(_pst_rx_ctl) ((_pst_rx_ctl)->bit_ta_user_idx)
#define MAC_GET_RX_CB_TA_USER_IDX(_pst_rx_ctl) ((_pst_rx_ctl)->bit_ta_user_idx)
#define MAC_GET_RX_CB_PAYLOAD_LEN(_pst_rx_ctl) ((_pst_rx_ctl)->us_frame_len - (_pst_rx_ctl)->uc_mac_header_len)
#define MAC_GET_RX_CB_NAN_FLAG(_pst_rx_ctl) ((_pst_rx_ctl)->bit_nan_flag)
#if defined(_PRE_PRODUCT_ID_HI110X_DEV)
#if (_PRE_PRODUCT_ID != _PRE_PRODUCT_ID_HI1106_DEV)
#define MAC_GET_RX_PAYLOAD_ADDR(_pst_rx_ctl, _pst_buf) \
    (OAL_NETBUF_PAYLOAD(_pst_buf) + ((_pst_rx_ctl)->uc_mac_header_len) - ((_pst_rx_ctl)->uc_mac_header_len))
#else
#define MAC_GET_RX_PAYLOAD_ADDR(_pst_rx_ctl, _pst_buf) \
    (OAL_NETBUF_PAYLOAD(_pst_buf) + ((_pst_rx_ctl)->uc_mac_header_len))
#endif
#define MAC_GET_RX_CB_MAC_HEADER_ADDR(_prx_cb_ctrl)    (mac_get_rx_cb_mac_hdr(_prx_cb_ctrl))
#else
#define MAC_GET_RX_CB_MAC_HEADER_ADDR(_pst_rx_ctl)     ((_pst_rx_ctl)->st_expand_cb.pul_mac_hdr_start_addr)
#define MAC_GET_RX_CB_DA_USER_IDX(_pst_rx_ctl)         ((_pst_rx_ctl)->st_expand_cb.us_da_user_idx)
#define MAC_GET_RX_PAYLOAD_ADDR(_pst_rx_ctl, _pst_buf) \
    ((uint8_t *)(mac_get_rx_cb_mac_hdr(_pst_rx_ctl)) + MAC_GET_RX_CB_MAC_HEADER_LEN(_pst_rx_ctl))
#endif

#if (defined(_PRE_PRODUCT_ID_HI110X_HOST) || (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1106_DEV))
#define MAC_RXCB_STATUS(_pst_rx_ctl)                 ((_pst_rx_ctl)->rx_status)
#define MAC_RX_CB_IS_MULTICAST(_pst_rx_ctl)          ((_pst_rx_ctl)->bit_mcast_bcast)
#define MAC_GET_RX_CB_IS_FIRST_SUB_MSDU(_pst_rx_ctl) ((_pst_rx_ctl)->bit_first_sub_msdu)
#define MAC_GET_RX_CB_IS_REO_TIMEOUT(_pst_rx_ctl)    ((_pst_rx_ctl)->bit_is_reo_timeout)
#define MAC_RXCB_IS_AMSDU_SUB_MSDU(_pst_rx_ctl) \
    ((MAC_RXCB_IS_AMSDU(_pst_rx_ctl) == OAL_TRUE) && (MAC_GET_RX_CB_IS_FIRST_SUB_MSDU(_pst_rx_ctl) == OAL_FALSE))
#define MAC_RXCB_IS_FIRST_AMSDU(_pst_rx_ctl) \
    ((MAC_RXCB_IS_AMSDU(_pst_rx_ctl) == OAL_TRUE) && (MAC_GET_RX_CB_IS_FIRST_SUB_MSDU(_pst_rx_ctl) == OAL_TRUE))
#endif



OAL_STATIC OAL_INLINE uint32_t *mac_get_rx_cb_mac_hdr(mac_rx_ctl_stru *pst_cb_ctrl)
{
#if defined(_PRE_PRODUCT_ID_HI110X_DEV)
#if (_PRE_PRODUCT_ID != _PRE_PRODUCT_ID_HI1106_DEV)
    return (uint32_t *)((uint8_t *)pst_cb_ctrl + OAL_MAX_CB_LEN);
#else
    return (uint32_t *)((uint8_t *)pst_cb_ctrl + OAL_MAX_CB_LEN + MAX_MAC_HEAD_LEN + MSDU_DSCR_LEN);
#endif
#else
    return MAC_GET_RX_CB_MAC_HEADER_ADDR(pst_cb_ctrl);
#endif
}


OAL_STATIC OAL_INLINE uint8_t *mac_netbuf_get_payload(oal_netbuf_stru *pst_netbuf)
{
#if defined(_PRE_PRODUCT_ID_HI110X_DEV)
    return OAL_NETBUF_PAYLOAD(pst_netbuf);
#else
    return OAL_NETBUF_PAYLOAD(pst_netbuf) + MAC_80211_FRAME_LEN;
#endif
}
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of mac_common.h */
