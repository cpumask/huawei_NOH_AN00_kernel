/*
 * power_log.h
 *
 * log for power module
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

#ifndef _POWER_LOG_H_
#define _POWER_LOG_H_

#define POWER_LOG_INVAID_OP                 (-22)
#define POWER_LOG_MAX_SIZE                  4096
#define POWER_LOG_RESERVED_SIZE             16

enum power_log_device_id {
	POWER_LOG_DEVICE_ID_BEGIN = 0,
	POWER_LOG_DEVICE_ID_SERIES_BATT = POWER_LOG_DEVICE_ID_BEGIN,
	POWER_LOG_DEVICE_ID_MULTI_BTB, /* for multi btb temp */
	POWER_LOG_DEVICE_ID_BD99954, /* for bd99954 */
	POWER_LOG_DEVICE_ID_BQ2419X, /* for bq2419x */
	POWER_LOG_DEVICE_ID_BQ2429X, /* for bq2429x */
	POWER_LOG_DEVICE_ID_BQ2560X, /* for bq2560x */
	POWER_LOG_DEVICE_ID_BQ25882, /* for bq25882 */
	POWER_LOG_DEVICE_ID_BQ25892, /* for bq25892 */
	POWER_LOG_DEVICE_ID_BQ25970, /* for bq25970 */
	POWER_LOG_DEVICE_ID_HL7019, /* for hl7019 */
	POWER_LOG_DEVICE_ID_RT9466, /* for rt9466 */
	POWER_LOG_DEVICE_ID_RT9471, /* for rt9471 */
	POWER_LOG_DEVICE_ID_RT9759, /* for rt9759 */
	POWER_LOG_DEVICE_ID_HI6522, /* for hi6522 */
	POWER_LOG_DEVICE_ID_HI6523, /* for hi6523 */
	POWER_LOG_DEVICE_ID_HI6526, /* for hi6526 */
	POWER_LOG_DEVICE_ID_END,
};

enum power_log_type {
	POWER_LOG_TYPE_BEGIN = 0,
	POWER_LOG_DUMP_LOG_HEAD = POWER_LOG_TYPE_BEGIN,
	POWER_LOG_DUMP_LOG_CONTENT,
	POWER_LOG_TYPE_END,
};

struct power_log_ops {
	const char *dev_name;
	void *dev_data;
	int (*dump_log_head)(char *, int, void *);
	int (*dump_log_content)(char *, int, void *);
};

struct power_log_device_info {
	struct mutex log_lock;
	char log_buf[POWER_LOG_MAX_SIZE];
	unsigned int total_ops;
	struct power_log_ops *ops[POWER_LOG_DEVICE_ID_END];
};

int power_log_common_operate(int type, char *buf, int size);
int power_log_ops_register(struct power_log_ops *ops);

#endif /* _POWER_LOG_H_ */
