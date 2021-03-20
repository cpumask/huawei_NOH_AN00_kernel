/*
 * ext_sensorhub_route.c
 *
 * code for external sensorhub route
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

#include "ext_sensorhub_route.h"

#include <linux/wait.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <huawei_platform/log/hw_log.h>
#include "securec.h"
#include "ext_sensorhub_frame.h"

#define HWLOG_TAG ext_sensorhub_route
HWLOG_REGIST();

#define ALL_COMMAND_ID  0xFF
#define WAIT_RESP_TIMEOUT_MS 200
#define WAIT_RESP_TIMEOUT_ERROR_CODE (-2)

static struct ext_sensorhub_route ext_route_tbl[] = {
	{	ROUTE_SENSOR_PORT, NULL,
		NULL, NULL, NULL,
		MAX_READ_LIST_LEN_SENSOR, 0
	},
	{   ROUTE_EXT_SENSORHUB_PORT, NULL,
		NULL, NULL, NULL,
		MAX_READ_LIST_LEN_EXT_SENSORHUB, 0
	},
	{   ROUTE_FLP_PORT, NULL,
		NULL, NULL, NULL,
		MAX_READ_LIST_LEN_FLP, 0
	},
	{   ROUTE_BT_PORT, NULL,
		NULL, NULL, NULL,
		MAX_READ_LIST_LEN_BT, 0
	},
	{   ROUTE_VIBRATOR_PORT, NULL,
		NULL, NULL, NULL,
		MAX_READ_LIST_LEN_VIBRATOR, 0
	},
	{   ROUTE_UPGRADE_PORT, NULL,
		NULL, NULL, NULL,
		MAX_READ_LIST_LEN_UPGRADE, 0
	},
	{   ROUTE_CHARGE_PORT, NULL,
		NULL, NULL, NULL,
		MAX_READ_LIST_LEN_CHARGE, 0
	},
	{   ROUTE_DEBUG_PORT, NULL,
		NULL, NULL, NULL,
		MAX_READ_LIST_LEN_DEBUG, 0
	},
	{   ROUTE_MNTN_PORT, NULL,
		NULL, NULL, NULL,
		MAX_READ_LIST_LEN_MNTN, 0
	},
	{   ROUTE_AUDIO_PORT, NULL,
		NULL, NULL, NULL,
		MAX_READ_LIST_LEN_AUDIO, 0
	},
	{   ROUTE_NFC_PORT, NULL,
		NULL, NULL, NULL,
		MAX_READ_LIST_LEN_NFC, 0
	},
	{   ROUTE_AT_PORT, NULL,
		NULL, NULL, NULL,
		MAX_READ_LIST_LEN_AT, 0
	},
	{   ROUTE_AR_PORT, NULL,
		NULL, NULL, NULL,
		MAX_READ_LIST_LEN_AR, 0
	},
	{   ROUTE_BT_A_PORT, NULL,
		NULL, NULL, NULL,
		MAX_READ_LIST_LEN_BT_A, 0
	},
	{   ROUTE_MOTION_PORT, NULL,
		NULL, NULL, NULL,
		MAX_READ_LIST_LEN_MOTION, 0
	},
};

static struct ext_sensorhub_buf_list *idle_buf_head;
static struct mutex buf_list_lock;

#define ROUTE_TBL_LEN \
(sizeof(ext_route_tbl) / sizeof(struct ext_sensorhub_route))

static int prepare_idle_buf(void)
{
	struct ext_sensorhub_buf_list *buf_node = NULL;
	int index = 0;

	mutex_lock(&buf_list_lock);
	idle_buf_head = kmalloc(sizeof(*idle_buf_head),
				GFP_KERNEL);
	if (!idle_buf_head) {
		mutex_unlock(&buf_list_lock);
		return -ENOMEM;
	}

	INIT_LIST_HEAD(&idle_buf_head->list);

	/* prepare MAX_IDLE_READ_LEN idle node, but do not malloc data buffer */
	for (index = 0; index < MAX_IDLE_READ_LEN; index++) {
		buf_node = kmalloc(sizeof(*buf_node),
				   GFP_KERNEL);
		if (!buf_node)
			return -ENOMEM;

		list_add_tail(&buf_node->list,
			      &idle_buf_head->list);
		buf_node->command_id = 0;
		buf_node->service_id = 0;
		buf_node->read_cnt = 0;
		buf_node->buffer = NULL;
	}
	mutex_unlock(&buf_list_lock);

	return 0;
}

/* release idle buffer and read buffer */
static int release_list_buf(void)
{
	int i;
	struct ext_sensorhub_buf_list *buf_node = NULL;
	struct ext_sensorhub_buf_list *tmp_node = NULL;

	/* communication already stopped, bufer list will not changes */
	mutex_lock(&buf_list_lock);
	list_for_each_entry_safe(buf_node, tmp_node,
				 &idle_buf_head->list, list) {
		/* data buffer in idle node already free */
		list_del(&buf_node->list);
		kfree(buf_node);
		buf_node = NULL;
	}
	kfree(idle_buf_head);
	idle_buf_head = NULL;
	mutex_unlock(&buf_list_lock);

	for (i = 0; i < ROUTE_TBL_LEN; ++i) {
		hwlog_err("port[%d], begin init.\n", i);
		mutex_lock(&ext_route_tbl[i].read_lock);
		list_for_each_entry_safe(
			buf_node, tmp_node,
			&ext_route_tbl[i].read_buf_head->list, list) {
			kfree(buf_node->buffer);
			buf_node->buffer = NULL;
			list_del(&buf_node->list);
			kfree(buf_node);
			buf_node = NULL;
		}
		kfree(ext_route_tbl[i].read_buf_head);
		ext_route_tbl[i].read_buf_head = NULL;
		mutex_unlock(&ext_route_tbl[i].read_lock);
	}
	return 0;
}

int ext_sensorhub_route_init(void)
{
	int status;
	int i;

	hwlog_info("begin init.\n", __func__);
	/* prepare idle_buf_head */
	status = prepare_idle_buf();
	if (status) {
		hwlog_err("prepare idle read buffer error: %d\n", status);
		return status;
	}
	/* for idle buffer */
	mutex_init(&buf_list_lock);
	/* prepare read buf head for routes */
	for (i = 0; i < ROUTE_TBL_LEN; ++i) {
		hwlog_info("port[%d], begin init.\n", i);
		mutex_init(&ext_route_tbl[i].write_lock);
		mutex_init(&ext_route_tbl[i].read_lock);
		mutex_init(&ext_route_tbl[i].read_count_lock);
		mutex_init(&ext_route_tbl[i].sub_cmds_lock);
		mutex_init(&ext_route_tbl[i].wait_lock);
		init_waitqueue_head(&ext_route_tbl[i].resp_wait);
		init_waitqueue_head(&ext_route_tbl[i].read_wait);

		if (!ext_route_tbl[i].read_buf_head) {
			ext_route_tbl[i].read_buf_head = kmalloc(
				sizeof(struct ext_sensorhub_buf_list),
				GFP_KERNEL);
			if (!ext_route_tbl[i].read_buf_head) {
				status = -ENOMEM;
				return status;
			}
			INIT_LIST_HEAD(&ext_route_tbl[i].read_buf_head->list);
		}
	}

	/* frame init */
	ext_sensorhub_frame_init();

	hwlog_info("end init.\n", __func__);
	return 0;
}

void ext_sensorhub_route_exit(void)
{
	/* frame exit */
	ext_sensorhub_frame_exit();

	/* release idle list and read list */
	release_list_buf();
}

/* match the subscribed cmds */
static bool match_route_cmd(struct subs_cmds *route_cmd,
			    struct ext_sensorhub_buf_list *resp_buf)
{
	bool ret = false;
	bool sm = false;
	bool cm = false;
	int i;

	if (!route_cmd || !resp_buf)
		return ret;

	/* service id is matched and (cid matched or subs cmd is all/255) */
	for (i = 0; i < route_cmd->cmd_cnt; ++i) {
		sm = route_cmd->service_ids[i] == resp_buf->service_id;
		cm = (route_cmd->command_ids[i] == ALL_COMMAND_ID) ||
		(route_cmd->command_ids[i] == resp_buf->command_id);
		if (sm && cm) {
			hwlog_debug("%s, matched cmd. sid: %d, cid: %d\n",
				    __func__, resp_buf->service_id,
				  resp_buf->command_id);
			ret = true;
			break;
		}
	}

	return ret;
}

static void free_buffer_node(struct ext_sensorhub_buf_list *buf_node)
{
	if (!buf_node)
		return;

	kfree(buf_node->buffer);
	buf_node->buffer = NULL;
	kfree(buf_node);
	buf_node = NULL;
}

static struct ext_sensorhub_buf_list *get_idle_buf_node(int idx)
{
	struct ext_sensorhub_buf_list *buf_node = NULL;
	/* if route already get max package in read list, cannot get idle */
	if (ext_route_tbl[idx].read_count >=
		ext_route_tbl[idx].max_read_list_len) {
		hwlog_err("route[%d] read buffer count exceed. Max: %d, current: %d\n",
			  idx, ext_route_tbl[idx].max_read_list_len,
			  ext_route_tbl[idx].read_count);
		return NULL;
	}

	/* whole buffer pool is empty */
	if (list_empty(&idle_buf_head->list)) {
		/* No Buf in idle list, sleep 2ms and retry */
		hwlog_err("%s buf list is full\n", __func__);
		return NULL;
	}
	buf_node = list_entry(idle_buf_head->list.prev,
			      struct ext_sensorhub_buf_list, list);

	return buf_node;
}

static bool ext_sensorhub_wait_resp(
	struct ext_sensorhub_buf_list *resp_buf, int i)
{
	mutex_lock(&ext_route_tbl[i].wait_lock);
	if (ext_route_tbl[i].wait_resp) {
		hwlog_debug("wait resp is not null. port: %d",
			    ext_route_tbl[i].port);
		if (ext_route_tbl[i].wait_resp->service_id ==
		    resp_buf->service_id &&
		    ext_route_tbl[i].wait_resp->command_id ==
		    resp_buf->command_id) {
			hwlog_info("match wait resp %s.sid: %d,cid: %d,port: %d",
				   __func__, resp_buf->service_id,
				   resp_buf->command_id, ext_route_tbl[i].port);
			ext_route_tbl[i].wait_resp->data = kmalloc(
				resp_buf->read_cnt, GFP_KERNEL);
			if (memcpy_s(ext_route_tbl[i].wait_resp->data,
				     resp_buf->read_cnt,
				     resp_buf->buffer,
				     resp_buf->read_cnt) != EOK) {
				mutex_unlock(&ext_route_tbl[i].wait_lock);
				return false;
			}
			ext_route_tbl[i].wait_resp->data_len =
			resp_buf->read_cnt;
			/* wake up resp_wait */
			wake_up_interruptible(&ext_route_tbl[i].resp_wait);
			mutex_unlock(&ext_route_tbl[i].wait_lock);
			return true;
		}
	}
	mutex_unlock(&ext_route_tbl[i].wait_lock);
	return false;
}

static void ext_sensorhub_route_data(
	struct ext_sensorhub_buf_list *resp_buf, int i)
{
	struct ext_sensorhub_buf_list *read_buf_tmp = NULL;
	struct ext_sensorhub_buf_list *buf_node = NULL;

	/* callback data */
	if (ext_route_tbl[i].callback) {
		hwlog_debug("callback data, read data cannot get");
		ext_route_tbl[i].callback(
			resp_buf->service_id, resp_buf->command_id,
			resp_buf->buffer, resp_buf->read_cnt);
		/* if a route cantains callback, do not put to read list */
		return;
	}

	mutex_lock(&buf_list_lock);
	buf_node = get_idle_buf_node(i);
	if (!buf_node) {
		hwlog_err("cannot get idle buffer node, resp will drop. sid: %d, cid: %d",
			  resp_buf->service_id, resp_buf->command_id);
		mutex_unlock(&buf_list_lock);
		/* check other route */
		return;
	}
	/* set resp to read list */
	buf_node->service_id = resp_buf->service_id;
	buf_node->command_id = resp_buf->command_id;
	buf_node->read_cnt = resp_buf->read_cnt;
	buf_node->buffer = kmalloc(buf_node->read_cnt, GFP_KERNEL);
	if (memcpy_s(buf_node->buffer, buf_node->read_cnt,
		     resp_buf->buffer, buf_node->read_cnt) < 0) {
		hwlog_err("cannot copy to idle buffer node");
		mutex_unlock(&buf_list_lock);
		return;
	}

	list_move_tail(&buf_node->list,
		       &ext_route_tbl[i].read_buf_head->list);
	mutex_unlock(&buf_list_lock);

	/* route read buffer get one package, count++ */
	mutex_lock(&ext_route_tbl[i].read_count_lock);
	ext_route_tbl[i].read_count = ext_route_tbl[i].read_count + 1;
	mutex_unlock(&ext_route_tbl[i].read_count_lock);
	/* wake up read_wait */
	wake_up_interruptible(&ext_route_tbl[i].read_wait);
}

void ext_sensorhub_route_append(struct ext_sensorhub_buf_list *resp_buf)
{
	int i;
	bool match_ret = false;

	if (!resp_buf) {
		hwlog_err("%s resp_buf is full", __func__);
		return;
	}

	/* check each route in route table */
	for (i = 0; i < ROUTE_TBL_LEN; ++i) {
		/* first check wait resp */
		if (ext_sensorhub_wait_resp(resp_buf, i))
			continue;

		mutex_lock(&ext_route_tbl[i].sub_cmds_lock);
		match_ret = match_route_cmd(ext_route_tbl[i].subs_cmds,
					    resp_buf);
		mutex_unlock(&ext_route_tbl[i].sub_cmds_lock);

		if (match_ret)
			ext_sensorhub_route_data(resp_buf, i);
	}

	/* free node and data buffer */
	free_buffer_node(resp_buf);
}

static int ext_sensorhub_route_item(unsigned short port,
				    struct ext_sensorhub_route **route_item)
{
	int i;

	for (i = 0; i < ROUTE_TBL_LEN; ++i) {
		if (port == ext_route_tbl[i].port) {
			*route_item = &ext_route_tbl[i];
			hwlog_debug("get route item, port[%d].\n",
				    port, __func__);
			return 0;
		}
	}

	hwlog_err("unknown port: %d in %s.\n", port, __func__);
	return -EINVAL;
}

static bool data_ready(struct ext_sensorhub_route *route_item)
{
	if (!route_item)
		return false;

	/* read buf list is empty, data not ready */
	if (list_empty(&route_item->read_buf_head->list))
		return false;

	return true;
}

int ext_sensorhub_route_reg_cb(unsigned short port,
			       unsigned char *service_ids,
			       unsigned char *command_ids,
			       unsigned short cmd_cnt, data_callback callback)
{
	struct ext_sensorhub_route *route_item = NULL;
	int i;

	if (cmd_cnt > MAX_SUBSCMDS_COUNT) {
		hwlog_err("subscribe cmd count[%d] exceeds", cmd_cnt);
		return -EINVAL;
	}

	if (ext_sensorhub_route_item(port, &route_item) != 0) {
		hwlog_err("sensorhub_route_item failed in %s\n", __func__);
		return -EINVAL;
	}

	mutex_lock(&route_item->sub_cmds_lock);
	if (route_item->subs_cmds) {
		/* free last subscribed */
		kfree(route_item->subs_cmds->command_ids);
		route_item->subs_cmds->command_ids = NULL;
		kfree(route_item->subs_cmds->service_ids);
		route_item->subs_cmds->service_ids = NULL;

		route_item->subs_cmds->cmd_cnt = cmd_cnt;
		route_item->subs_cmds->service_ids = service_ids;
		route_item->subs_cmds->command_ids = command_ids;

	} else {
		/* kfree when exit */
		route_item->subs_cmds = kmalloc(sizeof(*route_item->subs_cmds),
						GFP_KERNEL);
		route_item->subs_cmds->cmd_cnt = cmd_cnt;
		route_item->subs_cmds->service_ids = service_ids;
		route_item->subs_cmds->command_ids = command_ids;
	}

	/* delete in unregister and free service_ids command_ids */
	route_item->callback = callback;
	mutex_unlock(&route_item->sub_cmds_lock);

	for (i = 0; i < route_item->subs_cmds->cmd_cnt; ++i)
		hwlog_debug("route_item sid:%d, cid: %d\n",
			    route_item->subs_cmds->service_ids[i],
			    route_item->subs_cmds->command_ids[i]);

	return 0;
}

int ext_sensorhub_route_unreg_cb(unsigned short port)
{
	struct ext_sensorhub_route *route_item = NULL;

	hwlog_err("begin in %s, port: %d\n", __func__, port);
	if (ext_sensorhub_route_item(port, &route_item) != 0) {
		hwlog_err("sensorhub_route_item failed in %s\n", __func__);
		return -EINVAL;
	}

	mutex_lock(&route_item->sub_cmds_lock);
	if (route_item->subs_cmds) {
		kfree(route_item->subs_cmds->service_ids);
		kfree(route_item->subs_cmds->command_ids);

		route_item->subs_cmds->cmd_cnt = 0;
		route_item->subs_cmds->service_ids = NULL;
		route_item->subs_cmds->command_ids = NULL;

		kfree(route_item->subs_cmds);
		route_item->subs_cmds = NULL;
	}

	/* set to NULL */
	route_item->callback = NULL;
	mutex_unlock(&route_item->sub_cmds_lock);
	return 0;
}

long ext_sensorhub_route_config(unsigned short port,
				unsigned char __user *service_ids,
				unsigned char __user *command_ids,
				unsigned short cmd_cnt)
{
	struct ext_sensorhub_route *route_item = NULL;
	unsigned char *sids = NULL;
	unsigned char *cids = NULL;
	int i;

	if (cmd_cnt > MAX_SUBSCMDS_COUNT) {
		hwlog_err("subscribe cmd count[%d] exceeds", cmd_cnt);
		return -EINVAL;
	}

	if (cmd_cnt == 0)
		/* clear subs_cmds */
		return ext_sensorhub_route_unreg_cb(port);

	if (ext_sensorhub_route_item(port, &route_item) != 0) {
		hwlog_err("sensorhub_route_item failed in %s\n", __func__);
		return -EINVAL;
	}

	sids = kmalloc(cmd_cnt, GFP_KERNEL);
	cids = kmalloc(cmd_cnt, GFP_KERNEL);
	if (!sids || !cids)
		return -ENOMEM;

	/* get service ids from user space */
	if (copy_from_user(sids, service_ids, cmd_cnt))
		return -EFAULT;

	/* get command ids from user space */
	if (copy_from_user(cids, command_ids, cmd_cnt))
		return -EFAULT;

	mutex_lock(&route_item->sub_cmds_lock);
	if (route_item->subs_cmds) {
		kfree(route_item->subs_cmds->service_ids);
		kfree(route_item->subs_cmds->command_ids);
	} else {
		/* kfree when exit */
		route_item->subs_cmds = kmalloc(sizeof(*route_item->subs_cmds),
						GFP_KERNEL);
	}
	route_item->subs_cmds->cmd_cnt = cmd_cnt;
	route_item->subs_cmds->service_ids = sids;
	route_item->subs_cmds->command_ids = cids;
	route_item->callback = NULL;
	mutex_unlock(&route_item->sub_cmds_lock);

	return 0;
}

ssize_t ext_sensorhub_route_read_kernel(unsigned short port, char *buf,
					size_t count, unsigned char *service_id,
					unsigned char *command_id)
{
	struct ext_sensorhub_route *route_item = NULL;
	struct ext_sensorhub_buf_list *read_buf_node = NULL;
	ssize_t status;
	int ret;

	if (ext_sensorhub_route_item(port, &route_item) != 0) {
		hwlog_err("sensorhub_route_item failed in %s\n", __func__);
		return -EINVAL;
	}

	hwlog_debug("begin wait_event_interruptible, port[%d].\n",
		    port, __func__);
	/* woke up by signal */
	ret = wait_event_interruptible(route_item->read_wait,
				       data_ready(route_item));
	if (ret != 0) {
		hwlog_err("wait interruptible for read[port:%d] ret:%d",
			  port, ret);
		return -EFAULT;
	}

	hwlog_debug("end wait_event_interruptible, port[%d].\n",
		    port, __func__);

	mutex_lock(&route_item->read_lock);
	/* get read buf */
	read_buf_node = list_first_entry(&route_item->read_buf_head->list,
					 struct ext_sensorhub_buf_list, list);

	*service_id = read_buf_node->service_id;
	*command_id = read_buf_node->command_id;
	/* copy to user */
	status = read_buf_node->read_cnt;

	if (status > 0) {
		/* check user buf len(count) */
		if (count < status) {
			status = count;
			hwlog_err("read sid:%d, cid:%d, status:%d, recevie buffer len:%d not enough.\n",
				  *service_id, *command_id, status, count);
		}
		if (memcpy_s(buf, count, read_buf_node->buffer, status) < 0)
			hwlog_err("copy to read buffer error");
	}

	/* free kmalloc buffer */
	kfree(read_buf_node->buffer);
	read_buf_node->buffer = NULL;
	mutex_unlock(&route_item->read_lock);

	mutex_lock(&buf_list_lock);
	/* move to idle list */
	list_move_tail(&read_buf_node->list, &idle_buf_head->list);
	mutex_unlock(&buf_list_lock);

	/* route buffer list can set one more package */
	mutex_lock(&route_item->read_count_lock);
	route_item->read_count = route_item->read_count - 1;
	if (route_item->read_count <= 0)
		route_item->read_count = 0;

	mutex_unlock(&route_item->read_count_lock);

	return status;
}

ssize_t ext_sensorhub_route_read(unsigned short port, char __user *buf,
				 size_t count, unsigned char *service_id,
				 unsigned char *command_id)
{
	int ret = 0;
	int status = 0;
	int missing = 0;
	char *buffer_read = NULL;

	if (count <= 0) {
		hwlog_err("%s read count[%d] is invalid", __func__, count);
		return -EINVAL;
	}
	count = ext_sensorhub_get_min(count, FRAME_BUF_LEN);
	buffer_read = kmalloc(count, GFP_KERNEL);
	if (!buffer_read) {
		hwlog_err("%s cannot malloc buffer for length.", __func__);
		return -ENOMEM;
	}

	ret = ext_sensorhub_route_read_kernel(port, buffer_read, count,
					      service_id, command_id);
	if (ret <= 0) {
		kfree(buffer_read);
		buffer_read = NULL;
		return ret;
	}
	status = ret;
	if (status > count) {
		status = count;
		hwlog_err("%s read buffer exceeds, len result: %d.",
			  __func__, status);
	}

	missing = copy_to_user(buf, buffer_read, status);
	if (missing == status) {
		hwlog_err("cannot copy to user for read data, port:%d", port);
		status = -EFAULT;
	} else {
		status = status - missing;
	}
	kfree(buffer_read);
	buffer_read = NULL;
	return status;
}

static int wait_write_resp(unsigned short port, struct command *cmd,
			   struct cmd_resp *resp,
			   struct ext_sensorhub_route *route_item)
{
	int ret;
	int wait_len;

	/* wait for resp timeout */
	ret = wait_event_interruptible_timeout(
		route_item->resp_wait, route_item->wait_resp->data_len >= 0,
		msecs_to_jiffies(WAIT_RESP_TIMEOUT_MS));

	hwlog_debug("wait left time ms: %d", jiffies_to_msecs(ret));
	hwlog_debug("end wait resp in:%s port = %d. sid:%d, cid:%d",
		    __func__, port, cmd->service_id, cmd->command_id);
	if (ret == 0) {
		hwlog_err("wait resp timeout in:%s port = %d. sid:%d, cid:%d\n",
			  __func__, port, cmd->service_id, cmd->command_id);
		ret = WAIT_RESP_TIMEOUT_ERROR_CODE;
		goto err;
	}
	if (ret < 0) {
		hwlog_err("wait resp error: %d", ret);
		goto err;
	}
	/* check len. kfree wait_resp buffer, missing */
	wait_len = route_item->wait_resp->data_len;
	if (wait_len > resp->receive_buffer_len) {
		hwlog_err("%s not enough, port = %d. sid:%d, cid:%d, wait:%d, buffer:%d ",
			  __func__, port, cmd->service_id, cmd->command_id,
			  wait_len, resp->receive_buffer_len);
		wait_len = resp->receive_buffer_len;
	}

	ret = memcpy_s(resp->receive_buffer, resp->receive_buffer_len,
		       route_item->wait_resp->data, wait_len);
	if (ret >= 0)
		ret = wait_len;
	mutex_lock(&route_item->wait_lock);
	kfree(route_item->wait_resp->data);
	route_item->wait_resp->data = NULL;
	mutex_unlock(&route_item->wait_lock);
err:
	mutex_lock(&route_item->wait_lock);
	kfree(route_item->wait_resp);
	route_item->wait_resp = NULL;
	mutex_unlock(&route_item->wait_lock);
	return ret;
}

ssize_t ext_sensorhub_route_write_kernel(unsigned short port,
					 struct command *cmd,
					 bool need_resp, struct cmd_resp *resp)
{
	int ret;
	struct ext_sensorhub_route *route_item = NULL;
	unsigned char *send_buffer = NULL;
	unsigned char tid = TRANSACTION_ID_AP_TO_MCU;

	if (cmd->send_buffer_len < 0 ||
	    cmd->send_buffer_len > MAX_SEND_BUFFER_LEN)
		return -EINVAL;

	if (ext_sensorhub_route_item(port, &route_item) != 0) {
		hwlog_err("ext_sensorhub_route_write failed in %s port = %d!\n",
			  __func__, (int)port);
		return -EINVAL;
	}

	if (port == ROUTE_BT_PORT) {
		hwlog_debug("port is bt, set tid");
		tid = TRANSACTION_ID_AP_TO_BT;
	}

	/* the same port need write one by one */
	mutex_lock(&route_item->write_lock);

	send_buffer = cmd->send_buffer;
	/* need wait resp */
	if (need_resp) {
		hwlog_debug("need wait resp in:%s port = %d. sid:%d, cid:%d\n",
			    __func__, port, cmd->service_id, cmd->command_id);
		route_item->wait_resp = NULL;
		route_item->wait_resp = kmalloc(
			sizeof(struct wait_resp), GFP_KERNEL);
		if (!route_item->wait_resp)
			goto err;
		route_item->wait_resp->service_id = cmd->service_id;
		route_item->wait_resp->command_id = cmd->command_id;
		route_item->wait_resp->data_len = -1;
	}

	ret = send_frame_data(tid, cmd->service_id, cmd->command_id,
			      send_buffer, cmd->send_buffer_len);
	if (ret < 0) {
		hwlog_err("%s send error, port:%d ret: %d\n", __func__, port,
			  ret);
		if (need_resp) {
			mutex_lock(&route_item->wait_lock);
			kfree(route_item->wait_resp);
			route_item->wait_resp = NULL;
			mutex_unlock(&route_item->wait_lock);
		}

		goto err;
	}

	/* need wait resp */
	if (need_resp)
		ret = wait_write_resp(port, cmd, resp, route_item);

err:
	mutex_unlock(&route_item->write_lock);
	return ret;
}

static int make_kernel_param(struct ioctl_send_data *send_data,
			     struct command *cmd, struct cmd_resp *resp)
{
	cmd->service_id = send_data->service_id;
	cmd->command_id = send_data->command_id;
	cmd->send_buffer_len = send_data->send_buffer_len;
	cmd->send_buffer = NULL;
	if (cmd->send_buffer_len < 0 ||
	    cmd->send_buffer_len > MAX_SEND_BUFFER_LEN) {
		return -EINVAL;
	}
	cmd->send_buffer = kmalloc(cmd->send_buffer_len, GFP_KERNEL);
	if (!cmd->send_buffer)
		return -ENOMEM;

	/* get data from user */
	if (copy_from_user(cmd->send_buffer, send_data->send_buffer,
			   send_data->send_buffer_len) < 0) {
		kfree(cmd->send_buffer);
		cmd->send_buffer = NULL;
		hwlog_err("%s copy from user error", __func__);
		return -EFAULT;
	}

	if (send_data->need_resp) {
		resp->receive_buffer_len = ext_sensorhub_get_min(
			send_data->receive_buffer_len, FRAME_BUF_LEN);
		resp->receive_buffer = kmalloc(resp->receive_buffer_len,
					       GFP_KERNEL);
		if (!resp->receive_buffer) {
			kfree(cmd->send_buffer);
			cmd->send_buffer = NULL;
			hwlog_err("%s cannot malloc mem for resp.\n", __func__);
			return -ENOMEM;
		}
	}
	return 0;
}

ssize_t ext_sensorhub_route_write(unsigned short port,
				  struct ioctl_send_data *send_data)
{
	int ret;
	int resp_len;
	struct command cmd;
	struct cmd_resp resp;

	ret = make_kernel_param(send_data, &cmd, &resp);
	if (ret < 0)
		return ret;

	ret = ext_sensorhub_route_write_kernel(port, &cmd, send_data->need_resp,
					       &resp);
	if (ret <= 0) {
		kfree(cmd.send_buffer);
		cmd.send_buffer = NULL;
		if (send_data->need_resp) {
			kfree(resp.receive_buffer);
			resp.receive_buffer = NULL;
			hwlog_err("%s write kernel error port:%d, ret:%d\n",
				  __func__, port, ret);
		}

		return ret;
	}

	resp_len = ret;
	if (send_data->need_resp) {
		hwlog_debug("%s copy resp to user, resp len: %d\n",
			    __func__, resp_len);
		if (resp_len > send_data->receive_buffer_len) {
			resp_len = send_data->receive_buffer_len;
			hwlog_err("%s resp data len exceeds buffer len %d, port: %d\n",
				  __func__, resp_len, port);
		}

		/* resp data copy to user */
		ret = copy_to_user(send_data->receive_buffer,
				   resp.receive_buffer, resp_len);
		if (ret != 0) {
			hwlog_err("%s copy wait resp to user error, port = %d. sid:%d, cid:%d\n",
				  __func__, port,
			send_data->service_id, send_data->command_id);
		} else {
			/* return resp len */
			ret = resp_len;
		}

		kfree(resp.receive_buffer);
		resp.receive_buffer = NULL;
	}

	kfree(cmd.send_buffer);
	cmd.send_buffer = NULL;
	return ret;
}
