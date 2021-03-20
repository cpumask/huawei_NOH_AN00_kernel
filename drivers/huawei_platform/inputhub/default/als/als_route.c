/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: als route source file
 * Author: linjianpeng <linjianpeng1@huawei.com>
 * Create: 2020-05-25
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <asm/memory.h>
#include <linux/time64.h>
#include <asm/io.h>
#include <asm/memory.h>
#include <asm/cacheflush.h>
#include <linux/semaphore.h>
#include <linux/freezer.h>
#include <linux/of.h>
#include <linux/workqueue.h>
#include <linux/slab.h>
#include "contexthub_route.h"
#include "contexthub_boot.h"
#include "contexthub_debug.h"
#include "sensor_config.h"
#include "sensor_feima.h"
#include "sensor_sysfs.h"
#include "sensor_detect.h"
#include "als_tp_color.h"
#include "als_channel.h"
#include "als_route.h"
#include "als_para_table.h"
#include "als_para_table_ams.h"
#include "als_para_table_ams_tcs3701.h"
#include "als_para_table_ams_tcs3718.h"
#include "als_para_table_ams_tmd3702.h"
#include "als_para_table_ams_tmd3725.h"
#include "als_para_table_ams_tsl2591.h"
#include "als_para_table_avago.h"
#include "als_para_table_liteon.h"
#include "als_para_table_rohm.h"
#include "als_para_table_sensortek.h"
#include "als_para_table_sensortek_stk3638.h"
#include "als_para_table_silergy.h"
#include "als_para_table_st.h"
#include "als_para_table_vishay.h"
#include <securec.h>

#define HALF_LENGTH                   45
#define MAX_ALS_UD_CMD_BUF_ARGC       2
#define ALS_UNDER_TP_RGB_DATA         4
#define ALS_RAW_DATA_COLLECT          5
#define ALS_CALI_DATA_COLLECT         6
#define ALS_UD_SHIFT_NUM              32
#define FEIMA_FORMAT_DEC              10

enum {
	ALS_UD_CMD_START,
	ALS_UD_CMD_SET_ADDR,
	ALS_UD_CMD_BUFFER_UPDATE,
};

struct als_ud_cmd_map_t {
	const char *str;
	int cmd;
};

static struct als_ud_cmd_map_t als_ud_cmd_map[] = {
	{ "ADDR", ALS_UD_CMD_SET_ADDR },
	{ "BUFF", ALS_UD_CMD_BUFFER_UPDATE },
};

static int get_cmd(const char *str)
{
	int i = 0;

	for (; i < sizeof(als_ud_cmd_map) / sizeof(als_ud_cmd_map[0]); ++i) {
		if (str_fuzzy_match(str, als_ud_cmd_map[i].str))
			return als_ud_cmd_map[i].cmd;
	}
	return -1;
}

static DECLARE_WAIT_QUEUE_HEAD(sensorhub_als_block_waitq);

void wake_up_als_ud_block(void)
{
	wake_up_interruptible(&sensorhub_als_block_waitq);
}

static void show_als_debug_bh745(uint32_t id, short *dbg, uint32_t num)
{
	bh1745_als_para_table *table = NULL;

	table = als_get_bh1745_table_by_id(id);
	if (table == NULL)
		return;

	/* cofficient_red[0] */
	dbg[0] = table->bh745_para[0];
	/* cofficient_red[0] */
	dbg[1] = table->bh745_para[1];
	/* cofficient_green[0] */
	dbg[2] = table->bh745_para[3];
	/* cofficient_red[1] */
	dbg[3] = table->bh745_para[2];
	/* cofficient_green[1] */
	dbg[4] = table->bh745_para[4];
}

static void show_als_debug_apds251(uint32_t id, short *dbg, uint32_t num)
{
	apds9251_als_para_table *table = NULL;

	table = als_get_apds9251_table_by_id(id);
	if (table == NULL)
		return;

	/* apds251_para or apds9253_para */
	/* avago_cofficient[1] */
	dbg[1] = table->apds251_para[9];
	/* avago_cofficient[2] */
	dbg[2] = table->apds251_para[10];
	/* avago_cofficient[3] */
	dbg[3] = table->apds251_para[20];
	/* LUX_P */
	dbg[4] = table->apds251_para[2];
	/* LUX_R */
	dbg[5] = table->apds251_para[4];
	/* LUX_Q */
	dbg[6] = table->apds251_para[3];
	/* lux_mix */
	dbg[7] = table->apds251_para[19];
}

static void show_als_debug_apds9999(uint32_t id, short *dbg, uint32_t num)
{
	apds9999_als_para_table *table = NULL;

	table = als_get_apds9999_table_by_id(id);
	if (table == NULL)
		return;

	/* avago_cofficient[1] */
	dbg[1] = table->apds9999_para[9];
	/* avago_cofficient[2] */
	dbg[2] = table->apds9999_para[10];
	/* avago_cofficient[3] */
	dbg[3] = table->apds9999_para[20];
	/* LUX_P */
	dbg[4] = table->apds9999_para[2];
	/* LUX_R */
	dbg[5] = table->apds9999_para[4];
	/* LUX_Q */
	dbg[6] = table->apds9999_para[3];
	/* lux_mix */
	dbg[7] = table->apds9999_para[19];
}

static void show_als_debug_tmd3725(uint32_t id, short *dbg, uint32_t num)
{
	uint32_t k;
	tmd3725_als_para_table *table = NULL;

	table = als_get_tmd3725_table_by_id(id);
	if (table == NULL)
		return;

	/*
	 * tmd3725_para:
	 * 0: atime
	 * 1: again
	 * 2: dgf
	 * 3: c_coef
	 * 4: r_coef
	 * 5: g_coef
	 * 6: b_coef
	 */
	for (k = 0; k < 7; k++)
		dbg[1 + k] = table->tmd3725_para[k];
}

static void show_als_debug_tmd3702(uint32_t id, short *dbg, uint32_t num)
{
	uint32_t k;
	tmd3702_als_para_table *table = NULL;

	table = als_get_tmd3702_table_by_id(id);
	if (table == NULL)
		return;

	/*
	 * tmd3702_para:
	 * 0: atime
	 * 1: again
	 * 2: dgf
	 * 3: c_coef
	 * 4: r_coef
	 * 5: g_coef
	 * 6: b_coef
	 */
	for (k = 0; k < 7; k++)
		dbg[1 + k] = table->tmd3702_para[k];
}

static void show_als_debug_tcs3707(uint32_t id, short *dbg, uint32_t num)
{
	uint32_t k;
	tcs3707_als_para_table *table = NULL;

	table = als_get_tcs3707_table_by_id(id);
	if (table == NULL)
		return;

	/*
	 * tcs3707_para:
	 * 0: atime
	 * 1: again
	 * 2: dgf
	 * 3: c_coef
	 * 4: r_coef
	 * 5: g_coef
	 * 6: b_coef
	 */
	for (k = 0; k < 7; k++)
		dbg[1 + k] = table->tcs3707_para[k];
}

static void show_als_debug_tcs3701(uint32_t id, short *dbg, uint32_t num)
{
	uint32_t k;
	tcs3701_als_para_table *table = NULL;

	table = als_get_tcs3701_table_by_id(id);
	if (table == NULL)
		return;

	/*
	 * tcs3701_para:
	 * 0: atime
	 * 1: again
	 * 2: dgf
	 * 3: c_coef
	 * 4: r_coef
	 * 5: g_coef
	 * 6: b_coef
	 */
	for (k = 0; k < 7; k++)
		dbg[1 + k] = table->tcs3701_para[k];
}

static void show_als_debug_vcnl36658(uint32_t id, short *dbg, uint32_t num)
{
	uint32_t k;
	vcnl36658_als_para_table *table = NULL;

	table = als_get_vcnl36658_table_by_id(id);
	if (table == NULL)
		return;

	for (k = 0; k < 7; k++)
		dbg[1 + k] = table->vcnl36658_para[k];
}

static void show_als_debug_ltr582(uint32_t id, short *dbg, uint32_t num)
{
	uint32_t k;
	ltr582_als_para_table *table = NULL;

	table = als_get_ltr582_table_by_id(id);
	if (table == NULL)
		return;

	/*
	 * ltr582_para:
	 * 3: ad_radio
	 * 4: dc_radio
	 * 5: a_winfac
	 * 6: d_winfac
	 * 7: c_winfac
	 * 8: slope
	 * 9: slope_offset
	 */
	for (k = 0; k < 7; k++)
		dbg[1 + k] = table->ltr582_para[3 + k];
}

static void show_als_debug_tmd2745(uint32_t id, short *dbg, uint32_t num)
{
	uint32_t k;
	tmd2745_als_para_table *table = NULL;

	table = als_get_tmd2745_table_by_id(id);
	if (table == NULL)
		return;

	/*
	 * als_para:
	 * 0: D_factor
	 * 1: B_Coef
	 * 2: C_Coef
	 * 3: D_Coef
	 * 4: is_min_algo
	 * 5: is_auto_gain
	 */
	for (k = 0; k < 6; k++)
		dbg[k] = table->als_para[k];
}

static void show_als_debug_stk32670(uint32_t id, short *dbg, uint32_t num)
{
	int8_t i;
	als_para_normal_table *table = NULL;

	table = als_get_stk32670_table_by_id(id);
	if (table == NULL)
		return;

	for (i = 0; i < num; i++)
		dbg[i] = table->als_para[i + 1];
}

ssize_t als_debug_data_show(int32_t tag, struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct als_platform_data *pf_data = NULL;
	struct als_device_info *dev_info = NULL;
	uint32_t id;
	short dbg[ALS_DBG_PARA_SIZE] = { 0 }; /* als debug para */
	uint32_t num = sizeof(dbg) / sizeof(dbg[0]);

	pf_data = als_get_platform_data(tag);
	dev_info = als_get_device_info(tag);
	if (!pf_data || !dev_info || dev_info->chip_type == ALS_CHIP_NONE)
		return -1;

	if (!buf)
		return 0;

	id = dev_info->table_id;

	hwlog_info("%s chip_type=%d\n", __func__, dev_info->chip_type);

	if (dev_info->chip_type == ALS_CHIP_ROHM_RGB) /* bh745_para */
		show_als_debug_bh745(id, dbg, num);
	else if (dev_info->chip_type == ALS_CHIP_AVAGO_RGB ||
		dev_info->chip_type == ALS_CHIP_APDS9253_RGB)
		show_als_debug_apds251(id, dbg, num);
	else if (dev_info->chip_type == ALS_CHIP_APDS9999_RGB)
		show_als_debug_apds9999(id, dbg, num);
	else if (dev_info->chip_type == ALS_CHIP_AMS_TMD3725_RGB)
		show_als_debug_tmd3725(id, dbg, num);
	else if (dev_info->chip_type == ALS_CHIP_AMS_TMD3702_RGB)
		show_als_debug_tmd3702(id, dbg, num);
	else if (dev_info->chip_type == ALS_CHIP_AMS_TCS3707_RGB)
		show_als_debug_tcs3707(id, dbg, num);
	else if (dev_info->chip_type == ALS_CHIP_AMS_TCS3701_RGB)
		show_als_debug_tcs3701(id, dbg, num);
	else if (dev_info->chip_type == ALS_CHIP_VISHAY_VCNL36658)
		show_als_debug_vcnl36658(id, dbg, num);
	else if (dev_info->chip_type == ALS_CHIP_LITEON_LTR582)
		show_als_debug_ltr582(id, dbg, num);
	else if (dev_info->chip_type == ALS_CHIP_TMD2745)
		show_als_debug_tmd2745(id, dbg, num);
	else if (dev_info->chip_type == ALS_CHIP_STK32670)
		show_als_debug_stk32670(id, dbg, num);

	return snprintf_s(buf, BUF_SIZE, BUF_SIZE - 1,
		"%hd,%hd,%hd,%hd,%hd,%hd,%hd,%hd\n",
		dbg[0], dbg[1], dbg[2], dbg[3], dbg[4], dbg[5],
		dbg[6], dbg[7]);
}

static ssize_t store_als_debug_bh745(struct als_platform_data *pf_data,
	uint32_t id, const short *dbg, uint32_t num, size_t size)
{
	bh1745_als_para_table *table = NULL;

	table = als_get_bh1745_table_by_id(id);
	if (table == NULL)
		return -1;

	/* cofficient_judge */
	table->bh745_para[0] = dbg[0];
	/* cofficient_red[0] */
	table->bh745_para[1] = dbg[1];
	/* cofficient_green[0] */
	table->bh745_para[3] = dbg[2];
	/* cofficient_red[1] */
	table->bh745_para[2] = dbg[3];
	/* cofficient_green[1] */
	table->bh745_para[4] = dbg[4];
	if (memcpy_s(pf_data->als_extend_data, sizeof(pf_data->als_extend_data),
		table->bh745_para, sizeof(table->bh745_para)) != EOK)
		return -1;

	return size;
}

static ssize_t store_als_debug_apds251(struct als_platform_data *pf_data,
	uint32_t id, const short *dbg, uint32_t num, size_t size)
{
	apds9251_als_para_table *table = NULL;

	table = als_get_apds9251_table_by_id(id);
	if (table == NULL)
		return -1;

	/* avago_cofficient[1] */
	table->apds251_para[9] = dbg[1];
	/* avago_cofficient[2] */
	table->apds251_para[10] = dbg[2];
	/* avago_cofficient[3] */
	table->apds251_para[20] = dbg[3];
	/* LUX_P */
	table->apds251_para[2] = dbg[4];
	/* LUX_R */
	table->apds251_para[4] = dbg[5];
	/* LUX_Q */
	table->apds251_para[3] = dbg[6];
	/* lux_mix */
	table->apds251_para[19] = dbg[7];
	if (memcpy_s(pf_data->als_extend_data, sizeof(pf_data->als_extend_data),
		table->apds251_para, sizeof(table->apds251_para)) != EOK)
		return -1;

	return size;
}

static ssize_t store_als_debug_apds9999(struct als_platform_data *pf_data,
	uint32_t id, const short *dbg, uint32_t num, size_t size)
{
	apds9999_als_para_table *table = NULL;

	table = als_get_apds9999_table_by_id(id);
	if (table == NULL)
		return -1;

	/* avago_cofficient[1] */
	table->apds9999_para[9] = dbg[1];
	/* avago_cofficient[2] */
	table->apds9999_para[10] = dbg[2];
	/* avago_cofficient[3] */
	table->apds9999_para[20] = dbg[3];
	/* LUX_P */
	table->apds9999_para[2] = dbg[4];
	/* LUX_R */
	table->apds9999_para[4] = dbg[5];
	/* LUX_Q */
	table->apds9999_para[3] = dbg[6];
	/* lux_mix */
	table->apds9999_para[19] = dbg[7];
	if (memcpy_s(pf_data->als_extend_data, sizeof(pf_data->als_extend_data),
		table->apds9999_para, sizeof(table->apds9999_para)) != EOK)
		return -1;

	return size;
}

static ssize_t store_als_debug_tmd3725(struct als_platform_data *pf_data,
	uint32_t id, const short *dbg, uint32_t num, size_t size)
{
	uint32_t k;
	tmd3725_als_para_table *table = NULL;

	table = als_get_tmd3725_table_by_id(id);
	if (table == NULL)
		return -1;

	/*
	 * tmd3725_para:
	 * 0: atime
	 * 1: again
	 * 2: dgf
	 * 3: c_coef
	 * 4: r_coef
	 * 5: g_coef
	 * 6: b_coef
	 */
	for (k = 0; k < 7; k++)
		table->tmd3725_para[k] = dbg[1 + k];
	if (memcpy_s(pf_data->als_extend_data, sizeof(pf_data->als_extend_data),
		table->tmd3725_para, sizeof(table->tmd3725_para)) != EOK)
		return -1;

	return size;
}

static ssize_t store_als_debug_tmd3702(struct als_platform_data *pf_data,
	uint32_t id, const short *dbg, uint32_t num, size_t size)
{
	uint32_t k;
	tmd3702_als_para_table *table = NULL;

	table = als_get_tmd3702_table_by_id(id);
	if (table == NULL)
		return -1;

	/*
	 * tmd3702_para:
	 * 0: atime
	 * 1: again
	 * 2: dgf
	 * 3: c_coef
	 * 4: r_coef
	 * 5: g_coef
	 * 6: b_coef
	 */
	for (k = 0; k < 7; k++)
		table->tmd3702_para[k] = dbg[1 + k];
	if (memcpy_s(pf_data->als_extend_data, sizeof(pf_data->als_extend_data),
		table->tmd3702_para, sizeof(table->tmd3702_para)) != EOK)
		return -1;

	return size;
}

static ssize_t store_als_debug_tcs3707(struct als_platform_data *pf_data,
	uint32_t id, const short *dbg, uint32_t num, size_t size)
{
	tcs3707_als_para_table *table = NULL;

	table = als_get_tcs3707_table_by_id(id);
	if (table == NULL)
		return -1;

	/* atime */
	table->tcs3707_para[0] = dbg[1];
	/* again */
	table->tcs3707_para[1] = dbg[2];
	/* dgf */
	table->tcs3707_para[2] = dbg[3];
	/* c_coef */
	table->tcs3707_para[3] = dbg[4];
	/* r_coef */
	table->tcs3707_para[4] = dbg[5];
	/* g_coef */
	table->tcs3707_para[5] = dbg[6];
	/* b_coef */
	table->tcs3707_para[6] = dbg[7];
	if (memcpy_s(pf_data->als_extend_data, sizeof(pf_data->als_extend_data),
		table->tcs3707_para, sizeof(table->tcs3707_para)) != EOK)
		return -1;

	return size;
}

static ssize_t store_als_debug_tcs3701(struct als_platform_data *pf_data,
	uint32_t id, const short *dbg, uint32_t num, size_t size)
{
	tcs3701_als_para_table *table = NULL;

	table = als_get_tcs3701_table_by_id(id);
	if (table == NULL)
		return -1;

	/* atime */
	table->tcs3701_para[0] = dbg[1];
	/* again */
	table->tcs3701_para[1] = dbg[2];
	/* dgf */
	table->tcs3701_para[2] = dbg[3];
	/* c_coef */
	table->tcs3701_para[3] = dbg[4];
	/* r_coef */
	table->tcs3701_para[4] = dbg[5];
	/* g_coef */
	table->tcs3701_para[5] = dbg[6];
	/* b_coef */
	table->tcs3701_para[6] = dbg[7];
	if (memcpy_s(pf_data->als_extend_data, sizeof(pf_data->als_extend_data),
		table->tcs3701_para, sizeof(table->tcs3701_para)) != EOK)
		return -1;

	return size;
}

static ssize_t store_als_debug_vcnl36658(struct als_platform_data *pf_data,
	uint32_t id, const short *dbg, uint32_t num, size_t size)
{
	vcnl36658_als_para_table *table = NULL;

	table = als_get_vcnl36658_table_by_id(id);
	if (table == NULL)
		return -1;

	table->vcnl36658_para[0] = dbg[1];
	table->vcnl36658_para[1] = dbg[2];
	table->vcnl36658_para[2] = dbg[3];
	table->vcnl36658_para[3] = dbg[4];
	table->vcnl36658_para[4] = dbg[5];
	table->vcnl36658_para[5] = dbg[6];
	table->vcnl36658_para[6] = dbg[7];
	if (memcpy_s(pf_data->als_extend_data, sizeof(pf_data->als_extend_data),
		table->vcnl36658_para, sizeof(table->vcnl36658_para)) != EOK)
		return -1;

	return size;
}

static ssize_t store_als_debug_ltr582(struct als_platform_data *pf_data,
	uint32_t id, const short *dbg, uint32_t num, size_t size)
{
	ltr582_als_para_table *table = NULL;

	table = als_get_ltr582_table_by_id(id);
	if (table == NULL)
		return -1;

	/* ad_radio */
	table->ltr582_para[3] = dbg[1];
	/* dc_radio */
	table->ltr582_para[4] = dbg[2];
	/* a_winfac */
	table->ltr582_para[5] = dbg[3];
	/* d_winfac */
	table->ltr582_para[6] = dbg[4];
	/* c_winfac */
	table->ltr582_para[7] = dbg[5];
	/* slope */
	table->ltr582_para[8] = dbg[6];
	/* slope_offset */
	table->ltr582_para[9] = dbg[7];
	if (memcpy_s(pf_data->als_extend_data, sizeof(pf_data->als_extend_data),
		table->ltr582_para, sizeof(table->ltr582_para)) != EOK)
		return -1;

	return size;
}

static ssize_t store_als_debug_tmd2745(struct als_platform_data *pf_data,
	uint32_t id, const short *dbg, uint32_t num, size_t size)
{
	tmd2745_als_para_table *table = NULL;

	table = als_get_tmd2745_table_by_id(id);
	if (table == NULL)
		return -1;

	/* D_factor */
	table->als_para[0] = dbg[0];
	/* B_Coef */
	table->als_para[1] = dbg[1];
	/* C_Coef */
	table->als_para[2] = dbg[2];
	/* D_Coef */
	table->als_para[3] = dbg[3];
	/* is_min_algo */
	table->als_para[4] = dbg[4];
	/* is_auto_gain */
	table->als_para[5] = dbg[5];
	if (memcpy_s(pf_data->als_extend_data, sizeof(pf_data->als_extend_data),
		table->als_para, sizeof(table->als_para)) != EOK)
		return -1;

	return size;
}

static ssize_t store_als_debug_stk32670(struct als_platform_data *pf_data,
	uint32_t id, const short *dbg, uint32_t num, size_t size)
{
	int8_t i;
	als_para_normal_table *table = NULL;

	table = als_get_stk32670_table_by_id(id);
	if (table == NULL)
		return -1;

	for (i = 0; i < num; i++)
		table->als_para[i + 1] = dbg[i];

	if (sizeof(table->als_para) > SENSOR_PLATFORM_EXTEND_ALS_DATA_SIZE)
		return -1;

	if (memcpy_s(pf_data->als_extend_data, sizeof(pf_data->als_extend_data),
		table->als_para, sizeof(table->als_para)) != EOK)
		return -1;

	return size;
}

ssize_t als_debug_data_store(int32_t tag, struct device *dev,
	struct device_attribute *attr, const char *buf, size_t size)
{
	struct als_platform_data *pf_data = NULL;
	struct als_device_info *dev_info = NULL;
	uint32_t id;
	short dbg[ALS_DBG_PARA_SIZE] = { 0 }; /* als debug para */
	uint32_t num = sizeof(dbg) / sizeof(dbg[0]);

	pf_data = als_get_platform_data(tag);
	dev_info = als_get_device_info(tag);
	if (!pf_data || !dev_info || dev_info->chip_type == ALS_CHIP_NONE)
		return -1;

	if (!buf)
		return 0;

	id = dev_info->table_id;
	hwlog_info("%s:chip_type=%d\n", __func__, dev_info->chip_type);

	if (sscanf_s(buf, "%hd,%hd,%hd,%hd,%hd,%hd,%hd,%hd", &dbg[0], &dbg[1],
		&dbg[2], &dbg[3], &dbg[4], &dbg[5], &dbg[6],
		&dbg[7]) > 0)
		hwlog_info("%s: get parameter success\n", __func__);
	else
		hwlog_info("%s: get parameter fail\n", __func__);

	switch(dev_info->chip_type) {
	case ALS_CHIP_ROHM_RGB: /* bh745 */
		return store_als_debug_bh745(pf_data, id, dbg, num, size);
	case ALS_CHIP_AVAGO_RGB: /* apds251 */
		return store_als_debug_apds251(pf_data, id, dbg, num, size);
	case ALS_CHIP_APDS9999_RGB:
		return store_als_debug_apds9999(pf_data, id, dbg, num, size);
	case ALS_CHIP_AMS_TMD3725_RGB:
		return store_als_debug_tmd3725(pf_data, id, dbg, num, size);
	case ALS_CHIP_AMS_TMD3702_RGB:
		return store_als_debug_tmd3702(pf_data, id, dbg, num, size);
	case ALS_CHIP_AMS_TCS3707_RGB:
		return store_als_debug_tcs3707(pf_data, id, dbg, num, size);
	case ALS_CHIP_AMS_TCS3701_RGB:
		return store_als_debug_tcs3701(pf_data, id, dbg, num, size);
	case ALS_CHIP_VISHAY_VCNL36658:
		return store_als_debug_vcnl36658(pf_data, id, dbg, num, size);
	case ALS_CHIP_LITEON_LTR582:
		return store_als_debug_ltr582(pf_data, id, dbg, num, size);
	case ALS_CHIP_TMD2745:
		return store_als_debug_tmd2745(pf_data, id, dbg, num, size);
	case ALS_CHIP_STK32670:
		return store_als_debug_stk32670(pf_data, id, dbg, num, size);
	default:
		return size;
	}

	return size;
}

ssize_t als_ud_rgbl_status_show(int32_t tag, struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct als_platform_data *pf_data = NULL;
	struct als_device_info *dev_info = NULL;

	pf_data = als_get_platform_data(tag);
	dev_info = als_get_device_info(tag);
	if (!pf_data || !dev_info)
		return -1;

	get_als_under_tp_calibrate_data_by_tag(tag);
	if (pf_data->als_phone_type != ELE_PHONE_TYPE &&
		pf_data->als_ud_type != ALS_UD_MINUS_TP_RGB)
		return snprintf_s(buf, MAX_STR_SIZE, MAX_STR_SIZE - 1,
			"%d,%d,%d,%d\n", 0, 0, 0, 0);
	else if (dev_info->als_under_tp_cal_data.x == 0 &&
		dev_info->als_under_tp_cal_data.y == 0)
		return snprintf_s(buf, MAX_STR_SIZE, MAX_STR_SIZE - 1,
			"%d,%d,%d,%d\n", 662, 0, 762, 100);

	return snprintf_s(buf, MAX_STR_SIZE, MAX_STR_SIZE - 1, "%d,%d,%d,%d\n",
		dev_info->als_under_tp_cal_data.x - HALF_LENGTH > 0 ?
			dev_info->als_under_tp_cal_data.x - HALF_LENGTH : 0,
		dev_info->als_under_tp_cal_data.y - HALF_LENGTH > 0 ?
			dev_info->als_under_tp_cal_data.y - HALF_LENGTH : 0,
		dev_info->als_under_tp_cal_data.x + HALF_LENGTH,
		dev_info->als_under_tp_cal_data.y + HALF_LENGTH);
}

ssize_t als_rgb_status_store(int32_t tag, struct device *dev,
	struct device_attribute *attr, const char *buf, size_t size)
{
	struct als_device_info *dev_info = NULL;
	int cmd = 0;
	int arg = -1;
	int argc = 0;
	uint64_t para[2];

	dev_info = als_get_device_info(tag);
	if (!dev_info)
		return -1;

	for (; (buf = get_str_begin(buf)) != NULL; buf = get_str_end(buf)) {
		if (!cmd)
			cmd = get_cmd(buf);

		if (get_arg(buf, &arg)) {
			if (argc < MAX_ALS_UD_CMD_BUF_ARGC)
				para[argc++] = arg;
			else
				hwlog_err("too many args, %d will be ignored\n",
					arg);
		}
	}

	switch (cmd) {
	case ALS_UD_CMD_SET_ADDR:
		dev_info->als_rgb_pa_to_sh = para[1];
		send_als_ud_data_to_mcu(tag, SUB_CMD_SET_ALS_PA,
			(const void *)&dev_info->als_rgb_pa_to_sh,
			sizeof(dev_info->als_rgb_pa_to_sh), false);
		break;
	case ALS_UD_CMD_BUFFER_UPDATE:
		send_als_ud_data_to_mcu(tag, SUB_CMD_UPDATE_RGB_DATA,
			(const void *)&para, sizeof(para), false);
		break;
	default:
		hwlog_err("Unspport cmd\n");
		break;
	}
	return size;
}

ssize_t als_ud_rgbl_block_data_show(int32_t tag, struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct als_device_info *dev_info = NULL;
	int ret;
	unsigned long long head;
	unsigned long long left;
	unsigned long long bottom;
	unsigned long long right;

	dev_info = als_get_device_info(tag);
	if ((tag != TAG_ALS) || (!dev_info)) {
		hwlog_err("%s: als under tp no tag %d\n", __func__, tag);
		return -1;
	}

	/* Block node by soft irq */
	dev_info->sem_als_ud_rgbl_block_flag = 1;
	dev_info->als_ud_rgbl_block = 0;
	ret = wait_event_freezable_timeout(sensorhub_als_block_waitq,
		(dev_info->als_ud_rgbl_block != 0), msecs_to_jiffies(100000));
	if (ret <= 0) {
		hwlog_info("soft irq time out\n");
		return snprintf_s(buf, MAX_STR_SIZE, MAX_STR_SIZE - 1,
			"%d,%d,%d\n", 0, 0, 0);
	}

	if (dev_info->als_ud_need_reset) {
		dev_info->als_ud_need_reset = 0;
		head = dev_info->als_under_tp_cal_data.x - HALF_LENGTH > 0 ?
			dev_info->als_under_tp_cal_data.x - HALF_LENGTH : 0;
		left = dev_info->als_under_tp_cal_data.y - HALF_LENGTH > 0 ?
			dev_info->als_under_tp_cal_data.y - HALF_LENGTH : 0;
		bottom = dev_info->als_under_tp_cal_data.x + HALF_LENGTH;
		right = dev_info->als_under_tp_cal_data.y + HALF_LENGTH;
		return snprintf_s(buf, MAX_STR_SIZE, MAX_STR_SIZE - 1,
			"%d,%llu,%llu\n",
			-1, (head << ALS_UD_SHIFT_NUM) | left,
			(bottom << ALS_UD_SHIFT_NUM) | right);
	}
	hwlog_info("normal message transfer\n");
	return snprintf_s(buf, MAX_STR_SIZE, MAX_STR_SIZE - 1, "%llu,%u,%u\n",
		dev_info->als_ud_data_upload.sample_start_time,
		dev_info->als_ud_data_upload.sample_interval,
		dev_info->als_ud_data_upload.integ_time); /* unit:ms */
}

ssize_t als_sensorlist_info_show(int32_t tag, struct device *dev,
	struct device_attribute *attr, char *buf)
{
	uint32_t s_id = ALS;

	if (als_get_sensor_id_by_tag(tag, &s_id))
		return -1;

	if (memcpy_s(buf, MAX_STR_SIZE, &sensorlist_info[s_id],
		sizeof(struct sensorlist_info)) != EOK)
		return -1;

	return sizeof(struct sensorlist_info);
}

ssize_t als_offset_data_show(int32_t tag, struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct als_device_info *dev_info = NULL;

	dev_info = als_get_device_info(tag);
	if (!dev_info)
		return -1;

	return snprintf_s(buf, MAX_STR_SIZE, MAX_STR_SIZE - 1,
		"als OFFSET:%u %u %u %u %u %u\n",
		dev_info->als_offset[0], dev_info->als_offset[1],
		dev_info->als_offset[2], dev_info->als_offset[3],
		dev_info->als_offset[4], dev_info->als_offset[5]);
}

ssize_t als_always_on_store(int32_t tag, struct device *dev,
	struct device_attribute *attr, const char *buf, size_t size)
{
	struct als_device_info *dev_info = NULL;

	dev_info = als_get_device_info(tag);
	if (!dev_info)
		return -1;

	if (!buf)
		return size;

	dev_info->als_always_on =
		simple_strtol(buf, NULL, FEIMA_FORMAT_DEC);
	hwlog_info("get always on info = %d\n", dev_info->als_always_on);
	send_als_ud_data_to_mcu(tag, SUB_CMD_CHANGE_ALWAYS_ON_STATUS,
		(const void *)&dev_info->als_always_on,
		sizeof(dev_info->als_always_on), false);

	return size;
}

ssize_t als_mmi_para_show(int32_t tag, struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct als_device_info *dev_info = NULL;

	dev_info = als_get_device_info(tag);
	if (!dev_info)
		return -1;

	return snprintf_s(buf, MAX_STR_SIZE, MAX_STR_SIZE - 1, "%d %d\n",
		dev_info->als_mmi_para[ALS_MMI_LUX_MIN_ID],
		dev_info->als_mmi_para[ALS_MMI_LUX_MAX_ID]);
}

ssize_t als_calibrate_under_tp_store(int32_t tag, struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct als_device_info *dev_info = NULL;
	write_info_t pkg_ap = { 0 };
	read_info_t pkg_mcu = { 0 };
	pkt_parameter_req_t spkt;
	pkt_header_t *shd = (pkt_header_t *)&spkt;
	int ret;
	unsigned long val = 0;

	dev_info = als_get_device_info(tag);
	if (!dev_info)
		return -1;
	if (memset_s(&spkt, sizeof(spkt), 0, sizeof(spkt)) != EOK)
		return -1;
	if (!buf)
		return -EINVAL;
	if (strict_strtoul(buf, 10, &val)) {
		hwlog_err("attr_als_calibrate_under_tp_store: strtoul fail\n");
		return -EINVAL;
	}
	if (val != 3) {
		hwlog_err("attr_als_calibrate_under_tp_store: wrong value input\n");
		return -EINVAL;
	}

	spkt.subcmd = SUB_CMD_SELFCALI_REQ;
	pkg_ap.tag = tag;
	pkg_ap.cmd = CMD_CMN_CONFIG_REQ;
	pkg_ap.wr_buf = &shd[1];
	pkg_ap.wr_len = sizeof(val) + SUBCMD_LEN;
	if (memcpy_s(spkt.para, sizeof(spkt.para), &val, sizeof(val)) != EOK)
		return -1;
	hwlog_info("attr_als_calibrate_under_tp_store: under_tp_als calibrated val is %lu, len is %lu\n",
		val, sizeof(val));
	ret = write_customize_cmd(&pkg_ap, &pkg_mcu, true);
	if (ret) {
		hwlog_err("%s: send pkg to mcu fail, ret=%d\n", __func__, ret);
		return -1;
	}

	if (pkg_mcu.errno != 0) {
		hwlog_err("attr_als_calibrate_under_tp_store: mcu return fail\n");
		return -1;
	}

	if (memcpy_s(dev_info->als_under_tp_cal_rawdata,
		sizeof(dev_info->als_under_tp_cal_rawdata), pkg_mcu.data,
		sizeof(dev_info->als_under_tp_cal_rawdata)) != EOK)
		hwlog_err("%s: memcpy_s cal_rawdata error\n", __func__);
	hwlog_info("attr_als_calibrate_under_tp_store: mcu return als_rawdata suc. R=%d G=%d B=%d C=%d\n",
		dev_info->als_under_tp_cal_rawdata[0],
		dev_info->als_under_tp_cal_rawdata[1],
		dev_info->als_under_tp_cal_rawdata[2],
		dev_info->als_under_tp_cal_rawdata[3]);
	return 1;
}

ssize_t als_calibrate_under_tp_show(int32_t tag, struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct als_device_info *dev_info = NULL;

	if (!dev || !attr || !buf)
		return -1;
	dev_info = als_get_device_info(tag);
	if (!dev_info)
		return -1;

	return snprintf_s(buf, MAX_STR_SIZE, MAX_STR_SIZE - 1, "%d,%d,%d,%d",
		dev_info->als_under_tp_cal_rawdata[0],
		dev_info->als_under_tp_cal_rawdata[1],
		dev_info->als_under_tp_cal_rawdata[2],
		dev_info->als_under_tp_cal_rawdata[3]);
}

ssize_t als_rgb_data_under_tp_store(int32_t tag, struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct als_device_info *dev_info = NULL;
	write_info_t pkg_ap = { 0 };
	read_info_t pkg_mcu = { 0 };
	pkt_parameter_req_t spkt;
	pkt_header_t *shd = (pkt_header_t *)&spkt;
	int ret;
	unsigned long val = 0;

	dev_info = als_get_device_info(tag);
	if (!dev_info)
		return -1;
	if (memset_s(&spkt, sizeof(spkt), 0, sizeof(spkt)) != EOK)
		return -1;
	if (!buf) {
		hwlog_err("%s: buf is NULL\n", __func__);
		return -EINVAL;
	}
	ret = strict_strtoul(buf, 10, &val);
	if (ret) {
		hwlog_err("%s: strtoul fail\n", __func__);
		return -EINVAL;
	}
	if ((val != ALS_UNDER_TP_RGB_DATA) && (val != ALS_RAW_DATA_COLLECT) &&
		(val != ALS_CALI_DATA_COLLECT)) {
		hwlog_err("%s: wrong value input\n", __func__);
		return -EINVAL;
	}

	spkt.subcmd = SUB_CMD_SELFCALI_REQ;
	pkg_ap.tag = tag;
	pkg_ap.cmd = CMD_CMN_CONFIG_REQ;
	pkg_ap.wr_buf = &shd[1];
	pkg_ap.wr_len = sizeof(val) + SUBCMD_LEN;
	if (memcpy_s(spkt.para, sizeof(spkt.para), &val, sizeof(val)) != EOK)
		return -1;
	ret = write_customize_cmd(&pkg_ap, &pkg_mcu, true);
	if (ret) {
		hwlog_err("%s: send pkg to mcu fail,ret=%d\n", __func__, ret);
		return -1;
	}
	if (pkg_mcu.errno != 0) {
		hwlog_err("%s: mcu return fail\n", __func__);
		return -1;
	}

	if (memcpy_s(dev_info->als_under_tp_rgb_data,
		sizeof(dev_info->als_under_tp_rgb_data), pkg_mcu.data,
		sizeof(dev_info->als_under_tp_rgb_data)) != EOK)
		hwlog_err("%s: memcpy_s rgb_data error\n", __func__);
	ret = count;
	return ret;
}

ssize_t als_rgb_data_under_tp_show(int32_t tag, struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct als_device_info *dev_info = NULL;

	if (!dev || !attr || !buf)
		return -1;

	dev_info = als_get_device_info(tag);
	if (!dev_info)
		return -1;

	return snprintf_s(buf, MAX_STR_SIZE, MAX_STR_SIZE - 1,
		"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
		dev_info->als_under_tp_rgb_data[0],
		dev_info->als_under_tp_rgb_data[1],
		dev_info->als_under_tp_rgb_data[2],
		dev_info->als_under_tp_rgb_data[3],
		dev_info->als_under_tp_rgb_data[4],
		dev_info->als_under_tp_rgb_data[5],
		dev_info->als_under_tp_rgb_data[6],
		dev_info->als_under_tp_rgb_data[7],
		dev_info->als_under_tp_rgb_data[8],
		dev_info->als_under_tp_rgb_data[9],
		dev_info->als_under_tp_rgb_data[10],
		dev_info->als_under_tp_rgb_data[11],
		dev_info->als_under_tp_rgb_data[12],
		dev_info->als_under_tp_rgb_data[13],
		dev_info->als_under_tp_rgb_data[14],
		dev_info->als_under_tp_rgb_data[15]);
}

ssize_t als_under_tp_calidata_store(int32_t tag, struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct als_device_info *dev_info = NULL;
	int i;
	int ret;
	int als_undertp_calidata[ALS_UNDER_TP_CALDATA_LEN] = { 0 };

	dev_info = als_get_device_info(tag);
	if ((tag != TAG_ALS) || (!dev_info)) {
		hwlog_err("%s: als under tp no tag %d\n", __func__, tag);
		return -1;
	}

	if (!buf || count != (ALS_UNDER_TP_CALDATA_LEN * sizeof(int))) {
		hwlog_err("%s: buf NULL or count is %zu\n", __func__, count);
		return -EINVAL;
	}

	if (memcpy_s(als_undertp_calidata, sizeof(als_undertp_calidata),
		buf, sizeof(als_undertp_calidata)) != EOK)
		return -1;
	dev_info->als_under_tp_cal_data.x = (uint16_t)als_undertp_calidata[0];
	dev_info->als_under_tp_cal_data.y = (uint16_t)als_undertp_calidata[1];
	dev_info->als_under_tp_cal_data.width =
		(uint16_t)als_undertp_calidata[2];
	dev_info->als_under_tp_cal_data.length =
		(uint16_t)als_undertp_calidata[3];
	hwlog_info("attr_als_under_tp_calidata_store: x = %d, y = %d, width = %d, len = %d\n",
		dev_info->als_under_tp_cal_data.x,
		dev_info->als_under_tp_cal_data.y,
		dev_info->als_under_tp_cal_data.width,
		dev_info->als_under_tp_cal_data.length);
	if (memcpy_s(dev_info->als_under_tp_cal_data.a,
		sizeof(dev_info->als_under_tp_cal_data.a),
		als_undertp_calidata + 4,
		25 * sizeof(unsigned int)) != EOK) /* start from 4 to 29 */
		hwlog_err("%s: memcpy_s a error\n", __func__);
	for (i = 4; i < 29; i++)
		hwlog_info("attr_als_under_tp_calidata_store: als_under_tp_cal_data.a[%d] = %d\n",
			i - 4, dev_info->als_under_tp_cal_data.a[i - 4]);
	for (i = 29; i < ALS_UNDER_TP_CALDATA_LEN; i++) {
		dev_info->als_under_tp_cal_data.b[i - 29] =
			als_undertp_calidata[i];
		hwlog_info("attr_als_under_tp_calidata_store: als_under_tp_cal_data.b[%d] = %d\n",
			i - 29, dev_info->als_under_tp_cal_data.b[i - 29]);
	}
	ret = als_underscreen_calidata_save(tag);
	if (ret != 0) {
		ret = mcu_save_calidata_to_nv(tag, als_undertp_calidata);
		if (ret != 0) {
			hwlog_err("%s: als_underscreen_calidata_save fail\n", __func__);
			return -1;
		}
	}
	return count;
}

ssize_t als_under_tp_calidata_show(int32_t tag, struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct als_device_info *dev_info = NULL;
	int ret;
	int i, size;

	if (!dev || !attr || !buf)
		return -1;

	dev_info = als_get_device_info(tag);
	if ((tag != TAG_ALS) || (!dev_info)) {
		hwlog_err("%s: als under tp no tag %d\n", __func__, tag);
		return -1;
	}

	ret = als_under_tp_nv_read(tag);
	if (ret != 0) {
		hwlog_info("%s: read under screen als fail\n", __func__);
		return -1;
	}
	ret = snprintf_s(buf, MAX_STR_SIZE, MAX_STR_SIZE - 1, "%d,%d,%d,%d",
		dev_info->als_under_tp_cal_data.x,
		dev_info->als_under_tp_cal_data.y,
		dev_info->als_under_tp_cal_data.width,
		dev_info->als_under_tp_cal_data.length);
	if (ret <= 0) {
		hwlog_info("%s: write data to buf fail\n", __func__);
		return -1;
	}
	size = sizeof(dev_info->als_under_tp_cal_data.a);
	size /= sizeof(unsigned int);
	for (i = 0; i < size; i++) {
		ret = snprintf_s(buf, MAX_STR_SIZE, MAX_STR_SIZE - 1, "%s,%d",
			buf, dev_info->als_under_tp_cal_data.a[i]);
		if (ret <= 0) {
			hwlog_info("%s: write a[%d] to buf fail\n", __func__, i);
			return -1;
		}
	}
	size = sizeof(dev_info->als_under_tp_cal_data.b) / sizeof(int);
	for (i = 0; i < size; i++) {
		ret = snprintf_s(buf, MAX_STR_SIZE, MAX_STR_SIZE - 1, "%s,%d",
			buf, dev_info->als_under_tp_cal_data.b[i]);
		if (ret <= 0) {
			hwlog_info("%s: write b[%d] to buf fail\n", __func__, i);
			return -1;
		}
	}
	return (ssize_t)ret;
}

ssize_t als_calibrate_after_sale_show(int32_t tag, struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct als_device_info *dev_info = NULL;

	if (!dev || !attr || !buf)
		return -1;

	dev_info = als_get_device_info(tag);
	if (!dev_info)
		return -1;

	if (dev_info->als_ud_cali_xy[0] == 0)
		return snprintf_s(buf, MAX_STR_SIZE, MAX_STR_SIZE - 1,
			"%d\n", dev_info->als_support_under_screen_cali);

	return snprintf_s(buf, MAX_STR_SIZE, MAX_STR_SIZE - 1,
		"%d, %d, %d, %d, %d\n",
		dev_info->als_support_under_screen_cali,
		dev_info->als_ud_cali_xy[0], dev_info->als_ud_cali_xy[1],
		dev_info->als_ud_cali_xy[2], dev_info->als_ud_cali_xy[3]);
}

int send_als_ud_data_to_mcu(int32_t tag, uint32_t subcmd, const void *data,
	int length, bool is_recovery)
{
	int ret;
	write_info_t pkg_ap = { 0 };
	pkt_parameter_req_t cpkt;
	pkt_header_t *hd = (pkt_header_t *)&cpkt;

	pkg_ap.tag = tag;
	pkg_ap.cmd = CMD_CMN_CONFIG_REQ;
	cpkt.subcmd = subcmd;
	pkg_ap.wr_buf = &hd[1];
	pkg_ap.wr_len = length+SUBCMD_LEN;
	if (memcpy_s(cpkt.para, sizeof(cpkt.para), data, length) != EOK) {
		hwlog_err("%s: memcpy para fail\n", __func__);
		return -1;
	}

	if (is_recovery)
		return write_customize_cmd(&pkg_ap, NULL, false);

	ret = write_customize_cmd(&pkg_ap, NULL, true);
	if (ret) {
		hwlog_err("send als ud data to mcu fail,ret=%d\n", ret);
		return -1;
	}

	return 0;
}

void send_dc_status_to_sensorhub(uint32_t dc_status)
{
	uint32_t para;
	struct als_platform_data *pf_data = NULL;

	pf_data = als_get_platform_data(TAG_ALS);
	if (!pf_data)
		return;

	if (pf_data->als_phone_type == VOGUE_PHONE_TYPE ||
		pf_data->is_dc_supported) {
		para = (uint32_t)dc_status;
		send_als_ud_data_to_mcu(TAG_ALS, SUB_CMD_CHANGE_DC_STATUS,
			(const void *)&(para), sizeof(para), false);
	}
}

void save_light_to_sensorhub(uint32_t mipi_level, uint32_t bl_level)
{
	uint64_t timestamp;
	struct timespec64 ts;
	struct bright_data para;
	struct als_platform_data *pf_data = NULL;

	pf_data = als_get_platform_data(TAG_ALS);
	if (!pf_data)
		return;

	if (pf_data->als_phone_type == ELE_PHONE_TYPE ||
		pf_data->als_phone_type == VOGUE_PHONE_TYPE ||
		pf_data->is_bllevel_supported ||
		pf_data->als_ud_type == ALS_UD_MINUS_TP_RGB ||
		pf_data->als_ud_type == ALS_UD_MINUS_DSS_NOISE) {
		get_monotonic_boottime64(&ts);
		timestamp = ((unsigned long long)(ts.tv_sec * NSEC_PER_SEC) +
			(unsigned long long)ts.tv_nsec) / 1000000;
		para.mipi_data = mipi_level;
		para.bright_data = bl_level;
		para.time_stamp = (uint64_t)timestamp;
		send_als_ud_data_to_mcu(TAG_ALS, SUB_CMD_UPDATE_BL_LEVEL,
			(const void *)&(para), sizeof(para), false);
	}
}

int als_ud_reset_immediately(int32_t tag)
{
	struct als_platform_data *pf_data = NULL;
	struct als_device_info *dev_info = NULL;

	pf_data = als_get_platform_data(tag);
	dev_info = als_get_device_info(tag);
	if (!pf_data || !dev_info)
		return 0;

	hwlog_info("als_ud_reset_immediately in\n");
	if (dev_info->sem_als_ud_rgbl_block_flag == 1) {
		send_als_under_tp_calibrate_data_to_mcu(tag);
		dev_info->als_ud_rgbl_block = 1;
		dev_info->sem_als_ud_rgbl_block_flag = 0;
		dev_info->als_ud_need_reset = 1;
		wake_up_als_ud_block();
		hwlog_info("als_ud_reset successfully\n");
		return 0;
	}
	if (pf_data->als_ud_type == ALS_UD_MINUS_DSS_NOISE) {
		send_als_under_tp_calibrate_data_to_mcu(tag);
		hwlog_info("als_ud for alsc nv reset successfully\n");
		return 0;
	}
	hwlog_info("als_ud_reset failed\n");
	return -1;
}

/* save als para to nv */
int als_underscreen_calidata_save(int32_t tag)
{
	struct als_device_info *dev_info = NULL;

	dev_info = als_get_device_info(tag);
	if ((tag != TAG_ALS) || (!dev_info)) {
		hwlog_err("%s: als under tp no tag %d\n", __func__, tag);
		return -1;
	}

	if (sizeof(dev_info->als_under_tp_cal_data) >
		ALS_TP_CALIDATA_NV1_SIZE + ALS_TP_CALIDATA_NV2_SIZE) {
		if (write_calibrate_data_to_nv(ALS_TP_CALIDATA_NV1_NUM,
				ALS_TP_CALIDATA_NV1_SIZE,
				"ALSTP1", ((char *)&(dev_info->als_under_tp_cal_data)))) {
			hwlog_err("%s: write_calidata_to_nv1 fail\n",
				__func__);
			return -1;
		}
		if (write_calibrate_data_to_nv(ALS_TP_CALIDATA_NV2_NUM,
				ALS_TP_CALIDATA_NV2_SIZE,
				"ALSTP2",
				((char *)(&(dev_info->als_under_tp_cal_data)) +
				ALS_TP_CALIDATA_NV1_SIZE))) {
			hwlog_err("%s: write_calidata_to_nv2 fail\n",
				__func__);
			return -1;
		}
		if (write_calibrate_data_to_nv(ALS_TP_CALIDATA_NV3_NUM,
				sizeof(dev_info->als_under_tp_cal_data) -
				ALS_TP_CALIDATA_NV1_SIZE -
				ALS_TP_CALIDATA_NV2_SIZE,
				"ALSTP3",
				((char *)(&(dev_info->als_under_tp_cal_data)) +
				ALS_TP_CALIDATA_NV1_SIZE +
				ALS_TP_CALIDATA_NV2_SIZE))) {
			hwlog_err("%s: write_calidata_to_nv3 fail\n",
				__func__);
			return -1;
		}
		if (als_ud_reset_immediately(tag))
			hwlog_err( "%s: als ud reset fail\n", __FUNCTION__);
		else
			hwlog_info( "%s: als ud reset successfully\n", __FUNCTION__);
	}
	hwlog_info("als_underscreen_calidata_save success\n");
	return 0;
}

void get_als_calibrate_data_by_tag(int32_t tag)
{
	int ret;
	struct als_platform_data *pf_data = NULL;
	struct als_device_info *dev_info = NULL;

	pf_data = als_get_platform_data(tag);
	dev_info = als_get_device_info(tag);
	if (!pf_data || !dev_info)
		return;

	if ((dev_info->als_first_start_flag == 0) &&
		(als_flag_result_cali(dev_info) == 1)) {
		ret = send_als_calibrate_data_to_mcu(tag);
		if (ret)
			hwlog_err("get_als_calibrate_data_tag read from nv fail, ret=%d",
				ret);
		else
			hwlog_info("read nv success\n");

		ret = get_tpcolor_from_nv(tag);
		if (ret)
			hwlog_err("get_tpcolor_from_nv fail, ret=%d", ret);
		else
			hwlog_info("get_tpcolor_from_nv success\n");
	}
}

void get_als_under_tp_calibrate_data_by_tag(int32_t tag)
{
	int ret;
	struct als_platform_data *pf_data = NULL;
	struct als_device_info *dev_info = NULL;

	pf_data = als_get_platform_data(tag);
	dev_info = als_get_device_info(tag);
	if (!pf_data || !dev_info)
		return;

	if ((!dev_info->als_under_tp_first_start_flag) &&
		(dev_info->chip_type == ALS_CHIP_APDS9253_RGB ||
		dev_info->chip_type == ALS_CHIP_AMS_TCS3701_RGB ||
		(pf_data->als_ud_type == ALS_UD_MINUS_TP_RGB) ||
		(pf_data->als_ud_type == ALS_UD_MINUS_DSS_NOISE))) {
		ret = send_als_under_tp_calibrate_data_to_mcu(tag);
		if (ret)
			hwlog_err("%s read from nv fail,ret=%d", __func__, ret);
		else
			hwlog_info("%s read nv success\n", __func__);
	}
}

uint32_t als_ud_coef_block_calc_y(struct als_platform_data *pf_data,
	struct als_device_info *dev_info)
{
	uint32_t y;
	u8 tplcd_manufacture;

	tplcd_manufacture = get_tplcd_manufacture();

	if (pf_data->als_phone_type != TETON) {
		y = get_reduce_data(dev_info->als_under_tp_cal_data.y,
			BLOCK_MID);
	} else {
		if (tplcd_manufacture == BOE_TPLCD)
			y = get_reduce_data(BOE_RESOLUTION,
				dev_info->als_under_tp_cal_data.x);
		else
			y = get_reduce_data(SAMSUNG_RESOLUTION,
				dev_info->als_under_tp_cal_data.x);
	}

	return y;
}

uint32_t als_ud_coef_block_calc_x(struct als_platform_data *pf_data,
	struct als_device_info *dev_info)
{
	uint32_t x;

	if (pf_data->als_phone_type == TETON)
		x = get_reduce_data(dev_info->als_under_tp_cal_data.y,
			BLOCK_MID);
	else
		x = get_reduce_data(dev_info->als_under_tp_cal_data.x,
			BLOCK_MID);

	return x;
}
