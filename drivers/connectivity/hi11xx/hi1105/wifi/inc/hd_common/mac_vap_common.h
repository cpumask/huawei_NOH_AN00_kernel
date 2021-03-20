
#ifndef __MAC_VAP_COMMON_H__
#define __MAC_VAP_COMMON_H__

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
/*****************************************************************************
  2 �궨��
*****************************************************************************/
#define MAC_STR_CMD_MAX_SIZE 16

#define MAC_FCS_DBAC_IGNORE     0 /* ����DBAC���� */
#define MAC_FCS_DBAC_NEED_CLOSE 1 /* DBAC��Ҫ�ر� */
#define MAC_FCS_DBAC_NEED_OPEN  2 /* DBAC��Ҫ���� */

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/
typedef enum {
    MAC_SET_BEACON = 0,
    MAC_ADD_BEACON = 1,

    MAC_BEACON_OPERATION_BUTT
} mac_beacon_operation_type;
typedef uint8_t mac_beacon_operation_type_uint8;

typedef enum {
    AMPDU_SWITCH_BY_DEL_BA = 0,
    AMPDU_SWITCH_BY_BA_LUT,

    AMPDU_SWITCH_MODE_BUTT
} mac_ampdu_switch_mode;
typedef uint8_t mac_ampdu_switch_mode_enum_uint8;

#ifdef _PRE_WLAN_FEATURE_TCP_ACK_BUFFER
typedef enum {
    MAC_TCP_ACK_BUF_ENABLE,
    MAC_TCP_ACK_BUF_TIMEOUT,
    MAC_TCP_ACK_BUF_MAX,

    MAC_TCP_ACK_BUF_TYPE_BUTT
} mac_tcp_ack_buf_cfg_cmd_enum;
typedef uint8_t mac_tcp_ack_buf_cfg_cmd_enum_uint8;
#endif

typedef enum {
    MAC_RSSI_LIMIT_SHOW_INFO,
    MAC_RSSI_LIMIT_ENABLE,
    MAC_RSSI_LIMIT_DELTA,
    MAC_RSSI_LIMIT_THRESHOLD,

    MAC_RSSI_LIMIT_TYPE_BUTT
} mac_rssi_limit_type_enum;
typedef uint8_t mac_rssi_limit_type_enum_uint8;

/* ��������ά���������� */
typedef enum {
    MAC_SET_POW_RF_REG_CTL = 0, /* �����Ƿ�RF�Ĵ����� */
    MAC_SET_POW_FIX_LEVEL,      /* �̶����ʵȼ� */
    MAC_SET_POW_MAG_LEVEL,      /* ����֡���ʵȼ� */
    MAC_SET_POW_CTL_LEVEL,      /* ����֡���ʵȼ� */
    MAC_SET_POW_AMEND,          /* ����UPC code */
    MAC_SET_POW_NO_MARGIN,      /* ���ʲ������� */
    MAC_SET_POW_SHOW_LOG,       /* ��־��ʾ */
    MAC_SET_POW_SAR_LVL_DEBUG,  /* ��sar  */
#ifdef _PRE_WLAN_FEATURE_TAS_ANT_SWITCH
    MAC_TAS_POW_CTRL_DEBUG,  /* TAS������  */
    MAC_TAS_RSSI_CALI_DEBUG, /* TAS���߲���  */
    MAC_TAS_ANT_SWITCH,      /* TAS�����л�  */
#endif
    MAC_SHOW_TPC_TABLE_GAIN,
    MAC_POW_SAVE,
    MAC_POW_GET_PD_INFO,
    MAC_POW_SET_TPC_IDX,

    MAC_SET_POW_BUTT
} mac_set_pow_type_enum;
typedef uint8_t mac_set_pow_type_enum_uint8;

typedef enum {
    MAC_VAP_CONFIG_UCAST_DATA = 0,
    MAC_VAP_CONFIG_MCAST_DATA,
    MAC_VAP_CONFIG_BCAST_DATA,
    MAC_VAP_CONFIG_UCAST_MGMT_2G,
    MAC_VAP_CONFIG_UCAST_MGMT_5G,
    MAC_VAP_CONFIG_MBCAST_MGMT_2G,
    MAC_VAP_CONFIG_MBCAST_MGMT_5G,
    MAC_VAP_CONFIG_BUTT,
} mac_vap_config_dscr_frame_type_enum;
typedef uint8_t mac_vap_config_dscr_frame_type_uint8;

typedef enum { /* hi1102-cb */
    SHORTGI_20_CFG_ENUM,
    SHORTGI_40_CFG_ENUM,
    SHORTGI_80_CFG_ENUM,
    SHORTGI_BUTT_CFG
} short_gi_cfg_type;

/*****************************************************************************
  4 ȫ�ֱ�������
*****************************************************************************/
/*****************************************************************************
  5 ��Ϣͷ����
*****************************************************************************/
/*****************************************************************************
  6 ��Ϣ����
*****************************************************************************/
/*****************************************************************************
  7 STRUCT����
*****************************************************************************/
typedef struct {
    uint8_t uc_announced_channel;                           /* ���ŵ��� */
    wlan_channel_bandwidth_enum_uint8 en_announced_bandwidth; /* �´���ģʽ */
    uint8_t uc_ch_switch_cnt;                               /* �ŵ��л����� */
    wlan_ch_switch_status_enum_uint8 en_ch_switch_status;     /* �ŵ��л�״̬ */
    wlan_bw_switch_status_enum_uint8 en_bw_switch_status;     /* �����л�״̬ */
    oal_bool_enum_uint8 en_csa_present_in_bcn;                /* Beacon֡���Ƿ����CSA IE */
    wlan_csa_mode_tx_enum_uint8 en_csa_mode;
    uint8_t uc_start_chan_idx;
    uint8_t uc_end_chan_idx;
    wlan_channel_bandwidth_enum_uint8 en_user_pref_bandwidth;

    /* VAPΪSTAʱ�����г�Ա
     *
     *  ---|--------|--------------------|-----------------|-----------
     *     0        3                    0                 0
     *     X        A                    B                 C
     *
     *  sta���ŵ��л���ͨ����ͼ�������, ����Ϊ�л���������
     *  X->A A֮ǰΪδ���������ŵ��л�ʱ,�л�������Ϊ0
     *  ��A->Bʱ���Ϊsta�ȴ��л�״̬: en_waiting_to_shift_channelΪtrue
     *  ��B->CΪsta�ŵ��л���,���ȴ�ap��beacon״̬: en_waiting_for_apΪtrue
     *  C-> Ϊsta�յ���ap��beacon����׼�ŵ��л�����
     *
     *  A��ͨ�����յ�csa ie(beacon/action...), B��ͨ��Ϊtbtt�ж����л���������Ϊ
     *  0����csa ie�м�����Ϊ0��C����Ϊ�յ�beacon
     *
     *  ��A->C�Ĺ����У�������ظ��յ���csa ie�����ŵ��л�����
     *
     */
    // oal_bool_enum_uint8                      en_bw_change;             /* STA�Ƿ���Ҫ���д����л� */
    // oal_bool_enum_uint8                      en_waiting_for_ap;
    uint8_t uc_new_channel;                           /* ���Կ��Ǹ�����ϲ� */
    wlan_channel_bandwidth_enum_uint8 en_new_bandwidth; /* ���Կ��Ǹ�����ϲ� */
    oal_bool_enum_uint8 en_waiting_to_shift_channel;    /* �ȴ��л��ŵ� */
    uint8_t auc_rsv2[1];
    oal_bool_enum_uint8 en_te_b;
    uint8_t bit_wait_bw_change;        /* �յ�action֡,�ȴ��л����� */
    uint8_t uc_rsv1[2];       /* 2Ϊ�������, ap��һ�η��͵��л����� */
    uint8_t uc_linkloss_change_chanel; /* ���Կ��Ǹ�����ϲ� */
    wlan_linkloss_scan_switch_chan_enum_uint8 en_linkloss_scan_switch_chan;
    uint32_t ul_chan_report_for_te_a;
    mac_channel_stru st_old_channel; /* �ŵ��л�ʱ�����л�ǰ�ŵ���Ϣ */
    uint8_t _rom[4];
} mac_ch_switch_info_stru;

typedef struct {
    uint8_t flags;
    uint8_t mcs;
    uint16_t legacy;
    uint8_t nss;
    uint8_t bw;
    uint8_t rsv[3];
} mac_rate_info_stru;

#ifdef _PRE_WLAN_FEATURE_TWT
/* TWT���� */
typedef struct {
    uint64_t ull_twt_start_time;
    uint64_t ull_twt_interval;
    uint32_t ul_twt_duration; /* Nominal Minimum TWT Wake Duration */
    uint8_t uc_twt_flow_id;
    uint8_t uc_twt_announce_bit;
    uint8_t uc_twt_session_enable;
    uint8_t uc_twt_is_trigger_enabled;
    uint8_t uc_next_twt_size;
    uint8_t uc_twt_dialog_token;
    uint8_t auc_resv[2];
} mac_cfg_twt_stru;
#endif

/* CHR2.0ʹ�õ�STAͳ����Ϣ */
typedef struct {
    uint8_t uc_distance;      /* �㷨��tpc���룬��Ӧdmac_alg_tpc_user_distance_enum */
    uint8_t uc_cca_intr;      /* �㷨��cca_intr���ţ���Ӧalg_cca_opt_intf_enum */
    int8_t c_snr_ant0;        /* ����0�ϱ���SNRֵ */
    int8_t c_snr_ant1;        /* ����1�ϱ���SNRֵ */
    uint32_t ul_bcn_cnt;      /* �յ���beacon���� */
    uint32_t ul_bcn_tout_cnt; /* beacon��ʧ�ļ��� */
} station_info_extend_stru;

typedef struct {               /* hi1102-cb */
    uint8_t uc_shortgi_type; /* shortgi 20/40/80     */
    uint8_t uc_enable;       /* 1:enable; 0:disable  */
    uint8_t auc_resv[2];
} shortgi_cfg_stru;

/*****************************************************************************
  8 UNION����
*****************************************************************************/
typedef enum mac_rate_info_flags {
    MAC_RATE_INFO_FLAGS_MCS = BIT(0),
    MAC_RATE_INFO_FLAGS_VHT_MCS = BIT(1),
    MAC_RATE_INFO_FLAGS_40_MHZ_WIDTH = BIT(2),
    MAC_RATE_INFO_FLAGS_80_MHZ_WIDTH = BIT(3),
    MAC_RATE_INFO_FLAGS_SHORT_MIDDLE = BIT(4),
    MAC_RATE_INFO_FLAGS_160_MHZ_WIDTH = BIT(5),
    MAC_RATE_INFO_FLAGS_SHORT_GI = BIT(6),
    MAC_RATE_INFO_FLAGS_HE_MCS = BIT(7),
} mac_rate_info_flags;

typedef enum {
    MAC_WMM_SET_PARAM_TYPE_DEFAULT,
    MAC_WMM_SET_PARAM_TYPE_UPDATE_EDCA,

    MAC_WMM_SET_PARAM_TYPE_BUTT
} mac_wmm_set_param_type_enum;
typedef uint8_t mac_wmm_set_param_type_enum_uint8;


/* MAC vap�������Ա�ʶ */
typedef struct {
    uint32_t bit_uapsd : 1,
               bit_txop_ps : 1,
               bit_wpa : 1,
               bit_wpa2 : 1,
               bit_dsss_cck_mode_40mhz : 1, /* �Ƿ�������40M��ʹ��DSSS/CCK, 1-����, 0-������ */
               bit_rifs_tx_on : 1,
               bit_tdls_prohibited : 1,                /* tdlsȫ�ֽ��ÿ��أ� 0-���ر�, 1-�ر� */
               bit_tdls_channel_switch_prohibited : 1, /* tdls�ŵ��л�ȫ�ֽ��ÿ��أ� 0-���ر�, 1-�ر� */
               bit_hide_ssid : 1,                      /* AP��������ssid,  0-�ر�, 1-���� */
               bit_wps : 1,                            /* AP WPS����:0-�ر�, 1-���� */
               bit_11ac2g : 1,                         /* 2.4G�µ�11ac:0-�ر�, 1-���� */
               bit_keepalive : 1,                      /* vap KeepAlive���ܿ���: 0-�ر�, 1-���� */
               bit_smps : 2,                           /* vap ��ǰSMPS����(��ʱ��ʹ��) */
               bit_dpd_enbale : 1,                     /* dpd�Ƿ��� */
               bit_dpd_done : 1,                       /* dpd�Ƿ���� */
               bit_11ntxbf : 1,                        /* 11n txbf���� */
               bit_disable_2ght40 : 1,                 /* 2ght40��ֹλ��1-��ֹ��0-����ֹ */
               bit_peer_obss_scan : 1,                 /* �Զ�֧��obss scan����: 0-��֧��, 1-֧�� */
               bit_1024qam : 1,                        /* �Զ�֧��1024qam����: 0-��֧��, 1-֧�� */
               bit_wmm_cap : 1,                        /* ������STA������AP�Ƿ�֧��wmm������Ϣ */
               bit_is_interworking : 1,                /* ������STA������AP�Ƿ�֧��interworking���� */
               bit_ip_filter : 1,                      /* rx����ip�����˵Ĺ��� */
               bit_opmode : 1,                         /* �Զ˵�probe rsp�е�extended capbilities �Ƿ�֧��OPMODE */
               bit_nb : 1,                             /* Ӳ���Ƿ�֧��խ�� */
               bit_2040_autoswitch : 1,                /* �Ƿ�֧���滷���Զ�2040�����л� */
               bit_2g_custom_siso : 1,                 /* 2g�Ƿ��ƻ�������siso,Ĭ�ϵ���0,��ʼ˫���� */
               bit_5g_custom_siso : 1,                 /* 5g�Ƿ��ƻ�������siso,Ĭ�ϵ���0,��ʼ˫���� */
               bit_bt20dbm : 1,                        /* 20dbm�Ƿ�ʹ�ܣ�����host��sta��ɾ�ۺ��ж� */
               bit_icmp_filter : 1,                    /* rx����icmp�Ĺ���filter */
               bit_dcm : 1,                            /* 11ax dcm���� */
               bit_rx_stbc : 1;                        /* ������bss ֧��stbc */
} mac_cap_flag_stru;

/* cfg id��Ӧ�Ĳ����ṹ�� */
/* ����VAP�����ṹ��, ��Ӧcfgid: WLAN_CFGID_ADD_VAP */
typedef struct {
    wlan_vap_mode_enum_uint8 en_vap_mode;
    uint8_t uc_cfg_vap_indx;
    uint16_t us_muti_user_id; /* ���vap ��Ӧ��muti user index */

    uint8_t uc_vap_id;                  /* ��Ҫ��ӵ�vap id */
    wlan_p2p_mode_enum_uint8 en_p2p_mode; /* 0:��P2P�豸; 1:P2P_GO; 2:P2P_Device; 3:P2P_CL */

    uint8_t bit_11ac2g_enable : 1;
    uint8_t bit_disable_capab_2ght40 : 1;
    uint8_t bit_reserve : 6;
    uint8_t uc_dst_hal_dev_id;  // vap�ҽӵ�hal device id

    uint8_t bit_uapsd_enable : 1;
    uint8_t bit_reserve1 : 7;
    uint8_t probe_resp_enable;
    uint8_t probe_resp_status;
    uint8_t is_primary_vap;
    uint8_t resv2[1];
    oal_net_device_stru *pst_net_dev;
} mac_cfg_add_vap_param_stru;

typedef mac_cfg_add_vap_param_stru mac_cfg_del_vap_param_stru;

/* ����VAP�����ṹ�� ��Ӧcfgid: WLAN_CFGID_START_VAP */
typedef struct {
    oal_bool_enum_uint8 en_mgmt_rate_init_flag; /* start vapʱ�򣬹���֡�����Ƿ���Ҫ��ʼ�� */
    uint8_t uc_protocol;
    uint8_t uc_band;
    uint8_t uc_bandwidth;
    wlan_p2p_mode_enum_uint8 en_p2p_mode;
    uint8_t auc_resv2[3];
    int32_t l_ifindex;
    oal_net_device_stru *pst_net_dev; /* �˳�Ա����Host(WAL&HMAC)ʹ�ã�Device��(DMAC&ALG&HAL��)��ʹ�� */
} mac_cfg_start_vap_param_stru;
typedef mac_cfg_start_vap_param_stru mac_cfg_down_vap_param_stru;

/* CFG VAP h2d */
typedef struct {
    oal_net_device_stru *pst_net_dev;
} mac_cfg_vap_h2d_stru;

typedef struct {
    uint16_t us_sta_aid;
    uint8_t uc_uapsd_cap;
#ifdef _PRE_WLAN_FEATURE_TXOPPS
    oal_bool_enum_uint8 en_txop_ps;
#else
    uint8_t auc_resv[1];
#endif /* #ifdef _PRE_WLAN_FEATURE_TXOPPS */
    uint8_t bit_ap_chip_oui : 3,
              bit_resv : 5;
    uint8_t auc_resv1[3];
} mac_h2d_vap_info_stru;

typedef struct {
    oal_bool_enum_uint8 en_11ax_cap; /* �Ƿ�֧��11ax���� */
    oal_bool_enum_uint8 en_radar_detector_cap;
    oal_bool_enum_uint8 en_11n_sounding;
    wlan_bw_cap_enum_uint8 en_wlan_bw_max;

    uint16_t us_beacon_period;
    oal_bool_enum_uint8 en_green_field;
    oal_bool_enum_uint8 en_mu_beamformee_cap;

    oal_bool_enum_uint8 en_txopps_is_supp;
    oal_bool_enum_uint8 uc_su_bfee_num;
    oal_bool_enum_uint8 en_40m_shortgi;
    oal_bool_enum_uint8 en_11n_txbf;

    oal_bool_enum_uint8 en_40m_enable;
    uint8_t uc_bfer_num_sounding_dim;
    oal_bool_enum_uint8 en_su_bfee;
#ifdef _PRE_WLAN_FEATURE_11AX
    uint8_t uc_he_ntx_sts_below_80m;
    uint8_t uc_he_ntx_sts_over_80m;
    uint8_t uc_he_num_dim_below_80m;
    uint8_t uc_he_num_dim_over_80m;
#endif
    uint8_t uc_rsv[1];
} mac_d2h_mib_update_info_stru;

typedef struct {
    /* word 0 */
    wlan_prot_mode_enum_uint8 en_protection_mode; /* ����ģʽ */
    uint8_t uc_obss_non_erp_aging_cnt;          /* ָʾOBSS��non erp վ����ϻ�ʱ�� */
    uint8_t uc_obss_non_ht_aging_cnt;           /* ָʾOBSS��non ht վ����ϻ�ʱ�� */
    uint8_t bit_auto_protection : 1;            /* ָʾ���������Ƿ�����OAL_SWITCH_ON �򿪣� OAL_SWITCH_OFF �ر� */
    uint8_t bit_obss_non_erp_present : 1;       /* ָʾobss���Ƿ����non ERP��վ�� */
    uint8_t bit_obss_non_ht_present : 1;        /* ָʾobss���Ƿ����non HT��վ�� */
    uint8_t bit_rts_cts_protect_mode : 1;       /* ָrts_cts ���������Ƿ��, OAL_SWITCH_ON �򿪣� OAL_SWITCH_OFF �ر� */
    uint8_t bit_lsig_txop_protect_mode : 1;     /* ָʾL-SIG protect�Ƿ��, OAL_SWITCH_ON �򿪣� OAL_SWITCH_OFF �ر� */
    uint8_t bit_reserved : 3;

    /* word 1 */
    uint8_t uc_sta_no_short_slot_num;     /* ��֧��short slot��STA���� */
    uint8_t uc_sta_no_short_preamble_num; /* ��֧��short preamble��STA���� */
    uint8_t uc_sta_non_erp_num;           /* ��֧��ERP��STA���� */
    uint8_t uc_sta_non_ht_num;            /* ��֧��HT��STA���� */
    /* word 2 */
    uint8_t uc_sta_non_gf_num;        /* ֧��ERP/HT,��֧��GF��STA���� */
    uint8_t uc_sta_20M_only_num;      /* ֻ֧��20M Ƶ�ε�STA���� */
    uint8_t uc_sta_no_40dsss_cck_num; /* ����40M DSSS-CCK STA����  */
    uint8_t uc_sta_no_lsig_txop_num;  /* ��֧��L-SIG TXOP Protection STA���� */
} mac_protection_stru;

/* ����ͬ��������صĲ��� */
typedef struct {
    wlan_mib_ht_protection_enum_uint8 en_dot11HTProtection;
    oal_bool_enum_uint8 en_dot11RIFSMode;
    oal_bool_enum_uint8 en_dot11LSIGTXOPFullProtectionActivated;
    oal_bool_enum_uint8 en_dot11NonGFEntitiesPresent;

    mac_protection_stru st_protection;
} mac_h2d_protection_stru;

/* ����ͬ�������л��Ĳ��� */
typedef struct {
    wlan_channel_bandwidth_enum_uint8 en_40M_bandwidth;
    oal_bool_enum_uint8 en_40M_intol_user;
    uint8_t auc_resv[2];
} mac_bandwidth_stru;

/* Э����� ��Ӧcfgid: WLAN_CFGID_MODE */
typedef struct {
    wlan_protocol_enum_uint8 en_protocol;           /* Э�� */
    wlan_channel_band_enum_uint8 en_band;           /* Ƶ�� */
    wlan_channel_bandwidth_enum_uint8 en_bandwidth; /* ���� */
    uint8_t en_channel_idx;                       /* ��20M�ŵ��� */
} mac_cfg_mode_param_stru;

typedef struct {
    wlan_channel_band_enum_uint8 en_band;           /* Ƶ�� */
    wlan_channel_bandwidth_enum_uint8 en_bandwidth; /* ���� */
} mac_cfg_mib_by_bw_param_stru;

/* ����mac��ַ���� ��Ӧcfgid: WLAN_CFGID_STATION_ID */
typedef struct {
    uint8_t auc_station_id[WLAN_MAC_ADDR_LEN];
    wlan_p2p_mode_enum_uint8 en_p2p_mode;
    uint8_t auc_resv[1];
} mac_cfg_staion_id_param_stru;

/* SSID���� ��Ӧcfgid: WLAN_CFGID_SSID */
typedef struct {
    uint8_t uc_ssid_len;
    uint8_t auc_resv[2];
    int8_t ac_ssid[WLAN_SSID_MAX_LEN];
} mac_cfg_ssid_param_stru;

/* ��ȡ���ַ������� */
typedef struct {
    int32_t l_buff_len;
    uint8_t auc_buff[WLAN_IWPRIV_MAX_BUFF_LEN];
} mac_cfg_param_char_stru;

/* HOSTAPD ���ù���Ƶ�Σ��ŵ��ʹ������ */
typedef struct {
    wlan_channel_band_enum_uint8 en_band;           /* Ƶ�� */
    wlan_channel_bandwidth_enum_uint8 en_bandwidth; /* ���� */
    uint8_t uc_channel;                           /* �ŵ���� */
    uint8_t auc_resv[1];                          /* ����λ */
} mac_cfg_channel_param_stru;

/* HOSTAPD ����wiphy �����豸��Ϣ������RTS ����ֵ����Ƭ��������ֵ */
typedef struct {
    uint8_t uc_frag_threshold_changed;
    uint8_t uc_rts_threshold_changed;
    uint8_t uc_rsv[2];
    uint32_t ul_frag_threshold;
    uint32_t ul_rts_threshold;
} mac_cfg_wiphy_param_stru;

/* HOSTAPD ���� Beacon ��Ϣ */
typedef struct {
    int32_t l_interval;    /* beacon interval */
    int32_t l_dtim_period; /* DTIM period     */
    oal_bool_enum_uint8 en_privacy;
    uint8_t auc_rsn_ie[MAC_MAX_RSN_LEN];
    uint8_t auc_wpa_ie[MAC_MAX_RSN_LEN];
    oal_bool_enum_uint8 uc_hidden_ssid;
    oal_bool_enum_uint8 en_shortgi_20;
    oal_bool_enum_uint8 en_shortgi_40;
    oal_bool_enum_uint8 en_shortgi_80;
    wlan_protocol_enum_uint8 en_protocol;

    uint8_t uc_smps_mode;
    mac_beacon_operation_type_uint8 en_operation_type;
    uint8_t auc_resv1[2];
} mac_beacon_param_stru;

/* ����logģ�鿪�ص������������ */
typedef struct {
    oam_module_id_enum_uint16 en_mod_id; /* ��Ӧ��ģ��id */
    oal_bool_enum_uint8 en_switch;       /* ��Ӧ�Ŀ������� */
    uint8_t auc_resv[1];
} mac_cfg_log_module_param_stru;

/* �û���ص������������ */
typedef struct {
    uint8_t auc_mac_addr[WLAN_MAC_ADDR_LEN]; /* MAC��ַ */
    oal_bool_enum_uint8 en_ht_cap;             /* ht���� */
    uint8_t auc_resv[3];
    uint16_t us_user_idx; /* �û����� */
} mac_cfg_add_user_param_stru;

/* sniffer����������� */
typedef struct {
    uint8_t uc_sniffer_mode;                 /* snifferģʽ */
    uint8_t auc_mac_addr[WLAN_MAC_ADDR_LEN]; /* ��Ҫ���˵�MAC��ַ */
    uint8_t auc_resv[1];
} mac_cfg_sniffer_param_stru;

typedef mac_cfg_add_user_param_stru mac_cfg_del_user_param_stru;

/* ����֡��FCSͳ����Ϣ */
typedef struct {
    uint32_t ul_data_op;    /* ���ݲ���ģʽ:<0>����,<1>��� */
    uint32_t ul_print_info; /* ��ӡ��������:<0>�������� <1>��֡�� <2>self fcs correct, <3>other fcs correct, <4>fcs error  */
} mac_cfg_rx_fcs_info_stru;

/* �޳��û������������ */
typedef struct {
    uint8_t auc_mac_addr[WLAN_MAC_ADDR_LEN]; /* MAC��ַ */
    uint16_t us_reason_code;                 /* ȥ���� reason code */
} mac_cfg_kick_user_param_stru;

/* ��ͣtid����������� */
typedef struct {
    uint8_t auc_mac_addr[WLAN_MAC_ADDR_LEN]; /* MAC��ַ */
    uint8_t uc_tid;
    uint8_t uc_is_paused;
} mac_cfg_pause_tid_param_stru;

/* �����û��Ƿ�Ϊvip */
typedef struct {
    uint8_t auc_mac_addr[WLAN_MAC_ADDR_LEN]; /* MAC��ַ */
    uint8_t uc_vip_flag;
} mac_cfg_user_vip_param_stru;

/* ��ͣtid����������� */
typedef struct {
    uint8_t uc_aggr_tx_on;
    uint8_t uc_snd_type;
    mac_ampdu_switch_mode_enum_uint8 en_aggr_switch_mode;
    uint8_t uc_rsv;
} mac_cfg_ampdu_tx_on_param_stru;

#ifdef _PRE_WLAN_FEATURE_TCP_ACK_BUFFER
typedef struct {
    mac_tcp_ack_buf_cfg_cmd_enum_uint8 en_cmd;
    oal_bool_enum_uint8 en_enable;
    uint8_t uc_timeout_ms;
    uint8_t uc_count_limit;
} mac_cfg_tcp_ack_buf_stru;
#endif

typedef struct {
    mac_rssi_limit_type_enum_uint8 en_rssi_limit_type;  // 4�ֲ�������
    oal_bool_enum_uint8 en_rssi_limit_enable_flag;
    int8_t c_rssi;
    int8_t c_rssi_delta;
} mac_cfg_rssi_limit_stru;

/* ����hostĳ�����е�ÿ�ε��ȱ��ĸ�����low_waterline, high_waterline */
typedef struct {
    uint8_t uc_queue_type;
    uint8_t auc_resv[1];
    uint16_t us_burst_limit;
    uint16_t us_low_waterline;
    uint16_t us_high_waterline;
} mac_cfg_flowctl_param_stru;

/* ʹ��qempty���� */
typedef struct {
    uint8_t uc_is_on;
    uint8_t auc_resv[3];
} mac_cfg_resume_qempty_stru;

/* ����mpdu/ampdu�������  */
typedef struct {
    uint8_t uc_tid;
    uint8_t uc_packet_num;
    uint16_t us_packet_len;
    uint8_t auc_ra_mac[OAL_MAC_ADDR_LEN];
} mac_cfg_mpdu_ampdu_tx_param_stru;

typedef struct {
    uint8_t uc_show_profiling_type; /* 0:Rx 1: Tx */
    uint8_t uc_show_level;          /* 0:�ֶ������ȷ�� 1:ÿ���ڵ㡢�ֶζ������ȷ�� 2ÿ���ڵ㡢�ֶζ�������е� */
} mac_cfg_show_profiling_param_stru;

/* AMPDU��ص������������ */
typedef struct {
    uint8_t auc_mac_addr[WLAN_MAC_ADDR_LEN]; /* �û���MAC ADDR */
    uint8_t uc_tidno;                        /* ��Ӧ��tid�� */
    uint8_t auc_reserve[1];                  /* ȷ�ϲ��� */
} mac_cfg_ampdu_start_param_stru;

typedef mac_cfg_ampdu_start_param_stru mac_cfg_ampdu_end_param_stru;

#ifdef _PRE_WLAN_FEATURE_TWT
typedef struct {
    uint8_t auc_mac_addr[WLAN_MAC_ADDR_LEN]; /* �û���MAC ADDR */
    uint8_t uc_twt_setup_cmd;
    uint8_t uc_twt_flow_type;
    uint8_t uc_twt_flow_id;
    uint32_t ul_twt_arg;               /* [setup_cmd/flow type/flow ID] */
    uint32_t ul_twt_start_time_offset; /* us after TSF */
    uint32_t ul_twt_exponent;          /* interval_exponent */
    uint32_t ul_twt_duration;          /* wake_duration */
    uint32_t ul_intrval_mantissa;      /* interval_mantissa */
} mac_cfg_twt_setup_req_param_stru;

typedef struct {
    uint8_t auc_mac_addr[WLAN_MAC_ADDR_LEN]; /* �û���MAC ADDR */
    uint8_t uc_twt_flow_id;                  /* flow ID */
} mac_cfg_twt_teardown_req_param_stru;
#endif

/* BA�Ự��ص������������ */
typedef struct {
    uint8_t auc_mac_addr[WLAN_MAC_ADDR_LEN]; /* �û���MAC ADDR */
    uint8_t uc_tidno;                        /* ��Ӧ��tid�� */
    mac_ba_policy_enum_uint8 en_ba_policy;     /* BAȷ�ϲ��� */
    uint16_t us_buff_size;                   /* BA���ڵĴ�С */
    uint16_t us_timeout;                     /* BA�Ự�ĳ�ʱʱ�� */
} mac_cfg_addba_req_param_stru;

typedef struct {
    uint8_t auc_mac_addr[WLAN_MAC_ADDR_LEN];   /* �û���MAC ADDR */
    uint8_t uc_tidno;                          /* ��Ӧ��tid�� */
    mac_delba_initiator_enum_uint8 en_direction; /* ɾ��ba�Ự�ķ���� */
    mac_delba_trigger_enum_uint8 en_trigger;     /* ɾ��ԭ�� */
} mac_cfg_delba_req_param_stru;
#ifdef _PRE_WLAN_FEATURE_CSI
typedef struct {
    uint8_t auc_mac_addr[WLAN_MAC_ADDR_LEN]; /* csi��Ӧ��MAC ADDR */
    oal_bool_enum_uint8 en_cfg_csi_on;         /* csiʹ�� */
    uint8_t uc_csi_bw;                       /* csi�ɼ����� */
    uint8_t uc_csi_frame_type;               /* csi�ɼ���֡���� */
    uint8_t uc_csi_sample_period;            /* csi�������� */
    uint8_t uc_csi_phy_report_mode;          /* ����CSI PHY���ϱ�ģʽ  */
    uint8_t auc_reserve[1];
} mac_cfg_csi_param_stru;
#endif
#ifdef _PRE_WLAN_FEATURE_WMMAC
/* TSPEC��ص������������ */
typedef struct {
    mac_ts_info_stru ts_info;
    uint8_t uc_rsvd;
    uint16_t us_norminal_msdu_size;
    uint16_t us_max_msdu_size;
    uint32_t ul_min_data_rate;
    uint32_t ul_mean_data_rate;
    uint32_t ul_peak_data_rate;
    uint32_t ul_min_phy_rate;
    uint16_t us_surplus_bw;
    uint16_t us_medium_time;
} mac_cfg_wmm_tspec_stru_param_stru;

typedef struct {
    oal_switch_enum_uint8 en_wmm_ac_switch;
    oal_switch_enum_uint8 en_auth_flag; /* WMM AC��֤���� */
    uint16_t us_timeout_period;       /* ��ʱ����ʱʱ�� */
    uint8_t uc_factor;                /* �������ӣ�medium_time/2^factor */
    uint8_t auc_rsv[3];
} mac_cfg_wmm_ac_param_stru;

#endif

typedef struct {
    uint8_t auc_mac_addr[6];
    uint8_t uc_amsdu_max_num; /* amsdu������ */
    uint8_t auc_reserve[3];
    uint16_t us_amsdu_max_size; /* amsdu��󳤶� */
} mac_cfg_amsdu_start_param_stru;

/* ���͹������ò��� */
typedef struct {
    mac_set_pow_type_enum_uint8 en_type;
    uint8_t uc_reserve;
    uint8_t auc_value[18];
} mac_cfg_set_tx_pow_param_stru;

/* �����û����ò��� */
typedef struct {
    uint8_t uc_function_index;
    uint8_t auc_reserve[2];
    mac_vap_config_dscr_frame_type_uint8 en_type; /* ���õ�֡���� */
    int32_t l_value;
} mac_cfg_set_dscr_param_stru;

/* non-HTЭ��ģʽ���������ýṹ�� */
typedef struct {
    wlan_legacy_rate_value_enum_uint8 en_rate;     /* ����ֵ */
    wlan_phy_protocol_enum_uint8 en_protocol_mode; /* ��Ӧ��Э�� */
    uint8_t auc_reserve[2];                      /* ���� */
} mac_cfg_non_ht_rate_stru;

/* ���÷����������ڲ�Ԫ�ؽṹ�� */
typedef enum {
    RF_PAYLOAD_ALL_ZERO = 0,
    RF_PAYLOAD_ALL_ONE,
    RF_PAYLOAD_RAND,
    RF_PAYLOAD_BUTT
} mac_rf_payload_enum;
typedef uint8_t mac_rf_payload_enum_uint8;

typedef struct {
    uint8_t uc_param; /* ��ѯ��������Ϣ */
    wlan_phy_protocol_enum_uint8 en_protocol_mode;
    mac_rf_payload_enum_uint8 en_payload_flag;
    wlan_tx_ack_policy_enum_uint8 en_ack_policy;
    uint32_t ul_payload_len;
    uint32_t write_index;
    uint32_t base_addr_lsb;
    uint32_t base_addr_msb;
} mac_cfg_tx_comp_stru;

typedef struct {
    uint32_t ul_length;
    uint32_t aul_dieid[WLAN_DIEID_MAX_LEN];
} mac_cfg_show_dieid_stru;

/* ����Ӳ������ */
typedef struct {
    uint32_t ul_devid;
    uint32_t ul_mode;
    uint32_t ul_rate;
} mac_cfg_al_tx_hw_cfg_stru;

/* Ӳ���������� */
typedef struct {
    uint8_t bit_switch : 1,
              bit_dev_id : 1,
              bit_flag : 2, /* 0/1/2/3 ����֡�����ݡ�0:ȫ0��1:ȫ1��2:����� 3:�ظ�uc_content�����ݡ� */
              bit_rsv : 4;
    uint8_t uc_content;
    uint8_t auc_rsv[2];
    uint32_t ul_len;
    uint32_t ul_times; /* ���� 0Ϊ���޴� */
    uint32_t ul_ifs;   /* ����֡���,��λ0.1us */
} mac_cfg_al_tx_hw_stru;

typedef struct {
    uint8_t uc_offset_addr_a;
    uint8_t uc_offset_addr_b;
    uint16_t us_delta_gain;
} mac_cfg_dbb_scaling_stru;

/* Ƶƫ���������ʽ */
typedef struct {
    uint16_t us_idx;         /* ȫ����������ֵ */
    uint16_t us_chn;         /* �����ŵ� */
    int16_t as_corr_data[8]; /* У������ */
} mac_cfg_freq_skew_stru;

/* wfa edca�������� */
typedef struct {
    oal_bool_enum_uint8 en_switch;     /* ���� */
    wlan_wme_ac_type_enum_uint8 en_ac; /* AC */
    uint16_t us_val;                 /* ���� */
} mac_edca_cfg_stru;

/* PPM���������ʽ */
typedef struct {
    int8_t c_ppm_val;      /* PPM��ֵ */
    uint8_t uc_clock_freq; /* ʱ��Ƶ�� */
    uint8_t uc_resv[1];
} mac_cfg_adjust_ppm_stru;

typedef struct {
    uint8_t uc_pcie_pm_level; /* pcie�͹��ļ���,0->normal;1->L0S;2->L1;3->L1PM;4->L1s */
    uint8_t uc_resv[3];
} mac_cfg_pcie_pm_level_stru;

/* �û���Ϣ��ص������������ */
typedef struct {
    uint16_t us_user_idx; /* �û����� */
    uint8_t auc_reserve[2];
} mac_cfg_user_info_param_stru;

/* ����������͹������� */
typedef struct {
    uint8_t uc_pwr;
    uint8_t en_exceed_reg;
    uint8_t auc_resv[2];
} mac_cfg_regdomain_max_pwr_stru;

/* ��ȡ��ǰ������������ַ���������ṹ�� */
typedef struct {
    int8_t ac_country[3];
    uint8_t auc_resv[1];
} mac_cfg_get_country_stru;

/* query��Ϣ��ʽ:2�ֽ�WID x N */
typedef struct {
    wlan_tidno_enum_uint8 en_tid;
    uint8_t uc_resv[3];
} mac_cfg_get_tid_stru;

typedef struct {
    uint16_t us_user_id;
    int8_t c_rssi;
    int8_t c_free_power;
} mac_cfg_query_rssi_stru;

typedef struct {
    uint8_t uc_auth_req_st;
    uint8_t uc_asoc_req_st;
    uint8_t auc_resv[2];
} mac_cfg_query_mngpkt_sendstat_stru;

typedef struct {
    uint16_t us_user_id;
    uint8_t uc_ps_st;
    uint8_t auc_resv[1];
} mac_cfg_query_psst_stru;

typedef struct {
    uint16_t us_user_id;
    uint8_t auc_resv[2];
    uint32_t aul_tx_dropped[WLAN_WME_AC_BUTT];
} mac_cfg_query_drop_num_stru;

typedef struct {
    uint16_t us_user_id;
    uint8_t auc_resv[2];
    uint32_t ul_max_tx_delay; /* �������ʱ */
    uint32_t ul_min_tx_delay; /* ��С������ʱ */
    uint32_t ul_ave_tx_delay; /* ƽ��������ʱ */
} mac_cfg_query_tx_delay_stru;

typedef struct {
    uint8_t uc_vap_id;
    uint8_t uc_bsd;
    uint8_t auc_resv[2];
} mac_cfg_query_bsd_stru;

#ifdef _PRE_WLAN_DFT_STAT
typedef struct {
    uint8_t uc_device_distance;
    uint8_t uc_intf_state_cca;
    uint8_t uc_intf_state_co;
    uint8_t auc_resv[1];
} mac_cfg_query_ani_stru;
#endif

typedef struct {
    uint16_t us_user_id;
#ifdef _PRE_WLAN_DFT_STAT
    uint8_t uc_cur_per;
    uint8_t uc_bestrate_per;
#else
    uint8_t auc_resv[2];
#endif
    uint32_t ul_tx_rate;     /* ��ǰ�������� */
    uint32_t ul_tx_rate_min; /* һ��ʱ������С�������� */
    uint32_t ul_tx_rate_max; /* һ��ʱ������������� */
    uint32_t ul_rx_rate;     /* ��ǰ�������� */
    uint32_t ul_rx_rate_min; /* һ��ʱ������С�������� */
    uint32_t ul_rx_rate_max; /* һ��ʱ�������������� */
} mac_cfg_query_rate_stru;

#ifdef _PRE_WLAN_FEATURE_SMPS
typedef struct {
    uint32_t ul_cfg_id;
    uint32_t ul_ac;
    uint32_t ul_value;
} mac_cfg_set_smps_mode_stru;
#endif

#ifdef _PRE_WLAN_FEATURE_SMPS
typedef struct {
    wlan_mib_mimo_power_save_enum_uint8 en_smps_mode;
    uint8_t uc_trigger_smps_frame;
    uint16_t us_user_idx;
} mac_cfg_smps_mode_stru;
#endif

/* STA PS ���Ͳ��� */
typedef struct {
    uint8_t uc_vap_ps_mode;
} mac_cfg_ps_mode_param_stru;

typedef struct {
    uint16_t us_beacon_timeout;
    uint16_t us_tbtt_offset;
    uint16_t us_ext_tbtt_offset;
    uint16_t us_dtim3_on;
} mac_cfg_ps_param_stru;

typedef struct {
    uint8_t uc_psm_info_enable : 2;  // ����psm��ͳ��ά�����
    uint8_t uc_psm_debug_mode : 2;   // ����psm��debug��ӡģʽ
    uint8_t uc_psm_resd : 4;
} mac_cfg_ps_info_stru;

typedef struct {
    uint8_t uc_show_ip_addr : 4;          // show ip addr
    uint8_t uc_show_arpoffload_info : 4;  // show arpoffload ά��
} mac_cfg_arpoffload_info_stru;

typedef struct {
    uint8_t uc_in_suspend;         // ������
    uint8_t uc_arpoffload_switch;  // arpoffload����
} mac_cfg_suspend_stru;

typedef enum {
    MAC_STA_PM_SWITCH_OFF = 0,      /* �رյ͹��� */
    MAC_STA_PM_SWITCH_ON = 1,       /* �򿪵͹��� */
    MAC_STA_PM_MANUAL_MODE_ON = 2,  /* �����ֶ�sta pm mode */
    MAC_STA_PM_MANUAL_MODE_OFF = 3, /* �ر��ֶ�sta pm mode */
    MAC_STA_PM_SWITCH_BUTT,         /* ������� */
} mac_pm_switch_enum;
typedef uint8_t mac_pm_switch_enum_uint8;

typedef enum {
    MAC_STA_PM_CTRL_TYPE_HOST = 0, /* �͹��Ŀ������� HOST  */
    MAC_STA_PM_CTRL_TYPE_DBAC = 1, /* �͹��Ŀ������� DBAC  */
    MAC_STA_PM_CTRL_TYPE_ROAM = 2, /* �͹��Ŀ������� ROAM  */
    MAC_STA_PM_CTRL_TYPE_CMD = 3,  /* �͹��Ŀ������� CMD   */
    MAC_STA_PM_CTRL_TYPE_ERSRU = 4,  /* �͹��Ŀ������� ERSRU     */
    MAC_STA_PM_CTRL_TYPE_CSI = 5,  /* �͹��Ŀ������� CSI */
    MAC_STA_PM_CTRL_TYPE_BUTT,     /* ������ͣ�ӦС�� 8  */
} mac_pm_ctrl_type_enum;
typedef uint8_t mac_pm_ctrl_type_enum_uint8;

typedef struct {
    mac_pm_ctrl_type_enum_uint8 uc_pm_ctrl_type; /* mac_pm_ctrl_type_enum */
    mac_pm_switch_enum_uint8 uc_pm_enable;       /* mac_pm_switch_enum */
} mac_cfg_ps_open_stru;

/* P2P NOA�������ò��� */
typedef struct {
    uint32_t ul_start_time;
    uint32_t ul_duration;
    uint32_t ul_interval;
    uint8_t uc_count;
    uint8_t auc_rsv[3];
} mac_cfg_p2p_noa_param_stru;

/* P2P ���ܿ������� */
typedef struct {
    uint8_t uc_p2p_statistics_ctrl; /* 0:���P2P ͳ��ֵ�� 1:��ӡ���ͳ��ֵ */
    uint8_t auc_rsv[3];
} mac_cfg_p2p_stat_param_stru;

typedef struct {
    oal_bool_enum_uint8 en_open;    /* �򿪹رմ����� */
    mac_narrow_bw_enum_uint8 en_bw; /* 1M,5M,10M */
    uint8_t auc_rsv[2];
} mac_cfg_narrow_bw_stru;

#ifdef _PRE_WLAN_FEATURE_TXOPPS
/* STA txopps aidͬ�� */
typedef struct {
    uint16_t us_partial_aid;
    uint8_t en_protocol;
    uint8_t uc_enable;
} mac_cfg_txop_sta_stru;
#endif

typedef enum mac_vowifi_mkeep_alive_type {
    VOWIFI_MKEEP_ALIVE_TYPE_STOP = 0,
    VOWIFI_MKEEP_ALIVE_TYPE_START = 1,
    VOWIFI_MKEEP_ALIVE_TYPE_BUTT
} mac_vowifi_nat_keep_alive_type_enum;
typedef uint8_t mac_vowifi_nat_keep_alive_type_enum_uint8;

typedef struct {
    uint8_t uc_keep_alive_id;
    mac_vowifi_nat_keep_alive_type_enum_uint8 en_type;
    uint8_t auc_rsv[2];
} mac_vowifi_nat_keep_alive_basic_info_stru;

typedef struct {
    mac_vowifi_nat_keep_alive_basic_info_stru st_basic_info;
    uint8_t auc_src_mac[WLAN_MAC_ADDR_LEN];
    uint8_t auc_dst_mac[WLAN_MAC_ADDR_LEN];
    uint32_t ul_period_msec;
    uint16_t us_ip_pkt_len;
    uint8_t auc_rsv[2];
    uint8_t auc_ip_pkt_data[4];
} mac_vowifi_nat_keep_alive_start_info_stru;

/* vowifi��������������������� */
typedef enum {
    VOWIFI_SET_MODE = 0,
    VOWIFI_GET_MODE,
    VOWIFI_SET_PERIOD,
    VOWIFI_GET_PERIOD,
    VOWIFI_SET_LOW_THRESHOLD,
    VOWIFI_GET_LOW_THRESHOLD,
    VOWIFI_SET_HIGH_THRESHOLD,
    VOWIFI_GET_HIGH_THRESHOLD,
    VOWIFI_SET_TRIGGER_COUNT,
    VOWIFI_GET_TRIGGER_COUNT,
    VOWIFI_SET_IS_SUPPORT,

    VOWIFI_CMD_BUTT
} mac_vowifi_cmd_enum;
typedef uint8_t mac_vowifi_cmd_enum_uint8;

/* vowifi��������������������ṹ�� */
typedef struct {
    mac_vowifi_cmd_enum_uint8 en_vowifi_cfg_cmd; /* �������� */
    uint8_t uc_value;                          /* ����ֵ */
    uint8_t auc_resv[2];
} mac_cfg_vowifi_stru;

/*
 * enum nl80211_mfp - Management frame protection state
 * @NL80211_MFP_NO: Management frame protection not used
 * @NL80211_MFP_REQUIRED: Management frame protection required
 */
typedef enum {
    MAC_NL80211_MFP_NO,
    MAC_NL80211_MFP_REQUIRED,

    MAC_NL80211_MFP_BUTT
} mac_nl80211_mfp_enum;
typedef uint8_t mac_nl80211_mfp_enum_uint8;

/* �����ں����õ�connect�������·���������connect���� */
typedef struct {
    uint32_t ul_wpa_versions;
    uint32_t ul_group_suite;
    uint32_t aul_pair_suite[WLAN_PAIRWISE_CIPHER_SUITES];
    uint32_t aul_akm_suite[WLAN_AUTHENTICATION_SUITES];
    uint32_t ul_group_mgmt_suite;
} mac_crypto_settings_stru;

typedef struct {
    oal_bool_enum_uint8 en_privacy;                /* �Ƿ���ܱ�־ */
    oal_nl80211_auth_type_enum_uint8 en_auth_type; /* ��֤���ͣ�OPEN or SHARE-KEY */
    uint8_t uc_wep_key_len;                      /* WEP KEY���� */
    uint8_t uc_wep_key_index;                    /* WEP KEY���� */
    uint8_t auc_wep_key[WLAN_WEP104_KEY_LEN];    /* WEP KEY��Կ */
    mac_nl80211_mfp_enum_uint8 en_mgmt_proteced;   /* ��������pmf�Ƿ�ʹ�� */
    wlan_pmf_cap_status_uint8 en_pmf_cap;          /* �豸pmf���� */
    oal_bool_enum_uint8 en_wps_enable;
    mac_crypto_settings_stru st_crypto; /* ��Կ�׼���Ϣ */
#ifdef _PRE_WLAN_FEATURE_11R
    uint8_t auc_mde[8]; /* MD IE��Ϣ */
#endif                    // _PRE_WLAN_FEATURE_11R
    int8_t c_rssi;      /* ����AP��RSSI��Ϣ */
    int8_t c_ant0_rssi; /* ����0��rssi */
    int8_t c_ant1_rssi; /* ����1��rssi */
    int8_t   ant2_rssi;   /* ����2��rssi */
    int8_t   ant3_rssi;   /* ����3��rssi */
    uint8_t  auc_rsv[3];
} mac_conn_security_stru;

typedef enum {
    MAC_LPM_SOC_BUS_GATING = 0,
    MAC_LPM_SOC_PCIE_RD_BYPASS = 1,
    MAC_LPM_SOC_MEM_PRECHARGE = 2,
    MAC_LPM_SOC_PCIE_L0_S = 3,
    MAC_LPM_SOC_PCIE_L1_0 = 4,
    MAC_LPM_SOC_AUTOCG_ALL = 5,
    MAC_LPM_SOC_ADC_FREQ = 6,
    MAC_LPM_SOC_PCIE_L1S = 7,

    MAC_LPM_SOC_SET_BUTT
} mac_lpm_soc_set_enum;
typedef uint8_t mac_lpm_soc_set_enum_uint8;

typedef struct mac_cfg_lpm_soc_set_tag {
    mac_lpm_soc_set_enum_uint8 en_mode;
    uint8_t uc_on_off;
    uint8_t uc_pcie_idle;
    uint8_t auc_rsv[1];
} mac_cfg_lpm_soc_set_stru;

/* оƬ��֤������֡/����֡���� */
typedef enum {
    MAC_TEST_MGMT_BCST = 0,               /* ��beacon�㲥����֡ */
    MAC_TEST_MGMT_MCST = 1,               /* ��beacon�鲥����֡ */
    MAC_TEST_ATIM_UCST = 2,               /* ����ATIM֡ */
    MAC_TEST_UCST = 3,                    /* ��������֡ */
    MAC_TEST_CTL_BCST = 4,                /* �㲥����֡ */
    MAC_TEST_CTL_MCST = 5,                /* �鲥����֡ */
    MAC_TEST_CTL_UCST = 6,                /* ��������֡ */
    MAC_TEST_ACK_UCST = 7,                /* ACK����֡ */
    MAC_TEST_CTS_UCST = 8,                /* CTS����֡ */
    MAC_TEST_RTS_UCST = 9,                /* RTS����֡ */
    MAC_TEST_BA_UCST = 10,                /* BA����֡ */
    MAC_TEST_CF_END_UCST = 11,            /* CF-End����֡ */
    MAC_TEST_TA_RA_EUQAL = 12,            /* RA,TA��ͬ֡ */
    MAC_TEST_FTM = 13,                    /* FTM����֡ */
    MAC_TEST_MGMT_ACTION = 14,            /* ��������Action֡ */
    MAC_TEST_MGMT_BEACON_INCLUDE_IE = 15, /* Beacon ֡��β����������IE��ϢԪ�� */
    MAC_TEST_MAX_TYPE_NUM
} mac_test_frame_type;
typedef uint8_t mac_test_frame_type_enum_uint8;

typedef struct {
    uint8_t auc_mac_ra[WLAN_MAC_ADDR_LEN];
    uint8_t uc_pkt_num;
    mac_test_frame_type_enum_uint8 en_frame_type;
    uint8_t uc_frame_body[64];
    uint8_t uc_vap_idx;
    uint8_t uc_frame_cnt;
    uint8_t uc_frame_body_length;
    uint8_t uc_resv[1];
} mac_cfg_send_frame_param_stru;

typedef struct {
    int32_t l_is_psm; /* �Ƿ������� */
    int32_t l_is_qos; /* �Ƿ�qosnull */
    int32_t l_tidno;  /* tid�� */
} mac_cfg_tx_nulldata_stru;

/* ������̫��������Ҫ�Ĳ��� */
typedef struct {
    oam_ota_frame_direction_type_enum_uint8 en_frame_direction;
    oal_switch_enum_uint8 en_switch;
    uint8_t auc_user_macaddr[WLAN_MAC_ADDR_LEN];
} mac_cfg_eth_switch_param_stru;

/* ����80211����֡������Ҫ�Ĳ��� */
typedef struct {
    uint8_t bit_msdu_dscr : 1,
              bit_ba_info : 1,
              bit_himit_dscr : 1,
              bit_mu_dscr : 1,
              bit_rsv : 4;
} oam_sub_switch_stru;

typedef union {
    uint8_t value;
    oam_sub_switch_stru stru;
} oam_sub_switch_union;

typedef struct {
    oam_ota_frame_direction_type_enum_uint8 en_frame_direction;
    oam_user_track_frame_type_enum_uint8 en_frame_type;
    oal_switch_enum_uint8 en_frame_switch;
    oal_switch_enum_uint8 en_cb_switch;
    oal_switch_enum_uint8 en_dscr_switch;
    oam_sub_switch_union sub_switch;
    uint8_t auc_user_macaddr[WLAN_MAC_ADDR_LEN];
} mac_cfg_80211_ucast_switch_stru;

#ifdef _PRE_WLAN_FEATURE_TXOPPS
/* �������mac txoppsʹ�ܼĴ�����Ҫ���������� */
typedef struct {
    oal_switch_enum_uint8 en_machw_txopps_en;         /* sta�Ƿ�ʹ��txopps */
    oal_switch_enum_uint8 en_machw_txopps_condition1; /* txopps����1 */
    oal_switch_enum_uint8 en_machw_txopps_condition2; /* txopps����2 */
    uint8_t auc_resv[1];
} mac_txopps_machw_param_stru;
#endif
/* ����80211�鲥\�㲥֡������Ҫ�Ĳ��� */
typedef struct {
    oam_ota_frame_direction_type_enum_uint8 en_frame_direction;
    oam_user_track_frame_type_enum_uint8 en_frame_type;
    oal_switch_enum_uint8 en_frame_switch;
    oal_switch_enum_uint8 en_cb_switch;
    oal_switch_enum_uint8 en_dscr_switch;
    oam_sub_switch_union  sub_switch;
    uint8_t auc_resv[2];
} mac_cfg_80211_mcast_switch_stru;

/* ����probe request��probe response������Ҫ�Ĳ��� */
typedef struct {
    oam_ota_frame_direction_type_enum_uint8 en_frame_direction;
    oal_switch_enum_uint8 en_frame_switch;
    oal_switch_enum_uint8 en_cb_switch;
    oal_switch_enum_uint8 en_dscr_switch;
    oam_sub_switch_union  sub_switch;
    uint8_t             resv[3];
} mac_cfg_probe_switch_stru;

/* ��ȡmpdu��Ŀ��Ҫ�Ĳ��� */
typedef struct {
    uint8_t auc_user_macaddr[WLAN_MAC_ADDR_LEN];
    uint8_t auc_resv[2];
} mac_cfg_get_mpdu_num_stru;

typedef struct {
    uint8_t uc_aggr_num_switch; /* ���ƾۺϸ������� */
    uint8_t uc_aggr_num;        /* �ۺϸ��� */
    uint8_t auc_resv[2];
} mac_cfg_aggr_num_stru;

/* ͳ�������� */
typedef enum {
    MAC_STAT_TYPE_TID_DELAY,
    MAC_STAT_TYPE_TID_PER,
    MAC_STAT_TYPE_TID_THRPT,
    MAC_STAT_TYPE_USER_THRPT,
    MAC_STAT_TYPE_VAP_THRPT,
    MAC_STAT_TYPE_USER_BSD,

    MAC_STAT_TYPE_BUTT
} mac_stat_type_enum;
typedef uint8_t mac_stat_type_enum_uint8;

typedef struct {
    mac_stat_type_enum_uint8 en_stat_type;     /* ͳ������ */
    uint8_t uc_vap_id;                       /* vap id */
    uint8_t auc_mac_addr[WLAN_MAC_ADDR_LEN]; /* MAC��ַ */
    uint8_t uc_tidno;                        /* tidno */
    uint16_t us_stat_period;                 /* ͳ������ */
    uint16_t us_stat_num;                    /* ͳ�ƴ��� */
} mac_cfg_stat_param_stru;

typedef struct {
    uint8_t uc_bypass_type;
    uint8_t uc_value;
    uint8_t auc_resv[2];
} mac_cfg_set_thruput_bypass_stru;

typedef struct {
    uint8_t uc_cmd_type;
    uint8_t uc_len;
    uint16_t us_cfg_id;
    uint32_t ul_value;
} mac_cfg_set_tlv_stru;

typedef struct {
    uint16_t us_cfg_id;
    uint8_t uc_cmd_cnt;
    uint8_t uc_len;
    uint16_t us_set_id[MAC_STR_CMD_MAX_SIZE];
    uint32_t ul_value[MAC_STR_CMD_MAX_SIZE];
} mac_cfg_set_str_stru;

typedef struct {
    uint16_t us_user_idx;
    uint16_t us_rx_pn;
} mac_cfg_set_rx_pn_stru;

typedef struct {
    uint8_t uc_key_index;
    oal_bool_enum_uint8 en_pairwise;
    uint8_t auc_mac_addr[OAL_MAC_ADDR_LEN];
    mac_key_params_stru st_key;
} mac_addkey_param_stru;

typedef struct {
    uint8_t uc_key_index;
    oal_bool_enum_uint8 en_pairwise;
    uint8_t auc_mac_addr[OAL_MAC_ADDR_LEN];
} mac_removekey_param_stru;

typedef enum {
    MAC_MEMINFO_USER = 0,
    MAC_MEMINFO_VAP,
    MAC_MEMINFO_POOL_INFO,
    MAC_MEMINFO_POOL_DBG,

    MAC_MEMINFO_DSCR,
    MAC_MEMINFO_NETBUFF,
    MAC_MEMINFO_SDIO_TRX,
    MAC_MEMINFO_SAMPLE_ALLOC,
    MAC_MEMINFO_SAMPLE_FREE,
    MAC_MEMINFO_ALL,

    MAC_MEMINFO_BUTT
} mac_meminfo_cmd_enum;
typedef uint8_t mac_meminfo_cmd_enum_uint8;

typedef struct {
    mac_meminfo_cmd_enum_uint8 uc_meminfo_type;
    uint8_t uc_object_index;
} mac_cfg_meminfo_stru;

typedef enum {
    MAC_DYN_CALI_CFG_SET_EN_REALTIME_CALI_ADJUST,
    MAC_DYN_CALI_CFG_SET_2G_DSCR_INT,
    MAC_DYN_CALI_CFG_SET_5G_DSCR_INT,
    MAC_DYN_CALI_CFG_SET_CHAIN_INT,
    MAC_DYN_CALI_CFG_SET_PDET_MIN_TH,
    MAC_DYN_CALI_CFG_SET_PDET_MAX_TH,
    MAC_DYN_CALI_CFG_BUFF,
} mac_dyn_cali_cfg_enum;
typedef uint8_t mac_dyn_cali_cfg_enum_uint8;

/* ��̬У׼����ö�٣�����ֵ */
typedef struct {
    mac_dyn_cali_cfg_enum_uint8 en_dyn_cali_cfg; /* ��������ö�� */
    uint8_t uc_resv;                           /* �ֽڶ��� */
    uint16_t us_value;                         /* ���ò���ֵ */
} mac_ioctl_dyn_cali_param_stru;

typedef enum {
    /* ҵ������㷨���ò���,����ӵ���Ӧ��START��END֮�� */
    MAC_ALG_CFG_SCHEDULE_START,

    MAC_ALG_CFG_SCHEDULE_VI_CTRL_ENA,
    MAC_ALG_CFG_SCHEDULE_BEBK_MIN_BW_ENA,
    MAC_ALG_CFG_SCHEDULE_MVAP_SCH_ENA,
    MAC_ALG_CFG_FLOWCTRL_ENABLE_FLAG,
    MAC_ALG_CFG_SCHEDULE_VI_SCH_LIMIT,
    MAC_ALG_CFG_SCHEDULE_VO_SCH_LIMIT,
    MAC_ALG_CFG_SCHEDULE_VI_DROP_LIMIT,
    MAC_ALG_CFG_SCHEDULE_VI_MSDU_LIFE_MS,
    MAC_ALG_CFG_SCHEDULE_VO_MSDU_LIFE_MS,
    MAC_ALG_CFG_SCHEDULE_BE_MSDU_LIFE_MS,
    MAC_ALG_CFG_SCHEDULE_BK_MSDU_LIFE_MS,
    MAC_ALG_CFG_SCHEDULE_VI_LOW_DELAY_MS,
    MAC_ALG_CFG_SCHEDULE_VI_HIGH_DELAY_MS,
    MAC_ALG_CFG_SCHEDULE_VI_CTRL_MS,
    MAC_ALG_CFG_SCHEDULE_SCH_CYCLE_MS,
    MAC_ALG_CFG_SCHEDULE_TRAFFIC_CTRL_CYCLE,
    MAC_ALG_CFG_SCHEDULE_CIR_NVIP_KBPS,
    MAC_ALG_CFG_SCHEDULE_CIR_NVIP_KBPS_BE,
    MAC_ALG_CFG_SCHEDULE_CIR_NVIP_KBPS_BK,
    MAC_ALG_CFG_SCHEDULE_CIR_VIP_KBPS,
    MAC_ALG_CFG_SCHEDULE_CIR_VIP_KBPS_BE,
    MAC_ALG_CFG_SCHEDULE_CIR_VIP_KBPS_BK,
    MAC_ALG_CFG_SCHEDULE_CIR_VAP_KBPS,
    MAC_ALG_CFG_SCHEDULE_SM_TRAIN_DELAY,
    MAC_ALG_CFG_VIDEO_DROP_PKT_LIMIT,
    MAC_ALG_CFG_SCHEDULE_LOG_START,
    MAC_ALG_CFG_SCHEDULE_VAP_SCH_PRIO,

    MAC_ALG_CFG_SCHEDULE_LOG_END,
    MAC_ALG_CFG_SCHEDULE_SCH_METHOD,
    MAC_ALG_CFG_SCHEDULE_FIX_SCH_MODE,

    MAC_ALG_CFG_SCHEDULE_END,

    /* AUTORATE�㷨���ò���������ӵ���Ӧ��START��END֮�� */
    MAC_ALG_CFG_AUTORATE_START,

    MAC_ALG_CFG_AUTORATE_ENABLE,
    MAC_ALG_CFG_AUTORATE_USE_LOWEST_RATE,
    MAC_ALG_CFG_AUTORATE_SHORT_STAT_NUM,
    MAC_ALG_CFG_AUTORATE_SHORT_STAT_SHIFT,
    MAC_ALG_CFG_AUTORATE_LONG_STAT_NUM,
    MAC_ALG_CFG_AUTORATE_LONG_STAT_SHIFT,
    MAC_ALG_CFG_AUTORATE_MIN_PROBE_UP_INTVL_PKTNUM,
    MAC_ALG_CFG_AUTORATE_MIN_PROBE_DOWN_INTVL_PKTNUM,
    MAC_ALG_CFG_AUTORATE_MAX_PROBE_INTVL_PKTNUM,
    MAC_ALG_CFG_AUTORATE_PROBE_INTVL_KEEP_TIMES,
    MAC_ALG_CFG_AUTORATE_DELTA_GOODPUT_RATIO,
    MAC_ALG_CFG_AUTORATE_VI_PROBE_PER_LIMIT,
    MAC_ALG_CFG_AUTORATE_VO_PROBE_PER_LIMIT,
    MAC_ALG_CFG_AUTORATE_AMPDU_DURATION,
    MAC_ALG_CFG_AUTORATE_MCS0_CONT_LOSS_NUM,
    MAC_ALG_CFG_AUTORATE_UP_PROTOCOL_DIFF_RSSI,
    MAC_ALG_CFG_AUTORATE_RTS_MODE,
    MAC_ALG_CFG_AUTORATE_LEGACY_1ST_LOSS_RATIO_TH,
    MAC_ALG_CFG_AUTORATE_HT_VHT_1ST_LOSS_RATIO_TH,
    MAC_ALG_CFG_AUTORATE_LOG_ENABLE,
    MAC_ALG_CFG_AUTORATE_VO_RATE_LIMIT,
    MAC_ALG_CFG_AUTORATE_JUDGE_FADING_PER_TH,
    MAC_ALG_CFG_AUTORATE_AGGR_OPT,
    MAC_ALG_CFG_AUTORATE_MAX_AGGR_NUM,
    MAC_ALG_CFG_AUTORATE_LIMIT_1MPDU_PER_TH,
    MAC_ALG_CFG_AUTORATE_BTCOEX_PROBE_ENABLE,
    MAC_ALG_CFG_AUTORATE_BTCOEX_AGGR_ENABLE,
    MAC_ALG_CFG_AUTORATE_COEX_STAT_INTVL,
    MAC_ALG_CFG_AUTORATE_COEX_LOW_ABORT_TH,
    MAC_ALG_CFG_AUTORATE_COEX_HIGH_ABORT_TH,
    MAC_ALG_CFG_AUTORATE_COEX_AGRR_NUM_ONE_TH,
    MAC_ALG_CFG_AUTORATE_DYNAMIC_BW_ENABLE,
    MAC_ALG_CFG_AUTORATE_THRPT_WAVE_OPT,
    MAC_ALG_CFG_AUTORATE_GOODPUT_DIFF_TH,
    MAC_ALG_CFG_AUTORATE_PER_WORSE_TH,
    MAC_ALG_CFG_AUTORATE_RX_CTS_NO_BA_NUM,
    MAL_ALG_CFG_AUTORATE_VOICE_AGGR,
    MAC_ALG_CFG_AUTORATE_FAST_SMOOTH_SHIFT,
    MAC_ALG_CFG_AUTORATE_FAST_SMOOTH_AGGR_NUM,
    MAC_ALG_CFG_AUTORATE_SGI_PUNISH_PER,
    MAC_ALG_CFG_AUTORATE_SGI_PUNISH_NUM,
    MAL_ALG_CFG_AUTORATE_LAST_RATE_RANK_INDEX,
    MAL_ALG_CFG_AUTORATE_MWO_DET_DEBUG,
    MAL_ALG_CFG_AUTORATE_MWO_DET_PER_LOG,
    MAC_ALG_CFG_AUTORATE_RXCH_AGC_OPT,
    MAC_ALG_CFG_AUTORATE_RXCH_AGC_LOG,
    MAC_ALG_CFG_AUTORATE_WEAK_RSSI_TH,
    MAC_ALG_CFG_AUTORATE_RXCH_STAT_PERIOD,
    MAC_ALG_CFG_AUTORATE_RTS_ONE_TCP_DBG,
    MAC_ALG_CFG_AUTORATE_SCAN_USER_OPT,
    MAC_ALG_CFG_AUTORATE_MAX_TX_COUNT,
    MAC_ALG_CFG_AUTORATE_80M_40M_SWITCH,
    MAC_ALG_CFG_AUTORATE_40M_SWITCH_THR,
    MAC_ALG_CFG_AUTORATE_COLLISION_DET_EN,
    MAC_ALG_CFG_AUTORATE_SWITCH_11B,
    MAC_ALG_CFG_AUTORATE_ROM_NSS_AUTH,
    MAC_ALG_CFG_AUTORATE_END,

    MAC_ALG_CFG_AUTORATE_AGGR_START,
    MAC_ALG_CFG_AUTORATE_AGGR_PROBE_INTVL_NUM,
    MAC_ALG_CFG_AUTORATE_AGGR_STAT_SHIFT,
    MAC_ALG_CFG_AUTORATE_DBAC_AGGR_TIME,
    MAC_ALG_CFG_AUTORATE_DBG_VI_STATUS,
    MAC_ALG_CFG_AUTORATE_DBG_AGGR_LOG,
    MAC_ALG_CFG_AUTORATE_AGGR_NON_PROBE_PCK_NUM,
    MAC_ALG_CFG_AUTORATE_AGGR_MIN_AGGR_TIME_IDX,
    MAC_ALG_CFG_AUTORATE_AGGR_250US_DELTA_PER_TH,
    MAC_ALG_CFG_AUTORATE_AGGR_END,

    /* AUTORATE�㷨��־���ò���������ӵ���Ӧ��START��END֮�� */
    MAC_ALG_CFG_AUTORATE_LOG_START,

    MAC_ALG_CFG_AUTORATE_STAT_LOG_START,
    MAC_ALG_CFG_AUTORATE_SELECTION_LOG_START,
    MAC_ALG_CFG_AUTORATE_FIX_RATE_LOG_START,
    MAC_ALG_CFG_AUTORATE_STAT_LOG_WRITE,
    MAC_ALG_CFG_AUTORATE_SELECTION_LOG_WRITE,
    MAC_ALG_CFG_AUTORATE_FIX_RATE_LOG_WRITE,
    MAC_ALG_CFG_AUTORATE_AGGR_STAT_LOG_START,
    MAC_ALG_CFG_AUTORATE_AGGR_STAT_LOG_WRITE,

    MAC_ALG_CFG_AUTORATE_LOG_END,

    /* AUTORATE�㷨ϵͳ�����������ӵ���Ӧ��START��END֮�� */
    MAC_ALG_CFG_AUTORATE_TEST_START,

    MAC_ALG_CFG_AUTORATE_DISPLAY_RATE_SET,
    MAC_ALG_CFG_AUTORATE_CONFIG_FIX_RATE,
    MAC_ALG_CFG_AUTORATE_CYCLE_RATE,
    MAC_ALG_CFG_AUTORATE_DISPLAY_RX_RATE,

    MAC_ALG_CFG_AUTORATE_TEST_END,

    /* SMARTANT�㷨���ò����� ����ӵ���Ӧ��START��END֮�� */
    MAC_ALG_CFG_SMARTANT_START,

    MAC_ALG_CFG_SMARTANT_ENABLE,
    MAC_ALG_CFG_SMARTANT_CERTAIN_ANT,
    MAC_ALG_CFG_SMARTANT_TRAINING_PACKET_NUMBER,
    MAC_ALG_CFG_SMARTANT_CHANGE_ANT,
    MAC_ALG_CFG_SMARTANT_START_TRAIN,
    MAC_ALG_CFG_SMARTANT_SET_TRAINING_PACKET_NUMBER,
    MAC_ALG_CFG_SMARTANT_SET_LEAST_PACKET_NUMBER,
    MAC_ALG_CFG_SMARTANT_SET_ANT_CHANGE_INTERVAL,
    MAC_ALG_CFG_SMARTANT_SET_USER_CHANGE_INTERVAL,
    MAC_ALG_CFG_SMARTANT_SET_PERIOD_MAX_FACTOR,
    MAC_ALG_CFG_SMARTANT_SET_ANT_CHANGE_FREQ,
    MAC_ALG_CFG_SMARTANT_SET_ANT_CHANGE_THRESHOLD,

    MAC_ALG_CFG_SMARTANT_END,
    /* TXBF�㷨���ò���������ӵ���Ӧ��START��END֮�� */
    MAC_ALG_CFG_TXBF_START,
    MAC_ALG_CFG_TXBF_MASTER_SWITCH,
    MAC_ALG_CFG_TXBF_TXMODE_ENABLE,
    MAC_ALG_CFG_TXBF_11N_BFEE_ENABLE,
    MAC_ALG_CFG_TXBF_2G_BFER_ENABLE,
    MAC_ALG_CFG_TXBF_2NSS_BFER_ENABLE,
    MAC_ALG_CFG_TXBF_FIX_MODE,
    MAC_ALG_CFG_TXBF_FIX_SOUNDING,
    MAC_ALG_CFG_TXBF_PROBE_INT,
    MAC_ALG_CFG_TXBF_REMOVE_WORST,
    MAC_ALG_CFG_TXBF_STABLE_NUM,
    MAC_ALG_CFG_TXBF_PROBE_COUNT,
    MAC_ALG_CFG_TXBF_END,

    /* �������㷨���ò���������ӵ���Ӧ��START��END֮�� */
    MAC_ALG_CFG_ANTI_INTF_START,

    MAC_ALG_CFG_ANTI_INTF_IMM_ENABLE,
    MAC_ALG_CFG_ANTI_INTF_UNLOCK_ENABLE,
    MAC_ALG_CFG_ANTI_INTF_RSSI_STAT_CYCLE,
    MAC_ALG_CFG_ANTI_INTF_UNLOCK_CYCLE,
    MAC_ALG_CFG_ANTI_INTF_UNLOCK_DUR_TIME,
    MAC_ALG_CFG_ANTI_INTF_NAV_IMM_ENABLE,
    MAC_ALG_CFG_ANTI_INTF_GOODPUT_FALL_TH,
    MAC_ALG_CFG_ANTI_INTF_KEEP_CYC_MAX_NUM,
    MAC_ALG_CFG_ANTI_INTF_KEEP_CYC_MIN_NUM,
    MAC_ALG_CFG_ANTI_INTF_TX_TIME_FALL_TH,
    MAC_ALG_CFG_ANTI_INTF_PER_PROBE_EN,
    MAC_ALG_CFG_ANTI_INTF_PER_FALL_TH,
    MAC_ALG_CFG_ANTI_INTF_GOODPUT_JITTER_TH,
    MAC_ALG_CFG_ANTI_INTF_DEBUG_MODE,

    MAC_ALG_CFG_ANTI_INTF_END,

    /* ���ż���㷨���ò���������ӵ���Ӧ��START��END֮�� */
    MAC_ALG_CFG_INTF_DET_START,

    MAC_ALG_CFG_INTF_DET_CYCLE,
    MAC_ALG_CFG_INTF_DET_MODE,
    MAC_ALG_CFG_INTF_DET_DEBUG,
    MAC_ALG_CFG_INTF_DET_COCH_THR_STA,
    MAC_ALG_CFG_INTF_DET_ACI_HIGH_TH,
    MAC_ALG_CFG_INTF_DET_ACI_LOW_TH,
    MAC_ALG_CFG_INTF_DET_ACI_SYNC_TH,
    MAC_ALG_CFG_INTF_DET_COCH_NOINTF_STA,
    MAC_ALG_CFG_INTF_DET_COCH_THR_UDP,
    MAC_ALG_CFG_INTF_DET_COCH_THR_TCP,
    MAC_ALG_CFG_INTF_DET_ADJCH_SCAN_CYC,
    MAC_ALG_CFG_INTF_DET_ADJRATIO_THR,
    MAC_ALG_CFG_INTF_DET_SYNC_THR,
    MAC_ALG_CFG_INTF_DET_AVE_RSSI,
    MAC_ALG_CFG_INTF_DET_NO_ADJRATIO_TH,
    MAC_ALG_CFG_INTF_DET_NO_ADJCYC_TH,
    MAC_ALG_CFG_INTF_DET_COLLISION_TH,

    MAC_ALG_CFG_NEG_DET_NONPROBE_TH,

    MAC_ALG_CFG_INTF_DET_END,

    /* EDCA�Ż��㷨���ò���������ӵ���Ӧ��START��END֮�� */
    MAC_ALG_CFG_EDCA_OPT_START,

    MAC_ALG_CFG_EDCA_OPT_AP_EN_MODE,
    MAC_ALG_CFG_EDCA_OPT_STA_EN,
    MAC_ALG_CFG_TXOP_LIMIT_STA_EN,
    MAC_ALG_CFG_EDCA_OPT_STA_WEIGHT,
    MAC_ALG_CFG_EDCA_OPT_DEBUG_MODE,
    MAC_ALG_CFG_EDCA_ONE_BE_TCP_OPT,
    MAC_ALG_CFG_EDCA_ONE_BE_TCP_DBG,
    MAC_ALG_CFG_EDCA_ONE_BE_TCP_TH_NO_INTF,
    MAC_ALG_CFG_EDCA_ONE_BE_TCP_TH_INTF,

    MAC_ALG_CFG_EDCA_OPT_END,

    /* CCA�Ż��㷨���ò���������ӵ���Ӧ��START��END֮�� */
    MAC_ALG_CFG_CCA_OPT_START,

    MAC_ALG_CFG_CCA_OPT_ALG_EN_MODE,
    MAC_ALG_CFG_CCA_OPT_DEBUG_MODE,
    MAC_ALG_CFG_CCA_OPT_SET_CCA_TH_DEBUG,
    MAC_ALG_CFG_CCA_OPT_LOG,

    MAC_ALG_CFG_CCA_OPT_END,

    /* �㷨��־���ò���������ӵ���Ӧ��START��END֮�� */
    MAC_ALG_CFG_INTF_DET_LOG_START,

    MAC_ALG_CFG_INTF_DET_STAT_LOG_START,
    MAC_ALG_CFG_INTF_DET_STAT_LOG_WRITE,

    MAC_ALG_CFG_INTF_DET_LOG_END,

    /* TPC�㷨���ò���, ����ӵ���Ӧ��START��END֮�� */
    MAC_ALG_CFG_TPC_START,

    MAC_ALG_CFG_TPC_MODE,
    MAC_ALG_CFG_TPC_DEBUG,
    MAC_ALG_CFG_TPC_LOG,
    MAC_ALG_CFG_TPC_OVER_TMP_TH,
    MAC_ALG_CFG_TPC_DPD_ENABLE_RATE,
    MAC_ALG_CFG_TPC_TARGET_RATE_11B,
    MAC_ALG_CFG_TPC_TARGET_RATE_11AG,
    MAC_ALG_CFG_TPC_TARGET_RATE_HT20,
    MAC_ALG_CFG_TPC_TARGET_RATE_HT40,
    MAC_ALG_CFG_TPC_TARGET_RATE_VHT20,
    MAC_ALG_CFG_TPC_TARGET_RATE_VHT40,
    MAC_ALG_CFG_TPC_TARGET_RATE_VHT80,
    MAC_ALG_CFG_TPC_TARGET_RATE_VHT160,

    MAC_ALG_CFG_TPC_END,

    /* TPC�㷨��־���ò���������ӵ���Ӧ��START��END֮�� */
    MAC_ALG_CFG_TPC_LOG_START,

    MAC_ALG_CFG_TPC_STAT_LOG_START,
    MAC_ALG_CFG_TPC_STAT_LOG_WRITE,
    MAC_ALG_CFG_TPC_PER_PKT_LOG_START,
    MAC_ALG_CFG_TPC_PER_PKT_LOG_WRITE,
    MAC_ALG_CFG_TPC_GET_FRAME_POW,
    MAC_ALG_CFG_TPC_RESET_STAT,
    MAC_ALG_CFG_TPC_RESET_PKT,

    MAC_ALG_CFG_TPC_LOG_END,

#ifdef _PRE_WLAN_FEATURE_MU_TRAFFIC_CTL
    /* ���û����������㷨���ò��� */
    MAC_ALG_CFG_TRAFFIC_CTL_START,

    MAC_ALG_CFG_TRAFFIC_CTL_ENABLE,
    MAC_ALG_CFG_TRAFFIC_CTL_TIMEOUT,
    MAC_ALG_CFG_TRAFFIC_CTL_MIN_THRESHOLD,
    MAC_ALG_CFG_TRAFFIC_CTL_LOG_DEBUG,
    MAC_ALG_CFG_TRAFFIC_CTL_BUF_THRESHOLD,
    MAC_ALG_CFG_TRAFFIC_CTL_BUF_ADJ_ENABLE,
    MAC_ALG_CFG_TRAFFIC_CTL_BUF_ADJ_NUM,
    MAC_ALG_CFG_TRAFFIC_CTL_BUF_ADJ_CYCLE,
    MAC_ALG_CFG_TRAFFIC_CTL_RX_RESTORE_ENABLE,
    MAC_ALG_CFG_TRAFFIC_RX_RESTORE_NUM,
    MAC_ALG_CFG_TRAFFIC_RX_RESTORE_THRESHOLD,
    MAC_ALG_CFG_TRAFFIC_CTL_END,

    /* ��device���ն������������㷨���ò��� */
    MAC_ALG_CFG_RX_DSCR_CTL_START,
    MAC_ALG_CFG_RX_DSCR_CTL_ENABLE,
    MAC_ALG_CFG_RX_DSCR_CTL_LOG_DEBUG,
    MAC_ALG_CFG_RX_DSCR_CTL_END,
#endif

    /* MWO DET�㷨��־���ò���������ӵ���Ӧ��START��END֮�� */
    MAC_ALG_CFG_MWO_DET_START,

    MAC_ALG_CFG_MWO_DET_ENABLE,
    MAC_ALG_CFG_MWO_DET_END_RSSI_TH,
    MAC_ALG_CFG_MWO_DET_START_RSSI_TH,
    MAC_ALG_CFG_MWO_DET_DEBUG,

    MAC_ALG_CFG_MWO_DET_END,

    /* HiD2D��ؽӿ����ò���������ӵ���Ӧ��START��END֮�� */
    MAC_ALG_CFG_HID2D_START,

    MAC_ALG_CFG_HID2D_DEBUG_ENABLE,
    MAC_ALG_CFG_HID2D_RTS_ENABLE,
    MAC_ALG_CFG_HID2D_HIGH_BW_MCS_DISABLE,
    MAC_ALG_CFG_HID2D_HIGH_TXPOWER_ENABLE,
    MAC_ALG_CFG_HID2D_SET_APK_CCA_TH,

    MAC_ALG_CFG_HID2D_END,

    MAC_ALG_CFG_BUTT
} mac_alg_cfg_enum;
typedef uint8_t mac_alg_cfg_enum_uint8;

/* �㷨����ö�٣�����ֵ */
typedef struct {
    mac_alg_cfg_enum_uint8 en_alg_cfg; /* ��������ö�� */
    uint8_t uc_resv[3];              /* �ֽڶ��� */
    uint32_t ul_value;               /* ���ò���ֵ */
} mac_ioctl_alg_param_stru;

/* AUTORATE LOG �㷨����ö�٣�����ֵ */
typedef struct {
    mac_alg_cfg_enum_uint8 en_alg_cfg;         /* ��������ö�� */
    uint8_t auc_mac_addr[WLAN_MAC_ADDR_LEN]; /* MAC��ַ */
    uint8_t uc_ac_no;                        /* AC���� */
    uint8_t auc_resv[2];
    uint16_t us_value; /* ���ò���ֵ */
} mac_ioctl_alg_ar_log_param_stru;

/* AUTORATE ������ص�������� */
typedef struct {
    mac_alg_cfg_enum_uint8 en_alg_cfg;         /* ��������ö�� */
    uint8_t auc_mac_addr[WLAN_MAC_ADDR_LEN]; /* MAC��ַ */
    uint8_t auc_resv[1];
    uint16_t us_value; /* ������� */
} mac_ioctl_alg_ar_test_param_stru;

/* TXMODE LOG �㷨����ö�٣�����ֵ */
typedef struct {
    mac_alg_cfg_enum_uint8 en_alg_cfg;         /* ��������ö�� */
    uint8_t uc_ac_no;                        /* AC���� */
    uint8_t auc_mac_addr[WLAN_MAC_ADDR_LEN]; /* MAC��ַ */
    uint8_t auc_resv1[2];
    uint16_t us_value; /* ���ò���ֵ */
} mac_ioctl_alg_txbf_log_param_stru;

/* �㷨��������ӿ� */
typedef struct {
    uint8_t uc_argc;
    uint8_t auc_argv_offset[DMAC_ALG_CONFIG_MAX_ARG];
} mac_ioctl_alg_config_stru;

/* TPC LOG �㷨����ö�٣�����ֵ */
typedef struct {
    mac_alg_cfg_enum_uint8 en_alg_cfg;         /* ��������ö�� */
    uint8_t auc_mac_addr[WLAN_MAC_ADDR_LEN]; /* MAC��ַ */
    uint8_t uc_ac_no;                        /* AC���� */
    uint16_t us_value;                       /* ���ò���ֵ */
    int8_t *pc_frame_name;                   /* ��ȡ�ض�֡����ʹ�øñ��� */
} mac_ioctl_alg_tpc_log_param_stru;

/* cca opt LOG �㷨����ö�٣�����ֵ */
typedef struct {
    mac_alg_cfg_enum_uint8 en_alg_cfg; /* ��������ö�� */
    uint16_t us_value;               /* ͳ����ʱ�� */
    uint8_t auc_resv;
} mac_ioctl_alg_intfdet_log_param_stru;

/* m2s vap��Ϣ�ṹ�� */
typedef struct {
    uint8_t uc_vap_id;                  /* ҵ��vap id */
    wlan_nss_enum_uint8 en_vap_rx_nss;    /* vap�Ľ��տռ������� */
    wlan_m2s_type_enum_uint8 en_m2s_type; /* 0:���л� 1:Ӳ�л� */
    wlan_mib_mimo_power_save_enum_uint8 en_sm_power_save;

    oal_bool_enum_uint8 en_tx_stbc;
    oal_bool_enum_uint8 en_transmit_stagger_sounding;
    oal_bool_enum_uint8 en_vht_ctrl_field_supported;
    oal_bool_enum_uint8 en_vht_number_sounding_dimensions;

    oal_bool_enum_uint8 en_vht_su_beamformer_optionimplemented;
    oal_bool_enum_uint8 en_tx_vht_stbc_optionimplemented;
    oal_bool_enum_uint8 en_support_opmode;
    uint8_t uc_vht_ntx_sts;
    oal_bool_enum_uint8 en_su_bfee;
    uint8_t auc_reserved1[3];
#ifdef _PRE_WLAN_FEATURE_11AX
    oal_bool_enum_uint8 en_he_su_bfer;
    uint8_t uc_he_num_dim_below_80m;
    uint8_t uc_he_num_dim_over_80m;
    uint8_t uc_he_ntx_sts_below_80m;
    uint8_t uc_he_ntx_sts_over_80m;
    uint8_t auc_reserved2[3];
#endif
} mac_vap_m2s_stru;

#define MAX_MINIMUN_AP_COUNT      14
#ifdef _PRE_WLAN_FEATURE_FTM
/* FTM���Կ�����صĽṹ�� */
typedef struct {
    uint8_t uc_channel_num;
    uint8_t uc_burst_num;
    uint8_t uc_ftms_per_burst;
    oal_bool_enum_uint8 en_lci_civic_request;
    oal_bool_enum_uint8 en_is_asap_on;
    uint8_t uc_format_bw;
    uint8_t auc_bssid[WLAN_MAC_ADDR_LEN];
} mac_send_iftmr_stru;

typedef struct {
    oal_uint32 initiator_tsf_fix_offset;
    oal_uint32 responder_ptsf_offset;
} mac_ftm_timeout_stru;

typedef struct {
    uint32_t             ul_ftm_correct_time1;
    uint32_t             ul_ftm_correct_time2;
    uint32_t             ul_ftm_correct_time3;
    uint32_t             ul_ftm_correct_time4;
} mac_set_ftm_time_stru;

typedef struct {
    uint8_t auc_resv[2];
    uint8_t auc_mac[WLAN_MAC_ADDR_LEN];
} mac_send_ftm_stru;

typedef struct {
    uint8_t auc_mac[WLAN_MAC_ADDR_LEN];
    uint8_t uc_dialog_token;
    uint8_t uc_meas_token;

    uint16_t us_num_rpt;

    uint16_t us_start_delay;
    uint8_t auc_reserve[1];
    uint8_t uc_minimum_ap_count;
    uint8_t aauc_bssid[MAX_MINIMUN_AP_COUNT][WLAN_MAC_ADDR_LEN];
    uint8_t auc_channel[MAX_MINIMUN_AP_COUNT];
} mac_send_ftm_range_req_stru; /* ��ftm_range_req_stru ͬ���޸� */

typedef struct {
    uint8_t uc_tx_chain_selection;
    oal_bool_enum_uint8 en_is_mimo;
    uint8_t auc_reserve[2];
} ftm_m2s_stru;

typedef struct {
    uint8_t   auc_bssid[WLAN_MAC_ADDR_LEN];
} mac_neighbor_report_req_stru;

typedef struct {
    oal_bool_enum_uint8 en_lci_enable;
    oal_bool_enum_uint8 en_interworking_enable;
    oal_bool_enum_uint8 en_civic_enable;
    oal_bool_enum_uint8 en_geo_enable;
    uint8_t auc_bssid[WLAN_MAC_ADDR_LEN];
} mac_send_gas_init_req_stru;

typedef struct {
    uint8_t           uc_index;
    uint8_t           auc_resv[1];
    uint8_t           auc_mac[WLAN_MAC_ADDR_LEN];
}mac_ftm_set_white_list_stru;

typedef struct {
    uint32_t                      ul_cmd_bit_map;
    uint32_t                      ul_ftm_cali;
    oal_bool_enum_uint8             en_ftm_initiator_bit0;        /* ftm_initiator���� */
    mac_send_iftmr_stru             st_send_iftmr_bit1;           /* ����iftmr���� */
    oal_bool_enum_uint8             en_enable_bit2;               /* ʹ��FTM */
    oal_bool_enum_uint8             en_cali_bit3;                 /* FTM���� */
    mac_send_ftm_stru               st_send_ftm_bit4;             /* ����ftm���� */
    oal_bool_enum_uint8             en_ftm_resp_bit5;             /* ftm_resp���� */
    mac_set_ftm_time_stru           st_ftm_time_bit6;             /* ftm_time���� */
    mac_send_ftm_range_req_stru     st_send_range_req_bit7;       /* ����ftm_range_req���� */
    oal_bool_enum_uint8             en_ftm_range_bit8;            /* ftm_range���� */
    uint8_t                       uc_get_cali_reserv_bit9;
    ftm_m2s_stru                    st_m2s_bit11;
    oal_bool_enum_uint8             en_multipath_bit12;
    mac_ftm_timeout_stru            st_ftm_timeout_bit14;               /* �û����õ�ftm��ʱ����ʱʱ�� */
    mac_neighbor_report_req_stru    st_neighbor_report_req_bit15;       /* ����neighbour report request���� */
    mac_send_gas_init_req_stru      st_gas_init_req_bit16;              /* ����gas init req ���� */
    mac_ftm_set_white_list_stru     st_set_white_list_bit17;
}mac_ftm_debug_switch_stru;
#endif

#ifdef _PRE_WLAN_FEATURE_HID2D_TX_DROP
typedef struct {
    uint8_t uc_hid2d_debug_en;
    uint8_t uc_hid2d_delay_time;
    uint16_t us_resv;
} mac_hid2d_debug_switch_stru;
extern mac_hid2d_debug_switch_stru g_st_hid2d_debug_switch;
#endif

typedef struct {
    wlan_data_collect_enum_uint8 uc_type;
    uint32_t ul_reg_num;
} mac_data_collect_cfg_stru;
extern mac_data_collect_cfg_stru g_st_data_collect_cfg;

/*****************************************************************************
  9 OTHERS����
*****************************************************************************/
/*****************************************************************************
  10 ��������
*****************************************************************************/
#endif /* end of mac_vap_common.h */
