

#ifndef __HMAC_CHAN_MGMT_H__
#define __HMAC_CHAN_MGMT_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 1 ����ͷ�ļ����� */
#include "hmac_vap.h"
#include "mac_regdomain.h"
#ifdef _PRE_WLAN_FEATURE_DFS
#include "hmac_dfs.h"
#endif

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_CHAN_MGMT_H
/* 2 �궨�� */
#define HMAC_CHANNEL_SWITCH_COUNT 5

/* 3 ö�ٶ��� */
typedef enum {
    HMAC_OP_ALLOWED = BIT0,
    HMAC_SCA_ALLOWED = BIT1,
    HMAC_SCB_ALLOWED = BIT2,
} hmac_chan_op_enum;
typedef uint8_t hmac_chan_op_enum_uint8;

typedef enum {
    HMAC_NETWORK_SCA = 0,
    HMAC_NETWORK_SCB = 1,

    HMAC_NETWORK_BUTT,
} hmac_network_type_enum;
typedef uint8_t hmac_network_type_enum_uint8;

typedef enum {
    MAC_CHNL_AV_CHK_NOT_REQ = 0,  /* ����Ҫ�����ŵ�ɨ�� */
    MAC_CHNL_AV_CHK_IN_PROG = 1,  /* ���ڽ����ŵ�ɨ�� */
    MAC_CHNL_AV_CHK_COMPLETE = 2, /* �ŵ�ɨ������� */

    MAC_CHNL_AV_CHK_BUTT,
} mac_chnl_av_chk_enum;
typedef uint8_t mac_chnl_av_chk_enum_uint8;

/* 4 ȫ�ֱ������� */
/* 5 ��Ϣͷ���� */
/* 6 ��Ϣ���� */
/* 7 STRUCT���� */
typedef struct {
    uint16_t aus_num_networks[HMAC_NETWORK_BUTT];
    hmac_chan_op_enum_uint8 en_chan_op;
    uint8_t auc_resv[3];  // ����3�ֽ�
} hmac_eval_scan_report_stru;

typedef struct {
    uint8_t uc_idx;   /* �ŵ������� */
    uint16_t us_freq; /* �ŵ�Ƶ�� */
    uint8_t auc_resv;
} hmac_dfs_channel_info_stru;

/* Ƶ���ŵ�����ӳ���ϵ */
/* 8 UNION���� */
/* 9 OTHERS���� */
/* 10 �������� */
void hmac_chan_reval_bandwidth_sta(mac_vap_stru *pst_mac_vap, uint32_t ul_change);
void hmac_chan_disable_machw_tx(mac_vap_stru *pst_mac_vap);
void hmac_chan_enable_machw_tx(mac_vap_stru *pst_mac_vap);
void hmac_chan_multi_select_channel_mac(mac_vap_stru *pst_mac_vap,
                                        uint8_t uc_channel,
                                        wlan_channel_bandwidth_enum_uint8 en_bandwidth);
uint32_t hmac_start_bss_in_available_channel(hmac_vap_stru *pst_hmac_vap);
uint32_t hmac_chan_restart_network_after_switch(mac_vap_stru *pst_mac_vap);
uint32_t hmac_chan_switch_to_new_chan_complete(frw_event_mem_stru *pst_event_mem);
void hmac_chan_sync(mac_vap_stru *pst_mac_vap,
                    uint8_t uc_channel, wlan_channel_bandwidth_enum_uint8 en_bandwidth,
                    oal_bool_enum_uint8 en_switch_immediately);
void hmac_chan_update_40M_intol_user(mac_vap_stru *pst_mac_vap);

uint32_t hmac_chan_start_bss(hmac_vap_stru *pst_hmac_vap, mac_channel_stru *pst_channel,
                             wlan_protocol_enum_uint8 en_protocol);
void hmac_40M_intol_sync_data(mac_vap_stru *pst_mac_vap,
                              wlan_channel_bandwidth_enum_uint8 en_40M_bandwidth,
                              oal_bool_enum_uint8 en_40M_intol_user);
void hmac_chan_initiate_switch_to_new_channel(mac_vap_stru *pst_mac_vap,
                                              uint8_t uc_channel,
                                              wlan_channel_bandwidth_enum_uint8 en_bandwidth);
uint32_t hmac_check_ap_channel_follow_sta(mac_vap_stru *pst_check_mac_vap,
                                          const mac_channel_stru *pst_set_mac_channel,
                                          uint8_t *puc_ap_follow_channel);
uint16_t mac_get_center_freq1_from_freq_and_bandwidth(uint16_t freq,
    wlan_channel_bandwidth_enum_uint8 en_bandwidth);

/* 11 inline�������� */

OAL_STATIC OAL_INLINE void hmac_chan_initiate_switch_to_20MHz_ap(mac_vap_stru *pst_mac_vap)
{
    /* ����VAP����ģʽΪ20MHz */
    pst_mac_vap->st_channel.en_bandwidth = WLAN_BAND_WIDTH_20M;

    /* ���ô����л�״̬��������������һ��DTIMʱ���л���20MHz���� */
    pst_mac_vap->st_ch_switch_info.en_bw_switch_status = WLAN_BW_SWITCH_40_TO_20;
}


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 hmac_chan_scan_availability(mac_device_stru *pst_mac_device,
                                                                      mac_ap_ch_info_stru *pst_channel_info)
{
#ifdef _PRE_WLAN_FEATURE_DFS
    if (OAL_FALSE == mac_dfs_get_dfs_enable(pst_mac_device)) {
        return OAL_TRUE;
    }

    return ((pst_channel_info->en_ch_status != MAC_CHAN_NOT_SUPPORT) &&
            (pst_channel_info->en_ch_status != MAC_CHAN_BLOCK_DUE_TO_RADAR));
#else
    return OAL_TRUE;
#endif
}


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 hmac_chan_is_ch_op_allowed(
    hmac_eval_scan_report_stru *pst_chan_scan_report, uint8_t uc_chan_idx)
{
    return (pst_chan_scan_report[uc_chan_idx].en_chan_op & HMAC_OP_ALLOWED);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hmac_chan_mgmt.h */
