

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 1 头文件包含 */
#include "wlan_spec.h"
#include "mac_vap.h"
#include "mac_frame.h"
#include "hmac_mgmt_ap.h"
#include "hmac_encap_frame_ap.h"
#include "hmac_mgmt_bss_comm.h"
#include "hmac_rx_data.h"
#include "hmac_uapsd.h"
#include "securec.h"
#include "securectype.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_UAPSD_C

/* 2 全局变量定义 */
/* 3 函数实现 */

void hmac_uapsd_update_user_para(uint8_t *puc_payload,
                                         uint8_t uc_sub_type,
                                         uint32_t ul_msg_len,
                                         hmac_user_stru *pst_hmac_user)
{
    uint32_t ul_ret;
    uint8_t uc_max_sp;
    uint8_t uc_found_wmm = OAL_FALSE;
    uint8_t uc_en = OAL_FALSE;
    uint8_t uc_uapsd_flag = 0;
    mac_user_uapsd_status_stru st_uapsd_status;
    mac_vap_stru *pst_mac_vap = NULL;
    uint16_t us_len;
    uint16_t us_len_total;
    uint8_t *puc_param = NULL;
    uint8_t *puc_wmm_ie = OAL_PTR_NULL;
    int32_t l_ret;

    puc_wmm_ie = mac_get_wmm_ie(puc_payload, (uint16_t)ul_msg_len);
    if (puc_wmm_ie != OAL_PTR_NULL) {
        uc_found_wmm = OAL_TRUE;
    }

    /* 不存在WMM IE,直接返回 */
    if ((uc_found_wmm == OAL_FALSE) || (puc_wmm_ie == OAL_PTR_NULL)) {
        oam_warning_log1(pst_hmac_user->st_user_base_info.uc_vap_id, OAM_SF_PWR,
                         "Could not find WMM IE in assoc req,user_id[%d]\n",
                         pst_hmac_user->st_user_base_info.us_assoc_id);
        return;
    }

    memset_s(&st_uapsd_status, OAL_SIZEOF(mac_user_uapsd_status_stru), 0, OAL_SIZEOF(mac_user_uapsd_status_stru));
    st_uapsd_status.uc_qos_info = puc_wmm_ie[HMAC_UAPSD_WME_LEN];

    /* 8为WMM IE长度 */
    if ((puc_wmm_ie[HMAC_UAPSD_WME_LEN] & BIT0) == BIT0) {
        st_uapsd_status.uc_ac_trigger_ena[WLAN_WME_AC_VO] = 1;
        st_uapsd_status.uc_ac_delievy_ena[WLAN_WME_AC_VO] = 1;
        uc_en = OAL_TRUE;
    }

    if ((puc_wmm_ie[HMAC_UAPSD_WME_LEN] & BIT1) == BIT1) {
        st_uapsd_status.uc_ac_trigger_ena[WLAN_WME_AC_VI] = 1;
        st_uapsd_status.uc_ac_delievy_ena[WLAN_WME_AC_VI] = 1;
        uc_en = OAL_TRUE;
    }

    if ((puc_wmm_ie[HMAC_UAPSD_WME_LEN] & BIT2) == BIT2) {
        st_uapsd_status.uc_ac_trigger_ena[WLAN_WME_AC_BK] = 1;
        st_uapsd_status.uc_ac_delievy_ena[WLAN_WME_AC_BK] = 1;
        uc_en = OAL_TRUE;
    }

    if ((puc_wmm_ie[HMAC_UAPSD_WME_LEN] & BIT3) == BIT3) {
        st_uapsd_status.uc_ac_trigger_ena[WLAN_WME_AC_BE] = 1;
        st_uapsd_status.uc_ac_delievy_ena[WLAN_WME_AC_BE] = 1;
        uc_en = OAL_TRUE;
    }

    if (uc_en == OAL_TRUE) {
        uc_uapsd_flag |= MAC_USR_UAPSD_EN;
    }

    /* 设置max SP长度 */
    uc_max_sp = (puc_wmm_ie[HMAC_UAPSD_WME_LEN] >> 5) & 0x3;

    st_uapsd_status.uc_max_sp_len = (oal_value_not_in_valid_range(uc_max_sp, 1, 3)) ?
        HMAC_UAPSD_SEND_ALL : uc_max_sp * 2;

    /* Send uapsd_flag & uapsd_status syn to dmac */
    pst_mac_vap = (mac_vap_stru *)mac_res_get_mac_vap(pst_hmac_user->st_user_base_info.uc_vap_id);
    if (oal_unlikely(pst_mac_vap == OAL_PTR_NULL)) {
        oam_error_log1(0, OAM_SF_CFG,
                       "{hmac_uapsd_update_user_para::get mac_vap [%d] null.}",
                       pst_hmac_user->st_user_base_info.uc_vap_id);
        return;
    }
    us_len = OAL_SIZEOF(uint16_t);
    us_len_total = OAL_SIZEOF(uint16_t) + OAL_SIZEOF(uint8_t) + OAL_SIZEOF(mac_user_uapsd_status_stru);
    puc_param = (uint8_t *)oal_mem_alloc_m(OAL_MEM_POOL_ID_LOCAL, us_len_total, OAL_TRUE);
    if (oal_unlikely(puc_param == OAL_PTR_NULL)) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_uapsd_update_user_para::puc_param null.}");
        return;
    }
    // uc_user_index
    l_ret = memcpy_s(puc_param, us_len_total, &pst_hmac_user->st_user_base_info.us_assoc_id, us_len);
    // uc_uapsd_flag
    l_ret += memcpy_s(puc_param + us_len, us_len_total - us_len, &uc_uapsd_flag, OAL_SIZEOF(uint8_t));
    us_len += OAL_SIZEOF(uint8_t);
    // st_uapsd_status
    l_ret += memcpy_s(puc_param + us_len, us_len_total - us_len,
                      &st_uapsd_status, OAL_SIZEOF(mac_user_uapsd_status_stru));
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_CFG, "hmac_uapsd_update_user_para::memcpy fail!");
        oal_mem_free_m(puc_param, OAL_TRUE);
        return;
    }

    us_len += OAL_SIZEOF(mac_user_uapsd_status_stru);

    ul_ret = hmac_config_send_event(pst_mac_vap, WLAN_CFGID_UAPSD_UPDATE, us_len, puc_param);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_CFG,
                         "{hmac_uapsd_update_user_para::hmac_config_send_event fail. erro code is %u}", ul_ret);
    }
    oal_mem_free_m(puc_param, OAL_TRUE);

    return;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

