


#include "oal_ext_if.h"
#include "oal_types.h"
#include "oal_net.h"
#include "oal_aes.h"
#include "frw_ext_if.h"
#include "wlan_spec.h"
#include "wlan_types.h"
#include "mac_resource.h"
#include "mac_frame.h"
#include "mac_device.h"
#include "mac_resource.h"
#include "mac_vap.h"

#include "hmac_11i.h"
#ifdef _PRE_WLAN_FEATURE_WAPI
#include "hmac_wapi.h"
#endif
#include "hmac_main.h"
#include "hmac_crypto_tkip.h"
#include "hmac_config.h"
#include "hmac_mgmt_bss_comm.h"
#include "hmac_roam_main.h"
#include "securec.h"
#include "securectype.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_11I_C


OAL_STATIC wlan_priv_key_param_stru *hmac_get_key_info(mac_vap_stru *pst_mac_vap, uint8_t *puc_mac_addr,
    oal_bool_enum_uint8 en_pairwise, uint8_t uc_key_index, uint16_t *pus_user_idx)
{
    oal_bool_enum_uint8 en_macaddr_is_zero;
    mac_user_stru *pst_mac_user = OAL_PTR_NULL;
    uint32_t ul_ret;

    /* 1.1 ����mac addr �ҵ���Ӧsta������ */
    en_macaddr_is_zero = mac_addr_is_zero(puc_mac_addr);
    if (!MAC_11I_IS_PTK(en_macaddr_is_zero, en_pairwise)) {
        /* ������鲥�û�������ʹ��mac��ַ������, ���������ҵ��鲥user�ڴ����� */
        *pus_user_idx = pst_mac_vap->us_multi_user_idx;
    } else { /* �����û� */
        ul_ret = mac_vap_find_user_by_macaddr(pst_mac_vap, puc_mac_addr, pus_user_idx);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
                             "{hmac_get_key_info::mac_vap_find_user_by_macaddr failed[%d]}", ul_ret);
            return OAL_PTR_NULL;
        }
    }

    pst_mac_user = (mac_user_stru *)mac_res_get_mac_user(*pus_user_idx);
    if (pst_mac_user == OAL_PTR_NULL) {
        oam_warning_log1(pst_mac_vap->uc_vap_id,
            OAM_SF_WPA, "{hmac_get_key_info::pst_mac_user[%d] null.}", *pus_user_idx);
        return OAL_PTR_NULL;
    }

    /* LOG */
    oam_info_log2(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
                  "{hmac_get_key_info::key_index=%d,pairwise=%d.}", uc_key_index, en_pairwise);

    if (puc_mac_addr != OAL_PTR_NULL) {
        oam_info_log4(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
                      "{hmac_get_key_info::user[%d] mac_addr = %02X:XX:XX:XX:%02X:%02X.}",
                      *pus_user_idx, puc_mac_addr[0], puc_mac_addr[4], puc_mac_addr[5]); // mac��ַ�ĵ�0 4 5�ֽ�
    }

    return mac_user_get_key(pst_mac_user, uc_key_index);
}
#ifdef _PRE_WLAN_FEATURE_WAPI

uint32_t hmac_config_wapi_add_key(mac_vap_stru *pst_mac_vap,
    mac_addkey_param_stru *pst_payload_addkey_params)
{
    uint8_t uc_key_index;
    oal_bool_enum_uint8 en_pairwise;
    uint8_t *puc_mac_addr = OAL_PTR_NULL;
    mac_key_params_stru *pst_key_param = OAL_PTR_NULL;
    hmac_wapi_stru *pst_wapi = OAL_PTR_NULL;
    uint32_t ul_ret;
    uint16_t us_user_index = 0;
    mac_device_stru *pst_mac_device = OAL_PTR_NULL;

    uc_key_index = pst_payload_addkey_params->uc_key_index;
    if (uc_key_index >= HMAC_WAPI_MAX_KEYID) {
        oam_error_log1(0, OAM_SF_WPA, "{hmac_config_wapi_add_key::keyid==%u Err!.}", uc_key_index);
        return OAL_FAIL;
    }

    en_pairwise = pst_payload_addkey_params->en_pairwise;
    puc_mac_addr = (uint8_t *)pst_payload_addkey_params->auc_mac_addr;
    pst_key_param = &pst_payload_addkey_params->st_key;

    if (pst_key_param->key_len != (WAPI_KEY_LEN * 2)) { // key_lenֵΪ32��WAPI_KEY_LEN��2��
        oam_error_log1(0, OAM_SF_WPA, "{hmac_config_wapi_add_key:: key_len %u Err!.}", pst_key_param->key_len);
        return OAL_FAIL;
    }

    if (en_pairwise) {
        ul_ret = mac_vap_find_user_by_macaddr(pst_mac_vap, puc_mac_addr, &us_user_index);
        if (ul_ret != OAL_SUCC) {
            oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                "{hmac_config_wapi_add_key::mac_vap_find_user_by_macaddr failed. %u}", ul_ret);
            return OAL_ERR_CODE_PTR_NULL;
        }
    }

    pst_wapi = hmac_user_get_wapi_ptr(pst_mac_vap, en_pairwise, us_user_index);
    if (pst_wapi == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_WPA, "{hmac_config_wapi_add_key:: get pst_wapi  Err!.}");
        return OAL_FAIL;
    }

    hmac_wapi_add_key(pst_wapi, uc_key_index, pst_key_param->auc_key);

    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (pst_mac_device == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_config_wapi_add_key::pst_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    pst_mac_device->uc_wapi = OAL_TRUE;

    return OAL_SUCC;
}


uint32_t hmac_config_wapi_add_key_and_sync(mac_vap_stru *pst_mac_vap,
    mac_addkey_param_stru *pst_payload_addkey_params)
{
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    uint32_t ul_ret;

    oam_warning_log2(0, OAM_SF_WPA, "{hmac_config_wapi_add_key_and_sync:: key idx==%u, pairwise==%u}",
        pst_payload_addkey_params->uc_key_index, pst_payload_addkey_params->en_pairwise);

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{hmac_config_11i_add_key::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    ul_ret = hmac_config_wapi_add_key(&pst_hmac_vap->st_vap_base_info, pst_payload_addkey_params);
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
            "{hmac_config_11i_add_key::hmac_config_wapi_add_key fail[%d].}", ul_ret);
        return ul_ret;
    }

    ul_ret = hmac_config_send_event(pst_mac_vap, WLAN_CFGID_ADD_WAPI_KEY, 0, OAL_PTR_NULL);
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
            "{hmac_config_11i_add_key::WLAN_CFGID_ADD_WAPI_KEY send fail[%d].}", ul_ret);
        return ul_ret;
    }

    return ul_ret;
}

#endif /* #ifdef _PRE_WLAN_FEATURE_WAPI */
void hmac_user_clear_ptk_rx_pn(hmac_user_stru *hmac_user)
{
    memset_s(&(hmac_user->last_pn_info.qos_last_lsb_pn),
             WLAN_TID_MAX_NUM * OAL_SIZEOF(oal_uint32),
             0,
             WLAN_TID_MAX_NUM * OAL_SIZEOF(oal_uint32));
    memset_s(&(hmac_user->last_pn_info.qos_last_msb_pn),
             WLAN_TID_MAX_NUM * OAL_SIZEOF(oal_uint16),
             0,
             WLAN_TID_MAX_NUM * OAL_SIZEOF(oal_uint16));
    hmac_user->last_pn_info.ucast_nonqos_last_lsb_pn = 0;
    hmac_user->last_pn_info.ucast_nonqos_last_msb_pn = 0;
}
void hmac_user_clear_gtk_rx_pn(hmac_user_stru *hmac_user)
{
    hmac_user->last_pn_info.mcast_nonqos_last_lsb_pn = 0;
    hmac_user->last_pn_info.mcast_nonqos_last_msb_pn = 0;
    hmac_user->last_pn_info.ucast_nonqos_last_lsb_pn = 0;
    hmac_user->last_pn_info.ucast_nonqos_last_msb_pn = 0;
}


OAL_STATIC void hmac_user_clear_rx_pn(mac_vap_stru *mac_vap, hmac_user_stru *hmac_user,
    uint8_t pairwise)
{
    uint16_t user_idx = g_wlan_spec_cfg->invalid_user_id;

    if (pairwise) {
        hmac_user_clear_ptk_rx_pn(hmac_user);
    } else {
        hmac_user_clear_gtk_rx_pn(hmac_user);
    }

    if (!IS_STA(mac_vap)) {
        return;
    }

    /* STAֻ��һ�������û����鲥��Կ����ʱ����Ҫ�ҵ������û��¼�¼��PN�Ž������ */
    if (OAL_SUCC != mac_vap_find_user_by_macaddr(mac_vap, mac_vap->auc_bssid, &user_idx)) {
        oam_warning_log1(mac_vap->uc_vap_id, OAM_SF_WPA,
            "{hmac_user_clear_rx_pn::user_idx[%d] find_user_by_macaddr fail.}", user_idx);
        return;
    }

    /* �����û������ټ������� */
    if (user_idx == hmac_user->st_user_base_info.us_assoc_id) {
        return;
    }

    hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(user_idx);
    if (hmac_user == NULL) {
        return;
    }

    oam_warning_log2(mac_vap->uc_vap_id, OAM_SF_WPA,
        "{hmac_user_clear_rx_pn::user_idx[%d] multi_user_idx[%d].}", user_idx, mac_vap->us_multi_user_idx);

    hmac_user_clear_gtk_rx_pn(hmac_user);
    return;
}



uint32_t hmac_config_11i_add_key(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    uint8_t *puc_mac_addr = OAL_PTR_NULL;
    hmac_user_stru *pst_hmac_user = OAL_PTR_NULL;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    mac_key_params_stru *pst_key = OAL_PTR_NULL;
    mac_addkey_param_stru *pst_payload_addkey_params = OAL_PTR_NULL;
    uint32_t ul_ret;
    uint16_t us_user_idx = g_wlan_spec_cfg->invalid_user_id;
    oal_bool_enum_uint8 en_pairwise;
    uint8_t uc_key_index;

#ifdef _PRE_WLAN_FEATURE_WAPI
    mac_device_stru *pst_mac_device = OAL_PTR_NULL;
#endif

    /* 1.1 ��μ�� */
    if (oal_any_null_ptr2(pst_mac_vap, puc_param)) {
        oam_error_log2(0, OAM_SF_WPA, "{hmac_config_11i_add_key::param null,pst_mac_vap=%d, puc_param=%d.}",
                       (uintptr_t)pst_mac_vap, (uintptr_t)puc_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_config_11i_add_key::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 2.1 ��ȡ���� */
    pst_payload_addkey_params = (mac_addkey_param_stru *)puc_param;
    uc_key_index = pst_payload_addkey_params->uc_key_index;
    en_pairwise = pst_payload_addkey_params->en_pairwise;
    puc_mac_addr = (uint8_t *)pst_payload_addkey_params->auc_mac_addr;
    pst_key = &(pst_payload_addkey_params->st_key);

#ifdef _PRE_WLAN_FEATURE_WAPI
    if (oal_unlikely(WLAN_CIPHER_SUITE_SMS4 == pst_key->cipher)) {
        return hmac_config_wapi_add_key_and_sync(pst_mac_vap, pst_payload_addkey_params);
    }
#endif

    /* 2.2 ����ֵ���ֵ��� */
    if (uc_key_index >= WLAN_NUM_TK + WLAN_NUM_IGTK) {
        oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
            "{hmac_config_11i_add_key::invalid uc_key_index[%d].}", uc_key_index);
        return OAL_ERR_CODE_SECURITY_KEY_ID;
    }

    oam_info_log2(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
                  "{hmac_config_11i_add_key::key_index=%d, pairwise=%d.}", uc_key_index, en_pairwise);
    oam_info_log3(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
                  "{hmac_config_11i_add_key::pst_params cipher=0x%08x, keylen=%d, seqlen=%d.}",
                  pst_key->cipher, pst_key->key_len, pst_key->seq_len);
    oam_info_log4(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
        "{hmac_config_11i_add_key::mac addr=%02X:XX:XX:%02X:%02X:%02X}",
        puc_mac_addr[0], puc_mac_addr[3], puc_mac_addr[4], puc_mac_addr[5]); // mac��ַ�ĵ�0 3 4 5�ֽ�

    if (en_pairwise == OAL_TRUE) {
        /* ������Կ����ڵ����û��� */
        ul_ret = mac_vap_find_user_by_macaddr(pst_mac_vap, puc_mac_addr, &us_user_idx);
        if (ul_ret != OAL_SUCC) {
            // ����ɾ�û���hostapdɾ�û���ʱ�����޷���֤ԭ�ӹ��̣����ܳ��ֶ���ͬʱɾ��������
            oam_warning_log2(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
                "{hmac_config_11i_add_key::us_user_idx[%d] find_user_by_macaddr fail[%d].}", us_user_idx, ul_ret);
            return ul_ret;
        }
    } else {
        /* �鲥��Կ������鲥�û��� */
        us_user_idx = pst_mac_vap->us_multi_user_idx;
    }

    pst_hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(us_user_idx);
    if (pst_hmac_user == OAL_PTR_NULL) {
        oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
            "{hmac_config_11i_add_key::get_mac_user null.idx:%u}", us_user_idx);
        return OAL_ERR_CODE_SECURITY_USER_INVAILD;
    }

#ifdef _PRE_WLAN_FEATURE_WAPI
    /* 11i������£��ص�wapi�˿� */
    hmac_wapi_reset_port(&pst_hmac_user->st_wapi);

    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (pst_mac_device == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_config_11i_add_key::pst_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    pst_mac_device->uc_wapi = OAL_FALSE;
#endif

    /* 3.1 ���������Ը��µ��û��� */
    ul_ret = mac_vap_add_key(pst_mac_vap, &pst_hmac_user->st_user_base_info, uc_key_index, pst_key);
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
            "{hmac_config_11i_add_key::mac_11i_add_key fail[%d].}", ul_ret);
        return ul_ret;
    }

    if (en_pairwise == OAL_TRUE) {
        mac_user_set_key(&pst_hmac_user->st_user_base_info, WLAN_KEY_TYPE_PTK, pst_key->cipher, uc_key_index);
    } else {
        mac_user_set_key(&pst_hmac_user->st_user_base_info, WLAN_KEY_TYPE_RX_GTK, pst_key->cipher, uc_key_index);
    }
    /* �鲥��Կ���º�Ӧ����������û��¼�¼���鲥֡PN�ţ��������鲥�û��¼�¼���鲥֡PN�� */
    hmac_user_clear_rx_pn(pst_mac_vap, pst_hmac_user, en_pairwise);

    /* �����û�8021x�˿ںϷ��Ե�״̬Ϊ�Ϸ� */
    mac_user_set_port(&pst_hmac_user->st_user_base_info, OAL_TRUE);

    if ((pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_STA) &&
        (pst_mac_vap->en_vap_state == MAC_VAP_STATE_ROAMING) && (en_pairwise == OAL_FALSE)) {
        hmac_roam_add_key_done(pst_hmac_vap);
    }
    /***************************************************************************
    ���¼���DMAC��, ͬ��DMAC����
    ***************************************************************************/
    ul_ret = hmac_config_send_event(pst_mac_vap, WLAN_CFGID_ADD_KEY, us_len, puc_param);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
                         "{hmac_config_11i_add_key::hmac_config_send_event failed[%d].}", ul_ret);
    }

    return ul_ret;
}


uint32_t hmac_config_11i_get_key(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    wlan_priv_key_param_stru *pst_priv_key = OAL_PTR_NULL;
    oal_key_params_stru st_key;
    uint8_t uc_key_index;
    uint16_t us_user_idx = g_wlan_spec_cfg->invalid_user_id;
    oal_bool_enum_uint8 en_pairwise;
    uint8_t *puc_mac_addr = OAL_PTR_NULL;
    void *cookie = OAL_PTR_NULL;
    void (*callback)(void *, oal_key_params_stru *);
    mac_getkey_param_stru *pst_payload_getkey_params = OAL_PTR_NULL;

    callback = OAL_PTR_NULL;

    /* 1.1 ��μ�� */
    if (oal_any_null_ptr2(pst_mac_vap, puc_param)) {
        oam_error_log2(0, OAM_SF_WPA,
            "{hmac_config_11i_get_key::param null, pst_mac_vap=%x, puc_param=%x.}",
            (uintptr_t)pst_mac_vap, (uintptr_t)puc_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 2.1 ��ȡ���� */
    pst_payload_getkey_params = (mac_getkey_param_stru *)puc_param;
    uc_key_index = pst_payload_getkey_params->uc_key_index;
    en_pairwise = pst_payload_getkey_params->en_pairwise;
    puc_mac_addr = pst_payload_getkey_params->puc_mac_addr;
    cookie = pst_payload_getkey_params->cookie;
    callback = pst_payload_getkey_params->callback;

    /* 2.2 ����ֵ���ֵ��� */
    if (uc_key_index >= WLAN_NUM_TK + WLAN_NUM_IGTK) {
        oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
            "{hmac_config_11i_get_key::uc_key_index invalid[%d].}", uc_key_index);
        return OAL_ERR_CODE_SECURITY_KEY_ID;
    }

    /* 3.1 ��ȡ��Կ */
    pst_priv_key = hmac_get_key_info(pst_mac_vap, puc_mac_addr, en_pairwise, uc_key_index, &us_user_idx);
    if (pst_priv_key == OAL_PTR_NULL) {
        oam_error_log2(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
            "{hmac_config_11i_get_key::key is null.pairwise[%d], key_idx[%d]}",
            en_pairwise, uc_key_index);
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (pst_priv_key->ul_key_len == 0) {
        oam_info_log2(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
            "{hmac_config_11i_get_key::key len = 0.pairwise[%d], key_idx[%d]}",
            en_pairwise, uc_key_index);
        return OAL_ERR_CODE_SECURITY_KEY_LEN;
    }

    /* 4.1 ��Կ��ֵת�� */
    memset_s(&st_key, sizeof(st_key), 0, sizeof(st_key));
    st_key.key = pst_priv_key->auc_key;
    st_key.key_len = (int32_t)pst_priv_key->ul_key_len;
    st_key.seq = pst_priv_key->auc_seq;
    st_key.seq_len = (int32_t)pst_priv_key->ul_seq_len;
    st_key.cipher = pst_priv_key->ul_cipher;

    /* 5.1 ���ûص����� */
    if (callback != OAL_PTR_NULL) {
        callback(cookie, &st_key);
    }

    return OAL_SUCC;
}


uint32_t hmac_config_11i_remove_key(mac_vap_stru *pst_mac_vap,
    uint16_t us_len, uint8_t *puc_param)
{
    wlan_priv_key_param_stru *pst_priv_key = OAL_PTR_NULL;
    uint32_t ul_ret;
    uint8_t uc_key_index;
    uint16_t us_user_idx = g_wlan_spec_cfg->invalid_user_id;
    oal_bool_enum_uint8 en_pairwise;
    uint8_t *puc_mac_addr = OAL_PTR_NULL;
    mac_removekey_param_stru *pst_payload_removekey_params = OAL_PTR_NULL;
    wlan_cfgid_enum_uint16 en_cfgid;
    mac_user_stru *pst_mac_user = OAL_PTR_NULL;
    oal_bool_enum_uint8 en_macaddr_is_zero;

    /* 1.1 ��μ�� */
    if (oal_any_null_ptr2(pst_mac_vap, puc_param)) {
        oam_error_log2(0, OAM_SF_WPA, "{hmac_config_11i_remove_key::param null,pst_mac_vap=%x, puc_param=%x.}",
            (uintptr_t)pst_mac_vap, (uintptr_t)puc_param);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 2.1 ��ȡ���� */
    pst_payload_removekey_params = (mac_removekey_param_stru *)puc_param;
    uc_key_index = pst_payload_removekey_params->uc_key_index;
    en_pairwise = pst_payload_removekey_params->en_pairwise;
    puc_mac_addr = pst_payload_removekey_params->auc_mac_addr;

    oam_info_log2(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
        "{hmac_config_11i_remove_key::uc_key_index=%d, en_pairwise=%d.}",
        uc_key_index, en_pairwise);

    /* 2.2 ����ֵ���ֵ��� */
    if (uc_key_index >= WLAN_NUM_TK + WLAN_NUM_IGTK) {
        /* �ں˻��·�ɾ��6 ���鲥��Կ����������6���鲥��Կ����ռ� */
        /* ���ڼ�⵽key idx > �����Կ������������ */
        oam_info_log1(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
            "{hmac_config_11i_remove_key::invalid uc_key_index[%d].}", uc_key_index);
        return OAL_SUCC;
    }

    /* 3.1 ��ȡ������Կ��Ϣ */
    pst_priv_key = hmac_get_key_info(pst_mac_vap, puc_mac_addr, en_pairwise, uc_key_index, &us_user_idx);
    if (pst_priv_key == OAL_PTR_NULL) {
        
        if (us_user_idx == g_wlan_spec_cfg->invalid_user_id) {
            oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
                "{hmac_config_11i_remove_key::user already del.}");
            return OAL_SUCC;
        } else {
            oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
                "{hmac_config_11i_remove_key::pst_priv_key null.}");
            return OAL_ERR_CODE_SECURITY_USER_INVAILD;
        }
    }

    if (pst_priv_key->ul_key_len == 0) {
        /* �����⵽��Կû��ʹ�ã� ��ֱ�ӷ�����ȷ */
        oam_info_log0(pst_mac_vap->uc_vap_id, OAM_SF_WPA, "{hmac_config_11i_remove_key::ul_key_len=0.}");
        return OAL_SUCC;
    }

    /* 4.1 ������wep����wpa */
    if ((WLAN_CIPHER_SUITE_WEP40 == pst_priv_key->ul_cipher) ||
        (WLAN_CIPHER_SUITE_WEP104 == pst_priv_key->ul_cipher)) {
        mac_mib_set_wep(pst_mac_vap, uc_key_index, WLAN_WEP_40_KEY_SIZE);
        en_cfgid = WLAN_CFGID_REMOVE_WEP_KEY;
    } else {
        en_macaddr_is_zero = mac_addr_is_zero(puc_mac_addr);
        if (MAC_11I_IS_PTK(en_macaddr_is_zero, en_pairwise)) {
            pst_mac_user = mac_vap_get_user_by_addr(pst_mac_vap, puc_mac_addr);
            if (pst_mac_user == OAL_PTR_NULL) {
                return OAL_ERR_CODE_SECURITY_USER_INVAILD;
            }
            pst_mac_user->st_user_tx_info.st_security.en_cipher_key_type = HAL_KEY_TYPE_BUTT;
        } else {
            pst_mac_user = (mac_user_stru *)mac_res_get_mac_user(pst_mac_vap->us_multi_user_idx);
            if (pst_mac_user == OAL_PTR_NULL) {
                return OAL_ERR_CODE_SECURITY_USER_INVAILD;
            }
            mac_user_init_key(pst_mac_user);
        }
        en_cfgid = WLAN_CFGID_REMOVE_KEY;

        mac_user_set_port(pst_mac_user, OAL_FALSE);

        if (pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_STA &&
            pst_mac_vap->en_vap_state != MAC_VAP_STATE_STA_FAKE_UP) {
            mac_user_set_pmf_active(pst_mac_user, pst_mac_vap->en_user_pmf_cap);
        }
    }

    /* 4.2 ���¼���dmac�㴦�� */
    ul_ret = hmac_config_send_event(pst_mac_vap, en_cfgid, us_len, puc_param);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_error_log2(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
            "{hmac_config_11i_remove_key::hmac_config_send_event failed[%d], en_cfgid=%d .}",
            ul_ret, en_cfgid);
        return ul_ret;
    }

    /* 5.1 ɾ����Կ�ɹ���������Կ����Ϊ0 */
    pst_priv_key->ul_key_len = 0;

    return ul_ret;
}


uint32_t hmac_config_11i_set_default_key(mac_vap_stru *pst_mac_vap,
    uint16_t us_len, uint8_t *puc_param)
{
    uint32_t ul_ret;
    uint8_t uc_key_index;
    oal_bool_enum_uint8 en_unicast;
    oal_bool_enum_uint8 en_multicast;
    mac_setdefaultkey_param_stru *pst_payload_setdefaultkey_params = OAL_PTR_NULL;

    /* 1.1 ��μ�� */
    if (oal_any_null_ptr2(pst_mac_vap, puc_param)) {
        oam_error_log0(0, OAM_SF_WPA, "{hmac_config_11i_set_default_key::param null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 2.1 ��ȡ���� */
    pst_payload_setdefaultkey_params = (mac_setdefaultkey_param_stru *)puc_param;
    uc_key_index = pst_payload_setdefaultkey_params->uc_key_index;
    en_unicast = pst_payload_setdefaultkey_params->en_unicast;
    en_multicast = pst_payload_setdefaultkey_params->en_multicast;

    /* 2.2 ����ֵ���ֵ��� */
    if (uc_key_index >= (WLAN_NUM_TK + WLAN_NUM_IGTK)) {
        oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
            "{hmac_config_11i_set_default_key::invalid uc_key_index[%d].}",
            uc_key_index);
        return OAL_ERR_CODE_SECURITY_KEY_ID;
    }

    /* 2.3 ������Ч�Լ�� */
    if ((en_multicast == OAL_FALSE) && (en_unicast == OAL_FALSE)) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
            "{hmac_config_11i_set_default_key::not ptk or gtk,invalid mode.}");
        return OAL_ERR_CODE_SECURITY_PARAMETERS;
    }

    if (uc_key_index >= WLAN_NUM_TK) {
        /* 3.1 ����default mgmt key���� */
        ul_ret = mac_vap_set_default_mgmt_key(pst_mac_vap, uc_key_index);
    } else {
        ul_ret = mac_vap_set_default_wep_key(pst_mac_vap, uc_key_index);
    }

    if (ul_ret != OAL_SUCC) {
        oam_error_log2(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
                       "{hmac_config_11i_set_default_key::set key[%d] failed[%d].}", uc_key_index, ul_ret);
        return ul_ret;
    }

    /***************************************************************************
    ���¼���DMAC��, ͬ��DMAC����
    ***************************************************************************/
    ul_ret = hmac_config_send_event(pst_mac_vap, WLAN_CFGID_DEFAULT_KEY, us_len, puc_param);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
                       "{hmac_config_11i_set_default_key::hmac_config_send_event failed[%d].}", ul_ret);
    }
    oam_info_log3(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
        "{hmac_config_11i_set_default_key::key_id[%d] un[%d] mu[%d] OK}",
        uc_key_index, en_unicast, en_multicast);

    return ul_ret;
}


uint32_t hmac_config_11i_add_wep_entry(mac_vap_stru *pst_mac_vap,
    uint16_t us_len, uint8_t *puc_param)
{
    mac_user_stru *pst_mac_user = OAL_PTR_NULL;
    uint32_t ul_ret;

    if (oal_any_null_ptr2(pst_mac_vap, puc_param)) {
        oam_error_log0(0, OAM_SF_WPA, "{hmac_config_11i_add_wep_entry::PARMA NULL}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_mac_user = (mac_user_stru *)mac_vap_get_user_by_addr(pst_mac_vap, puc_param);
    if (pst_mac_user == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_WPA, "{hmac_config_11i_add_wep_entry::mac_user NULL}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    ul_ret = mac_user_update_wep_key(pst_mac_user, pst_mac_vap->us_multi_user_idx);
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
                       "{hmac_config_11i_add_wep_entry::mac_wep_add_usr_key failed[%d].}", ul_ret);
        return ul_ret;
    }
    /***************************************************************************
    ���¼���DMAC��, ͬ��DMAC����
    ***************************************************************************/
    ul_ret = hmac_config_send_event(pst_mac_vap, WLAN_CFGID_ADD_WEP_ENTRY, us_len, puc_param);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
                         "{hmac_config_11i_add_wep_entry::hmac_config_send_event failed[%d].}", ul_ret);
    }

    /* �����û��ķ��ͼ����׼� */
    oam_info_log1(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
                  "{hmac_config_11i_add_wep_entry:: usridx[%d] OK.}", pst_mac_user->us_assoc_id);

    return ul_ret;
}


uint32_t hmac_init_security(mac_vap_stru *pst_mac_vap, uint8_t *puc_addr, uint8_t uc_mac_len)
{
    uint32_t ul_ret = OAL_SUCC;
    uint16_t us_len;
    uint8_t *puc_param = OAL_PTR_NULL;

    if (pst_mac_vap == OAL_PTR_NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (uc_mac_len != WLAN_MAC_ADDR_LEN) {
        return OAL_FAIL;
    }

    if (OAL_TRUE == mac_is_wep_enabled(pst_mac_vap)) {
        puc_param = puc_addr;
        us_len = WLAN_MAC_ADDR_LEN;
        ul_ret = hmac_config_11i_add_wep_entry(pst_mac_vap, us_len, puc_param);
    }
    return ul_ret;
}


uint32_t hmac_check_capability_mac_phy_supplicant(mac_vap_stru *pst_mac_vap,
                                                        mac_bss_dscr_stru *pst_bss_dscr)
{
    uint32_t ul_ret;

    if (oal_any_null_ptr2(pst_mac_vap, pst_bss_dscr)) {
        oam_warning_log2(0, OAM_SF_WPA, "{hmac_check_capability_mac_phy_supplicant::input null %x %x.}",
            (uintptr_t)pst_mac_vap, (uintptr_t)pst_bss_dscr);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ����Э��ģʽ���³�ʼ��STA HT/VHT mibֵ */
    mac_vap_config_vht_ht_mib_by_protocol(pst_mac_vap);

    ul_ret = hmac_check_bss_cap_info(pst_bss_dscr->us_cap_info, pst_mac_vap);
    if (ul_ret != OAL_TRUE) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
            "{hmac_check_capability_mac_phy_supplicant::hmac_check_bss_cap_info failed[%d].}", ul_ret);
    }

    /* check bss capability info PHY,����PHY������ƥ���AP */
    mac_vap_check_bss_cap_info_phy_ap(pst_bss_dscr->us_cap_info, pst_mac_vap);

    return OAL_SUCC;
}


uint32_t hmac_sta_protocol_down_by_chipher(mac_vap_stru *pst_mac_vap, mac_bss_dscr_stru *pst_bss_dscr)
{
    mac_cfg_mode_param_stru st_cfg_mode;
    uint32_t aul_pair_suite[WLAN_PAIRWISE_CIPHER_SUITES] = { 0 };
    oal_bool_enum_uint8 en_legcy_only = OAL_FALSE;

    if (pst_mac_vap->en_protocol >= WLAN_HT_MODE) {
        /* ��WEP / TKIP ����ģʽ�£����ܹ�����HT MODE */
        if (OAL_TRUE == mac_mib_get_privacyinvoked(pst_mac_vap) &&
            OAL_FALSE == mac_mib_get_rsnaactivated(pst_mac_vap)) {
            en_legcy_only = OAL_TRUE;
        }

        if (pst_mac_vap->st_cap_flag.bit_wpa == OAL_TRUE) {
            aul_pair_suite[0] = MAC_WPA_CHIPER_TKIP;

            if (0 != mac_mib_wpa_pair_match_suites_s(pst_mac_vap, aul_pair_suite, sizeof(aul_pair_suite))) {
                en_legcy_only = OAL_TRUE;
            }
        }

        if (pst_mac_vap->st_cap_flag.bit_wpa2 == OAL_TRUE) {
            aul_pair_suite[0] = MAC_RSN_CHIPER_TKIP;

            if (0 != mac_mib_rsn_pair_match_suites_s(pst_mac_vap, aul_pair_suite, sizeof(aul_pair_suite))) {
                en_legcy_only = OAL_TRUE;
            }
        }

#ifdef _PRE_WLAN_FEATURE_WAPI
        if (pst_bss_dscr->uc_wapi) {
            en_legcy_only = OAL_TRUE;
        }
#endif
    }

    oam_warning_log3(pst_mac_vap->uc_vap_id, OAM_SF_WPA,
        "hmac_sta_protocol_down_by_chipher::legacy_only[%d],wpa[%d]wpa2[%d]",
        en_legcy_only, pst_mac_vap->st_cap_flag.bit_wpa, pst_mac_vap->st_cap_flag.bit_wpa2);

    st_cfg_mode.en_protocol = pst_mac_vap->en_protocol;

    if (en_legcy_only == OAL_TRUE) {
        if (WLAN_BAND_2G == pst_mac_vap->st_channel.en_band) {
            st_cfg_mode.en_protocol = WLAN_MIXED_ONE_11G_MODE;
            pst_mac_vap->st_channel.en_bandwidth = WLAN_BAND_WIDTH_20M;
        }
        if (WLAN_BAND_5G == pst_mac_vap->st_channel.en_band) {
            st_cfg_mode.en_protocol = WLAN_LEGACY_11A_MODE;
            pst_mac_vap->st_channel.en_bandwidth = WLAN_BAND_WIDTH_20M;
        }
    }

    if (st_cfg_mode.en_protocol >= WLAN_HT_MODE) {
        mac_mib_set_TxAggregateActived(pst_mac_vap, OAL_TRUE);
        mac_mib_set_AmsduAggregateAtive(pst_mac_vap, OAL_TRUE);
    } else {
        mac_mib_set_TxAggregateActived(pst_mac_vap, OAL_FALSE);
        mac_mib_set_AmsduAggregateAtive(pst_mac_vap, OAL_FALSE);
    }

    mac_vap_init_by_protocol(pst_mac_vap, st_cfg_mode.en_protocol);

    return OAL_SUCC;
}


uint32_t hmac_en_mic(hmac_vap_stru *pst_hmac_vap, hmac_user_stru *pst_hmac_user,
    oal_netbuf_stru *pst_netbuf, uint8_t *puc_iv_len)
{
    wlan_priv_key_param_stru *pst_key = OAL_PTR_NULL;
    uint32_t ul_ret;
    wlan_ciper_protocol_type_enum_uint8 en_cipher_type;
    wlan_cipher_key_type_enum_uint8 en_key_type;

    /* 1.1 ��μ�� */
    if (oal_any_null_ptr4(pst_hmac_vap, pst_hmac_user, pst_netbuf, puc_iv_len)) {
        oam_error_log4(0, OAM_SF_WPA, "{hmac_en_mic::hmac_vap=%d hmac_user=%d netbuf=%d iv_len=%d.}",
            (uintptr_t)pst_hmac_vap, (uintptr_t)pst_hmac_user, (uintptr_t)pst_netbuf, (uintptr_t)puc_iv_len);
        return OAL_ERR_CODE_PTR_NULL;
    }

    *puc_iv_len = 0;
    en_key_type = pst_hmac_user->st_user_base_info.st_user_tx_info.st_security.en_cipher_key_type;
    en_cipher_type = pst_hmac_user->st_user_base_info.st_key_info.en_cipher_type;
    pst_key = mac_user_get_key(&pst_hmac_user->st_user_base_info,
        pst_hmac_user->st_user_base_info.st_key_info.uc_default_index);
    if (pst_key == OAL_PTR_NULL) {
        oam_error_log1(0, OAM_SF_WPA, "{hmac_en_mic::mac_user_get_key FAIL. en_key_type[%d]}", en_key_type);
        return OAL_ERR_CODE_SECURITY_KEY_ID;
    }

    switch (en_cipher_type) {
        case WLAN_80211_CIPHER_SUITE_TKIP:
            if (en_key_type == 0 || en_key_type > 5) { // 5������Ч����Կ����
                return OAL_ERR_CODE_SECURITY_KEY_TYPE;
            }
            ul_ret = hmac_crypto_tkip_enmic(pst_key, pst_netbuf);
            if (ul_ret != OAL_SUCC) {
                oam_error_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_WPA,
                               "{hmac_en_mic::hmac_crypto_tkip_enmic failed[%d].}", ul_ret);
                return ul_ret;
            }
            *puc_iv_len = WEP_IV_FIELD_SIZE + EXT_IV_FIELD_SIZE;
            break;
        case WLAN_80211_CIPHER_SUITE_CCMP:
            *puc_iv_len = WEP_IV_FIELD_SIZE + EXT_IV_FIELD_SIZE;
            break;
        default:
            break;
    }

    return OAL_SUCC;
}


uint32_t hmac_de_mic(hmac_user_stru *pst_hmac_user, oal_netbuf_stru *pst_netbuf)
{
    wlan_priv_key_param_stru *pst_key = OAL_PTR_NULL;
    uint32_t ul_ret;
    wlan_ciper_protocol_type_enum_uint8 en_cipher_type;
    wlan_cipher_key_type_enum_uint8 en_key_type;

    /* 1.1 ��μ�� */
    if (oal_any_null_ptr2(pst_hmac_user, pst_netbuf)) {
        oam_error_log0(0, OAM_SF_WPA, "{hmac_de_mic::param null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    en_key_type = pst_hmac_user->st_user_base_info.st_user_tx_info.st_security.en_cipher_key_type;
    en_cipher_type = pst_hmac_user->st_user_base_info.st_key_info.en_cipher_type;
    pst_key = mac_user_get_key(&pst_hmac_user->st_user_base_info,
        pst_hmac_user->st_user_base_info.st_key_info.uc_default_index);
    if (pst_key == OAL_PTR_NULL) {
        oam_error_log1(0, OAM_SF_WPA, "{hmac_de_mic::mac_user_get_key FAIL. en_key_type[%d]}", en_key_type);
        return OAL_ERR_CODE_SECURITY_KEY_ID;
    }

    switch (en_cipher_type) {
        case WLAN_80211_CIPHER_SUITE_TKIP:
            if (en_key_type == 0 || en_key_type > 5) { // ����5������Ч����Կ����
                return OAL_ERR_CODE_SECURITY_KEY_TYPE;
            }
            ul_ret = hmac_crypto_tkip_demic(pst_key, pst_netbuf);
            if (ul_ret != OAL_SUCC) {
                oam_error_log1(pst_hmac_user->st_user_base_info.uc_vap_id, OAM_SF_WPA,
                               "{hmac_de_mic::hmac_crypto_tkip_demic failed[%d].}", ul_ret);
                return ul_ret;
            }
            break;
        default:
            break;
    }

    return OAL_SUCC;
}


uint32_t hmac_rx_tkip_mic_failure_process(frw_event_mem_stru *pst_event_mem)
{
    frw_event_stru *pst_event = OAL_PTR_NULL;
    frw_event_mem_stru *pst_hmac_event_mem = OAL_PTR_NULL;
    frw_event_hdr_stru *pst_event_hdr = OAL_PTR_NULL;
    dmac_to_hmac_mic_event_stru *pst_mic_event = OAL_PTR_NULL;

    if (pst_event_mem == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_WPA, "{hmac_rx_tkip_mic_failure_process::pst_event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ��ȡ�¼�ͷ���¼��ṹ��ָ�� */
    pst_event = frw_get_event_stru(pst_event_mem);
    pst_event_hdr = &(pst_event->st_event_hdr);
    pst_mic_event = (dmac_to_hmac_mic_event_stru *)&(pst_event->auc_event_data);

    /* ��mic�¼��׵�WAL */
    pst_hmac_event_mem = frw_event_alloc_m(OAL_SIZEOF(dmac_to_hmac_mic_event_stru));
    if (pst_hmac_event_mem == OAL_PTR_NULL) {
        oam_error_log0(pst_event_hdr->uc_vap_id, OAM_SF_WPA,
            "{hmac_rx_tkip_mic_failure_process::pst_hmac_event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ��д�¼� */
    pst_event = frw_get_event_stru(pst_hmac_event_mem);

    frw_event_hdr_init(&(pst_event->st_event_hdr),
                       FRW_EVENT_TYPE_HOST_CTX,
                       HMAC_HOST_CTX_EVENT_SUB_TYPE_MIC_FAILURE,
                       OAL_SIZEOF(dmac_to_hmac_mic_event_stru),
                       FRW_EVENT_PIPELINE_STAGE_0,
                       pst_event_hdr->uc_chip_id,
                       pst_event_hdr->uc_device_id,
                       pst_event_hdr->uc_vap_id);

    /* ȥ������STA mac��ַ */
    if (EOK != memcpy_s((uint8_t *)frw_get_event_payload(pst_hmac_event_mem), sizeof(dmac_to_hmac_mic_event_stru),
                        (uint8_t *)pst_mic_event, sizeof(dmac_to_hmac_mic_event_stru))) {
        oam_error_log0(0, OAM_SF_WPA, "hmac_rx_tkip_mic_failure_process::memcpy fail!");
        frw_event_free_m(pst_hmac_event_mem);
        return OAL_FAIL;
    }

    /* �ַ��¼� */
    frw_event_dispatch_event(pst_hmac_event_mem);
    frw_event_free_m(pst_hmac_event_mem);
    return OAL_SUCC;
}


uint32_t hmac_11i_ether_type_filter(hmac_vap_stru *pst_vap,
    hmac_user_stru *pst_hmac_user, uint16_t us_ether_type)
{
    if (pst_hmac_user->st_user_base_info.en_port_valid == OAL_FALSE) { /* �ж϶˿��Ƿ�� */
        /* ��������ʱ����Է�EAPOL ������֡������ */
        if (oal_byteorder_host_to_net_uint16(ETHER_TYPE_PAE) != us_ether_type) {
            oam_warning_log1(pst_vap->st_vap_base_info.uc_vap_id, OAM_SF_WPA,
                             "{hmac_11i_ether_type_filter::TYPE 0x%04x not permission.}", us_ether_type);
            return OAL_ERR_CODE_SECURITY_PORT_INVALID;
        }
    }

    return OAL_SUCC;
} /*lint -e578*/ /*lint -e19*/
oal_module_symbol(hmac_config_11i_set_default_key);
oal_module_symbol(hmac_config_11i_remove_key);
oal_module_symbol(hmac_config_11i_get_key);
oal_module_symbol(hmac_config_11i_add_key);
oal_module_symbol(hmac_config_11i_add_wep_entry); /*lint +e578*/ /*lint +e19*/

