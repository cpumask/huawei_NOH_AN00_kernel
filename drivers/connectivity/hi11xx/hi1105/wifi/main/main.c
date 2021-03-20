

/* 1 头文件包含 */
#define HISI_LOG_TAG "[WIFI_MAIN]"
#include "main.h"
#include "oal_kernel_file.h"
#include "oal_hcc_host_if.h"

#include "oneimage.h"
#include "wlan_chip_i.h"

#include "hmac_vap.h"
#include "wal_ext_if.h"
#include "hal_host_ext_if.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_MAIN_C

void platform_module_exit(uint16_t us_bitmap);
OAL_STATIC void builder_module_exit(uint16_t us_bitmap);


/* 3 函数实现 */

#if ((!defined(_PRE_PRODUCT_ID_HI110X_DEV)) || (_PRE_OS_VERSION_WIN32_RAW == _PRE_OS_VERSION))
OAL_STATIC void builder_module_exit(uint16_t us_bitmap)
{
#if (!defined(_PRE_PRODUCT_ID_HI110X_DEV))
    if (us_bitmap & BIT8) {
        wal_main_exit();
    }
    if (us_bitmap & BIT7) {
        hmac_main_exit();
    }

#endif

    return;
}
#endif

#if (defined(HI1102_EDA))

OAL_STATIC uint32_t device_test_create_cfg_vap(void)
{
    uint32_t ul_return;
    frw_event_mem_stru *pst_event_mem;
    frw_event_stru *pst_event;

    pst_event_mem = frw_event_alloc_m(0);
    if (oal_unlikely(pst_event_mem == OAL_PTR_NULL)) {
        oal_io_print("device_test_create_cfg_vap: hmac_init_event_process frw_event_alloc_m result=OAL_PTR_NULL.\n");
        return OAL_FAIL;
    }

    ul_return = dmac_init_event_process(pst_event_mem);
    if (ul_return != OAL_SUCC) {
        oal_io_print("device_test_create_cfg_vap: dmac_init_event_process result = fale.\n");
        frw_event_free_m(pst_event_mem);
        return OAL_FAIL;
    }

    pst_event = frw_get_event_stru(pst_event_mem);
    pst_event->st_event_hdr.uc_device_id = 0;

    ul_return = dmac_cfg_vap_init_event(pst_event_mem);
    if (ul_return != OAL_SUCC) {
        frw_event_free_m(pst_event_mem);
        return ul_return;
    }

    frw_event_free_m(pst_event_mem);

    return OAL_SUCC;
}
#endif

extern int32_t wlan_pm_open(void);
extern uint32_t wlan_pm_close(void);


int32_t host_module_init(void)
{
    int32_t l_return;
    uint16_t us_bitmap = 0;

#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
    oal_bool_enum_uint8 cali_fst_pwr_on = OAL_TRUE;

    if (wlan_chip_ops_init() != OAL_SUCC) {
        oal_io_print("host_module_init: wlan_chip_ops_init return fail\r\n");
        return OAL_FAIL;
    }

    /* 读定制化配置文件&NVRAM */
    hwifi_custom_host_read_cfg_init();
    /* 配置host全局变量值 */
    wlan_chip_host_global_init_param();
#endif  // #ifdef _PRE_PLAT_FEATURE_CUSTOMIZE

    l_return = hal_main_init();
    if (l_return != OAL_SUCC) {
        oal_io_print("host_module_init: hal_main_init return error code:%d\r\n", l_return);
        return l_return;
    }

    l_return = hmac_main_init();
    if (l_return != OAL_SUCC) {
        oal_io_print("host_module_init: hmac_main_init return error code:%d\r\n", l_return);
        return l_return;
    }

    l_return = wal_main_init();
    if (l_return != OAL_SUCC) {
        oal_io_print("host_module_init:wal_main_init return error code:%d\r\n", l_return);
        us_bitmap = BIT7;
        builder_module_exit(us_bitmap);
        return l_return;
    }

/*
 * if not TV, init g_wlan_cali_completed here.
 * if TV && kernel module, init g_wlan_cali_completed here.
 * if TV && built-in, init g_wlan_cali_completed in wifi_sysfs_init, which is an early stage,
 * to avoid using g_wlan_cali_completed before it is inited.
 */
#if ((!defined _PRE_PRODUCT_HI3751V811) || (defined CONFIG_HI110X_KERNEL_MODULES_BUILD_SUPPORT))
    oal_init_completion(&g_wlan_cali_completed);
#endif
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    g_custom_cali_done = OAL_FALSE;

#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
    wlan_chip_first_power_on_mark();
#endif

    /* 开机校准和定制化参数下发 */
    if (cali_fst_pwr_on == OAL_TRUE) {
        wlan_pm_open();
    }
#endif

    /* 启动完成后，输出打印 */
    oal_io_print("host_module_init:: host_main_init finish!\r\n");

    return OAL_SUCC;
}

#ifndef _PRE_LINUX_TEST
static const oal_wlan_cfg_stru g_wlan_spec_cfg_hi1103 = {
#else
static oal_wlan_cfg_stru g_wlan_spec_cfg_hi1103 = {
#endif
    .feature_m2s_modem_is_open = OAL_FALSE,
    .feature_priv_closed_is_open = OAL_TRUE,
    .feature_hiex_is_open = OAL_FALSE,
    .rx_listen_ps_is_open = OAL_FALSE,
    .feature_11ax_is_open = OAL_FALSE,
    .feature_twt_is_open = OAL_FALSE,
    .feature_11ax_er_su_dcm_is_open = OAL_FALSE,
    .feature_11ax_uora_is_supported = OAL_FALSE,
    .feature_ftm_is_open = OAL_FALSE,
    .feature_psm_dfx_ext_is_open = OAL_TRUE,
    .feature_wow_opt_is_open  = OAL_FALSE,
    .feature_dual_wlan_is_supported = OAL_FALSE,
    .feature_ht_self_cure_is_open = OAL_FALSE,
    .feature_slave_ax_is_support = OAL_FALSE,
    .longer_than_16_he_sigb_support = OAL_FALSE,
    .p2p_go_max_user_num = WLAN_P2P_GO_ASSOC_USER_MAX_NUM_1103,
    .max_asoc_user = WLAN_ASSOC_USER_MAX_NUM_HI1103,
    .max_active_user = WLAN_ASSOC_USER_MAX_NUM_HI1103,
    .max_user_limit = WLAN_ASSOC_USER_MAX_NUM_HI1103 + MAC_RES_MAX_BCAST_USER_NUM,
    .invalid_user_id = WLAN_ASSOC_USER_MAX_NUM_HI1103 + MAC_RES_MAX_BCAST_USER_NUM,
};

#ifndef _PRE_LINUX_TEST
static const oal_wlan_cfg_stru g_wlan_spec_cfg_hi1105 = {
#else
static oal_wlan_cfg_stru g_wlan_spec_cfg_hi1105 = {
#endif
    .feature_m2s_modem_is_open = OAL_TRUE,
    .feature_priv_closed_is_open = OAL_FALSE,
    .feature_hiex_is_open = OAL_TRUE,
    .rx_listen_ps_is_open = OAL_TRUE,
    .feature_11ax_is_open = OAL_TRUE,
    .feature_twt_is_open = OAL_TRUE,
    .feature_11ax_er_su_dcm_is_open = OAL_TRUE,
    .feature_11ax_uora_is_supported = OAL_FALSE,
    .feature_ftm_is_open = OAL_TRUE,
    .feature_psm_dfx_ext_is_open = OAL_TRUE,
    .feature_wow_opt_is_open  = OAL_FALSE,
    .feature_dual_wlan_is_supported = OAL_TRUE,
    .feature_ht_self_cure_is_open = OAL_TRUE,
    .feature_slave_ax_is_support = OAL_FALSE,
    .longer_than_16_he_sigb_support = OAL_FALSE,
    .p2p_go_max_user_num = WLAN_P2P_GO_ASSOC_USER_MAX_NUM_1105,
    .max_asoc_user = WLAN_ASSOC_USER_MAX_NUM_HI1105,
    .max_active_user = WLAN_ASSOC_USER_MAX_NUM_HI1105,
    .max_user_limit = WLAN_ASSOC_USER_MAX_NUM_HI1105 + MAC_RES_MAX_BCAST_USER_NUM,
    .invalid_user_id = WLAN_ASSOC_USER_MAX_NUM_HI1105 + MAC_RES_MAX_BCAST_USER_NUM,
};

#ifndef _PRE_LINUX_TEST
static const oal_wlan_cfg_stru g_wlan_spec_cfg_hi1106 = {
#else
static oal_wlan_cfg_stru g_wlan_spec_cfg_hi1106 = {
#endif
    .feature_m2s_modem_is_open = OAL_TRUE,
    .feature_priv_closed_is_open = OAL_FALSE,
    .feature_hiex_is_open = OAL_TRUE,
    .rx_listen_ps_is_open = OAL_TRUE,
    .feature_11ax_is_open = OAL_TRUE,
    .feature_twt_is_open = OAL_TRUE,
    .feature_11ax_er_su_dcm_is_open = OAL_TRUE,
    .feature_11ax_uora_is_supported = OAL_TRUE,
    .feature_ftm_is_open = OAL_TRUE,
    .feature_psm_dfx_ext_is_open = OAL_TRUE,
    .feature_wow_opt_is_open  = OAL_FALSE,
    .feature_dual_wlan_is_supported = OAL_FALSE,
    .feature_ht_self_cure_is_open = OAL_FALSE,
    .feature_slave_ax_is_support = OAL_TRUE,
    .longer_than_16_he_sigb_support = OAL_TRUE,
    .p2p_go_max_user_num = WLAN_P2P_GO_ASSOC_USER_MAX_NUM_1106,  /* 1106 P2P最大接入用户数为8 */
    .max_asoc_user = WLAN_ASSOC_USER_MAX_NUM,
    .max_active_user = WLAN_ASSOC_USER_MAX_NUM,
    .max_user_limit = MAC_RES_MAX_USER_LIMIT,
    .invalid_user_id = MAC_RES_MAX_USER_LIMIT,
};

#ifndef _PRE_LINUX_TEST
// 区分不同芯片host侧规格全局变量控制开关
const oal_wlan_cfg_stru *g_wlan_spec_cfg;
#else
// UT通过host侧产品宏控制指向对应全局变量
#if (_PRE_PRODUCT_VERSION == 1103)
oal_wlan_cfg_stru *g_wlan_spec_cfg = &g_wlan_spec_cfg_hi1103;
#elif (_PRE_PRODUCT_VERSION == 1105)
oal_wlan_cfg_stru *g_wlan_spec_cfg = &g_wlan_spec_cfg_hi1105;
#elif (_PRE_PRODUCT_VERSION == 1106)
oal_wlan_cfg_stru *g_wlan_spec_cfg = &g_wlan_spec_cfg_hi1106;
#else
// 其他芯片型号暂不处理，待适配
#endif
#endif

static int32_t wifi_feature_switch_cfg_init(void)
{
#ifdef _PRE_CONFIG_CONN_HISI_SYSFS_SUPPORT
    if (is_hisi_chiptype(BOARD_VERSION_HI1103)) {
        g_wlan_spec_cfg = &g_wlan_spec_cfg_hi1103;
    } else if (is_hisi_chiptype(BOARD_VERSION_HI1105)) {
        g_wlan_spec_cfg = &g_wlan_spec_cfg_hi1105;
    } else if (is_hisi_chiptype(BOARD_VERSION_HI1106)) {
        g_wlan_spec_cfg = &g_wlan_spec_cfg_hi1106;
    } else {
        oal_io_print("hi110x wifi unsupport chitype!\n");
        return -OAL_EFAIL;
    }
#endif
    return OAL_SUCC;
}


int32_t hi110x_host_main_init(void)
{
    int32_t l_return;

    hcc_flowctl_get_device_mode_register(hmac_flowctl_check_device_is_sta_mode);
    hcc_flowctl_operate_subq_register(hmac_vap_net_start_subqueue, hmac_vap_net_stop_subqueue);
    /* wifi相关编译选项初始化设置 */
    l_return = wifi_feature_switch_cfg_init();
    if (l_return != OAL_SUCC) {
        oal_io_print("hi110x_host_main_init: host_module_init wifi_feature_switch_cfg_init fail\n");
        return l_return;
    }

    l_return = host_module_init();
    if (l_return != OAL_SUCC) {
        oal_io_print("hi110x_host_main_init: host_module_init return error code: %d\n", l_return);
        return l_return;
    }

    wal_hipriv_register_inetaddr_notifier();
    wal_hipriv_register_inet6addr_notifier();

    /* 启动完成后，输出打印 */
    oal_io_print("hi110x_host_main_init:: hi110x_host_main_init finish!\n");

    return OAL_SUCC;
}


void hi110x_host_main_exit(void)
{
    uint16_t us_bitmap;

    wal_hipriv_unregister_inetaddr_notifier();
    wal_hipriv_unregister_inet6addr_notifier();

    us_bitmap = BIT6 | BIT7 | BIT8;
    builder_module_exit(us_bitmap);

    return;
}

/*lint -e578*/ /*lint -e19*/
#ifndef CONFIG_HI110X_KERNEL_MODULES_BUILD_SUPPORT
#ifdef _PRE_CONFIG_CONN_HISI_SYSFS_SUPPORT

#include "board.h"

int32_t g_wifi_init_flag = 0;
int32_t g_wifi_init_ret;
/* built-in */
OAL_STATIC ssize_t wifi_sysfs_set_init(struct kobject *dev, struct kobj_attribute *attr, const char *buf, size_t count)
{
    char mode[128] = { 0 };

    if (buf == NULL) {
        oal_io_print("buf is null r failed!%s\n", __FUNCTION__);
        return 0;
    }

    if (attr == NULL) {
        oal_io_print("attr is null r failed!%s\n", __FUNCTION__);
        return 0;
    }

    if (dev == NULL) {
        oal_io_print("dev is null r failed!%s\n", __FUNCTION__);
        return 0;
    }

    if ((sscanf_s(buf, "%20s", mode, sizeof(mode)) != 1)) {
        oal_io_print("set value one param!\n");
        return -OAL_EINVAL;
    }

    if (sysfs_streq("init", mode)) {
        /* init */
        if (g_wifi_init_flag == 0) {
            g_wifi_init_ret = hi110x_host_main_init();
            g_wifi_init_flag = 1;
        } else {
            oal_io_print("double init!\n");
        }
    } else {
        oal_io_print("invalid input:%s\n", mode);
    }

    return count;
}

OAL_STATIC ssize_t wifi_sysfs_get_init(struct kobject *dev, struct kobj_attribute *attr, char *buf)
{
    int ret = 0;

    if (buf == NULL) {
        oal_io_print("buf is null r failed!%s\n", __FUNCTION__);
        return 0;
    }

    if (attr == NULL) {
        oal_io_print("attr is null r failed!%s\n", __FUNCTION__);
        return 0;
    }

    if (dev == NULL) {
        oal_io_print("dev is null r failed!%s\n", __FUNCTION__);
        return 0;
    }

    if (g_wifi_init_flag == 1) {
        if (g_wifi_init_ret == OAL_SUCC) {
            ret += snprintf_s(buf + ret, PAGE_SIZE - ret, (PAGE_SIZE - ret) - 1, "running\n");
        } else {
            ret += snprintf_s(buf + ret, PAGE_SIZE - ret, (PAGE_SIZE - ret) - 1,
                              "boot failed ret=%d\n", g_wifi_init_ret);
        }
    } else {
        ret += snprintf_s(buf + ret, PAGE_SIZE - ret, (PAGE_SIZE - ret) - 1, "uninit\n");
    }

    return ret;
}
STATIC struct kobj_attribute g_dev_attr_wifi =
    __ATTR(wifi, S_IRUGO | S_IWUSR, wifi_sysfs_get_init, wifi_sysfs_set_init);
OAL_STATIC struct attribute *g_wifi_init_sysfs_entries[] = {
    &g_dev_attr_wifi.attr,
    NULL
};

OAL_STATIC struct attribute_group g_wifi_init_attribute_group = {
    .attrs = g_wifi_init_sysfs_entries,
};

int32_t wifi_sysfs_init(void)
{
    int32_t ret;
    oal_kobject *pst_root_boot_object = NULL;

    if ((false == is_hisi_chiptype(BOARD_VERSION_HI1103)) &&
        (false == is_hisi_chiptype(BOARD_VERSION_HI1105)) &&
        (false == is_hisi_chiptype(BOARD_VERSION_HI1106))) {
        return OAL_SUCC;
    }

    pst_root_boot_object = oal_get_sysfs_root_boot_object();
    if (pst_root_boot_object == NULL) {
        oal_io_print("[E]get root boot sysfs object failed!\n");
        return -OAL_EBUSY;
    }

    ret = sysfs_create_group(pst_root_boot_object, &g_wifi_init_attribute_group);
    if (ret) {
        oal_io_print("sysfs create plat boot group fail.ret=%d\n", ret);
        ret = -OAL_ENOMEM;
        return ret;
    }

/* if TV && built-in, init dd here */
#ifdef _PRE_PRODUCT_HI3751V811
    oal_init_completion(&g_wlan_cali_completed);
#endif

    return ret;
}

void wifi_sysfs_exit(void)
{
    /* need't exit,built-in */
    return;
}
oal_module_init(wifi_sysfs_init);
oal_module_exit(wifi_sysfs_exit);
#endif
#else
oal_module_init(hi110x_host_main_init);
oal_module_exit(hi110x_host_main_exit);
#endif

oal_module_license("GPL");
/*lint +e578*/ /*lint +e19*/

