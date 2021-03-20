

#ifndef __WLAN_CALI_1105_H__
#define __WLAN_CALI_1105_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 1 ����ͷ�ļ����� */
#include "oal_ext_if.h"

/* 2 �궨�� */
#ifdef _PRE_WLAN_ONLINE_DPD
#define HI1105_2G_DPD_CALI_CHANNEL_NUM_20M 4
#define HI1105_2G_DPD_CALI_CHANNEL_NUM_40M 3
#define HI1105_2G_DPD_CALI_CHANNEL_NUM     (HI1105_2G_DPD_CALI_CHANNEL_NUM_20M + HI1105_2G_DPD_CALI_CHANNEL_NUM_40M)
#define HI1105_DPD_CALI_TPC_LEVEL_NUM      3
#define HI1105_DPD_CALI_LUT_LENGTH         31
#define HI1105_DPD_OFFLINE_CALI_LUT_NUM    (HI1105_2G_DPD_CALI_CHANNEL_NUM * HI1105_DPD_CALI_TPC_LEVEL_NUM)
#endif

#define HI1105_2G_CHAN_NUM      (3)
#define HI1105_5G_20M_CHAN_NUM  (7)
#define HI1105_5G_80M_CHANNEL_NUM  (7)
#define HI1105_5G_160M_CHANNEL_NUM (2)
#define HI1105_5G_CHANNEL_NUM      (HI1105_5G_20M_CHAN_NUM + HI1105_5G_80M_CHANNEL_NUM + HI1105_5G_160M_CHANNEL_NUM)
#define HI1105_CALI_IQ_TONE_NUM    (16)

/* ������岹��ֵ������������޸� */
#define HI1105_CALI_RXDC_GAIN_LVL_NUM (8)       /* rx dc����ֵ��λ��Ŀ */

#ifdef _PRE_WLAN_1103_ATE
#define HI1105_TXDC_LPF_DAC_GAIN_LVL_NUM   1
#define HI1105_TXDC_MIXBUF_GAIN_LVL_NUM    1
#define HI1105_2G_TXDC_MIXBUF_GAIN_LVL_NUM 1
#define HI1105_5G_TXDC_MIXBUF_GAIN_LVL_NUM 1
#define HI1105_CALI_TXIQ_GAIN_LVL_NUM 1
#define HI1105_CALI_RXIQ_GAIN_LVL_NUM 1
#define HI1105_2G_TXIQ_GAIN_LVL_NUM   1
#define HI1105_5G_TXIQ_GAIN_LVL_NUM   1
#else
#define HI1105_TXDC_LPF_DAC_GAIN_LVL_NUM   8 /* tx dc����ֵlpf/dac��λ��Ŀ */
#define HI1105_TXDC_MIXBUF_GAIN_LVL_NUM    4 /* tx dc����ֵmixbuf��λ��Ŀ */
#define HI1105_2G_TXDC_MIXBUF_GAIN_LVL_NUM 1 /*  2G mixbuf���ú�TPCһ��(����0��),������λ����У׼ */
#define HI1105_5G_TXDC_MIXBUF_GAIN_LVL_NUM 2 /*  5G mixbuf���ú�TPCһ��(����0/1��),������λ����У׼ */
#define HI1105_CALI_TXIQ_GAIN_LVL_NUM 4      /* tx iq����ֵ��λ��Ŀ */
#define HI1105_CALI_RXIQ_GAIN_LVL_NUM 3      /* rx iq����ֵ��λ��Ŀ */
#define HI1105_2G_TXIQ_GAIN_LVL_NUM   4      /*  2G iq ��λ���ú�TPC����һ��(����0/1/2��)��ֻ��lpf��λ����У׼  */
#define HI1105_5G_TXIQ_GAIN_LVL_NUM   3      /*  5G iq ��λ���ú�TPC����һ��(����0/1/2��)����lpf,mixbuf��λ����У׼ */
#endif

#define HI1105_IQ_CALI_TIME_NUM   4         /*  Ϊ��ʡУ׼ʱ��, ����WIFI IQУ׼��ͬ��λ���ֶ��У׼ */

/* оƬ���üĴ��������겻���޸� */
/* C0/C1 TXDC�����Ĵ�����64�� = 2(MIMO/SISO) * 8(LPF_DAC_GAIN_LVL_NUM) * 4(MIXBUF_GAIN_LVL_NUM) */
/* C0/C1 TXIQ�����Ĵ�����56�� = 2(MIMO/SISO) * 7(LS_FIR_NUM) * 4(COMP_GAIN_LVL_NUM) */
/* C0/C1 RXIQ�����Ĵ�����42�� = 2(MIMO/SISO) * 7(LS_FIR_NUM) * 3(COMP_GAIN_LVL_NUM) */
#define HI1105_PHY_TXDC_COMP_LPF_DAC_GAIN_LVL_NUM   8   /* tx dc�����Ĵ���lpf/dac��λ�� */
#define HI1105_PHY_TXDC_COMP_MIXBUF_GAIN_LVL_NUM    4   /* tx dc�����Ĵ���mixbuf��λ�� */
#define HI1105_TXIQ_LSFIR_NUM   7
#define HI1105_PHY_TXIQ_COMP_GAIN_LVL_NUM 4             /* tx iq�����Ĵ�����λ�� */
#define HI1105_RXIQ_LSFIR_NUM   7
#define HI1105_PHY_RXIQ_COMP_GAIN_LVL_NUM 3             /* rx iq�����Ĵ�����λ�� */

#define HI1105_CALI_ADC_CH_NUM 4 /* 4·ADC */

/* У׼�ŵ��������� */
#define HI1105_CALI_2G_OTHER_CHANNEL_NUM 1 /* g_ast_2g_other_cali_channelУ׼�ŵ��� */
#define HI1105_CALI_5G_OTHER_CHANNEL_NUM 1 /* g_ast_5g_other_cali_channelУ׼�ŵ��� */

#define HI1105_CALI_IQ_LS_FILTER_TAP_NUM 7

#define HI1105_RXIQ_LSFIR_FEQ_NUM_320M     64
#define HI1105_RXIQ_LSFIR_FEQ_NUM_160M     32
#define HI1105_RXIQ_LSFIR_FEQ_NUM_80M      16
#define HI1105_RXIQ_LSFIR_FEQ_NUM_40M      16
#define HI1105_RXIQ_LSFIR_FEQ_NUM_80M_FPGA 32

#define HI1105_TXIQ_LSFIR_FEQ_NUM_640M      128
#define HI1105_TXIQ_LSFIR_FEQ_NUM_320M      64
#define HI1105_TXIQ_LSFIR_FEQ_NUM_160M      32
#define HI1105_TXIQ_LSFIR_FEQ_NUM_80M       32
#define HI1105_TXIQ_LSFIR_FEQ_NUM_160M_FPGA 64

/* 3 ö�ٶ��� */
typedef enum {
    HI1105_CALI_SISO,
    HI1105_CALI_MIMO,

    HI1105_CALI_CHAIN_NUM_BUTT,
} hi1105_rf_cali_chain_num_enum;
typedef uint8_t hi1105_rf_cali_chain_num_enum_uint8;
/* 4 ȫ�ֱ������� */
/* 5 ��Ϣͷ���� */
/* 6 ��Ϣ���� */
/* 7 STRUCT���� */
/* �����ṹ */
typedef struct {
    int32_t l_real;
    int32_t l_imag;
} hi1105_complex_stru;

/* ADCУ׼�Ĵ���ֵ
1. CMP
    0x400A0F08  RO C0_I_SUB0 [7:2]   C0_I_SUB1 [23:18]
    0x400A0F18  RO C0_I_SUB2 [7:2]   C0_I_SUB3 [23:18]
    0x400A0F38  RO C0_Q_SUB0 [7:2]   C0_Q_SUB1 [23:18]
    0x400A0F48  RO C0_Q_SUB2 [7:2]   C0_Q_SUB3 [23:18]

    0x400A0F68  RO C1_I_SUB0 [7:2]   C1_I_SUB1 [23:18]
    0x400A0F78  RO C1_I_SUB2 [7:2]   C1_I_SUB3 [23:18]
    0x400A0F98  RO C1_Q_SUB0 [7:2]   C1_Q_SUB1 [23:18]
    0x400A0FA8  RO C1_Q_SUB2 [7:2]   C1_Q_SUB3 [23:18]
2. CAP
    0x400A0280~0x400A029C  ch0_sub0_w1~ch0_sub7_w1 [16:0]
    0x400A02A0~0x400A02BC  ch1_sub0_w1~ch1_sub7_w1 [16:0]

    0x400A02C0~0x400A02DC  ch0_sub0_w2~ch0_sub7_w2 [16:0]
    0x400A02E0~0x400A02FC  ch1_sub0_w2~ch1_sub7_w2 [16:0]

    0x400A0300~0x400A031C  ch0_sub0_w3~ch0_sub7_w3 [16:0]
    0x400A0320~0x400A033C  ch1_sub0_w3~ch1_sub7_w3 [16:0]

    0x400A0340~0x400A035C  ch0_sub0_w4~ch0_sub7_w4 [16:0]
    0x400A0360~0x400A037C  ch1_sub0_w4~ch1_sub7_w4 [16:0]

    0x400A0380~0x400A039C  ch0_sub0_w5~ch0_sub7_w5 [16:0]
    0x400A03A0~0x400A03BC  ch1_sub0_w5~ch1_sub7_w5 [16:0]
3. INTR_GAIN
    0x400A0580~0x400A059C  INTR_GAIN_CAL_GAIN0~INTR_GAIN_CAL_GAIN7   [31:20]:1/Gainֵ [18:0]�����Gainֵ�ϱ�,�޷������ݣ�����λ11��7λС��λ

    0x400A05A0~0x400A05BC  INTR_GAIN_CAL_GAIN10~INTR_GAIN_CAL_GAIN17 [31:20]:1/Gainֵ [18:0]�����Gainֵ�ϱ�,�޷������ݣ�����λ11��7λС��λ
4. MOEC
    0x400A0640~0x400A065C  CH0_I/Q_moec_0~3 [16:0]

    0x400A0660~0x400A067C  CH1_I/Q_moec_0~3 [16:0]
5. MGEC
    0x400A07A0  MGEC_GAIN_CAL_C0_I_1 [16:0]  MGEC_GAIN_CAL_C0_I_2 [31:16]
    0x400A07A4  MGEC_GAIN_CAL_C0_Q_1 [16:0]  MGEC_GAIN_CAL_C0_Q_2 [31:16]
    0x400A07A8  MGEC_GAIN_CAL_C1_I_1 [16:0]  MGEC_GAIN_CAL_C1_I_2 [31:16]
    0x400A07AC  MGEC_GAIN_CAL_C1_Q_1 [16:0]  MGEC_GAIN_CAL_C1_Q_2 [31:16]

    0x400A07C0  MGEC_GAIN_CAL_C0_I_3 [16:0]  MGEC_GAIN_CAL_C0_I_Reserved [31:16]
    0x400A07C4  MGEC_GAIN_CAL_C0_Q_3 [16:0]  MGEC_GAIN_CAL_C0_Q_Reserved [31:16]
    0x400A07C8  MGEC_GAIN_CAL_C1_I_3 [16:0]  MGEC_GAIN_CAL_C1_I_Reserved [31:16]
    0x400A07CC  MGEC_GAIN_CAL_C1_Q_3 [16:0]  MGEC_GAIN_CAL_C1_Q_Reserved [31:16]
*/
typedef struct {
    /* ��2ΪIQ��·���� */
    uint8_t uc_cmp_cali[HI1105_CALI_ADC_CH_NUM * 2];
    uint32_t ul_cap_cali_w1[HI1105_CALI_ADC_CH_NUM * 2];
    uint32_t ul_cap_cali_w2[HI1105_CALI_ADC_CH_NUM * 2];
    uint32_t ul_cap_cali_w3[HI1105_CALI_ADC_CH_NUM * 2];
    uint32_t ul_cap_cali_w4[HI1105_CALI_ADC_CH_NUM * 2];
    uint32_t ul_cap_cali_w5[HI1105_CALI_ADC_CH_NUM * 2];
    int32_t l_moec[HI1105_CALI_ADC_CH_NUM * 2];
    uint16_t us_mgec[HI1105_CALI_ADC_CH_NUM * 2];
} hi1105_adc_cali_param_stru;

typedef struct {
    uint8_t uc_rx_gain_cmp_code; /* ��pilot RFʹ�ã�CУ׼����ֵ */

    /* LODIV ��ʱ��rx gain���ýṹ */
    uint8_t uc_rx_mimo_cmp;
    uint8_t uc_dpd_siso_cmp;
    uint8_t uc_dpd_mimo_cmp;
} hi1105_rx_gain_comp_val_stru;

typedef struct {
    uint16_t aus_analog_rxdc_siso_cmp[HI1105_CALI_RXDC_GAIN_LVL_NUM];
    uint16_t aus_analog_rxdc_mimo_extlna_cmp[HI1105_CALI_RXDC_GAIN_LVL_NUM];
    uint16_t aus_analog_rxdc_mimo_intlna_cmp[HI1105_CALI_RXDC_GAIN_LVL_NUM];
    uint16_t us_digital_rxdc_cmp_i;
    uint16_t us_digital_rxdc_cmp_q;
    int16_t s_cali_temperature;
    int16_t s_mimo_cali_temperature;
} hi1105_rx_dc_comp_val_stru;

typedef struct {
    uint16_t us_ppa_cmp;
    uint16_t us_atx_pwr_cmp;
    int16_t s_2g_tx_ppa_dc;
    int16_t s_2g_tx_power_dc;
    int8_t c_dp_init;
    uint8_t uc_dtx_pwr_cmp;
    uint8_t auc_rsv[2];
} hi1105_2G_tx_power_comp_val_stru;

typedef struct {
    uint16_t us_ppa_cmp;
    int16_t s_5g_tx_power_dc;
    uint16_t us_atx_pwr_cmp;
    uint8_t uc_dtx_pwr_cmp;
    uint8_t uc_mx_cmp;
} hi1105_5G_tx_power_comp_val_stru;

typedef struct {
    uint8_t uc_ssb_cmp;
    uint8_t uc_buf0_cmp;
    uint8_t uc_buf1_cmp;
    uint8_t uc_resv;
} hi1105_logen_comp_val_stru;

typedef struct {
    uint8_t uc_classa_cmp;
    uint8_t uc_classb_cmp;
    uint8_t auc_reserve[2];
} hi1105_pa_ical_val_stru;

typedef struct {
    uint16_t aus_txdc_cmp_i[HI1105_TXDC_LPF_DAC_GAIN_LVL_NUM];
    uint16_t aus_txdc_cmp_q[HI1105_TXDC_LPF_DAC_GAIN_LVL_NUM];
} hi1105_txdc_comp_val_stru;

typedef struct {
    uint8_t uc_ppf_val;
    uint8_t uc_logen_ssb_ppf;
    uint8_t auc_reserve[2];
} hi1105_ppf_comp_val_stru;

typedef struct {
    int32_t l_txiq_cmp_alpha[HI1105_CALI_TXIQ_GAIN_LVL_NUM];
    int32_t l_txiq_cmp_beta[HI1105_CALI_TXIQ_GAIN_LVL_NUM];
    int16_t as_txiq_comp_ls_fir_i[HI1105_CALI_TXIQ_GAIN_LVL_NUM][HI1105_TXIQ_LSFIR_NUM];
    int16_t as_txiq_comp_ls_fir_q[HI1105_CALI_TXIQ_GAIN_LVL_NUM][HI1105_TXIQ_LSFIR_NUM];
} hi1105_new_txiq_comp_stru;

typedef struct {
    int32_t l_rxiq_cmp_alpha[HI1105_CALI_RXIQ_GAIN_LVL_NUM];
    int32_t l_rxiq_cmp_beta[HI1105_CALI_RXIQ_GAIN_LVL_NUM];
    int16_t as_rxiq_comp_ls_fir_i[HI1105_CALI_RXIQ_GAIN_LVL_NUM][HI1105_RXIQ_LSFIR_NUM];
    int16_t as_rxiq_comp_ls_fir_q[HI1105_CALI_RXIQ_GAIN_LVL_NUM][HI1105_RXIQ_LSFIR_NUM];
} hi1105_new_rxiq_comp_stru;

typedef struct {
    hi1105_rx_dc_comp_val_stru ast_cali_rx_dc_cmp[HI1105_CALI_2G_OTHER_CHANNEL_NUM];
    hi1105_rx_gain_comp_val_stru ast_cali_rx_gain_cmp[HI1105_CALI_2G_OTHER_CHANNEL_NUM];
    hi1105_logen_comp_val_stru ast_cali_logen_cmp[HI1105_2G_CHAN_NUM];
    hi1105_2G_tx_power_comp_val_stru ast_cali_tx_power_cmp_2G[HI1105_2G_CHAN_NUM];
    hi1105_txdc_comp_val_stru ast_txdc_cmp_val[HI1105_2G_CHAN_NUM];
#ifdef _PRE_WLAN_NEW_IQ
    hi1105_new_txiq_comp_stru ast_new_txiq_cmp[HI1105_2G_CHAN_NUM][HI1105_CALI_CHAIN_NUM_BUTT];
    hi1105_new_rxiq_comp_stru ast_new_rxiq_cmp[HI1105_2G_CHAN_NUM][HI1105_CALI_CHAIN_NUM_BUTT];
#endif
} hi1105_2Gcali_param_stru;

typedef struct {
    hi1105_rx_dc_comp_val_stru ast_cali_rx_dc_cmp[HI1105_CALI_2G_OTHER_CHANNEL_NUM];
    hi1105_txdc_comp_val_stru ast_txdc_cmp_val[HI1105_2G_CHAN_NUM];
#ifdef _PRE_WLAN_NEW_IQ
    hi1105_new_txiq_comp_stru ast_new_txiq_cmp[HI1105_2G_CHAN_NUM][HI1105_CALI_CHAIN_NUM_BUTT];
    hi1105_new_rxiq_comp_stru ast_new_rxiq_cmp[HI1105_2G_CHAN_NUM][HI1105_CALI_CHAIN_NUM_BUTT];
#endif
} hi1105_2G_dbdc_cali_param_stru;

typedef struct {
    hi1105_rx_dc_comp_val_stru st_cali_rx_dc_cmp;
    hi1105_rx_gain_comp_val_stru st_cali_rx_gain_cmp;
    hi1105_logen_comp_val_stru st_cali_logen_cmp;
    hi1105_5G_tx_power_comp_val_stru st_cali_tx_power_cmp_5G;
    hi1105_ppf_comp_val_stru st_ppf_cmp_val;
    hi1105_txdc_comp_val_stru ast_txdc_cmp_val[HI1105_TXDC_MIXBUF_GAIN_LVL_NUM];
#ifdef _PRE_WLAN_NEW_IQ
    hi1105_new_txiq_comp_stru ast_new_txiq_cmp[HI1105_CALI_CHAIN_NUM_BUTT];
    hi1105_new_rxiq_comp_stru ast_new_rxiq_cmp[HI1105_CALI_CHAIN_NUM_BUTT];
#endif
} hi1105_5Gcali_param_stru;

typedef struct {
    hi1105_2Gcali_param_stru ast_2Gcali_param[HI1105_2G_CHAN_NUM];
    hi1105_5Gcali_param_stru ast_5Gcali_param[HI1105_5G_CHANNEL_NUM];
} hi1105_wifi_cali_param_stru;

typedef struct {
    uint16_t us_cali_time;
    uint16_t bit_temperature : 3,
               uc_5g_chan_idx1 : 5,
               uc_5g_chan_idx2 : 5,
               bit_rev : 3;
} hi1105_update_cali_channel_stru;

typedef struct {
    uint8_t uc_rc_cmp_code;
    uint8_t uc_r_cmp_code; /* ����PMU��ԭʼ5bit R code */
    /* ��Ҫ: MPW2��PILOT RF����, mpw2����cУ׼ֵ; pilot����800M rejection����code������C code����õ� */
    uint8_t uc_c_cmp_code;
    uint8_t uc_20M_rc_cmp_code;
} hi1105_rc_r_c_cali_param_stru;

#ifdef _PRE_WLAN_ONLINE_DPD
typedef struct {
    uint32_t aul_dpd_even_lut[HI1105_DPD_OFFLINE_CALI_LUT_NUM][HI1105_DPD_CALI_LUT_LENGTH];
    uint32_t aul_dpd_odd_lut[HI1105_DPD_OFFLINE_CALI_LUT_NUM][HI1105_DPD_CALI_LUT_LENGTH];

} hi1105_dpd_cali_lut_stru;
#endif

struct hi1105_cali_param_tag {
    uint32_t ul_dog_tag;
    hi1105_2Gcali_param_stru st_2Gcali_param;
    hi1105_5Gcali_param_stru ast_5Gcali_param[HI1105_5G_CHANNEL_NUM];
#ifdef _PRE_WLAN_NEW_IQ
    hi1105_new_rxiq_comp_stru ast_20M_rxiq_cmp[HI1105_2G_CHAN_NUM + HI1105_5G_20M_CHAN_NUM][HI1105_CALI_CHAIN_NUM_BUTT];
    hi1105_new_rxiq_comp_stru ast_20M_rxiq_dbdc_cmp[HI1105_2G_CHAN_NUM][HI1105_CALI_CHAIN_NUM_BUTT];
#endif
#ifdef _PRE_WLAN_ONLINE_DPD
    hi1105_dpd_cali_lut_stru st_dpd_cali_data;
#endif
    hi1105_ppf_comp_val_stru st_165chan_ppf_comp;
    hi1105_update_cali_channel_stru st_cali_update_info;
    uint32_t ul_check_hw_status;
    hi1105_pa_ical_val_stru st_pa_ical_cmp;
    hi1105_rc_r_c_cali_param_stru st_rc_r_c_cali_data;
    hi1105_2G_dbdc_cali_param_stru st_2g_dbdc_cali_param;
    int16_t s_2g_idet_gm;
    oal_bool_enum_uint8 en_use_lpf_10M_for_2grx_20m;
    oal_bool_enum_uint8 en_save_all;
    uint8_t uc_last_cali_fail_status;
    uint8_t uc_vdet_dc_comp;
    uint8_t uc_cur_cali_policy;
    oal_bool_enum_uint8 en_need_close_fem_cali;
    hi1105_adc_cali_param_stru st_adc_cali_data;
    uint8_t rx_en_invalid;
    uint8_t rsv[3]; /* �����ֶ�:3�ֽ� */
};

typedef struct hi1105_cali_param_tag hi1105_cali_param_stru;

/* 8 UNION���� */
/* 9 OTHERS���� */
/* 10 �������� */
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hal_cali.h */
