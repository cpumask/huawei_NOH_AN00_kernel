/*
 * cps7181.h
 *
 * cps7181 macro, addr etc.
 *
 * Copyright (c) 2020-2020 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#ifndef _CPS7181_H_
#define _CPS7181_H_

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/gpio.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_address.h>
#include <linux/of_gpio.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/workqueue.h>
#include <linux/bitops.h>
#include <linux/jiffies.h>

#include <huawei_platform/log/hw_log.h>
#include <huawei_platform/power/boost_5v.h>
#include <huawei_platform/power/hardware_ic/charge_pump.h>
#include <huawei_platform/power/wireless_power_supply.h>
#include <huawei_platform/power/wireless_charger.h>
#include <huawei_platform/power/wireless_direct_charger.h>
#include <huawei_platform/power/wireless_transmitter.h>
#include <huawei_platform/power/wired_channel_switch.h>
#include <chipset_common/hwpower/power_gpio.h>
#include <chipset_common/hwpower/power_i2c.h>
#include <huawei_platform/power/wireless/wireless_fw.h>

#include "cps7181_chip.h"

#define CPS7181_SW2TX_SLEEP_TIME              25 /* ms */
#define CPS7181_SW2TX_RETRY_TIME              1500 /* ms */

#define CPS7181_SHUTDOWN_SLEEP_TIME           200
#define CPS7181_RCV_MSG_SLEEP_TIME            100
#define CPS7181_RCV_MSG_SLEEP_CNT             10
#define CPS7181_WAIT_FOR_ACK_SLEEP_TIME       100
#define CPS7181_WAIT_FOR_ACK_RETRY_CNT        5
#define CPS7181_SEND_MSG_RETRY_CNT            2
#define CPS7181_RX_TMP_BUFF_LEN               32

struct cps7181_chip_info {
	u16 chip_id;
	u16 mtp_ver;
};

struct cps7181_global_val {
	bool sram_i2c_ready;
	bool mtp_chk_complete;
	bool rx_stop_chrg_flag;
	bool irq_abnormal_flag;
	struct qi_protocol_handle *qi_hdl;
};

struct cps7181_dev_info {
	struct i2c_client *client;
	struct device *dev;
	struct work_struct irq_work;
	struct work_struct mtp_check_work;
	struct mutex mutex_irq;
	struct cps7181_global_val g_val;
	bool irq_active;
	u8 rx_fod_5v[CPS7181_RX_FOD_LEN];
	u8 rx_fod_9v[CPS7181_RX_FOD_LEN];
	u8 rx_fod_12v[CPS7181_RX_FOD_LEN];
	u8 rx_fod_15v[CPS7181_RX_FOD_LEN];
	u8 rx_ldo_cfg_5v[CPS7181_RX_LDO_CFG_LEN];
	u8 rx_ldo_cfg_9v[CPS7181_RX_LDO_CFG_LEN];
	u8 rx_ldo_cfg_12v[CPS7181_RX_LDO_CFG_LEN];
	u8 rx_ldo_cfg_sc[CPS7181_RX_LDO_CFG_LEN];
	int rx_ss_good_lth;
	int gpio_en;
	int gpio_en_valid_val;
	int gpio_sleep_en;
	int gpio_int;
	int gpio_pwr_good;
	int irq_int;
	u16 irq_val;
	int irq_cnt;
	u16 ept_type;
};

/* cps7181 i2c */
int cps7181_read_byte(u16 reg, u8 *data);
int cps7181_read_word(u16 reg, u16 *data);
int cps7181_write_byte(u16 reg, u8 data);
int cps7181_write_word(u16 reg, u16 data);
int cps7181_write_byte_mask(u16 reg, u8 mask, u8 shift, u8 data);
int cps7181_write_word_mask(u16 reg, u16 mask, u16 shift, u16 data);
int cps7181_read_block(u16 reg, u8 *data, u8 len);
int cps7181_write_block(u16 reg, u8 *data, u8 data_len);
int cps7181_aux_write_word(u16 reg, u16 data);
int cps7181_fw_sram_i2c_init(u8 inc_mode);
/* cps7181 common */
void cps7181_chip_enable(int enable);
void cps7181_sleep_enable(int enable);
void cps7181_enable_irq(void);
void cps7181_disable_irq_nosync(void);
void cps7181_ps_control(enum wlps_ctrl_scene scene, int ctrl_flag);
void cps7181_get_dev_info(struct cps7181_dev_info **di);
struct device_node *cps7181_dts_dev_node(void);
int cps7181_get_mode(u8 *mode);
/* cps7181 chip_info */
int cps7181_get_chip_id(u16 *chip_id);
u8 *cps7181_get_die_id(void);
int cps7181_get_chip_info(struct cps7181_chip_info *info);
int cps7181_get_chip_info_str(char *info_str, int len);

/* cps7181 rx */
struct wireless_charge_device_ops *cps7181_get_rx_ops(void);
void cps7181_rx_mode_irq_handler(struct cps7181_dev_info *di);
void cps7181_rx_abnormal_irq_handler(struct cps7181_dev_info *di);
void cps7181_rx_shutdown_handler(void);
void cps7181_rx_probe_check_tx_exist(void);
int cps7181_rx_ops_register(void);

/* cps7181 tx */
void cps7181_tx_mode_irq_handler(struct cps7181_dev_info *di);
struct wireless_tx_device_ops *cps7181_get_tx_ops(void);
int cps7181_tx_ps_ops_register(void);
int cps7181_tx_ops_register(void);

/* cps7181 fw */
void cps7181_fw_mtp_check_work(struct work_struct *work);
int cps7181_fw_sram_update(enum wireless_mode sram_mode);
int cps7181_fw_ops_register(void);

/* cps7181 qi */
struct qi_protocol_ops *cps7181_get_qi_ops(void);
int cps7181_qi_ops_register(void);

/* cps7181 dts */
int cps7181_parse_dts(struct device_node *np, struct cps7181_dev_info *di);

#endif /* _CPS7181_H_ */
