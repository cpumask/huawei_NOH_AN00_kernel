
#if (_PRE_OS_VERSION == _PRE_OS_VERSION_LINUX)
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/types.h>
#include <linux/err.h>
#endif

#include "oal_util.h"
#include "oal_ext_if.h"
#include "oam_ext_if.h"
#include "oal_net.h"
#include "hal_ring.h"
#include "securec.h"
#include "hal_device.h"
#include "hal_dscr.h"
#include "hal_mac.h"
#include "hal_irq.h"
#include "oal_pcie_host.h"
#include "oal_kernel_file.h"
#include "oal_cfg80211.h"
#include "mac_common.h"
#include "oam_ext_if.h"
#include "oal_net.h"
#include "hi1106_common_ops.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HAL_HOST_DSCR_C

#ifdef CONFIG_ARCH_KIRIN_PCIE
/* to be removed */
#define OAL_MAKE_WORD64(lsw, msw) ((((uint64_t)(msw) << 32) & 0xFFFFFFFF00000000) | (lsw))
#define GET_LOW_32_BITS(a)  ((uint32_t)(((uint64_t)(a)) & 0x00000000FFFFFFFFUL))
#define GET_HIGH_32_BITS(a) ((uint32_t)((((uint64_t)(a)) & 0xFFFFFFFF00000000UL) >> 32UL))

hal_ring_mac_regs_info g_ring_reg_tbl[HAL_RING_ID_BUTT] = {
    [HAL_RING_TYPE_L_F] = {
        .base_lsb = HI1106_MAC_RX_NORM_DATA_FREE_RING_ADDR_LSB_REG,
        .base_msb = HI1106_MAC_RX_NORM_DATA_FREE_RING_ADDR_MSB_REG,
        .size     = HI1106_MAC_RX_NORM_DATA_FREE_RING_SIZE_REG,
        .wptr_reg = HI1106_MAC_RX_NORM_DATA_FREE_RING_WPTR_REG,
        .rptr_reg = HI1106_MAC_RX_NORM_DATA_FREE_RING_RPTR_REG,
        .cfg_rptr_reg = HI1106_MAC_RX_NORM_DATA_FREE_RING_RPTR_CFG_REG,
    },
    [HAL_RING_TYPE_S_F] = {
        .base_lsb = HI1106_MAC_RX_SMALL_DATA_FREE_RING_ADDR_LSB_REG,
        .base_msb = HI1106_MAC_RX_SMALL_DATA_FREE_RING_ADDR_MSB_REG,
        .size     = HI1106_MAC_RX_SMALL_DATA_FREE_RING_SIZE_REG,
        .wptr_reg = HI1106_MAC_RX_SMALL_DATA_FREE_RING_WPTR_REG,
        .rptr_reg = HI1106_MAC_RX_SMALL_DATA_FREE_RING_RPTR_REG,
        .cfg_rptr_reg = HI1106_MAC_RX_SMALL_DATA_FREE_RING_RPTR_CFG_REG,
    },
    [HAL_RING_TYPE_COMP] = {
        .base_lsb = HI1106_MAC_RX_DATA_CMP_RING_ADDR_LSB_REG,
        .base_msb = HI1106_MAC_RX_DATA_CMP_RING_ADDR_MSB_REG,
        .len      = HI1106_MAC_RX_DATA_BUFF_LEN_REG,
        .size     = HI1106_MAC_RX_DATA_CMP_RING_SIZE_REG,
        .wptr_reg = HI1106_MAC_RX_DATA_CMP_RING_WPTR_REG,
        .rptr_reg = HI1106_MAC_RX_DATA_CMP_RING_RPTR_REG,
        .cfg_wptr_reg = HI1106_MAC_RX_DATA_CMP_RING_WPTR_CFG_REG,
    },
    [TX_BA_INFO_RING] = {
        .base_lsb = HI1106_MAC_TX_BA_INFO_BUF_ADDR_LSB_REG,
        .base_msb = HI1106_MAC_TX_BA_INFO_BUF_ADDR_MSB_REG,
        .len      = 0,
        .size     = HI1106_MAC_TX_BA_INFO_BUF_DEPTH_REG,
        .wptr_reg = HI1106_MAC_BA_INFO_BUF_WPTR_REG,
        .rptr_reg = HI1106_MAC_TX_BA_INFO_RPTR_REG,
        .cfg_wptr_reg = HI1106_MAC_TX_BA_INFO_WPTR_REG,
    }
};

void hi1106_rx_buf_get_statics(hal_host_device_stru *hal_device)
{
    hal_device->rx_statics.comp_ring_get++;
    return;
}

int32_t hi1106_rx_get_node_idx(hal_rx_nodes *nodes, dma_addr_t dma_addr)
{
    uint32_t count = 0;
    uint32_t idx;
    hal_rx_node *node = NULL;

    if (nodes == NULL) {
        return -OAL_EFAIL;
    }

    node  = nodes->tbl;
    idx   = nodes->last_idx;
    do {
        idx++;
        if (idx == nodes->tbl_size) {
            idx = 0;
        }
        if ((node[idx].dma_addr == dma_addr) && (node[idx].allocated)) {
            return idx;
        }
        count++;
    } while (count < nodes->tbl_size);

    oam_warning_log0(0, 0, "hal_rx_get_node_idx: not find");
    return -OAL_EFAIL;
}


oal_netbuf_stru *hi1106_rx_get_node(hal_rx_nodes *rx_nodes, uint32_t idx)
{
    hal_rx_node *node = NULL;
    oal_pcie_linux_res *pcie_linux_res = oal_get_pcie_linux_res();

    if (pcie_linux_res == NULL) {
        oam_error_log0(0, 0, "hal_rx_get_node_idx: pcie_linux_res null");
        return NULL;
    }

    if (idx >= rx_nodes->tbl_size) {
        oam_error_log1(0, 0, "hal_rx_get_node_idx: idx [%d]", idx);
        return NULL;
    }
    if (rx_nodes->tbl[idx].allocated) {
        node = &rx_nodes->tbl[idx];
        /* �ͷ�IO��Դ */
        dma_unmap_single(&pcie_linux_res->pst_pcie_dev->dev, node->dma_addr,
            node->netbuf->len, rx_nodes->dma_dir);
        node->allocated = 0;
        return node->netbuf;
    } else {
        oam_error_log1(0, 0, "rx_get_node:node not alloced! node[%p]", idx);
        return NULL;
    }
}


void hi1106_rx_alloc_list_free(hal_host_device_stru *hal_dev, hal_host_rx_alloc_list_stru *alloc_list)
{
    oal_netbuf_head_stru        *list           = &alloc_list->list;
    oal_pcie_linux_res          *pcie_linux_res = oal_get_pcie_linux_res();
    oal_netbuf_stru             *skb            = NULL;
    oal_netbuf_stru             *next_skb       = NULL;
    pcie_cb_dma_res             *cb_res         = NULL;
    unsigned long                flags = 0;

    if (pcie_linux_res == NULL) {
        oam_error_log0(0, 0, "hi1106_rx_alloc_list_free:pcie_linux_res null");
        return;
    }
    oal_spin_lock_irq_save(&alloc_list->lock, &flags);
    if (oal_netbuf_peek(list) == NULL) {
        oal_spin_unlock_irq_restore(&alloc_list->lock, &flags);
        return;
    }
    oal_netbuf_search_for_each_safe(skb, next_skb, list) {
        cb_res = (pcie_cb_dma_res *)oal_netbuf_cb(skb);
        dma_unmap_single(&pcie_linux_res->pst_pcie_dev->dev, cb_res->paddr.addr,
            skb->len, hal_dev->rx_nodes->dma_dir);
        __netbuf_unlink(skb, list);
        oal_netbuf_free(skb);
    }
    oal_spin_unlock_irq_restore(&alloc_list->lock, &flags);
}

/* �������� : �ͷ�rx ring�ϵ�netbuff���ͷŶ�Ӧ��io��Դ */
void hi1106_rx_free_res(hal_host_device_stru *hal_device)
{
    hal_rx_node *node = NULL;
    oal_pcie_linux_res *pcie_linux_res = oal_get_pcie_linux_res();
    uint32_t count = 0;
    uint32_t free_num = 0;

    if (hal_device == NULL) {
        oam_error_log0(0, 0, "hal_rx_free_res: hal_device null");
        return;
    }

    if ((pcie_linux_res == NULL) || (hal_device->rx_nodes == NULL)) {
        oam_error_log0(0, 0, "hal_rx_free_res: pcie_linux_res null or rx_nodes now inited");
        return;
    }

    node = hal_device->rx_nodes->tbl;
    do {
        if ((node[count].allocated) && (node[count].netbuf != NULL)) {
            /* �ͷ�IO��Դ */
            dma_unmap_single(&pcie_linux_res->pst_pcie_dev->dev,
                node[count].dma_addr,
                node[count].netbuf->len,
                hal_device->rx_nodes->dma_dir);

            node[count].allocated = 0;
            oal_netbuf_free(node[count].netbuf);
            free_num++;
        }
        count++;
    } while (count < hal_device->rx_nodes->tbl_size);

    hal_device->rx_nodes->last_idx = 0;
    oam_info_log1(0, 0, "hal_rx_free_res: free succ. release cnt[%d]", free_num);
    return;
}


uint32_t hi1106_rx_alloc_node(hal_rx_nodes *rx_nodes, oal_netbuf_stru *netbuf, dma_addr_t *dma_addr)
{
    hal_rx_node *node = NULL;
    uint32_t count = 0;
    uint32_t idx;
    oal_pcie_linux_res *pcie_linux_res = oal_get_pcie_linux_res();

    if (pcie_linux_res == NULL) {
        oam_error_log0(0, 0, "hal_rx_alloc_node: pcie_linux_rsc null");
        return OAL_FAIL;
    }

    node = rx_nodes->tbl;
    *dma_addr = dma_map_single(&pcie_linux_res->pst_pcie_dev->dev,
        oal_netbuf_data(netbuf), oal_netbuf_len(netbuf), rx_nodes->dma_dir);

    if (dma_mapping_error(&pcie_linux_res->pst_pcie_dev->dev, *dma_addr)) {
        oam_warning_log1(0, 0, "hal_rx_alloc_node: len[%d]", oal_netbuf_len(netbuf));
        return OAL_FAIL;
    }

    idx = rx_nodes->last_idx;
    do {
        idx++;
        if (idx == rx_nodes->tbl_size) {
            idx = 0;
        }
        if (node[idx].allocated == 0) {
            node[idx].allocated = 1;
            break;
        }
        count++;
    } while (count < rx_nodes->tbl_size);

    if (count == rx_nodes->tbl_size) {
        oam_error_log0(0, 0, "hal_rx_alloc_node:alloc node fail.");
        return OAL_FAIL;
    }

    node[idx].dma_addr = *dma_addr;
    node[idx].netbuf   = netbuf;
    rx_nodes->last_idx = idx;
    return OAL_SUCC;
}

oal_netbuf_stru *hi1106_rx_get_next_sub_msdu(hal_host_device_stru *hal_device,
    oal_netbuf_stru *netbuf)
{
    hi1106_rx_mpdu_desc_stru    *rx_hw_dscr = NULL;
    dma_addr_t                   host_iova;
    uint64_t                     next_dscr_addr;
    hal_host_rx_alloc_list_stru *alloc_list = &hal_device->host_rx_normal_alloc_list;
    unsigned long                flags = 0;
    uint32_t                     pre_num = 0;
    oal_netbuf_stru             *sub_msdu = NULL;

    hi1106_rx_buf_get_statics(hal_device);
    rx_hw_dscr = (hi1106_rx_mpdu_desc_stru *)oal_netbuf_data(netbuf);
    if ((!rx_hw_dscr->next_sub_msdu_addr_lsb) && (!rx_hw_dscr->next_sub_msdu_addr_msb)) {
        return NULL;
    }

    next_dscr_addr = OAL_MAKE_WORD64(rx_hw_dscr->next_sub_msdu_addr_lsb, rx_hw_dscr->next_sub_msdu_addr_msb);
    if (OAL_SUCC != pcie_if_devva_to_hostca(0, next_dscr_addr, (uint64_t *)&host_iova)) {
        oam_warning_log0(0, OAM_SF_RX, "{hal_rx_get_next_sub_msdu:get next msdu fail.}");
        return NULL;
    }
    oal_spin_lock_irq_save(&alloc_list->lock, &flags);
    sub_msdu = hi1106_alloc_list_delete_netbuf(hal_device, alloc_list, host_iova, &pre_num);
    oal_spin_unlock_irq_restore(&alloc_list->lock, &flags);
    if (pre_num) {
        oam_error_log1(hal_device->device_id, OAM_SF_RX, "hi1106_rx_get_next_sub_msdu:pre num[%d]", pre_num);
    }
    if (sub_msdu == NULL) {
        oam_error_log0(hal_device->device_id, OAM_SF_RX, "hi1106_rx_get_next_sub_msdu:sub msdu not found");
    }
    return sub_msdu;
}

void hi1106_host_rx_amsdu_list_build(hal_host_device_stru *hal_device, oal_netbuf_stru *netbuf)
{
    oal_netbuf_stru                     *cur_netbuf;
    oal_netbuf_stru                     *next_netbuf = OAL_PTR_NULL;

    cur_netbuf = netbuf;
    while (cur_netbuf != OAL_PTR_NULL) {
        /* msdu������Ϊ�˲�й¶�ڴ�,msdu��Ҫȫ��������� */
        next_netbuf = hi1106_rx_get_next_sub_msdu(hal_device, cur_netbuf);
        oal_netbuf_next(cur_netbuf) = next_netbuf;
        cur_netbuf = next_netbuf;
    }
}

uint8_t hi1106_host_get_rx_msdu_status(oal_netbuf_stru *netbuf)
{
    hi1106_rx_mpdu_desc_stru *rx_hw_dscr = NULL;

    rx_hw_dscr = (hi1106_rx_mpdu_desc_stru *)oal_netbuf_data(netbuf);
    return rx_hw_dscr->bit_rx_status;
}


uint8_t hi1106_rx_is_frag_frm(mac_header_frame_control_stru *fc_ctl_hdr,
    hi1106_rx_mpdu_desc_stru *rx_msdu_hw_dscr)
{
    return (uint8_t)((rx_msdu_hw_dscr->bit_frame_format == MAC_FRAME_TYPE_80211) &&
        (fc_ctl_hdr->bit_more_frag || rx_msdu_hw_dscr->bit_fragment_num));
}

static void hi1106_host_set_rx_cb(mac_rx_ctl_stru *rx_ctl, hi1106_rx_mpdu_desc_stru *rx_hw_dscr)
{
    rx_ctl->bit_mcast_bcast     = rx_hw_dscr->bit_mcast_bcast;
    rx_ctl->bit_amsdu_enable    = rx_hw_dscr->bit_is_amsdu;
    rx_ctl->bit_buff_nums       = 1;
    rx_ctl->us_frame_control    = rx_hw_dscr->bit_frame_control;
    rx_ctl->bit_start_seqnum    = rx_hw_dscr->bit_start_seqnum;
    rx_ctl->bit_cipher_type     = rx_hw_dscr->bit_cipher_type;
    rx_ctl->bit_release_start_sn = rx_hw_dscr->bit_release_start_seqnum;
    rx_ctl->bit_release_end_sn  = rx_hw_dscr->bit_release_end_seqnum;
    rx_ctl->bit_fragment_num    = rx_hw_dscr->bit_fragment_num;
    rx_ctl->bit_frame_format    = rx_hw_dscr->bit_frame_format;
    rx_ctl->bit_process_flag    = rx_hw_dscr->bit_process_flag;
    rx_ctl->us_frame_len        = rx_hw_dscr->bit_packet_len;
    rx_ctl->bit_rx_user_id      = rx_hw_dscr->bit_user_id;
    rx_ctl->us_seq_num          = rx_hw_dscr->bit_sequence_num;
    rx_ctl->bit_ta_user_idx     = rx_hw_dscr->bit_user_id & 0x1f; /* 1f valid bits */
    rx_ctl->bit_is_first_buffer = rx_hw_dscr->bit_first_sub_msdu;
    rx_ctl->rx_tid              = rx_hw_dscr->bit_tid;
    rx_ctl->dst_hal_vap_id      = rx_hw_dscr->bit_dst_vap_id;
    rx_ctl->bit_band_id         = rx_hw_dscr->bit_band_id;
    rx_ctl->bit_dst_band_id     = rx_hw_dscr->bit_dst_band_id;
    rx_ctl->rx_status           = rx_hw_dscr->bit_rx_status;
    rx_ctl->bit_is_ampdu        = rx_hw_dscr->bit_is_ampdu;
    rx_ctl->uc_msdu_in_buffer   = rx_hw_dscr->bit_sub_msdu_num;
    rx_ctl->ul_rx_lsb_pn        = rx_hw_dscr->ul_pn_lsb;
    rx_ctl->us_rx_msb_pn        = rx_hw_dscr->bit_pn_msb;
    rx_ctl->bit_vap_id          = rx_hw_dscr->bit_vap_id;
    if ((rx_hw_dscr->bit_vap_id != HI1106_OTHER_BSS_ID) && (rx_hw_dscr->bit_vap_id >= HI1106_HAL_VAP_OFFSET)) {
        rx_ctl->bit_vap_id -= HI1106_HAL_VAP_OFFSET;
    }

    return;
}

/* �����ϱ�sdt rx msdu������ */
void hi1106_host_rx_dump_msdu_dscr(hi1106_rx_mpdu_desc_stru *rx_dscr)
{
    uint8_t macaddr[WLAN_MAC_ADDR_LEN] = {0};

    if (!oam_ota_get_rx_dscr_switch()) {
        return;
    }

    if (oam_report_dscr(&macaddr[0], (uint8_t *)rx_dscr, HAL_RX_DSCR_LEN, OAM_OTA_TYPE_RX_DSCR_PILOT) != OAL_SUCC) {
        oam_info_log0(0, OAM_SF_RX, "{rx_dump_msdu_dscr::rpt rx dscr fail}");
        return;
    }

    return;
}

uint32_t hi1106_host_rx_get_msdu_info_dscr(oal_netbuf_stru *netbuf,
    mac_rx_ctl_stru *rx_info)
{
    uint8_t                        mac_hdr_len = MAC_80211_FRAME_LEN;
    hi1106_rx_mpdu_desc_stru      *rx_msdu_hw_dscr = NULL;
    mac_header_frame_control_stru *fc_ctl_hdr = NULL;
    uint8_t                        order_length;
    mac_rx_ctl_stru                rx_ctl;
    pcie_cb_dma_res               *cb_res = NULL;

    if (oal_unlikely(oal_any_null_ptr2(netbuf, rx_info))) {
        oam_error_log0(0, OAM_SF_RX, "{hal_host_rx_get_msdu_info_dscr:: null param}");
        return OAL_FAIL;
    }
    cb_res = (pcie_cb_dma_res *)oal_netbuf_cb(netbuf);
    memset_s(&rx_ctl, OAL_SIZEOF(mac_rx_ctl_stru), 0, OAL_SIZEOF(mac_rx_ctl_stru));

    rx_msdu_hw_dscr = (hi1106_rx_mpdu_desc_stru *)oal_netbuf_data(netbuf);
    hi1106_host_set_rx_cb(&rx_ctl, rx_msdu_hw_dscr);

    fc_ctl_hdr = (mac_header_frame_control_stru *)&rx_ctl.us_frame_control;
    if ((fc_ctl_hdr->bit_from_ds == 1) && (fc_ctl_hdr->bit_to_ds == 1)) {
        mac_hdr_len += OAL_MAC_ADDR_LEN;
    }

    /* HTC + Header */
    order_length = (fc_ctl_hdr->bit_order ? OAL_SIZEOF(uint32_t) : 0);
    mac_hdr_len += order_length;
    /* QoS Header */
    if ((fc_ctl_hdr->bit_type == WLAN_DATA_BASICTYPE) &&
        ((fc_ctl_hdr->bit_sub_type & WLAN_QOS_DATA) == WLAN_QOS_DATA)) {
        mac_hdr_len += OAL_SIZEOF(uint16_t);
    }

    if ((fc_ctl_hdr->bit_type == WLAN_DATA_BASICTYPE) || (fc_ctl_hdr->bit_type == WLAN_MANAGEMENT)) {
        rx_ctl.uc_mac_header_len  = mac_hdr_len;
    } else {
        /* ctrl frame ������ CTS �� ACK ��Ҫ���� monitorģʽ������ */
        rx_ctl.uc_mac_header_len = MAC_80211_CTL_HEADER_LEN;
    }

    if (hi1106_rx_is_frag_frm(fc_ctl_hdr, rx_msdu_hw_dscr)) {
        rx_ctl.bit_is_fragmented = OAL_TRUE;
    }

    if ((rx_msdu_hw_dscr->bit_packet_len == 0) ||
        (rx_msdu_hw_dscr->bit_packet_len > (cb_res->len - HAL_RX_DSCR_LEN))) {
        oam_error_log3(0, OAM_SF_RX, "host_rx_get_msdu_info_dscr:dscr addr:0x%x, pkt len:%d, pcie cb len:%d",
            (uintptr_t)rx_msdu_hw_dscr, rx_msdu_hw_dscr->bit_packet_len, cb_res->len);
        oam_report_dscr(BROADCAST_MACADDR, (uint8_t *)rx_msdu_hw_dscr, HAL_RX_DSCR_LEN, OAM_OTA_TYPE_RX_DSCR_PILOT);
        return OAL_FAIL;
    }

    memcpy_s(rx_info, OAL_SIZEOF(mac_rx_ctl_stru), &rx_ctl, OAL_SIZEOF(mac_rx_ctl_stru));
    oal_netbuf_pull(netbuf, HAL_RX_DSCR_LEN);
    skb_trim(netbuf, rx_msdu_hw_dscr->bit_packet_len);

    hi1106_host_rx_dump_msdu_dscr(rx_msdu_hw_dscr);
    return OAL_SUCC;
}

uint32_t hi1106_dma_map_rx_buff(hal_rx_nodes *rx_nodes, oal_netbuf_stru *netbuf, dma_addr_t *dma_addr)
{
    oal_pcie_linux_res *pcie_linux_res = oal_get_pcie_linux_res();

    if (pcie_linux_res == NULL) {
        oam_error_log0(0, 0, "hal_rx_alloc_node: pcie_linux_rsc null");
        return OAL_FAIL;
    }

    *dma_addr = dma_map_single(&pcie_linux_res->pst_pcie_dev->dev,
        oal_netbuf_data(netbuf), oal_netbuf_len(netbuf), rx_nodes->dma_dir);

    if (dma_mapping_error(&pcie_linux_res->pst_pcie_dev->dev, *dma_addr)) {
        oam_warning_log1(0, 0, "hal_rx_alloc_node: len[%d]", oal_netbuf_len(netbuf));
        return OAL_FAIL;
    }
    return OAL_SUCC;
}

uint32_t hi1106_host_rx_add_buff_alloc(hal_host_device_stru *hal_device,
    hal_host_ring_ctl_stru *ring_ctl, uint32_t buff_size, hal_host_rx_alloc_list_stru *alloc_list)
{
    uint16_t need_add = 0;
    uint16_t succ_add_num = 0;
    uint16_t loop;
    oal_netbuf_stru *netbuf = NULL;
    size_t mpdu_desc_size = OAL_SIZEOF(hi1106_rx_mpdu_desc_stru);
    pcie_cb_dma_res *cb_res = NULL;
    dma_addr_t pci_dma_addr;
    uint64_t  devva;
    unsigned long lock_flag;

    hal_ring_get_entry_count(ring_ctl, &need_add);
    for (loop = 0; loop < need_add; loop++) {
        netbuf = oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF, buff_size, OAL_NETBUF_PRIORITY_HIGH);
        if (netbuf == OAL_PTR_NULL) {
            oam_warning_log0(0, 0, "hi1106_host_rx_add_buff_alloc skb alloc fail");
            break;
        }
        oal_netbuf_set_len(netbuf, buff_size);
        memset_s(oal_netbuf_data(netbuf), mpdu_desc_size, 0, mpdu_desc_size);
        if (OAL_SUCC != hi1106_dma_map_rx_buff(hal_device->rx_nodes, netbuf, &pci_dma_addr)) {
            oal_netbuf_free(netbuf);
            oam_warning_log0(0, 0, "host_rx_add_buff_alloc:alloc node fail");
            break;
        }
        cb_res = (pcie_cb_dma_res *)oal_netbuf_cb(netbuf);
        cb_res->paddr.addr = pci_dma_addr;
        cb_res->len = oal_netbuf_len(netbuf);
        if (OAL_SUCC != pcie_if_hostca_to_devva(0, pci_dma_addr, &devva)) {
            oal_netbuf_free(netbuf);
            break;
        }
        /* �ж��ϰ벿Ҳ�����alloc_list����Ҫ���ж� */
        oal_spin_lock_irq_save(&alloc_list->lock, &lock_flag);
        oal_netbuf_list_tail(&alloc_list->list, netbuf);
        oal_spin_unlock_irq_restore(&alloc_list->lock, &lock_flag);
        hal_ring_set_entries(ring_ctl, devva);
        succ_add_num++;
    }

    return succ_add_num;
}



oal_void hi1106_host_rx_add_buff(hal_host_device_stru *hal_device, uint8_t q_num)
{
    uint16_t                     add_num;
    unsigned long                flags = 0;
    uint32_t                     buff_size;
    hal_host_ring_ctl_stru      *ring_ctl = NULL;
    uint32_t                     mask;
    hal_host_rx_alloc_list_stru *alloc_list = NULL;

    if (!oal_pcie_link_state_up()) {
        oam_error_log0(0, OAM_SF_RX, "{hi1106_host_rx_add_buff::pcie link down!}");
        return;
    }

    if (q_num == HAL_RX_DSCR_NORMAL_PRI_QUEUE) {
        mask = HI1106_MAC_RPT_HOST_INTR_NORM_RING_EMPTY_MASK;
        ring_ctl = &hal_device->st_host_rx_normal_free_ring;
        buff_size = (hal_device->st_alrx.en_al_rx_flag ? ALRX_NETBUF_SIZE : LARGE_NETBUF_SIZE) + HAL_RX_DSCR_LEN;
        alloc_list = &hal_device->host_rx_normal_alloc_list;
    } else if (q_num == HAL_RX_DSCR_SMALL_QUEUE) {
        mask = HI1106_MAC_RPT_HOST_INTR_SMALL_RING_EMPTY_MASK;
        ring_ctl = &hal_device->st_host_rx_small_free_ring;
        buff_size = SMALL_NETBUF_SIZE + HAL_RX_DSCR_LEN;
        alloc_list = &hal_device->host_rx_small_alloc_list;
    } else {
        oam_warning_log1(0, 0, "invalid queue num[%d] in rx irq", q_num);
        return;
    }

    /* �����󲹳�Free Ring�ڴ� */
    oal_spin_lock_irq_save(&(hal_device->st_free_ring_lock), &flags);
    add_num = hi1106_host_rx_add_buff_alloc(hal_device, ring_ctl, buff_size, alloc_list);
    if (add_num > 0) {
        hal_ring_set_sw2hw(ring_ctl);
    }
    oal_spin_unlock_irq_restore(&hal_device->st_free_ring_lock, &flags);

    return;
}


uint32_t hi1106_host_rx_reset_smac_handler(frw_event_mem_stru *event_mem)
{
    frw_event_stru              *event      = NULL;
    frw_event_hdr_stru          *event_hdr  = NULL;
    hal_host_device_stru        *hal_dev    = NULL;
    hal_host_rx_alloc_list_stru *alloc_list = NULL;

    if (event_mem == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hi1106_host_rx_reset_smac_handler::vent_mem null.}");
        return OAL_PTR_NULL;
    }

    /* ��ȡ�¼�ͷ���¼��ṹ��ָ�� */
    event = frw_get_event_stru(event_mem);
    event_hdr = &event->st_event_hdr;
    hal_dev = hal_get_host_device(event_hdr->uc_device_id);
    if (hal_dev == NULL) {
        oam_error_log1(0, OAM_SF_ANY,
                       "hi1106_host_rx_reset_smac_handler: hal device[%d] null.",
                       event_hdr->uc_device_id);
        return OAL_PTR_NULL;
    }
    alloc_list = &hal_dev->host_rx_normal_alloc_list;
    hi1106_rx_alloc_list_free(hal_dev, alloc_list);

    return OAL_SUCC;
}


uint32_t hi1106_rx_mpdu_que_len(hal_host_device_stru *hal_device)
{
    unsigned long lock_flag;
    uint32_t que_len;
    oal_netbuf_head_stru *netbuf_head = OAL_PTR_NULL;
    hal_rx_mpdu_que *rx_mpdu;

    rx_mpdu = &hal_device->st_rx_mpdu;
    oal_spin_lock_irq_save(&rx_mpdu->st_spin_lock, &lock_flag);

    /* ��ȡ�������е�ǰ�����MPDU��Ŀ */
    netbuf_head = &(rx_mpdu->ast_rx_mpdu_list[(rx_mpdu->cur_idx + 1) % RXQ_NUM]);
    que_len = oal_netbuf_list_len(netbuf_head);
    oal_spin_unlock_irq_restore(&rx_mpdu->st_spin_lock, &lock_flag);

    return que_len;
}

static inline uint64_t hi1106_rx_ring_regs_addr_transfer(uint32_t reg_addr)
{
    uint64_t host_va = 0;
    if (OAL_SUCC == oal_pcie_devca_to_hostva(0, reg_addr, &host_va)) {
        return host_va;
    } else {
        oam_error_log1(0, OAM_SF_CFG, "{ring_ctl_regs_addr_transfer::addr[%x] transfer fail}", reg_addr);
        return 0;
    }
}

uint8_t hi1106_rx_ring_reg_init(hal_host_device_stru *hal_dev, hal_host_ring_ctl_stru *rctl, uint8_t ring_id)
{
    if ((hal_dev == NULL) || (rctl == NULL) || (ring_id >= HAL_RING_ID_BUTT)) {
        oam_error_log0(0, OAM_SF_CFG, "{hal_rx_ring_reg_init::null input}");
        return OAL_FALSE;
    }

    rctl->base_lsb      = hi1106_rx_ring_regs_addr_transfer(g_ring_reg_tbl[ring_id].base_lsb);
    rctl->base_msb      = hi1106_rx_ring_regs_addr_transfer(g_ring_reg_tbl[ring_id].base_msb);
    rctl->ring_size_reg = hi1106_rx_ring_regs_addr_transfer(g_ring_reg_tbl[ring_id].size);
    rctl->write_ptr_reg = hi1106_rx_ring_regs_addr_transfer(g_ring_reg_tbl[ring_id].wptr_reg);
    rctl->read_ptr_reg  = hi1106_rx_ring_regs_addr_transfer(g_ring_reg_tbl[ring_id].rptr_reg);

    if (!((rctl->base_lsb) && (rctl->base_msb) && (rctl->ring_size_reg) &&
        (rctl->write_ptr_reg) && (rctl->read_ptr_reg))) {
        oam_error_log2(0, OAM_SF_CFG, "{hi1106_rx_ring_reg_init::cfg NULL! base_lsb[0x%x] base_msb[0x%x]}",
            rctl->base_lsb, rctl->base_msb);
        oam_error_log3(0, OAM_SF_CFG, "{hi1106_rx_ring_reg_init::ring_size_reg[0x%x] wptr_reg[0x%x] rptr_reg[0x%x]}",
            rctl->ring_size_reg, rctl->write_ptr_reg, rctl->read_ptr_reg);
        return OAL_FALSE;
    }
    if (ring_id < HAL_RING_TYPE_COMP) {
        rctl->cfg_read_ptr_reg = hi1106_rx_ring_regs_addr_transfer(g_ring_reg_tbl[ring_id].cfg_rptr_reg);
        return !!(rctl->cfg_read_ptr_reg);
    } else {
        if (g_ring_reg_tbl[ring_id].len) {
            rctl->len_reg  = hi1106_rx_ring_regs_addr_transfer(g_ring_reg_tbl[ring_id].len);
            rctl->cfg_write_ptr_reg = hi1106_rx_ring_regs_addr_transfer(g_ring_reg_tbl[ring_id].cfg_wptr_reg);
            return ((rctl->len_reg) && (rctl->cfg_write_ptr_reg));
        } else {
            rctl->cfg_write_ptr_reg = hi1106_rx_ring_regs_addr_transfer(g_ring_reg_tbl[ring_id].cfg_wptr_reg);
            return OAL_TRUE;
        }
    }
}


oal_netbuf_stru *hi1106_alloc_list_delete_netbuf(hal_host_device_stru *hal_dev,
    hal_host_rx_alloc_list_stru *alloc_list, dma_addr_t host_iova, uint32_t *pre_num)
{
    oal_netbuf_stru             *next_skb       = NULL;
    oal_netbuf_stru             *skb            = NULL;
    pcie_cb_dma_res             *cb_res         = NULL;
    oal_netbuf_head_stru        *list           = &alloc_list->list;
    oal_pcie_linux_res          *pcie_linux_res = oal_get_pcie_linux_res();

    if (pcie_linux_res == NULL) {
        oam_error_log0(0, 0, "hi1106_alloc_list_delete_netbuf:pcie_linux_res null");
        return NULL;
    }
    oal_netbuf_search_for_each_safe(skb, next_skb, list) {
        cb_res = (pcie_cb_dma_res *)oal_netbuf_cb(skb);
        if (cb_res->paddr.addr == host_iova) {
            /* �ͷ�IO��Դ */
            dma_unmap_single(&pcie_linux_res->pst_pcie_dev->dev, cb_res->paddr.addr,
                skb->len, hal_dev->rx_nodes->dma_dir);
            __netbuf_unlink(skb, list);
            return skb;
        }
        /* pre_num��������һ�����޺�����������ƣ�TO BE DONE */
        (*pre_num)++;
    }
    return NULL;
}


void hi1106_init_alloc_list(hal_host_device_stru *hal_dev, hal_host_rx_alloc_list_stru *alloc_list)
{
    oal_netbuf_head_stru    *list = &alloc_list->list;
    uint32_t                 len;

    oal_spin_lock_init(&alloc_list->lock);

    len = oal_netbuf_list_len(list);
    if ((oal_netbuf_peek(list) != NULL) || (len != 0)) {
        oam_error_log1(hal_dev->device_id, OAM_SF_RX, "hi1106_init_alloc_list:list len[%d] not empty", len);
        hi1106_rx_alloc_list_free(hal_dev, alloc_list);
    }
    oal_netbuf_head_init(list);
}

OAL_STATIC int32_t  hi1106_rx_host_init_small_free_ring(hal_host_device_stru *pst_device, void *pnetdev)
{
    uint16_t entries;
    hal_host_ring_ctl_stru *rx_ring_ctl = NULL;
    uint32_t alloc_size;
    dma_addr_t rbase_dma_addr = 0;
    void *rbase_vaddr = NULL;
    uint64_t devva = 0;
    int32_t ret;
    oal_pcie_linux_res  *pcie_linux_res = oal_get_pcie_linux_res();

    if (pcie_linux_res == NULL) {
        return OAL_FAIL;
    }

    rx_ring_ctl = &(pst_device->st_host_rx_small_free_ring);
    memset_s(rx_ring_ctl, OAL_SIZEOF(hal_host_ring_ctl_stru), 0, OAL_SIZEOF(hal_host_ring_ctl_stru));

    entries = (pst_device->device_id == 0) ? HAL_RX_MSDU_SMALL_RING_NUM0 : HAL_RX_MSDU_SMALL_RING_NUM0;
    alloc_size = (entries * OAL_SIZEOF(uint64_t));
    rbase_vaddr = dma_alloc_coherent(&pcie_linux_res->pst_pcie_dev->dev, alloc_size, &rbase_dma_addr, GFP_KERNEL);
    if (rbase_vaddr == NULL) {
        oam_error_log2(0, 0, "normal_free:dma_alloc fail[%p]size[%d]", &pcie_linux_res->pst_pcie_dev->dev, alloc_size);
        return OAL_FAIL;
    }

    ret = pcie_if_hostca_to_devva(0, (uint64_t)rbase_dma_addr, &devva);
    if (ret != OAL_SUCC) {
        oam_error_log0(0, 0, "hi1106_rx_host_init_normal_free_ring alloc pcie_if_hostca_to_devva fail.");
        return OAL_FAIL;
    }

    rx_ring_ctl->entries = entries;
    rx_ring_ctl->p_entries = (uint64_t *)rbase_vaddr;
    rx_ring_ctl->entry_size = HAL_RX_ENTRY_SIZE;
    rx_ring_ctl->ring_type = HAL_RING_TYPE_FREE_RING;
    rx_ring_ctl->devva = devva;

    if (OAL_FALSE == hi1106_rx_ring_reg_init(pst_device, rx_ring_ctl, HAL_RING_TYPE_S_F)) {
        oam_error_log0(0, 0, "hi1106_rx_host_init_normal_free_ring alloc pcie_if_hostca_to_devva fail.");
        return OAL_FAIL;
    }
    hi1106_init_alloc_list(pst_device, &pst_device->host_rx_small_alloc_list);
    return OAL_SUCC;
}

OAL_STATIC int32_t  hi1106_rx_host_init_normal_free_ring(hal_host_device_stru *pst_device, void *pnetdev)
{
    uint16_t entries;
    hal_host_ring_ctl_stru *rx_ring_ctl = NULL;
    uint32_t alloc_size;
    void *rbase_vaddr = NULL;
    uint64_t devva = 0;
    dma_addr_t rbase_dma_addr = 0;
    oal_pcie_linux_res *pcie_linux_res = oal_get_pcie_linux_res();

    if (pcie_linux_res == NULL) {
        return OAL_FAIL;
    }

    rx_ring_ctl = &(pst_device->st_host_rx_normal_free_ring);
    memset_s(rx_ring_ctl, OAL_SIZEOF(hal_host_ring_ctl_stru), 0, OAL_SIZEOF(hal_host_ring_ctl_stru));

    entries = (pst_device->device_id == 0) ?
        HAL_RX_MSDU_NORMAL_RING_NUM0 : HAL_RX_MSDU_NORMAL_RING_NUM0;
    alloc_size = (entries * OAL_SIZEOF(uint64_t));

    rbase_vaddr = dma_alloc_coherent(&pcie_linux_res->pst_pcie_dev->dev, alloc_size, &rbase_dma_addr, GFP_KERNEL);
    if (rbase_vaddr == NULL) {
        oam_error_log0(0, 0, "hal_rx_host_init_normal_free_ring alloc pcie_linux_res  null.");
        return OAL_FAIL;
    }

    if (OAL_SUCC != pcie_if_hostca_to_devva(0, (uint64_t)rbase_dma_addr, &devva)) {
        oam_error_log0(0, 0, "hal_rx_host_init_normal_free_ring alloc pcie_if_hostca_to_devva fail.");
        return OAL_FAIL;
    }

    rx_ring_ctl->entries    = entries;
    rx_ring_ctl->p_entries  = (uint64_t *)rbase_vaddr;
    rx_ring_ctl->entry_size = HAL_RX_ENTRY_SIZE;
    rx_ring_ctl->ring_type  = HAL_RING_TYPE_FREE_RING;
    rx_ring_ctl->devva = devva;

    if (OAL_FALSE == hi1106_rx_ring_reg_init(pst_device, rx_ring_ctl, HAL_RING_TYPE_L_F)) {
        oam_error_log0(0, 0, "hal_rx_host_init_normal_free_ring alloc pcie_if_hostca_to_devva fail.");
        return OAL_FAIL;
    }
    hi1106_init_alloc_list(pst_device, &pst_device->host_rx_normal_alloc_list);
    oam_warning_log2(0, 0, "hal_rx_host_init_normal_free_ring :base[%x].size[%d]", devva, entries);
    return OAL_SUCC;
}

OAL_STATIC int32_t  hi1106_rx_host_init_complete_ring(hal_host_device_stru *pst_device, void *pnetdev)
{
    uint16_t entries;
    hal_host_ring_ctl_stru *rx_ring_ctl = NULL;
    uint32_t alloc_size;
    dma_addr_t rbase_dma_addr = 0;
    void *rbase_vaddr = NULL;
    uint64_t devva;
    oal_pcie_linux_res *pcie_linux_res = oal_get_pcie_linux_res();

    if (pcie_linux_res == NULL) {
        return OAL_FAIL;
    }

    oal_spin_lock_init(&pst_device->st_free_ring_lock);
    rx_ring_ctl = &(pst_device->st_host_rx_complete_ring);
    memset_s(rx_ring_ctl, OAL_SIZEOF(hal_host_ring_ctl_stru), 0, OAL_SIZEOF(hal_host_ring_ctl_stru));

    entries = HAL_RX_COMPLETE_RING_MAX;
    alloc_size = (entries * OAL_SIZEOF(uint64_t));
    rbase_vaddr = dma_alloc_coherent(&pcie_linux_res->pst_pcie_dev->dev, alloc_size, &rbase_dma_addr, GFP_KERNEL);
    if (rbase_vaddr == NULL) {
        return OAL_FAIL;
    }

    if (OAL_SUCC != pcie_if_hostca_to_devva(0, (uint64_t)rbase_dma_addr, &devva)) {
        return OAL_FAIL;
    }

    rx_ring_ctl->entries = entries;
    rx_ring_ctl->p_entries = (uint64_t *)rbase_vaddr;
    rx_ring_ctl->entry_size = HAL_RX_ENTRY_SIZE;
    rx_ring_ctl->ring_type = HAL_RING_TYPE_COMPLETE_RING;
    rx_ring_ctl->devva = devva;

    if (OAL_FALSE == hi1106_rx_ring_reg_init(pst_device, rx_ring_ctl, HAL_RING_TYPE_COMP)) {
        oam_error_log0(0, 0, "hal_rx_host_init_complete_ring alloc pcie_if_hostca_to_devva  fail.");
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

OAL_STATIC void  hi1106_host_init_ba_info_ring(hal_host_device_stru *pst_device)
{
    hal_host_ring_ctl_stru *rx_ring_ctl = NULL;
    uint32_t alloc_size;
    dma_addr_t rbase_dma_addr = 0;
    oal_uint *rbase_vaddr = NULL;
    uint64_t devva = 0;
    oal_pcie_linux_res *pcie_linux_res = oal_get_pcie_linux_res();

    if (pcie_linux_res == NULL) {
        oam_error_log0(0, 0, "hi1106_host_init_ba_info_ring::pcie_linux_res null!");
        return;
    }

    rx_ring_ctl = &(pst_device->st_host_tx_ba_info_ring);
    memset_s(rx_ring_ctl, OAL_SIZEOF(hal_host_ring_ctl_stru), 0, OAL_SIZEOF(hal_host_ring_ctl_stru));

    alloc_size = (HAL_BA_INFO_COUNT * BA_INFO_DATA_SIZE);
    rbase_vaddr = dma_alloc_coherent(&pcie_linux_res->pst_pcie_dev->dev, alloc_size, &rbase_dma_addr, GFP_KERNEL);
    if (rbase_vaddr == NULL) {
        oam_error_log0(0, 0, "hi1106_host_init_ba_info_ring::rbase_vaddr null!");
        return;
    }

    if (OAL_SUCC != pcie_if_hostca_to_devva(0, (uint64_t)rbase_dma_addr, &devva)) {
        oam_error_log0(0, 0, "hi1106_host_init_ba_info_ring::hostca_to_devva fail!");
        return;
    }

    rx_ring_ctl->entries = HAL_BA_INFO_COUNT;
    rx_ring_ctl->p_entries = (uint64_t *)rbase_vaddr;
    rx_ring_ctl->entry_size = BA_INFO_DATA_SIZE;
    rx_ring_ctl->ring_type = HAL_RING_TYPE_COMPLETE_RING;
    rx_ring_ctl->devva = devva;

    if (hi1106_rx_ring_reg_init(pst_device, rx_ring_ctl, TX_BA_INFO_RING) == OAL_FALSE) {
        oam_error_log0(0, 0, "hi1106_host_init_ba_info_ring ring init fail.");
    }
}

int32_t hi1106_rx_host_init_dscr_queue(oal_net_device_stru *pst_net_dev, uint8_t hal_dev_id)
{
    hal_host_device_stru *hal_device = hal_get_host_device(hal_dev_id);

    if ((hal_device == NULL) || (hal_dev_id >= WLAN_DEVICE_MAX_NUM_PER_CHIP)) {
        oam_error_log1(0, 0, "hal_rx_host_init_dscr_queue::hal dev null. id[%d].", hal_dev_id);
        return OAL_FAIL;
    }

    if (hal_device->inited) {
        oam_warning_log0(0, 0, "hal_rx_host_init_dscr_queue::already inited.");
        return OAL_SUCC;
    }

    /* host����device�Ĵ����ĵ�ַӳ����� */
    hi1106_host_regs_addr_init(hal_device);

    /* ��ʼ������Ӳ������ */
    if (OAL_SUCC != hi1106_rx_host_init_normal_free_ring(hal_device, (void*)pst_net_dev)) {
        oam_error_log0(0, 0, "hal_rx_host_init_dscr_queue::init_normal_free_ring fail.");
        return OAL_FAIL;
    }

    if (OAL_SUCC != hi1106_rx_host_init_small_free_ring(hal_device, (void*)pst_net_dev)) {
        oam_error_log0(0, 0, "hal_rx_host_init_dscr_queue::init_small_free_ring fail.");
        return OAL_FAIL;
    }

    if (OAL_SUCC != hi1106_rx_host_init_complete_ring(hal_device, (void*)pst_net_dev)) {
        oam_error_log0(0, 0, "hal_rx_host_init_dscr_queue::init_complete_ring fail.");
        return OAL_FAIL;
    }

    oal_atomic_set(&hal_device->bh_need_refill, OAL_FALSE);
    hal_device->inited = OAL_TRUE;
    oam_warning_log0(0, 0, "hal_rx_host_init_dscr_queue::inited succ.");

    return OAL_SUCC;
}

OAL_STATIC void  hi1106_rx_host_destory_free_ring(hal_host_ring_ctl_stru *rx_ring_ctl)
{
}

OAL_STATIC void  hi1106_rx_host_destory_comp_ring(hal_host_ring_ctl_stru *rx_ring_ctl)
{
}

void hi1106_rx_set_ring_regs(hal_host_ring_ctl_stru *rx_ring_ctl)
{
    if (rx_ring_ctl->devva == 0) {
        oam_error_log2(0, 0, "hi1106_rx_set_ring_regs::ring_type %d, subtype %d!",
            rx_ring_ctl->ring_type, (uintptr_t)rx_ring_ctl->ring_subtype);
        return;
    }

    oal_writel(GET_LOW_32_BITS(rx_ring_ctl->devva), (uintptr_t)rx_ring_ctl->base_lsb);
    oal_writel(GET_HIGH_32_BITS(rx_ring_ctl->devva), (uintptr_t)rx_ring_ctl->base_msb);
    oal_writel(rx_ring_ctl->entries, (uintptr_t)rx_ring_ctl->ring_size_reg);

    rx_ring_ctl->un_read_ptr.read_ptr = 0;
    rx_ring_ctl->un_write_ptr.write_ptr = 0;
}

int32_t hi1106_rx_host_start_dscr_queue(oal_net_device_stru *pst_net_dev, uint8_t hal_dev_id)
{
    hal_host_device_stru *hal_device  = hal_get_host_device(hal_dev_id);

    if (OAL_SUCC != hi1106_rx_host_init_dscr_queue(pst_net_dev, hal_dev_id)) {
        return OAL_FAIL;
    }

    /* LEN REG keep default */
    hi1106_rx_set_ring_regs(&(hal_device->st_host_rx_normal_free_ring));
    hi1106_rx_set_ring_regs(&(hal_device->st_host_rx_small_free_ring));
    hi1106_rx_set_ring_regs(&(hal_device->st_host_rx_complete_ring));

    if (hal_device->st_alrx.en_al_rx_flag) {
        if (!hal_device->st_host_rx_complete_ring.len_reg) {
            oam_error_log0(0, 0, "rx_host_start_dscr_queue::len_reg not inited!");
            return OAL_FAIL;
        }
        oal_writel(((ALRX_NETBUF_SIZE + HAL_RX_DSCR_LEN) | (HAL_RX_DSCR_LEN << NUM_16_BITS)),
            (uintptr_t)hal_device->st_host_rx_complete_ring.len_reg);
    }

    /* ������֡normal free ring����ڴ� */
    hi1106_host_rx_add_buff(hal_device, HAL_RX_DSCR_NORMAL_PRI_QUEUE);
    return OAL_SUCC;
}

int32_t hi1106_rx_host_stop_dscr_queue(oal_net_device_stru *pst_net_dev, uint8_t hal_dev_id)
{
    hal_host_device_stru   *hal_device  = hal_get_host_device(hal_dev_id);

    /* ����hostȥreset ring cfg regs�����⴦����Ҫ��ͬ������ */
    /* todo �ͷţ� upmap skbs */
    hi1106_rx_host_destory_free_ring(&hal_device->st_host_rx_small_free_ring);
    hi1106_rx_host_destory_free_ring(&hal_device->st_host_rx_normal_free_ring);
    hi1106_rx_host_destory_comp_ring(&hal_device->st_host_rx_complete_ring);

    return OAL_SUCC;
}

void hi1106_host_tx_intr_init(uint8_t hal_dev_id)
{
    hal_host_device_stru *hal_device  = hal_get_host_device(hal_dev_id);
    if (hal_device == NULL) {
        oam_error_log0(0, 0, "hi1106_host_tx_intr_init::haldev null!");
        return;
    }

    oam_warning_log1(0, 0, "hi1106_host_tx_intr_init::init state[%d]!", hal_device->tx_inited);
    if (!hal_device->tx_inited) {
        /* host����device�Ĵ����ĵ�ַӳ����� */
        hi1106_host_regs_addr_init(hal_device);

        hi1106_host_init_ba_info_ring(hal_device);
    }

    hi1106_rx_set_ring_regs(&(hal_device->st_host_tx_ba_info_ring));
    hal_device->tx_inited = OAL_TRUE;
    oam_warning_log0(0, 0, "hi1106_host_tx_intr_init::host tx comp init!");
}


void hi1106_tx_ba_info_dscr_get(uint8_t *ba_info_data, uint32_t size, hal_tx_ba_info_stru *tx_ba_info)
{
    hi1106_tx_ba_info_dscr_stru *tx_ba_info_dscr = (hi1106_tx_ba_info_dscr_stru *)ba_info_data;
    uint8_t tid = tx_ba_info_dscr->bit_tid;

    tx_ba_info[tid].user_id = tx_ba_info_dscr->bit_user_id;
    tx_ba_info[tid].tid = tx_ba_info_dscr->bit_tid;
    tx_ba_info[tid].ba_info_vld = tx_ba_info_dscr->bit_ba_info_vld;
}


#endif
