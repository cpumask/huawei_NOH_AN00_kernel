

#ifdef _PRE_WLAN_FEATURE_HID2D
/* 1 头文件包含 */
#include "mac_ie.h"
#include "mac_vap.h"
#include "mac_device.h"
#include "wlan_types.h"
#include "hmac_chan_mgmt.h"
#include "mac_device.h"
#include "hmac_hid2d.h"
#include "hmac_scan.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_HID2D_C

/* 2 全局变量定义 */
/* 全局待选信道覆盖所有信道，实际运行中可选择的待选信道根据GO与GC所支持的信道列表求交集所得，
   用全局待选信道的序号表示 */
/* 2.4G全频段待选信道 */
hmac_hid2d_chan_stru g_aus_channel_candidate_list_2G[HMAC_HID2D_CHANNEL_NUM_2G] = {
    // idx  chan_idx  chan_num   bandwidth
    {0,     0,      1,    WLAN_BAND_WIDTH_20M},
    {1,     1,      2,    WLAN_BAND_WIDTH_20M},
    {2,     2,      3,    WLAN_BAND_WIDTH_20M},
    {3,     3,      4,    WLAN_BAND_WIDTH_20M},
    {4,     4,      5,    WLAN_BAND_WIDTH_20M},
    {5,     5,      6,    WLAN_BAND_WIDTH_20M},
    {6,     6,      7,    WLAN_BAND_WIDTH_20M},
    {7,     7,      8,    WLAN_BAND_WIDTH_20M},
    {8,     8,      9,    WLAN_BAND_WIDTH_20M},
    {9,     9,      10,   WLAN_BAND_WIDTH_20M},
    {10,    10,     11,   WLAN_BAND_WIDTH_20M},
    {11,    11,     12,   WLAN_BAND_WIDTH_20M},
    {12,    12,     13,   WLAN_BAND_WIDTH_20M},
    {13,    13,     14,   WLAN_BAND_WIDTH_20M},
    {14,    0,      1,    WLAN_BAND_WIDTH_40PLUS},
    {15,    4,      5,    WLAN_BAND_WIDTH_40PLUS},
    {16,    6,      7,    WLAN_BAND_WIDTH_40PLUS},
    {17,    8,      9,    WLAN_BAND_WIDTH_40PLUS},
    {18,    1,      2,    WLAN_BAND_WIDTH_40PLUS},
    {19,    5,      6,    WLAN_BAND_WIDTH_40PLUS},
    {20,    7,      8,    WLAN_BAND_WIDTH_40PLUS},
    {21,    12,     13,   WLAN_BAND_WIDTH_40MINUS},
    {22,    4,      5,    WLAN_BAND_WIDTH_40MINUS},
    {23,    6,      7,    WLAN_BAND_WIDTH_40MINUS},
    {24,    10,     11,   WLAN_BAND_WIDTH_40MINUS},
    {25,    11,     12,   WLAN_BAND_WIDTH_40MINUS},
    {26,    9,      10,   WLAN_BAND_WIDTH_40MINUS},
    {27,    2,      3,    WLAN_BAND_WIDTH_40PLUS},
    {28,    3,      4,    WLAN_BAND_WIDTH_40PLUS},
    {29,    5,      6,    WLAN_BAND_WIDTH_40MINUS},
    {30,    7,      8,    WLAN_BAND_WIDTH_40MINUS},
    {31,    8,      9,    WLAN_BAND_WIDTH_40MINUS},
};
/* 5G全频段待选信道 */
hmac_hid2d_chan_stru g_aus_channel_candidate_list_5G[HMAC_HID2D_CHANNEL_NUM_5G] = {
    // idx  chan_idx  chan_num   bandwidth
    {0,     0,      36,    WLAN_BAND_WIDTH_20M},
    {1,     1,      40,    WLAN_BAND_WIDTH_20M},
    {2,     2,      44,    WLAN_BAND_WIDTH_20M},
    {3,     3,      48,    WLAN_BAND_WIDTH_20M},
    {4,     0,      36,    WLAN_BAND_WIDTH_40PLUS},
    {5,     1,      40,    WLAN_BAND_WIDTH_40MINUS},
    {6,     2,      44,    WLAN_BAND_WIDTH_40PLUS},
    {7,     3,      48,    WLAN_BAND_WIDTH_40MINUS},
    {8,     0,      36,    WLAN_BAND_WIDTH_80PLUSPLUS},
    {9,     1,      40,    WLAN_BAND_WIDTH_80MINUSPLUS},
    {10,    2,      44,    WLAN_BAND_WIDTH_80PLUSMINUS},
    {11,    3,      48,    WLAN_BAND_WIDTH_80MINUSMINUS},
    {12,    4,      52,    WLAN_BAND_WIDTH_20M},
    {13,    5,      56,    WLAN_BAND_WIDTH_20M},
    {14,    6,      60,    WLAN_BAND_WIDTH_20M},
    {15,    7,      64,    WLAN_BAND_WIDTH_20M},
    {16,    4,      52,    WLAN_BAND_WIDTH_40PLUS},
    {17,    5,      56,    WLAN_BAND_WIDTH_40MINUS},
    {18,    6,      60,    WLAN_BAND_WIDTH_40PLUS},
    {19,    7,      64,    WLAN_BAND_WIDTH_40MINUS},
    {20,    4,      52,    WLAN_BAND_WIDTH_80PLUSPLUS},
    {21,    5,      56,    WLAN_BAND_WIDTH_80MINUSPLUS},
    {22,    6,      60,    WLAN_BAND_WIDTH_80PLUSMINUS},
    {23,    7,      64,    WLAN_BAND_WIDTH_80MINUSMINUS},
    {24,    8,      100,   WLAN_BAND_WIDTH_20M},
    {25,    9,      104,   WLAN_BAND_WIDTH_20M},
    {26,    10,     108,   WLAN_BAND_WIDTH_20M},
    {27,    11,     112,   WLAN_BAND_WIDTH_20M},
    {28,    8,      100,   WLAN_BAND_WIDTH_40PLUS},
    {29,    9,      104,   WLAN_BAND_WIDTH_40MINUS},
    {30,    10,     108,   WLAN_BAND_WIDTH_40PLUS},
    {31,    11,     112,   WLAN_BAND_WIDTH_40MINUS},
    {32,    8,      100,   WLAN_BAND_WIDTH_80PLUSPLUS},
    {33,    9,      104,   WLAN_BAND_WIDTH_80MINUSPLUS},
    {34,    10,     108,   WLAN_BAND_WIDTH_80PLUSMINUS},
    {35,    11,     112,   WLAN_BAND_WIDTH_80MINUSMINUS},
    {36,    12,     116,   WLAN_BAND_WIDTH_20M},
    {37,    13,     120,   WLAN_BAND_WIDTH_20M},
    {38,    14,     124,   WLAN_BAND_WIDTH_20M},
    {39,    15,     128,   WLAN_BAND_WIDTH_20M},
    {40,    12,     116,   WLAN_BAND_WIDTH_40PLUS},
    {41,    13,     120,   WLAN_BAND_WIDTH_40MINUS},
    {42,    14,     124,   WLAN_BAND_WIDTH_40PLUS},
    {43,    15,     128,   WLAN_BAND_WIDTH_40MINUS},
    {44,    12,     116,   WLAN_BAND_WIDTH_80PLUSPLUS},
    {45,    13,     120,   WLAN_BAND_WIDTH_80MINUSPLUS},
    {46,    14,     124,   WLAN_BAND_WIDTH_80PLUSMINUS},
    {47,    15,     128,   WLAN_BAND_WIDTH_80MINUSMINUS},
    {48,    16,     132,   WLAN_BAND_WIDTH_20M},
    {49,    17,     136,   WLAN_BAND_WIDTH_20M},
    {50,    18,     140,   WLAN_BAND_WIDTH_20M},
    {51,    19,     144,   WLAN_BAND_WIDTH_20M},
    {52,    16,     132,   WLAN_BAND_WIDTH_40PLUS},
    {53,    17,     136,   WLAN_BAND_WIDTH_40MINUS},
    {54,    18,     140,   WLAN_BAND_WIDTH_40PLUS},
    {55,    19,     144,   WLAN_BAND_WIDTH_40MINUS},
    {56,    16,     132,   WLAN_BAND_WIDTH_80PLUSPLUS},
    {57,    17,     136,   WLAN_BAND_WIDTH_80MINUSPLUS},
    {58,    18,     140,   WLAN_BAND_WIDTH_80PLUSMINUS},
    {59,    19,     144,   WLAN_BAND_WIDTH_80MINUSMINUS},
    {60,    20,     149,   WLAN_BAND_WIDTH_20M},
    {61,    21,     153,   WLAN_BAND_WIDTH_20M},
    {62,    22,     157,   WLAN_BAND_WIDTH_20M},
    {63,    23,     161,   WLAN_BAND_WIDTH_20M},
    {64,    20,     149,   WLAN_BAND_WIDTH_40PLUS},
    {65,    21,     153,   WLAN_BAND_WIDTH_40MINUS},
    {66,    22,     157,   WLAN_BAND_WIDTH_40PLUS},
    {67,    23,     161,   WLAN_BAND_WIDTH_40MINUS},
    {68,    20,     149,   WLAN_BAND_WIDTH_80PLUSPLUS},
    {69,    21,     153,   WLAN_BAND_WIDTH_80MINUSPLUS},
    {70,    22,     157,   WLAN_BAND_WIDTH_80PLUSMINUS},
    {71,    23,     161,   WLAN_BAND_WIDTH_80MINUSMINUS},
    {72,    24,     165,   WLAN_BAND_WIDTH_20M}
};
/* 扫描顺序数组: 初始化为发布会场景的扫描顺序，正常业务场景需要在开始扫描时根据实际待选信道列表确定并赋值 */
uint8_t g_scan_chan_list[HMAC_HID2D_MAX_SCAN_CHAN_NUM] = { 8, 20, 32, 44, 56, 68 };
/* 一轮扫描的数组值，确定g_scan_chan_list的边界，初始化值为发布会场景 */
uint8_t g_scan_chan_num_max = HMAC_HID2D_SCAN_CHAN_NUM_FOR_APK;
/* 实际可支持的信道列表数组 */
uint8_t g_supp_chan_list[HMAC_HID2D_CHANNEL_NUM_5G] = { 0 };
/* 实际可支持的信道个数，确定g_supp_chan_list的边界 */
uint8_t g_supp_chan_num_max = 0;
/* 扫描信息 */
hmac_hid2d_auto_channel_switch_stru g_st_hmac_hid2d_acs_info = {
    .uc_acs_mode = OAL_FALSE,
};

/* 3 函数声明 */
uint32_t hmac_hid2d_scan_chan_once(mac_vap_stru *pst_mac_vap, uint8_t chan_index, uint8_t is_apk_scan);
uint32_t hmac_hid2d_timeout_handle(void *p_arg);
uint8_t hmac_hid2d_get_best_chan(mac_channel_stru *pst_scan_chan);
uint32_t hmac_hid2d_apk_scan_handler(hmac_scan_record_stru *pst_scan_record, uint8_t uc_scan_idx,
    hmac_hid2d_auto_channel_switch_stru *pst_hmac_hid2d_acs_info);

/* 4 函数实现 */

uint32_t hmac_hid2d_find_p2p_vap(mac_device_stru *pst_mac_device, mac_vap_stru **ppst_mac_vap)
{
    uint8_t uc_vap_index;
    uint8_t uc_no_p2p_vap_flag = OAL_TRUE;
    mac_vap_stru *pst_p2p_mac_vap = NULL;

    /* 找到挂接在该device上的p2p vap */
    for (uc_vap_index = 0; uc_vap_index < pst_mac_device->uc_vap_num; uc_vap_index++) {
        pst_p2p_mac_vap = mac_res_get_mac_vap(pst_mac_device->auc_vap_id[uc_vap_index]);
        if (pst_p2p_mac_vap == OAL_PTR_NULL) {
            continue;
        }
        if (pst_p2p_mac_vap->en_p2p_mode != WLAN_LEGACY_VAP_MODE) {
            uc_no_p2p_vap_flag = OAL_FALSE;
            break;
        }
    }
    if (uc_no_p2p_vap_flag == OAL_TRUE) {
        oam_error_log0(0, OAM_SF_ANTI_INTF, "{hmac_hid2d_find_p2p_vap_ext::no p2p vap error!}");
        return OAL_FAIL;
    }
    *ppst_mac_vap = pst_p2p_mac_vap;
    return OAL_SUCC;
}
uint32_t hmac_hid2d_find_up_p2p_vap(mac_device_stru *pst_mac_device, mac_vap_stru **ppst_mac_vap)
{
    uint8_t uc_vap_index;
    uint8_t uc_no_p2p_vap_flag = OAL_TRUE;
    mac_vap_stru *pst_p2p_mac_vap = NULL;
    mac_vap_stru *mac_vap = NULL;

    /* 找到挂接在该device上的p2p vap */
    for (uc_vap_index = 0; uc_vap_index < pst_mac_device->uc_vap_num; uc_vap_index++) {
        pst_p2p_mac_vap = mac_res_get_mac_vap(pst_mac_device->auc_vap_id[uc_vap_index]);
        if (pst_p2p_mac_vap == OAL_PTR_NULL) {
            continue;
        }
        mac_vap = pst_p2p_mac_vap;
        if (pst_p2p_mac_vap->en_p2p_mode == WLAN_P2P_GO_MODE || pst_p2p_mac_vap->en_p2p_mode == WLAN_P2P_CL_MODE) {
            uc_no_p2p_vap_flag = OAL_FALSE;
            break;
        }
    }
    if (uc_no_p2p_vap_flag == OAL_TRUE) {
        oam_error_log0(0, 0, "{hmac_hid2d_find_p2p_vap::no up p2p vap error!}");
        *ppst_mac_vap = mac_vap; /* pst_mac_vap是非p2p GO/CL vap，用于p2p断链后的方案善后处理 */
        return OAL_FAIL;
    }
    *ppst_mac_vap = pst_p2p_mac_vap;
    return OAL_SUCC;
}

uint32_t hmac_hid2d_scan_chan_start(mac_vap_stru *pst_mac_vap, mac_device_stru *pst_mac_device)
{
    hmac_hid2d_chan_info_stru *pst_chan_info = NULL;
    hmac_hid2d_auto_channel_switch_stru *pst_hmac_hid2d_acs_info;
    uint8_t uc_scan_chan_idx;

    pst_hmac_hid2d_acs_info = &g_st_hmac_hid2d_acs_info;

    /* 如果auto channel switch算法使能，则直接退出 */
    if (pst_hmac_hid2d_acs_info->uc_acs_mode == OAL_TRUE) {
        return OAL_SUCC;
    }
    pst_hmac_hid2d_acs_info->uc_apk_scan = OAL_TRUE;
    /* 初始化信道信息 */
    pst_chan_info = pst_hmac_hid2d_acs_info->chan_info_5G;
    memset_s(pst_chan_info, OAL_SIZEOF(hmac_hid2d_chan_info_stru) * HMAC_HID2D_CHANNEL_NUM_5G,
        0, OAL_SIZEOF(hmac_hid2d_chan_info_stru) * HMAC_HID2D_CHANNEL_NUM_5G);
    /* 每个信道扫描10次 */
    pst_hmac_hid2d_acs_info->uc_cur_scan_chan_idx = 0;
    pst_hmac_hid2d_acs_info->update_num = 1;
    /* max_update_num用于滑动平均更新中的老化系数修正 */
    pst_hmac_hid2d_acs_info->max_update_num =
        (HMAC_HID2D_DECAY * 10 - 100) / (100 - HMAC_HID2D_DECAY); // 公式: (10x - 100) / (100 - x)
    /* 开启一次扫描 */
    uc_scan_chan_idx = g_scan_chan_list[pst_hmac_hid2d_acs_info->uc_cur_scan_chan_idx];

    hmac_hid2d_scan_chan_once(pst_mac_vap, uc_scan_chan_idx, OAL_TRUE);

    /* 开启定时器 */
    frw_timer_create_timer_m(&pst_hmac_hid2d_acs_info->st_scan_chan_timer,
        hmac_hid2d_timeout_handle,
        HMAC_HID2D_SCAN_TIMER_CYCLE_MS,
        pst_mac_device,
        OAL_FALSE,
        OAM_MODULE_ID_HMAC,
        pst_mac_device->ul_core_id);

    return OAL_SUCC;
}

uint32_t hmac_hid2d_scan_chan_once(mac_vap_stru *pst_mac_vap, uint8_t chan_index, uint8_t is_apk_scan)
{
    hmac_hid2d_auto_channel_switch_stru *pst_hmac_hid2d_acs_info = NULL;
    hmac_hid2d_chan_stru *pst_candidate_list = NULL;
    mac_channel_stru scan_channel;
    mac_scan_req_h2d_stru st_h2d_scan_params;
    hmac_vap_stru *pst_hmac_vap;
    uint32_t ul_ret;

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_mac_vap->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, 0, "{hmac_hid2d_scan_chan_once::pst_hmac_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    pst_hmac_hid2d_acs_info = &g_st_hmac_hid2d_acs_info;
    /* 整改后只支持apk模式下的扫描 */
    if (is_apk_scan == OAL_TRUE) {
        scan_channel.en_band = WLAN_BAND_5G;
        pst_candidate_list = g_aus_channel_candidate_list_5G;
    } else {
        /* 不应该走入这个分支 */
        oam_error_log0(pst_mac_vap->uc_vap_id, 0, "hmac_hid2d_scan_chan_once: unsupported mode!");
    }
    scan_channel.uc_chan_number = pst_candidate_list[chan_index].uc_chan_number;
    scan_channel.en_bandwidth = pst_candidate_list[chan_index].en_bandwidth;
    scan_channel.uc_chan_idx = pst_candidate_list[chan_index].uc_chan_idx;

    /* 设置扫描参数 */
    memset_s(&st_h2d_scan_params, OAL_SIZEOF(mac_scan_req_h2d_stru), 0, OAL_SIZEOF(mac_scan_req_h2d_stru));
    st_h2d_scan_params.st_scan_params.uc_vap_id = pst_mac_vap->uc_vap_id;
    st_h2d_scan_params.st_scan_params.en_bss_type = WLAN_MIB_DESIRED_BSSTYPE_INFRA;
    st_h2d_scan_params.st_scan_params.en_scan_type = WLAN_SCAN_TYPE_PASSIVE;
    st_h2d_scan_params.st_scan_params.uc_probe_delay = 0;
    st_h2d_scan_params.st_scan_params.uc_scan_func = MAC_SCAN_FUNC_MEAS | MAC_SCAN_FUNC_STATS;
    st_h2d_scan_params.st_scan_params.uc_max_scan_count_per_channel = 1;
    st_h2d_scan_params.st_scan_params.us_scan_time = (is_apk_scan == OAL_TRUE) ?
        HMAC_HID2D_SCAN_TIME_PER_CHAN_APK : HMAC_HID2D_SCAN_TIME_PER_CHAN_ACS;
    st_h2d_scan_params.st_scan_params.en_scan_mode = WLAN_SCAN_MODE_HID2D_SCAN;
    st_h2d_scan_params.st_scan_params.uc_channel_nums = 1;
    st_h2d_scan_params.st_scan_params.ast_channel_list[0] = scan_channel;

    ul_ret = hmac_scan_proc_scan_req_event(pst_hmac_vap, &st_h2d_scan_params);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log0(pst_mac_vap->uc_vap_id, 0, "hmac_hid2d_scan_chan_once:hmac scan req failed");
    }

    return ul_ret;
}

uint8_t hmac_hid2d_get_best_chan(mac_channel_stru *pst_scan_chan)
{
    hmac_hid2d_chan_info_stru *pst_chan_info;
    hmac_hid2d_auto_channel_switch_stru *pst_hmac_hid2d_acs_info;
    uint8_t uc_index;
    uint8_t uc_chan_idx;
    int16_t highest_chan_load = 0;
    int16_t lowest_noise_rssi = 0;
    uint8_t best_chan_of_chan_load = 0;
    uint8_t best_chan_of_noise_rssi = 0;

    pst_hmac_hid2d_acs_info = &g_st_hmac_hid2d_acs_info;
    pst_chan_info = pst_hmac_hid2d_acs_info->chan_info_5G;

    for (uc_index = 0; uc_index < g_scan_chan_num_max; uc_index++) {
        uc_chan_idx = g_scan_chan_list[uc_index];
        oam_warning_log3(0, OAM_SF_SCAN, "{HiD2D results: channel[%d], avg chan_load is %d, avg noise rssi is %d}",
            g_aus_channel_candidate_list_5G[uc_chan_idx].uc_chan_number,
            pst_chan_info[uc_chan_idx].chan_load, pst_chan_info[uc_chan_idx].chan_noise_rssi);
        if (pst_chan_info[uc_chan_idx].chan_load > highest_chan_load) {
            highest_chan_load = pst_chan_info[uc_chan_idx].chan_load;
            best_chan_of_chan_load = uc_chan_idx;
        }
        if (pst_chan_info[uc_chan_idx].chan_noise_rssi < lowest_noise_rssi) {
            lowest_noise_rssi = pst_chan_info[uc_chan_idx].chan_noise_rssi;
            best_chan_of_noise_rssi = uc_chan_idx;
        }
    }
    /* 分别按照占空比最高和底噪最小选择了信道A和信道B */
    /* 如果信道A和信道B是一个信道，则就report该信道，否则优先选择占空比最高的信道A，除非信道B的占空比略小于A，
        但底噪比A低5dB以上 */
    if (best_chan_of_chan_load == best_chan_of_noise_rssi) {
        uc_chan_idx = best_chan_of_chan_load;
    } else if ((highest_chan_load - pst_chan_info[best_chan_of_noise_rssi].chan_load < HMAC_HID2D_CHAN_LOAD_DIFF)
        && (pst_chan_info[best_chan_of_chan_load].chan_noise_rssi - lowest_noise_rssi > HMAC_HID2D_NOISE_DIFF)) {
        uc_chan_idx = best_chan_of_noise_rssi;
    } else {
        uc_chan_idx = best_chan_of_chan_load;
    }
    pst_scan_chan->en_band = WLAN_BAND_5G;
    pst_scan_chan->uc_chan_number = g_aus_channel_candidate_list_5G[uc_chan_idx].uc_chan_number;
    pst_scan_chan->en_bandwidth = g_aus_channel_candidate_list_5G[uc_chan_idx].en_bandwidth;
    pst_scan_chan->uc_chan_idx = g_aus_channel_candidate_list_5G[uc_chan_idx].uc_chan_idx;
    oam_warning_log2(0, OAM_SF_SCAN, "{HiD2D results: best channel num is %d, bw is %d}",
        pst_scan_chan->uc_chan_number, pst_scan_chan->en_bandwidth);

    return OAL_SUCC;
}


uint32_t hmac_hid2d_timeout_handle(void *p_arg)
{
    mac_device_stru *pst_mac_device;
    mac_vap_stru *pst_p2p_vap = OAL_PTR_NULL;
    hmac_hid2d_auto_channel_switch_stru *pst_hmac_hid2d_acs_info = NULL;
    mac_channel_stru st_scan_chan;
    mac_channel_stru *pst_best_chan = NULL;
    uint8_t uc_scan_chan_idx;
    uint32_t ul_ret;
    int32_t l_ret;

    pst_mac_device = (mac_device_stru *)p_arg;
    if (pst_mac_device == OAL_PTR_NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }
    ul_ret = hmac_hid2d_find_p2p_vap(pst_mac_device, (mac_vap_stru **)&pst_p2p_vap);
    if (ul_ret != OAL_SUCC) {
        return ul_ret;
    }
    pst_hmac_hid2d_acs_info = &g_st_hmac_hid2d_acs_info;

    /* HiD2D发布会场景: 判断是否全部扫描结束，如果结束则评估最优信道并上报到hmac层，不结束则触发下一次扫描 */
    if ((pst_hmac_hid2d_acs_info->update_num < HMAC_HID2D_SCAN_TIMES_PER_CHAN_FOR_APK) ||
        (pst_hmac_hid2d_acs_info->update_num == HMAC_HID2D_SCAN_TIMES_PER_CHAN_FOR_APK &&
        pst_hmac_hid2d_acs_info->uc_cur_scan_chan_idx < HMAC_HID2D_SCAN_CHAN_NUM_FOR_APK - 1)) {
        pst_hmac_hid2d_acs_info->uc_cur_scan_chan_idx = (pst_hmac_hid2d_acs_info->uc_cur_scan_chan_idx + 1)
            % HMAC_HID2D_SCAN_CHAN_NUM_FOR_APK;
        uc_scan_chan_idx = g_scan_chan_list[pst_hmac_hid2d_acs_info->uc_cur_scan_chan_idx];
        if (pst_hmac_hid2d_acs_info->uc_cur_scan_chan_idx == 0) {
            pst_hmac_hid2d_acs_info->update_num++;
        }
        /* 再下发一次扫描，并开启定时器 */
        hmac_hid2d_scan_chan_once(pst_p2p_vap, uc_scan_chan_idx, OAL_TRUE);
        frw_timer_create_timer_m(&pst_hmac_hid2d_acs_info->st_scan_chan_timer,
            hmac_hid2d_timeout_handle,
            HMAC_HID2D_SCAN_TIMER_CYCLE_MS,
            pst_mac_device,
            OAL_FALSE,
            OAM_MODULE_ID_HMAC,
            pst_mac_device->ul_core_id);
    } else {
        /* 获取最空闲的信道 */
        pst_hmac_hid2d_acs_info->uc_apk_scan = OAL_FALSE;
        hmac_hid2d_get_best_chan(&st_scan_chan);
        /* 将扫描结果存储在mac_device_stru中 */
        pst_mac_device->is_ready_to_get_scan_result = OAL_TRUE;
        pst_best_chan = &(pst_mac_device->st_best_chan_for_hid2d);
        l_ret = memcpy_s(pst_best_chan, OAL_SIZEOF(mac_channel_stru), &st_scan_chan, OAL_SIZEOF(mac_channel_stru));
        if (l_ret != EOK) {
            oam_error_log1(0, OAM_SF_SCAN, "hmac_hid2d_best_chan_report::memcpy fail! l_ret[%d]", l_ret);
            return OAL_FAIL;
        }
    }
    return OAL_SUCC;
}

uint32_t hmac_hid2d_apk_scan_handler(hmac_scan_record_stru *pst_scan_record, uint8_t uc_scan_idx,
    hmac_hid2d_auto_channel_switch_stru *pst_hmac_hid2d_acs_info)
{
    hmac_hid2d_chan_info_stru *pst_chan_info;
    wlan_scan_chan_stats_stru *pst_chan_result;
    uint8_t uc_channel_number;
    uint8_t uc_scan_chan_idx;
    int16_t chan_load; /* 信道占空比 (千分之X) */
    int16_t noise_rssi; /* 信道的底噪 */
    int16_t decay; /* 滑动平均计算的老化系数 */
    int16_t avg_data;

    /* 获得当前扫描的信道 */
    uc_scan_chan_idx = g_scan_chan_list[pst_hmac_hid2d_acs_info->uc_cur_scan_chan_idx];
    pst_chan_info = pst_hmac_hid2d_acs_info->chan_info_5G;
    /* 获取上报的信道信息 */
    pst_chan_result = &(pst_scan_record->ast_chan_results[uc_scan_idx]);
    uc_channel_number = pst_chan_result->uc_channel_number;
    /* 如果上报的信道号和带宽与下发的信道号和带宽不匹配，则直接退出 */
    if (uc_channel_number != g_aus_channel_candidate_list_5G[uc_scan_chan_idx].uc_chan_number) {
        oam_warning_log2(0, OAM_SF_SCAN, "channel not match, scan chan is [%d], report chan is [%d]!",
            g_aus_channel_candidate_list_5G[uc_scan_chan_idx].uc_chan_number, uc_channel_number);
        return OAL_FAIL;
    }

    /* 获取扫描结果，统计占空比和底噪，并累加到chan_info中 */
    decay = HMAC_HID2D_DECAY;
    if (pst_hmac_hid2d_acs_info->update_num < pst_hmac_hid2d_acs_info->max_update_num) {
        decay = (1 + pst_hmac_hid2d_acs_info->update_num) * 100 / (10 + pst_hmac_hid2d_acs_info->update_num);
    }
    chan_load = (pst_chan_result->ul_total_free_time_80M_us << HMAC_HID2D_CHAN_LOAD_SHIFT)
        / pst_chan_result->ul_total_stats_time_us;
    avg_data = pst_chan_info[uc_scan_chan_idx].chan_load;
    pst_chan_info[uc_scan_chan_idx].chan_load = (decay * avg_data + (100 - decay) * chan_load) / 100;
    if (pst_chan_result->uc_free_power_cnt == 0) {
        noise_rssi = 0;
    } else {
        noise_rssi = (int8_t)(pst_chan_result->s_free_power_stats_20M
            / (int16_t)pst_chan_result->uc_free_power_cnt);
    }
    avg_data = pst_chan_info[uc_scan_chan_idx].chan_noise_rssi;
    pst_chan_info[uc_scan_chan_idx].chan_noise_rssi = (decay * avg_data + (100 - decay) * noise_rssi) / 100;
    return OAL_SUCC;
}

uint32_t hmac_hid2d_scan_complete_handler(hmac_scan_record_stru *pst_scan_record, uint8_t uc_scan_idx)
{
    hmac_hid2d_auto_channel_switch_stru *pst_hmac_hid2d_acs_info = NULL;
    uint32_t ul_ret = OAL_SUCC;

    if (pst_scan_record == OAL_PTR_NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_hmac_hid2d_acs_info = &g_st_hmac_hid2d_acs_info;
    if (pst_hmac_hid2d_acs_info->uc_apk_scan) {
        ul_ret = hmac_hid2d_apk_scan_handler(pst_scan_record, uc_scan_idx, pst_hmac_hid2d_acs_info);
    } else if (pst_hmac_hid2d_acs_info->uc_acs_mode) {
        oam_error_log0(0, 0, "hmac_hid2d_scan_complete_handler: unsupported mode!");
        return OAL_FAIL;
    }

    return ul_ret;
}

uint32_t hmac_hid2d_set_acs_mode(mac_vap_stru *pst_mac_vap, uint8_t uc_acs_mode)
{
    /* 正常业务逻辑下，不应该走入该函数 */
    oam_error_log0(0, 0, "hmac_hid2d_set_acs_mode: not support!");
    return OAL_SUCC;
}

uint32_t hmac_hid2d_cur_chan_stat_handler(mac_vap_stru *pst_mac_vap, uint8_t uc_len, uint8_t *puc_param)
{
    /* 正常业务逻辑下，不应该走入该函数 */
    oam_error_log0(0, 0, "hmac_hid2d_cur_chan_stat_handler: not support!");
    return OAL_SUCC;
}
#endif /* end of this file */

