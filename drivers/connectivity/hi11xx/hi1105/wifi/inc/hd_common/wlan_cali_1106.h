

#ifndef __WLAN_CALI_1106_H__
#define __WLAN_CALI_1106_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 1 其他头文件包含 */
#include "oal_ext_if.h"

/* 2 宏定义 */
#define HI1106_2G_CHAN_NUM      3
#define HI1106_5G_20M_CHAN_NUM  7
#define HI1106_5G_80M_CHANNEL_NUM  7
#define HI1106_5G_160M_CHANNEL_NUM 2
#define HI1106_5G_CHANNEL_NUM      (HI1106_5G_20M_CHAN_NUM + HI1106_5G_80M_CHANNEL_NUM + HI1106_5G_160M_CHANNEL_NUM)
#define HI1106_CALI_IQ_TONE_NUM    16
#define HI1105_2G_DPD_CALI_CHANNEL_NUM_20M 4
#define HI1105_2G_DPD_CALI_CHANNEL_NUM_40M 3
#if (_PRE_WLAN_CHIP_ASIC == _PRE_WLAN_CHIP_VERSION)
#define HI1106_DPD_TPC_LEVEL_NUM        4
#else
#define HI1106_DPD_TPC_LEVEL_NUM        1
#endif
#define HI1106_DPD_SIDE_NUM_FULL_BW     2
#define HI1106_5G_DPD_SIDE_CHANNEL_NUM  (HI1106_5G_80M_CHANNEL_NUM + HI1106_5G_160M_CHANNEL_NUM)
#define HI1106_DPD_EVEN_LUT_LENGTH      48
#define HI1106_DPD_ODD_LUT_LENGTH       48
#define HI1106_DPD_GLUE_LUT_LENGTH      43
#define HI1106_DPD_2G_LUT_NUM           (HI1106_2G_CHAN_NUM * HI1106_DPD_TPC_LEVEL_NUM)
#define HI1106_DPD_5G_LUT_NUM           (HI1106_5G_CHANNEL_NUM * HI1106_DPD_TPC_LEVEL_NUM)
#define HI1106_DPD_5G_SIDE_LUT_NUM      (HI1106_5G_DPD_SIDE_CHANNEL_NUM * HI1106_DPD_TPC_LEVEL_NUM * HI1106_DPD_SIDE_NUM_FULL_BW)
#define HI1106_DPD_DEVICE_CHANNEL_NUM_20M          (HI1106_2G_CHAN_NUM + HI1106_5G_20M_CHAN_NUM)
#define HI1106_DPD_DEVICE_CHANNEL_NUM_OTHER        1
#define HI1106_DPD_DEVICE_SIDE_CHANNEL_NUM_OTHER   (HI1106_DPD_DEVICE_CHANNEL_NUM_OTHER * HI1106_DPD_SIDE_NUM_FULL_BW)
#define HI1106_DPD_DEVICE_LUT_NUM_20M              (HI1106_DPD_DEVICE_CHANNEL_NUM_20M * HI1106_DPD_TPC_LEVEL_NUM)
#define HI1106_DPD_DEVICE_LUT_NUM_OTHER            (HI1106_DPD_DEVICE_CHANNEL_NUM_OTHER * HI1106_DPD_TPC_LEVEL_NUM)
#define HI1106_DPD_DEVICE_SIDE_LUT_NUM_OTHER       (HI1106_DPD_DEVICE_SIDE_CHANNEL_NUM_OTHER * HI1106_DPD_TPC_LEVEL_NUM)

/* 软件定义补偿值数组数，宏可修改 */
#define HI1106_CALI_RXDC_GAIN_LVL_NUM 8       /* rx dc补偿值档位数目 */

#ifdef _PRE_WLAN_1103_ATE
#define HI1106_TXDC_LPF_DAC_GAIN_LVL_NUM   1
#define HI1106_TXDC_MIXBUF_GAIN_LVL_NUM    1
#define HI1106_2G_TXDC_MIXBUF_GAIN_LVL_NUM 1
#define HI1106_5G_TXDC_MIXBUF_GAIN_LVL_NUM 1
#define HI1106_CALI_TXIQ_GAIN_LVL_NUM 1
#define HI1106_CALI_RXIQ_GAIN_LVL_NUM 1
#else
#if (_PRE_WLAN_CHIP_ASIC == _PRE_WLAN_CHIP_VERSION)
#define HI1106_TXDC_LPF_DAC_GAIN_LVL_NUM   8 /* tx dc补偿值lpf/dac档位数目 */
#define HI1106_CALI_TXIQ_GAIN_LVL_NUM 4      /* tx iq补偿值档位数目 */
#define HI1106_CALI_RXIQ_GAIN_LVL_NUM 8      /* rx iq补偿值档位数目 */
#else
#define HI1106_TXDC_LPF_DAC_GAIN_LVL_NUM   1 /* tx dc补偿值lpf/dac档位数目 */
#define HI1106_CALI_TXIQ_GAIN_LVL_NUM 1      /* tx iq补偿值档位数目, FPGA只校准1挡 */
#define HI1106_CALI_RXIQ_GAIN_LVL_NUM 1      /* rx iq补偿值档位数目, FPGA只校准1挡 */
#endif
#define HI1106_TXDC_MIXBUF_GAIN_LVL_NUM    4 /* tx dc补偿值mixbuf档位数目 */
#define HI1106_2G_TXDC_MIXBUF_GAIN_LVL_NUM 1 /*  2G mixbuf配置和TPC一致(配置0档),其他档位不做校准 */
#define HI1106_5G_TXDC_MIXBUF_GAIN_LVL_NUM 1 /*  5G mixbuf配置和TPC一致(配置0/1档),其他档位不做校准 */
#endif

#define HI1106_IQ_CALI_TIME_NUM   4         /*  为节省校准时间, 开关WIFI IQ校准不同档位，分多次校准 */

/* 芯片配置寄存器数，宏不可修改 */
/* TRX IQ lut表RAM_SIZE: 260(depth)*32(width) */
#define HI1106_PHY_TXDC_COMP_LPF_DAC_GAIN_LVL_NUM   8   /* tx dc补偿寄存器lpf/dac档位数 */
#define HI1106_PHY_TXDC_COMP_MIXBUF_GAIN_LVL_NUM    4   /* tx dc补偿寄存器mixbuf档位数 */
#define HI1106_PHY_TXIQ_COMP_LVL_NUM                4   /* tx iq补偿寄存器档位数目 */
#define HI1106_PHY_RXIQ_COMP_LVL_NUM                8   /* rx iq补偿寄存器档位数目 */
#define HI1106_CALI_IQ_QMC_QDC_COEF_NUM             10  /* 9阶系数 + 1个DC */

#define HI1106_CALI_IQ_QMC_SISO_COEF_ADDR_OFFSET    0
#define HI1106_CALI_IQ_QDC_SISO_COEF_ADDR_OFFSET    80
#define HI1106_CALI_IQ_QMC_FB_COEF_ADDR_OFFSET      240
#define HI1106_CALI_IQ_QDC_TX_COEF_ADDR_OFFSET      250

#define HI1106_CALI_ADC_CH_NUM 4 /* 4路ADC */

/* 校准信道个数定义 */
#define HI1106_CALI_2G_OTHER_CHANNEL_NUM 1 /* g_ast_2g_other_cali_channel校准信道数 */
#define HI1106_CALI_5G_OTHER_CHANNEL_NUM 1 /* g_ast_5g_other_cali_channel校准信道数 */

#define HI1106_CALI_IQ_LS_FILTER_TAP_NUM 7

#define HI1106_RXIQ_LSFIR_FEQ_NUM_320M     64
#define HI1106_RXIQ_LSFIR_FEQ_NUM_160M     32
#define HI1106_RXIQ_LSFIR_FEQ_NUM_80M      16
#define HI1106_RXIQ_LSFIR_FEQ_NUM_40M      16
#define HI1106_RXIQ_LSFIR_FEQ_NUM_80M_FPGA 32

#define HI1106_TXIQ_LSFIR_FEQ_NUM_640M      128
#define HI1106_TXIQ_LSFIR_FEQ_NUM_320M      64
#define HI1106_TXIQ_LSFIR_FEQ_NUM_160M      32
#define HI1106_TXIQ_LSFIR_FEQ_NUM_80M       32
#define HI1106_TXIQ_LSFIR_FEQ_NUM_160M_FPGA 64

/* rx dc补偿值档位数目 */
#define HAL_CALI_RXDC_GAIN_LVL_NUM      8

#define WLAN_RF_CALI_DFLT_2G_FREQ_BAND_NUM    3     /* 默认2G下，需要校准全部3个freq_band */
#define WLAN_RF_LNA_BLK_2G_FREQ_BAND_NUM      1     /* 2G LNA BLK校准下，只需要校准1个freq_band */

#define WLAN_RF_CALI_DFLT_5G_FREQ_BAND_NUM    7     /* 默认5G下，需要校准全部7个freq_band */
#define WLAN_RF_CALI_160M_5G_FREQ_BAND_NUM    2     /* 默认5G下，160M带宽需要校准2个freq_band */
#define WLAN_RF_LNA_BLK_5G_FREQ_BAND_NUM      1     /* 5G LNA BLK校准下，只需要校准1个freq_band */
#define WLAN_RF_CALI_DFLT_5G_FREQ_BAND_11B    1     /* 默认5G下，11b需要校准1个freq_band */

#if (_PRE_WLAN_CHIP_ASIC == _PRE_WLAN_CHIP_VERSION)
#define WLAN_TX_DCIQ_CALI_LVL                 4
#define WLAN_RX_IQ_CALI_LVL                   8
#else
#define WLAN_TX_DCIQ_CALI_LVL                 1
#define WLAN_RX_IQ_CALI_LVL                   1
#endif

#define WLAN_RX_DC_LNA_LVL                    8

#define WLAN_IQ_QMC_QDC_COEF_NUM      10  /* 9阶系数 + 1个DC */

#define WLAN_DPD_EVEN_LUT_LENGTH      48
#define WLAN_DPD_ODD_LUT_LENGTH       48
#define WLAN_DPD_GLUE_LUT_LENGTH      43
#if (_PRE_WLAN_CHIP_ASIC == _PRE_WLAN_CHIP_VERSION)
#define WLAN_DPD_TPC_LEVEL_NUM        4
#else
#define WLAN_DPD_TPC_LEVEL_NUM        1
#endif

/* 2G/5G RF划分子频段数目 */
#define WLAN_2G_RF_BAND_NUM      3
#define WLAN_5G_RF_BAND_NUM      7

/* 2G/5G 软件定义校准子频段数目 */
#define WLAN_5G_20M_SUB_BAND_NUM  7
#define WLAN_5G_80M_SUB_BAND_NUM  2
#define WLAN_2G_CALI_SUB_BAND_NUM WLAN_2G_RF_BAND_NUM
#define WLAN_5G_CALI_SUB_BAND_NUM (WLAN_5G_20M_SUB_BAND_NUM + WLAN_5G_80M_SUB_BAND_NUM)

#define WLAN_2G_BAND_BOUNDARY_NUM       (WLAN_2G_RF_BAND_NUM - 1)
#define WLAN_5G_BAND_BOUNDARY_NUM       WLAN_5G_RF_BAND_NUM

/* 3 枚举定义 */
typedef enum {
    HI1106_CALI_SISO,
    HI1106_CALI_MIMO,

    HI1106_CALI_CHAIN_NUM_BUTT,
} hi1106_rf_cali_chain_num_enum;
typedef uint8_t hi1106_rf_cali_chain_num_enum_uint8;

/* RF通道枚举 */
typedef enum {
    WLAN_SINGLE_STREAM_0 = 0,  /* C0 */
    WLAN_SINGLE_STREAM_1 = 1,  /* C1 */
    WLAN_SINGLE_STREAM_2 = 2,  /* C2 */
    WLAN_SINGLE_STREAM_3 = 3,  /* C3 */
    WLAN_STREAM_TYPE_BUTT = 4,
} wlan_stream_type_enum;
typedef uint8_t wlan_stream_type_enum_uint8;

/* RF Freq_band 枚举 */
typedef enum {
    WLAN_FREQ_BAND_0 = 1,
    WLAN_FREQ_BAND_1 = 2,
    WLAN_FREQ_BAND_2 = 3,
    WLAN_FREQ_BAND_3 = 4,
    WLAN_FREQ_BAND_4 = 5,
    WLAN_FREQ_BAND_5 = 6,
    WLAN_FREQ_BAND_6 = 7,

    WLAN_FREQ_BAND_IDX_BUTT
} wlan_freq_band_enum;
typedef uint8_t wlan_freq_band_enum_uint8;

/* 校准所需要使用的定制化 */
typedef struct {
    uint32_t cali_mask[2]; /* 校准开关, 2G/5G */
}wlan_cali_ini_custom_stru;

/* 4 全局变量声明 */
/* 5 消息头定义 */
/* 6 消息定义 */
/* 7 STRUCT定义 */
/* 复数结构 */
typedef struct {
    int32_t l_real;
    int32_t l_imag;
} hi1106_complex_stru;

/* ADC校准寄存器值
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
    0x400A0580~0x400A059C  INTR_GAIN_CAL_GAIN0~INTR_GAIN_CAL_GAIN7   [31:20]:1/Gain值 [18:0]计算的Gain值上报,无符号数据，整数位11，7位小数位

    0x400A05A0~0x400A05BC  INTR_GAIN_CAL_GAIN10~INTR_GAIN_CAL_GAIN17 [31:20]:1/Gain值 [18:0]计算的Gain值上报,无符号数据，整数位11，7位小数位
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
    /* 乘2为IQ两路数据 */
    uint8_t uc_cmp_cali[HI1106_CALI_ADC_CH_NUM * 2];
    uint32_t ul_cap_cali_w1[HI1106_CALI_ADC_CH_NUM * 2];
    uint32_t ul_cap_cali_w2[HI1106_CALI_ADC_CH_NUM * 2];
    uint32_t ul_cap_cali_w3[HI1106_CALI_ADC_CH_NUM * 2];
    uint32_t ul_cap_cali_w4[HI1106_CALI_ADC_CH_NUM * 2];
    uint32_t ul_cap_cali_w5[HI1106_CALI_ADC_CH_NUM * 2];
    int32_t l_moec[HI1106_CALI_ADC_CH_NUM * 2];
    uint16_t us_mgec[HI1106_CALI_ADC_CH_NUM * 2];
} hi1106_adc_cali_param_stru;

typedef struct {
    uint8_t uc_rx_gain_cmp_code; /* 仅pilot RF使用，C校准补偿值 */

    /* LODIV 暂时和rx gain复用结构 */
    uint8_t uc_rx_mimo_cmp;
    uint8_t uc_dpd_siso_cmp;
    uint8_t uc_dpd_mimo_cmp;
} hi1106_rx_gain_comp_val_stru;

typedef struct {
    uint16_t aus_analog_rxdc_siso_cmp[HI1106_CALI_RXDC_GAIN_LVL_NUM];
    uint16_t aus_analog_rxdc_mimo_extlna_cmp[HI1106_CALI_RXDC_GAIN_LVL_NUM];
    uint16_t aus_analog_rxdc_mimo_intlna_cmp[HI1106_CALI_RXDC_GAIN_LVL_NUM];
    uint16_t us_digital_rxdc_cmp_i;
    uint16_t us_digital_rxdc_cmp_q;
    int16_t s_cali_temperature;
    int16_t s_mimo_cali_temperature;
} hi1106_rx_dc_comp_val_stru;

typedef struct {
    uint16_t us_ppa_cmp;
    uint16_t us_atx_pwr_cmp;
    int16_t s_2g_tx_ppa_dc;
    int16_t s_2g_tx_power_dc;
    int8_t c_dp_init;
    uint8_t uc_dtx_pwr_cmp;
    uint8_t auc_rsv[2];
} hi1106_2G_tx_power_comp_val_stru;

typedef struct {
    uint16_t us_ppa_cmp;
    int16_t s_5g_tx_power_dc;
    uint16_t us_atx_pwr_cmp;
    uint8_t uc_dtx_pwr_cmp;
    uint8_t uc_mx_cmp;
} hi1106_5G_tx_power_comp_val_stru;

typedef struct {
    uint8_t uc_ssb_cmp;
    uint8_t uc_buf0_cmp;
    uint8_t uc_buf1_cmp;
    uint8_t uc_resv;
} hi1106_logen_comp_val_stru;

typedef struct {
    uint8_t uc_classa_cmp;
    uint8_t uc_classb_cmp;
    uint8_t auc_reserve[2];
} hi1106_pa_ical_val_stru;

typedef struct {
    uint16_t aus_txdc_cmp_i[HI1106_TXDC_LPF_DAC_GAIN_LVL_NUM];
    uint16_t aus_txdc_cmp_q[HI1106_TXDC_LPF_DAC_GAIN_LVL_NUM];
} hi1106_txdc_comp_val_stru;

typedef struct {
    uint8_t uc_ppf_val;
    uint8_t uc_logen_ssb_ppf;
    uint8_t auc_reserve[2];
} hi1106_ppf_comp_val_stru;

typedef struct {
    uint32_t qdc_qmc_coef[HI1106_CALI_IQ_QMC_QDC_COEF_NUM];
} hi1106_qdc_qmc_coef_stru;

typedef struct {
    hi1106_qdc_qmc_coef_stru qmc_siso[HI1106_CALI_TXIQ_GAIN_LVL_NUM];
    hi1106_qdc_qmc_coef_stru qmc_mimo[HI1106_CALI_TXIQ_GAIN_LVL_NUM];
    hi1106_qdc_qmc_coef_stru qmc_fb;
} hi1106_new_txiq_comp_stru;

typedef struct {
    hi1106_qdc_qmc_coef_stru qdc_siso[HI1106_CALI_RXIQ_GAIN_LVL_NUM];
    hi1106_qdc_qmc_coef_stru qdc_mimo[HI1106_CALI_RXIQ_GAIN_LVL_NUM];
    hi1106_qdc_qmc_coef_stru qdc_tx;
} hi1106_new_rxiq_comp_stru;

typedef struct {
    hi1106_rx_dc_comp_val_stru ast_cali_rx_dc_cmp[HI1106_CALI_2G_OTHER_CHANNEL_NUM];
    hi1106_rx_gain_comp_val_stru ast_cali_rx_gain_cmp[HI1106_CALI_2G_OTHER_CHANNEL_NUM];
    hi1106_logen_comp_val_stru ast_cali_logen_cmp[HI1106_2G_CHAN_NUM];
    hi1106_2G_tx_power_comp_val_stru ast_cali_tx_power_cmp_2G[HI1106_2G_CHAN_NUM];
    hi1106_txdc_comp_val_stru ast_txdc_cmp_val[HI1106_2G_CHAN_NUM];
    hi1106_new_txiq_comp_stru ast_new_txiq_cmp[HI1106_2G_CHAN_NUM];
    hi1106_new_rxiq_comp_stru ast_new_rxiq_cmp[HI1106_2G_CHAN_NUM];
} hi1106_2Gcali_param_stru;

typedef struct {
    hi1106_rx_dc_comp_val_stru ast_cali_rx_dc_cmp[HI1106_CALI_2G_OTHER_CHANNEL_NUM];
    hi1106_txdc_comp_val_stru ast_txdc_cmp_val[HI1106_2G_CHAN_NUM];
    hi1106_new_txiq_comp_stru ast_new_txiq_cmp[HI1106_2G_CHAN_NUM];
    hi1106_new_rxiq_comp_stru ast_new_rxiq_cmp[HI1106_2G_CHAN_NUM];
} hi1106_2G_dbdc_cali_param_stru;

typedef struct {
    hi1106_rx_dc_comp_val_stru st_cali_rx_dc_cmp;
    hi1106_rx_gain_comp_val_stru st_cali_rx_gain_cmp;
    hi1106_logen_comp_val_stru st_cali_logen_cmp;
    hi1106_5G_tx_power_comp_val_stru st_cali_tx_power_cmp_5G;
    hi1106_ppf_comp_val_stru st_ppf_cmp_val;
    hi1106_txdc_comp_val_stru ast_txdc_cmp_val[HI1106_TXDC_MIXBUF_GAIN_LVL_NUM];
    hi1106_new_txiq_comp_stru ast_new_txiq_cmp;
    hi1106_new_rxiq_comp_stru ast_new_rxiq_cmp;
} hi1106_5Gcali_param_stru;

typedef struct {
    hi1106_2Gcali_param_stru ast_2Gcali_param[HI1106_2G_CHAN_NUM];
    hi1106_5Gcali_param_stru ast_5Gcali_param[HI1106_5G_CHANNEL_NUM];
} hi1106_wifi_cali_param_stru;

typedef struct {
    uint16_t us_cali_time;
    uint16_t bit_temperature : 3,
               uc_5g_chan_idx1 : 5,
               uc_5g_chan_idx2 : 5,
               bit_rev : 3;
} hi1106_update_cali_channel_stru;

typedef struct {
    uint8_t uc_rc_cmp_code;
    uint8_t uc_r_cmp_code; /* 保存PMU的原始5bit R code */
    /* 重要: MPW2和PILOT RF公用, mpw2代表c校准值; pilot代表800M rejection补偿code，根据C code计算得到 */
    uint8_t uc_c_cmp_code;
    uint8_t uc_20M_rc_cmp_code;
} hi1106_rc_r_c_cali_param_stru;

typedef struct {
    uint8_t ip2_i_cmp_code;
    uint8_t ip2_q_cmp_code;
    uint8_t current_trim;
    uint8_t rsv;
} hi1106_rx_ip2_cali_param_stru;

typedef struct {
    uint32_t dpd_even_lut[HI1106_DPD_EVEN_LUT_LENGTH];
    uint32_t dpd_odd_lut[HI1106_DPD_ODD_LUT_LENGTH];
    uint32_t dpd_glue_lut[HI1106_DPD_GLUE_LUT_LENGTH];
} hi1106_dpd_lut_stru;

typedef struct {
    uint16_t analog_dpd_rxdc_cmp[HI1106_DPD_TPC_LEVEL_NUM];
    uint16_t digital_dpd_rxdc_cmp_i;
    uint16_t digital_dpd_rxdc_cmp_q;
    int16_t cali_temperature;
    uint16_t resv;
} hi1106_dpd_rxdc_comp_val_stru;

typedef struct {
    hi1106_dpd_lut_stru cali_param_2g[HI1106_DPD_2G_LUT_NUM];
    hi1106_dpd_lut_stru cali_normal_param_5g[HI1106_DPD_5G_LUT_NUM];
    hi1106_dpd_lut_stru cali_side_param_5g[HI1106_DPD_5G_SIDE_LUT_NUM];
    hi1106_dpd_rxdc_comp_val_stru dpd_rxdc_param_2g[HI1106_CALI_2G_OTHER_CHANNEL_NUM];
    hi1106_dpd_rxdc_comp_val_stru dpd_rxdc_param_5g[HI1106_5G_CHANNEL_NUM];
} hi1106_dpd_cali_param_stru;

typedef struct {
    hi1106_dpd_lut_stru cali_param_2g[HI1106_DPD_2G_LUT_NUM];
    hi1106_dpd_lut_stru cali_normal_param_5g[HI1106_DPD_5G_LUT_NUM];
    hi1106_dpd_lut_stru cali_side_param_5g[HI1106_DPD_5G_SIDE_LUT_NUM];
    hi1106_dpd_rxdc_comp_val_stru dpd_rxdc_param_2g[HI1106_CALI_2G_OTHER_CHANNEL_NUM];
    hi1106_dpd_rxdc_comp_val_stru dpd_rxdc_param_5g[HI1106_5G_CHANNEL_NUM];
} hi1106_dpd_cali_all_param_stru;

struct hi1106_cali_param_tag {
    uint32_t ul_dog_tag;
    hi1106_2Gcali_param_stru st_2Gcali_param;
    hi1106_5Gcali_param_stru ast_5Gcali_param[HI1106_5G_CHANNEL_NUM];
#ifdef _PRE_WLAN_NEW_IQ
    hi1106_new_rxiq_comp_stru ast_20M_rxiq_cmp[HI1106_2G_CHAN_NUM + HI1106_5G_20M_CHAN_NUM][HI1106_CALI_CHAIN_NUM_BUTT];
    hi1106_new_rxiq_comp_stru ast_20M_rxiq_dbdc_cmp[HI1106_2G_CHAN_NUM][HI1106_CALI_CHAIN_NUM_BUTT];
#endif
    hi1106_dpd_cali_param_stru st_dpd_cali_data;
    hi1106_ppf_comp_val_stru st_165chan_ppf_comp;
    hi1106_update_cali_channel_stru st_cali_update_info;
    uint32_t ul_check_hw_status;
    hi1106_pa_ical_val_stru st_pa_ical_cmp;
    hi1106_rc_r_c_cali_param_stru st_rc_r_c_cali_data;
    hi1106_2G_dbdc_cali_param_stru st_2g_dbdc_cali_param;
    int16_t s_2g_idet_gm;
    oal_bool_enum_uint8 en_use_lpf_10M_for_2grx_20m;
    oal_bool_enum_uint8 en_save_all;
    uint8_t uc_last_cali_fail_status;
    uint8_t uc_vdet_dc_comp;
    uint8_t uc_cur_cali_policy;
    oal_bool_enum_uint8 en_need_close_fem_cali;
    hi1106_adc_cali_param_stru st_adc_cali_data;
    hi1106_rx_ip2_cali_param_stru rx_ip2_cali_comp_data;
};

typedef struct {
    // RX DC
    uint8_t uc_analog_rxdc_cmp_intlna_i[HAL_CALI_RXDC_GAIN_LVL_NUM];
    uint8_t uc_analog_rxdc_cmp_intlna_q[HAL_CALI_RXDC_GAIN_LVL_NUM];
    uint8_t uc_analog_rxdc_cmp_extlna_i[HAL_CALI_RXDC_GAIN_LVL_NUM];
    uint8_t uc_analog_rxdc_cmp_extlna_q[HAL_CALI_RXDC_GAIN_LVL_NUM];
    uint16_t us_digital_rxdc_cmp_i;
    uint16_t us_digital_rxdc_cmp_q;
    // DPD RX DC
    uint8_t  analog_dpd_rxdc_cmp_intlna_i;
    uint8_t  analog_dpd_rxdc_cmp_intlna_q;
    uint8_t  analog_dpd_rxdc_cmp_extlna_i;
    uint8_t  analog_dpd_rxdc_cmp_extlna_q;
    uint16_t digital_dpd_rxdc_cmp_i;
    uint16_t digital_dpd_rxdc_cmp_q;
    uint8_t  dpd_rx_dc_mpw_lna_idx;   // mpw下，用第7档借用第7档lna的dcoc，实际lna不起作用
    uint8_t  resv[3];                 // 3，对齐
} hal_rxdc_cali_param_stru;

typedef struct hi1106_cali_param_tag hi1106_cali_param_stru;

typedef struct {
    uint16_t rc_cmp_code;
    uint16_t r_cmp_code;
    uint16_t c_cmp_code;
    uint16_t rc_20m_cmp_code;
} wlan_cali_rc_r_c_para_stru;

typedef struct {
    uint8_t lo_ssb_tune;
    uint8_t lo_filpeak_tune;
    uint8_t lo_filnotch_tune;
    uint8_t resv;
} wlan_cali_logen_para_stru;

typedef struct {
    uint8_t lna_blk_itrim_cmp;
    uint8_t lna_blk_rtrim_cmp;
    uint8_t resv[2];
} wlan_cali_lna_blk_para_stru;

typedef struct {
    uint8_t  uc_vco_ldo_index;
    uint8_t  uc_trim_buf2lo_code;
    uint8_t  auc_resev[2];
}wlan_cali_involt_swing_para_stru;

typedef struct {
    // RX DC
    uint8_t analog_rxdc_cmp_intlna_i[WLAN_RX_DC_LNA_LVL];
    uint8_t analog_rxdc_cmp_intlna_q[WLAN_RX_DC_LNA_LVL];
    uint8_t analog_rxdc_cmp_extlna_i[WLAN_RX_DC_LNA_LVL];
    uint8_t analog_rxdc_cmp_extlna_q[WLAN_RX_DC_LNA_LVL];
    uint16_t digital_rxdc_cmp_i;
    uint16_t digital_rxdc_cmp_q;
} wlan_cali_rxdc_para_stru;

typedef struct {
    uint8_t analog_dpd_rxdc_cmp_i[WLAN_DPD_TPC_LEVEL_NUM];
    uint8_t analog_dpd_rxdc_cmp_q[WLAN_DPD_TPC_LEVEL_NUM];
    uint16_t digital_dpd_rxdc_cmp_i;
    uint16_t digital_dpd_rxdc_cmp_q;
    int16_t cali_temperature;
    uint16_t resv;
} wlan_cali_dpd_rxdc_para_stru;

typedef struct {
    // DPD RX DC
    uint8_t  analog_dfs_rxdc_cmp_intlna_i;
    uint8_t  analog_dfs_rxdc_cmp_intlna_q;
    uint8_t  analog_dfs_rxdc_cmp_extlna_i;
    uint8_t  analog_dfs_rxdc_cmp_extlna_q;
    uint16_t digital_dfs_rxdc_cmp_i;
    uint16_t digital_dfs_rxdc_cmp_q;
} wlan_cali_dfs_rxdc_para_stru;

typedef struct {
    uint16_t  ppa_cap_cmp;
    uint16_t  ppa_cmp;
} wlan_cali_2g_txpwr_para_stru;

typedef struct {
    uint16_t  mx_cap_cmp;
    uint16_t  ppa_cap_cmp;
    uint16_t  ppa_cmp;
    uint16_t  rsv;
} wlan_cali_5g_txpwr_para_stru;

typedef struct {
    uint16_t  txdc_cmp_i[WLAN_TX_DCIQ_CALI_LVL];
    uint16_t  txdc_cmp_q[WLAN_TX_DCIQ_CALI_LVL];
} wlan_cali_txdc_para_stru;

typedef struct {
    uint32_t coef[WLAN_IQ_QMC_QDC_COEF_NUM];
} wlan_cali_iq_coef_stru;

typedef struct {
    uint8_t vga_code;
    uint8_t att_code;
    uint8_t reserved[2];  /* 2, 对齐 */
} wlan_cali_iq_gain_cfg_stru;

typedef struct {
    wlan_cali_iq_coef_stru qmc_siso[WLAN_TX_DCIQ_CALI_LVL];
    wlan_cali_iq_coef_stru qmc_mimo[WLAN_TX_DCIQ_CALI_LVL];
    wlan_cali_iq_coef_stru qmc_fb;
    /* 保存offline txiq校准调整的增益配置, 用于online iq校准 */
    wlan_cali_iq_gain_cfg_stru gain_cfg[WLAN_TX_DCIQ_CALI_LVL];
} wlan_cali_txiq_para_per_freq_stru;

typedef struct {
    wlan_cali_iq_coef_stru qdc_siso[WLAN_RX_IQ_CALI_LVL];
    wlan_cali_iq_coef_stru qdc_mimo[WLAN_RX_IQ_CALI_LVL];
    wlan_cali_iq_coef_stru qdc_tx;
} wlan_cali_rxiq_para_per_freq_stru;

typedef struct {
    uint32_t dpd_even_lut[WLAN_DPD_EVEN_LUT_LENGTH];
    uint32_t dpd_odd_lut[WLAN_DPD_ODD_LUT_LENGTH];
    uint32_t dpd_glue_lut[WLAN_DPD_GLUE_LUT_LENGTH];
    uint8_t vga_gain;
    uint8_t resv[3];
} wlan_cali_dpd_lut_stru;

typedef struct {
    wlan_cali_dpd_lut_stru dpd_cali_para[WLAN_DPD_TPC_LEVEL_NUM];
} wlan_cali_dpd_para_stru;

typedef struct {
    wlan_cali_rc_r_c_para_stru rc_r_c_cali_data;
    wlan_cali_logen_para_stru logen_cali_data[WLAN_RF_CALI_DFLT_2G_FREQ_BAND_NUM];
    wlan_cali_rxdc_para_stru  rxdc_cali_data_20[WLAN_RF_CALI_DFLT_2G_FREQ_BAND_NUM];
    wlan_cali_rxdc_para_stru  rxdc_cali_data_40[WLAN_RF_CALI_DFLT_2G_FREQ_BAND_NUM];
    wlan_cali_rxdc_para_stru  rxdc_mimo_cali_data_20[WLAN_RF_CALI_DFLT_2G_FREQ_BAND_NUM];
    wlan_cali_rxdc_para_stru  rxdc_mimo_cali_data_40[WLAN_RF_CALI_DFLT_2G_FREQ_BAND_NUM];
    wlan_cali_dpd_rxdc_para_stru  dpd_rxdc_cali_data_20[WLAN_RF_CALI_DFLT_2G_FREQ_BAND_NUM];
    wlan_cali_dpd_rxdc_para_stru  dpd_rxdc_cali_data_40[WLAN_RF_CALI_DFLT_2G_FREQ_BAND_NUM];
    wlan_cali_txdc_para_stru  txdc_cali_data_20[WLAN_RF_CALI_DFLT_2G_FREQ_BAND_NUM];
    wlan_cali_txdc_para_stru  txdc_cali_data_40[WLAN_RF_CALI_DFLT_2G_FREQ_BAND_NUM];
    wlan_cali_lna_blk_para_stru lna_blk_cali_data;
    wlan_cali_2g_txpwr_para_stru txpwr_cali_data[WLAN_RF_CALI_DFLT_2G_FREQ_BAND_NUM];
    wlan_cali_txiq_para_per_freq_stru txiq_cali_data_20[WLAN_RF_CALI_DFLT_2G_FREQ_BAND_NUM];
    wlan_cali_txiq_para_per_freq_stru txiq_cali_data_40[WLAN_RF_CALI_DFLT_2G_FREQ_BAND_NUM];
    wlan_cali_rxiq_para_per_freq_stru rxiq_cali_data_20[WLAN_RF_CALI_DFLT_2G_FREQ_BAND_NUM];
    wlan_cali_rxiq_para_per_freq_stru rxiq_cali_data_40[WLAN_RF_CALI_DFLT_2G_FREQ_BAND_NUM];
    wlan_cali_dpd_para_stru  dpd_cali_data_20[WLAN_RF_CALI_DFLT_2G_FREQ_BAND_NUM];
} wlan_cali_2g_para_stru;

typedef struct {
    wlan_cali_rc_r_c_para_stru rc_r_c_cali_data;
    wlan_cali_logen_para_stru logen_cali_data[WLAN_RF_CALI_DFLT_5G_FREQ_BAND_NUM];
    wlan_cali_rxdc_para_stru  rxdc_cali_data_20[WLAN_RF_CALI_DFLT_5G_FREQ_BAND_NUM];
    wlan_cali_rxdc_para_stru  rxdc_cali_data_40[WLAN_RF_CALI_DFLT_5G_FREQ_BAND_NUM];
    wlan_cali_rxdc_para_stru  rxdc_cali_data_80[WLAN_RF_CALI_DFLT_5G_FREQ_BAND_NUM];
    wlan_cali_rxdc_para_stru  rxdc_cali_data_160[WLAN_RF_CALI_160M_5G_FREQ_BAND_NUM];
    wlan_cali_rxdc_para_stru  rxdc_mimo_cali_data_20[WLAN_RF_CALI_DFLT_5G_FREQ_BAND_NUM];
    wlan_cali_rxdc_para_stru  rxdc_mimo_cali_data_40[WLAN_RF_CALI_DFLT_5G_FREQ_BAND_NUM];
    wlan_cali_rxdc_para_stru  rxdc_mimo_cali_data_80[WLAN_RF_CALI_DFLT_5G_FREQ_BAND_NUM];
    wlan_cali_rxdc_para_stru  rxdc_mimo_cali_data_160[WLAN_RF_CALI_160M_5G_FREQ_BAND_NUM];
    wlan_cali_dpd_rxdc_para_stru  dpd_rxdc_cali_data_20[WLAN_RF_CALI_DFLT_5G_FREQ_BAND_NUM];
    wlan_cali_dpd_rxdc_para_stru  dpd_rxdc_cali_data_40[WLAN_RF_CALI_DFLT_5G_FREQ_BAND_NUM];
    wlan_cali_dpd_rxdc_para_stru  dpd_rxdc_cali_data_80[WLAN_RF_CALI_DFLT_5G_FREQ_BAND_NUM];
    wlan_cali_dpd_rxdc_para_stru  dpd_rxdc_cali_data_160[WLAN_RF_CALI_160M_5G_FREQ_BAND_NUM];
    wlan_cali_txdc_para_stru  txdc_cali_data_20[WLAN_RF_CALI_DFLT_5G_FREQ_BAND_NUM];
    wlan_cali_txdc_para_stru  txdc_cali_data_40[WLAN_RF_CALI_DFLT_5G_FREQ_BAND_NUM];
    wlan_cali_txdc_para_stru  txdc_cali_data_80[WLAN_RF_CALI_DFLT_5G_FREQ_BAND_NUM];
    wlan_cali_txdc_para_stru  txdc_cali_data_160[WLAN_RF_CALI_160M_5G_FREQ_BAND_NUM];
    wlan_cali_lna_blk_para_stru lna_blk_cali_data;
    wlan_cali_5g_txpwr_para_stru txpwr_cali_data[WLAN_RF_CALI_DFLT_5G_FREQ_BAND_NUM];
    wlan_cali_5g_txpwr_para_stru txpwr_cali_data_80[WLAN_RF_CALI_DFLT_5G_FREQ_BAND_NUM];
    wlan_cali_txiq_para_per_freq_stru txiq_cali_data_20[WLAN_RF_CALI_DFLT_5G_FREQ_BAND_NUM];
    wlan_cali_txiq_para_per_freq_stru txiq_cali_data_40[WLAN_RF_CALI_DFLT_5G_FREQ_BAND_NUM];
    wlan_cali_txiq_para_per_freq_stru txiq_cali_data_80[WLAN_RF_CALI_DFLT_5G_FREQ_BAND_NUM];
    wlan_cali_txiq_para_per_freq_stru txiq_cali_data_160[WLAN_RF_CALI_160M_5G_FREQ_BAND_NUM];
    wlan_cali_rxiq_para_per_freq_stru rxiq_cali_data_20[WLAN_RF_CALI_DFLT_5G_FREQ_BAND_NUM];
    wlan_cali_rxiq_para_per_freq_stru rxiq_cali_data_40[WLAN_RF_CALI_DFLT_5G_FREQ_BAND_NUM];
    wlan_cali_rxiq_para_per_freq_stru rxiq_cali_data_80[WLAN_RF_CALI_DFLT_5G_FREQ_BAND_NUM];
    wlan_cali_rxiq_para_per_freq_stru rxiq_cali_data_160[WLAN_RF_CALI_160M_5G_FREQ_BAND_NUM];
    wlan_cali_dpd_para_stru  dpd_cali_data_20[WLAN_RF_CALI_DFLT_5G_FREQ_BAND_NUM];
    wlan_cali_dpd_para_stru  dpd_cali_data_80[WLAN_RF_CALI_DFLT_5G_FREQ_BAND_NUM];
    wlan_cali_dpd_para_stru  dpd_cali_data_160[WLAN_RF_CALI_160M_5G_FREQ_BAND_NUM];
    wlan_cali_dpd_para_stru  dpd_cali_data_side_plus_80[WLAN_RF_CALI_DFLT_5G_FREQ_BAND_NUM];
    wlan_cali_dpd_para_stru  dpd_cali_data_side_minus_80[WLAN_RF_CALI_DFLT_5G_FREQ_BAND_NUM];
    wlan_cali_dpd_para_stru  dpd_cali_data_side_plus_160[WLAN_RF_CALI_160M_5G_FREQ_BAND_NUM];
    wlan_cali_dpd_para_stru  dpd_cali_data_side_minus_160[WLAN_RF_CALI_160M_5G_FREQ_BAND_NUM];
    uint8_t  pa_current_data;
    uint8_t  resv[3];
} wlan_cali_5g_para_stru;

/* 8 UNION定义 */
/* 9 OTHERS定义 */
/* 10 函数声明 */
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hal_cali.h */
