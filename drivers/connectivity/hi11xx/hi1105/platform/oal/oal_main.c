

/* ͷ�ļ����� */
#include "oal_main.h"
#include "oal_workqueue.h"
#include "oal_schedule.h"
#include "oal_net.h"
#include "oal_hcc_host_if.h"
#include "oal_kernel_file.h"

#ifndef WIN32
#include "plat_firmware.h"
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
uint8_t wlan_service_device_per_chip[WLAN_SERVICE_DEVICE_MAX_NUM_PER_CHIP] = { WLAN_INIT_DEVICE_RADIO_CAP };

/* ��Щ�ں˰汾����û����CONFIG_STACKTRACE, ����CONFIG_STACKTRACE�ĺ� */
#define oal_dump_stack_str_init()
int32_t oal_dump_stack_str(uint8_t *puc_str, uint32_t ul_max_size)
{
    return 0;
}

/*
 * �� �� ��  : oal_main_init
 * ��������  : OALģ���ʼ������ڣ�����OALģ���ڲ��������Եĳ�ʼ����
 * �� �� ֵ  : ��ʼ������ֵ���ɹ���ʧ��ԭ��
 */
int32_t ATTR_OAL_NO_FUNC_TRACE oal_main_init(void)
{
    uint32_t ul_rslt;

#ifdef _PRE_MEM_TRACE
    mem_trace_init();
#endif

#ifdef _PRE_CONFIG_CONN_HISI_SYSFS_SUPPORT
    if (oal_conn_sysfs_root_obj_init() == NULL) {
        oal_io_print("hisi root sysfs init failed\n");
    }
#endif

/* Ϊ�˽��ģ�������ʱ�䣬����ʱ�����ӡ */
#if !defined(CONFIG_HI110X_KERNEL_MODULES_BUILD_SUPPORT) && \
    defined(_PRE_CONFIG_CONN_HISI_SYSFS_SUPPORT)
    /* 110X ����build in���ڴ�س�ʼ�����Ƶ��ں���ɣ���֤��Ƭ�ڴ�����ɹ� */
#else
    /* �ڴ�س�ʼ�� */
    ul_rslt = oal_mem_init_pool();
    if (ul_rslt != OAL_SUCC) {
        oal_io_print("oal_main_init: oal_mem_init_pool return error code: %d", ul_rslt);
        return -OAL_EFAIL;  //lint !e527
    }
#endif

    /* ��ʼ��: �����ϵ�chip��������1 */
    oal_bus_init_chip_num();
    ul_rslt = oal_bus_inc_chip_num();
    if (ul_rslt != OAL_SUCC) {
        oal_io_print("oal_pci_probe: oal_bus_inc_chip_num failed!\n");
        return -OAL_EIO;
    }

    /* �����ɹ� */
#if (_PRE_OS_VERSION_WIN32 == _PRE_OS_VERSION)
    memset_s(g_past_net_device, WLAN_VAP_SUPPORT_MAX_NUM_LIMIT * OAL_SIZEOF(oal_net_device_stru *),
             0, WLAN_VAP_SUPPORT_MAX_NUM_LIMIT * OAL_SIZEOF(oal_net_device_stru *));
#endif

#ifndef BFGX_UART_DOWNLOAD_SUPPORT
    /* HCC��ʼ�� */
    if (oal_unlikely(hcc_dev_init() != OAL_SUCC)) {
        oal_io_print("[ERROR]hcc_module_init return err null\n");
        return -OAL_EFAIL;
    }

#ifdef _PRE_CONFIG_HISI_CONN_SOFTWDFT
    if (oal_unlikely(oal_softwdt_init() != OAL_SUCC)) {
        oal_io_print("oal_softwdt_init init failed!\n");
        return -OAL_EFAIL;
    }
#endif

#ifdef _PRE_OAL_FEATURE_KEY_PROCESS_TRACE
    if (oal_unlikely(oal_dft_init() != OAL_SUCC)) {
        oal_io_print("oal_dft_init init failed!\n");
        return -OAL_EFAIL;
    }
#endif
#else
    oal_io_print("gnss only version not support wifi hcc\\n");
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
void ATTR_OAL_NO_FUNC_TRACE oal_main_exit(void)
{
#ifdef _PRE_OAL_FEATURE_KEY_PROCESS_TRACE
    oal_dft_exit();
#endif

#ifdef _PRE_CONFIG_HISI_CONN_SOFTWDFT
    oal_softwdt_exit();
#endif

    /* chip num��ʼ��:0 */
    oal_bus_init_chip_num();

#if !defined(CONFIG_HI110X_KERNEL_MODULES_BUILD_SUPPORT) && \
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
OAL_STATIC uint8_t oal_device_check_enable_num(void)
{
    uint8_t uc_device_num = 0;
    uint8_t uc_device_id;

    for (uc_device_id = 0; uc_device_id < WLAN_SERVICE_DEVICE_MAX_NUM_PER_CHIP; uc_device_id++) {
        if (wlan_service_device_per_chip[uc_device_id]) {
            uc_device_num++;
        }
    }
    return uc_device_num;
}

/*
 * �� �� ��  : oal_chip_get_device_num
 * ��������  : ����chip version��ȡdevice num
 * �������  : chip version
 */
uint8_t oal_chip_get_device_num(uint32_t ul_chip_ver)
{
    return oal_device_check_enable_num();
}
/*
 * �� �� ��  : oal_board_get_service_vap_start_id
 * ��������  : ��ȡboard�ϵ�ҵ��vap��ʵidx
 */
uint8_t oal_board_get_service_vap_start_id(void)
{
    uint8_t uc_device_num_per_chip = oal_device_check_enable_num();

    /* ����vap���� = mac device����,vap idx����������vap,��ҵ��vap */
    return (uint8_t)(WLAN_CHIP_MAX_NUM_PER_BOARD * uc_device_num_per_chip);
}

/*lint -e578*/ /*lint -e19*/
oal_module_symbol(oal_dump_stack_str);
oal_module_symbol(oal_chip_get_device_num);
oal_module_symbol(oal_board_get_service_vap_start_id);
oal_module_symbol(oal_main_init);
oal_module_symbol(oal_main_exit);
oal_module_symbol(l2cache_base);
oal_module_symbol(wlan_service_device_per_chip);
oal_module_license("GPL");
