

#ifndef __HMAC_MBO_H__
#define __HMAC_MBO_H__

/* 1 其他头文件包含 */
#include "hmac_11v.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_MBO_H
/* 2 宏定义 */
/* 3 枚举定义 */
#ifdef _PRE_WLAN_FEATURE_MBO

typedef enum {
    MAC_GLOBAL_OPERATING_CLASS_NUMBER81 = 81,
    MAC_GLOBAL_OPERATING_CLASS_NUMBER115 = 115,

    MAC_GLOBAL_OPERATING_CLASS_BUFF,
} mac_global_operaing_class_enum;
typedef uint8_t mac_global_operaing_class_enum_uint8;

/* 4 全局变量声明 */
/* 5 消息头定义 */
/* 6 消息定义 */
/* 7 STRUCT定义 */
/* 8 UNION定义 */
/* 9 OTHERS定义 */
/* 10 函数声明 */
void mac_set_mbo_ie(void *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len);
void mac_set_mbo_ie_ap(void *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len);
void mac_set_supported_operating_classes_ie(void *pst_vap, uint8_t *puc_buffer, uint8_t *puc_ie_len);
void hmac_scan_update_bss_assoc_disallowed_attr(mac_bss_dscr_stru *pst_bss_dscr,
                                                uint8_t *puc_frame_body,
                                                uint16_t us_frame_len);
void hmac_handle_ie_specific_mbo(uint8_t *puc_ie_data,
                                 hmac_neighbor_bss_info_stru *pst_bss_list_alloc,
                                 uint8_t uc_bss_list_index);
uint8_t *mac_find_mbo_attribute(uint8_t uc_eid, uint8_t *puc_ies, int32_t l_len);
uint32_t hmac_mbo_check_is_assoc_or_re_assoc_allowed(mac_vap_stru *pst_mac_vap,
                                                     mac_conn_param_stru *pst_connect_param,
                                                     mac_bss_dscr_stru *pst_bss_dscr);
#endif
#endif /* end of hmac_mbo.h */

