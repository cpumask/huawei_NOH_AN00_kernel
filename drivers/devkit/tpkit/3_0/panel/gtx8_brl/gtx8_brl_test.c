/*
 * brl_ts_test.c - TP test
 *
 * Copyright (C) 2015 - 2016 gtx8 Technology Incorporated
 * Copyright (C) 2015 - 2016 Yulong Cai <caiyulong@gtx8.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be a reference
 * to you, when you are integrating the gtx8's CTP IC into your system,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 */
#include "gtx8_brl.h"
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/firmware.h>


#define STATISTICS_DATA_LEN 32
#define MAX_TEST_ITEMS 10 /* 0P-1P-2P-3P-5P total test items */

#define GTP_CAP_TEST 1
#define GTP_DELTA_TEST 2
#define GTP_NOISE_TEST 3
#define GTP_SRAM_TEST 4
#define GTP_SHORT_TEST 5
#define GTP_SELFCAP_TEST 6
#define GTP_SELFNOISE_TEST 9

#define GTP_TEST_PASS 1
#define GTP_PANEL_REASON 2
#define SYS_SOFTWARE_REASON 3

#define TEST_FAIL 0

#define CSC_TP_MUTUAL_RAW_MIN "mutual_raw_min"
#define CSV_TP_MUTUAL_RAW_MAX "mutual_raw_max"
#define CSV_TP_SELF_RAW_TX_MIN "self_raw_tx_min"
#define CSV_TP_SELF_RAW_TX_MAX "self_raw_tx_max"
#define CSV_TP_SELF_RAW_RX_MIN "self_raw_rx_min"
#define CSV_TP_SELF_RAW_RX_MAX "self_raw_rx_max"
#define CSV_TP_TX_DELTA_MIN "tx_delta_min"
#define CSV_TP_TX_DELTA_MAX "tx_delta_max"
#define CSV_TP_RX_DELTA_MIN "rx_delta_min"
#define CSV_TP_RX_DELTA_MAX "rx_delta_max"
#define CSV_TP_MUTUAL_NOISE "mutual_noise"
#define CSV_TP_SELF_NOISE "self_noise"

#define MAX_TEST_TIME_MS 5000
#define DEFAULT_TEST_TIME_MS 1000
#define MIN_TEST_TIME_MS 800

#define MAX_DIFFDATA_TIMES 3

static u32 gt9897s_short_parms[] = {
	10, 150, 150, 150, 100, 100, 30
};

/* freq cmd data */
static u16 freq_buf[] = {
	1760, 2110, 2560
};

/*
 * struct ts_test_params - test parameters
 * drv_num: touch panel tx(driver) number
 * sen_num: touch panel tx(sensor) number
 * max_limits: max limits of rawdata
 * min_limits: min limits of rawdata
 * deviation_limits: channel deviation limits
 * short_threshold: short resistance threshold
 * r_drv_drv_threshold: resistance threshold between drv and drv
 * r_drv_sen_threshold: resistance threshold between drv and sen
 * r_sen_sen_threshold: resistance threshold between sen and sen
 * r_drv_gnd_threshold: resistance threshold between drv and gnd
 * r_sen_gnd_threshold: resistance threshold between sen and gnd
 * avdd_value: avdd voltage value
 */
struct ts_test_params {
	u32 rawdata_addr;
	u32 diffdata_addr;
	u32 self_rawdata_addr;
	u32 self_diffdata_addr;

	/* gt9897 need */
	u32 drv_num;
	u32 sen_num;

	s32 max_limits[MAX_DRV_NUM * MAX_SEN_NUM];
	s32 min_limits[MAX_DRV_NUM * MAX_SEN_NUM];
	s32 self_max_limits[MAX_DRV_NUM + MAX_SEN_NUM];
	s32 self_min_limits[MAX_DRV_NUM + MAX_SEN_NUM];
	s32 tx_delta_min[MAX_DRV_NUM * MAX_SEN_NUM];
	s32 tx_delta_max[MAX_DRV_NUM * MAX_SEN_NUM];
	s32 rx_delta_min[MAX_DRV_NUM * MAX_SEN_NUM];
	s32 rx_delta_max[MAX_DRV_NUM * MAX_SEN_NUM];
	s32 noise_threshold;
	s32 self_noise_threshold;
	u32 short_threshold;
	u32 r_drv_drv_threshold;
	u32 r_drv_sen_threshold;
	u32 r_sen_sen_threshold;
	u32 r_drv_gnd_threshold;
	u32 r_sen_gnd_threshold;
	u32 avdd_value;
};

/*
 * struct ts_test_rawdata - rawdata structure
 * data: rawdata buffer
 * size: rawdata size
 */
struct ts_test_rawdata {
	s16 data[MAX_DRV_NUM * MAX_SEN_NUM];
	u32 size;
};

struct ts_test_self_rawdata {
	s16 data[MAX_DRV_NUM + MAX_SEN_NUM];
	u32 size;
};

/*
 * struct brl_ts_test - main data structrue
 * ts: gtx8 touch screen data
 * test_config: test mode config data
 * orig_config: original config data
 * test_param: test parameters
 * rawdata: raw data structure
 * test_result: test result string
 */
struct brl_ts_test {
	struct brl_ts_data *ts;
	/* struct brl_ts_config test_config; */
	struct brl_ts_config orig_config;
	struct ts_test_params test_params;
	struct ts_test_rawdata rawdata;
	struct ts_test_rawdata noisedata[MAX_DIFFDATA_TIMES];
	struct ts_test_self_rawdata self_rawdata;
	struct ts_test_self_rawdata self_noisedata;
	struct ts_test_rawdata tx_delta;
	struct ts_test_rawdata rx_delta;

	const struct firmware *firmware;

	char test_result[MAX_TEST_ITEMS];
	char test_info[TS_RAWDATA_RESULT_MAX];
};

struct clk_parm {
	u8 gio;
	union {
		struct {
			/* division factor, if clk freq high than 64M need set this to enable 8s div */
			u8 div : 2;
			/* clk input corresponding IO, 0 for GIO 8, 0 for GIO 9*/
			u8 gio_sel : 1;
			/* osc enable, 0 for low level enable and 1 for high */
			u8 en  : 1;
			/* GIO number used to enable osc, this is GIO 8 or GIO 9*/
			u8 osc_en_io : 4;
		};
		u8 add_parm;
	};
	u32 clk_num;
	u64 freq; /* input osc freq */
	u32 freq_toler; /* osc input freq toler */
};

struct clk_test_parm {
	struct clk_parm hsync_parm;
	struct clk_parm vsync_parm;
	struct clk_parm osc_parm;
};

struct clk_test_parm clk_test_parms = {
	{ 4, {{0}}, 500, 243000, 3 }, /* Hsync */
	{ 5, {{0}}, 15, 60, 3 }, /* Vsync */
	{ 14, {{0, 1, 1, 8}}, 800, 38400000, 5 }, /* OSC */
};

static void brl_put_test_result_newformat(
	struct ts_rawdata_info_new *info, struct brl_ts_test *ts_test);
static void brl_shortcircut_test(struct brl_ts_test *ts_test);
static void brl_mbist_test(struct brl_ts_test *ts_test);

#define SHORT_FW_CMD_REG 0x10400
#define SHORT_TEST_TIME_REG 0x11FF2
#define INSPECT_PARAM_CMD 0xAA
#define INSPECT_CMD_ACK_DONE 0xEE
#define DFT_SHORT_THRESHOLD 16
#define DFT_DIFFCODE_SHORT_THRESHOLD 16
#define DFT_ADC_DUMP_NUM 1396
#define INSPECT_CMD_STATUS_FINISH 0x88

static void brl_clear_interrupt(void)
{
	u8 buf[1] = {0};
	u32 reg = brl_ts->ic_info.misc.touch_data_addr;

	brl_ts->ops.i2c_write(reg, buf, sizeof(buf));
}

static int brl_set_freq(u16 freq)
{
	struct brl_ts_cmd temp_cmd;
	int ret;

	temp_cmd.len = 6;
	temp_cmd.cmd = GOODIX_FREQ_CMD;
	temp_cmd.data[0] = (u8)freq;
	temp_cmd.data[1] = (u8)(freq >> 8);
	ret = brl_ts->ops.send_cmd(&temp_cmd);

	if (ret)
		goto exit;
	msleep(30); /* delay 30ms */

exit:
	return ret;
}

static void brl_rotate_abcd2cbad(int tx, int rx, s16 *data)
{
	s16 *temp_buf = NULL;
	int size = tx * rx;
	int i;
	int j;
	int col;

	temp_buf = kcalloc(size, sizeof(*temp_buf), GFP_KERNEL);
	if (!temp_buf) {
		TS_LOG_ERR("malloc failed\n");
		return;
	}

	for (i = 0, j = 0, col = 0; i < size; i++) {
		temp_buf[i] = data[j++ * rx + col];
		if (j == tx) {
			j = 0;
			col++;
		}
	}

	memcpy(data, temp_buf, size * sizeof(*temp_buf));
	kfree(temp_buf);
}

/*
 * Inquire in deviceinfo F Replace f
 */
static void str_ftof_inquire(char *strf)
{
	int i;
	int strf_len;

	strf_len = strlen(strf);
	for (i = 0; i < strf_len && i < TS_RAWDATA_DEVINFO_MAX; i++) {
		if (strf[i] == 'F')
			strf[i] = 'f';
	}
}

static int send_test_cmd(struct brl_ts_test *ts_test,
	struct brl_ts_cmd *cmd)
{
	int ret;
	u32 reg = SHORT_FW_CMD_REG;

	cmd->state = 0;
	cmd->ack = 0;
	goodix_append_checksum(&(cmd->buf[2]), cmd->len - 2,
		CHECKSUM_MODE_U8_LE);
	ret = ts_test->ts->ops.i2c_write(reg, cmd->buf, cmd->len + 2);
	if (ret < 0) {
		TS_LOG_ERR("send cmd failed\n");
		return ret;
	}
	mdelay(10);
	return ret;
}

static int brl_init_testlimits(struct brl_ts_test *ts_test)
{
	int ret;
	char file_path[100] = {0};
	char file_name[GTX8_FW_NAME_LEN] = {0};
	s32 data_buf[MAX_SEN_NUM] = {0};
	struct brl_ts_data *ts = ts_test->ts;
	struct ts_test_params *test_params = NULL;
	int tx;
	int rx;

	snprintf(file_name, GTX8_FW_NAME_LEN, "%s_%s_limits.csv",
		ts->dev_data->ts_platform_data->product_name, ts->project_id);

	/* change default limit file csv to vendor */
	snprintf(file_path, sizeof(file_path),
		"/odm/etc/firmware/ts/%s", file_name);
	TS_LOG_INFO("csv_file_path =%s", file_path);

	test_params = &ts_test->test_params;
	tx = test_params->drv_num;
	rx = test_params->sen_num;

	/* obtain mutual_raw_max */
	ret = ts_kit_parse_csvfile(file_path, CSV_TP_MUTUAL_RAW_MAX,
		test_params->max_limits, rx, tx);
	if (ret) {
		TS_LOG_ERR("Failed get %s", CSV_TP_MUTUAL_RAW_MAX);
		return ret;
	}

	/* obtain mutual_raw_min */
	ret = ts_kit_parse_csvfile(file_path, CSC_TP_MUTUAL_RAW_MIN,
		test_params->min_limits, rx, tx);
	if (ret) {
		TS_LOG_ERR("Failed get %s", CSC_TP_MUTUAL_RAW_MIN);
		return ret;
	}

	/* obtain self_raw_tx_max */
	ret = ts_kit_parse_csvfile(file_path, CSV_TP_SELF_RAW_TX_MAX,
		data_buf, 1, tx);
	if (ret) {
		TS_LOG_ERR("Failed get %s", CSV_TP_SELF_RAW_TX_MAX);
		return ret;
	}
	memcpy(ts_test->test_params.self_max_limits,
		data_buf, tx * sizeof(data_buf[0]));

	/* obtain self_raw_tx_min */
	ret = ts_kit_parse_csvfile(file_path, CSV_TP_SELF_RAW_TX_MIN,
		data_buf, 1, tx);
	if (ret) {
		TS_LOG_ERR("Failed get %s", CSV_TP_SELF_RAW_TX_MIN);
		return ret;
	}
	memcpy(ts_test->test_params.self_min_limits,
		data_buf, tx * sizeof(data_buf[0]));

	/* obtain self_raw_rx_max */
	ret = ts_kit_parse_csvfile(file_path, CSV_TP_SELF_RAW_RX_MAX,
		data_buf, 1, rx);
	if (ret) {
		TS_LOG_ERR("Failed get %s", CSV_TP_SELF_RAW_RX_MAX);
		return ret;
	}
	memcpy(ts_test->test_params.self_max_limits + tx,
		data_buf, rx * sizeof(data_buf[0]));

	/* obtain self_raw_rx_min */
	ret = ts_kit_parse_csvfile(file_path, CSV_TP_SELF_RAW_RX_MIN,
		data_buf, 1, rx);
	if (ret) {
		TS_LOG_ERR("Failed get %s", CSV_TP_SELF_RAW_RX_MIN);
		return ret;
	}
	memcpy(ts_test->test_params.self_min_limits + tx,
		data_buf, rx * sizeof(data_buf[0]));

	/* obtain tx_delta_max */
	ret = ts_kit_parse_csvfile(file_path, CSV_TP_TX_DELTA_MAX,
		test_params->tx_delta_max, rx, tx - 1);
	if (ret) {
		TS_LOG_ERR("Failed get %s", CSV_TP_TX_DELTA_MAX);
		return ret;
	}

	/* obtain tx_delta_min */
	ret = ts_kit_parse_csvfile(file_path, CSV_TP_TX_DELTA_MIN,
		test_params->tx_delta_min, rx, tx - 1);
	if (ret) {
		TS_LOG_ERR("Failed get %s", CSV_TP_TX_DELTA_MIN);
		return ret;
	}

	/* obtain rx_delta_max */
	ret = ts_kit_parse_csvfile(file_path, CSV_TP_RX_DELTA_MAX,
		test_params->rx_delta_max, rx - 1, tx);
	if (ret) {
		TS_LOG_ERR("Failed get %s", CSV_TP_RX_DELTA_MAX);
		return ret;
	}

	/* obtain rx_delta_min */
	ret = ts_kit_parse_csvfile(file_path, CSV_TP_RX_DELTA_MIN,
		test_params->rx_delta_min, rx - 1, tx);
	if (ret) {
		TS_LOG_ERR("Failed get %s", CSV_TP_RX_DELTA_MIN);
		return ret;
	}

	/* obtain mutual_noise */
	ret = ts_kit_parse_csvfile(file_path, CSV_TP_MUTUAL_NOISE,
		&test_params->noise_threshold, 1, 1);
	if (ret) {
		TS_LOG_ERR("Failed get %s", CSV_TP_MUTUAL_NOISE);
		return ret;
	}

	/* obtain self_noise */
	ret = ts_kit_parse_csvfile(file_path, CSV_TP_SELF_NOISE,
		&test_params->self_noise_threshold, 1, 1);
	if (ret) {
		TS_LOG_ERR("Failed get %s", CSV_TP_SELF_NOISE);
		return ret;
	}
	return ret;
}

static int brl_init_params(struct brl_ts_test *ts_test)
{
	int ret = 0;
	struct brl_ts_data *ts = ts_test->ts;
	struct ts_test_params *test_params = &ts_test->test_params;

	test_params->rawdata_addr = ts->ic_info.misc.mutual_rawdata_addr;
	test_params->diffdata_addr = ts->ic_info.misc.mutual_diffdata_addr;
	test_params->self_rawdata_addr = ts->ic_info.misc.self_rawdata_addr;
	test_params->self_diffdata_addr = ts->ic_info.misc.self_diffdata_addr;

	test_params->drv_num = ts->ic_info.parm.drv_num;
	test_params->sen_num = ts->ic_info.parm.sen_num;

	test_params->short_threshold = gt9897s_short_parms[0];
	test_params->r_drv_drv_threshold = gt9897s_short_parms[1];
	test_params->r_drv_sen_threshold = gt9897s_short_parms[2];
	test_params->r_sen_sen_threshold = gt9897s_short_parms[3];
	test_params->r_drv_gnd_threshold = gt9897s_short_parms[4];
	test_params->r_sen_gnd_threshold = gt9897s_short_parms[5];
	test_params->avdd_value = gt9897s_short_parms[6];

	return ret;
}

/*
 * gtx8_read_origconfig
 * read original config data
 */
static int brl_cache_origconfig(struct brl_ts_test *ts_test)
{
	int ret;
	u8 checksum;

	if (ts_test->ts->ops.read_cfg) {
		ret = ts_test->ts->ops.read_cfg(&ts_test->orig_config.data[0],
			GOODIX_CFG_MAX_SIZE);
		if (ret < 0) {
			TS_LOG_ERR("Failed to read original config data");
			return ret;
		}

		ts_test->orig_config.data[1] = GTX8_CONFIG_REFRESH_DATA;
		checksum = checksum_u8(&ts_test->orig_config.data[0], 3);
		ts_test->orig_config.data[3] = (u8)(0 - checksum);

		mutex_init(&ts_test->orig_config.lock);
		ts_test->orig_config.length = ret;
		ts_test->orig_config.delay = SEND_CFG_FLASH;
		strncpy(ts_test->orig_config.name,
			"original_config", MAX_STR_LEN);
		ts_test->orig_config.initialized = true;
	}
	return NO_ERR;
}

/*
 * brl_tptest_prepare
 * preparation before tp test
 */
#define INSPECT_FW_SWITCH_CMD 0x85
#define TEST_FW_PID "OST"  /* PID3-5 */
static bool init_flag;

static int brl_tptest_prepare(struct brl_ts_test *ts_test)
{
	int ret;
	struct brl_ts_config *config = NULL;

	TS_LOG_INFO("TP test preparation");
	ret = brl_cache_origconfig(ts_test);
	if (ret) {
		TS_LOG_ERR("Failed cache origin config");
		return ret;
	}

	if (init_flag != 1) {
		/* init reg addr and short cal map */
		ret = brl_init_params(ts_test);
		if (ret) {
			TS_LOG_ERR("Failed init register address\n");
			return ret;
		}

		/* parse test limits from csv */
		ret = brl_init_testlimits(ts_test);
		if (ret) {
			TS_LOG_ERR("Failed to init testlimits from csv:%d",
				ret);
			return ret;
		}
		init_flag = 1;
	}

	config = &ts_test->ts->test_cfg;
	if (ts_test->ts->ops.send_cfg && config->initialized) {
		ret = ts_test->ts->ops.send_cfg(config);
		if (ret) {
			TS_LOG_ERR("send test config failed");
			return ret;
		}
		msleep(100); /* delay 100ms */
	}
	return ret;
}

/*
 * brl_tptest_finish
 * finish test
 */
static int brl_tptest_finish(struct brl_ts_test *ts_test)
{
	int ret;
	struct brl_ts_config *config = NULL;

	TS_LOG_INFO("TP test finish");
	ret = ts_test->ts->ops.chip_reset();
	if (ret)
		TS_LOG_ERR("%s: chip reset failed", __func__);

	config = &ts_test->ts->normal_cfg;
	if (ts_test->ts->ops.send_cfg && config->initialized) {
		ret = ts_test->ts->ops.send_cfg(config);
		msleep(100); /* delay 100ms */
		if (ret)
			TS_LOG_ERR("Failed to send normal config:%d", ret);
	}
	return ret;
}

static int analysis_delta_data(struct brl_ts_test *ts_test)
{
	int size;
	int ret = NO_ERR;
	int i;
	s32 val;

	size = ts_test->tx_delta.size;
	for (i = 0; i < size; i++) {
		val = (s32)ts_test->tx_delta.data[i];
		if (val > ts_test->test_params.tx_delta_max[i] ||
			val < ts_test->test_params.tx_delta_min[i]) {
			TS_LOG_ERR("tx_data isn't in range,value:%d threshold:%d,%d\n",
				val, ts_test->test_params.tx_delta_min[i],
				ts_test->test_params.tx_delta_max[i]);
			ret = -1;
			break;
		}
	}

	size = ts_test->rx_delta.size;
	for (i = 0; i < size; i++) {
		val = (s32)ts_test->rx_delta.data[i];
		if (val > ts_test->test_params.rx_delta_max[i] ||
			val < ts_test->test_params.rx_delta_min[i]) {
			TS_LOG_ERR("rx_data isn't in range,value:%d threshold:%d,%d\n",
				val, ts_test->test_params.rx_delta_min[i],
				ts_test->test_params.rx_delta_max[i]);
			ret = -1;
			break;
		}
	}
	return ret;
}

static void brl_cache_tx_deltadata(struct brl_ts_test *ts_test)
{
	int i;
	int j;
	int pos = 0;
	int cnt;
	int sen_num = ts_test->test_params.sen_num;
	int drv_num = ts_test->test_params.drv_num;

	cnt = drv_num - 1;
	ts_test->tx_delta.size = sen_num * (drv_num - 1);

	for (i = 0; i < cnt; i++) {
		for (j = 0; j < sen_num; j++) {
			ts_test->tx_delta.data[pos] =
				ts_test->rawdata.data[j + i * sen_num] -
				ts_test->rawdata.data[j + sen_num + i * sen_num];
			if (ts_test->tx_delta.data[pos] < 0)
				ts_test->tx_delta.data[pos] =
					-ts_test->tx_delta.data[pos];
			pos++;
		}
	}
}

static void brl_cache_rx_deltadata(struct brl_ts_test *ts_test)
{
	int i;
	int j;
	int pos = 0;
	int cnt;
	int sen_num = ts_test->test_params.sen_num;
	int drv_num = ts_test->test_params.drv_num;

	cnt = sen_num - 1;
	ts_test->rx_delta.size = drv_num * (sen_num - 1);

	for (i = 0; i < cnt; i++) {
		for (j = 0; j < drv_num; j++) {
			ts_test->rx_delta.data[pos] =
				ts_test->rawdata.data[i + sen_num * j] -
				ts_test->rawdata.data[i + 1 + sen_num * j];
			if (ts_test->rx_delta.data[pos] < 0)
				ts_test->rx_delta.data[pos] =
					-ts_test->rx_delta.data[pos];
			pos++;
		}
	}
}

/*
 * brl_cache_rawdata - cache rawdata
 */
static int brl_cache_rawdata(struct brl_ts_test *ts_test)
{
	int ret = -EINVAL;
	int retry = GTX8_RETRY_NUM_3;
	u32 flag_addr = 0;
	u8 buf[1] = {0x00};
	u32 data_size;
	u32 data_addr;
	u32 sen_num = ts_test->test_params.sen_num;
	u32 drv_num = ts_test->test_params.drv_num;

	data_size = sen_num * drv_num;
	flag_addr = ts_test->ts->ic_info.misc.touch_data_addr;

	if (data_size > drv_num * sen_num || data_size <= 0) {
		TS_LOG_ERR("Invalid rawdata size(%u)", data_size);
		return ret;
	}
	brl_clear_interrupt();
	usleep_range(10000, 11000); /* 10000 : 10ms */
	while (retry--) {
		ret = ts_test->ts->ops.i2c_read(flag_addr, &buf[0], 1);
		if ((ret == 0) && (buf[0] & 0x80) == 0x80) {
			TS_LOG_INFO("rawdata is ready");
			break;
		}
		msleep(20); /* 20 : 20ms */
	}

	if (retry < 0) {
		TS_LOG_ERR("rawdata is not ready");
		ret = -EINVAL;
		goto cache_exit;
	}

	/* get rawdata */
	data_addr = ts_test->test_params.rawdata_addr;
	ret = ts_test->ts->ops.i2c_read(data_addr,
		(u8 *)&ts_test->rawdata.data[0],
		data_size * sizeof(u16));
	if (ret) {
		TS_LOG_ERR("Failed to read rawdata:%d", ret);
		goto cache_exit;
	}

	brl_rotate_abcd2cbad(drv_num, sen_num, ts_test->rawdata.data);
	ts_test->rawdata.size = data_size;
	TS_LOG_INFO("get rawdata finish");

	/* get self rawdata */
	data_size = sen_num + drv_num;
	data_addr = ts_test->test_params.self_rawdata_addr;
	ret = ts_test->ts->ops.i2c_read(data_addr,
		(u8 *)&ts_test->self_rawdata.data[0],
		data_size * sizeof(u16));
	if (ret) {
		TS_LOG_ERR("Failed to read self_rawdata:%d", ret);
		goto cache_exit;
	}

	ts_test->self_rawdata.size = data_size;
	TS_LOG_INFO("get self_rawdata finish");
cache_exit:
	return ret;
}

#define CMD_STATE_OK 0x0F
#define CMD_NOISE_TEST 0x1F
#define REG_NOISET_TEST 0x123D8
static int brl_cache_noisedata(struct brl_ts_test *ts_test,
	s16 *noise, int data_size)
{
	u8 buf[1];
	int retry = GTX8_RETRY_NUM_100;
	struct brl_ts_data *ts = ts_test->ts;
	u32 noise_reg = REG_NOISET_TEST;
	u32 cmd_reg = ts->ic_info.misc.cmd_addr;
	struct brl_ts_cmd cmd;
	int ret;

	memset((u8 *)noise, 0, data_size * sizeof(*noise));
	cmd.len = CONFIG_CMD_LEN;
	cmd.cmd = CMD_NOISE_TEST;
	ret = ts->ops.send_cmd(&cmd);
	if (ret)
		goto exit;
	while (retry--) {
		usleep_range(10000, 11000); /* 10000 : 10ms */
		ret = ts->ops.i2c_read(cmd_reg, buf, sizeof(buf));
		if (ret)
			goto exit;
		if (buf[0] == CMD_STATE_OK)
			break;
	}

	if (retry < 0) {
		ret = -EINVAL;
		TS_LOG_ERR("retry timeout %d status:0x%02x\n", retry, buf[0]);
		goto exit;
	}

	ret = ts->ops.i2c_read(noise_reg, (u8 *)noise, data_size * sizeof(*noise));

exit:
	return ret;
}

static int brl_cache_diffdata(struct brl_ts_test *ts_test)
{
	int ret = -EINVAL;
	u32 data_size;
	u32 data_addr;
	u32 i;
	u32 sen_num = ts_test->test_params.sen_num;
	u32 drv_num = ts_test->test_params.drv_num;

	/* get self diffdata */
	data_size = sen_num + drv_num;
	data_addr = ts_test->test_params.self_diffdata_addr;
	ret = ts_test->ts->ops.i2c_read(data_addr,
		(u8 *)&ts_test->self_noisedata.data[0],
		data_size * sizeof(u16));
	if (ret) {
		TS_LOG_ERR("Failed to read self_noisedata:%d", ret);
		goto cache_exit;
	}

	for (i = 0; i < data_size; i++) {
		if (ts_test->self_noisedata.data[i] < 0)
			ts_test->self_noisedata.data[i] =
				-ts_test->self_noisedata.data[i];
	}
	ts_test->self_noisedata.size = data_size;
	TS_LOG_INFO("get self_noisedata finish");

cache_exit:
	return ret;
}

static u8 analysis_raw_data(struct brl_ts_test *ts_test)
{
	u32 i;
	u32 size;
	u8 ret = 0x00;
	s32 val;

	/* analysis mutual raw */
	size = ts_test->rawdata.size;
	for (i = 0; i < size; i++) {
		val = (s32)ts_test->rawdata.data[i];
		if ((val < ts_test->test_params.min_limits[i]) ||
			(val > ts_test->test_params.max_limits[i])) {
			TS_LOG_ERR("mutual_raw data isn't in range, value:%d threshold:%d,%d\n",
				val, ts_test->test_params.min_limits[i],
				ts_test->test_params.max_limits[i]);
			ret |= 0x01;
			break;
		}
	}

	/* analysis self raw */
	size = ts_test->self_rawdata.size;
	for (i = 0; i < size; i++) {
		val = (s32)ts_test->self_rawdata.data[i];
		if ((val < ts_test->test_params.self_min_limits[i]) ||
			(val > ts_test->test_params.self_max_limits[i])) {
			TS_LOG_ERR("self_raw data isn't in range,value:%d threshold:%d,%d\n",
				val, ts_test->test_params.self_min_limits[i],
				ts_test->test_params.self_max_limits[i]);
			ret |= 0x02;
			break;
		}
	}
	return ret;
}

static int analysis_noisedata(struct brl_ts_test *ts_test)
{
	u32 i;
	u32 size;
	s32 val;

	/* analysis mutual diff */
	size = ts_test->noisedata[0].size;
	for (i = 0; i < size; i++) {
		val = (s32)ts_test->noisedata[0].data[i];
		if (val > ts_test->test_params.noise_threshold) {
			TS_LOG_ERR("the noisedata data is not in range, value:%d threshold:%d",
				val, ts_test->test_params.noise_threshold);
			return -EINVAL;
		}
	}
	return NO_ERR;
}

static int analysis_self_noisedata(struct brl_ts_test *ts_test)
{
	u32 i;
	u32 size;
	s32 val;

	/* analysis self diff */
	size = ts_test->self_noisedata.size;
	for (i = 0; i < size; i++) {
		val = (s32)ts_test->self_noisedata.data[i];
		if (val > ts_test->test_params.self_noise_threshold) {
			TS_LOG_ERR("the self_noisedata data is not in range, value:%d threshold:%d",
				val, ts_test->test_params.self_noise_threshold);
			return -EINVAL;
		}
	}
	return NO_ERR;
}

static void brl_capacitance_test(struct brl_ts_test *ts_test)
{
	int ret;
	u8 val;
	int cnt;
	struct brl_ts_cmd temp_cmd;
	u32 sen_num = ts_test->test_params.sen_num;
	u32 drv_num = ts_test->test_params.drv_num;
	u32 data_size = sen_num * drv_num;

	/* get noisedata */
	for (cnt = 0; cnt < MAX_DIFFDATA_TIMES; cnt++) {
		ret = brl_set_freq(freq_buf[cnt]);
		if (ret) {
			ts_test->test_result[GTP_NOISE_TEST] =
				SYS_SOFTWARE_REASON;
			TS_LOG_ERR("Failed to switch frequency\n");
			break;
		}
		ret = brl_cache_noisedata(ts_test,
			ts_test->noisedata[cnt].data, data_size);
		if (ret) {
			ts_test->test_result[GTP_NOISE_TEST] =
				SYS_SOFTWARE_REASON;
			TS_LOG_ERR("cache noisedatat failed\n");
			break;
		}
		brl_rotate_abcd2cbad(drv_num, sen_num,
			ts_test->noisedata[cnt].data);
		ts_test->noisedata[cnt].size = data_size;
	}
	TS_LOG_INFO("get noisedata finish\n");
	if (ts_test->test_result[GTP_NOISE_TEST] != SYS_SOFTWARE_REASON) {
		ret = analysis_noisedata(ts_test);
		if (ret)
			ts_test->test_result[GTP_NOISE_TEST] = GTP_PANEL_REASON;
		else
			ts_test->test_result[GTP_NOISE_TEST] = GTP_TEST_PASS;
	}

	/* get rawdata prepare */
	temp_cmd.cmd = GOODIX_CMD_RAWDATA;
	temp_cmd.len = CONFIG_CMD_LEN;
	ret = ts_test->ts->ops.send_cmd(&temp_cmd);

	ret = brl_cache_rawdata(ts_test);
	if (ret) {
		TS_LOG_ERR("Read rawdata failed\n");
		ts_test->test_result[GTP_CAP_TEST] = SYS_SOFTWARE_REASON;
		ts_test->test_result[GTP_SELFCAP_TEST] = SYS_SOFTWARE_REASON;
	} else {
		val = analysis_raw_data(ts_test);
		if (val & 0x01)
			ts_test->test_result[GTP_CAP_TEST] = GTP_PANEL_REASON;
		else
			ts_test->test_result[GTP_CAP_TEST] = GTP_TEST_PASS;

		if (val & 0x02)
			ts_test->test_result[GTP_SELFCAP_TEST] =
				GTP_PANEL_REASON;
		else
			ts_test->test_result[GTP_SELFCAP_TEST] = GTP_TEST_PASS;
		brl_cache_tx_deltadata(ts_test);
		brl_cache_rx_deltadata(ts_test);
		ret = analysis_delta_data(ts_test);
		if (ret)
			ts_test->test_result[GTP_DELTA_TEST] = GTP_PANEL_REASON;
		else
			ts_test->test_result[GTP_DELTA_TEST] = GTP_TEST_PASS;
	}

	ret = brl_cache_diffdata(ts_test);
	if (ret) {
		TS_LOG_ERR("Read self_noise failed");
		ts_test->test_result[GTP_SELFNOISE_TEST] = SYS_SOFTWARE_REASON;
	} else {
		ret = analysis_self_noisedata(ts_test);
		if (ret)
			ts_test->test_result[GTP_SELFNOISE_TEST] =
				GTP_PANEL_REASON;
		else
			ts_test->test_result[GTP_SELFNOISE_TEST] =
				GTP_TEST_PASS;
	}

	temp_cmd.cmd = GOODIX_CMD_COORD;
	temp_cmd.len = CONFIG_CMD_LEN;
	ret = ts_test->ts->ops.send_cmd(&temp_cmd);
	brl_clear_interrupt();
	TS_LOG_INFO("capacitance_test finish");
}

#define TEST_RESULT_REG 0x10410
#define TEST_RESULT_PASS 0x0F
static int brl_shortcircut_analysis(struct brl_ts_test *ts_test)
{
	int ret;
	int err = 0;
	struct test_result_t test_result;

	memset(&test_result, 0, sizeof(test_result));

	TS_LOG_INFO("start analysis test result");
	ret = ts_test->ts->ops.i2c_read(TEST_RESULT_REG, (u8 *)&test_result,
		sizeof(test_result));
	if (ret) {
		TS_LOG_ERR("Read TEST_RESULT_REG falied");
		return -1;
	}

	if (checksum_cmp((u8 *)&test_result, sizeof(test_result),
		CHECKSUM_MODE_U8_LE)) {
		TS_LOG_ERR("shrot result checksum err");
		return -1;
	}

	if (!(test_result.result & TEST_RESULT_PASS)) {
		TS_LOG_INFO("----- No shortcircut -----");
		return NO_ERR;
	}

	TS_LOG_INFO("short check return 0x%x", err);
	return err;
}

static void brl_prepare_test_failed(
	struct ts_rawdata_info_new *info)
{
	int dest_len;
	int available_len;

	dest_len = strlen(info->i2cinfo);
	available_len = sizeof(info->i2cinfo) - dest_len - 1;
	if (available_len > 0)
		strncat(&info->i2cinfo[dest_len], "0F", available_len);

	dest_len = strlen(info->i2cerrinfo);
	available_len = sizeof(info->i2cerrinfo) - dest_len - 1;
	if (available_len > 0)
		strncat(&info->i2cerrinfo[dest_len], "software reason",
			available_len);
}

static void brl_prepare_test_pass(struct ts_rawdata_info_new *info)
{
	snprintf(info->i2cinfo, TS_RAWDATA_RESULT_CODE_LEN,
		"%d%c", 0, 'P');
}

struct brl_ts_test gts_test;
int brl_get_rawdata(struct ts_rawdata_info *info,
	struct ts_cmd_node *out_cmd)
{
	int ret;

	if (!brl_ts || !info) {
		TS_LOG_ERR("%s: brl_ts is NULL\n", __func__);
		return -ENODEV;
	}

	gts_test.ts = brl_ts;
	brl_ts->test_mode = true;

	ret = brl_tptest_prepare(&gts_test);
	if (ret) {
		TS_LOG_ERR("Failed parse test peremeters, exit test");
		brl_prepare_test_failed((struct ts_rawdata_info_new *)info);
		goto exit_finish;
	}
	brl_prepare_test_pass((struct ts_rawdata_info_new *)info);
	TS_LOG_INFO("TP test prepare OK");

	brl_capacitance_test(&gts_test); /* 1F 2F 6F test */
	brl_shortcircut_test(&gts_test); /* 5F test */
	brl_mbist_test(&gts_test);
	brl_put_test_result_newformat((struct ts_rawdata_info_new *)info,
		&gts_test);
	brl_tptest_finish(&gts_test);

exit_finish:
	brl_ts->test_mode = false;

	return ret;
}


/*
 * brl_data_statistics
 * catlculate Avg Min Max value of data
 */
static void brl_data_statistics(u16 *data, size_t data_size,
	char *result, size_t res_size)
{
	u16 i;
	u16 avg;
	u16 min;
	u16 max;
	long long sum = 0;

	if (!data || !result) {
		TS_LOG_ERR("parameters error please check");
		return;
	}

	if (data_size <= 0 || res_size <= 0) {
		TS_LOG_ERR("parameter is illegva:data_size=%ld,res_size=%ld",
			data_size, res_size);
		return;
	}

	min = data[0];
	max = data[0];
	for (i = 0; i < data_size; i++) {
		sum += data[i];
		if (max < data[i])
			max = data[i];
		if (min > data[i])
			min = data[i];
	}
	avg = div_s64(sum, data_size);
	memset(result, 0, res_size);
	snprintf(result, res_size, "[%d,%d,%d]", avg, max, min);
}

static void brl_put_test_result_newformat(
	struct ts_rawdata_info_new *info, struct brl_ts_test *ts_test)
{
	u32 i;
	u32 cnt;
	int pos;
	int dest_len;
	int available_len;
	int tmp_testresult;
	int have_bus_error = 0;
	int have_panel_error = 0;
	char statistics_data[STATISTICS_DATA_LEN] = {0};
	struct ts_rawdata_newnodeinfo *pts_node = NULL;
	char testresut[] = {' ', 'P', 'F', 'F'};
	u8 fw_vendor[CHIP_INFO_LENGTH_EX] = {0};
	struct brl_ts_data *ts = brl_ts;

	TS_LOG_INFO("%s start\n", __func__);
	info->rx = ts_test->test_params.sen_num;
	info->tx = ts_test->test_params.drv_num;

	/* i2c info */
	for (i = 0; i < MAX_TEST_ITEMS; i++) {
		if (ts_test->test_result[i] == SYS_SOFTWARE_REASON)
			have_bus_error = 1;
		else if (ts_test->test_result[i] == GTP_PANEL_REASON)
			have_panel_error = 1;
	}

	/* CAP data info 1P */
	pts_node = kzalloc(sizeof(*pts_node), GFP_KERNEL);
	if (!pts_node) {
		TS_LOG_ERR("malloc failed\n");
		return;
	}
	if (ts_test->rawdata.size) {
		pts_node->values = kcalloc(ts_test->rawdata.size, sizeof(int),
			GFP_KERNEL);
		if (!pts_node->values) {
			TS_LOG_ERR("malloc failed  for values\n");
			kfree(pts_node);
			pts_node = NULL;
			return;
		}
		for (i = 0; i < ts_test->rawdata.size; i++)
			pts_node->values[i] = ts_test->rawdata.data[i];
		/* calculate rawdata min avg max vale */
		brl_data_statistics(&ts_test->rawdata.data[0],
			ts_test->rawdata.size, statistics_data,
			STATISTICS_DATA_LEN - 1);
		strncpy(pts_node->statistics_data, statistics_data,
			sizeof(pts_node->statistics_data) - 1);
	}
	pts_node->size = ts_test->rawdata.size;
	tmp_testresult = ts_test->test_result[GTP_CAP_TEST];
	pts_node->testresult = testresut[tmp_testresult];
	pts_node->typeindex = GTP_CAP_TEST;
	strncpy(pts_node->test_name, "Cap_Rawdata",
		sizeof(pts_node->test_name) - 1);
	list_add_tail(&pts_node->node, &info->rawdata_head);
	TS_LOG_INFO("%s cap test result %d %c\n", __func__, pts_node->typeindex,
		pts_node->testresult);
	/* tp test failed reason */
	if (have_bus_error)
		strncpy(pts_node->tptestfailedreason, "software reason",
			sizeof(pts_node->tptestfailedreason)-1);
	else if (have_panel_error)
		strncpy(pts_node->tptestfailedreason, "panel_reason",
			sizeof(pts_node->tptestfailedreason)-1);

	/* DELTA  2P */
	pts_node = kzalloc(sizeof(*pts_node), GFP_KERNEL);
	if (!pts_node) {
		TS_LOG_ERR("malloc failed\n");
		return;
	}
	pts_node->size = ts_test->tx_delta.size +
		ts_test->rx_delta.size + info->tx;
	if (pts_node->size) {
		pts_node->values = kcalloc(pts_node->size,
			sizeof(int), GFP_KERNEL);
		if (!pts_node->values) {
			TS_LOG_ERR("malloc failed  for values");
			kfree(pts_node);
			pts_node = NULL;
			return;
		}
		pos = 0;
		for (i = 0; i < ts_test->rx_delta.size; i++)
			pts_node->values[pos++] = ts_test->rx_delta.data[i];
		for (i = 0; i < (u32)info->tx; i++)
			pts_node->values[pos++] = 0;
		for (i = 0; i < ts_test->tx_delta.size; i++)
			pts_node->values[pos++] = ts_test->tx_delta.data[i];
		/* calculate rawdata min avg max value */
		brl_data_statistics(&ts_test->tx_delta.data[0],
			ts_test->tx_delta.size,
			statistics_data, STATISTICS_DATA_LEN - 1);
		strncpy(pts_node->statistics_data, statistics_data,
			sizeof(pts_node->statistics_data) - 1);
	}
	tmp_testresult = ts_test->test_result[GTP_DELTA_TEST];
	pts_node->testresult = testresut[tmp_testresult];
	pts_node->typeindex = GTP_DELTA_TEST;
	strncpy(pts_node->test_name, "Trx_delta",
		sizeof(pts_node->test_name) - 1);
	list_add_tail(&pts_node->node, &info->rawdata_head);
	TS_LOG_INFO("%s cap test result %d %c\n", __func__, pts_node->typeindex,
		pts_node->testresult);

	/* save noise data to info->buff  3P */
	pts_node = kzalloc(sizeof(*pts_node), GFP_KERNEL);
	if (!pts_node) {
		TS_LOG_ERR("malloc failed\n");
		return;
	}
	if (ts_test->noisedata[0].size) {
		pts_node->values =
			kcalloc(ts_test->noisedata[0].size * MAX_DIFFDATA_TIMES,
			sizeof(int), GFP_KERNEL);
		if (!pts_node->values) {
			TS_LOG_ERR("malloc failed  for values\n");
			kfree(pts_node);
			pts_node = NULL;
			return;
		}
		pos = 0;
		for (cnt = 0; cnt < MAX_DIFFDATA_TIMES; cnt++) {
			for (i = 0; i < ts_test->noisedata[cnt].size; i++)
				pts_node->values[pos++] =
					ts_test->noisedata[cnt].data[i];
		}

		/* calculate rawdata min avg max vale */
		brl_data_statistics(&ts_test->noisedata[0].data[0],
			ts_test->noisedata[0].size, statistics_data,
			STATISTICS_DATA_LEN - 1);
		strncpy(pts_node->statistics_data, statistics_data,
			sizeof(pts_node->statistics_data) - 1);
	}
	pts_node->size = ts_test->noisedata[0].size * MAX_DIFFDATA_TIMES;
	tmp_testresult = ts_test->test_result[GTP_NOISE_TEST];
	pts_node->testresult = testresut[tmp_testresult];
	pts_node->typeindex = GTP_NOISE_TEST;
	strncpy(pts_node->test_name, "noise_delta",
		sizeof(pts_node->test_name) - 1);
	list_add_tail(&pts_node->node, &info->rawdata_head);
	TS_LOG_INFO("%s cap test result %d %c\n", __func__, pts_node->typeindex,
		pts_node->testresult);

	/* SRAM test 4P */
	pts_node = kzalloc(sizeof(*pts_node), GFP_KERNEL);
	if (!pts_node) {
		TS_LOG_ERR("malloc failed");
		return;
	}
	pts_node->size = 0;
	tmp_testresult = ts_test->test_result[GTP_SRAM_TEST];
	pts_node->testresult = testresut[tmp_testresult];
	pts_node->typeindex = GTP_SRAM_TEST;
	strncpy(pts_node->test_name, "sram_test",
		sizeof(pts_node->test_name)-1);

	list_add_tail(&pts_node->node, &info->rawdata_head);
	TS_LOG_INFO("%s cap test result %d %c\n", __func__, pts_node->typeindex,
		pts_node->testresult);

	/* shortcircut  5P */
	pts_node = kzalloc(sizeof(*pts_node), GFP_KERNEL);
	if (!pts_node) {
		TS_LOG_ERR("malloc failed\n");
		return;
	}
	pts_node->size = 0;
	tmp_testresult = ts_test->test_result[GTP_SHORT_TEST];
	pts_node->testresult = testresut[tmp_testresult];
	pts_node->typeindex = GTP_SHORT_TEST;
	strncpy(pts_node->test_name, "open_test",
		sizeof(pts_node->test_name) - 1);
	list_add_tail(&pts_node->node, &info->rawdata_head);
	TS_LOG_INFO("%s cap test result %d %c\n", __func__, pts_node->typeindex,
		pts_node->testresult);

	/* SelfCap 6P */
	pts_node = kzalloc(sizeof(*pts_node), GFP_KERNEL);
	if (!pts_node) {
		TS_LOG_ERR("malloc failed\n");
		return;
	}
	if (ts_test->self_rawdata.size) {
		pts_node->values = kcalloc(
			ts_test->self_rawdata.size, sizeof(int), GFP_KERNEL);
		if (!pts_node->values) {
			TS_LOG_ERR("malloc failed  for values\n");
			kfree(pts_node);
			pts_node = NULL;
			return;
		}
		for (i = 0; i < ts_test->self_rawdata.size; i++) {
			if (i < (u32)info->rx)
				pts_node->values[i] =
					ts_test->self_rawdata.data[info->tx + i];
			else
				pts_node->values[i] =
					ts_test->self_rawdata.data[i - info->rx];
		}
		brl_data_statistics(&ts_test->self_rawdata.data[0],
			ts_test->self_rawdata.size, statistics_data,
			STATISTICS_DATA_LEN - 1);
		strncpy(pts_node->statistics_data, statistics_data,
			sizeof(pts_node->statistics_data) - 1);
	}
	pts_node->size = ts_test->self_rawdata.size;
	tmp_testresult = ts_test->test_result[GTP_SELFCAP_TEST];
	pts_node->testresult = testresut[tmp_testresult];
	pts_node->typeindex = GTP_SELFCAP_TEST;
	strncpy(pts_node->test_name, "Self_Cap",
		sizeof(pts_node->test_name) - 1);
	list_add_tail(&pts_node->node, &info->rawdata_head);
	TS_LOG_INFO("%s cap test result %d %c\n", __func__, pts_node->typeindex,
		pts_node->testresult);

	/* GTP_SELFNOISE_TEST 9P */
	pts_node = kzalloc(sizeof(*pts_node), GFP_KERNEL);
	if (!pts_node) {
		TS_LOG_ERR("malloc failed\n");
		return;
	}
	if (ts_test->self_noisedata.size) {
		pts_node->values = kcalloc(
			ts_test->self_noisedata.size, sizeof(int), GFP_KERNEL);
		if (!pts_node->values) {
			TS_LOG_ERR("malloc failed  for values\n");
			kfree(pts_node);
			pts_node = NULL;
			return;
		}
		for (i = 0; i < ts_test->self_noisedata.size; i++) {
			if (i < (u32)info->rx)
				pts_node->values[i] =
					ts_test->self_noisedata.data[info->tx + i];
			else
				pts_node->values[i] =
					ts_test->self_noisedata.data[i - info->rx];
		}
		brl_data_statistics(&ts_test->self_noisedata.data[0],
			ts_test->self_noisedata.size, statistics_data,
			STATISTICS_DATA_LEN - 1);
		strncpy(pts_node->statistics_data, statistics_data,
			sizeof(pts_node->statistics_data) - 1);
	}
	pts_node->size = ts_test->self_noisedata.size;
	tmp_testresult = ts_test->test_result[GTP_SELFNOISE_TEST];
	pts_node->testresult = testresut[tmp_testresult];
	pts_node->typeindex = GTP_SELFNOISE_TEST;
	strncpy(pts_node->test_name, "self_noise",
		sizeof(pts_node->test_name) - 1);
	list_add_tail(&pts_node->node, &info->rawdata_head);
	TS_LOG_INFO("%s cap test result %d %c\n", __func__, pts_node->typeindex,
		pts_node->testresult);

	/* dev info */
	dest_len = strlen(info->deviceinfo);
	available_len = TS_RAWDATA_DEVINFO_MAX - dest_len - 1;
	if (available_len > 0)
		strncat(&info->deviceinfo[dest_len], ts_test->ts->project_id,
			available_len);

	dest_len = strlen(info->deviceinfo);
	available_len = TS_RAWDATA_DEVINFO_MAX - dest_len - 1;
	if (available_len > 0)
		strncat(&info->deviceinfo[dest_len], "-", available_len);

	snprintf(fw_vendor, sizeof(fw_vendor),
		"%02X_%02X_%X", ts->hw_info.patch_vid[PATCH_VID_MINOR],
		ts->hw_info.patch_vid[PATCH_VID_MAJOR],
		ts->normal_cfg.data[NORMAL_CFG_DATA]);

	dest_len = strlen(info->deviceinfo);
	available_len = TS_RAWDATA_DEVINFO_MAX - dest_len - 1;
	if (available_len > 0)
		strncat(&info->deviceinfo[dest_len],
			fw_vendor, available_len);

	dest_len = strlen(info->deviceinfo);
	available_len = TS_RAWDATA_DEVINFO_MAX - dest_len - 1;
	if (available_len > 0)
		strncat(&info->deviceinfo[dest_len], ";", available_len);

	str_ftof_inquire(info->deviceinfo);
}

/* short test */
static int brl_short_test_prepare(struct brl_ts_test *ts_test)
{
	struct brl_ts_cmd tmp_cmd;
	struct brl_ts_version fw_ver;
	int ret;

	memset(&tmp_cmd, 0, sizeof(tmp_cmd));
	tmp_cmd.len = 4;
	tmp_cmd.cmd = INSPECT_FW_SWITCH_CMD;
	ret = ts_test->ts->ops.send_cmd(&tmp_cmd);
	if (ret) {
		TS_LOG_ERR("send test mode failed");
		return ret;
	}
	ret = ts_test->ts->ops.read_version(&fw_ver);
	if (ret) {
		TS_LOG_ERR("read test version failed");
		return ret;
	}
	ret = memcmp(&(fw_ver.patch_pid[3]), TEST_FW_PID, strlen(TEST_FW_PID));
	if (ret)
		TS_LOG_ERR("patch ID dismatch %s != %s", fw_ver.patch_pid,
			TEST_FW_PID);
	return ret;
}

#define TEST_PARM_CMD_LEN 0x0A
static void brl_shortcircut_test(struct brl_ts_test *ts_test)
{
	int ret;
	int retry;
	u16 test_time = 0;
	struct brl_ts_cmd test_parm_cmd;
	struct brl_ts_cmd tmp_cmd;

	ts_test->test_result[GTP_SHORT_TEST] = GTP_TEST_PASS;

	ret = brl_short_test_prepare(ts_test);
	if (ret) {
		TS_LOG_ERR("Failed enter short test mode");
		ts_test->test_result[GTP_SHORT_TEST] = SYS_SOFTWARE_REASON;
		return;
	}

	ret = ts_test->ts->ops.i2c_read(SHORT_TEST_TIME_REG,
		(u8 *)&test_time, 2);
	if (ret) {
		TS_LOG_INFO("failed get short test time info, default %dms",
			DEFAULT_TEST_TIME_MS);
		test_time = DEFAULT_TEST_TIME_MS;
	} else {
		test_time = le16_to_cpu(test_time);
		test_time /= 10; /* 10:convert to ms */
		if (test_time > MAX_TEST_TIME_MS) {
			TS_LOG_INFO("test time too long %d > %d",
				test_time, MAX_TEST_TIME_MS);
			test_time = MAX_TEST_TIME_MS;
		}
		TS_LOG_INFO("get test time %dms", test_time);
	}

	memset(&test_parm_cmd, 0, sizeof(test_parm_cmd));
	test_parm_cmd.len = TEST_PARM_CMD_LEN;
	test_parm_cmd.cmd = INSPECT_PARAM_CMD;
	test_parm_cmd.data[0] = DFT_SHORT_THRESHOLD & 0xFF;
	test_parm_cmd.data[1] = (DFT_SHORT_THRESHOLD >> 8) & 0xFF;
	test_parm_cmd.data[2] = DFT_DIFFCODE_SHORT_THRESHOLD & 0xFF;
	test_parm_cmd.data[3] = (DFT_DIFFCODE_SHORT_THRESHOLD >> 8) & 0xFF;
	test_parm_cmd.data[4] = DFT_ADC_DUMP_NUM & 0xFF;
	test_parm_cmd.data[5] = (DFT_ADC_DUMP_NUM >> 8) & 0xFF;
	send_test_cmd(ts_test, &test_parm_cmd);

	memset(&tmp_cmd, 0, sizeof(tmp_cmd));
	retry = 10; /* 10 : 10 times*/
	do {
		ret = ts_test->ts->ops.i2c_read(SHORT_FW_CMD_REG,
			tmp_cmd.buf, sizeof(tmp_cmd.buf));
		if (!ret && tmp_cmd.ack == INSPECT_CMD_ACK_DONE)
			break;
		TS_LOG_INFO("ack not ready 0x%x", tmp_cmd.ack);
		usleep_range(2000, 2100); /* 2000 : 2ms */
	} while (--retry);

	if (tmp_cmd.ack != INSPECT_CMD_ACK_DONE) {
		TS_LOG_ERR("failed get ack ready flag ack 0x%x != 0x%x",
			tmp_cmd.ack, INSPECT_CMD_ACK_DONE);
		ts_test->test_result[GTP_SHORT_TEST] = SYS_SOFTWARE_REASON;
		return;
	}

	msleep(test_time);
	retry = 50;
	do {
		ret = ts_test->ts->ops.i2c_read(SHORT_FW_CMD_REG, tmp_cmd.buf,
				sizeof(tmp_cmd.buf));
		if (!ret && tmp_cmd.state == INSPECT_CMD_STATUS_FINISH)
			break;
		TS_LOG_INFO("short test ack 0x%x status 0x%x",
			tmp_cmd.ack, tmp_cmd.state);
		msleep(50); /* 50 : 50ms */
	} while (--retry);

	if (tmp_cmd.state == INSPECT_CMD_STATUS_FINISH) {
		TS_LOG_INFO("shortcircut test finished");
		ret = brl_shortcircut_analysis(ts_test);
		if (ret)
			ts_test->test_result[GTP_SHORT_TEST] = GTP_PANEL_REASON;
	} else {
		ts_test->test_result[GTP_SHORT_TEST] = SYS_SOFTWARE_REASON;
	}
}

#define LOAD_MBIST_OK 0xAA
static int brl_load_mbist(struct brl_ts_test *ts_test)
{
	int reset_gpio = 0;
	struct brl_ts_version mbist_fw_version;
	struct brl_ts_data *ts = ts_test->ts;
	u8 reg_val[8] = {0};
	u8 temp_buf[64] = {0};
	int retry;
	int r;

	memset(&mbist_fw_version, 0, sizeof(mbist_fw_version));
	reset_gpio = ts->dev_data->ts_platform_data->reset_gpio;
	/* reset IC */
	TS_LOG_INFO("load mbist, reset");
	gpio_direction_output(reset_gpio, 0);
	usleep_range(2000, 2100); /* 2000 : 2ms */
	gpio_direction_output(reset_gpio, 1);

	retry = 100; /* 100: 100 times */
	/* Hold cpu */
	do {
		reg_val[2] = HOLD_MCU_STAT;
		r = ts->ops.i2c_write(HOLD_CPU_REG_W, reg_val, 4);
		r |= ts->ops.i2c_read(HOLD_CPU_REG_R, &temp_buf[0], 4);
		r |= ts->ops.i2c_read(HOLD_CPU_REG_R, &temp_buf[4], 4);
		r |= ts->ops.i2c_read(HOLD_CPU_REG_R, &temp_buf[8], 4);
		if (!r && !memcmp(&temp_buf[0], &temp_buf[4], 4) &&
			!memcmp(&temp_buf[4], &temp_buf[8], 4) &&
			!memcmp(&temp_buf[0], &temp_buf[8], 4)) {
			break;
		}
		usleep_range(1000, 1100); /* 1000 : 1ms */
	} while (--retry);
	if (!retry) {
		TS_LOG_ERR("Failed to hold CPU, return = %d", r);
		return -1;
	}
	TS_LOG_INFO("Success hold CPU");

	/* enable misctl clock */
	reg_val[0] = ENABLE_MISCTL_CLOCK;
	r = ts->ops.i2c_write(MISCTL_REG, reg_val, 1);
	TS_LOG_INFO("enbale misctl clock");

	/* open ESD_KEY */
	retry = 20; /* 20 : 20 times */
	do {
		reg_val[0] = OPEN_ESD_KEY;
		r = ts->ops.i2c_write(ESD_KEY_REG, reg_val, 1);
		if (r != 0)
			TS_LOG_ERR("Failed to write, return = %d", r);
		r = ts->ops.i2c_read(ESD_KEY_REG, temp_buf, 1);
		if (r != 0)
			TS_LOG_ERR("Failed to read, return = %d", r);
		if (!r && temp_buf[0] == OPEN_ESD_OK)
			break;
		usleep_range(1000, 1100); /* 1000 : 1ms */
	} while (--retry);
	if (!retry) {
		TS_LOG_ERR("Failed to enable esd key, return = %d", r);
		return -1;
	}
	TS_LOG_INFO("success enable esd key");

	/* disable watch dog */
	reg_val[0] = DISABLE_WATCHING_DOG;
	r = ts->ops.i2c_write(WATCH_DOG_REG, reg_val, 1);
	TS_LOG_INFO("disable watch dog");

	r = ts->ops.i2c_write(ISP_RAM_ADDR, (u8 *)ts_test->firmware->data,
		ISP_BANK0_SIZE);
	if (r < 0) {
		TS_LOG_ERR("Loading mbist write bank0 error\n");
		return r;
	}
	r = ts->ops.i2c_write(ISP_MBIST_ADDR,
		(u8 *)ts_test->firmware->data + ISP_BANK0_SIZE, ISP_BANK1_SIZE);
	if (r < 0) {
		TS_LOG_ERR("Loading mbist error");
		return r;
	}

	/* SET BOOT OPTION TO 0x55 */
	memset(reg_val, SET_BOOT_OPTION, 8);
	r = ts->ops.i2c_write(HW_REG_CPU_RUN_FROM, reg_val, 8); /* 8:reg len */
	if (r < 0) {
		TS_LOG_ERR("Failed set REG_CPU_RUN_FROM flag");
		return r;
	}
	TS_LOG_INFO("Success write [8]0x55 to 0x%02x", HW_REG_CPU_RUN_FROM);

	gpio_direction_output(reset_gpio, 0);
	usleep_range(2000, 2100); /* 2000 : 2ms */
	gpio_direction_output(reset_gpio, 1);
	usleep_range(20000, 21000); /* 20000 : 20ms */

	for (retry = 0; retry < 20; retry++) { /* 20 : 20 times */
		r = ts->ops.i2c_read(REG_MBIST_TEST_CTRL, reg_val, 1);
		if (!r && reg_val[0] == LOAD_MBIST_OK) {
			TS_LOG_INFO("success load mbist test fw");
			break;
		}
		usleep_range(10000, 11000); /* 10000 : 10ms */
	}

	if (reg_val[0] != LOAD_MBIST_OK) {
		TS_LOG_ERR("mbist state erro 0xD8B8(0x%02x) != 0xAA",
			reg_val[0]);
		return -1;
	}

	if (ts->ops.read_version(&mbist_fw_version)) {
		TS_LOG_ERR("failed read mbist fw version");
		return -1;
	}

	if (memcmp(&mbist_fw_version.patch_pid[0], "MBIST", 5)) {
		TS_LOG_ERR("patch id error %c%c%c%c%c != %s",
		mbist_fw_version.patch_pid[0], mbist_fw_version.patch_pid[1],
		mbist_fw_version.patch_pid[2], mbist_fw_version.patch_pid[3],
		mbist_fw_version.patch_pid[4], "MBIST");
		return -1;
	}
	return 0;
}

static int brl_mbist_request_firmware(struct brl_ts_test *ts_test)
{
	int ret;
	struct device *dev = &brl_ts->pdev->dev;
	char name[GTX8_FW_NAME_LEN + 1] = {0};

	snprintf(name, GTX8_FW_NAME_LEN, "ts/%s_Mbist.bin",
		brl_ts->fw_name_prefix);
	TS_LOG_INFO("%s: file_name = %s\n", __func__, name);

	ret = request_firmware(&ts_test->firmware, name, dev);
	if (ret < 0)
		TS_LOG_ERR("Firmware image %s not available,errno:%d\n",
			name, ret);
	else
		TS_LOG_INFO("Firmware image %s is ready\n", name);
	return ret;
}

static void brl_mbist_release_firmware(struct brl_ts_test *ts_test)
{
	if (ts_test->firmware) {
		release_firmware(ts_test->firmware);
		ts_test->firmware = NULL;
	}
}

#define START_MBIST_TEST 0x00
#define MBIST_TEST_STAT0 0x66
#define MBIST_TEST_STAT1 0xFF
#define MBIST_TEST_OK 0x00
static void brl_mbist_test(struct brl_ts_test *ts_test)
{
	int ret;
	int retry;
	uint8_t reg_val[4];

	ts_test->test_result[GTP_SRAM_TEST] = SYS_SOFTWARE_REASON;

	ret = brl_mbist_request_firmware(ts_test);
	if (ret) {
		TS_LOG_ERR("failed request_firmware");
		return;
	}

	ret = brl_load_mbist(ts_test);
	if (ret) {
		TS_LOG_ERR("failed load mbist fw");
		goto exit;
	}

	reg_val[0] = START_MBIST_TEST;
	ret = ts_test->ts->ops.i2c_write(REG_MBIST_TEST_CTRL, reg_val, 1);
	if (ret) {
		TS_LOG_ERR("failed start mbist test");
		goto exit;
	}

	for (retry = 0; retry < 20; retry++) {
		ret = ts_test->ts->ops.i2c_read(REG_MBIST_TEST_CTRL,
			reg_val, 2);
		if (!ret && reg_val[0] == MBIST_TEST_STAT0)
			break;
		usleep_range(10000, 11000); /* 10000 : 10ms */
	}

	if (ret || reg_val[0] != MBIST_TEST_STAT0 ||
			reg_val[1] == MBIST_TEST_STAT1) {
		TS_LOG_ERR("failed get mbist test result 0x%x 0x%x",
			reg_val[0], reg_val[1]);
		goto exit;
	}

	if (reg_val[1] != MBIST_TEST_OK) {
		ts_test->test_result[GTP_SRAM_TEST] = GTP_PANEL_REASON;
		TS_LOG_ERR("mbist test failed, result 0x%x", reg_val[1]);
	} else {
		ts_test->test_result[GTP_SRAM_TEST] = GTP_TEST_PASS;
		TS_LOG_INFO("mbist test pass");
	}

exit:
	brl_mbist_release_firmware(ts_test);
}

