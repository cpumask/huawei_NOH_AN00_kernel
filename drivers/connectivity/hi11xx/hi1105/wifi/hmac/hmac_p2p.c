


/* 1 头文件包含 */
#include "hmac_p2p.h"

#include "oal_util.h"
#include "hmac_ext_if.h"
#include "hmac_resource.h"
#include "hmac_device.h"
#include "hmac_vap.h"
#include "hmac_rx_filter.h"
#include "hmac_fsm.h"
#include "hmac_mgmt_bss_comm.h"
#include "mac_frame.h"
#include "hmac_user.h"
#include "hmac_mgmt_ap.h"
#include "securec.h"
#include "securectype.h"
#include "hmac_chan_mgmt.h"
#ifdef _PRE_WLAN_FEATURE_DFS
#include "hmac_dfs.h"
#endif

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_P2P_C
#define HMAC_P2P_GO_CSA_COUNT 5
#define HMAC_P2P_GO_VENDOR_CMD_CSA_COUNT 2

typedef struct mac_vap_state_priority {
    uint8_t uc_priority;
} mac_vap_state_priority_stru;

typedef struct hmac_input_req_priority {
    uint8_t uc_priority;
} hmac_input_req_priority_stru;

mac_vap_state_priority_stru g_mac_vap_state_priority_table[MAC_VAP_STATE_BUTT] = {
    { 0 }, /* {MAC_VAP_STATE_INIT */
    { 2 }, /* {MAC_VAP_STATE_UP */
    { 0 }, /* {MAC_VAP_STATE_PAUSE */
    /* ap 独有状态 */
    { 0 }, /* {MAC_VAP_STATE_AP_PAUSE */
    /* sta独有状态 */
    { 0 },  /* {MAC_VAP_STATE_STA_FAKE_UP */
    { 10 }, /* {MAC_VAP_STATE_STA_WAIT_SCAN */
    { 0 },  /* {MAC_VAP_STATE_STA_SCAN_COMP */
    { 10 }, /* {MAC_VAP_STATE_STA_JOIN_COMP */
    { 10 }, /* {MAC_VAP_STATE_STA_WAIT_AUTH_SEQ2 */
    { 10 }, /* {MAC_VAP_STATE_STA_WAIT_AUTH_SEQ4 */
    { 10 }, /* {MAC_VAP_STATE_STA_AUTH_COMP */
    { 10 }, /* {MAC_VAP_STATE_STA_WAIT_ASOC */
    { 10 }, /* {MAC_VAP_STATE_STA_OBSS_SCAN */
    { 10 }, /* {MAC_VAP_STATE_STA_BG_SCAN */
    { 0 },  /* MAC_VAP_STATE_STA_LISTEN */
    { 10 }  /* MAC_VAP_STATE_ROAMING */
};

hmac_input_req_priority_stru g_mac_fsm_input_type_priority_table[HMAC_FSM_INPUT_TYPE_BUTT] = {
    { 0 }, /* HMAC_FSM_INPUT_RX_MGMT */
    { 0 }, /* HMAC_FSM_INPUT_RX_DATA */
    { 0 }, /* HMAC_FSM_INPUT_TX_DATA */
    { 0 }, /* HMAC_FSM_INPUT_TIMER0_OUT */
    { 0 }, /* HMAC_FSM_INPUT_MISC */

    { 0 }, /* HMAC_FSM_INPUT_START_REQ */

    { 5 }, /* HMAC_FSM_INPUT_SCAN_REQ */
    { 5 }, /* HMAC_FSM_INPUT_JOIN_REQ */
    { 5 }, /* HMAC_FSM_INPUT_AUTH_REQ */
    { 5 }, /* HMAC_FSM_INPUT_ASOC_REQ */

    { 5 }, /* HMAC_FSM_INPUT_LISTEN_REQ */
    { 0 }  /* HMAC_FSM_INPUT_LISTEN_TIMEOUT */
};


uint32_t hmac_p2p_check_can_enter_state(mac_vap_stru *mac_vap, hmac_fsm_input_type_enum_uint8 input_req)
{
    mac_device_stru *mac_device = NULL;
    mac_vap_stru *other_vap = NULL;
    uint8_t vap_num;
    uint8_t vap_idx;

    /*
     * 2.1 检查其他vap 状态，判断输入事件优先级是否比vap 状态优先级高
     * 如果输入事件优先级高，则可以执行输入事件
     */
    mac_device = mac_res_get_dev(mac_vap->uc_device_id);
    if (mac_device == NULL) {
        oam_warning_log1(0, OAM_SF_CFG, "{hmac_p2p_check_can_enter_state::mac_res_get_dev fail.device_id = %u}",
                         mac_vap->uc_device_id);
        return OAL_ERR_CODE_PTR_NULL;
    }
    vap_num = mac_device->uc_vap_num;

    for (vap_idx = 0; vap_idx < vap_num; vap_idx++) {
        other_vap = mac_res_get_mac_vap(mac_device->auc_vap_id[vap_idx]);
        if (other_vap == NULL) {
            oam_warning_log1(0, OAM_SF_CFG, "{hmac_p2p_check_can_enter_state::get_mac_vap fail.vap_idx = %u}",
                             mac_device->auc_vap_id[vap_idx]);
            continue;
        }

        if (other_vap->uc_vap_id == mac_vap->uc_vap_id) {
            /* 如果检测到是自己，则继续检查其他VAP 状态 */
            continue;
        }

        if ((g_mac_vap_state_priority_table[other_vap->en_vap_state].uc_priority >
             g_mac_fsm_input_type_priority_table[input_req].uc_priority) ||
            hmac_go_is_auth(other_vap)) {
            return OAL_ERR_CODE_CONFIG_BUSY;
        }
    }
    return OAL_SUCC;
}

#define WLAN_MAX_SERVICE_P2P_DEV_NUM      1
#define WLAN_MAX_SERVICE_P2P_GOCLIENT_NUM 1


uint32_t hmac_check_p2p_vap_num(mac_device_stru *mac_device, wlan_p2p_mode_enum_uint8 p2p_mode)
{
    if (mac_device == NULL) {
        oam_warning_log0(0, OAM_SF_CFG, "{hmac_check_p2p_vap_num::mac_device is NULL}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (p2p_mode == WLAN_P2P_DEV_MODE) {
        /* 判断已创建的P2P_DEVICE 个数是否达到最大值,P2P0只能创建1个、且不能与AP共存 */
        if (mac_device->st_p2p_info.uc_p2p_device_num >= WLAN_MAX_SERVICE_P2P_DEV_NUM) {
            /* 已创建的P2P_DEV个数达到最大值 */
            oam_warning_log1(0, OAM_SF_CFG, "{hmac_config_check_vap_num::only can created 1 p2p_device[%d].}",
                mac_device->st_p2p_info.uc_p2p_device_num);
            return OAL_ERR_CODE_CONFIG_EXCEED_SPEC;
        }

        /* 剩余资源是否足够申请一份P2P_DEV */
        if (mac_device->uc_sta_num >= WLAN_SERVICE_STA_MAX_NUM_PER_DEVICE) {
            /* 已有AP创建 */
            oam_warning_log1(0, OAM_SF_CFG, "{hmac_check_p2p_vap_num::can not create p2p_device any more[%d].}",
                             mac_device->uc_sta_num);
            return OAL_ERR_CODE_CONFIG_EXCEED_SPEC;
        }
    } else if (p2p_mode == WLAN_P2P_CL_MODE) {
        /* 判断已创建的P2P_CLIENT 个数是否达到最大值 */
        if (mac_device->st_p2p_info.uc_p2p_goclient_num >= WLAN_MAX_SERVICE_P2P_GOCLIENT_NUM) {
            /* 已创建的P2P_DEV个数达到最大值 */
            oam_warning_log1(0, OAM_SF_CFG, "{hmac_check_p2p_vap_num::only can created 1 p2p_GO/Client[%d].}",
                             mac_device->st_p2p_info.uc_p2p_goclient_num);
            return OAL_ERR_CODE_CONFIG_EXCEED_SPEC;
        }

        /* P2P0是否已经创建 */
        if (mac_device->st_p2p_info.uc_p2p_device_num != WLAN_MAX_SERVICE_P2P_DEV_NUM) {
            /* 已有AP创建 */
            oam_warning_log1(0, OAM_SF_CFG, "{hmac_check_p2p_vap_num::there's no p2p0 %d,can not create p2p_CL.}",
                             mac_device->st_p2p_info.uc_p2p_device_num);
            return OAL_ERR_CODE_CONFIG_EXCEED_SPEC;
        }
    } else if (p2p_mode == WLAN_P2P_GO_MODE) {
        /* 判断已创建的P2P_CL/P2P_GO 个数是否达到最大值 */
        if (mac_device->st_p2p_info.uc_p2p_goclient_num >= WLAN_MAX_SERVICE_P2P_GOCLIENT_NUM) {
            /* 已创建的P2P_DEV个数达到最大值 */
            oam_warning_log1(0, OAM_SF_CFG, "{hmac_check_p2p_vap_num::only can created 1 p2p_GO/Client[%d].}",
                             mac_device->st_p2p_info.uc_p2p_goclient_num);
            return OAL_ERR_CODE_CONFIG_EXCEED_SPEC;
        }
        /* 是否有足够的AP vap模式资源供创建一个P2P_GO */
        if ((mac_device->uc_vap_num - mac_device->uc_sta_num) >= WLAN_SERVICE_AP_MAX_NUM_PER_DEVICE) {
            /* 已创建的AP个数达到最大值4 */
            oam_warning_log1(0, OAM_SF_CFG, "{hmac_check_p2p_vap_num::ap num exceeds the supported spec[%d].}",
                             mac_device->uc_vap_num - mac_device->uc_sta_num);
            return OAL_ERR_CODE_CONFIG_EXCEED_SPEC;
        }
    } else {
        return OAL_ERR_CODE_INVALID_CONFIG; /* 入参错误，无效配置 */
    }
    return OAL_SUCC;
}


uint32_t hmac_add_p2p_cl_vap(mac_vap_stru *mac_vap, uint16_t len, uint8_t *param)
{
    mac_cfg_add_vap_param_stru *vap_param = NULL;
    hmac_vap_stru *hmac_vap = NULL;
    uint32_t ret;
    mac_device_stru *mac_dev = NULL;
    uint8_t vap_id;
    wlan_p2p_mode_enum_uint8 p2p_mode;

    vap_param = (mac_cfg_add_vap_param_stru *)param;
    mac_dev = mac_res_get_dev(mac_vap->uc_device_id);
    if (oal_unlikely(mac_dev == NULL)) {
        oam_error_log0(mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_add_p2p_cl_vap::mac_dev null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* VAP个数判断 */
    p2p_mode = vap_param->en_p2p_mode;
    ret = hmac_check_p2p_vap_num(mac_dev, p2p_mode);
    if (ret != OAL_SUCC) {
        oam_warning_log1(mac_vap->uc_vap_id, OAM_SF_CFG,
                         "{hmac_add_p2p_cl_vap::hmac_config_check_vap_num failed[%d].}", ret);
        return ret;
    }

    /* P2P CL 和P2P0 共用一个VAP 结构，创建P2P CL 时不需要申请VAP 资源，需要返回p2p0 的vap 结构 */
    vap_id = mac_dev->st_p2p_info.uc_p2p0_vap_idx;
    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(vap_id);
    if (oal_unlikely(hmac_vap == NULL)) {
        oam_error_log1(mac_vap->uc_vap_id, OAM_SF_CFG,
                       "{hmac_add_p2p_cl_vap::mac_res_get_hmac_vap failed.vap_id:[%d].}", vap_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

    vap_param->uc_vap_id = vap_id;
    hmac_vap->pst_net_device = vap_param->pst_net_dev;

    /* 包括'\0' */
    if (EOK != memcpy_s(hmac_vap->auc_name, OAL_IF_NAME_SIZE, vap_param->pst_net_dev->name, OAL_IF_NAME_SIZE)) {
        oam_error_log0(0, OAM_SF_CFG, "hmac_add_p2p_cl_vap::memcpy fail!");
        return OAL_FAIL;
    }

    /* 将申请到的mac_vap空间挂到net_device priv指针上去 */
    oal_net_dev_priv(vap_param->pst_net_dev) = &hmac_vap->st_vap_base_info;
    vap_param->us_muti_user_id = hmac_vap->st_vap_base_info.us_multi_user_idx;
    /* 抛事件到DMAC层, 同步DMAC数据 */
    ret = hmac_config_send_event(&hmac_vap->st_vap_base_info, WLAN_CFGID_ADD_VAP, len, param);
    if (oal_unlikely(ret != OAL_SUCC)) {
        /* 异常处理，释放内存 */
        oam_error_log1(mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_add_p2p_cl_vap::alloc_event failed[%d].}", ret);
        return ret;
    }

    if (vap_param->en_vap_mode == WLAN_VAP_MODE_BSS_STA) {
        /* 初始化us_assoc_vap_id为最大值代表ap未关联 */
        mac_vap_set_assoc_id(&hmac_vap->st_vap_base_info, g_wlan_spec_cfg->invalid_user_id);
    }
    mac_vap_set_p2p_mode(&hmac_vap->st_vap_base_info, vap_param->en_p2p_mode);
    mac_inc_p2p_num(&hmac_vap->st_vap_base_info);

    oam_info_log3(mac_vap->uc_vap_id, OAM_SF_ANY, "{hmac_add_p2p_cl_vap::vap_mode[%d], p2p_mode[%d}, vap_id[%d]",
                  vap_param->en_vap_mode, vap_param->en_p2p_mode, vap_param->uc_vap_id);

    return OAL_SUCC;
}


uint32_t hmac_del_p2p_cl_vap(mac_vap_stru *mac_vap, uint16_t len, uint8_t *param)
{
    hmac_vap_stru *hmac_vap = NULL;
    uint32_t ret;
    mac_device_stru *mac_device = NULL;
    uint8_t vap_id;
    mac_cfg_del_vap_param_stru *del_vap_param = NULL;

    if (oal_unlikely(oal_any_null_ptr2(mac_vap, param))) {
        oam_error_log2(0, OAM_SF_CFG, "{hmac_config_del_vap::param null,mac_vap=%d param=%d.}",
                       (uintptr_t)mac_vap, (uintptr_t)param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    del_vap_param = (mac_cfg_del_vap_param_stru *)param;
    mac_device = mac_res_get_dev(mac_vap->uc_device_id);
    if (oal_unlikely(mac_device == NULL)) {
        oam_error_log1(mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_config_del_vap::mac_device null.devid[%d]}",
                       mac_vap->uc_device_id);
        return OAL_ERR_CODE_PTR_NULL;
    }
    /* 如果是删除P2P CL ，则不需要释放VAP 资源 */
    vap_id = mac_device->st_p2p_info.uc_p2p0_vap_idx;
    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(vap_id);
    if (oal_unlikely(hmac_vap == NULL)) {
        oam_error_log1(mac_vap->uc_vap_id, OAM_SF_CFG,
                       "{hmac_config_del_vap::get_hmac_vap fail.vap_id[%d]}", vap_id);
        return OAL_ERR_CODE_PTR_NULL;
    }
    oal_net_dev_priv(hmac_vap->pst_net_device) = NULL;
    hmac_vap->pst_net_device = hmac_vap->pst_p2p0_net_device;
    mac_dec_p2p_num(&hmac_vap->st_vap_base_info);
    mac_vap_set_p2p_mode(&hmac_vap->st_vap_base_info, WLAN_P2P_DEV_MODE);
    if (memcpy_s(mac_mib_get_StationID(&hmac_vap->st_vap_base_info), WLAN_MAC_ADDR_LEN,
                 mac_mib_get_p2p0_dot11StationID(&hmac_vap->st_vap_base_info), WLAN_MAC_ADDR_LEN) != EOK) {
        oam_error_log0(0, OAM_SF_CFG, "hmac_del_p2p_cl_vap::memcpy fail!");
        return OAL_FAIL;
    }

    /* 抛事件到DMAC层, 同步DMAC数据 */
    ret = hmac_config_send_event(mac_vap, WLAN_CFGID_DEL_VAP, len, param);
    if (oal_unlikely(ret != OAL_SUCC)) {
        oam_warning_log1(mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_del_p2p_cl_vap::send_event failed[%d].}", ret);
        return ret;
    }

    oam_warning_log2(mac_vap->uc_vap_id, OAM_SF_ANY, "{hmac_del_p2p_cl_vap::func out.vap_mode[%d], p2p_mode[%d]}",
                     del_vap_param->en_vap_mode, del_vap_param->en_p2p_mode);
    return OAL_SUCC;
}


uint32_t hmac_p2p_send_listen_expired_to_host(hmac_vap_stru *hmac_vap)
{
    mac_device_stru *mac_device = NULL;
    frw_event_mem_stru *event_mem = NULL;
    frw_event_stru *event = NULL;
    oal_wireless_dev_stru *wdev = NULL;
    mac_p2p_info_stru *p2p_info = NULL;
    hmac_p2p_listen_expired_stru *p2p_listen_expired = NULL;

    mac_device = mac_res_get_dev(hmac_vap->st_vap_base_info.uc_device_id);
    if (mac_device == NULL) {
        oam_warning_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_P2P,
                         "{hmac_send_mgmt_to_host::mac_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    p2p_info = &mac_device->st_p2p_info;
    /* 填写上报监听超时, 上报的网络设备应该采用p2p0 */
    if (hmac_vap->pst_p2p0_net_device && hmac_vap->pst_p2p0_net_device->ieee80211_ptr) {
        wdev = hmac_vap->pst_p2p0_net_device->ieee80211_ptr;
    } else if (hmac_vap->pst_net_device && hmac_vap->pst_net_device->ieee80211_ptr) {
        wdev = hmac_vap->pst_net_device->ieee80211_ptr;
    } else {
        oam_warning_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_P2P,
                         "{hmac_send_mgmt_to_host::vap has deleted.}");
        return OAL_FAIL;
    }

    /* 组装事件到WAL，上报监听结束 */
    event_mem = frw_event_alloc_m(OAL_SIZEOF(hmac_p2p_listen_expired_stru));
    if (event_mem == NULL) {
        oam_error_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_P2P,
                       "{hmac_send_mgmt_to_host::event_mem null.}");
        return OAL_FAIL;
    }

    /* 填写事件 */
    event = frw_get_event_stru(event_mem);
    frw_event_hdr_init(&(event->st_event_hdr), FRW_EVENT_TYPE_HOST_CTX, HMAC_HOST_CTX_EVENT_SUB_TYPE_LISTEN_EXPIRED,
                       OAL_SIZEOF(hmac_p2p_listen_expired_stru), FRW_EVENT_PIPELINE_STAGE_0,
                       hmac_vap->st_vap_base_info.uc_chip_id, hmac_vap->st_vap_base_info.uc_device_id,
                       hmac_vap->st_vap_base_info.uc_vap_id);

    p2p_listen_expired = (hmac_p2p_listen_expired_stru *)(event->auc_event_data);
    p2p_listen_expired->st_listen_channel = p2p_info->st_listen_channel;
    p2p_listen_expired->pst_wdev = wdev;

    /* 分发事件 */
    frw_event_dispatch_event(event_mem);
    frw_event_free_m(event_mem);

    return OAL_SUCC;
}


uint32_t hmac_p2p_send_listen_expired_to_device(hmac_vap_stru *hmac_vap)
{
    mac_device_stru *mac_device = NULL;
    mac_vap_stru *mac_vap = NULL;
    mac_p2p_info_stru *p2p_info = NULL;
    uint32_t ret;
    hmac_device_stru *hmac_device = NULL;

    mac_device = mac_res_get_dev(hmac_vap->st_vap_base_info.uc_device_id);
    if (mac_device == NULL) {
        oam_error_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_P2P,
                       "{hmac_p2p_send_listen_expired_to_device::mac_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    oam_info_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_P2P,
                  "{hmac_p2p_send_listen_expired_to_device:: listen timeout!.}");
    /* 抛事件到DMAC层, 同步DMAC数据 */
    p2p_info = &mac_device->st_p2p_info;
    mac_vap = &hmac_vap->st_vap_base_info;
    ret = hmac_config_send_event(mac_vap, WLAN_CFGID_CFG80211_CANCEL_REMAIN_ON_CHANNEL,
                                 OAL_SIZEOF(mac_p2p_info_stru), (uint8_t *)p2p_info);
    /* 强制stop listen */
    hmac_device = hmac_res_get_mac_dev(hmac_vap->st_vap_base_info.uc_device_id);
    if (oal_unlikely(hmac_device == NULL)) {
        oam_error_log1(0, OAM_SF_ANY, "{hmac_p2p_send_listen_expired_to_device::hmac_device[%d] null!}",
                       hmac_vap->st_vap_base_info.uc_device_id);
        return OAL_ERR_CODE_PTR_NULL;
    }
    if (hmac_device->st_scan_mgmt.en_is_scanning == OAL_TRUE) {
        hmac_device->st_scan_mgmt.en_is_scanning = OAL_FALSE;
    }

    if (oal_unlikely(ret != OAL_SUCC)) {
        oam_warning_log1(mac_vap->uc_vap_id, OAM_SF_P2P,
            "{hmac_p2p_send_listen_expired_to_device::hmac_config_send_event failed[%d].}", ret);
        return ret;
    }

    return OAL_SUCC;
}


void hmac_disable_p2p_pm(hmac_vap_stru *hmac_vap)
{
    mac_vap_stru *mac_vap = NULL;
    mac_cfg_p2p_ops_param_stru p2p_ops;
    mac_cfg_p2p_noa_param_stru p2p_noa;
    uint32_t ret;

    mac_vap = &(hmac_vap->st_vap_base_info);
    memset_s(&p2p_noa, OAL_SIZEOF(p2p_noa), 0, OAL_SIZEOF(p2p_noa));
    ret = hmac_config_set_p2p_ps_noa(mac_vap, OAL_SIZEOF(mac_cfg_p2p_noa_param_stru), (uint8_t *)&p2p_noa);
    if (ret != OAL_SUCC) {
        oam_error_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_P2P,
                       "{hmac_disable_p2p_pm::hmac_config_set_p2p_ps_noa disable p2p NoA fail.}");
    }
    memset_s(&p2p_ops, OAL_SIZEOF(p2p_ops), 0, OAL_SIZEOF(p2p_ops));
    ret = hmac_config_set_p2p_ps_ops(mac_vap, OAL_SIZEOF(mac_cfg_p2p_ops_param_stru), (uint8_t *)&p2p_ops);
    if (ret != OAL_SUCC) {
        oam_error_log0(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_P2P,
                       "{hmac_disable_p2p_pm::hmac_config_set_p2p_ps_ops disable p2p OppPS fail.}");
    }
}


oal_bool_enum_uint8 hmac_is_p2p_go_neg_req_frame(OAL_CONST uint8_t *data)
{
    uint8_t ret = ((data[MAC_ACTION_OFFSET_CATEGORY + MAC_80211_FRAME_LEN] == MAC_ACTION_CATEGORY_PUBLIC) &&
                   (data[P2P_PUB_ACT_OUI_OFF1 + MAC_80211_FRAME_LEN] == WFA_OUI_BYTE1) &&
                   (data[P2P_PUB_ACT_OUI_OFF2 + MAC_80211_FRAME_LEN] == WFA_OUI_BYTE2) &&
                   (data[P2P_PUB_ACT_OUI_OFF3 + MAC_80211_FRAME_LEN] == WFA_OUI_BYTE3) &&
                   (data[P2P_PUB_ACT_OUI_TYPE_OFF + MAC_80211_FRAME_LEN] == WFA_P2P_v1_0) &&
                   (data[P2P_PUB_ACT_OUI_SUBTYPE_OFF + MAC_80211_FRAME_LEN] == P2P_PAF_GON_REQ));

    if (ret != OAL_FALSE) {
        return OAL_TRUE;
    } else {
        return OAL_FALSE;
    }
}


uint8_t hmac_get_p2p_status(uint32_t p2p_status, hmac_cfgp2p_status_enum_uint32 status)
{
    if (p2p_status & BIT(status)) {
        return OAL_TRUE;
    } else {
        return OAL_FALSE;
    }
}


void hmac_set_p2p_status(uint32_t *p2p_status, hmac_cfgp2p_status_enum_uint32 status)
{
    *p2p_status |= ((uint32_t)BIT(status));
}


void hmac_clr_p2p_status(uint32_t *p2p_status, hmac_cfgp2p_status_enum_uint32 status)
{
    *p2p_status &= ~((uint32_t)BIT(status));
}


OAL_STATIC void hmac_p2p_go_back_to_80m(mac_vap_stru *go_mac_vap)
{
    wlan_bw_cap_enum_uint8 bw_cap;
    wlan_channel_bandwidth_enum_uint8 new_80bw;

    bw_cap = mac_vap_bw_mode_to_bw(go_mac_vap->st_channel.en_bandwidth);
    if (bw_cap == WLAN_BW_CAP_160M) {
        new_80bw = mac_regdomain_get_support_bw_mode(WLAN_BAND_WIDTH_80M, go_mac_vap->st_channel.uc_chan_number);
        bw_cap = mac_vap_bw_mode_to_bw(new_80bw);
        if (bw_cap == WLAN_BW_CAP_80M) {
            oam_warning_log2(0, OAM_SF_QOS, "{hmac_p2p_go_back_to_80m::GO BW change from \
                [%d] to [%d]}", go_mac_vap->st_channel.en_bandwidth, new_80bw);
#ifdef _PRE_WLAN_FEATURE_DFS
            hmac_dfs_status_set(HMAC_BACK_80M);
#endif
            go_mac_vap->st_ch_switch_info.uc_ch_switch_cnt = 1;
            go_mac_vap->st_ch_switch_info.en_csa_mode      = WLAN_CSA_MODE_TX_ENABLE;
            hmac_chan_initiate_switch_to_new_channel(go_mac_vap,
                go_mac_vap->st_channel.uc_chan_number, new_80bw);
        }
    }
}


void hmac_p2p_sta_join_go_back_to_80m_handle(mac_vap_stru *sta_mac_vap)
{
#ifdef _PRE_WLAN_FEATURE_160M
    mac_vap_stru *go_mac_vap = NULL;
    if (!IS_LEGACY_STA(sta_mac_vap)) {
        return;
    }
    if (mac_vap_go_can_not_in_160M_check(sta_mac_vap, sta_mac_vap->st_channel.uc_chan_number) == OAL_TRUE) {
        go_mac_vap = mac_vap_find_another_up_vap_by_mac_vap(sta_mac_vap);
        if (go_mac_vap != NULL && IS_P2P_GO(go_mac_vap)) {
            hmac_p2p_go_back_to_80m(go_mac_vap);
        }
    }
#endif
}

uint32_t hmac_p2p_change_go_channel(mac_vap_stru *mac_vap, uint8_t len, uint8_t *param)
{
    uint8_t new_chanel;
    wlan_bw_cap_enum_uint8 bw_cap;
    wlan_channel_bandwidth_enum_uint8 go_new_bw;
    mac_device_stru *mac_device = NULL;
    mac_vap_stru *all_mac_vap[2] = {NULL}; /* 2为变量个数 */
    mac_vap_stru *p2p_vap = NULL;

    new_chanel = *(uint8_t *)param;

    mac_device = mac_res_get_dev(mac_vap->uc_device_id);
    if (mac_device == NULL) {
        return OAL_FAIL;
    }

    oam_warning_log3(0, OAM_SF_ANY, "{hmac_p2p_change_go_channel::vap_nums=[%d], en_dbac_running = %d, new_chanel=%d.}",
        mac_device->uc_vap_num, mac_device->en_dbac_running, new_chanel);
    if (mac_device_find_2up_vap(mac_device, &all_mac_vap[0], &all_mac_vap[1]) == OAL_SUCC) {
        if (mac_device->en_dbac_running) {
            p2p_vap = (IS_LEGACY_STA(all_mac_vap[0]) ? all_mac_vap[1] : all_mac_vap[0]);
        }
    } else if (mac_device_find_up_vap(mac_device, &p2p_vap) != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_ANY, "{hmac_p2p_change_go_channel::mac_device_find_up_vap fail.}");
        return OAL_FAIL;
    }

    if (p2p_vap == NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{hmac_p2p_change_go_channel::p2p_vap is null.}");
        return OAL_FAIL;
    }

    oam_warning_log4(p2p_vap->uc_vap_id, OAM_SF_ANY,
        "{hmac_p2p_change_go_channel::vap_nums=%d , en_p2p_mode=[%d], vap_support_csa=%d, new_chanel=%d.}",
        mac_device->uc_vap_num, p2p_vap->en_p2p_mode, p2p_vap->bit_vap_support_csa, new_chanel);
    if (!IS_P2P_GO(p2p_vap) || !p2p_vap->bit_vap_support_csa) {
        return OAL_FAIL;
    }

    if (mac_is_channel_num_valid(p2p_vap->st_channel.en_band, new_chanel) != OAL_SUCC) {
        oam_warning_log2(p2p_vap->uc_vap_id, OAM_SF_ANY, "{hmac_p2p_change_go_channel::vap_band=[%d], new_chanel=%d.}",
            p2p_vap->st_channel.en_band, new_chanel);
        return OAL_FAIL;
    }

    bw_cap = mac_vap_bw_mode_to_bw(p2p_vap->st_channel.en_bandwidth);
    go_new_bw = mac_regdomain_get_bw_by_channel_bw_cap(new_chanel, bw_cap);

    oam_warning_log4(0, OAM_SF_DBAC, "{hmac_p2p_change_go_channel::go channel from [%d][%d] to [%d][%d]}",
        p2p_vap->st_channel.uc_chan_number, p2p_vap->st_channel.en_bandwidth, new_chanel, go_new_bw);
    p2p_vap->st_ch_switch_info.uc_ch_switch_cnt = HMAC_P2P_GO_VENDOR_CMD_CSA_COUNT;
    p2p_vap->st_ch_switch_info.en_csa_mode      = WLAN_CSA_MODE_TX_ENABLE;
    hmac_chan_initiate_switch_to_new_channel(p2p_vap, new_chanel, go_new_bw);

    return OAL_SUCC;
}


uint32_t hmac_p2p_set_scenes(mac_vap_stru *mac_vap, uint8_t len, uint8_t *param)
{
    mac_device_stru *p_mac_device = OAL_PTR_NULL;
    uint8_t value;
    uint32_t ret;

    value = *(uint8_t *)param;

    p_mac_device = mac_res_get_dev(mac_vap->uc_device_id);
    if (p_mac_device == OAL_PTR_NULL) {
        oam_warning_log0(mac_vap->uc_vap_id, OAM_SF_ANY, "hmac_p2p_set_scenes:: mac_device is NULL");
        return OAL_PTR_NULL;
    }

    p_mac_device->st_p2p_info.p2p_scenes = value;

    /* 抛事件到DMAC层, 同步DMAC数据 */
    ret = hmac_config_send_event(mac_vap, WALN_CFGID_SET_P2P_SCENES, len, param);
    if (oal_unlikely(ret != OAL_SUCC)) {
        oam_warning_log1(mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_p2p_set_scenes:: failed[%d].}", ret);
    }

    return OAL_SUCC;
}

mac_p2p_scenes_enum_uint8 hmac_p2p_get_scenes(mac_vap_stru *mac_vap)
{
    mac_device_stru *mac_device = OAL_PTR_NULL;
    mac_p2p_scenes_enum_uint8 value;

    if (mac_vap == OAL_PTR_NULL || IS_LEGACY_VAP(mac_vap)) {
        return AC_P2P_SCENES_BUTT;
    }

    mac_device = mac_res_get_dev(mac_vap->uc_device_id);
    if (mac_device == OAL_PTR_NULL) {
        oam_warning_log0(mac_vap->uc_vap_id, OAM_SF_P2P, "hmac_p2p_get_scenes:: mac_device is NULL");
        return AC_P2P_SCENES_BUTT;
    }

    value = mac_device->st_p2p_info.p2p_scenes;

    return value;
}

void hmac_vap_join_avoid_dbac_back_to_11n_handle(mac_vap_stru *mac_vap, mac_bss_dscr_stru *bss_dscr)
{
    if (mac_vap_need_proto_downgrade(mac_vap) == OAL_FALSE ||
        bss_dscr->en_p2p_scenes != MAC_P2P_SCENES_LOW_LATENCY) {
        return;
    }

    if (OAL_TRUE == mac_vap_avoid_dbac_close_vht_protocol(mac_vap)) {
        bss_dscr->en_vht_capable = OAL_FALSE;
        mac_mib_set_VHTOptionImplemented(mac_vap, OAL_FALSE);
    } else {
        mac_mib_set_VHTOptionImplemented(mac_vap, OAL_TRUE);
    }
}

/*lint -e578*/ /*lint -e19*/
oal_module_symbol(hmac_get_p2p_status);
oal_module_symbol(hmac_set_p2p_status);
oal_module_symbol(hmac_clr_p2p_status);
/*lint +e578*/ /*lint +e19*/
