/*
 * hisi_rproc.c
 *
 * hisi rproc communication interfacer
 *
 * Copyright (c) 2018-2020 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#include <linux/hisi/hisi_rproc.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/notifier.h>
#include <linux/delay.h>
#include <linux/wait.h>
#include <linux/hisi/hisi_mailbox.h>
#include <linux/hisi/hisi_log.h>

#define HISI_LOG_TAG AP_MAILBOX_TAG

#define rproc_pr_err(fmt, args...)                                             \
	do {                                                                   \
		pr_err(fmt "\n", ##args);                                      \
	} while (0)

#define rproc_pr_debug(fmt, args...)                       \
	do {                                                   \
	} while (0)

enum call_type_t {
	ASYNC_CALL = 0,
	SYNC_CALL
};

struct hisi_rproc_info {
	rproc_id_t rproc_id;
	struct atomic_notifier_head notifier;
	struct notifier_block nb;
	struct hisi_mbox *mbox;
};

static int is_rproc_table_ready;

#define rproc_table_delay()                                                    \
	do {                                                                   \
		is_rproc_table_ready = 1;                                      \
	} while (0)
#define rproc_table_not_delay()                                                \
	do {                                                                   \
		is_rproc_table_ready = 0;                                      \
	} while (0)
#define get_ready_rproc_table_delay() ({ is_rproc_table_ready; })

static struct hisi_rproc_info rproc_table[HISI_RPROC_NUM];


static struct hisi_rproc_info *find_rproc(rproc_id_t rproc_id)
{
	struct hisi_rproc_info *rproc = NULL;
	unsigned int i;

	for (i = 0; i < sizeof(rproc_table) / sizeof(*rproc); i++) {
		if (rproc_id == rproc_table[i].rproc_id &&
			rproc_table[i].mbox != NULL) {
			rproc = &rproc_table[i];
			break;
		}
	}
	return rproc;
}

int hisi_rproc_xfer_async(
	rproc_id_t rproc_id, rproc_msg_t *msg, rproc_msg_len_t len)
{
	struct hisi_rproc_info *rproc = NULL;
	struct hisi_mbox_task *tx_task = NULL;
	struct hisi_mbox *mbox = NULL;
	mbox_ack_type_t ack_type = AUTO_ACK;
	int ret;

	if (len > MBOX_CHAN_DATA_SIZE) {
		ret = -EINVAL;
		goto out;
	}

	rproc = find_rproc(rproc_id);
	if (!rproc) {
		rproc_pr_err("%s:invalid rproc xfer:%u", __func__, rproc_id);
		ret = -EINVAL;
		goto out;
	}

	mbox = rproc->mbox;

	tx_task = hisi_mbox_task_alloc(mbox, msg, len, ack_type);
	if (!tx_task) {
		rproc_pr_err("no mem");
		ret = -ENOMEM;
		goto out;
	}

	ret = hisi_mbox_msg_send_async(mbox, tx_task);
	if (ret) {
		/* -12:tx_fifo full */
		rproc_pr_err(
			"%s async send failed, errno: %d", mbox->tx->name, ret);
		hisi_mbox_task_free(&tx_task);
	}

out:
	return ret;
}
EXPORT_SYMBOL(hisi_rproc_xfer_async);

int hisi_rproc_xfer_sync(rproc_id_t rproc_id, rproc_msg_t *msg,
	rproc_msg_len_t len, rproc_msg_t *ack_buffer,
	rproc_msg_len_t ack_buffer_len)
{
	struct hisi_rproc_info *rproc = NULL;
	struct hisi_mbox *mbox = NULL;
	mbox_ack_type_t ack_type = MANUAL_ACK;
	int ret;

	if (len > MBOX_CHAN_DATA_SIZE || ack_buffer_len > MBOX_CHAN_DATA_SIZE) {
		ret = -EINVAL;
		goto out;
	}

	rproc = find_rproc(rproc_id);
	if (!rproc) {
		rproc_pr_err("%s:invalid rproc xfer:%u", __func__, rproc_id);
		ret = -EINVAL;
		goto out;
	}

	mbox = rproc->mbox;

	ret = hisi_mbox_msg_send_sync(
		mbox, msg, len, ack_type, ack_buffer, ack_buffer_len);
	if (ret)
		rproc_pr_err("fail to sync send");

out:
	return ret;
}
EXPORT_SYMBOL(hisi_rproc_xfer_sync);

static int hisi_rproc_rx_notifier(
	struct notifier_block *nb, unsigned long len, void *msg)
{
	struct hisi_rproc_info *rproc =
		container_of(nb, struct hisi_rproc_info, nb);

	atomic_notifier_call_chain(&rproc->notifier, len, msg);
	return 0;
}

int hisi_rproc_rx_register(rproc_id_t rproc_id, struct notifier_block *nb)
{
	struct hisi_rproc_info *rproc = NULL;

	if (!nb) {
		rproc_pr_err("invalid notifier block");
		return -EINVAL;
	}

	rproc = find_rproc(rproc_id);
	if (!rproc) {
		rproc_pr_err("%s:invalid rproc xfer:%u", __func__, rproc_id);
		return -EINVAL;
	}
	atomic_notifier_chain_register(&rproc->notifier, nb);
	return 0;
}
EXPORT_SYMBOL(hisi_rproc_rx_register);

int hisi_rproc_rx_unregister(rproc_id_t rproc_id, struct notifier_block *nb)
{
	struct hisi_rproc_info *rproc = NULL;

	if (!nb) {
		rproc_pr_err("invalid notifier block");
		return -EINVAL;
	}

	rproc = find_rproc(rproc_id);
	if (!rproc) {
		rproc_pr_err("%s:invalid rproc xfer:%u", __func__, rproc_id);
		return  -EINVAL;
	}
	atomic_notifier_chain_unregister(&rproc->notifier, nb);
	return 0;
}
EXPORT_SYMBOL(hisi_rproc_rx_unregister);

/*
 * Function name:hisi_rproc_put.
 * Discription:release the ipc channel's structure, it's usually called by
 * module_exit function, but the module_exit function should never be used  .
 * Parameters:
 * @ rproc_id_t
 * return value:
 * @ -ENODEV-->failed, other-->succeed.
 */
int hisi_rproc_put(rproc_id_t rproc_id)
{
	struct hisi_rproc_info *rproc = NULL;
	unsigned int i;

	for (i = 0; i < sizeof(rproc_table) / sizeof(*rproc); i++) {
		rproc = &rproc_table[i];
		if (rproc->mbox && rproc_id == rproc->rproc_id) {
			hisi_mbox_put(&rproc->mbox);
			break;
		}
	}
	if (unlikely(sizeof(rproc_table) / sizeof(*rproc) == i)) {
		if (!rproc)
			rproc_pr_err("[%s]rproc pointer is null!\n", __func__);
		else
			rproc_pr_err("\nrelease the ipc channel %d 's structure failed\n",
				rproc->rproc_id);
		return -ENODEV;
	}
	return 0;
}

/*
 * Function name:hisi_rproc_flush_tx.
 * Discription:flush the tx_work queue.
 * Parameters:
 * @ rproc_id_t
 * return value:
 * @ -ENODEV-->failed, other-->succeed.
 */
int hisi_rproc_flush_tx(rproc_id_t rproc_id)
{
	struct hisi_rproc_info *rproc = NULL;
	unsigned int i;

	for (i = 0; i < sizeof(rproc_table) / sizeof(*rproc); i++) {
		rproc = &rproc_table[i];
		/* MBOX8/9/23/24 may be null in austin and dallas */
		if (!rproc->mbox)
			continue;
		if (rproc->mbox->tx && rproc_id == rproc->rproc_id) {
			hisi_mbox_empty_task(rproc->mbox->tx);
			break;
		}
	}

	if (unlikely(sizeof(rproc_table) / sizeof(*rproc) == i))
		return -ENODEV;

	return 0;
}

/*
 * Function name:hisi_rproc_is_suspend.
 * Discription:judge the mailbox is suspend.
 * Parameters:
 * return value:
 * @ 0-->not suspend, other-->suspend.
 */
int hisi_rproc_is_suspend(rproc_id_t rproc_id)
{
	struct hisi_rproc_info *rproc = NULL;
	struct hisi_mbox_device *mdev = NULL;
	unsigned long flags = 0;
	int ret = 0;

	rproc = find_rproc(rproc_id);
	if (!rproc || !rproc->mbox || !rproc->mbox->tx) {
		rproc_pr_err("%s:invalid rproc xfer:%u", __func__, rproc_id);
		return -EINVAL;
	}

	mdev = rproc->mbox->tx;
	spin_lock_irqsave(&mdev->status_lock, flags);
	if ((MDEV_DEACTIVATED & mdev->status))
		ret = -ENODEV;
	spin_unlock_irqrestore(&mdev->status_lock, flags);
	return ret;
}
EXPORT_SYMBOL(hisi_rproc_is_suspend);

static int hisi_rproc_table_init(
	int cur_index, int rproc_start, int rproc_end)
{
	int i;

	for (i = 0; i < (rproc_end - rproc_start); i++) {
		rproc_table[cur_index].rproc_id = i + rproc_start;
		cur_index++;
	}
	rproc_pr_debug("%s: start-%d, end-%d, cur_index-%d", __func__,
		rproc_start, rproc_end, cur_index);
	return cur_index;
}

int hisi_rproc_init(void)
{
	struct hisi_rproc_info *rproc = NULL;
	struct hisi_mbox_task *ptask = NULL;
	unsigned int i;
	int cur_index = 0;

	if (!get_ready_rproc_table_delay()) {
		rproc_table_delay();
		/* PERI_NS_IPC rproc table init */
		cur_index = hisi_rproc_table_init(
			cur_index, HISI_RPROC_LPM3_MBX0, HISI_RPROC_NSIPC_MAX);
		/* AO_NS_IPC rproc table init */
		cur_index = hisi_rproc_table_init(
			cur_index, HISI_RPROC_AO_MBX0, HISI_RPROC_AO_NSIPC_MAX);
		/* NPU_IPC rproc table init */
		cur_index = hisi_rproc_table_init(
			cur_index, HISI_RPROC_NPU_MBX0, HISI_RPROC_NPU_IPC_MAX);

		/* PERI_CRG_NS_IPC rproc table init */
		cur_index = hisi_rproc_table_init(cur_index,
			HISI_RPROC_CFGIPC_MBX0, HISI_RPROC_CFGIPC_MAX);
	}

	for (i = 0; i < sizeof(rproc_table) / sizeof(*rproc); i++) {
		rproc = &rproc_table[i];
		if (!rproc->mbox) {
			ATOMIC_INIT_NOTIFIER_HEAD(&rproc->notifier);

			rproc->nb.next = NULL;
			rproc->nb.notifier_call = hisi_rproc_rx_notifier;
			/*
			 * rproc->rproc_id as mdev_index to get the right
			 * mailbox-dev
			 */
			rproc->mbox =
				hisi_mbox_get(rproc->rproc_id, &rproc->nb);
			if (!rproc->mbox) {
				rproc_pr_debug("%s rproc[%d] mbox is not exist",
					__func__, rproc->rproc_id);
				continue;
			}
			rproc_pr_debug("%s rproc[%d] get mbox", __func__,
				rproc->rproc_id);
		}
	}

	if (!g_txtaskbuffer) {
		g_txtaskbuffer = (struct hisi_mbox_task *)kzalloc(
			TX_TASK_DDR_NODE_NUM * sizeof(*ptask), GFP_KERNEL);
		if (!g_txtaskbuffer) {
			rproc_pr_err("\n failed to get g_txtaskbuffer\n");
			return -ENOMEM;
		}

		ptask = g_txtaskbuffer;
		for (i = 0; i < TX_TASK_DDR_NODE_NUM; i++) {
			/*
			 * init the tx buffer 's node , set
			 * the flag to available
			 */
			ptask->tx_buffer[0] = TX_TASK_DDR_NODE_AVA;
			ptask++;
		}
	}

	return 0;
}
EXPORT_SYMBOL(hisi_rproc_init);

static void __exit hisi_rproc_exit(void)
{
	struct hisi_rproc_info *rproc = NULL;
	unsigned int i;

	rproc_table_not_delay();

	for (i = 0; i < sizeof(rproc_table) / sizeof(*rproc); i++) {
		rproc = &rproc_table[i];
		if (rproc->mbox)
			hisi_mbox_put(&rproc->mbox);
	}
}

module_exit(hisi_rproc_exit);
