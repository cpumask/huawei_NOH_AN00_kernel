

/* 1 其他头文件包含 */
#include "oal_pcie_linux.h"
#include "hmac_tx_msdu_dscr.h"
#include "hmac_tx_data.h"
#include "hal_host_ext_if.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_DRIVER_HMAC_TX_MSDU_DSCR_C

#ifdef CONFIG_ARCH_KIRIN_PCIE

typedef enum {
    RING_PTR_EQUAL = 0,
    RING_PTR_SMALLER,
    RING_PTR_GREATER,
} ring_ptr_compare_enum;

#define OAL_MAKE_WORD64(lsw, msw) ((((uint64_t)(msw) << 32) & 0xFFFFFFFF00000000) | (lsw))
#define HMAC_TX_RING_MAX_AMSDU_NUM 7

hmac_tx_ring_device_info_mgmt_stru g_tx_ring_device_info_mgmt = { 0 };

/* 4 函数实现 */

OAL_STATIC OAL_INLINE void hmac_tx_rw_ptr_inc(un_rw_ptr *rw_ptr, uint16_t ring_size)
{
    if (++rw_ptr->st_rw_ptr.bit_rw_ptr >= ring_size) {
        rw_ptr->st_rw_ptr.bit_rw_ptr = 0;
        rw_ptr->st_rw_ptr.bit_wrap_flag = !rw_ptr->st_rw_ptr.bit_wrap_flag;
    }
}


OAL_STATIC OAL_INLINE ring_ptr_compare_enum hmac_tx_rw_ptr_compare(un_rw_ptr rw_ptr1, un_rw_ptr rw_ptr2)
{
    if (rw_ptr1.rw_ptr == rw_ptr2.rw_ptr) {
        return RING_PTR_EQUAL;
    }

    if (rw_ptr1.st_rw_ptr.bit_wrap_flag == rw_ptr2.st_rw_ptr.bit_wrap_flag) {
        return (rw_ptr1.st_rw_ptr.bit_rw_ptr > rw_ptr2.st_rw_ptr.bit_rw_ptr) ? RING_PTR_GREATER : RING_PTR_SMALLER;
    } else {
        return (rw_ptr1.st_rw_ptr.bit_rw_ptr < rw_ptr2.st_rw_ptr.bit_rw_ptr) ? RING_PTR_GREATER : RING_PTR_SMALLER;
    }
}


OAL_STATIC OAL_INLINE void hmac_tx_msdu_ring_inc_write_ptr(hmac_msdu_info_ring_stru *tx_ring)
{
    un_rw_ptr write_ptr = { .rw_ptr = tx_ring->base_ring_info.write_index };

    hmac_tx_rw_ptr_inc(&write_ptr, RING_DEPTH(tx_ring->base_ring_info.size));

    tx_ring->base_ring_info.write_index = write_ptr.rw_ptr;
}


OAL_STATIC OAL_INLINE void hmac_tx_msdu_ring_inc_release_ptr(hmac_msdu_info_ring_stru *tx_ring)
{
    un_rw_ptr release_ptr = { .rw_ptr = tx_ring->release_index };

    hmac_tx_rw_ptr_inc(&release_ptr, RING_DEPTH(tx_ring->base_ring_info.size));

    tx_ring->release_index = release_ptr.rw_ptr;
}


OAL_STATIC uint32_t hmac_tx_dma_map_phyaddr(uint8_t *host_addr, uint32_t len, dma_addr_t *physaddr)
{
    oal_pcie_linux_res *pcie_linux_res = oal_get_pcie_linux_res();
    if (!pcie_linux_res) {
        oam_error_log0(0, 0, "{hmac_tx_dma_map_phyaddr::pcie_linux_res NULL}");
        return OAL_FAIL;
    }

    *physaddr = dma_map_single(&pcie_linux_res->pst_pcie_dev->dev, host_addr, len, DMA_FROM_DEVICE);
    if (dma_mapping_error(&pcie_linux_res->pst_pcie_dev->dev, *physaddr)) {
        oam_error_log0(0, 0, "{hmac_tx_dma_map_phyaddr::addr map failed}");
        return OAL_FAIL;
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t hmac_tx_dma_unmap_addr(uint8_t *dma_addr, uint32_t len)
{
    oal_pcie_linux_res *pcie_linux_res = oal_get_pcie_linux_res();
    if (!pcie_linux_res) {
        oam_error_log0(0, OAM_SF_TX, "{hmac_tx_dma_unmap_addr::pcie_linux_res NULL}");
        return OAL_FAIL;
    }

    dma_unmap_single(&pcie_linux_res->pst_pcie_dev->dev, (dma_addr_t)dma_addr, len, DMA_FROM_DEVICE);

    return OAL_SUCC;
}


uint32_t hmac_tx_hostva_to_devva(uint8_t *hostva, uint32_t alloc_size, uint64_t *devva)
{
    dma_addr_t pcie_dma_addr;

    if (hmac_tx_dma_map_phyaddr(hostva, alloc_size, &pcie_dma_addr) != OAL_SUCC) {
        return OAL_FAIL;
    }

    return pcie_if_hostca_to_devva(0, pcie_dma_addr, devva);
}


OAL_STATIC uint32_t hmac_tx_dma_alloc_ring_buf(hmac_msdu_info_ring_stru *tx_ring, uint32_t host_ring_buf_size)
{
    dma_addr_t host_ring_dma_addr = 0;
    void *host_ring_vaddr = NULL;
    oal_pcie_linux_res *pcie_linux_res = oal_get_pcie_linux_res();

    if (pcie_linux_res == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    host_ring_vaddr = dma_alloc_coherent(&pcie_linux_res->pst_pcie_dev->dev, host_ring_buf_size,
                                         &host_ring_dma_addr, GFP_KERNEL);
    if (host_ring_vaddr == NULL) {
        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }

    tx_ring->host_ring_buf = (uint8_t *)host_ring_vaddr;
    tx_ring->host_ring_dma_addr = (uint64_t)host_ring_dma_addr;
    tx_ring->host_ring_buf_size = host_ring_buf_size;

    return OAL_SUCC;
}


OAL_STATIC uint32_t hmac_tx_dma_release_ring_buf(hmac_msdu_info_ring_stru *tx_ring)
{
    oal_pcie_linux_res *pcie_linux_res = oal_get_pcie_linux_res();

    if (!pcie_linux_res || !tx_ring->host_ring_buf || !tx_ring->host_ring_dma_addr || !tx_ring->host_ring_buf_size) {
        oam_warning_log3(0, OAM_SF_TX, "{hmac_tx_dma_release_ring_buf::buf[0x%x], dma[0x%x], size[%d]}",
            (uintptr_t)tx_ring->host_ring_buf, (uintptr_t)tx_ring->host_ring_dma_addr, tx_ring->host_ring_buf_size);
        return OAL_FAIL;
    }

    dma_free_coherent(&pcie_linux_res->pst_pcie_dev->dev, tx_ring->host_ring_buf_size,
                      tx_ring->host_ring_buf, (dma_addr_t)tx_ring->host_ring_dma_addr);

    return OAL_SUCC;
}


uint32_t hmac_tx_alloc_ring(hmac_msdu_info_ring_stru *tx_ring)
{
    uint32_t ret;
    uint16_t host_ring_size = RING_DEPTH(tx_ring->base_ring_info.size);
    uint32_t host_ring_buf_size = host_ring_size * TX_RING_MSDU_INFO_LEN;
    uint32_t host_netbuf_buf_size = host_ring_size * OAL_SIZEOF(oal_netbuf_stru *);

    if (tx_ring->base_ring_info.offload) {
        return OAL_SUCC;
    }

    ret = hmac_tx_dma_alloc_ring_buf(tx_ring, host_ring_buf_size);
    if (ret != OAL_SUCC) {
        oam_error_log1(0, OAM_SF_TX, "{hmac_tx_host_ring_alloc::dma alloc host buf failed[%d]}", ret);
        return ret;
    }

    tx_ring->netbuf_list = (oal_netbuf_stru **)oal_memalloc(host_netbuf_buf_size);
    if (tx_ring->netbuf_list == NULL) {
        oam_error_log0(0, OAM_SF_TX, "{hmac_tx_host_ring_alloc::kmalloc netbuflist failed}");
        hmac_tx_dma_release_ring_buf(tx_ring);
        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }

    memset_s(tx_ring->host_ring_buf, host_ring_buf_size, 0, host_ring_buf_size);
    memset_s(tx_ring->netbuf_list, host_netbuf_buf_size, 0, host_netbuf_buf_size);

    return OAL_SUCC;
}


OAL_STATIC uint32_t hmac_tx_ring_set_update_info(
    hmac_tid_info_stru *tx_tid_info, tid_update_stru *tid_update_info, tid_cmd_enum_uint8 cmd)
{
    uint64_t devva = 0;
    uint8_t msdu_ring_size = OAL_SIZEOF(msdu_info_ring_stru);
    hmac_msdu_info_ring_stru *tx_ring = &tx_tid_info->non_ba_tx_ring;

    tid_update_info->cmd = cmd;
    tid_update_info->user_id = tx_tid_info->user_index;
    tid_update_info->tid_no = tx_tid_info->tid_no;
    tid_update_info->cur_bytes = tx_tid_info->cur_bytes;

    memcpy_s(&tid_update_info->msdu_ring, msdu_ring_size, &tx_ring->base_ring_info, msdu_ring_size);
    if (tx_ring->base_ring_info.offload || cmd != TID_CMDID_CREATE) {
        return OAL_SUCC;
    }

    if (pcie_if_hostca_to_devva(0, tx_ring->host_ring_dma_addr, &devva) != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_TX, "{hmac_tx_ring_set_update_info::hostca to devva failed}");
        return OAL_FAIL;
    }

    tid_update_info->msdu_ring.base_addr_lsb = GET_LOW_32_BITS(devva);
    tid_update_info->msdu_ring.base_addr_msb = GET_HIGH_32_BITS(devva);

    return OAL_SUCC;
}


OAL_STATIC uint32_t hmac_tx_sync_ring_info(
    mac_user_stru *mac_user, hmac_tid_info_stru *tx_tid_info, tid_cmd_enum_uint8 cmd)
{
    uint32_t ret;
    mac_vap_stru *mac_vap = OAL_PTR_NULL;
    tid_update_stru tid_update_info = { 0 };

    if (hmac_tx_ring_set_update_info(tx_tid_info, &tid_update_info, cmd) != OAL_SUCC) {
        return OAL_FAIL;
    }
    mac_vap = mac_res_get_mac_vap(mac_user->uc_vap_id);
    if (mac_vap == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_WPA, "{hmac_tx_sync_ring_info::mac_vap is null}");
        return OAL_FAIL;
    }
    ret = hmac_config_send_event(mac_vap, WLAN_CFGID_TX_TID_UPDATE,
        OAL_SIZEOF(tid_update_stru), (uint8_t *)&tid_update_info);
    if (ret != OAL_SUCC) {
        oam_error_log1(0, OAM_SF_WPA, "{hmac_tx_ring_info_sync::hmac_config_send_event failed[%d]}", ret);
    }

    return ret;
}


OAL_STATIC void hmac_tx_reg_write_base_addr(hmac_tx_ring_device_info_stru *tx_ring_device_info, uint64_t devva)
{
    oal_writel(GET_LOW_32_BITS(devva), (uintptr_t)tx_ring_device_info->word_addr[0]);
    oal_writel(GET_HIGH_32_BITS(devva), (uintptr_t)tx_ring_device_info->word_addr[1]);
}


OAL_STATIC OAL_INLINE void hmac_tx_reg_write_wptr(hmac_tx_ring_device_info_stru *tx_ring_device_info, uint16_t wptr)
{
    tx_ring_device_info->msdu_info_word2.word2_bit.write_ptr = wptr;

    oal_writel(tx_ring_device_info->msdu_info_word2.word2, (uintptr_t)tx_ring_device_info->word_addr[2]);
}


OAL_STATIC OAL_INLINE void hmac_tx_reg_write_rptr(hmac_tx_ring_device_info_stru *tx_ring_device_info, uint16_t rptr)
{
    tx_ring_device_info->msdu_info_word3.word3_bit.read_ptr = rptr;

    oal_writel(tx_ring_device_info->msdu_info_word3.word3, (uintptr_t)tx_ring_device_info->word_addr[3]);
}


OAL_STATIC OAL_INLINE void hmac_tx_reg_write_ring_depth(
    hmac_tx_ring_device_info_stru *tx_ring_device_info, uint16_t ring_depth)
{
    tx_ring_device_info->msdu_info_word2.word2_bit.tx_msdu_ring_depth = ring_depth;

    oal_writel(tx_ring_device_info->msdu_info_word2.word2, (uintptr_t)tx_ring_device_info->word_addr[2]);
}


OAL_STATIC OAL_INLINE void hmac_tx_reg_write_max_amsdu_num(
    hmac_tx_ring_device_info_stru *tx_ring_device_info, uint16_t max_amsdu_num)
{
    tx_ring_device_info->msdu_info_word2.word2_bit.max_aggr_amsdu_num = max_amsdu_num;

    oal_writel(tx_ring_device_info->msdu_info_word2.word2, (uintptr_t)tx_ring_device_info->word_addr[2]);
}


OAL_STATIC void hmac_reg_write_create_ring_info(
    hmac_tx_ring_device_info_stru *tx_ring_device_info, hmac_msdu_info_ring_stru *tx_ring)
{
    uint64_t devva = 0;

    if (pcie_if_hostca_to_devva(0, tx_ring->host_ring_dma_addr, &devva) != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_TX, "{hmac_tx_ring_set_update_info::hostca to devva failed}");
        return;
    }

    hmac_tx_reg_write_base_addr(tx_ring_device_info, devva);
    hmac_tx_reg_write_ring_depth(tx_ring_device_info, tx_ring->base_ring_info.size);
    hmac_tx_reg_write_max_amsdu_num(tx_ring_device_info, tx_ring->base_ring_info.max_amsdu_num);
    hmac_tx_reg_write_rptr(tx_ring_device_info, 0);
    hmac_tx_reg_write_wptr(tx_ring_device_info, 0);
}


OAL_STATIC void hmac_reg_write_del_ring_info(hmac_tx_ring_device_info_stru *tx_ring_device_info)
{
    hmac_tx_reg_write_base_addr(tx_ring_device_info, 0);
    hmac_tx_reg_write_ring_depth(tx_ring_device_info, 0);
    hmac_tx_reg_write_max_amsdu_num(tx_ring_device_info, 0);
    hmac_tx_reg_write_rptr(tx_ring_device_info, 0);
    hmac_tx_reg_write_wptr(tx_ring_device_info, 0);
}


OAL_STATIC OAL_INLINE void hmac_reg_write_enque_ring_info(
    hmac_tx_ring_device_info_stru *tx_ring_device_info, hmac_msdu_info_ring_stru *tx_ring)
{
    un_rw_ptr write_origin = { .rw_ptr = tx_ring_device_info->msdu_info_word2.word2_bit.write_ptr };
    un_rw_ptr write_new = { .rw_ptr = tx_ring->base_ring_info.write_index };

    if (hmac_tx_rw_ptr_compare(write_new, write_origin) == RING_PTR_SMALLER) {
        oam_error_log2(0, OAM_SF_TX, "{hmac_tx_reg_write_wptr::new write_ptr[%d] < origin write_ptr[%d], return}",
            write_new.st_rw_ptr.bit_rw_ptr, write_origin.st_rw_ptr.bit_rw_ptr);
        return;
    }

    hmac_tx_reg_write_wptr(tx_ring_device_info, tx_ring->base_ring_info.write_index);
}


OAL_STATIC void hmac_tx_reg_write_ring_info(
    hmac_user_stru *hmac_user, hmac_tid_info_stru *tx_tid_info, tid_cmd_enum_uint8 cmd)
{
    hmac_msdu_info_ring_stru *tx_ring = &tx_tid_info->non_ba_tx_ring;
    hmac_tx_ring_device_info_stru *tx_ring_device_info = tx_tid_info->tx_ring_device_info;

    if (tx_ring->base_ring_info.offload) {
        return;
    }

    if (cmd == TID_CMDID_ENQUE) {
        hmac_reg_write_enque_ring_info(tx_ring_device_info, tx_ring);
    } else if (cmd == TID_CMDID_CREATE) {
        hmac_reg_write_create_ring_info(tx_ring_device_info, tx_ring);
    } else if (cmd == TID_CMDID_DEL) {
        hmac_reg_write_del_ring_info(tx_ring_device_info);
    }
}


OAL_STATIC hmac_tx_ring_device_info_stru *hmac_get_tx_ring_device_info(uint16_t user_lut_id, uint8_t tid)
{
    return &g_tx_ring_device_info_mgmt.tx_ring_device_info[user_lut_id][tid];
}


OAL_STATIC uint32_t hmac_get_device_tx_ring_base_addr(uint8_t lut_idx, uint8_t tid)
{
    return g_tx_ring_device_info_mgmt.tx_ring_device_base_addr + TX_RING_INFO_LEN * (lut_idx * WLAN_TIDNO_BUTT + tid);
}


OAL_STATIC uint32_t hmac_tx_ring_device_addr_init(
    hmac_tx_ring_device_info_stru *tx_ring_device_info, uint8_t user_lut_idx, uint8_t tid)
{
    uint64_t hostva = 0;
    int32_t ret;
    uint32_t word_index;
    uint32_t user_tid_ring_base_addr = hmac_get_device_tx_ring_base_addr(user_lut_idx, tid);

    for (word_index = 0; word_index < TX_RING_INFO_WORD_NUM; word_index++) {
        ret = oal_pcie_devca_to_hostva(0, user_tid_ring_base_addr, &hostva);
        if (ret != OAL_SUCC) {
            oam_error_log1(0, OAM_SF_TX, "{hmac_tx_set_user_tid_ring_addr::devca to hostva failed[%d]}", ret);
            return ret;
        }

        tx_ring_device_info->word_addr[word_index] = hostva;
        user_tid_ring_base_addr += OAL_SIZEOF(uint32_t);
    }

    return OAL_SUCC;
}


OAL_STATIC uint32_t hmac_tx_ring_device_info_init(
    hmac_tx_ring_device_info_stru *ring_device_info, uint8_t user_lut_index, uint8_t tid)
{
    memset_s(ring_device_info, OAL_SIZEOF(hmac_tx_ring_device_info_stru), 0, OAL_SIZEOF(hmac_tx_ring_device_info_stru));

    return hmac_tx_ring_device_addr_init(ring_device_info, user_lut_index, tid);
}


OAL_STATIC void hmac_user_set_ring(
    hmac_tid_info_stru *tx_tid_info, hmac_user_stru *hmac_user, uint8_t tid, uint8_t size)
{
    memset_s(tx_tid_info, OAL_SIZEOF(hmac_tid_info_stru), 0, OAL_SIZEOF(hmac_tid_info_stru));

    tx_tid_info->tid_no = tid;
    tx_tid_info->user_index = hmac_user->st_user_base_info.us_assoc_id;
    tx_tid_info->non_ba_tx_ring.base_ring_info.size = size;
    tx_tid_info->non_ba_tx_ring.base_ring_info.offload = OAL_FALSE;
    tx_tid_info->non_ba_tx_ring.base_ring_info.read_index = 0;
    tx_tid_info->non_ba_tx_ring.base_ring_info.write_index = 0;
    tx_tid_info->non_ba_tx_ring.base_ring_info.max_amsdu_num =
        (tid != WLAN_TIDNO_BCAST) ? HMAC_TX_RING_MAX_AMSDU_NUM : 0;
    tx_tid_info->tx_ring_device_info = hmac_get_tx_ring_device_info(hmac_user->lut_index, tid);
    tx_tid_info->tx_update_freq = 1;

    if (hmac_tx_ring_device_info_init(tx_tid_info->tx_ring_device_info, hmac_user->lut_index, tid) != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_CFG, "{hmac_user_set_ring::tx ring device info init failed!}");
    }

    oal_spin_lock_init(&tx_tid_info->non_ba_tx_ring.lock);
}


uint32_t hmac_tx_ring_init(hmac_user_stru *hmac_user, uint8_t tid, uint8_t ring_size)
{
    hmac_tid_info_stru *tx_tid_info = &hmac_user->tx_tid_info[tid];

    if (hmac_get_tx_switch() != HOST_TX) {
        oam_warning_log0(0, 0, "{hmac_tx_ring_init::ring init return due to tx_switch != HOST_TX}");
        return OAL_SUCC;
    }

    hmac_user_set_ring(tx_tid_info, hmac_user, tid, ring_size);

    if (hmac_tx_alloc_ring(&tx_tid_info->non_ba_tx_ring) != OAL_SUCC) {
        return OAL_FAIL;
    }

    hmac_tx_reg_write_ring_info(hmac_user, tx_tid_info, TID_CMDID_CREATE);

    tx_tid_info->non_ba_tx_ring.enabled = OAL_TRUE;

    return OAL_SUCC;
}


uint32_t hmac_set_tx_ring_device_base_addr(frw_event_mem_stru *frw_mem)
{
    frw_event_stru *frw_event = NULL;
    mac_d2h_tx_ring_base_addr_stru *d2h_tx_ring_base_addr = NULL;

    if (frw_mem == OAL_PTR_NULL) {
        return OAL_FAIL;
    }

    frw_event = frw_get_event_stru(frw_mem);
    d2h_tx_ring_base_addr = (mac_d2h_tx_ring_base_addr_stru *)frw_event->auc_event_data;

    g_tx_ring_device_info_mgmt.tx_ring_device_base_addr = d2h_tx_ring_base_addr->tx_ring_base_addr;

    return OAL_SUCC;
}


OAL_STATIC OAL_INLINE msdu_info_stru *hmac_tx_get_ring_msdu_info(hmac_msdu_info_ring_stru *tx_ring, uint16_t index)
{
    return (msdu_info_stru *)(tx_ring->host_ring_buf + index * TX_RING_MSDU_INFO_LEN);
}


OAL_STATIC uint32_t hmac_tx_unmap_msdu_dma_addr(
    hmac_msdu_info_ring_stru *tx_ring, uint16_t release_index, uint32_t netbuf_len)
{
    uint64_t netbuf_iova = 0;
    uint32_t ret;
    msdu_info_stru *msdu_info = hmac_tx_get_ring_msdu_info(tx_ring, release_index);

    ret = pcie_if_devva_to_hostca(0, OAL_MAKE_WORD64(msdu_info->msdu_addr_lsb, msdu_info->msdu_addr_msb), &netbuf_iova);
    if (ret != OAL_SUCC) {
        oam_error_log1(0, OAM_SF_TX, "{hmac_tx_unmap_msdu_dma_addr::hostca to devva fail[%d]", ret);
        return ret;
    }

    return hmac_tx_dma_unmap_addr((uint8_t *)(uintptr_t)netbuf_iova, netbuf_len + TX_HW_MSDU_DSCR_LEN);
}


OAL_STATIC OAL_INLINE void hmac_tx_msdu_update_free_cnt(hmac_msdu_info_ring_stru *tx_msdu_ring, oal_netbuf_stru *netbuf)
{
    hmac_tid_info_stru *tid_info;
    hmac_user_stru *hmac_user = NULL;

    tid_info = oal_container_of(tx_msdu_ring, hmac_tid_info_stru, non_ba_tx_ring);

    if (tx_msdu_ring->msdu_cnt > 0) {
        tx_msdu_ring->msdu_cnt--;
    } else {
        oam_error_log0(0, OAM_SF_TX, "{hmac_tx_msdu_update_free_cnt::mpdu list cnt is 0.}");
    }

    if (tx_msdu_ring->msdu_len >= oal_netbuf_len(netbuf)) {
        tx_msdu_ring->msdu_len -= oal_netbuf_len(netbuf);
    } else {
        oam_error_log2(0, OAM_SF_TX, "{hmac_tx_msdu_update_free_cnt::wrong mpdu list len:%d, del mpdu len:%d.}",
            tx_msdu_ring->msdu_len, oal_netbuf_len(netbuf));
    }

    if (tid_info->cur_msdus > 0) {
        tid_info->cur_msdus--;
    } else {
        oam_error_log0(0, OAM_SF_TX, "{hmac_tx_msdu_update_free_cnt::tid msdu cnt is 0.}");
    }

    if (tid_info->cur_bytes >= oal_netbuf_get_len(netbuf)) {
        tid_info->cur_bytes -= oal_netbuf_get_len(netbuf);
        hmac_user = mac_res_get_hmac_user(tid_info->user_index);
        if (hmac_user == OAL_PTR_NULL) {
            oam_error_log0(0, OAM_SF_WPA, "{hmac_tx_msdu_update_free_cnt::hmac_user is null}");
            return;
        }
        if (tid_info->tx_msdus % hmac_user->tx_tid_info[tid_info->tid_no].tx_update_freq == 0) {
            hmac_tx_sync_ring_info(&hmac_user->st_user_base_info, tid_info, TID_CMDID_DEQUE);
        }
    } else {
        oam_error_log2(0, OAM_SF_TX, "{hmac_tx_msdu_update_free_cnt::wrong tid msdu len:%d, del msdu len:%d.}",
            tid_info->cur_bytes, oal_netbuf_get_len(netbuf));
    }
}


OAL_STATIC void hmac_tx_ring_release_netbuf(hmac_msdu_info_ring_stru *tx_ring, un_rw_ptr release_ptr)
{
    uint16_t release_index = release_ptr.st_rw_ptr.bit_rw_ptr;
    oal_netbuf_stru *netbuf = NULL;

    if (tx_ring->netbuf_list == NULL) {
        oam_error_log0(0, OAM_SF_TX, "{hmac_tx_ring_release_netbuf::netbuf list is NULL!}");
        return;
    }

    netbuf = tx_ring->netbuf_list[release_index];
    if (netbuf == NULL) {
        oam_error_log1(0, OAM_SF_TX, "{hmac_tx_ring_release_netbuf::netbuf[%d] is NULL!}", release_index);
        return;
    }

    if (hmac_tx_unmap_msdu_dma_addr(tx_ring, release_index, netbuf->len) != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_TX, "{hmac_tx_ring_release_netbuf::unmap msdu dma failed}");
    }

    hmac_tx_msdu_update_free_cnt(tx_ring, netbuf);

    oal_netbuf_free(netbuf);
    tx_ring->netbuf_list[release_index] = NULL;
}


OAL_STATIC OAL_INLINE void hmac_tx_reset_msdu_info(hmac_msdu_info_ring_stru *tx_ring, un_rw_ptr release_ptr)
{

    msdu_info_stru *msdu_info = hmac_tx_get_ring_msdu_info(tx_ring, release_ptr.st_rw_ptr.bit_rw_ptr);
    if (tx_ring->host_tx_flag == HOST_AL_TX_FLAG) {
        return;
    }

    memset_s(msdu_info, OAL_SIZEOF(msdu_info_stru), 0, OAL_SIZEOF(msdu_info_stru));
}


OAL_STATIC void hmac_tx_ring_release_all_netbuf(
    hmac_msdu_info_ring_stru *tx_ring, oal_bool_enum_uint8 release_to_read_ptr)
{
    un_rw_ptr release_ptr = { .rw_ptr = tx_ring->release_index };
    un_rw_ptr target_ptr = { .rw_ptr = release_to_read_ptr ?
        tx_ring->base_ring_info.read_index : tx_ring->base_ring_info.write_index
    };

    while (hmac_tx_rw_ptr_compare(release_ptr, target_ptr) == RING_PTR_SMALLER) {
        hmac_tx_ring_release_netbuf(tx_ring, release_ptr);
        hmac_tx_reset_msdu_info(tx_ring, release_ptr);
        hmac_tx_msdu_ring_inc_release_ptr(tx_ring);
        release_ptr.rw_ptr = tx_ring->release_index;
    }
}


OAL_STATIC void hmac_tx_ring_reset(hmac_msdu_info_ring_stru *tx_ring)
{
    memset_s(&tx_ring->base_ring_info, OAL_SIZEOF(msdu_info_ring_stru), 0, OAL_SIZEOF(msdu_info_ring_stru));
    tx_ring->host_ring_buf = NULL;
    tx_ring->host_ring_dma_addr = 0;
    tx_ring->host_ring_buf_size = 0;
    tx_ring->release_index = 0;
    tx_ring->msdu_cnt = 0;
    tx_ring->msdu_len = 0;
    tx_ring->netbuf_list = NULL;
}


void hmac_tx_host_ring_release(hmac_msdu_info_ring_stru *tx_ring)
{
    if (tx_ring->base_ring_info.offload) {
        return;
    }

    hmac_tx_ring_release_all_netbuf(tx_ring, OAL_FALSE);
    oal_free(tx_ring->netbuf_list);

    if (hmac_tx_dma_release_ring_buf(tx_ring) != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_TX, "{hmac_tx_host_ring_release::host buf release failed}");
    }

    hmac_tx_ring_reset(tx_ring);
}


OAL_STATIC void hmac_tx_ring_deinit(hmac_user_stru *hmac_user, uint8_t tid)
{
    hmac_tid_info_stru *tx_tid_info = &hmac_user->tx_tid_info[tid];

    tx_tid_info->non_ba_tx_ring.enabled = OAL_FALSE;
    hmac_tx_reg_write_ring_info(hmac_user, tx_tid_info, TID_CMDID_DEL);
    hmac_tx_host_ring_release(&tx_tid_info->non_ba_tx_ring);
}


void hmac_user_tx_ring_deinit(hmac_user_stru *hmac_user)
{
    uint32_t tid;

    if (hmac_get_tx_switch() != HOST_TX) {
        oam_warning_log0(0, 0, "{hmac_user_tx_ring_deinit::ring deinit return due to tx_switch != HOST_TX}");
        return;
    }

    for (tid = 0; tid < WLAN_TID_MAX_NUM; tid++) {
        hmac_tx_ring_deinit(hmac_user, tid);
    }
}


OAL_STATIC OAL_INLINE oal_bool_enum_uint8 hmac_tx_ring_rw_ptr_check(hmac_msdu_info_ring_stru *tx_msdu_ring)
{
    un_rw_ptr release_ptr = { .rw_ptr = tx_msdu_ring->release_index };
    un_rw_ptr read_ptr = { .rw_ptr = tx_msdu_ring->base_ring_info.read_index };
    un_rw_ptr write_ptr = { .rw_ptr = tx_msdu_ring->base_ring_info.write_index };
    ring_ptr_compare_enum read_release_compare = hmac_tx_rw_ptr_compare(read_ptr, release_ptr);
    ring_ptr_compare_enum write_read_compare = hmac_tx_rw_ptr_compare(write_ptr, read_ptr);

    return (read_release_compare == RING_PTR_EQUAL || read_release_compare == RING_PTR_GREATER) &&
           (write_read_compare == RING_PTR_EQUAL || write_read_compare == RING_PTR_GREATER);
}


OAL_STATIC uint32_t hmac_tx_netbuf_expand_headroom(oal_netbuf_stru *netbuf, mac_tx_ctl_stru *tx_ctl)
{
    int32_t ret;
    uint8_t mac_hdr[MAX_MAC_HEAD_LEN] = { 0 };

    if (oal_netbuf_headroom(netbuf) > TX_HW_MSDU_DSCR_LEN) {
        return OAL_SUCC;
    }

    oam_warning_log0(0, OAM_SF_TX, "{hmac_tx_netbuf_expand_headroom::netbuf headroom not enough, expanding...}");

    if (memcpy_s(mac_hdr, MAX_MAC_HEAD_LEN, (uint8_t *)MAC_GET_CB_FRAME_HEADER_ADDR(tx_ctl),
        MAX_MAC_HEAD_LEN) != EOK) {
        return OAL_FAIL;
    }

    ret = oal_netbuf_expand_head(netbuf, TX_HW_MSDU_DSCR_LEN, 0, GFP_ATOMIC);
    if (ret != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_tx_netbuf_expand_headroom::expand headroom failed}");
        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }

    if (memcpy_s(oal_netbuf_data(netbuf), MAX_MAC_HEAD_LEN, mac_hdr, MAX_MAC_HEAD_LEN) != EOK) {
        return OAL_FAIL;
    }

    MAC_GET_CB_FRAME_HEADER_ADDR(tx_ctl) = (mac_ieee80211_frame_stru *)oal_netbuf_data(netbuf);

    return OAL_SUCC;
}


OAL_STATIC void hmac_tx_set_msdu_frag_info(msdu_info_stru *msdu_info,
    mac_tx_ctl_stru *tx_ctl, oal_netbuf_stru *netbuf)
{
    mac_ieee80211_frame_stru *mac_header = MAC_GET_CB_FRAME_HEADER_ADDR(tx_ctl);

    msdu_info->more_frag = mac_header->st_frame_control.bit_more_frag;
    msdu_info->frag_num = mac_header->bit_frag_num;
}

#define CSUM_TYPE_IPV4_TCP 1
#define CSUM_TYPE_IPV4_UDP 2
#define CSUM_TYPE_IPV6_TCP 3
#define CSUM_TYPE_IPV6_UDP 4


OAL_STATIC void hmac_tx_set_msdu_csum_info(msdu_info_stru *msdu_info, oal_netbuf_stru *netbuf)
{
    mac_ether_header_stru *ether_header = (mac_ether_header_stru *)oal_netbuf_data(netbuf);
    if (ether_header->us_ether_type == oal_host2net_short(ETHER_TYPE_IP)) {
        struct iphdr *iph = (struct iphdr *)(netbuf->head + netbuf->network_header);
        if (iph->protocol == OAL_IPPROTO_TCP) {
            msdu_info->csum_type = CSUM_TYPE_IPV4_TCP;
        } else if (iph->protocol == OAL_IPPROTO_UDP) {
            msdu_info->csum_type = CSUM_TYPE_IPV4_UDP;
        }
    } else if (ether_header->us_ether_type == oal_host2net_short(ETHER_TYPE_IPV6)) {
        struct ipv6hdr *ipv6h = (struct ipv6hdr *)(netbuf->head + netbuf->network_header);
        if (ipv6h->nexthdr == OAL_IPPROTO_TCP) {
            msdu_info->csum_type = CSUM_TYPE_IPV6_TCP;
        } else if (ipv6h->nexthdr == OAL_IPPROTO_UDP) {
            msdu_info->csum_type = CSUM_TYPE_IPV6_UDP;
        }
    }
}


OAL_STATIC void hmac_tx_set_msdu_info_from_netbuf(
    mac_vap_stru *mac_vap, msdu_info_stru *msdu_info, oal_netbuf_stru *netbuf, mac_tx_ctl_stru *tx_ctl, uint64_t devva)
{
    msdu_info->msdu_addr_lsb = GET_LOW_32_BITS(devva);
    msdu_info->msdu_addr_msb = GET_HIGH_32_BITS(devva);
    msdu_info->msdu_len = oal_netbuf_len(netbuf);
    msdu_info->data_type = MAC_GET_CB_DATA_TYPE(tx_ctl);

    if (MAC_GET_CB_IS_802_3_SNAP(tx_ctl)) {
        msdu_info->data_type |= DATA_TYPE_802_3_SNAP;
    }

    msdu_info->csum_type = 0;

    if (netbuf->ip_summed == CHECKSUM_PARTIAL) {
        hmac_tx_set_msdu_csum_info(msdu_info, netbuf);
    }

    if (MAC_GET_CB_DATA_TYPE(tx_ctl) == DATA_TYPE_80211) {
        hmac_tx_set_msdu_frag_info(msdu_info, tx_ctl, netbuf);
    }

    if (mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_AP && !MAC_GET_CB_IS_4ADDRESS(tx_ctl)) {
        msdu_info->from_ds = 1;
        msdu_info->to_ds = 0;
    } else if (mac_vap->en_vap_mode == WLAN_VAP_MODE_BSS_STA && !MAC_GET_CB_IS_4ADDRESS(tx_ctl)) {
        msdu_info->from_ds = 0;
        msdu_info->to_ds = 1;
    } else {
        msdu_info->from_ds = 1;
        msdu_info->to_ds = 1;
    }
}


uint8_t *hmac_tx_netbuf_init_msdu_dscr(oal_netbuf_stru *netbuf)
{
    uint8_t *msdu_dscr_addr = (uint8_t *)((uintptr_t)oal_netbuf_data(netbuf) - TX_HW_MSDU_DSCR_LEN);

    memset_s(msdu_dscr_addr, TX_HW_MSDU_DSCR_LEN, 0, TX_HW_MSDU_DSCR_LEN);

    return msdu_dscr_addr;
}


uint32_t hmac_tx_set_msdu_info(mac_vap_stru *mac_vap, msdu_info_stru *msdu_info, oal_netbuf_stru *netbuf)
{
    uint32_t ret;
    uint64_t devva;
    mac_tx_ctl_stru *tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(netbuf);

    if (hmac_tx_netbuf_expand_headroom(netbuf, tx_ctl) != OAL_SUCC) {
        return OAL_FAIL;
    }

    ret = hmac_tx_hostva_to_devva(hmac_tx_netbuf_init_msdu_dscr(netbuf), netbuf->len + TX_HW_MSDU_DSCR_LEN, &devva);
    if (ret != OAL_SUCC) {
        oam_error_log1(0, OAM_SF_TX, "{hmac_tx_set_msdu_info::hostva to devva failed[%d]}", ret);
        return ret;
    }

    hmac_tx_set_msdu_info_from_netbuf(mac_vap, msdu_info, netbuf, tx_ctl, devva);

    return OAL_SUCC;
}


OAL_STATIC OAL_INLINE void hmac_record_netbuf_addr(hmac_msdu_info_ring_stru *tx_msdu_ring, oal_netbuf_stru *netbuf)
{
    un_rw_ptr write_ptr = { .rw_ptr = tx_msdu_ring->base_ring_info.write_index };

    tx_msdu_ring->netbuf_list[write_ptr.st_rw_ptr.bit_rw_ptr] = netbuf;
}


OAL_STATIC uint32_t hmac_tx_set_msdu_info_to_ring(
    mac_vap_stru *mac_vap, hmac_msdu_info_ring_stru *tx_msdu_ring, oal_netbuf_stru *netbuf)
{
    un_rw_ptr write_ptr = { .rw_ptr = tx_msdu_ring->base_ring_info.write_index };
    un_rw_ptr release_ptr = { .rw_ptr = tx_msdu_ring->release_index };
    msdu_info_stru *ring_msdu_info = NULL;

    if (IS_RW_RING_FULL(&release_ptr, &write_ptr)) {
        return OAL_FAIL;
    }

    ring_msdu_info = hmac_tx_get_ring_msdu_info(tx_msdu_ring, write_ptr.st_rw_ptr.bit_rw_ptr);
    if (hmac_tx_set_msdu_info(mac_vap, ring_msdu_info, netbuf) != OAL_SUCC) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    hmac_record_netbuf_addr(tx_msdu_ring, netbuf);
    hmac_tx_msdu_ring_inc_write_ptr(tx_msdu_ring);

    return OAL_SUCC;
}


OAL_STATIC OAL_INLINE void hmac_tx_update_statistics(
    hmac_tid_info_stru *tid_info, hmac_msdu_info_ring_stru *tx_msdu_ring, oal_netbuf_stru *netbuf)
{
    tx_msdu_ring->msdu_cnt++;
    tx_msdu_ring->msdu_len += netbuf->len;

    tid_info->tx_msdus++;
    tid_info->last_period_tx_msdus++;
    tid_info->cur_msdus++;
    tid_info->cur_bytes += netbuf->len;
}


uint32_t hmac_tx_ring_push_msdu(mac_vap_stru *mac_vap, hmac_user_stru *hmac_user,
    hmac_tid_info_stru *tid_info, hmac_msdu_info_ring_stru *tx_msdu_ring, oal_netbuf_stru *netbuf)
{
    oal_spin_lock_bh(&tx_msdu_ring->lock);

    if (hmac_tx_set_msdu_info_to_ring(mac_vap, tx_msdu_ring, netbuf) != OAL_SUCC) {
        oal_spin_unlock_bh(&tx_msdu_ring->lock);
        return OAL_FAIL;
    }

    hmac_tx_update_statistics(tid_info, tx_msdu_ring, netbuf);

    hmac_tx_reg_write_ring_info(hmac_user, tid_info, TID_CMDID_ENQUE);

    if (tid_info->tx_msdus % hmac_user->tx_tid_info[tid_info->tid_no].tx_update_freq != 0) {
        oal_spin_unlock_bh(&tx_msdu_ring->lock);
        return OAL_SUCC;
    }

    oal_spin_unlock_bh(&tx_msdu_ring->lock);

    return hmac_tx_sync_ring_info(&hmac_user->st_user_base_info, tid_info, TID_CMDID_ENQUE);
}


void hmac_tx_ring_release_msdu(hmac_user_stru *hmac_user, uint8_t tid)
{
    hmac_tid_info_stru *hmac_tid_info = &hmac_user->tx_tid_info[tid];
    hmac_msdu_info_ring_stru *tx_ring = &hmac_tid_info->non_ba_tx_ring;

    oal_spin_lock_bh(&tx_ring->lock);

    if (hmac_tx_ring_rw_ptr_check(tx_ring) != OAL_TRUE) {
        oal_spin_unlock_bh(&tx_ring->lock);
        oam_warning_log3(0, OAM_SF_AMSDU, "{hmac_set_tx_hw_msdu_info_ring::tid[%d], rptr[%d], wptr[%d]}",
            tid, tx_ring->base_ring_info.read_index, tx_ring->base_ring_info.write_index);
        return;
    }

    hmac_tx_ring_release_all_netbuf(tx_ring, OAL_TRUE);

    oal_spin_unlock_bh(&tx_ring->lock);
}

#endif
