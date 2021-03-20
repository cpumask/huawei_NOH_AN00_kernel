

#ifndef __HMAC_CALI_MGMT_H__
#define __HMAC_CALI_MGMT_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 1 ����ͷ�ļ����� */
#include "frw_ext_if.h"

#include "hmac_vap.h"
#include "plat_cali.h"
#include "wlan_spec.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_CALI_MGMT_H
/* 2 �궨�� */
#define DPD_CORRAM_DATA_NUM                (512)
#define DPD_CORRAM_DATA_LEN                (1024)
#define HI1103_DPD_OFFLINE_CALI_BW_NUM     (2)
#define HI1103_2G_DPD_CALI_CHANNEL_NUM_20M (4)
#define HI1103_2G_DPD_CALI_CHANNEL_NUM_40M (3)
#define HI1103_2G_DPD_CALI_CHANNEL_NUM     (HI1103_2G_DPD_CALI_CHANNEL_NUM_20M + HI1103_2G_DPD_CALI_CHANNEL_NUM_40M)

#define HI1103_DPD_CALI_TPC_LEVEL_NUM (2)
#define HI1103_DPD_CALI_LUT_LENGTH    (31)

#define HI1103_DPD_OFFLINE_CALI_LUT_NUM (HI1103_2G_DPD_CALI_CHANNEL_NUM * HI1103_DPD_CALI_TPC_LEVEL_NUM)

#define HI1103_CALI_SISO_TXDC_GAIN_LVL_NUM      (8) /* SISO tx dc����ֵ��λ��Ŀ */
#define HI1103_CALI_MIMO_TXDC_GAIN_LVL_NUM      (8) /* MIMO tx dc����ֵ��λ��Ŀ */
#define HI1103_CALI_TXDC_GAIN_LVL_NUM           (HI1103_CALI_SISO_TXDC_GAIN_LVL_NUM +  \
                                                 HI1103_CALI_MIMO_TXDC_GAIN_LVL_NUM)
#define HI1103_CALI_IQ_TONE_NUM                 (16)
#define HI1103_CALI_RXDC_GAIN_LVL_NUM           (8) /* rx dc����ֵ��λ��Ŀ */
#define HI1103_CALI_ADC_CH_NUM                  (4) /* 4·ADC */
#define HI1103_CALI_2G_OTHER_CHANNEL_NUM        (1)
#define HI1103_2G_CHANNEL_NUM                   (3)
#define HI1103_5G_20M_CHANNEL_NUM               (7)
#define HI1103_5G_80M_CHANNEL_NUM               (7)
#define HI1103_5G_160M_CHANNEL_NUM              (2)
#define HI1103_5G_CHANNEL_NUM                   (HI1103_5G_20M_CHANNEL_NUM +  \
                                                 HI1103_5G_80M_CHANNEL_NUM +  \
                                                 HI1103_5G_160M_CHANNEL_NUM)
#define HI1103_CALI_RXIQ_LS_FILTER_TAP_NUM_160M (17)
#define HI1103_CALI_TXIQ_LS_FILTER_TAP_NUM      (15)
#define HI1103_CALI_RXIQ_LS_FILTER_TAP_NUM_320M (HI1103_CALI_RXIQ_LS_FILTER_TAP_NUM_160M)

#define HI1103_CALI_RXIQ_LS_FILTER_TAP_NUM_80M 15

#define HI1103_RXIQ_LSFIR_FEQ_NUM_320M 128
#define HI1103_CALI_RXIQ_LS_FILTER_FEQ_NUM_160M 64
#define HI1103_CALI_RXIQ_LS_FILTER_FEQ_NUM_80M  32

#define HI1103_CALI_TXIQ_LS_FILTER_FEQ_NUM_640M 256
#define HI1103_CALI_TXIQ_LS_FILTER_FEQ_NUM_320M 128
#define HI1103_CALI_TXIQ_LS_FILTER_FEQ_NUM_160M 64

#define NUM_80M_SNGL_TONE_1_4_CIRCLE  16 /* DAC 80Mʱ1/4����1.25M�������������� */
#define NUM_160M_SNGL_TONE_1_4_CIRCLE 16 /* DAC 80Mʱ1/4����2.5M�������������� */
#define NUM_320M_SNGL_TONE_1_4_CIRCLE 32 /* DAC 160Mʱ1/4����2.5M�������������� */
#define NUM_640M_SNGL_TONE_1_4_CIRCLE 64 /* DAC 320Mʱ1/4����2.5M�������������� */

#define NUM_80M_SNGL_TONE_1_CIRCLE 2 /* DAC 80Mʱ1/4����1.25M������������ڣ�1/4����Ϊһ����λ */
#define NUM_BW_SNGL_TONE_1_CIRCLE 4 /* DAC ��ͬ�����������һ�����ڣ�1/4����Ϊһ����λ */

#define HI1103_CALI_MATRIX_DATA_NUMS  23
#define HI1106_DPD_CORRAM_DATA_NUM    6144
#define HI1106_CALI_MATRIX_DATA_NUMS  1

#define HMAC_NETBUF_OFFSET  104

/* 3 ö�ٶ��� */
/* 4 ȫ�ֱ������� */
extern int8_t g_wait_wlan_power_on_cali;
extern oal_completion g_wlan_cali_completed;
/* 5 ��Ϣͷ���� */
/* 6 ��Ϣ���� */
/* 7 STRUCT���� */
typedef enum {
    HI1103_CALI_SISO,
    HI1103_CALI_MIMO,

    HI1103_CALI_CHAIN_NUM_BUTT,
} hi1103_rf_cali_chain_num_enum;

typedef struct {
    uint8_t uc_rx_gain_cmp_code; /* ��pilot RFʹ�ã�CУ׼����ֵ */

    /* LODIV ��ʱ��rx gain���ýṹ */
    uint8_t uc_rx_mimo_cmp;
    uint8_t uc_dpd_siso_cmp;
    uint8_t uc_dpd_mimo_cmp;
} hi1103_rx_gain_comp_val_stru;

typedef struct {
    uint16_t aus_analog_rxdc_siso_cmp[HI1103_CALI_RXDC_GAIN_LVL_NUM];
    uint16_t aus_analog_rxdc_mimo_extlna_cmp[HI1103_CALI_RXDC_GAIN_LVL_NUM];
    uint16_t aus_analog_rxdc_mimo_intlna_cmp[HI1103_CALI_RXDC_GAIN_LVL_NUM];
    uint16_t us_digital_rxdc_cmp_i;
    uint16_t us_digital_rxdc_cmp_q;
    int16_t s_cali_temperature;
    int16_t s_mimo_cali_temperature;
} hi1103_rx_dc_comp_val_stru;

typedef struct {
    uint8_t uc_ppa_cmp;
    uint8_t uc_atx_pwr_cmp;
    uint8_t uc_dtx_pwr_cmp;
    int8_t c_dp_init;
    int16_t s_2g_tx_ppa_dc;
    int16_t s_2g_tx_power_dc;
} hi1103_2G_tx_power_comp_val_stru;

typedef struct {
    uint8_t uc_ppa_cmp;
    uint8_t uc_mx_cmp;
    uint8_t uc_atx_pwr_cmp;
    uint8_t uc_dtx_pwr_cmp;
    int16_t s_5g_tx_power_dc;
    uint8_t auc_reserve[2];
} hi1103_5G_tx_power_comp_val_stru;

typedef struct {
    uint8_t uc_ssb_cmp;
    uint8_t uc_buf0_cmp;
    uint8_t uc_buf1_cmp;
    uint8_t uc_resv;
} hi1103_logen_comp_val_stru;

typedef struct {
    uint8_t uc_classa_cmp;
    uint8_t uc_classb_cmp;
    uint8_t auc_reserve[2];
} hi1103_pa_ical_val_stru;

typedef struct {
    uint16_t us_txdc_cmp_i;
    uint16_t us_txdc_cmp_q;
} hi1103_txdc_comp_val_stru;

typedef struct {
    uint8_t uc_ppf_val;
    uint8_t auc_reserve[3];
} hi1103_ppf_comp_val_stru;

typedef struct {
    uint16_t us_txiq_cmp_p;
    uint16_t us_txiq_cmp_e;
} hi1103_txiq_comp_val_stru;

typedef struct {
    int32_t l_txiq_cmp_alpha_p;
    int32_t l_txiq_cmp_beta_p;
    int32_t l_txiq_cmp_alpha_n;
    int32_t l_txiq_cmp_beta_n;
} hi1103_new_txiq_comp_val_stru;

typedef struct {
    int32_t l_txiq_cmp_alpha;
    int32_t l_txiq_cmp_beta;
    int16_t as_txiq_comp_ls_filt[HI1103_CALI_TXIQ_LS_FILTER_TAP_NUM];
    uint8_t auc_resev[2];
} hi1103_new_txiq_time_comp_val_stru;

typedef struct {
#ifndef _PRE_WLAN_110X_PILOT
    int32_t l_rxiq_cmp_u1;
    int32_t l_rxiq_cmp_u2;
#endif
    int32_t l_rxiq_cmp_alpha;
    int32_t l_rxiq_cmp_beta;
    int16_t as_rxiq_comp_ls_filt[HI1103_CALI_RXIQ_LS_FILTER_TAP_NUM_160M];
    int16_t as_rxiq_bw80M_siso_comp_ls_filt[HI1103_CALI_RXIQ_LS_FILTER_TAP_NUM_160M];
} hi1103_new_rxiq_comp_val_stru;

typedef struct {
    /* ��2ΪIQ��·���� */
    uint8_t uc_cmp_cali[HI1103_CALI_ADC_CH_NUM * 2];
    uint32_t ul_cap_cali_w1[HI1103_CALI_ADC_CH_NUM * 2];
    uint32_t ul_cap_cali_w2[HI1103_CALI_ADC_CH_NUM * 2];
    uint32_t ul_cap_cali_w3[HI1103_CALI_ADC_CH_NUM * 2];
    uint32_t ul_cap_cali_w4[HI1103_CALI_ADC_CH_NUM * 2];
    uint32_t ul_cap_cali_w5[HI1103_CALI_ADC_CH_NUM * 2];
    int32_t l_moec[HI1103_CALI_ADC_CH_NUM * 2];
    uint16_t us_mgec[HI1103_CALI_ADC_CH_NUM * 2];
} hi1103_adc_cali_param_stru;

typedef struct {
    hi1103_rx_dc_comp_val_stru st_cali_rx_dc_cmp[HI1103_CALI_2G_OTHER_CHANNEL_NUM];
    hi1103_rx_gain_comp_val_stru st_cali_rx_gain_cmp[HI1103_CALI_2G_OTHER_CHANNEL_NUM];
    hi1103_logen_comp_val_stru st_cali_logen_cmp[HI1103_2G_CHANNEL_NUM];
    hi1103_2G_tx_power_comp_val_stru st_cali_tx_power_cmp_2G[HI1103_2G_CHANNEL_NUM];
    hi1103_txdc_comp_val_stru ast_txdc_cmp_val[HI1103_2G_CHANNEL_NUM][HI1103_CALI_TXDC_GAIN_LVL_NUM];
#ifdef _PRE_WLAN_NEW_IQ
#ifndef _PRE_WLAN_110X_PILOT
    hi1103_new_txiq_comp_val_stru ast_new_txiq_cmp_val[HI1103_2G_CHANNEL_NUM][HI1103_CALI_IQ_TONE_NUM];
#endif
    hi1103_new_txiq_time_comp_val_stru ast_new_txiq_time_cmp_val[HI1103_2G_CHANNEL_NUM];
    hi1103_new_rxiq_comp_val_stru ast_new_rxiq_cmp_val[HI1103_2G_CHANNEL_NUM];
#endif
} hi1103_2Gcali_param_stru;

typedef struct {
    hi1103_rx_dc_comp_val_stru st_cali_rx_dc_cmp[HI1103_CALI_2G_OTHER_CHANNEL_NUM];
    hi1103_txdc_comp_val_stru ast_txdc_cmp_val[HI1103_2G_CHANNEL_NUM][HI1103_CALI_TXDC_GAIN_LVL_NUM];
#ifdef _PRE_WLAN_NEW_IQ
#ifndef _PRE_WLAN_110X_PILOT
    hi1103_new_txiq_comp_val_stru ast_new_txiq_cmp_val[HI1103_2G_CHANNEL_NUM][HI1103_CALI_IQ_TONE_NUM];
#endif
    hi1103_new_txiq_time_comp_val_stru ast_new_txiq_time_cmp_val[HI1103_2G_CHANNEL_NUM];
    hi1103_new_rxiq_comp_val_stru ast_new_rxiq_cmp_val[HI1103_2G_CHANNEL_NUM];
#endif
} hi1103_2G_dbdc_cali_param_stru;

typedef struct {
    hi1103_rx_dc_comp_val_stru st_cali_rx_dc_cmp;
    hi1103_rx_gain_comp_val_stru st_cali_rx_gain_cmp;
    hi1103_logen_comp_val_stru st_cali_logen_cmp;
    hi1103_5G_tx_power_comp_val_stru st_cali_tx_power_cmp_5G;
    hi1103_ppf_comp_val_stru st_ppf_cmp_val;
    hi1103_txdc_comp_val_stru ast_txdc_cmp_val[HI1103_CALI_TXDC_GAIN_LVL_NUM];
#ifdef _PRE_WLAN_NEW_IQ
#ifndef _PRE_WLAN_110X_PILOT
    hi1103_new_txiq_comp_val_stru ast_new_txiq_cmp_val[HI1103_CALI_IQ_TONE_NUM];
#endif
    hi1103_new_txiq_time_comp_val_stru ast_new_txiq_time_cmp_val[HI1103_CALI_CHAIN_NUM_BUTT];
    hi1103_new_rxiq_comp_val_stru st_new_rxiq_cmp_val;
#endif
} hi1103_5Gcali_param_stru;

typedef struct {
    hi1103_2Gcali_param_stru st_2Gcali_param[HI1103_2G_CHANNEL_NUM];
    hi1103_5Gcali_param_stru st_5Gcali_param[HI1103_5G_CHANNEL_NUM];
} hi1103_wifi_cali_param_stru;

typedef struct {
    uint16_t ul_cali_time;
    uint16_t bit_temperature : 3,
               uc_5g_chan_idx1 : 5,
               uc_5g_chan_idx2 : 5,
               bit_rev : 3;
} hi1103_update_cali_channel_stru;

typedef struct {
    uint8_t uc_rc_cmp_code;
    uint8_t uc_r_cmp_code;
    /* ��Ҫ: MPW2��PILOT RF����, mpw2����cУ׼ֵ; pilot����800M rejection����code������C code����õ� */
    uint8_t uc_c_cmp_code;
    uint8_t uc_20M_rc_cmp_code;
} hi1103_rc_r_c_cali_param_stru;

#ifdef _PRE_WLAN_ONLINE_DPD
typedef struct {
    uint32_t aul_dpd_even_lut[HI1103_DPD_OFFLINE_CALI_LUT_NUM][HI1103_DPD_CALI_LUT_LENGTH];
    uint32_t aul_dpd_odd_lut[HI1103_DPD_OFFLINE_CALI_LUT_NUM][HI1103_DPD_CALI_LUT_LENGTH];
} hi1103_dpd_cali_lut_stru;
#endif

typedef struct {
    uint32_t ul_dog_tag;
    hi1103_2Gcali_param_stru ast_2Gcali_param;
    hi1103_5Gcali_param_stru ast_5Gcali_param[HI1103_5G_CHANNEL_NUM];
#ifdef _PRE_WLAN_ONLINE_DPD
    hi1103_dpd_cali_lut_stru st_dpd_cali_data;
#endif
    hi1103_ppf_comp_val_stru st_165chan_ppf_comp;
    hi1103_update_cali_channel_stru st_cali_update_info;
    uint32_t ul_check_hw_status;
    hi1103_pa_ical_val_stru st_pa_ical_cmp;
    hi1103_rc_r_c_cali_param_stru st_rc_r_c_cali_data;
    hi1103_2G_dbdc_cali_param_stru ast_2g_dbdc_cali_param;
    int16_t s_2g_idet_gm;
    oal_bool_enum_uint8 en_use_lpf_10M_for_2grx_20m;
    oal_bool_enum_uint8 en_save_all;
    uint8_t uc_last_cali_fail_status;
    oal_bool_enum_uint8 en_need_close_fem_cali;
    uint8_t auc_resv[2];
    hi1103_adc_cali_param_stru st_adc_cali_data;
} hi1103_cali_param_stru;

// added for bt 20dbm cali
typedef struct {
    uint8_t uc_atx_pwr_cmp;
    uint8_t uc_dtx_pwr_cmp;
    int16_t s_2g_tx_power_dc;
} hi1103_bt20dbm_txpwr_cal_stru;
typedef struct {
    int32_t l_txiq_cmp_alpha;
    int32_t l_txiq_cmp_beta;
    hi1103_bt20dbm_txpwr_cal_stru st_20dbmtxpwr;
    hi1103_txdc_comp_val_stru ast_txdc_cmp_val[8];  // wifi siso tx��λ
} hi1103_bt20dbm_txcal_val_stru;
typedef struct {
    hi1103_bt20dbm_txcal_val_stru st_bt20dbm_txcali_param[HI1103_2G_CHANNEL_NUM];
} hi1103_bt20dbm_txcal_param_stru;

typedef struct {
    uint16_t us_atx_pwr_cmp;
    int16_t s_2g_tx_power_dc;
    uint8_t uc_dtx_pwr_cmp;
    uint8_t auc_rsv[3];
} hi1105_bt20dbm_txpwr_cal_stru;
typedef struct {
    int32_t l_txiq_cmp_alpha[HI1105_CALI_TXIQ_GAIN_LVL_NUM];
    int32_t l_txiq_cmp_beta[HI1105_CALI_TXIQ_GAIN_LVL_NUM];
    hi1105_bt20dbm_txpwr_cal_stru st_20dbmtxpwr;
    hi1105_txdc_comp_val_stru st_txdc_cmp_val;
} hi1105_bt20dbm_txcal_val_stru;
typedef struct {
    hi1105_bt20dbm_txcal_val_stru st_bt20dbm_txcali_param[HI1103_2G_CHANNEL_NUM];
} hi1105_bt20dbm_txcal_param_stru;

typedef struct {
    uint16_t us_atx_pwr_cmp;
    int16_t s_2g_tx_power_dc;
    uint8_t uc_dtx_pwr_cmp;
    uint8_t rsv1;
    uint16_t rsv2;
} hi1106_bt20dbm_txpwr_cal_stru;
typedef struct {
    hi1106_bt20dbm_txpwr_cal_stru st_20dbmtxpwr;
    hi1106_txdc_comp_val_stru st_txdc_cmp_val;
} hi1106_bt20dbm_txcal_val_stru;
typedef struct {
    hi1106_bt20dbm_txcal_val_stru st_bt20dbm_txcali_param[HI1103_2G_CHANNEL_NUM];
} hi1106_bt20dbm_txcal_param_stru;
// end added of 20dbm cali
uint32_t hmac_save_cali_event(frw_event_mem_stru *pst_event_mem);
uint32_t hmac_send_cali_data(mac_vap_stru *mac_vap);
uint32_t hmac_send_cali_matrix_data(mac_vap_stru *mac_vap);
void hmac_get_cali_data_for_bt20dbm(void);
void hmac_get_1105_cali_data_for_bt20dbm(void);
void hmac_get_1106_cali_data_for_bt20dbm(void);
void hmac_set_close_fem_cali_status(oal_bool_enum_uint8 en_switch);
/* У׼����оƬ�ص����� */
uint32_t hmac_send_cali_matrix_data_1103(mac_vap_stru *mac_vap);
uint32_t hmac_send_1105_cali_matrix_data(mac_vap_stru *mac_vap);
uint32_t hmac_send_1106_cali_matrix_data(mac_vap_stru *mac_vap);
uint32_t hmac_send_cali_data_1103(mac_vap_stru *mac_vap);
uint32_t hmac_send_cali_data_1105(mac_vap_stru *mac_vap);
uint32_t hmac_send_cali_data_1106(mac_vap_stru *mac_vap);
uint32_t hmac_save_cali_event_1103(frw_event_mem_stru *event_mem);
uint32_t hmac_save_cali_event_1105(frw_event_mem_stru *event_mem);
uint32_t hmac_save_cali_event_1106(frw_event_mem_stru *event_mem);

int32_t hmac_cali_ops_init(void);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hmac_mgmt_classifier.h */


