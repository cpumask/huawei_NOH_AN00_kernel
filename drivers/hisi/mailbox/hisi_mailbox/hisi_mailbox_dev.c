/*
 * hisi_mailbox_dev.c
 *
 * Hisilicon mailbox device driver
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
#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/hisi/hisi_irq_affinity.h>
#include <linux/hisi/hisi_mailbox.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/kern_levels.h>
#include <linux/mod_devicetable.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
#include <linux/slab.h>
#include <linux/version.h>
#include <securec.h>
#include <linux/hisi/hisi_rproc.h>
#include <soc_sctrl_interface.h>
#include <linux/hisi/hisi_log.h>

#define HISI_LOG_TAG AP_MAILBOX_TAG

#define FAST_MBOX (1 << 0)
#define COMM_MBOX (1 << 1)
#define SOURCE_MBOX (1 << 2)
#define DESTINATION_MBOX (1 << 3)

#define EVERY_LOOP_TIME_MS 5

#define IPC_UNLOCKED 0x00000000
#define IPCACKMSG 0x00000000
#define COMM_MBOX_IRQ (-2)
#define AUTOMATIC_ACK_CONFIG (1 << 0)
#define NO_FUNC_CONFIG (0 << 0)

/* Optimize interrupts assignment */
#define IPC_IRQ_AFFINITY_CPU 1

#define STATE_NUMBER 0x4

#define MAILBOX_ASYNC_UDELAY_CNT 1000
#define DEFAULT_MAILBOX_TIMEOUT 300
#define DEFAULT_FIFO_SIZE 256
#define DEFAULT_SCHED_PRIORITY 20
#define MAILBOX_NO_USED 0
#define MAX_AP_IPC_INDEX 99

#define RPROC_NUMBER 16
#define IPC_MAX_SRC_ID 2
#define IPC_FUNC 3
#define IPC_RESIDUE 100

#define SCTRL_TIME_ADDR (SOC_SCTRL_SCBBPDRXSTAT1_ADDR(0))

#define mdev_err(fmt, args...)                                                 \
	do {                                                                   \
		pr_err(fmt "\n", ##args);                                      \
	} while (0)

#define mdev_info(fmt, args...)                                                \
	do {                                                                   \
		pr_info(fmt "\n", ##args);                                     \
	} while (0)

#define mdev_debug(fmt, args...)                                               \
	do {                                                              \
	} while (0)

enum { RX_BUFFER_TYPE = 0,
	ACK_BUFFER_TYPE,
	MBOX_BUFFER_TYPE_MAX
};

/*
 * Table for available remote processors. DTS sub-node, "remote_processor_type",
 * of node, "hisi_mdev", is configured according to the table.
 * If the table was modified, DTS configiuration should be updated accordingly.
 */
enum remote_processor_type_t {
	GIC = 0,
	GIC_1 = 0,
	GIC_2,
	IOM3,
	LPM3,
	HIFI,
	MCPU,
	BBE16,
	IVP32,
	ISP,
	UNCERTAIN_REMOTE_PROCESSOR,
	NPU_IPC_GIC = 2,
	HI3XXX_RP_TYPES
};

struct hisi_common_mbox_info {
	int gic_1_irq_requested;
	int gic_2_irq_requested;
	int cmbox_gic_1_irq;
	int cmbox_gic_2_irq;
	struct hisi_mbox_device *cmdev;
};

struct hisi_ipc_device {
	void __iomem *base;
	void __iomem *system_time_reg_addr;
	u32 unlock;
	mbox_msg_t *buf_pool;
	unsigned int ipc_type;
	unsigned int capability;
	unsigned int rproc_src_id[IPC_MAX_SRC_ID];
	const char *rproc_name[RPROC_NUMBER];
	struct hisi_common_mbox_info *cmbox_info;
	struct hisi_mbox_device **mdev_res;
};

struct hisi_mbox_device_priv {
	u8 func;
	enum remote_processor_type_t src;
	enum remote_processor_type_t des;
	int mbox_channel;
	int irq;
	int capability;
	int used;
	unsigned int timeout;
	unsigned int fifo_size;
	unsigned int sched_priority;
	unsigned int sched_policy;
	struct hisi_ipc_device *idev;
};

/*
 * HiIPCV230 have a state machine, the state machine have 4 status:
 * 4'b0001:IDLE_STATE
 * 4'b0010:SOURCE_STATE
 * 4'b0100:DEST_STATE
 * 4'b1000:ACK_STATE
 */
char *ipc_state_name[STATE_NUMBER] = {
	"%s  is idle\n",
	"%s  is occupied\n",
	"%s  may be power off or freeze\n",
	"%s  have no time to handle ACK\n"
};

enum IPC_STATE_MACHINE {
	IDLE_STATE,
	SOURCE_STATE,
	DEST_STATE,
	ACK_STATE
};

unsigned char _rproc_find_index(const char *mdev_name, unsigned int pro_code)
{
	unsigned char index = 0;

	while (pro_code) {
		index++;
		pro_code >>= 1;
	}
	return index;
}

static const char *rproc_analysis(
	struct hisi_mbox_device *mdev, unsigned int pro_code)
{
	const char *mdev_name = mdev->name;
	struct hisi_mbox_device_priv *priv = mdev->priv;
	unsigned char index = _rproc_find_index(mdev_name, pro_code);
	if (!index)
		return "ERR_RPROC";
	index--;
	if (likely(index < RPROC_NUMBER))
		return (priv->idev->rproc_name[index]);
	else
		return "ERR_RPROC";
}

char *ipc_state_analysis(unsigned int mode, unsigned char *outp)
{
	unsigned char index = 0;

	if (!outp)
		return "%s ERR_STATE\n";

	mode >>= 4; /* bit4~bit7 is the state machine index */
	while (mode) {
		index++;
		mode >>= 1;
	}
	if (likely(index))
		index--;
	else
		return "%s ERR_STATE\n";

	*outp = index;

	if (likely(index < STATE_NUMBER))
		return ipc_state_name[index];
	else
		return "%s ERR_STATE\n";
}

static inline void __ipc_lock(void __iomem *base, unsigned int lock_key)
{
	__raw_writel(lock_key, base + IPCLOCK());
}

static inline void __ipc_unlock(void __iomem *base, unsigned int key)
{
	__raw_writel(key, base + IPCLOCK());
}

static inline unsigned int __ipc_lock_status(const void __iomem *base)
{
	return __raw_readl(base + IPCLOCK());
}

static inline void __ipc_set_src(void __iomem *base, int source, int mdev)
{
	__raw_writel(IPCBITMASK((unsigned int)source),
		base + IPCMBxSOURCE((unsigned int)mdev));
}

static inline unsigned int __ipc_read_src(const void __iomem *base, int mdev)
{
	return __raw_readl(
		base + IPCMBxSOURCE((unsigned int)mdev));
}

static inline void __ipc_set_des(void __iomem *base, int source, int mdev)
{
	__raw_writel(IPCBITMASK((unsigned int)source),
		base + IPCMBxDSET((unsigned int)mdev));
}

static inline void __ipc_clr_des(void __iomem *base, int source, int mdev)
{
	__raw_writel(IPCBITMASK((unsigned int)source),
		base + IPCMBxDCLR((unsigned int)mdev));
}

static inline unsigned int __ipc_des_status(const void __iomem *base, int mdev)
{
	return __raw_readl(
		base + IPCMBxDSTATUS((unsigned int)mdev));
}

static inline void __ipc_send(void __iomem *base, unsigned int tosend, int mdev)
{
	__raw_writel(
		tosend, base + IPCMBxSEND((unsigned int)mdev));
}

static inline unsigned int __ipc_read(
	const void __iomem *base, int mdev, int index)
{
	return __raw_readl(base + IPCMBxDATA((unsigned int)mdev,
		(unsigned int)index));
}

static inline void __ipc_write(
	void __iomem *base, u32 data, int mdev, int index)
{
	__raw_writel(data, base + IPCMBxDATA((unsigned int)mdev,
		(unsigned int)index));
}

static inline unsigned int __ipc_cpu_imask_get(
	const void __iomem *base, int mdev)
{
	return __raw_readl(
		base + IPCMBxIMASK((unsigned int)mdev));
}

static inline void __ipc_cpu_imask_clr(
	void __iomem *base, unsigned int toclr, int mdev)
{
	unsigned int reg;

	reg = __raw_readl(
		base + IPCMBxIMASK((unsigned int)mdev));
	reg = reg & (~(toclr));

	__raw_writel(
		reg, base + IPCMBxIMASK((unsigned int)mdev));
}

static inline void __ipc_cpu_imask_all(void __iomem *base, int mdev)
{
	__raw_writel(
		(~0), base + IPCMBxIMASK((unsigned int)mdev));
}

static inline void __ipc_cpu_iclr(
	void __iomem *base, unsigned int toclr, int mdev)
{
	__raw_writel(
		toclr, base + IPCMBxICLR((unsigned int)mdev));
}

static inline int __ipc_cpu_istatus(const void __iomem *base, int mdev)
{
	return __raw_readl(
		base + IPCMBxICLR((unsigned int)mdev));
}


static inline unsigned int __ipc_mbox_irstatus(
	const struct hisi_mbox_device_priv *priv)
{
	return __raw_readl(
		priv->idev->base + IPCCPUxIRST((unsigned int)priv->src));
}

static inline unsigned int __ipc_mbox_istatus(
	const struct hisi_mbox_device_priv *priv, unsigned int src)
{
	return __raw_readl(
		priv->idev->base + IPCCPUxIMST(src));
}


static inline unsigned int __ipc_status(const void __iomem *base, int mdev)
{
	return __raw_readl(
		base + IPCMBxMODE((unsigned int)mdev));
}

static inline void __ipc_mode(void __iomem *base, unsigned int mode, int mdev)
{
	__raw_writel(
		mode, base + IPCMBxMODE((unsigned int)mdev));
}

static int hisi_mdev_startup(struct hisi_mbox_device *mdev)
{
	/*
	 * nothing won't be done during suspend & resume flow for HI3xxx IPC.
	 * see dummy like SR function, hisi_mdev_suspend & hisi_mdev_resume.
	 * reserve runtime power management proceeding for further modification,
	 * if necessary.
	 */
	return 0;
}

static void hisi_mdev_shutdown(struct hisi_mbox_device *mdev)
{
	/*
	 * nothing won't be done during suspend & resume flow for HI3xxx IPC.
	 * see dummy like SR function, hisi_mdev_suspend & hisi_mdev_resume.
	 * reserve runtime power management proceeding for further modification,
	 * if necessary.
	 */
}

#define HISI_MDEV_MAX_ANALYSIS_BUFFER 512
static void hisi_mdev_dump_status(struct hisi_mbox_device *mdev)
{
	struct hisi_mbox_device_priv *priv = mdev->priv;
	/*
	 * the size 512 is the sumary max size of  sys_rproc_name and
	 * ipc_state_name
	 */
	char finalfortmat[HISI_MDEV_MAX_ANALYSIS_BUFFER] = {0};
	char statem = 0;
	int ret;

	const char *src_name = rproc_analysis(
		mdev, __ipc_read_src(priv->idev->base, priv->mbox_channel));
	const char *des_name = rproc_analysis(
		mdev, __ipc_des_status(priv->idev->base, priv->mbox_channel));
	/* \0013 is the  KERN_SOH KERN_ERR */
	char *direcstr = KERN_ERR "[ap_ipc]: [%s]-->[%s], ";
	char *machinestr = ipc_state_analysis(
		__ipc_status(priv->idev->base, priv->mbox_channel),
		(unsigned char *)&statem);

	ret = memcpy_s(finalfortmat, HISI_MDEV_MAX_ANALYSIS_BUFFER, direcstr,
		    strlen(direcstr));
	if (ret != EOK)
		mdev_err("%s memcpy_s error", __func__);
	ret = strncat_s(finalfortmat, sizeof(finalfortmat), machinestr,
		    strlen(machinestr));
	if (ret != EOK) {
		mdev_err("%s strncat_s error", __func__);
		return;
	}

	if (statem == DEST_STATE)
		printk(finalfortmat, src_name, des_name, des_name);
	else if (statem == ACK_STATE)
		printk(finalfortmat, src_name, des_name, src_name);
	else
		printk(finalfortmat, src_name, des_name, mdev->name);
}

static void hisi_mdev_dump_regs(struct hisi_mbox_device *mdev)
{
}

static int hisi_mdev_check(
	struct hisi_mbox_device *mdev, mbox_mail_type_t mtype, int mdev_index)
{
	struct hisi_mbox_device_priv *priv = mdev->priv;
	int ret = RPUNACCESSIBLE;
	int index = priv->mbox_channel;

	index = index + priv->idev->ipc_type;
	if ((mtype == TX_MAIL) && (SOURCE_MBOX & priv->func) &&
		(index == mdev_index) && (priv->used == 1))
		ret = RPACCESSIBLE;
	else if ((mtype == RX_MAIL) && (DESTINATION_MBOX & priv->func) &&
		 (index == mdev_index) && (priv->used == 1))
		ret = RPACCESSIBLE;

	return ret;
}

static void hisi_mdev_clr_ack(struct hisi_mbox_device *mdev)
{
	struct hisi_mbox_device_priv *priv = mdev->priv;
	unsigned int imask;
	unsigned int toclr;

	imask = __ipc_cpu_imask_get(priv->idev->base, priv->mbox_channel);
	toclr = (IPCBITMASK(priv->idev->rproc_src_id[0]) |
			IPCBITMASK(priv->idev->rproc_src_id[1])) &
		(~imask);
	__ipc_cpu_iclr(priv->idev->base, toclr, priv->mbox_channel);
}

static void hisi_mdev_clr_irq_and_ack(struct hisi_mbox_device *mdev)
{
	struct hisi_mbox_device_priv *priv = mdev->priv;
	unsigned int status;
	unsigned int imask;
	unsigned int todo;
	int i;

	/*
	 * temporarily, local processor will clean msg register,
	 * and ack zero for an ipc from remote processors.
	 */
	for (i = 0; i < priv->capability; i++)
		__ipc_write(priv->idev->base, IPCACKMSG, priv->mbox_channel, i);

	imask = __ipc_cpu_imask_get(priv->idev->base, priv->mbox_channel);
	/*
	 * get the irq unmask core bits, and clear the irq according to the
	 * unmask core bits,
	 * because the irq to be sure triggered to the unmasked cores
	 */
	todo = (IPCBITMASK(priv->idev->rproc_src_id[0]) |
		       IPCBITMASK(priv->idev->rproc_src_id[1])) &
	       (~imask);

	__ipc_cpu_iclr(priv->idev->base, todo, priv->mbox_channel);

	status = __ipc_status(priv->idev->base, priv->mbox_channel);
	if ((DESTINATION_STATUS & status) &&
		(!(AUTOMATIC_ACK_CONFIG & status)))
		__ipc_send(priv->idev->base, todo, priv->mbox_channel);
}

static void hisi_mdev_ack(
	struct hisi_mbox_device *mdev, mbox_msg_t *msg, mbox_msg_len_t len)
{
}

static mbox_msg_len_t hisi_mdev_hw_read(
	struct hisi_mbox_device *mdev, mbox_msg_t *msg)
{
	struct hisi_mbox_device_priv *priv = mdev->priv;
	mbox_msg_len_t cap = priv->capability;
	int i;

	for (i = 0; i < cap; i++)
		msg[i] = __ipc_read(priv->idev->base, priv->mbox_channel, i);

	return cap;
}
/*
 * to judge the four kind machine status of the ip,
 * they are idle,src,des,ack
 */
static int hisi_mdev_is_stm(struct hisi_mbox_device *mdev, unsigned int stm)
{
	struct hisi_mbox_device_priv *priv = mdev->priv;
	int is_stm = 0;

	if ((stm & __ipc_status(priv->idev->base, priv->mbox_channel)))
		is_stm = 1;

	return is_stm;
}

static mbox_msg_len_t hisi_mdev_receive_msg(
	struct hisi_mbox_device *mdev, mbox_msg_t **buf)
{
	mbox_msg_t *_buf = NULL;
	mbox_msg_len_t len = 0;

	if (!buf)
		return len;

	if (hisi_mdev_is_stm(mdev, ACK_STATUS))
		_buf = mdev->ack_buffer;
	else
		_buf = mdev->rx_buffer;

	if (_buf)
		len = hisi_mdev_hw_read(mdev, _buf);
	*buf = _buf;

	hisi_mdev_clr_irq_and_ack(mdev);
	return len;
}

static int hisi_mdev_unlock(struct hisi_mbox_device *mdev)
{
	struct hisi_mbox_device_priv *priv = mdev->priv;
	int retry = 3; /* unclok retry cnt */

	do {
		__ipc_unlock(priv->idev->base, priv->idev->unlock);
		if (__ipc_lock_status(priv->idev->base) == IPC_UNLOCKED)
			break;

		udelay(10); /* wait for ipc unlock */
		retry--;
	} while (retry);

	if (!retry)
		return -ENODEV;

	return 0;
}

static int hisi_mdev_occupy(struct hisi_mbox_device *mdev)
{
	struct hisi_mbox_device_priv *priv = mdev->priv;
	int retry = 10; /* Mailbox status judgment times */

	do {
		/*
		 * Hardware lock
		 * A hardware lock is needed here to lock a mailbox resource,
		 * which could be used by another remote proccessor, such as
		 * a HiIPCV230 common mailbox-25/mailbox-26.
		 */
		if (!(__ipc_status(priv->idev->base, priv->mbox_channel) &
			    IDLE_STATUS)) {
			asm volatile("wfe");
		} else {
			/*
			 * set the source processor bit, we set common mailbox's
			 * source processor bit through dtsi
			 */
			__ipc_set_src(priv->idev->base, priv->src,
				priv->mbox_channel);
			if (__ipc_read_src(
				    priv->idev->base, priv->mbox_channel) &
				IPCBITMASK((unsigned int)priv->src))
				break;
		}
		retry--;
		/* Hardware unlock */
	} while (retry);

	if (!retry)
		return -ENODEV;

	return 0;
}

static int hisi_mdev_hw_send(struct hisi_mbox_device *mdev, mbox_msg_t *msg,
	mbox_msg_len_t len, int ack_mode)
{
	struct hisi_mbox_device_priv *priv = mdev->priv;
	int i;
	unsigned int temp;

	/* interrupts unmask */
	__ipc_cpu_imask_all(priv->idev->base, priv->mbox_channel);

	if (ack_mode == AUTO_ACK)
		temp = IPCBITMASK((unsigned int)priv->des);
	else
		temp = IPCBITMASK((unsigned int)priv->src) |
		       IPCBITMASK((unsigned int)priv->des);

	__ipc_cpu_imask_clr(priv->idev->base, temp, priv->mbox_channel);

	/* des config */

	__ipc_set_des(priv->idev->base, priv->des, priv->mbox_channel);

	/* ipc mode config */
	if (ack_mode == AUTO_ACK)
		temp = AUTOMATIC_ACK_CONFIG;
	else
		temp = NO_FUNC_CONFIG;

	__ipc_mode(priv->idev->base, temp, priv->mbox_channel);

	/* write data */
	for (i = 0; i < ((priv->capability < len) ? priv->capability : len);
		i++)
		__ipc_write(priv->idev->base, msg[i], priv->mbox_channel, i);

	/* enable sending */
	__ipc_send(priv->idev->base, IPCBITMASK((unsigned int)priv->src),
		priv->mbox_channel);
	return 0;
}

static void hisi_mdev_ensure_channel(struct hisi_mbox_device *mdev)
{
	struct hisi_mbox_device_priv *priv = mdev->priv;
	int loop = priv->timeout /
		EVERY_LOOP_TIME_MS + MAILBOX_ASYNC_UDELAY_CNT;
	int timeout = 0;

	if (mdev->ops->is_stm(mdev, IDLE_STATUS)) {
		return; /* IDLE STATUS, return directly */
	} else if (mdev->ops->is_stm(mdev, ACK_STATUS)) {
		/*
		 * the ack status is reached, just release, the sync and async
		 * is mutexed by by mdev->dev_lock
		 */
		goto release; /* ACK STATUS, release the channel directly */
	} else { /* DEST and SRC STATUS, the dest is processing, wait here */
		/* the worst situation is to delay 1000*5us+60*5ms = 305ms */
		while (timeout < loop) {
			if (timeout < MAILBOX_ASYNC_UDELAY_CNT) {
				udelay(5);
			} else {
				/*
				 * the hifi may power off when send ipc msg, so
				 * the ack status may wait 20ms
				 */
				usleep_range(3000, 5000); /* Min pause us and max pause us */
			}
			/* if the ack status is ready, break out */
			if (mdev->ops->is_stm(mdev, ACK_STATUS))
				break;
			timeout++;
		}
		if (unlikely(timeout == loop)) {
			mdev_err("%s ipc_timeout", mdev->name);
			if (mdev->ops->status)
				mdev->ops->status(mdev);
		}
		goto release;
	}

release:
	/* release the channel */
	mdev->ops->refresh(mdev);
}

static int hisi_mdev_send_msg(struct hisi_mbox_device *mdev, mbox_msg_t *msg,
	mbox_msg_len_t len, int ack_mode)
{
	struct hisi_mbox_device_priv *priv = mdev->priv;
	int err = 0;

	/* all the mailbox channel is treated as fast-mailbox */
	if (DESTINATION_MBOX & priv->func) {
		mdev_err("mdev %s has no tx ability", mdev->name);
		err = -EMDEVCLEAN;
		goto out;
	}
	/*
	 * Whenever an ipc starts,
	 * ipc module has to be unlocked at the very beginning.
	 */
	if (hisi_mdev_unlock(mdev)) {
		mdev_err("mdev %s can not be unlocked", mdev->name);
		err = -EMDEVCLEAN;
		goto out;
	}

	if (hisi_mdev_occupy(mdev)) {
		mdev_err("mdev %s can not be occupied", mdev->name);
		err = -EMDEVCLEAN;
		goto out;
	}

	(void)hisi_mdev_hw_send(mdev, msg, len, ack_mode);

out:
	return err;
}

static void hisi_mdev_release(struct hisi_mbox_device *mdev)
{
	struct hisi_mbox_device_priv *priv = mdev->priv;

	__ipc_cpu_imask_all(priv->idev->base, priv->mbox_channel);
	__ipc_set_src(priv->idev->base, priv->src, priv->mbox_channel);

	asm volatile("sev");
}

static int hisi_mdev_gic_irq_request(struct hisi_mbox_device *mdev,
	irq_handler_t handler, void *dev_id)
{
	struct hisi_mbox_device_priv *priv = mdev->priv;
	int ret;

	if (!strncmp("ao-mailbox", mdev->name, strlen("ao-mailbox")))
		ret = request_irq(priv->irq, handler,
			IRQF_NO_SUSPEND, mdev->name, dev_id);
	else if (!strncmp("mailbox-4", mdev->name, strlen("mailbox-4")))
		ret = request_irq(priv->irq, handler,
			IRQF_NO_SUSPEND, mdev->name, dev_id);
	else
		ret = request_irq(priv->irq, handler,
			0, mdev->name, dev_id);

	return ret;
}

static int hisi_mdev_irq_request(
	struct hisi_mbox_device *mdev, irq_handler_t handler, void *p)
{
	struct hisi_mbox_device_priv *priv = mdev->priv;

	int ret = 0;

	if (priv->idev->cmbox_info->cmbox_gic_1_irq == priv->irq) {
		if (!priv->idev->cmbox_info->gic_1_irq_requested++) {
			ret = hisi_mdev_gic_irq_request(mdev, handler,
				(void *)priv->idev->cmbox_info->cmdev);
			if (ret) {
				mdev_err("fast source %s request gic_1_irq %d failed",
					mdev->name, priv->irq);
				priv->idev->cmbox_info->gic_1_irq_requested--;
				goto out;
			}
			hisi_irqaffinity_register(
				priv->irq, IPC_IRQ_AFFINITY_CPU);
		}
	} else if (priv->idev->cmbox_info->cmbox_gic_2_irq == priv->irq) {
		if (!priv->idev->cmbox_info->gic_2_irq_requested++) {
			ret = hisi_mdev_gic_irq_request(mdev, handler,
				(void *)priv->idev->cmbox_info->cmdev);
			if (ret) {
				mdev_err("fast source %s request gic_2_irq %d failed",
					mdev->name, priv->irq);
				priv->idev->cmbox_info->gic_2_irq_requested--;
				goto out;
			}
			hisi_irqaffinity_register(
				priv->irq, IPC_IRQ_AFFINITY_CPU);
		}
	} else {
		ret = hisi_mdev_gic_irq_request(mdev, handler, p);
		if (ret) {
			mdev_err("fast desitnation %s request irq %d failed",
				mdev->name, priv->irq);
			goto out;
		}

		hisi_irqaffinity_register(priv->irq, IPC_IRQ_AFFINITY_CPU);
	}

out:
	return ret;
}

static void hisi_mdev_irq_free(struct hisi_mbox_device *mdev, void *p)
{
	struct hisi_mbox_device_priv *priv = mdev->priv;

	if (priv->idev->cmbox_info->cmbox_gic_1_irq == priv->irq) {
		if (!--priv->idev->cmbox_info->gic_1_irq_requested)
			free_irq(priv->irq,
				(void *)priv->idev->cmbox_info->cmdev);
	} else if (priv->idev->cmbox_info->cmbox_gic_2_irq == priv->irq) {
		if (!--priv->idev->cmbox_info->gic_2_irq_requested)
			free_irq(priv->irq,
				(void *)priv->idev->cmbox_info->cmdev);
	} else {
		free_irq(priv->irq, p);
	}
}

static void hisi_mdev_irq_enable(struct hisi_mbox_device *mdev)
{
	enable_irq((unsigned int)mdev->cur_irq);
}

static void hisi_mdev_irq_disable(struct hisi_mbox_device *mdev)
{
	disable_irq_nosync((unsigned int)mdev->cur_irq);
}

static struct hisi_mbox_device *hisi_mdev_irq_to_mdev(
	struct hisi_mbox_device *_mdev, struct list_head *list, int irq)
{
	struct hisi_mbox_device_priv *_priv = NULL;
	struct hisi_mbox_device *mdev = NULL;
	struct hisi_mbox_device_priv *priv = NULL;
	enum remote_processor_type_t src = UNCERTAIN_REMOTE_PROCESSOR;

	unsigned int regval;

	if ((list_empty(list)) || (!_mdev)) {
		mdev_err("invalid input");
		goto out;
	}

	_priv = _mdev->priv;

	/* fast destination mailboxes use unique irq number */
	if ((DESTINATION_MBOX & _priv->func) && (FAST_MBOX & _priv->func)) {
		mdev = _mdev;
		goto out;
	}

	if (_priv->idev->rproc_src_id[0] == _priv->idev->rproc_src_id[1]) {
		src = _priv->idev->rproc_src_id[0];
	} else {
		/*
		 * fast source & common mailboxes share
		 * GIC_1 & GIC_2 irq number
		 */
		if (irq == _priv->idev->cmbox_info->cmbox_gic_1_irq) {
			src = _priv->idev->rproc_src_id[0];
		} else if (irq == _priv->idev->cmbox_info->cmbox_gic_2_irq) {
			src = _priv->idev->rproc_src_id[1];
		} else {
			mdev_err("odd irq for hisi mailboxes");
			goto out;
		}
	}

	regval = __ipc_mbox_istatus(_priv, src);
	if (!regval) {
		mdev = NULL;
		goto out;
	}
	list_for_each_entry(mdev, list, node) {
		priv = mdev->priv;

		if ((regval & IPCBITMASK((unsigned int)priv->mbox_channel)) &&
			(priv->func & SOURCE_MBOX))
			goto out;
	}

out:
	/* it is nearly occured */
	return mdev;
}

void __iomem *hisi_mdev_system_time_reg(struct hisi_mbox_device *mdev)
{
	struct hisi_mbox_device_priv *priv = mdev->priv;

	return priv->idev->system_time_reg_addr;
}

static unsigned int hisi_mdev_timeout(struct hisi_mbox_device *mdev)
{
	struct hisi_mbox_device_priv *priv = mdev->priv;

	return priv->timeout;
}

static unsigned int hisi_mdev_fifo_size(struct hisi_mbox_device *mdev)
{
	struct hisi_mbox_device_priv *priv = mdev->priv;

	return priv->fifo_size;
}

static unsigned int hisi_mdev_sched_priority(struct hisi_mbox_device *mdev)
{
	struct hisi_mbox_device_priv *priv = mdev->priv;

	return priv->sched_priority;
}

static unsigned int hisi_mdev_sched_policy(struct hisi_mbox_device *mdev)
{
	struct hisi_mbox_device_priv *priv = mdev->priv;

	return priv->sched_policy;
}

struct hisi_mbox_dev_ops hisi_mdev_ops = {
	.startup = hisi_mdev_startup,
	.shutdown = hisi_mdev_shutdown,
	.check = hisi_mdev_check,
	.recv = hisi_mdev_receive_msg,
	.send = hisi_mdev_send_msg,
	.ack = hisi_mdev_ack,
	.refresh = hisi_mdev_release,
	.get_timeout = hisi_mdev_timeout,
	.get_fifo_size = hisi_mdev_fifo_size,
	.get_sched_priority = hisi_mdev_sched_priority,
	.get_sched_policy = hisi_mdev_sched_policy,
	.request_irq = hisi_mdev_irq_request,
	.free_irq = hisi_mdev_irq_free,
	.enable_irq = hisi_mdev_irq_enable,
	.disable_irq = hisi_mdev_irq_disable,
	.irq_to_mdev = hisi_mdev_irq_to_mdev,
	.is_stm = hisi_mdev_is_stm,
	.clr_ack = hisi_mdev_clr_ack,
	.ensure_channel = hisi_mdev_ensure_channel,
	.status = hisi_mdev_dump_status,
	.dump_regs = hisi_mdev_dump_regs,
};

static void hisi_mdev_put(struct hisi_ipc_device *idev)
{
	struct hisi_mbox_device **list = idev->mdev_res;
	struct hisi_mbox_device *mdev = NULL;
	int i = 0;

	iounmap(idev->base);

	kfree(idev->cmbox_info);
	kfree(idev->buf_pool);
	mdev = list[0];
	while (mdev) {
		i++;
		kfree(mdev->priv);
		kfree(mdev);
		mdev = list[i];
	}
}

static int hisi_mdev_remove(struct platform_device *pdev)
{
	struct hisi_ipc_device *idev = platform_get_drvdata(pdev);

	platform_set_drvdata(pdev, NULL);

	if (idev) {
		hisi_mbox_device_unregister(idev->mdev_res);
		hisi_mdev_put(idev);
		kfree(idev);
	}

	return 0;
}

static struct hisi_common_mbox_info *mdev_get_cmbox_info(struct device_node *node)
{
	int cm_gic_1_irq;
	int cm_gic_2_irq;
	struct hisi_common_mbox_info *cmbox_info = NULL;

	cmbox_info = kzalloc(sizeof(*cmbox_info), GFP_KERNEL);
	if (!cmbox_info)
		return NULL;

	cm_gic_1_irq = irq_of_parse_and_map(node, 0);
	cm_gic_2_irq = irq_of_parse_and_map(node, 1);
	cmbox_info->gic_1_irq_requested = 0;
	cmbox_info->gic_2_irq_requested = 0;
	cmbox_info->cmbox_gic_1_irq = cm_gic_1_irq;
	cmbox_info->cmbox_gic_2_irq = cm_gic_2_irq;
	cmbox_info->cmdev = NULL;
	return cmbox_info;
}

static int mdev_get_rproc_name(struct device_node *node,
	struct hisi_ipc_device *idev)
{
	int ret;
	const char *ipc_rproc_name[RPROC_NUMBER] = {0};
	unsigned int rproc_num = 0;
	unsigned int i;

	ret = of_property_read_u32(node, "rproc_num", &rproc_num);
	if (ret) {
		mdev_err("prop \"rproc_num\" error %d", ret);
		return -ENODEV;
	}

	for (i = 0; i < RPROC_NUMBER; i++) {
		if (i < rproc_num) {
			ret = of_property_read_string_index(
				node, "rproc_name", i, &ipc_rproc_name[i]);
			if (ret < 0) {
				mdev_err("prop rproc_name error %d", ret);
				return -ENODEV;
			}
		} else {
			ipc_rproc_name[i] = "ERR_RPROC";
		}
		mdev_debug("%s rproc_name is %s", __func__, ipc_rproc_name[i]);
	}

	for (i = 0; i < RPROC_NUMBER; i++)
		idev->rproc_name[i] = ipc_rproc_name[i];

	return 0;
}

static void mdev_idev_info_print(struct hisi_ipc_device *idev,
	unsigned int mdev_num)
{
	mdev_debug("ipc_base: 0x%lx", (unsigned long)idev->base);
	mdev_debug("capability: %u", idev->capability);
	mdev_debug("unlock_key: 0x%x", idev->unlock);
	mdev_debug("mailboxes: %u", mdev_num);
	mdev_debug("%s ipc type[%u] rproc_src_id is [%u] [%u]",
		__func__, idev->ipc_type, idev->rproc_src_id[0], idev->rproc_src_id[1]);
	mdev_debug("buffer pool: 0x%lx", (unsigned long)idev->buf_pool);
}

static int mdev_idev_basic_info_get(struct hisi_ipc_device *idev,
	struct device_node *node)
{
	int ret;

	ret = of_property_read_u32(node, "capability", &(idev->capability));
	if (ret) {
		mdev_err("prop \"capability\" error %d", ret);
		return ret;
	}

	ret = of_property_read_u32(node, "unlock_key", &(idev->unlock));
	if (ret) {
		mdev_err("prop \"key\" error %d", ret);
		return ret;
	}

	/* some mbox have 2 src */
	ret = of_property_read_u32_array(
		node, "rproc_src_id", idev->rproc_src_id, 2);
	if (ret) {
		mdev_err("prop \"rproc_src_id\" error %d", ret);
		return ret;
	}

	ret = of_property_read_u32(node, "ipc_type", &(idev->ipc_type));
	if (ret) {
		mdev_err("prop \"ipc_type\" error %d", ret);
		return ret;
	}

	return ret;
}

static int mdev_idev_init(struct hisi_ipc_device *idev,
	struct device_node *node, struct hisi_mbox_device **mdevs)
{
	mbox_msg_t *buf_pool = NULL;
	void __iomem *system_time_reg_addr = NULL;
	mbox_msg_len_t buf_pool_len;
	unsigned int mdev_num;
	int ret;

	idev->base = of_iomap(node, 0);
	if (!idev->base) {
		mdev_err("iomap error");
		goto out;
	}

	system_time_reg_addr = of_iomap(node, 1);
	if (!system_time_reg_addr) {
		idev->system_time_reg_addr = NULL;
	} else {
		idev->system_time_reg_addr = system_time_reg_addr + SCTRL_TIME_ADDR;
	}

	ret = mdev_idev_basic_info_get(idev, node);
	if (ret)
		goto to_iounmap;

	ret = mdev_get_rproc_name(node, idev);
	if (ret)
		goto to_iounmap;

	idev->cmbox_info = mdev_get_cmbox_info(node);
	if (!idev->cmbox_info)
		goto free_cmbox;

	ret = of_property_read_u32(node, "mailboxes", &mdev_num);
	if (ret) {
		mdev_err("prop \"mailboxes\" error %d", ret);
		goto free_cmbox;
	}

	buf_pool_len = idev->capability * MBOX_BUFFER_TYPE_MAX * mdev_num;
	buf_pool = kzalloc(sizeof(*buf_pool) * buf_pool_len, GFP_KERNEL);
	if (!buf_pool)
		goto free_cmbox;

	idev->mdev_res = mdevs;
	idev->buf_pool = buf_pool;

	mdev_idev_info_print(idev, mdev_num);

	return 0;

free_cmbox:
	kfree(idev->cmbox_info);
to_iounmap:
	if (system_time_reg_addr)
		iounmap(system_time_reg_addr);
	iounmap(idev->base);
out:
	return -ENODEV;
}

static void mdev_idev_free(struct hisi_ipc_device *idev)
{
	kfree(idev->buf_pool);
	kfree(idev->cmbox_info);
	iounmap(idev->base);
}

static int mdev_priv_get_irq(struct hisi_ipc_device *idev,
	struct hisi_mbox_device_priv *priv, struct device_node *son,
	struct hisi_mbox_device *mdev)
{
	unsigned int output[IPC_FUNC] = {0};
	int ret;

	ret = of_property_read_u32_array(son, "func", output, IPC_FUNC);
	if (ret)
		return -ENODEV;

	priv->func |= (output[0] ? FAST_MBOX : COMM_MBOX); /* mbox_type */

	priv->func |= (output[1] ? SOURCE_MBOX : 0); /* is_src_mbox */

	priv->func |= (output[2] ? DESTINATION_MBOX : 0); /* is_des_mbox */

	if ((FAST_MBOX & priv->func) && (DESTINATION_MBOX & priv->func)) {
		mdev_debug("func FAST DES MBOX");
		priv->irq = irq_of_parse_and_map(son, 0);
		mdev_debug("irq: %d", priv->irq);
	} else if ((FAST_MBOX & priv->func) && (SOURCE_MBOX & priv->func)) {
		mdev_debug("func FAST SRC MBOX");
		priv->irq = (priv->src == GIC_1) ?
			idev->cmbox_info->cmbox_gic_1_irq :
			idev->cmbox_info->cmbox_gic_2_irq;
		mdev_debug("irq: %d", priv->irq);
		/*
		 * set the cmdev, the cmdev will be used in acore't
		 * interrupts
		 */
		if (!idev->cmbox_info->cmdev)
			idev->cmbox_info->cmdev = mdev;
	} else {
		/* maybe GIC_1 OR GIC_2 */
		mdev_debug(" xxxxxxxxx we don't use comm-mailbox , we use it as fast-mailbox");
		/*
		 * we don't use comm-mailbox ,we use it as fast-mailbox,
		 * please set the comm to fast in the dtsi
		 */
		priv->irq = COMM_MBOX_IRQ;
		idev->cmbox_info->cmdev = mdev;
		mdev_debug("irq: %d\n", priv->irq);
	}

	return 0;
}

static void mdev_priv_info_print(struct hisi_mbox_device_priv *priv)
{
	mdev_debug("src_bit: %d", priv->src);
	mdev_debug("des_bit: %d", priv->des);
	mdev_debug("index: %d", priv->mbox_channel);
	mdev_debug("timeout: %u", priv->timeout);
	mdev_debug("fifo_size: %u", priv->fifo_size);
	mdev_debug("sched_priority: %u", priv->sched_priority);
	mdev_debug("sched_policy: %u", priv->sched_policy);
}

static struct hisi_mbox_device_priv *mdev_priv_init(struct device_node *son,
	struct hisi_ipc_device *idev, struct hisi_mbox_device *mdev, unsigned int used)
{
	int ret;
	struct hisi_mbox_device_priv *priv = NULL;

	priv = kzalloc(sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return NULL;

	ret = of_property_read_u32(son, "src_bit", (u32 *)&(priv->src));
	if (ret)
		goto free_priv;

	ret = of_property_read_u32(son, "des_bit", (u32 *)&(priv->des));
	if (ret)
		goto free_priv;

	/*
	 * get software code-index to mbox_channel and calculate the
	 * right mbox_channel
	 */
	ret = of_property_read_u32(son, "index", (u32 *)&(priv->mbox_channel));
	if (ret)
		goto free_priv;
	/*
	 * to distinguish different ipc and calculate the true
	 * mailbox-index(100)
	 */
	if (priv->mbox_channel > MAX_AP_IPC_INDEX)
		priv->mbox_channel = priv->mbox_channel % IPC_RESIDUE;

	ret = of_property_read_u32(son, "timeout", &(priv->timeout));
	if (ret || (priv->timeout % EVERY_LOOP_TIME_MS != 0))
		priv->timeout = DEFAULT_MAILBOX_TIMEOUT;

	ret = of_property_read_u32(son, "fifo_size", &(priv->fifo_size));
	if (ret)
		priv->fifo_size = DEFAULT_FIFO_SIZE;

	ret = of_property_read_u32(son, "sched_priority",
		&(priv->sched_priority));
	if (ret)
		priv->sched_priority = DEFAULT_SCHED_PRIORITY;

	ret = of_property_read_u32(son, "sched_policy", &(priv->sched_policy));
	if (ret)
		priv->sched_policy = SCHED_RR; /* default is SCHED_RR */

	ret = mdev_priv_get_irq(idev, priv, son, mdev);
	if (ret)
		goto free_priv;

	priv->capability = idev->capability;
	priv->idev = idev;
	priv->used = used;
	mdev_priv_info_print(priv);
	return priv;

free_priv:
	kfree(priv);
	return NULL;
}

static int hisi_mdev_get(struct hisi_ipc_device *idev,
	struct hisi_mbox_device **mdevs, struct device_node *node)
{
	struct device_node *son = NULL;
	struct hisi_mbox_device *mdev = NULL;
	struct hisi_mbox_device_priv *priv = NULL;
	mbox_msg_t *rx_buffer = NULL;
	mbox_msg_t *ack_buffer = NULL;
	mbox_msg_t *buf_pool = NULL;

	int mdev_index_temp = 0; /* some mailbox is not used */
	int ret;
	unsigned int used, i;

	ret = mdev_idev_init(idev, node, mdevs);
	if (ret)
		return ret;

	buf_pool = idev->buf_pool;

	for (i = 0; (son = of_get_next_child(node, son)); i++) {
		used = 0;

		ret = of_property_read_u32(son, "used", &used);
		if (ret) {
			mdev_err("mailbox-%d has no tag <used>",
				mdev_index_temp);
			goto to_break;
		}
		if (used == MAILBOX_NO_USED) {
			mdev_debug("mailbox node %s is not used", son->name);
			continue;
		}

		mdev = kzalloc(sizeof(*mdev), GFP_KERNEL);
		if (!mdev) {
			ret = -ENOMEM;
			goto to_break;
		}

		priv = mdev_priv_init(son, idev, mdev, used);
		if (!priv) {
			ret = -ENOMEM;
			goto free_mdev;
		}

		mdev_debug("mailbox node: %s", son->name);

		rx_buffer = buf_pool + idev->capability * RX_BUFFER_TYPE;
		ack_buffer = buf_pool + idev->capability * ACK_BUFFER_TYPE;
		buf_pool = buf_pool + idev->capability * MBOX_BUFFER_TYPE_MAX;
		mdev_debug("rx_buffer: 0x%pK\nack_buffer: 0x%pK",
			(unsigned long)rx_buffer, (unsigned long)ack_buffer);

		mdev->name = son->name;
		mdev->priv = priv;
		mdev->rx_buffer = rx_buffer;
		mdev->ack_buffer = ack_buffer;
		mdev->ops = &hisi_mdev_ops;

		mdevs[mdev_index_temp] = mdev;
		mdev_index_temp++;
		continue;

free_mdev:
		kfree(mdev);
to_break:
		break;
	}

	if (!ret)
		return ret;

	while (i--) {
		kfree(mdevs[i]->priv);
		kfree(mdevs[i]);
	}
	mdev_idev_free(idev);
	return ret;
}

static int hisi_mdev_probe(struct platform_device *pdev)
{
	struct hisi_ipc_device *idev = NULL;
	struct hisi_mbox_device **mdev_res = NULL;
	struct device_node *node = pdev->dev.of_node;
	int mdev_num;
	int ret;

	if (!node) {
		mdev_err("dts[%s] node not found", "hisilicon,HiIPCV230");
		ret = -ENODEV;
		goto out;
	}

	idev = kzalloc(sizeof(*idev), GFP_KERNEL);
	if (!idev) {
		mdev_err("no mem for ipc resouce");
		ret = -ENOMEM;
		goto out;
	}

	ret = of_property_read_u32(node, "mailboxes", (u32 *)&mdev_num);
	if (ret) {
		mdev_err("no mailboxes resources");
		ret = -ENODEV;
		goto free_idev;
	}

	mdev_res = kzalloc((sizeof(*mdev_res) * (mdev_num + 1)), GFP_KERNEL);
	if (!mdev_res) {
		ret = -ENOMEM;
		goto free_idev;
	}
	mdev_res[mdev_num] = NULL;

	ret = hisi_mdev_get(idev, mdev_res, node);
	if (ret) {
		mdev_err("can not get ipc resource");
		ret = -ENODEV;
		goto free_mdevs;
	}

	ret = hisi_mbox_device_register(&pdev->dev, mdev_res);
	if (ret) {
		mdev_err("mdevs register failed");
		ret = -ENODEV;
		goto put_res;
	}

	platform_set_drvdata(pdev, idev);

	mdev_debug("HiIPCV230 mailboxes are ready");
	/*
	 * we call it here to let the pl011_init can use the
	 * rproc send function
	 */
	hisi_rproc_init();

	return 0;

put_res:
	hisi_mdev_put(idev);
free_mdevs:
	kfree(idev->mdev_res);
	idev->mdev_res = NULL;
free_idev:
	kfree(idev);
out:
	return ret;
}

static int hisi_mdev_suspend(struct device *dev)
{
	struct platform_device *pdev =
		container_of(dev, struct platform_device, dev);
	struct hisi_ipc_device *idev = platform_get_drvdata(pdev);

	mdev_info("%s: suspend +", __func__);
	if (idev)
		hisi_mbox_device_deactivate(idev->mdev_res);
	mdev_info("%s: suspend -", __func__);
	return 0;
}

static int hisi_mdev_resume(struct device *dev)
{
	struct platform_device *pdev =
		container_of(dev, struct platform_device, dev);
	struct hisi_ipc_device *idev = platform_get_drvdata(pdev);

	mdev_info("%s: resume +", __func__);
	if (idev)
		hisi_mbox_device_activate(idev->mdev_res);
	mdev_info("%s: resume -", __func__);
	return 0;
}

#ifdef CONFIG_OF
static const struct of_device_id hisi_mdev_of_match[] = {
	{
		.compatible = "hisilicon,HiIPCV230",
	},
	{},
};

MODULE_DEVICE_TABLE(of, hisi_mdev_of_match);
#endif

static const struct dev_pm_ops hisi_mdev_pm_ops = {
	.suspend_late = hisi_mdev_suspend,
	.resume_early = hisi_mdev_resume,
};

static struct platform_driver hisi_mdev_driver = {
	.probe = hisi_mdev_probe,
	.remove = hisi_mdev_remove,
	.driver = {

			.name = "HiIPCV230-mailbox",
			.owner = THIS_MODULE,
			.of_match_table = of_match_ptr(hisi_mdev_of_match),
			.pm = &hisi_mdev_pm_ops,
		},
};

static int __init hisi_mdev_init(void)
{
	mdev_err("%s init!", __func__);

	platform_driver_register(&hisi_mdev_driver);
	return 0;
}

core_initcall(hisi_mdev_init);

static void __exit hisi_mdev_exit(void)
{
	platform_driver_unregister(&hisi_mdev_driver);
}

module_exit(hisi_mdev_exit);

MODULE_DESCRIPTION("HiIPCV230 ipc, mailbox device driver");
MODULE_LICENSE("GPL V2");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
