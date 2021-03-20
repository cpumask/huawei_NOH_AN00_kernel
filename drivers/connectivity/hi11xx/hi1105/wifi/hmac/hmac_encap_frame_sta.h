

#ifndef __PREPARE_FRAME_STA_H__
#define __PREPARE_FRAME_STA_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 1 其他头文件包含 */
#include "oal_ext_if.h"
#include "oal_types.h"
#include "hmac_vap.h"

/* 2 宏定义 */
// for CSEC cycle complexity
#ifdef _PRE_WLAN_NARROW_BAND
#define HMAC_MGMT_ASOC_REQ_ENABLE_NB(_pst_scaned_bss, _pst_mac_vap) \
    (((_pst_scaned_bss)->st_bss_dscr_info.en_nb_capable == OAL_TRUE) && \
    ((_pst_mac_vap)->st_nb.en_open == OAL_TRUE) && ((_pst_mac_vap)->st_cap_flag.bit_nb == OAL_TRUE))
#endif

#define HMAC_MGMT_ASOC_REQ_ENABLE_VHT_CAPABLE(_pst_scaned_bss) \
    (((_pst_scaned_bss)->st_bss_dscr_info.en_vht_capable == OAL_TRUE) &&  \
    ((_pst_scaned_bss)->st_bss_dscr_info.en_vendor_vht_capable == OAL_FALSE))

/* 3 枚举定义 */
/* 4 全局变量声明 */
/* 5 消息头定义 */
/* 6 消息定义 */
/* 7 STRUCT定义 */
/* 8 UNION定义 */
/* 9 OTHERS定义 */
/* 10 函数声明 */
uint32_t hmac_mgmt_encap_asoc_req_sta(hmac_vap_stru *pst_hmac_sta,
                                                   uint8_t *puc_req_frame,
                                                   uint8_t *puc_curr_bssid);
uint16_t hmac_mgmt_encap_auth_req(hmac_vap_stru *pst_sta, uint8_t *puc_mgmt_frame);
uint16_t hmac_mgmt_encap_auth_req_seq3(hmac_vap_stru *pst_sta,
                                                    uint8_t *puc_mgmt_frame,
                                                    uint8_t *puc_mac_hrd);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hmac_encap_frame_sta.h */
