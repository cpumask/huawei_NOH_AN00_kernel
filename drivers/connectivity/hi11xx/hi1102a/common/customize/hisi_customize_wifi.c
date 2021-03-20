

#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
/*  Header File Including */
#include "hisi_customize_wifi.h"

#include <linux/kernel.h>
#include <linux/time.h>

#include "hisi_ini.h"
#include "plat_type.h"
#include "oam_ext_if.h"
#include "oal_sdio_comm.h"

/* �ն�ͷ�ļ� */
#include <linux/mtd/hisi_nve_interface.h>
#include <linux/etherdevice.h>
#include "plat_firmware.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_CUSTOMIZE

/* Global Variable Definition */
int32 al_host_init_params[WLAN_CFG_INIT_BUTT] = {0}; /* ini���ƻ��������� */
int32 al_dts_params[WLAN_CFG_DTS_BUTT] = {0};        /* dts���ƻ��������� */
uint8 auc_nv_params[NUM_OF_NV_MAX_TXPOWER] = {0};    /* nv���ƻ��������� */
int32 al_nvram_init_params[NVRAM_PARAMS_INDEX_BUTT] = {0}; /* ini�ļ���NV�������� */

oal_uint32 al_priv_cust_init_params[WLAN_CFG_PRIV_BUTT] = {
    0x1fff,         // cali_mask
    0x10,           // cali_data_mask
    1,              // temp_pro_en
    1,              // temp_pro_reduce_pwr_en
    90,             // temp_pro_safe_th
    100,            // temp_pro_over_th
    110,            // temp_pro_pa_off_th
    -130,           // dsss2ofdm_dbb_pwr_bo_val
    10,             // fast_ps_check_cnt
    6,              // voe_switch_mask
    0,              // dyn_bypass_extlna
    1,              // hcc_flowctrl_type
    0x2             // ext_fem_5g_type
};

int8 ac_country_code[COUNTRY_CODE_LEN] = "00";
uint8 auc_wifimac[MAC_LEN] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
wlan_cus_pwr_fit_para_stru as_pro_line_params[DY_CALI_PARAMS_NUM] = {{0}};      /* ���ⶨ�ƻ��������� */
uint8 auc_cust_nvram_info[WLAN_CFG_DTS_NVRAM_END][DY_CALI_PARAMS_LEN] = {{0}};  /* NVRAM���� */
oal_bool_enum_uint8 en_nv_dp_init_is_null = OAL_TRUE;                               /* NVRAM��dp init�ÿձ�־ */
wlan_customize_private_stru al_priv_cust_params[WLAN_CFG_PRIV_BUTT] = {{ 0, 0 }}; /* ˽�ж��ƻ��������� */
oal_int16 gs_extre_point_vals[DY_CALI_NUM_5G_BAND] = {0};

wlan_customize_power_params_stru cust_nv_params = {{0}}; /* ����͹��ʶ��ƻ����� */
wlan_nrcoex_ini_stru g_st_nrcoex_ini = {0};
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
oal_uint8 auc_sar_params[CUS_NUM_OF_SAR_LVL][CUS_SAR_NUM];
#endif
oal_bool_enum_uint8 g_aen_tas_switch_en[WITP_RF_CHANNEL_NUMS] = {OAL_FALSE};

oal_bool_enum_uint8 en_fact_cali_completed = OAL_FALSE;                           /* �Ƿ��в���У׼ */
oal_bool_enum_uint8 hwifi_get_fact_cali_completed(oal_void)
{
    return en_fact_cali_completed;
}
oal_void hwifi_set_fact_cali_completed(oal_bool_enum_uint8 fact_cali_completed)
{
    en_fact_cali_completed = fact_cali_completed;
}
wlan_cust_country_code_ingore_flag_stru g_st_cust_country_code_ignore_flag = {0}; /* ���ƻ����������� */

/*
 * ���ƻ��ṹ��
 * default values as follows:
 * ampdu_tx_max_num:            WLAN_AMPDU_TX_MAX_NUM               = 64
 * switch:                      ON                                  = 1
 * scan_band:                   ROAM_BAND_2G_BIT|ROAM_BAND_5G_BIT   = 3
 * scan_orthogonal:             ROAM_SCAN_CHANNEL_ORG_BUTT          = 4
 */
wlan_customize_stru wlan_customize = {
    64,                   /* addba_buffer_size */
    1,                    /* roam switch */
    3,                    /* roam scan band */
    4,                    /* roam scan org */
    -70,                  /* roam trigger 2G */
    -70,                  /* roam trigger 5G */
    10,                   /* roam delta 2G */
    10,                   /* roam delta 5G */
    0,                    /* random mac addr scan */
    0,                    /* disable_capab_2ght40 */
    0,                    /* lte_gpio_check_switch */
    0,                    /* lte_ism_priority */
    0,                    /* lte_rx_act */
    0,                    /* lte_tx_act */
};

/*
 *  regdomain <-> country code map table
 *  max support country num: MAX_COUNTRY_COUNT
 */
OAL_STATIC countryinfo_stru country_info_table[] = {
    { REGDOMAIN_COMMON, { '0', '0' }},  // WORLD DOMAIN
    { REGDOMAIN_FCC,    { 'A', 'D' }},  // ANDORRA
    { REGDOMAIN_ETSI,   { 'A', 'E' }},  // UAE
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
    { REGDOMAIN_ETSI, { 'B', 'L' }},
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
    { REGDOMAIN_ETSI, { 'C', 'U' }},    // CUBA
    { REGDOMAIN_ETSI, { 'C', 'Y' }},    // CYPRUS
    { REGDOMAIN_ETSI, { 'C', 'Z' }},    // CZECH REPUBLIC
    { REGDOMAIN_ETSI, { 'D', 'E' }},    // GERMANY
    { REGDOMAIN_ETSI, { 'D', 'K' }},    // DENMARK
    { REGDOMAIN_FCC, { 'D', 'O' }},     // DOMINICAN REPUBLIC
    { REGDOMAIN_ETSI, { 'D', 'Z' }},    // ALGERIA
    { REGDOMAIN_FCC, { 'E', 'C' }},     // ECUADOR
    { REGDOMAIN_ETSI, { 'E', 'E' }},    // ESTONIA
    { REGDOMAIN_ETSI, { 'E', 'G' }},    // EGYPT
    { REGDOMAIN_ETSI, { 'E', 'S' }},    // SPAIN
    { REGDOMAIN_ETSI, { 'E', 'T' }},    // ETHIOPIA
    { REGDOMAIN_ETSI, { 'F', 'I' }},    // FINLAND
    { REGDOMAIN_ETSI, { 'F', 'R' }},    // FRANCE
    { REGDOMAIN_ETSI, { 'G', 'B' }},    // UNITED KINGDOM
    { REGDOMAIN_FCC, { 'G', 'D' }},     // GRENADA
    { REGDOMAIN_ETSI, { 'G', 'E' }},    // GEORGIA
    { REGDOMAIN_ETSI, { 'G', 'F' }},    // FRENCH GUIANA
    { REGDOMAIN_ETSI, { 'G', 'L' }},    // GREENLAND
    { REGDOMAIN_ETSI, { 'G', 'P' }},    // GUADELOUPE
    { REGDOMAIN_ETSI, { 'G', 'R' }},    // GREECE
    { REGDOMAIN_FCC, { 'G', 'T' }},     // GUATEMALA
    { REGDOMAIN_FCC, { 'G', 'U' }},     // GUAM
    { REGDOMAIN_ETSI, { 'H', 'K' }},    // HONGKONG
    { REGDOMAIN_FCC, { 'H', 'N' }},     // HONDURAS
    { REGDOMAIN_ETSI, { 'H', 'R' }},    // Croatia
    { REGDOMAIN_ETSI, { 'H', 'U' }},    // HUNGARY
    { REGDOMAIN_ETSI, { 'I', 'D' }},    // INDONESIA
    { REGDOMAIN_ETSI, { 'I', 'E' }},    // IRELAND
    { REGDOMAIN_ETSI, { 'I', 'L' }},    // ISRAEL
    { REGDOMAIN_ETSI, { 'I', 'N' }},    // INDIA
    { REGDOMAIN_ETSI, { 'I', 'Q' }},    // IRAQ
    { REGDOMAIN_ETSI, { 'I', 'R' }},    // IRAN, ISLAMIC REPUBLIC OF
    { REGDOMAIN_ETSI, { 'I', 'S' }},    // ICELNAD
    { REGDOMAIN_ETSI, { 'I', 'T' }},    // ITALY
    { REGDOMAIN_FCC, { 'J', 'M' }},     // JAMAICA
    { REGDOMAIN_ETSI,  { 'J', 'P' }},   // JAPAN
    { REGDOMAIN_ETSI, { 'J', 'O' }},    // JORDAN
    { REGDOMAIN_ETSI, { 'K', 'E' }},    // KENYA
    { REGDOMAIN_ETSI, { 'K', 'H' }},    // CAMBODIA
    { REGDOMAIN_ETSI, { 'K', 'P' }},    // KOREA, DEMOCRATIC PEOPLE's REPUBLIC OF
    { REGDOMAIN_ETSI, { 'K', 'R' }},    // KOREA, REPUBLIC OF
    { REGDOMAIN_ETSI, { 'K', 'W' }},    // KUWAIT
    { REGDOMAIN_ETSI, { 'K', 'Y' }},    // Cayman Is
    { REGDOMAIN_ETSI, { 'K', 'Z' }},    // KAZAKHSTAN
    { REGDOMAIN_ETSI, { 'L', 'B' }},    // LEBANON
    { REGDOMAIN_ETSI, { 'L', 'I' }},    // LIECHTENSTEIN
    { REGDOMAIN_ETSI, { 'L', 'K' }},    // SRI-LANKA
    { REGDOMAIN_ETSI, { 'L', 'S' }},    // KINGDOM OF LESOTH
    { REGDOMAIN_ETSI, { 'L', 'T' }},    // LITHUANIA
    { REGDOMAIN_ETSI, { 'L', 'U' }},    // LUXEMBOURG
    { REGDOMAIN_ETSI, { 'L', 'V' }},    // LATVIA
    { REGDOMAIN_ETSI, { 'M', 'A' }},    // MOROCCO
    { REGDOMAIN_ETSI, { 'M', 'C' }},    // MONACO
    { REGDOMAIN_ETSI, { 'M', 'D' }},    // REPUBLIC OF MOLDOVA
    { REGDOMAIN_ETSI, { 'M', 'E' }},    // Montenegro
    { REGDOMAIN_FCC, { 'M', 'H' }},     // Marshall Is
    { REGDOMAIN_ETSI, { 'M', 'K' }},    // MACEDONIA, THE FORMER YUGOSLAV REPUBLIC OF
    { REGDOMAIN_ETSI, { 'M', 'M' }},    // MYANMAR
    { REGDOMAIN_FCC, { 'M', 'N' }},     // MONGOLIA
    { REGDOMAIN_ETSI, { 'M', 'O' }},    // MACAO
    { REGDOMAIN_FCC, { 'M', 'P' }},     // NORTHERN MARIANA ISLANDS
    { REGDOMAIN_ETSI, { 'M', 'Q' }},    // MARTINIQUE
    { REGDOMAIN_ETSI, { 'M', 'R' }},    // Mauritania
    { REGDOMAIN_ETSI, { 'M', 'T' }},    // MALTA
    { REGDOMAIN_ETSI, { 'M', 'V' }},    // Maldives
    { REGDOMAIN_ETSI, { 'M', 'U' }},    // MAURITIUS
    { REGDOMAIN_ETSI, { 'M', 'W' }},    // MALAWI
    { REGDOMAIN_ETSI, { 'M', 'X' }},    // MEXICO
    { REGDOMAIN_ETSI, { 'M', 'Y' }},    // MALAYSIA
    { REGDOMAIN_ETSI, { 'N', 'G' }},    // NIGERIA
    { REGDOMAIN_FCC, { 'N', 'I' }},     // NICARAGUA
    { REGDOMAIN_ETSI, { 'N', 'L' }},    // NETHERLANDS
    { REGDOMAIN_ETSI, { 'N', 'O' }},    // NORWAY
    { REGDOMAIN_ETSI, { 'N', 'P' }},    // NEPAL
    { REGDOMAIN_ETSI, { 'N', 'Z' }},    // NEW-ZEALAND
    { REGDOMAIN_ETSI, { 'O', 'M' }},    // OMAN
    { REGDOMAIN_FCC, { 'P', 'A' }},     // PANAMA
    { REGDOMAIN_FCC, { 'P', 'E' }},     // PERU
    { REGDOMAIN_ETSI, { 'P', 'F' }},    // FRENCH POLYNESIA
    { REGDOMAIN_ETSI, { 'P', 'G' }},    // PAPUA NEW GUINEA
    { REGDOMAIN_ETSI, { 'P', 'H' }},    // PHILIPPINES
    { REGDOMAIN_ETSI, { 'P', 'K' }},    // PAKISTAN
    { REGDOMAIN_ETSI, { 'P', 'L' }},    // POLAND
    { REGDOMAIN_FCC, { 'P', 'R' }},     // PUERTO RICO
    { REGDOMAIN_FCC, { 'P', 'S' }},     // PALESTINIAN TERRITORY, OCCUPIED
    { REGDOMAIN_ETSI, { 'P', 'T' }},    // PORTUGAL
    { REGDOMAIN_FCC, { 'P', 'Y' }},     // PARAGUAY
    { REGDOMAIN_ETSI, { 'Q', 'A' }},    // QATAR
    { REGDOMAIN_ETSI, { 'R', 'E' }},    // REUNION
    { REGDOMAIN_ETSI, { 'R', 'O' }},    // ROMAINIA
    { REGDOMAIN_ETSI, { 'R', 'S' }},    // SERBIA
    { REGDOMAIN_ETSI, { 'R', 'U' }},    // RUSSIA
    { REGDOMAIN_FCC, { 'R', 'W' }},     // RWANDA
    { REGDOMAIN_ETSI, { 'S', 'A' }},    // SAUDI ARABIA
    { REGDOMAIN_ETSI, { 'S', 'D' }},    // SUDAN ,REPUBLIC OF THE
    { REGDOMAIN_ETSI, { 'S', 'E' }},    // SWEDEN
    { REGDOMAIN_ETSI, { 'S', 'G' }},    // SINGAPORE
    { REGDOMAIN_ETSI, { 'S', 'I' }},    // SLOVENNIA
    { REGDOMAIN_ETSI, { 'S', 'K' }},    // SLOVAKIA
    { REGDOMAIN_ETSI, { 'S', 'N' }},    // Senegal
    { REGDOMAIN_ETSI, { 'S', 'V' }},    // EL SALVADOR
    { REGDOMAIN_ETSI, { 'S', 'Y' }},    // SYRIAN ARAB REPUBLIC
    { REGDOMAIN_ETSI, { 'T', 'H' }},    // THAILAND
    { REGDOMAIN_ETSI, { 'T', 'N' }},    // TUNISIA
    { REGDOMAIN_ETSI, { 'T', 'R' }},    // TURKEY
    { REGDOMAIN_ETSI, { 'T', 'T' }},    // TRINIDAD AND TOBAGO
    { REGDOMAIN_FCC, { 'T', 'W' }},     // TAIWAN, PRIVINCE OF CHINA
    { REGDOMAIN_FCC, { 'T', 'Z' }},     // TANZANIA, UNITED REPUBLIC OF
    { REGDOMAIN_ETSI, { 'U', 'A' }},    // UKRAINE
    { REGDOMAIN_ETSI, { 'U', 'G' }},    // UGANDA
    { REGDOMAIN_FCC, { 'U', 'S' }},     // USA
    { REGDOMAIN_FCC, { 'U', 'Y' }},     // URUGUAY
    { REGDOMAIN_ETSI, { 'U', 'Z' }},    // UZBEKISTAN
    { REGDOMAIN_FCC, { 'V', 'E' }},     // VENEZUELA
    { REGDOMAIN_FCC, { 'V', 'I' }},     // VIRGIN ISLANDS, US
    { REGDOMAIN_ETSI, { 'V', 'N' }},    // VIETNAM
    { REGDOMAIN_ETSI, { 'Y', 'E' }},    // YEMEN
    { REGDOMAIN_ETSI, { 'Y', 'T' }},    // MAYOTTE
    { REGDOMAIN_ETSI, { 'Z', 'A' }},    // SOUTH AFRICA
    { REGDOMAIN_ETSI, { 'Z', 'M' }},    // Zambia
    { REGDOMAIN_ETSI, { 'Z', 'W' }},    // ZIMBABWE
    { REGDOMAIN_FCC,  { 'Z', 'Z' }},    // country code without mobile

    { REGDOMAIN_COUNT, { '9', '9' }},
};

/*
 * regdomain <-> plat_tag mapping table
 */
OAL_STATIC regdomain_plat_tag_map_stru plat_tag_mapping_table[] = {
    { REGDOMAIN_FCC,    INI_MODU_POWER_FCC },  // FCC
    { REGDOMAIN_ETSI,   INI_MODU_WIFI },      // ETSI
    { REGDOMAIN_JAPAN,  INI_MODU_WIFI },     // JP
    { REGDOMAIN_COMMON, INI_MODU_WIFI },    // COMMON

    { REGDOMAIN_COUNT, INI_MODU_INVALID }
};

OAL_STATIC wlan_cfg_cmd wifi_config_dts[] = {
    /* 5g���� */
    { "band_5g_enable", WLAN_CFG_DTS_BAND_5G_ENABLE },
    /* ����У׼������ֵ */
    { "cali_tone_amp_grade",    WLAN_CFG_DTS_CALI_TONE_AMP_GRADE },

    /* bt У׼ */
    { "cali_txpwr_dpn_band1",   WLAN_CFG_DTS_BT_CALI_TXPWR_DPN_BAND1 },
    { "cali_txpwr_dpn_band2",   WLAN_CFG_DTS_BT_CALI_TXPWR_DPN_BAND2 },
    { "cali_txpwr_dpn_band3",   WLAN_CFG_DTS_BT_CALI_TXPWR_DPN_BAND3 },
    { "cali_txpwr_dpn_band4",   WLAN_CFG_DTS_BT_CALI_TXPWR_DPN_BAND4 },
    { "cali_txpwr_dpn_band5",   WLAN_CFG_DTS_BT_CALI_TXPWR_DPN_BAND5 },
    { "cali_txpwr_dpn_band6",   WLAN_CFG_DTS_BT_CALI_TXPWR_DPN_BAND6 },
    { "cali_txpwr_dpn_band7",   WLAN_CFG_DTS_BT_CALI_TXPWR_DPN_BAND7 },
    { "cali_txpwr_dpn_band8",   WLAN_CFG_DTS_BT_CALI_TXPWR_DPN_BAND8 },
    { "cali_txpwr_num",         WLAN_CFG_DTS_BT_CALI_TXPWR_NUM },
    { "cali_txpwr_freq1",       WLAN_CFG_DTS_BT_CALI_TXPWR_FREQ1 },
    { "cali_txpwr_freq2",       WLAN_CFG_DTS_BT_CALI_TXPWR_FREQ2 },
    { "cali_txpwr_freq3",       WLAN_CFG_DTS_BT_CALI_TXPWR_FREQ3 },
    { "cali_txpwr_freq4",       WLAN_CFG_DTS_BT_CALI_TXPWR_FREQ4 },
    { "cali_txpwr_freq5",       WLAN_CFG_DTS_BT_CALI_TXPWR_FREQ5 },
    { "cali_txpwr_freq6",       WLAN_CFG_DTS_BT_CALI_TXPWR_FREQ6 },
    { "cali_txpwr_freq7",       WLAN_CFG_DTS_BT_CALI_TXPWR_FREQ7 },
    { "cali_txpwr_freq8",       WLAN_CFG_DTS_BT_CALI_TXPWR_FREQ8 },
    { "cali_bt_insertion_loss", WLAN_CFG_DTS_BT_CALI_INSERTION_LOSS },
    { "cali_bt_gm_cali_en",     WLAN_CFG_DTS_BT_CALI_GM_CALI_EN },
    { "cali_bt_gm0_db10",       WLAN_CFG_DTS_BT_CALI_GM0_DB10 },
    { "cali_bt_base_power",     WLAN_CFG_DTS_BT_CALI_BASE_POWER },
    { "cali_bt_is_dpn_calc",    WLAN_CFG_DTS_BT_CALI_IS_DPN_CALC },
#ifdef _PRE_WLAN_FIT_BASED_REALTIME_CALI
    /* ��̬У׼ */
    { "dyn_cali_dscr_interval", WLAN_CFG_DTS_DYN_CALI_DSCR_ITERVL },
    /* DPN 40M 20M 11b */
    { "dpn24g_ch1_core0",    WLAN_CFG_DTS_2G_CORE0_DPN_CH1 },
    { "dpn24g_ch2_core0",    WLAN_CFG_DTS_2G_CORE0_DPN_CH2 },
    { "dpn24g_ch3_core0",    WLAN_CFG_DTS_2G_CORE0_DPN_CH3 },
    { "dpn24g_ch4_core0",    WLAN_CFG_DTS_2G_CORE0_DPN_CH4 },
    { "dpn24g_ch5_core0",    WLAN_CFG_DTS_2G_CORE0_DPN_CH5 },
    { "dpn24g_ch6_core0",    WLAN_CFG_DTS_2G_CORE0_DPN_CH6 },
    { "dpn24g_ch7_core0",    WLAN_CFG_DTS_2G_CORE0_DPN_CH7 },
    { "dpn24g_ch8_core0",    WLAN_CFG_DTS_2G_CORE0_DPN_CH8 },
    { "dpn24g_ch9_core0",    WLAN_CFG_DTS_2G_CORE0_DPN_CH9 },
    { "dpn24g_ch10_core0",   WLAN_CFG_DTS_2G_CORE0_DPN_CH10 },
    { "dpn24g_ch11_core0",   WLAN_CFG_DTS_2G_CORE0_DPN_CH11 },
    { "dpn24g_ch12_core0",   WLAN_CFG_DTS_2G_CORE0_DPN_CH12 },
    { "dpn24g_ch13_core0",   WLAN_CFG_DTS_2G_CORE0_DPN_CH13 },
    { "dpn5g_core0_b0",      WLAN_CFG_DTS_5G_CORE0_DPN_B0 },
    { "dpn5g_core0_b1",      WLAN_CFG_DTS_5G_CORE0_DPN_B1 },
    { "dpn5g_core0_b2",      WLAN_CFG_DTS_5G_CORE0_DPN_B2 },
    { "dpn5g_core0_b3",      WLAN_CFG_DTS_5G_CORE0_DPN_B3 },
    { "dpn5g_core0_b4",      WLAN_CFG_DTS_5G_CORE0_DPN_B4 },
    { "dyn_cali_opt_switch", WLAN_CFG_DTS_DYN_CALI_OPT_SWITCH },
    { "gm0_dB10_amend",      WLAN_CFG_DTS_DYN_CALI_GM0_DB10_AMEND },
#endif
    { "5g_upc_up_limit",     WLAN_CFG_DTS_5G_UPC_UP_LIMIT },
    { "5g_iq_backoff_power", WLAN_CFG_DTS_5G_IQ_BACKOFF_POWER },
    { "5g_iq_cali_power",    WLAN_CFG_DTS_5G_IQ_CALI_POWER },

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
    { "cali_txpwr_pa_dc_ref_5g_val_band1",  WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_5G_VAL_BAND1 },
    { "cali_txpwr_pa_dc_ref_5g_val_band2",  WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_5G_VAL_BAND2 },
    { "cali_txpwr_pa_dc_ref_5g_val_band3",  WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_5G_VAL_BAND3 },
    { "cali_txpwr_pa_dc_ref_5g_val_band4",  WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_5G_VAL_BAND4 },
    { "cali_txpwr_pa_dc_ref_5g_val_band5",  WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_5G_VAL_BAND5 },
    { "cali_txpwr_pa_dc_ref_5g_val_band6",  WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_5G_VAL_BAND6 },
    { "cali_txpwr_pa_dc_ref_5g_val_band7",  WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_5G_VAL_BAND7 },

    /* RF REGISTER */
    { "rf_reg117", WLAN_CFG_DTS_RF_REG117 },
    { "rf_reg123", WLAN_CFG_DTS_RF_REG123 },
    { "rf_reg124", WLAN_CFG_DTS_RF_REG124 },
    { "rf_reg125", WLAN_CFG_DTS_RF_REG125 },
    { "rf_reg126", WLAN_CFG_DTS_RF_REG126 },
    { OAL_PTR_NULL, 0 }
};

OAL_STATIC wlan_cfg_cmd wifi_config_cmds[] = {
    /* ROAM */
    { "roam_switch",     WLAN_CFG_INIT_ROAM_SWITCH },
    { "scan_orthogonal", WLAN_CFG_INIT_SCAN_ORTHOGONAL },
    { "trigger_b",       WLAN_CFG_INIT_TRIGGER_B },
    { "trigger_a",       WLAN_CFG_INIT_TRIGGER_A },
    { "delta_b",         WLAN_CFG_INIT_DELTA_B },
    { "delta_a",         WLAN_CFG_INIT_DELTA_A },

    /* ���� */
    { "ampdu_tx_max_num",        WLAN_CFG_INIT_AMPDU_TX_MAX_NUM },
    { "used_mem_for_start",      WLAN_CFG_INIT_USED_MEM_FOR_START },
    { "used_mem_for_stop",       WLAN_CFG_INIT_USED_MEM_FOR_STOP },
    { "rx_ack_limit",            WLAN_CFG_INIT_RX_ACK_LIMIT },
    { "sdio_d2h_assemble_count", WLAN_CFG_INIT_SDIO_D2H_ASSEMBLE_COUNT },
    { "sdio_h2d_assemble_count", WLAN_CFG_INIT_SDIO_H2D_ASSEMBLE_COUNT },
    /* LINKLOSS */
    { "link_loss_threshold_wlan_bt",     WLAN_CFG_INIT_LINK_LOSS_THRESHOLD_WLAN_BT },
    { "link_loss_threshold_wlan_dbac",   WLAN_CFG_INIT_LINK_LOSS_THRESHOLD_WLAN_DBAC },
    { "link_loss_threshold_wlan_normal", WLAN_CFG_INIT_LINK_LOSS_THRESHOLD_WLAN_NORMAL },
    /* �Զ���Ƶ */
    { "pps_threshold_level_0", WLAN_CFG_INIT_PPS_THRESHOLD_LEVEL_0 },
    { "pps_threshold_level_1", WLAN_CFG_INIT_PPS_THRESHOLD_LEVEL_1 },
    { "pps_threshold_level_2", WLAN_CFG_INIT_PPS_THRESHOLD_LEVEL_2 },
    { "pps_threshold_level_3", WLAN_CFG_INIT_PPS_THRESHOLD_LEVEL_3 },
    { "device_type_level_0",   WLAN_CFG_INIT_DEVICE_TYPE_LEVEL_0 },
    { "device_type_level_1",   WLAN_CFG_INIT_DEVICE_TYPE_LEVEL_1 },
    { "device_type_level_2",   WLAN_CFG_INIT_DEVICE_TYPE_LEVEL_2 },
    { "device_type_level_3",   WLAN_CFG_INIT_DEVICE_TYPE_LEVEL_3 },
    /* �͹��� */
    { "powermgmt_switch", WLAN_CFG_INIT_POWERMGMT_SWITCH },
    { "ps_mode", WLAN_CFG_INIT_PS_MODE },

    /* ��ά�ɲ� */
    { "loglevel", WLAN_CFG_INIT_LOGLEVEL },
    /* 2G RFǰ�� */
    { "rf_line_txrx_gain_db_2g_band1_mult4",  WLAN_CFG_INIT_RF_LINE_TXRX_GAIN_DB_2G_BAND1_MULT4 },
    { "rf_line_txrx_gain_db_2g_band1_mult10", WLAN_CFG_INIT_RF_LINE_TXRX_GAIN_DB_2G_BAND1_MULT10 },
    { "rf_line_txrx_gain_db_2g_band2_mult4",  WLAN_CFG_INIT_RF_LINE_TXRX_GAIN_DB_2G_BAND2_MULT4 },
    { "rf_line_txrx_gain_db_2g_band2_mult10", WLAN_CFG_INIT_RF_LINE_TXRX_GAIN_DB_2G_BAND2_MULT10 },
    { "rf_line_txrx_gain_db_2g_band3_mult4",  WLAN_CFG_INIT_RF_LINE_TXRX_GAIN_DB_2G_BAND3_MULT4 },
    { "rf_line_txrx_gain_db_2g_band3_mult10", WLAN_CFG_INIT_RF_LINE_TXRX_GAIN_DB_2G_BAND3_MULT10 },
    /* 5G RFǰ�� */
    { "rf_line_txrx_gain_db_5g_band1_mult4",  WLAN_CFG_INIT_RF_LINE_TXRX_GAIN_DB_5G_BAND1_MULT4 },
    { "rf_line_txrx_gain_db_5g_band1_mult10", WLAN_CFG_INIT_RF_LINE_TXRX_GAIN_DB_5G_BAND1_MULT10 },
    { "rf_line_txrx_gain_db_5g_band2_mult4",  WLAN_CFG_INIT_RF_LINE_TXRX_GAIN_DB_5G_BAND2_MULT4 },
    { "rf_line_txrx_gain_db_5g_band2_mult10", WLAN_CFG_INIT_RF_LINE_TXRX_GAIN_DB_5G_BAND2_MULT10 },
    { "rf_line_txrx_gain_db_5g_band3_mult4",  WLAN_CFG_INIT_RF_LINE_TXRX_GAIN_DB_5G_BAND3_MULT4 },
    { "rf_line_txrx_gain_db_5g_band3_mult10", WLAN_CFG_INIT_RF_LINE_TXRX_GAIN_DB_5G_BAND3_MULT10 },
    { "rf_line_txrx_gain_db_5g_band4_mult4",  WLAN_CFG_INIT_RF_LINE_TXRX_GAIN_DB_5G_BAND4_MULT4 },
    { "rf_line_txrx_gain_db_5g_band4_mult10", WLAN_CFG_INIT_RF_LINE_TXRX_GAIN_DB_5G_BAND4_MULT10 },
    { "rf_line_txrx_gain_db_5g_band5_mult4",  WLAN_CFG_INIT_RF_LINE_TXRX_GAIN_DB_5G_BAND5_MULT4 },
    { "rf_line_txrx_gain_db_5g_band5_mult10", WLAN_CFG_INIT_RF_LINE_TXRX_GAIN_DB_5G_BAND5_MULT10 },
    { "rf_line_txrx_gain_db_5g_band6_mult4",  WLAN_CFG_INIT_RF_LINE_TXRX_GAIN_DB_5G_BAND6_MULT4 },
    { "rf_line_txrx_gain_db_5g_band6_mult10", WLAN_CFG_INIT_RF_LINE_TXRX_GAIN_DB_5G_BAND6_MULT10 },
    { "rf_line_txrx_gain_db_5g_band7_mult4",  WLAN_CFG_INIT_RF_LINE_TXRX_GAIN_DB_5G_BAND7_MULT4 },
    { "rf_line_txrx_gain_db_5g_band7_mult10", WLAN_CFG_INIT_RF_LINE_TXRX_GAIN_DB_5G_BAND7_MULT10 },
    { "rf_line_rx_gain_db_5g",                WLAN_CFG_INIT_RF_LINE_RX_GAIN_DB_5G },
    { "lna_gain_db_5g",                       WLAN_CFG_INIT_LNA_GAIN_DB_5G },
    { "rf_line_tx_gain_db_5g",                WLAN_CFG_INIT_RF_LINE_TX_GAIN_DB_5G },
    { "ext_switch_isexist_5g",                WLAN_CFG_INIT_EXT_SWITCH_ISEXIST_5G },
    { "ext_pa_isexist_5g",                    WLAN_CFG_INIT_EXT_PA_ISEXIST_5G },
    { "ext_lna_isexist_5g",                   WLAN_CFG_INIT_EXT_LNA_ISEXIST_5G },
    { "lna_on2off_time_ns_5g",                WLAN_CFG_INIT_LNA_ON2OFF_TIME_NS_5G },
    { "lna_off2on_time_ns_5g",                WLAN_CFG_INIT_LNA_OFF2ON_TIME_NS_5G },
    /* SCAN */
    { "random_mac_addr_scan", WLAN_CFG_INIT_RANDOM_MAC_ADDR_SCAN },
    /* 11AC2G */
    { "11ac2g_enable",                      WLAN_CFG_INIT_11AC2G_ENABLE },
    { "disable_capab_2ght40",               WLAN_CFG_INIT_DISABLE_CAPAB_2GHT40 },
    { "dual_antenna_enable",                WLAN_CFG_INIT_DUAL_ANTENNA_ENABLE }, /* ˫���߿��� */
    { "far_dist_pow_gain_switch",           WLAN_CFG_INIT_FAR_DIST_POW_GAIN_SWITCH },
    { "lte_gpio_check_switch",              WLAN_CFG_LTE_GPIO_CHECK_SWITCH }, /* lte�ܽż�⿪�� */
    { "lte_ism_priority",                   WLAN_ATCMDSRV_LTE_ISM_PRIORITY },
    { "lte_rx_act",                         WLAN_ATCMDSRV_LTE_RX_ACT },
    { "lte_tx_act",                         WLAN_ATCMDSRV_LTE_TX_ACT },
    { "far_dist_dsss_scale_promote_switch", WLAN_CFG_INIT_FAR_DIST_DSSS_SCALE_PROMOTE_SWITCH },
    { "delta_cca_ed_high_20th_2g",          WLAN_CFG_INIT_DELTA_CCA_ED_HIGH_20TH_2G },
    { "delta_cca_ed_high_40th_2g",          WLAN_CFG_INIT_DELTA_CCA_ED_HIGH_40TH_2G },
    { "delta_cca_ed_high_20th_5g",          WLAN_CFG_INIT_DELTA_CCA_ED_HIGH_20TH_5G },
    { "delta_cca_ed_high_40th_5g",          WLAN_CFG_INIT_DELTA_CCA_ED_HIGH_40TH_5G },
#ifdef _PRE_WLAN_DOWNLOAD_PM
    { "download_rate_limit_pps", WLAN_CFG_INIT_DOWNLOAD_RATE_LIMIT_PPS },
#endif
#ifdef _PRE_WLAN_FEATURE_MULTI_NETBUF_AMSDU
    { "tx_amsdu_ampdu",      WLAN_CFG_INIT_AMPDU_AMSDU_SKB },
    { "tx_amsdu_ampdu_th_l", WLAN_CFG_INIT_AMSDU_AMPDU_TH_LOW },
    { "tx_amsdu_ampdu_th_h", WLAN_CFG_INIT_AMSDU_AMPDU_TH_HIGH },
    { "rx_ampdu_amsdu",      WLAN_CFG_INIT_RX_AMPDU_AMSDU_SKB },
#endif
#ifdef _PRE_WLAN_TCP_OPT
    { "en_tcp_ack_filter",      WLAN_CFG_INIT_TCP_ACK_FILTER },
    { "rx_tcp_ack_filter_th_l", WLAN_CFG_INIT_TCP_ACK_FILTER_TH_LOW },
    { "rx_tcp_ack_filter_th_h", WLAN_CFG_INIT_TCP_ACK_FILTER_TH_HIGH },
#endif
    {"en_device_rx_tcp_ack_filter",     WLAN_CFG_INIT_DEVICE_RX_TCP_ACK_FILTER},
    {"device_rx_tcp_ack_2G_th",         WLAN_CFG_INIT_DEVICE_RX_TCP_ACK_2G_TH},
    {"device_rx_tcp_ack_5G_th",         WLAN_CFG_INIT_DEVICE_RX_TCP_ACK_5G_TH},

    { "thread_bindcpu",      WLAN_CFG_INIT_BINDCPU },
    { "thread_bindcpu_mask", WLAN_CFG_INIT_BINDCPU_MASK },
    { "bindcpu_th_pps_high",  WLAN_CFG_INIT_BINDCPU_PPSHIGH },
    { "bindcpu_th_pps_low",   WLAN_CFG_INIT_BINDCPU_PPSLOW },

    { "txopt_switch_sta", WLAN_CFG_INIT_TX_OPT_SWITCH_STA},
    { "txopt_switch_ap", WLAN_CFG_INIT_TX_OPT_SWITCH_AP},
    { "txopt_pps_high", WLAN_CFG_INIT_TX_OPT_PPS_HIGH},
    { "txopt_pps_low", WLAN_CFG_INIT_TX_OPT_PPS_LOW},
    { "txopt_cwmin", WLAN_CFG_INIT_TX_OPT_CWMIN},
    { "txopt_cwmax", WLAN_CFG_INIT_TX_OPT_CWMAX},
    { "txopt_txoplimit", WLAN_CFG_INIT_TX_OPT_TXOPLIMIT},
    { "txopt_dyn_txoplimit", WLAN_CFG_INIT_TX_OPT_DYN_TXOPLIMIT},

    { "tcp_ack_buf_switch",   WLAN_CFG_INIT_TX_TCP_ACK_BUF },
    { "tcp_ack_buf_th_h",     WLAN_CFG_INIT_TCP_ACK_BUF_HIGH },
    { "tcp_ack_buf_th_l",     WLAN_CFG_INIT_TCP_ACK_BUF_LOW },
    { "tcp_ack_buf_th_h_40M", WLAN_CFG_INIT_TCP_ACK_BUF_HIGH_40M },
    { "tcp_ack_buf_th_l_40M", WLAN_CFG_INIT_TCP_ACK_BUF_LOW_40M },
    { "tcp_ack_buf_th_h_80M", WLAN_CFG_INIT_TCP_ACK_BUF_HIGH_80M },
    { "tcp_ack_buf_th_l_80M", WLAN_CFG_INIT_TCP_ACK_BUF_LOW_80M },

    /* DYN_BYPASS_EXTLNA */
    { "dyn_bypss_extlna_th_switch", WLAN_CFG_INIT_RX_DYN_BYPASS_EXTLNA },
    { "dyn_bypss_extlna_th_h",      WLAN_CFG_INIT_RX_DYN_BYPASS_EXTLNA_HIGH },
    { "dyn_bypss_extlna_th_l",      WLAN_CFG_INIT_RX_DYN_BYPASS_EXTLNA_LOW },

    /* SMALL_AMSDU */
    { "small_amsdu_switch",   WLAN_CFG_INIT_TX_SMALL_AMSDU },
    { "small_amsdu_th_h",     WLAN_CFG_INIT_SMALL_AMSDU_HIGH },
    { "small_amsdu_th_l",     WLAN_CFG_INIT_SMALL_AMSDU_LOW },
    { "small_amsdu_pps_th_h", WLAN_CFG_INIT_SMALL_AMSDU_PPS_HIGH },
    { "small_amsdu_pps_th_l", WLAN_CFG_INIT_SMALL_AMSDU_PPS_LOW },

    { "delta_pwr_ref_2g_20m",           WLAN_CFG_INIT_DELTA_PWR_REF_2G_20M },
    { "delta_pwr_ref_2g_40m",           WLAN_CFG_INIT_DELTA_PWR_REF_2G_40M },
    { "delta_pwr_ref_5g_20m",           WLAN_CFG_INIT_DELTA_PWR_REF_5G_20M },
    { "delta_pwr_ref_5g_40m",           WLAN_CFG_INIT_DELTA_PWR_REF_5G_40M },
    { "delta_pwr_ref_5g_80m",           WLAN_CFG_INIT_DELTA_PWR_REF_5G_80M },

    /* ��Ƶ�������� */
    { "nbfh_tbtt_offset",    WLAN_CFG_INIT_NBFH_TBTT_OFFSET },
    { "nbfh_tbtt_sync_time", WLAN_CFG_INIT_NBFH_TBTT_SYNC_TIME },
    { "nbfh_dwell_time",     WLAN_CFG_INIT_NBFH_DWELL_TIME },
    { "nbfh_beacon_time",    WLAN_CFG_INIT_NBFH_BEACON_TIME },

    /* 11U */
    { "passpoint_switch",    WLAN_CFG_INIT_INTERWORKING_SWITCH },
    /* connect */
    { "random_mac_addr_connect", WLAN_CFG_INIT_RANDOM_MAC_ADDR_CONNECT },
    /* TAS SWITCH */
    { "tas_ant_switch_en", WLAN_CFG_INIT_TAS_ANT_SWITCH},
#ifdef _PRE_WLAN_FEATURE_NRCOEX
    {"nrcoex_enable",                   WLAN_CFG_INIT_NRCOEX_ENABLE},
    {"nrcoex_rule0_freq",               WLAN_CFG_INIT_NRCOEX_RULE0_FREQ},
    {"nrcoex_rule0_40m_20m_gap0",       WLAN_CFG_INIT_NRCOEX_RULE0_40M_20M_GAP0},
    {"nrcoex_rule0_160m_80m_gap0",      WLAN_CFG_INIT_NRCOEX_RULE0_160M_80M_GAP0},
    {"nrcoex_rule0_40m_20m_gap1",       WLAN_CFG_INIT_NRCOEX_RULE0_40M_20M_GAP1},
    {"nrcoex_rule0_160m_80m_gap1",      WLAN_CFG_INIT_NRCOEX_RULE0_160M_80M_GAP1},
    {"nrcoex_rule0_40m_20m_gap2",       WLAN_CFG_INIT_NRCOEX_RULE0_40M_20M_GAP2},
    {"nrcoex_rule0_160m_80m_gap2",      WLAN_CFG_INIT_NRCOEX_RULE0_160M_80M_GAP2},
    {"nrcoex_rule0_smallgap0_act",      WLAN_CFG_INIT_NRCOEX_RULE0_GAP0_ACT},
    {"nrcoex_rule0_gap01_act",          WLAN_CFG_INIT_NRCOEX_RULE0_GAP1_ACT},
    {"nrcoex_rule0_gap12_act",          WLAN_CFG_INIT_NRCOEX_RULE0_GAP2_ACT},
    {"nrcoex_rule0_rxslot_rssi",        WLAN_CFG_INIT_NRCOEX_RULE0_RXSLOT_RSSI},
    {"nrcoex_rule1_freq",               WLAN_CFG_INIT_NRCOEX_RULE1_FREQ},
    {"nrcoex_rule1_40m_20m_gap0",       WLAN_CFG_INIT_NRCOEX_RULE1_40M_20M_GAP0},
    {"nrcoex_rule1_160m_80m_gap0",      WLAN_CFG_INIT_NRCOEX_RULE1_160M_80M_GAP0},
    {"nrcoex_rule1_40m_20m_gap1",       WLAN_CFG_INIT_NRCOEX_RULE1_40M_20M_GAP1},
    {"nrcoex_rule1_160m_80m_gap1",      WLAN_CFG_INIT_NRCOEX_RULE1_160M_80M_GAP1},
    {"nrcoex_rule1_40m_20m_gap2",       WLAN_CFG_INIT_NRCOEX_RULE1_40M_20M_GAP2},
    {"nrcoex_rule1_160m_80m_gap2",      WLAN_CFG_INIT_NRCOEX_RULE1_160M_80M_GAP2},
    {"nrcoex_rule1_smallgap0_act",      WLAN_CFG_INIT_NRCOEX_RULE1_GAP0_ACT},
    {"nrcoex_rule1_gap01_act",          WLAN_CFG_INIT_NRCOEX_RULE1_GAP1_ACT},
    {"nrcoex_rule1_gap12_act",          WLAN_CFG_INIT_NRCOEX_RULE1_GAP2_ACT},
    {"nrcoex_rule1_rxslot_rssi",        WLAN_CFG_INIT_NRCOEX_RULE1_RXSLOT_RSSI},
    {"nrcoex_rule2_freq",               WLAN_CFG_INIT_NRCOEX_RULE2_FREQ},
    {"nrcoex_rule2_40m_20m_gap0",       WLAN_CFG_INIT_NRCOEX_RULE2_40M_20M_GAP0},
    {"nrcoex_rule2_160m_80m_gap0",      WLAN_CFG_INIT_NRCOEX_RULE2_160M_80M_GAP0},
    {"nrcoex_rule2_40m_20m_gap1",       WLAN_CFG_INIT_NRCOEX_RULE2_40M_20M_GAP1},
    {"nrcoex_rule2_160m_80m_gap1",      WLAN_CFG_INIT_NRCOEX_RULE2_160M_80M_GAP1},
    {"nrcoex_rule2_40m_20m_gap2",       WLAN_CFG_INIT_NRCOEX_RULE2_40M_20M_GAP2},
    {"nrcoex_rule2_160m_80m_gap2",      WLAN_CFG_INIT_NRCOEX_RULE2_160M_80M_GAP2},
    {"nrcoex_rule2_smallgap0_act",      WLAN_CFG_INIT_NRCOEX_RULE2_GAP0_ACT},
    {"nrcoex_rule2_gap01_act",          WLAN_CFG_INIT_NRCOEX_RULE2_GAP1_ACT},
    {"nrcoex_rule2_gap12_act",          WLAN_CFG_INIT_NRCOEX_RULE2_GAP2_ACT},
    {"nrcoex_rule2_rxslot_rssi",        WLAN_CFG_INIT_NRCOEX_RULE2_RXSLOT_RSSI},
    {"nrcoex_rule3_freq",               WLAN_CFG_INIT_NRCOEX_RULE3_FREQ},
    {"nrcoex_rule3_40m_20m_gap0",       WLAN_CFG_INIT_NRCOEX_RULE3_40M_20M_GAP0},
    {"nrcoex_rule3_160m_80m_gap0",      WLAN_CFG_INIT_NRCOEX_RULE3_160M_80M_GAP0},
    {"nrcoex_rule3_40m_20m_gap1",       WLAN_CFG_INIT_NRCOEX_RULE3_40M_20M_GAP1},
    {"nrcoex_rule3_160m_80m_gap1",      WLAN_CFG_INIT_NRCOEX_RULE3_160M_80M_GAP1},
    {"nrcoex_rule3_40m_20m_gap2",       WLAN_CFG_INIT_NRCOEX_RULE3_40M_20M_GAP2},
    {"nrcoex_rule3_160m_80m_gap2",      WLAN_CFG_INIT_NRCOEX_RULE3_160M_80M_GAP2},
    {"nrcoex_rule3_smallgap0_act",      WLAN_CFG_INIT_NRCOEX_RULE3_GAP0_ACT},
    {"nrcoex_rule3_gap01_act",          WLAN_CFG_INIT_NRCOEX_RULE3_GAP1_ACT},
    {"nrcoex_rule3_gap12_act",          WLAN_CFG_INIT_NRCOEX_RULE3_GAP2_ACT},
    {"nrcoex_rule3_rxslot_rssi",        WLAN_CFG_INIT_NRCOEX_RULE3_RXSLOT_RSSI},
#endif
    { OAL_PTR_NULL, 0 }
};

OAL_STATIC wlan_cfg_cmd nvram_config_ini[] = {
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

    /* խ��2.4G�������� */
    {"nvram_max_txpwr_base_2p4g_nb", NVRAM_PARAMS_TX_BASE_POWER_2P4G_NB},

    /* ��׼���� */
    { "nvram_max_txpwr_base_2p4g", NVRAM_PARAMS_TX_BASE_POWER_2P4G },
    { "nvram_max_txpwr_base_5g_1", NVRAM_PARAMS_TX_BASE_POWER_5G_BAND1 },
    { "nvram_max_txpwr_base_5g_2", NVRAM_PARAMS_TX_BASE_POWER_5G_BAND2 },
    { "nvram_max_txpwr_base_5g_3", NVRAM_PARAMS_TX_BASE_POWER_5G_BAND3 },
    { "nvram_max_txpwr_base_5g_4", NVRAM_PARAMS_TX_BASE_POWER_5G_BAND4 },
    { "nvram_max_txpwr_base_5g_5", NVRAM_PARAMS_TX_BASE_POWER_5G_BAND5 },
    { "nvram_max_txpwr_base_5g_6", NVRAM_PARAMS_TX_BASE_POWER_5G_BAND6 },
    { "nvram_max_txpwr_base_5g_7", NVRAM_PARAMS_TX_BASE_POWER_5G_BAND7 },

    /* 5G��band����书�� */
    { "5g_max_pow_high_band_ce", NVRAM_PARAMS_5G_HIGH_BAND_MAX_POW },

    /* SAR���� */
    /* 0~3�� */
    { "sar_txpwr_ctrl_5g_band1_0", NVRAM_PARAMS_SAR_5G_BANK1_0 },
    { "sar_txpwr_ctrl_5g_band2_0", NVRAM_PARAMS_SAR_5G_BANK2_0 },
    { "sar_txpwr_ctrl_5g_band3_0", NVRAM_PARAMS_SAR_5G_BANK3_0 },
    { "sar_txpwr_ctrl_5g_band4_0", NVRAM_PARAMS_SAR_5G_BANK4_0 },
    { "sar_txpwr_ctrl_5g_band5_0", NVRAM_PARAMS_SAR_5G_BANK5_0 },
    { "sar_txpwr_ctrl_5g_band6_0", NVRAM_PARAMS_SAR_5G_BANK6_0 },
    { "sar_txpwr_ctrl_5g_band7_0", NVRAM_PARAMS_SAR_5G_BANK7_0 },
    { "sar_txpwr_ctrl_2g_0",       NVRAM_PARAMS_SAR_2G_0 },
    /* 4~7�� */
    { "sar_txpwr_ctrl_5g_band1_1", NVRAM_PARAMS_SAR_5G_BANK1_1 },
    { "sar_txpwr_ctrl_5g_band2_1", NVRAM_PARAMS_SAR_5G_BANK2_1 },
    { "sar_txpwr_ctrl_5g_band3_1", NVRAM_PARAMS_SAR_5G_BANK3_1 },
    { "sar_txpwr_ctrl_5g_band4_1", NVRAM_PARAMS_SAR_5G_BANK4_1 },
    { "sar_txpwr_ctrl_5g_band5_1", NVRAM_PARAMS_SAR_5G_BANK5_1 },
    { "sar_txpwr_ctrl_5g_band6_1", NVRAM_PARAMS_SAR_5G_BANK6_1 },
    { "sar_txpwr_ctrl_5g_band7_1", NVRAM_PARAMS_SAR_5G_BANK7_1 },
    { "sar_txpwr_ctrl_2g_1",       NVRAM_PARAMS_SAR_2G_1 },
    /* 8~11�� */
    { "sar_txpwr_ctrl_5g_band1_2", NVRAM_PARAMS_SAR_5G_BANK1_2 },
    { "sar_txpwr_ctrl_5g_band2_2", NVRAM_PARAMS_SAR_5G_BANK2_2 },
    { "sar_txpwr_ctrl_5g_band3_2", NVRAM_PARAMS_SAR_5G_BANK3_2 },
    { "sar_txpwr_ctrl_5g_band4_2", NVRAM_PARAMS_SAR_5G_BANK4_2 },
    { "sar_txpwr_ctrl_5g_band5_2", NVRAM_PARAMS_SAR_5G_BANK5_2 },
    { "sar_txpwr_ctrl_5g_band6_2", NVRAM_PARAMS_SAR_5G_BANK6_2 },
    { "sar_txpwr_ctrl_5g_band7_2", NVRAM_PARAMS_SAR_5G_BANK7_2 },
    { "sar_txpwr_ctrl_2g_2",       NVRAM_PARAMS_SAR_2G_2 },
    /* 12~15�� */
    { "sar_txpwr_ctrl_5g_band1_3", NVRAM_PARAMS_SAR_5G_BANK1_3 },
    { "sar_txpwr_ctrl_5g_band2_3", NVRAM_PARAMS_SAR_5G_BANK2_3 },
    { "sar_txpwr_ctrl_5g_band3_3", NVRAM_PARAMS_SAR_5G_BANK3_3 },
    { "sar_txpwr_ctrl_5g_band4_3", NVRAM_PARAMS_SAR_5G_BANK4_3 },
    { "sar_txpwr_ctrl_5g_band5_3", NVRAM_PARAMS_SAR_5G_BANK5_3 },
    { "sar_txpwr_ctrl_5g_band6_3", NVRAM_PARAMS_SAR_5G_BANK6_3 },
    { "sar_txpwr_ctrl_5g_band7_3", NVRAM_PARAMS_SAR_5G_BANK7_3 },
    { "sar_txpwr_ctrl_2g_3",       NVRAM_PARAMS_SAR_2G_3 },
    /* 16~19�� */
    { "sar_txpwr_ctrl_5g_band1_4", NVRAM_PARAMS_SAR_5G_BANK1_4 },
    { "sar_txpwr_ctrl_5g_band2_4", NVRAM_PARAMS_SAR_5G_BANK2_4 },
    { "sar_txpwr_ctrl_5g_band3_4", NVRAM_PARAMS_SAR_5G_BANK3_4 },
    { "sar_txpwr_ctrl_5g_band4_4", NVRAM_PARAMS_SAR_5G_BANK4_4 },
    { "sar_txpwr_ctrl_5g_band5_4", NVRAM_PARAMS_SAR_5G_BANK5_4 },
    { "sar_txpwr_ctrl_5g_band6_4", NVRAM_PARAMS_SAR_5G_BANK6_4 },
    { "sar_txpwr_ctrl_5g_band7_4", NVRAM_PARAMS_SAR_5G_BANK7_4 },
    { "sar_txpwr_ctrl_2g_4",       NVRAM_PARAMS_SAR_2G_4 },

    /* �ߴ��ŵ������ */
    { "side_band_txpwr_limit_5g_20m_0", NVRAM_PARAMS_SIDE_BAND_TXPWR_5G_20M_0 },
    { "side_band_txpwr_limit_5g_20m_1", NVRAM_PARAMS_SIDE_BAND_TXPWR_5G_20M_1 },
    { "side_band_txpwr_limit_5g_20m_2", NVRAM_PARAMS_SIDE_BAND_TXPWR_5G_20M_2 },
    { "side_band_txpwr_limit_5g_40m_0", NVRAM_PARAMS_SIDE_BAND_TXPWR_5G_40M_0 },
    { "side_band_txpwr_limit_5g_40m_1", NVRAM_PARAMS_SIDE_BAND_TXPWR_5G_40M_1 },
    { "side_band_txpwr_limit_5g_80m_0", NVRAM_PARAMS_SIDE_BAND_TXPWR_5G_80M_0 },
    { "side_band_txpwr_limit_5g_80m_1", NVRAM_PARAMS_SIDE_BAND_TXPWR_5G_80M_1 },
    { "side_band_txpwr_limit_24g_ch1",  NVRAM_PARAMS_SIDE_BAND_TXPWR_2G_1 },
    { "side_band_txpwr_limit_24g_ch2",  NVRAM_PARAMS_SIDE_BAND_TXPWR_2G_2 },
    { "side_band_txpwr_limit_24g_ch3",  NVRAM_PARAMS_SIDE_BAND_TXPWR_2G_3 },
    { "side_band_txpwr_limit_24g_ch4",  NVRAM_PARAMS_SIDE_BAND_TXPWR_2G_4 },
    { "side_band_txpwr_limit_24g_ch5",  NVRAM_PARAMS_SIDE_BAND_TXPWR_2G_5 },
    { "side_band_txpwr_limit_24g_ch6",  NVRAM_PARAMS_SIDE_BAND_TXPWR_2G_6 },
    { "side_band_txpwr_limit_24g_ch7",  NVRAM_PARAMS_SIDE_BAND_TXPWR_2G_7 },
    { "side_band_txpwr_limit_24g_ch8",  NVRAM_PARAMS_SIDE_BAND_TXPWR_2G_8 },
    { "side_band_txpwr_limit_24g_ch9",  NVRAM_PARAMS_SIDE_BAND_TXPWR_2G_9 },
    { "side_band_txpwr_limit_24g_ch10", NVRAM_PARAMS_SIDE_BAND_TXPWR_2G_10 },
    { "side_band_txpwr_limit_24g_ch11", NVRAM_PARAMS_SIDE_BAND_TXPWR_2G_11 },
    { "side_band_txpwr_limit_24g_ch12", NVRAM_PARAMS_SIDE_BAND_TXPWR_2G_12 },
    { "side_band_txpwr_limit_24g_ch13", NVRAM_PARAMS_SIDE_BAND_TXPWR_2G_13 },
    {"ext_side_band_txpwr_limit_5g_20m", NVRAM_PARAMS_EXT_SIDE_BAND_TXPWR_5G_20M},
    {"ext_side_band_txpwr_limit_5g_40m", NVRAM_PARAMS_EXT_SIDE_BAND_TXPWR_5G_40M},
    { OAL_PTR_NULL,     NVRAM_PARAMS_INDEX_BUTT }
};

OAL_STATIC wlan_cfg_cmd wifi_config_priv[] = {
    /* У׼���� */
    { "cali_mask", WLAN_CFG_PRIV_CALI_MASK },
    /* bit4:����ȡNV���������(1:����ȡ 0����ȡ) */
    { "cali_data_mask", WLAN_CFG_PRIV_CALI_DATA_MASK },
    { "temp_pro_en",              WLAN_CFG_PRIV_TEMP_PRO_EN },
    { "temp_pro_reduce_pwr_en",   WLAN_CFG_PRIV_TEMP_PRO_REDUCE_PWR_EN },
    { "temp_pro_safe_th",         WLAN_CFG_PRIV_TEMP_PRO_SAFE_TH },
    { "temp_pro_over_th",         WLAN_CFG_PRIV_TEMP_PRO_OVER_TH },
    { "temp_pro_pa_off_th",       WLAN_CFG_PRIV_TEMP_PRO_PA_OFF_TH },
    { "dsss2ofdm_dbb_pwr_bo_val", WLAN_DSSS2OFDM_DBB_PWR_BO_VAL },
    { "fast_ps_check_cnt",        WLAN_CFG_PRIV_FAST_CHECK_CNT },
    { "voe_switch_mask",          WLAN_CFG_PRIV_VOE_SWITCH },
    { "dyn_bypass_extlna_enable", WLAN_CFG_PRIV_DYN_BYPASS_EXTLNA },
    { "hcc_flowctrl_type",        WLAN_CFG_PRIV_HCC_FLOWCTRL_TYPE },
    { "ext_fem_5g_type",          WLAN_CFG_PRIV_5G_EXT_FEM_TYPE },
    { OAL_PTR_NULL,               0 }
};

OAL_STATIC wlan_cfg_nv_map_handler wifi_nvram_cfg_handler[] = {
    { "WITXBW0",        "dp2ginit0", HWIFI_CFG_NV_WITXNVBWC0_NUMBER,     WLAN_CFG_NVRAM_DP2G_INIT0,           {0}},
    { "WITXCCK",        "pa2gccka0", HWIFI_CFG_NV_WITXNVCCK_NUMBER,      WLAN_CFG_DTS_NVRAM_RATIO_PA2GCCKA0,  {0}},
    { "WINVRAM",        "pa2ga0",    HWIFI_CFG_NV_WINVRAM_NUMBER,        WLAN_CFG_NVRAM_RATIO_PA2GA0,         {0}},
    { "WITXL0",         "pa2g40a0",  HWIFI_CFG_NV_WITXL2G5G0_NUMBER,     WLAN_CFG_DTS_NVRAM_RATIO_PA2G40A0,   {0}},
    { "WINVRAM",        "pa5ga0",    HWIFI_CFG_NV_WINVRAM_NUMBER,        WLAN_CFG_DTS_NVRAM_RATIO_PA5GA0,     {0}},
    { "WINVRAM",        "pa5glowa0", HWIFI_CFG_NV_WITX_5G_LOW_NUMBER,    WLAN_CFG_DTS_NVRAM_RATIO_PA5GA0_LOW, {0}},
};

OAL_STATIC wlan_cfg_cmd nvram_pro_line_config_ini[] = {
    /* OAL_PTR_NULL��ʾ����ini����� */
    { OAL_PTR_NULL, WLAN_CFG_NVRAM_DP2G_INIT0 },
    /* ����nvram���� */
    { "nvram_pa2gccka0",        WLAN_CFG_DTS_NVRAM_RATIO_PA2GCCKA0 },
    { "nvram_pa2ga0",           WLAN_CFG_NVRAM_RATIO_PA2GA0 },
    { "nvram_pa2g40a0",         WLAN_CFG_DTS_NVRAM_RATIO_PA2G40A0 },
    { "nvram_pa5ga0",           WLAN_CFG_DTS_NVRAM_RATIO_PA5GA0 },
    { "nvram_pa5ga0_low",       WLAN_CFG_DTS_NVRAM_RATIO_PA5GA0_LOW },
    { "nvram_pa5ga0_band1",     WLAN_CFG_DTS_NVRAM_RATIO_PA5GA0_BAND1 },
    { "nvram_pa2gcwa0",         WLAN_CFG_DTS_NVRAM_RATIO_PA2GCWA0 },
    { "nvram_pa5ga0_band1_low", WLAN_CFG_DTS_NVRAM_RATIO_PA5GA0_BAND1_LOW },

    { "nvram_ppa2gcwa0", WLAN_CFG_DTS_NVRAM_RATIO_PPA2GCWA0 },

    { "bt_nvram_power_cwa0", BT_CFG_DTS_NVRAM_POWER_CWA0 },
    { "bt_nvram_ppa_cwa0",   BT_CFG_DTS_NVRAM_PPA_CWA0 },
    { OAL_PTR_NULL,          WLAN_CFG_DTS_NVRAM_PARAMS_BUTT },
};

/*
 * �� �� ��  : original_value_for_dts_params
 * ��������  : dts���ƻ�������ֵ����
 */
OAL_STATIC oal_void original_value_for_dts_params(oal_void)
{
    al_dts_params[WLAN_CFG_DTS_BAND_5G_ENABLE] = 1;

    /* У׼ */
    al_dts_params[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN1]  = WLAN_CALI_TXPWR_REF_2G_CH1_VAL;
    al_dts_params[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN2]  = WLAN_CALI_TXPWR_REF_2G_CH2_VAL;
    al_dts_params[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN3]  = WLAN_CALI_TXPWR_REF_2G_CH3_VAL;
    al_dts_params[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN4]  = WLAN_CALI_TXPWR_REF_2G_CH4_VAL;
    al_dts_params[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN5]  = WLAN_CALI_TXPWR_REF_2G_CH5_VAL;
    al_dts_params[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN6]  = WLAN_CALI_TXPWR_REF_2G_CH6_VAL;
    al_dts_params[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN7]  = WLAN_CALI_TXPWR_REF_2G_CH7_VAL;
    al_dts_params[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN8]  = WLAN_CALI_TXPWR_REF_2G_CH8_VAL;
    al_dts_params[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN9]  = WLAN_CALI_TXPWR_REF_2G_CH9_VAL;
    al_dts_params[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN10] = WLAN_CALI_TXPWR_REF_2G_CH10_VAL;
    al_dts_params[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN11] = WLAN_CALI_TXPWR_REF_2G_CH11_VAL;
    al_dts_params[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN12] = WLAN_CALI_TXPWR_REF_2G_CH12_VAL;
    al_dts_params[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_2G_VAL_CHAN13] = WLAN_CALI_TXPWR_REF_2G_CH13_VAL;
    al_dts_params[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_5G_VAL_BAND1]  = WLAN_CALI_TXPWR_REF_5G_BAND1_VAL;
    al_dts_params[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_5G_VAL_BAND2]  = WLAN_CALI_TXPWR_REF_5G_BAND2_VAL;
    al_dts_params[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_5G_VAL_BAND3]  = WLAN_CALI_TXPWR_REF_5G_BAND3_VAL;
    al_dts_params[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_5G_VAL_BAND4]  = WLAN_CALI_TXPWR_REF_5G_BAND4_VAL;
    al_dts_params[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_5G_VAL_BAND5]  = WLAN_CALI_TXPWR_REF_5G_BAND5_VAL;
    al_dts_params[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_5G_VAL_BAND6]  = WLAN_CALI_TXPWR_REF_5G_BAND6_VAL;
    al_dts_params[WLAN_CFG_DTS_CALI_TXPWR_PA_DC_REF_5G_VAL_BAND7]  = WLAN_CALI_TXPWR_REF_5G_BAND7_VAL;
    al_dts_params[WLAN_CFG_DTS_CALI_TONE_AMP_GRADE] = WLAN_CALI_TONE_GRADE_AMP;

    /* rf register */
    al_dts_params[WLAN_CFG_DTS_RF_REG117] = WLAN_REG117_RF;
    al_dts_params[WLAN_CFG_DTS_RF_REG123] = WLAN_REG123_RF;
    al_dts_params[WLAN_CFG_DTS_RF_REG124] = WLAN_REG124_RF;
    al_dts_params[WLAN_CFG_DTS_RF_REG125] = WLAN_REG125_RF;
    al_dts_params[WLAN_CFG_DTS_RF_REG126] = WLAN_REG126_RF;
    /* bt tmp */
    al_dts_params[WLAN_CFG_DTS_BT_CALI_TXPWR_DPN_BAND1] = WLAN_BT_CALI_TXPWR_BAND1_DPN;
    al_dts_params[WLAN_CFG_DTS_BT_CALI_TXPWR_DPN_BAND2] = WLAN_BT_CALI_TXPWR_BAND2_DPN;
    al_dts_params[WLAN_CFG_DTS_BT_CALI_TXPWR_DPN_BAND3] = WLAN_BT_CALI_TXPWR_BAND3_DPN;
    al_dts_params[WLAN_CFG_DTS_BT_CALI_TXPWR_DPN_BAND4] = WLAN_BT_CALI_TXPWR_BAND4_DPN;
    al_dts_params[WLAN_CFG_DTS_BT_CALI_TXPWR_DPN_BAND5] = WLAN_BT_CALI_TXPWR_BAND5_DPN;
    al_dts_params[WLAN_CFG_DTS_BT_CALI_TXPWR_DPN_BAND6] = WLAN_BT_CALI_TXPWR_BAND6_DPN;
    al_dts_params[WLAN_CFG_DTS_BT_CALI_TXPWR_DPN_BAND7] = WLAN_BT_CALI_TXPWR_BAND7_DPN;
    al_dts_params[WLAN_CFG_DTS_BT_CALI_TXPWR_DPN_BAND8] = WLAN_BT_CALI_TXPWR_BAND8_DPN;
    al_dts_params[WLAN_CFG_DTS_BT_CALI_TXPWR_NUM] = WLAN_BT_CALI_TXPWR_NUM_VAL;
    al_dts_params[WLAN_CFG_DTS_BT_CALI_TXPWR_FREQ1] = WLAN_BT_CALI_TXPWR_FRE1;
    al_dts_params[WLAN_CFG_DTS_BT_CALI_TXPWR_FREQ2] = WLAN_BT_CALI_TXPWR_FRE2;
    al_dts_params[WLAN_CFG_DTS_BT_CALI_TXPWR_FREQ3] = WLAN_BT_CALI_TXPWR_FRE3;
    al_dts_params[WLAN_CFG_DTS_BT_CALI_TXPWR_FREQ4] = WLAN_BT_CALI_TXPWR_FRE4;
    al_dts_params[WLAN_CFG_DTS_BT_CALI_TXPWR_FREQ5] = WLAN_BT_CALI_TXPWR_FRE5;
    al_dts_params[WLAN_CFG_DTS_BT_CALI_TXPWR_FREQ6] = WLAN_BT_CALI_TXPWR_FRE6;
    al_dts_params[WLAN_CFG_DTS_BT_CALI_TXPWR_FREQ7] = WLAN_BT_CALI_TXPWR_FRE7;
    al_dts_params[WLAN_CFG_DTS_BT_CALI_TXPWR_FREQ8] = WLAN_BT_CALI_TXPWR_FRE8;
    al_dts_params[WLAN_CFG_DTS_BT_CALI_INSERTION_LOSS] = WLAN_BT_CALI_LOSS_VAL;
    al_dts_params[WLAN_CFG_DTS_BT_CALI_GM_CALI_EN] = WLAN_BT_CALI_GM_EN_VAL;
    al_dts_params[WLAN_CFG_DTS_BT_CALI_GM0_DB10] = WLAN_BT_CALI_GM0_DB10_VAL;
    al_dts_params[WLAN_CFG_DTS_BT_CALI_BASE_POWER] = WLAN_BT_CALI_BASE_POWER_VAL;
    al_dts_params[WLAN_CFG_DTS_BT_CALI_IS_DPN_CALC] = WLAN_BT_CALI_IS_DPN_VAL;

#ifdef _PRE_WLAN_FIT_BASED_REALTIME_CALI
    /* ��̬У׼ */
    al_dts_params[WLAN_CFG_DTS_DYN_CALI_DSCR_ITERVL] = WLAN_DYN_CALI_DSCR_ITERVL_VAL;
    /* DPN 40M 20M 11b */
    al_dts_params[WLAN_CFG_DTS_2G_CORE0_DPN_CH1] = WLAN_2G_CORE0_CH1_DPN;
    al_dts_params[WLAN_CFG_DTS_2G_CORE0_DPN_CH2] = WLAN_2G_CORE0_CH2_DPN;
    al_dts_params[WLAN_CFG_DTS_2G_CORE0_DPN_CH3] = WLAN_2G_CORE0_CH3_DPN;
    al_dts_params[WLAN_CFG_DTS_2G_CORE0_DPN_CH4] = WLAN_2G_CORE0_CH4_DPN;
    al_dts_params[WLAN_CFG_DTS_2G_CORE0_DPN_CH5] = WLAN_2G_CORE0_CH5_DPN;
    al_dts_params[WLAN_CFG_DTS_2G_CORE0_DPN_CH6] = WLAN_2G_CORE0_CH6_DPN;
    al_dts_params[WLAN_CFG_DTS_2G_CORE0_DPN_CH7] = WLAN_2G_CORE0_CH7_DPN;
    al_dts_params[WLAN_CFG_DTS_2G_CORE0_DPN_CH8] = WLAN_2G_CORE0_CH8_DPN;
    al_dts_params[WLAN_CFG_DTS_2G_CORE0_DPN_CH9] = WLAN_2G_CORE0_CH9_DPN;
    al_dts_params[WLAN_CFG_DTS_2G_CORE0_DPN_CH10] = WLAN_2G_CORE0_CH10_DPN;
    al_dts_params[WLAN_CFG_DTS_2G_CORE0_DPN_CH11] = WLAN_2G_CORE0_CH11_DPN;
    al_dts_params[WLAN_CFG_DTS_2G_CORE0_DPN_CH12] = WLAN_2G_CORE0_CH12_DPN;
    al_dts_params[WLAN_CFG_DTS_2G_CORE0_DPN_CH13] = WLAN_2G_CORE0_CH13_DPN;
    al_dts_params[WLAN_CFG_DTS_5G_CORE0_DPN_B0] = WLAN_5G_CORE0_B0_DPN;
    al_dts_params[WLAN_CFG_DTS_5G_CORE0_DPN_B1] = WLAN_5G_CORE0_B1_DPN;
    al_dts_params[WLAN_CFG_DTS_5G_CORE0_DPN_B2] = WLAN_5G_CORE0_B2_DPN;
    al_dts_params[WLAN_CFG_DTS_5G_CORE0_DPN_B3] = WLAN_5G_CORE0_B3_DPN;
    al_dts_params[WLAN_CFG_DTS_5G_CORE0_DPN_B4] = WLAN_5G_CORE0_B4_DPN;
    al_dts_params[WLAN_CFG_DTS_DYN_CALI_OPT_SWITCH] = WLAN_DYN_CALI_OPT_SWITCH_VAL;
    al_dts_params[WLAN_CFG_DTS_DYN_CALI_GM0_DB10_AMEND] = WLAN_DYN_CALI_GM0_DB10_AMEND_VAL;
#endif
    al_dts_params[WLAN_CFG_DTS_5G_UPC_UP_LIMIT] = WLAN_5G_UPC_UP_LIMIT_VAL;
    al_dts_params[WLAN_CFG_DTS_5G_IQ_BACKOFF_POWER] = WLAN_5G_IQ_BACKOFF_POWER_VAL;
    al_dts_params[WLAN_CFG_DTS_5G_IQ_CALI_POWER] = WLAN_5G_IQ_CALI_POWER_VAL;
}

/*
 * �� �� ��  : host_params_init_first_of_rf
 * ��������  : ��RFǰ�˶��ƻ�����ȫ������ al_host_init_params ����ֵ
 *             ini�ļ���ȡʧ��ʱ�ó�ֵ(����������)
 */
OAL_STATIC oal_void host_params_init_first_of_rf(oal_void)
{
    /* 2G RFǰ�� */
    al_host_init_params[WLAN_CFG_INIT_RF_LINE_TXRX_GAIN_DB_2G_BAND1_MULT4] = WLAN_DB_2G_BAND1_MULT4;
    al_host_init_params[WLAN_CFG_INIT_RF_LINE_TXRX_GAIN_DB_2G_BAND1_MULT10] = WLAN_DB_2G_BAND1_MULT10;
    al_host_init_params[WLAN_CFG_INIT_RF_LINE_TXRX_GAIN_DB_2G_BAND2_MULT4] = WLAN_DB_2G_BAND2_MULT4;
    al_host_init_params[WLAN_CFG_INIT_RF_LINE_TXRX_GAIN_DB_2G_BAND2_MULT10] = WLAN_DB_2G_BAND2_MULT10;
    al_host_init_params[WLAN_CFG_INIT_RF_LINE_TXRX_GAIN_DB_2G_BAND3_MULT4] = WLAN_DB_2G_BAND3_MULT4;
    al_host_init_params[WLAN_CFG_INIT_RF_LINE_TXRX_GAIN_DB_2G_BAND3_MULT10] = WLAN_DB_2G_BAND3_MULT10;
    /* 5G RFǰ�� */
    al_host_init_params[WLAN_CFG_INIT_RF_LINE_TXRX_GAIN_DB_5G_BAND1_MULT4] = WLAN_DB_5G_BAND1_MULT4;
    al_host_init_params[WLAN_CFG_INIT_RF_LINE_TXRX_GAIN_DB_5G_BAND1_MULT10] = WLAN_DB_5G_BAND1_MULT10;
    al_host_init_params[WLAN_CFG_INIT_RF_LINE_TXRX_GAIN_DB_5G_BAND2_MULT4] = WLAN_DB_5G_BAND2_MULT4;
    al_host_init_params[WLAN_CFG_INIT_RF_LINE_TXRX_GAIN_DB_5G_BAND2_MULT10] = WLAN_DB_5G_BAND2_MULT10;
    al_host_init_params[WLAN_CFG_INIT_RF_LINE_TXRX_GAIN_DB_5G_BAND3_MULT4] = WLAN_DB_5G_BAND3_MULT4;
    al_host_init_params[WLAN_CFG_INIT_RF_LINE_TXRX_GAIN_DB_5G_BAND3_MULT10] = WLAN_DB_5G_BAND3_MULT10;
    al_host_init_params[WLAN_CFG_INIT_RF_LINE_TXRX_GAIN_DB_5G_BAND4_MULT4] = WLAN_DB_5G_BAND4_MULT4;
    al_host_init_params[WLAN_CFG_INIT_RF_LINE_TXRX_GAIN_DB_5G_BAND4_MULT10] = WLAN_DB_5G_BAND4_MULT10;
    al_host_init_params[WLAN_CFG_INIT_RF_LINE_TXRX_GAIN_DB_5G_BAND5_MULT4] = WLAN_DB_5G_BAND5_MULT4;
    al_host_init_params[WLAN_CFG_INIT_RF_LINE_TXRX_GAIN_DB_5G_BAND5_MULT10] = WLAN_DB_5G_BAND5_MULT10;
    al_host_init_params[WLAN_CFG_INIT_RF_LINE_TXRX_GAIN_DB_5G_BAND6_MULT4] = WLAN_DB_5G_BAND6_MULT4;
    al_host_init_params[WLAN_CFG_INIT_RF_LINE_TXRX_GAIN_DB_5G_BAND6_MULT10] = WLAN_DB_5G_BAND6_MULT10;
    al_host_init_params[WLAN_CFG_INIT_RF_LINE_TXRX_GAIN_DB_5G_BAND7_MULT4] = WLAN_DB_5G_BAND7_MULT4;
    al_host_init_params[WLAN_CFG_INIT_RF_LINE_TXRX_GAIN_DB_5G_BAND7_MULT10] = WLAN_DB_5G_BAND7_MULT10;
    al_host_init_params[WLAN_CFG_INIT_RF_LINE_RX_GAIN_DB_5G] = WLAN_RF_LINE_RX_GAIN_DB_5G;
    al_host_init_params[WLAN_CFG_INIT_LNA_GAIN_DB_5G] = WLAN_LNA_GAIN_DB_5G;
    al_host_init_params[WLAN_CFG_INIT_RF_LINE_TX_GAIN_DB_5G] = WLAN_RF_LINE_TX_GAIN_DB_5G;
    al_host_init_params[WLAN_CFG_INIT_EXT_SWITCH_ISEXIST_5G] = WLAN_EXT_SWITCH_ISEXIST_5G;
    al_host_init_params[WLAN_CFG_INIT_EXT_PA_ISEXIST_5G] = WLAN_EXT_PA_ISEXIST_5G;
    al_host_init_params[WLAN_CFG_INIT_EXT_LNA_ISEXIST_5G] = WLAN_EXT_LNA_ISEXIST_5G;
    al_host_init_params[WLAN_CFG_INIT_LNA_ON2OFF_TIME_NS_5G] = WLAN_LNA_ON2OFF_TIME_NS_5G;
    al_host_init_params[WLAN_CFG_INIT_LNA_OFF2ON_TIME_NS_5G] = WLAN_LNA_OFF2ON_TIME_NS_5G;
}
 /*
 * �� �� ��  : host_params_init_performace_throughtput
 * ��������  : ����������Ż�����ֵ
 *             ini�ļ���ȡʧ��ʱ�ó�ֵ(����������)
 */
OAL_STATIC oal_void host_params_init_performace_throughtput(oal_void)
{
    /* �����Ƶ��� */
    al_host_init_params[WLAN_CFG_INIT_BINDCPU] = OAL_TRUE;
    al_host_init_params[WLAN_CFG_INIT_BINDCPU_MASK] = WLAN_BINDCPU_DEFAULT_MASK;
    al_host_init_params[WLAN_CFG_INIT_BINDCPU_PPSHIGH] = WLAN_BUSY_CPU_PPS_THROUGHT;
    al_host_init_params[WLAN_CFG_INIT_BINDCPU_PPSLOW] = WLAN_IDLE_CPU_PPS_THROUGHT;

    /* ���ܱ�ƴ�Ż���� */
    al_host_init_params[WLAN_CFG_INIT_TX_OPT_SWITCH_STA] = WLAN_TX_OPT_SWITCH_STA;
    al_host_init_params[WLAN_CFG_INIT_TX_OPT_SWITCH_AP] = WLAN_TX_OPT_SWITCH_AP;
    al_host_init_params[WLAN_CFG_INIT_TX_OPT_PPS_HIGH] = WLAN_TX_OPT_TH_HIGH;
    al_host_init_params[WLAN_CFG_INIT_TX_OPT_PPS_LOW] = WLAN_TX_OPT_TH_LOW;
    al_host_init_params[WLAN_CFG_INIT_TX_OPT_CWMIN] = WLAN_TX_OPT_CWMIN;
    al_host_init_params[WLAN_CFG_INIT_TX_OPT_CWMAX] = WLAN_TX_OPT_CWMAX;
    al_host_init_params[WLAN_CFG_INIT_TX_OPT_TXOPLIMIT] = WLAN_TX_OPT_TXOPLIMIT;
    al_host_init_params[WLAN_CFG_INIT_TX_OPT_DYN_TXOPLIMIT] = WLAN_TX_OPT_DYN_TXOPLIMIT;

    al_host_init_params[WLAN_CFG_INIT_DEVICE_RX_TCP_ACK_FILTER] = OAL_FALSE;
    al_host_init_params[WLAN_CFG_INIT_DEVICE_RX_TCP_ACK_2G_TH]  = WLAN_DEVICE_RX_TCP_ACK_DEFAULT_2G_TH;
    al_host_init_params[WLAN_CFG_INIT_DEVICE_RX_TCP_ACK_5G_TH]  = WLAN_DEVICE_RX_TCP_ACK_DEFAULT_5G_TH;

    al_host_init_params[WLAN_CFG_INIT_TX_TCP_ACK_BUF] = OAL_TRUE;
    al_host_init_params[WLAN_CFG_INIT_TCP_ACK_BUF_HIGH] = WLAN_TCP_ACK_BUF_THROUGHPUT_THRESHOLD_HIGH;
    al_host_init_params[WLAN_CFG_INIT_TCP_ACK_BUF_LOW] = WLAN_TCP_ACK_BUF_THROUGHPUT_THRESHOLD_LOW;
    al_host_init_params[WLAN_CFG_INIT_TCP_ACK_BUF_HIGH_40M] = WLAN_TCP_ACK_BUF_THROUGHPUT_THRESHOLD_HIGH_40M;
    al_host_init_params[WLAN_CFG_INIT_TCP_ACK_BUF_LOW_40M] = WLAN_TCP_ACK_BUF_THROUGHPUT_THRESHOLD_LOW_40M;
    al_host_init_params[WLAN_CFG_INIT_TCP_ACK_BUF_HIGH_80M] = WLAN_TCP_ACK_BUF_THROUGHPUT_THRESHOLD_HIGH_80M;
    al_host_init_params[WLAN_CFG_INIT_TCP_ACK_BUF_LOW_80M] = WLAN_TCP_ACK_BUF_THROUGHPUT_THRESHOLD_LOW_80M;
    /* RX DYN BYPASS EXTLNA ���ƻ����� */
    al_host_init_params[WLAN_CFG_INIT_RX_DYN_BYPASS_EXTLNA] = OAL_FALSE;
    al_host_init_params[WLAN_CFG_INIT_RX_DYN_BYPASS_EXTLNA_HIGH] = WLAN_DYN_BYPASS_EXTLNA_THROUGHPUT_THRESHOLD_HIGH;
    al_host_init_params[WLAN_CFG_INIT_RX_DYN_BYPASS_EXTLNA_LOW] = WLAN_DYN_BYPASS_EXTLNA_THROUGHPUT_THRESHOLD_LOW;
    /* SMALL AMSDU ���ƻ����� */
    al_host_init_params[WLAN_CFG_INIT_TX_SMALL_AMSDU] = OAL_TRUE;
    al_host_init_params[WLAN_CFG_INIT_SMALL_AMSDU_HIGH] = WLAN_SMALL_AMSDU_THROUGHPUT_THRESHOLD_HIGH;
    al_host_init_params[WLAN_CFG_INIT_SMALL_AMSDU_LOW] = WLAN_SMALL_AMSDU_THROUGHPUT_THRESHOLD_LOW;
    al_host_init_params[WLAN_CFG_INIT_SMALL_AMSDU_PPS_HIGH] = WLAN_SMALL_AMSDU_PPS_THRESHOLD_HIGH;
    al_host_init_params[WLAN_CFG_INIT_SMALL_AMSDU_PPS_LOW] = WLAN_SMALL_AMSDU_PPS_THRESHOLD_LOW;

#ifdef _PRE_WLAN_FEATURE_MULTI_NETBUF_AMSDU
    al_host_init_params[WLAN_CFG_INIT_AMPDU_AMSDU_SKB] = OAL_FALSE;
    al_host_init_params[WLAN_CFG_INIT_AMSDU_AMPDU_TH_HIGH] = WLAN_AMSDU_AMPDU_THROUGHPUT_THRESHOLD_HIGH;
    al_host_init_params[WLAN_CFG_INIT_AMSDU_AMPDU_TH_LOW] = WLAN_AMSDU_AMPDU_THROUGHPUT_THRESHOLD_LOW;
    al_host_init_params[WLAN_CFG_INIT_RX_AMPDU_AMSDU_SKB] = OAL_FALSE;
#endif
#ifdef _PRE_WLAN_TCP_OPT
    al_host_init_params[WLAN_CFG_INIT_TCP_ACK_FILTER] = OAL_TRUE;
    al_host_init_params[WLAN_CFG_INIT_TCP_ACK_FILTER_TH_HIGH] = WLAN_TCP_ACK_FILTER_TH_HIGH;
    al_host_init_params[WLAN_CFG_INIT_TCP_ACK_FILTER_TH_LOW] = WLAN_TCP_ACK_FILTER_TH_LOW;
#endif
}


OAL_STATIC oal_void host_params_init_cca(oal_void)
{
    al_host_init_params[WLAN_CFG_INIT_DELTA_CCA_ED_HIGH_20TH_2G] = 0;
    al_host_init_params[WLAN_CFG_INIT_DELTA_CCA_ED_HIGH_40TH_2G] = 0;
    al_host_init_params[WLAN_CFG_INIT_DELTA_CCA_ED_HIGH_20TH_5G] = 0;
    al_host_init_params[WLAN_CFG_INIT_DELTA_CCA_ED_HIGH_40TH_5G] = 0;
}
#ifdef _PRE_WLAN_FEATURE_NRCOEX

OAL_STATIC oal_void host_params_init_nrcoex_params(oal_void)
{
    al_host_init_params[WLAN_CFG_INIT_NRCOEX_ENABLE] = OAL_FALSE;
    al_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE0_FREQ] = 0x0;
    al_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE0_40M_20M_GAP0] = 0x0;
    al_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE0_160M_80M_GAP0] = 0x0;
    al_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE0_40M_20M_GAP1] = 0x0;
    al_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE0_160M_80M_GAP1] = 0x0;
    al_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE0_40M_20M_GAP2] = 0x0;
    al_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE0_160M_80M_GAP2] = 0x0;
    al_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE0_GAP0_ACT] = 0xff;
    al_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE0_GAP1_ACT] = 0xff;
    al_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE0_GAP2_ACT] = 0xff;
    al_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE0_RXSLOT_RSSI] = 0x0;
    al_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE1_FREQ] = 0x0;
    al_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE1_40M_20M_GAP0] = 0x0;
    al_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE1_160M_80M_GAP0] = 0x0;
    al_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE1_40M_20M_GAP1] = 0x0;
    al_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE1_160M_80M_GAP1] = 0x0;
    al_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE1_40M_20M_GAP2] = 0x0;
    al_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE1_160M_80M_GAP2] = 0x0;
    al_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE1_GAP0_ACT] = 0xff;
    al_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE1_GAP1_ACT] = 0xff;
    al_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE1_GAP2_ACT] = 0xff;
    al_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE1_RXSLOT_RSSI] = 0x0;
    al_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE2_FREQ] = 0x0;
    al_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE2_40M_20M_GAP0] = 0x0;
    al_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE2_160M_80M_GAP0] = 0x0;
    al_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE2_40M_20M_GAP1] = 0x0;
    al_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE2_160M_80M_GAP1] = 0x0;
    al_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE2_40M_20M_GAP2] = 0x0;
    al_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE2_160M_80M_GAP2] = 0x0;
    al_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE2_GAP0_ACT] = 0xff;
    al_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE2_GAP1_ACT] = 0xff;
    al_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE2_GAP2_ACT] = 0xff;
    al_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE2_RXSLOT_RSSI] = 0x0;
    al_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE3_FREQ] = 0x0;
    al_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE3_40M_20M_GAP0] = 0x0;
    al_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE3_160M_80M_GAP0] = 0x0;
    al_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE3_40M_20M_GAP1] = 0x0;
    al_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE3_160M_80M_GAP1] = 0x0;
    al_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE3_40M_20M_GAP2] = 0x0;
    al_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE3_160M_80M_GAP2] = 0x0;
    al_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE3_GAP0_ACT] = 0xff;
    al_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE3_GAP1_ACT] = 0xff;
    al_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE3_GAP2_ACT] = 0xff;
    al_host_init_params[WLAN_CFG_INIT_NRCOEX_RULE3_RXSLOT_RSSI] = 0x0;
}
#endif
/*
 * �� �� ��  : host_params_init_first
 * ��������  : �����ƻ�����ȫ������ al_host_init_params ����ֵ
 *             ini�ļ���ȡʧ��ʱ�ó�ֵ
 * �� �� ֵ  : OAL_SUCC �� ʧ�ܴ�����
 */
OAL_STATIC oal_void host_params_init_first(oal_void)
{
    /* ROAM */
    al_host_init_params[WLAN_CFG_INIT_ROAM_SWITCH] = WLAN_ROAM_SWITCH_MODE;
    al_host_init_params[WLAN_CFG_INIT_SCAN_ORTHOGONAL] = WLAN_SCAN_ORTHOGONAL_VAL;
    al_host_init_params[WLAN_CFG_INIT_TRIGGER_B] = WLAN_TRIGGER_B_VAL;
    al_host_init_params[WLAN_CFG_INIT_TRIGGER_A] = WLAN_TRIGGER_A_VAL;
    al_host_init_params[WLAN_CFG_INIT_DELTA_B] = WLAN_DELTA_B_VAL;
    al_host_init_params[WLAN_CFG_INIT_DELTA_A] = WLAN_DELTA_A_VAL;

    /* ���� */
    al_host_init_params[WLAN_CFG_INIT_AMPDU_TX_MAX_NUM] = WLAN_AMPDU_TX_MAX_BUF_SIZE;
    al_host_init_params[WLAN_CFG_INIT_USED_MEM_FOR_START] = WLAN_MEM_FOR_START;
    al_host_init_params[WLAN_CFG_INIT_USED_MEM_FOR_STOP] = WLAN_MEM_FOR_STOP;
    al_host_init_params[WLAN_CFG_INIT_RX_ACK_LIMIT] = WLAN_RX_ACK_LIMIT_VAL;
    al_host_init_params[WLAN_CFG_INIT_SDIO_D2H_ASSEMBLE_COUNT] = HISDIO_DEV2HOST_SCATT_MAX;
    al_host_init_params[WLAN_CFG_INIT_SDIO_H2D_ASSEMBLE_COUNT] = WLAN_SDIO_H2D_ASSEMBLE_COUNT_VAL;
    /* LINKLOSS */
    al_host_init_params[WLAN_CFG_INIT_LINK_LOSS_THRESHOLD_WLAN_BT] = WLAN_LOSS_THRESHOLD_WLAN_BT;
    al_host_init_params[WLAN_CFG_INIT_LINK_LOSS_THRESHOLD_WLAN_DBAC] = WLAN_LOSS_THRESHOLD_WLAN_DBAC;
    al_host_init_params[WLAN_CFG_INIT_LINK_LOSS_THRESHOLD_WLAN_NORMAL] = WLAN_LOSS_THRESHOLD_WLAN_NORMAL;

    /* �Զ���Ƶ */
    al_host_init_params[WLAN_CFG_INIT_PPS_THRESHOLD_LEVEL_0] = PPS_VALUE_0;
    al_host_init_params[WLAN_CFG_INIT_PPS_THRESHOLD_LEVEL_1] = PPS_VALUE_1;
    al_host_init_params[WLAN_CFG_INIT_PPS_THRESHOLD_LEVEL_2] = PPS_VALUE_2;
    al_host_init_params[WLAN_CFG_INIT_PPS_THRESHOLD_LEVEL_3] = PPS_VALUE_3;
    al_host_init_params[WLAN_CFG_INIT_DEVICE_TYPE_LEVEL_0] = FREQ_IDLE;
    al_host_init_params[WLAN_CFG_INIT_DEVICE_TYPE_LEVEL_1] = FREQ_MIDIUM;
    al_host_init_params[WLAN_CFG_INIT_DEVICE_TYPE_LEVEL_2] = FREQ_HIGHER;
    al_host_init_params[WLAN_CFG_INIT_DEVICE_TYPE_LEVEL_3] = FREQ_HIGHEST;
    /* �͹��� */
    al_host_init_params[WLAN_CFG_INIT_POWERMGMT_SWITCH] = 1;
    al_host_init_params[WLAN_CFG_INIT_PS_MODE] = 1;
    /* ��ά�ɲ� ��־���� */
    al_host_init_params[WLAN_CFG_INIT_LOGLEVEL] = OAM_LOG_LEVEL_WARNING;

    host_params_init_first_of_rf();
    host_params_init_performace_throughtput();

    /* SCAN */
    al_host_init_params[WLAN_CFG_INIT_RANDOM_MAC_ADDR_SCAN] = 0;
    /* CONNECT */
    al_host_init_params[WLAN_CFG_INIT_RANDOM_MAC_ADDR_CONNECT] = 1;
    /* TAS SWITCH */
    al_host_init_params[WLAN_CFG_INIT_TAS_ANT_SWITCH] = OAL_FALSE;
    /* 11AC2G */
    al_host_init_params[WLAN_CFG_INIT_11AC2G_ENABLE] = 1;
    al_host_init_params[WLAN_CFG_INIT_DISABLE_CAPAB_2GHT40] = 0;
    al_host_init_params[WLAN_CFG_INIT_DUAL_ANTENNA_ENABLE] = 0;
    al_host_init_params[WLAN_CFG_INIT_FAR_DIST_POW_GAIN_SWITCH] = 1;
    al_host_init_params[WLAN_CFG_LTE_GPIO_CHECK_SWITCH] = 0;
    al_host_init_params[WLAN_ATCMDSRV_LTE_ISM_PRIORITY] = 0;
    al_host_init_params[WLAN_ATCMDSRV_LTE_RX_ACT] = 0;
    al_host_init_params[WLAN_ATCMDSRV_LTE_TX_ACT] = 0;
    al_host_init_params[WLAN_CFG_INIT_FAR_DIST_DSSS_SCALE_PROMOTE_SWITCH] = 1;
    host_params_init_cca();
    al_host_init_params[WLAN_CFG_INIT_DELTA_PWR_REF_2G_20M] = 0;
    al_host_init_params[WLAN_CFG_INIT_DELTA_PWR_REF_2G_40M] = 0;
    al_host_init_params[WLAN_CFG_INIT_DELTA_PWR_REF_5G_20M] = 0;
    al_host_init_params[WLAN_CFG_INIT_DELTA_PWR_REF_5G_40M] = 0;
    al_host_init_params[WLAN_CFG_INIT_DELTA_PWR_REF_5G_80M] = 0;

#ifdef _PRE_WLAN_DOWNLOAD_PM
    al_host_init_params[WLAN_CFG_INIT_DOWNLOAD_RATE_LIMIT_PPS] = 0;
#endif
    /* ��Ƶ�������� */
    al_host_init_params[WLAN_CFG_INIT_NBFH_TBTT_OFFSET] = WLAN_NBFH_TBTT_OFFSET;
    al_host_init_params[WLAN_CFG_INIT_NBFH_TBTT_SYNC_TIME] = WLAN_NBFH_TBTT_SYNC_TIME;
    al_host_init_params[WLAN_CFG_INIT_NBFH_DWELL_TIME] = WLAN_NBFH_DWELL_TIME;
    al_host_init_params[WLAN_CFG_INIT_NBFH_BEACON_TIME] = WLAN_NBFH_BEACON_TIME;

    /* INTERWORKING */
    al_host_init_params[WLAN_CFG_INIT_INTERWORKING_SWITCH] = WLAN_INTERWORKING_SWITCH;
#ifdef _PRE_WLAN_FEATURE_NRCOEX
    host_params_init_nrcoex_params();
#endif
}

/*
 * �� �� ��  : host_sar_params_init
 * ��������  : ��ʼ��SAR��ֵ
 */
OAL_STATIC oal_void host_sar_params_init(oal_void)
{
    al_nvram_init_params[NVRAM_PARAMS_SAR_5G_BANK1_0] = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SAR_5G_BANK2_0] = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SAR_5G_BANK3_0] = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SAR_5G_BANK4_0] = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SAR_5G_BANK5_0] = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SAR_5G_BANK6_0] = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SAR_5G_BANK7_0] = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SAR_2G_0]       = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SAR_5G_BANK1_1] = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SAR_5G_BANK2_1] = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SAR_5G_BANK3_1] = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SAR_5G_BANK4_1] = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SAR_5G_BANK5_1] = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SAR_5G_BANK6_1] = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SAR_5G_BANK7_1] = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SAR_2G_1]       = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SAR_5G_BANK1_2] = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SAR_5G_BANK2_2] = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SAR_5G_BANK3_2] = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SAR_5G_BANK4_2] = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SAR_5G_BANK5_2] = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SAR_5G_BANK6_2] = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SAR_5G_BANK7_2] = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SAR_2G_2]       = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SAR_5G_BANK1_3] = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SAR_5G_BANK2_3] = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SAR_5G_BANK3_3] = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SAR_5G_BANK4_3] = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SAR_5G_BANK5_3] = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SAR_5G_BANK6_3] = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SAR_5G_BANK7_3] = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SAR_2G_3]       = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SAR_5G_BANK1_4] = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SAR_5G_BANK2_4] = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SAR_5G_BANK3_4] = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SAR_5G_BANK4_4] = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SAR_5G_BANK5_4] = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SAR_5G_BANK6_4] = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SAR_5G_BANK7_4] = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SAR_2G_4]       = 0xFFFFFFFF;
}
/*
 * �� �� ��  : host_nvram_params_init
 * ��������  : �����ƻ�����ȫ������ al_nvram_init_params�й�����ز�������ֵ
 *             ini�ļ���ȡʧ��ʱ�ó�ֵ
 * �� �� ֵ  : OAL_SUCC �� ʧ�ܴ�����
 */
OAL_STATIC oal_void host_nvram_params_init(oal_void)
{
    /* delt power ��ʼ�� */
    al_nvram_init_params[NVRAM_PARAMS_INDEX_0]                 = 0x14141414;
    al_nvram_init_params[NVRAM_PARAMS_INDEX_1]                 = 0x050F1414;
    al_nvram_init_params[NVRAM_PARAMS_INDEX_2]                 = 0x05050505;
    al_nvram_init_params[NVRAM_PARAMS_INDEX_3]                 = 0xFB0005FB;
    al_nvram_init_params[NVRAM_PARAMS_INDEX_4]                 = 0xF1F1FB00;
    al_nvram_init_params[NVRAM_PARAMS_INDEX_5]                 = 0x00050500;
    al_nvram_init_params[NVRAM_PARAMS_INDEX_6]                 = 0x0505F6F6;
    al_nvram_init_params[NVRAM_PARAMS_INDEX_7]                 = 0xE2F1F1FB;
    al_nvram_init_params[NVRAM_PARAMS_INDEX_8]                 = 0xF1FBFBFB;
    al_nvram_init_params[NVRAM_PARAMS_INDEX_9]                 = 0xF100DDE7;
    al_nvram_init_params[NVRAM_PARAMS_INDEX_10]                = 0xE7F1F1F1;
    al_nvram_init_params[NVRAM_PARAMS_INDEX_11]                = 0x0000DDDD;
    /* 2.4Gխ���������ʳ�ʼ�� */
    al_nvram_init_params[NVRAM_PARAMS_TX_BASE_POWER_2P4G_NB]  = INIT_NVM_BASE_TXPWR_2G_NB;
    /* ��׼���� */
    al_nvram_init_params[NVRAM_PARAMS_TX_BASE_POWER_2P4G]     = INIT_NVM_BASE_TXPWR_2G;
    al_nvram_init_params[NVRAM_PARAMS_TX_BASE_POWER_5G_BAND1] = INIT_NVM_BASE_TXPWR_5G;
    al_nvram_init_params[NVRAM_PARAMS_TX_BASE_POWER_5G_BAND2] = INIT_NVM_BASE_TXPWR_5G;
    al_nvram_init_params[NVRAM_PARAMS_TX_BASE_POWER_5G_BAND3] = INIT_NVM_BASE_TXPWR_5G;
    al_nvram_init_params[NVRAM_PARAMS_TX_BASE_POWER_5G_BAND4] = INIT_NVM_BASE_TXPWR_5G;
    al_nvram_init_params[NVRAM_PARAMS_TX_BASE_POWER_5G_BAND5] = INIT_NVM_BASE_TXPWR_5G;
    al_nvram_init_params[NVRAM_PARAMS_TX_BASE_POWER_5G_BAND6] = INIT_NVM_BASE_TXPWR_5G;
    al_nvram_init_params[NVRAM_PARAMS_TX_BASE_POWER_5G_BAND7] = INIT_NVM_BASE_TXPWR_5G;
    /* 5G ��band����书�� */
    al_nvram_init_params[NVRAM_PARAMS_5G_HIGH_BAND_MAX_POW] = 0xFF;
    /* SARֵ */
    host_sar_params_init();
    /* �ߴ��ŵ������ */
    al_nvram_init_params[NVRAM_PARAMS_SIDE_BAND_TXPWR_5G_20M_0] = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SIDE_BAND_TXPWR_5G_20M_1] = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SIDE_BAND_TXPWR_5G_20M_2] = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SIDE_BAND_TXPWR_5G_40M_0] = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SIDE_BAND_TXPWR_5G_40M_1] = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SIDE_BAND_TXPWR_5G_80M_0] = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SIDE_BAND_TXPWR_5G_80M_1] = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SIDE_BAND_TXPWR_2G_1]  = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SIDE_BAND_TXPWR_2G_2]  = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SIDE_BAND_TXPWR_2G_3]  = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SIDE_BAND_TXPWR_2G_4]  = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SIDE_BAND_TXPWR_2G_5]  = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SIDE_BAND_TXPWR_2G_6]  = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SIDE_BAND_TXPWR_2G_7]  = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SIDE_BAND_TXPWR_2G_8]  = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SIDE_BAND_TXPWR_2G_9]  = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SIDE_BAND_TXPWR_2G_10] = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SIDE_BAND_TXPWR_2G_11] = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SIDE_BAND_TXPWR_2G_12] = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_SIDE_BAND_TXPWR_2G_13] = 0xFFFFFFFF;
    /* FCC CE�αߴ���������Ĭ��ֵ */
    al_nvram_init_params[NVRAM_PARAMS_EXT_SIDE_BAND_TXPWR_5G_20M] = 0xFFFFFFFF;
    al_nvram_init_params[NVRAM_PARAMS_EXT_SIDE_BAND_TXPWR_5G_40M] = 0xFFFFFFFF;
}

/*
 * �� �� ��  : hwifi_get_init_priv_value
 * ��������  : ��ȡ˽�ж��ƻ�����ֵ
 * �� �� ֵ  : OAL_SUCC �� ʧ�ܴ�����
 */
int32 hwifi_get_init_priv_value(oal_int32 l_cfg_id, oal_int32 *pl_priv_value)
{
    if ((l_cfg_id < 0) || (l_cfg_id >= WLAN_CFG_PRIV_BUTT)) {
        OAM_ERROR_LOG2(0, OAM_SF_ANY, "hwifi_get_init_priv_value cfg id[%d] out of range, max[%d]",
                       l_cfg_id, WLAN_CFG_PRIV_BUTT - 1);
        return OAL_FAIL;
    }

    *pl_priv_value = al_priv_cust_params[l_cfg_id].l_val;

    return OAL_SUCC;
}

/*
 * �� �� ��  : hwifi_get_regdomain_from_country_code
 * ��������  : ���ݹ������ҵ���Ӧ��regdomain
 * �������  : country_code ������
 * �� �� ֵ  : OAL_SUCC �� ʧ�ܴ�����
 */
regdomain_enum hwifi_get_regdomain_from_country_code(const countrycode_t country_code)
{
    regdomain_enum en_regdomain = REGDOMAIN_COMMON;
    int32 table_idx = 0;

    while (country_info_table[table_idx].en_regdomain != REGDOMAIN_COUNT) {
        if (oal_memcmp(country_code, country_info_table[table_idx].auc_country_code, COUNTRY_CODE_LEN) == 0) {
            en_regdomain = country_info_table[table_idx].en_regdomain;
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
 * �� �� ��  : hwifi_get_plat_tag_from_country_code
 * ��������  : ���ݹ������ҵ�ƽ̨��Ӧ��tag
 */
int32 hwifi_get_plat_tag_from_country_code(const countrycode_t country_code)
{
    regdomain_enum en_regdomain;
    int32 table_idx = 0;

    en_regdomain = hwifi_get_regdomain_from_country_code(country_code);

    while ((plat_tag_mapping_table[table_idx].en_regdomain != REGDOMAIN_COUNT)
           && (plat_tag_mapping_table[table_idx].plat_tag != INI_MODU_INVALID)) {
        /* matched */
        if (en_regdomain == plat_tag_mapping_table[table_idx].en_regdomain) {
            return plat_tag_mapping_table[table_idx].plat_tag;
        }

        ++table_idx;
    }

    /* not found, use common regdomain */
    return INI_MODU_WIFI;
}

/*
 * �� �� ��  : hwifi_custom_host_read_dyn_cali_nvram
 * ��������  : ������ȡnvram�е�dpint��У׼ϵ��ֵ
 */
oal_int32 hwifi_custom_host_read_dyn_cali_nvram(oal_void)
{
    oal_int32 l_ret;
    oal_uint8 uc_idx = WLAN_CFG_DTS_NVRAM_RATIO_PA2GCCKA0;
    oal_uint8 uc_param_idx;
    oal_uint8 uc_times_idx = 0;
    oal_int8 *puc_str = NULL;
    oal_uint8 *pc_end = ";";
    oal_uint8 *pc_sep = ",";
    oal_int8 *pc_ctx = NULL;
    oal_int8 *pc_token = NULL;
    oal_int32 l_priv_value;
    oal_bool_enum_uint8 en_get_nvram_data_flag = OAL_FALSE;
    oal_bool_enum_uint8 tmp_en_fact_cali_completed = OAL_FALSE;
    oal_uint8 *puc_buffer_cust_nvram_tmp = OAL_PTR_NULL;
    oal_int32 *pl_params = OAL_PTR_NULL;
    oal_uint8 *puc_cust_nvram_info = OAL_PTR_NULL; /* NVRAM���� */

    puc_buffer_cust_nvram_tmp = (oal_uint8 *)OS_KZALLOC_GFP(DY_CALI_PARAMS_LEN);
    pl_params = (oal_int32 *)OS_KZALLOC_GFP(DY_CALI_PARAMS_NUM * DY_CALI_PARAMS_TIMES * OAL_SIZEOF(oal_int32));
    puc_cust_nvram_info = (oal_uint8 *)OS_KZALLOC_GFP(WLAN_CFG_DTS_NVRAM_END * DY_CALI_PARAMS_LEN);
    if (OAL_ANY_NULL_PTR3(puc_buffer_cust_nvram_tmp, pl_params, puc_cust_nvram_info)) {
        OAM_ERROR_LOG3(0, OAM_SF_CUSTOM,
                       "hwifi_custom_host_read_dyn_cali_nvram::alloc failed.puc_buffer_cust_nvram_tmp[%x],pl_params[%x],puc_cust_nvram_info[%x]",
                       (uintptr_t)puc_buffer_cust_nvram_tmp,
                       (uintptr_t)pl_params,
                       (uintptr_t)puc_cust_nvram_info);
        if (puc_buffer_cust_nvram_tmp != OAL_PTR_NULL) {
            OS_MEM_KFREE(puc_buffer_cust_nvram_tmp);
        }

        if (pl_params != OAL_PTR_NULL) {
            OS_MEM_KFREE(pl_params);
        }

        if (puc_cust_nvram_info != OAL_PTR_NULL) {
            OS_MEM_KFREE(puc_cust_nvram_info);
        }
        /* ��ֹ��һ��ʧ�ܵĽ��Ӱ����һ�εĽ��������� */
        memset_s(auc_cust_nvram_info, OAL_SIZEOF(auc_cust_nvram_info), 0, OAL_SIZEOF(auc_cust_nvram_info));
        return INI_FILE_TIMESPEC_UNRECONFIG;
    }

    memset_s(puc_buffer_cust_nvram_tmp, DY_CALI_PARAMS_LEN, 0, DY_CALI_PARAMS_LEN);
    memset_s(pl_params, DY_CALI_PARAMS_NUM * DY_CALI_PARAMS_TIMES * OAL_SIZEOF(oal_int32),
             0, DY_CALI_PARAMS_NUM * DY_CALI_PARAMS_TIMES * OAL_SIZEOF(oal_int32));
    memset_s(puc_cust_nvram_info, WLAN_CFG_DTS_NVRAM_END * DY_CALI_PARAMS_LEN,
             0, WLAN_CFG_DTS_NVRAM_END * DY_CALI_PARAMS_LEN);

    /* �ж϶��ƻ����Ƿ�ʹ��nvram�еĶ�̬У׼���� */
    l_ret = hwifi_get_init_priv_value(WLAN_CFG_PRIV_CALI_DATA_MASK, &l_priv_value);
    if (l_ret == OAL_SUCC) {
        en_get_nvram_data_flag = !!(HI1102A_CUST_READ_NVRAM_MASK & (oal_uint32)l_priv_value);
        if (en_get_nvram_data_flag) {
            OAL_IO_PRINT("hwifi_custom_host_read_dyn_cali_nvram::get_nvram_data_flag[%d] to abandon nvram data!!\r\n",
                         l_priv_value);
            memset_s(auc_cust_nvram_info, OAL_SIZEOF(auc_cust_nvram_info), 0, OAL_SIZEOF(auc_cust_nvram_info));
            OS_MEM_KFREE(puc_buffer_cust_nvram_tmp);
            OS_MEM_KFREE(puc_cust_nvram_info);
            OS_MEM_KFREE(pl_params);
            return INI_FILE_TIMESPEC_UNRECONFIG;
        }
    }

    /* ���ϵ�� */
    for (uc_idx = WLAN_CFG_DTS_NVRAM_RATIO_PA2GCCKA0; uc_idx < WLAN_CFG_DTS_NVRAM_END; uc_idx++) {
        l_ret = read_conf_from_nvram(puc_buffer_cust_nvram_tmp, DY_CALI_PARAMS_LEN,
                                     wifi_nvram_cfg_handler[uc_idx].ul_nv_map_idx,
                                     wifi_nvram_cfg_handler[uc_idx].puc_nv_name);
        if (l_ret != INI_SUCC) {
            memset_s(puc_cust_nvram_info + (uc_idx * DY_CALI_PARAMS_LEN * OAL_SIZEOF(oal_uint8)),
                     DY_CALI_PARAMS_LEN * OAL_SIZEOF(oal_uint8), 0, DY_CALI_PARAMS_LEN * OAL_SIZEOF(oal_uint8));
            OAL_IO_PRINT("hwifi_custom_host_read_dyn_cali_nvram::NVRAM get fail NV id[%d] name[%s] para[%s]!\r\n",
                         wifi_nvram_cfg_handler[uc_idx].ul_nv_map_idx,
                         wifi_nvram_cfg_handler[uc_idx].puc_nv_name,
                         wifi_nvram_cfg_handler[uc_idx].puc_param_name);
            continue;
        }

        puc_str = OAL_STRSTR(puc_buffer_cust_nvram_tmp, wifi_nvram_cfg_handler[uc_idx].puc_param_name);
        if (puc_str == OAL_PTR_NULL) {
            memset_s(puc_cust_nvram_info + (uc_idx * DY_CALI_PARAMS_LEN * OAL_SIZEOF(oal_uint8)),
                     DY_CALI_PARAMS_LEN * OAL_SIZEOF(oal_uint8), 0, DY_CALI_PARAMS_LEN * OAL_SIZEOF(oal_uint8));
            OAL_IO_PRINT("hwifi_custom_host_read_dyn_cali_nvram::NVRAM get wrong val NV id[%d] name[%s] para[%s]!\r\n",
                         wifi_nvram_cfg_handler[uc_idx].ul_nv_map_idx,
                         wifi_nvram_cfg_handler[uc_idx].puc_nv_name,
                         wifi_nvram_cfg_handler[uc_idx].puc_param_name);
            continue;
        }

        /* ��ȡ�Ⱥź����ʵ�ʲ��� */
        puc_str += (OAL_STRLEN(wifi_nvram_cfg_handler[uc_idx].puc_param_name) + 1);
        pc_token = oal_strtok(puc_str, pc_end, &pc_ctx);
        if (pc_token == OAL_PTR_NULL) {
            OAM_ERROR_LOG1(0, OAM_SF_CUSTOM, "hwifi_custom_host_read_dyn_cali_nvram::get null value check NV id[%d]!",
                           wifi_nvram_cfg_handler[uc_idx].ul_nv_map_idx);
            OAL_IO_PRINT("hwifi_custom_host_read_dyn_cali_nvram::get null check NV id[%d] name[%s] para[%s]!\r\n",
                         wifi_nvram_cfg_handler[uc_idx].ul_nv_map_idx,
                         wifi_nvram_cfg_handler[uc_idx].puc_nv_name,
                         wifi_nvram_cfg_handler[uc_idx].puc_param_name);
            memset_s(puc_cust_nvram_info + (uc_idx * DY_CALI_PARAMS_LEN * OAL_SIZEOF(oal_uint8)),
                     DY_CALI_PARAMS_LEN * OAL_SIZEOF(oal_uint8), 0, DY_CALI_PARAMS_LEN * OAL_SIZEOF(oal_uint8));
            continue;
        }

        l_ret = memcpy_s(puc_cust_nvram_info + (uc_idx * DY_CALI_PARAMS_LEN * OAL_SIZEOF(oal_uint8)),
                         (WLAN_CFG_DTS_NVRAM_END - uc_idx) * DY_CALI_PARAMS_LEN,
                         pc_token, OAL_STRLEN(pc_token));
        if (l_ret != EOK) {
            OAM_ERROR_LOG1(0, OAM_SF_CUSTOM, "hwifi_custom_host_read_dyn_cali_nvram::memcpy fail![%d]", l_ret);
            OS_MEM_KFREE(puc_buffer_cust_nvram_tmp);
            OS_MEM_KFREE(puc_cust_nvram_info);
            OS_MEM_KFREE(pl_params);
            return INI_FILE_TIMESPEC_UNRECONFIG;
        }

        *(puc_cust_nvram_info + (uc_idx * DY_CALI_PARAMS_LEN * OAL_SIZEOF(oal_uint8)) + OAL_STRLEN(pc_token)) = *pc_end;

        /* ���ϵ����ȡ��� */
        if (uc_idx <= WLAN_CFG_DTS_NVRAM_RATIO_PA5GA0_LOW) {
            /* ���β��������Լ�� */
            pc_token = oal_strtok(pc_token, pc_sep, &pc_ctx);
            uc_param_idx = 0;
            /* ��ȡ���ƻ�ϵ�� */
            while (pc_token != OAL_PTR_NULL) {
                OAL_IO_PRINT("hwifi_custom_host_read_dyn_cali_nvram::get [%s]\n!", pc_token);
                /* ���ַ���ת����10������ */
                *(pl_params + uc_param_idx) = (oal_int32)oal_strtol(pc_token, OAL_PTR_NULL, 10);
                pc_token = oal_strtok(OAL_PTR_NULL, pc_sep, &pc_ctx);
                uc_param_idx++;
            }
            if (uc_param_idx % DY_CALI_PARAMS_TIMES) {
                OAM_ERROR_LOG1(0, OAM_SF_CUSTOM, "hwifi_custom_host_read_dyn_cali_nvram::check NV id[%d]!",
                               wifi_nvram_cfg_handler[uc_idx].ul_nv_map_idx);
                memset_s(puc_cust_nvram_info + (uc_idx * DY_CALI_PARAMS_LEN * OAL_SIZEOF(oal_uint8)),
                         DY_CALI_PARAMS_LEN * OAL_SIZEOF(oal_uint8), 0, DY_CALI_PARAMS_LEN * OAL_SIZEOF(oal_uint8));
                continue;
            }
            uc_times_idx = uc_param_idx / DY_CALI_PARAMS_TIMES;
            /* ������ϵ����0��� */
            while (uc_times_idx--) {
                if (pl_params[(uc_times_idx)*DY_CALI_PARAMS_TIMES] == 0) {
                    OAM_ERROR_LOG1(0, OAM_SF_CUSTOM, "hwifi_custom_host_read_dyn_cali_nvram::check NV id[%d]!",
                                   wifi_nvram_cfg_handler[uc_idx].ul_nv_map_idx);
                    memset_s(puc_cust_nvram_info + (uc_idx * DY_CALI_PARAMS_LEN * OAL_SIZEOF(oal_uint8)),
                             DY_CALI_PARAMS_LEN * OAL_SIZEOF(oal_uint8),
                             0, DY_CALI_PARAMS_LEN * OAL_SIZEOF(oal_uint8));
                    break;
                }
            }

            if (tmp_en_fact_cali_completed == OAL_FALSE) {
                tmp_en_fact_cali_completed = OAL_TRUE;
            }
        }
    }
    hwifi_set_fact_cali_completed(tmp_en_fact_cali_completed);
    OS_MEM_KFREE(puc_buffer_cust_nvram_tmp);
    OS_MEM_KFREE(pl_params);

    /* ���NVRAM�Ƿ��޸� */
    if (oal_memcmp(puc_cust_nvram_info, auc_cust_nvram_info, OAL_SIZEOF(auc_cust_nvram_info)) == 0) {
        OS_MEM_KFREE(puc_cust_nvram_info);
        return INI_FILE_TIMESPEC_UNRECONFIG;
    }

    l_ret = memcpy_s(auc_cust_nvram_info, OAL_SIZEOF(auc_cust_nvram_info),
                     puc_cust_nvram_info, OAL_SIZEOF(auc_cust_nvram_info));
    if (l_ret != EOK) {
        OAM_ERROR_LOG1(0, OAM_SF_CUSTOM, "hwifi_custom_host_read_dyn_cali_nvram::memcpy fail![%d]", l_ret);
        OS_MEM_KFREE(puc_cust_nvram_info);
        return INI_FILE_TIMESPEC_UNRECONFIG;
    }
    OS_MEM_KFREE(puc_cust_nvram_info);
    return INI_NVRAM_RECONFIG;
}

#ifdef _PRE_WLAN_FEATURE_NRCOEX
wlan_nrcoex_ini_stru* hwifi_get_nrcoex_ini(oal_void)
{
    return &g_st_nrcoex_ini;
}

oal_uint32 hwifi_get_nrcoex_rule_value(oal_int32 l_start_id, oal_uint8 uc_rule_index)
{
    oal_int32  l_cfg_id;
    oal_uint32 ul_value = 0;
    oal_int32  l_ret;

    l_cfg_id = l_start_id + (uc_rule_index * HAL_NRCOEX_RULE_PARAMS_NUM);
    if (g_st_nrcoex_ini.en_wifi_nrcoex_switch == OAL_TRUE) {
        l_ret = get_cust_conf_int32(INI_MODU_WIFI, wifi_config_cmds[l_cfg_id].name, &ul_value);
        if (l_ret != INI_SUCC) {
            OAM_WARNING_LOG2(0, 0, "hwifi_get_nrcoex_rule_value:read fail,cfg_id=%d,l_ret=%d", l_cfg_id, l_ret);
            ul_value = (oal_uint32)al_host_init_params[l_cfg_id];
        }
    } else {
        ul_value = (oal_uint32)al_host_init_params[l_cfg_id];
    }
    return ul_value;
}


oal_void hwifi_set_nrcoex_rule(wlan_nrcoex_rule_stru *pst_nrcoex_rule, oal_uint8 uc_rule_index)
{
    oal_uint32            ul_value;

    /* ��ȡ�������Ƶ�ʷ�Χ */
    /* ��16bitΪƵ�ʵ�����,��16bitΪƵ�ʵ����� */
    ul_value = hwifi_get_nrcoex_rule_value(WLAN_CFG_INIT_NRCOEX_RULE0_FREQ, uc_rule_index);
    pst_nrcoex_rule->us_high_freq_th = ((ul_value & 0xffff0000) >> 16);
    pst_nrcoex_rule->us_low_freq_th = (ul_value & 0xffff);
    /* ��ȡ�������20M��40M��0��Ƶ�ʲ�ֵ */
    /* ��16bitΪ40MƵ�ʲ�ֵ,��16bitΪ20MƵ�ʲ�ֵ */
    ul_value = hwifi_get_nrcoex_rule_value(WLAN_CFG_INIT_NRCOEX_RULE0_40M_20M_GAP0, uc_rule_index);
    pst_nrcoex_rule->ast_threshold_params[WLAN_BW_CAP_20M].us_relative_freq_gap0 = (ul_value & 0xffff);
    pst_nrcoex_rule->ast_threshold_params[WLAN_BW_CAP_40M].us_relative_freq_gap0 = ((ul_value & 0xffff0000) >> 16);
    /* ��ȡ��������80M��160M��0��Ƶ�ʲ�ֵ */
    /* ��16bitΪ160MƵ�ʲ�ֵ,��16bitΪ80MƵ�ʲ�ֵ */
    ul_value = hwifi_get_nrcoex_rule_value(WLAN_CFG_INIT_NRCOEX_RULE0_160M_80M_GAP0, uc_rule_index);
    pst_nrcoex_rule->ast_threshold_params[WLAN_BW_CAP_80M].us_relative_freq_gap0 = (ul_value & 0xffff);
    pst_nrcoex_rule->ast_threshold_params[WLAN_BW_CAP_160M].us_relative_freq_gap0 = ((ul_value & 0xffff0000) >> 16);
    /* ��ȡ�������20M��40M��1��Ƶ�ʲ�ֵ */
    /* ��16bitΪ40MƵ�ʲ�ֵ,��16bitΪ20MƵ�ʲ�ֵ */
    ul_value = hwifi_get_nrcoex_rule_value(WLAN_CFG_INIT_NRCOEX_RULE0_40M_20M_GAP1, uc_rule_index);
    pst_nrcoex_rule->ast_threshold_params[WLAN_BW_CAP_20M].us_relative_freq_gap1 = (ul_value & 0xffff);
    pst_nrcoex_rule->ast_threshold_params[WLAN_BW_CAP_40M].us_relative_freq_gap1 = ((ul_value & 0xffff0000) >> 16);
    /* ��ȡ��������80M��160M��1��Ƶ�ʲ�ֵ */
    /* ��16bitΪ160MƵ�ʲ�ֵ,��16bitΪ80MƵ�ʲ�ֵ */
    ul_value = hwifi_get_nrcoex_rule_value(WLAN_CFG_INIT_NRCOEX_RULE0_160M_80M_GAP1, uc_rule_index);
    pst_nrcoex_rule->ast_threshold_params[WLAN_BW_CAP_80M].us_relative_freq_gap1 = (ul_value & 0xffff);
    pst_nrcoex_rule->ast_threshold_params[WLAN_BW_CAP_160M].us_relative_freq_gap1 = ((ul_value & 0xffff0000) >> 16);
    /* ��ȡ�������20M��40M��2��Ƶ�ʲ�ֵ */
    /* ��16bitΪ40MƵ�ʲ�ֵ,��16bitΪ20MƵ�ʲ�ֵ */
    ul_value = hwifi_get_nrcoex_rule_value(WLAN_CFG_INIT_NRCOEX_RULE0_40M_20M_GAP2, uc_rule_index);
    pst_nrcoex_rule->ast_threshold_params[WLAN_BW_CAP_20M].us_relative_freq_gap2 = (ul_value & 0xffff);
    pst_nrcoex_rule->ast_threshold_params[WLAN_BW_CAP_40M].us_relative_freq_gap2 = ((ul_value & 0xffff0000) >> 16);
    /* ��ȡ��������80M��160M��2��Ƶ�ʲ�ֵ */
    /* ��16bitΪ160MƵ�ʲ�ֵ,��16bitΪ80MƵ�ʲ�ֵ */
    ul_value = hwifi_get_nrcoex_rule_value(WLAN_CFG_INIT_NRCOEX_RULE0_160M_80M_GAP2, uc_rule_index);
    pst_nrcoex_rule->ast_threshold_params[WLAN_BW_CAP_80M].us_relative_freq_gap2 = (ul_value & 0xffff);
    pst_nrcoex_rule->ast_threshold_params[WLAN_BW_CAP_160M].us_relative_freq_gap2 = ((ul_value & 0xffff0000) >> 16);
    /* ��ȡ��0���Ĺ��ʿ���ֵ */
    /* ������������ֵ,bit31~bit24Ϊ160M,bit23~bit16Ϊ80M,bit15~bit8Ϊ40M,bit7~bit0Ϊ20M */
    ul_value = hwifi_get_nrcoex_rule_value(WLAN_CFG_INIT_NRCOEX_RULE0_GAP0_ACT, uc_rule_index);
    pst_nrcoex_rule->ast_threshold_params[WLAN_BW_CAP_20M].uc_limit_power_gear0 = (ul_value & 0xff);
    pst_nrcoex_rule->ast_threshold_params[WLAN_BW_CAP_40M].uc_limit_power_gear0 = ((ul_value & 0xff00) >> 8);
    pst_nrcoex_rule->ast_threshold_params[WLAN_BW_CAP_80M].uc_limit_power_gear0 = ((ul_value & 0xff0000) >> 16);
    pst_nrcoex_rule->ast_threshold_params[WLAN_BW_CAP_160M].uc_limit_power_gear0 = ((ul_value & 0xff000000) >> 24);
    /* ��ȡ��1���Ĺ��ʿ���ֵ */
    /* ������������ֵ,bit31~bit24Ϊ160M,bit23~bit16Ϊ80M,bit15~bit8Ϊ40M,bit7~bit0Ϊ20M */
    ul_value = hwifi_get_nrcoex_rule_value(WLAN_CFG_INIT_NRCOEX_RULE0_GAP1_ACT, uc_rule_index);
    pst_nrcoex_rule->ast_threshold_params[WLAN_BW_CAP_20M].uc_limit_power_gear1 = (ul_value & 0xff);
    pst_nrcoex_rule->ast_threshold_params[WLAN_BW_CAP_40M].uc_limit_power_gear1 = ((ul_value & 0xff00) >> 8);
    pst_nrcoex_rule->ast_threshold_params[WLAN_BW_CAP_80M].uc_limit_power_gear1 = ((ul_value & 0xff0000) >> 16);
    pst_nrcoex_rule->ast_threshold_params[WLAN_BW_CAP_160M].uc_limit_power_gear1 = ((ul_value & 0xff000000) >> 24);
    /* ��ȡ��2���Ĺ��ʿ���ֵ */
    /* ������������ֵ,bit31~bit24Ϊ160M,bit23~bit16Ϊ80M,bit15~bit8Ϊ40M,bit7~bit0Ϊ20M */
    ul_value = hwifi_get_nrcoex_rule_value(WLAN_CFG_INIT_NRCOEX_RULE0_GAP2_ACT, uc_rule_index);
    pst_nrcoex_rule->ast_threshold_params[WLAN_BW_CAP_20M].uc_limit_power_gear2 = (ul_value & 0xff);
    pst_nrcoex_rule->ast_threshold_params[WLAN_BW_CAP_40M].uc_limit_power_gear2 = ((ul_value & 0xff00) >> 8);
    pst_nrcoex_rule->ast_threshold_params[WLAN_BW_CAP_80M].uc_limit_power_gear2 = ((ul_value & 0xff0000) >> 16);
    pst_nrcoex_rule->ast_threshold_params[WLAN_BW_CAP_160M].uc_limit_power_gear2 = ((ul_value & 0xff000000) >> 24);
    /* ��ȡRSSI����ֵ */
    /* ��������RSSI����ֵ,bit31~bit24Ϊ160M,bit23~bit16Ϊ80M,bit15~bit8Ϊ40M,bit7~bit0Ϊ20M */
    ul_value = hwifi_get_nrcoex_rule_value(WLAN_CFG_INIT_NRCOEX_RULE0_RXSLOT_RSSI, uc_rule_index);
    pst_nrcoex_rule->ast_threshold_params[WLAN_BW_CAP_20M].c_rssi_th = (ul_value & 0xff);
    pst_nrcoex_rule->ast_threshold_params[WLAN_BW_CAP_40M].c_rssi_th = ((ul_value & 0xff00) >> 8);
    pst_nrcoex_rule->ast_threshold_params[WLAN_BW_CAP_80M].c_rssi_th = ((ul_value & 0xff0000) >> 16);
    pst_nrcoex_rule->ast_threshold_params[WLAN_BW_CAP_160M].c_rssi_th = ((ul_value & 0xff000000) >> 24);
}


oal_uint32 hwifi_config_read_nrcoex_params(oal_void)
{
    int32                  l_ret = INI_FAILED;
    oal_uint32             ul_value = 0;
    oal_uint8              uc_rule_index = 0;
    wlan_nrcoex_rule_stru *pst_nrcoex_rule = OAL_PTR_NULL;

    /* ��ȡnr���濪�� */
    l_ret = get_cust_conf_int32(INI_MODU_WIFI, wifi_config_cmds[WLAN_CFG_INIT_NRCOEX_ENABLE].name, &ul_value);
    if (l_ret != INI_SUCC) {
        OAM_WARNING_LOG0(0, 0, "hwifi_config_init_nrcoex_params:read switch fail");
        ul_value = (oal_uint32)al_host_init_params[WLAN_CFG_INIT_NRCOEX_ENABLE];
    }
    g_st_nrcoex_ini.en_wifi_nrcoex_switch = (ul_value == 0 ? OAL_FALSE : OAL_TRUE);
    /* ��ȡ������� */
    for (uc_rule_index = 0; uc_rule_index < HAL_NRCOEX_RULE_NUM; uc_rule_index++) {
        pst_nrcoex_rule = &g_st_nrcoex_ini.ast_nrcoex_rule[uc_rule_index];
        hwifi_set_nrcoex_rule(pst_nrcoex_rule, uc_rule_index);
    }
    return OAL_SUCC;
}
#endif

oal_uint8 *hwifi_get_nvram_param(oal_uint32 ul_nvram_param_idx)
{
    return auc_cust_nvram_info[ul_nvram_param_idx];
}

/*
 * �� �� ��  : hwifi_custom_host_force_read_cfg_init
 * ��������  : �״ζ�ȡ���ƻ������ļ������
 */
int32 hwifi_custom_host_force_read_cfg_init(void)
{
    oal_int32 l_nv_read_ret;
    oal_int32 l_ini_read_ret;

    /* �Ȼ�ȡ˽�ж��ƻ��� */
    hwifi_config_init(CUS_TAG_PRIV_INI);

    /* ��ȡnvram�����Ƿ��޸� */
    l_nv_read_ret = hwifi_custom_host_read_dyn_cali_nvram();
    /* ��鶨�ƻ��ļ��еĲ��������Ƿ��޸� */
    l_ini_read_ret = ini_file_check_conf_update();
    if (l_ini_read_ret || l_nv_read_ret) {
        OAM_WARNING_LOG0(0, OAM_SF_CFG, "hwifi_custom_host_force_read_cfg_init file is updated");
        hwifi_config_init(CUS_TAG_PRO_LINE_INI);
    }

    if (l_ini_read_ret == INI_FILE_TIMESPEC_UNRECONFIG) {
        OAM_WARNING_LOG0(0, OAM_SF_CFG, "hwifi_custom_host_force_read_cfg_init file is not updated");
    }

    hwifi_config_init(CUS_TAG_DTS);

    l_ini_read_ret = hwifi_config_init(CUS_TAG_NV);
    if (OAL_UNLIKELY(l_ini_read_ret != OAL_SUCC)) {
        OAL_IO_PRINT("hwifi_custom_host_force_read_cfg_init NV fail l_ret[%d].\r\n", l_ini_read_ret);
    }

    hwifi_config_init(CUS_TAG_INI);

    /* ������ɺ������ӡ */
    OAL_IO_PRINT("hwifi_custom_host_force_read_cfg_init finish!\r\n");

    return OAL_SUCC;
}

/*
 * �� �� ��  : hwifi_custom_host_read_cfg_init
 * ��������  : �״ζ�ȡ���ƻ������ļ������
 */
int32 hwifi_custom_host_read_cfg_init(void)
{
    oal_int32 l_nv_read_ret;
    oal_int32 l_ini_read_ret;

    /* �Ȼ�ȡ˽�ж��ƻ��� */
    hwifi_config_init(CUS_TAG_PRIV_INI);
#ifdef _PRE_WLAN_FEATURE_NRCOEX
    hwifi_config_read_nrcoex_params();
#endif
    /* ��ȡnvram�����Ƿ��޸� */
    l_nv_read_ret = hwifi_custom_host_read_dyn_cali_nvram();
    /* ��鶨�ƻ��ļ��еĲ��������Ƿ��޸� */
    l_ini_read_ret = ini_file_check_conf_update();
    if (l_ini_read_ret || l_nv_read_ret) {
        OAM_WARNING_LOG0(0, OAM_SF_CFG, "hwifi_custom_host_read_cfg_init file is updated");
        hwifi_config_init(CUS_TAG_PRO_LINE_INI);
    }

    if (l_ini_read_ret == INI_FILE_TIMESPEC_UNRECONFIG) {
        OAM_WARNING_LOG0(0, OAM_SF_CFG, "hwifi_custom_host_read_cfg_init file is not updated");
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    hwifi_config_init(CUS_TAG_DTS);

    l_ini_read_ret = hwifi_config_init(CUS_TAG_NV);
    if (OAL_UNLIKELY(l_ini_read_ret != OAL_SUCC)) {
        OAL_IO_PRINT("hwifi_custom_host_read_cfg_init NV fail l_ret[%d].\r\n", l_ini_read_ret);
    }
    hwifi_config_init(CUS_TAG_INI);
    /* ������ɺ������ӡ */
    OAL_IO_PRINT("hwifi_custom_host_read_cfg_init finish!\r\n");

    return OAL_SUCC;
}

/*
 * �� �� ��  : hwifi_config_init_delt_txpower_nvram
 * ��������  : ��SAR
 * �� �� ֵ  : OAL_SUCC �� ʧ�ܴ�����
 */
OAL_STATIC oal_int32 hwifi_config_init_delt_txpower_nvram(void)
{
    int32 l_ret = INI_FAILED;
    int32 l_cfg_id;
    int32 aul_nvram_params[NVRAM_PARAMS_INDEX_BUTT] = {0};
    int32 l_plat_tag;

    l_plat_tag = hwifi_get_plat_tag_from_country_code(hwifi_get_country_code());
    OAM_WARNING_LOG1(0, OAM_SF_ANY, "hwifi_config_init_delt_txpower_nvram plat_tag:0x%2x!", l_plat_tag);

    for (l_cfg_id = NVRAM_PARAMS_DELT_POWER_START; l_cfg_id <= NVRAM_PARAMS_DELT_POWER_END; l_cfg_id++) {
        if (l_cfg_id != nvram_config_ini[l_cfg_id].case_entry) {
            OAM_ERROR_LOG2(0, OAM_SF_ANY, "hwifi_config_init_delt_txpower_nvram:l_cfg_id=%d,case_entry=%d",
                           l_cfg_id, nvram_config_ini[l_cfg_id].case_entry);
        }
        l_ret = get_cust_conf_int32(l_plat_tag, nvram_config_ini[l_cfg_id].name, &aul_nvram_params[l_cfg_id]);
        if (l_ret == INI_FAILED) {
            OAM_WARNING_LOG1(0, OAM_SF_ANY, "hwifi_config_init_delt_txpower_nvram read %d from ini failed!", l_cfg_id);
            /* ��ȡʧ�ܾ�ʹ��ϵͳĬ��ֵ */
            aul_nvram_params[l_cfg_id] = al_nvram_init_params[l_cfg_id];
        }
    }

    l_ret = memcpy_s(cust_nv_params.ac_delt_txpower, sizeof(cust_nv_params.ac_delt_txpower),
                     aul_nvram_params, sizeof(cust_nv_params.ac_delt_txpower));
    if (l_ret != EOK) {
        OAM_ERROR_LOG1(0, OAM_SF_CUSTOM, "hwifi_config_init_delt_txpower_nvram::memcpy fail![%d]", l_ret);
        return INI_FAILED;
    }
    return INI_SUCC;
}
OAL_STATIC oal_void hwifi_config_init_check_sar_lvl_idx(oal_uint32 ul_cus_id, oal_uint8 uc_cfg_id,
    oal_uint8 uc_band_id, oal_uint32 ul_nvram_params)
{
    oal_uint8 uc_sar_lvl_idx;
    oal_uint8 auc_nvram_params[CUS_NUM_OF_SAR_ONE_PARAM_NUM];
    oal_int32 l_ret;

    l_ret = memcpy_s(auc_nvram_params, OAL_SIZEOF(auc_nvram_params),
                     &ul_nvram_params, OAL_SIZEOF(ul_nvram_params));
    if (l_ret != EOK) {
        OAM_ERROR_LOG1(0, OAM_SF_CUSTOM, "hwifi_config_init_check_sar_lvl_idx::memcpy fail![%d]", l_ret);
    }

    for (uc_sar_lvl_idx = 0; uc_sar_lvl_idx < CUS_NUM_OF_SAR_ONE_PARAM_NUM; uc_sar_lvl_idx++) {
        /* �������� */
        if (auc_nvram_params[uc_sar_lvl_idx] < CUS_MIN_OF_SAR_VAL) {
            OAM_WARNING_LOG2(0, OAM_SF_CUSTOM, "hwifi_config_init_check_sar_lvl_idx:uc_cfg_id[%d], sar_val=%d",
                             ul_cus_id, auc_nvram_params[uc_sar_lvl_idx]);
        }
        auc_sar_params[uc_sar_lvl_idx + uc_cfg_id * CUS_NUM_OF_SAR_ONE_PARAM_NUM][uc_band_id] =
            auc_nvram_params[uc_sar_lvl_idx];
    }
}
/*
 * �� �� ��  : hwifi_config_init_sar_ctrl_nvram
 * ��������  : ��SAR
 * �� �� ֵ  : OAL_SUCC �� ʧ�ܴ�����
 */
OAL_STATIC oal_int32 hwifi_config_init_sar_ctrl_nvram(void)
{
    oal_int32 l_ret = INI_FAILED;
    oal_uint8 uc_cfg_id;
    oal_uint8 uc_band_id;
    oal_uint32 ul_cus_id = NVRAM_PARAMS_SAR_START;
    oal_uint32 ul_nvram_params = 0;
    oal_int32 l_plat_tag;

    l_plat_tag = hwifi_get_plat_tag_from_country_code(hwifi_get_country_code());
    OAM_WARNING_LOG1(0, OAM_SF_ANY, "hwifi_config_init_sar_ctrl_nvram plat_tag:0x%2x!", l_plat_tag);

    for (uc_cfg_id = 0; uc_cfg_id < CUS_NUM_OF_SAR_PER_BAND_PAR_NUM; uc_cfg_id++) {
        for (uc_band_id = 0; uc_band_id < CUS_NUM_OF_SAR_PARAMS; uc_band_id++) {
            l_ret = get_cust_conf_int32(l_plat_tag, nvram_config_ini[ul_cus_id].name, &ul_nvram_params);
            if (l_ret != INI_SUCC) {
                OAM_WARNING_LOG1(0, OAM_SF_CFG, "hwifi_config_init_sar_ctrl_nvram read ini failed id[%d]!", ul_cus_id);
                /* ��ȡʧ��ʱ,ʹ�ó�ʼֵ */
                ul_nvram_params = al_nvram_init_params[ul_cus_id];
            }

            /* �������� */
            hwifi_config_init_check_sar_lvl_idx(ul_cus_id, uc_cfg_id, uc_band_id, ul_nvram_params);
            ul_cus_id++;
        }
    }

    return INI_SUCC;
}

/*
 * �� �� ��  : hwifi_config_init_5g_high_band_max_pow_nvram
 * ��������  : �������ƻ��е�5G��band����͹���
 * �� �� ֵ  : OAL_SUCC �� ʧ�ܴ�����
 */
oal_void hwifi_config_init_5g_high_band_max_pow_nvram(oal_void)
{
    int32 l_cfg_id = NVRAM_PARAMS_5G_HIGH_BAND_MAX_POW;
    int32 l_val = 0xFF;
    int32 l_ret;
    oal_uint8 uc_5g_high_band_max_pow;
    oal_int32 l_plat_tag;

    l_plat_tag = hwifi_get_plat_tag_from_country_code(hwifi_get_country_code());
    OAM_WARNING_LOG1(0, OAM_SF_ANY, "hwifi_config_init_5g_high_band_max_pow_nvram plat_tag:0x%2x!", l_plat_tag);

    l_ret = get_cust_conf_int32(l_plat_tag, nvram_config_ini[l_cfg_id].name, &l_val);
    if ((l_ret == INI_FAILED) || (l_val < CUS_MIN_BASE_TXPOWER_VAL)) {
        /* ��ȡʧ�ܾ�ʹ��ϵͳĬ��ֵ */
        uc_5g_high_band_max_pow = (oal_uint8)al_nvram_init_params[l_cfg_id];
    } else {
        uc_5g_high_band_max_pow = (oal_uint8)l_val;
    }

    cust_nv_params.uc_5g_high_band_max_pow = uc_5g_high_band_max_pow;
}

/*
 * �� �� ��  : hwifi_config_hitalk_base_power
 * ��������  : խ��ģʽ���滻2.4G��������
 * �� �� ֵ  : OAL_SUCC �� ʧ�ܴ�����
 */
oal_void hwifi_config_hitalk_base_power(oal_int32 l_plat_tag)
{
    oal_int32 l_base_power_tmp = 0;
    oal_int32 l_ret;
    l_ret = get_cust_conf_int32(l_plat_tag, nvram_config_ini[NVRAM_PARAMS_TX_BASE_POWER_2P4G_NB].name,
                                &l_base_power_tmp);

    if ((l_ret == INI_FAILED) || (l_base_power_tmp > MAX_TXPOWER_MAX) || (l_base_power_tmp < MAX_TXPOWER_MIN)) {
        /* ��ȡʧ�ܾ�ʹ��ϵͳĬ��ֵ */
        cust_nv_params.uc_nb_base_power = al_nvram_init_params[NVRAM_PARAMS_TX_BASE_POWER_2P4G_NB];
    } else {
        cust_nv_params.uc_nb_base_power = l_base_power_tmp;
    }
}


/*
 * �� �� ��  : hwifi_config_init_base_power
 * ��������  : �������ƻ��е�base power
 * �� �� ֵ  : OAL_SUCC �� ʧ�ܴ�����
 */
oal_int32 hwifi_config_init_base_power(oal_void)
{
    int32 l_cfg_id;
    int32 l_base_power_idx = 0;
    int32 l_base_power_tmp = 0;
    int32 l_ret = INI_FAILED;
    oal_uint8 auc_base_power[CUS_BASE_POWER_NUM];
    oal_int32 l_plat_tag;

    l_plat_tag = hwifi_get_plat_tag_from_country_code(hwifi_get_country_code());
    OAM_WARNING_LOG1(0, OAM_SF_ANY, "hwifi_config_init_base_power plat_tag:0x%2x!", l_plat_tag);
    for (l_cfg_id = NVRAM_PARAMS_TX_BASE_POWER_START; l_cfg_id <= NVRAM_PARAMS_TX_BASE_POWER_END; l_cfg_id++) {
        l_ret = get_cust_conf_int32(l_plat_tag, nvram_config_ini[l_cfg_id].name, &l_base_power_tmp);
        if ((l_ret == INI_FAILED) || (l_base_power_tmp > MAX_TXPOWER_MAX) || (l_base_power_tmp < MAX_TXPOWER_MIN)) {
            /* ��ȡʧ�ܾ�ʹ��ϵͳĬ��ֵ */
            auc_base_power[l_base_power_idx] = al_nvram_init_params[l_cfg_id];
        } else {
            auc_base_power[l_base_power_idx] = l_base_power_tmp;
        }
        l_base_power_idx++;
    }

    l_ret = memcpy_s(cust_nv_params.auc_base_power, OAL_SIZEOF(cust_nv_params.auc_base_power),
                     auc_base_power, OAL_SIZEOF(oal_uint8) * CUS_BASE_POWER_NUM);
    if (l_ret != EOK) {
        OAM_ERROR_LOG1(0, OAM_SF_CUSTOM, "hwifi_config_init_base_power::memcpy fail![%d]", l_ret);
        return INI_FAILED;
    }

    /* խ��ģʽ���滻2.4G�������� */
    hwifi_config_hitalk_base_power(l_plat_tag);
    return INI_SUCC;
}

/*
 * �� �� ��  : hwifi_config_init_fcc_ce_txpwr_nvram
 * ��������  : �������ƻ���FCC��CE�Ĺ�������ֵ
 * �� �� ֵ  : OAL_SUCC �� ʧ�ܴ�����
 */
oal_int32 hwifi_config_init_fcc_ce_txpwr_nvram(oal_void)
{
    int32 l_ret = INI_FAILED;
    uint32 ul_cfg_id;
    uint8 uc_param_idx = 0;
    int32 *pl_nvram_params = OAL_PTR_NULL;
    int32 *pl_fcc_txpwr_limit_params = OAL_PTR_NULL;
    oal_uint32 ul_param_len = (NVRAM_PARAMS_SIDE_BAND_TXPWR_END - NVRAM_PARAMS_SIDE_BAND_TXPWR_START + 1) *
                              OAL_SIZEOF(int32);
    oal_int32 l_plat_tag;
    oal_uint8 uc_loop;

    l_plat_tag = hwifi_get_plat_tag_from_country_code(hwifi_get_country_code());
    OAM_WARNING_LOG1(0, OAM_SF_ANY, "hwifi_config_init_fcc_ce_txpwr_nvram plat_tag:0x%2x!", l_plat_tag);
    pl_fcc_txpwr_limit_params = (int32 *)kzalloc(ul_param_len, (GFP_KERNEL | __GFP_NOWARN));
    if (pl_fcc_txpwr_limit_params == OAL_PTR_NULL) {
        OAM_ERROR_LOG1(0, OAM_SF_CUSTOM, "hwifi_config_init_fcc_ce_txpwr_nvram::mem alloc [%d] fail!", ul_param_len);
        return INI_FAILED;
    }
    memset_s(pl_fcc_txpwr_limit_params, ul_param_len, 0, ul_param_len);
    pl_nvram_params = pl_fcc_txpwr_limit_params;
    for (ul_cfg_id = NVRAM_PARAMS_SIDE_BAND_TXPWR_START; ul_cfg_id <= NVRAM_PARAMS_SIDE_BAND_TXPWR_END;
         ul_cfg_id++) {
        l_ret = get_cust_conf_int32(l_plat_tag, nvram_config_ini[ul_cfg_id].name, pl_nvram_params + uc_param_idx);
        if (l_ret != INI_SUCC) {
            OAM_WARNING_LOG1(0, OAM_SF_CFG, "hwifi_config_init_fcc_ce_txpwr_nvram read ini failed id[%d]!", ul_cfg_id);
            /* ��ȡʧ��ʱ,ʹ�ó�ʼֵ */
            pl_nvram_params[uc_param_idx] = al_nvram_init_params[ul_cfg_id];
        }
        uc_param_idx++;
    }
    memset_s(&cust_nv_params.st_fcc_ce_txpwer_limit, OAL_SIZEOF(wlan_fcc_ce_power_limit_stru),
             0, OAL_SIZEOF(wlan_fcc_ce_power_limit_stru));
    /* 5g */
    /* CH140/CH100/CH64/CH36 */
    l_ret = memcpy_s(cust_nv_params.st_fcc_ce_txpwer_limit.auc_5g_txpwr_20M, FCC_CE_CH_NUM_5G_20M,
                     pl_nvram_params, OAL_SIZEOF(int32));
    pl_nvram_params++;
    /* CH161/CH157/CH153/CH149 */
    l_ret += memcpy_s(cust_nv_params.st_fcc_ce_txpwer_limit.auc_5g_txpwr_20M + OAL_SIZEOF(int32),
                      FCC_CE_CH_NUM_5G_20M - OAL_SIZEOF(int32),
                      pl_nvram_params, OAL_SIZEOF(int32));
    pl_nvram_params++;
    /* CH165 */ /* ƫ���Ѿ����������ݵ�2*4�ֽڣ�ʣ�೤����buff�ܳ��ȼ�ȥ�ѿ��������ݵ�2*4�ֽ� */
    l_ret += memcpy_s(cust_nv_params.st_fcc_ce_txpwer_limit.auc_5g_txpwr_20M + 2 * OAL_SIZEOF(int32),
                      FCC_CE_CH_NUM_5G_20M - 2 * OAL_SIZEOF(int32),
                      pl_nvram_params, OAL_SIZEOF(oal_uint8));
    pl_nvram_params++;
    /* CH134/CH102/CH62/CH38 */
    l_ret += memcpy_s(cust_nv_params.st_fcc_ce_txpwer_limit.auc_5g_txpwr_40M, FCC_CE_CH_NUM_5G_40M,
                      pl_nvram_params, OAL_SIZEOF(int32));
    pl_nvram_params++;
    /* CH159/CH151 */ /* ֻ����2�ֽ����ݣ���Ϊ֮ǰ�Ѿ�����4�ֽڣ�buff��Сֻʣ6-4=2���ֽ� */
    l_ret += memcpy_s(cust_nv_params.st_fcc_ce_txpwer_limit.auc_5g_txpwr_40M + OAL_SIZEOF(int32),

                      FCC_CE_CH_NUM_5G_40M - OAL_SIZEOF(int32),
                      pl_nvram_params, 2 * OAL_SIZEOF(oal_uint8));
    pl_nvram_params++;
    /* CH122/CH106/CH58/CH42 */
    l_ret += memcpy_s(cust_nv_params.st_fcc_ce_txpwer_limit.auc_5g_txpwr_80M, FCC_CE_CH_NUM_5G_80M,
                      pl_nvram_params, OAL_SIZEOF(int32));
    pl_nvram_params++;
    /* CH155 */
    l_ret += memcpy_s(cust_nv_params.st_fcc_ce_txpwer_limit.auc_5g_txpwr_80M + OAL_SIZEOF(int32),
                      FCC_CE_CH_NUM_5G_80M - OAL_SIZEOF(int32),
                      pl_nvram_params, OAL_SIZEOF(oal_uint8));
    if (l_ret != EOK) {
        OAM_ERROR_LOG1(0, OAM_SF_CUSTOM, "hwifi_config_init_fcc_ce_txpwr_nvram::memcpy fail![%d]", l_ret);
        kfree(pl_fcc_txpwr_limit_params);
        return INI_FAILED;
    }
    /* 2.4g */
    for (uc_loop = 0; uc_loop < MAC_2G_CHANNEL_NUM; uc_loop++) {
        pl_nvram_params++;

        l_ret = memcpy_s(cust_nv_params.st_fcc_ce_txpwer_limit.auc_2g_txpwr[uc_loop],
                         FCC_CE_SIG_TYPE_NUM_2G * OAL_SIZEOF(oal_uint8),
                         pl_nvram_params, FCC_CE_SIG_TYPE_NUM_2G * OAL_SIZEOF(oal_uint8));
        if (l_ret != EOK) {
            OAM_ERROR_LOG1(0, OAM_SF_CUSTOM, "hwifi_config_init_fcc_ce_txpwr_nvram::memcpy fail![%d]", l_ret);
            kfree(pl_fcc_txpwr_limit_params);
            return INI_FAILED;
        }
    }
    kfree(pl_fcc_txpwr_limit_params);
    return INI_SUCC;
}

/*
 * �� �� ��  : hwifi_config_read_ext_fcc_ce_txpwr
 * ��������  : �������ƻ���FCC��CE�αߴ��ŵ��Ĺ�������ֵ
 * �� �� ֵ  : OAL_SUCC �� ʧ�ܴ�����
 */
oal_int32 hwifi_config_read_ext_fcc_ce_txpwr(oal_void)
{
    oal_int32                         l_plat_tag;
    oal_int32                         l_value = 0xFFFFFFFF;
    oal_int32                         l_ret;

    l_plat_tag = hwifi_get_plat_tag_from_country_code(hwifi_get_country_code());
    l_ret = get_cust_conf_int32(l_plat_tag, nvram_config_ini[NVRAM_PARAMS_EXT_SIDE_BAND_TXPWR_5G_20M].name, &l_value);
    if (l_ret != INI_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_CUSTOM, "hwifi_config_read_ext_fcc_ce_txpwr:get 20M ext value fail,ret=%d", l_ret);
        l_value = al_nvram_init_params[NVRAM_PARAMS_EXT_SIDE_BAND_TXPWR_5G_20M];
    }
    /* 136/104/60/40 20M */
    l_ret = memcpy_s(cust_nv_params.st_ext_fcc_ce_txpwer_limit.auc_5g_txpwr_20M, FCC_CE_EXT_CH_NUM_5G_20M,
                     &l_value, OAL_SIZEOF(int32));
    if (l_ret != EOK) {
        OAM_ERROR_LOG1(0, OAM_SF_CUSTOM, "hwifi_config_read_ext_fcc_ce_txpwr:memcpy fail,l_ret=%d", l_ret);
        return INI_FAILED;
    }
    l_ret = get_cust_conf_int32(l_plat_tag, nvram_config_ini[NVRAM_PARAMS_EXT_SIDE_BAND_TXPWR_5G_40M].name, &l_value);
    if (l_ret != INI_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_CUSTOM, "hwifi_config_read_ext_fcc_ce_txpwr:get 40M ext value fail,ret=%d", l_ret);
        l_value = al_nvram_init_params[NVRAM_PARAMS_EXT_SIDE_BAND_TXPWR_5G_40M];
    }
    /* 126/110/54/46 40M */
    l_ret = memcpy_s(cust_nv_params.st_ext_fcc_ce_txpwer_limit.auc_5g_txpwr_40M, FCC_CE_EXT_CH_NUM_5G_40M,
                     &l_value, OAL_SIZEOF(int32));
    if (l_ret != EOK) {
        OAM_ERROR_LOG1(0, OAM_SF_CUSTOM, "hwifi_config_read_ext_fcc_ce_txpwr:memcpy fail,l_ret=%d", l_ret);
        return INI_FAILED;
    }
    return INI_SUCC;
}
/*
 * �� �� ��  : hwifi_get_gnss_scan_by_i3c
 * ��������  : ��ȡplat ini�ļ��е�i3c_switch����������ÿ���wifi��gnss scan�Ŀ���
 */
oal_uint8 hwifi_get_gnss_scan_by_i3c(void)
{
    oal_uint8 uc_i3c_switch;
    uc_i3c_switch = !!get_i3c_switch_mode();

    OAM_WARNING_LOG1(0, OAM_SF_CFG, "hwifi_get_gnss_scan_by_i3c :: read i3c from ini [%d]..", uc_i3c_switch);
    return uc_i3c_switch;
}

/*
 * �� �� ��  : hwifi_get_gnss_scan_by_i3c
 * ��������  : ��ȡplat ini�ļ��е�i3c_switch����������ÿ���wifi��gnss scan�Ŀ���
 */
oal_bool_enum_uint8 hwifi_get_tas_state(void)
{
    return g_aen_tas_switch_en[WITP_RF_CHANNEL_ZERO];
}

/*
 * �� �� ��  : hwifi_config_init_nvram
 * ��������  : handle nvram customize params
 * �� �� ֵ  : OAL_SUCC �� ʧ�ܴ�����
 */
OAL_STATIC int32 hwifi_config_init_nvram(void)
{
    OAL_STATIC oal_bool_enum en_nvm_initialed = OAL_FALSE; /* �Ƿ�Ϊ��һ�γ�ʼ��������ǹ�������µ��õı��ӿڣ�����ȥnvm��ȡ���� */
    int32                    l_cfg_id;
    int32                    l_ret = INI_FAILED;

    memset_s(&cust_nv_params, OAL_SIZEOF(cust_nv_params), 0, OAL_SIZEOF(cust_nv_params));
    if (en_nvm_initialed == OAL_FALSE) {
        if (hwifi_get_regdomain_from_country_code(hwifi_get_country_code()) != REGDOMAIN_FCC) {
            l_ret = get_cust_conf_string(CUST_MODU_NVRAM, OAL_PTR_NULL, auc_nv_params, sizeof(auc_nv_params));
            if ((l_ret == INI_SUCC) && (auc_nv_params[0] != 0)) {
                en_nvm_initialed = OAL_TRUE; /* ��ȡ�ɹ�������־λ��TRUE */
                return INI_SUCC;
            }
            OAM_WARNING_LOG3 (0, OAM_SF_ANY,
                              "hwifi_config_init_nvram read nvram failed[ret:%d] or wrong values[first eight values:0x%x %x], read dts instead!",
                              l_ret, *((oal_uint32 *)auc_nv_params),
                              *((oal_uint32 *)(auc_nv_params + sizeof(uint32)))); /* ��ӡǰ8�ֽڵ�ֵ */
        } else {
            en_nvm_initialed = OAL_TRUE;
        }
    }
    /* ���cfg_id�Ƿ���ȷ���� */
    for (l_cfg_id = NVRAM_PARAMS_INDEX_0; l_cfg_id < NVRAM_PARAMS_INDEX_BUTT; l_cfg_id++) {
        if (nvram_config_ini[l_cfg_id].case_entry != l_cfg_id) {
            OAM_ERROR_LOG2(0, OAM_SF_ANY, "hwifi_config_init_nvram:nvram_config_ini case_entry[%d]!=l_cfg_id[%d]",
                           nvram_config_ini[l_cfg_id].case_entry, l_cfg_id);
        }
    }
    l_ret = hwifi_config_init_delt_txpower_nvram(); /* delt txpower */
    l_ret += hwifi_config_init_sar_ctrl_nvram(); /* SAR */
    l_ret += hwifi_config_init_base_power(); /* ��׼���� */
    /* 5G ��band����书�� */
    hwifi_config_init_5g_high_band_max_pow_nvram();
    /* FCC/CE �ߴ��ŵ������ */
    l_ret += hwifi_config_init_fcc_ce_txpwr_nvram();
    l_ret += hwifi_config_read_ext_fcc_ce_txpwr();
    if (l_ret != INI_SUCC) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "hwifi_config_init_nvram:ini fail,l_ret=%d", l_ret);
        return l_ret;
    }
    return INI_SUCC;
}

/*
 * �� �� ��  : hwifi_config_init_private_custom
 * ��������  : ��ʼ��˽�ж���ȫ�ֱ�������
 * �� �� ֵ  : OAL_SUCC �� ʧ�ܴ�����
 */
OAL_STATIC int32 hwifi_config_init_private_custom(void)
{
    int32 l_cfg_id;
    int32 l_ret = INI_FAILED;

    for (l_cfg_id = 0; l_cfg_id < WLAN_CFG_PRIV_BUTT; l_cfg_id++) {
        if (l_cfg_id != wifi_config_priv[l_cfg_id].case_entry) {
            OAM_ERROR_LOG2(0, OAM_SF_CFG, "hwifi_config_init_private_custom:l_cfg_id[%d]!=case_entry[%d]",
                           l_cfg_id, wifi_config_priv[l_cfg_id].case_entry);
        }
        /* ��ȡ private ������ֵ */
        l_ret = get_cust_conf_int32(INI_MODU_WIFI, wifi_config_priv[l_cfg_id].name,
                                    &(al_priv_cust_params[l_cfg_id].l_val));

        if (l_ret == INI_FAILED) {
            al_priv_cust_params[l_cfg_id].l_val = al_priv_cust_init_params[l_cfg_id];
            OAM_WARNING_LOG2(0, OAM_SF_CFG, "hwifi_config_init_private_custom:cfg_id[%d]used default value[%d]",
                             l_cfg_id, al_priv_cust_init_params[l_cfg_id]);
            al_priv_cust_params[l_cfg_id].en_value_state = OAL_FALSE;
            continue;
        }
        al_priv_cust_params[l_cfg_id].en_value_state = OAL_TRUE;
    }
    OAM_WARNING_LOG0(0, OAM_SF_CFG, "hwifi_config_init_private_custom read from ini success!");

    return INI_SUCC;
}

/*
 * �� �� ��  : hwifi_config_sepa_coefficient_from_param
 * ��������  : ���ַ����зָ����ϵ����
 * �� �� ֵ  : OAL_SUCC �� ʧ�ܴ�����
 */
OAL_STATIC oal_uint32 hwifi_config_sepa_coefficient_from_param(const char *puc_cust_param_info,
                                                               oal_int32 *pl_coe_params,
                                                               oal_uint16 *pus_param_num,
                                                               oal_uint16 us_max_idx)
{
    oal_int8 *pc_token = NULL;
    oal_int8 *pc_ctx = NULL;
    oal_int8 *pc_end = ";";
    oal_int8 *pc_sep = ",";
    oal_uint16 us_param_num = 0;
    oal_uint8 auc_cust_param[DY_CALI_PARAMS_LEN];
    oal_int32 l_ret;

    l_ret = memcpy_s(auc_cust_param, OAL_SIZEOF(auc_cust_param), puc_cust_param_info, OAL_STRLEN(puc_cust_param_info));
    if (l_ret != EOK) {
        OAM_ERROR_LOG1(0, OAM_SF_CUSTOM, "hwifi_config_sepa_coefficient_from_param::memcpy fail![%d]", l_ret);
        return OAL_FAIL;
    }

    pc_token = oal_strtok(auc_cust_param, pc_end, &pc_ctx);
    if (pc_token == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_CUSTOM, "hwifi_config_sepa_coefficient_from_param read get null value check!");
        return OAL_ERR_CODE_PTR_NULL;
    }
    pc_token = oal_strtok(pc_token, pc_sep, &pc_ctx);
    /* ��ȡ���ƻ�ϵ�� */
    while (pc_token != OAL_PTR_NULL) {
        if (us_param_num == us_max_idx) {
            OAM_ERROR_LOG2(0, OAM_SF_CUSTOM,
                           "hwifi_config_sepa_coefficient_from_param::nv or ini param is too many idx[%d] Max[%d]",
                           us_param_num, us_max_idx);
            return OAL_FAIL;
        }
        OAL_IO_PRINT("hwifi_config_sepa_coefficient_from_param get [%s]\n!", pc_token);
        /* ���ַ���ת����10������ */
        *(pl_coe_params + us_param_num) = (oal_int32)oal_strtol(pc_token, OAL_PTR_NULL, 10);
        pc_token = oal_strtok(OAL_PTR_NULL, pc_sep, &pc_ctx);
        us_param_num++;
    }

    if (us_param_num % DY_CALI_PARAMS_TIMES) {
        OAM_ERROR_LOG1(0, OAM_SF_CUSTOM, "hwifi_config_sepa_coefficient_from_param read get wrong num[%d] check!",
                       us_param_num);
        return OAL_FAIL;
    }

    *pus_param_num = us_param_num;
    return OAL_SUCC;
}

/*
 * �� �� ��  : hwifi_config_get_5g_curv_switch_point
 * ��������  : ����ini�ļ���ȡ5G�������߹����л���
 * �� �� ֵ  : OAL_SUCC �� ʧ�ܴ�����
 */
OAL_STATIC oal_void hwifi_config_get_5g_curv_switch_point(const char *puc_ini_pa_params, oal_uint32 ul_cfg_id)
{
    oal_int32 l_ini_params[CUS_NUM_5G_BW * DY_CALI_PARAMS_TIMES] = {0};
    oal_uint16 us_ini_param_num = 0;
    oal_uint8 uc_secon_ratio_idx = 0;
    oal_uint8 uc_param_idx;
    oal_int16 *ps_extre_point_val = NULL;

    if ((ul_cfg_id != WLAN_CFG_DTS_NVRAM_RATIO_PA5GA0) && (ul_cfg_id != WLAN_CFG_DTS_NVRAM_RATIO_PA5GA0_BAND1)) {
        return;
    }
    /* ��ȡ���ϵ���� */
    if (hwifi_config_sepa_coefficient_from_param(puc_ini_pa_params, l_ini_params, &us_ini_param_num,
                                                 OAL_SIZEOF(l_ini_params) / OAL_SIZEOF(oal_int32)) != OAL_SUCC) {
        OAM_ERROR_LOG2(0, OAM_SF_CUSTOM,
                       "hwifi_config_get_5g_curv_switch_point::ini is unsuitable,num of ini[%d] cfg_id[%d]!",
                       us_ini_param_num, ul_cfg_id);
        return;
    }

    ps_extre_point_val = gs_extre_point_vals;
    us_ini_param_num /= DY_CALI_PARAMS_TIMES;
    if (ul_cfg_id <= WLAN_CFG_DTS_NVRAM_RATIO_PA5GA0) {
        if (us_ini_param_num != CUS_NUM_5G_BW) {
            OAM_ERROR_LOG2(0, OAM_SF_CUSTOM,
                           "hwifi_config_get_5g_curv_switch_point::ul_cfg_id[%d] us_ini_param_num[%d]",
                           ul_cfg_id, us_ini_param_num);
            return;
        }
        /* ��һ����������band1 */
        ps_extre_point_val++;
    } else {
        if (us_ini_param_num != 1) {
            OAM_ERROR_LOG2(0, OAM_SF_CUSTOM,
                           "hwifi_config_get_5g_curv_switch_point::ul_cfg_id[%d] us_ini_param_num[%d]",
                           ul_cfg_id, us_ini_param_num);
            return;
        }
    }

    /* ����5g����switch point */
    for (uc_param_idx = 0; uc_param_idx < us_ini_param_num; uc_param_idx++) {
        *(ps_extre_point_val + uc_param_idx) = (oal_int16)HWIFI_DYN_CALI_GET_EXTRE_POINT(l_ini_params +
                                                                                         uc_secon_ratio_idx);
        OAL_IO_PRINT("hwifi_config_get_5g_curv_switch_point::extre power[%d] param_idx[%d] cfg_id[%d]!\r\n",
                     *(ps_extre_point_val + uc_param_idx), uc_param_idx, ul_cfg_id);
        OAL_IO_PRINT("hwifi_config_get_5g_curv_switch_point::param[%d %d] uc_secon_ratio_idx[%d]!\r\n",
                     (l_ini_params + uc_secon_ratio_idx)[0], (l_ini_params + uc_secon_ratio_idx)[1],
                     uc_secon_ratio_idx);
        uc_secon_ratio_idx += DY_CALI_PARAMS_TIMES;
    }

    return;
}

/*
 * �� �� ��  : hwifi_config_nvram_second_coefficient_check
 * ��������  : �������nvram�еĶ���ϵ���Ƿ����
 * �� �� ֵ  : OAL_SUCC �� ʧ�ܴ�����
 */
OAL_STATIC oal_uint32 hwifi_config_nvram_second_coef_check(oal_uint8 *puc_cust_nvram_info,
                                                           oal_uint8 *puc_ini_pa_params, oal_uint32 ul_pa_len,
                                                           oal_uint32 ul_cfg_id,
                                                           oal_int16 *ps_5g_delt_power, oal_uint32 ul_delt_len)
{
    oal_int32 l_ini_params[CUS_NUM_5G_BW * DY_CALI_PARAMS_TIMES] = {0};
    oal_int32 l_nv_params[CUS_NUM_5G_BW * DY_CALI_PARAMS_TIMES] = {0};
    oal_uint16 us_ini_param_num = 0;
    oal_uint16 us_nv_param_num = 0;
    oal_uint8 uc_secon_ratio_idx = 0;
    oal_uint8 uc_param_idx;

    /* ��ȡ���ϵ���� */
    if (hwifi_config_sepa_coefficient_from_param(puc_cust_nvram_info, l_nv_params, &us_nv_param_num,
                                                 OAL_SIZEOF(l_nv_params)) != OAL_SUCC ||
        hwifi_config_sepa_coefficient_from_param(puc_ini_pa_params, l_ini_params,
                                                 &us_ini_param_num, OAL_SIZEOF(l_ini_params)) != OAL_SUCC ||
        (us_nv_param_num != us_ini_param_num)) {
        OAM_ERROR_LOG2(0, OAM_SF_CUSTOM,
            "hwifi_config_nvram_second_coefficient_check::nvram or ini is unsuitable,num of nv and ini[%d %d]!",
            us_nv_param_num, us_ini_param_num);
        return OAL_FAIL;
    }

    us_nv_param_num /= DY_CALI_PARAMS_TIMES;
    /* ���nv��ini�ж���ϵ���Ƿ�ƥ�� */
    for (uc_param_idx = 0; uc_param_idx < us_nv_param_num; uc_param_idx++) {
        if (l_ini_params[uc_secon_ratio_idx] != l_nv_params[uc_secon_ratio_idx]) {
            OAM_WARNING_LOG4(0, OAM_SF_CUSTOM,
                             "hwifi_config_nvram_second_coefficient_check::nvram get mismatch value idx[%d %d] val are [%d] and [%d]!",
                             uc_param_idx,
                             uc_secon_ratio_idx,
                             l_ini_params[uc_secon_ratio_idx],
                             l_nv_params[uc_secon_ratio_idx]);
            /* ���������ϵ����nvram��Ϊ׼��ˢ��NV�еĶ�����������л��� */
            hwifi_config_get_5g_curv_switch_point(puc_cust_nvram_info, ul_cfg_id);
            uc_secon_ratio_idx += DY_CALI_PARAMS_TIMES;
            continue;
        }

        if (ul_cfg_id == WLAN_CFG_DTS_NVRAM_RATIO_PA5GA0) {
            /* ��������ϵ�delt power */
            *(ps_5g_delt_power + uc_param_idx) = HWIFI_GET_5G_PRO_LINE_DELT_POW_PER_BAND(l_nv_params +
                                                                                         uc_secon_ratio_idx,
                                                                                         l_ini_params +
                                                                                         uc_secon_ratio_idx);
            OAL_IO_PRINT("hwifi_config_nvram_second_coefficient_check::delt power[%d] param_idx[%d] cfg_id[%d]!\r\n",
                         *(ps_5g_delt_power + uc_param_idx), uc_param_idx, ul_cfg_id);
        }
        uc_secon_ratio_idx += DY_CALI_PARAMS_TIMES;
    }
    return OAL_SUCC;
}

/*
 * �� �� ��  : hwifi_config_init_dy_cali_custom
 * ��������  : ��ȡ���ƻ��ļ��Ͳ���ϵ��
 * �� �� ֵ  : OAL_SUCC �� ʧ�ܴ�����
 */
OAL_STATIC oal_uint32 hwifi_config_init_dy_cali_custom(oal_void)
{
    oal_uint32 ul_cfg_id;
    oal_uint32 ul_ret = OAL_SUCC;
    oal_uint8 auc_ini_pa_params[DY_CALI_PARAMS_LEN] = {0};
    oal_uint8 uc_idx = 0;
    oal_uint16 us_param_num = 0;
    oal_uint16 us_per_param_num = 0;
    oal_uint8 uc_cali_param_idx;
    oal_int32 l_params[DY_CALI_PARAMS_TIMES * DY_CALI_PARAMS_NUM] = {0};
    oal_uint8 *puc_cust_nvram_info = OAL_PTR_NULL;
    oal_int16 s_5g_delt_power[CUS_NUM_5G_BW] = {0};
    oal_uint8 uc_delt_pwr_idx = 0;
    oal_uint32 ul_cfg_id_tmp;
    oal_int32 l_ret;

    for (ul_cfg_id = WLAN_CFG_DTS_NVRAM_RATIO_PA2GCCKA0; ul_cfg_id < WLAN_CFG_DTS_NVRAM_PARAMS_BUTT; ul_cfg_id++) {
        /* �������ϵ�� */
        if (get_cust_conf_string(INI_MODU_WIFI, nvram_pro_line_config_ini[ul_cfg_id].name,
                                 auc_ini_pa_params, DY_CALI_PARAMS_LEN) == INI_FAILED) {
            if (OAL_VALUE_EQ_ANY2(ul_cfg_id, WLAN_CFG_DTS_NVRAM_RATIO_PA5GA0_BAND1_LOW,
                                  WLAN_CFG_DTS_NVRAM_RATIO_PA5GA0_LOW)) {
                ul_cfg_id_tmp = ((ul_cfg_id == WLAN_CFG_DTS_NVRAM_RATIO_PA5GA0_BAND1_LOW) ?
                                 WLAN_CFG_DTS_NVRAM_RATIO_PA5GA0_BAND1 :
                                 (ul_cfg_id == WLAN_CFG_DTS_NVRAM_RATIO_PA5GA0_LOW) ?
                                 WLAN_CFG_DTS_NVRAM_RATIO_PA5GA0 : ul_cfg_id);
                get_cust_conf_string(INI_MODU_WIFI, nvram_pro_line_config_ini[ul_cfg_id_tmp].name,
                                     auc_ini_pa_params, DY_CALI_PARAMS_LEN);
            } else {
                OAM_ERROR_LOG1(0, OAM_SF_CUSTOM, "hwifi_config_init_dy_cali_custom read, check id[%d] exists!",
                               ul_cfg_id);
                ul_ret = OAL_FAIL;
                break;
            }
        }

        if (OAL_VALUE_EQ_ANY2(ul_cfg_id, WLAN_CFG_DTS_NVRAM_RATIO_PA5GA0, WLAN_CFG_DTS_NVRAM_RATIO_PA5GA0_BAND1)) {
            /* ��ȡini�еĶ�����������л��� */
            hwifi_config_get_5g_curv_switch_point(auc_ini_pa_params, ul_cfg_id);
        }

        if (ul_cfg_id <= WLAN_CFG_DTS_NVRAM_RATIO_PA5GA0_LOW) {
            puc_cust_nvram_info = hwifi_get_nvram_param(ul_cfg_id);
            /* ��ȡnv�еĲ���ֵ,Ϊ�����ini�ļ��ж�ȡ */
            if (OAL_STRLEN(puc_cust_nvram_info)) {
                /* NVRAM����ϵ���쳣���� */
                if (hwifi_config_nvram_second_coef_check(puc_cust_nvram_info, auc_ini_pa_params, DY_CALI_PARAMS_LEN,
                                                         ul_cfg_id, s_5g_delt_power, CUS_NUM_5G_BW) == OAL_SUCC) {
                    if ((ul_cfg_id == WLAN_CFG_DTS_NVRAM_RATIO_PA5GA0_LOW) &&
                        (memcmp(puc_cust_nvram_info, auc_ini_pa_params, OAL_STRLEN(puc_cust_nvram_info)))) {
                        memset_s(s_5g_delt_power, OAL_SIZEOF(s_5g_delt_power), 0, OAL_SIZEOF(s_5g_delt_power));
                    }

                    l_ret = memcpy_s(auc_ini_pa_params, OAL_SIZEOF(auc_ini_pa_params),
                                     puc_cust_nvram_info, OAL_STRLEN(puc_cust_nvram_info));
                    if (l_ret != EOK) {
                        OAM_ERROR_LOG1(0, OAM_SF_CUSTOM, "hwifi_config_init_dy_cali_custom::memcpy fail![%d]", l_ret);
                        return OAL_FAIL;
                    }
                } else {
                    ul_ret = OAL_FAIL;
                    break;
                }
            } else {
                /* �ṩ���ߵ�һ���ϵ�У׼��ʼֵ */
                l_ret = memcpy_s(puc_cust_nvram_info, DY_CALI_PARAMS_LEN,
                                 auc_ini_pa_params, OAL_STRLEN(auc_ini_pa_params));
                if (l_ret != EOK) {
                    OAM_ERROR_LOG1(0, OAM_SF_CUSTOM, "hwifi_config_init_dy_cali_custom::memcpy fail![%d]", l_ret);
                    return OAL_FAIL;
                }
            }
        }

        if (hwifi_config_sepa_coefficient_from_param(auc_ini_pa_params,
                                                     l_params + us_param_num,
                                                     &us_per_param_num,
                                                     OAL_SIZEOF(l_params) - us_param_num) != OAL_SUCC) {
            ul_ret = OAL_FAIL;
            OAM_ERROR_LOG2(0, OAM_SF_CUSTOM,
                           "hwifi_config_init_dy_cali_custom read get wrong value,len[%d] check id[%d] exists!",
                           OAL_STRLEN(puc_cust_nvram_info), ul_cfg_id);
            break;
        }
        us_param_num += us_per_param_num;
    }

    if (ul_ret == OAL_FAIL) {
        /* �����ֹ�·���device */
        memset_s(as_pro_line_params, OAL_SIZEOF(as_pro_line_params), 0, OAL_SIZEOF(as_pro_line_params));

    } else {
        if (us_param_num != OAL_SIZEOF(l_params) / OAL_SIZEOF(oal_int32)) {
            OAM_ERROR_LOG1(0, OAM_SF_CUSTOM, "hwifi_config_init_dy_cali_custom read get wrong ini value num[%d]!",
                           us_param_num);

            memset_s(as_pro_line_params, OAL_SIZEOF(as_pro_line_params), 0, OAL_SIZEOF(as_pro_line_params));
            return OAL_FAIL;
        }
        /* l_params �ǰ���cfg id��˳��ȡ�����ݣ�as_pro_line_params�ǰ��� device������˳������ */
        for (uc_cali_param_idx = 0; uc_cali_param_idx < DY_CALI_PARAMS_BASE_NUM; uc_cali_param_idx++) {
            if (uc_cali_param_idx == DY_2G_CALI_PARAMS_NUM - 1) {
                /* 5g band1 & 2g CW */
                uc_cali_param_idx += PRO_LINE_2G_TO_5G_OFFSET;
            }
            as_pro_line_params[uc_cali_param_idx].l_pow_par2 = l_params[uc_idx++];
            as_pro_line_params[uc_cali_param_idx].l_pow_par1 = l_params[uc_idx++];
            as_pro_line_params[uc_cali_param_idx].l_pow_par0 = l_params[uc_idx++];
        }
        /* 5g band2&3 4&5 6 7 low power */
        for (uc_cali_param_idx = DY_CALI_PARAMS_BASE_NUM + 1; uc_cali_param_idx < DY_CALI_PARAMS_NUM_WLAN - 1;
             uc_cali_param_idx++) {
            as_pro_line_params[uc_cali_param_idx].l_pow_par2 = l_params[uc_idx++];
            as_pro_line_params[uc_cali_param_idx].l_pow_par1 = l_params[uc_idx++];
            as_pro_line_params[uc_cali_param_idx].l_pow_par0 = l_params[uc_idx++];

            CUS_FLUSH_NV_RATIO_BY_DELT_POW(as_pro_line_params[uc_cali_param_idx].l_pow_par2,
                                           as_pro_line_params[uc_cali_param_idx].l_pow_par1,
                                           as_pro_line_params[uc_cali_param_idx].l_pow_par0,
                                           s_5g_delt_power[uc_delt_pwr_idx]);

            uc_delt_pwr_idx++;
        }

        /* 5g band1 */
        as_pro_line_params[DY_2G_CALI_PARAMS_NUM].l_pow_par2 = l_params[uc_idx++];
        as_pro_line_params[DY_2G_CALI_PARAMS_NUM].l_pow_par1 = l_params[uc_idx++];
        as_pro_line_params[DY_2G_CALI_PARAMS_NUM].l_pow_par0 = l_params[uc_idx++];

        /* 2g CW */
        as_pro_line_params[DY_2G_CALI_PARAMS_NUM - 1].l_pow_par2 = l_params[uc_idx++];
        as_pro_line_params[DY_2G_CALI_PARAMS_NUM - 1].l_pow_par1 = l_params[uc_idx++];
        as_pro_line_params[DY_2G_CALI_PARAMS_NUM - 1].l_pow_par0 = l_params[uc_idx++];

        /* 5g band1 low power */
        as_pro_line_params[DY_CALI_PARAMS_BASE_NUM].l_pow_par2 = l_params[uc_idx++];
        as_pro_line_params[DY_CALI_PARAMS_BASE_NUM].l_pow_par1 = l_params[uc_idx++];
        as_pro_line_params[DY_CALI_PARAMS_BASE_NUM].l_pow_par0 = l_params[uc_idx++];

        /* 2g cw ppa */
        as_pro_line_params[DY_CALI_PARAMS_NUM_WLAN - 1].l_pow_par2 = l_params[uc_idx++];
        as_pro_line_params[DY_CALI_PARAMS_NUM_WLAN - 1].l_pow_par1 = l_params[uc_idx++];
        as_pro_line_params[DY_CALI_PARAMS_NUM_WLAN - 1].l_pow_par0 = l_params[uc_idx++];

        // BT power fit params
        as_pro_line_params[DY_CALI_PARAMS_NUM_WLAN].l_pow_par2 = l_params[uc_idx++];
        as_pro_line_params[DY_CALI_PARAMS_NUM_WLAN].l_pow_par1 = l_params[uc_idx++];
        as_pro_line_params[DY_CALI_PARAMS_NUM_WLAN].l_pow_par0 = l_params[uc_idx++];
        // BT PPA Vdet fit params
        as_pro_line_params[DY_CALI_PARAMS_NUM_WLAN + 1].l_pow_par2 = l_params[uc_idx++];
        as_pro_line_params[DY_CALI_PARAMS_NUM_WLAN + 1].l_pow_par1 = l_params[uc_idx++];
        as_pro_line_params[DY_CALI_PARAMS_NUM_WLAN + 1].l_pow_par0 = l_params[uc_idx++];
    }

    return ul_ret;
}

/*
 * �� �� ��  : hwifi_config_sepa_dpn_from_param
 * ��������  : Separate DPN values from INI item.
 */
OAL_STATIC oal_uint32 hwifi_config_sepa_dpn_from_param(oal_uint8 *puc_cust_param_info, oal_int32 *pl_dpn_params,
                                                       oal_uint16 *pus_param_num, oal_uint16 us_max_idx)
{
    oal_int8 *pc_token = NULL;
    char *pc_ctx = NULL;
    oal_int8 *pc_end = ";";
    oal_int8 *pc_sep = ",";
    oal_uint16 us_param_num = 0;
    oal_uint8 auc_cust_param[DY_CALI_PARAMS_LEN];
    oal_int32 l_ret;

    l_ret = memcpy_s(auc_cust_param, OAL_SIZEOF(auc_cust_param), puc_cust_param_info, OAL_STRLEN(puc_cust_param_info));
    if (l_ret != EOK) {
        OAM_ERROR_LOG1(0, OAM_SF_CUSTOM, "hwifi_config_sepa_dpn_from_param::memcpy fail![%d]", l_ret);
        return OAL_FAIL;
    }

    pc_token = strtok_s(auc_cust_param, pc_end, &pc_ctx);
    if (pc_token == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_CUSTOM, "hwifi_config_sepa_dpn_from_param read get null value check!");
        return OAL_ERR_CODE_PTR_NULL;
    }
    pc_token = strtok_s(pc_token, pc_sep, &pc_ctx);
    /* ��ȡ���ƻ�ϵ�� */
    while (pc_token != NULL) {
        if (us_param_num == us_max_idx) {
            OAM_ERROR_LOG2(0, OAM_SF_CUSTOM,
                           "hwifi_config_sepa_dpn_from_param::ini item has too many parameters[%d] max[%d]",
                           us_param_num, us_max_idx);
            return OAL_FAIL;
        }
        OAL_IO_PRINT("hwifi_config_sepa_dpn_from_param get [%s]\n!", pc_token);
        /* ���ַ���ת����10������ */
        *(pl_dpn_params + us_param_num) = (oal_int32)oal_strtol(pc_token, OAL_PTR_NULL, 10);
        pc_token = strtok_s(OAL_PTR_NULL, pc_sep, &pc_ctx);
        us_param_num++;
    }

    *pus_param_num = us_param_num;
    return OAL_SUCC;
}

/*
 * �� �� ��  : hwifi_config_get_bt_dpn
 * ��������  : Get BT DPN values.
 */
oal_uint32 hwifi_config_get_bt_dpn(oal_int32 *pl_params, oal_uint16 size)
{
    oal_uint32 ul_cfg_id;
    oal_uint8 auc_ini_params[DY_CALI_PARAMS_LEN] = {0};
    oal_uint16 us_param_num = 0;
    oal_uint16 us_per_param_num = 0;

    for (ul_cfg_id = WLAN_CFG_DTS_BT_CALI_TXPWR_DPN_BAND1; ul_cfg_id <= WLAN_CFG_DTS_BT_CALI_TXPWR_DPN_BAND8;
         ul_cfg_id++) {
        if (get_cust_conf_string(INI_MODU_WIFI, wifi_config_dts[ul_cfg_id].name,
                                 auc_ini_params, DY_CALI_PARAMS_LEN) == INI_FAILED) {
            OAM_ERROR_LOG1(0, OAM_SF_CUSTOM, "hwifi_config_init_dts_cali read, check id[%d] exists!", ul_cfg_id);
            return OAL_TRUE;
            break;
        }

        if (hwifi_config_sepa_dpn_from_param(auc_ini_params, pl_params + us_param_num,
                                             &us_per_param_num, size - us_param_num) != OAL_SUCC) {
            OAM_ERROR_LOG2(0, OAM_SF_CUSTOM,
                           "hwifi_config_init_dts_cali read get wrong value,len[%d] check id[%d] exists!",
                           OAL_STRLEN(auc_ini_params), ul_cfg_id);
            return OAL_TRUE;
            break;
        }
        us_param_num += us_per_param_num;
    }

    return OAL_FALSE;
}


/*
 * �� �� ��  : hwifi_config_init
 * ��������  : netdev open ���õĶ��ƻ������
 *             ��ȡini�ļ������� al_host_init_params ȫ������?
 */
int32 hwifi_config_init(int32 cus_tag)
{
    int32 l_cfg_id;
    int32 l_ret = INI_FAILED;
    int32 l_ori_val;
    wlan_cfg_cmd *pgast_wifi_config = NULL;
    int32 *pgal_params = NULL;
    int32 l_cfg_value = 0;
    int32 l_wlan_cfg_butt;

    switch (cus_tag) {
        case CUS_TAG_NV:
            host_nvram_params_init();
            return hwifi_config_init_nvram();
        case CUS_TAG_INI:
            host_params_init_first();
            pgast_wifi_config = wifi_config_cmds;
            pgal_params = al_host_init_params;
            l_wlan_cfg_butt = WLAN_CFG_INIT_BUTT;
            break;
        case CUS_TAG_DTS:
            original_value_for_dts_params();
            pgast_wifi_config = wifi_config_dts;
            pgal_params = al_dts_params;
            l_wlan_cfg_butt = WLAN_CFG_DTS_USED_END;
            break;
        case CUS_TAG_PRIV_INI:
            return hwifi_config_init_private_custom();
        case CUS_TAG_PRO_LINE_INI:
            return hwifi_config_init_dy_cali_custom();
        default:
            OAM_ERROR_LOG1(0, OAM_SF_ANY, "hisi_customize_wifi tag number[0x%x] not correct!", cus_tag);
            return INI_FAILED;
    }

    for (l_cfg_id = 0; l_cfg_id < l_wlan_cfg_butt; ++l_cfg_id) {
        if (l_cfg_id != pgast_wifi_config[l_cfg_id].case_entry) {
            OAM_ERROR_LOG2(0, OAM_SF_ANY, "hwifi_config_init:l_cfg_id[%d]!=case_entry[%d]!!!!",
                           l_cfg_id, pgast_wifi_config[l_cfg_id].case_entry);
        }
        /* ��ȡini������ֵ */
        l_ret = get_cust_conf_int32(INI_MODU_WIFI, pgast_wifi_config[l_cfg_id].name, &l_cfg_value);

        if (l_ret == INI_FAILED) {
            OAM_WARNING_LOG1(0, OAM_SF_ANY, "hwifi_config_init read ini file failed, check if cfg_id[%d] exists!",
                             l_cfg_id);
            continue;
        }

        l_ori_val = pgal_params[l_cfg_id];
        pgal_params[l_cfg_id] = l_cfg_value;
        OAM_WARNING_LOG3(0, OAM_SF_ANY, "hwifi_config_init [cfg_id:%d]value changed from [init:%d] to [config:%d]. \n",
                         l_cfg_id, l_ori_val, pgal_params[l_cfg_id]);
    }
    return INI_SUCC;
}

/*
 * �� �� ��  : char2byte
 * ��������  : ͳ��ֵ���ж����޶�ȡ��mac��ַ
 * �� �� ֵ  : OAL_SUCC �� ʧ�ܴ�����
 */
OAL_STATIC int char2byte(const char *strori, char *outbuf)
{
    int i = 0;
    int temp = 0;
    int sum = 0;
    const int l_loop_times = 12; /* ���ֽڱ����ǲ�����ȷ��mac��ַ:xx:xx:xx:xx:xx:xx */

    for (i = 0; i < l_loop_times; i++) {
        switch (strori[i]) {
            case '0' ... '9':
                temp = strori[i] - '0';
                break;

            case 'a' ... 'f':
                temp = strori[i] - 'a' + 10; /* ��10Ϊ�˱�֤'a'~'f'�ֱ��Ӧ10~15 */
                break;

            case 'A' ... 'F':
                temp = strori[i] - 'A' + 10; /* ��10Ϊ�˱�֤'A'~'F'�ֱ��Ӧ10~15 */
                break;
        }

        sum += temp;
        /* Ϊ�������ȷ��mac��ַ:xx:xx:xx:xx:xx:xx */
        if (i % 2 == 0) {
            outbuf[i / 2] |= temp << 4;
        } else {
            outbuf[i / 2] |= temp;
        }
    }

    return sum;
}

/*
 * �� �� ��  : hwifi_get_mac_addr
 * ��������  : ��nvram�л�ȡmac��ַ
 *             �����ȡʧ�ܣ������һ��mac��ַ
 * �������  : puc_buf ptr
 * �� �� ֵ  : OAL_SUCC �� ʧ�ܴ�����
 */
int32 hwifi_get_mac_addr(uint8 *puc_buf)
{
    struct hisi_nve_info_user st_info;
    int32 l_ret;
    int32 l_sum = 0;

    if (puc_buf == NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "hisi_customize_wifi::buf is NULL!");
        return INI_FAILED;
    }

    memset_s(puc_buf, WLAN_MAC_ADDR_LEN, 0, MAC_LEN);

    memset_s(&st_info, sizeof(st_info), 0, sizeof(st_info));
    st_info.nv_number = NV_WLAN_NUM;  // nve item

    l_ret = strncpy_s(st_info.nv_name, OAL_SIZEOF(st_info.nv_name), "MACWLAN", sizeof("MACWLAN"));
    if (l_ret != EOK) {
        OAM_ERROR_LOG1(0, OAM_SF_CUSTOM, "hwifi_get_mac_addr::strncpy fail![%d]", l_ret);
        return INI_FAILED;
    }

    st_info.valid_size = NV_WLAN_VALID_SIZE;
    st_info.nv_operation = NV_READ;

    if ((auc_wifimac[0] != 0) || (auc_wifimac[1] != 0) || (auc_wifimac[2] != 0) || (auc_wifimac[3] != 0) ||
        (auc_wifimac[4] != 0) || (auc_wifimac[5] != 0)) {
        l_ret = memcpy_s(puc_buf, MAC_LEN, auc_wifimac, MAC_LEN);
        if (l_ret != EOK) {
            OAM_ERROR_LOG1(0, OAM_SF_CUSTOM, "hwifi_get_mac_addr::memcpy fail![%d]", l_ret);
            return INI_FAILED;
        }
        return INI_SUCC;
    }

    l_ret = hisi_nve_direct_access(&st_info);

    if (!l_ret) {
        l_sum = char2byte(st_info.nv_data, puc_buf);
        if (l_sum != 0) {
            INI_WARNING("hisi_customize_wifi get MAC from NV: mac=" MACFMT "\n", MAC2STR(puc_buf));

            l_ret = memcpy_s(auc_wifimac, MAC_LEN, puc_buf, MAC_LEN);
            if (l_ret != EOK) {
                OAM_ERROR_LOG1(0, OAM_SF_CUSTOM, "hwifi_get_mac_addr::memcpy fail![%d]", l_ret);
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

int32 hwifi_get_init_value(int32 cus_tag, int32 cfg_id)
{
    int32 *pgal_params = OAL_PTR_NULL;
    int32 l_wlan_cfg_butt;

    if (cus_tag == CUS_TAG_INI) {
        pgal_params = &al_host_init_params[0];
        l_wlan_cfg_butt = WLAN_CFG_INIT_BUTT;
    } else if (cus_tag == CUS_TAG_DTS) {
        pgal_params = &al_dts_params[0];
        l_wlan_cfg_butt = WLAN_CFG_DTS_BUTT;
    } else {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "hisi_customize_wifi tag number[0x%2x] not correct!", cus_tag);
        return INI_FAILED;
    }

    if ((cfg_id < 0) || (l_wlan_cfg_butt <= cfg_id)) {
        OAM_ERROR_LOG2(0, OAM_SF_ANY, "hisi_customize_wifi cfg id[%d] out of range, max cfg id is:%d",
                       cfg_id, l_wlan_cfg_butt - 1);
        return INI_FAILED;
    }
    return pgal_params[cfg_id];
}

/*
 * �� �� ��  : hwifi_get_country_code
 * ��������  : ��ȡ������
 * �� �� ֵ  : OAL_SUCC �� ʧ�ܴ�����
 */
int8 *hwifi_get_country_code(void)
{
    int32 l_ret;

    if (ac_country_code[0] != '0' && ac_country_code[1] != '0') {
        return ac_country_code;
    }

    /* ��ȡcust������ */
    l_ret = get_cust_conf_string(INI_MODU_WIFI, STR_COUNTRY_CODE, ac_country_code, sizeof(ac_country_code));

    if (l_ret == INI_FAILED) {
        /* ����ȥ�������� */
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "hisi_customize_wifi read country code failed, check if it exists!");
        l_ret = strncpy_s(ac_country_code, OAL_SIZEOF(ac_country_code), "99", sizeof("99") - 1);
        if (l_ret != EOK) {
            OAM_ERROR_LOG1(0, OAM_SF_CUSTOM, "hwifi_config_init_fcc_ce_txpwr_nvram::strncpy fail![%d]", l_ret);
            return ac_country_code;
        }
    }
    else {
        if (!OAL_MEMCMP(ac_country_code, "99", sizeof("99") - 1)) { /* ����ȥ�������� */
            OAM_WARNING_LOG0(0, OAM_SF_ANY, "hwifi_get_country_code is set 99!");
            g_st_cust_country_code_ignore_flag.en_country_code_ingore_ini_flag = OAL_TRUE;
        }
    }

    ac_country_code[COUNTRY_CODE_LEN - 1] = '\0'; /* ��'\0'��β */

    return ac_country_code;
}

/*
 * �� �� ��  : hwifi_set_country_code
 * ��������  : ���ù�����
 * �������  : country_code ptr, country_code len
 */
void hwifi_set_country_code(int8 *country_code, const uint32 len)
{
    oal_int32 l_ret;

    if ((country_code == OAL_PTR_NULL) || (len != COUNTRY_CODE_LEN)) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "hwifi_get_country_code ptr null or illegal len!");
        return;
    }

    l_ret = memcpy_s(ac_country_code, COUNTRY_CODE_LEN, country_code, COUNTRY_CODE_LEN);
    if (l_ret != EOK) {
        OAM_ERROR_LOG1(0, OAM_SF_CUSTOM, "hwifi_set_country_code::memcpy fail![%d]", l_ret);
        return;
    }

    ac_country_code[COUNTRY_CODE_LEN - 1] = '\0'; /* ��'\0'��β */

    return;
}

wlan_customize_power_params_stru *hwifi_get_nvram_params(void)
{
    return &cust_nv_params;
}

/*
 * �� �� ��  : hwifi_atcmd_update_host_nv_params
 * ��������  : ������ֻ������AT����:AT^WICALDATA=0,,,,,,���ã���������벻Ҫ���ñ��ӿ�
 *             ������ֻ������AT����:AT^WICALDATA=0,,,,,,���ã���������벻Ҫ���ñ��ӿ�
 * �� �� ֵ  : OAL_SUCC �� ʧ�ܴ�����
 */
int32 hwifi_atcmd_update_host_nv_params(void)
{
    int32 l_ret;

    memset_s(auc_nv_params, sizeof(auc_nv_params), 0x00, sizeof(auc_nv_params));

    l_ret = get_cust_conf_string(CUST_MODU_NVRAM, OAL_PTR_NULL, auc_nv_params, sizeof(auc_nv_params));
    if ((l_ret == INI_FAILED) || !auc_nv_params[0]) {
        /* �������̱��뷵�سɹ���ʧ���򱾴�У׼ʧ�ܣ���Ӧ���ٰ����������ߣ�ֱ�ӷ���ʧ�� */
        /* ʧ��ԭ�����������ڵ��ò�У����֮ǰд��NV�Ĳ������Ѿ�ʧ�ܵ���NV����Ϊ�� */
        OAM_ERROR_LOG2(0, OAM_SF_ANY,
            "hwifi_atcmd_update_host_nv_params::read nvram params failed or nv is empty, ret=[%d], nv_param[%u]!!",
            l_ret, auc_nv_params[0]);
        memset_s(auc_nv_params, sizeof(auc_nv_params), 0x00, sizeof(auc_nv_params));

        return INI_FAILED;
    }

    OAM_WARNING_LOG0(0, OAM_SF_ANY, "hwifi_atcmd_update_host_nv_params::update nvram params succ.");
    return INI_SUCC;
}

/* �������� */
EXPORT_SYMBOL_GPL(hwifi_get_gnss_scan_by_i3c);
EXPORT_SYMBOL_GPL(g_st_cust_country_code_ignore_flag);
EXPORT_SYMBOL_GPL(wlan_customize);
EXPORT_SYMBOL_GPL(as_pro_line_params);
EXPORT_SYMBOL_GPL(gs_extre_point_vals);
EXPORT_SYMBOL_GPL(en_fact_cali_completed);
EXPORT_SYMBOL_GPL(hwifi_config_init);
EXPORT_SYMBOL_GPL(hwifi_get_mac_addr);
EXPORT_SYMBOL_GPL(hwifi_get_init_value);
EXPORT_SYMBOL_GPL(hwifi_get_country_code);
EXPORT_SYMBOL_GPL(hwifi_get_nvram_params);
EXPORT_SYMBOL_GPL(hwifi_set_country_code);
EXPORT_SYMBOL_GPL(hwifi_is_regdomain_changed);
EXPORT_SYMBOL_GPL(hwifi_atcmd_update_host_nv_params);
EXPORT_SYMBOL_GPL(hwifi_custom_host_read_cfg_init);
EXPORT_SYMBOL_GPL(hwifi_custom_host_force_read_cfg_init);
EXPORT_SYMBOL_GPL(hwifi_get_init_priv_value);
EXPORT_SYMBOL_GPL(hwifi_get_nvram_param);
#ifdef _PRE_WLAN_FEATURE_NRCOEX
EXPORT_SYMBOL_GPL(hwifi_get_nrcoex_ini);
#endif
EXPORT_SYMBOL_GPL(en_nv_dp_init_is_null);
EXPORT_SYMBOL_GPL(hwifi_config_init_base_power);
EXPORT_SYMBOL_GPL(hwifi_get_plat_tag_from_country_code);
EXPORT_SYMBOL_GPL(hwifi_get_regdomain_from_country_code);
EXPORT_SYMBOL_GPL(auc_sar_params);
EXPORT_SYMBOL_GPL(hwifi_config_get_bt_dpn);
EXPORT_SYMBOL_GPL(g_aen_tas_switch_en);
EXPORT_SYMBOL_GPL(hwifi_get_tas_state);

#endif /* #ifdef _PRE_PLAT_FEATURE_CUSTOMIZE */
