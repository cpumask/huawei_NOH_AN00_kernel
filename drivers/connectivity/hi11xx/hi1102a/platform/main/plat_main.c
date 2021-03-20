

#define HISI_LOG_TAG "[plat_init]"

/* ͷ�ļ����� */
#include "plat_main.h"
#include "securec.h"
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include "board.h"
#include "oneimage.h"
#include "plat_pm.h"
#include "hw_bfg_ps.h"
#include "oal_kernel_file.h"
#include "hisi_ini.h"
#endif

#ifdef _PRE_PLAT_FEATURE_HI110X_PCIE
#include "oal_pcie_linux.h"
#endif
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include "bfgx_exception_rst.h"
#include "chr_devs.h"
#endif
#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_PLAT_MAIN_C

/* ʵ��plat_init�м�����gnss��صĺ�����ʼ������ */
OAL_STATIC oal_int32 plat_gnss_init(oal_void)
{
    oal_int32 l_return = OAL_FAIL;

    if (!get_i3c_switch_mode()) {
        l_return = gnss_timesync_event_init();
        if (l_return != OAL_SUCC) {
            OAL_IO_PRINT("plat_init: timesync_2gnss_init return error code: %d\r\n", l_return);
            return l_return;
        }
    }
#ifdef CONFIG_HI110X_GPS_REFCLK
    l_return = hi_gps_plat_init();
    if (l_return != OAL_SUCC) {
        OAL_IO_PRINT("plat_init: hi_gps_plat_init fail\r\n");
        goto gps_plat_init_fail;
    }
#endif

#ifdef CONFIG_HI110X_GPS_SYNC
    l_return = gnss_sync_init();
    if (l_return != OAL_SUCC) {
        OAL_IO_PRINT("plat_init: gnss_sync_init fail\r\n");
        goto gps_sync_init_fail;
    }
#endif

    return OAL_SUCC;

#ifdef CONFIG_HI110X_GPS_SYNC
gps_sync_init_fail:
    hi_gps_plat_exit();
#endif
#ifdef CONFIG_HI110X_GPS_REFCLK
gps_plat_init_fail:
    gnss_timesync_event_exit();
#endif

    return l_return;
}
/* ʵ��plat_exit�м�����gnss��صĺ����˳����� */
OAL_STATIC oal_void plat_gnss_exit(oal_void)
{
#ifdef CONFIG_HI110X_GPS_SYNC
    gnss_sync_exit();
#endif
#ifdef CONFIG_HI110X_GPS_REFCLK
    hi_gps_plat_exit();
#endif
    if (!get_i3c_switch_mode()) {
        gnss_timesync_event_exit();
    }
}

/*
 * �� �� ��  : plat_init
 * ��������  : ƽ̨��ʼ����������ڣ�Ŀǰʵ����wifiҵ��Ŀ¼�£��˴���ʱע�գ�
 *             ����Ų������
 */
oal_int32 plat_init(oal_void)
{
    oal_int32 l_return;
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#ifdef HI110X_DRV_VERSION
    OAL_IO_PRINT("HI110X_DRV_VERSION: %s\r\n", HI110X_DRV_VERSION);
#endif
    if (is_my_chip() == false) {
        return OAL_SUCC;
    }
#endif

#ifdef CONFIG_HUAWEI_DSM
    hw_1102a_register_wifi_dsm_client();
    hw_1102a_register_bt_dsm_client();
#endif

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    l_return = hi110x_board_init();
    if (l_return != OAL_SUCC) {
        OAL_IO_PRINT("plat_init: hi110x_board_init fail\r\n");
        goto board_init_fail;
    }
#endif

#ifdef CONFIG_HI1102_PLAT_HW_CHR
    l_return = chr_miscdevs_init();
    if (l_return != OAL_SUCC) {
        OAL_IO_PRINT("plat_init: chr_miscdev_init return error code: %d\r\n", l_return);

        goto chr_miscdevs_init_fail;
    }
#endif

    l_return = plat_exception_reset_init();
    if (l_return != OAL_SUCC) {
        OAL_IO_PRINT("plat_init: plat_exception_reset_init fail\r\n");
        goto plat_exception_rst_init_fail;
    }

    l_return = oal_wifi_platform_load_dev();
    if (l_return != OAL_SUCC) {
        OAL_IO_PRINT("oal_wifi_platform_load_dev fail:%d\r\n", l_return);
        goto wifi_load_sdio_fail;
    }
#ifdef CONFIG_HWCONNECTIVITY
    l_return = hw_misc_connectivity_init();
    if (l_return != OAL_SUCC) {
        OAL_IO_PRINT("hw_misc_connectivity_init fail:%d\r\n", l_return);
        goto hw_misc_connectivity_init_fail;
    }
#endif
    l_return = oal_main_init();
    if (l_return != OAL_SUCC) {
        OAL_IO_PRINT("plat_init: oal_main_init return error code: %d\r\n", l_return);
        goto oal_main_init_fail;
    }

    l_return = oam_main_init();
    if (l_return != OAL_SUCC) {
        OAL_IO_PRINT("plat_init: oam_main_init return error code: %d\r\n", l_return);
        goto oam_main_init_fail;
    }

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    l_return = sdt_drv_main_init();
    if (l_return != OAL_SUCC) {
        OAL_IO_PRINT("plat_init: sdt_drv_main_init return error code: %d\r\n", l_return);
        goto sdt_drv_main_init_fail;
    }
#endif

    l_return = frw_main_init();
    if (l_return != OAL_SUCC) {
        OAL_IO_PRINT("plat_init: frw_main_init return error code: %d\r\n", l_return);
        goto frw_main_init_fail;
    }

#if defined(_PRE_PLAT_FEATURE_HI110X_PCIE) && defined(CONFIG_ARCH_SD56XX)
    OAL_IO_PRINT("plat_init:: platform_main_init finish!\r\n");
    return OAL_SUCC;
#endif

    l_return = low_power_init();
    if (l_return != OAL_SUCC) {
        OAL_IO_PRINT("plat_init: low_power_init return error code: %d\r\n", l_return);
        goto low_power_init_fail;
    }

    l_return = hw_ps_init();
    if (l_return != OAL_SUCC) {
        OAL_IO_PRINT("plat_init: hw_ps_init return error code: %d\r\n", l_return);
        goto hw_ps_init_fail;
    }

    /* �����淶Ȧ���Ӷ�ԭ�򣬰�gnss��صļ�����ʼ���������ŵ�һ�������� */
    l_return = plat_gnss_init();
    if (l_return != OAL_SUCC) {
        OAL_IO_PRINT("plat_init: plat_gnss_init return error code: %d\r\n", l_return);
        goto plat_gnss_init_fail;
    }

    /* ������ɺ������ӡ */
    OAL_IO_PRINT("plat_init:: platform_main_init finish!\r\n");

    return OAL_SUCC;

plat_gnss_init_fail:
    hw_ps_exit();
hw_ps_init_fail:
    low_power_exit();
low_power_init_fail:
    frw_main_exit();
frw_main_init_fail:
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    sdt_drv_main_exit();
sdt_drv_main_init_fail:
#endif
    oam_main_exit();
oam_main_init_fail:
    oal_main_exit();
oal_main_init_fail:
/* �쳣�رյ�Դ */
#ifdef HAVE_HISI_NFC
    hi_wlan_power_off();
#endif
#ifdef CONFIG_HWCONNECTIVITY
    hw_misc_connectivity_exit();
hw_misc_connectivity_init_fail:
#endif
    oal_wifi_platform_unload_dev();
wifi_load_sdio_fail:
    plat_exception_reset_exit();
plat_exception_rst_init_fail:

#ifdef CONFIG_HI1102_PLAT_HW_CHR
    chr_miscdevs_exit();
chr_miscdevs_init_fail:
#endif

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    hi110x_board_exit();
#endif
board_init_fail:
    CHR_EXCEPTION_REPORT(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_PLAT, CHR_LAYER_DRV,
                         CHR_PLT_DRV_EVENT_INIT, CHR_PLAT_DRV_ERROR_PLAT_INIT);

    return l_return;
}

/*
 * �� �� ��  : plat_exit
 * ��������  : ƽ̨ж�غ�������ڣ�Ŀǰʵ����wifiҵ��Ŀ¼�£��˴���ʱע�գ�
 *             ����Ų������
 */
oal_void plat_exit(oal_void)
{
    /* �����淶Ȧ���Ӷ�ԭ�򣬰�gnss��صļ�����ʼ���������ŵ�һ�������� */
    plat_gnss_exit();

    hw_ps_exit();
    low_power_exit();
    frw_main_exit();

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    sdt_drv_main_exit();
#endif

    oam_main_exit();

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#ifdef CONFIG_HWCONNECTIVITY
    hw_misc_connectivity_exit();
#endif
#endif

    oal_main_exit();

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    hi110x_board_exit();
#endif

#ifdef CONFIG_HI1102_PLAT_HW_CHR
    chr_miscdevs_exit();
#endif

    oal_wifi_platform_unload_dev();

    plat_exception_reset_exit();
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    ini_cfg_exit();
#endif

#ifdef CONFIG_HUAWEI_DSM
    hw_1102a_unregister_wifi_dsm_client();
    hw_1102a_unregister_bt_dsm_client();
#endif

    return;
}

/*lint -e578*/ /*lint -e19*/
#if defined(_PRE_PRODUCT_ID_HI110X_HOST) && !defined(CONFIG_HI110X_KERNEL_MODULES_BUILD_SUPPORT) && \
    defined(_PRE_CONFIG_CONN_HISI_SYSFS_SUPPORT)
OAL_STATIC oal_int32 g_plat_init_flag = 0;
OAL_STATIC oal_int32 g_plat_init_ret;
/* built-in */
OAL_STATIC ssize_t plat_sysfs_set_init(struct kobject *dev, struct kobj_attribute *attr,
                                       const char *buf, size_t count)
{
    const uint32 ul_mode_len = 128;
    char mode[ul_mode_len];

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
        if (g_plat_init_flag == 0) {
            g_plat_init_ret = plat_init();
            g_plat_init_flag = 1;
        } else {
            OAL_IO_PRINT("double init!\n");
        }
    } else {
        OAL_IO_PRINT("invalid input:%s\n", mode);
    }

    return count;
}

OAL_STATIC ssize_t plat_sysfs_get_init(struct kobject *dev, struct kobj_attribute *attr, char *buf)
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

    if (g_plat_init_flag == 1) {
        if (g_plat_init_ret == OAL_SUCC) {
            ret = snprintf_s(buf + ret, PAGE_SIZE - ret, PAGE_SIZE - ret - 1, "running\n");
        } else {
            ret = snprintf_s(buf + ret, PAGE_SIZE - ret, PAGE_SIZE - ret - 1, "boot failed ret=%d\n", g_plat_init_ret);
        }
    } else {
        ret = snprintf_s(buf + ret, PAGE_SIZE - ret, PAGE_SIZE - ret - 1, "uninit\n");
    }

    if (ret < 0) {
        OAL_IO_PRINT("log str format err line[%d]\n", __LINE__);
    }

    return ret;
}
STATIC struct kobj_attribute g_dev_attr_plat =
    __ATTR(plat, S_IRUGO | S_IWUSR, plat_sysfs_get_init, plat_sysfs_set_init);
OAL_STATIC struct attribute *g_plat_init_sysfs_entries[] = {
    &g_dev_attr_plat.attr,
    NULL
};

OAL_STATIC struct attribute_group g_plat_init_attribute_group = {
    .attrs = g_plat_init_sysfs_entries,
};

oal_int32 plat_sysfs_init(oal_void)
{
    oal_int32 ret;
    oal_uint32 ul_rslt;
    oal_kobject *pst_root_boot_object = NULL;

    if (is_hisi_chiptype(BOARD_VERSION_HI1102A) == false) {
        return OAL_SUCC;
    }

    /* 110X ����build in���ڴ�س�ʼ�����Ƶ��ں���ɣ���֤��Ƭ�ڴ�����ɹ� */
    ul_rslt = oal_mem_init_pool();
    if (ul_rslt != OAL_SUCC) {
        OAL_IO_PRINT("oal_main_init: oal_mem_init_pool return error code: %d", ul_rslt);
        return -OAL_EFAIL;
    }

    OAL_IO_PRINT("mem pool init succ\n");

    pst_root_boot_object = oal_get_sysfs_root_boot_object();
    if (pst_root_boot_object == NULL) {
        OAL_IO_PRINT("[E]get root boot sysfs object failed!\n");
        return -OAL_EBUSY;
    }

    ret = sysfs_create_group(pst_root_boot_object, &g_plat_init_attribute_group);
    if (ret) {
        OAL_IO_PRINT("sysfs create plat boot group fail.ret=%d\n", ret);
        ret = -OAL_ENOMEM;
        return ret;
    }

    return ret;
}

oal_void plat_sysfs_exit(oal_void)
{
    /* need't exit,built-in */
    return;
}
oal_module_init(plat_sysfs_init);
oal_module_exit(plat_sysfs_exit);
#else
oal_module_init(plat_init);
oal_module_exit(plat_exit);
#endif
