

/* 1 头文件包含 */
#include "wlan_spec.h"
#include "wlan_mib.h"

#include "mac_frame.h"
#include "mac_ie.h"
#include "mac_regdomain.h"
#include "mac_user.h"
#include "mac_vap.h"

#include "mac_device.h"
#include "hmac_device.h"
#include "hmac_user.h"
#include "hmac_mgmt_sta.h"
#include "hmac_fsm.h"
#include "hmac_rx_data.h"
#include "hmac_chan_mgmt.h"
#include "hmac_mgmt_bss_comm.h"
#include "hmac_encap_frame_sta.h"
#include "hmac_sme_sta.h"
#include "hmac_scan.h"

#include "hmac_tx_amsdu.h"

#include "hmac_11i.h"

#include "hmac_protection.h"

#include "hmac_config.h"
#include "hmac_ext_if.h"
#include "hmac_p2p.h"

#include "hmac_mgmt_ap.h"

#include "hmac_blockack.h"
#include "frw_ext_if.h"

#include "hmac_roam_main.h"

#ifdef _PRE_WLAN_FEATURE_WAPI
#include "hmac_wapi.h"
#endif

#include "hisi_customize_wifi.h"

#ifdef _PRE_WLAN_FEATURE_VIRTUAL_MULTI_STA
#include "hmac_wds.h"
#endif

#ifdef _PRE_WLAN_FEATURE_SMPS
#include "hmac_smps.h"
#endif

#ifdef _PRE_WLAN_FEATURE_OPMODE_NOTIFY
#include "hmac_opmode.h"
#endif

#ifdef _PRE_WLAN_FEATURE_WMMAC
#include "hmac_wmmac.h"
#endif

#ifdef _PRE_WLAN_FEATURE_11V_ENABLE
#include "hmac_11v.h"
#endif

#include "hmac_btcoex.h"

#ifdef _PRE_WLAN_FEATURE_SNIFFER
#ifdef CONFIG_HW_SNIFFER
#include <hwnet/ipv4/sysctl_sniffer.h>
#endif
#endif
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include "plat_pm_wlan.h"
#endif

#ifdef _PRE_WLAN_FEATURE_HIEX
#include "hmac_hiex.h"
#endif
#ifdef _PRE_WLAN_FEATURE_11AX
#include "hmac_wifi6_self_cure.h"
#endif
#include "hmac_ht_self_cure.h"
#include "securec.h"
#include "securectype.h"
#ifdef _PRE_WLAN_FEATURE_TWT
#include "hmac_twt.h"
#endif

#undef THIS_FILE_ID
#define THIS_FILE_ID       OAM_FILE_ID_HMAC_MGMT_STA_C
#define MAC_ADDR(_puc_mac) ((uint32_t)(((uint32_t)(_puc_mac)[2] << 24) |  \
                                         ((uint32_t)(_puc_mac)[3] << 16) |  \
                                         ((uint32_t)(_puc_mac)[4] << 8) |  \
                                         ((uint32_t)(_puc_mac)[5])))

/* _puc_ie是指向ht cap字段的指针，故偏移5,6,7,8字节分别对应MCS四条空间流所支持的速率 */
#define IS_INVALID_HT_RATE_HP(_puc_ie) \
    ((0x02 == (_puc_ie)[5]) && (0x00 == (_puc_ie)[6]) && (0x05 == (_puc_ie)[7]) && (0x00 == (_puc_ie)[8]))

/* 2 静态函数声明 */
/* 3 全局变量定义 */
/* 11b 协议速率 */
hmac_data_rate_stru g_st_data_11b_rates[WLAN_11B_BUTT] = {
    { 0x82, 0x02, WLAN_11B_1_M_BPS },       /* 1 Mbps   */
    { 0x84, 0x04, WLAN_11B_2_M_BPS },       /* 2 Mbps   */
    { 0x8B, 0x0B, WLAN_11B_5_HALF_M_BPS }, /* 5.5 Mbps */
    { 0x96, 0x16, WLAN_11B_11_M_BPS }       /* 11 Mbps  */
};

/* 11ag 协议速率 */
hmac_data_rate_stru g_st_data_legacy_ofdm_rates[WLAN_11AG_BUTT] = {
    { 0x8C, 0x0C, WLAN_11AG_6M_BPS },  /* 6 Mbps   */
    { 0x92, 0x12, WLAN_11AG_9M_BPS },  /* 9 Mbps   */
    { 0x98, 0x18, WLAN_11AG_12M_BPS }, /* 12 Mbps  */
    { 0xA4, 0x24, WLAN_11AG_18M_BPS }, /* 18 Mbps  */
    { 0xB0, 0x30, WLAN_11AG_24M_BPS }, /* 24 Mbps  */
    { 0xC8, 0x48, WLAN_11AG_36M_BPS }, /* 36 Mbps  */
    { 0xE0, 0x60, WLAN_11AG_48M_BPS }, /* 48 Mbps  */
    { 0xEC, 0x6C, WLAN_11AG_54M_BPS }  /* 54 Mbps  */
};
/* 4 函数实现 */

OAL_STATIC uint32_t hmac_mgmt_timeout_sta(void *arg)
{
    hmac_vap_stru *hmac_vap = OAL_PTR_NULL;
    hmac_mgmt_timeout_param_stru *timeout_param;

    timeout_param = (hmac_mgmt_timeout_param_stru *)arg;
    if (timeout_param == OAL_PTR_NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(timeout_param->uc_vap_id);
    if (hmac_vap == OAL_PTR_NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    return hmac_fsm_call_func_sta(hmac_vap, HMAC_FSM_INPUT_TIMER0_OUT, timeout_param);
}


void hmac_update_join_req_params_2040(mac_vap_stru *mac_vap, mac_bss_dscr_stru *bss_dscr)
{
    if (((OAL_FALSE == mac_mib_get_HighThroughputOptionImplemented(mac_vap)) &&
         (OAL_FALSE == mac_mib_get_VHTOptionImplemented(mac_vap))) ||
        ((bss_dscr->en_ht_capable == OAL_FALSE) && (bss_dscr->en_vht_capable == OAL_FALSE))) {
        mac_vap->st_channel.en_bandwidth = WLAN_BAND_WIDTH_20M;
        return;
    }

    /* 使能40MHz */
    /* (1) 用户开启"40MHz运行"特性(即STA侧 dot11FortyMHzOperationImplemented为true) */
    /* (2) AP在40MHz运行 */
    if (OAL_TRUE == mac_mib_get_FortyMHzOperationImplemented(mac_vap)) {
        switch (bss_dscr->en_channel_bandwidth) {
            case WLAN_BAND_WIDTH_40PLUS:
            case WLAN_BAND_WIDTH_80PLUSPLUS:
            case WLAN_BAND_WIDTH_80PLUSMINUS:
                mac_vap->st_channel.en_bandwidth = WLAN_BAND_WIDTH_40PLUS;
                break;

            case WLAN_BAND_WIDTH_40MINUS:
            case WLAN_BAND_WIDTH_80MINUSPLUS:
            case WLAN_BAND_WIDTH_80MINUSMINUS:
                mac_vap->st_channel.en_bandwidth = WLAN_BAND_WIDTH_40MINUS;
                break;

            default:
                mac_vap->st_channel.en_bandwidth = WLAN_BAND_WIDTH_20M;
                break;
        }
    }

    /* 更新STA侧带宽与AP一致 */
    /* (1) STA AP均支持11AC */
    /* (2) STA支持40M带宽(FortyMHzOperationImplemented为TRUE)，
           定制化禁止2GHT40时，2G下FortyMHzOperationImplemented=FALSE，不更新带宽 */
    /* (3) STA支持80M带宽(即STA侧 dot11VHTChannelWidthOptionImplemented为0) */
    if ((OAL_TRUE == mac_mib_get_VHTOptionImplemented(mac_vap)) &&
        (bss_dscr->en_vht_capable == OAL_TRUE)) {
        if (OAL_TRUE == mac_mib_get_FortyMHzOperationImplemented(mac_vap)) {
            /* 不超过mac device最大带宽能力 */
            MAC_VAP_GET_CAP_BW(mac_vap) = mac_vap_get_bandwith(mac_mib_get_dot11VapMaxBandWidth(mac_vap),
                                                               bss_dscr->en_channel_bandwidth);
        }
    }

    oam_warning_log2(mac_vap->uc_vap_id, OAM_SF_2040,
                     "{hmac_update_join_req_params_2040::en_channel_bandwidth=%d, mac vap bw[%d].}",
                     bss_dscr->en_channel_bandwidth, MAC_VAP_GET_CAP_BW(mac_vap));

    /* 如果AP和STA同时支持20/40共存管理功能，则使能STA侧频谱管理功能 */
    if ((OAL_TRUE == mac_mib_get_2040BSSCoexistenceManagementSupport(mac_vap)) &&
        (bss_dscr->uc_coex_mgmt_supp == 1)) {
        mac_mib_set_SpectrumManagementImplemented(mac_vap, OAL_TRUE);
    }
}


OAL_STATIC void hmac_update_join_req_params_prot_sta(hmac_vap_stru *hmac_vap, hmac_join_req_stru *join_req)
{
    if (WLAN_MIB_DESIRED_BSSTYPE_INFRA == mac_mib_get_DesiredBSSType(&hmac_vap->st_vap_base_info)) {
        hmac_vap->st_vap_base_info.st_cap_flag.bit_wmm_cap = join_req->st_bss_dscr.uc_wmm_cap;
        mac_vap_set_uapsd_cap(&hmac_vap->st_vap_base_info, join_req->st_bss_dscr.uc_uapsd_cap);
    }

    hmac_update_join_req_params_2040(&(hmac_vap->st_vap_base_info), &(join_req->st_bss_dscr));
}


OAL_STATIC void hmac_sta_bandwidth_down_by_channel(mac_vap_stru *mac_vap)
{
    oal_bool_enum_uint8 bandwidth_change_to_20M = OAL_FALSE;

    switch (mac_vap->st_channel.en_bandwidth) {
        case WLAN_BAND_WIDTH_40PLUS:
            /* 1. 64 144 161信道不支持40+ */
            if ((mac_vap->st_channel.uc_chan_number >= 64 && mac_vap->st_channel.uc_chan_number < 100) ||
                (mac_vap->st_channel.uc_chan_number >= 144 && mac_vap->st_channel.uc_chan_number < 149) ||
                (mac_vap->st_channel.uc_chan_number >= 161 && mac_vap->st_channel.uc_chan_number < 184)) {
                bandwidth_change_to_20M = OAL_TRUE;
            }
            break;

        case WLAN_BAND_WIDTH_40MINUS:
            /* 1. 100 149 184信道不支持40- */
            if ((mac_vap->st_channel.uc_chan_number > 64 && mac_vap->st_channel.uc_chan_number <= 100) ||
                (mac_vap->st_channel.uc_chan_number > 144 && mac_vap->st_channel.uc_chan_number <= 149) ||
                (mac_vap->st_channel.uc_chan_number > 161 && mac_vap->st_channel.uc_chan_number <= 184)) {
                bandwidth_change_to_20M = OAL_TRUE;
            }
            break;

        case WLAN_BAND_WIDTH_80PLUSPLUS:
        case WLAN_BAND_WIDTH_80PLUSMINUS:
        case WLAN_BAND_WIDTH_80MINUSPLUS:
        case WLAN_BAND_WIDTH_80MINUSMINUS:
            /* 165信道不支持80M, 暂时不考虑出现更多信道异常问题 */
            if (mac_vap->st_channel.uc_chan_number == 165) {
                bandwidth_change_to_20M = OAL_TRUE;
            }
            break;

        /* 160M的带宽校验暂时不考虑 */
        default:
            break;
    }

    /* 需要降带宽 */
    if (bandwidth_change_to_20M == OAL_TRUE) {
        oam_warning_log2(mac_vap->uc_vap_id, OAM_SF_SCAN,
            "{hmac_sta_bandwidth_down_by_channel:: channel[%d] not support bandwidth[%d], need to change to 20M.}",
            mac_vap->st_channel.uc_chan_number, mac_vap->st_channel.en_bandwidth);

        mac_vap->st_channel.en_bandwidth = WLAN_BAND_WIDTH_20M;
    }
}


oal_bool_enum_uint8 hmac_is_rate_support(uint8_t *rates, uint8_t rate_num, uint8_t rate)
{
    oal_bool_enum_uint8 rate_is_supp = OAL_FALSE;
    uint8_t loop;

    if (rates == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_is_rate_support::rates null}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    for (loop = 0; loop < rate_num; loop++) {
        if ((rates[loop] & 0x7F) == rate) {
            rate_is_supp = OAL_TRUE;
            break;
        }
    }

    return rate_is_supp;
}


oal_bool_enum_uint8 hmac_is_support_11grate(uint8_t *rates, uint8_t rate_num)
{
    if (rates == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_is_rate_support::rates null}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    if ((OAL_TRUE == hmac_is_rate_support(rates, rate_num, 0x0C))
        || (OAL_TRUE == hmac_is_rate_support(rates, rate_num, 0x12))
        || (OAL_TRUE == hmac_is_rate_support(rates, rate_num, 0x18))
        || (OAL_TRUE == hmac_is_rate_support(rates, rate_num, 0x24))
        || (OAL_TRUE == hmac_is_rate_support(rates, rate_num, 0x30))
        || (OAL_TRUE == hmac_is_rate_support(rates, rate_num, 0x48))
        || (OAL_TRUE == hmac_is_rate_support(rates, rate_num, 0x60))
        || (OAL_TRUE == hmac_is_rate_support(rates, rate_num, 0x6C))) {
        return OAL_TRUE;
    }

    return OAL_FALSE;
}


oal_bool_enum_uint8 hmac_is_support_11brate(uint8_t *rates, uint8_t rate_num)
{
    if (rates == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_is_support_11brate::rates null}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    if ((OAL_TRUE == hmac_is_rate_support(rates, rate_num, 0x02))
        || (OAL_TRUE == hmac_is_rate_support(rates, rate_num, 0x04))
        || (OAL_TRUE == hmac_is_rate_support(rates, rate_num, 0x0B))
        || (OAL_TRUE == hmac_is_rate_support(rates, rate_num, 0x16))) {
        return OAL_TRUE;
    }

    return OAL_FALSE;
}
OAL_STATIC void hmac_sta_get_user_protocol_by_sup_rate(mac_bss_dscr_stru *bss_dscr,
    wlan_protocol_enum_uint8 *protocol_mode)
{
    if (OAL_TRUE == hmac_is_support_11grate(bss_dscr->auc_supp_rates,
                                                bss_dscr->uc_num_supp_rates)) {
        *protocol_mode = WLAN_LEGACY_11G_MODE;
        if (OAL_TRUE == hmac_is_support_11brate(bss_dscr->auc_supp_rates,
                                                    bss_dscr->uc_num_supp_rates)) {
            *protocol_mode = WLAN_MIXED_ONE_11G_MODE;
        }
    } else if (OAL_TRUE == hmac_is_support_11brate(bss_dscr->auc_supp_rates,
                                                       bss_dscr->uc_num_supp_rates)) {
        *protocol_mode = WLAN_LEGACY_11B_MODE;
    } else {
        oam_warning_log0(0, OAM_SF_ANY, "{hmac_sta_get_user_protocol::get user protocol failed.}");
    }
}

OAL_STATIC void hmac_sta_get_user_protocol(mac_bss_dscr_stru *bss_dscr,
    wlan_protocol_enum_uint8 *protocol_mode)
{
    if (g_wlan_spec_cfg->feature_11ax_is_open && (bss_dscr->en_he_capable == OAL_TRUE)) {
        *protocol_mode = WLAN_HE_MODE;
    } else if (bss_dscr->en_vht_capable == OAL_TRUE) {
        *protocol_mode = WLAN_VHT_MODE;
    } else if (bss_dscr->en_ht_capable == OAL_TRUE) {
        *protocol_mode = WLAN_HT_MODE;
    } else {
        if (bss_dscr->st_channel.en_band == WLAN_BAND_5G) { /* 判断是否是5G */
            *protocol_mode = WLAN_LEGACY_11A_MODE;
        } else {
            /* 嵌套深度优化封装 */
            hmac_sta_get_user_protocol_by_sup_rate(bss_dscr, protocol_mode);
        }
    }
}


uint32_t hmac_sta_get_user_protocol_etc(mac_bss_dscr_stru *bss_dscr,
    wlan_protocol_enum_uint8 *protocol_mode)
{
    /* 入参保护 */
    if (oal_any_null_ptr2(bss_dscr, protocol_mode)) {
        oam_error_log2(0, OAM_SF_SCAN, "{hmac_sta_get_user_protocol_etc::param null,%x %x.}",
                       (uintptr_t)bss_dscr, (uintptr_t)protocol_mode);
        return OAL_ERR_CODE_PTR_NULL;
    }

    hmac_sta_get_user_protocol(bss_dscr, protocol_mode);

    return OAL_SUCC;
}


OAL_STATIC uint32_t hmac_sta_send_auth_seq3(hmac_vap_stru *sta, uint8_t *mac_hdr)
{
    oal_netbuf_stru *auth_frame = OAL_PTR_NULL;
    hmac_user_stru *hmac_user_ap = OAL_PTR_NULL;
    mac_tx_ctl_stru *tx_ctl = OAL_PTR_NULL;

    uint16_t auth_frame_len;
    uint32_t ret;

    /* 准备seq = 3的认证帧 */
    auth_frame = oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF, WLAN_MEM_NETBUF_SIZE2, OAL_NETBUF_PRIORITY_MID);
    if (auth_frame == OAL_PTR_NULL) {
        /* TBD: 复位mac */
        oam_error_log0(sta->st_vap_base_info.uc_vap_id, OAM_SF_AUTH, "{hmac_wait_auth_sta::pst_auth_frame null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    oal_mem_netbuf_trace(auth_frame, OAL_TRUE);

    memset_s(oal_netbuf_cb(auth_frame), oal_netbuf_cb_size(), 0, oal_netbuf_cb_size());

    auth_frame_len = hmac_mgmt_encap_auth_req_seq3(sta, (uint8_t *)oal_netbuf_header(auth_frame), mac_hdr);
    oal_netbuf_put(auth_frame, auth_frame_len);

    hmac_user_ap = mac_res_get_hmac_user(sta->st_vap_base_info.us_assoc_vap_id);
    if (hmac_user_ap == OAL_PTR_NULL) {
        oal_netbuf_free(auth_frame);
        oam_error_log1(sta->st_vap_base_info.uc_vap_id, OAM_SF_AUTH,
                       "{hmac_wait_auth_sta::pst_hmac_user[%d] null.}",
                       sta->st_vap_base_info.us_assoc_vap_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 填写发送和发送完成需要的参数 */
    tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(auth_frame);
    MAC_GET_CB_MPDU_LEN(tx_ctl) = auth_frame_len;                                 /* 发送需要帧长度 */
    MAC_GET_CB_TX_USER_IDX(tx_ctl) = hmac_user_ap->st_user_base_info.us_assoc_id; /* 发送完成要获取用户 */

    /* 抛事件给dmac发送 */
    ret = hmac_tx_mgmt_send_event(&sta->st_vap_base_info, auth_frame, auth_frame_len);
    if (ret != OAL_SUCC) {
        oal_netbuf_free(auth_frame);
        oam_warning_log1(sta->st_vap_base_info.uc_vap_id, OAM_SF_AUTH,
                         "{hmac_wait_auth_sta::hmac_tx_mgmt_send_event failed[%d].}", ret);
        return ret;
    }

    frw_timer_immediate_destroy_timer_m(&sta->st_mgmt_timer);

    /* 更改状态为MAC_VAP_STATE_STA_WAIT_AUTH_SEQ4，并启动定时器 */
    hmac_fsm_change_state(sta, MAC_VAP_STATE_STA_WAIT_AUTH_SEQ4);

    frw_timer_create_timer_m(&sta->st_mgmt_timer,
                           hmac_mgmt_timeout_sta,
                           sta->st_mgmt_timer.ul_timeout,
                           &sta->st_mgmt_timetout_param,
                           OAL_FALSE,
                           OAM_MODULE_ID_HMAC,
                           sta->st_vap_base_info.ul_core_id);
    return OAL_SUCC;
}


uint32_t hmac_sta_wait_join_dtim_config(hmac_vap_stru *hmac_sta, hmac_join_req_stru *join_req)
{
    dmac_ctx_set_dtim_tsf_reg_stru *set_dtim_tsf_reg_params = OAL_PTR_NULL;
    frw_event_mem_stru *event_mem = OAL_PTR_NULL;
    frw_event_stru *event = OAL_PTR_NULL;
    uint32_t ret;

    /* 抛事件到DMAC, 申请事件内存 */
    event_mem = frw_event_alloc_m(OAL_SIZEOF(dmac_ctx_set_dtim_tsf_reg_stru));
    if (event_mem == OAL_PTR_NULL) {
        oam_error_log1(hmac_sta->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                       "{hmac_sta_wait_join_dtim_config::event_mem alloc null, size[%d].}",
                       OAL_SIZEOF(dmac_ctx_set_dtim_tsf_reg_stru));
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 填写事件 */
    event = frw_get_event_stru(event_mem);

    frw_event_hdr_init(&(event->st_event_hdr),
                       FRW_EVENT_TYPE_WLAN_CTX,
                       DMAC_WLAN_CTX_EVENT_SUB_TYPE_JOIN_DTIM_TSF_REG,
                       OAL_SIZEOF(dmac_ctx_set_dtim_tsf_reg_stru),
                       FRW_EVENT_PIPELINE_STAGE_1,
                       hmac_sta->st_vap_base_info.uc_chip_id,
                       hmac_sta->st_vap_base_info.uc_device_id,
                       hmac_sta->st_vap_base_info.uc_vap_id);

    set_dtim_tsf_reg_params = (dmac_ctx_set_dtim_tsf_reg_stru *)event->auc_event_data;

    /* 将Ap bssid和tsf REG 设置值保存在事件payload中 */
    set_dtim_tsf_reg_params->ul_dtim_cnt = join_req->st_bss_dscr.uc_dtim_cnt;
    set_dtim_tsf_reg_params->ul_dtim_period = join_req->st_bss_dscr.uc_dtim_period;
    set_dtim_tsf_reg_params->us_tsf_bit0 = BIT0;
    memcpy_s(set_dtim_tsf_reg_params->auc_bssid, WLAN_MAC_ADDR_LEN,
             hmac_sta->st_vap_base_info.auc_bssid, WLAN_MAC_ADDR_LEN);

    /* 分发事件 */
    ret = frw_event_dispatch_event(event_mem);
    frw_event_free_m(event_mem);
    return ret;
}

uint32_t hmac_sta_wait_join(hmac_vap_stru *hmac_sta, void *msg)
{
    hmac_join_req_stru *join_req = OAL_PTR_NULL;
    hmac_join_rsp_stru join_rsp;
    uint32_t ret;

    if (oal_any_null_ptr2(hmac_sta, msg)) {
        oam_error_log2(0, OAM_SF_SCAN, "{hmac_sta_wait_join::param null,%x %x.}",
                       (uintptr_t)hmac_sta, (uintptr_t)msg);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 1102 P2PSTA共存 todo 更新参数失败的话需要返回而不是继续下发Join动作 */
    ret = hmac_p2p_check_can_enter_state(&(hmac_sta->st_vap_base_info), HMAC_FSM_INPUT_ASOC_REQ);
    if (ret != OAL_SUCC) {
        /* 不能进入监听状态，返回设备忙 */
        oam_warning_log1(hmac_sta->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_sta_wait_join fail,device busy: ul_ret=%d}\r\n", ret);
        return OAL_ERR_CODE_CONFIG_BUSY;
    }

    join_req = (hmac_join_req_stru *)msg;
    /* 更新JOIN REG params 到MIB及MAC寄存器 */
    ret = hmac_sta_update_join_req_params(hmac_sta, join_req);
    if (ret != OAL_SUCC) {
        oam_error_log1(hmac_sta->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                       "{hmac_sta_wait_join::get hmac_sta_update_join_req_params fail[%d]!}", ret);
        return ret;
    }

    /* 非proxy sta模式时，需要将dtim参数配置到dmac */
    ret = hmac_sta_wait_join_dtim_config(hmac_sta, join_req);
    if (ret != OAL_SUCC) {
        oam_error_log1(hmac_sta->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                       "{hmac_sta_wait_join::get hmac_sta_wait_join_dtim_config fail[%d]!}", ret);
    return ret;
    }

    join_rsp.en_result_code = HMAC_MGMT_SUCCESS;
    /* 切换STA状态到JOIN_COMP */
    hmac_fsm_change_state(hmac_sta, MAC_VAP_STATE_STA_JOIN_COMP);

    /* 发送JOIN成功消息给SME hmac_handle_join_rsp_sta */
    hmac_send_rsp_to_sme_sta(hmac_sta, HMAC_SME_JOIN_RSP, (uint8_t *)&join_rsp);

    oam_warning_log4(hmac_sta->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                     "{hmac_sta_wait_join::Join AP channel=%d idx=%d, bandwidth=%d Beacon Period=%d SUCC.}",
                     join_req->st_bss_dscr.st_channel.uc_chan_number, join_req->st_bss_dscr.st_channel.uc_chan_idx,
                     hmac_sta->st_vap_base_info.st_channel.en_bandwidth, join_req->st_bss_dscr.us_beacon_period);

    return OAL_SUCC;
}


uint32_t hmac_sta_wait_join_rx(hmac_vap_stru *pst_hmac_sta, void *p_param)
{
    dmac_wlan_crx_event_stru *pst_mgmt_rx_event = OAL_PTR_NULL;
    dmac_rx_ctl_stru *pst_rx_ctrl = OAL_PTR_NULL;
    mac_rx_ctl_stru *pst_rx_info = OAL_PTR_NULL;
    uint8_t *puc_mac_hdr = OAL_PTR_NULL;
    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL;
    frw_event_stru *pst_event = OAL_PTR_NULL;
    dmac_ctx_set_dtim_tsf_reg_stru st_set_dtim_tsf_reg_params = { 0 };
    uint8_t *puc_tim_elm = OAL_PTR_NULL;
    uint16_t us_rx_len;

    uint8_t auc_bssid[6] = { 0 };
    uint8_t *puc_bssid = OAL_PTR_NULL;

    if (oal_any_null_ptr2(pst_hmac_sta, p_param)) {
        oam_error_log2(0, OAM_SF_SCAN, "{hmac_sta_wait_join_rx::param null,%x %x.}",
                       (uintptr_t)pst_hmac_sta, (uintptr_t)p_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_mgmt_rx_event = (dmac_wlan_crx_event_stru *)p_param;
    pst_rx_ctrl = (dmac_rx_ctl_stru *)oal_netbuf_cb(pst_mgmt_rx_event->pst_netbuf);
    pst_rx_info = (mac_rx_ctl_stru *)(&(pst_rx_ctrl->st_rx_info));
    puc_mac_hdr = (uint8_t *)MAC_GET_RX_CB_MAC_HEADER_ADDR(pst_rx_info);
    us_rx_len = pst_rx_ctrl->st_rx_info.us_frame_len; /* 消息总长度,不包括FCS */

    /* 在WAIT_JOIN状态下，处理接收到的beacon帧 */
    switch (mac_get_frame_type_and_subtype(puc_mac_hdr)) {
        case WLAN_FC0_SUBTYPE_BEACON | WLAN_FC0_TYPE_MGT: {
            /* 获取Beacon帧中的mac地址，即AP的mac地址 */
            mac_get_bssid(puc_mac_hdr, auc_bssid, sizeof(auc_bssid));

            /* 如果STA保存的AP mac地址与接收beacon帧的mac地址匹配，则更新beacon帧中的DTIM count值到STA本地mib库中 */
            puc_bssid = &auc_bssid[0];
            if (0 == oal_memcmp(puc_bssid, pst_hmac_sta->st_vap_base_info.auc_bssid, WLAN_MAC_ADDR_LEN)) {
                puc_tim_elm = mac_find_ie(MAC_EID_TIM, puc_mac_hdr + MAC_TAG_PARAM_OFFSET,
                                              us_rx_len - MAC_TAG_PARAM_OFFSET);
                /* 从tim IE中提取 DTIM count值,写入到MAC H/W REG中 */
                if ((puc_tim_elm != OAL_PTR_NULL) && (puc_tim_elm[1] >= MAC_MIN_TIM_LEN)) {
                    mac_mib_set_dot11dtimperiod(&pst_hmac_sta->st_vap_base_info, puc_tim_elm[3]);

                    /* 将dtim_cnt和dtim_period保存在事件payload中 */
                    st_set_dtim_tsf_reg_params.ul_dtim_cnt = puc_tim_elm[2];
                    st_set_dtim_tsf_reg_params.ul_dtim_period = puc_tim_elm[3];
                } else {
                    oam_warning_log0(pst_hmac_sta->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
                                     "{hmac_sta_wait_join::Do not find Tim ie.}");
                }

                /* 将Ap bssid和tsf REG 设置值保存在事件payload中 */
                memcpy_s(st_set_dtim_tsf_reg_params.auc_bssid, WLAN_MAC_ADDR_LEN, auc_bssid, WLAN_MAC_ADDR_LEN);
                st_set_dtim_tsf_reg_params.us_tsf_bit0 = BIT0;

                /* 抛事件到DMAC, 申请事件内存 */
                pst_event_mem = frw_event_alloc_m(OAL_SIZEOF(dmac_ctx_set_dtim_tsf_reg_stru));
                if (pst_event_mem == OAL_PTR_NULL) {
                    oam_error_log0(pst_hmac_sta->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
                                   "{hmac_sta_wait_join::pst_event_mem null.}");
                    return OAL_ERR_CODE_PTR_NULL;
                }

                /* 填写事件 */
                pst_event = frw_get_event_stru(pst_event_mem);

                frw_event_hdr_init(&(pst_event->st_event_hdr),
                                   FRW_EVENT_TYPE_WLAN_CTX,
                                   DMAC_WLAN_CTX_EVENT_SUB_TYPE_JOIN_DTIM_TSF_REG,
                                   OAL_SIZEOF(dmac_ctx_set_dtim_tsf_reg_stru),
                                   FRW_EVENT_PIPELINE_STAGE_1,
                                   pst_hmac_sta->st_vap_base_info.uc_chip_id,
                                   pst_hmac_sta->st_vap_base_info.uc_device_id,
                                   pst_hmac_sta->st_vap_base_info.uc_vap_id);

                /* 拷贝参数 */
                if (EOK != memcpy_s(frw_get_event_payload(pst_event_mem),
                                    OAL_SIZEOF(dmac_ctx_set_dtim_tsf_reg_stru),
                                    (uint8_t *)&st_set_dtim_tsf_reg_params,
                                    OAL_SIZEOF(dmac_ctx_set_dtim_tsf_reg_stru))) {
                    oam_error_log0(0, OAM_SF_SCAN, "hmac_sta_wait_join_rx::memcpy fail!");
                    frw_event_free_m(pst_event_mem);
                    return OAL_FAIL;
                }

                /* 分发事件 */
                frw_event_dispatch_event(pst_event_mem);
                frw_event_free_m(pst_event_mem);
            }
        }
        break;

        case WLAN_FC0_SUBTYPE_ACTION | WLAN_FC0_TYPE_MGT: {
            /* do nothing  */
        }
        break;

        default:
        {
            /* do nothing */
        }
        break;
    }

    return OAL_SUCC;
}


uint32_t hmac_sta_wait_join_timeout(hmac_vap_stru *pst_hmac_sta, void *pst_msg)
{
    hmac_join_rsp_stru st_join_rsp = { 0 };

    if (oal_any_null_ptr2(pst_hmac_sta, pst_msg)) {
        oam_error_log2(0, OAM_SF_SCAN, "{hmac_sta_wait_join_timeout::param null,%x %x.}",
                       (uintptr_t)pst_hmac_sta, (uintptr_t)pst_msg);
        return OAL_ERR_CODE_PTR_NULL;
    }

    oam_error_log0(pst_hmac_sta->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
                   "{hmac_sta_wait_join_timeout::join timeout.}");
    /* 进入timeout处理函数表示join没有成功，把join的结果设置为timeout上报给sme */
    st_join_rsp.en_result_code = HMAC_MGMT_TIMEOUT;

    /* 将hmac状态机切换为MAC_VAP_STATE_STA_FAKE_UP */
    hmac_fsm_change_state(pst_hmac_sta, MAC_VAP_STATE_STA_FAKE_UP);

    /* 发送超时消息给SME */
    hmac_send_rsp_to_sme_sta(pst_hmac_sta, HMAC_SME_JOIN_RSP, (uint8_t *)&st_join_rsp);

    return OAL_SUCC;
}


uint32_t hmac_sta_wait_join_misc(hmac_vap_stru *pst_hmac_sta, void *pst_msg)
{
    hmac_join_rsp_stru st_join_rsp;
    hmac_misc_input_stru *st_misc_input = (hmac_misc_input_stru *)pst_msg;

    if (oal_any_null_ptr2(pst_hmac_sta, pst_msg)) {
        oam_error_log2(0, OAM_SF_SCAN, "{hmac_sta_wait_join_misc::param null,%x %x.}",
                       (uintptr_t)pst_hmac_sta, (uintptr_t)pst_msg);
        return OAL_ERR_CODE_PTR_NULL;
    }

    oam_info_log0(pst_hmac_sta->st_vap_base_info.uc_vap_id, OAM_SF_SCAN, "{hmac_sta_wait_join_misc::enter func.}");
    switch (st_misc_input->en_type) {
        /* 处理TBTT中断  */
        case HMAC_MISC_TBTT: {
            /* 接收到TBTT中断，意味着JOIN成功了，所以取消JOIN开始时设置的定时器,发消息通知SME  */
            frw_timer_immediate_destroy_timer_m(&pst_hmac_sta->st_mgmt_timer);

            st_join_rsp.en_result_code = HMAC_MGMT_SUCCESS;

            oam_info_log0(pst_hmac_sta->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
                          "{hmac_sta_wait_join_misc::join succ.}");
            /* 切换STA状态到JOIN_COMP */
            hmac_fsm_change_state(pst_hmac_sta, MAC_VAP_STATE_STA_JOIN_COMP);

            /* 发送JOIN成功消息给SME */
            hmac_send_rsp_to_sme_sta(pst_hmac_sta, HMAC_SME_JOIN_RSP, (uint8_t *)&st_join_rsp);
        }
        break;

        default:
        {
            /* Do Nothing */
        }
        break;
    }
    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_SAE

uint32_t hmac_sta_sae_connect_info_wpas(hmac_vap_stru *hmac_sta, hmac_auth_req_stru *auth_req)
{
    hmac_user_stru *hmac_user_ap = OAL_PTR_NULL;
    uint16_t us_user_index = g_wlan_spec_cfg->invalid_user_id;
    uint32_t ret = OAL_SUCC;

    oam_warning_log0(hmac_sta->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                     "{hmac_sta_sae_connect_info_wpas:: report external auth to wpa_s.}");

    /* 给STA 添加用户 */
    hmac_user_ap = (hmac_user_stru *)mac_res_get_hmac_user(hmac_sta->st_vap_base_info.us_assoc_vap_id);
    if (hmac_user_ap == OAL_PTR_NULL) {
        ret = hmac_user_add(&(hmac_sta->st_vap_base_info), hmac_sta->st_vap_base_info.auc_bssid, &us_user_index);
        if (ret != OAL_SUCC) {
            oam_warning_log1(hmac_sta->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                             "{hmac_sta_sae_connect_info_wpas:: add sae user failed[%d].}", ret);
            return OAL_FAIL;
        }
    }

    oal_workqueue_schedule(&(hmac_sta->st_sae_report_ext_auth_worker));

    /* 切换STA 到MAC_VAP_STATE_STA_WAIT_AUTH_SEQ2 */
    hmac_fsm_change_state(hmac_sta, MAC_VAP_STATE_STA_WAIT_AUTH_SEQ2);

    /* 启动认证超时定时器 */
    hmac_sta->st_mgmt_timetout_param.en_state = MAC_VAP_STATE_STA_WAIT_AUTH_SEQ2;
    hmac_sta->st_mgmt_timetout_param.uc_vap_id = hmac_sta->st_vap_base_info.uc_vap_id;
    hmac_sta->st_mgmt_timetout_param.us_user_index = us_user_index;
    frw_timer_create_timer_m(&hmac_sta->st_mgmt_timer,
                           hmac_mgmt_timeout_sta,
                           auth_req->us_timeout,
                           &hmac_sta->st_mgmt_timetout_param,
                           OAL_FALSE,
                           OAM_MODULE_ID_HMAC,
                           hmac_sta->st_vap_base_info.ul_core_id);
    return OAL_SUCC;
}
#endif


uint32_t hmac_sta_assemble_auth_seq1(hmac_vap_stru *hmac_sta, hmac_auth_req_stru *auth_req, oal_netbuf_stru *auth_frame)
{
    hmac_user_stru *hmac_user_ap = OAL_PTR_NULL;
    mac_tx_ctl_stru *tx_ctl = OAL_PTR_NULL;
    uint16_t auth_len;
    uint32_t ret;

    /* 组seq = 1 的认证请求帧 */
    auth_len = hmac_mgmt_encap_auth_req(hmac_sta, (uint8_t *)(oal_netbuf_header(auth_frame)));

    oal_netbuf_put(auth_frame, auth_len);
    hmac_user_ap = mac_res_get_hmac_user(hmac_sta->st_vap_base_info.us_assoc_vap_id);
    if (hmac_user_ap == OAL_PTR_NULL) {
        oal_netbuf_free(auth_frame);
        oam_error_log1(hmac_sta->st_vap_base_info.uc_vap_id, OAM_SF_AUTH,
            "{hmac_sta_assemble_auth_frame::pst_hmac_user[%d] null.}", hmac_sta->st_vap_base_info.us_assoc_vap_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 为填写发送描述符准备参数 */
    tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(auth_frame); /* 获取cb结构体 */
    MAC_GET_CB_MPDU_LEN(tx_ctl) = auth_len;                 /* dmac发送需要的mpdu长度 */
    /* 发送完成需要获取user结构体 */
    MAC_GET_CB_TX_USER_IDX(tx_ctl) = hmac_user_ap->st_user_base_info.us_assoc_id;

    /* 如果是WEP，需要将ap的mac地址写入lut */
    ret = hmac_init_security(&(hmac_sta->st_vap_base_info), hmac_user_ap->st_user_base_info.auc_user_mac_addr,
                             sizeof(hmac_user_ap->st_user_base_info.auc_user_mac_addr));

    if (ret != OAL_SUCC) {
        oam_error_log1(hmac_sta->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                       "{hmac_sta_assemble_auth_frame::hmac_init_security failed[%d].}", ret);
    }

    /* 抛事件让dmac将该帧发送 */
    ret = hmac_tx_mgmt_send_event(&hmac_sta->st_vap_base_info, auth_frame, auth_len);
    if (ret != OAL_SUCC) {
        oal_netbuf_free(auth_frame);
        oam_warning_log1(hmac_sta->st_vap_base_info.uc_vap_id, OAM_SF_AUTH,
                         "{hmac_sta_assemble_auth_frame::hmac_tx_mgmt_send_event failed[%d].}", ret);
        return ret;
    }

    /* 更改状态 */
    hmac_fsm_change_state(hmac_sta, MAC_VAP_STATE_STA_WAIT_AUTH_SEQ2);

    /* 启动认证超时定时器 */
    hmac_sta->st_mgmt_timetout_param.en_state = MAC_VAP_STATE_STA_WAIT_AUTH_SEQ2;
    hmac_sta->st_mgmt_timetout_param.us_user_index = hmac_user_ap->st_user_base_info.us_assoc_id;
    hmac_sta->st_mgmt_timetout_param.uc_vap_id = hmac_sta->st_vap_base_info.uc_vap_id;
    frw_timer_create_timer_m(&hmac_sta->st_mgmt_timer, hmac_mgmt_timeout_sta, auth_req->us_timeout,
        &hmac_sta->st_mgmt_timetout_param, OAL_FALSE, OAM_MODULE_ID_HMAC, hmac_sta->st_vap_base_info.ul_core_id);

    return OAL_SUCC;
}


uint32_t hmac_sta_wait_auth(hmac_vap_stru *hmac_sta, void *msg)
{
    hmac_auth_req_stru *auth_req = OAL_PTR_NULL;
    oal_netbuf_stru *auth_frame = OAL_PTR_NULL;
    uint32_t ret;

    if (oal_any_null_ptr2(hmac_sta, msg)) {
        oam_error_log2(0, OAM_SF_AUTH, "{hmac_sta_wait_auth::param null,%x %x.}", (uintptr_t)hmac_sta, (uintptr_t)msg);
        return OAL_ERR_CODE_PTR_NULL;
    }

    auth_req = (hmac_auth_req_stru *)msg;

#ifdef _PRE_WLAN_FEATURE_SAE
    if (mac_mib_get_AuthenticationMode(&(hmac_sta->st_vap_base_info)) == WLAN_WITP_AUTH_SAE
        && (hmac_sta->bit_sae_connect_with_pmkid == OAL_FALSE)) {
        ret = hmac_sta_sae_connect_info_wpas(hmac_sta, auth_req);
        return ret;
    }
#endif

    /* 申请认证帧空间 */
    auth_frame = oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF, WLAN_MEM_NETBUF_SIZE2, OAL_NETBUF_PRIORITY_MID);

    if (auth_frame == OAL_PTR_NULL) {
        oam_error_log0(hmac_sta->st_vap_base_info.uc_vap_id, OAM_SF_AUTH, "{hmac_wait_auth_sta::puc_auth_frame null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    oal_mem_netbuf_trace(auth_frame, OAL_TRUE);

    memset_s(oal_netbuf_cb(auth_frame), oal_netbuf_cb_size(), 0, oal_netbuf_cb_size());

    memset_s((uint8_t *)oal_netbuf_header(auth_frame), MAC_80211_FRAME_LEN, 0, MAC_80211_FRAME_LEN);

    /* 组seq = 1 的认证请求帧 */
    ret = hmac_sta_assemble_auth_seq1(hmac_sta, auth_req, auth_frame);
    if (ret != OAL_SUCC) {
        return ret;
    }

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_SAE

OAL_STATIC uint32_t hmac_sta_process_sae_commit(hmac_vap_stru *pst_sta, oal_netbuf_stru *pst_netbuf)
{
    hmac_user_stru *pst_hmac_user_ap = OAL_PTR_NULL;
    mac_rx_ctl_stru *pst_rx_ctrl = OAL_PTR_NULL; /* 每一个MPDU的控制信息 */
    uint8_t *puc_mac_hdr = OAL_PTR_NULL;
    uint16_t us_status;
    hmac_auth_rsp_stru st_auth_rsp = {{0}, };

    pst_hmac_user_ap = mac_res_get_hmac_user(pst_sta->st_vap_base_info.us_assoc_vap_id);
    if (pst_hmac_user_ap == OAL_PTR_NULL) {
        oam_error_log1(pst_sta->st_vap_base_info.uc_vap_id, OAM_SF_AUTH,
                       "{hmac_sta_wait_auth_seq2_rx::pst_hmac_user[%d] null.}",
                       pst_sta->st_vap_base_info.us_assoc_vap_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb(pst_netbuf); /* 每一个MPDU的控制信息 */
    puc_mac_hdr = (uint8_t *)MAC_GET_RX_CB_MAC_HEADER_ADDR(pst_rx_ctrl);

    oam_warning_log2(pst_sta->st_vap_base_info.uc_vap_id, OAM_SF_SAE,
                     "{hmac_sta_wait_auth_seq2_rx::rx sae auth frame, status_code %d, seq_num %d.}",
                     mac_get_auth_status(puc_mac_hdr),
                     mac_get_auth_seq_num(puc_mac_hdr));

    us_status = mac_get_auth_status(puc_mac_hdr);
    if (us_status == MAC_AP_FULL) {
        oam_warning_log1(pst_sta->st_vap_base_info.uc_vap_id, OAM_SF_AUTH,
                         "{hmac_sta_wait_auth_seq2_rx::AP refuse STA auth reason[%d]!}",
                         us_status);

        /* 取消定时器 */
        frw_timer_immediate_destroy_timer_m(&pst_sta->st_mgmt_timer);

        st_auth_rsp.us_status_code = us_status;

        /* 上报给SME认证结果 */
        hmac_send_rsp_to_sme_sta(pst_sta, HMAC_SME_AUTH_RSP, (uint8_t *)&st_auth_rsp);

        return OAL_SUCC;
    }

    /* SAE commit帧的seq number是1，confirm帧的seq number是2 */
    if (WLAN_AUTH_TRASACTION_NUM_ONE != mac_get_auth_seq_num(puc_mac_hdr)) {
        oam_warning_log0(pst_sta->st_vap_base_info.uc_vap_id, OAM_SF_SAE,
            "{hmac_sta_wait_auth_seq2_rx::drop sae auth frame for wrong seq num}");
        return OAL_SUCC;
    }

    pst_sta->duplicate_auth_seq4_flag = OAL_FALSE; // seq4置位

    /* 取消定时器 */
    frw_timer_immediate_destroy_timer_m(&pst_sta->st_mgmt_timer);

    /* 切换STA 到MAC_VAP_STATE_STA_WAIT_AUTH_SEQ4 */
    hmac_fsm_change_state(pst_sta, MAC_VAP_STATE_STA_WAIT_AUTH_SEQ4);

    /* SAE 判断seq number以后，上传给wpas 处理 */
    hmac_rx_mgmt_send_to_host(pst_sta, pst_netbuf);

    /* 启动认证超时定时器 */
    pst_sta->st_mgmt_timetout_param.en_state = MAC_VAP_STATE_STA_WAIT_AUTH_SEQ4;
    pst_sta->st_mgmt_timetout_param.uc_vap_id = pst_sta->st_vap_base_info.uc_vap_id;
    pst_sta->st_mgmt_timetout_param.us_user_index = pst_hmac_user_ap->st_user_base_info.us_assoc_id;
    frw_timer_create_timer_m(&pst_sta->st_mgmt_timer, hmac_mgmt_timeout_sta, pst_sta->st_mgmt_timer.ul_timeout,
        &pst_sta->st_mgmt_timetout_param, OAL_FALSE, OAM_MODULE_ID_HMAC, pst_sta->st_vap_base_info.ul_core_id);

    return OAL_SUCC;
}
#endif


uint32_t hmac_sta_wait_auth_seq2_rx(hmac_vap_stru *pst_sta, void *pst_msg)
{
    dmac_wlan_crx_event_stru *pst_crx_event = OAL_PTR_NULL;
    mac_rx_ctl_stru *pst_rx_ctrl = OAL_PTR_NULL; /* 每一个MPDU的控制信息 */
    uint8_t *puc_mac_hdr = OAL_PTR_NULL;
    uint16_t us_auth_alg = 0;
    hmac_auth_rsp_stru st_auth_rsp = {
        { 0 },
    };

    if (oal_any_null_ptr2(pst_sta, pst_msg)) {
        oam_error_log2(0, OAM_SF_AUTH, "{hmac_sta_wait_auth_seq2_rx::param null,%x %x.}",
                       (uintptr_t)pst_sta, (uintptr_t)pst_msg);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_crx_event = (dmac_wlan_crx_event_stru *)pst_msg;
    pst_rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb(pst_crx_event->pst_netbuf); /* 每一个MPDU的控制信息 */
    puc_mac_hdr = (uint8_t *)MAC_GET_RX_CB_MAC_HEADER_ADDR(pst_rx_ctrl);

#ifdef _PRE_WLAN_WAKEUP_SRC_PARSE
    if (OAL_TRUE == wlan_pm_wkup_src_debug_get()) {
        wlan_pm_wkup_src_debug_set(OAL_FALSE);
        oam_warning_log1(pst_sta->st_vap_base_info.uc_vap_id, OAM_SF_RX,
                         "{wifi_wake_src:hmac_sta_wait_auth_seq2_rx::wakeup mgmt type[0x%x]}",
                         mac_get_frame_type_and_subtype(puc_mac_hdr));
    }
#endif

    if ((WLAN_FC0_SUBTYPE_AUTH | WLAN_FC0_TYPE_MGT) != mac_get_frame_type_and_subtype(puc_mac_hdr)) {
        return OAL_SUCC;
    }

    us_auth_alg = mac_get_auth_alg(puc_mac_hdr);

#ifdef _PRE_WLAN_FEATURE_SAE
    /* 注意:mib 值中填写的auth_alg 值来自内核，和ieee定义的auth_alg取值不同 */
    if ((us_auth_alg == WLAN_MIB_AUTH_ALG_SAE) && (pst_sta->bit_sae_connect_with_pmkid == OAL_FALSE)) {
        return hmac_sta_process_sae_commit(pst_sta, pst_crx_event->pst_netbuf);
    }
#endif

    if (WLAN_AUTH_TRASACTION_NUM_TWO != mac_get_auth_seq_num(puc_mac_hdr)) {
        return OAL_SUCC;
    }

#ifdef _PRE_WLAN_FEATURE_SNIFFER
#ifdef CONFIG_HW_SNIFFER
    proc_sniffer_write_file(NULL, 0, puc_mac_hdr, pst_rx_ctrl->us_frame_len, 0);
#endif
#endif
    /* AUTH alg CHECK */
    if ((mac_mib_get_AuthenticationMode(&pst_sta->st_vap_base_info) != us_auth_alg)
        && (WLAN_WITP_AUTH_AUTOMATIC != mac_mib_get_AuthenticationMode(&pst_sta->st_vap_base_info))) {
        oam_warning_log2(pst_sta->st_vap_base_info.uc_vap_id, OAM_SF_AUTH,
                         "{hmac_sta_wait_auth_seq2_rx::rcv unexpected auth alg[%d/%d].}",
                         us_auth_alg, mac_mib_get_AuthenticationMode(&pst_sta->st_vap_base_info));
    }
    st_auth_rsp.us_status_code = mac_get_auth_status(puc_mac_hdr);
    if (st_auth_rsp.us_status_code != MAC_SUCCESSFUL_STATUSCODE) {
        oam_warning_log1(pst_sta->st_vap_base_info.uc_vap_id, OAM_SF_AUTH,
            "{hmac_sta_wait_auth_seq2_rx::AP refuse STA auth reason[%d]!}", st_auth_rsp.us_status_code);

        frw_timer_immediate_destroy_timer_m(&pst_sta->st_mgmt_timer);

        /* 上报给SME认证结果 */
        hmac_send_rsp_to_sme_sta(pst_sta, HMAC_SME_AUTH_RSP, (uint8_t *)&st_auth_rsp);

        if (st_auth_rsp.us_status_code != MAC_AP_FULL) {
            chr_exception(chr_wifi_drv(CHR_WIFI_DRV_EVENT_CONNECT, CHR_WIFI_DRV_ERROR_AUTH_REJECTED));
        }
        return OAL_SUCC;
    }

    /* auth response status_code 成功处理 */
    if (us_auth_alg == WLAN_WITP_AUTH_OPEN_SYSTEM) {
        frw_timer_immediate_destroy_timer_m(&pst_sta->st_mgmt_timer);

        /* 将状态更改为AUTH_COMP */
        hmac_fsm_change_state(pst_sta, MAC_VAP_STATE_STA_AUTH_COMP);
        st_auth_rsp.us_status_code = HMAC_MGMT_SUCCESS;

        /* 上报给SME认证成功 */
        hmac_send_rsp_to_sme_sta(pst_sta, HMAC_SME_AUTH_RSP, (uint8_t *)&st_auth_rsp);
        return OAL_SUCC;
    } else if (us_auth_alg == WLAN_WITP_AUTH_SHARED_KEY) {
        return hmac_sta_send_auth_seq3(pst_sta, puc_mac_hdr);
    } else {
        frw_timer_immediate_destroy_timer_m(&pst_sta->st_mgmt_timer);

        /* 接收到AP 回复的auth response 中支持认证算法当前不支持的情况下，status code 却是SUCC,
            认为认证成功，并且继续出发关联 */
        oam_warning_log1(pst_sta->st_vap_base_info.uc_vap_id, OAM_SF_AUTH,
                         "{hmac_sta_wait_auth_seq2_rx::AP's auth_alg [%d] not support!}", us_auth_alg);

        /* 将状态更改为AUTH_COMP */
        hmac_fsm_change_state(pst_sta, MAC_VAP_STATE_STA_AUTH_COMP);
        st_auth_rsp.us_status_code = HMAC_MGMT_SUCCESS;

        /* 上报给SME认证成功 */
        hmac_send_rsp_to_sme_sta(pst_sta, HMAC_SME_AUTH_RSP, (uint8_t *)&st_auth_rsp);
    }

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_SAE

OAL_STATIC uint32_t hmac_sta_process_sae_confirm(hmac_vap_stru *pst_sta, oal_netbuf_stru *pst_netbuf)
{
    mac_rx_ctl_stru *pst_rx_ctrl = OAL_PTR_NULL; /* 每一个MPDU的控制信息 */
    uint8_t *puc_mac_hdr = OAL_PTR_NULL;

    pst_rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb(pst_netbuf); /* 每一个MPDU的控制信息 */
    puc_mac_hdr = (uint8_t *)MAC_GET_RX_CB_MAC_HEADER_ADDR(pst_rx_ctrl);

    /* SAE commit帧的seq number是1，confirm帧的seq number是2 */
    if (WLAN_AUTH_TRASACTION_NUM_TWO != mac_get_auth_seq_num(puc_mac_hdr)) {
        oam_warning_log2(pst_sta->st_vap_base_info.uc_vap_id, OAM_SF_SAE,
            "{hmac_sta_wait_auth_seq4_rx::drop sae auth frame, status_code %d, seq_num %d.}",
            mac_get_auth_status(puc_mac_hdr),
            mac_get_auth_seq_num(puc_mac_hdr));
        return OAL_SUCC;
    }

    if (pst_sta->duplicate_auth_seq4_flag == OAL_TRUE) { // wpa3 auth seq4重复帧过滤，禁止上报waps
        oam_warning_log2(pst_sta->st_vap_base_info.uc_vap_id, OAM_SF_SAE,
            "{hmac_sta_wait_auth_seq4_rx::drop sae auth frame, status_code %d, seq_num %d.}",
            mac_get_auth_status(puc_mac_hdr), mac_get_auth_seq_num(puc_mac_hdr));
        return OAL_SUCC;
    }

    /* 取消定时器 */
    frw_timer_immediate_destroy_timer_m(&pst_sta->st_mgmt_timer);

    /* SAE 判断seq number以后，上传给wpas 处理 */
    hmac_rx_mgmt_send_to_host(pst_sta, pst_netbuf);

    oam_warning_log2(pst_sta->st_vap_base_info.uc_vap_id, OAM_SF_SAE,
                     "{hmac_sta_wait_auth_seq4_rx::rx sae auth frame, status_code %d, seq_num %d.}",
                     mac_get_auth_status(puc_mac_hdr),
                     mac_get_auth_seq_num(puc_mac_hdr));
    pst_sta->duplicate_auth_seq4_flag = OAL_TRUE;
    return OAL_SUCC;
}
#endif


uint32_t hmac_sta_wait_auth_seq4_rx(hmac_vap_stru *pst_sta, void *p_msg)
{
    dmac_wlan_crx_event_stru *pst_crx_event = OAL_PTR_NULL;
    mac_rx_ctl_stru *pst_rx_ctrl = OAL_PTR_NULL; /* 每一个MPDU的控制信息 */
    uint8_t *puc_mac_hdr = OAL_PTR_NULL;
    hmac_auth_rsp_stru st_auth_rsp = {
        { 0 },
    };

    if (oal_any_null_ptr2(pst_sta, p_msg)) {
        oam_error_log2(0, OAM_SF_AUTH, "{hmac_sta_wait_auth_seq4_rx::param null,%x %x.}",
                       (uintptr_t)p_msg, (uintptr_t)p_msg);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_crx_event = (dmac_wlan_crx_event_stru *)p_msg;
    pst_rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb(pst_crx_event->pst_netbuf); /* 每一个MPDU的控制信息 */
    puc_mac_hdr = (uint8_t *)MAC_GET_RX_CB_MAC_HEADER_ADDR(pst_rx_ctrl);

#ifdef _PRE_WLAN_WAKEUP_SRC_PARSE
    if (OAL_TRUE == wlan_pm_wkup_src_debug_get()) {
        wlan_pm_wkup_src_debug_set(OAL_FALSE);
        oam_warning_log1(pst_sta->st_vap_base_info.uc_vap_id, OAM_SF_RX,
                         "{wifi_wake_src:hmac_sta_wait_auth_seq4_rx::wakeup mgmt type[0x%x]}",
                         mac_get_frame_type_and_subtype(puc_mac_hdr));
    }
#endif

    if ((WLAN_FC0_SUBTYPE_AUTH | WLAN_FC0_TYPE_MGT) != mac_get_frame_type_and_subtype(puc_mac_hdr)) {
        return OAL_SUCC;
    }

#ifdef _PRE_WLAN_FEATURE_SAE
    /* 注意:mib 值中填写的auth_alg 值来自内核，和ieee定义的auth_alg取值不同 */
    if ((mac_get_auth_alg(puc_mac_hdr) == WLAN_MIB_AUTH_ALG_SAE)
        && (pst_sta->bit_sae_connect_with_pmkid == OAL_FALSE)) {
        return hmac_sta_process_sae_confirm(pst_sta, pst_crx_event->pst_netbuf);
    }
#endif

#ifdef _PRE_WLAN_FEATURE_SNIFFER
#ifdef CONFIG_HW_SNIFFER
        proc_sniffer_write_file(NULL, 0, puc_mac_hdr, pst_rx_ctrl->us_frame_len, 0);
#endif
#endif
        if ((WLAN_AUTH_TRASACTION_NUM_FOUR == mac_get_auth_seq_num(puc_mac_hdr)) &&
            (MAC_SUCCESSFUL_STATUSCODE == mac_get_auth_status(puc_mac_hdr))) {
            /* 接收到seq = 4 且状态位为succ 取消定时器 */
            frw_timer_immediate_destroy_timer_m(&pst_sta->st_mgmt_timer);

            st_auth_rsp.us_status_code = HMAC_MGMT_SUCCESS;

            /* 更改sta状态为MAC_VAP_STATE_STA_AUTH_COMP */
            hmac_fsm_change_state(pst_sta, MAC_VAP_STATE_STA_AUTH_COMP);

            /* 将认证结果上报SME */
            hmac_send_rsp_to_sme_sta(pst_sta, HMAC_SME_AUTH_RSP, (uint8_t *)&st_auth_rsp);
        } else {
            oam_warning_log1(pst_sta->st_vap_base_info.uc_vap_id, OAM_SF_AUTH,
                "{hmac_sta_wait_auth_seq4_rx::transaction num.status[%d]}", mac_get_auth_status(puc_mac_hdr));
            /* 等待定时器超时 */
        }

    return OAL_SUCC;
}


uint32_t hmac_sta_wait_asoc(hmac_vap_stru *pst_sta, void *pst_msg)
{
    hmac_asoc_req_stru *pst_hmac_asoc_req = OAL_PTR_NULL;
    oal_netbuf_stru *pst_asoc_req_frame = OAL_PTR_NULL;
    mac_tx_ctl_stru *pst_tx_ctl = OAL_PTR_NULL;
    hmac_user_stru *pst_hmac_user_ap = OAL_PTR_NULL;
    uint32_t ul_asoc_frame_len;
    uint32_t ul_ret;
    uint8_t *puc_curr_bssid = OAL_PTR_NULL;

    if (oal_any_null_ptr2(pst_sta, pst_msg)) {
        oam_error_log2(0, OAM_SF_ASSOC, "{hmac_sta_wait_asoc::param null,%x %x.}",
                       (uintptr_t)pst_sta, (uintptr_t)pst_msg);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hmac_asoc_req = (hmac_asoc_req_stru *)pst_msg;

    pst_asoc_req_frame = oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF, WLAN_MEM_NETBUF_SIZE2, OAL_NETBUF_PRIORITY_MID);

    if (pst_asoc_req_frame == OAL_PTR_NULL) {
        oam_error_log0(pst_sta->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                       "{hmac_sta_wait_asoc::pst_asoc_req_frame null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    oal_mem_netbuf_trace(pst_asoc_req_frame, OAL_TRUE);

    memset_s(oal_netbuf_cb(pst_asoc_req_frame), oal_netbuf_cb_size(), 0, oal_netbuf_cb_size());

    /* 将mac header清零 */
    memset_s((uint8_t *)oal_netbuf_header(pst_asoc_req_frame), MAC_80211_FRAME_LEN, 0, MAC_80211_FRAME_LEN);

    /* 组帧 (Re)Assoc_req_Frame */
    if (pst_sta->bit_reassoc_flag) {
        puc_curr_bssid = pst_sta->st_vap_base_info.auc_bssid;
    } else {
        puc_curr_bssid = OAL_PTR_NULL;
    }
    ul_asoc_frame_len = hmac_mgmt_encap_asoc_req_sta(pst_sta, (uint8_t *)(oal_netbuf_header(pst_asoc_req_frame)),
                                                         puc_curr_bssid);
    oal_netbuf_put(pst_asoc_req_frame, ul_asoc_frame_len);

    if (ul_asoc_frame_len == 0) {
        oam_warning_log0(pst_sta->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_sta_wait_asoc::hmac_mgmt_encap_asoc_req_sta null.}");
        oal_netbuf_free(pst_asoc_req_frame);

        return OAL_FAIL;
    }

    if (oal_unlikely(ul_asoc_frame_len < OAL_ASSOC_REQ_IE_OFFSET)) {
        oam_error_log1(pst_sta->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                       "{hmac_sta_wait_asoc::invalid ul_asoc_req_ie_len[%u].}",
                       ul_asoc_frame_len);
        oam_report_dft_params(BROADCAST_MACADDR, (uint8_t *)oal_netbuf_header(pst_asoc_req_frame),
                                  (uint16_t)ul_asoc_frame_len, OAM_OTA_TYPE_80211_FRAME);
        oal_netbuf_free(pst_asoc_req_frame);
        return OAL_FAIL;
    }
    /* Should we change the ie buff from local mem to netbuf ? */
    /* 此处申请的内存，只在上报给内核后释放 */
    pst_hmac_user_ap = (hmac_user_stru *)mac_res_get_hmac_user(pst_sta->st_vap_base_info.us_assoc_vap_id);
    if (pst_hmac_user_ap == OAL_PTR_NULL) {
        oam_error_log0(pst_sta->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                       "{hmac_sta_wait_asoc::pst_hmac_user_ap null.}");
        oal_netbuf_free(pst_asoc_req_frame);
        return OAL_ERR_CODE_PTR_NULL;
    }

    hmac_user_free_asoc_req_ie(pst_sta->st_vap_base_info.us_assoc_vap_id);
    ul_ret = hmac_user_set_asoc_req_ie(pst_hmac_user_ap,
                                       oal_netbuf_header(pst_asoc_req_frame) + OAL_ASSOC_REQ_IE_OFFSET,
                                       ul_asoc_frame_len - OAL_ASSOC_REQ_IE_OFFSET,
                                       (uint8_t)(pst_sta->bit_reassoc_flag));
    if (ul_ret != OAL_SUCC) {
        oam_error_log0(pst_sta->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                       "{hmac_sta_wait_asoc::hmac_user_set_asoc_req_ie failed}");
        oal_netbuf_free(pst_asoc_req_frame);
        return OAL_FAIL;
    }

    pst_tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_asoc_req_frame);

    MAC_GET_CB_MPDU_LEN(pst_tx_ctl) = (uint16_t)ul_asoc_frame_len;
    MAC_GET_CB_TX_USER_IDX(pst_tx_ctl) = pst_hmac_user_ap->st_user_base_info.us_assoc_id;

    /* 抛事件让DMAC将该帧发送 */
    ul_ret = hmac_tx_mgmt_send_event(&(pst_sta->st_vap_base_info),
                                         pst_asoc_req_frame,
                                         (uint16_t)ul_asoc_frame_len);
    if (ul_ret != OAL_SUCC) {
        oal_netbuf_free(pst_asoc_req_frame);
        hmac_user_free_asoc_req_ie(pst_sta->st_vap_base_info.us_assoc_vap_id);
        oam_warning_log1(pst_sta->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_sta_wait_asoc::hmac_tx_mgmt_send_event failed[%d].}", ul_ret);
        return ul_ret;
    }

    /* 更改状态 */
    hmac_fsm_change_state(pst_sta, MAC_VAP_STATE_STA_WAIT_ASOC);

    /* 启动关联超时定时器, 为对端ap分配一个定时器，如果超时ap没回asoc rsp则启动超时处理 */
    pst_sta->st_mgmt_timetout_param.en_state = MAC_VAP_STATE_STA_WAIT_ASOC;
    pst_sta->st_mgmt_timetout_param.us_user_index = pst_hmac_user_ap->st_user_base_info.us_assoc_id;
    pst_sta->st_mgmt_timetout_param.uc_vap_id = pst_sta->st_vap_base_info.uc_vap_id;
    pst_sta->st_mgmt_timetout_param.en_status_code = MAC_ASOC_RSP_TIMEOUT;

    frw_timer_create_timer_m(&(pst_sta->st_mgmt_timer),
                           hmac_mgmt_timeout_sta,
                           pst_hmac_asoc_req->us_assoc_timeout,
                           &(pst_sta->st_mgmt_timetout_param),
                           OAL_FALSE,
                           OAM_MODULE_ID_HMAC,
                           pst_sta->st_vap_base_info.ul_core_id);

    return OAL_SUCC;
}



void hmac_p2p_listen_comp_cb(void *p_arg)
{
    hmac_vap_stru *pst_hmac_vap=NULL;
    mac_device_stru *pst_mac_device=NULL;
    hmac_scan_record_stru *pst_scan_record;

    pst_scan_record = (hmac_scan_record_stru *)p_arg;

    /* 判断listen完成时的状态 */
    if ((pst_scan_record->en_scan_rsp_status != MAC_SCAN_SUCCESS) &&
        (!hmac_get_feature_switch(HMAC_MIRACAST_REDUCE_LOG_SWITCH))) {
        oam_warning_log1(0, OAM_SF_P2P, "{hmac_p2p_listen_comp_cb::listen failed, listen rsp status: %d.}",
                         pst_scan_record->en_scan_rsp_status);
    }

    pst_hmac_vap = mac_res_get_hmac_vap(pst_scan_record->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log1(0, OAM_SF_P2P, "{hmac_p2p_listen_comp_cb::pst_hmac_vap is null:vap_id %d.}",
                       pst_scan_record->uc_vap_id);
        return;
    }

    pst_mac_device = mac_res_get_dev(pst_scan_record->uc_device_id);
    if (pst_mac_device == OAL_PTR_NULL) {
        oam_error_log1(0, OAM_SF_P2P, "{hmac_p2p_listen_comp_cb::pst_mac_device is null:vap_id %d.}",
                       pst_scan_record->uc_device_id);
        return;
    }

    
    if (pst_scan_record->ull_cookie == pst_mac_device->st_p2p_info.ull_last_roc_id) {
        /* 状态机调用: hmac_p2p_listen_timeout */
        if (OAL_SUCC != hmac_fsm_call_func_sta(pst_hmac_vap, HMAC_FSM_INPUT_LISTEN_TIMEOUT,
                                                   &(pst_hmac_vap->st_vap_base_info))) {
            oam_warning_log0(0, OAM_SF_P2P, "{hmac_p2p_listen_comp_cb::hmac_fsm_call_func_sta fail.}");
        }
    } else {
        oam_warning_log3(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_P2P,
            "{hmac_p2p_listen_comp_cb::ignore listen complete.scan_report_cookie[%x], \
            current_listen_cookie[%x], ull_last_roc_id[%x].}",
            pst_scan_record->ull_cookie,
            pst_mac_device->st_scan_params.ull_cookie,
            pst_mac_device->st_p2p_info.ull_last_roc_id);
    }

    return;
}

void hmac_mgmt_tx_roc_comp_cb(void *p_arg)
{
    hmac_vap_stru *pst_hmac_vap=NULL;
    mac_device_stru *pst_mac_device=NULL;
    hmac_scan_record_stru *pst_scan_record;

    pst_scan_record = (hmac_scan_record_stru *)p_arg;

    /* 判断listen完成时的状态 */
    if (pst_scan_record->en_scan_rsp_status != MAC_SCAN_SUCCESS) {
        oam_warning_log1(0, OAM_SF_P2P, "{hmac_mgmt_tx_roc_comp_cb::listen failed, listen rsp status: %d.}",
                         pst_scan_record->en_scan_rsp_status);
    }

    pst_hmac_vap = mac_res_get_hmac_vap(pst_scan_record->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log1(0, OAM_SF_P2P, "{hmac_mgmt_tx_roc_comp_cb::pst_hmac_vap is null:vap_id %d.}",
                       pst_scan_record->uc_vap_id);
        return;
    }

    pst_mac_device = mac_res_get_dev(pst_scan_record->uc_device_id);
    if (pst_mac_device == OAL_PTR_NULL) {
        oam_error_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_P2P,
                       "{hmac_mgmt_tx_roc_comp_cb::pst_mac_device is null:vap_id %d.}",
                       pst_scan_record->uc_device_id);
        return;
    }

    /* 由于P2P0 和P2P_CL 共用vap 结构体，监听超时，返回监听前保存的状态 */
    if (pst_hmac_vap->st_vap_base_info.en_vap_state >= MAC_VAP_STATE_STA_JOIN_COMP &&
        pst_hmac_vap->st_vap_base_info.en_vap_state <= MAC_VAP_STATE_STA_WAIT_ASOC) {
        oam_warning_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_P2P,
                         "{hmac_mgmt_tx_roc_comp_cb::vap is connecting,can not change vap state.}");
    } else if (pst_hmac_vap->st_vap_base_info.en_p2p_mode != WLAN_LEGACY_VAP_MODE) {
        mac_vap_state_change(&pst_hmac_vap->st_vap_base_info, pst_mac_device->st_p2p_info.en_last_vap_state);
    } else {
        mac_vap_state_change(&pst_hmac_vap->st_vap_base_info,
                                 pst_hmac_vap->st_vap_base_info.en_last_vap_state);
    }
    oam_warning_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_P2P, "{hmac_mgmt_tx_roc_comp_cb}");
}


OAL_STATIC void hmac_cfg80211_prepare_listen_req_param(mac_scan_req_stru *pst_scan_params, int8_t *puc_param)
{
    mac_remain_on_channel_param_stru *pst_remain_on_channel;
    mac_channel_stru *pst_channel_tmp = OAL_PTR_NULL;

    pst_remain_on_channel = (mac_remain_on_channel_param_stru *)puc_param;

    /* 设置监听信道信息到扫描参数中 */
    pst_scan_params->ast_channel_list[0].en_band = pst_remain_on_channel->en_band;
    pst_scan_params->ast_channel_list[0].en_bandwidth = pst_remain_on_channel->en_listen_channel_type;
    pst_scan_params->ast_channel_list[0].uc_chan_number = pst_remain_on_channel->uc_listen_channel;
    pst_scan_params->ast_channel_list[0].uc_chan_idx = 0;
    pst_channel_tmp = &(pst_scan_params->ast_channel_list[0]);
    if (OAL_SUCC != mac_get_channel_idx_from_num(pst_channel_tmp->en_band,
                                                     pst_channel_tmp->uc_chan_number,
                                                     &(pst_channel_tmp->uc_chan_idx))) {
        oam_warning_log2(0, OAM_SF_P2P,
            "{hmac_cfg80211_prepare_listen_req_param::mac_get_channel_idx_from_num fail.band[%u] channel[%u]}",
            pst_channel_tmp->en_band, pst_channel_tmp->uc_chan_number);
    }

    /* 设置其它监听参数 */
    pst_scan_params->uc_max_scan_count_per_channel = 1;
    pst_scan_params->uc_channel_nums = 1;
    pst_scan_params->uc_scan_func = MAC_SCAN_FUNC_P2P_LISTEN;
    pst_scan_params->us_scan_time = (uint16_t)pst_remain_on_channel->ul_listen_duration;
    if (IEEE80211_ROC_TYPE_MGMT_TX == pst_remain_on_channel->en_roc_type) {
        pst_scan_params->p_fn_cb = hmac_mgmt_tx_roc_comp_cb;
        if (hmac_get_feature_switch(HMAC_MIRACAST_SINK_SWITCH)) {
            pst_scan_params->bit_roc_type_tx_mgmt = OAL_TRUE;
        }
    } else {
        pst_scan_params->p_fn_cb = hmac_p2p_listen_comp_cb;
        if (hmac_get_feature_switch(HMAC_MIRACAST_SINK_SWITCH)) {
            pst_scan_params->bit_roc_type_tx_mgmt = OAL_FALSE;
        }
    }
    pst_scan_params->ull_cookie = pst_remain_on_channel->ull_cookie;
    pst_scan_params->bit_is_p2p0_scan = OAL_TRUE;
    pst_scan_params->uc_p2p0_listen_channel = pst_remain_on_channel->uc_listen_channel;

    return;
}


uint32_t hmac_p2p_listen_timeout(hmac_vap_stru *pst_hmac_vap_sta, void *p_param)
{
    mac_device_stru *pst_mac_device = OAL_PTR_NULL;
    hmac_vap_stru *pst_hmac_vap;
    mac_vap_stru *pst_mac_vap;
    hmac_device_stru *pst_hmac_device = OAL_PTR_NULL;
    hmac_scan_record_stru *pst_scan_record = OAL_PTR_NULL;

    pst_mac_vap = (mac_vap_stru *)p_param;
    pst_hmac_vap = mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log1(0, OAM_SF_P2P, "{hmac_p2p_listen_timeout::mac_res_get_hmac_vap fail.vap_id[%u]!}",
                       pst_mac_vap->uc_vap_id);
        return OAL_ERR_CODE_PTR_NULL;
    }
    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (pst_mac_device == OAL_PTR_NULL) {
        oam_warning_log1(0, OAM_SF_P2P, "{hmac_p2p_listen_timeout::mac_res_get_dev fail.device_id[%u]!}",
                         pst_mac_vap->uc_device_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 获取hmac device */
    pst_hmac_device = hmac_res_get_mac_dev(pst_mac_device->uc_device_id);
    if (oal_unlikely(pst_hmac_device == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_P2P, "{hmac_p2p_listen_timeout::pst_hmac_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    oam_info_log2(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_P2P,
                  "{hmac_p2p_listen_timeout::current pst_mac_vap channel is [%d] state[%d]}",
                  pst_mac_vap->st_channel.uc_chan_number,
                  pst_hmac_vap->st_vap_base_info.en_vap_state);

    oam_info_log2(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_P2P,
                  "{hmac_p2p_listen_timeout::next pst_mac_vap channel is [%d] state[%d]}",
                  pst_mac_vap->st_channel.uc_chan_number,
                  pst_mac_device->st_p2p_info.en_last_vap_state);

    /* 由于P2P0 和P2P_CL 共用vap 结构体，监听超时，返回监听前保存的状态 */
    if (pst_hmac_vap->st_vap_base_info.en_p2p_mode != WLAN_LEGACY_VAP_MODE) {
        mac_vap_state_change(&pst_hmac_vap->st_vap_base_info, pst_mac_device->st_p2p_info.en_last_vap_state);
    } else {
        mac_vap_state_change(&pst_hmac_vap->st_vap_base_info,
                                 pst_hmac_vap->st_vap_base_info.en_last_vap_state);
    }

    pst_scan_record = &(pst_hmac_device->st_scan_mgmt.st_scan_record_mgmt);
    if (pst_scan_record->ull_cookie == pst_mac_device->st_p2p_info.ull_last_roc_id) {
        /* 3.1 抛事件到WAL ，上报监听结束 */
        hmac_p2p_send_listen_expired_to_host(pst_hmac_vap);
    }

    /* 3.2 抛事件到DMAC ，返回监听信道 */
    hmac_p2p_send_listen_expired_to_device(pst_hmac_vap);

    return OAL_SUCC;
}


uint32_t hmac_p2p_remain_on_channel(hmac_vap_stru *pst_hmac_vap_sta, void *p_param)
{
    mac_device_stru *pst_mac_device = OAL_PTR_NULL;
    mac_vap_stru *pst_mac_vap;
    mac_remain_on_channel_param_stru *pst_remain_on_channel;
    mac_scan_req_h2d_stru st_scan_h2d_params;
    uint32_t ul_ret;

    pst_remain_on_channel = (mac_remain_on_channel_param_stru *)p_param;

    pst_mac_vap = mac_res_get_mac_vap(pst_hmac_vap_sta->st_vap_base_info.uc_vap_id);
    if (pst_mac_vap == OAL_PTR_NULL) {
        oam_error_log1(0, OAM_SF_P2P, "{hmac_p2p_remain_on_channel::mac_res_get_mac_vap fail.vap_id[%u]!}",
                       pst_hmac_vap_sta->st_vap_base_info.uc_vap_id);
        return OAL_ERR_CODE_PTR_NULL;
    }
    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (oal_unlikely(pst_mac_device == OAL_PTR_NULL)) {
        oam_error_log1(0, OAM_SF_ANY,
            "{hmac_p2p_listen_timeout::pst_mac_device[%d](%d) null!}", pst_mac_vap->uc_device_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

    
    if (pst_hmac_vap_sta->st_vap_base_info.en_vap_state == MAC_VAP_STATE_STA_LISTEN) {
        hmac_p2p_send_listen_expired_to_host(pst_hmac_vap_sta);
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_P2P,
            "{hmac_p2p_remain_on_channel::listen nested, send remain on channel expired to host!curr_state[%d]}",
            pst_hmac_vap_sta->st_vap_base_info.en_vap_state);
    }

    /* 修改P2P_DEVICE 状态为监听状态 */
    
    mac_vap_state_change((mac_vap_stru *)&pst_hmac_vap_sta->st_vap_base_info, MAC_VAP_STATE_STA_LISTEN);
    oam_info_log4(pst_mac_vap->uc_vap_id, OAM_SF_P2P,
        "{hmac_p2p_remain_on_channel: get in listen state!last_state %d, channel %d, duration %d, curr_state %d}",
        pst_mac_device->st_p2p_info.en_last_vap_state,
        pst_remain_on_channel->uc_listen_channel,
        pst_remain_on_channel->ul_listen_duration,
        pst_hmac_vap_sta->st_vap_base_info.en_vap_state);

    /* 准备监听参数 */
    memset_s(&st_scan_h2d_params, OAL_SIZEOF(mac_scan_req_h2d_stru), 0, OAL_SIZEOF(mac_scan_req_h2d_stru));
    hmac_cfg80211_prepare_listen_req_param(&(st_scan_h2d_params.st_scan_params), (int8_t *)pst_remain_on_channel);

    /* 调用扫描入口，准备进行监听动作，不管监听动作执行成功或失败，都返回监听成功 */
    /* 状态机调用: hmac_scan_proc_scan_req_event */
    ul_ret = hmac_fsm_call_func_sta(pst_hmac_vap_sta, HMAC_FSM_INPUT_SCAN_REQ, (void *)(&st_scan_h2d_params));
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_SCAN,
            "{hmac_p2p_remain_on_channel::hmac_fsm_call_func_sta fail[%d].}", ul_ret);
        if (hmac_get_feature_switch(HMAC_MIRACAST_SINK_SWITCH)) {
            /* 大屏需要持续侦听，在下发侦听失败后，需通知wpas恢复P2P状态为IDLE，以便后续能再次进入侦听流程  */
            return ul_ret;
        }
    }

    return OAL_SUCC;
}



OAL_STATIC uint32_t hmac_sta_not_up_rx_wnm_action(mac_wnm_action_type_enum_uint8 en_wnm_action_field,
                                                    hmac_vap_stru *pst_hmac_vap_sta,
                                                    hmac_user_stru *pst_hmac_user,
                                                    oal_netbuf_stru *pst_netbuf)
{
    switch (en_wnm_action_field) {
#ifdef _PRE_WLAN_FEATURE_11V_ENABLE
        /* bss transition request 帧处理入口 */
        case MAC_WNM_ACTION_BSS_TRANSITION_MGMT_REQUEST:
            if (pst_hmac_user != OAL_PTR_NULL) {    /* BSS Transtion Request hmac user exist */
                hmac_rx_bsst_req_action(pst_hmac_vap_sta, pst_hmac_user, pst_netbuf);
            } else {
                oam_warning_log0(pst_hmac_vap_sta->st_vap_base_info.uc_vap_id, OAM_SF_RX,
                    "{hmac_sta_not_up_rx_mgmt:: bss transition pst_hmac_user don't exist.}");
            }
            break;
#endif
        default:
            hmac_rx_mgmt_send_to_host(pst_hmac_vap_sta, pst_netbuf);
            break;
    }

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_FTM

OAL_STATIC uint32_t hmac_sta_not_up_rx_public_action(mac_public_action_type_enum_uint8 en_public_action_field,
    hmac_vap_stru *pst_hmac_vap_sta, oal_netbuf_stru *pst_netbuf)
{
    uint32_t ul_ret;

    switch (en_public_action_field) {
        case MAC_PUB_GAS_INIT_RESP:
            oam_warning_log0(0, OAM_SF_RX, "{hmac_sta_not_up_rx_public_action::receive gas initial response frame.}");
            ul_ret = hmac_ftm_rx_gas_initial_response_frame(pst_hmac_vap_sta, pst_netbuf);
            if (ul_ret == OAL_FAIL) {
                hmac_rx_mgmt_send_to_host(pst_hmac_vap_sta, pst_netbuf);
            }
            break;

        default:
            oam_warning_log0(0, OAM_SF_RX, "{hmac_sta_not_up_rx_public_action::rx other public action frame }");
            hmac_rx_mgmt_send_to_host(pst_hmac_vap_sta, pst_netbuf);
            break;
    }

    return OAL_SUCC;
}
#endif

OAL_STATIC void hmac_sta_not_up_rx_mgmt_action_proc(uint8_t *puc_data,
    hmac_vap_stru *pst_hmac_vap_sta, hmac_user_stru *pst_hmac_user, dmac_wlan_crx_event_stru *pst_mgmt_rx_event)
{
    switch (puc_data[MAC_ACTION_OFFSET_CATEGORY]) {
        case MAC_ACTION_CATEGORY_WNM:
            hmac_sta_not_up_rx_wnm_action(puc_data[MAC_ACTION_OFFSET_ACTION], pst_hmac_vap_sta,
                                          pst_hmac_user, pst_mgmt_rx_event->pst_netbuf);
            break;
#ifdef _PRE_WLAN_FEATURE_FTM // FTM认证
        case MAC_ACTION_CATEGORY_PUBLIC:
            if (g_wlan_spec_cfg->feature_ftm_is_open) {
                hmac_sta_not_up_rx_public_action(puc_data[MAC_ACTION_OFFSET_ACTION], pst_hmac_vap_sta,
                                                 pst_mgmt_rx_event->pst_netbuf);
                break;
            }
            /* fall through */
#endif
        default:
            hmac_rx_mgmt_send_to_host(pst_hmac_vap_sta, pst_mgmt_rx_event->pst_netbuf);
            break;
    }
}


uint32_t hmac_sta_not_up_rx_mgmt(hmac_vap_stru *pst_hmac_vap_sta, void *p_param)
{
    dmac_wlan_crx_event_stru *pst_mgmt_rx_event = OAL_PTR_NULL;
    mac_rx_ctl_stru *pst_rx_info = OAL_PTR_NULL;
    uint8_t *puc_mac_hdr = OAL_PTR_NULL;
    uint8_t *puc_data = OAL_PTR_NULL;
    uint8_t uc_mgmt_frm_type;
    mac_ieee80211_frame_stru *pst_frame_hdr = OAL_PTR_NULL; /* 保存mac帧的指针 */
    hmac_user_stru *pst_hmac_user = OAL_PTR_NULL;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    if (oal_any_null_ptr2(pst_hmac_vap_sta, p_param)) {
        oam_error_log2(0, OAM_SF_RX, "{hmac_sta_not_up_rx_mgmt::PTR_NULL, hmac_vap_addr[%p], param[%p].}",
                       (uintptr_t)pst_hmac_vap_sta, (uintptr_t)p_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_mgmt_rx_event = (dmac_wlan_crx_event_stru *)p_param;
    pst_rx_info = (mac_rx_ctl_stru *)oal_netbuf_cb(pst_mgmt_rx_event->pst_netbuf);
    puc_mac_hdr = (uint8_t *)MAC_GET_RX_CB_MAC_HEADER_ADDR(pst_rx_info);
    if (puc_mac_hdr == OAL_PTR_NULL) {
        oam_error_log3(0, OAM_SF_RX, "{hmac_sta_not_up_rx_mgmt::mac_hdr null, vap_id %d, frame_len %d, mac_hdr_len %d}",
            pst_rx_info->bit_vap_id, pst_rx_info->us_frame_len, pst_rx_info->uc_mac_header_len);
        return OAL_ERR_CODE_PTR_NULL;
    }
    /* 获取帧体指针 */
    puc_data = (uint8_t *)MAC_GET_RX_CB_MAC_HEADER_ADDR(pst_rx_info) + pst_rx_info->uc_mac_header_len;
    /* STA在NOT UP状态下接收到各种管理帧处理 */
    uc_mgmt_frm_type = mac_get_frame_type_and_subtype(puc_mac_hdr);

#ifdef _PRE_WLAN_WAKEUP_SRC_PARSE
    if (OAL_TRUE == wlan_pm_wkup_src_debug_get()) {
        wlan_pm_wkup_src_debug_set(OAL_FALSE);
        oam_warning_log1(pst_hmac_vap_sta->st_vap_base_info.uc_vap_id, OAM_SF_RX,
                         "{wifi_wake_src:hmac_sta_not_up_rx_mgmt::wakeup mgmt type[0x%x]}", uc_mgmt_frm_type);
    }
#endif

#ifdef _PRE_WLAN_FEATURE_SNIFFER
#ifdef CONFIG_HW_SNIFFER
    proc_sniffer_write_file(NULL, 0, puc_mac_hdr, pst_rx_info->us_frame_len, 0);
#endif
#endif
    switch (uc_mgmt_frm_type) {
        /* 判断接收到的管理帧类型 */
        case WLAN_FC0_SUBTYPE_PROBE_REQ | WLAN_FC0_TYPE_MGT:
            pst_mac_vap = &(pst_hmac_vap_sta->st_vap_base_info);
            /* 判断为P2P设备,则上报probe req帧到wpa_supplicant */
            if (!IS_LEGACY_VAP(pst_mac_vap)) {
                hmac_rx_mgmt_send_to_host(pst_hmac_vap_sta, pst_mgmt_rx_event->pst_netbuf);
            }
            break;
        case WLAN_FC0_SUBTYPE_ACTION | WLAN_FC0_TYPE_MGT:
#if defined(_PRE_WLAN_FEATURE_LOCATION) || defined(_PRE_WLAN_FEATURE_PSD_ANALYSIS)
            if (0 == oal_memcmp(puc_data + 2, g_auc_huawei_oui, MAC_OUI_LEN)) {
                hmac_huawei_action_process(pst_hmac_vap_sta, pst_mgmt_rx_event->pst_netbuf, puc_data[2 + MAC_OUI_LEN]);
            } else
#endif
            /* 如果是Action 帧，则直接上报wpa_supplicant */
            {
                pst_frame_hdr = (mac_ieee80211_frame_stru *)puc_mac_hdr;
                pst_hmac_user = mac_vap_get_hmac_user_by_addr(&pst_hmac_vap_sta->st_vap_base_info,
                                                              pst_frame_hdr->auc_address2);

                oam_warning_log1(pst_hmac_vap_sta->st_vap_base_info.uc_vap_id, OAM_SF_RX,
                    "{hmac_sta_not_up_rx_mgmt::category=[%d].}", puc_data[MAC_ACTION_OFFSET_CATEGORY]);
                hmac_sta_not_up_rx_mgmt_action_proc(puc_data, pst_hmac_vap_sta, pst_hmac_user, pst_mgmt_rx_event);
            }
            break;
        case WLAN_FC0_SUBTYPE_PROBE_RSP | WLAN_FC0_TYPE_MGT:
            /* 如果是probe response帧，则直接上报wpa_supplicant */
            hmac_rx_mgmt_send_to_host(pst_hmac_vap_sta, pst_mgmt_rx_event->pst_netbuf);
            break;
        default:
            break;
    }
    return OAL_SUCC;
}


OAL_STATIC uint32_t hmac_update_vht_opern_ie_sta(mac_vap_stru *pst_mac_vap,
                                                   hmac_user_stru *pst_hmac_user,
                                                   uint8_t *puc_payload)
{
    if (oal_any_null_ptr3(pst_mac_vap, pst_hmac_user, puc_payload)) {
        oam_error_log3(0, OAM_SF_ASSOC, "{hmac_update_vht_opern_ie_sta::param null,%x %x %x.}",
                       (uintptr_t)pst_mac_vap, (uintptr_t)pst_hmac_user, (uintptr_t)puc_payload);
        return MAC_NO_CHANGE;
    }

    /* 支持11ac，才进行后续的处理 */
    if (OAL_FALSE == mac_mib_get_VHTOptionImplemented(pst_mac_vap)) {
        return MAC_NO_CHANGE;
    }

    return mac_ie_proc_vht_opern_ie(pst_mac_vap, puc_payload, &(pst_hmac_user->st_user_base_info));
}

#ifdef _PRE_WLAN_FEATURE_11AX

OAL_STATIC uint32_t hmac_update_he_opern_ie_sta(mac_vap_stru *pst_mac_vap,
                                                  hmac_user_stru *pst_hmac_user,
                                                  uint8_t *puc_payload)
{
    mac_user_stru *pst_mac_user = OAL_PTR_NULL;

    if (oal_any_null_ptr3(pst_mac_vap, pst_hmac_user, puc_payload)) {
        oam_error_log3(0, OAM_SF_ASSOC, "{hmac_update_he_opern_ie_sta::param null,%X %X %X.}",
                       (uintptr_t)pst_mac_vap, (uintptr_t)pst_hmac_user, (uintptr_t)puc_payload);
        return MAC_NO_CHANGE;
    }

    /* 支持11ax，才进行后续的处理 */
    if (OAL_FALSE == mac_mib_get_HEOptionImplemented(pst_mac_vap)) {
        return MAC_NO_CHANGE;
    }

    pst_mac_user = &(pst_hmac_user->st_user_base_info);
    return mac_ie_proc_he_opern_ie(pst_mac_vap, puc_payload, pst_mac_user);
}


OAL_STATIC void hmac_sta_up_update_he_edca_params_mib(hmac_vap_stru *pst_hmac_sta,
                                                          mac_frame_he_mu_ac_parameter *pst_ac_param)
{
    uint8_t uc_aci;
    uint8_t uc_mu_edca_timer;
    uint32_t ul_txop_limit;

    /*      AC Parameters Record Format           */
    /* ------------------------------------------ */
    /* |     1     |       1       |      1     | */
    /* ------------------------------------------ */
    /* | ACI/AIFSN | ECWmin/ECWmax | MU EDCA Timer | */
    /* ------------------------------------------ */
    /************* ACI/AIFSN Field ***************/
    /*     ---------------------------------- */
    /* bit |   4   |  1  |  2  |    1     |   */
    /*     ---------------------------------- */
    /*     | AIFSN | ACM | ACI | Reserved |   */
    /*     ---------------------------------- */
    uc_aci = pst_ac_param->bit_ac_index;
    ;

    /* ECWmin/ECWmax Field     */
    /*     ------------------- */
    /* bit |   4    |   4    | */
    /*     ------------------- */
    /*     | ECWmin | ECWmax | */
    /*     ------------------- */
    /* 在mib库中和寄存器里保存的TXOP值都是以 8TU 为单位的
       ==MU EDCA Timer 单位是8TU --8 * 1024us
    */
    uc_mu_edca_timer = pst_ac_param->uc_mu_edca_timer;
    ul_txop_limit = (uint32_t)(uc_mu_edca_timer);

    /* 更新相应的MIB库信息 */
    if (uc_aci < WLAN_WME_AC_BUTT) {
        mac_mib_set_QAPMUEDCATableIndex(&pst_hmac_sta->st_vap_base_info, uc_aci, uc_aci + 1); /* 注: 协议规定取值1 2 3 4 */
        mac_mib_set_QAPMUEDCATableCWmin(&pst_hmac_sta->st_vap_base_info, uc_aci, pst_ac_param->bit_ecw_min);
        mac_mib_set_QAPMUEDCATableCWmax(&pst_hmac_sta->st_vap_base_info, uc_aci, pst_ac_param->bit_ecw_max);
        mac_mib_set_QAPMUEDCATableAIFSN(&pst_hmac_sta->st_vap_base_info, uc_aci, pst_ac_param->bit_aifsn);
        mac_mib_set_QAPMUEDCATableTXOPLimit(&pst_hmac_sta->st_vap_base_info, uc_aci, ul_txop_limit);
        mac_mib_set_QAPMUEDCATableMandatory(&pst_hmac_sta->st_vap_base_info, uc_aci, pst_ac_param->bit_acm);
    }
}


void hmac_sta_up_update_he_edca_params(uint8_t *puc_payload,
                                           uint16_t us_msg_len,
                                           hmac_vap_stru *pst_hmac_sta,
                                           uint8_t uc_frame_sub_type,
                                           hmac_user_stru *pst_hmac_user)
{
    uint8_t uc_ac_num_loop;
    uint32_t ul_ret;
    mac_device_stru *pst_mac_device = OAL_PTR_NULL;
    uint8_t *puc_ie = OAL_PTR_NULL;
    mac_frame_he_mu_edca_parameter_ie_stru st_mu_edca_value;

    memset_s(&st_mu_edca_value, OAL_SIZEOF(mac_frame_he_mu_edca_parameter_ie_stru),
             0, OAL_SIZEOF(mac_frame_he_mu_edca_parameter_ie_stru));
    if (OAL_FALSE == mac_mib_get_HEOptionImplemented(&(pst_hmac_sta->st_vap_base_info))) {
        return;
    }

    if (pst_hmac_sta->uc_disable_mu_edca) {
        return;
    }

    pst_mac_device = (mac_device_stru *)mac_res_get_dev(pst_hmac_sta->st_vap_base_info.uc_device_id);
    if (pst_mac_device == OAL_PTR_NULL) {
        oam_warning_log1(0, OAM_SF_ASSOC,
                         "{hmac_sta_up_update_he_edca_params::mac_res_get_dev fail.device_id[%u]!}",
                         pst_hmac_sta->st_vap_base_info.uc_device_id);
        return;
    }

    /************************ MU EDCA Parameter Set Element ***************************/
    /* ------------------------------------------------------------------------------------------- */
    /* | EID | LEN | Ext EID|MU Qos Info |MU AC_BE Parameter Record | MU AC_BK Parameter Record  | */
    /* ------------------------------------------------------------------------------------------- */
    /* |  1  |  1  |   1    |    1       |     3                    |        3                   | */
    /* ------------------------------------------------------------------------------------------- */
    /* ------------------------------------------------------------------------------ ------------- */
    /* | MU AC_VI Parameter Record | MU AC_VO Parameter Record                                   | */
    /* ------------------------------------------------------------------------------------------- */
    /* |    3                      |     3                                                       | */
    /******************* QoS Info field when sent from WMM AP *****************/
    /* -------------------------------------------------------------------------------------------- */
    /*    | EDCA Parameter Set Update Count | Q-Ack | Queue Request |TXOP Request | More Data Ack| */
    /* --------------------------------------------------------------------------------------------- */
    /* bit |        0~3                      |  1    |  1            |   1         |     1        | */
    /* --------------------------------------------------------------------------------------------- */
    /**************************************************************************/
    puc_ie = mac_find_ie_ext_ie(MAC_EID_HE, MAC_EID_EXT_HE_EDCA, puc_payload, us_msg_len);
    if (puc_ie != OAL_PTR_NULL) {
        ul_ret = mac_ie_parse_mu_edca_parameter(puc_ie, &st_mu_edca_value);
        if (ul_ret != OAL_SUCC) {
            return;
        }

        /* 如果收到的是beacon帧，并且param_set_count没有改变
           则STA也不用做任何改变，直接返回即可 */
        if ((uc_frame_sub_type == WLAN_FC0_SUBTYPE_BEACON) &&
            (st_mu_edca_value.st_qos_info.bit_edca_update_count ==
             pst_hmac_sta->st_vap_base_info.uc_he_mu_edca_update_count)) {
            return;
        }

        pst_hmac_sta->st_vap_base_info.uc_he_mu_edca_update_count = st_mu_edca_value.st_qos_info.bit_edca_update_count;

        /* 针对每一个AC，更新EDCA参数 */
        for (uc_ac_num_loop = 0; uc_ac_num_loop < WLAN_WME_AC_BUTT; uc_ac_num_loop++) {
            hmac_sta_up_update_he_edca_params_mib(pst_hmac_sta,
                &(st_mu_edca_value.ast_mu_ac_parameter[uc_ac_num_loop]));
        }

        /* 更新EDCA相关的MAC寄存器 */
        hmac_sta_up_update_mu_edca_params_machw(pst_hmac_sta, MAC_WMM_SET_PARAM_TYPE_UPDATE_EDCA);
        return;
    }
}


void hmac_sta_up_update_he_nfrp_params(uint8_t *payload,
    uint16_t msg_len, hmac_vap_stru *hmac_sta, uint8_t frame_sub_type, hmac_user_stru *hmac_user)
{
    uint8_t tmp_nfrp_thr_exp = 0;
    mac_user_stru *mac_user = &(hmac_user->st_user_base_info);
    frw_event_mem_stru *event_mem = OAL_PTR_NULL;
    frw_event_stru *event = OAL_PTR_NULL;
    uint8_t *ie = OAL_PTR_NULL;

    if (IS_LEGACY_STA(&(hmac_sta->st_vap_base_info)) == OAL_FALSE) {
        return;
    }

    if (mac_user == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ASSOC, "{hmac_sta_up_update_he_nfrp_params::mac_user NULL!}");
        return;
    }

    if (mac_mib_get_HEOptionImplemented(&(hmac_sta->st_vap_base_info)) == OAL_FALSE) {
        return;
    }

    /************ NDP Feedback Report Parameter Set Element ****************/
    /* ------------------------------------------------------------------- */
    /* | EID | LEN | Ext EID|Resource Request Buffer Threshold Exponent |  */
    /* ------------------------------------------------------------------- */
    /* |  1  |  1  |   1    |                     1                     |  */
    /* ------------------------------------------------------------------- */

    ie = mac_find_ie_ext_ie(MAC_EID_HE, MAC_EID_EXT_HE_NDP_FEEDBACK_REPORT, payload, msg_len);
    if (ie == OAL_PTR_NULL) {
        return;
    }

    if (mac_ie_parse_he_ndp_feedback_report_ie(ie, &tmp_nfrp_thr_exp) != OAL_SUCC) {
        return;
    }

    /* 如果收到的是beacon帧，并且uc_resource_req_buff_threshold_exp没有改变,
       则STA也不做任何改变，直接返回即可 */
    if ((frame_sub_type == WLAN_FC0_SUBTYPE_BEACON) && (mac_user->uc_res_req_buff_threshold_exp == tmp_nfrp_thr_exp)) {
        return;
    }
    mac_user->uc_res_req_buff_threshold_exp = tmp_nfrp_thr_exp;

    /* 抛事件到dmac更新resource request buffer threshold exponent数值 */
    event_mem = frw_event_alloc_m(OAL_SIZEOF(uint8_t));
    if (event_mem == OAL_PTR_NULL) {
        oam_error_log1(hmac_sta->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                       "{hmac_sta_up_update_he_nfrp_params::event_mem alloc null, size[%d].}", OAL_SIZEOF(uint8_t));
        return;
    }

    /* 填写事件 */
    event = frw_get_event_stru(event_mem);
    frw_event_hdr_init(&(event->st_event_hdr), FRW_EVENT_TYPE_WLAN_CTX,
        DMAC_WLAN_CTX_EVENT_SUB_TYPE_STA_SET_NDP_FEEDBACK_REPORT_REG,
        OAL_SIZEOF(uint8_t), FRW_EVENT_PIPELINE_STAGE_1, hmac_sta->st_vap_base_info.uc_chip_id,
        hmac_sta->st_vap_base_info.uc_device_id, hmac_sta->st_vap_base_info.uc_vap_id);

    /* 拷贝参数 */
    if (memcpy_s(frw_get_event_payload(event_mem), OAL_SIZEOF(uint8_t),
        &tmp_nfrp_thr_exp, OAL_SIZEOF(uint8_t)) != EOK) {
        oam_error_log0(0, OAM_SF_ASSOC, "hmac_sta_up_update_he_nfrp_params::memcpy fail!");
        frw_event_free_m(event_mem);
        return;
    }

    /* 分发事件 */
    frw_event_dispatch_event(event_mem);
    frw_event_free_m(event_mem);

    return;
}


uint32_t hmac_sta_up_update_mu_edca_params_machw(hmac_vap_stru *pst_hmac_sta,
                                                   mac_wmm_set_param_type_enum_uint8 en_type)
{
    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL;
    frw_event_stru *pst_event = OAL_PTR_NULL;
    dmac_ctx_sta_asoc_set_edca_reg_stru st_asoc_set_edca_reg_param = { 0 };
    dmac_ctx_sta_asoc_set_edca_reg_stru *pst_edca_para;

    /* 抛事件到dmac写寄存器 */
    /* 申请事件内存 */
    pst_event_mem = frw_event_alloc_m(OAL_SIZEOF(dmac_ctx_sta_asoc_set_edca_reg_stru));
    if (pst_event_mem == OAL_PTR_NULL) {
        oam_error_log1(pst_hmac_sta->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                       "{hmac_update_vht_opern_ie_sta::event_mem alloc null, size[%d].}",
                       OAL_SIZEOF(dmac_ctx_sta_asoc_set_edca_reg_stru));
        return OAL_ERR_CODE_PTR_NULL;
    }

    st_asoc_set_edca_reg_param.uc_vap_id = pst_hmac_sta->st_vap_base_info.uc_vap_id;
    st_asoc_set_edca_reg_param.en_set_param_type = en_type;

    /* 填写事件 */
    pst_event = frw_get_event_stru(pst_event_mem);
    frw_event_hdr_init(&(pst_event->st_event_hdr),
                       FRW_EVENT_TYPE_WLAN_CTX,
                       DMAC_WLAN_CTX_EVENT_SUB_TYPE_STA_SET_MU_EDCA_REG,
                       OAL_SIZEOF(dmac_ctx_sta_asoc_set_edca_reg_stru),
                       FRW_EVENT_PIPELINE_STAGE_1,
                       pst_hmac_sta->st_vap_base_info.uc_chip_id,
                       pst_hmac_sta->st_vap_base_info.uc_device_id,
                       pst_hmac_sta->st_vap_base_info.uc_vap_id);

    memcpy_s((uint8_t *)&st_asoc_set_edca_reg_param.ast_wlan_mib_qap_edac,
             (OAL_SIZEOF(wlan_mib_Dot11QAPEDCAEntry_stru) * WLAN_WME_AC_BUTT),
             (uint8_t *)pst_hmac_sta->st_vap_base_info.st_wlan_mib_mu_edca.ast_wlan_mib_qap_edac,
             (OAL_SIZEOF(wlan_mib_Dot11QAPEDCAEntry_stru) * WLAN_WME_AC_BUTT));

    /* 拷贝参数 */
    pst_edca_para = (dmac_ctx_sta_asoc_set_edca_reg_stru *)frw_get_event_payload(pst_event_mem);
    memcpy_s(pst_edca_para, OAL_SIZEOF(dmac_ctx_sta_asoc_set_edca_reg_stru),
             (uint8_t *)&st_asoc_set_edca_reg_param, OAL_SIZEOF(dmac_ctx_sta_asoc_set_edca_reg_stru));

    /* 分发事件 */
    frw_event_dispatch_event(pst_event_mem);
    frw_event_free_m(pst_event_mem);

    return OAL_SUCC;
}


void hmac_sta_up_update_assoc_rsp_sr_params(uint8_t *puc_payload,
                                                uint16_t us_msg_len,
                                                hmac_vap_stru *pst_hmac_sta,
                                                uint8_t uc_frame_sub_type,
                                                hmac_user_stru *pst_hmac_user)
{
    uint32_t ul_ret;
    uint8_t *puc_ie = OAL_PTR_NULL;
    mac_frame_he_spatial_reuse_parameter_set_ie_stru st_sr_ie;
    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL;
    frw_event_stru *pst_event = OAL_PTR_NULL;
    dmac_ctx_sta_asoc_set_sr_reg_stru *pst_sr_reg_param = OAL_PTR_NULL;

    if (OAL_FALSE == mac_mib_get_HEOptionImplemented(&(pst_hmac_sta->st_vap_base_info))) {
        return;
    }

    puc_ie = mac_find_ie_ext_ie(MAC_EID_HE, MAC_EID_EXT_HE_SRP, puc_payload, us_msg_len);
    if (puc_ie == OAL_PTR_NULL) {
        return;
    }

    memset_s(&st_sr_ie, OAL_SIZEOF(st_sr_ie), 0, OAL_SIZEOF(st_sr_ie));
    ul_ret = mac_ie_parse_spatial_reuse_parameter(puc_ie, &st_sr_ie);
    if (ul_ret != OAL_SUCC) {
        return;
    }

    /* 抛事件到dmac写寄存器 */
    /* 申请事件内存 */
    pst_event_mem = frw_event_alloc_m(OAL_SIZEOF(dmac_ctx_sta_asoc_set_sr_reg_stru));
    if (pst_event_mem == OAL_PTR_NULL) {
        oam_error_log1(pst_hmac_sta->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                       "{hmac_sta_up_update_assoc_rsp_sr_params::event_mem alloc null, size[%d].}",
                       OAL_SIZEOF(dmac_ctx_sta_asoc_set_edca_reg_stru));
        return;
    }

    /* 填写事件 */
    pst_event = frw_get_event_stru(pst_event_mem);
    frw_event_hdr_init(&(pst_event->st_event_hdr),
                       FRW_EVENT_TYPE_WLAN_CTX,
                       DMAC_WLAN_CTX_EVENT_SUB_TYPE_STA_SET_SPATIAL_REUSE_REG,
                       OAL_SIZEOF(dmac_ctx_sta_asoc_set_sr_reg_stru),
                       FRW_EVENT_PIPELINE_STAGE_1,
                       pst_hmac_sta->st_vap_base_info.uc_chip_id,
                       pst_hmac_sta->st_vap_base_info.uc_device_id,
                       pst_hmac_sta->st_vap_base_info.uc_vap_id);
    pst_sr_reg_param = (dmac_ctx_sta_asoc_set_sr_reg_stru *)frw_get_event_payload(pst_event_mem);

    memcpy_s(&pst_sr_reg_param->st_sr_ie, OAL_SIZEOF(mac_frame_he_spatial_reuse_parameter_set_ie_stru),
             (uint8_t *)&st_sr_ie, OAL_SIZEOF(mac_frame_he_spatial_reuse_parameter_set_ie_stru));

    /* 分发事件 */
    frw_event_dispatch_event(pst_event_mem);
    frw_event_free_m(pst_event_mem);

    return;
}

#endif


uint32_t hmac_sta_up_update_edca_params_machw(hmac_vap_stru *pst_hmac_sta,
                                                    mac_wmm_set_param_type_enum_uint8 en_type)
{
    frw_event_mem_stru *pst_event_mem;
    frw_event_stru *pst_event = OAL_PTR_NULL;
    dmac_ctx_sta_asoc_set_edca_reg_stru st_asoc_set_edca_reg_param = { 0 };

    /* 抛事件到dmac写寄存器 */
    /* 申请事件内存 */
    pst_event_mem = frw_event_alloc_m(OAL_SIZEOF(dmac_ctx_sta_asoc_set_edca_reg_stru));
    if (pst_event_mem == OAL_PTR_NULL) {
        oam_error_log1(pst_hmac_sta->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                       "{hmac_update_vht_opern_ie_sta::event_mem alloc null, size[%d].}",
                       OAL_SIZEOF(dmac_ctx_sta_asoc_set_edca_reg_stru));
        return OAL_ERR_CODE_PTR_NULL;
    }

    st_asoc_set_edca_reg_param.uc_vap_id = pst_hmac_sta->st_vap_base_info.uc_vap_id;
    st_asoc_set_edca_reg_param.en_set_param_type = en_type;

    /* 填写事件 */
    pst_event = frw_get_event_stru(pst_event_mem);
    frw_event_hdr_init(&(pst_event->st_event_hdr),
                       FRW_EVENT_TYPE_WLAN_CTX,
                       DMAC_WLAN_CTX_EVENT_SUB_TYPE_STA_SET_EDCA_REG,
                       OAL_SIZEOF(dmac_ctx_sta_asoc_set_edca_reg_stru),
                       FRW_EVENT_PIPELINE_STAGE_1,
                       pst_hmac_sta->st_vap_base_info.uc_chip_id,
                       pst_hmac_sta->st_vap_base_info.uc_device_id,
                       pst_hmac_sta->st_vap_base_info.uc_vap_id);

    if (en_type != MAC_WMM_SET_PARAM_TYPE_DEFAULT) {
        memcpy_s((uint8_t *)&st_asoc_set_edca_reg_param.ast_wlan_mib_qap_edac,
                 (OAL_SIZEOF(wlan_mib_Dot11QAPEDCAEntry_stru) * WLAN_WME_AC_BUTT),
                 (uint8_t *)&pst_hmac_sta->st_vap_base_info.pst_mib_info->st_wlan_mib_edca.ast_wlan_mib_qap_edac,
                 (OAL_SIZEOF(wlan_mib_Dot11QAPEDCAEntry_stru) * WLAN_WME_AC_BUTT));
    }

    /* 拷贝参数 */
    if (EOK != memcpy_s(frw_get_event_payload(pst_event_mem), OAL_SIZEOF(dmac_ctx_sta_asoc_set_edca_reg_stru),
                        (uint8_t *)&st_asoc_set_edca_reg_param, OAL_SIZEOF(dmac_ctx_sta_asoc_set_edca_reg_stru))) {
        oam_error_log0(0, OAM_SF_ANY, "hmac_sta_up_update_edca_params_machw::memcpy fail!");
        frw_event_free_m(pst_event_mem);
        return OAL_FAIL;
    }

    /* 分发事件 */
    frw_event_dispatch_event(pst_event_mem);
    frw_event_free_m(pst_event_mem);

    return OAL_SUCC;
}


OAL_STATIC void hmac_sta_up_update_edca_params_mib(hmac_vap_stru *pst_hmac_sta, uint8_t *puc_payload)
{
    uint8_t uc_aifsn;
    uint8_t uc_aci;
    uint8_t uc_ecwmin;
    uint8_t uc_ecwmax;
    uint16_t us_txop_limit;
    oal_bool_enum_uint8 en_acm;
    /*         AC Parameters Record Format        */
    /* ------------------------------------------ */
    /* |     1     |       1       |      2     | */
    /* ------------------------------------------ */
    /* | ACI/AIFSN | ECWmin/ECWmax | TXOP Limit | */
    /* ------------------------------------------ */
    /************* ACI/AIFSN Field ***************/
    /*     ---------------------------------- */
    /* bit |   4   |  1  |  2  |    1     |   */
    /*     ---------------------------------- */
    /*     | AIFSN | ACM | ACI | Reserved |   */
    /*     ---------------------------------- */
    uc_aifsn = puc_payload[0] & MAC_WMM_QOS_PARAM_AIFSN_MASK;
    en_acm = (puc_payload[0] & BIT4) ? OAL_TRUE : OAL_FALSE;
    uc_aci = (puc_payload[0] >> MAC_WMM_QOS_PARAM_ACI_BIT_OFFSET) & MAC_WMM_QOS_PARAM_ACI_MASK;

    /* ECWmin/ECWmax Field */
    /*     ------------------- */
    /* bit |   4    |   4    | */
    /*     ------------------- */
    /*     | ECWmin | ECWmax | */
    /*     ------------------- */
    uc_ecwmin = (puc_payload[1] & MAC_WMM_QOS_PARAM_ECWMIN_MASK);
    uc_ecwmax = ((puc_payload[1] & MAC_WMM_QOS_PARAM_ECWMAX_MASK) >> MAC_WMM_QOS_PARAM_ECWMAX_BIT_OFFSET);

    /* 在mib库中和寄存器里保存的TXOP值都是以us为单位的，但是传输的时候是以32us为
       单位进行传输的，因此在解析的时候需要将解析到的值乘以32
    */
    us_txop_limit = puc_payload[2] |
                    ((puc_payload[3] & MAC_WMM_QOS_PARAM_TXOPLIMIT_MASK) << MAC_WMM_QOS_PARAM_BIT_NUMS_OF_ONE_BYTE);
    us_txop_limit = (uint16_t)(us_txop_limit << MAC_WMM_QOS_PARAM_TXOPLIMIT_SAVE_TO_TRANS_TIMES);

    /* 更新相应的MIB库信息 */
    if (uc_aci < WLAN_WME_AC_BUTT) {
        /* 注: 协议规定取值1 2 3 4 */
        mac_mib_set_QAPEDCATableIndex(&pst_hmac_sta->st_vap_base_info, uc_aci, uc_aci + 1);
        mac_mib_set_QAPEDCATableCWmin(&pst_hmac_sta->st_vap_base_info, uc_aci, uc_ecwmin);
        mac_mib_set_QAPEDCATableCWmax(&pst_hmac_sta->st_vap_base_info, uc_aci, uc_ecwmax);
        mac_mib_set_QAPEDCATableAIFSN(&pst_hmac_sta->st_vap_base_info, uc_aci, uc_aifsn);
        mac_mib_set_QAPEDCATableTXOPLimit(&pst_hmac_sta->st_vap_base_info, uc_aci, us_txop_limit);
        mac_mib_set_QAPEDCATableMandatory(&pst_hmac_sta->st_vap_base_info, uc_aci, en_acm);
    }
}


void hmac_sta_up_update_edca_params(uint8_t *puc_payload,
                                            uint16_t us_msg_len,
                                            hmac_vap_stru *pst_hmac_sta,
                                            uint8_t uc_frame_sub_type,
                                            hmac_user_stru *pst_hmac_user)
{
    uint16_t us_msg_offset = 0;
    uint8_t uc_param_set_cnt;
    uint8_t uc_ac_num_loop;
    uint32_t ul_ret;
    mac_device_stru *pst_mac_device;
    uint8_t uc_edca_param_set;
    uint8_t *puc_ie = OAL_PTR_NULL;
#ifdef _PRE_WLAN_FEATURE_WMMAC
    oal_bool_enum_uint8 en_wmmac_auth_flag;
#endif

    pst_mac_device = (mac_device_stru *)mac_res_get_dev(pst_hmac_sta->st_vap_base_info.uc_device_id);
    if (pst_mac_device == OAL_PTR_NULL) {
        oam_warning_log1(0, OAM_SF_ASSOC,
                         "{hmac_sta_up_update_edca_params::mac_res_get_dev fail.device_id[%u]!}",
                         pst_hmac_sta->st_vap_base_info.uc_device_id);
        return;
    }

    /************************ WMM Parameter Element ***************************/
    /* ------------------------------------------------------------------------------ */
    /* | EID | LEN | OUI |OUI Type |OUI Subtype |Version |QoS Info |Resd |AC Params | */
    /* ------------------------------------------------------------------------------ */
    /* |  1  |  1  |  3  |    1    |     1      |    1   |    1    |  1  |    16    | */
    /* ------------------------------------------------------------------------------ */
    /******************* QoS Info field when sent from WMM AP *****************/
    /*        --------------------------------------------                    */
    /*          | Parameter Set Count | Reserved | U-APSD |                   */
    /*          --------------------------------------------                  */
    /*   bit    |        0~3          |   4~6    |   7    |                   */
    /*          --------------------------------------------                  */
    /**************************************************************************/
    puc_ie = mac_get_wmm_ie(puc_payload, us_msg_len);
    if (puc_ie != OAL_PTR_NULL) {
        /* 解析wmm ie是否携带EDCA参数 */
        uc_edca_param_set = puc_ie[MAC_OUISUBTYPE_WMM_PARAM_OFFSET];
        uc_param_set_cnt = puc_ie[HMAC_WMM_QOS_PARAMS_HDR_LEN] & 0x0F;

        /* 如果收到的是beacon帧，并且param_set_count没有改变，说明AP的WMM参数没有变
           则STA也不用做任何改变，直接返回即可 */
        if ((WLAN_FC0_SUBTYPE_BEACON == uc_frame_sub_type)
            && (uc_param_set_cnt == pst_hmac_sta->st_vap_base_info.uc_wmm_params_update_count)) {
            return;
        }

        pst_mac_device->en_wmm = OAL_TRUE;

        if (WLAN_FC0_SUBTYPE_BEACON == uc_frame_sub_type) {
            /* 保存QoS Info */
            mac_vap_set_wmm_params_update_count(&pst_hmac_sta->st_vap_base_info, uc_param_set_cnt);
        }

        if (puc_ie[HMAC_WMM_QOS_PARAMS_HDR_LEN] & BIT7) {
            mac_user_set_apsd(&(pst_hmac_user->st_user_base_info), OAL_TRUE);
        } else {
            mac_user_set_apsd(&(pst_hmac_user->st_user_base_info), OAL_FALSE);
        }

        us_msg_offset = (HMAC_WMM_QOSINFO_AND_RESV_LEN + HMAC_WMM_QOS_PARAMS_HDR_LEN);

        /* wmm ie中不携带edca参数 直接返回 */
        if (uc_edca_param_set != MAC_OUISUBTYPE_WMM_PARAM) {
            return;
        }

        /* 针对每一个AC，更新EDCA参数 */
        for (uc_ac_num_loop = 0; uc_ac_num_loop < WLAN_WME_AC_BUTT; uc_ac_num_loop++) {
            hmac_sta_up_update_edca_params_mib(pst_hmac_sta, &puc_ie[us_msg_offset]);
#ifdef _PRE_WLAN_FEATURE_WMMAC
            if (OAL_TRUE == mac_mib_get_QAPEDCATableMandatory(&(pst_hmac_sta->st_vap_base_info), uc_ac_num_loop)) {
                en_wmmac_auth_flag = (pst_hmac_sta->en_wmmac_auth_flag &&
                    uc_frame_sub_type == WLAN_FC0_SUBTYPE_REASSOC_RSP &&
                    pst_hmac_user->st_user_base_info.st_ts_info[uc_ac_num_loop].en_ts_status == MAC_TS_SUCCESS);
                if (en_wmmac_auth_flag) {
                    return;
                }
                pst_hmac_user->st_user_base_info.st_ts_info[uc_ac_num_loop].en_ts_status = MAC_TS_INIT;
                pst_hmac_user->st_user_base_info.st_ts_info[uc_ac_num_loop].uc_tsid = 0xFF;
            } else {
                pst_hmac_user->st_user_base_info.st_ts_info[uc_ac_num_loop].en_ts_status = MAC_TS_NONE;
                pst_hmac_user->st_user_base_info.st_ts_info[uc_ac_num_loop].uc_tsid = 0xFF;
            }

            oam_info_log2(pst_hmac_sta->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                          "{hmac_sta_up_update_edca_params::ac num[%d], ts status[%d].}",
                          uc_ac_num_loop, pst_hmac_user->st_user_base_info.st_ts_info[uc_ac_num_loop].en_ts_status);
#endif  // _PRE_WLAN_FEATURE_WMMAC
            us_msg_offset += HMAC_WMM_AC_PARAMS_RECORD_LEN;
        }

        hcc_host_update_vi_flowctl_param(
            mac_mib_get_QAPEDCATableCWmin(&pst_hmac_sta->st_vap_base_info, WLAN_WME_AC_BE),
            mac_mib_get_QAPEDCATableCWmin(&pst_hmac_sta->st_vap_base_info, WLAN_WME_AC_VI));

        /* 更新EDCA相关的MAC寄存器 */
        hmac_sta_up_update_edca_params_machw(pst_hmac_sta, MAC_WMM_SET_PARAM_TYPE_UPDATE_EDCA);

        return;
    }

    puc_ie = mac_find_ie(MAC_EID_HT_CAP, puc_payload, us_msg_len);
    if (puc_ie != OAL_PTR_NULL) {
        /* 再查找HT CAP能力第2字节BIT 5 short GI for 20M 能力位 */
        if ((puc_ie[1] >= 2) && (puc_ie[2] & BIT5))
        {
            mac_vap_init_wme_param(&pst_hmac_sta->st_vap_base_info);
            pst_mac_device->en_wmm = OAL_TRUE;
            hcc_host_update_vi_flowctl_param(
                mac_mib_get_QAPEDCATableCWmin(&pst_hmac_sta->st_vap_base_info, WLAN_WME_AC_BE),
                mac_mib_get_QAPEDCATableCWmin(&pst_hmac_sta->st_vap_base_info, WLAN_WME_AC_VI));
            /* 更新EDCA相关的MAC寄存器 */
            hmac_sta_up_update_edca_params_machw(pst_hmac_sta, MAC_WMM_SET_PARAM_TYPE_UPDATE_EDCA);

            return;
        }
    }

    if (WLAN_FC0_SUBTYPE_ASSOC_RSP == uc_frame_sub_type) {
        /* 当与STA关联的AP不是QoS的，STA会去使能EDCA寄存器，并默认利用VO级别发送数据 */
        ul_ret = hmac_sta_up_update_edca_params_machw(pst_hmac_sta, MAC_WMM_SET_PARAM_TYPE_DEFAULT);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log1(pst_hmac_sta->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                "{hmac_sta_up_update_edca_params::hmac_sta_up_update_edca_params_machw failed[%d].}", ul_ret);
        }

        pst_mac_device->en_wmm = OAL_FALSE;
    }
}

#ifdef _PRE_WLAN_FEATURE_TXOPPS

uint32_t hmac_sta_set_txopps_partial_aid(mac_vap_stru *pst_mac_vap)
{
    uint16_t us_temp_aid;
    uint8_t uc_temp_bssid;
    uint32_t ul_ret;
    mac_cfg_txop_sta_stru st_txop_info = { 0 };

    /* 此处需要注意:按照协议规定(802.11ac-2013.pdf,9.17a)，ap分配给sta的aid，不可以
       使计算出来的partial aid为0，后续如果ap支持的最大关联用户数目超过512，必须要
       对aid做这个合法性检查!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    */

    if (pst_mac_vap->en_protocol != WLAN_VHT_MODE && pst_mac_vap->en_protocol != WLAN_VHT_ONLY_MODE) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_TXOP, "{hmac_sta_set_txopps_partial_aid::VAP VHT unsupport}.");
        return OAL_SUCC;
    }

    /* 计算partial aid */
    if (OAL_TRUE != mac_vap_get_txopps(pst_mac_vap)) {
        st_txop_info.us_partial_aid = 0;
    } else {
        us_temp_aid = pst_mac_vap->us_sta_aid & 0x1FF;
        uc_temp_bssid = (pst_mac_vap->auc_bssid[5] & 0x0F) ^ ((pst_mac_vap->auc_bssid[5] & 0xF0) >> 4);
        st_txop_info.us_partial_aid = (us_temp_aid + (uc_temp_bssid << 5)) & ((1 << 9) - 1);
    }

    /***************************************************************************
    抛事件到DMAC层, 同步DMAC数据
    ***************************************************************************/
    ul_ret = hmac_config_send_event(pst_mac_vap, WLAN_CFGID_STA_TXOP_AID,
                                        OAL_SIZEOF(mac_cfg_txop_sta_stru), (uint8_t *)&st_txop_info);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_TXOP,
                         "{hmac_sta_set_txopps_partial_aid::hmac_config_send_event failed[%d].}", ul_ret);
    }

    return OAL_SUCC;
}
#endif


void hmac_sta_update_mac_user_info(hmac_user_stru *pst_hmac_user_ap, uint16_t us_user_idx)
{
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    mac_user_stru *pst_mac_user_ap = OAL_PTR_NULL;
    uint32_t ul_ret;

    if (pst_hmac_user_ap == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_RX, "{hmac_sta_update_mac_user_info::param null.}");
        return;
    }

    pst_mac_vap = (mac_vap_stru *)mac_res_get_mac_vap(pst_hmac_user_ap->st_user_base_info.uc_vap_id);
    if (oal_unlikely(pst_mac_vap == OAL_PTR_NULL)) {
        oam_error_log1(0, OAM_SF_RX, "{hmac_sta_update_mac_user_info::get mac_vap [vap_id:%d] null.}",
                       pst_hmac_user_ap->st_user_base_info.uc_vap_id);
        return;
    }

    pst_mac_user_ap = &(pst_hmac_user_ap->st_user_base_info);

    oam_warning_log3(pst_mac_vap->uc_vap_id, OAM_SF_RX,
                     "{hmac_sta_update_mac_user_info::us_user_idx:%d,en_avail_bandwidth:%d,en_cur_bandwidth:%d}",
                     us_user_idx,
                     pst_mac_user_ap->en_avail_bandwidth,
                     pst_mac_user_ap->en_cur_bandwidth);

    ul_ret = hmac_config_user_info_syn(pst_mac_vap, pst_mac_user_ap);
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_RX,
                       "{hmac_sta_update_mac_user_info::hmac_config_user_info_syn failed[%d].}", ul_ret);
    }

    ul_ret = hmac_config_user_rate_info_syn(pst_mac_vap, pst_mac_user_ap);
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_RX,
                       "{hmac_sta_wait_asoc_rx::hmac_syn_rate_info failed[%d].}", ul_ret);
    }
    return;
}


uint8_t *hmac_sta_find_ie_in_probe_rsp(mac_vap_stru *pst_mac_vap, uint8_t uc_eid, uint16_t *pus_index)
{
    mac_bss_dscr_stru *pst_bss_dscr = OAL_PTR_NULL;
    uint8_t *puc_ie = OAL_PTR_NULL;
    uint8_t *puc_payload = OAL_PTR_NULL;
    uint8_t us_offset;

    if (pst_mac_vap == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_SCAN, "{find ie fail, pst_mac_vap is null.}");
        return OAL_PTR_NULL;
    }

    pst_bss_dscr = (mac_bss_dscr_stru *)hmac_scan_get_scanned_bss_by_bssid(pst_mac_vap, pst_mac_vap->auc_bssid);

    if (pst_bss_dscr == OAL_PTR_NULL) {
        oam_warning_log4(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
                         "hmac_sta_find_ie_in_probe_rsp::find the bss failed by bssid:%02X:XX:XX:%02X:%02X:%02X",
                         pst_mac_vap->auc_bssid[0],
                         pst_mac_vap->auc_bssid[3],
                         pst_mac_vap->auc_bssid[4],
                         pst_mac_vap->auc_bssid[5]);

        return OAL_PTR_NULL;
    }

    /* 以IE开头的payload，返回供调用者使用 */
    us_offset = MAC_80211_FRAME_LEN + MAC_TIME_STAMP_LEN + MAC_BEACON_INTERVAL_LEN + MAC_CAP_INFO_LEN;
    /*lint -e416*/
    puc_payload = (uint8_t *)(pst_bss_dscr->auc_mgmt_buff + us_offset);
    /*lint +e416*/
    if (pst_bss_dscr->ul_mgmt_len < us_offset) {
        return OAL_PTR_NULL;
    }

    puc_ie = mac_find_ie(uc_eid, puc_payload, (int32_t)(pst_bss_dscr->ul_mgmt_len - us_offset));
    if (puc_ie == OAL_PTR_NULL) {
        return OAL_PTR_NULL;
    }

    /* IE长度初步校验 */
    if (*(puc_ie + 1) == 0) {
        oam_warning_log1(0, OAM_SF_ANY, "{IE[%d] len in probe rsp is 0, find ie fail.}", uc_eid);
        return OAL_PTR_NULL;
    }

    *pus_index = (uint16_t)(puc_ie - puc_payload);

    oam_warning_log1(0, OAM_SF_ANY, "{found ie[%d] in probe rsp.}", uc_eid);

    return puc_payload;
}


void hmac_sta_check_ht_cap_ie(mac_vap_stru *pst_mac_sta,
                                  uint8_t *puc_payload,
                                  mac_user_stru *pst_mac_user_ap,
                                  uint16_t *pus_amsdu_maxsize,
                                  uint16_t us_payload_len)
{
    uint8_t *puc_ie = OAL_PTR_NULL;
    uint8_t *puc_payload_for_ht_cap_chk = OAL_PTR_NULL;
    uint16_t us_ht_cap_index;

    if (oal_any_null_ptr3(pst_mac_sta, puc_payload, pst_mac_user_ap)) {
        return;
    }

    puc_ie = mac_find_ie(MAC_EID_HT_CAP, puc_payload, us_payload_len);
    if (puc_ie == OAL_PTR_NULL || puc_ie[1] < MAC_HT_CAP_LEN) {
        puc_payload_for_ht_cap_chk = hmac_sta_find_ie_in_probe_rsp(pst_mac_sta, MAC_EID_HT_CAP, &us_ht_cap_index);
        if (puc_payload_for_ht_cap_chk == OAL_PTR_NULL) {
            oam_warning_log0(0, OAM_SF_ANY, "{hmac_sta_check_ht_cap_ie::puc_payload_for_ht_cap_chk is null.}");
            return;
        }

        /*lint -e413*/
        if (puc_payload_for_ht_cap_chk[us_ht_cap_index + 1] < MAC_HT_CAP_LEN) {
            oam_warning_log1(0, OAM_SF_ANY, "{hmac_sta_check_ht_cap_ie::invalid ht cap len[%d].}",
                             puc_payload_for_ht_cap_chk[us_ht_cap_index + 1]);
            return;
        }
        /*lint +e413*/
        puc_ie = puc_payload_for_ht_cap_chk + us_ht_cap_index; /* 赋值HT CAP IE */
    } else {
        if (puc_ie < puc_payload) {
            return;
        }
        us_ht_cap_index = (uint16_t)(puc_ie - puc_payload);
        puc_payload_for_ht_cap_chk = puc_payload;
    }

    if ((WLAN_BAND_2G == pst_mac_sta->st_channel.en_band) && IS_INVALID_HT_RATE_HP(puc_ie)) {
        oam_warning_log0(pst_mac_sta->uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_sta_update_ht_cap:: invalid mcs set, disable HT.}");
        mac_user_set_ht_capable(pst_mac_user_ap, OAL_FALSE);
        return;
    }

    /* 根据协议值设置特性，必须在hmac_amsdu_init_user后面调用 */
    mac_ie_proc_ht_sta(pst_mac_sta, puc_payload_for_ht_cap_chk, us_ht_cap_index,
                           pst_mac_user_ap, pus_amsdu_maxsize);
}


void hmac_sta_check_ext_cap_ie(mac_vap_stru *pst_mac_sta,
                                   mac_user_stru *pst_mac_user_ap,
                                   uint8_t *puc_payload,
                                   uint16_t us_rx_len)
{
    uint8_t *puc_ie;
    uint8_t *puc_payload_proc = OAL_PTR_NULL;
    uint16_t us_index;

    puc_ie = mac_find_ie(MAC_EID_EXT_CAPS, puc_payload, us_rx_len);
    if (puc_ie == OAL_PTR_NULL || puc_ie[1] < MAC_MIN_XCAPS_LEN) {
        puc_payload_proc = hmac_sta_find_ie_in_probe_rsp(pst_mac_sta, MAC_EID_EXT_CAPS, &us_index);
        if (puc_payload_proc == OAL_PTR_NULL) {
            return;
        }

        /*lint -e413*/
        if (puc_payload_proc[us_index + 1] < MAC_MIN_XCAPS_LEN) {
            oam_warning_log1(0, OAM_SF_ANY, "{hmac_sta_check_ext_cap_ie::invalid ext cap len[%d].}",
                             puc_payload_proc[us_index + 1]);
            return;
        }
        /*lint +e413*/
    } else {
        puc_payload_proc = puc_payload;
        if (puc_ie < puc_payload) {
            return;
        }

        us_index = (uint16_t)(puc_ie - puc_payload);
    }

    /* 处理 Extended Capabilities IE */
    /*lint -e613*/
    mac_ie_proc_ext_cap_ie(pst_mac_user_ap, &puc_payload_proc[us_index]);
    /*lint +e613*/
}


uint32_t hmac_sta_check_ht_opern_ie(mac_vap_stru *pst_mac_sta,
                                      mac_user_stru *pst_mac_user_ap,
                                      uint8_t *puc_payload,
                                      uint16_t us_rx_len)
{
    uint8_t *puc_ie;
    uint8_t *puc_payload_proc = OAL_PTR_NULL;
    uint16_t us_index;
    uint32_t ul_change = MAC_NO_CHANGE;

    puc_ie = mac_find_ie(MAC_EID_HT_OPERATION, puc_payload, us_rx_len);
    if (puc_ie == OAL_PTR_NULL || puc_ie[1] < MAC_HT_OPERN_LEN) {
        puc_payload_proc = hmac_sta_find_ie_in_probe_rsp(pst_mac_sta, MAC_EID_HT_OPERATION, &us_index);
        if (puc_payload_proc == OAL_PTR_NULL) {
            return ul_change;
        }

        /*lint -e413*/
        if (puc_payload_proc[us_index + 1] < MAC_HT_OPERN_LEN) {
            oam_warning_log1(0, OAM_SF_ANY, "{hmac_sta_check_ht_opern_ie::invalid ht cap len[%d].}",
                             puc_payload_proc[us_index + 1]);
            return ul_change;
        }
        /*lint +e413*/
    } else {
        puc_payload_proc = puc_payload;
        if (puc_ie < puc_payload) {
            return ul_change;
        }

        us_index = (uint16_t)(puc_ie - puc_payload);
    }

    ul_change |= mac_proc_ht_opern_ie(pst_mac_sta, &puc_payload_proc[us_index], pst_mac_user_ap);

    return ul_change;
}


uint32_t hmac_mgmt_assoc_rsp_check_ht_sta(mac_vap_stru *pst_mac_sta,
                                uint8_t *puc_payload,
                                uint16_t us_rx_len,
                                hmac_user_stru *p_hmac_user_ap,
                                uint16_t *pus_amsdu_maxsize)
{
    uint32_t ul_change = MAC_NO_CHANGE;

    if (oal_any_null_ptr3(pst_mac_sta, puc_payload, p_hmac_user_ap)) {
        return ul_change;
    }

    hmac_ht_self_cure_event_set(pst_mac_sta, p_hmac_user_ap->st_user_base_info.auc_user_mac_addr,
        HMAC_HT_SELF_CURE_EVENT_ASSOC);
    hmac_user_set_close_ht_flag(pst_mac_sta, p_hmac_user_ap);

    /* 初始化HT cap为FALSE，入网时会把本地能力跟随AP能力 */
    mac_user_set_ht_capable(&p_hmac_user_ap->st_user_base_info, OAL_FALSE);

    /* 至少支持11n才进行后续的处理 */
    if (OAL_FALSE == mac_mib_get_HighThroughputOptionImplemented(pst_mac_sta)) {
        return ul_change;
    }

    hmac_sta_check_ht_cap_ie(pst_mac_sta, puc_payload, &p_hmac_user_ap->st_user_base_info,
        pus_amsdu_maxsize, us_rx_len);
    /* sta处理AP的 Extended Capability */
    hmac_sta_check_ext_cap_ie(pst_mac_sta, &p_hmac_user_ap->st_user_base_info, puc_payload, us_rx_len);

    ul_change = hmac_sta_check_ht_opern_ie(pst_mac_sta, &p_hmac_user_ap->st_user_base_info, puc_payload, us_rx_len);

    if (p_hmac_user_ap->st_user_base_info.st_vht_hdl.en_vht_capable == OAL_FALSE) {
        hmac_user_set_amsdu_level(p_hmac_user_ap, WLAN_TX_AMSDU_BY_2);
    }

    return ul_change;
}



OAL_STATIC void hmac_1024qam_judge(hmac_user_stru *pst_hmac_user, uint8_t *puc_payload, uint16_t us_msg_len)
{
    uint8_t *puc_tmp_ie = OAL_PTR_NULL;
    puc_tmp_ie = mac_find_vendor_ie(MAC_HUAWEI_VENDER_IE, MAC_HISI_1024QAM_IE, puc_payload, us_msg_len);
    if (puc_tmp_ie != OAL_PTR_NULL) {
        pst_hmac_user->st_user_base_info.st_cap_info.bit_1024qam_cap = OAL_TRUE;
        pst_hmac_user->st_user_base_info.st_cap_info.bit_20mmcs11_compatible_1103 = OAL_TRUE;
        pst_hmac_user->user_hisi = OAL_TRUE; /* 支持1024qam且可获得MAC_HUAWEI_VENDER_IE的为hisi芯片 */
        return;
    }
    pst_hmac_user->user_hisi = OAL_FALSE;

    if (get_hi110x_subchip_type() != BOARD_VERSION_HI1103) {
        /* BCM私有IE的1024QAM */
        puc_tmp_ie = mac_find_vendor_ie(MAC_WLAN_OUI_BROADCOM_EPIGRAM, MAC_WLAN_OUI_VENDOR_VHT_TYPE,
                                            puc_payload, us_msg_len);
        if ((puc_tmp_ie != OAL_PTR_NULL) && (puc_tmp_ie[1] >= MAC_WLAN_OUI_VENDOR_VHT_HEADER)) {
            if ((puc_tmp_ie[6] == MAC_WLAN_OUI_VENDOR_VHT_SUBTYPE2)
                || (puc_tmp_ie[6] == MAC_WLAN_OUI_VENDOR_VHT_SUBTYPE4)) {
                pst_hmac_user->st_user_base_info.st_cap_info.bit_1024qam_cap = OAL_TRUE;
                pst_hmac_user->st_user_base_info.st_cap_info.bit_20mmcs11_compatible_1103 = OAL_FALSE;
            }
        }
    }
}

#ifdef _PRE_WLAN_FEATURE_11AX
uint32_t hmac_proc_he_uora_para_update(mac_vap_stru *mac_vap, uint8_t uora_para_change)
{
    uint32_t ret;

    ret = hmac_config_send_event(mac_vap, WLAN_CFGID_HE_UORA_OCW_UPDATE,
                                     OAL_SIZEOF(uint8_t), (uint8_t *)&uora_para_change);
    if (ret != OAL_SUCC) {
        oam_error_log1(mac_vap->uc_vap_id, OAM_SF_11AX,
                       "{hmac_proc_he_uora_para_update::send event[WLAN_CFGID_UORA_OCW_UPDATE] failed[%d].}", ret);
    }

    return ret;
}

OAL_STATIC uint32_t hmac_ie_check_he_sta(hmac_vap_stru *pst_hmac_sta,
                                           hmac_user_stru *pst_hmac_user,
                                           uint8_t *puc_payload,
                                           uint16_t us_msg_len)
{
    mac_user_stru *pst_mac_user;
    mac_vap_stru *pst_mac_vap;
    mac_he_hdl_stru *pst_he_hdl;
    uint8_t *puc_tmp_ie = OAL_PTR_NULL;
    uint32_t ul_change = 0;
    oal_bool_enum_uint8 uora_para_change;

    pst_mac_user = &(pst_hmac_user->st_user_base_info);
    pst_mac_vap  = &(pst_hmac_sta->st_vap_base_info);

    pst_he_hdl = MAC_USER_HE_HDL_STRU(pst_mac_user);
    memset_s(pst_he_hdl, OAL_SIZEOF(mac_he_hdl_stru), 0, OAL_SIZEOF(mac_he_hdl_stru));
    puc_tmp_ie = mac_find_ie_ext_ie(MAC_EID_HE, MAC_EID_EXT_HE_CAP, puc_payload, us_msg_len);
    if (puc_tmp_ie != OAL_PTR_NULL) {
        hmac_proc_he_cap_ie(pst_mac_vap, pst_hmac_user, puc_tmp_ie);
    }

    puc_tmp_ie = mac_find_ie_ext_ie(MAC_EID_HE, MAC_EID_EXT_HE_OPERATION, puc_payload, us_msg_len);
    if (puc_tmp_ie != OAL_PTR_NULL) {
        ul_change |= hmac_update_he_opern_ie_sta(pst_mac_vap, pst_hmac_user, puc_tmp_ie);
    }

    /* 解析BSS Coloe change announcement IE  */
    puc_tmp_ie = mac_find_ie_ext_ie(MAC_EID_HE, MAC_EID_EXT_HE_BSS_COLOR_CHANGE_ANNOUNCEMENT,
                                    puc_payload, us_msg_len);
    if (puc_tmp_ie != OAL_PTR_NULL) {
        hmac_proc_he_bss_color_change_announcement_ie(pst_mac_vap, pst_hmac_user, puc_tmp_ie);
    }

    uora_para_change = mac_proc_he_uora_parameter_set_ie(pst_mac_vap, puc_payload, us_msg_len);
    if (uora_para_change == OAL_TRUE) {
        hmac_proc_he_uora_para_update(pst_mac_vap, OAL_TRUE);
    }

    return ul_change;
}
#endif

OAL_STATIC void hmac_proc_asoc_rsp_set_sta_ampdu_active(mac_vap_stru *pst_mac_vap,
    hmac_vap_stru *pst_hmac_sta, hmac_user_stru *pst_hmac_user)
{
    if (pst_mac_vap->st_channel.en_band == WLAN_BAND_2G) {
#ifdef _PRE_WLAN_FEATURE_11AX
        if (pst_hmac_user->st_user_base_info.en_protocol_mode == WLAN_HE_MODE) {
            pst_hmac_sta->bit_rx_ampduplusamsdu_active = !(g_uc_host_rx_ampdu_amsdu & (BIT2));
        } else
#endif
        {
            pst_hmac_sta->bit_rx_ampduplusamsdu_active = g_uc_host_rx_ampdu_amsdu & BIT1;
        }
    } else {
        pst_hmac_sta->bit_rx_ampduplusamsdu_active = g_uc_host_rx_ampdu_amsdu & BIT0;
    }
}

OAL_STATIC void hmac_process_assoc_rsp_bw_change_info(mac_vap_stru *pst_mac_vap,
    hmac_vap_stru *pst_hmac_sta, hmac_user_stru *pst_hmac_user, uint32_t ul_change)
{
    if (MAC_BW_CHANGE & ul_change) {
        /* 获取用户与VAP带宽能力交集,通知硬件切带宽 */
        hmac_chan_sync(pst_mac_vap, pst_mac_vap->st_channel.uc_chan_number, pst_mac_vap->st_channel.en_bandwidth,
                           OAL_TRUE);
        oam_warning_log4(pst_hmac_sta->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_process_assoc_rsp:change BW. change[0x%x], vap band[%d] chnl[%d], bandwidth[%d].}",
                         ul_change, pst_mac_vap->st_channel.en_band, pst_mac_vap->st_channel.uc_chan_number,
                         MAC_VAP_GET_CAP_BW(pst_mac_vap));
        oam_warning_log4(pst_hmac_sta->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_process_assoc_rsp:mac usr id[%d] bw cap[%d]avil[%d]cur[%d].}",
                         pst_hmac_user->st_user_base_info.us_assoc_id,
                         pst_hmac_user->st_user_base_info.en_bandwidth_cap,
                         pst_hmac_user->st_user_base_info.en_avail_bandwidth,
                         pst_hmac_user->st_user_base_info.en_cur_bandwidth);
    }
}


uint32_t hmac_process_assoc_rsp(hmac_vap_stru *pst_hmac_sta, hmac_user_stru *pst_hmac_user,
    uint8_t *puc_mac_hdr, uint16_t us_hdr_len, uint8_t *puc_payload, uint16_t us_msg_len)
{
    uint32_t ul_rslt;
    uint16_t us_aid;
    uint8_t *puc_tmp_ie = OAL_PTR_NULL;
    uint32_t ul_ret;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    mac_user_stru *pst_mac_user = OAL_PTR_NULL;
    uint32_t ul_change = MAC_NO_CHANGE;
    wlan_channel_bandwidth_enum_uint8 en_sta_new_bandwidth;
    uint8_t uc_avail_mode;
#ifdef _PRE_WLAN_FEATURE_11AC2G
    oal_bool_enum_uint8 en_avail_mode_is_vht;
#endif

    if (pst_hmac_sta == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ASSOC, "{hmac_process_assoc_rsp::pst_hmac_sta null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_mac_vap = &(pst_hmac_sta->st_vap_base_info);
    pst_mac_user = &(pst_hmac_user->st_user_base_info);

    /* 更新关联ID */
    us_aid = mac_get_asoc_id(puc_payload);
    if ((us_aid > 0) && (us_aid <= 2007)) {
        mac_vap_set_aid(pst_mac_vap, us_aid);
    } else {
        oam_warning_log1(pst_hmac_sta->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_process_assoc_rsp::invalid us_sta_aid[%d].}", us_aid);
    }

    puc_payload += (MAC_CAP_INFO_LEN + MAC_STATUS_CODE_LEN + MAC_AID_LEN);
    us_msg_len -= (MAC_CAP_INFO_LEN + MAC_STATUS_CODE_LEN + MAC_AID_LEN);

    /* 初始化安全端口过滤参数 */
    ul_rslt = hmac_init_user_security_port(&(pst_hmac_sta->st_vap_base_info), &(pst_hmac_user->st_user_base_info));
    if (ul_rslt != OAL_SUCC) {
        oam_error_log1(pst_hmac_sta->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                       "{hmac_process_assoc_rsp::hmac_init_user_security_port failed[%d].}", ul_rslt);
    }

#if (_PRE_WLAN_FEATURE_PMF != _PRE_PMF_NOT_SUPPORT)
    /* STA模式下的pmf能力来源于WPA_supplicant，只有启动pmf和不启动pmf两种类型 */
    mac_user_set_pmf_active(&pst_hmac_user->st_user_base_info, pst_mac_vap->en_user_pmf_cap);
    oam_warning_log1(pst_hmac_sta->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                     "{hmac_process_assoc_rsp::set user pmf[%d].}", pst_mac_vap->en_user_pmf_cap);
#endif /* #if(_PRE_WLAN_FEATURE_PMF != _PRE_PMF_NOT_SUPPORT) */

#ifdef _PRE_WLAN_FEATURE_11AX
    if (g_wlan_spec_cfg->feature_11ax_is_open) {
        hmac_sta_up_update_assoc_rsp_sr_params(puc_payload, us_msg_len, pst_hmac_sta,
                                               mac_get_frame_sub_type(puc_mac_hdr), pst_hmac_user);
        hmac_sta_up_update_he_nfrp_params(puc_payload, us_msg_len, pst_hmac_sta,
            mac_get_frame_sub_type(puc_mac_hdr), pst_hmac_user);
    }
#endif

    /* 更新关联用户的 QoS protocol table */
    hmac_mgmt_update_assoc_user_qos_table(puc_payload, us_msg_len, pst_hmac_user);

    /* 更新关联用户的legacy速率集合 */
    hmac_user_init_rates(pst_hmac_user);
    hmac_ie_proc_assoc_user_legacy_rate(puc_payload, us_msg_len, pst_hmac_user);

#ifdef _PRE_WLAN_FEATURE_TXBF
    /* 更新11n txbf能力 */
    puc_tmp_ie = mac_find_vendor_ie(MAC_HUAWEI_VENDER_IE, MAC_EID_11NTXBF, puc_payload, us_msg_len);
    hmac_mgmt_update_11ntxbf_cap(puc_tmp_ie, pst_hmac_user);
#endif
    hmac_user_cap_update_by_hisi_cap_ie(pst_hmac_user, puc_payload, us_msg_len);
#ifdef _PRE_WLAN_FEATURE_11AX
    /* 更新 11ax HE Capabilities ie */
    if (g_wlan_spec_cfg->feature_11ax_is_open) {
        ul_change |= hmac_ie_check_he_sta(pst_hmac_sta, pst_hmac_user, puc_payload, us_msg_len);
    }
#endif

    /* 更新11ac VHT capabilities ie */
    memset_s(&(pst_hmac_user->st_user_base_info.st_vht_hdl), OAL_SIZEOF(mac_vht_hdl_stru),
             0, OAL_SIZEOF(mac_vht_hdl_stru));
    puc_tmp_ie = mac_find_ie(MAC_EID_VHT_CAP, puc_payload, us_msg_len);
    if (puc_tmp_ie != OAL_PTR_NULL) {
        hmac_proc_vht_cap_ie(pst_mac_vap, pst_hmac_user, puc_tmp_ie);
#ifdef _PRE_WLAN_FEATURE_1024QAM
        hmac_1024qam_judge(pst_hmac_user, puc_payload, us_msg_len);

#endif
    }

#ifdef _PRE_WLAN_FEATURE_VIRTUAL_MULTI_STA
    /* vap开启4地址能力并调用check接口校验用户是否支持4地址 是则将用户的4地址传输能力位置1 */
    if (pst_hmac_sta->st_wds_table.en_wds_vap_mode == WDS_MODE_REPEATER_STA) {
        if (hmac_vmsta_check_user_a4_support(puc_mac_hdr, us_hdr_len + us_msg_len)) {
            pst_hmac_user->uc_is_wds = OAL_TRUE;
            oam_warning_log0(pst_hmac_user->st_user_base_info.uc_vap_id, OAM_SF_ASSOC,
                             "{hmac_ap_up_rx_asoc_req::user surpport 4 address.}");
        }
    }
#endif  // _PRE_WLAN_FEATURE_VIRTUAL_MULTI_STA

    /* 更新11ac VHT operation ie */
    puc_tmp_ie = mac_find_ie(MAC_EID_VHT_OPERN, puc_payload, us_msg_len);
    if (puc_tmp_ie != OAL_PTR_NULL) {
        ul_change |= hmac_update_vht_opern_ie_sta(pst_mac_vap, pst_hmac_user, puc_tmp_ie);
    }

    /* 更新 HT 参数  以及 EXTEND CAPABILITY */
    memset_s(&(pst_hmac_user->st_user_base_info.st_ht_hdl), OAL_SIZEOF(mac_user_ht_hdl_stru),
        0, OAL_SIZEOF(mac_user_ht_hdl_stru));
    ul_change |= hmac_mgmt_assoc_rsp_check_ht_sta(&pst_hmac_sta->st_vap_base_info, puc_payload, us_msg_len,
        pst_hmac_user, &pst_hmac_user->us_amsdu_maxsize);

    if (OAL_TRUE == hmac_user_ht_support(pst_hmac_user)) {
        /* 评估是否需要进行带宽切换 */
        /* 根据BRCM VENDOR OUI 适配2G 11AC */
        if (pst_hmac_user->st_user_base_info.st_vht_hdl.en_vht_capable == OAL_FALSE) {
            uint8_t *puc_vendor_vht_ie;
            uint32_t ul_vendor_vht_ie_offset = MAC_WLAN_OUI_VENDOR_VHT_HEADER + MAC_IE_HDR_LEN;
            puc_vendor_vht_ie = mac_find_vendor_ie(MAC_WLAN_OUI_BROADCOM_EPIGRAM,
                                                       MAC_WLAN_OUI_VENDOR_VHT_TYPE,
                                                       puc_payload, us_msg_len);

            if ((puc_vendor_vht_ie != OAL_PTR_NULL) && (puc_vendor_vht_ie[1] >= ul_vendor_vht_ie_offset)) {
                oam_warning_log0(pst_hmac_sta->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                    "{hmac_process_assoc_rsp::find broadcom/epigram vendor ie, enable hidden bit_11ac2g}");

                /* 进入此函数代表user支持2G 11ac */
                puc_tmp_ie = mac_find_ie(MAC_EID_VHT_CAP, puc_vendor_vht_ie + ul_vendor_vht_ie_offset,
                                             (int32_t)(puc_vendor_vht_ie[1] - MAC_WLAN_OUI_VENDOR_VHT_HEADER));
                if (puc_tmp_ie != OAL_PTR_NULL) {
                    hmac_proc_vht_cap_ie(pst_mac_vap, pst_hmac_user, puc_tmp_ie);
                }

                /* 更新11ac VHT operation ie */
                puc_tmp_ie = mac_find_ie(MAC_EID_VHT_OPERN, puc_vendor_vht_ie + ul_vendor_vht_ie_offset,
                                             (int32_t)(puc_vendor_vht_ie[1] - MAC_WLAN_OUI_VENDOR_VHT_HEADER));
                if (puc_tmp_ie != OAL_PTR_NULL) {
                    ul_change |= hmac_update_vht_opern_ie_sta(pst_mac_vap, pst_hmac_user, puc_tmp_ie);
                }
            }
        }
    }
#ifdef _PRE_WLAN_FEATURE_HISTREAM
    /* 更新histream能力 */
    pst_hmac_user->st_user_base_info.st_cap_info.bit_histream_cap = OAL_FALSE;
    puc_tmp_ie = mac_find_vendor_ie(MAC_HUAWEI_VENDER_IE, MAC_HISI_HISTREAM_IE, puc_payload, us_msg_len);
    if (puc_tmp_ie != OAL_PTR_NULL) {
        pst_hmac_user->st_user_base_info.st_cap_info.bit_histream_cap = OAL_TRUE;
    }
#endif  // _PRE_WLAN_FEATURE_HISTREAM

#ifdef _PRE_WLAN_FEATURE_HIEX
    if (g_wlan_spec_cfg->feature_hiex_is_open) {
        hmac_rx_assoc_rsp_parse_tb_frame_gain(puc_payload, us_msg_len);
        ul_change |= hmac_hiex_rx_assoc_rsp(pst_hmac_sta, pst_hmac_user, puc_payload, us_msg_len);
    }
#endif

    /* 获取用户的协议模式 */
    hmac_set_user_protocol_mode(pst_mac_vap, pst_hmac_user);

    uc_avail_mode =
        g_auc_avail_protocol_mode[pst_mac_vap->en_protocol][pst_hmac_user->st_user_base_info.en_protocol_mode];
#ifdef _PRE_WLAN_FEATURE_11AC2G
    en_avail_mode_is_vht = ((pst_mac_vap->en_protocol == WLAN_HT_MODE) &&
        (pst_hmac_user->st_user_base_info.en_protocol_mode == WLAN_VHT_MODE) &&
        (pst_mac_vap->st_cap_flag.bit_11ac2g == OAL_TRUE) && (pst_mac_vap->st_channel.en_band == WLAN_BAND_2G));
    if (en_avail_mode_is_vht) {
        uc_avail_mode = WLAN_VHT_MODE;
    }
#endif

    hmac_proc_asoc_rsp_set_sta_ampdu_active(pst_mac_vap, pst_hmac_sta, pst_hmac_user);
    /* 获取用户与VAP协议模式交集 */
    mac_user_set_avail_protocol_mode(&pst_hmac_user->st_user_base_info, uc_avail_mode);
    mac_user_set_cur_protocol_mode(&pst_hmac_user->st_user_base_info,
                                       pst_hmac_user->st_user_base_info.en_avail_protocol_mode);

#ifdef _PRE_WLAN_FEATURE_11AX
    if (g_wlan_spec_cfg->feature_11ax_is_open) {
        if (pst_hmac_user->st_user_base_info.en_cur_protocol_mode == WLAN_HE_MODE) {
            pst_hmac_user->uc_tx_ba_limit = DMAC_UCAST_FRAME_TX_COMP_TIMES_HE;
        }
    }
#endif

    /* 获取用户和VAP 可支持的11a/b/g 速率交集 */
    hmac_vap_set_user_avail_rates(&(pst_hmac_sta->st_vap_base_info), pst_hmac_user);

    /* 获取用户与VAP空间流交集 */
    ul_ret = hmac_user_set_avail_num_space_stream(pst_mac_user, pst_mac_vap->en_vap_rx_nss);
    oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC, "{hmac_process_assoc_rsp:ap max nss is [%d]}",
                     pst_hmac_user->st_user_base_info.en_user_max_cap_nss);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_process_assoc_rsp::mac_user_set_avail_num_space_stream failed[%d].}", ul_ret);
    }

    /* 获取用户和VAP带宽，并判断是否有带宽变化需要通知硬件切带宽 */
    en_sta_new_bandwidth = mac_vap_get_ap_usr_opern_bandwidth(pst_mac_vap, &(pst_hmac_user->st_user_base_info));
    ul_change |= mac_vap_set_bw_check(pst_mac_vap, en_sta_new_bandwidth);

    /* 同步用户的带宽能力 */
    mac_user_set_bandwidth_info(pst_mac_user, mac_vap_bw_mode_to_bw(en_sta_new_bandwidth),
                                    mac_vap_bw_mode_to_bw(en_sta_new_bandwidth));

    /* 获取用户160M带宽下的空间流 */
    hmac_user_set_num_spatial_stream_160M(pst_mac_user);

#ifdef _PRE_WLAN_FEATURE_SMPS
    /* 根据smps更新空间流能力 */
    if (!IS_VAP_SINGLE_NSS(pst_mac_vap) && !IS_USER_SINGLE_NSS(pst_mac_user)) {
        hmac_smps_update_user_status(pst_mac_vap, pst_mac_user);
    }
#endif

#ifdef _PRE_WLAN_FEATURE_OPMODE_NOTIFY
    /* 处理Operating Mode Notification 信息元素 */
    ul_ret = hmac_check_opmode_notify(pst_hmac_sta, puc_mac_hdr, puc_payload, us_msg_len, pst_hmac_user);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_process_assoc_rsp::hmac_check_opmode_notify failed[%d].}", ul_ret);
    }
#endif

    mac_user_set_asoc_state(&pst_hmac_user->st_user_base_info, MAC_USER_STATE_ASSOC);

    /* 同步空间流信息 */
    ul_ret = hmac_config_user_num_spatial_stream_cap_syn(pst_mac_vap, pst_mac_user);
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC,
                       "{hmac_process_assoc_rsp::hmac_config_user_num_spatial_stream_cap_syn failed[%d].}", ul_ret);
    }

    /* dmac offload架构下，同步STA USR信息到dmac */
    ul_ret = hmac_config_user_cap_syn(&(pst_hmac_sta->st_vap_base_info), &pst_hmac_user->st_user_base_info);

    ul_ret = hmac_config_user_info_syn(&(pst_hmac_sta->st_vap_base_info), &pst_hmac_user->st_user_base_info);

    ul_ret = hmac_config_user_rate_info_syn(&(pst_hmac_sta->st_vap_base_info), &pst_hmac_user->st_user_base_info);

    /* dmac offload架构下，同步STA USR信息到dmac */
    ul_ret = hmac_config_sta_vap_info_syn(&(pst_hmac_sta->st_vap_base_info));

    /* edac 设置放在关联状态设置完成之后，便于知道需要设置的队列ID,sta更新自身的edca parameters, assoc rsp帧是管理帧 */
    hmac_sta_up_update_edca_params(puc_payload, us_msg_len, pst_hmac_sta,
                                       mac_get_frame_sub_type(puc_mac_hdr), pst_hmac_user);

    hmac_arp_probe_type_set(pst_mac_vap, OAL_TRUE, HMAC_VAP_ARP_PROBE_TYPE_HTC);
#ifdef _PRE_WLAN_FEATURE_11AX
    if (g_wlan_spec_cfg->feature_11ax_is_open) {
        hmac_sta_up_update_he_edca_params(puc_payload, us_msg_len, pst_hmac_sta,
                                          mac_get_frame_sub_type(puc_mac_hdr), pst_hmac_user);
    }
#endif

#ifdef _PRE_WLAN_FEATURE_TXOPPS
    /* sta计算自身的partial aid，写入到mac寄存器 */
    hmac_sta_set_txopps_partial_aid(pst_mac_vap);
#endif

    
    hmac_process_assoc_rsp_bw_change_info(pst_mac_vap, pst_hmac_sta, pst_hmac_user, ul_change);

    return OAL_SUCC;
}


uint32_t hmac_sta_sync_vap(hmac_vap_stru *pst_hmac_vap,
                             mac_channel_stru *pst_channel,
                             wlan_protocol_enum_uint8 en_protocol)
{
    mac_device_stru *pst_mac_device = OAL_PTR_NULL;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    uint32_t ul_ret;
    mac_cfg_mode_param_stru st_prot_param;
    mac_channel_stru *pst_channel_dst = OAL_PTR_NULL;

    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_SCAN, "{hmac_sta_sync_vap::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_mac_vap = &(pst_hmac_vap->st_vap_base_info);

    /* 获取mac device指针 */
    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (oal_unlikely(pst_mac_device == OAL_PTR_NULL)) {
        hmac_fsm_change_state(pst_hmac_vap, MAC_VAP_STATE_INIT);
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_SCAN, "{hmac_sta_sync_vap::pst_mac_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_channel_dst = pst_channel ? pst_channel : &pst_mac_vap->st_channel;

    st_prot_param.en_channel_idx = pst_channel_dst->uc_chan_number;
    st_prot_param.en_bandwidth = pst_channel_dst->en_bandwidth;
    st_prot_param.en_band = pst_channel_dst->en_band;

    // use input protocol if exists
    st_prot_param.en_protocol = (en_protocol >= WLAN_PROTOCOL_BUTT) ? pst_mac_vap->en_protocol : en_protocol;

    ul_ret = mac_is_channel_num_valid(st_prot_param.en_band, st_prot_param.en_channel_idx);
    if (ul_ret != OAL_SUCC) {
        hmac_fsm_change_state(pst_hmac_vap, MAC_VAP_STATE_INIT);
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_SCAN,
                         "{hmac_sta_sync_vap::mac_is_channel_num_valid failed[%d].}", ul_ret);
        return ul_ret;
    }

    oam_warning_log3(pst_mac_vap->uc_vap_id, OAM_SF_SCAN,
                     "{hmac_sta_sync_vap::AP: Starting network in Channel: %d, bandwidth: %d protocol: %d.}",
                     st_prot_param.en_channel_idx, st_prot_param.en_bandwidth, st_prot_param.en_protocol);

    /* 同时更改多个VAP的信道，此时需要强制清除记录 */
    /* 若启动了DBAC，则按照原始流程进行 */
    if (!mac_is_dbac_enabled(pst_mac_device))
    {
        pst_mac_device->uc_max_channel = 0;
        pst_mac_device->en_max_bandwidth = WLAN_BAND_WIDTH_BUTT;
    }

    // force channel setting is required
    pst_mac_vap->st_channel = *pst_channel_dst;

    ul_ret = hmac_config_set_freq(pst_mac_vap, OAL_SIZEOF(uint8_t), &st_prot_param.en_channel_idx);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_SCAN,
                         "{hmac_sta_sync_vap::hmac_config_set_freq failed[%d].}", ul_ret);
        return ul_ret;
    }

    ul_ret = hmac_config_set_mode(pst_mac_vap, (uint16_t)OAL_SIZEOF(st_prot_param), (uint8_t *)&st_prot_param);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_SCAN,
                         "{hmac_sta_sync_vap::hmac_config_send_event failed[%d].}", ul_ret);
        return ul_ret;
    }

    return OAL_SUCC;
}


uint32_t hmac_sta_wait_asoc_rx(hmac_vap_stru *pst_hmac_sta, void *pst_msg)
{
    mac_status_code_enum_uint16 en_asoc_status;
    uint8_t uc_frame_sub_type;
    dmac_wlan_crx_event_stru *pst_mgmt_rx_event = OAL_PTR_NULL;
    dmac_rx_ctl_stru *pst_rx_ctrl = OAL_PTR_NULL;
    mac_rx_ctl_stru *pst_rx_info = OAL_PTR_NULL;
    uint8_t *puc_mac_hdr = OAL_PTR_NULL;
    uint8_t *puc_payload = OAL_PTR_NULL;
    uint16_t us_msg_len;
    uint16_t us_hdr_len;
    hmac_asoc_rsp_stru st_asoc_rsp;
    uint8_t auc_addr_sa[WLAN_MAC_ADDR_LEN];
    uint16_t us_user_idx;
    uint32_t ul_rslt;
    hmac_user_stru *pst_hmac_user_ap = OAL_PTR_NULL;
    uint32_t ul_ret;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    mac_cfg_user_info_param_stru st_hmac_user_info_event;
    uint8_t uc_ap_follow_channel = 0;

    if (oal_any_null_ptr2(pst_hmac_sta, pst_msg)) {
        oam_error_log2(0, OAM_SF_ASSOC, "{hmac_sta_wait_asoc_rx::param null,%x %x.}",
                       (uintptr_t)pst_hmac_sta, (uintptr_t)pst_msg);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_mac_vap = &(pst_hmac_sta->st_vap_base_info);

    pst_mgmt_rx_event = (dmac_wlan_crx_event_stru *)pst_msg;
    pst_rx_ctrl = (dmac_rx_ctl_stru *)oal_netbuf_cb(pst_mgmt_rx_event->pst_netbuf);
    pst_rx_info = (mac_rx_ctl_stru *)(&(pst_rx_ctrl->st_rx_info));
    puc_mac_hdr = (uint8_t *)MAC_GET_RX_CB_MAC_HEADER_ADDR(pst_rx_info);
    puc_payload = (uint8_t *)(puc_mac_hdr) + pst_rx_info->uc_mac_header_len;
    us_msg_len = pst_rx_info->us_frame_len - pst_rx_info->uc_mac_header_len; /* 消息总长度,不包括FCS */
    us_hdr_len = pst_rx_info->uc_mac_header_len;

    uc_frame_sub_type = mac_get_frame_type_and_subtype(puc_mac_hdr);
    en_asoc_status = mac_get_asoc_status(puc_payload);

#ifdef _PRE_WLAN_WAKEUP_SRC_PARSE
    if (OAL_TRUE == wlan_pm_wkup_src_debug_get()) {
        wlan_pm_wkup_src_debug_set(OAL_FALSE);
        oam_warning_log1(pst_hmac_sta->st_vap_base_info.uc_vap_id, OAM_SF_RX,
                         "{wifi_wake_src:hmac_sta_wait_asoc_rx::wakeup mgmt type[0x%x]}",
                         uc_frame_sub_type);
    }
#endif

    if ((WLAN_FC0_SUBTYPE_ASSOC_RSP | WLAN_FC0_TYPE_MGT) != uc_frame_sub_type &&
        (WLAN_FC0_SUBTYPE_REASSOC_RSP | WLAN_FC0_TYPE_MGT) != uc_frame_sub_type) {
        oam_warning_log1(pst_hmac_sta->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_sta_wait_asoc_rx:: uc_frame_sub_type=0x%02x.}", uc_frame_sub_type);
        /* 打印此netbuf相关信息 */
        mac_rx_report_80211_frame((uint8_t *)&(pst_hmac_sta->st_vap_base_info),
                                      (uint8_t *)pst_rx_ctrl,
                                      pst_mgmt_rx_event->pst_netbuf,
                                      OAM_OTA_TYPE_RX_HMAC_CB);
        return OAL_FAIL;
    }

#ifdef _PRE_WLAN_FEATURE_SNIFFER
#ifdef CONFIG_HW_SNIFFER
    proc_sniffer_write_file(NULL, 0, puc_mac_hdr, pst_rx_info->us_frame_len, 0);
#endif
#endif
    if (en_asoc_status != MAC_SUCCESSFUL_STATUSCODE) {
        oam_warning_log1(pst_hmac_sta->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_sta_wait_asoc_rx:: AP refuse STA assoc reason=%d.}", en_asoc_status);
        if (en_asoc_status != MAC_AP_FULL) {
            chr_exception(chr_wifi_drv(CHR_WIFI_DRV_EVENT_CONNECT, CHR_WIFI_DRV_ERROR_ASSOC_REJECTED));
        }

        pst_hmac_sta->st_mgmt_timetout_param.en_status_code = en_asoc_status;

        return OAL_FAIL;
    }

    if (us_msg_len < OAL_ASSOC_RSP_FIXED_OFFSET) {
        oam_error_log1(pst_hmac_sta->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                       "{hmac_sta_wait_asoc_rx::asoc_rsp_body is too short(%d) to going on!}",
                       us_msg_len);
        return OAL_FAIL;
    }

    /* 获取SA 地址 */
    mac_get_address2(puc_mac_hdr, auc_addr_sa, WLAN_MAC_ADDR_LEN);

    /* 根据SA 找到对应AP USER结构 */
    ul_rslt = mac_vap_find_user_by_macaddr(&(pst_hmac_sta->st_vap_base_info), auc_addr_sa, &us_user_idx);

    if (ul_rslt != OAL_SUCC) {
        oam_warning_log1(pst_hmac_sta->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_sta_wait_asoc_rx:: mac_vap_find_user_by_macaddr failed[%d].}", ul_rslt);

        return ul_rslt;
    }

    /* 获取STA关联的AP的用户指针 */
    pst_hmac_user_ap = mac_res_get_hmac_user(us_user_idx);
    if (pst_hmac_user_ap == OAL_PTR_NULL) {
        oam_warning_log1(pst_hmac_sta->st_vap_base_info.uc_vap_id, OAM_SF_AUTH,
                         "{hmac_sta_wait_asoc_rx::pst_hmac_user_ap[%d] null.}", us_user_idx);
        return OAL_FAIL;
    }

    /* 取消定时器 */
    frw_timer_immediate_destroy_timer_m(&(pst_hmac_sta->st_mgmt_timer));

    ul_ret = hmac_process_assoc_rsp(pst_hmac_sta, pst_hmac_user_ap, puc_mac_hdr, us_hdr_len, puc_payload, us_msg_len);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(pst_hmac_sta->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_sta_wait_asoc_rx::hmac_process_assoc_rsp failed[%d].}", ul_ret);
        return ul_ret;
    }

    /* 关联上用户之后，初始化黑名单方案 */
    hmac_btcoex_blacklist_handle_init(pst_hmac_user_ap);

    if (OAL_TRUE == hmac_btcoex_check_exception_in_list(pst_hmac_sta, auc_addr_sa)) {
        if (BTCOEX_BLACKLIST_TPYE_FIX_BASIZE == HMAC_BTCOEX_GET_BLACKLIST_TYPE(pst_hmac_user_ap)) {
            oam_warning_log0(pst_hmac_sta->st_vap_base_info.uc_vap_id, OAM_SF_COEX,
                             "{hmac_sta_wait_asoc_rx::mac_addr in blacklist.}");
            pst_hmac_user_ap->st_hmac_user_btcoex.st_hmac_btcoex_addba_req.en_ba_handle_allow = OAL_FALSE;
        } else {
            pst_hmac_user_ap->st_hmac_user_btcoex.st_hmac_btcoex_addba_req.en_ba_handle_allow = OAL_TRUE;
        }
    } else {
        /* 初始允许建立聚合，两个方案保持对齐 */
        pst_hmac_user_ap->st_hmac_user_btcoex.st_hmac_btcoex_addba_req.en_ba_handle_allow = OAL_TRUE;
    }

    /* STA切换到UP状态 */
    hmac_fsm_change_state(pst_hmac_sta, MAC_VAP_STATE_UP);

    /* user已经关联上，抛事件给DMAC，在DMAC层挂用户算法钩子 */
    hmac_user_add_notify_alg(&(pst_hmac_sta->st_vap_base_info), us_user_idx);

    hmac_roam_info_init(pst_hmac_sta);

    /* 将用户(AP)在本地的状态信息设置为已关联状态 */
    /* 准备消息，上报给APP */
    st_asoc_rsp.en_result_code = HMAC_MGMT_SUCCESS;
    st_asoc_rsp.en_status_code = MAC_SUCCESSFUL_STATUSCODE;

    /* 记录关联响应帧的部分内容，用于上报给内核 */
    st_asoc_rsp.ul_asoc_rsp_ie_len = us_msg_len - OAL_ASSOC_RSP_FIXED_OFFSET; /* 除去MAC帧头24字节和FIXED部分6字节 */
    st_asoc_rsp.puc_asoc_rsp_ie_buff = puc_mac_hdr + OAL_ASSOC_RSP_IE_OFFSET;

    /* 获取AP的mac地址 */
    mac_get_bssid(puc_mac_hdr, st_asoc_rsp.auc_addr_ap, WLAN_MAC_ADDR_LEN);

    /* 获取关联请求帧信息 */
    st_asoc_rsp.puc_asoc_req_ie_buff = pst_hmac_user_ap->puc_assoc_req_ie_buff;
    st_asoc_rsp.ul_asoc_req_ie_len = pst_hmac_user_ap->ul_assoc_req_ie_len;

    hmac_send_rsp_to_sme_sta (pst_hmac_sta, HMAC_SME_ASOC_RSP, (uint8_t *)(&st_asoc_rsp));

    /* 1102 STA 入网后，上报VAP 信息和用户信息 */
    st_hmac_user_info_event.us_user_idx = us_user_idx;

    hmac_config_vap_info(pst_mac_vap, OAL_SIZEOF(uint32_t), (uint8_t *)&ul_ret);
    hmac_config_user_info(pst_mac_vap, OAL_SIZEOF(mac_cfg_user_info_param_stru), (uint8_t *)&st_hmac_user_info_event);

    if (OAL_SUCC != oal_notice_sta_join_result(pst_hmac_sta->st_vap_base_info.uc_chip_id, OAL_TRUE)) {
        oam_warning_log0(pst_hmac_sta->st_vap_base_info.uc_vap_id, OAM_SF_COEX,
                         "{hmac_sta_wait_asoc_rx::oal_notice_sta_join_result fail.}");
    }

    /* 信道跟随检查 */
    ul_rslt = hmac_check_ap_channel_follow_sta(pst_mac_vap, &pst_mac_vap->st_channel, &uc_ap_follow_channel);
    if (ul_rslt == OAL_SUCC) {
        oam_warning_log1(pst_hmac_sta->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
            "{hmac_sta_wait_asoc_rx:: after hmac_check_ap_channel_follow_sta. ap channel change to %d}",
            uc_ap_follow_channel);
    }

    // will not do any sync if proxysta enabled
    // allow running DBAC on different channels of same band while P2P defined
    return OAL_SUCC;
}

uint32_t hmac_sta_get_mngpkt_sendstat(mac_vap_stru *pst_mac_vap,
                                        mac_cfg_query_mngpkt_sendstat_stru *pst_sendstat_info)
{
    mac_device_stru *pst_mac_dev = OAL_PTR_NULL;

    if (pst_sendstat_info == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_sta_get_mngpkt_sendstat::pst_sendstat_info null.}");
        return OAL_FAIL;
    }

    pst_mac_dev = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (pst_mac_dev == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
                       "{hmac_sta_get_mngpkt_sendstat::mac_res_get_dev failed.}");
        return OAL_FAIL;
    }

    pst_sendstat_info->uc_auth_req_st = pst_mac_dev->uc_auth_req_sendst;
    pst_sendstat_info->uc_asoc_req_st = pst_mac_dev->uc_asoc_req_sendst;

    return OAL_SUCC;
}

void hmac_sta_clear_auth_req_sendstat(mac_vap_stru *pst_mac_vap)
{
    mac_device_stru *pst_mac_dev;

    pst_mac_dev = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (pst_mac_dev == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
            "{hmac_sta_clear_auth_req_sendstat::mac_res_get_dev failed.}");
        return;
    }

    pst_mac_dev->uc_auth_req_sendst = 0;
}

void hmac_sta_clear_asoc_req_sendstat(mac_vap_stru *pst_mac_vap)
{
    mac_device_stru *pst_mac_dev;

    pst_mac_dev = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (pst_mac_dev == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
                       "{hmac_sta_clear_asoc_req_sendstat::mac_res_get_dev failed.}");
        return;
    }

    pst_mac_dev->uc_asoc_req_sendst = 0;
}


uint32_t hmac_sta_auth_timeout(hmac_vap_stru *pst_hmac_sta, void *p_param)
{
    hmac_auth_rsp_stru st_auth_rsp = {
        {
            0,
        },
    };
    mac_cfg_query_mngpkt_sendstat_stru st_mngpkt_sendstat_info;

    /* and send it to the host.                                          */
    if (MAC_VAP_STATE_STA_WAIT_AUTH_SEQ2 == pst_hmac_sta->st_vap_base_info.en_vap_state) {
        st_auth_rsp.us_status_code = MAC_AUTH_RSP2_TIMEOUT;
    } else if (MAC_VAP_STATE_STA_WAIT_AUTH_SEQ4 == pst_hmac_sta->st_vap_base_info.en_vap_state) {
        st_auth_rsp.us_status_code = MAC_AUTH_RSP4_TIMEOUT;
    } else {
        st_auth_rsp.us_status_code = HMAC_MGMT_TIMEOUT;
    }

    if (OAL_SUCC == hmac_sta_get_mngpkt_sendstat(&pst_hmac_sta->st_vap_base_info, &st_mngpkt_sendstat_info)) {
        if (st_mngpkt_sendstat_info.uc_auth_req_st > 0) {
            st_auth_rsp.us_status_code = MAC_AUTH_REQ_SEND_FAIL_BEGIN + st_mngpkt_sendstat_info.uc_auth_req_st;
            hmac_sta_clear_auth_req_sendstat(&pst_hmac_sta->st_vap_base_info);
        }
    }

    /* Send the response to host now. */
    hmac_send_rsp_to_sme_sta(pst_hmac_sta, HMAC_SME_AUTH_RSP, (uint8_t *)&st_auth_rsp);

    return OAL_SUCC;
}


uint32_t hmac_get_phy_rate_and_protocol(uint8_t uc_pre_rate,
                                          wlan_protocol_enum_uint8 *en_protocol,
                                          uint8_t *uc_rate_idx)
{
    uint32_t uc_idx;

    for (uc_idx = 0; uc_idx < WLAN_11B_BUTT; uc_idx++) {
        if (g_st_data_11b_rates[uc_idx].uc_expand_rate == uc_pre_rate ||
            g_st_data_11b_rates[uc_idx].uc_mac_rate == uc_pre_rate) {
            *uc_rate_idx = uc_idx;
            *en_protocol = 0; /* 11b 协议速率 */
            return OAL_SUCC;
        }
    }

    for (uc_idx = 0; uc_idx < WLAN_11AG_BUTT; uc_idx++) {
        if (g_st_data_legacy_ofdm_rates[uc_idx].uc_expand_rate == uc_pre_rate ||
            g_st_data_legacy_ofdm_rates[uc_idx].uc_mac_rate == uc_pre_rate) {
            *uc_rate_idx = uc_idx;
            *en_protocol = 1; /* 11ag 协议速率 */
            return OAL_SUCC;
        }
    }

    return OAL_FAIL;
}


uint32_t hmac_sta_get_min_rate(dmac_set_rate_stru *pst_rate_params, hmac_join_req_stru *pst_join_req)
{
    uint32_t uc_idx;
    uint8_t auc_min_rate_idx[2] = { 0 }; /* 第一个存储11b协议对应的速率，第二个存储11ag协议对应的速率 */
    uint8_t uc_min_rate_idx = 0;
    uint8_t uc_protocol = 0; /* 表示基础速率级的协议，0:11b  1:legacy ofdm */

    if (oal_any_null_ptr2(pst_rate_params, pst_join_req)) {
        oam_error_log0(0, OAM_SF_SCAN, "{hmac_sta_get_min_rate::NULL param}");
        return OAL_FAIL;
    }

    memset_s(pst_rate_params, OAL_SIZEOF(dmac_set_rate_stru), 0, OAL_SIZEOF(dmac_set_rate_stru));

    // get min rate
    for (uc_idx = 0; uc_idx < pst_join_req->st_bss_dscr.uc_num_supp_rates; uc_idx++) {
        if (OAL_SUCC != hmac_get_phy_rate_and_protocol(pst_join_req->st_bss_dscr.auc_supp_rates[uc_idx],
                                                       &uc_protocol, &uc_min_rate_idx)) {
            oam_error_log2(0, OAM_SF_SCAN, "{hmac_sta_get_min_rate::hmac_get_rate_protocol failed (%d).[%d]}",
                           uc_min_rate_idx, uc_idx);
            continue;
        }

        /* 根据枚举wlan_phy_protocol_enum 填写数组对应位置的值 */
        if (pst_rate_params->un_capable_flag.uc_value & ((uint32_t)1 << (uc_protocol))) {
            if (auc_min_rate_idx[uc_protocol] > uc_min_rate_idx) {
                auc_min_rate_idx[uc_protocol] = uc_min_rate_idx;
            }
        } else {
            auc_min_rate_idx[uc_protocol] = uc_min_rate_idx;
            pst_rate_params->un_capable_flag.uc_value |= ((uint32_t)1 << (uc_protocol));
        }

        if (uc_min_rate_idx == 0x08) { // st_data_rates第八个是24M
            pst_rate_params->un_capable_flag.st_capable.bit_ht_capable = OAL_TRUE;
            pst_rate_params->un_capable_flag.st_capable.bit_vht_capable = OAL_TRUE;
        }
    }

    /* 与储存在扫描结果描述符中的能力进行比较，看能力是否匹配 */
    pst_rate_params->un_capable_flag.st_capable.bit_ht_capable &= pst_join_req->st_bss_dscr.en_ht_capable;
    pst_rate_params->un_capable_flag.st_capable.bit_vht_capable &= pst_join_req->st_bss_dscr.en_vht_capable;
    pst_rate_params->uc_min_rate[0] = g_st_data_11b_rates[auc_min_rate_idx[0]].uc_hal_wlan_rate_index;
    pst_rate_params->uc_min_rate[1] = g_st_data_legacy_ofdm_rates[auc_min_rate_idx[1]].uc_hal_wlan_rate_index;

    oam_warning_log4(0, OAM_SF_ASSOC,
                     "{hmac_sta_get_min_rate:: uc_min_rate_idx[%d]un_capable_flag.uc_value[%d]legacy rate[%d][%d]}",
                     uc_min_rate_idx, pst_rate_params->un_capable_flag.uc_value,
                     pst_rate_params->uc_min_rate[0], pst_rate_params->uc_min_rate[1]);

    return OAL_SUCC;
}

#if (_PRE_WLAN_FEATURE_PMF != _PRE_PMF_NOT_SUPPORT)

void hmac_sta_roam_update_pmf(hmac_vap_stru *pst_hmac_vap, mac_bss_dscr_stru *pst_mac_bss_dscr)
{
    uint8_t ul_match_suite2 = 0xff;
    uint8_t ul_match_suite3 = 0xff;
    mac_crypto_settings_stru st_crypto = {0};
    uint16_t us_rsn_cap_info;
    wlan_pmf_cap_status_uint8 en_pmf_cap;
    hmac_device_stru *pst_hmac_dev;

    if (pst_hmac_vap->st_vap_base_info.en_vap_state != MAC_VAP_STATE_ROAMING) {
        return;
    }

    memset_s(&st_crypto, OAL_SIZEOF(mac_crypto_settings_stru), 0, OAL_SIZEOF(mac_crypto_settings_stru));

    if (pst_mac_bss_dscr->puc_rsn_ie != OAL_PTR_NULL) {
        mac_ie_get_rsn_cipher(pst_mac_bss_dscr->puc_rsn_ie, &st_crypto); // TBD: only two set suites

        ul_match_suite2 = mac_rsn_ie_akm_match_suites_s(st_crypto.aul_akm_suite,
                                                        sizeof(st_crypto.aul_akm_suite), MAC_RSN_AKM_PSK);
        if (ul_match_suite2 == 0xff) {
            ul_match_suite2 = mac_rsn_ie_akm_match_suites_s(st_crypto.aul_akm_suite,
                                                            sizeof(st_crypto.aul_akm_suite), MAC_RSN_AKM_PSK_SHA256);
        }

        ul_match_suite3 = mac_rsn_ie_akm_match_suites_s(st_crypto.aul_akm_suite,
                                                        sizeof(st_crypto.aul_akm_suite), MAC_RSN_AKM_SAE);
    }

    us_rsn_cap_info = mac_get_rsn_capability(pst_mac_bss_dscr->puc_rsn_ie);

#ifdef _PRE_WLAN_FEATURE_SAE
    if (((mac_mib_get_AuthenticationMode(&pst_hmac_vap->st_vap_base_info) == WLAN_WITP_AUTH_SAE) ||
        (pst_hmac_vap->en_sae_connect == OAL_TRUE)) &&
        (ul_match_suite2 == 0xff) && (ul_match_suite3 != 0xff)) {
        en_pmf_cap = MAC_RSN_CAP_TO_PMF(us_rsn_cap_info); // wpa3 only, including sae, sae/ft-sae, not wpa2/wpa3 mixed
    } else {
        en_pmf_cap = MAC_IS_RSN_ENABLE_PMF(us_rsn_cap_info); // wpa/wpa2, wpa2 and wpa2/wpa3 mixed
    }
#else
    en_pmf_cap = MAC_IS_RSN_ENABLE_PMF(us_rsn_cap_info);
#endif

    pst_hmac_dev = hmac_res_get_mac_dev(pst_hmac_vap->st_vap_base_info.uc_device_id);
    if (pst_hmac_dev == OAL_PTR_NULL) {
        oam_error_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY, "hmac_sta_roam_update_pmf:: hmac device not find.");
        return;
    }

    if ((OAL_TRUE == hmac_device_pmf_find_black_list(pst_hmac_dev, pst_mac_bss_dscr->auc_mac_addr)) &&
        MAC_IS_RSN_PMF_ONLY_ENABLE(us_rsn_cap_info)) {
        en_pmf_cap = MAC_PMF_DISABLED;
        oam_warning_log3(0, OAM_SF_ANY, "hmac_sta_roam_update_pmf::black list mac addr %02X:xx:xx:xx:%02X:%02X",
                         pst_mac_bss_dscr->auc_mac_addr[0],
                         pst_mac_bss_dscr->auc_mac_addr[4],
                         pst_mac_bss_dscr->auc_mac_addr[5]);
    }

    hmac_config_vap_pmf_cap(&(pst_hmac_vap->st_vap_base_info), en_pmf_cap);
}
#endif

#ifdef _PRE_WLAN_FEATURE_11AX

void hmac_sta_update_join_multi_bssid_info(mac_vap_stru *pst_mac_vap,
                                               dmac_ctx_join_req_set_reg_stru *pst_reg_params,
                                               mac_multi_bssid_info *pst_mbssid_info)
{
    if (IS_CUSTOM_OPEN_MULTI_BSSID_SWITCH(pst_mac_vap)) {
        memcpy_s(&pst_reg_params->st_mbssid_info, OAL_SIZEOF(mac_multi_bssid_info),
                 pst_mbssid_info, OAL_SIZEOF(mac_multi_bssid_info));
        mac_mib_set_he_MultiBSSIDActived(pst_mac_vap, pst_mbssid_info->bit_ext_cap_multi_bssid_activated);
    }
}
#endif
#ifdef _PRE_WLAN_FEATURE_M2S
OAL_STATIC OAL_INLINE oal_bool_enum_uint8 hmac_sta_support_opmode_rules(hmac_join_req_stru *pst_join_req)
{
    
    /* 双流及其以下，三四流ht设备都配置为不支持opmode */
    return ((pst_join_req->st_bss_dscr.en_support_max_nss == WLAN_SINGLE_NSS) ||
            (pst_join_req->st_bss_dscr.en_vht_capable == OAL_FALSE) ||
            (pst_join_req->st_bss_dscr.en_support_max_nss == WLAN_DOUBLE_NSS &&
             pst_join_req->st_bss_dscr.en_support_opmode == OAL_FALSE));
}
#endif


void hmac_sta_update_join_req_params_for_roam(mac_vap_stru *pst_mac_vap, mac_ap_type_enum_uint16 *pen_ap_type)
{
    hmac_user_stru *pst_hmac_user = NULL;

    if (pst_mac_vap->en_vap_state == MAC_VAP_STATE_ROAMING) {
        pst_hmac_user = mac_res_get_hmac_user(pst_mac_vap->us_assoc_vap_id);
        if (pst_hmac_user != NULL) {
            *pen_ap_type = hmac_compability_ap_tpye_identify(pst_mac_vap, pst_mac_vap->auc_bssid);

            oam_warning_log1(0, OAM_SF_TX,
                             "{hmac_sta_update_join_req_params_for_roam::ROAM vap en_ap_type[%d].}\r\n",
                             *pen_ap_type);

            pst_hmac_user->en_user_ap_type = *pen_ap_type; /* AP类型 */
        }
    }
}

#ifdef _PRE_WLAN_FEATURE_11AX

OAL_STATIC oal_bool_enum_uint8 hmac_sta_join_can_not_start_he(mac_vap_stru *p_mac_vap,
    mac_bss_dscr_stru *p_bss_dscr)
{
    if (hmac_wifi6_self_cure_mac_is_wifi6_blacklist_type(p_bss_dscr->auc_bssid)) {
        oam_warning_log3(0, OAM_SF_SCAN, "hmac_sta_join_can_not_start_he::mac:%02X:XX:XX:XX:%02X:%02X \
            in wifi6 balcklist, close HEOptionImplemented",
            p_bss_dscr->auc_bssid[0], /* 0为mac地址字节位置 */
            p_bss_dscr->auc_bssid[4], /* 4为mac地址字节位置 */
            p_bss_dscr->auc_bssid[5]); /* 5为mac地址字节位置 */
        return OAL_TRUE;
    }

    if (p_bss_dscr->en_p2p_scenes ==  MAC_P2P_SCENES_LOW_LATENCY || mac_is_secondary_sta(p_mac_vap)) {
        return mac_vap_can_not_start_he_protocol(p_mac_vap);
    }

    return OAL_FALSE;
}


OAL_STATIC void hmac_sta_join_update_protocol_mib(mac_vap_stru *p_mac_vap,
    mac_bss_dscr_stru *p_bss_dscr)
{
    mac_mib_set_HEOptionImplemented(p_mac_vap, OAL_FALSE);
    if ((p_bss_dscr->en_he_capable == OAL_TRUE) && (IS_CUSTOM_OPEN_11AX_SWITCH(p_mac_vap))) {
        mac_mib_set_HEOptionImplemented(p_mac_vap, OAL_TRUE);
        if (OAL_TRUE == hmac_sta_join_can_not_start_he(p_mac_vap, p_bss_dscr)) {
            p_bss_dscr->en_he_capable = OAL_FALSE;
            mac_mib_set_HEOptionImplemented(p_mac_vap, OAL_FALSE);
        }
    }

    /* 2g 11ax sta先入网时,gc 5g 入网回退到11协议 */
    hmac_vap_join_avoid_dbac_back_to_11n_handle(p_mac_vap, p_bss_dscr);
    oam_warning_log3(0, OAM_SF_SCAN, "hmac_sta_join_update_protocol_mib::bss_he_cap=%d,bss_vht_cap=%d \
        en_p2p_scenes=%d", p_bss_dscr->en_he_capable, p_bss_dscr->en_vht_capable,
        p_bss_dscr->en_p2p_scenes);
    oam_warning_log3(0, OAM_SF_SCAN, "hmac_sta_join_update_protocol_mib::he_mib=%d, \
        VAP_IS_SUPPORT_11AX=%d, MAC_VAP_GET_CAP_BW=%d",
        mac_mib_get_HEOptionImplemented(p_mac_vap),
        MAC_VAP_IS_SUPPORT_11AX(p_mac_vap), MAC_VAP_GET_CAP_BW(p_mac_vap));
}
#endif

OAL_STATIC oal_void hmac_sta_update_join_bw(mac_vap_stru *p_mac_vap,  hmac_join_req_stru *p_join_req)
{
    /* 更新频带、主20MHz信道号，与AP通信 DMAC切换信道时直接调用 */
    MAC_VAP_GET_CAP_BW(p_mac_vap) = mac_vap_get_bandwith(mac_mib_get_dot11VapMaxBandWidth(p_mac_vap),
        p_join_req->st_bss_dscr.en_channel_bandwidth);

    oam_warning_log4(p_mac_vap->uc_vap_id, OAM_SF_SCAN, "hmac_sta_update_join_bw::bss_info:he_cap=%d,vht_cap=%d \
        p2p_scenes=%d, bw=%d",
        p_join_req->st_bss_dscr.en_he_capable, p_join_req->st_bss_dscr.en_vht_capable,
        p_join_req->st_bss_dscr.en_p2p_scenes, p_join_req->st_bss_dscr.en_channel_bandwidth);

    /* 5G 回退到40M */
    if (p_join_req->st_bss_dscr.en_p2p_scenes == MAC_P2P_SCENES_LOW_LATENCY &&
        OAL_TRUE == mac_vap_p2p_bw_back_to_40m(p_mac_vap, p_mac_vap->st_channel.uc_chan_number,
            &MAC_VAP_GET_CAP_BW(p_mac_vap))) {
        p_join_req->st_bss_dscr.en_channel_bandwidth = MAC_VAP_GET_CAP_BW(p_mac_vap);
        oam_warning_log1(p_mac_vap->uc_vap_id, 0, "{hmac_sta_update_join_bw::change bss_bw =%d",
            p_join_req->st_bss_dscr.en_channel_bandwidth);
    }
    oam_warning_log3(p_mac_vap->uc_vap_id, OAM_SF_SCAN, "hmac_sta_update_join_bw::dot11VapMaxBandWidth=%d, \
        bss_bw=%d, vap_bw=%d", mac_mib_get_dot11VapMaxBandWidth(p_mac_vap),
        p_join_req->st_bss_dscr.en_channel_bandwidth, MAC_VAP_GET_CAP_BW(p_mac_vap));
}


/* wlan0入网或者漫游时检查是否需要断开wlan1 */
OAL_STATIC uint32_t hmac_dual_sta_join_check(hmac_vap_stru *hmac_vap, hmac_join_req_stru *join_req)
{
    int32_t need_disconnect = OAL_FALSE;
    mac_vap_stru *other_vap = NULL;
    mac_vap_stru *mac_vap = &hmac_vap->st_vap_base_info;
    mac_cfg_kick_user_param_stru kick_user_param;
    uint8_t auc_mac_addr[WLAN_MAC_ADDR_LEN] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

    if (!mac_is_dual_sta_mode() || !mac_is_primary_legacy_sta(mac_vap)) {
        return OAL_SUCC;
    }

    other_vap = mac_vap_find_another_up_vap_by_mac_vap(mac_vap);
    if (!other_vap) {
        /* 没有其他up vap, 不需要检查。*/
        return OAL_SUCC;
    }

    if (!mac_is_secondary_sta(other_vap)) {
        oam_error_log2(other_vap->uc_vap_id, OAM_SF_ASSOC, "{hmac_dual_sta_join_check::"
            "unexpect vap, vap mode[%d], p2p mode[%d]}", other_vap->en_vap_mode, other_vap->en_p2p_mode);
        return OAL_SUCC;
    }

    if (mac_vap->en_vap_state == MAC_VAP_STATE_ROAMING) {
        if (hmac_roam_pri_sta_join_check(hmac_vap, &join_req->st_bss_dscr, other_vap) == OAL_SUCC) {
            need_disconnect = OAL_FALSE;
        } else {
            need_disconnect = OAL_TRUE;
        }
    } else {
        /* wlan0入网时, wlan1已关联。需要先断开wlan1。 */
        need_disconnect = OAL_TRUE;
        oam_warning_log0(mac_vap->uc_vap_id, OAM_SF_ASSOC,
            "{hmac_dual_sta_join_check::sec sta is connected before pri sta, disconnect sec sta!}");
    }

    if (need_disconnect) {
        kick_user_param.us_reason_code = MAC_UNSPEC_REASON;
        memcpy_s(kick_user_param.auc_mac_addr, WLAN_MAC_ADDR_LEN, auc_mac_addr, WLAN_MAC_ADDR_LEN);
        if (hmac_config_kick_user(other_vap, OAL_SIZEOF(kick_user_param),
            (uint8_t *)&kick_user_param) != OAL_SUCC) {
            oam_warning_log0(mac_vap->uc_vap_id, OAM_SF_ASSOC,
                "{hmac_dual_sta_join_check::secondary sta disconnect failed!");
            return OAL_FAIL;
        }
    }
    return OAL_SUCC;
}


uint32_t hmac_sta_update_join_req_params(hmac_vap_stru *pst_hmac_vap, hmac_join_req_stru *pst_join_req)
{
    mac_vap_stru *pst_mac_vap;
    dmac_ctx_join_req_set_reg_stru *pst_reg_params = OAL_PTR_NULL;
    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL;
    frw_event_stru *pst_event = OAL_PTR_NULL;
    uint32_t ul_ret;
    mac_device_stru *pst_mac_device = OAL_PTR_NULL;
    wlan_mib_ieee802dot11_stru *pst_mib_info;
    mac_cfg_mode_param_stru st_cfg_mode;
    uint8_t *puc_cur_ssid = OAL_PTR_NULL;
    mac_ap_type_enum_uint16 en_ap_type = 0;

    pst_mac_vap = &(pst_hmac_vap->st_vap_base_info);
    pst_mib_info = pst_mac_vap->pst_mib_info;
    if (pst_mib_info == OAL_PTR_NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (pst_mac_device == OAL_PTR_NULL) {
        return OAL_ERR_CODE_MAC_DEVICE_NULL;
    }

    /* 设置BSSID */
    mac_vap_set_bssid(pst_mac_vap, pst_join_req->st_bss_dscr.auc_bssid);

    /* 更新mib库对应的dot11BeaconPeriod值 */
    mac_mib_set_BeaconPeriod (pst_mac_vap, (uint32_t)(pst_join_req->st_bss_dscr.us_beacon_period));

    /* 更新mib库对应的ul_dot11CurrentChannel值 */
    mac_vap_set_current_channel(pst_mac_vap, pst_join_req->st_bss_dscr.st_channel.en_band,
                                    pst_join_req->st_bss_dscr.st_channel.uc_chan_number);

#ifdef _PRE_WLAN_FEATURE_11D
    /* 更新sta期望加入的国家字符串 */
    pst_hmac_vap->ac_desired_country[0] = pst_join_req->st_bss_dscr.ac_country[0];
    pst_hmac_vap->ac_desired_country[1] = pst_join_req->st_bss_dscr.ac_country[1];
    pst_hmac_vap->ac_desired_country[2] = pst_join_req->st_bss_dscr.ac_country[2];
#endif

    /* 更新mib库对应的ssid */
    puc_cur_ssid = mac_mib_get_DesiredSSID(pst_mac_vap);
    memcpy_s(puc_cur_ssid, WLAN_SSID_MAX_LEN, pst_join_req->st_bss_dscr.ac_ssid, WLAN_SSID_MAX_LEN);
    puc_cur_ssid[WLAN_SSID_MAX_LEN - 1] = '\0';

    /* 更新频带、主20MHz信道号，与AP通信 DMAC切换信道时直接调用 */
    pst_mac_vap->st_channel.uc_chan_number = pst_join_req->st_bss_dscr.st_channel.uc_chan_number;
    pst_mac_vap->st_channel.en_band = pst_join_req->st_bss_dscr.st_channel.en_band;

    hmac_sta_update_join_bw(pst_mac_vap, pst_join_req);
    hmac_p2p_sta_join_go_back_to_80m_handle(pst_mac_vap);

    /* 在STA未配置协议模式情况下，根据要关联的AP，更新mib库中对应的HT/VHT能力 */
    if (pst_hmac_vap->bit_sta_protocol_cfg == OAL_SWITCH_OFF) {
        memset_s(&st_cfg_mode, OAL_SIZEOF(mac_cfg_mode_param_stru), 0, OAL_SIZEOF(mac_cfg_mode_param_stru));

        mac_mib_set_HighThroughputOptionImplemented(pst_mac_vap, pst_join_req->st_bss_dscr.en_ht_capable);
        mac_mib_set_VHTOptionImplemented(pst_mac_vap, pst_join_req->st_bss_dscr.en_vht_capable);

#ifdef _PRE_WLAN_FEATURE_11AX
        if (g_wlan_spec_cfg->feature_11ax_is_open) {
            hmac_sta_join_update_protocol_mib(pst_mac_vap, &pst_join_req->st_bss_dscr);
        }
#endif
        if (hmac_dual_sta_join_check(pst_hmac_vap, pst_join_req) != OAL_SUCC) {
            oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC, "{hmac_sta_update_join_req_params::dual sta check fail!}");
            return OAL_FAIL;
        }

        /* STA更新LDPC和STBC的能力,更新能力保存到Activated mib中 */
        mac_mib_set_LDPCCodingOptionActivated(pst_mac_vap, (oal_bool_enum_uint8)(pst_join_req->st_bss_dscr.en_ht_ldpc &
            mac_mib_get_LDPCCodingOptionImplemented(pst_mac_vap)));
        mac_mib_set_TxSTBCOptionActivated(pst_mac_vap, (oal_bool_enum_uint8)(pst_join_req->st_bss_dscr.en_ht_stbc &
            mac_mib_get_TxSTBCOptionImplemented(pst_mac_vap)));

        /* 关联2G AP，且2ght40禁止位为1时，不学习AP的HT 40能力 */
        if (!(pst_mac_vap->st_channel.en_band == WLAN_BAND_2G && pst_mac_vap->st_cap_flag.bit_disable_2ght40)) {
            if (pst_join_req->st_bss_dscr.en_bw_cap == WLAN_BW_CAP_20M) {
                mac_mib_set_FortyMHzOperationImplemented(pst_mac_vap, OAL_FALSE);
            } else {
                mac_mib_set_FortyMHzOperationImplemented(pst_mac_vap, OAL_TRUE);
            }
        }

        /* 根据要加入AP的协议模式更新STA侧速率集 */
        ul_ret = hmac_sta_get_user_protocol_etc(&(pst_join_req->st_bss_dscr), &(st_cfg_mode.en_protocol));
        if (ul_ret != OAL_SUCC) {
            oam_error_log1(0, OAM_SF_SCAN,
                           "{hmac_sta_update_join_req_params::hmac_sta_get_user_protocol fail %d.}", ul_ret);
            return ul_ret;
        }

        oam_warning_log2(pst_mac_vap->uc_vap_id, OAM_SF_SCAN, "{hmac_sta_update_join_req_params::mac_vap_pro[%d], \
            cfg_pro[%d]}", pst_mac_vap->en_protocol, st_cfg_mode.en_protocol);

        st_cfg_mode.en_band = pst_join_req->st_bss_dscr.st_channel.en_band;
        st_cfg_mode.en_bandwidth = mac_vap_get_bandwith(mac_mib_get_dot11VapMaxBandWidth(pst_mac_vap),
                                                        pst_join_req->st_bss_dscr.en_channel_bandwidth);
        st_cfg_mode.en_channel_idx = pst_join_req->st_bss_dscr.st_channel.uc_chan_number;
        ul_ret = hmac_config_sta_update_rates(pst_mac_vap, &st_cfg_mode, (void *)&pst_join_req->st_bss_dscr);
        if (ul_ret != OAL_SUCC) {
            oam_error_log1(0, OAM_SF_SCAN,
                           "{hmac_sta_update_join_req_params::hmac_config_sta_update_rates fail %d.}", ul_ret);
            return ul_ret;
        }
    }

    /* 有些加密协议只能工作在legacy */
    hmac_sta_protocol_down_by_chipher(pst_mac_vap, &pst_join_req->st_bss_dscr);

    st_cfg_mode.en_protocol = pst_mac_vap->en_protocol;
    st_cfg_mode.en_band = pst_mac_vap->st_channel.en_band;
    st_cfg_mode.en_bandwidth = pst_mac_vap->st_channel.en_bandwidth;
    st_cfg_mode.en_channel_idx = pst_join_req->st_bss_dscr.st_channel.uc_chan_number;
    hmac_config_sta_update_rates(pst_mac_vap, &st_cfg_mode, (void *)&pst_join_req->st_bss_dscr);

    /* STA首先以20MHz运行，如果要切换到40 or 80MHz运行，需要满足一下条件: */
    /* (1) 用户支持40 or 80MHz运行 */
    /* (2) AP支持40 or 80MHz运行(HT Supported Channel Width Set = 1 && VHT Supported Channel Width Set = 0) */
    /* (3) AP在40 or 80MHz运行(SCO = SCA or SCB && VHT Channel Width = 1) */
    pst_mac_vap->st_channel.en_bandwidth = WLAN_BAND_WIDTH_20M;

    ul_ret = mac_get_channel_idx_from_num(pst_mac_vap->st_channel.en_band, pst_mac_vap->st_channel.uc_chan_number,
        &(pst_mac_vap->st_channel.uc_chan_idx));
    if (ul_ret != OAL_SUCC) {
        oam_error_log2(pst_mac_vap->uc_vap_id, OAM_SF_SCAN,
            "{hmac_sta_update_join_req_params::band and channel_num are not compatible.band[%d], channel_num[%d]}",
            pst_mac_vap->st_channel.en_band, pst_mac_vap->st_channel.uc_chan_number);
        return ul_ret;
    }

    /* 更新协议相关信息，包括WMM P2P 11I 20/40M等 */
    hmac_update_join_req_params_prot_sta(pst_hmac_vap, pst_join_req);
    /* 入网优化，不同频段下的能力不一样 */
    if (WLAN_BAND_2G == pst_mac_vap->st_channel.en_band) {
        mac_mib_set_ShortPreambleOptionImplemented(pst_mac_vap, WLAN_LEGACY_11B_MIB_SHORT_PREAMBLE);
        mac_mib_set_SpectrumManagementRequired(pst_mac_vap, OAL_FALSE);
    } else {
        mac_mib_set_ShortPreambleOptionImplemented(pst_mac_vap, WLAN_LEGACY_11B_MIB_LONG_PREAMBLE);
        mac_mib_set_SpectrumManagementRequired(pst_mac_vap, OAL_TRUE);
    }

    /* 根据协议信道做带宽约束, 此时已经跟进join参数刷新好带宽和信道，此时直接根据当前信道再次刷新带宽 */
    hmac_sta_bandwidth_down_by_channel(pst_mac_vap);

    if (0 == hmac_calc_up_ap_num(pst_mac_device)) {
        pst_mac_device->uc_max_channel = pst_mac_vap->st_channel.uc_chan_number;
        pst_mac_device->en_max_band = pst_mac_vap->st_channel.en_band;
        pst_mac_device->en_max_bandwidth = pst_mac_vap->st_channel.en_bandwidth;
    }

    hmac_sta_update_join_req_params_for_roam(pst_mac_vap, &en_ap_type);

    /* 抛事件到DMAC, 申请事件内存 */
    pst_event_mem = frw_event_alloc_m(OAL_SIZEOF(dmac_ctx_join_req_set_reg_stru));
    if (pst_event_mem == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_SCAN, "{hmac_sta_update_join_req_params::event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 填写事件 */
    pst_event = frw_get_event_stru(pst_event_mem);

    frw_event_hdr_init(&(pst_event->st_event_hdr),
                       FRW_EVENT_TYPE_WLAN_CTX,
                       DMAC_WLAN_CTX_EVENT_SUB_TYPE_JOIN_SET_REG,
                       OAL_SIZEOF(dmac_ctx_join_req_set_reg_stru),
                       FRW_EVENT_PIPELINE_STAGE_1,
                       pst_hmac_vap->st_vap_base_info.uc_chip_id,
                       pst_hmac_vap->st_vap_base_info.uc_device_id,
                       pst_hmac_vap->st_vap_base_info.uc_vap_id);

    pst_reg_params = (dmac_ctx_join_req_set_reg_stru *)pst_event->auc_event_data;

    /* 设置需要写入寄存器的BSSID信息 */
    oal_set_mac_addr(pst_reg_params->auc_bssid, pst_join_req->st_bss_dscr.auc_bssid);

    /* 填写信道相关信息 */
    memcpy_s(&pst_reg_params->st_current_channel, OAL_SIZEOF(mac_channel_stru),
        &pst_mac_vap->st_channel, OAL_SIZEOF(mac_channel_stru));

    /* 填写速率相关信息 */
    hmac_sta_get_min_rate(&pst_reg_params->st_min_rate, pst_join_req);

    /* 设置beaocn period信息 */
    pst_reg_params->us_beacon_period = (pst_join_req->st_bss_dscr.us_beacon_period);

    /* 同步FortyMHzOperationImplemented */
    pst_reg_params->en_dot11FortyMHzOperationImplemented = mac_mib_get_FortyMHzOperationImplemented(pst_mac_vap);

    /* 设置beacon filter关闭 */
    pst_reg_params->ul_beacon_filter = OAL_FALSE;

    /* 设置no frame filter打开 */
    pst_reg_params->ul_non_frame_filter = OAL_TRUE;

    /* 保存黑名单类型 */
    pst_reg_params->en_ap_type = en_ap_type;

    /* 下发ssid */
    memcpy_s(pst_reg_params->auc_ssid, WLAN_SSID_MAX_LEN, pst_join_req->st_bss_dscr.ac_ssid, WLAN_SSID_MAX_LEN);
    pst_reg_params->auc_ssid[WLAN_SSID_MAX_LEN - 1] = '\0';

#ifdef _PRE_WLAN_FEATURE_11AX
    if (g_wlan_spec_cfg->feature_11ax_is_open) {
        pst_reg_params->bit_SetHeCap = mac_mib_get_HEOptionImplemented(pst_mac_vap);
        hmac_sta_update_join_multi_bssid_info(pst_mac_vap, pst_reg_params, &pst_join_req->st_bss_dscr.st_mbssid_info);
    }
#endif
    pst_reg_params->bit_SetVHTCap = mac_mib_get_VHTOptionImplemented(pst_mac_vap);
    /* 分发事件 */
    frw_event_dispatch_event(pst_event_mem);
    frw_event_free_m(pst_event_mem);

#ifdef _PRE_WLAN_FEATURE_M2S
    oam_warning_log4(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC,
                     "{hmac_sta_update_join_req_params::ap nss[%d],ap band[%d],ap ht cap[%d],ap vht cap[%d].}",
                     pst_join_req->st_bss_dscr.en_support_max_nss, pst_join_req->st_bss_dscr.st_channel.en_band,
                     pst_join_req->st_bss_dscr.en_ht_capable, pst_join_req->st_bss_dscr.en_vht_capable);

    pst_mac_vap->st_cap_flag.bit_opmode = !hmac_sta_support_opmode_rules(pst_join_req);

    /* 同步vap修改信息到device侧 */
    hmac_config_vap_m2s_info_syn(pst_mac_vap);
#endif

#if (_PRE_WLAN_FEATURE_PMF != _PRE_PMF_NOT_SUPPORT)
    hmac_sta_roam_update_pmf(pst_hmac_vap, &pst_join_req->st_bss_dscr);
#endif

    return OAL_SUCC;
}


uint32_t hmac_sta_wait_asoc_timeout(hmac_vap_stru *pst_hmac_sta, void *p_param)
{
    hmac_asoc_rsp_stru st_asoc_rsp = { 0 };
    hmac_mgmt_timeout_param_stru *pst_timeout_param = OAL_PTR_NULL;
    mac_cfg_query_mngpkt_sendstat_stru st_mngpkt_sendstat_info;

    if (oal_any_null_ptr2(pst_hmac_sta, p_param)) {
        oam_error_log2(0, OAM_SF_SCAN, "{hmac_sta_wait_asoc_timeout::param null,%x %x.}",
                       (uintptr_t)pst_hmac_sta, (uintptr_t)p_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_timeout_param = (hmac_mgmt_timeout_param_stru *)p_param;

    /* 填写关联结果 */
    st_asoc_rsp.en_result_code = HMAC_MGMT_TIMEOUT;

    /* 关联超时失败,原因码上报wpa_supplicant */
    st_asoc_rsp.en_status_code = pst_timeout_param->en_status_code;

    if (OAL_SUCC == hmac_sta_get_mngpkt_sendstat(&pst_hmac_sta->st_vap_base_info, &st_mngpkt_sendstat_info)) {
        if (st_mngpkt_sendstat_info.uc_asoc_req_st > 0 && st_asoc_rsp.en_status_code == MAC_ASOC_RSP_TIMEOUT) {
            st_asoc_rsp.en_status_code = MAC_ASOC_REQ_SEND_FAIL_BEGIN + st_mngpkt_sendstat_info.uc_asoc_req_st;
            hmac_sta_clear_asoc_req_sendstat(&pst_hmac_sta->st_vap_base_info);
        }
    }

    /* 发送关联结果给SME */
    hmac_send_rsp_to_sme_sta(pst_hmac_sta, HMAC_SME_ASOC_RSP, (uint8_t *)&st_asoc_rsp);

    return OAL_SUCC;
}


void hmac_sta_handle_disassoc_rsp(hmac_vap_stru *pst_hmac_vap, uint16_t us_disasoc_reason_code)
{
    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL;
    frw_event_stru *pst_event = OAL_PTR_NULL;

    /* 通告vap sta去关联 */
    if (pst_hmac_vap->st_vap_base_info.en_vap_mode == WLAN_VAP_MODE_BSS_STA) {
        oal_notice_sta_join_result(pst_hmac_vap->st_vap_base_info.uc_chip_id, OAL_FALSE);
    }

    /* 抛加入完成事件到WAL */
    pst_event_mem = frw_event_alloc_m(OAL_SIZEOF(uint16_t));
    if (pst_event_mem == OAL_PTR_NULL) {
        oam_error_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
                       "{hmac_sta_handle_disassoc_rsp::pst_event_mem null.}");
        return;
    }

    /* 填写事件 */
    pst_event = frw_get_event_stru(pst_event_mem);

    frw_event_hdr_init(&(pst_event->st_event_hdr),
                       FRW_EVENT_TYPE_HOST_CTX,
                       HMAC_HOST_CTX_EVENT_SUB_TYPE_DISASOC_COMP_STA,
                       OAL_SIZEOF(uint16_t),
                       FRW_EVENT_PIPELINE_STAGE_0,
                       pst_hmac_vap->st_vap_base_info.uc_chip_id,
                       pst_hmac_vap->st_vap_base_info.uc_device_id,
                       pst_hmac_vap->st_vap_base_info.uc_vap_id);

    *((uint16_t *)pst_event->auc_event_data) = us_disasoc_reason_code; /* 事件payload填写的是错误码 */

    /* 分发事件 */
    frw_event_dispatch_event(pst_event_mem);
    frw_event_free_m(pst_event_mem);

    return;
}


OAL_STATIC uint32_t hmac_sta_rx_deauth_req(hmac_vap_stru *pst_hmac_vap, uint8_t *puc_mac_hdr,
                                             oal_bool_enum_uint8 en_is_protected)
{
    uint8_t auc_bssid[WLAN_MAC_ADDR_LEN] = { 0 };
    hmac_user_stru *pst_hmac_user_vap = OAL_PTR_NULL;
    uint16_t us_user_idx = 0xffff;
    uint32_t ul_ret;
    uint8_t *puc_da = OAL_PTR_NULL;
    uint8_t *puc_sa = OAL_PTR_NULL;
    uint32_t ul_ret_del_user;

    if (oal_any_null_ptr2(pst_hmac_vap, puc_mac_hdr)) {
        oam_error_log2(0, OAM_SF_AUTH, "{hmac_sta_rx_deauth_req::param null,%p %p.}",
                       (uintptr_t)pst_hmac_vap, (uintptr_t)puc_mac_hdr);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 增加接收到去认证帧或者去关联帧时的维测信息 */
    mac_rx_get_sa((mac_ieee80211_frame_stru *)puc_mac_hdr, &puc_sa);
    oam_warning_log4(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_AUTH, "{hmac_sta_rx_deauth_req::\
        Because of err_code[%d], received deauth or disassoc frame frome source addr, sa xx:xx:xx:%2x:%2x:%2x.}",
        *((uint16_t *)(puc_mac_hdr + MAC_80211_FRAME_LEN)), puc_sa[3], puc_sa[4], puc_sa[5]);

    mac_get_address2(puc_mac_hdr, auc_bssid, WLAN_MAC_ADDR_LEN);

    ul_ret = mac_vap_find_user_by_macaddr(&pst_hmac_vap->st_vap_base_info, auc_bssid, &us_user_idx);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_AUTH,
                         "{hmac_sta_rx_deauth_req::find user failed[%d],other bss deauth frame!}", ul_ret);
        return ul_ret;
    }
    pst_hmac_user_vap = mac_res_get_hmac_user(us_user_idx);
    if (pst_hmac_user_vap == OAL_PTR_NULL) {
        oam_error_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_AUTH,
                       "{hmac_sta_rx_deauth_req::pst_hmac_user_vap[%d] null.}", us_user_idx);
        /* 没有查到对应的USER,发送去认证消息 */
        hmac_mgmt_send_deauth_frame(&(pst_hmac_vap->st_vap_base_info), auc_bssid, MAC_NOT_AUTHED, OAL_FALSE);

        hmac_fsm_change_state(pst_hmac_vap, MAC_VAP_STATE_STA_FAKE_UP);
        /* 上报内核sta已经和某个ap去关联 */
        hmac_sta_handle_disassoc_rsp(pst_hmac_vap, *((uint16_t *)(puc_mac_hdr + MAC_80211_FRAME_LEN)));
        return OAL_FAIL;
    }

#if (_PRE_WLAN_FEATURE_PMF != _PRE_PMF_NOT_SUPPORT)
    /* 检查是否需要发送SA query request */
    if ((pst_hmac_user_vap->st_user_base_info.en_user_asoc_state == MAC_USER_STATE_ASSOC) &&
        (OAL_SUCC == hmac_pmf_check_err_code(&pst_hmac_user_vap->st_user_base_info,
                                                 en_is_protected, puc_mac_hdr))) {
        /* 在关联状态下收到未加密的ReasonCode 6/7需要启动SA Query流程 */
        ul_ret = hmac_start_sa_query(&pst_hmac_vap->st_vap_base_info, pst_hmac_user_vap,
                                         pst_hmac_user_vap->st_user_base_info.st_cap_info.bit_pmf_active);
        if (ul_ret != OAL_SUCC) {
            return OAL_ERR_CODE_PMF_SA_QUERY_START_FAIL;
        }

        return OAL_SUCC;
    }

#endif

    /* 如果该用户的管理帧加密属性不一致，丢弃该报文 */
    mac_rx_get_da((mac_ieee80211_frame_stru *)puc_mac_hdr, &puc_da);
    if ((OAL_TRUE != ether_is_multicast(puc_da)) &&
        (en_is_protected != pst_hmac_user_vap->st_user_base_info.st_cap_info.bit_pmf_active)) {
        oam_error_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_AUTH,
                       "{hmac_sta_rx_deauth_req::PMF check failed.}");

        return OAL_FAIL;
    }

    /* TBD 上报system error 复位 mac */
    /* 删除user */
    ul_ret_del_user = hmac_user_del(&pst_hmac_vap->st_vap_base_info, pst_hmac_user_vap);
    if (ul_ret_del_user != OAL_SUCC) {
        oam_error_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_AUTH,
                       "{hmac_sta_rx_deauth_req::hmac_user_del failed.}");

        /* 上报内核sta已经和某个ap去关联 */
        hmac_sta_handle_disassoc_rsp(pst_hmac_vap, *((uint16_t *)(puc_mac_hdr + MAC_80211_FRAME_LEN)));
        return OAL_FAIL;
    }

    /* 上报内核sta已经和某个ap去关联 */
    hmac_sta_handle_disassoc_rsp(pst_hmac_vap, *((uint16_t *)(puc_mac_hdr + MAC_80211_FRAME_LEN)));

    return OAL_SUCC;
}


OAL_STATIC uint32_t hmac_sta_up_rx_beacon(hmac_vap_stru *pst_hmac_vap_sta, oal_netbuf_stru *pst_netbuf)
{
    dmac_rx_ctl_stru *pst_rx_ctrl = OAL_PTR_NULL;
    mac_rx_ctl_stru *pst_rx_info = OAL_PTR_NULL;
    mac_ieee80211_frame_stru *pst_mac_hdr = OAL_PTR_NULL;
    uint32_t ul_ret;
    uint16_t us_frame_len, us_frame_offset, us_user_idx;
    uint8_t *puc_frame_body = OAL_PTR_NULL;
    uint8_t uc_frame_sub_type;
    hmac_user_stru *pst_hmac_user = OAL_PTR_NULL;
    uint8_t auc_addr_sa[WLAN_MAC_ADDR_LEN];
#ifdef _PRE_WLAN_FEATURE_TXBF
    uint8_t *puc_txbf_vendor_ie;
#endif

    pst_rx_ctrl = (dmac_rx_ctl_stru *)oal_netbuf_cb(pst_netbuf);
    pst_rx_info = (mac_rx_ctl_stru *)(&(pst_rx_ctrl->st_rx_info));
    pst_mac_hdr = (mac_ieee80211_frame_stru *)MAC_GET_RX_CB_MAC_HEADER_ADDR(pst_rx_info);
    puc_frame_body = (uint8_t *)pst_mac_hdr + pst_rx_info->uc_mac_header_len;
    us_frame_len = pst_rx_info->us_frame_len - pst_rx_info->uc_mac_header_len; /* 帧体长度 */

    us_frame_offset = MAC_TIME_STAMP_LEN + MAC_BEACON_INTERVAL_LEN + MAC_CAP_INFO_LEN;
    uc_frame_sub_type = mac_get_frame_sub_type((uint8_t *)pst_mac_hdr);

    /* 来自其它bss的Beacon不做处理 */
    ul_ret = oal_compare_mac_addr(pst_hmac_vap_sta->st_vap_base_info.auc_bssid, pst_mac_hdr->auc_address3);
    if (ul_ret != 0) {
        return OAL_SUCC;
    }

    /* 获取管理帧的源地址SA */
    mac_get_address2((uint8_t *)pst_mac_hdr, auc_addr_sa, WLAN_MAC_ADDR_LEN);

    /* 根据SA 地地找到对应AP USER结构 */
    ul_ret = mac_vap_find_user_by_macaddr(&(pst_hmac_vap_sta->st_vap_base_info), auc_addr_sa, &us_user_idx);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(pst_hmac_vap_sta->st_vap_base_info.uc_vap_id, OAM_SF_RX,
                         "{hmac_sta_up_rx_beacon::mac_vap_find_user_by_macaddr failed[%d].}", ul_ret);
        return ul_ret;
    }
    pst_hmac_user = mac_res_get_hmac_user(us_user_idx);
    if (pst_hmac_user == OAL_PTR_NULL) {
        oam_error_log1(pst_hmac_vap_sta->st_vap_base_info.uc_vap_id, OAM_SF_RX,
                       "{hmac_sta_up_rx_beacon::pst_hmac_user[%d] null.}", us_user_idx);
        return OAL_ERR_CODE_PTR_NULL;
    }
#ifdef _PRE_WLAN_FEATURE_TXBF
    /* 更新11n txbf能力 */
    puc_txbf_vendor_ie = mac_find_vendor_ie(MAC_HUAWEI_VENDER_IE, MAC_EID_11NTXBF,
                                                (uint8_t *)pst_mac_hdr + MAC_TAG_PARAM_OFFSET,
                                                us_frame_len - us_frame_offset);
    hmac_mgmt_update_11ntxbf_cap(puc_txbf_vendor_ie, pst_hmac_user);
#endif

    /* 更新edca参数 */
    hmac_sta_up_update_edca_params(puc_frame_body + us_frame_offset, us_frame_len - us_frame_offset,
                                       pst_hmac_vap_sta, uc_frame_sub_type, pst_hmac_user);

#ifdef _PRE_WLAN_FEATURE_11AX
    if (g_wlan_spec_cfg->feature_11ax_is_open) {
        hmac_sta_up_update_he_edca_params(puc_frame_body + us_frame_offset, us_frame_len - us_frame_offset,
                                          pst_hmac_vap_sta, uc_frame_sub_type, pst_hmac_user);
        hmac_sta_up_update_he_nfrp_params(puc_frame_body + us_frame_offset,
            us_frame_len - us_frame_offset, pst_hmac_vap_sta, uc_frame_sub_type, pst_hmac_user);
    }
#endif

    return OAL_SUCC;
}


OAL_STATIC void hmac_sta_up_rx_action_nonuser(hmac_vap_stru *pst_hmac_vap, oal_netbuf_stru *pst_netbuf)
{
    dmac_rx_ctl_stru *pst_rx_ctrl;
    uint8_t *puc_data;
    mac_ieee80211_frame_stru *pst_frame_hdr; /* 保存mac帧的指针 */

    pst_rx_ctrl = (dmac_rx_ctl_stru *)oal_netbuf_cb(pst_netbuf);

    /* 获取帧头信息 */
    pst_frame_hdr = (mac_ieee80211_frame_stru *)MAC_GET_RX_CB_MAC_HEADER_ADDR(&pst_rx_ctrl->st_rx_info);

    /* 获取帧体指针 */
    puc_data = (uint8_t *)MAC_GET_RX_CB_MAC_HEADER_ADDR(&pst_rx_ctrl->st_rx_info) +
        pst_rx_ctrl->st_rx_info.uc_mac_header_len;

    /* Category */
    switch (puc_data[MAC_ACTION_OFFSET_CATEGORY]) {
        case MAC_ACTION_CATEGORY_PUBLIC: {
            /* Action */
            switch (puc_data[MAC_ACTION_OFFSET_ACTION]) {
                case MAC_PUB_VENDOR_SPECIFIC: {
#if defined(_PRE_WLAN_FEATURE_LOCATION) || defined(_PRE_WLAN_FEATURE_PSD_ANALYSIS)
                    if (0 == oal_memcmp(puc_data + MAC_ACTION_CATEGORY_AND_CODE_LEN, g_auc_huawei_oui, MAC_OUI_LEN)) {
                        oam_warning_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_RX,
                                         "{hmac_sta_up_rx_action_nonuser::hmac location get.}");
                        hmac_huawei_action_process(pst_hmac_vap, pst_netbuf,
                                                   puc_data[MAC_ACTION_CATEGORY_AND_CODE_LEN + MAC_OUI_LEN]);
                    }
#endif
                    break;
                }
                default:
                    break;
            }
        }
        break;

        default:
            break;
    }
    return;
}


void hmac_sta_rx_radio_measurment(hmac_vap_stru *pst_hmac_vap,
                                      hmac_user_stru *pst_hmac_user,
                                      oal_netbuf_stru *pst_netbuf,
                                      uint8_t *puc_data)
{
    if (pst_hmac_vap->bit_11k_enable == OAL_FALSE) {
        oam_warning_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_RX,
            "{hmac_sta_rx_radio_measurment::11k is not enable,do not process this action frame.}");
        return;
    }

    switch (puc_data[MAC_ACTION_OFFSET_ACTION]) {
#ifdef _PRE_WLAN_FEATURE_11K
        case MAC_RM_ACTION_NEIGHBOR_REPORT_RESPONSE:
            hmac_roam_rx_neighbor_response_action(pst_hmac_vap, pst_netbuf);
            break;
#endif  // _PRE_WLAN_FEATURE_11K

        default:
            oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_RX,
                             "{hmac_sta_rx_radio_measurment::action code[%d] invalid.}",
                             puc_data[MAC_ACTION_OFFSET_ACTION]);
            break;
    }

    return;
}

#ifdef _PRE_WLAN_FEATURE_WMMAC
OAL_STATIC void hmac_sta_up_rx_action_wmmac_qos_proc(hmac_vap_stru *pst_hmac_vap,
    hmac_user_stru *pst_hmac_user, uint8_t *puc_data)
{
    if (g_en_wmmac_switch == OAL_TRUE) {
        switch (puc_data[MAC_ACTION_OFFSET_ACTION]) {
            case MAC_WMMAC_ACTION_ADDTS_RSP:
                hmac_mgmt_rx_addts_rsp(pst_hmac_vap, pst_hmac_user, puc_data);
                break;
            case MAC_WMMAC_ACTION_DELTS:
                hmac_mgmt_rx_delts(pst_hmac_vap, pst_hmac_user, puc_data);
                break;
            default:
                break;
        }
    }
}
#endif  // _PRE_WLAN_FEATURE_WMMAC
#ifdef _PRE_WLAN_FEATURE_PSM_ABN_PKTS_STAT
OAL_STATIC void hmac_sta_up_rx_addba_req_stat(hmac_vap_stru *pst_hmac_vap)
{
    mac_vap_stru *pst_mac_vap;
    hmac_device_stru *pst_hmac_device;

    pst_mac_vap = &(pst_hmac_vap->st_vap_base_info);
    pst_hmac_device = (hmac_device_stru *)hmac_res_get_mac_dev(pst_mac_vap->uc_device_id);
    if (pst_hmac_device == OAL_PTR_NULL) {
        return;
    }
    // 只统计wlan0并处于暗屏状态的数量
    if ((pst_hmac_device->pst_device_base_info->uc_in_suspend) && (mac_is_primary_legacy_sta(pst_mac_vap))) {
        pst_hmac_device->ul_addba_req_cnt++;
    }
}
#endif


OAL_STATIC void hmac_sta_up_rx_action_ba(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    uint8_t *data, mac_ieee80211_frame_stru *frame_hdr)
{
    switch (data[MAC_ACTION_OFFSET_ACTION]) {
        case MAC_BA_ACTION_ADDBA_REQ:
            hmac_btcoex_check_rx_same_baw_start_from_addba_req(hmac_vap, hmac_user,
                frame_hdr, data);
#ifdef _PRE_WLAN_FEATURE_PSM_ABN_PKTS_STAT
            hmac_sta_up_rx_addba_req_stat(hmac_vap);
#endif
            hmac_mgmt_rx_addba_req(hmac_vap, hmac_user, data);
            break;
        case MAC_BA_ACTION_ADDBA_RSP:
            hmac_mgmt_rx_addba_rsp(hmac_vap, hmac_user, data);
            break;
        case MAC_BA_ACTION_DELBA:
            hmac_mgmt_rx_delba(hmac_vap, hmac_user, data);
            break;

        default:
            break;
    }
}

void hmac_mgmt_sta_roam_rx_sa_query(hmac_vap_stru *p_hmac_vap, oal_netbuf_stru *p_netbuf,
    uint8_t *data, oal_bool_enum_uint8 isProtected)
{
    switch (data[MAC_ACTION_OFFSET_ACTION]) {
        case MAC_SA_QUERY_ACTION_REQUEST:
            hmac_rx_sa_query_req(p_hmac_vap, p_netbuf, isProtected);
            break;
        default:
            break;
    }
}

void hmac_mgmt_sta_roam_rx_action(hmac_vap_stru *p_hmac_vap, oal_netbuf_stru *p_netbuf,
    oal_bool_enum_uint8 isProtected)
{
    dmac_rx_ctl_stru *p_rx_ctrl = OAL_PTR_NULL;
    uint8_t *data = OAL_PTR_NULL;
    hmac_user_stru *p_hmac_user = OAL_PTR_NULL;
    mac_ieee80211_frame_stru *p_mac_hdr = OAL_PTR_NULL;

    if (p_hmac_vap->st_vap_base_info.en_vap_state != MAC_VAP_STATE_ROAMING) {
        return;
    }

    p_rx_ctrl = (dmac_rx_ctl_stru *)oal_netbuf_cb(p_netbuf);
    p_mac_hdr = (mac_ieee80211_frame_stru *)MAC_GET_RX_CB_MAC_HEADER_ADDR(&p_rx_ctrl->st_rx_info);
    data = MAC_GET_RX_PAYLOAD_ADDR(&p_rx_ctrl->st_rx_info, p_netbuf);

    p_hmac_user = mac_res_get_hmac_user(p_hmac_vap->st_vap_base_info.us_assoc_vap_id);
    if (p_hmac_user == OAL_PTR_NULL) {
        return;
    }
    if (oal_compare_mac_addr(p_hmac_user->st_user_base_info.auc_user_mac_addr, p_mac_hdr->auc_address3)) {
        return;
    }

    switch (data[MAC_ACTION_OFFSET_CATEGORY]) {
#if (_PRE_WLAN_FEATURE_PMF != _PRE_PMF_NOT_SUPPORT)
        case MAC_ACTION_CATEGORY_SA_QUERY: {
            hmac_mgmt_sta_roam_rx_sa_query(p_hmac_vap, p_netbuf, data, isProtected);
            break;
        }
#endif
        default:
            break;
    }
}


OAL_STATIC void hmac_sta_up_rx_action(hmac_vap_stru *pst_hmac_vap,
                                          oal_netbuf_stru *pst_netbuf,
                                          oal_bool_enum_uint8 en_is_protected)
{
    dmac_rx_ctl_stru *pst_rx_ctrl = OAL_PTR_NULL;
    uint8_t *puc_data = OAL_PTR_NULL;
    mac_ieee80211_frame_stru *pst_frame_hdr = OAL_PTR_NULL; /* 保存mac帧的指针 */
    hmac_user_stru *pst_hmac_user = OAL_PTR_NULL;
    uint8_t *puc_p2p0_mac_addr;

    pst_rx_ctrl = (dmac_rx_ctl_stru *)oal_netbuf_cb(pst_netbuf);

    /* 获取帧头信息 */
    pst_frame_hdr = (mac_ieee80211_frame_stru *)MAC_GET_RX_CB_MAC_HEADER_ADDR(&pst_rx_ctrl->st_rx_info);
    /* P2P0设备所接受的action全部上报 */
    puc_p2p0_mac_addr = mac_mib_get_p2p0_dot11StationID(&pst_hmac_vap->st_vap_base_info);
    if (0 == oal_compare_mac_addr(pst_frame_hdr->auc_address1, puc_p2p0_mac_addr)) {
        hmac_rx_mgmt_send_to_host(pst_hmac_vap, pst_netbuf);
    }

    /* 获取发送端的用户指针 */
    pst_hmac_user = mac_vap_get_hmac_user_by_addr(&pst_hmac_vap->st_vap_base_info, pst_frame_hdr->auc_address2);
    if (pst_hmac_user == OAL_PTR_NULL) {
        oam_warning_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_RX,
                         "{hmac_sta_up_rx_action::mac_vap_find_user_by_macaddr failed.}");
        hmac_sta_up_rx_action_nonuser(pst_hmac_vap, pst_netbuf);
        return;
    }

    /* 获取帧体指针 */
    puc_data = (uint8_t *)MAC_GET_RX_CB_MAC_HEADER_ADDR(&pst_rx_ctrl->st_rx_info) +
        pst_rx_ctrl->st_rx_info.uc_mac_header_len;

    /* Category */
    switch (puc_data[MAC_ACTION_OFFSET_CATEGORY]) {
        case MAC_ACTION_CATEGORY_BA:
            hmac_sta_up_rx_action_ba(pst_hmac_vap, pst_hmac_user, puc_data, pst_frame_hdr);
            break;

        case MAC_ACTION_CATEGORY_WNM:
            oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_RX,
                             "{hmac_sta_up_rx_action::MAC_ACTION_CATEGORY_WNM action=%d.}",
                             puc_data[MAC_ACTION_OFFSET_ACTION]);
            switch (puc_data[MAC_ACTION_OFFSET_ACTION]) {
#ifdef _PRE_WLAN_FEATURE_11V_ENABLE
                /* bss transition request 帧处理入口 */
                case MAC_WNM_ACTION_BSS_TRANSITION_MGMT_REQUEST:
                    hmac_rx_bsst_req_action(pst_hmac_vap, pst_hmac_user, pst_netbuf);
                    break;
#endif
                default:
#ifdef _PRE_WLAN_FEATURE_HS20
                    /* 上报WNM Notification Request Action帧 */
                    hmac_rx_mgmt_send_to_host(pst_hmac_vap, pst_netbuf);
#endif
                    break;
            }
            break;

        case MAC_ACTION_CATEGORY_PUBLIC: {
            /* Action */
            switch (puc_data[MAC_ACTION_OFFSET_ACTION]) {
                case MAC_PUB_VENDOR_SPECIFIC:
                    /* 查找OUI-OUI type值为 50 6F 9A - 09 (WFA P2P v1.0)  */
                    /* 并用hmac_rx_mgmt_send_to_host接口上报 */
                    if (OAL_TRUE == mac_ie_check_p2p_action(puc_data + MAC_ACTION_CATEGORY_AND_CODE_LEN)) {
                        hmac_rx_mgmt_send_to_host(pst_hmac_vap, pst_netbuf);
                    }
#if defined(_PRE_WLAN_FEATURE_LOCATION) || defined(_PRE_WLAN_FEATURE_PSD_ANALYSIS)
                    if (0 == oal_memcmp(puc_data + MAC_ACTION_CATEGORY_AND_CODE_LEN, g_auc_huawei_oui, MAC_OUI_LEN)) {
                        oam_warning_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_RX,
                                         "{hmac_sta_up_rx_action::hmac location get.}");
                        hmac_huawei_action_process(pst_hmac_vap, pst_netbuf,
                                                   puc_data[MAC_ACTION_CATEGORY_AND_CODE_LEN + MAC_OUI_LEN]);
                    }
#endif
                    break;

                case MAC_PUB_GAS_INIT_RESP:
                case MAC_PUB_GAS_COMBAK_RESP:
#ifdef _PRE_WLAN_FEATURE_HS20
                    /* 上报GAS查询的ACTION帧 */
                    hmac_rx_mgmt_send_to_host(pst_hmac_vap, pst_netbuf);
#endif
                    break;

                default:
                    break;
            }
        }
        break;

#ifdef _PRE_WLAN_FEATURE_WMMAC
        case MAC_ACTION_CATEGORY_WMMAC_QOS: {
            hmac_sta_up_rx_action_wmmac_qos_proc(pst_hmac_vap, pst_hmac_user, puc_data);
        }
        break;
#endif  // _PRE_WLAN_FEATURE_WMMAC

#if (_PRE_WLAN_FEATURE_PMF != _PRE_PMF_NOT_SUPPORT)
        case MAC_ACTION_CATEGORY_SA_QUERY: {
            switch (puc_data[MAC_ACTION_OFFSET_ACTION]) {
                case MAC_SA_QUERY_ACTION_REQUEST:
                    hmac_rx_sa_query_req(pst_hmac_vap, pst_netbuf, en_is_protected);
                    break;
                case MAC_SA_QUERY_ACTION_RESPONSE:
                    hmac_rx_sa_query_rsp(pst_hmac_vap, pst_netbuf, en_is_protected);
                    break;

                default:
                    break;
            }
        }
        break;
#endif
        case MAC_ACTION_CATEGORY_VENDOR: {
            /* 查找OUI-OUI type值为 50 6F 9A - 09 (WFA P2P v1.0)  */
            /* 并用hmac_rx_mgmt_send_to_host接口上报 */
            if (OAL_TRUE == mac_ie_check_p2p_action(puc_data + MAC_ACTION_CATEGORY_AND_CODE_LEN)) {
                hmac_rx_mgmt_send_to_host(pst_hmac_vap, pst_netbuf);
            }
        }
        break;
#ifdef _PRE_WLAN_FEATURE_11R
        case MAC_ACTION_CATEGORY_FAST_BSS_TRANSITION: {
            if (pst_hmac_vap->bit_11r_enable != OAL_TRUE) {
                break;
            }
            hmac_roam_rx_ft_action(pst_hmac_vap, pst_netbuf);
            break;
        }
#endif  // _PRE_WLAN_FEATURE_11R
        case MAC_ACTION_CATEGORY_VHT: {
            switch (puc_data[MAC_ACTION_OFFSET_ACTION]) {
#ifdef _PRE_WLAN_FEATURE_OPMODE_NOTIFY
                case MAC_VHT_ACTION_OPREATING_MODE_NOTIFICATION:
                    hmac_mgmt_rx_opmode_notify_frame(pst_hmac_vap, pst_hmac_user, pst_netbuf);
                    break;
#endif
                case MAC_VHT_ACTION_BUTT:
                default:
                    break;
            }
        }
        break;

        case MAC_ACTION_CATEGORY_RADIO_MEASURMENT: {
            hmac_sta_rx_radio_measurment(pst_hmac_vap, pst_hmac_user, pst_netbuf, puc_data);
        }
        break;
#ifdef _PRE_WLAN_FEATURE_11AX
        case MAC_ACTION_CATEGORY_S1G: {
            if (g_wlan_spec_cfg->feature_11ax_is_open) {
                switch (puc_data[MAC_ACTION_OFFSET_ACTION]) {
#ifdef _PRE_WLAN_FEATURE_TWT
                    case MAC_S1G_ACTION_TWT_SETUP:
                        hmac_sta_rx_twt_setup_frame(pst_hmac_vap, pst_hmac_user, puc_data);
                        break;

                    case MAC_S1G_ACTION_TWT_TEARDOWN:
                        hmac_sta_rx_twt_teardown_frame(pst_hmac_vap, pst_hmac_user, puc_data);
                        break;

                    case MAC_S1G_ACTION_TWT_INFORMATION:
                        hmac_sta_rx_twt_information_frame(pst_hmac_vap, pst_hmac_user, puc_data);
                        break;
#endif
                    default:
                        break;
                }
                break;
            }
        }
            /* fall through */
#endif

        default:
            break;
    }
}

OAL_STATIC void hmac_sta_up_rx_deauth_disassoc(hmac_vap_stru *pst_hmac_vap_sta,
    mac_rx_ctl_stru *pst_rx_info, uint8_t *puc_mac_hdr, oal_bool_enum_uint8 en_is_protected)
{
    if (pst_rx_info->us_frame_len < pst_rx_info->uc_mac_header_len + MAC_80211_REASON_CODE_LEN) {
        oam_warning_log1(pst_hmac_vap_sta->st_vap_base_info.uc_vap_id, OAM_SF_RX,
                         "hmac_sta_up_rx_mgmt:: invalid deauth_req length[%d]}", pst_rx_info->us_frame_len);
    } else {
        hmac_sta_rx_deauth_req(pst_hmac_vap_sta, puc_mac_hdr, en_is_protected);
    }
}

uint32_t hmac_sta_up_rx_mgmt(hmac_vap_stru *pst_hmac_vap_sta, void *p_param)
{
    dmac_wlan_crx_event_stru *pst_mgmt_rx_event = OAL_PTR_NULL;
    dmac_rx_ctl_stru *pst_rx_ctrl = OAL_PTR_NULL;
    mac_rx_ctl_stru *pst_rx_info = OAL_PTR_NULL;
    uint8_t *puc_mac_hdr = OAL_PTR_NULL;
    uint8_t uc_mgmt_frm_type;
    oal_bool_enum_uint8 en_is_protected;
#ifdef _PRE_WLAN_WAKEUP_SRC_PARSE
    uint8_t *puc_data;
#endif

    if (oal_any_null_ptr2(pst_hmac_vap_sta, p_param)) {
        oam_error_log2(0, OAM_SF_RX, "{hmac_sta_up_rx_mgmt::param null,%x %x.}",
                       (uintptr_t)pst_hmac_vap_sta, (uintptr_t)p_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_mgmt_rx_event = (dmac_wlan_crx_event_stru *)p_param;
    pst_rx_ctrl = (dmac_rx_ctl_stru *)oal_netbuf_cb(pst_mgmt_rx_event->pst_netbuf);
    pst_rx_info = (mac_rx_ctl_stru *)(&(pst_rx_ctrl->st_rx_info));
    puc_mac_hdr = (uint8_t *)MAC_GET_RX_CB_MAC_HEADER_ADDR(pst_rx_info);
    en_is_protected = mac_is_protectedframe(puc_mac_hdr);
    if (en_is_protected >= OAL_BUTT) {
        oam_warning_log1(0, OAM_SF_RX, "{hmac_sta_up_rx_mgmt::en_is_protected is %d.}", en_is_protected);
        return OAL_SUCC;
    }

    /* STA在UP状态下 接收到的各种管理帧处理 */
    uc_mgmt_frm_type = mac_get_frame_sub_type(puc_mac_hdr);

#ifdef _PRE_WLAN_WAKEUP_SRC_PARSE
    if (OAL_TRUE == wlan_pm_wkup_src_debug_get()) {
        wlan_pm_wkup_src_debug_set(OAL_FALSE);
        oam_warning_log2(pst_hmac_vap_sta->st_vap_base_info.uc_vap_id, OAM_SF_RX,
                         "{wifi_wake_src:hmac_sta_up_rx_mgmt::wakeup frame type[0x%x] sub type[0x%x]}",
                         mac_get_frame_type(puc_mac_hdr), uc_mgmt_frm_type);
        /* action帧唤醒时打印action帧类型 */
        if ((WLAN_FC0_SUBTYPE_ACTION | WLAN_FC0_TYPE_MGT) == mac_get_frame_type_and_subtype(puc_mac_hdr)) {
            /* 获取帧体指针 */
            puc_data = (uint8_t *)MAC_GET_RX_CB_MAC_HEADER_ADDR(&pst_rx_ctrl->st_rx_info) +
                pst_rx_ctrl->st_rx_info.uc_mac_header_len;
            oam_warning_log2(pst_hmac_vap_sta->st_vap_base_info.uc_vap_id, OAM_SF_RX,
                "{wifi_wake_src:hmac_sta_up_rx_mgmt::wakeup action category[0x%x], action details[0x%x]}",
                puc_data[MAC_ACTION_OFFSET_CATEGORY], puc_data[MAC_ACTION_OFFSET_ACTION]);
        }
    }
#endif

    /* Bar frame proc here */
    if (WLAN_FC0_TYPE_CTL == mac_get_frame_type(puc_mac_hdr)) {
        uc_mgmt_frm_type = mac_get_frame_sub_type(puc_mac_hdr);
        if (WLAN_FC0_SUBTYPE_BAR == uc_mgmt_frm_type) {
#ifdef _PRE_WLAN_FEATURE_SNIFFER
#ifdef CONFIG_HW_SNIFFER
            proc_sniffer_write_file(NULL, 0, puc_mac_hdr, pst_rx_info->us_frame_len, 0);
#endif
#endif
            hmac_up_rx_bar(pst_hmac_vap_sta, pst_rx_ctrl, pst_mgmt_rx_event->pst_netbuf);
        }
    } else if (WLAN_FC0_TYPE_MGT == mac_get_frame_type(puc_mac_hdr)) {
#ifdef _PRE_WLAN_FEATURE_SNIFFER
#ifdef CONFIG_HW_SNIFFER
        proc_sniffer_write_file(NULL, 0, puc_mac_hdr, pst_rx_info->us_frame_len, 0);
#endif
#endif
        switch (uc_mgmt_frm_type) {
            case WLAN_FC0_SUBTYPE_DEAUTH:
            case WLAN_FC0_SUBTYPE_DISASSOC:
                hmac_sta_up_rx_deauth_disassoc(pst_hmac_vap_sta, pst_rx_info, puc_mac_hdr, en_is_protected);
                break;

            case WLAN_FC0_SUBTYPE_BEACON:
                hmac_sta_up_rx_beacon(pst_hmac_vap_sta, pst_mgmt_rx_event->pst_netbuf);
                break;

            case WLAN_FC0_SUBTYPE_ACTION:
                hmac_sta_up_rx_action(pst_hmac_vap_sta, pst_mgmt_rx_event->pst_netbuf, en_is_protected);
                break;
            default:
                break;
        }
    }

    return OAL_SUCC;
}

/*lint -e578*/ /*lint -e19*/
oal_module_symbol(hmac_check_capability_mac_phy_supplicant);
/*lint +e578*/ /*lint +e19*/
