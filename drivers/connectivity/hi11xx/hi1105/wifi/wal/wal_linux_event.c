

/* 1 ͷ�ļ����� */
#include "oal_ext_if.h"
#include "frw_ext_if.h"
#include "wal_linux_ioctl.h"
#include "wal_main.h"
#include "wal_config.h"
#include "wal_linux_scan.h"
#include "hmac_resource.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_WAL_LINUX_EVENT_C

/* 2 ȫ�ֱ������� */
/* 3 ����ʵ�� */

int32_t wal_cfg80211_start_req(oal_net_device_stru *pst_net_dev,
                                     OAL_CONST void *ps_param,
                                     uint16_t us_len,
                                     wlan_cfgid_enum_uint16 en_wid,
                                     oal_bool_enum_uint8 en_need_rsp)
{
    wal_msg_write_stru st_write_msg;
    wal_msg_stru *pst_rsp_msg = OAL_PTR_NULL;
    uint32_t ul_err_code;
    int32_t l_ret;

    if (ps_param == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_SCAN, "{wal_cfg80211_start_req::param is null!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ��д msg ��Ϣͷ */
    st_write_msg.en_wid = en_wid;
    st_write_msg.us_len = us_len;

    /* ��д msg ��Ϣ�� */
    if (us_len > WAL_MSG_WRITE_MAX_LEN) {
        oam_error_log2(0, OAM_SF_SCAN,
                       "{wal_cfg80211_start_req::us_len %d > WAL_MSG_WRITE_MAX_LEN %d err!}\r\n",
                       us_len, WAL_MSG_WRITE_MAX_LEN);
        return OAL_ERR_CODE_INVALID_CONFIG;
    }
    if (EOK != memcpy_s(st_write_msg.auc_value, us_len, ps_param, us_len)) {
        oam_error_log0(0, OAM_SF_SCAN, "wal_cfg80211_start_req::memcpy fail!");
        return -OAL_EFAIL;
    }

    /***************************************************************************
           ���¼���wal�㴦��
    ***************************************************************************/
    l_ret = wal_send_cfg_event(pst_net_dev,
                                   WAL_MSG_TYPE_WRITE,
                                   WAL_MSG_WRITE_MSG_HDR_LENGTH + us_len,
                                   (uint8_t *)&st_write_msg,
                                   en_need_rsp,
                                   en_need_rsp ? &pst_rsp_msg : OAL_PTR_NULL);
    if (oal_unlikely(l_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_SCAN,
                         "{wal_cfg80211_start_req::wal_send_cfg_event return err code %d!}\r\n", l_ret);
        return l_ret;
    }

    if (en_need_rsp && (pst_rsp_msg != OAL_PTR_NULL)) {
        /* ��ȡ���صĴ����� */
        ul_err_code = wal_check_and_release_msg_resp(pst_rsp_msg);
        if (ul_err_code != OAL_SUCC) {
            oam_warning_log1(0, OAM_SF_SCAN,
                             "{wal_cfg80211_start_req::wal_send_cfg_event return err code:[%u]}",
                             ul_err_code);
            return -OAL_EFAIL;
        }
    }

    return OAL_SUCC;
}


uint32_t wal_cfg80211_start_scan(oal_net_device_stru *pst_net_dev, mac_cfg80211_scan_param_stru *pst_scan_param)
{
    if (pst_scan_param == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_SCAN,
                       "{wal_cfg80211_start_scan::scan failed, null ptr, pst_scan_param = null.}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    return (uint32_t)wal_cfg80211_start_req(pst_net_dev, pst_scan_param,
                                                  OAL_SIZEOF(mac_cfg80211_scan_param_stru),
                                                  WLAN_CFGID_CFG80211_START_SCAN, OAL_FALSE);
}


uint32_t wal_cfg80211_start_sched_scan(oal_net_device_stru *pst_net_dev, mac_pno_scan_stru *pst_pno_scan_info)
{
    mac_pno_scan_stru *pst_pno_scan_params;
    uint32_t ul_ret;
    /* ����pno����ɨ��������˴�����hmac���ͷ� */
    pst_pno_scan_params = (mac_pno_scan_stru *)oal_mem_alloc_m(OAL_MEM_POOL_ID_LOCAL,
                                                             OAL_SIZEOF(mac_pno_scan_stru), OAL_FALSE);
    if (pst_pno_scan_params == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_SCAN,
                         "{wal_cfg80211_start_sched_scan::alloc pno scan param memory failed!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    memcpy_s(pst_pno_scan_params, OAL_SIZEOF(mac_pno_scan_stru), pst_pno_scan_info, OAL_SIZEOF(mac_pno_scan_stru));

    ul_ret = (uint32_t)wal_cfg80211_start_req(pst_net_dev, &pst_pno_scan_params,
                                                    OAL_SIZEOF(mac_pno_scan_stru *),
                                                    WLAN_CFGID_CFG80211_START_SCHED_SCAN, OAL_TRUE);

    /* ����wal_cfg80211_start_req�ڲ�ִ�гɹ����ͳһ���ⲿ�ͷ� */
    oal_mem_free_m(pst_pno_scan_params, OAL_FALSE);

    return ul_ret;
}


int32_t wal_cfg80211_start_connect(oal_net_device_stru *pst_net_dev, mac_conn_param_stru *pst_mac_conn_param)
{
    return wal_cfg80211_start_req(pst_net_dev,
                                      pst_mac_conn_param,
                                      OAL_SIZEOF(mac_conn_param_stru),
                                      WLAN_CFGID_CFG80211_START_CONNECT,
                                      OAL_TRUE);
}


int32_t wal_cfg80211_start_disconnect(oal_net_device_stru *pst_net_dev,
                                            mac_cfg_kick_user_param_stru *pst_disconnect_param)
{
    mac_vap_stru *pst_mac_vap;
    hmac_device_stru *hmac_device = NULL;
    hmac_scan_record_stru *scan_record = NULL;

    pst_mac_vap = oal_net_dev_priv(pst_net_dev);
    // ��ֹ���ܴ��ڵ�ɨ��
    if (pst_mac_vap != OAL_PTR_NULL && (pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_STA)) {
        hmac_device = hmac_res_get_mac_dev(pst_mac_vap->uc_device_id);
        if (hmac_device == NULL) {
            oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_SCAN,
                             "{wal_cfg80211_start_disconnect::hmac_device[%d] is null!}",
                             pst_mac_vap->uc_device_id);
            return -OAL_EINVAL;
        }
        /* abort����ɨ�裬��Ҫ�Ȱ�ɨ��ص������ó�null�����������ִ��������������ɾuser����ʱ�Ҳ���user */
        scan_record = &hmac_device->st_scan_mgmt.st_scan_record_mgmt;
        if (scan_record->en_scan_mode == WLAN_SCAN_MODE_ROAM_SCAN) {
            scan_record->p_fn_cb = NULL;
        }
        if (hmac_device->st_scan_mgmt.pst_request == NULL) {
            /* �ж��Ƿ�����ڲ�ɨ�裬������ڣ�Ҳ��Ҫֹͣ */
            if ((hmac_device->st_scan_mgmt.en_is_scanning == OAL_TRUE) &&
                (pst_mac_vap->uc_vap_id == scan_record->uc_vap_id) &&
                (pst_mac_vap->en_vap_state == MAC_VAP_STATE_STA_LISTEN)) {
                oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_SCAN, "{cfg80211_start_disconnect::clear scan cb!}");
                scan_record->p_fn_cb = NULL;
            }
        }
        wal_force_scan_abort_then_scan_comp(pst_net_dev);
    }

    /* ע�� ������Ϣδ���������ֱ�ӷ��أ�����WPA_SUPPLICANT�����·���Ϣ����������ȵ�����ʱ���쳣���ѣ����º����·�����Ϣ����Ϊ����ʧ�ܣ�
       Ŀǰ��ȥ�����¼��޸�Ϊ�ȴ���Ϣ������������ϱ������һ�������OAL_FALSE��ΪOAL_TRUE */
    return wal_cfg80211_start_req(pst_net_dev, pst_disconnect_param,
                                      OAL_SIZEOF(mac_cfg_kick_user_param_stru), WLAN_CFGID_KICK_USER, OAL_TRUE);
}

#ifdef _PRE_WLAN_FEATURE_SAE

uint32_t wal_cfg80211_do_external_auth(oal_net_device_stru *pst_netdev, hmac_external_auth_req_stru *pst_ext_auth)
{
    return (uint32_t)wal_cfg80211_start_req(pst_netdev, pst_ext_auth,
                                                  OAL_SIZEOF(*pst_ext_auth),
                                                  WLAN_CFGID_CFG80211_EXTERNAL_AUTH, OAL_TRUE);
}
#endif /* _PRE_WLAN_FEATURE_SAE */

