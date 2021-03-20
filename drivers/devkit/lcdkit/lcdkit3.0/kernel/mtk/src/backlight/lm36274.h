/*
* Simple driver for Texas Instruments LM3630 LED Flash driver chip
* Copyright (C) 2012 Texas Instruments
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 2 as
* published by the Free Software Foundation.
*
*/

#include <linux/module.h>
#include <linux/regmap.h>
#include <linux/gpio.h>
#include <linux/i2c.h>

#ifndef __LM36274_H
#define __LM36274_H

#define LM36274_NAME                        "lm36274"
#define DTS_COMP_LM36274                    "ti,lm36274"
#define LM36274_HIDDEN_REG_SUPPORT          "lm36274_hidden_reg_support"
#define LM36274_HW_ENABLE                   "lm36274_hw_enable"
#define LM36274_HW_EN_GPIO                  "lm36274_hw_en_gpio"
#define LM36274_HW_EN_DELAY                 "bl_on_kernel_mdelay"
#define LM36274_ONLY_BACKLIGHT "lm36274_only_backlight"
#define LM36274_HW_EN_PULL_LOW "lm36274_hw_en_pull_low"
#define LM36274_RW_REG_MAX 15


#define GPIO_DIR_OUT                        1
#define GPIO_OUT_ONE                        1
#define GPIO_OUT_ZERO                       0

#define MAX_RATE_NUM                        9
/* base reg */
#define REG_BL_CONFIG_1                     0x02
#define REG_BL_CONFIG_2                     0x03
#define REG_BL_BRIGHTNESS_LSB               0x04
#define REG_BL_BRIGHTNESS_MSB               0x05
#define REG_AUTO_FREQ_LOW                   0x06
#define REG_AUTO_FREQ_HIGH                  0x07
#define REG_BL_ENABLE                       0x08
#define REG_DISPLAY_BIAS_CONFIG_1           0x09
#define REG_DISPLAY_BIAS_CONFIG_2           0x0A
#define REG_DISPLAY_BIAS_CONFIG_3           0x0B
#define REG_LCM_BOOST_BIAS                  0x0C
#define REG_VPOS_BIAS                       0x0D
#define REG_VNEG_BIAS                       0x0E
#define REG_FLAGS                           0x0F
#define REG_BL_OPTION_1                     0x10
#define REG_BL_OPTION_2                     0x11
#define REG_PWM_TO_DIGITAL_LSB              0x12
#define REG_PWM_TO_DIGITAL_MSB              0x13
#define REG_HIDDEN_ADDR                     0x6A
#define REG_SET_SECURITYBIT_ADDR            0x50
#define REG_SET_SECURITYBIT_VAL             0x08
#define REG_CLEAR_SECURITYBIT_VAL           0x00

/* mask code */
#define MASK_BL_LSB                         0x07
#define MASK_LCM_EN                         0xE0
#define MASK_SOFTWARE_RESET                 0x80

/* update bit val */
#define DEVICE_FAULT_OCCUR                  0
#define DEVICE_RESET                        0x1

#define MSB                                 3
#define LSB                                 0x07
#define BL_MAX                              2047
#define BL_MIN                              0
#define BL_CONFIG_MODE_REG_NUM              3
#define BL_CONFIG_CURR_REG_NUM              2
#define BL_CONFIG_ENABLE_REG_NUM            1
#define BL_LOWER_POW_DELAY                  6
#define BL_MAX_PREFLASHING_TIMER            800
#define MAX_BL_RESUME_TIMMER                400
#define PROTECT_BL_RESUME_TIMMER            28
#define REG_REVISION                        0x01
#define REG_MAX                             0x14
#define LOG_LEVEL_INFO                      8

/* bl_mode_config reg */
#define BL_MAX_CONFIG_REG_NUM               3
#define FLAG_CHECK_NUM                      3

#define lm36274_emerg(msg, ...)    \
	do { if (lm36274_msg_level > 0)  \
		printk(KERN_EMERG "[lm36274]%s: "msg, __func__, ## __VA_ARGS__); } while (0)
#define lm36274_alert(msg, ...)    \
	do { if (lm36274_msg_level > 1)  \
		printk(KERN_ALERT "[lm36274]%s: "msg, __func__, ## __VA_ARGS__); } while (0)
#define lm36274_crit(msg, ...)    \
	do { if (lm36274_msg_level > 2)  \
		printk(KERN_CRIT "[lm36274]%s: "msg, __func__, ## __VA_ARGS__); } while (0)
#define lm36274_err(msg, ...)    \
	do { if (lm36274_msg_level > 3)  \
		printk(KERN_ERR "[lm36274]%s: "msg, __func__, ## __VA_ARGS__); } while (0)
#define lm36274_warning(msg, ...)    \
	do { if (lm36274_msg_level > 4)  \
		printk(KERN_WARNING "[lm36274]%s: "msg, __func__, ## __VA_ARGS__); } while (0)
#define lm36274_notice(msg, ...)    \
	do { if (lm36274_msg_level > 5)  \
		printk(KERN_NOTICE "[lm36274]%s: "msg, __func__, ## __VA_ARGS__); } while (0)
#define lm36274_info(msg, ...)    \
	do { if (lm36274_msg_level > 6)  \
		printk(KERN_INFO "[lm36274]%s: "msg, __func__, ## __VA_ARGS__); } while (0)
#define lm36274_debug(msg, ...)    \
	do { if (lm36274_msg_level > 7)  \
		printk(KERN_DEBUG "[lm36274]%s: "msg, __func__, ## __VA_ARGS__); } while (0)

#define LM36274_VOL_400 (0x00)
#define LM36274_VOL_405 (0x01)
#define LM36274_VOL_410 (0x02)
#define LM36274_VOL_415 (0x03)
#define LM36274_VOL_420 (0x04)
#define LM36274_VOL_425 (0x05)
#define LM36274_VOL_430 (0x06)
#define LM36274_VOL_435 (0x07)
#define LM36274_VOL_440 (0x08)
#define LM36274_VOL_445 (0x09)
#define LM36274_VOL_450 (0x0A)
#define LM36274_VOL_455 (0x0B)
#define LM36274_VOL_460 (0x0C)
#define LM36274_VOL_465 (0x0D)
#define LM36274_VOL_470 (0x0E)
#define LM36274_VOL_475 (0x0F)
#define LM36274_VOL_480 (0x10)
#define LM36274_VOL_485 (0x11)
#define LM36274_VOL_490 (0x12)
#define LM36274_VOL_495 (0x13)
#define LM36274_VOL_500 (0x14)
#define LM36274_VOL_505 (0x15)
#define LM36274_VOL_510 (0x16)
#define LM36274_VOL_515 (0x17)
#define LM36274_VOL_520 (0x18)
#define LM36274_VOL_525 (0x19)
#define LM36274_VOL_530 (0x1A)
#define LM36274_VOL_535 (0x1B)
#define LM36274_VOL_540 (0x1C)
#define LM36274_VOL_545 (0x1D)
#define LM36274_VOL_550 (0x1E)
#define LM36274_VOL_555 (0x1F)
#define LM36274_VOL_560 (0x20)
#define LM36274_VOL_565 (0x21)
#define LM36274_VOL_570 (0x22)
#define LM36274_VOL_575 (0x23)
#define LM36274_VOL_580 (0x24)
#define LM36274_VOL_585 (0x25)
#define LM36274_VOL_590 (0x26)
#define LM36274_VOL_595 (0x27)
#define LM36274_VOL_600 (0x28)
#define LM36274_VOL_605 (0x29)
#define LM36274_VOL_640 (0x30)
#define LM36274_VOL_645 (0x31)
#define LM36274_VOL_650 (0x32)

static char *lm36274_dts_string[LM36274_RW_REG_MAX] = {
	"lm36274_bl_config_1",
	"lm36274_bl_config_2",
	"lm36274_bl_brightness_lsb",
	"lm36274_bl_brightness_msb",
	"lm36274_auto_freq_low",
	"lm36274_auto_freq_high",
	"lm36274_display_bias_config_2",
	"lm36274_display_bias_config_3",
	"lm36274_lcm_boost_bias",
	"lm36274_vpos_bias",
	"lm36274_vneg_bias",
	"lm36274_bl_option_1",
	"lm36274_bl_option_2",
	"lm36274_display_bias_config_1",
	"lm36274_bl_en",
};

static unsigned int lm36274_reg_addr[LM36274_RW_REG_MAX] = {
	REG_BL_CONFIG_1,
	REG_BL_CONFIG_2,
	REG_BL_BRIGHTNESS_LSB,
	REG_BL_BRIGHTNESS_MSB,
	REG_AUTO_FREQ_LOW,
	REG_AUTO_FREQ_HIGH,
	REG_DISPLAY_BIAS_CONFIG_2,
	REG_DISPLAY_BIAS_CONFIG_3,
	REG_LCM_BOOST_BIAS,
	REG_VPOS_BIAS,
	REG_VNEG_BIAS,
	REG_BL_OPTION_1,
	REG_BL_OPTION_2,
	REG_DISPLAY_BIAS_CONFIG_1,
	REG_BL_ENABLE,
};

struct lm36274_voltage {
	unsigned int voltage;
	int value;
};

struct lm36274_chip_data {
	struct device *dev;
	struct i2c_client *client;
	struct regmap *regmap;
	struct semaphore test_sem;
};

struct backlight_information {
	/* whether support lm36274 or not */
	int lm36274_support;
	/* which i2c bus controller lm36274 mount */
	int lm36274_i2c_bus_id;
	/* lm36274 hw_en gpio */
	int lm36274_hw_en_gpio;
	int lm36274_hw_en;
	int bl_on_kernel_mdelay;
	int bl_only;
	int lm36274_hw_en_pull_low;
};

enum bl_enable{
	EN_2_SINK = 0x15,
	EN_4_SINK = 0x1F,
	BL_RESET = 0x80,
	BL_DISABLE = 0x00,
};

enum lcm_en {
	NORMAL_MODE = 0x80,
	BIAS_SUPPLY_OFF = 0x00,
};

enum {
	BL_OVP_25V = 0x40,
	BL_OVP_29V = 0x60,
};

enum {
	CURRENT_RAMP_0US = 0x85,
	CURRENT_RAMP_5MS = 0xAD,
};

enum {
	LSB_MIN = 0x00,
	LSB_10MA = 0x05,
	LSB_30MA = 0x07,
};

enum {
	MSB_MIN = 0x00,
	MSB_10MA = 0xD2,
	MSB_30MA = 0xFF,
};

enum bl_option_2{
	BL_OCP_1 = 0x35,    /*1.2A */
	BL_OCP_2 = 0x36,    /*1.5A */
	BL_OCP_3 = 0x37,    /*1.8A */
};

enum {
	BIAS_BOOST_TIME_0 = 0x00, /*156ns*/
	BIAS_BOOST_TIME_1 = 0x10, /*181ns*/
	BIAS_BOOST_TIME_2 = 0x20, /*206ns*/
	BIAS_BOOST_TIME_3 = 0x30, /*231ns*/
};

enum resume_type {
	RESUME_IDLE = 0x0,
	RESUME_2_SINK = 0x1,
	RESUME_REMP_OVP_OCP = 0x2,
};

enum {
	OVP_FAULT_FLAG = 0x02,
	OCP_FAULT_FLAG = 0x01,
	TSD_FAULT_FLAG = 0x40,
};

struct lm36274_check_flag {
	u8 flag;
	int err_no;
};

int lm36274_set_backlight(unsigned int bl_level);
ssize_t lm36274_set_reg(u8 bl_reg,u8 bl_mask,u8 bl_val);

#endif /* __LM36274_H */

