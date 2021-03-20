

#if defined(_PRE_PRODUCT_ID_HI110X_HOST)

/* ͷ�ļ����� */
#include <linux/kernel.h>
#include <linux/time.h>
#include "hisi_customize_wifi_hi110x.h"
#include "hisi_ini.h"
#include "plat_type.h"
#include "oam_ext_if.h"
#include "wlan_spec.h"
#include "oal_sdio_comm.h"
#include "oal_hcc_host_if.h"
#include "oal_main.h"
#include "mac_common.h"
#include "plat_pm_wlan.h"
#include "plat_firmware.h"

/* �ն�ͷ�ļ� */
#ifdef _PRE_CONFIG_READ_DYNCALI_E2PROM
#define hisi_nve_direct_access(...)  0
#else
#include <linux/mtd/hisi_nve_interface.h>
#endif

#include <linux/etherdevice.h>

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HISI_CUSTOMIZE_WIFI_HI110X_C

/* ȫ�ֱ������� */
OAL_STATIC int32 g_host_init_params[WLAN_CFG_INIT_BUTT] = {0}; /* ini���ƻ��������� */
OAL_STATIC int32 g_dts_params[WLAN_CFG_DTS_BUTT] = {0};        /* dts���ƻ��������� */
int8 g_wifi_country_code[COUNTRY_CODE_LEN] = "00";
OAL_STATIC uint8 g_wifi_mac[MAC_LEN] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
OAL_STATIC int32 g_nvram_init_params[NVRAM_PARAMS_PWR_INDEX_BUTT] = {0};         /* ini�ļ���NV�������� */
OAL_STATIC wlan_customize_private_stru g_priv_cust_params[WLAN_CFG_PRIV_BUTT] = {{ 0, 0 }}; /* ˽�ж��ƻ��������� */
wlan_cust_country_code_ingore_flag_stru g_cust_country_code_ignore_flag = {0};   /* ���ƻ����������� */
OAL_STATIC wlan_init_cust_nvram_params g_cust_nv_params = {{{0}}};               /* ����͹��ʶ��ƻ����� */

/* ���ⶨ�ƻ��������� */
wlan_customize_pwr_fit_para_stru g_pro_line_params[WLAN_RF_CHANNEL_NUMS][DY_CALI_PARAMS_NUM] = {{{0}}};
OAL_STATIC uint8 g_cust_nvram_info[WLAN_CFG_DTS_NVRAM_END][CUS_PARAMS_LEN_MAX] = {{0}}; /* NVRAM���� */
oal_bool_enum_uint8 g_en_nv_dp_init_is_null = OAL_TRUE;                              /* NVRAM��dp init�ÿձ�־ */
oal_int16 g_gs_extre_point_vals[WLAN_RF_CHANNEL_NUMS][DY_CALI_NUM_5G_BAND] = {{0}};
OAL_STATIC uint8 g_wlan_open_cnt = 0;
#ifdef _PRE_WLAN_FEATURE_TAS_ANT_SWITCH
oal_bool_enum_uint8 g_tas_switch_en[WLAN_RF_CHANNEL_MAX_NUMS] = {0};
#endif
oal_bool_enum_uint8 g_en_fact_cali_completed = OAL_FALSE;
oal_bool_enum_uint8 g_wlan_cal_intvl_enable = OAL_TRUE; /* ʹ�ܿ�wifi�ظ�У׼�ļ�� */
oal_bool_enum_uint8 g_wlan_cal_disable_switch = OAL_FALSE; /* ����wifi��У׼ */

wlan_customize_stru g_wlan_customize = {
    WLAN_AMPDU_TX_DEFAULT_NUM, /* addba_buffer_size */
    1,                    /* roam switch */
    CUS_ROAM_SCAN_ORTHOGONAL_DEFAULT,  /* roam scan org */
    -70,                  /* roam trigger 2G */
    -70,                  /* roam trigger 5G */
    10,                   /* roam delta 2G */
    10,                   /* roam delta 5G */
    0,                    /* random mac addr scan */
    0,                    /* disable_capab_2ght40 */
    0, /* lte_gpio_check_switch */
    0, /* ism_priority */
    0, /* lte_rx */
    0, /* lte_tx */
    0, /* lte_inact */
    0, /* ism_rx_act */
    0, /* bant_pri */
    0, /* bant_status */
    0, /* want_pri */
    0, /* want_status */
};
wlan_customize_stru *g_wlan_cust = &g_wlan_customize;

/*
 *  regdomain <-> country code map table
 *  max support country num: MAX_COUNTRY_COUNT
 *
 */
OAL_STATIC countryinfo_stru g_country_info_table[] = {
    /* Note:too few initializers for unsigned char [3] */
    /*lint -e785*/
    { REGDOMAIN_COMMON, { '0', '0' }},  // WORLD DOMAIN
    { REGDOMAIN_FCC,    { 'A', 'A' }},  // Specific country code for pad
    { REGDOMAIN_FCC, { 'A', 'D' }},     // ANDORRA
    { REGDOMAIN_ETSI, { 'A', 'E' }},    // UAE
    { REGDOMAIN_ETSI, { 'A', 'F' }},    // AFGHANISTAN
    { REGDOMAIN_ETSI, { 'A', 'G' }},    // ANTIGUA AND BARBUDA
    { REGDOMAIN_ETSI, { 'A', 'I' }},    // ANGUILLA
    { REGDOMAIN_ETSI, { 'A', 'L' }},    // ALBANIA
    { REGDOMAIN_ETSI, { 'A', 'M' }},    // ARMENIA
    { REGDOMAIN_ETSI, { 'A', 'N' }},    // NETHERLANDS ANTILLES
    { REGDOMAIN_ETSI, { 'A', 'O' }},    // ANGOLA
    { REGDOMAIN_FCC, { 'A', 'R' }},     // ARGENTINA
    { REGDOMAIN_FCC, { 'A', 'S' }},     // AMERICAN SOMOA
    { REGDOMAIN_ETSI, { 'A', 'T' }},    // AUSTRIA
    { REGDOMAIN_ETSI, { 'A', 'U' }},    // AUSTRALIA
    { REGDOMAIN_ETSI, { 'A', 'W' }},    // ARUBA
    { REGDOMAIN_ETSI, { 'A', 'Z' }},    // AZERBAIJAN
    { REGDOMAIN_ETSI, { 'B', 'A' }},    // BOSNIA AND HERZEGOVINA
    { REGDOMAIN_FCC, { 'B', 'B' }},     // BARBADOS
    { REGDOMAIN_ETSI, { 'B', 'D' }},    // BANGLADESH
    { REGDOMAIN_ETSI, { 'B', 'E' }},    // BELGIUM
    { REGDOMAIN_ETSI, { 'B', 'G' }},    // BULGARIA
    { REGDOMAIN_ETSI, { 'B', 'H' }},    // BAHRAIN
    { REGDOMAIN_ETSI, { 'B', 'L' }},    //
    { REGDOMAIN_FCC, { 'B', 'M' }},     // BERMUDA
    { REGDOMAIN_ETSI, { 'B', 'N' }},    // BRUNEI DARUSSALAM
    { REGDOMAIN_FCC, { 'B', 'O' }},     // BOLIVIA
    { REGDOMAIN_FCC, { 'B', 'R' }},     // BRAZIL
    { REGDOMAIN_FCC, { 'B', 'S' }},     // BAHAMAS
    { REGDOMAIN_ETSI, { 'B', 'Y' }},    // BELARUS
    { REGDOMAIN_ETSI, { 'B', 'Z' }},    // BELIZE
    { REGDOMAIN_FCC, { 'C', 'A' }},     // CANADA
    { REGDOMAIN_ETSI, { 'C', 'H' }},    // SWITZERLAND
    { REGDOMAIN_FCC, { 'C', 'L' }},     // CHILE
    { REGDOMAIN_COMMON, { 'C', 'N' }},  // CHINA
    { REGDOMAIN_FCC, { 'C', 'O' }},     // COLOMBIA
    { REGDOMAIN_FCC, { 'C', 'R' }},     // COSTA RICA
    { REGDOMAIN_ETSI, { 'C', 'S' }},
    { REGDOMAIN_ETSI, { 'C', 'U' }},  // CUBA
    { REGDOMAIN_ETSI, { 'C', 'Y' }},  // CYPRUS
    { REGDOMAIN_ETSI, { 'C', 'Z' }},  // CZECH REPUBLIC
    { REGDOMAIN_ETSI, { 'D', 'E' }},  // GERMANY
    { REGDOMAIN_ETSI, { 'D', 'K' }},  // DENMARK
    { REGDOMAIN_FCC, { 'D', 'O' }},   // DOMINICAN REPUBLIC
    { REGDOMAIN_ETSI, { 'D', 'Z' }},  // ALGERIA
    { REGDOMAIN_FCC, { 'E', 'C' }},   // ECUADOR
    { REGDOMAIN_ETSI, { 'E', 'E' }},  // ESTONIA
    { REGDOMAIN_ETSI, { 'E', 'G' }},  // EGYPT
    { REGDOMAIN_ETSI, { 'E', 'S' }},  // SPAIN
    { REGDOMAIN_ETSI, { 'E', 'T' }},  // ETHIOPIA
    { REGDOMAIN_ETSI, { 'F', 'I' }},  // FINLAND
    { REGDOMAIN_ETSI, { 'F', 'R' }},  // FRANCE
    { REGDOMAIN_ETSI, { 'G', 'B' }},  // UNITED KINGDOM
    { REGDOMAIN_FCC, { 'G', 'D' }},   // GRENADA
    { REGDOMAIN_ETSI, { 'G', 'E' }},  // GEORGIA
    { REGDOMAIN_ETSI, { 'G', 'F' }},  // FRENCH GUIANA
    { REGDOMAIN_ETSI, { 'G', 'L' }},  // GREENLAND
    { REGDOMAIN_ETSI, { 'G', 'P' }},  // GUADELOUPE
    { REGDOMAIN_ETSI, { 'G', 'R' }},  // GREECE
    { REGDOMAIN_FCC, { 'G', 'T' }},   // GUATEMALA
    { REGDOMAIN_FCC, { 'G', 'U' }},   // GUAM
    { REGDOMAIN_ETSI, { 'H', 'K' }},  // HONGKONG
    { REGDOMAIN_FCC, { 'H', 'N' }},   // HONDURAS
    { REGDOMAIN_ETSI, { 'H', 'R' }},  // Croatia
    { REGDOMAIN_ETSI, { 'H', 'U' }},  // HUNGARY
    { REGDOMAIN_ETSI, { 'I', 'D' }},  // INDONESIA
    { REGDOMAIN_ETSI, { 'I', 'E' }},  // IRELAND
    { REGDOMAIN_ETSI, { 'I', 'L' }},  // ISRAEL
    { REGDOMAIN_ETSI, { 'I', 'N' }},  // INDIA
    { REGDOMAIN_ETSI, { 'I', 'Q' }},  // IRAQ
    { REGDOMAIN_ETSI, { 'I', 'R' }},  // IRAN, ISLAMIC REPUBLIC OF
    { REGDOMAIN_ETSI, { 'I', 'S' }},  // ICELNAD
    { REGDOMAIN_ETSI, { 'I', 'T' }},  // ITALY
    { REGDOMAIN_FCC, { 'J', 'M' }},   // JAMAICA
    { REGDOMAIN_ETSI, { 'J', 'P' }},  // JAPAN
    { REGDOMAIN_ETSI, { 'J', 'O' }},  // JORDAN
    { REGDOMAIN_ETSI, { 'K', 'E' }},  // KENYA
    { REGDOMAIN_ETSI, { 'K', 'H' }},  // CAMBODIA
    { REGDOMAIN_ETSI, { 'K', 'P' }},  // KOREA, DEMOCRATIC PEOPLE's REPUBLIC OF
    { REGDOMAIN_ETSI, { 'K', 'R' }},  // KOREA, REPUBLIC OF
    { REGDOMAIN_ETSI, { 'K', 'W' }},  // KUWAIT
    { REGDOMAIN_ETSI, { 'K', 'Y' }},  // Cayman Is
    { REGDOMAIN_ETSI, { 'K', 'Z' }},  // KAZAKHSTAN
    { REGDOMAIN_ETSI, { 'L', 'B' }},  // LEBANON
    { REGDOMAIN_ETSI, { 'L', 'I' }},  // LIECHTENSTEIN
    { REGDOMAIN_ETSI, { 'L', 'K' }},  // SRI-LANKA
    { REGDOMAIN_ETSI, { 'L', 'S' }},  // KINGDOM OF LESOTH
    { REGDOMAIN_ETSI, { 'L', 'T' }},  // LITHUANIA
    { REGDOMAIN_ETSI, { 'L', 'U' }},  // LUXEMBOURG
    { REGDOMAIN_ETSI, { 'L', 'V' }},  // LATVIA
    { REGDOMAIN_ETSI, { 'M', 'A' }},  // MOROCCO
    { REGDOMAIN_ETSI, { 'M', 'C' }},  // MONACO
    { REGDOMAIN_ETSI, { 'M', 'D' }},  // REPUBLIC OF MOLDOVA
    { REGDOMAIN_ETSI, { 'M', 'E' }},  // Montenegro
    { REGDOMAIN_FCC, { 'M', 'H' }},   // Marshall Is
    { REGDOMAIN_ETSI, { 'M', 'K' }},  // MACEDONIA, THE FORMER YUGOSLAV REPUBLIC OF
    { REGDOMAIN_ETSI, { 'M', 'M' }},  // MYANMAR
    { REGDOMAIN_FCC, { 'M', 'N' }},   // MONGOLIA
    { REGDOMAIN_ETSI, { 'M', 'O' }},  // MACAO
    { REGDOMAIN_FCC, { 'M', 'P' }},   // NORTHERN MARIANA ISLANDS
    { REGDOMAIN_ETSI, { 'M', 'Q' }},  // MARTINIQUE
    { REGDOMAIN_ETSI, { 'M', 'R' }},  // Mauritania
    { REGDOMAIN_ETSI, { 'M', 'T' }},  // MALTA
    { REGDOMAIN_ETSI, { 'M', 'V' }},  // Maldives
    { REGDOMAIN_ETSI, { 'M', 'U' }},  // MAURITIUS
    { REGDOMAIN_ETSI, { 'M', 'W' }},  // MALAWI
    { REGDOMAIN_ETSI, { 'M', 'X' }},  // MEXICO
    { REGDOMAIN_ETSI, { 'M', 'Y' }},  // MALAYSIA
    { REGDOMAIN_ETSI, { 'N', 'G' }},  // NIGERIA
    { REGDOMAIN_FCC, { 'N', 'I' }},   // NICARAGUA
    { REGDOMAIN_ETSI, { 'N', 'L' }},  // NETHERLANDS
    { REGDOMAIN_ETSI, { 'N', 'O' }},  // NORWAY
    { REGDOMAIN_ETSI, { 'N', 'P' }},  // NEPAL
    { REGDOMAIN_ETSI, { 'N', 'Z' }},  // NEW-ZEALAND
    { REGDOMAIN_ETSI, { 'O', 'M' }},  // OMAN
    { REGDOMAIN_FCC, { 'P', 'A' }},   // PANAMA
    { REGDOMAIN_FCC, { 'P', 'E' }},   // PERU
    { REGDOMAIN_ETSI, { 'P', 'F' }},  // FRENCH POLYNESIA
    { REGDOMAIN_ETSI, { 'P', 'G' }},  // PAPUA NEW GUINEA
    { REGDOMAIN_ETSI, { 'P', 'H' }},  // PHILIPPINES
    { REGDOMAIN_ETSI, { 'P', 'K' }},  // PAKISTAN
    { REGDOMAIN_ETSI, { 'P', 'L' }},  // POLAND
    { REGDOMAIN_FCC, { 'P', 'R' }},   // PUERTO RICO
    { REGDOMAIN_FCC, { 'P', 'S' }},   // PALESTINIAN TERRITORY, OCCUPIED
    { REGDOMAIN_ETSI, { 'P', 'T' }},  // PORTUGAL
    { REGDOMAIN_FCC, { 'P', 'Y' }},   // PARAGUAY
    { REGDOMAIN_ETSI, { 'Q', 'A' }},  // QATAR
    { REGDOMAIN_ETSI, { 'R', 'E' }},  // REUNION
    { REGDOMAIN_ETSI, { 'R', 'O' }},  // ROMAINIA
    { REGDOMAIN_ETSI, { 'R', 'S' }},  // SERBIA
    { REGDOMAIN_ETSI, { 'R', 'U' }},  // RUSSIA
    { REGDOMAIN_FCC,  { 'R', 'W' }},   // RWANDA
    { REGDOMAIN_ETSI, { 'S', 'A' }},  // SAUDI ARABIA
    { REGDOMAIN_ETSI, { 'S', 'D' }},  // SUDAN ,REPUBLIC OF THE
    { REGDOMAIN_ETSI, { 'S', 'E' }},  // SWEDEN
    { REGDOMAIN_ETSI, { 'S', 'G' }},  // SINGAPORE
    { REGDOMAIN_ETSI, { 'S', 'I' }},  // SLOVENNIA
    { REGDOMAIN_ETSI, { 'S', 'K' }},  // SLOVAKIA
    { REGDOMAIN_ETSI, { 'S', 'N' }},  // SENEGAL
    { REGDOMAIN_ETSI, { 'S', 'V' }},  // EL SALVADOR
    { REGDOMAIN_ETSI, { 'S', 'Y' }},  // SYRIAN ARAB REPUBLIC
    { REGDOMAIN_ETSI, { 'T', 'H' }},  // THAILAND
    { REGDOMAIN_ETSI, { 'T', 'N' }},  // TUNISIA
    { REGDOMAIN_ETSI, { 'T', 'R' }},  // TURKEY
    { REGDOMAIN_ETSI, { 'T', 'T' }},  // TRINIDAD AND TOBAGO
    { REGDOMAIN_FCC, { 'T', 'W' }},   // TAIWAN, PRIVINCE OF CHINA
    { REGDOMAIN_FCC, { 'T', 'Z' }},   // TANZANIA, UNITED REPUBLIC OF
    { REGDOMAIN_ETSI, { 'U', 'A' }},  // UKRAINE
    { REGDOMAIN_ETSI, { 'U', 'G' }},  // UGANDA
    { REGDOMAIN_FCC, { 'U', 'S' }},   // USA
    { REGDOMAIN_FCC, { 'U', 'Y' }},   // URUGUAY
    { REGDOMAIN_ETSI, { 'U', 'Z' }},  // UZBEKISTAN
    { REGDOMAIN_FCC, { 'V', 'E' }},   // VENEZUELA
    { REGDOMAIN_FCC, { 'V', 'I' }},   // VIRGIN ISLANDS, US
    { REGDOMAIN_ETSI, { 'V', 'N' }},  // VIETNAM
    { REGDOMAIN_ETSI, { 'Y', 'E' }},  // YEMEN
    { REGDOMAIN_ETSI, { 'Y', 'T' }},  // MAYOTTE
    { REGDOMAIN_ETSI, { 'Z', 'A' }},  // SOUTH AFRICA
    { REGDOMAIN_ETSI, { 'Z', 'M' }},  // Zambia
    { REGDOMAIN_ETSI, { 'Z', 'W' }},  // ZIMBABWE
    { REGDOMAIN_FCC,  { 'Z', 'Z' }},  // country code without mobile

    { REGDOMAIN_COUNT, { '9', '9' }}
    /*lint +e785*/
};

OAL_STATIC wlan_cfg_cmd g_wifi_config_dts[] = {
    /* У׼ */
    { "cali_txpwr_pa_dc_ref_2g_val_chan1",  WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN1 },
    { "cali_txpwr_pa_dc_ref_2g_val_chan2",  WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN2 },
    { "cali_txpwr_pa_dc_ref_2g_val_chan3",  WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN3 },
    { "cali_txpwr_pa_dc_ref_2g_val_chan4",  WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN4 },
    { "cali_txpwr_pa_dc_ref_2g_val_chan5",  WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN5 },
    { "cali_txpwr_pa_dc_ref_2g_val_chan6",  WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN6 },
    { "cali_txpwr_pa_dc_ref_2g_val_chan7",  WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN7 },
    { "cali_txpwr_pa_dc_ref_2g_val_chan8",  WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN8 },
    { "cali_txpwr_pa_dc_ref_2g_val_chan9",  WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN9 },
    { "cali_txpwr_pa_dc_ref_2g_val_chan10", WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN10 },
    { "cali_txpwr_pa_dc_ref_2g_val_chan11", WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN11 },
    { "cali_txpwr_pa_dc_ref_2g_val_chan12", WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN12 },
    { "cali_txpwr_pa_dc_ref_2g_val_chan13", WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN13 },

    { "cali_txpwr_pa_dc_ref_5g_val_band1", WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_5G_VAL_BAND1 },
    { "cali_txpwr_pa_dc_ref_5g_val_band2", WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_5G_VAL_BAND2 },
    { "cali_txpwr_pa_dc_ref_5g_val_band3", WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_5G_VAL_BAND3 },
    { "cali_txpwr_pa_dc_ref_5g_val_band4", WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_5G_VAL_BAND4 },
    { "cali_txpwr_pa_dc_ref_5g_val_band5", WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_5G_VAL_BAND5 },
    { "cali_txpwr_pa_dc_ref_5g_val_band6", WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_5G_VAL_BAND6 },
    { "cali_txpwr_pa_dc_ref_5g_val_band7", WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_5G_VAL_BAND7 },
    { "cali_tone_amp_grade",               WLAN_CFG_DTS_CALI_TONE_AMP_GRADE },
    /* DPDУ׼���ƻ� */
    { "dpd_cali_ch_core0",          WLAN_CFG_DTS_DPD_CALI_CH_CORE0 },
    { "dpd_use_cail_ch_idx0_core0", WLAN_CFG_DTS_DPD_USE_CALI_CH_IDX0_CORE0 },
    { "dpd_use_cail_ch_idx1_core0", WLAN_CFG_DTS_DPD_USE_CALI_CH_IDX1_CORE0 },
    { "dpd_use_cail_ch_idx2_core0", WLAN_CFG_DTS_DPD_USE_CALI_CH_IDX2_CORE0 },
    { "dpd_use_cail_ch_idx3_core0", WLAN_CFG_DTS_DPD_USE_CALI_CH_IDX3_CORE0 },
    { "dpd_cali_20m_del_pow_core0", WLAN_CFG_DTS_DPD_CALI_20M_DEL_POW_CORE0 },
    { "dpd_cali_40m_del_pow_core0", WLAN_CFG_DTS_DPD_CALI_40M_DEL_POW_CORE0 },
    { "dpd_cali_ch_core1",          WLAN_CFG_DTS_DPD_CALI_CH_CORE1 },
    { "dpd_use_cail_ch_idx0_core1", WLAN_CFG_DTS_DPD_USE_CALI_CH_IDX0_CORE1 },
    { "dpd_use_cail_ch_idx1_core1", WLAN_CFG_DTS_DPD_USE_CALI_CH_IDX1_CORE1 },
    { "dpd_use_cail_ch_idx2_core1", WLAN_CFG_DTS_DPD_USE_CALI_CH_IDX2_CORE1 },
    { "dpd_use_cail_ch_idx3_core1", WLAN_CFG_DTS_DPD_USE_CALI_CH_IDX3_CORE1 },
    { "dpd_cali_20m_del_pow_core1", WLAN_CFG_DTS_DPD_CALI_20M_DEL_POW_CORE1 },
    { "dpd_cali_40m_del_pow_core1", WLAN_CFG_DTS_DPD_CALI_40M_DEL_POW_CORE1 },
    /* ��̬У׼ */
    { "dyn_cali_dscr_interval", WLAN_CFG_DTS_DYN_CALI_DSCR_ITERVL },
    { "dyn_cali_opt_switch",    WLAN_CFG_DTS_DYN_CALI_OPT_SWITCH },
    { "gm0_dB10_amend",         WLAN_CFG_DTS_DYN_CALI_GM0_DB10_AMEND },
    /* DPN 40M 20M 11b */
    { "dpn24g_ch1_core0",  WLAN_CFG_DTS_2G_CORE0_DPN_CH1 },
    { "dpn24g_ch2_core0",  WLAN_CFG_DTS_2G_CORE0_DPN_CH2 },
    { "dpn24g_ch3_core0",  WLAN_CFG_DTS_2G_CORE0_DPN_CH3 },
    { "dpn24g_ch4_core0",  WLAN_CFG_DTS_2G_CORE0_DPN_CH4 },
    { "dpn24g_ch5_core0",  WLAN_CFG_DTS_2G_CORE0_DPN_CH5 },
    { "dpn24g_ch6_core0",  WLAN_CFG_DTS_2G_CORE0_DPN_CH6 },
    { "dpn24g_ch7_core0",  WLAN_CFG_DTS_2G_CORE0_DPN_CH7 },
    { "dpn24g_ch8_core0",  WLAN_CFG_DTS_2G_CORE0_DPN_CH8 },
    { "dpn24g_ch9_core0",  WLAN_CFG_DTS_2G_CORE0_DPN_CH9 },
    { "dpn24g_ch10_core0", WLAN_CFG_DTS_2G_CORE0_DPN_CH10 },
    { "dpn24g_ch11_core0", WLAN_CFG_DTS_2G_CORE0_DPN_CH11 },
    { "dpn24g_ch12_core0", WLAN_CFG_DTS_2G_CORE0_DPN_CH12 },
    { "dpn24g_ch13_core0", WLAN_CFG_DTS_2G_CORE0_DPN_CH13 },
    { "dpn5g_core0_b0",    WLAN_CFG_DTS_5G_CORE0_DPN_B0 },
    { "dpn5g_core0_b1",    WLAN_CFG_DTS_5G_CORE0_DPN_B1 },
    { "dpn5g_core0_b2",    WLAN_CFG_DTS_5G_CORE0_DPN_B2 },
    { "dpn5g_core0_b3",    WLAN_CFG_DTS_5G_CORE0_DPN_B3 },
    { "dpn5g_core0_b4",    WLAN_CFG_DTS_5G_CORE0_DPN_B4 },
    { "dpn5g_core0_b5",    WLAN_CFG_DTS_5G_CORE0_DPN_B5 },
    { "dpn5g_core0_b6",    WLAN_CFG_DTS_5G_CORE0_DPN_B6 },
    { "dpn24g_ch1_core1",  WLAN_CFG_DTS_2G_CORE1_DPN_CH1 },
    { "dpn24g_ch2_core1",  WLAN_CFG_DTS_2G_CORE1_DPN_CH2 },
    { "dpn24g_ch3_core1",  WLAN_CFG_DTS_2G_CORE1_DPN_CH3 },
    { "dpn24g_ch4_core1",  WLAN_CFG_DTS_2G_CORE1_DPN_CH4 },
    { "dpn24g_ch5_core1",  WLAN_CFG_DTS_2G_CORE1_DPN_CH5 },
    { "dpn24g_ch6_core1",  WLAN_CFG_DTS_2G_CORE1_DPN_CH6 },
    { "dpn24g_ch7_core1",  WLAN_CFG_DTS_2G_CORE1_DPN_CH7 },
    { "dpn24g_ch8_core1",  WLAN_CFG_DTS_2G_CORE1_DPN_CH8 },
    { "dpn24g_ch9_core1",  WLAN_CFG_DTS_2G_CORE1_DPN_CH9 },
    { "dpn24g_ch10_core1", WLAN_CFG_DTS_2G_CORE1_DPN_CH10 },
    { "dpn24g_ch11_core1", WLAN_CFG_DTS_2G_CORE1_DPN_CH11 },
    { "dpn24g_ch12_core1", WLAN_CFG_DTS_2G_CORE1_DPN_CH12 },
    { "dpn24g_ch13_core1", WLAN_CFG_DTS_2G_CORE1_DPN_CH13 },
    { "dpn5g_core1_b0",    WLAN_CFG_DTS_5G_CORE1_DPN_B0 },
    { "dpn5g_core1_b1",    WLAN_CFG_DTS_5G_CORE1_DPN_B1 },
    { "dpn5g_core1_b2",    WLAN_CFG_DTS_5G_CORE1_DPN_B2 },
    { "dpn5g_core1_b3",    WLAN_CFG_DTS_5G_CORE1_DPN_B3 },
    { "dpn5g_core1_b4",    WLAN_CFG_DTS_5G_CORE1_DPN_B4 },
    { "dpn5g_core1_b5",    WLAN_CFG_DTS_5G_CORE1_DPN_B5 },
    { "dpn5g_core1_b6",    WLAN_CFG_DTS_5G_CORE1_DPN_B6 },
    { OAL_PTR_NULL, 0 }
};

OAL_STATIC wlan_cfg_cmd g_wifi_config_priv[] = {
    /* У׼���� */
    { "cali_mask", WLAN_CFG_PRIV_CALI_MASK },
    /*
     * #bit0:��wifi����У׼ bit1:��wifi�����ϴ� bit2:����У׼ bit3:��̬У׼��ƽDebug
     * #bit4:����ȡNV���������(1:����ȡ 0����ȡ)
     */
    { "cali_data_mask", WLAN_CFG_PRIV_CALI_DATA_MASK },
    { "cali_auto_cali_mask", WLAN_CFG_PRIV_CALI_AUTOCALI_MASK },
    { "bw_max_width", WLAN_CFG_PRIV_BW_MAX_WITH },
    { "ldpc_coding",  WLAN_CFG_PRIV_LDPC_CODING },
    { "rx_stbc",      WLAN_CFG_PRIV_RX_STBC },
    { "tx_stbc",      WLAN_CFG_PRIV_TX_STBC },
    { "su_bfer",      WLAN_CFG_PRIV_SU_BFER },
    { "su_bfee",      WLAN_CFG_PRIV_SU_BFEE },
    { "mu_bfer",      WLAN_CFG_PRIV_MU_BFER },
    { "mu_bfee",      WLAN_CFG_PRIV_MU_BFEE },

    { "11n_txbf", WLAN_CFG_PRIV_11N_TXBF },

    { "1024qam_en", WLAN_CFG_PRIV_1024_QAM },
    /* DBDC */
    { "radio_cap_0",     WLAN_CFG_PRIV_DBDC_RADIO_0 },
    { "radio_cap_1",     WLAN_CFG_PRIV_DBDC_RADIO_1 },
    { "fastscan_switch", WLAN_CFG_PRIV_FASTSCAN_SWITCH },

    /* RSSI�����л� */
    { "rssi_ant_switch", WLAN_CFG_ANT_SWITCH },

     /* ��������ѧϰ���ܿ��� */
    { "countrycode_selfstudy", WLAN_CFG_PRIV_COUNRTYCODE_SELFSTUDY_CFG},

    { "m2s_function_mask_ext", WLAN_CFG_PRIV_M2S_FUNCTION_EXT_MASK},
    { "m2s_function_mask", WLAN_CFG_PRIV_M2S_FUNCTION_MASK },

    { "mcm_custom_function_mask", WLAN_CFG_PRIV_MCM_CUSTOM_FUNCTION_MASK},
    { "mcm_function_mask", WLAN_CFG_PRIV_MCM_FUNCTION_MASK },
    { "linkloss_threshold_fixed", WLAN_CFG_PRRIV_LINKLOSS_THRESHOLD_FIXED },

    { "aput_support_160m", WLAN_CFG_APUT_160M_ENABLE},
    { "radar_isr_forbid", WLAN_CFG_RADAR_ISR_FORBID},

    { "download_rate_limit_pps", WLAN_CFG_PRIV_DOWNLOAD_RATE_LIMIT_PPS },
#ifdef _PRE_WLAN_FEATURE_TXOPPS
    { "txopps_switch", WLAN_CFG_PRIV_TXOPPS_SWITCH },
#endif
    { "over_temper_protect_threshold",   WLAN_CFG_PRIV_OVER_TEMPER_PROTECT_THRESHOLD },
    { "over_temp_pro_enable",            WLAN_CFG_PRIV_OVER_TEMP_PRO_ENABLE },
    { "over_temp_pro_reduce_pwr_enable", WLAN_CFG_PRIV_OVER_TEMP_PRO_REDUCE_PWR_ENABLE },
    { "over_temp_pro_safe_th",           WLAN_CFG_PRIV_OVER_TEMP_PRO_SAFE_TH },
    { "over_temp_pro_over_th",           WLAN_CFG_PRIV_OVER_TEMP_PRO_OVER_TH },
    { "over_temp_pro_pa_off_th",         WLAN_CFG_PRIV_OVER_TEMP_PRO_PA_OFF_TH },

    { "dsss2ofdm_dbb_pwr_bo_val",   WLAN_DSSS2OFDM_DBB_PWR_BO_VAL },
    { "evm_fail_pll_reg_fix",       WLAN_CFG_PRIV_EVM_PLL_REG_FIX },
    { "voe_switch_mask",            WLAN_CFG_PRIV_VOE_SWITCH },
    { "11ax_switch_mask",           WLAN_CFG_PRIV_11AX_SWITCH },
    { "htc_switch_mask",            WLAN_CFG_PRIV_HTC_SWITCH },
    { "multi_bssid_switch_mask",    WLAN_CFG_PRIV_MULTI_BSSID_SWITCH},
    { "ac_priv_mask",               WLAN_CFG_PRIV_AC_SUSPEND},
    { "dyn_bypass_extlna_enable",   WLAN_CFG_PRIV_DYN_BYPASS_EXTLNA },
    { "ctrl_frame_tx_chain",        WLAN_CFG_PRIV_CTRL_FRAME_TX_CHAIN },
    { "upc_cali_code_for_18dBm_c0", WLAN_CFG_PRIV_CTRL_UPC_FOR_18DBM_CO },
    { "upc_cali_code_for_18dBm_c1", WLAN_CFG_PRIV_CTRL_UPC_FOR_18DBM_C1 },
    { "11b_double_chain_bo_pow",    WLAN_CFG_PRIV_CTRL_11B_DOUBLE_CHAIN_BO_POW },
    { "hcc_flowctrl_type",          WLAN_CFG_PRIV_HCC_FLOWCTRL_TYPE },
    { "lock_cpu_freq",              WLAN_CFG_PRIV_LOCK_CPU_FREQ },
    { "mbo_switch_mask",            WLAN_CFG_PRIV_MBO_SWITCH},
    { "dynamic_dbac_adjust_mask",   WLAN_CFG_PRIV_DYNAMIC_DBAC_SWITCH},
    { "dc_flowctl_switch",          WLAN_CFG_PRIV_DC_FLOWCTL_SWITCH },
    { "phy_cap_mask",               WLAN_CFG_PRIV_PHY_CAP_SWITCH},
    { "hal_ps_rssi_param",          WLAN_CFG_PRIV_HAL_PS_RSSI_PARAM},
    { "hal_ps_pps_param",           WLAN_CFG_PRIV_HAL_PS_PPS_PARAM},
    { "optimized_feature_mask",     WLAN_CFG_PRIV_OPTIMIZED_FEATURE_SWITCH},
    { "ddr_switch_mask",            WLAN_CFG_PRIV_DDR_SWITCH},

    { "fem_backoff_pow",              WLAN_CFG_PRIV_FEM_DELT_POW},
    { "tpc_adj_pow_start_idx_by_fem", WLAN_CFG_PRIV_FEM_ADJ_TPC_TBL_START_IDX},
    { "hiex_cap",                     WLAN_CFG_PRIV_HIEX_CAP},
    { "tx_switch",                    WLAN_CFG_PRIV_TX_SWITCH },
    { "ftm_cap",                      WLAN_CFG_PRIV_FTM_CAP},

    { OAL_PTR_NULL,                 0 }
};

/* kunpeng eeprom */
#ifdef _PRE_CONFIG_READ_DYNCALI_E2PROM
OAL_STATIC wlan_cfg_nv_map_handler g_wifi_nvram_cfg_handler[WLAN_CFG_DTS_NVRAM_END] = {
    {"WITXCCK",  "pa2gccka0",   HWIFI_CFG_NV_WITXNVCCK_NUMBER,       WLAN_CFG_DTS_NVRAM_RATIO_PA2GCCKA0, {0}, 0x100},
    {"WINVRAM",  "pa2ga0",      HWIFI_CFG_NV_WINVRAM_NUMBER,         WLAN_CFG_NVRAM_RATIO_PA2GA0,        {0}, 0x0},
    {"WITXLC0", "pa2g40a0",    HWIFI_CFG_NV_WITXL2G5G0_NUMBER,      WLAN_CFG_DTS_NVRAM_RATIO_PA2G40A0,  {0}, 0x400},
    {"WINVRAM",  "pa5ga0",      HWIFI_CFG_NV_WINVRAM_NUMBER,         WLAN_CFG_DTS_NVRAM_RATIO_PA5GA0,    {0}, 0x80},
    {"WITXCCK",  "pa2gccka1",   HWIFI_CFG_NV_WITXNVCCK_NUMBER,       WLAN_CFG_DTS_NVRAM_RATIO_PA2GCCKA1, {0}, 0x180},
    {"WITXRF1",  "pa2ga1",      HWIFI_CFG_NV_WITXNVC1_NUMBER,        WLAN_CFG_DTS_NVRAM_RATIO_PA2GA1,    {0}, 0x200},
    {"WITXLC1",  "pa2g40a1",    HWIFI_CFG_NV_WITXL2G5G1_NUMBER,      WLAN_CFG_DTS_NVRAM_RATIO_PA2G40A1,  {0}, 0x480},
    {"WITXRF1",  "pa5ga1",      HWIFI_CFG_NV_WITXNVC1_NUMBER,        WLAN_CFG_DTS_NVRAM_RATIO_PA5GA1,    {0}, 0x280},
    {"WIC0_5GLOW",  "pa5glowa0",  HWIFI_CFG_NV_WITXNVBWC0_NUMBER,    WLAN_CFG_DTS_NVRAM_RATIO_PA5GA0_LOW,  {0}, 0x300},
    {"WIC1_5GLOW",  "pa5glowa1",  HWIFI_CFG_NV_WITXNVBWC1_NUMBER,    WLAN_CFG_DTS_NVRAM_RATIO_PA5GA1_LOW,  {0}, 0x380},
    // DPN
    {"WIC0CCK",  "mf2gccka0",   HWIFI_CFG_NV_MUFREQ_CCK_C0_NUMBER,   WLAN_CFG_DTS_NVRAM_MUFREQ_2GCCK_C0,  {0}, 0x0},
    {"WC0OFDM",  "mf2ga0",      HWIFI_CFG_NV_MUFREQ_2G20_C0_NUMBER,  WLAN_CFG_DTS_NVRAM_MUFREQ_2G20_C0,   {0}, 0x0},
    {"C02G40M",  "mf2g40a0",    HWIFI_CFG_NV_MUFREQ_2G40_C0_NUMBER,  WLAN_CFG_DTS_NVRAM_MUFREQ_2G40_C0,   {0}, 0x0},
    {"WIC1CCK",  "mf2gccka1",   HWIFI_CFG_NV_MUFREQ_CCK_C1_NUMBER,   WLAN_CFG_DTS_NVRAM_MUFREQ_2GCCK_C1,  {0}, 0x0},
    {"WC1OFDM",  "mf2ga1",      HWIFI_CFG_NV_MUFREQ_2G20_C1_NUMBER,  WLAN_CFG_DTS_NVRAM_MUFREQ_2G20_C1,   {0}, 0x0},
    {"C12G40M",  "mf2g40a1",    HWIFI_CFG_NV_MUFREQ_2G40_C1_NUMBER,  WLAN_CFG_DTS_NVRAM_MUFREQ_2G40_C1,   {0}, 0x0},
    {"WIC0_5G160M", "dpn160c0", HWIFI_CFG_NV_MUFREQ_5G160_C0_NUMBER, WLAN_CFG_DTS_NVRAM_MUFREQ_5G160_C0, {0}, 0x500},
    {"WIC1_5G160M", "dpn160c1", HWIFI_CFG_NV_MUFREQ_5G160_C1_NUMBER, WLAN_CFG_DTS_NVRAM_MUFREQ_5G160_C1, {0}, 0x580},
};
#else
OAL_STATIC wlan_cfg_nv_map_handler g_wifi_nvram_cfg_handler[WLAN_CFG_DTS_NVRAM_END] = {
    { "WITXCCK",    "pa2gccka0", HWIFI_CFG_NV_WITXNVCCK_NUMBER,  WLAN_CFG_DTS_NVRAM_RATIO_PA2GCCKA0,  {0}},
    { "WINVRAM",    "pa2ga0",    HWIFI_CFG_NV_WINVRAM_NUMBER,    WLAN_CFG_NVRAM_RATIO_PA2GA0,         {0}},
    { "WITXLC0",    "pa2g40a0",  HWIFI_CFG_NV_WITXL2G5G0_NUMBER, WLAN_CFG_DTS_NVRAM_RATIO_PA2G40A0,   {0}},
    { "WINVRAM",    "pa5ga0",    HWIFI_CFG_NV_WINVRAM_NUMBER,    WLAN_CFG_DTS_NVRAM_RATIO_PA5GA0,     {0}},
    { "WITXCCK",    "pa2gccka1", HWIFI_CFG_NV_WITXNVCCK_NUMBER,  WLAN_CFG_DTS_NVRAM_RATIO_PA2GCCKA1,  {0}},
    { "WITXRF1",    "pa2ga1",    HWIFI_CFG_NV_WITXNVC1_NUMBER,   WLAN_CFG_DTS_NVRAM_RATIO_PA2GA1,     {0}},
    { "WITXLC1",    "pa2g40a1",  HWIFI_CFG_NV_WITXL2G5G1_NUMBER, WLAN_CFG_DTS_NVRAM_RATIO_PA2G40A1,   {0}},
    { "WITXRF1",    "pa5ga1",    HWIFI_CFG_NV_WITXNVC1_NUMBER,   WLAN_CFG_DTS_NVRAM_RATIO_PA5GA1,     {0}},
    { "W5GLOW0",    "pa5glowa0", HWIFI_CFG_NV_WITXNVBWC0_NUMBER, WLAN_CFG_DTS_NVRAM_RATIO_PA5GA0_LOW, {0}},
    { "W5GLOW1",    "pa5glowa1", HWIFI_CFG_NV_WITXNVBWC1_NUMBER, WLAN_CFG_DTS_NVRAM_RATIO_PA5GA1_LOW, {0}},
    // DPN
    { "WIC0CCK",     "mf2gccka0", HWIFI_CFG_NV_MUFREQ_CCK_C0_NUMBER,   WLAN_CFG_DTS_NVRAM_MUFREQ_2GCCK_C0, {0}},
    { "WC0OFDM",     "mf2ga0",    HWIFI_CFG_NV_MUFREQ_2G20_C0_NUMBER,  WLAN_CFG_DTS_NVRAM_MUFREQ_2G20_C0,  {0}},
    { "C02G40M",     "mf2g40a0",  HWIFI_CFG_NV_MUFREQ_2G40_C0_NUMBER,  WLAN_CFG_DTS_NVRAM_MUFREQ_2G40_C0,  {0}},
    { "WIC1CCK",     "mf2gccka1", HWIFI_CFG_NV_MUFREQ_CCK_C1_NUMBER,   WLAN_CFG_DTS_NVRAM_MUFREQ_2GCCK_C1, {0}},
    { "WC1OFDM",     "mf2ga1",    HWIFI_CFG_NV_MUFREQ_2G20_C1_NUMBER,  WLAN_CFG_DTS_NVRAM_MUFREQ_2G20_C1,  {0}},
    { "C12G40M",     "mf2g40a1",  HWIFI_CFG_NV_MUFREQ_2G40_C1_NUMBER,  WLAN_CFG_DTS_NVRAM_MUFREQ_2G40_C1,  {0}},
    { "W160MC0",     "dpn160c0",  HWIFI_CFG_NV_MUFREQ_5G160_C0_NUMBER, WLAN_CFG_DTS_NVRAM_MUFREQ_5G160_C0, {0}},
    { "W160MC1",     "dpn160c1",  HWIFI_CFG_NV_MUFREQ_5G160_C1_NUMBER, WLAN_CFG_DTS_NVRAM_MUFREQ_5G160_C1, {0}},
};
#endif

OAL_STATIC wlan_cfg_cmd g_wifi_config_cmds[] = {
    /* ROAM */
    { "roam_switch",         WLAN_CFG_INIT_ROAM_SWITCH },
    { "scan_orthogonal",     WLAN_CFG_INIT_SCAN_ORTHOGONAL },
    { "trigger_b",           WLAN_CFG_INIT_TRIGGER_B },
    { "trigger_a",           WLAN_CFG_INIT_TRIGGER_A },
    { "delta_b",             WLAN_CFG_INIT_DELTA_B },
    { "delta_a",             WLAN_CFG_INIT_DELTA_A },
    { "dense_env_trigger_b", WLAN_CFG_INIT_DENSE_ENV_TRIGGER_B },
    { "dense_env_trigger_a", WLAN_CFG_INIT_DENSE_ENV_TRIGGER_A },
    { "scenario_enable",     WLAN_CFG_INIT_SCENARIO_ENABLE },
    { "candidate_good_rssi", WLAN_CFG_INIT_CANDIDATE_GOOD_RSSI },
    { "candidate_good_num",  WLAN_CFG_INIT_CANDIDATE_GOOD_NUM },
    { "candidate_weak_num",  WLAN_CFG_INIT_CANDIDATE_WEAK_NUM },
    { "interval_variable",   WLAN_CFG_INIT_INTERVAL_VARIABLE },

    /* ���� */
    { "ampdu_tx_max_num",        WLAN_CFG_INIT_AMPDU_TX_MAX_NUM },
    { "used_mem_for_start",      WLAN_CFG_INIT_USED_MEM_FOR_START },
    { "used_mem_for_stop",       WLAN_CFG_INIT_USED_MEM_FOR_STOP },
    { "rx_ack_limit",            WLAN_CFG_INIT_RX_ACK_LIMIT },
    { "sdio_d2h_assemble_count", WLAN_CFG_INIT_SDIO_D2H_ASSEMBLE_COUNT },
    { "sdio_h2d_assemble_count", WLAN_CFG_INIT_SDIO_H2D_ASSEMBLE_COUNT },
    /* LINKLOSS */
    { "link_loss_threshold_bt",     WLAN_CFG_INIT_LINK_LOSS_THRESHOLD_BT },
    { "link_loss_threshold_dbac",   WLAN_CFG_INIT_LINK_LOSS_THRESHOLD_DBAC },
    { "link_loss_threshold_normal", WLAN_CFG_INIT_LINK_LOSS_THRESHOLD_NORMAL },
    /* �Զ���Ƶ */
    { "pss_threshold_level_0",  WLAN_CFG_INIT_PSS_THRESHOLD_LEVEL_0 },
    { "cpu_freq_limit_level_0", WLAN_CFG_INIT_CPU_FREQ_LIMIT_LEVEL_0 },
    { "ddr_freq_limit_level_0", WLAN_CFG_INIT_DDR_FREQ_LIMIT_LEVEL_0 },
    { "pss_threshold_level_1",  WLAN_CFG_INIT_PSS_THRESHOLD_LEVEL_1 },
    { "cpu_freq_limit_level_1", WLAN_CFG_INIT_CPU_FREQ_LIMIT_LEVEL_1 },
    { "ddr_freq_limit_level_1", WLAN_CFG_INIT_DDR_FREQ_LIMIT_LEVEL_1 },
    { "pss_threshold_level_2",  WLAN_CFG_INIT_PSS_THRESHOLD_LEVEL_2 },
    { "cpu_freq_limit_level_2", WLAN_CFG_INIT_CPU_FREQ_LIMIT_LEVEL_2 },
    { "ddr_freq_limit_level_2", WLAN_CFG_INIT_DDR_FREQ_LIMIT_LEVEL_2 },
    { "pss_threshold_level_3",  WLAN_CFG_INIT_PSS_THRESHOLD_LEVEL_3 },
    { "cpu_freq_limit_level_3", WLAN_CFG_INIT_CPU_FREQ_LIMIT_LEVEL_3 },
    { "ddr_freq_limit_level_3", WLAN_CFG_INIT_DDR_FREQ_LIMIT_LEVEL_3 },
    { "device_type_level_0",    WLAN_CFG_INIT_DEVICE_TYPE_LEVEL_0 },
    { "device_type_level_1",    WLAN_CFG_INIT_DEVICE_TYPE_LEVEL_1 },
    { "device_type_level_2",    WLAN_CFG_INIT_DEVICE_TYPE_LEVEL_2 },
    { "device_type_level_3",    WLAN_CFG_INIT_DEVICE_TYPE_LEVEL_3 },
    /* �޸�DMA latency,����cpu��������idle state */
    { "lock_dma_latency",  WLAN_CFG_PRIV_DMA_LATENCY },
    { "lock_cpu_th_high",           WLAN_CFG_PRIV_LOCK_CPU_TH_HIGH },
    { "lock_cpu_th_low",            WLAN_CFG_PRIV_LOCK_CPU_TH_LOW },
    /* �շ��ж϶�̬��� */
    { "irq_affinity",       WLAN_CFG_INIT_IRQ_AFFINITY },
    { "cpu_id_th_low",      WLAN_CFG_INIT_IRQ_TH_LOW },
    { "cpu_id_th_high",     WLAN_CFG_INIT_IRQ_TH_HIGH },
    { "cpu_id_pps_th_low",  WLAN_CFG_INIT_IRQ_PPS_TH_LOW },
    { "cpu_id_pps_th_high", WLAN_CFG_INIT_IRQ_PPS_TH_HIGH },
#ifdef _PRE_WLAN_FEATURE_AMPDU_TX_HW
    /* Ӳ���ۺ�ʹ�� */
    { "hw_ampdu",      WLAN_CFG_INIT_HW_AMPDU },
    { "hw_ampdu_th_l", WLAN_CFG_INIT_HW_AMPDU_TH_LOW },
    { "hw_ampdu_th_h", WLAN_CFG_INIT_HW_AMPDU_TH_HIGH },
#endif
#ifdef _PRE_WLAN_FEATURE_MULTI_NETBUF_AMSDU
    { "tx_amsdu_ampdu",      WLAN_CFG_INIT_AMPDU_AMSDU_SKB },
    { "tx_amsdu_ampdu_th_l", WLAN_CFG_INIT_AMSDU_AMPDU_TH_LOW },
    { "tx_amsdu_ampdu_th_m", WLAN_CFG_INIT_AMSDU_AMPDU_TH_MIDDLE },
    { "tx_amsdu_ampdu_th_h", WLAN_CFG_INIT_AMSDU_AMPDU_TH_HIGH },
#endif
#ifdef _PRE_WLAN_TCP_OPT
    { "en_tcp_ack_filter",      WLAN_CFG_INIT_TCP_ACK_FILTER },
    { "rx_tcp_ack_filter_th_l", WLAN_CFG_INIT_TCP_ACK_FILTER_TH_LOW },
    { "rx_tcp_ack_filter_th_h", WLAN_CFG_INIT_TCP_ACK_FILTER_TH_HIGH },
#endif

    { "small_amsdu_switch",   WLAN_CFG_INIT_TX_SMALL_AMSDU },
    { "small_amsdu_th_h",     WLAN_CFG_INIT_SMALL_AMSDU_HIGH },
    { "small_amsdu_th_l",     WLAN_CFG_INIT_SMALL_AMSDU_LOW },
    { "small_amsdu_pps_th_h", WLAN_CFG_INIT_SMALL_AMSDU_PPS_HIGH },
    { "small_amsdu_pps_th_l", WLAN_CFG_INIT_SMALL_AMSDU_PPS_LOW },

    { "tcp_ack_buf_switch",    WLAN_CFG_INIT_TX_TCP_ACK_BUF },
    { "tcp_ack_buf_th_h",      WLAN_CFG_INIT_TCP_ACK_BUF_HIGH },
    { "tcp_ack_buf_th_l",      WLAN_CFG_INIT_TCP_ACK_BUF_LOW },
    { "tcp_ack_buf_th_h_40M",  WLAN_CFG_INIT_TCP_ACK_BUF_HIGH_40M },
    { "tcp_ack_buf_th_l_40M",  WLAN_CFG_INIT_TCP_ACK_BUF_LOW_40M },
    { "tcp_ack_buf_th_h_80M",  WLAN_CFG_INIT_TCP_ACK_BUF_HIGH_80M },
    { "tcp_ack_buf_th_l_80M",  WLAN_CFG_INIT_TCP_ACK_BUF_LOW_80M },
    { "tcp_ack_buf_th_h_160M", WLAN_CFG_INIT_TCP_ACK_BUF_HIGH_160M },
    { "tcp_ack_buf_th_l_160M", WLAN_CFG_INIT_TCP_ACK_BUF_LOW_160M },

    { "tcp_ack_buf_userctl_switch", WLAN_CFG_INIT_TX_TCP_ACK_BUF_USERCTL },
    { "tcp_ack_buf_userctl_h",      WLAN_CFG_INIT_TCP_ACK_BUF_USERCTL_HIGH },
    { "tcp_ack_buf_userctl_l",      WLAN_CFG_INIT_TCP_ACK_BUF_USERCTL_LOW },

    { "dyn_bypass_extlna_th_switch", WLAN_CFG_INIT_RX_DYN_BYPASS_EXTLNA },
    { "dyn_bypass_extlna_th_h",      WLAN_CFG_INIT_RX_DYN_BYPASS_EXTLNA_HIGH },
    { "dyn_bypass_extlna_th_l",      WLAN_CFG_INIT_RX_DYN_BYPASS_EXTLNA_LOW },

    { "rx_ampdu_amsdu", WLAN_CFG_INIT_RX_AMPDU_AMSDU_SKB },
    { "rx_ampdu_bitmap", WLAN_CFG_INIT_RX_AMPDU_BITMAP },

    /* �͹��� */
    { "powermgmt_switch", WLAN_CFG_INIT_POWERMGMT_SWITCH },

    { "ps_mode",                        WLAN_CFG_INIT_PS_MODE },
    { "min_fast_ps_idle",               WLAN_CFG_INIT_MIN_FAST_PS_IDLE },
    { "max_fast_ps_idle",               WLAN_CFG_INIT_MAX_FAST_PS_IDLE },
    { "auto_fast_ps_thresh_screen_on",  WLAN_CFG_INIT_AUTO_FAST_PS_THRESH_SCREENON },
    { "auto_fast_ps_thresh_screen_off", WLAN_CFG_INIT_AUTO_FAST_PS_THRESH_SCREENOFF },
    /* ��ά�ɲ� */
    { "loglevel", WLAN_CFG_INIT_LOGLEVEL },
    /* 2G RFǰ�˲��� */
    { "rf_rx_insertion_loss_2g_b1", WLAN_CFG_INIT_RF_RX_INSERTION_LOSS_2G_BAND1 },
    { "rf_rx_insertion_loss_2g_b2", WLAN_CFG_INIT_RF_RX_INSERTION_LOSS_2G_BAND2 },
    { "rf_rx_insertion_loss_2g_b3", WLAN_CFG_INIT_RF_RX_INSERTION_LOSS_2G_BAND3 },
    /* 5G RFǰ�˲��� */
    { "rf_rx_insertion_loss_5g_b1", WLAN_CFG_INIT_RF_RX_INSERTION_LOSS_5G_BAND1 },
    { "rf_rx_insertion_loss_5g_b2", WLAN_CFG_INIT_RF_RX_INSERTION_LOSS_5G_BAND2 },
    { "rf_rx_insertion_loss_5g_b3", WLAN_CFG_INIT_RF_RX_INSERTION_LOSS_5G_BAND3 },
    { "rf_rx_insertion_loss_5g_b4", WLAN_CFG_INIT_RF_RX_INSERTION_LOSS_5G_BAND4 },
    { "rf_rx_insertion_loss_5g_b5", WLAN_CFG_INIT_RF_RX_INSERTION_LOSS_5G_BAND5 },
    { "rf_rx_insertion_loss_5g_b6", WLAN_CFG_INIT_RF_RX_INSERTION_LOSS_5G_BAND6 },
    { "rf_rx_insertion_loss_5g_b7", WLAN_CFG_INIT_RF_RX_INSERTION_LOSS_5G_BAND7 },
    /* ���ڶ��ƻ�����PWR RFֵ��ƫ�� */
    { "rf_line_rf_pwr_ref_rssi_db_2g_c0_mult4",  WLAN_CFG_INIT_RF_PWR_REF_RSSI_2G_C0_MULT4 },
    { "rf_line_rf_pwr_ref_rssi_db_2g_c1_mult4",  WLAN_CFG_INIT_RF_PWR_REF_RSSI_2G_C1_MULT4 },
    { "rf_line_rf_pwr_ref_rssi_db_5g_c0_mult4",  WLAN_CFG_INIT_RF_PWR_REF_RSSI_5G_C0_MULT4 },
    { "rf_line_rf_pwr_ref_rssi_db_5g_c1_mult4",  WLAN_CFG_INIT_RF_PWR_REF_RSSI_5G_C1_MULT4 },

    {"rf_rssi_amend_2g_c0", WLAN_CFG_INIT_RF_AMEND_RSSI_2G_C0},
    {"rf_rssi_amend_2g_c1", WLAN_CFG_INIT_RF_AMEND_RSSI_2G_C1},
    {"rf_rssi_amend_5g_c0", WLAN_CFG_INIT_RF_AMEND_RSSI_5G_C0},
    {"rf_rssi_amend_5g_c1", WLAN_CFG_INIT_RF_AMEND_RSSI_5G_C1},

    /* fem */
    { "rf_lna_bypass_gain_db_2g", WLAN_CFG_INIT_RF_LNA_BYPASS_GAIN_DB_2G },
    { "rf_lna_gain_db_2g",        WLAN_CFG_INIT_RF_LNA_GAIN_DB_2G },
    { "rf_pa_db_b0_2g",           WLAN_CFG_INIT_RF_PA_GAIN_DB_B0_2G },
    { "rf_pa_db_b1_2g",           WLAN_CFG_INIT_RF_PA_GAIN_DB_B1_2G },
    { "rf_pa_db_lvl_2g",          WLAN_CFG_INIT_RF_PA_GAIN_LVL_2G },
    { "ext_switch_isexist_2g",    WLAN_CFG_INIT_EXT_SWITCH_ISEXIST_2G },
    { "ext_pa_isexist_2g",        WLAN_CFG_INIT_EXT_PA_ISEXIST_2G },
    { "ext_lna_isexist_2g",       WLAN_CFG_INIT_EXT_LNA_ISEXIST_2G },
    { "lna_on2off_time_ns_2g",    WLAN_CFG_INIT_LNA_ON2OFF_TIME_NS_2G },
    { "lna_off2on_time_ns_2g",    WLAN_CFG_INIT_LNA_OFF2ON_TIME_NS_2G },
    { "rf_lna_bypass_gain_db_5g", WLAN_CFG_INIT_RF_LNA_BYPASS_GAIN_DB_5G },
    { "rf_lna_gain_db_5g",        WLAN_CFG_INIT_RF_LNA_GAIN_DB_5G },
    { "rf_pa_db_b0_5g",           WLAN_CFG_INIT_RF_PA_GAIN_DB_B0_5G },
    { "rf_pa_db_b1_5g",           WLAN_CFG_INIT_RF_PA_GAIN_DB_B1_5G },
    { "rf_pa_db_lvl_5g",          WLAN_CFG_INIT_RF_PA_GAIN_LVL_5G },
    { "ext_switch_isexist_5g",    WLAN_CFG_INIT_EXT_SWITCH_ISEXIST_5G },
    { "ext_pa_isexist_5g",        WLAN_CFG_INIT_EXT_PA_ISEXIST_5G },
    { "ext_lna_isexist_5g",       WLAN_CFG_INIT_EXT_LNA_ISEXIST_5G },
    { "lna_on2off_time_ns_5g",    WLAN_CFG_INIT_LNA_ON2OFF_TIME_NS_5G },
    { "lna_off2on_time_ns_5g",    WLAN_CFG_INIT_LNA_OFF2ON_TIME_NS_5G },
    /* SCAN */
    { "random_mac_addr_scan", WLAN_CFG_INIT_RANDOM_MAC_ADDR_SCAN },
    /* 11AC2G */
    { "11ac2g_enable",        WLAN_CFG_INIT_11AC2G_ENABLE },
    { "disable_capab_2ght40", WLAN_CFG_INIT_DISABLE_CAPAB_2GHT40 },
    { "dual_antenna_enable",  WLAN_CFG_INIT_DUAL_ANTENNA_ENABLE }, /* ˫���߿��� */

    { "probe_resp_mode", WLAN_CFG_INIT_PROBE_RESP_MODE},
    { "miracast_enable", WLAN_CFG_INIT_MIRACAST_SINK_ENABLE},
    { "reduce_miracast_log", WLAN_CFG_INIT_REDUCE_MIRACAST_LOG},
    { "core_bind_cap", WLAN_CFG_INIT_CORE_BIND_CAP},
    {"only_fast_mode", WLAN_CFG_INIT_FAST_MODE},

    /* sta keepalive cnt th */
    { "sta_keepalive_cnt_th", WLAN_CFG_INIT_STA_KEEPALIVE_CNT_TH }, /* ��̬����У׼ */

    { "far_dist_pow_gain_switch", WLAN_CFG_INIT_FAR_DIST_POW_GAIN_SWITCH },
    { "far_dist_dsss_scale_promote_switch", WLAN_CFG_INIT_FAR_DIST_DSSS_SCALE_PROMOTE_SWITCH },
    { "chann_radio_cap", WLAN_CFG_INIT_CHANN_RADIO_CAP },

    { "lte_gpio_check_switch",    WLAN_CFG_LTE_GPIO_CHECK_SWITCH }, /* lte?????? */
    { "ism_priority",             WLAN_ATCMDSRV_ISM_PRIORITY },
    { "lte_rx",                   WLAN_ATCMDSRV_LTE_RX },
    { "lte_tx",                   WLAN_ATCMDSRV_LTE_TX },
    { "lte_inact",                WLAN_ATCMDSRV_LTE_INACT },
    { "ism_rx_act",               WLAN_ATCMDSRV_ISM_RX_ACT },
    { "bant_pri",                 WLAN_ATCMDSRV_BANT_PRI },
    { "bant_status",              WLAN_ATCMDSRV_BANT_STATUS },
    { "want_pri",                 WLAN_ATCMDSRV_WANT_PRI },
    { "want_status",              WLAN_ATCMDSRV_WANT_STATUS },
    { "tx5g_upc_mix_gain_ctrl_1", WLAN_TX5G_UPC_MIX_GAIN_CTRL_1 },
    { "tx5g_upc_mix_gain_ctrl_2", WLAN_TX5G_UPC_MIX_GAIN_CTRL_2 },
    { "tx5g_upc_mix_gain_ctrl_3", WLAN_TX5G_UPC_MIX_GAIN_CTRL_3 },
    { "tx5g_upc_mix_gain_ctrl_4", WLAN_TX5G_UPC_MIX_GAIN_CTRL_4 },
    { "tx5g_upc_mix_gain_ctrl_5", WLAN_TX5G_UPC_MIX_GAIN_CTRL_5 },
    { "tx5g_upc_mix_gain_ctrl_6", WLAN_TX5G_UPC_MIX_GAIN_CTRL_6 },
    { "tx5g_upc_mix_gain_ctrl_7", WLAN_TX5G_UPC_MIX_GAIN_CTRL_7 },
    /* ���ƻ�RF����PAƫ�üĴ��� */
    { "tx2g_pa_gate_236", WLAN_TX2G_PA_GATE_VCTL_REG236 },
    { "tx2g_pa_gate_237", WLAN_TX2G_PA_GATE_VCTL_REG237 },
    { "tx2g_pa_gate_238", WLAN_TX2G_PA_GATE_VCTL_REG238 },
    { "tx2g_pa_gate_239", WLAN_TX2G_PA_GATE_VCTL_REG239 },
    { "tx2g_pa_gate_240", WLAN_TX2G_PA_GATE_VCTL_REG240 },
    { "tx2g_pa_gate_241", WLAN_TX2G_PA_GATE_VCTL_REG241 },
    { "tx2g_pa_gate_242", WLAN_TX2G_PA_GATE_VCTL_REG242 },
    { "tx2g_pa_gate_243", WLAN_TX2G_PA_GATE_VCTL_REG243 },
    { "tx2g_pa_gate_244", WLAN_TX2G_PA_GATE_VCTL_REG244 },

    { "tx2g_pa_gate_253",       WLAN_TX2G_PA_VRECT_GATE_THIN_REG253 },
    { "tx2g_pa_gate_254",       WLAN_TX2G_PA_VRECT_GATE_THIN_REG254 },
    { "tx2g_pa_gate_255",       WLAN_TX2G_PA_VRECT_GATE_THIN_REG255 },
    { "tx2g_pa_gate_256",       WLAN_TX2G_PA_VRECT_GATE_THIN_REG256 },
    { "tx2g_pa_gate_257",       WLAN_TX2G_PA_VRECT_GATE_THIN_REG257 },
    { "tx2g_pa_gate_258",       WLAN_TX2G_PA_VRECT_GATE_THIN_REG258 },
    { "tx2g_pa_gate_259",       WLAN_TX2G_PA_VRECT_GATE_THIN_REG259 },
    { "tx2g_pa_gate_260",       WLAN_TX2G_PA_VRECT_GATE_THIN_REG260 },
    { "tx2g_pa_gate_261",       WLAN_TX2G_PA_VRECT_GATE_THIN_REG261 },
    { "tx2g_pa_gate_262",       WLAN_TX2G_PA_VRECT_GATE_THIN_REG262 },
    { "tx2g_pa_gate_263",       WLAN_TX2G_PA_VRECT_GATE_THIN_REG263 },
    { "tx2g_pa_gate_264",       WLAN_TX2G_PA_VRECT_GATE_THIN_REG264 },
    { "tx2g_pa_gate_265",       WLAN_TX2G_PA_VRECT_GATE_THIN_REG265 },
    { "tx2g_pa_gate_266",       WLAN_TX2G_PA_VRECT_GATE_THIN_REG266 },
    { "tx2g_pa_gate_267",       WLAN_TX2G_PA_VRECT_GATE_THIN_REG267 },
    { "tx2g_pa_gate_268",       WLAN_TX2G_PA_VRECT_GATE_THIN_REG268 },
    { "tx2g_pa_gate_269",       WLAN_TX2G_PA_VRECT_GATE_THIN_REG269 },
    { "tx2g_pa_gate_270",       WLAN_TX2G_PA_VRECT_GATE_THIN_REG270 },
    { "tx2g_pa_gate_271",       WLAN_TX2G_PA_VRECT_GATE_THIN_REG271 },
    { "tx2g_pa_gate_272",       WLAN_TX2G_PA_VRECT_GATE_THIN_REG272 },
    { "tx2g_pa_gate_273",       WLAN_TX2G_PA_VRECT_GATE_THIN_REG273 },
    { "tx2g_pa_gate_274",       WLAN_TX2G_PA_VRECT_GATE_THIN_REG274 },
    { "tx2g_pa_gate_275",       WLAN_TX2G_PA_VRECT_GATE_THIN_REG275 },
    { "tx2g_pa_gate_276",       WLAN_TX2G_PA_VRECT_GATE_THIN_REG276 },
    { "tx2g_pa_gate_277",       WLAN_TX2G_PA_VRECT_GATE_THIN_REG277 },
    { "tx2g_pa_gate_278",       WLAN_TX2G_PA_VRECT_GATE_THIN_REG278 },
    { "tx2g_pa_gate_279",       WLAN_TX2G_PA_VRECT_GATE_THIN_REG279 },
    { "tx2g_pa_gate_280_band1", WLAN_TX2G_PA_VRECT_GATE_THIN_REG280_BAND1 },
    { "tx2g_pa_gate_281",       WLAN_TX2G_PA_VRECT_GATE_THIN_REG281 },
    { "tx2g_pa_gate_282",       WLAN_TX2G_PA_VRECT_GATE_THIN_REG282 },
    { "tx2g_pa_gate_283",       WLAN_TX2G_PA_VRECT_GATE_THIN_REG283 },
    { "tx2g_pa_gate_284",       WLAN_TX2G_PA_VRECT_GATE_THIN_REG284 },
    { "tx2g_pa_gate_280_band2", WLAN_TX2G_PA_VRECT_GATE_THIN_REG280_BAND2 },
    { "tx2g_pa_gate_280_band3", WLAN_TX2G_PA_VRECT_GATE_THIN_REG280_BAND3 },
    { "delta_cca_ed_high_20th_2g", WLAN_CFG_INIT_DELTA_CCA_ED_HIGH_20TH_2G },
    { "delta_cca_ed_high_40th_2g", WLAN_CFG_INIT_DELTA_CCA_ED_HIGH_40TH_2G },
    { "delta_cca_ed_high_20th_5g", WLAN_CFG_INIT_DELTA_CCA_ED_HIGH_20TH_5G },
    { "delta_cca_ed_high_40th_5g", WLAN_CFG_INIT_DELTA_CCA_ED_HIGH_40TH_5G },
    { "delta_cca_ed_high_80th_5g", WLAN_CFG_INIT_DELTA_CCA_ED_HIGH_80TH_5G },
    { "voe_switch_mask",           WLAN_CFG_INIT_VOE_SWITCH },
    { "11ax_switch_mask",          WLAN_CFG_INIT_11AX_SWITCH },
    { "htc_switch_mask",           WLAN_CFG_INIT_HTC_SWITCH },
    { "multi_bssid_switch_mask",   WLAN_CFG_INIT_MULTI_BSSID_SWITCH},
    /* ldac m2s rssi */
    { "ldac_threshold_m2s", WLAN_CFG_INIT_LDAC_THRESHOLD_M2S },
    { "ldac_threshold_s2m", WLAN_CFG_INIT_LDAC_THRESHOLD_S2M },
    /* btcoex mcm rssi */
    { "btcoex_threshold_mcm_down", WLAN_CFG_INIT_BTCOEX_THRESHOLD_MCM_DOWN },
    { "btcoex_threshold_mcm_up",   WLAN_CFG_INIT_BTCOEX_THRESHOLD_MCM_UP },
#ifdef _PRE_WLAN_FEATURE_NRCOEX
    /* 5g nr coex */
    {"nrcoex_enable",                   WLAN_CFG_INIT_NRCOEX_ENABLE},
    {"nrcoex_rule0_freq",               WLAN_CFG_INIT_NRCOEX_RULE0_FREQ},
    {"nrcoex_rule0_40m_20m_gap0",       WLAN_CFG_INIT_NRCOEX_RULE0_40M_20M_GAP0},
    {"nrcoex_rule0_160m_80m_gap0",      WLAN_CFG_INIT_NRCOEX_RULE0_160M_80M_GAP0},
    {"nrcoex_rule0_40m_20m_gap1",       WLAN_CFG_INIT_NRCOEX_RULE0_40M_20M_GAP1},
    {"nrcoex_rule0_160m_80m_gap1",      WLAN_CFG_INIT_NRCOEX_RULE0_160M_80M_GAP1},
    {"nrcoex_rule0_40m_20m_gap2",       WLAN_CFG_INIT_NRCOEX_RULE0_40M_20M_GAP2},
    {"nrcoex_rule0_160m_80m_gap2",      WLAN_CFG_INIT_NRCOEX_RULE0_160M_80M_GAP2},
    {"nrcoex_rule0_smallgap0_act",      WLAN_CFG_INIT_NRCOEX_RULE0_SMALLGAP0_ACT},
    {"nrcoex_rule0_gap01_act",          WLAN_CFG_INIT_NRCOEX_RULE0_GAP01_ACT},
    {"nrcoex_rule0_gap12_act",          WLAN_CFG_INIT_NRCOEX_RULE0_GAP12_ACT},
    {"nrcoex_rule0_rxslot_rssi",        WLAN_CFG_INIT_NRCOEX_RULE0_RXSLOT_RSSI},
    {"nrcoex_rule1_freq",               WLAN_CFG_INIT_NRCOEX_RULE1_FREQ},
    {"nrcoex_rule1_40m_20m_gap0",       WLAN_CFG_INIT_NRCOEX_RULE1_40M_20M_GAP0},
    {"nrcoex_rule1_160m_80m_gap0",      WLAN_CFG_INIT_NRCOEX_RULE1_160M_80M_GAP0},
    {"nrcoex_rule1_40m_20m_gap1",       WLAN_CFG_INIT_NRCOEX_RULE1_40M_20M_GAP1},
    {"nrcoex_rule1_160m_80m_gap1",      WLAN_CFG_INIT_NRCOEX_RULE1_160M_80M_GAP1},
    {"nrcoex_rule1_40m_20m_gap2",       WLAN_CFG_INIT_NRCOEX_RULE1_40M_20M_GAP2},
    {"nrcoex_rule1_160m_80m_gap2",      WLAN_CFG_INIT_NRCOEX_RULE1_160M_80M_GAP2},
    {"nrcoex_rule1_smallgap0_act",      WLAN_CFG_INIT_NRCOEX_RULE1_SMALLGAP0_ACT},
    {"nrcoex_rule1_gap01_act",          WLAN_CFG_INIT_NRCOEX_RULE1_GAP01_ACT},
    {"nrcoex_rule1_gap12_act",          WLAN_CFG_INIT_NRCOEX_RULE1_GAP12_ACT},
    {"nrcoex_rule1_rxslot_rssi",        WLAN_CFG_INIT_NRCOEX_RULE1_RXSLOT_RSSI},
    {"nrcoex_rule2_freq",               WLAN_CFG_INIT_NRCOEX_RULE2_FREQ},
    {"nrcoex_rule2_40m_20m_gap0",       WLAN_CFG_INIT_NRCOEX_RULE2_40M_20M_GAP0},
    {"nrcoex_rule2_160m_80m_gap0",      WLAN_CFG_INIT_NRCOEX_RULE2_160M_80M_GAP0},
    {"nrcoex_rule2_40m_20m_gap1",       WLAN_CFG_INIT_NRCOEX_RULE2_40M_20M_GAP1},
    {"nrcoex_rule2_160m_80m_gap1",      WLAN_CFG_INIT_NRCOEX_RULE2_160M_80M_GAP1},
    {"nrcoex_rule2_40m_20m_gap2",       WLAN_CFG_INIT_NRCOEX_RULE2_40M_20M_GAP2},
    {"nrcoex_rule2_160m_80m_gap2",      WLAN_CFG_INIT_NRCOEX_RULE2_160M_80M_GAP2},
    {"nrcoex_rule2_smallgap0_act",      WLAN_CFG_INIT_NRCOEX_RULE2_SMALLGAP0_ACT},
    {"nrcoex_rule2_gap01_act",          WLAN_CFG_INIT_NRCOEX_RULE2_GAP01_ACT},
    {"nrcoex_rule2_gap12_act",          WLAN_CFG_INIT_NRCOEX_RULE2_GAP12_ACT},
    {"nrcoex_rule2_rxslot_rssi",        WLAN_CFG_INIT_NRCOEX_RULE2_RXSLOT_RSSI},
    {"nrcoex_rule3_freq",               WLAN_CFG_INIT_NRCOEX_RULE3_FREQ},
    {"nrcoex_rule3_40m_20m_gap0",       WLAN_CFG_INIT_NRCOEX_RULE3_40M_20M_GAP0},
    {"nrcoex_rule3_160m_80m_gap0",      WLAN_CFG_INIT_NRCOEX_RULE3_160M_80M_GAP0},
    {"nrcoex_rule3_40m_20m_gap1",       WLAN_CFG_INIT_NRCOEX_RULE3_40M_20M_GAP1},
    {"nrcoex_rule3_160m_80m_gap1",      WLAN_CFG_INIT_NRCOEX_RULE3_160M_80M_GAP1},
    {"nrcoex_rule3_40m_20m_gap2",       WLAN_CFG_INIT_NRCOEX_RULE3_40M_20M_GAP2},
    {"nrcoex_rule3_160m_80m_gap2",      WLAN_CFG_INIT_NRCOEX_RULE3_160M_80M_GAP2},
    {"nrcoex_rule3_smallgap0_act",      WLAN_CFG_INIT_NRCOEX_RULE3_SMALLGAP0_ACT},
    {"nrcoex_rule3_gap01_act",          WLAN_CFG_INIT_NRCOEX_RULE3_GAP01_ACT},
    {"nrcoex_rule3_gap12_act",          WLAN_CFG_INIT_NRCOEX_RULE3_GAP12_ACT},
    {"nrcoex_rule3_rxslot_rssi",        WLAN_CFG_INIT_NRCOEX_RULE3_RXSLOT_RSSI},
#endif
#ifdef _PRE_WLAN_FEATURE_MBO
    {"mbo_switch_mask",                 WLAN_CFG_INIT_MBO_SWITCH},
#endif
    { "dynamic_dbac_adjust_mask",       WLAN_CFG_INIT_DYNAMIC_DBAC_SWITCH},
    { "ddr_freq",                       WLAN_CFG_INIT_DDR_FREQ},
    { "hiex_cap",                       WLAN_CFG_INIT_HIEX_CAP},
    { "ftm_cap",                        WLAN_CFG_INIT_FTM_CAP},
    { "rf_filter_narrow_rssi_amend_2g_c0", WLAN_CFG_INIT_RF_FILTER_NARROW_RSSI_AMEND_2G_C0},
    { "rf_filter_narrow_rssi_amend_2g_c1", WLAN_CFG_INIT_RF_FILTER_NARROW_RSSI_AMEND_2G_C1},
    { "rf_filter_narrow_rssi_amend_5g_c0", WLAN_CFG_INIT_RF_FILTER_NARROW_RSSI_AMEND_5G_C0},
    { "rf_filter_narrow_rssi_amend_5g_c1", WLAN_CFG_INIT_RF_FILTER_NARROW_RSSI_AMEND_5G_C1},

    { OAL_PTR_NULL,         0 }
};

OAL_STATIC wlan_cfg_cmd g_nvram_config_ini[NVRAM_PARAMS_PWR_INDEX_BUTT] = {
    { "nvram_params0",  NVRAM_PARAMS_INDEX_0 },
    { "nvram_params1",  NVRAM_PARAMS_INDEX_1 },
    { "nvram_params2",  NVRAM_PARAMS_INDEX_2 },
    { "nvram_params3",  NVRAM_PARAMS_INDEX_3 },
    { "nvram_params4",  NVRAM_PARAMS_INDEX_4 },
    { "nvram_params5",  NVRAM_PARAMS_INDEX_5 },
    { "nvram_params6",  NVRAM_PARAMS_INDEX_6 },
    { "nvram_params7",  NVRAM_PARAMS_INDEX_7 },
    { "nvram_params8",  NVRAM_PARAMS_INDEX_8 },
    { "nvram_params9",  NVRAM_PARAMS_INDEX_9 },
    { "nvram_params10", NVRAM_PARAMS_INDEX_10 },
    { "nvram_params11", NVRAM_PARAMS_INDEX_11 },
    { "nvram_params12", NVRAM_PARAMS_INDEX_12 },
    { "nvram_params13", NVRAM_PARAMS_INDEX_13 },
    { "nvram_params14", NVRAM_PARAMS_INDEX_14 },
    { "nvram_params15", NVRAM_PARAMS_INDEX_15 },
    { "nvram_params16", NVRAM_PARAMS_INDEX_16 },
    { "nvram_params17", NVRAM_PARAMS_INDEX_17 },
    { "nvram_params59", NVRAM_PARAMS_INDEX_DPD_0 },
    { "nvram_params60", NVRAM_PARAMS_INDEX_DPD_1 },
    { "nvram_params61", NVRAM_PARAMS_INDEX_DPD_2 },
    /* 11B & OFDM delta power */
    { "nvram_params62", NVRAM_PARAMS_INDEX_11B_OFDM_DELT_POW },
    /* 5G cali upper upc limit */
    { "nvram_params63", NVRAM_PARAMS_INDEX_IQ_MAX_UPC },
    /* 2G low pow amend */
    { "nvram_params64",                  NVRAM_PARAMS_INDEX_2G_LOW_POW_AMEND },
    { OAL_PTR_NULL,                      NVRAM_PARAMS_TXPWR_INDEX_BUTT },
    { "nvram_max_txpwr_base_2p4g",       NVRAM_PARAMS_INDEX_19 },
    { "nvram_max_txpwr_base_5g",         NVRAM_PARAMS_INDEX_20 },
    { "nvram_max_txpwr_base_2p4g_slave", NVRAM_PARAMS_INDEX_21 },
    { "nvram_max_txpwr_base_5g_slave",   NVRAM_PARAMS_INDEX_22 },
    { OAL_PTR_NULL,                      NVRAM_PARAMS_BASE_INDEX_BUTT },
    { "nvram_delt_max_base_txpwr",       NVRAM_PARAMS_INDEX_DELT_BASE_POWER_23 },
    { OAL_PTR_NULL,                      NVRAM_PARAMS_INDEX_24_RSV },
    /* FCC */
    { "fcc_side_band_txpwr_limit_5g_20m_0", NVRAM_PARAMS_INDEX_25 },
    { "fcc_side_band_txpwr_limit_5g_20m_1", NVRAM_PARAMS_INDEX_26 },
    { "fcc_side_band_txpwr_limit_5g_40m_0", NVRAM_PARAMS_INDEX_27 },
    { "fcc_side_band_txpwr_limit_5g_40m_1", NVRAM_PARAMS_INDEX_28 },
    { "fcc_side_band_txpwr_limit_5g_80m_0", NVRAM_PARAMS_INDEX_29 },
    { "fcc_side_band_txpwr_limit_5g_80m_1", NVRAM_PARAMS_INDEX_30 },
    { "fcc_side_band_txpwr_limit_5g_160m",  NVRAM_PARAMS_INDEX_31 },
    { "fcc_side_band_txpwr_limit_24g_ch1",  NVRAM_PARAMS_INDEX_32 },
    { "fcc_side_band_txpwr_limit_24g_ch2",  NVRAM_PARAMS_INDEX_33 },
    { "fcc_side_band_txpwr_limit_24g_ch3",  NVRAM_PARAMS_INDEX_34 },
    { "fcc_side_band_txpwr_limit_24g_ch4",  NVRAM_PARAMS_INDEX_35 },
    { "fcc_side_band_txpwr_limit_24g_ch5",  NVRAM_PARAMS_INDEX_36 },
    { "fcc_side_band_txpwr_limit_24g_ch6",  NVRAM_PARAMS_INDEX_37 },
    { "fcc_side_band_txpwr_limit_24g_ch7",  NVRAM_PARAMS_INDEX_38 },
    { "fcc_side_band_txpwr_limit_24g_ch8",  NVRAM_PARAMS_INDEX_39 },
    { "fcc_side_band_txpwr_limit_24g_ch9",  NVRAM_PARAMS_INDEX_40 },
    { "fcc_side_band_txpwr_limit_24g_ch10", NVRAM_PARAMS_INDEX_41 },
    { "fcc_side_band_txpwr_limit_24g_ch11", NVRAM_PARAMS_INDEX_42 },
    { "fcc_side_band_txpwr_limit_24g_ch12", NVRAM_PARAMS_INDEX_43 },
    { "fcc_side_band_txpwr_limit_24g_ch13", NVRAM_PARAMS_INDEX_44 },
    { OAL_PTR_NULL,                         NVRAM_PARAMS_FCC_END_INDEX_BUTT },
    /* CE */
    { "ce_side_band_txpwr_limit_5g_20m_0", NVRAM_PARAMS_INDEX_CE_0 },
    { "ce_side_band_txpwr_limit_5g_20m_1", NVRAM_PARAMS_INDEX_CE_1 },
    { "ce_side_band_txpwr_limit_5g_40m_0", NVRAM_PARAMS_INDEX_CE_2 },
    { "ce_side_band_txpwr_limit_5g_40m_1", NVRAM_PARAMS_INDEX_CE_3 },
    { "ce_side_band_txpwr_limit_5g_80m_0", NVRAM_PARAMS_INDEX_CE_4 },
    { "ce_side_band_txpwr_limit_5g_80m_1", NVRAM_PARAMS_INDEX_CE_5 },
    { "ce_side_band_txpwr_limit_5g_160m",  NVRAM_PARAMS_INDEX_CE_6 },
    { "ce_side_band_txpwr_limit_24g_ch1",  NVRAM_PARAMS_INDEX_CE_7 },
    { "ce_side_band_txpwr_limit_24g_ch2",  NVRAM_PARAMS_INDEX_CE_8 },
    { "ce_side_band_txpwr_limit_24g_ch3",  NVRAM_PARAMS_INDEX_CE_9 },
    { "ce_side_band_txpwr_limit_24g_ch4",  NVRAM_PARAMS_INDEX_CE_10 },
    { "ce_side_band_txpwr_limit_24g_ch5",  NVRAM_PARAMS_INDEX_CE_11 },
    { "ce_side_band_txpwr_limit_24g_ch6",  NVRAM_PARAMS_INDEX_CE_12 },
    { "ce_side_band_txpwr_limit_24g_ch7",  NVRAM_PARAMS_INDEX_CE_13 },
    { "ce_side_band_txpwr_limit_24g_ch8",  NVRAM_PARAMS_INDEX_CE_14 },
    { "ce_side_band_txpwr_limit_24g_ch9",  NVRAM_PARAMS_INDEX_CE_15 },
    { "ce_side_band_txpwr_limit_24g_ch10", NVRAM_PARAMS_INDEX_CE_16 },
    { "ce_side_band_txpwr_limit_24g_ch11", NVRAM_PARAMS_INDEX_CE_17 },
    { "ce_side_band_txpwr_limit_24g_ch12", NVRAM_PARAMS_INDEX_CE_18 },
    { "ce_side_band_txpwr_limit_24g_ch13", NVRAM_PARAMS_INDEX_CE_19 },
    { OAL_PTR_NULL,                        NVRAM_PARAMS_CE_END_INDEX_BUTT },
    /* FCC */
    { "fcc_side_band_txpwr_limit_5g_20m_0_c1", NVRAM_PARAMS_INDEX_25_C1 },
    { "fcc_side_band_txpwr_limit_5g_20m_1_c1", NVRAM_PARAMS_INDEX_26_C1 },
    { "fcc_side_band_txpwr_limit_5g_40m_0_c1", NVRAM_PARAMS_INDEX_27_C1 },
    { "fcc_side_band_txpwr_limit_5g_40m_1_c1", NVRAM_PARAMS_INDEX_28_C1 },
    { "fcc_side_band_txpwr_limit_5g_80m_0_c1", NVRAM_PARAMS_INDEX_29_C1 },
    { "fcc_side_band_txpwr_limit_5g_80m_1_c1", NVRAM_PARAMS_INDEX_30_C1 },
    { "fcc_side_band_txpwr_limit_5g_160m_c1",  NVRAM_PARAMS_INDEX_31_C1 },
    { "fcc_side_band_txpwr_limit_24g_ch1_c1",  NVRAM_PARAMS_INDEX_32_C1 },
    { "fcc_side_band_txpwr_limit_24g_ch2_c1",  NVRAM_PARAMS_INDEX_33_C1 },
    { "fcc_side_band_txpwr_limit_24g_ch3_c1",  NVRAM_PARAMS_INDEX_34_C1 },
    { "fcc_side_band_txpwr_limit_24g_ch4_c1",  NVRAM_PARAMS_INDEX_35_C1 },
    { "fcc_side_band_txpwr_limit_24g_ch5_c1",  NVRAM_PARAMS_INDEX_36_C1 },
    { "fcc_side_band_txpwr_limit_24g_ch6_c1",  NVRAM_PARAMS_INDEX_37_C1 },
    { "fcc_side_band_txpwr_limit_24g_ch7_c1",  NVRAM_PARAMS_INDEX_38_C1 },
    { "fcc_side_band_txpwr_limit_24g_ch8_c1",  NVRAM_PARAMS_INDEX_39_C1 },
    { "fcc_side_band_txpwr_limit_24g_ch9_c1",  NVRAM_PARAMS_INDEX_40_C1 },
    { "fcc_side_band_txpwr_limit_24g_ch10_c1", NVRAM_PARAMS_INDEX_41_C1 },
    { "fcc_side_band_txpwr_limit_24g_ch11_c1", NVRAM_PARAMS_INDEX_42_C1 },
    { "fcc_side_band_txpwr_limit_24g_ch12_c1", NVRAM_PARAMS_INDEX_43_C1 },
    { "fcc_side_band_txpwr_limit_24g_ch13_c1", NVRAM_PARAMS_INDEX_44_C1 },
    { OAL_PTR_NULL,                            NVRAM_PARAMS_FCC_C1_END_INDEX_BUTT },
    /* CE */
    { "ce_side_band_txpwr_limit_5g_20m_0_c1", NVRAM_PARAMS_INDEX_CE_0_C1 },
    { "ce_side_band_txpwr_limit_5g_20m_1_c1", NVRAM_PARAMS_INDEX_CE_1_C1 },
    { "ce_side_band_txpwr_limit_5g_40m_0_c1", NVRAM_PARAMS_INDEX_CE_2_C1 },
    { "ce_side_band_txpwr_limit_5g_40m_1_c1", NVRAM_PARAMS_INDEX_CE_3_C1 },
    { "ce_side_band_txpwr_limit_5g_80m_0_c1", NVRAM_PARAMS_INDEX_CE_4_C1 },
    { "ce_side_band_txpwr_limit_5g_80m_1_c1", NVRAM_PARAMS_INDEX_CE_5_C1 },
    { "ce_side_band_txpwr_limit_5g_160m_c1",  NVRAM_PARAMS_INDEX_CE_6_C1 },
    { "ce_side_band_txpwr_limit_24g_ch1_c1",  NVRAM_PARAMS_INDEX_CE_7_C1 },
    { "ce_side_band_txpwr_limit_24g_ch2_c1",  NVRAM_PARAMS_INDEX_CE_8_C1 },
    { "ce_side_band_txpwr_limit_24g_ch3_c1",  NVRAM_PARAMS_INDEX_CE_9_C1 },
    { "ce_side_band_txpwr_limit_24g_ch4_c1",  NVRAM_PARAMS_INDEX_CE_10_C1 },
    { "ce_side_band_txpwr_limit_24g_ch5_c1",  NVRAM_PARAMS_INDEX_CE_11_C1 },
    { "ce_side_band_txpwr_limit_24g_ch6_c1",  NVRAM_PARAMS_INDEX_CE_12_C1 },
    { "ce_side_band_txpwr_limit_24g_ch7_c1",  NVRAM_PARAMS_INDEX_CE_13_C1 },
    { "ce_side_band_txpwr_limit_24g_ch8_c1",  NVRAM_PARAMS_INDEX_CE_14_C1 },
    { "ce_side_band_txpwr_limit_24g_ch9_c1",  NVRAM_PARAMS_INDEX_CE_15_C1 },
    { "ce_side_band_txpwr_limit_24g_ch10_c1", NVRAM_PARAMS_INDEX_CE_16_C1 },
    { "ce_side_band_txpwr_limit_24g_ch11_c1", NVRAM_PARAMS_INDEX_CE_17_C1 },
    { "ce_side_band_txpwr_limit_24g_ch12_c1", NVRAM_PARAMS_INDEX_CE_18_C1 },
    { "ce_side_band_txpwr_limit_24g_ch13_c1", NVRAM_PARAMS_INDEX_CE_19_C1 },
    { OAL_PTR_NULL,                           NVRAM_PARAMS_CE_C1_END_INDEX_BUTT },
    /* SAR */
    { "sar_txpwr_ctrl_5g_band1_0", NVRAM_PARAMS_INDEX_45 },
    { "sar_txpwr_ctrl_5g_band2_0", NVRAM_PARAMS_INDEX_46 },
    { "sar_txpwr_ctrl_5g_band3_0", NVRAM_PARAMS_INDEX_47 },
    { "sar_txpwr_ctrl_5g_band4_0", NVRAM_PARAMS_INDEX_48 },
    { "sar_txpwr_ctrl_5g_band5_0", NVRAM_PARAMS_INDEX_49 },
    { "sar_txpwr_ctrl_5g_band6_0", NVRAM_PARAMS_INDEX_50 },
    { "sar_txpwr_ctrl_5g_band7_0", NVRAM_PARAMS_INDEX_51 },
    { "sar_txpwr_ctrl_2g_0",       NVRAM_PARAMS_INDEX_52 },
    { "sar_txpwr_ctrl_5g_band1_1", NVRAM_PARAMS_INDEX_53 },
    { "sar_txpwr_ctrl_5g_band2_1", NVRAM_PARAMS_INDEX_54 },
    { "sar_txpwr_ctrl_5g_band3_1", NVRAM_PARAMS_INDEX_55 },
    { "sar_txpwr_ctrl_5g_band4_1", NVRAM_PARAMS_INDEX_56 },
    { "sar_txpwr_ctrl_5g_band5_1", NVRAM_PARAMS_INDEX_57 },
    { "sar_txpwr_ctrl_5g_band6_1", NVRAM_PARAMS_INDEX_58 },
    { "sar_txpwr_ctrl_5g_band7_1", NVRAM_PARAMS_INDEX_59 },
    { "sar_txpwr_ctrl_2g_1",       NVRAM_PARAMS_INDEX_60 },
    { "sar_txpwr_ctrl_5g_band1_2", NVRAM_PARAMS_INDEX_61 },
    { "sar_txpwr_ctrl_5g_band2_2", NVRAM_PARAMS_INDEX_62 },
    { "sar_txpwr_ctrl_5g_band3_2", NVRAM_PARAMS_INDEX_63 },
    { "sar_txpwr_ctrl_5g_band4_2", NVRAM_PARAMS_INDEX_64 },
    { "sar_txpwr_ctrl_5g_band5_2", NVRAM_PARAMS_INDEX_65 },
    { "sar_txpwr_ctrl_5g_band6_2", NVRAM_PARAMS_INDEX_66 },
    { "sar_txpwr_ctrl_5g_band7_2", NVRAM_PARAMS_INDEX_67 },
    { "sar_txpwr_ctrl_2g_2",       NVRAM_PARAMS_INDEX_68 },
    { "sar_txpwr_ctrl_5g_band1_3", NVRAM_PARAMS_INDEX_69 },
    { "sar_txpwr_ctrl_5g_band2_3", NVRAM_PARAMS_INDEX_70 },
    { "sar_txpwr_ctrl_5g_band3_3", NVRAM_PARAMS_INDEX_71 },
    { "sar_txpwr_ctrl_5g_band4_3", NVRAM_PARAMS_INDEX_72 },
    { "sar_txpwr_ctrl_5g_band5_3", NVRAM_PARAMS_INDEX_73 },
    { "sar_txpwr_ctrl_5g_band6_3", NVRAM_PARAMS_INDEX_74 },
    { "sar_txpwr_ctrl_5g_band7_3", NVRAM_PARAMS_INDEX_75 },
    { "sar_txpwr_ctrl_2g_3",       NVRAM_PARAMS_INDEX_76 },
    { "sar_txpwr_ctrl_5g_band1_4", NVRAM_PARAMS_INDEX_77 },
    { "sar_txpwr_ctrl_5g_band2_4", NVRAM_PARAMS_INDEX_78 },
    { "sar_txpwr_ctrl_5g_band3_4", NVRAM_PARAMS_INDEX_79 },
    { "sar_txpwr_ctrl_5g_band4_4", NVRAM_PARAMS_INDEX_80 },
    { "sar_txpwr_ctrl_5g_band5_4", NVRAM_PARAMS_INDEX_81 },
    { "sar_txpwr_ctrl_5g_band6_4", NVRAM_PARAMS_INDEX_82 },
    { "sar_txpwr_ctrl_5g_band7_4", NVRAM_PARAMS_INDEX_83 },
    { "sar_txpwr_ctrl_2g_4",       NVRAM_PARAMS_INDEX_84 },
    { OAL_PTR_NULL,                NVRAM_PARAMS_SAR_END_INDEX_BUTT },
    {"sar_txpwr_ctrl_5g_band1_0_c1", NVRAM_PARAMS_INDEX_45_C1},
    {"sar_txpwr_ctrl_5g_band2_0_c1", NVRAM_PARAMS_INDEX_46_C1},
    {"sar_txpwr_ctrl_5g_band3_0_c1", NVRAM_PARAMS_INDEX_47_C1},
    {"sar_txpwr_ctrl_5g_band4_0_c1", NVRAM_PARAMS_INDEX_48_C1},
    {"sar_txpwr_ctrl_5g_band5_0_c1", NVRAM_PARAMS_INDEX_49_C1},
    {"sar_txpwr_ctrl_5g_band6_0_c1", NVRAM_PARAMS_INDEX_50_C1},
    {"sar_txpwr_ctrl_5g_band7_0_c1", NVRAM_PARAMS_INDEX_51_C1},
    {"sar_txpwr_ctrl_2g_0_c1",       NVRAM_PARAMS_INDEX_52_C1},
    {"sar_txpwr_ctrl_5g_band1_1_c1", NVRAM_PARAMS_INDEX_53_C1},
    {"sar_txpwr_ctrl_5g_band2_1_c1", NVRAM_PARAMS_INDEX_54_C1},
    {"sar_txpwr_ctrl_5g_band3_1_c1", NVRAM_PARAMS_INDEX_55_C1},
    {"sar_txpwr_ctrl_5g_band4_1_c1", NVRAM_PARAMS_INDEX_56_C1},
    {"sar_txpwr_ctrl_5g_band5_1_c1", NVRAM_PARAMS_INDEX_57_C1},
    {"sar_txpwr_ctrl_5g_band6_1_c1", NVRAM_PARAMS_INDEX_58_C1},
    {"sar_txpwr_ctrl_5g_band7_1_c1", NVRAM_PARAMS_INDEX_59_C1},
    {"sar_txpwr_ctrl_2g_1_c1",       NVRAM_PARAMS_INDEX_60_C1},
    {"sar_txpwr_ctrl_5g_band1_2_c1", NVRAM_PARAMS_INDEX_61_C1},
    {"sar_txpwr_ctrl_5g_band2_2_c1", NVRAM_PARAMS_INDEX_62_C1},
    {"sar_txpwr_ctrl_5g_band3_2_c1", NVRAM_PARAMS_INDEX_63_C1},
    {"sar_txpwr_ctrl_5g_band4_2_c1", NVRAM_PARAMS_INDEX_64_C1},
    {"sar_txpwr_ctrl_5g_band5_2_c1", NVRAM_PARAMS_INDEX_65_C1},
    {"sar_txpwr_ctrl_5g_band6_2_c1", NVRAM_PARAMS_INDEX_66_C1},
    {"sar_txpwr_ctrl_5g_band7_2_c1", NVRAM_PARAMS_INDEX_67_C1},
    {"sar_txpwr_ctrl_2g_2_c1",       NVRAM_PARAMS_INDEX_68_C1},
    {"sar_txpwr_ctrl_5g_band1_3_c1", NVRAM_PARAMS_INDEX_69_C1},
    {"sar_txpwr_ctrl_5g_band2_3_c1", NVRAM_PARAMS_INDEX_70_C1},
    {"sar_txpwr_ctrl_5g_band3_3_c1", NVRAM_PARAMS_INDEX_71_C1},
    {"sar_txpwr_ctrl_5g_band4_3_c1", NVRAM_PARAMS_INDEX_72_C1},
    {"sar_txpwr_ctrl_5g_band5_3_c1", NVRAM_PARAMS_INDEX_73_C1},
    {"sar_txpwr_ctrl_5g_band6_3_c1", NVRAM_PARAMS_INDEX_74_C1},
    {"sar_txpwr_ctrl_5g_band7_3_c1", NVRAM_PARAMS_INDEX_75_C1},
    {"sar_txpwr_ctrl_2g_3_c1",       NVRAM_PARAMS_INDEX_76_C1},
    {"sar_txpwr_ctrl_5g_band1_4_c1", NVRAM_PARAMS_INDEX_77_C1},
    {"sar_txpwr_ctrl_5g_band2_4_c1", NVRAM_PARAMS_INDEX_78_C1},
    {"sar_txpwr_ctrl_5g_band3_4_c1", NVRAM_PARAMS_INDEX_79_C1},
    {"sar_txpwr_ctrl_5g_band4_4_c1", NVRAM_PARAMS_INDEX_80_C1},
    {"sar_txpwr_ctrl_5g_band5_4_c1", NVRAM_PARAMS_INDEX_81_C1},
    {"sar_txpwr_ctrl_5g_band6_4_c1", NVRAM_PARAMS_INDEX_82_C1},
    {"sar_txpwr_ctrl_5g_band7_4_c1", NVRAM_PARAMS_INDEX_83_C1},
    {"sar_txpwr_ctrl_2g_4_c1",       NVRAM_PARAMS_INDEX_84_C1},
    {OAL_PTR_NULL,                   NVRAM_PARAMS_SAR_C1_END_INDEX_BUTT },

#ifdef _PRE_WLAN_FEATURE_TAS_ANT_SWITCH
    { "tas_ant_switch_en", NVRAM_PARAMS_TAS_ANT_SWITCH_EN },
    { "tas_txpwr_ctrl_params", NVRAM_PARAMS_TAS_PWR_CTRL },
#endif
    { "5g_max_pow_high_band_fcc_ce", NVRAM_PARAMS_5G_FCC_CE_HIGH_BAND_MAX_PWR },
    { "5g_iq_cali_lpf_lvl",          NVRAM_PARAMS_INDEX_IQ_LPF_LVL},

    { "cfg_tpc_ru_pow_5g_0",         NVRAM_CFG_TPC_RU_POWER_20M_5G},
    { "cfg_tpc_ru_pow_5g_1",         NVRAM_CFG_TPC_RU_POWER_40M_5G},
    { "cfg_tpc_ru_pow_5g_2",         NVRAM_CFG_TPC_RU_POWER_80M_L_5G},
    { "cfg_tpc_ru_pow_5g_3",         NVRAM_CFG_TPC_RU_POWER_80M_H_5G},
    { "cfg_tpc_ru_pow_5g_4",         NVRAM_CFG_TPC_RU_POWER_160M_L_5G},
    { "cfg_tpc_ru_pow_5g_5",         NVRAM_CFG_TPC_RU_POWER_160M_H_5G},
    { "cfg_tpc_ru_pow_2g_0",         NVRAM_CFG_TPC_RU_POWER_20M_2G},
    { "cfg_tpc_ru_pow_2g_1",         NVRAM_CFG_TPC_RU_POWER_40M_0_2G},
    { "cfg_tpc_ru_pow_2g_2",         NVRAM_CFG_TPC_RU_POWER_40M_1_2G},

    { "cfg_tpc_ru_max_pow_242_106_52_26_mimo_2g",      NVRAM_CFG_TPC_RU_MAX_POWER_0_2G_MIMO },
    { "cfg_tpc_ru_max_pow_484_mimo_2g",                NVRAM_CFG_TPC_RU_MAX_POWER_1_2G_MIMO },
    { "cfg_tpc_ru_max_pow_242_106_52_26_siso_2g_c0",   NVRAM_CFG_TPC_RU_MAX_POWER_0_2G_C0 },
    { "cfg_tpc_ru_max_pow_484_siso_2g_c0",             NVRAM_CFG_TPC_RU_MAX_POWER_1_2G_C0 },
    { "cfg_tpc_ru_max_pow_242_106_52_26_siso_2g_c1",   NVRAM_CFG_TPC_RU_MAX_POWER_0_2G_C1 },
    { "cfg_tpc_ru_max_pow_484_siso_2g_c1",             NVRAM_CFG_TPC_RU_MAX_POWER_1_2G_C1 },

    { "cfg_tpc_ru_max_pow_242_106_52_26_mimo_5g_b1",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_MIMO_B1 },
    { "cfg_tpc_ru_max_pow_1992_996_484_mimo_5g_b1",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_MIMO_B1 },
    { "cfg_tpc_ru_max_pow_242_106_52_26_siso_5g_c0_b1",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_C0_B1 },
    { "cfg_tpc_ru_max_pow_1992_996_484_siso_5g_c0_b1",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_C0_B1 },
    { "cfg_tpc_ru_max_pow_242_106_52_26_siso_5g_c1_b1",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_C1_B1 },
    { "cfg_tpc_ru_max_pow_1992_996_484_siso_5g_c1_b1",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_C1_B1 },
    { "cfg_tpc_ru_max_pow_242_106_52_26_mimo_5g_b2",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_MIMO_B2 },
    { "cfg_tpc_ru_max_pow_1992_996_484_mimo_5g_b2",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_MIMO_B2 },
    { "cfg_tpc_ru_max_pow_242_106_52_26_siso_5g_c0_b2",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_C0_B2 },
    { "cfg_tpc_ru_max_pow_1992_996_484_siso_5g_c0_b2",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_C0_B2 },
    { "cfg_tpc_ru_max_pow_242_106_52_26_siso_5g_c1_b2",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_C1_B2 },
    { "cfg_tpc_ru_max_pow_1992_996_484_siso_5g_c1_b2",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_C1_B2 },
    { "cfg_tpc_ru_max_pow_242_106_52_26_mimo_5g_b3",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_MIMO_B3 },
    { "cfg_tpc_ru_max_pow_1992_996_484_mimo_5g_b3",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_MIMO_B3 },
    { "cfg_tpc_ru_max_pow_242_106_52_26_siso_5g_c0_b3",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_C0_B3 },
    { "cfg_tpc_ru_max_pow_1992_996_484_siso_5g_c0_b3",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_C0_B3 },
    { "cfg_tpc_ru_max_pow_242_106_52_26_siso_5g_c1_b3",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_C1_B3 },
    { "cfg_tpc_ru_max_pow_1992_996_484_siso_5g_c1_b3",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_C1_B3 },
    { "cfg_tpc_ru_max_pow_242_106_52_26_mimo_5g_b4",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_MIMO_B4 },
    { "cfg_tpc_ru_max_pow_1992_996_484_mimo_5g_b4",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_MIMO_B4 },
    { "cfg_tpc_ru_max_pow_242_106_52_26_siso_5g_c0_b4",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_C0_B4 },
    { "cfg_tpc_ru_max_pow_1992_996_484_siso_5g_c0_b4",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_C0_B4 },
    { "cfg_tpc_ru_max_pow_242_106_52_26_siso_5g_c1_b4",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_C1_B4 },
    { "cfg_tpc_ru_max_pow_1992_996_484_siso_5g_c1_b4",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_C1_B4 },
    { "cfg_tpc_ru_max_pow_242_106_52_26_mimo_5g_b5",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_MIMO_B5 },
    { "cfg_tpc_ru_max_pow_1992_996_484_mimo_5g_b5",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_MIMO_B5 },
    { "cfg_tpc_ru_max_pow_242_106_52_26_siso_5g_c0_b5",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_C0_B5 },
    { "cfg_tpc_ru_max_pow_1992_996_484_siso_5g_c0_b5",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_C0_B5 },
    { "cfg_tpc_ru_max_pow_242_106_52_26_siso_5g_c1_b5",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_C1_B5 },
    { "cfg_tpc_ru_max_pow_1992_996_484_siso_5g_c1_b5",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_C1_B5 },
    { "cfg_tpc_ru_max_pow_242_106_52_26_mimo_5g_b6",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_MIMO_B6 },
    { "cfg_tpc_ru_max_pow_1992_996_484_mimo_5g_b6",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_MIMO_B6 },
    { "cfg_tpc_ru_max_pow_242_106_52_26_siso_5g_c0_b6",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_C0_B6 },
    { "cfg_tpc_ru_max_pow_1992_996_484_siso_5g_c0_b6",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_C0_B6 },
    { "cfg_tpc_ru_max_pow_242_106_52_26_siso_5g_c1_b6",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_C1_B6 },
    { "cfg_tpc_ru_max_pow_1992_996_484_siso_5g_c1_b6",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_C1_B6 },
    { "cfg_tpc_ru_max_pow_242_106_52_26_mimo_5g_b7",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_MIMO_B7 },
    { "cfg_tpc_ru_max_pow_1992_996_484_mimo_5g_b7",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_MIMO_B7 },
    { "cfg_tpc_ru_max_pow_242_106_52_26_siso_5g_c0_b7",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_C0_B7 },
    { "cfg_tpc_ru_max_pow_1992_996_484_siso_5g_c0_b7",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_C0_B7 },
    { "cfg_tpc_ru_max_pow_242_106_52_26_siso_5g_c1_b7",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_C1_B7 },
    { "cfg_tpc_ru_max_pow_1992_996_484_siso_5g_c1_b7",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_C1_B7 },
    { "cfg_tpc_ru_max_pow_242_106_52_26_mimo_5g_b1_ce",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_MIMO_B1_CE },
    { "cfg_tpc_ru_max_pow_1992_996_484_mimo_5g_b1_ce",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_MIMO_B1_CE },
    { "cfg_tpc_ru_max_pow_242_106_52_26_siso_5g_c0_b1_ce",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_C0_B1_CE },
    { "cfg_tpc_ru_max_pow_1992_996_484_siso_5g_c0_b1_ce",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_C0_B1_CE },
    { "cfg_tpc_ru_max_pow_242_106_52_26_siso_5g_c1_b1_ce",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_C1_B1_CE },
    { "cfg_tpc_ru_max_pow_1992_996_484_siso_5g_c1_b1_ce",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_C1_B1_CE },
    { "cfg_tpc_ru_max_pow_242_106_52_26_mimo_5g_b2_ce",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_MIMO_B2_CE },
    { "cfg_tpc_ru_max_pow_1992_996_484_mimo_5g_b2_ce",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_MIMO_B2_CE },
    { "cfg_tpc_ru_max_pow_242_106_52_26_siso_5g_c0_b2_ce",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_C0_B2_CE },
    { "cfg_tpc_ru_max_pow_1992_996_484_siso_5g_c0_b2_ce",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_C0_B2_CE },
    { "cfg_tpc_ru_max_pow_242_106_52_26_siso_5g_c1_b2_ce",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_C1_B2_CE },
    { "cfg_tpc_ru_max_pow_1992_996_484_siso_5g_c1_b2_ce",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_C1_B2_CE },
    { "cfg_tpc_ru_max_pow_242_106_52_26_mimo_5g_b3_ce",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_MIMO_B3_CE },
    { "cfg_tpc_ru_max_pow_1992_996_484_mimo_5g_b3_ce",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_MIMO_B3_CE },
    { "cfg_tpc_ru_max_pow_242_106_52_26_siso_5g_c0_b3_ce",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_C0_B3_CE },
    { "cfg_tpc_ru_max_pow_1992_996_484_siso_5g_c0_b3_ce",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_C0_B3_CE },
    { "cfg_tpc_ru_max_pow_242_106_52_26_siso_5g_c1_b3_ce",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_C1_B3_CE },
    { "cfg_tpc_ru_max_pow_1992_996_484_siso_5g_c1_b3_ce",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_C1_B3_CE },
    { "cfg_tpc_ru_max_pow_242_106_52_26_mimo_5g_b4_ce",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_MIMO_B4_CE },
    { "cfg_tpc_ru_max_pow_1992_996_484_mimo_5g_b4_ce",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_MIMO_B4_CE },
    { "cfg_tpc_ru_max_pow_242_106_52_26_siso_5g_c0_b4_ce",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_C0_B4_CE },
    { "cfg_tpc_ru_max_pow_1992_996_484_siso_5g_c0_b4_ce",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_C0_B4_CE },
    { "cfg_tpc_ru_max_pow_242_106_52_26_siso_5g_c1_b4_ce",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_C1_B4_CE },
    { "cfg_tpc_ru_max_pow_1992_996_484_siso_5g_c1_b4_ce",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_C1_B4_CE },
    { "cfg_tpc_ru_max_pow_242_106_52_26_mimo_5g_b5_ce",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_MIMO_B5_CE },
    { "cfg_tpc_ru_max_pow_1992_996_484_mimo_5g_b5_ce",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_MIMO_B5_CE },
    { "cfg_tpc_ru_max_pow_242_106_52_26_siso_5g_c0_b5_ce",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_C0_B5_CE },
    { "cfg_tpc_ru_max_pow_1992_996_484_siso_5g_c0_b5_ce",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_C0_B5_CE },
    { "cfg_tpc_ru_max_pow_242_106_52_26_siso_5g_c1_b5_ce",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_C1_B5_CE },
    { "cfg_tpc_ru_max_pow_1992_996_484_siso_5g_c1_b5_ce",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_C1_B5_CE },
    { "cfg_tpc_ru_max_pow_242_106_52_26_mimo_5g_b6_ce",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_MIMO_B6_CE },
    { "cfg_tpc_ru_max_pow_1992_996_484_mimo_5g_b6_ce",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_MIMO_B6_CE },
    { "cfg_tpc_ru_max_pow_242_106_52_26_siso_5g_c0_b6_ce",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_C0_B6_CE },
    { "cfg_tpc_ru_max_pow_1992_996_484_siso_5g_c0_b6_ce",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_C0_B6_CE },
    { "cfg_tpc_ru_max_pow_242_106_52_26_siso_5g_c1_b6_ce",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_C1_B6_CE },
    { "cfg_tpc_ru_max_pow_1992_996_484_siso_5g_c1_b6_ce",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_C1_B6_CE },
    { "cfg_tpc_ru_max_pow_242_106_52_26_mimo_5g_b7_ce",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_MIMO_B7_CE },
    { "cfg_tpc_ru_max_pow_1992_996_484_mimo_5g_b7_ce",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_MIMO_B7_CE },
    { "cfg_tpc_ru_max_pow_242_106_52_26_siso_5g_c0_b7_ce",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_C0_B7_CE },
    { "cfg_tpc_ru_max_pow_1992_996_484_siso_5g_c0_b7_ce",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_C0_B7_CE },
    { "cfg_tpc_ru_max_pow_242_106_52_26_siso_5g_c1_b7_ce",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_C1_B7_CE },
    { "cfg_tpc_ru_max_pow_1992_996_484_siso_5g_c1_b7_ce",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_C1_B7_CE },
    { "cfg_tpc_ru_max_pow_242_106_52_26_mimo_5g_b1_fcc",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_MIMO_B1_FCC },
    { "cfg_tpc_ru_max_pow_1992_996_484_mimo_5g_b1_fcc",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_MIMO_B1_FCC },
    { "cfg_tpc_ru_max_pow_242_106_52_26_siso_5g_c0_b1_fcc",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_C0_B1_FCC },
    { "cfg_tpc_ru_max_pow_1992_996_484_siso_5g_c0_b1_fcc",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_C0_B1_FCC },
    { "cfg_tpc_ru_max_pow_242_106_52_26_siso_5g_c1_b1_fcc",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_C1_B1_FCC },
    { "cfg_tpc_ru_max_pow_1992_996_484_siso_5g_c1_b1_fcc",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_C1_B1_FCC },
    { "cfg_tpc_ru_max_pow_242_106_52_26_mimo_5g_b2_fcc",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_MIMO_B2_FCC },
    { "cfg_tpc_ru_max_pow_1992_996_484_mimo_5g_b2_fcc",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_MIMO_B2_FCC },
    { "cfg_tpc_ru_max_pow_242_106_52_26_siso_5g_c0_b2_fcc",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_C0_B2_FCC },
    { "cfg_tpc_ru_max_pow_1992_996_484_siso_5g_c0_b2_fcc",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_C0_B2_FCC },
    { "cfg_tpc_ru_max_pow_242_106_52_26_siso_5g_c1_b2_fcc",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_C1_B2_FCC },
    { "cfg_tpc_ru_max_pow_1992_996_484_siso_5g_c1_b2_fcc",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_C1_B2_FCC },
    { "cfg_tpc_ru_max_pow_242_106_52_26_mimo_5g_b3_fcc",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_MIMO_B3_FCC },
    { "cfg_tpc_ru_max_pow_1992_996_484_mimo_5g_b3_fcc",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_MIMO_B3_FCC },
    { "cfg_tpc_ru_max_pow_242_106_52_26_siso_5g_c0_b3_fcc",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_C0_B3_FCC },
    { "cfg_tpc_ru_max_pow_1992_996_484_siso_5g_c0_b3_fcc",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_C0_B3_FCC },
    { "cfg_tpc_ru_max_pow_242_106_52_26_siso_5g_c1_b3_fcc",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_C1_B3_FCC },
    { "cfg_tpc_ru_max_pow_1992_996_484_siso_5g_c1_b3_fcc",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_C1_B3_FCC },
    { "cfg_tpc_ru_max_pow_242_106_52_26_mimo_5g_b4_fcc",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_MIMO_B4_FCC },
    { "cfg_tpc_ru_max_pow_1992_996_484_mimo_5g_b4_fcc",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_MIMO_B4_FCC },
    { "cfg_tpc_ru_max_pow_242_106_52_26_siso_5g_c0_b4_fcc",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_C0_B4_FCC },
    { "cfg_tpc_ru_max_pow_1992_996_484_siso_5g_c0_b4_fcc",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_C0_B4_FCC },
    { "cfg_tpc_ru_max_pow_242_106_52_26_siso_5g_c1_b4_fcc",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_C1_B4_FCC },
    { "cfg_tpc_ru_max_pow_1992_996_484_siso_5g_c1_b4_fcc",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_C1_B4_FCC },
    { "cfg_tpc_ru_max_pow_242_106_52_26_mimo_5g_b5_fcc",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_MIMO_B5_FCC },
    { "cfg_tpc_ru_max_pow_1992_996_484_mimo_5g_b5_fcc",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_MIMO_B5_FCC },
    { "cfg_tpc_ru_max_pow_242_106_52_26_siso_5g_c0_b5_fcc",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_C0_B5_FCC },
    { "cfg_tpc_ru_max_pow_1992_996_484_siso_5g_c0_b5_fcc",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_C0_B5_FCC },
    { "cfg_tpc_ru_max_pow_242_106_52_26_siso_5g_c1_b5_fcc",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_C1_B5_FCC },
    { "cfg_tpc_ru_max_pow_1992_996_484_siso_5g_c1_b5_fcc",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_C1_B5_FCC },
    { "cfg_tpc_ru_max_pow_242_106_52_26_mimo_5g_b6_fcc",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_MIMO_B6_FCC },
    { "cfg_tpc_ru_max_pow_1992_996_484_mimo_5g_b6_fcc",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_MIMO_B6_FCC },
    { "cfg_tpc_ru_max_pow_242_106_52_26_siso_5g_c0_b6_fcc",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_C0_B6_FCC },
    { "cfg_tpc_ru_max_pow_1992_996_484_siso_5g_c0_b6_fcc",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_C0_B6_FCC },
    { "cfg_tpc_ru_max_pow_242_106_52_26_siso_5g_c1_b6_fcc",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_C1_B6_FCC },
    { "cfg_tpc_ru_max_pow_1992_996_484_siso_5g_c1_b6_fcc",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_C1_B6_FCC },
    { "cfg_tpc_ru_max_pow_242_106_52_26_mimo_5g_b7_fcc",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_MIMO_B7_FCC },
    { "cfg_tpc_ru_max_pow_1992_996_484_mimo_5g_b7_fcc",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_MIMO_B7_FCC },
    { "cfg_tpc_ru_max_pow_242_106_52_26_siso_5g_c0_b7_fcc",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_C0_B7_FCC },
    { "cfg_tpc_ru_max_pow_1992_996_484_siso_5g_c0_b7_fcc",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_C0_B7_FCC },
    { "cfg_tpc_ru_max_pow_242_106_52_26_siso_5g_c1_b7_fcc",    NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_C1_B7_FCC },
    { "cfg_tpc_ru_max_pow_1992_996_484_siso_5g_c1_b7_fcc",    NVRAM_CFG_TPC_RU_MAX_POWER_1_5G_C1_B7_FCC },

    { "cfg_tpc_ru_max_pow_2g",      NVRAM_CFG_TPC_RU_MAX_POWER_2G },
    { "cfg_tpc_ru_max_pow_5g",      NVRAM_CFG_TPC_RU_MAX_POWER_5G },
};

OAL_STATIC wlan_cfg_cmd g_nvram_pro_line_config_ini[] = {
    /* ����nvram���� */
    { "nvram_pa2gccka0",  WLAN_CFG_DTS_NVRAM_RATIO_PA2GCCKA0 },
    { "nvram_pa2ga0",     WLAN_CFG_NVRAM_RATIO_PA2GA0 },
    { "nvram_pa2g40a0",   WLAN_CFG_DTS_NVRAM_RATIO_PA2G40A0 },
    { "nvram_pa5ga0",     WLAN_CFG_DTS_NVRAM_RATIO_PA5GA0 },
    { "nvram_pa2gccka1",  WLAN_CFG_DTS_NVRAM_RATIO_PA2GCCKA1 },
    { "nvram_pa2ga1",     WLAN_CFG_DTS_NVRAM_RATIO_PA2GA1 },
    { "nvram_pa2g40a1",   WLAN_CFG_DTS_NVRAM_RATIO_PA2G40A1 },
    { "nvram_pa5ga1",     WLAN_CFG_DTS_NVRAM_RATIO_PA5GA1 },
    { "nvram_pa5ga0_low", WLAN_CFG_DTS_NVRAM_RATIO_PA5GA0_LOW },
    { "nvram_pa5ga1_low", WLAN_CFG_DTS_NVRAM_RATIO_PA5GA1_LOW },

    { OAL_PTR_NULL, WLAN_CFG_DTS_NVRAM_MUFREQ_2GCCK_C0 },
    { OAL_PTR_NULL, WLAN_CFG_DTS_NVRAM_MUFREQ_2G20_C0 },
    { OAL_PTR_NULL, WLAN_CFG_DTS_NVRAM_MUFREQ_2G40_C0 },
    { OAL_PTR_NULL, WLAN_CFG_DTS_NVRAM_MUFREQ_2GCCK_C1 },
    { OAL_PTR_NULL, WLAN_CFG_DTS_NVRAM_MUFREQ_2G20_C1 },
    { OAL_PTR_NULL, WLAN_CFG_DTS_NVRAM_MUFREQ_2G40_C1 },
    { OAL_PTR_NULL, WLAN_CFG_DTS_NVRAM_MUFREQ_5G160_C0 },
    { OAL_PTR_NULL, WLAN_CFG_DTS_NVRAM_MUFREQ_5G160_C1 },

    { "nvram_pa5ga0_band1",     WLAN_CFG_DTS_NVRAM_RATIO_PA5GA0_BAND1 },
    { "nvram_pa5ga1_band1",     WLAN_CFG_DTS_NVRAM_RATIO_PA5GA1_BAND1 },
    { "nvram_pa2gcwa0",         WLAN_CFG_DTS_NVRAM_RATIO_PA2GCWA0 },
    { "nvram_pa2gcwa1",         WLAN_CFG_DTS_NVRAM_RATIO_PA2GCWA1 },
    { "nvram_pa5ga0_band1_low", WLAN_CFG_DTS_NVRAM_RATIO_PA5GA0_BAND1_LOW },
    { "nvram_pa5ga1_band1_low", WLAN_CFG_DTS_NVRAM_RATIO_PA5GA1_BAND1_LOW },

    { "nvram_ppa2gcwa0", WLAN_CFG_DTS_NVRAM_RATIO_PPA2GCWA0 },
    { "nvram_ppa2gcwa1", WLAN_CFG_DTS_NVRAM_RATIO_PPA2GCWA1 },

    { OAL_PTR_NULL, WLAN_CFG_DTS_NVRAM_PARAMS_BUTT },
};

/*
 * �� �� ��  : original_value_for_nvram_params
 * ��������  : ����书�ʶ��ƻ�������ֵ����
 */
OAL_STATIC oal_void original_value_for_nvram_params(oal_void)
{
    oal_uint32 uc_param_idx;
    g_nvram_init_params[NVRAM_PARAMS_INDEX_0] = 0x0000F6F6;
    g_nvram_init_params[NVRAM_PARAMS_INDEX_1] = 0xFBE7F1FB;
    g_nvram_init_params[NVRAM_PARAMS_INDEX_2] = 0xE7F1F1FB;
    g_nvram_init_params[NVRAM_PARAMS_INDEX_3] = 0xECF6F6D8;
    g_nvram_init_params[NVRAM_PARAMS_INDEX_4] = 0xD8D8E2EC;
    g_nvram_init_params[NVRAM_PARAMS_INDEX_5] = 0x000000E2;
    g_nvram_init_params[NVRAM_PARAMS_INDEX_6] = 0x0000F1F6;
    g_nvram_init_params[NVRAM_PARAMS_INDEX_7] = 0xE2ECF600;
    g_nvram_init_params[NVRAM_PARAMS_INDEX_8] = 0xF1FBFBFB;
    g_nvram_init_params[NVRAM_PARAMS_INDEX_9] = 0x00F1D3EA;
    g_nvram_init_params[NVRAM_PARAMS_INDEX_10] = 0xE7EC0000;
    g_nvram_init_params[NVRAM_PARAMS_INDEX_11] = 0xC9CED3CE;
    /*  2.4g 5g 20M mcs9 */
    g_nvram_init_params[NVRAM_PARAMS_INDEX_12] = 0xD8DDCED3;
    g_nvram_init_params[NVRAM_PARAMS_INDEX_13] = 0xC9C9CED3;
    g_nvram_init_params[NVRAM_PARAMS_INDEX_14] = 0x000000C4;
    g_nvram_init_params[NVRAM_PARAMS_INDEX_15] = 0xEC000000;
    g_nvram_init_params[NVRAM_PARAMS_INDEX_16] = 0xC9CECEE7;
    g_nvram_init_params[NVRAM_PARAMS_INDEX_17] = 0x000000C4;
    /* DPD ��ʱ�߽����ʹ��� */
    g_nvram_init_params[NVRAM_PARAMS_INDEX_DPD_0] = 0xE2ECEC00;
    g_nvram_init_params[NVRAM_PARAMS_INDEX_DPD_1] = 0xE2E200E2;
    g_nvram_init_params[NVRAM_PARAMS_INDEX_DPD_2] = 0x0000C4C4;
    /* 11B��OFDM���ʲ� */
    g_nvram_init_params[NVRAM_PARAMS_INDEX_11B_OFDM_DELT_POW] = 0xA0A00000;
    /* 5G���ʺ�IQУ׼UPC����ֵ */
    g_nvram_init_params[NVRAM_PARAMS_INDEX_IQ_MAX_UPC] = 0xD8D83030;
    for (uc_param_idx = NVRAM_PARAMS_FCC_START_INDEX;
         uc_param_idx < NVRAM_PARAMS_SAR_C1_END_INDEX_BUTT; uc_param_idx++) {
        /* FCC/CE/SAR������֤ */
        g_nvram_init_params[uc_param_idx] = 0xFFFFFFFF;
    }
#ifdef _PRE_WLAN_FEATURE_TAS_ANT_SWITCH
    g_nvram_init_params[NVRAM_PARAMS_TAS_ANT_SWITCH_EN] = 0x0;
    g_nvram_init_params[NVRAM_PARAMS_TAS_PWR_CTRL] = 0x0;
#endif
    g_nvram_init_params[NVRAM_PARAMS_5G_FCC_CE_HIGH_BAND_MAX_PWR] = 0x00FA00FA;
    g_nvram_init_params[NVRAM_PARAMS_INDEX_IQ_LPF_LVL] = 0x00001111;
}

/*
 * �� �� ��  : original_value_for_dts_params
 * ��������  : dts���ƻ�������ֵ����
 */
OAL_STATIC oal_void original_value_for_dts_params(oal_void)
{
    /* У׼ */
    g_dts_params[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN1] = WLAN_CALI_TXPWR_REF_2G_CH1_VAL;
    g_dts_params[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN2] = WLAN_CALI_TXPWR_REF_2G_CH2_VAL;
    g_dts_params[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN3] = WLAN_CALI_TXPWR_REF_2G_CH3_VAL;
    g_dts_params[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN4] = WLAN_CALI_TXPWR_REF_2G_CH4_VAL;
    g_dts_params[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN5] = WLAN_CALI_TXPWR_REF_2G_CH5_VAL;
    g_dts_params[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN6] = WLAN_CALI_TXPWR_REF_2G_CH6_VAL;
    g_dts_params[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN7] = WLAN_CALI_TXPWR_REF_2G_CH7_VAL;
    g_dts_params[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN8] = WLAN_CALI_TXPWR_REF_2G_CH8_VAL;
    g_dts_params[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN9] = WLAN_CALI_TXPWR_REF_2G_CH9_VAL;
    g_dts_params[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN10] = WLAN_CALI_TXPWR_REF_2G_CH10_VAL;
    g_dts_params[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN11] = WLAN_CALI_TXPWR_REF_2G_CH11_VAL;
    g_dts_params[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN12] = WLAN_CALI_TXPWR_REF_2G_CH12_VAL;
    g_dts_params[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN13] = WLAN_CALI_TXPWR_REF_2G_CH13_VAL;
    g_dts_params[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_5G_VAL_BAND1] = WLAN_CALI_TXPWR_REF_5G_BAND1_VAL;
    g_dts_params[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_5G_VAL_BAND2] = WLAN_CALI_TXPWR_REF_5G_BAND2_VAL;
    g_dts_params[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_5G_VAL_BAND3] = WLAN_CALI_TXPWR_REF_5G_BAND3_VAL;
    g_dts_params[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_5G_VAL_BAND4] = WLAN_CALI_TXPWR_REF_5G_BAND4_VAL;
    g_dts_params[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_5G_VAL_BAND5] = WLAN_CALI_TXPWR_REF_5G_BAND5_VAL;
    g_dts_params[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_5G_VAL_BAND6] = WLAN_CALI_TXPWR_REF_5G_BAND6_VAL;
    g_dts_params[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_5G_VAL_BAND7] = WLAN_CALI_TXPWR_REF_5G_BAND7_VAL;
    g_dts_params[WLAN_CFG_DTS_CALI_TONE_AMP_GRADE] = WLAN_CALI_TONE_GRADE_AMP;
    /* DPDУ׼���ƻ� */
    g_dts_params[WLAN_CFG_DTS_DPD_CALI_CH_CORE0] = 0x641DA71,
    g_dts_params[WLAN_CFG_DTS_DPD_USE_CALI_CH_IDX0_CORE0] = 0x11110000,
    g_dts_params[WLAN_CFG_DTS_DPD_USE_CALI_CH_IDX1_CORE0] = 0x33222,
    g_dts_params[WLAN_CFG_DTS_DPD_USE_CALI_CH_IDX2_CORE0] = 0x22211000,
    g_dts_params[WLAN_CFG_DTS_DPD_USE_CALI_CH_IDX3_CORE0] = 0x2,
    g_dts_params[WLAN_CFG_DTS_DPD_CALI_20M_DEL_POW_CORE0] = 0x00000000,
    g_dts_params[WLAN_CFG_DTS_DPD_CALI_40M_DEL_POW_CORE0] = 0x00000000,
    g_dts_params[WLAN_CFG_DTS_DPD_CALI_CH_CORE1] = 0x641DA71,
    g_dts_params[WLAN_CFG_DTS_DPD_USE_CALI_CH_IDX0_CORE1] = 0x11110000,
    g_dts_params[WLAN_CFG_DTS_DPD_USE_CALI_CH_IDX1_CORE1] = 0x33222,
    g_dts_params[WLAN_CFG_DTS_DPD_USE_CALI_CH_IDX2_CORE1] = 0x22211000,
    g_dts_params[WLAN_CFG_DTS_DPD_USE_CALI_CH_IDX3_CORE1] = 0x2,
    g_dts_params[WLAN_CFG_DTS_DPD_CALI_20M_DEL_POW_CORE1] = 0x00000000,
    g_dts_params[WLAN_CFG_DTS_DPD_CALI_40M_DEL_POW_CORE1] = 0x00000000,
    g_dts_params[WLAN_CFG_DTS_DYN_CALI_DSCR_ITERVL] = 0x0;
}

/*
 * �� �� ��  : host_auto_freq_params_init
 * ��������  : �����ƻ�����ȫ������ g_host_init_params���Զ���Ƶ��ز�������ֵ
 *             ini�ļ���ȡʧ��ʱ�ó�ֵ
 */
OAL_STATIC oal_void host_auto_freq_params_init(oal_void)
{
    g_host_init_params[WLAN_CFG_INIT_PSS_THRESHOLD_LEVEL_0] = PPS_VALUE_0;
    g_host_init_params[WLAN_CFG_INIT_CPU_FREQ_LIMIT_LEVEL_0] = CPU_MIN_FREQ_VALUE_0;
    g_host_init_params[WLAN_CFG_INIT_DDR_FREQ_LIMIT_LEVEL_0] = DDR_MIN_FREQ_VALUE_0;
    g_host_init_params[WLAN_CFG_INIT_DEVICE_TYPE_LEVEL_0] = FREQ_IDLE;
    g_host_init_params[WLAN_CFG_INIT_PSS_THRESHOLD_LEVEL_1] = PPS_VALUE_1;
    g_host_init_params[WLAN_CFG_INIT_CPU_FREQ_LIMIT_LEVEL_1] = CPU_MIN_FREQ_VALUE_1;
    g_host_init_params[WLAN_CFG_INIT_DDR_FREQ_LIMIT_LEVEL_1] = DDR_MIN_FREQ_VALUE_1;
    g_host_init_params[WLAN_CFG_INIT_DEVICE_TYPE_LEVEL_1] = FREQ_MIDIUM;
    g_host_init_params[WLAN_CFG_INIT_PSS_THRESHOLD_LEVEL_2] = PPS_VALUE_2;
    g_host_init_params[WLAN_CFG_INIT_CPU_FREQ_LIMIT_LEVEL_2] = CPU_MIN_FREQ_VALUE_2;
    g_host_init_params[WLAN_CFG_INIT_DDR_FREQ_LIMIT_LEVEL_2] = DDR_MIN_FREQ_VALUE_2;
    g_host_init_params[WLAN_CFG_INIT_DEVICE_TYPE_LEVEL_2] = FREQ_HIGHER;
    g_host_init_params[WLAN_CFG_INIT_PSS_THRESHOLD_LEVEL_3] = PPS_VALUE_3;
    g_host_init_params[WLAN_CFG_INIT_CPU_FREQ_LIMIT_LEVEL_3] = CPU_MIN_FREQ_VALUE_3;
    g_host_init_params[WLAN_CFG_INIT_DDR_FREQ_LIMIT_LEVEL_3] = DDR_MIN_FREQ_VALUE_3;
    g_host_init_params[WLAN_CFG_INIT_DEVICE_TYPE_LEVEL_3] = FREQ_HIGHEST;
}

#ifdef _PRE_WLAN_FEATURE_MULTI_NETBUF_AMSDU
/*
 * �� �� ��  : host_amsdu_th_params_init
 * ��������  : �����ƻ�����ȫ������ g_host_init_params��amsdu�ۺ�������ز�������ֵ
 *             ini�ļ���ȡʧ��ʱ�ó�ֵ
 */
OAL_STATIC oal_void host_amsdu_th_params_init(oal_void)
{
    g_host_init_params[WLAN_CFG_INIT_AMPDU_AMSDU_SKB] = OAL_FALSE;
    g_host_init_params[WLAN_CFG_INIT_AMSDU_AMPDU_TH_HIGH]   = WLAN_AMSDU_AMPDU_TH_HIGH;
    g_host_init_params[WLAN_CFG_INIT_AMSDU_AMPDU_TH_MIDDLE] = WLAN_AMSDU_AMPDU_TH_MIDDLE;
    g_host_init_params[WLAN_CFG_INIT_AMSDU_AMPDU_TH_LOW]    = WLAN_AMSDU_AMPDU_TH_LOW;
}
#endif


/*
 * �� �� ��  : host_params_init_first
 * ��������  : �����ƻ�����ȫ������ g_host_init_params ����ֵ
 *             ini�ļ���ȡʧ��ʱ�ó�ֵ
 */
OAL_STATIC oal_void host_params_performance_init(oal_void)
{
    /* ��̬��PCIE�ж� */
    g_host_init_params[WLAN_CFG_PRIV_DMA_LATENCY] = OAL_FALSE;
    g_host_init_params[WLAN_CFG_PRIV_LOCK_CPU_TH_HIGH] = 0;
    g_host_init_params[WLAN_CFG_PRIV_LOCK_CPU_TH_LOW] = 0;
    g_host_init_params[WLAN_CFG_INIT_IRQ_AFFINITY] = OAL_FALSE;
    g_host_init_params[WLAN_CFG_INIT_IRQ_TH_HIGH] = WLAN_IRQ_TH_HIGH;
    g_host_init_params[WLAN_CFG_INIT_IRQ_TH_LOW] = WLAN_IRQ_TH_LOW;
    g_host_init_params[WLAN_CFG_INIT_IRQ_PPS_TH_HIGH] = WLAN_IRQ_PPS_TH_HIGH;
    g_host_init_params[WLAN_CFG_INIT_IRQ_PPS_TH_LOW] = WLAN_IRQ_PPS_TH_LOW;
#ifdef _PRE_WLAN_FEATURE_AMPDU_TX_HW
    /* Ӳ���ۺ϶��ƻ��� */
    g_host_init_params[WLAN_CFG_INIT_HW_AMPDU] = OAL_FALSE;
    g_host_init_params[WLAN_CFG_INIT_HW_AMPDU_TH_HIGH] = WLAN_HW_AMPDU_TH_HIGH;
    g_host_init_params[WLAN_CFG_INIT_HW_AMPDU_TH_LOW] = WLAN_HW_AMPDU_TH_LOW;
#endif

#ifdef _PRE_WLAN_TCP_OPT
    g_host_init_params[WLAN_CFG_INIT_TCP_ACK_FILTER] = OAL_FALSE;
    g_host_init_params[WLAN_CFG_INIT_TCP_ACK_FILTER_TH_HIGH] = WLAN_TCP_ACK_FILTER_TH_HIGH;
    g_host_init_params[WLAN_CFG_INIT_TCP_ACK_FILTER_TH_LOW] = WLAN_TCP_ACK_FILTER_TH_LOW;
#endif

    g_host_init_params[WLAN_CFG_INIT_TX_SMALL_AMSDU] = OAL_TRUE;
    g_host_init_params[WLAN_CFG_INIT_SMALL_AMSDU_HIGH] = WLAN_SMALL_AMSDU_HIGH;
    g_host_init_params[WLAN_CFG_INIT_SMALL_AMSDU_LOW] = WLAN_SMALL_AMSDU_LOW;
    g_host_init_params[WLAN_CFG_INIT_SMALL_AMSDU_PPS_HIGH] = WLAN_SMALL_AMSDU_PPS_HIGH;
    g_host_init_params[WLAN_CFG_INIT_SMALL_AMSDU_PPS_LOW] = WLAN_SMALL_AMSDU_PPS_LOW;

    g_host_init_params[WLAN_CFG_INIT_TX_TCP_ACK_BUF] = OAL_TRUE;
    g_host_init_params[WLAN_CFG_INIT_TCP_ACK_BUF_HIGH] = WLAN_TCP_ACK_BUF_HIGH;
    g_host_init_params[WLAN_CFG_INIT_TCP_ACK_BUF_LOW] = WLAN_TCP_ACK_BUF_LOW;
    g_host_init_params[WLAN_CFG_INIT_TCP_ACK_BUF_HIGH_40M] = WLAN_TCP_ACK_BUF_HIGH_40M;
    g_host_init_params[WLAN_CFG_INIT_TCP_ACK_BUF_LOW_40M] = WLAN_TCP_ACK_BUF_LOW_40M;
    g_host_init_params[WLAN_CFG_INIT_TCP_ACK_BUF_HIGH_80M] = WLAN_TCP_ACK_BUF_HIGH_80M;
    g_host_init_params[WLAN_CFG_INIT_TCP_ACK_BUF_LOW_80M] = WLAN_TCP_ACK_BUF_LOW_80M;
    g_host_init_params[WLAN_CFG_INIT_TCP_ACK_BUF_HIGH_160M] = WLAN_TCP_ACK_BUF_HIGH_160M;
    g_host_init_params[WLAN_CFG_INIT_TCP_ACK_BUF_LOW_160M] = WLAN_TCP_ACK_BUF_LOW_160M;

    g_host_init_params[WLAN_CFG_INIT_TX_TCP_ACK_BUF_USERCTL] = OAL_FALSE;
    g_host_init_params[WLAN_CFG_INIT_TCP_ACK_BUF_USERCTL_HIGH] = WLAN_TCP_ACK_BUF_USERCTL_HIGH;
    g_host_init_params[WLAN_CFG_INIT_TCP_ACK_BUF_USERCTL_LOW] = WLAN_TCP_ACK_BUF_USERCTL_LOW;

    g_host_init_params[WLAN_CFG_INIT_RX_DYN_BYPASS_EXTLNA] = OAL_FALSE;
    g_host_init_params[WLAN_CFG_INIT_RX_DYN_BYPASS_EXTLNA_HIGH] = WLAN_RX_DYN_BYPASS_EXTLNA_HIGH;
    g_host_init_params[WLAN_CFG_INIT_RX_DYN_BYPASS_EXTLNA_LOW] = WLAN_RX_DYN_BYPASS_EXTLNA_LOW;
    g_host_init_params[WLAN_CFG_INIT_RX_AMPDU_AMSDU_SKB] = OAL_FALSE;
    g_host_init_params[WLAN_CFG_INIT_RX_AMPDU_BITMAP] = OAL_FALSE;
    g_host_init_params[WLAN_CFG_INIT_HIEX_CAP] = WLAN_HIEX_DEV_CAP;
    g_host_init_params[WLAN_CFG_INIT_SDIO_D2H_ASSEMBLE_COUNT] = HISDIO_DEV2HOST_SCATT_MAX;
    g_host_init_params[WLAN_CFG_INIT_SDIO_H2D_ASSEMBLE_COUNT] = WLAN_SDIO_H2D_ASSEMBLE_COUNT_VAL;
    g_host_init_params[WLAN_CFG_INIT_FTM_CAP] = OAL_FALSE;
}
/*
 * ��������  : �����ƻ�����ȫ������ g_host_init_params ����ֵ
 *             ini�ļ���ȡʧ��ʱ�ó�ֵ
 */
OAL_STATIC oal_void host_btcoex_rssi_th_params_init(oal_void)
{
    /* ldac m2s rssi */
    g_host_init_params[WLAN_CFG_INIT_LDAC_THRESHOLD_M2S] = WLAN_BTCOEX_THRESHOLD_MCM_DOWN; /* Ĭ��������ޣ���֧�� */
    g_host_init_params[WLAN_CFG_INIT_LDAC_THRESHOLD_S2M] = WLAN_BTCOEX_THRESHOLD_MCM_UP;

    /* mcm btcoex rssi */
    g_host_init_params[WLAN_CFG_INIT_BTCOEX_THRESHOLD_MCM_DOWN] = WLAN_BTCOEX_THRESHOLD_MCM_DOWN; /* Ĭ��������ޣ���֧�� */
    g_host_init_params[WLAN_CFG_INIT_BTCOEX_THRESHOLD_MCM_UP] = WLAN_BTCOEX_THRESHOLD_MCM_UP;
}

/*
 * �� �� ��  : host_params_init_first
 * ��������  : �����ƻ�����ȫ������ g_host_init_params ����ֵ
 *             ini�ļ���ȡʧ��ʱ�ó�ֵ
 */
OAL_STATIC oal_void host_params_init_first(oal_void)
{
    oal_uint8 uc_param_idx;

    /* ROAM */
    g_host_init_params[WLAN_CFG_INIT_ROAM_SWITCH] = WLAN_ROAM_SWITCH_MODE;
    g_host_init_params[WLAN_CFG_INIT_SCAN_ORTHOGONAL] = WLAN_SCAN_ORTHOGONAL_VAL;
    g_host_init_params[WLAN_CFG_INIT_TRIGGER_B] = WLAN_TRIGGER_B_VAL;
    g_host_init_params[WLAN_CFG_INIT_TRIGGER_A] = WLAN_TRIGGER_A_VAL;
    g_host_init_params[WLAN_CFG_INIT_DELTA_B] = WLAN_DELTA_B_VAL;
    g_host_init_params[WLAN_CFG_INIT_DELTA_A] = WLAN_DELTA_A_VAL;

    /* ���� */
    g_host_init_params[WLAN_CFG_INIT_AMPDU_TX_MAX_NUM] = WLAN_AMPDU_TX_MAX_BUF_SIZE;
    g_host_init_params[WLAN_CFG_INIT_USED_MEM_FOR_START] = WLAN_MEM_FOR_START;
    g_host_init_params[WLAN_CFG_INIT_USED_MEM_FOR_STOP] = WLAN_MEM_FOR_STOP;
    g_host_init_params[WLAN_CFG_INIT_RX_ACK_LIMIT] = WLAN_RX_ACK_LIMIT_VAL;
    /* LINKLOSS */
    g_host_init_params[WLAN_CFG_INIT_LINK_LOSS_THRESHOLD_BT] = WLAN_LOSS_THRESHOLD_WLAN_BT;
    g_host_init_params[WLAN_CFG_INIT_LINK_LOSS_THRESHOLD_DBAC] = WLAN_LOSS_THRESHOLD_WLAN_DBAC;
    g_host_init_params[WLAN_CFG_INIT_LINK_LOSS_THRESHOLD_NORMAL] = WLAN_LOSS_THRESHOLD_WLAN_NORMAL;
    /* �Զ���Ƶ */
    host_auto_freq_params_init();

#ifdef _PRE_WLAN_FEATURE_MULTI_NETBUF_AMSDU
    host_amsdu_th_params_init();
#endif

    host_params_performance_init();

    /* �͹��� */
    g_host_init_params[WLAN_CFG_INIT_POWERMGMT_SWITCH] = OAL_TRUE;
    g_host_init_params[WLAN_CFG_INIT_PS_MODE] = WLAN_PS_MODE;
    g_host_init_params[WLAN_CFG_INIT_MIN_FAST_PS_IDLE] = WLAN_MIN_FAST_PS_IDLE;
    g_host_init_params[WLAN_CFG_INIT_MAX_FAST_PS_IDLE] = WLAN_MAX_FAST_PS_IDLE;
    g_host_init_params[WLAN_CFG_INIT_AUTO_FAST_PS_THRESH_SCREENON] = WLAN_AUTO_FAST_PS_SCREENON;
    g_host_init_params[WLAN_CFG_INIT_AUTO_FAST_PS_THRESH_SCREENOFF] = WLAN_AUTO_FAST_PS_SCREENOFF;

    /* ��ά�ɲ� */
    /* ��־���� */
    g_host_init_params[WLAN_CFG_INIT_LOGLEVEL] = OAM_LOG_LEVEL_WARNING;
    /* 2G RFǰ�� */
    g_host_init_params[WLAN_CFG_INIT_RF_RX_INSERTION_LOSS_2G_BAND1] = 0xF4F4;
    g_host_init_params[WLAN_CFG_INIT_RF_RX_INSERTION_LOSS_2G_BAND2] = 0xF4F4;
    g_host_init_params[WLAN_CFG_INIT_RF_RX_INSERTION_LOSS_2G_BAND3] = 0xF4F4;
    /* 5G RFǰ�� */
    g_host_init_params[WLAN_CFG_INIT_RF_RX_INSERTION_LOSS_5G_BAND1] = 0xF8F8;
    g_host_init_params[WLAN_CFG_INIT_RF_RX_INSERTION_LOSS_5G_BAND2] = 0xF8F8;
    g_host_init_params[WLAN_CFG_INIT_RF_RX_INSERTION_LOSS_5G_BAND3] = 0xF8F8;
    g_host_init_params[WLAN_CFG_INIT_RF_RX_INSERTION_LOSS_5G_BAND4] = 0xF8F8;
    g_host_init_params[WLAN_CFG_INIT_RF_RX_INSERTION_LOSS_5G_BAND5] = 0xF8F8;
    g_host_init_params[WLAN_CFG_INIT_RF_RX_INSERTION_LOSS_5G_BAND6] = 0xF8F8;
    g_host_init_params[WLAN_CFG_INIT_RF_RX_INSERTION_LOSS_5G_BAND7] = 0xF8F8;
    /* fem */
    g_host_init_params[WLAN_CFG_INIT_RF_LNA_BYPASS_GAIN_DB_2G] = 0xFFF4FFF4;
    g_host_init_params[WLAN_CFG_INIT_RF_LNA_GAIN_DB_2G] = 0x00140014;
    g_host_init_params[WLAN_CFG_INIT_RF_PA_GAIN_DB_B0_2G] = 0xFFF4FFF4;
    g_host_init_params[WLAN_CFG_INIT_RF_PA_GAIN_DB_B1_2G] = 0xFFF4FFF4;
    g_host_init_params[WLAN_CFG_INIT_RF_PA_GAIN_LVL_2G] = 0x00010001;
    g_host_init_params[WLAN_CFG_INIT_EXT_SWITCH_ISEXIST_2G] = 0x00010001;
    g_host_init_params[WLAN_CFG_INIT_EXT_PA_ISEXIST_2G] = 0x00010001;
    g_host_init_params[WLAN_CFG_INIT_EXT_LNA_ISEXIST_2G] = 0x00010001;
    g_host_init_params[WLAN_CFG_INIT_LNA_ON2OFF_TIME_NS_2G] = 0x02760276;
    g_host_init_params[WLAN_CFG_INIT_LNA_OFF2ON_TIME_NS_2G] = 0x01400140;
    g_host_init_params[WLAN_CFG_INIT_RF_LNA_BYPASS_GAIN_DB_5G] = 0xFFF4FFF4;
    g_host_init_params[WLAN_CFG_INIT_RF_LNA_GAIN_DB_5G] = 0x00140014;
    g_host_init_params[WLAN_CFG_INIT_RF_PA_GAIN_DB_B0_5G] = 0xFFF4FFF4;
    g_host_init_params[WLAN_CFG_INIT_RF_PA_GAIN_DB_B1_5G] = 0xFFF4FFF4;
    g_host_init_params[WLAN_CFG_INIT_RF_PA_GAIN_LVL_5G] = 0x00010001;
    g_host_init_params[WLAN_CFG_INIT_EXT_SWITCH_ISEXIST_5G] = 0x00010001;
    g_host_init_params[WLAN_CFG_INIT_EXT_PA_ISEXIST_5G] = 0x00010001;
    g_host_init_params[WLAN_CFG_INIT_EXT_LNA_ISEXIST_5G] = 0x00010001;
    g_host_init_params[WLAN_CFG_INIT_LNA_ON2OFF_TIME_NS_5G] = 0x02760276;
    g_host_init_params[WLAN_CFG_INIT_LNA_OFF2ON_TIME_NS_5G] = 0x01400140;
    /* ���ڶ��ƻ�����PWR RFֵ��ƫ�� */
    for (uc_param_idx = WLAN_CFG_INIT_RF_PWR_REF_RSSI_2G_C0_MULT4;
         uc_param_idx <= WLAN_CFG_INIT_RF_AMEND_RSSI_5G_C1; uc_param_idx++) {
        g_host_init_params[uc_param_idx] = 0;
    }
    /* SCAN */
    g_host_init_params[WLAN_CFG_INIT_RANDOM_MAC_ADDR_SCAN] = 1;
    /* 11AC2G */
    g_host_init_params[WLAN_CFG_INIT_11AC2G_ENABLE] = 1;
    g_host_init_params[WLAN_CFG_INIT_DISABLE_CAPAB_2GHT40] = 0;
    g_host_init_params[WLAN_CFG_INIT_DUAL_ANTENNA_ENABLE] = 0;

    /* miracast */
    g_host_init_params[WLAN_CFG_INIT_PROBE_RESP_MODE] = 0x10;
    g_host_init_params[WLAN_CFG_INIT_MIRACAST_SINK_ENABLE] = 0;
    g_host_init_params[WLAN_CFG_INIT_REDUCE_MIRACAST_LOG] = 0;
    g_host_init_params[WLAN_CFG_INIT_CORE_BIND_CAP] = 1;
    g_host_init_params[WLAN_CFG_INIT_FAST_MODE] = 0;

    /* sta keepalive cnt th */
    g_host_init_params[WLAN_CFG_INIT_STA_KEEPALIVE_CNT_TH] = WLAN_STA_KEEPALIVE_CNT_TH;
    g_host_init_params[WLAN_CFG_INIT_FAR_DIST_POW_GAIN_SWITCH] = 1;
    g_host_init_params[WLAN_CFG_INIT_FAR_DIST_DSSS_SCALE_PROMOTE_SWITCH] = 1;
    g_host_init_params[WLAN_CFG_INIT_CHANN_RADIO_CAP] = 0xF;

    g_host_init_params[WLAN_CFG_LTE_GPIO_CHECK_SWITCH] = 0;
    g_host_init_params[WLAN_ATCMDSRV_ISM_PRIORITY] = 0;
    g_host_init_params[WLAN_ATCMDSRV_LTE_RX] = 0;
    g_host_init_params[WLAN_ATCMDSRV_LTE_TX] = 0;
    g_host_init_params[WLAN_ATCMDSRV_LTE_INACT] = 0;
    g_host_init_params[WLAN_ATCMDSRV_ISM_RX_ACT] = 0;
    g_host_init_params[WLAN_ATCMDSRV_BANT_PRI] = 0;
    g_host_init_params[WLAN_ATCMDSRV_BANT_STATUS] = 0;
    g_host_init_params[WLAN_ATCMDSRV_WANT_PRI] = 0;
    g_host_init_params[WLAN_ATCMDSRV_WANT_STATUS] = 0;
    g_host_init_params[WLAN_TX5G_UPC_MIX_GAIN_CTRL_1] = 0;
    g_host_init_params[WLAN_TX5G_UPC_MIX_GAIN_CTRL_2] = 0;
    g_host_init_params[WLAN_TX5G_UPC_MIX_GAIN_CTRL_3] = 0;
    g_host_init_params[WLAN_TX5G_UPC_MIX_GAIN_CTRL_4] = 0;
    g_host_init_params[WLAN_TX5G_UPC_MIX_GAIN_CTRL_5] = 0;
    g_host_init_params[WLAN_TX5G_UPC_MIX_GAIN_CTRL_6] = 0;
    g_host_init_params[WLAN_TX5G_UPC_MIX_GAIN_CTRL_7] = 0;
    /* PA bias */
    g_host_init_params[WLAN_TX2G_PA_GATE_VCTL_REG236] = 0x12081208;
    g_host_init_params[WLAN_TX2G_PA_GATE_VCTL_REG237] = 0x2424292D;
    g_host_init_params[WLAN_TX2G_PA_GATE_VCTL_REG238] = 0x24242023;
    g_host_init_params[WLAN_TX2G_PA_GATE_VCTL_REG239] = 0x24242020;
    g_host_init_params[WLAN_TX2G_PA_GATE_VCTL_REG240] = 0x24242020;
    g_host_init_params[WLAN_TX2G_PA_GATE_VCTL_REG241] = 0x24241B1B;
    g_host_init_params[WLAN_TX2G_PA_GATE_VCTL_REG242] = 0x24241B1B;
    g_host_init_params[WLAN_TX2G_PA_GATE_VCTL_REG243] = 0x24241B1B;
    g_host_init_params[WLAN_TX2G_PA_GATE_VCTL_REG244] = 0x24241B1B;

    g_host_init_params[WLAN_TX2G_PA_VRECT_GATE_THIN_REG253] = 0x14141414;
    g_host_init_params[WLAN_TX2G_PA_VRECT_GATE_THIN_REG254] = 0x13131313;
    g_host_init_params[WLAN_TX2G_PA_VRECT_GATE_THIN_REG255] = 0x12121212;
    g_host_init_params[WLAN_TX2G_PA_VRECT_GATE_THIN_REG256] = 0x12121212;
    g_host_init_params[WLAN_TX2G_PA_VRECT_GATE_THIN_REG257] = 0x12121212;
    g_host_init_params[WLAN_TX2G_PA_VRECT_GATE_THIN_REG258] = 0x12121212;
    g_host_init_params[WLAN_TX2G_PA_VRECT_GATE_THIN_REG259] = 0x12121212;
    g_host_init_params[WLAN_TX2G_PA_VRECT_GATE_THIN_REG260] = 0x12121212;
    g_host_init_params[WLAN_TX2G_PA_VRECT_GATE_THIN_REG261] = 0x0F0F0F0F;
    g_host_init_params[WLAN_TX2G_PA_VRECT_GATE_THIN_REG262] = 0x0D0D0D0D;
    g_host_init_params[WLAN_TX2G_PA_VRECT_GATE_THIN_REG263] = 0x0A0B0A0B;
    g_host_init_params[WLAN_TX2G_PA_VRECT_GATE_THIN_REG264] = 0x0A0A0A0A;
    g_host_init_params[WLAN_TX2G_PA_VRECT_GATE_THIN_REG265] = 0x0A0A0A0A;
    g_host_init_params[WLAN_TX2G_PA_VRECT_GATE_THIN_REG266] = 0x0A0A0A0A;
    g_host_init_params[WLAN_TX2G_PA_VRECT_GATE_THIN_REG267] = 0x0A0A0A0A;
    g_host_init_params[WLAN_TX2G_PA_VRECT_GATE_THIN_REG268] = 0x0A0A0A0A;
    g_host_init_params[WLAN_TX2G_PA_VRECT_GATE_THIN_REG269] = 0x0F0F0F0F;
    g_host_init_params[WLAN_TX2G_PA_VRECT_GATE_THIN_REG270] = 0x0D0D0D0D;
    g_host_init_params[WLAN_TX2G_PA_VRECT_GATE_THIN_REG271] = 0x0A0B0A0B;
    g_host_init_params[WLAN_TX2G_PA_VRECT_GATE_THIN_REG272] = 0x0A0A0A0A;
    g_host_init_params[WLAN_TX2G_PA_VRECT_GATE_THIN_REG273] = 0x0A0A0A0A;
    g_host_init_params[WLAN_TX2G_PA_VRECT_GATE_THIN_REG274] = 0x0A0A0A0A;
    g_host_init_params[WLAN_TX2G_PA_VRECT_GATE_THIN_REG275] = 0x0A0A0A0A;
    g_host_init_params[WLAN_TX2G_PA_VRECT_GATE_THIN_REG276] = 0x0A0A0A0A;
    g_host_init_params[WLAN_TX2G_PA_VRECT_GATE_THIN_REG277] = 0x0D0D0D0D;
    g_host_init_params[WLAN_TX2G_PA_VRECT_GATE_THIN_REG278] = 0x0D0D0D0D;
    g_host_init_params[WLAN_TX2G_PA_VRECT_GATE_THIN_REG279] = 0x0D0D0A0B;
    g_host_init_params[WLAN_TX2G_PA_VRECT_GATE_THIN_REG280_BAND1] = 0x0D0D0A0A;
    g_host_init_params[WLAN_TX2G_PA_VRECT_GATE_THIN_REG280_BAND2] = 0x0D0D0A0A;
    g_host_init_params[WLAN_TX2G_PA_VRECT_GATE_THIN_REG280_BAND3] = 0x0D0D0A0A;
    g_host_init_params[WLAN_TX2G_PA_VRECT_GATE_THIN_REG281] = 0x0D0D0A0A;
    g_host_init_params[WLAN_TX2G_PA_VRECT_GATE_THIN_REG282] = 0x0D0D0A0A;
    g_host_init_params[WLAN_TX2G_PA_VRECT_GATE_THIN_REG283] = 0x0D0D0A0A;
    g_host_init_params[WLAN_TX2G_PA_VRECT_GATE_THIN_REG284] = 0x0D0D0A0A;
    host_btcoex_rssi_th_params_init();
    /* connect */
    g_host_init_params[WLAN_CFG_INIT_DDR_FREQ] = WLAN_DDR_CAHNL_FREQ;

#ifdef _PRE_WLAN_FEATURE_NRCOEX
    /* 5g nr coex */
    g_host_init_params[WLAN_CFG_INIT_NRCOEX_ENABLE]                  = 0;
    g_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE0_FREQ]              = 0x00000000;
    g_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE0_40M_20M_GAP0]      = 0x00000000;
    g_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE0_160M_80M_GAP0]     = 0x00000000;
    g_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE0_40M_20M_GAP1]      = 0x00000000;
    g_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE0_160M_80M_GAP1]     = 0x00000000;
    g_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE0_40M_20M_GAP2]      = 0x00000000;
    g_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE0_160M_80M_GAP2]     = 0x00000000;
    g_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE0_SMALLGAP0_ACT]     = 0xFFFFFFFF;
    g_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE0_GAP01_ACT]         = 0xFFFFFFFF;
    g_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE0_GAP12_ACT]         = 0xFFFFFFFF;
    g_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE0_RXSLOT_RSSI]       = 0xFFFFFFFF;
    g_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE1_FREQ]              = 0x00000000;
    g_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE1_40M_20M_GAP0]      = 0x00000000;
    g_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE1_160M_80M_GAP0]     = 0x00000000;
    g_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE1_40M_20M_GAP1]      = 0x00000000;
    g_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE1_160M_80M_GAP1]     = 0x00000000;
    g_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE1_40M_20M_GAP2]      = 0x00000000;
    g_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE1_160M_80M_GAP2]     = 0x00000000;
    g_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE1_SMALLGAP0_ACT]     = 0xFFFFFFFF;
    g_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE1_GAP01_ACT]         = 0xFFFFFFFF;
    g_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE1_GAP12_ACT]         = 0xFFFFFFFF;
    g_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE1_RXSLOT_RSSI]       = 0xFFFFFFFF;
    g_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE2_FREQ]              = 0x00000000;
    g_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE2_40M_20M_GAP0]      = 0x00000000;
    g_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE2_160M_80M_GAP0]     = 0x00000000;
    g_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE2_40M_20M_GAP1]      = 0x00000000;
    g_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE2_160M_80M_GAP1]     = 0x00000000;
    g_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE2_40M_20M_GAP2]      = 0x00000000;
    g_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE2_160M_80M_GAP2]     = 0x00000000;
    g_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE2_SMALLGAP0_ACT]     = 0xFFFFFFFF;
    g_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE2_GAP01_ACT]         = 0xFFFFFFFF;
    g_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE2_GAP12_ACT]         = 0xFFFFFFFF;
    g_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE2_RXSLOT_RSSI]       = 0xFFFFFFFF;
    g_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE3_FREQ]              = 0x00000000;
    g_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE3_40M_20M_GAP0]      = 0x00000000;
    g_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE3_160M_80M_GAP0]     = 0x00000000;
    g_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE3_40M_20M_GAP1]      = 0x00000000;
    g_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE3_160M_80M_GAP1]     = 0x00000000;
    g_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE3_40M_20M_GAP2]      = 0x00000000;
    g_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE3_160M_80M_GAP2]     = 0x00000000;
    g_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE3_SMALLGAP0_ACT]     = 0xFFFFFFFF;
    g_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE3_GAP01_ACT]         = 0xFFFFFFFF;
    g_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE3_GAP12_ACT]         = 0xFFFFFFFF;
    g_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE3_RXSLOT_RSSI]       = 0xFFFFFFFF;
#endif
}

/*
 * �� �� ��  : hwifi_get_regdomain_from_country_code
 * ��������  : ���ݹ������ҵ���Ӧ��regdomain
 */
regdomain_enum_uint8 hwifi_get_regdomain_from_country_code(const countrycode_t country_code)
{
    regdomain_enum_uint8 en_regdomain = REGDOMAIN_COMMON;
    int32 table_idx = 0;

    while (g_country_info_table[table_idx].en_regdomain != REGDOMAIN_COUNT) {
        if (oal_memcmp(country_code, g_country_info_table[table_idx].auc_country_code, COUNTRY_CODE_LEN) == 0) {
            en_regdomain = g_country_info_table[table_idx].en_regdomain;
            break;
        }
        ++table_idx;
    }

    return en_regdomain;
}

/*
 * �� �� ��  : hwifi_is_regdomain_changed
 * ��������  : ������ı�󣬶�Ӧ��regdomain�Ƿ��б仯
 */
int32 hwifi_is_regdomain_changed(const countrycode_t country_code_old, const countrycode_t country_code_new)
{
    return hwifi_get_regdomain_from_country_code(country_code_old) !=
           hwifi_get_regdomain_from_country_code(country_code_new);
}

/*
 * �� �� ��  : hwifi_custom_adapt_device_ini_freq_param
 * ��������  : ��ʼ��device�ඨ�ƻ�iniƵ����ص�������
 */
int32 hwifi_custom_adapt_device_ini_freq_param(oal_uint8 *puc_data, oal_uint32 *pul_data_len)
{
    config_device_freq_h2d_stru st_device_freq_data = {0};
    oal_uint8 uc_index;
    oal_int32 l_val;
    oal_uint32 cfg_id;
    oal_int32 l_ret;
    hmac_to_dmac_cfg_custom_data_stru st_syn_msg = {0};
    const oal_uint32 ul_dev_data_buff_size = sizeof(st_device_freq_data.st_device_data) /
                                             sizeof(st_device_freq_data.st_device_data[0]);

    if (puc_data == NULL) {
        oam_error_log1(0, OAM_SF_CFG, "{hwifi_custom_adapt_device_ini_freq_param puc_data is NULL last data_len[%d].}",
                       *pul_data_len);
        return INI_FAILED;
    }

    st_syn_msg.en_syn_id = CUSTOM_CFGID_INI_FREQ_ID;

    for (uc_index = 0, cfg_id = WLAN_CFG_INIT_PSS_THRESHOLD_LEVEL_0; uc_index < ul_dev_data_buff_size; uc_index++) {
        l_val = hwifi_get_init_value(CUS_TAG_INI, cfg_id);
        if (PPS_VALUE_0 <= l_val && l_val <= PPS_VALUE_3) {
            st_device_freq_data.st_device_data[uc_index].ul_speed_level = (oal_uint32)l_val;
            cfg_id += WLAN_CFG_ID_OFFSET;
        } else {
            oam_error_log1(0, OAM_SF_CFG,
                           "{hwifi_custom_adapt_device_ini_freq_param get wrong PSS_THRESHOLD_LEVEL[%d]!}", l_val);
            return OAL_FALSE;
        }
    }

    for (uc_index = 0, cfg_id = WLAN_CFG_INIT_DEVICE_TYPE_LEVEL_0; uc_index < ul_dev_data_buff_size; uc_index++) {
        l_val = hwifi_get_init_value(CUS_TAG_INI, cfg_id);
        if (l_val >= FREQ_IDLE && l_val <= FREQ_HIGHEST) {
            st_device_freq_data.st_device_data[uc_index].ul_cpu_freq_level = (oal_uint32)l_val;
            cfg_id++;
        } else {
            oam_error_log1(0, OAM_SF_CFG,
                           "{hwifi_custom_adapt_device_ini_freq_param get wrong DEVICE_TYPE_LEVEL [%d]!}", l_val);
            return OAL_FALSE;
        }
    }
    st_device_freq_data.uc_set_type = FREQ_SYNC_DATA;

    st_syn_msg.ul_len = OAL_SIZEOF(st_device_freq_data);

    l_ret = memcpy_s(puc_data, (WLAN_LARGE_NETBUF_SIZE - *pul_data_len), &st_syn_msg, CUSTOM_MSG_DATA_HDR_LEN);
    l_ret += memcpy_s(puc_data + CUSTOM_MSG_DATA_HDR_LEN,
                      (WLAN_LARGE_NETBUF_SIZE - *pul_data_len - CUSTOM_MSG_DATA_HDR_LEN),
                      &st_device_freq_data,
                      OAL_SIZEOF(st_device_freq_data));
    if (l_ret != EOK) {
        oam_error_log2(0, OAM_SF_CFG, "{hwifi_custom_adapt_device_ini_freq_param::memcpy_s fail[%d]. data_len[%d]}",
                       l_ret, *pul_data_len);
        *pul_data_len += (st_syn_msg.ul_len + CUSTOM_MSG_DATA_HDR_LEN);
        return OAL_FAIL;
    }

    *pul_data_len += (st_syn_msg.ul_len + CUSTOM_MSG_DATA_HDR_LEN);
    oam_warning_log1(0, OAM_SF_CFG, "{hwifi_custom_adapt_device_ini_freq_param da_len[%d].}", *pul_data_len);

    return OAL_SUCC;
}

/*
 * �� �� ��  : hwifi_custom_adapt_ini_device_perf_param
 * ��������  : ����device���ƻ�������ʼ��
 */
OAL_STATIC oal_void hwifi_custom_adapt_device_ini_perf_param(oal_uint8 *puc_data, oal_uint32 *pul_data_len)
{
    const oal_uint32 ul_tmp_len = 8;
    const oal_uint32 ul_itoa_len = 5; /* ����ת�ַ���֮��ĳ��� */
    oal_int8 ac_tmp[ul_tmp_len];
    oal_uint8 uc_sdio_assem_h2d;
    oal_uint8 uc_sdio_assem_d2h;
    oal_int32 l_ret;
    config_device_perf_h2d_stru st_device_perf;
    hmac_to_dmac_cfg_custom_data_stru st_syn_msg = {0};

    if (puc_data == NULL) {
        oam_error_log1(0, OAM_SF_CFG, "{hwifi_custom_adapt_device_ini_perf_param puc_data is NULL last data_len[%d].}",
                       *pul_data_len);
        return;
    }

    memset_s(ac_tmp, OAL_SIZEOF(ac_tmp), 0, OAL_SIZEOF(ac_tmp));
    memset_s(&st_device_perf, OAL_SIZEOF(st_device_perf), 0, OAL_SIZEOF(st_device_perf));

    st_syn_msg.en_syn_id = CUSTOM_CFGID_INI_PERF_ID;

    /* SDIO FLOWCTRL */
    // device�����Ϸ����ж�
    oal_itoa(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_USED_MEM_FOR_START),
             st_device_perf.ac_used_mem_param, ul_itoa_len);
    oal_itoa(hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_USED_MEM_FOR_STOP), ac_tmp, ul_itoa_len);
    st_device_perf.ac_used_mem_param[OAL_STRLEN(st_device_perf.ac_used_mem_param)] = ' ';
    l_ret = memcpy_s(st_device_perf.ac_used_mem_param + OAL_STRLEN(st_device_perf.ac_used_mem_param),
                     (OAL_SIZEOF(st_device_perf.ac_used_mem_param) - OAL_STRLEN(st_device_perf.ac_used_mem_param)),
                     ac_tmp, OAL_STRLEN(ac_tmp));
    if (l_ret != EOK) {
        oam_error_log1(0, OAM_SF_CFG, "{hwifi_custom_adapt_device_ini_perf_param memcpy_s failed[%d].}", l_ret);
        return;
    }

    st_device_perf.ac_used_mem_param[OAL_STRLEN(st_device_perf.ac_used_mem_param)] = '\0';

    /* SDIO ASSEMBLE COUNT:H2D */
    uc_sdio_assem_h2d = (oal_uint8)hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_SDIO_H2D_ASSEMBLE_COUNT);
    // �ж�ֵ�ĺϷ���
    if (uc_sdio_assem_h2d >= 1 && uc_sdio_assem_h2d <= HISDIO_HOST2DEV_SCATT_MAX) {
        hcc_assemble_count = uc_sdio_assem_h2d;
    } else {
        oam_error_log2(0, OAM_SF_ANY,
                       "{hwifi_custom_adapt_device_ini_perf_param::sdio_assem_h2d[%d] out of range(0,%d], check value in ini file!}\r\n",
                       uc_sdio_assem_h2d, HISDIO_HOST2DEV_SCATT_MAX);
    }

    /* SDIO ASSEMBLE COUNT:D2H */
    uc_sdio_assem_d2h = (oal_uint8)hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_SDIO_D2H_ASSEMBLE_COUNT);
    // �ж�ֵ�ĺϷ���
    if (uc_sdio_assem_d2h >= 1 && uc_sdio_assem_d2h <= HISDIO_DEV2HOST_SCATT_MAX) {
        st_device_perf.uc_sdio_assem_d2h = uc_sdio_assem_d2h;
    } else {
        st_device_perf.uc_sdio_assem_d2h = HISDIO_DEV2HOST_SCATT_MAX;
        oam_error_log2(0, OAM_SF_ANY,
                       "{hwifi_custom_adapt_device_ini_perf_param::sdio_assem_d2h[%d] out of range(0,%d], check value in ini file!}\r\n",
                       uc_sdio_assem_d2h, HISDIO_DEV2HOST_SCATT_MAX);
    }

    st_syn_msg.ul_len = OAL_SIZEOF(st_device_perf);

    l_ret = memcpy_s(puc_data, (WLAN_LARGE_NETBUF_SIZE - *pul_data_len), &st_syn_msg, CUSTOM_MSG_DATA_HDR_LEN);
    l_ret += memcpy_s(puc_data + CUSTOM_MSG_DATA_HDR_LEN,
                      (WLAN_LARGE_NETBUF_SIZE - *pul_data_len - CUSTOM_MSG_DATA_HDR_LEN),
                      &st_device_perf, OAL_SIZEOF(st_device_perf));
    if (l_ret != EOK) {
        oam_error_log2(0, OAM_SF_CFG, "{hwifi_custom_adapt_device_ini_perf_param::memcpy_s fail[%d]. data_len[%d]}",
                       l_ret, *pul_data_len);
        *pul_data_len += (OAL_SIZEOF(st_device_perf) + CUSTOM_MSG_DATA_HDR_LEN);
        return;
    }

    *pul_data_len += (OAL_SIZEOF(st_device_perf) + CUSTOM_MSG_DATA_HDR_LEN);

    oam_warning_log1(0, OAM_SF_CFG, "{hwifi_custom_adapt_device_ini_perf_param::da_len[%d].}", *pul_data_len);
}

/*
 * �� �� ��  : hwifi_custom_adapt_device_ini_end_param
 * ��������  : ���ö��ƻ�����������־
 */
OAL_STATIC oal_void hwifi_custom_adapt_device_ini_end_param(oal_uint8 *puc_data, oal_uint32 *pul_data_len)
{
    oal_int32 l_ret;
    hmac_to_dmac_cfg_custom_data_stru st_syn_msg = {0};

    if (puc_data == NULL) {
        oam_error_log1(0, OAM_SF_CFG, "{hwifi_custom_adapt_device_ini_end_param puc_data::NULL data_len[%d].}",
                       *pul_data_len);
        return;
    }

    st_syn_msg.en_syn_id = CUSTOM_CFGID_INI_ENDING_ID;
    st_syn_msg.ul_len = 0;

    l_ret = memcpy_s(puc_data, (WLAN_LARGE_NETBUF_SIZE - *pul_data_len), &st_syn_msg, OAL_SIZEOF(st_syn_msg));
    if (l_ret != EOK) {
        oam_error_log2(0, OAM_SF_CFG, "{hwifi_custom_adapt_device_ini_end_param::memcpy_s fail[%d]. data_len[%d]}",
                       l_ret, *pul_data_len);
        *pul_data_len += OAL_SIZEOF(st_syn_msg);
        return;
    }

    *pul_data_len += OAL_SIZEOF(st_syn_msg);

    oam_warning_log1(0, OAM_SF_CFG, "{hwifi_custom_adapt_device_ini_end_param::da_len[%d].}", *pul_data_len);
}

/*
 * �� �� ��  : hwifi_custom_adapt_device_ini_linkloss_param
 * ��������  : linkloss���޶��ƻ�
 */
OAL_STATIC oal_void hwifi_custom_adapt_device_ini_linkloss_param(oal_uint8 *puc_data, oal_uint32 *pul_data_len)
{
    oal_uint8 ast_threshold[WLAN_LINKLOSS_MODE_BUTT] = {0};
    oal_int32  l_ret;

    hmac_to_dmac_cfg_custom_data_stru st_syn_msg = {0};

    if (puc_data == NULL) {
        oam_error_log1(0, OAM_SF_CFG, "{hwifi_custom_adapt_device_ini_linkloss_param::puc_data is NULL data_len[%d].}",
                       *pul_data_len);
        return;
    }

    st_syn_msg.en_syn_id = CUSTOM_CFGID_INI_LINKLOSS_ID;

    ast_threshold[WLAN_LINKLOSS_MODE_BT] =
        (oal_uint8)hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_LINK_LOSS_THRESHOLD_BT);
    ast_threshold[WLAN_LINKLOSS_MODE_DBAC] =
        (oal_uint8)hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_LINK_LOSS_THRESHOLD_DBAC);
    ast_threshold[WLAN_LINKLOSS_MODE_NORMAL] =
        (oal_uint8)hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_LINK_LOSS_THRESHOLD_NORMAL);

    st_syn_msg.ul_len = OAL_SIZEOF(ast_threshold);

    l_ret = memcpy_s(puc_data, (WLAN_LARGE_NETBUF_SIZE - *pul_data_len), &st_syn_msg, CUSTOM_MSG_DATA_HDR_LEN);
    l_ret += memcpy_s(puc_data + CUSTOM_MSG_DATA_HDR_LEN,
                      (WLAN_LARGE_NETBUF_SIZE - *pul_data_len - CUSTOM_MSG_DATA_HDR_LEN),
                      &ast_threshold, OAL_SIZEOF(ast_threshold));
    if (l_ret != EOK) {
        oam_error_log2(0, OAM_SF_CFG,
                       "{hwifi_custom_adapt_device_ini_linkloss_param::memcpy_s fail[%d]. data_len[%d]}",
                       l_ret, *pul_data_len);
        *pul_data_len += (OAL_SIZEOF(ast_threshold) + CUSTOM_MSG_DATA_HDR_LEN);
        return;
    }

    *pul_data_len += (OAL_SIZEOF(ast_threshold) + CUSTOM_MSG_DATA_HDR_LEN);

    oam_warning_log1(0, OAM_SF_CFG, "{hwifi_custom_adapt_device_ini_linkloss_param::da_len[%d].}", *pul_data_len);
}

/*
 * �� �� ��  : hwifi_custom_adapt_device_ini_ldac_m2s_rssi_param
 * ��������  : ldac m2s rssi���޶��ƻ�
 */
OAL_STATIC oal_void hwifi_custom_adapt_device_ini_ldac_m2s_rssi_param(oal_uint8 *puc_data, oal_uint32 *pul_data_len)
{
    oal_int8 ast_ldac_m2s_rssi_threshold[WLAN_M2S_LDAC_RSSI_BUTT] = { 0, 0 }; /* ��ǰm2s��s2m���ޣ�������չ�����ö�� */
    oal_int32 l_ret;

    hmac_to_dmac_cfg_custom_data_stru st_syn_msg = {0};

    if (puc_data == NULL) {
        oam_error_log1(0, OAM_SF_CFG,
            "{hwifi_custom_adapt_device_ini_ldac_m2s_rssi_param::puc_data is NULL data_len[%d].}", *pul_data_len);
        return;
    }

    st_syn_msg.en_syn_id = CUSTOM_CFGID_PRIV_INI_LDAC_M2S_TH_ID;

    ast_ldac_m2s_rssi_threshold[WLAN_M2S_LDAC_RSSI_TO_SISO] =
        (oal_int8)hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_LDAC_THRESHOLD_M2S);
    ast_ldac_m2s_rssi_threshold[WLAN_M2S_LDAC_RSSI_TO_MIMO] =
        (oal_int8)hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_LDAC_THRESHOLD_S2M);

    st_syn_msg.ul_len = OAL_SIZEOF(ast_ldac_m2s_rssi_threshold);

    l_ret = memcpy_s(puc_data, (WLAN_LARGE_NETBUF_SIZE - *pul_data_len), &st_syn_msg, CUSTOM_MSG_DATA_HDR_LEN);
    l_ret += memcpy_s(puc_data + CUSTOM_MSG_DATA_HDR_LEN,
                      (WLAN_LARGE_NETBUF_SIZE - *pul_data_len - CUSTOM_MSG_DATA_HDR_LEN),
                      &ast_ldac_m2s_rssi_threshold,
                      OAL_SIZEOF(ast_ldac_m2s_rssi_threshold));
    if (l_ret != EOK) {
        oam_error_log2(0, OAM_SF_CFG,
            "{hwifi_custom_adapt_device_ini_ldac_m2s_rssi_param::memcpy_s fail[%d]. data_len[%d]}",
            l_ret, *pul_data_len);
        *pul_data_len += (OAL_SIZEOF(ast_ldac_m2s_rssi_threshold) + CUSTOM_MSG_DATA_HDR_LEN);
        return;
    }

    *pul_data_len += (OAL_SIZEOF(ast_ldac_m2s_rssi_threshold) + CUSTOM_MSG_DATA_HDR_LEN);

    oam_warning_log3(0, OAM_SF_CFG, "{hwifi_custom_adapt_device_ini_ldac_m2s_rssi_param::da_len[%d], \
        m2s[%d], s2m[%d].}", *pul_data_len,
        ast_ldac_m2s_rssi_threshold[WLAN_M2S_LDAC_RSSI_TO_SISO],
        ast_ldac_m2s_rssi_threshold[WLAN_M2S_LDAC_RSSI_TO_MIMO]);
}

/*
 * �� �� ��  : hwifi_custom_adapt_device_ini_btcoex_mcm_rssi_param
 * ��������  : btcoex mcm rssi���޶��ƻ�
 */
OAL_STATIC oal_void hwifi_custom_adapt_device_ini_btcoex_mcm_rssi_param(oal_uint8 *puc_data, oal_uint32 *pul_data_len)
{
    oal_int8 ast_ldac_m2s_rssi_threshold[WLAN_BTCOEX_RSSI_MCM_BUTT] = { 0, 0 }; /* ��ǰm2s��s2m���ޣ�������չ�����ö�� */
    oal_int32 l_ret;

    hmac_to_dmac_cfg_custom_data_stru st_syn_msg = {0};

    if (puc_data == NULL) {
        oam_error_log1(0, OAM_SF_CFG,
                       "{hwifi_custom_adapt_device_ini_btcoex_mcm_rssi_param::puc_data is NULL data_len[%d].}",
                       *pul_data_len);
        return;
    }

    st_syn_msg.en_syn_id = CUSTOM_CFGID_PRIV_INI_BTCOEX_MCM_TH_ID;

    ast_ldac_m2s_rssi_threshold[WLAN_BTCOEX_RSSI_MCM_DOWN] =
        (oal_int8)hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_BTCOEX_THRESHOLD_MCM_DOWN);
    ast_ldac_m2s_rssi_threshold[WLAN_BTCOEX_RSSI_MCM_UP] =
        (oal_int8)hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_BTCOEX_THRESHOLD_MCM_UP);

    st_syn_msg.ul_len = OAL_SIZEOF(ast_ldac_m2s_rssi_threshold);

    l_ret = memcpy_s(puc_data, (WLAN_LARGE_NETBUF_SIZE - *pul_data_len), &st_syn_msg, CUSTOM_MSG_DATA_HDR_LEN);
    l_ret += memcpy_s(puc_data + CUSTOM_MSG_DATA_HDR_LEN,
                      (WLAN_LARGE_NETBUF_SIZE - *pul_data_len - CUSTOM_MSG_DATA_HDR_LEN),
                      &ast_ldac_m2s_rssi_threshold,
                      OAL_SIZEOF(ast_ldac_m2s_rssi_threshold));
    if (l_ret != EOK) {
        oam_error_log2(0, OAM_SF_CFG,
                       "{hwifi_custom_adapt_device_ini_btcoex_mcm_rssi_param::memcpy_s fail[%d]. data_len[%d]}",
                       l_ret, *pul_data_len);
        *pul_data_len += (OAL_SIZEOF(ast_ldac_m2s_rssi_threshold) + CUSTOM_MSG_DATA_HDR_LEN);
        return;
    }

    *pul_data_len += (OAL_SIZEOF(ast_ldac_m2s_rssi_threshold) + CUSTOM_MSG_DATA_HDR_LEN);

    oam_warning_log3(0, OAM_SF_CFG, "{hwifi_custom_adapt_device_ini_btcoex_mcm_rssi_param::da_len[%d], \
        mcm down[%d], mcm up[%d].}", *pul_data_len,
        ast_ldac_m2s_rssi_threshold[WLAN_BTCOEX_RSSI_MCM_DOWN],
        ast_ldac_m2s_rssi_threshold[WLAN_BTCOEX_RSSI_MCM_UP]);
}

#ifdef _PRE_WLAN_FEATURE_NRCOEX
/*
 * �� �� ��  : hwifi_custom_adapt_device_ini_nrcoex_param
 * ��������  : nr coex���ű��ƻ�
*/
OAL_STATIC oal_void hwifi_custom_adapt_device_ini_nrcoex_param(oal_uint8 *puc_data, oal_uint32 *pul_data_len)
{
    hmac_to_dmac_cfg_custom_data_stru   st_syn_msg = {0};
    nrcoex_cfg_info_stru st_nrcoex_ini_info = {0};
    oal_int32*           p_tmp_value = &st_nrcoex_ini_info.un_nrcoex_rule_data[0].st_nrcoex_rule_data.ul_freq;
    oal_uint32           cfg_id;
    oal_int32            l_ret;

    if (puc_data == NULL) {
        oam_error_log1(0, OAM_SF_CFG,
                       "{hwifi_custom_adapt_device_ini_nrcoex_param::puc_data is NULL data_len[%d].}",
                       *pul_data_len);
        return;
    }

    st_syn_msg.en_syn_id = CUSTOM_CFGID_PRIV_INI_NRCOEX_ID;

    st_nrcoex_ini_info.uc_nrcoex_enable = (oal_uint8)hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_NRCOEX_ENABLE);

    for (cfg_id = WLAN_CFG_INIT_NRCOEX_RULE0_FREQ; cfg_id <= WLAN_CFG_INIT_NRCOEX_RULE3_RXSLOT_RSSI; cfg_id++) {
        if (p_tmp_value > (&st_nrcoex_ini_info.un_nrcoex_rule_data[DMAC_WLAN_NRCOEX_INTERFERE_RULE_NUM - 1].st_nrcoex_rule_data.l_rxslot_rssi)) {
            oam_error_log1(0, OAM_SF_CFG,
                           "{hwifi_custom_adapt_device_ini_nrcoex_param::data overflow, cfg_id:[%d].}",
                           cfg_id);
            return;
        }
        *p_tmp_value = hwifi_get_init_value(CUS_TAG_INI, cfg_id);
        p_tmp_value++;
    }

    st_syn_msg.ul_len = OAL_SIZEOF(st_nrcoex_ini_info);

    l_ret = memcpy_s(puc_data, (WLAN_LARGE_NETBUF_SIZE - *pul_data_len), &st_syn_msg, CUSTOM_MSG_DATA_HDR_LEN);
    l_ret += memcpy_s(puc_data + CUSTOM_MSG_DATA_HDR_LEN,
                      (WLAN_LARGE_NETBUF_SIZE - *pul_data_len - CUSTOM_MSG_DATA_HDR_LEN),
                      &st_nrcoex_ini_info,
                      OAL_SIZEOF(st_nrcoex_ini_info));
    if (l_ret != EOK) {
        oam_error_log2(0, OAM_SF_CFG,
                       "{hwifi_custom_adapt_device_ini_nrcoex_param::memcpy_s fail[%d]. data_len[%d]}",
                       l_ret, *pul_data_len);
        *pul_data_len += (OAL_SIZEOF(st_nrcoex_ini_info) + CUSTOM_MSG_DATA_HDR_LEN);
        return;
    }

    *pul_data_len += (OAL_SIZEOF(st_nrcoex_ini_info) + CUSTOM_MSG_DATA_HDR_LEN);

    oam_warning_log1(0, OAM_SF_CFG, "{hwifi_custom_adapt_device_ini_nrcoex_param::da_len[%d].}", *pul_data_len);
}
#endif

/*
 * �� �� ��  : hwifi_custom_adapt_device_ini_pm_switch_param
 * ��������  : �͹��Ķ��ƻ�
 */
OAL_STATIC oal_void hwifi_custom_adapt_device_ini_pm_switch_param(oal_uint8 *puc_data, oal_uint32 *pul_data_len)
{
    hmac_to_dmac_cfg_custom_data_stru st_syn_msg;
    oal_int32  l_ret;

    if (puc_data == NULL) {
        oam_error_log1(0, OAM_SF_CFG,
                       "{hwifi_custom_adapt_device_ini_pm_switch_param::puc_data is NULL data_len[%d].}",
                       *pul_data_len);
        return;
    }

    st_syn_msg.en_syn_id = CUSTOM_CFGID_INI_PM_SWITCH_ID;

    g_wlan_device_pm_switch = hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_POWERMGMT_SWITCH);
    g_wlan_host_pm_switch = (g_wlan_device_pm_switch == WLAN_DEV_ALL_ENABLE ||
                          g_wlan_device_pm_switch == WLAN_DEV_LIGHT_SLEEP_SWITCH_EN) ? OAL_TRUE : OAL_FALSE;

    st_syn_msg.auc_msg_body[0] = g_wlan_device_pm_switch;
    st_syn_msg.ul_len = OAL_SIZEOF(st_syn_msg) - CUSTOM_MSG_DATA_HDR_LEN;

    l_ret = memcpy_s(puc_data, (WLAN_LARGE_NETBUF_SIZE - *pul_data_len), &st_syn_msg, OAL_SIZEOF(st_syn_msg));
    if (l_ret != EOK) {
        oam_error_log2(0, OAM_SF_CFG,
                       "{hwifi_custom_adapt_device_ini_pm_switch_param::memcpy_s fail[%d]. data_len[%d]}",
                       l_ret, *pul_data_len);
        *pul_data_len += OAL_SIZEOF(st_syn_msg);
        return;
    }

    *pul_data_len += OAL_SIZEOF(st_syn_msg);

    oam_warning_log3(0, OAM_SF_CFG,
                     "{hwifi_custom_adapt_device_ini_pm_switch_param::da_len[%d].device[%d]host[%d]pm switch}",
                     *pul_data_len, g_wlan_device_pm_switch, g_wlan_host_pm_switch);
}

/*
 * �� �� ��  : hwifi_custom_adapt_device_ini_fast_ps_check_cnt
 * ��������  : max ps mode check cnt���ƻ���20ms��ʱ����鼸�Σ�������idle��ʱʱ��
 */
OAL_STATIC oal_void hwifi_custom_adapt_device_ini_fast_ps_check_cnt(oal_uint8 *puc_data, oal_uint32 *pul_data_len)
{
    hmac_to_dmac_cfg_custom_data_stru st_syn_msg;
    oal_int32  l_ret;

    if (puc_data == NULL) {
        oam_error_log1(0, OAM_SF_CFG,
                       "{hwifi_custom_adapt_device_ini_fast_ps_check_cnt::puc_data is NULL data_len[%d].}",
                       *pul_data_len);
        return;
    }

    st_syn_msg.en_syn_id = CUSTOM_CFGID_INI_PS_FAST_CHECK_CNT_ID;

    st_syn_msg.auc_msg_body[0] = g_wlan_min_fast_ps_idle;
    st_syn_msg.auc_msg_body[1] = g_wlan_max_fast_ps_idle;
    st_syn_msg.auc_msg_body[2] = g_wlan_auto_ps_screen_on;
    st_syn_msg.auc_msg_body[3] = g_wlan_auto_ps_screen_off;
    st_syn_msg.ul_len = OAL_SIZEOF(st_syn_msg) - CUSTOM_MSG_DATA_HDR_LEN;

    l_ret = memcpy_s(puc_data, (WLAN_LARGE_NETBUF_SIZE - *pul_data_len), &st_syn_msg, OAL_SIZEOF(st_syn_msg));
    if (l_ret != EOK) {
        oam_error_log2(0, OAM_SF_CFG,
                       "{hwifi_custom_adapt_device_ini_fast_ps_check_cnt::memcpy_s fail[%d]. data_len[%d]}",
                       l_ret, *pul_data_len);
        *pul_data_len += OAL_SIZEOF(st_syn_msg);
        return;
    }

    *pul_data_len += OAL_SIZEOF(st_syn_msg);

    oam_warning_log4(0, OAM_SF_CFG,
                     "{hwifi_custom_adapt_device_ini_fast_ps_check_cnt:fast_ps idle min/max[%d/%d],auto_ps thresh screen on/off[%d/%d]}",
                     g_wlan_min_fast_ps_idle, g_wlan_max_fast_ps_idle,
                     g_wlan_auto_ps_screen_on, g_wlan_auto_ps_screen_off);
}

/*
 * BUCK stay in fastmode
 */
OAL_STATIC void hwifi_custom_adapt_device_ini_fast_mode(uint8_t *data, uint32_t *data_len)
{
    errno_t ret;
    hmac_to_dmac_cfg_custom_data_stru syn_msg = {0};

    if (data == NULL) {
        oam_error_log1(0, OAM_SF_CFG, "{hwifi_custom_adapt_device_ini_fast_mode:: data_len[%d].}", *data_len);
        return;
    }

    syn_msg.en_syn_id = CUSTOM_CFGID_INI_FAST_MODE;
    syn_msg.auc_msg_body[0] = (uint8_t)hwifi_get_init_value(CUS_TAG_INI, WLAN_CFG_INIT_FAST_MODE);
    syn_msg.ul_len = OAL_SIZEOF(syn_msg) - CUSTOM_MSG_DATA_HDR_LEN;
    ret = memcpy_s(data, (WLAN_LARGE_NETBUF_SIZE - *data_len), &syn_msg, OAL_SIZEOF(syn_msg));
    if (ret != EOK) {
        oam_error_log2(0, OAM_SF_CFG,
                       "{hwifi_custom_adapt_device_ini_fast_mode:: memcpy_s fail[%d]. data_len[%d]}",
                       ret, *data_len);
        *data_len += OAL_SIZEOF(syn_msg);
        return;
    }

    *data_len += OAL_SIZEOF(syn_msg);

    oam_warning_log2(0, OAM_SF_CFG, "{hwifi_custom_adapt_device_ini_fast_mode:fast_mode = [%d], data_len = [%d]}",
                     syn_msg.auc_msg_body[0], *data_len);
}

/*
 * �� �� ��  : hwifi_custom_adapt_device_priv_ini_radio_cap_param
 * ��������  : �·�˽�ж�̬/��̬dbdc���ö��ƻ��device
 */
OAL_STATIC oal_int32 hwifi_custom_adapt_device_priv_ini_radio_cap_param(oal_uint8 *puc_data, oal_uint32 *pul_data_len)
{
    oal_int32 l_ret;
    hmac_to_dmac_cfg_custom_data_stru st_syn_msg = {0};
    oal_int32 l_priv_value = 0;
    oal_uint8 uc_cmd_idx;
    oal_uint8 uc_device_idx;
    oal_uint8 auc_wlan_service_device_per_chip[WLAN_SERVICE_DEVICE_MAX_NUM_PER_CHIP] = { WLAN_INIT_DEVICE_RADIO_CAP };

    if (puc_data == NULL) {
        oam_error_log1(0, OAM_SF_CFG,
                       "{hwifi_custom_adapt_device_priv_ini_radio_cap_param::puc_data is NULL data_len[%d].}",
                       *pul_data_len);
        return OAL_FAIL;
    }

    /* Ϊ�˲�Ӱ��host device��ʼ�����������»�ȡ���ƻ��ļ�������ֵ */
    uc_cmd_idx = WLAN_CFG_PRIV_DBDC_RADIO_0;
    for (uc_device_idx = 0; uc_device_idx < WLAN_SERVICE_DEVICE_MAX_NUM_PER_CHIP; uc_device_idx++) {
        l_ret = hwifi_get_init_priv_value(uc_cmd_idx++, &l_priv_value);
        if (l_ret == OAL_SUCC) {
            oam_warning_log1(0, OAM_SF_ANY,
                             "{hwifi_custom_adapt_device_priv_ini_radio_cap_param::WLAN_CFG_PRIV_DBDC_RADIO_0 [%d].}",
                             l_priv_value);
            auc_wlan_service_device_per_chip[uc_device_idx] = (oal_uint8)(oal_uint32)l_priv_value;
        }
    }

    st_syn_msg.en_syn_id = CUSTOM_CFGID_PRIV_INI_RADIO_CAP_ID;
    st_syn_msg.ul_len = OAL_SIZEOF(auc_wlan_service_device_per_chip);

    l_ret = memcpy_s(puc_data, WLAN_LARGE_NETBUF_SIZE, &st_syn_msg, CUSTOM_MSG_DATA_HDR_LEN);
    l_ret += memcpy_s(puc_data + CUSTOM_MSG_DATA_HDR_LEN, (WLAN_LARGE_NETBUF_SIZE - *pul_data_len),
                      auc_wlan_service_device_per_chip,
                      OAL_SIZEOF(auc_wlan_service_device_per_chip));
    if (l_ret != EOK) {
        oam_error_log2(0, OAM_SF_CFG,
                       "{hwifi_custom_adapt_device_priv_ini_radio_cap_param::memcpy_s fail[%d]. data_len[%d]}",
                       l_ret, *pul_data_len);
        *pul_data_len += (OAL_SIZEOF(auc_wlan_service_device_per_chip) + CUSTOM_MSG_DATA_HDR_LEN);
        return OAL_FAIL;
    }

    *pul_data_len += (OAL_SIZEOF(auc_wlan_service_device_per_chip) + CUSTOM_MSG_DATA_HDR_LEN);

    oam_warning_log2(0, OAM_SF_CFG, "{hwifi_custom_adapt_device_priv_ini_radio_cap_param::da_len[%d] radio_cap_0[%d].}",
                     *pul_data_len, auc_wlan_service_device_per_chip[0]);

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_RX_LISTEN_POWER_SAVING
/*
 * �� �� ��  : hwifi_custom_adapt_device_priv_ini_download_pm_param
 * ��������  : �·�˽�п���rx listen ps rssi���ƻ��device
 */
OAL_STATIC oal_int32 hwifi_custom_adapt_device_priv_ini_hal_ps_rssi_param(oal_uint8 *puc_data,
                                                                          oal_uint32 *pul_data_len)
{
    hmac_to_dmac_cfg_custom_data_stru st_syn_msg = {0};
    oal_int32 l_ret;
    oal_int32 l_priv_val = 0;
    oal_uint32 ul_hal_ps_rssi_params = 0;

    if (puc_data == NULL) {
        oam_error_log1(0, OAM_SF_CFG,
                       "{hwifi_custom_adapt_device_priv_ini_hal_ps_rssi_param::puc_data NULL data_len[%d].}",
                       *pul_data_len);
        return OAL_FAIL;
    }

    l_ret = hwifi_get_init_priv_value(WLAN_CFG_PRIV_HAL_PS_RSSI_PARAM, &l_priv_val);
    if (l_ret == OAL_SUCC) {
        ul_hal_ps_rssi_params = (oal_uint32)l_priv_val;
        oal_io_print("hwifi_custom_adapt_device_priv_ini_hal_ps_rssi_param::read hal_ps_rssi_params[%d]l_ret[%d]\r\n",
                     ul_hal_ps_rssi_params, l_ret);
    } else {
        return OAL_FAIL;
    }

    st_syn_msg.en_syn_id = CUSTOM_CFGID_PRIV_INI_HAL_PS_RSSI_ID;
    st_syn_msg.ul_len = OAL_SIZEOF(ul_hal_ps_rssi_params);

    l_ret = memcpy_s(puc_data, (WLAN_LARGE_NETBUF_SIZE - *pul_data_len), &st_syn_msg, CUSTOM_MSG_DATA_HDR_LEN);
    l_ret += memcpy_s(puc_data + CUSTOM_MSG_DATA_HDR_LEN,
                      (WLAN_LARGE_NETBUF_SIZE - *pul_data_len - CUSTOM_MSG_DATA_HDR_LEN),
                      &ul_hal_ps_rssi_params, OAL_SIZEOF(ul_hal_ps_rssi_params));
    if (l_ret != EOK) {
        oam_error_log2(0, OAM_SF_CFG,
                       "{hwifi_custom_adapt_device_priv_ini_hal_ps_rssi_param::memcpy_s fail[%d]. data_len[%d]}",
                       l_ret, *pul_data_len);
        *pul_data_len += (OAL_SIZEOF(ul_hal_ps_rssi_params) + CUSTOM_MSG_DATA_HDR_LEN);

        return OAL_FAIL;
    }

    *pul_data_len += (OAL_SIZEOF(ul_hal_ps_rssi_params) + CUSTOM_MSG_DATA_HDR_LEN);

    oam_warning_log2(0, OAM_SF_CFG,
                     "{hwifi_custom_adapt_device_priv_ini_hal_ps_rssi_param::da_len[%d] ul_hal_pps_rssi_params [%X].}",
                     *pul_data_len, ul_hal_ps_rssi_params);

    return OAL_SUCC;
}

/*
 * �� �� ��  : hwifi_custom_adapt_device_priv_ini_download_pm_param
 * ��������  : �·�˽�п���rx listen ps pps���ƻ��device
 */
OAL_STATIC oal_int32 hwifi_custom_adapt_device_priv_ini_hal_ps_pps_param(oal_uint8 *puc_data,
                                                                         oal_uint32 *pul_data_len)
{
    hmac_to_dmac_cfg_custom_data_stru st_syn_msg = {0};
    oal_int32 l_ret;
    oal_int32 l_priv_val = 0;
    oal_uint32 ul_hal_ps_pps_params = 0;

    if (puc_data == NULL) {
        oam_error_log1(0, OAM_SF_CFG,
                       "{hwifi_custom_adapt_device_priv_ini_hal_ps_pps_param::puc_data NULL data_len[%d].}",
                       *pul_data_len);
        return OAL_FAIL;
    }

    l_ret = hwifi_get_init_priv_value(WLAN_CFG_PRIV_HAL_PS_PPS_PARAM, &l_priv_val);
    if (l_ret == OAL_SUCC) {
        ul_hal_ps_pps_params = (oal_uint32)l_priv_val;
        oal_io_print("hwifi_custom_adapt_device_priv_ini_hal_ps_pps_param::read hal_ps_pps_params[%d]l_ret[%d]\r\n",
                     ul_hal_ps_pps_params, l_ret);
    } else {
        return OAL_FAIL;
    }

    st_syn_msg.en_syn_id = CUSTOM_CFGID_PRIV_INI_HAL_PS_PPS_ID;
    st_syn_msg.ul_len = OAL_SIZEOF(ul_hal_ps_pps_params);

    l_ret = memcpy_s(puc_data, (WLAN_LARGE_NETBUF_SIZE - *pul_data_len), &st_syn_msg, CUSTOM_MSG_DATA_HDR_LEN);
    l_ret += memcpy_s(puc_data + CUSTOM_MSG_DATA_HDR_LEN,
                      (WLAN_LARGE_NETBUF_SIZE - *pul_data_len - CUSTOM_MSG_DATA_HDR_LEN),
                      &ul_hal_ps_pps_params, OAL_SIZEOF(ul_hal_ps_pps_params));
    if (l_ret != EOK) {
        oam_error_log2(0, OAM_SF_CFG,
                       "{hwifi_custom_adapt_device_priv_ini_hal_ps_pps_param::memcpy_s fail[%d]. data_len[%d]}",
                       l_ret, *pul_data_len);
        *pul_data_len += (OAL_SIZEOF(ul_hal_ps_pps_params) + CUSTOM_MSG_DATA_HDR_LEN);

        return OAL_FAIL;
    }

    *pul_data_len += (OAL_SIZEOF(ul_hal_ps_pps_params) + CUSTOM_MSG_DATA_HDR_LEN);

    oam_warning_log2(0, OAM_SF_CFG,
                     "{hwifi_custom_adapt_device_priv_ini_hal_ps_pps_param::da_len[%d] ul_hal_ps_pps_params [%X].}",
                     *pul_data_len, ul_hal_ps_pps_params);

    return OAL_SUCC;
}

#endif

/*
 * �� �� ��  : hwifi_custom_adapt_device_priv_ini_hiex_cap
 * ��������  : �·�˽�п���hiex cap���ƻ��device
 */
OAL_STATIC oal_int32 hwifi_custom_adapt_device_priv_ini_hiex_cap(oal_uint8 *puc_data,
    oal_uint32 *pul_data_len)
{
    hmac_to_dmac_cfg_custom_data_stru st_syn_msg = {0};
    oal_int32 l_ret;
    oal_int32 l_priv_val = 0;
    oal_uint32 ul_hiex_cap = 0;

    if (puc_data == NULL) {
        oam_error_log1(0, OAM_SF_CFG, "{hwifi_custom_adapt_device_priv_ini_hiex_cap::puc_data NULL data_len[%d].}",
            *pul_data_len);
        return OAL_FAIL;
    }
    l_ret = hwifi_get_init_priv_value(WLAN_CFG_PRIV_HIEX_CAP, &l_priv_val);
    if (l_ret == OAL_SUCC) {
        ul_hiex_cap = (oal_uint32)l_priv_val;
        oal_io_print("hwifi_custom_adapt_device_priv_ini_hiex_cap::read hiex_cap[%d]l_ret[%d]\r\n", ul_hiex_cap, l_ret);
    } else {
        return OAL_FAIL;
    }

    st_syn_msg.en_syn_id = CUSTOM_CFGID_PRIV_INI_HIEX_CAP_ID;
    st_syn_msg.ul_len = OAL_SIZEOF(ul_hiex_cap);
    l_ret = memcpy_s(puc_data, (WLAN_LARGE_NETBUF_SIZE - *pul_data_len), &st_syn_msg, CUSTOM_MSG_DATA_HDR_LEN);
    l_ret += memcpy_s(puc_data + CUSTOM_MSG_DATA_HDR_LEN,
        (WLAN_LARGE_NETBUF_SIZE - *pul_data_len - CUSTOM_MSG_DATA_HDR_LEN), &ul_hiex_cap, OAL_SIZEOF(ul_hiex_cap));
    if (l_ret != EOK) {
        oam_error_log2(0, OAM_SF_CFG, "{hwifi_custom_adapt_device_priv_ini_hiex_cap::memcpy_s fail[%d] data_len[%d]}",
            l_ret, *pul_data_len);
        *pul_data_len += (OAL_SIZEOF(ul_hiex_cap) + CUSTOM_MSG_DATA_HDR_LEN);
        return OAL_FAIL;
    }

    *pul_data_len += (OAL_SIZEOF(ul_hiex_cap) + CUSTOM_MSG_DATA_HDR_LEN);
    oam_warning_log2(0, OAM_SF_CFG, "{hwifi_custom_adapt_device_priv_ini_hiex_cap::data_len[%d] ul_hiex_cap[0x%X]}",
        *pul_data_len, ul_hiex_cap);

    return OAL_SUCC;
}

/*
 * �� �� ��  : hwifi_custom_adapt_device_priv_ini_temper_thread_param
 * ��������  : �·����±������ö��ƻ��device
 */
OAL_STATIC oal_int32 hwifi_custom_adapt_device_priv_ini_temper_thread_param(oal_uint8 *puc_data,
                                                                            oal_uint32 *pul_data_len)
{
    oal_int32 l_ret;
    hmac_to_dmac_cfg_custom_data_stru st_syn_msg = {0};
    oal_int32 l_priv_val = 0;
    oal_uint32 ul_over_temp_protect_thread;

    if (puc_data == NULL) {
        oam_error_log1(0, OAM_SF_CFG,
                       "{hwifi_custom_adapt_device_priv_ini_temper_thread_param::puc_data is NULL data_len[%d].}",
                       *pul_data_len);
        return OAL_FAIL;
    }

    l_ret = hwifi_get_init_priv_value(WLAN_CFG_PRIV_OVER_TEMPER_PROTECT_THRESHOLD, &l_priv_val);
    if (l_ret == OAL_SUCC) {
        ul_over_temp_protect_thread = (oal_uint32)l_priv_val;
        oal_io_print("hwifi_custom_adapt_device_priv_ini_temper_thread_param::read over_temp_protect_thread[%d]\r\n",
                     ul_over_temp_protect_thread);
    } else {
        return OAL_FAIL;
    }

    st_syn_msg.en_syn_id = CUSTOM_CFGID_PRIV_INI_OVER_TEMPER_PROTECT_THRESHOLD_ID;
    st_syn_msg.ul_len = OAL_SIZEOF(ul_over_temp_protect_thread);

    l_ret = memcpy_s(puc_data, (WLAN_LARGE_NETBUF_SIZE - *pul_data_len), &st_syn_msg, CUSTOM_MSG_DATA_HDR_LEN);
    l_ret += memcpy_s(puc_data + CUSTOM_MSG_DATA_HDR_LEN,
                      (WLAN_LARGE_NETBUF_SIZE - *pul_data_len - CUSTOM_MSG_DATA_HDR_LEN),
                      &ul_over_temp_protect_thread,
                      OAL_SIZEOF(ul_over_temp_protect_thread));
    if (l_ret != EOK) {
        oam_error_log2(0, OAM_SF_CFG,
                       "{hwifi_custom_adapt_device_priv_ini_temper_thread_param::memcpy_s fail[%d]. data_len[%d]}",
                       l_ret, *pul_data_len);
        *pul_data_len += (OAL_SIZEOF(ul_over_temp_protect_thread) + CUSTOM_MSG_DATA_HDR_LEN);
        return OAL_FAIL;
    }

    *pul_data_len += (OAL_SIZEOF(ul_over_temp_protect_thread) + CUSTOM_MSG_DATA_HDR_LEN);

    oam_warning_log2(0, OAM_SF_CFG,
        "{hwifi_custom_adapt_device_priv_ini_temper_thread_param::da_len[%d] over_temp_protect_thread[0x%x].}",
        *pul_data_len, ul_over_temp_protect_thread);

    return OAL_SUCC;
}

oal_uint8 hwifi_custom_cali_ini_param(oal_uint8 uc_priv_cfg_value)
{
    oal_uint8 uc_cali_interval;

    /* ����Ĭ�ϴ�У׼�����ϴ��·� */
    g_wlan_open_cnt++;
    if (g_custom_cali_done == OAL_FALSE) {
        uc_priv_cfg_value |= (CALI_DATA_REFRESH_MASK | CALI_DATA_REUPLOAD_MASK);
    } else if (g_wlan_cal_disable_switch == OAL_TRUE) {
        uc_priv_cfg_value &= (~CALI_DATA_REFRESH_MASK);
    } else {
        if (g_wlan_cal_intvl_enable && (uc_priv_cfg_value & CALI_DATA_REFRESH_MASK)) {
            uc_cali_interval = (uc_priv_cfg_value >> CALI_INTVL_OFFSET) + 1;
            if (g_wlan_open_cnt % uc_cali_interval) {
                uc_priv_cfg_value &= (~CALI_DATA_REFRESH_MASK);
            }
        }
    }

    return uc_priv_cfg_value;
}

OAL_STATIC OAL_INLINE oal_void hwifi_custom_adapt_hcc_flowctrl_type(hmac_to_dmac_cfg_custom_data_stru *pst_syn_msg,
                                                                    oal_uint8 *puc_priv_cfg_value)
{
    if (hcc_bus_flowctrl_init(*puc_priv_cfg_value) != OAL_SUCC) {
        /* GPIO�����ж�ע��ʧ�ܣ�ǿ��deviceʹ��SDIO����(type = 0) */
        *puc_priv_cfg_value = 0;
    }
    pst_syn_msg->en_syn_id = CUSTOM_CFGID_PRIV_INI_HCC_FLOWCTRL_TYPE_ID;
    oal_io_print("hwifi_custom_adapt_mac_device_priv_ini_param::sdio_flow_ctl_type[0x%x].\r\n",
                 *puc_priv_cfg_value);
}

OAL_STATIC oal_void hwifi_custom_adapt_priv_ini_param_extend_etc(
    hmac_to_dmac_cfg_custom_data_stru *pst_syn_msg, wlan_cfg_priv_id_uint8 uc_cfg_id)
{
    switch (uc_cfg_id) {
        case WLAN_CFG_PRIV_PHY_CAP_SWITCH:
            pst_syn_msg->en_syn_id = CUSTOM_CFGID_PRIV_INI_PHY_CAP_SWITCH_ID;
            break;
        case WLAN_CFG_PRIV_OPTIMIZED_FEATURE_SWITCH:
            pst_syn_msg->en_syn_id = CUSTOM_CFGID_PRIV_INI_OPTIMIZED_FEATURE_SWITCH_ID;
            break;
        case WLAN_CFG_PRIV_DDR_SWITCH:
            pst_syn_msg->en_syn_id = CUSTOM_CFGID_PRIV_DDR_SWITCH_ID;
            break;
        case WLAN_CFG_PRIV_TX_SWITCH:
            pst_syn_msg->en_syn_id = CUSTOM_CFGID_PRIV_INI_TX_SWITCH_ID;
            break;
        case WLAN_CFG_PRIV_FTM_CAP:
            pst_syn_msg->en_syn_id = CUSTOM_CFGID_PRIV_INI_FTM_CAP_ID;
            break;

        default:
            break;
    }
}

OAL_STATIC oal_void hwifi_custom_adapt_priv_ini_param_extend(hmac_to_dmac_cfg_custom_data_stru *pst_syn_msg,
                                                             wlan_cfg_priv_id_uint8 uc_cfg_id,
                                                             oal_uint8 *puc_priv_cfg_value)
{
    switch (uc_cfg_id) {
        case WLAN_CFG_PRIV_DYN_BYPASS_EXTLNA:
            pst_syn_msg->en_syn_id = CUSTOM_CFGID_PRIV_DYN_BYPASS_EXTLNA_ID;
            oal_io_print("hwifi_custom_adapt_priv_ini_para_exd::dyn_bps_extlna_enable[%d].\r\n", *puc_priv_cfg_value);
            break;
        case WLAN_CFG_PRIV_CTRL_FRAME_TX_CHAIN:
            pst_syn_msg->en_syn_id = CUSTOM_CFGID_PRIV_CTRL_FRAME_TX_CHAIN_ID;
            oal_io_print("hwifi_custom_adapt_priv_ini_para_exd::Ctrl frame tx chain[%d].\r\n", *puc_priv_cfg_value);
            break;
        case WLAN_CFG_PRIV_CTRL_UPC_FOR_18DBM_CO:
            pst_syn_msg->en_syn_id = CUSTOM_CFGID_PRIV_CTRL_UPC_FOR_18DBM_C0_ID;
            oal_io_print("hwifi_custom_adapt_priv_ini_para_exd::18dBm_upc0[0x%x].\r\n", *puc_priv_cfg_value);
            break;
        case WLAN_CFG_PRIV_CTRL_UPC_FOR_18DBM_C1:
            pst_syn_msg->en_syn_id = CUSTOM_CFGID_PRIV_CTRL_UPC_FOR_18DBM_C1_ID;
            oal_io_print("hwifi_custom_adapt_priv_ini_para_exd::18dBm_upc1[0x%x].\r\n", *puc_priv_cfg_value);
            break;
        case WLAN_CFG_PRIV_CTRL_11B_DOUBLE_CHAIN_BO_POW:
            pst_syn_msg->en_syn_id = CUSTOM_CFGID_PRIV_CTRL_11B_DOUBLE_CHAIN_BO_POW_ID;
            oal_io_print("hwifi_custom_adapt_priv_ini_para_exd::11b_2chain_bo_pow[0x%x].\r\n", *puc_priv_cfg_value);
            break;
        case WLAN_CFG_RADAR_ISR_FORBID:
            pst_syn_msg->en_syn_id = CUSTOM_CFGID_PRIV_RADAR_ISR_FORBID_ID;
            oal_io_print("hwifi_custom_adapt_priv_ini_para_exd::radar isr forbid[%d].\r\n", *puc_priv_cfg_value);
            break;
        case WLAN_CFG_PRIV_HCC_FLOWCTRL_TYPE:
            hwifi_custom_adapt_hcc_flowctrl_type(pst_syn_msg, puc_priv_cfg_value);
            break;
        case WLAN_CFG_PRIV_MBO_SWITCH:
            pst_syn_msg->en_syn_id = CUSTOM_CFGID_PRIV_INI_MBO_SWITCH_ID;
            oal_io_print("hwifi_custom_adapt_priv_ini_para_exd::mbo switch[%d].\r\n", *puc_priv_cfg_value);
            break;
        case WLAN_CFG_PRIV_DYNAMIC_DBAC_SWITCH:
            pst_syn_msg->en_syn_id = CUSTOM_CFGID_PRIV_INI_DYNAMIC_DBAC_ID;
            oal_io_print("hwifi_custom_adapt_priv_ini_para_exd::dynamic_dbac switch[%d].\r\n", *puc_priv_cfg_value);
            break;
        case WLAN_CFG_PRIV_DC_FLOWCTL_SWITCH:
            pst_syn_msg->en_syn_id = CUSTOM_CFGID_PRIV_DC_FLOWCTRL_ID;
            oal_io_print("hwifi_custom_adapt_priv_ini_para_exd::dc flowctl[%d].\r\n",
                         *puc_priv_cfg_value);
            break;

        default:
            break;
    }
    hwifi_custom_adapt_priv_ini_param_extend_etc(pst_syn_msg, uc_cfg_id);
}

OAL_STATIC oal_void hwifi_custom_adapt_priv_11ax_feature_ini_param(hmac_to_dmac_cfg_custom_data_stru *pst_syn_msg,
                                                                   wlan_cfg_priv_id_uint8 uc_cfg_id,
                                                                   oal_uint8 uc_priv_cfg_value)
{
    switch (uc_cfg_id) {
        case WLAN_CFG_PRIV_VOE_SWITCH:
            pst_syn_msg->en_syn_id = CUSTOM_CFGID_PRIV_INI_VOE_SWITCH_ID;
            oal_io_print("hwifi_custom_adapt_mac_device_priv_ini_param::voe switch[%d].\r\n", uc_priv_cfg_value);
            break;
        case WLAN_CFG_PRIV_11AX_SWITCH:
            pst_syn_msg->en_syn_id = CUSTOM_CFGID_PRIV_INI_11AX_SWITCH_ID;
            break;
        case WLAN_CFG_PRIV_HTC_SWITCH:
            pst_syn_msg->en_syn_id = CUSTOM_CFGID_PRIV_INI_HTC_SWITCH_ID;
            break;
        case WLAN_CFG_PRIV_MULTI_BSSID_SWITCH:
            pst_syn_msg->en_syn_id = CUSTOM_CFGID_PRIV_INI_MBSSID_SWITCH_ID;
            break;
        case WLAN_CFG_PRIV_AC_SUSPEND:
            pst_syn_msg->en_syn_id = CUSTOM_CFGID_PRIV_INI_AC_SUSPEND_ID;
            break;
        case WLAN_CFG_PRIV_DYNAMIC_DBAC_SWITCH:
            pst_syn_msg->en_syn_id = CUSTOM_CFGID_PRIV_INI_DYNAMIC_DBAC_ID;
            break;
        default:
            break;
    }
}

OAL_STATIC oal_void hwifi_custom_adapt_priv_feature_ini_param(hmac_to_dmac_cfg_custom_data_stru *pst_syn_msg,
                                                              wlan_cfg_priv_id_uint8 uc_cfg_id, oal_uint8 *puc_priv_cfg_value)
{
    switch (uc_cfg_id) {
        case WLAN_CFG_PRIV_COUNRTYCODE_SELFSTUDY_CFG:
            pst_syn_msg->en_syn_id = CUSTOM_CFGID_PRIV_COUNTRYCODE_SELFSTUDY_CFG_ID;
            oal_io_print("hwifi_custom_adapt_mac_device_priv_ini_param::country code self study[%d].\r\n",
                         *puc_priv_cfg_value);
            break;
        case WLAN_CFG_PRIV_M2S_FUNCTION_MASK:
            pst_syn_msg->en_syn_id = CUSTOM_CFGID_PRIV_M2S_FUNCTION_MASK_ID;
            oal_io_print("hwifi_custom_adapt_mac_device_priv_ini_param::m2s_mask[0x%x].\r\n", *puc_priv_cfg_value);
            break;

        case WLAN_CFG_PRIV_M2S_FUNCTION_EXT_MASK:
            pst_syn_msg->en_syn_id = CUSTOM_CFGID_PRIV_M2S_FUNCTION_EXT_MASK_ID;
            oal_io_print("hwifi_custom_adapt_mac_device_priv_ini_param::m2s_mask_ext[0x%x].\r\n", *puc_priv_cfg_value);
            break;
        case WLAN_CFG_PRIV_MCM_FUNCTION_MASK:
            pst_syn_msg->en_syn_id = CUSTOM_CFGID_PRIV_MCM_FUNCTION_MASK_ID;
            oal_io_print("hwifi_custom_adapt_mac_device_priv_ini_param::mcm_msk[0x%x].\r\n", *puc_priv_cfg_value);
            break;

        case WLAN_CFG_PRIV_MCM_CUSTOM_FUNCTION_MASK:
            pst_syn_msg->en_syn_id = CUSTOM_CFGID_PRIV_MCM_CUSTOM_FUNCTION_MASK_ID;
            oal_io_print("hwifi_custom_adapt_mac_device_priv_ini_param::mcm_custom[0x%x].\r\n", *puc_priv_cfg_value);
            break;
        case WLAN_CFG_PRIV_FASTSCAN_SWITCH:
            pst_syn_msg->en_syn_id = CUSTOM_CFGID_PRIV_FASTSCAN_SWITCH_ID;
            oal_io_print("hwifi_custom_adapt_mac_device_priv_ini_param::fastcan [0x%x].\r\n", *puc_priv_cfg_value);
            break;
        default:
            break;
    }
}

/*
 * �� �� ��  : hwifi_custom_adapt_priv_ini_param_extend
 * ��������  : ��Ȧ���ӶȲ�ֺ���
 */
OAL_STATIC oal_void hwifi_custom_adapt_priv_ini_extend(hmac_to_dmac_cfg_custom_data_stru *pst_syn_msg,
                                                       wlan_cfg_priv_id_uint8 uc_cfg_id,
                                                       oal_uint8 *puc_priv_cfg_value)
{
    switch (uc_cfg_id) {
        case WLAN_CFG_PRIV_DYN_BYPASS_EXTLNA:
        case WLAN_CFG_PRIV_CTRL_FRAME_TX_CHAIN:
        case WLAN_CFG_PRIV_CTRL_UPC_FOR_18DBM_CO:
        case WLAN_CFG_PRIV_CTRL_UPC_FOR_18DBM_C1:
        case WLAN_CFG_PRIV_CTRL_11B_DOUBLE_CHAIN_BO_POW:
        case WLAN_CFG_RADAR_ISR_FORBID:
        case WLAN_CFG_PRIV_HCC_FLOWCTRL_TYPE:
        case WLAN_CFG_PRIV_MBO_SWITCH:
        case WLAN_CFG_PRIV_DC_FLOWCTL_SWITCH:
        case WLAN_CFG_PRIV_PHY_CAP_SWITCH:
        case WLAN_CFG_PRIV_OPTIMIZED_FEATURE_SWITCH:
        case WLAN_CFG_PRIV_DDR_SWITCH:
        case WLAN_CFG_PRIV_TX_SWITCH:
        case WLAN_CFG_PRIV_FTM_CAP:
            hwifi_custom_adapt_priv_ini_param_extend(pst_syn_msg, uc_cfg_id, puc_priv_cfg_value);
            break;
        default:
            break;
    }
}


/*
 * �� �� ��  : hwifi_custom_adapt_priv_ini_param
 * ��������  : �·�˽�п���device���ö��ƻ��device
 */
OAL_STATIC oal_int32 hwifi_custom_adapt_priv_ini_param(wlan_cfg_priv_id_uint8 uc_cfg_id,
                                                       oal_uint8 *puc_data, oal_uint32 *pul_len)
{
    oal_int32 l_ret;
    hmac_to_dmac_cfg_custom_data_stru st_syn_msg = {0};
    oal_int32 l_priv_val = 0;
    oal_uint8 uc_priv_cfg_value;

    if (puc_data == NULL) {
        oam_error_log1(0, OAM_SF_CFG,
                       "{hwifi_custom_adapt_mac_device_priv_ini_param::puc_data is NULL data_len[%d].}", *pul_len);
        return OAL_FAIL;
    }

    l_ret = hwifi_get_init_priv_value(uc_cfg_id, &l_priv_val);
    if (l_ret != OAL_SUCC) {
        return OAL_FAIL;
    }

    uc_priv_cfg_value = (oal_uint8)(oal_uint32)l_priv_val;

    switch (uc_cfg_id) {
        case WLAN_CFG_PRIV_LDPC_CODING:
            st_syn_msg.en_syn_id = CUSTOM_CFGID_PRIV_INI_LDPC_CODING_ID;
            oal_io_print("hwifi_custom_adapt_mac_device_priv_ini_param::ldpc coding[%d].\r\n", uc_priv_cfg_value);
            break;
        case WLAN_CFG_PRIV_BW_MAX_WITH:
            st_syn_msg.en_syn_id = CUSTOM_CFGID_PRIV_INI_BW_MAX_WITH_ID;
            oal_io_print("hwifi_custom_adapt_mac_device_priv_ini_param::max_bw[%d].\r\n", uc_priv_cfg_value);
            break;
        case WLAN_CFG_PRIV_RX_STBC:
            st_syn_msg.en_syn_id = CUSTOM_CFGID_PRIV_INI_RX_STBC_ID;
            oal_io_print("hwifi_custom_adapt_mac_device_priv_ini_param::rx_stbc[%d].\r\n", uc_priv_cfg_value);
            break;
        case WLAN_CFG_PRIV_TX_STBC:
            st_syn_msg.en_syn_id = CUSTOM_CFGID_PRIV_INI_TX_STBC_ID;
            oal_io_print("hwifi_custom_adapt_mac_device_priv_ini_param::tx_stbc[%d].\r\n", uc_priv_cfg_value);
            break;
        case WLAN_CFG_PRIV_SU_BFER:
            st_syn_msg.en_syn_id = CUSTOM_CFGID_PRIV_INI_SU_BFER_ID;
            oal_io_print("hwifi_custom_adapt_mac_device_priv_ini_param::su bfer[%d].\r\n", uc_priv_cfg_value);
            break;
        case WLAN_CFG_PRIV_SU_BFEE:
            st_syn_msg.en_syn_id = CUSTOM_CFGID_PRIV_INI_SU_BFEE_ID;
            oal_io_print("hwifi_custom_adapt_mac_device_priv_ini_param::su bfee[%d].\r\n", uc_priv_cfg_value);
            break;
        case WLAN_CFG_PRIV_MU_BFER:
            st_syn_msg.en_syn_id = CUSTOM_CFGID_PRIV_INI_MU_BFER_ID;
            oal_io_print("hwifi_custom_adapt_mac_device_priv_ini_param::mu bfer[%d].\r\n", uc_priv_cfg_value);
            break;
        case WLAN_CFG_PRIV_MU_BFEE:
            st_syn_msg.en_syn_id = CUSTOM_CFGID_PRIV_INI_MU_BFEE_ID;
            oal_io_print("hwifi_custom_adapt_mac_device_priv_ini_param::mu bfee[%d].\r\n", uc_priv_cfg_value);
            break;
        case WLAN_CFG_PRIV_11N_TXBF:
            st_syn_msg.en_syn_id = CUSTOM_CFGID_PRIV_INI_11N_TXBF_ID;
            oal_io_print("hwifi_custom_adapt_mac_device_priv_ini_param::11n txbf[%d].\r\n", uc_priv_cfg_value);
            break;

        case WLAN_CFG_PRIV_1024_QAM:
            st_syn_msg.en_syn_id = CUSTOM_CFGID_PRIV_INI_1024_QAM_ID;
            oal_io_print("hwifi_custom_adapt_mac_device_priv_ini_param::1024qam[%d].\r\n", uc_priv_cfg_value);
            break;
        case WLAN_CFG_PRIV_CALI_DATA_MASK:
            /* ����Ĭ�ϴ�У׼�����ϴ��·� */
            uc_priv_cfg_value = hwifi_custom_cali_ini_param(uc_priv_cfg_value);
            st_syn_msg.en_syn_id = CUSTOM_CFGID_PRIV_CALI_DATA_MASK_ID;
            oal_io_print("hwifi_custom_adapt_mac_device_priv_ini_param::g_uc_g_wlan_open_cnt[%d]priv_cali_data_up_down[0x%x].\r\n",
                         g_wlan_open_cnt, uc_priv_cfg_value);
            break;
        case WLAN_CFG_PRIV_CALI_AUTOCALI_MASK:
            /* ����Ĭ�ϲ��򿪿���У׼ */
            uc_priv_cfg_value = (g_custom_cali_done == OAL_FALSE) ? OAL_FALSE : uc_priv_cfg_value;
            st_syn_msg.en_syn_id = CUSTOM_CFGID_PRIV_INI_AUTOCALI_MASK_ID;
            oal_io_print("hwifi_custom_adapt_mac_device_priv_ini_param::g_custom_cali_done[%d]auto_cali_mask[0x%x].\r\n",
                         g_custom_cali_done, uc_priv_cfg_value);
            break;

        case WLAN_CFG_PRIV_M2S_FUNCTION_MASK:
        case WLAN_CFG_PRIV_M2S_FUNCTION_EXT_MASK:
        case WLAN_CFG_PRIV_MCM_FUNCTION_MASK:
        case WLAN_CFG_PRIV_MCM_CUSTOM_FUNCTION_MASK:
        case WLAN_CFG_PRIV_COUNRTYCODE_SELFSTUDY_CFG:
        case WLAN_CFG_PRIV_FASTSCAN_SWITCH:
            hwifi_custom_adapt_priv_feature_ini_param(&st_syn_msg, uc_cfg_id, &uc_priv_cfg_value);
            break;

        case WLAN_CFG_ANT_SWITCH:
            st_syn_msg.en_syn_id = CUSTOM_CFGID_PRIV_ANT_SWITCH_ID;
            oal_io_print("hwifi_custom_adapt_mac_device_priv_ini_param::ant switch[%d].\r\n", uc_priv_cfg_value);
            break;
        case WLAN_CFG_PRRIV_LINKLOSS_THRESHOLD_FIXED:
            st_syn_msg.en_syn_id = CUSTOM_CFGID_PRIV_LINKLOSS_THRESHOLD_FIXED_ID;
            oal_io_print("hwifi_custom_adapt_mac_device_priv_ini_param::linkloss threshold fixed[%d].\r\n",
                         uc_priv_cfg_value);
            break;
#ifdef _PRE_WLAN_FEATURE_TXOPPS
        case WLAN_CFG_PRIV_TXOPPS_SWITCH:
            st_syn_msg.en_syn_id = CUSTOM_CFGID_PRIV_INI_TXOPPS_SWITCH_ID;
            oal_io_print("hwifi_custom_adapt_mac_device_priv_ini_param::uc_priv_cfg_value[0x%x].\r\n",
                         uc_priv_cfg_value);
            break;
#endif
        case WLAN_CFG_PRIV_OVER_TEMP_PRO_ENABLE:
            st_syn_msg.en_syn_id = CUSTOM_CFGID_PRIV_INI_TEMP_PRO_ENABLE_ID;
            oal_io_print("hwifi_custom_adapt_mac_device_priv_ini_param::temp pro enable[%d].\r\n", uc_priv_cfg_value);
            break;
        case WLAN_CFG_PRIV_OVER_TEMP_PRO_REDUCE_PWR_ENABLE:
            st_syn_msg.en_syn_id = CUSTOM_CFGID_PRIV_INI_TEMP_PRO_REDUCE_PWR_ENABLE_ID;
            oal_io_print("hwifi_custom_adapt_mac_device_priv_ini_param::temp pro reduce pwr enable[%d].\r\n",
                         uc_priv_cfg_value);
            break;
        case WLAN_CFG_PRIV_OVER_TEMP_PRO_SAFE_TH:
            st_syn_msg.en_syn_id = CUSTOM_CFGID_PRIV_INI_TEMP_PRO_SAFE_TH_ID;
            oal_io_print("hwifi_custom_adapt_mac_device_priv_ini_param::temp pro safe th[%d].\r\n", uc_priv_cfg_value);
            break;
        case WLAN_CFG_PRIV_OVER_TEMP_PRO_OVER_TH:
            st_syn_msg.en_syn_id = CUSTOM_CFGID_PRIV_INI_TEMP_PRO_OVER_TH_ID;
            oal_io_print("hwifi_custom_adapt_mac_device_priv_ini_param::temp pro over th[%d].\r\n", uc_priv_cfg_value);
            break;
        case WLAN_CFG_PRIV_OVER_TEMP_PRO_PA_OFF_TH:
            st_syn_msg.en_syn_id = CUSTOM_CFGID_PRIV_INI_TEMP_PRO_PA_OFF_TH_ID;
            oal_io_print("hwifi_custom_adapt_mac_device_priv_ini_param::temp pro pa off th[%d].\r\n",
                         uc_priv_cfg_value);
            break;

        case WLAN_CFG_PRIV_EVM_PLL_REG_FIX:
            st_syn_msg.en_syn_id = CUSTOM_CFGID_PRIV_INI_EVM_PLL_REG_FIX_ID;
            oal_io_print("hwifi_custom_adapt_mac_device_priv_ini_param::temp pro safe th[%d].\r\n", uc_priv_cfg_value);
            break;
        case WLAN_CFG_PRIV_VOE_SWITCH:
        case WLAN_CFG_PRIV_11AX_SWITCH:
        case WLAN_CFG_PRIV_HTC_SWITCH:
        case WLAN_CFG_PRIV_MULTI_BSSID_SWITCH:
        case WLAN_CFG_PRIV_AC_SUSPEND:
        case WLAN_CFG_PRIV_DYNAMIC_DBAC_SWITCH:
            hwifi_custom_adapt_priv_11ax_feature_ini_param(&st_syn_msg, uc_cfg_id, uc_priv_cfg_value);
            break;

        default:
            break;
    }
    hwifi_custom_adapt_priv_ini_extend(&st_syn_msg, uc_cfg_id, &uc_priv_cfg_value);

    st_syn_msg.ul_len = OAL_SIZEOF(uc_priv_cfg_value);
    l_ret = memcpy_s(puc_data, (WLAN_LARGE_NETBUF_SIZE - *pul_len), &st_syn_msg, CUSTOM_MSG_DATA_HDR_LEN);
    l_ret += memcpy_s(puc_data + CUSTOM_MSG_DATA_HDR_LEN, (WLAN_LARGE_NETBUF_SIZE - *pul_len -
        CUSTOM_MSG_DATA_HDR_LEN), &uc_priv_cfg_value, OAL_SIZEOF(uc_priv_cfg_value));
    *pul_len += (OAL_SIZEOF(uc_priv_cfg_value) + CUSTOM_MSG_DATA_HDR_LEN);

    if (l_ret != EOK) {
        oam_error_log2(0, OAM_SF_CFG,
            "{hwifi_custom_adapt_mac_device_priv_ini_param::memcpy_s fail[%d]. data_len[%d]}", l_ret, *pul_len);
        return OAL_FAIL;
    }
    return OAL_SUCC;
}

/*
 * �� �� ��  : hwifi_custom_adapt_device_priv_ini_dsss2ofdm_dbb_pwr_bo_param
 * ��������  : �·�˽�ж���11b�Ļ��˹���ֵ��device
 */
OAL_STATIC oal_int32 hwifi_custom_adapt_device_priv_ini_dsss2ofdm_dbb_pwr_bo_param(oal_uint8 *puc_data,
                                                                                   oal_uint32 *pul_data_len)
{
    oal_int32 l_ret;
    hmac_to_dmac_cfg_custom_data_stru st_syn_msg = {0};
    oal_int32 l_priv_val = 0;
    oal_int16 l_dsss2ofdm_dbb_pwr_bo;

    if (puc_data == NULL) {
        oam_error_log1(0, OAM_SF_CFG,
            "{hwifi_custom_adapt_device_priv_ini_dsss2ofdm_dbb_pwr_bo_param::puc_data is NULL data_len[%d].}",
            *pul_data_len);
        return OAL_FAIL;
    }

    l_ret = hwifi_get_init_priv_value(WLAN_DSSS2OFDM_DBB_PWR_BO_VAL, &l_priv_val);
    if (l_ret != OAL_SUCC) {
        return OAL_FAIL;
    }

    l_dsss2ofdm_dbb_pwr_bo = (oal_int16)l_priv_val;
    st_syn_msg.en_syn_id = CUSTOM_CFGID_PRIV_INI_DSSS2OFDM_DBB_PWR_BO_VAL_ID;
    st_syn_msg.ul_len = OAL_SIZEOF(l_dsss2ofdm_dbb_pwr_bo);

    l_ret = memcpy_s(puc_data, (WLAN_LARGE_NETBUF_SIZE - *pul_data_len), &st_syn_msg, CUSTOM_MSG_DATA_HDR_LEN);
    l_ret += memcpy_s(puc_data + CUSTOM_MSG_DATA_HDR_LEN,
                      (WLAN_LARGE_NETBUF_SIZE - *pul_data_len - CUSTOM_MSG_DATA_HDR_LEN),
                      &l_dsss2ofdm_dbb_pwr_bo,
                      OAL_SIZEOF(l_dsss2ofdm_dbb_pwr_bo));
    if (l_ret != EOK) {
        oam_error_log2(0, OAM_SF_CFG,
            "{hwifi_custom_adapt_device_priv_ini_dsss2ofdm_dbb_pwr_bo_param::memcpy_s fail[%d]. data_len[%d]}",
            l_ret, *pul_data_len);
        *pul_data_len += (OAL_SIZEOF(l_dsss2ofdm_dbb_pwr_bo) + CUSTOM_MSG_DATA_HDR_LEN);
        return OAL_FAIL;
    }

    *pul_data_len += (OAL_SIZEOF(l_dsss2ofdm_dbb_pwr_bo) + CUSTOM_MSG_DATA_HDR_LEN);

    oam_warning_log2(0, OAM_SF_CFG,
                     "{hwifi_custom_adapt_device_priv_ini_dsss2ofdm_dbb_pwr_bo_param::da_len[%d] l_dsss2ofdm_dbb_pwr_bo[0x%x].}",
                     *pul_data_len, l_dsss2ofdm_dbb_pwr_bo);
    return OAL_SUCC;
}

/*
 * �� �� ��  : hwifi_custom_adapt_device_priv_ini_cali_mask_param
 * ��������  : �·�˽�п���У׼���ö��ƻ��device
 */
OAL_STATIC oal_int32 hwifi_custom_adapt_device_priv_ini_cali_mask_param(oal_uint8 *puc_data, oal_uint32 *pul_data_len)
{
    oal_int32 l_ret;
    hmac_to_dmac_cfg_custom_data_stru st_syn_msg = {0};
    oal_int32 l_priv_val = 0;
    oal_uint32 ul_cali_mask;
#ifdef _PRE_PHONE_RUNMODE_FACTORY
    oal_int32 l_chip_type = get_hi110x_subchip_type();
#endif

    if (puc_data == NULL) {
        oam_error_log1(0, OAM_SF_CFG,
                       "{hwifi_custom_adapt_device_priv_ini_cali_mask_param::puc_data is NULL data_len[%d].}",
                       *pul_data_len);
        return OAL_FAIL;
    }

    l_ret = hwifi_get_init_priv_value(WLAN_CFG_PRIV_CALI_MASK, &l_priv_val);
    if (l_ret == OAL_SUCC) {
        ul_cali_mask = (oal_uint32)l_priv_val;
#ifdef _PRE_PHONE_RUNMODE_FACTORY
    if (l_chip_type == BOARD_VERSION_HI1106) {
        ul_cali_mask = (ul_cali_mask & (~CALI_HI1106_MIMO_MASK)) | CALI_MUL_TIME_CALI_MASK;
    }else if (l_chip_type == BOARD_VERSION_HI1105) {
        ul_cali_mask = (ul_cali_mask & (~CALI_MIMO_MASK)) | CALI_MUL_TIME_CALI_MASK;
    }
#endif
        oal_io_print("hwifi_custom_adapt_device_priv_ini_cali_mask_param::read cali_mask[%d]l_ret[%d]\r\n",
                     ul_cali_mask, l_ret);
    } else {
        return OAL_FAIL;
    }

    st_syn_msg.en_syn_id = CUSTOM_CFGID_PRIV_INI_CALI_MASK_ID;
    st_syn_msg.ul_len = OAL_SIZEOF(ul_cali_mask);

    l_ret = memcpy_s(puc_data, (WLAN_LARGE_NETBUF_SIZE - *pul_data_len), &st_syn_msg, CUSTOM_MSG_DATA_HDR_LEN);
    l_ret += memcpy_s(puc_data + CUSTOM_MSG_DATA_HDR_LEN,
                      (WLAN_LARGE_NETBUF_SIZE - *pul_data_len - CUSTOM_MSG_DATA_HDR_LEN),
                      &ul_cali_mask, OAL_SIZEOF(ul_cali_mask));
    if (l_ret != EOK) {
        oam_error_log2(0, OAM_SF_CFG,
                       "{hwifi_custom_adapt_device_priv_ini_cali_mask_param::memcpy_s fail[%d]. data_len[%d]}",
                       l_ret, *pul_data_len);
        *pul_data_len += (OAL_SIZEOF(ul_cali_mask) + CUSTOM_MSG_DATA_HDR_LEN);

        return OAL_FAIL;
    }

    *pul_data_len += (OAL_SIZEOF(ul_cali_mask) + CUSTOM_MSG_DATA_HDR_LEN);

    oam_warning_log2(0, OAM_SF_CFG, "{hwifi_custom_adapt_device_priv_ini_cali_mask_param::da_len[%d] cali_mask[0x%x].}",
                     *pul_data_len, ul_cali_mask);

    return OAL_SUCC;
}

/*
 * �� �� ��  : hwifi_custom_adapt_device_priv_ini_download_pm_param
 * ��������  : �·�˽�п���У׼���ö��ƻ��device
 */
OAL_STATIC oal_int32 hwifi_custom_adapt_device_priv_ini_download_pm_param(oal_uint8 *puc_data, oal_uint32 *pul_data_len)
{
    oal_int32 l_ret;
    hmac_to_dmac_cfg_custom_data_stru st_syn_msg = {0};
    oal_int32 l_priv_val = 0;
    oal_uint16 us_download_rate_limit_pps;

    if (puc_data == NULL) {
        oam_error_log1(0, OAM_SF_CFG,
                       "{hwifi_custom_adapt_device_priv_ini_download_pm_param::puc_data is NULL data_len[%d].}",
                       *pul_data_len);
        return OAL_FAIL;
    }

    l_ret = hwifi_get_init_priv_value(WLAN_CFG_PRIV_DOWNLOAD_RATE_LIMIT_PPS, &l_priv_val);
    if (l_ret == OAL_SUCC) {
        us_download_rate_limit_pps = (oal_uint16)(oal_uint32)l_priv_val;
        oal_io_print("hwifi_custom_adapt_device_priv_ini_download_pm_param::read download_rate_limit_pps[%d]l_ret[%d]\r\n",
                     us_download_rate_limit_pps, l_ret);
    } else {
        return OAL_FAIL;
    }

    st_syn_msg.en_syn_id = CUSTOM_CFGID_PRIV_INI_DOWNLOAD_RATELIMIT_PPS;
    st_syn_msg.ul_len = OAL_SIZEOF(us_download_rate_limit_pps);

    l_ret = memcpy_s(puc_data, (WLAN_LARGE_NETBUF_SIZE - *pul_data_len), &st_syn_msg, CUSTOM_MSG_DATA_HDR_LEN);
    l_ret += memcpy_s(puc_data + CUSTOM_MSG_DATA_HDR_LEN,
                      (WLAN_LARGE_NETBUF_SIZE - *pul_data_len - CUSTOM_MSG_DATA_HDR_LEN),
                      &us_download_rate_limit_pps, OAL_SIZEOF(us_download_rate_limit_pps));
    if (l_ret != EOK) {
        oam_error_log2(0, OAM_SF_CFG,
                       "{hwifi_custom_adapt_device_priv_ini_download_pm_param::memcpy_s fail[%d]. data_len[%d]}",
                       l_ret, *pul_data_len);
        *pul_data_len += (OAL_SIZEOF(us_download_rate_limit_pps) + CUSTOM_MSG_DATA_HDR_LEN);

        return OAL_FAIL;
    }

    *pul_data_len += (OAL_SIZEOF(us_download_rate_limit_pps) + CUSTOM_MSG_DATA_HDR_LEN);

    oam_warning_log2(0, OAM_SF_CFG,
                     "{hwifi_custom_adapt_device_priv_ini_download_pm_param::da_len[%d] download_rate_limit [%d]pps.}",
                     *pul_data_len, us_download_rate_limit_pps);

    return OAL_SUCC;
}

/*
 * �� �� ��  : hwifi_custom_adapt_device_ini_param
 * ��������  : ini device���ϵ�ǰ���ƻ���������
 */
int32 hwifi_custom_adapt_device_ini_param(oal_uint8 *puc_data)
{
    oal_uint32 ul_data_len = 0;

    if (puc_data == NULL) {
        oam_error_log0(0, OAM_SF_CFG, "{hwifi_custom_adapt_device_ini_param::puc_data is NULL.}");
        return INI_FAILED;
    }
    /*
     * ������Ϣ�ĸ�ʽ����:
     * +-------------------------------------------------------------------+
     * | CFGID0    |DATA0 Length| DATA0 Value | ......................... |
     * +-------------------------------------------------------------------+
     * | 4 Bytes   |4 Byte      | DATA  Length| ......................... |
     * +-------------------------------------------------------------------+
     */
    /* �Զ���Ƶ */
    hwifi_custom_adapt_device_ini_freq_param(puc_data + ul_data_len, &ul_data_len);

    /* ���� */
    hwifi_custom_adapt_device_ini_perf_param(puc_data + ul_data_len, &ul_data_len);

    /* linkloss */
    hwifi_custom_adapt_device_ini_linkloss_param(puc_data + ul_data_len, &ul_data_len);

    /* �͹��� */
    hwifi_custom_adapt_device_ini_pm_switch_param(puc_data + ul_data_len, &ul_data_len);

    /* fast ps mode ������ */
    hwifi_custom_adapt_device_ini_fast_ps_check_cnt(puc_data + ul_data_len, &ul_data_len);

    /* BUCK stay in fastmode */
    hwifi_custom_adapt_device_ini_fast_mode(puc_data + ul_data_len, &ul_data_len);

    /* ldac m2s rssi���� */
    hwifi_custom_adapt_device_ini_ldac_m2s_rssi_param(puc_data + ul_data_len, &ul_data_len);

    /* ldac m2s rssi���� */
    hwifi_custom_adapt_device_ini_btcoex_mcm_rssi_param(puc_data + ul_data_len, &ul_data_len);

#ifdef _PRE_WLAN_FEATURE_NRCOEX
    /* nr coex ���ƻ����� */
    hwifi_custom_adapt_device_ini_nrcoex_param(puc_data + ul_data_len, &ul_data_len);
#endif

    /* ���� */
    hwifi_custom_adapt_device_ini_end_param(puc_data + ul_data_len, &ul_data_len);

    return ul_data_len;
}


/*
 * �� �� ��  : hwifi_custom_adapt_device_priv_ini_param_extend
 * ��������  : ini device���ϵ�ǰ���ƻ���������extend
 */
oal_void hwifi_custom_adapt_device_priv_ini_param_extend(oal_uint8 *puc_data, oal_uint32 ul_data_len,
                                                         oal_uint32 *pul_len)
{
    oal_int32 l_ret;

    hwifi_custom_adapt_device_priv_ini_temper_thread_param(puc_data + ul_data_len, &ul_data_len);
    hwifi_custom_adapt_device_priv_ini_dsss2ofdm_dbb_pwr_bo_param(puc_data + ul_data_len, &ul_data_len);
    hwifi_custom_adapt_priv_ini_param(WLAN_CFG_PRIV_EVM_PLL_REG_FIX, puc_data + ul_data_len, &ul_data_len);
    hwifi_custom_adapt_priv_ini_param(WLAN_CFG_PRIV_VOE_SWITCH, puc_data + ul_data_len, &ul_data_len);
    hwifi_custom_adapt_priv_ini_param(WLAN_CFG_PRIV_11AX_SWITCH, puc_data + ul_data_len, &ul_data_len);
    hwifi_custom_adapt_priv_ini_param(WLAN_CFG_PRIV_HTC_SWITCH, puc_data + ul_data_len, &ul_data_len);
    hwifi_custom_adapt_priv_ini_param(WLAN_CFG_PRIV_MULTI_BSSID_SWITCH, puc_data + ul_data_len, &ul_data_len);
    hwifi_custom_adapt_priv_ini_param(WLAN_CFG_PRIV_AC_SUSPEND, puc_data + ul_data_len, &ul_data_len);
    hwifi_custom_adapt_priv_ini_param(WLAN_CFG_PRIV_DYNAMIC_DBAC_SWITCH, puc_data + ul_data_len, &ul_data_len);
    hwifi_custom_adapt_priv_ini_param(WLAN_CFG_PRIV_DYN_BYPASS_EXTLNA, puc_data + ul_data_len, &ul_data_len);
    hwifi_custom_adapt_priv_ini_param(WLAN_CFG_PRIV_CTRL_FRAME_TX_CHAIN, puc_data + ul_data_len, &ul_data_len);
    hwifi_custom_adapt_priv_ini_param(WLAN_CFG_PRIV_CTRL_UPC_FOR_18DBM_CO, puc_data + ul_data_len, &ul_data_len);
    hwifi_custom_adapt_priv_ini_param(WLAN_CFG_PRIV_CTRL_UPC_FOR_18DBM_C1, puc_data + ul_data_len, &ul_data_len);
    hwifi_custom_adapt_priv_ini_param(WLAN_CFG_PRIV_CTRL_11B_DOUBLE_CHAIN_BO_POW, puc_data + ul_data_len, &ul_data_len);
    hwifi_custom_adapt_priv_ini_param(WLAN_CFG_PRIV_HCC_FLOWCTRL_TYPE, puc_data + ul_data_len, &ul_data_len);
    hwifi_custom_adapt_priv_ini_param(WLAN_CFG_PRIV_MBO_SWITCH, puc_data + ul_data_len, &ul_data_len);
    hwifi_custom_adapt_priv_ini_param(WLAN_CFG_PRIV_DC_FLOWCTL_SWITCH, puc_data + ul_data_len, &ul_data_len);
    hwifi_custom_adapt_priv_ini_param(WLAN_CFG_PRIV_PHY_CAP_SWITCH, puc_data + ul_data_len, &ul_data_len);
    hwifi_custom_adapt_priv_ini_param(WLAN_CFG_PRIV_OPTIMIZED_FEATURE_SWITCH, puc_data + ul_data_len, &ul_data_len);

    l_ret = hwifi_custom_adapt_priv_ini_param(WLAN_CFG_PRIV_FTM_CAP, puc_data + ul_data_len, &ul_data_len);
    if (l_ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_CFG, "{hwifi_custom_adapt_device_priv_ini_param_extend::FTM ini fail.}");
    }

#ifdef _PRE_WLAN_RX_LISTEN_POWER_SAVING
    hwifi_custom_adapt_device_priv_ini_hal_ps_rssi_param(puc_data + ul_data_len, &ul_data_len);
    hwifi_custom_adapt_device_priv_ini_hal_ps_pps_param(puc_data + ul_data_len, &ul_data_len);
#endif
    l_ret = hwifi_custom_adapt_priv_ini_param(WLAN_CFG_PRIV_DDR_SWITCH, puc_data + ul_data_len, &ul_data_len);
    if (l_ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_CFG, "{hwifi_custom_adapt_device_priv_ini_param_extend::ini fail.}");
    }
    l_ret = hwifi_custom_adapt_device_priv_ini_hiex_cap(puc_data + ul_data_len, &ul_data_len);
    if (l_ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_CFG, "{hwifi_custom_adapt_device_priv_ini_param_extend::hiex cap ini fail}");
    }
    hwifi_custom_adapt_priv_ini_param(WLAN_CFG_PRIV_TX_SWITCH, puc_data + ul_data_len, &ul_data_len);

    *pul_len = ul_data_len;
}


OAL_STATIC oal_int32 hwifi_custom_adapt_device_priv_ini_fem_pow_ctl_param(oal_uint8 *puc_data,
    oal_uint32 *pul_data_len)
{
    oal_int32 l_ret;
    hmac_to_dmac_cfg_custom_data_stru st_syn_msg = {0};
    oal_int32 l_priv_fem_ctl_value = 0;
    oal_int32 l_priv_fem_adj_value = 0;
    dmac_ax_fem_pow_ctl_stru st_fem_pow_ctl = {0};

    if (puc_data == NULL) {
        oam_error_log1(0, OAM_SF_CUSTOM, "{hwifi_custom_adapt_device_priv_ini_fem_pow_ctl_param::puc_data is \
        NULL data_len[%d].}", *pul_data_len);
        return OAL_FAIL;
    }

    /* Ϊ��˽�ж��ƻ�ֵ */
    l_ret = hwifi_get_init_priv_value(WLAN_CFG_PRIV_FEM_DELT_POW, &l_priv_fem_ctl_value);
    l_ret += hwifi_get_init_priv_value(WLAN_CFG_PRIV_FEM_ADJ_TPC_TBL_START_IDX, &l_priv_fem_adj_value);
    if (l_ret != OAL_SUCC) {
        return OAL_FAIL;
    }
    st_fem_pow_ctl.uc_fem_delt_pow = l_priv_fem_ctl_value;
    st_fem_pow_ctl.uc_tpc_adj_pow_start_idx = l_priv_fem_adj_value;
    st_syn_msg.en_syn_id = CUSTOM_CFGID_PRIV_FEM_POW_CTL_ID;
    st_syn_msg.ul_len = OAL_SIZEOF(st_fem_pow_ctl);

    l_ret = memcpy_s(puc_data, WLAN_LARGE_NETBUF_SIZE - *pul_data_len, &st_syn_msg, CUSTOM_MSG_DATA_HDR_LEN);
    l_ret += memcpy_s(puc_data + CUSTOM_MSG_DATA_HDR_LEN,
                      (WLAN_LARGE_NETBUF_SIZE - *pul_data_len) - CUSTOM_MSG_DATA_HDR_LEN,
                      &st_fem_pow_ctl, OAL_SIZEOF(st_fem_pow_ctl));
    if (l_ret != EOK) {
        oam_error_log3(0, OAM_SF_CUSTOM, "{hwifi_custom_adapt_device_priv_ini_fem_pow_ctl_param::memcpy_s fail[%d]. \
        data_len[%d] MAX_len[%d]}", l_ret, *pul_data_len, WLAN_LARGE_NETBUF_SIZE);
        return OAL_FAIL;
    }

    *pul_data_len += OAL_SIZEOF(st_fem_pow_ctl) + CUSTOM_MSG_DATA_HDR_LEN;
    oam_warning_log3(0, OAM_SF_CFG,
                     "{hwifi_custom_adapt_device_priv_ini_fem_pow_ctl_param::len[%d]delt_pow[%d] start_idx[%d].}",
                     *pul_data_len, st_fem_pow_ctl.uc_fem_delt_pow, st_fem_pow_ctl.uc_tpc_adj_pow_start_idx);

    return OAL_SUCC;
}

/*
 * �� �� ��  : hwifi_custom_adapt_device_priv_ini_param
 * ��������  : ini device���ϵ�ǰ���ƻ���������
 */
int32 hwifi_custom_adapt_device_priv_ini_param(oal_uint8 *puc_data)
{
    oal_uint32 ul_data_len = 0;

    if (puc_data == NULL) {
        oam_error_log0(0, OAM_SF_CFG, "{hwifi_custom_adapt_device_priv_ini_param::puc_data is NULL.}");
        return INI_FAILED;
    }

    /*
     * ������Ϣ�ĸ�ʽ����:
     * +-------------------------------------------------------------------+
     * | CFGID0    |DATA0 Length| DATA0 Value | ......................... |
     * +-------------------------------------------------------------------+
     * | 4 Bytes   |4 Byte      | DATA  Length| ......................... |
     * +-------------------------------------------------------------------+
     */
    /* ˽�ж��ƻ� */
    hwifi_custom_adapt_device_priv_ini_radio_cap_param(puc_data, &ul_data_len);
    hwifi_custom_adapt_priv_ini_param(WLAN_CFG_PRIV_BW_MAX_WITH, puc_data + ul_data_len, &ul_data_len);
    hwifi_custom_adapt_priv_ini_param(WLAN_CFG_PRIV_LDPC_CODING, puc_data + ul_data_len, &ul_data_len);
    hwifi_custom_adapt_priv_ini_param(WLAN_CFG_PRIV_RX_STBC, puc_data + ul_data_len, &ul_data_len);
    hwifi_custom_adapt_priv_ini_param(WLAN_CFG_PRIV_TX_STBC, puc_data + ul_data_len, &ul_data_len);
    hwifi_custom_adapt_priv_ini_param(WLAN_CFG_PRIV_SU_BFER, puc_data + ul_data_len, &ul_data_len);
    hwifi_custom_adapt_priv_ini_param(WLAN_CFG_PRIV_SU_BFEE, puc_data + ul_data_len, &ul_data_len);
    hwifi_custom_adapt_priv_ini_param(WLAN_CFG_PRIV_MU_BFER, puc_data + ul_data_len, &ul_data_len);
    hwifi_custom_adapt_priv_ini_param(WLAN_CFG_PRIV_MU_BFEE, puc_data + ul_data_len, &ul_data_len);
    hwifi_custom_adapt_priv_ini_param(WLAN_CFG_PRIV_11N_TXBF, puc_data + ul_data_len, &ul_data_len);
    hwifi_custom_adapt_priv_ini_param(WLAN_CFG_PRIV_1024_QAM, puc_data + ul_data_len, &ul_data_len);
    hwifi_custom_adapt_device_priv_ini_cali_mask_param(puc_data + ul_data_len, &ul_data_len);
    hwifi_custom_adapt_priv_ini_param(WLAN_CFG_PRIV_CALI_DATA_MASK, puc_data + ul_data_len, &ul_data_len);
    hwifi_custom_adapt_priv_ini_param(WLAN_CFG_PRIV_CALI_AUTOCALI_MASK, puc_data + ul_data_len, &ul_data_len);
    hwifi_custom_adapt_priv_ini_param(WLAN_CFG_PRIV_OVER_TEMP_PRO_ENABLE, puc_data + ul_data_len, &ul_data_len);
    hwifi_custom_adapt_priv_ini_param(WLAN_CFG_PRIV_OVER_TEMP_PRO_REDUCE_PWR_ENABLE,
                                      puc_data + ul_data_len, &ul_data_len);

    hwifi_custom_adapt_priv_ini_param(WLAN_CFG_PRIV_OVER_TEMP_PRO_SAFE_TH, puc_data + ul_data_len, &ul_data_len);
    hwifi_custom_adapt_priv_ini_param(WLAN_CFG_PRIV_OVER_TEMP_PRO_OVER_TH, puc_data + ul_data_len, &ul_data_len);
    hwifi_custom_adapt_priv_ini_param(WLAN_CFG_PRIV_OVER_TEMP_PRO_PA_OFF_TH, puc_data + ul_data_len, &ul_data_len);
    hwifi_custom_adapt_priv_ini_param(WLAN_CFG_PRIV_FASTSCAN_SWITCH, puc_data + ul_data_len, &ul_data_len);
    hwifi_custom_adapt_priv_ini_param(WLAN_CFG_ANT_SWITCH, puc_data + ul_data_len, &ul_data_len);
    hwifi_custom_adapt_priv_ini_param(WLAN_CFG_PRIV_M2S_FUNCTION_MASK, puc_data + ul_data_len, &ul_data_len);
    hwifi_custom_adapt_priv_ini_param(WLAN_CFG_PRIV_M2S_FUNCTION_EXT_MASK, puc_data + ul_data_len, &ul_data_len);
    hwifi_custom_adapt_priv_ini_param(WLAN_CFG_PRIV_MCM_FUNCTION_MASK, puc_data + ul_data_len, &ul_data_len);
    hwifi_custom_adapt_priv_ini_param(WLAN_CFG_PRIV_MCM_CUSTOM_FUNCTION_MASK, puc_data + ul_data_len, &ul_data_len);
    hwifi_custom_adapt_priv_ini_param(WLAN_CFG_PRRIV_LINKLOSS_THRESHOLD_FIXED, puc_data + ul_data_len, &ul_data_len);
    hwifi_custom_adapt_priv_ini_param(WLAN_CFG_PRIV_COUNRTYCODE_SELFSTUDY_CFG, puc_data + ul_data_len, &ul_data_len);
    hwifi_custom_adapt_priv_ini_param(WLAN_CFG_RADAR_ISR_FORBID, puc_data + ul_data_len, &ul_data_len);
    hwifi_custom_adapt_device_priv_ini_download_pm_param(puc_data + ul_data_len, &ul_data_len);
#ifdef _PRE_WLAN_FEATURE_TXOPPS
    hwifi_custom_adapt_priv_ini_param(WLAN_CFG_PRIV_TXOPPS_SWITCH, puc_data + ul_data_len, &ul_data_len);
#endif
    hwifi_custom_adapt_device_priv_ini_param_extend(puc_data, ul_data_len, &ul_data_len);
    hwifi_custom_adapt_device_priv_ini_fem_pow_ctl_param(puc_data + ul_data_len, &ul_data_len);
    oam_warning_log2(0, OAM_SF_CFG, "{hwifi_custom_adapt_device_priv_ini_param::da_len[%d]MAX[%d].}",
                     ul_data_len, WLAN_LARGE_NETBUF_SIZE);
    return ul_data_len;
}

int32_t hwifi_hcc_custom_get_data(uint16 syn_id, oal_netbuf_stru *netbuf)
{
    uint32_t data_len = 0;
    uint8_t *netbuf_data = (uint8_t *)oal_netbuf_data(netbuf);

    switch (syn_id) {
        case CUSTOM_CFGID_INI_ID:
            /* INI hmac to dmac ������ */
            data_len = hwifi_custom_adapt_device_ini_param(netbuf_data);
            break;
        case CUSTOM_CFGID_PRIV_INI_ID:
            /* ˽�ж��ƻ������� */
            data_len = hwifi_custom_adapt_device_priv_ini_param(netbuf_data);
            break;
        default:
            oam_error_log1(0, OAM_SF_CFG, "hwifi_hcc_custom_ini_data_buf::unknown us_syn_id[%d]", syn_id);
            break;
    }

    return data_len;
}

/*
 * �� �� ��  : hwifi_hcc_custom_netbuf_alloc
 * ��������  : �·����ƻ���������pst_netbuf�ڴ�����
 */
oal_netbuf_stru* hwifi_hcc_custom_netbuf_alloc(void)
{
    oal_netbuf_stru *netbuf;
    uint8_t count;
    // count�����ڴ�����������������3��
    for (count = 1; count <= 3; count++) {
        netbuf = oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF, WLAN_LARGE_NETBUF_SIZE, OAL_NETBUF_PRIORITY_HIGH);
        if (netbuf != NULL) {
            return netbuf;
        }
        oal_msleep(30);    // �ڴ�����ʧ�� ��ʱ30ms
    }
    oam_error_log0(0, OAM_SF_CFG, "hwifi_hcc_custom_netbuf_alloc::alloc netbuf 3 times fail.");
    return NULL;
}

/*
 * �� �� ��  : hwifi_hcc_custom_ini_data_buf
 * ��������  : �·����ƻ���������
 */
int32 hwifi_hcc_custom_ini_data_buf(uint16 us_syn_id)
{
    oal_netbuf_stru *pst_netbuf;
    oal_uint32 ul_data_len;
    oal_int32 l_ret;
    oal_uint32 ul_max_data_len = hcc_get_max_buf_len();

    struct hcc_transfer_param st_hcc_transfer_param = {0};
    struct hcc_handler *hcc = hcc_get_110x_handler();
    if (hcc == NULL) {
        oam_error_log0(0, OAM_SF_CFG, "hwifi_hcc_custom_ini_data_buf hcc::is is null");
        return -OAL_EFAIL;
    }
    pst_netbuf = hwifi_hcc_custom_netbuf_alloc();
    if (pst_netbuf == NULL) {
        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }

    /* ��netbuf */
    ul_data_len = hwifi_hcc_custom_get_data(us_syn_id, pst_netbuf);
    if (ul_data_len > ul_max_data_len) {
        oam_error_log2(0, OAM_SF_CFG,
                       "hwifi_hcc_custom_ini_data_buf::got wrong ul_data_len[%d] max_len[%d]",
                       ul_data_len, ul_max_data_len);
        oal_netbuf_free(pst_netbuf);
        return OAL_FAIL;
    }

    if (ul_data_len == 0) {
        oam_error_log1(0, OAM_SF_CFG, "hwifi_hcc_custom_ini_data_buf::data is null us_syn_id[%d]", us_syn_id);
        oal_netbuf_free(pst_netbuf);
        return OAL_SUCC;
    }

    if ((pst_netbuf->data_len) || (pst_netbuf->data == NULL)) {
        oal_io_print("netbuf:0x%lx, len:%d\r\n", (uintptr_t)pst_netbuf, pst_netbuf->data_len);
        oal_netbuf_free(pst_netbuf);
        return OAL_FAIL;
    }

    oal_netbuf_put(pst_netbuf, ul_data_len);
    hcc_hdr_param_init(&st_hcc_transfer_param, HCC_ACTION_TYPE_CUSTOMIZE,
                       us_syn_id,
                       0,
                       HCC_FC_WAIT,
                       DATA_HI_QUEUE);

    l_ret = (oal_uint32)hcc_tx(hcc, pst_netbuf, &st_hcc_transfer_param);
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_error_log2(0, OAM_SF_CFG, "hwifi_hcc_custom_ini_data_buf fail ret[%d]pst_netbuf[0x%lx]",
                       l_ret, (uintptr_t)pst_netbuf);
        oal_netbuf_free(pst_netbuf);
    }

    return l_ret;
}

oal_int32 hwifi_read_conf_from(oal_uint8 *puc_buffer_cust_nvram, oal_uint8 uc_idx)
{
    oal_int32 l_ret;
#ifdef _PRE_CONFIG_READ_DYNCALI_E2PROM
#ifdef _PRE_SUSPORT_OEMINFO
    l_ret = is_hitv_miniproduct();
    if (l_ret == OAL_SUCC) {
        l_ret = read_conf_from_oeminfo(puc_buffer_cust_nvram, CUS_PARAMS_LEN_MAX,
                    g_wifi_nvram_cfg_handler[uc_idx].ul_eeprom_offset);
    } else {
        l_ret = read_conf_from_eeprom(puc_buffer_cust_nvram, CUS_PARAMS_LEN_MAX,
                    g_wifi_nvram_cfg_handler[uc_idx].ul_eeprom_offset);
    }
#else
    l_ret = read_conf_from_eeprom(puc_buffer_cust_nvram, CUS_PARAMS_LEN_MAX,
        g_wifi_nvram_cfg_handler[uc_idx].ul_eeprom_offset);
#endif
#else
    l_ret = read_conf_from_nvram(puc_buffer_cust_nvram, CUS_PARAMS_LEN_MAX,
        g_wifi_nvram_cfg_handler[uc_idx].ul_nv_map_idx, g_wifi_nvram_cfg_handler[uc_idx].puc_nv_name);
#endif
    return l_ret;
}

/*
 * �� �� ��  : hwifi_custom_host_read_dyn_cali_nvram
 * ��������  : ������ȡnvram�е�dpint��У׼ϵ��ֵ
 */
oal_int32 hwifi_custom_host_read_dyn_cali_nvram(oal_void)
{
    oal_int32 l_ret;
    oal_uint8 uc_idx;
    oal_uint8 uc_param_idx;
    oal_uint8 uc_times_idx = 0;
    oal_int8 *puc_str;
    oal_uint8 *pc_end = ";";
    oal_uint8 *pc_sep = ",";
    oal_int8 *pc_ctx;
    oal_int8 *pc_token;
    oal_int32 l_priv_value;
    oal_bool_enum_uint8 en_get_nvram_data_flag = OAL_FALSE;
    oal_uint8 *puc_buffer_cust_nvram_tmp = OAL_PTR_NULL;
    oal_int32 *pl_params = OAL_PTR_NULL;
    oal_uint8 *puc_g_cust_nvram_info = OAL_PTR_NULL; /* NVRAM���� */
    oal_bool_enum_uint8 tmp_en_fact_cali_completed = OAL_FALSE;

    /* �ж϶��ƻ����Ƿ�ʹ��nvram�еĶ�̬У׼���� */
    l_ret = hwifi_get_init_priv_value(WLAN_CFG_PRIV_CALI_DATA_MASK, &l_priv_value);
    if (l_ret == OAL_SUCC) {
        en_get_nvram_data_flag = !!(HI1103_CUST_READ_NVRAM_MASK & (uint32)l_priv_value);
        if (en_get_nvram_data_flag) {
            oal_io_print("hwifi_custom_host_read_dyn_cali_nvram::get_nvram_data_flag[%d] to abandon nvram data!!\r\n",
                         l_priv_value);
            memset_s(g_cust_nvram_info, OAL_SIZEOF(g_cust_nvram_info), 0, OAL_SIZEOF(g_cust_nvram_info));
            return INI_FILE_TIMESPEC_UNRECONFIG;
        }
    }

    puc_buffer_cust_nvram_tmp = (oal_uint8 *)os_kzalloc_gfp(CUS_PARAMS_LEN_MAX * OAL_SIZEOF(oal_uint8));
    if (puc_buffer_cust_nvram_tmp == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_CUSTOM,
                       "hwifi_custom_host_read_dyn_cali_nvram::puc_buffer_cust_nvram_tmp mem alloc fail!");
        memset_s(g_cust_nvram_info, OAL_SIZEOF(g_cust_nvram_info), 0, OAL_SIZEOF(g_cust_nvram_info));
        return INI_FILE_TIMESPEC_UNRECONFIG;
    }

    pl_params = (oal_int32 *)os_kzalloc_gfp(DY_CALI_PARAMS_NUM * DY_CALI_PARAMS_TIMES * OAL_SIZEOF(oal_int32));
    if (pl_params == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_CUSTOM, "hwifi_custom_host_read_dyn_cali_nvram::ps_params mem alloc fail!");
        os_mem_kfree(puc_buffer_cust_nvram_tmp);
        memset_s(g_cust_nvram_info, OAL_SIZEOF(g_cust_nvram_info), 0, OAL_SIZEOF(g_cust_nvram_info));
        return INI_FILE_TIMESPEC_UNRECONFIG;
    }

    puc_g_cust_nvram_info = (oal_uint8 *)os_kzalloc_gfp(WLAN_CFG_DTS_NVRAM_END *
                                                      CUS_PARAMS_LEN_MAX * OAL_SIZEOF(oal_uint8));
    if (puc_g_cust_nvram_info == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_CUSTOM, "hwifi_custom_host_read_dyn_cali_nvram::puc_g_cust_nvram_info mem alloc fail!");
        os_mem_kfree(puc_buffer_cust_nvram_tmp);
        os_mem_kfree(pl_params);
        memset_s(g_cust_nvram_info, OAL_SIZEOF(g_cust_nvram_info), 0, OAL_SIZEOF(g_cust_nvram_info));
        return INI_FILE_TIMESPEC_UNRECONFIG;
    }

    memset_s(puc_buffer_cust_nvram_tmp,
             CUS_PARAMS_LEN_MAX * OAL_SIZEOF(oal_uint8),
             0,
             CUS_PARAMS_LEN_MAX * OAL_SIZEOF(oal_uint8));
    memset_s(pl_params,
             DY_CALI_PARAMS_NUM * DY_CALI_PARAMS_TIMES * OAL_SIZEOF(oal_int32),
             0,
             DY_CALI_PARAMS_NUM * DY_CALI_PARAMS_TIMES * OAL_SIZEOF(oal_int32));
    memset_s(puc_g_cust_nvram_info,
             WLAN_CFG_DTS_NVRAM_END * CUS_PARAMS_LEN_MAX * OAL_SIZEOF(oal_uint8),
             0,
             WLAN_CFG_DTS_NVRAM_END * CUS_PARAMS_LEN_MAX * OAL_SIZEOF(oal_uint8));

    /* ���ϵ�� */
    for (uc_idx = WLAN_CFG_DTS_NVRAM_RATIO_PA2GCCKA0; uc_idx < WLAN_CFG_DTS_NVRAM_END; uc_idx++) {
        l_ret = hwifi_read_conf_from(puc_buffer_cust_nvram_tmp, uc_idx);
        if (l_ret != INI_SUCC) {
            memset_s(puc_g_cust_nvram_info + (uc_idx * CUS_PARAMS_LEN_MAX * OAL_SIZEOF(oal_uint8)),
                     CUS_PARAMS_LEN_MAX * OAL_SIZEOF(oal_uint8), 0,
                     CUS_PARAMS_LEN_MAX * OAL_SIZEOF(oal_uint8));
            oal_io_print("hwifi_custom_host_read_dyn_cali_nvram::NVRAM get fail NV id[%d] name[%s] para[%s]!\r\n",
                         g_wifi_nvram_cfg_handler[uc_idx].ul_nv_map_idx,
                         g_wifi_nvram_cfg_handler[uc_idx].puc_nv_name,
                         g_wifi_nvram_cfg_handler[uc_idx].puc_param_name);
            continue;
        }

        puc_str = OAL_STRSTR(puc_buffer_cust_nvram_tmp, g_wifi_nvram_cfg_handler[uc_idx].puc_param_name);
        if (puc_str == OAL_PTR_NULL) {
            memset_s(puc_g_cust_nvram_info + (uc_idx * CUS_PARAMS_LEN_MAX * OAL_SIZEOF(oal_uint8)),
                     CUS_PARAMS_LEN_MAX * OAL_SIZEOF(oal_uint8), 0,
                     CUS_PARAMS_LEN_MAX * OAL_SIZEOF(oal_uint8));
            oal_io_print("hwifi_custom_host_read_dyn_cali_nvram::NVRAM get wrong val NV id[%d] name[%s] para[%s]!\r\n",
                         g_wifi_nvram_cfg_handler[uc_idx].ul_nv_map_idx,
                         g_wifi_nvram_cfg_handler[uc_idx].puc_nv_name,
                         g_wifi_nvram_cfg_handler[uc_idx].puc_param_name);
            continue;
        }

        /* ��ȡ�Ⱥź����ʵ�ʲ��� */
        puc_str += (OAL_STRLEN(g_wifi_nvram_cfg_handler[uc_idx].puc_param_name) + 1);
        pc_token = oal_strtok(puc_str, pc_end, &pc_ctx);
        if (pc_token == OAL_PTR_NULL) {
            oam_error_log1(0, OAM_SF_CUSTOM, "hwifi_custom_host_read_dyn_cali_nvram::get null value check NV id[%d]!",
                           g_wifi_nvram_cfg_handler[uc_idx].ul_nv_map_idx);
            oal_io_print("hwifi_custom_host_read_dyn_cali_nvram::get null check NV id[%d] name[%s] para[%s]!\r\n",
                         g_wifi_nvram_cfg_handler[uc_idx].ul_nv_map_idx,
                         g_wifi_nvram_cfg_handler[uc_idx].puc_nv_name,
                         g_wifi_nvram_cfg_handler[uc_idx].puc_param_name);
            memset_s(puc_g_cust_nvram_info + (uc_idx * CUS_PARAMS_LEN_MAX * OAL_SIZEOF(oal_uint8)),
                     CUS_PARAMS_LEN_MAX * OAL_SIZEOF(oal_uint8), 0,
                     CUS_PARAMS_LEN_MAX * OAL_SIZEOF(oal_uint8));
            continue;
        }

        l_ret = memcpy_s(puc_g_cust_nvram_info + (uc_idx * CUS_PARAMS_LEN_MAX * OAL_SIZEOF(oal_uint8)),
                         (WLAN_CFG_DTS_NVRAM_END - uc_idx) * CUS_PARAMS_LEN_MAX * OAL_SIZEOF(oal_uint8),
                         pc_token, OAL_STRLEN(pc_token));
        if (l_ret != EOK) {
            l_ret = INI_FAILED;
            oam_error_log1(0, OAM_SF_CUSTOM, "hwifi_custom_host_read_dyn_cali_nvram::memcpy_s fail[%d]!", l_ret);
            break;
        }

        *(puc_g_cust_nvram_info + (uc_idx * CUS_PARAMS_LEN_MAX * OAL_SIZEOF(oal_uint8)) + OAL_STRLEN(pc_token)) = *pc_end;

        /* ���ϵ����ȡ��� */
        if (uc_idx <= WLAN_CFG_DTS_NVRAM_RATIO_PA5GA1_LOW) {
            /* ���β��������Լ�� */
            pc_token = oal_strtok(pc_token, pc_sep, &pc_ctx);
            uc_param_idx = 0;
            /* ��ȡ���ƻ�ϵ�� */
            while (pc_token != OAL_PTR_NULL) {
                /* ���ַ���ת����10������ */
                *(pl_params + uc_param_idx) = (oal_int32)oal_strtol(pc_token, OAL_PTR_NULL, 10);
                pc_token = oal_strtok(OAL_PTR_NULL, pc_sep, &pc_ctx);
                uc_param_idx++;
            }
            if (uc_param_idx % DY_CALI_PARAMS_TIMES) {
                oam_error_log1(0, OAM_SF_CUSTOM, "hwifi_custom_host_read_dyn_cali_nvram::check NV id[%d]!",
                               g_wifi_nvram_cfg_handler[uc_idx].ul_nv_map_idx);
                memset_s(puc_g_cust_nvram_info + (uc_idx * CUS_PARAMS_LEN_MAX * OAL_SIZEOF(oal_uint8)),
                         CUS_PARAMS_LEN_MAX * OAL_SIZEOF(oal_uint8), 0,
                         CUS_PARAMS_LEN_MAX * OAL_SIZEOF(oal_uint8));
                continue;
            }
            uc_times_idx = uc_param_idx / DY_CALI_PARAMS_TIMES;
            /* ������ϵ����0��� */
            while (uc_times_idx--) {
                if (pl_params[(uc_times_idx)*DY_CALI_PARAMS_TIMES] == 0) {
                    oam_error_log1(0, OAM_SF_CUSTOM, "hwifi_custom_host_read_dyn_cali_nvram::check NV id[%d]!",
                                   g_wifi_nvram_cfg_handler[uc_idx].ul_nv_map_idx);
                    memset_s(puc_g_cust_nvram_info + (uc_idx * CUS_PARAMS_LEN_MAX * OAL_SIZEOF(oal_uint8)),
                             CUS_PARAMS_LEN_MAX * OAL_SIZEOF(oal_uint8), 0,
                             CUS_PARAMS_LEN_MAX * OAL_SIZEOF(oal_uint8));
                    break;
                }
            }

            tmp_en_fact_cali_completed = OAL_TRUE;
        }
    }

    g_en_fact_cali_completed = tmp_en_fact_cali_completed;

    os_mem_kfree(puc_buffer_cust_nvram_tmp);
    os_mem_kfree(pl_params);

    /* ���NVRAM�Ƿ��޸� */
    if (oal_memcmp(puc_g_cust_nvram_info, g_cust_nvram_info, OAL_SIZEOF(g_cust_nvram_info)) == 0) {
        os_mem_kfree(puc_g_cust_nvram_info);
        return INI_FILE_TIMESPEC_UNRECONFIG;
    }

    if (memcpy_s(g_cust_nvram_info, OAL_SIZEOF(g_cust_nvram_info),
                 puc_g_cust_nvram_info, OAL_SIZEOF(g_cust_nvram_info)) != EOK) {
        os_mem_kfree(puc_g_cust_nvram_info);
        return INI_FAILED;
    }

    os_mem_kfree(puc_g_cust_nvram_info);

    return INI_NVRAM_RECONFIG;
}

oal_uint8 *hwifi_get_nvram_param(oal_uint32 ul_nvram_param_idx)
{
    return g_cust_nvram_info[ul_nvram_param_idx];
}
/*
 * �� �� ��  : custom_host_read_cfg_init
 * ��������  : �״ζ�ȡ���ƻ������ļ������
 */
int32 hwifi_custom_host_read_cfg_init(void)
{
    oal_int32 l_nv_read_ret;
    oal_int32 l_ini_read_ret;
    if (get_hi110x_subchip_type() == BOARD_VERSION_HI1106) {
        return hwifi_1106_custom_host_read_cfg_init();
    }
    /* �Ȼ�ȡ˽�ж��ƻ��� */
    hwifi_config_init(CUS_TAG_PRIV_INI);

    /* ��ȡnvram�����Ƿ��޸� */
    l_nv_read_ret = hwifi_custom_host_read_dyn_cali_nvram();
    /* ��鶨�ƻ��ļ��еĲ��������Ƿ��޸� */
    l_ini_read_ret = ini_file_check_conf_update();
    if (l_ini_read_ret || l_nv_read_ret) {
        oam_warning_log0(0, OAM_SF_CFG, "hwifi_custom_host_read_cfg_init config is updated");
        hwifi_config_init(CUS_TAG_PRO_LINE_INI);
    }

    if (l_ini_read_ret == INI_FILE_TIMESPEC_UNRECONFIG) {
        oam_warning_log0(0, OAM_SF_CFG, "hwifi_custom_host_read_cfg_init file is not updated");
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    hwifi_config_init(CUS_TAG_DTS);
    l_ini_read_ret = hwifi_config_init(CUS_TAG_NV);
    if (oal_unlikely(l_ini_read_ret != OAL_SUCC)) {
        oal_io_print("hwifi_custom_host_read_cfg_init NV fail l_ret[%d].\r\n", l_ini_read_ret);
    }

    hwifi_config_init(CUS_TAG_INI);

    /* ������ɺ������ӡ */
    oal_io_print("hwifi_custom_host_read_cfg_init finish!\r\n");

    return OAL_SUCC;
}

/*
 * �� �� ��  : hwifi_hcc_customize_h2d_data_cfg
 * ��������  : Э��ջ��ʼ��ǰ���ƻ��������
 */
int32 hwifi_hcc_customize_h2d_data_cfg(void)
{
    oal_int32 l_ret;

    if (get_hi110x_subchip_type() == BOARD_VERSION_HI1106) {
        return hwifi_1106_hcc_customize_h2d_data_cfg();
    }
    /* wifi�ϵ�ʱ�ض����ƻ����� */
    l_ret = hwifi_custom_host_read_cfg_init();
    if (l_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_CFG, "hwifi_hcc_customize_h2d_data_cfg data ret[%d]", l_ret);
    }

    // ������ɹ�������ʧ��
    l_ret = hwifi_hcc_custom_ini_data_buf(CUSTOM_CFGID_PRIV_INI_ID);
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_error_log1(0, OAM_SF_CFG, "hwifi_hcc_customize_h2d_data_cfg priv data fail, ret[%d]", l_ret);
        return INI_FAILED;
    }

    l_ret = hwifi_hcc_custom_ini_data_buf(CUSTOM_CFGID_INI_ID);
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_error_log1(0, OAM_SF_CFG, "hwifi_hcc_customize_h2d_data_cfg ini data fail, ret[%d]", l_ret);
        return INI_FAILED;
    }
    return INI_SUCC;
}

/*
 * �� �� ��  : hwifi_config_fcc_ce_5g_high_band_txpwr_nvram
 * ��������  : FCC/CE 5G��band��֤
 */
OAL_STATIC oal_void hwifi_config_fcc_ce_5g_high_band_txpwr_nvram(regdomain_enum regdomain_type)
{
    oal_uint8 uc_5g_max_pwr_for_high_band;
    oal_int32 l_val = g_nvram_init_params[NVRAM_PARAMS_5G_FCC_CE_HIGH_BAND_MAX_PWR];
    wlan_cust_nvram_params *pst_g_cust_nv_params = hwifi_get_nvram_params();

    /* FCC/CE 5G ��band������书�� */
    if (get_cust_conf_int32(INI_MODU_WIFI,
                            g_nvram_config_ini[NVRAM_PARAMS_5G_FCC_CE_HIGH_BAND_MAX_PWR].name,
                            &l_val) != INI_SUCC) {
        /* ��ȡʧ��ʱ,ʹ�ó�ʼֵ */
        l_val = g_nvram_init_params[NVRAM_PARAMS_5G_FCC_CE_HIGH_BAND_MAX_PWR];
    }

    uc_5g_max_pwr_for_high_band = (oal_uint8)((regdomain_type == REGDOMAIN_ETSI) ?
                                              cus_get_low_16bits(l_val) : cus_get_high_16bits(l_val));
    /* ������Ч�Լ�� */
    if (cus_val_invalid(uc_5g_max_pwr_for_high_band, CUS_MAX_BASE_TXPOWER_VAL, CUS_MIN_BASE_TXPOWER_VAL)) {
        oam_warning_log1(0, OAM_SF_CFG, "hwifi_config_init_nvram read 5g_max_pow_high_band[%d] failed!", l_val);
        uc_5g_max_pwr_for_high_band = CUS_MAX_BASE_TXPOWER_VAL;
    }

    pst_g_cust_nv_params->uc_5g_max_pwr_fcc_ce_for_high_band = uc_5g_max_pwr_for_high_band;
}

/*
 * �� �� ��  : hwifi_config_init_iq_lpf_nvram_param
 * ��������  : iqУ׼lpf��λ
 */
OAL_STATIC oal_void hwifi_config_init_iq_lpf_nvram_param(oal_void)
{
    oal_int32 l_nvram_params = 0;
    oal_int32 l_cfg_id = NVRAM_PARAMS_INDEX_IQ_LPF_LVL;
    oal_int32 l_ret;
    wlan_cust_nvram_params *pst_g_cust_nv_params = hwifi_get_nvram_params();

    l_ret = get_cust_conf_int32(INI_MODU_WIFI, g_nvram_config_ini[l_cfg_id].name, &l_nvram_params);
    if (l_ret != INI_SUCC) {
        oam_warning_log1(0, OAM_SF_CFG, "hwifi_config_init_iq_lpf_nvram_param read id[%d] from ini failed!", l_cfg_id);

        /* ��ȡʧ��ʱ,ʹ�ó�ʼֵ */
        l_nvram_params = g_nvram_init_params[l_cfg_id];
    }

    /*  5g iq cali lvl  */
    l_ret += memcpy_s(pst_g_cust_nv_params->auc_5g_iq_cali_lpf_params,
                      NUM_OF_NV_5G_LPF_LVL, &l_nvram_params, NUM_OF_NV_5G_LPF_LVL);
}
/*
 * �� �� ��  : hwifi_config_init_fcc_ce_txpwr_nvram
 * ��������  : FCC/CE��֤
 */
OAL_STATIC int32 hwifi_config_init_fcc_ce_txpwr_nvram(oal_uint8 uc_chn_idx)
{
    int32 l_ret = INI_FAILED;
    uint8 uc_cfg_id;
    uint8 uc_param_idx = 0;
    int32 *pl_nvram_params = OAL_PTR_NULL;
    int32 *pl_fcc_ce_txpwr_limit_params = OAL_PTR_NULL;
    regdomain_enum regdomain_type;
    oal_uint8 uc_param_len;
    oal_uint8 uc_start_idx = 0;
    oal_uint8 uc_end_idx = 0;
    wlan_cust_nvram_params *pst_g_cust_nv_params = hwifi_get_nvram_params();  /* ����͹��ʶ��ƻ����� */
    wlan_cust_cfg_custom_fcc_ce_txpwr_limit_stru *pst_fcc_ce_param;

    /* ��ȡ��������Ϣ */
    regdomain_type = hwifi_get_regdomain_from_country_code((oal_uint8 *)g_wifi_country_code);
    /* ���ݹ�������Ϣѡ���·�FCC����CE���� */
    hwifi_config_fcc_ce_5g_high_band_txpwr_nvram(regdomain_type);

    if (uc_chn_idx == WLAN_RF_CHANNEL_ZERO) {
        if (regdomain_type == REGDOMAIN_ETSI) {
            uc_start_idx = NVRAM_PARAMS_CE_START_INDEX;
            uc_end_idx = NVRAM_PARAMS_CE_END_INDEX_BUTT;
        } else {
            uc_start_idx = NVRAM_PARAMS_FCC_START_INDEX;
            uc_end_idx = NVRAM_PARAMS_FCC_END_INDEX_BUTT;
        }
    } else {
        if (regdomain_type == REGDOMAIN_ETSI) {
            uc_start_idx = NVRAM_PARAMS_CE_C1_START_INDEX;
            uc_end_idx = NVRAM_PARAMS_CE_C1_END_INDEX_BUTT;
        } else {
            uc_start_idx = NVRAM_PARAMS_FCC_C1_START_INDEX;
            uc_end_idx = NVRAM_PARAMS_FCC_C1_END_INDEX_BUTT;
        }
    }

    uc_param_len = (uc_end_idx - uc_start_idx) * OAL_SIZEOF(int32);
    pl_fcc_ce_txpwr_limit_params = (int32 *)os_kzalloc_gfp(uc_param_len);
    if (pl_fcc_ce_txpwr_limit_params == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_CUSTOM, "hwifi_config_init_fcc_txpwr_nvram::pl_nvram_params mem alloc fail!");
        return INI_FAILED;
    }
    memset_s(pl_fcc_ce_txpwr_limit_params, uc_param_len, 0, uc_param_len);
    pl_nvram_params = pl_fcc_ce_txpwr_limit_params;

    for (uc_cfg_id = uc_start_idx; uc_cfg_id < uc_end_idx; uc_cfg_id++) {
        l_ret = get_cust_conf_int32(INI_MODU_WIFI, g_nvram_config_ini[uc_cfg_id].name, pl_nvram_params + uc_param_idx);
        oal_io_print("{hwifi_config_init_fcc_txpwr_nvram params[%d]=0x%x!\r\n}",
                     uc_param_idx, pl_nvram_params[uc_param_idx]);

        if (l_ret != INI_SUCC) {
            oam_warning_log1(0, OAM_SF_CFG, "hwifi_config_init_nvram read id[%d] from ini failed!", uc_cfg_id);
            /* ��ȡʧ��ʱ,ʹ�ó�ʼֵ */
            pl_nvram_params[uc_param_idx] = g_nvram_init_params[uc_cfg_id];
        }
        uc_param_idx++;
    }

    pst_fcc_ce_param = &pst_g_cust_nv_params->ast_fcc_ce_param[uc_chn_idx];
    /* 5g */
    l_ret = memcpy_s(pst_fcc_ce_param->auc_5g_fcc_txpwr_limit_params_20M,
                     OAL_SIZEOF(pst_fcc_ce_param->auc_5g_fcc_txpwr_limit_params_20M),
                     pl_nvram_params, OAL_SIZEOF(int32));
    pl_nvram_params++;
    /* ƫ���Ѿ����������ݵ�4�ֽڣ�ʣ�೤����buff�ܳ��ȼ�ȥ�ѿ��������ݵ�6-4=2�ֽ� */
    l_ret += memcpy_s(pst_fcc_ce_param->auc_5g_fcc_txpwr_limit_params_20M + OAL_SIZEOF(int32),
                      (OAL_SIZEOF(pst_fcc_ce_param->auc_5g_fcc_txpwr_limit_params_20M) - OAL_SIZEOF(int32)),
                      pl_nvram_params, 2 * OAL_SIZEOF(oal_uint8));
    pl_nvram_params++;
    l_ret += memcpy_s(pst_fcc_ce_param->auc_5g_fcc_txpwr_limit_params_40M,
                      OAL_SIZEOF(pst_fcc_ce_param->auc_5g_fcc_txpwr_limit_params_40M),
                      pl_nvram_params, OAL_SIZEOF(int32));
    pl_nvram_params++;
    /* ƫ���Ѿ����������ݵ�4�ֽڣ�ʣ�೤����buff�ܳ��ȼ�ȥ�ѿ��������ݵ�6-4=2�ֽ� */
    l_ret += memcpy_s(pst_fcc_ce_param->auc_5g_fcc_txpwr_limit_params_40M + OAL_SIZEOF(int32),
                      (OAL_SIZEOF(pst_fcc_ce_param->auc_5g_fcc_txpwr_limit_params_40M) - OAL_SIZEOF(int32)),
                      pl_nvram_params, 2 * OAL_SIZEOF(oal_uint8));
    pl_nvram_params++;
    l_ret += memcpy_s(pst_fcc_ce_param->auc_5g_fcc_txpwr_limit_params_80M,
                      OAL_SIZEOF(pst_fcc_ce_param->auc_5g_fcc_txpwr_limit_params_80M),
                      pl_nvram_params, OAL_SIZEOF(int32));
    pl_nvram_params++;
    l_ret += memcpy_s(pst_fcc_ce_param->auc_5g_fcc_txpwr_limit_params_80M + OAL_SIZEOF(int32),
                      (OAL_SIZEOF(pst_fcc_ce_param->auc_5g_fcc_txpwr_limit_params_80M) - OAL_SIZEOF(int32)),
                      pl_nvram_params, OAL_SIZEOF(oal_uint8));
    pl_nvram_params++;
    l_ret += memcpy_s(pst_fcc_ce_param->auc_5g_fcc_txpwr_limit_params_160M,
                      OAL_SIZEOF(pst_fcc_ce_param->auc_5g_fcc_txpwr_limit_params_160M), pl_nvram_params,
                      CUS_NUM_5G_160M_SIDE_BAND * OAL_SIZEOF(oal_uint8));
    if (l_ret != EOK) {
        oam_error_log1(0, OAM_SF_CUSTOM, "hwifi_config_init_fcc_txpwr_nvram::memcpy_s fail[%d]!", l_ret);
        os_mem_kfree(pl_fcc_ce_txpwr_limit_params);
        return INI_FAILED;
    }

    /* 2.4g */
    for (uc_cfg_id = 0; uc_cfg_id < MAC_2G_CHANNEL_NUM; uc_cfg_id++) {
        pl_nvram_params++;
        l_ret = memcpy_s(pst_fcc_ce_param->auc_2g_fcc_txpwr_limit_params[uc_cfg_id],
                         CUS_NUM_FCC_CE_2G_PRO * OAL_SIZEOF(oal_uint8),
                         pl_nvram_params, CUS_NUM_FCC_CE_2G_PRO * OAL_SIZEOF(oal_uint8));
        if (l_ret != EOK) {
            oam_error_log1(0, OAM_SF_CUSTOM, "hwifi_config_init_fcc_txpwr_nvram::memcpy_s fail[%d]!", l_ret);
            os_mem_kfree(pl_fcc_ce_txpwr_limit_params);
            return INI_FAILED;
        }
    }

    os_mem_kfree(pl_fcc_ce_txpwr_limit_params);
    return INI_SUCC;
}

/*
 * �� �� ��  : hwifi_config_check_sar_ctrl_nvram
 * ��������  : ��SAR�������
 */
OAL_STATIC oal_void hwifi_config_check_sar_ctrl_nvram(oal_uint8 *puc_nvram_params, oal_uint8 uc_cfg_id,
                                                      oal_uint8 uc_band_id, oal_uint8 uc_chn_idx)
{
    oal_uint8 uc_sar_lvl_idx;
    wlan_init_cust_nvram_params *pst_g_cust_nv_params = hwifi_get_init_nvram_params();

    for (uc_sar_lvl_idx = 0; uc_sar_lvl_idx < CUS_NUM_OF_SAR_ONE_PARAM_NUM; uc_sar_lvl_idx++) {
        /* �������� */
        if (puc_nvram_params[uc_sar_lvl_idx] <= CUS_MIN_OF_SAR_VAL) {
            oam_error_log4(0, OAM_SF_CUSTOM,
                           "hwifi_config_check_sar_ctrl_nvram::uc_cfg_id[%d]uc_band_id[%d] val[%d] abnormal check ini file for chn[%d]!",
                           uc_cfg_id, uc_band_id, puc_nvram_params[uc_sar_lvl_idx], uc_chn_idx);
            puc_nvram_params[uc_sar_lvl_idx] = 0xFF;
        }

        if (uc_chn_idx == WLAN_RF_CHANNEL_ZERO) {
            pst_g_cust_nv_params->st_sar_ctrl_params[uc_sar_lvl_idx + uc_cfg_id *
                CUS_NUM_OF_SAR_ONE_PARAM_NUM][uc_band_id].auc_sar_ctrl_params_c0 = puc_nvram_params[uc_sar_lvl_idx];
        } else {
            pst_g_cust_nv_params->st_sar_ctrl_params[uc_sar_lvl_idx + uc_cfg_id *
                CUS_NUM_OF_SAR_ONE_PARAM_NUM][uc_band_id].auc_sar_ctrl_params_c1 = puc_nvram_params[uc_sar_lvl_idx];
        }
    }
}

/*
 * �� �� ��  : hwifi_config_init_sar_ctrl_nvram
 * ��������  : ��SAR
 */
OAL_STATIC oal_int32 hwifi_config_init_sar_ctrl_nvram(oal_uint8 uc_chn_idx)
{
    oal_int32  l_ret = INI_FAILED;
    oal_uint8  uc_cfg_id;
    oal_uint8  uc_band_id;
    oal_uint8  uc_cus_id = uc_chn_idx == WLAN_RF_CHANNEL_ZERO ? NVRAM_PARAMS_SAR_START_INDEX :
                                                                NVRAM_PARAMS_SAR_C1_START_INDEX;
    oal_uint32 ul_nvram_params = 0;
    oal_uint8  auc_nvram_params[CUS_NUM_OF_SAR_ONE_PARAM_NUM];

    for (uc_cfg_id = 0; uc_cfg_id < CUS_NUM_OF_SAR_PER_BAND_PAR_NUM; uc_cfg_id++) {
        for (uc_band_id = 0; uc_band_id < CUS_NUM_OF_SAR_PARAMS; uc_band_id++) {
            l_ret = get_cust_conf_int32(INI_MODU_WIFI, g_nvram_config_ini[uc_cus_id].name, &ul_nvram_params);
            if (l_ret != INI_SUCC) {
                oam_warning_log1(0, OAM_SF_CUSTOM, "hwifi_config_init_sar_ctrl_nvram read id[%d] from ini failed!",
                                 uc_cus_id);
                /* ��ȡʧ��ʱ,ʹ�ó�ʼֵ */
                ul_nvram_params = 0xFFFFFFFF;
            }
            oal_io_print("{hwifi_config_init_sar_ctrl_nvram::chn[%d] params %s 0x%x!\r\n}",
                         uc_chn_idx, g_nvram_config_ini[uc_cus_id].name, ul_nvram_params);
            if (memcpy_s(auc_nvram_params, OAL_SIZEOF(auc_nvram_params),
                         &ul_nvram_params, OAL_SIZEOF(ul_nvram_params)) != EOK) {
                oam_error_log3(0, OAM_SF_CUSTOM,
                               "hwifi_config_init_sar_ctrl_nvram::uc_cfg_id[%d]band_id[%d]param[%d] set failed!",
                               uc_cfg_id, uc_band_id, ul_nvram_params);
                return INI_FAILED;
            }

            /* �������� */
            hwifi_config_check_sar_ctrl_nvram(auc_nvram_params, uc_cfg_id, uc_band_id, uc_chn_idx);
            uc_cus_id++;
        }
    }
    return INI_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_TAS_ANT_SWITCH
/*
 * �� �� ��  : hwifi_config_init_tas_ctrl_nvram
 * ��������  : tas���书��
 */
OAL_STATIC oal_int32 hwifi_config_init_tas_ctrl_nvram(oal_void)
{
    oal_int32 l_ret;
    oal_uint8 uc_band_idx;
    oal_uint8 uc_rf_idx;
    oal_uint32 ul_nvram_params = 0;
    oal_int8 ac_tas_ctrl_params[WLAN_BAND_BUTT][WLAN_RF_CHANNEL_NUMS] = {{0}};
    wlan_cust_nvram_params *pst_g_cust_nv_params = hwifi_get_nvram_params();

    l_ret = get_cust_conf_int32(INI_MODU_WIFI, g_nvram_config_ini[NVRAM_PARAMS_TAS_ANT_SWITCH_EN].name,
                                &ul_nvram_params);
    if (l_ret == INI_SUCC) {
        oam_warning_log1(0, OAM_SF_CUSTOM, "hwifi_config_init_tas_ctrl_nvram g_aen_tas_switch_en[%d]!",
                         ul_nvram_params);
        g_tas_switch_en[WLAN_RF_CHANNEL_ZERO] = (oal_bool_enum_uint8)cus_get_low_16bits(ul_nvram_params);
        g_tas_switch_en[WLAN_RF_CHANNEL_ONE] = (oal_bool_enum_uint8)cus_get_high_16bits(ul_nvram_params);
    }

    l_ret = get_cust_conf_int32(INI_MODU_WIFI, g_nvram_config_ini[NVRAM_PARAMS_TAS_PWR_CTRL].name, &ul_nvram_params);
    oal_io_print("{hwifi_config_init_tas_ctrl_nvram params[%d]=0x%x!\r\n}", NVRAM_PARAMS_TAS_PWR_CTRL, ul_nvram_params);
    if (l_ret != INI_SUCC) {
        oam_warning_log1(0, OAM_SF_CUSTOM, "hwifi_config_init_tas_ctrl_nvram read id[%d] from ini failed!",
                         NVRAM_PARAMS_TAS_PWR_CTRL);
        /* ��ȡʧ��ʱ,ʹ�ó�ʼֵ */
        ul_nvram_params = g_nvram_init_params[NVRAM_PARAMS_TAS_PWR_CTRL];
    }

    if (memcpy_s(ac_tas_ctrl_params, OAL_SIZEOF(ac_tas_ctrl_params),
                 &ul_nvram_params, OAL_SIZEOF(ac_tas_ctrl_params)) != EOK) {
        return INI_FAILED;
    }

    for (uc_band_idx = 0; uc_band_idx < WLAN_BAND_BUTT; uc_band_idx++) {
        for (uc_rf_idx = 0; uc_rf_idx < WLAN_RF_CHANNEL_NUMS; uc_rf_idx++) {
            if ((ac_tas_ctrl_params[uc_band_idx][uc_rf_idx] > CUS_MAX_OF_TAS_PWR_CTRL_VAL) ||
                (ac_tas_ctrl_params[uc_rf_idx][uc_rf_idx] < 0)) {
                oam_error_log4(0, OAM_SF_CUSTOM,
                               "hwifi_config_init_tas_ctrl_nvram::band[%d] rf[%d] ul_nvram_params[%d],val[%d] out of the normal check ini file!",
                               uc_band_idx, uc_rf_idx, ul_nvram_params, ac_tas_ctrl_params[uc_rf_idx][uc_rf_idx]);
                ac_tas_ctrl_params[uc_band_idx][uc_rf_idx] = 0;
            }
            pst_g_cust_nv_params->auc_tas_ctrl_params[uc_rf_idx][uc_band_idx] =
                (oal_uint8)ac_tas_ctrl_params[uc_band_idx][uc_rf_idx];
        }
    }

    return INI_SUCC;
}
#endif

/*
 * �� �� ��  : hwifi_config_sepa_coefficient_from_param
 * ��������  : ���ַ����зָ����ϵ����
 */
OAL_STATIC oal_uint32 hwifi_config_sepa_coefficient_from_param(oal_uint8 *puc_cust_param_info, oal_int32 *pl_coe_params,
                                                               oal_uint16 *pus_param_num, oal_uint16 us_max_idx)
{
    oal_int8 *pc_token;
    oal_int8 *pc_ctx;
    oal_int8 *pc_end = ";";
    oal_int8 *pc_sep = ",";
    oal_uint16 us_param_num = 0;
    oal_uint8 auc_cust_param[CUS_PARAMS_LEN_MAX];

    if (memcpy_s(auc_cust_param, CUS_PARAMS_LEN_MAX, puc_cust_param_info, OAL_STRLEN(puc_cust_param_info)) != EOK) {
        return OAL_FAIL;
    }

    pc_token = oal_strtok(auc_cust_param, pc_end, &pc_ctx);
    if (pc_token == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_CUSTOM, "hwifi_config_sepa_coefficient_from_param read get null value check!");
        return OAL_ERR_CODE_PTR_NULL;
    }
    pc_token = oal_strtok(pc_token, pc_sep, &pc_ctx);
    /* ��ȡ���ƻ�ϵ�� */
    while (pc_token) {
        if (us_param_num == us_max_idx) {
            oam_error_log2(0, OAM_SF_CUSTOM,
                           "hwifi_config_sepa_coefficient_from_param::nv or ini param is too many idx[%d] Max[%d]",
                           us_param_num, us_max_idx);
            return OAL_FAIL;
        }
        /* ���ַ���ת����10������ */
        *(pl_coe_params + us_param_num) = (oal_int32)oal_strtol(pc_token, OAL_PTR_NULL, 10);
        pc_token = oal_strtok(OAL_PTR_NULL, pc_sep, &pc_ctx);
        us_param_num++;
    }

    *pus_param_num = us_param_num;
    return OAL_SUCC;
}

/*
 * �� �� ��  : hwifi_get_max_txpwr_base
 * ��������  : ��ȡ���ƻ���׼���书��
 */
oal_void hwifi_get_max_txpwr_base(oal_int32 l_plat_tag, oal_uint8 uc_nvram_base_param_idx,
                                  oal_uint8 *puc_txpwr_base_params, oal_uint8 uc_param_num)
{
    oal_uint8 uc_param_idx;
    oal_int32 l_ret = INI_FAILED;
    oal_uint8 *puc_base_pwr_params = OAL_PTR_NULL;
    oal_uint16 us_per_param_num = 0;
    oal_int32 l_nv_params[DY_CALI_NUM_5G_BAND] = {0};

    puc_base_pwr_params = (oal_uint8 *)os_kzalloc_gfp(CUS_PARAMS_LEN_MAX * OAL_SIZEOF(oal_uint8));
    if (puc_base_pwr_params == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_CUSTOM, "hwifi_get_max_txpwr_base::puc_base_pwr_params mem alloc fail!");
        l_ret = INI_FAILED;
    } else {
        memset_s(puc_base_pwr_params,
                 CUS_PARAMS_LEN_MAX * OAL_SIZEOF(oal_uint8),
                 0,
                 CUS_PARAMS_LEN_MAX * OAL_SIZEOF(oal_uint8));
        l_ret = get_cust_conf_string(l_plat_tag, g_nvram_config_ini[uc_nvram_base_param_idx].name,
                                     puc_base_pwr_params, CUS_PARAMS_LEN_MAX - 1);
        if (l_ret == INI_SUCC) {
            if ((hwifi_config_sepa_coefficient_from_param(puc_base_pwr_params, l_nv_params,
                                                          &us_per_param_num, uc_param_num) == OAL_SUCC) &&
                (us_per_param_num == uc_param_num)) {
                /* ���������Լ�� */
                for (uc_param_idx = 0; uc_param_idx < uc_param_num; uc_param_idx++) {
                    if (cus_val_invalid(l_nv_params[uc_param_idx],
                        CUS_MAX_BASE_TXPOWER_VAL, CUS_MIN_BASE_TXPOWER_VAL)) {
                        oam_error_log3(0, OAM_SF_CUSTOM,
                            "hwifi_get_max_txpwr_base read %dth from ini val[%d] out of range replaced by [%d]!",
                            uc_nvram_base_param_idx, l_nv_params[uc_param_idx], CUS_MAX_BASE_TXPOWER_VAL);
                        *(puc_txpwr_base_params + uc_param_idx) = CUS_MAX_BASE_TXPOWER_VAL;
                    } else {
                        *(puc_txpwr_base_params + uc_param_idx) = (oal_uint8)l_nv_params[uc_param_idx];
                    }
                }
                os_mem_kfree(puc_base_pwr_params);
                return;
            }
        } else {
            oam_error_log3(0, OAM_SF_CUSTOM,
                           "hwifi_get_max_txpwr_base::l_plat_tag[%d] read %dth failed ret[%d] check ini files!",
                           l_plat_tag, uc_nvram_base_param_idx, l_ret);
        }
    }

    if (l_ret != INI_SUCC) {
        /* ʧ��Ĭ��ʹ�ó�ʼֵ */
        for (uc_param_idx = 0; uc_param_idx < uc_param_num; uc_param_idx++) {
            *(puc_txpwr_base_params + uc_param_idx) = CUS_MAX_BASE_TXPOWER_VAL;
        }

        oam_error_log2(0, OAM_SF_CFG, "hwifi_get_max_txpwr_base read failed ret[%d] replaced by ini_val[%d]!",
                       l_ret, CUS_MAX_BASE_TXPOWER_VAL);
    }
    os_mem_kfree(puc_base_pwr_params);
    return;
}

/*
 * �� �� ��  : hwifi_get_max_txpwr_base_delt_val
 * ��������  : ��ȡmimo���ƻ���׼���书�ʲ�
 */
static oal_void hwifi_get_max_txpwr_base_delt_val(oal_uint8 uc_cus_id,
                                                  oal_int8 *pc_txpwr_base_delt_params,
                                                  oal_uint8 uc_param_num)
{
    oal_uint8  uc_param_idx;
    oal_int32  ul_nvram_params  = 0;

    memset_s(pc_txpwr_base_delt_params, uc_param_num, 0, uc_param_num);
    if (INI_SUCC == get_cust_conf_int32(INI_MODU_WIFI, g_nvram_config_ini[uc_cus_id].name, &ul_nvram_params)) {
        if (memcpy_s(pc_txpwr_base_delt_params, uc_param_num, &ul_nvram_params, uc_param_num) != EOK) {
            oam_error_log0(0, OAM_SF_CUSTOM, "hwifi_get_max_txpwr_base_delt_val::copy failed!");
            return;
        }

        /* ���������Լ�� */
        for (uc_param_idx = 0; uc_param_idx < uc_param_num; uc_param_idx++) {
            if (cus_val_invalid(pc_txpwr_base_delt_params[uc_param_idx], 0, CUS_MAX_BASE_TXPOWER_DELT_VAL_MIN)) {
                        oam_error_log1(0, OAM_SF_CUSTOM,
                                       "hwifi_get_max_txpwr_base_delt_val::ini val[%d] out of range replaced by zero!",
                                       pc_txpwr_base_delt_params[uc_param_idx]);
                        pc_txpwr_base_delt_params[uc_param_idx] = 0;
            }
        }
    }

    return;
}


static oal_void hwifi_get_cfg_delt_ru_pow_params(void)
{
    int32 l_cfg_idx_one = 0;
    int32 l_cfg_idx_two = 0;
    wlan_cust_nvram_params *pst_g_cust_nv_params = hwifi_get_nvram_params();

    for (l_cfg_idx_one = 0; l_cfg_idx_one < WLAN_BW_CAP_BUTT; l_cfg_idx_one++) {
        for (l_cfg_idx_two = 0; l_cfg_idx_two < WLAN_HE_RU_SIZE_BUTT; l_cfg_idx_two++) {
            oal_io_print("%s[%d][%d]:%d \n", "5g_cfg_tpc_ru_pow", l_cfg_idx_one, l_cfg_idx_two,
                pst_g_cust_nv_params->ac_fullbandwidth_to_ru_power_5g[l_cfg_idx_one][l_cfg_idx_two]);
        }
    }
    for (l_cfg_idx_one = 0; l_cfg_idx_one < WLAN_BW_CAP_80M; l_cfg_idx_one++) {
        for (l_cfg_idx_two = 0; l_cfg_idx_two < WLAN_HE_RU_SIZE_996; l_cfg_idx_two++) {
            oal_io_print("%s[%d][%d]:%d \n", "2g_cfg_tpc_ru_pow", l_cfg_idx_one, l_cfg_idx_two,
                pst_g_cust_nv_params->ac_fullbandwidth_to_ru_power_2g[l_cfg_idx_one][l_cfg_idx_two]);
        }
    }
}


static oal_void hwifi_get_delt_ru_pow_val_5g(void)
{
    oal_int32 l_nvram_params = 0;
    oal_int32 l_ret;
    int32 l_cfg_idx_one;
    int32 l_cfg_idx_two;
    wlan_cust_nvram_params *pst_g_cust_nv_params = hwifi_get_nvram_params();

    /* 20m 26~242tone ru_power */
    if (get_cust_conf_int32(INI_MODU_WIFI, g_nvram_config_ini[NVRAM_CFG_TPC_RU_POWER_20M_5G].name, &l_nvram_params) != INI_SUCC) {
        l_nvram_params = g_nvram_init_params[NVRAM_CFG_TPC_RU_POWER_20M_5G];
    }
    l_ret = memcpy_s(pst_g_cust_nv_params->ac_fullbandwidth_to_ru_power_5g[WLAN_BW_CAP_20M], sizeof(l_nvram_params), \
                     &l_nvram_params, sizeof(l_nvram_params));

    /* 40m 26~242tone ru_power */
    if (get_cust_conf_int32(INI_MODU_WIFI, g_nvram_config_ini[NVRAM_CFG_TPC_RU_POWER_40M_5G].name, &l_nvram_params) != INI_SUCC) {
        l_nvram_params = g_nvram_init_params[NVRAM_CFG_TPC_RU_POWER_40M_5G];
    }
    l_ret += memcpy_s(pst_g_cust_nv_params->ac_fullbandwidth_to_ru_power_5g[WLAN_BW_CAP_40M], sizeof(l_nvram_params), \
                      &l_nvram_params, sizeof(l_nvram_params));

    /* 40m 484tone/80m 26~106tone ru_power */
    if (get_cust_conf_int32(INI_MODU_WIFI, g_nvram_config_ini[NVRAM_CFG_TPC_RU_POWER_80M_L_5G].name, &l_nvram_params) != INI_SUCC) {
        l_nvram_params = g_nvram_init_params[NVRAM_CFG_TPC_RU_POWER_80M_L_5G];
    }
    pst_g_cust_nv_params->ac_fullbandwidth_to_ru_power_5g[WLAN_BW_CAP_40M][WLAN_HE_RU_SIZE_484] = cus_get_first_byte(l_nvram_params);
    pst_g_cust_nv_params->ac_fullbandwidth_to_ru_power_5g[WLAN_BW_CAP_80M][WLAN_HE_RU_SIZE_26] = cus_get_second_byte(l_nvram_params);
    pst_g_cust_nv_params->ac_fullbandwidth_to_ru_power_5g[WLAN_BW_CAP_80M][WLAN_HE_RU_SIZE_52] = cus_get_third_byte(l_nvram_params);
    pst_g_cust_nv_params->ac_fullbandwidth_to_ru_power_5g[WLAN_BW_CAP_80M][WLAN_HE_RU_SIZE_106] = cus_get_fourth_byte(l_nvram_params);

    /* 80m 242~996tone/160m 26tone ru_power */
    if (get_cust_conf_int32(INI_MODU_WIFI, g_nvram_config_ini[NVRAM_CFG_TPC_RU_POWER_80M_H_5G].name, &l_nvram_params) != INI_SUCC) {
        l_nvram_params = g_nvram_init_params[NVRAM_CFG_TPC_RU_POWER_80M_H_5G];
    }
    pst_g_cust_nv_params->ac_fullbandwidth_to_ru_power_5g[WLAN_BW_CAP_80M][WLAN_HE_RU_SIZE_242] = cus_get_first_byte(l_nvram_params);
    pst_g_cust_nv_params->ac_fullbandwidth_to_ru_power_5g[WLAN_BW_CAP_80M][WLAN_HE_RU_SIZE_484] = cus_get_second_byte(l_nvram_params);
    pst_g_cust_nv_params->ac_fullbandwidth_to_ru_power_5g[WLAN_BW_CAP_80M][WLAN_HE_RU_SIZE_996] = cus_get_third_byte(l_nvram_params);
    pst_g_cust_nv_params->ac_fullbandwidth_to_ru_power_5g[WLAN_BW_CAP_160M][WLAN_HE_RU_SIZE_26] = cus_get_fourth_byte(l_nvram_params);

    /* 160m 52~996tone ru_power */
    if (get_cust_conf_int32(INI_MODU_WIFI, g_nvram_config_ini[NVRAM_CFG_TPC_RU_POWER_160M_L_5G].name, &l_nvram_params) != INI_SUCC) {
        l_nvram_params = g_nvram_init_params[NVRAM_CFG_TPC_RU_POWER_160M_L_5G];
    }
    l_ret += memcpy_s(&(pst_g_cust_nv_params->ac_fullbandwidth_to_ru_power_5g[WLAN_BW_CAP_160M][WLAN_HE_RU_SIZE_52]),
                      sizeof(l_nvram_params), &l_nvram_params, sizeof(l_nvram_params));
    if (get_cust_conf_int32(INI_MODU_WIFI, g_nvram_config_ini[NVRAM_CFG_TPC_RU_POWER_160M_H_5G].name, &l_nvram_params) != INI_SUCC) {
        l_nvram_params = g_nvram_init_params[NVRAM_CFG_TPC_RU_POWER_160M_H_5G];
    }
    l_ret += memcpy_s(&(pst_g_cust_nv_params->ac_fullbandwidth_to_ru_power_5g[WLAN_BW_CAP_160M][WLAN_HE_RU_SIZE_996]),
                      sizeof(oal_uint8)*2, &l_nvram_params, sizeof(oal_uint8)*2);
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_CUSTOM, "hwifi_get_delt_ru_pow_val_5g::memcpy failed!");
        memset_s(pst_g_cust_nv_params->ac_fullbandwidth_to_ru_power_5g, sizeof(oal_int8) * WLAN_BW_CAP_BUTT * WLAN_HE_RU_SIZE_BUTT, 0, \
                 sizeof(oal_int8)*WLAN_BW_CAP_BUTT * WLAN_HE_RU_SIZE_BUTT);
        return;
    }

    /* ������Ч�Լ�� */
    for (l_cfg_idx_one = 0; l_cfg_idx_one < WLAN_BW_CAP_BUTT; l_cfg_idx_one++) {
        for (l_cfg_idx_two = 0; l_cfg_idx_two < WLAN_HE_RU_SIZE_BUTT; l_cfg_idx_two++) {
            if (cus_val_invalid(pst_g_cust_nv_params->ac_fullbandwidth_to_ru_power_5g[l_cfg_idx_one][l_cfg_idx_two],
                                CUS_RU_DELT_POW_MAX, CUS_RU_DELT_POW_MIN)) {
                oam_error_log4(0, OAM_SF_CUSTOM, "hwifi_get_delt_ru_pow_val_5g:cfg bw[%d]ru[%d] val[%d] invalid! max[%d]",
                               l_cfg_idx_one, l_cfg_idx_two, pst_g_cust_nv_params->ac_fullbandwidth_to_ru_power_5g[l_cfg_idx_one][l_cfg_idx_two],
                               CUS_RU_DELT_POW_MAX);
                pst_g_cust_nv_params->ac_fullbandwidth_to_ru_power_5g[l_cfg_idx_one][l_cfg_idx_two] = 0;
            }
        }
    }

    return;
}


static oal_void hwifi_get_delt_ru_pow_val_2g(void)
{
    oal_int32 l_nvram_params = 0;
    oal_int32 l_ret;
    int32 l_cfg_idx_one;
    int32 l_cfg_idx_two;
    wlan_cust_nvram_params *pst_g_cust_nv_params = hwifi_get_nvram_params();

    /* 20m 26~242tone ru_power */
    if (get_cust_conf_int32(INI_MODU_WIFI, g_nvram_config_ini[NVRAM_CFG_TPC_RU_POWER_20M_2G].name, &l_nvram_params) != INI_SUCC) {
        l_nvram_params = g_nvram_init_params[NVRAM_CFG_TPC_RU_POWER_20M_2G];
    }
    l_ret = memcpy_s(pst_g_cust_nv_params->ac_fullbandwidth_to_ru_power_2g[WLAN_BW_CAP_20M], sizeof(l_nvram_params),
                     &l_nvram_params, sizeof(l_nvram_params));

    /* 40m 26~242tone ru_power */
    if (get_cust_conf_int32(INI_MODU_WIFI, g_nvram_config_ini[NVRAM_CFG_TPC_RU_POWER_40M_0_2G].name, &l_nvram_params) != INI_SUCC) {
        l_nvram_params = g_nvram_init_params[NVRAM_CFG_TPC_RU_POWER_40M_0_2G];
    }
    l_ret += memcpy_s(pst_g_cust_nv_params->ac_fullbandwidth_to_ru_power_2g[WLAN_BW_CAP_40M], sizeof(l_nvram_params),
                      &l_nvram_params, sizeof(l_nvram_params));

    /* 40m 484tone ru_power */
    if (get_cust_conf_int32(INI_MODU_WIFI, g_nvram_config_ini[NVRAM_CFG_TPC_RU_POWER_40M_1_2G].name, &l_nvram_params) != INI_SUCC) {
        l_nvram_params = g_nvram_init_params[NVRAM_CFG_TPC_RU_POWER_40M_1_2G];
    }
    pst_g_cust_nv_params->ac_fullbandwidth_to_ru_power_2g[WLAN_BW_CAP_40M][WLAN_HE_RU_SIZE_484] = cus_get_first_byte(l_nvram_params);

    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_CUSTOM, "hwifi_get_delt_ru_pow_val_2g::memcpy failed!");
        memset_s(pst_g_cust_nv_params->ac_fullbandwidth_to_ru_power_5g, sizeof(oal_int8) * WLAN_BW_CAP_80M * WLAN_HE_RU_SIZE_996, \
                 0, sizeof(oal_int8) * WLAN_BW_CAP_80M * WLAN_HE_RU_SIZE_996);
        return;
    }

    /* ������Ч�Լ�� */
    for (l_cfg_idx_one = 0; l_cfg_idx_one < WLAN_BW_CAP_80M; l_cfg_idx_one++) {
        for (l_cfg_idx_two = 0; l_cfg_idx_two < WLAN_HE_RU_SIZE_996; l_cfg_idx_two++) {
            if (cus_val_invalid(pst_g_cust_nv_params->ac_fullbandwidth_to_ru_power_2g[l_cfg_idx_one][l_cfg_idx_two],
                                CUS_RU_DELT_POW_MAX, CUS_RU_DELT_POW_MIN)) {
                oam_error_log4(0, OAM_SF_CUSTOM, "hwifi_get_delt_ru_pow_val_2g:cfg bw[%d]ru[%d] val[%d] invalid! max[%d]",
                               l_cfg_idx_one, l_cfg_idx_two, pst_g_cust_nv_params->ac_fullbandwidth_to_ru_power_2g[l_cfg_idx_one][l_cfg_idx_two],
                               CUS_RU_DELT_POW_MAX);
                pst_g_cust_nv_params->ac_fullbandwidth_to_ru_power_2g[l_cfg_idx_one][l_cfg_idx_two] = 0;
            }
        }
    }

    return;
}

#if defined(_PRE_WLAN_FEATURE_11AX)


static oal_int32 hwifi_get_5g_ru_max_ru_pow_val(wlan_cust_nvram_params *g_cust_nv_params)
{
    oal_int32 nvram_params = 0;
    oal_int32 ret = 0;
    oal_uint32 idx;
    oal_uint32 band_idx;
    oal_uint32 nvram_idx;
    regdomain_enum_uint8 regdomain_type;

    /* ���ݹ�������Ϣѡ���·�FCC����CE���� */
    regdomain_type = hwifi_get_regdomain_from_country_code((oal_uint8 *)g_wifi_country_code);
    nvram_idx = (regdomain_type == REGDOMAIN_ETSI) ? NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_MIMO_B1_CE : \
                (regdomain_type == REGDOMAIN_FCC) ? NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_MIMO_B1_FCC : \
                NVRAM_CFG_TPC_RU_MAX_POWER_0_5G_MIMO_B1;

    for (band_idx = 0; band_idx < MAC_NUM_5G_BAND; band_idx++) {
        for (idx = CUS_POW_TX_CHAIN_MIMO; idx < CUS_POW_TX_CHAIN_BUTT; idx++) {
            if (get_cust_conf_int32(INI_MODU_WIFI, g_nvram_config_ini[nvram_idx].name, &nvram_params) != INI_SUCC) {
                nvram_params = 0xFFFFFFFF;
            }
            ret += memcpy_s(g_cust_nv_params->auc_tpc_tb_ru_5g_max_power[band_idx].auc_tb_ru_5g_max_power[idx],
                sizeof(nvram_params), &nvram_params, sizeof(nvram_params));
            nvram_idx++;
            if (get_cust_conf_int32(INI_MODU_WIFI, g_nvram_config_ini[nvram_idx].name, &nvram_params) != INI_SUCC) {
                nvram_params = 0xFFFFFF;
            }
            ret += memcpy_s(&g_cust_nv_params->auc_tpc_tb_ru_5g_max_power[band_idx].auc_tb_ru_5g_max_power\
                // 1992_996_484,3��ru�Ķ��ƻ���
                [idx][WLAN_HE_RU_SIZE_484], sizeof(nvram_params), &nvram_params, 3 * sizeof(oal_uint8));
            nvram_idx++;
        }
    }

    return ret;
}


static oal_void hwifi_get_total_max_ru_pow_val(wlan_cust_nvram_params *pst_g_cust_nv_params)
{
    oal_uint32 ul_idx;
    oal_uint32 ul_nvram_idx;
    oal_int32 l_nvram_params = 0;

    ul_nvram_idx = NVRAM_CFG_TPC_RU_MAX_POWER_2G;
    for (ul_idx = WLAN_BAND_2G; ul_idx < WLAN_BAND_BUTT; ul_idx++) {
        if (get_cust_conf_int32(INI_MODU_WIFI, g_nvram_config_ini[ul_nvram_idx].name, &l_nvram_params) != INI_SUCC) {
            l_nvram_params = 0xFF;
        }
        pst_g_cust_nv_params->auc_tpc_tb_ru_max_power[ul_idx] = cus_get_first_byte(l_nvram_params);
        ul_nvram_idx++;
    }
}


static oal_void hwifi_get_max_ru_pow_val(void)
{
    oal_int32 l_nvram_params = 0;
    oal_int32 l_ret = 0;
    wlan_cust_nvram_params *pst_g_cust_nv_params = hwifi_get_nvram_params();
    oal_uint32 ul_idx;
    oal_uint32 ul_nvram_idx = NVRAM_CFG_TPC_RU_MAX_POWER_0_2G_MIMO;

    for (ul_idx = CUS_POW_TX_CHAIN_MIMO; ul_idx < CUS_POW_TX_CHAIN_BUTT; ul_idx++) {
        if (get_cust_conf_int32(INI_MODU_WIFI, g_nvram_config_ini[ul_nvram_idx].name, &l_nvram_params) != INI_SUCC) {
            l_nvram_params = 0xFFFFFFFF;
        }
        l_ret += memcpy_s(pst_g_cust_nv_params->auc_tpc_tb_ru_2g_max_power[ul_idx], sizeof(l_nvram_params), \
                          &l_nvram_params, sizeof(l_nvram_params));
        ul_nvram_idx++;
        if (get_cust_conf_int32(INI_MODU_WIFI, g_nvram_config_ini[ul_nvram_idx].name, &l_nvram_params) != INI_SUCC) {
            l_nvram_params = 0xFF;
        }
        pst_g_cust_nv_params->auc_tpc_tb_ru_2g_max_power[ul_idx][WLAN_HE_RU_SIZE_484] = cus_get_first_byte(l_nvram_params);
        ul_nvram_idx++;
    }
    hwifi_get_5g_ru_max_ru_pow_val(pst_g_cust_nv_params);
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_CUSTOM, "hwifi_get_max_ru_pow_val::memcpy failed!");
        memset_s(pst_g_cust_nv_params->auc_tpc_tb_ru_2g_max_power, sizeof(pst_g_cust_nv_params->auc_tpc_tb_ru_2g_max_power),
                 0xFF, sizeof(pst_g_cust_nv_params->auc_tpc_tb_ru_2g_max_power));
        memset_s(pst_g_cust_nv_params->auc_tpc_tb_ru_5g_max_power, sizeof(pst_g_cust_nv_params->auc_tpc_tb_ru_5g_max_power),
                 0xFF, sizeof(pst_g_cust_nv_params->auc_tpc_tb_ru_5g_max_power));
    }

    hwifi_get_total_max_ru_pow_val(pst_g_cust_nv_params);
}
#endif


OAL_STATIC OAL_INLINE oal_void hwifi_config_init_ru_pow_nvram(void)
{
    hwifi_get_delt_ru_pow_val_5g();
    hwifi_get_delt_ru_pow_val_2g();

#if defined(_PRE_WLAN_FEATURE_11AX)
    hwifi_get_max_ru_pow_val();
#endif
}

/*
 * �� �� ��  : hwifi_config_init_nvram
 * ��������  : handle nvram customize params
 */
OAL_STATIC int32 hwifi_config_init_nvram(void)
{
    int32 l_ret = INI_FAILED;
    int32 l_cfg_id;
    int32 al_nvram_params[NVRAM_PARAMS_TXPWR_INDEX_BUTT] = {0};
    int32 l_val;
    wlan_cust_nvram_params *pst_g_cust_nv_params = hwifi_get_nvram_params();

    memset_s(&g_cust_nv_params, OAL_SIZEOF(g_cust_nv_params), 0, OAL_SIZEOF(g_cust_nv_params));

    /* read nvm failed or data not exist or country_code updated, read ini:cust_spec > cust_common > default */
    /* find plat tag */
    for (l_cfg_id = NVRAM_PARAMS_INDEX_0; l_cfg_id < NVRAM_PARAMS_TXPWR_INDEX_BUTT; l_cfg_id++) {
        l_ret = get_cust_conf_int32(INI_MODU_WIFI, g_nvram_config_ini[l_cfg_id].name, &al_nvram_params[l_cfg_id]);
        oam_info_log2(0, OAM_SF_CFG, "{hwifi_config_init_nvram aul_nvram_params[%d]=0x%x!}",
                      l_cfg_id, al_nvram_params[l_cfg_id]);

        if (l_ret != INI_SUCC) {
            oam_warning_log1(0, OAM_SF_CFG, "hwifi_config_init_nvram read id[%d] from ini failed!", l_cfg_id);

            /* ��ȡʧ��ʱ,ʹ�ó�ʼֵ */
            al_nvram_params[l_cfg_id] = g_nvram_init_params[l_cfg_id];
        }
    }

    l_ret = memcpy_s(pst_g_cust_nv_params->ac_delt_txpwr_params,
                     NUM_OF_NV_MAX_TXPOWER,
                     al_nvram_params,
                     NUM_OF_NV_MAX_TXPOWER);
    l_ret += memcpy_s(pst_g_cust_nv_params->ac_dpd_delt_txpwr_params, NUM_OF_NV_DPD_MAX_TXPOWER,
                      al_nvram_params + NVRAM_PARAMS_INDEX_DPD_0, NUM_OF_NV_DPD_MAX_TXPOWER);

    l_val = cus_get_low_16bits(*(al_nvram_params + NVRAM_PARAMS_INDEX_11B_OFDM_DELT_POW));
    l_ret += memcpy_s(pst_g_cust_nv_params->ac_11b_delt_txpwr_params, NUM_OF_NV_11B_DELTA_TXPOWER,
                      &l_val, NUM_OF_NV_11B_DELTA_TXPOWER);
    /* FEM OFF IQ CALI POW */
    l_val = cus_get_high_16bits(*(al_nvram_params + NVRAM_PARAMS_INDEX_11B_OFDM_DELT_POW));
    l_ret += memcpy_s(pst_g_cust_nv_params->auc_fem_off_iq_cal_pow_params,
                      OAL_SIZEOF(pst_g_cust_nv_params->auc_fem_off_iq_cal_pow_params),
                      &l_val, OAL_SIZEOF(pst_g_cust_nv_params->auc_fem_off_iq_cal_pow_params));

    l_ret += memcpy_s(pst_g_cust_nv_params->auc_5g_upper_upc_params, NUM_OF_NV_5G_UPPER_UPC,
                      al_nvram_params + NVRAM_PARAMS_INDEX_IQ_MAX_UPC, NUM_OF_NV_5G_UPPER_UPC);

    l_ret += memcpy_s(pst_g_cust_nv_params->ac_2g_low_pow_amend_params, NUM_OF_NV_2G_LOW_POW_DELTA_VAL,
                      al_nvram_params + NVRAM_PARAMS_INDEX_2G_LOW_POW_AMEND, NUM_OF_NV_2G_LOW_POW_DELTA_VAL);
    if (l_ret != EOK) {
        oam_error_log1(0, OAM_SF_CFG, "hwifi_config_init_nvram read from ini failed[%d]!", l_ret);
        return INI_FAILED;
    }

    for (l_cfg_id = 0; l_cfg_id < NUM_OF_NV_2G_LOW_POW_DELTA_VAL; l_cfg_id++) {
        if (cus_abs(pst_g_cust_nv_params->ac_2g_low_pow_amend_params[l_cfg_id]) > CUS_2G_LOW_POW_AMEND_ABS_VAL_MAX) {
            pst_g_cust_nv_params->ac_2g_low_pow_amend_params[l_cfg_id] = 0;
        }
    }

    /* ��׼���� */
    hwifi_get_max_txpwr_base(INI_MODU_WIFI, NVRAM_PARAMS_INDEX_19,
                             pst_g_cust_nv_params->auc_2g_txpwr_base_params[WLAN_RF_CHANNEL_ZERO], CUS_BASE_PWR_NUM_2G);
    hwifi_get_max_txpwr_base(INI_MODU_WIFI, NVRAM_PARAMS_INDEX_20,
                             pst_g_cust_nv_params->auc_5g_txpwr_base_params[WLAN_RF_CHANNEL_ZERO], CUS_BASE_PWR_NUM_5G);
    hwifi_get_max_txpwr_base(INI_MODU_WIFI, NVRAM_PARAMS_INDEX_21,
                             pst_g_cust_nv_params->auc_2g_txpwr_base_params[WLAN_RF_CHANNEL_ONE], CUS_BASE_PWR_NUM_2G);
    hwifi_get_max_txpwr_base(INI_MODU_WIFI, NVRAM_PARAMS_INDEX_22,
                             pst_g_cust_nv_params->auc_5g_txpwr_base_params[WLAN_RF_CHANNEL_ONE], CUS_BASE_PWR_NUM_5G);
    /* FCC/CE/SAR */
    for (l_cfg_id = 0; l_cfg_id < WLAN_RF_CHANNEL_NUMS; l_cfg_id++) {
        hwifi_config_init_fcc_ce_txpwr_nvram(l_cfg_id);
        hwifi_config_init_sar_ctrl_nvram(l_cfg_id);
    }
    hwifi_get_max_txpwr_base_delt_val(NVRAM_PARAMS_INDEX_DELT_BASE_POWER_23,
                                      &pst_g_cust_nv_params->ac_delt_txpwr_base_params[0][0],
                                      OAL_SIZEOF(pst_g_cust_nv_params->ac_delt_txpwr_base_params));

    /* extend */
    hwifi_config_init_iq_lpf_nvram_param();

#ifdef _PRE_WLAN_FEATURE_TAS_ANT_SWITCH
    /* TAS */
    hwifi_config_init_tas_ctrl_nvram();
#endif
    hwifi_config_init_ru_pow_nvram();

    oam_info_log0(0, OAM_SF_CFG, "hwifi_config_init_nvram read from ini success!");
    return INI_SUCC;
}

/*
 * �� �� ��  : hwifi_config_init_private_custom
 * ��������  : ��ʼ��˽�ж���ȫ�ֱ�������
 */
OAL_STATIC int32 hwifi_config_init_private_custom(void)
{
    int32 l_cfg_id;
    int32 l_ret = INI_FAILED;

    for (l_cfg_id = 0; l_cfg_id < WLAN_CFG_PRIV_BUTT; l_cfg_id++) {
        /* ��ȡ private ������ֵ */
        l_ret = get_cust_conf_int32(INI_MODU_WIFI, g_wifi_config_priv[l_cfg_id].name,
                                    &(g_priv_cust_params[l_cfg_id].l_val));
        if (l_ret == INI_FAILED) {
            g_priv_cust_params[l_cfg_id].en_value_state = OAL_FALSE;
            continue;
        }
        g_priv_cust_params[l_cfg_id].en_value_state = OAL_TRUE;
    }

    oam_warning_log0(0, OAM_SF_CFG, "hwifi_config_init_private_custom read from ini success!");

    return INI_SUCC;
}

/*
 * �� �� ��  : hwifi_config_get_switch_point_5g
 * ��������  : ����ini�ļ���ȡ5G�������߹����л���
 */
OAL_STATIC oal_void hwifi_config_get_5g_curv_switch_point(oal_uint8 *puc_ini_pa_params, oal_uint32 ul_cfg_id)
{
    oal_int32 l_ini_params[CUS_NUM_5G_BW * DY_CALI_PARAMS_TIMES] = {0};
    oal_uint16 us_ini_param_num = 0;
    oal_uint8 uc_secon_ratio_idx = 0;
    oal_uint8 uc_param_idx;
    oal_uint8 uc_chain_idx;
    oal_int16 *ps_extre_point_val;

    if ((ul_cfg_id == WLAN_CFG_DTS_NVRAM_RATIO_PA5GA0) || (ul_cfg_id == WLAN_CFG_DTS_NVRAM_RATIO_PA5GA0_BAND1)) {
        uc_chain_idx = WLAN_RF_CHANNEL_ZERO;
    } else if ((ul_cfg_id == WLAN_CFG_DTS_NVRAM_RATIO_PA5GA1) || (ul_cfg_id == WLAN_CFG_DTS_NVRAM_RATIO_PA5GA1_BAND1)) {
        uc_chain_idx = WLAN_RF_CHANNEL_ONE;
    } else {
        return;
    }

    /* ��ȡ���ϵ���� */
    if (hwifi_config_sepa_coefficient_from_param(puc_ini_pa_params, l_ini_params, &us_ini_param_num,
                                                 OAL_SIZEOF(l_ini_params) / OAL_SIZEOF(oal_int32)) != OAL_SUCC ||
        (us_ini_param_num % DY_CALI_PARAMS_TIMES)) {
        oam_error_log2(0, OAM_SF_CUSTOM,
                       "hwifi_config_get_5g_curv_switch_point::ini is unsuitable,num of ini[%d] cfg_id[%d]!",
                       us_ini_param_num, ul_cfg_id);
        return;
    }

    ps_extre_point_val = g_gs_extre_point_vals[uc_chain_idx];
    us_ini_param_num /= DY_CALI_PARAMS_TIMES;
    if (ul_cfg_id <= WLAN_CFG_DTS_NVRAM_RATIO_PA5GA1) {
        if (us_ini_param_num != CUS_NUM_5G_BW) {
            oam_error_log2(0, OAM_SF_CUSTOM,
                           "hwifi_config_get_5g_curv_switch_point::ul_cfg_id[%d] us_ini_param_num[%d]",
                           ul_cfg_id, us_ini_param_num);
            return;
        }
        ps_extre_point_val++;
    } else {
        if (us_ini_param_num != 1) {
            oam_error_log2(0, OAM_SF_CUSTOM,
                           "hwifi_config_get_5g_curv_switch_point::ul_cfg_id[%d] us_ini_param_num[%d]",
                           ul_cfg_id, us_ini_param_num);
            return;
        }
    }

    /* ����5g����switch point */
    for (uc_param_idx = 0; uc_param_idx < us_ini_param_num; uc_param_idx++) {
        *(ps_extre_point_val + uc_param_idx) = (oal_int16)hwifi_dyn_cali_get_extre_point(l_ini_params +
                                               uc_secon_ratio_idx);
        oal_io_print("hwifi_config_get_5g_curv_switch_point::extre power[%d] param_idx[%d] cfg_id[%d]!\r\n",
                     *(ps_extre_point_val + uc_param_idx), uc_param_idx, ul_cfg_id);
        oal_io_print("hwifi_config_get_5g_curv_switch_point::param[%d %d] uc_secon_ratio_idx[%d]!\r\n",
            (l_ini_params + uc_secon_ratio_idx)[0], (l_ini_params + uc_secon_ratio_idx)[1], uc_secon_ratio_idx);
        uc_secon_ratio_idx += DY_CALI_PARAMS_TIMES;
    }

    return;
}

/*
 * �� �� ��  : hwifi_config_nvram_second_coefficient_check
 * ��������  : �������nvram�еĶ���ϵ���Ƿ����
 */
OAL_STATIC oal_uint32 hwifi_config_nvram_second_coefficient_check(oal_uint8 *puc_g_cust_nvram_info,
                                                                  oal_uint8 *puc_ini_pa_params,
                                                                  oal_uint32 ul_cfg_id,
                                                                  oal_int16 *ps_5g_delt_power)
{
    oal_int32 l_ini_params[CUS_NUM_5G_BW * DY_CALI_PARAMS_TIMES] = {0};
    oal_int32 l_nv_params[CUS_NUM_5G_BW * DY_CALI_PARAMS_TIMES] = {0};
    oal_uint16 us_ini_param_num = 0;
    oal_uint16 us_nv_param_num = 0;
    oal_uint8 uc_secon_ratio_idx = 0;
    oal_uint8 uc_param_idx;

    /* ��ȡ���ϵ���� */
    if (hwifi_config_sepa_coefficient_from_param(puc_g_cust_nvram_info, l_nv_params, &us_nv_param_num,
                                                 OAL_SIZEOF(l_nv_params) / OAL_SIZEOF(oal_int16)) != OAL_SUCC ||
        (us_nv_param_num % DY_CALI_PARAMS_TIMES) ||
        hwifi_config_sepa_coefficient_from_param(puc_ini_pa_params, l_ini_params, &us_ini_param_num,
                                                 OAL_SIZEOF(l_ini_params) / OAL_SIZEOF(oal_int16)) != OAL_SUCC ||
        (us_ini_param_num % DY_CALI_PARAMS_TIMES) || (us_nv_param_num != us_ini_param_num)) {
        oam_error_log2(0, OAM_SF_CUSTOM,
            "hwifi_config_nvram_second_coefficient_check::nvram or ini is unsuitable,num of nv and ini[%d %d]!",
            us_nv_param_num, us_ini_param_num);
        return OAL_FAIL;
    }

    us_nv_param_num /= DY_CALI_PARAMS_TIMES;
    /* ���nv��ini�ж���ϵ���Ƿ�ƥ�� */
    for (uc_param_idx = 0; uc_param_idx < us_nv_param_num; uc_param_idx++) {
        if (l_ini_params[uc_secon_ratio_idx] != l_nv_params[uc_secon_ratio_idx]) {
            oam_warning_log4(0, OAM_SF_CUSTOM,
                             "hwifi_config_nvram_second_coefficient_check::nvram get mismatch value idx[%d %d] val are [%d] and [%d]!",
                             uc_param_idx, uc_secon_ratio_idx,
                             l_ini_params[uc_secon_ratio_idx],
                             l_nv_params[uc_secon_ratio_idx]);

            /* ���������ϵ����nvram��Ϊ׼��ˢ��NV�еĶ�����������л��� */
            hwifi_config_get_5g_curv_switch_point(puc_g_cust_nvram_info, ul_cfg_id);
            uc_secon_ratio_idx += DY_CALI_PARAMS_TIMES;
            continue;
        }

        if ((ul_cfg_id == WLAN_CFG_DTS_NVRAM_RATIO_PA5GA0) || (ul_cfg_id == WLAN_CFG_DTS_NVRAM_RATIO_PA5GA1)) {
            /* ��������ϵ�delt power */
            *(ps_5g_delt_power + uc_param_idx) = hwifi_get_5g_pro_line_delt_pow_per_band(l_nv_params +
                                                                                         uc_secon_ratio_idx,
                                                                                         l_ini_params +
                                                                                         uc_secon_ratio_idx);
            oal_io_print("hwifi_config_nvram_second_coefficient_check::delt power[%d] param_idx[%d] cfg_id[%d]!\r\n",
                         *(ps_5g_delt_power + uc_param_idx), uc_param_idx, ul_cfg_id);
        }
        uc_secon_ratio_idx += DY_CALI_PARAMS_TIMES;
    }

    return OAL_SUCC;
}

/*
 * �� �� ��  : hwifi_config_init_dy_cali_custom
 * ��������  : ��ȡ���ƻ��ļ��Ͷ��β���ϵ��
 */
OAL_STATIC oal_uint32 hwifi_config_init_dy_cali_custom(oal_void)
{
    oal_uint32 ul_cfg_id;
    oal_uint32 ul_ret = OAL_SUCC;
    oal_uint8 uc_idx = 0;
    oal_uint16 us_param_num = 0;
    oal_uint16 us_per_param_num = 0;
    oal_uint8 uc_rf_idx;
    oal_uint8 uc_cali_param_idx;
    oal_int16 s_5g_delt_power[WLAN_RF_CHANNEL_NUMS][CUS_NUM_5G_BW] = {{0}};
    oal_uint8 uc_delt_pwr_idx = 0;
    oal_uint32 ul_cfg_id_tmp;
    oal_uint8 *puc_g_cust_nvram_info = OAL_PTR_NULL;
    oal_uint8 *puc_nv_pa_params = OAL_PTR_NULL;
    oal_int32 *pl_params = OAL_PTR_NULL;
    oal_uint16 us_param_len = WLAN_RF_CHANNEL_NUMS * DY_CALI_PARAMS_TIMES *
                              DY_CALI_PARAMS_NUM * OAL_SIZEOF(oal_int32);

    puc_nv_pa_params = (oal_uint8 *)os_kzalloc_gfp(CUS_PARAMS_LEN_MAX * OAL_SIZEOF(oal_uint8));
    if (puc_nv_pa_params == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_CUSTOM, "hwifi_config_init_dy_cali_custom::puc_nv_pa_params mem alloc fail!");
        return OAL_FAIL;
    }

    pl_params = (oal_int32 *)os_kzalloc_gfp(us_param_len);
    if (pl_params == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_CUSTOM, "hwifi_config_init_dy_cali_custom::ps_params mem alloc fail!");
        os_mem_kfree(puc_nv_pa_params);
        return OAL_FAIL;
    }

    memset_s(puc_nv_pa_params,
             CUS_PARAMS_LEN_MAX * OAL_SIZEOF(oal_uint8),
             0,
             CUS_PARAMS_LEN_MAX * OAL_SIZEOF(oal_uint8));
    memset_s(pl_params, us_param_len, 0, us_param_len);

    for (ul_cfg_id = WLAN_CFG_DTS_NVRAM_RATIO_PA2GCCKA0; ul_cfg_id < WLAN_CFG_DTS_NVRAM_PARAMS_BUTT; ul_cfg_id++) {
        /* �������ϵ�� */
        if ((ul_cfg_id >= WLAN_CFG_DTS_NVRAM_MUFREQ_2GCCK_C0) && (ul_cfg_id < WLAN_CFG_DTS_NVRAM_END)) {
            /* DPN */
            continue;
        }

        if (get_cust_conf_string(INI_MODU_WIFI, g_nvram_pro_line_config_ini[ul_cfg_id].name,
                                 puc_nv_pa_params, CUS_PARAMS_LEN_MAX - 1) == INI_FAILED) {
            if (oal_value_eq_any4(ul_cfg_id, WLAN_CFG_DTS_NVRAM_RATIO_PA5GA0_LOW,
                                  WLAN_CFG_DTS_NVRAM_RATIO_PA5GA1_LOW,
                                  WLAN_CFG_DTS_NVRAM_RATIO_PA5GA0_BAND1_LOW,
                                  WLAN_CFG_DTS_NVRAM_RATIO_PA5GA1_BAND1_LOW)) {
                ul_cfg_id_tmp = ((ul_cfg_id == WLAN_CFG_DTS_NVRAM_RATIO_PA5GA0_BAND1_LOW) ?
                                 WLAN_CFG_DTS_NVRAM_RATIO_PA5GA0_BAND1 :
                                 (ul_cfg_id == WLAN_CFG_DTS_NVRAM_RATIO_PA5GA0_LOW) ?
                                 WLAN_CFG_DTS_NVRAM_RATIO_PA5GA0 :
                                 (ul_cfg_id == WLAN_CFG_DTS_NVRAM_RATIO_PA5GA1_BAND1_LOW) ?
                                 WLAN_CFG_DTS_NVRAM_RATIO_PA5GA1_BAND1 :
                                 (ul_cfg_id == WLAN_CFG_DTS_NVRAM_RATIO_PA5GA1_LOW) ?
                                 WLAN_CFG_DTS_NVRAM_RATIO_PA5GA1 : ul_cfg_id);
                get_cust_conf_string(INI_MODU_WIFI, g_nvram_pro_line_config_ini[ul_cfg_id_tmp].name,
                                     puc_nv_pa_params, CUS_PARAMS_LEN_MAX - 1);
            } else {
                oam_error_log1(0, OAM_SF_CUSTOM, "hwifi_config_init_dy_cali_custom read, check id[%d] exists!",
                               ul_cfg_id);
                ul_ret = OAL_FAIL;
                break;
            }
        }

        /* ��ȡini�еĶ�����������л��� */
        hwifi_config_get_5g_curv_switch_point(puc_nv_pa_params, ul_cfg_id);

        if (ul_cfg_id <= WLAN_CFG_DTS_NVRAM_RATIO_PA5GA1_LOW) {
            puc_g_cust_nvram_info = hwifi_get_nvram_param(ul_cfg_id);
            /* ��ȡnv�еĲ���ֵ,Ϊ�����ini�ļ��ж�ȡ */
            if (OAL_STRLEN(puc_g_cust_nvram_info)) {
                /* NVRAM����ϵ���쳣���� */
                if (hwifi_config_nvram_second_coefficient_check(puc_g_cust_nvram_info, puc_nv_pa_params,
                                                                ul_cfg_id, s_5g_delt_power[ul_cfg_id <
                                                                WLAN_CFG_DTS_NVRAM_RATIO_PA2GCCKA1 ?
                                                                WLAN_RF_CHANNEL_ZERO :
                                                                WLAN_RF_CHANNEL_ONE]) == OAL_SUCC) {
                    /* �ֻ����low partΪ��,��ȡini�е�ϵ��,�����ݲ���������;����ֱ�Ӵ�nvram��ȡ�� */
                    if ((ul_cfg_id == WLAN_CFG_DTS_NVRAM_RATIO_PA5GA0_LOW) &&
                        (oal_memcmp(puc_g_cust_nvram_info, puc_nv_pa_params, OAL_STRLEN(puc_g_cust_nvram_info)))) {
                        memset_s(s_5g_delt_power[WLAN_RF_CHANNEL_ZERO], CUS_NUM_5G_BW * OAL_SIZEOF(oal_int16),
                                 0, CUS_NUM_5G_BW * OAL_SIZEOF(oal_int16));
                    }
                    if ((ul_cfg_id == WLAN_CFG_DTS_NVRAM_RATIO_PA5GA1_LOW) &&
                        (oal_memcmp(puc_g_cust_nvram_info, puc_nv_pa_params, OAL_STRLEN(puc_g_cust_nvram_info)))) {
                        memset_s(s_5g_delt_power[WLAN_RF_CHANNEL_ONE], CUS_NUM_5G_BW * OAL_SIZEOF(oal_int16),
                                 0, CUS_NUM_5G_BW * OAL_SIZEOF(oal_int16));
                    }

                    if (memcpy_s(puc_nv_pa_params, CUS_PARAMS_LEN_MAX * OAL_SIZEOF(oal_uint8),
                                 puc_g_cust_nvram_info, OAL_STRLEN(puc_g_cust_nvram_info)) != EOK) {
                        ul_ret = OAL_FAIL;
                        break;
                    }
                } else {
                    ul_ret = OAL_FAIL;
                    break;
                }
            } else {
                /* �ṩ���ߵ�һ���ϵ�У׼��ʼֵ */
                if (memcpy_s(puc_g_cust_nvram_info, CUS_PARAMS_LEN_MAX,
                             puc_nv_pa_params, OAL_STRLEN(puc_nv_pa_params)) != EOK) {
                    ul_ret = OAL_FAIL;
                    break;
                }
            }
        }

        if (hwifi_config_sepa_coefficient_from_param(puc_nv_pa_params, pl_params + us_param_num,
                                                     &us_per_param_num, us_param_len - us_param_num) != OAL_SUCC ||
            (us_per_param_num % DY_CALI_PARAMS_TIMES)) {
            ul_ret = OAL_FAIL;
            oam_error_log3(0, OAM_SF_CUSTOM,
                           "hwifi_config_init_dy_cali_custom read get wrong value,len[%d] check id[%d] exists us_per_param_num[%d]!",
                           OAL_STRLEN(puc_g_cust_nvram_info), ul_cfg_id, us_per_param_num);
            break;
        }
        us_param_num += us_per_param_num;
    }

    os_mem_kfree(puc_nv_pa_params);

    if (ul_ret == OAL_FAIL) {
        /* �����ֹ�·���device */
        memset_s(g_pro_line_params, OAL_SIZEOF(g_pro_line_params), 0, OAL_SIZEOF(g_pro_line_params));
    } else {
        if (us_param_num != us_param_len / OAL_SIZEOF(oal_int32)) {
            oam_error_log1(0, OAM_SF_CUSTOM,
                           "hwifi_config_init_dy_cali_custom read get wrong ini value num[%d]!", us_param_num);
            memset_s(g_pro_line_params, OAL_SIZEOF(g_pro_line_params), 0, OAL_SIZEOF(g_pro_line_params));
            os_mem_kfree(pl_params);
            return OAL_FAIL;
        }
        for (uc_rf_idx = 0; uc_rf_idx < WLAN_RF_CHANNEL_NUMS; uc_rf_idx++) {
            for (uc_cali_param_idx = 0; uc_cali_param_idx < DY_CALI_PARAMS_BASE_NUM; uc_cali_param_idx++) {
                if (uc_cali_param_idx == (DY_2G_CALI_PARAMS_NUM - 1)) {
                    /* band1 & CW */
                    uc_cali_param_idx += PRO_LINE_2G_TO_5G_OFFSET;
                }
                g_pro_line_params[uc_rf_idx][uc_cali_param_idx].l_pow_par2 = pl_params[uc_idx++];
                g_pro_line_params[uc_rf_idx][uc_cali_param_idx].l_pow_par1 = pl_params[uc_idx++];
                g_pro_line_params[uc_rf_idx][uc_cali_param_idx].l_pow_par0 = pl_params[uc_idx++];
            }
        }

        /* 5g band2&3 4&5 6 7 low power */
        for (uc_rf_idx = 0; uc_rf_idx < WLAN_RF_CHANNEL_NUMS; uc_rf_idx++) {
            uc_delt_pwr_idx = 0;
            for (uc_cali_param_idx = DY_CALI_PARAMS_BASE_NUM + 1;
                 uc_cali_param_idx < DY_CALI_PARAMS_NUM - 1; uc_cali_param_idx++) {
                g_pro_line_params[uc_rf_idx][uc_cali_param_idx].l_pow_par2 = pl_params[uc_idx++];
                g_pro_line_params[uc_rf_idx][uc_cali_param_idx].l_pow_par1 = pl_params[uc_idx++];
                g_pro_line_params[uc_rf_idx][uc_cali_param_idx].l_pow_par0 = pl_params[uc_idx++];

                cus_flush_nv_ratio_by_delt_pow(g_pro_line_params[uc_rf_idx][uc_cali_param_idx].l_pow_par2,
                                               g_pro_line_params[uc_rf_idx][uc_cali_param_idx].l_pow_par1,
                                               g_pro_line_params[uc_rf_idx][uc_cali_param_idx].l_pow_par0,
                                               s_5g_delt_power[uc_rf_idx][uc_delt_pwr_idx]);
                uc_delt_pwr_idx++;
            }
        }

        /* band1 & CW */
        for (uc_rf_idx = 0; uc_rf_idx < WLAN_RF_CHANNEL_NUMS; uc_rf_idx++) {
            g_pro_line_params[uc_rf_idx][DY_2G_CALI_PARAMS_NUM].l_pow_par2 = pl_params[uc_idx++];
            g_pro_line_params[uc_rf_idx][DY_2G_CALI_PARAMS_NUM].l_pow_par1 = pl_params[uc_idx++];
            g_pro_line_params[uc_rf_idx][DY_2G_CALI_PARAMS_NUM].l_pow_par0 = pl_params[uc_idx++];
        }
        for (uc_rf_idx = 0; uc_rf_idx < WLAN_RF_CHANNEL_NUMS; uc_rf_idx++) {
            g_pro_line_params[uc_rf_idx][DY_2G_CALI_PARAMS_NUM - 1].l_pow_par2 = pl_params[uc_idx++];
            g_pro_line_params[uc_rf_idx][DY_2G_CALI_PARAMS_NUM - 1].l_pow_par1 = pl_params[uc_idx++];
            g_pro_line_params[uc_rf_idx][DY_2G_CALI_PARAMS_NUM - 1].l_pow_par0 = pl_params[uc_idx++];
        }
        for (uc_rf_idx = 0; uc_rf_idx < WLAN_RF_CHANNEL_NUMS; uc_rf_idx++) {
            /* 5g band1 low power */
            /* band1���߲�У׼ */
            g_pro_line_params[uc_rf_idx][DY_CALI_PARAMS_BASE_NUM].l_pow_par2 = pl_params[uc_idx++];
            g_pro_line_params[uc_rf_idx][DY_CALI_PARAMS_BASE_NUM].l_pow_par1 = pl_params[uc_idx++];
            g_pro_line_params[uc_rf_idx][DY_CALI_PARAMS_BASE_NUM].l_pow_par0 = pl_params[uc_idx++];
        }

        for (uc_rf_idx = 0; uc_rf_idx < WLAN_RF_CHANNEL_NUMS; uc_rf_idx++) {
            /* 2g cw ppa */
            g_pro_line_params[uc_rf_idx][DY_CALI_PARAMS_NUM - 1].l_pow_par2 = pl_params[uc_idx++];
            g_pro_line_params[uc_rf_idx][DY_CALI_PARAMS_NUM - 1].l_pow_par1 = pl_params[uc_idx++];
            g_pro_line_params[uc_rf_idx][DY_CALI_PARAMS_NUM - 1].l_pow_par0 = pl_params[uc_idx++];
        }
    }

    os_mem_kfree(pl_params);
    return ul_ret;
}

/*
 * �� �� ��  : hwifi_config_init
 * ��������  : netdev open ���õĶ��ƻ������
 *             ��ȡini�ļ������� g_host_init_params ȫ������
 */
int32 hwifi_config_init(int32 cus_tag)
{
    int32 l_cfg_id;
    int32 l_ret = INI_FAILED;
    int32 l_ori_val;
    wlan_cfg_cmd *pgast_wifi_config;
    int32 *pgal_params;
    int32 l_cfg_value = 0;
    int32 l_wlan_cfg_butt;

    switch (cus_tag) {
        case CUS_TAG_NV:
            original_value_for_nvram_params();
            return hwifi_config_init_nvram();
        case CUS_TAG_INI:
            host_params_init_first();
            pgast_wifi_config = g_wifi_config_cmds;
            pgal_params = g_host_init_params;
            l_wlan_cfg_butt = WLAN_CFG_INIT_BUTT;
            break;
        case CUS_TAG_DTS:
            original_value_for_dts_params();
            pgast_wifi_config = g_wifi_config_dts;
            pgal_params = g_dts_params;
            l_wlan_cfg_butt = WLAN_CFG_DTS_BUTT;
            break;
        case CUS_TAG_PRIV_INI:
            return hwifi_config_init_private_custom();
        case CUS_TAG_PRO_LINE_INI:
            return hwifi_config_init_dy_cali_custom();
        default:
            oam_error_log1(0, OAM_SF_CUSTOM, "hwifi_config_init tag number[0x%x] not correct!", cus_tag);
            return INI_FAILED;
    }

    for (l_cfg_id = 0; l_cfg_id < l_wlan_cfg_butt; l_cfg_id++) {
        /* ��ȡini������ֵ */
        l_ret = get_cust_conf_int32(INI_MODU_WIFI, pgast_wifi_config[l_cfg_id].name, &l_cfg_value);
        if (l_ret == INI_FAILED) {
            oam_info_log2(0, OAM_SF_CUSTOM, "hwifi_config_init read ini file cfg_id[%d]tag[%d] not exist!",
                          l_cfg_id, cus_tag);
            continue;
        }
        l_ori_val = pgal_params[pgast_wifi_config[l_cfg_id].case_entry];
        pgal_params[pgast_wifi_config[l_cfg_id].case_entry] = l_cfg_value;
    }

    return INI_SUCC;
}

#ifndef _PRE_PRODUCT_HI3751V811
/*
 * �� �� ��  : char2byte
 * ��������  : ͳ��ֵ���ж����޶�ȡ��mac��ַ
 */
OAL_STATIC uint32 char2byte(const char *strori, char *outbuf)
{
    int i = 0;
    uint8 temp = 0;
    uint32 sum = 0;
    uint8 *ptr_out = (uint8 *)outbuf;
    const int l_loop_times = 12; /* ���ֽڱ����ǲ�����ȷ��mac��ַ:xx:xx:xx:xx:xx:xx */

    for (i = 0; i < l_loop_times; i++) {
        switch (strori[i]) {
            case '0' ... '9':
                temp = strori[i] - '0';
                break;

            case 'a' ... 'f':
                temp = (strori[i] - 'a') + 10; /* ��10Ϊ�˱�֤'a'~'f'�ֱ��Ӧ10~15 */
                break;

            case 'A' ... 'F':
                temp = (strori[i] - 'A') + 10; /* ��10Ϊ�˱�֤'A'~'F'�ֱ��Ӧ10~15 */
                break;
            default:
                break;
        }

        sum += temp;
        /* Ϊ�������ȷ��mac��ַ:xx:xx:xx:xx:xx:xx */
        if (i % 2 == 0) {
            ptr_out[i / 2] |= (temp << 4);
        } else {
            ptr_out[i / 2] |= temp;
        }
    }

    return sum;
}
#endif


#ifdef _PRE_PRODUCT_HI3751V811
#define WIFI_2G_MAC_TYPE    (1)
#define WIFI_5G_MAC_TYPE    (2)
#define WIFI_P2P_MAC_TYPE   (3)
#define BT_MAC_TYPE         (4)

extern int32 get_board_mac(uint8 *puc_buf, uint8 type);
#endif

#if defined(_PRE_CONFIG_READ_DYNCALI_E2PROM) && defined(_PRE_CONFIG_READ_E2PROM_MAC)
/*
 * �� �� ��  : hwifi_get_mac_addr_drveprom
 * ��������  : ��e2prom�л�ȡmac��ַ���ú�_PRE_CONFIG_READ_E2PROM_MACADDR����PC�ʹ���
 */
int32 hwifi_get_mac_addr_drveprom(uint8 *puc_buf)
{
#define READ_CHECK_MAX_CNT    20
#define READ_CHECK_WAIT_TIME  50

    char original_mac_addr[12] = {0};
    int32 ret = INI_FAILED;
    int32 i;
    unsigned int offset = 0;
    unsigned int bit_Len = 12;
    uint32 ul_sum = 0;

    if (g_wifi_mac[0] != 0 || g_wifi_mac[1]  != 0 || g_wifi_mac[2]  != 0 || g_wifi_mac[3] != 0
        || g_wifi_mac[4] != 0 || g_wifi_mac[5] != 0) {
        if (memcpy_s(puc_buf, WLAN_MAC_ADDR_LEN, g_wifi_mac, MAC_LEN) != EOK) {
            return INI_FAILED;
        }

        ini_warning("hwifi_get_mac_addr_drveprom get MAC from g_wifi_mac SUCC\n");
        return INI_SUCC;
    }

    for (i = 0; i < READ_CHECK_MAX_CNT; i++) {
        if (DrvEepromRead("MACWLAN", offset, original_mac_addr, bit_Len) == INI_SUCC) {
            ini_warning("hwifi_get_mac_addr_drveprom get MAC from EEPROM SUCC\n");
            ret = INI_SUCC;
            break;
        }

        msleep(READ_CHECK_WAIT_TIME);
    }

    if (ret == INI_SUCC) {
        oal_io_print("hwifi_get_mac_addr_drveprom DrvEepromRead return success\n");
        ul_sum = char2byte(original_mac_addr, (int8 *)puc_buf);
        if (ul_sum != 0) {
            ini_warning("hwifi_get_mac_addr_drveprom get MAC from EEPROM: mac=" MACFMT "\n", ini_mac2str(puc_buf));
            if (memcpy_s(g_wifi_mac, MAC_LEN, puc_buf, MAC_LEN) != EOK) {
                ini_warning("hwifi_get_mac_addr_drveprom memcpy_s g_wifi_mac fail\n");
            }
        } else {
            ini_warning("hwifi_get_mac_addr_drveprom get MAC from EEPROM is not char,use random mac\n");
            random_ether_addr(puc_buf);
            puc_buf[1] = 0x11;
            puc_buf[2] = 0x03;
        }
    } else {
        oal_io_print("hwifi_get_mac_addr DrvEepromRead return fail,use random mac\n");
        chr_exception_report(CHR_READ_EEPROM_ERROR_EVENTID, CHR_SYSTEM_WIFI, CHR_LAYER_DRV,
                             CHR_PLT_DRV_EVENT_INIT, CHR_PLAT_DRV_ERROR_EEPROM_READ_INIT);
        random_ether_addr(puc_buf);
        puc_buf[1] = 0x11;
        puc_buf[2] = 0x03;
    }

    return ret;
}
#endif

/*
 * �� �� ��  : hwifi_get_mac_addr
 * ��������  : ��e2prom�л�ȡmac��ַ���ú�_PRE_CONFIG_READ_E2PROM_MACADDR����PC�ʹ���
 */
#if defined(_PRE_CONFIG_READ_DYNCALI_E2PROM) && (!defined(_PRE_CONFIG_READ_E2PROM_MAC))
int32 hwifi_get_mac_addr(uint8 *puc_buf)
{
    if (puc_buf == NULL) {
        oam_error_log0(0, OAM_SF_ANY, "hwifi_get_mac_addr::buf is NULL!");
        return INI_FAILED;
    }

    if (get_board_mac(puc_buf, WIFI_2G_MAC_TYPE) == 0) {
        ini_warning("hwifi_get_mac_addr get MAC from NV: mac="MACFMT"\n", ini_mac2str(puc_buf));
        if (memcpy_s(g_wifi_mac, MAC_LEN, puc_buf, MAC_LEN) != EOK) {
            oam_error_log0(0, OAM_SF_ANY, "hwifi_get_mac_addr::memcpy_s mac to g_wifi_mac failed");
        }
    } else {
        random_ether_addr(puc_buf);
        puc_buf[1] = 0x11;
        puc_buf[2] = 0x02;
        ini_warning("hwifi_get_mac_addr get random mac: mac="MACFMT"\n", ini_mac2str(puc_buf));
    }
    return INI_SUCC;
}

#elif defined(_PRE_CONFIG_READ_DYNCALI_E2PROM) && defined(_PRE_CONFIG_READ_E2PROM_MAC)
int32 hwifi_get_mac_addr(uint8 *puc_buf)
{
    return hwifi_get_mac_addr_drveprom(puc_buf);
}

#else
int32 hwifi_get_mac_addr(uint8 *puc_buf)
{
    struct hisi_nve_info_user st_info;
    int32 l_ret;
    uint32 sum = 0;

    if (puc_buf == NULL) {
        oam_error_log0(0, OAM_SF_ANY, "hwifi_get_mac_addr::buf is NULL!");
        return INI_FAILED;
    }

    memset_s(puc_buf, MAC_LEN, 0, MAC_LEN);

    memset_s(&st_info, sizeof(st_info), 0, sizeof(st_info));

    st_info.nv_number = NV_WLAN_NUM;  // nve item

    if (strcpy_s(st_info.nv_name, sizeof(st_info.nv_name), "MACWLAN") != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "hwifi_get_mac_addr:: strcpy_s failed.");
        return INI_FAILED;
    }

    st_info.valid_size = NV_WLAN_VALID_SIZE;
    st_info.nv_operation = NV_READ;

    if (g_wifi_mac[0] != 0 || g_wifi_mac[1]  != 0 || g_wifi_mac[2]  != 0 || g_wifi_mac[3] != 0
        || g_wifi_mac[4] != 0 || g_wifi_mac[5] != 0) {
        if (memcpy_s(puc_buf, WLAN_MAC_ADDR_LEN, g_wifi_mac, MAC_LEN) != EOK) {
            return INI_FAILED;
        }

        return INI_SUCC;
    }

    l_ret = hisi_nve_direct_access(&st_info);
    if (!l_ret) {
        sum = char2byte(st_info.nv_data, (int8 *)puc_buf);
        if (sum != 0) {
            ini_warning("hwifi_get_mac_addr get MAC from NV: mac=" MACFMT "\n", ini_mac2str(puc_buf));
            if (memcpy_s(g_wifi_mac, MAC_LEN, puc_buf, MAC_LEN) != EOK) {
                return INI_FAILED;
            }
        } else {
            random_ether_addr(puc_buf);
            puc_buf[1] = 0x11;
            puc_buf[2] = 0x02;
        }
    } else {
        random_ether_addr(puc_buf);
        puc_buf[1] = 0x11;
        puc_buf[2] = 0x02;
    }

    return INI_SUCC;
}
#endif

int32 hwifi_get_init_value(int32 cus_tag, int32 cfg_id)
{
    int32 *pgal_params = OAL_PTR_NULL;
    int32 l_wlan_cfg_butt;

    if (cus_tag == CUS_TAG_INI) {
        pgal_params = &g_host_init_params[0];
        l_wlan_cfg_butt = WLAN_CFG_INIT_BUTT;
    } else if (cus_tag == CUS_TAG_DTS) {
        pgal_params = &g_dts_params[0];
        l_wlan_cfg_butt = WLAN_CFG_DTS_BUTT;
    } else {
        oam_error_log1(0, OAM_SF_ANY, "hwifi_get_init_value tag number[0x%2x] not correct!", cus_tag);
        return INI_FAILED;
    }

    if (cfg_id < 0 || l_wlan_cfg_butt <= cfg_id) {
        oam_error_log2(0, OAM_SF_ANY, "hwifi_get_init_value cfg id[%d] out of range, max cfg id is:%d",
                       cfg_id, (l_wlan_cfg_butt - 1));
        return INI_FAILED;
    }

    return pgal_params[cfg_id];
}

int32 hwifi_get_init_priv_value(oal_int32 l_cfg_id, oal_int32 *pl_priv_value)
{
    if (l_cfg_id < 0 || l_cfg_id >= WLAN_CFG_PRIV_BUTT) {
        oam_error_log2(0, OAM_SF_ANY, "hwifi_get_init_priv_value cfg id[%d] out of range, max[%d]",
                       l_cfg_id, WLAN_CFG_PRIV_BUTT - 1);
        return OAL_FAIL;
    }

    if (g_priv_cust_params[l_cfg_id].en_value_state == OAL_FALSE) {
        return OAL_FAIL;
    }

    *pl_priv_value = g_priv_cust_params[l_cfg_id].l_val;

    return OAL_SUCC;
}
int8 *hwifi_get_country_code(void)
{
    /* Note:declaration of symbol "l_ret" hides symbol "l_ret" */
    /*lint -e578*/
    int32 l_ret;
    /*lint +e578*/
    if (g_wifi_country_code[0] != '0' && g_wifi_country_code[1] != '0') {
        return g_wifi_country_code;
    }

    /* ��ȡcust������ */
    l_ret = get_cust_conf_string(INI_MODU_WIFI, STR_COUNTRY_CODE, g_wifi_country_code, sizeof(g_wifi_country_code));
    if (l_ret == INI_FAILED) {
        oam_warning_log0(0, OAM_SF_ANY, "hwifi_get_country_code read country code failed, check if it exists!");
        if (strncpy_s(g_wifi_country_code, COUNTRY_CODE_LEN, "99", sizeof("99") - 1) != EOK) { /* ����ȥ�������� */
            return g_wifi_country_code;
        }
    } else {
        if (!OAL_MEMCMP(g_wifi_country_code, "99", sizeof("99") - 1)) { /* ����ȥ�������� */
            oam_warning_log0(0, OAM_SF_ANY, "hwifi_get_country_code is set 99!");
            g_cust_country_code_ignore_flag.en_country_code_ingore_ini_flag = OAL_TRUE;
        }
    }
    g_wifi_country_code[COUNTRY_CODE_LEN - 1] = '\0'; /* ��'\0'��β */

    return g_wifi_country_code;
}
void hwifi_set_country_code(const int8 *country_code, const uint32 len)
{
    oal_int32 l_ret;

    if (country_code == OAL_PTR_NULL || len != COUNTRY_CODE_LEN) {
        oam_error_log0(0, OAM_SF_ANY, "hwifi_get_country_code ptr null or illegal len!");
        return;
    }

    l_ret = memcpy_s(g_wifi_country_code, COUNTRY_CODE_LEN, country_code, COUNTRY_CODE_LEN);
    if (l_ret != EOK) {
        return;
    }

    g_wifi_country_code[COUNTRY_CODE_LEN - 1] = '\0'; /* ��'\0'��β */
}

/*
 * �� �� ��  : hwifi_get_init_nvram_params
 * ��������  : ��ȡ���ƻ�NV���ݽṹ��
 */
void *hwifi_get_init_nvram_params(void)
{
    return &g_cust_nv_params;
}

/*
 * �� �� ��  : hwifi_get_nvram_params
 * ��������  : ��ȡ���ƻ�����书�ʺͶ�Ӧ��scalingֵ
 */
void *hwifi_get_nvram_params(void)
{
    return &g_cust_nv_params.st_pow_ctrl_custom_param;
}

/*
 * �� �� ��  : hwifi_get_cfg_params
 * ��������  : host�鿴ini���ƻ�����ά������
 */
oal_void hwifi_get_cfg_pow_ctrl_params(oal_uint8 uc_chn_idx)
{
    int32 l_cfg_idx_one = 0;
    int32 l_cfg_idx_two = 0;
    wlan_cust_nvram_params *pst_g_cust_nv_params = hwifi_get_nvram_params();
    wlan_cust_cfg_custom_fcc_ce_txpwr_limit_stru *pst_fcc_ce_param;

    pst_fcc_ce_param = &pst_g_cust_nv_params->ast_fcc_ce_param[uc_chn_idx];
    for (l_cfg_idx_one = 0; l_cfg_idx_one < CUS_NUM_5G_20M_SIDE_BAND; l_cfg_idx_one++) {
        oal_io_print("%s[%d] \t [config:%d]\n", "fcc_txpwr_limit_5g:20M side_band", l_cfg_idx_one,
                     pst_fcc_ce_param->auc_5g_fcc_txpwr_limit_params_20M[l_cfg_idx_one]);
    }

    for (l_cfg_idx_one = 0; l_cfg_idx_one < CUS_NUM_5G_40M_SIDE_BAND; l_cfg_idx_one++) {
        oal_io_print("%s[%d] \t [config:%d]\n", "fcc_txpwr_limit_5g:40M side_band", l_cfg_idx_one,
                     pst_fcc_ce_param->auc_5g_fcc_txpwr_limit_params_40M[l_cfg_idx_one]);
    }
    for (l_cfg_idx_one = 0; l_cfg_idx_one < CUS_NUM_5G_80M_SIDE_BAND; l_cfg_idx_one++) {
        oal_io_print("%s[%d] \t [config:%d]\n", "fcc_txpwr_limit_5g:80M side_band", l_cfg_idx_one,
                     pst_fcc_ce_param->auc_5g_fcc_txpwr_limit_params_80M[l_cfg_idx_one]);
    }
    for (l_cfg_idx_one = 0; l_cfg_idx_one < CUS_NUM_5G_160M_SIDE_BAND; l_cfg_idx_one++) {
        oal_io_print("%s[%d] \t [config:%d]\n", "fcc_txpwr_limit_5g:160M side_band", l_cfg_idx_one,
                     pst_fcc_ce_param->auc_5g_fcc_txpwr_limit_params_160M[l_cfg_idx_one]);
    }
    for (l_cfg_idx_one = 0; l_cfg_idx_one < MAC_2G_CHANNEL_NUM; l_cfg_idx_one++) {
        for (l_cfg_idx_two = 0; l_cfg_idx_two < CUS_NUM_FCC_CE_2G_PRO; l_cfg_idx_two++) {
            oal_io_print("%s[%d] [%d] \t [config:%d]\n", "fcc_txpwr_limit_2g: chan", l_cfg_idx_one, l_cfg_idx_two,
                         pst_fcc_ce_param->auc_2g_fcc_txpwr_limit_params[l_cfg_idx_one][l_cfg_idx_two]);
        }
    }
}

/*
 * �� �� ��  : hwifi_get_cfg_params
 * ��������  : host�鿴ini���ƻ�����ά������
 */
oal_void hwifi_get_cfg_iq_lpf_lvl_params(void)
{
    int32 l_cfg_idx_one = 0;
    wlan_cust_nvram_params *pst_g_cust_nv_params = hwifi_get_nvram_params();

    for (l_cfg_idx_one = 0; l_cfg_idx_one < NUM_OF_NV_5G_LPF_LVL; ++l_cfg_idx_one) {
        oal_io_print("%s%d \t [config:%d]\n", "5g_iq_cali_lpf_lvl", l_cfg_idx_one,
                     pst_g_cust_nv_params->auc_5g_iq_cali_lpf_params[l_cfg_idx_one]);
    }
}

/*
 * �� �� ��  : hwifi_get_cfg_params
 * ��������  : host�鿴ini���ƻ�����ά������
 */
int hwifi_get_cfg_params(void)
{
    int32 l_cfg_idx_one = 0;
    int32 l_cfg_idx_two = 0;
    wlan_cust_nvram_params *pst_g_cust_nv_params = hwifi_get_nvram_params(); /* ����͹��ʶ��ƻ����� */
    wlan_init_cust_nvram_params *pst_init_g_cust_nv_params = hwifi_get_init_nvram_params();

    oal_io_print("\nhwifi_get_cfg_params\n");

    // CUS_TAG_INI
    for (l_cfg_idx_one = 0; l_cfg_idx_one < WLAN_CFG_INIT_BUTT; ++l_cfg_idx_one) {
        oal_io_print("%s \t [config:0x%x]\n", g_wifi_config_cmds[l_cfg_idx_one].name,
                     g_host_init_params[l_cfg_idx_one]);
    }

    // CUS_TAG_TXPWR
    for (l_cfg_idx_one = 0; l_cfg_idx_one < NUM_OF_NV_MAX_TXPOWER; ++l_cfg_idx_one) {
        oal_io_print("%s%d \t [config:%d]\n", "delt_txpwr_params", l_cfg_idx_one,
                     pst_g_cust_nv_params->ac_delt_txpwr_params[l_cfg_idx_one]);
    }

    for (l_cfg_idx_one = 0; l_cfg_idx_one < NUM_OF_NV_DPD_MAX_TXPOWER; ++l_cfg_idx_one) {
        oal_io_print("%s%d \t [config:%d]\n", "delt_dpd_txpwr_params",
                     l_cfg_idx_one, pst_g_cust_nv_params->ac_dpd_delt_txpwr_params[l_cfg_idx_one]);
    }

    for (l_cfg_idx_one = 0; l_cfg_idx_one < NUM_OF_NV_11B_DELTA_TXPOWER; ++l_cfg_idx_one) {
        oal_io_print("%s%d \t [config:%d]\n", "delt_11b_txpwr_params", l_cfg_idx_one,
                     pst_g_cust_nv_params->ac_11b_delt_txpwr_params[l_cfg_idx_one]);
    }

    for (l_cfg_idx_one = 0; l_cfg_idx_one < WLAN_RF_CHANNEL_NUMS; ++l_cfg_idx_one) {
        oal_io_print("%s%d \t [config:%d]\n", "5g_IQ_cali_pow", l_cfg_idx_one,
                     pst_g_cust_nv_params->auc_fem_off_iq_cal_pow_params[l_cfg_idx_one]);
    }

    for (l_cfg_idx_one = 0; l_cfg_idx_one < NUM_OF_NV_5G_UPPER_UPC; ++l_cfg_idx_one) {
        oal_io_print("%s%d \t [config:%d]\n", "5g_upper_upc_params", l_cfg_idx_one,
                     pst_g_cust_nv_params->auc_5g_upper_upc_params[l_cfg_idx_one]);
    }

    for (l_cfg_idx_one = 0; l_cfg_idx_one < NUM_OF_NV_2G_LOW_POW_DELTA_VAL; ++l_cfg_idx_one) {
        oal_io_print("%s%d \t [config:%d]\n", "2g_low_pow_amend_val", l_cfg_idx_one,
                     pst_g_cust_nv_params->ac_2g_low_pow_amend_params[l_cfg_idx_one]);
    }

    for (l_cfg_idx_one = 0; l_cfg_idx_one < WLAN_RF_CHANNEL_NUMS; l_cfg_idx_one++) {
        for (l_cfg_idx_two = 0; l_cfg_idx_two < CUS_BASE_PWR_NUM_2G; l_cfg_idx_two++) {
            oal_io_print("%s[%d][%d] \t [config:%d]\n", "2G base_pwr_params", l_cfg_idx_one,
                         l_cfg_idx_two, pst_g_cust_nv_params->auc_2g_txpwr_base_params[l_cfg_idx_one][l_cfg_idx_two]);
        }
        for (l_cfg_idx_two = 0; l_cfg_idx_two < CUS_BASE_PWR_NUM_5G; l_cfg_idx_two++) {
            oal_io_print("%s[%d][%d] \t [config:%d]\n", "5G base_pwr_params", l_cfg_idx_one,
                         l_cfg_idx_two, pst_g_cust_nv_params->auc_5g_txpwr_base_params[l_cfg_idx_one][l_cfg_idx_two]);
        }
    }

    for (l_cfg_idx_one = 0; l_cfg_idx_one < WLAN_RF_CHANNEL_NUMS; l_cfg_idx_one++) {
        /* FCC/CE */
        oal_io_print("%s \t [RF:%d]\n", "hwifi_get_cfg_pow_ctrl_params", l_cfg_idx_one);
        hwifi_get_cfg_pow_ctrl_params(l_cfg_idx_one);
    }

    for (l_cfg_idx_one = 0; l_cfg_idx_one < CUS_NUM_OF_SAR_LVL; l_cfg_idx_one++) {
        for (l_cfg_idx_two = 0; l_cfg_idx_two < CUS_NUM_OF_SAR_PARAMS; l_cfg_idx_two++) {
            oal_io_print("%s[%d][%d] \t [config:C0 %d C1 %d]\n", "sar_ctrl_params: lvl", l_cfg_idx_one, l_cfg_idx_two,
                pst_init_g_cust_nv_params->st_sar_ctrl_params[l_cfg_idx_one][l_cfg_idx_two].auc_sar_ctrl_params_c0,
                pst_init_g_cust_nv_params->st_sar_ctrl_params[l_cfg_idx_one][l_cfg_idx_two].auc_sar_ctrl_params_c1);
        }
    }

#ifdef _PRE_WLAN_FEATURE_TAS_ANT_SWITCH
    for (l_cfg_idx_one = 0; l_cfg_idx_one < WLAN_RF_CHANNEL_NUMS; l_cfg_idx_one++) {
        for (l_cfg_idx_two = 0; l_cfg_idx_two < WLAN_BAND_BUTT; l_cfg_idx_two++) {
            oal_io_print("%s[%d][%d] \t [config:%d]\n", "tas_ctrl_params: lvl", l_cfg_idx_one, l_cfg_idx_two,
                         pst_g_cust_nv_params->auc_tas_ctrl_params[l_cfg_idx_one][l_cfg_idx_two]);
        }
    }
#endif

    oal_io_print("%s \t [config:%d]\n", g_nvram_config_ini[NVRAM_PARAMS_5G_FCC_CE_HIGH_BAND_MAX_PWR].name,
                 pst_g_cust_nv_params->uc_5g_max_pwr_fcc_ce_for_high_band);
    // CUS_TAG_DTS
    for (l_cfg_idx_one = 0; l_cfg_idx_one < WLAN_CFG_DTS_BUTT; ++l_cfg_idx_one) {
        oal_io_print("%s \t [config:0x%x]\n", g_wifi_config_dts[l_cfg_idx_one].name,
                     g_dts_params[l_cfg_idx_one]);
    }
    hwifi_get_cfg_iq_lpf_lvl_params();

    /* pro line */
    for (l_cfg_idx_one = 0; l_cfg_idx_one < WLAN_RF_CHANNEL_NUMS; l_cfg_idx_one++) {
        for (l_cfg_idx_two = 0; l_cfg_idx_two < DY_CALI_PARAMS_NUM; l_cfg_idx_two++) {
            oal_io_print("%s CORE[%d]para_idx[%d]::{%d, %d, %d}\n", "g_pro_line_params: ",
                         l_cfg_idx_one, l_cfg_idx_two,
                         g_pro_line_params[l_cfg_idx_one][l_cfg_idx_two].l_pow_par2,
                         g_pro_line_params[l_cfg_idx_one][l_cfg_idx_two].l_pow_par1,
                         g_pro_line_params[l_cfg_idx_one][l_cfg_idx_two].l_pow_par0);
        }
    }
    /* NVRAM */
    oal_io_print("%s : { %d }\n", "en_nv_dp_init_is_null: ", g_en_nv_dp_init_is_null);
    for (l_cfg_idx_one = 0; l_cfg_idx_one < WLAN_CFG_DTS_NVRAM_END; l_cfg_idx_one++) {
        oal_io_print("%s para_idx[%d] name[%s]::DATA{%s}\n", "dp init & ratios nvram_param: ", l_cfg_idx_one,
                     g_wifi_nvram_cfg_handler[l_cfg_idx_one].puc_param_name,
                     hwifi_get_nvram_param(l_cfg_idx_one));
    }

    /* RU DELT POW  */
    hwifi_get_cfg_delt_ru_pow_params();

    return INI_SUCC;
}

EXPORT_SYMBOL_GPL(g_cust_country_code_ignore_flag);
EXPORT_SYMBOL_GPL(g_wlan_customize);
EXPORT_SYMBOL_GPL(g_pro_line_params);
EXPORT_SYMBOL_GPL(g_gs_extre_point_vals);
EXPORT_SYMBOL_GPL(g_en_nv_dp_init_is_null);
EXPORT_SYMBOL_GPL(g_wlan_cust);
#ifdef _PRE_WLAN_FEATURE_TAS_ANT_SWITCH
EXPORT_SYMBOL_GPL(g_tas_switch_en);
#endif
EXPORT_SYMBOL_GPL(g_en_fact_cali_completed);
EXPORT_SYMBOL_GPL(g_wlan_cal_intvl_enable);
EXPORT_SYMBOL_GPL(g_wlan_cal_disable_switch);
EXPORT_SYMBOL_GPL(hwifi_config_init);
EXPORT_SYMBOL_GPL(hwifi_get_mac_addr);
EXPORT_SYMBOL_GPL(hwifi_get_init_value);
EXPORT_SYMBOL_GPL(hwifi_get_country_code);
EXPORT_SYMBOL_GPL(hwifi_get_nvram_params);
EXPORT_SYMBOL_GPL(hwifi_get_init_nvram_params);
EXPORT_SYMBOL_GPL(hwifi_set_country_code);
EXPORT_SYMBOL_GPL(hwifi_is_regdomain_changed);
EXPORT_SYMBOL_GPL(hwifi_get_cfg_params);
EXPORT_SYMBOL_GPL(hwifi_custom_host_read_cfg_init);
EXPORT_SYMBOL_GPL(hwifi_get_init_priv_value);
EXPORT_SYMBOL_GPL(hwifi_get_regdomain_from_country_code);
EXPORT_SYMBOL_GPL(hwifi_get_nvram_param);


#endif  // #if defined(_PRE_PRODUCT_ID_HI110X_HOST)
