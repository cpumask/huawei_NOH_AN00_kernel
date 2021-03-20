/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
 * Description: Sensor Hub Channel Bridge
 */

#include <linux/device.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/slab.h>
#include <linux/types.h>

#include <huawei_platform/log/hwlog_kernel.h>

#include "big_data_channel.h"
#include "contexthub_recovery.h"
#include "contexthub_pm.h"
#include "contexthub_ext_log.h"
#include "contexthub_route.h"
#include "sensor_config.h"
#include "sensor_detect.h"
#include "sensor_sysfs.h"

#include "contexthub_debug.h"

#define SOFTIRON_ARGS_NUM   9
#define MUTI_CONVER         2
#define INTERVAL            1000
#define ch_is_digit(ch) ('0' <= (ch) && (ch) <= '9')
#define ch_is_hex(ch) (('A' <= (ch) && (ch) <= 'F') || ('a' <= (ch) && (ch) <= 'f'))
#define ch_is_hexdigit(ch) (ch_is_digit(ch) || ch_is_hex(ch))

static aod_display_pos_t *g_aod_pos;
static char show_str[MAX_STR_SIZE] = {0};
static struct class *iomcu_power;
static DEFINE_MUTEX(mutex_pstatus);
iomcu_power_status i_power_status;

extern int iom3_need_recovery(int modid, exp_source_t f);
extern struct CONFIG_ON_DDR *pConfigOnDDr;
extern struct class *sensors_class;
extern struct als_platform_data als_data;
extern struct ps_platform_data ps_data;
extern struct sar_platform_data sar_pdata;
extern struct adux_sar_add_data_t adux_sar_add_data;
extern char sensor_chip_info[SENSOR_MAX][MAX_CHIP_INFO_LEN];
extern int g_iom3_state;
extern int iom3_power_state;

static struct t_sensor_debug_operations_list sensor_debug_operations_list = {
	.mlock = __MUTEX_INITIALIZER(sensor_debug_operations_list.mlock),
	.head = LIST_HEAD_INIT(sensor_debug_operations_list.head),
};

static char *iomcu_app_id_str[] = {
	[TAG_STEP_COUNTER] = "TAG_STEP_COUNTER",
	[TAG_SIGNIFICANT_MOTION] = "TAG_SIGNIFICANT_MOTION",
	[TAG_STEP_DETECTOR] = "TAG_STEP_DETECTOR",
	[TAG_AR] = "TAG_ACTIVITY",
	[TAG_ORIENTATION] = "TAG_ORIENTATION",
	[TAG_LINEAR_ACCEL] = "TAG_LINEAR_ACCEL",
	[TAG_GRAVITY] = "TAG_GRAVITY",
	[TAG_ROTATION_VECTORS] = "TAG_ROTATION_VECTORS",
	[TAG_GEOMAGNETIC_RV] = "TAG_GEOMAGNETIC_RV",
	[TAG_MOTION] = "TAG_MOTION",
	[TAG_ACCEL] = "TAG_ACCEL",
	[TAG_GYRO] = "TAG_GYRO",
	[TAG_MAG] = "TAG_MAG",
	[TAG_ALS] = "TAG_ALS",
	[TAG_PS] = "TAG_PS",
	[TAG_PRESSURE] = "TAG_PRESSURE",
	[TAG_PDR] = "TAG_PDR",
	[TAG_AR] = "TAG_AR",
	[TAG_FINGERSENSE] = "TAG_FINGERSENSE",
	[TAG_PHONECALL] = "TAG_PHONECALL",
	[TAG_GPS_4774_I2C] = "TAG_GPS_4774_I2C",
	[TAG_MAG_UNCALIBRATED] = "TAG_MAG_UNCALIBRATED",
	[TAG_GYRO_UNCALIBRATED] = "TAG_GYRO_UNCALIBRATED",
	[TAG_HANDPRESS] = "TAG_HANDPRESS",
	[TAG_CA] = "TAG_CA",
	[TAG_OIS] = "TAG_OIS",
	[TAG_FP] = "TAG_FP",
	[TAG_CAP_PROX] = "TAG_CAP_PROX",
	[TAG_KEY] = "TAG_KEY",
	[TAG_AOD] = "TAG_AOD",
	[TAG_MAGN_BRACKET] = "TAG_MAGN_BRACKET",
	[TAG_GPS] = "TAG_GPS",
	[TAG_FLP] = "TAG_FLP",
	[TAG_HINGE] = "TAG_HINGE",
	[TAG_RPC] = "TAG_RPC",
	[TAG_FP_UD] = "TAG_FP_UD",
	[TAG_ACCEL_UNCALIBRATED] = "TAG_ACCEL_UNCALIBRATED",
	[TAG_DROP] = "TAG_DROP",
	[TAG_BIG_DATA] = "TAG_BIG_DATA",
	[TAG_HW_PRIVATE_APP_END] = "TAG_HW_PRIVATE_APP_END",
};

/* to find tag by str */
static const struct sensor_debug_tag_map tag_map_tab[] = {
	{"accel", TAG_ACCEL},
	{"magnitic", TAG_MAG},
	{"gyro", TAG_GYRO},
	{"als_light", TAG_ALS},
	{"ps_promixy", TAG_PS},
	{"linear_accel", TAG_LINEAR_ACCEL},
	{"gravity", TAG_GRAVITY},
	{"orientation", TAG_ORIENTATION},
	{"rotationvector", TAG_ROTATION_VECTORS},
	{"maguncalibrated", TAG_MAG_UNCALIBRATED},
	{"gamerv", TAG_GAME_RV},
	{"gyrouncalibrated", TAG_GYRO_UNCALIBRATED},
	{"significantmotion", TAG_SIGNIFICANT_MOTION},
	{"stepdetector", TAG_STEP_DETECTOR},
	{"stepcounter", TAG_STEP_COUNTER},
	{"geomagnetic", TAG_GEOMAGNETIC_RV},
	{"airpress", TAG_PRESSURE},
	{"handpress", TAG_HANDPRESS},
	{"cap_prox", TAG_CAP_PROX},
	{"hall", TAG_HALL},
	{"fault", TAG_FAULT},
	{"ar", TAG_AR},
	{"fingersense", TAG_FINGERSENSE},
	{"fingerprint", TAG_FP},
	{"key", TAG_KEY},
	{"aod", TAG_AOD},
	{"magn_bracket", TAG_MAGN_BRACKET},
	{"hinge", TAG_HINGE},
	{"environment", TAG_ENVIRONMENT},
	{"fingerprint_ud", TAG_FP_UD},
	{"acceluncalibrated", TAG_ACCEL_UNCALIBRATED},
	{"tof", TAG_TOF},
	{"drop", TAG_DROP},
};

static const char *fault_type_table[] = {
	"hardfault",
	"busfault",
	"memfault",
	"usagefault",
	"rdrdump",
};

static int open_sensor(int tag, int argv[], int argc)
{
	int ret;
	interval_param_t delay_param = {
		.period = argv[0],
		.batch_count = 1,
		.mode = AUTO_MODE,
		.reserved[0] = TYPE_STANDARD /* for step counter only */
	};

	if (tag == -1)
		return -1;

	if (ap_sensor_enable(tag, true))
		return 0;

	hwlog_info("open sensor %d\n", tag);
	if (tag == TAG_STEP_COUNTER)
		ret = inputhub_sensor_enable_stepcounter(true, TYPE_STANDARD);
	else
		ret = inputhub_sensor_enable(tag, true);
	if (!ret && (argc > 0)) {
		hwlog_info("set sensor %d delay %d ms\n", tag, argv[0]);
		ret = inputhub_sensor_setdelay(tag, &delay_param);
	}

	return ret;
}

static int set_delay(int tag, int argv[], int argc)
{
	interval_param_t delay_param = {
		.period = argv[0],
		.batch_count = 1,
		.mode = AUTO_MODE,
		.reserved[0] = TYPE_STANDARD /* for step counter only */
	};

	if (tag == -1 || argc == 0)
		return -1;

	if (ap_sensor_setdelay(tag, argv[0]))
		return 0;

	hwlog_info("set sensor %d delay %d ms\n", tag, argv[0]);
	inputhub_sensor_setdelay(tag, &delay_param);
	return 0;
}

static int close_sensor(int tag, int argv[], int argc)
{
	if (tag == -1)
		return -1;

	if (ap_sensor_enable(tag, false))
		return 0;

	hwlog_info("close sensor %d\n", tag);
	if (tag == TAG_STEP_COUNTER)
		inputhub_sensor_enable_stepcounter(false, TYPE_STANDARD);
	else
		inputhub_sensor_enable(tag, false);

	return 0;
}

static int set_sensor_slave_addr(int tag, int argv[], int argc)
{
	int ret;
	unsigned int i2c_address;
	write_info_t pkg_ap;
	read_info_t pkg_mcu;
	pkt_parameter_req_t cpkt;
	pkt_header_t *hd = (pkt_header_t *)&cpkt;

	if (argc == 0)
		return -1;

	i2c_address = (unsigned int)argv[0] & 0xFF;
	memset(&pkg_ap, 0, sizeof(pkg_ap));
	memset(&pkg_mcu, 0, sizeof(pkg_mcu));
	pkg_ap.tag = tag;
	pkg_ap.cmd = CMD_CMN_CONFIG_REQ;
	cpkt.subcmd = SUB_CMD_SET_SLAVE_ADDR_REQ;
	pkg_ap.wr_buf = &hd[1];
	pkg_ap.wr_len = sizeof(i2c_address) + SUBCMD_LEN;
	memcpy(cpkt.para, &i2c_address, sizeof(i2c_address));
	hwlog_info("%s, %s, i2c_addr:0x%x\n", __func__, obj_tag_str[tag],
		i2c_address);

	ret = write_customize_cmd(&pkg_ap, &pkg_mcu, true);
	if (ret != 0) {
		hwlog_err("set %s slave addr failed, ret = %d in %s\n",
			obj_tag_str[tag], ret, __func__);
		return -1;
	}
	if (pkg_mcu.errno != 0)
		hwlog_err("set %s slave addr failed errno = %d in %s\n",
			obj_tag_str[tag], pkg_mcu.errno, __func__);
	else
		hwlog_info("set %s new slave addr:0x%x success\n",
			obj_tag_str[tag], i2c_address);
	return 0;
}

static int set_sensor_softiron(int tag, int argv[], int argc)
{
	int i;
	int ret;
	write_info_t pkg_ap;
	pkt_parameter_req_t cpkt;
	pkt_header_t *hd = (pkt_header_t *)&cpkt;
	int gyro_args_to_mcu[SOFTIRON_ARGS_NUM * MUTI_CONVER] = {0};

	hwlog_info("sensorhub_dbg set_sensor_paramet() argc = %d\n", argc);
	if (argc != SOFTIRON_ARGS_NUM)
		return -1;

	for (i = 0; i < SOFTIRON_ARGS_NUM; i++) {
		hwlog_info("%d\n", argv[i]);
		gyro_args_to_mcu[i * MUTI_CONVER] =
			(unsigned char)(((unsigned short int)argv[i]) & 0x000000FF);
		gyro_args_to_mcu[i * MUTI_CONVER + 1] =
			(unsigned char)((((unsigned short int)argv[i]) & 0x0000FF00) >> 8);
		hwlog_info("%d %d\n", gyro_args_to_mcu[i * MUTI_CONVER],
			gyro_args_to_mcu[i * MUTI_CONVER + 1]);
	}

	memset(&pkg_ap, 0, sizeof(pkg_ap));
	memset(&cpkt, 0, sizeof(cpkt));

	pkg_ap.tag = tag;
	pkg_ap.cmd = CMD_CMN_CONFIG_REQ;
	cpkt.subcmd = SUB_CMD_ADDITIONAL_INFO;
	pkg_ap.wr_buf = &hd[1];
	pkg_ap.wr_len = sizeof(int) * SOFTIRON_ARGS_NUM * MUTI_CONVER +
		SUBCMD_LEN;
	memcpy(cpkt.para, gyro_args_to_mcu,
		sizeof(int) * SOFTIRON_ARGS_NUM * MUTI_CONVER);

	ret = write_customize_cmd(&pkg_ap, NULL, false);
	if (ret != 0) {
		hwlog_err("set %s sensor_softiron failed, ret = %d in %s\n",
			obj_tag_str[tag], ret, __func__);
		return -1;
	}
	return 0;
}

static void iomcu_big_data_flush(uint32_t event_id)
{
	int ret;
	write_info_t pkg_ap;

	memset(&pkg_ap, 0, sizeof(pkg_ap));
	pkg_ap.tag = TAG_BIG_DATA;
	pkg_ap.cmd = CMD_BIG_DATA_SEND_TO_AP;
	pkg_ap.wr_buf = &event_id;
	pkg_ap.wr_len = sizeof(event_id);

	ret = write_customize_cmd(&pkg_ap, NULL, false);
	if (ret != 0)
		hwlog_err("%s failed, ret = %d\n", __func__, ret);
}

static int big_data_test(int tag, int argv[], int argc)
{
	uint32_t def;
	uint32_t sel;
	uint64_t fetch_data = 0;

	if (argc != 2)
		return -1;

	def = argv[0];
	sel = argv[1];

	if (sel == 0) {
		iomcu_big_data_flush(def);
	} else {
		iomcu_dubai_log_fetch(def, &fetch_data, sizeof(fetch_data));
		hwlog_info("big data test type=%d, res=hi:%d, low:%d\n", def,
			(uint32_t)(fetch_data >> 32), (uint32_t)(fetch_data));
	}

	return 0;
}

static int set_sensor_data_mode(int tag, int argv[], int argc)
{
	int ret;
	write_info_t pkg_ap;
	pkt_parameter_req_t cpkt;
	pkt_header_t *hd = (pkt_header_t *)&cpkt;

	if (argc != 1)
		return -1;
	memset(&pkg_ap, 0, sizeof(pkg_ap));
	memset(&cpkt, 0, sizeof(cpkt));
	pkg_ap.tag = tag;
	pkg_ap.cmd = CMD_CMN_CONFIG_REQ;
	cpkt.subcmd = SUB_CMD_SET_DATA_MODE;
	pkg_ap.wr_buf = &hd[1];
	pkg_ap.wr_len = sizeof(int) * SOFTIRON_ARGS_NUM * MUTI_CONVER +
		SUBCMD_LEN;
	cpkt.para[0] = (int)argv[0];

	ret = write_customize_cmd(&pkg_ap, NULL, false);
	if (ret != 0) {
		hwlog_err("set %s sensor_mode failed, ret = %d in %s\n",
			obj_tag_str[tag], ret, __func__);
		return -1;
	}
	return 0;
}

int set_log_level(int tag, int argv[], int argc)
{
	int ret;
	write_info_t pkg_ap;
	uint32_t log_level;

	if (argc != 1)
		return -1;

	log_level = argv[0];

	if (log_level > DEBUG_LEVEL)
		return -1;

	memset(&pkg_ap, 0, sizeof(pkg_ap));

	pkg_ap.tag = TAG_SYS;
	pkg_ap.cmd = CMD_SYS_LOG_LEVEL_REQ;
	pkg_ap.wr_buf = &log_level;
	pkg_ap.wr_len = sizeof(log_level);

	ret = write_customize_cmd(&pkg_ap, NULL, true);
	if (ret != 0) {
		hwlog_err("%s faile to write cmd\n", __func__);
		return -1;
	}

	pConfigOnDDr->log_level = log_level;
	hwlog_info("%s set log level %d success\n", __func__, log_level);
	return 0;
}

static int set_fault_type(int tag, int argv[], int argc)
{
	int ret;
	write_info_t pkg_ap;
	uint8_t fault_type;

	if (argc == 0)
		return -1;

	fault_type = (unsigned int)argv[0] & 0xFF;
	if (fault_type >= (sizeof(fault_type_table) / sizeof(fault_type_table[0]))) {
		hwlog_err("unsupported fault_type : %d\n", fault_type);
		return -1;
	}

	memset(&pkg_ap, 0, sizeof(pkg_ap));

	pkg_ap.tag = TAG_FAULT;
	pkg_ap.cmd = CMD_SET_FAULT_TYPE_REQ;
	pkg_ap.wr_buf = &fault_type;
	pkg_ap.wr_len = sizeof(fault_type);

	hwlog_info("%s,%s,fault type:%s\n", __func__, obj_tag_str[TAG_FAULT],
		fault_type_table[fault_type]);
	ret = write_customize_cmd(&pkg_ap, NULL, true);
	if (ret != 0) {
		hwlog_err("set fault type %s failed, ret = %d in %s\n",
			fault_type_table[fault_type], ret, __func__);
		return -1;
	}
	hwlog_info("set fault type %s succ\n", fault_type_table[fault_type]);
	return 0;
}

static int set_fault_addr(int tag, int argv[], int argc)
{
	int ret;
	write_info_t pkg_ap;
	pkt_fault_addr_req_t cpkt;
	pkt_header_t *hd = (pkt_header_t *)&cpkt;

	if (argc == 0)
		return -1;

	memset(&pkg_ap, 0, sizeof(pkg_ap));

	pkg_ap.tag = TAG_FAULT;
	pkg_ap.cmd = CMD_SET_FAULT_ADDR_REQ;
	cpkt.wr = (unsigned int)argv[0] & 0xFF;
	cpkt.fault_addr = argv[1];
	pkg_ap.wr_buf = &hd[1];
	pkg_ap.wr_len = 5;

	ret = write_customize_cmd(&pkg_ap, NULL, true);
	if (ret != 0) {
		hwlog_err("%s set addr, read/write:%d, 0x%x err, ret=%d\n",
			__func__, argv[0], argv[1], ret);
		return -1;
	}
	hwlog_info("set fault addr,read/write:%d,fault addr:0x%x succ\n",
		argv[0], argv[1]);
	return 0;
}

static int aod_test(int tag, int argv[], int argc)
{
	int count_res;
	write_info_t pkg_ap;
	read_info_t pkg_mcu;
	aod_req_t cpkt;
	pkt_header_t *hd = (pkt_header_t *)&cpkt;
	aod_display_pos_t *aod_pos = NULL;
	struct timespec now;

	if (argc == 0)
		return -1;

	memset(&pkg_ap, 0, sizeof(pkg_ap));
	memset(&pkg_mcu, 0, sizeof(pkg_mcu));

	pkg_ap.tag = TAG_AOD;
	pkg_ap.cmd = CMD_CMN_CONFIG_REQ;
	if (argv[0] == 1) {
		cpkt.sub_cmd = SUB_CMD_AOD_SET_DISPLAY_SPACE_REQ;
		cpkt.display_param.display_spaces[0].x_start =
			AOD_SINGLE_CLOCK_OFFSET_X;
		cpkt.display_param.display_spaces[0].y_start =
			AOD_SINGLE_CLOCK_OFFSET_Y;
		cpkt.display_param.display_spaces[0].x_size =
			AOD_SINGLE_CLOCK_RES_X;
		cpkt.display_param.display_spaces[0].y_size =
			AOD_SINGLE_CLOCK_RES_Y;
		cpkt.display_param.dual_clocks = 0;
		cpkt.display_param.display_type = 0;
		cpkt.display_param.display_space_count = 5;
		cpkt.display_param.display_spaces[1].x_start =
			TIME_DIGIT_HOUR_OFFSET;
		cpkt.display_param.display_spaces[2].x_start =
			TIME_DIGIT_HOUR_OFFSET + AOD_SINGLE_CLOCK_DIGITS_RES_X;
		cpkt.display_param.display_spaces[3].x_start =
			TIME_DIGIT_MINU_OFFSET;
		cpkt.display_param.display_spaces[4].x_start =
			TIME_DIGIT_MINU_OFFSET + AOD_SINGLE_CLOCK_DIGITS_RES_X;
		for (count_res = 1; count_res < 5; count_res++) {
			cpkt.display_param.display_spaces[count_res].y_start =
				TIME_DIGIT_TIME_OFFSET;
			cpkt.display_param.display_spaces[count_res].x_size =
				AOD_SINGLE_CLOCK_DIGITS_RES_X;
			cpkt.display_param.display_spaces[count_res].y_size =
				AOD_SINGLE_CLOCK_DIGITS_RES_Y;
		}
		hwlog_info("@@@@@%ld %ld\n", sizeof(cpkt.display_param) +
			sizeof(cpkt.sub_cmd), sizeof(cpkt) - sizeof(cpkt.hd));
		pkg_ap.wr_len = sizeof(cpkt) - sizeof(cpkt.hd);
	} else if (argv[0] == 2) {
		cpkt.sub_cmd = SUB_CMD_AOD_SETUP_REQ;
		cpkt.config_param.aod_fb = 0;
		cpkt.config_param.screen_info.xres = SCREEN_RES_X;
		cpkt.config_param.screen_info.yres = SCREEN_RES_Y;
		cpkt.config_param.screen_info.pixel_format =
			AOD_DRV_PIXEL_FORMAT_RGB_565;
		cpkt.config_param.aod_digits_addr = DIGITS_BITMAPS_OFFSET;
		cpkt.config_param.bitmap_size.bitmap_type_count = 2;
		cpkt.config_param.bitmap_size.bitmap_size[0].xres =
			AOD_SINGLE_CLOCK_DIGITS_RES_X;
		cpkt.config_param.bitmap_size.bitmap_size[0].yres =
			AOD_SINGLE_CLOCK_DIGITS_RES_Y;
		cpkt.config_param.bitmap_size.bitmap_size[1].xres = 0;
		cpkt.config_param.bitmap_size.bitmap_size[1].yres = 0;
		pkg_ap.wr_len = sizeof(cpkt) - sizeof(cpkt.hd);
		hwlog_info("@@@@@%ld %ld\n", sizeof(cpkt.config_param) +
			sizeof(cpkt.sub_cmd), sizeof(cpkt) - sizeof(cpkt.hd));
	} else if (argv[0] == 3) {
		cpkt.sub_cmd = SUB_CMD_AOD_SET_TIME_REQ;
		getnstimeofday(&now);
		cpkt.time_param.curr_time = now.tv_nsec;
		cpkt.time_param.time_format = 0;
		cpkt.time_param.time_zone = -30;
		cpkt.time_param.sec_time_zone = 0;
		pkg_ap.wr_len = sizeof(cpkt) - sizeof(cpkt.hd);
		hwlog_info("@@@@@%ld %ld\n", sizeof(cpkt.time_param) +
			sizeof(cpkt.sub_cmd), sizeof(cpkt) - sizeof(cpkt.hd));
	} else if (argv[0] == 4) {
		cpkt.sub_cmd = SUB_CMD_AOD_START_REQ;
		cpkt.start_param.intelli_switching = 1;
		cpkt.start_param.aod_pos.x_start = 0;
		cpkt.start_param.aod_pos.y_start = 0;
		pkg_ap.wr_len = sizeof(cpkt) - sizeof(cpkt.hd);
		hwlog_info("@@@@@%ld %ld\n", sizeof(cpkt.start_param) +
			sizeof(cpkt.sub_cmd), sizeof(cpkt) - sizeof(cpkt.hd));
	} else if (argv[0] == 5) {
		cpkt.sub_cmd = SUB_CMD_AOD_STOP_REQ;
		pkg_ap.wr_len = sizeof(cpkt.sub_cmd);
	} else if (argv[0] == 6) {
		cpkt.sub_cmd = SUB_CMD_AOD_START_UPDATING_REQ;
		pkg_ap.wr_len = sizeof(cpkt.sub_cmd);
	} else if (argv[0] == 7) {
		cpkt.sub_cmd = SUB_CMD_AOD_END_UPDATING_REQ;
		if (g_aod_pos) {
			cpkt.display_pos.x_start = g_aod_pos->x_start;
			cpkt.display_pos.y_start = g_aod_pos->y_start;
		}
		pkg_ap.wr_len = sizeof(cpkt) - sizeof(cpkt.hd);
		hwlog_info("@@@@@%ld %ld\n", sizeof(cpkt.display_pos) +
			sizeof(cpkt.sub_cmd), sizeof(cpkt) - sizeof(cpkt.hd));
	} else {
		hwlog_err("error cmd %d.\n", argv[0]);
	}
	pkg_ap.wr_buf = &hd[1];
	hwlog_info("%s aod cmd %d sent.\n", __func__, argv[0]);
	count_res = write_customize_cmd(&pkg_ap, &pkg_mcu, true);
	if (count_res != 0) {
		hwlog_err("%s aod test failed, cmd is %d, ret = %d.\n",
			__func__, argv[0], count_res);
		return -1;
	}
	if (pkg_mcu.errno != 0) {
		hwlog_err("%s cmd is %d, errno = %d\n", __func__, argv[0],
			pkg_mcu.errno);
		return -1;
	} else if (argv[0] == 5 || argv[0] == 6) {
		aod_pos = (aod_display_pos_t *)&pkg_mcu.data;
		if (argv[0] == 6)
			g_aod_pos = (aod_display_pos_t *)&pkg_mcu.data;
		hwlog_info("%s aod pos x is %d, y is %d.\n",  __func__,
			aod_pos->x_start, aod_pos->y_start);
	}
	hwlog_info("%s aod cmd is %d success.\n", __func__, argv[0]);
	return 0;
}

static int activity_common_handle(const int argv[], write_info_t *pkg_ap,
	ar_stop_cmd_t *ar_stop, unsigned long long activity)
{
	ar_start_cmd_t *ar_start = NULL;
	ar_config_event_t *activity_list = NULL;
	unsigned int start_len = 0;
	unsigned int i;
	unsigned char cmd = (unsigned int)argv[0] & 0xff;
	unsigned char core = ((unsigned int)argv[0] & 0xff00) >> 8;
	unsigned int interval = ((unsigned int)argv[0] & 0xff0000) >> 16;

	if ((cmd == CMD_CMN_OPEN_REQ) || (cmd == CMD_CMN_CLOSE_REQ)) {
		pkg_ap->cmd = cmd;
		pkg_ap->wr_buf = NULL;
		pkg_ap->wr_len = 0;
	} else if ((cmd == SUB_CMD_FLP_AR_START_REQ) ||
		(cmd == SUB_CMD_FLP_AR_UPDATE_REQ)) {
		pkg_ap->cmd = CMD_CMN_CONFIG_REQ;
		ar_start = kzalloc(sizeof(ar_start_cmd_t) + (AR_UNKNOWN + 1) *
			(sizeof(ar_config_event_t) + sizeof(char)), GFP_KERNEL);
		if (!ar_start)
			return -1;
		ar_start->core_cmd.sub_cmd = cmd;
		ar_start->core_cmd.core = core;
		ar_start->start_param.report_interval =
			(interval > UINT_MAX / INTERVAL) ? UINT_MAX :
			(interval * INTERVAL);

		for (i = 0; i < AR_UNKNOWN; i++) {
			if (activity & (1ULL << i)) {
				activity_list = kzalloc(sizeof(ar_config_event_t) + sizeof(char),
					GFP_KERNEL);
				if (!activity_list)
					break;

				activity_list->activity = i;
				activity_list->event_type = EVENT_BOTH;
				activity_list->len = sizeof(char);
				activity_list->buf[0] = (char)i;
				memcpy((char *)ar_start + sizeof(ar_start_cmd_t) +
					start_len, activity_list,
					sizeof(ar_config_event_t) + sizeof(char));
				start_len += sizeof(ar_config_event_t) + sizeof(char);
				ar_start->start_param.num++;
				kfree(activity_list);
			}
		}
		hwlog_info("receive activity num %d in %s\n",
			ar_start->start_param.num, __func__);
		pkg_ap->wr_buf = ar_start;
		pkg_ap->wr_len = (int)(sizeof(ar_start_cmd_t) +
			ar_start->start_param.num *
			(sizeof(ar_config_event_t) + sizeof(char)));
	} else {
		pkg_ap->cmd = CMD_CMN_CONFIG_REQ;
		ar_stop->core_cmd.sub_cmd = cmd;
		ar_stop->core_cmd.core = core;
		if (interval > UINT_MAX / INTERVAL)
			ar_stop->para = UINT_MAX;
		else
			ar_stop->para = interval * INTERVAL;
		pkg_ap->wr_buf = ar_stop;
		pkg_ap->wr_len = sizeof(*ar_stop);
	}
	kfree(ar_start);
	hwlog_info("set mcucmd=%d core=%d interval=%d activity=0x%llx\n",
		cmd, core, interval, activity);
	return 0;
}

static int activity_common_test(int tag, int argv[], int argc)
{
	int ret;
	unsigned long long activity = 0;
	write_info_t pkg_ap;
	read_info_t pkg_mcu;
	ar_stop_cmd_t ar_stop;

	if (argc == 3)
		activity = (unsigned long long)argv[2] & 0xffffffff;
	activity = (activity << 32) | ((unsigned long long)argv[1] & 0xffffffff);

	memset(&pkg_ap, 0, sizeof(pkg_ap));
	memset(&pkg_mcu, 0, sizeof(pkg_mcu));
	memset(&ar_stop, 0, sizeof(ar_stop));

	pkg_ap.tag = tag;

	ret = activity_common_handle(argv, &pkg_ap, &ar_stop,
		activity);
	if (ret != 0)
		return ret;

	ret = write_customize_cmd(&pkg_ap, &pkg_mcu, true);
	hwlog_info("set mcu cmd errno=%d tag=%d\n", pkg_mcu.errno, tag);
	return ret;
}

static int environment_test(int tag, int argv[], int argc)
{
	int ret;

	ret = activity_common_test(TAG_ENVIRONMENT, argv, argc);
	if (ret)
		hwlog_err("%s send cmd to mcu fail,ret=%d\n", __func__, ret);
	return ret;
}

static int ar_test(int tag, int argv[], int argc)
{
	int ret;

	ret = activity_common_test(TAG_AR, argv, argc);
	if (ret)
		hwlog_err("%s send cmd to mcu fail,ret=%d\n", __func__, ret);
	return ret;
}

static int set_ps_type(int tag, int argv[], int argc)
{
	int ret;
	unsigned int type = argv[0];
	write_info_t pkg_ap;
	read_info_t pkg_mcu;
	pkt_parameter_req_t spkt;
	pkt_header_t *shd = (pkt_header_t *)&spkt;

	memset(&pkg_ap, 0, sizeof(pkg_ap));
	memset(&pkg_mcu, 0, sizeof(pkg_mcu));
	memset(&spkt, 0, sizeof(spkt));
	hwlog_info("%s: data type is %d(0.nor_data1.raw_data)\n",
		__func__, argv[0]);
	if (type > SET_PS_TYPE_NUMB_MAX) {
		hwlog_err("%s:set data type fail\n", __func__);
		return -1;
	}
	pkg_ap.tag = TAG_PS;
	spkt.subcmd = SUB_CMD_SET_DATA_TYPE_REQ;
	pkg_ap.cmd = CMD_CMN_CONFIG_REQ;
	pkg_ap.wr_buf = &shd[1];
	pkg_ap.wr_len = sizeof(type) + SUBCMD_LEN;
	memcpy(spkt.para, &type, sizeof(type));
	ret = write_customize_cmd(&pkg_ap, &pkg_mcu, true);
	if (pkg_mcu.errno != 0)
		hwlog_err("send tag ps cmd to mcu fail,ret=%d\n", ret);
	return ret;
}

static int set_als_type(int tag, int argv[], int argc)
{
	int ret;
	unsigned int type = argv[0];
	write_info_t pkg_ap;
	read_info_t pkg_mcu;
	pkt_parameter_req_t spkt;
	pkt_header_t *shd = (pkt_header_t *)&spkt;

	memset(&pkg_ap, 0, sizeof(pkg_ap));
	memset(&pkg_mcu, 0, sizeof(pkg_mcu));
	memset(&spkt, 0, sizeof(spkt));
	hwlog_info("%s:data type is %d(0.nor_data1.raw_data)\n",
		__func__, argv[0]);
	if (type > SET_ALS_TYPE_NUMB_MAX) {
		hwlog_err("%s:set data type fail, inval val\n", __func__);
		return -1;
	}
	pkg_ap.tag = TAG_ALS;
	spkt.subcmd = SUB_CMD_SET_DATA_TYPE_REQ;
	pkg_ap.cmd = CMD_CMN_CONFIG_REQ;
	pkg_ap.wr_buf = &shd[1];
	pkg_ap.wr_len = sizeof(type) + SUBCMD_LEN;
	memcpy(spkt.para, &type, sizeof(type));
	ret = write_customize_cmd(&pkg_ap, &pkg_mcu, true);
	if (pkg_mcu.errno != 0)
		hwlog_err("send tag als cmd to mcu fail,ret=%d\n", ret);
	return ret;
}
static int als_param_write(int tag, int argv[], int argc)
{
	int count_res;
	int param_num = argv[0];
	s16 als_para[30]; /* bh is 25 ,apds is 21, tmd is 29 */
	write_info_t pkg_ap;
	read_info_t pkg_mcu;
	pkt_parameter_req_t spkt;
	pkt_header_t *shd = (pkt_header_t *)&spkt;

	memset(&pkg_ap, 0, sizeof(pkg_ap));
	memset(&pkg_mcu, 0, sizeof(pkg_mcu));
	memset(&als_para, 0, sizeof(als_para));
	memset(&spkt, 0, sizeof(spkt));
	if (param_num > 29) {
		hwlog_err("%s:param_num %d inval\n", __func__, param_num);
		return -1;
	}
	for (count_res = 0; count_res < param_num; count_res++) {
		als_para[count_res] = argv[count_res + 1];
		hwlog_info("als_par[%d]:%d\n", count_res, als_para[count_res]);
	}
	for (count_res = 0; count_res < param_num; count_res++) {
		if (als_para[count_res] > MAX_SINGNED_SHORT ||
			als_para[count_res] < MIN_SINGNED_SHORT) {
			hwlog_err("%s:als param data invalid\n", __func__);
			return -1;
		}
	}
	memcpy(als_data.als_extend_data, als_para, ((sizeof(s16) * param_num >
		SENSOR_PLATFORM_EXTEND_ALS_DATA_SIZE) ?
		SENSOR_PLATFORM_EXTEND_ALS_DATA_SIZE :
		(sizeof(s16) * param_num)));
	spkt.subcmd = SUB_CMD_SET_PARAMET_REQ;
	pkg_ap.tag = TAG_ALS;
	pkg_ap.cmd = CMD_CMN_CONFIG_REQ;
	pkg_ap.wr_buf = &shd[1];
	pkg_ap.wr_len = sizeof(als_data) + SUBCMD_LEN;
	memcpy(spkt.para, &als_data, sizeof(als_data));

	hwlog_info("%s g_iom3_state = %d,tag =%d ,cmd =%d\n", __func__,
		g_iom3_state, pkg_ap.tag, pkg_ap.cmd);

	if (g_iom3_state == IOM3_ST_RECOVERY || iom3_power_state == ST_SLEEP)
		count_res = write_customize_cmd(&pkg_ap, NULL, false);
	else
		count_res = write_customize_cmd(&pkg_ap, &pkg_mcu, true);

	if (count_res) {
		hwlog_err("send tag %d to mcu err,ret=%d\n", pkg_ap.tag, count_res);
	} else {
		if (pkg_mcu.errno != 0)
			hwlog_err("send ALS param to mcu fail\n");
		else
			hwlog_info("send ALS param to mcu succes\n");
	}
	return count_res;
}

static int ps_param_write(int tag, int argv[], int argc)
{
	int ret;
	int pwindows_value = argv[0];
	int pwave_value = argv[1];
	write_info_t pkg_ap;
	read_info_t pkg_mcu;
	pkt_parameter_req_t spkt;
	pkt_header_t *shd = (pkt_header_t *)&spkt;

	memset(&pkg_ap, 0, sizeof(pkg_ap));
	memset(&pkg_mcu, 0, sizeof(pkg_mcu));
	memset(&spkt, 0, sizeof(spkt));

	if (pwindows_value < 0 || pwave_value < 0 || argv[2] < 0 ||
		pwindows_value > MAX_SINGNED_SHORT ||
		pwave_value > MAX_SINGNED_SHORT ||
		argv[2] > MAX_SINGNED_SHORT) {
		hwlog_err("%s is fail %d %d %d\n", __func__, pwindows_value,
			pwave_value, argv[2]);
		return -1;
	}

	ps_data.pwindows_value = pwindows_value;
	ps_data.pwave_value = pwave_value;
	ps_data.threshold_value = argv[2];
	spkt.subcmd = SUB_CMD_SET_PARAMET_REQ;
	pkg_ap.tag = TAG_PS;
	pkg_ap.cmd = CMD_CMN_CONFIG_REQ;
	pkg_ap.wr_buf = &shd[1];
	pkg_ap.wr_len = sizeof(ps_data) + SUBCMD_LEN;
	memcpy(spkt.para, &ps_data, sizeof(ps_data));

	hwlog_info("%s g_iom3_state = %d,tag =%d ,cmd =%d\n", __func__,
		g_iom3_state, pkg_ap.tag, pkg_ap.cmd);

	if (g_iom3_state == IOM3_ST_RECOVERY || iom3_power_state == ST_SLEEP)
		ret = write_customize_cmd(&pkg_ap, NULL, false);
	else
		ret = write_customize_cmd(&pkg_ap, &pkg_mcu, true);

	if (ret) {
		hwlog_err("send tag %d to mcu fail,ret=%d\n", pkg_ap.tag, ret);
	} else {
		if (pkg_mcu.errno != 0)
			hwlog_err("send PS param to mcu fail\n");
		else
			hwlog_info("send PS param to mcu succes\n");
	}
	return ret;
}

static int sar_param_set(int tag, int argv[], int argc)
{
	int i;
	int ret;
	write_info_t pkg_ap;
	read_info_t pkg_mcu;
	pkt_parameter_req_t spkt;
	pkt_header_t *shd = (pkt_header_t *)&spkt;

	memset(&pkg_ap, 0, sizeof(pkg_ap));
	memset(&pkg_mcu, 0, sizeof(pkg_mcu));
	memset(&spkt, 0, sizeof(spkt));

	if (argv[0] == SAR_SET_REGISTER) {
		if ((argc - 1) > ADI_SAR_INIT_REG_VAL_LENGTH) {
			hwlog_err("%s:input data big than init_reg_val\n",
				__func__);
			return -1;
		}
		spkt.subcmd = SUB_CMD_SET_PARAMET_REQ;
		for (i = 1; i < argc; i++)
			sar_pdata.sar_datas.adux_data.init_reg_val[i - 1] =
				argv[i];
		memcpy(spkt.para, &sar_pdata, sizeof(sar_pdata));
	}

	if (argv[0] == SAR_SET_THRESHOLD) {
		if ((argc - 1) > ADI_SAR_THRESHOLD_TO_MODEM_LENGTH * STG_SUPPORTED_NUM) {
			hwlog_err("%s:input data big than threshold_tomodem\n",
				__func__);
			return -1;
		}
		spkt.subcmd = SUB_CMD_SET_ADD_DATA_REQ;
		memset(&adux_sar_add_data, 0x00, sizeof(adux_sar_add_data));
		for (i = 1; i < argc; i++)
			adux_sar_add_data.threshold_to_modem_stg[i - 1] =
				argv[i];
		memcpy(spkt.para, &adux_sar_add_data,
			sizeof(adux_sar_add_data));
	}

	pkg_ap.tag = TAG_CAP_PROX;
	pkg_ap.cmd = CMD_CMN_CONFIG_REQ;
	pkg_ap.wr_buf = &shd[1];
	pkg_ap.wr_len = sizeof(sar_pdata) + SUBCMD_LEN;

	hwlog_info("%s g_iom3_state = %d,tag =%d ,cmd =%d\n", __func__,
		g_iom3_state, pkg_ap.tag, pkg_ap.cmd);

	if (g_iom3_state == IOM3_ST_RECOVERY || iom3_power_state == ST_SLEEP)
		ret = write_customize_cmd(&pkg_ap, NULL, false);
	else
		ret = write_customize_cmd(&pkg_ap, &pkg_mcu, true);

	if (ret) {
		hwlog_err("send tag %d cfg data to mcu fail,ret=%d\n",
			pkg_ap.tag, ret);
	} else {
		if (pkg_mcu.errno != 0)
			hwlog_err("send sar param to mcu fail\n");
		else
			hwlog_info("send sar param to mcu succes\n");
	}
	return ret;
}

static int sar_param_write(int tag, int argv[], int argc)
{
	int i;
	int ret;
	write_info_t pkg_ap;
	read_info_t pkg_mcu;
	pkt_parameter_req_t spkt;
	pkt_header_t *shd = (pkt_header_t *)&spkt;

	memset(&pkg_ap, 0, sizeof(pkg_ap));
	memset(&pkg_mcu, 0, sizeof(pkg_mcu));
	memset(&spkt, 0, sizeof(spkt));

	if (!strncmp(sensor_chip_info[CAP_PROX], "huawei,semtech-sx9323",
		strlen("huawei,semtech-sx9323"))) {
		if (argv[0] == SAR_SET_REGISTER) {
			if ((argc - 1) > SEMTECH_SAR_INIT_REG_VAL_LENGTH) {
				hwlog_err("%s:input big than init_reg_val\n",
					__func__);
				return -1;
			}
			for (i = 1; i < argc; i++)
				sar_pdata.sar_datas.semteck_data.init_reg_val[i - 1] = argv[i];
		}
		if (argv[0] == SAR_SET_THRESHOLD) {
			if ((argc - 1) > SEMTECH_SAR_THRESHOLD_TO_MODEM_LENGTH) {
				hwlog_err("%s:input big than modom array\n",
					__func__);
				return -1;
			}
			for (i = 1; i < argc; i++)
				sar_pdata.sar_datas.semteck_data.threshold_to_modem[i - 1] = argv[i];
		}
		if (argv[0] == SAR_SET_THRESHOLD_AND_REGISTER) {
			if ((argc - 1) > SEMTECH_SAR_INIT_REG_VAL_LENGTH +
				SEMTECH_SAR_THRESHOLD_TO_MODEM_LENGTH) {
				hwlog_err("%s input data size too larged\n",
					__func__);
				return -1;
			}
			for (i = 1; i < SEMTECH_SAR_THRESHOLD_TO_MODEM_LENGTH + 1; i++)
				sar_pdata.sar_datas.semteck_data.threshold_to_modem[i - 1] = argv[i];
			for (i = SEMTECH_SAR_THRESHOLD_TO_MODEM_LENGTH + 1; i < argc; i++)
				sar_pdata.sar_datas.semteck_data.init_reg_val[i - 1 - SEMTECH_SAR_THRESHOLD_TO_MODEM_LENGTH] = argv[i];
		}
	} else {
		hwlog_err("%s:sar not support the operate\n", __func__);
		return -1;
	}

	spkt.subcmd = SUB_CMD_SET_PARAMET_REQ;
	pkg_ap.tag = TAG_CAP_PROX;
	pkg_ap.cmd = CMD_CMN_CONFIG_REQ;
	pkg_ap.wr_buf = &shd[1];
	pkg_ap.wr_len = sizeof(sar_pdata) + SUBCMD_LEN;
	memcpy(spkt.para, &sar_pdata, sizeof(sar_pdata));

	hwlog_info("%s g_iom3_state = %d,tag =%d ,cmd =%d\n", __func__,
		g_iom3_state, pkg_ap.tag, pkg_ap.cmd);

	if (g_iom3_state == IOM3_ST_RECOVERY || iom3_power_state == ST_SLEEP)
		ret = write_customize_cmd(&pkg_ap, NULL, false);
	else
		ret = write_customize_cmd(&pkg_ap, &pkg_mcu, true);

	if (ret) {
		hwlog_err("send tag %d cfg data to mcu fail,ret=%d\n",
			pkg_ap.tag, ret);
	} else {
		if (pkg_mcu.errno != 0)
			hwlog_err("send sar param to mcu fail\n");
		else
			hwlog_info("send sar param to mcu succes\n");
	}
	return ret;
}

static int change_sar_mode(int tag, int argv[], int argc)
{
	int ret;
	write_info_t pkg_ap;
	read_info_t pkg_mcu;
	pkt_parameter_req_t spkt;
	pkt_header_t *shd = (pkt_header_t *)&spkt;

	memset(&pkg_ap, 0, sizeof(pkg_ap));
	memset(&pkg_mcu, 0, sizeof(pkg_mcu));
	memset(&spkt, 0, sizeof(spkt));

	if (!strncmp(sensor_chip_info[CAP_PROX], "huawei,semtech-sx9323",
		strlen("huawei,semtech-sx9323"))) {
		if (argc != 1 || (argv[0] != SAR_DEBUG_MODE &&
			argv[0] != SAR_NORMAL_MODE)) {
			hwlog_err("%s: Input incorrect mode.\n", __func__);
			return -1;
		}
	} else if (!strncmp(sensor_chip_info[CAP_PROX], "huawei,abov-a96t3x6",
		strlen("huawei,abov-a96t3x6"))) {
		if ((argc != 1) || ((argv[0] != SAR_DEBUG_MODE) &&
			(argv[0] != SAR_NORMAL_MODE))) {
			hwlog_err("%s: Input incorrect mode\n", __func__);
			return -1;
		}
	} else {
		hwlog_err("%s:sar not support the operation.\n", __func__);
		return -1;
	}

	spkt.subcmd = SUB_CMD_SET_DATA_MODE;
	pkg_ap.tag = TAG_CAP_PROX;
	pkg_ap.cmd = CMD_CMN_CONFIG_REQ;
	pkg_ap.wr_buf = &shd[1];
	pkg_ap.wr_len = sizeof(argv[0]) + SUBCMD_LEN;
	memcpy(spkt.para, &argv[0], sizeof(argv[0]));

	hwlog_info("%s g_iom3_state = %d,tag =%d ,cmd =%d\n", __func__,
		g_iom3_state, pkg_ap.tag, pkg_ap.cmd);

	if (g_iom3_state == IOM3_ST_RECOVERY || iom3_power_state == ST_SLEEP)
		ret = write_customize_cmd(&pkg_ap, NULL, false);
	else
		ret = write_customize_cmd(&pkg_ap, &pkg_mcu, true);

	if (ret) {
		hwlog_err("send tag %d sar mode to mcu fail,ret=%d\n",
			pkg_ap.tag, ret);
	} else {
		if (pkg_mcu.errno != 0)
			hwlog_err("send sar mode to mcu fail\n");
		else
			hwlog_info("send sar mode to mcu succes\n");
	}
	return ret;
}

static int rpc_test(int tag, int argv[], int argc)
{
	int ret;
	unsigned char cmd = argv[0];
	unsigned int stat = argv[1];
	write_info_t pkg_ap;
	read_info_t pkg_mcu;
	rpc_test_ioctl_t pkg_ioctl;

	memset(&pkg_ap, 0, sizeof(pkg_ap));
	memset(&pkg_mcu, 0, sizeof(pkg_mcu));
	pkg_ap.tag = TAG_RPC;

	if (cmd == SUB_CMD_RPC_UPDATE_REQ) {
		pkg_ap.cmd = CMD_CMN_CONFIG_REQ;
		pkg_ioctl.sub_cmd = cmd;
		pkg_ioctl.test_sarinfo = stat;
		pkg_ap.wr_buf = &pkg_ioctl;
		pkg_ap.wr_len = sizeof(pkg_ioctl);
		hwlog_info("command in %s, cmd=%d\n", __func__, cmd);
	} else if (cmd == SUB_CMD_RPC_LIBHAND_REQ) {
		pkg_ap.cmd = CMD_CMN_CONFIG_REQ;
		pkg_ioctl.sub_cmd = cmd;
		pkg_ioctl.test_sarinfo = stat;
		pkg_ap.wr_buf = &pkg_ioctl;
		pkg_ap.wr_len = sizeof(pkg_ioctl);
		hwlog_info("command in %s, cmd=%d\n", __func__, cmd);
	} else {
		hwlog_err("error command in %s, cmd=%d\n", __func__, cmd);
		return -1;
	}
	ret = write_customize_cmd(&pkg_ap, &pkg_mcu, true);
	if (ret) {
		hwlog_err("send rpc cmd to mcu fail,ret=%d\n", ret);
		return ret;
	}
	if (pkg_mcu.errno != 0)
		hwlog_err("set rpc cmd fail cmd=%d\n", cmd);
	else
		hwlog_info("set rpc cmd success cmd=%d\n", cmd);
	hwlog_info("tag=%d argc=%d\n", pkg_ap.tag, argc);

	return ret;
}
static int register_sensorhub_debug_operation(const char *func_name,
	sensor_debug_pfunc op)
{
	struct sensor_debug_cmd *node = NULL;
	struct sensor_debug_cmd *n = NULL;
	int ret = 0;

	if (!func_name || !op) {
		hwlog_err("error in %s\n", __func__);
		return -EINVAL;
	}

	mutex_lock(&sensor_debug_operations_list.mlock);
	list_for_each_entry_safe(node, n, &sensor_debug_operations_list.head,
		entry) {
		if (op == node->operation) {
			hwlog_warn("%s already registed in %s\n", func_name,
				__func__);
			goto out; /* return when already registed */
		}
	}
	node = kzalloc(sizeof(*node), GFP_ATOMIC);
	if (!node) {
		ret = -ENOMEM;
		goto out;
	}
	node->str = func_name;
	node->operation = op;
	list_add_tail(&node->entry, &sensor_debug_operations_list.head);
out:
	mutex_unlock(&sensor_debug_operations_list.mlock);
	return ret;
}

static int unregister_sensorhub_debug_operation(sensor_debug_pfunc op)
{
	struct sensor_debug_cmd *pos = NULL;
	struct sensor_debug_cmd *n = NULL;

	if (!op) {
		hwlog_err("error in %s\n", __func__);
		return -EINVAL;
	}

	mutex_lock(&sensor_debug_operations_list.mlock);
	list_for_each_entry_safe(pos, n, &sensor_debug_operations_list.head,
		entry) {
		if (op == pos->operation) {
			list_del(&pos->entry);
			kfree(pos);
			break;
		}
	}
	mutex_unlock(&sensor_debug_operations_list.mlock);
	return 0;
}

static void register_my_debug_operations(void)
{
	register_sensorhub_debug_op(open_sensor);
	register_sensorhub_debug_op(set_delay);
	register_sensorhub_debug_op(close_sensor);
	register_sensorhub_debug_op(set_sensor_slave_addr);
	register_sensorhub_debug_op(set_sensor_softiron);
	register_sensorhub_debug_op(big_data_test);
	register_sensorhub_debug_op(set_sensor_data_mode);
	register_sensorhub_debug_op(set_fault_type);
	register_sensorhub_debug_op(set_fault_addr);
	register_sensorhub_debug_op(set_log_level);
	register_sensorhub_debug_op(ar_test);
	register_sensorhub_debug_op(aod_test);
	register_sensorhub_debug_op(environment_test);
	register_sensorhub_debug_op(rpc_test);
	register_sensorhub_debug_op(set_als_type);
	register_sensorhub_debug_op(als_param_write);
	register_sensorhub_debug_op(set_ps_type);
	register_sensorhub_debug_op(ps_param_write);
	register_sensorhub_debug_op(sar_param_write);
	register_sensorhub_debug_op(change_sar_mode);
	register_sensorhub_debug_op(sar_param_set);
}

static void unregister_my_debug_operations(void)
{
	unregister_sensorhub_debug_operation(open_sensor);
	unregister_sensorhub_debug_operation(set_delay);
	unregister_sensorhub_debug_operation(close_sensor);
	unregister_sensorhub_debug_operation(set_sensor_slave_addr);
	unregister_sensorhub_debug_operation(set_sensor_softiron);
	unregister_sensorhub_debug_operation(big_data_test);
	unregister_sensorhub_debug_operation(set_sensor_data_mode);
	unregister_sensorhub_debug_operation(set_fault_type);
	unregister_sensorhub_debug_operation(set_fault_addr);
	unregister_sensorhub_debug_operation(set_log_level);
	unregister_sensorhub_debug_operation(ar_test);
	unregister_sensorhub_debug_operation(aod_test);
	unregister_sensorhub_debug_operation(environment_test);
	unregister_sensorhub_debug_operation(rpc_test);
	unregister_sensorhub_debug_operation(set_als_type);
	unregister_sensorhub_debug_operation(als_param_write);
	unregister_sensorhub_debug_operation(set_ps_type);
	unregister_sensorhub_debug_operation(ps_param_write);
	unregister_sensorhub_debug_operation(sar_param_write);
	unregister_sensorhub_debug_operation(change_sar_mode);
	unregister_sensorhub_debug_operation(sar_param_set);
}

static inline bool is_space_ch(char ch)
{
	return (ch == ' ') || (ch == '\t');
}

static inline bool end_of_string(char ch)
{
	bool ret = false;

	switch (ch) {
	case '\0':
	case '\r':
	case '\n':
		ret = true;
		break;

	default:
		ret = false;
		break;
	}

	return ret;
}

/* find first pos */
const char *get_str_begin(const char *cmd_buf)
{
	if (!cmd_buf)
		return NULL;

	while (is_space_ch(*cmd_buf))
		++cmd_buf;

	if (end_of_string(*cmd_buf))
		return NULL;

	return cmd_buf;
}

/* find last pos */
const char *get_str_end(const char *cmd_buf)
{
	if (!cmd_buf)
		return NULL;

	while (!is_space_ch(*cmd_buf) && !end_of_string(*cmd_buf))
		++cmd_buf;

	return cmd_buf;
}

/* fuzzy matching */
static bool str_fuzzy_match(const char *cmd_buf, const char *target)
{
	if (!cmd_buf || !target)
		return false;

	for (; !is_space_ch(*cmd_buf) && !end_of_string(*cmd_buf) && *target;
		++target) {
		if (*cmd_buf == *target)
			++cmd_buf;
	}

	return is_space_ch(*cmd_buf) || end_of_string(*cmd_buf);
}

static sensor_debug_pfunc get_operation(const char *str)
{
	sensor_debug_pfunc op = NULL;
	struct sensor_debug_cmd *node = NULL;
	struct sensor_debug_cmd *n = NULL;

	mutex_lock(&sensor_debug_operations_list.mlock);
	list_for_each_entry_safe(node, n, &sensor_debug_operations_list.head,
		entry) {
		if (str_fuzzy_match(str, node->str)) {
			op = node->operation;
			break;
		}
	}
	mutex_unlock(&sensor_debug_operations_list.mlock);
	return op;
}

static int get_sensor_tag(const char *str)
{
	int i;

	for (i = 0; i < sizeof(tag_map_tab) / sizeof(tag_map_tab[0]); ++i) {
		if (str_fuzzy_match(str, tag_map_tab[i].str))
			return tag_map_tab[i].tag;
	}
	return -1;
}

bool get_arg(const char *str, int *arg)
{
	int val = 0;
	bool neg = false;
	bool hex = false;

	if (*str == '-') {
		++str;
		neg = true;
	}

	if ((*str == '0') && ((*(str + 1) == 'x') || (*(str + 1) == 'X'))) {
		str += 2;
		hex = true;
	}

	if (hex) {
		for (; !is_space_ch(*str) && !end_of_string(*str); ++str) {
			if (!ch_is_hexdigit(*str))
				return false;
			val <<= 4;
			val |= (ch_is_digit(*str) ? (*str - '0') :
				(((*str | 0x20) - 'a') + 10));
		}
	} else {
		for (; !is_space_ch(*str) && !end_of_string(*str); ++str) {
			if (!ch_is_digit(*str))
				return false;
			val *= 10;
			val += *str - '0';
		}
	}

	*arg = neg ? -val : val;
	return true;
}

static void parse_str(const char *cmd_buf)
{
	sensor_debug_pfunc operation = NULL;
	int tag = -1;
	int arg = -1;
	int argv[MAX_CMD_BUF_ARGC] = { 0 };
	int argc = 0;

	for (; (cmd_buf = get_str_begin(cmd_buf)) != NULL;
		cmd_buf = get_str_end(cmd_buf)) {
		if (!operation)
			operation = get_operation(cmd_buf);

		if (tag == -1)
			tag = get_sensor_tag(cmd_buf);

		if (get_arg(cmd_buf, &arg)) {
			if (argc < MAX_CMD_BUF_ARGC)
				argv[argc++] = arg;
			else
				hwlog_err("%d will be ignored\n", arg);
		}
	}

	if (operation)
		operation(tag, argv, argc);
}

static ssize_t cls_attr_debug_show_func(struct class *cls,
	struct class_attribute *attr, char *buf)
{
	int i;
	unsigned int offset = 0;
	struct sensor_debug_cmd *node = NULL;
	struct sensor_debug_cmd *n = NULL;

	offset += sprintf(buf + offset,
		"operations format:\necho operation+tag+optarg > %s\n",
		attr->attr.name);
	offset += sprintf(buf + offset,
		"for example:\nto open accel we input: echo open_sensor accel > %s\n",
		attr->attr.name);
	offset += sprintf(buf + offset,
		"to setdelay accel 100 ms we input: echo set_delay accel 100 > %s\n",
		attr->attr.name);

	offset += sprintf(buf + offset, "\noperations supported as follow:\n");
	mutex_lock(&sensor_debug_operations_list.mlock);
	list_for_each_entry_safe(node, n, &sensor_debug_operations_list.head,
		entry) {
		offset += sprintf(buf + offset, "%s\n", node->str);
	}
	mutex_unlock(&sensor_debug_operations_list.mlock);

	offset += sprintf(buf + offset, "\ntags supported as follow:\n");
	for (i = 0; i < sizeof(tag_map_tab) / sizeof(tag_map_tab[0]); ++i)
		offset += sprintf(buf + offset, "%s\n", tag_map_tab[i].str);

	return offset;
}

static ssize_t cls_attr_debug_store_func(struct class *cls,
	struct class_attribute *attr, const char *buf, size_t size)
{
	parse_str(buf);
	return size;
}

static struct class_attribute class_attr_sensorhub_dbg = __ATTR(sensorhub_dbg,
	0660, cls_attr_debug_show_func, cls_attr_debug_store_func);

static ssize_t cls_attr_dump_show_func(struct class *cls,
	struct class_attribute *attr, char *buf)
{
	hwlog_info("read sensorhub_dump node, IOM7 will restart\n");
	iom3_need_recovery(SENSORHUB_USER_MODID, SH_FAULT_USER_DUMP);
	return snprintf(buf, MAX_STR_SIZE,
		"read sensorhub_dump node, IOM7 will restart\n");
}

static struct class_attribute class_attr_sensorhub_dump =
	__ATTR(sensorhub_dump, 0660, cls_attr_dump_show_func, NULL);

static ssize_t cls_attr_kernel_support_lib_ver_show_func(struct class *cls,
	struct class_attribute *attr, char *buf)
{
	uint32_t ver = 15; /* 15:for support large resolution acc sensor */

	hwlog_info("read %s %d\n", __func__, ver);
	memcpy(buf, &ver, sizeof(ver));
	return sizeof(ver);
}

static struct class_attribute class_attr_libsensor_ver = __ATTR(libsensor_ver,
	0660, cls_attr_kernel_support_lib_ver_show_func, NULL);

void create_debug_files(void)
{
	if (class_create_file(sensors_class, &class_attr_sensorhub_dbg))
		hwlog_err("create files failed in %s\n", __func__);

	if (class_create_file(sensors_class, &class_attr_sensorhub_dump))
		hwlog_err("create files sensorhub_dump in %s\n", __func__);

	if (class_create_file(sensors_class, &class_attr_libsensor_ver))
		hwlog_err("create files libsensor_ver in %s\n", __func__);
}

static const char *get_iomcu_power_status(void)
{
	int status;

	memset(show_str, 0, MAX_STR_SIZE);

	mutex_lock(&mutex_pstatus);
	status = i_power_status.power_status;
	mutex_unlock(&mutex_pstatus);

	switch (status) {
	case SUB_POWER_ON:
		snprintf(show_str, MAX_STR_SIZE, "%s", "SUB_POWER_ON");
		break;
	case SUB_POWER_OFF:
		snprintf(show_str, MAX_STR_SIZE, "%s", "SUB_POWER_OFF");
		break;
	default:
		snprintf(show_str, MAX_STR_SIZE, "%s", "unknown status");
		break;
	}
	return show_str;
}

static const char *get_iomcu_current_opened_app(void)
{
	int i;
	char buf[SINGLE_STR_LENGTH_MAX] = {0};
	int index = 0;
	int copy_length;

	memset(show_str, 0, MAX_STR_SIZE);

	mutex_lock(&mutex_pstatus);
	for (i = 0; i < TAG_END; i++) {
		memset(buf, 0, SINGLE_STR_LENGTH_MAX);
		if (i_power_status.app_status[i]) {
			if (obj_tag_str[i]) {
				copy_length = (strlen(obj_tag_str[i]) >
					(SINGLE_STR_LENGTH_MAX - 1)) ?
					(SINGLE_STR_LENGTH_MAX - 1) :
					strlen(obj_tag_str[i]);
				strncpy(buf, obj_tag_str[i], copy_length);
			} else {
				copy_length = 2;
				snprintf(buf, 3, "%3d", i);
			}
			buf[copy_length] = '\n';
			index += (copy_length + 1);
			if (index < MAX_STR_SIZE) {
				strcat(show_str, buf);
			} else {
				show_str[MAX_STR_SIZE - 1] = 'X';
				hwlog_err("show_str too long\n");
				break;
			}
		}
	}
	mutex_unlock(&mutex_pstatus);
	return show_str;
}

static int get_iomcu_idle_time(void)
{
	return i_power_status.idle_time;
}

static const char *get_iomcu_active_app_during_suspend(void)
{
	int i;
	char buf[SINGLE_STR_LENGTH_MAX] = {0};
	int index;
	int get_iomcu;
	uint64_t bit_map;
	int copy_length;

	memset(show_str, 0, MAX_STR_SIZE);

	mutex_lock(&mutex_pstatus);
	bit_map = i_power_status.active_app_during_suspend;
	mutex_unlock(&mutex_pstatus);

	for (i = 0; i < TAG_HW_PRIVATE_APP_END; i++) {
		memset(buf, 0, SINGLE_STR_LENGTH_MAX);
		get_iomcu = (bit_map >> i) & 0x01;
		if (get_iomcu) {
			if (iomcu_app_id_str[i]) {
				copy_length = (strlen(iomcu_app_id_str[i]) >
					(SINGLE_STR_LENGTH_MAX - 1)) ?
					(SINGLE_STR_LENGTH_MAX - 1) :
					strlen(iomcu_app_id_str[i]);
				strncpy(buf, iomcu_app_id_str[i], copy_length);
			} else {
				copy_length = 2;
				snprintf(buf, 3, "%3d", i);
			}
			buf[copy_length] = '\n';
			index += (copy_length + 1);
			if (index < MAX_STR_SIZE) {
				strcat(show_str, buf);
			} else {
				show_str[MAX_STR_SIZE - 1] = 'X';
				hwlog_err("show_str too long\n");
				break;
			}
		}
	}
	return show_str;
}

static int mcu_power_log_process(const pkt_header_t *head)
{
	hwlog_info("%s in\n", __func__);

	mutex_lock(&mutex_pstatus);
	i_power_status.idle_time =
		((pkt_power_log_report_req_t *)head)->idle_time;
	i_power_status.active_app_during_suspend =
		((pkt_power_log_report_req_t *)head)->current_app_mask;
	mutex_unlock(&mutex_pstatus);

	hwlog_info("last iomcu idle time:%d,active apps high:0x%x,low:0x%x\n",
		i_power_status.idle_time,
		(uint32_t)((i_power_status.active_app_during_suspend >> 32) & 0xffffffff),
		(uint32_t)(i_power_status.active_app_during_suspend & 0xffffffff));
	return 0;
}

static ssize_t show_power_status(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	return snprintf(buf, MAX_STR_SIZE, "%s\n", get_iomcu_power_status());
}

static ssize_t show_app_status(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	return snprintf(buf, MAX_STR_SIZE, "%s\n",
		get_iomcu_current_opened_app());
}
static ssize_t show_idle_time(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	return snprintf(buf, MAX_STR_SIZE, "%d\n", get_iomcu_idle_time());
}

static ssize_t show_active_app_during_suspend(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	return snprintf(buf, MAX_STR_SIZE, "%s\n",
		get_iomcu_active_app_during_suspend());
}

static DEVICE_ATTR(power_status, 0440, show_power_status, NULL);
static DEVICE_ATTR(current_app, 0440, show_app_status, NULL);
static DEVICE_ATTR(idle_time, 0440, show_idle_time, NULL);
static DEVICE_ATTR(active_app_during_suspend, 0440,
	show_active_app_during_suspend, NULL);

static struct attribute *power_info_attrs[] = {
	&dev_attr_power_status.attr,
	&dev_attr_current_app.attr,
	&dev_attr_idle_time.attr,
	&dev_attr_active_app_during_suspend.attr,
	NULL,
};

static const struct attribute_group power_info_attrs_grp = {
	.attrs = power_info_attrs,
};

static struct power_dbg power_info = {
	.name = "power_info",
	.attrs_group = &power_info_attrs_grp,
};

static int iomcu_power_info_init(void)
{
	memset(&i_power_status, 0, sizeof(iomcu_power_status));

	register_mcu_event_notifier(TAG_LOG, CMD_LOG_POWER_REQ,
		mcu_power_log_process);

	iomcu_power = class_create(THIS_MODULE, "iomcu_power");
	if (IS_ERR(iomcu_power)) {
		hwlog_err(" %s class creat fail\n", __func__);
		return -1;
	}

	power_info.dev = device_create(iomcu_power, NULL, 0, &power_info,
		power_info.name);
	if (!power_info.dev) {
		hwlog_err(" %s creat dev fail\n", __func__);
		class_destroy(iomcu_power);
		return -1;
	}

	if (power_info.attrs_group) {
		if (sysfs_create_group(&power_info.dev->kobj,
			power_info.attrs_group)) {
			hwlog_err("create files failed in %s\n", __func__);
		} else {
			hwlog_info("%s ok\n", __func__);
			return 0;
		}
	} else {
		hwlog_err("power_info.attrs_group is null\n");
	}

	device_destroy(iomcu_power, 0);
	class_destroy(iomcu_power);
	return -1;
}

static void iomcu_power_info_exit(void)
{
	device_destroy(iomcu_power, 0);
	class_destroy(iomcu_power);
}

static int sensorhub_debug_init(void)
{
	if (is_sensorhub_disabled())
		return -1;
	register_my_debug_operations();
	iomcu_power_info_init();
	inputhub_ext_log_init();

	return 0;
}

static void sensorhub_debug_exit(void)
{
	unregister_my_debug_operations();
	iomcu_power_info_exit();
}

late_initcall_sync(sensorhub_debug_init);
module_exit(sensorhub_debug_exit);

MODULE_AUTHOR("SensorHub <smartphone@huawei.com>");
MODULE_DESCRIPTION("SensorHub debug driver");
MODULE_LICENSE("GPL");
