

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "mac_frame.h"
#include "mac_vap.h"

#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_MAC_FRAME_RAM_C
/*****************************************************************************
  4 ����ʵ��
*****************************************************************************/

oal_void mac_set_rrm_enabled_cap_field_cb(oal_void *pst_vap, oal_uint8 *puc_buffer, oal_uint8 *puc_ie_len)
{
}


oal_void mac_set_vht_capabilities_ie_cb(oal_void *pst_vap, oal_uint8 *puc_buffer, oal_uint8 *puc_ie_len)
{
}

oal_void mac_set_ht_capabilities_ie_cb(oal_void *pst_vap, oal_uint8 *puc_buffer, oal_uint8 *puc_ie_len)
{
}

oal_void mac_set_ext_capabilities_ie_cb(oal_void *pst_vap, oal_uint8 *puc_buffer, oal_uint8 *puc_ie_len)
{
    mac_ext_cap_ie_stru     *pst_ext_cap = OAL_PTR_NULL;
    mac_vap_stru            *pst_mac_vap;

    pst_mac_vap = (mac_vap_stru *)pst_vap;

    if ((g_customize_interworking == OAL_FALSE) || (pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_AP)) {
        pst_ext_cap = (mac_ext_cap_ie_stru *)(puc_buffer + MAC_IE_HDR_LEN);
        pst_ext_cap->bit_interworking = 0;
    }
}


oal_rom_cb_result_enum_uint8 mac_set_rsn_ie_cb(
    oal_void *pst_vap, oal_uint8 *puc_pmkid, oal_uint8 *puc_buffer, oal_uint8 *puc_ie_len, oal_uint32 *pul_ret)
{
    return OAL_CONTINUE;
}


oal_rom_cb_result_enum_uint8 mac_set_wpa_ie_cb(
    oal_void *pst_vap, oal_uint8 *puc_buffer, oal_uint8 *puc_ie_len, oal_uint32 *pul_ret)
{
    return OAL_CONTINUE;
}


