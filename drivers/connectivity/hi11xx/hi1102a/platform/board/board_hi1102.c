

/* 1 Header File Including */
#include "board_hi1102.h"


#ifdef _PRE_CONFIG_USE_DTS
#include <linux/of.h>
#include <linux/of_gpio.h>
#endif
/*lint -e322*/ /*lint -e7*/
#include <linux/clk.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/timer.h>
#include <linux/delay.h>
#ifdef CONFIG_PINCTRL
#include <linux/pinctrl/consumer.h>
#endif
/*lint +e322*/ /*lint +e7*/
#include "board.h"
#include "plat_debug.h"
#include "plat_pm.h"
#include "oal_hcc_host_if.h"
#include "plat_uart.h"
#include "plat_firmware.h"
#include "securec.h"

/* 3 Function Definition */
int32 hi1102_get_board_power_gpio(void)
{
    int32 ret;
    int32 physical_gpio = 0;
    ret = get_board_gpio(DTS_NODE_HI110X, DTS_PROP_GPIO_POWEN_ON, &physical_gpio);
    if (ret != BOARD_SUCC) {
        ps_print_err("get dts prop %s failed\n", DTS_PROP_GPIO_POWEN_ON);
        return BOARD_FAIL;
    }

    g_board_info.power_on_enable = physical_gpio;
#ifdef GPIOF_OUT_INIT_LOW
    ret = gpio_request_one(physical_gpio, GPIOF_OUT_INIT_LOW, PROC_NAME_GPIO_POWEN_ON);
    if (ret) {
        ps_print_err("%s gpio_request failed\n", PROC_NAME_GPIO_POWEN_ON);
        return BOARD_FAIL;
    }
#else
    ret = gpio_request(physical_gpio, PROC_NAME_GPIO_POWEN_ON);
    if (ret) {
        ps_print_err("%s gpio_request failed\n", PROC_NAME_GPIO_POWEN_ON);
        return BOARD_FAIL;
    }

    gpio_direction_output(physical_gpio, 0);
#endif
    return BOARD_SUCC;
}

void hi1102_free_board_power_gpio(void)
{
    gpio_free(g_board_info.power_on_enable);
}

int32 hi1102_board_wakeup_gpio_init(void)
{
    int32 ret;
    int32 physical_gpio = 0;

    /* wifi wake host gpio request */
    ret = get_board_gpio(DTS_NODE_HI110X_WIFI, DTS_PROP_GPIO_WLAN_WAKEUP_HOST, &physical_gpio);
    if (ret != BOARD_SUCC) {
        ps_print_err("get dts prop %s failed\n", DTS_PROP_GPIO_WLAN_WAKEUP_HOST);
        goto err_get_wlan_wkup_host_gpio;
    }

    g_board_info.wlan_wakeup_host = physical_gpio;
#ifdef GPIOF_IN
    ret = gpio_request_one(physical_gpio, GPIOF_IN, PROC_NAME_GPIO_WLAN_WAKEUP_HOST);
    if (ret) {
        ps_print_err("%s gpio_request failed\n", PROC_NAME_GPIO_WLAN_WAKEUP_HOST);
        goto err_get_wlan_wkup_host_gpio;
    }
#else
    ret = gpio_request(physical_gpio, PROC_NAME_GPIO_WLAN_WAKEUP_HOST);
    if (ret) {
        ps_print_err("%s gpio_request failed\n", PROC_NAME_GPIO_WLAN_WAKEUP_HOST);
        goto err_get_wlan_wkup_host_gpio;
    }
    gpio_direction_input(physical_gpio);
#endif

    /* bfgx wake host gpio request */
    ret = get_board_gpio(DTS_NODE_HI110X_BFGX, DTS_PROP_GPIO_BFGN_WAKEUP_HOST, &physical_gpio);
    if (ret != BOARD_SUCC) {
        ps_print_err("get dts prop %s failed\n", DTS_PROP_GPIO_BFGN_WAKEUP_HOST);
        goto err_get_bfgx_wkup_host_gpio;
    }

    g_board_info.bfgn_wakeup_host = physical_gpio;
#ifdef GPIOF_IN
    ret = gpio_request_one(physical_gpio, GPIOF_IN, PROC_NAME_GPIO_BFGN_WAKEUP_HOST);
    if (ret) {
        ps_print_err("%s gpio_request failed\n", PROC_NAME_GPIO_BFGN_WAKEUP_HOST);
        goto err_get_bfgx_wkup_host_gpio;
    }
#else
    ret = gpio_request(physical_gpio, PROC_NAME_GPIO_BFGN_WAKEUP_HOST);
    if (ret) {
        ps_print_err("%s gpio_request failed\n", PROC_NAME_GPIO_BFGN_WAKEUP_HOST);
        goto err_get_bfgx_wkup_host_gpio;
    }
    gpio_direction_input(physical_gpio);
#endif
    return BOARD_SUCC;

err_get_bfgx_wkup_host_gpio:
    gpio_free(g_board_info.wlan_wakeup_host);
err_get_wlan_wkup_host_gpio:

    return BOARD_FAIL;
}

void hi1102_free_board_wakeup_gpio(void)
{
    gpio_free(g_board_info.wlan_wakeup_host);
    gpio_free(g_board_info.bfgn_wakeup_host);
}

static void hi1102_prepare_to_power_on(void)
{
    int32 ret;

    if (g_board_info.need_power_prepare == NO_NEED_POWER_PREPARE) {
        return;
    }

    if (OAL_IS_ERR_OR_NULL(g_board_info.pctrl) || OAL_IS_ERR_OR_NULL(g_board_info.pins_idle)) {
        ps_print_err("power pinctrl is err, pctrl is %p, pins_idle is %p\n",
                     g_board_info.pctrl, g_board_info.pins_idle);
        return;
    }
#ifdef _PRE_CONFIG_USE_DTS
    /* set LowerPower mode */
    ret = pinctrl_select_state(g_board_info.pctrl, g_board_info.pins_idle);
    if (ret != BOARD_SUCC) {
        ps_print_err("power prepare:set LOWPOWER mode failed, ret:%d\n", ret);
        return;
    }
#endif
#ifdef GPIOF_OUT_INIT_LOW
    ret = gpio_request_one(g_board_info.xldo_pinmux, GPIOF_OUT_INIT_LOW, PROC_NAME_GPIO_XLDO_PINMUX);
    if (ret) {
        ps_print_err("%s gpio_request failed\n", PROC_NAME_GPIO_XLDO_PINMUX);
        CHR_EXCEPTION(CHR_WIFI_DEV(CHR_WIFI_DEV_EVENT_CHIP, CHR_WIFI_DEV_ERROR_GPIO));
        return;
    }
#else
    ret = gpio_request(g_board_info.xldo_pinmux, PROC_NAME_GPIO_XLDO_PINMUX);
    if (ret) {
        ps_print_err("%s gpio_request failed\n", PROC_NAME_GPIO_XLDO_PINMUX);
        CHR_EXCEPTION(CHR_WIFI_DEV(CHR_WIFI_DEV_EVENT_CHIP, CHR_WIFI_DEV_ERROR_GPIO));
        return;
    }
    gpio_direction_output(g_board_info.xldo_pinmux, 0);
#endif

    gpio_direction_output(g_board_info.xldo_pinmux, g_board_info.gpio_xldo_level);
    g_board_info.pinmux_set_result = PINMUX_SET_SUCC;

    return;
}

static void hi1102_post_to_power_on(void)
{
    int32 ret;

    if (g_board_info.need_power_prepare == NO_NEED_POWER_PREPARE) {
        return;
    }

    if (g_board_info.pinmux_set_result == PINMUX_SET_SUCC) {
        g_board_info.pinmux_set_result = PINMUX_SET_INIT;
        gpio_free(g_board_info.xldo_pinmux);
    }

    if (OAL_IS_ERR_OR_NULL(g_board_info.pctrl) || OAL_IS_ERR_OR_NULL(g_board_info.pins_normal)) {
        ps_print_err("power pinctrl is err, pctrl is %p, pins_idle is %p\n",
                     g_board_info.pctrl, g_board_info.pins_normal);
        return;
    }
#ifdef _PRE_CONFIG_USE_DTS
    /* set NORMAL mode */
    ret = pinctrl_select_state(g_board_info.pctrl, g_board_info.pins_normal);
    if (ret != BOARD_SUCC) {
        ps_print_err("power prepare:set NORMAL mode failed, ret:%d\n", ret);
        return;
    }
#else
#warning DTS invalid
#endif

    return;
}

int32 hi1102_bfgx_dev_power_on(void)
{
    int32 error = BFGX_POWER_SUCCESS;
    struct ps_core_s *ps_core_d = NULL;
    struct pm_drv_data *pm_data = pm_get_drvdata();
    if (pm_data == NULL) {
        ps_print_err("pm_data is NULL!\n");
        return BFGX_POWER_FAILED;
    }

    ps_get_core_reference(&ps_core_d);
    if (unlikely(ps_core_d == NULL)) {
        ps_print_err("ps_core_d is err\n");
        return BFGX_POWER_FAILED;
    }

    if (wlan_is_shutdown()) {
        ps_print_info("bfgx pull up power_on_enable gpio!\n");

        hi1102_board_power_on(BFGX_POWER);
        if (open_tty_drv(ps_core_d->pm_data) != BFGX_POWER_SUCCESS) {
            ps_print_err("open tty fail!\n");
            error = BFGX_POWER_TTY_OPEN_FAIL;
            goto bfgx_power_on_fail;
        }

        if (firmware_download_function(BFGX_CFG) != BFGX_POWER_SUCCESS) {
            hcc_bus_disable_state(pm_data->pst_wlan_pm_info->pst_bus, OAL_BUS_STATE_ALL);
            ps_print_err("bfgx download firmware fail!\n");
            error = BFGX_POWER_DOWNLOAD_FIRMWARE_FAIL;
            goto bfgx_power_on_fail;
        }
        hcc_bus_disable_state(pm_data->pst_wlan_pm_info->pst_bus, OAL_BUS_STATE_ALL);
    } else {
        if (open_tty_drv(ps_core_d->pm_data) != BFGX_POWER_SUCCESS) {
            ps_print_err("open tty fail!\n");
            error = BFGX_POWER_TTY_OPEN_FAIL;
            goto bfgx_power_on_fail;
        }

        if (wlan_pm_open_bcpu() != BFGX_POWER_SUCCESS) {
            ps_print_err("wifi dereset bcpu fail!\n");
            error = BFGX_POWER_WIFI_DERESET_BCPU_FAIL;
            CHR_EXCEPTION(CHR_GNSS_DRV(CHR_GNSS_DRV_EVENT_PLAT, CHR_PLAT_DRV_ERROR_OPEN_BCPU));
            goto bfgx_power_on_fail;
        }
    }

bfgx_power_on_fail:

    return error;
}

int32 hi1102_bfgx_dev_power_off(void)
{
    int32 error = BFGX_POWER_SUCCESS;
    struct ps_core_s *ps_core_d = NULL;
    struct pm_drv_data *pm_data = pm_get_drvdata();
    if (pm_data == NULL) {
        ps_print_err("pm_data is NULL!\n");
        return BFGX_POWER_FAILED;
    }

    ps_get_core_reference(&ps_core_d);
    if (unlikely(ps_core_d == NULL)) {
        ps_print_err("ps_core_d is err\n");
        return BFGX_POWER_FAILED;
    }

    if (wlan_is_shutdown()) {
        if (release_tty_drv(ps_core_d->pm_data) != SUCCESS) {
            /* ����ִ�е��˴���˵������һ����ҵ���Ѿ��رգ�����tty�Ƿ�رճɹ���device��Ҫ�µ� */
            ps_print_err("wifi off, close tty is err!");
        }

        pm_data->bfgx_dev_state = BFGX_SLEEP;

        ps_print_info("bfgx pull down power_on_enable gpio\n");

        hi1102_board_power_off(BFGX_POWER);
    } else {
        if (uart_bfgx_close_cmd() != SUCCESS) {
            /* bfgx self close fail �ˣ�����ҲҪͨ��wifi shutdown bcpu */
            ps_print_err("bfgx self close fail\n");
            CHR_EXCEPTION(CHR_GNSS_DRV(CHR_GNSS_DRV_EVENT_PLAT, CHR_PLAT_DRV_ERROR_CLOSE_BCPU));
        }

        if (release_tty_drv(ps_core_d->pm_data) != SUCCESS) {
            /* ����ִ�е��˴���˵��bfgx����ҵ���Ѿ��رգ�����tty�Ƿ�رճɹ�����Ҫ�ر�bcpu */
            ps_print_err("wifi on, close tty is err!");
        }

        pm_data->bfgx_dev_state = BFGX_SLEEP;

        if (wlan_pm_shutdown_bcpu_cmd() != SUCCESS) {
            ps_print_err("wifi shutdown bcpu fail\n");
            CHR_EXCEPTION(CHR_GNSS_DRV(CHR_GNSS_DRV_EVENT_PLAT, CHR_PLAT_DRV_ERROR_CLOSE_BCPU));
            error = -FAILURE;
        }
    }

    return error;
}

int32 hi1102_wlan_power_off(void)
{
    struct pm_drv_data *pm_data = pm_get_drvdata();
    if (pm_data == NULL) {
        ps_print_err("pm_data is NULL!\n");
        return -FAILURE;
    }

    if (bfgx_is_shutdown()) {
        ps_print_info("wifi pull down power_on_enable!\n");
        hcc_bus_disable_state(hcc_get_current_110x_bus(), OAL_BUS_STATE_ALL);
        hi1102_board_power_off(WLAN_POWER);
        DECLARE_DFT_TRACE_KEY_INFO("wlan_poweroff_by_gpio", OAL_DFT_TRACE_SUCC);
    } else {
        /* �ȹر�SDIO TXͨ�� */
        hcc_bus_disable_state(hcc_get_current_110x_bus(), OAL_BUS_STATE_TX);

        /* wakeup dev,send poweroff cmd to wifi */
        if (wlan_pm_poweroff_cmd() != OAL_SUCC) {
            /* wifi self close ʧ����Ҳ��������ִ�У�uart�ر�WCPU���쳣�ָ��Ƴٵ�wifi�´�open��ʱ��ִ�� */
            DECLARE_DFT_TRACE_KEY_INFO("wlan_poweroff_by_sdio_fail", OAL_DFT_TRACE_FAIL);
            CHR_EXCEPTION(CHR_WIFI_DRV(CHR_WIFI_DRV_EVENT_PLAT, CHR_PLAT_DRV_ERROR_CLOSE_WCPU));
#ifdef PLATFORM_DEBUG_ENABLE
            return -FAILURE;
#endif
        }

        hcc_bus_disable_state(hcc_get_current_110x_bus(), OAL_BUS_STATE_ALL);

        /* power off cmd execute succ,send shutdown wifi cmd to BFGN */
        if (uart_wifi_close() != OAL_SUCC) {
            /* uart�ر�WCPUʧ��Ҳ����ִ�У�DFR�Ƴٵ�wifi�´�open��ʱ��ִ�� */
            DECLARE_DFT_TRACE_KEY_INFO("wlan_poweroff_uart_cmd_fail", OAL_DFT_TRACE_FAIL);
            CHR_EXCEPTION(CHR_WIFI_DRV(CHR_WIFI_DRV_EVENT_PLAT, CHR_PLAT_DRV_ERROR_CLOSE_WCPU));
        }
        DECLARE_DFT_TRACE_KEY_INFO("wlan_poweroff_by_bcpu_ok", OAL_DFT_TRACE_SUCC);
    }

    pm_data->pst_wlan_pm_info->ul_wlan_power_state = POWER_STATE_SHUTDOWN;

    return SUCCESS;
}

int32 hi1102_wlan_power_on(void)
{
    int32 ret;
    int32 error = WIFI_POWER_SUCCESS;
    struct pm_drv_data *pm_data = pm_get_drvdata();
    if (pm_data == NULL) {
        ps_print_err("pm_data is NULL!\n");
        return -FAILURE;
    }

    if (bfgx_is_shutdown()) {
        ps_print_suc("wifi pull up power_on_enable gpio!\n");
        hi1102_board_power_on(WLAN_POWER);
        DECLARE_DFT_TRACE_KEY_INFO("wlan_poweron_by_gpio_ok", OAL_DFT_TRACE_SUCC);

        hcc_bus_power_action(hcc_get_current_110x_bus(), HCC_BUS_POWER_PATCH_LOAD_PREPARE);

        if (firmware_download_function(BFGX_AND_WIFI_CFG) == WIFI_POWER_SUCCESS) {
            pm_data->pst_wlan_pm_info->ul_wlan_power_state = POWER_STATE_OPEN;
        } else {
            ps_print_err("firmware download fail\n");
            error = WIFI_POWER_BFGX_OFF_FIRMWARE_DOWNLOAD_FAIL;
            goto wifi_power_fail;
        }

        ret = hcc_bus_power_action(hcc_get_current_110x_bus(), HCC_BUS_POWER_PATCH_LAUCH);
        if (ret != 0) {
            DECLARE_DFT_TRACE_KEY_INFO("wlan_poweron HCC_BUS_POWER_PATCH_LAUCH by gpio fail", OAL_DFT_TRACE_FAIL);
            ps_print_err("wlan_poweron HCC_BUS_POWER_PATCH_LAUCH failed=%d\n", ret);
            error = WIFI_POWER_BFGX_OFF_BOOT_UP_FAIL;
            CHR_EXCEPTION(CHR_WIFI_DRV(CHR_WIFI_DRV_EVENT_PLAT, CHR_PLAT_DRV_ERROR_WCPU_BOOTUP));
            goto wifi_power_fail;
        }
    } else {
        if (uart_wifi_open() != WIFI_POWER_SUCCESS) {
            DECLARE_DFT_TRACE_KEY_INFO("wlan_poweron_by_uart_fail", OAL_DFT_TRACE_FAIL);
            error = WIFI_POWER_BFGX_DERESET_WCPU_FAIL;
            CHR_EXCEPTION(CHR_WIFI_DRV(CHR_WIFI_DRV_EVENT_PLAT, CHR_PLAT_DRV_ERROR_OPEN_WCPU));
            goto wifi_power_fail;
        } else {
            hcc_bus_power_action(hcc_get_current_110x_bus(), HCC_BUS_POWER_PATCH_LOAD_PREPARE);
            if (firmware_download_function(WIFI_CFG) == WIFI_POWER_SUCCESS) {
                pm_data->pst_wlan_pm_info->ul_wlan_power_state = POWER_STATE_OPEN;
            } else {
                ps_print_err("firmware download fail\n");
                error = WIFI_POWER_BFGX_ON_FIRMWARE_DOWNLOAD_FAIL;
                goto wifi_power_fail;
            }

            ret = hcc_bus_power_action(hcc_get_current_110x_bus(), HCC_BUS_POWER_PATCH_LAUCH);
            if (ret != 0) {
                DECLARE_DFT_TRACE_KEY_INFO("wlan_poweron HCC_BUS_POWER_PATCH_LAUCH byuart_fail", OAL_DFT_TRACE_FAIL);
                ps_print_err("wlan_poweron by uart HCC_BUS_POWER_PATCH_LAUCH failed =%d", ret);
                error = WIFI_POWER_BFGX_ON_BOOT_UP_FAIL;
                CHR_EXCEPTION(CHR_WIFI_DRV(CHR_WIFI_DRV_EVENT_PLAT, CHR_PLAT_DRV_ERROR_WCPU_BOOTUP));
                goto wifi_power_fail;
            }
        }
    }

    return WIFI_POWER_SUCCESS;

wifi_power_fail:
    return error;
}

int32 hi1102_board_power_on(uint32 ul_subsystem)
{
    int32 ret = -EFAIL;
    uintptr_t gpio = g_board_info.power_on_enable;

    if (ul_subsystem >= POWER_BUTT) {
        ps_print_err("power input system:%d error\n", ul_subsystem);
        return ret;
    }

    hi1102_prepare_to_power_on();

    /* ��һ��ö��ʱBUS ��δ��ʼ�� */
    ret = hcc_bus_power_ctrl_register(hcc_get_current_110x_bus(), HCC_BUS_CTRL_POWER_UP,
                                      board_wlan_gpio_power_on, (void *)gpio);
    hcc_bus_power_action(hcc_get_current_110x_bus(), HCC_BUS_POWER_UP);

    hi1102_post_to_power_on();

    return SUCC;
}

int32 hi1102_board_power_off(uint32 ul_subsystem)
{
    int32 ret;
    uintptr_t gpio = g_board_info.power_on_enable;

    if (ul_subsystem >= POWER_BUTT) {
        ps_print_err("power input system:%d error\n", ul_subsystem);
        return -EFAIL;
    }

    ret = hcc_bus_power_ctrl_register(hcc_get_current_110x_bus(), HCC_BUS_CTRL_POWER_DOWN,
                                      board_wlan_gpio_power_off, (void *)gpio);
    hcc_bus_power_action(hcc_get_current_110x_bus(), HCC_BUS_POWER_DOWN);

    return SUCC;
}

int32 hi1102_board_power_reset(uint32 ul_subsystem)
{
    hi1102_board_power_on(ul_subsystem);
    return SUCC;
}

int32 hi1102_get_board_pmu_clk32k(void)
{
    int32 ret;
    const char *clk_name = NULL;

    ret = get_board_custmize(DTS_NODE_HI110X, DTS_PROP_CLK_32K, &clk_name);
    if (ret != BOARD_SUCC) {
        return BOARD_FAIL;
    }
    g_board_info.clk_32k_name = clk_name;

    return BOARD_SUCC;
}

int32 hi1102_get_board_uart_port(void)
{
#ifdef _PRE_CONFIG_USE_DTS
    int32 ret;
    struct device_node *np = NULL;
    const char *uart_port = NULL;

    ret = get_board_dts_node(&np, DTS_NODE_HI110X_BFGX);
    if (ret != BOARD_SUCC) {
        ps_print_err("DTS read node %s fail!!!\n", DTS_NODE_HI110X);
        return BOARD_FAIL;
    }

    /* ʹ��uart4����Ҫ��dts������DTS_PROP_UART_PCLK�ָ��uart4������sensorhub */
    if (of_property_read_bool(np, DTS_PROP_UART_PCLK)) {
        ps_print_info("uart pclk normal\n");
        g_board_info.uart_pclk = UART_PCLK_NORMAL;
    } else {
        ps_print_info("uart pclk from sensorhub\n");
        g_board_info.uart_pclk = UART_PCLK_FROM_SENSORHUB;
    }

    ret = get_board_custmize(DTS_NODE_HI110X_BFGX, DTS_PROP_UART_POART, &uart_port);
    if (ret != BOARD_SUCC) {
        return BOARD_FAIL;
    }

    g_board_info.uart_port = uart_port;
    ps_print_info(" g_board_info.uart_port=%s\n", g_board_info.uart_port);
    return BOARD_SUCC;
#else
    return BOARD_SUCC;
#endif
}

#ifdef HAVE_HISI_IR
int32 hi1102_board_ir_ctrl_gpio_init(void)
{
    int32 ret;
    int32 physical_gpio = 0;

    /* ir ctrl gpio request */
    ret = get_board_gpio(DTS_NODE_HI110X_BFGX, DTS_PROP_GPIO_BFGN_IR_CTRL, &physical_gpio);
    if (ret != BOARD_SUCC) {
        ps_print_info("dts prop %s not exist\n", DTS_PROP_GPIO_BFGN_IR_CTRL);
        g_board_info.bfgx_ir_ctrl_gpio = -1;
    } else {
        g_board_info.bfgx_ir_ctrl_gpio = physical_gpio;

#ifdef GPIOF_OUT_INIT_LOW
        ret = gpio_request_one(physical_gpio, GPIOF_OUT_INIT_LOW, PROC_NAME_GPIO_BFGN_IR_CTRL);
        if (ret) {
            ps_print_err("%s gpio_request failed\n", PROC_NAME_GPIO_BFGN_IR_CTRL);
        }
#else
        ret = gpio_request(physical_gpio, PROC_NAME_GPIO_BFGN_IR_CTRL);
        if (ret) {
            ps_print_err("%s gpio_request failed\n", PROC_NAME_GPIO_BFGN_IR_CTRL);
        } else {
            gpio_direction_output(physical_gpio, 0);
        }
#endif
    }

    return BOARD_SUCC;
}

int32 hi1102_board_ir_ctrl_pmic_init(struct platform_device *pdev)
{
#ifdef _PRE_CONFIG_USE_DTS
    int32 ret = BOARD_FAIL;
    struct device_node *np = NULL;
    int32 irled_voltage = 0;
    if (pdev == NULL) {
        ps_print_err("board pmu pdev is NULL!\n");
        return ret;
    }

    ret = get_board_dts_node(&np, DTS_NODE_HI110X_BFGX);
    if (ret != BOARD_SUCC) {
        ps_print_err("DTS read node %s fail!!!\n", DTS_NODE_HI110X_BFGX);
        return ret;
    }

    g_board_info.bfgn_ir_ctrl_ldo = regulator_get(&pdev->dev, DTS_IRLED_LDO_POWER);

    if (IS_ERR(g_board_info.bfgn_ir_ctrl_ldo)) {
        ps_print_err("board_ir_ctrl_pmic_init get ird ldo failed\n");
        return ret;
    }

    ret = of_property_read_u32(np, DTS_IRLED_VOLTAGE, &irled_voltage);
    if (ret == BOARD_SUCC) {
        ps_print_info("set irled voltage %d mv\n", irled_voltage / 1000);
        ret = regulator_set_voltage(g_board_info.bfgn_ir_ctrl_ldo, (int)irled_voltage, (int)irled_voltage);
        if (ret) {
            ps_print_err("board_ir_ctrl_pmic_init set voltage ldo failed\n");
            return ret;
        }
    } else {
        ps_print_err("get irled voltage failed ,use default\n");
    }

    ret = regulator_set_mode(g_board_info.bfgn_ir_ctrl_ldo, REGULATOR_MODE_NORMAL);
    if (ret) {
        ps_print_err("board_ir_ctrl_pmic_init set ldo mode failed\n");
        return ret;
    }
    ps_print_info("board_ir_ctrl_pmic_init success\n");
    return BOARD_SUCC;
#else
    return BOARD_SUCC;
#endif
}
#endif

int32 hi1102_board_ir_ctrl_init(struct platform_device *pdev)
{
#if (defined HAVE_HISI_IR) && (defined _PRE_CONFIG_USE_DTS)
    int ret;
    struct device_node *np = NULL;

    ret = get_board_dts_node(&np, DTS_NODE_HI110X_BFGX);
    if (ret != BOARD_SUCC) {
        ps_print_err("DTS read node %s fail!!!\n", DTS_NODE_HI110X_BFGX);
        goto err_get_ir_ctrl_gpio;
    }
    g_board_info.have_ir = of_property_read_bool(np, "have_ir");
    if (!g_board_info.have_ir) {
        ps_print_err("board has no Ir");
    } else {
        ret = of_property_read_u32(np, DTS_PROP_IR_LDO_TYPE, &g_board_info.irled_power_type);
        ps_print_info("read property ret is %d, irled_power_type is %d\n", ret, g_board_info.irled_power_type);
        if (ret != BOARD_SUCC) {
            ps_print_err("get dts prop %s failed\n", DTS_PROP_IR_LDO_TYPE);
            goto err_get_ir_ctrl_gpio;
        }

        if (g_board_info.irled_power_type == IR_GPIO_CTRL) {
            ret = hi1102_board_ir_ctrl_gpio_init();
            if (ret != BOARD_SUCC) {
                ps_print_err("ir_ctrl_gpio init failed\n");
                goto err_get_ir_ctrl_gpio;
            }
        } else if (g_board_info.irled_power_type == IR_LDO_CTRL) {
            ret = hi1102_board_ir_ctrl_pmic_init(pdev);
            if (ret != BOARD_SUCC) {
                ps_print_err("ir_ctrl_pmic init failed\n");
                goto err_get_ir_ctrl_gpio;
            }
        } else {
            ps_print_err("get ir_ldo_type failed!err num is %d\n", g_board_info.irled_power_type);
            goto err_get_ir_ctrl_gpio;
        }
    }
    return BOARD_SUCC;

err_get_ir_ctrl_gpio:
    return BOARD_FAIL;
#else
    return BOARD_SUCC;
#endif
}

int32 hi1102_check_evb_or_fpga(void)
{
#ifdef _PRE_CONFIG_USE_DTS
    int32 ret;
    struct device_node *np = NULL;

    ret = get_board_dts_node(&np, DTS_NODE_HI110X);
    if (ret != BOARD_SUCC) {
        ps_print_err("DTS read node %s fail!!!\n", DTS_NODE_HI110X);
        return BOARD_FAIL;
    }

    ret = of_property_read_bool(np, DTS_PROP_VERSION);
    if (ret) {
        ps_print_info("HI1102 ASIC VERSION\n");
        g_board_info.is_asic = VERSION_ASIC;
    } else {
        ps_print_info("HI1102 FPGA VERSION\n");
        g_board_info.is_asic = VERSION_FPGA;
    }

    return BOARD_SUCC;
#else
    return BOARD_SUCC;
#endif
}

int32 hi1102_board_get_power_pinctrl(struct platform_device *pdev)
{
#ifdef _PRE_CONFIG_USE_DTS
    int32 ret;
    int32 physical_gpio = 0;
    struct device_node *np = NULL;
    struct pinctrl *pinctrl = NULL;
    struct pinctrl_state *pinctrl_def = NULL;
    struct pinctrl_state *pinctrl_idle = NULL;

    /* ����Ƿ���Ҫprepare before board power on */
    /* JTAG SELECT ���ͣ�XLDO MODEѡ��2.8v */
    ret = get_board_dts_node(&np, DTS_NODE_HI110X);
    if (ret != BOARD_SUCC) {
        ps_print_err("DTS read node %s fail!!!\n", DTS_NODE_HI110X);
        goto err_read_dts_node;
    }

    ret = of_property_read_bool(np, DTS_PROP_POWER_PREPARE);
    if (ret) {
        ps_print_info("need prepare before board power on\n");
        g_board_info.need_power_prepare = NEED_POWER_PREPARE;
    } else {
        ps_print_info("no need prepare before board power on\n");
        g_board_info.need_power_prepare = NO_NEED_POWER_PREPARE;
    }

    if (g_board_info.need_power_prepare == NO_NEED_POWER_PREPARE) {
        return BOARD_SUCC;
    }

    pinctrl = devm_pinctrl_get(&pdev->dev);
    if (OAL_IS_ERR_OR_NULL(pinctrl)) {
        ps_print_err("iomux_lookup_block failed, and the value of pinctrl is %p\n", pinctrl);
        CHR_EXCEPTION(CHR_WIFI_DEV(CHR_WIFI_DEV_EVENT_CHIP, CHR_WIFI_DEV_ERROR_IOMUX));
        goto err_pinctrl_get;
    }
    g_board_info.pctrl = pinctrl;

    pinctrl_def = pinctrl_lookup_state(pinctrl, "default");
    if (OAL_IS_ERR_OR_NULL(pinctrl_def)) {
        ps_print_err("pinctrl_lookup_state default failed, and the value of pinctrl_def is %p\n", pinctrl_def);
        goto err_lookup_default;
    }
    g_board_info.pins_normal = pinctrl_def;

    pinctrl_idle = pinctrl_lookup_state(pinctrl, "idle");
    if (OAL_IS_ERR_OR_NULL(pinctrl_idle)) {
        ps_print_err("pinctrl_lookup_state idel failed, and the value of pinctrl_idle is %p\n", pinctrl_idle);
        goto err_lookup_idle;
    }
    g_board_info.pins_idle = pinctrl_idle;

    ret = pinctrl_select_state(g_board_info.pctrl, g_board_info.pins_normal);
    if (ret < 0) {
        ps_print_err("pinctrl_select_state default failed.\n");
        goto err_select_state;
    }
    ret = get_board_gpio(DTS_NODE_HI110X, DTS_PROP_GPIO_XLDO_PINMUX, &physical_gpio);
    if (ret != BOARD_SUCC) {
        ps_print_err("get dts prop %s failed\n", DTS_PROP_GPIO_POWEN_ON);
        CHR_EXCEPTION(CHR_WIFI_DEV(CHR_WIFI_DEV_EVENT_CHIP, CHR_WIFI_DEV_ERROR_GPIO));
        goto err_get_xldo_pinmux;
    }

    g_board_info.xldo_pinmux = physical_gpio;

    ret = of_property_read_u32(np, DTS_PROP_GPIO_XLDO_LEVEL, &physical_gpio);
    if (ret != BOARD_SUCC) {
        ps_print_err("get dts prop %s failed\n", DTS_PROP_GPIO_XLDO_LEVEL);
        CHR_EXCEPTION(CHR_WIFI_DEV(CHR_WIFI_DEV_EVENT_CHIP, CHR_WIFI_DEV_ERROR_GPIO));
        goto err_read_xldo_level;
    }

    g_board_info.gpio_xldo_level = physical_gpio;

    return BOARD_SUCC;

err_read_xldo_level:
err_get_xldo_pinmux:
err_select_state:
err_lookup_idle:
err_lookup_default:
    devm_pinctrl_put(g_board_info.pctrl);
err_pinctrl_get:
err_read_dts_node:

    return BOARD_FAIL;
#else
    return BOARD_SUCC;
#endif
}

int32 hi1102_get_ini_file_name_from_dts(int8 *dts_prop, int8 *prop_value, uint32 size)
{
#ifdef _PRE_CONFIG_USE_DTS
    int32 ret;
    struct device_node *np = NULL;
    int32 len;
    int8 out_str[HISI_CUST_NVRAM_LEN] = {0};

    np = of_find_compatible_node(NULL, NULL, CUST_COMP_NODE);
    if (np == NULL) {
        INI_ERROR("dts node %s not found", CUST_COMP_NODE);
        return INI_FAILED;
    }

    len = of_property_count_u8_elems(np, dts_prop);
    if (len < 0) {
        INI_ERROR("can't get len of dts prop(%s)", dts_prop);
        return INI_FAILED;
    }

    len = INI_MIN(len, (int32)sizeof(out_str));
    INI_DEBUG("read len of dts prop %s is:%d", dts_prop, len);
    ret = of_property_read_u8_array(np, dts_prop, out_str, len);
    if (ret < 0) {
        INI_ERROR("read dts prop (%s) fail", dts_prop);
        return INI_FAILED;
    }

    len = INI_MIN(len, (int32)size);
    if (memcpy_s(prop_value, (size_t)size, out_str, (size_t)len) != EOK) {
        INI_ERROR("memcpy_s error, destlen=%d, srclen=%d\n ", size, len);
        return INI_FAILED;
    }
    INI_DEBUG("dts prop %s value is:%s", dts_prop, prop_value);
#endif
    return INI_SUCC;
}
