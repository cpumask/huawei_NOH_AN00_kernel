

/* Include Head file */
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include "plat_debug.h"
#include "plat_pm.h"
#include "bfgx_exception_rst.h"
#include "oneimage.h"
#include "oal_kernel_file.h"
#include "oal_sdio_host_if.h"
#include "oal_hcc_host_if.h"
#include "plat_pm_wlan.h"
#include "hisi_ini.h"
#include "plat_uart.h"
#include "board.h"
#include "plat_firmware.h"
#include "oam_rdr.h"
#include "platform_common_clk.h"
#include "securec.h"
/* Define global variable */
struct kobject *sysfs_hi110x_bfgx = NULL;
struct kobject *sysfs_hisi_pmdbg = NULL;

#ifdef PLATFORM_DEBUG_ENABLE
struct kobject *sysfs_hi110x_debug = NULL;
int32 uart_rx_dump = UART_DUMP_CLOSE;
#endif

#ifdef HAVE_HISI_NFC
struct kobject *sysfs_hisi_nfc = NULL;
#endif

int32 plat_loglevel = PLAT_LOG_INFO;
int32 bug_on_enable = BUG_ON_DISABLE;

/* Function implement */
STATIC ssize_t show_wifi_pmdbg(struct kobject *kobj, struct kobj_attribute *attr, int8 *buf)
{
    struct wlan_pm_s *pst_wlan_pm = wlan_pm_get_drv();
    oal_int32 ret = 0;

    if (buf == NULL) {
        ps_print_err("buf is NULL\n");
        return -FAILURE;
    }

#ifdef PLATFORM_DEBUG_ENABLE
    ret = snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "wifi_pm_debug usage: \n"
                     " 1:dump host info 2:dump device info\n"
                     " 3:open wifi      4:close wifi \n"
                     " 5:enable pm      6:disable pm \n");
#else
    ret = snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "wifi_pm_debug usage: \n"
                     " 1:dump host info 2:dump device info\n");
#endif
    if (ret <= 0) {
        return ret;
    }

    ret += wlan_pm_host_info_print(pst_wlan_pm, buf + ret, PAGE_SIZE - ret);

    return ret;
}

STATIC ssize_t store_wifi_pmdbg(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    struct wlan_pm_s *pst_wlan_pm = wlan_pm_get_drv();
    int input;

    if (buf == NULL) {
        ps_print_err("buf is NULL\n");
        return -FAILURE;
    }

    input = oal_atoi(buf);

    if (pst_wlan_pm == NULL) {
        OAL_IO_PRINT("pm_data is NULL!\n");
        return -FAILURE;
    }

    /* case x => echo x->���Խڵ��� */
    switch (input) {
        case 1:
            wlan_pm_dump_host_info();
            break;
        case 2:
            wlan_pm_dump_device_info();
            break;
#ifdef PLATFORM_DEBUG_ENABLE
        case 3:
            wlan_pm_open();
            break;
        case 4:
            wlan_pm_close();
            break;
        case 5:
            wlan_pm_enable();
            break;
        case 6:
            wlan_pm_disable();
            break;
#endif
        default:
            break;
    }

    return count;
}

STATIC ssize_t show_bfgx_pmdbg(struct kobject *kobj, struct kobj_attribute *attr, int8 *buf)
{
    ps_print_info("%s\n", __func__);

    if (buf == NULL) {
        ps_print_err("buf is NULL\n");
        return -FAILURE;
    }

    return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "cmd       func     \n"
                      "  1  plat pm enable\n  2  plat pm disable\n"
                      "  3   bt  pm enable\n  4   bt  pm disable\n"
                      "  5  gnss pm enable\n  6  gnss pm disable\n"
                      "  7   nfc pm enable\n  8   nfc pm disable\n"
                      "  9  pm ctrl enable\n  10 pm ctrl disable\n"
                      "  11 pm debug switch\n ");
}

STATIC ssize_t store_bfgx_pmdbg(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    struct pm_drv_data *pm_data = NULL;
    struct ps_core_s *ps_core_d = NULL;
    int32 cmd;
    int32 ret;

    if (buf == NULL) {
        ps_print_err("buf is NULL\n");
        return -FAILURE;
    }

    cmd = simple_strtol(buf, NULL, 10); /* ���ַ���ת����10������ */
    ps_print_info("cmd:%d\n", cmd);

    pm_data = pm_get_drvdata();
    if (pm_data == NULL) {
        ps_print_err("pm_data is NULL!\n");
        return -FAILURE;
    }

    ps_get_core_reference(&ps_core_d);
    if (ps_core_d == NULL) {
        ps_print_err("ps_core_d is NULL\n");
        return -FAILURE;
    }

    ret = prepare_to_visit_node(ps_core_d);
    if (ret < 0) {
        ps_print_err("prepare work FAIL\n");
        return -FAILURE;
    }

    /* case x => echo x->���Խڵ��� */
    switch (cmd) {
        case 1: /* disable plat lowpower function */
            pm_data->bfgx_lowpower_enable = BFGX_PM_ENABLE;
            ps_print_info("bfgx platform pm enable\n");
            ps_tx_sys_cmd(ps_core_d, SYS_MSG, SYS_CFG_PL_ENABLE_PM);
            mod_timer(&pm_data->bfg_timer, jiffies + (BT_SLEEP_TIME * HZ / 1000));
            pm_data->bfg_timer_mod_cnt++;
            break;
        case 2: /* enable plat lowpower function */
            pm_data->bfgx_lowpower_enable = BFGX_PM_DISABLE;
            ps_print_info("bfgx platform pm disable\n");
            ps_tx_sys_cmd(ps_core_d, SYS_MSG, SYS_CFG_PL_DISABLE_PM);
            break;
        case 3: /* enable bt lowpower function */
            pm_data->bfgx_bt_lowpower_enable = BFGX_PM_ENABLE;
            ps_print_info("bfgx bt pm enable\n");
            ps_tx_sys_cmd(ps_core_d, SYS_MSG, SYS_CFG_BT_ENABLE_PM);
            break;
        case 4: /* disable bt lowpower function */
            pm_data->bfgx_bt_lowpower_enable = BFGX_PM_DISABLE;
            ps_print_info("bfgx bt pm disable\n");
            ps_tx_sys_cmd(ps_core_d, SYS_MSG, SYS_CFG_BT_DISABLE_PM);
            break;
        case 5: /* enable gnss lowpower function */
            pm_data->bfgx_gnss_lowpower_enable = BFGX_PM_ENABLE;
            ps_print_info("bfgx gnss pm enable\n");
            ps_tx_sys_cmd(ps_core_d, SYS_MSG, SYS_CFG_GNSS_ENABLE_PM);
            break;
        case 6: /* disable gnss lowpower function */
            pm_data->bfgx_gnss_lowpower_enable = BFGX_PM_DISABLE;
            ps_print_info("bfgx gnss pm disable\n");
            ps_tx_sys_cmd(ps_core_d, SYS_MSG, SYS_CFG_GNSS_DISABLE_PM);
            break;
        case 7: /* enable nfc lowpower function */
            pm_data->bfgx_nfc_lowpower_enable = BFGX_PM_ENABLE;
            break;
        case 8: /* disable nfc lowpower function */
            pm_data->bfgx_nfc_lowpower_enable = BFGX_PM_DISABLE;
            break;
        case 9:
            pm_data->bfgx_pm_ctrl_enable = BFGX_PM_ENABLE;
            break;
        case 10:
            pm_data->bfgx_pm_ctrl_enable = BFGX_PM_DISABLE;
            break;
        case 11:
            ps_tx_sys_cmd(ps_core_d, SYS_MSG, PL_PM_DEBUG);
            break;

        default:
            ps_print_err("unknown cmd %d\n", cmd);
            break;
    }

    post_to_visit_node(ps_core_d);

    return count;
}

STATIC struct kobj_attribute wifi_pmdbg =
    __ATTR(wifi_pm, 0644, (void *)show_wifi_pmdbg, (void *)store_wifi_pmdbg);

STATIC struct kobj_attribute bfgx_pmdbg =
    __ATTR(bfgx_pm, 0644, (void *)show_bfgx_pmdbg, (void *)store_bfgx_pmdbg);

STATIC struct attribute *pmdbg_attrs[] = {
    &wifi_pmdbg.attr,
    &bfgx_pmdbg.attr,
    NULL,
};

STATIC struct attribute_group pmdbg_attr_grp = {
    .attrs = pmdbg_attrs,
};

STATIC ssize_t show_download_mode(struct kobject *kobj, struct kobj_attribute *attr, int8 *buf)
{
    BOARD_INFO *tmp_board_info = NULL;
    if (buf == NULL) {
        ps_print_err("buf is NULL\n");
        return -FAILURE;
    }
    tmp_board_info = get_hi110x_board_info();
    if (tmp_board_info == NULL) {
        ps_print_err("tmp_board_info is null");
        return -FAILURE;
    }
    if (((tmp_board_info->wlan_download_channel) < MODE_DOWNLOAD_BUTT)
        && (tmp_board_info->bfgn_download_channel < MODE_DOWNLOAD_BUTT)) {
        return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "wlan:%s,bfgn:%s\n",
                          g_device_download_mode_list[tmp_board_info->wlan_download_channel].name,
                          g_device_download_mode_list[tmp_board_info->bfgn_download_channel].name);
    } else {
        ps_print_err("download_firmware_mode:%d error", tmp_board_info->wlan_download_channel);
        return -FAILURE;
    }
}

/* called by octty from hal to decide open or close uart */
STATIC ssize_t show_install(struct kobject *kobj, struct kobj_attribute *attr, int8 *buf)
{
    struct ps_plat_s *pm_data = NULL;

    PS_PRINT_FUNCTION_NAME;

    if (buf == NULL) {
        ps_print_err("buf is NULL\n");
        return -FAILURE;
    }

    pm_data = dev_get_drvdata(&hw_ps_device->dev);
    if (pm_data == NULL) {
        ps_print_err("pm_data is NULL!\n");
        return -FAILURE;
    }

    return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "%d\n", pm_data->ldisc_install);
}

/* read by octty from hal to decide open which uart */
STATIC ssize_t show_dev_name(struct kobject *kobj, struct kobj_attribute *attr, int8 *buf)
{
    struct ps_plat_s *pm_data = NULL;

    PS_PRINT_FUNCTION_NAME;

    if (buf == NULL) {
        ps_print_err("buf is NULL\n");
        return -FAILURE;
    }

    pm_data = dev_get_drvdata(&hw_ps_device->dev);
    if (pm_data == NULL) {
        ps_print_err("pm_data is NULL!\n");
        return -FAILURE;
    }

    return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "%s\n", pm_data->dev_name);
}

/* read by octty from hal to decide what baud rate to use */
STATIC ssize_t show_baud_rate(struct kobject *kobj, struct kobj_attribute *attr, int8 *buf)
{
    struct ps_plat_s *pm_data = NULL;

    PS_PRINT_FUNCTION_NAME;

    if (buf == NULL) {
        ps_print_err("buf is NULL\n");
        return -FAILURE;
    }

    pm_data = dev_get_drvdata(&hw_ps_device->dev);
    if (pm_data == NULL) {
        ps_print_err("pm_data is NULL!\n");
        return -FAILURE;
    }

    return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "%ld\n", pm_data->baud_rate);
}

/* read by octty from hal to decide whether or not use flow cntrl */
STATIC ssize_t show_flow_cntrl(struct kobject *kobj, struct kobj_attribute *attr, int8 *buf)
{
    struct ps_plat_s *pm_data = NULL;

    PS_PRINT_FUNCTION_NAME;

    if (buf == NULL) {
        ps_print_err("buf is NULL\n");
        return -FAILURE;
    }

    pm_data = dev_get_drvdata(&hw_ps_device->dev);
    if (pm_data == NULL) {
        ps_print_err("pm_data is NULL!\n");
        return -FAILURE;
    }

    return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "%d\n", pm_data->flow_cntrl);
}

/* show curr bfgx proto yes or not opened state */
STATIC ssize_t show_bfgx_active_state(struct kobject *kobj, struct kobj_attribute *attr, int8 *buf)
{
    struct ps_plat_s *pm_data = NULL;
    uint8 bt_state = POWER_STATE_SHUTDOWN;
    uint8 fm_state = POWER_STATE_SHUTDOWN;
    uint8 gnss_state = POWER_STATE_SHUTDOWN;
#ifdef HAVE_HISI_IR
    uint8 ir_state = POWER_STATE_SHUTDOWN;
#endif
#ifdef HAVE_HISI_NFC
    uint8 nfc_state = POWER_STATE_SHUTDOWN;
#endif

    ps_print_dbg("%s\n", __func__);

    if (buf == NULL) {
        ps_print_err("buf is NULL\n");
        return -FAILURE;
    }

    pm_data = dev_get_drvdata(&hw_ps_device->dev);
    if (pm_data == NULL) {
        ps_print_err("pm_data is NULL!\n");
        return -EFAULT;
    }

    if (atomic_read(&pm_data->core_data->bfgx_info[BFGX_BT].subsys_state) == true) {
        bt_state = POWER_STATE_OPEN;
    }

    if (atomic_read(&pm_data->core_data->bfgx_info[BFGX_FM].subsys_state) == true) {
        fm_state = POWER_STATE_OPEN;
    }

    if (atomic_read(&pm_data->core_data->bfgx_info[BFGX_GNSS].subsys_state) == true) {
        gnss_state = POWER_STATE_OPEN;
    }

#ifdef HAVE_HISI_IR
    if (atomic_read(&pm_data->core_data->bfgx_info[BFGX_IR].subsys_state) == true) {
        ir_state = POWER_STATE_OPEN;
    }
#endif

#ifdef HAVE_HISI_NFC
    if (atomic_read(&pm_data->core_data->bfgx_info[BFGX_NFC].subsys_state) == true) {
        nfc_state = POWER_STATE_OPEN;
    }
#endif

#if ((defined HAVE_HISI_IR) && (defined HAVE_HISI_NFC))
    return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "bt:%d; fm:%d; gnss:%d; ir:%d; nfc:%d;\n",
                      bt_state, fm_state, gnss_state, ir_state, nfc_state);
#elif (!(defined HAVE_HISI_IR) && !(defined HAVE_HISI_NFC))
    return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "bt:%d; fm:%d; gnss:%d;\n",
                      bt_state, fm_state, gnss_state);
#elif ((defined HAVE_HISI_IR))
    return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "bt:%d; fm:%d; gnss:%d; ir:%d;\n",
                      bt_state, fm_state, gnss_state, ir_state);
#else
    return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "bt:%d; fm:%d; gnss:%d; nfc:%d;\n",
                      bt_state, fm_state, gnss_state, nfc_state);
#endif
}

STATIC ssize_t show_ini_file_name(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    if (buf == NULL) {
        ps_print_err("buf is NULL\n");
        return -FAILURE;
    }

    return snprintf_s(buf, PAGE_SIZE, INI_FILE_PATH_LEN, "%s", g_ini_file_name);
}

STATIC ssize_t show_country_code(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
#ifdef _PRE_PLAT_FEATURE_CUSTOMIZE
    int8 *country_code = NULL;
    int ret;
    int8 ibuf[COUNTRY_CODE_LEN] = {0};

    if (buf == NULL) {
        ps_print_err("buf is NULL\n");
        return -FAILURE;
    }

    country_code = hwifi_get_country_code();
    if (strncmp(country_code, "99", strlen("99")) == 0) {
        ret = get_cust_conf_string(INI_MODU_WIFI, STR_COUNTRY_CODE, ibuf, sizeof(ibuf));
        if (ret == INI_SUCC) {
            ret = strncpy_s(buf, PAGE_SIZE, ibuf, COUNTRY_CODE_LEN);
            if (ret != EOK) {
                ps_print_err("strncpy_s error, please check!\n");
                return -FAILURE;
            }
            buf[COUNTRY_CODE_LEN - 1] = '\0';
            return strlen(buf);
        } else {
            ps_print_err("get dts country_code error\n");
            return 0;
        }
    } else {
        return snprintf_s(buf, PAGE_SIZE, COUNTRY_CODE_LEN, "%s", country_code);
    }
#else
    return -FAILURE;
#endif
}
STATIC ssize_t show_exception_info(struct kobject *kobj, struct kobj_attribute *attr, int8 *buf)
{
    ps_print_info("%s\n", __func__);

    if (buf == NULL) {
        ps_print_err("buf is NULL\n");
        return -FAILURE;
    }

    return hisi_conn_save_stat_info(buf, plat_get_dfr_sinfo(buf, 0), PAGE_SIZE);
    ;
}

STATIC ssize_t show_exception_count(struct kobject *kobj, struct kobj_attribute *attr, int8 *buf)
{
    ps_print_dbg("exception debug: wifi rst count is %d\n", plat_get_excp_total_cnt());
    return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "%d\n", plat_get_excp_total_cnt());
}

#ifdef HAVE_HISI_GNSS
STATIC ssize_t show_gnss_lowpower_state(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    struct pm_drv_data *pm_data = pm_get_drvdata();

    ps_print_dbg("show_gnss_lowpower_state!\n");

    if (buf == NULL) {
        ps_print_err("buf is NULL\n");
        return -FAILURE;
    }

    if (pm_data == NULL) {
        ps_print_err("pm_data is NULL!\n");
        return -FAILURE;
    }

    return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "%d\n", atomic_read(&pm_data->gnss_sleep_flag));
}

STATIC ssize_t gnss_lowpower_state_store(struct kobject *kobj, struct kobj_attribute *attr,
                                         const char *buf, size_t count)
{
    int flag;
    struct pm_drv_data *pm_data = NULL;

    if (buf == NULL) {
        ps_print_err("buf is NULL\n");
        return -FAILURE;
    }

    pm_data = pm_get_drvdata();
    if (pm_data == NULL) {
        ps_print_err("pm_data is NULL!\n");
        return -FAILURE;
    }

    flag = simple_strtol(buf, NULL, 10); /* ���ַ���ת����10������ */
    ps_print_info("flag = %d!\n", flag);

    /* gnss write the flag to request sleep */
    if (flag == 1) {
        if (pm_data->bfgx_lowpower_enable == BFGX_PM_DISABLE) {
            ps_print_warning("gnss low power disabled!\n");
            return -FAILURE;
        }
        if (pm_data->ps_pm_interface->bfgx_dev_state_get() == BFGX_SLEEP) {
            ps_print_warning("gnss proc: dev has been sleep, not allow dev slp\n");
            return -FAILURE;
        }

        /* if bt and fm are both shutdown ,we will pull down gpio directly */
        if (!timer_pending(&pm_data->bfg_timer)) {
            ps_print_info("gnss low power request sleep!\n");
            if (queue_work(pm_data->wkup_dev_workqueue, &pm_data->send_allow_sleep_work) != true) {
                ps_print_info("queue_work send_allow_sleep_work not return true\n");
            }
        }

        /* set the flag to 1 means gnss request sleep */
        atomic_set(&pm_data->gnss_sleep_flag, GNSS_AGREE_SLEEP);
    } else {
        ps_print_err("invalid gnss lowpower data!\n");
        return -FAILURE;
    }

    return count;
}
#endif

STATIC ssize_t show_loglevel(struct kobject *kobj, struct kobj_attribute *attr, int8 *buf)
{
    ps_print_info("%s\n", __func__);

    if (buf == NULL) {
        ps_print_err("buf is NULL\n");
        return -FAILURE;
    }

    return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1,
                      "curr loglevel=%d, curr bug_on=%d\nalert:0\nerr:1\nwarning:2\nfunc|succ|info:3\ndebug:4\nbug_on enable:b\nbug_on disable:B\n",
                      plat_loglevel, bug_on_enable);
}

STATIC ssize_t store_loglevel(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    int32 loglevel;

    ps_print_info("%s\n", __func__);

    if (buf == NULL) {
        ps_print_err("buf is NULL\n");
        return -FAILURE;
    }

    /* bug on set */
    if (*buf == 'b') {
        bug_on_enable = BUG_ON_ENABLE;
        ps_print_info("BUG_ON enable sucess, bug_on_enable = %d\n", bug_on_enable);
        return count;
    } else if (*buf == 'B') {
        bug_on_enable = BUG_ON_DISABLE;
        ps_print_info("BUG_ON disable sucess, bug_on_enable = %d\n", bug_on_enable);
        return count;
    } else if (*buf == 'r') {
        ps_set_device_log_status(true);
        ps_print_info("enable device log\n");
        return count;
    } else if (*buf == 'R') {
        ps_set_device_log_status(false);
        ps_print_info("disable device log\n");
        return count;
    }

    loglevel = simple_strtol(buf, NULL, 10); /* ���ַ���ת����10������ */
    if (loglevel < PLAT_LOG_ALERT) {
        plat_loglevel = PLAT_LOG_ALERT;
    } else if (loglevel > PLAT_LOG_DEBUG) {
        plat_loglevel = PLAT_LOG_DEBUG;
    } else {
        plat_loglevel = loglevel;
    }

    return count;
}

#ifdef HAVE_HISI_IR
STATIC ssize_t show_ir_mode(struct kobject *kobj, struct kobj_attribute *attr, int8 *buf)
{
    struct pm_drv_data *pm_data = pm_get_drvdata();

    ps_print_info("%s\n", __func__);

    if (pm_data == NULL) {
        ps_print_err("pm_data is NULL!\n");
        return -FAILURE;
    }

    if (!g_board_info.have_ir) {
        ps_print_err("board have no ir module");
        return -FAILURE;
    }
    if (buf == NULL) {
        ps_print_err("buf is NULL\n");
        return -FAILURE;
    }

    if (!isAsic()) {
        ps_print_err("HI1102 FPGA VERSION, ir contral gpio not exist\n");
        return -FAILURE;
    }

    if (g_board_info.irled_power_type == IR_GPIO_CTRL) {
        return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "%d\n", gpio_get_value(pm_data->board->bfgx_ir_ctrl_gpio));
    } else if (g_board_info.irled_power_type == IR_LDO_CTRL) {
        return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "%d\n",
                          (regulator_is_enabled(pm_data->board->bfgn_ir_ctrl_ldo)) > 0 ? 1 : 0);
    }

    return -FAILURE;
}

STATIC ssize_t store_ir_mode(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    int32 ir_ctrl_level;
    int ret;

    struct pm_drv_data *pm_data = pm_get_drvdata();

    if (pm_data == NULL) {
        ps_print_err("pm_data is NULL!\n");
        return -FAILURE;
    }

    ps_print_info("into %s,irled_power_type is %d\n", __func__, pm_data->board->irled_power_type);

    if (buf == NULL) {
        ps_print_err("buf is NULL\n");
        return -FAILURE;
    }

    if (!g_board_info.have_ir) {
        ps_print_err("board have no ir module");
        return -FAILURE;
    }

    if (pm_data->board->irled_power_type == IR_GPIO_CTRL) {
        if (!isAsic()) {
            ps_print_err("HI1102 FPGA VERSION, ignore ir contral gpio\n");
            return count;
        }

        ir_ctrl_level = simple_strtol(buf, NULL, 10); /* ���ַ���ת����10������ */
        if (ir_ctrl_level == GPIO_LOWLEVEL) {
            gpio_direction_output(pm_data->board->bfgx_ir_ctrl_gpio, GPIO_LOWLEVEL);
        } else if (ir_ctrl_level == GPIO_HIGHLEVEL) {
            gpio_direction_output(pm_data->board->bfgx_ir_ctrl_gpio, GPIO_HIGHLEVEL);
        } else {
            ps_print_err("gpio level should be 0 or 1, cur value is [%d]\n", ir_ctrl_level);
            return -FAILURE;
        }
    } else if (pm_data->board->irled_power_type == IR_LDO_CTRL) {
        if (IS_ERR(pm_data->board->bfgn_ir_ctrl_ldo)) {
            ps_print_err("ir_ctrl get ird ldo failed\n");
            return -FAILURE;
        }

        ir_ctrl_level = simple_strtol(buf, NULL, 10); /* ���ַ���ת����10������ */
        if (ir_ctrl_level == GPIO_LOWLEVEL) {
            ret = regulator_disable(pm_data->board->bfgn_ir_ctrl_ldo);
            if (ret) {
                ps_print_err("ir_ctrl disable ldo failed\n");
            }
        } else if (ir_ctrl_level == GPIO_HIGHLEVEL) {
            ret = regulator_enable(pm_data->board->bfgn_ir_ctrl_ldo);
            if (ret) {
                ps_print_err("ir_ctrl enable ldo failed\n");
            }
        } else {
            ps_print_err("ir_ctrl level should be 0 or 1, cur value is [%d]\n", ir_ctrl_level);
            return -FAILURE;
        }
    } else {
        ps_print_err("get ir_ldo_type error! ir_ldo_type is %d!\n", pm_data->board->irled_power_type);
        return -FAILURE;
    }
    ps_print_info("set ir ctrl mode as %d!\n", (int)ir_ctrl_level);
    return count;
}
#endif

STATIC ssize_t bfgx_sleep_state_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    struct pm_drv_data *pm_data = pm_get_drvdata();

    if (buf == NULL) {
        ps_print_err("buf is NULL\n");
        return -FAILURE;
    }

    if (pm_data == NULL) {
        ps_print_err("pm_data is NULL!\n");
        return -FAILURE;
    }

    return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "%d\n", pm_data->bfgx_dev_state);
}

STATIC ssize_t bfgx_wkup_host_count_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    struct pm_drv_data *pm_data = pm_get_drvdata();

    if (buf == NULL) {
        ps_print_err("buf is NULL\n");
        return -FAILURE;
    }

    if (pm_data == NULL) {
        ps_print_err("pm_data is NULL!\n");
        return -FAILURE;
    }

    return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "%d\n", pm_data->bfg_wakeup_host);
}

/*
 * �� �� ��  : get_bin_file_path
 * ��������  : ��ȡconfig�ļ��н���������device bin�ļ���·��
 * �������  : bin_file_num ptr
 * �������  : int32 *bin_file �ļ�����
 */
STATIC int8 **get_bin_file_path(int32 *bin_file_num)
{
    int32 loop;
    int32 l_len;
    int32 index = 0;
    int8 **path;
    int8 *begin = NULL;
    if (bin_file_num == NULL) {
        ps_print_err("bin file count is NULL!\n");
        return NULL;
    }
    *bin_file_num = 0;
    /* �ҵ�ȫ�ֱ����д�����ļ��ĸ��� */
    for (loop = 0; loop < g_cfg_info.al_count[BFGX_AND_WIFI_CFG]; loop++) {
        if (g_cfg_info.apst_cmd[BFGX_AND_WIFI_CFG][loop].cmd_type == FILE_TYPE_CMD) {
            (*bin_file_num)++;
        }
    }
    /* Ϊ���bin�ļ�·����ָ����������ռ� */
    path = (int8 **)OS_KMALLOC_GFP((*bin_file_num) * OAL_SIZEOF(int8 *));
    if (unlikely(path == NULL)) {
        ps_print_err("malloc path space fail!\n");
        return NULL;
    }
    /* ��֤û��ʹ�õ�����Ԫ��ȫ��ָ��NULL,��ֹ�����ͷ� */
    memset_s((void *)path, (*bin_file_num) * OAL_SIZEOF(int8 *), 0, (*bin_file_num) * OAL_SIZEOF(int8 *));
    /* ��bin�ļ���·��ȫ��������һ��ָ�������� */
    for (loop = 0; loop < g_cfg_info.al_count[BFGX_AND_WIFI_CFG]; loop++) {
        if (g_cfg_info.apst_cmd[BFGX_AND_WIFI_CFG][loop].cmd_type == FILE_TYPE_CMD && index < *bin_file_num) {
            begin = OS_STR_CHR(g_cfg_info.apst_cmd[BFGX_AND_WIFI_CFG][loop].cmd_para, '/');
            if (begin == NULL) {
                continue;
            }
            l_len = (int32)OAL_STRLEN(begin);
            path[index] = (int8 *)OS_KMALLOC_GFP(l_len + 1);
            if (path[index] == NULL) {
                ps_print_err("malloc file path mem fail! index is %d\n", index);
                for (loop = 0; loop < index; loop++) {
                    USERCTL_KFREE(path[loop]);
                }
                USERCTL_KFREE(path);
                return NULL;
            }
            memcpy_s(path[index], l_len + 1, begin, l_len + 1);
            index++;
        }
    }
    return path;
}

/*
 * �� �� �� : dev_version_show
 * �������� : ��ʾdevice����汾��Ϣ
 */
STATIC ssize_t dev_version_show(struct kobject *kobj, struct kobj_attribute *attr, int8 *buf)
{
    BOARD_INFO *hi110x_board_info = NULL;
    int8 *dev_version_bfgx = NULL;
    int8 *dev_version_wifi = NULL;
    int8 **pca_bin_file_path;
    int32 bin_file_num, loop, ret;
    ps_print_info("%s\n", __func__);

    if (unlikely(buf == NULL)) {
        ps_print_err("buf is NULL\n");
        return -FAILURE;
    }
    hi110x_board_info = get_hi110x_board_info();
    if (unlikely(hi110x_board_info == NULL)) {
        ps_print_err("get board info fail\n");
        return -FAILURE;
    }

    /* ��1103ϵͳ��ʱ��֧��device����汾�� */
    if (hi110x_board_info->chip_nr != BOARD_VERSION_HI1103) {
        return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "%s.\n%s.\n"
                          "NOTE:\n"
                          "     device software version only support on hi1103 now!!!\n",
                          hi110x_board_info->chip_type, param_version.param_version);
    }
    /* ���device�Ƿ�򿪹���cfg�����ļ��Ƿ������, ����ȫ�ֱ�����û������, ��ʾ�û�����һ��frimware */
    if (g_cfg_info.al_count[BFGX_AND_WIFI_CFG] == 0) {
        return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "%s.\n%s.\n"
                          "NOTE:\n"
                          "You need open bt or wifi once to download frimware to get device bin file path to parse!\n",
                          hi110x_board_info->chip_type, param_version.param_version);
    }
    /* ��ȫ�ֱ����л�ȡbin�ļ���·������ָ��bin�ļ�����·����ָ������ */
    pca_bin_file_path = get_bin_file_path(&bin_file_num);
    if (unlikely(pca_bin_file_path == NULL)) {
        ps_print_err("get bin file path from cfg_info fail\n");
        return -FAILURE;
    }
    /* �����ҵ������е�bin�ļ�����device����汾�� */
    for (loop = 0; loop < bin_file_num; loop++) {
        dev_version_bfgx = get_str_from_file(pca_bin_file_path[loop], DEV_SW_STR_BFGX);
        if (dev_version_bfgx != NULL) {
            break;
        }
    }
    for (loop = 0; loop < bin_file_num; loop++) {
        dev_version_wifi = get_str_from_file(pca_bin_file_path[loop], DEV_SW_STR_WIFI);
        if (dev_version_wifi != NULL) {
            break;
        }
    }
    ret = snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1,
                     "%s.\n%s.\nBFGX DEVICE VERSION:%s.\nWIFI DEVICE VERSION:%s.\n",
                     hi110x_board_info->chip_type, param_version.param_version,
                     dev_version_bfgx, dev_version_wifi);
    /* �ͷ�����������ڴ�ռ� */
    for (loop = 0; loop < bin_file_num; loop++) {
        USERCTL_KFREE(pca_bin_file_path[loop]);
    }
    USERCTL_KFREE(pca_bin_file_path);
    USERCTL_KFREE(dev_version_bfgx);
    USERCTL_KFREE(dev_version_wifi);
    return ret;
}

#ifdef CONFIG_HISI_PMU_RTC_READCOUNT
STATIC ssize_t show_hisi_gnss_ext_rtc_count(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    unsigned long rtc_count;

    if ((kobj == NULL) || (attr == NULL) || (buf == NULL)) {
        ps_print_err("paramater is NULL\n");
        return 0;
    }

    rtc_count = hisi_pmu_rtc_readcount();
    ps_print_info("show_hisi_gnss_ext_rtc_count: %ld\n", rtc_count);
    if (memcpy_s(buf, PAGE_SIZE, (char *)&rtc_count, sizeof(rtc_count)) != EOK) {
        ps_print_err("memcpy_s error, destlen=%lu, srclen=%lu\n ", PAGE_SIZE, sizeof(rtc_count));
        return 0;
    }
    return sizeof(rtc_count);
}

#endif
STATIC struct kobj_attribute plat_exception_info =
    __ATTR(excp_info, 0444, (void *)show_exception_info, NULL);

#ifdef CONFIG_HI110X_GPS_SYNC
STATIC int32 gnss_sync_convert_mode_modem0(int32 rat_mode, uint32 version)
{
    int32 sync_mode = gnss_sync_mode_unknown;
    switch (rat_mode) {
        case gnss_rat_mode_gsm:
            sync_mode = gnss_sync_mode_g1;
            break;
        case gnss_rat_mode_wcdma:
            sync_mode = gnss_sync_mode_pw;
            break;
        case gnss_rat_mode_lte:
            sync_mode = gnss_sync_mode_lte;
            break;
        case gnss_rat_mode_cdma:
            sync_mode = gnss_sync_mode_cdma;
            break;
        case gnss_rat_mode_nr:
            if (version >= gnss_sync_version_5g) {
                sync_mode = gnss_sync_mode_nstu;
            }
            break;
        default:
            sync_mode = gnss_sync_mode_unknown;
            break;
    }
    return sync_mode;
}

STATIC int32 gnss_sync_convert_mode_modem1(int32 rat_mode)
{
    int32 sync_mode;
    switch (rat_mode) {
        case gnss_rat_mode_gsm:
            sync_mode = gnss_sync_mode_g2;
            break;
        case gnss_rat_mode_wcdma:
            sync_mode = gnss_sync_mode_sw;
            break;
        case gnss_rat_mode_lte:
            sync_mode = gnss_sync_mode_lte2;
            break;
        case gnss_rat_mode_cdma:
            sync_mode = gnss_sync_mode_cdma;
            break;
        default:
            sync_mode = gnss_sync_mode_unknown;
            break;
    }
    return sync_mode;
}

STATIC int32 gnss_sync_convert_mode_modem2(int32 rat_mode)
{
    int32 sync_mode;
    switch (rat_mode) {
        case gnss_rat_mode_gsm:
            sync_mode = gnss_sync_mode_g3;
            break;
        case gnss_rat_mode_cdma:
            sync_mode = gnss_sync_mode_cdma;
            break;
        default:
            sync_mode = gnss_sync_mode_unknown;
            break;
    }
    return sync_mode;
}

/* Driver��Host�ӿڶ���Ϊ���¼���������ÿ������֮���á�,���ָ�      */
/* "modem_id,rat_mode"                                            */
STATIC int32 gnss_sync_convert_mode(const int8 *rcv_data, int32 *set_mode, uint32 version)
{
    int8 *pc_split = ",";
    int8 *pc_str1 = (int8 *)rcv_data;
    int8 *pc_str2 = NULL;
    int32 modem_id;
    int32 rat_mode;

    if (pc_str1 == OAL_PTR_NULL || set_mode == OAL_PTR_NULL) {
        ps_print_err("received data or set mode is null\n");
        return -FAILURE;
    }

    modem_id = oal_atoi(pc_str1);
    if (modem_id < 0) {
        ps_print_err("modem id is invalid\n");
        return -FAILURE;
    }

    pc_str2 = oal_strstr(pc_str1, pc_split);
    if (pc_str2 == OAL_PTR_NULL) {
        ps_print_err("cannot find the split\n");
        return -FAILURE;
    }

    pc_str2++;
    rat_mode = oal_atoi(pc_str2);
    if (rat_mode < gnss_rat_mode_no_service || rat_mode >= gnss_rat_mode_butt) {
        ps_print_err("rat mode is invalid\n");
        return -FAILURE;
    }

    switch (modem_id) {
        case 0:
            *set_mode = gnss_sync_convert_mode_modem0(rat_mode, version);
            break;
        case 1:
            *set_mode = gnss_sync_convert_mode_modem1(rat_mode);
            break;
        case 2:
            *set_mode = gnss_sync_convert_mode_modem2(rat_mode);
            break;
        default:
            *set_mode = gnss_sync_mode_unknown;
            break;
    }

    return SUCCESS;
}

STATIC int32 gnss_sync_mode_ctrl(struct gnss_sync_data *sync_data, int32 set_mode)
{
    int32 curr_mode = 0;
    if (set_mode != gnss_sync_mode_unknown) {
        curr_mode = readl(sync_data->addr_base_virt + sync_data->addr_offset);
        curr_mode = (int32)(((uint32)curr_mode & 0xfffffff0) | (uint32)set_mode); // only bit 3:0
        writel(curr_mode, (sync_data->addr_base_virt + sync_data->addr_offset));         // model selection
    } else {
        return -FAILURE;
    }

    return SUCCESS;
}

STATIC ssize_t show_hisi_gnss_sync_mode(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    int32 mode;
    struct gnss_sync_data *sync_data = gnss_get_sync_data();

    ps_print_dbg("show hisi gnss sync mode!\n");

    if (buf == NULL) {
        ps_print_err("buf is NULL\n");
        return -FAILURE;
    }

    if (sync_data == NULL) {
        ps_print_err("sync_data is NULL!\n");
        return -FAILURE;
    }

    if (sync_data->addr_base_virt == NULL) {
        ps_print_err("register virutal address is NULL!\n");
        return -FAILURE;
    }

    mode = readl(sync_data->addr_base_virt + sync_data->addr_offset);

    return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "%d\n", mode);
}

STATIC ssize_t store_hisi_gnss_sync_mode(struct kobject *kobj, struct kobj_attribute *attr,
                                         const char *buf, size_t count)
{
    int32 ret = -FAILURE;
    int32 set_mode = gnss_sync_mode_unknown;
    struct gnss_sync_data *sync_data = NULL;

    ps_print_info("store hisi gnss sync mode!\n");

    if (buf == NULL) {
        ps_print_err("buf is NULL\n");
        return -FAILURE;
    }

    sync_data = gnss_get_sync_data();
    if (sync_data == NULL) {
        ps_print_err("sync_data is NULL!\n");
        return -FAILURE;
    }

    if (sync_data->addr_base_virt == NULL) {
        ps_print_err("register virutal address is NULL!\n");
        return -FAILURE;
    }

    if (sync_data->version > gnss_sync_version_off && sync_data->version < gnss_sync_version_butt) {
        ret = gnss_sync_convert_mode(buf, &set_mode, sync_data->version);
    } else {
        ps_print_warning("sync version %d has not been realized!\n", sync_data->version);
        return -FAILURE;
    }

    if (ret != SUCCESS) {
        return ret;
    }
    ps_print_info("gnss sync mode \"%s\" convert to %d", buf, set_mode);

    ret = gnss_sync_mode_ctrl(sync_data, set_mode);
    if (ret != SUCCESS) {
        return ret;
    }

    return count;
}
#endif

STATIC struct kobj_attribute download_mode =
    __ATTR(device_download_mode, 0444, (void *)show_download_mode, NULL);

STATIC struct kobj_attribute ldisc_install =
    __ATTR(install, 0444, (void *)show_install, NULL);

STATIC struct kobj_attribute uart_dev_name =
    __ATTR(dev_name, 0444, (void *)show_dev_name, NULL);

STATIC struct kobj_attribute uart_baud_rate =
    __ATTR(baud_rate, 0444, (void *)show_baud_rate, NULL);

STATIC struct kobj_attribute uart_flow_cntrl =
    __ATTR(flow_cntrl, 0444, (void *)show_flow_cntrl, NULL);

STATIC struct kobj_attribute bfgx_active_state =
    __ATTR(bfgx_state, 0444, (void *)show_bfgx_active_state, NULL);

STATIC struct kobj_attribute ini_file_name =
    __ATTR(ini_file_name, 0444, (void *)show_ini_file_name, NULL);

STATIC struct kobj_attribute country_code =
    __ATTR(country_code, 0444, (void *)show_country_code, NULL);

STATIC struct kobj_attribute rst_count =
    __ATTR(rst_count, 0444, (void *)show_exception_count, NULL);

#ifdef HAVE_HISI_GNSS
STATIC struct kobj_attribute gnss_lowpower_cntrl =
    __ATTR(gnss_lowpower_state, 0644, (void *)show_gnss_lowpower_state, (void *)gnss_lowpower_state_store);
#endif

STATIC struct kobj_attribute bfgx_loglevel =
    __ATTR(loglevel, 0664, (void *)show_loglevel, (void *)store_loglevel);

#ifdef HAVE_HISI_IR
STATIC struct kobj_attribute bfgx_ir_ctrl =
    __ATTR(ir_ctrl, 0664, (void *)show_ir_mode, (void *)store_ir_mode);
#endif

#ifdef CONFIG_HISI_PMU_RTC_READCOUNT
STATIC struct kobj_attribute hisi_gnss_ext_rtc =
    __ATTR(gnss_ext_rtc, 0444, (void *)show_hisi_gnss_ext_rtc_count, NULL);
#endif

#ifdef CONFIG_HI110X_GPS_SYNC
STATIC struct kobj_attribute hisi_gnss_sync =
    __ATTR(gnss_sync, 0664, (void *)show_hisi_gnss_sync_mode, (void *)store_hisi_gnss_sync_mode);
#endif

STATIC struct kobj_attribute bfgx_sleep_attr =
    __ATTR(bfgx_sleep_state, 0444, (void *)bfgx_sleep_state_show, NULL);

STATIC struct kobj_attribute bfgx_wkup_host_count_attr =
    __ATTR(bfgx_wkup_host_count, 0444, (void *)bfgx_wkup_host_count_show, NULL);

STATIC struct kobj_attribute device_version =
    __ATTR(version, 0444, (void *)dev_version_show, NULL);

STATIC struct attribute *bfgx_attrs[] = {
    &download_mode.attr,
    &ldisc_install.attr,
    &uart_dev_name.attr,
    &uart_baud_rate.attr,
    &uart_flow_cntrl.attr,
    &bfgx_active_state.attr,
    &ini_file_name.attr,
    &country_code.attr,
    &rst_count.attr,
    &plat_exception_info.attr,
#ifdef HAVE_HISI_GNSS
    &gnss_lowpower_cntrl.attr,
#endif
    &bfgx_loglevel.attr,
#ifdef HAVE_HISI_IR
    &bfgx_ir_ctrl.attr,
#endif
    &bfgx_sleep_attr.attr,
    &bfgx_wkup_host_count_attr.attr,
    &device_version.attr,
#ifdef CONFIG_HISI_PMU_RTC_READCOUNT
    &hisi_gnss_ext_rtc.attr,
#endif
#ifdef CONFIG_HI110X_GPS_SYNC
    &hisi_gnss_sync.attr,
#endif
    NULL,
};

STATIC struct attribute_group bfgx_attr_grp = {
    .attrs = bfgx_attrs,
};

#ifdef PLATFORM_DEBUG_ENABLE
STATIC ssize_t show_exception_dbg(struct kobject *kobj, struct kobj_attribute *attr, int8 *buf)
{
    ps_print_info("%s\n", __func__);

    if (buf == NULL) {
        ps_print_err("buf is NULL\n");
        return -FAILURE;
    }

    return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1,
                      "==========cmd  func             \n"
                      "   1  clear dfr info          \n"
                      "   2  cause bfgx beattimer timerout\n"
                      "   3  enable dfr subsystem rst\n"
                      "   4  disble dfr subsystem rst\n"
                      "   5  en dfr bfgx pwron fail\n"
                      "   6  en dfr bfgx pwroff fail\n"
                      "   7  en dfr wifi wkup fail\n"
                      "   8  cause bfgx panic\n"
                      "   9  cause bfgx arp exception system rst\n"
                      "   10 bfgx wkup fail\n"
                      "   11 wifi WIFI_TRANS_FAIL\n"
                      "   12 wifi WIFI_POWER ON FAIL\n");
}

STATIC ssize_t store_exception_dbg(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    int32 cmd;
    int32 ret = 0;
    struct ps_core_s *ps_core_d = NULL;
    struct st_exception_info *pst_exception_data = NULL;
    struct wlan_pm_s *pst_wlan_pm = wlan_pm_get_drv();
    hcc_bus *hi_bus = hcc_get_current_110x_bus();
    struct hcc_handler *pst_hcc = hcc_get_110x_handler();

    if (buf == NULL) {
        ps_print_err("[dfr_test]buf is NULL\n");
        return -FAILURE;
    }
    if (hi_bus == NULL) {
        ps_print_err("[dfr_test]hi_bus is null");
        return -FAILURE;
    }

    get_exception_info_reference(&pst_exception_data);
    if (pst_exception_data == NULL) {
        ps_print_err("[dfr_test]get exception info reference is error\n");
        return 0;
    }

    ps_get_core_reference(&ps_core_d);
    if (ps_core_d == NULL) {
        ps_print_err("[dfr_test]ps_core_d is NULL\n");
        return 0;
    }

    cmd = simple_strtol(buf, NULL, 10); /* ���ַ���ת����10������ */
    ps_print_info("[dfr_test]cmd:%d\n", cmd);
    /* case x => echo x->���Խڵ��� */
    switch (cmd) {
        case 1:
            ps_print_info("[dfr_test]clear dfr info\n");
            memset_s(&pst_exception_data->etype_info, sizeof(pst_exception_data->etype_info),
                     0, sizeof(pst_exception_data->etype_info));
            break;
        case 2:
            ps_print_info("[dfr_test]bfgx device timeout cause ++\n");
            ret = prepare_to_visit_node(ps_core_d);
            if (ret < 0) {
                ps_print_err("[dfr_test]prepare work FAIL\n");
                return ret;
            }

            ret = is_dfr_test_en(BFGX_POWEON_FAULT);
            ps_print_info("[dfr_test]before timout test, poweron fail test status 0x%x", ret);

            pst_exception_data->debug_beat_flag = 0;
            /* �ȴ�dfr��ɣ��ȴ�����dfr���̣���ֹ˯�� */
            while (atomic_read(&pst_exception_data->is_reseting_device) == PLAT_EXCEPTION_RESET_IDLE) {
                ;
            };
            post_to_visit_node(ps_core_d);
            ps_print_info("[dfr_test]bfgx device timeout cause --\n");
            break;
        case 3:
            ps_print_info("[dfr_test]enable dfr subsystem rst\n");
            pst_exception_data->subsystem_rst_en = DFR_TEST_ENABLE;
            break;
        case 4:
            ps_print_info("[dfr_test]disable dfr subsystem rst\n");
            pst_exception_data->subsystem_rst_en = DFR_TEST_DISABLE;
            break;
        case 5:
            ps_print_info("[dfr_test]bfgx powon fail dfr test en,next poweron will fail\n");
            set_excp_test_en(BFGX_POWEON_FAULT);
            break;
        case 6:
            ps_print_info("[dfr_test]bfgx pwr off dfr test en,next poweroff will fail\n");
            set_excp_test_en(BFGX_POWEOFF_FAULT);
            break;
        case 7:
            if (pst_hcc == NULL) {
                ps_print_info("pst_hcc is null\n");
                break;
            }
            if (oal_atomic_read(&pst_hcc->state) != HCC_ON) {
                ps_print_info("wifi is closed\n");
                break;
            }
            ps_print_info("[dfr_test]wifi wkup dfr test en,next wkup will fail\n");
            if (pst_wlan_pm->ul_wlan_dev_state == HOST_ALLOW_TO_SLEEP) {
                hcc_tx_transfer_lock(hcc_get_110x_handler());
                set_excp_test_en(WIFI_WKUP_FAULT);
                wlan_pm_wakeup_dev();
                hcc_tx_transfer_unlock(hcc_get_110x_handler());
            } else {
                ps_print_warning("[dfr_test]wifi wkup dfr trigger fail , cur state:%ld\n",
                                 pst_wlan_pm->ul_wlan_dev_state);
            }
            break;
        case 8:
            ret = prepare_to_visit_node(ps_core_d);
            if (ret < 0) {
                ps_print_err("[dfr_test]prepare work FAIL\n");
                return ret;
            }
            ps_print_info("[dfr_test]bfgx device panic cause\n");
            ps_tx_sys_cmd(ps_core_d, SYS_MSG, SYS_CFG_DEV_PANIC);
            post_to_visit_node(ps_core_d);
            break;
        case 9:
            ps_print_info("[dfr_test]trigger hal BFGX_ARP_TIMEOUT exception\n");
            ret = is_dfr_test_en(BFGX_POWEON_FAULT);
            ps_print_info("[dfr_test]before arp timeout test, poweron fail test status 0x%x", ret);

            plat_exception_handler(SUBSYS_BFGX, THREAD_GNSS, BFGX_ARP_TIMEOUT);
            break;
        case 10:
            ps_print_info("[dfr_test]bfgx wkup dfr test en,next wkup will fail \n");
            plat_exception_handler(SUBSYS_BFGX, THREAD_NFC, BFGX_WAKEUP_FAIL);
            break;
        case 11:
            if (pst_hcc == NULL) {
                ps_print_info("pst_hcc is null\n");
                break;
            }
            if (oal_atomic_read(&pst_hcc->state) != HCC_ON) {
                ps_print_info("wifi is closed\n");
                break;
            }
            ps_print_info("[dfr_test]wifi WIFI_TRANS_FAIL submit \n");
            hcc_tx_transfer_lock(pst_hcc);
            if (hcc_bus_pm_wakeup_device(hi_bus) == OAL_SUCC) {
                hcc_bus_exception_submit(hi_bus, WIFI_TRANS_FAIL);
            }
            hcc_tx_transfer_unlock(pst_hcc);
            break;

        case 12:
            ps_print_info("[dfr_test]wifi WIFI_power on failed\n");
            set_excp_test_en(WIFI_POWER_ON_FAULT);
            break;

        default:
            ps_print_err("[dfr_test]unknown cmd %d\n", cmd);
            break;
    }
    return count;
}

STATIC ssize_t show_uart_rx_dump(struct kobject *kobj, struct kobj_attribute *attr, int8 *buf)
{
    ps_print_info("[ps_uart_dump] %s\n", __func__);

    if (buf == NULL) {
        ps_print_err("[ps_uart_dump] buf is NULL\n");
        return -FAILURE;
    }

    return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "curr uart dump status =%d\n no:0\n yes:1\n", uart_rx_dump);
}

STATIC ssize_t store_uart_rx_dump(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    struct ps_core_s *ps_core_d = NULL;

    if (buf == NULL) {
        ps_print_err("[ps_uart_dump] buf is NULL\n");
        return -FAILURE;
    }

    ps_get_core_reference(&ps_core_d);
    if (unlikely(ps_core_d == NULL)) {
        ps_print_err("[ps_uart_dump]ps_core_d is NULL\n");
        return -EINVAL;
    }
    uart_rx_dump = simple_strtol(buf, NULL, 10); /* ���ַ���ת����10������ */
    /* ���ر�uart_rx_dump���ܣ����ļ�Ҳ�ر� */
    if (!uart_rx_dump) {
        ps_core_d->pre_time = 0;
        if (ps_core_d->rx_data_fp != NULL) {
            filp_close(ps_core_d->rx_data_fp, NULL);
            ps_core_d->rx_data_fp = NULL;
        }
    }
    ps_print_info("[ps_uart_dump]uart_rx_dump aft %d\n", uart_rx_dump);
    return count;
}

STATIC ssize_t show_dev_test(struct kobject *kobj, struct kobj_attribute *attr, int8 *buf)
{
    ps_print_info("%s\n", __func__);

    if (buf == NULL) {
        ps_print_err("buf is NULL\n");
        return -FAILURE;
    }

    return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1,
                      "cmd  func\n  1  cause bfgx panic\n  2  enable exception recovery\n  3  enable wifi open bcpu\n"
                      "  4  pull up power gpio\n  5  pull down power gpio\n  6  uart loop test\n"
                      "  7  wifi download test\n  8  open uart\n  9  close uart\n"
                      "  10 open bt\n  11 bfg download test\n  12 bfg wifi download test\n"
                      "  13 wlan_pm_open_bcpu\n  14 wlan_power_on \n  15 wlan_power_off\n"
                      "  16 wlan_pm_open\n  17 wlan_pm_close\n  18 wlan gpio power on\n  19 bfg gpio power on\n  20 gnss monitor enable\n"
                      "  21 rdr test\n  22 bfgx power on\n");
}

STATIC ssize_t store_dev_test(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    int32 cmd;
    int32 ret;
    uint8 send_data[] = { 0x7e, 0x0, 0x06, 0x0, 0x0, 0x7e };
    struct ps_core_s *ps_core_d = NULL;
    struct st_exception_info *pst_exception_data = NULL;
    BOARD_INFO *bd_info = NULL;

    ps_print_info("%s\n", __func__);

    bd_info = get_hi110x_board_info();
    if (unlikely(bd_info == NULL)) {
        ps_print_err("board info is err\n");
        return BFGX_POWER_FAILED;
    }

    if (buf == NULL) {
        ps_print_err("buf is NULL\n");
        return -FAILURE;
    }

    get_exception_info_reference(&pst_exception_data);
    if (pst_exception_data == NULL) {
        ps_print_err("get exception info reference is error\n");
        return 0;
    }

    ps_get_core_reference(&ps_core_d);
    if (unlikely(ps_core_d == NULL)) {
        ps_print_err("ps_core_d is NULL\n");
        return -EINVAL;
    }

    cmd = simple_strtol(buf, NULL, 10); /* ���ַ���ת����10������ */
    /* case x => echo x->���Խڵ��� */
    switch (cmd) {
        case 1:
            ret = prepare_to_visit_node(ps_core_d);
            if (ret < 0) {
                ps_print_err("prepare work FAIL\n");
                return ret;
            }

            ps_print_info("bfgx test cmd %d, cause device panic\n", cmd);
            ps_tx_sys_cmd(ps_core_d, SYS_MSG, SYS_CFG_DEV_PANIC);

            post_to_visit_node(ps_core_d);
            break;
        case 2:
            ps_print_info("cmd %d,enable platform dfr\n", cmd);
            pst_exception_data->exception_reset_enable = PLAT_EXCEPTION_ENABLE;
            break;
        case 3:
            ps_print_info("cmd %d,enable wifi open bcpu\n", cmd);
            wifi_open_bcpu_set(1);
            break;
        case 4:
            ps_print_info("cmd %d,test pull up power gpio\n", cmd);
            bd_info->bd_ops.board_power_on(WLAN_POWER);
            break;
        case 5:
            ps_print_info("cmd %d,test pull down power gpio\n", cmd);
            bd_info->bd_ops.board_power_off(WLAN_POWER);
            break;
        case 6:
            ps_print_info("cmd %d,start uart loop test\n", cmd);
            uart_loop_test();
            break;
        case 7:
            ps_print_info("cmd %d,firmware download wifi_cfg\n", cmd);
            firmware_download_function(WIFI_CFG);
            break;
        case 8:
            ps_print_info("cmd %d,open uart\n", cmd);
            open_tty_drv(ps_core_d->pm_data);
            break;
        case 9:
            ps_print_info("cmd %d,close uart\n", cmd);
            release_tty_drv(ps_core_d->pm_data);
            break;
        case 10:
            ps_print_info("cmd %d,uart cmd test\n", cmd);
            ps_write_tty(ps_core_d, send_data, sizeof(send_data));
            break;
        case 11:
            ps_print_info("cmd %d,firmware download bfgx_cfg\n", cmd);
            firmware_download_function(BFGX_CFG);
            break;
        case 12:
            ps_print_info("cmd %d,firmware download bfgx_and_wifi cfg\n", cmd);
            firmware_download_function(BFGX_AND_WIFI_CFG);
            break;
        case 13:
            ps_print_info("cmd %d,wlan_pm_open_bcpu\n", cmd);
            wlan_pm_open_bcpu();
            break;
        case 14:
            ps_print_info("cmd %d,hi1103_wlan_power_on\n", cmd);
            hi1103_wlan_power_on();
            break;
        case 15:
            ps_print_info("cmd %d,hi1103_wlan_power_off\n", cmd);
            hi1103_wlan_power_off();
            break;
        case 16:
            ps_print_info("cmd %d,wlan_pm_open\n", cmd);
            wlan_pm_open();
            break;
        case 17:
            ps_print_info("cmd %d,wlan_pm_close\n", cmd);
            wlan_pm_close();
            break;
        case 18:
            ps_print_info("cmd %d,wlan gpio power on\n", cmd);
            bd_info->bd_ops.board_power_on(WLAN_POWER);
            break;
        case 19:
            ps_print_info("cmd %d,bfgx gpio power on\n", cmd);
            bd_info->bd_ops.board_power_on(BFGX_POWER);
            break;
        case 20:
            ps_print_info("cmd %d,bfgx gpio power on\n", cmd);
            g_device_monitor_enable = 1;
            break;
        case 21:
            RDR_EXCEPTION(MODID_CONN_WIFI_EXEC);
            RDR_EXCEPTION(MODID_CONN_WIFI_CHAN_EXEC);
            RDR_EXCEPTION(MODID_CONN_WIFI_WAKEUP_FAIL);
            RDR_EXCEPTION(MODID_CONN_BFGX_EXEC);
            RDR_EXCEPTION(MODID_CONN_BFGX_BEAT_TIMEOUT);
            RDR_EXCEPTION(MODID_CONN_BFGX_WAKEUP_FAIL);
            break;
        case 22:
            ps_print_err("bfgx power on %d\n", cmd);
            bfgx_dev_power_control(BFGX_BT, BFG_POWER_GPIO_UP);
            break;
        default:
            ps_print_err("unknown cmd %d\n", cmd);
            break;
    }

    return count;
}

STATIC ssize_t show_octty_test(struct kobject *kobj, struct kobj_attribute *attr, int8 *buf)
{
    ps_print_info("%s\n", __func__);

    if (buf == NULL) {
        ps_print_err("buf is NULL\n");
        return -FAILURE;
    }

    return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "cmd  func\n 1 close octty\n 2 open octty\n"
                      " 3 change uart buard rate to wake_dev_buad_rate with flow control disable\n"
                      " 4 change uart buad rate to normal_buad_rate with flow control enable\n");
}

STATIC ssize_t store_octty_test(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    int32 cmd;
    struct ps_core_s *ps_core_d = NULL;
    int32 result = -EINVAL;

    ps_print_info("%s\n", __func__);
    ps_get_core_reference(&ps_core_d);
    if (unlikely(ps_core_d == NULL)) {
        ps_print_err("ps_core_d is NULL\n");
        return -FAILURE;
    }

    if (buf == NULL) {
        ps_print_err("buf is NULL\n");
        return -FAILURE;
    }

    cmd = simple_strtol(buf, NULL, 10); /* ���ַ���ת����10������ */
    /* case x => echo x->���Խڵ��� */
    switch (cmd) {
        case 1:
            result = release_tty_drv(ps_core_d->pm_data);  // close tty
            break;
        case 2:
            result = open_tty_drv(ps_core_d->pm_data);  // open tty
            break;
        case 3:
            // BUAD RATE:115200, flow control: disable
            result = ps_change_uart_baud_rate(WKUP_DEV_BAUD_RATE, FLOW_CTRL_DISABLE);
            break;
        case 4:
            // BUAD_RATE:4000000, flow control:enable
            result = ps_change_uart_baud_rate(LOW_FREQ_BAUD_RATE, FLOW_CTRL_ENABLE);
        default:
            ps_print_err("unknown cmd %d\n", cmd);
            break;
    }

    if (result != 0) {
        ps_print_err("cmd[%d] test error\n", cmd);
    }

    return count;
}

STATIC ssize_t show_wifi_mem_dump(struct kobject *kobj, struct kobj_attribute *attr, int8 *buf)
{
    ps_print_info("%s\n", __func__);

    if (buf == NULL) {
        ps_print_err("buf is NULL\n");
        return -FAILURE;
    }

    return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "cmd         func             \n"
                      " 1    uart halt wcpu         \n"
                      " 2    uart read wifi pub reg \n"
                      " 3    uart read wifi priv reg\n"
                      " 4    uart read wifi mem     \n"
                      " 5    equal cmd 1+2+3+4      \n");
}

STATIC ssize_t store_wifi_mem_dump(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    int32 cmd;
    int32 ret;
    struct ps_core_s *ps_core_d = NULL;
    struct st_exception_info *pst_exception_data = NULL;

    ps_print_info("%s\n", __func__);

    if (buf == NULL) {
        ps_print_err("buf is NULL\n");
        return -FAILURE;
    }

    get_exception_info_reference(&pst_exception_data);
    if (pst_exception_data == NULL) {
        ps_print_err("get exception info reference is error\n");
        return 0;
    }

    ps_get_core_reference(&ps_core_d);
    if (unlikely(ps_core_d == NULL)) {
        ps_print_err("ps_core_d is NULL\n");
        return -EINVAL;
    }

    ret = prepare_to_visit_node(ps_core_d);
    if (ret < 0) {
        ps_print_err("prepare work FAIL\n");
        return ret;
    }

    cmd = simple_strtol(buf, NULL, 10); /* ���ַ���ת����10������ */
    /* case x => echo x->���Խڵ��� */
    switch (cmd) {
        case 1:
            ps_print_info("wifi mem dump cmd %d, halt wcpu\n", cmd);
            uart_halt_wcpu();
            break;
        case 2:
            ps_print_info("wifi mem dump cmd %d, read wifi public register\n", cmd);
            if (uart_read_wifi_mem(WIFI_PUB_REG) == EXCEPTION_SUCCESS) {
                /* send cmd to oam_hisi to rotate file */
            } else {
            };
            break;
        case 3:
            ps_print_info("wifi mem dump cmd %d, read wifi priv register\n", cmd);
            if (uart_read_wifi_mem(WIFI_PRIV_REG) == EXCEPTION_SUCCESS) {
                /* send cmd to oam_hisi to rotate file */
            } else {
            };
            break;
        case 4:
            ps_print_info("wifi mem dump cmd %d, read wifi mem\n", cmd);
            if (uart_read_wifi_mem(WIFI_MEM) == EXCEPTION_SUCCESS) {
                /* send cmd to oam_hisi to rotate file */
            } else {
            };
            break;
        case 5:
            ps_print_info("wifi mem dump cmd %d\n", cmd);
            debug_uart_read_wifi_mem(1);
            break;
        default:
            ps_print_err("error cmd:[%d]\n", cmd);
            break;
    }

    post_to_visit_node(ps_core_d);

    return count;
}

/*
 * �� �� ��  : show_bfgx_dump
 * ��������  : ��ʾSDIO�ϱ�BFGX��reg
 * �������  : kobj ptr, attr ptr, buf ptr
 */
STATIC ssize_t show_bfgx_dump(struct kobject *kobj, struct kobj_attribute *attr, int8 *buf)
{
    ps_print_info("%s\n", __func__);

    if (buf == NULL) {
        ps_print_err("buf is NULL\n");
        return -FAILURE;
    }

    return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "cmd           func            \n"
                      " 1    sdio read bcpu pub reg  \n"
                      " 2    sdio read bcpu priv reg \n"
                      " 3    sdio read bcpu mem      \n"
                      " 4    equal cmd 1+2+3         \n");
}

/*
 * �� �� ��  : show_coeff_dump
 * ��������  : ��ʾ��ʱ�ӵ�����ʽϵ��
 * �������  : kobj ptr, attr ptr, buf ptr
 */
STATIC ssize_t show_coeff_dump(struct kobject *kobj, struct kobj_attribute *attr, int8 *buf)
{
    dcxo_dl_para_stru *pst_dl_para = NULL;

    ps_print_info("[dcxo]%s\n", __func__);

    if (buf == NULL) {
        ps_print_err("[dcxo] buf is NULL\n");
        return -FAILURE;
    }

    if (get_tcxo_dcxo_mode() == INI_TCXO_MODE) {
        return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "use tcxo mode, no coeff\n");
    }

    pst_dl_para = (dcxo_dl_para_stru *)get_dcxo_data_buf_addr();
    if (pst_dl_para == NULL) {
        ps_print_err("[dcxo] %s pst_para is NULL\n", __func__);
        return INI_FAILED;
    }

    return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "dcxo coeff list:                \n"
                      "a0             = %d hex(0x%x)   \n"
                      "a1             = %d hex(0x%x)   \n"
                      "a2             = %d hex(0x%x)   \n"
                      "a3             = %d hex(0x%x)   \n"
                      "t0             = %d hex(0x%x)   \n"
                      "dcxo_tcxo_flag = %d             \n"
                      "check_data     = 0x%x           \n",
                      pst_dl_para->coeff_para[COEFF_A0], pst_dl_para->coeff_para[COEFF_A0],
                      pst_dl_para->coeff_para[COEFF_A1], pst_dl_para->coeff_para[COEFF_A1],
                      pst_dl_para->coeff_para[COEFF_A2], pst_dl_para->coeff_para[COEFF_A2],
                      pst_dl_para->coeff_para[COEFF_A3], pst_dl_para->coeff_para[COEFF_A3],
                      pst_dl_para->coeff_para[COEFF_T0], pst_dl_para->coeff_para[COEFF_T0],
                      pst_dl_para->tcxo_dcxo_flag, pst_dl_para->check_data);
}

/*
 * �� �� ��  : store_bfgx_reg_and_reg_dump
 * ��������  : SDIO�ϱ�BFGX��reg
 * �������  : kobj ptr, attr ptr, buf ptr�� count
 */
STATIC ssize_t store_bfgx_reg_and_reg_dump(struct kobject *kobj, struct kobj_attribute *attr,
                                           const char *buf, size_t count)
{
    int32 cmd;
    int32 ret;
    struct ps_core_s *ps_core_d = NULL;
    struct st_exception_info *pst_exception_data = NULL;

    ps_print_info("%s\n", __func__);

    if (buf == NULL) {
        ps_print_err("buf is NULL\n");
        return -FAILURE;
    }

    get_exception_info_reference(&pst_exception_data);
    if (pst_exception_data == NULL) {
        ps_print_err("get exception info reference is error\n");
        return 0;
    }

    ps_get_core_reference(&ps_core_d);
    if (unlikely(ps_core_d == NULL)) {
        ps_print_err("ps_core_d is NULL\n");
        return -EINVAL;
    }

    ret = prepare_to_visit_node(ps_core_d);
    if (ret < 0) {
        ps_print_err("prepare work FAIL\n");
        return ret;
    }

    cmd = simple_strtol(buf, NULL, 10); /* ���ַ���ת����10������ */
    /* case x => echo x->���Խڵ��� */
    switch (cmd) {
        case 1:
            ps_print_info("bfgx mem dump cmd %d,sdio read bcpu pub reg\n", cmd);
            debug_sdio_read_bfgx_reg_and_mem(BFGX_PUB_REG);
            break;
        case 2:
            ps_print_info("bfgx mem dump cmd %d, sdio read bcpu priv reg\n", cmd);
            debug_sdio_read_bfgx_reg_and_mem(BFGX_PRIV_REG);
            break;
        case 3:
            ps_print_info("bfgx mem dump cmd %d, sdio read bcpu mem\n", cmd);
            debug_sdio_read_bfgx_reg_and_mem(BFGX_MEM);
            break;
        case 4:
            ps_print_info("bfgx mem dump cmd %d, sdio read bcpu reg and mem\n", cmd);
            debug_sdio_read_bfgx_reg_and_mem(SDIO_BFGX_MEM_DUMP_BOTTOM);
            break;
        default:
            ps_print_err("error cmd:[%d]\n", cmd);
            break;
    }

    post_to_visit_node(ps_core_d);

    return count;
}

#ifdef BFGX_UART_DOWNLOAD_SUPPORT
STATIC ssize_t show_bfgx_uart_download(struct kobject *kobj, struct kobj_attribute *attr, int8 *buf)
{
    ps_print_info("%s\n", __func__);

    if (buf == NULL) {
        ps_print_err("buf is NULL\n");
        return -FAILURE;
    }

    return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1,
                      "baud:%-14s file_len:%-10d send_len:%-10d status:%-5d speed(byte/ms):%-6d usedtime(ms):%-6d \n",
                      uart_download_test.baud, uart_download_test.file_len,
                      uart_download_test.xmodern_len, uart_download_test.send_status,
                      uart_download_test.file_len / uart_download_test.used_time,
                      uart_download_test.used_time);
    return -FAILURE;
}

STATIC ssize_t store_bfgx_uart_download(struct kobject *kobj, struct kobj_attribute *attr,
                                        const char *buf, size_t count)
{
#define BFGX_UART_TEST_CMD_LEN 100
    uint8 baud[BFGX_UART_TEST_CMD_LEN] = {0};
    uint8 buf_data[BFGX_UART_TEST_CMD_LEN];
    int32 i;
    int32 buf_len = strlen(buf);
    const char *tmp_buf = buf;
    uint32 file_len;
    uint32 max_index;
    ps_print_info("%s\n", __func__);
    ps_print_dbg("buf:%s,len:%d,count:%d\n", buf, buf_len, (int32)count);
    while (*tmp_buf == ' ') {
        tmp_buf++;
        buf_len--;
        if (!buf_len) {
            return count;
        }
    };
    i = 0;
    while (*tmp_buf != ' ') {
        if (i < BFGX_UART_TEST_CMD_LEN) {
            baud[i++] = *tmp_buf;
        }
        tmp_buf++;
        buf_len--;
        if (!buf_len) {
            return count;
        }
    };
    max_index = i < BFGX_UART_TEST_CMD_LEN ? i : (BFGX_UART_TEST_CMD_LEN - 1);
    baud[max_index] = '\0';
    i = 0;
    ps_print_dbg("@#baud:%s,tmp_buf:%s\n", baud, tmp_buf);
    while (*tmp_buf == ' ') {
        tmp_buf++;
        buf_len--;
        if (!buf_len) {
            return count;
        }
    };
    while (((*tmp_buf >= '0') && (*tmp_buf <= '9')) || ((*tmp_buf >= 'a') && (*tmp_buf <= 'z'))) {
        if (!buf_len) {
            return count;
        }
        if (i < BFGX_UART_TEST_CMD_LEN) {
            buf_data[i++] = *tmp_buf;
        }
        tmp_buf++;
        buf_len--;
    };
    max_index = i < BFGX_UART_TEST_CMD_LEN ? i : (BFGX_UART_TEST_CMD_LEN - 1);
    buf_data[max_index] = '\0';
    file_len = simple_strtol(buf_data, NULL, 0);

    ps_print_info("baud:[%s],file_len[%d]", baud, file_len);
    uart_download_test(baud, file_len);
    return count;
}
#endif /* BFGX_UART_DOWNLOAD_SUPPORT */

#define WIFI_BOOT_TEST_DATA_BUFF_LEN 1024
static uint32 wifi_boot_file_len = 0;
static uint32 wifi_boot_total = 0;
static uint32 wifi_power_on_succ = 0;
static uint32 wifi_power_on_fail = 0;
static uint32 wifi_power_off_succ = 0;
static uint32 wifi_power_off_fail = 0;

STATIC ssize_t show_wifi_download(struct kobject *kobj, struct kobj_attribute *attr, int8 *buf)
{
    ps_print_info("%s\n", __func__);
    if (buf == NULL) {
        ps_print_err("buf is NULL\n");
        return -FAILURE;
    }

    return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1,
                      "total:%-10d file_len:%-10d on_succ:%-10d on_fail:%-10d off_succ:%-10d off_fail:%-10d\n",
                      wifi_boot_total, wifi_boot_file_len,
                      wifi_power_on_succ, wifi_power_on_fail,
                      wifi_power_off_succ, wifi_power_off_fail);
    ;
}

void wifi_boot_download_test(uint32 ul_test_len)
{
    mm_segment_t fs = {0};
    OS_KERNEL_FILE_STRU *fp = {0};
    int8 write_data[WIFI_BOOT_TEST_DATA_BUFF_LEN];
    int32 write_total_circle = ul_test_len / WIFI_BOOT_TEST_DATA_BUFF_LEN;
    int32 write_data_extra_len = ul_test_len % WIFI_BOOT_TEST_DATA_BUFF_LEN;
    int32 write_count = 0;
    const char *filename = "/system/vendor/firmware/test_wifi_boot";
    // init
    for (write_count = 0; write_count < WIFI_BOOT_TEST_DATA_BUFF_LEN; write_count++) {
        write_data[write_count] = write_count;
    }

    fs = get_fs();
    set_fs(KERNEL_DS);
    // create file
    fp = filp_open(filename, O_RDWR | O_CREAT, 0664);
    if (OAL_IS_ERR_OR_NULL(fp)) {
        set_fs(fs);
        ps_print_err("create file error,fp = 0x%p\n", fp);
        return;
    }
    if (write_total_circle != 0) {
        for (write_count = 0; write_count < write_total_circle; write_count++) {
            vfs_write(fp, write_data, WIFI_BOOT_TEST_DATA_BUFF_LEN, &fp->f_pos);
        }
    }
    if (write_data_extra_len != 0) {
        vfs_write(fp, write_data, write_data_extra_len, &fp->f_pos);
    }
    set_fs(fs);
    filp_close(fp, NULL);
    ps_print_info("#@file:%s prepare succ", filename);
}

STATIC ssize_t store_wifi_download(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
#define WIFI_DOWNLOAD_TEST_CMD_LEN 50
    uint8 buf_data[WIFI_DOWNLOAD_TEST_CMD_LEN];
    int32 i;
    int32 buf_len = strlen(buf);
    const char *tmp_buf = buf;
    uint32 file_len;
    uint32 max_index;
    ps_print_info("%s\n", __func__);
    ps_print_dbg("buf:%s,len:%d,count:%d\n", buf, buf_len, (int32)count);

    // get params
    while (*tmp_buf == ' ') {
        tmp_buf++;
        buf_len--;
        if (!buf_len) {
            return count;
        }
    };
    i = 0;
    while (((*tmp_buf >= '0') && (*tmp_buf <= '9')) || ((*tmp_buf >= 'a') && (*tmp_buf <= 'z'))) {
        if (!buf_len) {
            return count;
        }
        if (i < WIFI_DOWNLOAD_TEST_CMD_LEN) {
            buf_data[i++] = *tmp_buf;
        }
        tmp_buf++;
        buf_len--;
    };
    max_index = i < WIFI_DOWNLOAD_TEST_CMD_LEN ? i : (WIFI_DOWNLOAD_TEST_CMD_LEN - 1);
    buf_data[max_index] = '\0';
    file_len = simple_strtol(buf_data, NULL, 0);
    ps_print_info("#@get file len:%d prepare succ", file_len);

    // do download test and set flag
    wifi_boot_file_len = file_len;
    wifi_boot_download_test(file_len);
    wifi_boot_total++;
    if (hi1103_wlan_power_on() == 0) {
        wifi_power_on_succ++;
        ps_print_info("#@power on succ\n");
    } else {
        wifi_power_on_fail++;
        ps_print_info("#@power on fail\n");
    };
    if (hi1103_wlan_power_off() == 0) {
        wifi_power_off_succ++;
        ps_print_info("#@power off succ\n");
    } else {
        wifi_power_off_fail++;
        ps_print_info("#@power on fail\n");
    };
    return count;
}

#ifdef _PRE_CONFIG_GPIO_TO_SSI_DEBUG
STATIC ssize_t show_ssi_test(struct kobject *kobj, struct kobj_attribute *attr, int8 *buf)
{
    ps_print_info("%s\n", __func__);

    if (buf == NULL) {
        ps_print_err("buf is NULL\n");
        return -FAILURE;
    }
    if (g_ssi_test_st.used_time == 0) {
        ps_print_err("used_time==0\n");
        return -FAILURE;
    }
    return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "len:%-14d time:%-14d status:%-5d speed(byte/ms):%-8d\n",
                      g_ssi_test_st.trans_len, g_ssi_test_st.used_time, g_ssi_test_st.send_status,
                      g_ssi_test_st.trans_len / g_ssi_test_st.used_time);
}

STATIC ssize_t store_ssi_test(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
#define SSI_TEST_CMD_MAX_LEN 50
    int32 cmd;
    struct ps_core_s *ps_core_d = NULL;
    char s_addr[SSI_TEST_CMD_MAX_LEN];
    int32 dsc_addr;
    char s_data[SSI_TEST_CMD_MAX_LEN];
    int32 set_data;
    const char *tmp_buf = buf;
    size_t buf_len = count;
    int32 i = 0;
    int32 max_index;
    ps_print_info("%s\n", __func__);

    if (buf == NULL) {
        ps_print_err("buf is NULL\n");
        return -FAILURE;
    }

    ps_get_core_reference(&ps_core_d);
    if (unlikely(ps_core_d == NULL)) {
        ps_print_err("ps_core_d is NULL\n");
        return -EINVAL;
    }

    cmd = simple_strtol(buf, NULL, 10); /* ���ַ���ת����10������ */
    memset_s(&g_ssi_test_st, sizeof(g_ssi_test_st), 0, sizeof(g_ssi_test_st));
    /* case x => echo x->���Խڵ��� */
    switch (cmd) {
        case 1:
            ps_print_info("ssi download test cmd %d\n", cmd);
            g_ssi_test_st.test_type = SSI_MEM_TEST;
            ssi_download_test(&g_ssi_test_st);
            break;
        case 2:
            ps_print_info("ssi download test cmd %d\n", cmd);
            g_ssi_test_st.test_type = SSI_FILE_TEST;
            ssi_download_test(&g_ssi_test_st);
            break;
        case 3:
            ps_print_info("power on enable cmd %d\n", cmd);
            hi1103_chip_power_on();
            hi1103_bfgx_enable();
            hi1103_wifi_enable();
            break;
        case 4:
            ps_print_info("power off enable cmd %d\n", cmd);
            hi1103_bfgx_disable();
            hi1103_wifi_disable();
            hi1103_chip_power_off();
            break;
        case 5:
            ps_print_info("hard ware cmd %d\n", cmd);
            test_hd_ssi_write();
            break;
        default:
            ps_print_dbg("default cmd %s\n", buf);
            tmp_buf++;
            while (*tmp_buf == ' ') {
                tmp_buf++;
                buf_len--;
                if (!buf_len) {
                    return count;
                }
            };
            i = 0;
            while (((*tmp_buf >= '0') && (*tmp_buf <= '9')) || ((*tmp_buf >= 'a') && (*tmp_buf <= 'z'))) {
                if (i < SSI_TEST_CMD_MAX_LEN) {
                    s_addr[i++] = *tmp_buf;
                }
                tmp_buf++;
                buf_len--;
                if (!buf_len) {
                    return count;
                }
            };
            max_index = i < SSI_TEST_CMD_MAX_LEN ? i : (SSI_TEST_CMD_MAX_LEN - 1);
            s_addr[max_index] = '\0';
            i = 0;
            dsc_addr = simple_strtol(s_addr, NULL, 0);
            switch (buf[0]) {
                case 'r':
                    ps_print_info("ssi read: 0x%x=0x%x\n", dsc_addr, ssi_single_read(dsc_addr));
                    break;
                case 'w':
                    while (*tmp_buf == ' ') {
                        tmp_buf++;
                        buf_len--;
                        if (!buf_len) {
                            return count;
                        }
                    };
                    while (((*tmp_buf >= '0') && (*tmp_buf <= '9')) || ((*tmp_buf >= 'a') && (*tmp_buf <= 'z'))) {
                        if (!buf_len) {
                            return count;
                        }
                        if (i < SSI_TEST_CMD_MAX_LEN) {
                            s_data[i++] = *tmp_buf;
                        }
                        tmp_buf++;
                        buf_len--;
                    };
                    max_index = i < SSI_TEST_CMD_MAX_LEN ? i : (SSI_TEST_CMD_MAX_LEN - 1);
                    s_data[max_index] = '\0';
                    set_data = simple_strtol(s_data, NULL, 0);
                    ps_print_info("ssi_write s_addr:0x%x,s_data:0x%x\n", dsc_addr, set_data);
                    if (ssi_single_write(dsc_addr, set_data) != 0) {
                        ps_print_err("ssi write fail s_addr:0x%x s_data:0x%x\n", dsc_addr, set_data);
                    }
                    break;
                default:
                    ps_print_info("no suit cmd:%c\n", buf[0]);
                    break;
            }
            return count;
            ps_print_err("error cmd:[%d]\n", cmd);
            break;
    }

    return count;
}
#endif

STATIC struct kobj_attribute plat_exception_dbg =
    __ATTR(exception_dbg, 0644, (void *)show_exception_dbg, (void *)store_exception_dbg);

STATIC struct kobj_attribute uart_dumpctrl =
    __ATTR(uart_rx_dump, 0664, (void *)show_uart_rx_dump, (void *)store_uart_rx_dump);

STATIC struct kobj_attribute bfgx_dev_test =
    __ATTR(bfgx_test, 0664, (void *)show_dev_test, (void *)store_dev_test);

STATIC struct kobj_attribute octty_pre_test =
    __ATTR(octty_test, 0664, (void *)show_octty_test, (void *)store_octty_test);

STATIC struct kobj_attribute wifi_mem_dump =
    __ATTR(wifi_mem, 0664, (void *)show_wifi_mem_dump, (void *)store_wifi_mem_dump);

STATIC struct kobj_attribute bfgx_mem_and_reg_dump =
    __ATTR(bfgx_dump, 0664, (void *)show_bfgx_dump, (void *)store_bfgx_reg_and_reg_dump);

STATIC struct kobj_attribute dcxo_coeff_dump =
    __ATTR(dcxo_dump, 0664, (void *)show_coeff_dump, NULL);

STATIC struct kobj_attribute wifi_boot_test =
    __ATTR(wifi_boot, 0664, (void *)show_wifi_download, (void *)store_wifi_download);

#ifdef BFGX_UART_DOWNLOAD_SUPPORT
STATIC struct kobj_attribute bfgx_uart_download =
    __ATTR(bfgx_boot, 0664, (void *)show_bfgx_uart_download, (void *)store_bfgx_uart_download);
#endif /* BFGX_UART_DOWNLOAD_SUPPORT */

#ifdef _PRE_CONFIG_GPIO_TO_SSI_DEBUG
STATIC struct kobj_attribute ssi_test_trans =
    __ATTR(ssi_test, 0664, (void *)show_ssi_test, (void *)store_ssi_test);
#endif

STATIC struct attribute *hi110x_debug_attrs[] = {
    &plat_exception_dbg.attr,
    &uart_dumpctrl.attr,
    &bfgx_dev_test.attr,
    &octty_pre_test.attr,
    &wifi_mem_dump.attr,
    &bfgx_mem_and_reg_dump.attr,
    &dcxo_coeff_dump.attr,
    &wifi_boot_test.attr,
#ifdef BFGX_UART_DOWNLOAD_SUPPORT
    &bfgx_uart_download.attr,
#endif
#ifdef _PRE_CONFIG_GPIO_TO_SSI_DEBUG
    &ssi_test_trans.attr,
#endif
    NULL,
};

STATIC struct attribute_group hi110x_debug_attr_grp = {
    .attrs = hi110x_debug_attrs,
};
#endif

#ifdef HAVE_HISI_NFC
STATIC ssize_t show_hisi_nfc_conf_name(struct kobject *kobj, struct kobj_attribute *attr, int8 *buf)
{
    char hisi_nfc_conf_name[BUFF_LEN] = {0};
    int32 ret;

    if (buf == NULL) {
        ps_print_err("buf is NULL\n");
        return -FAILURE;
    }

    ret = read_nfc_conf_name_from_dts(hisi_nfc_conf_name, sizeof(hisi_nfc_conf_name),
                                      DTS_COMP_HISI_NFC_NAME, DTS_COMP_HW_HISI_NFC_CONFIG_NAME);
    if (ret < 0) {
        ps_print_err("read_nfc_conf_name_from_dts %s,ret = %d\n", DTS_COMP_HW_HISI_NFC_CONFIG_NAME, ret);
        return ret;
    }

    return snprintf_s(buf, PAGE_SIZE, sizeof(hisi_nfc_conf_name), "%s", hisi_nfc_conf_name);
}

STATIC ssize_t show_brcm_nfc_conf_name(struct kobject *kobj, struct kobj_attribute *attr, int8 *buf)
{
    char brcm_nfc_conf_name[BUFF_LEN] = {0};
    int32 ret;

    if (buf == NULL) {
        ps_print_err("buf is NULL\n");
        return -FAILURE;
    }

    ret = read_nfc_conf_name_from_dts(brcm_nfc_conf_name, sizeof(brcm_nfc_conf_name),
                                      DTS_COMP_HISI_NFC_NAME, DTS_COMP_HW_BRCM_NFC_CONFIG_NAME);
    if (ret < 0) {
        ps_print_err("read_nfc_conf_name_from_dts %s,ret = %d\n", DTS_COMP_HW_BRCM_NFC_CONFIG_NAME, ret);
        return ret;
    }

    return snprintf_s(buf, PAGE_SIZE, sizeof(brcm_nfc_conf_name), "%s", brcm_nfc_conf_name);
}

STATIC struct kobj_attribute hisi_nfc_conf =
    __ATTR(nxp_config_name, 0444, (void *)show_hisi_nfc_conf_name, NULL);

STATIC struct kobj_attribute brcm_nfc_conf =
    __ATTR(nfc_brcm_conf_name, 0444, (void *)show_brcm_nfc_conf_name, NULL);

STATIC struct attribute *hisi_nfc_attrs[] = {
    &hisi_nfc_conf.attr,
    &brcm_nfc_conf.attr,
    NULL,
};

STATIC struct attribute_group hisi_nfc_attr_grp = {
    .attrs = hisi_nfc_attrs,
};
#endif

int32 bfgx_user_ctrl_init(void)
{
    int status;
    struct kobject *pst_root_object = NULL;

    pst_root_object = oal_get_sysfs_root_object();
    if (pst_root_object == NULL) {
        ps_print_err("[E]get root sysfs object failed!\n");
        return -EFAULT;
    }

    sysfs_hisi_pmdbg = kobject_create_and_add("pmdbg", pst_root_object);
    if (sysfs_hisi_pmdbg == NULL) {
        ps_print_err("Failed to creat sysfs_hisi_pmdbg !!!\n ");
        goto fail_g_sysfs_hisi_pmdbg;
    }

    status = oal_debug_sysfs_create_group(sysfs_hisi_pmdbg, &pmdbg_attr_grp);
    if (status) {
        ps_print_err("failed to create sysfs_hisi_pmdbg sysfs entries\n");
        goto fail_create_pmdbg_group;
    }

    sysfs_hi110x_bfgx = kobject_create_and_add("hi110x_ps", NULL);
    if (sysfs_hi110x_bfgx == NULL) {
        ps_print_err("Failed to creat sysfs_hi110x_ps !!!\n ");
        goto fail_g_sysfs_hi110x_bfgx;
    }

    status = oal_sysfs_create_group(sysfs_hi110x_bfgx, &bfgx_attr_grp);
    if (status) {
        ps_print_err("failed to create sysfs_hi110x_bfgx sysfs entries\n");
        goto fail_create_bfgx_group;
    }

#ifdef PLATFORM_DEBUG_ENABLE
    sysfs_hi110x_debug = kobject_create_and_add("hi110x_debug", NULL);
    if (sysfs_hi110x_debug == NULL) {
        ps_print_err("Failed to creat sysfs_hi110x_debug !!!\n ");
        goto fail_g_sysfs_hi110x_debug;
    }

    status = oal_debug_sysfs_create_group(sysfs_hi110x_debug, &hi110x_debug_attr_grp);
    if (status) {
        ps_print_err("failed to create sysfs_hi110x_debug sysfs entries\n");
        goto fail_create_hi110x_debug_group;
    }
#endif

#ifdef HAVE_HISI_NFC
    if (!is_my_nfc_chip()) {
        ps_print_err("cfg dev board nfc chip type is not match, skip driver init\n");
    } else {
        ps_print_info("cfg dev board nfc type is matched with hisi_nfc, continue\n");
        sysfs_hisi_nfc = kobject_create_and_add("nfc", NULL);
        if (sysfs_hisi_nfc == NULL) {
            ps_print_err("Failed to creat sysfs_hisi_nfc !!!\n ");
            goto fail_g_sysfs_hisi_nfc;
        }

        status = oal_debug_sysfs_create_group(sysfs_hisi_nfc, &hisi_nfc_attr_grp);
        if (status) {
            ps_print_err("failed to create sysfs_hisi_nfc sysfs entries\n");
            goto fail_create_hisi_nfc_group;
        }
    }
#endif

    return 0;

#ifdef HAVE_HISI_NFC
fail_create_hisi_nfc_group:
    kobject_put(sysfs_hisi_nfc);
fail_g_sysfs_hisi_nfc:
#ifdef PLATFORM_DEBUG_ENABLE
    oal_debug_sysfs_remove_group(sysfs_hi110x_debug, &hi110x_debug_attr_grp);
#endif
#endif
#ifdef PLATFORM_DEBUG_ENABLE
fail_create_hi110x_debug_group:
    kobject_put(sysfs_hi110x_debug);
fail_g_sysfs_hi110x_debug:
#endif
    oal_sysfs_remove_group(sysfs_hi110x_bfgx, &bfgx_attr_grp);
fail_create_bfgx_group:
    kobject_put(sysfs_hi110x_bfgx);
fail_g_sysfs_hi110x_bfgx:
    oal_debug_sysfs_remove_group(sysfs_hisi_pmdbg, &pmdbg_attr_grp);
fail_create_pmdbg_group:
    kobject_put(sysfs_hisi_pmdbg);
fail_g_sysfs_hisi_pmdbg:
    return -EFAULT;
}

void bfgx_user_ctrl_exit(void)
{
#ifdef HAVE_HISI_NFC
    if (is_my_nfc_chip()) {
        oal_debug_sysfs_remove_group(sysfs_hisi_nfc, &hisi_nfc_attr_grp);
        kobject_put(sysfs_hisi_nfc);
    }
#endif

#ifdef PLATFORM_DEBUG_ENABLE
    oal_debug_sysfs_remove_group(sysfs_hi110x_debug, &hi110x_debug_attr_grp);
    kobject_put(sysfs_hi110x_debug);
#endif

    oal_sysfs_remove_group(sysfs_hi110x_bfgx, &bfgx_attr_grp);
    kobject_put(sysfs_hi110x_bfgx);

    oal_debug_sysfs_remove_group(sysfs_hisi_pmdbg, &pmdbg_attr_grp);
    kobject_put(sysfs_hisi_pmdbg);
}
