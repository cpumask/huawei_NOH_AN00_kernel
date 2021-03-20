
#ifdef _PRE_WLAN_1102A_CHR
#include "board.h"
#endif
/* 1 ͷ�ļ����� */
#include "oal_ext_if.h"
#include "oam_ext_if.h"
#include "plat_pm_wlan.h"
#include "hmac_dfx.h"
#include "hmac_ext_if.h"
#include "plat_cali.h"
#include "hmac_device.h"
#include "hmac_resource.h"
#include "hmac_config.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_DFX_C

/* 2 ȫ�ֱ������� */
OAL_STATIC oam_cfg_data_stru g_ast_cfg_data[OAM_CFG_TYPE_BUTT] = {
    { OAM_CFG_TYPE_MAX_ASOC_USER, "USER_SPEC", "max_asoc_user", 31 },
};

#if defined(_PRE_PRODUCT_ID_HI110X_HOST)
oal_uint16 wlan_assoc_user_max_num = 32; /* �����û��������� Root APģʽ��Ϊ32��,Repeaterģʽ����15�� */
oal_uint16 hmac_get_wlan_assoc_user_max_num(oal_void)
{
    return wlan_assoc_user_max_num;
}
oal_void hmac_set_wlan_assoc_user_max_num(oal_uint16 assoc_user_max_num)
{
    wlan_assoc_user_max_num = assoc_user_max_num;
}

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
/* ����WL_L2_DRAM��С���ƣ�Ŀǰ��ʱ����2��ҵ��vap�������񿪷Ŵ������Ż� TBD */
oal_uint32 g_ul_wlan_vap_max_num_per_device = 4 + 1; /* 4��AP + 1������vap */
#else
oal_uint32 g_ul_wlan_vap_max_num_per_device = 4 + 1; /* 4��AP + 1������vap */
#endif
oal_uint32 hmac_get_wlan_vap_max_num_per_device(oal_void)
{
    return g_ul_wlan_vap_max_num_per_device;
}
oal_void hmac_set_wlan_vap_max_num_per_device(oal_uint32 ul_wlan_vap_max_num_per_device)
{
    g_ul_wlan_vap_max_num_per_device = ul_wlan_vap_max_num_per_device;
}
#endif
#define CHR_QUERY_WIFI_INFO_TIME  (5 * OAL_TIME_HZ)
#define CHR_WEB_FAIL_ROAM_RX_RATE_TH 13500 /* CHR�ϱ����������¼��������ε�rx rate���� */
#define CHR_WEB_FAIL_ROAM_RSSI_TH (-55) /* CHR�ϱ����������¼��������ε�RSSI����,���ڸ����޲ſ��ܴ������� */
#define CHR_WEB_FAIL_ROAM_MIN_INTERVAL 600000 /* CHR�������ε���С���, 10���� */
#define CHR_WEB_FAIL_ROAM_PROTECT_PERIOD 60000 /* �չ����ϻ��в�����������,��������һ��������1����,�������ڲ����� */
/* 3 ����ʵ��  TBD �û��������� */

oal_int32 oam_cfg_get_item_by_id(oam_cfg_type_enum_uint16 en_cfg_type)
{
    oal_uint32 ul_loop;

    for (ul_loop = 0; ul_loop < OAM_CFG_TYPE_BUTT; ul_loop++) {
        if (en_cfg_type == g_ast_cfg_data[ul_loop].en_cfg_type) {
            break;
        }
    }

    if (ul_loop == OAM_CFG_TYPE_BUTT) {
        OAL_IO_PRINT("oam_cfg_get_item_by_id::get cfg item failed!\n");
        return -OAL_FAIL;
    }

    return g_ast_cfg_data[ul_loop].l_val;
}

#if ((defined(_PRE_PRODUCT_ID_HI110X_DEV)) || (defined(_PRE_PRODUCT_ID_HI110X_HOST)))

oal_void hmac_custom_init(oal_uint32 ul_psta_enable)
{
    /* Ӳ������:3��STA; 2��AP */
    /* ������:
     * 1)AP ģʽ:  2��ap + 1������vap
     * 2)STA ģʽ: 3��sta + 1������vap
     * 3)STA+P2P����ģʽ:  1��sta + 1��P2P_dev + 1��P2P_GO/Client + 1������vap
     * 4)STA+Proxy STA����ģʽ:  1��sta + ?��proxy STA + 1������vap
     */
    hmac_set_wlan_assoc_user_max_num(8); /* �����û�����������8�� */
    hmac_set_wlan_vap_max_num_per_device(WLAN_SERVICE_VAP_MAX_NUM_PER_DEVICE + WLAN_CONFIG_VAP_MAX_NUM_PER_DEVICE);
}

#else

oal_void hmac_custom_init(oal_uint32 ul_psta_enable)
{
    hmac_set_wlan_assoc_user_max_num(WLAN_ASSOC_USER_MAX_NUM_LIMIT);
    hmac_set_wlan_vap_max_num_per_device(WLAN_SERVICE_VAP_MAX_NUM_PER_DEVICE + WLAN_CONFIG_VAP_MAX_NUM_PER_DEVICE);
}
#endif


OAL_STATIC oal_uint32 oam_cfg_restore_all_item(oal_int32 al_default_cfg_data[])
{
    oal_uint32 ul_loop;

    for (ul_loop = 0; ul_loop < OAM_CFG_TYPE_BUTT; ul_loop++) {
        g_ast_cfg_data[ul_loop].l_val = al_default_cfg_data[ul_loop];
    }

    return OAL_SUCC;
}


oal_int32 oam_cfg_get_all_item(oal_void)
{
    oal_int32     al_default_cfg_data[OAM_CFG_TYPE_BUTT] = {0};
    oal_uint8    *puc_plaintext = OAL_PTR_NULL;
    oal_uint8    *puc_ciphertext = OAL_PTR_NULL;
    oal_uint32    ul_file_size = 0;
    oal_uint32    ul_loop;
    oal_int32     l_ret;
    oal_int       i_key[OAL_AES_KEYSIZE_256] = { 0x1, 0x2, 0x3, 0x4d, 0x56, 0x10, 0x11, 0x12,
                                                 0x1, 0x2, 0x3, 0x4d, 0x56, 0x10, 0x11, 0x12,
                                                 0x1, 0x2, 0x3, 0x4d, 0x56, 0x10, 0x11, 0x12,
                                                 0x1, 0x2, 0x3, 0x4d, 0x56, 0x10, 0x11, 0x12 };

    oal_aes_key_stru st_aes_key;

    /* ����Ĭ�����ã������ȡ�����ļ�����Ϣ��ʱ���м���ʧ�ܵ����������Ҫ�ָ�
       ǰ��ȫ��������Ϣ������ģ����ص�ʱ����԰���Ĭ�����ü���
 */
    for (ul_loop = 0; ul_loop < OAM_CFG_TYPE_BUTT; ul_loop++) {
        al_default_cfg_data[ul_loop] = g_ast_cfg_data[ul_loop].l_val;
    }

    /* ��ȡ�ļ���С����ȡ�ļ�ָ�� */
    l_ret = oal_file_size(&ul_file_size);
    if (l_ret != OAL_SUCC) {
        OAL_IO_PRINT("oam_cfg_get_all_item::get file size failed!\n");
        return l_ret;
    }

    /* �������ļ��е��������ݶ���һ�����������ʱ�����Ǽ��ܵ� */
    puc_ciphertext = oal_memalloc(ul_file_size + OAM_CFG_STR_END_SIGN_LEN);
    if (puc_ciphertext == OAL_PTR_NULL) {
        OAL_IO_PRINT("oam_cfg_get_all_item::alloc ciphertext buf failed! load ko with default cfg!\n");
        return OAL_ERR_CODE_PTR_NULL;
    }
    memset_s(puc_ciphertext, ul_file_size + OAM_CFG_STR_END_SIGN_LEN, 0, ul_file_size + OAM_CFG_STR_END_SIGN_LEN);

    l_ret = oam_cfg_read_file_to_buf((oal_int8 *)puc_ciphertext, ul_file_size);
    if (l_ret != OAL_SUCC) {
        OAL_IO_PRINT("oam_cfg_get_all_item::get cfg data from file failed! fail id-->%d\n", l_ret);
        oal_free(puc_ciphertext);
        return l_ret;
    }

    /* �������Ŀռ䣬�������Ľ��� */
    puc_plaintext = oal_memalloc(ul_file_size + OAM_CFG_STR_END_SIGN_LEN);
    if (puc_plaintext == OAL_PTR_NULL) {
        OAL_IO_PRINT("oam_cfg_get_all_item::alloc pc_plaintext buf failed! load ko with default cfg!\n");
        oal_free(puc_ciphertext);

        return OAL_ERR_CODE_PTR_NULL;
    }
    memset_s(puc_plaintext, ul_file_size + OAM_CFG_STR_END_SIGN_LEN, 0, ul_file_size + OAM_CFG_STR_END_SIGN_LEN);

    /* ���� */
    l_ret = (oal_int32)oal_aes_expand_key(&st_aes_key, (oal_uint8 *)i_key, OAL_AES_KEYSIZE_256);
    if (l_ret != OAL_SUCC) {
        oal_free(puc_plaintext);
        oal_free(puc_ciphertext);

        return l_ret;
    }

    oam_cfg_decrypt_all_item(&st_aes_key, (oal_int8 *)puc_ciphertext,
                             (oal_int8 *)puc_plaintext, ul_file_size);

    /* ��ȡ�����ļ���ÿһ�����Ϣ�����浽OAM�ڲ��ṹ�� */
    for (ul_loop = 0; ul_loop < OAM_CFG_TYPE_BUTT; ul_loop++) {
        l_ret = oam_cfg_get_one_item((oal_int8 *)puc_plaintext,
                                     g_ast_cfg_data[ul_loop].pc_section,
                                     g_ast_cfg_data[ul_loop].pc_key,
                                     &g_ast_cfg_data[ul_loop].l_val);
        /* �����ȡĳһ����ֵ���ɹ�����ָ��������Ĭ��ֵ */
        if (l_ret != OAL_SUCC) {
            OAL_IO_PRINT("oam_cfg_get_all_item::get cfg item fail! ul_loop=%d\n", ul_loop);

            oam_cfg_restore_all_item(al_default_cfg_data);
            oal_free(puc_plaintext);
            oal_free(puc_ciphertext);

            return l_ret;
        }
    }

    /* �ͷŻ����� */
    oal_free(puc_plaintext);
    oal_free(puc_ciphertext);

    return OAL_SUCC;
}
#ifdef _PRE_WLAN_1102A_CHR

/**********************ȫ�ֱ���****************************/
/* ȥ��������7��ԭ��(0~6),Ĭ��ֵ����Ϊ7����ʾû��ȥ�������� */
hmac_chr_disasoc_reason_stru g_hmac_chr_disasoc_reason = { 0, DMAC_DISASOC_MISC_BUTT };

/* ��������, ����4��˽�ж�������5200-5203 */
oal_uint16 g_hmac_chr_connect_code = 0;

hmac_chr_del_ba_info_stru g_hmac_chr_del_ba_info = { 0, 0, MAC_UNSPEC_REASON };

/**********************��ȡȫ�ֱ�����ַ****************************/
hmac_chr_disasoc_reason_stru *hmac_chr_disasoc_reason_get_pointer(void)
{
    return &g_hmac_chr_disasoc_reason;
}

oal_uint16 *hmac_chr_connect_code_get_pointer(void)
{
    return &g_hmac_chr_connect_code;
}

hmac_chr_del_ba_info_stru *hmac_chr_del_ba_info_get_pointer(void)
{
    return &g_hmac_chr_del_ba_info;
}

/* �ظ�ȫ�������ĳ�ʼֵ */
oal_void hmac_chr_info_clean(void)
{
    g_hmac_chr_disasoc_reason.us_user_id = 0;
    g_hmac_chr_disasoc_reason.en_disasoc_reason = DMAC_DISASOC_MISC_BUTT;
    g_hmac_chr_connect_code = 0;
    g_hmac_chr_del_ba_info.uc_ba_num = 0;
    g_hmac_chr_del_ba_info.uc_del_ba_tid = 0;
    g_hmac_chr_del_ba_info.en_del_ba_reason = MAC_UNSPEC_REASON;

    return;
}

/**********************CHR���ͻ�ȡ****************************/
/* �ֽ׶�CHRֻ����STA״̬(������P2P)�����Բ�����vap_id */
/* ��� */
oal_void hmac_chr_set_disasoc_reason(oal_uint16 user_id, oal_uint16 reason_id)
{
    hmac_chr_disasoc_reason_stru *pst_disasoc_reason = OAL_PTR_NULL;

    pst_disasoc_reason = hmac_chr_disasoc_reason_get_pointer();

    pst_disasoc_reason->us_user_id = user_id;
    pst_disasoc_reason->en_disasoc_reason = (dmac_disasoc_misc_reason_enum)reason_id;

    return;
}
/* ��ȡ */
oal_void hmac_chr_get_disasoc_reason(hmac_chr_disasoc_reason_stru *pst_disasoc_reason)
{
    hmac_chr_disasoc_reason_stru *pst_disasoc_reason_temp = OAL_PTR_NULL;

    pst_disasoc_reason_temp = hmac_chr_disasoc_reason_get_pointer();

    pst_disasoc_reason->us_user_id = pst_disasoc_reason_temp->us_user_id;
    pst_disasoc_reason->en_disasoc_reason = pst_disasoc_reason_temp->en_disasoc_reason;

    return;
}

/* ��� */
/* ����ɾ���ۺϵ����� ����ͳ�� */
oal_void hmac_chr_set_del_ba_info(oal_uint8 uc_tid, oal_uint16 reason_id)
{
    hmac_chr_del_ba_info_stru *pst_del_ba_info = OAL_PTR_NULL;

    pst_del_ba_info = hmac_chr_del_ba_info_get_pointer();

    pst_del_ba_info->uc_del_ba_tid = uc_tid;
    pst_del_ba_info->en_del_ba_reason = (mac_reason_code_enum)reason_id;

    return;
}

/* ��ȡ */
oal_void hmac_chr_get_del_ba_info(mac_vap_stru *pst_mac_vap, hmac_chr_del_ba_info_stru *pst_del_ba_reason)
{
    hmac_chr_del_ba_info_stru *pst_del_ba_info = OAL_PTR_NULL;

    pst_del_ba_info = hmac_chr_del_ba_info_get_pointer();

    pst_del_ba_reason->uc_ba_num = 0;
    pst_del_ba_reason->uc_del_ba_tid = pst_del_ba_info->uc_del_ba_tid;
    pst_del_ba_reason->en_del_ba_reason = pst_del_ba_info->en_del_ba_reason;

    return;
}
oal_void hmac_chr_set_connect_code(oal_uint16 connect_code)
{
    oal_uint16 *pus_connect_code = OAL_PTR_NULL;

    pus_connect_code = hmac_chr_connect_code_get_pointer();
    *pus_connect_code = connect_code;
    return;
}

oal_void hmac_chr_get_connect_code(oal_uint16 *pus_connect_code)
{
    pus_connect_code = hmac_chr_connect_code_get_pointer();
    return;
}


OAL_STATIC oal_void hmac_chr_wifi_ext_info_completion(hmac_vap_stru         *pst_hmac_vap,
                                                      chr_wifi_ext_info_stru *pst_wifi_ext_info)
{
    oal_uint8 uc_idx;
    chr_wifi_ext_info_query_stru st_chr_wifi_ext_info_query = pst_hmac_vap->st_chr_wifi_ext_info_query;

    pst_wifi_ext_info->ul_tbtt_cnt = st_chr_wifi_ext_info_query.st_chr_wifi_ext_info.ul_tbtt_cnt;
    pst_wifi_ext_info->ul_rx_beacon_cnt = st_chr_wifi_ext_info_query.st_chr_wifi_ext_info.ul_rx_beacon_cnt;

    pst_wifi_ext_info->en_distance_id = st_chr_wifi_ext_info_query.st_chr_wifi_ext_info.en_distance_id;
    pst_wifi_ext_info->ul_upc1_01_data = st_chr_wifi_ext_info_query.st_chr_wifi_ext_info.ul_upc1_01_data;

    pst_wifi_ext_info->en_tas_state = board_get_wifi_tas_gpio_state();

    for (uc_idx = 0; uc_idx < WLAN_TID_MAX_NUM; uc_idx++) {
        pst_wifi_ext_info->auc_is_paused[uc_idx] =
        st_chr_wifi_ext_info_query.st_chr_wifi_ext_info.auc_is_paused[uc_idx];
    }
}


OAL_STATIC oal_void hmac_chr_get_wifi_ext_info_from_device(mac_vap_stru *pst_mac_vap,
                                                           chr_wifi_ext_info_stru *pst_wifi_ext_info,
                                                           oal_uint32 chr_event_id)
{
    mac_user_stru   *pst_mac_user = OAL_PTR_NULL;
    hmac_vap_stru   *pst_hmac_vap = OAL_PTR_NULL;
    oal_int         i_leftime;

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        return;
    }

    /* ���û�й�����AP���·���ѯ����ֹ�յ� */
    pst_mac_user = mac_res_get_mac_user(pst_mac_vap->uc_assoc_vap_id);
    if (pst_mac_user == OAL_PTR_NULL) {
        /* ������쳣���ߵ��µ�û��user����ͨ��get station�ӿڻ�ȡbeacon��tbtt���� */
        if (chr_event_id == CHR_WIFI_DISCONNECT_QUERY_EVENTID) {
            pst_wifi_ext_info->ul_rx_beacon_cnt = pst_hmac_vap->st_station_info_extend.ul_bcn_cnt;
            pst_wifi_ext_info->ul_tbtt_cnt = pst_hmac_vap->st_station_info_extend.ul_tbtt_cnt;
        }
        OAM_WARNING_LOG2(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
                         "{hmac_chr_get_wifi_ext_info_from_dmac::pst_user is null,record beacon [%d],tbtt_cnt[%d].}",
                         pst_wifi_ext_info->ul_rx_beacon_cnt, pst_wifi_ext_info->ul_tbtt_cnt);
        return;
    }

    pst_hmac_vap->st_chr_wifi_ext_info_query.en_chr_info_ext_query_completed_flag = OAL_FALSE;

    hmac_config_query_chr_info_ext(pst_mac_vap, 0, 0);

    i_leftime = OAL_WAIT_EVENT_INTERRUPTIBLE_TIMEOUT(
                    pst_hmac_vap->st_chr_wifi_ext_info_query.st_query_chr_wait_q,
                    (pst_hmac_vap->st_chr_wifi_ext_info_query.en_chr_info_ext_query_completed_flag == OAL_TRUE),
                    CHR_QUERY_WIFI_INFO_TIME);
    if (i_leftime == 0) {
        /* ��ʱ��û���ϱ�ɨ����� */
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{hmac_chr_get_wifi_ext_info_from_dmac::query info wait for %ld ms timeout!}",
                         CHR_QUERY_WIFI_INFO_TIME);
        return;
    } else if (i_leftime < 0) {
        /* ��ʱ���ڲ����� */
        OAM_WARNING_LOG1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{hmac_chr_get_wifi_ext_info_from_dmac::query info wait for %ld ms error!}",
                         CHR_QUERY_WIFI_INFO_TIME);
        return;
    } else {
        hmac_chr_wifi_ext_info_completion(pst_hmac_vap, pst_wifi_ext_info);
        return;
    }
}

oal_void hmac_chr_get_vap_info(mac_vap_stru *pst_mac_vap, hmac_chr_vap_info_stru *pst_chr_vap_info)
{
    mac_user_stru     *pst_mac_user;
    mac_device_stru   *pst_mac_device;
    hmac_vap_stru     *pst_hmac_vap = OAL_PTR_NULL;

    pst_mac_device = mac_res_get_dev(0);

    pst_chr_vap_info->uc_vap_state = pst_mac_vap->en_vap_state;
    pst_chr_vap_info->uc_vap_num = pst_mac_device->uc_vap_num;
    pst_chr_vap_info->uc_vap_rx_nss = pst_mac_vap->en_vap_rx_nss;
    pst_chr_vap_info->uc_protocol = pst_mac_vap->en_protocol;

    /* sta ������AP������ */
    pst_mac_user = mac_res_get_mac_user(pst_mac_vap->uc_assoc_vap_id);
    if (pst_mac_user != OAL_PTR_NULL) {
        pst_chr_vap_info->uc_ap_spatial_stream_num = pst_mac_user->uc_num_spatial_stream;
        pst_chr_vap_info->bit_ap_11ntxbf = pst_mac_user->st_cap_info.bit_11ntxbf;
        pst_chr_vap_info->bit_ap_qos = pst_mac_user->st_cap_info.bit_qos;
        pst_chr_vap_info->uc_ap_protocol_mode = pst_mac_user->en_protocol_mode;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);

    pst_chr_vap_info->bit_sta_11ntxbf = pst_mac_vap->st_cap_flag.bit_11ntxbf;
    pst_chr_vap_info->bit_is_dbac_running = mac_is_dbac_running(pst_mac_device);

    if (pst_hmac_vap != OAL_PTR_NULL) {
        pst_chr_vap_info->bit_ampdu_active    = pst_hmac_vap->en_ampdu_tx_on_switch;
        pst_chr_vap_info->bit_amsdu_active    = pst_hmac_vap->en_amsdu_ampdu_active;
    }
    pst_chr_vap_info->bit_is_dbdc_running = 0;
    pst_chr_vap_info->bit_ap_1024qam_cap = 0;
    return;
}

oal_uint32 hmac_chr_get_chip_info(oal_uint32 chr_event_id)
{
    oal_uint8                uc_vap_index;
    mac_vap_stru             *pst_mac_vap = OAL_PTR_NULL;
    mac_device_stru          *pst_mac_device = OAL_PTR_NULL;
    hmac_chr_info             st_hmac_chr_info;
    chr_wifi_ext_info_stru    st_wifi_ext_info = {0};

    pst_mac_device = mac_res_get_dev(0);
    if (pst_mac_device == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ACS, "{hmac_chr_get_chip_info::pst_mac_device null.}");
        return OAL_ERR_CODE_MAC_DEVICE_NULL;
    }

    if (pst_mac_device->uc_vap_num == 0) {
        return OAL_FAIL;
    }

    for (uc_vap_index = 0; uc_vap_index < pst_mac_device->uc_vap_num; uc_vap_index++) {
        pst_mac_vap = (mac_vap_stru *)mac_res_get_mac_vap(pst_mac_device->auc_vap_id[uc_vap_index]);
        if (pst_mac_vap == OAL_PTR_NULL) {
            continue;
        }

        if (IS_LEGACY_STA(pst_mac_vap)) {
            memset_s(&st_hmac_chr_info, OAL_SIZEOF(hmac_chr_info), 0, OAL_SIZEOF(hmac_chr_info));
            hmac_chr_get_disasoc_reason(&st_hmac_chr_info.st_disasoc_reason);
            hmac_chr_get_del_ba_info(pst_mac_vap, &st_hmac_chr_info.st_del_ba_info);
            hmac_chr_get_connect_code(&st_hmac_chr_info.us_connect_code);
            hmac_chr_get_vap_info(pst_mac_vap, &st_hmac_chr_info.st_vap_info);
            CHR_EXCEPTION_P(chr_event_id, (oal_uint8 *)(&st_hmac_chr_info), OAL_SIZEOF(hmac_chr_info));

            /* Q�汾chr��������,ͨ��device��ȡchr������Ϣ */
            hmac_chr_get_wifi_ext_info_from_device(pst_mac_vap, &st_wifi_ext_info, chr_event_id);
            CHR_EXCEPTION_Q(chr_event_id, CHR_REPORT_EXT_WIFI_INFO,
                            (oal_uint8 *)(&st_wifi_ext_info), OAL_SIZEOF(st_wifi_ext_info));
        }
    }

    /* ���ȫ�ֱ�������ʷֵ */
    hmac_chr_info_clean();

    return OAL_SUCC;
}

oal_uint32 hmac_get_chr_info_event_hander(oal_uint32 chr_event_id)
{
    oal_uint32 ul_ret = 0;

    switch (chr_event_id) {
        case CHR_WIFI_DISCONNECT_QUERY_EVENTID:
        case CHR_WIFI_CONNECT_FAIL_QUERY_EVENTID:
        case CHR_WIFI_WEB_FAIL_QUERY_EVENTID:
        case CHR_WIFI_WEB_SLOW_QUERY_EVENTID:
            ul_ret = hmac_chr_get_chip_info(chr_event_id);
            if (ul_ret != OAL_SUCC) {
                OAM_WARNING_LOG0(0, OAM_SF_ANY, "{hmac_chr_get_chip_info::hmac_chr_get_web_fail_slow_info fail.}");
                return ul_ret;
            }
            break;
        default:
            break;
    }

    return OAL_SUCC;
}
#ifdef _PRE_WLAN_FEATURE_BTCOEX

oal_void hmac_get_bt_info(hmac_device_stru *pst_hmac_device, hmac_get_wifi_info_ext_stru *pst_get_wifi_info_ext)
{
        hal_btcoex_btble_status_stru *pst_btble_status;
        oal_int32 l_ret;

        /* ��ȡ����btble��״̬ */
        pst_btble_status = &pst_hmac_device->st_hmac_device_btcoex.st_btble_status;
        l_ret = memcpy_s(&pst_get_wifi_info_ext->st_bt_status, OAL_SIZEOF(chr_bt_status_stru),
            &pst_btble_status->un_bt_status.st_bt_status, OAL_SIZEOF(chr_bt_status_stru));
        l_ret += memcpy_s(&pst_get_wifi_info_ext->st_ble_status, OAL_SIZEOF(chr_ble_status_stru),
            &pst_btble_status->un_ble_status.st_ble_status, OAL_SIZEOF(chr_ble_status_stru));
        if (l_ret != EOK) {
            OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hmac_get_wifi_info_ext::memcpy fail.}");
        }
}
#endif


oal_uint32 hmac_get_wifi_info_ext(hmac_get_wifi_info_ext_stru *pst_get_wifi_info_ext)
{
    oal_uint8                       uc_vap_index;
    mac_vap_stru                    *pst_mac_vap = OAL_PTR_NULL;
    mac_device_stru                 *pst_mac_device = OAL_PTR_NULL;
    hmac_vap_stru                   *pst_hmac_vap = OAL_PTR_NULL;
    oal_cali_param_stru             *pst_cali_data = OAL_PTR_NULL;
    hmac_device_stru               *pst_hmac_device = OAL_PTR_NULL;

    if (pst_get_wifi_info_ext == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{hmac_get_wifi_info_ext::pst_get_wifi_info_ext null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_mac_device = mac_res_get_dev(0);
    if (pst_mac_device == OAL_PTR_NULL) {
        return OAL_ERR_CODE_MAC_DEVICE_NULL;
    }

    pst_hmac_device = hmac_res_get_mac_dev(pst_mac_device->uc_device_id);
    if (pst_hmac_device == OAL_PTR_NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    for (uc_vap_index = 0; uc_vap_index < pst_mac_device->uc_vap_num; uc_vap_index++) {
        pst_mac_vap = (mac_vap_stru *)mac_res_get_mac_vap(pst_mac_device->auc_vap_id[uc_vap_index]);
        if (pst_mac_vap == OAL_PTR_NULL) {
            continue;
        }

        if (!IS_LEGACY_STA(pst_mac_vap)) {
            continue;
        }

        pst_hmac_vap = mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
        if (pst_hmac_vap == OAL_PTR_NULL) {
            continue;
        }

#ifdef _PRE_WLAN_DFT_STAT
        /* ��ȡdevice���ϱ��Ŀ����Ų��� */
        pst_get_wifi_info_ext->uc_device_distance = pst_hmac_vap->uc_device_distance;
        pst_get_wifi_info_ext->uc_intf_state_cca = pst_hmac_vap->uc_intf_state_cca;
        pst_get_wifi_info_ext->uc_intf_state_co  = pst_hmac_vap->uc_intf_state_co;
#endif
#ifdef _PRE_WLAN_FEATURE_BTCOEX
        hmac_get_bt_info(pst_hmac_device, pst_get_wifi_info_ext);
#endif
        /* ��ȡоƬ���� SS FF TT */
        pst_cali_data = (oal_cali_param_stru *)get_cali_data_buf_addr();
        if (pst_cali_data != OAL_PTR_NULL) {
            pst_get_wifi_info_ext->uc_chip_type = pst_cali_data->st_bfgn_cali_data.uc_chip_type_code;
        }
    }
    return OAL_SUCC;
}

oal_void hmac_chr_connect_fail_query_and_report(hmac_vap_stru *pst_hmac_vap,
                                                mac_status_code_enum_uint16 connet_code)
{
    mac_chr_connect_fail_report_stru st_chr_connect_fail_report = { 0 };

    if (IS_LEGACY_STA(&pst_hmac_vap->st_vap_base_info)) {
        /* ������ѯ */
        hmac_chr_set_connect_code(connet_code);
        /* �����ϱ� */
#ifdef CONFIG_HW_GET_EXT_SIG
        st_chr_connect_fail_report.ul_noise = pst_hmac_vap->station_info.noise;
        st_chr_connect_fail_report.ul_chload = pst_hmac_vap->station_info.chload;
#endif
        st_chr_connect_fail_report.c_signal = pst_hmac_vap->station_info.signal;
        st_chr_connect_fail_report.uc_distance = pst_hmac_vap->st_station_info_extend.uc_distance;
        st_chr_connect_fail_report.uc_cca_intr = pst_hmac_vap->st_station_info_extend.uc_cca_intr;

        st_chr_connect_fail_report.us_err_code = connet_code;
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "{hmac_chr_get_chip_info::hmac_chr_get_web_fail_slow_info fail.}");
        CHR_EXCEPTION_P(CHR_WIFI_CONNECT_FAIL_REPORT_EVENTID, (oal_uint8 *)(&st_chr_connect_fail_report),
                        OAL_SIZEOF(mac_chr_connect_fail_report_stru));
    }
    return;
}

oal_bool_enum_uint8 hmac_chr_is_reach_roam_condition(hmac_vap_stru *pst_hmac_vap)
{
    oal_uint32 ul_loop;
    oal_uint32 ul_rx_rate;
    mac_user_stru *pst_mac_user;
    mac_vap_stru  *pst_mac_vap;
    oal_uint32 ul_cur_timestamp;
    oal_uint32 ul_delt_assoc_time;
    oal_uint32 ul_delt_roam_time;

    pst_mac_vap = &pst_hmac_vap->st_vap_base_info;
    pst_mac_user = (mac_user_stru *)mac_res_get_mac_user(pst_mac_vap->uc_assoc_vap_id);
    if (pst_mac_user == OAL_PTR_NULL) {
        OAM_WARNING_LOG0(0, OAM_SF_ANY, "hmac_chr_is_reach_roam_condition:pst_mac_user is null");
        return OAL_FALSE;
    }
    if ((pst_mac_user->en_avail_bandwidth == WLAN_BW_CAP_20M) ||
        (pst_hmac_vap->station_info.signal > CHR_WEB_FAIL_ROAM_RSSI_TH) ||
        (pst_mac_vap->st_channel.en_band != WLAN_BAND_2G) ||
        (pst_mac_vap->en_vap_state == MAC_VAP_STATE_ROAMING)) {
        return OAL_FALSE;
    }
    ul_cur_timestamp = OAL_TIME_GET_STAMP_MS();
    /* ����չ���û��þʹ����� */
    ul_delt_assoc_time = (oal_uint32)OAL_TIME_GET_RUNTIME(pst_hmac_vap->ul_assoc_timestamp, ul_cur_timestamp);
    ul_delt_roam_time = (oal_uint32)OAL_TIME_GET_RUNTIME(pst_hmac_vap->ul_web_fail_roam_timestamp, ul_cur_timestamp);
    if ((ul_delt_assoc_time < CHR_WEB_FAIL_ROAM_PROTECT_PERIOD) ||
        (ul_delt_roam_time < CHR_WEB_FAIL_ROAM_MIN_INTERVAL)) {
        return OAL_FALSE;
    }
    /* ÿ3s����һ�μ�¼,ʱ��̫��Ĳ��ܷ�ӳ��ǰ״̬��˴˴�����ͳ�ƹ����¼,
       �����������ģ����Ҫ��ͳ�Ƹ������Ӵ˴��߼����޸Ĵ����µļ�¼��ʼͳ����Ҫ��ʱ��� */
    for (ul_loop = 0; ul_loop < HMAC_STA_RSSI_LOG_NUM; ul_loop++) {
        ul_rx_rate = pst_hmac_vap->st_history_rx_rate.aul_rate[ul_loop];
        if ((ul_rx_rate > CHR_WEB_FAIL_ROAM_RX_RATE_TH) ||
            (ul_rx_rate == 0)) { /* 0Ϊ��Чֵ */
            return OAL_FALSE;
        }
    }
    return OAL_TRUE;
}
oal_void hmac_chr_trigger_roam(mac_vap_stru  *pst_mac_vap)
{
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        OAM_WARNING_LOG1(0, OAM_SF_ANY, "hmac_chr_trigger_roam:pst_hmac_vap is null,vap_id=%d", pst_mac_vap->uc_vap_id);
        return;
    }
    if (hmac_chr_is_reach_roam_condition(pst_hmac_vap) == OAL_FALSE) {
        return;
    }
    pst_hmac_vap->ul_web_fail_roam_timestamp = OAL_TIME_GET_STAMP_MS();;
    memcpy_s(pst_hmac_vap->st_20m_ap_info.auc_bssid, OAL_MAC_ADDR_LEN, pst_mac_vap->auc_bssid, OAL_MAC_ADDR_LEN);
    pst_hmac_vap->en_web_fail_roam = OAL_TRUE;
    pst_hmac_vap->st_20m_ap_info.c_rssi = CHR_WEB_FAIL_ROAM_RSSI_TH;
    OAM_WARNING_LOG1(0, OAM_SF_ANY, "hmac_chr_trigger_roam:cur_timestamp=%d", pst_hmac_vap->ul_web_fail_roam_timestamp);
    hmac_roam_start_reconnect(pst_hmac_vap, pst_mac_vap->auc_bssid);
}
oal_void hmac_chr_process_web_fail(oal_uint32 ul_chr_event_id, oal_uint32 ul_rx_rate)
{
    mac_device_stru *pst_mac_device = OAL_PTR_NULL;
    mac_vap_stru    *pst_mac_vap = OAL_PTR_NULL;

    if ((ul_chr_event_id != CHR_WIFI_WEB_FAIL_QUERY_EVENTID) &&
        (ul_chr_event_id != CHR_WIFI_WEB_SLOW_QUERY_EVENTID)) {
        return;
    }
    if (ul_rx_rate > CHR_WEB_FAIL_ROAM_RX_RATE_TH) {
        return;
    }
    pst_mac_device = mac_res_get_dev(0);
    if (pst_mac_device == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ACS, "{hmac_chr_process_error_id::pst_mac_device null.}");
        return;
    }
    if (mac_device_calc_up_vap_num(pst_mac_device) != 1) { /* 1��ʾֻ��һ��VAP */
        return;
    }
    mac_device_find_legacy_sta(pst_mac_device, &pst_mac_vap);
    if (pst_mac_vap == OAL_PTR_NULL) {
        return;
    }
    hmac_chr_trigger_roam(pst_mac_vap);
}
#endif

oal_uint32 oam_cfg_init(oal_void)
{
    oal_int32 l_ret;

    l_ret = oam_cfg_get_all_item();

    return (oal_uint32)l_ret;
}

oal_uint32 hmac_dfx_init(void)
{
    oam_register_init_hook(OM_WIFI, oam_cfg_init);
#ifdef _PRE_WLAN_1102A_CHR
    chr_host_callback_register(hmac_get_chr_info_event_hander);
    chr_get_wifi_ext_info_callback_register(hmac_get_wifi_info_ext);
    chr_process_web_fail_callback_register(hmac_chr_process_web_fail);
#endif
    hmac_custom_init(OAL_FALSE);
    return OAL_SUCC;
}

oal_uint32 hmac_dfx_exit(void)
{
#ifdef _PRE_WLAN_1102A_CHR
    chr_host_callback_unregister();
    chr_get_wifi_ext_info_callback_unregister();
    chr_process_web_fail_callback_unregister();
#endif
    return OAL_SUCC;
}

