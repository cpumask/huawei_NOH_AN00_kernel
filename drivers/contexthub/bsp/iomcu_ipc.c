/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Contexthub activity recognition driver.
 * Author: Huawei
 * Create: 2017-03-31
 */
#include <linux/types.h>
#include <linux/completion.h>
#include <linux/hisi/contexthub/iomcu_shmem.h>
#include <linux/hisi/contexthub/iomcu_ipc.h>
#include <linux/hisi/contexthub/iomcu_dump.h>
#include <linux/hisi/hisi_mailbox.h>
#include <linux/hisi/hisi_rproc.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/mutex.h>
#include <linux/semaphore.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/types.h>
#include <linux/workqueue.h>
#include <securec.h>
#include <securectype.h>


int __weak inputhub_recv_msg_app_handler(const pkt_header_t *head,
	bool is_notifier)
{ return 0; }
void __weak inputhub_pm_callback(pkt_header_t *head)
{}
void __weak inputhub_update_info(const void *buf,
	int ret, bool is_in_recovery)
{}
void __weak init_locks(void)
{}
void __weak init_mcu_notifier_list(void)
{}
void __weak inputhub_debuginfo_update(pkt_header_t *head)
{}


#define IPCSHM_DEBUG pr_debug

#define IOM3_ST_NORMAL   0
#define IOM3_ST_RECOVERY 1
#define IOM3_ST_REPEAT   2

#define SH_FAULT_IPC_TX_TIMEOUT_CNT 32
#define SH_FAULT_IPC_RX_TIMEOUT_CNT 5

#define IOMCU_IPC_RX_WAIT_TIMEOUT 2000

struct iomcu_notifier_work {
	struct work_struct worker;
	void *data;
};

struct iomcu_notifier_node {
	int tag;
	int cmd;
	int (*notify)(const pkt_header_t *data);
	struct workqueue_struct *wq;
	struct work_struct works;
	struct list_head entry;
};

struct iomcu_notifier {
	struct list_head head;
	spinlock_t lock;
	struct workqueue_struct *iomcu_notifier_wq;
};

struct iomcu_event_waiter {
	struct completion complete;
	struct read_info *rd;
	uint8_t tag;
	uint8_t cmd;
	uint16_t tranid;
	struct list_head entry;
};

static struct mcu_event_wait_list {
	spinlock_t slock;
	struct list_head head;
} mcu_event_wait_list;

static atomic_t  g_ipc_atomic_tranid  = ATOMIC_INIT(0);
static atomic_t  g_ipc_atomic_senderr  = ATOMIC_INIT(0);
static atomic_t  g_ipc_atomic_recverr  = ATOMIC_INIT(0);

static struct iomcu_notifier iomcu_notifier = {
	LIST_HEAD_INIT(iomcu_notifier.head)
};


static inline bool tag_cmd_match(uint8_t tag, uint8_t cmd,
	uint16_t tranid, pkt_header_t *recv)
{
	return (tag == recv->tag) && ((cmd + 1) == recv->cmd) &&
		   (tranid == recv->tranid);
}


static int iomcu_ipc_send(const char *buf, unsigned int length)
{
	mbox_msg_len_t len;
	int ret;

	len = (length + sizeof(mbox_msg_t) - 1) / (sizeof(mbox_msg_t));
	ret =
	RPROC_SYNC_SEND(ipc_ap_to_iom_mbx, (mbox_msg_t *)buf, len, NULL, 0);
	if (ret) {
		pr_err("ap ipc err[%d], iom3_rec_state[%d]g_iom3_state[%d]iom3_power_state[%d]\n",
		ret, atomic_read(&iom3_rec_state), g_iom3_state, iom3_power_state);
		return -1;
	}
	return ret;
}


static void init_wait_entry_add_list(struct iomcu_event_waiter *waiter)
{
	unsigned long flags = 0;

	init_completion(&waiter->complete);
	spin_lock_irqsave(&mcu_event_wait_list.slock, flags);
	list_add(&waiter->entry, &mcu_event_wait_list.head);
	spin_unlock_irqrestore(&mcu_event_wait_list.slock, flags);
}


static void list_del_iomcu_event_waiter(struct iomcu_event_waiter *self)
{
	unsigned long flags = 0;

	spin_lock_irqsave(&mcu_event_wait_list.slock, flags);
	list_del(&self->entry);
	spin_unlock_irqrestore(&mcu_event_wait_list.slock, flags);
}


static void iomcu_data_distribution(pkt_header_t *src,
	struct iomcu_event_waiter *des)
{
	pkt_subcmd_resp_t *head = (pkt_subcmd_resp_t *)src;
	int ret;
	int secfloor_size = sizeof(head->hd.errno) + sizeof(head->subcmd);

	if (des->rd == NULL)
		return;

	/* even cmds are resp cmd */
	if (0 == (src->cmd & 1)) {
		if (head->hd.length <= MAX_PKT_LENGTH + secfloor_size) {
			des->rd->errno = head->hd.errno;
			if (head->hd.length < secfloor_size) {
				pr_err("%s head->hd.length %u error\n", __func__, head->hd.length);
				return;
			}
			des->rd->data_length = head->hd.length - secfloor_size;
			ret = memcpy_s(des->rd->data, MAX_PKT_LENGTH,
			(char *)head + sizeof(pkt_subcmd_resp_t),
			des->rd->data_length);

			if (ret != EOK)
				pr_err("%s memcpy_s error\n", __func__);
		} else {
			des->rd->errno = -EINVAL;
			des->rd->data_length = 0;
			pr_err("data too large from mcu in %s\n", __func__);
		}
	}
}


static void wake_up_iomcu_event_waiter(pkt_header_t *head)
{
	unsigned long flags = 0;
	struct iomcu_event_waiter *pos = NULL;
	struct iomcu_event_waiter *n = NULL;

	spin_lock_irqsave(&mcu_event_wait_list.slock, flags);
	list_for_each_entry_safe(pos, n, &mcu_event_wait_list.head, entry) {
		if (tag_cmd_match(pos->tag, pos->cmd, pos->tranid, head)) {
			iomcu_data_distribution(head, pos);
			complete(&pos->complete);
		}
	}
	spin_unlock_irqrestore(&mcu_event_wait_list.slock, flags);
}


static int iomcu_send_cmd(pkt_header_t *pkt, unsigned int length)
{
	int ret;

	if (length > MAX_PKT_LENGTH) {
		pr_err("length = %u is too large in %s\n", length, __func__);
		return -EINVAL;
	}

	if (pkt->tag < TAG_BEGIN || pkt->tag >= TAG_END) {
		pr_err("tag = %d is wrong in %s\n", pkt->tag, __func__);
		return -EINVAL;
	}

	ret = iomcu_ipc_send((const char *)pkt, length);
	if (atomic_read(&iom3_rec_state) == IOM3_RECOVERY_IDLE && (ret != 0)) {
		pr_warn("%s iom3 is recoverying [%d]\n", __func__, atomic_read(&g_ipc_atomic_senderr));
		if (atomic_inc_return(&g_ipc_atomic_senderr) > SH_FAULT_IPC_TX_TIMEOUT_CNT) {
			pr_err("SH_FAULT_IPC_TX_TIMEOUT tag[%u]cmd[%u]\n", pkt->tag, pkt->cmd);
			iom3_need_recovery(SENSORHUB_MODID,
				SH_FAULT_IPC_TX_TIMEOUT);
		}
	} else {
		atomic_set(&g_ipc_atomic_senderr, 0);
	}

	inputhub_update_info((const char *)pkt, ret, true);
	return ret;
}


static int iomcu_write_ipc_msg_sync(pkt_header_t *pkt, unsigned int length,
	struct read_info *rd, uint8_t waiter_cmd)
{
	int ret;
	struct iomcu_event_waiter waiter;

	pkt->resp =
		pkt->hw_trans_mode == IOMCU_TRANS_SHMEM_TYPE ? NO_RESP : RESP;

	waiter.rd = rd;
	waiter.tag = pkt->tag;
	waiter.cmd = waiter_cmd;
	waiter.tranid = pkt->tranid;
	init_wait_entry_add_list(&waiter);

	IPCSHM_DEBUG("[%s]R[%d]T[0x%x]C[0x%x]L[%u]id[0x%x]\n",
	__func__, pkt->resp, pkt->tag, pkt->cmd, pkt->length, pkt->tranid);

	ret = iomcu_send_cmd(pkt, length);
	if (ret != 0)
		goto FIN;

	ret = wait_for_completion_timeout(&waiter.complete, msecs_to_jiffies(IOMCU_IPC_RX_WAIT_TIMEOUT));
	if (ret == 0) {
		ret = -ETIME;
		pr_warn("[%s][repoTIMEOUT]tag[0x%x]cmd[0x%x]resp[%u]hw[%u]len[%u]tranid[0x%x]\n",
		__func__, pkt->tag, pkt->cmd, pkt->resp, pkt->hw_trans_mode, pkt->length, pkt->tranid);
		if (atomic_inc_return(&g_ipc_atomic_recverr) > SH_FAULT_IPC_RX_TIMEOUT_CNT) {
			pr_err("SH_FAULT_IPC_RX_TIMEOUT tag[%u]cmd[%u]\n", pkt->tag, pkt->cmd);
			iom3_need_recovery(SENSORHUB_MODID,
				SH_FAULT_IPC_RX_TIMEOUT);
		}
	} else {
		ret = 0;
		atomic_set(&g_ipc_atomic_recverr, 0);
	}
FIN:
	list_del_iomcu_event_waiter(&waiter);
	return ret;
}


int iomcu_write_ipc_msg(pkt_header_t *pkt, unsigned int length,
	struct read_info *rd, uint8_t waiter_cmd)
{
	int ret = 0;

	if (pkt == NULL)
		return -EINVAL;

	if (rd == NULL) {
		pkt->resp = NO_RESP;
		IPCSHM_DEBUG("[%s]resp[%d]tag[0x%x]cmd[0x%x]len[%u]\n",
		__func__, pkt->resp, pkt->tag, pkt->cmd, pkt->length);
		ret = iomcu_send_cmd(pkt, length);
	} else {
		ret = iomcu_write_ipc_msg_sync(pkt, length, rd, waiter_cmd);
	}
	return ret;
}


static int write_customize_cmd_para_check(const struct write_info *wr)
{
	if (wr == NULL) {
		pr_err("NULL pointer in %s\n", __func__);
		return -EINVAL;
	}

	if (wr->tag < TAG_BEGIN || wr->tag >= TAG_END || wr->wr_len < 0 ||
		wr->wr_len > (int)shmem_get_capacity()) {
		pr_err("[%s]tag[0x%x]orLen[0x%x]err\n", __func__,
			wr->tag, wr->wr_len);
		return -EINVAL;
	}

	if (wr->wr_len != 0 && wr->wr_buf == NULL) {
		pr_err("tag = %d, wr_len %d  error in %s\n",
		wr->tag, wr->wr_len, __func__);
		return -EINVAL;
	}

	return 0;
}


int write_customize_cmd(const struct write_info *wr, struct read_info *rd,
	bool is_lock)
{
	int ret;
	unsigned int send_length;
	char buf[MAX_SEND_LEN];
	pkt_header_t *pkt = (pkt_header_t *)buf;

	(void)memset_s(buf, sizeof(buf), 0, sizeof(buf));
	ret = write_customize_cmd_para_check(wr);
	if (ret != 0)
		return ret;
	ret = iomcu_check_dump_status();
	if (ret != 0)
		return ret;

	pkt->tranid = (uint8_t)atomic_inc_return(&g_ipc_atomic_tranid);
	IPCSHM_DEBUG("[%s]tag[0x%x]cmd[0x%x]len[%d]tranid[0x%x]\n",
	__func__, wr->tag, wr->cmd, wr->wr_len, pkt->tranid);

	if (wr->wr_len <= (int)IPC_BUF_USR_SIZE) {
		pkt->tag = wr->tag;
		pkt->app_tag = wr->app_tag;
		pkt->cmd = wr->cmd;
		pkt->length = wr->wr_len;
		pkt->hw_trans_mode = IOMCU_TRANS_IPC_TYPE;
		if (wr->wr_len != 0) {
			ret = memcpy_s((void *)(pkt + 1), IPC_BUF_USR_SIZE,
				wr->wr_buf, wr->wr_len);
			if (ret != EOK) {
				pr_err("%s memcpy_s error\n", __func__);
				return ret;
			}
		}

		send_length = pkt->length + sizeof(pkt_header_t);
		return iomcu_write_ipc_msg(pkt, send_length, rd, wr->cmd);
	} else {
		return ipcshm_send((struct write_info *)wr, pkt, rd);
	}
}

/* lint -e86 */
static void init_iomcu_notifier_list(void)
{
	INIT_LIST_HEAD(&iomcu_notifier.head);
	spin_lock_init(&iomcu_notifier.lock);
	iomcu_notifier.iomcu_notifier_wq =
		create_freezable_workqueue("iomcu_ipc");
}

static void init_mcu_event_wait_list(void)
{
	INIT_LIST_HEAD(&mcu_event_wait_list.head);
	spin_lock_init(&mcu_event_wait_list.slock);
}


static void iomcu_notifier_handler(struct work_struct *work)
{
	struct iomcu_notifier_work *p =
		container_of(work, struct iomcu_notifier_work, worker);
	struct iomcu_notifier_node *pos = NULL;
	struct iomcu_notifier_node *n = NULL;
	unsigned long flags = 0;
	const pkt_header_t *cur_pht = (pkt_header_t *)p->data;

	IPCSHM_DEBUG("%s tag[0x%x]cmd[0x%x]len[%u]\n",
	__func__, cur_pht->tag, cur_pht->cmd, cur_pht->length);

	spin_lock_irqsave(&iomcu_notifier.lock, flags);
	list_for_each_entry_safe(pos, n, &iomcu_notifier.head, entry) {
		if ((pos->tag == cur_pht->tag) &&
		    (pos->cmd == cur_pht->cmd || pos->cmd == CMD_ALL)) {
			if (pos->notify != NULL) {
				spin_unlock_irqrestore(&iomcu_notifier.lock,
					flags);
				pos->notify((const pkt_header_t *)p->data);
				spin_lock_irqsave(&iomcu_notifier.lock, flags);
			}
		}
	}
	spin_unlock_irqrestore(&iomcu_notifier.lock, flags);
	(void)inputhub_recv_msg_app_handler(cur_pht, true);

	kfree(p->data);
	kfree(p);
}


int register_mcu_event_notifier(int tag,
	int cmd, int (*notify)(const pkt_header_t *head))
{
	struct iomcu_notifier_node *pnode = NULL;
	int ret = 0;
	unsigned long flags = 0;

	if ((!(tag >= TAG_BEGIN && tag < TAG_END)) || notify == NULL)
		return -EINVAL;

	spin_lock_irqsave(&iomcu_notifier.lock, flags);
	list_for_each_entry(pnode, &iomcu_notifier.head, entry) {
		if ((tag == pnode->tag) && (cmd == pnode->cmd)
			&& (notify == pnode->notify)) {
			pr_warn("T[0x%x]C[0x%x]N[%pK]already registed%s\n!",
			tag, cmd, notify, __func__);
			goto out;
		}
	}

	pnode = kzalloc(sizeof(struct iomcu_notifier_node), GFP_ATOMIC);
	if (pnode == NULL) {
		ret = -ENOMEM;
		goto out;
	}
	pnode->tag = tag;
	pnode->cmd = cmd;
	pnode->notify = notify;

	list_add(&pnode->entry, &iomcu_notifier.head);
out:
	spin_unlock_irqrestore(&iomcu_notifier.lock, flags);
	return ret;
}


int unregister_mcu_event_notifier(int tag, int cmd,
	int (*notify)(const pkt_header_t *head))
{
	struct iomcu_notifier_node *pos = NULL;
	struct iomcu_notifier_node *n = NULL;
	unsigned long flags = 0;

	if ((!(tag >= TAG_BEGIN && tag < TAG_END)) || notify == NULL)
		return -EINVAL;

	spin_lock_irqsave(&iomcu_notifier.lock, flags);
	list_for_each_entry_safe(pos, n, &iomcu_notifier.head, entry) {
		if ((tag == pos->tag) && (cmd == pos->cmd) &&
			(notify == pos->notify)) {
			list_del(&pos->entry);
			kfree(pos);
			break;
		}
	}
	spin_unlock_irqrestore(&iomcu_notifier.lock, flags);
	return 0;
}


int iomcu_route_recv_mcu_data(const char *buf, unsigned int length)
{
	const pkt_header_t *head = (const pkt_header_t *)buf;
	struct iomcu_notifier_work *notifier_work = NULL;
	int ret;

	if (head == NULL)
		return 0;

	IPCSHM_DEBUG("[%s]tag[0x%x]cmd[0x%x]len[%u]tran[0x%x]\n",
		__func__, head->tag, head->cmd, head->length, head->tranid);

	if (head->length > shmem_get_capacity()) {
		pr_err("[%s]len[0x%x]too big.\n", __func__,
			head->length);
		return 0;
	}

	notifier_work = kzalloc(sizeof(struct iomcu_notifier_work), GFP_ATOMIC);
	if (notifier_work == NULL)
		return 0;

	if (head->hw_trans_mode == IOMCU_TRANS_SHMEM_TYPE) {
		notifier_work->data = ipcshm_get_data(buf);
		if (notifier_work->data == NULL) {
			kfree(notifier_work);
			pr_err("[%s] shmem_get_data error\n", __func__);
			return 0;
		}
	} else {
		notifier_work->data =
		kzalloc(head->length + sizeof(pkt_header_t), GFP_ATOMIC);
		if (notifier_work->data == NULL) {
			kfree(notifier_work);
			pr_err("[%s] kzalloc notifier error\n", __func__);
			return 0;
		}

		ret = memcpy_s(notifier_work->data, MAX_PKT_LENGTH, head,
			sizeof(pkt_header_t) + head->length);
		if (ret != EOK)
			pr_err("%s memcpy_s error\n", __func__);
	}

	inputhub_debuginfo_update(notifier_work->data);
	wake_up_iomcu_event_waiter(notifier_work->data);
	inputhub_pm_callback(notifier_work->data);

	INIT_WORK(&notifier_work->worker, iomcu_notifier_handler);
	queue_work(iomcu_notifier.iomcu_notifier_wq, &notifier_work->worker);

	return 0;
}

void iomcu_ipc_init(void)
{
	init_locks();
	init_iomcu_notifier_list();
	init_mcu_event_wait_list();
	(void)contexthub_shmem_init();
}
/* lint +e86 */

