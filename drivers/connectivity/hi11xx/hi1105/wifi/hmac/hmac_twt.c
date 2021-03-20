

#ifdef _PRE_WLAN_FEATURE_TWT

/* 1 头文件包含 */
#include "oal_util.h"
#include "hmac_resource.h"
#include "hmac_mgmt_bss_comm.h"
#include "hmac_hcc_adapt.h"
#include "hmac_twt.h"
#include "securec.h"
#include "securectype.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_TWT_C

/* 2 静态函数声明 */
/* 3 全局变量定义 */
/* 4 函数实现 */
void hmac_twt_init(hmac_vap_stru *hmac_vap, mac_twt_element_ie_individual_stru *twt_element,
    mac_twt_action_mgmt_args_stru *twt_action_args)
{
    /*
     * TWT argument
     * -----------------------------------------
     * |setup_cmd |flow_type |flow_ID |Reserve |
     * -----------------------------------------
     * |1         |1         |1       |1       |
     * -----------------------------------------
     */
    twt_element->uc_category = MAC_ACTION_CATEGORY_S1G;
    twt_element->uc_action = MAC_S1G_ACTION_TWT_SETUP;
    twt_element->uc_dialog_token = hmac_vap->st_twt_cfg.uc_twt_dialog_token;
    twt_element->uc_element_id = MAC_EID_TWT;
    twt_element->uc_len = sizeof(mac_twt_element_ie_individual_stru) - 5; /* 5表示twt头部长度 */
    twt_element->st_control.bit_ndp_paging_indicator = 0;
    twt_element->st_control.bit_responder_pm_mode = 0;
    twt_element->st_control.bit_negotiation = 0;
    twt_element->st_control.bit_resv = 0;
    twt_element->st_request_type.bit_request = 1; /* 如果支持twt responder，回复setup帧时把该位清除 */
    twt_element->st_request_type.bit_setup_command = twt_action_args->uc_twt_setup_cmd; /* 由user决定 */
    /* 默认trigger-enabled TWT，与认证用例对齐，该位最终由AP决定 */
    twt_element->st_request_type.bit_trigger = 1;
    twt_element->st_request_type.bit_implicit = 1;
    /* 由user决定, annouced or unannounced */
    twt_element->st_request_type.bit_flow_type = twt_action_args->uc_twt_flow_type;
    twt_element->st_request_type.bit_flow_id = twt_action_args->uc_twt_flow_id; /* 由user决定 */
    twt_element->st_request_type.bit_exponent = twt_action_args->ul_twt_exponent; /* unit: microsecond(us) */
    twt_element->st_request_type.bit_protection = 0;
    twt_element->ull_twt = twt_action_args->ul_twt_start_time_offset;
    twt_element->uc_min_duration = twt_action_args->ul_twt_duration; /* user decide, unit: 256us */
    twt_element->us_mantissa = twt_action_args->ul_intrval_mantissa; /* user decide, unit: microsecond(us) */
    twt_element->uc_channel = 0;
}

uint16_t hmac_mgmt_encap_twt_setup_req(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user, uint8_t *data,
    mac_twt_action_mgmt_args_stru *twt_action_args)
{
    uint16_t index;
    mac_twt_element_ie_individual_stru twt_element;
    if (oal_any_null_ptr3(hmac_vap, data, twt_action_args)) {
        oam_error_log3(0, OAM_SF_11AX, "{hmac_mgmt_encap_twt_setup_req::null param.vap:%x data:%x twt:%x}",
                       (uintptr_t)hmac_vap, (uintptr_t)data, (uintptr_t)twt_action_args);
        return OAL_ERR_CODE_PTR_NULL;
    }
    memset_s((uint8_t *)&twt_element, sizeof(mac_twt_element_ie_individual_stru),
             0, sizeof(mac_twt_element_ie_individual_stru));
    oam_warning_log3(0, OAM_SF_11AX, "{hmac_mgmt_encap_twt_setup_req:: mac_addr[%02x XX XX XX %02x %02x]!.}",
                     hmac_user->st_user_base_info.auc_user_mac_addr[0],
                     hmac_user->st_user_base_info.auc_user_mac_addr[4], /* 4表示MAC地址的第五位 */
                     hmac_user->st_user_base_info.auc_user_mac_addr[5]); /* 5表示MAC地址的第六位 */
    /*
     * TWT element
     * --------------------------------------------------------
     * |Element ID |Length |Control |TWT Parameter Information|
     * --------------------------------------------------------
     * |1          |1      |1       |variable                 |
     * --------------------------------------------------------
     */
    /* Set the fields in the frame header */
    /* Frame Control Field 中只需要设置Type/Subtype值，其他设置为0 */
    mac_hdr_set_frame_control(data, WLAN_PROTOCOL_VERSION | WLAN_FC0_TYPE_MGT | WLAN_FC0_SUBTYPE_ACTION);

    /* DA is address of STA requesting association */
    oal_set_mac_addr(data + HMAC_80211_FC_AND_DID_LEN, hmac_user->st_user_base_info.auc_user_mac_addr);

    /* SA的值为dot11MACAddress的值 */
    oal_set_mac_addr(data + HMAC_80211_FC_AND_DID_LEN + MAC_LEN, mac_mib_get_StationID(&hmac_vap->st_vap_base_info));

    oal_set_mac_addr(data + HMAC_80211_FC_AND_DID_LEN + MAC_LEN + MAC_LEN, hmac_vap->st_vap_base_info.auc_bssid);

    /* Set the contents of the frame body */
    /* 将索引指向frame body起始位置 */
    index = MAC_80211_FRAME_LEN;

    hmac_twt_init(hmac_vap, &twt_element, twt_action_args);
    oam_warning_log4(0, OAM_SF_11AX,
        "{hmac_mgmt_encap_twt_setup_req::entry bit_setup_command:%d bit_flow_type:%d bit_flow_id:%d trigger:%d}",
        twt_element.st_request_type.bit_setup_command, twt_element.st_request_type.bit_flow_type,
        twt_element.st_request_type.bit_flow_id, twt_element.st_request_type.bit_trigger);
    oam_warning_log3(0, OAM_SF_11AX,
        "{hmac_mgmt_encap_twt_setup_req::entry ull_twt:%d uc_min_duration:%d us_mantissa:%d}",
        twt_element.ull_twt, twt_element.uc_min_duration, twt_element.us_mantissa);

    if (memcpy_s((uint8_t *)(data + MAC_80211_FRAME_LEN), sizeof(mac_twt_element_ie_individual_stru),
        (uint8_t *)&twt_element, sizeof(mac_twt_element_ie_individual_stru)) != EOK) {
        oam_error_log0(0, OAM_SF_11AX, "hmac_mgmt_encap_twt_setup_req::memcpy fail!");
        return index;
    }
    index = MAC_80211_FRAME_LEN + sizeof(mac_twt_element_ie_individual_stru);
    /* 返回的帧长度中不包括FCS */
    return index;
}


uint16_t hmac_mgmt_encap_twt_teardown_req(hmac_vap_stru *pst_vap,
                                          hmac_user_stru *pst_hmac_user,
                                          uint8_t *puc_data,
                                          mac_twt_action_mgmt_args_stru *pst_twt_action_args)
{
    uint16_t us_index;
    mac_twt_teardown_stru pst_twt_teardown;
    if (oal_any_null_ptr3(pst_vap, puc_data, pst_twt_action_args)) {
        oam_error_log3(0, OAM_SF_11AX, "{hmac_mgmt_encap_twt_teardown_req::null param.vap:%x data:%x twt:%x}",
                       (uintptr_t)pst_vap, (uintptr_t)puc_data, (uintptr_t)pst_twt_action_args);
        return OAL_ERR_CODE_PTR_NULL;
    }
    memset_s((uint8_t *)&pst_twt_teardown, OAL_SIZEOF(mac_twt_teardown_stru), 0, OAL_SIZEOF(mac_twt_teardown_stru));
    /*************************************************************************/
    /*                Set the fields in the frame header                     */
    /*************************************************************************/
    /* Frame Control Field 中只需要设置Type/Subtype值，其他设置为0 */
    mac_hdr_set_frame_control(puc_data, WLAN_PROTOCOL_VERSION | WLAN_FC0_TYPE_MGT | WLAN_FC0_SUBTYPE_ACTION);

    /* DA is address of STA requesting association */
    oal_set_mac_addr(puc_data + 4, pst_hmac_user->st_user_base_info.auc_user_mac_addr); // 前4字节为DA前的字段

    /* SA的值为dot11MACAddress的值 */
    oal_set_mac_addr(puc_data + 10, mac_mib_get_StationID(&pst_vap->st_vap_base_info)); // 前10字节为SA前的字段

    oal_set_mac_addr(puc_data + 16, pst_vap->st_vap_base_info.auc_bssid);              // 前16字节为bssid前的字段

    /*************************************************************************/
    /*                Set the contents of the frame body                     */
    /*************************************************************************/
    /* 将索引指向frame body起始位置 */
    us_index = MAC_80211_FRAME_LEN;

    pst_twt_teardown.uc_category = MAC_ACTION_CATEGORY_S1G;
    pst_twt_teardown.uc_action = MAC_S1G_ACTION_TWT_TEARDOWN;
    pst_twt_teardown.bit_twt_flow_id = pst_twt_action_args->uc_twt_flow_id; /* 由user决定 */
    pst_twt_teardown.bit_nego_type = 0;

    if (EOK != memcpy_s((uint8_t *)(puc_data + MAC_80211_FRAME_LEN), OAL_SIZEOF(mac_twt_teardown_stru),
        (uint8_t *)&pst_twt_teardown, OAL_SIZEOF(mac_twt_teardown_stru))) {
        oam_error_log0(0, OAM_SF_11AX, "hmac_mgmt_encap_twt_teardown_req::memcpy fail!");
        return us_index;
    }
    us_index = MAC_80211_FRAME_LEN + OAL_SIZEOF(mac_twt_teardown_stru);
    /* 返回的帧长度中不包括FCS */
    return us_index;
}


uint32_t hmac_mgmt_tx_twt_setup_precheck(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    mac_twt_action_mgmt_args_stru *twt_action_args, mac_vap_stru **mac_vap)
{
    mac_device_stru *mac_device;

    if (oal_any_null_ptr3(hmac_vap, hmac_user, twt_action_args)) {
        oam_error_log3(0, OAM_SF_11AX, "{hmac_mgmt_tx_twt_setup::null param, %x %x %x.}",
                       (uintptr_t)hmac_vap, (uintptr_t)hmac_user, (uintptr_t)twt_action_args);
        return OAL_ERR_CODE_PTR_NULL;
    }

    *mac_vap = &(hmac_vap->st_vap_base_info);
    mac_device = mac_res_get_dev((*mac_vap)->uc_device_id);
    if (mac_device == OAL_PTR_NULL) {
        oam_error_log0((*mac_vap)->uc_vap_id, OAM_SF_11AX, "{hmac_mgmt_tx_twt_setup::pst_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t hmac_mgmt_tx_twt_steup_alloc_and_encap(hmac_user_stru *hmac_user,
    mac_twt_action_mgmt_args_stru *twt_action_args, oal_netbuf_stru **twt_setup_req, hmac_vap_stru **hmac_vap,
    uint16_t *frame_len)
{
    dmac_ctx_action_event_stru wlan_ctx_action;
    mac_tx_ctl_stru *tx_ctl = NULL;

    /* 申请TWT SETUP管理帧内存 */
    *twt_setup_req = oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF, WLAN_MEM_NETBUF_SIZE2, OAL_NETBUF_PRIORITY_MID);
    if (*twt_setup_req == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_11AX, "{hmac_mgmt_tx_twt_setup::pst_twt_setup_req null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    oal_mem_netbuf_trace(*twt_setup_req, OAL_TRUE);
    oal_netbuf_prev(*twt_setup_req) = OAL_PTR_NULL;
    oal_netbuf_next(*twt_setup_req) = OAL_PTR_NULL;

    (*hmac_vap)->st_twt_cfg.uc_twt_dialog_token++;
    oam_warning_log1(0, OAM_SF_11AX, "{hmac_mgmt_tx_twt_setup::token,%d}", (*hmac_vap)->st_twt_cfg.uc_twt_dialog_token);

    /* 调用封装管理帧接口 */
    *frame_len = hmac_mgmt_encap_twt_setup_req(*hmac_vap, hmac_user, oal_netbuf_data(*twt_setup_req), twt_action_args);

    memset_s((uint8_t *)&wlan_ctx_action, sizeof(wlan_ctx_action), 0, sizeof(wlan_ctx_action));

    /* 赋值要传入Dmac的信息 */
    wlan_ctx_action.us_frame_len = *frame_len;
    wlan_ctx_action.uc_hdr_len = MAC_80211_FRAME_LEN;
    wlan_ctx_action.en_action_category = MAC_ACTION_CATEGORY_S1G;
    wlan_ctx_action.uc_action = MAC_S1G_ACTION_TWT_SETUP;
    wlan_ctx_action.us_user_idx = hmac_user->st_user_base_info.us_assoc_id;

    if (memcpy_s((uint8_t *)(oal_netbuf_data(*twt_setup_req) + (*frame_len)), sizeof(dmac_ctx_action_event_stru),
        (uint8_t *)&wlan_ctx_action, sizeof(dmac_ctx_action_event_stru)) != EOK) {
        oam_error_log0(0, OAM_SF_11AX, "hmac_mgmt_tx_twt_setup::memcpy fail!");
        oal_netbuf_free(*twt_setup_req);
        return OAL_FAIL;
    }
    oal_netbuf_put(*twt_setup_req, (*frame_len + sizeof(dmac_ctx_action_event_stru)));
    /* 初始化CB */
    memset_s(oal_netbuf_cb(*twt_setup_req), oal_netbuf_cb_size(), 0, oal_netbuf_cb_size());
    tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(*twt_setup_req);
    MAC_GET_CB_MPDU_LEN(tx_ctl) = *frame_len + sizeof(dmac_ctx_action_event_stru);
    MAC_GET_CB_FRAME_TYPE(tx_ctl) = WLAN_CB_FRAME_TYPE_ACTION;
    MAC_GET_CB_FRAME_SUBTYPE(tx_ctl) = WLAN_ACTION_TWT_SETUP_REQ;

    return OAL_SUCC;
}


uint32_t hmac_mgmt_tx_twt_setup(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    mac_twt_action_mgmt_args_stru *twt_action_args)
{
    mac_vap_stru *mac_vap = NULL;
    frw_event_mem_stru *event_mem;
    oal_netbuf_stru *twt_setup_req = NULL;
    uint16_t frame_len = 0;
    frw_event_stru *pst_hmac_to_dmac_ctx_event;
    dmac_tx_event_stru *tx_event;
    uint32_t ret;

    ret = hmac_mgmt_tx_twt_setup_precheck(hmac_vap, hmac_user, twt_action_args, &mac_vap);
    if (ret != OAL_SUCC) {
        return ret;
    }

    ret = hmac_mgmt_tx_twt_steup_alloc_and_encap(hmac_user, twt_action_args, &twt_setup_req, &hmac_vap, &frame_len);
    if (ret != OAL_SUCC) {
        return ret;
    }

    event_mem = frw_event_alloc_m(sizeof(dmac_tx_event_stru));
    if (event_mem == OAL_PTR_NULL) {
        oam_error_log0(mac_vap->uc_vap_id, OAM_SF_11AX, "{hmac_mgmt_tx_twt_setup::pst_event_mem null.}");
        oal_netbuf_free(twt_setup_req);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hmac_to_dmac_ctx_event = frw_get_event_stru(event_mem);
    frw_event_hdr_init(&(pst_hmac_to_dmac_ctx_event->st_event_hdr), FRW_EVENT_TYPE_WLAN_CTX,
        DMAC_WLAN_CTX_EVENT_SUB_TYPE_MGMT, sizeof(dmac_tx_event_stru), FRW_EVENT_PIPELINE_STAGE_1,
        mac_vap->uc_chip_id, mac_vap->uc_device_id, mac_vap->uc_vap_id);

    tx_event = (dmac_tx_event_stru *)(pst_hmac_to_dmac_ctx_event->auc_event_data);
    tx_event->pst_netbuf = twt_setup_req;
    tx_event->us_frame_len = frame_len;

    ret = frw_event_dispatch_event(event_mem);
    if (ret != OAL_SUCC) {
        oam_error_log1(mac_vap->uc_vap_id, OAM_SF_11AX,
                       "{hmac_mgmt_tx_twt_setup::frw_event_dispatch_event failed[%d].}", ret);
        oal_netbuf_free(twt_setup_req);
        frw_event_free_m(event_mem);
        return ret;
    }

    frw_event_free_m(event_mem);
    return OAL_SUCC;
}


uint32_t hmac_mgmt_tx_twt_teardown(hmac_vap_stru *pst_hmac_vap,
    hmac_user_stru *pst_hmac_user, mac_twt_action_mgmt_args_stru *pst_twt_action_args)
{
    mac_device_stru *pst_device;
    mac_vap_stru *pst_mac_vap;
    frw_event_mem_stru *pst_event_mem; /* 申请事件返回的内存指针 */
    oal_netbuf_stru *pst_twt_setup_req;
    uint16_t us_frame_len;
    frw_event_stru *pst_hmac_to_dmac_ctx_event;
    dmac_tx_event_stru *pst_tx_event;
    dmac_ctx_action_event_stru st_wlan_ctx_action;
    uint32_t ul_ret;
    mac_tx_ctl_stru *pst_tx_ctl;

    if (oal_any_null_ptr3(pst_hmac_vap, pst_hmac_user, pst_twt_action_args)) {
        oam_error_log3(0, OAM_SF_11AX, "{hmac_mgmt_tx_twt_teardown::null param, %x %x %x.}",
                       (uintptr_t)pst_hmac_vap, (uintptr_t)pst_hmac_user, (uintptr_t)pst_twt_action_args);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_mac_vap = &(pst_hmac_vap->st_vap_base_info);

    /* 获取device结构 */
    pst_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (pst_device == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_11AX, "{hmac_mgmt_tx_twt_teardown::pst_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 申请TWT SETUP管理帧内存 */
    pst_twt_setup_req = oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF, WLAN_MEM_NETBUF_SIZE2, OAL_NETBUF_PRIORITY_MID);
    if (pst_twt_setup_req == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_11AX,
                       "{hmac_mgmt_tx_twt_teardown::pst_twt_teardown_req null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    oal_mem_netbuf_trace(pst_twt_setup_req, OAL_TRUE);

    oal_netbuf_prev(pst_twt_setup_req) = OAL_PTR_NULL;
    oal_netbuf_next(pst_twt_setup_req) = OAL_PTR_NULL;

    /* 调用封装管理帧接口 */
    us_frame_len = hmac_mgmt_encap_twt_teardown_req(pst_hmac_vap, pst_hmac_user,
                                                    oal_netbuf_data(pst_twt_setup_req),
                                                    pst_twt_action_args);
    memset_s((uint8_t *)&st_wlan_ctx_action, OAL_SIZEOF(st_wlan_ctx_action), 0, OAL_SIZEOF(st_wlan_ctx_action));
    /* 赋值要传入Dmac的信息 */
    st_wlan_ctx_action.us_frame_len = us_frame_len;
    st_wlan_ctx_action.uc_hdr_len = MAC_80211_FRAME_LEN;
    st_wlan_ctx_action.en_action_category = MAC_ACTION_CATEGORY_S1G;
    st_wlan_ctx_action.uc_action = MAC_S1G_ACTION_TWT_TEARDOWN;
    st_wlan_ctx_action.us_user_idx = pst_hmac_user->st_user_base_info.us_assoc_id;

    if (EOK != memcpy_s((uint8_t *)(oal_netbuf_data(pst_twt_setup_req) + us_frame_len),
        OAL_SIZEOF(dmac_ctx_action_event_stru), (uint8_t *)&st_wlan_ctx_action,
        OAL_SIZEOF(dmac_ctx_action_event_stru))) {
        oam_error_log0(0, OAM_SF_11AX, "hmac_mgmt_tx_twt_teardown::memcpy fail!");
        oal_netbuf_free(pst_twt_setup_req);
        return OAL_FAIL;
    }
    oal_netbuf_put(pst_twt_setup_req, (us_frame_len + OAL_SIZEOF(dmac_ctx_action_event_stru)));

    /* 初始化CB */
    memset_s(oal_netbuf_cb(pst_twt_setup_req), oal_netbuf_cb_size(), 0, oal_netbuf_cb_size());
    pst_tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_twt_setup_req);
    MAC_GET_CB_MPDU_LEN(pst_tx_ctl) = us_frame_len + OAL_SIZEOF(dmac_ctx_action_event_stru);
    MAC_GET_CB_FRAME_TYPE(pst_tx_ctl) = WLAN_CB_FRAME_TYPE_ACTION;
    /* 在dmac_tx_action_process  skip, 直接send to air */
    MAC_GET_CB_FRAME_SUBTYPE(pst_tx_ctl) = WLAN_ACTION_TWT_TEARDOWN_REQ;

    pst_event_mem = frw_event_alloc_m(OAL_SIZEOF(dmac_tx_event_stru));
    if (pst_event_mem == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_11AX, "{hmac_mgmt_tx_twt_teardown::pst_event_mem null.}");
        oal_netbuf_free(pst_twt_setup_req);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hmac_to_dmac_ctx_event = frw_get_event_stru(pst_event_mem);
    frw_event_hdr_init(&(pst_hmac_to_dmac_ctx_event->st_event_hdr),
                       FRW_EVENT_TYPE_WLAN_CTX,
                       DMAC_WLAN_CTX_EVENT_SUB_TYPE_MGMT,
                       OAL_SIZEOF(dmac_tx_event_stru),
                       FRW_EVENT_PIPELINE_STAGE_1,
                       pst_mac_vap->uc_chip_id,
                       pst_mac_vap->uc_device_id,
                       pst_mac_vap->uc_vap_id);

    pst_tx_event = (dmac_tx_event_stru *)(pst_hmac_to_dmac_ctx_event->auc_event_data);
    pst_tx_event->pst_netbuf = pst_twt_setup_req;
    pst_tx_event->us_frame_len = us_frame_len;

    ul_ret = frw_event_dispatch_event(pst_event_mem);
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_11AX,
                       "{hmac_mgmt_tx_twt_teardown::frw_event_dispatch_event failed[%d].}", ul_ret);
        oal_netbuf_free(pst_twt_setup_req);
        frw_event_free_m(pst_event_mem);
        return ul_ret;
    }

    pst_hmac_vap->st_twt_cfg.uc_twt_session_enable = 0;

    frw_event_free_m(pst_event_mem);
    return OAL_SUCC;
}


OAL_STATIC uint32_t hmac_sta_twt_save_parameters(hmac_vap_stru *pst_hmac_vap,
    mac_twt_element_ie_individual_stru *pst_twt_element)
{
    if (oal_any_null_ptr2(pst_hmac_vap, pst_twt_element)) {
        oam_error_log2(0, OAM_SF_11AX, "{hmac_sta_twt_save_parameters::null param, 0x%x 0x%x 0x%x.}",
                       (uintptr_t)pst_hmac_vap, (uintptr_t)pst_twt_element);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /*************************************************************************/
    /*                  TWT Element Format                                   */
    /* --------------------------------------------------------------------- */
    /* |Element ID | Length | Control | TWT parameter Information            */
    /* --------------------------------------------------------------------- */
    /* | 1         | 1      | 1      |      Variable             |           */
    /* --------------------------------------------------------------------- */
    /*************************************************************************/
    pst_hmac_vap->st_twt_cfg.ull_twt_start_time = pst_twt_element->ull_twt;
    pst_hmac_vap->st_twt_cfg.ul_twt_duration =
        ((uint32_t)pst_twt_element->uc_min_duration) << 8; /* Nominal Minimum TWT Wake Duration, 单位是256us */
    pst_hmac_vap->st_twt_cfg.ull_twt_interval =
        (uint64_t)pst_twt_element->us_mantissa << pst_twt_element->st_request_type.bit_exponent;
    pst_hmac_vap->st_twt_cfg.uc_twt_flow_id = pst_twt_element->st_request_type.bit_flow_id;
    pst_hmac_vap->st_twt_cfg.uc_twt_announce_bit = pst_twt_element->st_request_type.bit_flow_type;
    pst_hmac_vap->st_twt_cfg.uc_twt_is_trigger_enabled = pst_twt_element->st_request_type.bit_trigger;
    pst_hmac_vap->st_twt_cfg.uc_twt_session_enable = 1;

    return OAL_SUCC;
}


OAL_STATIC uint32_t hmac_sta_twt_config_accept_action(hmac_vap_stru *pst_hmac_vap,
    mac_twt_action_mgmt_args_stru *pst_twt_action_args, mac_twt_element_ie_individual_stru *pst_twt_element)
{
    if (oal_any_null_ptr3(pst_hmac_vap, pst_twt_action_args, pst_twt_element)) {
        oam_error_log3(0, OAM_SF_11AX, "{hmac_sta_twt_config_accept_action::null param, 0x%x 0x%x 0x%x.}",
                       (uintptr_t)pst_hmac_vap, (uintptr_t)pst_twt_action_args, (uintptr_t)pst_twt_element);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /*************************************************************************/
    /*                  TWT Element Format                                   */
    /* --------------------------------------------------------------------- */
    /* |Element ID | Length | Control | TWT parameter Information            */
    /* --------------------------------------------------------------------- */
    /* | 1         | 1      | 1      |      Variable             |           */
    /* --------------------------------------------------------------------- */
    /*************************************************************************/
    pst_twt_action_args->uc_category = MAC_ACTION_CATEGORY_S1G;
    pst_twt_action_args->uc_action = MAC_S1G_ACTION_TWT_SETUP;
    pst_twt_action_args->uc_twt_setup_cmd = MAC_TWT_COMMAND_ACCEPT;
    pst_twt_action_args->uc_twt_flow_type = pst_twt_element->st_request_type.bit_flow_type;
    pst_twt_action_args->uc_twt_flow_id = pst_twt_element->st_request_type.bit_flow_id;
    pst_twt_action_args->ul_twt_start_time_offset = (uint32_t)(pst_twt_element->ull_twt & 0xffffffff);
    pst_twt_action_args->ul_twt_exponent = pst_twt_element->st_request_type.bit_exponent;
    pst_twt_action_args->ul_twt_duration = pst_twt_element->uc_min_duration;
    pst_twt_action_args->ul_intrval_mantissa = pst_twt_element->us_mantissa;

    return OAL_SUCC;
}


OAL_STATIC uint32_t hmac_sta_twt_update_parameters_to_device(hmac_vap_stru *pst_hmac_vap,
    hmac_user_stru *pst_hmac_user)
{
    mac_vap_stru *pst_mac_vap;
    frw_event_mem_stru *pst_event_mem; /* 申请事件返回的内存指针 */
    frw_event_stru *pst_hmac_to_dmac_crx_sync;
    dmac_ctx_update_twt_stru *pst_update_twt_cfg_event;

    if (oal_any_null_ptr2(pst_hmac_vap, pst_hmac_user)) {
        oam_error_log2(0, OAM_SF_11AX, "{hmac_sta_twt_update_parameters_to_device:: null param, 0x%x 0x%x 0x%x.}",
                       (uintptr_t)pst_hmac_vap, (uintptr_t)pst_hmac_user);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_mac_vap = &(pst_hmac_vap->st_vap_base_info);

    /* 抛事件到DMAC处理 */
    pst_event_mem = frw_event_alloc_m(OAL_SIZEOF(dmac_ctx_update_twt_stru));
    if (pst_event_mem == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_11AX,
                       "{hmac_sta_twt_update_parameters_to_device::pst_event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 获得事件指针 */
    pst_hmac_to_dmac_crx_sync = frw_get_event_stru(pst_event_mem);

    /* 填写事件头 */
    frw_event_hdr_init(&(pst_hmac_to_dmac_crx_sync->st_event_hdr),
                       FRW_EVENT_TYPE_WLAN_CTX,
                       DMAC_WLAN_CTX_EVENT_SUB_TYPE_UPDATE_TWT,
                       OAL_SIZEOF(dmac_ctx_action_event_stru),
                       FRW_EVENT_PIPELINE_STAGE_1,
                       pst_mac_vap->uc_chip_id,
                       pst_mac_vap->uc_device_id,
                       pst_mac_vap->uc_vap_id);

    /* 获取帧体信息，由于DMAC的同步，填写事件payload */
    pst_update_twt_cfg_event = (dmac_ctx_update_twt_stru *)(pst_hmac_to_dmac_crx_sync->auc_event_data);
    pst_update_twt_cfg_event->en_action_category = MAC_ACTION_CATEGORY_S1G;
    pst_update_twt_cfg_event->uc_action = MAC_S1G_ACTION_TWT_SETUP;
    pst_update_twt_cfg_event->us_user_idx = pst_hmac_user->st_user_base_info.us_assoc_id;
    pst_update_twt_cfg_event->st_twt_cfg.ull_twt_start_time = pst_hmac_vap->st_twt_cfg.ull_twt_start_time;
    pst_update_twt_cfg_event->st_twt_cfg.ull_twt_interval = pst_hmac_vap->st_twt_cfg.ull_twt_interval;
    pst_update_twt_cfg_event->st_twt_cfg.ul_twt_duration = pst_hmac_vap->st_twt_cfg.ul_twt_duration;
    pst_update_twt_cfg_event->st_twt_cfg.uc_twt_flow_id = pst_hmac_vap->st_twt_cfg.uc_twt_flow_id;
    pst_update_twt_cfg_event->st_twt_cfg.uc_twt_announce_bit = pst_hmac_vap->st_twt_cfg.uc_twt_announce_bit;
    pst_update_twt_cfg_event->st_twt_cfg.uc_twt_is_trigger_enabled = pst_hmac_vap->st_twt_cfg.uc_twt_is_trigger_enabled;
    pst_update_twt_cfg_event->st_twt_cfg.uc_twt_session_enable = pst_hmac_vap->st_twt_cfg.uc_twt_session_enable;
    pst_update_twt_cfg_event->st_twt_cfg.uc_next_twt_size = 0; /* 只有twt information帧才有 */

    /* 分发 */
    frw_event_dispatch_event(pst_event_mem);

    /* 释放事件内存 */
    frw_event_free_m(pst_event_mem);
    return OAL_SUCC;
}


uint32_t hmac_sta_rx_twt_setup_frame(hmac_vap_stru *pst_hmac_vap,
    hmac_user_stru *pst_hmac_user, uint8_t *puc_payload)
{
    mac_twt_element_ie_individual_stru *pst_twt_element;
    mac_twt_command_enum_uint8 en_command;
    uint64_t ull_twt_interval;
    uint32_t ul_twt_wake_duration;
    mac_twt_action_mgmt_args_stru st_twt_action_args;

    if (g_st_mac_device_custom_cfg.st_11ax_custom_cfg.bit_twt_responder_support == OAL_FALSE) {
        oam_warning_log0(0, OAM_SF_11AX,
            "{hmac_sta_rx_twt_setup_frame::twt responder support is off in the ini file!.}");
        return OAL_FAIL;
    }

    if (oal_any_null_ptr3(pst_hmac_vap, pst_hmac_user, puc_payload)) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    /******************************************************************/
    /*       TWT setup Frame - Frame Body                         */
    /* --------------------------------------------------------------- */
    /* | Category | Action | Dialog | TWT element     |               */
    /* --------------------------------------------------------------- */
    /* | 1        | 1      | 1      |  Variable  |                    */
    /* --------------------------------------------------------------- */
    /*                                                                */
    /******************************************************************/
    /*************************************************************************/
    /*                  TWT Element Format              */
    /* --------------------------------------------------------------------- */
    /* |Element ID | Length | Control | TWT parameter Information            */
    /* --------------------------------------------------------------------- */
    /* | 1         | 1      | 1      |      Variable             |           */
    /* --------------------------------------------------------------------- */
    /*************************************************************************/
    pst_twt_element = (mac_twt_element_ie_individual_stru *)(puc_payload);

    oam_warning_log3(0, OAM_SF_11AX,
                     "{hmac_sta_rx_twt_setup_frame:: flow_id:%d, bit_flow_type:%d, bit_setup_command:%d}",
                     pst_twt_element->st_request_type.bit_flow_id,
                     pst_twt_element->st_request_type.bit_flow_type,
                     pst_twt_element->st_request_type.bit_setup_command);

    /* 非TWT element */
    if (pst_twt_element->uc_element_id != MAC_EID_TWT) {
        oam_warning_log1(0, OAM_SF_11AX,
                         "{hmac_sta_rx_twt_setup_frame:: It's not a twt element id:%d}",
                         pst_twt_element->uc_element_id);
        return OAL_FAIL;
    }

    en_command = (mac_twt_command_enum_uint8)pst_twt_element->st_request_type.bit_setup_command;

    /* 大于2的twt commad都是ap发送过来的，需要检查dialog token */
    if ((en_command > MAC_TWT_COMMAND_DEMAND) &&
        (pst_twt_element->uc_dialog_token != pst_hmac_vap->st_twt_cfg.uc_twt_dialog_token)) {
        oam_warning_log3(0, OAM_SF_11AX,
            "{hmac_sta_rx_twt_setup_frame:: twt dialog token is not same: rx %d, tx:%d, twt setup command:%d}",
            pst_twt_element->uc_dialog_token,
            pst_hmac_vap->st_twt_cfg.uc_twt_dialog_token,
            en_command);
        return OAL_FAIL;
    }

    /* 检查twt参数，非法参数不允许建立twt会话 */
    ull_twt_interval = (uint64_t)pst_twt_element->us_mantissa << pst_twt_element->st_request_type.bit_exponent;
    ul_twt_wake_duration = pst_twt_element->uc_min_duration * 256;
    if (ull_twt_interval < ul_twt_wake_duration) {
        oam_warning_log2(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_11AX,
                         "{hmac_sta_rx_twt_setup_frame::error twt interval [%d] < twt wake duration!.}",
                         ull_twt_interval, ul_twt_wake_duration);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    switch (en_command) {
        case MAC_TWT_COMMAND_ACCEPT:
            hmac_sta_twt_save_parameters(pst_hmac_vap, pst_twt_element);
            hmac_sta_twt_update_parameters_to_device(pst_hmac_vap, pst_hmac_user);
            break;

        case MAC_TWT_COMMAND_REQUEST:
        case MAC_TWT_COMMAND_SUGGEST:
        case MAC_TWT_COMMAND_DEMAND:
            hmac_sta_twt_save_parameters(pst_hmac_vap, pst_twt_element);
            hmac_sta_twt_config_accept_action(pst_hmac_vap, &st_twt_action_args, pst_twt_element);
            /* 在dmac发送完成中断收到ack后再设置twt参数 */
            hmac_mgmt_tx_twt_setup(pst_hmac_vap, pst_hmac_user, &st_twt_action_args);
            break;

        default:
            break;
    }

    return OAL_SUCC;
}


uint32_t hmac_sta_rx_twt_teardown_frame(hmac_vap_stru *pst_hmac_vap,
    hmac_user_stru *pst_hmac_user, uint8_t *puc_payload)
{
    mac_device_stru *pst_device;
    mac_vap_stru *pst_mac_vap;
    mac_twt_teardown_stru *pst_twt_teardown;
    frw_event_mem_stru *pst_event_mem; /* 申请事件返回的内存指针 */
    frw_event_stru *pst_hmac_to_dmac_crx_sync;
    dmac_ctx_update_twt_stru *pst_update_twt_cfg_event;

    if (oal_any_null_ptr3(pst_hmac_vap, pst_hmac_user, puc_payload)) {
        oam_error_log3(0, OAM_SF_11AX,
                       "{hmac_sta_rx_twt_teardown_frame::twt teardown receive failed, null param, 0x%x 0x%x 0x%x.}",
                       (uintptr_t)pst_hmac_vap, (uintptr_t)pst_hmac_user, (uintptr_t)puc_payload);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_mac_vap = &(pst_hmac_vap->st_vap_base_info);

    /* 获取device结构 */
    pst_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (pst_device == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_11AX,
                       "{hmac_sta_rx_twt_teardown_frame::twt teardown receive failed, pst_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /******************************************************************/
    /*       TWT teardown Frame - Frame Body                         */
    /* --------------------------------------------------------------- */
    /* | Category | Action | TWT Flow   |                             */
    /* --------------------------------------------------------------- */
    /* | 1        | 1      |    1       |                             */
    /* --------------------------------------------------------------- */
    /*                                                                */
    /******************************************************************/
    pst_twt_teardown = (mac_twt_teardown_stru *)(puc_payload);

    oam_warning_log2(pst_mac_vap->uc_vap_id, OAM_SF_11AX,
                     "{hmac_sta_rx_twt_teardown_frame:: flow_id:%d, bit_nego_type:%d}",
                     pst_twt_teardown->bit_twt_flow_id, pst_twt_teardown->bit_nego_type);

    /* 只支持删除单播TWT会话 */
    if (pst_twt_teardown->bit_nego_type != 0) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_11AX,
                         "{hmac_sta_rx_twt_teardown_frame:: not support twt negotiation type:%d}",
                         pst_twt_teardown->bit_nego_type);
        return OAL_FAIL;
    }

    /* 检查TWT是否在运行 */
    if (pst_hmac_vap->st_twt_cfg.uc_twt_session_enable == 0) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_11AX,
                         "{hmac_sta_rx_twt_teardown_frame:: twt session is already disabled!}");
        return OAL_FAIL;
    }

    /* 检查flow id */
    if (pst_hmac_vap->st_twt_cfg.uc_twt_flow_id != pst_twt_teardown->bit_twt_flow_id) {
        oam_warning_log2(pst_mac_vap->uc_vap_id, OAM_SF_11AX,
                         "{hmac_sta_rx_twt_teardown_frame:: the flow id [%d] is invalid,our flow id is [%d]}",
                         pst_twt_teardown->bit_twt_flow_id, pst_hmac_vap->st_twt_cfg.uc_twt_flow_id);
        return OAL_FAIL;
    }

    /*  To-Do, bit number不一样 */
    pst_hmac_vap->st_twt_cfg.ull_twt_start_time = 0;
    pst_hmac_vap->st_twt_cfg.ul_twt_duration = 0;
    pst_hmac_vap->st_twt_cfg.ull_twt_interval = 0;
    pst_hmac_vap->st_twt_cfg.uc_twt_flow_id = pst_twt_teardown->bit_twt_flow_id;
    pst_hmac_vap->st_twt_cfg.uc_twt_announce_bit = 0;
    pst_hmac_vap->st_twt_cfg.uc_twt_is_trigger_enabled = 0;
    pst_hmac_vap->st_twt_cfg.uc_twt_session_enable = 0;

    /* 抛事件到DMAC处理 */
    pst_event_mem = frw_event_alloc_m(OAL_SIZEOF(dmac_ctx_update_twt_stru));
    if (pst_event_mem == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_11AX,
                       "{hmac_sta_rx_twt_teardown_frame::pst_event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 获得事件指针 */
    pst_hmac_to_dmac_crx_sync = frw_get_event_stru(pst_event_mem);

    /* 填写事件头 */
    frw_event_hdr_init(&(pst_hmac_to_dmac_crx_sync->st_event_hdr),
                       FRW_EVENT_TYPE_WLAN_CTX,
                       DMAC_WLAN_CTX_EVENT_SUB_TYPE_UPDATE_TWT,
                       OAL_SIZEOF(dmac_ctx_action_event_stru),
                       FRW_EVENT_PIPELINE_STAGE_1,
                       pst_mac_vap->uc_chip_id,
                       pst_mac_vap->uc_device_id,
                       pst_mac_vap->uc_vap_id);

    /* 获取帧体信息，由于DMAC的同步，填写事件payload */
    pst_update_twt_cfg_event = (dmac_ctx_update_twt_stru *)(pst_hmac_to_dmac_crx_sync->auc_event_data);
    pst_update_twt_cfg_event->en_action_category = MAC_ACTION_CATEGORY_S1G;
    pst_update_twt_cfg_event->uc_action = MAC_S1G_ACTION_TWT_TEARDOWN;
    pst_update_twt_cfg_event->us_user_idx = pst_hmac_user->st_user_base_info.us_assoc_id;
    pst_update_twt_cfg_event->st_twt_cfg.ull_twt_start_time = pst_hmac_vap->st_twt_cfg.ull_twt_start_time;
    pst_update_twt_cfg_event->st_twt_cfg.ull_twt_interval = pst_hmac_vap->st_twt_cfg.ull_twt_interval;
    pst_update_twt_cfg_event->st_twt_cfg.ul_twt_duration = pst_hmac_vap->st_twt_cfg.ul_twt_duration;
    pst_update_twt_cfg_event->st_twt_cfg.uc_twt_flow_id = pst_hmac_vap->st_twt_cfg.uc_twt_flow_id;
    pst_update_twt_cfg_event->st_twt_cfg.uc_twt_announce_bit = pst_hmac_vap->st_twt_cfg.uc_twt_announce_bit;
    pst_update_twt_cfg_event->st_twt_cfg.uc_twt_is_trigger_enabled = pst_hmac_vap->st_twt_cfg.uc_twt_is_trigger_enabled;
    pst_update_twt_cfg_event->st_twt_cfg.uc_twt_session_enable = pst_hmac_vap->st_twt_cfg.uc_twt_session_enable;
    pst_update_twt_cfg_event->st_twt_cfg.uc_next_twt_size = 0; /* 只有twt information帧才有 */

    /* 分发 */
    frw_event_dispatch_event(pst_event_mem);

    /* 释放事件内存 */
    frw_event_free_m(pst_event_mem);
    return OAL_SUCC;
}


OAL_STATIC uint64_t hmac_get_next_twt(hmac_vap_stru *pst_hmac_vap, uint8_t *puc_payload,
    uint8_t uc_next_twt_subfield_size)
{
    uint32_t ul_next_twt_low;
    uint64_t ul_next_twt_high;
    uint64_t ull_next_twt;

    switch (uc_next_twt_subfield_size) {
        case HMAC_NEXT_TWT_SUBFIELD_SIZE_BITS_0:
            ull_next_twt = 0;
            break;

        case HMAC_NEXT_TWT_SUBFIELD_SIZE_BITS_32: /* 32 bits */
            ul_next_twt_low = puc_payload[0];
            ul_next_twt_low |= (puc_payload[1] << 8);
            ul_next_twt_low |= (puc_payload[2] << 16);
            ul_next_twt_low |= (puc_payload[3] << 24);
            ull_next_twt = ul_next_twt_low;
            break;

        case HMAC_NEXT_TWT_SUBFIELD_SIZE_BITS_48: /* 48 bits */
            ul_next_twt_low = puc_payload[0];
            ul_next_twt_low |= (puc_payload[1] << 8);
            ul_next_twt_low |= (puc_payload[2] << 16);
            ul_next_twt_low |= (puc_payload[3] << 24);
            ul_next_twt_high = puc_payload[4];
            ul_next_twt_high |= (puc_payload[5] << 8);
            ull_next_twt = ul_next_twt_low;
            ull_next_twt |= (ul_next_twt_high << 32);
            break;

        case HMAC_NEXT_TWT_SUBFIELD_SIZE_BITS_64: /* 64 bits */
            ul_next_twt_low = puc_payload[0];
            ul_next_twt_low |= (puc_payload[1] << 8);
            ul_next_twt_low |= (puc_payload[2] << 16);
            ul_next_twt_low |= (puc_payload[3] << 24);
            ul_next_twt_high = puc_payload[4];
            ul_next_twt_high |= (puc_payload[5] << 8);
            ul_next_twt_high |= (puc_payload[6] << 16);
            ul_next_twt_high |= ((uint32_t)puc_payload[7] << 24);
            ull_next_twt = ul_next_twt_low;
            ull_next_twt |= (ul_next_twt_high << 32);
            break;

        default:
            ull_next_twt = 0;
            oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_11AX,
                             "{hmac_get_next_twt:: invalid next twt size [%d]!}",
                             uc_next_twt_subfield_size);
            break;
    }

    return ull_next_twt;
}


uint32_t hmac_sta_rx_twt_information_frame(hmac_vap_stru *pst_hmac_vap,
                                           hmac_user_stru *pst_hmac_user,
                                           uint8_t *puc_payload)
{
    mac_device_stru *pst_device;
    mac_vap_stru *pst_mac_vap;
    mac_twt_information_frame_stru *pst_twt_information_frame_body;

    frw_event_mem_stru *pst_event_mem; /* 申请事件返回的内存指针 */
    frw_event_stru *pst_hmac_to_dmac_crx_sync;
    dmac_ctx_update_twt_stru *pst_update_twt_cfg_event;

    uint64_t ull_next_twt;
    uint8_t uc_next_twt_subfield_size;

    if (oal_any_null_ptr3(pst_hmac_vap, pst_hmac_user, puc_payload)) {
        oam_error_log3(0, OAM_SF_11AX,
                       "{hmac_sta_rx_twt_information_frame::null ptr:0x%x 0x%x 0x%x.}",
                       (uintptr_t)pst_hmac_vap, (uintptr_t)pst_hmac_user, (uintptr_t)puc_payload);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_mac_vap = &(pst_hmac_vap->st_vap_base_info);

    /* 获取device结构 */
    pst_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (pst_device == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_11AX,
                       "{hmac_sta_rx_twt_information_frame::twt information receive failed, pst_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /******************************************************************/
    /*       TWT information Frame - Frame Body                       */
    /* --------------------------------------------------------------- */
    /* | Category | Action | TWT element | Next TWT                   */
    /* --------------------------------------------------------------- */
    /* | 1        | 1      | 1           |  Variable                   */
    /* --------------------------------------------------------------- */
    /*                                                                */
    /******************************************************************/
    pst_twt_information_frame_body = (mac_twt_information_frame_stru *)(puc_payload);

    oam_warning_log2(0, OAM_SF_11AX, "{hmac_sta_rx_twt_information_frame:: flow id:%d, next twt subfield size:%d}",
                     pst_twt_information_frame_body->st_twt_information_filed.bit_twt_flow_id,
                     pst_twt_information_frame_body->st_twt_information_filed.bit_next_twt_subfield_size);

    /* 检查TWT是否在运行 */
    if (pst_hmac_vap->st_twt_cfg.uc_twt_session_enable == 0) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_11AX,
                         "{hmac_sta_rx_twt_information_frame:: twt session is disabled!}");
        return OAL_FAIL;
    }

    /* 检查flow id */
    if (pst_hmac_vap->st_twt_cfg.uc_twt_flow_id !=
        pst_twt_information_frame_body->st_twt_information_filed.bit_twt_flow_id) {
        oam_warning_log2(pst_mac_vap->uc_vap_id, OAM_SF_11AX,
                         "{hmac_sta_rx_twt_teardown_frame:: the flow id [%d] is invalid,our flow id is [%d]}",
                         pst_twt_information_frame_body->st_twt_information_filed.bit_twt_flow_id,
                         pst_hmac_vap->st_twt_cfg.uc_twt_flow_id);
        return OAL_FAIL;
    }

    uc_next_twt_subfield_size = pst_twt_information_frame_body->st_twt_information_filed.bit_next_twt_subfield_size;
    puc_payload += OAL_SIZEOF(mac_twt_information_frame_stru);

    ull_next_twt = hmac_get_next_twt(pst_hmac_vap, puc_payload, uc_next_twt_subfield_size);

    /*  这里只更新twt start time和next twt size */
    pst_hmac_vap->st_twt_cfg.ull_twt_start_time = ull_next_twt;
    pst_hmac_vap->st_twt_cfg.uc_next_twt_size = uc_next_twt_subfield_size;
    oam_warning_log2(pst_mac_vap->uc_vap_id, OAM_SF_11AX,
                     "{hmac_sta_rx_twt_information_frame:: ull_next_twt=[%ld], uc_next_twt_subfield_size=[%d]}",
                     ull_next_twt, uc_next_twt_subfield_size);

    /* 抛事件到DMAC处理 */
    pst_event_mem = frw_event_alloc_m(OAL_SIZEOF(dmac_ctx_update_twt_stru));
    if (pst_event_mem == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_11AX,
                       "{hmac_sta_rx_twt_information_frame::pst_event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 获得事件指针 */
    pst_hmac_to_dmac_crx_sync = frw_get_event_stru(pst_event_mem);

    /* 填写事件头 */
    frw_event_hdr_init(&(pst_hmac_to_dmac_crx_sync->st_event_hdr),
                       FRW_EVENT_TYPE_WLAN_CTX,
                       DMAC_WLAN_CTX_EVENT_SUB_TYPE_UPDATE_TWT,
                       OAL_SIZEOF(dmac_ctx_action_event_stru),
                       FRW_EVENT_PIPELINE_STAGE_1,
                       pst_mac_vap->uc_chip_id,
                       pst_mac_vap->uc_device_id,
                       pst_mac_vap->uc_vap_id);

    /* 获取帧体信息，由于DMAC的同步，填写事件payload */
    pst_update_twt_cfg_event = (dmac_ctx_update_twt_stru *)(pst_hmac_to_dmac_crx_sync->auc_event_data);
    pst_update_twt_cfg_event->en_action_category = MAC_ACTION_CATEGORY_S1G;
    pst_update_twt_cfg_event->uc_action = MAC_S1G_ACTION_TWT_INFORMATION;
    pst_update_twt_cfg_event->us_user_idx = pst_hmac_user->st_user_base_info.us_assoc_id;
    pst_update_twt_cfg_event->st_twt_cfg.ull_twt_start_time = pst_hmac_vap->st_twt_cfg.ull_twt_start_time;
    pst_update_twt_cfg_event->st_twt_cfg.ull_twt_interval = pst_hmac_vap->st_twt_cfg.ull_twt_interval;
    pst_update_twt_cfg_event->st_twt_cfg.ul_twt_duration = pst_hmac_vap->st_twt_cfg.ul_twt_duration;
    pst_update_twt_cfg_event->st_twt_cfg.uc_twt_flow_id = pst_hmac_vap->st_twt_cfg.uc_twt_flow_id;
    pst_update_twt_cfg_event->st_twt_cfg.uc_twt_announce_bit = pst_hmac_vap->st_twt_cfg.uc_twt_announce_bit;
    pst_update_twt_cfg_event->st_twt_cfg.uc_twt_is_trigger_enabled = pst_hmac_vap->st_twt_cfg.uc_twt_is_trigger_enabled;
    pst_update_twt_cfg_event->st_twt_cfg.uc_twt_session_enable = pst_hmac_vap->st_twt_cfg.uc_twt_session_enable;
    pst_update_twt_cfg_event->st_twt_cfg.uc_next_twt_size = pst_hmac_vap->st_twt_cfg.uc_next_twt_size;

    /* 分发 */
    frw_event_dispatch_event(pst_event_mem);

    /* 释放事件内存 */
    frw_event_free_m(pst_event_mem);
    return OAL_SUCC;
}


uint32_t hmac_dbac_teardown_twt_session(mac_device_stru *pst_dev)
{
    mac_chip_stru *pst_chip = OAL_PTR_NULL;
    mac_vap_stru *pst_mac_vap_temp;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    uint8_t uc_vap_num;
    uint32_t ul_ret;

    pst_chip = hmac_res_get_mac_chip(pst_dev->uc_chip_id);
    if (pst_chip == OAL_PTR_NULL) {
        oam_error_log1(0, OAM_SF_ANY,
            "hmac_dbac_teardown_twt_session::hmac_res_get_mac_chip id[%d] NULL",
            pst_dev->uc_chip_id);
        return OAL_FAIL;
    }

    uc_vap_num = hmac_get_chip_vap_num(pst_chip);

    oam_warning_log1(0, OAM_SF_ANY, "{hmac_dbac_teardown_twt_session::check vap num for twt,vap num=[%d]}", uc_vap_num);

    /* 非DBDC场景，如果启动了3个及以上的vap，说明是wlan0/p2p共存，
       此时如果已经建立twt会话，则需要删除twt会话 */
    if ((pst_dev->en_dbdc_running == OAL_FALSE) && (uc_vap_num >= 3)) {
        /* 先找到wlan vap */
        ul_ret = mac_device_find_up_sta_wlan(pst_dev, &pst_mac_vap_temp);
        if ((ul_ret == OAL_SUCC) && (pst_mac_vap_temp != OAL_PTR_NULL)) {
            pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap_temp->uc_vap_id);
            if (pst_hmac_vap == OAL_PTR_NULL) {
                oam_error_log0(0, OAM_SF_CFG, "{hmac_dbac_teardown_twt_session::mac_res_get_hmac_vap failed.}");
                return OAL_FAIL;
            }

            oam_warning_log1(0, OAM_SF_ANY,
                "{hmac_dbac_teardown_twt_session::twt_session_enable=[%d]}",
                pst_hmac_vap->st_twt_cfg.uc_twt_session_enable);

            /* 如果该vap的twt会话已经开启，则删除twt会话 */
            if (pst_hmac_vap->st_twt_cfg.uc_twt_session_enable == 1) {
                hmac_config_twt_teardown_req_auto(&(pst_hmac_vap->st_vap_base_info));
            }
        }
    }

    return OAL_SUCC;
}



uint32_t hmac_twt_auto_setup_session(mac_device_stru *pst_device)
{
    mac_chip_stru *pst_chip = OAL_PTR_NULL;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    hmac_user_stru *pst_hmac_user = OAL_PTR_NULL;
    mac_vap_stru *pst_mac_vap_temp;
    uint8_t uc_vap_num;
    uint32_t ul_ret;
    mac_he_hdl_stru st_he_hdl;

    pst_chip = hmac_res_get_mac_chip(pst_device->uc_chip_id);
    if (pst_chip == OAL_PTR_NULL) {
        oam_error_log1(0, OAM_SF_ANY,
            "hmac_twt_auto_setup_session::hmac_res_get_mac_chip id[%d] NULL",
            pst_device->uc_chip_id);
        return OAL_FAIL;
    }

    uc_vap_num = hmac_get_chip_vap_num(pst_chip);

    oam_warning_log1(0, OAM_SF_ANY, "{hmac_twt_auto_setup_session::check vap num for twt,vap num=[%d]}", uc_vap_num);

    /* 只有1个vap的时候，如果ap支持twt，sta没有建立会话，则建立twt会话 */
    if ((pst_device->en_dbdc_running == OAL_FALSE) && (uc_vap_num == 2)) {
        ul_ret = mac_device_find_up_sta_wlan(pst_device, &pst_mac_vap_temp);
        if ((ul_ret == OAL_SUCC) && (pst_mac_vap_temp != OAL_PTR_NULL)) {
            /* 重新建立twt会话 */
            pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap_temp->uc_vap_id);
            if (pst_hmac_vap == OAL_PTR_NULL) {
                oam_error_log0(0, OAM_SF_CFG, "{hmac_twt_auto_setup_session::mac_res_get_hmac_vap failed.}");
                return OAL_FAIL;
            }

            /* 获用户 */
            pst_hmac_user = mac_res_get_hmac_user(pst_hmac_vap->st_vap_base_info.us_assoc_vap_id);
            if (pst_hmac_user == OAL_PTR_NULL) {
                /* 和ap侧一样，上层已经删除了的话，属于正常 */
                oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG,
                                 "{hmac_twt_auto_setup_session::pst_hmac_user[%d] is null.}",
                                 pst_hmac_vap->st_vap_base_info.us_assoc_vap_id);
                return OAL_ERR_CODE_PTR_NULL;
            }

            /* 判断该AP是否支持twt，如果是fast ps模式，先检查ap是否支持twt，
             如果支持twt，建立twt会话，然后直接返回，否则进入fast ps模式 */
            mac_user_get_he_hdl(&(pst_hmac_user->st_user_base_info), &st_he_hdl);
            if (st_he_hdl.st_he_cap_ie.st_he_mac_cap.bit_twt_responder_support == OAL_TRUE) {
                if (pst_hmac_vap->st_twt_cfg.uc_twt_session_enable == 0) {
                    hmac_config_twt_setup_req_auto(&(pst_hmac_vap->st_vap_base_info));
                }
            }
        }
    }

    return OAL_SUCC;
}

uint32_t _hmac_twt_auto_setup_session(mac_device_stru *pst_device)
{
    if (g_wlan_spec_cfg->feature_twt_is_open) {
        return hmac_twt_auto_setup_session(pst_device);
    }

    return OAL_SUCC;
}

OAL_STATIC void hmac_config_twt_action_args_init(mac_twt_action_mgmt_args_stru *pst_twt_action_args,
    mac_cfg_twt_setup_req_param_stru *pst_twt_setup_req)
{
    pst_twt_action_args->uc_category = MAC_ACTION_CATEGORY_S1G;
    pst_twt_action_args->uc_action = MAC_S1G_ACTION_TWT_SETUP;
    pst_twt_action_args->uc_twt_setup_cmd = pst_twt_setup_req->uc_twt_setup_cmd;
    pst_twt_action_args->uc_twt_flow_type = pst_twt_setup_req->uc_twt_flow_type;
    pst_twt_action_args->uc_twt_flow_id = pst_twt_setup_req->uc_twt_flow_id;
    pst_twt_action_args->ul_twt_start_time_offset = pst_twt_setup_req->ul_twt_start_time_offset;
    pst_twt_action_args->ul_twt_exponent = pst_twt_setup_req->ul_twt_exponent;
    pst_twt_action_args->ul_twt_duration = pst_twt_setup_req->ul_twt_duration;
    pst_twt_action_args->ul_intrval_mantissa = pst_twt_setup_req->ul_intrval_mantissa;
    return;
}



uint32_t hmac_config_twt_setup_req(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    mac_cfg_twt_setup_req_param_stru *pst_twt_setup_req;
    hmac_user_stru *pst_hmac_user;
    hmac_vap_stru *pst_hmac_vap;
    mac_twt_action_mgmt_args_stru st_twt_action_args; /* 用于填写ACTION帧的参数 */
    mac_he_hdl_stru st_he_hdl;
    uint64_t ull_twt_interval;
    uint32_t ul_twt_wake_duration;
    mac_device_stru *pst_dev = OAL_PTR_NULL;
    mac_chip_stru *pst_chip = OAL_PTR_NULL;
    uint8_t uc_vap_num;

    if (g_st_mac_device_custom_cfg.st_11ax_custom_cfg.bit_twt_requester_support == OAL_FALSE) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_11AX,
            "{hmac_config_twt_setup_req::twt requester support is off in the ini file!.}");
        return OAL_FAIL;
    }

    pst_twt_setup_req = (mac_cfg_twt_setup_req_param_stru *)puc_param;

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 如果是已经启动多vap的情况下，不能建立TWT会话 */
    pst_dev = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (oal_unlikely(pst_dev == OAL_PTR_NULL)) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_chip = hmac_res_get_mac_chip(pst_dev->uc_chip_id);
    if (pst_chip == OAL_PTR_NULL) {
        return OAL_FAIL;
    }
    uc_vap_num = hmac_get_chip_vap_num(pst_chip);
    /* 非DBDC场景，如果启动了3个及以上的vap，说明是wlan0/p2p共存，
    此时如果已经建立twt会话，则需要删除twt会话 */
    if ((pst_dev->en_dbdc_running == OAL_FALSE) && (uc_vap_num >= HMAC_TWT_P2P_RUNNING_VAP_NUM)) {
        return OAL_FAIL;
    }

    /* 获取用户对应的索引 */
    pst_hmac_user = mac_vap_get_hmac_user_by_addr(pst_mac_vap, pst_twt_setup_req->auc_mac_addr);
    if (pst_hmac_user == OAL_PTR_NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 判断该AP是否支持twt */
    mac_user_get_he_hdl(&(pst_hmac_user->st_user_base_info), &st_he_hdl);
    if (st_he_hdl.st_he_cap_ie.st_he_mac_cap.bit_twt_responder_support == OAL_FALSE) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_11AX,
            "{hmac_config_twt_setup_req::the ap doesn't support twt responder!.}");
        return OAL_ERR_CODE_INVALID_CONFIG;
    }
    /* TWT Wake Interval Exponent: 5 bits， 取值范围[0,31] */
    if (pst_twt_setup_req->ul_twt_exponent > 31) {
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    /* Nominal Minimum TWT Wake Duration: 1byte, 取值范围[0, 255] */
    if (pst_twt_setup_req->ul_twt_duration > 255) {
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    ull_twt_interval = (uint64_t)pst_twt_setup_req->ul_intrval_mantissa << pst_twt_setup_req->ul_twt_exponent;
    ul_twt_wake_duration = pst_twt_setup_req->ul_twt_duration * 256;
    if (ull_twt_interval < ul_twt_wake_duration) {
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    hmac_config_twt_action_args_init(&st_twt_action_args, pst_twt_setup_req);
    hmac_mgmt_tx_twt_action(pst_hmac_vap, pst_hmac_user, &st_twt_action_args);

    return OAL_SUCC;
}



uint32_t hmac_config_twt_setup_req_auto(mac_vap_stru *pst_mac_vap)
{
    hmac_user_stru *pst_hmac_user;
    hmac_vap_stru *pst_hmac_vap;
    mac_twt_action_mgmt_args_stru st_twt_action_args; /* 用于填写ACTION帧的参数 */
    mac_he_hdl_stru st_he_hdl;

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_11AX, "{hmac_config_twt_setup_req_auto::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 获取用户对应的索引 */
    pst_hmac_user = mac_res_get_hmac_user(pst_hmac_vap->st_vap_base_info.us_assoc_vap_id);
    if (pst_hmac_user == OAL_PTR_NULL) {
        /* 和ap侧一样，上层已经删除了的话，属于正常 */
        oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_config_twt_setup_req_auto::pst_hmac_user[%d] is null.}",
                         pst_hmac_vap->st_vap_base_info.us_assoc_vap_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 判断该AP是否支持twt */
    mac_user_get_he_hdl(&(pst_hmac_user->st_user_base_info), &st_he_hdl);
    if (st_he_hdl.st_he_cap_ie.st_he_mac_cap.bit_twt_responder_support == OAL_FALSE) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_11AX,
                         "{hmac_config_twt_setup_req_auto::the ap doesn't support twt responder!.}");
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    st_twt_action_args.uc_category = MAC_ACTION_CATEGORY_S1G;
    st_twt_action_args.uc_action = MAC_S1G_ACTION_TWT_SETUP;
    st_twt_action_args.uc_twt_setup_cmd = 1;
    st_twt_action_args.uc_twt_flow_type = 0;
    st_twt_action_args.uc_twt_flow_id = 0;
    st_twt_action_args.ul_twt_start_time_offset = 0;
    st_twt_action_args.ul_twt_exponent = 10;
    st_twt_action_args.ul_twt_duration = 255;
    st_twt_action_args.ul_intrval_mantissa = 512;

    oam_warning_log4(pst_mac_vap->uc_vap_id, OAM_SF_11AX, "{hmac_config_twt_setup_req_auto:: %d, %d, %d, %d}",
                     st_twt_action_args.uc_twt_setup_cmd, st_twt_action_args.uc_twt_flow_type,
                     st_twt_action_args.uc_twt_flow_id, st_twt_action_args.ul_twt_start_time_offset);
    oam_warning_log3(pst_mac_vap->uc_vap_id, OAM_SF_11AX, "{hmac_config_twt_setup_req_auto:: %d, %d, %d}",
                     st_twt_action_args.ul_twt_exponent, st_twt_action_args.ul_twt_duration,
                     st_twt_action_args.ul_intrval_mantissa);

    hmac_mgmt_tx_twt_action(pst_hmac_vap, pst_hmac_user, &st_twt_action_args);

    return OAL_SUCC;
}


uint32_t hmac_config_twt_teardown_req(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    mac_cfg_twt_teardown_req_param_stru *pst_twt_teardown_req;
    hmac_user_stru *pst_hmac_user;
    hmac_vap_stru *pst_hmac_vap;
    mac_twt_action_mgmt_args_stru st_twt_action_args; /* 用于填写ACTION帧的参数 */

    pst_twt_teardown_req = (mac_cfg_twt_teardown_req_param_stru *)puc_param;
    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_11AX, "{hmac_config_twt_teardown_req::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    /* 获取用户对应的索引 */
    pst_hmac_user = mac_vap_get_hmac_user_by_addr(pst_mac_vap, pst_twt_teardown_req->auc_mac_addr);
    if (pst_hmac_user == OAL_PTR_NULL) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_11AX, "{hmac_config_twt_teardown_req::pst_hmac_user null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (pst_hmac_vap->st_twt_cfg.uc_twt_session_enable == 0) {
        oam_warning_log4(pst_mac_vap->uc_vap_id, OAM_SF_11AX,
            "{hmac_config_twt_teardown_req::the twt session of the ap[*:*:%x:%x:%x:%x] hasn't seted up yet!.}",
            pst_twt_teardown_req->auc_mac_addr[2],
            pst_twt_teardown_req->auc_mac_addr[3],
            pst_twt_teardown_req->auc_mac_addr[4],
            pst_twt_teardown_req->auc_mac_addr[5]);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    if (pst_twt_teardown_req->uc_twt_flow_id != pst_hmac_vap->st_twt_cfg.uc_twt_flow_id) {
        oam_warning_log2(pst_mac_vap->uc_vap_id, OAM_SF_11AX,
            "{hmac_config_twt_teardown_req:: twt flow id isn't same:seted up id[%d],request teardown id[%d]!.}",
            pst_hmac_vap->st_twt_cfg.uc_twt_flow_id, pst_twt_teardown_req->uc_twt_flow_id);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }

    st_twt_action_args.uc_category = MAC_ACTION_CATEGORY_S1G;
    st_twt_action_args.uc_action = MAC_S1G_ACTION_TWT_TEARDOWN;
    st_twt_action_args.uc_twt_flow_id = pst_twt_teardown_req->uc_twt_flow_id;

    hmac_mgmt_tx_twt_action(pst_hmac_vap, pst_hmac_user, &st_twt_action_args);

    return OAL_SUCC;
}



uint32_t hmac_config_twt_teardown_req_auto(mac_vap_stru *pst_mac_vap)
{
    hmac_user_stru *pst_hmac_user;
    hmac_vap_stru *pst_hmac_vap;
    mac_twt_action_mgmt_args_stru st_twt_action_args; /* 用于填写ACTION帧的参数 */

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (oal_unlikely(pst_hmac_vap == OAL_PTR_NULL)) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_11AX, "{hmac_config_twt_teardown_req::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 获取用户对应的索引 */
    pst_hmac_user = mac_res_get_hmac_user(pst_hmac_vap->st_vap_base_info.us_assoc_vap_id);
    if (oal_unlikely(pst_hmac_user == OAL_PTR_NULL)) {
        /* 和ap侧一样，上层已经删除了的话，属于正常 */
        oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_config_twt_setup_req_auto::pst_hmac_user[%d] is null.}",
                         pst_hmac_vap->st_vap_base_info.us_assoc_vap_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

    st_twt_action_args.uc_category = MAC_ACTION_CATEGORY_S1G;
    st_twt_action_args.uc_action = MAC_S1G_ACTION_TWT_TEARDOWN;
    st_twt_action_args.uc_twt_flow_id = pst_hmac_vap->st_twt_cfg.uc_twt_flow_id;

    hmac_mgmt_tx_twt_action(pst_hmac_vap, pst_hmac_user, &st_twt_action_args);

    return OAL_SUCC;
}


uint32_t hmac_proc_rx_process_twt_sync_event_tx_adapt(frw_event_mem_stru *pst_event_mem)
{
    return hmac_hcc_tx_event_payload_to_netbuf(pst_event_mem, OAL_SIZEOF(dmac_ctx_update_twt_stru));
}


uint32_t hmac_mgmt_tx_twt_action(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
                                 mac_twt_action_mgmt_args_stru *twt_action_args)
{
    if (oal_any_null_ptr3(hmac_vap, hmac_user, twt_action_args)) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (twt_action_args->uc_category == (uint8_t)MAC_ACTION_CATEGORY_S1G) {
        switch (twt_action_args->uc_action) {
            case MAC_S1G_ACTION_TWT_SETUP:
                oam_info_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_11AX,
                              "{hmac_mgmt_tx_twt_action::MAC_TWT_SETUP.}");
                hmac_mgmt_tx_twt_setup(hmac_vap, hmac_user, twt_action_args);
                break;

            case MAC_S1G_ACTION_TWT_TEARDOWN:
                oam_info_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_11AX,
                              "{hmac_mgmt_tx_twt_action::MAC_TWT_TEARDOWN.}");
                hmac_mgmt_tx_twt_teardown(hmac_vap, hmac_user, twt_action_args);
                break;

            default:
                return OAL_FAIL; /* 错误类型待修改 */
        }
    } else {
        oam_info_log1(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_11AX,
                      "{hmac_mgmt_tx_twt_action::invalid action type[%d].}", twt_action_args->uc_category);
    }
    return OAL_SUCC;
}


uint32_t mac_device_find_up_sta_wlan(mac_device_stru *pst_mac_device, mac_vap_stru **ppst_mac_vap)
{
    uint8_t uc_vap_idx;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;

    for (uc_vap_idx = 0; uc_vap_idx < pst_mac_device->uc_vap_num; uc_vap_idx++) {
        pst_mac_vap = mac_res_get_mac_vap(pst_mac_device->auc_vap_id[uc_vap_idx]);
        if (oal_unlikely(pst_mac_vap == OAL_PTR_NULL)) {
            oam_warning_log1(0, OAM_SF_SCAN,
                             "mac_device_find_up_sta_wlan::vap is null! vap id is %d",
                             pst_mac_device->auc_vap_id[uc_vap_idx]);
            continue;
        }

        if ((pst_mac_vap->en_vap_state == MAC_VAP_STATE_UP || pst_mac_vap->en_vap_state == MAC_VAP_STATE_PAUSE) &&
            (pst_mac_vap->en_p2p_mode == WLAN_LEGACY_VAP_MODE)) {
            *ppst_mac_vap = pst_mac_vap;

            return OAL_SUCC;
        }
    }

    *ppst_mac_vap = OAL_PTR_NULL;

    return OAL_FAIL;
}

#endif /* _PRE_WLAN_FEATURE_TWT */