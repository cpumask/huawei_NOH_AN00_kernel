

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include "oal_kernel_file.h"
#include "wlan_spec.h"
#include "mac_vap.h"
#include "mac_ie.h"
#include "mac_frame.h"
#include "hmac_mgmt_bss_comm.h"
#include "mac_resource.h"
#include "hmac_device.h"
#include "hmac_resource.h"
#include "hmac_fsm.h"
#include "hmac_encap_frame.h"
#include "hmac_tx_amsdu.h"
#include "hmac_mgmt_ap.h"
#include "hmac_mgmt_sta.h"
#include "hmac_blockack.h"
#include "hmac_p2p.h"
#include "hmac_dfx.h"

#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_MGMT_BSS_COMM_C

#ifdef _PRE_WLAN_FEATURE_SNIFFER
#ifdef CONFIG_HW_SNIFFER
#include <hwnet/ipv4/sysctl_sniffer.h>
#endif
#endif
#include "securec.h"
#include "securectype.h"

/*
 * 行:代表VAP 的协议能力
 * 列:代表USER 的协议能力
 */
#if defined(_PRE_WLAN_FEATURE_11AX) || defined(_PRE_WLAN_FEATURE_11AX_ROM)
uint8_t g_auc_avail_protocol_mode[WLAN_PROTOCOL_BUTT][WLAN_PROTOCOL_BUTT] = {
    { WLAN_LEGACY_11A_MODE, WLAN_PROTOCOL_BUTT,   WLAN_PROTOCOL_BUTT,   WLAN_PROTOCOL_BUTT,
      WLAN_PROTOCOL_BUTT,   WLAN_LEGACY_11A_MODE, WLAN_LEGACY_11A_MODE, WLAN_PROTOCOL_BUTT,
      WLAN_PROTOCOL_BUTT,   WLAN_PROTOCOL_BUTT,   WLAN_LEGACY_11A_MODE },    // 11A
    { WLAN_PROTOCOL_BUTT,   WLAN_LEGACY_11B_MODE, WLAN_LEGACY_11B_MODE, WLAN_LEGACY_11B_MODE,
      WLAN_LEGACY_11B_MODE, WLAN_LEGACY_11B_MODE, WLAN_LEGACY_11B_MODE, WLAN_PROTOCOL_BUTT,
      WLAN_PROTOCOL_BUTT,   WLAN_PROTOCOL_BUTT,   WLAN_LEGACY_11B_MODE },    // 11B
    { WLAN_PROTOCOL_BUTT,   WLAN_PROTOCOL_BUTT,   WLAN_LEGACY_11G_MODE, WLAN_LEGACY_11G_MODE,
      WLAN_LEGACY_11G_MODE, WLAN_LEGACY_11G_MODE, WLAN_LEGACY_11G_MODE, WLAN_PROTOCOL_BUTT,
      WLAN_PROTOCOL_BUTT,   WLAN_LEGACY_11G_MODE, WLAN_LEGACY_11G_MODE },    // 11G
    { WLAN_PROTOCOL_BUTT,   WLAN_LEGACY_11B_MODE, WLAN_LEGACY_11G_MODE, WLAN_MIXED_ONE_11G_MODE,
      WLAN_MIXED_ONE_11G_MODE, WLAN_MIXED_ONE_11G_MODE, WLAN_MIXED_ONE_11G_MODE, WLAN_PROTOCOL_BUTT,
      WLAN_PROTOCOL_BUTT,   WLAN_PROTOCOL_BUTT,   WLAN_PROTOCOL_BUTT },    // ONE_11G
    { WLAN_PROTOCOL_BUTT,   WLAN_PROTOCOL_BUTT,   WLAN_LEGACY_11G_MODE, WLAN_MIXED_ONE_11G_MODE,
      WLAN_MIXED_TWO_11G_MODE, WLAN_MIXED_ONE_11G_MODE, WLAN_MIXED_ONE_11G_MODE, WLAN_PROTOCOL_BUTT,
      WLAN_PROTOCOL_BUTT,   WLAN_PROTOCOL_BUTT,   WLAN_PROTOCOL_BUTT },    // TWO_11G
    { WLAN_LEGACY_11A_MODE, WLAN_LEGACY_11B_MODE, WLAN_LEGACY_11G_MODE, WLAN_MIXED_ONE_11G_MODE,
      WLAN_MIXED_ONE_11G_MODE, WLAN_HT_MODE,      WLAN_HT_MODE,         WLAN_HT_ONLY_MODE,
      WLAN_PROTOCOL_BUTT,   WLAN_HT_11G_MODE,     WLAN_HT_MODE },    // HT
    { WLAN_LEGACY_11A_MODE, WLAN_LEGACY_11B_MODE, WLAN_LEGACY_11G_MODE, WLAN_MIXED_ONE_11G_MODE,
      WLAN_MIXED_ONE_11G_MODE, WLAN_HT_MODE,      WLAN_VHT_MODE,        WLAN_HT_ONLY_MODE,
      WLAN_HT_ONLY_MODE,    WLAN_PROTOCOL_BUTT,   WLAN_VHT_MODE },    // VHT
    { WLAN_PROTOCOL_BUTT,   WLAN_PROTOCOL_BUTT,   WLAN_PROTOCOL_BUTT,   WLAN_PROTOCOL_BUTT,
      WLAN_PROTOCOL_BUTT,   WLAN_HT_ONLY_MODE,    WLAN_HT_ONLY_MODE,    WLAN_HT_ONLY_MODE,
      WLAN_HT_ONLY_MODE,    WLAN_HT_ONLY_MODE,    WLAN_HT_ONLY_MODE },    // HT_ONLY
    { WLAN_PROTOCOL_BUTT,   WLAN_PROTOCOL_BUTT,   WLAN_PROTOCOL_BUTT,   WLAN_PROTOCOL_BUTT,
      WLAN_PROTOCOL_BUTT,   WLAN_PROTOCOL_BUTT,   WLAN_VHT_ONLY_MODE,   WLAN_PROTOCOL_BUTT,
      WLAN_VHT_ONLY_MODE,   WLAN_PROTOCOL_BUTT,   WLAN_PROTOCOL_BUTT },    // VHT_ONLY
    { WLAN_PROTOCOL_BUTT,   WLAN_PROTOCOL_BUTT,   WLAN_LEGACY_11G_MODE, WLAN_LEGACY_11G_MODE,
      WLAN_LEGACY_11G_MODE, WLAN_HT_11G_MODE,     WLAN_PROTOCOL_BUTT,   WLAN_HT_ONLY_MODE,
      WLAN_PROTOCOL_BUTT,   WLAN_HT_11G_MODE,     WLAN_PROTOCOL_BUTT },    // HT_11G
    { WLAN_LEGACY_11A_MODE, WLAN_LEGACY_11B_MODE, WLAN_LEGACY_11G_MODE, WLAN_MIXED_ONE_11G_MODE,
      WLAN_MIXED_ONE_11G_MODE, WLAN_HT_MODE,      WLAN_VHT_MODE,        WLAN_HT_ONLY_MODE,
      WLAN_VHT_ONLY_MODE,   WLAN_PROTOCOL_BUTT,   WLAN_HE_MODE },    /* he */
};
#else
uint8_t g_auc_avail_protocol_mode[WLAN_PROTOCOL_BUTT][WLAN_PROTOCOL_BUTT] = {
    { WLAN_LEGACY_11A_MODE, WLAN_PROTOCOL_BUTT,   WLAN_PROTOCOL_BUTT,   WLAN_PROTOCOL_BUTT,
      WLAN_PROTOCOL_BUTT,   WLAN_LEGACY_11A_MODE, WLAN_LEGACY_11A_MODE, WLAN_PROTOCOL_BUTT,
      WLAN_PROTOCOL_BUTT,   WLAN_PROTOCOL_BUTT },
    { WLAN_PROTOCOL_BUTT,   WLAN_LEGACY_11B_MODE, WLAN_LEGACY_11B_MODE, WLAN_LEGACY_11B_MODE,
      WLAN_LEGACY_11B_MODE, WLAN_LEGACY_11B_MODE, WLAN_LEGACY_11B_MODE, WLAN_PROTOCOL_BUTT,
      WLAN_PROTOCOL_BUTT,   WLAN_PROTOCOL_BUTT },
    { WLAN_PROTOCOL_BUTT,   WLAN_PROTOCOL_BUTT,   WLAN_LEGACY_11G_MODE, WLAN_LEGACY_11G_MODE,
      WLAN_LEGACY_11G_MODE, WLAN_LEGACY_11G_MODE, WLAN_LEGACY_11G_MODE, WLAN_PROTOCOL_BUTT,
      WLAN_PROTOCOL_BUTT,   WLAN_LEGACY_11G_MODE },
    { WLAN_PROTOCOL_BUTT,   WLAN_LEGACY_11B_MODE, WLAN_LEGACY_11G_MODE, WLAN_MIXED_ONE_11G_MODE,
      WLAN_MIXED_ONE_11G_MODE, WLAN_MIXED_ONE_11G_MODE, WLAN_MIXED_ONE_11G_MODE, WLAN_PROTOCOL_BUTT,
      WLAN_PROTOCOL_BUTT,   WLAN_LEGACY_11G_MODE },
    { WLAN_PROTOCOL_BUTT,   WLAN_PROTOCOL_BUTT,   WLAN_LEGACY_11G_MODE, WLAN_MIXED_ONE_11G_MODE,
      WLAN_MIXED_TWO_11G_MODE, WLAN_MIXED_ONE_11G_MODE, WLAN_MIXED_ONE_11G_MODE, WLAN_PROTOCOL_BUTT,
      WLAN_PROTOCOL_BUTT,   WLAN_LEGACY_11G_MODE },
    { WLAN_LEGACY_11A_MODE, WLAN_LEGACY_11B_MODE, WLAN_LEGACY_11G_MODE, WLAN_MIXED_ONE_11G_MODE,
      WLAN_MIXED_ONE_11G_MODE, WLAN_HT_MODE,      WLAN_HT_MODE,         WLAN_HT_ONLY_MODE,
      WLAN_PROTOCOL_BUTT,   WLAN_HT_11G_MODE },
    { WLAN_LEGACY_11A_MODE, WLAN_LEGACY_11B_MODE, WLAN_LEGACY_11G_MODE, WLAN_MIXED_ONE_11G_MODE,
      WLAN_MIXED_ONE_11G_MODE, WLAN_HT_MODE,      WLAN_VHT_MODE,        WLAN_HT_ONLY_MODE,
      WLAN_VHT_ONLY_MODE,   WLAN_PROTOCOL_BUTT },
    { WLAN_PROTOCOL_BUTT,   WLAN_PROTOCOL_BUTT,   WLAN_PROTOCOL_BUTT,   WLAN_PROTOCOL_BUTT,
      WLAN_PROTOCOL_BUTT,   WLAN_HT_ONLY_MODE,    WLAN_HT_ONLY_MODE,    WLAN_HT_ONLY_MODE,
      WLAN_HT_ONLY_MODE,    WLAN_HT_ONLY_MODE },
    { WLAN_PROTOCOL_BUTT,   WLAN_PROTOCOL_BUTT,   WLAN_PROTOCOL_BUTT,   WLAN_PROTOCOL_BUTT,
      WLAN_PROTOCOL_BUTT,   WLAN_PROTOCOL_BUTT,   WLAN_VHT_ONLY_MODE,   WLAN_PROTOCOL_BUTT,
      WLAN_VHT_ONLY_MODE,   WLAN_PROTOCOL_BUTT },
    { WLAN_PROTOCOL_BUTT,   WLAN_PROTOCOL_BUTT,   WLAN_LEGACY_11G_MODE, WLAN_LEGACY_11G_MODE,
      WLAN_LEGACY_11G_MODE, WLAN_HT_11G_MODE,     WLAN_PROTOCOL_BUTT,   WLAN_HT_ONLY_MODE,
      WLAN_PROTOCOL_BUTT,   WLAN_HT_11G_MODE },
};
#endif

uint32_t hmac_mgmt_tx_addba_timeout(void *arg);


void hmac_rx_ba_session_decr(hmac_vap_stru *hmac_vap, uint8_t tidno)
{
    if (mac_mib_get_RxBASessionNumber(&hmac_vap->st_vap_base_info) == 0) {
        oam_warning_log1(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_BA,
            "{hmac_rx_ba_session_decr::tid[%d] rx_session already zero.}", tidno);
        return;
    }

    mac_mib_decr_RxBASessionNumber(&hmac_vap->st_vap_base_info);

    oam_warning_log2(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_BA,
                     "{hmac_rx_ba_session_decr::tid[%d] tx_session[%d] remove.}",
                     tidno, mac_mib_get_RxBASessionNumber(&hmac_vap->st_vap_base_info));
}


void hmac_tx_ba_session_decr(hmac_vap_stru *hmac_vap, uint8_t tidno)
{
    if (mac_mib_get_TxBASessionNumber(&hmac_vap->st_vap_base_info) == 0) {
        oam_warning_log1(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_BA,
                         "{hmac_tx_ba_session_decr::tid[%d] tx_session already zero.}", tidno);
        return;
    }

    mac_mib_decr_TxBASessionNumber(&hmac_vap->st_vap_base_info);

    oam_warning_log2(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_BA,
                     "{hmac_tx_ba_session_decr::tid[%d] tx_session[%d] remove.}",
                     tidno, mac_mib_get_TxBASessionNumber(&hmac_vap->st_vap_base_info));
}


uint16_t hmac_mgmt_encap_addba_req(hmac_vap_stru *pst_vap,
                                   uint8_t *puc_data,
                                   dmac_ba_tx_stru *pst_tx_ba,
                                   uint8_t uc_tid)
{
    uint16_t us_index;
    uint16_t us_ba_param;
    if (oal_any_null_ptr3(pst_vap, puc_data, pst_tx_ba)) {
        oam_error_log3(0, OAM_SF_BA, "{hmac_mgmt_encap_addba_req::null param.vap:%x data:%x ba:%x}",
                       (uintptr_t)pst_vap, (uintptr_t)puc_data, (uintptr_t)pst_tx_ba);
        return OAL_ERR_CODE_PTR_NULL;
    }

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
    /* Frame Control Field 中只需要设置Type/Subtype值，其他设置为0 */
    mac_hdr_set_frame_control(puc_data, WLAN_PROTOCOL_VERSION | WLAN_FC0_TYPE_MGT | WLAN_FC0_SUBTYPE_ACTION);

    /* DA is address of STA requesting association */
    oal_set_mac_addr(puc_data + 4, pst_tx_ba->puc_dst_addr);

    /* SA的值为dot11MACAddress的值 */
    oal_set_mac_addr(puc_data + 10, mac_mib_get_StationID(&pst_vap->st_vap_base_info));

    oal_set_mac_addr(puc_data + 16, pst_vap->st_vap_base_info.auc_bssid);

    /*************************************************************************/
    /*                Set the contents of the frame body                     */
    /*************************************************************************/
    /* 将索引指向frame body起始位置 */
    us_index = MAC_80211_FRAME_LEN;

    /* 设置Category */
    puc_data[us_index++] = MAC_ACTION_CATEGORY_BA;

    /* 设置Action */
    puc_data[us_index++] = MAC_BA_ACTION_ADDBA_REQ;

    /* 设置Dialog Token */
    puc_data[us_index++] = pst_tx_ba->uc_dialog_token;

    /*
        设置Block Ack Parameter set field
        bit0 - AMSDU Allowed
        bit1 - Immediate or Delayed block ack
        bit2-bit5 - TID
        bit6-bit15 -  Buffer size
    */
    us_ba_param = pst_tx_ba->en_amsdu_supp;        /* bit0 */
    us_ba_param |= (pst_tx_ba->uc_ba_policy << 1); /* bit1 */
    us_ba_param |= (uc_tid << 2);                  /* bit2 */

    us_ba_param |= (uint16_t)(pst_tx_ba->us_baw_size << 6); /* bit6 */

    puc_data[us_index++] = (uint8_t)(us_ba_param & 0xFF);
    puc_data[us_index++] = (uint8_t)((us_ba_param >> 8) & 0xFF);

    /* 设置BlockAck timeout */
    puc_data[us_index++] = (uint8_t)(pst_tx_ba->us_ba_timeout & 0xFF);
    puc_data[us_index++] = (uint8_t)((pst_tx_ba->us_ba_timeout >> 8) & 0xFF);

    /*
        Block ack starting sequence number字段由硬件设置
        bit0-bit3 fragmentnumber
        bit4-bit15: sequence number
    */
    /* us_buf_seq此处暂不填写，在dmac侧会补充填写 */
    *(uint16_t *)&puc_data[us_index++] = 0;
    us_index++;

    /* 返回的帧长度中不包括FCS */
    return us_index;
}
OAL_STATIC void hmac_mgmt_encap_addba_rsp_set_ba_param(hmac_vap_stru *pst_vap,
    hmac_user_stru *pst_hmac_user, hmac_ba_rx_stru *pst_addba_rsp, uint8_t uc_tid, uint16_t *pus_ba_param)
{
#ifdef _PRE_WLAN_FEATURE_11AX
    oal_bool_enum_uint8 en_is_HE_implemented;

    // 降低圈复杂度
    en_is_HE_implemented = (g_wlan_spec_cfg->feature_11ax_is_open &&
                            ((OAL_TRUE == mac_mib_get_HEOptionImplemented(&pst_vap->st_vap_base_info)) &&
                            (MAC_USER_IS_HE_USER(&pst_hmac_user->st_user_base_info))));
    /* 标志vap工作在11ax */
    if (en_is_HE_implemented) {
        *pus_ba_param |= (uint16_t)(pst_hmac_user->aus_tid_baw_size[uc_tid] << 6); /* BA param set的BIT6~BIT15 */
    } else
#endif
    {
        *pus_ba_param |= (uint16_t)(pst_addba_rsp->us_baw_size << 6); /* 最大聚合个数是BA param set的BIT6~BIT15 */
    }
    hmac_btcoex_set_addba_rsp_ba_param(pst_vap, pst_hmac_user, pst_addba_rsp, pus_ba_param);
}


uint16_t hmac_mgmt_encap_addba_rsp(hmac_vap_stru *pst_vap, uint8_t *puc_data,
    hmac_ba_rx_stru *pst_addba_rsp, uint8_t uc_tid, uint8_t uc_status)
{
    uint16_t us_index;
    uint16_t us_ba_param;
    hmac_user_stru *pst_hmac_user = NULL;
    if (oal_any_null_ptr3(pst_vap, puc_data, pst_addba_rsp)) {
        oam_error_log3(0, OAM_SF_BA, "{hmac_mgmt_encap_addba_req::null param.vap:%x data:%x rsp:%x}",
                       (uintptr_t)pst_vap, (uintptr_t)puc_data, (uintptr_t)pst_addba_rsp);
        return OAL_ERR_CODE_PTR_NULL;
    }
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
    mac_hdr_set_frame_control(puc_data, WLAN_PROTOCOL_VERSION | WLAN_FC0_TYPE_MGT | WLAN_FC0_SUBTYPE_ACTION);
    /* DA is address of STA requesting association */
    oal_set_mac_addr(puc_data + 4, pst_addba_rsp->puc_transmit_addr);
    /* SA is the dot11MACAddress */
    oal_set_mac_addr(puc_data + 10, mac_mib_get_StationID(&pst_vap->st_vap_base_info));
    oal_set_mac_addr(puc_data + 16, pst_vap->st_vap_base_info.auc_bssid);
    /*************************************************************************/
    /*                Set the contents of the frame body                     */
    /*************************************************************************/
    /*************************************************************************/
    /*             ADDBA Response Frame - Frame Body                         */
    /*    ---------------------------------------------------------------    */
    /*    | Category | Action | Dialog | Status  | Parameters | Timeout |    */
    /*    ---------------------------------------------------------------    */
    /*    | 1        | 1      | 1      | 2       | 2          | 2       |    */
    /*    ---------------------------------------------------------------    */
    /*                                                                       */
    /*************************************************************************/
    /* Initialize index and the frame data pointer */
    us_index = MAC_80211_FRAME_LEN;
    /* Action Category设置 */
    puc_data[us_index++] = MAC_ACTION_CATEGORY_BA;
    /* 特定Action种类下的action的帧类型 */
    puc_data[us_index++] = MAC_BA_ACTION_ADDBA_RSP;
    /* Dialog Token域设置，需要从req中copy过来 */
    puc_data[us_index++] = pst_addba_rsp->uc_dialog_token;
    /* 状态域设置 */
    puc_data[us_index++] = uc_status;
    puc_data[us_index++] = 0;
    /* Block Ack Parameter设置 */
    /* B0 - AMSDU Support, B1- Immediate or Delayed block ack */
    /* B2-B5 : TID, B6-B15: Buffer size */
    us_ba_param = pst_addba_rsp->en_amsdu_supp;        /* BIT0 */
    us_ba_param |= (pst_addba_rsp->uc_ba_policy << 1); /* BIT1 */
    us_ba_param |= (uc_tid << 2);                      /* BIT2 */
    /* 手动设置聚合个数，屏蔽删建BA时不采用64聚合 */
    pst_hmac_user = mac_vap_get_hmac_user_by_addr(&(pst_vap->st_vap_base_info), pst_addba_rsp->puc_transmit_addr);
    if (pst_hmac_user != OAL_PTR_NULL) {
        hmac_mgmt_encap_addba_rsp_set_ba_param(pst_vap, pst_hmac_user, pst_addba_rsp, uc_tid, &us_ba_param);
    } else {
        us_ba_param |= (uint16_t)(pst_addba_rsp->us_baw_size << 6); /* BIT6 */
    }
    puc_data[us_index++] = (uint8_t)(us_ba_param & 0xFF);
    puc_data[us_index++] = (uint8_t)((us_ba_param >> 8) & 0xFF);
    puc_data[us_index++] = 0x00;
    puc_data[us_index++] = 0x00;
    /* 返回的帧长度中不包括FCS */
    return us_index;
}


uint16_t hmac_mgmt_encap_delba(hmac_vap_stru *pst_vap,
    uint8_t *puc_data, uint8_t *puc_addr, uint8_t uc_tid,
    mac_delba_initiator_enum_uint8 en_initiator, uint8_t reason)
{
    uint16_t us_index;

    if (oal_any_null_ptr3(pst_vap, puc_data, puc_addr)) {
        oam_error_log3(0, OAM_SF_BA, "{hmac_mgmt_encap_delba::null param, %x %x %x.}",
                       (uintptr_t)pst_vap, (uintptr_t)puc_data, (uintptr_t)puc_addr);
        return OAL_ERR_CODE_PTR_NULL;
    }

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
    mac_hdr_set_frame_control(puc_data, WLAN_PROTOCOL_VERSION | WLAN_FC0_TYPE_MGT | WLAN_FC0_SUBTYPE_ACTION);

    /* duration */
    puc_data[2] = 0;
    puc_data[3] = 0;

    /* DA is address of STA requesting association */
    oal_set_mac_addr(puc_data + 4, puc_addr);

    /* SA is the dot11MACAddress */
    oal_set_mac_addr(puc_data + 10, mac_mib_get_StationID(&pst_vap->st_vap_base_info));

    oal_set_mac_addr(puc_data + 16, pst_vap->st_vap_base_info.auc_bssid);

    /* seq control */
    puc_data[22] = 0;
    puc_data[23] = 0;

    /*************************************************************************/
    /*                Set the contents of the frame body                     */
    /*************************************************************************/
    /*************************************************************************/
    /*             DELBA Response Frame - Frame Body                         */
    /*        -------------------------------------------------              */
    /*        | Category | Action |  Parameters | Reason code |              */
    /*        -------------------------------------------------              */
    /*        | 1        | 1      |       2     | 2           |              */
    /*        -------------------------------------------------              */
    /*                          Parameters                                   */
    /*                  -------------------------------                      */
    /*                  | Reserved | Initiator |  TID  |                     */
    /*                  -------------------------------                      */
    /*             bit  |    11    |    1      |  4    |                     */
    /*                  --------------------------------                     */
    /*************************************************************************/
    /* Initialize index and the frame data pointer */
    us_index = MAC_80211_FRAME_LEN;

    /* Category */
    puc_data[us_index++] = MAC_ACTION_CATEGORY_BA;

    /* Action */
    puc_data[us_index++] = MAC_BA_ACTION_DELBA;

    /* DELBA parameter set */
    /* B0 - B10 -reserved */
    /* B11 - initiator */
    /* B12-B15 - TID */
    puc_data[us_index++] = 0;
    puc_data[us_index] = (uint8_t)(en_initiator << 3);
    puc_data[us_index++] |= (uint8_t)(uc_tid << 4);

    /* Reason field */
    /* Reason can be either of END_BA, QSTA_LEAVING, UNKNOWN_BA */
    puc_data[us_index++] = reason;
    puc_data[us_index++] = 0;

    /* 返回的帧长度中不包括FCS */
    return us_index;
}


uint32_t hmac_mgmt_tx_addba_req(hmac_vap_stru *pst_hmac_vap,
                                hmac_user_stru *pst_hmac_user,
                                mac_action_mgmt_args_stru *pst_action_args)
{
    mac_device_stru *pst_device = OAL_PTR_NULL;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL; /* 申请事件返回的内存指针 */
    oal_netbuf_stru *pst_addba_req = OAL_PTR_NULL;
    dmac_ba_tx_stru st_tx_ba;
    uint8_t uc_tidno;
    uint16_t us_frame_len;
    frw_event_stru *pst_hmac_to_dmac_ctx_event = OAL_PTR_NULL;
    dmac_tx_event_stru *pst_tx_event = OAL_PTR_NULL;
    dmac_ctx_action_event_stru st_wlan_ctx_action;
    uint32_t ul_ret;
    mac_tx_ctl_stru *pst_tx_ctl = OAL_PTR_NULL;

    if (oal_any_null_ptr3(pst_hmac_vap, pst_hmac_user, pst_action_args)) {
        oam_error_log3(0, OAM_SF_BA, "{hmac_mgmt_tx_addba_req::null param, %x %x %x.}",
                       (uintptr_t)pst_hmac_vap, (uintptr_t)pst_hmac_user, (uintptr_t)pst_action_args);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_mac_vap = &(pst_hmac_vap->st_vap_base_info);
    if (pst_mac_vap->en_vap_state == MAC_VAP_STATE_BUTT) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_BA,
                         "{hmac_mgmt_tx_addba_req:: vap has been down/del, vap_state[%d].}",
                         pst_mac_vap->en_vap_state);
        return OAL_FAIL;
    }
    /* 获取device结构 */
    pst_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (pst_device == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_BA, "{hmac_mgmt_tx_addba_req::pst_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 申请ADDBA_REQ管理帧内存 */
    pst_addba_req = oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF, WLAN_MEM_NETBUF_SIZE2, OAL_NETBUF_PRIORITY_MID);
    if (pst_addba_req == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_BA, "{hmac_mgmt_tx_addba_req::pst_addba_req null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    oal_mem_netbuf_trace(pst_addba_req, OAL_TRUE);

    oal_netbuf_prev(pst_addba_req) = OAL_PTR_NULL;
    oal_netbuf_next(pst_addba_req) = OAL_PTR_NULL;

    uc_tidno = (uint8_t)(pst_action_args->ul_arg1);
    /* 对tid对应的txBA会话状态加锁 */
    oal_spin_lock_bh(&(pst_hmac_user->ast_tid_info[uc_tidno].st_ba_tx_info.st_ba_status_lock));

    pst_hmac_vap->uc_ba_dialog_token++;
    st_tx_ba.uc_dialog_token = pst_hmac_vap->uc_ba_dialog_token; /* 保证ba会话创建能够区分 */
    st_tx_ba.us_baw_size = (uint8_t)(pst_action_args->ul_arg2);
    st_tx_ba.uc_ba_policy = (uint8_t)(pst_action_args->ul_arg3);
    st_tx_ba.us_ba_timeout = (uint16_t)(pst_action_args->ul_arg4);
    st_tx_ba.puc_dst_addr = pst_hmac_user->st_user_base_info.auc_user_mac_addr;

    /* 发端对AMPDU+AMSDU的支持 */
    st_tx_ba.en_amsdu_supp = (oal_bool_enum_uint8)mac_mib_get_AmsduPlusAmpduActive(pst_mac_vap);

    /*lint -e502*/
    if (st_tx_ba.en_amsdu_supp == OAL_FALSE) {
        HMAC_USER_SET_AMSDU_NOT_SUPPORT(pst_hmac_user, uc_tidno);
    } else {
        HMAC_USER_SET_AMSDU_SUPPORT(pst_hmac_user, uc_tidno); /* lint +e502 */
    }

    /* 调用封装管理帧接口 */
    us_frame_len = hmac_mgmt_encap_addba_req(pst_hmac_vap, oal_netbuf_data(pst_addba_req), &st_tx_ba, uc_tidno);
    memset_s((uint8_t *)&st_wlan_ctx_action, OAL_SIZEOF(st_wlan_ctx_action), 0, OAL_SIZEOF(st_wlan_ctx_action));
    /* 赋值要传入Dmac的信息 */
    st_wlan_ctx_action.us_frame_len = us_frame_len;
    st_wlan_ctx_action.uc_hdr_len = MAC_80211_FRAME_LEN;
    st_wlan_ctx_action.en_action_category = MAC_ACTION_CATEGORY_BA;
    st_wlan_ctx_action.uc_action = MAC_BA_ACTION_ADDBA_REQ;
    st_wlan_ctx_action.us_user_idx = pst_hmac_user->st_user_base_info.us_assoc_id;
    st_wlan_ctx_action.uc_tidno = uc_tidno;
    st_wlan_ctx_action.uc_dialog_token = st_tx_ba.uc_dialog_token;
    st_wlan_ctx_action.uc_ba_policy = st_tx_ba.uc_ba_policy;
    st_wlan_ctx_action.us_baw_size = st_tx_ba.us_baw_size;
    st_wlan_ctx_action.us_ba_timeout = st_tx_ba.us_ba_timeout;
    st_wlan_ctx_action.en_amsdu_supp = st_tx_ba.en_amsdu_supp;

    if (EOK != memcpy_s((uint8_t *)(oal_netbuf_data(pst_addba_req) + us_frame_len),
        OAL_SIZEOF(dmac_ctx_action_event_stru), (uint8_t *)&st_wlan_ctx_action,
        OAL_SIZEOF(dmac_ctx_action_event_stru))) {
        oam_error_log0(0, OAM_SF_BA, "hmac_mgmt_tx_addba_req::memcpy fail!");
        oal_netbuf_free(pst_addba_req);
        oal_spin_unlock_bh(&(pst_hmac_user->ast_tid_info[uc_tidno].st_ba_tx_info.st_ba_status_lock));
        return OAL_FAIL;
    }
    oal_netbuf_put(pst_addba_req, (us_frame_len + OAL_SIZEOF(dmac_ctx_action_event_stru)));

    /* 初始化CB */
    memset_s(oal_netbuf_cb(pst_addba_req), oal_netbuf_cb_size(), 0, oal_netbuf_cb_size());
    pst_tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_addba_req);
    MAC_GET_CB_MPDU_LEN(pst_tx_ctl) = us_frame_len + OAL_SIZEOF(dmac_ctx_action_event_stru);
    MAC_GET_CB_FRAME_TYPE(pst_tx_ctl) = WLAN_CB_FRAME_TYPE_ACTION;
    MAC_GET_CB_FRAME_SUBTYPE(pst_tx_ctl) = WLAN_ACTION_BA_ADDBA_REQ;

    pst_event_mem = frw_event_alloc_m(OAL_SIZEOF(dmac_tx_event_stru));
    if (pst_event_mem == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_BA, "{hmac_mgmt_tx_addba_req::pst_event_mem null.}");
        oal_netbuf_free(pst_addba_req);
        oal_spin_unlock_bh(&(pst_hmac_user->ast_tid_info[uc_tidno].st_ba_tx_info.st_ba_status_lock));
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
    pst_tx_event->pst_netbuf = pst_addba_req;
    pst_tx_event->us_frame_len = us_frame_len + OAL_SIZEOF(dmac_ctx_action_event_stru);

    ul_ret = frw_event_dispatch_event(pst_event_mem);
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_BA,
                       "{hmac_mgmt_tx_addba_req::frw_event_dispatch_event failed[%d].}", ul_ret);
        oal_netbuf_free(pst_addba_req);
        frw_event_free_m(pst_event_mem);
        oal_spin_unlock_bh(&(pst_hmac_user->ast_tid_info[uc_tidno].st_ba_tx_info.st_ba_status_lock));
        return ul_ret;
    }

    frw_event_free_m(pst_event_mem);

    /* 更新对应的TID信息 */
    pst_hmac_user->ast_tid_info[uc_tidno].st_ba_tx_info.en_ba_status = DMAC_BA_INPROGRESS;
    pst_hmac_user->ast_tid_info[uc_tidno].st_ba_tx_info.uc_dialog_token = st_tx_ba.uc_dialog_token;
    pst_hmac_user->ast_tid_info[uc_tidno].st_ba_tx_info.uc_ba_policy = st_tx_ba.uc_ba_policy;

    mac_mib_incr_TxBASessionNumber(pst_mac_vap);
    /* 启动ADDBA超时计时器 */
    frw_timer_create_timer_m(&pst_hmac_user->ast_tid_info[uc_tidno].st_ba_tx_info.st_addba_timer,
        hmac_mgmt_tx_addba_timeout, WLAN_ADDBA_TIMEOUT,
        &pst_hmac_user->ast_tid_info[uc_tidno].st_ba_tx_info.st_alarm_data, OAL_FALSE,
        OAM_MODULE_ID_HMAC, pst_device->ul_core_id);

    /* 对tid对应的tx BA会话状态解锁 */
    oal_spin_unlock_bh(&(pst_hmac_user->ast_tid_info[uc_tidno].st_ba_tx_info.st_ba_status_lock));

    return OAL_SUCC;
}


uint32_t hmac_mgmt_tx_addba_rsp(hmac_vap_stru *pst_hmac_vap, hmac_user_stru *pst_hmac_user,
    hmac_ba_rx_stru *pst_ba_rx_info, uint8_t uc_tid, uint8_t uc_status)
{
    mac_device_stru *pst_device = OAL_PTR_NULL;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL; /* 申请事件返回的内存指针 */
    frw_event_stru *pst_hmac_to_dmac_ctx_event = OAL_PTR_NULL;
    dmac_tx_event_stru *pst_tx_event = OAL_PTR_NULL;
    dmac_ctx_action_event_stru st_wlan_ctx_action;
    oal_netbuf_stru *pst_addba_rsp = OAL_PTR_NULL;
    uint16_t us_frame_len;
    uint32_t ul_ret;
    mac_tx_ctl_stru *pst_tx_ctl = OAL_PTR_NULL;

    if (oal_any_null_ptr3(pst_hmac_vap, pst_hmac_user, pst_ba_rx_info)) {
        oam_error_log3(0, OAM_SF_BA, "{hmac_mgmt_tx_addba_rsp::send addba rsp failed, null param, %x %x %x.}",
                       (uintptr_t)pst_hmac_vap, (uintptr_t)pst_hmac_user, (uintptr_t)pst_ba_rx_info);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_mac_vap = &(pst_hmac_vap->st_vap_base_info);
    if (pst_mac_vap->en_vap_state == MAC_VAP_STATE_BUTT) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_BA,
                         "{hmac_mgmt_tx_addba_rsp:: vap has been down/del, vap_state[%d].}",
                         pst_mac_vap->en_vap_state);
        return OAL_FAIL;
    }
    /* 获取device结构 */
    pst_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (pst_device == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_BA,
                       "{hmac_mgmt_tx_addba_rsp::send addba rsp failed, pst_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 申请ADDBA_RSP管理帧内存 */
    pst_addba_rsp = oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF, WLAN_MEM_NETBUF_SIZE2, OAL_NETBUF_PRIORITY_MID);
    if (pst_addba_rsp == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_BA,
                       "{hmac_mgmt_tx_addba_rsp::send addba rsp failed, pst_addba_rsp mem alloc failed.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    oal_mem_netbuf_trace(pst_addba_rsp, OAL_TRUE);

    oal_netbuf_prev(pst_addba_rsp) = OAL_PTR_NULL;
    oal_netbuf_next(pst_addba_rsp) = OAL_PTR_NULL;

    us_frame_len = hmac_mgmt_encap_addba_rsp(pst_hmac_vap, oal_netbuf_data(pst_addba_rsp),
                                             pst_ba_rx_info, uc_tid, uc_status);
    memset_s((uint8_t *)&st_wlan_ctx_action, OAL_SIZEOF(st_wlan_ctx_action), 0, OAL_SIZEOF(st_wlan_ctx_action));
    st_wlan_ctx_action.en_action_category = MAC_ACTION_CATEGORY_BA;
    st_wlan_ctx_action.uc_action = MAC_BA_ACTION_ADDBA_RSP;
    st_wlan_ctx_action.uc_hdr_len = MAC_80211_FRAME_LEN;
    st_wlan_ctx_action.us_baw_size = pst_hmac_user->aus_tid_baw_size[uc_tid];
    st_wlan_ctx_action.us_frame_len = us_frame_len;
    st_wlan_ctx_action.us_user_idx = pst_hmac_user->st_user_base_info.us_assoc_id;
    st_wlan_ctx_action.uc_tidno = uc_tid;
    st_wlan_ctx_action.uc_status = uc_status;
    st_wlan_ctx_action.us_ba_timeout = pst_ba_rx_info->us_ba_timeout;
    st_wlan_ctx_action.en_back_var = pst_ba_rx_info->en_back_var;
    st_wlan_ctx_action.uc_lut_index = pst_ba_rx_info->uc_lut_index;
    st_wlan_ctx_action.us_baw_start = pst_ba_rx_info->us_baw_start;
    st_wlan_ctx_action.uc_ba_policy = pst_ba_rx_info->uc_ba_policy;

    if (memcpy_s((uint8_t *)(oal_netbuf_data(pst_addba_rsp) + us_frame_len),
        OAL_SIZEOF(dmac_ctx_action_event_stru), (uint8_t *)&st_wlan_ctx_action,
        OAL_SIZEOF(dmac_ctx_action_event_stru)) != EOK) {
        oam_error_log0(0, OAM_SF_BA, "hmac_mgmt_tx_addba_rsp::memcpy fail!");
        oal_netbuf_free(pst_addba_rsp);
        return OAL_FAIL;
    }
    oal_netbuf_put(pst_addba_rsp, (us_frame_len + OAL_SIZEOF(dmac_ctx_action_event_stru)));

    /* 填写netbuf的cb字段，共发送管理帧和发送完成接口使用 */
    memset_s(oal_netbuf_cb(pst_addba_rsp), oal_netbuf_cb_size(), 0, oal_netbuf_cb_size());
    pst_tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_addba_rsp);
    MAC_GET_CB_TX_USER_IDX(pst_tx_ctl) = pst_hmac_user->st_user_base_info.us_assoc_id;
    MAC_GET_CB_WME_TID_TYPE(pst_tx_ctl) = uc_tid;
    MAC_GET_CB_MPDU_LEN(pst_tx_ctl) = us_frame_len + OAL_SIZEOF(dmac_ctx_action_event_stru);
    MAC_GET_CB_FRAME_TYPE(pst_tx_ctl) = WLAN_CB_FRAME_TYPE_ACTION;
    MAC_GET_CB_FRAME_SUBTYPE(pst_tx_ctl) = WLAN_ACTION_BA_ADDBA_RSP;

    pst_event_mem = frw_event_alloc_m(OAL_SIZEOF(dmac_tx_event_stru));
    if (pst_event_mem == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_BA,
                       "{hmac_mgmt_tx_addba_rsp::send addba rsp failed, pst_event_mem mem alloc failed.}");
        oal_netbuf_free(pst_addba_rsp);

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

    /* 填写事件payload */
    pst_tx_event = (dmac_tx_event_stru *)(pst_hmac_to_dmac_ctx_event->auc_event_data);
    pst_tx_event->pst_netbuf = pst_addba_rsp;
    pst_tx_event->us_frame_len = us_frame_len + OAL_SIZEOF(dmac_ctx_action_event_stru);

    ul_ret = frw_event_dispatch_event(pst_event_mem);
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_BA,
            "{hmac_mgmt_tx_addba_rsp::send addba rsp failed, frw_event_dispatch_event failed[%d].}", ul_ret);
        oal_netbuf_free(pst_addba_rsp);
    } else {
        pst_ba_rx_info->en_ba_status = DMAC_BA_COMPLETE;
    }

    frw_event_free_m(pst_event_mem);
    return ul_ret;
}

uint32_t hmac_mgmt_tx_delba(hmac_vap_stru *pst_hmac_vap,
    hmac_user_stru *pst_hmac_user, mac_action_mgmt_args_stru *pst_action_args)
{
    mac_device_stru *pst_device = OAL_PTR_NULL;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL; /* 申请事件返回的内存指针 */
    oal_netbuf_stru *pst_delba = OAL_PTR_NULL;
    uint16_t us_frame_len;
    frw_event_stru *pst_hmac_to_dmac_ctx_event = OAL_PTR_NULL;
    dmac_tx_event_stru *pst_tx_event = OAL_PTR_NULL;
    dmac_ctx_action_event_stru st_wlan_ctx_action;
    mac_delba_initiator_enum_uint8 en_initiator;
    uint32_t ul_ret;
    mac_tx_ctl_stru *pst_tx_ctl = OAL_PTR_NULL;
    uint8_t uc_tidno;

    if (oal_any_null_ptr3(pst_hmac_vap, pst_hmac_user, pst_action_args)) {
        oam_error_log3(0, OAM_SF_BA, "{hmac_mgmt_tx_delba::null param, %x %x %x.}",
                       (uintptr_t)pst_hmac_vap, (uintptr_t)pst_hmac_user, (uintptr_t)pst_action_args);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_mac_vap = &(pst_hmac_vap->st_vap_base_info);
    if (pst_mac_vap->en_vap_state == MAC_VAP_STATE_BUTT) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_BA,
                         "{hmac_mgmt_tx_delba:: vap has been down/del, vap_state[%d].}",
                         pst_mac_vap->en_vap_state);
        return OAL_FAIL;
    }
    /* 获取device结构 */
    pst_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (pst_device == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_BA, "{hmac_mgmt_tx_delba::pst_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    en_initiator = (mac_delba_initiator_enum_uint8)(pst_action_args->ul_arg2);
    uc_tidno = (uint8_t)(pst_action_args->ul_arg1);

    /* 对tid对应的tx BA会话状态加锁 */
    oal_spin_lock_bh(&(pst_hmac_user->ast_tid_info[uc_tidno].st_ba_tx_info.st_ba_status_lock));

    
    if (en_initiator == MAC_ORIGINATOR_DELBA) {
        if (pst_hmac_user->ast_tid_info[uc_tidno].st_ba_tx_info.en_ba_status == DMAC_BA_INIT) {
            oal_spin_unlock_bh(&(pst_hmac_user->ast_tid_info[uc_tidno].st_ba_tx_info.st_ba_status_lock));
            return OAL_SUCC;
        }
    }

    /* 申请DEL_BA管理帧内存 */
    pst_delba = oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF, WLAN_MEM_NETBUF_SIZE2, OAL_NETBUF_PRIORITY_MID);
    if (pst_delba == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_BA, "{hmac_mgmt_tx_delba::pst_delba null.}");
        oal_spin_unlock_bh(&(pst_hmac_user->ast_tid_info[uc_tidno].st_ba_tx_info.st_ba_status_lock));
        return OAL_ERR_CODE_PTR_NULL;
    }

    oal_mem_netbuf_trace(pst_delba, OAL_TRUE);

    oal_netbuf_prev(pst_delba) = OAL_PTR_NULL;
    oal_netbuf_next(pst_delba) = OAL_PTR_NULL;

    /* 调用封装管理帧接口 */
    us_frame_len = hmac_mgmt_encap_delba(pst_hmac_vap,
                                         (uint8_t *)oal_netbuf_header(pst_delba),
                                         pst_action_args->puc_arg5, uc_tidno,
                                         en_initiator, (uint8_t)pst_action_args->ul_arg3);
    memset_s((uint8_t *)&st_wlan_ctx_action, OAL_SIZEOF(st_wlan_ctx_action), 0, OAL_SIZEOF(st_wlan_ctx_action));
    st_wlan_ctx_action.us_frame_len = us_frame_len;
    st_wlan_ctx_action.uc_hdr_len = MAC_80211_FRAME_LEN;
    st_wlan_ctx_action.en_action_category = MAC_ACTION_CATEGORY_BA;
    st_wlan_ctx_action.uc_action = MAC_BA_ACTION_DELBA;
    st_wlan_ctx_action.us_user_idx = pst_hmac_user->st_user_base_info.us_assoc_id;
    st_wlan_ctx_action.uc_tidno = uc_tidno;
    st_wlan_ctx_action.uc_initiator = en_initiator;

    if (memcpy_s((uint8_t *)(oal_netbuf_data(pst_delba) + us_frame_len),
        OAL_SIZEOF(dmac_ctx_action_event_stru), (uint8_t *)&st_wlan_ctx_action,
        OAL_SIZEOF(dmac_ctx_action_event_stru)) != EOK) {
        oam_error_log0(0, OAM_SF_BA, "hmac_mgmt_tx_delba::memcpy fail!");
        /* 释放管理帧内存到netbuf内存池 */
        oal_netbuf_free(pst_delba);
        /* 对tid对应的tx BA会话状态解锁 */
        oal_spin_unlock_bh(&(pst_hmac_user->ast_tid_info[uc_tidno].st_ba_tx_info.st_ba_status_lock));
        return OAL_FAIL;
    }
    oal_netbuf_put(pst_delba, (us_frame_len + OAL_SIZEOF(dmac_ctx_action_event_stru)));

    hmac_chr_set_del_ba_info(uc_tidno, (uint16_t)pst_action_args->ul_arg3);
    /* 初始化CB */
    memset_s(oal_netbuf_cb(pst_delba), oal_netbuf_cb_size(), 0, oal_netbuf_cb_size());
    pst_tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_delba);
    MAC_GET_CB_MPDU_LEN(pst_tx_ctl) = us_frame_len + OAL_SIZEOF(dmac_ctx_action_event_stru);
    MAC_GET_CB_FRAME_TYPE(pst_tx_ctl) = WLAN_CB_FRAME_TYPE_ACTION;
    MAC_GET_CB_FRAME_SUBTYPE(pst_tx_ctl) = WLAN_ACTION_BA_DELBA;

    /* 抛事件，到DMAC模块发送 */
    pst_event_mem = frw_event_alloc_m(OAL_SIZEOF(dmac_tx_event_stru));
    if (pst_event_mem == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_BA, "{hmac_mgmt_tx_delba::pst_event_mem null.}");
        /* 释放管理帧内存到netbuf内存池 */
        oal_netbuf_free(pst_delba);
        /* 对tid对应的tx BA会话状态解锁 */
        oal_spin_unlock_bh(&(pst_hmac_user->ast_tid_info[uc_tidno].st_ba_tx_info.st_ba_status_lock));
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 获得事件指针 */
    pst_hmac_to_dmac_ctx_event = frw_get_event_stru(pst_event_mem);

    /* 填写事件头 */
    frw_event_hdr_init(&(pst_hmac_to_dmac_ctx_event->st_event_hdr),
                       FRW_EVENT_TYPE_WLAN_CTX,
                       DMAC_WLAN_CTX_EVENT_SUB_TYPE_MGMT,
                       OAL_SIZEOF(dmac_tx_event_stru),
                       FRW_EVENT_PIPELINE_STAGE_1,
                       pst_mac_vap->uc_chip_id,
                       pst_mac_vap->uc_device_id,
                       pst_mac_vap->uc_vap_id);

    /* 填写事件payload */
    pst_tx_event = (dmac_tx_event_stru *)(pst_hmac_to_dmac_ctx_event->auc_event_data);
    pst_tx_event->pst_netbuf = pst_delba;
    pst_tx_event->us_frame_len = us_frame_len + OAL_SIZEOF(dmac_ctx_action_event_stru);

    /* 分发 */
    ul_ret = frw_event_dispatch_event(pst_event_mem);
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_BA,
                       "{hmac_mgmt_tx_delba::frw_event_dispatch_event failed[%d].}", ul_ret);
        oal_netbuf_free(pst_delba);
        frw_event_free_m(pst_event_mem);
        oal_spin_unlock_bh(&(pst_hmac_user->ast_tid_info[uc_tidno].st_ba_tx_info.st_ba_status_lock));
        return ul_ret;
    }

    frw_event_free_m(pst_event_mem);

    if (en_initiator == MAC_RECIPIENT_DELBA) {
        /* 更新对应的TID信息 */
        hmac_ba_reset_rx_handle(pst_device, pst_hmac_user, uc_tidno, OAL_FALSE);
    } else {
        /* 更新对应的TID信息 */
        pst_hmac_user->ast_tid_info[uc_tidno].st_ba_tx_info.en_ba_status = DMAC_BA_INIT;
        pst_hmac_user->auc_ba_flag[uc_tidno] = 0;

        hmac_tx_ba_session_decr(pst_hmac_vap, pst_hmac_user->ast_tid_info[uc_tidno].uc_tid_no);
        /* 还原设置AMPDU下AMSDU的支持情况 */
        HMAC_USER_SET_AMSDU_SUPPORT(pst_hmac_user, uc_tidno);
    }

    /* 对tid对应的tx BA会话状态解锁 */
    oal_spin_unlock_bh(&(pst_hmac_user->ast_tid_info[uc_tidno].st_ba_tx_info.st_ba_status_lock));

    return OAL_SUCC;
}


uint32_t hmac_mgmt_rx_addba_req(hmac_vap_stru *pst_hmac_vap,
    hmac_user_stru *pst_hmac_user, uint8_t *puc_payload)
{
    mac_device_stru *pst_device = OAL_PTR_NULL;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    uint8_t uc_tid;
    uint8_t uc_status;
    hmac_ba_rx_stru *pst_ba_rx_stru = OAL_PTR_NULL;
    uint32_t ul_ret;

    if (oal_any_null_ptr3(pst_hmac_vap, pst_hmac_user, puc_payload)) {
        oam_error_log3(0, OAM_SF_BA, "{hmac_mgmt_rx_addba_req:: null param, 0x%x 0x%x 0x%x.}",
            (uintptr_t)pst_hmac_vap, (uintptr_t)pst_hmac_user, (uintptr_t)puc_payload);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_mac_vap = &(pst_hmac_vap->st_vap_base_info);

    /* 11n以上能力才可接收ampdu */
    if ((!(pst_mac_vap->en_protocol >= WLAN_HT_MODE)) ||
        (!(pst_hmac_user->st_user_base_info.en_protocol_mode >= WLAN_HT_MODE))) {
        oam_warning_log2(pst_mac_vap->uc_vap_id, OAM_SF_BA,
            "{hmac_mgmt_rx_addba_req::ampdu not supprot or not open,vap protocol mode:%d,user protocol mode:%d.}",
            pst_mac_vap->en_protocol, pst_hmac_user->st_user_base_info.en_protocol_mode);
        return OAL_SUCC;
    }

    /* 获取device结构 */
    pst_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (pst_device == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_BA,
                       "{hmac_mgmt_rx_addba_req::addba req receive failed, pst_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /******************************************************************/
    /*       ADDBA Request Frame - Frame Body                         */
    /* --------------------------------------------------------------- */
    /* | Category | Action | Dialog | Parameters | Timeout | SSN     | */
    /* --------------------------------------------------------------- */
    /* | 1        | 1      | 1      | 2          | 2       | 2       | */
    /* --------------------------------------------------------------- */
    /*                                                                */
    /******************************************************************/
    uc_tid = (puc_payload[3] & 0x3C) >> 2;
    if (uc_tid >= WLAN_TID_MAX_NUM) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_BA,
                         "{hmac_mgmt_rx_addba_req::addba req receive failed, tid %d overflow.}", uc_tid);
        return OAL_ERR_CODE_ARRAY_OVERFLOW;
    }

    if (pst_hmac_user->ast_tid_info[uc_tid].pst_ba_rx_info != OAL_PTR_NULL) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_BA,
                         "{hmac_mgmt_rx_addba_req::addba req received, but tid [%d] already set up.}", uc_tid);
        hmac_ba_reset_rx_handle(pst_device, pst_hmac_user, uc_tid, OAL_FALSE);
    }

    /* ADDBA REQ baw size来区分重排序队列是64还是256 */
    pst_ba_rx_stru = (hmac_ba_rx_stru *)oal_mem_alloc_m(OAL_MEM_POOL_ID_LOCAL,
                                                        (uint16_t)OAL_SIZEOF(hmac_ba_rx_stru), OAL_TRUE);
    if (pst_ba_rx_stru == OAL_PTR_NULL) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_BA,
            "{hmac_mgmt_rx_addba_req::alloc fail.tid[%d],baw[%d]}", uc_tid);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hmac_user->ast_tid_info[uc_tid].pst_ba_rx_info = pst_ba_rx_stru;

    /* Ba会话参数初始化 */
    pst_ba_rx_stru->en_ba_status = DMAC_BA_INIT;
    pst_ba_rx_stru->uc_dialog_token = puc_payload[2];
    pst_ba_rx_stru->uc_ba_policy = (puc_payload[3] & 0x02) >> 1;
    pst_ba_rx_stru->us_ba_timeout = puc_payload[5] | (puc_payload[6] << 8);
    /* 初始化接收窗口 */
    pst_ba_rx_stru->us_baw_start = (puc_payload[7] >> 4) | (puc_payload[8] << 4);
    pst_ba_rx_stru->us_baw_size = (puc_payload[3] & 0xC0) >> 6;
    pst_ba_rx_stru->us_baw_size |= (puc_payload[4] << 2);

    wlan_chip_ba_rx_hdl_init(pst_hmac_vap, pst_hmac_user, uc_tid);

    /* profiling测试中，接收端不删除ba */
    frw_timer_create_timer_m(&(pst_hmac_user->ast_tid_info[uc_tid].st_ba_timer), hmac_ba_timeout_fn,
        pst_hmac_vap->us_rx_timeout[WLAN_WME_TID_TO_AC(uc_tid)],
        &(pst_ba_rx_stru->st_alarm_data), OAL_FALSE, OAM_MODULE_ID_HMAC, pst_device->ul_core_id);

    mac_mib_incr_RxBASessionNumber(pst_mac_vap);

    /* 判断建立能否成功 */
    uc_status = hmac_mgmt_check_set_rx_ba_ok(pst_hmac_vap, pst_hmac_user, pst_ba_rx_stru, pst_device,
        &(pst_hmac_user->ast_tid_info[uc_tid]));
    if (uc_status == MAC_SUCCESSFUL_STATUSCODE) {
        pst_hmac_user->ast_tid_info[uc_tid].pst_ba_rx_info->en_ba_status = DMAC_BA_INPROGRESS;
    }

    ul_ret = hmac_mgmt_tx_addba_rsp(pst_hmac_vap, pst_hmac_user, pst_ba_rx_stru, uc_tid, uc_status);

    oam_warning_log4(pst_mac_vap->uc_vap_id, OAM_SF_BA,
        "{hmac_mgmt_rx_addba_req::process addba req receive and send addba rsp, uc_tid[%d], \
        uc_status[%d], baw_size[%d], RXAMSDU[%d]}\r\n",
        uc_tid, uc_status, pst_ba_rx_stru->us_baw_size, pst_ba_rx_stru->en_amsdu_supp);
    if ((uc_status != MAC_SUCCESSFUL_STATUSCODE) || (ul_ret != 0)) {
        /* pst_hmac_user->ast_tid_info[uc_tid].pst_ba_rx_info修改为在函数中置空，与其他调用一致 */
        hmac_ba_reset_rx_handle(pst_device, pst_hmac_user, uc_tid, OAL_FALSE);
    }

    return OAL_SUCC;
}


uint32_t hmac_mgmt_rx_addba_rsp(hmac_vap_stru *pst_hmac_vap,
    hmac_user_stru *pst_hmac_user, uint8_t *puc_payload)
{
    mac_device_stru *pst_mac_device = OAL_PTR_NULL;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;
    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL; /* 申请事件返回的内存指针 */
    frw_event_stru *pst_hmac_to_dmac_crx_sync = OAL_PTR_NULL;
    dmac_ctx_action_event_stru *pst_rx_addba_rsp_event = OAL_PTR_NULL;
    uint8_t uc_tidno;
    hmac_tid_stru *pst_tid = OAL_PTR_NULL;
    uint8_t uc_dialog_token;
    uint8_t uc_ba_policy;
    uint16_t us_baw_size;
    uint8_t uc_ba_status;
    oal_bool_enum_uint8 en_amsdu_supp;

    if (oal_any_null_ptr3(pst_hmac_vap, pst_hmac_user, puc_payload)) {
        oam_error_log3(0, OAM_SF_BA, "{hmac_mgmt_rx_addba_rsp::null param, %x %x %x.}",
                       (uintptr_t)pst_hmac_vap, (uintptr_t)pst_hmac_user, (uintptr_t)puc_payload);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_mac_vap = &(pst_hmac_vap->st_vap_base_info);

    /* 获取device结构 */
    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (oal_unlikely(pst_mac_device == OAL_PTR_NULL)) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_BA, "{hmac_mgmt_rx_addba_rsp::pst_mac_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /******************************************************************/
    /*       ADDBA Response Frame - Frame Body                        */
    /* --------------------------------------------------------------- */
    /* | Category | Action | Dialog | Status  | Parameters | Timeout | */
    /* --------------------------------------------------------------- */
    /* | 1        | 1      | 1      | 2       | 2          | 2       | */
    /* --------------------------------------------------------------- */
    /*                                                                */
    /******************************************************************/
    uc_tidno = (puc_payload[5] & 0x3C) >> 2;
    /* 协议支持tid为0~15,02只支持tid0~7 */
    if (uc_tidno >= WLAN_TID_MAX_NUM) {
        /* 对于tid > 7的resp直接忽略 */
        oam_warning_log3(pst_mac_vap->uc_vap_id, OAM_SF_BA,
                         "{hmac_mgmt_rx_addba_rsp::addba rsp tid[%d]} token[%d] state[%d]",
                         uc_tidno, puc_payload[2], puc_payload[3]);
        return OAL_SUCC;
    }

    uc_dialog_token = puc_payload[2];
    uc_ba_status = puc_payload[3];
    uc_ba_policy = ((puc_payload[5] & 0x02) >> 1);
    en_amsdu_supp = puc_payload[5] & BIT0;

    pst_tid = &(pst_hmac_user->ast_tid_info[uc_tidno]);

    /* 对tid对应的tx BA会话状态加锁 */
    oal_spin_lock_bh(&(pst_hmac_user->ast_tid_info[uc_tidno].st_ba_tx_info.st_ba_status_lock));

    /* BA状态成功，但token、policy不匹配，需要删除聚合 */
    
    if ((pst_tid->st_ba_tx_info.en_ba_status == DMAC_BA_INPROGRESS) && (uc_ba_status == MAC_SUCCESSFUL_STATUSCODE)) {
        if ((uc_dialog_token != pst_tid->st_ba_tx_info.uc_dialog_token) ||
            (uc_ba_policy != pst_tid->st_ba_tx_info.uc_ba_policy)) {
            /* 对tid对应的tx BA会话状态解锁 */
            oal_spin_unlock_bh(&(pst_hmac_user->ast_tid_info[uc_tidno].st_ba_tx_info.st_ba_status_lock));
            oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_BA,
                "{hmac_mgmt_rx_addba_rsp::addba rsp tid[%d],status SUCC,but token/policy wr}", uc_tidno);
            oam_warning_log4(pst_mac_vap->uc_vap_id, OAM_SF_BA,
                "{hmac_mgmt_rx_addba_rsp::rsp policy[%d],req policy[%d], rsp dialog[%d], req dialog[%d]}",
                uc_ba_policy, pst_tid->st_ba_tx_info.uc_ba_policy,
                uc_dialog_token, pst_tid->st_ba_tx_info.uc_dialog_token);
            return OAL_SUCC;
        }
    }

    /* 停止计时器 */
    if (pst_tid->st_ba_tx_info.st_addba_timer.en_is_registerd == OAL_TRUE) {
        frw_timer_immediate_destroy_timer_m(&pst_tid->st_ba_tx_info.st_addba_timer);
    }

    if (pst_tid->st_ba_tx_info.en_ba_status == DMAC_BA_INIT) {
        /* 对tid对应的tx BA会话状态解锁 */
        oal_spin_unlock_bh(&(pst_hmac_user->ast_tid_info[uc_tidno].st_ba_tx_info.st_ba_status_lock));
        hmac_tx_ba_del(pst_hmac_vap, pst_hmac_user, uc_tidno);
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_BA,
            "{hmac_mgmt_rx_addba_rsp::addba rsp is received when ba status is DMAC_BA_INIT.tid[%d]}", uc_tidno);
        return OAL_SUCC;
    }

    if (pst_tid->st_ba_tx_info.en_ba_status == DMAC_BA_COMPLETE) {
        oam_warning_log2(pst_mac_vap->uc_vap_id, OAM_SF_BA,
            "{hmac_mgmt_rx_addba_rsp::addba rsp is received when ba status is \
            DMAC_BA_COMPLETE or uc_dialog_token wrong. tid[%d], status[%d]}",
            uc_tidno, pst_tid->st_ba_tx_info.en_ba_status);
        /* 对tid对应的tx BA会话状态解锁 */
        oal_spin_unlock_bh(&(pst_hmac_user->ast_tid_info[uc_tidno].st_ba_tx_info.st_ba_status_lock));
        return OAL_SUCC;
    }

    /* 抛事件到DMAC处理 */
    pst_event_mem = frw_event_alloc_m(OAL_SIZEOF(dmac_ctx_action_event_stru));
    if (pst_event_mem == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_BA, "{hmac_mgmt_rx_addba_rsp::pst_event_mem null.}");
        oal_spin_unlock_bh(&(pst_hmac_user->ast_tid_info[uc_tidno].st_ba_tx_info.st_ba_status_lock));
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 获得事件指针 */
    pst_hmac_to_dmac_crx_sync = frw_get_event_stru(pst_event_mem);

    /* 填写事件头 */
    frw_event_hdr_init(&(pst_hmac_to_dmac_crx_sync->st_event_hdr),
                       FRW_EVENT_TYPE_WLAN_CTX,
                       DMAC_WLAN_CTX_EVENT_SUB_TYPE_BA_SYNC,
                       OAL_SIZEOF(dmac_ctx_action_event_stru),
                       FRW_EVENT_PIPELINE_STAGE_1,
                       pst_mac_vap->uc_chip_id,
                       pst_mac_vap->uc_device_id,
                       pst_mac_vap->uc_vap_id);

    /* 获取帧体信息，由于DMAC的同步，填写事件payload */
    pst_rx_addba_rsp_event = (dmac_ctx_action_event_stru *)(pst_hmac_to_dmac_crx_sync->auc_event_data);
    pst_rx_addba_rsp_event->en_action_category = MAC_ACTION_CATEGORY_BA;
    pst_rx_addba_rsp_event->uc_action = MAC_BA_ACTION_ADDBA_RSP;
    pst_rx_addba_rsp_event->us_user_idx = pst_hmac_user->st_user_base_info.us_assoc_id;
    pst_rx_addba_rsp_event->uc_status = uc_ba_status;
    pst_rx_addba_rsp_event->uc_tidno = uc_tidno;
    pst_rx_addba_rsp_event->uc_dialog_token = uc_dialog_token;
    pst_rx_addba_rsp_event->uc_ba_policy = uc_ba_policy;
    pst_rx_addba_rsp_event->us_ba_timeout = puc_payload[7] | (puc_payload[8] << 8);
    pst_rx_addba_rsp_event->en_amsdu_supp = en_amsdu_supp;

    us_baw_size = (uint16_t)(((puc_payload[5] & 0xC0) >> 6) | (puc_payload[6] << 2));
    if ((us_baw_size == 0) || (us_baw_size > WLAN_AMPDU_TX_MAX_BUF_SIZE)) {
        us_baw_size = WLAN_AMPDU_TX_MAX_BUF_SIZE;
    }

    pst_rx_addba_rsp_event->us_baw_size = us_baw_size;

    /* 分发 */
    frw_event_dispatch_event(pst_event_mem);

    /* 释放事件内存 */
    frw_event_free_m(pst_event_mem);

    /* 先抛事件，再处理host BA句柄，防止异步发送ADDBA REQ */
    if (uc_ba_status == MAC_SUCCESSFUL_STATUSCODE) {
        /* 设置hmac模块对应的BA句柄的信息 */
        pst_tid->st_ba_tx_info.en_ba_status = DMAC_BA_COMPLETE;
        pst_tid->st_ba_tx_info.uc_addba_attemps = 0;

        /*lint -e502*/
        if (en_amsdu_supp && mac_mib_get_AmsduPlusAmpduActive(pst_mac_vap)) {
            HMAC_USER_SET_AMSDU_SUPPORT(pst_hmac_user, uc_tidno);
        } else {
            HMAC_USER_SET_AMSDU_NOT_SUPPORT(pst_hmac_user, uc_tidno);
        }
        /*lint +e502*/
    } else { /* BA被拒绝 */
        oam_warning_log2(pst_mac_vap->uc_vap_id, OAM_SF_BA,
            "{hmac_mgmt_rx_addba_rsp::addba rsp  status err[%d].tid[%d],DEL BA.}", uc_ba_status, uc_tidno);

        hmac_tx_ba_session_decr(pst_hmac_vap, uc_tidno);

        /* 先抛事件删除dmac旧BA句柄后，再重置HMAC模块信息，确保删除dmac ba事件在下一次ADDBA REQ事件之前到达dmac */
        pst_tid->st_ba_tx_info.en_ba_status = DMAC_BA_INIT;
    }

    /* 对tid对应的tx BA会话状态解锁 */
    oal_spin_unlock_bh(&(pst_hmac_user->ast_tid_info[uc_tidno].st_ba_tx_info.st_ba_status_lock));

    return OAL_SUCC;
}


uint32_t hmac_mgmt_rx_delba(hmac_vap_stru *pst_hmac_vap,
    hmac_user_stru *pst_hmac_user, uint8_t *puc_payload)
{
    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL; /* 申请事件返回的内存指针 */
    frw_event_stru *pst_hmac_to_dmac_crx_sync = OAL_PTR_NULL;
    dmac_ctx_action_event_stru *pst_wlan_crx_action = OAL_PTR_NULL;
    mac_device_stru *pst_device = OAL_PTR_NULL;
    hmac_tid_stru *pst_tid = OAL_PTR_NULL;
    uint8_t uc_tid;
    uint8_t uc_initiator;
    uint16_t us_reason;

    if (oal_unlikely(oal_any_null_ptr3(pst_hmac_vap, pst_hmac_user, puc_payload))) {
        oam_error_log3(0, OAM_SF_BA, "{hmac_mgmt_rx_delba::null param, %x %x %x.}",
                       (uintptr_t)pst_hmac_vap, (uintptr_t)pst_hmac_user, (uintptr_t)puc_payload);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 获取device结构 */
    pst_device = mac_res_get_dev(pst_hmac_vap->st_vap_base_info.uc_device_id);
    if (oal_unlikely(pst_device == OAL_PTR_NULL)) {
        oam_error_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_BA,
                       "{hmac_mgmt_rx_delba::pst_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /************************************************/
    /*       DELBA Response Frame - Frame Body      */
    /* -------------------------------------------- */
    /* | Category | Action | Parameters | Reason  | */
    /* -------------------------------------------- */
    /* | 1        | 1      | 2          | 2       | */
    /* -------------------------------------------- */
    /*                                              */
    /************************************************/
    uc_tid = (puc_payload[3] & 0xF0) >> 4;
    uc_initiator = (puc_payload[3] & 0x08) >> 3;
    us_reason = (puc_payload[4] & 0xFF) | ((puc_payload[5] << 8) & 0xFF00);

    /* tid保护，避免数组越界 */
    if (uc_tid >= WLAN_TID_MAX_NUM) {
        oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_BA,
                         "{hmac_mgmt_rx_delba::delba receive failed, tid %d overflow.}", uc_tid);
        return OAL_ERR_CODE_ARRAY_OVERFLOW;
    }
    pst_tid = &(pst_hmac_user->ast_tid_info[uc_tid]);

    oam_warning_log3(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_BA,
                     "{hmac_mgmt_rx_delba::receive delba from peer sta, tid[%d], uc_initiator[%d], reason[%d].}",
                     uc_tid, uc_initiator, us_reason);

    /* 对tid对应的tx BA会话状态加锁 */
    oal_spin_lock_bh(&(pst_tid->st_ba_tx_info.st_ba_status_lock));

    /* 重置BA发送会话 */
    if (uc_initiator == MAC_RECIPIENT_DELBA) {
        if (pst_tid->st_ba_tx_info.en_ba_status == DMAC_BA_INIT) {
            oal_spin_unlock_bh(&(pst_tid->st_ba_tx_info.st_ba_status_lock));
            return OAL_SUCC;
        }

        pst_hmac_user->auc_ba_flag[uc_tid] = 0;

        /* 还原设置AMPDU下AMSDU的支持情况 */
        HMAC_USER_SET_AMSDU_SUPPORT(pst_hmac_user, uc_tid);

        hmac_tx_ba_session_decr(pst_hmac_vap, pst_hmac_user->ast_tid_info[uc_tid].uc_tid_no);
    } else { /* 重置BA接收会话 */
        hmac_ba_reset_rx_handle(pst_device, pst_hmac_user, uc_tid, OAL_FALSE);
        oam_info_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_BA,
                      "{hmac_mgmt_rx_delba::rcv rx dir del ba.}\r\n");
    }

    /* 抛事件到DMAC处理 */
    pst_event_mem = frw_event_alloc_m(OAL_SIZEOF(dmac_ctx_action_event_stru));
    if (pst_event_mem == OAL_PTR_NULL) {
        oam_error_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_BA,
                       "{hmac_mgmt_rx_delba::pst_event_mem null.}");
        oal_spin_unlock_bh(&(pst_tid->st_ba_tx_info.st_ba_status_lock));
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 获得事件指针 */
    pst_hmac_to_dmac_crx_sync = frw_get_event_stru(pst_event_mem);

    /* 填写事件头 */
    frw_event_hdr_init(&(pst_hmac_to_dmac_crx_sync->st_event_hdr),
                       FRW_EVENT_TYPE_WLAN_CTX,
                       DMAC_WLAN_CTX_EVENT_SUB_TYPE_BA_SYNC,
                       OAL_SIZEOF(dmac_ctx_action_event_stru),
                       FRW_EVENT_PIPELINE_STAGE_1,
                       pst_hmac_vap->st_vap_base_info.uc_chip_id,
                       pst_hmac_vap->st_vap_base_info.uc_device_id,
                       pst_hmac_vap->st_vap_base_info.uc_vap_id);

    /* 填写事件payload */
    pst_wlan_crx_action = (dmac_ctx_action_event_stru *)(pst_hmac_to_dmac_crx_sync->auc_event_data);
    pst_wlan_crx_action->en_action_category = MAC_ACTION_CATEGORY_BA;
    pst_wlan_crx_action->uc_action = MAC_BA_ACTION_DELBA;
    pst_wlan_crx_action->us_user_idx = pst_hmac_user->st_user_base_info.us_assoc_id;

    pst_wlan_crx_action->uc_tidno = uc_tid;
    pst_wlan_crx_action->uc_initiator = uc_initiator;

    /* 分发 */
    frw_event_dispatch_event(pst_event_mem);

    /* 释放事件内存 */
    frw_event_free_m(pst_event_mem);

    /* DELBA事件先处理再改状态,防止addba req先处理 */
    if (uc_initiator == MAC_RECIPIENT_DELBA) {
        pst_tid->st_ba_tx_info.en_ba_status = DMAC_BA_INIT;
    }

    /* 对tid对应的tx BA会话状态解锁 */
    oal_spin_unlock_bh(&(pst_tid->st_ba_tx_info.st_ba_status_lock));

    return OAL_SUCC;
}


uint32_t hmac_mgmt_tx_addba_timeout(void *arg)
{
    hmac_vap_stru *vap = OAL_PTR_NULL; /* vap指针 */
    uint8_t *usr_dst_mac = OAL_PTR_NULL; /* 保存用户目的地址的指针 */
    hmac_user_stru *hmac_user = OAL_PTR_NULL;
    mac_action_mgmt_args_stru action_args;
    dmac_ba_alarm_stru *alarm_data = OAL_PTR_NULL;
    uint32_t ret;

    if (arg == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_BA, "{hmac_mgmt_tx_addba_timeout::p_arg null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    alarm_data = (dmac_ba_alarm_stru *)arg;
    hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(alarm_data->us_mac_user_idx);
    if (hmac_user == OAL_PTR_NULL) {
        oam_warning_log1(0, OAM_SF_BA, "{hmac_mgmt_tx_addba_timeout::pst_hmac_user[%d] null.}",
                         alarm_data->us_mac_user_idx);
        return OAL_ERR_CODE_PTR_NULL;
    }

    usr_dst_mac = hmac_user->st_user_base_info.auc_user_mac_addr;
    vap = (hmac_vap_stru *)mac_res_get_hmac_vap(alarm_data->uc_vap_id);
    if (oal_unlikely(vap == OAL_PTR_NULL)) {
        oam_error_log0(hmac_user->st_user_base_info.uc_vap_id, OAM_SF_BA,
                       "{hmac_mgmt_tx_addba_timeout::pst_hmac_user null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 生成DELBA帧 */
    action_args.uc_category = MAC_ACTION_CATEGORY_BA;
    action_args.uc_action = MAC_BA_ACTION_DELBA;
    action_args.ul_arg1 = alarm_data->uc_tid; /* 该数据帧对应的TID号 */
    action_args.ul_arg2 = MAC_ORIGINATOR_DELBA; /* DELBA中，触发删除BA会话的发起端 */
    action_args.ul_arg3 = MAC_QSTA_TIMEOUT; /* DELBA中代表删除reason */
    action_args.puc_arg5 = usr_dst_mac; /* DELBA中代表目的地址 */

    oam_warning_log1(hmac_user->st_user_base_info.uc_vap_id, OAM_SF_BA,
        "{hmac_mgmt_tx_addba_timeout::TID[%d] add timeout.}", alarm_data->uc_tid);
    ret = hmac_mgmt_tx_delba(vap, hmac_user, &action_args);

    return ret;
}


uint32_t hmac_mgmt_tx_ampdu_start(hmac_vap_stru *pst_hmac_vap,
    hmac_user_stru *pst_hmac_user, mac_priv_req_args_stru *pst_priv_req)
{
    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL; /* 申请事件返回的内存指针 */
    frw_event_stru *pst_crx_priv_req_event = OAL_PTR_NULL;
    mac_priv_req_args_stru *pst_rx_ampdu_start_event = OAL_PTR_NULL;
    uint8_t uc_tidno;
    hmac_tid_stru *pst_tid = OAL_PTR_NULL;
    uint32_t ul_ret;

    if (oal_any_null_ptr3(pst_hmac_vap, pst_hmac_user, pst_priv_req)) {
        oam_error_log3(0, OAM_SF_AMPDU, "{hmac_mgmt_tx_ampdu_start::param null, %x %x %x.}",
                       (uintptr_t)pst_hmac_vap, (uintptr_t)pst_hmac_user, (uintptr_t)pst_priv_req);
        return OAL_ERR_CODE_PTR_NULL;
    }

    uc_tidno = pst_priv_req->uc_arg1;
    pst_tid = &(pst_hmac_user->ast_tid_info[uc_tidno]);

    /* AMPDU为NORMAL ACK时，对应的BA会话没有建立，则返回 */
    if (pst_priv_req->uc_arg3 == WLAN_TX_NORMAL_ACK) {
        if (pst_tid->st_ba_tx_info.en_ba_status == DMAC_BA_INIT) {
            oam_info_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_AMPDU,
                          "{hmac_mgmt_tx_ampdu_start::normal ack but ba session is not build.}");
            return OAL_SUCC;
        }
    }

    /* 抛事件到DMAC处理 */
    pst_event_mem = frw_event_alloc_m(OAL_SIZEOF(mac_priv_req_args_stru));
    if (pst_event_mem == OAL_PTR_NULL) {
        oam_error_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_AMPDU,
                       "{hmac_mgmt_tx_ampdu_start::pst_event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 获得事件指针 */
    pst_crx_priv_req_event = frw_get_event_stru(pst_event_mem);

    /* 填写事件头 */
    frw_event_hdr_init(&(pst_crx_priv_req_event->st_event_hdr),
                       FRW_EVENT_TYPE_WLAN_CTX,
                       DMAC_WLAN_CTX_EVENT_SUB_TYPE_PRIV_REQ,
                       OAL_SIZEOF(mac_priv_req_args_stru),
                       FRW_EVENT_PIPELINE_STAGE_1,
                       pst_hmac_vap->st_vap_base_info.uc_chip_id,
                       pst_hmac_vap->st_vap_base_info.uc_device_id,
                       pst_hmac_vap->st_vap_base_info.uc_vap_id);

    /* 获取设置AMPDU的参数，到dmac进行设置 */
    pst_rx_ampdu_start_event = (mac_priv_req_args_stru *)(pst_crx_priv_req_event->auc_event_data);
    pst_rx_ampdu_start_event->uc_type = MAC_A_MPDU_START;
    pst_rx_ampdu_start_event->uc_arg1 = pst_priv_req->uc_arg1;
    pst_rx_ampdu_start_event->uc_arg2 = pst_priv_req->uc_arg2;
    pst_rx_ampdu_start_event->uc_arg3 = pst_priv_req->uc_arg3;
    pst_rx_ampdu_start_event->us_user_idx = pst_hmac_user->st_user_base_info.us_assoc_id; /* 保存的是资源池的索引 */

    /* 分发 */
    ul_ret = frw_event_dispatch_event(pst_event_mem);

    /* 释放事件内存 */
    frw_event_free_m(pst_event_mem);

    return ul_ret;
}


uint32_t hmac_mgmt_tx_ampdu_end(hmac_vap_stru *pst_hmac_vap,
    hmac_user_stru *pst_hmac_user, mac_priv_req_args_stru *pst_priv_req)
{
    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL; /* 申请事件返回的内存指针 */
    frw_event_stru *pst_crx_priv_req_event = OAL_PTR_NULL;
    mac_priv_req_args_stru *pst_rx_ampdu_end_event = OAL_PTR_NULL;
    uint32_t ul_ret;

    if (oal_any_null_ptr3(pst_hmac_vap, pst_hmac_user, pst_priv_req)) {
        oam_error_log3(0, OAM_SF_AMPDU, "{hmac_mgmt_tx_ampdu_end::param null, %d %d %d.}",
                       (uintptr_t)pst_hmac_vap, (uintptr_t)pst_hmac_user, (uintptr_t)pst_priv_req);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 抛事件到DMAC处理 */
    pst_event_mem = frw_event_alloc_m(OAL_SIZEOF(mac_priv_req_args_stru));
    if (pst_event_mem == OAL_PTR_NULL) {
        oam_error_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_AMPDU,
                       "{hmac_mgmt_tx_ampdu_end::pst_event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 获得事件指针 */
    pst_crx_priv_req_event = frw_get_event_stru(pst_event_mem);

    /* 填写事件头 */
    frw_event_hdr_init(&(pst_crx_priv_req_event->st_event_hdr),
                       FRW_EVENT_TYPE_WLAN_CTX,
                       DMAC_WLAN_CTX_EVENT_SUB_TYPE_PRIV_REQ,
                       OAL_SIZEOF(mac_priv_req_args_stru),
                       FRW_EVENT_PIPELINE_STAGE_1,
                       pst_hmac_vap->st_vap_base_info.uc_chip_id,
                       pst_hmac_vap->st_vap_base_info.uc_device_id,
                       pst_hmac_vap->st_vap_base_info.uc_vap_id);

    /* 获取设置AMPDU的参数，到dmac进行设置 */
    pst_rx_ampdu_end_event = (mac_priv_req_args_stru *)(pst_crx_priv_req_event->auc_event_data);
    pst_rx_ampdu_end_event->uc_type = MAC_A_MPDU_END;                                   /* 类型 */
    pst_rx_ampdu_end_event->uc_arg1 = pst_priv_req->uc_arg1;                            /* tid no */
    pst_rx_ampdu_end_event->us_user_idx = pst_hmac_user->st_user_base_info.us_assoc_id; /* 保存的是资源池的索引 */

    /* 分发 */
    ul_ret = frw_event_dispatch_event(pst_event_mem);

    /* 释放事件内存 */
    frw_event_free_m(pst_event_mem);

    return ul_ret;
}


uint32_t hmac_tx_mgmt_send_event(mac_vap_stru *pst_vap, oal_netbuf_stru *pst_mgmt_frame, uint16_t us_frame_len)
{
    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL;
    frw_event_stru *pst_event = OAL_PTR_NULL;
    uint32_t ul_return;
    dmac_tx_event_stru *pst_ctx_stru = OAL_PTR_NULL;

    if (pst_vap == OAL_PTR_NULL || pst_mgmt_frame == OAL_PTR_NULL) {
        oam_error_log2(0, OAM_SF_SCAN, "{hmac_tx_mgmt_send_event::param null, %x %x.}",
                       (uintptr_t)pst_vap, (uintptr_t)pst_mgmt_frame);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 抛事件给DMAC,让DMAC完成配置VAP创建 */
    pst_event_mem = frw_event_alloc_m(OAL_SIZEOF(dmac_tx_event_stru));
    if (oal_unlikely(pst_event_mem == OAL_PTR_NULL)) {
        oam_error_log0(pst_vap->uc_vap_id, OAM_SF_SCAN, "{hmac_tx_mgmt_send_event::pst_event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_event = frw_get_event_stru(pst_event_mem);

    /* 填写事件头 */
    frw_event_hdr_init(&(pst_event->st_event_hdr), FRW_EVENT_TYPE_WLAN_CTX,
                       DMAC_WLAN_CTX_EVENT_SUB_TYPE_MGMT,
                       OAL_SIZEOF(dmac_tx_event_stru),
                       FRW_EVENT_PIPELINE_STAGE_1,
                       pst_vap->uc_chip_id,
                       pst_vap->uc_device_id,
                       pst_vap->uc_vap_id);

    pst_ctx_stru = (dmac_tx_event_stru *)pst_event->auc_event_data;
    pst_ctx_stru->pst_netbuf = pst_mgmt_frame;
    pst_ctx_stru->us_frame_len = us_frame_len;
#ifdef _PRE_WLAN_FEATURE_SNIFFER
#ifdef CONFIG_HW_SNIFFER
    proc_sniffer_write_file(NULL, 0, (uint8_t *)(oal_netbuf_header(pst_mgmt_frame)), us_frame_len, 1);
#endif
#endif

    ul_return = frw_event_dispatch_event(pst_event_mem);
    if (ul_return != OAL_SUCC) {
        oam_error_log1(pst_vap->uc_vap_id, OAM_SF_SCAN,
                       "{hmac_tx_mgmt_send_event::frw_event_dispatch_event failed[%d].}", ul_return);
        frw_event_free_m(pst_event_mem);
        return ul_return;
    }

    /* 释放事件 */
    frw_event_free_m(pst_event_mem);

    return OAL_SUCC;
}


uint32_t hmac_mgmt_reset_psm(mac_vap_stru *pst_vap, uint16_t us_user_id)
{
    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL;
    frw_event_stru *pst_event = OAL_PTR_NULL;
    uint16_t *pus_user_id = OAL_PTR_NULL;
    hmac_user_stru *pst_hmac_user = OAL_PTR_NULL;

    if (pst_vap == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_PWR, "{hmac_mgmt_reset_psm::pst_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    /* 在这里直接做重置的一些操作，不需要再次同步 */
    pst_hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(us_user_id);
    if (oal_unlikely(pst_hmac_user == OAL_PTR_NULL)) {
        oam_error_log1(pst_vap->uc_vap_id, OAM_SF_PWR,
                       "{hmac_mgmt_reset_psm::pst_hmac_user[%d] null.}",
                       us_user_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_event_mem = frw_event_alloc_m(OAL_SIZEOF(uint16_t));
    if (oal_unlikely(pst_event_mem == OAL_PTR_NULL)) {
        oam_error_log0(pst_vap->uc_vap_id, OAM_SF_PWR, "{hmac_mgmt_reset_psm::pst_event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_event = frw_get_event_stru(pst_event_mem);

    /* 填写事件头 */
    frw_event_hdr_init(&(pst_event->st_event_hdr),
                       FRW_EVENT_TYPE_WLAN_CTX,
                       DMAC_WLAN_CTX_EVENT_SUB_TYPE_RESET_PSM,
                       OAL_SIZEOF(uint16_t),
                       FRW_EVENT_PIPELINE_STAGE_1,
                       pst_vap->uc_chip_id,
                       pst_vap->uc_device_id,
                       pst_vap->uc_vap_id);

    pus_user_id = (uint16_t *)pst_event->auc_event_data;

    *pus_user_id = us_user_id;

    frw_event_dispatch_event(pst_event_mem);

    frw_event_free_m(pst_event_mem);

    return OAL_SUCC;
}

#if (_PRE_WLAN_FEATURE_PMF != _PRE_PMF_NOT_SUPPORT)
OAL_STATIC uint32_t hmac_sa_query_del_user(mac_vap_stru *pst_mac_vap, hmac_user_stru *pst_hmac_user)
{
    wlan_vap_mode_enum_uint8 en_vap_mode;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    uint32_t ul_ret;
    mac_sa_query_stru *pst_sa_query_info;

    if (oal_any_null_ptr2(pst_mac_vap, pst_hmac_user)) {
        oam_error_log2(0, OAM_SF_PMF, "{hmac_sa_query_del_user::param null, %x %x.}",
                       (uintptr_t)pst_mac_vap, (uintptr_t)pst_hmac_user);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_device_id, OAM_SF_PMF, "{hmac_sa_query_del_user::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* pending SA Query requests 计数器清零 & sa query流程开始时间清零 */
    pst_sa_query_info = &pst_hmac_user->st_sa_query_info;
    pst_sa_query_info->ul_sa_query_count = 0;
    pst_sa_query_info->ul_sa_query_start_time = 0;

    /* 修改 state & 删除 user */
    en_vap_mode = pst_mac_vap->en_vap_mode;
    switch (en_vap_mode) {
        case WLAN_VAP_MODE_BSS_AP: {
            /* 抛事件上报内核，已经去关联某个STA */
            hmac_handle_disconnect_rsp_ap(pst_hmac_vap, pst_hmac_user);
        }
        break;

        case WLAN_VAP_MODE_BSS_STA: {
            /* 上报内核sta已经和某个ap去关联 */
            hmac_sta_handle_disassoc_rsp(pst_hmac_vap, MAC_DEAUTH_LV_SS);
        }
        break;
        default:
            break;
    }

    /* 删除user */
    ul_ret = hmac_user_del(&pst_hmac_vap->st_vap_base_info, pst_hmac_user);
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(pst_mac_vap->uc_device_id, OAM_SF_PMF,
                       "{hmac_sa_query_del_user::hmac_user_del failed[%d].}", ul_ret);
        return OAL_FAIL;
    }
    return OAL_SUCC;
}


OAL_STATIC uint32_t hmac_send_sa_query_req(mac_vap_stru *pst_mac_vap, hmac_user_stru *pst_hmac_user,
    oal_bool_enum_uint8 en_is_protected, uint16_t us_init_trans_id)
{
    uint16_t us_sa_query_len;
    oal_netbuf_stru *pst_sa_query = OAL_PTR_NULL;
    mac_tx_ctl_stru *pst_tx_ctl = OAL_PTR_NULL;
    uint32_t ul_ret;

    if (oal_any_null_ptr2(pst_mac_vap, pst_hmac_user)) {
        oam_error_log2(0, OAM_SF_PMF, "{hmac_send_sa_query_req::param null, %x %x.}",
                       (uintptr_t)pst_mac_vap, (uintptr_t)pst_hmac_user);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 申请SA Query 帧空间 */
    pst_sa_query = (oal_netbuf_stru *)oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF,
                                                           WLAN_MEM_NETBUF_SIZE2,
                                                           OAL_NETBUF_PRIORITY_MID);
    if (pst_sa_query == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_PMF, "{hmac_send_sa_query_req::pst_sa_query null.}");
        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }

    /* 封装SA Query request帧 */
    memset_s(oal_netbuf_cb(pst_sa_query), oal_netbuf_cb_size(), 0, oal_netbuf_cb_size());
    us_sa_query_len = hmac_encap_sa_query_req(pst_mac_vap, (uint8_t *)oal_netbuf_header(pst_sa_query),
        pst_hmac_user->st_user_base_info.auc_user_mac_addr, pst_hmac_user->st_sa_query_info.us_sa_query_trans_id);

    /* 单播管理帧加密 */
    if (en_is_protected == OAL_TRUE) {
        mac_set_protectedframe((uint8_t *)oal_netbuf_header(pst_sa_query));
    }

    pst_tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_sa_query); /* 获取cb结构体 */
    MAC_GET_CB_MPDU_LEN(pst_tx_ctl) = us_sa_query_len;           /* dmac发送需要的mpdu长度 */
    /* 发送完成需要获取user结构体 */
    MAC_GET_CB_TX_USER_IDX(pst_tx_ctl) = pst_hmac_user->st_user_base_info.us_assoc_id;

    oal_netbuf_put(pst_sa_query, us_sa_query_len);

    /* Buffer this frame in the Memory Queue for transmission */
    ul_ret = hmac_tx_mgmt_send_event(pst_mac_vap, pst_sa_query, us_sa_query_len);
    if (ul_ret != OAL_SUCC) {
        oal_netbuf_free(pst_sa_query);
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_PMF,
                         "{hmac_send_sa_query_req::hmac_tx_mgmt_send_event failed[%d].}", ul_ret);
        return ul_ret;
    }

    return OAL_SUCC;
}

uint32_t hmac_pmf_check_err_code(mac_user_stru *pst_user_base_info,
    oal_bool_enum_uint8 en_is_protected, uint8_t *puc_mac_hdr)
{
    oal_bool_enum_uint8 en_aim_err_code;
    uint16_t us_err_code;

    us_err_code = oal_make_word16(puc_mac_hdr[MAC_80211_FRAME_LEN], puc_mac_hdr[MAC_80211_FRAME_LEN + 1]);
    en_aim_err_code = ((us_err_code == MAC_NOT_AUTHED) || (us_err_code == MAC_NOT_ASSOCED)) ? OAL_TRUE : OAL_FALSE;

    if ((pst_user_base_info->st_cap_info.bit_pmf_active == OAL_TRUE) &&
        (en_aim_err_code == OAL_TRUE || en_is_protected == OAL_FALSE)) {
        return OAL_SUCC;
    }

    return OAL_FAIL;
}


uint32_t hmac_sa_query_interval_timeout(void *p_arg)
{
    hmac_user_stru *pst_hmac_user;
    hmac_vap_stru *pst_hmac_vap;
    uint32_t ul_relt;
    uint32_t ul_now;
    uint32_t ul_passed;
    uint32_t ul_retry_timeout;

    pst_hmac_user = (hmac_user_stru *)p_arg;
    if (pst_hmac_user == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_sa_query_interval_timeout::invalid param.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_hmac_user->st_user_base_info.uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (pst_hmac_user->st_user_base_info.en_user_asoc_state != MAC_USER_STATE_ASSOC) {
        pst_hmac_user->st_sa_query_info.ul_sa_query_count = 0;
        oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
                         "{hmac_sa_query_interval_timeout::USER UNEXPECTED STATE[%d].}",
                         pst_hmac_user->st_user_base_info.en_user_asoc_state);
        return OAL_SUCC;
    }

    ul_now = (uint32_t)oal_time_get_stamp_ms();
    ul_passed = (uint32_t)oal_time_get_runtime(pst_hmac_user->st_sa_query_info.ul_sa_query_start_time, ul_now);
    if (ul_passed >= mac_mib_get_dot11AssociationSAQueryMaximumTimeout(&(pst_hmac_vap->st_vap_base_info))) {
        /* change state & ul_sa_query_count=0 */
        oam_warning_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
                         "{hmac_sa_query_interval_timeout::Deleting user.}");
        ul_relt = hmac_sa_query_del_user(&(pst_hmac_vap->st_vap_base_info), pst_hmac_user);
        if (ul_relt != OAL_SUCC) {
            oam_error_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
                           "{hmac_sa_query_interval_timeout::hmac_sa_query_del_user failed[%d].}", ul_relt);
            return OAL_ERR_CODE_PMF_SA_QUERY_DEL_USER_FAIL;
        }
        return OAL_SUCC;
    }

    /* 再发送一帧sa query request */
    pst_hmac_user->st_sa_query_info.us_sa_query_trans_id += 1;
    pst_hmac_user->st_sa_query_info.ul_sa_query_count += 1;
    oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
                     "{hmac_sa_query_interval_timeout::SA query, trans_id %d.}",
                     pst_hmac_user->st_sa_query_info.us_sa_query_trans_id);

    ul_retry_timeout = mac_mib_get_dot11AssociationSAQueryRetryTimeout(&(pst_hmac_vap->st_vap_base_info));

    /* 设置间隔定时器 */
    frw_timer_create_timer_m(&(pst_hmac_user->st_sa_query_info.st_sa_query_interval_timer),
        hmac_sa_query_interval_timeout, ul_retry_timeout, (void *)pst_hmac_user, OAL_FALSE, OAM_MODULE_ID_HMAC,
        pst_hmac_vap->st_vap_base_info.ul_core_id);

    ul_relt = hmac_send_sa_query_req(&(pst_hmac_vap->st_vap_base_info),
        pst_hmac_user, pst_hmac_user->st_sa_query_info.en_is_protected,
        pst_hmac_user->st_sa_query_info.us_sa_query_trans_id);
    if (ul_relt != OAL_SUCC) {
        oam_error_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
                       "{hmac_sa_query_interval_timeout::hmac_send_sa_query_req failed[%d].}", ul_relt);
        return OAL_ERR_CODE_PMF_SA_QUERY_REQ_SEND_FAIL;
    }
    return OAL_SUCC;
}


uint32_t hmac_start_sa_query(mac_vap_stru *pst_mac_vap,
    hmac_user_stru *pst_hmac_user, oal_bool_enum_uint8 en_is_protected)
{
    uint32_t ul_retry_timeout;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    uint32_t ul_ret;
    uint16_t us_init_trans_id = 0;

    /* 入参判断 */
    if (oal_any_null_ptr2(pst_mac_vap, pst_hmac_user)) {
        oam_error_log2(0, OAM_SF_ANY, "{hmac_start_sa_query::param null, %x %x.}",
                       (uintptr_t)pst_mac_vap, (uintptr_t)pst_hmac_user);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 判断vap有无pmf能力 */
    if (pst_hmac_user->st_user_base_info.st_cap_info.bit_pmf_active != OAL_TRUE) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{hmac_start_sa_query::bit_pmf_active is down.}");
        return OAL_ERR_CODE_PMF_DISABLED;
    }

    /* 避免重复启动SA Query流程 */
    if (pst_hmac_user->st_sa_query_info.ul_sa_query_count != 0) {
        oam_info_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{hmac_start_sa_query::SA Query is already in process.}");
        return OAL_SUCC;
    }

    /* 获得hmac vap 结构指针 */
    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{hmac_start_sa_query::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    ul_retry_timeout = mac_mib_get_dot11AssociationSAQueryRetryTimeout(pst_mac_vap);

    /* 记录sa query流程开始时间,单位ms */
    pst_hmac_user->st_sa_query_info.ul_sa_query_start_time = (uint32_t)oal_time_get_stamp_ms();
    /* 获得初始trans_id */
    pst_hmac_user->st_sa_query_info.us_sa_query_trans_id = (uint16_t)oal_time_get_stamp_ms();

    pst_hmac_user->st_sa_query_info.ul_sa_query_count = 1;
    pst_hmac_user->st_sa_query_info.en_is_protected = en_is_protected;

    oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                     "{hmac_start_sa_query::SA query, trans_id %d.}",
                     pst_hmac_user->st_sa_query_info.us_sa_query_trans_id);

    /* 设置间隔定时器 */
    frw_timer_create_timer_m(&(pst_hmac_user->st_sa_query_info.st_sa_query_interval_timer),
        hmac_sa_query_interval_timeout, ul_retry_timeout, (void *)pst_hmac_user,
        OAL_FALSE, OAM_MODULE_ID_HMAC, pst_mac_vap->ul_core_id);

    /* 发送SA Query request，开始查询流程 */
    ul_ret = hmac_send_sa_query_req(pst_mac_vap, pst_hmac_user, en_is_protected, us_init_trans_id);
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                       "{hmac_start_sa_query::hmac_send_sa_query_req failed[%d].}", ul_ret);
        return OAL_ERR_CODE_PMF_SA_QUERY_REQ_SEND_FAIL;
    }

    return OAL_SUCC;
}


void hmac_send_sa_query_rsp(mac_vap_stru *pst_mac_vap, uint8_t *pst_hdr, oal_bool_enum_uint8 en_is_protected)
{
    uint16_t us_sa_query_len;
    oal_netbuf_stru *pst_sa_query = 0;
    mac_tx_ctl_stru *pst_tx_ctl = OAL_PTR_NULL;
    uint16_t us_user_idx = g_wlan_spec_cfg->invalid_user_id;
    uint32_t ul_ret;

    if (oal_any_null_ptr2(pst_mac_vap, pst_hdr)) {
        oam_error_log2(0, OAM_SF_ANY, "{hmac_send_sa_query_rsp::param null, %d %d.}",
                       (uintptr_t)pst_mac_vap, (uintptr_t)pst_hdr);
        return;
    }
    oam_info_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{hmac_send_sa_query_rsp::AP ready to TX a sa query rsp.}");

    pst_sa_query = (oal_netbuf_stru *)oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF,
                                                           WLAN_MEM_NETBUF_SIZE2,
                                                           OAL_NETBUF_PRIORITY_MID);
    if (pst_sa_query == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{hmac_send_sa_query_rsp::pst_sa_query null.}");
        return;
    }

    memset_s(oal_netbuf_cb(pst_sa_query), oal_netbuf_cb_size(), 0, oal_netbuf_cb_size());
    us_sa_query_len = hmac_encap_sa_query_rsp(pst_mac_vap, pst_hdr, (uint8_t *)oal_netbuf_header(pst_sa_query));

    /* 单播管理帧加密 */
    if (en_is_protected == OAL_TRUE) {
        mac_set_protectedframe((uint8_t *)oal_netbuf_header(pst_sa_query));
    }

    ul_ret = mac_vap_find_user_by_macaddr(pst_mac_vap,
                                          ((mac_ieee80211_frame_stru *)pst_hdr)->auc_address2,
                                          &us_user_idx);
    if (ul_ret != OAL_SUCC) {
        oam_error_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                       "{hmac_send_sa_query_rsp::mac_vap_find_user_by_macaddr fail,error_code=%d.}", ul_ret);
        oal_netbuf_free(pst_sa_query);
        return;
    }

    pst_tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_sa_query); /* 获取cb结构体 */
    MAC_GET_CB_MPDU_LEN(pst_tx_ctl) = us_sa_query_len;           /* dmac发送需要的mpdu长度 */
    MAC_GET_CB_TX_USER_IDX(pst_tx_ctl) = us_user_idx;            /* 发送完成需要获取user结构体 */

    oal_netbuf_put(pst_sa_query, us_sa_query_len);

    /* Buffer this frame in the Memory Queue for transmission */
    ul_ret = hmac_tx_mgmt_send_event(pst_mac_vap, pst_sa_query, us_sa_query_len);
    if (ul_ret != OAL_SUCC) {
        oal_netbuf_free(pst_sa_query);
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{hmac_send_sa_query_rsp::hmac_tx_mgmt_send_event failed[%d].}", ul_ret);
    }

    return;
}
#endif

void hmac_mgmt_send_deauth_frame(mac_vap_stru *pst_mac_vap, const unsigned char *puc_da,
    uint16_t us_err_code, oal_bool_enum_uint8 en_is_protected)
{
    uint16_t us_deauth_len;
    oal_netbuf_stru *pst_deauth = 0;
    mac_tx_ctl_stru *pst_tx_ctl = OAL_PTR_NULL;
    uint32_t ul_ret;

    if (oal_any_null_ptr2(pst_mac_vap, puc_da)) {
        oam_error_log2(0, OAM_SF_AUTH, "{hmac_mgmt_send_deauth_frame::param null, %d %d.}",
                       (uintptr_t)pst_mac_vap, (uintptr_t)pst_mac_vap);
        return;
    }

    if (pst_mac_vap->en_vap_state == MAC_VAP_STATE_BUTT) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_AUTH,
                         "{hmac_mgmt_send_deauth_frame:: vap has been down/del, vap_state[%d].}",
                         pst_mac_vap->en_vap_state);
        return;
    }
    pst_deauth = (oal_netbuf_stru *)oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF,
                                                         WLAN_MEM_NETBUF_SIZE2,
                                                         OAL_NETBUF_PRIORITY_MID);

    oal_mem_netbuf_trace(pst_deauth, OAL_TRUE);

    if (pst_deauth == OAL_PTR_NULL) {
        /* Reserved Memory pool tried for high priority deauth frames */
        pst_deauth = (oal_netbuf_stru *)oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF,
                                                             WLAN_MEM_NETBUF_SIZE2,
                                                             OAL_NETBUF_PRIORITY_MID);
        if (pst_deauth == OAL_PTR_NULL) {
            oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_AUTH, "{hmac_mgmt_send_deauth_frame::pst_deauth null.}");
            return;
        }
    }

    memset_s(oal_netbuf_cb(pst_deauth), oal_netbuf_cb_size(), 0, oal_netbuf_cb_size());

    us_deauth_len = hmac_mgmt_encap_deauth(pst_mac_vap, (uint8_t *)oal_netbuf_header(pst_deauth),
                                           puc_da, us_err_code);
    if (us_deauth_len == 0) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_AUTH, "{hmac_mgmt_send_deauth_frame:: us_deauth_len = 0.}");
        oal_netbuf_free(pst_deauth);
        return;
    }
    oal_netbuf_put(pst_deauth, us_deauth_len);

    /* MIB variables related to deauthentication are updated */
    /* 增加发送去认证帧时的维测信息 */
    oam_warning_log4(pst_mac_vap->uc_vap_id, OAM_SF_CONN,
                     "{hmac_mgmt_send_deauth_frame:: DEAUTH TX :  to %2x:XX:XX:XX:%2x:%2x, status code[%d]}",
                     puc_da[0], puc_da[4], puc_da[5], us_err_code);

    pst_tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_deauth); /* 获取cb结构体 */
    MAC_GET_CB_MPDU_LEN(pst_tx_ctl) = us_deauth_len;           /* dmac发送需要的mpdu长度 */
    /* 发送完成需要获取user结构体 */
    ul_ret = mac_vap_set_cb_tx_user_idx(pst_mac_vap, pst_tx_ctl, puc_da);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log3(pst_mac_vap->uc_vap_id, OAM_SF_AUTH,
                         "(hmac_mgmt_send_deauth_frame::fail to find user by xx:xx:xx:%2x:%2x:%2x.}",
                         puc_da[3],
                         puc_da[4],
                         puc_da[5]);
    }

    /* Buffer this frame in the Memory Queue for transmission */
    ul_ret = hmac_tx_mgmt_send_event(pst_mac_vap, pst_deauth, us_deauth_len);
    if (ul_ret != OAL_SUCC) {
        oal_netbuf_free(pst_deauth);

        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_AUTH,
                         "{hmac_mgmt_send_deauth_frame::hmac_tx_mgmt_send_event failed[%d].}", ul_ret);
    }
}

uint32_t hmac_config_send_deauth(mac_vap_stru *pst_mac_vap, uint8_t *puc_da)
{
    hmac_user_stru *pst_hmac_user = OAL_PTR_NULL;

    if (oal_unlikely(oal_any_null_ptr2(pst_mac_vap, puc_da))) {
        oam_error_log2(0, OAM_SF_AUTH, "{hmac_config_send_deauth::param null, %d %d.}",
                       (uintptr_t)pst_mac_vap, (uintptr_t)puc_da);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hmac_user = mac_vap_get_hmac_user_by_addr(pst_mac_vap, puc_da);
    if (pst_hmac_user == OAL_PTR_NULL) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_AUTH, "{hmac_config_send_deauth::pst_hmac_user null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (pst_hmac_user->st_user_base_info.en_user_asoc_state != MAC_USER_STATE_ASSOC) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_AUTH,
                         "{hmac_config_send_deauth::the user is unassociated.}");
        return OAL_FAIL;
    }

    /* 发去认证帧 */
    hmac_mgmt_send_deauth_frame(pst_mac_vap, puc_da, MAC_AUTH_NOT_VALID, OAL_FALSE);

    /* 删除用户 */
    hmac_user_del(pst_mac_vap, pst_hmac_user);

    return OAL_SUCC;
}


void hmac_mgmt_send_disassoc_frame(mac_vap_stru *pst_mac_vap, uint8_t *puc_da,
    uint16_t us_err_code, oal_bool_enum_uint8 en_is_protected)
{
    uint16_t us_disassoc_len;
    oal_netbuf_stru *pst_disassoc = 0;
    mac_tx_ctl_stru *pst_tx_ctl = OAL_PTR_NULL;
    uint32_t ul_ret;

    if (oal_any_null_ptr2(pst_mac_vap, puc_da)) {
        oam_error_log2(0, OAM_SF_ASSOC, "{hmac_mgmt_send_disassoc_frame::param null, %x %x.}",
                       (uintptr_t)pst_mac_vap, (uintptr_t)pst_mac_vap);
        return;
    }

    if (pst_mac_vap->en_vap_state == MAC_VAP_STATE_BUTT) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_mgmt_send_disassoc_frame:: vap has been down/del, vap_state[%d].}",
                         pst_mac_vap->en_vap_state);
        return;
    }
    pst_disassoc = (oal_netbuf_stru *)oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF,
                                                           WLAN_MEM_NETBUF_SIZE2,
                                                           OAL_NETBUF_PRIORITY_MID);
    if (pst_disassoc == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC, "{hmac_mgmt_send_disassoc_frame::pst_disassoc null.}");
        return;
    }

    oal_mem_netbuf_trace(pst_disassoc, OAL_TRUE);

    memset_s(oal_netbuf_cb(pst_disassoc), oal_netbuf_cb_size(), 0, oal_netbuf_cb_size());

    us_disassoc_len = hmac_mgmt_encap_disassoc(pst_mac_vap, (uint8_t *)oal_netbuf_header(pst_disassoc),
                                               puc_da, us_err_code);
    if (us_disassoc_len == 0) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC,
                       "{hmac_mgmt_send_disassoc_frame:: us_disassoc_len = 0.}");
        oal_netbuf_free(pst_disassoc);
        return;
    }

    /* 增加发送去关联帧时的维测信息 */
    oam_warning_log4(pst_mac_vap->uc_vap_id, OAM_SF_CONN,
        "{hmac_mgmt_send_disassoc_frame:: DISASSOC tx, Because of err_code[%d], \
        send disassoc frame to dest addr, da[%2X:XX:XX:XX:%2X:%2X].}",
        us_err_code, puc_da[0], puc_da[4], puc_da[5]);

    pst_tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_disassoc);
    MAC_GET_CB_MPDU_LEN(pst_tx_ctl) = us_disassoc_len;
    /* 填写非法值,发送完成之后获取用户为NULL,直接释放去认证帧  */
    ul_ret = mac_vap_set_cb_tx_user_idx(pst_mac_vap, pst_tx_ctl, puc_da);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log3(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC,
                         "(hmac_mgmt_send_disassoc_frame::fail to find user by xx:xx:xx:0x:0x:0x.}",
                         puc_da[3], puc_da[4],
                         puc_da[5]);
    }

    oal_netbuf_put(pst_disassoc, us_disassoc_len);

    /* 加入发送队列 */
    ul_ret = hmac_tx_mgmt_send_event(pst_mac_vap, pst_disassoc, us_disassoc_len);
    if (ul_ret != OAL_SUCC) {
        oal_netbuf_free(pst_disassoc);

        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_mgmt_send_disassoc_frame::hmac_tx_mgmt_send_event failed[%d].}", ul_ret);
    }
}

OAL_STATIC void hmac_mgmt_update_qos_by_ie(uint8_t *payload, uint16_t msg_len, mac_vap_stru *mac_vap,
    hmac_user_stru **hmac_user)
{
    uint8_t *ie = OAL_PTR_NULL;

    ie = mac_get_wmm_ie(payload, msg_len);
    if (ie != OAL_PTR_NULL) {
        mac_user_set_qos(&((*hmac_user)->st_user_base_info),
                         mac_vap->pst_mib_info->st_wlan_mib_sta_config.en_dot11QosOptionImplemented);
        return;
    }

    if (IS_STA(mac_vap)) {
        /* 如果关联用户之前就是没有携带wmm ie, 再查找HT CAP能力 */
        ie = mac_find_ie(MAC_EID_HT_CAP, payload, msg_len);
        if (ie != OAL_PTR_NULL) {
            if ((ie[BIT_OFFSET_1] >= 2) && (ie[BIT_OFFSET_2] & BIT5)) { // 2表示ht cap长度的校验值
                mac_user_set_qos(&((*hmac_user)->st_user_base_info), OAL_TRUE);
            }
        }
    }
    return;
}


void hmac_mgmt_update_assoc_user_qos_table(uint8_t *payload, uint16_t msg_len, hmac_user_stru *hmac_user)
{
    frw_event_mem_stru *event_mem = OAL_PTR_NULL;
    frw_event_stru *event = OAL_PTR_NULL;
    dmac_ctx_asoc_set_reg_stru asoc_set_reg_param = { 0 };
    mac_vap_stru *mac_vap = OAL_PTR_NULL;

    /* 如果关联用户之前就是wmm使能的，什么都不用做，直接返回  */
    if (hmac_user->st_user_base_info.st_cap_info.bit_qos == OAL_TRUE) {
        return;
    }

    mac_user_set_qos(&hmac_user->st_user_base_info, OAL_FALSE);

    mac_vap = (mac_vap_stru *)mac_res_get_mac_vap(hmac_user->st_user_base_info.uc_vap_id);
    if (oal_unlikely(mac_vap == OAL_PTR_NULL)) {
        oam_error_log0(hmac_user->st_user_base_info.uc_vap_id, OAM_SF_ASSOC,
                       "{hmac_mgmt_update_assoc_user_qos_table::pst_mac_vap null.}");
        return;
    }

    hmac_mgmt_update_qos_by_ie(payload, msg_len, mac_vap, &hmac_user);

    /* 如果关联用户到现在仍然不是wmm使能的，什么也不做，直接返回 */
    if (hmac_user->st_user_base_info.st_cap_info.bit_qos == OAL_FALSE) {
        return;
    }

    /* 当关联用户从不支持wmm到支持wmm转换时，抛事件到DMAC 写寄存器 */
    event_mem = frw_event_alloc_m(OAL_SIZEOF(dmac_ctx_asoc_set_reg_stru));
    if (event_mem == OAL_PTR_NULL) {
        oam_error_log1(hmac_user->st_user_base_info.uc_vap_id, OAM_SF_ASSOC,
                       "{hmac_mgmt_update_assoc_user_qos_table::event_mem alloc null, size[%d].}",
                       OAL_SIZEOF(dmac_ctx_asoc_set_reg_stru));
        return;
    }

    asoc_set_reg_param.uc_user_index = hmac_user->st_user_base_info.us_assoc_id;

    event = frw_get_event_stru(event_mem);
    frw_event_hdr_init(&(event->st_event_hdr), FRW_EVENT_TYPE_WLAN_CTX, DMAC_WLAN_CTX_EVENT_SUB_TYPE_ASOC_WRITE_REG,
                       OAL_SIZEOF(dmac_ctx_asoc_set_reg_stru), FRW_EVENT_PIPELINE_STAGE_1,
                       hmac_user->st_user_base_info.uc_chip_id, hmac_user->st_user_base_info.uc_device_id,
                       hmac_user->st_user_base_info.uc_vap_id);

    if (memcpy_s(event->auc_event_data, OAL_SIZEOF(dmac_ctx_asoc_set_reg_stru),
        (void *)&asoc_set_reg_param, OAL_SIZEOF(dmac_ctx_asoc_set_reg_stru)) != EOK) {
        oam_error_log0(0, OAM_SF_ASSOC, "hmac_mgmt_update_assoc_user_qos_table::memcpy fail!");
        frw_event_free_m(event_mem);
        return;
    }

    frw_event_dispatch_event(event_mem);
    frw_event_free_m(event_mem);
}


#ifdef _PRE_WLAN_FEATURE_TXBF
void hmac_mgmt_update_11ntxbf_cap(uint8_t *puc_payload, hmac_user_stru *pst_hmac_user)
{
    mac_11ntxbf_vendor_ie_stru *pst_vendor_ie;
    if (puc_payload == OAL_PTR_NULL) {
        return;
    }

    /* 检测到vendor ie */
    pst_vendor_ie = (mac_11ntxbf_vendor_ie_stru *)puc_payload;

    if (pst_vendor_ie->uc_len < (OAL_SIZEOF(mac_11ntxbf_vendor_ie_stru) - MAC_IE_HDR_LEN)) {
        oam_warning_log1(0, OAM_SF_ANY, "hmac_mgmt_update_11ntxbf_cap:invalid vendor ie len[%d]",
                         pst_vendor_ie->uc_len);
        return;
    }

    pst_hmac_user->st_user_base_info.st_cap_info.bit_11ntxbf = pst_vendor_ie->st_11ntxbf.bit_11ntxbf;

    return;
}
#endif /* #ifdef _PRE_WLAN_FEATURE_TXBF */


uint32_t hmac_check_bss_cap_info(uint16_t us_cap_info, mac_vap_stru *pst_mac_vap)
{
    hmac_vap_stru *pst_hmac_vap;
    uint32_t ul_ret;
    wlan_mib_desired_bsstype_enum_uint8 en_bss_type;

    /* 获取CAP INFO里BSS TYPE */
    en_bss_type = mac_get_bss_type(us_cap_info);

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_CFG, "{hmac_check_bss_cap_info::pst_hmac_vap null.}");
        return OAL_FAIL;
    }

    /* 比较BSS TYPE是否一致 不一致，如果是STA仍然发起入网，增加兼容性，其它模式则返回不支持 */
    if (en_bss_type != mac_mib_get_DesiredBSSType(pst_mac_vap)) {
        oam_warning_log2(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_check_bss_cap_info::vap_bss_type[%d] is different from asoc_user_bss_type[%d].}",
                         mac_mib_get_DesiredBSSType(pst_mac_vap), en_bss_type);
    }

    if (OAL_TRUE == mac_mib_get_WPSActive(pst_mac_vap)) {
        return OAL_TRUE;
    }

    /* 比较CAP INFO中privacy位，检查是否加密，加密不一致，返回失败 */
    ul_ret = mac_check_mac_privacy(us_cap_info, (uint8_t *)pst_mac_vap);
    if (ul_ret != OAL_TRUE) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_ASSOC,
                         "{hmac_check_bss_cap_info::mac privacy capabilities is different.}");
    }

    return OAL_TRUE;
}
OAL_STATIC OAL_INLINE oal_bool_enum_uint8 hmac_user_protocol_mode_vap_protocol_check(
    mac_user_stru *pst_mac_user, mac_vap_stru *pst_mac_vap)
{
    /* 兼容性问题：思科AP 2.4G（11b）和5G(11a)共存时发送的assoc rsp帧携带的速率分别是11g和11b，导致STA创建用户时通知算法失败，
    Autorate失效，DBAC情况下，DBAC无法启动已工作的VAP状态无法恢复的问题 临时方案，建议针对对端速率异常的情况统一分析优化 */
    return (((pst_mac_user->en_protocol_mode == WLAN_LEGACY_11B_MODE) &&
             (pst_mac_vap->en_protocol == WLAN_LEGACY_11A_MODE)) ||
            ((pst_mac_user->en_protocol_mode == WLAN_LEGACY_11G_MODE) &&
             (pst_mac_vap->en_protocol == WLAN_LEGACY_11B_MODE)));
}

OAL_STATIC void hmac_set_usr_proto_mode_by_band(mac_vap_stru *mac_vap, hmac_user_stru *hmac_user,
    mac_user_stru *mac_user)
{
    if (mac_vap->st_channel.en_band == WLAN_BAND_5G) { /* 判断是否是5G */
        mac_user_set_protocol_mode(mac_user, WLAN_LEGACY_11A_MODE);
    } else {
        if (hmac_is_support_11grate(hmac_user->st_op_rates.auc_rs_rates,
            hmac_user->st_op_rates.uc_rs_nrates) == OAL_TRUE) {
            mac_user_set_protocol_mode(mac_user, WLAN_LEGACY_11G_MODE);
            if (hmac_is_support_11brate(hmac_user->st_op_rates.auc_rs_rates,
                hmac_user->st_op_rates.uc_rs_nrates) == OAL_TRUE) {
                mac_user_set_protocol_mode(mac_user, WLAN_MIXED_ONE_11G_MODE);
            }
        } else if (hmac_is_support_11brate(hmac_user->st_op_rates.auc_rs_rates,
            hmac_user->st_op_rates.uc_rs_nrates) == OAL_TRUE) {
            mac_user_set_protocol_mode(mac_user, WLAN_LEGACY_11B_MODE);
        } else {
            oam_warning_log0(mac_vap->uc_vap_id, OAM_SF_ANY, "{hmac_set_user_protocol_mode::set usr proto fail.}");
        }
    }
    return;
}

OAL_STATIC void hmac_set_usr_proto_mode_core(mac_vap_stru *mac_vap, hmac_user_stru *hmac_user,
    mac_user_stru *mac_user, mac_user_ht_hdl_stru *mac_ht_hdl, mac_vht_hdl_stru *mac_vht_hdl)
{
#ifdef _PRE_WLAN_FEATURE_11AX
    if (MAC_USER_IS_HE_USER(&hmac_user->st_user_base_info)) {
        mac_user_set_protocol_mode(mac_user, WLAN_HE_MODE);
    } else if (mac_vht_hdl->en_vht_capable == OAL_TRUE) {
#else
    if (mac_vht_hdl->en_vht_capable == OAL_TRUE) {
#endif
        mac_user_set_protocol_mode(mac_user, WLAN_VHT_MODE);
    } else if (mac_ht_hdl->en_ht_capable == OAL_TRUE) {
        mac_user_set_protocol_mode(mac_user, WLAN_HT_MODE);
    } else {
        hmac_set_usr_proto_mode_by_band(mac_vap, hmac_user, mac_user);
    }
    return;
}

void hmac_set_user_protocol_mode(mac_vap_stru *mac_vap, hmac_user_stru *hmac_user)
{
    mac_user_stru *mac_user = &hmac_user->st_user_base_info;
    mac_user_ht_hdl_stru *mac_ht_hdl = &(mac_user->st_ht_hdl);
    mac_vht_hdl_stru *mac_vht_hdl = &(mac_user->st_vht_hdl);

    hmac_set_usr_proto_mode_core(mac_vap, hmac_user, mac_user, mac_ht_hdl, mac_vht_hdl);

    if (hmac_user_protocol_mode_vap_protocol_check(mac_user, mac_vap)) {
        mac_user_set_protocol_mode(mac_user, mac_vap->en_protocol);
        if (memcpy_s((void *)&(hmac_user->st_op_rates), OAL_SIZEOF(hmac_user->st_op_rates),
            (void *)&(mac_vap->st_curr_sup_rates.st_rate), OAL_SIZEOF(mac_rate_stru)) != EOK) {
            oam_error_log0(0, OAM_SF_ANY, "hmac_set_user_protocol_mode::memcpy fail!");
            return;
        }
    }

    /*
     * 兼容性问题：ws880配置11a时beacon和probe resp帧中协议vht能力，
     * vap的protocol能力要根据关联响应帧的实际能力刷新成实际11a能力
     */
    if ((mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_STA) &&
        (mac_vap->en_protocol == WLAN_VHT_MODE) && (mac_user->en_protocol_mode == WLAN_LEGACY_11A_MODE)) {
        mac_vap_init_by_protocol(mac_vap, mac_user->en_protocol_mode);
    }

    mac_vap_tx_data_set_user_htc_cap(mac_vap, &hmac_user->st_user_base_info);
}

void hmac_user_init_rates(hmac_user_stru *pst_hmac_user)
{
    memset_s((uint8_t *)(&pst_hmac_user->st_op_rates), OAL_SIZEOF(pst_hmac_user->st_op_rates),
             0, OAL_SIZEOF(pst_hmac_user->st_op_rates));
}


uint8_t hmac_add_user_rates(hmac_user_stru *pst_hmac_user, uint8_t uc_rates_cnt, uint8_t *puc_rates)
{
    if (pst_hmac_user->st_op_rates.uc_rs_nrates + uc_rates_cnt > WLAN_USER_MAX_SUPP_RATES) {
        uc_rates_cnt = WLAN_USER_MAX_SUPP_RATES - pst_hmac_user->st_op_rates.uc_rs_nrates;
    }

    if (memcpy_s(&(pst_hmac_user->st_op_rates.auc_rs_rates[pst_hmac_user->st_op_rates.uc_rs_nrates]),
        WLAN_USER_MAX_SUPP_RATES - pst_hmac_user->st_op_rates.uc_rs_nrates, puc_rates, uc_rates_cnt) != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "hmac_add_user_rates::memcpy fail!");
        return uc_rates_cnt;
    }
    pst_hmac_user->st_op_rates.uc_rs_nrates += uc_rates_cnt;

    return uc_rates_cnt;
}

#if (_PRE_WLAN_FEATURE_PMF != _PRE_PMF_NOT_SUPPORT)


void hmac_rx_sa_query_req(hmac_vap_stru *pst_hmac_vap,
    oal_netbuf_stru *pst_netbuf, oal_bool_enum_uint8 en_is_protected)
{
    uint8_t *puc_sa;
    hmac_user_stru *pst_hmac_user = OAL_PTR_NULL;
    uint8_t *puc_mac_hdr = OAL_PTR_NULL;

    if (oal_any_null_ptr2(pst_hmac_vap, pst_netbuf)) {
        oam_error_log2(0, OAM_SF_RX, "{hmac_rx_sa_query_req::null param %x %x.}",
                       (uintptr_t)pst_hmac_vap, (uintptr_t)pst_netbuf);
        return;
    }

    puc_mac_hdr = oal_netbuf_header(pst_netbuf);

    mac_rx_get_sa((mac_ieee80211_frame_stru *)puc_mac_hdr, &puc_sa);
    pst_hmac_user = mac_vap_get_hmac_user_by_addr(&pst_hmac_vap->st_vap_base_info, puc_sa);
    if (pst_hmac_user == OAL_PTR_NULL) {
        oam_error_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_RX,
                       "{hmac_rx_sa_query_req::pst_hmac_user null.}");
        return;
    }

    /* 如果该用户的管理帧加密属性不一致，丢弃该报文 */
    if (en_is_protected != pst_hmac_user->st_user_base_info.st_cap_info.bit_pmf_active) {
        oam_warning_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_RX,
                         "{hmac_rx_sa_query_req::PMF check failed.}");
        return;
    }

    /* sa Query rsp发送 */
    hmac_send_sa_query_rsp(&pst_hmac_vap->st_vap_base_info, puc_mac_hdr, en_is_protected);

    return;
}

void hmac_rx_sa_query_rsp(hmac_vap_stru *pst_hmac_vap,
    oal_netbuf_stru *pst_netbuf, oal_bool_enum_uint8 en_is_protected)
{
    uint8_t *puc_mac_hdr = OAL_PTR_NULL;
    uint8_t *puc_sa;
    hmac_user_stru *pst_hmac_user = OAL_PTR_NULL;
    uint16_t *pus_trans_id;
    mac_sa_query_stru *pst_sa_query_info;

    if (oal_any_null_ptr2(pst_hmac_vap, pst_netbuf)) {
        oam_error_log2(0, OAM_SF_AMPDU, "{hmac_rx_sa_query_rsp::param null,%x %x.}",
                       (uintptr_t)pst_hmac_vap, (uintptr_t)pst_netbuf);
        return;
    }

    puc_mac_hdr = oal_netbuf_header(pst_netbuf);

    mac_rx_get_sa((mac_ieee80211_frame_stru *)puc_mac_hdr, &puc_sa);
    pst_hmac_user = mac_vap_get_hmac_user_by_addr(&pst_hmac_vap->st_vap_base_info, puc_sa);
    if (pst_hmac_user == OAL_PTR_NULL) {
        oam_error_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_AMPDU,
                       "{hmac_rx_sa_query_rsp::pst_hmac_user null.}");
        return;
    }

    /* 如果该用户的管理帧加密属性不一致，丢弃该报文 */
    if (en_is_protected != pst_hmac_user->st_user_base_info.st_cap_info.bit_pmf_active) {
        oam_error_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_AMPDU,
                       "{hmac_rx_sa_query_rsp::PMF check failed.}");
        return;
    }

    /* 对比trans_id */
    pus_trans_id = (uint16_t *)(puc_mac_hdr + MAC_80211_FRAME_LEN + 2);
    pst_sa_query_info = &pst_hmac_user->st_sa_query_info;

    /* 收到有效的SA query reqponse，保留这条有效的SA */
    if (oal_memcmp(pus_trans_id, &(pst_sa_query_info->us_sa_query_trans_id), 2) == OAL_SUCC) {
        /* pending SA Query requests 计数器清零 & sa query流程开始时间清零 */
        pst_sa_query_info->ul_sa_query_count = 0;
        pst_sa_query_info->ul_sa_query_start_time = 0;

        /* 删除timer */
        if (pst_sa_query_info->st_sa_query_interval_timer.en_is_registerd != OAL_FALSE) {
            frw_timer_immediate_destroy_timer_m(&(pst_sa_query_info->st_sa_query_interval_timer));
        }
    }

    return;
}
#endif

OAL_STATIC uint32_t hmac_rx_mgmt_event_set_intf_name(hmac_vap_stru *hmac_vap,
    hmac_rx_mgmt_event_stru *mgmt_frame, mac_rx_ctl_stru *rx_info)
{
    mac_ieee80211_frame_stru *pst_frame_hdr = OAL_PTR_NULL;
    mac_vap_stru *mac_vap = &(hmac_vap->st_vap_base_info);
    uint8_t hal_vap_id;
    int32_t l_ret;

    l_ret = memcpy_s(mgmt_frame->ac_name, OAL_IF_NAME_SIZE, hmac_vap->pst_net_device->name, OAL_IF_NAME_SIZE);
    if (!IS_LEGACY_VAP(mac_vap)) {
        /*
         * 仅针对P2P设备做处理。P2P vap 存在一个vap 对应多个hal_vap 情况，
         * 非P2P vap 不存在一个vap 对应多个hal_vap 情况
         */
        /* 对比接收到的管理帧vap_id 是否和vap 中hal_vap_id 相同 */
        /* 从管理帧cb字段中的hal vap id 的相应信息查找对应的net dev 指针 */
        pst_frame_hdr = (mac_ieee80211_frame_stru *)mac_get_rx_cb_mac_hdr(rx_info);
        hal_vap_id = MAC_GET_RX_CB_HAL_VAP_IDX(rx_info);
        if (oal_compare_mac_addr(pst_frame_hdr->auc_address1, mac_mib_get_p2p0_dot11StationID(mac_vap)) == OAL_SUCC) {
            /* 第二个net dev槽 */
            l_ret += memcpy_s(mgmt_frame->ac_name, OAL_IF_NAME_SIZE,
                              hmac_vap->pst_p2p0_net_device->name, OAL_IF_NAME_SIZE);
        } else if (oal_compare_mac_addr(pst_frame_hdr->auc_address1, mac_mib_get_StationID(mac_vap)) == OAL_SUCC) {
            l_ret += memcpy_s(mgmt_frame->ac_name, OAL_IF_NAME_SIZE,
                              hmac_vap->pst_net_device->name, OAL_IF_NAME_SIZE);
        } else if ((hal_vap_id == WLAN_HAL_OHTER_BSS_ID) && (IS_P2P_CL(mac_vap) || IS_P2P_DEV(mac_vap)) &&
                   (pst_frame_hdr->st_frame_control.bit_sub_type == WLAN_PROBE_REQ)) {
            if (!hmac_get_feature_switch(HMAC_MIRACAST_SINK_SWITCH)) {
                return OAL_FAIL;
            }
            l_ret += memcpy_s(mgmt_frame->ac_name, OAL_IF_NAME_SIZE,
                              hmac_vap->pst_p2p0_net_device->name, OAL_IF_NAME_SIZE);
        } else {
            return OAL_FAIL;
        }
    }
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_SCAN, "hmac_rx_mgmt_event_set_intf_name::memcpy failed!");
        return OAL_FAIL;
    }
    return OAL_SUCC;
}

void hmac_send_mgmt_to_host(hmac_vap_stru *pst_hmac_vap,
    oal_netbuf_stru *puc_buf, uint16_t us_len, int32_t l_freq)
{
    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL;
    frw_event_stru *pst_event = OAL_PTR_NULL;
    hmac_rx_mgmt_event_stru *pst_mgmt_frame = OAL_PTR_NULL;
    mac_rx_ctl_stru *pst_rx_info = OAL_PTR_NULL;
    uint8_t *pst_mgmt_data = OAL_PTR_NULL;
    int32_t l_ret;

    /* 抛关联一个新的sta完成事件到WAL */
    pst_event_mem = frw_event_alloc_m(OAL_SIZEOF(hmac_rx_mgmt_event_stru));
    if (pst_event_mem == OAL_PTR_NULL) {
        oam_error_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
                       "{hmac_send_mgmt_to_host::malloc fail}");
        return;
    }

    pst_rx_info = (mac_rx_ctl_stru *)oal_netbuf_cb(puc_buf);

    if (us_len > WLAN_MEM_NETBUF_SIZE2) { // 长度保护: 防止下方申请内存溢出
        oam_error_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
            "{hmac_send_mgmt_to_host::frame len[%d] invalid.}", us_len);
        frw_event_free_m(pst_event_mem);
        return;
    }

    pst_mgmt_data = (uint8_t *)oal_memalloc(us_len);
    if (pst_mgmt_data == OAL_PTR_NULL) {
        oam_error_log0(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_SCAN,
                       "{hmac_send_mgmt_to_host::malloc fail.}");
        frw_event_free_m(pst_event_mem);
        return;
    }
    l_ret = memcpy_s(pst_mgmt_data, us_len, (uint8_t *)MAC_GET_RX_CB_MAC_HEADER_ADDR(pst_rx_info), us_len);
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_SCAN, "hmac_send_mgmt_to_host::memcpy fail!");
        frw_event_free_m(pst_event_mem);
        oal_free(pst_mgmt_data);
        return;
    }

    /* 填写事件 */
    pst_event = frw_get_event_stru(pst_event_mem);

    frw_event_hdr_init(&(pst_event->st_event_hdr), FRW_EVENT_TYPE_HOST_CTX, HMAC_HOST_CTX_EVENT_SUB_TYPE_RX_MGMT,
        OAL_SIZEOF(hmac_rx_mgmt_event_stru), FRW_EVENT_PIPELINE_STAGE_0, pst_hmac_vap->st_vap_base_info.uc_chip_id,
        pst_hmac_vap->st_vap_base_info.uc_device_id, pst_hmac_vap->st_vap_base_info.uc_vap_id);

    /* 填写上报管理帧数据 */
    pst_mgmt_frame = (hmac_rx_mgmt_event_stru *)(pst_event->auc_event_data);
    pst_mgmt_frame->puc_buf = (uint8_t *)pst_mgmt_data;
    pst_mgmt_frame->us_len = us_len;
    pst_mgmt_frame->l_freq = l_freq;
    pst_mgmt_frame->event_type = MAC_GET_RX_CB_NAN_FLAG(pst_rx_info) ?
        HMAC_RX_MGMT_EVENT_TYPE_NAN : HMAC_RX_MGMT_EVENT_TYPE_NORMAL;
    oal_netbuf_len(puc_buf) = us_len;

    if (hmac_rx_mgmt_event_set_intf_name(pst_hmac_vap, pst_mgmt_frame, pst_rx_info) != OAL_SUCC) {
        frw_event_free_m(pst_event_mem);
        oal_free(pst_mgmt_data);
        return;
    }

    /* 分发事件 */
    frw_event_dispatch_event(pst_event_mem);
    frw_event_free_m(pst_event_mem);
}


uint32_t hmac_wpas_mgmt_tx(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    oal_netbuf_stru *pst_netbuf_mgmt_tx = OAL_PTR_NULL;
    mac_tx_ctl_stru *pst_tx_ctl = OAL_PTR_NULL;
    uint32_t ul_ret;
    mac_mgmt_frame_stru *pst_mgmt_tx = OAL_PTR_NULL;
    mac_device_stru *pst_mac_device = OAL_PTR_NULL;
    uint8_t ua_dest_mac_addr[WLAN_MAC_ADDR_LEN];
    uint16_t us_user_idx;

    pst_mgmt_tx = (mac_mgmt_frame_stru *)puc_param;
    if (pst_mgmt_tx->frame == NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_P2P, "{hmac_wpas_mgmt_tx::frame is null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    pst_netbuf_mgmt_tx = pst_mgmt_tx->frame;
    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (pst_mac_device == OAL_PTR_NULL) {
        oal_netbuf_free(pst_netbuf_mgmt_tx);
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_P2P, "{hmac_wpas_mgmt_tx::pst_mac_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_P2P, "{hmac_wpas_mgmt_tx::mgmt frame id=[%d]}",
        pst_mgmt_tx->mgmt_frame_id);


    memset_s(oal_netbuf_cb(pst_netbuf_mgmt_tx), OAL_SIZEOF(mac_tx_ctl_stru), 0, OAL_SIZEOF(mac_tx_ctl_stru));
    
    mac_get_addr1(oal_netbuf_data(pst_netbuf_mgmt_tx), ua_dest_mac_addr, WLAN_MAC_ADDR_LEN);
    us_user_idx = g_wlan_spec_cfg->invalid_user_id;
    /* 管理帧可能发给已关联的用户，也可能发给未关联的用户。已关联的用户可以找到，未关联的用户找不到。不用判断返回值 */
    ul_ret = mac_vap_find_user_by_macaddr(pst_mac_vap, ua_dest_mac_addr, &us_user_idx);

    pst_tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_netbuf_mgmt_tx); /* 获取cb结构体 */
    MAC_GET_CB_MPDU_LEN(pst_tx_ctl) = oal_netbuf_len(pst_netbuf_mgmt_tx); /* dmac发送需要的mpdu长度 */
    MAC_GET_CB_TX_USER_IDX(pst_tx_ctl) = us_user_idx;                  /* 发送完成需要获取user结构体 */
    MAC_GET_CB_IS_NEED_RESP(pst_tx_ctl) = OAL_TRUE;
    MAC_GET_CB_IS_NEEDRETRY(pst_tx_ctl) = OAL_TRUE;
    MAC_GET_CB_MGMT_FRAME_ID(pst_tx_ctl) = pst_mgmt_tx->mgmt_frame_id;
    /* 仅用subtype做识别, 置action会误入dmac_tx_action_process */
    MAC_GET_CB_FRAME_TYPE(pst_tx_ctl) = WLAN_CB_FRAME_TYPE_MGMT;
    MAC_GET_CB_FRAME_SUBTYPE(pst_tx_ctl) = WLAN_ACTION_P2PGO_FRAME_SUBTYPE;

    /* Buffer this frame in the Memory Queue for transmission */
    ul_ret = hmac_tx_mgmt_send_event(pst_mac_vap, pst_netbuf_mgmt_tx, oal_netbuf_len(pst_netbuf_mgmt_tx));
    if (ul_ret != OAL_SUCC) {
        oal_netbuf_free(pst_netbuf_mgmt_tx);

        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_P2P,
                         "{hmac_wpas_mgmt_tx::hmac_tx_mgmt_send_event failed[%d].}", ul_ret);
        return OAL_FAIL;
    }

    return OAL_SUCC;
}


void hmac_rx_mgmt_send_to_host(hmac_vap_stru *pst_hmac_vap, oal_netbuf_stru *pst_netbuf)
{
    mac_rx_ctl_stru *pst_rx_info;
    int32_t l_freq;

    pst_rx_info = (mac_rx_ctl_stru *)oal_netbuf_cb(pst_netbuf);
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 7, 0))
    l_freq = oal_ieee80211_channel_to_frequency(pst_rx_info->uc_channel_number,
        (pst_rx_info->uc_channel_number > 14) ? NL80211_BAND_5GHZ : NL80211_BAND_2GHZ);
#else
    l_freq = oal_ieee80211_channel_to_frequency(pst_rx_info->uc_channel_number,
        (pst_rx_info->uc_channel_number > 14) ? IEEE80211_BAND_5GHZ : IEEE80211_BAND_2GHZ);
#endif
    hmac_send_mgmt_to_host(pst_hmac_vap, pst_netbuf, pst_rx_info->us_frame_len, l_freq);
}

#ifdef _PRE_WLAN_FEATURE_LOCATION

uint8_t g_auc_send_csi_buf[HMAC_CSI_SEND_BUF_LEN] = { 0 };
uint8_t g_auc_send_ftm_buf[HMAC_FTM_SEND_BUF_LEN] = { 0 };


OAL_STATIC uint32_t hmac_netlink_location_send(hmac_vap_stru *pst_hmac_vap, oal_netbuf_stru *pst_netbuf)
{
    hmac_location_event_stru *pst_location_event;
    struct timeval st_tv;
    oal_time_stru st_local_time;
    uint16_t us_action_len;
    uint8_t *puc_payload;
    mac_rx_ctl_stru *pst_rx_ctrl;
    uint32_t ul_index = 0;
    uint32_t *pul_len = OAL_PTR_NULL;
    uint8_t *puc_send_csi_buf = g_auc_send_csi_buf;
    uint8_t *puc_send_ftm_buf = g_auc_send_ftm_buf;
    int32_t l_ret;

    pst_location_event = (hmac_location_event_stru *)mac_netbuf_get_payload(pst_netbuf);
    pst_rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb(pst_netbuf);
    us_action_len = pst_rx_ctrl->us_frame_len;

    if (us_action_len < MAC_CSI_LOCATION_INFO_LEN) {
        oam_error_log1(0, OAM_SF_FTM, "{hmac_netlink_location_send: unexpected len %d}", us_action_len);
        return OAL_ERR_CODE_MSG_LENGTH_ERR;
    }
    do_gettimeofday(&st_tv);
    oal_get_real_time_m(&st_local_time);

    switch (pst_location_event->uc_location_type) {
        case MAC_HISI_LOCATION_RSSI_IE:

            break;
        case MAC_HISI_LOCATION_CSI_IE:
            puc_payload = (uint8_t *)(pst_location_event->auc_payload);

            /* 第一片 */
            if ((puc_payload[0] == 0) &&
                ((puc_payload[1] == 0) || (puc_payload[1] == 1))) {
                memset_s(puc_send_csi_buf, HMAC_CSI_SEND_BUF_LEN, 0, HMAC_CSI_SEND_BUF_LEN);

                /* Type 4Bytes */
                *(uint32_t *)&puc_send_csi_buf[ul_index] = (uint32_t)2;
                ul_index += 4;

                /* len 4Bytes */
                pul_len = (uint32_t *)&puc_send_csi_buf[ul_index];
                ul_index += 4;

                /* mac1 6Bytes */
                l_ret += memcpy_s(&puc_send_csi_buf[ul_index], WLAN_MAC_ADDR_LEN,
                                  pst_location_event->auc_mac_server, WLAN_MAC_ADDR_LEN);
                ul_index += WLAN_MAC_ADDR_LEN;

                /* mac2 6Bytes */
                l_ret += memcpy_s(&puc_send_csi_buf[ul_index], WLAN_MAC_ADDR_LEN,
                                  pst_location_event->auc_mac_client, WLAN_MAC_ADDR_LEN);
                ul_index += WLAN_MAC_ADDR_LEN;

                /* timestamp23Bytes */
                ul_index += snprintf_s(puc_send_csi_buf + ul_index, HMAC_FTM_SEND_BUF_LEN, HMAC_FTM_SEND_BUF_LEN - 1,
                                       "%04d-%02d-%02d %02d:%02d:%02d.%03ld", st_local_time.tm_year + 1900,
                                       st_local_time.tm_mon + 1, st_local_time.tm_mday,
                                       st_local_time.tm_hour, st_local_time.tm_min,
                                       st_local_time.tm_sec, st_tv.tv_usec / 1000);
                if (ul_index < 0) {
                    oam_error_log0(0, OAM_SF_FTM, "hmac_netlink_location_send::snprintf_s error!");
                    return OAL_FAIL;
                }

                /* rssi snr */
                l_ret += memcpy_s(&puc_send_csi_buf[ul_index], MAC_REPORT_RSSIINFO_SNR_LEN,
                                  puc_payload + 3, MAC_REPORT_RSSIINFO_SNR_LEN);

                *pul_len = ul_index + MAC_REPORT_RSSIINFO_SNR_LEN;
                oam_warning_log1(0, OAM_SF_FTM, "{hmac_netlink_location_send::send len[%d].}", *pul_len);
            }

            pul_len = (uint32_t *)&puc_send_csi_buf[4];

            oam_warning_log2(0, OAM_SF_FTM, "{hmac_netlink_location_send::len[%d], copy_len[%d].}",
                             *pul_len, us_action_len - MAC_CSI_LOCATION_INFO_LEN);
            oam_warning_log3(0, OAM_SF_FTM, "{hmac_netlink_location_send::frag [%d], [%d] ,[%d].}",
                             puc_payload[0], puc_payload[1], puc_payload[2]);

            if (*pul_len + us_action_len - MAC_CSI_LOCATION_INFO_LEN > HMAC_CSI_SEND_BUF_LEN) {
                oam_error_log0(0, OAM_SF_FTM, "{hmac_netlink_location_send::puc_send_buf not enough.}");
                return OAL_FAIL;
            }

            l_ret += memcpy_s(&puc_send_csi_buf[*pul_len], us_action_len - MAC_CSI_LOCATION_INFO_LEN,
                              puc_payload + 3 + MAC_REPORT_RSSIINFO_SNR_LEN,
                              us_action_len - MAC_CSI_LOCATION_INFO_LEN);
            *pul_len += us_action_len - MAC_CSI_LOCATION_INFO_LEN;

            /* 最后一片 */
            if (pst_location_event->auc_payload[2] == 1) {
                drv_netlink_location_send((void *)puc_send_csi_buf, *pul_len);
                oam_warning_log1(0, OAM_SF_FTM, "{hmac_netlink_location_send::send len[%d].}", *pul_len);
            }
            break;
        case MAC_HISI_LOCATION_FTM_IE:
            memset_s(puc_send_ftm_buf, HMAC_FTM_SEND_BUF_LEN, 0, HMAC_FTM_SEND_BUF_LEN);
            *(uint32_t *)&puc_send_ftm_buf[ul_index] = (uint32_t)3;
            ul_index += 4;
            *(uint32_t *)&puc_send_ftm_buf[ul_index] = 99;
            ul_index += 4;
            l_ret += memcpy_s(&puc_send_ftm_buf[ul_index], WLAN_MAC_ADDR_LEN,
                              pst_location_event->auc_mac_server, WLAN_MAC_ADDR_LEN);
            ul_index += WLAN_MAC_ADDR_LEN;
            l_ret += memcpy_s(&puc_send_ftm_buf[ul_index], WLAN_MAC_ADDR_LEN,
                              pst_location_event->auc_mac_client, WLAN_MAC_ADDR_LEN);
            ul_index += WLAN_MAC_ADDR_LEN;
            ul_index += snprintf_s(puc_send_ftm_buf + ul_index, HMAC_FTM_SEND_BUF_LEN,
                                   HMAC_FTM_SEND_BUF_LEN - 1,
                                   "%04d-%02d-%02d %02d:%02d:%02d.%03ld",
                                   st_local_time.tm_year + 1900, st_local_time.tm_mon + 1, // 年份从1900年开始
                                   st_local_time.tm_mday,
                                   st_local_time.tm_hour, st_local_time.tm_min,
                                   st_local_time.tm_sec, st_tv.tv_usec / 1000);

            puc_payload = (uint8_t *)(pst_location_event->auc_payload);

            l_ret += memcpy_s(&puc_send_ftm_buf[ul_index], 56, puc_payload, 56);
            drv_netlink_location_send((void *)puc_send_ftm_buf, 99);
            break;
        default:
            return OAL_SUCC;
    }
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_FTM, "hmac_netlink_location_send::memcpy fail!");
        return OAL_FAIL;
    }

    return OAL_SUCC;
}
#endif
#if defined(_PRE_WLAN_FEATURE_LOCATION) || defined(_PRE_WLAN_FEATURE_PSD_ANALYSIS)
#ifndef _PRE_LINUX_TEST

OAL_STATIC uint32_t hmac_proc_location_action(hmac_vap_stru *pst_hmac_vap, oal_netbuf_stru *pst_netbuf)
{
    hmac_location_event_stru *pst_location_event;
    struct timeval st_tv;
    oal_time_stru st_local_time;
    uint8_t auc_filename[128];
    uint16_t us_action_len;
    uint16_t us_delta_len;
    int32_t l_str_len;
    oal_file *f_file;
    oal_mm_segment_t old_fs;
    uint8_t *puc_payload;
    mac_rx_ctl_stru *pst_rx_ctrl;
    int8_t *pc_psd_payload;

    /* Vendor Public Action Header| EID |Length |OUI | type | mac_s | mac_c | rssi */
    /* 获取本地时间精确到us 2017-11-03-23-50-12-xxxxxxxx */
    pst_location_event = (hmac_location_event_stru *)mac_netbuf_get_payload(pst_netbuf);
    pst_rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb(pst_netbuf);
    us_action_len = pst_rx_ctrl->us_frame_len;

    if (us_action_len < MAC_CSI_LOCATION_INFO_LEN) {
        oam_error_log1(0, OAM_SF_FTM, "{hmac_proc_location_action: unexpected len %d}", us_action_len);
        return OAL_ERR_CODE_MSG_LENGTH_ERR;
    }
    do_gettimeofday(&st_tv);
    oal_get_real_time_m(&st_local_time);

    /* 获取文件路径\data\log\location\wlan0\ */
    l_str_len = snprintf_s(auc_filename, OAL_SIZEOF(auc_filename), OAL_SIZEOF(auc_filename) - 1,
                           "/data/log/location/%s/", pst_hmac_vap->auc_name);
    if (l_str_len < 0) {
        oam_error_log0(0, OAM_SF_FTM, "hmac_proc_location_action::snprintf_s error!");
        return OAL_FAIL;
    }

    oam_warning_log1(0, OAM_SF_FTM, "{hmac_proc_location_action::location type[%d]}",
                     pst_location_event->uc_location_type);

    switch (pst_location_event->uc_location_type) {
        case MAC_HISI_LOCATION_RSSI_IE:
            /* 获取文件名 MAC_ADDR_S_MAC_ADDR_C_RSSI */
            l_str_len += snprintf_s(auc_filename + l_str_len, OAL_SIZEOF(auc_filename) - l_str_len,
                                    OAL_SIZEOF(auc_filename) - l_str_len - 1, "RSSI.TXT");

            break;
        case MAC_HISI_LOCATION_CSI_IE:
            /* 获取文件名 MAC_ADDR_S_MAC_ADDR_C_CSI */
            l_str_len += snprintf_s(auc_filename + l_str_len, OAL_SIZEOF(auc_filename) - l_str_len,
                                    OAL_SIZEOF(auc_filename) - l_str_len - 1, "CSI.TXT");

            break;
        case MAC_HISI_LOCATION_FTM_IE:
            /* 获取文件名 MAC_ADDR_S_MAC_ADDR_C_FTM */
            l_str_len += snprintf_s(auc_filename + l_str_len, OAL_SIZEOF(auc_filename) - l_str_len,
                                    OAL_SIZEOF(auc_filename) - l_str_len - 1, "FTM.TXT");

            break;
        case MAC_HISI_PSD_SAVE_FILE_IE:
            /* 获取文件名 MAC_ADDR_S_MAC_ADDR_C_FTM */
            l_str_len += snprintf_s(auc_filename + l_str_len, OAL_SIZEOF(auc_filename) - l_str_len,
                                    OAL_SIZEOF(auc_filename) - l_str_len - 1, "PSD.TXT");

            break;
        default:
            return OAL_SUCC;
    }

    f_file = oal_kernel_file_open(auc_filename, OAL_O_RDWR | OAL_O_CREAT | OAL_O_APPEND);
    if (IS_ERR_OR_NULL(f_file)) {
        oam_error_log1(0, OAM_SF_FTM, "{hmac_proc_location_action: ****************save file failed %d }", l_str_len);

        return OAL_ERR_CODE_OPEN_FILE_FAIL;
    }
    old_fs = oal_get_fs();

    /* 对于CSI来说，payload[0]表示当前的分片序列号；payload[1]表示内存块分段序列号，0表示不分段，1表示第一个分段
       payload[2]表示当前分片是不是最后一片  */
    if (pst_location_event->uc_location_type == MAC_HISI_LOCATION_CSI_IE) {
        if ((pst_location_event->auc_payload[0] == 0) &&
            ((pst_location_event->auc_payload[1] == 0) || (pst_location_event->auc_payload[1] == 1))) {
            oal_kernel_file_print(f_file, "%04d-%02d-%02d-", st_local_time.tm_year + 1900,
                st_local_time.tm_mon + 1, st_local_time.tm_mday);
            oal_kernel_file_print(f_file, "%02d-%02d-%02d-%08d : ", st_local_time.tm_hour,
                st_local_time.tm_min, st_local_time.tm_sec, st_tv.tv_usec);
            /* rssi snr */
            puc_payload = (uint8_t *)(pst_location_event->auc_payload);
            l_str_len = 0;
            while (l_str_len < MAC_REPORT_RSSIINFO_SNR_LEN) {
                oal_kernel_file_print(f_file, "%02X ", *(puc_payload++));
                l_str_len++;
            }
        }
    } else {
        oal_kernel_file_print(f_file, "%04d-%02d-%02d-", st_local_time.tm_year + 1900,
            st_local_time.tm_mon + 1, st_local_time.tm_mday);
        oal_kernel_file_print(f_file, "%02d-%02d-%02d-%08d : ", st_local_time.tm_hour,
            st_local_time.tm_min, st_local_time.tm_sec, st_tv.tv_usec);
    }

    l_str_len = 0;
    puc_payload = (uint8_t *)(pst_location_event->auc_payload);

    if (pst_location_event->uc_location_type == MAC_HISI_LOCATION_CSI_IE) {
        us_delta_len = us_action_len - MAC_CSI_LOCATION_INFO_LEN;
        puc_payload += MAC_REPORT_RSSIINFO_SNR_LEN + 3;
    } else {
        us_delta_len = us_action_len - MAC_FTM_LOCATION_INFO_LEN + MAC_REPORT_RSSIINFO_LEN;
    }

    if (pst_location_event->uc_location_type != MAC_HISI_PSD_SAVE_FILE_IE) {
        while (l_str_len < us_delta_len) {
            oal_kernel_file_print(f_file, "%02X ", *(puc_payload++));
            l_str_len++;
        }
    } else {
        pc_psd_payload = (int8_t *)(pst_location_event->auc_payload);
        while (l_str_len < us_delta_len) {
            oal_kernel_file_print(f_file, "%d ", *(pc_psd_payload++));
            l_str_len++;
        }
    }

#if 1
    if (oal_value_eq_any3(pst_location_event->uc_location_type,
        MAC_HISI_LOCATION_RSSI_IE, MAC_HISI_LOCATION_FTM_IE, MAC_HISI_PSD_SAVE_FILE_IE) ||
        ((pst_location_event->uc_location_type == MAC_HISI_LOCATION_CSI_IE) &&
        ((pst_location_event->auc_payload[2] == 1) &&
        oal_value_eq_any2(pst_location_event->auc_payload[1], 0, 2)))) {
        l_str_len += oal_kernel_file_print(f_file, "\n");
    }
#endif
    oal_kernel_file_close(f_file);
    oal_set_fs(old_fs);

    return OAL_SUCC;
}
#else
OAL_STATIC uint32_t hmac_proc_location_action(hmac_vap_stru *pst_hmac_vap, oal_netbuf_stru *pst_netbuf)
{
    return OAL_SUCC;
}
#endif

uint32_t hmac_huawei_action_process(hmac_vap_stru *pst_hmac_vap, oal_netbuf_stru *pst_netbuf, uint8_t uc_type)
{
    switch (uc_type) {
        case MAC_HISI_LOCATION_RSSI_IE:
        case MAC_HISI_LOCATION_CSI_IE:
        case MAC_HISI_LOCATION_FTM_IE:
        case MAC_HISI_PSD_SAVE_FILE_IE:
            /* 将其他设备上报的私有信息去掉ie头抛事件到hmac进行保存 */
            /* type | mac_s | mac_c   | csi or ftm or rssi   */
            /* csi 信息注意长度 */
            hmac_proc_location_action(pst_hmac_vap, pst_netbuf);
#ifdef _PRE_WLAN_FEATURE_LOCATION
            hmac_netlink_location_send(pst_hmac_vap, pst_netbuf);
#endif
            break;
        default:
            break;
    }

    return OAL_SUCC;
}
#endif

uint32_t hmac_mgmt_tx_event_status(mac_vap_stru *pst_mac_vap, uint8_t uc_len, uint8_t *puc_param)
{
    dmac_crx_mgmt_tx_status_stru *pst_mgmt_tx_status_param = OAL_PTR_NULL;
    dmac_crx_mgmt_tx_status_stru *pst_mgmt_tx_status_param_2wal = OAL_PTR_NULL;
    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL;
    frw_event_stru *pst_event = OAL_PTR_NULL;
    uint32_t ul_ret;

    if (puc_param == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_P2P, "{hmac_mgmt_tx_event_status::puc_param is null!}\r\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_mgmt_tx_status_param = (dmac_crx_mgmt_tx_status_stru *)puc_param;

    oam_warning_log3(pst_mac_vap->uc_vap_id, OAM_SF_P2P,
        "{hmac_mgmt_tx_event_status::dmac tx mgmt status report.userindx[%d], tx mgmt status[%d], frame_id[%d]}",
        pst_mgmt_tx_status_param->us_user_idx,
        pst_mgmt_tx_status_param->uc_dscr_status,
        pst_mgmt_tx_status_param->mgmt_frame_id);

    /* 抛管理帧发送完成事件到WAL */
    pst_event_mem = frw_event_alloc_m(OAL_SIZEOF(dmac_crx_mgmt_tx_status_stru));
    if (pst_event_mem == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_P2P, "{hmac_mgmt_tx_event_status::pst_event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 填写事件 */
    pst_event = frw_get_event_stru(pst_event_mem);

    frw_event_hdr_init(&(pst_event->st_event_hdr),
                       FRW_EVENT_TYPE_HOST_CTX,
                       HMAC_HOST_CTX_EVENT_SUB_TYPE_MGMT_TX_STATUS,
                       OAL_SIZEOF(dmac_crx_mgmt_tx_status_stru),
                       FRW_EVENT_PIPELINE_STAGE_0,
                       pst_mac_vap->uc_chip_id,
                       pst_mac_vap->uc_device_id,
                       pst_mac_vap->uc_vap_id);

    pst_mgmt_tx_status_param_2wal = (dmac_crx_mgmt_tx_status_stru *)(pst_event->auc_event_data);
    pst_mgmt_tx_status_param_2wal->uc_dscr_status = pst_mgmt_tx_status_param->uc_dscr_status;
    pst_mgmt_tx_status_param_2wal->mgmt_frame_id = pst_mgmt_tx_status_param->mgmt_frame_id;

    /* 分发事件 */
    ul_ret = frw_event_dispatch_event(pst_event_mem);
    frw_event_free_m(pst_event_mem);

    return ul_ret;
}


void hmac_vap_set_user_avail_rates(mac_vap_stru *pst_mac_vap, hmac_user_stru *pst_hmac_user)
{
    mac_user_stru *pst_mac_user = OAL_PTR_NULL;
    mac_curr_rateset_stru *pst_mac_vap_rate = OAL_PTR_NULL;
    hmac_rate_stru *pst_hmac_user_rate;
    mac_rate_stru st_avail_op_rates;
    uint8_t uc_mac_vap_rate_num;
    uint8_t uc_hmac_user_rate_num;
    uint8_t uc_vap_rate_idx;
    uint8_t uc_user_rate_idx;
    uint8_t uc_user_avail_rate_idx = 0;

    /* 获取VAP和USER速率的结构体指针 */
    pst_mac_user = &(pst_hmac_user->st_user_base_info);
    pst_mac_vap_rate = &(pst_mac_vap->st_curr_sup_rates);
    pst_hmac_user_rate = &(pst_hmac_user->st_op_rates);
    memset_s((uint8_t *)(&st_avail_op_rates), OAL_SIZEOF(mac_rate_stru), 0, OAL_SIZEOF(mac_rate_stru));

    uc_mac_vap_rate_num = pst_mac_vap_rate->st_rate.uc_rs_nrates;
    uc_hmac_user_rate_num = pst_hmac_user_rate->uc_rs_nrates;

    for (uc_vap_rate_idx = 0; uc_vap_rate_idx < uc_mac_vap_rate_num; uc_vap_rate_idx++) {
        for (uc_user_rate_idx = 0; uc_user_rate_idx < uc_hmac_user_rate_num; uc_user_rate_idx++) {
            if ((pst_mac_vap_rate->st_rate.ast_rs_rates[uc_vap_rate_idx].uc_mac_rate > 0) &&
                (IS_EQUAL_RATES(pst_mac_vap_rate->st_rate.ast_rs_rates[uc_vap_rate_idx].uc_mac_rate,
                pst_hmac_user_rate->auc_rs_rates[uc_user_rate_idx]))) {
                st_avail_op_rates.auc_rs_rates[uc_user_avail_rate_idx] =
                                (pst_hmac_user_rate->auc_rs_rates[uc_user_rate_idx] & 0x7F);
                uc_user_avail_rate_idx++;
                st_avail_op_rates.uc_rs_nrates++;
            }
        }
    }

    mac_user_set_avail_op_rates(pst_mac_user, st_avail_op_rates.uc_rs_nrates, st_avail_op_rates.auc_rs_rates);
    oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
        "{hmac_vap_set_user_avail_rates::uc_avail_op_rates_num=[%d].}", st_avail_op_rates.uc_rs_nrates);
}

uint32_t hmac_proc_ht_cap_ie(mac_vap_stru *pst_mac_vap, mac_user_stru *pst_mac_user, uint8_t *puc_ht_cap_ie)
{
    uint8_t uc_mcs_bmp_index;
    mac_user_ht_hdl_stru *pst_ht_hdl = OAL_PTR_NULL;
    uint16_t us_tmp_info_elem;
    uint16_t us_tmp_txbf_low;
    uint16_t us_ht_cap_info;
    uint32_t ul_tmp_txbf_elem;
    uint16_t us_offset = 0;

    if (oal_any_null_ptr3(pst_mac_vap, pst_mac_user, puc_ht_cap_ie)) {
        oam_warning_log3(0, OAM_SF_ROAM,
                         "{hmac_proc_ht_cap_ie::PARAM NULL! vap=[0x%X],user=[0x%X],cap_ie=[0x%X].}",
                         (uintptr_t)pst_mac_vap, (uintptr_t)pst_mac_user, (uintptr_t)puc_ht_cap_ie);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 至少支持11n才进行后续的处理 */
    if (OAL_FALSE == mac_mib_get_HighThroughputOptionImplemented(pst_mac_vap)) {
        return OAL_SUCC;
    }

    mac_user_set_ht_capable(pst_mac_user, OAL_TRUE);

    pst_ht_hdl = &pst_mac_user->st_ht_hdl;

    /* 带有 HT Capability Element 的 AP，标示它具有HT capable. */
    pst_ht_hdl->en_ht_capable = OAL_TRUE;

    us_offset += MAC_IE_HDR_LEN;

    /********************************************/
    /*     解析 HT Capabilities Info Field      */
    /********************************************/
    us_ht_cap_info = oal_make_word16(puc_ht_cap_ie[us_offset], puc_ht_cap_ie[us_offset + 1]);

    /* 检查STA所支持的LDPC编码能力 B0，0:不支持，1:支持 */
    pst_ht_hdl->bit_ldpc_coding_cap = (us_ht_cap_info & BIT0);

    /* 提取AP所支持的带宽能力  */
    pst_ht_hdl->bit_supported_channel_width = ((us_ht_cap_info & BIT1) >> 1);

    /* 检查空间复用节能模式 B2~B3 */
    mac_ie_proc_sm_power_save_field(pst_mac_user, (uint8_t)((us_ht_cap_info & (BIT3 | BIT2)) >> 2));

    /* 提取AP支持Greenfield情况 */
    pst_ht_hdl->bit_ht_green_field = ((us_ht_cap_info & BIT4) >> 4);

    /* 提取AP支持20MHz Short-GI情况 */
    pst_ht_hdl->bit_short_gi_20mhz = ((us_ht_cap_info & BIT5) >> 5);

    /* 提取AP支持40MHz Short-GI情况 */
    pst_ht_hdl->bit_short_gi_40mhz = ((us_ht_cap_info & BIT6) >> 6);

    /* 提取AP支持STBC PPDU情况 */
    pst_ht_hdl->bit_rx_stbc = (uint8_t)((us_ht_cap_info & (BIT9 | BIT8)) >> 8);

    /* 提取AP 40M上DSSS/CCK的支持情况 */
    pst_ht_hdl->bit_dsss_cck_mode_40mhz = ((us_ht_cap_info & BIT12) >> 12);

    /* 提取AP L-SIG TXOP 保护的支持情况 */
    pst_ht_hdl->bit_lsig_txop_protection = ((us_ht_cap_info & BIT15) >> 15);

    us_offset += MAC_HT_CAPINFO_LEN;

    /********************************************/
    /*     解析 A-MPDU Parameters Field         */
    /********************************************/
    /* 提取 Maximum Rx A-MPDU factor (B1 - B0) */
    pst_ht_hdl->uc_max_rx_ampdu_factor = (puc_ht_cap_ie[us_offset] & 0x03);

    /* 提取 Minmum Rx A-MPDU factor (B3 - B2) */
    pst_ht_hdl->uc_min_mpdu_start_spacing = (puc_ht_cap_ie[us_offset] >> 2) & 0x07;

    us_offset += MAC_HT_AMPDU_PARAMS_LEN;

    /********************************************/
    /*     解析 Supported MCS Set Field         */
    /********************************************/
    for (uc_mcs_bmp_index = 0; uc_mcs_bmp_index < WLAN_HT_MCS_BITMASK_LEN; uc_mcs_bmp_index++) {
        pst_ht_hdl->uc_rx_mcs_bitmask[uc_mcs_bmp_index] =
            (mac_mib_get_SupportedMCSTxValue(pst_mac_vap, uc_mcs_bmp_index)) &
           (*(uint8_t *)(puc_ht_cap_ie + us_offset + uc_mcs_bmp_index));
    }

    pst_ht_hdl->uc_rx_mcs_bitmask[WLAN_HT_MCS_BITMASK_LEN - 1] &= 0x1F;

    us_offset += MAC_HT_SUP_MCS_SET_LEN;

    /********************************************/
    /* 解析 HT Extended Capabilities Info Field */
    /********************************************/
    us_ht_cap_info = oal_make_word16(puc_ht_cap_ie[us_offset], puc_ht_cap_ie[us_offset + 1]);

    /* 提取 HTC support Information */
    pst_ht_hdl->uc_htc_support = ((us_ht_cap_info & BIT10) >> 10);

    us_offset += MAC_HT_EXT_CAP_LEN;

    /********************************************/
    /*  解析 Tx Beamforming Field               */
    /********************************************/
    us_tmp_info_elem = oal_make_word16(puc_ht_cap_ie[us_offset], puc_ht_cap_ie[us_offset + 1]);
    us_tmp_txbf_low = oal_make_word16(puc_ht_cap_ie[us_offset + 2], puc_ht_cap_ie[us_offset + 3]);
    ul_tmp_txbf_elem = oal_make_word32(us_tmp_info_elem, us_tmp_txbf_low);
    pst_ht_hdl->bit_imbf_receive_cap = (ul_tmp_txbf_elem & BIT0);
    pst_ht_hdl->bit_receive_staggered_sounding_cap = ((ul_tmp_txbf_elem & BIT1) >> 1);
    pst_ht_hdl->bit_transmit_staggered_sounding_cap = ((ul_tmp_txbf_elem & BIT2) >> 2);
    pst_ht_hdl->bit_receive_ndp_cap = ((ul_tmp_txbf_elem & BIT3) >> 3);
    pst_ht_hdl->bit_transmit_ndp_cap = ((ul_tmp_txbf_elem & BIT4) >> 4);
    pst_ht_hdl->bit_imbf_cap = ((ul_tmp_txbf_elem & BIT5) >> 5);
    pst_ht_hdl->bit_calibration = ((ul_tmp_txbf_elem & 0x000000C0) >> 6);
    pst_ht_hdl->bit_exp_csi_txbf_cap = ((ul_tmp_txbf_elem & BIT8) >> 8);
    pst_ht_hdl->bit_exp_noncomp_txbf_cap = ((ul_tmp_txbf_elem & BIT9) >> 9);
    pst_ht_hdl->bit_exp_comp_txbf_cap = ((ul_tmp_txbf_elem & BIT10) >> 10);
    pst_ht_hdl->bit_exp_csi_feedback = ((ul_tmp_txbf_elem & 0x00001800) >> 11);
    pst_ht_hdl->bit_exp_noncomp_feedback = ((ul_tmp_txbf_elem & 0x00006000) >> 13);
    pst_ht_hdl->bit_exp_comp_feedback = ((ul_tmp_txbf_elem & 0x0001C000) >> 15);
    pst_ht_hdl->bit_min_grouping = ((ul_tmp_txbf_elem & 0x00060000) >> 17);
    pst_ht_hdl->bit_csi_bfer_ant_number = ((ul_tmp_txbf_elem & 0x001C0000) >> 19);
    pst_ht_hdl->bit_noncomp_bfer_ant_number = ((ul_tmp_txbf_elem & 0x00600000) >> 21);
    pst_ht_hdl->bit_comp_bfer_ant_number = ((ul_tmp_txbf_elem & 0x01C00000) >> 23);
    pst_ht_hdl->bit_csi_bfee_max_rows = ((ul_tmp_txbf_elem & 0x06000000) >> 25);
    pst_ht_hdl->bit_channel_est_cap = ((ul_tmp_txbf_elem & 0x18000000) >> 27);

    return OAL_SUCC;
}


uint32_t hmac_proc_vht_cap_ie(mac_vap_stru *pst_mac_vap,
    hmac_user_stru *pst_hmac_user, uint8_t *puc_vht_cap_ie)
{
    mac_vht_hdl_stru *pst_mac_vht_hdl = OAL_PTR_NULL;
    mac_vht_hdl_stru st_mac_vht_hdl;
    mac_user_stru *pst_mac_user = OAL_PTR_NULL;
    uint16_t us_vht_cap_filed_low;
    uint16_t us_vht_cap_filed_high;
    uint32_t ul_vht_cap_field;
    uint16_t us_rx_mcs_map;
    uint16_t us_tx_mcs_map;
    uint16_t us_rx_highest_supp_logGi_data;
    uint16_t us_tx_highest_supp_logGi_data;
    uint16_t us_msg_idx = 0;
    int32_t l_ret;

    /* 解析vht cap IE */
    if (oal_any_null_ptr3(pst_mac_vap, pst_hmac_user, puc_vht_cap_ie)) {
        oam_error_log3(0, OAM_SF_ANY,
                       "{hmac_proc_vht_cap_ie::param null,mac_vap[0x%x], hmac_user[0x%x], vht_cap_ie[0x%x].}",
                       (uintptr_t)pst_mac_vap, (uintptr_t)pst_hmac_user, (uintptr_t)puc_vht_cap_ie);

        return OAL_ERR_CODE_PTR_NULL;
    }

    if (puc_vht_cap_ie[1] < MAC_VHT_CAP_IE_LEN) {
        oam_warning_log1(0, OAM_SF_ANY, "{hmac_proc_vht_cap_ie::invalid vht cap ie len[%d].}", puc_vht_cap_ie[1]);
        return OAL_FAIL;
    }

    pst_mac_user = &pst_hmac_user->st_user_base_info;

    /* 支持11ac，才进行后续的处理 */
    if (OAL_FALSE == mac_mib_get_VHTOptionImplemented(pst_mac_vap)) {
        return OAL_SUCC;
    }

    pst_mac_vht_hdl = &st_mac_vht_hdl;
    mac_user_get_vht_hdl(pst_mac_user, pst_mac_vht_hdl);

    /* 进入此函数代表user支持11ac */
    pst_mac_vht_hdl->en_vht_capable = OAL_TRUE;

#ifdef _PRE_WLAN_FEATURE_11AC2G
    /* 定制化实现如果不支持11ac2g模式，则关掉vht cap */
    if ((pst_mac_vap->st_cap_flag.bit_11ac2g == OAL_FALSE)
        && (WLAN_BAND_2G == pst_mac_vap->st_channel.en_band)) {
        pst_mac_vht_hdl->en_vht_capable = OAL_FALSE;
    }
#endif

    us_msg_idx += MAC_IE_HDR_LEN;

    /* 解析VHT capablities info field */
    us_vht_cap_filed_low = oal_make_word16(puc_vht_cap_ie[us_msg_idx], puc_vht_cap_ie[us_msg_idx + 1]);
    us_vht_cap_filed_high = oal_make_word16(puc_vht_cap_ie[us_msg_idx + 2], puc_vht_cap_ie[us_msg_idx + 3]);
    ul_vht_cap_field = oal_make_word32(us_vht_cap_filed_low, us_vht_cap_filed_high);

    /* 解析max_mpdu_length 参见11ac协议 Table 8-183u */
    pst_mac_vht_hdl->bit_max_mpdu_length = (ul_vht_cap_field & (BIT1 | BIT0));
    pst_mac_vht_hdl->us_max_mpdu_length =
            mac_ie_get_max_mpdu_len_by_vht_cap(pst_mac_vht_hdl->bit_max_mpdu_length);

    hmac_user_set_amsdu_level_by_max_mpdu_len(pst_hmac_user,
        pst_mac_vht_hdl->us_max_mpdu_length);

    /* 解析supported_channel_width */
    pst_mac_vht_hdl->bit_supported_channel_width = ((ul_vht_cap_field & (BIT3 | BIT2)) >> 2);

    /* 解析rx_ldpc */
    pst_mac_vht_hdl->bit_rx_ldpc = ((ul_vht_cap_field & BIT4) >> 4);

    /* 解析short_gi_80mhz和short_gi_160mhz支持情况 */
    pst_mac_vht_hdl->bit_short_gi_80mhz = ((ul_vht_cap_field & BIT5) >> 5);
    pst_mac_vht_hdl->bit_short_gi_80mhz &= mac_mib_get_VHTShortGIOptionIn80Implemented(pst_mac_vap);

    pst_mac_vht_hdl->bit_short_gi_160mhz = ((ul_vht_cap_field & BIT6) >> 6);
    pst_mac_vht_hdl->bit_short_gi_160mhz &= mac_mib_get_VHTShortGIOptionIn160and80p80Implemented(pst_mac_vap);

    /* 解析tx_stbc 和rx_stbc */
    pst_mac_vht_hdl->bit_tx_stbc = ((ul_vht_cap_field & BIT7) >> 7);
    pst_mac_vht_hdl->bit_rx_stbc = ((ul_vht_cap_field & (BIT10 | BIT9 | BIT8)) >> 8);

    /* 解析su_beamformer_cap和su_beamformee_cap */
    pst_mac_vht_hdl->bit_su_beamformer_cap = ((ul_vht_cap_field & BIT11) >> 11);
    pst_mac_vht_hdl->bit_su_beamformee_cap = ((ul_vht_cap_field & BIT12) >> 12);

    /* 解析num_bf_ant_supported */
    pst_mac_vht_hdl->bit_num_bf_ant_supported = ((ul_vht_cap_field & (BIT15 | BIT14 | BIT13)) >> 13);

    /* 以对端天线数初始化可用最大空间流 */
    pst_mac_user->en_avail_bf_num_spatial_stream = pst_mac_vht_hdl->bit_num_bf_ant_supported;

    /* 解析num_sounding_dim */
    pst_mac_vht_hdl->bit_num_sounding_dim = ((ul_vht_cap_field & (BIT18 | BIT17 | BIT16)) >> 16);

    /* 解析mu_beamformer_cap和mu_beamformee_cap */
    pst_mac_vht_hdl->bit_mu_beamformer_cap = ((ul_vht_cap_field & BIT19) >> 19);
    pst_mac_vht_hdl->bit_mu_beamformee_cap = ((ul_vht_cap_field & BIT20) >> 20);

    /* 解析vht_txop_ps */
    pst_mac_vht_hdl->bit_vht_txop_ps = ((ul_vht_cap_field & BIT21) >> 21);
#ifdef _PRE_WLAN_FEATURE_TXOPPS
    if (pst_mac_vht_hdl->bit_vht_txop_ps == OAL_TRUE && OAL_TRUE == mac_mib_get_txopps(pst_mac_vap)) {
        mac_vap_set_txopps(pst_mac_vap, OAL_TRUE);
    }
#endif
    /* 解析htc_vht_capable */
    pst_mac_vht_hdl->bit_htc_vht_capable = ((ul_vht_cap_field & BIT22) >> 22);

    /* 解析max_ampdu_len_exp */
    pst_mac_vht_hdl->bit_max_ampdu_len_exp = ((ul_vht_cap_field & (BIT25 | BIT24 | BIT23)) >> 23);

    /* 解析vht_link_adaptation */
    pst_mac_vht_hdl->bit_vht_link_adaptation = ((ul_vht_cap_field & (BIT27 | BIT26)) >> 26);

    /* 解析rx_ant_pattern */
    pst_mac_vht_hdl->bit_rx_ant_pattern = ((ul_vht_cap_field & BIT28) >> 28);

    /* 解析tx_ant_pattern */
    pst_mac_vht_hdl->bit_tx_ant_pattern = ((ul_vht_cap_field & BIT29) >> 29);

    /* 解析extend_nss_bw_supp */
    pst_mac_vht_hdl->bit_extend_nss_bw_supp = ((ul_vht_cap_field & (BIT31 | BIT30)) >> 30);

    us_msg_idx += MAC_VHT_CAP_INFO_FIELD_LEN;

    /* 解析VHT Supported MCS Set field */
    us_rx_mcs_map = oal_make_word16(puc_vht_cap_ie[us_msg_idx], puc_vht_cap_ie[us_msg_idx + 1]);

    l_ret = memcpy_s(&(pst_mac_vht_hdl->st_rx_max_mcs_map), OAL_SIZEOF(mac_rx_max_mcs_map_stru),
                     &us_rx_mcs_map, OAL_SIZEOF(mac_rx_max_mcs_map_stru));

    us_msg_idx += MAC_VHT_CAP_RX_MCS_MAP_FIELD_LEN;

    /* 解析rx_highest_supp_logGi_data */
    us_rx_highest_supp_logGi_data = oal_make_word16(puc_vht_cap_ie[us_msg_idx], puc_vht_cap_ie[us_msg_idx + 1]);
    pst_mac_vht_hdl->bit_rx_highest_rate = us_rx_highest_supp_logGi_data & (0x1FFF);

    us_msg_idx += MAC_VHT_CAP_RX_HIGHEST_DATA_FIELD_LEN;

    /* 解析tx_mcs_map */
    us_tx_mcs_map = oal_make_word16(puc_vht_cap_ie[us_msg_idx], puc_vht_cap_ie[us_msg_idx + 1]);
    l_ret += memcpy_s(&(pst_mac_vht_hdl->st_tx_max_mcs_map), OAL_SIZEOF(mac_tx_max_mcs_map_stru),
                      &us_tx_mcs_map, OAL_SIZEOF(mac_tx_max_mcs_map_stru));
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "hmac_proc_vht_cap_ie::memcpy fail!");
        return OAL_FAIL;
    }

    us_msg_idx += MAC_VHT_CAP_TX_MCS_MAP_FIELD_LEN;

    /* 解析tx_highest_supp_logGi_data */
    us_tx_highest_supp_logGi_data = oal_make_word16(puc_vht_cap_ie[us_msg_idx], puc_vht_cap_ie[us_msg_idx + 1]);
    pst_mac_vht_hdl->bit_tx_highest_rate = us_tx_highest_supp_logGi_data & (0x1FFF);

    mac_user_set_vht_hdl(pst_mac_user, pst_mac_vht_hdl);
    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_11AX

uint32_t hmac_proc_he_cap_ie(mac_vap_stru *pst_mac_vap, hmac_user_stru *pst_hmac_user, uint8_t *puc_he_cap_ie)
{
    mac_he_hdl_stru *pst_mac_he_hdl;
    mac_he_hdl_stru st_mac_he_hdl;
    mac_user_stru *pst_mac_user;
    uint32_t ul_ret;

    /* 解析he cap IE */
    if (oal_any_null_ptr2(pst_mac_vap, pst_hmac_user)) {
        oam_error_log2(0, OAM_SF_11AX, "{hmac_proc_he_cap_ie::param null,mac_vap[0x%x], hmac_user[0x%x].}",
                       (uintptr_t)pst_mac_vap, (uintptr_t)pst_hmac_user);
        return OAL_ERR_CODE_PTR_NULL;
    }
    if (puc_he_cap_ie == OAL_PTR_NULL) {
        oam_warning_log1(0, OAM_SF_11AX, "{hmac_proc_he_cap_ie::param null, he_cap_ie[0x%x].}",
                         (uintptr_t)puc_he_cap_ie);
        return OAL_ERR_CODE_PTR_NULL;
    }

    if (puc_he_cap_ie[1] < MAC_HE_CAP_MIN_LEN) {
        oam_warning_log1(0, OAM_SF_11AX, "{hmac_proc_he_cap_ie::invalid he cap ie len[%d].}", puc_he_cap_ie[1]);
        return OAL_FAIL;
    }

    pst_mac_user = &pst_hmac_user->st_user_base_info;

    /* 支持11ax，才进行后续的处理 */
    if (OAL_FALSE == mac_mib_get_HEOptionImplemented(pst_mac_vap)) {
        return OAL_SUCC;
    }

    pst_mac_he_hdl = &st_mac_he_hdl;
    mac_user_get_he_hdl(pst_mac_user, pst_mac_he_hdl);

    /* 解析 HE MAC Capabilities Info */
    ul_ret = mac_ie_parse_he_cap(puc_he_cap_ie, &pst_mac_he_hdl->st_he_cap_ie);
    if (ul_ret != OAL_SUCC) {
        return OAL_FAIL;
    }

    pst_mac_he_hdl->en_he_capable = OAL_TRUE;

    mac_user_set_he_hdl(pst_mac_user, pst_mac_he_hdl);

    /* 适配user支持的Beamforming空间流个数 */
    pst_mac_user->en_avail_bf_num_spatial_stream =
        (pst_mac_vap->st_channel.en_bandwidth <= WLAN_BAND_WIDTH_80MINUSMINUS) ?
        pst_mac_he_hdl->st_he_cap_ie.st_he_phy_cap.bit_beamformee_sts_below_80mhz :
        pst_mac_he_hdl->st_he_cap_ie.st_he_phy_cap.bit_beamformee_sts_over_80mhz;

    return OAL_SUCC;
}


uint32_t hmac_proc_he_bss_color_change_announcement_ie(mac_vap_stru *pst_mac_vap,
    hmac_user_stru *pst_hmac_user, uint8_t *puc_bss_color_ie)
{
    mac_frame_bss_color_change_annoncement_ie_stru st_bss_color_info;
    mac_he_hdl_stru *pst_mac_he_hdl;

    if (OAL_FALSE == mac_mib_get_HEOptionImplemented(pst_mac_vap)) {
        return OAL_SUCC;
    }

    memset_s(&st_bss_color_info, OAL_SIZEOF(mac_frame_bss_color_change_annoncement_ie_stru),
             0, OAL_SIZEOF(mac_frame_bss_color_change_annoncement_ie_stru));
    if (OAL_SUCC != mac_ie_parse_he_bss_color_change_announcement_ie(puc_bss_color_ie, &st_bss_color_info)) {
        return OAL_SUCC;
    }

    pst_mac_he_hdl = MAC_USER_HE_HDL_STRU(&pst_hmac_user->st_user_base_info);
    pst_mac_he_hdl->bit_change_announce_bss_color = st_bss_color_info.bit_new_bss_color;
    pst_mac_he_hdl->bit_change_announce_bss_color_exist = 1;

    oam_warning_log2(0, 0, "hmac_proc_he_bss_color_change_announcement_ie,bss_color=%d, bss_color_exist=[%d]",
        pst_mac_he_hdl->bit_change_announce_bss_color, pst_mac_he_hdl->bit_change_announce_bss_color_exist);

    return OAL_SUCC;
}
#endif


void hmac_add_and_clear_repeat_op_rates(uint8_t *puc_ie_rates,
    uint8_t uc_ie_num_rates, hmac_rate_stru *pst_op_rates)
{
    uint8_t uc_ie_rates_idx;
    uint8_t uc_user_rates_idx;

    for (uc_ie_rates_idx = 0; uc_ie_rates_idx < uc_ie_num_rates; uc_ie_rates_idx++) {
        /* 判断该速率是否已经记录在op中 */
        for (uc_user_rates_idx = 0; uc_user_rates_idx < pst_op_rates->uc_rs_nrates; uc_user_rates_idx++) {
            if (IS_EQUAL_RATES(puc_ie_rates[uc_ie_rates_idx], pst_op_rates->auc_rs_rates[uc_user_rates_idx])) {
                break;
            }
        }

        /* 相等时，说明ie中的速率与op中的速率都不相同，可以加入op的速率集中 */
        if (uc_user_rates_idx == pst_op_rates->uc_rs_nrates) {
            /* 当长度超出限制时告警，不加入op rates中 */
            if (pst_op_rates->uc_rs_nrates == WLAN_USER_MAX_SUPP_RATES) {
                oam_warning_log0(0, OAM_SF_ANY,
                    "{hmac_add_and_clear_repeat_op_rates::user option rates more then WLAN_USER_MAX_SUPP_RATES.}");
                break;
            }
            pst_op_rates->auc_rs_rates[pst_op_rates->uc_rs_nrates++] = puc_ie_rates[uc_ie_rates_idx];
        }
    }
}


uint32_t hmac_ie_proc_assoc_user_legacy_rate(uint8_t *puc_payload,
    uint32_t us_rx_len, hmac_user_stru *pst_hmac_user)
{
    uint8_t *puc_ie = OAL_PTR_NULL;
    mac_user_stru *pst_mac_user;
    mac_vap_stru *pst_mac_vap;
    uint8_t uc_num_rates;
    uint8_t uc_num_ex_rates;

    pst_mac_user = &(pst_hmac_user->st_user_base_info);

    pst_mac_vap = mac_res_get_mac_vap(pst_mac_user->uc_vap_id);
    if (pst_mac_vap == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_ie_proc_assoc_user_legacy_rate::pst_mac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* user的可选速率集此时应该为空 */
    if (pst_hmac_user->st_op_rates.uc_rs_nrates != 0) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                         "{hmac_ie_proc_assoc_user_legacy_rate::the number of user option rates is not 0.}");
    }

    /* 1.处理基础速率集 */
    puc_ie = mac_find_ie(MAC_EID_RATES, puc_payload, (int32_t)us_rx_len);
    if (puc_ie != OAL_PTR_NULL) {
        uc_num_rates = puc_ie[1];
        /*
         * 判断supported rates长度，当长度为0时告警，当长度超出限制时，在hmac_add_and_clear_repeat_op_rates里告警，
         * extended supported rates同样处理
         */
        if (uc_num_rates < MAC_MIN_RATES_LEN) {
            oam_warning_log2(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                "{hmac_ie_proc_assoc_user_legacy_rate::supported rates length less then \
                MAC_MIN_RATES_LEN vap mode[%d] num_rates[%d]}",
                pst_mac_vap->en_vap_mode, uc_num_rates);
        } else {
            hmac_add_and_clear_repeat_op_rates(puc_ie + MAC_IE_HDR_LEN, uc_num_rates, &(pst_hmac_user->st_op_rates));
        }
    } else {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
            "{hmac_ie_proc_assoc_user_legacy_rate::unsupport basic rates vap mode[%d]}", pst_mac_vap->en_vap_mode);
    }

    /* 2.处理扩展速率集 */
    puc_ie = mac_find_ie(MAC_EID_XRATES, puc_payload, (int32_t)us_rx_len);
    if (puc_ie != OAL_PTR_NULL) {
        uc_num_ex_rates = puc_ie[1];
        /* AP模式下，协议大于11a且小等11G时，不允许对端携带拓展速率集，STA模式下不做限制 */
        if ((pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_AP) &&
            ((pst_mac_vap->en_protocol <= WLAN_LEGACY_11G_MODE) && (pst_mac_vap->en_protocol > WLAN_LEGACY_11A_MODE))) {
            oam_warning_log2(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                "{hmac_ie_proc_assoc_user_legacy_rate::invaild xrates rate vap protocol[%d] num_ex_rates[%d]}",
                pst_mac_vap->en_protocol, uc_num_ex_rates);
        } else {
            /* 长度告警处理同上 */
            if (uc_num_ex_rates < MAC_MIN_XRATE_LEN) {
                oam_warning_log2(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                    "{hmac_ie_proc_assoc_user_legacy_rate::extended supported rates length less then \
                    MAC_MIN_RATES_LEN vap mode[%d] num_rates[%d]}",
                    pst_mac_vap->en_vap_mode, uc_num_ex_rates);
            } else {
                hmac_add_and_clear_repeat_op_rates(puc_ie + MAC_IE_HDR_LEN,
                                                   uc_num_ex_rates,
                                                   &(pst_hmac_user->st_op_rates));
            }
        }
    } else {
        if (pst_mac_vap->st_channel.en_band == WLAN_BAND_2G) {
            oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_ANY,
                "{hmac_ie_proc_assoc_user_legacy_rate::unsupport xrates vap mode[%d]}", pst_mac_vap->en_vap_mode);
        }
    }

    if (pst_hmac_user->st_op_rates.uc_rs_nrates == 0) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_ANY, "{hmac_ie_proc_assoc_user_legacy_rate::rate is 0.}");
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_HS20

uint32_t hmac_interworking_check(hmac_vap_stru *pst_hmac_vap, uint8_t *puc_param)
{
    uint8_t *puc_extend_cap_ie;
    mac_bss_dscr_stru *pst_bss_dscr;

    if (oal_unlikely(oal_any_null_ptr2(pst_hmac_vap, puc_param))) {
        oam_warning_log0(0, OAM_SF_ANY, "{hmac_interworking_check:: check failed, null ptr!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_bss_dscr = (mac_bss_dscr_stru *)puc_param;
    if (pst_bss_dscr->ul_mgmt_len < (MAC_80211_FRAME_LEN + MAC_SSID_OFFSET)) {
        oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG,
                         "{hmac_interworking_check:: mgmt_len(%d) < (80211_FRAME_LEN+SSID_OFFSET).}",
                         pst_bss_dscr->ul_mgmt_len);
        return OAL_FAIL;
    }

    /* 查找interworking ie */
    /*lint -e416*/
    puc_extend_cap_ie = mac_find_ie(MAC_EID_EXT_CAPS,
                                    pst_bss_dscr->auc_mgmt_buff + MAC_80211_FRAME_LEN + MAC_SSID_OFFSET,
                                    (int32_t)(pst_bss_dscr->ul_mgmt_len - MAC_80211_FRAME_LEN - MAC_SSID_OFFSET));
    /*lint +e416*/
    if (puc_extend_cap_ie == OAL_PTR_NULL) {
        oam_warning_log1(pst_hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_CFG,
                         "{hmac_interworking_check:: puc_extend_cap_ie is NULL, the ul_mgmt_len is %d.}",
                         pst_bss_dscr->ul_mgmt_len);
        return OAL_FAIL;
    }

    /*  未检测到interworking能力位，返回fail */
    if (puc_extend_cap_ie[1] < 4 || !(puc_extend_cap_ie[5] & 0x80)) {
        pst_hmac_vap->st_vap_base_info.st_cap_flag.bit_is_interworking = OAL_FALSE;
        return OAL_FAIL;
    }

    pst_hmac_vap->st_vap_base_info.st_cap_flag.bit_is_interworking = OAL_TRUE;
    return OAL_SUCC;
}
#endif  // _PRE_WLAN_FEATURE_HS20

/*lint -e19*/
oal_module_symbol(hmac_config_send_deauth);
oal_module_symbol(hmac_wpas_mgmt_tx);
/*lint +e19*/
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

