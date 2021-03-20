

#include "hmac_hcc_adapt.h"
#include "mac_resource.h"
#include "oal_hcc_host_if.h"
#include "frw_event_main.h"
#include "hmac_vap.h"

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include "plat_pm_wlan.h"
#endif

#include "securec.h"
#include "securectype.h"


#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_HCC_ADAPT_C

OAL_STATIC uint8_t g_hcc_sched_stat[FRW_EVENT_TYPE_BUTT];
OAL_STATIC uint8_t g_hcc_flowctrl_stat[FRW_EVENT_TYPE_BUTT];
OAL_STATIC uint32_t g_hcc_sched_event_pkts[FRW_EVENT_TYPE_BUTT] = { 0 };
OAL_STATIC uint8_t g_wlan_queue_to_dmac_queue[WLAN_NET_QUEUE_BUTT];

extern uint32_t hmac_hcc_tx_netbuf(frw_event_mem_stru *pst_hcc_event_mem,
                                   oal_netbuf_stru *pst_netbuf, uint32_t ul_hdr_len,
                                   uint32_t fc_type,
                                   uint32_t queue_id);
uint32_t hmac_hcc_tx_netbuf_auto(frw_event_mem_stru *pst_hcc_event_mem,
                                 oal_netbuf_stru *pst_netbuf, uint32_t ul_hdr_len);
extern uint32_t hmac_hcc_tx_data(frw_event_mem_stru *pst_hcc_event_mem, oal_netbuf_stru *pst_netbuf);

/* 3 ����ʵ�� */
void hmac_tx_net_queue_map_init(void)
{
    memset_s(g_wlan_queue_to_dmac_queue, OAL_SIZEOF(g_wlan_queue_to_dmac_queue),
             DATA_LO_QUEUE, OAL_SIZEOF(g_wlan_queue_to_dmac_queue));
    g_wlan_queue_to_dmac_queue[WLAN_HI_QUEUE] = DATA_HI_QUEUE;
    g_wlan_queue_to_dmac_queue[WLAN_NORMAL_QUEUE] = DATA_LO_QUEUE;
    g_wlan_queue_to_dmac_queue[WLAN_TCP_DATA_QUEUE] = DATA_TCP_DATA_QUEUE;
    g_wlan_queue_to_dmac_queue[WLAN_TCP_ACK_QUEUE] = DATA_TCP_ACK_QUEUE;
    g_wlan_queue_to_dmac_queue[WLAN_UDP_BK_QUEUE] = DATA_UDP_BK_QUEUE;
    g_wlan_queue_to_dmac_queue[WLAN_UDP_BE_QUEUE] = DATA_UDP_BE_QUEUE;
    g_wlan_queue_to_dmac_queue[WLAN_UDP_VI_QUEUE] = DATA_UDP_VI_QUEUE;
    g_wlan_queue_to_dmac_queue[WLAN_UDP_VO_QUEUE] = DATA_UDP_VO_QUEUE;

    hcc_tx_wlan_queue_map_set(hcc_get_110x_handler(), DATA_HI_QUEUE, WLAN_HI_QUEUE);
    hcc_tx_wlan_queue_map_set(hcc_get_110x_handler(), DATA_LO_QUEUE, WLAN_NORMAL_QUEUE);
    hcc_tx_wlan_queue_map_set(hcc_get_110x_handler(), DATA_TCP_DATA_QUEUE, WLAN_TCP_DATA_QUEUE);
    hcc_tx_wlan_queue_map_set(hcc_get_110x_handler(), DATA_TCP_ACK_QUEUE, WLAN_TCP_ACK_QUEUE);
    hcc_tx_wlan_queue_map_set(hcc_get_110x_handler(), DATA_UDP_BK_QUEUE, WLAN_UDP_BK_QUEUE);
    hcc_tx_wlan_queue_map_set(hcc_get_110x_handler(), DATA_UDP_BE_QUEUE, WLAN_UDP_BE_QUEUE);
    hcc_tx_wlan_queue_map_set(hcc_get_110x_handler(), DATA_UDP_VI_QUEUE, WLAN_UDP_VI_QUEUE);
    hcc_tx_wlan_queue_map_set(hcc_get_110x_handler(), DATA_UDP_VO_QUEUE, WLAN_UDP_VO_QUEUE);
}

#ifdef _PRE_CONFIG_CONN_HISI_SYSFS_SUPPORT
int32_t hmac_tx_event_pkts_info_print(void *data, char *buf, int32_t buf_len)
{
    int i;
    int32_t ret = 0;
    uint64_t total = 0;
    struct hcc_handler *hcc = hcc_get_110x_handler();
    if (hcc == NULL) {
        return ret;
    }
    ret += snprintf_s(buf + ret, buf_len - ret, (buf_len - ret) - 1, "tx_event_pkts_info_show\n");
    for (i = 0; i < FRW_EVENT_TYPE_BUTT; i++) {
        if (g_hcc_sched_event_pkts[i]) {
            ret += snprintf_s(buf + ret, buf_len - ret, buf_len - ret - 1,
                              "event:%d, pkts:%10u\n", i, g_hcc_sched_event_pkts[i]);
        }
        total += g_hcc_sched_event_pkts[i];
    }

    if (total) {
        ret += snprintf_s(buf + ret, buf_len - ret, (buf_len - ret) - 1, "total:%llu\n", total);
    }

    return ret;
}
#endif

#ifdef _PRE_CONFIG_HISI_PANIC_DUMP_SUPPORT
OAL_STATIC declare_wifi_panic_stru(hmac_panic_hcc_adapt, hmac_tx_event_pkts_info_print);
#endif

void hmac_tx_sched_info_init(void)
{
    memset_s(g_hcc_sched_stat, OAL_SIZEOF(g_hcc_sched_stat),
             DATA_LO_QUEUE, OAL_SIZEOF(g_hcc_sched_stat));

    /* set the event sched PRI, TBD */
    g_hcc_sched_stat[FRW_EVENT_TYPE_HIGH_PRIO] = DATA_HI_QUEUE;
    g_hcc_sched_stat[FRW_EVENT_TYPE_HOST_CRX] = DATA_HI_QUEUE;
    g_hcc_sched_stat[FRW_EVENT_TYPE_HOST_DRX] = DATA_LO_QUEUE;
    g_hcc_sched_stat[FRW_EVENT_TYPE_HOST_CTX] = DATA_HI_QUEUE;
    g_hcc_sched_stat[FRW_EVENT_TYPE_HOST_SDT_REG] = DATA_HI_QUEUE;
    g_hcc_sched_stat[FRW_EVENT_TYPE_WLAN_CRX] = DATA_HI_QUEUE;
    g_hcc_sched_stat[FRW_EVENT_TYPE_WLAN_DRX] = DATA_LO_QUEUE;
    g_hcc_sched_stat[FRW_EVENT_TYPE_WLAN_CTX] = DATA_HI_QUEUE;
    g_hcc_sched_stat[FRW_EVENT_TYPE_WLAN_TX_COMP] = DATA_HI_QUEUE;
    g_hcc_sched_stat[FRW_EVENT_TYPE_TBTT] = DATA_HI_QUEUE;
    g_hcc_sched_stat[FRW_EVENT_TYPE_TIMEOUT] = DATA_HI_QUEUE;
    g_hcc_sched_stat[FRW_EVENT_TYPE_DMAC_MISC] = DATA_HI_QUEUE;

    memset_s(g_hcc_flowctrl_stat, OAL_SIZEOF(g_hcc_flowctrl_stat),
             HCC_FC_NONE, OAL_SIZEOF(g_hcc_flowctrl_stat));
    /* ����HOST���¼��������Kernel Net����ѡ�����������+�����ķ�ʽ��
    �����Wlan To Wlan �ķ�ʽ��ֱ�Ӷ���! */
    g_hcc_flowctrl_stat[FRW_EVENT_TYPE_HOST_DRX] = HCC_FC_DROP | HCC_FC_NET;
}

OAL_STATIC OAL_INLINE void hmac_hcc_adapt_extend_hdr_init(frw_event_hdr_stru *pst_event_hdr,
                                                          oal_netbuf_stru *pst_netbuf)
{
    struct frw_hcc_extend_hdr *pst_hdr;

    pst_hdr = (struct frw_hcc_extend_hdr *)oal_netbuf_data(pst_netbuf);
    pst_hdr->en_nest_type = pst_event_hdr->en_type;
    pst_hdr->uc_nest_sub_type = pst_event_hdr->uc_sub_type;
    pst_hdr->device_id = pst_event_hdr->uc_device_id;
    pst_hdr->chip_id = pst_event_hdr->uc_chip_id;
    pst_hdr->vap_id = pst_event_hdr->uc_vap_id;
}


uint32_t check_headroom_add_length(mac_tx_ctl_stru *pst_tx_ctl,
                                   frw_event_type_enum_uint8 en_nest_type,
                                   uint8_t uc_nest_sub_type, uint8_t uc_cb_length)
{
    uint32_t ul_headroom_add;

    if (MAC_GET_CB_80211_MAC_HEAD_TYPE(pst_tx_ctl) == 1) {
        /* case 1: data from net, mac head is maintence in netbuff */
        /*lint -e778*/
        ul_headroom_add = uc_cb_length + MAC_80211_QOS_HTC_4ADDR_FRAME_LEN;  // �ṹ��϶�����4
        /*lint +e778*/
    } else if ((en_nest_type == FRW_EVENT_TYPE_WLAN_CTX) && (uc_nest_sub_type == DMAC_WLAN_CTX_EVENT_SUB_TYPE_MGMT)) {
        /* case 2: mgmt frame, mac header is maintence in payload part */
        ul_headroom_add = uc_cb_length + (MAX_MAC_HEAD_LEN - MAC_80211_FRAME_LEN);
    } else {
        /* case 3: data from net, mac head not maintence in netbuff */
        /* case 4: netbuff alloced in adapt layer */
        ul_headroom_add = uc_cb_length + MAX_MAC_HEAD_LEN;
    }

    return ul_headroom_add;
}


void hmac_adjust_netbuf_data(oal_netbuf_stru *pst_netbuf, mac_tx_ctl_stru *pst_tx_ctrl,
                             frw_event_type_enum_uint8 en_nest_type, uint8_t uc_nest_sub_type)
{
    uint8_t *puc_data_hdr;
    uint8_t uc_cb_length;
    int32_t l_ret = EOK;

    /* �ڽ���HCC֮ǰ����CB�ֶκ�Macͷ���������payload֮ǰ */
    puc_data_hdr = oal_netbuf_data(pst_netbuf);

    uc_cb_length = OAL_SIZEOF(mac_tx_ctl_stru) - OAL_SIZEOF(mac_tx_expand_cb_stru);

    if ((en_nest_type == FRW_EVENT_TYPE_WLAN_CTX) && (uc_nest_sub_type == DMAC_WLAN_CTX_EVENT_SUB_TYPE_MGMT)) {
        /* case 1: mgmt frame, mac header is maintence in payload part */
        l_ret += memcpy_s(puc_data_hdr, uc_cb_length, (uint8_t *)pst_tx_ctrl, uc_cb_length);
        /* copy mac hdr */
        l_ret += memmove_s(puc_data_hdr + uc_cb_length, MAC_80211_FRAME_LEN,
                           puc_data_hdr + uc_cb_length + (MAX_MAC_HEAD_LEN - MAC_80211_FRAME_LEN),
                           MAC_80211_FRAME_LEN);
    } else if (MAC_GET_CB_FRAME_HEADER_ADDR(pst_tx_ctrl) != OAL_PTR_NULL) {
        /* case 2: data from net, mac head not maintence in netbuff */
        /* case 3: netbuff alloced in adapt layer */
        l_ret += memcpy_s(puc_data_hdr, uc_cb_length, (uint8_t *)pst_tx_ctrl, uc_cb_length);

        /* ֡ͷ��֡�岻������֡ͷ�����������¼��ڴ棬�˴���Ҫ�ͷ� */
        if (MAC_GET_CB_80211_MAC_HEAD_TYPE(pst_tx_ctrl) == 0) {
            l_ret += memmove_s(puc_data_hdr + uc_cb_length, MAC_GET_CB_FRAME_HEADER_LENGTH(pst_tx_ctrl),
                               (uint8_t *)MAC_GET_CB_FRAME_HEADER_ADDR(pst_tx_ctrl),
                               MAC_GET_CB_FRAME_HEADER_LENGTH(pst_tx_ctrl));
            oal_mem_free_m((uint8_t *)MAC_GET_CB_FRAME_HEADER_ADDR(pst_tx_ctrl), OAL_TRUE);
        }
    }
    if (l_ret != EOK) {
        oam_error_log0(0, OAM_SF_ANY, "hmac_adjust_netbuf_data::memcpy or memmove fail!");
        return;
    }
}


uint32_t hmac_hcc_tx_netbuf_auto(frw_event_mem_stru *pst_hcc_event_mem,
                                 oal_netbuf_stru *pst_netbuf, uint32_t ul_hdr_len)
{
    uint32_t fc_type, queue_id;
    frw_event_hdr_stru *pst_event_hdr;
    frw_event_type_enum_uint8 en_type;
    pst_event_hdr = frw_get_event_hdr(pst_hcc_event_mem);
    en_type = pst_event_hdr->en_type;

    if (oal_warn_on(en_type >= FRW_EVENT_TYPE_BUTT)) {
        oal_netbuf_free(pst_netbuf);
        oam_warning_log1(0, OAM_SF_ANY, "{hmac_hcc_tx_netbuf_auto::FRW_EVENT_TYPE[%d] over limit!}", en_type);
        return OAL_FAIL;
    }

    queue_id = g_hcc_sched_stat[en_type];
    fc_type = g_hcc_flowctrl_stat[en_type];

    /* ���ڴ���̫�����Ļ�ȡ����к� */
    if (en_type == FRW_EVENT_TYPE_HOST_DRX) {
        queue_id = oal_skb_get_queue_mapping(pst_netbuf);
        if (oal_warn_on(queue_id >= WLAN_NET_QUEUE_BUTT)) {
            queue_id = DATA_LO_QUEUE;
        } else {
            queue_id = g_wlan_queue_to_dmac_queue[queue_id];
        }
    }

    return hmac_hcc_tx_netbuf(pst_hcc_event_mem, pst_netbuf, ul_hdr_len, fc_type, queue_id);
}


uint32_t hmac_hcc_tx_netbuf(frw_event_mem_stru *pst_hcc_event_mem,
                            oal_netbuf_stru *pst_netbuf, uint32_t ul_hdr_len,
                            uint32_t fc_type,
                            uint32_t queue_id)
{
    frw_event_hdr_stru *pst_event_hdr = OAL_PTR_NULL;
    uint32_t ret = OAL_SUCC; // ����ʼ��ut����fail
    uint32_t ul_netbuf_old_addr;
    uint32_t ul_netbuf_new_addr;
    uint32_t ul_addr_offset;
    uint32_t ul_hcc_head_len = 0;
    uint32_t ul_hcc_head_pad;
    int32_t ul_head_room;

    declare_hcc_tx_param_initializer(st_hcc_transfer_param,
                                     HCC_ACTION_TYPE_WIFI,
                                     0,
                                     ul_hdr_len + OAL_SIZEOF(struct frw_hcc_extend_hdr),
                                     fc_type,
                                     queue_id);

    if (oal_unlikely(pst_netbuf == NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_hcc_tx_data:: pst_netbuf is null}");
        return -OAL_EINVAL;
    }

    ul_hcc_head_len = HCC_HDR_LEN + OAL_SIZEOF(struct frw_hcc_extend_hdr);

    /* һ������չHCC�����9�ֽ� */
    if (oal_netbuf_headroom(pst_netbuf) > 0) {
        /* ��֤cb 4bytes���� */
        oal_netbuf_push(pst_netbuf, 1);

        ul_hcc_head_pad = ul_hcc_head_len;
    } else {
        ul_hcc_head_pad = ul_hcc_head_len + 1;
    }

    ul_head_room = (int32_t)oal_netbuf_headroom(pst_netbuf);
    if (ul_hcc_head_pad > ul_head_room) {
        oam_warning_log1(0, OAM_SF_ANY, "{hmac_hcc_tx_netbuf expand head done![%d]}", ul_head_room);
        ret = (uint32_t)oal_netbuf_expand_head(pst_netbuf, (int32_t)ul_hcc_head_pad - ul_head_room, 0, GFP_ATOMIC);
        if (oal_warn_on(ret != OAL_SUCC)) {
            oam_error_log0(0, OAM_SF_ANY, "{hmac_hcc_tx_data:: alloc head room failed.}");
            oal_netbuf_free(pst_netbuf);
            return OAL_ERR_CODE_ALLOC_MEM_FAIL;
        }
    }

    if (ul_hcc_head_pad == ul_hcc_head_len) {
        /* �ָ���CB�׵�ַ */
        oal_netbuf_pull(pst_netbuf, 1);
    }

    ul_netbuf_old_addr = (uint32_t)(uintptr_t)(oal_netbuf_data(pst_netbuf) + ul_hdr_len);
    ul_netbuf_new_addr = OAL_ROUND_DOWN(ul_netbuf_old_addr, 4); // 4��4�ֽڶ���
    ul_addr_offset = ul_netbuf_old_addr - ul_netbuf_new_addr;

    /* δ����ʱ��host�������ݰ��ƣ��˴�����host�����device */
    if (ul_addr_offset) {
        ul_head_room = (int32_t)oal_netbuf_headroom(pst_netbuf);
        if (ul_addr_offset > ul_head_room) {
            ret = oal_netbuf_expand_head(pst_netbuf, ul_addr_offset - ul_head_room, 0, GFP_ATOMIC);
            if (oal_warn_on(ret != OAL_SUCC)) {
                oam_error_log0(0, OAM_SF_ANY, "{hmac_hcc_tx_data:: alloc head room failed.}");
                oal_netbuf_free(pst_netbuf);
                return OAL_ERR_CODE_ALLOC_MEM_FAIL;
            }
        }

        if (EOK != memmove_s((uint8_t *)oal_netbuf_data(pst_netbuf) - ul_addr_offset, oal_netbuf_len(pst_netbuf),
            (uint8_t *)oal_netbuf_data(pst_netbuf), oal_netbuf_len(pst_netbuf))) {
            oam_error_log0(0, OAM_SF_ANY, "hmac_hcc_tx_data::memmove fail!");
        }
        oal_netbuf_push(pst_netbuf, ul_addr_offset);
        oal_netbuf_trim(pst_netbuf, ul_addr_offset);
    }

    /* add frw hcc extend area */
    oal_netbuf_push(pst_netbuf, OAL_SIZEOF(struct frw_hcc_extend_hdr));

    pst_event_hdr = frw_get_event_hdr(pst_hcc_event_mem);
    hmac_hcc_adapt_extend_hdr_init(pst_event_hdr, pst_netbuf);

    // expand 14B�������½�40%,��ȷ��!
#ifdef CONFIG_PRINTK
    ret = (uint32_t)hcc_tx(hcc_get_110x_handler(), pst_netbuf, &st_hcc_transfer_param);
    if (oal_unlikely(ret != OAL_SUCC)) {
        /* hcc �ر�ʱ�·�������,������Ҫ���� */
        if (oal_unlikely(ret == -OAL_EBUSY)) {
            if (oal_print_rate_limit(30 * PRINT_RATE_SECOND)) {
                oal_warn_on(1);
            }
            oal_io_print("[E]hmac_tx event[%u:%u] drop!\n", pst_event_hdr->en_type, pst_event_hdr->uc_sub_type);
            ret = OAL_SUCC;
            declare_dft_trace_key_info("hcc_is_busy", OAL_DFT_TRACE_OTHER);
        }

        if (ret == -OAL_EIO) {
            /* hcc exception, drop the pkts */
            ret = OAL_SUCC;
        }

        oal_netbuf_free(pst_netbuf);
    } else if (oal_likely(pst_event_hdr->en_type < FRW_EVENT_TYPE_BUTT)) {
        g_hcc_sched_event_pkts[pst_event_hdr->en_type]++;
    }

    return ret;
#else
    /* UT Failed! Should remove this macro when DMT! */
    return ret;
#endif
}


uint32_t hmac_hcc_tx_data(frw_event_mem_stru *pst_hcc_event_mem, oal_netbuf_stru *pst_netbuf)
{
    frw_event_hdr_stru *pst_event_hdr = OAL_PTR_NULL;
    frw_event_type_enum_uint8 en_type;
    uint8_t uc_sub_type;
    mac_tx_ctl_stru *pst_tx_ctrl = OAL_PTR_NULL;
    uint32_t ul_headroom_add;
    int32_t ret;
    uint8_t auc_macheader[MAC_80211_QOS_HTC_4ADDR_FRAME_LEN] = { 0 };
    uint8_t uc_cb_length;

    /* ��ȡǶ�׵�ҵ���¼����� */
    pst_event_hdr = frw_get_event_hdr(pst_hcc_event_mem);

    en_type = pst_event_hdr->en_type;
    uc_sub_type = pst_event_hdr->uc_sub_type;

    if (oal_unlikely(pst_netbuf == NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_hcc_tx_data:: pst_netbuf is null}");
        return OAL_FAIL;
    }

    pst_tx_ctrl = (mac_tx_ctl_stru *)oal_netbuf_cb(pst_netbuf);
    if (oal_warn_on(MAC_GET_CB_IS_4ADDRESS(pst_tx_ctrl))) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_hcc_tx_data:: use 4 address.}");
        oal_netbuf_free(pst_netbuf);
        return OAL_FAIL;
    }

    uc_cb_length = OAL_SIZEOF(mac_tx_ctl_stru) - OAL_SIZEOF(mac_tx_expand_cb_stru);

    ul_headroom_add = check_headroom_add_length(pst_tx_ctrl, en_type, uc_sub_type, uc_cb_length);
    if (ul_headroom_add > oal_netbuf_headroom(pst_netbuf)) {
        if (MAC_GET_CB_80211_MAC_HEAD_TYPE(pst_tx_ctrl) == 1) {
            if (EOK != memcpy_s(auc_macheader, sizeof(auc_macheader),
                (uint8_t *)MAC_GET_CB_FRAME_HEADER_ADDR(pst_tx_ctrl), MAX_MAC_HEAD_LEN)) {
                oam_error_log0(0, OAM_SF_ANY, "hmac_hcc_tx_data::memcpy fail!");
                oal_netbuf_free(pst_netbuf);
                return OAL_FAIL;
            }
        }

        ret = oal_netbuf_expand_head(pst_netbuf,
                                     (int32_t)ul_headroom_add - (int32_t)oal_netbuf_headroom(pst_netbuf),
                                     0, GFP_ATOMIC);
        if (oal_warn_on(ret != OAL_SUCC)) {
            oam_error_log0(0, OAM_SF_ANY, "{hmac_hcc_tx_data:: alloc head room failed.}");
            oal_netbuf_free(pst_netbuf);
            return OAL_ERR_CODE_ALLOC_MEM_FAIL;
        }

        if (MAC_GET_CB_80211_MAC_HEAD_TYPE(pst_tx_ctrl) == 1) {
            if (EOK != memcpy_s(oal_netbuf_data(pst_netbuf), MAX_MAC_HEAD_LEN, auc_macheader, MAX_MAC_HEAD_LEN)) {
                oam_error_log0(0, OAM_SF_ANY, "hmac_hcc_tx_data::memcpy fail!");
                hmac_hcc_tx_netbuf_auto(pst_hcc_event_mem, pst_netbuf, uc_cb_length + MAX_MAC_HEAD_LEN);
                return OAL_FAIL;
            }
            MAC_GET_CB_FRAME_HEADER_ADDR(pst_tx_ctrl) = (mac_ieee80211_frame_stru *)oal_netbuf_data(pst_netbuf);
        }
    }

    /* �޸�netbuff��dataָ���len */
    oal_netbuf_push(pst_netbuf, ul_headroom_add);
    hmac_adjust_netbuf_data(pst_netbuf, pst_tx_ctrl, en_type, uc_sub_type);

    /* netbuf���ܳɹ�����ɷ��ͺ����ͷ�! */
    hmac_hcc_tx_netbuf_auto(pst_hcc_event_mem, pst_netbuf, uc_cb_length + MAX_MAC_HEAD_LEN);
    return OAL_SUCC;
}

uint32_t hmac_hcc_tx_netbuf_adapt(frw_event_mem_stru *pst_hcc_event_mem,
                                  oal_netbuf_stru *pst_netbuf)
{
    return hmac_hcc_tx_netbuf_auto(pst_hcc_event_mem, pst_netbuf, 0);
}

uint32_t hmac_hcc_tx_event_buf_to_netbuf(frw_event_mem_stru *pst_event_mem,
                                         uint8_t *pst_buf,
                                         uint32_t payload_size)
{
    oal_netbuf_stru *pst_netbuf;
    /* ����netbuf����¼�payload */
    pst_netbuf = hcc_netbuf_alloc(payload_size);
    if (oal_warn_on(pst_netbuf == NULL)) {
        oal_io_print("hmac_hcc_tx_event_buf_to_netbuf alloc netbuf failed!\n");
        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }

    /* ���ṹ�忽����netbuff������ */
    oal_netbuf_put(pst_netbuf, payload_size);
    if (EOK != memcpy_s((uint8_t *)(oal_netbuf_data(pst_netbuf)),
        payload_size, (uint8_t *)pst_buf, payload_size)) {
        oam_error_log0(0, OAM_SF_ANY, "hmac_hcc_tx_event_buf_to_netbuf::memcpy fail!");
        oal_netbuf_free(pst_netbuf);
        return OAL_FAIL;
    }

    return hmac_hcc_tx_netbuf_adapt(pst_event_mem, pst_netbuf);
}


uint32_t hmac_hcc_tx_event_payload_to_netbuf(frw_event_mem_stru *pst_event_mem,
                                             uint32_t payload_size)
{
    uint8_t *pst_event_payload = OAL_PTR_NULL;

    if (oal_warn_on(pst_event_mem == NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "hmac_hcc_tx_event_payload_to_netbuf:pst_event_mem null!");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ȡҵ���¼���Ϣ */
    pst_event_payload = frw_get_event_payload(pst_event_mem);
    return hmac_hcc_tx_event_buf_to_netbuf(pst_event_mem, pst_event_payload, payload_size);
}


uint32_t hmac_hcc_rx_event_comm_adapt(frw_event_mem_stru *pst_hcc_event_mem)
{
    uint8_t bit_mac_header_len;
    frw_event_hdr_stru *pst_event_hdr;
    hcc_event_stru *pst_hcc_event_payload;

    mac_rx_ctl_stru *pst_rx_ctrl = OAL_PTR_NULL;
    uint8_t *puc_hcc_extend_hdr = OAL_PTR_NULL;
    int32_t l_ret;

    /* step1 ��ȡǶ�׵�ҵ���¼����� */
    pst_event_hdr = frw_get_event_hdr(pst_hcc_event_mem);
    pst_hcc_event_payload = (hcc_event_stru *)frw_get_event_payload(pst_hcc_event_mem);
    /*
     * ��ɴ�51Mac rx ctl ��02 Mac rx ctl�Ŀ���,
     * �����˴�,pad_payload�Ѿ���0
     */
    /* hcc protocol header
    |-------hcc total(64B)-----|-----------package mem--------------|
    |hcc hdr|pad hdr|hcc extend|pad_payload|--------payload---------| */
    if (oal_warn_on(pst_hcc_event_payload->pst_netbuf == NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "hmac_hcc_rx_event_comm_adapt:did't found netbuf!");
        return OAL_FAIL;
    }

    puc_hcc_extend_hdr = oal_netbuf_data((oal_netbuf_stru *)pst_hcc_event_payload->pst_netbuf);
    bit_mac_header_len = ((mac_rx_ctl_stru *)puc_hcc_extend_hdr)->uc_mac_header_len;
    if (bit_mac_header_len) {
        if (bit_mac_header_len > MAX_MAC_HEAD_LEN) {
            oam_error_log3(pst_event_hdr->uc_vap_id, OAM_SF_ANY, "invaild mac header len:%d,main:%d,sub:%d",
                           bit_mac_header_len, pst_event_hdr->en_type, pst_event_hdr->uc_sub_type);
            oal_print_hex_dump(puc_hcc_extend_hdr,
                               (int32_t)oal_netbuf_len((oal_netbuf_stru *)pst_hcc_event_payload->pst_netbuf),
                               32, "invaild mac header len");
            return OAL_FAIL;
        }

        pst_rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb((oal_netbuf_stru *)pst_hcc_event_payload->pst_netbuf);
        l_ret = memcpy_s(pst_rx_ctrl, OAL_SIZEOF(mac_rx_ctl_stru),
                         puc_hcc_extend_hdr, OAL_SIZEOF(mac_rx_ctl_stru));

        /* ��Ҫ�޸�pst_rx_ctrl������ָ�� */
        MAC_GET_RX_CB_MAC_HEADER_ADDR(pst_rx_ctrl) = (uint32_t *)(puc_hcc_extend_hdr + OAL_MAX_CB_LEN +
                                                                    MAX_MAC_HEAD_LEN - pst_rx_ctrl->uc_mac_header_len);

        /* ��mac header��������ߵ�ַƫ��8���ֽڿ�����ʹ��mac header��payload���������� */
        l_ret += memmove_s((uint8_t *)MAC_GET_RX_CB_MAC_HEADER_ADDR(pst_rx_ctrl), pst_rx_ctrl->uc_mac_header_len,
                           (uint8_t *)((uint8_t *)MAC_GET_RX_CB_MAC_HEADER_ADDR(pst_rx_ctrl) -
                                         (MAX_MAC_HEAD_LEN - pst_rx_ctrl->uc_mac_header_len)),
                           pst_rx_ctrl->uc_mac_header_len);
        if (l_ret != EOK) {
            oam_error_log0(0, OAM_SF_ANY, "hmac_hcc_rx_event_comm_adapt::memcpy or memmove fail!");
        }

        /* ��netbuff dataָ���Ƶ�payloadλ�� */
        oal_netbuf_pull(pst_hcc_event_payload->pst_netbuf,
                        OAL_MAX_CB_LEN + (MAX_MAC_HEAD_LEN - pst_rx_ctrl->uc_mac_header_len));
    } else {
        oal_netbuf_pull(pst_hcc_event_payload->pst_netbuf, (OAL_MAX_CB_LEN + MAX_MAC_HEAD_LEN));
    }

    return OAL_SUCC;
}


frw_event_mem_stru *hmac_hcc_expand_rx_adpat_event(frw_event_mem_stru *pst_hcc_event_mem, uint32_t event_size)
{
    frw_event_hdr_stru *pst_hcc_event_hdr;
    hcc_event_stru *pst_hcc_event_payload;
    oal_netbuf_stru *pst_hcc_netbuf;
    frw_event_type_enum_uint8 en_type;
    uint8_t uc_sub_type;
    uint8_t uc_chip_id;
    uint8_t uc_device_id;
    uint8_t uc_vap_id;
    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL; /* ҵ���¼������Ϣ */

    /* ��ȡHCC�¼���Ϣ */
    pst_hcc_event_hdr = frw_get_event_hdr(pst_hcc_event_mem);
    pst_hcc_event_payload = (hcc_event_stru *)frw_get_event_payload(pst_hcc_event_mem);
    pst_hcc_netbuf = pst_hcc_event_payload->pst_netbuf;
    en_type = pst_hcc_event_hdr->en_type;
    uc_sub_type = pst_hcc_event_hdr->uc_sub_type;
    uc_chip_id = pst_hcc_event_hdr->uc_chip_id;
    uc_device_id = pst_hcc_event_hdr->uc_device_id;
    uc_vap_id = pst_hcc_event_hdr->uc_vap_id;

    /* ǿ��ת��Ϊuint16ǰ�����ж� */
    if (oal_unlikely(event_size > 0xffff)) {
        oam_warning_log1(0, OAM_SF_ANY,
                         "hmac_hcc_rx_netbuf_convert_to_event::invalid event len:%d", event_size);
        /* �ͷ�hcc�¼��������netbuf�ڴ� */
        oal_netbuf_free(pst_hcc_netbuf);
        return OAL_PTR_NULL;
    }

    /* ����ҵ���¼� */
    pst_event_mem = frw_event_alloc_m((uint16_t)event_size);
    if (oal_warn_on(pst_event_mem == OAL_PTR_NULL)) {
        oam_warning_log1(0, OAM_SF_ANY,
                         "hmac_hcc_rx_netbuf_convert_to_event  alloc event failed,event len:%d", event_size);
        /* �ͷ�hcc�¼��������netbuf�ڴ� */
        oal_netbuf_free(pst_hcc_netbuf);
        return OAL_PTR_NULL;
    }

    /* ��ҵ���¼�ͷ */
    frw_event_hdr_init(frw_get_event_hdr(pst_event_mem),
                       en_type,
                       uc_sub_type,
                       (uint16_t)event_size,
                       FRW_EVENT_PIPELINE_STAGE_1,
                       uc_chip_id,
                       uc_device_id,
                       uc_vap_id);

    return pst_event_mem;
}


frw_event_mem_stru *hmac_hcc_rx_netbuf_convert_to_event(frw_event_mem_stru *pst_hcc_event_mem,
                                                        uint32_t revert_size)
{
    hcc_event_stru *pst_hcc_event_payload = OAL_PTR_NULL;
    oal_netbuf_stru *pst_hcc_netbuf = OAL_PTR_NULL;
    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL; /* ҵ���¼������Ϣ */

    if (oal_warn_on(pst_hcc_event_mem == NULL)) {
        return NULL;
    }

    /* filter the extend buf */
    if (hmac_hcc_rx_event_comm_adapt(pst_hcc_event_mem) == OAL_FAIL) {
        return NULL;
    }

    pst_hcc_event_payload = (hcc_event_stru *)frw_get_event_payload(pst_hcc_event_mem);
    pst_hcc_netbuf = pst_hcc_event_payload->pst_netbuf;

    if (oal_warn_on(pst_hcc_netbuf == NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "Fatal Error,payload did't contain any netbuf!");
        return OAL_PTR_NULL;
    }

    if (revert_size > oal_netbuf_len(pst_hcc_netbuf)) {
        revert_size = oal_netbuf_len(pst_hcc_netbuf);
    }

    pst_event_mem = hmac_hcc_expand_rx_adpat_event(pst_hcc_event_mem, revert_size);
    if (pst_event_mem == OAL_PTR_NULL) {
        return OAL_PTR_NULL;
    }

    if (revert_size) {
        if (EOK != memcpy_s((uint8_t *)frw_get_event_payload(pst_event_mem), revert_size,
            (uint8_t *)oal_netbuf_data(pst_hcc_netbuf), revert_size)) {
            oam_error_log0(0, OAM_SF_ANY, "hmac_hcc_rx_netbuf_convert_to_event::memcpy fail!");
            oal_netbuf_free(pst_hcc_netbuf);
            frw_event_free_m(pst_event_mem);
            return OAL_PTR_NULL;
        }
    }
    /* �ͷ�hcc�¼��������netbuf�ڴ� */
    oal_netbuf_free(pst_hcc_netbuf);

    return pst_event_mem;
}


frw_event_mem_stru *hmac_hcc_rx_convert_netbuf_to_event_default(frw_event_mem_stru *pst_hcc_event_mem)
{
    hcc_event_stru *pst_hcc_event_payload = OAL_PTR_NULL;

    if (oal_warn_on(pst_hcc_event_mem == OAL_PTR_NULL)) {
        return OAL_PTR_NULL;
    }

    pst_hcc_event_payload = (hcc_event_stru *)frw_get_event_payload(pst_hcc_event_mem);
    return hmac_hcc_rx_netbuf_convert_to_event(pst_hcc_event_mem, pst_hcc_event_payload->ul_buf_len);
}


frw_event_mem_stru *hmac_hcc_test_rx_adapt(frw_event_mem_stru *pst_hcc_event_mem)
{
    hcc_event_stru *pst_hcc_event_payload;

    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL;
    hcc_event_stru *pst_hcc_rx_event = OAL_PTR_NULL;

    pst_hcc_event_payload = (hcc_event_stru *)frw_get_event_payload(pst_hcc_event_mem);

    /* filter the extend buf */
    if (hmac_hcc_rx_event_comm_adapt(pst_hcc_event_mem) == OAL_FAIL) {
        return NULL;
    }

    pst_event_mem = hmac_hcc_expand_rx_adpat_event(pst_hcc_event_mem, OAL_SIZEOF(hcc_event_stru));
    if (pst_event_mem == NULL) {
        return NULL;
    }

    /* ��ҵ���¼���Ϣ */
    pst_hcc_rx_event = (hcc_event_stru *)frw_get_event_payload(pst_event_mem);
    pst_hcc_rx_event->pst_netbuf = pst_hcc_event_payload->pst_netbuf;
    pst_hcc_rx_event->ul_buf_len = (uint32_t)oal_netbuf_len((oal_netbuf_stru *)pst_hcc_event_payload->pst_netbuf);

    return pst_event_mem;
}

frw_event_mem_stru *hmac_rx_convert_netbuf_to_netbuf_default(frw_event_mem_stru *pst_hcc_event_mem)
{
    hcc_event_stru *pst_hcc_event_payload;

    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL;

    dmac_tx_event_stru *pst_ctx_event = OAL_PTR_NULL;

    pst_hcc_event_payload = (hcc_event_stru *)frw_get_event_payload(pst_hcc_event_mem);

    /* filter the extend buf */
    if (hmac_hcc_rx_event_comm_adapt(pst_hcc_event_mem) == OAL_FAIL) {
        return NULL;
    }

    pst_event_mem = hmac_hcc_expand_rx_adpat_event(pst_hcc_event_mem, OAL_SIZEOF(dmac_tx_event_stru));
    if (pst_event_mem == NULL) {
        return NULL;
    }

    pst_ctx_event = (dmac_tx_event_stru *)frw_get_event_payload(pst_event_mem);

    pst_ctx_event->pst_netbuf = pst_hcc_event_payload->pst_netbuf;
    pst_ctx_event->us_frame_len = (uint16_t)oal_netbuf_len((oal_netbuf_stru *)pst_hcc_event_payload->pst_netbuf);

    oam_info_log2(0, OAM_SF_ANY, "{hmac_rx_convert_netbuf_to_netbuf_default::netbuf = %p, frame len = %d.}",
                  (uintptr_t)(pst_ctx_event->pst_netbuf), pst_ctx_event->us_frame_len);

    return pst_event_mem;
}

frw_event_mem_stru *hmac_rx_convert_netbuf_to_netbuf_ring(frw_event_mem_stru *phcc_event_mem)
{
    hcc_event_stru *hcc_event_payload;

    frw_event_mem_stru *pevent_mem = OAL_PTR_NULL;

    dmac_tx_event_stru *pctx_event = OAL_PTR_NULL;

    hcc_event_payload = (hcc_event_stru *)frw_get_event_payload(phcc_event_mem);

    /* filter the extend buf */
    if (hmac_hcc_rx_event_wow_comm_adapt(phcc_event_mem) != OAL_SUCC) {
        return NULL;
    }

    pevent_mem = hmac_hcc_expand_rx_adpat_event(phcc_event_mem, OAL_SIZEOF(dmac_tx_event_stru));
    if (pevent_mem == NULL) {
        return NULL;
    }

    pctx_event = (dmac_tx_event_stru *)frw_get_event_payload(pevent_mem);

    pctx_event->pst_netbuf = hcc_event_payload->pst_netbuf;
    pctx_event->us_frame_len = (uint16_t)oal_netbuf_len((oal_netbuf_stru *)hcc_event_payload->pst_netbuf);

    oam_info_log2(0, OAM_SF_ANY, "{hmac_rx_convert_netbuf_to_netbuf_wow::netbuf = %p, frame len = %d.}",
                  (uintptr_t)(pctx_event->pst_netbuf), pctx_event->us_frame_len);

    return pevent_mem;
}



frw_event_mem_stru *hmac_rx_process_data_rx_adapt(frw_event_mem_stru *pst_hcc_event_mem)
{
    hcc_event_stru *pst_hcc_event_payload = OAL_PTR_NULL;

    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL;
    dmac_wlan_drx_event_stru *pst_wlan_rx_event = OAL_PTR_NULL;

    pst_hcc_event_payload = (hcc_event_stru *)frw_get_event_payload(pst_hcc_event_mem);

    /* filter the extend buf */
    if (hmac_hcc_rx_event_comm_adapt(pst_hcc_event_mem) == OAL_FAIL) {
        return NULL;
    }

    pst_event_mem = hmac_hcc_expand_rx_adpat_event(pst_hcc_event_mem, OAL_SIZEOF(dmac_wlan_drx_event_stru));
    if (pst_event_mem == NULL) {
        return NULL;
    }

    /* ��ҵ���¼���Ϣ */
    pst_wlan_rx_event = (dmac_wlan_drx_event_stru *)frw_get_event_payload(pst_event_mem);
    pst_wlan_rx_event->pst_netbuf = pst_hcc_event_payload->pst_netbuf;
    pst_wlan_rx_event->us_netbuf_num = 1;  // Ŀǰ��֧��ͨ��SDIO��������Ĭ�϶��ǵ�֡

    return pst_event_mem;
}

frw_event_mem_stru *hmac_rx_process_mgmt_event_rx_adapt(frw_event_mem_stru *pst_hcc_event_mem)
{
    hcc_event_stru *pst_hcc_event_payload;

    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL;
    dmac_wlan_crx_event_stru *pst_crx_event = OAL_PTR_NULL;

    /* ȡHCC�¼���Ϣ */
    pst_hcc_event_payload = (hcc_event_stru *)frw_get_event_payload(pst_hcc_event_mem);

    /* filter the extend buf */
    if (hmac_hcc_rx_event_comm_adapt(pst_hcc_event_mem) == OAL_FAIL) {
        return NULL;
    }

    pst_event_mem = hmac_hcc_expand_rx_adpat_event(pst_hcc_event_mem, OAL_SIZEOF(dmac_wlan_crx_event_stru));
    if (pst_event_mem == NULL) {
        return NULL;
    }

    /* ��ҵ���¼���Ϣ */
    pst_crx_event = (dmac_wlan_crx_event_stru *)frw_get_event_payload(pst_event_mem);
    pst_crx_event->pst_netbuf = pst_hcc_event_payload->pst_netbuf;

    return pst_event_mem;
}


uint32_t hmac_hcc_rx_event_wow_comm_adapt(frw_event_mem_stru *pst_hcc_event_mem)
{
    uint8_t bit_mac_header_len;
    frw_event_hdr_stru *pst_event_hdr;
    hcc_event_stru *pst_hcc_event_payload;
    mac_rx_ctl_stru *pst_rx_ctrl = OAL_PTR_NULL;
    uint8_t *puc_hcc_extend_hdr = OAL_PTR_NULL;
    int32_t l_ret;

    /* step1 ��ȡǶ�׵�ҵ���¼����� */
    pst_event_hdr = frw_get_event_hdr(pst_hcc_event_mem);
    pst_hcc_event_payload = (hcc_event_stru *)frw_get_event_payload(pst_hcc_event_mem);
    /* hcc protocol header
    |-------hcc total(64B)-----|-----------package mem--------------|
    |hcc hdr|pad hdr|hcc extend|pad_payload|--------payload---------| */
    if (oal_warn_on(pst_hcc_event_payload->pst_netbuf == NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "hmac_hcc_rx_event_comm_adapt:did't found netbuf!");
        return OAL_FAIL;
    }

    puc_hcc_extend_hdr = oal_netbuf_data((oal_netbuf_stru *)pst_hcc_event_payload->pst_netbuf);
    bit_mac_header_len = ((mac_rx_ctl_stru *)puc_hcc_extend_hdr)->uc_mac_header_len;
    if (bit_mac_header_len) {
        if (bit_mac_header_len > MAX_MAC_HEAD_LEN) {
            oam_error_log3(pst_event_hdr->uc_vap_id, OAM_SF_ANY, "invaild mac header len:%d,main:%d,sub:%d",
                bit_mac_header_len, pst_event_hdr->en_type, pst_event_hdr->uc_sub_type);
            oal_print_hex_dump(puc_hcc_extend_hdr,
                               (int32_t)oal_netbuf_len((oal_netbuf_stru *)pst_hcc_event_payload->pst_netbuf),
                               32, "invaild mac header len");
            return OAL_FAIL;
        }

        pst_rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb((oal_netbuf_stru *)pst_hcc_event_payload->pst_netbuf);
        l_ret = memcpy_s(pst_rx_ctrl, OAL_SIZEOF(mac_rx_ctl_stru), puc_hcc_extend_hdr, OAL_SIZEOF(mac_rx_ctl_stru));
        if (l_ret != EOK) {
            oam_error_log0(0, OAM_SF_ANY, "hmac_hcc_rx_event_comm_adapt::memcpy fail!");
        }

        /* ��Ҫ�޸�pst_rx_ctrl������ָ�� */
        MAC_GET_RX_CB_MAC_HEADER_ADDR(pst_rx_ctrl) =
            (uint32_t *)(puc_hcc_extend_hdr + OAL_MAX_CB_LEN + MAX_MAC_HEAD_LEN);
    }

    /* ��netbuff dataָ���Ƶ�mac hdrλ�� */
    oal_netbuf_pull(pst_hcc_event_payload->pst_netbuf, (OAL_MAX_CB_LEN + MAX_MAC_HEAD_LEN));

    return OAL_SUCC;
}

frw_event_mem_stru *hmac_rx_process_wow_mgmt_event_rx_adapt(frw_event_mem_stru *pst_hcc_event_mem)
{
    hcc_event_stru *pst_hcc_event_payload;

    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL;
    dmac_wlan_crx_event_stru *pst_crx_event = OAL_PTR_NULL;

    /* ȡHCC�¼���Ϣ */
    pst_hcc_event_payload = (hcc_event_stru *)frw_get_event_payload(pst_hcc_event_mem);

    /* filter the extend buf */
    if (hmac_hcc_rx_event_wow_comm_adapt(pst_hcc_event_mem) == OAL_FAIL) {
        return NULL;
    }

    pst_event_mem = hmac_hcc_expand_rx_adpat_event(pst_hcc_event_mem, OAL_SIZEOF(dmac_wlan_crx_event_stru));
    if (pst_event_mem == NULL) {
        return NULL;
    }

    /* ��ҵ���¼���Ϣ */
    pst_crx_event = (dmac_wlan_crx_event_stru *)frw_get_event_payload(pst_event_mem);
    pst_crx_event->pst_netbuf = pst_hcc_event_payload->pst_netbuf;

    return pst_event_mem;
}



frw_event_mem_stru *hmac_rx_process_wow_data_rx_adapt(frw_event_mem_stru *pst_hcc_event_mem)
{
    hcc_event_stru *pst_hcc_event_payload = OAL_PTR_NULL;
    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL;
    dmac_wlan_drx_event_stru *pst_wlan_rx_event = OAL_PTR_NULL;

    pst_hcc_event_payload = (hcc_event_stru *)frw_get_event_payload(pst_hcc_event_mem);

    /* filter the extend buf */
    if (hmac_hcc_rx_event_wow_comm_adapt(pst_hcc_event_mem) == OAL_FAIL) {
        return NULL;
    }

    pst_event_mem = hmac_hcc_expand_rx_adpat_event(pst_hcc_event_mem, OAL_SIZEOF(dmac_wlan_drx_event_stru));
    if (pst_event_mem == NULL) {
        return NULL;
    }

    /* ��ҵ���¼���Ϣ */
    pst_wlan_rx_event = (dmac_wlan_drx_event_stru *)frw_get_event_payload(pst_event_mem);
    pst_wlan_rx_event->pst_netbuf = pst_hcc_event_payload->pst_netbuf;
    pst_wlan_rx_event->us_netbuf_num = 1;  // Ŀǰ��֧��ͨ��SDIO��������Ĭ�϶��ǵ�֡

    return pst_event_mem;
}


frw_event_mem_stru *hmac_cali2hmac_misc_event_rx_adapt(frw_event_mem_stru *pst_hcc_event_mem)
{
    hcc_event_stru *pst_hcc_event_payload = OAL_PTR_NULL;
    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL;
    hal_cali_hal2hmac_event_stru *pst_cali_save_event = OAL_PTR_NULL;

    pst_hcc_event_payload = (hcc_event_stru *)frw_get_event_payload(pst_hcc_event_mem);

    if (hmac_hcc_rx_event_comm_adapt(pst_hcc_event_mem) == OAL_FAIL) {
        return NULL;
    }

    pst_event_mem = hmac_hcc_expand_rx_adpat_event(pst_hcc_event_mem, OAL_SIZEOF(hal_cali_hal2hmac_event_stru));
    if (pst_event_mem == NULL) {
        oal_io_print("cali_hmac_rx_adapt_fail\r\n");
        return NULL;
    }

    /* ��ҵ���¼���Ϣ */
    pst_cali_save_event = (hal_cali_hal2hmac_event_stru *)frw_get_event_payload(pst_event_mem);
    pst_cali_save_event->pst_netbuf = pst_hcc_event_payload->pst_netbuf;
    pst_cali_save_event->us_netbuf_num = 1;  // Ŀǰ��֧��ͨ��SDIO��������Ĭ�϶��ǵ�֡

    return pst_event_mem;
}

#ifdef _PRE_WLAN_ONLINE_DPD

frw_event_mem_stru *hmac_dpd_rx_adapt(frw_event_mem_stru *pst_hcc_event_mem)
{
    hcc_event_stru *pst_hcc_event_payload;

    frw_event_mem_stru *pst_event_mem;
    hal_cali_hal2hmac_event_stru *pst_cali_save_event;

    pst_hcc_event_payload = (hcc_event_stru *)frw_get_event_payload(pst_hcc_event_mem);

    if (hmac_hcc_rx_event_comm_adapt(pst_hcc_event_mem) == OAL_FAIL) {
        return NULL;
    }

    pst_event_mem = hmac_hcc_expand_rx_adpat_event(pst_hcc_event_mem, OAL_SIZEOF(hal_cali_hal2hmac_event_stru));
    if (pst_event_mem == NULL) {
        oal_io_print("cali_hmac_rx_adapt_fail\r\n");
        return NULL;
    }

    /* ��ҵ���¼���Ϣ */
    pst_cali_save_event = (hal_cali_hal2hmac_event_stru *)frw_get_event_payload(pst_event_mem);
    pst_cali_save_event->pst_netbuf = pst_hcc_event_payload->pst_netbuf;
    pst_cali_save_event->us_netbuf_num = 1;  // Ŀǰ��֧��ͨ��SDIO��������Ĭ�϶��ǵ�֡

    return pst_event_mem;
}
#endif

#ifdef _PRE_WLAN_FEATURE_APF

frw_event_mem_stru *hmac_apf_program_report_rx_adapt(frw_event_mem_stru *pst_hcc_event_mem)
{
    hcc_event_stru *pst_hcc_event_payload;
    frw_event_mem_stru *pst_event_mem = NULL;
    dmac_apf_report_event_stru *pst_report_event = NULL;

    pst_hcc_event_payload = (hcc_event_stru *)frw_get_event_payload(pst_hcc_event_mem);

    /* filter the extend buf */
    if (hmac_hcc_rx_event_comm_adapt(pst_hcc_event_mem) == OAL_FAIL) {
        return NULL;
    }

    pst_event_mem = hmac_hcc_expand_rx_adpat_event(pst_hcc_event_mem, OAL_SIZEOF(dmac_apf_report_event_stru));
    if (pst_event_mem == NULL) {
        return NULL;
    }

    /* ��ҵ���¼���Ϣ */
    pst_report_event = (dmac_apf_report_event_stru *)frw_get_event_payload(pst_event_mem);
    pst_report_event->p_program = pst_hcc_event_payload->pst_netbuf;

    return pst_event_mem;
}
#endif

uint32_t hmac_proc_add_user_tx_adapt(frw_event_mem_stru *pst_event_mem)
{
    return hmac_hcc_tx_event_payload_to_netbuf(pst_event_mem, OAL_SIZEOF(dmac_ctx_add_user_stru));
}


uint32_t hmac_proc_del_user_tx_adapt(frw_event_mem_stru *pst_event_mem)
{
    return hmac_hcc_tx_event_payload_to_netbuf(pst_event_mem, OAL_SIZEOF(dmac_ctx_del_user_stru));
}


/*lint -e413*/
uint32_t hmac_proc_config_syn_tx_adapt(frw_event_mem_stru *pst_event_mem)
{
    hmac_to_dmac_cfg_msg_stru *pst_syn_cfg_payload;
    pst_syn_cfg_payload = (hmac_to_dmac_cfg_msg_stru *)frw_get_event_payload(pst_event_mem);

    return hmac_hcc_tx_event_payload_to_netbuf(pst_event_mem,
        (pst_syn_cfg_payload->us_len + (uint32_t)OAL_OFFSET_OF(hmac_to_dmac_cfg_msg_stru, auc_msg_body)));
}
/*lint +e413*/

/*lint -e413*/
uint32_t hmac_proc_config_syn_alg_tx_adapt(frw_event_mem_stru *pst_event_mem)
{
    hmac_to_dmac_cfg_msg_stru *pst_syn_cfg_payload;
    pst_syn_cfg_payload = (hmac_to_dmac_cfg_msg_stru *)frw_get_event_payload(pst_event_mem);

    return hmac_hcc_tx_event_payload_to_netbuf(pst_event_mem,
        (pst_syn_cfg_payload->us_len + (uint32_t)OAL_OFFSET_OF(hmac_to_dmac_cfg_msg_stru, auc_msg_body)));
}
/*lint +e413*/

uint32_t hmac_sdt_recv_reg_cmd_tx_adapt(frw_event_mem_stru *pst_event_mem)
{
    return hmac_hcc_tx_event_payload_to_netbuf(pst_event_mem,
        (uint32_t)((frw_get_event_stru(pst_event_mem))->st_event_hdr.us_length));
}

#if defined(_PRE_WLAN_FEATURE_DATA_SAMPLE) || defined(_PRE_WLAN_FEATURE_PSD_ANALYSIS)

uint32_t hmac_sdt_recv_sample_cmd_tx_adapt(frw_event_mem_stru *pst_event_mem)
{
    return hmac_hcc_tx_event_payload_to_netbuf(pst_event_mem,
        (uint32_t)((frw_get_event_stru(pst_event_mem))->st_event_hdr.us_length) - OAL_SIZEOF(frw_event_hdr_stru));
}
#endif

#ifdef _PRE_WLAN_FEATURE_HIEX

uint32_t hmac_hiex_h2d_msg_tx_adapt(frw_event_mem_stru *pst_event_mem)
{
    return hmac_hcc_tx_event_payload_to_netbuf(pst_event_mem,
        (uint32_t)((frw_get_event_stru(pst_event_mem))->st_event_hdr.us_length) - OAL_SIZEOF(frw_event_hdr_stru));
}
#endif


uint32_t hmac_proc_tx_host_tx_adapt(frw_event_mem_stru *pst_event_mem)
{
    oal_netbuf_stru *pst_current_netbuf;
    oal_netbuf_stru *pst_current_netbuf_tmp = NULL;
    dmac_tx_event_stru *pst_dmac_tx_event_payload;
    uint32_t ret;

    /* ȡҵ���¼���Ϣ */
    pst_dmac_tx_event_payload = (dmac_tx_event_stru *)frw_get_event_payload(pst_event_mem);
    pst_current_netbuf = pst_dmac_tx_event_payload->pst_netbuf;

    while (pst_current_netbuf != OAL_PTR_NULL) {
        /* ������netbuf�׳�֮ǰָ����һ��netbuf����ֹfrw_event_dispatch_event ������ netbuf->next */
        pst_current_netbuf_tmp = pst_current_netbuf;
        pst_current_netbuf = oal_netbuf_next(pst_current_netbuf);

        /* netbuf ʧ���ɱ��������ͷ�! */
        ret = hmac_hcc_tx_data(pst_event_mem, pst_current_netbuf_tmp);
        if (ret != OAL_SUCC) {
            return ret;
        }
    }
    return OAL_SUCC;
}


uint32_t hmac_proc_set_edca_param_tx_adapt(frw_event_mem_stru *pst_event_mem)
{
    return hmac_hcc_tx_event_payload_to_netbuf(pst_event_mem, OAL_SIZEOF(dmac_ctx_sta_asoc_set_edca_reg_stru));
}

#ifdef _PRE_WLAN_FEATURE_11AX

uint32_t hmac_proc_set_mu_edca_param_tx_adapt(frw_event_mem_stru *pst_event_mem)
{
    return hmac_hcc_tx_event_payload_to_netbuf(pst_event_mem, OAL_SIZEOF(dmac_ctx_sta_asoc_set_edca_reg_stru));
}


uint32_t hmac_proc_set_spatial_reuse_param_tx_adapt(frw_event_mem_stru *pst_event_mem)
{
    return hmac_hcc_tx_event_payload_to_netbuf(pst_event_mem, OAL_SIZEOF(dmac_ctx_sta_asoc_set_sr_reg_stru));
}

#endif


uint32_t hmac_scan_proc_scan_req_event_tx_adapt(frw_event_mem_stru *pst_event_mem)
{
    dmac_tx_event_stru *pst_dmac_tx_event = OAL_PTR_NULL;

    if (oal_unlikely(pst_event_mem == OAL_PTR_NULL)) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    oam_info_log0(0, OAM_SF_ANY, "{hmac_scan_proc_scan_req_event_tx_adapt:: scan req, enter into tx adapt.}");
    pst_dmac_tx_event = (dmac_tx_event_stru *)frw_get_event_payload(pst_event_mem);

    return hmac_hcc_tx_event_buf_to_netbuf(pst_event_mem,
                                           (uint8_t *)oal_netbuf_data(pst_dmac_tx_event->pst_netbuf),
                                           pst_dmac_tx_event->us_frame_len);
}


uint32_t hmac_send_event_netbuf_tx_adapt(frw_event_mem_stru *pst_event_mem)
{
    dmac_tx_event_stru *pst_dmac_tx_event = OAL_PTR_NULL;

    if (pst_event_mem == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_CALIBRATE, "{hmac_send_event_netbuf_tx_adapt::pst_event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_dmac_tx_event = (dmac_tx_event_stru *)frw_get_event_payload(pst_event_mem);
    return hmac_hcc_tx_event_buf_to_netbuf(pst_event_mem,
                                           (uint8_t *)oal_netbuf_data(pst_dmac_tx_event->pst_netbuf),
                                           pst_dmac_tx_event->us_frame_len);
}

uint32_t hmac_config_update_ip_filter_tx_adapt(frw_event_mem_stru *pst_event_mem)
{
    dmac_tx_event_stru *pst_dmac_tx_event = NULL;

    if (pst_event_mem == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_config_update_ip_filter_tx_adapt:: pst_event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_dmac_tx_event = (dmac_tx_event_stru *)frw_get_event_payload(pst_event_mem);
    return hmac_hcc_tx_event_buf_to_netbuf(pst_event_mem,
                                           (uint8_t *)oal_netbuf_data(pst_dmac_tx_event->pst_netbuf),
                                           pst_dmac_tx_event->us_frame_len);
}


uint32_t hmac_scan_proc_sched_scan_req_event_tx_adapt(frw_event_mem_stru *pst_event_mem)
{
    mac_pno_scan_stru *h2d_pno_scan_req_params = OAL_PTR_NULL; /* �·�PNO����ɨ������ */

    if (oal_unlikely(pst_event_mem == OAL_PTR_NULL)) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    h2d_pno_scan_req_params = (mac_pno_scan_stru *)(uintptr_t)(*(unsigned long *)frw_get_event_payload(pst_event_mem));

    return hmac_hcc_tx_event_buf_to_netbuf(pst_event_mem, (uint8_t *)h2d_pno_scan_req_params,
        OAL_SIZEOF(mac_pno_scan_stru));
}


uint32_t hmac_mgmt_update_user_qos_table_tx_adapt(frw_event_mem_stru *pst_event_mem)
{
    return hmac_hcc_tx_event_payload_to_netbuf(pst_event_mem, OAL_SIZEOF(dmac_ctx_asoc_set_reg_stru));
}


uint32_t hmac_proc_join_set_reg_event_tx_adapt(frw_event_mem_stru *pst_event_mem)
{
    oam_info_log0(0, OAM_SF_ANY, "{hmac_proc_join_set_reg_event_tx_adapt::tx adapt.}");
    return hmac_hcc_tx_event_payload_to_netbuf(pst_event_mem, OAL_SIZEOF(dmac_ctx_join_req_set_reg_stru));
}


uint32_t hmac_proc_join_set_dtim_reg_event_tx_adapt(frw_event_mem_stru *pst_event_mem)
{
    oam_info_log0(0, OAM_SF_ANY, "{hmac_proc_join_set_dtim_reg_event_tx_adapt::tx adapt.}");
    return hmac_hcc_tx_event_payload_to_netbuf(pst_event_mem, OAL_SIZEOF(dmac_ctx_set_dtim_tsf_reg_stru));
}


uint32_t hmac_hcc_tx_convert_event_to_netbuf_uint32(frw_event_mem_stru *pst_event_mem)
{
    return hmac_hcc_tx_event_payload_to_netbuf(pst_event_mem, OAL_SIZEOF(uint32_t));
}


uint32_t hmac_hcc_tx_convert_event_to_netbuf_uint16(frw_event_mem_stru *pst_event_mem)
{
    return hmac_hcc_tx_event_payload_to_netbuf(pst_event_mem, OAL_SIZEOF(uint16_t));
}


uint32_t hmac_hcc_tx_convert_event_to_netbuf_uint8(frw_event_mem_stru *pst_event_mem)
{
    return hmac_hcc_tx_event_payload_to_netbuf(pst_event_mem, OAL_SIZEOF(uint8_t));
}


uint32_t hmac_user_add_notify_alg_tx_adapt(frw_event_mem_stru *pst_event_mem)
{
    oam_info_log0(0, OAM_SF_ANY, "{hmac_user_add_notify_alg_tx_adapt::tx adapt.}");
    return hmac_hcc_tx_event_payload_to_netbuf(pst_event_mem, OAL_SIZEOF(dmac_ctx_add_user_stru));
}


uint32_t hmac_proc_rx_process_sync_event_tx_adapt(frw_event_mem_stru *pst_event_mem)
{
    return hmac_hcc_tx_event_payload_to_netbuf(pst_event_mem, OAL_SIZEOF(dmac_ctx_action_event_stru));
}

uint32_t hmac_chan_select_channel_mac_tx_adapt(frw_event_mem_stru *pst_event_mem)
{
    return hmac_hcc_tx_event_payload_to_netbuf(pst_event_mem, OAL_SIZEOF(dmac_set_chan_stru));
}


uint32_t hmac_chan_initiate_switch_to_new_channel_tx_adapt(frw_event_mem_stru *pst_event_mem)
{
    return hmac_hcc_tx_event_payload_to_netbuf(pst_event_mem, OAL_SIZEOF(dmac_set_ch_switch_info_stru));
}

#ifdef _PRE_WLAN_FEATURE_EDCA_OPT_AP

uint32_t hmac_edca_opt_stat_event_tx_adapt(frw_event_mem_stru *pst_event_mem)
{
    return hmac_hcc_tx_event_payload_to_netbuf(pst_event_mem, OAL_SIZEOF(uint8_t) * 16);
}

#endif

int32_t hmac_rx_extend_hdr_vaild_check(struct frw_hcc_extend_hdr *pst_extend_hdr)
{
    if (oal_unlikely(pst_extend_hdr->en_nest_type >= FRW_EVENT_TYPE_BUTT)) {
        return OAL_FALSE;
    }

    return OAL_TRUE;
}

int32_t hmac_rx_wifi_post_action_function(struct hcc_handler *hcc, uint8_t stype,
                                          hcc_netbuf_stru *pst_hcc_netbuf, uint8_t *pst_context)
{
    int32_t ret;
    hmac_vap_stru *pst_hmac_vap = OAL_PTR_NULL;
    struct frw_hcc_extend_hdr *pst_extend_hdr = OAL_PTR_NULL;

    frw_event_mem_stru *pst_event_mem = OAL_PTR_NULL; /* event mem */
    frw_event_stru *pst_event = OAL_PTR_NULL;
    hcc_event_stru *pst_event_payload = OAL_PTR_NULL;
    mac_rx_ctl_stru *pst_rx_ctl = OAL_PTR_NULL;
    uint8_t *puc_hcc_extend_hdr = OAL_PTR_NULL;

    oal_reference(hcc);

    pst_extend_hdr = (struct frw_hcc_extend_hdr *)oal_netbuf_data(pst_hcc_netbuf->pst_netbuf);
    if (OAL_TRUE != hmac_rx_extend_hdr_vaild_check(pst_extend_hdr)) {
        oal_print_hex_dump(oal_netbuf_data(pst_hcc_netbuf->pst_netbuf),
                           (int32_t)oal_netbuf_len(pst_hcc_netbuf->pst_netbuf),
                           32, "invaild frw extend hdr: ");
        return -OAL_EINVAL;
    }

    pst_hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_extend_hdr->vap_id);

    frw_event_task_lock();
    if (oal_unlikely(pst_hmac_vap == NULL)) {
        oam_warning_log1(0, OAM_SF_ANY, "hmac rx adapt ignored,pst vap is null, vap id:%u", pst_extend_hdr->vap_id);
        frw_event_task_unlock();
        oal_netbuf_free(pst_hcc_netbuf->pst_netbuf);
        return -OAL_EINVAL;
    }

    if (oal_unlikely(pst_hmac_vap->st_vap_base_info.uc_init_flag != MAC_VAP_VAILD)) {
        if (pst_extend_hdr->vap_id == 0) {
            /* ����VAP������ */
        } else {
            oam_warning_log2(pst_extend_hdr->vap_id, OAM_SF_ANY, "hmac rx adapt ignored,main:%u,sub:%u",
                             pst_extend_hdr->en_nest_type, pst_extend_hdr->uc_nest_sub_type);
            frw_event_task_unlock();
            oal_netbuf_free(pst_hcc_netbuf->pst_netbuf);
            return -OAL_ENOMEM;
        }
    }
    frw_event_task_unlock();

    pst_event_mem = frw_event_alloc_m(OAL_SIZEOF(hcc_event_stru));
    if (pst_event_mem == NULL) {
        oal_io_print("[WARN]event mem alloc failed\n");
        return -OAL_ENOMEM;
    }

    /* trim the frw hcc extend header */
    oal_netbuf_pull(pst_hcc_netbuf->pst_netbuf, OAL_SIZEOF(struct frw_hcc_extend_hdr));

    /* event hdr point */
    pst_event = frw_get_event_stru(pst_event_mem);

    /* fill event hdr */
    frw_event_hdr_init(&(pst_event->st_event_hdr),
                       pst_extend_hdr->en_nest_type,
                       pst_extend_hdr->uc_nest_sub_type,
                       OAL_SIZEOF(hcc_event_stru),
                       FRW_EVENT_PIPELINE_STAGE_1,
                       pst_extend_hdr->chip_id,
                       pst_extend_hdr->device_id,
                       pst_extend_hdr->vap_id);

    pst_event_payload = (hcc_event_stru *)frw_get_event_payload(pst_event_mem);
    pst_event_payload->pst_netbuf = pst_hcc_netbuf->pst_netbuf;
    pst_event_payload->ul_buf_len = oal_netbuf_len(pst_hcc_netbuf->pst_netbuf);

    puc_hcc_extend_hdr = oal_netbuf_data((oal_netbuf_stru *)pst_event_payload->pst_netbuf);
    pst_rx_ctl = (mac_rx_ctl_stru *)puc_hcc_extend_hdr;

    if (!(pst_rx_ctl->bit_is_beacon)) {
        pm_wifi_rxtx_count++;  // // �հ�ͳ�� for pm
    }

#ifdef _PRE_WLAN_WAKEUP_SRC_PARSE
    if ((OAL_TRUE == wlan_pm_wkup_src_debug_get()) && ((pst_extend_hdr->en_nest_type != FRW_EVENT_TYPE_WLAN_DRX))) {
        oal_io_print("wifi_wake_src:event[%d],subtype[%d]!\n",
                     pst_extend_hdr->en_nest_type, pst_extend_hdr->uc_nest_sub_type);

        /* ����֡�¼��������ڹ���֡���������д�ӡ����Ĺ���֡���ͺ�ر� */
        if (!((pst_extend_hdr->en_nest_type == FRW_EVENT_TYPE_WLAN_CRX) &&
            (pst_extend_hdr->uc_nest_sub_type == DMAC_WLAN_CRX_EVENT_SUB_TYPE_RX))) {
            wlan_pm_wkup_src_debug_set(OAL_FALSE);
        }
    }
#endif

    frw_event_task_lock();
    ret = (int32_t)frw_event_dispatch_event(pst_event_mem);
    frw_event_task_unlock();
    if (oal_warn_on(ret != OAL_SUCC)) {
        /* ����¼����ʧ�ܣ��ڴ�ʧ���ɸú����ͷţ�ֱ�ӵ��õ���rx adapt�����ͷ�! */
        oal_io_print("[WARN]hcc rx post event failed!!!ret=%u,main:%d,sub:%d\n",
                     ret,
                     pst_extend_hdr->en_nest_type,
                     pst_extend_hdr->uc_nest_sub_type);
    }
    frw_event_free_m(pst_event_mem);

    return ret;
}

int32_t hmac_hcc_adapt_init(void)
{
    hmac_tx_net_queue_map_init();
    hmac_tx_sched_info_init();
    hcc_rx_register(hcc_get_110x_handler(), HCC_ACTION_TYPE_WIFI, hmac_rx_wifi_post_action_function, NULL);
#ifdef _PRE_CONFIG_HISI_PANIC_DUMP_SUPPORT
    hwifi_panic_log_register(&hmac_panic_hcc_adapt, NULL);
#endif
    return OAL_SUCC;
}
