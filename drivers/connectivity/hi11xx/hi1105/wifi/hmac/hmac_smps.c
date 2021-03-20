

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef _PRE_WLAN_FEATURE_SMPS

/* 1 头文件包含 */
#include "hmac_config.h"
#include "hmac_smps.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_SMPS_C

/* 2 全局变量定义 */
/* 3 函数实现 */

uint32_t hmac_smps_update_user_status(mac_vap_stru *pst_mac_vap, mac_user_stru *pst_mac_user)
{
    wlan_mib_mimo_power_save_enum_uint8 en_user_smps_mode;
    wlan_nss_enum_uint8 en_avail_num_spatial_stream;

    if (oal_any_null_ptr2(pst_mac_vap, pst_mac_user)) {
        oam_error_log2(0, OAM_SF_SMPS,
                       "{hmac_smps_update_user_status: NULL PTR pst_mac_vap is [%x] and pst_mac_user is [%x].}",
                       (uintptr_t)pst_mac_vap, (uintptr_t)pst_mac_user);
        return OAL_ERR_CODE_PTR_NULL;
    }

    en_user_smps_mode = (wlan_mib_mimo_power_save_enum_uint8)pst_mac_user->st_ht_hdl.bit_sm_power_save;

    switch (en_user_smps_mode) {
        case WLAN_MIB_MIMO_POWER_SAVE_STATIC:
            mac_user_set_sm_power_save(pst_mac_user, WLAN_MIB_MIMO_POWER_SAVE_STATIC);
            en_avail_num_spatial_stream = WLAN_SINGLE_NSS;
            oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_SMPS,
                             "{hmac_smps_update_user_status:user smps_mode update STATIC!}");
            break;
        case WLAN_MIB_MIMO_POWER_SAVE_DYNAMIC:
            oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_SMPS,
                             "{hmac_smps_update_user_status:user smps_mode update DYNAMIC!}");
            mac_user_set_sm_power_save(pst_mac_user, WLAN_MIB_MIMO_POWER_SAVE_DYNAMIC);
            en_avail_num_spatial_stream = oal_min(pst_mac_vap->en_vap_rx_nss, WLAN_DOUBLE_NSS);
            break;
        case WLAN_MIB_MIMO_POWER_SAVE_MIMO:
            mac_user_set_sm_power_save(pst_mac_user, WLAN_MIB_MIMO_POWER_SAVE_MIMO);
            en_avail_num_spatial_stream = oal_min(pst_mac_vap->en_vap_rx_nss, WLAN_DOUBLE_NSS);
            break;
        default:
            oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_SMPS,
                             "{hmac_smps_update_user_status: en_user_smps_mode mode[%d] fail!}",
                             en_user_smps_mode);
            return OAL_FAIL;
    }

    mac_user_set_avail_num_spatial_stream(pst_mac_user, en_avail_num_spatial_stream);
    mac_user_set_smps_opmode_notify_nss(pst_mac_user, mac_user_get_avail_num_spatial_stream(pst_mac_user));

    return OAL_SUCC;
}


uint32_t hmac_mgmt_rx_smps_frame(mac_vap_stru *pst_mac_vap, hmac_user_stru *pst_hmac_user, uint8_t *puc_data)
{
    wlan_mib_mimo_power_save_enum_uint8 en_user_smps_mode;

    if (oal_any_null_ptr3(pst_mac_vap, pst_hmac_user, puc_data)) {
        oam_error_log3(0, OAM_SF_SMPS,
                       "{hmac_mgmt_rx_smps_frame::null param, 0x%x 0x%x 0x%x.}",
                       (uintptr_t)pst_mac_vap, (uintptr_t)pst_hmac_user, (uintptr_t)puc_data);
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (OAL_FALSE == mac_mib_get_HighThroughputOptionImplemented(pst_mac_vap)) {
        return OAL_SUCC;
    }

    /* 更新STA的sm_power_save field, 获取enable bit */
    if ((puc_data[MAC_ACTION_OFFSET_ACTION + 1] & BIT0) == 0) {
        en_user_smps_mode = WLAN_MIB_MIMO_POWER_SAVE_MIMO;
    } else {
        /* 如果SMPS enable,则配置为相应模式(不考虑动态状态更新，动态只支持配置命令配置) */
        if ((puc_data[MAC_ACTION_OFFSET_ACTION + 1] & BIT1) == 0) {
            /* 静态SMPS */
            en_user_smps_mode = WLAN_MIB_MIMO_POWER_SAVE_STATIC;
        } else { /* 动态SMPS */
            en_user_smps_mode = WLAN_MIB_MIMO_POWER_SAVE_DYNAMIC;
        }
    }

    oam_warning_log3(pst_mac_vap->uc_vap_id, OAM_SF_SMPS,
                     "{hmac_ap_up_rx_smps_frame::user[%d] smps mode[%d] change to[%d]!}",
                     pst_hmac_user->st_user_base_info.us_assoc_id,
                     pst_hmac_user->st_user_base_info.st_ht_hdl.bit_sm_power_save,
                     en_user_smps_mode);

    /* 用户更新的smps能力不能超过本vap的能力 */
    if (en_user_smps_mode > mac_mib_get_smps(pst_mac_vap)) {
        oam_warning_log3(pst_mac_vap->uc_vap_id, OAM_SF_SMPS,
                         "{hmac_mgmt_rx_smps_frame::user[%d] new smps mode[%d] beyond vap smps mode[%d]!}",
                         pst_hmac_user->st_user_base_info.us_assoc_id,
                         en_user_smps_mode,
                         mac_mib_get_smps(pst_mac_vap));
        return OAL_FAIL;
    }

    /* 如果user的SMPS状态发生改变，需要做user和vap状态更新 */
    if (en_user_smps_mode != pst_hmac_user->st_user_base_info.st_ht_hdl.bit_sm_power_save) {
        pst_hmac_user->st_user_base_info.st_ht_hdl.bit_sm_power_save = en_user_smps_mode;

        return hmac_smps_update_user_status(pst_mac_vap, &(pst_hmac_user->st_user_base_info));
    } else {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_SMPS,
                         "{hmac_mgmt_rx_smps_frame::user smps mode donot change!!!}");
        return OAL_SUCC;
    }
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

