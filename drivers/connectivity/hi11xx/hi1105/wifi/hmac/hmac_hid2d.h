

#ifndef __HMAC_HID2D_H__
#define __HMAC_HID2D_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 1 ����ͷ�ļ����� */
#include "hmac_vap.h"
#include "hmac_device.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_HID2D_H

#ifdef _PRE_WLAN_FEATURE_HID2D

/* 2 �궨�� */
#define HMAC_HID2D_CHANNEL_NUM_2G                  32     /* 2G��ȫ�����ŵ� */
#define HMAC_HID2D_CHANNEL_NUM_5G                  73     /* 5G��ȫ�����ŵ� */
#define HMAC_HID2D_MAX_SCAN_CHAN_NUM               25     /* ����ɨ���ŵ���Ŀ */
#define HMAC_HID2D_SCAN_CHAN_NUM_FOR_APK           6      /* ������ɨ���ŵ��� */
#define HMAC_HID2D_SCAN_TIMES_PER_CHAN_FOR_APK     10     /* ������ÿ���ŵ�ɨ����� */
#define HMAC_HID2D_SCAN_TIME_PER_CHAN_APK          150    /* ������ɨ��ÿ���ŵ��Ĳ���ʱ��Ϊ150ms */
#define HMAC_HID2D_SCAN_TIME_PER_CHAN_ACS          15     /* ACSģʽ��ÿ���ŵ��Ĳ���ʱ��Ϊ15ms */
#define HMAC_HID2D_SCAN_TIMER_CYCLE_MS             1000
#define HMAC_HID2D_DECAY                           90     /* ����ƽ���е��ϻ�ϵ�� */
#define HMAC_HID2D_CHAN_LOAD_SHIFT                 10     /* �����ŵ���æ�ȵ��������� */
#define HMAC_HID2D_CHAN_LOAD_DIFF                  50
#define HMAC_HID2D_NOISE_DIFF                      5

/* 3 ö�ٶ��� */
/* 4 ȫ�ֱ������� */
/* 5 ��Ϣͷ���� */
/* 6 ��Ϣ���� */
/* 7 STRUCT���� */
/* ȫ�ִ�ѡ�ŵ���ÿ���ŵ��ı�ʾ */
typedef struct {
    uint8_t idx;                            /* ��� */
    uint8_t uc_chan_idx;                    /* ��20MHz�ŵ������� */
    uint8_t uc_chan_number;                 /* ��20MHz�ŵ��� */
    wlan_channel_bandwidth_enum_uint8 en_bandwidth; /* ����ģʽ */
}hmac_hid2d_chan_stru;

/* ��ѡ�ŵ�����Ϣ�ṹ�� */
typedef struct {
    uint8_t good_cnt;                       /* ���ŵ��ŵ�������õ������� */
    int16_t update_num;                     /* ���ŵ��ŵ���Ϣ���´��������ڻ���ƽ�� */
    int16_t chan_noise_rssi;                /* ���ŵ��ĵ��� */
    int16_t chan_load;                      /* ���ŵ���ռ�ձ�: free_time / total_time */
    int16_t estimated_snr;                  /* ���ŵ�Ԥ����SNR */
    int32_t estimated_throughput;           /* ���ŵ�Ԥ���������� */
}hmac_hid2d_chan_info_stru;

typedef struct {
    uint8_t uc_vap_id;                          /* P2P��Ӧ��mac vap id */
    uint8_t uc_apk_scan;                        /* ������apk������5Gȫ�ŵ�ɨ�裬����һ��ҵ��ģʽ�µ�ɨ���ϱ� */
    uint8_t uc_acs_mode;                        /* �Զ��ŵ��л��㷨ʹ�� */

    wlan_channel_band_enum_uint8 uc_cur_band;     /* HiD2D������P2P����Ƶ�� */
    uint8_t uc_cur_chan_idx;                    /* ��ǰ�ŵ���ȫ�ִ�ѡ�ŵ��б�������Ӧ���±� */

    uint8_t uc_best_chan_idx;                   /* ��õ��ŵ��� */
    uint8_t uc_cur_scan_chan_idx;               /* ��ǰɨ���ŵ���ɨ��list�е��±� */
    int16_t update_num;
    int16_t max_update_num;                     /* ���ڻ���ƽ�� */
    uint16_t us_scan_times_for_apk_scan;        /* APKɨ��ģʽ��ɨ����� */
    frw_timeout_stru st_scan_chan_timer;          /* �ŵ�ɨ�����ڶ�ʱ�� */
    hmac_hid2d_chan_info_stru chan_info_5G[HMAC_HID2D_CHANNEL_NUM_5G]; /* 5G��ѡ�ŵ���Ϣ */
} hmac_hid2d_auto_channel_switch_stru;

extern hmac_hid2d_chan_stru g_aus_channel_candidate_list_5G[];
extern uint8_t g_scan_chan_list[];
extern uint8_t g_scan_chan_num_max;
extern hmac_hid2d_auto_channel_switch_stru g_st_hmac_hid2d_acs_info;
/* 8 UNION���� */
/* 9 OTHERS���� */
/* 10 �������� */
uint32_t hmac_hid2d_set_acs_mode(mac_vap_stru *pst_mac_vap, uint8_t uc_acs_mode);
uint32_t hmac_hid2d_scan_chan_start(mac_vap_stru *pst_mac_vap, mac_device_stru *pst_mac_device);
uint32_t hmac_hid2d_scan_complete_handler(hmac_scan_record_stru *pst_scan_record,
    uint8_t uc_scan_idx);
uint32_t hmac_hid2d_cur_chan_stat_handler(mac_vap_stru *pst_mac_vap, uint8_t uc_len,
    uint8_t *puc_param);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif


#endif /* end of hmac_hid2d.h */
