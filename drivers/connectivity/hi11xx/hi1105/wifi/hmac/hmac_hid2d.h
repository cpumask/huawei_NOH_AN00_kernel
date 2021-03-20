

#ifndef __HMAC_HID2D_H__
#define __HMAC_HID2D_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 1 其他头文件包含 */
#include "hmac_vap.h"
#include "hmac_device.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_HID2D_H

#ifdef _PRE_WLAN_FEATURE_HID2D

/* 2 宏定义 */
#define HMAC_HID2D_CHANNEL_NUM_2G                  32     /* 2G上全部的信道 */
#define HMAC_HID2D_CHANNEL_NUM_5G                  73     /* 5G上全部的信道 */
#define HMAC_HID2D_MAX_SCAN_CHAN_NUM               25     /* 最多的扫描信道数目 */
#define HMAC_HID2D_SCAN_CHAN_NUM_FOR_APK           6      /* 发布会扫描信道数 */
#define HMAC_HID2D_SCAN_TIMES_PER_CHAN_FOR_APK     10     /* 发布会每个信道扫描次数 */
#define HMAC_HID2D_SCAN_TIME_PER_CHAN_APK          150    /* 发布会扫描每个信道的测量时间为150ms */
#define HMAC_HID2D_SCAN_TIME_PER_CHAN_ACS          15     /* ACS模式下每个信道的测量时间为15ms */
#define HMAC_HID2D_SCAN_TIMER_CYCLE_MS             1000
#define HMAC_HID2D_DECAY                           90     /* 滑动平均中的老化系数 */
#define HMAC_HID2D_CHAN_LOAD_SHIFT                 10     /* 计算信道繁忙度的扩大因子 */
#define HMAC_HID2D_CHAN_LOAD_DIFF                  50
#define HMAC_HID2D_NOISE_DIFF                      5

/* 3 枚举定义 */
/* 4 全局变量声明 */
/* 5 消息头定义 */
/* 6 消息定义 */
/* 7 STRUCT定义 */
/* 全局待选信道中每个信道的表示 */
typedef struct {
    uint8_t idx;                            /* 序号 */
    uint8_t uc_chan_idx;                    /* 主20MHz信道索引号 */
    uint8_t uc_chan_number;                 /* 主20MHz信道号 */
    wlan_channel_bandwidth_enum_uint8 en_bandwidth; /* 带宽模式 */
}hmac_hid2d_chan_stru;

/* 待选信道的信息结构体 */
typedef struct {
    uint8_t good_cnt;                       /* 该信道信道质量最好的周期数 */
    int16_t update_num;                     /* 该信道信道信息更新次数，用于滑动平均 */
    int16_t chan_noise_rssi;                /* 该信道的底噪 */
    int16_t chan_load;                      /* 该信道的占空比: free_time / total_time */
    int16_t estimated_snr;                  /* 该信道预估的SNR */
    int32_t estimated_throughput;           /* 该信道预估的吞吐量 */
}hmac_hid2d_chan_info_stru;

typedef struct {
    uint8_t uc_vap_id;                          /* P2P对应的mac vap id */
    uint8_t uc_apk_scan;                        /* 区别是apk触发的5G全信道扫描，还是一般业务模式下的扫描上报 */
    uint8_t uc_acs_mode;                        /* 自动信道切换算法使能 */

    wlan_channel_band_enum_uint8 uc_cur_band;     /* HiD2D建立的P2P所在频段 */
    uint8_t uc_cur_chan_idx;                    /* 当前信道在全局待选信道列表中所对应的下标 */

    uint8_t uc_best_chan_idx;                   /* 最好的信道号 */
    uint8_t uc_cur_scan_chan_idx;               /* 当前扫描信道在扫描list中的下标 */
    int16_t update_num;
    int16_t max_update_num;                     /* 用于滑动平均 */
    uint16_t us_scan_times_for_apk_scan;        /* APK扫描模式的扫描次数 */
    frw_timeout_stru st_scan_chan_timer;          /* 信道扫描周期定时器 */
    hmac_hid2d_chan_info_stru chan_info_5G[HMAC_HID2D_CHANNEL_NUM_5G]; /* 5G待选信道信息 */
} hmac_hid2d_auto_channel_switch_stru;

extern hmac_hid2d_chan_stru g_aus_channel_candidate_list_5G[];
extern uint8_t g_scan_chan_list[];
extern uint8_t g_scan_chan_num_max;
extern hmac_hid2d_auto_channel_switch_stru g_st_hmac_hid2d_acs_info;
/* 8 UNION定义 */
/* 9 OTHERS定义 */
/* 10 函数声明 */
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
