
/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#define HISI_LOG_TAG    "[WIFI_MAIN]"
#include "main.h"
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include "sdt_drv.h"
#elif ((_PRE_OS_VERSION_WIN32 == _PRE_OS_VERSION)||(_PRE_OS_VERSION_WIN32_RAW == _PRE_OS_VERSION))
#include "mac_resource.h"
#endif

#if (defined(_PRE_PRODUCT_ID_HI110X_HOST))
#include "hmac_ext_if.h"
#include "wal_ext_if.h"
#include "dmac_ext_if.h"
#include "oal_kernel_file.h"

#elif (defined(_PRE_PRODUCT_ID_HI110X_DEV))
#include "oam_log.h"
#include "oal_main.h"
#include "uart.h"
#include "oam_msgsendrecv.h"
#include "oam_data_send.h"
#include "uart.h"

#include "oal_hcc_slave_if.h"
#include "pm_extern.h"

#include "hal_ext_if.h"

#include "dmac_ext_if.h"
#include "dmac_alg.h"

#include "dmac_pm_sta.h"
#include "hal_rf.h"

#include "dcxo_manager.h"

#ifdef _PRE_WLAN_FEATURE_BTCOEX
#include "dmac_btcoex.h"
#endif

#ifdef _PRE_WLAN_ALG_ENABLE
#include "alg_ext_if.h"
#endif

#include "oal_interface_for_rom.h"

#elif(_PRE_PRODUCT_ID_HI1151==_PRE_PRODUCT_ID)
#include "hal_ext_if.h"
#include "dmac_ext_if.h"
#include "dmac_alg.h"
#ifdef _PRE_WLAN_ALG_ENABLE
#include "alg_ext_if.h"
#endif
#include "hmac_ext_if.h"
#include "wal_ext_if.h"
#endif

#if (defined(_PRE_PRODUCT_ID_HI110X_HOST))
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
#include "hisi_customize_wifi.h"
#endif /* #ifdef _PRE_PLAT_FEATURE_CUSTOMIZE */
#endif

#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_MAIN_C
/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
oal_void platform_module_exit(oal_uint16 us_bitmap);
oal_void builder_module_exit(oal_uint16 us_bitmap);
/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/

oal_void builder_module_exit(oal_uint16 us_bitmap)
{
#if ((!defined(_PRE_PRODUCT_ID_HI110X_DEV)) || (_PRE_OS_VERSION_WIN32_RAW == _PRE_OS_VERSION))
#if (!defined(_PRE_PRODUCT_ID_HI110X_DEV))
        if (BIT8 & us_bitmap) {
            wal_main_exit();
        }
        if (BIT7 & us_bitmap) {
            hmac_main_exit();
        }
#elif (!defined(_PRE_PRODUCT_ID_HI110X_HOST))
#ifdef _PRE_WLAN_ALG_ENABLE
            if (BIT6 & us_bitmap) {
                alg_main_exit();
            }
#endif
        if (BIT5 & us_bitmap) {
            dmac_main_exit();
        }
        if (BIT4 & us_bitmap) {
            hal_main_exit();
        }
        platform_module_exit(us_bitmap);
#endif
#endif
    return;
}

#if ((_PRE_OS_VERSION_WIN32 == _PRE_OS_VERSION)&&(defined(_PRE_PRODUCT_ID_HI110X_HOST)))

oal_uint32 host_test_get_chip_msg(oal_void)
{
    oal_uint32             ul_return;
    mac_chip_stru         *pst_chip;
    frw_event_mem_stru    *pst_event_mem;
    frw_event_stru        *pst_event;             /* �¼��ṹ�� */
    oal_uint32             ul_dev_id;
    oal_netbuf_stru       *pst_netbuf;
    dmac_tx_event_stru    *pst_ctx_event;
    oal_uint8             *pst_mac_rates_11g;
    pst_event_mem = FRW_EVENT_ALLOC(OAL_SIZEOF(dmac_tx_event_stru));
    if (OAL_UNLIKELY(pst_event_mem == OAL_PTR_NULL)) {
        OAL_IO_PRINT("host_test_get_chip_msg: hmac_init_event_process FRW_EVENT_ALLOC result = OAL_PTR_NULL.\n");
        return OAL_FAIL;
    }

    /* ����netbuf�ڴ� */
    pst_netbuf = OAL_MEM_NETBUF_ALLOC(OAL_NORMAL_NETBUF, WLAN_MEM_NETBUF_SIZE2, OAL_NETBUF_PRIORITY_MID);
    if (OAL_UNLIKELY(pst_netbuf == OAL_PTR_NULL)) {
        OAL_IO_PRINT("host_test_get_chip_msg: hmac_init_event_process OAL_MEM_NETBUF_ALLOC result = OAL_PTR_NULL.\n");
        return OAL_FAIL;
    }

    pst_event                 = (frw_event_stru *)pst_event_mem->puc_data;
    pst_ctx_event             = (dmac_tx_event_stru *)pst_event->auc_event_data;
    pst_ctx_event->pst_netbuf = pst_netbuf;
    pst_mac_rates_11g = (oal_uint8*)oal_netbuf_data(pst_ctx_event->pst_netbuf);
    pst_chip = (mac_chip_stru *)(pst_mac_rates_11g + sizeof(mac_data_rate_stru) * MAC_DATARATES_PHY_80211G_NUM);

    ul_dev_id = (oal_uint32) oal_queue_dequeue(&(g_st_mac_res.st_dev_res.st_queue));
    /* 0Ϊ��Чֵ */
    if (ul_dev_id == 0) {
        OAL_IO_PRINT("host_test_get_chip_msg:oal_queue_dequeue return 0!");
        FRW_EVENT_FREE(pst_event_mem);
        return OAL_FAIL;
    }
    pst_chip->auc_device_id[0] = (oal_uint8)(ul_dev_id - 1);

    /* ����ul_chip_ver��ͨ��hal_chip_init_by_version������� */
    pst_chip->uc_device_nums = 1;
    pst_chip->uc_chip_id = 0;
    pst_chip->en_chip_state = OAL_TRUE;

    /* ��hal_chip_get_version�����õ�,1102 02��ҪSOC�ṩ�Ĵ�����ʵ�� */
    pst_chip->ul_chip_ver = WLAN_CHIP_VERSION;
    pst_chip->pst_chip_stru = OAL_PTR_NULL;
    ul_return = hmac_init_event_process(pst_event_mem);
    if (OAL_UNLIKELY(ul_return != OAL_SUCC)) {
        OAL_IO_PRINT("host_test_get_chip_msg: hmac_init_event_process  ul_return != OAL_SUCC\n");
        FRW_EVENT_FREE(pst_event_mem);
        oal_netbuf_free(pst_netbuf);
        return OAL_FAIL;
    }
    return OAL_SUCC;
}
#endif
#if  (defined(HI110x_EDA))

oal_uint32 device_test_create_cfg_vap(oal_void)
{
    oal_uint32          ul_return;
    frw_event_mem_stru *pst_event_mem;
    frw_event_stru     *pst_event;

    pst_event_mem = FRW_EVENT_ALLOC(0);
    if (OAL_UNLIKELY(pst_event_mem == OAL_PTR_NULL)) {
        OAL_IO_PRINT("device_test_create_cfg_vap: hmac_init_event_process FRW_EVENT_ALLOC result = OAL_PTR_NULL.\n");
        return OAL_FAIL;
    }

    ul_return = dmac_init_event_process(pst_event_mem);
    if (ul_return != OAL_SUCC) {
        OAL_IO_PRINT("device_test_create_cfg_vap: dmac_init_event_process result = fale.\n");
        FRW_EVENT_FREE(pst_event_mem);
        return OAL_FAIL;
    }

    pst_event = (frw_event_stru *)pst_event_mem->puc_data;
    pst_event->st_event_hdr.uc_device_id = 0;

    ul_return = dmac_cfg_vap_init_event(pst_event_mem);
    if (ul_return != OAL_SUCC) {
        FRW_EVENT_FREE(pst_event_mem);
        return ul_return;
    }

    FRW_EVENT_FREE(pst_event_mem);
    return OAL_SUCC;
}
#endif

#if ((defined(_PRE_PRODUCT_ID_HI110X_DEV))||(_PRE_PRODUCT_ID_HI1151==_PRE_PRODUCT_ID))

oal_void platform_module_exit(oal_uint16 us_bitmap)
{
    if (BIT3 & us_bitmap) {
        frw_main_exit();
    }

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#endif
    if (BIT1 & us_bitmap) {
        oam_main_exit();
    }
    if (BIT0 & us_bitmap) {
        oal_main_exit();
    }
    return;
}


oal_int32 platform_module_init(oal_void)
{
    oal_int32  l_return;
    oal_uint16  us_bitmap  = 0;

    // ���￼�ǹ���dfr�ָ���������Ҫ�����һ���쳣ʱ���״̬��־
    if (dcxo_support()) {
        dcxo_flg_clean(SUB_WIFI);
    }
    l_return = oal_main_init();
    if (l_return != OAL_SUCC) {
        OAL_IO_PRINT("platform_module_init: oal_main_init return error code: %d\r\n", l_return);
        return l_return;
    }
#if (!defined(HI110x_EDA))
    l_return = oam_main_init();
    if (l_return != OAL_SUCC) {
        OAL_IO_PRINT("platform_module_init: oam_main_init return error code: %d\r\n", l_return);
        us_bitmap = BIT0;
        builder_module_exit(us_bitmap);/*lint !e522*/
        return l_return;
    }
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#endif
#endif
    l_return = frw_main_init();
    if (l_return != OAL_SUCC) {
        OAL_IO_PRINT("platform_module_init: frw_main_init return error code: %d\r\n", l_return);
        us_bitmap = BIT0 | BIT1 | BIT2;
        builder_module_exit(us_bitmap);/*lint !e522*/
        return l_return;
    }

    /* ������ɺ������ӡ */
    OAL_IO_PRINT("platform_module_init:: platform_main_init finish!\r\n");

    return OAL_SUCC;
}


oal_int32  device_module_init(oal_void)
{
    oal_int32  l_return;
    oal_uint16 us_bitmap = 0;
    l_return = hal_main_init();
    if (l_return != OAL_SUCC) {
        OAL_IO_PRINT("device_module_init: hal_main_init return error code: %d", l_return);
        return l_return;
    }
    l_return = dmac_main_init();
    if (l_return != OAL_SUCC) {
        OAL_IO_PRINT("device_module_init: dmac_main_init return error code: %d", l_return);
        us_bitmap = BIT4;
        builder_module_exit(us_bitmap);/*lint !e522*/
        return l_return;
    }
#if (!defined(HI110x_EDA))
#if defined(_PRE_WLAN_ALG_ENABLE) 
    l_return = alg_main_init();
    if (l_return != OAL_SUCC) {
        OAL_IO_PRINT("device_module_init: alg_main_init return error code : %d", l_return);
        us_bitmap = BIT4 | BIT5;
        builder_module_exit(us_bitmap);/*lint !e522*/
        return l_return;
    }
#endif
#endif
    /* ������ɺ������ӡ */
    OAL_IO_PRINT("device_module_init:: device_module_init finish!\r\n");
    return OAL_SUCC;
}
#endif
#if ((defined(_PRE_PRODUCT_ID_HI110X_HOST))||(_PRE_PRODUCT_ID_HI1151==_PRE_PRODUCT_ID))
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE

oal_uint32 hwifi_config_host_global_dts_param(oal_void)
{
    /* ��ȡ5g���� */
    mac_set_band_5g_enabled(!!hwifi_get_init_value(CUS_TAG_DTS, WLAN_CFG_DTS_BAND_5G_ENABLE));
    return OAL_SUCC;
}
#endif /* #ifdef _PRE_PLAT_FEATURE_CUSTOMIZE */

oal_int32  host_module_init(oal_void)
{
    oal_int32  l_return;
    oal_uint16 us_bitmap = 0;

#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
    /* �����ƻ������ļ�&NVRAM */
    hwifi_custom_host_force_read_cfg_init();
    hwifi_config_host_global_dts_param();
#endif /* #ifdef _PRE_PLAT_FEATURE_CUSTOMIZE */

    l_return = hmac_main_init();
    if (l_return != OAL_SUCC) {
        OAL_IO_PRINT("host_module_init: hmac_main_init return error code: %d", l_return);
        return l_return;
    }
    l_return = wal_main_init();
    if (l_return != OAL_SUCC) {
        OAL_IO_PRINT("host_module_init: wal_main_init return error code: %d", l_return);
        us_bitmap = BIT7;
        builder_module_exit(us_bitmap);
        return l_return;
    }
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)&&(_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    wlan_pm_set_custom_cali_done(OAL_FALSE);
    wlan_pm_open();
#endif
    /* ������ɺ������ӡ */
    OAL_IO_PRINT("host_module_init:: host_main_init finish!");
    return OAL_SUCC;
}
#endif

#if (defined(_PRE_PRODUCT_ID_HI110X_DEV))

oal_int32  hi1102_device_main_init(oal_void)
{
    oal_int32  l_return;
    oal_uint16  us_bitmap;
    l_return = platform_module_init();
    if (l_return != OAL_SUCC) {
        OAL_IO_PRINT("host_bottom_main_init: platform_module_init return error code: %d\r\n", l_return);
        return l_return;
    }
#ifndef HI110x_EDA
    /* ����pktram�ڴ�,02A PKTMEM �ڴ��Ӧ��ϵ����:
    0/1 128k 0x60000000~0x6001FFFF-----��ӦƬѡ0
    1/2 128k 0x60020000~0x6003FFFF-----��ӦƬѡ1
    3/4 32k  ----��ӦƬѡ3
    �����ڴ�0x60010000~0x6003FFFF(IQ��H����0x60010000~0x6001FFFF,
    pktmem�������ݵĲ���ռ��0x60020000~0x6002FFFF,
    pktmem���͵���������ռ��0x6003000~0x6003FFFF) */
    g_puc_matrix_data = (oal_uint8 *)OAL_MEM_SAMPLE_NETBUF_ALLOC(3, 1); /* 3/4 32k  ----��ӦƬѡ3 */
    if (g_puc_matrix_data == OAL_PTR_NULL) {
        OAM_ERROR_LOG0(0, 0, "{hi110x_device_main_init:matrix data room alloc failed.}\r\n");
    }
#endif
    l_return = device_module_init();
    if (l_return != OAL_SUCC) {
        OAL_IO_PRINT("host_bottom_main_init: device_module_init return error code: %d\r\n", l_return);
        us_bitmap = BIT0 | BIT1 | BIT2 | BIT3;
        builder_module_exit(us_bitmap);/*lint !e522*/
        return l_return;
    }
#if (!defined(HI110x_EDA))
    /* device_ready:����HCC�ӿ�֪ͨHmac,Dmac�Ѿ���ɳ�ʼ�� */
    pfn_SDIO_SendMsgSync(D2H_MSG_WLAN_READY);
#endif
    /* ������ɺ������ӡ */
    OAL_IO_PRINT("Hi1102_device_main_init:: Hi1102_device_main_init finish!\r\n");

    return OAL_SUCC;
}


oal_void device_main_init(oal_void)
{
    /* init */
#ifndef _PRE_EDA
    oal_int32 l_return;
    l_return = hi1102_device_main_init();
    if (l_return != OAL_SUCC) {
        OAL_IO_PRINT("device_main_function: Hi1102_device_main_init return error code: %d", l_return);
        /* ��ʼ��ʧ�ܲ��˳������򣬵ȴ����� */
        for (;;) {
            ;
        }
    }
#endif
    OAL_IO_PRINT("device_main_function: hi1102_device_main_init succ!!\r\n");
#if (SUB_SYSTEM == SUB_SYS_WIFI)
    pm_wlan_isr_register();
#if (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1102_DEV)
    pm_wlan_func_register(device_psm_main_function, dmac_psm_check_hw_txq_state, dmac_psm_check_txrx_state,
                          dmac_psm_clean_state, dmac_psm_save_start_dma, dmac_psm_save_ps_state,
                          dmac_psm_recover_no_powerdown, dmac_psm_recover_start_dma, dmac_psm_recover_powerdown,
                          dmac_psm_cbb_stopwork, dmac_psm_rf_sleep, dmac_psm_sync_tsf_to_sta, dmac_psm_sync_tsf_to_ap,
                          dmac_psm_is_fake_queues_empty);
#elif (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1102A_DEV)
    pm_wlan_func_register(device_psm_main_function, dmac_psm_check_hw_txq_state, dmac_psm_check_txrx_state,
                          dmac_psm_clean_state, dmac_psm_save_start_dma, dmac_psm_save_ps_state,
                          dmac_psm_recover_no_powerdown, dmac_psm_recover_start_dma, dmac_psm_recover_powerdown,
                          dmac_psm_cbb_stopwork, dmac_psm_rf_sleep, hal_rf_temperature_trig_online_rxdc_cali,
                          dmac_psm_deep_sleep_notify, dmac_psm_deep_sleep_wakeup_notify);
#endif
#endif
    g_gpio_int_count = 0;
}


oal_uint8 device_psm_main_function(oal_void)
{
#if defined(_PRE_WLAN_FEATURE_BTCOEX) 
    mac_device_stru         *pst_mac_device;
    hal_to_dmac_device_stru *pst_hal_device;
#endif

#if defined(_PRE_WLAN_FEATURE_BTCOEX) 
    pst_mac_device = mac_res_get_dev(0);
    if (OAL_UNLIKELY(pst_mac_device == OAL_PTR_NULL)) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{device_psm_main_function::pst_device[id:0] is NULL!}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    pst_hal_device = pst_mac_device->pst_device_stru;
#endif
#ifdef _PRE_WLAN_FEATURE_BTCOEX
    hal_btcoex_process_bt_status(pst_hal_device, OAL_TRUE);
#endif
    device_main_function();
    return OAL_SUCC;
}

#elif (defined(_PRE_PRODUCT_ID_HI110X_HOST))
#include "hmac_vap.h"
#include "oal_hcc_host_if.h"


oal_int32  hi1102_host_main_init(oal_void)
{
    oal_int32  l_return;
    OAL_IO_PRINT("hi1102_host_main_init:: Hi1102_host_main_init enter!\n");

#ifdef _PRE_WLAN_FEATURE_OFFLOAD_FLOWCTL
    hcc_flowctl_get_device_mode_register(hmac_flowctl_check_device_is_sta_mode);
    hcc_flowctl_operate_subq_register(hmac_vap_net_start_subqueue, hmac_vap_net_stop_subqueue);
#else
    hcc_tx_flow_ctrl_cb_register(hmac_vap_net_stopall, hmac_vap_net_startall);
#endif

    l_return = host_module_init();
    if (l_return != OAL_SUCC) {
        OAL_IO_PRINT("Hi1102_host_main_init: host_module_init return error code: %d", l_return);
        return l_return;
    }

#ifdef _PRE_WLAN_FEATURE_ARP_OFFLOAD
    wal_hipriv_register_inetaddr_notifier();
    wal_hipriv_register_inet6addr_notifier();
#endif

    /* ������ɺ������ӡ */
    OAL_IO_PRINT("hi1102_host_main_init:: Hi1102_host_main_init finish!\n");

    return OAL_SUCC;
}

oal_void  hi1102_host_main_exit(oal_void)
{
    oal_uint16 us_bitmap;

    OAL_IO_PRINT("hi1102_host_main_exit:: Hi1102_host_main_exit enter!\n");

#ifdef _PRE_WLAN_FEATURE_ARP_OFFLOAD
    wal_hipriv_unregister_inetaddr_notifier();
    wal_hipriv_unregister_inet6addr_notifier();
#endif
    OAL_IO_PRINT("hi1102_host_main_exit:: begin remove wifi module!\n");
    us_bitmap =  BIT6 | BIT7 | BIT8;
    builder_module_exit(us_bitmap);
    OAL_IO_PRINT("hi1102_host_main_exit:: wifi module removed!\n");

    /* ���غ���ȥ��ʼ�� */
#ifdef _PRE_WLAN_FEATURE_OFFLOAD_FLOWCTL
    hcc_flowctl_get_device_mode_register(OAL_PTR_NULL);
    hcc_flowctl_operate_subq_register(OAL_PTR_NULL, OAL_PTR_NULL);
#else
    hcc_tx_flow_ctrl_cb_register(OAL_PTR_NULL, OAL_PTR_NULL);
#endif
    OAL_IO_PRINT("hi1102_host_main_exit:: Hi1102_host_main_exit finish!\n");
    return ;
}
#elif (_PRE_PRODUCT_ID_HI1151==_PRE_PRODUCT_ID)


oal_int32 hi1151_main_init(oal_void)
{
    oal_int32  l_return;
    oal_uint16  us_bitmap  = 0;

    l_return = platform_module_init();
    if (l_return != OAL_SUCC) {
        OAL_IO_PRINT("Hi1151_main_init: platform_module_init return error code: %d/r/n", l_return);
        return l_return;
    }
    l_return = device_module_init();
    if (l_return != OAL_SUCC) {
        OAL_IO_PRINT("Hi1151_main_init: device_module_init return error code: %d/r/n", l_return);
        us_bitmap = BIT0 | BIT1 | BIT2 | BIT3;
        builder_module_exit(us_bitmap);
        return l_return;
    }
    l_return = host_module_init();
    if (l_return != OAL_SUCC) {
        OAL_IO_PRINT("Hi1151_main_init: host_module_init return error code: %d/r/n", l_return);
        us_bitmap = BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6;
        builder_module_exit(us_bitmap);
        return l_return;
    }
    /* ������ɺ������ӡ */
    OAL_IO_PRINT("Hi1151_main_init:: Hi1151_main_init finish!/r/n");
    return OAL_SUCC;
}

oal_void  hi1151_main_exit(oal_void)
{
    oal_uint16 us_bitmap;

    us_bitmap = BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7 | BIT8;
    builder_module_exit(us_bitmap);
    return ;
}
#endif

/*lint -e578*//*lint -e19*/
#if (defined(_PRE_PRODUCT_ID_HI110X_HOST))
#ifndef CONFIG_HI110X_KERNEL_MODULES_BUILD_SUPPORT
#ifdef _PRE_CONFIG_CONN_HISI_SYSFS_SUPPORT

#include "board.h"
#include "oneimage.h"

oal_int32 g_wifi_init_flag = 0;
oal_int32 g_wifi_init_ret;
/* built-in */
OAL_STATIC ssize_t  wifi_sysfs_set_init(struct kobject *dev, struct kobj_attribute *attr, const char *buf, size_t count)
{
    char mode[128] = {0};
    if (buf == NULL) {
        OAL_IO_PRINT("buf is null r failed!%s\n", __FUNCTION__);
        return 0;
    }

    if (attr == NULL) {
        OAL_IO_PRINT("attr is null r failed!%s\n", __FUNCTION__);
        return 0;
    }

    if (dev == NULL) {
        OAL_IO_PRINT("dev is null r failed!%s\n", __FUNCTION__);
        return 0;
    }
    if ((sscanf_s(buf, "%20s", mode, sizeof(mode)) != 1)) {
        OAL_IO_PRINT("set value one param!\n");
        return -OAL_EINVAL;
    }
    if (sysfs_streq("init", mode)) {
        /* init */
        if (g_wifi_init_flag == 0) {
            g_wifi_init_ret = hi1102_host_main_init();
            g_wifi_init_flag = 1;
        } else {
            OAL_IO_PRINT("double init!\n");
        }
    } else {
        OAL_IO_PRINT("invalid input:%s\n", mode);
    }

    return count;
}

OAL_STATIC ssize_t  wifi_sysfs_get_init(struct kobject *dev, struct kobj_attribute *attr, char *buf)
{
    int ret = 0;
    if (buf == NULL) {
        OAL_IO_PRINT("buf is null r failed!%s\n", __FUNCTION__);
        return 0;
    }

    if (attr == NULL) {
        OAL_IO_PRINT("attr is null r failed!%s\n", __FUNCTION__);
        return 0;
    }

    if (dev == NULL) {
        OAL_IO_PRINT("dev is null r failed!%s\n", __FUNCTION__);
        return 0;
    }
    if (g_wifi_init_flag == 1) {
        if (g_wifi_init_ret == OAL_SUCC) {
            ret += snprintf_s(buf + ret, PAGE_SIZE - ret, PAGE_SIZE - ret - 1, "running\n");
        } else {
            ret += snprintf_s(buf + ret, PAGE_SIZE - ret, PAGE_SIZE - ret - 1, "boot failed ret=%d", g_wifi_init_ret);
        }
    } else {
        ret += snprintf_s(buf + ret, PAGE_SIZE - ret, PAGE_SIZE - ret - 1, "uninit\n");
    }

    return ret;
}
STATIC struct kobj_attribute dev_attr_wifi =
    __ATTR(wifi, S_IRUGO | S_IWUSR, wifi_sysfs_get_init, wifi_sysfs_set_init);
OAL_STATIC struct attribute *wifi_init_sysfs_entries[] = {
    &dev_attr_wifi.attr,
    NULL
};

OAL_STATIC struct attribute_group wifi_init_attribute_group = {
    .attrs = wifi_init_sysfs_entries,
};

oal_int32  wifi_sysfs_init(oal_void)
{
    oal_int32 ret;
    oal_kobject*     pst_root_boot_object = NULL;
    if (!is_hisi_chiptype(BOARD_VERSION_HI1102A)) {
        return OAL_SUCC;
    }

    pst_root_boot_object = oal_get_sysfs_root_boot_object();
    if (pst_root_boot_object == NULL) {
        OAL_IO_PRINT("[E]get root boot sysfs object failed!\n");
        return -OAL_EBUSY;
    }

    ret = sysfs_create_group(pst_root_boot_object, &wifi_init_attribute_group);
    if (ret) {
        OAL_IO_PRINT("sysfs create plat boot group fail.ret=%d\n", ret);
        ret = -OAL_ENOMEM;
        return ret;
    }

    return ret;
}

oal_void  wifi_sysfs_exit(oal_void) 
{
    /* need't exit,built-in */
    return;
}
oal_module_init(wifi_sysfs_init);
oal_module_exit(wifi_sysfs_exit);
#endif
#else
oal_module_init(hi1102_host_main_init);
oal_module_exit(hi1102_host_main_exit);
#endif
#elif  (_PRE_PRODUCT_ID_HI1151==_PRE_PRODUCT_ID)
oal_module_init(hi1151_main_init);
oal_module_exit(hi1151_main_exit);
#endif
oal_module_license("GPL");
/*lint +e578*/ /*lint +e19*/


