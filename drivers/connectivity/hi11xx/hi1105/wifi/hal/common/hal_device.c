

#if (_PRE_OS_VERSION == _PRE_OS_VERSION_LINUX)
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/jiffies.h>
#include <linux/wait.h>
#include <linux/workqueue.h>
#include <linux/kthread.h>

#include "board.h"
#endif

#include "oal_util.h"
#include "oal_net.h"
#include "oal_ext_if.h"
#include "oam_ext_if.h"
#include "hal_ring.h"
#include "securec.h"
#include "hal_device.h"
#include "hal_mac.h"
#include "hal_irq.h"
#include "hal_host_ext_if.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HAL_DEV_C

#ifdef CONFIG_ARCH_KIRIN_PCIE
hal_host_chip_stru  g_hal_chip;

uint8_t hal_rx_mpdu_que_is_full(hal_rx_mpdu_que *rx_mpdu)
{
    oal_netbuf_head_stru *pst_que = NULL;
    pst_que = &(rx_mpdu->ast_rx_mpdu_list[(rx_mpdu->cur_idx) % RXQ_NUM]);

    return (uint8_t)(oal_netbuf_list_len(pst_que) > rx_mpdu->max_buff_num);
}


oal_void hal_host_rx_mpdu_que_pop(hal_host_device_stru *hal_device,
    oal_netbuf_head_stru *netbuf_head)
{
    hal_rx_mpdu_que         *rx_mpdu = NULL;
    unsigned long            lock_flag;
    uint8_t                  next_q_idx;

    rx_mpdu = &(hal_device->st_rx_mpdu);

    oal_spin_lock_irq_save(&rx_mpdu->st_spin_lock, &lock_flag);
    next_q_idx = (rx_mpdu->cur_idx + 1) % RXQ_NUM;

    /* 取出skb，放在netbuf_head */
    oal_netbuf_queue_splice_tail_init(&(rx_mpdu->ast_rx_mpdu_list[(rx_mpdu->cur_idx) % RXQ_NUM]), netbuf_head);

    /* 乒乓切换队列 */
    rx_mpdu->cur_idx = next_q_idx;
    oal_spin_unlock_irq_restore(&rx_mpdu->st_spin_lock, &lock_flag);
    return ;
}


static hal_rx_nodes *hal_init_rx_nodes(uint32_t req_entries, enum dma_data_direction dma_dir)
{
    hal_rx_node  *node = NULL;
    hal_rx_nodes *nodes = NULL;

    node = kcalloc(req_entries, sizeof(*node), GFP_KERNEL);
    if (node == NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hal_init_rx_nodes:: node alloc fail.}");
        return NULL;
    }

    nodes = kzalloc(sizeof(*nodes), GFP_KERNEL);
    if (nodes == NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hal_init_rx_nodes:: nodes alloc fail.}");
        kfree(node);
        return NULL;
    }

    nodes->tbl      = node;
    nodes->tbl_size = req_entries;
    nodes->dma_dir  = dma_dir;
    oam_warning_log0(0, OAM_SF_ANY, "{hal_init_rx_nodes:: nodes alloc SUCC.}");

    return nodes;
}

int32_t hal_rx_mpdu_que_init(hal_rx_mpdu_que *rx_mpdu)
{
    uint32_t que_idx;

    rx_mpdu->cur_idx = 0;
    rx_mpdu->max_buff_num = HMAC_MSDU_RX_MAX_BUFF_NUM;
    rx_mpdu->process_num_per_round = HMAC_MSDU_RX_MAX_PROCESS_NUM;

    oal_spin_lock_init(&rx_mpdu->st_spin_lock);
    for (que_idx = 0; que_idx < RXQ_NUM; que_idx++) {
        oal_netbuf_list_head_init(&rx_mpdu->ast_rx_mpdu_list[que_idx]);
    }
    return OAL_SUCC;
}

hal_host_device_stru *hal_get_host_device(uint8_t dev_id)
{
    if (dev_id >= WLAN_DEVICE_MAX_NUM_PER_CHIP) {
        return NULL;
    }

    return &g_hal_chip.hal_device[dev_id];
}


/* 功能描述 : 释放host rx相关资源 */
void hal_device_reset_rx_res(void)
{
    hal_host_device_stru *hal_dev = NULL;
    hal_dev = hal_get_host_device(0);
    hal_rx_alloc_list_free(hal_dev, &hal_dev->host_rx_normal_alloc_list);
    hal_rx_alloc_list_free(hal_dev, &hal_dev->host_rx_small_alloc_list);
    hal_dev = hal_get_host_device(1);
    hal_rx_alloc_list_free(hal_dev, &hal_dev->host_rx_normal_alloc_list);
    hal_rx_alloc_list_free(hal_dev, &hal_dev->host_rx_small_alloc_list);
    return;
}

/* 功能描述 : 常收维测信息输出 */
void hal_device_rx_fcs_info(void)
{
    hal_host_device_stru *hal_device = hal_get_host_device(0);

    oam_warning_log3(0, OAM_SF_CFG, "hal_device_rx_fcs_info:packets info, mpdu succ[%d], ampdu succ[%d],fail[%d]",
        hal_device->st_alrx.rx_normal_mdpu_succ_num,
        hal_device->st_alrx.rx_ampdu_succ_num,
        hal_device->st_alrx.rx_ppdu_fail_num);

    hal_device->st_alrx.rx_normal_mdpu_succ_num = 0;
    hal_device->st_alrx.rx_ampdu_succ_num = 0;
    hal_device->st_alrx.rx_ppdu_fail_num = 0;
    return;
}

void hal_set_alrx_status(uint8_t status)
{
    hal_host_device_stru *hal_dev = hal_get_host_device(0);

    hal_dev->st_alrx.en_al_rx_flag = status;
    oam_warning_log1(0, OAM_SF_ANY, "{hal_host_device_init::al rx enable [%d].}", status);
    return;
}


uint32_t hal_host_device_init(hal_host_chip_stru *hal_chip, uint8_t dev_id)
{
    hal_host_device_stru *hal_device = NULL;
    int32_t               ret;

    hal_device            = &hal_chip->hal_device[dev_id];
    hal_device->chip_id   = hal_chip->chip_id;
    hal_device->device_id = dev_id;
    hal_device->inited    = 0;
    hal_device->tx_inited = 0;

    hal_device->rx_nodes = hal_init_rx_nodes(RX_NODES, DMA_FROM_DEVICE);
    if (hal_device->rx_nodes == NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hal_host_device_init::rx nodes fail.}");
        return OAL_FAIL;
    }

    ret = hal_rx_mpdu_que_init(&hal_device->st_rx_mpdu);
    if (ret != OAL_SUCC) {
        oam_error_log1(0, OAM_SF_ANY, "hal_host_device_init::rx mpdu que init failed. ret:%d.", ret);
        return OAL_FAIL;
    }

    return OAL_SUCC;
}


oal_void hal_host_board_init(oal_void)
{
    hal_host_chip_stru *hal_chip = &g_hal_chip;
    uint8_t dev_id;
    uint32_t ret = OAL_SUCC;

    /* tdo 非ddr收帧，不走此接口 */
    memset_s(&g_hal_chip, sizeof(hal_host_chip_stru), 0, sizeof(hal_host_chip_stru));

    hal_chip->chip_id = 0;
    hal_chip->device_nums = WLAN_DEVICE_MAX_NUM_PER_CHIP;
    for (dev_id = 0; dev_id < hal_chip->device_nums; dev_id++) {
        ret = hal_host_device_init(hal_chip, dev_id);
        if (ret != OAL_SUCC) {
            oam_error_log2(0, OAM_SF_ANY, "hal_device_init failed: dev_id:%d, l_ret:%d.", dev_id, ret);
            return;
        }
    }
    hal_host_chip_irq_init();
    oam_warning_log1(0, OAM_SF_ANY, "hal_device_init::hal dev init succ[%x].", (uintptr_t)hal_chip);
    return;
}

oal_module_symbol(hal_set_alrx_status);
oal_module_symbol(hal_device_reset_rx_res);
oal_module_symbol(hal_device_rx_fcs_info);
#endif
