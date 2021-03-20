/*
 * zrhung_wp_io.c
 *
 * Report frozen screen alarm events when IO is overloaded
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

#include "chipset_common/hwzrhung/zrhung.h"

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/backing-dev.h>
#include <linux/blkdev.h>
#include <linux/blk-mq.h>
#include <linux/highmem.h>
#include <linux/mm.h>
#include <linux/kernel_stat.h>
#include <linux/string.h>
#include <linux/init.h>
#include <linux/list_sort.h>
#include <linux/pm_runtime.h>
#include <linux/math64.h>
#include "securec.h"

#define CFG_SIZE 64
#define CMD_SIZE 64
#define MAX_CFG_NUM 4
#define MAX_EVENTS 16
#define MAX_WP_BUFSIZE 1024
#define LATENCY_ZRHUNG_LOG_DURATION 60
#define NANOS_PER_MILLISECOND 1000000
#define NANOS_PER_SECOND (NANOS_PER_MILLISECOND * 1000)

#define IOWP_WARN(format, args...)			\
	pr_warn("iowp: " format, ##args)
#define IOWP_INFO(format, args...)			\
	pr_info("iowp: " format, ##args)

enum {
	IOWP_CFG_NOT_READY = 0,
	IOWP_CFG_READY = 1,
	IOWP_CFG_INITIALIZED = 2,
};

struct iowp_event {
	pid_t cur_pid;
	pid_t tgid;
	char name[BDEVNAME_SIZE];
	u64 stamp; /* ms */
};

struct iowp_config {
	unsigned int is_ready;
	unsigned int enabled;
	unsigned int threshold;
	u64 period; /* ms */
	u64 silence;
};

struct iowp_main {
	struct iowp_config config;
	struct iowp_event events[MAX_EVENTS];
	unsigned int free;
	u64 last_report_stamp;
};

struct iowp_work {
	struct workqueue_struct *workq;
	struct work_struct config_work;
	struct work_struct send_work;
};

static char iowp_logbuf[MAX_WP_BUFSIZE];
static struct iowp_main io_wp;
static struct iowp_work io_wp_work;

static inline unsigned int iowp_config_is_ready(void)
{
	return io_wp.config.is_ready;
}

static inline unsigned int iowp_config_is_enabled(void)
{
	return io_wp.config.enabled;
}

static inline u64 iowp_config_get_period(void)
{
	return io_wp.config.period;
}

static int iowp_parse_config(const char *str, uint32_t len,
			     struct iowp_config *cfg_ptr)
{
	int ret = 0;

	if ((str == NULL) || (len == 0) || (cfg_ptr == NULL))
		return -1;

	if (sscanf(str, "%d,%llu,%u,%llu", &cfg_ptr->enabled, &cfg_ptr->period,
		   &cfg_ptr->threshold, &cfg_ptr->silence) == MAX_CFG_NUM)
		ret = 1;

	return ret;
}

static u8 iowp_config_load(void)
{
	char config_str[CFG_SIZE] = {0};
	int ret;
	struct iowp_config *config = &io_wp.config;

	ret = zrhung_get_config(ZRHUNG_WP_IO, config_str, sizeof(config_str));
	if (ret > 0) {
		IOWP_WARN("zrhung config not ready, wait\n");
		config->is_ready = IOWP_CFG_NOT_READY;
	} else if (ret < 0) {
		IOWP_WARN("zrhung iowp is not enabled\n");
		config->is_ready = IOWP_CFG_READY;
		config->enabled = false;
	} else {
		IOWP_INFO("config string(%s)\n", config_str);
		if (iowp_parse_config(config_str, CFG_SIZE, config) > 0)
			config->is_ready = IOWP_CFG_INITIALIZED;
		else
			config->is_ready = IOWP_CFG_READY;
	}

	IOWP_INFO("ready:%d enable:%d period:%llu threshold:%u silence:%llu\n",
		  config->is_ready, config->enabled, config->period,
		  config->threshold, config->silence);

	return config->is_ready;
}

static void iowp_event_ctor(struct iowp_event *this, pid_t pid, pid_t tgid,
			    const char *device_name)
{
#ifdef CONFIG_HISI_TIME
	this->stamp = hisi_getcurtime() / NANOS_PER_MILLISECOND;
#else
	this->stamp = local_clock() / NANOS_PER_MILLISECOND;
#endif
	/* copy IO latency timeout info */
	this->cur_pid = pid;
	this->tgid = tgid;
	if (memcpy_s(this->name, BDEVNAME_SIZE, device_name, strlen(device_name) + 1) != EOK)
		IOWP_WARN("memcpy from device_name to this->name failed\n");

	IOWP_INFO("cur_pid:[%d], tgid[%d], device_name[%s]\n", this->cur_pid,
		  this->tgid, this->name);
}

static int iowp_event_format(struct iowp_event *this, char *buf, size_t len)
{
	return scnprintf(buf, len,
			 "PID:[%d], TGID[%d], Device_Name[%s] reach the warning throttle\n",
			 this->cur_pid, this->tgid, this->name);
}

static size_t iowp_format_events(unsigned int pos)
{
	unsigned int s = pos;
	size_t total_len = sizeof(iowp_logbuf);
	size_t written_len = 0;
	size_t w_len;

	for (; s != io_wp.free; s = (s + 1) % MAX_EVENTS) {
		w_len = iowp_event_format(&io_wp.events[s],
					  iowp_logbuf + written_len,
					  total_len - written_len);
		written_len += w_len;
	}
	IOWP_INFO("written_len: %zu\n", written_len);
	return written_len;
}

static void iowp_format_cmd(char *cmd, unsigned int len)
{
	if (cmd != NULL) {
#ifdef CONFIG_HISI_TIME
		u64 cur_stamp = hisi_getcurtime() / NANOS_PER_SECOND;
#else
		u64 cur_stamp = local_clock() / NANOS_PER_SECOND;
#endif
		int ret = snprintf_s(cmd, len, len - 1, "d=%d,e=%llu", LATENCY_ZRHUNG_LOG_DURATION, cur_stamp);
		if (ret == -1)
			IOWP_WARN("output cmd snprintf_s error\n");

		IOWP_INFO("cur_stamp: %llu\n", cur_stamp);
	}
}

static bool iowp_check_threshold(unsigned int *pos)
{
	struct iowp_config *config = &io_wp.config;
	struct iowp_event *events = io_wp.events;
	unsigned int last = io_wp.free;
	unsigned int new;
	unsigned int distance;
	bool need_report = false;

	distance = (last >= config->threshold) ? (last - config->threshold) : (MAX_EVENTS + last - config->threshold);
	if (events[distance].stamp == 0)
		return need_report;

	new = io_wp.free == 0 ? (MAX_EVENTS - 1) : (io_wp.free - 1);
	if ((events[new].stamp > iowp_config_get_period() + events[distance].stamp)) {
		IOWP_INFO("need_report: %d\n", need_report);
		return need_report;
	}

	need_report = true;
	if (pos)
		*pos = distance;

	return need_report;
}

static struct iowp_event *iowp_get_slot(void)
{
	struct iowp_event *ptr = io_wp.events + io_wp.free;

	io_wp.free++;
	io_wp.free = io_wp.free % MAX_EVENTS;
	IOWP_INFO("free: %d\n", io_wp.free);
	return ptr;
}

void iowp_event_config_work(struct work_struct *work)
{
	IOWP_WARN("%s: send event: %d\n", __func__, ZRHUNG_WP_IO);
	if (iowp_config_load() == IOWP_CFG_INITIALIZED)
		IOWP_INFO("Config Load successfully\n");
}

/* Send envent through zrhung_send_event */
void iowp_event_send_work(struct work_struct *work)
{
	char zrhung_cmd[CMD_SIZE] = {0};

	IOWP_INFO("%s: send event: %d\n", __func__, ZRHUNG_WP_IO);
	iowp_format_cmd(zrhung_cmd, sizeof(zrhung_cmd));
	IOWP_INFO("%s: cmd_buf: %s\n", __func__, zrhung_cmd);
	zrhung_send_event(ZRHUNG_WP_IO, zrhung_cmd, iowp_logbuf);
	IOWP_INFO("%s: send event buffer: %s\n", __func__, iowp_logbuf);
}

/* io watchponit init ZeroHung config in blk-core */
void iowp_init(void)
{
	IOWP_INFO("%s: %d\n", __func__, iowp_config_is_ready());
	if (iowp_config_is_ready() > IOWP_CFG_NOT_READY)
		return;

	memset(&io_wp, 0, sizeof(io_wp));
	queue_work(io_wp_work.workq, &io_wp_work.config_work);
}

/* io watchponit init ZeroHung workqueque in blk-core */
void iowp_workqueue_init(void)
{
	io_wp_work.workq = create_workqueue("io_latency_workq");
	if (io_wp_work.workq == NULL) {
		IOWP_WARN("%s: create workq failed\n", __func__);
		return;
	}
	INIT_WORK(&io_wp_work.config_work, iowp_event_config_work);
	INIT_WORK(&io_wp_work.send_work, iowp_event_send_work);
}

/* io watchponit report IO latency timeout to ZeroHung */
void iowp_report(pid_t pid, pid_t tgid, const char *name)
{
	unsigned int pos = 0;
	struct iowp_event *cur_event = NULL;
	size_t buf_len;

	/* initialize config */
	iowp_init();

	/* check if foreground process */
	if (pid == get_fg_pid())
		return;

	/* check if iowp config is ready */
	if (iowp_config_is_ready() == IOWP_CFG_NOT_READY) {
		IOWP_WARN("iowp config is not ready ,exit\n");
		return;
	}

	/* check if iowp is enabled */
	if (!iowp_config_is_enabled()) {
		IOWP_WARN("iowp disabled ,exit\n");
		return;
	}

	/* get free slot event */
	cur_event = iowp_get_slot();
	iowp_event_ctor(cur_event, pid, tgid, name);
	if (cur_event->stamp < io_wp.last_report_stamp + io_wp.config.silence) {
		IOWP_WARN
		    ("curr(%llu) less then last report(%llu) + silence(%llu)\n",
		     cur_event->stamp,
		     io_wp.last_report_stamp,
		     io_wp.config.silence);
		return;
	}

	/* check if need to report events */
	if (true == iowp_check_threshold(&pos)) {
		buf_len = iowp_format_events(pos);

		io_wp.last_report_stamp = cur_event->stamp;
		queue_work(io_wp_work.workq, &io_wp_work.send_work);
		IOWP_WARN("buffer length (%zu)\n", buf_len);
	}
}
