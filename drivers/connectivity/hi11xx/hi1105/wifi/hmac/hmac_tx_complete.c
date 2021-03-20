

#include "hmac_tx_complete.h"
#include "hmac_tx_msdu_dscr.h"
#include "mac_common.h"
#include "hal_device.h"
#include "hal_ring.h"
#include "hal_dscr.h"
#include "hal_host_ext_if.h"
#include "oal_pcie_host.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_TX_COMPLETE_C

#ifdef CONFIG_ARCH_KIRIN_PCIE

OAL_STATIC void hmac_tx_tid_info_update(hmac_user_stru *hmac_user, mac_tx_ba_info_stru *tx_ba_info)
{
    hmac_user->tx_tid_info[tx_ba_info->tid].non_ba_tx_ring.base_ring_info.read_index = tx_ba_info->read_index;
}


OAL_STATIC uint32_t hmac_tx_ba_complete_tid_process(mac_tx_ba_info_stru *tx_ba_info)
{
    hmac_user_stru *hmac_user = NULL;

    if (!tx_ba_info->ba_info_vld) {
        return OAL_SUCC;
    }

    hmac_user = mac_res_get_hmac_user(tx_ba_info->user_id);
    if (hmac_user == NULL) {
        oam_error_log1(0, OAM_SF_TX, "{hmac_tx_complete_tid_process::hmac user[%d] NULL!}", tx_ba_info->user_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

    hmac_tx_tid_info_update(hmac_user, tx_ba_info);

    hmac_tx_ring_release_msdu(hmac_user, tx_ba_info->tid);
    return OAL_SUCC;
}


uint32_t hmac_tx_ba_complete_event_handler(mac_vap_stru *mac_vap, uint8_t len, uint8_t *param)
{
    uint8_t tid;
    mac_d2h_tx_ba_complete_info_stru *tx_ba_complete_info = NULL;

    if (param == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    tx_ba_complete_info = (mac_d2h_tx_ba_complete_info_stru *)param;

    for (tid = 0; tid < WLAN_TIDNO_BUTT; tid++) {
        if (hmac_tx_ba_complete_tid_process(&tx_ba_complete_info->tx_ba_info[tid]) != OAL_SUCC) {
            oam_error_log0(0, OAM_SF_TX, "{hmac_tx_ba_complete_event_handler::ba info process failed}");
        }
    }

    return OAL_SUCC;
}


OAL_STATIC uint16_t hmac_tx_ba_info_get_count(hal_host_device_stru *hal_dev)
{
    uint32_t ret;
    uint16_t ba_info_count = 0;
    hal_host_ring_ctl_stru *ba_info_ring = &hal_dev->st_host_tx_ba_info_ring;

    ret = hal_ring_get_entry_count(ba_info_ring, &ba_info_count);
    if (ret != OAL_SUCC) {
        oam_error_log2(0, OAM_SF_TX, "{dmac_tx_ba_info_count_get:: get entry count failed[%d] dev_id[%d]}",
                       ret, hal_dev->device_id);
        return 0;
    }

    return ba_info_count;
}


OAL_STATIC void hmac_tx_get_ba_info(hal_host_device_stru *hal_dev, uint16_t ba_entry_count,
    hal_tx_ba_info_stru *tx_ba_info, uint32_t size)
{
    uint32_t ret = OAL_FAIL;
    uint32_t index;
    uint8_t ba_info_data[BA_INFO_DATA_SIZE];
    hal_host_ring_ctl_stru *ba_info_ring = &hal_dev->st_host_tx_ba_info_ring;

    /* 遍历查询每一个tx ba info块 */
    for (index = 0; index < ba_entry_count; index++) {
        memset_s(ba_info_data, BA_INFO_DATA_SIZE, 0, BA_INFO_DATA_SIZE);

        ret = hal_ring_get_entries(ba_info_ring, ba_info_data, 1);
        if (ret != OAL_SUCC) {
            oam_error_log1(0, OAM_SF_TX, "{hmac_tx_get_ba_info:: get ring entries failed[%d]}", ret);
            break;
        }
        hal_tx_ba_info_dscr_get(ba_info_data, BA_INFO_DATA_SIZE, tx_ba_info);
    }

    /* 更新tx ba info ring的read ptr,保证下次中断时从read ptr到write ptr获取最新的ba_entry_count */
    if (ret == OAL_SUCC) {
        hal_ring_set_sw2hw(ba_info_ring);
    }
}


OAL_STATIC void hmac_tx_complete_get_tid_info(
    hal_host_device_stru *hal_device, mac_tx_ba_info_stru *mac_tx_ba_info, hal_tx_ba_info_stru *tx_ba_info)
{
    uint8_t tid;
    hmac_user_stru *hmac_user = NULL;
    hmac_tid_info_stru *tx_tid_info = NULL;

    if (tx_ba_info->tid >= WLAN_TIDNO_BUTT || tx_ba_info->user_id >= HAL_MAX_TX_BA_LUT_SIZE) {
        oam_error_log3(0, OAM_SF_TX, "{hmac_tx_complete_get_tid_info::invalid tid%d, lut index %d, vld %d}",
            tx_ba_info->tid, tx_ba_info->user_id, tx_ba_info->ba_info_vld);
        tx_ba_info->ba_info_vld = OAL_FALSE;
        return;
    }

    hmac_user = g_hmac_lut_index_tbl[tx_ba_info->user_id].user;
    if (hmac_user == NULL) {
        tx_ba_info->ba_info_vld = OAL_FALSE;
        return;
    }

    tid = tx_ba_info->tid;
    tx_tid_info = &hmac_user->tx_tid_info[tid];

    /* tx BA info ring中获取当前发送完成用户信息 */
    mac_tx_ba_info[tid].ba_info_vld = tx_ba_info->ba_info_vld;
    mac_tx_ba_info[tid].user_id = hmac_user->st_user_base_info.us_assoc_id;
    mac_tx_ba_info[tid].tid = tx_ba_info->tid;
    /* tx msdu info ring中,获取当前发送完成的msdu read指针,用于释放host msdu ring中skb */
    mac_tx_ba_info[tid].read_index = oal_readl((uintptr_t)tx_tid_info->tx_ring_device_info->word_addr[3]);
}


static void hmac_tx_complete_process(hal_host_device_stru *hal_device, hal_tx_ba_info_stru *tx_ba_info,
    uint32_t size)
{
    uint32_t index;
    mac_tx_ba_info_stru tx_complete_info;

    for (index = 0; index < size; index++) {
        if (tx_ba_info[index].ba_info_vld) {
            memset_s(&tx_complete_info, sizeof(mac_tx_ba_info_stru), 0, sizeof(mac_tx_ba_info_stru));
            hmac_tx_complete_get_tid_info(hal_device, &tx_complete_info, &tx_ba_info[index]);

            if (hmac_tx_ba_complete_tid_process(&tx_complete_info) != OAL_SUCC) {
                oam_error_log0(0, OAM_SF_TX, "{hmac_tx_complete_process::ba info process failed}");
            }
        }
    }
}


oal_uint32 hmac_tx_complete_event_handler(frw_event_mem_stru *event_mem)
{
    hal_host_rx_event *tx_ba_complete_event = NULL;
    hal_host_device_stru *hal_dev = NULL;
    hal_tx_ba_info_stru tx_ba_info[WLAN_TID_MAX_NUM];
    uint16_t ba_info_cnt;

    if (event_mem == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_RX, "{hmac_tx_complete_event_handler::event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    tx_ba_complete_event = (hal_host_rx_event *)(frw_get_event_stru(event_mem)->auc_event_data);
    hal_dev = tx_ba_complete_event->hal_dev;
    frw_event_free_m(event_mem);

    if (!oal_pcie_link_state_up()) {
        oam_error_log0(0, OAM_SF_TX, "{hmac_tx_complete_event_handler::pcie link down!}");
        return 0;
    }

    ba_info_cnt = hmac_tx_ba_info_get_count(hal_dev);
    if (ba_info_cnt == 0) {
        return OAL_SUCC;
    }

    memset_s(&tx_ba_info, sizeof(tx_ba_info), 0, sizeof(tx_ba_info));
    /* 获取当前发送完成中断上报,ba info ring中所有的ba info信息 */
    hmac_tx_get_ba_info(hal_dev, ba_info_cnt, tx_ba_info, WLAN_TID_MAX_NUM);

    hmac_tx_complete_process(hal_dev, tx_ba_info, WLAN_TID_MAX_NUM);

    return OAL_SUCC;
}
#endif

