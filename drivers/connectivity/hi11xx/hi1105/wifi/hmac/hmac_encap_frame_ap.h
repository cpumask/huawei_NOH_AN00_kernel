

#ifndef __HMAC_ENCAP_FRAME_AP_H__
#define __HMAC_ENCAP_FRAME_AP_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 1 其他头文件包含 */
#include "oal_ext_if.h"
#include "mac_vap.h"
#include "hmac_user.h"
#include "hmac_vap.h"
#include "securec.h"
#include "securectype.h"

/* 2 宏定义 */
typedef enum {
    /* 加密方式为open */
    HMAC_AP_AUTH_SEQ1_OPEN_ANY = 0,
    /* 加密方式为wep,处理重传帧 */
    HMAC_AP_AUTH_SEQ1_WEP_NOT_RESEND,
    /* 加密方式为wep,处理非重传帧 */
    HMAC_AP_AUTH_SEQ1_WEP_RESEND,
    /* 加密方式为open */
    HMAC_AP_AUTH_SEQ3_OPEN_ANY,
    /* 加密方式为WEP,assoc状态为auth comlete */
    HMAC_AP_AUTH_SEQ3_WEP_COMPLETE,
    /* 加密方式为WEP,assoc状态为assoc */
    HMAC_AP_AUTH_SEQ3_WEP_ASSOC,
    /* 什么也不做 */
    HMAC_AP_AUTH_DUMMY,

    HMAC_AP_AUTH_BUTT
} hmac_ap_auth_process_code_enum;
typedef uint8_t hmac_ap_auth_process_code_enum_uint8;

/* 3 枚举定义 */
/* 4 全局变量声明 */
/* 5 消息头定义 */
/* 6 消息定义 */
/* 7 STRUCT定义 */
typedef struct tag_hmac_auth_rsp_param_stru {
    /* 收到auth request是否为重传帧 */
    uint8_t uc_auth_resend;
    /* 记录是否为wep */
    oal_bool_enum_uint8 en_is_wep_allowed;
    /* 记录认证的类型 */
    uint16_t us_auth_type;
    /* 记录函数处理前，user的关联状态 */
    mac_user_asoc_state_enum_uint8 en_user_asoc_state;
    uint8_t uc_pad[3];

} hmac_auth_rsp_param_stru;

typedef hmac_ap_auth_process_code_enum_uint8 (*hmac_auth_rsp_fun)(mac_vap_stru *pst_mac_vap,
                                                                  hmac_auth_rsp_param_stru *pst_auth_rsp_param,
                                                                  uint8_t *puc_code,
                                                                  mac_user_asoc_state_enum_uint8 *pst_usr_ass_stat);

typedef struct tag_hmac_auth_rsp_handle_stru {
    hmac_auth_rsp_param_stru st_auth_rsp_param;
    hmac_auth_rsp_fun st_auth_rsp_fun;
} hmac_auth_rsp_handle_stru;
/* 8 UNION定义 */
/* 9 OTHERS定义 */

OAL_STATIC OAL_INLINE void hmac_mgmt_encap_chtxt(uint8_t *puc_frame,
                                                     uint8_t *puc_chtxt,
                                                     uint16_t *pus_auth_rsp_len,
                                                     hmac_user_stru *pst_hmac_user_sta)
{
    int32_t l_ret;

    /* Challenge Text Element                  */
    /* --------------------------------------- */
    /* |Element ID | Length | Challenge Text | */
    /* --------------------------------------- */
    /* | 1         |1       |1 - 253         | */
    /* --------------------------------------- */
    puc_frame[6] = MAC_EID_CHALLENGE;
    puc_frame[7] = WLAN_CHTXT_SIZE;

    /* 将challenge text拷贝到帧体中去 */
    l_ret = memcpy_s(&puc_frame[8], WLAN_CHTXT_SIZE, puc_chtxt, WLAN_CHTXT_SIZE);

    /* 认证帧长度增加Challenge Text Element的长度 */
    *pus_auth_rsp_len += (WLAN_CHTXT_SIZE + MAC_IE_HDR_LEN);

    /* 保存明文的challenge text */
    l_ret += memcpy_s(pst_hmac_user_sta->auc_ch_text, WLAN_CHTXT_SIZE, &puc_frame[8], WLAN_CHTXT_SIZE);
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "hmac_mgmt_encap_chtxt::memcpy fail!");
    }
}

/* 10 函数声明 */
uint16_t hmac_encap_auth_rsp(mac_vap_stru *pst_mac_vap, oal_netbuf_stru *pst_auth_rsp,
                                          oal_netbuf_stru *pst_auth_req, uint8_t *puc_chtxt, uint8_t uc_chtxt_len);

#if defined(_PRE_WLAN_FEATURE_SAE)
oal_err_code_enum hmac_encap_auth_rsp_get_user_idx(mac_vap_stru *pst_mac_vap,
                                                              uint8_t *puc_mac_addr,
                                                              uint8_t uc_mac_len,
                                                              uint8_t uc_is_seq1,
                                                              uint8_t *puc_auth_resend,
                                                              uint16_t *pus_user_index);
uint32_t hmac_get_assoc_comeback_time(mac_vap_stru *pst_mac_vap,
                                               hmac_user_stru *pst_hmac_user);
#endif
uint32_t hmac_mgmt_encap_asoc_rsp_ap(mac_vap_stru *pst_mac_ap,
                                                  const unsigned char *puc_sta_addr,
                                                  uint16_t us_assoc_id,
                                                  mac_status_code_enum_uint16 en_status_code,
                                                  uint8_t *puc_asoc_rsp,
                                                  uint16_t us_type);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hmac_encap_frame_ap.h */
