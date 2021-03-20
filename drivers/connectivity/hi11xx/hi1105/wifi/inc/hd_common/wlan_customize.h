

#ifndef __WLAN_CUSTOMIZE_H__
#define __WLAN_CUSTOMIZE_H__

/* 1 其他头文件包含 */
#include "oal_ext_if.h"
#include "wlan_spec.h"
#include "mac_user.h"
#include "securec.h"
#include "securectype.h"
#include "mac_common.h"

/* 2 宏定义 */
#define MAC_NUM_2G_BAND    3 /* 2g band数 */
#define MAC_NUM_5G_BAND    7 /* 5g band数 */
#define MAC_2G_CHANNEL_NUM 13
#define DEV_WORK_FREQ_LVL_NUM  4
#define CUS_DY_CALI_NUM_5G_BAND    5     /* 动态校准5g band1 2&3 4&5 6 7 */

#define RF_DYN_POW_BAND_NUM  4  /*  band2&3 4&5 6 7总4个  */
#define CUS_5G_BASE_PWR_NUM      5 /* 5g Base power 5个 band1 2&3 4&5 6 7 */
#define CUS_2G_BASE_PWR_NUM      1
#define HAL_CUS_NUM_FCC_CE_2G_PRO       3    /* 定制化2g FCC/CE 11B+OFDM_20M+OFDM_40M */
#define HAL_NUM_5G_20M_SIDE_BAND        6    /* 定制化5g边带数 */
#define HAL_NUM_5G_40M_SIDE_BAND        6
#define HAL_NUM_5G_80M_SIDE_BAND        5
#define HAL_NUM_5G_160M_SIDE_BAND       2
#define HAL_CUS_NUM_OF_SAR_PARAMS 8 /* 定制化降SAR参数 5G_BAND1~7 2.4G */
#define MAC_DPD_CALI_CUS_PARAMS_NUM (7)
#define CUS_NUM_2G_DELTA_RSSI_NUM  2     /* 40M/20M */
#define CUS_NUM_5G_DELTA_RSSI_NUM  4     /* 160M/80M/40M/20M */
#define CUS_NUM_2G_AMEND_RSSI_NUM  3     /* 40M/20M/11B */
#define CUS_NUM_5G_AMEND_RSSI_NUM  4     /* 160M/80M/40M/20M */
#define CUS_2G_FILTER_NARROW_AMEND_RSSI_NUM 2 /* 11b(phymode 40M)/20M(phymode 40M) */
#define CUS_5G_FILTER_NARROW_AMEND_RSSI_NUM 3 /* 真实信号20M下的phymode 40/80/160 */

#define CUS_SAR_LVL_NUM            20 /* 定制化降SAR档位数 */

#define WLAN_5G_BAND_IDX0 0
#define WLAN_5G_BAND_IDX1 1
#define WLAN_5G_BAND_IDX2 2
#define WLAN_5G_BAND_IDX3 3
#define WLAN_5G_BAND_IDX4 4
#define WLAN_5G_BAND_IDX5 5
#define WLAN_5G_BAND_IDX6 6

#define WLAN_2G_BAND_IDX0 0
#define WLAN_2G_BAND_IDX1 1
#define WLAN_2G_BAND_IDX2 2

#define WLAN_2G_DELTA_RSSI_BAND_20M 0
#define WLAN_2G_DELTA_RSSI_BAND_40M 1

#define WLAN_5G_DELTA_RSSI_BAND_20M 0
#define WLAN_5G_DELTA_RSSI_BAND_40M 1
#define WLAN_5G_DELTA_RSSI_BAND_80M 2
#define WLAN_5G_DELTA_RSSI_BAND_160M 3

#define WLAN_2G_AMEND_RSSI_BAND_20M 0
#define WLAN_2G_AMEND_RSSI_BAND_40M 1
#define WLAN_2G_AMEND_RSSI_BAND_11B 2

#define WLAN_5G_AMEND_RSSI_BAND_20M 0
#define WLAN_5G_AMEND_RSSI_BAND_40M 1
#define WLAN_5G_AMEND_RSSI_BAND_80M 2
#define WLAN_5G_AMEND_RSSI_BAND_160M 3

#define WLAN_RF_DYN_POW_BAND_2_3 0
#define WLAN_RF_DYN_POW_BAND_4_5 1
#define WLAN_RF_DYN_POW_BAND_6   2
#define WLAN_RF_DYN_POW_BAND_7   3

#define WLAN_2G_CHN_IDX0 0
#define WLAN_2G_CHN_IDX1 1
#define WLAN_2G_CHN_IDX2 2
#define WLAN_2G_CHN_IDX3 3
#define WLAN_2G_CHN_IDX4 4
#define WLAN_2G_CHN_IDX5 5
#define WLAN_2G_CHN_IDX6 6
#define WLAN_2G_CHN_IDX7 7
#define WLAN_2G_CHN_IDX8 8
#define WLAN_2G_CHN_IDX9 9
#define WLAN_2G_CHN_IDX10 10
#define WLAN_2G_CHN_IDX11 11
#define WLAN_2G_CHN_IDX12 12

#define CUS_DPD_CALI_PARAMS0 0
#define CUS_DPD_CALI_PARAMS1 1
#define CUS_DPD_CALI_PARAMS2 2
#define CUS_DPD_CALI_PARAMS3 3
#define CUS_DPD_CALI_PARAMS4 4
#define CUS_DPD_CALI_PARAMS5 5
#define CUS_DPD_CALI_PARAMS6 6

#define DPD_CHN1_CHN8_NUM 8
#define DPD_CHN9_CHN13_NUM 5

#define DEV_WORK_FREQ_LVL0 0
#define DEV_WORK_FREQ_LVL1 1
#define DEV_WORK_FREQ_LVL2 2
#define DEV_WORK_FREQ_LVL3 3

#define CUS_GET_BOOL(_val)         ((!(_val)) ? OAL_FALSE : OAL_TRUE);
#define CUS_SET_BOOL(_val)         ((_val) = (!!(_val)))

#define CUS_CHECK_VAL(_val, _max, _min, _default)  \
    ((_val) = ((_val) <= (_max)  && (_val) >=(_min)) ? (_val) : (_default))

#define CUS_CHECK_MIN_VAL(_val, _min, _default) ((_val) = ((_val) >=(_min)) ? (_val) : (_default))
#define CUS_CHECK_MAX_VAL(_val, _max, _default) ((_val) = ((_val) <=(_max)) ? (_val) : (_default))

/* 3 枚举定义 */
/* 枚举变量个数是4的倍数:保持4字节对齐 */
typedef enum {
    WLAN_CFGID_SET_CUS_RF_CFG = 0, /* 前端rf相关定制化 */
    WLAN_CFGID_SET_CUS_RF_CALI = 1, /* rf校准相关定制化 */
    WLAN_CFGID_SET_CUS_CAP = 2,      /* 特性功能能力定制化 */
    WLAN_CFGID_SET_CUS_POW = 3,             /* 功率定制化 */
    WLAN_CFGID_SET_CUS_DYN_POW_CALI = 4,       /* 动态功率校准定制化 */

    WLAN_CFGID_SET_CUS_BUTT
} cus_cust_cfgid_enum;

typedef enum {
    HAL_POW_RF_SEL_CHAIN_1 = 0, /* SISO power */
    HAL_POW_RF_SEL_CHAIN_2,     /* 2X2 MIMO power */
    HAL_POW_RF_SEL_CHAIN_3,     /* 3X3 MIMO power */
    HAL_POW_RF_SEL_CHAIN_4,     /* 4X4 MIMO power */
    HAL_POW_RF_SEL_CHAIN_BUTT
} hal_pow_rf_chain_sel_enum;

typedef enum {
    WLAN_CUST_NUM_1 = 1,
    WLAN_CUST_NUM_2,
    WLAN_CUST_NUM_3,
    WLAN_CUST_NUM_4,
}WLAN_CFG_NUM;

typedef enum {
    DYN_PWR_CUST_SNGL_MODE_11B = 0,
    DYN_PWR_CUST_SNGL_MODE_OFDM20,
    DYN_PWR_CUST_SNGL_MODE_OFDM40,
    DYN_PWR_CUST_SNGL_MODE_CW,
    DYN_PWR_CUST_2G_SNGL_MODE_BUTT,
} cust_dyn_2g_pwr_sngl_mode_enum;

typedef enum {
    /*  nvram_params0  */
    CUST_DELTA_POW_2P4G_11B_1M_2M,
    CUST_DELTA_POW_2P4G_11B_5M_11M,
    CUST_DELTA_POW_2P4G_11G_6M_9M,
    CUST_DELTA_POW_2P4G_11G_12M_18M,
    /*  nvram_params1  */
    CUST_DELTA_POW_2P4G_11G_24M_36M,
    CUST_DELTA_POW_2P4G_11G_48M,
    CUST_DELTA_POW_2P4G_11G_54M,
    CUST_DELTA_POW_2P4G_HT20_VHT20_MCS0,
    /*  nvram_params2  */
    CUST_DELTA_POW_2P4G_HT20_VHT20_MCS2_3,
    CUST_DELTA_POW_2P4G_HT20_VHT20_MCS4_5,
    CUST_DELTA_POW_2P4G_HT20_VHT20_MCS6,
    CUST_DELTA_POW_2P4G_HT20_VHT20_MCS7,
    /*  nvram_params3  */
    CUST_DELTA_POW_2P4G_VHT20_MCS8,
    CUST_DELTA_POW_2P4G_HT40_VHT40_MCS0_1,
    CUST_DELTA_POW_2P4G_HT40_VHT40_MCS2_3,
    CUST_DELTA_POW_2P4G_HT40_VHT40_MCS4_5,
    /*  nvram_params4  */
    CUST_DELTA_POW_2P4G_HT40_VHT40_MCS6,
    CUST_DELTA_POW_2P4G_HT40_VHT40_MCS7,
    CUST_DELTA_POW_2P4G_VHT40_MCS8,
    CUST_DELTA_POW_2P4G_VHT40_MCS9,
    /*  nvram_params5  */
    CUST_DELTA_POW_2P4G_HT40_MCS32,
    CUST_DELTA_POW_5G_11A_6M_9M,
    CUST_DELTA_POW_5G_11A_12M_18M,
    CUST_DELTA_POW_5G_11A_24M_36M,
    /*  nvram_params6  */
    CUST_DELTA_POW_5G_11A_48M,
    CUST_DELTA_POW_5G_11A_54M,
    CUST_DELTA_POW_5G_HT20_VHT20_MCS0_1,
    CUST_DELTA_POW_5G_HT20_VHT20_MCS2_3,
    /*  nvram_params7  */
    CUST_DELTA_POW_5G_HT20_VHT20_MCS4_5,
    CUST_DELTA_POW_5G_HT20_VHT20_MCS6,
    CUST_DELTA_POW_5G_HT20_VHT20_MCS7,
    CUST_DELTA_POW_5G_VHT20_MCS8,
    /*  nvram_params8  */
    CUST_DELTA_POW_5G_HT40_VHT40_MCS0_1,
    CUST_DELTA_POW_5G_HT40_VHT40_MCS2_3,
    CUST_DELTA_POW_5G_HT40_VHT40_MCS4_5,
    CUST_DELTA_POW_5G_HT40_VHT40_MCS6,
    /*  nvram_params9  */
    CUST_DELTA_POW_5G_HT40_VHT40_MCS7,
    CUST_DELTA_POW_5G_VHT40_MCS8,
    CUST_DELTA_POW_5G_HT40_MCS32,
    CUST_DELTA_POW_5G_VHT80_MCS0_1,
    /*  nvram_params10  */
    CUST_DELTA_POW_5G_VHT80_MCS2_3,
    CUST_DELTA_POW_5G_VHT80_MCS4_5,
    CUST_DELTA_POW_5G_VHT80_MCS6,
    CUST_DELTA_POW_5G_VHT80_MCS7,
    /*  nvram_params11  */
    CUST_DELTA_POW_5G_VHT80_MCS8,
    CUST_DELTA_POW_2P4G_VHT20_MCS9,
    CUST_DELTA_POW_2P4G_VHT20_MCS10,
    CUST_DELTA_POW_2P4G_VHT20_MCS11,
    /*  nvram_params12  */
    CUST_DELTA_POW_2P4G_VHT40_MCS10,
    CUST_DELTA_POW_2P4G_VHT40_MCS11,
    CUST_DELTA_POW_5G_VHT20_MCS9,
    CUST_DELTA_POW_5G_VHT20_MCS10,
    /*  nvram_params13  */
    CUST_DELTA_POW_5G_VHT20_MCS11,
    CUST_DELTA_POW_5G_VHT40_MCS10,
    CUST_DELTA_POW_5G_VHT40_MCS11,
    CUST_DELTA_POW_5G_VHT80_MCS10,
    /*  nvram_params14  */
    CUST_DELTA_POW_5G_VHT80_MCS11,
    CUST_DELTA_POW_5G_VHT160_MCS0,
    CUST_DELTA_POW_5G_VHT160_MCS1,
    CUST_DELTA_POW_5G_VHT160_MCS2,
    /*  nvram_params15  */
    CUST_DELTA_POW_5G_VHT160_MCS3,
    CUST_DELTA_POW_5G_VHT160_MCS4,
    CUST_DELTA_POW_5G_VHT160_MCS5,
    CUST_DELTA_POW_5G_VHT160_MCS6,
    /*  nvram_params16  */
    CUST_DELTA_POW_5G_VHT160_MCS7,
    CUST_DELTA_POW_5G_VHT160_MCS8,
    CUST_DELTA_POW_5G_VHT160_MCS9,
    CUST_DELTA_POW_5G_VHT160_MCS10,
    /*  nvram_params17  */
    CUST_DELTA_POW_5G_VHT160_MCS11,
    CUST_DELTA_POW_5G_VHT40_MCS9,
    CUST_DELTA_POW_5G_VHT80_MCS9,
    CUST_DELTA_POW_RSV1,
    CUST_NORMAL_DELTA_POW_RATE_BUTT = CUST_DELTA_POW_RSV1,
    /*  4字节对齐填充后BUTT  */
    CUST_NORMAL_DELTA_POW_RATE_ALGN_BUTT,
} wlan_cust_delta_pow_enum;

typedef enum {
    /*  nvram_params59  */
    CUST_DELTA_POW_24G_HT20_VHT20_DPD_ON_MCS7,
    CUST_DELTA_POW_24G_VHT20_DPD_ON_MCS8,
    CUST_DELTA_POW_24G_VHT20_DPD_ON_MCS9,
    CUST_DELTA_POW_24G_VHT20_DPD_ON_MCS10,
    /*  nvram_params60  */
    CUST_DELTA_POW_24G_VHT20_DPD_ON_MCS11,
    CUST_DELTA_POW_24G_HT40_VHT40_DPD_ON_MCS7,
    CUST_DELTA_POW_24G_VHT40_DPD_ON_MCS8,
    CUST_DELTA_POW_24G_VHT40_DPD_ON_MCS9,
    /*  nvram_params61  */
    CUST_DELTA_POW_24G_VHT40_DPD_ON_MCS10,
    CUST_DELTA_POW_24G_VHT40_DPD_ON_MCS11,
    CUST_DPD_DELTA_POW_RATE_RSV1,
    CUST_DPD_DELTA_POW_RATE_BUTT = CUST_DPD_DELTA_POW_RATE_RSV1,
    CUST_DPD_DELTA_POW_RATE_RSV2,
    /*  4字节对齐填充后BUTT  */
    CUST_DPD_DELTA_POW_RATE_ALGN_BUTT,
}wlan_cust_dpd_delta_pow_enum;

/* 定制化 DPD校准配置参数 */
typedef struct {
    oal_uint32 dpd_cali_cus[MAC_DPD_CALI_CUS_PARAMS_NUM][WLAN_RF_CHANNEL_MAX_NUMS];
} mac_dpd_cali_stru;

typedef struct {
    mac_dpd_cali_stru dpd_cali_para;
    int16_t gm0_dB10_amend[WLAN_RF_CHANNEL_MAX_NUMS];
    uint16_t dyn_cali_dscr_interval[WLAN_BAND_BUTT]; /* 动态校准开关2.4g 5g */

    oal_bool_enum_uint8 en_dyn_cali_opt_switch;
    uint8_t band_5g_enable;
    uint8_t resv[2];  /* 保留字段:2字节 */
} mac_cus_rf_cali_stru;

/* 定制化 power ref 2g 5g配置参数 */
/* customize rf front cfg struct */
typedef struct {
    /* 2g */
    oal_int8 gain_db_2g[MAC_NUM_2G_BAND][WLAN_RF_CHANNEL_MAX_NUMS];
    /* 5g */
    oal_int8 gain_db_5g[MAC_NUM_5G_BAND][WLAN_RF_CHANNEL_MAX_NUMS];
} mac_cust_rf_loss_gain_db_stru;

typedef struct {
    oal_int8 delta_rssi_2g[CUS_NUM_2G_DELTA_RSSI_NUM][WLAN_RF_CHANNEL_MAX_NUMS];
    oal_int8 delta_rssi_5g[CUS_NUM_5G_DELTA_RSSI_NUM][WLAN_RF_CHANNEL_MAX_NUMS];
} mac_cus_rf_delta_pwr_ref_stru;

typedef struct {
    oal_int8 amend_rssi_2g[CUS_NUM_2G_AMEND_RSSI_NUM][WLAN_RF_CHANNEL_MAX_NUMS];
    oal_int8 amend_rssi_5g[CUS_NUM_5G_AMEND_RSSI_NUM][WLAN_RF_CHANNEL_MAX_NUMS];
} mac_custom_rf_amend_rssi_stru;

typedef struct {
    oal_uint16 lna_on2off_time_ns[WLAN_RF_CHANNEL_MAX_NUMS]; /* LNA开到LNA关的时间(ns) */
    oal_uint16 lna_off2on_time_ns[WLAN_RF_CHANNEL_MAX_NUMS]; /* LNA关到LNA开的时间(ns) */

    oal_int8 lna_bypass_gain_db[WLAN_RF_CHANNEL_MAX_NUMS];   /* 外部LNA bypass时的增益(dB) */
    oal_int8 lna_gain_db[WLAN_RF_CHANNEL_MAX_NUMS];          /* 外部LNA增益(dB) */
    oal_int8 pa_gain_b0_db[WLAN_RF_CHANNEL_MAX_NUMS];        /* 外部PA b0 增益(dB) */
    oal_uint8 ext_switch_isexist[WLAN_RF_CHANNEL_MAX_NUMS]; /* 是否使用外部switch */

    oal_uint8 ext_pa_isexist[WLAN_RF_CHANNEL_MAX_NUMS];                  /* 是否使用外部pa */
    oal_fem_lp_state_enum_uint8 en_fem_lp_enable[WLAN_RF_CHANNEL_MAX_NUMS]; /* 是否支持fem低功耗标志 */
    oal_int8 c_fem_spec_value[WLAN_RF_CHANNEL_MAX_NUMS];                    /* fem spec功率点 */
    oal_uint8 ext_lna_isexist[WLAN_RF_CHANNEL_MAX_NUMS];                 /* 是否使用外部lna */

    oal_int8 pa_gain_b1_db[WLAN_RF_CHANNEL_MAX_NUMS];     /* 外部PA b1增益(dB) */
    oal_uint8 pa_gain_lvl_num[WLAN_RF_CHANNEL_MAX_NUMS]; /* 外部PA 增益档位数 */
} mac_custom_ext_rf_stru;

typedef struct {
    mac_cust_rf_loss_gain_db_stru rf_loss_gain_db;           /* 2.4g 5g 插损 */
    mac_custom_ext_rf_stru ext_rf[WLAN_BAND_BUTT];   /* 2.4g 5g fem */
    mac_cus_rf_delta_pwr_ref_stru delta_pwr_ref;         /* delta_rssi */
    mac_custom_rf_amend_rssi_stru rssi_amend_cfg;       /* rssi_amend */

    uint8_t far_dist_pow_gain_switch;           /* 超远距离功率增益开关 */
    uint8_t far_dist_dsss_scale_promote_switch; /* 超远距11b 1m 2m dbb scale提升使能开关 */
    uint8_t chn_radio_cap;
    int8_t delta_cca_ed_high_80th_5g;

    /* 注意，如果修改了对应的位置，需要同步修改函数: hal_config_custom_rf  */
    int8_t delta_cca_ed_high_20th_2g;
    int8_t delta_cca_ed_high_40th_2g;
    int8_t delta_cca_ed_high_20th_5g;
    int8_t delta_cca_ed_high_40th_5g;

    uint8_t dual_antenna_en;
    uint8_t rsv[3];    /* 保留字段:3字节 */
} mac_customize_rf_front_sru;

typedef struct {
    uint32_t over_temper_protect_th;
    uint8_t en_over_temp_pro;
    uint8_t en_reduce_pwr;
    uint8_t over_temp_pro_safe_th;
    uint8_t over_temp_pro_over_th;

    uint8_t over_temp_pro_pa_off_th;
    uint8_t rsv[3];    /* 保留字段:3字节 */
}over_temper_stru;

typedef struct {
    oal_uint32 speed_level;    /* 吞吐量门限 */
    oal_uint32 min_cup_freq; /* CPU频率level */
    oal_uint32 min_ddr_freq; /* DDR主频下限 */
    uint8_t cpu_freq_type; /* device主频类型 */
    uint8_t uc_rsv[3];    /* 保留字段:3字节 */
} dev_work_lvl_stru;

typedef struct {
    uint8_t wlan_min_fast_ps_idle;
    uint8_t wlan_max_fast_ps_idle;
    uint8_t wlan_auto_ps_thresh_screen_on;
    uint8_t wlan_auto_ps_thresh_screen_off;
} cust_fast_pow_save_stru;

typedef struct {
    uint16_t upc_for_18dbm[WLAN_RF_CHANNEL_MAX_NUMS];
    dev_work_lvl_stru dev_frequency[DEV_WORK_FREQ_LVL_NUM];   /*  device工作频率  */
    cust_fast_pow_save_stru fast_ps;
    dmac_ax_fem_pow_ctl_stru fem_pow;

    uint32_t cali_mask;
    int32_t loglevel;
    uint32_t voe_switch_mask;
    uint32_t wifi_11ax_switch_mask;
    uint32_t mult_bssid_switch_mask;
    uint32_t htc_switch_mask;
    uint32_t hiex_cap;
    int32_t tx_switch;
    uint32_t ps_rssi_param;
    uint32_t ps_pps_param;

    over_temper_stru over_temper_protect;

    wlan_bw_cap_enum_uint8 en_channel_width;
    oal_bool_enum_uint8 en_ldpc_is_supp;     /* 是否支持接收LDPC编码的包 */
    oal_bool_enum_uint8 en_rx_stbc_is_supp;  /* 是否支持stbc接收,支持2个空间流 */
    oal_bool_enum_uint8 en_tx_stbc_is_supp;  /* 是否支持最少2x1 STBC发送 */

    oal_bool_enum_uint8 en_su_bfmer_is_supp; /* 是否支持单用户beamformer */
    oal_bool_enum_uint8 en_su_bfmee_is_supp; /* 是否支持单用户beamformee */
    oal_bool_enum_uint8 en_mu_bfmer_is_supp; /* 是否支持多用户beamformer */
    oal_bool_enum_uint8 en_mu_bfmee_is_supp; /* 是否支持多用户beamformee */

    uint8_t supp_11n_txbf;  /* 11n是否支持txbf */
    oal_bool_enum_uint8 en_1024qam_is_supp;  /* 支持1024QAM速率 */
    uint8_t radio_cap[WLAN_DEVICE_MAX_NUM_PER_CHIP];

    uint8_t rssi_switch_ant;
    oal_bool_enum_uint8 en_country_self_study;  /* 支持国家码自学习功能 */
    uint8_t mcm_custom_func_mask;
    uint8_t mcm_func_mask;

    uint16_t used_mem_for_start;
    uint16_t used_mem_for_stop;

    uint8_t sdio_assem_d2h;
    uint8_t sdio_assem_h2d;
    uint8_t linkloss_threshold_fixed;
    uint8_t aput_160M_switch;

    int16_t dsss2ofdm_dbb_pwr_bo_val;
    uint16_t download_rate_limit_pps;

    uint8_t ac_suspend_mask;
    uint8_t dyn_extlna_bypass;
    int8_t frame_tx_chain_ctl;
    uint8_t double_chain_11b_bo_pow;

    uint8_t hcc_flowctrl_type;
    uint8_t hcc_flowctrl_switch;
    uint8_t autocali_switch;
    uint8_t txopps_switch;

    uint8_t phy_cap_mask;
    uint8_t wlan_device_pm_switch;
    uint8_t fast_mode;
    uint8_t optimized_feature_mask;

    uint8_t ddr_switch_mask;
    uint8_t dc_flowctl_switch;
    uint8_t mbo_switch;
    uint8_t fastscan_switch;

    uint8_t linkloss_th[WLAN_LINKLOSS_MODE_BUTT];
    uint8_t ftm_cap;

    uint8_t btcoex_threshold_mcm[WLAN_BTCOEX_RSSI_MCM_BUTT];
    uint8_t radar_isr_forbid;
    uint8_t dbac_dynamic_switch;

    uint8_t evm_pll_reg_fix;
    uint8_t cali_data_mask;
    uint8_t rsv[2];    /* 保留字段:2字节 */
}mac_cust_feature_cap_sru;
typedef struct {
    int8_t dpn_2g[DYN_PWR_CUST_2G_SNGL_MODE_BUTT][MAC_2G_CHANNEL_NUM];
    int8_t dpn_5g[WLAN_BW_CAP_80PLUS80][MAC_NUM_5G_BAND];
} cus_dy_cali_dpn_stru;

typedef struct {
    hal_pwr_fit_para_stru pa2gccka_para[WLAN_RF_CHANNEL_MAX_NUMS];
    hal_pwr_fit_para_stru pa2ga_para[WLAN_RF_CHANNEL_MAX_NUMS];
    hal_pwr_fit_para_stru pa2g40a_para[WLAN_RF_CHANNEL_MAX_NUMS];
    hal_pwr_fit_para_stru pa2gcwa_para[WLAN_RF_CHANNEL_MAX_NUMS];
    hal_pwr_fit_para_stru ppa2gcwa_para[WLAN_RF_CHANNEL_MAX_NUMS];

    hal_pwr_fit_para_stru pa5ga_para[WLAN_RF_CHANNEL_MAX_NUMS][RF_DYN_POW_BAND_NUM];
    hal_pwr_fit_para_stru pa5ga_low_para[WLAN_RF_CHANNEL_MAX_NUMS][RF_DYN_POW_BAND_NUM];
    hal_pwr_fit_para_stru pa5ga_band1[WLAN_RF_CHANNEL_MAX_NUMS];
    hal_pwr_fit_para_stru pa5ga_band1_low[WLAN_RF_CHANNEL_MAX_NUMS];
    cus_dy_cali_dpn_stru dpn_para[WLAN_RF_CHANNEL_MAX_NUMS];
    int16_t extre_point_val[CUS_DY_CALI_NUM_5G_BAND];
    int8_t rsv[2];    /* 保留字段:2字节 */
}mac_cust_dyn_pow_sru;

/* FCC/CE边带功率定制项 */
typedef struct {
    uint8_t fcc_txpwr_limit_5g_20M[HAL_NUM_5G_20M_SIDE_BAND];
    uint8_t fcc_txpwr_limit_5g_40M[HAL_NUM_5G_40M_SIDE_BAND];
    uint8_t fcc_txpwr_limit_5g_80M[HAL_NUM_5G_80M_SIDE_BAND];
    uint8_t fcc_txpwr_limit_5g_160M[HAL_NUM_5G_160M_SIDE_BAND];
    uint8_t fcc_txpwr_limit_2g[WLAN_2G_SUB_BAND_NUM][HAL_CUS_NUM_FCC_CE_2G_PRO];
    uint8_t rsv[2]; /* 保留字段:2字节 */
} hal_cfg_custom_fcc_txpwr_limit_stru;

typedef struct {
    uint8_t sar_ctrl_params[HAL_CUS_NUM_OF_SAR_PARAMS];
} wlan_cust_sar_cfg_stru;

typedef struct {
    oal_uint8 ru_pwr_2g[WLAN_HE_RU_SIZE_996];
    oal_int8  rsv[3]; /* 保留字段:3字节 */
} wlan_cust_2g_ru_pow_stru;

typedef struct {
    oal_uint8 ru_pwr_5g[WLAN_HE_RU_SIZE_BUTT];
    oal_int8  rsv;
} wlan_cust_5g_ru_pow_stru;


typedef struct {
    oal_uint8 ru_pwr_2g[WLAN_HE_RU_SIZE_996];
    oal_int8  rsv[3]; /* 保留字段:3字节 */
} wlan_cust_2g_tpc_ru_pow_stru;

typedef struct {
    oal_uint8 ru_pwr_5g[WLAN_HE_RU_SIZE_BUTT];
    oal_int8  rsv;
} wlan_cust_5g_tpc_ru_pow_stru;


/* 4字节对齐 */
typedef struct {
    int8_t delt_txpwr_params[CUST_NORMAL_DELTA_POW_RATE_ALGN_BUTT];
    int8_t dpd_delt_txpwr_params[CUST_DPD_DELTA_POW_RATE_ALGN_BUTT];
    int8_t delt_txpwr_11b_params[WLAN_RF_CHANNEL_MAX_NUMS];
    uint8_t upper_upc_5g_params[WLAN_RF_CHANNEL_MAX_NUMS];
    uint8_t backoff_pow_5g_params[WLAN_RF_CHANNEL_MAX_NUMS];
    uint8_t dsss_low_pow_amend_2g[WLAN_RF_CHANNEL_MAX_NUMS];
    uint8_t ofdm_low_pow_amend_2g[WLAN_RF_CHANNEL_MAX_NUMS];

    uint8_t txpwr_base_2g_params[WLAN_RF_CHANNEL_MAX_NUMS][CUS_2G_BASE_PWR_NUM];
    uint8_t txpwr_base_5g_params[WLAN_RF_CHANNEL_MAX_NUMS][CUS_5G_BASE_PWR_NUM];
    int8_t delt_txpwr_base_params[WLAN_BAND_BUTT][WLAN_RF_CHANNEL_MAX_NUMS]; /* MIMO相对SISO的base power差值 */
    /* FCC/CE边带功率定制项 */
    hal_cfg_custom_fcc_txpwr_limit_stru fcc_ce_param[WLAN_RF_CHANNEL_MAX_NUMS];
    /* SAR CTRL */
    wlan_cust_sar_cfg_stru sar_ctrl_params[WLAN_RF_CHANNEL_MAX_NUMS][CUS_SAR_LVL_NUM];
#ifdef _PRE_WLAN_FEATURE_TAS_ANT_SWITCH
    /* TAS CTRL */
    oal_uint8 tas_ctrl_params[WLAN_BAND_BUTT][WLAN_RF_CHANNEL_MAX_NUMS];
#endif
    wlan_cust_2g_ru_pow_stru  full_bw_ru_pow_2g[WLAN_BW_CAP_80M];
    wlan_cust_5g_ru_pow_stru  full_bw_ru_pow_5g[WLAN_BW_CAP_BUTT];

    wlan_cust_2g_tpc_ru_pow_stru tpc_tb_ru_2g_max_pow[HAL_POW_RF_SEL_CHAIN_BUTT];
    wlan_cust_5g_tpc_ru_pow_stru tpc_tb_ru_5g_max_pow[HAL_POW_RF_SEL_CHAIN_BUTT];
    oal_uint8 tpc_tb_ru_max_power[WLAN_BAND_BUTT];
    oal_uint8 rsv[2];    /* 保留字段:2字节 */

    uint32_t fcc_ce_max_pwr_for_5g_high_band;
}wlan_cust_pow_stru;

#endif /* end of wlan_customize.h */
