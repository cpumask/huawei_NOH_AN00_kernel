

#ifndef __PLAT_CALI_H__
#define __PLAT_CALI_H__

/* ͷ�ļ����� */
#include "plat_type.h"
#include "oal_types.h"
#include "oal_util.h"

/* �궨�� */
#define OAL_2G_CHANNEL_NUM            3
#define OAL_5G_20M_CHANNEL_NUM        7
#define OAL_5G_80M_CHANNEL_NUM        7
#define OAL_5G_160M_CHANNEL_NUM       2
#define OAL_5G_CHANNEL_NUM            (OAL_5G_20M_CHANNEL_NUM + OAL_5G_80M_CHANNEL_NUM + OAL_5G_160M_CHANNEL_NUM)
#define OAL_5G_DEVICE_CHANNEL_NUM     7
#define OAL_CALI_HCC_BUF_NUM          3
#define OAL_CALI_HCC_BUF_SIZE         1500
#define OAL_CALI_IQ_TONE_NUM          16
#define OAL_CALI_TXDC_GAIN_LVL_NUM    16 /* tx dc����ֵ��λ��Ŀ */
#define OAL_BT_RF_FEQ_NUM             79 /* total Rf frequency number */
#define OAL_BT_CHANNEL_NUM            8  /* total Rf frequency number */
#define OAL_BT_POWER_CALI_CHANNEL_NUM 3
#define OAL_BT_NVRAM_DATA_LENGTH      104
#define OAL_BT_NVRAM_NAME             "BTCALNV"
#define OAL_BT_NVRAM_NUMBER           352

#define WIFI_2_4G_ONLY              0x2424
#define SYS_EXCEP_REBOOT            0xC7C7
#define OAL_CALI_PARAM_ADDITION_LEN 8
#define OAL_5G_IQ_CALI_TONE_NUM     8

#define CHECK_5G_ENABLE "radio_cap_0"

/* wifiУ׼buf���� */
#define RF_CALI_DATA_BUF_LEN             0x4bb0
#define RF_SINGLE_CHAN_CALI_DATA_BUF_LEN (RF_CALI_DATA_BUF_LEN >> 1)
/* У׼�ṹ���С */
#define OAL_SINGLE_CALI_DATA_STRU_LEN (RF_CALI_DATA_BUF_LEN + 4)
#define OAL_DOUBLE_CALI_DATA_STRU_LEN (OAL_SINGLE_CALI_DATA_STRU_LEN)

/* 1105wifiУ׼buf���� */
#define HI1105_CALI_DATA_BUF_LEN             0xb380
#define HI1105_SINGLE_CHAN_CALI_DATA_BUF_LEN (HI1105_CALI_DATA_BUF_LEN >> 1)

/* 1105У׼�ṹ���С */
#define OAL_SOLO_CALI_DATA_STRU_LEN (HI1105_CALI_DATA_BUF_LEN + 4)
#define OAL_MIMO_CALI_DATA_STRU_LEN (OAL_SOLO_CALI_DATA_STRU_LEN)

/* 1106wifiУ׼buf���� */
#define HI1106_CALI_DATA_BUF_LEN             (HI1106_SINGLE_CHAN_CALI_DATA_BUF_LEN << 1)
#define HI1106_SINGLE_CHAN_CALI_DATA_BUF_LEN (OAL_SIZEOF(hi1106_cali_param_stru))

/* 1106У׼�ṹ���С */
#define HI1106_SOLO_CALI_DATA_STRU_LEN (HI1106_CALI_DATA_BUF_LEN + 4)
#define HI1106_MIMO_CALI_DATA_STRU_LEN (HI1106_SOLO_CALI_DATA_STRU_LEN)

/* ȫ�ֱ������� */
extern oal_uint32 g_cali_update_channel_info;
extern oal_uint8 g_netdev_is_open;

/* STRUCT ���� */
typedef struct {
    oal_uint16 us_analog_rxdc_cmp;
    oal_uint16 us_digital_rxdc_cmp_i;
    oal_uint16 us_digital_rxdc_cmp_q;
    oal_uint8 auc_reserve[2]; // 2�ֽڱ�������
} oal_rx_dc_comp_val_stru;

typedef struct {
    oal_uint8 upc_ppa_cmp;
    oal_int8 ac_atx_pwr_cmp;
    oal_uint8 dtx_pwr_cmp;
    oal_uint8 auc_reserve[1]; // 1�ֽڱ�������
} oal_2G_tx_power_comp_val_stru;

typedef struct {
    oal_uint8 upc_ppa_cmp;
    oal_uint8 upc_mx_cmp;
    oal_int8 ac_atx_pwr_cmp;
    oal_uint8 dtx_pwr_cmp;
} oal_5G_tx_power_comp_val_stru;

typedef struct {
    oal_uint16 us_txdc_cmp_i;
    oal_uint16 us_txdc_cmp_q;
} oal_txdc_comp_val_stru;

typedef struct {
    oal_uint8 uc_ppf_val;
    oal_uint8 auc_reserve[3]; // 3�ֽڱ�������
} oal_ppf_comp_val_stru;

typedef struct {
    oal_uint16 us_txiq_cmp_p;
    oal_uint16 us_txiq_cmp_e;
} oal_txiq_comp_val_stru;

typedef struct {
    oal_uint16 ul_cali_time;
    oal_uint16 bit_temperature : 3,
               uc_5g_chan_idx1 : 5,
               uc_5g_chan_idx2 : 5,
               en_update_bt : 3;
} oal_update_cali_channel_stru;

typedef struct {
    oal_uint32 ul_wifi_2_4g_only;
    oal_uint32 ul_excep_reboot;
    oal_uint32 ul_reserve[OAL_CALI_PARAM_ADDITION_LEN];
} oal_cali_param_addition_stru;

/* �������� */
extern oal_int32 get_cali_count(oal_uint32 *count);
extern oal_int32 get_bfgx_cali_data(oal_uint8 *buf, oal_uint32 *len, oal_uint32 buf_len);
extern void *get_cali_data_buf_addr(void);
extern oal_int32 cali_data_buf_malloc(void);
extern void cali_data_buf_free(void);

typedef struct {
    char *name;
    int32 init_value;
} bfgx_ini_cmd;

/* ����5���궨�壬���Ҫ�޸ĳ��ȣ���Ҫͬ���޸�device�ĺ궨�� */
#define BFGX_BT_CALI_DATA_SIZE             492
#define WIFI_CALI_DATA_FOR_FM_RC_CODE_SIZE 20
#define BFGX_NV_DATA_SIZE                  128
#define BFGX_BT_CUST_INI_SIZE              512
#define WIFI_CALI_DATA_FOR_BT              896
/* ���ǽṹ�����峤�ȿ���SDIO���س���512�������ԣ����ﳤ��Ϊ2048 */
typedef struct {
    oal_uint8 auc_bfgx_data[BFGX_BT_CALI_DATA_SIZE];
    oal_uint8 auc_wifi_rc_code_data[WIFI_CALI_DATA_FOR_FM_RC_CODE_SIZE];
    oal_uint8 auc_nv_data[BFGX_NV_DATA_SIZE];
    oal_uint8 auc_bt_cust_ini_data[BFGX_BT_CUST_INI_SIZE];
    oal_uint8 auc_wifi_cali_for_bt_data[WIFI_CALI_DATA_FOR_BT];
} bfgx_cali_data_stru;

#define BFGX_CALI_DATA_BUF_LEN (sizeof(bfgx_cali_data_stru))

extern struct completion g_cali_recv_done;

int32 bfgx_customize_init(void);
void *bfgx_get_cali_data_buf(uint32 *pul_len);
void *wifi_get_bfgx_rc_data_buf_addr(uint32 *pul_len);
void *wifi_get_bt_cali_data_buf(uint32 *pul_len);
int32 bfgx_cali_data_init(void);

#endif /* end of plat_cali.h */
