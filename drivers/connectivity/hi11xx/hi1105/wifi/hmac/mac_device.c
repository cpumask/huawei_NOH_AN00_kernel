

/* 1 ͷ�ļ����� */
#include "oam_ext_if.h"
#include "frw_ext_if.h"
#include "mac_device.h"
#include "mac_resource.h"
#include "mac_regdomain.h"
#ifdef _PRE_WLAN_FEATURE_HIEX
#include "mac_hiex.h"
#endif
#ifdef _PRE_WLAN_FEATURE_DFS
#include "hmac_dfs.h"
#endif

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_MAC_DEVICE_C

/* macģ����ӵ�ȫ�ֿ��Ʊ��� */
OAL_STATIC mac_board_stru g_st_mac_board;
mac_board_stru *g_pst_mac_board = &g_st_mac_board;

#ifdef _PRE_WLAN_FEATURE_WMMAC
oal_bool_enum_uint8 g_en_wmmac_switch = OAL_TRUE;
#endif

/* ����ָ����ÿ��chip��mac device��Ƶ������ */
uint8_t g_auc_mac_device_radio_cap[WLAN_SERVICE_DEVICE_MAX_NUM_PER_CHIP] = {
    MAC_DEVICE_2G_5G
};

uint32_t g_ul_ddr_freq = 0;
mac_device_custom_cfg_stru g_st_mac_device_custom_cfg;
uint8_t g_optimized_feature_switch_bitmap = 0;

/* ÿ��chip�µ�mac device�����������ƻ� */
/* 02��51ÿ��chip��ֻ��1��device��51˫оƬʱÿ��chip����������ȵ� */
/* 03 DBDC����ʱmac device����ȡ2(���Ǿ�̬DBDC), ����ȡ1 */
/* Ĭ�ϳ�ʼ��ֵΪ��̬DBDC��ȡHAL Device0��������ֵ */
OAL_STATIC mac_device_capability_stru g_st_mac_device_capability[WLAN_SERVICE_DEVICE_MAX_NUM_PER_CHIP] = {
    {
        /* nss num */
        WLAN_HAL0_NSS_NUM,      /* ��ֵ����hal device����һ�£����ƻ��򿪺���ͳһˢ��һ�� */
        /* mib_bw_supp_width */
        WLAN_HAL0_BW_MAX_WIDTH, /* TBD:03 ini FPGA 20_40M,ASIC 160M */
        /* NB */
        WLAN_HAL0_NB_IS_EN,
        /* 1024QAM */
        WLAN_HAL0_1024QAM_IS_EN,

        /* 80211 MC */
        WLAN_HAL0_11MC_IS_EN,
        /* ldpc coding */
        WLAN_HAL0_LDPC_IS_EN,
        /* tx stbc */
        WLAN_HAL0_TX_STBC_IS_EN, /* ��ֵ����hal device����һ�£����ƻ��򿪺���ͳһˢ��һ�� */
        /* rx stbc */
        WLAN_HAL0_RX_STBC_IS_EN, /* ��ֵ����hal device����һ�£����ƻ��򿪺���ͳһˢ��һ�� */

        /* su bfer */
        WLAN_HAL0_SU_BFER_IS_EN, /* ��ֵ����hal device����һ�£����ƻ��򿪺���ͳһˢ��һ�� */
        /* su bfee */
        WLAN_HAL0_SU_BFEE_IS_EN, /* ��ֵ����hal device����һ�£����ƻ��򿪺���ͳһˢ��һ�� */
        /* mu bfer */
        WLAN_HAL0_MU_BFER_IS_EN, /* ��ֵ����hal device����һ�£����ƻ��򿪺���ͳһˢ��һ�� */
        /* mu bfee */
        WLAN_HAL0_MU_BFEE_IS_EN, /* ��ֵ����hal device����һ�£����ƻ��򿪺���ͳһˢ��һ�� */
        /* 11ax */
        WLAN_HAL0_11AX_IS_EN,
    },
};
mac_device_capability_stru *g_pst_mac_device_capability = &g_st_mac_device_capability[0];

mac_blacklist_info_stru g_ast_blacklist[WLAN_SERVICE_DEVICE_SUPPORT_MAX_NUM_SPEC * WLAN_VAP_SUPPORT_MAX_NUM_LIMIT];

#ifdef _PRE_WLAN_FEATURE_NEGTIVE_DET
mac_cfg_pk_mode_stru g_st_wifi_pk_mode_status = { 0 }; /* PK modeģʽ */
#endif
/* 3 ����ʵ�� */
uint32_t mac_board_init(void)
{
    memset_s(g_pst_mac_board, OAL_SIZEOF(mac_board_stru), 0, OAL_SIZEOF(mac_board_stru));
    return OAL_SUCC;
}


void mac_random_mac_oui_init(mac_device_stru *pst_mac_device)
{
    /* ��ʼ�����mac ouiΪ0(3���ֽڶ���0),
     * ȷ��ֻ��ϵͳ�·���Чmac oui�Ž������mac��ַɨ��(�����macɨ�迪�ش򿪵������)
     */
    pst_mac_device->en_is_random_mac_addr_scan = OAL_FALSE;
    pst_mac_device->auc_mac_oui[0] = 0x0;
    pst_mac_device->auc_mac_oui[1] = 0x0;
    pst_mac_device->auc_mac_oui[2] = 0x0;
}

#ifdef _PRE_WLAN_FEATURE_HIEX

void mac_hiex_cap_init(mac_device_stru *pst_mac_device)
{
    if (memcpy_s(&pst_mac_device->st_hiex_cap, OAL_SIZEOF(mac_hiex_cap_stru), &g_st_default_hiex_cap,
        OAL_SIZEOF(mac_hiex_cap_stru)) != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "mac_hiex_cap_init::hiex cap memcpy fail!");
    }
}
#endif

OAL_STATIC uint32_t mac_device_init_protocol_cand_cap(mac_device_stru *pst_mac_device,
    uint32_t ul_chip_ver, uint8_t uc_device_id)
{
#ifdef _PRE_WLAN_FEATURE_11AX
    pst_mac_device->en_protocol_cap = g_wlan_spec_cfg->feature_11ax_is_open ?
        WLAN_PROTOCOL_CAP_HE : WLAN_PROTOCOL_CAP_VHT;
#endif

    pst_mac_device->en_band_cap =
        mac_device_check_5g_enable(uc_device_id) ? WLAN_BAND_CAP_2G_5G : WLAN_BAND_CAP_2G;

    return OAL_SUCC;
}

void mac_device_init_params(mac_device_stru *pst_mac_device)
{
    pst_mac_device->en_max_bandwidth = WLAN_BAND_WIDTH_BUTT;
    pst_mac_device->en_max_band = WLAN_BAND_BUTT;
    pst_mac_device->uc_max_channel = 0;
    pst_mac_device->ul_beacon_interval = WLAN_BEACON_INTVAL_DEFAULT;

    pst_mac_device->st_bss_id_list.us_num_networks = 0;
}

OAL_STATIC oal_void mac_device_p2p_info_init(mac_p2p_info_stru *p_p2p_info)
{
    p_p2p_info->uc_p2p_device_num = 0;
    p_p2p_info->uc_p2p_goclient_num = 0;
    p_p2p_info->pst_primary_net_device = OAL_PTR_NULL; /* ��ʼ����net_device Ϊ��ָ�� */
    p_p2p_info->p2p_scenes = MAC_P2P_SCENES_LOW_LATENCY;
}


uint32_t mac_device_init(mac_device_stru *pst_mac_device,
                               uint32_t ul_chip_ver,
                               uint8_t uc_chip_id,
                               uint8_t uc_device_id)
{
    uint8_t uc_device_id_per_chip;

    if (pst_mac_device == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{mac_device_init::pst_mac_device null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    memset_s(pst_mac_device, OAL_SIZEOF(mac_device_stru), 0, OAL_SIZEOF(mac_device_stru));

    /* ��ʼ��device������ */
    pst_mac_device->uc_chip_id = uc_chip_id;
    pst_mac_device->uc_device_id = uc_device_id;

    /* ��ʼ��device�����һЩ���� */
    mac_device_init_params(pst_mac_device);

    /* 03����ҵ��device,00 01,ȡ��ͬ���ƻ�,51˫оƬ00 11,ȡͬһ�����ƻ� */
    uc_device_id_per_chip = uc_device_id - uc_chip_id;
    if (uc_device_id_per_chip >= WLAN_SERVICE_DEVICE_MAX_NUM_PER_CHIP) {
        oam_error_log3(0, OAM_SF_ANY,
                       "{mac_device_init::pst_mac_device device id[%d] chip id[%d] >support[%d].}",
                       uc_device_id, uc_chip_id, WLAN_SERVICE_DEVICE_MAX_NUM_PER_CHIP);
        uc_device_id_per_chip = 0;
    }

    /* �����ƻ�����Ϣ���浽mac device�ṹ���� */
    /* ��ʼ��mac device������ */
    if (EOK != memcpy_s(&pst_mac_device->st_device_cap, OAL_SIZEOF(mac_device_capability_stru),
                        &g_pst_mac_device_capability[uc_device_id_per_chip], OAL_SIZEOF(mac_device_capability_stru))) {
        oam_error_log0(0, OAM_SF_ANY, "mac_device_init::memcpy fail!");
        return OAL_FAIL;
    }

#ifdef _PRE_WLAN_FEATURE_SMPS
    /* ����03��˵����һ��deviceֻ֧��SISO,����ΪMIMO SAVE״̬�Ƿ�û��Ӱ�죬��ȷ��; ��Щ������m2s�л��лᶯ̬�仯 */
    MAC_DEVICE_GET_MODE_SMPS(pst_mac_device) = WLAN_MIB_MIMO_POWER_SAVE_MIMO;
#endif

    pst_mac_device->en_device_state = OAL_TRUE;
    pst_mac_device->en_reset_switch = OAL_FALSE;

    /* Ĭ�Ϲر�wmm,wmm��ʱ��������Ϊ0 */
    pst_mac_device->en_wmm = OAL_TRUE;

    /* ����оƬ�汾��ʼ��device������Ϣ */
    if (mac_device_init_protocol_cand_cap(pst_mac_device, ul_chip_ver, uc_device_id) != OAL_SUCC) {
        return OAL_ERR_CODE_CONFIG_UNSUPPORT;
    }

    /* ��ʼ��vap numͳ����Ϣ */
    pst_mac_device->uc_vap_num = 0;
    pst_mac_device->uc_sta_num = 0;
    mac_device_p2p_info_init(&pst_mac_device->st_p2p_info);

    /* ��ʼ��Ĭ�Ϲ����� */
    mac_init_regdomain();

    /* ��ʼ���ŵ��б� */
    mac_init_channel_list();

    /* ��ʼ����λ״̬ */
    MAC_DEV_RESET_IN_PROGRESS(pst_mac_device, OAL_FALSE);
    pst_mac_device->us_device_reset_num = 0;

    /* Ĭ�Ϲر�DBAC���� */
    pst_mac_device->en_dbac_enabled = OAL_TRUE;

    pst_mac_device->en_dbdc_running = OAL_FALSE;
    mac_set_2040bss_switch(pst_mac_device, OAL_FALSE);

#ifdef _PRE_PRODUCT_ID_HI110X_DEV
    pst_mac_device->uc_in_suspend = OAL_FALSE;
#endif

    pst_mac_device->uc_arpoffload_switch = OAL_FALSE;

    pst_mac_device->uc_wapi = OAL_FALSE;

    /* AGC��ͨ��Ĭ��Ϊ����Ӧ   */
#if defined(_PRE_PRODUCT_ID_HI110X_DEV)
    pst_mac_device->uc_scan_count = 0;
#endif

    mac_random_mac_oui_init(pst_mac_device);

#ifdef _PRE_WLAN_FEATURE_DFS
    mac_dfs_init(pst_mac_device);
#endif /* #ifdef _PRE_WLAN_FEATURE_DFS */

#ifdef _PRE_WLAN_FEATURE_HIEX
    mac_hiex_cap_init(pst_mac_device);
#endif

    return OAL_SUCC;
}


uint32_t mac_device_exit(mac_device_stru *pst_device)
{
    if (oal_unlikely(pst_device == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{dmac_device_exit::pst_device null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_device->uc_vap_num = 0;
    pst_device->uc_sta_num = 0;
    pst_device->st_p2p_info.uc_p2p_device_num = 0;
    pst_device->st_p2p_info.uc_p2p_goclient_num = 0;

    mac_res_free_dev(pst_device->uc_device_id);

    mac_device_set_state(pst_device, OAL_FALSE);

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_11AX

uint8_t mac_device_trans_bandwith_to_he_capinfo(wlan_bw_cap_enum_uint8 en_max_op_bd)
{
    uint8_t uc_he_band_width_set = 0;
    switch (en_max_op_bd) {
        case WLAN_BW_CAP_20M:
            return uc_he_band_width_set;
        case WLAN_BW_CAP_40M:
            uc_he_band_width_set |= BIT0; /* Bit0:ָʾ2.4G֧��40MHz */
            return uc_he_band_width_set;
        case WLAN_BW_CAP_80M:
            uc_he_band_width_set |= BIT0 | BIT1; /* Bit1:ָʾ5G֧��40MHz\80MHz */
            return uc_he_band_width_set;
        case WLAN_BW_CAP_160M:
            uc_he_band_width_set |= BIT0 | BIT1 | BIT2; /* Bit2:ָʾ5G֧��160MHz */
            return uc_he_band_width_set;
        case WLAN_BW_CAP_80PLUS80:
            uc_he_band_width_set |= BIT0 | BIT1 | BIT2 | BIT3; /* Bit3:ָʾ5G֧��80+80MHz */
            return uc_he_band_width_set;
        default:
            oam_error_log1(0, OAM_SF_ANY,
                           "{mac_device_trans_bandwith_to_he_capinfo::bandwith[%d] is invalid.}", en_max_op_bd);
            return WLAN_MIB_VHT_SUPP_WIDTH_BUTT;
    }
}
#endif


uint32_t mac_chip_exit(mac_board_stru *pst_board, mac_chip_stru *pst_chip)
{
    if (oal_unlikely(pst_chip == OAL_PTR_NULL || pst_board == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_chip_init::param null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_chip->uc_device_nums = 0;

    /* destroy�������״̬��ΪFALSE */
    pst_chip->en_chip_state = OAL_FALSE;

    return OAL_SUCC;
}


uint32_t mac_board_exit(mac_board_stru *pst_board)
{
    if (oal_unlikely(pst_board == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{mac_board_exit::pst_board null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    return OAL_SUCC;
}
void mac_device_set_dfr_reset(mac_device_stru *pst_mac_device, uint8_t uc_device_reset_in_progress)
{
    pst_mac_device->uc_device_reset_in_progress = uc_device_reset_in_progress;
}

void mac_device_set_state(mac_device_stru *pst_mac_device, uint8_t en_device_state)
{
    pst_mac_device->en_device_state = en_device_state;
}


wlan_mib_vht_supp_width_enum_uint8 mac_device_trans_bandwith_to_vht_capinfo(wlan_bw_cap_enum_uint8 en_max_op_bd)
{
    switch (en_max_op_bd) {
        case WLAN_BW_CAP_20M:
        case WLAN_BW_CAP_40M:
        case WLAN_BW_CAP_80M:
            return WLAN_MIB_VHT_SUPP_WIDTH_80;
        case WLAN_BW_CAP_160M:
            return WLAN_MIB_VHT_SUPP_WIDTH_160;
        case WLAN_BW_CAP_80PLUS80:
            return WLAN_MIB_VHT_SUPP_WIDTH_80PLUS80;
        default:
            oam_error_log1(0, OAM_SF_ANY,
                           "{mac_device_trans_bandwith_to_vht_capinfo::bandwith[%d] is invalid.}",
                           en_max_op_bd);
            return WLAN_MIB_VHT_SUPP_WIDTH_BUTT;
    }
}


uint32_t mac_device_check_5g_enable(uint8_t uc_device_id)
{
    uint8_t        uc_device_id_per_chip;
    mac_device_stru *pst_mac_device;

    pst_mac_device = mac_res_get_dev(uc_device_id);

    if (pst_mac_device == OAL_PTR_NULL) {
        oam_warning_log1(0, OAM_SF_ANY, "mac_device_check_5g_enable::get dev fail uc_device_id[%d]", uc_device_id);
        return OAL_FALSE;
    }

    /* 03����ҵ��device,00 01,ȡ��ͬ���ƻ�,51˫оƬ00 11,ȡͬһ�����ƻ� */
    uc_device_id_per_chip = uc_device_id - pst_mac_device->uc_chip_id;

    return !!(g_auc_mac_device_radio_cap[uc_device_id_per_chip] & MAC_DEVICE_5G);
}
uint32_t mac_chip_init(mac_chip_stru *pst_chip, uint8_t uc_device_max)
{
    pst_chip->uc_assoc_user_cnt = 0;
    pst_chip->uc_active_user_cnt = 0;

    /* ����device���� */
    pst_chip->uc_device_nums = uc_device_max;

    /* ��ʼ������ٽ�state��ΪTRUE */
    pst_chip->en_chip_state = OAL_TRUE;

    return OAL_SUCC;
}
void mac_device_set_beacon_interval(mac_device_stru *pst_mac_device, uint32_t ul_beacon_interval)
{
    pst_mac_device->ul_beacon_interval = ul_beacon_interval;
}

void mac_chip_inc_assoc_user(mac_chip_stru *pst_mac_chip)
{
    pst_mac_chip->uc_assoc_user_cnt++;
    oam_warning_log1(0, OAM_SF_UM, "{mac_chip_inc_assoc_user::uc_asoc_user_cnt[%d].}", pst_mac_chip->uc_assoc_user_cnt);
    if (pst_mac_chip->uc_assoc_user_cnt == 0xFF) {
        oam_error_log0(0, OAM_SF_UM, "{mac_chip_inc_assoc_user::uc_asoc_user_cnt=0xFF now!}");
        oam_report_backtrace();
    }
}

void mac_chip_dec_assoc_user(mac_chip_stru *pst_mac_chip)
{
    oam_warning_log1(0, OAM_SF_UM, "{mac_chip_dec_assoc_user::uc_asoc_user_cnt[%d].}", pst_mac_chip->uc_assoc_user_cnt);
    if (pst_mac_chip->uc_assoc_user_cnt == 0) {
        oam_error_log0(0, OAM_SF_UM, "{mac_chip_dec_assoc_user::uc_assoc_user_cnt is already zero.}");
        oam_report_backtrace();
    } else {
        pst_mac_chip->uc_assoc_user_cnt--;
    }
}


void *mac_device_get_all_rates(mac_device_stru *pst_dev)
{
    return (void *)pst_dev->st_mac_rates_11g;
}


void mac_blacklist_get_pointer(wlan_vap_mode_enum_uint8 en_vap_mode,
                                   uint8_t uc_chip_id, uint8_t uc_dev_id,
                                   uint8_t uc_vap_id, mac_blacklist_info_stru **pst_blacklist_info)
{
    uint8_t uc_device_index;
    uint8_t uc_vap_index;
    uint16_t us_array_index = 0;

    uc_device_index = uc_dev_id;

    uc_vap_index = uc_vap_id;

    if (en_vap_mode == WLAN_VAP_MODE_BSS_AP || en_vap_mode == WLAN_VAP_MODE_BSS_STA) {
        us_array_index = uc_device_index * WLAN_VAP_SUPPORT_MAX_NUM_LIMIT + uc_vap_index;
        if (us_array_index >= WLAN_SERVICE_DEVICE_SUPPORT_MAX_NUM_SPEC * WLAN_VAP_SUPPORT_MAX_NUM_LIMIT) {
            oam_error_log2(uc_vap_index, OAM_SF_ANY,
                           "{mac_blacklist_get_pointer::en_vap_mode=%d, vap index=%d is wrong.}",
                           en_vap_mode, us_array_index);
            return;
        }
    } else {
        oam_error_log1(uc_vap_index, OAM_SF_ANY, "{mac_blacklist_get_pointer::en_vap_mode=%d is wrong.}", en_vap_mode);
        return;
    }

    *pst_blacklist_info = &g_ast_blacklist[us_array_index];
    g_ast_blacklist[us_array_index].uc_blacklist_vap_index = uc_vap_index;
    g_ast_blacklist[us_array_index].uc_blacklist_device_index = uc_device_index;

}

void mac_set_dual_sta_mode(uint8_t mode)
{
    mac_device_stru *mac_device = mac_res_get_dev(0);
    mac_device->is_dual_sta_mode = mode;
}

uint8_t mac_is_dual_sta_mode(void)
{
    mac_device_stru *mac_device = mac_res_get_dev(0);
    return mac_device->is_dual_sta_mode;
}

