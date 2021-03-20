/*
 * rt9466.c
 *
 * rt9466 driver
 *
 * Copyright (c) 2017-2020 Huawei Technologies Co., Ltd.
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

#include <linux/module.h>
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/of_device.h>
#include <linux/of_irq.h>
#include <linux/of_gpio.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/delay.h>
#include <linux/mutex.h>
#include <linux/version.h>
#include <linux/workqueue.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/pm_runtime.h>
#include <huawei_platform/log/hw_log.h>
#include <huawei_platform/power/huawei_charger.h>
#include <../../../charger/charger_ap/charging_core.h>
#include <rt9466.h>
#ifdef CONFIG_HUAWEI_USB_SHORT_CIRCUIT_PROTECT
#include <huawei_platform/power/usb_short_circuit_protect.h>
#endif

#define HWLOG_TAG rt9466_charger
HWLOG_REGIST();

#define I2C_ACCESS_MAX_RETRY    5
#define RT9466_DRV_VERSION      "1.0.2_HUAWEI"
#define RT9466_TDEG_EOC_16MS    0x7
#define RT9466_SW_RESET_NO_HZ   0x10
#define RT9466_SW_RESET_HZ      0x14
#define VBUS_THRESHOLD          3800
#define VBUS_POST_THRESHOLD     1000
#define RT9466_BOOST_VOTGBST    5000
#define AICR_LOWER_BOUND        150
#define VBUS_FAKE_5V            5000

#define GPIO_LOW                0
#define GPIO_HIGH               1
#define DELAY_10MS              10
#define DELAY_50MS              50
#define DELAY_100MS             100
#define DELAY_1500MS            1500
#define VOLT_THRES_7P5          7500 /* mv, 9V detect threshold */
#define RETRY_MAX3              3

#define CEB_GPIO_NAME_LEN       10
#define IRQ_GPIO_NAME_LEN       10
#define IRQ_NAME_LEN            5
#define PRINT_BUFF_SIZE         26
#define MIN_CV                  4350

static int huawei_force_hz; /* for huawei force enable hz */
static struct i2c_client *g_rt9466_i2c;
static int hiz_iin_limit_flag;
static int g_rt9466_cv;
/* ======================= */
/* RT9466 Parameter        */
/* ======================= */

static const u32 rt9466_boost_oc_threshold[] = {
	500, 700, 1100, 1300, 1800, 2100, 2400, 3000,
}; /* mA */

static const u32 rt9466_safety_timer[] = {
	4, 6, 8, 10, 12, 14, 16, 20,
}; /* hour */

static const u32 rt9466_wdt[] = {
	8, 40, 80, 160,
}; /* s */

/* Register 0x01 ~ 0x10 */
static u8 reset_reg_data[] = {
	0x10, 0x03, 0x23, 0x3C, 0x67, 0x0B, 0x4C, 0xA1,
	0x3C, 0x58, 0x2C, 0x82, 0x52, 0x05, 0x00, 0x10
};

enum rt9466_irq_idx {
	RT9466_IRQIDX_CHG_STATC = 0,
	RT9466_IRQIDX_CHG_FAULT,
	RT9466_IRQIDX_TS_STATC,
	RT9466_IRQSTAT_MAX,
	RT9466_IRQIDX_CHG_IRQ1 = RT9466_IRQSTAT_MAX,
	RT9466_IRQIDX_CHG_IRQ2,
	RT9466_IRQIDX_CHG_IRQ3,
	RT9466_IRQIDX_MAX,
};

static const u8 rt9466_irq_maskall[RT9466_IRQIDX_MAX] = {
	0xF0, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF
};

enum rt9466_charging_status {
	RT9466_CHG_STATUS_READY = 0,
	RT9466_CHG_STATUS_PROGRESS,
	RT9466_CHG_STATUS_DONE,
	RT9466_CHG_STATUS_FAULT,
	RT9466_CHG_STATUS_MAX,
};

static const char *rt9466_chg_status_name[RT9466_CHG_STATUS_MAX] = {
	"ready", "progress", "done", "fault",
};

static const u8 rt9466_val_en_hidden_mode[] = {
	0x49, 0x32, 0xB6, 0x27, 0x48, 0x18, 0x03, 0xE2,
};

enum rt9466_iin_limit_sel {
	RT9466_IIMLMTSEL_PSEL_OTG,
	RT9466_IINLMTSEL_AICR = 2,
	RT9466_IINLMTSEL_LOWER_LEVEL, /* lower of above two */
};

enum rt9466_adc_sel {
	RT9466_ADC_VBUS_DIV5 = 1,
	RT9466_ADC_VBUS_DIV2,
	RT9466_ADC_VSYS,
	RT9466_ADC_VBAT,
	RT9466_ADC_TS_BAT = 6,
	RT9466_ADC_IBUS = 8,
	RT9466_ADC_IBAT,
	RT9466_ADC_REGN = 11,
	RT9466_ADC_TEMP_JC,
	RT9466_ADC_MAX,
};

/*
 * Unit for each ADC parameter
 * 0 stands for reserved
 * For TS_BAT, the real unit is 0.25.
 * Here we use 25, please remember to divide 100 while showing the value
 */
static const int rt9466_adc_unit[RT9466_ADC_MAX] = {
	0,
	RT9466_ADC_UNIT_VBUS_DIV5,
	RT9466_ADC_UNIT_VBUS_DIV2,
	RT9466_ADC_UNIT_VSYS,
	RT9466_ADC_UNIT_VBAT,
	0,
	RT9466_ADC_UNIT_TS_BAT,
	0,
	RT9466_ADC_UNIT_IBUS,
	RT9466_ADC_UNIT_IBAT,
	0,
	RT9466_ADC_UNIT_REGN,
	RT9466_ADC_UNIT_TEMP_JC,
};

static const int rt9466_adc_offset[RT9466_ADC_MAX] = {
	0,
	RT9466_ADC_OFFSET_VBUS_DIV5,
	RT9466_ADC_OFFSET_VBUS_DIV2,
	RT9466_ADC_OFFSET_VSYS,
	RT9466_ADC_OFFSET_VBAT,
	0,
	RT9466_ADC_OFFSET_TS_BAT,
	0,
	RT9466_ADC_OFFSET_IBUS,
	RT9466_ADC_OFFSET_IBAT,
	0,
	RT9466_ADC_OFFSET_REGN,
	RT9466_ADC_OFFSET_TEMP_JC,
};

struct rt9466_desc {
	u32 ichg;            /* mA */
	u32 aicr;            /* mA */
	u32 mivr;            /* mV */
	u32 cv;              /* mV */
	u32 ieoc;            /* mA */
	u32 safety_timer;    /* hour */
	u32 ircmp_resistor;  /* uohm */
	u32 ircmp_vclamp;    /* mV */
	bool en_te;
	bool en_wdt;
	bool en_irq_pulse;
	bool en_jeita;
	int regmap_represent_slave_addr;
	const char *regmap_name;
	const char *chg_name;
	bool ceb_invert;
	int hiz_iin_limit;
};

/* These default values will be applied if there's no property in dts */
static struct rt9466_desc rt9466_default_desc = {
	.ichg = 2000,           /* mA */
	.aicr = 500,            /* mA */
	.mivr = 4400,           /* mV */
	.cv = 4350,             /* mA */
	.ieoc = 250,            /* mA */
	.safety_timer = 12,     /* hour */
	.ircmp_resistor = 25,   /* mohm */
	.ircmp_vclamp = 32,     /* mV */
	.en_te = true,
	.en_wdt = true,
	.en_irq_pulse = false,
	.en_jeita = false,
	.regmap_represent_slave_addr = RT9466_SLAVE_ADDR,
	.regmap_name = "rt9466",
	.chg_name = "primary_chg",
	.ceb_invert = false,
};

struct rt9466_info {
	struct i2c_client *client;
	struct mutex io_lock;
	struct mutex adc_lock;
	struct mutex irq_lock;
	struct mutex aicr_lock;
	struct mutex ichg_lock;
	struct mutex ieoc_lock;
	struct mutex hidden_mode_lock;
	struct rt9466_desc *desc;
	struct device *dev;
	wait_queue_head_t wait_queue;
	int irq;
	int aicr_limit;
	u32 intr_gpio;
	u32 ceb_gpio;
	u8 chip_rev;
	u8 irq_flag[RT9466_IRQIDX_MAX];
	u8 irq_stat[RT9466_IRQSTAT_MAX];
	u8 irq_mask[RT9466_IRQIDX_MAX];
	u32 hidden_mode_cnt;
	u32 ieoc;
	bool ieoc_wkard;
	struct work_struct init_work;
	struct work_struct irq_work;
	struct work_struct aicr_work;
	struct delayed_work mivr_unmask_dwork;
	atomic_t pwr_rdy;
	struct charge_core_data *core_data;
	bool irq_active;
	int fcp_support;
	int gpio_fcp;
};

/* ======================= */
/* Register Address        */
/* ======================= */

static const unsigned char rt9466_reg_addr[] = {
	RT9466_REG_CORE_CTRL0,
	RT9466_REG_CHG_CTRL1,
	RT9466_REG_CHG_CTRL2,
	RT9466_REG_CHG_CTRL3,
	RT9466_REG_CHG_CTRL4,
	RT9466_REG_CHG_CTRL5,
	RT9466_REG_CHG_CTRL6,
	RT9466_REG_CHG_CTRL7,
	RT9466_REG_CHG_CTRL8,
	RT9466_REG_CHG_CTRL9,
	RT9466_REG_CHG_CTRL10,
	RT9466_REG_CHG_CTRL11,
	RT9466_REG_CHG_CTRL12,
	RT9466_REG_CHG_CTRL13,
	RT9466_REG_CHG_CTRL14,
	RT9466_REG_CHG_CTRL15,
	RT9466_REG_CHG_CTRL16,
	RT9466_REG_CHG_ADC,
	RT9466_REG_CHG_CTRL19,
	RT9466_REG_CHG_CTRL17,
	RT9466_REG_CHG_CTRL18,
	RT9466_REG_DEVICE_ID,
	RT9466_REG_CHG_STAT,
	RT9466_REG_CHG_NTC,
	RT9466_REG_ADC_DATA_H,
	RT9466_REG_ADC_DATA_L,
	RT9466_REG_CHG_STATC,
	RT9466_REG_CHG_FAULT,
	RT9466_REG_TS_STATC,
	/* Skip IRQ evt to prevent reading clear while dumping registers */
	RT9466_REG_CHG_STATC_CTRL,
	RT9466_REG_CHG_FAULT_CTRL,
	RT9466_REG_TS_STATC_CTRL,
	RT9466_REG_CHG_IRQ1_CTRL,
	RT9466_REG_CHG_IRQ2_CTRL,
	RT9466_REG_CHG_IRQ3_CTRL,
};

/* function declare here */
static int rt9466_set_aicr(int aicr);
static int rt9466_is_support_fcp(void);
static int rt9466_enable_hz(int en);
static int rt9466_kick_wdt(void);
static int rt9466_init_setting(struct rt9466_info *info);
static inline int rt9466_irq_init(struct rt9466_info *info);

/* ========================= */
/* I2C operations            */
/* ========================= */
static int rt9466_device_read(void *client, u32 addr, int leng, void *dst)
{
	struct i2c_client *i2c = (struct i2c_client *)client;

	if (!i2c)
		return -1;

	return i2c_smbus_read_i2c_block_data(i2c, addr, leng, dst);
}

static int rt9466_device_write(void *client, u32 addr, int leng,
	const void *src)
{
	struct i2c_client *i2c = (struct i2c_client *)client;

	if (!i2c)
		return -1;

	return i2c_smbus_write_i2c_block_data(i2c, addr, leng, src);
}

static inline int __rt9466_i2c_write_byte(struct rt9466_info *info, u8 cmd,
	u8 data)
{
	int ret;
	int retry = 0;

	if (!info->client)
		return -1;

	do {
		ret = rt9466_device_write(info->client, cmd, 1, &data);
		retry++;
		if (ret < 0)
			udelay(10); /* delay 10us after i2c write */
	} while (ret < 0 && retry < I2C_ACCESS_MAX_RETRY);

	if (ret < 0)
		hwlog_err("I2CW[0x%02X] = 0x%02X fail\n", cmd, data);
	else
		hwlog_debug("I2CW[0x%02X] = 0x%02X\n", cmd, data);

	return ret;
}

static int rt9466_i2c_write_byte(struct rt9466_info *info, u8 cmd, u8 data)
{
	int ret;

	mutex_lock(&info->io_lock);
	ret = __rt9466_i2c_write_byte(info, cmd, data);
	mutex_unlock(&info->io_lock);

	return ret;
}

static int __rt9466_i2c_read_byte(struct rt9466_info *info, u8 cmd)
{
	int ret;
	int ret_val = 0;
	int retry = 0;

	if (!info->client)
		return -1;

	do {
		ret = rt9466_device_read(info->client, cmd, 1, &ret_val);
		retry++;
		if (ret < 0)
			udelay(10); /* delay 10 us after i2c read fail */
	} while (ret < 0 && retry < I2C_ACCESS_MAX_RETRY);

	if (ret < 0) {
		hwlog_err("I2CR[0x%02X] fail\n", cmd);
		return ret;
	}

	ret_val = (u8)ret_val & 0xFF;

	hwlog_debug("I2CR[0x%02X] = 0x%02X\n", cmd, ret_val);

	return ret_val;
}

static int rt9466_i2c_read_byte(struct rt9466_info *info, u8 cmd)
{
	int ret;

	mutex_lock(&info->io_lock);
	ret = __rt9466_i2c_read_byte(info, cmd);
	mutex_unlock(&info->io_lock);

	if (ret < 0)
		return ret;

	return ((u8)ret & 0xFF);
}

static inline int __rt9466_i2c_block_write(struct rt9466_info *info,
	u8 cmd, u32 leng, const u8 *data)
{
	return rt9466_device_write(info->client, cmd, leng, data);
}


static int rt9466_i2c_block_write(struct rt9466_info *info, u8 cmd,
	u32 leng, const u8 *data)
{
	int ret;

	mutex_lock(&info->io_lock);
	ret = __rt9466_i2c_block_write(info, cmd, leng, data);
	mutex_unlock(&info->io_lock);

	return ret;
}

static inline int __rt9466_i2c_block_read(struct rt9466_info *info,
	u8 cmd, u32 leng, u8 *data)
{
	return rt9466_device_read(info->client, cmd, leng, data);
}


static int rt9466_i2c_block_read(struct rt9466_info *info, u8 cmd,
	u32 leng, u8 *data)
{
	int ret;

	mutex_lock(&info->io_lock);
	ret = __rt9466_i2c_block_read(info, cmd, leng, data);
	mutex_unlock(&info->io_lock);

	return ret;
}

static int rt9466_i2c_test_bit(struct rt9466_info *info, u8 cmd, u8 shift,
	bool *is_one)
{
	int ret;
	u8 data;

	ret = rt9466_i2c_read_byte(info, cmd);
	if (ret < 0) {
		*is_one = false;
		return ret;
	}

	data = ret & (BIT_TRUE << shift);
	*is_one = (data == 0 ? false : true);

	return ret;
}

static int rt9466_i2c_update_bits(struct rt9466_info *info, u8 cmd,
	u8 data, u8 mask)
{
	int ret;
	u8 reg_data;

	mutex_lock(&info->io_lock);
	ret = __rt9466_i2c_read_byte(info, cmd);
	if (ret < 0) {
		mutex_unlock(&info->io_lock);
		return ret;
	}

	reg_data = (u8)ret & 0xFF;
	reg_data &= ~mask;
	reg_data |= (data & mask);

	ret = __rt9466_i2c_write_byte(info, cmd, reg_data);
	mutex_unlock(&info->io_lock);

	return ret;
}

static inline int rt9466_set_bit(struct rt9466_info *info, u8 reg, u8 mask)
{
	return rt9466_i2c_update_bits(info, reg, mask, mask);
}

static inline int rt9466_clr_bit(struct rt9466_info *info, u8 reg, u8 mask)
{
	return rt9466_i2c_update_bits(info, reg, 0x00, mask);
}

static inline void rt9466_irq_set_flag(struct rt9466_info *info, u8 *irq,
	u8 mask)
{
	mutex_lock(&info->irq_lock);
	*irq |= mask;
	mutex_unlock(&info->irq_lock);
}

static inline void rt9466_irq_clr_flag(struct rt9466_info *info, u8 *irq,
	u8 mask)
{
	mutex_lock(&info->irq_lock);
	*irq &= ~mask;
	mutex_unlock(&info->irq_lock);
}

static inline u8 rt9466_closest_reg(u32 min, u32 max, u32 step, u32 target)
{
	if (target < min)
		return 0;

	if (target >= max)
		return (max - min) / step;

	return (target - min) / step;
}

static u8 rt9466_closest_reg_via_tbl(const u32 *tbl, u32 tbl_size,
	u32 target)
{
	u32 i;

	if (target < tbl[0])
		return 0;

	for (i = 0; i < tbl_size - 1; i++) {
		if (target >= tbl[i] && target < tbl[i + 1])
			return i;
	}

	return tbl_size - 1;
}

static u32 rt9466_closest_value(u32 min, u32 max, u32 step,
	u8 reg_val)
{
	u32 ret_val;

	ret_val = min + reg_val * step;
	if (ret_val > max)
		ret_val = max;

	return ret_val;
}

static int rt9466_get_aicr(struct rt9466_info *info, u32 *aicr)
{
	int ret;
	u8 reg_aicr;

	ret = rt9466_i2c_read_byte(info, RT9466_REG_CHG_CTRL3);
	if (ret < 0)
		return ret;

	reg_aicr = ((u8)ret & RT9466_MASK_AICR) >> RT9466_SHIFT_AICR;
	*aicr = rt9466_closest_value(RT9466_AICR_MIN, RT9466_AICR_MAX,
		RT9466_AICR_STEP, reg_aicr);

	return ret;
}

static int rt9466_get_ichg(struct rt9466_info *info, u32 *ichg)
{
	int ret;
	u8 reg_ichg;

	ret = rt9466_i2c_read_byte(info, RT9466_REG_CHG_CTRL7);
	if (ret < 0)
		return ret;

	reg_ichg = ((u8)ret & RT9466_MASK_ICHG) >> RT9466_SHIFT_ICHG;
	*ichg = rt9466_closest_value(RT9466_ICHG_MIN, RT9466_ICHG_MAX,
		RT9466_ICHG_STEP, reg_ichg);

	return ret;
}

static int rt9466_get_mivr(struct rt9466_info *info, u32 *mivr)
{
	int ret;
	u8 reg_mivr;

	ret = rt9466_i2c_read_byte(info, RT9466_REG_CHG_CTRL6);
	if (ret < 0)
		return ret;
	reg_mivr = (((u8)ret & RT9466_MASK_MIVR) >> RT9466_SHIFT_MIVR) & 0xFF;

	*mivr = rt9466_closest_value(RT9466_MIVR_MIN, RT9466_MIVR_MAX,
		RT9466_MIVR_STEP, reg_mivr);

	return ret;
}

static int rt9466_get_ieoc(struct rt9466_info *info, u32 *ieoc)
{
	int ret;
	u8 reg_ieoc;

	ret = rt9466_i2c_read_byte(info, RT9466_REG_CHG_CTRL9);
	if (ret < 0)
		return ret;

	reg_ieoc = ((u8)ret & RT9466_MASK_IEOC) >> RT9466_SHIFT_IEOC;
	*ieoc = rt9466_closest_value(RT9466_IEOC_MIN, RT9466_IEOC_MAX,
		RT9466_IEOC_STEP, reg_ieoc);

	return ret;
}

static int rt9466_get_adc(struct rt9466_info *info,
	enum rt9466_adc_sel adc_sel, int *adc_val)
{
	int ret, i;
	int max_wait_times = 6;
	u8 adc_data[2] = { 0, 0 }; /* init array */
	u32 aicr = 0;
	u32 ichg = 0;
	bool adc_start = false;

	mutex_lock(&info->adc_lock);

	/* Select ADC to desired channel */
	ret = rt9466_i2c_update_bits(info, RT9466_REG_CHG_ADC,
		(u8)adc_sel << RT9466_SHIFT_ADC_IN_SEL,
		RT9466_MASK_ADC_IN_SEL);
	if (ret < 0) {
		hwlog_err("select ch to %d fail\n", adc_sel);
		goto out;
	}

	/* Coefficient for IBUS & IBAT */
	if (adc_sel == RT9466_ADC_IBUS) {
		mutex_lock(&info->aicr_lock);
		ret = rt9466_get_aicr(info, &aicr);
		if (ret < 0) {
			hwlog_err("get aicr fail\n");
			goto out_unlock_all;
		}
	} else if (adc_sel == RT9466_ADC_IBAT) {
		mutex_lock(&info->ichg_lock);
		ret = rt9466_get_ichg(info, &ichg);
		if (ret < 0) {
			hwlog_err("get ichg fail\n");
			goto out_unlock_all;
		}
	}

	/* Start ADC conversation */
	ret = rt9466_set_bit(info, RT9466_REG_CHG_ADC, RT9466_MASK_ADC_START);
	if (ret < 0) {
		hwlog_err("start con fail, sel = %d\n", adc_sel);
		goto out_unlock_all;
	}

	for (i = 0; i < max_wait_times; i++) {
		msleep(35); /* before test need 35ms delay */
		ret = rt9466_i2c_test_bit(info, RT9466_REG_CHG_ADC,
			RT9466_SHIFT_ADC_START, &adc_start);
		if (ret >= 0 && !adc_start)
			break;
	}
	if (i == max_wait_times) {
		hwlog_err("wait con fail, sel = %d\n", adc_sel);
		ret = -EINVAL;
		goto out_unlock_all;
	}

	usleep_range(1000, 1100); /* before read need 1ms delay */

	/* Read ADC data high/low byte */
	ret = rt9466_i2c_block_read(info, RT9466_REG_ADC_DATA_H, 2, adc_data);
	if (ret < 0) {
		hwlog_err("read ADC data fail\n");
		goto out_unlock_all;
	}

	/* Calculate ADC value */
	*adc_val = ((adc_data[0] << 8) + adc_data[1]) *
		rt9466_adc_unit[adc_sel] + rt9466_adc_offset[adc_sel];

	hwlog_info("adc_sel = %d, adc_h = 0x%02X, adc_l = 0x%02X, val = %d\n",
		adc_sel, adc_data[0], adc_data[1], *adc_val);

	ret = 0;

out_unlock_all:
	/* Coefficient of IBUS & IBAT */
	if (adc_sel == RT9466_ADC_IBUS) {
		if (aicr < 400) /* 400mA */
			*adc_val = *adc_val * 67 / 100; /* adc conv value */
		mutex_unlock(&info->aicr_lock);
	} else if (adc_sel == RT9466_ADC_IBAT) {
		if (ichg >= 100 && ichg <= 450) /* 100~450mA */
			*adc_val = *adc_val * 57 / 100;
		else if (ichg >= 500 && ichg <= 850) /* 500~850mA */
			*adc_val = *adc_val * 63 / 100;  /* adc conv value */
		mutex_unlock(&info->ichg_lock);
	}

out:
	mutex_unlock(&info->adc_lock);
	return ret;
}

static int rt9466_is_charging_enable(struct rt9466_info *info, bool *en)
{
	return rt9466_i2c_test_bit(info, RT9466_REG_CHG_CTRL2,
		RT9466_SHIFT_CHG_EN, en);
}

static inline int rt9466_enable_ilim(struct rt9466_info *info, bool en)
{
	int ret;

	hwlog_info("en = %d\n", en);
	if (en)
		ret = rt9466_set_bit(info, RT9466_REG_CHG_CTRL3,
			RT9466_MASK_ILIM_EN);
	else
		ret = rt9466_clr_bit(info, RT9466_REG_CHG_CTRL3,
			RT9466_MASK_ILIM_EN);

	return ret;
}

static int __rt9466_set_aicr(struct rt9466_info *info, u32 aicr)
{
	u8 reg_aicr;

	reg_aicr = rt9466_closest_reg(RT9466_AICR_MIN, RT9466_AICR_MAX,
		RT9466_AICR_STEP, aicr);

	hwlog_info("aicr = %d(0x%02X)\n", aicr, reg_aicr);

	return rt9466_i2c_update_bits(info, RT9466_REG_CHG_CTRL3,
		reg_aicr << RT9466_SHIFT_AICR, RT9466_MASK_AICR);
}

static int __rt9466_run_aicr(struct rt9466_info *info)
{
	int ret;
	u32 aicr = 0;
	bool mivr_act = false;

	/* Check whether MIVR loop is active */
	ret = rt9466_i2c_test_bit(info, RT9466_REG_CHG_STATC,
		RT9466_SHIFT_CHG_MIVR, &mivr_act);
	if (ret < 0) {
		hwlog_err("read mivr stat fail\n");
		goto out;
	}

	if (!mivr_act) {
		hwlog_info("mivr loop is not active\n");
		goto out;
	}

	mutex_lock(&info->aicr_lock);
	ret = rt9466_get_aicr(info, &aicr);
	if (ret < 0)
		goto unlock_out;

	aicr -= RT9466_AICR_STEP;
	if (aicr < AICR_LOWER_BOUND)
		aicr = AICR_LOWER_BOUND;
	ret = __rt9466_set_aicr(info, aicr);
	if (ret < 0)
		goto unlock_out;

	info->aicr_limit = aicr;
	hwlog_debug("aicr upper bound = %dmA\n", info->aicr_limit);

unlock_out:
	mutex_unlock(&info->aicr_lock);
out:
	return ret;
}

static void rt9466_mivr_unmask_dwork_handler(struct work_struct *work)
{
	int ret;
	struct delayed_work *dwork = to_delayed_work(work);
	struct rt9466_info *info = (struct rt9466_info *)container_of(dwork,
		struct rt9466_info, mivr_unmask_dwork);

	/* Enable MIVR IRQ */
	ret = rt9466_clr_bit(info, RT9466_REG_CHG_STATC_CTRL,
				     RT9466_MASK_CHG_MIVR);
	if (ret < 0)
		hwlog_err("en MIVR IRQ failed\n");
}

static int rt9466_toggle_cfo(struct rt9466_info *info)
{
	int ret;
	u8 data = 0;

	mutex_lock(&info->io_lock);
	ret = rt9466_device_read(info->client, RT9466_REG_CHG_CTRL2, 1, &data);
	if (ret < 0) {
		hwlog_err("read cfo fail\n");
		goto out;
	}

	/* CFO off */
	data &= ~RT9466_MASK_CFO_EN;
	ret = rt9466_device_write(info->client, RT9466_REG_CHG_CTRL2, 1, &data);
	if (ret < 0) {
		hwlog_err("cfo off fail\n");
		goto out;
	}

	/* CFO on */
	data |= RT9466_MASK_CFO_EN;
	ret = rt9466_device_write(info->client, RT9466_REG_CHG_CTRL2, 1, &data);
	if (ret < 0)
		hwlog_err("cfo on fail\n");

out:
	mutex_unlock(&info->io_lock);
	return ret;
}

/* IRQ handlers */
static int rt9466_reserved_irq_handler(struct rt9466_info *info)
{
	return 0;
}

static int rt9466_pwr_rdy_irq_handler(struct rt9466_info *info)
{
	int ret;
	bool pwr_rdy = false;

	ret = rt9466_i2c_test_bit(info, RT9466_REG_CHG_STATC,
		RT9466_SHIFT_PWR_RDY, &pwr_rdy);
	if (ret < 0)
		return ret;

	ret = rt9466_enable_hz(!pwr_rdy);
	if (ret < 0) {
		hwlog_err("enable_hz fail\n");
		return ret;
	}
	/* reset aicl_limit */
	if (pwr_rdy)
		info->aicr_limit = -1;

	atomic_set(&info->pwr_rdy, pwr_rdy);
	return 0;
}

static int rt9466_chg_mivr_irq_handler(struct rt9466_info *info)
{
	int ret;
	bool mivr_act = false;
	int adc_ibus = 0;

	/* Disable MIVR IRQ */
	ret = rt9466_set_bit(info, RT9466_REG_CHG_STATC_CTRL,
				RT9466_MASK_CHG_MIVR);

	/* Check whether MIVR loop is active */
	ret = rt9466_i2c_test_bit(info, RT9466_REG_CHG_STATC,
		RT9466_SHIFT_CHG_MIVR, &mivr_act);
	if (ret < 0) {
		hwlog_err("read mivr stat failed\n");
		goto out;
	}

	if (!mivr_act) {
		hwlog_info("mivr loop is not active\n");
		goto out;
	}

	if (strcmp(info->desc->chg_name, "primary_chg") == 0) {
		/* Check IBUS ADC */
		ret = rt9466_get_adc(info, RT9466_ADC_IBUS, &adc_ibus);
		if (ret < 0) {
			hwlog_err("get ibus fail\n");
			goto out;
		}
		if (adc_ibus < 100) { /* 100mA */
			ret = rt9466_toggle_cfo(info);
			goto out;
		}
	}
	schedule_work(&info->aicr_work);
	return ret;
out:
	/* Enable MIVR IRQ */
	schedule_delayed_work(&info->mivr_unmask_dwork, msecs_to_jiffies(200));
	return ret;
}

static int rt9466_chg_aicr_irq_handler(struct rt9466_info *info)
{
	return 0;
}

static int rt9466_chg_treg_irq_handler(struct rt9466_info *info)
{
	return 0;
}

static int rt9466_chg_vsysuv_irq_handler(struct rt9466_info *info)
{
	return 0;
}

static int rt9466_chg_vsysov_irq_handler(struct rt9466_info *info)
{
	return 0;
}

static int rt9466_chg_vbatov_irq_handler(struct rt9466_info *info)
{
	return 0;
}

static int rt9466_chg_vbusov_irq_handler(struct rt9466_info *info)
{
	return 0;
}

static int rt9466_ts_batcold_irq_handler(struct rt9466_info *info)
{
	return 0;
}

static int rt9466_ts_batcool_irq_handler(struct rt9466_info *info)
{
	return 0;
}

static int rt9466_ts_batwarm_irq_handler(struct rt9466_info *info)
{
	return 0;
}

static int rt9466_ts_bathot_irq_handler(struct rt9466_info *info)
{
	return 0;
}

static int rt9466_ts_statci_irq_handler(struct rt9466_info *info)
{
	return 0;
}

static int rt9466_chg_faulti_irq_handler(struct rt9466_info *info)
{
	return 0;
}

static int rt9466_chg_statci_irq_handler(struct rt9466_info *info)
{
	return 0;
}

static int rt9466_chg_tmri_irq_handler(struct rt9466_info *info)
{
	return 0;
}

static int rt9466_chg_batabsi_irq_handler(struct rt9466_info *info)
{
	return 0;
}

static int rt9466_chg_adpbadi_irq_handler(struct rt9466_info *info)
{
	return 0;
}

static int rt9466_chg_rvpi_irq_handler(struct rt9466_info *info)
{
	return 0;
}

static int rt9466_otpi_irq_handler(struct rt9466_info *info)
{
	return 0;
}

static int rt9466_chg_aiclmeasi_irq_handler(struct rt9466_info *info)
{
	if (!info)
		return -EINVAL;

	rt9466_irq_set_flag(info, &info->irq_flag[RT9466_IRQIDX_CHG_IRQ2],
		RT9466_MASK_CHG_AICLMEASI);
	wake_up_interruptible(&info->wait_queue);

	return 0;
}

static int rt9466_chg_ichgmeasi_irq_handler(struct rt9466_info *info)
{
	return 0;
}

static int rt9466_wdtmri_irq_handler(struct rt9466_info *info)
{
	int ret;

	ret = rt9466_kick_wdt();
	if (ret < 0)
		hwlog_err("kick wdt fail\n");

	return ret;
}

static int rt9466_ssfinishi_irq_handler(struct rt9466_info *info)
{
	return 0;
}

static int rt9466_chg_rechgi_irq_handler(struct rt9466_info *info)
{
	return 0;
}

static int rt9466_chg_termi_irq_handler(struct rt9466_info *info)
{
	return 0;
}

static int rt9466_chg_ieoci_irq_handler(struct rt9466_info *info)
{
	return 0;
}

static int rt9466_adc_donei_irq_handler(struct rt9466_info *info)
{
	return 0;
}

static int rt9466_pumpx_donei_irq_handler(struct rt9466_info *info)
{
	return 0;
}

static int rt9466_bst_batuvi_irq_handler(struct rt9466_info *info)
{
	return 0;
}

static int rt9466_bst_midovi_irq_handler(struct rt9466_info *info)
{
	return 0;
}

static int rt9466_bst_olpi_irq_handler(struct rt9466_info *info)
{
	return 0;
}

struct irq_mapping_tbl {
	const char *name;
	int (*hdlr)(struct rt9466_info *info);
};

#define RT9466_IRQ_MAPPING(_name) \
	{.name = #_name, .hdlr = rt9466_ ## _name ## _irq_handler}

static const struct irq_mapping_tbl rt9466_irq_mapping_tbl[] = {
	RT9466_IRQ_MAPPING(reserved),
	RT9466_IRQ_MAPPING(reserved),
	RT9466_IRQ_MAPPING(reserved),
	RT9466_IRQ_MAPPING(reserved),
	RT9466_IRQ_MAPPING(chg_treg),
	RT9466_IRQ_MAPPING(chg_aicr),
	RT9466_IRQ_MAPPING(chg_mivr),
	RT9466_IRQ_MAPPING(pwr_rdy),
	RT9466_IRQ_MAPPING(reserved),
	RT9466_IRQ_MAPPING(reserved),
	RT9466_IRQ_MAPPING(reserved),
	RT9466_IRQ_MAPPING(reserved),
	RT9466_IRQ_MAPPING(chg_vsysuv),
	RT9466_IRQ_MAPPING(chg_vsysov),
	RT9466_IRQ_MAPPING(chg_vbatov),
	RT9466_IRQ_MAPPING(chg_vbusov),
	RT9466_IRQ_MAPPING(reserved),
	RT9466_IRQ_MAPPING(reserved),
	RT9466_IRQ_MAPPING(reserved),
	RT9466_IRQ_MAPPING(reserved),
	RT9466_IRQ_MAPPING(ts_batcold),
	RT9466_IRQ_MAPPING(ts_batcool),
	RT9466_IRQ_MAPPING(ts_batwarm),
	RT9466_IRQ_MAPPING(ts_bathot),
	RT9466_IRQ_MAPPING(ts_statci),
	RT9466_IRQ_MAPPING(chg_faulti),
	RT9466_IRQ_MAPPING(chg_statci),
	RT9466_IRQ_MAPPING(chg_tmri),
	RT9466_IRQ_MAPPING(chg_batabsi),
	RT9466_IRQ_MAPPING(chg_adpbadi),
	RT9466_IRQ_MAPPING(chg_rvpi),
	RT9466_IRQ_MAPPING(otpi),
	RT9466_IRQ_MAPPING(chg_aiclmeasi),
	RT9466_IRQ_MAPPING(chg_ichgmeasi),
	RT9466_IRQ_MAPPING(reserved),
	RT9466_IRQ_MAPPING(wdtmri),
	RT9466_IRQ_MAPPING(ssfinishi),
	RT9466_IRQ_MAPPING(chg_rechgi),
	RT9466_IRQ_MAPPING(chg_termi),
	RT9466_IRQ_MAPPING(chg_ieoci),
	RT9466_IRQ_MAPPING(adc_donei),
	RT9466_IRQ_MAPPING(pumpx_donei),
	RT9466_IRQ_MAPPING(reserved),
	RT9466_IRQ_MAPPING(reserved),
	RT9466_IRQ_MAPPING(reserved),
	RT9466_IRQ_MAPPING(bst_batuvi),
	RT9466_IRQ_MAPPING(bst_midovi),
	RT9466_IRQ_MAPPING(bst_olpi),
};

static inline int rt9466_enable_irqrez(struct rt9466_info *info, bool en)
{
	int ret;

	hwlog_debug("en = %d\n", en);
	if (en)
		ret = rt9466_set_bit(info, RT9466_REG_CHG_CTRL13,
			RT9466_MASK_IRQ_REZ);
	else
		ret = rt9466_clr_bit(info, RT9466_REG_CHG_CTRL13,
			RT9466_MASK_IRQ_REZ);

	return ret;
}

static int __rt9466_irq_handler(struct rt9466_info *info)
{
	int ret;
	int i;
	int j;
	u8 evt[RT9466_IRQIDX_MAX] = { 0 };
	u8 mask[RT9466_IRQIDX_MAX] = { 0 };
	u8 stat[RT9466_IRQSTAT_MAX] = { 0 };

	/* Read event and skip CHG_IRQ3 */
	ret = rt9466_i2c_block_read(info, RT9466_REG_CHG_IRQ1, 2, &evt[3]);
	if (ret < 0) {
		hwlog_err("read evt fail\n");
		goto i2c_read_error;
	}

	ret = rt9466_i2c_block_read(info, RT9466_REG_CHG_STATC, 3, evt);
	if (ret < 0) {
		hwlog_err("read stat fail\n");
		goto i2c_read_error;
	}

	/* Read mask */
	ret = rt9466_i2c_block_read(info, RT9466_REG_CHG_STATC_CTRL,
		ARRAY_SIZE(mask), mask);
	if (ret < 0) {
		hwlog_err("read mask fail\n");
		goto i2c_read_error;
	}

	/* Store/Update stat */
	memcpy(stat, info->irq_stat, RT9466_IRQSTAT_MAX);

	for (i = 0; i < RT9466_IRQIDX_MAX; i++) {
		evt[i] &= ~mask[i];
		if (i < RT9466_IRQSTAT_MAX) {
			info->irq_stat[i] = evt[i];
			evt[i] ^= stat[i];
		}
		for (j = 0; j < 8; j++) {
			if (!(evt[i] & (BIT_TRUE << j)))
				continue;
			if (rt9466_irq_mapping_tbl[i * 8 + j].hdlr)
				(rt9466_irq_mapping_tbl[i * 8 + j].hdlr)(info);
		}
	}

i2c_read_error:
	return ret;
}

/**********************************************************
 *  Function:       rt9466_irq_work
 *  Discription:    handler for chargerIC fault irq in charging process
 *  Parameters:   work:chargerIC fault interrupt workqueue
 *  return value:  NULL
 **********************************************************/
static void rt9466_irq_work(struct work_struct *work)
{
	struct rt9466_info *info =
	    container_of(work, struct rt9466_info, irq_work);
	int ret;

	msleep(100); /* sleep 100ms before irq handle */
	ret = __rt9466_irq_handler(info);
	ret = rt9466_enable_irqrez(info, true);
	if (ret < 0)
		hwlog_err("en irqrez fail\n");

	if (info->irq_active == 0) {
		info->irq_active = 1;
		enable_irq(info->irq);
	}
}

static irqreturn_t rt9466_irq_handler(int irq, void *data)
{
	struct rt9466_info *info = (struct rt9466_info *)data;

	if (info->irq_active == 1) {
		info->irq_active = 0;
		disable_irq_nosync(info->irq);
		schedule_work(&info->irq_work);
	} else {
		hwlog_info("The irq is not enable,do nothing!\n");
	}

	return IRQ_HANDLED;
}

static int rt9466_irq_register(struct rt9466_info *info)
{
	int ret;
	int len;
	char *name = NULL;

	/* request gpio */
	len = strlen(info->desc->chg_name);
	name = devm_kzalloc(info->dev, len + IRQ_GPIO_NAME_LEN, GFP_KERNEL);
	if (!name)
		return -ENOMEM;

	snprintf(name,  len + IRQ_GPIO_NAME_LEN, "%s_irq_gpio",
		info->desc->chg_name);
	ret = devm_gpio_request_one(info->dev, info->intr_gpio, GPIOF_IN, name);
	if (ret < 0) {
		hwlog_err("gpio request fail\n");
		return ret;
	}

	ret = gpio_to_irq(info->intr_gpio);
	if (ret < 0) {
		hwlog_err("irq mapping fail\n");
		return ret;
	}
	info->irq = ret;
	hwlog_info("irq = %d\n", info->irq);

	/* Request threaded IRQ */
	name = devm_kzalloc(info->dev, len + IRQ_NAME_LEN, GFP_KERNEL);
	snprintf(name, len + IRQ_NAME_LEN, "%s_irq", info->desc->chg_name);
	ret = devm_request_threaded_irq(info->dev, info->irq, NULL,
		rt9466_irq_handler, IRQF_TRIGGER_FALLING | IRQF_ONESHOT, name,
		info);
	if (ret < 0) {
		hwlog_err("request thread irq fail\n");
		return ret;
	}
	device_init_wakeup(info->dev, true);

	return 0;
}

static inline int rt9466_irq_init(struct rt9466_info *info)
{
	return rt9466_i2c_block_write(info, RT9466_REG_CHG_STATC_CTRL,
		ARRAY_SIZE(info->irq_mask), info->irq_mask);
}

static bool rt9466_is_hw_exist(struct rt9466_info *info)
{
	int vendor_id;
	u8 chip_rev;
	int vendor_id_ex;

	vendor_id = i2c_smbus_read_byte_data(info->client,
		RT9466_REG_DEVICE_ID);
	if (vendor_id < 0)
		return false;

	vendor_id_ex = i2c_smbus_read_byte_data(info->client,
		RT9466_REG_DEVICE_ID_EX);
	if (vendor_id_ex < 0)
		return false;

	vendor_id = (u8)vendor_id & 0xFF;
	chip_rev = (u8)vendor_id & 0x0F;
	vendor_id_ex = (u8)vendor_id_ex & 0xFF;

	if (vendor_id_ex != RT9466_VENDOR_ID_EX) {
		hwlog_err("vendor id is incorrect vid: 0x%02X\n",
			vendor_id_ex);
		return false;
	}

	hwlog_info("chip_rev = 0x%02X\n", chip_rev);
	info->chip_rev = chip_rev;

	return true;
}

static int rt9466_set_safety_timer(struct rt9466_info *info, u32 hr)
{
	u8 reg_st;

	reg_st = rt9466_closest_reg_via_tbl(rt9466_safety_timer,
		ARRAY_SIZE(rt9466_safety_timer), hr);

	hwlog_info("time = %d(0x%02X)\n", hr, reg_st);

	return rt9466_i2c_update_bits(info, RT9466_REG_CHG_CTRL12,
		reg_st << RT9466_SHIFT_WT_FC, RT9466_MASK_WT_FC);
}

static int rt9466_enable_wdt(struct rt9466_info *info, bool en)
{
	int ret;

	hwlog_info("en = %d\n", en);
	if (en)
		ret = rt9466_set_bit(info, RT9466_REG_CHG_CTRL13,
			RT9466_MASK_WDT_EN);
	else
		ret = rt9466_clr_bit(info, RT9466_REG_CHG_CTRL13,
			RT9466_MASK_WDT_EN);

	return ret;
}

static int rt9466_select_input_current_limit(struct rt9466_info *info,
	enum rt9466_iin_limit_sel sel)
{
	hwlog_info("sel = %d\n", sel);
	return rt9466_i2c_update_bits(info, RT9466_REG_CHG_CTRL2,
		(u8)sel << RT9466_SHIFT_IINLMTSEL, RT9466_MASK_IINLMTSEL);
}

static int rt9466_enable_hidden_mode(struct rt9466_info *info, bool en)
{
	int ret = 0;

	mutex_lock(&info->hidden_mode_lock);

	if (en) {
		if (info->hidden_mode_cnt == 0) {
			ret = rt9466_i2c_block_write(info, 0x70,
				ARRAY_SIZE(rt9466_val_en_hidden_mode),
				rt9466_val_en_hidden_mode);
			if (ret < 0)
				goto i2c_read_error;
		}
		info->hidden_mode_cnt++;
	} else {
		if (info->hidden_mode_cnt == 1) /* last one */
			ret = rt9466_i2c_write_byte(info, 0x70, 0x00);
		info->hidden_mode_cnt--;
		if (ret < 0)
			goto i2c_read_error;
	}
	hwlog_debug("en = %d\n", en);
	goto out;

i2c_read_error:
	hwlog_err("en = %d fail\n", en);
out:
	mutex_unlock(&info->hidden_mode_lock);
	return ret;
}

static int rt9466_set_iprec(struct rt9466_info *info, u32 iprec)
{
	u8 reg_iprec;

	reg_iprec = rt9466_closest_reg(RT9466_IPREC_MIN, RT9466_IPREC_MAX,
		RT9466_IPREC_STEP, iprec);

	hwlog_info("iprec = %d(0x%02X)\n", iprec, reg_iprec);

	return rt9466_i2c_update_bits(info, RT9466_REG_CHG_CTRL8,
		reg_iprec << RT9466_SHIFT_IPREC, RT9466_MASK_IPREC);
}

static int rt9466_post_init(struct rt9466_info *info)
{
	int ret;

	rt9466_enable_hidden_mode(info, true);

	/* Disable PSK mode */
	ret = rt9466_clr_bit(info, RT9466_REG_CHG_HIDDEN_CTRL9, 0x80);
	if (ret < 0) {
		hwlog_err("disable skip mode fail\n");
		goto out;
	}

	/* Disable TS auto sensing */
	ret = rt9466_clr_bit(info, RT9466_REG_CHG_HIDDEN_CTRL15, 0x01);
	if (ret < 0)
		goto out;

	/* Set precharge current to 850mA, only do this in normal boot */
	if (info->chip_rev <= RT9466_CHIP_REV_E3) {
		/* Worst case delay: wait auto sensing */
		msleep(200);

		/* default pre charge 850mA */
		ret = rt9466_set_iprec(info, 850);
		if (ret < 0)
			goto out;

		/* Increase Isys drop threshold to 2.5A */
		ret = rt9466_i2c_write_byte(info, RT9466_REG_CHG_HIDDEN_CTRL7,
			0x1c);
		if (ret < 0)
			goto out;
	}

	/* Only revision <= E1 needs the following workaround */
	if (info->chip_rev > RT9466_CHIP_REV_E1)
		goto out;

	/* ICC: modify sensing node, make it more accurate */
	ret = rt9466_i2c_write_byte(info, RT9466_REG_CHG_HIDDEN_CTRL8, 0x00);
	if (ret < 0)
		goto out;

	/* DIMIN level */
	ret = rt9466_i2c_write_byte(info, RT9466_REG_CHG_HIDDEN_CTRL9, 0x86);

out:
	rt9466_enable_hidden_mode(info, false);
	return ret;
}

static inline int rt9466_enable_safety_timer(struct rt9466_info *info,
	bool en)
{
	int ret;

	hwlog_info("en = %d\n", en);
	if (en)
		ret = rt9466_set_bit(info, RT9466_REG_CHG_CTRL12,
			RT9466_MASK_TMR_EN);
	else
		ret = rt9466_clr_bit(info, RT9466_REG_CHG_CTRL12,
			RT9466_MASK_TMR_EN);

	return ret;
}

static int __rt9466_set_ieoc(struct rt9466_info *info, u32 ieoc)
{
	int ret;
	u8 reg_ieoc;

	/* IEOC workaround */
	if (info->ieoc_wkard)
		/* ICHG < 900mA && ieoc set val < 200mA, here comp 50mA */
		ieoc += 50;

	reg_ieoc = rt9466_closest_reg(RT9466_IEOC_MIN, RT9466_IEOC_MAX,
		RT9466_IEOC_STEP, ieoc);

	hwlog_info("ieoc = %d(0x%02X)\n", ieoc, reg_ieoc);

	ret = rt9466_i2c_update_bits(info, RT9466_REG_CHG_CTRL9,
		reg_ieoc << RT9466_SHIFT_IEOC, RT9466_MASK_IEOC);
	if (ret < 0)
		hwlog_err("ret = %d\n", ret);

	/* Store IEOC */
	return rt9466_get_ieoc(info, &info->ieoc);
}

static int rt9466_enable_jeita(struct rt9466_info *info, bool en)
{
	int ret;

	hwlog_info("en = %d\n", en);
	if (en)
		ret = rt9466_set_bit(info, RT9466_REG_CHG_CTRL16,
			RT9466_MASK_JEITA_EN);
	else
		ret = rt9466_clr_bit(info, RT9466_REG_CHG_CTRL16,
			RT9466_MASK_JEITA_EN);

	return ret;
}

static int rt9466_get_charging_status(struct rt9466_info *info,
	enum rt9466_charging_status *chg_stat)
{
	int ret;

	ret = rt9466_i2c_read_byte(info, RT9466_REG_CHG_STAT);
	if (ret < 0)
		return ret;

	*chg_stat = ((u8)ret & RT9466_MASK_CHG_STAT) >> RT9466_SHIFT_CHG_STAT;

	return 0;
}

static int __rt9466_set_ichg(struct rt9466_info *info, u32 ichg)
{
	int ret;
	u8 reg_ichg;

	reg_ichg = rt9466_closest_reg(RT9466_ICHG_MIN, RT9466_ICHG_MAX,
		RT9466_ICHG_STEP, ichg);

	hwlog_info("ichg = %d(0x%02X)\n", ichg, reg_ichg);

	ret = rt9466_i2c_update_bits(info, RT9466_REG_CHG_CTRL7,
		reg_ichg << RT9466_SHIFT_ICHG, RT9466_MASK_ICHG);

	/* Make IEOC accurate */
	if (ichg < 900 && !info->ieoc_wkard) { /* 900mA */
		ret = __rt9466_set_ieoc(info, info->ieoc + 50);
		info->ieoc_wkard = true;
	} else if (ichg >= 900 && info->ieoc_wkard) {
		info->ieoc_wkard = false;
		ret = __rt9466_set_ieoc(info, info->ieoc - 50);
	}

	return ret;
}

static int rt9466_set_ircmp_resistor(struct rt9466_info *info, u32 uohm)
{
	u8 reg_resistor;

	reg_resistor = rt9466_closest_reg(RT9466_IRCMP_RES_MIN,
		RT9466_IRCMP_RES_MAX, RT9466_IRCMP_RES_STEP, uohm);

	hwlog_info("resistor = %d(0x%02X)\n", uohm, reg_resistor);

	return rt9466_i2c_update_bits(info, RT9466_REG_CHG_CTRL18,
		reg_resistor << RT9466_SHIFT_IRCMP_RES, RT9466_MASK_IRCMP_RES);
}

static int rt9466_set_ircmp_vclamp(struct rt9466_info *info, u32 uv)
{
	u8 reg_vclamp;

	reg_vclamp = rt9466_closest_reg(RT9466_IRCMP_VCLAMP_MIN,
		RT9466_IRCMP_VCLAMP_MAX, RT9466_IRCMP_VCLAMP_STEP, uv);

	hwlog_info("vclamp = %d(0x%02X)\n", uv, reg_vclamp);

	return rt9466_i2c_update_bits(info, RT9466_REG_CHG_CTRL18,
		reg_vclamp << RT9466_SHIFT_IRCMP_VCLAMP,
		RT9466_MASK_IRCMP_VCLAMP);
}

static int rt9466_enable_irq_pulse(struct rt9466_info *info, bool en)
{
	int ret;

	hwlog_info("en = %d\n", en);
	if (en)
		ret = rt9466_set_bit(info, RT9466_REG_CHG_CTRL1,
			RT9466_MASK_IRQ_PULSE);
	else
		ret = rt9466_clr_bit(info, RT9466_REG_CHG_CTRL1,
			RT9466_MASK_IRQ_PULSE);

	return ret;
}

static inline int rt9466_get_irq_number(struct rt9466_info *info,
	const char *name)
{
	int i;

	if (!name) {
		hwlog_err("null name\n");
		return -EINVAL;
	}

	for (i = 0; i < ARRAY_SIZE(rt9466_irq_mapping_tbl); i++) {
		if (!strcmp(name, rt9466_irq_mapping_tbl[i].name))
			return i;
	}

	return -EINVAL;
}

static inline const char *rt9466_get_irq_name(struct rt9466_info *info,
	int irqnum)
{
	if (irqnum >= 0 && irqnum < ARRAY_SIZE(rt9466_irq_mapping_tbl))
		return rt9466_irq_mapping_tbl[irqnum].name;

	return "not found";
}

static inline void rt9466_irq_mask(struct rt9466_info *info, int irqnum)
{
	hwlog_debug(" irq = %d, %s\n", irqnum,
		rt9466_get_irq_name(info, irqnum));
	info->irq_mask[irqnum / 8] |= (BIT_TRUE << (irqnum % 8));
}

static inline void rt9466_irq_unmask(struct rt9466_info *info, int irqnum)
{
	hwlog_debug("irq = %d, %s\n", irqnum,
		rt9466_get_irq_name(info, irqnum));
	info->irq_mask[irqnum / 8] &= ~(BIT_TRUE << (irqnum % 8));
}


static int rt9466_parse_dt(struct rt9466_info *info, struct device *dev)
{
	int ret;
	int irq_cnt = 0;
	struct rt9466_desc *desc = NULL;
	struct device_node *np = dev->of_node;
	const char *name = NULL;
	char *ceb_name = NULL;
	int irqnum;
	int len;

	if (!np) {
		hwlog_err("no device node\n");
		return -EINVAL;
	}

	info->desc = &rt9466_default_desc;

	desc = devm_kzalloc(dev, sizeof(*desc), GFP_KERNEL);
	if (!desc)
		return -ENOMEM;
	memcpy(desc, &rt9466_default_desc, sizeof(*desc));

	if (of_property_read_string(np, "charger_name", &desc->chg_name) < 0)
		hwlog_err("no charger name\n");

	info->intr_gpio = of_get_named_gpio(np, "rt,intr_gpio", 0);
	if (!gpio_is_valid(info->intr_gpio)) {
		hwlog_err("get intr_gpio fail\n");
		return -1;
	}
	hwlog_info("intr_gpio is %d\n", info->intr_gpio);

	info->ceb_gpio = of_get_named_gpio(np, "rt,ceb_gpio", 0);
	if (!gpio_is_valid(info->ceb_gpio)) {
		hwlog_err("get ceb_gpio fail\n");
		return -1;
	}
	hwlog_info("ceb_gpiois %d\n", info->ceb_gpio);

	/* request gpio */
	len = strlen(info->desc->chg_name);
	ceb_name = devm_kzalloc(info->dev, len + CEB_GPIO_NAME_LEN,
		GFP_KERNEL);
	snprintf(ceb_name,  len + CEB_GPIO_NAME_LEN, "%s_ceb_gpio",
		info->desc->chg_name);
	ret = devm_gpio_request_one(info->dev, info->ceb_gpio,
		GPIOF_DIR_OUT, ceb_name);
	if (ret < 0) {
		hwlog_err("ceb gpio request fail\n");
		return ret;
	}

	if (of_property_read_u32(np, "regmap_represent_slave_addr",
		&desc->regmap_represent_slave_addr) < 0)
		hwlog_err("no regmap slave addr\n");

	if (of_property_read_string(np, "regmap_name",
		&(desc->regmap_name)) < 0)
		hwlog_err("no regmap name\n");

	if (of_property_read_u32(np, "ichg", &desc->ichg) < 0)
		hwlog_err("no ichg\n");

	if (of_property_read_u32(np, "aicr", &desc->aicr) < 0)
		hwlog_err("no aicr\n");

	if (of_property_read_u32(np, "mivr", &desc->mivr) < 0)
		hwlog_err("no mivr\n");

	if (of_property_read_u32(np, "cv", &desc->cv) < 0)
		hwlog_err("no cv\n");

	if (of_property_read_u32(np, "ieoc", &desc->ieoc) < 0)
		hwlog_err("no ieoc\n");

	if (of_property_read_u32(np, "safety_timer", &desc->safety_timer) < 0)
		hwlog_err("no safety timer\n");

	if (of_property_read_u32(np, "ircmp_resistor",
		&desc->ircmp_resistor) < 0)
		hwlog_err("no ircmp resistor\n");

	if (of_property_read_u32(np, "ircmp_vclamp", &desc->ircmp_vclamp) < 0)
		hwlog_err("no ircmp vclamp\n");

	if (of_property_read_u32(np, "hiz_iin_limit",
			&(desc->hiz_iin_limit)) < 0) {
		desc->hiz_iin_limit = 0;
		hwlog_err("no hiz_iin_limit\n");
	}
	hwlog_info("hiz_iin_limit = %d\n", desc->hiz_iin_limit);

	if (of_property_read_u32(np, "custom_cv", &g_rt9466_cv) < 0) {
		g_rt9466_cv = 0;
		hwlog_err("no custom_cv\n");
	}
	hwlog_info("custom_cv=%d\n", g_rt9466_cv);

	desc->en_te = of_property_read_bool(np, "en_te");
	desc->en_wdt = of_property_read_bool(np, "en_wdt");
	desc->en_irq_pulse = of_property_read_bool(np, "en_irq_pulse");
	desc->en_jeita = of_property_read_bool(np, "en_jeita");
	desc->ceb_invert = of_property_read_bool(np, "ceb_invert");

	while (true) {
		ret = of_property_read_string_index(np, "interrupt-names",
			irq_cnt, &name);
		if (ret < 0)
			break;
		irq_cnt++;
		irqnum = rt9466_get_irq_number(info, name);
		if (irqnum >= 0)
			rt9466_irq_unmask(info, irqnum);
	}

	info->desc = desc;

	return 0;
}

/* =========================================================== */
/* Released interfaces                                         */
/* =========================================================== */

static int rt9466_enable_hz(int en)
{
	struct rt9466_info *info = i2c_get_clientdata(g_rt9466_i2c);
	int ret;

	if (!info) {
		hwlog_err("info is NULL\n");
		return -1;
	}

	if (huawei_force_hz == 1)
		en = 1;

	hwlog_info("en = %d\n", en);
	if (en)
		ret = rt9466_set_bit(info, RT9466_REG_CHG_CTRL1,
			RT9466_MASK_HZ_EN);
	else
		ret = rt9466_clr_bit(info, RT9466_REG_CHG_CTRL1,
			RT9466_MASK_HZ_EN);

	return ret;
}

static int rt9466_huawei_force_enable_hz(int en)
{
	struct rt9466_info *info = NULL;
	struct rt9466_desc *desc = NULL;
	static int first_in = 1;
	int ret;

	info = i2c_get_clientdata(g_rt9466_i2c);
	if (!info) {
		hwlog_err("info is NULL\n");
		return 0;
	}
	desc = info->desc;
	if (!desc) {
		hwlog_err("desc is NULL\n");
		return 0;
	}

	if (en > 0) {
#ifdef CONFIG_HUAWEI_USB_SHORT_CIRCUIT_PROTECT
		/* hiz iin limit enable flag */
		if (desc->hiz_iin_limit == 1 &&
			is_uscp_hiz_mode() && !is_in_rt_uscp_mode()) {
			hiz_iin_limit_flag = HIZ_IIN_FLAG_TRUE;
			if (first_in) {
				hwlog_info("is_uscp_hiz_mode, enable:%d\n", en);
				first_in = 0;
				/* set inputcurrent to 100mA */
				return rt9466_set_aicr(IINLIM_100);
			} else {
				return 0;
			}
		} else {
#endif
			huawei_force_hz = en;
			ret = rt9466_enable_hz(en);
#ifdef CONFIG_HUAWEI_USB_SHORT_CIRCUIT_PROTECT
		}
#endif
	} else {
		hiz_iin_limit_flag = HIZ_IIN_FLAG_FALSE;
		first_in = 1;
		huawei_force_hz = en;
		ret = rt9466_enable_hz(en);
	}
	return ret;
}

static int rt9466_enable_charging(int en)
{
	int ret;
	int adc_vbus = 0;
	struct rt9466_info *info = i2c_get_clientdata(g_rt9466_i2c);

	if (!info) {
		hwlog_err("info is NULL\n");
		return -1;
	}
	hwlog_info("en = %d\n", en);
	/* Check VBUS_ADC */
	ret = rt9466_get_adc(info, RT9466_ADC_VBUS_DIV5, &adc_vbus);
	if (ret < 0)
		hwlog_err("get vbus fail\n");

	/* set hz/ceb pin for secondary charger */
	if (strcmp(info->desc->chg_name, "secondary_chg") == 0) {
		ret = rt9466_enable_hz(!en);
		if (ret < 0) {
			hwlog_err("set hz of sec chg fail\n");
			return ret;
		}
		if (info->desc->ceb_invert)
			gpio_set_value(info->ceb_gpio, en);
		else
			gpio_set_value(info->ceb_gpio, !en);
	}
	if (adc_vbus >= VBUS_THRESHOLD) {
		ret = rt9466_enable_hz(false);
		if (ret < 0)
			hwlog_info("enable hz fail\n");
	}
	if (en) {
		ret = rt9466_set_bit(info, RT9466_REG_CHG_CTRL2,
			RT9466_MASK_CHG_EN);
	} else {
		ret = rt9466_clr_bit(info, RT9466_REG_CHG_CTRL2,
			RT9466_MASK_CHG_EN);
		if (ret < 0)
			hwlog_info("rt9466 dis_chg fail\n");
		if (adc_vbus < VBUS_THRESHOLD)
			ret = rt9466_enable_hz(true);
	}
	hwlog_info("adc_vbus = %d\n", adc_vbus);
	if (ret < 0)
		hwlog_err("fail\n");
	return ret;
}

static int rt9466_set_aicr(int aicr)
{
	int ret;
	int adc_vbus = 0;
	int mivr = 0;
	int aicl_vth;
	struct rt9466_info *info = i2c_get_clientdata(g_rt9466_i2c);

	if (!info) {
		hwlog_err("info is NULL\n");
		return -1;
	}
	/* check vbus > v_th */
	ret = rt9466_get_adc(info, RT9466_ADC_VBUS_DIV5, &adc_vbus);
	if (ret < 0) {
		hwlog_err("get ibus fail\n");
		return ret;
	}
	ret = rt9466_get_mivr(info, &mivr);
	if (ret < 0)
		return ret;

	/* Check if there's a suitable AICL_VTH */
	aicl_vth = mivr + 200;
	if (adc_vbus > aicl_vth)
		info->aicr_limit = -1;

	/* limit aicr */
	if (info->aicr_limit > 0 && info->aicr_limit < aicr) {
		aicr = info->aicr_limit;
		hwlog_info("aicr limit by %d\n", info->aicr_limit);
	}

	mutex_lock(&info->aicr_lock);
	ret = __rt9466_set_aicr(info, aicr);
	mutex_unlock(&info->aicr_lock);

	return ret;
}

static int rt9466_set_ichg(int ichg)
{
	int ret;
	struct rt9466_info *info = i2c_get_clientdata(g_rt9466_i2c);

	if (!info) {
		hwlog_err("info is NULL\n");
		return -1;
	}
	mutex_lock(&info->ichg_lock);
	mutex_lock(&info->ieoc_lock);
	ret = __rt9466_set_ichg(info, ichg);
	mutex_unlock(&info->ieoc_lock);
	mutex_unlock(&info->ichg_lock);

	return (ret < 0 ? ret : 0);
}

static int rt9466_set_cv(int cv)
{
	u8 reg_cv;
	struct rt9466_info *info = i2c_get_clientdata(g_rt9466_i2c);

	if (!info) {
		hwlog_err("info is NULL\n");
		return -1;
	}

	if ((g_rt9466_cv > MIN_CV) && (cv > g_rt9466_cv)) {
		hwlog_info("set cv to custom_cv=%d\n", g_rt9466_cv);
		cv = g_rt9466_cv;
	}

	reg_cv = rt9466_closest_reg(RT9466_CV_MIN, RT9466_CV_MAX,
		RT9466_CV_STEP, cv);

	hwlog_info("cv = %d(0x%02X)\n", cv, reg_cv);

	return rt9466_i2c_update_bits(info, RT9466_REG_CHG_CTRL4,
		reg_cv << RT9466_SHIFT_CV, RT9466_MASK_CV);
}

static int rt9466_set_mivr(int mivr)
{
	u8 reg_mivr;
	struct rt9466_info *info = i2c_get_clientdata(g_rt9466_i2c);

	if (!info) {
		hwlog_err("info is NULL\n");
		return -1;
	}
	reg_mivr = rt9466_closest_reg(RT9466_MIVR_MIN, RT9466_MIVR_MAX,
		RT9466_MIVR_STEP, mivr);

	hwlog_info("mivr = %d(0x%02X)\n", mivr, reg_mivr);

	return rt9466_i2c_update_bits(info, RT9466_REG_CHG_CTRL6,
		reg_mivr << RT9466_SHIFT_MIVR, RT9466_MASK_MIVR);
}

static int rt9466_set_ieoc(int ieoc)
{
	int ret;
	struct rt9466_info *info = i2c_get_clientdata(g_rt9466_i2c);

	if (!info) {
		hwlog_err("info is NULL\n");
		return -1;
	}
	mutex_lock(&info->ichg_lock);
	mutex_lock(&info->ieoc_lock);
	ret = __rt9466_set_ieoc(info, ieoc);
	mutex_unlock(&info->ieoc_lock);
	mutex_unlock(&info->ichg_lock);

	return (ret < 0 ? ret : 0);
}

static int rt9466_set_boost_voltage(int voltage)
{
	u8 reg_voltage;
	struct rt9466_info *info = i2c_get_clientdata(g_rt9466_i2c);

	if (!info) {
		hwlog_err("info is NULL\n");
		return -1;
	}
	if (voltage < RT9466_BOOST_VOREG_MIN)
		return -1;
	reg_voltage = (voltage - RT9466_BOOST_VOREG_MIN) /
		RT9466_BOOST_VOREG_STEP;
	hwlog_info("boost voltage = %d\n", reg_voltage);

	return rt9466_i2c_update_bits(info, RT9466_REG_CHG_CTRL5,
		reg_voltage << RT9466_SHIFT_BOOST_VOREG,
		RT9466_MASK_BOOST_VOREG);
}

static int rt9466_set_boost_current_limit(int current_limit)
{
	u8 reg_ilimit;
	struct rt9466_info *info = i2c_get_clientdata(g_rt9466_i2c);

	if (!info) {
		hwlog_err("info is NULL\n");
		return -1;
	}

	reg_ilimit = rt9466_closest_reg_via_tbl(rt9466_boost_oc_threshold,
		ARRAY_SIZE(rt9466_boost_oc_threshold), current_limit);

	hwlog_info("boost ilimit = %d(0x%02X)\n", current_limit, reg_ilimit);

	return rt9466_i2c_update_bits(info, RT9466_REG_CHG_CTRL10,
		reg_ilimit << RT9466_SHIFT_BOOST_OC, RT9466_MASK_BOOST_OC);
}

static int rt9466_enable_discharging(int en)
{
	int ret = 0;
	int i;
	int reg_value;
	unsigned int max_retry_cnt = 3;
	struct rt9466_info *info = i2c_get_clientdata(g_rt9466_i2c);

	if (!en)
		return ret;

	/* Set bit2 of reg[0x21] to 1 to enable discharging */
	ret = rt9466_set_bit(info, RT9466_REG_CHG_HIDDEN_CTRL2, 0x04);
	if (ret < 0) {
		hwlog_err("enable discharging fail\n");
		goto out;
	}

	/* Wait for discharging to 0V */
	msleep(200);

	for (i = 0; i < max_retry_cnt; i++) {
		/* Disable discharging */
		ret = rt9466_clr_bit(info, RT9466_REG_CHG_HIDDEN_CTRL2, 0x04);
		if (ret < 0)
			continue;
		reg_value = rt9466_i2c_read_byte(info,
			RT9466_REG_CHG_HIDDEN_CTRL2);
		if (reg_value < 0)
			continue;

		if (((u8)reg_value & 0x04) == 0)
			break;
	}
	if (i == max_retry_cnt)
		hwlog_info("disable discharging fail\n");
out:
	return ret;
}

/* Disable OTG workaround */
static int rt9466_check_vbus_hz_workaround(int disable)
{
	int ret;
	int adc_vbus = 0;
	int vbus_last;
	struct rt9466_info *info = i2c_get_clientdata(g_rt9466_i2c);

	if (!info) {
		hwlog_err("info is NULL\n");
		return -1;
	}
	hwlog_info("en = %d\n", disable);
	while (1) {
		/* Check VBUS_ADC */
		ret = rt9466_get_adc(info, RT9466_ADC_VBUS_DIV5, &adc_vbus);
		if (ret < 0)
			hwlog_err("get vbus fail\n");
		vbus_last = adc_vbus;

		if (adc_vbus < VBUS_POST_THRESHOLD)
			ret = rt9466_enable_hz(true);
		else
			ret = rt9466_enable_hz(false);
		/* Wait for adc_vbus active */
		msleep(40);
		/* Check VBUS ADC */
		ret = rt9466_get_adc(info, RT9466_ADC_VBUS_DIV5, &adc_vbus);
		if (ret < 0)
			hwlog_err("get vbus fail\n");

		hwlog_info("vbus_last = %d, vbus_now = %d\n",
			vbus_last, adc_vbus);

		if (((vbus_last > VBUS_POST_THRESHOLD) &&
			(adc_vbus > VBUS_POST_THRESHOLD)) ||
			((vbus_last < VBUS_POST_THRESHOLD) &&
			(adc_vbus < VBUS_POST_THRESHOLD)))
			break;
	}
	return ret;
}

static int rt9466_enable_otg(int en)
{
	int ret;
	bool en_otg = false;
	u8 hidden_val = en ? 0x00 : 0x0F;
	u8 lg_slew_rate = en ? 0x7c : 0x73;
	struct rt9466_info *info = i2c_get_clientdata(g_rt9466_i2c);

	hwlog_info("en = %d\n", en);

	if (!info) {
		hwlog_err("info is NULL\n");
		return -1;
	}

	if (en) {
		ret = rt9466_enable_hz(false);
		if (ret < 0) {
			hwlog_err("enable hz: %d fail\n", en);
			goto out;
		}
	}

	rt9466_enable_hidden_mode(info, true);

	/* Set OTG_OC */
	ret = rt9466_set_boost_current_limit(info->core_data->otg_curr);
	if (ret < 0) {
		hwlog_err("set current limit fail\n");
		return ret;
	}

	/*
	 * Slow Low side mos Gate driver slew rate
	 * for decline VBUS noise
	 * reg[0x23] = 0x7c after entering OTG mode
	 * reg[0x23] = 0x73 after leaving OTG mode
	 */
	ret = rt9466_i2c_write_byte(info, RT9466_REG_CHG_HIDDEN_CTRL4,
		lg_slew_rate);
	if (ret < 0) {
		hwlog_err("set Low side mos gate drive speed fail\n");
		goto out;
	}

	/* Enable WDT */
	if (en && info->desc->en_wdt) {
		ret = rt9466_enable_wdt(info, true);
		if (ret < 0) {
			hwlog_err("en wdt fail\n");
			goto err_en_otg;
		}
	}

	/* Switch OPA mode */
	if (en)
		ret = rt9466_set_bit(info, RT9466_REG_CHG_CTRL1,
				RT9466_MASK_OPA_MODE);
	else
		ret = rt9466_clr_bit(info, RT9466_REG_CHG_CTRL1,
				RT9466_MASK_OPA_MODE);

	msleep(20); /* delay 20ms after set opa bit */

	if (en) {
		ret = rt9466_i2c_test_bit(info, RT9466_REG_CHG_CTRL1,
			RT9466_SHIFT_OPA_MODE, &en_otg);
		if (ret < 0 || !en_otg) {
			hwlog_err("otg fail\n");
			goto err_en_otg;
		}
	}

	/*
	 * reg[0x25] = 0x00 after entering OTG mode
	 * reg[0x25] = 0x0F after leaving OTG mode
	 */
	ret = rt9466_i2c_write_byte(info, RT9466_REG_CHG_HIDDEN_CTRL6,
		hidden_val);
	if (ret < 0)
		hwlog_err("workaroud fail\n");

	/* discharge when disable_otg */
	if (!en) {
		ret = rt9466_enable_discharging(!en);
		if (ret < 0)
			hwlog_err("discharging fail\n");
	}
	/* Disable WDT */
	if (!en) {
		ret = rt9466_enable_wdt(info, false);
		if (ret < 0)
			hwlog_err("disable wdt fail\n");

		ret = rt9466_check_vbus_hz_workaround(en);
		if (ret < 0)
			hwlog_err("check vbus and hz status fail\n");
	}
	goto en_hidden_mode;

err_en_otg:
	/* Disable WDT */
	ret = rt9466_enable_wdt(info, false);
	if (ret < 0)
		hwlog_err("disable wdt fail\n");

	/* Recover Low side mos Gate slew rate */
	ret = rt9466_i2c_write_byte(info, RT9466_REG_CHG_HIDDEN_CTRL4, 0x73);
	if (ret < 0)
		hwlog_err("recover Low side mos Gate drive speed fail\n");
	ret = -EIO;
out:
	rt9466_enable_hz(true);
en_hidden_mode:
	rt9466_enable_hidden_mode(info, false);
	return ret;
}

static int rt9466_enable_te(int en)
{
	struct rt9466_info *info = i2c_get_clientdata(g_rt9466_i2c);
	int ret;

	if (!info) {
		hwlog_err("info is NULL\n");
		return -1;
	}
	hwlog_info("en = %d\n", en);
	if (en)
		ret = rt9466_set_bit(info, RT9466_REG_CHG_CTRL2,
			RT9466_MASK_TE_EN);
	else
		ret = rt9466_clr_bit(info, RT9466_REG_CHG_CTRL2,
			RT9466_MASK_TE_EN);

	return ret;
}

static int rt9466_set_wdt(int wdt)
{
	int ret;
	u8 reg_wdt;
	struct rt9466_info *info = i2c_get_clientdata(g_rt9466_i2c);

	if (!info) {
		hwlog_err("info is NULL\n");
		return -1;
	}

	if (wdt == WDT_STOP)
		return rt9466_enable_wdt(info, false);
	ret = rt9466_enable_wdt(info, true);
	if (ret < 0)
		return ret;

	reg_wdt = rt9466_closest_reg_via_tbl(rt9466_wdt,
		ARRAY_SIZE(rt9466_wdt), wdt);

	hwlog_info("wdt = %d(0x%02X)\n", wdt, reg_wdt);

	return rt9466_i2c_update_bits(info, RT9466_REG_CHG_CTRL13,
		reg_wdt << RT9466_SHIFT_WDT, RT9466_MASK_WDT);
}

static int rt9466_get_charge_state(unsigned int *state)
{
	int ret;
	u8 chg_fault;
	u8 chg_stat;
	struct rt9466_info *info = i2c_get_clientdata(g_rt9466_i2c);
	enum rt9466_charging_status chg_status = RT9466_CHG_STATUS_READY;

	if (!info) {
		hwlog_err("info is NULL\n");
		return -1;
	}

	ret = rt9466_get_charging_status(info, &chg_status);
	if (ret < 0)
		return ret;

	ret = rt9466_i2c_read_byte(info, RT9466_REG_CHG_STATC);
	if (ret < 0)
		return ret;
	chg_stat = (u8)ret & 0xFF;

	ret = rt9466_i2c_read_byte(info, RT9466_REG_CHG_FAULT);
	if (ret < 0)
		return ret;
	chg_fault = (u8)ret & 0xFF;

	if (chg_status == RT9466_CHG_STATUS_DONE)
		*state |= CHAGRE_STATE_CHRG_DONE;
	if (!(chg_stat & RT9466_MASK_PWR_RDY))
		*state |= CHAGRE_STATE_NOT_PG;
	if (chg_fault & RT9466_MASK_VBUSOV)
		*state |= CHAGRE_STATE_VBUS_OVP;
	if (chg_fault & RT9466_MASK_VBATOV)
		*state |= CHAGRE_STATE_BATT_OVP;

	return 0;
}

static int rt9466_get_vbus(unsigned int *vbus)
{
	int ret;
	int adc_vbus = 0;
	struct rt9466_info *info = i2c_get_clientdata(g_rt9466_i2c);

	if (!info) {
		hwlog_err("rt9466 get vbus para err\n");
		return -1;
	}

	ret = rt9466_get_adc(info, RT9466_ADC_VBUS_DIV5, &adc_vbus);
	if (ret < 0) {
		*vbus = 0;
		return -1;
	}

	hwlog_info("rt9466 get vbus %d\n", adc_vbus);
	if (adc_vbus >= VBUS_FAKE_5V) {
		*vbus = VBUS_FAKE_5V; /* compatible with 9V adapter */
		if (!rt9466_is_support_fcp())
			*vbus = adc_vbus;
	} else {
		*vbus = adc_vbus;
	}

	return 0;
}

static int rt9466_get_ibus(void)
{
	int ret;
	int adc_ibus = 0;
	struct rt9466_info *info = i2c_get_clientdata(g_rt9466_i2c);

	if (!info) {
		hwlog_err("info is NULL\n");
		return -1;
	}
	ret = rt9466_get_adc(info, RT9466_ADC_IBUS, &adc_ibus);
	if (ret < 0)
		return -1;
	return adc_ibus;
}

static int rt9466_kick_wdt(void)
{
	struct rt9466_info *info = i2c_get_clientdata(g_rt9466_i2c);
	enum rt9466_charging_status chg_status = RT9466_CHG_STATUS_READY;

	if (!info) {
		hwlog_err("info is NULL\n");
		return -1;
	}
	/* Any I2C communication can reset watchdog timer */
	return rt9466_get_charging_status(info, &chg_status);
}

static int rt9466_check_input_dpm_state(void)
{
	int ret;
	struct rt9466_info *info = i2c_get_clientdata(g_rt9466_i2c);

	if (!info) {
		hwlog_err("info is NULL\n");
		return -1;
	}
	ret = rt9466_i2c_read_byte(info, RT9466_REG_CHG_STATC);
	if (ret < 0)
		return ret;

	if (((u8)ret & RT9466_MASK_CHG_MIVRM) ||
		((u8)ret & RT9466_MASK_CHG_AICRM))
		return TRUE;
	return FALSE;
}

static int rt9466_check_input_vdpm_state(void)
{
	int ret;
	struct rt9466_info *info = i2c_get_clientdata(g_rt9466_i2c);

	if (!info) {
		hwlog_err("info is NULL\n");
		return -1;
	}
	ret = rt9466_i2c_read_byte(info, RT9466_REG_CHG_STATC);
	if (ret < 0)
		return ret;

	if ((u8)ret & RT9466_MASK_CHG_MIVRM)
		return TRUE;
	return FALSE;
}

static int rt9466_check_input_idpm_state(void)
{
	int ret;
	struct rt9466_info *info = i2c_get_clientdata(g_rt9466_i2c);

	if (!info) {
		hwlog_err("info is NULL\n");
		return -1;
	}
	ret = rt9466_i2c_read_byte(info, RT9466_REG_CHG_STATC);
	if (ret < 0)
		return ret;

	if ((u8)ret & RT9466_MASK_CHG_AICRM)
		return TRUE;
	return FALSE;
}
static int rt9466_get_register_head(char *reg_head, int size, void *dev_data)
{
	char buff[PRINT_BUFF_SIZE] = {0};
	int i;

	if (!reg_head)
		return 0;

	memset(reg_head, 0, size);
	for (i = 0; i < ARRAY_SIZE(rt9466_reg_addr); i++) {
		snprintf(buff, PRINT_BUFF_SIZE, "Reg[0x%2x] ",
			rt9466_reg_addr[i]);
		strncat(reg_head, buff, strlen(buff));
	}
	return 0;
}

static int rt9466_dump_register(char *reg_value, int size, void *dev_data)
{
	int i, ret;
	u32 ichg = 0;
	u32 aicr = 0;
	u32 mivr = 0;
	u32 ieoc = 0;
	bool chg_en = 0;
	int adc_vsys = 0;
	int adc_vbat = 0;
	int adc_ibat = 0;
	int adc_ibus = 0;
	int adc_vbus = 0;
	enum rt9466_charging_status chg_status = RT9466_CHG_STATUS_READY;
	u8 chg_stat;
	u8 chg_ctrl[2] = { 0 }; /* ctrl reg need 2 byte */
	struct rt9466_info *info = i2c_get_clientdata(g_rt9466_i2c);
	char buff[PRINT_BUFF_SIZE] = { 0 };

	if (!info) {
		hwlog_err("info is NULL\n");
		return -1;
	}

	ret = rt9466_get_ichg(info, &ichg);
	ret = rt9466_get_aicr(info, &aicr);
	ret = rt9466_get_mivr(info, &mivr);
	ret = rt9466_is_charging_enable(info, &chg_en);
	ret = rt9466_get_ieoc(info, &ieoc);
	ret = rt9466_get_charging_status(info, &chg_status);
	ret = rt9466_get_adc(info, RT9466_ADC_VSYS, &adc_vsys);
	ret = rt9466_get_adc(info, RT9466_ADC_VBAT, &adc_vbat);
	ret = rt9466_get_adc(info, RT9466_ADC_IBAT, &adc_ibat);
	ret = rt9466_get_adc(info, RT9466_ADC_IBUS, &adc_ibus);
	ret = rt9466_get_adc(info, RT9466_ADC_VBUS_DIV5, &adc_vbus);
	chg_stat = rt9466_i2c_read_byte(info, RT9466_REG_CHG_STATC);
	ret = rt9466_i2c_block_read(info, RT9466_REG_CHG_CTRL1, 2, chg_ctrl);

	if (!reg_value)
		return 0;

	memset(reg_value, 0, size);
	for (i = 0; i < ARRAY_SIZE(rt9466_reg_addr); i++) {
		ret = rt9466_i2c_read_byte(info, rt9466_reg_addr[i]);
		if (ret < 0)
			continue;
		snprintf(buff, PRINT_BUFF_SIZE, "0x%-8x", ret & 0xFF);
		strncat(reg_value, buff, strlen(buff));
	}

	hwlog_info(
		"ICHG = %dmA, AICR = %dmA, MIVR = %dmV, IEOC = %dmA\n",
		ichg, aicr, mivr, ieoc);

	hwlog_info(
	"VSYS = %dmV, VBAT = %dmV, IBAT = %dmA, IBUS = %dmA, VBUS = %dmV\n",
	adc_vsys, adc_vbat, adc_ibat, adc_ibus, adc_vbus);

	hwlog_info(
		"CHG_EN = %d, CHG_STATUS = %s, CHG_STAT = 0x%02X\n",
		chg_en, rt9466_chg_status_name[chg_status], chg_stat);

	hwlog_info("CHG_CTRL1 = 0x%02X, CHG_CTRL2 = 0x%02X\n",
		chg_ctrl[0], chg_ctrl[1]);

	return ret;
}

static int rt9466_run_aicr(struct rt9466_info *info)
{
	int ret;

	/* Disable MIVR IRQ */
	ret = rt9466_set_bit(info, RT9466_REG_CHG_STATC_CTRL,
			RT9466_MASK_CHG_MIVR);
	if (ret < 0)
		hwlog_err("disable MIVR IRQ failed\n");

	ret = __rt9466_run_aicr(info);
	return ret;
}

static int rt9466_enable_shipping_mode(int en)
{
	struct rt9466_info *info = i2c_get_clientdata(g_rt9466_i2c);
	int ret;

	hwlog_info("en = %d\n", en);
	if (en)
		ret = rt9466_set_bit(info, RT9466_REG_CHG_CTRL2,
			RT9466_MASK_SHIPMODE);
	else
		ret = rt9466_clr_bit(info, RT9466_REG_CHG_CTRL2,
			RT9466_MASK_SHIPMODE);

	return ret;
}

static int rt9466_5v_chip_init(void)
{
	int ret;
	u8 chg_ctrl2;
	u8 statc_ctrl;
	struct rt9466_info *info = i2c_get_clientdata(g_rt9466_i2c);

	if (!info) {
		hwlog_err("info is NULL\n");
		return -1;
	}

	ret = rt9466_i2c_read_byte(info, RT9466_REG_CHG_STATC_CTRL);
	if (ret < 0)
		return ret;
	statc_ctrl = ret & 0xff;

	ret = rt9466_i2c_read_byte(info, RT9466_REG_CHG_CTRL1);
	if (ret < 0)
		return ret;
	chg_ctrl2 = (u8)ret & 0xff;
	chg_ctrl2 = (chg_ctrl2 & RT9466_MASK_IINLMTSEL)
		>> RT9466_SHIFT_IINLMTSEL;

	hiz_iin_limit_flag = HIZ_IIN_FLAG_FALSE;
	if (chg_ctrl2 != RT9466_IINLMTSEL_AICR &&
		statc_ctrl != info->irq_mask[RT9466_IRQIDX_CHG_STATC]) {
		hwlog_err("chg_ctrl2 = 0x%02X, statc_ctrl = 0x%02X\n",
			chg_ctrl2, statc_ctrl);
		hwlog_err("[WARNING]: reset happened\n");
		rt9466_init_setting(info);
		rt9466_irq_init(info);
		schedule_work(&info->init_work);
	}

	return 0;
}

static int rt9466_chip_init(struct chip_init_crit *init_crit)
{
	int ret;

	if (!init_crit) {
		hwlog_err("init_crit is null\n");
		return -ENOMEM;
	}
	switch (init_crit->vbus) {
	case ADAPTER_5V:
		ret = rt9466_5v_chip_init();
		break;
	default:
		hwlog_err("init mode err\n");
		ret = -1;
		break;
	}
	return ret;
}
static int rt9466_device_check(void)
{
	int ret;
	u8 vendor_id;
	struct rt9466_info *info = i2c_get_clientdata(g_rt9466_i2c);

	ret = rt9466_i2c_read_byte(info, RT9466_REG_DEVICE_ID);
	if (ret < 0)
		return CHARGE_IC_BAD;

	vendor_id = (u8)ret & 0xF0;
	if (vendor_id != RT9466_VENDOR_ID) {
		hwlog_err("vendor id is incorrect (0x%02X)\n", vendor_id);
		return CHARGE_IC_BAD;
	}
	return CHARGE_IC_GOOD;
}

static struct charge_device_ops rt9466_ops = {
	.set_charger_hiz = rt9466_huawei_force_enable_hz,
	.set_charge_enable = rt9466_enable_charging,
	.set_input_current = rt9466_set_aicr,
	.set_charge_current = rt9466_set_ichg,
	.set_terminal_voltage = rt9466_set_cv,
	.set_dpm_voltage = rt9466_set_mivr,
	.set_terminal_current = rt9466_set_ieoc,
	.set_otg_current = rt9466_set_boost_current_limit,
	.set_otg_enable = rt9466_enable_otg,
	.set_term_enable = rt9466_enable_te,
	.set_watchdog_timer = rt9466_set_wdt,
	.get_charge_state = rt9466_get_charge_state,
	.get_ibus = rt9466_get_ibus,
	.get_vbus = rt9466_get_vbus,
	.reset_watchdog_timer = rt9466_kick_wdt,
	.check_input_dpm_state = rt9466_check_input_dpm_state,
	.check_input_vdpm_state = rt9466_check_input_vdpm_state,
	.check_input_idpm_state = rt9466_check_input_idpm_state,
	.set_batfet_disable = rt9466_enable_shipping_mode,
	.chip_init = rt9466_chip_init,
	.dev_check = rt9466_device_check,
	.turn_on_ico = NULL,
	.set_boost_voltage = rt9466_set_boost_voltage,
};

static struct power_log_ops rt9466_log_ops = {
	.dev_name = "rt9466",
	.dump_log_head = rt9466_get_register_head,
	.dump_log_content = rt9466_dump_register,
};

static struct charger_otg_device_ops rt9466_otg_ops = {
	.dev_name = "rt9466",
	.otg_set_charger_enable = rt9466_enable_charging,
	.otg_set_enable = rt9466_enable_otg,
	.otg_set_current = rt9466_set_boost_current_limit,
	.otg_set_watchdog_timer = rt9466_set_wdt,
	.otg_reset_watchdog_timer = rt9466_kick_wdt,
};

static int rt9466_fcp_init(struct rt9466_info *info)
{
	int ret;
	struct device_node *np = NULL;

	if (!info || !info->dev || !info->dev->of_node) {
		hwlog_err("info is null\n");
		goto fail_init_fcp;
	}

	np = info->dev->of_node;
	ret = of_property_read_u32(np, "fcp_support", &(info->fcp_support));
	if (ret) {
		hwlog_err("fcp_support dts read failed\n");
		goto fail_init_fcp;
	}
	hwlog_info("fcp_support=%d\n", info->fcp_support);

	if (!info->fcp_support)
		goto fail_init_fcp;

	info->gpio_fcp = of_get_named_gpio(np, "gpio_fcp", 0);
	hwlog_info("gpio_fcp=%d\n", info->gpio_fcp);

	if (!gpio_is_valid(info->gpio_fcp)) {
		hwlog_err("gpio is not valid\n");
		goto fail_init_fcp;
	}

	ret = gpio_request(info->gpio_fcp, "gpio_fcp");
	if (ret) {
		hwlog_err("gpio request fail\n");
		goto fail_init_fcp;
	}

	/* gpio_fcp init to input mode */
	ret = gpio_direction_input(info->gpio_fcp);
	if (ret < 0) {
		gpio_free(info->gpio_fcp);
		hwlog_err("gpio set input fail\n");
		goto fail_init_fcp;
	}

	return 0;

fail_init_fcp:
	if (info)
		info->fcp_support = 0;
	return -1;
}

static bool rt9466_isvbus9v(void)
{
	int ret;
	int vbus = 0;
	int try_cnt = 0;

	while (try_cnt < RETRY_MAX3) {
		try_cnt++;
		ret = rt9466_get_vbus(&vbus);
		if (!ret)
			break;

		msleep(DELAY_10MS);
	}

	if (vbus > VOLT_THRES_7P5)
		return true;

	return false;
}

static int rt9466_fcp_reset(void)
{
	struct rt9466_info *di = i2c_get_clientdata(g_rt9466_i2c);

	if (!di) {
		hwlog_err("di is null\n");
		return -EINVAL;
	}

	/* reset gpio to input status */
	gpio_set_value(di->gpio_fcp, GPIO_LOW);
	gpio_direction_input(di->gpio_fcp);

	hwlog_info("fcp reset gpio_fcp\n");
	return 0;
}

static int rt9466_fcp_adapter_detect(void)
{
	int try_cnt = 0;
	int ret;
	struct rt9466_info *di = i2c_get_clientdata(g_rt9466_i2c);

	hwlog_info("fcp detect enter\n");
	if (!di) {
		hwlog_err("di is null\n");
		return -1;
	}

	/*
	 * first:
	 * 1. wait 1.5s after DCP detect
	 * 2. set gpio_fcp to output high
	 * 3. wait 100ms up to 9v
	 */
	msleep(DELAY_1500MS);
	gpio_direction_output(di->gpio_fcp, GPIO_LOW);
	gpio_set_value(di->gpio_fcp, GPIO_HIGH);
	msleep(DELAY_100MS);
	hwlog_info("fcp detect set gpio_fcp = %d\n",
		gpio_get_value(di->gpio_fcp));

	/* second: detect ovp to judge 9v */
	while (try_cnt < RETRY_MAX3) {
		try_cnt++;
		if (rt9466_isvbus9v()) {
			hwlog_info("fcp detect upto9v succ\n");
			return 0;
		}
		msleep(DELAY_50MS);
	}

	/* third: reset when detect 9v failed */
	ret = rt9466_fcp_reset();
	if (ret)
		hwlog_err("fcp reset fail\n");

	hwlog_err("fcp detect upto9v fail\n");
	return -1;
}

static int rt9466_is_support_fcp(void)
{
	struct rt9466_info *di = i2c_get_clientdata(g_rt9466_i2c);

	if (!di) {
		hwlog_err("di is null\n");
		return -EINVAL;
	}

	if (di->fcp_support) {
		hwlog_info("support fcp\n");
		return 0;
	}

	hwlog_err("not support fcp %d\n", di->fcp_support);
	return 1; /* not support */
}

static int rt9466_fcp_reg_read_block(int reg, int *val, int num)
{
	int i;

	if (!val) {
		hwlog_err("val is null\n");
		return -1;
	}

	for (i = 0; i < num; i++)
		val[i] = 0;

	return 0;
}

static int rt9466_fcp_reg_write_block(int reg, const int *val, int num)
{
	return 0;
}

static struct fcp_protocol_ops rt9466_fcp_protocol_ops = {
	.chip_name = "rt9466",
	.reg_read = rt9466_fcp_reg_read_block,
	.reg_write = rt9466_fcp_reg_write_block,
	.detect_adapter = rt9466_fcp_adapter_detect,
	.soft_reset_master = rt9466_fcp_reset,
	.soft_reset_slave = rt9466_fcp_reset,
	.get_master_status = NULL,
	.stop_charging_config = rt9466_fcp_reset,
	.is_accp_charger_type = NULL,
};

static int __rt9466_enable_auto_sensing(struct rt9466_info *info, bool en)
{
	int ret;
	u8 auto_sense = 0;
	u8 exit_hid = 0x00;

	/* enter hidden mode */
	ret = rt9466_device_write(info->client, 0x70,
		ARRAY_SIZE(rt9466_val_en_hidden_mode),
		rt9466_val_en_hidden_mode);
	if (ret < 0)
		return ret;

	ret = rt9466_device_read(info->client, RT9466_REG_CHG_HIDDEN_CTRL15, 1,
		&auto_sense);
	if (ret < 0) {
		hwlog_err("read auto sense fail\n");
		goto out;
	}

	if (en)
		auto_sense &= 0xFE; /* clear bit0 */
	else
		auto_sense |= 0x01; /* set bit0 */
	ret = rt9466_device_write(info->client, RT9466_REG_CHG_HIDDEN_CTRL15, 1,
		&auto_sense);
	if (ret < 0)
		hwlog_err("en = %d fail\n", en);

out:
	return rt9466_device_write(info->client, 0x70, 1, &exit_hid);
}

/*
 * This function is used in shutdown function
 * Use i2c smbus directly
 */
static int rt9466_sw_reset(struct rt9466_info *info)
{
	int ret;
	int adc_vbus = 0;
	bool opa_mode = false;
	u8 evt[RT9466_IRQIDX_MAX] = {0};

	/* Disable auto sensing/Enable HZ,ship mode of secondary charger */
	if (strcmp(info->desc->chg_name, "secondary_chg") == 0) {
		mutex_lock(&info->hidden_mode_lock);
		mutex_lock(&info->io_lock);
		__rt9466_enable_auto_sensing(info, false);
		mutex_unlock(&info->io_lock);
		mutex_unlock(&info->hidden_mode_lock);

		reset_reg_data[0] = 0x14; /* HZ */
		reset_reg_data[1] = 0x83; /* Shipping mode */
	}

	/* Mask all irq */
	mutex_lock(&info->io_lock);
	ret = rt9466_device_write(info->client, RT9466_REG_CHG_STATC_CTRL,
		ARRAY_SIZE(rt9466_irq_maskall), rt9466_irq_maskall);
	if (ret < 0)
		hwlog_err("mask all irq fail\n");

	/* Read event and skip CHG_IRQ3 */
	ret = rt9466_device_read(info->client, RT9466_REG_CHG_IRQ1, 2, &evt[3]);
	if (ret < 0)
		hwlog_err("read evt fail\n");

	/* Reset necessary registers */
	ret = rt9466_device_write(info->client, RT9466_REG_CHG_CTRL1,
		ARRAY_SIZE(reset_reg_data), reset_reg_data);
	if (ret < 0)
		hwlog_err("reset registers fail\n");
	mutex_unlock(&info->io_lock);

	/* Check pwr_rdy and opa_mode */
	ret = rt9466_get_adc(info, RT9466_ADC_VBUS_DIV5, &adc_vbus);
	if (ret < 0)
		hwlog_err("get vbus fail\n");
	ret = rt9466_i2c_test_bit(info, RT9466_REG_CHG_CTRL1,
				RT9466_SHIFT_OPA_MODE, &opa_mode);
	if (ret < 0)
		hwlog_err("i2c test bit fail\n");
	if ((adc_vbus < VBUS_THRESHOLD) && !opa_mode)
		ret = rt9466_enable_hz(true);
	else
		ret = rt9466_enable_hz(false);
	if (ret < 0)
		hwlog_err("operate hiz fail\n");

	return ret;
}

static int rt9466_set_tdeg_eoc(struct rt9466_info *info, u32 tdeg)
{
	int ret;

	ret = rt9466_i2c_update_bits(info, RT9466_REG_CHG_CTRL9,
		tdeg, RT9466_MASK_CHG_TDEG_EOC);
	if (ret < 0)
		hwlog_err("fail\n");

	return ret;
}

static int rt9466_init_setting(struct rt9466_info *info)
{
	int ret;
	struct rt9466_desc *desc = info->desc;
	u8 evt[RT9466_IRQIDX_MAX] = {0};

	/* mask all irq */
	ret = rt9466_i2c_block_write(info, RT9466_REG_CHG_STATC_CTRL,
		ARRAY_SIZE(rt9466_irq_maskall), rt9466_irq_maskall);
	if (ret < 0) {
		hwlog_err("mask all irq fail\n");
		goto err;
	}

	/* Read event and skip CHG_IRQ3 */
	ret = rt9466_i2c_block_read(info, RT9466_REG_CHG_IRQ1, 2, &evt[3]);
	if (ret < 0) {
		hwlog_err("read evt fail\n");
		goto err;
	}

	ret = __rt9466_set_ichg(info, desc->ichg);
	if (ret < 0)
		hwlog_err("set ichg fail\n");

	ret = __rt9466_set_aicr(info, desc->aicr);
	if (ret < 0)
		hwlog_err("set aicr fail\n");

	ret = rt9466_set_mivr(desc->mivr);
	if (ret < 0)
		hwlog_err("set mivr fail\n");

	ret = rt9466_set_cv(desc->cv);
	if (ret < 0)
		hwlog_err("set cv fail\n");

	ret = __rt9466_set_ieoc(info, desc->ieoc);
	if (ret < 0)
		hwlog_err("set ieoc fail\n");

	ret = rt9466_enable_te(desc->en_te);
	if (ret < 0)
		hwlog_err("set te fail\n");

	ret = rt9466_enable_safety_timer(info, false);
	if (ret < 0)
		hwlog_err("disable chg timer fail\n");

	ret = rt9466_set_safety_timer(info, desc->safety_timer);
	if (ret < 0)
		hwlog_err("set fast timer fail\n");

	ret = rt9466_enable_safety_timer(info, true);
	if (ret < 0)
		hwlog_err("enable chg timer fail\n");

	ret = rt9466_enable_wdt(info, desc->en_wdt);
	if (ret < 0)
		hwlog_err("set wdt fail\n");

	ret = rt9466_enable_jeita(info, desc->en_jeita);
	if (ret < 0)
		hwlog_err("disable jeita fail\n");

	ret = rt9466_enable_irq_pulse(info, desc->en_irq_pulse);
	if (ret < 0)
		hwlog_err("set irq pulse fail\n");

	/* Set ircomp according */
	ret = rt9466_set_ircmp_resistor(info, desc->ircmp_resistor);
	if (ret < 0)
		hwlog_err("set ircmp resistor fail\n");

	ret = rt9466_set_ircmp_vclamp(info, desc->ircmp_vclamp);
	if (ret < 0)
		hwlog_err("set ircmp clamp fail\n");

	ret = rt9466_set_tdeg_eoc(info, RT9466_TDEG_EOC_16MS);
	if (ret < 0)
		hwlog_err("set tdeg eoc to 16ms fail\n");

	ret = rt9466_post_init(info);
	if (ret < 0) {
		hwlog_err("workaround fail\n");
		return ret;
	}

	/* Enable HZ mode of secondary charger */
	if (strcmp(info->desc->chg_name, "secondary_chg") == 0) {
		ret = rt9466_enable_hz(true);
		if (ret < 0)
			hwlog_err("hz sec chg fail\n");
	}
err:
	return ret;
}

static void rt9466_init_setting_work_handler(struct work_struct *work)
{
	int ret;
	int retry_cnt = 0;
	int adc_vbus = 0;
	bool opa_mode = false;
	struct rt9466_info *info = (struct rt9466_info *)container_of(work,
		struct rt9466_info, init_work);

	if (!info) {
		hwlog_err("info is NULL\n");
		return;
	}

	do {
		/* Select IINLMTSEL to use AICR */
		ret = rt9466_select_input_current_limit(info,
			RT9466_IINLMTSEL_AICR);
		if (ret < 0) {
			hwlog_err("sel ilmtsel fail\n");
			retry_cnt++;
		}
	} while (retry_cnt < 5 && ret < 0); /* retry max 5 times if sel fail */

	msleep(150); /* after sel current need 150 ms delay */

	/* Check pwr_rdy and opa_mode */
	ret = rt9466_get_adc(info, RT9466_ADC_VBUS_DIV5, &adc_vbus);
	if (ret < 0)
		hwlog_err("get vbus fail\n");
	ret = rt9466_i2c_test_bit(info, RT9466_REG_CHG_CTRL1,
				RT9466_SHIFT_OPA_MODE, &opa_mode);
	if (ret < 0)
		hwlog_err("i2c test bit fail\n");
	if ((adc_vbus < VBUS_THRESHOLD) && !opa_mode)
		ret = rt9466_enable_hz(true);
	else
		ret = rt9466_enable_hz(false);
	retry_cnt = 0;
	do {
		/* Disable hardware ILIM */
		ret = rt9466_enable_ilim(info, false);
		if (ret < 0) {
			hwlog_err("disable ilim fail\n");
			retry_cnt++;
		}
	} while (retry_cnt < 5 && ret < 0);
}

static void rt9466_aicr_work_handler(struct work_struct *work)
{
	int ret;
	struct rt9466_info *info = (struct rt9466_info *)container_of(work,
		struct rt9466_info, aicr_work);

	if (!info) {
		hwlog_err("info is NULL\n");
		return;
	}

	ret = rt9466_run_aicr(info);
	if (ret < 0)
		hwlog_err("run aicr failed\n");

	/* Enable MIVR IRQ */
	schedule_delayed_work(&info->mivr_unmask_dwork, msecs_to_jiffies(200));
}

static int rt9466_disable_auto_pmid(struct rt9466_info *info)
{
	int ret;

	rt9466_enable_hidden_mode(info, true);
	/* disable auto pmid */
	ret = rt9466_clr_bit(info, RT9466_REG_CHG_HIDDEN_CTRL2, 0x10);
	if (ret < 0)
		hwlog_err("disable auto pmid fail\n");
	rt9466_enable_hidden_mode(info, false);
	return ret;
}

/* ========================= */
/* I2C driver function       */
/* ========================= */

static int rt9466_probe(struct i2c_client *client,
	const struct i2c_device_id *dev_id)
{
	int ret;
	int adc_vbus = 0;
	struct rt9466_info *info = NULL;
	struct power_devices_info_data *power_dev_info = NULL;
	bool opa_mode = false;

	info = devm_kzalloc(&client->dev, sizeof(struct rt9466_info),
		GFP_KERNEL);
	if (!info)
		return -ENOMEM;

	mutex_init(&info->io_lock);
	mutex_init(&info->adc_lock);
	mutex_init(&info->irq_lock);
	mutex_init(&info->aicr_lock);
	mutex_init(&info->ichg_lock);
	mutex_init(&info->hidden_mode_lock);
	mutex_init(&info->ieoc_lock);
	info->client = client;
	info->dev = &client->dev;
	info->aicr_limit = -1;
	info->hidden_mode_cnt = 0;
	info->ieoc_wkard = false;
	info->ieoc = 250; /* register default value 250mA */
	memcpy(info->irq_mask, rt9466_irq_maskall, RT9466_IRQIDX_MAX);
	atomic_set(&info->pwr_rdy, 0);

	/* Init wait queue head */
	init_waitqueue_head(&info->wait_queue);

	INIT_WORK(&info->init_work, rt9466_init_setting_work_handler);
	INIT_WORK(&info->irq_work, rt9466_irq_work);
	INIT_WORK(&info->aicr_work, rt9466_aicr_work_handler);
	INIT_DELAYED_WORK(&info->mivr_unmask_dwork,
				rt9466_mivr_unmask_dwork_handler);

	/* Is HW exist */
	if (!rt9466_is_hw_exist(info)) {
		hwlog_err("no exists\n");
		ret = -ENODEV;
		goto rt_9466_err_no_dev;
	}
	i2c_set_clientdata(client, info);
	g_rt9466_i2c = client;

	ret = rt9466_disable_auto_pmid(info);
	if (ret < 0) {
		hwlog_err("disable auto pmid fail\n");
		goto rt_9466_err_no_dev;
	}

	ret = rt9466_parse_dt(info, &client->dev);
	if (ret < 0)
		goto rt_9466_err_parse_dt;

	/* Check adc_vbus and opa_mode */
	ret = rt9466_get_adc(info, RT9466_ADC_VBUS_DIV5, &adc_vbus);
	if (ret < 0)
		hwlog_err("get vbus fail\n");
	ret = rt9466_i2c_test_bit(info, RT9466_REG_CHG_CTRL1,
				RT9466_SHIFT_OPA_MODE, &opa_mode);
	if (ret < 0)
		hwlog_err("i2c test bit fail\n");
	if ((adc_vbus < VBUS_THRESHOLD) && !opa_mode)
		reset_reg_data[0] = RT9466_SW_RESET_HZ;
	else
		reset_reg_data[0] = RT9466_SW_RESET_NO_HZ;

	ret = rt9466_sw_reset(info);
	if (ret < 0)
		hwlog_err("sw reset fail\n");
	reset_reg_data[0] = RT9466_SW_RESET_NO_HZ;

	ret = rt9466_init_setting(info);
	if (ret < 0) {
		hwlog_err("init setting fail\n");
		goto rt_9466_err_init_setting;
	}

	ret = charge_ops_register(&rt9466_ops);
	if (ret < 0) {
		hwlog_err("register charge ops fail!\n");
		goto rt_9466_err_chgops;
	}

	ret = charger_otg_ops_register(&rt9466_otg_ops);
	if (ret) {
		hwlog_err("register charger_otg ops fail\n");
		goto rt_9466_err_chgops;
	}

	info->core_data = charge_core_get_params();
	if (!info->core_data) {
		hwlog_err("info->core_data is NULL\n");
		ret = -EINVAL;
		goto rt_9466_err_core_data;
	}

	/* set OTG boost_voltage */
	ret = rt9466_set_boost_voltage(RT9466_BOOST_VOTGBST);
	if (ret < 0)
		hwlog_err("set RT9466 boost voltage fail\n");

	info->irq_active = 1;
	ret = rt9466_irq_register(info);
	if (ret < 0) {
		hwlog_err("irq register fail\n");
		goto rt_9466_err_irq_register;
	}

	ret = rt9466_irq_init(info);
	if (ret < 0) {
		hwlog_err("irq init fail\n");
		goto rt_9466_err_irq_init;
	}

	if (!rt9466_fcp_init(info))
		fcp_protocol_ops_register(&rt9466_fcp_protocol_ops);

	schedule_work(&info->init_work);

	power_dev_info = power_devices_info_register();
	if (power_dev_info) {
		power_dev_info->dev_name = info->dev->driver->name;
		power_dev_info->dev_id = 0;
		power_dev_info->ver_id = 0;
	}

	hwlog_err("successfully\n");
	rt9466_log_ops.dev_data = (void *)info;
	power_log_ops_register(&rt9466_log_ops);
	return ret;

rt_9466_err_irq_init:
rt_9466_err_irq_register:
rt_9466_err_chgops:
rt_9466_err_init_setting:
rt_9466_err_core_data:
rt_9466_err_parse_dt:
rt_9466_err_no_dev:
	mutex_destroy(&info->io_lock);
	mutex_destroy(&info->adc_lock);
	mutex_destroy(&info->irq_lock);
	mutex_destroy(&info->aicr_lock);
	mutex_destroy(&info->ichg_lock);
	mutex_destroy(&info->hidden_mode_lock);
	mutex_destroy(&info->ieoc_lock);
	devm_kfree(&client->dev, info);
	info = NULL;
	return ret;
}

static int rt9466_remove(struct i2c_client *client)
{
	struct rt9466_info *info = i2c_get_clientdata(client);

	if (info) {
		mutex_destroy(&info->io_lock);
		mutex_destroy(&info->adc_lock);
		mutex_destroy(&info->irq_lock);
		mutex_destroy(&info->aicr_lock);
		mutex_destroy(&info->ichg_lock);
		mutex_destroy(&info->hidden_mode_lock);
		mutex_destroy(&info->ieoc_lock);

		if (info->gpio_fcp)
			gpio_free(info->gpio_fcp);
	}

	return 0;
}

static void rt9466_shutdown(struct i2c_client *client)
{
	int ret;
	struct rt9466_info *info = i2c_get_clientdata(client);

	if (!info) {
		hwlog_err("info is NULL\n");
		return;
	}

	if (info) {
		ret = rt9466_sw_reset(info);
		if (ret < 0)
			hwlog_err("sw reset fail\n");
	}
}

static int rt9466_suspend(struct device *dev)
{
	struct rt9466_info *info = dev_get_drvdata(dev);

	if (device_may_wakeup(dev))
		enable_irq_wake(info->irq);

	return 0;
}

static int rt9466_resume(struct device *dev)
{
	struct rt9466_info *info = dev_get_drvdata(dev);

	if (device_may_wakeup(dev))
		disable_irq_wake(info->irq);

	return 0;
}

static SIMPLE_DEV_PM_OPS(rt9466_pm_ops, rt9466_suspend, rt9466_resume);

static const struct i2c_device_id rt9466_i2c_id[] = {
	{ "rt9466", 0 },
	{}
};
MODULE_DEVICE_TABLE(i2c, rt9466_i2c_id);

static const struct of_device_id rt9466_of_match[] = {
	{
		.compatible = "richtek,rt9466",
		.data = NULL,
	},
	{},
};
MODULE_DEVICE_TABLE(of, rt9466_of_match);

static struct i2c_driver rt9466_i2c_driver = {
	.driver = {
		.name = "rt9466",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(rt9466_of_match),
		.pm = &rt9466_pm_ops,
	},
	.probe = rt9466_probe,
	.remove = rt9466_remove,
	.shutdown = rt9466_shutdown,
	.id_table = rt9466_i2c_id,
};

static int __init rt9466_init(void)
{
	return i2c_add_driver(&rt9466_i2c_driver);
}

static void __exit rt9466_exit(void)
{
	i2c_del_driver(&rt9466_i2c_driver);
}

module_init(rt9466_init);
module_exit(rt9466_exit);


MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("rt9466 charger module driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
