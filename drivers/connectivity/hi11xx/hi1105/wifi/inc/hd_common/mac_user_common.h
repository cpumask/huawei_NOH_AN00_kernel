

#ifndef __MAC_USER_COMMON_H__
#define __MAC_USER_COMMON_H__

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include "wlan_mib.h"
#include "hiex_msg.h"
#include "mac_frame.h"
/*****************************************************************************
  2 宏定义
*****************************************************************************/
/*****************************************************************************
  3 枚举定义
*****************************************************************************/
/*****************************************************************************
  4 全局变量声明
*****************************************************************************/
/*****************************************************************************
  5 消息头定义
*****************************************************************************/
/*****************************************************************************
  6 消息定义
*****************************************************************************/
/*****************************************************************************
  7 STRUCT定义
*****************************************************************************/
/* user下ht相关信息 */
typedef struct {
    /* ht cap */
    oal_bool_enum_uint8 en_ht_capable;   /* HT capable              */
    uint8_t uc_max_rx_ampdu_factor;    /* Max AMPDU Rx Factor     */
    uint8_t uc_min_mpdu_start_spacing; /* Min AMPDU Start Spacing */
    uint8_t uc_htc_support;            /* HTC 域支持              */

    uint16_t bit_ldpc_coding_cap : 1,  /* LDPC 编码 capability    */
               bit_supported_channel_width : 1, /* STA 支持的带宽   0: 20Mhz, 1: 20/40Mhz  */
               bit_sm_power_save : 2,           /* SM 省电模式             */
               bit_ht_green_field : 1,          /* 绿野模式                */
               bit_short_gi_20mhz : 1,          /* 20M下短保护间隔         */
               bit_short_gi_40mhz : 1,          /* 40M下短保护间隔         */
               bit_tx_stbc : 1,                 /* Indicates support for the transmission of PPDUs using STBC */
               bit_rx_stbc : 2,                 /* 支持 Rx STBC            */
               bit_ht_delayed_block_ack : 1,    /* Indicates support for HT-delayed Block Ack opera-tion. */
               bit_max_amsdu_length : 1,        /* Indicates maximum A-MSDU length. */
               bit_dsss_cck_mode_40mhz : 1,     /* 40M下 DSSS/CCK 模式  0:不使用 40M dsss/cck, 1: 使用 40M dsss/cck */
               bit_resv : 1,
               /*
                * Indicates whether APs receiving this information or reports of this informa-tion are required to
                * prohibit 40 MHz transmissions
                */
               bit_forty_mhz_intolerant : 1,
               bit_lsig_txop_protection : 1; /* 支持L-SIG TXOP保护 */

    uint8_t uc_rx_mcs_bitmask[WLAN_HT_MCS_BITMASK_LEN]; /* Rx MCS bitmask */

    /* ht operation, VAP是STA, user是AP独有 */
    uint8_t uc_primary_channel;

    uint8_t bit_secondary_chan_offset : 2,
              bit_sta_chan_width : 1,
              bit_rifs_mode : 1,
              bit_HT_protection : 2,
              bit_nongf_sta_present : 1,
              bit_obss_nonht_sta_present : 1;

    uint8_t bit_dual_beacon : 1,
              bit_dual_cts_protection : 1,
              bit_secondary_beacon : 1,
              bit_lsig_txop_protection_full_support : 1,
              bit_pco_active : 1,
              bit_pco_phase : 1,
              bit_resv6 : 2;

    uint8_t uc_chan_center_freq_seg2;

    uint8_t auc_basic_mcs_set[16];

    uint32_t bit_imbf_receive_cap : 1,         /* 隐式TxBf接收能力 */
               bit_receive_staggered_sounding_cap : 1,  /* 接收交错探测帧的能力 */
               bit_transmit_staggered_sounding_cap : 1, /* 发送交错探测帧的能力 */
               bit_receive_ndp_cap : 1,                 /* 接收NDP能力 */
               bit_transmit_ndp_cap : 1,                /* 发送NDP能力 */
               bit_imbf_cap : 1,                        /* 隐式TxBf能力 */
               /*
                * 0=不支持，1=站点可以用CSI报告相应校准请求，但不能发起校准，
                * 2=保留，3=站点可以发起，也可以相应校准请求
                */
               bit_calibration : 2,
               bit_exp_csi_txbf_cap : 1,                /* 应用CSI反馈进行TxBf的能力 */
               bit_exp_noncomp_txbf_cap : 1,            /* 应用非压缩矩阵进行TxBf的能力 */
               bit_exp_comp_txbf_cap : 1,               /* 应用压缩矩阵进行TxBf的能力 */
               bit_exp_csi_feedback : 2,                /* 0=不支持，1=延迟反馈，2=立即反馈，3=延迟和立即反馈 */
               bit_exp_noncomp_feedback : 2,            /* 0=不支持，1=延迟反馈，2=立即反馈，3=延迟和立即反馈 */
               bit_exp_comp_feedback : 2,               /* 0=不支持，1=延迟反馈，2=立即反馈，3=延迟和立即反馈 */
               bit_min_grouping : 2,                    /* 0=不分组，1=1,2分组，2=1,4分组，3=1,2,4分组 */
               /*
                * CSI反馈时，bfee最多支持的beamformer天线数，
                * 0=1Tx天线探测，1=2Tx天线探测，2=3Tx天线探测，3=4Tx天线探测
                */
               bit_csi_bfer_ant_number : 2,
               /*
                * 非压缩矩阵反馈时，bfee最多支持的beamformer天线数，
                * 0=1Tx天线探测，1=2Tx天线探测，2=3Tx天线探测，3=4Tx天线探测
                */
               bit_noncomp_bfer_ant_number : 2,
               /*
                * 压缩矩阵反馈时，bfee最多支持的beamformer天线数，
                * 0=1Tx天线探测，1=2Tx天线探测，2=3Tx天线探测，3=4Tx天线探测
                */
               bit_comp_bfer_ant_number : 2,
               bit_csi_bfee_max_rows : 2,               /* bfer支持的来自bfee的CSI显示反馈的最大行数 */
               bit_channel_est_cap : 2,                 /* 信道估计的能力，0=1空时流，依次递增 */
               bit_reserved : 3;
} mac_user_ht_hdl_stru;

typedef struct {
    uint16_t us_max_mcs_1ss : 2, /* 一个空间流的MCS最大支持MAP */
               us_max_mcs_2ss : 2,        /* 一个空间流的MCS最大支持MAP */
               us_max_mcs_3ss : 2,        /* 一个空间流的MCS最大支持MAP */
               us_max_mcs_4ss : 2,        /* 一个空间流的MCS最大支持MAP */
               us_max_mcs_5ss : 2,        /* 一个空间流的MCS最大支持MAP */
               us_max_mcs_6ss : 2,        /* 一个空间流的MCS最大支持MAP */
               us_max_mcs_7ss : 2,        /* 一个空间流的MCS最大支持MAP */
               us_max_mcs_8ss : 2;        /* 一个空间流的MCS最大支持MAP */
} mac_max_mcs_map_stru;

typedef mac_max_mcs_map_stru mac_tx_max_mcs_map_stru;
typedef mac_max_mcs_map_stru mac_rx_max_mcs_map_stru;

typedef struct {
    uint16_t us_max_mpdu_length;
    uint16_t us_basic_mcs_set;

    uint32_t bit_max_mpdu_length : 2,
               bit_supported_channel_width : 2,
               bit_rx_ldpc : 1,
               bit_short_gi_80mhz : 1,
               bit_short_gi_160mhz : 1,
               bit_tx_stbc : 1,
               bit_rx_stbc : 3,
               bit_su_beamformer_cap : 1,    /* SU bfer能力，要过AP认证，必须填1 */
               bit_su_beamformee_cap : 1,    /* SU bfee能力，要过STA认证，必须填1 */
               bit_num_bf_ant_supported : 3, /* SU时，最大接收NDP的Nsts，最小是1 */
               bit_num_sounding_dim : 3,     /* SU时，表示Nsts最大值，最小是1 */
               bit_mu_beamformer_cap : 1,    /* 不支持，set to 0 */
               bit_mu_beamformee_cap : 1,    /* 不支持，set to 0 */
               bit_vht_txop_ps : 1,
               bit_htc_vht_capable : 1,
               bit_max_ampdu_len_exp : 3,
               bit_vht_link_adaptation : 2,
               bit_rx_ant_pattern : 1,
               bit_tx_ant_pattern : 1,
               bit_extend_nss_bw_supp : 2; /* 解析vht Capabilities IE: VHT Capabilities Info field */

    mac_tx_max_mcs_map_stru st_tx_max_mcs_map;
    mac_rx_max_mcs_map_stru st_rx_max_mcs_map;

    uint32_t bit_rx_highest_rate : 13,
               bit_tx_highest_rate : 13,
               bit_user_num_spatial_stream_160M : 4,
               bit_resv3 : 2; /* 解析vht Capabilities IE: VHT Supported MCS Set field */

    oal_bool_enum_uint8 en_vht_capable; /* VHT capable */

    /* vht operation只有是ap的情况下有 */
    wlan_mib_vht_op_width_enum_uint8 en_channel_width; /* 解析VHT Operation IE */
    /* uc_channel_width的取值，0 -- 20/40M, 1 -- 80M, 2 -- 160M */
    uint8_t uc_channel_center_freq_seg0;
    uint8_t uc_channel_center_freq_seg1;
} mac_vht_hdl_stru;

typedef struct {
    uint32_t bit_spectrum_mgmt : 1, /* 频谱管理: 0=不支持, 1=支持 */
               bit_qos : 1,                  /* QOS: 0=非QOS站点, 1=QOS站点 */
               bit_barker_preamble_mode : 1, /* 供STA保存BSS中站点是否都支持short preamble， 0=支持， 1=不支持 */
               /* 自动节能: 0=不支持, 1=支持 */
               /*
                * 目前bit_apsd只有写没有读，wifi联盟已自己定义了WMM节能IE代替cap apsd功能,
                * 此处预留为后续可能出的兼容性问题提供接口
                */
               bit_apsd : 1,
               bit_pmf_active : 1,      /* 管理帧加密使能开关 */
               bit_erp_use_protect : 1, /* 供STA保存AP是否启用了ERP保护 */
               bit_11ntxbf : 1,
               bit_proxy_arp : 1,
               bit_histream_cap : 1,
               bit_1024qam_cap : 1,     /* Support 1024QAM */
               bit_bss_transition : 1,  /* Support bss transition */
               bit_mdie : 1,            /* mobility domain IE presented, for 11r cap */
               bit_11k_enable : 1,
               bit_smps_cap : 1,        /* sta模式下，标识对端是否支持smps */
               bit_dcm_cap : 1,
               bit_20mmcs11_compatible_1103 : 1,
               bit_p2p_support_csa : 1, /* p2p 支持csa */
               bit_p2p_scenes : 1, /* p2p 业务场景 */
               bit_resv : 14;
} mac_user_cap_info_stru;

typedef struct {
    uint8_t uc_qos_info;   /* 关联请求中的WMM IE的QOS info field */
    uint8_t uc_max_sp_len; /* 从qos info字段中提取翻译的最大服务长度 */
    uint8_t auc_resv[2];
    uint8_t uc_ac_trigger_ena[WLAN_WME_AC_BUTT]; /* 4个AC的trigger能力 */
    uint8_t uc_ac_delievy_ena[WLAN_WME_AC_BUTT]; /* 4个AC的delivery能力 */
} mac_user_uapsd_status_stru;

/* 密钥管理结构体 */
typedef struct {
    wlan_ciper_protocol_type_enum_uint8 en_cipher_type;
    uint8_t uc_default_index;  /* 默认索引 */
    uint8_t uc_igtk_key_index; /* igtk索引 */
    uint8_t bit_gtk : 1,       /* 指示RX GTK的槽位，02使用 */
              bit_rsv : 7;
    wlan_priv_key_param_stru ast_key[WLAN_NUM_TK + WLAN_NUM_IGTK]; /* key缓存 */
    uint8_t uc_last_gtk_key_idx;                                 /* igtk索引 */
    uint8_t auc_reserved[1];
#ifdef _PRE_WLAN_FEATURE_SOFT_CRYPTO
    uint32_t ul_iv; /* random iv */
#endif
} mac_key_mgmt_stru;

/* 空间流信息结构体 */
typedef struct {
    uint16_t            us_user_idx;
    wlan_nss_enum_uint8   en_avail_num_spatial_stream;            /* Tx和Rx支持Nss的交集,供算法调用 */
    wlan_nss_enum_uint8   en_user_max_cap_nss;                    /* 用户支持的空间流个数 */
    wlan_nss_enum_uint8   en_user_num_spatial_stream_160M;        /* 用户带宽为160M时支持的空间流个数 */
    wlan_nss_enum_uint8   en_smps_opmode_notify_nss;              /* opmode 或者smps 通知修改的nss值  */
    uint8_t             auc_reserv[2];
}mac_user_nss_stru;

/* m2s user信息结构体 */
typedef struct {
    uint16_t              us_user_idx;
    wlan_nss_enum_uint8     en_user_max_cap_nss;                    /* 用户自身空间流能力 */
    wlan_nss_enum_uint8     en_avail_num_spatial_stream;            /* Tx和Rx支持Nss的交集,供算法调用 */

    wlan_nss_enum_uint8     en_avail_bf_num_spatial_stream;         /* 用户支持的Beamforming空间流个数 */
    wlan_bw_cap_enum_uint8  en_avail_bandwidth;                     /* 用户和VAP带宽能力交集,供算法调用 */
    wlan_bw_cap_enum_uint8  en_cur_bandwidth;                       /* 默认值与en_avail_bandwidth相同,供算法调用修改 */
    wlan_mib_mimo_power_save_enum_uint8 en_cur_smps_mode;
    wlan_nss_enum_uint8     en_smps_opmode_nodify_nss;              /* opmode 或者smps 通知修改的nss值 */
    uint8_t auc_rsv[3];
}mac_user_m2s_stru;

typedef struct mac_key_params_tag {
    uint8_t auc_key[OAL_WPA_KEY_LEN];
    uint8_t auc_seq[OAL_WPA_SEQ_LEN];
    int32_t key_len;
    int32_t seq_len;
    uint32_t cipher;
} mac_key_params_stru;

#if defined(_PRE_WLAN_FEATURE_11AX) || defined(_PRE_WLAN_FEATURE_11AX_ROM)
typedef struct {
    mac_frame_he_cap_ie_stru st_he_cap_ie; /* HE Cap ie */
    /* HE Operation */
    mac_frame_he_oper_ie_stru st_he_oper_ie;
    oal_bool_enum_uint8 en_he_capable;                 /* HE capable */
    wlan_mib_vht_op_width_enum_uint8 en_channel_width; /* 解析VHT Operation IE */
    uint16_t us_max_mpdu_length;

    uint8_t bit_change_announce_bss_color : 6, /* 保存bss color change announcement ie数据 */
              bit_change_announce_bss_color_exist : 1,
              bit_he_oper_bss_color_exist : 1;
    uint8_t bit_he_duration_rts_threshold_exist : 1,
              bit_resv : 7;
} mac_he_hdl_stru;
#endif

/* 用户与AP的关联状态枚举 */
typedef enum {
    MAC_USER_STATE_PAUSE = 0,
    MAC_USER_STATE_AUTH_COMPLETE = 1,
    MAC_USER_STATE_AUTH_KEY_SEQ1 = 2,
    MAC_USER_STATE_ASSOC = 3,

    MAC_USER_STATE_BUTT = 4
} hmac_user_asoc_state_enum;
typedef uint8_t mac_user_asoc_state_enum_uint8;

typedef struct {
    uint16_t us_user_idx;
    uint8_t uc_arg1;
    uint8_t uc_arg2;

    /* 协议模式信息 */
    wlan_protocol_enum_uint8 en_cur_protocol_mode;
    wlan_protocol_enum_uint8 en_protocol_mode;
    uint8_t en_avail_protocol_mode; /* 用户和VAP协议模式交集, 供算法调用 */

    wlan_bw_cap_enum_uint8 en_bandwidth_cap;   /* 用户带宽能力信息 */
    wlan_bw_cap_enum_uint8 en_avail_bandwidth; /* 用户和VAP带宽能力交集,供算法调用 */
    wlan_bw_cap_enum_uint8 en_cur_bandwidth;   /* 默认值与en_avail_bandwidth相同,供算法调用修改 */

    oal_bool_enum_uint8 en_user_pmf;
    mac_user_asoc_state_enum_uint8 en_user_asoc_state; /* 用户关联状态 */
#ifdef _PRE_WLAN_FEATURE_11AX
    uint8_t   bit_in_htc_blacklist   : 1, /* 当前user是否在htc blacklist表里 */
              bit_rev                : 7;
    uint8_t   rsv[3]; /* 3为数组大小 */
#endif
} mac_h2d_usr_info_stru;

typedef struct {
    mac_user_cap_info_stru st_user_cap_info; /* 用户能力信息 */
    uint16_t us_user_idx;
    uint8_t auc_resv[2];
#ifdef _PRE_WLAN_FEATURE_HIEX
    mac_hiex_cap_stru st_hiex_cap;
#endif
} mac_h2d_usr_cap_stru;


typedef struct {
    uint16_t us_user_idx;

#if defined(_PRE_WLAN_FEATURE_11AX) || defined(_PRE_WLAN_FEATURE_11AX_ROM)
    mac_he_hdl_stru st_he_hdl;
#endif
    /* vht速率集信息 */
    mac_vht_hdl_stru st_vht_hdl;

    /* ht速率集信息 */
    mac_user_ht_hdl_stru st_ht_hdl;

    /* legacy速率集信息 */
    uint8_t uc_avail_rs_nrates;
    uint8_t auc_avail_rs_rates[WLAN_MAX_SUPP_RATES];

    wlan_protocol_enum_uint8 en_protocol_mode; /* 用户协议模式 */
} mac_h2d_usr_rate_info_stru;

typedef struct {
    uint16_t us_user_idx;
    wlan_protocol_enum_uint8 en_avail_protocol_mode; /* 用户协议模式 */
    wlan_bw_cap_enum_uint8 en_bandwidth_cap;         /* 用户带宽能力信息 */
    wlan_bw_cap_enum_uint8 en_avail_bandwidth;       /* 用户和VAP带宽能力交集,供算法调用 */
    wlan_bw_cap_enum_uint8 en_cur_bandwidth;         /* 默认值与en_avail_bandwidth相同,供算法调用修改 */
    uint8_t auc_rsv[2];
} mac_h2d_user_bandwidth_stru;

typedef struct {
    mac_channel_stru st_channel;
    uint16_t us_user_idx;
    wlan_bw_cap_enum_uint8 en_bandwidth_cap;   /* 用户带宽能力信息 */
    wlan_bw_cap_enum_uint8 en_avail_bandwidth; /* 用户和VAP带宽能力交集,供算法调用 */
    wlan_bw_cap_enum_uint8 en_cur_bandwidth;   /* 默认值与en_avail_bandwidth相同,供算法调用修改 */
    uint8_t auc_rsv[3];
} mac_d2h_syn_info_stru;

typedef struct {
    uint16_t us_user_idx;
    mac_user_asoc_state_enum_uint8 en_asoc_state;
    uint8_t uc_rsv[1];
} mac_h2d_user_asoc_state_stru;

/* user tx参数，从架构分析文档获得成员 */
typedef struct {
    wlan_security_txop_params_stru st_security;
} mac_user_tx_param_stru;

/* roam trigger 数据结构体 */
typedef struct {
    int8_t c_trigger_2G;
    int8_t c_trigger_5G;
    uint16_t us_roam_interval;
} mac_roam_trigger_stru;

/* roam hmac 同步 dmac数据结构体 */
typedef struct {
    uint16_t us_sta_aid;
    uint16_t us_pad;
    mac_channel_stru st_channel;
    mac_user_cap_info_stru st_cap_info;
    mac_key_mgmt_stru st_key_info;
    mac_user_tx_param_stru st_user_tx_info;
    uint32_t ul_back_to_old;
} mac_h2d_roam_sync_stru;
/*****************************************************************************
  8 UNION定义
*****************************************************************************/
/*****************************************************************************
  9 OTHERS定义
*****************************************************************************/
/*****************************************************************************
  10 函数声明
*****************************************************************************/
#endif /* end of mac_user_common.h */
