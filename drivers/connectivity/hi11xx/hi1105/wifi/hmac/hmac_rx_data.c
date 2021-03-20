

#include "oal_ext_if.h"
#include "oam_ext_if.h"
#include "oal_net.h"
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include "plat_pm_wlan.h"
#endif
#include "mac_frame.h"
#include "mac_data.h"
#include "hmac_rx_data.h"
#include "hmac_vap.h"
#include "hmac_ext_if.h"
#include "hmac_frag.h"
#include "hmac_11i.h"
#include "mac_vap.h"
#include "hal_device.h"
#include "hmac_blockack.h"
#include "hmac_tcp_opt.h"
#include "hal_host_ext_if.h"
#include "hmac_user.h"

#ifdef _PRE_WLAN_FEATURE_WAPI
#include "hmac_wapi.h"
#endif
#ifdef _PRE_WLAN_WAKEUP_SRC_PARSE
#include <linux/ip.h>
#include <net/tcp.h>
#include <net/udp.h>
#include <net/icmp.h>
#include <linux/ieee80211.h>
#include <linux/ipv6.h>
#endif

#include <hmac_auto_adjust_freq.h>  // 为hmac_auto_adjust_freq.c统计收包数准备

#ifdef _PRE_WLAN_PKT_TIME_STAT
#include <hwnet/ipv4/wifi_delayst.h>
#endif

#ifdef _PRE_WLAN_FEATURE_SNIFFER
#ifdef CONFIG_HW_SNIFFER
#include <hwnet/ipv4/sysctl_sniffer.h>
#endif
#endif
#ifdef CONFIG_HUAWEI_DUBAI
#include <chipset_common/dubai/dubai.h>
#endif

#ifdef _PRE_WLAN_RR_PERFORMENCE_DEBUG
#include "hmac_rr_performance.h"
#endif
#include "hmac_ht_self_cure.h"
#include "securec.h"
#include "securectype.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_RX_DATA_C

/* 2 全局变量定义 */
/* 3 函数实现 */

#ifdef _PRE_WLAN_DFT_DUMP_FRAME
void hmac_rx_report_eth_frame(mac_vap_stru *pst_mac_vap,
                              oal_netbuf_stru *pst_netbuf)
{
    uint16_t us_user_idx = 0;
    mac_ether_header_stru *pst_ether_hdr = OAL_PTR_NULL;
    uint32_t ul_ret;
    uint8_t auc_user_macaddr[WLAN_MAC_ADDR_LEN] = { 0 };
    oal_switch_enum_uint8 en_eth_switch = 0;

    if (oal_unlikely(pst_netbuf == OAL_PTR_NULL)) {
        return;
    }

    /* 将skb的data指针指向以太网的帧头 */
    oal_netbuf_push(pst_netbuf, ETHER_HDR_LEN);

    /* 获取目的用户资源池id */
    if (pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_AP) {
        pst_ether_hdr = (mac_ether_header_stru *)oal_netbuf_data(pst_netbuf);
        if (oal_unlikely(pst_ether_hdr == OAL_PTR_NULL)) {
            oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_RX, "{hmac_rx_report_eth_frame::pst_ether_hdr null.}");
            oal_netbuf_pull(pst_netbuf, ETHER_HDR_LEN);
            return;
        }

        ul_ret = mac_vap_find_user_by_macaddr(pst_mac_vap, pst_ether_hdr->auc_ether_shost, &us_user_idx);
        if (ul_ret == OAL_ERR_CODE_PTR_NULL) {
            oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_RX, "{hmac_rx_report_eth_frame::ul_ret null.}");
            oal_netbuf_pull(pst_netbuf, ETHER_HDR_LEN);
            return;
        }

        if (ul_ret == OAL_FAIL) {
            oal_netbuf_pull(pst_netbuf, ETHER_HDR_LEN);
            return;
        }

        oal_set_mac_addr(auc_user_macaddr, pst_ether_hdr->auc_ether_shost);
    } else if (pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_STA) {
        if (pst_mac_vap->us_user_nums == 0) {
            oal_netbuf_pull(pst_netbuf, ETHER_HDR_LEN);
            /* SUCC , return */
            return;
        }

        us_user_idx = pst_mac_vap->us_assoc_vap_id;
        oal_set_mac_addr(auc_user_macaddr, pst_mac_vap->auc_bssid);
    }

    ul_ret = oam_report_eth_frame_get_switch(us_user_idx, OAM_OTA_FRAME_DIRECTION_TYPE_RX, &en_eth_switch);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_RX,
                         "{hmac_rx_report_eth_frame::oam_report_eth_frame_get_switch failed[%d].}", ul_ret);
        oal_netbuf_pull(pst_netbuf, ETHER_HDR_LEN);
        return;
    }

    if (en_eth_switch == OAL_SWITCH_ON) {
        /* 将要送往以太网的帧上报 */
        ul_ret = oam_report_eth_frame(auc_user_macaddr,
                                      oal_netbuf_data(pst_netbuf),
                                      (uint16_t)oal_netbuf_len(pst_netbuf),
                                      OAM_OTA_FRAME_DIRECTION_TYPE_RX);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log1(pst_mac_vap->uc_vap_id, OAM_SF_RX,
                             "{hmac_rx_report_eth_frame::oam_report_eth_frame return err: 0x%x.}\r\n", ul_ret);
        }
    }

    oal_netbuf_pull(pst_netbuf, ETHER_HDR_LEN);
}
#endif


void hmac_rx_mpdu_to_netbuf_list(oal_netbuf_head_stru *netbuf_head, oal_netbuf_stru *netbuf_mpdu)
{
    oal_netbuf_stru *netbuf_cur = OAL_PTR_NULL;
    oal_netbuf_stru *netbuf_next = OAL_PTR_NULL;

    if (netbuf_head == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_RX, "{hmac_rx_mpdu_to_netbuf_list::netbuf_head NULL.}");
        return;
    }

    netbuf_cur = netbuf_mpdu;
    while (netbuf_cur != OAL_PTR_NULL) {
        netbuf_next = oal_netbuf_next(netbuf_cur);
        oal_netbuf_add_to_list_tail(netbuf_cur, netbuf_head);
        netbuf_cur = netbuf_next;
    }
    return;
}


OAL_STATIC OAL_INLINE uint32_t hmac_rx_frame_80211_to_eth(oal_netbuf_stru *pst_netbuf,
                                                          const unsigned char *puc_da,
                                                          const unsigned char *puc_sa)
{
    mac_ether_header_stru *pst_ether_hdr = OAL_PTR_NULL;
    mac_llc_snap_stru *pst_snap;
    uint8_t ul_offset_len;

    /* 注意:默认使用protocol形式的snap头，length形式snap头形式仅前两个字节格式与protocol形式相同 */
    pst_snap = (mac_llc_snap_stru *)oal_netbuf_data(pst_netbuf);
    if (oal_likely(mac_snap_is_protocol_type(MAC_GET_SNAP_TYPE(pst_snap)))) {
        ul_offset_len = HMAC_RX_DATA_ETHER_OFFSET_LENGTH;
    } else {
        ul_offset_len = MAC_SUBMSDU_HEADER_LEN;
    }
    /*
     * 将payload向前扩充一定长度来构成以太网头的14字节空间。protocl形式的以太头会覆盖原本的snap头，
     * length形式的会将snap头作为数据部分
     */
    oal_netbuf_push(pst_netbuf, ul_offset_len);
    if (oal_unlikely(oal_netbuf_len(pst_netbuf) < sizeof(mac_ether_header_stru))) {
        oam_warning_log2(0, OAM_SF_RX, "hmac_rx_frame_80211_to_eth::"
            "No room for eth hdr, offset[%d], netbuf len[%d]", ul_offset_len, oal_netbuf_len(pst_netbuf));
        return OAL_FAIL;
    }

    pst_ether_hdr = (mac_ether_header_stru *)oal_netbuf_data(pst_netbuf);
    pst_ether_hdr->us_ether_type =
        (ul_offset_len == HMAC_RX_DATA_ETHER_OFFSET_LENGTH) ? pst_snap->us_ether_type : pst_netbuf->len;
    oal_set_mac_addr(pst_ether_hdr->auc_ether_shost, puc_sa);
    oal_set_mac_addr(pst_ether_hdr->auc_ether_dhost, puc_da);
    return OAL_SUCC;
}


void hmac_rx_free_netbuf(oal_netbuf_stru *pst_netbuf, uint16_t us_nums)
{
    oal_netbuf_stru *pst_netbuf_temp = OAL_PTR_NULL;
    uint16_t us_netbuf_num;

    if (oal_unlikely(pst_netbuf == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_RX, "{hmac_rx_free_netbuf::pst_netbuf null.}\r\n");
        return;
    }

    for (us_netbuf_num = us_nums; us_netbuf_num > 0; us_netbuf_num--) {
        pst_netbuf_temp = oal_netbuf_next(pst_netbuf);

        /* 减少netbuf对应的user引用计数 */
        oal_netbuf_free(pst_netbuf);

        pst_netbuf = pst_netbuf_temp;

        if (pst_netbuf == OAL_PTR_NULL) {
            if (oal_unlikely(us_netbuf_num != 1)) {
                oam_error_log2(0, OAM_SF_RX,
                               "{hmac_rx_free_netbuf::pst_netbuf list broken, us_netbuf_num[%d]us_nums[%d].}",
                               us_netbuf_num, us_nums);
                return;
            }
            break;
        }
    }
}


void hmac_rx_free_netbuf_list(oal_netbuf_head_stru *pst_netbuf_hdr, uint16_t uc_num_buf)
{
    oal_netbuf_stru *pst_netbuf = OAL_PTR_NULL;
    uint16_t us_idx;

    if (oal_unlikely(pst_netbuf_hdr == OAL_PTR_NULL)) {
        oam_info_log0(0, OAM_SF_RX, "{hmac_rx_free_netbuf_list::pst_netbuf null.}");
        return;
    }

    oam_info_log1(0, OAM_SF_RX, "{hmac_rx_free_netbuf_list::free [%d].}", uc_num_buf);

    for (us_idx = uc_num_buf; us_idx > 0; us_idx--) {
        pst_netbuf = oal_netbuf_delist(pst_netbuf_hdr);
        if (pst_netbuf != OAL_PTR_NULL) {
            oam_info_log0(0, OAM_SF_RX, "{hmac_rx_free_netbuf_list::pst_netbuf null.}");
            oal_netbuf_free(pst_netbuf);
        }
    }
}


OAL_STATIC uint32_t hmac_rx_transmit_to_wlan(frw_event_hdr_stru *pst_event_hdr,
                                             oal_netbuf_head_stru *pst_netbuf_head)
{
    oal_netbuf_stru *pst_netbuf = OAL_PTR_NULL; /* 从netbuf链上取下来的指向netbuf的指针 */
    uint32_t ul_netbuf_num;
    uint32_t ul_ret;
    oal_netbuf_stru *pst_buf_tmp = OAL_PTR_NULL; /* 暂存netbuf指针，用于while循环 */
    mac_tx_ctl_stru *pst_tx_ctl = OAL_PTR_NULL;
    mac_vap_stru *pst_mac_vap = OAL_PTR_NULL;

    if (oal_unlikely(oal_any_null_ptr2(pst_event_hdr, pst_netbuf_head))) {
        oam_error_log2(0, OAM_SF_RX, "{hmac_rx_transmit_to_wlan::param null, %d %d.}",
                       (uintptr_t)pst_event_hdr, (uintptr_t)pst_netbuf_head);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 获取链头的net buffer */
    pst_netbuf = oal_netbuf_peek(pst_netbuf_head);

    /* 获取mac vap 结构 */
    ul_ret = hmac_tx_get_mac_vap(pst_event_hdr->uc_vap_id, &pst_mac_vap);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        ul_netbuf_num = oal_netbuf_list_len(pst_netbuf_head);
        hmac_rx_free_netbuf(pst_netbuf, (uint16_t)ul_netbuf_num);
        oam_warning_log3(pst_event_hdr->uc_vap_id, OAM_SF_RX,
                         "{hmac_rx_transmit_to_wlan::find vap [%d] failed[%d], free [%d] netbuffer.}",
                         pst_event_hdr->uc_vap_id, ul_ret, ul_netbuf_num);
        return ul_ret;
    }

    /* 循环处理每一个netbuf，按照以太网帧的方式处理 */
    while (pst_netbuf != OAL_PTR_NULL) {
        pst_buf_tmp = oal_netbuf_next(pst_netbuf);

        oal_netbuf_next(pst_netbuf) = OAL_PTR_NULL;
        oal_netbuf_prev(pst_netbuf) = OAL_PTR_NULL;

        pst_tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_netbuf);
        memset_s(pst_tx_ctl, sizeof(mac_tx_ctl_stru), 0, sizeof(mac_tx_ctl_stru));

        MAC_GET_CB_EVENT_TYPE(pst_tx_ctl) = FRW_EVENT_TYPE_WLAN_DTX;
        MAC_GET_CB_EVENT_SUBTYPE(pst_tx_ctl) = DMAC_TX_WLAN_DTX;

        /* set the queue map id when wlan to wlan */
        oal_skb_set_queue_mapping(pst_netbuf, WLAN_NORMAL_QUEUE);
        ul_ret = hmac_tx_lan_to_wlan(pst_mac_vap, pst_netbuf);
        if (ul_ret != OAL_SUCC) {
            hmac_free_netbuf_list(pst_netbuf);
        }
        pst_netbuf = pst_buf_tmp;
    }
    return OAL_SUCC;
}


void hmac_rx_free_amsdu_netbuf(oal_netbuf_stru *pst_netbuf)
{
    oal_netbuf_stru *pst_netbuf_next = OAL_PTR_NULL;
    while (pst_netbuf != OAL_PTR_NULL) {
        pst_netbuf_next = oal_get_netbuf_next(pst_netbuf);
        oal_netbuf_free(pst_netbuf);
        pst_netbuf = pst_netbuf_next;
    }
}


void hmac_rx_clear_amsdu_last_netbuf_pointer(oal_netbuf_stru *pst_netbuf, uint16_t uc_num_buf)
{
    while (pst_netbuf != OAL_PTR_NULL) {
        uc_num_buf--;
        if (uc_num_buf == 0) {
            pst_netbuf->next = OAL_PTR_NULL;
            break;
        }
        pst_netbuf = oal_get_netbuf_next(pst_netbuf);
    }
}


OAL_STATIC void hmac_rx_init_amsdu_state(oal_netbuf_stru *pst_netbuf, dmac_msdu_proc_state_stru *pst_msdu_state)
{
    mac_rx_ctl_stru *pst_rx_ctrl = OAL_PTR_NULL; /* MPDU的控制信息 */
    uint32_t       ul_need_pull_len;

    /* 首次进入该函数解析AMSDU */
    if (oal_all_zero_value2(pst_msdu_state->uc_procd_netbuf_nums, pst_msdu_state->uc_procd_msdu_in_netbuf)) {
        pst_msdu_state->pst_curr_netbuf      = pst_netbuf;

        /* AMSDU时，首个netbuf的中包含802.11头，对应的payload需要偏移 */
        pst_rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb(pst_msdu_state->pst_curr_netbuf);

        pst_msdu_state->puc_curr_netbuf_data   = (uint8_t*)MAC_GET_RX_CB_MAC_HEADER_ADDR(pst_rx_ctrl) +
                                                 pst_rx_ctrl->uc_mac_header_len;
        pst_msdu_state->uc_netbuf_nums_in_mpdu = pst_rx_ctrl->bit_buff_nums;
        pst_msdu_state->uc_msdu_nums_in_netbuf = pst_rx_ctrl->uc_msdu_in_buffer;
        pst_msdu_state->us_submsdu_offset      = 0;

        /* 使netbuf 指向amsdu 帧头 */
        ul_need_pull_len = (uint32_t)(pst_msdu_state->puc_curr_netbuf_data - oal_netbuf_payload(pst_netbuf));
        oal_netbuf_pull(pst_msdu_state->pst_curr_netbuf, ul_need_pull_len);
    }

    return;
}


OAL_STATIC uint32_t hmac_rx_amsdu_check_frame_len(oal_netbuf_stru *pst_netbuf,
    dmac_msdu_proc_state_stru *pst_msdu_state, uint16_t us_submsdu_len, uint8_t uc_submsdu_pad_len)
{
    if (us_submsdu_len > WLAN_MAX_NETBUF_SIZE ||
        (pst_msdu_state->us_submsdu_offset + MAC_SUBMSDU_HEADER_LEN + us_submsdu_len >
        oal_netbuf_len(pst_netbuf))) {
        return OAL_FAIL;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t hmac_rx_parse_amsdu(oal_netbuf_stru *pst_netbuf,
    dmac_msdu_stru *pst_msdu, dmac_msdu_proc_state_stru *pst_msdu_state,
    mac_msdu_proc_status_enum_uint8 *pen_proc_state)
{
    mac_rx_ctl_stru *pst_rx_ctrl = OAL_PTR_NULL;    /* MPDU的控制信息 */
    uint8_t *puc_buffer_data_addr = OAL_PTR_NULL; /* 指向netbuf数据域的指针 */
    uint16_t us_offset;                           /* submsdu相对于data指针的偏移 */
    uint16_t us_submsdu_len = 0;                  /* submsdu的长度 */
    uint8_t uc_submsdu_pad_len = 0;               /* submsdu的填充长度 */
    uint8_t *puc_submsdu_hdr = OAL_PTR_NULL;      /* 指向submsdu头部的指针 */
    oal_netbuf_stru *pst_netbuf_prev = OAL_PTR_NULL;
    oal_bool_enum_uint8 b_need_free_netbuf;

    if (oal_unlikely(pst_netbuf == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_RX, "{hmac_rx_parse_amsdu::pst_netbuf null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 首次进入该函数解析AMSDU */
    hmac_rx_init_amsdu_state(pst_netbuf, pst_msdu_state);

    /* 获取submsdu的头指针 */
    puc_buffer_data_addr = pst_msdu_state->puc_curr_netbuf_data;
    us_offset = pst_msdu_state->us_submsdu_offset;
    puc_submsdu_hdr = puc_buffer_data_addr + us_offset;

    /* 1个netbuf 只包含一个msdu */
    if (pst_msdu_state->uc_msdu_nums_in_netbuf == 1) {
        mac_get_submsdu_len(puc_submsdu_hdr, &us_submsdu_len);
        oal_set_mac_addr(pst_msdu->auc_sa, (puc_submsdu_hdr + MAC_SUBMSDU_SA_OFFSET));
        oal_set_mac_addr(pst_msdu->auc_da, (puc_submsdu_hdr + MAC_SUBMSDU_DA_OFFSET));

        /* 指向amsdu帧体 */
        oal_netbuf_pull(pst_msdu_state->pst_curr_netbuf, MAC_SUBMSDU_HEADER_LEN);

        if (us_submsdu_len > oal_netbuf_len(pst_msdu_state->pst_curr_netbuf)) {
            *pen_proc_state = MAC_PROC_ERROR;
            oam_warning_log2(0, OAM_SF_RX,
                             "{hmac_rx_parse_amsdu::us_submsdu_len %d is not valid netbuf len=%d.}",
                             us_submsdu_len,
                             oal_netbuf_len(pst_msdu_state->pst_curr_netbuf));
            hmac_rx_free_amsdu_netbuf(pst_msdu_state->pst_curr_netbuf);
            return OAL_FAIL;
        }

        oal_netbuf_trim(pst_msdu_state->pst_curr_netbuf, oal_netbuf_len(pst_msdu_state->pst_curr_netbuf));

        oal_netbuf_put(pst_msdu_state->pst_curr_netbuf, us_submsdu_len);

        /* 直接使用该netbuf上报给内核 免去一次netbuf申请和拷贝 */
        b_need_free_netbuf = OAL_FALSE;
        pst_msdu->pst_netbuf = pst_msdu_state->pst_curr_netbuf;
    } else {
        /* 获取submsdu的相关信息 */
        mac_get_submsdu_len(puc_submsdu_hdr, &us_submsdu_len);
        mac_get_submsdu_pad_len(MAC_SUBMSDU_HEADER_LEN + us_submsdu_len, &uc_submsdu_pad_len);
        oal_set_mac_addr(pst_msdu->auc_sa, (puc_submsdu_hdr + MAC_SUBMSDU_SA_OFFSET));
        oal_set_mac_addr(pst_msdu->auc_da, (puc_submsdu_hdr + MAC_SUBMSDU_DA_OFFSET));

        if (hmac_rx_amsdu_check_frame_len(pst_netbuf, pst_msdu_state, us_submsdu_len, uc_submsdu_pad_len) != OAL_SUCC) {
            oam_warning_log4(0, OAM_SF_RX,
                "hmac_rx_parse_amsdu::submsdu not valid. submsdu_len %d, offset %d. msdu_nums %d, procd_msdu %d.",
                us_submsdu_len,
                pst_msdu_state->us_submsdu_offset,
                pst_msdu_state->uc_msdu_nums_in_netbuf,
                pst_msdu_state->uc_procd_msdu_in_netbuf);
            oam_stat_vap_incr(0, rx_no_buff_dropped, 1);
            hmac_rx_free_amsdu_netbuf(pst_msdu_state->pst_curr_netbuf);
            return OAL_FAIL;
        }

        /* 针对当前的netbuf，申请新的subnetbuf，并设置对应的netbuf的信息，赋值给对应的msdu */
        pst_msdu->pst_netbuf = oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF,
                                                    (MAC_SUBMSDU_HEADER_LEN + us_submsdu_len + uc_submsdu_pad_len),
                                                    OAL_NETBUF_PRIORITY_MID);
        if (pst_msdu->pst_netbuf == OAL_PTR_NULL) {
            oam_error_log3(0, OAM_SF_RX,
                "{hmac_rx_parse_amsdu::alloc netbuf fail. submsdu len %d, pad_len %d, total_len %d}",
                us_submsdu_len,
                uc_submsdu_pad_len,
                (MAC_SUBMSDU_HEADER_LEN + us_submsdu_len + uc_submsdu_pad_len));
            oam_stat_vap_incr(0, rx_no_buff_dropped, 1);
            hmac_rx_free_amsdu_netbuf(pst_msdu_state->pst_curr_netbuf);
            return OAL_FAIL;
        }

        oal_mem_netbuf_trace(pst_msdu->pst_netbuf, OAL_TRUE);

        /* 针对每一个子msdu，修改netbuf的end、data、tail、len指针 */
        oal_netbuf_put(pst_msdu->pst_netbuf, us_submsdu_len + MAC_SUBMSDU_HEADER_LEN);
        oal_netbuf_pull(pst_msdu->pst_netbuf, MAC_SUBMSDU_HEADER_LEN);
        if (EOK != memcpy_s(pst_msdu->pst_netbuf->data, us_submsdu_len + uc_submsdu_pad_len,
            (puc_submsdu_hdr + MAC_SUBMSDU_HEADER_LEN), us_submsdu_len)) {
            oam_warning_log4(0, OAM_SF_RX,
                "{hmac_rx_parse_amsdu::memcpy fail. submsdu len %d, pad_len %d. msdu_nums %d, procd_msdu %d.}",
                us_submsdu_len,
                uc_submsdu_pad_len,
                pst_msdu_state->uc_msdu_nums_in_netbuf,
                pst_msdu_state->uc_procd_msdu_in_netbuf);
        }

        b_need_free_netbuf = OAL_TRUE;
    }

    /* 增加当前已处理的msdu的个数 */
    pst_msdu_state->uc_procd_msdu_in_netbuf++;

    /* 获取当前的netbuf中的下一个msdu进行处理 */
    if (pst_msdu_state->uc_procd_msdu_in_netbuf < pst_msdu_state->uc_msdu_nums_in_netbuf) {
        pst_msdu_state->us_submsdu_offset += us_submsdu_len + uc_submsdu_pad_len + MAC_SUBMSDU_HEADER_LEN;
    } else if (pst_msdu_state->uc_procd_msdu_in_netbuf == pst_msdu_state->uc_msdu_nums_in_netbuf) {
        pst_msdu_state->uc_procd_netbuf_nums++;

        pst_netbuf_prev = pst_msdu_state->pst_curr_netbuf;

        /* 获取该MPDU对应的下一个netbuf的内容 */
        if (pst_msdu_state->uc_procd_netbuf_nums < pst_msdu_state->uc_netbuf_nums_in_mpdu) {
            pst_msdu_state->pst_curr_netbuf = oal_netbuf_next(pst_msdu_state->pst_curr_netbuf);
            pst_msdu_state->puc_curr_netbuf_data = oal_netbuf_data(pst_msdu_state->pst_curr_netbuf);

            pst_rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb(pst_msdu_state->pst_curr_netbuf);

            pst_msdu_state->uc_msdu_nums_in_netbuf = pst_rx_ctrl->uc_msdu_in_buffer;
            pst_msdu_state->us_submsdu_offset = 0;
            pst_msdu_state->uc_procd_msdu_in_netbuf = 0;

            /* amsdu 第二个netbuf len是0, 先put到最大size */
            oal_netbuf_put(pst_msdu_state->pst_curr_netbuf, WLAN_MEM_NETBUF_SIZE2);
        } else if (pst_msdu_state->uc_procd_netbuf_nums == pst_msdu_state->uc_netbuf_nums_in_mpdu) {
            *pen_proc_state = MAC_PROC_LAST_MSDU;
            if (b_need_free_netbuf) {
                oal_netbuf_free(pst_netbuf_prev);
            }
            return OAL_SUCC;
        } else {
            *pen_proc_state = MAC_PROC_ERROR;
            oam_warning_log0(0, OAM_SF_RX,
                "{hmac_rx_parse_amsdu::pen_proc_state is err for uc_procd_netbuf_nums > uc_netbuf_nums_in_mpdul.}");
            hmac_rx_free_amsdu_netbuf(pst_msdu_state->pst_curr_netbuf);
            return OAL_FAIL;
        }
        if (b_need_free_netbuf) {
            oal_netbuf_free(pst_netbuf_prev);
        }
    } else {
        *pen_proc_state = MAC_PROC_ERROR;
        oam_warning_log0(0, OAM_SF_RX,
            "{hmac_rx_parse_amsdu::pen_proc_state is err for uc_procd_netbuf_nums > uc_netbuf_nums_in_mpdul.}");
        hmac_rx_free_amsdu_netbuf(pst_msdu_state->pst_curr_netbuf);
        return OAL_FAIL;
    }

    *pen_proc_state = MAC_PROC_MORE_MSDU;

    return OAL_SUCC;
}


OAL_STATIC void hmac_rx_prepare_msdu_list_to_wlan(hmac_vap_stru *pst_vap,
    oal_netbuf_head_stru *pst_netbuf_header,
    oal_netbuf_stru *pst_netbuf,
    mac_ieee80211_frame_stru *pst_frame_hdr)
{
    mac_rx_ctl_stru *pst_rx_ctrl = OAL_PTR_NULL;                      /* 指向MPDU控制块信息的指针 */
    dmac_msdu_stru st_msdu;                                           /* 保存解析出来的每一个MSDU */
    mac_msdu_proc_status_enum_uint8 en_process_state = MAC_PROC_BUTT; /* 解析AMSDU的状态 */
    dmac_msdu_proc_state_stru st_msdu_state = { 0 };                  /* 记录MPDU的处理信息 */
    uint8_t *puc_addr = OAL_PTR_NULL;
    uint32_t ul_ret;
    uint8_t auc_sa[WLAN_MAC_ADDR_LEN], auc_da[WLAN_MAC_ADDR_LEN];
    hmac_user_stru *pst_hmac_user = OAL_PTR_NULL;

    /* 解析MPDU-->MSDU */ /* 将MSDU组成netbuf链 */
    oal_mem_netbuf_trace(pst_netbuf, OAL_TRUE);

    /* 获取该MPDU的控制信息 */
    pst_rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb(pst_netbuf);

    memset_s(&st_msdu, OAL_SIZEOF(dmac_msdu_stru), 0, OAL_SIZEOF(dmac_msdu_stru));

    /* 情况一:不是AMSDU聚合，则该MPDU对应一个MSDU，同时对应一个NETBUF,将MSDU还原
       成以太网格式帧以后直接加入到netbuf链表最后
    */
    if (pst_rx_ctrl->bit_amsdu_enable == OAL_FALSE) {
        pst_hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(MAC_GET_RX_CB_TA_USER_IDX(pst_rx_ctrl));
        if (oal_unlikely(pst_hmac_user == OAL_PTR_NULL)) {
            /* 打印此net buf相关信息 */
            mac_rx_report_80211_frame((uint8_t *)&(pst_vap->st_vap_base_info), (uint8_t *)pst_rx_ctrl, pst_netbuf,
                OAM_OTA_TYPE_RX_HMAC_CB);
            return;
        }

        pst_netbuf = hmac_defrag_process(pst_hmac_user, pst_netbuf, pst_rx_ctrl->uc_mac_header_len);
        if (pst_netbuf == OAL_PTR_NULL) {
            return;
        }

        /* 重新获取该MPDU的控制信息 */
        pst_rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb(pst_netbuf);

        /* 从MAC头中获取源地址和目的地址 */
        mac_rx_get_sa(pst_frame_hdr, &puc_addr);
        oal_set_mac_addr(&auc_sa[0], puc_addr);

        mac_rx_get_da(pst_frame_hdr, &puc_addr);
        oal_set_mac_addr(&auc_da[0], puc_addr);

        /* 将netbuf的data指针指向mac frame的payload处，也就是指向了8字节的snap头 */
        oal_netbuf_pull(pst_netbuf, pst_rx_ctrl->uc_mac_header_len);

        /* 将MSDU转化为以太网格式的帧 */
        ul_ret = hmac_rx_frame_80211_to_eth(pst_netbuf, auc_da, auc_sa);
        if (ul_ret != OAL_SUCC) {
            oal_netbuf_free(pst_netbuf);
            return;
        }
#ifdef _PRE_WLAN_PKT_TIME_STAT
        memset_s(oal_netbuf_cb(pst_netbuf), OAL_NETBUF_CB_ORIGIN, 0, OAL_NETBUF_CB_ORIGIN);
#else
        memset_s(oal_netbuf_cb(pst_netbuf), oal_netbuf_cb_size(), 0, oal_netbuf_cb_size());
#endif
        /* 将MSDU加入到netbuf链的最后 */
        oal_netbuf_add_to_list_tail(pst_netbuf, pst_netbuf_header);
    } else { /* 情况二:AMSDU聚合 */
        st_msdu_state.uc_procd_netbuf_nums = 0;
        st_msdu_state.uc_procd_msdu_in_netbuf = 0;
        /* amsdu 最后一个netbuf next指针设为 NULL 出错时方便释放amsdu netbuf */
        hmac_rx_clear_amsdu_last_netbuf_pointer(pst_netbuf, pst_rx_ctrl->bit_buff_nums);
        do {
            /* 获取下一个要转发的msdu */
            ul_ret = hmac_rx_parse_amsdu(pst_netbuf, &st_msdu, &st_msdu_state, &en_process_state);
            if (ul_ret != OAL_SUCC) {
                oam_warning_log1(pst_vap->st_vap_base_info.uc_vap_id, OAM_SF_RX,
                                 "{hmac_rx_prepare_msdu_list_to_wlan::hmac_rx_parse_amsdu failed[%d].}", ul_ret);
                return;
            }

            /* 将MSDU转化为以太网格式的帧 */
            ul_ret = hmac_rx_frame_80211_to_eth(st_msdu.pst_netbuf, st_msdu.auc_da, st_msdu.auc_sa);
            if (ul_ret != OAL_SUCC) {
                oal_netbuf_free(st_msdu.pst_netbuf);
                continue;
            }
            /* 将MSDU加入到netbuf链的最后 */
            oal_netbuf_add_to_list_tail(st_msdu.pst_netbuf, pst_netbuf_header);
        } while (en_process_state != MAC_PROC_LAST_MSDU);
    }

    return;
}


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 hmac_is_tcp_udp_frame(oal_netbuf_stru *pst_netbuf)
{
    oal_ip_header_stru *pst_ip_header = OAL_PTR_NULL;
    oal_ipv6hdr_stru *pst_ipv6_header = OAL_PTR_NULL;
    mac_ether_header_stru *pst_ether_header = (mac_ether_header_stru *)(oal_netbuf_data(pst_netbuf) - ETHER_HDR_LEN);

    /*lint -e778*/
    if (oal_host2net_short(ETHER_TYPE_IP) == pst_ether_header->us_ether_type) {
        /*lint +e778*/
        pst_ip_header = (oal_ip_header_stru *)(pst_ether_header + 1);

        if ((pst_ip_header->uc_protocol == OAL_IPPROTO_TCP) ||
            ((pst_ip_header->uc_protocol == OAL_IPPROTO_UDP) &&
            (OAL_FALSE == mac_is_dhcp_port((mac_ip_header_stru *)pst_ip_header)))) { // DHCP帧直接给内核
            return OAL_TRUE;
        }
    } else if (oal_host2net_short(ETHER_TYPE_IPV6) == pst_ether_header->us_ether_type) {
        pst_ipv6_header = (oal_ipv6hdr_stru *)(pst_ether_header + 1);
        if ((pst_ipv6_header->nexthdr == OAL_IPPROTO_TCP) ||
            ((pst_ipv6_header->nexthdr == OAL_IPPROTO_UDP) &&
            (OAL_FALSE == mac_is_dhcp6(pst_ipv6_header)))) { // DHCP帧直接给内核
            return OAL_TRUE;
        }
    }

    return OAL_FALSE;
}

#ifdef _PRE_WLAN_WAKEUP_SRC_PARSE

OAL_STATIC void hmac_parse_ipv4_packet(void *pst_eth)
{
    const struct iphdr *iph = OAL_PTR_NULL;
    uint32_t iphdr_len;
    struct tcphdr *th = OAL_PTR_NULL;
    struct udphdr *uh = OAL_PTR_NULL;
    struct icmphdr *icmph = OAL_PTR_NULL;

    oal_io_print(WIFI_WAKESRC_TAG "ipv4 packet.\n");
    iph = (struct iphdr *)((mac_ether_header_stru *)pst_eth + 1);
    iphdr_len = iph->ihl * 4;

    oal_io_print(WIFI_WAKESRC_TAG "src ip:%d.x.x.%d, dst ip:%d.x.x.%d\n", IPADDR(iph->saddr), IPADDR(iph->daddr));
    if (iph->protocol == IPPROTO_UDP) {
        uh = (struct udphdr *)((uint8_t *)iph + iphdr_len);
        oal_io_print(WIFI_WAKESRC_TAG "UDP packet, src port:%d, dst port:%d.\n",
                     oal_ntoh_16(uh->source), oal_ntoh_16(uh->dest));
#ifdef CONFIG_HUAWEI_DUBAI
        dubai_log_packet_wakeup_stats("DUBAI_TAG_PACKET_WAKEUP_UDP_V4", "port", oal_ntoh_16(uh->dest));
#endif
    } else if (iph->protocol == IPPROTO_TCP) {
        th = (struct tcphdr *)((uint8_t *)iph + iphdr_len);
        oal_io_print(WIFI_WAKESRC_TAG "TCP packet, src port:%d, dst port:%d.\n",
                     oal_ntoh_16(th->source), oal_ntoh_16(th->dest));
#ifdef CONFIG_HUAWEI_DUBAI
        dubai_log_packet_wakeup_stats("DUBAI_TAG_PACKET_WAKEUP_TCP_V4", "port", oal_ntoh_16(th->dest));
#endif
    } else if (iph->protocol == IPPROTO_ICMP) {
        icmph = (struct icmphdr *)((uint8_t *)iph + iphdr_len);
        oal_io_print(WIFI_WAKESRC_TAG "ICMP packet, type(%d):%s, code:%d.\n", icmph->type,
            ((icmph->type == 0) ? "ping reply" : ((icmph->type == 8) ?
            "ping request" : "other icmp pkt")), icmph->code);
#ifdef CONFIG_HUAWEI_DUBAI
        dubai_log_packet_wakeup_stats("DUBAI_TAG_PACKET_WAKEUP", "protocol", (int32_t)iph->protocol);
#endif
    } else if (iph->protocol == IPPROTO_IGMP) {
        oal_io_print(WIFI_WAKESRC_TAG "IGMP packet.\n");
#ifdef CONFIG_HUAWEI_DUBAI
        dubai_log_packet_wakeup_stats("DUBAI_TAG_PACKET_WAKEUP", "protocol", (int32_t)iph->protocol);
#endif
    } else {
        oal_io_print(WIFI_WAKESRC_TAG "other IPv4 packet.\n");
#ifdef CONFIG_HUAWEI_DUBAI
        dubai_log_packet_wakeup_stats("DUBAI_TAG_PACKET_WAKEUP", "protocol", (int32_t)iph->protocol);
#endif
    }

    return;
}


OAL_STATIC void hmac_parse_ipv6_packet(void *pst_eth)
{
    struct ipv6hdr *ipv6h = OAL_PTR_NULL;

    oal_io_print(WIFI_WAKESRC_TAG "ipv6 packet.\n");
    ipv6h = (struct ipv6hdr *)((mac_ether_header_stru *)pst_eth + 1);
    oal_io_print(WIFI_WAKESRC_TAG "version: %d, payload length: %d, nh->nexthdr: %d. \n",
                 ipv6h->version, oal_ntoh_16(ipv6h->payload_len), ipv6h->nexthdr);
    oal_io_print(WIFI_WAKESRC_TAG "ipv6 src addr:%04x:x:x:x:x:x:x:%04x \n", IPADDR6(ipv6h->saddr));
    oal_io_print(WIFI_WAKESRC_TAG "ipv6 dst addr:%04x:x:x:x:x:x:x:%04x \n", IPADDR6(ipv6h->daddr));
    if (OAL_IPPROTO_ICMPV6 == ipv6h->nexthdr) {
        oal_nd_msg_stru *pst_rx_nd_hdr;
        pst_rx_nd_hdr = (oal_nd_msg_stru *)(ipv6h + 1);
        oal_io_print(WIFI_WAKESRC_TAG "ipv6 nd type: %d. \n", pst_rx_nd_hdr->icmph.icmp6_type);
    }
#ifdef CONFIG_HUAWEI_DUBAI
    dubai_log_packet_wakeup_stats("DUBAI_TAG_PACKET_WAKEUP", "protocol", IPPROTO_IPV6);
#endif

    return;
}


OAL_STATIC void hmac_parse_arp_packet(void *pst_eth)
{
    const struct iphdr *iph = OAL_PTR_NULL;
    int iphdr_len;
    struct arphdr *arp;

    iph = (struct iphdr *)((mac_ether_header_stru *)pst_eth + 1);
    iphdr_len = iph->ihl * 4;
    arp = (struct arphdr *)((uint8_t *)iph + iphdr_len);
    oal_io_print(WIFI_WAKESRC_TAG "ARP packet, hardware type:%d, protocol type:%d, opcode:%d.\n",
                 oal_ntoh_16(arp->ar_hrd), oal_ntoh_16(arp->ar_pro), oal_ntoh_16(arp->ar_op));

    return;
}


OAL_STATIC void hmac_parse_8021x_packet(void *pst_eth)
{
    struct ieee8021x_hdr *hdr = (struct ieee8021x_hdr *)((mac_ether_header_stru *)pst_eth + 1);

    oal_io_print(WIFI_WAKESRC_TAG "802.1x frame: version:%d, type:%d, length:%d\n",
                 hdr->version, hdr->type, oal_ntoh_16(hdr->length));

    return;
}


void hmac_parse_packet(oal_netbuf_stru *pst_netbuf_eth)
{
    uint16_t us_type;
    mac_ether_header_stru *pst_ether_hdr;

    pst_ether_hdr = (mac_ether_header_stru *)oal_netbuf_data(pst_netbuf_eth);
    if (oal_unlikely(pst_ether_hdr == OAL_PTR_NULL)) {
        oal_io_print(WIFI_WAKESRC_TAG "ether header is null.\n");
        return;
    }

    us_type = pst_ether_hdr->us_ether_type;
    oal_io_print(WIFI_WAKESRC_TAG "protocol type:0x%04x\n", oal_ntoh_16(us_type));

    if (us_type == oal_host2net_short(ETHER_TYPE_IP)) {
        hmac_parse_ipv4_packet((void *)pst_ether_hdr);
    } else if (us_type == oal_host2net_short(ETHER_TYPE_IPV6)) {
        hmac_parse_ipv6_packet((void *)pst_ether_hdr);
    } else if (us_type == oal_host2net_short(ETHER_TYPE_ARP)) {
        hmac_parse_arp_packet((void *)pst_ether_hdr);
    } else if (us_type == oal_host2net_short(ETHER_TYPE_PAE)) {
        hmac_parse_8021x_packet((void *)pst_ether_hdr);
    } else {
        oal_io_print(WIFI_WAKESRC_TAG "receive other packet.\n");
    }

    return;
}
#endif


OAL_STATIC void hmac_mips_optimize_statistic_perform(mac_ip_header_stru *ip, hmac_user_stru *hmac_user_st,
                                                     uint16_t assoc_id)
{
    if (((ip->uc_protocol == MAC_UDP_PROTOCAL) &&
        (hmac_user_st->aaul_txrx_data_stat[WLAN_WME_AC_BE][WLAN_RX_UDP_DATA] <
        (HMAC_EDCA_OPT_PKT_NUM + 10))) ||   /* 10报文个数 */
        ((ip->uc_protocol == MAC_TCP_PROTOCAL) &&
        (hmac_user_st->aaul_txrx_data_stat[WLAN_WME_AC_BE][WLAN_RX_TCP_DATA] <
        (HMAC_EDCA_OPT_PKT_NUM + 10)))) {   /* 10报文个数 */
        hmac_edca_opt_rx_pkts_stat(assoc_id, WLAN_TIDNO_BEST_EFFORT, ip);
    }
}


OAL_STATIC void hmac_rx_transmit_msdu_to_lan(hmac_vap_stru *pst_vap,
    hmac_user_stru *pst_hmac_user, dmac_msdu_stru *pst_msdu)
{
    oal_net_device_stru *pst_device = OAL_PTR_NULL;
    oal_netbuf_stru *pst_netbuf;
    mac_ether_header_stru *pst_ether_hdr = OAL_PTR_NULL;
    uint8_t *puc_mac_addr;
    mac_vap_stru *pst_mac_vap = &(pst_vap->st_vap_base_info);
#ifdef _PRE_WLAN_FEATURE_EDCA_OPT_AP
    hmac_user_stru *pst_hmac_user_st = OAL_PTR_NULL;
    mac_ip_header_stru *pst_ip = OAL_PTR_NULL;
    uint16_t us_assoc_id = 0xffff;
#endif
#if defined(_PRE_WLAN_FEATURE_VIRTUAL_MULTI_STA)
    hmac_wds_stas_stru *pst_sta;
    oal_netbuf_stru *pst_temp_netbuf = OAL_PTR_NULL;
#endif
    uint32_t ret;
    /* 获取netbuf，该netbuf的data指针已经指向payload处 */
    pst_netbuf = pst_msdu->pst_netbuf;
    puc_mac_addr = pst_msdu->auc_ta;

    oal_netbuf_prev(pst_netbuf) = OAL_PTR_NULL;
    oal_netbuf_next(pst_netbuf) = OAL_PTR_NULL;

#if defined(_PRE_WLAN_FEATURE_VIRTUAL_MULTI_STA)
    if (OAL_SUCC == hmac_wds_find_sta(pst_vap, pst_msdu->auc_sa, &pst_sta)) {
        pst_temp_netbuf = oal_netbuf_copy(pst_netbuf, GFP_ATOMIC);
        ret = hmac_rx_frame_80211_to_eth(pst_temp_netbuf, pst_msdu->auc_da, pst_msdu->auc_ta);
        ret = hmac_rx_frame_80211_to_eth(pst_netbuf, pst_msdu->auc_da, pst_msdu->auc_sa);
    } else {
        ret = hmac_rx_frame_80211_to_eth(pst_netbuf, pst_msdu->auc_da, pst_msdu->auc_sa);
    }
#else
    {
        ret = hmac_rx_frame_80211_to_eth(pst_netbuf, pst_msdu->auc_da, pst_msdu->auc_sa);
    }
#endif
    if (oal_unlikely(ret != OAL_SUCC)) {
        oal_netbuf_free(pst_netbuf);
        return;
    }

    pst_ether_hdr = (mac_ether_header_stru *)oal_netbuf_data(pst_netbuf);

#ifdef _PRE_WLAN_WAKEUP_SRC_PARSE
    if (OAL_TRUE == wlan_pm_wkup_src_debug_get()) {
        oal_io_print(WIFI_WAKESRC_TAG "rx: hmac_parse_packet!\n");
        hmac_parse_packet(pst_netbuf);
        wlan_pm_wkup_src_debug_set(OAL_FALSE);
    }
#endif
#ifdef _PRE_WLAN_FEATURE_SPECIAL_PKT_LOG
    hmac_parse_special_ipv4_packet(oal_netbuf_data(pst_netbuf), oal_netbuf_get_len(pst_netbuf), HMAC_PKT_DIRECTION_RX);
#endif
    if (OAL_SUCC != hmac_11i_ether_type_filter(pst_vap, pst_hmac_user, pst_ether_hdr->us_ether_type)) {
        /* 接收安全数据过滤 */
        oam_report_eth_frame(puc_mac_addr, (uint8_t *)pst_ether_hdr,
                             (uint16_t)oal_netbuf_len(pst_netbuf), OAM_OTA_FRAME_DIRECTION_TYPE_RX);

        oal_netbuf_free(pst_netbuf);
        oam_stat_vap_incr(pst_vap->st_vap_base_info.uc_vap_id, rx_portvalid_check_fail_dropped, 1);
        return;
    }

    /* 获取net device hmac创建的时候，需要记录netdevice指针 */
    pst_device = pst_vap->pst_net_device;

    /* 对protocol模式赋值 */
    oal_netbuf_protocol(pst_netbuf) = oal_eth_type_trans(pst_netbuf, pst_device);
    if (OAL_FALSE == ether_is_multicast(pst_msdu->auc_da)) {
        oal_atomic_inc(&(pst_vap->st_hmac_arp_probe.ul_rx_unicast_pkt_to_lan));
    }
    /* 信息统计与帧上报分离 */
    /* 增加统计信息 */
    HMAC_VAP_DFT_STATS_PKT_INCR(pst_vap->st_query_stats.ul_rx_pkt_to_lan, 1);
    HMAC_VAP_DFT_STATS_PKT_INCR(pst_vap->st_query_stats.ul_rx_bytes_to_lan, oal_netbuf_len(pst_netbuf));
    oam_stat_vap_incr(pst_vap->st_vap_base_info.uc_vap_id, rx_pkt_to_lan, 1); /* 增加发往LAN的帧的数目 */
    /* 增加发送LAN的字节数 */
    oam_stat_vap_incr(pst_vap->st_vap_base_info.uc_vap_id, rx_bytes_to_lan, oal_netbuf_len(pst_netbuf));

#ifdef _PRE_WLAN_DFT_DUMP_FRAME
    hmac_rx_report_eth_frame(&pst_vap->st_vap_base_info, pst_netbuf);
#endif

#ifdef _PRE_WLAN_FEATURE_EDCA_OPT_AP
    if ((pst_mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_AP) && (pst_vap->uc_edca_opt_flag_ap == OAL_TRUE)) {
        /*lint -e778*/
        if (oal_host2net_short(ETHER_TYPE_IP) == pst_ether_hdr->us_ether_type) {
#if defined(_PRE_WLAN_FEATURE_VIRTUAL_MULTI_STA)
            if ((OAL_SUCC != mac_vap_find_user_by_macaddr(pst_mac_vap,
                pst_ether_hdr->auc_ether_shost, &us_assoc_id)) &&
                (OAL_SUCC != hmac_find_valid_user_by_wds_sta(pst_vap, pst_ether_hdr->auc_ether_shost, &us_assoc_id)))
#else
            if (OAL_SUCC != mac_vap_find_user_by_macaddr(pst_mac_vap, pst_ether_hdr->auc_ether_shost, &us_assoc_id))
#endif
            {
                oam_warning_log4(pst_vap->st_vap_base_info.uc_vap_id, OAM_SF_M2U,
                    "{hmac_rx_transmit_msdu_to_lan::find_user_by_macaddr[%02x:XX:XX:%02x:%02x:%02x]failed}",
                    (uint32_t)(pst_ether_hdr->auc_ether_shost[0]), (uint32_t)(pst_ether_hdr->auc_ether_shost[3]),
                    (uint32_t)(pst_ether_hdr->auc_ether_shost[4]), (uint32_t)(pst_ether_hdr->auc_ether_shost[5]));
                oal_netbuf_free(pst_netbuf);
                return;
            }
            pst_hmac_user_st = (hmac_user_stru *)mac_res_get_hmac_user(us_assoc_id);
            if (pst_hmac_user_st == OAL_PTR_NULL) {
                oam_error_log1(pst_vap->st_vap_base_info.uc_vap_id, OAM_SF_RX,
                    "{hmac_rx_transmit_msdu_to_lan::mac_res_get_hmac_user fail. assoc_id: %u}", us_assoc_id);
                oal_netbuf_free(pst_netbuf);
                return;
            }

            pst_ip = (mac_ip_header_stru *)(pst_ether_hdr + 1);
            /* mips优化:解决开启业务统计性能差10M问题 */
            hmac_mips_optimize_statistic_perform(pst_ip, pst_hmac_user_st, us_assoc_id);
        } /*lint +e778*/
    }
#endif

    oal_mem_netbuf_trace(pst_netbuf, OAL_TRUE);
#ifdef _PRE_WLAN_PKT_TIME_STAT
    memset_s(oal_netbuf_cb(pst_netbuf), OAL_NETBUF_CB_ORIGIN, 0, OAL_NETBUF_CB_ORIGIN);
#else
    memset_s(oal_netbuf_cb(pst_netbuf), oal_netbuf_cb_size(), 0, oal_netbuf_cb_size());
#endif

#ifdef _PRE_WLAN_RR_PERFORMENCE_DEBUG
    /* RR性能检测接收流程host转发至lan口位置打点 */
    hmac_rr_rx_h2w_timestamp();
#endif

    /* 将skb转发给桥 */
    if (OAL_TRUE == hmac_get_rxthread_enable()) {
        hmac_rxdata_netbuf_enqueue(pst_netbuf);
    } else {
        oal_notice_netif_rx(pst_netbuf);
        oal_netif_rx_ni(pst_netbuf);
    }

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 11, 0))
    /* 4.11以上内核版本net_device结构体中没有last_rx字段 */
#else
    /* 置位net_dev->jiffies变量 */
    oal_netdevice_last_rx(pst_device) = OAL_TIME_JIFFY;
#endif
}

#ifdef _PRE_WLAN_FEATURE_PSM_ABN_PKTS_STAT
OAL_STATIC void hmac_rx_group_rekey_stat(mac_vap_stru *pst_mac_vap, oal_netbuf_stru *pst_netbuf)
{
    hmac_device_stru *pst_hmac_device;

    pst_hmac_device = hmac_res_get_mac_dev(pst_mac_vap->uc_device_id);
    if (pst_hmac_device == OAL_PTR_NULL) {
        oam_error_log1(0, OAM_SF_ANY, "{hmac_rx_group_rekey_stat::mac_res_get_dev fail.device_id = %d}",
                       pst_mac_vap->uc_device_id);
        return;
    }

    if ((pst_hmac_device->pst_device_base_info->uc_in_suspend) && (mac_is_primary_legacy_sta(pst_mac_vap))) {
        pst_hmac_device->ul_group_rekey_cnt++;
    }
}
#endif

static void hmac_rx_set_ht_self_cure_event(mac_vap_stru *mac_vap,
    mac_ieee80211_qos_htc_frame_addr4_stru *mac_header, uint8_t event_id)
{
    if (mac_header == NULL) {
        return;
    }

    if (!ether_is_broadcast(mac_header->auc_address1) && !ether_is_broadcast(mac_header->auc_address2)) {
        hmac_ht_self_cure_event_set(mac_vap, mac_header->auc_address2, event_id);
    }
}



void hmac_rx_vip_info(mac_vap_stru *vap, uint8_t dataType,
                      oal_netbuf_stru *pst_buf, mac_rx_ctl_stru *pst_rx_ctrl)
{
    mac_ieee80211_qos_htc_frame_addr4_stru *pst_mac_header = OAL_PTR_NULL;
    mac_llc_snap_stru *pst_llc = OAL_PTR_NULL;
    oal_ip_header_stru *ipHdr = OAL_PTR_NULL;
    oal_eth_arphdr_stru *arpHead = OAL_PTR_NULL;
    int32_t l_ret = EOK;
    uint8_t sip[ETH_SENDER_IP_ADDR_LEN]; /* sender IP address */
    uint8_t dip[ETH_SENDER_IP_ADDR_LEN]; /* sender IP address */
    mac_eapol_type_enum_uint8 en_eapol_type;
    uint8_t uc_dhcp_type;

    /* 收到skb为80211头 */
    /* 获取LLC SNAP */
    pst_llc = (mac_llc_snap_stru *)(oal_netbuf_data(pst_buf) + pst_rx_ctrl->uc_mac_header_len);
    pst_mac_header = (mac_ieee80211_qos_htc_frame_addr4_stru *)MAC_GET_RX_CB_MAC_HEADER_ADDR(pst_rx_ctrl);
    if (pst_mac_header == OAL_PTR_NULL) {
        return;
    }
    if (dataType == MAC_DATA_EAPOL) {
        en_eapol_type = mac_get_eapol_key_type((uint8_t *)(pst_llc + 1));
        oam_warning_log3(vap->uc_vap_id, OAM_SF_ANY, "{hmac_rx_vip_info::EAPOL type=%u, len==%u, \
            user[%d]}[1:1/4 2:2/4 3:3/4 4:4/4]", en_eapol_type, oal_netbuf_len(pst_buf),
            MAC_GET_RX_CB_TA_USER_IDX(pst_rx_ctrl));
#ifdef _PRE_WLAN_FEATURE_PSM_ABN_PKTS_STAT
        hmac_rx_group_rekey_stat(vap, pst_buf);
#endif
        hmac_rx_set_ht_self_cure_event(vap, pst_mac_header, HAMC_HT_SELF_CURE_EVENT_RX_EAPOL);
    } else if (dataType == MAC_DATA_DHCP) {
        ipHdr = (oal_ip_header_stru *)(pst_llc + 1);

        l_ret += memcpy_s((uint8_t *)sip, ETH_SENDER_IP_ADDR_LEN, (uint8_t *)&ipHdr->ul_saddr, OAL_SIZEOF(uint32_t));
        l_ret += memcpy_s((uint8_t *)dip, ETH_SENDER_IP_ADDR_LEN, (uint8_t *)&ipHdr->ul_daddr, OAL_SIZEOF(uint32_t));
        if (l_ret != EOK) {
            oam_error_log0(0, OAM_SF_ANY, "hmac_rx_vip_info::memcpy fail!");
            return;
        }

        uc_dhcp_type = mac_get_dhcp_frame_type(ipHdr);
        hmac_rx_set_ht_self_cure_event(vap, pst_mac_header, HMAC_HT_SELF_CURE_EVENT_RX_DHCP_FRAME);
        oam_warning_log1(vap->uc_vap_id, OAM_SF_ANY, "{hmac_rx_vip_info::DHCP type=%d.[1:discovery 2:offer \
            3:request 4:decline 5:ack 6:nack 7:release 8:inform.]", uc_dhcp_type);
        oam_warning_log4(vap->uc_vap_id, OAM_SF_ANY, "{hmac_rx_vip_info:: DHCP sip: %d.%d, dip: %d.%d.",
                         sip[2], sip[3], dip[2], dip[3]);
    } else {
        arpHead = (oal_eth_arphdr_stru *)(pst_llc + 1);
        oam_warning_log1(vap->uc_vap_id, OAM_SF_ANY, "{hmac_rx_vip_info::ARP type=%d.[2:arp resp 3:arp req]", dataType);
        oam_warning_log4(vap->uc_vap_id, OAM_SF_ANY, "{hmac_rx_vip_info:: ARP sip: %d.%d, dip: %d.%d",
            arpHead->auc_ar_sip[2], arpHead->auc_ar_sip[3], arpHead->auc_ar_tip[2], arpHead->auc_ar_tip[3]);
    }

    oam_warning_log4(vap->uc_vap_id, OAM_SF_ANY, "{hmac_rx_vip_info::send to wlan smac: %x:%x, dmac: %x:%x]",
        pst_mac_header->auc_address2[4], pst_mac_header->auc_address2[5],
        pst_mac_header->auc_address1[4], pst_mac_header->auc_address1[5]);
}

static void hmac_rx_lan_frame_ap_proc(hmac_vap_stru *hmac_vap, hmac_user_stru *hmac_user,
    dmac_msdu_stru *msdu, oal_netbuf_head_stru *netbuf_hdr)
{
    uint16_t user_dix = g_wlan_spec_cfg->invalid_user_id;
    if (mac_vap_find_user_by_macaddr(&hmac_vap->st_vap_base_info, msdu->auc_da, &user_dix) == OAL_SUCC) {
        /* 将MSDU转化为以太网格式的帧 */
        if (hmac_rx_frame_80211_to_eth(msdu->pst_netbuf, msdu->auc_da, msdu->auc_sa) != OAL_SUCC) {
            oal_netbuf_free(msdu->pst_netbuf);
            return;
        }
        /* 将MSDU加入到netbuf链的最后 */
        oal_netbuf_add_to_list_tail(msdu->pst_netbuf, netbuf_hdr);
    } else {
        /* 将MSDU转发到LAN */
        hmac_rx_transmit_msdu_to_lan(hmac_vap, hmac_user, msdu);
    }
}


void hmac_rx_lan_frame_classify(hmac_vap_stru *pst_vap,
                                oal_netbuf_stru *pst_netbuf,
                                mac_ieee80211_frame_stru *pst_frame_hdr)
{
    mac_rx_ctl_stru *pst_rx_ctrl = OAL_PTR_NULL;                      /* 指向MPDU控制块信息的指针 */
    dmac_msdu_stru st_msdu;                                           /* 保存解析出来的每一个MSDU */
    mac_msdu_proc_status_enum_uint8 en_process_state = MAC_PROC_BUTT; /* 解析AMSDU的状态 */
    dmac_msdu_proc_state_stru st_msdu_state = { 0 };                  /* 记录MPDU的处理信息 */
    uint8_t *puc_addr = OAL_PTR_NULL;
    uint32_t ul_ret;
    hmac_user_stru *pst_hmac_user = OAL_PTR_NULL;
    uint8_t uc_datatype;
#ifdef _PRE_WLAN_FEATURE_WAPI
    hmac_wapi_stru *pst_wapi;
    oal_bool_enum_uint8 en_is_mcast;
#endif
#if defined _PRE_WLAN_FEATURE_VIRTUAL_MULTI_STA
    uint8_t uc_is_first = OAL_TRUE;
#endif
    oal_netbuf_head_stru st_w2w_netbuf_hdr;
    frw_event_hdr_stru st_event_hdr;

    if (oal_unlikely(oal_any_null_ptr3(pst_vap, pst_netbuf, pst_frame_hdr))) {
        oam_error_log0(0, OAM_SF_RX, "{hmac_rx_lan_frame_classify::params null.}");
        return;
    }

    memset_s(&st_msdu, OAL_SIZEOF(dmac_msdu_stru), 0, OAL_SIZEOF(dmac_msdu_stru));
    mac_get_transmit_addr(pst_frame_hdr, &puc_addr);
    oal_set_mac_addr(st_msdu.auc_ta, puc_addr);

    /* 获取该MPDU的控制信息 */
    pst_rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb(pst_netbuf);

    pst_hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(MAC_GET_RX_CB_TA_USER_IDX(pst_rx_ctrl));
    if (oal_unlikely(pst_hmac_user == OAL_PTR_NULL)) {
        /* 打印此net buf相关信息 */
        oam_error_log4(pst_vap->st_vap_base_info.uc_vap_id, OAM_SF_RX,
            "{hmac_rx_lan_frame_classify::cb info,vap id=%d mac_hdr_len=%d,frame_len=%d mac_hdr_start_addr=0x%08x}",
            pst_rx_ctrl->bit_vap_id, pst_rx_ctrl->uc_mac_header_len,
            pst_rx_ctrl->us_frame_len, (uintptr_t)MAC_GET_RX_CB_MAC_HEADER_ADDR(pst_rx_ctrl));
        oam_error_log3(pst_vap->st_vap_base_info.uc_vap_id, OAM_SF_RX,
                       "{hmac_rx_lan_frame_classify::user_idx=%d,net_buf ptr addr=0x%08x, cb ptr addr=0x%08x.}",
                       MAC_GET_RX_CB_TA_USER_IDX(pst_rx_ctrl),
                       (uintptr_t)pst_netbuf, (uintptr_t)pst_rx_ctrl);
#ifdef _PRE_WLAN_DFT_DUMP_FRAME
        mac_rx_report_80211_frame((uint8_t *)&(pst_vap->st_vap_base_info),
                                  (uint8_t *)pst_rx_ctrl, pst_netbuf, OAM_OTA_TYPE_RX_HMAC_CB);
#endif

        return;
    }

    oal_netbuf_list_head_init(&st_w2w_netbuf_hdr);
    st_event_hdr.uc_chip_id = pst_vap->st_vap_base_info.uc_chip_id;
    st_event_hdr.uc_device_id = pst_vap->st_vap_base_info.uc_device_id;
    st_event_hdr.uc_vap_id = pst_vap->st_vap_base_info.uc_vap_id;

    /* offload 下amsdu帧可能分成多个seq相同的netbuf上报 只有在last amsdu buffer才能置位
        否则容易导致重排序缓冲区在弱信号下强制移窗后收到ba start之前seq的amsdu帧丢帧
     */
    if ((pst_rx_ctrl->bit_amsdu_enable == OAL_FALSE) || (pst_rx_ctrl->bit_is_last_buffer == OAL_TRUE)) {
        hmac_ba_update_rx_bitmap(pst_hmac_user, pst_frame_hdr);
    }
#ifdef _PRE_WLAN_FEATURE_SNIFFER
#ifdef CONFIG_HW_SNIFFER
    proc_sniffer_write_file(NULL, 0, (uint8_t *)oal_netbuf_payload(pst_netbuf), pst_rx_ctrl->us_frame_len, 0);
#endif
#endif
    if (pst_rx_ctrl->bit_amsdu_enable == OAL_FALSE) {
        /* 情况一:不是AMSDU聚合，则该MPDU对应一个MSDU，同时对应一个NETBUF */
#ifdef _PRE_WLAN_FEATURE_WAPI
        en_is_mcast = ether_is_multicast(pst_frame_hdr->auc_address1);
        /*lint -e730*/
        pst_wapi = hmac_user_get_wapi_ptr(&pst_vap->st_vap_base_info, !en_is_mcast,
                                          pst_hmac_user->st_user_base_info.us_assoc_id);
        /*lint +e730*/
        if (pst_wapi == OAL_PTR_NULL) {
            oam_warning_log0(0, OAM_SF_WPA, "{hmac_rx_lan_frame_classify:: get pst_wapi Err!.}");
            HMAC_USER_STATS_PKT_INCR(pst_hmac_user->ul_rx_pkt_drop, 1);
            return;
        }

        if ((OAL_TRUE == WAPI_PORT_FLAG(pst_wapi)) && (pst_wapi->wapi_netbuff_rxhandle != OAL_PTR_NULL)) {
            pst_netbuf = pst_wapi->wapi_netbuff_rxhandle(pst_wapi, pst_netbuf);
            if (pst_netbuf == OAL_PTR_NULL) {
                oam_warning_log0(pst_vap->st_vap_base_info.uc_vap_id, OAM_SF_RX,
                                 "{hmac_rx_lan_frame_classify:: wapi decrypt FAIL!}");
                HMAC_USER_STATS_PKT_INCR(pst_hmac_user->ul_rx_pkt_drop, 1);
                return;
            }

            /* 重新获取该MPDU的控制信息 */
            pst_rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb(pst_netbuf);
        }
#endif /* #ifdef _PRE_WLAN_FEATURE_WAPI */

        pst_netbuf = hmac_defrag_process(pst_hmac_user, pst_netbuf, pst_rx_ctrl->uc_mac_header_len);
        if (pst_netbuf == OAL_PTR_NULL) {
            return;
        }

        /* 重新获取该MPDU的控制信息 */
        pst_rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb(pst_netbuf);
        pst_frame_hdr = (mac_ieee80211_frame_stru *)MAC_GET_RX_CB_MAC_HEADER_ADDR(pst_rx_ctrl);

        /* 打印出关键帧信息 */
        uc_datatype = mac_get_data_type_from_80211(pst_netbuf, pst_rx_ctrl->uc_mac_header_len);
        if (uc_datatype <= MAC_DATA_ARP_RSP) {
            hmac_rx_vip_info(&pst_vap->st_vap_base_info, uc_datatype, pst_netbuf, pst_rx_ctrl);
        }

        /* 对当前的msdu进行赋值 */
        st_msdu.pst_netbuf = pst_netbuf;

        /* 获取源地址和目的地址 */
        mac_rx_get_sa(pst_frame_hdr, &puc_addr);
        oal_set_mac_addr(st_msdu.auc_sa, puc_addr);

        mac_rx_get_da(pst_frame_hdr, &puc_addr);
        oal_set_mac_addr(st_msdu.auc_da, puc_addr);

        /* 将netbuf的data指针指向mac frame的payload处 */
        oal_netbuf_pull(pst_netbuf, pst_rx_ctrl->uc_mac_header_len);

        if (pst_vap->st_vap_base_info.en_vap_mode == WLAN_VAP_MODE_BSS_AP) {
            hmac_rx_lan_frame_ap_proc(pst_vap, pst_hmac_user, &st_msdu, &st_w2w_netbuf_hdr);
        } else {
            /* 将MSDU转发到LAN */
            hmac_rx_transmit_msdu_to_lan(pst_vap, pst_hmac_user, &st_msdu);
        }
    } else {
        /* 情况二:AMSDU聚合 */
        st_msdu_state.uc_procd_netbuf_nums = 0;
        st_msdu_state.uc_procd_msdu_in_netbuf = 0;

        /* amsdu 最后一个netbuf next指针设为 NULL 出错时方便释放amsdu netbuf */
        hmac_rx_clear_amsdu_last_netbuf_pointer(pst_netbuf, pst_rx_ctrl->bit_buff_nums);

        do {
            /* 获取下一个要转发的msdu */
            ul_ret = hmac_rx_parse_amsdu(pst_netbuf, &st_msdu, &st_msdu_state, &en_process_state);
            if (ul_ret != OAL_SUCC) {
                oam_warning_log1(pst_vap->st_vap_base_info.uc_vap_id, OAM_SF_RX,
                                 "{hmac_rx_lan_frame_classify::hmac_rx_parse_amsdu failed[%d].}", ul_ret);
                return;
            }
#ifdef _PRE_WLAN_FEATURE_VIRTUAL_MULTI_STA
            /* Virtual Multi-STA有自己的能力位方式，不通过4地址来识别 */
            if ((uc_is_first == OAL_TRUE) &&
                (pst_vap->st_wds_table.en_wds_vap_mode == WDS_MODE_ROOTAP) &&
                hmac_vmsta_get_user_a4_support(pst_vap, pst_frame_hdr->auc_address2)) {
                hmac_wds_update_table(pst_vap, pst_frame_hdr->auc_address2, (&st_msdu)->auc_sa, WDS_TABLE_ADD_ENTRY);
                uc_is_first = OAL_FALSE;
            }
#endif

            if (pst_vap->st_vap_base_info.en_vap_mode == WLAN_VAP_MODE_BSS_AP) {
                hmac_rx_lan_frame_ap_proc(pst_vap, pst_hmac_user, &st_msdu, &st_w2w_netbuf_hdr);
            } else {
                /* 将每一个MSDU转发到LAN */
                hmac_rx_transmit_msdu_to_lan(pst_vap, pst_hmac_user, &st_msdu);
            }
        } while (en_process_state != MAC_PROC_LAST_MSDU);
    }

    if (pst_vap->st_vap_base_info.en_vap_mode == WLAN_VAP_MODE_BSS_AP) {
        /*  将MSDU链表交给发送流程处理 */
        if (OAL_FALSE == oal_netbuf_list_empty(&st_w2w_netbuf_hdr) &&
            OAL_PTR_NULL != oal_netbuf_tail(&st_w2w_netbuf_hdr) &&
            OAL_PTR_NULL != oal_netbuf_peek(&st_w2w_netbuf_hdr)) {
            oal_netbuf_next((oal_netbuf_tail(&st_w2w_netbuf_hdr))) = OAL_PTR_NULL;
            oal_netbuf_prev((oal_netbuf_peek(&st_w2w_netbuf_hdr))) = OAL_PTR_NULL;

            hmac_rx_transmit_to_wlan(&st_event_hdr, &st_w2w_netbuf_hdr);
        }
    }
}


uint32_t hmac_rx_copy_netbuff(oal_netbuf_stru **ppst_dest_netbuf,
                              oal_netbuf_stru *pst_src_netbuf, uint8_t uc_vap_id,
                              mac_ieee80211_frame_stru **ppul_mac_hdr_start_addr)
{
    mac_rx_ctl_stru *pst_rx_ctrl = OAL_PTR_NULL;
    int32_t l_ret;

    if (pst_src_netbuf == OAL_PTR_NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    *ppst_dest_netbuf = oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF, WLAN_MEM_NETBUF_SIZE2, OAL_NETBUF_PRIORITY_MID);
    if (oal_unlikely(*ppst_dest_netbuf == OAL_PTR_NULL)) {
        oam_warning_log0(uc_vap_id, OAM_SF_RX, "{hmac_rx_copy_netbuff::pst_netbuf_copy null.}");
        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }

    /* 信息复制 */
    l_ret = memcpy_s(oal_netbuf_cb(*ppst_dest_netbuf), OAL_SIZEOF(mac_rx_ctl_stru),
                     oal_netbuf_cb(pst_src_netbuf), OAL_SIZEOF(mac_rx_ctl_stru));  // modify src bug
    l_ret += memcpy_s(oal_netbuf_data(*ppst_dest_netbuf), oal_netbuf_len(pst_src_netbuf),
                      oal_netbuf_data(pst_src_netbuf), oal_netbuf_len(pst_src_netbuf));
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_RX, "hmac_rx_copy_netbuff::memcpy fail!");
        oal_netbuf_free(*ppst_dest_netbuf);
        return OAL_FAIL;
    }

    /* 设置netbuf长度、TAIL指针 */
    oal_netbuf_put(*ppst_dest_netbuf, oal_netbuf_get_len(pst_src_netbuf));

    /* 调整MAC帧头的指针copy后，对应的mac header的头已经发生变化) */
    pst_rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb(*ppst_dest_netbuf);
    MAC_GET_RX_CB_MAC_HEADER_ADDR(pst_rx_ctrl) = (uint32_t *)oal_netbuf_data(*ppst_dest_netbuf);
    *ppul_mac_hdr_start_addr = (mac_ieee80211_frame_stru *)oal_netbuf_data(*ppst_dest_netbuf);

    return OAL_SUCC;
}


void hmac_rx_process_data_filter(oal_netbuf_head_stru *pst_netbuf_header,
                                 oal_netbuf_stru *pst_temp_netbuf,
                                 uint16_t us_netbuf_num)
{
    oal_netbuf_stru *pst_netbuf = OAL_PTR_NULL;
    mac_rx_ctl_stru *pst_rx_ctrl = OAL_PTR_NULL;
    hmac_user_stru *pst_hmac_user = OAL_PTR_NULL;
    uint32_t ul_ret;
    uint8_t uc_buf_nums;
    uint8_t uc_vap_id;
    oal_bool_enum_uint8 en_is_ba_buf;
    uint8_t uc_netbuf_num;

    while (us_netbuf_num != 0) {
        en_is_ba_buf = OAL_FALSE;
        pst_netbuf = pst_temp_netbuf;
        if (pst_netbuf == OAL_PTR_NULL) {
            oam_warning_log1(0, OAM_SF_RX, "{hmac_rx_process_data_filter::us_netbuf_num = %d}", us_netbuf_num);
            break;
        }

        pst_rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb(pst_netbuf);
        uc_buf_nums = pst_rx_ctrl->bit_buff_nums;

        /* 获取下一个要处理的MPDU */
        oal_netbuf_get_appointed_netbuf(pst_netbuf, uc_buf_nums, &pst_temp_netbuf);
        us_netbuf_num = oal_sub(us_netbuf_num, uc_buf_nums);

        uc_vap_id = pst_rx_ctrl->uc_mac_vap_id;
        /* 双芯片下，0和1都是配置vap id，因此这里需要采用业务vap 其实id和整板最大vap mac num值来做判断 */
        if (oal_board_get_service_vap_start_id() > uc_vap_id || uc_vap_id >= WLAN_VAP_SUPPORT_MAX_NUM_LIMIT) {
            oam_error_log1(0, OAM_SF_RX, "{hmac_rx_process_data_filter::Invalid vap_id.vap_id[%u]}", uc_vap_id);
            hmac_rx_free_netbuf_list(pst_netbuf_header, uc_buf_nums);
            continue;
        }

        // make sure ta user idx is exist
        pst_hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(MAC_GET_RX_CB_TA_USER_IDX(pst_rx_ctrl));
        if (pst_hmac_user == OAL_PTR_NULL) {
            oam_warning_log1(0, OAM_SF_ROAM,
                             "{hmac_rx_process_data_filter::pst_hmac_user[%d] null.}",
                             MAC_GET_RX_CB_TA_USER_IDX(pst_rx_ctrl));
            hmac_rx_free_netbuf_list(pst_netbuf_header, uc_buf_nums);
            continue;
        }

        ul_ret = hmac_ba_filter_serv(pst_hmac_user, pst_rx_ctrl, pst_netbuf_header, &en_is_ba_buf);
        if (ul_ret != OAL_SUCC) {
            hmac_rx_free_netbuf_list(pst_netbuf_header, uc_buf_nums);
            continue;
        }

        if (en_is_ba_buf == OAL_TRUE) {
            continue;
        }

        /* 如果不buff进reorder队列，则重新挂到链表尾，保序 */
        for (uc_netbuf_num = 0; uc_netbuf_num < uc_buf_nums; uc_netbuf_num++) {
            pst_netbuf = oal_netbuf_delist_nolock(pst_netbuf_header);
            if (oal_likely(pst_netbuf != OAL_PTR_NULL)) {
                oal_netbuf_list_tail_nolock(pst_netbuf_header, pst_netbuf);
            } else {
                oam_warning_log0(pst_rx_ctrl->bit_vap_id, OAM_SF_RX,
                                 "{hmac_rx_process_data_filter::no buff error.}");
            }
        }
    }
}

#ifdef _PRE_WLAN_TCP_OPT
OAL_STATIC oal_bool_enum_uint8 hmac_transfer_rx_tcp_ack_handler(hmac_device_stru *pst_hmac_device,
                                                                hmac_vap_stru *hmac_vap,
                                                                oal_netbuf_stru *netbuf)
{
#ifndef WIN32
    mac_rx_ctl_stru *pst_rx_ctrl; /* 指向MPDU控制块信息的指针 */
    oal_netbuf_stru *pst_mac_llc_snap_netbuf;

    pst_rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);
    pst_mac_llc_snap_netbuf = (oal_netbuf_stru *)(netbuf->data + pst_rx_ctrl->uc_mac_header_len);
#ifdef _PRE_WLAN_TCP_OPT_DEBUG
    oam_warning_log1(0, OAM_SF_TX,
                     "{hmac_transfer_rx_handler::uc_mac_header_len = %d}\r\n", pst_rx_ctrl->uc_mac_header_len);
#endif
    if (OAL_TRUE == hmac_judge_rx_netbuf_classify(pst_mac_llc_snap_netbuf)) {
#ifdef _PRE_WLAN_TCP_OPT_DEBUG
        oam_warning_log0(0, OAM_SF_TX,
                         "{hmac_transfer_rx_handler::netbuf is tcp ack.}\r\n");
#endif
        oal_spin_lock_bh(&hmac_vap->st_hamc_tcp_ack[HCC_RX].data_queue_lock[HMAC_TCP_ACK_QUEUE]);
        oal_netbuf_list_tail(&hmac_vap->st_hamc_tcp_ack[HCC_RX].data_queue[HMAC_TCP_ACK_QUEUE],
                             netbuf);

        oal_spin_unlock_bh(&hmac_vap->st_hamc_tcp_ack[HCC_RX].data_queue_lock[HMAC_TCP_ACK_QUEUE]);
        hmac_sched_transfer();
        return OAL_TRUE;
    }
#endif
    return OAL_FALSE;
}


void hmac_transfer_rx_handle(hmac_device_stru *pst_hmac_device,
                             hmac_vap_stru *pst_hmac_vap,
                             oal_netbuf_head_stru *pst_netbuf_header)
{
    oal_netbuf_head_stru st_temp_header;
    oal_netbuf_stru *pst_netbuf = OAL_PTR_NULL;

    oal_netbuf_head_init(&st_temp_header);
    while (!!(pst_netbuf = oal_netbuf_delist_nolock(pst_netbuf_header))) {
        if (OAL_FALSE == hmac_transfer_rx_tcp_ack_handler(pst_hmac_device, pst_hmac_vap, pst_netbuf)) {
            oal_netbuf_list_tail_nolock(&st_temp_header, pst_netbuf);
        }
    }
    /*lint -e522*/
    oal_warn_on(!oal_netbuf_list_empty(pst_netbuf_header));
    /*lint +e522*/
    oal_netbuf_splice_init(&st_temp_header, pst_netbuf_header);
}
#endif


void hmac_rx_lan_frame(oal_netbuf_head_stru *pst_netbuf_header)
{
    uint32_t ul_netbuf_num;
    oal_netbuf_stru *pst_temp_netbuf = OAL_PTR_NULL;
    oal_netbuf_stru *pst_netbuf = OAL_PTR_NULL;
    uint8_t uc_buf_nums;
    mac_rx_ctl_stru *pst_rx_ctrl = OAL_PTR_NULL;
    mac_ieee80211_frame_stru *pst_frame_hdr = OAL_PTR_NULL;
    hmac_vap_stru *pst_vap = OAL_PTR_NULL;
#if defined(_PRE_WLAN_FEATURE_VIRTUAL_MULTI_STA)
    uint8_t *puc_mac_addr;
#endif

    ul_netbuf_num = oal_netbuf_get_buf_num(pst_netbuf_header);
    pst_temp_netbuf = oal_netbuf_peek(pst_netbuf_header);

    while (ul_netbuf_num != 0) {
        pst_netbuf = pst_temp_netbuf;
        if (pst_netbuf == NULL) {
            break;
        }

        pst_rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb(pst_netbuf);
        pst_frame_hdr = (mac_ieee80211_frame_stru *)MAC_GET_RX_CB_MAC_HEADER_ADDR(pst_rx_ctrl);
        uc_buf_nums = pst_rx_ctrl->bit_buff_nums;

        oal_netbuf_get_appointed_netbuf(pst_netbuf, uc_buf_nums, &pst_temp_netbuf);
        ul_netbuf_num = oal_sub(ul_netbuf_num, uc_buf_nums);

        pst_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_rx_ctrl->uc_mac_vap_id);
        if (pst_vap == OAL_PTR_NULL) {
            oam_error_log1(0, OAM_SF_RX, "{hmac_rx_lan_frame::mac_res_get_hmac_vap null. vap_id:%u}",
                           pst_rx_ctrl->uc_mac_vap_id);
            continue;
        }

        MAC_GET_RX_CB_DA_USER_IDX(pst_rx_ctrl) = pst_vap->st_vap_base_info.us_assoc_vap_id;

#if defined(_PRE_WLAN_FEATURE_VIRTUAL_MULTI_STA)
        mac_rx_get_sa(pst_frame_hdr, &puc_mac_addr);
        /* Also we should drop multicast pkt whose sa belond to lan side(like LanCpe/br0) */
        if ((pst_vap->st_wds_table.en_wds_vap_mode == WDS_MODE_REPEATER_STA) &&
            (OAL_SUCC == hmac_wds_neigh_not_expired(pst_vap, puc_mac_addr))) {
            oam_warning_log1(pst_rx_ctrl->uc_mac_vap_id, OAM_SF_RX,
                             "{hmac_rx_lan_frame::free %d netbuf up to netdevice.}",
                             uc_buf_nums);
            hmac_rx_free_netbuf(pst_netbuf, (uint16_t)uc_buf_nums);
            continue;
        }
#endif

        hmac_rx_lan_frame_classify(pst_vap, pst_netbuf, pst_frame_hdr);
    }

    if (OAL_TRUE == hmac_get_rxthread_enable()) {
        hmac_rxdata_sched();
    }

    return;
}


void hmac_rx_process_data_ap_tcp_ack_opt(hmac_vap_stru *pst_vap, oal_netbuf_head_stru *pst_netbuf_header)
{
    frw_event_hdr_stru st_event_hdr;
    mac_ieee80211_frame_stru *pst_frame_hdr = OAL_PTR_NULL;      /* 保存mac帧的指针 */
    mac_ieee80211_frame_stru *pst_copy_frame_hdr = OAL_PTR_NULL; /* 保存mac帧的指针 */
    uint8_t *puc_da = OAL_PTR_NULL;                            /* 保存用户目的地址的指针 */
    hmac_user_stru *pst_hmac_da_user = OAL_PTR_NULL;
    uint32_t ul_rslt;
    uint16_t us_user_dix;
    mac_rx_ctl_stru *pst_rx_ctrl = OAL_PTR_NULL;     /* 每一个MPDU的控制信息 */
    uint16_t us_netbuf_num;                        /* netbuf链表的个数 */
    uint8_t uc_buf_nums;                           /* 每个mpdu占有buf的个数 */
    oal_netbuf_stru *pst_netbuf = OAL_PTR_NULL;      /* 用于保存当前处理的MPDU的第一个netbuf指针 */
    oal_netbuf_stru *pst_temp_netbuf = OAL_PTR_NULL; /* 用于临时保存下一个需要处理的netbuf指针 */
    oal_netbuf_stru *pst_netbuf_copy = OAL_PTR_NULL; /* 用于保存组播帧copy */
    oal_netbuf_head_stru st_w2w_netbuf_hdr;          /* 保存wlan to wlan的netbuf链表的头 */

    /* 循环收到的每一个MPDU，处情况如下:
        1、组播帧时，调用WLAN TO WLAN和WLAN TO LAN接口
        2、其他，根据实际情况，调用WLAN TO LAN接口或者WLAN TO WLAN接口 */
    oal_netbuf_list_head_init(&st_w2w_netbuf_hdr);
    pst_temp_netbuf = oal_netbuf_peek(pst_netbuf_header);
    us_netbuf_num = (uint16_t)oal_netbuf_get_buf_num(pst_netbuf_header);
    st_event_hdr.uc_chip_id = pst_vap->st_vap_base_info.uc_chip_id;
    st_event_hdr.uc_device_id = pst_vap->st_vap_base_info.uc_device_id;
    st_event_hdr.uc_vap_id = pst_vap->st_vap_base_info.uc_vap_id;

    while (us_netbuf_num != 0) {
        pst_netbuf = pst_temp_netbuf;
        if (pst_netbuf == OAL_PTR_NULL) {
            break;
        }

        pst_rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb(pst_netbuf);

        /* 获取帧头信息 */
        pst_frame_hdr = (mac_ieee80211_frame_stru *)MAC_GET_RX_CB_MAC_HEADER_ADDR(pst_rx_ctrl);

        /* 获取当前MPDU占用的netbuf数目 */
        uc_buf_nums = pst_rx_ctrl->bit_buff_nums;

        /* 获取下一个要处理的MPDU */
        oal_netbuf_get_appointed_netbuf(pst_netbuf, uc_buf_nums, &pst_temp_netbuf);
        us_netbuf_num = oal_sub(us_netbuf_num, uc_buf_nums);

        pst_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_rx_ctrl->uc_mac_vap_id);
        if (oal_unlikely(pst_vap == OAL_PTR_NULL)) {
            oam_warning_log0(pst_rx_ctrl->bit_vap_id, OAM_SF_RX, "{hmac_rx_process_data_ap::pst_vap null.}");
            hmac_rx_free_netbuf(pst_netbuf, (uint16_t)uc_buf_nums);
            continue;
        }

        /* 获取接收端地址  */
        mac_rx_get_da(pst_frame_hdr, &puc_da);

        /* 目的地址为组播地址时，进行WLAN_TO_WLAN和WLAN_TO_LAN的转发 */
        if (ether_is_multicast(puc_da)) {
            oam_info_log0(st_event_hdr.uc_vap_id, OAM_SF_RX,
                          "{hmac_rx_lan_frame_classify::the frame is a group frame.}");
            oam_stat_vap_incr(pst_vap->st_vap_base_info.uc_vap_id, rx_mcast_cnt, 1);

            if (OAL_SUCC != hmac_rx_copy_netbuff(&pst_netbuf_copy, pst_netbuf,
                                                 pst_rx_ctrl->uc_mac_vap_id, &pst_copy_frame_hdr)) {
                oam_warning_log0(st_event_hdr.uc_vap_id, OAM_SF_RX,
                                 "{hmac_rx_process_data_ap::send mcast pkt to air fail.}");

                oam_stat_vap_incr(pst_vap->st_vap_base_info.uc_vap_id, rx_no_buff_dropped, 1);
                hmac_rx_free_netbuf(pst_netbuf, (uint16_t)uc_buf_nums);
                continue;
            }

            hmac_rx_lan_frame_classify(pst_vap, pst_netbuf, pst_frame_hdr);  // 上报网络层

            /* 将MPDU解析成单个MSDU，把所有的MSDU组成一个netbuf链 */
            hmac_rx_prepare_msdu_list_to_wlan(pst_vap, &st_w2w_netbuf_hdr, pst_netbuf_copy, pst_copy_frame_hdr);
            continue;
        }

        ul_rslt = OAL_FAIL;
        us_user_dix = g_wlan_spec_cfg->invalid_user_id;
        if ((OAL_FALSE == mac_is_4addr((uint8_t *)pst_frame_hdr)) ||
            ((OAL_TRUE == mac_is_4addr((uint8_t *)pst_frame_hdr)) && (pst_rx_ctrl->bit_amsdu_enable == OAL_FALSE))) {
            /* 获取目的地址对应的用户指针 */
            ul_rslt = mac_vap_find_user_by_macaddr(&pst_vap->st_vap_base_info, puc_da, &us_user_dix);
            if (ul_rslt == OAL_ERR_CODE_PTR_NULL) { /* 查找用户失败 */
                /* 释放当前处理的MPDU占用的netbuf */
                hmac_rx_free_netbuf(pst_netbuf, (uint16_t)uc_buf_nums);
                oam_stat_vap_incr(pst_vap->st_vap_base_info.uc_vap_id, rx_da_check_dropped, 1);
                continue;
            }
        }

        /* 没有找到对应的用户 */
        if (ul_rslt != OAL_SUCC) {
            oam_info_log0(st_event_hdr.uc_vap_id, OAM_SF_RX,
                          "{hmac_rx_lan_frame_classify::the frame is a unique frame.}");
            /* 目的用户不在AP的用户表中，调用wlan_to_lan转发接口 */
            hmac_rx_lan_frame_classify(pst_vap, pst_netbuf, pst_frame_hdr);
            continue;
        }

        /* 目的用户已在AP的用户表中，进行WLAN_TO_WLAN转发 */
        pst_hmac_da_user = (hmac_user_stru *)mac_res_get_hmac_user(us_user_dix);
        if (pst_hmac_da_user == OAL_PTR_NULL) {
            oam_warning_log1(st_event_hdr.uc_vap_id, OAM_SF_RX,
                             "{hmac_rx_lan_frame_classify::pst_hmac_da_user[%d] null.}", us_user_dix);
            oam_stat_vap_incr(pst_vap->st_vap_base_info.uc_vap_id, rx_da_check_dropped, 1);

            hmac_rx_free_netbuf(pst_netbuf, (uint16_t)uc_buf_nums);
            continue;
        }

        if (pst_hmac_da_user->st_user_base_info.en_user_asoc_state != MAC_USER_STATE_ASSOC) {
            oam_warning_log0(st_event_hdr.uc_vap_id, OAM_SF_RX,
                             "{hmac_rx_lan_frame_classify::the station is not associated with ap.}");
            oam_stat_vap_incr(pst_vap->st_vap_base_info.uc_vap_id, rx_da_check_dropped, 1);

            hmac_rx_free_netbuf(pst_netbuf, (uint16_t)uc_buf_nums);
            hmac_mgmt_send_deauth_frame(&pst_vap->st_vap_base_info, puc_da, MAC_NOT_AUTHED, OAL_FALSE);

            continue;
        }

        /* 将目的地址的资源池索引值放到cb字段中，user的asoc id会在关联的时候被赋值 */
        MAC_GET_RX_CB_DA_USER_IDX(pst_rx_ctrl) = pst_hmac_da_user->st_user_base_info.us_assoc_id;

        /* 将MPDU解析成单个MSDU，把所有的MSDU组成一个netbuf链 */
        hmac_rx_prepare_msdu_list_to_wlan(pst_vap, &st_w2w_netbuf_hdr, pst_netbuf, pst_frame_hdr);
    }

    /*  将MSDU链表交给发送流程处理 */
    if (OAL_FALSE == oal_netbuf_list_empty(&st_w2w_netbuf_hdr) && OAL_PTR_NULL != oal_netbuf_tail(&st_w2w_netbuf_hdr) &&
        OAL_PTR_NULL != oal_netbuf_peek(&st_w2w_netbuf_hdr)) {
        oal_netbuf_next((oal_netbuf_tail(&st_w2w_netbuf_hdr))) = OAL_PTR_NULL;
        oal_netbuf_prev((oal_netbuf_peek(&st_w2w_netbuf_hdr))) = OAL_PTR_NULL;

        hmac_rx_transmit_to_wlan(&st_event_hdr, &st_w2w_netbuf_hdr);
    }

    if (OAL_TRUE == hmac_get_rxthread_enable()) {
        hmac_rxdata_sched();
    }
}


uint32_t hmac_rx_process_data_sta_tcp_ack_opt(hmac_vap_stru *pst_vap, oal_netbuf_head_stru *pst_netbuf_header)
{
    /* 将需要上报的帧逐一出队处理 */
    hmac_rx_lan_frame(pst_netbuf_header);

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_MONITOR
OAL_STATIC int32_t hmac_sniffer_get_pkt_len_phy_addr(hmac_device_stru *hmac_device,
    mac_rx_ctl_stru *rx_ctrl, uint8_t **mac_payload_addr)
{
    int32_t pkt_len = 0;
    if (hmac_device->sniffer_mode == WLAN_SINFFER_ON) {
        pkt_len = rx_ctrl->us_frame_len - SNIFFER_RX_INFO_SIZE;
        *mac_payload_addr += (rx_ctrl->us_frame_len - SNIFFER_RX_INFO_SIZE); // 偏移到payload尾部phy信息起始位置
    } else {
        pkt_len = rx_ctrl->us_frame_len;
        *mac_payload_addr += rx_ctrl->uc_mac_header_len;
    }

    return pkt_len;
}

OAL_STATIC oal_bool_enum_uint8 hmac_sniffer_data_is_invalid(hmac_device_stru *hmac_device,
    int32_t pkt_len, mac_rx_ctl_stru *rx_ctrl)
{
    uint16_t payload_len = rx_ctrl->us_frame_len - rx_ctrl->uc_mac_header_len;

    return (pkt_len < 0 ||
            ((hmac_device->sniffer_mode == WLAN_SINFFER_ON) && (payload_len < SNIFFER_RX_INFO_SIZE)) ||
            ((hmac_device->sniffer_mode == WLAN_SNIFFER_TRAVEL_CAP_ON) &&
            (payload_len < SNIFFER_RX_INFO_SIZE + sizeof(uint16_t))));
}


void hmac_sniffer_save_data(hmac_device_stru *hmac_device,
    oal_netbuf_stru *pst_netbuf, uint16_t us_netbuf_num)
{
    oal_netbuf_stru *pst_temp_netbuf = OAL_PTR_NULL;
    mac_rx_ctl_stru *pst_rx_ctrl = OAL_PTR_NULL; /* 指向MPDU控制块信息的指针 */
    uint8_t *puc_mac_payload_addr = OAL_PTR_NULL;
    hal_sniffer_rx_status_stru *pst_rx_status = OAL_PTR_NULL;
    hal_sniffer_rx_statistic_stru *pst_sniffer_rx_statistic = OAL_PTR_NULL;
    ieee80211_radiotap_stru st_ieee80211_radiotap;
    hal_statistic_stru *pst_per_rate = OAL_PTR_NULL;
    uint32_t *pul_rate_kbps = OAL_PTR_NULL;
    int32_t pkt_len;
    /******************************************************************************************************************/
    /*                   netbuf                                                                                  */
    /* -------------------------------------------------------------------------------------------------------------- */
    /* |frame body |rx status |rx statistic | rate_kbps |  rate info   | original frame len(travel mode only) |       */
    /* -------------------------------------------------------------------------------------------------------------- */
    /* |variable   |   6      |     8       | 4         |      4       |                    2                 |       */
    /* -------------------------------------------------------------------------------------------------------------- */
    /*                                                                                                                */
    /******************************************************************************************************************/
    while (us_netbuf_num != 0) {
        pst_temp_netbuf = pst_netbuf;
        if (pst_temp_netbuf == OAL_PTR_NULL) {
            break;
        }

        pst_netbuf = oal_netbuf_next(pst_temp_netbuf);

        /* 获取该MPDU的控制信息 */
        pst_rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb(pst_temp_netbuf);
        puc_mac_payload_addr = oal_netbuf_payload(pst_temp_netbuf);
        pkt_len = hmac_sniffer_get_pkt_len_phy_addr(hmac_device, pst_rx_ctrl, &puc_mac_payload_addr);
        if (hmac_sniffer_data_is_invalid(hmac_device, pkt_len, pst_rx_ctrl)) {
            oam_warning_log1(0, OAM_SF_RX, "iPacketLength(%d) < 0", pkt_len);
            continue;
        }

        pst_rx_status = (hal_sniffer_rx_status_stru *)puc_mac_payload_addr;
        pst_sniffer_rx_statistic = (hal_sniffer_rx_statistic_stru *)(puc_mac_payload_addr +
            OAL_SIZEOF(hal_sniffer_rx_status_stru));
        pul_rate_kbps = (uint32_t *)((puc_mac_payload_addr +
            (OAL_SIZEOF(hal_sniffer_rx_status_stru) + OAL_SIZEOF(hal_sniffer_rx_statistic_stru))));
        pst_per_rate =
            (hal_statistic_stru *)(puc_mac_payload_addr + (OAL_SIZEOF(hal_sniffer_rx_status_stru) +
            OAL_SIZEOF(hal_sniffer_rx_statistic_stru) + OAL_SIZEOF(uint32_t)));

#ifdef _PRE_WLAN_FEATURE_MONITOR_DEBUG
        oam_warning_log4(0, OAM_SF_RX,
            "{hmac_sniffer_save_data:: chn[%d], frame len[%d], pul_rate_kbps[%d], uc_bandwidth[%d].}",
            pst_rx_ctrl->uc_channel_number, pst_rx_ctrl->us_frame_len, (*pul_rate_kbps), pst_per_rate->uc_bandwidth);

        oam_warning_log4(0, OAM_SF_RX,
                         "{hmac_sniffer_save_data::c_rssi_dbm[%d], c_snr_ant0[%d], c_snr_ant1[%d], c_ant0_rssi[%d].}",
                         pst_sniffer_rx_statistic->c_rssi_dbm, pst_sniffer_rx_statistic->c_snr_ant0,
                         pst_sniffer_rx_statistic->c_snr_ant1, pst_sniffer_rx_statistic->c_ant0_rssi);

        oam_warning_log4(0, OAM_SF_RX,
            "{hmac_sniffer_save_data::status[%d],freq_bandwidth_mode[%d], ext_spatial_streams[%d], protocol_mode[%d].}",
            pst_rx_status->bit_dscr_status, pst_rx_status->bit_freq_bandwidth_mode,
            pst_rx_status->bit_ext_spatial_streams, pst_rx_status->un_nss_rate.st_rate.bit_protocol_mode);
#endif
        hmac_sniffer_fill_radiotap(&st_ieee80211_radiotap, pst_rx_ctrl, pst_rx_status,
                                   pst_sniffer_rx_statistic, puc_mac_payload_addr, pul_rate_kbps, pst_per_rate);
#ifdef _PRE_WLAN_FEATURE_SNIFFER
#ifdef CONFIG_HW_SNIFFER
        /* 调用终端提供的接口写入到文件 */
        proc_sniffer_write_file((uint8_t *)&st_ieee80211_radiotap, OAL_SIZEOF(ieee80211_radiotap_stru),
                                (uint8_t *)oal_netbuf_payload(pst_temp_netbuf), pkt_len, DIRECTION_MONITOR_MODE);
#endif
#endif
        us_netbuf_num--;
    }

    return;
}
#endif


OAL_STATIC uint32_t hmac_rx_wlan_frame_process(hmac_vap_stru *hmac_vap,
    oal_netbuf_stru *netbuf, mac_ieee80211_frame_stru *frame_hdr)
{
    mac_rx_ctl_stru *rx_ctrl = NULL;
    dmac_msdu_stru rx_msdu;
    uint8_t *frame_addr = NULL;
    hmac_user_stru *hmac_user = NULL;
    uint8_t frm_type;

    memset_s(&rx_msdu, OAL_SIZEOF(dmac_msdu_stru), 0, OAL_SIZEOF(dmac_msdu_stru));
    mac_get_transmit_addr(frame_hdr, &frame_addr);
    oal_set_mac_addr(rx_msdu.auc_ta, frame_addr);
    /* 获取该MPDU的控制信息 */
    rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);

    hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(MAC_GET_RX_CB_TA_USER_IDX(rx_ctrl));
    if (oal_unlikely(hmac_user == OAL_PTR_NULL)) {
        oam_error_log4(0, 0, "{hmac_rx_wlan_frame_process:vap:%d machdr_len:%d,frm_len:%d machdr_addr:0x%08x}",
            rx_ctrl->bit_vap_id, rx_ctrl->uc_mac_header_len, rx_ctrl->us_frame_len,
            (uintptr_t)MAC_GET_RX_CB_MAC_HEADER_ADDR(rx_ctrl));
    }

    /* 重新获取该MPDU的控制信息 */
    rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);

    /* 打印出关键帧信息 */
    frm_type = mac_get_data_type_from_80211(netbuf, rx_ctrl->uc_mac_header_len);
    if (frm_type <= MAC_DATA_ARP_RSP) {
        hmac_rx_vip_info(&hmac_vap->st_vap_base_info, frm_type, netbuf, rx_ctrl);
    }

    /* 对当前的msdu进行赋值 */
    rx_msdu.pst_netbuf = netbuf;

    /* 获取源地址和目的地址 */
    mac_rx_get_sa(frame_hdr, &frame_addr);
    oal_set_mac_addr(rx_msdu.auc_sa, frame_addr);
    mac_rx_get_da(frame_hdr, &frame_addr);
    oal_set_mac_addr(rx_msdu.auc_da, frame_addr);

    /* 将netbuf的data指针指向mac frame的payload处 */
    oal_netbuf_pull(netbuf, rx_ctrl->uc_mac_header_len);
    return hmac_rx_frame_80211_to_eth(netbuf, rx_msdu.auc_da, rx_msdu.auc_sa);
}


OAL_STATIC void hmac_rx_statistics(hmac_vap_stru *pst_vap, oal_netbuf_stru *pst_netbuf)
{
    /* 增加统计信息 */
    HMAC_VAP_DFT_STATS_PKT_INCR(pst_vap->st_query_stats.ul_rx_pkt_to_lan, 1);
    HMAC_VAP_DFT_STATS_PKT_INCR(pst_vap->st_query_stats.ul_rx_bytes_to_lan, oal_netbuf_len(pst_netbuf));
    oam_stat_vap_incr(pst_vap->st_vap_base_info.uc_vap_id, rx_pkt_to_lan, 1); /* 增加发往LAN的帧的数目 */
    /* 增加发送LAN的字节数 */
    oam_stat_vap_incr(pst_vap->st_vap_base_info.uc_vap_id, rx_bytes_to_lan, oal_netbuf_len(pst_netbuf));
    return;
}


uint32_t hmac_rx_mpdu_free(oal_netbuf_stru *p_netbuf_mpdu)
{
    oal_netbuf_stru *netbuf_cur;
    oal_netbuf_stru *netbuf_next = OAL_PTR_NULL;

    netbuf_cur = p_netbuf_mpdu;
    while (netbuf_cur != OAL_PTR_NULL) {
        netbuf_next = oal_netbuf_next(netbuf_cur);
        oal_netbuf_free(netbuf_cur);
        netbuf_cur = netbuf_next;
    }
    return OAL_SUCC;
}


OAL_STATIC uint64_t hmac_user_pn_get(hmac_last_pn_stru *last_pn_info, mac_rx_ctl_stru *rx_ctl, uint8_t qos)
{
    uint64_t last_pn_val;

    if (qos == OAL_TRUE) {
        last_pn_val = ((uint64_t)last_pn_info->qos_last_msb_pn[rx_ctl->rx_tid] << 32) |
                           (last_pn_info->qos_last_lsb_pn[rx_ctl->rx_tid]);
    } else {
        /* 非qos时，组播和单播使用的pn号不一样 */
        if (MAC_RX_CB_IS_MULTICAST(rx_ctl) == OAL_TRUE) {
            last_pn_val = ((uint64_t)last_pn_info->mcast_nonqos_last_msb_pn << 32) |
                               (last_pn_info->mcast_nonqos_last_lsb_pn);
        } else {
            last_pn_val = ((uint64_t)last_pn_info->ucast_nonqos_last_msb_pn << 32) |
                               (last_pn_info->ucast_nonqos_last_lsb_pn);
        }
    }
    return last_pn_val;
}


OAL_STATIC void hmac_user_pn_update(hmac_last_pn_stru *hmac_last_pn, mac_rx_ctl_stru *rx_ctl, uint8_t qos)
{
    if (qos == OAL_TRUE) {
        hmac_last_pn->qos_last_msb_pn[rx_ctl->rx_tid] = rx_ctl->us_rx_msb_pn;
        hmac_last_pn->qos_last_lsb_pn[rx_ctl->rx_tid] = rx_ctl->ul_rx_lsb_pn;
    } else {
        if (MAC_RX_CB_IS_MULTICAST(rx_ctl) == OAL_TRUE) {
            hmac_last_pn->mcast_nonqos_last_msb_pn = rx_ctl->us_rx_msb_pn;
            hmac_last_pn->mcast_nonqos_last_lsb_pn = rx_ctl->ul_rx_lsb_pn;
        } else {
            hmac_last_pn->ucast_nonqos_last_msb_pn = rx_ctl->us_rx_msb_pn;
            hmac_last_pn->ucast_nonqos_last_lsb_pn = rx_ctl->ul_rx_lsb_pn;
        }
    }
}

/*
* PN窗口:
*  1)新报文的PN如果在窗口外，强制移"窗"，同时丢掉新报文;
*  2)新报文的PN如果在(last_PN-256 ~ last_PN)之间，认为PN检查失败，丢掉新报文.
*/
#define HMAC_PN_WINDOW_SIZE 256
#define HMAC_NEED_PN_WINDOD_SHIFT(_new, _cur)  (((_new) + HMAC_PN_WINDOW_SIZE) < (_cur))

void hmac_rx_pn_check(oal_netbuf_stru *netbuf)
{
    uint64_t                       last_pn_val;
    uint64_t                       rx_pn_val;
    oal_uint8                      qos_flg = OAL_FALSE;
    hmac_user_stru                *pst_user = NULL;
    mac_rx_ctl_stru               *rx_ctl = NULL;
    mac_header_frame_control_stru *frame_control = NULL;

    rx_ctl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);
    frame_control = (mac_header_frame_control_stru *)&rx_ctl->us_frame_control;

    if ((frame_control->bit_protected_frame == OAL_FALSE) || (frame_control->bit_retry == OAL_TRUE)) {
        return;
    }

    /* 如果是BA缓存区超时上报的报文，则不进行检查 */
    if (OAL_TRUE == MAC_GET_RX_CB_IS_REO_TIMEOUT(rx_ctl)) {
        oam_warning_log1(0, OAM_SF_RX, "{hmac_rx_pn_check::SEQ NUM[%d].}", rx_ctl->us_seq_num);
        return;
    }

    /* AMSDU聚合包的pn号一样，故只检查首包  */
    if (MAC_RXCB_IS_AMSDU_SUB_MSDU(rx_ctl)) {
        return;
    }

    if ((frame_control->bit_type == WLAN_DATA_BASICTYPE) && (frame_control->bit_sub_type >= WLAN_QOS_DATA) &&
        (frame_control->bit_sub_type <= WLAN_QOS_NULL_FRAME)) {
        qos_flg = OAL_TRUE;
    }
    /* TID 合法性检查 */
    if (oal_unlikely((qos_flg == OAL_TRUE) && (rx_ctl->rx_tid >= WLAN_TID_MAX_NUM))) {
        oam_warning_log3(0, OAM_SF_RX, "{hmac_rx_pn_check::uc_qos_flg %d user id[%d] tid[%d].}",
                         qos_flg, MAC_RXCB_TA_USR_ID(rx_ctl), rx_ctl->rx_tid);
        return;
    }

    rx_pn_val = ((uint64_t)rx_ctl->us_rx_msb_pn << 32) | (rx_ctl->ul_rx_lsb_pn);
    pst_user = (hmac_user_stru *)mac_res_get_hmac_user(MAC_GET_RX_CB_TA_USER_IDX(rx_ctl));
    if (pst_user == OAL_PTR_NULL) {
        oam_warning_log1(0, OAM_SF_RX, "{hmac_rx_pn_check::hmac_user[%d] null.}", MAC_GET_RX_CB_TA_USER_IDX(rx_ctl));
        return;
    }

    last_pn_val = hmac_user_pn_get(&pst_user->last_pn_info, rx_ctl, qos_flg);
    /* 首包pn为0，此处对首包不检查 */
    if ((last_pn_val >= rx_pn_val) && (last_pn_val != 0)) {
        oam_error_log4(0, OAM_SF_RX, "{hmac_rx_pn_check::last pn[%ld] send pn[%ld] user id[%d] tid[%d].}",
                       last_pn_val, rx_pn_val, MAC_RXCB_TA_USR_ID(rx_ctl), rx_ctl->rx_tid);
        oam_error_log3(0, OAM_SF_RX, "{hmac_rx_pn_check:: en_qos[%d] seq_num[%d] is_mcast[%d].}",
                       qos_flg, rx_ctl->us_seq_num, MAC_RX_CB_IS_MULTICAST(rx_ctl));
        if (HMAC_NEED_PN_WINDOD_SHIFT(rx_pn_val, last_pn_val)) {
            hmac_user_pn_update(&pst_user->last_pn_info, rx_ctl, qos_flg);
        }
        return;
    }

    hmac_user_pn_update(&pst_user->last_pn_info, rx_ctl, qos_flg);
}


OAL_STATIC void hmac_rx_enqueue(oal_netbuf_stru *netbuf, oal_net_device_stru *net_dev)
{
    oal_netbuf_prev(netbuf) = OAL_PTR_NULL;
    oal_netbuf_next(netbuf) = OAL_PTR_NULL;

    hmac_rx_pn_check(netbuf);

    /* 对protocol模式赋值 */
    oal_netbuf_protocol(netbuf) = oal_eth_type_trans(netbuf, net_dev);
    memset_s(oal_netbuf_cb(netbuf), oal_netbuf_cb_size(), 0, oal_netbuf_cb_size());

    if (OAL_TRUE == hmac_get_rxthread_enable()) {
        hmac_rxdata_netbuf_enqueue(netbuf);
    } else {
        oal_notice_netif_rx(netbuf);
        oal_netif_rx_ni(netbuf);
    }
    return;
}


static inline void hmac_rx_prepare_to_wlan(oal_netbuf_head_stru *netbuf_header, oal_netbuf_stru *netbuf)
{
#ifdef _PRE_WLAN_PKT_TIME_STAT
    memset_s(oal_netbuf_cb(netbuf), OAL_NETBUF_CB_ORIGIN, 0, OAL_NETBUF_CB_ORIGIN);
#else
    memset_s(oal_netbuf_cb(netbuf), oal_netbuf_cb_size(), 0, oal_netbuf_cb_size());
#endif
    /* 将MSDU加入到netbuf链的最后 */
    oal_netbuf_add_to_list_tail(netbuf, netbuf_header);
    return;
}

OAL_STATIC uint8_t hmac_rx_is_vap_status_valid(mac_rx_ctl_stru *rx_ctl, hmac_vap_stru **pphmac_vap)
{
    hmac_vap_stru *hmac_vap = OAL_PTR_NULL;

    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(rx_ctl->uc_mac_vap_id);
    if (oal_unlikely(hmac_vap == OAL_PTR_NULL)) {
        return OAL_FALSE;
    }

    if (hmac_vap->pst_net_device == OAL_PTR_NULL) {
        return OAL_FALSE;
    }

    *pphmac_vap = hmac_vap;
    return OAL_TRUE;
}


OAL_STATIC uint8_t hmac_rx_w2w_is_da_within_same_bss(oal_netbuf_stru *netbuf, mac_rx_ctl_stru *rx_ctl,
    hmac_vap_stru *hmac_vap)
{
    hmac_user_stru *hmac_da_user = NULL;

    hmac_da_user = (hmac_user_stru *)mac_res_get_hmac_user(MAC_GET_RX_DST_USER_ID(rx_ctl));
    if (hmac_da_user == NULL) {
        hmac_rx_enqueue(netbuf, hmac_vap->pst_net_device);
        return OAL_FALSE;
    }

    if (hmac_da_user->st_user_base_info.en_user_asoc_state != MAC_USER_STATE_ASSOC) {
        hmac_rx_free_netbuf(netbuf, (uint16_t)1);
        hmac_mgmt_send_deauth_frame(&hmac_vap->st_vap_base_info,
            hmac_da_user->st_user_base_info.auc_user_mac_addr, MAC_NOT_AUTHED, OAL_FALSE);
        return OAL_FALSE;
    }
    /* tdo */
    MAC_GET_RX_CB_DA_USER_IDX(rx_ctl) = hmac_da_user->st_user_base_info.us_assoc_id;
    return OAL_TRUE;
}

OAL_STATIC OAL_INLINE uint8_t *hmac_rx_get_eth_da(oal_netbuf_stru *netbuf)
{
    mac_ether_header_stru    *p_eth_hdr = OAL_PTR_NULL;

    p_eth_hdr = (mac_ether_header_stru *)oal_netbuf_data(netbuf);
    return (uint8_t *)p_eth_hdr->auc_ether_dhost;
}


OAL_STATIC void hmac_rx_wlan_to_wlan_proc(hmac_vap_stru *hmac_vap,
    oal_netbuf_head_stru *pw2w_netbuf_hdr)
{
    frw_event_hdr_stru event_hdr;
    memset_s(&event_hdr, sizeof(frw_event_hdr_stru), 0, sizeof(frw_event_hdr_stru));

    event_hdr.uc_chip_id = hmac_vap->st_vap_base_info.uc_chip_id;
    event_hdr.uc_device_id = hmac_vap->st_vap_base_info.uc_device_id;
    event_hdr.uc_vap_id = hmac_vap->st_vap_base_info.uc_vap_id;

    if (OAL_FALSE == oal_netbuf_list_empty(pw2w_netbuf_hdr) &&
        NULL != oal_netbuf_tail(pw2w_netbuf_hdr) &&
        NULL != oal_netbuf_peek(pw2w_netbuf_hdr)) {
        oal_netbuf_next((oal_netbuf_tail(pw2w_netbuf_hdr))) = OAL_PTR_NULL;
        oal_netbuf_prev((oal_netbuf_peek(pw2w_netbuf_hdr))) = OAL_PTR_NULL;
        if (OAL_SUCC != hmac_rx_transmit_to_wlan(&event_hdr, pw2w_netbuf_hdr)) {
            oam_warning_log0(0, 0, "{hmac_rx_wlan_to_wlan_proc:: rx transmit to wlan fail}");
        }
    }
    return;
}


void hmac_rx_data_ap_proc(hmac_vap_stru *hmac_vap, oal_netbuf_head_stru *p_netbuf_head)
{
    oal_netbuf_stru          *netbuf = OAL_PTR_NULL;
    oal_netbuf_stru          *temp_netbuf = OAL_PTR_NULL;
    mac_rx_ctl_stru          *rx_ctl = OAL_PTR_NULL;
    mac_ieee80211_frame_stru *copy_frame_hdr = OAL_PTR_NULL;
    uint16_t                  netbuf_num;
    oal_netbuf_stru          *netbuf_copy = OAL_PTR_NULL;
    oal_netbuf_head_stru      w2w_netbuf_hdr;
    hmac_vap_stru            *hmac_vap_temp = OAL_PTR_NULL;

    oal_netbuf_list_head_init(&w2w_netbuf_hdr);
    temp_netbuf = oal_netbuf_peek(p_netbuf_head);
    netbuf_num = (uint16_t)oal_netbuf_get_buf_num(p_netbuf_head);
    while (netbuf_num != 0) {
        netbuf = temp_netbuf;
        if (netbuf == OAL_PTR_NULL) {
            break;
        }

        oal_netbuf_get_appointed_netbuf(netbuf, 1, &temp_netbuf);
        netbuf_num = oal_sub(netbuf_num, 1);
        rx_ctl     = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);
        if (OAL_TRUE != hmac_rx_is_vap_status_valid(rx_ctl, &hmac_vap_temp)) {
            hmac_rx_free_netbuf(netbuf, (uint16_t)1);
            continue;
        }

        if ((OAL_TRUE == ether_is_multicast(hmac_rx_get_eth_da(netbuf)))) {
            if (OAL_SUCC != hmac_rx_copy_netbuff(&netbuf_copy, netbuf, rx_ctl->uc_mac_vap_id, &copy_frame_hdr)) {
                hmac_rx_free_netbuf(netbuf, (uint16_t)1);
                continue;
            }

            hmac_rx_enqueue(netbuf, hmac_vap_temp->pst_net_device);
            hmac_rx_prepare_to_wlan(&w2w_netbuf_hdr, netbuf_copy);
            continue;
        }

        /* check hw valid status */
        if (MAC_RX_CB_IS_DEST_CURR_BSS(rx_ctl)) {
            /* check if da belong to the same bss */
            if (OAL_TRUE != hmac_rx_w2w_is_da_within_same_bss(netbuf, rx_ctl, hmac_vap_temp)) {
                continue;
            }

            /* 目的用户已在AP的用户表中，进行WLAN_TO_WLAN转发 */
            hmac_rx_prepare_to_wlan(&w2w_netbuf_hdr, netbuf);
        } else {
            hmac_rx_enqueue(netbuf, hmac_vap_temp->pst_net_device);
        }
    }

    /*  将MSDU链表交给发送流程处理 */
    hmac_rx_wlan_to_wlan_proc(hmac_vap, &w2w_netbuf_hdr);
    /* sch frames to kernel */
    if (OAL_TRUE == hmac_get_rxthread_enable()) {
        hmac_rxdata_sched();
    }
}


oal_bool_enum_uint8 hmac_rx_is_qos_data_frame(mac_rx_ctl_stru *rx_ctl)
{
    uint8_t frame_type = mac_get_frame_type_and_subtype((uint8_t *)&rx_ctl->us_frame_control);

    return (frame_type != (WLAN_FC0_SUBTYPE_QOS | WLAN_FC0_TYPE_DATA)) ? OAL_FALSE : OAL_TRUE;
}


static oal_bool_enum_uint8 hmac_rx_qos_data_need_drop(oal_netbuf_stru *netbuf_mpdu,
    hmac_vap_stru *hmac_vap, mac_rx_ctl_stru *rx_ctl)
{
    hmac_user_stru                *hmac_user = NULL;
    hmac_ba_rx_stru               *ba_rx_hdl = NULL;
    uint16_t                       seq_frag_num;
    uint8_t                        rx_tid;

    if (!hmac_rx_is_qos_data_frame(rx_ctl)) {
        return OAL_FALSE;
    }

    rx_tid = MAC_GET_RX_CB_TID(rx_ctl);
    hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(MAC_GET_RX_CB_TA_USER_IDX(rx_ctl));
    if (!MAC_TID_IS_VALID(rx_tid) || oal_any_null_ptr1(hmac_user)) {
        oam_error_log2(0, OAM_SF_RX, "{hmac_rx_qos_data_need_drop:: tid invalid[%d]or hmac_user[%d] null.}",
            rx_tid, MAC_GET_RX_CB_TA_USER_IDX(rx_ctl));
        return OAL_FALSE;
    }

    /* qos 已建ba返回 */
    ba_rx_hdl = hmac_user->ast_tid_info[rx_tid].pst_ba_rx_info;
    if (ba_rx_hdl != NULL && (ba_rx_hdl->en_ba_status == DMAC_BA_COMPLETE)) {
        return OAL_FALSE;
    }

    if (rx_ctl->bit_is_fragmented) {
        seq_frag_num = ((oal_uint16)(rx_ctl->us_seq_num) << 4) | rx_ctl->bit_fragment_num; /* 合并sn放高4位 */
    } else {
        seq_frag_num = rx_ctl->us_seq_num;
    }

    /* no ba qos data，retry 且seq num相同则过滤，否则更新 */
    if (((mac_header_frame_control_stru *)&rx_ctl->us_frame_control)->bit_retry == OAL_TRUE &&
        (seq_frag_num == hmac_user->qos_last_seq_frag_num[rx_tid])) {
        oam_warning_log2(hmac_user->st_user_base_info.uc_vap_id, OAM_SF_RX,
            "{hmac_rx_qos_data_need_drop::tid is[%d],qos duplicate frame[%d].}", rx_tid, seq_frag_num);
        return OAL_TRUE;
    }

    hmac_user->qos_last_seq_frag_num[rx_tid] = seq_frag_num;
    return OAL_FALSE;
}

OAL_STATIC uint32_t hmac_rx_data_pre_proc(hmac_vap_stru *hmac_vap, oal_netbuf_stru *netbuf)
{
    mac_rx_ctl_stru *rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);
    oal_net_device_stru *net_dev = hmac_vap->pst_net_device;
    mac_ieee80211_frame_stru *frame_hdr = OAL_PTR_NULL;

    /* 对于上报的eth格式帧，不使用此变量 */
    frame_hdr = (mac_ieee80211_frame_stru *)MAC_GET_RX_CB_MAC_HEADER_ADDR(rx_ctrl);
    if ((rx_ctrl->bit_buff_nums > 1) || oal_any_null_ptr2(net_dev, MAC_GET_RX_CB_MAC_HEADER_ADDR(rx_ctrl))) {
        oam_warning_log1(0, 0, "{hmac_rx_data_pre_proc:rx cb buf nums[%d] err}", rx_ctrl->bit_buff_nums);
        return OAL_FAIL;
    }

    if ((rx_ctrl->bit_frame_format != MAC_FRAME_TYPE_80211) && (rx_ctrl->bit_frame_format != MAC_FRAME_TYPE_RTH)) {
        oam_error_log1(0, 0, "{hmac_rx_data_pre_proc::frm_format[%d] err}", rx_ctrl->bit_frame_format);
        return OAL_FAIL;
    }

    /* 去掉hcc多计算的帧长 */
    if (rx_ctrl->us_frame_len > rx_ctrl->uc_mac_header_len) {
        rx_ctrl->us_frame_len -= rx_ctrl->uc_mac_header_len;
    }

    if (hmac_rx_qos_data_need_drop(netbuf, hmac_vap, rx_ctrl)) {
        oam_warning_log0(0, OAM_SF_RX, "{hmac_rx_data_pre_proc:: dup mpdu free buf}");
        return OAL_FAIL;
    }

    /* 去分片处理及帧格式转换 */
    if (rx_ctrl->bit_frame_format == MAC_FRAME_TYPE_80211) {
        if (OAL_SUCC != hmac_rx_wlan_frame_process(hmac_vap, netbuf, frame_hdr)) {
            return OAL_FAIL;
        }
    }

    /* 增加统计信息 */
    hmac_rx_statistics(hmac_vap, netbuf);

    /* 统计收包的总数，计入全局变量 */
    hmac_auto_freq_wifi_rx_stat(1);
    hmac_auto_freq_wifi_rx_bytes_stat(oal_netbuf_len(netbuf));
    return OAL_SUCC;
}


uint32_t hmac_rx_data_wnd_proc(hmac_user_stru *pst_user, hmac_ba_rx_stru *ba_rx_hdl,
    oal_netbuf_stru *netbuf, oal_netbuf_head_stru *netbuf_head)
{
    mac_rx_ctl_stru *rx_ctl = OAL_PTR_NULL;
    uint32_t         ret = OAL_SUCC;

    if (oal_unlikely(oal_any_null_ptr4(pst_user, ba_rx_hdl, netbuf, netbuf_head))) {
        oam_error_log4(0, 0, "{hmac_rx_data_wnd_proc::hmac_user[%p],ba_rx_hdl[%p],netbuf_mpdu[%p],netbuf_head[%p]}",
            (uintptr_t)pst_user, (uintptr_t)ba_rx_hdl, (uintptr_t)netbuf, (uintptr_t)netbuf_head);
        return OAL_ERR_CODE_PTR_NULL;
    }

    rx_ctl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);
    switch (MAC_GET_RX_CB_PROC_FLAG(rx_ctl)) {
        case HAL_RX_PROC_FLAGS_RELEASE:
            ret = hmac_ba_rx_data_release(pst_user, ba_rx_hdl, netbuf, netbuf_head);
            break;
        case HAL_RX_PROC_FLAGS_BUFFER:
            ret = hmac_ba_rx_data_buffer(pst_user, ba_rx_hdl, netbuf, netbuf_head);
            break;
        case HAL_RX_PROC_FLAGS_OUT_OF_WIND_DROP:
        /* fall through */
        case HAL_RX_PROC_FLAGS_DUPLICATE_DROP:
        /* fall through */
        default:
            hmac_rx_mpdu_free(netbuf);
            break;
    }

    if (oal_unlikely(ret != OAL_SUCC)) {
        /* buffered data soft check失败时会进入此处，目前广播队列也进缓存，因此需要让check失败的包上报 */
        hmac_rx_mpdu_to_netbuf_list(netbuf_head, netbuf);
        oam_warning_log1(0, OAM_SF_RX, "{hmac_rx_data_wnd_proc:process flag:%d}", MAC_GET_RX_CB_PROC_FLAG(rx_ctl));
    }

    return OAL_SUCC;
}

uint32_t hmac_rx_data_reorder_proc(oal_netbuf_head_stru *netbuf_head, oal_netbuf_stru *netbuf)
{
    mac_rx_ctl_stru *rx_ctl = NULL;
    hmac_user_stru  *hmac_user = NULL;
    hmac_vap_stru   *hmac_vap = NULL;
    uint32_t         ret;
    hmac_ba_rx_stru *ba_rx_hdl = NULL;

    rx_ctl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);
    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(rx_ctl->uc_mac_vap_id);
    if (hmac_vap == OAL_PTR_NULL) {
        oam_warning_log1(0, OAM_SF_RX, "{hmac_rx_data_reorder_proc::vap null![%d]}", rx_ctl->uc_mac_vap_id);
        hmac_rx_mpdu_to_netbuf_list(netbuf_head, netbuf);
        return OAL_SUCC;
    }

    ret = hmac_ba_check_rx_aggrate(hmac_vap, rx_ctl);
    if (ret != OAL_SUCC) {
        hmac_rx_mpdu_to_netbuf_list(netbuf_head, netbuf);
        return OAL_SUCC;
    }

    hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(MAC_GET_RX_CB_TA_USER_IDX(rx_ctl));
    if (hmac_user == OAL_PTR_NULL) {
        oam_warning_log1(0, OAM_SF_RX, "{hmac_rx_data_reorder_proc::hmac_user[%d] null.}",
            MAC_GET_RX_CB_TA_USER_IDX(rx_ctl));
        hmac_rx_mpdu_to_netbuf_list(netbuf_head, netbuf);
        return OAL_SUCC;
    }

    ba_rx_hdl = hmac_user->ast_tid_info[MAC_GET_RX_CB_TID(rx_ctl)].pst_ba_rx_info;
    if (ba_rx_hdl == OAL_PTR_NULL) {
        oam_info_log0(0, OAM_SF_RX, "{hmac_rx_data_reorder_proc::rx ba null!}");
        hmac_rx_mpdu_to_netbuf_list(netbuf_head, netbuf);
        return OAL_SUCC;
    }

    hmac_ba_rx_hdl_hold(ba_rx_hdl);
    if (ba_rx_hdl->en_ba_status != DMAC_BA_COMPLETE) {
        hmac_ba_rx_hdl_put(ba_rx_hdl);
        hmac_rx_mpdu_to_netbuf_list(netbuf_head, netbuf);
        return OAL_SUCC;
    }

    ret = hmac_rx_data_wnd_proc(hmac_user, ba_rx_hdl, netbuf, netbuf_head);
    if (ret != OAL_SUCC) {
        hmac_ba_rx_hdl_put(ba_rx_hdl);
        return ret;
    }

    hmac_ba_rx_hdl_put(ba_rx_hdl);
    return OAL_SUCC;
}

void hmac_rx_data_sta_proc(oal_netbuf_head_stru *netbuf_header)
{
    oal_netbuf_stru *netbuf = NULL;
    mac_rx_ctl_stru *rx_ctl = NULL;
    hmac_vap_stru *hmac_vap = NULL;

    while (!oal_netbuf_list_empty(netbuf_header)) {
        netbuf = oal_netbuf_delist(netbuf_header);
        rx_ctl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);
        hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(rx_ctl->uc_mac_vap_id);
        if (hmac_vap == OAL_PTR_NULL) {
            oam_error_log1(0, OAM_SF_RX, "{hmac_rx_data_sta_proc::hmac_vap null. vap_id:%u}", rx_ctl->uc_mac_vap_id);
            continue;
        } else if (hmac_vap->pst_net_device == NULL) {
            oam_error_log1(0, OAM_SF_RX, "{hmac_rx_data_sta_proc::vap_id:%d, netdev null}", rx_ctl->uc_mac_vap_id);

            continue;
        }
        if (rx_ctl->bit_mcast_bcast == OAL_FALSE) {
            oal_atomic_inc(&(hmac_vap->st_hmac_arp_probe.ul_rx_unicast_pkt_to_lan));
        }
        hmac_rx_enqueue(netbuf, hmac_vap->pst_net_device);
    }

    if (OAL_TRUE == hmac_get_rxthread_enable()) {
        hmac_rxdata_sched();
    }

    return;
}

#ifdef CONFIG_ARCH_KIRIN_PCIE
static uint8_t hmac_rx_da_is_in_same_bss(oal_netbuf_stru *netbuf, mac_rx_ctl_stru *rx_ctl,
    hmac_vap_stru *hmac_vap)
{
    hmac_user_stru *hmac_da_user = NULL;
    uint16_t user_dix = 0;
    uint8_t *puc_da = hmac_rx_get_eth_da(netbuf);

    if (mac_vap_find_user_by_macaddr(&hmac_vap->st_vap_base_info, puc_da, &user_dix) != OAL_SUCC) {
        oam_info_log0(0, 0, "{hmac_rx_da_is_in_same_bss::find da fail}");
        return OAL_FALSE;
    }

    hmac_da_user = (hmac_user_stru *)mac_res_get_hmac_user(user_dix);
    if (hmac_da_user == NULL) {
        oam_error_log1(0, 0, "{hmac_rx_da_is_in_same_bss::find da fail usr idx:%d}", user_dix);
        return OAL_FALSE;
    }

    if (hmac_da_user->st_user_base_info.en_user_asoc_state != MAC_USER_STATE_ASSOC) {
        hmac_mgmt_send_deauth_frame(&hmac_vap->st_vap_base_info,
            hmac_da_user->st_user_base_info.auc_user_mac_addr, MAC_NOT_AUTHED, OAL_FALSE);
        return OAL_FALSE;
    }

    MAC_GET_RX_CB_DA_USER_IDX(rx_ctl) = hmac_da_user->st_user_base_info.us_assoc_id;
    return OAL_TRUE;
}


void hmac_rx_host_ring_ap_proc(hmac_vap_stru *hmac_vap, oal_netbuf_head_stru *p_netbuf_head)
{
    oal_netbuf_stru          *netbuf = NULL;
    mac_rx_ctl_stru          *rx_ctl = NULL;
    mac_ieee80211_frame_stru *copy_frame_hdr = NULL;
    uint16_t                  netbuf_num;
    oal_netbuf_stru          *netbuf_copy = NULL;
    oal_netbuf_head_stru      w2w_netbuf_hdr;
    oal_netbuf_stru          *netbuf_next = NULL;

    if (oal_netbuf_list_empty(p_netbuf_head)) {
        oam_error_log0(0, OAM_SF_RX, "{hmac_rx_host_ring_ap_proc::netbuf list empty}");
        return;
    }

    oal_netbuf_list_head_init(&w2w_netbuf_hdr);
    netbuf = oal_netbuf_peek(p_netbuf_head);
    netbuf_num = (uint16_t)oal_netbuf_get_buf_num(p_netbuf_head);
    while (netbuf_num--) {
        if (netbuf == NULL) {
            oam_error_log0(0, OAM_SF_RX, "{hmac_rx_host_ring_ap_proc::netbuf null}");
            break;
        }

        netbuf_next = oal_netbuf_next(netbuf);
        rx_ctl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);
        if (OAL_TRUE == ether_is_multicast(hmac_rx_get_eth_da(netbuf))) {
            /* copy_frame_hdr复用03接口，06不用 */
            if (OAL_SUCC != hmac_rx_copy_netbuff(&netbuf_copy, netbuf, rx_ctl->uc_mac_vap_id, &copy_frame_hdr)) {
                oal_netbuf_delete(netbuf, p_netbuf_head);
                hmac_rx_free_netbuf(netbuf, (uint16_t)1);
                netbuf = netbuf_next;
                continue;
            }
            hmac_rx_prepare_to_wlan(&w2w_netbuf_hdr, netbuf_copy);
        } else if (OAL_TRUE == hmac_rx_da_is_in_same_bss(netbuf, rx_ctl, hmac_vap)) {
            /* check if da belong to the same bss. need check MAC_RX_CB_IS_DEST_CURR_BSS */
            oal_netbuf_delete(netbuf, p_netbuf_head);
            /* 目的用户已在AP的用户表中，进行WLAN_TO_WLAN转发 */
            hmac_rx_prepare_to_wlan(&w2w_netbuf_hdr, netbuf);
        } else {
            /* 该帧呆在链表里面 等待to lan */
            oam_info_log1(0, OAM_SF_RX, "{hmac_rx_host_ring_ap_proc::dst_is_valid[%d]}", rx_ctl->bit_dst_is_valid);
            oam_info_log4(0, OAM_SF_RX, "{hmac_rx_host_ring_ap_proc::band[%d] dst[%d] vap[%d]dst[%d]}",
                rx_ctl->bit_band_id, rx_ctl->bit_dst_band_id, rx_ctl->bit_vap_id, rx_ctl->dst_hal_vap_id);
        }
        netbuf = netbuf_next;
    }

    /*  将MSDU链表交给发送流程处理 */
    hmac_rx_wlan_to_wlan_proc(hmac_vap, &w2w_netbuf_hdr);
}

/* Ring中取出来的属于单个vap的帧的处理 */
uint32_t hmac_rx_vap_frame_proc(hmac_vap_stru *hmac_vap,
    oal_netbuf_head_stru *local_list,  oal_netbuf_head_stru *netbuf_head)
{
    oal_netbuf_stru *netbuf = NULL;
    if (oal_unlikely(oal_any_null_ptr3(hmac_vap, local_list, netbuf_head))) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* local list中的帧做转发 */
    if (hmac_vap->st_vap_base_info.en_vap_mode == WLAN_VAP_MODE_BSS_AP) {
        hmac_rx_host_ring_ap_proc(hmac_vap, local_list);
    }

    /* local_list非空，继续做W2L处理 */
    while (!oal_netbuf_list_empty(local_list)) {
        netbuf = oal_netbuf_delist(local_list);
        if (netbuf == NULL) {
            break;
        }
        oal_netbuf_add_to_list_tail(netbuf, netbuf_head);
    }
    return OAL_SUCC;
}

uint32_t hmac_rx_host_ring_reorder_proc(oal_netbuf_head_stru *netbuf_head, oal_netbuf_stru *netbuf)
{
    mac_rx_ctl_stru *rx_ctl = NULL;
    hmac_user_stru  *hmac_user = NULL;
    hmac_vap_stru   *hmac_vap = NULL;
    uint32_t         ret;
    hmac_ba_rx_stru *ba_rx_hdl = NULL;
    oal_netbuf_head_stru st_tmp_list;

    oal_netbuf_head_init(&st_tmp_list);
    rx_ctl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);
    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(rx_ctl->uc_mac_vap_id);
    if (hmac_vap == OAL_PTR_NULL) {
        oam_warning_log1(0, OAM_SF_RX, "{hmac_rx_data_reorder_proc::vap null![%d]}", rx_ctl->uc_mac_vap_id);
        hmac_rx_mpdu_to_netbuf_list(netbuf_head, netbuf);
        return OAL_FAIL;
    }

    if (OAL_SUCC != hmac_ba_check_rx_aggrate(hmac_vap, rx_ctl)) {
        hmac_rx_mpdu_to_netbuf_list(&st_tmp_list, netbuf);
        return hmac_rx_vap_frame_proc(hmac_vap, &st_tmp_list, netbuf_head);
    } else {
        /* tdo */
        hmac_rx_mpdu_to_netbuf_list(&st_tmp_list, netbuf);
        return hmac_rx_vap_frame_proc(hmac_vap, &st_tmp_list, netbuf_head);
    }

    hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(MAC_GET_RX_CB_TA_USER_IDX(rx_ctl));
    if (hmac_user == OAL_PTR_NULL) {
        oam_warning_log1(0, OAM_SF_RX, "{hmac_rx_data_reorder_proc::hmac_user[%d] null.}",
            MAC_GET_RX_CB_TA_USER_IDX(rx_ctl));
        hmac_rx_mpdu_to_netbuf_list(&st_tmp_list, netbuf);
        return hmac_rx_vap_frame_proc(hmac_vap, &st_tmp_list, netbuf_head);
    }

    ba_rx_hdl = hmac_user->ast_tid_info[MAC_GET_RX_CB_TID(rx_ctl)].pst_ba_rx_info;
    if (ba_rx_hdl == OAL_PTR_NULL) {
        hmac_rx_mpdu_to_netbuf_list(&st_tmp_list, netbuf);
        return hmac_rx_vap_frame_proc(hmac_vap, &st_tmp_list, netbuf_head);
    }

    hmac_ba_rx_hdl_hold(ba_rx_hdl);
    if (ba_rx_hdl->en_ba_status != DMAC_BA_COMPLETE) {
        hmac_ba_rx_hdl_put(ba_rx_hdl);
        hmac_rx_mpdu_to_netbuf_list(&st_tmp_list, netbuf);
        return hmac_rx_vap_frame_proc(hmac_vap, &st_tmp_list, netbuf_head);
    }

    ret = hmac_rx_data_wnd_proc(hmac_user, ba_rx_hdl, netbuf, netbuf_head);
    if (ret != OAL_SUCC) {
        hmac_ba_rx_hdl_put(ba_rx_hdl);
        return ret;
    }

    hmac_ba_rx_hdl_put(ba_rx_hdl);
    return OAL_SUCC;
}


/* 功能描述 : 检查上报帧的vap是否有效 */
static hmac_user_stru *hmac_hal_get_user(hal_host_device_stru *hal_device, mac_rx_ctl_stru *rx_ctl)
{
    hmac_user_stru *hmac_user = NULL;
    uint8_t  usr_lut;

    usr_lut = MAC_GET_RX_CB_TA_USER_IDX(rx_ctl);
    if (oal_unlikely(usr_lut >= HAL_MAX_LUT)) {
        oam_warning_log1(0, OAM_SF_RX, "{hmac_hal_get_user::invalid usr_lut id[%d].}", usr_lut);
        return NULL;
    }

    if (hal_device->user_sts_info[usr_lut].user_valid) {
        hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(hal_device->user_sts_info[usr_lut].user_id);
        if (oal_unlikely(hmac_user == NULL)) {
            oam_warning_log0(0, OAM_SF_RX, "{hmac_hal_get_user::not find valid user.}");
            return NULL;
        }
        MAC_GET_RX_CB_TA_USER_IDX(rx_ctl) = hmac_user->st_user_base_info.us_assoc_id;
    } else {
        oam_warning_log3(0, OAM_SF_RX, "{hmac_hal_get_user::hal dev[%d] invalid usr_lut[%d]. haldev[%x]}",
            hal_device->device_id, usr_lut, (uintptr_t)hal_device);
        return NULL;
    }

    return hmac_user;
}

static void hmac_rx_skb_refill(hal_host_device_stru *hal_dev)
{
    if (oal_atomic_read(&hal_dev->bh_need_refill)) {
        oal_atomic_set(&hal_dev->bh_need_refill, OAL_FALSE);
        /* 目前仅补充normal的free ring, small ring不使用 */
        hal_host_rx_add_buff(hal_dev, HAL_RX_DSCR_NORMAL_PRI_QUEUE);
    }
}

static oal_bool_enum_uint8 hmac_ap_rx_recheck_user_idx(hmac_vap_stru *hmac_vap,
    oal_netbuf_stru *netbuf, mac_rx_ctl_stru *rx_ctl)
{
    mac_ieee80211_frame_stru *frmhdr = NULL;
    uint8_t *transmit_addr = NULL;
    uint16_t user_idx = MAC_GET_RX_CB_TA_USER_IDX(rx_ctl);
    if (user_idx < g_wlan_spec_cfg->invalid_user_id) {
        return OAL_FALSE;
    }

    frmhdr = (mac_ieee80211_frame_stru *)oal_netbuf_header(netbuf);
    transmit_addr = frmhdr->auc_address2;
    if (mac_vap_find_user_by_macaddr(&(hmac_vap->st_vap_base_info), transmit_addr, &user_idx) == OAL_SUCC) {
        oam_error_log2(0, OAM_SF_RX, "{hmac_ap_rx_recheck_user_idx::user_idx [%d] change to [%d].}",
            MAC_GET_RX_CB_TA_USER_IDX(rx_ctl), user_idx);
        MAC_GET_RX_CB_TA_USER_IDX(rx_ctl) = user_idx;
        hmac_rx_report_eth_frame(&hmac_vap->st_vap_base_info, netbuf);
        return OAL_FALSE;
    } else {
        return OAL_TRUE;
    }
}

static oal_bool_enum_uint8 hmac_rx_eapol_while_sta_just_up(hmac_vap_stru *hmac_vap, oal_netbuf_stru *netbuf)
{
    mac_ether_header_stru *ether_hdr = (mac_ether_header_stru *)oal_netbuf_data(netbuf);

    if (hmac_vap->st_vap_base_info.en_vap_mode != WLAN_VAP_MODE_BSS_STA) {
        return OAL_FALSE;
    }

    if (hmac_vap->st_vap_base_info.en_vap_state != MAC_VAP_STATE_UP &&
        hmac_vap->st_vap_base_info.en_vap_state != MAC_VAP_STATE_STA_WAIT_ASOC) {
        oam_warning_log0(0, OAM_SF_RX, "{hmac_rx_ring_entry_proc:hmac_vap is not up}");
        return OAL_FALSE;
    }

    if (oal_byteorder_host_to_net_uint16(ETHER_TYPE_PAE) != ether_hdr->us_ether_type)  {
        return OAL_FALSE;
    }

    return OAL_TRUE;
}

static uint32_t hmac_rx_ring_entry_proc(hal_host_device_stru *hal_dev, oal_netbuf_stru *netbuf,
    oal_netbuf_head_stru *netbuf_head)
{
    hmac_vap_stru            *hmac_vap = NULL;
    hmac_user_stru           *hmac_user = NULL;
    mac_rx_ctl_stru          *rx_ctl = NULL;
    mac_ieee80211_frame_stru *frmhdr = NULL;

    rx_ctl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);
    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(rx_ctl->uc_mac_vap_id);
    if (hmac_vap == NULL) {
        return OAL_FAIL;
    }
    if (hmac_vap->st_vap_base_info.en_vap_mode == WLAN_VAP_MODE_BSS_AP) {
        if (hmac_ap_rx_recheck_user_idx(hmac_vap, netbuf, rx_ctl)) {
            return OAL_FAIL;
        }
    }

    hmac_user = hmac_hal_get_user(hal_dev, rx_ctl);
    if (hmac_user == NULL) {
        if (hmac_rx_eapol_while_sta_just_up(hmac_vap, netbuf)) {
            oam_warning_log0(MAC_RXCB_VAP_ID(rx_ctl), OAM_SF_RX,
                "{hmac_rx_ring_entry_proc::report EAPOL when vap just up}");
            return OAL_SUCC;
        } else {
            oam_error_log2(0, OAM_SF_RX, "{hmac_rx_ring_entry_proc:vap[%d],user[%d]}",
                MAC_RXCB_VAP_ID(rx_ctl), MAC_RXCB_TA_USR_ID(rx_ctl));
            return OAL_FAIL;
        }
    }

    if (MAC_RX_DSCR_NEED_DROP(MAC_RXCB_STATUS(rx_ctl)) ||
        hmac_rx_qos_data_need_drop(netbuf, hmac_vap, rx_ctl)) {
        return OAL_FAIL;
    }

    if (!MAC_TID_IS_VALID(MAC_GET_RX_CB_TID(rx_ctl))) {
        oam_error_log1(0, OAM_SF_RX, "{hmac_rx_ring_entry_proc::invalid tid[%d].}", MAC_GET_RX_CB_TID(rx_ctl));
        return OAL_FAIL;
    }

    /* 去分片，帧格式转换 */
    if (rx_ctl->bit_frame_format == MAC_FRAME_TYPE_80211) {
        frmhdr = (mac_ieee80211_frame_stru *)oal_netbuf_header(netbuf);
        if (OAL_SUCC != hmac_rx_wlan_frame_process(hmac_vap, netbuf, frmhdr)) {
            return OAL_FAIL;
        }
    }

    if (OAL_SUCC != hmac_rx_host_ring_reorder_proc(netbuf_head, netbuf)) {
        oam_warning_log0(0, OAM_SF_RX, "{hmac_rx_ring_entry_proc::filter_reorder fail.}");
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

/* 功能描述 : 检查上报帧的vap是否有效 */
static uint32_t hmac_is_hal_vap_valid(hal_host_device_stru *hal_device, mac_rx_ctl_stru *rx_ctl)
{
    uint8_t   hal_vap_id;

    if (MAC_RXCB_IS_AMSDU_SUB_MSDU(rx_ctl)) {
        return OAL_SUCC;
    }

    hal_vap_id = MAC_GET_RX_CB_HAL_VAP_IDX(rx_ctl);
    if (oal_unlikely(hal_vap_id >= HAL_MAX_VAP_NUM) && (hal_vap_id != WLAN_HAL_OHTER_BSS_ID)) {
        oam_error_log1(0, OAM_SF_RX, "{hmac_is_hal_vap_valid::invalid hal vap id[%d].}", hal_vap_id);
        return OAL_FAIL;
    }

    if (!hal_device->hal_vap_sts_info[hal_vap_id].hal_vap_valid) {
        oam_warning_log2(0, OAM_SF_RX, "{hmac_is_hal_vap_valid::haldev[%d] invalid hal vap[%d]. }",
            hal_device->device_id, hal_vap_id);
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

/* 功能描述 : 检查上报帧的tid是否有效 */
OAL_STATIC uint32_t hmac_is_tid_valid(hal_host_device_stru *hal_device, mac_rx_ctl_stru *rx_ctl)
{
    uint8_t frame_type;

    if (MAC_RXCB_IS_AMSDU_SUB_MSDU(rx_ctl)) {
        return OAL_SUCC;
    }

    /* 判断该帧是不是qos帧 */
    frame_type = mac_get_frame_type_and_subtype((uint8_t *)&rx_ctl->us_frame_control);
    if (frame_type != (WLAN_FC0_SUBTYPE_QOS | WLAN_FC0_TYPE_DATA)) {
        oam_warning_log1(0, OAM_SF_RX, "{hmac_is_tid_valid::non qos frame,fc[0x%x]}", rx_ctl->us_frame_control);
        return OAL_SUCC;
    }

    if (!MAC_TID_IS_VALID(MAC_GET_RX_CB_TID(rx_ctl))) {
        oam_error_log1(0, OAM_SF_RX, "{hmac_is_tid_valid::invalid tid[%d].}", MAC_GET_RX_CB_TID(rx_ctl));
        return OAL_FAIL;
    }
    return OAL_SUCC;
}

static void hmac_alrx_process(hal_host_device_stru *hal_device, mac_rx_ctl_stru *rx_ctl)
{
    if (rx_ctl->rx_status != HAL_RX_SUCCESS) {
        hal_device->st_alrx.rx_ppdu_fail_num++;
    } else if (rx_ctl->bit_is_ampdu == OAL_TRUE) {
        hal_device->st_alrx.rx_ampdu_succ_num++;
    } else {
        hal_device->st_alrx.rx_normal_mdpu_succ_num++;
    }
    return;
}

/* Rxcb中amsdu非首帧的信息填充 */
static inline void hmac_rx_update_submsdu_rxctl(mac_rx_ctl_stru *dst_rxcb, mac_rx_ctl_stru *src_rxcb)
{
    MAC_GET_RX_CB_TID(dst_rxcb) = MAC_GET_RX_CB_TID(src_rxcb);
    MAC_GET_RX_CB_HAL_VAP_IDX(dst_rxcb) = MAC_GET_RX_CB_HAL_VAP_IDX(src_rxcb);
    MAC_RXCB_IS_AMSDU(dst_rxcb) = MAC_RXCB_IS_AMSDU(src_rxcb);
}

/* Rxcb中扩展信息填充 */
static inline void hmac_rx_update_enpand_rxctl(hal_host_device_stru *hal_dev,
    mac_rx_ctl_stru *rxcb, oal_netbuf_stru *netbuf)
{
    rxcb->st_expand_cb.pul_mac_hdr_start_addr = (uint32_t *)oal_netbuf_data(netbuf);
    MAC_GET_RX_CB_MAC_VAP_ID(rxcb) = hal_dev->hal_vap_sts_info[MAC_GET_RX_CB_HAL_VAP_IDX(rxcb)].mac_vap_id;
}

/* 功能描述 : 1.把hal描述符转换成sw的描述符 */
static uint32_t hmac_rx_msdu_process(hal_host_device_stru *hal_dev, oal_netbuf_stru *netbuf)
{
    mac_rx_ctl_stru      *rxctl = NULL;
    oal_netbuf_stru      *cur_netbuf = netbuf;
    oal_netbuf_stru      *next_netbuf = NULL;
    uint32_t              error_flag = OAL_FALSE;
    uint32_t              amsdu_flag = OAL_FALSE;
    mac_rx_ctl_stru       st_rx_ctl;

    memset_s(&st_rx_ctl, sizeof(mac_rx_ctl_stru), 0, sizeof(mac_rx_ctl_stru));
    while (cur_netbuf != NULL) {
        next_netbuf = hal_rx_get_next_sub_msdu(hal_dev, cur_netbuf);
        oal_netbuf_next(cur_netbuf) = next_netbuf;

        rxctl = (mac_rx_ctl_stru *)oal_netbuf_cb(cur_netbuf);
        if (OAL_SUCC != hal_host_rx_get_msdu_info_dscr(cur_netbuf, rxctl)) {
            error_flag = OAL_TRUE;
            cur_netbuf = next_netbuf;
            continue;
        }

        if (hal_dev->st_alrx.en_al_rx_flag) {
            hmac_alrx_process(hal_dev, rxctl);
            cur_netbuf = next_netbuf;
            continue;
        }

        /* AMSDU子帧cb填写需要的信息 */
        if (MAC_RXCB_IS_FIRST_AMSDU(rxctl) == OAL_TRUE) {
            memcpy_s(&st_rx_ctl, sizeof(mac_rx_ctl_stru), rxctl, sizeof(mac_rx_ctl_stru));
            amsdu_flag = OAL_TRUE;
        } else if (amsdu_flag == OAL_TRUE) {
            /* 只有submsdu会走此分支 */
            hmac_rx_update_submsdu_rxctl(rxctl, &st_rx_ctl);
        }

        if ((OAL_SUCC != hmac_is_hal_vap_valid(hal_dev, rxctl)) ||
            (OAL_SUCC != hmac_is_tid_valid(hal_dev, rxctl))) {
            error_flag = OAL_TRUE;
            cur_netbuf = next_netbuf;
            continue;
        }

        hmac_rx_update_enpand_rxctl(hal_dev, rxctl, cur_netbuf);
        cur_netbuf = next_netbuf;
    }

    if (error_flag || hal_dev->st_alrx.en_al_rx_flag) {
        return OAL_FAIL;
    }
    return OAL_SUCC;
}


oal_netbuf_stru *hmac_host_rx_defrag(hal_host_device_stru *hal_dev, oal_netbuf_stru *netbuf)
{
    hmac_user_stru *hmac_user = NULL;
    mac_rx_ctl_stru *rx_ctl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);
    uint8_t  usr_lut;

    if ((rx_ctl->bit_amsdu_enable == OAL_TRUE) || (rx_ctl->bit_frame_format != MAC_FRAME_TYPE_80211)) {
        return netbuf;
    }

    usr_lut = MAC_GET_RX_CB_TA_USER_IDX(rx_ctl);
    if (oal_unlikely(usr_lut >= HAL_MAX_LUT)) {
        oam_error_log1(0, OAM_SF_RX, "{hmac_host_rx_defrag::invalid usr_lut id[%d].}", usr_lut);
        return netbuf;
    }

    if (hal_dev->user_sts_info[usr_lut].user_valid) {
        hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(hal_dev->user_sts_info[usr_lut].user_id);
        if (hmac_user == NULL) {
            oam_warning_log1(0, OAM_SF_RX, "{hmac_host_rx_defrag::usr_lut id[%d] null.}", usr_lut);
            return netbuf;
        }
    } else {
        oam_error_log1(0, OAM_SF_RX, "{hmac_host_rx_defrag::usr_lut id[%d] invalid.}", usr_lut);
        return netbuf;
    }

    return hmac_defrag_process(hmac_user, netbuf, rx_ctl->uc_mac_header_len);
}


static uint32_t hmac_rx_ring_data_process(hal_host_device_stru *hal_dev,
    oal_netbuf_head_stru *netbuf_head_orig, oal_netbuf_head_stru *netbuf_head)
{
    oal_netbuf_stru *netbuf = NULL;
    uint8_t          rx_status;

    if (oal_unlikely(oal_any_null_ptr3(hal_dev, netbuf_head_orig, netbuf_head))) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 可能是asmdu链,也可能是单个msdu */
    while (!oal_netbuf_list_empty(netbuf_head_orig)) {
        netbuf = oal_netbuf_delist(netbuf_head_orig);
        if (netbuf == NULL) {
            oam_error_log0(0, OAM_SF_RX, "hmac_rx_ring_data_process::netbuf null");
            break;
        }
        rx_status = hal_host_get_rx_msdu_status(netbuf);
        if ((rx_status == HAL_RX_FCS_ERROR) || (rx_status == HAL_RX_NEW)) {
            if (rx_status == HAL_RX_NEW) {
                oam_error_log1(0, OAM_SF_RX, "hmac_rx_ring_data_process::rx_status:[%d]", rx_status);
                oam_report_dscr(BROADCAST_MACADDR, (uint8_t *)oal_netbuf_data(netbuf), HAL_RX_DSCR_LEN,
                    OAM_OTA_TYPE_RX_DSCR_PILOT);
            }
            /* 由于此时MPDU描述符还没有串链，需要先串链，再释放内存 */
            hal_host_rx_amsdu_list_build(hal_dev, netbuf);
            hmac_rx_mpdu_free(netbuf);
            continue;
        }

        /* ring里面的数据需要逐一进行提取cb、帧校验等处理 */
        if (OAL_SUCC != hmac_rx_msdu_process(hal_dev, netbuf)) {
            hmac_rx_mpdu_free(netbuf);
            continue;
        }

        netbuf = hmac_host_rx_defrag(hal_dev, netbuf);
        if (netbuf == NULL) {
            continue;
        }

        /* 协议转换等处理动作 */
        if (OAL_SUCC != hmac_rx_ring_entry_proc(hal_dev, netbuf, netbuf_head)) {
            oam_warning_log0(0, OAM_SF_RX, "hmac_rx_ring_data_process::ring_entry_proc:fail");
            hmac_rx_mpdu_free(netbuf);
            continue;
        }
    }

    return OAL_SUCC;
}
#endif


uint32_t hmac_rx_host_ring_data_event(frw_event_mem_stru *event_mem)
{
#ifdef CONFIG_ARCH_KIRIN_PCIE
    hal_host_device_stru        *hal_device = NULL;
    hal_host_rx_event           *wlan_rx_event = NULL;
    oal_netbuf_head_stru         netbuf_head;
    oal_netbuf_head_stru         netbuf_head_ori;
    uint32_t                     ret;

    if (oal_unlikely(event_mem == NULL)) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    wlan_rx_event = (hal_host_rx_event *)frw_get_event_stru(event_mem)->auc_event_data;
    hal_device    = wlan_rx_event->hal_dev;

    oal_netbuf_list_head_init(&netbuf_head);
    oal_netbuf_list_head_init(&netbuf_head_ori);
#ifndef _PRE_LINUX_TEST /* tod */
    hal_host_rx_mpdu_que_pop(hal_device, &netbuf_head_ori);
#endif
    ret = hmac_rx_ring_data_process(hal_device, &netbuf_head_ori, &netbuf_head);
    if (ret != OAL_SUCC) {
        oam_error_log1(0, OAM_SF_RX, "{hmac_rx_host_ring_data_event::ret[%d].}", ret);
        hmac_rx_free_netbuf_list(&netbuf_head, oal_netbuf_list_len(&netbuf_head));
        return ret;
    }

    /* To wlan */
    if (!oal_netbuf_list_empty(&netbuf_head)) {
        hmac_rx_data_sta_proc(&netbuf_head);
    }

    /* skb_refill */
    hmac_rx_skb_refill(hal_device);
#endif
    return OAL_SUCC;
}


oal_netbuf_stru *hmac_device_rx_defrag(oal_netbuf_stru *netbuf)
{
    hmac_user_stru *hmac_user = NULL;
    mac_rx_ctl_stru *rx_ctl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);

    if ((rx_ctl->bit_amsdu_enable == OAL_TRUE) || (rx_ctl->bit_frame_format != MAC_FRAME_TYPE_80211)) {
        return netbuf;
    }

    hmac_user = (hmac_user_stru *)mac_res_get_hmac_user(MAC_GET_RX_CB_TA_USER_IDX(rx_ctl));
    if (oal_unlikely(hmac_user == NULL)) {
        oam_warning_log4(0, 0, "{hmac_device_rx_defrag:vap:%d machdr_len:%d,frm_len:%d machdr_addr:0x%08x}",
            rx_ctl->bit_vap_id, rx_ctl->uc_mac_header_len, rx_ctl->us_frame_len,
            (uintptr_t)MAC_GET_RX_CB_MAC_HEADER_ADDR(rx_ctl));
        return netbuf;
    }

    return hmac_defrag_process(hmac_user, netbuf, rx_ctl->uc_mac_header_len);
}


void hmac_device_rx_data_proc(hmac_vap_stru *hmac_vap, oal_netbuf_head_stru *netbuf_head)
{
    if (hmac_vap->st_vap_base_info.en_vap_mode == WLAN_VAP_MODE_BSS_STA) {
        hmac_rx_data_sta_proc(netbuf_head);
    } else if (hmac_vap->st_vap_base_info.en_vap_mode == WLAN_VAP_MODE_BSS_AP) {
        hmac_rx_data_ap_proc(hmac_vap, netbuf_head);
    } else {
        oam_warning_log1(0, 0, "{hmac_device_rx_data_proc:vap mode[%d]err}", hmac_vap->st_vap_base_info.en_vap_mode);
    }
}


uint32_t hmac_rx_process_ring_data_event(frw_event_mem_stru *event_mem)
{
    frw_event_stru           *event = NULL;
    dmac_wlan_drx_event_stru *wlan_rx_event = NULL;
    oal_netbuf_stru          *netbuf = NULL;
    uint16_t                  netbuf_num;
    hmac_vap_stru            *hmac_vap = NULL;
    oal_netbuf_head_stru      netbuf_head;

    if (oal_unlikely(event_mem == NULL)) {
        return OAL_ERR_CODE_PTR_NULL;
    }
    event = frw_get_event_stru(event_mem);
    wlan_rx_event = (dmac_wlan_drx_event_stru *)(event->auc_event_data);
    netbuf = wlan_rx_event->pst_netbuf;
    netbuf_num = wlan_rx_event->us_netbuf_num;
    if (oal_unlikely(netbuf == NULL)) {
        oam_error_log0(0, OAM_SF_RX, "{hmac_rx_process_ring_data_event::pst_netbuf null}");
        return OAL_SUCC;
    }
    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(event->st_event_hdr.uc_vap_id);
    if (hmac_vap == NULL) {
        oam_error_log0(event->st_event_hdr.uc_vap_id, OAM_SF_RX, "{hmac_rx_process_ring_data_event::hmac_vap null.}");
        hmac_rx_free_netbuf(netbuf, netbuf_num);
        return OAL_ERR_CODE_PTR_NULL;
    }
    if (netbuf_num != 1) {
        oam_warning_log1(event->st_event_hdr.uc_vap_id, 0, "hmac_rx_process_ring_data_event:netbuf_num=%d", netbuf_num);
        hmac_rx_free_netbuf(netbuf, netbuf_num);
        return OAL_SUCC;
    }
    /* 去分片 */
    netbuf = hmac_device_rx_defrag(netbuf);
    if (netbuf == NULL) {
        return OAL_SUCC;
    }
    /* 帧格式转换 */
    if (OAL_SUCC != hmac_rx_data_pre_proc(hmac_vap, netbuf)) {
        hmac_rx_free_netbuf(netbuf, netbuf_num);
        return OAL_SUCC;
    }
    oal_netbuf_list_head_init(&netbuf_head);
    /* 过滤及重排序处理 */
    if (OAL_SUCC != hmac_rx_data_reorder_proc(&netbuf_head, netbuf)) {
        oam_warning_log0(0, OAM_SF_RX, "{hmac_rx_process_ring_data_event::filter_reorder fail.}");
        return OAL_FAIL;
    }
    /* To kernel or to wlan */
    hmac_device_rx_data_proc(hmac_vap, &netbuf_head);
    return OAL_SUCC;
}


OAL_STATIC void hmac_rx_process_data_tcp_ack_opt(hmac_vap_stru *hmac_vap, oal_netbuf_head_stru *netbuf_header)
{
    if (hmac_vap->st_vap_base_info.en_vap_mode == WLAN_VAP_MODE_BSS_STA) {
        hmac_rx_process_data_sta_tcp_ack_opt(hmac_vap, netbuf_header);
    } else if (hmac_vap->st_vap_base_info.en_vap_mode == WLAN_VAP_MODE_BSS_AP) {
        hmac_rx_process_data_ap_tcp_ack_opt(hmac_vap, netbuf_header);
    } else {
        oam_warning_log1(0, OAM_SF_RX, "{hmac_rx_process_data_event::RX FAIL! VAP MODE[%d]!.}",
                         hmac_vap->st_vap_base_info.en_vap_mode);
    }
}
#ifdef _PRE_WLAN_FEATURE_MONITOR
uint32_t hmac_monitor_report_frame_to_sdt(mac_rx_ctl_stru *pst_cb_ctrl, oal_netbuf_stru *pst_curr_netbuf)
{
    oal_uint8 *puc_payload;
    mac_ieee80211_frame_stru *pst_frame_hdr;
    oal_uint32 ul_rslt;

    puc_payload = MAC_GET_RX_PAYLOAD_ADDR(pst_cb_ctrl, pst_curr_netbuf);
    pst_frame_hdr = (mac_ieee80211_frame_stru *)mac_get_rx_cb_mac_hdr(pst_cb_ctrl);

    ul_rslt = oam_report_80211_frame(BROADCAST_MACADDR, (oal_uint8 *)(pst_frame_hdr),
        MAC_GET_RX_CB_MAC_HEADER_LEN(pst_cb_ctrl), puc_payload,
        pst_cb_ctrl->us_frame_len, OAM_OTA_FRAME_DIRECTION_TYPE_RX);
    return ul_rslt;
}
#endif

#ifdef _PRE_WLAN_FEATURE_MONITOR
OAL_STATIC uint32_t hmac_rx_process_data_event_monitor_handle(hmac_device_stru *pst_hmac_device,
    oal_netbuf_stru *pst_netbuf, uint16_t us_netbuf_num)
{
    mac_rx_ctl_stru *pst_rx_ctrl = OAL_PTR_NULL;

    pst_hmac_device->ul_data_frames_cnt += us_netbuf_num;
    pst_rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb(pst_netbuf);
    if (oal_value_eq_any2(pst_hmac_device->pst_device_base_info->uc_monitor_ota_mode,
        WLAN_MONITOR_OTA_HOST_RPT, WLAN_MONITOR_OTA_ALL_RPT)) {
        hmac_monitor_report_frame_to_sdt(pst_rx_ctrl, pst_netbuf);
    }
    hmac_sniffer_save_data(pst_hmac_device, pst_netbuf, us_netbuf_num);
    hmac_rx_free_netbuf(pst_netbuf, us_netbuf_num);
    return OAL_SUCC;
}
#endif


uint32_t hmac_rx_process_data_event(frw_event_mem_stru *pst_event_mem)
{
    frw_event_stru *pst_event = OAL_PTR_NULL;
    frw_event_hdr_stru *pst_event_hdr = OAL_PTR_NULL;
    dmac_wlan_drx_event_stru *pst_wlan_rx_event = OAL_PTR_NULL;
    oal_netbuf_stru *pst_netbuf = OAL_PTR_NULL; /* 用于临时保存下一个需要处理的netbuf指针 */
    uint16_t us_netbuf_num;                   /* netbuf链表的个数 */
    oal_netbuf_head_stru st_netbuf_header;      /* 存储上报给网络层的数据 */
    oal_netbuf_stru *pst_temp_netbuf = OAL_PTR_NULL;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
#ifdef _PRE_WLAN_TCP_OPT
    hmac_device_stru *pst_hmac_device = OAL_PTR_NULL;
#endif

    if (oal_unlikely(pst_event_mem == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_RX, "{hmac_rx_process_data_event::pst_event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

#ifdef _PRE_WLAN_RR_PERFORMENCE_DEBUG
    /* RR性能检测接收流程dmac       to hmac位置打点 */
    hmac_rr_rx_d2h_timestamp();
#endif

    /* 获取事件头和事件结构体指针 */
    pst_event = frw_get_event_stru(pst_event_mem);
    pst_event_hdr = &(pst_event->st_event_hdr);
    pst_wlan_rx_event = (dmac_wlan_drx_event_stru *)(pst_event->auc_event_data);
    pst_netbuf = pst_wlan_rx_event->pst_netbuf;
    us_netbuf_num = pst_wlan_rx_event->us_netbuf_num;

    if (oal_unlikely(pst_netbuf == OAL_PTR_NULL)) {
        oam_error_log1(0, OAM_SF_RX, "{hmac_rx_process_data_event::us_netbuf_num = %d.}", us_netbuf_num);
        return OAL_SUCC; /* 这个是事件处理函数，为了防止51的UT挂掉 返回 true */
    }

#if defined(_PRE_WLAN_TCP_OPT) || defined(_PRE_WLAN_FEATURE_MONITOR)
    pst_hmac_device = hmac_res_get_mac_dev(pst_event_hdr->uc_device_id);
    if (pst_hmac_device == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{hmac_rx_process_data_event::pst_hmac_device null.}");
        hmac_rx_free_netbuf(pst_netbuf, us_netbuf_num);
        return OAL_ERR_CODE_PTR_NULL;
    }
#endif

#ifdef _PRE_WLAN_FEATURE_MONITOR
    if (pst_hmac_device->en_monitor_mode == OAL_TRUE) {
        return hmac_rx_process_data_event_monitor_handle(pst_hmac_device, pst_netbuf, us_netbuf_num);
    }
#endif

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_event_hdr->uc_vap_id);
    if (pst_hmac_vap == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_RX, "{hmac_rx_process_data_event::pst_hmac_vap null.}");
        hmac_rx_free_netbuf(pst_netbuf, us_netbuf_num);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* If mib info is null ptr,release the netbuf */
    if (pst_hmac_vap->st_vap_base_info.pst_mib_info == OAL_PTR_NULL) {
        oam_warning_log0(0, OAM_SF_ANY, "{hmac_rx_process_data_event::pst_mib_info null.}");
        hmac_rx_free_netbuf(pst_netbuf, us_netbuf_num);
        return OAL_SUCC;
    }

    /* 统计收包的总数，计入全局变量 */
    hmac_auto_freq_wifi_rx_stat(us_netbuf_num);

    /* 将所有netbuff全部入链表 */
    oal_netbuf_list_head_init(&st_netbuf_header);
    while (us_netbuf_num != 0) {
        pst_temp_netbuf = pst_netbuf;
        if (pst_temp_netbuf == OAL_PTR_NULL) {
            break;
        }

        pst_netbuf = oal_netbuf_next(pst_temp_netbuf);

        hmac_auto_freq_wifi_rx_bytes_stat(oal_netbuf_len(pst_temp_netbuf));
        oal_netbuf_list_tail_nolock(&st_netbuf_header, pst_temp_netbuf);
        us_netbuf_num--;
    }

    if (us_netbuf_num != 0) {
        oam_error_log2(0, OAM_SF_RX, "{hmac_rx_process_data_event::us_netbuf_num[%d], event_buf_num[%d].}",
                       us_netbuf_num, pst_wlan_rx_event->us_netbuf_num);
    }

    hmac_rx_process_data_filter(&st_netbuf_header, pst_wlan_rx_event->pst_netbuf, pst_wlan_rx_event->us_netbuf_num);

#ifdef _PRE_WLAN_TCP_OPT
    if (pst_hmac_device->sys_tcp_rx_ack_opt_enable == OAL_TRUE) {
        hmac_transfer_rx_handle(pst_hmac_device, pst_hmac_vap, &st_netbuf_header);
    }
#endif

    hmac_rx_process_data_tcp_ack_opt(pst_hmac_vap, &st_netbuf_header);

    return OAL_SUCC;
}

#ifdef _PRE_WLAN_FEATURE_SPECIAL_PKT_LOG

OAL_STATIC const uint8_t *hmac_dhcp_get_option(const uint8_t *puc_opt_buff,
                                               uint32_t ul_opt_total_len,
                                               uint8_t uc_opt,
                                               uint32_t *pul_option_len,
                                               uint32_t ul_expect_len)
{
    const uint8_t *puc_buff = puc_opt_buff;
    const uint8_t *puc_buff_end = puc_buff + ul_opt_total_len;
    uint8_t uc_opt_len = 0;
    uint8_t uc_len;
    uint8_t uc_opt_type;
    const uint8_t *puc_opt_val = OAL_PTR_NULL;

    /* DHCP Options are in TLV format with T and L each being a single
     * byte.We must make sure there is enough room to read both T and L.
     */
    while (puc_buff + 1 < puc_buff_end) {
        uc_opt_type = *puc_buff;
        puc_buff++;  // point to length

        if (uc_opt_type == uc_opt) {
            puc_opt_val = puc_buff + 1;
            uc_opt_len += oal_min(*puc_buff, puc_buff_end - puc_opt_val);
        }
        switch (uc_opt_type) {
            case DHO_PAD:
                continue;
            case DHO_END:
                break;
            default:
                break;
        }

        uc_len = *puc_buff++;
        puc_buff += uc_len;  // skip value, now point to type
    }

    if (ul_expect_len > 0 && uc_opt_len != ul_expect_len) {
        return OAL_PTR_NULL;  // unexpect length of value
    }

    if (pul_option_len) {
        *pul_option_len = uc_opt_len;
    }

    return puc_opt_val;
}

static int32_t hmac_dhcp_get_option_uint32(uint32_t *pul_option_val, const uint8_t *puc_opt_buff,
                                           uint32_t ul_opt_len, uint8_t uc_option)
{
    const uint8_t *puc_val = hmac_dhcp_get_option(puc_opt_buff, ul_opt_len, uc_option,
                                                  OAL_PTR_NULL, OAL_SIZEOF(uint32_t));
    uint32_t ul_val;

    if (puc_val == OAL_PTR_NULL) {
        return OAL_FAIL;
    }
    if (EOK != memcpy_s(&ul_val, OAL_SIZEOF(ul_val), puc_val, OAL_SIZEOF(ul_val))) {
        oam_error_log0(0, OAM_SF_ANY, "hmac_dhcp_get_option_uint32::memcpy fail!");
        return OAL_FAIL;
    }
    if (pul_option_val) {
        *pul_option_val = oal_ntoh_32(ul_val);
    }
    return OAL_SUCC;
}

static int32_t hmac_dhcp_get_option_uint8(uint8_t *puc_option_val, const uint8_t *puc_opt_buff,
                                          uint32_t ul_opt_len, uint8_t uc_option)
{
    const uint8_t *puc_val = hmac_dhcp_get_option(puc_opt_buff, ul_opt_len, uc_option,
                                                  OAL_PTR_NULL, OAL_SIZEOF(uint8_t));

    if (puc_val == OAL_PTR_NULL) {
        return OAL_FAIL;
    }

    if (puc_option_val) {
        *puc_option_val = *(puc_val);
    }
    return OAL_SUCC;
}


OAL_STATIC void hmac_parse_special_dhcp_packet(uint8_t *puc_buff, uint32_t ul_buflen, uint8_t *puc_dst)
{
    uint8_t uc_type = 0;
    uint32_t ul_req_ip = 0;
    uint32_t ul_req_srv = 0;
    uint32_t ul_len;
    oal_dhcp_packet_stru *pst_msg;
    if (ul_buflen <= OAL_SIZEOF(oal_dhcp_packet_stru)) {
        oal_io_print(WIFI_SEPCIAL_IPV4_PKT_TAG "invalid dhcp packet\n");
        return;
    }
    pst_msg = (oal_dhcp_packet_stru *)puc_buff;
    ul_len = ul_buflen - OAL_SIZEOF(oal_dhcp_packet_stru);
    if (hmac_dhcp_get_option_uint8(&uc_type, &pst_msg->options[4], ul_len, DHO_MESSAGETYPE) == OAL_FAIL) {
        oal_io_print(WIFI_SEPCIAL_IPV4_PKT_TAG "get message type failed\n");
        return;
    }

    if (uc_type == DHCP_DISCOVER) {
        oal_io_print(WIFI_SEPCIAL_IPV4_PKT_TAG "type: DHCP_DISCOVER\n");
    } else if (uc_type == DHCP_OFFER) {
        oal_io_print(WIFI_SEPCIAL_IPV4_PKT_TAG "type: DHCP_OFFER, ip:%d.x.x.%d srvip:%d.x.x.%d MAC:" HWMACSTR "\n",
                     IPADDR(pst_msg->yiaddr), IPADDR(pst_msg->siaddr), HWMAC2STR(puc_dst));
    } else if (uc_type == DHCP_REQUEST) {
        hmac_dhcp_get_option_uint32(&ul_req_ip, &pst_msg->options[4], ul_len, DHO_IPADDRESS);
        hmac_dhcp_get_option_uint32(&ul_req_srv, &pst_msg->options[4], ul_len, DHO_SERVERID);
        ul_req_ip = oal_ntoh_32(ul_req_ip);
        ul_req_srv = oal_ntoh_32(ul_req_srv);
        oal_io_print(WIFI_SEPCIAL_IPV4_PKT_TAG "type: DHCP_REQUEST, ip:%d.x.x.%d srvip:%d.x.x.%d\n",
                     IPADDR(pst_msg->yiaddr), IPADDR(pst_msg->siaddr));
    } else if (uc_type == DHCP_ACK) {
        oal_io_print(WIFI_SEPCIAL_IPV4_PKT_TAG "type: DHCP_ACK MAC:" HWMACSTR "\n", HWMAC2STR(puc_dst));
    } else if (uc_type == DHCP_NAK) {
        oal_io_print(WIFI_SEPCIAL_IPV4_PKT_TAG "type: DHCP_NAK MAC:" HWMACSTR "\n", HWMAC2STR(puc_dst));
    }
}


OAL_STATIC void hmac_parse_dns_query(uint8_t *puc_msg, uint32_t ul_msg_len)
{
    uint8_t *puc_buff = puc_msg;
    uint8_t *puc_end = puc_msg + ul_msg_len;
    uint8_t uc_qlen;
    uint8_t auc_domain[DNS_MAX_DOMAIN_LEN + 2] = { 0 };
    uint8_t *puc_domain = auc_domain;
    uint8_t *puc_domain_end = auc_domain + DNS_MAX_DOMAIN_LEN;

    while (puc_buff < puc_end) {
        uc_qlen = *puc_buff++;
        if (uc_qlen > 0) {
            if ((puc_buff + uc_qlen < puc_end) && (puc_domain + uc_qlen <= puc_domain_end)) {
                if (EOK != memcpy_s(puc_domain, uc_qlen, puc_buff, uc_qlen)) {
                    oam_error_log0(0, OAM_SF_ANY, "hmac_parse_dns_query::memcpy fail!");
                    return;
                }
                puc_domain += uc_qlen;
                *puc_domain = '.';
                *(puc_domain + 1) = '\0';
                puc_domain += 1;
            }
            puc_buff += uc_qlen;
        } else {
            // only printf one record of query item
            break;
        }
    }
    oal_io_print(WIFI_SEPCIAL_IPV4_PKT_TAG "domain name: %s \n", auc_domain);
}


OAL_STATIC void hmac_parse_dns_answer(uint8_t *puc_msg, uint16_t us_msg_len, uint16_t us_qdcount)
{
    uint8_t *puc_buff = puc_msg;
    uint8_t *puc_end = puc_msg + us_msg_len;
    uint8_t uc_qlen;
    uint16_t us_type = 0;
    uint16_t us_rdlen = 0;
    uint32_t ul_ipv4 = 0;
    uint8_t auc_domain[DNS_MAX_DOMAIN_LEN + 2] = { 0 };
    uint8_t auc_domain_s[DNS_MAX_DOMAIN_LEN + 2] = { 0 };
    uint8_t *puc_domain = auc_domain;
    uint8_t *puc_domain_end = auc_domain + DNS_MAX_DOMAIN_LEN;
    int32_t l_ret = EOK;
    // skip Questions
    while (us_qdcount > 0 && puc_buff < puc_end) {
        uc_qlen = *puc_buff++;
        if (uc_qlen > 0) {
            if ((puc_buff + uc_qlen < puc_end) && (puc_domain + uc_qlen <= puc_domain_end)) {
                l_ret += memcpy_s(puc_domain, uc_qlen, puc_buff, uc_qlen);
                puc_domain += uc_qlen;
                *puc_domain = '.';
                *(puc_domain + 1) = '\0';
                puc_domain += 1;
            }
            puc_buff += uc_qlen;
        } else {
            puc_buff += 4;  // class: 2 bytes, type: 2 bytes
            us_qdcount--;
            l_ret += memcpy_s(auc_domain_s, DNS_MAX_DOMAIN_LEN + 2, auc_domain, DNS_MAX_DOMAIN_LEN + 2);
            puc_domain = auc_domain;
            puc_domain_end = auc_domain + DNS_MAX_DOMAIN_LEN;
            memset_s(auc_domain, OAL_SIZEOF(auc_domain), 0, OAL_SIZEOF(auc_domain));
        }
    }

    // parse Answers
    while (puc_buff + 12 < puc_end) {
        puc_buff += 2;  // name: 2 bytes;
        l_ret += memcpy_s(&us_type, OAL_SIZEOF(us_type), puc_buff, OAL_SIZEOF(us_type));
        puc_buff += 8;  // type, class: 2bytes, ttl: 4bytes;
        l_ret += memcpy_s(&us_rdlen, OAL_SIZEOF(us_rdlen), puc_buff, OAL_SIZEOF(us_rdlen));
        puc_buff += 2;
        us_type = oal_ntoh_16(us_type);
        us_rdlen = oal_ntoh_16(us_rdlen);
        if (us_type == OAL_NS_T_A && us_rdlen == 4) {
            l_ret += memcpy_s(&ul_ipv4, us_rdlen, puc_buff, us_rdlen);
            oal_io_print(WIFI_SEPCIAL_IPV4_PKT_TAG "domain name: %s %d.x.x.%d\n", auc_domain_s, IPADDR(ul_ipv4));
            return;
        }
        puc_buff += us_rdlen;
    }
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "hmac_parse_dns_answer::memcpy fail!");
        return;
    }

    oal_io_print(WIFI_SEPCIAL_IPV4_PKT_TAG "domain name: %s\n", auc_domain_s);
}


OAL_STATIC void hmac_parse_special_dns_packet(uint8_t *puc_msg, uint32_t ul_msg_len)
{
    uint8_t uc_qr, uc_opcode, uc_rcode;
    uint16_t us_flag, us_qdcount;
    uint32_t ul_dns_hdr_len = OAL_SIZEOF(oal_dns_hdr_stru);
    oal_dns_hdr_stru *pst_dns_hdr = NULL;

    if (ul_dns_hdr_len >= ul_msg_len) {
        oal_io_print(WIFI_SEPCIAL_IPV4_PKT_TAG "invalid dns packet\n");
        return;
    }

    pst_dns_hdr = (oal_dns_hdr_stru *)puc_msg;
    us_flag = oal_ntoh_16(pst_dns_hdr->flags);
    us_qdcount = oal_ntoh_16(pst_dns_hdr->qdcount);
    uc_qr = dns_get_qr_from_flag(us_flag);
    uc_opcode = dns_get_opcode_from_flag(us_flag);
    uc_rcode = dns_get_rcode_from_flag(us_flag);

    if (uc_qr == OAL_NS_Q_REQUEST) {
        if (uc_opcode == OAL_NS_O_QUERY && us_qdcount > 0) {
            hmac_parse_dns_query(puc_msg + ul_dns_hdr_len, ul_msg_len - ul_dns_hdr_len);
        } else {
            oal_io_print(WIFI_SEPCIAL_IPV4_PKT_TAG "dont parse dns request pkt, opcode: %u, qd: %u\n",
                         uc_opcode, us_qdcount);
        }
    } else if (uc_qr == OAL_NS_Q_RESPONSE) {
        if (uc_opcode == OAL_NS_O_QUERY && uc_rcode == OAL_NS_R_NOERROR) {
            hmac_parse_dns_answer(puc_msg + ul_dns_hdr_len, ul_msg_len - ul_dns_hdr_len, us_qdcount);
        } else {
            oal_io_print(WIFI_SEPCIAL_IPV4_PKT_TAG "dont parse dns response pkt, opcode: %u, rcode: %u\n",
                         uc_opcode, uc_rcode);
        }
    }
}


void hmac_parse_special_ipv4_packet(uint8_t *puc_pktdata, uint32_t ul_datalen,
                                    hmac_pkt_direction_enum en_pkt_direction)
{
    uint32_t ul_iphdr_len;
    const struct iphdr *pst_iph;
    struct udphdr *pst_uh;
    uint16_t us_src_port;
    uint16_t us_dst_port;
    uint16_t us_udp_len;
    uint16_t us_tot_len;
    uint16_t us_udphdr_len;
    uint8_t *puc_uplayer_data;

    // invalid ipv4 packet
    if ((puc_pktdata == NULL) || (ul_datalen <= ETH_HLEN + 20)) {
        return;
    }
    pst_iph = (struct iphdr *)(puc_pktdata + ETH_HLEN);
    ul_iphdr_len = pst_iph->ihl * 4;
    us_tot_len = oal_ntoh_16(pst_iph->tot_len);
    // invalid ipv4 packet
    if (ul_datalen < (us_tot_len + ETH_HLEN)) {
        return;
    }

    if (pst_iph->protocol == IPPROTO_UDP) {
        pst_uh = (struct udphdr *)(puc_pktdata + ETH_HLEN + ul_iphdr_len);
        us_src_port = oal_ntoh_16(pst_uh->source);
        us_dst_port = oal_ntoh_16(pst_uh->dest);
        us_udp_len = oal_ntoh_16(pst_uh->len);
        us_udphdr_len = (uint16_t)OAL_SIZEOF(struct udphdr);
        // invalid udp packet
        if (us_udp_len <= us_udphdr_len) {
            return;
        }

        us_udp_len -= us_udphdr_len; // skip udp header
        puc_uplayer_data = (uint8_t *)(puc_pktdata + ETH_HLEN + ul_iphdr_len + us_udphdr_len);  // skip udp header
        if (us_src_port == DNS_SERVER_PORT || us_dst_port == DNS_SERVER_PORT) {
            oal_io_print(WIFI_SEPCIAL_IPV4_PKT_TAG "%s parse dns packet\n", GET_PKT_DIRECTION_STR(en_pkt_direction));
            hmac_parse_special_dns_packet(puc_uplayer_data, us_udp_len);
        } else if ((us_src_port == DHCP_SERVER_PORT && us_dst_port == DHCP_CLIENT_PORT) ||
                   (us_dst_port == DHCP_SERVER_PORT && us_src_port == DHCP_CLIENT_PORT)) {
            oal_io_print(WIFI_SEPCIAL_IPV4_PKT_TAG "%s parse dhcp packet\n", GET_PKT_DIRECTION_STR(en_pkt_direction));
            hmac_parse_special_dhcp_packet(puc_uplayer_data, us_udp_len, puc_pktdata + ETH_ALEN);
        }
    }
}
#endif
