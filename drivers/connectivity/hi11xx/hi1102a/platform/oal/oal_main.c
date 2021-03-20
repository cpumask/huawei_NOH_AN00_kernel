

/* ͷ�ļ����� */
#include "oal_main.h"
#include "oal_workqueue.h"
#include "oal_mem.h"
#include "oal_schedule.h"
#include "oal_net.h"
#if defined(_PRE_PRODUCT_ID_HI110X_HOST)
#include "oal_hcc_host_if.h"
#endif
#include "oal_kernel_file.h"

#if defined(_PRE_PRODUCT_ID_HI110X_HOST)
#ifndef WIN32
#include "plat_firmware.h"
#endif
#endif

#ifdef _PRE_MEM_TRACE
#include "mem_trace.h"
#endif
#include "securec.h"

#define OAL_MAX_TRACE_ENTRY 64

typedef struct {
    oal_spin_lock_stru st_lock;
    unsigned long aul_trace_entries[OAL_MAX_TRACE_ENTRY];
} oal_stacktrace_stru;

/* ȫ�ֱ������� */
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
void __iomem *l2cache_base;
#endif

/* ��̬/��̬DBDC������һ��Ĭ��ʹ�� */
oal_uint8 wlan_service_device_per_chip[WLAN_SERVICE_DEVICE_MAX_NUM_PER_CHIP] = { WLAN_INIT_DEVICE_RADIO_CAP };

oal_uint8 hitalk_status = 0; /* ��¼��ǰ�̼����� */

/* ����ʵ�� */
/* ��Щ�ں˰汾����û����CONFIG_STACKTRACE, ����CONFIG_STACKTRACE�ĺ� */
#define oal_dump_stack_str_init()
oal_int32 oal_dump_stack_str(oal_uint8 *puc_str, oal_uint32 ul_max_size)
{
    return 0;
}

/*
 * �� �� ��  : oal_main_init
 * ��������  : OALģ���ʼ������ڣ�����OALģ���ڲ��������Եĳ�ʼ����
 * �� �� ֵ  : ��ʼ������ֵ���ɹ���ʧ��ԭ��
 */
oal_int32 ATTR_OAL_NO_FUNC_TRACE oal_main_init(oal_void)
{
    oal_uint32 ul_rslt;
#ifdef _PRE_MEM_TRACE
    mem_trace_init();
#endif

#ifdef _PRE_CONFIG_CONN_HISI_SYSFS_SUPPORT
    if (oal_conn_sysfs_root_obj_init() == NULL) {
        OAL_IO_PRINT("hisi root sysfs init failed\n");
    }
#endif

#if defined(_PRE_PRODUCT_ID_HI110X_HOST) && !defined(CONFIG_HI110X_KERNEL_MODULES_BUILD_SUPPORT) && \
    defined(_PRE_CONFIG_CONN_HISI_SYSFS_SUPPORT)
    /* 110X ����build in���ڴ�س�ʼ�����Ƶ��ں���ɣ���֤��Ƭ�ڴ�����ɹ� */
#else
    /* �ڴ�س�ʼ�� */
    ul_rslt = oal_mem_init_pool();
    if (ul_rslt != OAL_SUCC) {
        OAL_IO_PRINT("oal_main_init: oal_mem_init_pool return error code: %d", ul_rslt);
        return -OAL_EFAIL;  //lint !e527
    }
#endif

#if ((defined(_PRE_PRODUCT_ID_HI110X_HOST)) || (defined(_PRE_PRODUCT_ID_HI110X_DEV)))

    /* ��ʼ��: �����ϵ�chip��������1 */
    oal_bus_init_chip_num();
    ul_rslt = oal_bus_inc_chip_num();
    if (ul_rslt != OAL_SUCC) {
        OAL_IO_PRINT("oal_pci_probe: oal_bus_inc_chip_num failed!\n");
        return -OAL_EIO;
    }
#endif

    /* �����ɹ� */
#if (_PRE_OS_VERSION_WIN32 == _PRE_OS_VERSION)
    memset_s(past_net_device, WLAN_VAP_SUPPORT_MAX_NUM_LIMIT * OAL_SIZEOF(oal_net_device_stru *),
             0, WLAN_VAP_SUPPORT_MAX_NUM_LIMIT * OAL_SIZEOF(oal_net_device_stru *));
#endif

#ifndef BFGX_UART_DOWNLOAD_SUPPORT
#if defined(_PRE_PRODUCT_ID_HI110X_HOST)
    /* HCC��ʼ�� */
    if (OAL_UNLIKELY(hcc_dev_init() != OAL_SUCC)) {
        OAL_IO_PRINT("[ERROR]hcc_module_init return err null\n");
        return -OAL_EFAIL;
    }
#if defined(_PRE_PLAT_FEATURE_HI110X_PCIE) && defined(CONFIG_ARCH_SD56XX)
    /* 5610 udp pcie chip test */
    hcc_enable(hcc_get_110x_handler(), OAL_FALSE);
#endif
#endif

#ifdef _PRE_CONFIG_HISI_CONN_SOFTWDFT
    if (OAL_UNLIKELY(oal_softwdt_init() != OAL_SUCC)) {
        OAL_IO_PRINT("oal_softwdt_init init failed!\n");
        return -OAL_EFAIL;
    }
#endif

#ifdef _PRE_OAL_FEATURE_KEY_PROCESS_TRACE
    if (OAL_UNLIKELY(oal_dft_init() != OAL_SUCC)) {
        OAL_IO_PRINT("oal_dft_init init failed!\n");
        return -OAL_EFAIL;
    }
#endif
#else
    OAL_IO_PRINT("gnss only version not support wifi hcc\\n");
#endif

    oal_dump_stack_str_init();

    oal_workqueue_init();

#if ((_PRE_CONFIG_TARGET_PRODUCT == _PRE_TARGET_PRODUCT_TYPE_E5) && (_PRE_TEST_MODE != _PRE_TEST_MODE_UT))
    oal_register_syscore_ops();
#endif
    return OAL_SUCC;
}

/*
 * �� �� ��  : oal_main_exit
 * ��������  : OALģ��ж��
 * �� �� ֵ  : ģ��ж�ط���ֵ���ɹ���ʧ��ԭ��
 */
oal_void ATTR_OAL_NO_FUNC_TRACE oal_main_exit(oal_void)
{
#ifdef _PRE_OAL_FEATURE_KEY_PROCESS_TRACE
    oal_dft_exit();
#endif

#ifdef _PRE_CONFIG_HISI_CONN_SOFTWDFT
    oal_softwdt_exit();
#endif

#if ((defined(_PRE_PRODUCT_ID_HI110X_HOST)) || (defined(_PRE_PRODUCT_ID_HI110X_DEV)))

    /* chip num��ʼ��:0 */
    oal_bus_init_chip_num();
#endif

#if defined(_PRE_PRODUCT_ID_HI110X_HOST) && !defined(CONFIG_HI110X_KERNEL_MODULES_BUILD_SUPPORT) && \
    defined(_PRE_CONFIG_CONN_HISI_SYSFS_SUPPORT)
    /* 110X ����build in���ڴ�س�ʼ�����Ƶ��ں���ɣ���֤��Ƭ�ڴ�����ɹ� */
#else
    /* �ڴ��ж�� */
    oal_mem_exit();
#endif

#ifdef _PRE_CONFIG_CONN_HISI_SYSFS_SUPPORT
    oal_conn_sysfs_root_boot_obj_exit();
    oal_conn_sysfs_root_obj_exit();
#endif
    oal_workqueue_exit();

#if ((_PRE_CONFIG_TARGET_PRODUCT == _PRE_TARGET_PRODUCT_TYPE_E5) && (_PRE_TEST_MODE != _PRE_TEST_MODE_UT))
    oal_unregister_syscore_ops();
#endif

#ifdef _PRE_MEM_TRACE
    mem_trace_exit();
#endif

    return;
}

/*
 * �� �� ��  : oal_device_check_enable_num
 * ��������  : ��鵥chip��mac deviceʹ�ܸ���
 * �� �� ֵ  : ʹ��device����
 */
OAL_STATIC oal_uint8 oal_device_check_enable_num(oal_void)
{
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
    oal_uint8 uc_device_num = 0;
    oal_uint8 uc_device_id;

    for (uc_device_id = 0; uc_device_id < WLAN_SERVICE_DEVICE_MAX_NUM_PER_CHIP; uc_device_id++) {
        if (wlan_service_device_per_chip[uc_device_id]) {
            uc_device_num++;
        }
    }
    return uc_device_num;
#else
    return WLAN_SERVICE_DEVICE_MAX_NUM_PER_CHIP;
#endif  // #ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
}

/*
 * �� �� ��  : oal_chip_get_version
 * ��������  : ����chip version��ȡdevice num
 */
oal_uint8 oal_chip_get_device_num(oal_uint32 ul_chip_ver)
{
    return oal_device_check_enable_num();
}
/*
 * �� �� ��  : oal_board_get_service_vap_start_id
 * ��������  : ��ȡboard�ϵ�ҵ��vap��ʵidx
 */
oal_uint8 oal_board_get_service_vap_start_id(oal_void)
{
    oal_uint8 uc_device_num_per_chip = oal_device_check_enable_num();

    /* ����vap���� = mac device����,vap idx����������vap,��ҵ��vap */
    return (oal_uint8)(WLAN_CHIP_MAX_NUM_PER_BOARD * uc_device_num_per_chip);
}

/*lint -e578*/ /*lint -e19*/
oal_module_symbol(oal_dump_stack_str);
oal_module_symbol(oal_chip_get_device_num);
oal_module_symbol(oal_board_get_service_vap_start_id);
oal_module_symbol(oal_main_init);
oal_module_symbol(oal_main_exit);
oal_module_symbol(l2cache_base);
oal_module_symbol(wlan_service_device_per_chip);
oal_module_symbol(hitalk_status);
oal_module_license("GPL");
