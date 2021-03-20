

#ifndef __HMAC_SCAN_H__
#define __HMAC_SCAN_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 1 ����ͷ�ļ����� */
#include "oal_ext_if.h"
#include "frw_ext_if.h"
#include "hmac_vap.h"
#include "mac_device.h"
#include "hmac_device.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_SCAN_H
/* 2 �궨�� */
/* ɨ�赽��bss���ϻ�ʱ�䣬С�ڴ�ֵ���·���ɨ������ʱ����ɾ����bss��Ϣ */
#define HMAC_SCAN_MAX_SCANNED_BSS_EXPIRE 25000 /* 25000 milliseconds */

/* ɨ��������Ч�Ըߵ�ʱ�䷶Χ������������Ŀ�����ѡ�Ȳ��� */
#define HMAC_SCAN_MAX_VALID_SCANNED_BSS_EXPIRE 5000 /* 5000 milliseconds */

/* ɨ�赽��bss��rssi�ϻ�ʱ�� */
#define HMAC_SCAN_MAX_SCANNED_RSSI_EXPIRE 400       /* 400 milliseconds */

#define HMAC_INIT_SCAN_TIMEOUT_MS 10000
/* ɨ�赽��bss��rssi�ź����ޣ��������²����Ƕ�obss��Ӱ�� */
#define HMAC_OBSS_RSSI_TH (-85)
/* 3 ö�ٶ��� */
/* ʹ�ܡ���ֹɨ��ĵ������� */
typedef enum {
    HMAC_BGSCAN_DISABLE = 0,
    HMAC_BGSCAN_ENABLE = 1,
    HMAC_SCAN_DISABLE = 2,

    HMAC_BUTT
} hmac_scan_state_enum;
typedef uint8_t hmac_scan_state_enum_uint8;

typedef enum {
    MAC_TRY_INIT_SCAN_VAP_UP,
    MAC_TRY_INIT_SCAN_SET_CHANNEL,
    MAC_TRY_INIT_SCAN_START_DBAC,
    MAC_TRY_INIT_SCAN_RESCAN,

    MAC_TRY_INIT_SCAN_BUTT
} mac_try_init_scan_type;
typedef uint8_t mac_try_init_scan_type_enum_uint8;

/* 4 ȫ�ֱ������� */
extern uint32_t g_pd_bss_expire_time;

/* 5 ��Ϣͷ���� */
/* 6 ��Ϣ���� */
/* 7 STRUCT���� */
/* 8 UNION���� */
/* 9 OTHERS���� */
/* 10 �������� */
/* HMAC SCAN����ӿ� */
void hmac_scan_print_scanned_bss_info(uint8_t uc_device_id);

mac_bss_dscr_stru *hmac_scan_find_scanned_bss_dscr_by_index(uint8_t uc_device_id,
                                                                       uint32_t ul_bss_index);
hmac_scanned_bss_info *hmac_scan_find_scanned_bss_by_bssid(
    hmac_bss_mgmt_stru *pst_bss_mgmt, uint8_t *puc_bssid);
void *hmac_scan_get_scanned_bss_by_bssid(mac_vap_stru *pst_mac_vap,
                                                    uint8_t *puc_mac_addr);
void hmac_scan_clean_scan(hmac_scan_stru *pst_scan);
uint32_t hmac_scan_proc_scanned_bss(frw_event_mem_stru *pst_event_mem);
uint32_t hmac_scan_proc_scan_comp_event(frw_event_mem_stru *pst_event_mem);
uint32_t hmac_scan_proc_scan_req_event_exception(hmac_vap_stru *pst_hmac_vap,
                                                              void *p_params);
uint32_t hmac_scan_proc_scan_req_event(hmac_vap_stru *pst_hmac_vap, void *p_params);
uint32_t hmac_scan_proc_sched_scan_req_event(hmac_vap_stru *pst_hmac_vap,
                                                          void *p_params);
void hmac_scan_init(hmac_device_stru *pst_hmac_device);
void hmac_scan_exit(hmac_device_stru *pst_hmac_device);
uint32_t hmac_scan_process_chan_result_event(frw_event_mem_stru *pst_event_mem);
uint32_t hmac_bgscan_enable(mac_vap_stru *pst_mac_vap, uint16_t us_len,
                                         uint8_t *puc_param);
uint32_t hmac_init_scan_process(hmac_device_stru *pst_hmac_dev,
                                             mac_vap_stru *pst_mac_vap,
                                             hmac_acs_cfg_stru *pst_acs_cfg);
uint32_t hmac_init_scan_try(mac_device_stru *pst_mac_device, mac_vap_stru *pst_mac_vap,
                                         mac_try_init_scan_type en_type, hmac_acs_cfg_stru *pst_acs_cfg);
uint32_t hmac_init_scan_cancel_timer(hmac_device_stru *pst_hmac_dev);
oal_bool_enum_uint8 hmac_device_in_init_scan(mac_device_stru *pst_mac_device);
uint32_t hmac_init_scan_sync_channel(hmac_scan_record_stru *pst_scan_record,
                                              hmac_device_stru *pst_hmac_dev);
uint32_t hmac_start_all_bss_of_device(hmac_device_stru *pst_dev);

void hmac_scan_set_sour_mac_addr_in_probe_req(hmac_vap_stru *pst_hmac_vap,
                                                             uint8_t *puc_sour_mac_addr,
                                                             oal_bool_enum_uint8 en_is_rand_mac_addr_scan,
                                                             oal_bool_enum_uint8 en_is_p2p0_scan);

uint32_t  hmac_scan_check_is_dispatch_scan_req(hmac_vap_stru *pst_hmac_vap,
    hmac_device_stru *pst_hmac_device);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hmac_scan.h */
