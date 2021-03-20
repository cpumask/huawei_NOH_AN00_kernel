

#include "oam_ext_if.h"
#include "frw_ext_if.h"

#include "wlan_chip_i.h"

#include "mac_device.h"
#include "mac_resource.h"
#include "mac_regdomain.h"
#include "mac_vap.h"

#include "hmac_resource.h"
#include "hmac_device.h"
#include "hmac_vap.h"
#include "hmac_rx_filter.h"

#include "hmac_chan_mgmt.h"
#include "hmac_dfs.h"
#include "hmac_rx_filter.h"

#include "hmac_hcc_adapt.h"

#include "hmac_dfs.h"
#include "hmac_config.h"
#include "hmac_resource.h"
#include "hmac_device.h"
#include "hmac_scan.h"
#include "hmac_rx_data.h"
#include "hmac_hcc_adapt.h"
#include "hmac_dfx.h"
#include "hmac_protection.h"

#ifdef _PRE_WLAN_TCP_OPT
#include "mac_data.h"
#include "hmac_tcp_opt_struc.h"
#include "hmac_tcp_opt.h"
#endif
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include <linux/fb.h>
#include <linux/list.h>
#include "plat_pm_wlan.h"
#endif

#include "hisi_customize_wifi.h"
#include "hal_device.h"

#include "securec.h"
#include "securectype.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_DEVICE_C

/* 2 ȫ�ֱ������� */
/*lint -e578*/ /*lint -e19*/
oal_module_license("GPL");
/*lint +e578*/ /*lint +e19*/

/* 3 ����ʵ�� */

uint32_t hmac_device_exit(mac_board_stru *pst_board, mac_chip_stru *pst_chip, hmac_device_stru *pst_hmac_device)
{
    mac_device_stru *pst_device = OAL_PTR_NULL;
    uint32_t ul_return;
    hmac_vap_stru *pst_vap = OAL_PTR_NULL;
    mac_cfg_down_vap_param_stru st_down_vap;
    uint8_t uc_vap_idx;
    uint32_t ul_ret;

    if (pst_hmac_device == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_device_exit::pst_hmac_device null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ɨ��ģ��ȥ��ʼ�� */
    hmac_scan_exit(pst_hmac_device);

    pst_device = pst_hmac_device->pst_device_base_info;
    if (pst_device == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_device_exit::pst_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ������ȥע�� */
#ifdef CONFIG_HAS_EARLYSUSPEND
    unregister_early_suspend(&pst_hmac_device->early_suspend);
#endif

    /* ��������vap��ʼ����HMAC������������VAPж��Ҳ��HMAC�� */
    uc_vap_idx = pst_device->uc_cfg_vap_id;
    pst_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(uc_vap_idx);
    if (pst_vap == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_device_exit::pst_vap null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    ul_return = hmac_config_del_vap(&pst_vap->st_vap_base_info,
                                    OAL_SIZEOF(mac_cfg_down_vap_param_stru),
                                    (uint8_t *)&st_down_vap);
    if (ul_return != OAL_SUCC) {
        oam_warning_log1(pst_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
                         "{hmac_device_exit::hmac_config_del_vap failed[%d].}", ul_return);
        return ul_return;
    }

    for (uc_vap_idx = 0; uc_vap_idx < pst_device->uc_vap_num; uc_vap_idx++) {
        /* ��ȡ���ұ�һλΪ1��λ������ֵ��Ϊvap�������±� */
        pst_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_device->auc_vap_id[uc_vap_idx]);
        if (pst_vap == OAL_PTR_NULL) {
            oam_warning_log1(0, OAM_SF_ANY,
                             "{hmac_device_exit::mac_res_get_hmac_vap failed vap_idx[%u].}", uc_vap_idx);
            continue;
        }

        ul_return = hmac_vap_destroy(pst_vap);
        if (ul_return != OAL_SUCC) {
            oam_warning_log1(pst_vap->st_vap_base_info.uc_vap_id, OAM_SF_ANY,
                             "{hmac_device_exit::hmac_vap_destroy failed[%d].}", ul_return);
            return ul_return;
        }
        pst_device->auc_vap_id[uc_vap_idx] = 0;
    }

    /* �ͷŹ����ṹ�� �Լ� ��Ӧ�������� */
    ul_ret = mac_device_exit(pst_device);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{mac_chip_exit::p_device_destroy_fun failed[%d].}", ul_ret);

        return ul_ret;
    }

    /* ָ�����mac device��ָ��Ϊ�� */
    pst_hmac_device->pst_device_base_info = OAL_PTR_NULL;

    return OAL_SUCC;
}


OAL_STATIC uint32_t hmac_chip_exit(mac_board_stru *pst_board, mac_chip_stru *pst_chip)
{
    hmac_device_stru *pst_hmac_device = OAL_PTR_NULL;
    uint32_t ul_ret;
    uint8_t uc_device;

    if (oal_unlikely(oal_any_null_ptr2(pst_chip, pst_board))) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_chip_exit::param null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    for (uc_device = 0; uc_device < pst_chip->uc_device_nums; uc_device++) {
        pst_hmac_device = hmac_res_get_mac_dev(pst_chip->auc_device_id[uc_device]);

        /* ��Ų��λ�� �ͷ���Դ */
        hmac_res_free_mac_dev(pst_chip->auc_device_id[uc_device]);

        ul_ret = hmac_device_exit(pst_board, pst_chip, pst_hmac_device);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log1(0, OAM_SF_ANY, "{hmac_chip_exit::hmac_device_exit failed[%d].}", ul_ret);
            return ul_ret;
        }
    }

    ul_ret = mac_chip_exit(pst_board, pst_chip);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_ANY, "{hmac_chip_exit::mac_chip_exit failed[%d].}", ul_ret);
        return ul_ret;
    }

    return OAL_SUCC;
}


uint32_t hmac_board_exit(mac_board_stru *pst_board)
{
    uint8_t uc_chip_idx;
    uint32_t ul_ret;
    uint8_t uc_chip_id_bitmap;

    if (oal_unlikely(pst_board == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_board_exit::pst_board null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    uc_chip_id_bitmap = pst_board->uc_chip_id_bitmap;
    while (uc_chip_id_bitmap != 0) {
        /* ��ȡ���ұ�һλΪ1��λ������ֵ��Ϊchip�������±� */
        uc_chip_idx = oal_bit_find_first_bit_one_byte(uc_chip_id_bitmap);
        if (oal_unlikely(uc_chip_idx >= WLAN_CHIP_MAX_NUM_PER_BOARD)) {
            oam_error_log2(0, OAM_SF_ANY, "{hmac_board_exit::invalid uc_chip_idx[%d] uc_chip_id_bitmap=%d.}",
                           uc_chip_idx, uc_chip_id_bitmap);
            return OAL_ERR_CODE_ARRAY_OVERFLOW;
        }

        ul_ret = hmac_chip_exit(pst_board, &(pst_board->ast_chip[uc_chip_idx]));
        if (ul_ret != OAL_SUCC) {
            oam_warning_log1(0, OAM_SF_ANY, "{hmac_board_exit::mac_chip_exit failed[%d].}", ul_ret);
            return ul_ret;
        }

        /* �����Ӧ��bitmapλ */
        oal_bit_clear_bit_one_byte(&uc_chip_id_bitmap, uc_chip_idx);
        /* �����Ӧ��bitmapλ */
        oal_bit_clear_bit_one_byte(&pst_board->uc_chip_id_bitmap, uc_chip_idx);
    }

    /* �������ֵĳ�ʼ�� */
    mac_board_exit(pst_board);

    return OAL_SUCC;
}


OAL_STATIC uint32_t hmac_cfg_vap_init(mac_device_stru *pst_device)
{
    int8_t ac_vap_netdev_name[MAC_NET_DEVICE_NAME_LENGTH];
    uint32_t ul_ret;
    hmac_vap_stru *pst_vap = OAL_PTR_NULL;

    /* ��ʼ�������У�ֻ��ʼ������vap������vap��Ҫͨ��������� */
    /*lint -e413*/
    ul_ret = mac_res_alloc_hmac_vap(&pst_device->uc_cfg_vap_id,
                                    OAL_OFFSET_OF(hmac_vap_stru, st_vap_base_info));
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_warning_log1(pst_device->uc_cfg_vap_id, OAM_SF_ANY,
                         "{hmac_cfg_vap_init::mac_res_alloc_hmac_vap failed[%d].}", ul_ret);
        return ul_ret;
    }
    /*lint +e413*/
    pst_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(pst_device->uc_cfg_vap_id);
    if (pst_vap == OAL_PTR_NULL) {
        oam_warning_log0(pst_device->uc_cfg_vap_id, OAM_SF_ANY, "{hmac_cfg_vap_init::pst_vap null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ��hmac_vap_stru�ṹ���ʼ����ֵΪ0 */
    memset_s(pst_vap, OAL_SIZEOF(hmac_vap_stru), 0, OAL_SIZEOF(hmac_vap_stru));

    {
        mac_cfg_add_vap_param_stru st_param = { 0 }; /* ��������VAP�����ṹ�� */
        st_param.en_vap_mode = WLAN_VAP_MODE_CONFIG;
        st_param.bit_11ac2g_enable = OAL_TRUE;
        st_param.bit_disable_capab_2ght40 = OAL_FALSE;
        ul_ret = hmac_vap_init(pst_vap,
                               pst_device->uc_chip_id,
                               pst_device->uc_device_id,
                               pst_device->uc_cfg_vap_id,
                               &st_param);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log1(pst_device->uc_cfg_vap_id, OAM_SF_ANY,
                             "{hmac_cfg_vap_init::hmac_vap_init failed[%d].}", ul_ret);
            return ul_ret;
        }
    }

    snprintf_s(ac_vap_netdev_name, MAC_NET_DEVICE_NAME_LENGTH, MAC_NET_DEVICE_NAME_LENGTH - 1,
               "Hisilicon%d", pst_device->uc_chip_id);

    ul_ret = hmac_vap_creat_netdev(pst_vap, ac_vap_netdev_name,
                                   sizeof(ac_vap_netdev_name),
                                   (int8_t *)(pst_device->auc_hw_addr));
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(pst_device->uc_cfg_vap_id, OAM_SF_ANY,
                         "{hmac_cfg_vap_init::hmac_vap_creat_netdev failed[%d].}", ul_ret);
        return ul_ret;
    }

    return OAL_SUCC;
}

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
extern oal_bool_enum g_wlan_host_pm_switch;

void hmac_do_suspend_action(mac_device_stru *pst_mac_device, uint8_t uc_in_suspend)
{
    hmac_device_stru *pst_hmac_device;
    uint32_t ul_ret;
    mac_vap_stru *pst_cfg_mac_vap = OAL_PTR_NULL;
    mac_cfg_suspend_stru st_suspend;
    uint32_t ul_is_wlan_poweron;

    pst_hmac_device = hmac_res_get_mac_dev(pst_mac_device->uc_device_id);
    if (pst_hmac_device == OAL_PTR_NULL) {
        oam_warning_log0(pst_mac_device->uc_cfg_vap_id, OAM_SF_ANY,
                         "{hmac_do_suspend_action::pst_hmac_device null.}");
        return;
    }

    oal_spin_lock(&pst_hmac_device->st_suspend_lock);

    pst_mac_device->uc_in_suspend = uc_in_suspend;

    pst_cfg_mac_vap = (mac_vap_stru *)mac_res_get_mac_vap(pst_mac_device->uc_cfg_vap_id);
    if (pst_cfg_mac_vap == OAL_PTR_NULL) {
        oam_warning_log0(pst_mac_device->uc_cfg_vap_id, OAM_SF_ANY, "{hmac_do_suspend_action::pst_vap null.}");
        oal_spin_unlock(&pst_hmac_device->st_suspend_lock);
        return;
    }

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    ul_is_wlan_poweron = wlan_pm_is_poweron();
#endif
    /* ����host�͹��Ĳ���device���ϵ����Ҫ��������״̬ͬ����device */
    if (g_wlan_host_pm_switch && ul_is_wlan_poweron) {
        st_suspend.uc_in_suspend = uc_in_suspend;

        if ((uc_in_suspend == OAL_TRUE)
#ifdef _PRE_WLAN_FEATURE_WAPI
            && (pst_hmac_device->pst_device_base_info->uc_wapi != OAL_TRUE)
#endif
        ) {
            pst_hmac_device->pst_device_base_info->uc_arpoffload_switch = OAL_TRUE;
            st_suspend.uc_arpoffload_switch = OAL_TRUE;
        } else {
            pst_hmac_device->pst_device_base_info->uc_arpoffload_switch = OAL_FALSE;
            st_suspend.uc_arpoffload_switch = OAL_FALSE;
        }
        hmac_wake_lock();
        /***************************************************************************
            ���¼���DMAC��, ͬ����Ļ����״̬��DMAC
        ***************************************************************************/
        ul_ret = hmac_config_send_event(pst_cfg_mac_vap,
                                        WLAN_CFGID_SUSPEND_ACTION_SYN,
                                        OAL_SIZEOF(mac_cfg_suspend_stru),
                                        (uint8_t *)&st_suspend);
        if (oal_unlikely(ul_ret != OAL_SUCC)) {
            oam_warning_log1(0, OAM_SF_CFG, "{hmac_suspend_action::hmac_config_send_event failed[%d]}", ul_ret);
        }
        hmac_wake_unlock();
    }
    oal_spin_unlock(&pst_hmac_device->st_suspend_lock);
}

#ifdef CONFIG_HAS_EARLYSUSPEND

void hmac_early_suspend(struct early_suspend *early_sup)
{
    hmac_device_stru *pst_hmac_device;

    pst_hmac_device = oal_container_of(early_sup, hmac_device_stru, early_suspend);
    hmac_do_suspend_action(pst_hmac_device->pst_device_base_info, OAL_TRUE);
}


void hmac_late_resume(struct early_suspend *early_sup)
{
    hmac_device_stru *pst_hmac_device;

    pst_hmac_device = oal_container_of(early_sup, hmac_device_stru, early_suspend);
    hmac_do_suspend_action(pst_hmac_device->pst_device_base_info, OAL_FALSE);
}
#endif
#endif  // end (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)

uint32_t hmac_send_evt2wal(mac_vap_stru *pst_mac_vap, uint8_t uc_evtid,
    uint8_t *puc_evt, uint32_t ul_evt_len)
{
    frw_event_mem_stru *pst_event_mem;
    frw_event_stru *pst_event = OAL_PTR_NULL;
    uint32_t ul_ret;

    pst_event_mem = frw_event_alloc_m((uint16_t)ul_evt_len);
    if (pst_event_mem == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, OAM_SF_SCAN, "{hmac_mgmt_scan_req_exception::pst_event_mem null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* ��д�¼� */
    pst_event = frw_get_event_stru(pst_event_mem);

    frw_event_hdr_init(&(pst_event->st_event_hdr),
                       FRW_EVENT_TYPE_HOST_CTX,
                       uc_evtid,
                       (uint16_t)ul_evt_len,
                       FRW_EVENT_PIPELINE_STAGE_0,
                       pst_mac_vap->uc_chip_id,
                       pst_mac_vap->uc_device_id,
                       pst_mac_vap->uc_vap_id);

    if (EOK != memcpy_s((void *)pst_event->auc_event_data, ul_evt_len, (void *)puc_evt, ul_evt_len)) {
        oam_error_log0(0, OAM_SF_SCAN, "hmac_send_evt2wal::memcpy fail!");
        frw_event_free_m(pst_event_mem);
        return OAL_FAIL;
    }

    /* �ַ��¼� */
    ul_ret = frw_event_dispatch_event(pst_event_mem);
    frw_event_free_m(pst_event_mem);
    return ul_ret;
}

uint32_t hmac_config_host_dev_init(mac_vap_stru *pst_mac_vap, uint16_t us_len, uint8_t *puc_param)
{
    mac_device_stru *pst_mac_device = OAL_PTR_NULL;
    uint32_t ul_loop;
    hmac_device_stru *pst_hmac_device = OAL_PTR_NULL;

    if (pst_mac_vap == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_device_init:: pst_mac_device NULL pointer!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_mac_device = mac_res_get_dev(pst_mac_vap->uc_device_id);
    if (pst_mac_device == OAL_PTR_NULL) {
        oam_error_log1(0, OAM_SF_ANY, "{hmac_device_init:: pst_mac_device[%d] NULL pointer!}",
                       pst_mac_vap->uc_device_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

    for (ul_loop = 0; ul_loop < MAC_MAX_SUPP_CHANNEL; ul_loop++) {
        pst_mac_device->st_ap_channel_list[ul_loop].us_num_networks = 0;
        pst_mac_device->st_ap_channel_list[ul_loop].en_ch_type = MAC_CH_TYPE_NONE;
    }

    pst_hmac_device = hmac_res_get_mac_dev(pst_mac_vap->uc_device_id);
    if (oal_unlikely(pst_hmac_device == OAL_PTR_NULL)) {
        oam_error_log1(0, OAM_SF_ANY, "{hmac_config_host_dev_init::pst_hmac_device[%d] null!}",
                       pst_mac_vap->uc_device_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

    return OAL_SUCC;
}


uint32_t hmac_config_host_dev_exit(mac_vap_stru *pst_mac_vap)
{
    hmac_device_stru *pst_hmac_device = hmac_res_get_mac_dev(pst_mac_vap->uc_device_id);
    if (oal_unlikely(pst_hmac_device == OAL_PTR_NULL)) {
        oam_error_log1(0, OAM_SF_ANY, "{hmac_config_host_dev_exit::pst_hmac_device[%d] null!}",
                       pst_mac_vap->uc_device_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

#ifdef CONFIG_ARCH_KIRIN_PCIE
#ifndef _PRE_LINUX_TEST
    /* wifi �µ�������ͷ�rx ring�����Դ */
    hal_device_reset_rx_res();
#endif
#endif
    return OAL_SUCC;
}

#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE

void hmac_device_cap_init_customize(hmac_device_stru *pst_hmac_dev)
{
    wlan_chip_update_aput_160M_enable(&(pst_hmac_dev->en_ap_support_160m));
    oam_warning_log1(0, 0, "hmac_device_cap_init_customize::APUT 160M enable[%d]", pst_hmac_dev->en_ap_support_160m);
}
#endif


OAL_STATIC void hmac_device_init_ext(hmac_device_stru *pst_hmac_device)
{
    /* ɨ��ģ���ʼ�� */
    hmac_scan_init(pst_hmac_device);

    /* ��ʼ��P2P �ȴ����� */
    oal_wait_queue_init_head(&(pst_hmac_device->st_netif_change_event));
    /* ��ʼ���͹���֡����ͳ�����ݲ�ѯ�ȴ����� */
    oal_wait_queue_init_head(&(pst_hmac_device->st_psm_flt_stat_query.st_wait_queue));
#ifdef _PRE_WLAN_TCP_OPT
    pst_hmac_device->sys_tcp_tx_ack_opt_enable = DEFAULT_TX_TCP_ACK_OPT_ENABLE;
    pst_hmac_device->sys_tcp_rx_ack_opt_enable = DEFAULT_RX_TCP_ACK_OPT_ENABLE;
#endif

#ifdef _PRE_WLAN_FEATURE_MONITOR
    pst_hmac_device->ul_mgmt_frames_cnt = 0;
    pst_hmac_device->ul_control_frames_cnt = 0;
    pst_hmac_device->ul_data_frames_cnt = 0;
    pst_hmac_device->ul_others_frames_cnt = 0;
#endif

#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
    hmac_device_cap_init_customize(pst_hmac_device);
#endif
}


uint32_t hmac_device_init(uint8_t *puc_device_id, mac_chip_stru *pst_chip)
{
    uint8_t uc_dev_id;
    mac_device_stru *pst_mac_device = OAL_PTR_NULL;
    hmac_device_stru *pst_hmac_device = OAL_PTR_NULL;
    uint32_t ul_ret;
    uint32_t ul_loop;

    /* ���빫��mac device�ṹ�� */
    ul_ret = mac_res_alloc_hmac_dev(&uc_dev_id);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_error_log1(0, OAM_SF_ANY, "{hmac_device_init::mac_res_alloc_dmac_dev failed[%d].}", ul_ret);

        return OAL_FAIL;
    }

    /* ��ȡmac device�ṹ��ָ�� */
    pst_mac_device = mac_res_get_dev(uc_dev_id);
    if (pst_mac_device == OAL_PTR_NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_device_init::pst_device null.}");

        return OAL_ERR_CODE_PTR_NULL;
    }

    ul_ret = mac_device_init(pst_mac_device, pst_chip->ul_chip_ver, pst_chip->uc_chip_id, uc_dev_id);
    if (ul_ret != OAL_SUCC) {
        oam_error_log2(0, OAM_SF_ANY, "{hmac_device_init::mac_device_init failed[%d], chip_ver[0x%x].}",
                       ul_ret, pst_chip->ul_chip_ver);

        mac_res_free_dev(uc_dev_id);
        return ul_ret;
    }

    /* ����hmac device��Դ */
    if (oal_unlikely(hmac_res_alloc_mac_dev(uc_dev_id) != OAL_SUCC)) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_device_init::hmac_res_alloc_mac_dev failed.}");
        return OAL_FAIL;
    }

    /* ��ȡhmac device����������ز�����ֵ */
    pst_hmac_device = hmac_res_get_mac_dev(uc_dev_id);
    if (oal_unlikely(pst_hmac_device == OAL_PTR_NULL)) {
        oam_error_log1(0, OAM_SF_ANY, "{hmac_device_init::pst_hmac_device[%] null!}", uc_dev_id);
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* �ṹ���ʼ�� */
    memset_s(pst_hmac_device, OAL_SIZEOF(hmac_device_stru), 0, OAL_SIZEOF(hmac_device_stru));

    pst_hmac_device->pst_device_base_info = pst_mac_device;

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    oal_spin_lock_init(&pst_hmac_device->st_suspend_lock);
#ifdef CONFIG_HAS_EARLYSUSPEND
    pst_hmac_device->early_suspend.level = EARLY_SUSPEND_LEVEL_BLANK_SCREEN + 20;
    pst_hmac_device->early_suspend.suspend = hmac_early_suspend;
    pst_hmac_device->early_suspend.resume = hmac_late_resume;
    register_early_suspend(&pst_hmac_device->early_suspend);
#endif
    oal_spin_lock(&pst_hmac_device->st_suspend_lock);
    pst_mac_device->uc_in_suspend = OAL_FALSE;
    oal_spin_unlock(&pst_hmac_device->st_suspend_lock);
#endif

    hmac_device_init_ext(pst_hmac_device);

    for (ul_loop = 0; ul_loop < MAC_MAX_SUPP_CHANNEL; ul_loop++) {
        pst_mac_device->st_ap_channel_list[ul_loop].us_num_networks = 0;
        pst_mac_device->st_ap_channel_list[ul_loop].en_ch_type = MAC_CH_TYPE_NONE;
    }

    /* ���θ�ֵ��CHIP����Ҫ�����device id */
    *puc_device_id = uc_dev_id;

    /* ����vap��ʼ�� */
    ul_ret = hmac_cfg_vap_init(pst_mac_device);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_error_log1(0, OAM_SF_ANY, "{hmac_chip_init::hmac_cfg_vap_init failed[%d].}", ul_ret);
        return OAL_FAIL;
    }

#ifdef _PRE_WLAN_FEATURE_DFS
    hmac_dfs_init(pst_mac_device);
#endif

    return OAL_SUCC;
}


#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
extern uint8_t g_auc_mac_device_radio_cap[];
#endif  // #ifdef _PRE_PLAT_FEATURE_CUSTOMIZE

OAL_STATIC uint32_t hmac_chip_init(mac_chip_stru *pst_chip, uint8_t uc_chip_id)
{
    uint8_t uc_device;
    uint32_t ul_ret;
    uint8_t uc_device_max;

    oam_info_log0(0, OAM_SF_ANY, "{hmac_chip_init::func enter.}");

    pst_chip->uc_chip_id = uc_chip_id;

    /* OAL�ӿڻ�ȡ֧��device���� */
    uc_device_max = oal_chip_get_device_num(pst_chip->ul_chip_ver);
    if (uc_device_max == 0) {
        oam_error_log0(0, OAM_SF_ANY, "{hmac_chip_init::device max num is zero.}");
        return OAL_FAIL;
    }

    if (uc_device_max > WLAN_SERVICE_DEVICE_MAX_NUM_PER_CHIP) {
        oam_error_log2(0, OAM_SF_ANY, "{hmac_chip_init::device max num is %d,more than %d.}",
                       uc_device_max, WLAN_SERVICE_DEVICE_MAX_NUM_PER_CHIP);
        return OAL_FAIL;
    }

    for (uc_device = 0; uc_device < uc_device_max; uc_device++) {
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
        if (g_auc_mac_device_radio_cap[uc_device] == MAC_DEVICE_DISABLE) {
            oam_error_log1(0, OAM_SF_ANY, "{hmac_chip_init::mac device id[%d] disable.}", uc_device);
            continue;
        }
#endif  // #ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
        /* hmac device�ṹ��ʼ�� */
        ul_ret = hmac_device_init(&pst_chip->auc_device_id[uc_device], pst_chip);
        if (oal_unlikely(ul_ret != OAL_SUCC)) {
            oam_error_log1(0, OAM_SF_ANY, "{hmac_chip_init::hmac_device_init failed[%d].}", ul_ret);
            return OAL_FAIL;
        }
    }

    mac_chip_init(pst_chip, uc_device_max);

    oam_info_log0(0, OAM_SF_ANY, "{hmac_chip_init::func out.}");

    return OAL_SUCC;
}


uint32_t hmac_board_init(mac_board_stru *pst_board)
{
    uint8_t uc_chip;
    uint32_t ul_ret;
    uint32_t ul_chip_max_num;

    mac_board_init();

    /* chip֧�ֵ��������PCIe���ߴ����ṩ; */
    ul_chip_max_num = oal_bus_get_chip_num();

    for (uc_chip = 0; uc_chip < ul_chip_max_num; uc_chip++) {
        ul_ret = hmac_chip_init(&pst_board->ast_chip[uc_chip], uc_chip);
        if (ul_ret != OAL_SUCC) {
            oam_warning_log2(0, OAM_SF_ANY,
                             "{hmac_init_event_process::hmac_chip_init failed[%d], uc_chip_id[%d].}",
                             ul_ret, uc_chip);
            return OAL_FAIL;
        }

        oal_bit_set_bit_one_byte(&pst_board->uc_chip_id_bitmap, uc_chip);
    }
    return OAL_SUCC;
}

#if (_PRE_WLAN_FEATURE_PMF != _PRE_PMF_NOT_SUPPORT)
oal_bool_enum_uint8 hmac_device_pmf_find_black_list(hmac_device_stru *pst_hmac_dev, uint8_t *puc_mac_addr)
{
    uint8_t uc_loop_idx;

    for (uc_loop_idx = 0; uc_loop_idx < pst_hmac_dev->st_pmf_black_list.uc_cnt; ++uc_loop_idx) {
        if (0 == oal_memcmp(pst_hmac_dev->st_pmf_black_list.auc_black_list[uc_loop_idx],
                            puc_mac_addr, WLAN_MAC_ADDR_LEN)) {
            return OAL_TRUE;
        }
    }

    return OAL_FALSE;
}

void hmac_device_pmf_add_black_list(hmac_device_stru *pst_hmac_dev, uint8_t *puc_mac_addr)
{
    uint8_t uc_black_add_pos;

    if (OAL_TRUE == hmac_device_pmf_find_black_list(pst_hmac_dev, puc_mac_addr)) {
        return;
    }

    /* �������б�δ�������뵽�б�ĩβ�����º��������������򸲸������һ�� */
    if (pst_hmac_dev->st_pmf_black_list.uc_cnt < PMF_BLACK_LIST_MAX_CNT) {
        uc_black_add_pos = pst_hmac_dev->st_pmf_black_list.uc_cnt++;
    } else {
        uc_black_add_pos = pst_hmac_dev->st_pmf_black_list.uc_first_idx++;
        pst_hmac_dev->st_pmf_black_list.uc_first_idx %= PMF_BLACK_LIST_MAX_CNT;
    }

    if (EOK != memcpy_s(pst_hmac_dev->st_pmf_black_list.auc_black_list[uc_black_add_pos],
                        WLAN_MAC_ADDR_LEN, puc_mac_addr, WLAN_MAC_ADDR_LEN)) {
        oam_error_log0(0, OAM_SF_ANY, "hmac_pmf_add_black_list::memcpy fail!");
        return;
    }

    oam_warning_log4(0, OAM_SF_ANY, "hmac_pmf_add_black_list::cnt[%d] mac addr %02X:xx:xx:xx:%02X:%02X",
        pst_hmac_dev->st_pmf_black_list.uc_cnt, puc_mac_addr[0], puc_mac_addr[4], puc_mac_addr[5]); // mac��ַ��0,4,5�ֽ�
}
#endif


void hmac_device_create_random_mac_addr(mac_device_stru *pst_mac_dev, mac_vap_stru *pst_mac_vap)
{
    hmac_device_stru *pst_hmac_dev = OAL_PTR_NULL;
    oal_bool_enum_uint8 en_is_random_mac_addr_scan;

    if (!IS_LEGACY_STA(pst_mac_vap)) {
        return;
    }

    pst_hmac_dev = hmac_res_get_mac_dev(pst_mac_dev->uc_device_id);
    if (pst_hmac_dev == OAL_PTR_NULL) {
        oam_error_log0(pst_mac_vap->uc_vap_id, 0, "hmac_device_create_random_mac_addr::hmac device is null");
        return;
    }

#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
    en_is_random_mac_addr_scan = g_wlan_cust->uc_random_mac_addr_scan;
#else
    en_is_random_mac_addr_scan = pst_hmac_dev->st_scan_mgmt.en_is_random_mac_addr_scan;
#endif

    if (en_is_random_mac_addr_scan == OAL_FALSE) {
        return;
    }

    oal_random_ether_addr(pst_hmac_dev->st_scan_mgmt.auc_random_mac);
    pst_hmac_dev->st_scan_mgmt.auc_random_mac[0] = pst_mac_dev->auc_mac_oui[0] & 0xfe; /* ��֤�ǵ���mac */
    pst_hmac_dev->st_scan_mgmt.auc_random_mac[1] = pst_mac_dev->auc_mac_oui[1];
    pst_hmac_dev->st_scan_mgmt.auc_random_mac[2] = pst_mac_dev->auc_mac_oui[2];  // ��mac��ַoui��2�ֽ�ֵ��ֵ�����mac��2�ֽ�

    oam_warning_log4(pst_mac_vap->uc_vap_id, OAM_SF_SCAN,
                     "{hmac_device_create_random_mac_addr::rand_mac_addr[%02X:XX:XX:%02X:%02X:%02X].}",
                     pst_hmac_dev->st_scan_mgmt.auc_random_mac[0],
                     pst_hmac_dev->st_scan_mgmt.auc_random_mac[3],   // ���MAC��3�ֽ�
                     pst_hmac_dev->st_scan_mgmt.auc_random_mac[4],   // ���MAC��4�ֽ�
                     pst_hmac_dev->st_scan_mgmt.auc_random_mac[5]);  // ���MAC��5�ֽ�

    return;
}
