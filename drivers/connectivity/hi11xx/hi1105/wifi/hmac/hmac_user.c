

/* 1 头文件包含 */
#include "oam_ext_if.h"

#include "wlan_chip_i.h"

#include "hmac_user.h"
#include "hmac_main.h"
#include "hmac_tx_amsdu.h"
#include "hmac_protection.h"
#include "hmac_smps.h"
#include "hmac_ext_if.h"
#include "hmac_config.h"
#include "hmac_mgmt_ap.h"
#include "hmac_chan_mgmt.h"
#include "hmac_fsm.h"
#include "hmac_sme_sta.h"

#ifdef _PRE_WLAN_FEATURE_WAPI
#include "hmac_wapi.h"
#endif

#include "hmac_roam_main.h"

#include "hmac_blockack.h"
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
#include "hisi_customize_wifi.h"
#endif /* #ifdef _PRE_PLAT_FEATURE_CUSTOMIZE */

#include "hmac_scan.h"
#include "hmac_dbac.h"
#include "securec.h"
#include "securectype.h"
#ifdef _PRE_WLAN_FEATURE_11AX
#include "mac_ie.h"
#endif
#ifdef _PRE_WLAN_FEATURE_HIEX
#include "hmac_hiex.h"
#endif
#include "hmac_ht_self_cure.h"
#include "hmac_tx_msdu_dscr.h"
#include "hmac_host_tx_data.h"
#include "hal_device.h"
#include "hmac_vowifi.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_USER_C
/* 2 全局变量定义 */
#define HMAC_USER_TID_TX_RING_SIZE 6

extern uint8_t g_wlan_ps_mode;

hmac_lut_index_tbl_stru g_hmac_lut_index_tbl[HAL_MAX_TX_BA_LUT_SIZE];

/* 3 函数实现 */

hmac_user_stru *mac_res_get_hmac_user_alloc(uint16_t us_idx)
{
    hmac_user_stru *pst_hmac_user;

    pst_hmac_user = (hmac_user_stru *)_mac_res_get_hmac_user(us_idx);
    if (pst_hmac_user == OAL_PTR_NULL) {
        oam_error_log1(0, OAM_SF_UM, "{mac_res_get_hmac_user_init::pst_hmac_user null,user_idx=%d.}", us_idx);
        return OAL_PTR_NULL;
    }

    /* 重复申请异常,避免影响业务，暂时打印error但正常申请 */
    if (pst_hmac_user->st_user_base_info.uc_is_user_alloced == MAC_USER_ALLOCED) {
        oam_error_log1(0, OAM_SF_UM, "{mac_res_get_hmac_user_init::[E]user has been alloced,user_idx=%d.}", us_idx);
    }

    return pst_hmac_user;
}


hmac_user_stru *mac_res_get_hmac_user(uint16_t us_idx)
{
    hmac_user_stru *pst_hmac_user;

    pst_hmac_user = (hmac_user_stru *)_mac_res_get_hmac_user(us_idx);
    if (pst_hmac_user == OAL_PTR_NULL) {
        return OAL_PTR_NULL;
    }

    /* 异常: 用户资源已被释放 */
    if (pst_hmac_user->st_user_base_info.uc_is_user_alloced != MAC_USER_ALLOCED) {
        oal_mem_print_funcname(OAL_RET_ADDR);
        /*
         * host侧获取用户时用户已经释放属于正常，返回空指针，
         * 后续调用者查找用户失败，请打印WARNING并直接释放buf，
         * 走其他分支等等
         */
        oam_warning_log1(0, OAM_SF_UM, "{mac_res_get_hmac_user::[E]user has been freed,user_idx=%d.}", us_idx);
        return OAL_PTR_NULL;
    }

    return pst_hmac_user;
}


uint32_t hmac_user_alloc(uint16_t *pus_user_idx)
{
    hmac_user_stru *pst_hmac_user = OAL_PTR_NULL;
    uint32_t ul_rslt;
    uint16_t us_user_idx_temp;

    if (oal_unlikely(pus_user_idx == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_UM, "{hmac_user_alloc::pus_user_idx null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 申请hmac user内存 */
    /*lint -e413*/
    ul_rslt = mac_res_alloc_hmac_user(&us_user_idx_temp, OAL_OFFSET_OF(hmac_user_stru, st_user_base_info));
    if (ul_rslt != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_UM, "{hmac_user_alloc::mac_res_alloc_hmac_user failed[%d].}", ul_rslt);
        return ul_rslt;
    }
    /*lint +e413*/
    pst_hmac_user = mac_res_get_hmac_user_alloc(us_user_idx_temp);
    if (pst_hmac_user == OAL_PTR_NULL) {
        mac_res_free_mac_user(us_user_idx_temp);
        oam_error_log1(0, OAM_SF_UM, "{hmac_user_alloc::pst_hmac_user null,user_idx=%d.}", us_user_idx_temp);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 初始清0 */
    memset_s(pst_hmac_user, OAL_SIZEOF(hmac_user_stru), 0, OAL_SIZEOF(hmac_user_stru));
    /* 标记user资源已被alloc */
    pst_hmac_user->st_user_base_info.uc_is_user_alloced = MAC_USER_ALLOCED;

    *pus_user_idx = us_user_idx_temp;

    return OAL_SUCC;
}


uint32_t hmac_user_free(uint16_t us_idx)
{
    hmac_user_stru *pst_hmac_user;
    uint32_t ul_ret;

    pst_hmac_user = mac_res_get_hmac_user(us_idx);
    if (pst_hmac_user == OAL_PTR_NULL) {
        oam_error_log1(0, OAM_SF_UM, "{hmac_user_free::pst_hmac_user null,user_idx=%d.}", us_idx);
        return OAL_ERR_CODE_PTR_NULL;
    }

    ul_ret = mac_res_free_mac_user(us_idx);
    if (ul_ret == OAL_SUCC) {
        /* 清除alloc标志 */
        pst_hmac_user->st_user_base_info.uc_is_user_alloced = MAC_USER_FREED;
    }

    return ul_ret;
}


uint32_t hmac_user_free_asoc_req_ie(uint16_t us_idx)
{
    hmac_user_stru *pst_hmac_user;

    pst_hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(us_idx);
    if (pst_hmac_user == OAL_PTR_NULL) {
        oam_warning_log1(0, OAM_SF_ASSOC, "{hmac_vap_free_asoc_req_ie::pst_hmac_user[%d] null.}", us_idx);
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (pst_hmac_user->puc_assoc_req_ie_buff != OAL_PTR_NULL) {
        oal_mem_free_m(pst_hmac_user->puc_assoc_req_ie_buff, OAL_TRUE);
        pst_hmac_user->puc_assoc_req_ie_buff = OAL_PTR_NULL;
        pst_hmac_user->ul_assoc_req_ie_len = 0;
    } else {
        pst_hmac_user->ul_assoc_req_ie_len = 0;
    }
    return OAL_SUCC;
}


uint32_t hmac_user_set_asoc_req_ie(hmac_user_stru *pst_hmac_user,
                                   uint8_t *puc_payload, uint32_t ul_payload_len, uint8_t uc_reass_flag)
{
    /* 重关联比关联请求帧头多了AP的MAC地址  */
    ul_payload_len -= ((uc_reass_flag == OAL_TRUE) ? WLAN_MAC_ADDR_LEN : 0);

    pst_hmac_user->puc_assoc_req_ie_buff = oal_mem_alloc_m(OAL_MEM_POOL_ID_LOCAL, (uint16_t)ul_payload_len, OAL_TRUE);
    if (pst_hmac_user->puc_assoc_req_ie_buff == OAL_PTR_NULL) {
        oam_error_log1(pst_hmac_user->st_user_base_info.uc_vap_id, OAM_SF_ASSOC,
                       "{hmac_user_set_asoc_req_ie :: Alloc %u bytes failed for puc_assoc_req_ie_buff failed .}",
                       (uint16_t)ul_payload_len);
        pst_hmac_user->ul_assoc_req_ie_len = 0;
        return OAL_FAIL;
    }
    if (memcpy_s(pst_hmac_user->puc_assoc_req_ie_buff, ul_payload_len,
                 puc_payload + ((uc_reass_flag == OAL_TRUE) ? WLAN_MAC_ADDR_LEN : 0),
                 ul_payload_len) != EOK) {
        oal_mem_free_m(pst_hmac_user->puc_assoc_req_ie_buff, OAL_TRUE);
        oam_error_log0(0, OAM_SF_ASSOC, "hmac_user_set_asoc_req_ie::memcpy fail!");
        return OAL_FAIL;
    }
    pst_hmac_user->ul_assoc_req_ie_len = ul_payload_len;

    return OAL_SUCC;
}


OAL_STATIC void hmac_user_tid_init(hmac_user_stru *hmac_user)
{
    uint32_t tid;
    hmac_ba_tx_stru *tx_ba = NULL;

    for (tid = 0; tid < WLAN_TID_MAX_NUM; tid++) {
        hmac_user->ast_tid_info[tid].uc_tid_no = (uint8_t)tid;

        hmac_user->ast_tid_info[tid].us_hmac_user_idx = hmac_user->st_user_base_info.us_assoc_id;

        /* 初始化ba rx操作句柄 */
        hmac_user->ast_tid_info[tid].pst_ba_rx_info = NULL;
        oal_spin_lock_init(&(hmac_user->ast_tid_info[tid].st_ba_tx_info.st_ba_status_lock));
        hmac_user->ast_tid_info[tid].st_ba_tx_info.en_ba_status = DMAC_BA_INIT;
        hmac_user->ast_tid_info[tid].st_ba_tx_info.uc_addba_attemps = 0;
        hmac_user->ast_tid_info[tid].st_ba_tx_info.uc_dialog_token = 0;
        hmac_user->ast_tid_info[tid].st_ba_tx_info.uc_ba_policy = 0;
        hmac_user->ast_tid_info[tid].en_ba_handle_tx_enable = OAL_TRUE;
        hmac_user->ast_tid_info[tid].en_ba_handle_rx_enable = OAL_TRUE;

        hmac_user->auc_ba_flag[tid] = 0;

        /* addba req超时处理函数入参填写 */
        tx_ba = &hmac_user->ast_tid_info[tid].st_ba_tx_info;
        tx_ba->st_alarm_data.pst_ba = (void *)tx_ba;
        tx_ba->st_alarm_data.uc_tid = tid;
        tx_ba->st_alarm_data.us_mac_user_idx = hmac_user->st_user_base_info.us_assoc_id;
        tx_ba->st_alarm_data.uc_vap_id = hmac_user->st_user_base_info.uc_vap_id;

        /* 初始化用户关联请求帧参数 */
        hmac_user->puc_assoc_req_ie_buff = NULL;
        hmac_user->ul_assoc_req_ie_len = 0;

        /* 初始化上一帧的seq num, 防止当前帧seq num为0时被误过滤 */
        hmac_user->qos_last_seq_frag_num[tid] = 0xffff;

#ifdef CONFIG_ARCH_KIRIN_PCIE
        hmac_tx_ring_init(hmac_user, tid, HMAC_USER_TID_TX_RING_SIZE);
#endif

        oal_spin_lock_init(&(hmac_user->ast_tid_info[tid].st_ba_timer_lock));
    }
}

#ifdef CONFIG_ARCH_KIRIN_PCIE
#define HMAC_UPDATE_FREQ_TIMER_PERIOD 1000  /* 1s更新一次调度事件下发频率 */


void hmac_init_tx_update_freq_timer(hmac_user_stru *hmac_user)
{
    if (hmac_get_tx_switch() != HOST_TX) {
        oam_warning_log0(0, 0, "{hmac_init_tx_update_freq_mgmt::return due to tx_switch != HOST_TX}");
        return;
    }

    if (hmac_user->tx_update_freq_adjust_timer.en_is_registerd) {
        oam_warning_log0(0, OAM_SF_CFG, "{hmac_init_tx_update_freq_adjust_timer::timer already init}");
        return;
    }

    frw_timer_create_timer_m(&hmac_user->tx_update_freq_adjust_timer, hmac_tx_update_freq_timeout,
                             HMAC_UPDATE_FREQ_TIMER_PERIOD, hmac_user, OAL_TRUE, OAM_MODULE_ID_HMAC, 0);
}


void hmac_deinit_tx_update_freq_timer(hmac_user_stru *hmac_user)
{
    if (hmac_get_tx_switch() != HOST_TX) {
        oam_warning_log0(0, 0, "{hmac_deinit_tx_update_freq_mgmt::return due to tx_switch != HOST_TX}");
        return;
    }

    if (hmac_user->tx_update_freq_adjust_timer.en_is_registerd) {
        frw_timer_immediate_destroy_timer_m(&hmac_user->tx_update_freq_adjust_timer);
    }
}
#endif


uint32_t hmac_user_init(hmac_user_stru *pst_hmac_user)
{
#ifdef _PRE_WLAN_FEATURE_EDCA_OPT_AP
    uint8_t uc_ac_idx;
    uint8_t uc_data_idx;
#endif

    /* 初始化tid信息 */
    hmac_user_tid_init(pst_hmac_user);

#ifdef _PRE_WLAN_FEATURE_EDCA_OPT_AP
    for (uc_ac_idx = 0; uc_ac_idx < WLAN_WME_AC_BUTT; uc_ac_idx++) {
        for (uc_data_idx = 0; uc_data_idx < WLAN_TXRX_DATA_BUTT; uc_data_idx++) {
            pst_hmac_user->aaul_txrx_data_stat[uc_ac_idx][uc_data_idx] = 0;
        }
    }
#endif

    pst_hmac_user->pst_defrag_netbuf = OAL_PTR_NULL;
    pst_hmac_user->user_hisi = OAL_FALSE;
    pst_hmac_user->en_user_bw_limit = OAL_FALSE;
#if (_PRE_WLAN_FEATURE_PMF != _PRE_PMF_NOT_SUPPORT)
    pst_hmac_user->st_sa_query_info.ul_sa_query_count = 0;
    pst_hmac_user->st_sa_query_info.ul_sa_query_start_time = 0;
#endif
    memset_s(&pst_hmac_user->st_defrag_timer, OAL_SIZEOF(frw_timeout_stru), 0, OAL_SIZEOF(frw_timeout_stru));
    pst_hmac_user->ul_rx_pkt_drop = 0;

    /* 清除usr统计信息 */
    oam_stats_clear_user_stat_info(pst_hmac_user->st_user_base_info.us_assoc_id);

    pst_hmac_user->ul_first_add_time = (uint32_t)oal_time_get_stamp_ms();

    pst_hmac_user->us_clear_judge_count = 0;
    pst_hmac_user->assoc_err_code = 0; /* 去关联 error code */

#ifdef CONFIG_ARCH_KIRIN_PCIE
    hmac_init_tx_update_freq_timer(pst_hmac_user);
#endif

    return OAL_SUCC;
}

OAL_STATIC uint32_t hmac_user_set__num_spatial_stream_vht_capable(mac_user_stru *pst_mac_user,
    mac_vht_hdl_stru *pst_mac_vht_hdl, wlan_nss_enum_uint8 *pen_user_num_spatial_stream)
{
    if (pst_mac_vht_hdl->st_rx_max_mcs_map.us_max_mcs_4ss != 3) {
        *pen_user_num_spatial_stream = WLAN_FOUR_NSS;
    } else if (pst_mac_vht_hdl->st_rx_max_mcs_map.us_max_mcs_3ss != 3) {
        *pen_user_num_spatial_stream = WLAN_TRIPLE_NSS;
    } else if (pst_mac_vht_hdl->st_rx_max_mcs_map.us_max_mcs_2ss != 3) {
        *pen_user_num_spatial_stream = WLAN_DOUBLE_NSS;
    } else if (pst_mac_vht_hdl->st_rx_max_mcs_map.us_max_mcs_1ss != 3) {
        *pen_user_num_spatial_stream = WLAN_SINGLE_NSS;
    } else {
        oam_warning_log0(pst_mac_user->uc_vap_id, OAM_SF_ANY,
                         "{hmac_user_set_avail_num_space_stream::invalid vht nss.}");

        return OAL_FAIL;
    }

    return OAL_SUCC;
}


uint32_t hmac_user_set_avail_num_space_stream(mac_user_stru *pst_mac_user, wlan_nss_enum_uint8 en_vap_nss)
{
    mac_user_ht_hdl_stru *pst_mac_ht_hdl;
    mac_vht_hdl_stru *pst_mac_vht_hdl;
#ifdef _PRE_WLAN_FEATURE_11AX
    mac_he_hdl_stru *pst_mac_he_hdl = MAC_USER_HE_HDL_STRU(pst_mac_user);
#endif
    wlan_nss_enum_uint8 en_user_num_spatial_stream = 0;
    uint32_t ul_ret = OAL_SUCC;

    /* AP(STA)为legacy设备，只支持1根天线，不需要再判断天线个数 */
    /* 获取HT和VHT结构体指针 */
    pst_mac_ht_hdl = &(pst_mac_user->st_ht_hdl);
    pst_mac_vht_hdl = &(pst_mac_user->st_vht_hdl);
#ifdef _PRE_WLAN_FEATURE_11AX
    if (g_wlan_spec_cfg->feature_11ax_is_open && pst_mac_he_hdl->en_he_capable == OAL_TRUE) {
        ul_ret = mac_ie_from_he_cap_get_nss(pst_mac_he_hdl, &en_user_num_spatial_stream);
    } else if (pst_mac_vht_hdl->en_vht_capable == OAL_TRUE)
#else // ut 11AX宏未打开，待后续修改
    if (pst_mac_vht_hdl->en_vht_capable == OAL_TRUE)
#endif
    {
        // 降低圈复杂度
        if (hmac_user_set__num_spatial_stream_vht_capable(pst_mac_user, pst_mac_vht_hdl,
                                                          &en_user_num_spatial_stream) != OAL_SUCC) {
            ul_ret = OAL_FAIL;
        }
    } else if (pst_mac_ht_hdl->en_ht_capable == OAL_TRUE) {
        if (pst_mac_ht_hdl->uc_rx_mcs_bitmask[3] > 0) {
            en_user_num_spatial_stream = WLAN_FOUR_NSS;
        } else if (pst_mac_ht_hdl->uc_rx_mcs_bitmask[2] > 0) {
            en_user_num_spatial_stream = WLAN_TRIPLE_NSS;
        } else if (pst_mac_ht_hdl->uc_rx_mcs_bitmask[1] > 0) {
            en_user_num_spatial_stream = WLAN_DOUBLE_NSS;
        } else if (pst_mac_ht_hdl->uc_rx_mcs_bitmask[0] > 0) {
            en_user_num_spatial_stream = WLAN_SINGLE_NSS;
        } else {
            oam_warning_log0(pst_mac_user->uc_vap_id, OAM_SF_ANY,
                             "{hmac_user_set_avail_num_space_stream::invalid ht nss.}");

            ul_ret = OAL_FAIL;
        }
    } else {
        en_user_num_spatial_stream = WLAN_SINGLE_NSS;
    }

    /* 赋值给用户结构体变量 */
    mac_user_set_num_spatial_stream(pst_mac_user, en_user_num_spatial_stream);
    mac_user_set_avail_num_spatial_stream(pst_mac_user,
                                          oal_min(pst_mac_user->en_user_max_cap_nss, en_vap_nss));
    mac_user_set_smps_opmode_notify_nss(pst_mac_user, WLAN_NSS_LIMIT);

    return ul_ret;
}


uint8_t hmac_user_calculate_num_spatial_stream(wlan_nss_enum_uint8 *puc_nss1, wlan_nss_enum_uint8 *puc_nss2)
{
    uint8_t uc_nss_valid_flag = OAL_TRUE;

    switch (*puc_nss1) {
        case 0: /* 0流表示不支持 */
            *puc_nss1 = WLAN_NSS_LIMIT;
            break;

        case 1: /* 1流的宏定义WLAN_SINGLE_NSS(值是0) */
            *puc_nss1 = WLAN_SINGLE_NSS;
            break;

        case 2: /* 2流的宏定义WLAN_DOUBLE_NSS(值是1) */
            *puc_nss1 = WLAN_DOUBLE_NSS;
            break;

        default:
            oam_error_log1(0, OAM_SF_ANY, "{hmac_user_calculate_num_spatial_stream::error puc_nss1[%d]}", *puc_nss1);
            uc_nss_valid_flag = OAL_FALSE;
            break;
    }

    switch (*puc_nss2) {
        case 0: /* 0流表示不支持 */
            *puc_nss2 = WLAN_NSS_LIMIT;
            break;

        case 1: /* 1流的宏定义WLAN_SINGLE_NSS(值是0) */
            *puc_nss2 = WLAN_SINGLE_NSS;
            break;

        case 2: /* 2流的宏定义WLAN_DOUBLE_NSS(值是1) */
            *puc_nss2 = WLAN_DOUBLE_NSS;
            break;

        case 3: /* 3流的宏定义WLAN_DOUBLE_NSS(值是2) */
            *puc_nss2 = WLAN_TRIPLE_NSS;
            break;

        default:
            oam_error_log1(0, OAM_SF_ANY, "{hmac_user_calculate_num_spatial_stream::error puc_nss2[%d]}", *puc_nss2);
            uc_nss_valid_flag = OAL_FALSE;
            break;
    }
    return uc_nss_valid_flag;
}


void hmac_user_set_num_spatial_stream_160M(mac_user_stru *pst_mac_user)
{
    mac_vht_hdl_stru *pst_mac_vht_hdl = OAL_PTR_NULL;
    /* 计算结果是实际流数，取值 0 或 1 或 2 */
    wlan_nss_enum_uint8 en_user_num_spatial_stream1 = ((pst_mac_user->en_user_max_cap_nss + 1) >> 1);
    /* 计算结果是实际流数，取值 0 或 1 或 2 或 3  */
    wlan_nss_enum_uint8 en_user_num_spatial_stream2 = (((pst_mac_user->en_user_max_cap_nss + 1) * 3) >> 2);
    wlan_nss_enum_uint8 en_user_num_spatial_stream = WLAN_NSS_LIMIT;
    uint8_t uc_ret;

    uc_ret = hmac_user_calculate_num_spatial_stream(&en_user_num_spatial_stream1, &en_user_num_spatial_stream2);
    if (uc_ret == OAL_FALSE) {
        oam_error_log0(0, OAM_SF_ANY,
                       "{hmac_user_set_num_spatial_stream_160M::hmac_user_calculate_num_spatial_stream error!}");
        return;
    }

    /* 获取HT和VHT结构体指针 */
    pst_mac_vht_hdl = &(pst_mac_user->st_vht_hdl);

    if (pst_mac_vht_hdl->bit_supported_channel_width > WLAN_MIB_VHT_SUPP_WIDTH_80) {
        en_user_num_spatial_stream = pst_mac_user->en_user_max_cap_nss;
    } else {
        switch (pst_mac_vht_hdl->bit_extend_nss_bw_supp) {
            case WLAN_EXTEND_NSS_BW_SUPP0:
                en_user_num_spatial_stream = WLAN_NSS_LIMIT;
                break;

            case WLAN_EXTEND_NSS_BW_SUPP1:
            case WLAN_EXTEND_NSS_BW_SUPP2:
                en_user_num_spatial_stream = en_user_num_spatial_stream1;
                break;

            case WLAN_EXTEND_NSS_BW_SUPP3:
                en_user_num_spatial_stream = en_user_num_spatial_stream2;
                break;

            default:
                oam_warning_log1(0, OAM_SF_ANY,
                                 "{hmac_user_set_num_spatial_stream_160M::error extend_nss_bw_supp[%d]}",
                                 pst_mac_vht_hdl->bit_extend_nss_bw_supp);
                break;
        }
    }
    mac_user_set_num_spatial_stream_160M(pst_mac_user, en_user_num_spatial_stream);
}

#if (_PRE_WLAN_FEATURE_PMF != _PRE_PMF_NOT_SUPPORT)

OAL_STATIC void hmac_stop_sa_query_timer(hmac_user_stru *pst_hmac_user)
{
    frw_timeout_stru *pst_sa_query_interval_timer;

    pst_sa_query_interval_timer = &(pst_hmac_user->st_sa_query_info.st_sa_query_interval_timer);
    if (pst_sa_query_interval_timer->en_is_registerd != OAL_FALSE) {
        frw_timer_immediate_destroy_timer_m(pst_sa_query_interval_timer);
    }
}
#endif


#ifdef _PRE_WLAN_FEATURE_WAPI
hmac_wapi_stru *hmac_user_get_wapi_ptr(mac_vap_stru *pst_mac_vap,
                                       oal_bool_enum_uint8 en_pairwise,
                                       uint16_t us_pairwise_idx)
{
    hmac_user_stru *pst_hmac_user = NULL;
    uint16_t us_user_index;

    if (en_pairwise == OAL_TRUE) {
        us_user_index = us_pairwise_idx;
    } else {
        us_user_index = pst_mac_vap->us_multi_user_idx;
    }

    pst_hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(us_user_index);
    if (pst_hmac_user == OAL_PTR_NULL) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{hmac_user_get_wapi_ptr::pst_hmac_user[%d] null.}", us_user_index);
        return OAL_PTR_NULL;
    }

    return &pst_hmac_user->st_wapi;
}
#endif


void hmac_compability_ap_tpye_identify_for_btcoex(mac_vap_stru *pst_mac_vap,
                                                  mac_bss_dscr_stru *pst_bss_dscr, uint8_t *puc_mac_addr,
                                                  mac_ap_type_enum_uint16 *pen_ap_type)
{
    wlan_nss_enum_uint8 en_support_max_nss = WLAN_SINGLE_NSS;
    if (pst_bss_dscr != OAL_PTR_NULL) {
        /* 一级生效，直接置self-cts,苹果路由器有其他方案，这里暂时不开 */
        /* 二级生效，需要考虑mac地址约束 */
        if (pst_bss_dscr->en_btcoex_blacklist_chip_oui & MAC_BTCOEX_BLACKLIST_LEV0) {
            /* 关联ap识别: AP OUI + chip OUI + 双流 + 2G 需要dmac侧刷新ps机制时one pkt帧发送类型 */
            if (MAC_IS_DLINK_AP(puc_mac_addr) || MAC_IS_HAIER_AP(puc_mac_addr) ||
                MAC_IS_FAST_AP(puc_mac_addr) || MAC_IS_TPINK_845_AP(puc_mac_addr)) {
#ifdef _PRE_WLAN_FEATURE_M2S
                en_support_max_nss = pst_bss_dscr->en_support_max_nss;
#endif
                if (HMAC_AP_IS_BW_2G_DOUBLE_NSS(en_support_max_nss, pst_mac_vap)) {
                    oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_COEX,
                        "hmac_compability_ap_tpye_identify_for_btcoex: lev0 btcoex ps self-cts blacklist!");
                    *pen_ap_type |= MAC_AP_TYPE_BTCOEX_PS_BLACKLIST;
                }
            } else if (MAC_IS_JCG_AP(puc_mac_addr)) {
                /* JCG路由器为了兼容aptxHD和660,需要关闭cts回复功能 */
                oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_COEX,
                    "hmac_compability_ap_tpye_identify_for_btcoex: lev0 JCG btcoex disable cts blacklist!");
                *pen_ap_type |= MAC_AP_TYPE_BTCOEX_DISABLE_CTS;
            } else if (MAC_IS_FEIXUN_K3(puc_mac_addr)) {
                oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_COEX,
                                 "hmac_compability_ap_tpye_identify_for_btcoex: lev0 k3 btcoex ps self-cts blacklist!");
                *pen_ap_type |= MAC_AP_TYPE_BTCOEX_PS_BLACKLIST;
            } else if (MAC_IS_FAST_FW450R(puc_mac_addr)) {
                oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_COEX,
                                 "hmac_compability_ap_tpye_identify_for_btcoex: lev0 btcoex sco part middle priority!");
                *pen_ap_type |= MAC_AP_TYPE_BTCOEX_SCO_MIDDLE_PRIORITY;
            }
#ifdef _PRE_WLAN_FEATURE_11AX
            if (g_wlan_spec_cfg->feature_11ax_is_open) {
                if (MAC_IS_AX3000(puc_mac_addr) && (pst_mac_vap->en_protocol == WLAN_HE_MODE)) {
                    oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_COEX,
                                     "hmac_compability_ap_tpye_identify_for_btcoex: lev0 btcoex 20dbm blacklist!!");
                    *pen_ap_type |= MAC_AP_TYPE_BTCOEX_20DBM_BLACKLIST;
                }
            }
#endif
            
            if (MAC_IS_SCO_RETRY_BLACKLIST(puc_mac_addr)) {
                oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_COEX,
                    "hmac_compability_ap_tpye_identify_for_btcoex: lev0 btcoex sco retry middle priority!");
                *pen_ap_type |= MAC_AP_TYPE_BTCOEX_SCO_RETRY_MIDDLE_PRIORITY;
            }
        }
    }
}

OAL_STATIC void hmac_get_ap_tpye(mac_vap_stru *pst_mac_vap, mac_bss_dscr_stru *pst_bss_dscr)
{
#ifdef _PRE_WLAN_FEATURE_11AX
    if (g_wlan_spec_cfg->feature_11ax_is_open) {
        pst_mac_vap->st_cap_flag.bit_rx_stbc = OAL_TRUE;
        if (MAC_IS_HUAWEITE_AP(pst_bss_dscr)) {
            oam_warning_log0(pst_mac_vap->uc_vap_id, 0, "hmac_get_ap_tpye: rx stbc blacklist!");
            pst_mac_vap->st_cap_flag.bit_rx_stbc = OAL_FALSE;
        }
    }
#endif
}


mac_ap_type_enum_uint16 hmac_compability_ap_tpye_identify(mac_vap_stru *pst_mac_vap, uint8_t *puc_mac_addr)
{
    mac_ap_type_enum_uint16 en_ap_type = 0;

#if defined(_PRE_WLAN_1103_DDC_BUGFIX)
    wlan_nss_enum_uint8 en_support_max_nss = WLAN_SINGLE_NSS;
#endif

    mac_bss_dscr_stru *pst_bss_dscr = OAL_PTR_NULL;

    pst_bss_dscr = (mac_bss_dscr_stru *)hmac_scan_get_scanned_bss_by_bssid(pst_mac_vap, puc_mac_addr);

    if (MAC_IS_GOLDEN_AP(puc_mac_addr)) {
        en_ap_type |= MAC_AP_TYPE_GOLDENAP;
    }

    if (pst_bss_dscr != OAL_PTR_NULL) {
#ifdef _PRE_WLAN_1103_DDC_BUGFIX
        /* DDC白名单: AP OUI + chip OUI + 三流 + 2G */
        if (MAC_IS_BELKIN_AP(puc_mac_addr) || MAC_IS_TRENDNET_AP(puc_mac_addr) ||
            MAC_IS_NETGEAR_WNDR_AP(puc_mac_addr)) {
#ifdef _PRE_WLAN_FEATURE_M2S
            en_support_max_nss = pst_bss_dscr->en_support_max_nss;
#endif
            if (pst_bss_dscr->en_atheros_chip_oui == OAL_TRUE) {
                if ((MAC_IS_NETGEAR_WNDR_AP(puc_mac_addr) && en_support_max_nss == WLAN_DOUBLE_NSS) ||
                    ((MAC_IS_BELKIN_AP(puc_mac_addr) || MAC_IS_TRENDNET_AP(puc_mac_addr)) &&
                     (en_support_max_nss == WLAN_TRIPLE_NSS) && (pst_mac_vap->st_channel.en_band == WLAN_BAND_2G))) {
                    oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                                     "hmac_compability_ap_tpye_identify is in ddc whitelist!");
                    en_ap_type |= MAC_AP_TYPE_DDC_WHITELIST;
                }
            }
        }
#endif

        hmac_compability_ap_tpye_identify_for_btcoex(pst_mac_vap, pst_bss_dscr, puc_mac_addr, &en_ap_type);

#ifdef _PRE_WLAN_FEATURE_M2S
        /* 关联时候识别360随身wifi或者2G的TP-LINK7300,后续不能切siso */
        if (MAC_IS_360_AP0(puc_mac_addr) || MAC_IS_360_AP1(puc_mac_addr) || MAC_IS_360_AP2(puc_mac_addr) ||
            (MAC_IS_TP_LINK_7300(pst_bss_dscr) && (pst_mac_vap->st_channel.en_band == WLAN_BAND_2G))) {
            oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_M2S,
                             "hmac_compability_ap_tpye_identify: m2s blacklist!");
            en_ap_type |= MAC_AP_TYPE_M2S;
        }
#endif

        /* 关闭时候识别斐讯k3，不能进行漫游 */
        if (MAC_IS_FEIXUN_K3(puc_mac_addr)) {
            if (pst_bss_dscr->en_roam_blacklist_chip_oui == OAL_TRUE) {
                oam_warning_log0(pst_mac_vap->uc_vap_id, 0, "hmac_compability_ap_tpye_identify: roam blacklist!");
                en_ap_type |= MAC_AP_TYPE_ROAM;
            }
        }

        /* 识别HUAWEI ，不支持rx stbc */
        hmac_get_ap_tpye(pst_mac_vap, pst_bss_dscr);
        pst_mac_vap->bit_ap_chip_oui = pst_bss_dscr->en_is_tplink_oui;

        if ((pst_bss_dscr->en_atheros_chip_oui == OAL_TRUE) && (MAC_IS_LINKSYS_EA8500_AP(puc_mac_addr))) {
            oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_ROAM,
                             "hmac_compability_ap_tpye_identify: aggr blacklist!");
            en_ap_type |= MAC_AP_TYPE_AGGR_BLACKLIST;
        }
    }

    /* 打桩1102逻辑 */
    en_ap_type |= MAC_AP_TYPE_NORMAL;

    return en_ap_type;
}

void hmac_sdio_to_pcie_switch(mac_vap_stru *pst_mac_vap)
{
    /* 如果VAP工作在5G,则切换为PCIE */
    if (pst_mac_vap->st_channel.en_band == WLAN_BAND_5G) {
        hi110x_switch_to_hcc_highspeed_chan(1);
    }
}



void hmac_pcie_to_sdio_switch(mac_chip_stru *pst_mac_chip)
{
    /* chip下无其他关联用户，则切换为SDIO */
    if (pst_mac_chip->uc_assoc_user_cnt == 0) {
        hi110x_switch_to_hcc_highspeed_chan(0);
    }
}


void hmac_dbdc_need_kick_user(mac_vap_stru *pst_mac_vap, mac_device_stru *pst_mac_dev)
{
    uint8_t uc_vap_idx;
    mac_vap_stru *pst_another_vap = OAL_PTR_NULL;
    mac_cfg_kick_user_param_stru st_kick_user_param;
    uint8_t auc_mac_addr[WLAN_MAC_ADDR_LEN] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

    if (pst_mac_dev->en_dbdc_running == OAL_FALSE) {
        return;
    }

    /* DBDC状态下，优先踢掉处于关联状态的用户 */
    for (uc_vap_idx = 0; uc_vap_idx < pst_mac_dev->uc_vap_num; uc_vap_idx++) {
        pst_another_vap = mac_res_get_mac_vap(pst_mac_dev->auc_vap_id[uc_vap_idx]);
        if (pst_another_vap == OAL_PTR_NULL) {
            continue;
        }

        if (pst_another_vap->uc_vap_id == pst_mac_vap->uc_vap_id) {
            continue;
        }

        if (pst_another_vap->en_vap_state >= MAC_VAP_STATE_STA_JOIN_COMP &&
            pst_another_vap->en_vap_state <= MAC_VAP_STATE_STA_WAIT_ASOC) {
            oam_warning_log2(pst_another_vap->uc_vap_id, OAM_SF_UM,
                             "hmac_dbdc_need_kick_user::vap mode[%d] state[%d] kick user",
                             pst_another_vap->en_vap_mode, pst_another_vap->en_vap_state);

            st_kick_user_param.us_reason_code = MAC_UNSPEC_REASON;
            oal_set_mac_addr(st_kick_user_param.auc_mac_addr, auc_mac_addr);
            hmac_config_kick_user(pst_another_vap, OAL_SIZEOF(uint32_t), (uint8_t *)&st_kick_user_param);

            return;
        }
    }
}
OAL_STATIC void hmac_user_del_destroy_timer(hmac_user_stru *pst_hmac_user)
{
    if (pst_hmac_user->st_mgmt_timer.en_is_registerd == OAL_TRUE) {
        frw_timer_immediate_destroy_timer_m(&pst_hmac_user->st_mgmt_timer);
    }

    if (pst_hmac_user->st_defrag_timer.en_is_registerd == OAL_TRUE) {
        frw_timer_immediate_destroy_timer_m(&pst_hmac_user->st_defrag_timer);
    }
}
#ifdef _PRE_WLAN_FEATURE_PSM_ABN_PKTS_STAT
OAL_STATIC void hmac_abn_pkts_stat_clean(mac_vap_stru *pst_mac_vap)
{
    hmac_device_stru *pst_hmac_device;

    pst_hmac_device = hmac_res_get_mac_dev(pst_mac_vap->uc_device_id);
    if (oal_unlikely(pst_hmac_device == OAL_PTR_NULL)) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_UM, "{hmac_user_del::pst_hmac_device null.}");
        return;
    }
    if (mac_is_primary_legacy_sta(pst_mac_vap)) {
        pst_hmac_device->ul_addba_req_cnt = 0;
        pst_hmac_device->ul_group_rekey_cnt = 0;
    }
}
#endif
#ifdef CONFIG_ARCH_KIRIN_PCIE
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
static void hmac_user_del_update_hal_lut(uint8_t hal_dev_id, uint8_t user_index)
{
    hal_host_device_stru *haldev = NULL;
    uint8_t loop;

    haldev = hal_get_host_device(hal_dev_id);
    if (haldev == NULL) {
        return;
    }

    for (loop = 0; loop < HAL_MAX_LUT; loop++) {
        if (haldev->user_sts_info[loop].user_id == user_index) {
            haldev->user_sts_info[loop].user_valid = 0;
            oam_warning_log2(hal_dev_id, OAM_SF_ANY, "{hmac_update_hal_lut::lut[%d] user_index[%x]}",
                             loop, user_index);
            return;
        }
    }
    oam_warning_log1(hal_dev_id, OAM_SF_ANY, "{hmac_update_hal_lut::user_index[%x]}", user_index);
    return;
}
#endif
#endif

#ifdef _PRE_WLAN_FEATURE_WMMAC

OAL_STATIC void hmac_user_del_addts_timer(hmac_user_stru *pst_hmac_user)
{
    uint8_t uc_ac_index;
    for (uc_ac_index = 0; uc_ac_index < WLAN_WME_AC_BUTT; uc_ac_index++) {
        if (pst_hmac_user->st_user_base_info.st_ts_info[uc_ac_index].st_addts_timer.en_is_registerd == OAL_TRUE) {
            frw_timer_immediate_destroy_timer_m(
                &(pst_hmac_user->st_user_base_info.st_ts_info[uc_ac_index].st_addts_timer));
        }

        memset_s(&(pst_hmac_user->st_user_base_info.st_ts_info[uc_ac_index]), OAL_SIZEOF(mac_ts_stru), 0,
                 OAL_SIZEOF(mac_ts_stru));
        pst_hmac_user->st_user_base_info.st_ts_info[uc_ac_index].uc_tsid = 0xFF;
    }
}
#endif
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
#define USED_MEM_STR_LEN  5
uint32_t hmac_custom_set_mult_user_flowctrl(mac_vap_stru *mac_vap)
{
    uint8_t pc_param[16] = { 0 };
    int8_t pc_tmp[8] = { 0 };
    struct wlan_flow_ctrl_params flow_ctrl = wlan_chip_get_flow_ctrl_used_mem();

    oal_itoa(flow_ctrl.start, pc_param, USED_MEM_STR_LEN);
    oal_itoa(flow_ctrl.stop, pc_tmp, USED_MEM_STR_LEN);

    pc_param[OAL_STRLEN(pc_param)] = ' ';
    if (memcpy_s(pc_param + OAL_STRLEN(pc_param), sizeof(pc_param) - OAL_STRLEN(pc_param),
                 pc_tmp, OAL_STRLEN(pc_tmp)) != EOK) {
        oam_error_log0(0, OAM_SF_UM, "hmac_custom_set_mult_user_flowctrl::memcpy fail!");
        return OAL_FAIL;
    }
    hmac_config_sdio_flowctrl(mac_vap, (uint16_t)(OAL_STRLEN(pc_param) + 1), pc_param);

    return OAL_SUCC;
}
#endif

static void hmac_user_del_vap_init(hmac_vap_stru *hmac_vap, mac_device_stru *mac_device, hmac_user_stru *hmac_user)
{
    if (hmac_vap->st_vap_base_info.en_vap_mode == WLAN_VAP_MODE_BSS_STA) {
        hmac_fsm_change_state(hmac_vap, MAC_VAP_STATE_STA_FAKE_UP);
    } else if (hmac_vap->st_vap_base_info.en_vap_mode == WLAN_VAP_MODE_BSS_AP) {
        mac_vap_csa_support_set(&hmac_vap->st_vap_base_info, FALSE);
        if (hmac_user->assoc_err_code != MAC_IEEE_802_1X_AUTH_FAIL) {
            hmac_go_flow_sta_channel_handle(mac_device);
        }
    }
    hmac_user->assoc_err_code = 0; // 清除错误标志
}


uint32_t hmac_user_del(mac_vap_stru *pst_mac_vap, hmac_user_stru *pst_hmac_user)
{
    uint16_t us_user_index;
    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL;
    frw_event_stru *pst_event = OAL_PTR_NULL;
    dmac_ctx_del_user_stru *pst_del_user_payload = OAL_PTR_NULL;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    mac_device_stru *pst_mac_device = OAL_PTR_NULL;
    mac_user_stru *pst_mac_user = OAL_PTR_NULL;
    uint32_t ul_ret;
    mac_chip_stru *pst_mac_chip = OAL_PTR_NULL;
#ifdef _PRE_WLAN_FEATURE_WAPI
    hmac_user_stru *pst_hmac_user_multi = NULL;
#endif
    mac_ap_type_enum_uint16 en_ap_type = 0;
    if (oal_unlikely(oal_any_null_ptr2(pst_mac_vap, pst_hmac_user))) {
        oam_error_log0(0, OAM_SF_UM, "{hmac_user_del::param null}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    pst_mac_user = (mac_user_stru *)(&pst_hmac_user->st_user_base_info);
    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (oal_unlikely(pst_mac_device == OAL_PTR_NULL)) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_UM, "{hmac_user_del::pst_mac_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    hmac_dbdc_need_kick_user(pst_mac_vap, pst_mac_device);
    oam_warning_log4(pst_mac_vap->uc_vap_id, OAM_SF_UM, "{hmac_user_del::del user[%d] start,is multi user[%d], "
        "user mac:XX:XX:XX:XX:%02X:%02X}", pst_mac_user->us_assoc_id, pst_mac_user->en_is_multi_user,
        pst_mac_user->auc_user_mac_addr[4], pst_mac_user->auc_user_mac_addr[5]);
#ifdef _PRE_WLAN_FEATURE_HIEX
    hmac_hiex_user_exit(pst_hmac_user);
#endif
    /* 删除user时候，需要更新保护机制 */
    ul_ret = hmac_protection_del_user(pst_mac_vap, &(pst_hmac_user->st_user_base_info));
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_UM, "{hmac_user_del::hmac_protection_del_user[%d]}", ul_ret);
    }
    /* 删除用户统计保护相关的信息，向dmac同步然后设置相应的保护模式 */
    if (pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_AP) {
       if (hmac_user_protection_sync_data(pst_mac_vap) != OAL_SUCC) {
           oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC, "{hmac_user_del:prot update mibfail}");
       }
    }

    /* 获取用户对应的索引 */
    us_user_index = pst_hmac_user->st_user_base_info.us_assoc_id;

    /* 删除hmac user 的关联请求帧空间 */
    hmac_user_free_asoc_req_ie(pst_hmac_user->st_user_base_info.us_assoc_id);

#if (_PRE_WLAN_FEATURE_PMF != _PRE_PMF_NOT_SUPPORT)
    hmac_stop_sa_query_timer(pst_hmac_user);
#endif

#ifdef CONFIG_ARCH_KIRIN_PCIE
    hmac_deinit_tx_update_freq_timer(pst_hmac_user);
    hmac_user_tx_ring_deinit(pst_hmac_user);
#endif

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_UM, "{hmac_user_del::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#ifdef CONFIG_ARCH_KIRIN_PCIE
    hmac_user_del_update_hal_lut(pst_hmac_vap->hal_dev_id, us_user_index);
#endif
#endif
    if (pst_hmac_vap->st_vap_base_info.pst_vowifi_cfg_param != OAL_PTR_NULL) {
        if (pst_hmac_vap->st_vap_base_info.pst_vowifi_cfg_param->en_vowifi_mode == VOWIFI_LOW_THRES_REPORT) {
            /* 针对漫游和去关联场景,切换vowifi语音状态 */
            hmac_config_vowifi_report((&pst_hmac_vap->st_vap_base_info), 0, OAL_PTR_NULL);
        }
    }

    pst_mac_chip = hmac_res_get_mac_chip(pst_mac_device->uc_chip_id);
    if (pst_mac_chip == OAL_PTR_NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

#ifdef _PRE_WLAN_FEATURE_WAPI
    hmac_wapi_deinit(&pst_hmac_user->st_wapi);

    /* STA模式下，清组播wapi加密端口 */
    pst_hmac_user_multi = (hmac_user_stru *)mac_res_get_hmac_user(pst_hmac_vap->st_vap_base_info.us_multi_user_idx);
    if (pst_hmac_user_multi == OAL_PTR_NULL) {
        oam_error_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY, "{hmac_user_del::get user[%u] fail!}",
                       pst_hmac_vap->st_vap_base_info.us_multi_user_idx);
        return OAL_ERR_CODE_PTR_NULL;
    }

    hmac_wapi_reset_port(&pst_hmac_user_multi->st_wapi);

    pst_mac_device->uc_wapi = OAL_FALSE;

#endif

    if (pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_STA) {
        hmac_device_create_random_mac_addr(pst_mac_device, pst_mac_vap);

        mac_vap_set_aid(pst_mac_vap, 0);
        /* fastsleep可能修改ps mode，去关联时重置 */
        pst_hmac_vap->uc_ps_mode = g_wlan_ps_mode;
        pst_hmac_vap->ipaddr_obtained = OAL_FALSE;
        
        hmac_roam_wpas_connect_state_notify(pst_hmac_vap, WPAS_CONNECT_STATE_IPADDR_REMOVED);

        hmac_roam_exit(pst_hmac_vap);

        en_ap_type = hmac_compability_ap_tpye_identify(pst_mac_vap, pst_mac_user->auc_user_mac_addr);
    }

    /* 用户层可能修改tcp ack缓存机制，去关联时重置 */
    pst_hmac_vap->tcp_ack_buf_use_ctl_switch = OAL_SWITCH_OFF;

#ifdef _PRE_WLAN_FEATURE_PSM_ABN_PKTS_STAT
    hmac_abn_pkts_stat_clean(pst_mac_vap);
#endif

    /* 抛事件到DMAC层, 删除dmac用户 */
    pst_event_mem = frw_event_alloc_m(OAL_SIZEOF(dmac_ctx_del_user_stru));
    if (oal_unlikely(pst_event_mem == OAL_PTR_NULL)) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_UM, "{hmac_user_del::pst_event_mem null.}");
        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }

    pst_event = frw_get_event_stru(pst_event_mem);
    pst_del_user_payload = (dmac_ctx_del_user_stru *)pst_event->auc_event_data;
    pst_del_user_payload->us_user_idx = us_user_index;
    pst_del_user_payload->en_ap_type = en_ap_type;
#if (_PRE_OS_VERSION_WIN32 != _PRE_OS_VERSION)
    /* TBD: 添加此操作51DMT异常，暂看不出异常原因 */
    /* 用户 mac地址和idx 需至少一份有效，供dmac侧查找待删除的用户 */
    memcpy_s(pst_del_user_payload->auc_user_mac_addr, WLAN_MAC_ADDR_LEN,
             pst_mac_user->auc_user_mac_addr, WLAN_MAC_ADDR_LEN);
#endif

    /* 填充事件头 */
    frw_event_hdr_init(&(pst_event->st_event_hdr), FRW_EVENT_TYPE_WLAN_CTX, DMAC_WLAN_CTX_EVENT_SUB_TYPE_DEL_USER,
                       OAL_SIZEOF(dmac_ctx_del_user_stru), FRW_EVENT_PIPELINE_STAGE_1,  pst_mac_vap->uc_chip_id,
                       pst_mac_vap->uc_device_id, pst_mac_vap->uc_vap_id);

    ul_ret = frw_event_dispatch_event(pst_event_mem);
    frw_event_free_m(pst_event_mem);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        /* 做维测，如果删除用户失败，前面清hmac资源的操作本身已经异常，需要定位 */
        oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_UM, "{hmac_user_del::dispatch_event failed[%d].}", ul_ret);
        return ul_ret;
    }

#ifdef _PRE_WLAN_FEATURE_SMPS
    /* 删除用户，更新SMPS能力 */
    mac_user_set_sm_power_save(&pst_hmac_user->st_user_base_info, 0);
#endif

#if defined(_PRE_WLAN_FEATURE_WMMAC)
    /* 删除user时删除发送addts req超时定时器 */
    hmac_user_del_addts_timer(pst_hmac_user);
#endif  // defined(_PRE_WLAN_FEATURE_WMMAC)

    hmac_tid_clear(pst_mac_vap, pst_hmac_user);
    hmac_user_del_destroy_timer(pst_hmac_user);

    /* 从vap中删除用户 */
    mac_vap_del_user(pst_mac_vap, us_user_index);
    hmac_arp_probe_destory(pst_hmac_vap, pst_hmac_user);

    hmac_chan_update_40M_intol_user(pst_mac_vap);

#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
    if (pst_mac_vap->us_user_nums == 5) {
        /* AP用户达到5时，调整流控参数为配置文件原有值 */
        if (hmac_custom_set_mult_user_flowctrl(pst_mac_vap) != OAL_SUCC) {
            return OAL_FAIL;
        }
    }
#endif

    g_hmac_lut_index_tbl[pst_hmac_user->lut_index].user = NULL;
    mac_user_del_ra_lut_index(pst_mac_chip->st_lut_table.auc_ra_lut_index_table, pst_hmac_user->lut_index);
    pst_hmac_user->lut_index = WLAN_ASSOC_USER_MAX_NUM;

    /* 释放用户内存 */
    ul_ret = hmac_user_free(us_user_index);
    if (ul_ret == OAL_SUCC) {
        /* chip下已关联user个数-- */
        mac_chip_dec_assoc_user(pst_mac_chip);
    } else {
        oam_error_log1(0, OAM_SF_UM, "{hmac_user_del::mac_res_free_mac_user fail[%d].}", ul_ret);
    }

    hmac_user_del_vap_init(pst_hmac_vap, pst_mac_device, pst_hmac_user);

    /* 删除5G用户时需考虑是不是切为SDIO */
    hmac_pcie_to_sdio_switch(pst_mac_chip);

    pst_hmac_vap->en_roam_prohibit_on = OAL_FALSE;

    return OAL_SUCC;
}


OAL_STATIC uint32_t hmac_user_add_post_event(mac_vap_stru *pst_mac_vap, hmac_user_stru *pst_hmac_user,
                                             uint8_t *puc_mac_addr, uint16_t us_user_idx)
{
    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL;
    frw_event_stru *pst_event = OAL_PTR_NULL;
    dmac_ctx_add_user_stru *pst_add_user_payload = OAL_PTR_NULL;
    mac_bss_dscr_stru *pst_bss_dscr = OAL_PTR_NULL;
    uint32_t ul_ret;

    pst_event_mem = frw_event_alloc_m(OAL_SIZEOF(dmac_ctx_add_user_stru));
    if (oal_unlikely(pst_event_mem == OAL_PTR_NULL)) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_UM, "{hmac_user_add::pst_event_mem null.}");
        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }

    pst_event = frw_get_event_stru(pst_event_mem);
    pst_add_user_payload = (dmac_ctx_add_user_stru *)pst_event->auc_event_data;
    pst_add_user_payload->us_user_idx = us_user_idx;
    pst_add_user_payload->en_ap_type = pst_hmac_user->en_user_ap_type;
    pst_add_user_payload->lut_index = pst_hmac_user->lut_index;
    oal_set_mac_addr(pst_add_user_payload->auc_user_mac_addr, puc_mac_addr);

    /* 获取扫描的bss信息 */
    pst_bss_dscr = (mac_bss_dscr_stru *)hmac_scan_get_scanned_bss_by_bssid(pst_mac_vap, puc_mac_addr);
    pst_add_user_payload->c_rssi = (pst_bss_dscr != OAL_PTR_NULL) ?
                                   pst_bss_dscr->c_rssi : oal_get_real_rssi((int16_t)OAL_RSSI_INIT_MARKER);

    /* 填充事件头 */
    frw_event_hdr_init(&(pst_event->st_event_hdr),
                       FRW_EVENT_TYPE_WLAN_CTX,
                       DMAC_WLAN_CTX_EVENT_SUB_TYPE_ADD_USER,
                       OAL_SIZEOF(dmac_ctx_add_user_stru),
                       FRW_EVENT_PIPELINE_STAGE_1,
                       pst_mac_vap->uc_chip_id,
                       pst_mac_vap->uc_device_id,
                       pst_mac_vap->uc_vap_id);

    ul_ret = frw_event_dispatch_event(pst_event_mem);

    frw_event_free_m(pst_event_mem);
    return ul_ret;
}


uint32_t hmac_user_add(mac_vap_stru *pst_mac_vap, uint8_t *puc_mac_addr, uint16_t *pus_user_index)
{
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    hmac_user_stru *pst_hmac_user = OAL_PTR_NULL;
    uint32_t ul_ret;
    uint16_t us_user_idx;
    mac_cfg_80211_ucast_switch_stru st_80211_ucast_switch = { 0 };
    mac_ap_type_enum_uint16 en_ap_type = 0;
    mac_chip_stru *pst_mac_chip = OAL_PTR_NULL;
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
    int8_t pc_param[] = "30 25";
    uint16_t us_len;
#endif
#ifdef _PRE_WLAN_FEATURE_WAPI
    mac_device_stru *pst_mac_device = NULL;
#endif
    uint8_t lut_idx;

    if (oal_unlikely(oal_any_null_ptr3(pst_mac_vap, puc_mac_addr, pus_user_index))) {
        oam_error_log3(0, OAM_SF_UM, "{hmac_user_add::param null, %x %x %x.}",
                       (uintptr_t)pst_mac_vap, (uintptr_t)puc_mac_addr, (uintptr_t)pus_user_index);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_UM, "{hmac_user_add::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

#ifdef _PRE_WLAN_FEATURE_WAPI
    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (pst_mac_device == OAL_PTR_NULL) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_UM, "{hmac_user_add::pst_mac_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
#endif

    pst_mac_chip = hmac_res_get_mac_chip(pst_mac_vap->uc_chip_id);
    if (pst_mac_chip == OAL_PTR_NULL) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_UM, "{hmac_user_add::pst_mac_chip null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* chip级别最大用户数判断 */
    if (pst_mac_chip->uc_assoc_user_cnt >= mac_chip_get_max_asoc_user(pst_mac_chip->uc_chip_id)) {
        oam_warning_log1(0, OAM_SF_UM, "{hmac_user_add::invalid uc_assoc_user_cnt[%d].}",
                         pst_mac_chip->uc_assoc_user_cnt);
        return OAL_ERR_CODE_CONFIG_EXCEED_SPEC;
    }

    if (pst_hmac_vap->st_vap_base_info.us_user_nums >= mac_mib_get_MaxAssocUserNums(pst_mac_vap)) {
        oam_warning_log2(pst_mac_vap->uc_vap_id, OAM_SF_UM,
                         "{hmac_user_add::invalid us_user_nums[%d], us_user_nums_max[%d].}",
                         pst_hmac_vap->st_vap_base_info.us_user_nums, mac_mib_get_MaxAssocUserNums(pst_mac_vap));
        return OAL_ERR_CODE_CONFIG_EXCEED_SPEC;
    }

    /* 如果此用户已经创建，则返回失败 */
    ul_ret = mac_vap_find_user_by_macaddr(pst_mac_vap, puc_mac_addr, &us_user_idx);
    if (ul_ret == OAL_SUCC) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_UM,
                         "{hmac_user_add::mac_vap_find_user_by_macaddr success[%d].}", ul_ret);
        return OAL_FAIL;
    }

    if (pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_STA) {
        if (IS_P2P_CL(pst_mac_vap)) {
            if (pst_hmac_vap->st_vap_base_info.us_user_nums >= 2) {
                oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_UM,
                                 "{hmac_user_add::a STA can only associated with 2 ap.}");
                return OAL_FAIL;
            }
        } else
        {
            if (pst_hmac_vap->st_vap_base_info.us_user_nums >= 1) {
                oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_UM,
                                 "{hmac_user_add::a STA can only associated with one ap.}");
                return OAL_FAIL;
            }
            en_ap_type = hmac_compability_ap_tpye_identify(pst_mac_vap, puc_mac_addr);
        }
        hmac_roam_init(pst_hmac_vap);
    }

    /* 申请hmac用户内存，并初始清0 */
    ul_ret = hmac_user_alloc(&us_user_idx);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_UM,
                         "{hmac_user_add::hmac_user_alloc failed[%d].}", ul_ret);
        return ul_ret;
    }

    /* 单播用户不能使用userid为0，需重新申请一个。将userid作为aid分配给对端，处理psm时会出错 */
    if (us_user_idx == 0) {
        hmac_user_free(us_user_idx);
        ul_ret = hmac_user_alloc(&us_user_idx);
        if ((ul_ret != OAL_SUCC) || (us_user_idx == 0)) {
            oam_warning_log2(pst_mac_vap->uc_vap_id, OAM_SF_UM,
                             "{hmac_user_add::hmac_user_alloc failed ret[%d] us_user_idx[%d].}",
                             ul_ret, us_user_idx);
            return ul_ret;
        }
    }

    *pus_user_index = us_user_idx; /* 出参赋值 */

    pst_hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(us_user_idx);
    if (pst_hmac_user == OAL_PTR_NULL) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_UM, "{hmac_user_add::pst_hmac_user[%d] null}", us_user_idx);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 申请lut index */
    lut_idx = mac_user_get_ra_lut_index(pst_mac_chip->st_lut_table.auc_ra_lut_index_table, 0, WLAN_ASSOC_USER_MAX_NUM);
    if (lut_idx >= mac_chip_get_max_active_user()) {
        hmac_user_free(us_user_idx);
        oam_warning_log0(0, OAM_SF_ANY, "{hmac_user_add::uc_lut_idx >= WLAN_ASSOC_USER_MAX_NUM.}");
        return OAL_FAIL;
    }

    pst_hmac_user->lut_index = lut_idx;
    pst_hmac_user->en_user_ap_type = en_ap_type; /* AP类型 */

    g_hmac_lut_index_tbl[lut_idx].user = pst_hmac_user;

    /* 初始化mac_user_stru */
    mac_user_init(&(pst_hmac_user->st_user_base_info), us_user_idx, puc_mac_addr,
                  pst_mac_vap->uc_chip_id, pst_mac_vap->uc_device_id, pst_mac_vap->uc_vap_id);
    mac_user_init_rom(&(pst_hmac_user->st_user_base_info), us_user_idx);

#ifdef _PRE_WLAN_FEATURE_WAPI
    /* 初始化单播wapi对象 */
    hmac_wapi_init(&pst_hmac_user->st_wapi, OAL_TRUE);
    pst_mac_device->uc_wapi = OAL_FALSE;
#endif

#ifdef _PRE_WLAN_FEATURE_HIEX
    if (g_wlan_spec_cfg->feature_hiex_is_open) {
        hmac_hiex_user_init(pst_hmac_user);
    }
#endif

    /* 设置amsdu域 */
    hmac_amsdu_init_user(pst_hmac_user);

    pst_hmac_user->uc_tx_ba_limit = DMAC_UCAST_FRAME_TX_COMP_TIMES;

    /***************************************************************************
        抛事件到DMAC层, 创建dmac用户
    ***************************************************************************/
    ul_ret = hmac_user_add_post_event(pst_mac_vap, pst_hmac_user, puc_mac_addr, us_user_idx);
    if (ul_ret != OAL_SUCC) {
        /* 异常处理，释放内存，device下关联用户数还没有++，这里不需要判断返回值做--操作 */
        hmac_user_free(us_user_idx);
        /* 不应该出现用户添加失败，失败需要定位具体原因 */
        oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_UM,
                       "{hmac_user_add::frw_event_dispatch_event failed[%d].}", ul_ret);
        return ul_ret;
    }

    /* 添加用户到MAC VAP */
    ul_ret = mac_vap_add_assoc_user(pst_mac_vap, us_user_idx);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_UM,
                         "{hmac_user_add::mac_vap_add_assoc_user failed[%d].}", ul_ret);

        /* 异常处理，释放内存，device下关联用户数还没有++，这里不需要判断返回值做--操作 */
        hmac_user_free(us_user_idx);
        return OAL_FAIL;
    }

#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
    if (pst_mac_vap->us_user_nums == 6) {
        /* AP用户达到6时，调整流控参数为Stop为25，Start为30 */
        us_len = (uint16_t)(OAL_STRLEN(pc_param) + 1);
        hmac_config_sdio_flowctrl(pst_mac_vap, us_len, pc_param);
    }
#endif

    /* 初始话hmac user部分信息 */
    hmac_user_init(pst_hmac_user);
    hmac_arp_probe_init(pst_hmac_vap, pst_hmac_user);
    mac_chip_inc_assoc_user(pst_mac_chip);

    /* 打开80211单播管理帧开关，观察关联过程，关联成功了就关闭 */
    st_80211_ucast_switch.en_frame_direction = OAM_OTA_FRAME_DIRECTION_TYPE_TX;
    st_80211_ucast_switch.en_frame_type = OAM_USER_TRACK_FRAME_TYPE_MGMT;
    st_80211_ucast_switch.en_frame_switch = OAL_SWITCH_ON;
    st_80211_ucast_switch.en_cb_switch = OAL_SWITCH_ON;
    st_80211_ucast_switch.en_dscr_switch = OAL_SWITCH_ON;
    memcpy_s(st_80211_ucast_switch.auc_user_macaddr, OAL_SIZEOF(st_80211_ucast_switch.auc_user_macaddr),
             (const void *)puc_mac_addr, OAL_SIZEOF(st_80211_ucast_switch.auc_user_macaddr));
    hmac_config_80211_ucast_switch(pst_mac_vap, OAL_SIZEOF(st_80211_ucast_switch),
                                   (uint8_t *)&st_80211_ucast_switch);

    st_80211_ucast_switch.en_frame_direction = OAM_OTA_FRAME_DIRECTION_TYPE_RX;
    st_80211_ucast_switch.en_frame_type = OAM_USER_TRACK_FRAME_TYPE_MGMT;
    st_80211_ucast_switch.en_frame_switch = OAL_SWITCH_ON;
    st_80211_ucast_switch.en_cb_switch = OAL_SWITCH_ON;
    st_80211_ucast_switch.en_dscr_switch = OAL_SWITCH_ON;
    hmac_config_80211_ucast_switch(pst_mac_vap, OAL_SIZEOF(st_80211_ucast_switch),
                                   (uint8_t *)&st_80211_ucast_switch);
    /* 添加5G用户时需考虑是不是切为PCIE */
    hmac_sdio_to_pcie_switch(pst_mac_vap);

    pst_hmac_vap->en_roam_prohibit_on = (en_ap_type & MAC_AP_TYPE_ROAM) ? OAL_TRUE : OAL_FALSE;

    oam_warning_log4(pst_mac_vap->uc_vap_id, OAM_SF_UM, "{hmac_user_add::user[%d] mac:%02X:XX:XX:XX:%02X:%02X}",
                     us_user_idx,
                     puc_mac_addr[0],
                     puc_mac_addr[4],
                     puc_mac_addr[5]);

    return OAL_SUCC;
}


uint32_t hmac_config_add_user(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    mac_cfg_add_user_param_stru *pst_add_user = OAL_PTR_NULL;
    uint16_t us_user_index;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    hmac_user_stru *pst_hmac_user = OAL_PTR_NULL;
    uint32_t ul_ret;
    mac_user_ht_hdl_stru st_ht_hdl;
    uint32_t ul_rslt;
    mac_device_stru *pst_mac_device = OAL_PTR_NULL;
    mac_chip_stru *pst_mac_chip = OAL_PTR_NULL;

    pst_add_user = (mac_cfg_add_user_param_stru *)puc_param;

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_UM, "{hmac_config_add_user::pst_hmac_vap null.}");
        return OAL_FAIL;
    }

    ul_ret = hmac_user_add(pst_mac_vap, pst_add_user->auc_mac_addr, &us_user_index);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_UM,
                         "{hmac_config_add_user::hmac_user_add failed.}", ul_ret);
        return ul_ret;
    }

    pst_hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(us_user_index);
    if (pst_hmac_user == OAL_PTR_NULL) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_UM,
                         "{hmac_config_add_user::pst_hmac_user[%d] null.}", us_user_index);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* TBD hmac_config_add_user 此接口删除，相应调用需要整改，duankaiyong&guyanjie */
    /* 设置qos域，后续如有需要可以通过配置命令参数配置 */
    mac_user_set_qos(&pst_hmac_user->st_user_base_info, OAL_TRUE);

    /* 设置HT域 */
    mac_user_get_ht_hdl(&pst_hmac_user->st_user_base_info, &st_ht_hdl);
    st_ht_hdl.en_ht_capable = pst_add_user->en_ht_cap;

    if (pst_add_user->en_ht_cap == OAL_TRUE) {
        pst_hmac_user->st_user_base_info.en_cur_protocol_mode = WLAN_HT_MODE;
        pst_hmac_user->st_user_base_info.en_avail_protocol_mode = WLAN_HT_MODE;
    }

    /* 设置HT相关的信息:应该在关联的时候赋值 这个值配置的合理性有待考究 2012->page:786 */
    st_ht_hdl.uc_min_mpdu_start_spacing = 6;
    st_ht_hdl.uc_max_rx_ampdu_factor = 3;
    mac_user_set_ht_hdl(&pst_hmac_user->st_user_base_info, &st_ht_hdl);

    mac_user_set_asoc_state(&pst_hmac_user->st_user_base_info, MAC_USER_STATE_ASSOC);

    /* 设置amsdu域 */
    hmac_amsdu_init_user(pst_hmac_user);

    /***************************************************************************
        抛事件到DMAC层, 同步DMAC数据
    ***************************************************************************/
    /* 重新设置DMAC需要的参数 */
    pst_add_user->us_user_idx = us_user_index;

    ul_ret = hmac_config_send_event(&pst_hmac_vap->st_vap_base_info,
                                    WLAN_CFGID_ADD_USER,
                                    us_len,
                                    puc_param);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        /* 异常处理，释放内存 */
        ul_rslt = hmac_user_free(us_user_index);
        if (ul_rslt == OAL_SUCC) {
            pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
            if (pst_mac_device == OAL_PTR_NULL) {
                oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_UM, "{hmac_config_add_user::pst_mac_device null.}");
                return OAL_ERR_CODE_PTR_NULL;
            }

            pst_mac_chip = hmac_res_get_mac_chip(pst_mac_device->uc_chip_id);
            if (pst_mac_chip == OAL_PTR_NULL) {
                oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_UM, "{hmac_user_add::pst_mac_chip null.}");
                return OAL_ERR_CODE_PTR_NULL;
            }

            /* hmac_add_user成功时chip下关联用户数已经++, 这里的chip下已关联user个数要-- */
            mac_chip_dec_assoc_user(pst_mac_chip);
        }

        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_UM,
                         "{hmac_config_add_user::hmac_config_send_event failed[%d].}", ul_ret);
        return ul_ret;
    }

    /* 增加用户后通知算法初始化用户结构体 */
    hmac_user_add_notify_alg(&pst_hmac_vap->st_vap_base_info, us_user_index);

    if (IS_LEGACY_VAP(pst_mac_vap)) {
        mac_vap_state_change(pst_mac_vap, MAC_VAP_STATE_UP);
    }

    return OAL_SUCC;
}


uint32_t hmac_config_del_user(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    mac_cfg_del_user_param_stru *pst_del_user = OAL_PTR_NULL;
    hmac_user_stru *pst_hmac_user = OAL_PTR_NULL;
    hmac_vap_stru *pst_hmac_vap;
    uint16_t us_user_index;
    uint32_t ul_ret;

    pst_del_user = (mac_cfg_add_user_param_stru *)puc_param;

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_UM, "{hmac_config_del_user::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 获取用户对应的索引 */
    ul_ret = mac_vap_find_user_by_macaddr(pst_mac_vap, pst_del_user->auc_mac_addr, &us_user_index);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_UM,
                         "{hmac_config_del_user::mac_vap_find_user_by_macaddr failed[%d].}", ul_ret);
        return ul_ret;
    }

    /* 获取hmac用户 */
    pst_hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(us_user_index);
    if (pst_hmac_user == OAL_PTR_NULL) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_UM,
                         "{hmac_config_del_user::pst_hmac_user[%d] null.}", us_user_index);
        return OAL_ERR_CODE_PTR_NULL;
    }

    ul_ret = hmac_user_del(pst_mac_vap, pst_hmac_user);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log2(pst_mac_vap->uc_vap_id, OAM_SF_UM,
                         "{hmac_config_del_user::hmac_user_del failed[%d] device_id[%d].}",
                         ul_ret, pst_mac_vap->uc_device_id);
        return ul_ret;
    }

    return OAL_SUCC;
}


uint32_t hmac_user_add_multi_user(mac_vap_stru *pst_mac_vap, uint16_t *pus_user_index)
{
    uint32_t ul_ret;
    uint16_t us_user_index;
    mac_user_stru *pst_mac_user = OAL_PTR_NULL;
#ifdef _PRE_WLAN_FEATURE_WAPI
    hmac_user_stru *pst_hmac_user = NULL;
#endif

    ul_ret = hmac_user_alloc(&us_user_index);
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_UM,
                       "{hmac_user_add_multi_user::hmac_user_alloc failed[%d].}", ul_ret);
        return ul_ret;
    }

    /* 初始化组播用户基本信息 */
    pst_mac_user = (mac_user_stru *)mac_res_get_mac_user(us_user_index);
    if (pst_mac_user == OAL_PTR_NULL) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_UM,
                         "{hmac_user_add_multi_user::pst_mac_user[%d] null.}", us_user_index);
        return OAL_ERR_CODE_PTR_NULL;
    }

    mac_user_init(pst_mac_user, us_user_index, OAL_PTR_NULL,
                  pst_mac_vap->uc_chip_id, pst_mac_vap->uc_device_id, pst_mac_vap->uc_vap_id);
    mac_user_init_rom(pst_mac_user, us_user_index);

    *pus_user_index = us_user_index;

#ifdef _PRE_WLAN_FEATURE_WAPI
    pst_hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(us_user_index);
    if (pst_hmac_user == OAL_PTR_NULL) {
        oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                       "{hmac_user_add_multi_user::hmac_user[%d] null.}", us_user_index);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 初始化wapi对象 */
    hmac_wapi_init(&pst_hmac_user->st_wapi, OAL_FALSE);
#endif

    oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                     "{hmac_user_add_multi_user, user index[%d].}", us_user_index);

    return OAL_SUCC;
}


uint32_t hmac_user_del_multi_user(mac_vap_stru *pst_mac_vap)
{
#ifdef _PRE_WLAN_FEATURE_WAPI
    hmac_user_stru *pst_hmac_user;
#endif

#ifdef _PRE_WLAN_FEATURE_WAPI
    pst_hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(pst_mac_vap->us_multi_user_idx);
    if (pst_hmac_user == OAL_PTR_NULL) {
        oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{hmac_user_del_multi_user::get hmac_user[%d] null.}",
                       pst_mac_vap->us_multi_user_idx);
        return OAL_ERR_CODE_PTR_NULL;
    }

    hmac_wapi_deinit(&pst_hmac_user->st_wapi);
#endif

    hmac_user_free(pst_mac_vap->us_multi_user_idx);

    return OAL_SUCC;
}


#ifdef _PRE_WLAN_FEATURE_WAPI
uint8_t hmac_user_is_wapi_connected(uint8_t uc_device_id)
{
    uint8_t uc_vap_idx;
    hmac_user_stru *pst_hmac_user_multi = NULL;
    mac_device_stru *pst_mac_device = NULL;
    mac_vap_stru *pst_mac_vap = NULL;

    pst_mac_device = mac_res_get_dev(uc_device_id);
    if (oal_unlikely(pst_mac_device == OAL_PTR_NULL)) {
        oam_error_log1(0, OAM_SF_UM, "{hmac_user_is_wapi_connected::pst_mac_device null.id %u}", uc_device_id);
        return OAL_FALSE;
    }

    for (uc_vap_idx = 0; uc_vap_idx < pst_mac_device->uc_vap_num; uc_vap_idx++) {
        pst_mac_vap = mac_res_get_mac_vap(pst_mac_device->auc_vap_id[uc_vap_idx]);
        if (oal_unlikely(pst_mac_vap == OAL_PTR_NULL)) {
            oam_warning_log1(0, OAM_SF_CFG, "vap is null! vap id is %d", pst_mac_device->auc_vap_id[uc_vap_idx]);
            continue;
        }

        if (!IS_STA(pst_mac_vap)) {
            continue;
        }

        pst_hmac_user_multi = (hmac_user_stru *)mac_res_get_hmac_user(pst_mac_vap->us_multi_user_idx);
        if ((pst_hmac_user_multi != OAL_PTR_NULL)
            && (WAPI_PORT_FLAG(&pst_hmac_user_multi->st_wapi) == OAL_TRUE)) {
            return OAL_TRUE;
        }
    }

    return OAL_FALSE;
}
#endif /* #ifdef _PRE_WLAN_FEATURE_WAPI */


uint32_t hmac_user_add_notify_alg(mac_vap_stru *pst_mac_vap, uint16_t us_user_idx)
{
    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL;
    frw_event_stru *pst_event = OAL_PTR_NULL;
    dmac_ctx_add_user_stru *pst_add_user_payload = OAL_PTR_NULL;
    uint32_t ul_ret;
    hmac_user_stru *pst_hmac_user = OAL_PTR_NULL;

    /* 抛事件给Dmac，在dmac层挂用户算法钩子 */
    pst_event_mem = frw_event_alloc_m(OAL_SIZEOF(dmac_ctx_add_user_stru));
    if (oal_unlikely(pst_event_mem == OAL_PTR_NULL)) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{hmac_user_add_notify_alg::pst_event_mem null.}");
        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }

    pst_event = frw_get_event_stru(pst_event_mem);
    pst_add_user_payload = (dmac_ctx_add_user_stru *)pst_event->auc_event_data;
    pst_add_user_payload->us_user_idx = us_user_idx;
    pst_add_user_payload->us_sta_aid = pst_mac_vap->us_sta_aid;
    pst_hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(us_user_idx);
    if (oal_unlikely(pst_hmac_user == OAL_PTR_NULL)) {
        oam_error_log1(0, OAM_SF_CFG, "{hmac_user_add_notify_alg::null param,pst_hmac_user[%d].}", us_user_idx);
        frw_event_free_m(pst_event_mem);
        return OAL_ERR_CODE_PTR_NULL;
    }

    mac_user_get_vht_hdl(&pst_hmac_user->st_user_base_info, &pst_add_user_payload->st_vht_hdl);
    mac_user_get_ht_hdl(&pst_hmac_user->st_user_base_info, &pst_add_user_payload->st_ht_hdl);
    /* 填充事件头 */
    frw_event_hdr_init(&(pst_event->st_event_hdr),
                       FRW_EVENT_TYPE_WLAN_CTX,
                       DMAC_WLAN_CTX_EVENT_SUB_TYPE_NOTIFY_ALG_ADD_USER,
                       OAL_SIZEOF(dmac_ctx_add_user_stru),
                       FRW_EVENT_PIPELINE_STAGE_1,
                       pst_mac_vap->uc_chip_id,
                       pst_mac_vap->uc_device_id,
                       pst_mac_vap->uc_vap_id);

    ul_ret = frw_event_dispatch_event(pst_event_mem);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        /* 异常处理，释放内存 */
        frw_event_free_m(pst_event_mem);

        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{hmac_user_add_notify_alg::frw_event_dispatch_event failed[%d].}", ul_ret);
        return ul_ret;
    }
    frw_event_free_m(pst_event_mem);

    return OAL_SUCC;
}


hmac_user_stru *mac_vap_get_hmac_user_by_addr(mac_vap_stru *pst_mac_vap, uint8_t *puc_mac_addr)
{
    uint32_t ul_ret;
    uint16_t us_user_idx = 0xffff;
    hmac_user_stru *pst_hmac_user = OAL_PTR_NULL;

    /* 根据mac addr找sta索引 */
    ul_ret = mac_vap_find_user_by_macaddr(pst_mac_vap, puc_mac_addr, &us_user_idx);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY,
                         "{mac_vap_get_hmac_user_by_addr::find_user_by_macaddr failed[%d].}", ul_ret);
        if (puc_mac_addr != OAL_PTR_NULL) {
            oam_warning_log3(0, OAM_SF_ANY, "{mac_vap_get_hmac_user_by_addr:: mac_addr[%02x XX XX XX %02x %02x]!.}",
                             puc_mac_addr[0], puc_mac_addr[4], puc_mac_addr[5]);
        }
        return OAL_PTR_NULL;
    }

    /* 根据sta索引找到user内存区域 */
    pst_hmac_user = mac_res_get_hmac_user(us_user_idx);
    if (pst_hmac_user == OAL_PTR_NULL) {
        oam_error_log1(0, OAM_SF_ANY, "{mac_vap_get_hmac_user_by_addr::user[%d] ptr null.}", us_user_idx);
    }
    return pst_hmac_user;
}


void hmac_user_set_amsdu_level(hmac_user_stru *pst_hmac_user,
                               wlan_tx_amsdu_enum_uint8 en_amsdu_level)
{
    pst_hmac_user->en_amsdu_level = en_amsdu_level;

    oam_warning_log1(0, OAM_SF_ANY, "{hmac_user_set_amsdu_level::en_amsdu_level=[%d].}",
                     pst_hmac_user->en_amsdu_level);
}


void hmac_user_set_amsdu_level_by_max_mpdu_len(hmac_user_stru *pst_hmac_user,
                                               uint16_t us_max_mpdu_len)
{
    wlan_tx_amsdu_enum_uint8 en_amsdu_level;

    if (us_max_mpdu_len == 3895) {
        en_amsdu_level = WLAN_TX_AMSDU_BY_2;
    } else {
        en_amsdu_level = WLAN_TX_AMSDU_BY_4;
    }

    hmac_user_set_amsdu_level(pst_hmac_user, en_amsdu_level);
}

void hmac_user_set_close_ht_flag(mac_vap_stru *p_mac_vap, hmac_user_stru *p_hmac_user)
{
    if (g_wlan_spec_cfg->feature_ht_self_cure_is_open == OAL_FALSE ||
        p_mac_vap->en_protocol >= WLAN_HT_MODE) {
        return;
    }

    /* 首次入网 */
    if (p_mac_vap->en_vap_state != MAC_VAP_STATE_ROAMING) {
        if (hmac_ht_self_cure_in_blacklist(p_hmac_user->st_user_base_info.auc_user_mac_addr)) {
            p_hmac_user->closeHtFalg = OAL_TRUE;
            oam_warning_log4(0, OAM_SF_SCAN, "hmac_user_set_close_ht_flag::closeHtFalg=%d, %02X:XX:XX:XX:%02X:%02X",
                             p_hmac_user->closeHtFalg,
                             p_hmac_user->st_user_base_info.auc_user_mac_addr[0],  /* 0 mac地址第0字节 */
                             p_hmac_user->st_user_base_info.auc_user_mac_addr[4],  /* 4 mac地址第4字节 */
                             p_hmac_user->st_user_base_info.auc_user_mac_addr[5]); /* 5 mac地址第5字节 */
        }
    }
}

void hmac_user_cap_update_by_hisi_cap_ie(hmac_user_stru *pst_hmac_user,
                                         uint8_t *puc_payload, uint32_t ul_msg_len)
{
    mac_hisi_cap_vendor_ie_stru st_hisi_cap_ie;

    pst_hmac_user->st_user_base_info.st_cap_info.bit_p2p_support_csa = OAL_FALSE;
    pst_hmac_user->st_user_base_info.st_cap_info.bit_dcm_cap         = OAL_FALSE;

    if (mac_get_hisi_cap_vendor_ie(puc_payload, ul_msg_len, &st_hisi_cap_ie) == OAL_SUCC) {
#ifdef _PRE_WLAN_FEATURE_11AX
        if ((g_wlan_spec_cfg->feature_11ax_is_open) && (st_hisi_cap_ie.bit_11ax_support != OAL_FALSE)) {
            pst_hmac_user->st_user_base_info.st_cap_info.bit_dcm_cap = st_hisi_cap_ie.bit_dcm_support;
        }
#endif
        pst_hmac_user->st_user_base_info.st_cap_info.bit_p2p_support_csa = st_hisi_cap_ie.bit_p2p_csa_support;
        pst_hmac_user->st_user_base_info.st_cap_info.bit_p2p_scenes = st_hisi_cap_ie.bit_p2p_scenes;
    }
}

/*lint -e19*/
oal_module_symbol(hmac_user_alloc);
oal_module_symbol(hmac_user_init);
oal_module_symbol(hmac_config_kick_user);
oal_module_symbol(mac_vap_get_hmac_user_by_addr);
oal_module_symbol(mac_res_get_hmac_user);
oal_module_symbol(hmac_user_free_asoc_req_ie);
oal_module_symbol(hmac_user_set_asoc_req_ie); /*lint +e19*/
