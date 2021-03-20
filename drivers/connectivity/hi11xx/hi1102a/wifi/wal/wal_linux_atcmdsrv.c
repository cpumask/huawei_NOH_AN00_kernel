

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "oal_ext_if.h"
#include "oal_profiling.h"

#include "oam_ext_if.h"
#include "frw_ext_if.h"

#include "wlan_types.h"

#include "mac_vap.h"
#include "mac_resource.h"
#include "mac_ie.h"
#include "hmac_resource.h"
#include "hmac_scan.h"
#include "hmac_config.h"

#include "hmac_ext_if.h"
#include "hmac_chan_mgmt.h"

#include "wal_main.h"
#include "wal_config.h"
#include "wal_regdb.h"
#include "wal_linux_scan.h"
#include "wal_linux_atcmdsrv.h"
#include "wal_linux_bridge.h"
#include "wal_linux_flowctl.h"
#include "wal_linux_event.h"

#if ((defined(_PRE_PRODUCT_ID_HI110X_DEV)) || (defined(_PRE_PRODUCT_ID_HI110X_HOST)))
#include "plat_cali.h"
#include "oal_hcc_host_if.h"
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/pinctrl/consumer.h>
#include "board.h"
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
#include "hisi_customize_wifi.h"
#endif /* #ifdef _PRE_PLAT_FEATURE_CUSTOMIZE */
#endif
#endif

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_WAL_LINUX_ATCMDSRV_C

/*****************************************************************************
  2 �ṹ�嶨��
*****************************************************************************/
#if (defined(_PRE_PRODUCT_ID_HI110X_DEV) || defined(_PRE_PRODUCT_ID_HI110X_HOST))
typedef enum {
    CHECK_LTE_GPIO_INIT = 0,      /* ��ʼ�� */
    CHECK_LTE_GPIO_LOW = 1,       /* ����Ϊ�͵�ƽ */
    CHECK_LTE_GPIO_HIGH = 2,      /* ����Ϊ�ߵ�ƽ */
    CHECK_LTE_GPIO_RESUME = 3,    /* �ָ��Ĵ������� */
    CHECK_LTE_GPIO_DEV_LEVEL = 4, /* ��ȡdevice GPIO�ܽŵ�ƽֵ */
    CHECK_LTE_GPIO_BUTT
} check_lte_gpio_step;

typedef struct {
    oal_uint8 uc_mode; /* ģʽ */
    oal_uint8 uc_band; /* Ƶ�� */
} wal_atcmdsrv_mode_stru;

typedef struct {
    oal_uint32 ul_datarate; /* at�������õ�����ֵ */
    oal_int8 *puc_datarate; /* �����ַ��� */
} wal_atcmdsrv_datarate_stru;

OAL_CONST wal_atcmdsrv_mode_stru g_ast_atcmdsrv_mode_table[] = {
    { WLAN_LEGACY_11A_MODE,    WLAN_BAND_5G },    /* 11a, 5G, OFDM */
    { WLAN_LEGACY_11B_MODE,    WLAN_BAND_2G },    /* 11b, 2.4G */
    { WLAN_LEGACY_11G_MODE,    WLAN_BAND_2G },    /* �ɵ�11g only�ѷ���, 2.4G, OFDM */
    { WLAN_MIXED_ONE_11G_MODE, WLAN_BAND_2G }, /* 11bg, 2.4G */
    { WLAN_MIXED_TWO_11G_MODE, WLAN_BAND_2G }, /* 11g only, 2.4G */
    { WLAN_HT_MODE,            WLAN_BAND_5G },            /* 11n(11bgn����11an������Ƶ���ж�) */
    { WLAN_VHT_MODE,           WLAN_BAND_5G },           /* 11ac */
    { WLAN_HT_ONLY_MODE,       WLAN_BAND_5G },       /* 11n only 5Gmode,ֻ�д�HT���豸�ſ��Խ��� */
    { WLAN_VHT_ONLY_MODE,      WLAN_BAND_5G },      /* 11ac only mode ֻ�д�VHT���豸�ſ��Խ��� */
    { WLAN_HT_11G_MODE,        WLAN_BAND_2G },        /* 11ng,������11b */
    { WLAN_HT_ONLY_MODE_2G,    WLAN_BAND_2G },    /* 11nonlg 2Gmode */
    { WLAN_VHT_ONLY_MODE_2G,   WLAN_BAND_2G },   /* 11ac 2g mode ֻ�д�VHT���豸�ſ��Խ��� */
    { WLAN_PROTOCOL_BUTT,      WLAN_BAND_2G },
};

OAL_STATIC OAL_CONST wal_atcmdsrv_datarate_stru past_atcmdsrv_non_ht_rate_table[] = {
    { 0,  " 0 " }, /* mcs0, װ���·�65 */
    { 1,  " 1 " },
    { 2,  " 2 " },
    { 5,  " 5.5 " },
    { 6,  " 6 " },
    { 7,  " 7 " },
    { 8,  " 8 " },
    { 9,  " 9 " },
    { 11, " 11 " },
    { 12, " 12 " },
    { 18, " 18 " },
    { 24, " 24 " },
    { 36, " 36 " },
    { 48, " 48 " },
    { 54, " 54 " },
};
oal_uint8 g_auc_channel_idx[WAL_ATCMDSRV_CHANNEL_NUM] = { 48, 64, 112, 128, 144, 165 };
oal_uint64 ull_chipcheck_total_time;

wal_efuse_bits *st_efuse_bits = OAL_PTR_NULL;
oal_int32 g_l_bandwidth;
oal_int32 g_l_mode;
oal_int32 wlan_cal_disable_1102a = OAL_FALSE;

#define WAL_ATCMDSRV_WIFI_CHAN_NUM       2
#define WAL_ATCMDSRV_DYN_INTVAL_LEN      19
#define WAL_ATCMDSRV_DYN_INTVAL_LOCATION (WAL_ATCMDSRV_DYN_INTVAL_LEN - 2)

#endif
/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/

oal_int32 wal_atcmsrv_ioctl_get_rx_pckg(oal_net_device_stru *pst_net_dev, oal_int32 *pl_rx_pckg_succ_num)
{
    oal_int32 l_ret;
    mac_cfg_rx_fcs_info_stru *pst_rx_fcs_info = OAL_PTR_NULL;
    wal_msg_write_stru st_write_msg;
    mac_vap_stru *pst_mac_vap;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    oal_int32 i_leftime;
    hmac_atcmdsrv_get_stats_stru *pst_atcmdsrv = OAL_PTR_NULL;

    pst_mac_vap = OAL_NET_DEV_PRIV(pst_net_dev);
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_atcmsrv_ioctl_get_dbb_num::OAL_NET_DEV_PRIV, return null!}");
        return -OAL_EINVAL;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_atcmsrv_ioctl_get_dbb_num::get_hmac_vap failed!}");
        return OAL_FAIL;
    }

    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    pst_atcmdsrv = &(pst_hmac_vap->st_atcmdsrv_get_status);
    pst_atcmdsrv->uc_get_rx_pkct_flag = OAL_FALSE;
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_RX_FCS_INFO, OAL_SIZEOF(mac_cfg_rx_fcs_info_stru));

    /* ��������������� */
    pst_rx_fcs_info = (mac_cfg_rx_fcs_info_stru *)(st_write_msg.auc_value);
    /* ������������02�Ѿ�û������ */
    pst_rx_fcs_info->ul_data_op = 1;
    pst_rx_fcs_info->ul_print_info = 0;

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_rx_fcs_info_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_rx_fcs_info::return err code %d!}\r\n", l_ret);
        return l_ret;
    }

    /* �����ȴ�dmac�ϱ� */
    i_leftime = OAL_WAIT_EVENT_INTERRUPTIBLE_TIMEOUT(pst_hmac_vap->query_wait_q,
                                                     (oal_uint32)(pst_atcmdsrv->uc_get_rx_pkct_flag == OAL_TRUE),
                                                     WAL_ATCMDSRB_GET_RX_PCKT);
    if (i_leftime == 0) {
        /* ��ʱ��û���ϱ�ɨ����� */
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{wal_atcmsrv_ioctl_get_rx_pckg::dbb_num wait for %ld ms timeout!}",
                         ((WAL_ATCMDSRB_DBB_NUM_TIME * 1000) / OAL_TIME_HZ)); /* 1s����1000ms */
        return -OAL_EINVAL;
    } else if (i_leftime < 0) {
        /* ��ʱ���ڲ����� */
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{wal_atcmsrv_ioctl_get_rx_pckg::dbb_num wait for %ld ms error!}",
                         ((WAL_ATCMDSRB_DBB_NUM_TIME * 1000) / OAL_TIME_HZ)); /* 1s����1000ms */
        return -OAL_EINVAL;
    } else {
        *pl_rx_pckg_succ_num = (oal_int)pst_hmac_vap->st_atcmdsrv_get_status.ul_rx_pkct_succ_num;
        return OAL_SUCC;
    }
}


oal_int32 wal_atcmsrv_ioctl_set_hw_addr(oal_net_device_stru *pst_net_dev, oal_uint8 *pc_hw_addr)
{
    oal_int32 l_ret;
    mac_cfg_staion_id_param_stru *pst_mac_cfg_para;
    wal_msg_write_stru st_write_msg;

    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_STATION_ID, OAL_SIZEOF(mac_cfg_staion_id_param_stru));

    /* ��������������� */
    pst_mac_cfg_para = (mac_cfg_staion_id_param_stru *)(st_write_msg.auc_value);
    /* ������������02�Ѿ�û������ */
    pst_mac_cfg_para->en_p2p_mode = WLAN_LEGACY_VAP_MODE;
    oal_set_mac_addr(pst_mac_cfg_para->auc_station_id, pc_hw_addr);

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_staion_id_param_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_rx_fcs_info::return err code %d!}\r\n", l_ret);
        return l_ret;
    }
    return OAL_SUCC;
}

#if (defined(_PRE_PRODUCT_ID_HI110X_DEV) || defined(_PRE_PRODUCT_ID_HI110X_HOST))

OAL_STATIC oal_int32 wal_atcmsrv_ioctl_set_freq(oal_net_device_stru *pst_net_dev, oal_int32 l_freq)
{
    wal_msg_write_stru st_write_msg;

    oal_int32 l_ret;

    OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_atcmsrv_ioctl_set_freq:l_freq[%d]", l_freq);
    /***************************************************************************
        ���¼���wal�㴦��
    ***************************************************************************/
    /* ��д��Ϣ */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_CURRENT_CHANEL, OAL_SIZEOF(oal_int32));
    *((oal_int32 *)(st_write_msg.auc_value)) = l_freq;

    /* ������Ϣ */
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(oal_int32),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_atcmsrv_ioctl_set_freq::return err code %d!}", l_ret);
        return l_ret;
    }

    return OAL_SUCC;
}

OAL_STATIC oal_int32 wal_atcmsrv_ioctl_set_country(oal_net_device_stru *pst_net_dev, oal_int8 *puc_countrycode)
{
#ifdef _PRE_WLAN_FEATURE_11D
    oal_int32 l_ret;

    l_ret = wal_regdomain_update(pst_net_dev, puc_countrycode);
    if (l_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_atcmsrv_ioctl_set_country::regdomain_update return err code %d!}", l_ret);
        return l_ret;
    }
#endif
    return OAL_SUCC;
}

OAL_STATIC oal_int32 wal_atcmsrv_ioctl_set_txpower(oal_net_device_stru *pst_net_dev, oal_int32 l_txpower)
{
    wal_msg_write_stru st_write_msg;

    oal_int32 l_ret;

    OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_atcmsrv_ioctl_set_txpower:l_txpower[%d]", l_txpower);

    /***************************************************************************
        ���¼���wal�㴦��
    ***************************************************************************/
    /* ��д��Ϣ */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_TX_POWER, OAL_SIZEOF(oal_int32));
    *((oal_int32 *)(st_write_msg.auc_value)) = l_txpower;

    /* ������Ϣ */
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(oal_int32),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_atcmsrv_ioctl_set_txpower::return err code %d!}", l_ret);
        return l_ret;
    }

    return OAL_SUCC;
}

OAL_STATIC oal_int32 wal_atcmsrv_ioctl_set_mode(oal_net_device_stru *pst_net_dev, oal_int32 l_mode)
{
    wal_msg_write_stru st_write_msg;
    mac_cfg_mode_param_stru *pst_mode_param;
    oal_uint8 uc_prot_idx;
    mac_vap_stru *pst_mac_vap;
    oal_uint8 uc_arr_num = OAL_ARRAY_SIZE(g_ast_atcmdsrv_mode_table);

    oal_int32 l_ret;

    pst_mac_vap = OAL_NET_DEV_PRIV(pst_net_dev);
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_atcmsrv_ioctl_set_mode::OAL_NET_DEV_PRIV, return null!}");
        return -OAL_EINVAL;
    }

    /* ��ȡģʽ��Ӧ��band */
    for (uc_prot_idx = 0; uc_prot_idx < uc_arr_num; uc_prot_idx++) {
        if (g_ast_atcmdsrv_mode_table[uc_prot_idx].uc_mode == (oal_uint8)l_mode) {
            break;
        }
    }

    if (uc_prot_idx >= uc_arr_num) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "{wal_atcmsrv_ioctl_set_mode:err code[%u]}", uc_prot_idx);
        return -OAL_EINVAL;
    }
    /***************************************************************************
        ���¼���wal�㴦��
    ***************************************************************************/
    /* ��д��Ϣ */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_MODE, OAL_SIZEOF(mac_cfg_mode_param_stru));

    /* ����ģʽ��������ģʽ��ʱ�򽫴���Ĭ�ϳ�20M */
    pst_mode_param = (mac_cfg_mode_param_stru *)(st_write_msg.auc_value);
    if (l_mode == WLAN_HT_ONLY_MODE_2G) {
        pst_mode_param->en_protocol = WLAN_HT_ONLY_MODE;
    } else if (l_mode == WLAN_VHT_ONLY_MODE_2G) {
        pst_mode_param->en_protocol = WLAN_VHT_MODE;
    } else {
        pst_mode_param->en_protocol = (oal_uint8)l_mode;
    }

    pst_mode_param->en_band = (wlan_channel_band_enum_uint8)g_ast_atcmdsrv_mode_table[uc_prot_idx].uc_band;
    pst_mode_param->en_bandwidth = WLAN_BAND_WIDTH_20M;
    /* δ��ʹ�ã�������ɾ�� */
    OAM_WARNING_LOG3(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
                     "{wal_atcmsrv_ioctl_set_mode::protocol[%d],band[%d],bandwidth[%d]!}\r\n",
                     pst_mode_param->en_protocol, pst_mode_param->en_band, pst_mode_param->en_bandwidth);

    /* ������Ϣ */
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_mode_param_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_atcmsrv_ioctl_set_mode::return err code %d!}\r\n", l_ret);
        return l_ret;
    }
    g_l_mode = pst_mode_param->en_protocol;
    return OAL_SUCC;
}

OAL_STATIC oal_int32 wal_atcmsrv_ioctl_set_datarate(oal_net_device_stru *pst_net_dev, oal_int32 l_datarate)
{
    oal_uint8 uc_idx;
    oal_uint32 ul_ret;
    mac_vap_stru *pst_mac_vap;
    oal_uint8 en_bw_index = 0;
    mac_cfg_tx_comp_stru *pst_set_bw_param;
    wal_msg_write_stru st_write_msg;
    oal_int32 l_ret;
    oal_uint8 uc_arr_num = OAL_ARRAY_SIZE(past_atcmdsrv_non_ht_rate_table);

    pst_mac_vap = OAL_NET_DEV_PRIV(pst_net_dev);
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_atcmsrv_ioctl_set_datarate::OAL_NET_DEV_PRIV, return null!}");
        return -OAL_EINVAL;
    }

    OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "wal_atcmsrv_ioctl_set_datarate:l_datarate[%d]", l_datarate);

    /* ��ȡ���ʶ�Ӧ���ַ�����������������ʵ���Ӧ�ӿ� */
    for (uc_idx = 0; uc_idx < uc_arr_num; uc_idx++) {
        if (past_atcmdsrv_non_ht_rate_table[uc_idx].ul_datarate == (oal_uint32)l_datarate) {
            break;
        }
    }
    if (uc_idx >= uc_arr_num) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "uc_idx Overrunning!");
        return -OAL_EINVAL;
    }
    if (g_l_mode == WLAN_HT_ONLY_MODE) { /* ����������Ϊ7ʱ��ʾMCS7 */
        ul_ret = wal_hipriv_set_mcs(pst_net_dev, (oal_int8 *)past_atcmdsrv_non_ht_rate_table[uc_idx].puc_datarate);
    } else if (g_l_mode == WLAN_VHT_MODE) {
        ul_ret = wal_hipriv_set_mcsac(pst_net_dev, (oal_int8 *)past_atcmdsrv_non_ht_rate_table[uc_idx].puc_datarate);
    } else {
        ul_ret = wal_hipriv_set_rate(pst_net_dev, (oal_int8 *)past_atcmdsrv_non_ht_rate_table[uc_idx].puc_datarate);
    }
    if (ul_ret != OAL_SUCC) {
        return -OAL_EFAIL;
    }
    /* ���ó������������� */
    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_BW, OAL_SIZEOF(mac_cfg_tx_comp_stru));

    /* ��������������������� */
    pst_set_bw_param = (mac_cfg_tx_comp_stru *)(st_write_msg.auc_value);
    if ((g_l_bandwidth >= WLAN_BAND_WIDTH_80PLUSPLUS)
        && (g_l_bandwidth <= WLAN_BAND_WIDTH_80MINUSMINUS)) {
        en_bw_index = 8; /* 8�����80M���� */
    } else if ((g_l_bandwidth >= WLAN_BAND_WIDTH_40PLUS)
               && (g_l_bandwidth <= WLAN_BAND_WIDTH_40MINUS)) {
        en_bw_index = 4; /* 4�����40M���� */
    } else {
        en_bw_index = 0;
    }
    pst_set_bw_param->uc_param = (oal_uint8)(en_bw_index);

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_tx_comp_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_set_bw::return err code [%d]!}\r\n", l_ret);
        return l_ret;
    }
    return OAL_SUCC;
}

OAL_STATIC oal_int32 wal_atcmsrv_ioctl_set_bandwidth(oal_net_device_stru *pst_net_dev, oal_int32 l_bandwidth)
{
    wal_msg_write_stru st_write_msg;
    mac_cfg_mode_param_stru *pst_mode_param;

    oal_int32 l_ret;

    OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_atcmsrv_ioctl_set_bandwidth:l_bandwidth[%d]", l_bandwidth);
    g_l_bandwidth = l_bandwidth;

    /***************************************************************************
        ���¼���wal�㴦��
    ***************************************************************************/
    /* ��д��Ϣ */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_BANDWIDTH, OAL_SIZEOF(oal_int32));

    /* ���ô���ʱ��ģʽ�����޸ģ����ǰ���֮ǰ��ֵ���� */
    pst_mode_param = (mac_cfg_mode_param_stru *)(st_write_msg.auc_value);

    pst_mode_param->en_bandwidth = (oal_uint8)l_bandwidth;

    /* ������Ϣ */
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_mode_param_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_atcmsrv_ioctl_set_mode::return err code %d!}\r\n", l_ret);
        return l_ret;
    }

    return OAL_SUCC;
}

OAL_STATIC oal_int32 wal_atcmsrv_ioctl_set_always_tx(oal_net_device_stru *pst_net_dev, oal_int32 l_always_tx)
{
    wal_msg_write_stru st_write_msg;
    oal_int32 l_ret;
    mac_cfg_tx_comp_stru *pst_set_bcast_param;
    oal_uint8 auc_param[] = { "all" };
    oal_uint16 us_len;

    OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_atcmsrv_ioctl_set_always_tx:l_always_tx[%d]", l_always_tx);
    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_ALWAYS_TX_1102, OAL_SIZEOF(mac_cfg_tx_comp_stru));

    /* ��������������������� */
    pst_set_bcast_param = (mac_cfg_tx_comp_stru *)(st_write_msg.auc_value);

    /* װ�����Ե������ֱ�ӽ������������ú� */
    pst_set_bcast_param->en_payload_flag = RF_PAYLOAD_RAND;
    pst_set_bcast_param->ul_payload_len = WAL_ATCMDSRB_IOCTL_AL_TX_LEN;
    pst_set_bcast_param->uc_param = (oal_uint8)l_always_tx;

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_tx_comp_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_atcmsrv_ioctl_set_always_tx::return err code [%d]!}", l_ret);
        return l_ret;
    }

    /* ��ӡ���мĴ��� */
    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    if (memcpy_s(st_write_msg.auc_value, OAL_SIZEOF(st_write_msg.auc_value),
                 auc_param, OAL_STRLEN((oal_int8 *)auc_param)) != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "wal_atcmsrv_ioctl_set_always_tx::memcpy fail!");
        return OAL_FAIL;
    }

    st_write_msg.auc_value[OAL_STRLEN((oal_int8 *)auc_param)] = '\0';
    us_len = (oal_uint16)(OAL_STRLEN((oal_int8 *)auc_param) + 1);

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_REG_INFO, us_len);

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + us_len,
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_atcmsrv_ioctl_set_always_tx::return err code [%d]!}\r\n", l_ret);
    }

    return OAL_SUCC;
}


OAL_STATIC oal_void wal_atcmdsrv_ioctl_convert_dbb_num(oal_uint32 ul_dbb_num, oal_uint8 *pc_dbb_num)
{
    oal_uint8 uc_temp;

    /* MAC H/w version register format */
    /* ------------------------------------------------ */
    /* | 31 - 24 | 23 - 16 | 15 - 12 | 11 - 0 | */
    /* ------------------------------------------------ */
    /* | BN      | Y1      | Y2      |   Y3   | */
    /* ------------------------------------------------ */
    /* Format the version as BN.Y1.Y2.Y3 with all values in hex i.e. the */
    /* version string would be XX.XX.X.XXX. */
    /* For e.g. 0225020A saved in the version register would translate to */
    /* the configuration interface version number 02.25.0.20A */
    uc_temp = (ul_dbb_num & 0xF0000000) >> 28; /* pc_dbb_num��ȡBNֵ��BN��1���ֽ�28-31bit�� */
    pc_dbb_num[0] = WAL_ATCMDSRV_GET_HEX_CHAR(uc_temp);
    uc_temp = (ul_dbb_num & 0x0F000000) >> 24; /* pc_dbb_num��ȡBNֵ��BN��2���ֽ�24-27bit�� */
    pc_dbb_num[1] = WAL_ATCMDSRV_GET_HEX_CHAR(uc_temp);

    pc_dbb_num[2] = '.'; /* pc_dbb_num[2]��ֵ'.' */

    uc_temp = (ul_dbb_num & 0x00F00000) >> 20; /* pc_dbb_num��ȡY1��ֵ��Y1��1���ֽ�20-23bit�� */
    pc_dbb_num[3] = WAL_ATCMDSRV_GET_HEX_CHAR(uc_temp); /* pc_dbb_num[3]ΪY1�ĵ�һ���ֽڵ�ֵ */
    uc_temp = (ul_dbb_num & 0x000F0000) >> 16; /* pc_dbb_num��ȡY1��ֵ��Y1��2���ֽ�16-19bit�� */
    pc_dbb_num[4] = WAL_ATCMDSRV_GET_HEX_CHAR(uc_temp); /* pc_dbb_num[4]ΪY1�ĵڶ����ֽڵ�ֵ */
    pc_dbb_num[5] = '.'; /* pc_dbb_num[5]��ֵ'.' */

    uc_temp = (ul_dbb_num & 0x0000F000) >> 12; /* pc_dbb_num��ȡY2��ֵ��Y2�ֽ�12-15bit�� */
    pc_dbb_num[6] = WAL_ATCMDSRV_GET_HEX_CHAR(uc_temp); /* pc_dbb_num[6]ΪY2��ֵ */
    pc_dbb_num[7] = '.'; /* pc_dbb_num[7]��ֵ'.' */

    uc_temp = (ul_dbb_num & 0x00000F00) >> 8; /* pc_dbb_num��ȡY3��ֵ��Y2�ֽ�8-11bit�� */
    pc_dbb_num[8] = WAL_ATCMDSRV_GET_HEX_CHAR(uc_temp); /* pc_dbb_num[8]ΪY1�ĵ�һ���ֽڵ�ֵ */
    uc_temp = (ul_dbb_num & 0x000000F0) >> 4; /* pc_dbb_num��ȡY3��ֵ��Y2�ֽ�4-7bit�� */
    pc_dbb_num[9] = WAL_ATCMDSRV_GET_HEX_CHAR(uc_temp); /* pc_dbb_num[9]ΪY1�ĵڶ����ֽڵ�ֵ */
    uc_temp = (ul_dbb_num & 0x0000000F) >> 0; /* pc_dbb_num��ȡY3��ֵ��Y2�ֽ�0-3bit�� */
    pc_dbb_num[10] = WAL_ATCMDSRV_GET_HEX_CHAR(uc_temp); /* pc_dbb_num[10]ΪY1�ĵ������ֽڵ�ֵ */

    return;
}


OAL_STATIC oal_int32 wal_atcmsrv_ioctl_get_dbb_num(oal_net_device_stru *pst_net_dev, oal_int8 *pc_dbb_num)
{
    wal_msg_write_stru st_write_msg;
    oal_int32 l_ret;
    oal_int32 i_leftime;
    mac_vap_stru *pst_mac_vap;
    hmac_vap_stru *pst_hmac_vap;
    hmac_atcmdsrv_get_stats_stru *pst_atcmdsrv;

    pst_mac_vap = OAL_NET_DEV_PRIV(pst_net_dev);
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_atcmsrv_ioctl_get_dbb_num::OAL_NET_DEV_PRIV, return null!}");
        return -OAL_EINVAL;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_atcmsrv_ioctl_get_dbb_num::get_hmac_vap failed!}");
        return OAL_FAIL;
    }

    /***************************************************************************
                              ���¼���wal�㴦��
    ***************************************************************************/
    pst_atcmdsrv = &(pst_hmac_vap->st_atcmdsrv_get_status);
    pst_atcmdsrv->uc_get_dbb_completed_flag = OAL_FALSE;
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_GET_VERSION, 0);

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH,
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_atcmsrv_ioctl_get_dbb_num::wal_send_cfg_event return err [%d]!}", l_ret);
        return l_ret;
    }
    /* �����ȴ�dmac�ϱ� */
    /*lint -e730*/
    i_leftime = OAL_WAIT_EVENT_INTERRUPTIBLE_TIMEOUT(pst_hmac_vap->query_wait_q,
                                                     (pst_atcmdsrv->uc_get_dbb_completed_flag == OAL_TRUE),
                                                     WAL_ATCMDSRB_DBB_NUM_TIME);
    /*lint +e730*/
    if (i_leftime == 0) {
        /* ��ʱ��û���ϱ�ɨ����� */
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{wal_atcmsrv_ioctl_get_dbb_num::dbb_num wait for %ld ms timeout!}",
                         ((WAL_ATCMDSRB_DBB_NUM_TIME * 1000) / OAL_TIME_HZ)); /* 1s����1000ms */
        return -OAL_EINVAL;
    } else if (i_leftime < 0) {
        /* ��ʱ���ڲ����� */
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{wal_atcmsrv_ioctl_get_dbb_num::dbb_num wait for %ld ms error!}",
                         ((WAL_ATCMDSRB_DBB_NUM_TIME * 1000) / OAL_TIME_HZ)); /* 1s����1000ms */
        return -OAL_EINVAL;
    } else {
        if (pst_hmac_vap->st_atcmdsrv_get_status.ul_dbb_num != 0x210061b) {
            OAM_ERROR_LOG1(0, OAM_SF_ANY, "wal_atcmsrv_ioctl_get_dbb_num:ul_dbb_num[0x%x],not match 0x210061b",
                           pst_hmac_vap->st_atcmdsrv_get_status.ul_dbb_num);
            return -OAL_EINVAL;
        }
        wal_atcmdsrv_ioctl_convert_dbb_num(pst_hmac_vap->st_atcmdsrv_get_status.ul_dbb_num, (oal_uint8 *)pc_dbb_num);
        return OAL_SUCC;
    }
}


OAL_STATIC oal_int32 wal_atcmsrv_ioctl_lte_gpio_mode(oal_net_device_stru *pst_net_dev,
                                                     oal_int32 l_check_lte_gpio_step)
{
    wal_msg_write_stru st_write_msg;
    oal_int32 l_ret;
    oal_int32 i_leftime;
    mac_vap_stru *pst_mac_vap;
    hmac_vap_stru *pst_hmac_vap;
    hmac_atcmdsrv_get_stats_stru *pst_atcmdsrv;

    pst_mac_vap = OAL_NET_DEV_PRIV(pst_net_dev);
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_atcmsrv_ioctl_lte_gpio_mode::OAL_NET_DEV_PRIV, return null!}");
        return -OAL_EINVAL;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                       "{wal_atcmsrv_ioctl_lte_gpio_mode::mac_res_get_hmac_vap failed!}");
        return -OAL_EINVAL;
    }

    pst_atcmdsrv = &(pst_hmac_vap->st_atcmdsrv_get_status);
    pst_atcmdsrv->uc_lte_gpio_check_flag = OAL_FALSE;

    /***************************************************************************
         ���¼���wal�㴦��
     ***************************************************************************/
    /* ��д��Ϣ */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_CHECK_LTE_GPIO, OAL_SIZEOF(oal_int32));

    /* ����LTE�麸����ģʽ */
    *(oal_int32 *)(st_write_msg.auc_value) = l_check_lte_gpio_step;

    /* ������Ϣ */
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(oal_int32),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_atcmsrv_ioctl_lte_gpio_mode::return err code %d!}\r\n", l_ret);
        return l_ret;
    }
    /* �����ȴ�dmac�ϱ� */
    /*lint -e730*/
    i_leftime = OAL_WAIT_EVENT_INTERRUPTIBLE_TIMEOUT(pst_hmac_vap->query_wait_q,
                                                     (pst_atcmdsrv->uc_lte_gpio_check_flag == OAL_TRUE),
                                                     WAL_ATCMDSRB_DBB_NUM_TIME);
    /*lint +e730*/
    if (i_leftime == 0) {
        /* ��ʱ��û���ϱ�ɨ����� */
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{wal_atcmsrv_ioctl_lte_gpio_mode:: wait for %ld ms timeout!}",
                         ((WAL_ATCMDSRB_DBB_NUM_TIME * 1000) / OAL_TIME_HZ)); /* 1s����1000ms */
        return -OAL_EINVAL;
    } else if (i_leftime < 0) {
        /* ��ʱ���ڲ����� */
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_atcmsrv_ioctl_lte_gpio_mode::wait for%ld ms error!}",
                         ((WAL_ATCMDSRB_DBB_NUM_TIME * 1000) / OAL_TIME_HZ)); /* 1s����1000ms */
        return -OAL_EINVAL;
    } else {
        return OAL_SUCC;
    }
}


OAL_STATIC oal_int32 wal_atcmsrv_ioctl_lte_gpio_level_set(oal_int32 l_gpio_level)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    oal_int32 l_ret;
    OAM_WARNING_LOG1(0, 0, "wal_atcmsrv_ioctl_lte_gpio_level_set:SET LTE GPIO %d!", l_gpio_level);

    l_ret = gpio_direction_output(wlan_customize.ul_lte_ism_priority, l_gpio_level);
    if (l_ret != OAL_SUCC) {
        OAM_WARNING_LOG0(0, 0, "wal_atcmsrv_ioctl_lte_gpio_level_set:SET LTE ISM PRIORITY FAIL!");
        return l_ret;
    }

    l_ret = gpio_direction_output(wlan_customize.ul_lte_rx_act, l_gpio_level);
    if (l_ret != OAL_SUCC) {
        OAM_WARNING_LOG0(0, 0, "wal_atcmsrv_ioctl_lte_gpio_level_set:SET LTE RX ACT FAIL!");
        return l_ret;
    }

    l_ret = gpio_direction_output(wlan_customize.ul_lte_tx_act, l_gpio_level);
    if (l_ret != OAL_SUCC) {
        OAM_WARNING_LOG0(0, 0, "wal_atcmsrv_ioctl_lte_gpio_level_set:SET LTE TX ACT FAIL!");
        return l_ret;
    }
    return l_ret;
#else
    return OAL_SUCC;
#endif
}


OAL_STATIC oal_int32 wal_atcmsrv_ioctl_lte_gpio_level_check(oal_net_device_stru *pst_net_dev,
                                                            oal_int32 l_gpio_level)
{
    oal_int32 l_ret;

    l_ret = wal_atcmsrv_ioctl_lte_gpio_mode(pst_net_dev, CHECK_LTE_GPIO_DEV_LEVEL);
    if (l_ret < 0) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_atcmsrv_ioctl_lte_gpio_level_check::GET DEV LTE GPIO LEVEL FAIL!}");
        return -OAL_EINVAL;
    }

    l_ret = -OAL_EINVAL;

    if (l_gpio_level == 0) {
        if (hmac_atcmsrv_get_lte_gpio_level() == 0x0) {
            OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_atcmsrv_ioctl_lte_gpio_level_check::check gpio low mode SUCC!}");
            l_ret = OAL_SUCC;
        }
    } else if (l_gpio_level == 1) {
        /* CHECK BIT_2 BIT_5 BIT_6 */
        if (hmac_atcmsrv_get_lte_gpio_level() == 0x64) {
            OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_atcmsrv_ioctl_lte_gpio_level_check::check gpio high mode SUCC!}");
            l_ret = OAL_SUCC;
        }
    } else {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "{wal_atcmsrv_ioctl_lte_gpio_level_check::unknown param l_gpio_level %d!}",
                       l_gpio_level);
    }

    return l_ret;
}


OAL_STATIC oal_int32 wal_atcmsrv_ioctl_lte_gpio_set(oal_void)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    oal_int32 l_ret = -OAL_EFAIL;
    /* �����ܽ����ó�gpioģʽ */
    if (g_board_info.need_power_prepare) {
        /* set LowerPower mode */
        l_ret = pinctrl_select_state(g_board_info.pctrl, g_board_info.pins_idle);
        if (l_ret) {
            OAM_WARNING_LOG0(0, 0, "wal_atcmsrv_ioctl_lte_gpio_set:set mode gpio fail");
            return -OAL_EFAIL;
        }
    }

    l_ret = gpio_request_one(wlan_customize.ul_lte_ism_priority, GPIOF_OUT_INIT_LOW,
                             WAL_ATCMDSRV_LTE_ISM_PRIORITY_NAME);
    if (l_ret) {
        OAM_WARNING_LOG0(0, 0, "wal_atcmsrv_ioctl_lte_gpio_set:set LTE_ISM_PRIORITY mode gpio fail");
        if (g_board_info.need_power_prepare) {
            l_ret = pinctrl_select_state(g_board_info.pctrl, g_board_info.pins_normal);
            if (l_ret) {
                OAM_WARNING_LOG0(0, 0, "wal_atcmsrv_ioctl_lte_gpio_set:set pinctrl_select_state fail");
            }
        }
        return -OAL_EFAIL;
    }

    l_ret = gpio_request_one(wlan_customize.ul_lte_rx_act, GPIOF_OUT_INIT_LOW, WAL_ATCMDSRV_LTE_RX_ACT_NAME);
    if (l_ret) {
        OAM_WARNING_LOG0(0, 0, "wal_atcmsrv_ioctl_lte_gpio_set:set LTE_RX_ACT mode gpio fail");
        gpio_free(wlan_customize.ul_lte_ism_priority);
        if (g_board_info.need_power_prepare) {
            l_ret = pinctrl_select_state(g_board_info.pctrl, g_board_info.pins_normal);
            if (l_ret) {
                OAM_WARNING_LOG0(0, 0, "wal_atcmsrv_ioctl_lte_gpio_set:set pinctrl_select_state fail");
            }
        }
        return -OAL_EFAIL;
    }

    l_ret = gpio_request_one(wlan_customize.ul_lte_tx_act, GPIOF_OUT_INIT_LOW, WAL_ATCMDSRV_LTE_TX_ACT_NAME);
    if (l_ret) {
        OAM_WARNING_LOG0(0, 0, "wal_atcmsrv_ioctl_lte_gpio_set:set LTE_TX_ACT mode gpio fail");
        gpio_free(wlan_customize.ul_lte_ism_priority);
        gpio_free(wlan_customize.ul_lte_rx_act);
        if (g_board_info.need_power_prepare) {
            l_ret = pinctrl_select_state(g_board_info.pctrl, g_board_info.pins_normal);
            if (l_ret) {
                OAM_WARNING_LOG0(0, 0, "wal_atcmsrv_ioctl_lte_gpio_set:set pinctrl_select_state fail");
            }
        }
        return -OAL_EFAIL;
    }
#endif
    return OAL_SUCC;
}

OAL_STATIC oal_void wal_atcmsrv_ioctl_lte_gpio_free(oal_net_device_stru *pst_net_dev)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    wal_msg_write_stru st_write_msg;
    oal_int32 l_ret;

    gpio_free(wlan_customize.ul_lte_ism_priority);

    gpio_free(wlan_customize.ul_lte_rx_act);

    gpio_free(wlan_customize.ul_lte_tx_act);
    if (g_board_info.need_power_prepare) {
        l_ret = pinctrl_select_state(g_board_info.pctrl, g_board_info.pins_normal);
        if (l_ret) {
            OAM_WARNING_LOG0(0, 0, "wal_atcmsrv_ioctl_lte_gpio_free:set pinctrl_select_state fail");
        }
    }
    /***************************************************************************
         ���¼���wal�㴦��
     ***************************************************************************/
    /* ��д��Ϣ */
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_CHECK_LTE_GPIO, OAL_SIZEOF(oal_int32));

    /* ����LTE�麸����ģʽ */
    *(oal_int32 *)(st_write_msg.auc_value) = CHECK_LTE_GPIO_RESUME;

    /* ������Ϣ */
    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(oal_int32),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_atcmsrv_ioctl_lte_gpio_mode::return err code %d!}\r\n", l_ret);
    }
#endif
}


OAL_STATIC oal_int32 wal_atcmsrv_ioctl_lte_gpio_check(oal_net_device_stru *pst_net_dev)
{
    oal_int32 l_ret;

    /* step1 ���ùܽ�Ϊgpioģʽ */
    OAM_WARNING_LOG0(0, 0, "wal_atcmsrv_ioctl_lte_gpio_check:enter lte gpio check!");
    /* ��ʼ��host�ܽ� */
    l_ret = wal_atcmsrv_ioctl_lte_gpio_set();
    if (l_ret != OAL_SUCC) {
        return l_ret;
    }

    /* ��ʼ��device lte�������ż�� */
    l_ret = wal_atcmsrv_ioctl_lte_gpio_mode(pst_net_dev, CHECK_LTE_GPIO_INIT);
    if (l_ret != OAL_SUCC) {
        OAM_WARNING_LOG0(0, 0, "wal_atcmsrv_ioctl_lte_gpio_check:CHECK_LTE_GPIO_INIT FAIL!");
        wal_atcmsrv_ioctl_lte_gpio_free(pst_net_dev);
        return l_ret;
    }

    /* step2 ����host�ܽ�Ϊ�ͣ���ȡdevice��� */
    /* ��gpioȫ������Ϊ�� */
    l_ret = wal_atcmsrv_ioctl_lte_gpio_level_set(0);
    if (l_ret != OAL_SUCC) {
        OAM_WARNING_LOG0(0, 0, "wal_atcmsrv_ioctl_lte_gpio_check:SET LTE GPIO LOW FAIL!");
        wal_atcmsrv_ioctl_lte_gpio_free(pst_net_dev);
        return l_ret;
    }

    /* ��ȡdevice GPIO�ܽŵ�ƽ */
    l_ret = wal_atcmsrv_ioctl_lte_gpio_level_check(pst_net_dev, 0);
    if (l_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, 0, "wal_atcmsrv_ioctl_lte_gpio_check:gpio low mode fail[%x]", 
            hmac_atcmsrv_get_lte_gpio_level());
        wal_atcmsrv_ioctl_lte_gpio_free(pst_net_dev);
        return l_ret;
    }

    /* step3 ����host�ܽ�Ϊ�ߣ���ȡdevice��� */
    /* ��gpioȫ������Ϊ�� */
    l_ret = wal_atcmsrv_ioctl_lte_gpio_level_set(1);
    if (l_ret != OAL_SUCC) {
        OAM_WARNING_LOG0(0, 0, "wal_atcmsrv_ioctl_lte_gpio_check:SET LTE GPIO HIGH FAIL!");
        wal_atcmsrv_ioctl_lte_gpio_free(pst_net_dev);
        return l_ret;
    }

    /* ��ȡdevice GPIO�ܽŵ�ƽ */
    l_ret = wal_atcmsrv_ioctl_lte_gpio_level_check(pst_net_dev, 1);
    if (l_ret != 0) {
        OAM_WARNING_LOG1(0, 0, "wal_atcmsrv_ioctl_lte_gpio_check:gpio high mode FAIL[%x]!", 
            hmac_atcmsrv_get_lte_gpio_level());
        wal_atcmsrv_ioctl_lte_gpio_free(pst_net_dev);
        return l_ret;
    }

    wal_atcmsrv_ioctl_lte_gpio_free(pst_net_dev);

    return OAL_SUCC;
}
oal_uint64 ul_gpio_wakeup_host_int_get_save;

oal_int32 wal_atcmsrv_ioctl_get_hw_status(oal_net_device_stru *pst_net_dev, oal_int32 *pl_fem_pa_status)
{
    oal_cali_param_stru *pst_cali_data;
    oal_uint64 ul_gpio_wakeup_host_int_get;
    oal_uint32 ul_check_gpio_wakeup_host_status = 0;
    oal_int32 l_ret;
    oal_int8 auc_dbb[WAL_ATCMDSRV_IOCTL_DBB_LEN];
    oal_uint32 ul_lte_status = 0;
    /* device�ϱ���Ϣ�����������жϣ���⻽������ */
    l_ret = wal_atcmsrv_ioctl_get_dbb_num(pst_net_dev, auc_dbb);
    if (l_ret != OAL_SUCC) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "wal_atcmsrv_ioctl_get_fem_pa_status:Failed to get dbb num !");
    }

    /* device����host gpio���ż�� */
    ul_gpio_wakeup_host_int_get = oal_get_gpio_int_count_para();
    if (ul_gpio_wakeup_host_int_get_save == ul_gpio_wakeup_host_int_get) {
        ul_check_gpio_wakeup_host_status = 1;

        OAM_ERROR_LOG0(0, OAM_SF_ANY, "wal_atcmsrv_ioctl_get_fem_pa_status:check wl_host_wake_up gpio fail!");
    }
    ul_gpio_wakeup_host_int_get_save = ul_gpio_wakeup_host_int_get;

    pst_cali_data = (oal_cali_param_stru *)get_cali_data_buf_addr();
    if (pst_cali_data == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_atcmsrv_ioctl_get_hw_status:pst_cali_data: return null!}");
        return -OAL_EINVAL;
    }
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    if (wlan_customize.ul_lte_gpio_check_switch == 1) {
        /* ��ȡlte����ܽŽ�� */
        l_ret = wal_atcmsrv_ioctl_lte_gpio_check(pst_net_dev);
        if (l_ret != OAL_SUCC) {
            ul_lte_status = 1;
        }
    } else {
        ul_lte_status = 0;
    }
#else
    /* ��ȡlte����ܽŽ�� */
    l_ret = wal_atcmsrv_ioctl_lte_gpio_check(pst_net_dev);
    if (l_ret != OAL_SUCC) {
        ul_lte_status = 1;
    }
#endif
    /* ul_check_gpio_wakeup_host_status����4λ��ʾ���gpio����host��״̬ */
    *pl_fem_pa_status = (oal_int32)(pst_cali_data->ul_check_hw_status | (ul_check_gpio_wakeup_host_status << 4) |
                                    (ul_lte_status << 5)); /* ul_lte_status����5λ��ʾlte��״̬ */

    if (*pl_fem_pa_status != 0) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY,
                       "{fem_2G_pa_status[bit0-bit1],fem_5G_pa_status[bit2-bit3];check_hw_status[0x%x]}",
                       *pl_fem_pa_status);
    }

    return OAL_SUCC;
}


oal_void wal_atcmsrv_ioctl_get_fem_pa_status(oal_net_device_stru *pst_net_dev, oal_int32 *pl_fem_pa_status)
{
    oal_cali_param_stru *pst_cali_data;

    pst_cali_data = (oal_cali_param_stru *)get_cali_data_buf_addr();
    if (pst_cali_data == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_atcmsrv_ioctl_get_hw_status:pst_cali_data:return null!}");
        return;
    }

    *pl_fem_pa_status = (oal_int32)(pst_cali_data->ul_check_hw_status);

    if (*pl_fem_pa_status != 0) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY,
                       "{fem_pa_status[bit0-bit1],lna_status[bit2-bit3],gpio[bit4],ul_check_hw_status[0x%x]}",
                       *pl_fem_pa_status);
#ifdef _PRE_WLAN_1102A_CHR
        CHR_EXCEPTION_REPORT(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_WIFI, CHR_LAYER_DEV,
                             CHR_WIFI_DEV_EVENT_CHIP, CHR_WIFI_DEV_ERROR_FEM_FAIL);
#endif
#ifdef CONFIG_HUAWEI_DSM
        hw_1102a_dsm_client_notify(DSM_WIFI_FEMERROR, "%s: fem error", __FUNCTION__);
#endif
    }
}


OAL_STATIC oal_int32 wal_atcmsrv_ioctl_set_always_rx(oal_net_device_stru *pst_net_dev, oal_int32 l_always_rx)
{
    wal_msg_write_stru st_write_msg;
    oal_int32 l_ret;
    oal_uint8 auc_param[] = { "all" };
    oal_uint16 us_len;

    /* ��״̬��ֵ */
    *(oal_uint8 *)(st_write_msg.auc_value) = (oal_uint8)l_always_rx;

    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_ALWAYS_RX, OAL_SIZEOF(oal_uint8));

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(oal_uint8),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_hipriv_always_rx::return err code [%d]!}\r\n", l_ret);
        return l_ret;
    }

    /* ��ӡ���мĴ��� */
    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    if (memcpy_s(st_write_msg.auc_value, OAL_SIZEOF(st_write_msg.auc_value),
                 auc_param, OAL_STRLEN((oal_int8 *)auc_param)) != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "wal_atcmsrv_ioctl_set_always_rx::memcpy fail!");
        return OAL_FAIL;
    }

    st_write_msg.auc_value[OAL_STRLEN((oal_int8 *)auc_param)] = '\0';
    us_len = (oal_uint16)(OAL_STRLEN((oal_int8 *)auc_param) + 1);

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_REG_INFO, us_len);

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + us_len,
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_atcmsrv_ioctl_set_always_rx::return err code [%d]!}\r\n", l_ret);
    }
    return OAL_SUCC;
}


OAL_STATIC oal_int32 wal_atcmsrv_ioctl_set_pm_switch(oal_net_device_stru *pst_net_dev, oal_int32 l_pm_switch)
{
    wal_msg_write_stru st_write_msg;

    oal_int32 l_ret;
    oal_uint8 sta_pm_on[5] = " 0 ";

    OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_atcmsrv_ioctl_set_pm_switch:l_pm_switch[%d]", l_pm_switch);

    *(oal_uint8 *)(st_write_msg.auc_value) = (oal_uint8)l_pm_switch;

    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_SET_PM_SWITCH, OAL_SIZEOF(oal_int32));

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(oal_int32),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_atcmsrv_ioctl_set_pm_switch::return err code [%d]!}\r\n", l_ret);
        return l_ret;
    }
#ifdef _PRE_WLAN_FEATURE_STA_PM
    l_ret = wal_hipriv_sta_pm_on(pst_net_dev, sta_pm_on);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_atcmsrv_ioctl_set_pm_switch::CMD_SET_STA_PM_ON return err code [%d]!}",
                         l_ret);
        return l_ret;
    }

#endif

    return OAL_SUCC;
}

OAL_STATIC oal_int32 wal_atcmsrv_ioctl_get_rx_rssi(oal_net_device_stru *pst_net_dev, oal_int32 *pl_rx_rssi)
{
    oal_int32 l_ret;
    mac_cfg_rx_fcs_info_stru *pst_rx_fcs_info;
    wal_msg_write_stru st_write_msg;
    mac_vap_stru *pst_mac_vap;
    hmac_vap_stru *pst_hmac_vap;
    oal_int32 i_leftime;
    hmac_atcmdsrv_get_stats_stru *pst_atcmdsrv;

    pst_mac_vap = OAL_NET_DEV_PRIV(pst_net_dev);
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_atcmsrv_ioctl_get_rx_rssi::OAL_NET_DEV_PRIV, return null!}");
        return -OAL_EINVAL;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_atcmsrv_ioctl_get_rx_rssi::get_hmac_vap failed!}");
        return OAL_FAIL;
    }

    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    pst_atcmdsrv = &(pst_hmac_vap->st_atcmdsrv_get_status);
    pst_atcmdsrv->uc_get_rx_pkct_flag = OAL_FALSE;
    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_RX_FCS_INFO, OAL_SIZEOF(mac_cfg_rx_fcs_info_stru));

    /* ��������������� */
    pst_rx_fcs_info = (mac_cfg_rx_fcs_info_stru *)(st_write_msg.auc_value);
    /* ������������02�Ѿ�û������ */
    pst_rx_fcs_info->ul_data_op = 0;
    pst_rx_fcs_info->ul_print_info = 0;

    l_ret = wal_send_cfg_event(pst_net_dev,
                               WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + OAL_SIZEOF(mac_cfg_rx_fcs_info_stru),
                               (oal_uint8 *)&st_write_msg,
                               OAL_FALSE,
                               OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_atcmsrv_ioctl_get_rx_rssi::return err code %d!}\r\n", l_ret);
        return l_ret;
    }

    /* �����ȴ�dmac�ϱ� */
    i_leftime = OAL_WAIT_EVENT_INTERRUPTIBLE_TIMEOUT(pst_hmac_vap->query_wait_q,
                                                     (oal_uint32)(pst_atcmdsrv->uc_get_rx_pkct_flag == OAL_TRUE),
                                                     WAL_ATCMDSRB_GET_RX_PCKT);
    if (i_leftime == 0) {
        /* ��ʱ��û���ϱ�ɨ����� */
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{wal_atcmsrv_ioctl_get_rx_rssi::dbb_num wait for %ld ms timeout!}",
                         ((WAL_ATCMDSRB_DBB_NUM_TIME * 1000) / OAL_TIME_HZ)); /* 1s����1000ms */
        return -OAL_EINVAL;
    } else if (i_leftime < 0) {
        /* ��ʱ���ڲ����� */
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{wal_atcmsrv_ioctl_get_rx_rssi::dbb_num wait for %ld ms error!}",
                         ((WAL_ATCMDSRB_DBB_NUM_TIME * 1000) / OAL_TIME_HZ)); /* 1s����1000ms */
        return -OAL_EINVAL;
    } else {
        /* �������� */
        *pl_rx_rssi = (oal_int)pst_hmac_vap->st_atcmdsrv_get_status.s_rx_rssi;
        return OAL_SUCC;
    }
}

OAL_STATIC oal_int32 wal_atcmsrv_ioctl_set_chipcheck(oal_net_device_stru *pst_net_dev,
                                                     oal_int32 *l_chipcheck_result)
{
    oal_int32 ul_ret;
    ul_ret = wlan_device_mem_check();

    return ul_ret;
}

OAL_STATIC oal_int32 wal_atcmsrv_ioctl_get_chipcheck_result(oal_net_device_stru *pst_net_dev,
                                                            oal_int32 *l_chipcheck_result)
{
    *l_chipcheck_result = wlan_device_mem_check_result(&ull_chipcheck_total_time);

    OAM_WARNING_LOG1(0, OAM_SF_ANY, "wal_atcmsrv_ioctl_get_chipcheck_result:result[%d]", *l_chipcheck_result);
    return OAL_SUCC;
}

OAL_STATIC oal_int32 wal_atcmsrv_ioctl_set_uart_loop(oal_net_device_stru *pst_net_dev,
                                                     oal_int32 *l_uart_loop_set)
{
    return conn_test_uart_loop((oal_int8 *)&l_uart_loop_set);
}

OAL_STATIC oal_int32 wal_atcmsrv_ioctl_set_sdio_loop(oal_net_device_stru *pst_net_dev,
                                                     oal_int32 *l_wifi_chan_loop_set)
{
    return conn_test_wifi_chan_loop((oal_int8 *)&l_wifi_chan_loop_set);
}

OAL_STATIC oal_int32 wal_atcmdsrv_efuse_regs_read(oal_net_device_stru *pst_net_dev)
{
    oal_int32 l_ret;
    wal_msg_write_stru st_write_msg;
    mac_vap_stru *pst_mac_vap;
    hmac_vap_stru *pst_hmac_vap;
    oal_int32 i_leftime;
    oal_uint8 auc_param[] = { "efuse" };
    oal_uint16 us_len;

    pst_mac_vap = OAL_NET_DEV_PRIV(pst_net_dev);
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_atcmdsrv_efuse_regs_read::OAL_NET_DEV_PRIV, return null!}");
        return -OAL_EINVAL;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "wal_atcmdsrv_efuse_regs_read:mac_res_get_hmac_vap failed");
        return -OAL_EINVAL;
    }

    pst_hmac_vap->st_atcmdsrv_get_status.uc_efuse_reg_flag = OAL_FALSE;

    /* ��ӡ���мĴ��� */
    /***************************************************************************
                                ���¼���wal�㴦��
    ***************************************************************************/
    if (memcpy_s(st_write_msg.auc_value, OAL_SIZEOF(st_write_msg.auc_value),
                 auc_param, OAL_STRLEN((oal_int8 *)auc_param)) != EOK) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "wal_atcmdsrv_efuse_regs_read::memcpy fail!");
        return -OAL_EINVAL;
    }
    st_write_msg.auc_value[OAL_STRLEN((oal_int8 *)auc_param)] = '\0';

    us_len = (oal_uint16)(OAL_STRLEN((oal_int8 *)auc_param) + 1);

    WAL_WRITE_MSG_HDR_INIT(&st_write_msg, WLAN_CFGID_REG_INFO, us_len);

    l_ret = wal_send_cfg_event(pst_net_dev, WAL_MSG_TYPE_WRITE,
                               WAL_MSG_WRITE_MSG_HDR_LENGTH + us_len,
                               (oal_uint8 *)&st_write_msg, OAL_FALSE, OAL_PTR_NULL);
    if (OAL_UNLIKELY(l_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "{wal_atcmdsrv_efuse_regs_read::return err code [%d]!}\r\n", l_ret);
        return -OAL_EINVAL;
    }

    /* �����ȴ�dmac�ϱ� */
    i_leftime =
        OAL_WAIT_EVENT_INTERRUPTIBLE_TIMEOUT(pst_hmac_vap->query_wait_q,
                                             (oal_uint32)(pst_hmac_vap->st_atcmdsrv_get_status.uc_efuse_reg_flag ==
                                             OAL_TRUE), WAL_ATCMDSRB_DBB_NUM_TIME);
    if (i_leftime == 0) {
        /* ��ʱ��û���ϱ�ɨ����� */
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{wal_atcmdsrv_efuse_regs_read::efuse_regs wait for %ld ms timeout!}",
                         ((WAL_ATCMDSRB_DBB_NUM_TIME * 1000) / OAL_TIME_HZ)); /* 1s����1000ms */
        return -OAL_EINVAL;
    } else if (i_leftime < 0) {
        /* ��ʱ���ڲ����� */
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{wal_atcmdsrv_efuse_regs_read::efuse_regs wait for %ld ms error!}",
                         ((WAL_ATCMDSRB_DBB_NUM_TIME * 1000) / OAL_TIME_HZ)); /* 1s����1000ms */
        return -OAL_EINVAL;
    } else {
        return OAL_SUCC;
    }
}

OAL_STATIC void wal_atcmdsrv_efuse_info_print(void)
{
    oal_uint32 loop = 0;
    oal_uint32 high_bit = WAL_ATCMDSRV_EFUSE_REG_WIDTH - 1;
    oal_uint32 low_bit = 0;
    for (loop = 0; loop < WAL_ATCMDSRV_EFUSE_BUFF_LEN; loop++) {
        OAM_WARNING_LOG3(0, 0, "HI1102_DIE_ID: ATE bits:[%d:%d] value[0x%x]",
                         high_bit, low_bit, hmac_get_efuse_buffer(loop));
        high_bit += WAL_ATCMDSRV_EFUSE_REG_WIDTH;
        low_bit += WAL_ATCMDSRV_EFUSE_REG_WIDTH;
    }
}


OAL_STATIC oal_int32 wal_atcmdsrv_ioctl_efuse_bits_check(void)
{
    oal_int32 result = OAL_SUCC;

    st_efuse_bits = (wal_efuse_bits *)hmac_get_efuse_buffer_first_addr();

    /* ��ӡ����efuse�ֶ� */
    wal_atcmdsrv_efuse_info_print();
    /***********************************************
    (1): DIE_ID [154:0]
    (2): ӳ��λ��Ϊ
            1): die_id_0 [31:   0]
            2): die_id_1 [63:  32]
            3): die_id_2 [95:  64]
            4): die_id_3 [127: 96]
            5): die_id_4 [154:128]
    (3): ��ȡ����ֵ
    (4): ��ӡdie ID
************************************************/
    /*************************************************
    (1): Reserve0 [159:155]
    (2): Ԥ��,Ϊ��,����ֵΪ��
**************************************************/
    if (st_efuse_bits->reserve0 != 0) {
        OAM_ERROR_LOG1(0, 0, "HI1102_DIE_ID Reserve0[159:155]:expect value[0x00]error val[0x%x]",
                       st_efuse_bits->reserve0);
        result = -OAL_EINVAL;
    }

    /**************************************************
    (1): CHIP ID [167:160]
    (2): ��ȡ0x02
    (4): ����ֵΪ��
***************************************************/
    if (st_efuse_bits->chip_id != WAL_ATCMDSRV_EFUSE_CHIP_ID) {
        OAM_ERROR_LOG1(0, 0, "HI1102_DIE_ID CHIP_ID[167:160]:expect value[0x02] error val[0x%x]",
                       st_efuse_bits->chip_id);
        result = -OAL_EINVAL;
    }

    /*
    (1): Reserve1 [170:169]
    (2): Ԥ��,Ϊ��,����ֵΪ��
     */
    if (st_efuse_bits->reserve1 != 0) {
        OAM_ERROR_LOG1(0, 0, "HI1102_DIE_ID Reserve1[170:169]:expect value[0x00] error value[0x%x]",
                       st_efuse_bits->reserve1);
        result = -OAL_EINVAL;
    }

    /*
    (1): CHIP FUNCTION Value [202:171]
    (2): ӳ��λ��Ϊ
            1):chip_function_value_low  [191:171]
            2):chip_function_value_high [202:192]
    (3): ��ȡ����ֵ
     */
    /*
    (1): ADC [206:203]
    (2): [205]��[206]����ͬʱȡ1
    (3): ����ֵ�Ϸ�
     */
    if (((st_efuse_bits->adc) & WAL_ATCMDSRV_EFUSE_ADC_ERR_FLAG) == WAL_ATCMDSRV_EFUSE_ADC_ERR_FLAG) {
        OAM_ERROR_LOG1(0, 0, "HI1102_DIE_ID ADC[206:203]:expect value others error value[0x%x]\n", st_efuse_bits->adc);
        result = -OAL_EINVAL;
    }
    /*
    (1): Reserve2 [207:207]
    (2): Ԥ��,Ϊ��,����ֵΪ��
     */
    if (st_efuse_bits->reserve2 != 0) {
        OAM_ERROR_LOG1(0, 0, "HI1102_DIE_ID Reserve2:expect value[0x00]error val[207:207][0x%x]",
                       st_efuse_bits->reserve2);
        result = -OAL_EINVAL;
    }

    /*
    (1): BCPU [208:208]
    (2): ��ȡ����ֵ
     */
    /*
    (1): Reserve3 [227:209]
    (2): ӳ��λ��Ϊ
            1): reserve3_low  [223:209]
            2): reserve3_high [227:224]
    (3): Ԥ��,Ϊ��,����ֵΪ��
     */
    if ((st_efuse_bits->reserve3_low != 0) || (st_efuse_bits->reserve3_high != 0)) {
        OAM_ERROR_LOG1(0, 0, "HI1102_DIE_ID Reserve3[223:209]:expect value[0x00] error value[0x%x]\n",
                       st_efuse_bits->reserve3_low);
        OAM_ERROR_LOG1(0, 0, "HI1102_DIE_ID Reserve3[227:224]:expect value[0x00] error value[0x%x]\n",
                       st_efuse_bits->reserve3_high);
        result = -OAL_EINVAL;
    }

    /*
    (1): PMU TRIM Value [247:228]
    (2): ��ȡ����ֵ
     */
    /*
    (1): NFC PMU TRIM Value [253:248]
     */
    /*
    (1): Reserve4 [255:254]
    (2): Ԥ��,Ϊ��,����ֵΪ��
     */
    if (st_efuse_bits->reserve4 != 0) {
        OAM_ERROR_LOG1(0, 0, "HI1102_DIE_ID Reserve4[255:254]:expect value[0x00] error value[0x%x]\n",
                       st_efuse_bits->reserve4);
        result = -OAL_EINVAL;
    }

    return result;
}

OAL_STATIC oal_int32 wal_atcmsrv_ioctl_dieid_inform(oal_net_device_stru *pst_net_dev, oal_uint16 *pl_die_id)
{
    oal_int32 l_ret;
    oal_uint16 ul_loop = 0;

    /* ��ȡefuse�ֶ� */
    l_ret = wal_atcmdsrv_efuse_regs_read(pst_net_dev);
    if (l_ret != OAL_SUCC) {
        OAM_WARNING_LOG0(0, 0, "wal_atcmsrv_ioctl_efuse_check:get efuse reg fail");
        return l_ret;
    }
    /* �ϱ�efuse�ֶ� */
    for (ul_loop = 0; ul_loop < 16; ul_loop++) { /* efuse buffer�ĳ���Ϊ16 */
        pl_die_id[ul_loop] = hmac_get_efuse_buffer(ul_loop);
    }
    return OAL_SUCC;
}


OAL_STATIC oal_int32 wal_atcmsrv_ioctl_efuse_check(oal_net_device_stru *pst_net_dev,
                                                   oal_int32 *pl_efuse_check_result)
{
    oal_int32 l_ret;

    /* ��ȡefuse�ֶ� */
    l_ret = wal_atcmdsrv_efuse_regs_read(pst_net_dev);
    if (l_ret != OAL_SUCC) {
        OAM_WARNING_LOG0(0, 0, "wal_atcmsrv_ioctl_efuse_check:get efuse reg fail");
        *pl_efuse_check_result = OAL_TRUE;
        return l_ret;
    }
    /* ���efuse�ֶ� */
    l_ret = wal_atcmdsrv_ioctl_efuse_bits_check();
    if (l_ret != OAL_SUCC) {
        OAM_WARNING_LOG0(0, 0, "wal_atcmsrv_ioctl_efuse_check:check efuse reg fail");
        *pl_efuse_check_result = OAL_TRUE;
        return l_ret;
    }
    return OAL_SUCC;
}

OAL_STATIC oal_int32 wal_atcmsrv_ioctl_get_pd_tran_param(oal_net_device_stru *pst_net_dev, oal_int8 *c_pd_param)
{
    oal_int32 l_ret = OAL_SUCC;
    oal_uint32 ul_param_num = (oal_uint32)c_pd_param[WAL_ATCMDSRV_NV_WINVRAM_LENGTH - 1];
    oal_int8 *pc_pd_info;

    OAM_WARNING_LOG1(0, 0, "wal_atcmsrv_ioctl_get_pd_tran_param::s_pd_param end element is %d",
                     c_pd_param[WAL_ATCMDSRV_NV_WINVRAM_LENGTH - 1]);

    if (ul_param_num >= WLAN_CFG_DTS_NVRAM_END) {
        OAM_ERROR_LOG1(0, 0, "wal_atcmsrv_ioctl_get_pd_tran_param::num[%d] larger than array_row", ul_param_num);
        return OAL_FAIL;
    }

    pc_pd_info = (oal_int8 *)hwifi_get_nvram_param(ul_param_num);
    if (pc_pd_info == NULL) {
        OAM_WARNING_LOG0(0, 0, "wal_atcmsrv_ioctl_get_pd_tran_param::null ptr!");
        return OAL_FAIL;
    }

    if (memcpy_s(c_pd_param, OAL_MIN(WAL_ATCMDSRV_NV_WINVRAM_LENGTH, DY_CALI_PARAMS_LEN),
                 pc_pd_info, OAL_MIN(WAL_ATCMDSRV_NV_WINVRAM_LENGTH, DY_CALI_PARAMS_LEN)) != EOK) {
        OAM_ERROR_LOG0(0, 0, "wal_atcmsrv_ioctl_get_pd_tran_param::memcpy fail!");
        return OAL_FAIL;
    }

    OAL_IO_PRINT("wal_atcmsrv_ioctl_get_pd_tran_param::pd_param is %s", c_pd_param);
    return l_ret;
}

OAL_STATIC oal_int32 wal_atcmsrv_ioctl_io_test(oal_net_device_stru *pst_net_dev, oal_int16 *s_wkup_io_status)
{
    oal_int32 l_ret;
    OAM_WARNING_LOG0(0, 0, "{wal_atcmsrv_ioctl_io_test::func entered}");
    l_ret = hi1102a_dev_io_test();
    if (l_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, 0, "{wal_atcmsrv_ioctl_io_test::io test ret is %d}", l_ret);
        *s_wkup_io_status = 1;
    }

    return l_ret;
}


OAL_STATIC oal_int32 wal_atcmdsrv_ioctl_dyn_intervel_set(oal_net_device_stru *pst_net_dev, oal_int8 *c_dyn_intv)
{
    oal_int32 l_ret = -1;
#ifdef _PRE_WLAN_FIT_BASED_REALTIME_CALI
    oal_int8 c_dyn_cmd[WAL_ATCMDSRV_WIFI_CHAN_NUM][WAL_ATCMDSRV_DYN_INTVAL_LEN] = {
        "2g_dscr_interval  ",
        "5g_dscr_interval  " };
    oal_int8 c_i = 0;

    for (c_i = 0; c_i < WAL_ATCMDSRV_WIFI_CHAN_NUM; c_i++) {
        c_dyn_cmd[c_i][WAL_ATCMDSRV_DYN_INTVAL_LOCATION] = c_dyn_intv[c_i];
        l_ret = (oal_int32)wal_hipriv_dyn_cali_cfg(pst_net_dev, &c_dyn_cmd[c_i][0]);
        if (l_ret != OAL_SUCC) {
            OAM_WARNING_LOG1(0, 0, "{wal_atcmdsrv_ioctl_dyn_intervel_set::ret is %d}", l_ret);
        }
    }
#endif
    return l_ret;
}


OAL_STATIC oal_int32 wal_atcmdsrv_ioctl_pt_set(oal_net_device_stru *pst_net_dev, oal_int32 l_pt_set)
{
    if (l_pt_set) {
        OAM_WARNING_LOG0(0, 0, "wifi pull up power_on_enable gpio!\n");
        hi1102a_chip_power_on();
    } else {
        OAM_WARNING_LOG0(0, 0, "wifi pull down power_on_enable!\n");
        hi1102a_chip_power_off();
    }

    return OAL_SUCC;
}

OAL_STATIC oal_int32 wal_atcmdsrv_ioctl_selfcali_intervel_set(oal_net_device_stru *pst_net_dev,
                                                              oal_int32 l_selfcali_en)
{
    if (!l_selfcali_en) {
        wlan_cal_disable_1102a = OAL_TRUE; /* ��ֹÿ�����µ���У׼ */
    } else {
        wlan_cal_disable_1102a = OAL_FALSE; /* ʹ��ÿ�����µ���У׼ */
    }
    OAM_WARNING_LOG1(0, 0, "wal_atcmdsrv_ioctl_selfcali_intervel_set::set selfcali_interval to %d!",
                     wlan_cal_disable_1102a);
    chr_set_at_test(OAL_TRUE);

    return OAL_SUCC;
}


OAL_STATIC oal_int32 wal_atcmsrv_ioctl_set_bss_expire_age(oal_net_device_stru *pst_net_dev,
                                                          oal_uint32 ul_expire_age)
{
    hmac_set_pd_bss_expire_time((ul_expire_age < WAL_ATCMSRV_MIN_BSS_EXPIRATION_AGE)
                           ? WAL_ATCMSRV_MIN_BSS_EXPIRATION_AGE
                           : ul_expire_age);
    hmac_set_pd_bss_expire_time((hmac_get_pd_bss_expire_time() > WAL_ATCMSRV_MAX_BSS_EXPIRATION_AGE)
                           ? WAL_ATCMSRV_MAX_BSS_EXPIRATION_AGE
                           : hmac_get_pd_bss_expire_time());

    OAM_WARNING_LOG2(0, OAM_SF_CFG,
                     "wal_atcmsrv_ioctl_set_bss_expire_age::pd_bss_expire_time %d, input expire time %d",
                     hmac_get_pd_bss_expire_time(), ul_expire_age);

    return OAL_SUCC;
}

OAL_STATIC oal_int32 wal_atcmdsrv_ioctl_tas_ant_set(oal_net_device_stru *pst_net_dev, oal_int32 l_tas_ant_set)
{
    board_wifi_tas_set(l_tas_ant_set);
    return OAL_SUCC;
}


OAL_STATIC oal_int32 wal_atcmdsrv_ioctl_broadcast_loop(oal_net_device_stru *pst_net_dev,
                                                       oal_uint32 ul_broadcast_test_num)
{
    oal_uint32 ul_ret;
    oal_uint8 uc_packet_thr = 10;    /* Ĭ������ */
    oal_uint8 uc_packet_timeout = 1; /* Ĭ�ϳ�ʱʱ����1s */
    mac_vap_stru *pst_mac_vap;
    hmac_device_stru *pst_hmac_device = OAL_PTR_NULL;

    pst_mac_vap = OAL_NET_DEV_PRIV(pst_net_dev);
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_atcmdsrv_ioctl_broadcast_loop::OAL_NET_DEV_PRIV, return null!}");
        return -OAL_EINVAL;
    }
    if (pst_mac_vap->en_vap_state != MAC_VAP_STATE_UP) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_atcmdsrv_ioctl_broadcast_loop::vap is not up!}");
        return -OAL_EINVAL;
    }
    if (!IS_LEGACY_STA(pst_mac_vap)) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_atcmdsrv_ioctl_broadcast_loop::not STA!}");
        return -OAL_EINVAL;
    }
    if ((pst_mac_vap->st_cap_flag.bit_wpa2 == OAL_FALSE) || (pst_mac_vap->st_cap_flag.bit_wpa == OAL_TRUE)) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_atcmdsrv_ioctl_broadcast_loop::not wpa2!}");
        return -OAL_EINVAL;
    }
    pst_hmac_device = hmac_res_get_mac_dev(pst_mac_vap->uc_device_id);
    if (pst_hmac_device == OAL_PTR_NULL) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "{wal_atcmdsrv_ioctl_broadcast_loop::hmac_res_get_mac_dev null. device_id:%u}",
                       pst_mac_vap->uc_device_id);
        return -OAL_EINVAL;
    }

    OAM_WARNING_LOG3(0, OAM_SF_ANY,
                     "{wal_atcmdsrv_ioctl_broadcast_loop::packet_num=[%d],packet_thr=[%d],packet_timeout=[%d]}",
                     ul_broadcast_test_num, uc_packet_thr, uc_packet_timeout);

    ul_ret = wal_hipriv_packet_check_send_event(pst_net_dev, pst_mac_vap, pst_hmac_device, ul_broadcast_test_num,
                                                uc_packet_thr, uc_packet_timeout);
    if ((ul_ret == OAL_SUCC) && (pst_hmac_device->st_packet_check.en_pkt_check_result == OAL_TRUE)) {
        return OAL_SUCC;
    } else {
        return -OAL_EINVAL;
    }
}


OAL_STATIC oal_int32 wal_atcmsrv_ioctl_get_wifi_connect_info(oal_net_device_stru *pst_net_dev,
                                                             struct wal_atcmdsrv_wifi_connect_info *pst_connect_info)
{
    mac_vap_stru *pst_mac_vap;
    hmac_vap_stru *pst_hmac_vap;
    hmac_device_stru *pst_hmac_device;
    hmac_bss_mgmt_stru *pst_bss_mgmt; /* ����ɨ���bss����Ľṹ�� */
    hmac_scanned_bss_info *pst_scanned_bss_info = OAL_PTR_NULL;

    if ((pst_net_dev == OAL_PTR_NULL) || (pst_connect_info == OAL_PTR_NULL)) {
        OAM_ERROR_LOG2(0, OAM_SF_CFG, "wal_atcmsrv_ioctl_get_wifi_connect_info:null pointer.net_dev %p,connect_info %p",
                       (uintptr_t)pst_net_dev, (uintptr_t)pst_connect_info);
        return -OAL_EFAIL;
    }

    memset_s(pst_connect_info, OAL_SIZEOF(*pst_connect_info), 0, OAL_SIZEOF(*pst_connect_info));

    pst_mac_vap = OAL_NET_DEV_PRIV(pst_net_dev);
    if (pst_mac_vap == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{wal_atcmsrv_ioctl_get_wifi_connect_info::vap is null.}");
        return -OAL_EINVAL;
    }

    if (pst_mac_vap->en_vap_mode != WLAN_VAP_MODE_BSS_STA) {
        OAM_ERROR_LOG1(0, OAM_SF_ANY, "{wal_atcmsrv_ioctl_get_wifi_connect_info::invalid vap mode.vap_mode [%d]}",
                       pst_mac_vap->en_vap_mode);
        return -OAL_EINVAL;
    }

    if (pst_mac_vap->en_vap_state == MAC_VAP_STATE_UP) {
        pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
        if (pst_hmac_vap == OAL_PTR_NULL) {
            OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{wal_wifi_connect_info::get_hmac_vap fail}");
            return -OAL_EINVAL;
        }

        /* ��ȡhmac device �ṹ */
        pst_hmac_device = hmac_res_get_mac_dev(pst_mac_vap->uc_device_id);
        if (pst_hmac_device == OAL_PTR_NULL) {
            OAM_WARNING_LOG0(0, OAM_SF_SCAN, "{wal_atcmsrv_ioctl_get_wifi_connect_info::hmac_device is null.}");
            return -OAL_EINVAL;
        }

        pst_connect_info->en_status = ATCMDSRV_WIFI_CONNECTED;
        pst_connect_info->c_rssi = pst_hmac_vap->station_info.signal;
        memcpy_s(pst_connect_info->auc_bssid, WLAN_MAC_ADDR_LEN, pst_mac_vap->auc_bssid, WLAN_MAC_ADDR_LEN);

        /* ��ȡ����ɨ���bss����Ľṹ�� */
        pst_bss_mgmt = &(pst_hmac_device->st_scan_mgmt.st_scan_record_mgmt.st_bss_mgmt);
        /* ������ɾ����ǰ���� */
        oal_spin_lock(&(pst_bss_mgmt->st_lock));
        pst_scanned_bss_info = hmac_scan_find_scanned_bss_by_bssid(pst_bss_mgmt, pst_connect_info->auc_bssid);
        if (pst_scanned_bss_info == OAL_PTR_NULL) {
            OAM_WARNING_LOG4(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{wal_atcmsrv_ioctl_get_wifi_connect_info::find bss \
                fail by bssid:%02X:XX:XX:%02X:%02X:%02X}", pst_connect_info->auc_bssid[0],
                             /* auc_bssid��3��4byte��Ϊ���������ӡ */
                             pst_connect_info->auc_bssid[3], pst_connect_info->auc_bssid[4],
                             pst_connect_info->auc_bssid[5]); /* auc_bssid��5byte��Ϊ���������ӡ */

            /* ���� */
            oal_spin_unlock(&(pst_bss_mgmt->st_lock));
            return -OAL_EINVAL;
        }
        /* ���� */
        oal_spin_unlock(&(pst_bss_mgmt->st_lock));

        if (memcpy_s(pst_connect_info->auc_ssid, WLAN_SSID_MAX_LEN,
                     pst_scanned_bss_info->st_bss_dscr_info.ac_ssid, WLAN_SSID_MAX_LEN) != EOK) {
            OAM_ERROR_LOG0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "wal_atcmsrv_ioctl_get_wifi_connect_info::memcpy fail!");
            return -OAL_EINVAL;
        }
    } else {
        pst_connect_info->en_status = ATCMDSRV_WIFI_DISCONNECT;
    }

    OAM_WARNING_LOG4(0, OAM_SF_CFG,
                     "wal_atcmsrv_ioctl_get_wifi_connect_info::state %d, rssi %d, BSSID[XX:XX:XX:XX:%02X:%02X]",
                     pst_connect_info->en_status, pst_connect_info->c_rssi,
                     /* auc_bssid��4��5byte��Ϊ���������ӡ */
                     pst_connect_info->auc_bssid[4], pst_connect_info->auc_bssid[5]);

    return OAL_SUCC;
}


oal_int32 wal_atcmdsrv_wifi_priv_cmd(oal_net_device_stru *pst_net_dev, oal_ifreq_stru *pst_ifr, oal_int32 ul_cmd)
{
    wal_atcmdsrv_wifi_priv_cmd_stru st_priv_cmd;
    oal_int32 l_ret = OAL_SUCC;

    if ((pst_ifr->ifr_data == OAL_PTR_NULL) || (pst_net_dev == OAL_PTR_NULL)) {
        l_ret = -OAL_EINVAL;
        return l_ret;
    }
    /* ���û�̬���ݿ������ں�̬ */
    if (oal_copy_from_user(&st_priv_cmd, pst_ifr->ifr_data, sizeof(wal_atcmdsrv_wifi_priv_cmd_stru))) {
        l_ret = -OAL_EINVAL;
        return l_ret;
    }

    switch (st_priv_cmd.ul_cmd) {
        case WAL_ATCMDSRV_IOCTL_CMD_WI_FREQ_SET:
            l_ret = wal_atcmsrv_ioctl_set_freq(pst_net_dev, st_priv_cmd.pri_data.l_freq);
            break;
        case WAL_ATCMDSRV_IOCTL_CMD_WI_POWER_SET:
            /* l_pow��Ա�ĵ�λ�����Ƕ���?02���õĵ�λ��1db��03���õĵ�λ��0.1db,02A���õĵ�λ�Ƿ���0.01db */
            /* ���02A���õ�λ��0.01db������Ҫ��l_powת��Ϊ0.1db,��ǰ�ο�03����0.1db���� */
            l_ret = wal_atcmsrv_ioctl_set_txpower(pst_net_dev, st_priv_cmd.pri_data.l_pow);
            break;
        case WAL_ATCMDSRV_IOCTL_CMD_MODE_SET:
            l_ret = wal_atcmsrv_ioctl_set_mode(pst_net_dev, st_priv_cmd.pri_data.l_mode);
            break;
        case WAL_ATCMDSRV_IOCTL_CMD_DATARATE_SET:
            l_ret = wal_atcmsrv_ioctl_set_datarate(pst_net_dev, st_priv_cmd.pri_data.l_datarate);
            break;
        case WAL_ATCMDSRV_IOCTL_CMD_BAND_SET:
            l_ret = wal_atcmsrv_ioctl_set_bandwidth(pst_net_dev, st_priv_cmd.pri_data.l_bandwidth);
            break;
        case WAL_ATCMDSRV_IOCTL_CMD_ALWAYS_TX_SET:
            l_ret = wal_atcmsrv_ioctl_set_always_tx(pst_net_dev, st_priv_cmd.pri_data.l_awalys_tx);
            break;
        case WAL_ATCMDSRV_IOCTL_CMD_DBB_GET:
            l_ret = wal_atcmsrv_ioctl_get_dbb_num(pst_net_dev, st_priv_cmd.pri_data.auc_dbb);
            if (oal_copy_to_user(pst_ifr->ifr_data, &st_priv_cmd, sizeof(wal_atcmdsrv_wifi_priv_cmd_stru))) {
                OAM_ERROR_LOG0(0, OAM_SF_ANY, "wal_atcmdsrv_wifi_priv_cmd:Failed to copy ioctl_data to user !");
                l_ret = -OAL_EINVAL;
            }
            break;
        case WAL_ATCMDSRV_IOCTL_CMD_HW_STATUS_GET:
            l_ret = wal_atcmsrv_ioctl_get_hw_status(pst_net_dev, &st_priv_cmd.pri_data.l_fem_pa_status);
            if (oal_copy_to_user(pst_ifr->ifr_data, &st_priv_cmd, sizeof(wal_atcmdsrv_wifi_priv_cmd_stru))) {
                OAM_ERROR_LOG0(0, OAM_SF_ANY, "wal_atcmdsrv_wifi_priv_cmd:Failed to copy ioctl_data to user !");
                l_ret = -OAL_EINVAL;
            }
            break;
        case WAL_ATCMDSRV_IOCTL_CMD_ALWAYS_RX_SET:
            l_ret = wal_atcmsrv_ioctl_set_always_rx(pst_net_dev, st_priv_cmd.pri_data.l_awalys_rx);
            break;
        case WAL_ATCMDSRV_IOCTL_CMD_HW_ADDR_SET:
            l_ret = wal_atcmsrv_ioctl_set_hw_addr(pst_net_dev, st_priv_cmd.pri_data.auc_mac_addr);
            break;
        case WAL_ATCMDSRV_IOCTL_CMD_RX_PCKG_GET:
            l_ret = wal_atcmsrv_ioctl_get_rx_pckg(pst_net_dev, &st_priv_cmd.pri_data.l_rx_pkcg);
            if (oal_copy_to_user(pst_ifr->ifr_data, &st_priv_cmd, sizeof(wal_atcmdsrv_wifi_priv_cmd_stru))) {
                OAM_ERROR_LOG0(0, OAM_SF_ANY, "wal_atcmdsrv_wifi_priv_cmd:Failed to copy ioctl_data to user !");
                l_ret = -OAL_EINVAL;
            }
            break;
        case WAL_ATCMDSRV_IOCTL_CMD_PM_SWITCH:
            l_ret = wal_atcmsrv_ioctl_set_pm_switch(pst_net_dev, st_priv_cmd.pri_data.l_pm_switch);
            break;
        case WAL_ATCMDSRV_IOCTL_CMD_RX_RSSI:
            l_ret = wal_atcmsrv_ioctl_get_rx_rssi(pst_net_dev, &st_priv_cmd.pri_data.l_rx_rssi);
            if (oal_copy_to_user(pst_ifr->ifr_data, &st_priv_cmd, sizeof(wal_atcmdsrv_wifi_priv_cmd_stru))) {
                OAM_ERROR_LOG0(0, OAM_SF_ANY, "wal_atcmdsrv_wifi_priv_cmd:Failed to copy ioctl_data to user !");
                l_ret = -OAL_EINVAL;
            }
            break;
        case WAL_ATCMDSRV_IOCTL_CMD_CHIPCHECK_SET:
            l_ret = wal_atcmsrv_ioctl_set_chipcheck(pst_net_dev, &st_priv_cmd.pri_data.l_chipcheck_result);
            break;
        case WAL_ATCMDSRV_IOCTL_CMD_CHIPCHECK_RESULT:
            l_ret = wal_atcmsrv_ioctl_get_chipcheck_result(pst_net_dev, &st_priv_cmd.pri_data.l_chipcheck_result);
            if (oal_copy_to_user(pst_ifr->ifr_data, &st_priv_cmd, sizeof(wal_atcmdsrv_wifi_priv_cmd_stru))) {
                OAM_ERROR_LOG0(0, OAM_SF_ANY, "wal_atcmdsrv_wifi_priv_cmd_etc:Failed to copy ioctl_data to user !");
                l_ret = -OAL_EINVAL;
            }
            break;
        case WAL_ATCMDSRV_IOCTL_CMD_UART_LOOP_SET:
            l_ret = wal_atcmsrv_ioctl_set_uart_loop(pst_net_dev, &st_priv_cmd.pri_data.l_uart_loop_set);
            break;
        case WAL_ATCMDSRV_IOCTL_CMD_SDIO_LOOP_SET:
            l_ret = wal_atcmsrv_ioctl_set_sdio_loop(pst_net_dev, &st_priv_cmd.pri_data.l_sdio_loop_set);
            break;
        case WAL_ATCMDSRV_IOCTL_CMD_EFUSE_CHECK:
            l_ret = wal_atcmsrv_ioctl_efuse_check(pst_net_dev, &st_priv_cmd.pri_data.l_efuse_check_result);
            if (oal_copy_to_user(pst_ifr->ifr_data, &st_priv_cmd, sizeof(wal_atcmdsrv_wifi_priv_cmd_stru))) {
                OAM_ERROR_LOG0(0, OAM_SF_ANY, "wal_atcmdsrv_wifi_priv_cmd:Failed to copy ioctl_data to user !");
                l_ret = -OAL_EINVAL;
            }
            break;
        case WAL_ATCMDSRV_IOCTL_CMD_DIEID_INFORM:
            l_ret = wal_atcmsrv_ioctl_dieid_inform(pst_net_dev, (oal_uint16 *)st_priv_cmd.pri_data.die_id);
            if (oal_copy_to_user(pst_ifr->ifr_data, &st_priv_cmd, sizeof(wal_atcmdsrv_wifi_priv_cmd_stru))) {
                OAM_ERROR_LOG0(0, OAM_SF_ANY, "wal_atcmdsrv_wifi_priv_cmd:Failed to copy ioctl_data to user !");
                l_ret = -OAL_EINVAL;
            }
            break;
        case WAL_ATCMDSRV_IOCTL_CMD_SET_COUNTRY:
            l_ret = wal_atcmsrv_ioctl_set_country(pst_net_dev, st_priv_cmd.pri_data.auc_country_code);
            break;
        case WAL_ATCMDSRV_IOCTL_CMD_SET_BSS_EXPIRE_AGE:
            l_ret = wal_atcmsrv_ioctl_set_bss_expire_age(pst_net_dev, st_priv_cmd.pri_data.ul_bss_expire_age);
            break;

        case WAL_ATCMDSRV_IOCTL_CMD_GET_CONN_INFO:
            l_ret = wal_atcmsrv_ioctl_get_wifi_connect_info(pst_net_dev, &st_priv_cmd.pri_data.st_connect_info);

            OAL_IO_PRINT("wal_atcmsrv_ioctl_get_wifi_connect_info::status %d, %.32s,%02x:%02x:xx:xx:%02x:%02x,%d",
                         st_priv_cmd.pri_data.st_connect_info.en_status,
                         st_priv_cmd.pri_data.st_connect_info.auc_ssid,
                         st_priv_cmd.pri_data.st_connect_info.auc_bssid[0],
                         st_priv_cmd.pri_data.st_connect_info.auc_bssid[1],
                         st_priv_cmd.pri_data.st_connect_info.auc_bssid[4], /* auc_bssid��4byte��Ϊ���������ӡ */
                         st_priv_cmd.pri_data.st_connect_info.auc_bssid[5], /* auc_bssid��5byte��Ϊ���������ӡ */
                         st_priv_cmd.pri_data.st_connect_info.c_rssi);

            oal_copy_to_user(pst_ifr->ifr_data, &st_priv_cmd, sizeof(wal_atcmdsrv_wifi_priv_cmd_stru));
            break;
        case WAL_ATCMDSRV_IOCTL_CMD_GET_PDET_PARAM:
            l_ret = wal_atcmsrv_ioctl_get_pd_tran_param(pst_net_dev, st_priv_cmd.pri_data.ac_pd_tran_param);
            break;
        case WAL_ATCMDSRV_IOCTL_CMD_IO_TEST:
            l_ret = wal_atcmsrv_ioctl_io_test(pst_net_dev, &st_priv_cmd.pri_data.s_wkup_io_status);
            if (oal_copy_to_user(pst_ifr->ifr_data, &st_priv_cmd, sizeof(wal_atcmdsrv_wifi_priv_cmd_stru))) {
                OAM_ERROR_LOG0(0, OAM_SF_ANY, "wal_atcmdsrv_wifi_priv_cmd_etc:Failed to copy ioctl_data to user !");
                l_ret = -OAL_EINVAL;
            }
            break;
        case WAL_ATCMDSRV_IOCTL_CMD_DYN_INTERVAL:
            l_ret = wal_atcmdsrv_ioctl_dyn_intervel_set(pst_net_dev, st_priv_cmd.pri_data.c_dyn_interval);
            break;
        case WAL_ATCMDSRV_IOCTL_CMD_PT_STE:
            l_ret = wal_atcmdsrv_ioctl_pt_set(pst_net_dev, st_priv_cmd.pri_data.l_pt_set);
            break;
        case WAL_ATCMDSRV_IOCTL_CMD_SELFCALI_INTERVAL:
            l_ret = wal_atcmdsrv_ioctl_selfcali_intervel_set(pst_net_dev, st_priv_cmd.pri_data.selfcali_interval_set);
            break;
        case WAL_ATCMDSRV_IOCTL_CMD_TAS_ANT_SET:
            l_ret = wal_atcmdsrv_ioctl_tas_ant_set(pst_net_dev, st_priv_cmd.pri_data.l_tas_ant_set);
            break;
        case WAL_ATCMDSRV_IOCTL_CMD_BROADCAST_LOOP:
            l_ret = wal_atcmdsrv_ioctl_broadcast_loop(pst_net_dev, st_priv_cmd.pri_data.ul_broadcast_test_num);
            break;

        default:
            break;
    }
    return l_ret;
}
#endif

