/*
 * zrhung_event.c
 *
 * Interfaces implementation for sending hung event from kernel
 *
 * Copyright (c) 2017-2019 Huawei Technologies Co., Ltd.
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

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <log/hiview_hievent.h>
#include <asm/current.h>
#include "huawei_platform/log/hw_log.h"
#include "chipset_common/hwzrhung/zrhung.h"
#include "zrhung_common.h"
#include "zrhung_transtation.h"

#define HWLOG_TAG zrhung

HWLOG_REGIST();

int zrhung_send_hievent(enum zrhung_wp_id id, const char *cmd_buf, const char *msg_buf)
{
	int ret;
	struct hiview_hievent* zrhung_event = NULL;
	zrhung_event = hiview_hievent_create(RAW_EVENT_ZRHUNG);
	if (zrhung_event == NULL)
		return -EINVAL;
	hiview_hievent_put_integral(zrhung_event, "WATCHPOINTID", id);
	hiview_hievent_put_integral(zrhung_event, "PID", current->pid);
	hiview_hievent_put_integral(zrhung_event, "TGID", current->tgid);
	if (cmd_buf != NULL)
		hiview_hievent_put_string(zrhung_event, "CMD", cmd_buf);
	if (msg_buf != NULL)
		hiview_hievent_put_string(zrhung_event, "MSG", msg_buf);
	ret = hiview_hievent_report(zrhung_event);
	if (ret < 0)
		hwlog_err("failed to send hievent");
	hiview_hievent_destroy(zrhung_event);
	return ret;
}

int zrhung_send_event(enum zrhung_wp_id id, const char *cmd_buf, const char *msg_buf)
{
	int ret;
	int cmd_len = 0;
	int msg_len = 0;
	if (zrhung_is_id_valid(id) < 0) {
		hwlog_err("Bad watchpoint id");
		return -EINVAL;
	}

	if (cmd_buf)
		cmd_len = strlen(cmd_buf);
	if (cmd_len > ZRHUNG_CMD_LEN_MAX) {
		hwlog_err("watchpoint cmd too long");
		return -EINVAL;
	}

	if (msg_buf)
		msg_len = strlen(msg_buf);
	if (msg_len > ZRHUNG_MSG_LEN_MAX) {
		hwlog_err("watchpoint msg buffer too long");
		return -EINVAL;
	}

	ret = zrhung_send_hievent(id, cmd_buf, msg_buf);
	hwlog_info("zrhung send event from kernel: wp=%d, ret=%d", id, ret);
	return ret;
}
EXPORT_SYMBOL(zrhung_send_event);
