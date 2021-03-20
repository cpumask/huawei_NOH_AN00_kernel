

#include "oal_pcie_linux.h"
#include "oal_util.h"
#include "oal_net.h"
#include "oal_ext_if.h"
#include "oam_ext_if.h"
#include "hal_common_ops.h"
#include "hal_ring.h"
#include "securec.h"
#include "hal_device.h"
#include "hal_irq.h"
#include "hal_mac.h"
#include "hal_dscr.h"
#include "hi1106_common_ops.h"
#include "oal_pcie_host.h"
#include "oam_ext_if.h"
#include "wlan_spec.h"
#include "mac_frame.h"
#include "hmac_rx_data.h"
#include "hmac_user.h"
#include "hmac_ext_if.h"
#include "hmac_config.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HAL_IRQ_C

#ifdef CONFIG_ARCH_KIRIN_PCIE

static uint32_t hi1106_get_skb_from_ring_entry(uint64_t hw_dscr_addr, hal_host_ring_ctl_stru *ring_ctl,
    hal_host_device_stru *hal_dev, oal_netbuf_stru **pp_skb)
{
    int32_t                      ret;
    OAL_STATIC uint64_t          dscr_prev   = 0;
    dma_addr_t                   host_iova   = 0;
    uint32_t                     pre_num     = 0;
    hal_host_rx_alloc_list_stru *alloc_list  = &hal_dev->host_rx_normal_alloc_list; /* 目前仅用normal_list */

    if (oal_unlikely(hw_dscr_addr == 0)) {
        oam_error_log1(0, OAM_SF_RX, "{hal_get_skb_from_ring_entry:rd_idx[%d], addr is zero!}",
            ring_ctl->un_read_ptr.st_read_ptr.bit_read_ptr);
        return OAL_FAIL;
    }

    if (dscr_prev == hw_dscr_addr) {
        oam_error_log1(0, OAM_SF_RX, "{hal_get_skb_from_ring_entry:duplicate dscr 0x%x!}", hw_dscr_addr);
    }

    dscr_prev = hw_dscr_addr;
    ret = pcie_if_devva_to_hostca(0, dscr_prev, (uint64_t *)&host_iova);
    if (ret != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_RX, "{hal_get_skb_from_ring_entry:devva2hostca fail.}");
        return OAL_FAIL;
    }

    /* 中断下半部补充netbuf可能也会操作此链表 */
    oal_spin_lock(&alloc_list->lock);
    *pp_skb = hi1106_alloc_list_delete_netbuf(hal_dev, alloc_list, host_iova, &pre_num);
    oal_spin_unlock(&alloc_list->lock);

    /* 后续加入全局统计，封装统计接口 */
    if (pre_num) {
        oam_warning_log1(hal_dev->device_id, OAM_SF_RX,
            "hi1106_get_skb_from_ring_entry:not first netbuf, pre netbuf num[%d]", pre_num);
    }
    if (*pp_skb == NULL) {
        oam_error_log0(hal_dev->device_id, OAM_SF_RX, "hi1106_get_skb_from_ring_entry:not found skb");
        return OAL_FAIL;
    }
    return OAL_SUCC;
}


uint32_t hi1106_host_rx_ring_entry_get(hal_host_device_stru *hal_dev,
    hal_host_ring_ctl_stru *ring_ctl, oal_netbuf_head_stru *netbuf_head)
{
    uint16_t             count;
    uint16_t             rd_idx;
    uint64_t             hw_dscr_addr;
    uint32_t             ret;
    oal_netbuf_stru     *netbuf = OAL_PTR_NULL;

    if (oal_unlikely(oal_any_null_ptr3(hal_dev, ring_ctl, netbuf_head))) {
        oam_error_log0(0, OAM_SF_RX, "{hal_host_rx_ring_entry_get::input para null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 从HAL获取ring元素的个数 */
    ret = hal_ring_get_entry_count(ring_ctl, &count);
    if (ret != OAL_SUCC) {
        oam_error_log1(0, OAM_SF_RX, "{hal_host_rx_ring_entry_get, return code %d.}", ret);
        return ret;
    }

    /* 循环处理每一个元素 */
    while (count) {
        hw_dscr_addr = 0;
        rd_idx = ring_ctl->un_read_ptr.st_read_ptr.bit_read_ptr;
        if (oal_unlikely(ring_ctl->p_entries[rd_idx]) == 0) {
            /* 如果读出的地址不正确，ERROR上报SDT，需要调整delay的时间 */
            oam_error_log2(0, OAM_SF_RX, "hal_host_rx_ring_entry_get:addr:0x%x, rptr:%u.",
                ring_ctl->p_entries[rd_idx], rd_idx);
            break;
        }

        /* 读取ring上的元素 */
        ret = hal_ring_get_entries(ring_ctl, (uint8_t *)&hw_dscr_addr, 1);
        if (oal_unlikely(ret != OAL_SUCC)) {
            oam_error_log1(0, OAM_SF_RX, "{hal_host_rx_ring_entry_get, return code:%d.}", ret);
            break;
        }

        /* 从物理地址获取skb地址 */
        ret = hi1106_get_skb_from_ring_entry(hw_dscr_addr,  ring_ctl, hal_dev, &netbuf);
        if (oal_unlikely(ret != OAL_SUCC)) {
            count--;
            continue;
        }

        oal_netbuf_add_to_list_tail(netbuf, netbuf_head);
        count--;
    }

    /* 更新complete ring的rptr指针 */
    return hal_ring_set_sw2hw(ring_ctl);
}


uint32_t hi1106_host_rx_mpdu_que_push(hal_rx_mpdu_que *rx_mpdu,
    oal_netbuf_head_stru *netbuf_head)
{
    oal_netbuf_head_stru *rx_que = NULL;
    unsigned long         lock_flag;

    oal_spin_lock_irq_save(&rx_mpdu->st_spin_lock, &lock_flag);
    rx_que = &(rx_mpdu->ast_rx_mpdu_list[rx_mpdu->cur_idx % RXQ_NUM]);

    /* 防止缓存过多的数据包，导致内存不足 */
    if (oal_netbuf_list_len(rx_que) > rx_mpdu->max_buff_num) {
        oal_spin_unlock_irq_restore(&rx_mpdu->st_spin_lock, &lock_flag);
        return OAL_FAIL;
    }

    oal_netbuf_queue_splice_tail_init(netbuf_head, rx_que);
    oal_spin_unlock_irq_restore(&rx_mpdu->st_spin_lock, &lock_flag);

    return OAL_SUCC;
}


oal_void hi1106_host_rx_mpdu_list_build(hal_host_device_stru *hal_device,
    oal_netbuf_head_stru *list)
{
}


OAL_STATIC oal_void  hi1106_irq_host_rx_complete_notify(hal_host_device_stru *hal_device)
{
    frw_event_stru           *event = NULL;
    frw_event_mem_stru       *event_mem = NULL;
    hal_host_rx_event        *wlan_rx_event = NULL;

    event_mem = frw_event_alloc_m(OAL_SIZEOF(hal_host_rx_event));
    if (event_mem == NULL) {
        return;
    }

    event = frw_get_event_stru(event_mem);
    frw_event_hdr_init(&(event->st_event_hdr),
                       FRW_EVENT_TYPE_HOST_DDR_DRX,
                       HAL_WLAN_DDR_DRX_EVENT_SUBTYPE,
                       OAL_SIZEOF(hal_host_rx_event),
                       FRW_EVENT_PIPELINE_STAGE_0,
                       0, hal_device->device_id, 0);

    wlan_rx_event = (hal_host_rx_event *)event->auc_event_data;
    wlan_rx_event->hal_dev = hal_device;
    frw_event_dispatch_event(event_mem);
    frw_event_free_m(event_mem);
    return;
}


OAL_STATIC oal_void hi1106_irq_host_rx_complete_isr(hal_host_device_stru *hal_device, uint32_t mask)
{
    uint32_t                     ret;
    oal_netbuf_head_stru         rx_msdu_que;
    hal_rx_mpdu_que             *rx_mpdu = NULL;

    if (oal_unlikely(hal_device == NULL)) {
        return;
    }

    rx_mpdu = &(hal_device->st_rx_mpdu);
    oal_atomic_set(&hal_device->bh_need_refill, OAL_TRUE);
    oal_netbuf_list_head_init(&rx_msdu_que);
    /* 从complete ring上读取元素，并转换为skb结构 */
    ret = hi1106_host_rx_ring_entry_get(hal_device, &hal_device->st_host_rx_complete_ring, &rx_msdu_que);
    if (ret != OAL_SUCC) {
        hi1106_clear_host_mac_int_status(hal_device, mask);
        hi1106_host_rx_mpdu_list_build(hal_device, &rx_msdu_que);
        return;
    }

    /* 将接收到的MSDU报文放入接收报文队列 */
    ret = hi1106_host_rx_mpdu_que_push(rx_mpdu, &rx_msdu_que);
    if (ret != OAL_SUCC) {
        hi1106_host_rx_mpdu_list_build(hal_device, &rx_msdu_que);
    }

    hi1106_clear_host_mac_int_status(hal_device, mask);
    /* 如果工作队列没有数据，通知下半部取数据进行处理 */
    if (hi1106_rx_mpdu_que_len(hal_device) == 0) {
        hi1106_irq_host_rx_complete_notify(hal_device);
    }
}


oal_void hi1106_irq_host_rx_ring_empty_isr(hal_host_device_stru *hal_dev, uint32_t mask,
    hal_rx_dscr_queue_id_enum_uint8 rx_qid)
{
    hi1106_clear_host_mac_int_status(hal_dev, mask);
    /* 设置下半部补内存标志 */
    oal_atomic_set(&hal_dev->bh_need_refill, OAL_TRUE);
}


oal_void hi1106_irq_host_tx_complete_isr(hal_host_device_stru *hal_dev, uint32_t mask)
{
    frw_event_stru           *event = NULL;
    frw_event_mem_stru       *event_mem = NULL;
    hal_host_rx_event  *wlan_tx_event = NULL;
    uint32_t ret;

    hi1106_clear_host_mac_int_status(hal_dev, mask);

    if (!hal_dev->tx_inited) {
        oam_error_log0(0, OAM_SF_RX, "irq_host_tx_complete_isr:: host tx int not inited.\n");
    }

    event_mem = frw_event_alloc_m(OAL_SIZEOF(hal_host_rx_event));
    if (event_mem == NULL) {
        oam_error_log0(0, OAM_SF_RX, "irq_host_tx_complete_isr:: MEM alloc fail.\n");
        return;
    }

    event = frw_get_event_stru(event_mem);
    frw_event_hdr_init(&(event->st_event_hdr),
                       FRW_EVENT_TYPE_WLAN_TX_COMP,
                       HAL_TX_COMP_SUB_TYPE_TX,
                       OAL_SIZEOF(hal_host_rx_event),
                       FRW_EVENT_PIPELINE_STAGE_0,
                       0, hal_dev->device_id, 0);

    wlan_tx_event = (hal_host_rx_event *)event->auc_event_data;
    wlan_tx_event->hal_dev = hal_dev;

    ret = frw_event_dispatch_event(event_mem);
    if (ret != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_RX, "{irq_host_tx_complete_isr failed.}\r\n");
        frw_event_free_m(event_mem);
    }
}

oal_void hi1106_irq_host_mac_isr(hal_host_device_stru *hal_dev)
{
    uint32_t               state = 0;
    uint32_t               mask = 0;
    host_intr_status_union int_state;

    if (hal_dev == NULL) {
        oal_io_print("irq_host_mac_isr:: input para err.\n");
        return;
    }

    if (!hal_dev->inited) {
        oal_io_print("irq_host_mac_isr:: not inited.\n");
    }

    /* 读取MAC中断状态,中断mask寄存器 */
    hi1106_get_host_mac_int_status(hal_dev, &state);
    hi1106_get_host_mac_int_mask(hal_dev, &mask);

    /* 一次处理所有中断 */
    state &= ~mask;
    int_state.u32 = state;
    while (state) {
        /* 保留最低置1位，其它位为0 */
        int_state.u32 = (uint32_t)((uint32_t)(-state) & state);  //lint !e501
        state &= ~((uint32_t)int_state.u32);
        if (int_state.bits.rpt_host_intr_rx_complete != 0) {
            /* 接收数据报文中断 */
            hi1106_irq_host_rx_complete_isr(hal_dev, int_state.u32);
        } else if (int_state.bits.rpt_host_intr_tx_complete) {
            /* 发送完成上报中断 */
            hi1106_irq_host_tx_complete_isr(hal_dev, int_state.u32);
        } else if (int_state.bits.rpt_host_intr_norm_ring_empty != 0) {
            hi1106_irq_host_rx_ring_empty_isr(hal_dev, int_state.u32, HAL_RX_DSCR_NORMAL_PRI_QUEUE);
        } else if (int_state.bits.rpt_host_intr_small_ring_empty != 0) {
            hi1106_irq_host_rx_ring_empty_isr(hal_dev, int_state.u32, HAL_RX_DSCR_SMALL_QUEUE);
        } else if (int_state.bits.rpt_host_intr_ba_win_exceed != 0) {
            /* 接收到的BA窗口越界异常中断 */
            hi1106_clear_host_mac_int_status(hal_dev, int_state.u32);
        } else if (int_state.bits.rpt_host_intr_rx_ppdu_desc != 0) {
            /* RX PPDU描述符Complete Ring上报中断 */
            hi1106_clear_host_mac_int_status(hal_dev, int_state.u32);
        } else if (int_state.bits.rpt_host_intr_data_ring_overrun != 0) {
            /* 数据帧Complete Ring满写异常中断 */
            hi1106_clear_host_mac_int_status(hal_dev, int_state.u32);
        } else {
            oam_warning_log1(0, OAM_SF_RX, "{hal_irq_host_mac_isr error, state.u32:0x%x.}", int_state.u32);
            hi1106_clear_host_mac_int_status(hal_dev, int_state.u32);
        }
    }
    hi1106_clear_host_mac_int_status(hal_dev, int_state.u32);
}


oal_void hi1106_mac_irq_host_all(oal_void *p_dev)
{
    hal_host_device_stru *hal_dev = (hal_host_device_stru *)p_dev;
    hi1106_irq_host_mac_isr(hal_dev);
}


oal_void hi1106_irq_host_phy_isr(hal_host_device_stru *hal_dev)
{
    uint32_t               state = 0;
    uint32_t               mask = 0;
    phy_intr_rpt_union     int_state;

    if (hal_dev == NULL) {
        oal_io_print("host_phy_isr input para err\n");
        return;
    }

    /* 读取MAC中断状态寄存器 */
    hi1106_get_host_phy_int_status(hal_dev, &state);

    /* 读取中断mask寄存器 */
    hi1106_get_host_phy_int_mask(hal_dev, &mask);

    state &= ~mask;
    int_state.u32 = state;
    hi1106_clear_host_phy_int_status(hal_dev, int_state.u32);
}

void hi1106_phy_irq_host_all(void *p_dev)
{
    hal_host_device_stru *hal_dev = (hal_host_device_stru *)p_dev;
    hi1106_irq_host_phy_isr(hal_dev);
}


void hi1106_host_chip_irq_init(void)
{
    static pcie_wlan_callback_group_stru st_isr_cb;

    memset_s(&st_isr_cb, OAL_SIZEOF(pcie_wlan_callback_group_stru),
        0, OAL_SIZEOF(pcie_wlan_callback_group_stru));

    st_isr_cb.pcie_mac_2g_isr_callback.para = (void *)hal_get_host_device(0);
    st_isr_cb.pcie_mac_2g_isr_callback.pf_func = hi1106_mac_irq_host_all;
    st_isr_cb.pcie_phy_2g_isr_callback.para = (void *)hal_get_host_device(0);
    st_isr_cb.pcie_phy_2g_isr_callback.pf_func = hi1106_phy_irq_host_all;

    st_isr_cb.pcie_mac_5g_isr_callback.para = (void *)hal_get_host_device(1);
    st_isr_cb.pcie_mac_5g_isr_callback.pf_func = hi1106_mac_irq_host_all;
    st_isr_cb.pcie_phy_5g_isr_callback.para = (void *)hal_get_host_device(1);
    st_isr_cb.pcie_phy_5g_isr_callback.pf_func = hi1106_phy_irq_host_all;

    pcie_wlan_func_register(&st_isr_cb);
    oam_warning_log0(0, OAM_SF_RX, "{host chip irq registered succ.}");
}
#endif
