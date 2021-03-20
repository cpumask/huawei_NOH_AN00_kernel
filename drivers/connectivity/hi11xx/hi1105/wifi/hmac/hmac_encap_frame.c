

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 1 头文件包含 */
#include "wlan_spec.h"
#include "mac_resource.h"
#include "hmac_encap_frame.h"
#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_ENCAP_FRAME_C
/* 2 全局变量定义 */
/* 3 函数实现 */

uint16_t hmac_encap_sa_query_req(mac_vap_stru *pst_mac_vap, uint8_t *puc_data,
                                       uint8_t *puc_da, uint16_t us_trans_id)
{
    uint16_t us_len;

    /*************************************************************************/
    /*                        Management Frame Format                        */
    /* --------------------------------------------------------------------  */
    /* |Frame Control|Duration|DA|SA|BSSID|Sequence Control|Frame Body|FCS|  */
    /* --------------------------------------------------------------------  */
    /* | 2           |2       |6 |6 |6    |2               |0 - 2312  |4  |  */
    /* --------------------------------------------------------------------  */
    /*                                                                       */
    /*************************************************************************/
    /*************************************************************************/
    /*                Set the fields in the frame header                     */
    /*************************************************************************/
    /* All the fields of the Frame Control Field are set to zero. Only the   */
    /* Type/Subtype field is set.                                            */
    mac_hdr_set_frame_control(puc_data, WLAN_FC0_SUBTYPE_ACTION);
    /*  Set DA  */
    oal_set_mac_addr(((mac_ieee80211_frame_stru *)puc_data)->auc_address1, puc_da);
    /*  Set SA  */
    oal_set_mac_addr(((mac_ieee80211_frame_stru *)puc_data)->auc_address2, mac_mib_get_StationID(pst_mac_vap));
    /*  Set SSID  */
    oal_set_mac_addr(((mac_ieee80211_frame_stru *)puc_data)->auc_address3, pst_mac_vap->auc_bssid);

    /*************************************************************************/
    /*                Set the contents of the frame body                     */
    /*************************************************************************/
    /*************************************************************************/
    /*                  SA Query Frame - Frame Body                          */
    /* --------------------------------------------------------------------- */
    /* |   Category   |SA Query Action |  Transaction Identifier           | */
    /* --------------------------------------------------------------------- */
    /* |1             |1               |2 Byte                             | */
    /* --------------------------------------------------------------------- */
    /*                                                                       */
    /*************************************************************************/
    puc_data[MAC_80211_FRAME_LEN] = MAC_ACTION_CATEGORY_SA_QUERY;
    puc_data[MAC_80211_FRAME_LEN + 1] = MAC_SA_QUERY_ACTION_REQUEST;
    puc_data[MAC_80211_FRAME_LEN + 2] = (us_trans_id & 0x00FF);
    puc_data[MAC_80211_FRAME_LEN + 3] = (us_trans_id & 0xFF00) >> 8;

    us_len = MAC_80211_FRAME_LEN + MAC_SA_QUERY_LEN;
    return us_len;
}


uint16_t hmac_encap_sa_query_rsp(mac_vap_stru *pst_mac_vap, uint8_t *pst_hdr, uint8_t *puc_data)
{
    uint16_t us_len;

    /*************************************************************************/
    /*                        Management Frame Format                        */
    /* --------------------------------------------------------------------  */
    /* |Frame Control|Duration|DA|SA|BSSID|Sequence Control|Frame Body|FCS|  */
    /* --------------------------------------------------------------------  */
    /* | 2           |2       |6 |6 |6    |2               |0 - 2312  |4  |  */
    /* --------------------------------------------------------------------  */
    /*                                                                       */
    /*************************************************************************/
    /*************************************************************************/
    /*                Set the fields in the frame header                     */
    /*************************************************************************/
    /* All the fields of the Frame Control Field are set to zero. Only the   */
    /* Type/Subtype field is set.                                            */
    mac_hdr_set_frame_control(puc_data, WLAN_FC0_SUBTYPE_ACTION);
    /* Set DA  */
    oal_set_mac_addr(((mac_ieee80211_frame_stru *)puc_data)->auc_address1,
                     ((mac_ieee80211_frame_stru *)pst_hdr)->auc_address2);
    /*  Set SA  */
    oal_set_mac_addr(((mac_ieee80211_frame_stru *)puc_data)->auc_address2, mac_mib_get_StationID(pst_mac_vap));
    /*  Set SSID  */
    oal_set_mac_addr(((mac_ieee80211_frame_stru *)puc_data)->auc_address3, pst_mac_vap->auc_bssid);
    /*************************************************************************/
    /*                Set the contents of the frame body                     */
    /*************************************************************************/
    /*************************************************************************/
    /*                  SA Query Frame - Frame Body                          */
    /* --------------------------------------------------------------------- */
    /* |   Category   |SA Query Action |  Transaction Identifier           | */
    /* --------------------------------------------------------------------- */
    /* |1             |1               |2 Byte                             | */
    /* --------------------------------------------------------------------- */
    /*                                                                       */
    /*************************************************************************/
    puc_data[MAC_80211_FRAME_LEN] = pst_hdr[MAC_80211_FRAME_LEN];
    puc_data[MAC_80211_FRAME_LEN + 1] = MAC_SA_QUERY_ACTION_RESPONSE;
    puc_data[MAC_80211_FRAME_LEN + 2] = pst_hdr[MAC_80211_FRAME_LEN + 2];
    puc_data[MAC_80211_FRAME_LEN + 3] = pst_hdr[MAC_80211_FRAME_LEN + 3];

    us_len = MAC_80211_FRAME_LEN + MAC_SA_QUERY_LEN;
    return us_len;
}


uint16_t hmac_mgmt_encap_deauth(mac_vap_stru *pst_mac_vap, uint8_t *puc_data,
                                      const unsigned char *puc_da, uint16_t us_err_code)
{
    uint16_t us_deauth_len = 0;
    mac_device_stru *pst_mac_device = OAL_PTR_NULL;
    mac_vap_stru *pst_up_vap1 = OAL_PTR_NULL;
    mac_vap_stru *pst_up_vap2 = OAL_PTR_NULL;
    uint32_t ul_ret;

    /*************************************************************************/
    /*                        Management Frame Format                        */
    /* --------------------------------------------------------------------  */
    /* |Frame Control|Duration|DA|SA|BSSID|Sequence Control|Frame Body|FCS|  */
    /* --------------------------------------------------------------------  */
    /* | 2           |2       |6 |6 |6    |2               |0 - 2312  |4  |  */
    /* --------------------------------------------------------------------  */
    /*                                                                       */
    /*************************************************************************/
    /*************************************************************************/
    /*                Set the fields in the frame header                     */
    /*************************************************************************/
    /* All the fields of the Frame Control Field are set to zero. Only the   */
    /* Type/Subtype field is set.                                            */
    mac_hdr_set_frame_control(puc_data, WLAN_FC0_SUBTYPE_DEAUTH);

    /* Set DA to address of unauthenticated STA */
    oal_set_mac_addr(((mac_ieee80211_frame_stru *)puc_data)->auc_address1, puc_da);

    if (us_err_code & MAC_SEND_TWO_DEAUTH_FLAG) {
        us_err_code = us_err_code & ~MAC_SEND_TWO_DEAUTH_FLAG;

        pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
        if (oal_unlikely(pst_mac_device == OAL_PTR_NULL)) {
            us_deauth_len = 0;
            oam_error_log1(0, OAM_SF_ANY,
                           "{hmac_mgmt_encap_deauth::pst_mac_device[%d] null!}",
                           pst_mac_vap->uc_device_id);
            return us_deauth_len;
        }

        ul_ret = mac_device_find_2up_vap(pst_mac_device, &pst_up_vap1, &pst_up_vap2);
        if (ul_ret == OAL_SUCC) {
            /* 获取另外一个VAP，组帧时修改地址2为另外1个VAP的MAC地址 */
            if (pst_mac_vap->uc_vap_id != pst_up_vap1->uc_vap_id) {
                pst_up_vap2 = pst_up_vap1;
            }

            if (pst_up_vap2->pst_mib_info == OAL_PTR_NULL) {
                us_deauth_len = 0;
                oam_error_log0(pst_up_vap2->uc_vap_id, OAM_SF_AUTH,
                               "hmac_mgmt_encap_deauth: pst_up_vap2 mib ptr null.");
                return us_deauth_len;
            }
            oal_set_mac_addr(((mac_ieee80211_frame_stru *)puc_data)->auc_address2, mac_mib_get_StationID(pst_up_vap2));
            oal_set_mac_addr(((mac_ieee80211_frame_stru *)puc_data)->auc_address3, pst_up_vap2->auc_bssid);
        }

        oam_warning_log1(0, OAM_SF_AUTH,
                         "hmac_mgmt_encap_deauth: send the second deauth frame. error code:%d", us_err_code);
    } else
    {
        if (pst_mac_vap->pst_mib_info == OAL_PTR_NULL) {
            us_deauth_len = 0;
            oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_AUTH,
                           "hmac_mgmt_encap_deauth: pst_mac_vap mib ptr null.");
            return us_deauth_len;
        }

        /* SA is the dot11MACAddress */
        oal_set_mac_addr(((mac_ieee80211_frame_stru *)puc_data)->auc_address2, mac_mib_get_StationID(pst_mac_vap));
        oal_set_mac_addr(((mac_ieee80211_frame_stru *)puc_data)->auc_address3, pst_mac_vap->auc_bssid);
    }
    /*************************************************************************/
    /*                Set the contents of the frame body                     */
    /*************************************************************************/
    /*************************************************************************/
    /*                  Deauthentication Frame - Frame Body                  */
    /* --------------------------------------------------------------------- */
    /* |                           Reason Code                             | */
    /* --------------------------------------------------------------------- */
    /* |2 Byte                                                             | */
    /* --------------------------------------------------------------------- */
    /*                                                                       */
    /*************************************************************************/
    /* Set Reason Code to 'Class2 error' */
    puc_data[MAC_80211_FRAME_LEN] = (us_err_code & 0x00FF);
    puc_data[MAC_80211_FRAME_LEN + 1] = (us_err_code & 0xFF00) >> 8;

    us_deauth_len = MAC_80211_FRAME_LEN + WLAN_REASON_CODE_LEN;
    return us_deauth_len;
}


uint16_t hmac_mgmt_encap_disassoc(mac_vap_stru *pst_mac_vap, uint8_t *puc_data,
                                        uint8_t *puc_da, uint16_t us_err_code)
{
    uint16_t us_disassoc_len = 0;

    /*************************************************************************/
    /*                        Management Frame Format                        */
    /* --------------------------------------------------------------------  */
    /* |Frame Control|Duration|DA|SA|BSSID|Sequence Control|Frame Body|FCS|  */
    /* --------------------------------------------------------------------  */
    /* | 2           |2       |6 |6 |6    |2               |0 - 2312  |4  |  */
    /* --------------------------------------------------------------------  */
    /*                                                                       */
    /*************************************************************************/
    /*************************************************************************/
    /*                            设置帧头                                   */
    /*************************************************************************/
    /* 设置subtype   */
    mac_hdr_set_frame_control(puc_data, WLAN_FC0_SUBTYPE_DISASSOC);

    if (pst_mac_vap->pst_mib_info == OAL_PTR_NULL) {
        us_disassoc_len = 0;
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC, "hmac_mgmt_encap_disassoc: pst_mac_vap mib ptr null.");
        return us_disassoc_len;
    }
    /* 设置DA */
    oal_set_mac_addr(((mac_ieee80211_frame_stru *)puc_data)->auc_address1, puc_da);

    /* 设置SA */
    oal_set_mac_addr(((mac_ieee80211_frame_stru *)puc_data)->auc_address2, mac_mib_get_StationID(pst_mac_vap));

    /* 设置bssid */
    oal_set_mac_addr(((mac_ieee80211_frame_stru *)puc_data)->auc_address3,
                     pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_AP ?
                     mac_mib_get_StationID(pst_mac_vap) : pst_mac_vap->auc_bssid);

    /*************************************************************************/
    /*                  Disassociation 帧 - 帧体                  */
    /* --------------------------------------------------------------------- */
    /* |                           Reason Code                             | */
    /* --------------------------------------------------------------------- */
    /* |2 Byte                                                             | */
    /* --------------------------------------------------------------------- */
    /*                                                                       */
    /*************************************************************************/
    /* 设置reason code */
    puc_data[MAC_80211_FRAME_LEN] = (us_err_code & 0x00FF);
    puc_data[MAC_80211_FRAME_LEN + 1] = (us_err_code & 0xFF00) >> 8;

    us_disassoc_len = MAC_80211_FRAME_LEN + WLAN_REASON_CODE_LEN;
    return us_disassoc_len;
}

uint16_t hmac_encap_notify_chan_width(mac_vap_stru *pst_mac_vap, uint8_t *puc_data, uint8_t *puc_da)
{
    uint16_t us_len;

    /*************************************************************************/
    /*                        Management Frame Format                        */
    /* --------------------------------------------------------------------  */
    /* |Frame Control|Duration|DA|SA|BSSID|Sequence Control|Frame Body|FCS|  */
    /* --------------------------------------------------------------------  */
    /* | 2           |2       |6 |6 |6    |2               |0 - 2312  |4  |  */
    /* --------------------------------------------------------------------  */
    /*                                                                       */
    /*************************************************************************/
    /*************************************************************************/
    /*                Set the fields in the frame header                     */
    /*************************************************************************/
    /* All the fields of the Frame Control Field are set to zero. Only the   */
    /* Type/Subtype field is set.                                            */
    mac_hdr_set_frame_control(puc_data, WLAN_FC0_SUBTYPE_ACTION);
    /*  Set DA  */
    oal_set_mac_addr(((mac_ieee80211_frame_stru *)puc_data)->auc_address1, puc_da);
    /*  Set SA  */
    oal_set_mac_addr(((mac_ieee80211_frame_stru *)puc_data)->auc_address2, mac_mib_get_StationID(pst_mac_vap));
    /*  Set SSID  */
    oal_set_mac_addr(((mac_ieee80211_frame_stru *)puc_data)->auc_address3, pst_mac_vap->auc_bssid);

    /*************************************************************************/
    /*                Set the contents of the frame body                     */
    /*************************************************************************/
    /*************************************************************************/
    /*                  SA Query Frame - Frame Body                          */
    /* --------------------------------------------------------------------- */
    /* |   Category   |SA Query Action |  Transaction Identifier           | */
    /* --------------------------------------------------------------------- */
    /* |1             |1               |2 Byte                             | */
    /* --------------------------------------------------------------------- */
    /*                                                                       */
    /*************************************************************************/
    puc_data[MAC_80211_FRAME_LEN] = MAC_ACTION_CATEGORY_HT;
    puc_data[MAC_80211_FRAME_LEN + 1] = MAC_HT_ACTION_NOTIFY_CHANNEL_WIDTH;
    puc_data[MAC_80211_FRAME_LEN + 2] = (pst_mac_vap->st_channel.en_bandwidth > WLAN_BAND_WIDTH_20M) ? 1 : 0;

    us_len = MAC_80211_FRAME_LEN + MAC_HT_NOTIFY_CHANNEL_WIDTH_LEN;
    return us_len;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

