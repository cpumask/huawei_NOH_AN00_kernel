/*
 * ext_sensorhub_commu.c
 *
 * code for communication with external sensorhub
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

#include "ext_sensorhub_commu.h"

#include <linux/errno.h>
#include <linux/stddef.h>
#include <linux/of_gpio.h>
#include <linux/of_address.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/pm_wakeup.h>
#include <soc_gpio_interface.h>
#include <linux/syscore_ops.h>

#include "securec.h"
#include "ext_sensorhub_sdio.h"
#include "ext_sensorhub_frame.h"

#define COMMU_READ_DELAY_TIME_US	100
#define COMMU_READ_MAX_DELAY_TIMES	200
#define COMMU_WRITE_DELAY_TIME_US	30
#define COMMU_WRITE_MAX_DELAY_TIMES	700
#define COMMU_RETRY_INTERVAL_MS		1
#define COMMU_MAX_RETRY_TIMES		10
#define COMMU_ALIGN_BYTES_COUNT		4
#define AO_GPIO_GROUP_STEP		8
#define IO_BUFFER_LEN			40

struct commu_data {
	/* The mutex lock for communication operation */
	struct mutex commu_op_lock;
	unsigned int wake_irq;
	unsigned int init_irq;
	int wakeup_ap_gpio;
	int wakeup_mcu_gpio;
	int ap_read_ready_gpio;
	int mcu_read_ready_gpio;

	struct work_struct wakeup_read_work;
	struct work_struct init_work;
	struct wakeup_source wake_lock;
	bool initialed;

	struct semaphore sr_wake_sema;
};

static struct commu_data g_commu_data;
static void __iomem *g_ao_gpio_base;

static int commu_request_common_gpio(
	int *gpio, const char *compat, const char *name)
{
	struct device_node *np = NULL;

	np = of_find_compatible_node(NULL, NULL, compat);
	if (!np) {
		pr_err("gpio_request %s: %s node not found\n", __func__, *name);
		return -ENODEV;
	}

	*gpio = of_get_named_gpio(np, name, 0);
	if (*gpio < 0) {
		pr_err("gpio_request error %s:%d.\n", name, *gpio);
		return -ENODEV;
	}

	if (gpio_request(*gpio, name) < 0) {
		pr_err("gpio_request Failed to request gpio %d for %s\n",
		       *gpio, name);
		return -ENODEV;
	}

	return 0;
}

static int commu_request_direction_gpio(
	int *gpio, const char *compat, const char *name)
{
	int ret;

	ret = commu_request_common_gpio(gpio, compat, name);
	if (ret < 0)
		return ret;

	if (gpio_direction_input(*gpio) < 0) {
		pr_err("gpio_request Failed to set dir %d for %s\n",
		       *gpio, compat);
		return -ENODEV;
	}

	return ret;
}

static int commu_request_gpio(struct commu_data *commu_data)
{
	int ret;

	/* Get MCU Wakeup AP GPIO */
	ret = commu_request_direction_gpio(&commu_data->wakeup_ap_gpio,
					   "mcu,wakeupap", "mcu_wakeup_ap");
	if (ret < 0) {
		pr_err("%s: mcu,wakeupap node not found\n", __func__);
		return ret;
	}
	commu_data->wake_irq = gpio_to_irq(commu_data->wakeup_ap_gpio);
	pr_info("request_irq_gpio commu_data.wake_irq = %u",
		commu_data->wake_irq);

	/* Get AP Wakeup MCU Gpio */
	ret = commu_request_common_gpio(&commu_data->wakeup_mcu_gpio,
					"mcu,wakeupmcu", "ap_wakeup_mcu");
	if (ret < 0) {
		pr_err(" %s: mcu,wakeupmcu node not found\n", __func__);
		return ret;
	}
	pr_info("ap_wakeup_mcu gpio:%d done\n",
		commu_data->wakeup_mcu_gpio);

	/* Get AP Read Sync Gpio */
	ret = commu_request_common_gpio(&commu_data->ap_read_ready_gpio,
					"mcu,readsync", "ap_read_sync");
	if (ret < 0) {
		pr_err("%s: mcu,readsync node not found\n", __func__);
		return ret;
	}
	pr_info("ap_read_ready_gpio gpio:%d done\n",
		commu_data->ap_read_ready_gpio);

	/* Get AP Write Sync Gpio */
	ret = commu_request_direction_gpio(&commu_data->mcu_read_ready_gpio,
					   "mcu,writesync", "ap_write_sync");
	if (ret < 0) {
		pr_err("%s: mcu,writesync node not found\n", __func__);
		return ret;
	}
	pr_info("mcu_read_ready_gpio gpio:%d done\n",
		commu_data->mcu_read_ready_gpio);
	/* for sdio init irq */
	commu_data->init_irq = gpio_to_irq(commu_data->mcu_read_ready_gpio);
	pr_info("request_irq_gpio commu_data.init_irq = %u",
		commu_data->init_irq);

	return 0;
}

/* do after gpio init */
static int hisi_get_gpio_base(void)
{
	struct device_node *dn = NULL;
	unsigned int group;
	char *io_buffer = NULL;
	int err;
	int ret = 0;

	pr_info("%s wake up gpio: %d\n", __func__, g_commu_data.wakeup_ap_gpio);
	group = g_commu_data.wakeup_ap_gpio / AO_GPIO_GROUP_STEP;
	io_buffer = kzalloc(IO_BUFFER_LEN, GFP_KERNEL);
	if (!io_buffer)
		return -ENOMEM;
	memset_s(io_buffer, IO_BUFFER_LEN, 0, IO_BUFFER_LEN);
	err = snprintf_s(io_buffer, IO_BUFFER_LEN, IO_BUFFER_LEN - 1,
			 "arm,primecell%u", group);
	if (err < 0) {
		pr_err("[%s]snprintf_s fail[%d]\n", __func__, ret);
		ret = err;
		goto err_free;
	}

	dn = of_find_compatible_node(NULL, NULL, io_buffer);
	if (!dn) {
		pr_err("%s: hisilicon,primecell%d No compatible node found\n",
		       __func__, group);
		ret = -ENODEV;
		goto err_free;
	}
	g_ao_gpio_base = of_iomap(dn, 0);
	if (!g_ao_gpio_base) {
		ret = -EINVAL;
		of_node_put(dn);
		goto err_free;
	}
	of_node_put(dn);
	pr_info("%s get ao gpio base success\n", __func__);

err_free:
	kfree(io_buffer);
	return ret;
}

static bool check_resume_irq(void)
{
	unsigned int offset;
	bool ret = false;
	unsigned int data;

	offset = g_commu_data.wakeup_ap_gpio % AO_GPIO_GROUP_STEP;
	data = readl(g_ao_gpio_base + SOC_GPIO_GPIOIE_ADDR(0));
	data = data & readl(g_ao_gpio_base + SOC_GPIO_GPIOMIS_ADDR(0));
	ret = (data & BIT((u32)offset)) != 0;
	pr_info("%s pending result: %d\n", __func__, ret);
	return ret;
}

static void record_commu_wakeup(void)
{
	// do not wake by commu, do nothing
	if (!check_resume_irq())
		return;

	pr_info("will record for dubai");
	notify_commu_wakeup(true);
}

static void commu_release_gpio(struct commu_data *commu_data)
{
	gpio_free(commu_data->wakeup_ap_gpio);
	gpio_free(commu_data->wakeup_mcu_gpio);
	gpio_free(commu_data->ap_read_ready_gpio);
	gpio_free(commu_data->mcu_read_ready_gpio);
}

static irqreturn_t commu_wake_irq(int irq, void *arg)
{
	struct commu_data *commu_data = (struct commu_data *)arg;

	schedule_work(&commu_data->wakeup_read_work);
	return IRQ_HANDLED;
}

static irqreturn_t commu_init_irq(int irq, void *arg)
{
	struct commu_data *commu_data = (struct commu_data *)arg;

	/* check mcu wakeup ap is high, mcu reboot, clear init flag */
	if (gpio_get_value(commu_data->mcu_read_ready_gpio) == 1 &&
	    gpio_get_value(commu_data->wakeup_ap_gpio) == 1) {
		pr_err("%s mcu is rebooting, clear init flag", __func__);
		commu_data->initialed = false;
		notify_mcu_reboot();
		return IRQ_HANDLED;
	}
	schedule_work(&commu_data->init_work);
	return IRQ_HANDLED;
}

static int commu_request_irq(struct commu_data *commu_data)
{
	int retval;

	if (!commu_data) {
		pr_err("%s commu_data is null\n", __func__);
		return -EINVAL;
	}
	retval = request_irq(commu_data->wake_irq, commu_wake_irq,
			     IRQF_TRIGGER_RISING | IRQF_NO_SUSPEND,
			     "commu wake irq", commu_data);
	if (retval < 0) {
		pr_err("couldn't acquire MCU HOST WAKE UP IRQ reval = %d\n",
		       retval);
		return retval;
	}

	retval = request_irq(commu_data->init_irq, commu_init_irq,
			     IRQF_TRIGGER_RISING | IRQF_NO_SUSPEND,
			     "commu wake irq", commu_data);
	if (retval < 0) {
		pr_err("couldn't acquire MCU HOST INIT IRQ reval = %d\n",
		       retval);
		return retval;
	}

	return 0;
}

static void commu_wakeup_read_work(struct work_struct *work)
{
	struct commu_data *commu_data = container_of(work, struct commu_data,
		wakeup_read_work);
	unsigned int delay_times = 0;
	int ret;
	unsigned char *read_buf = NULL;
	u32 xfercount = 0;

	if (!commu_data) {
		pr_err("%s commu data NULL\n", __func__);
		return;
	}

	ret = down_timeout(&commu_data->sr_wake_sema, 6 * HZ);
	if (ret == -ETIME) {
		pr_err("%s down sr wake sema timeout", __func__);
		return;
	}
	up(&commu_data->sr_wake_sema);

	/* check mcu rx ready is high, mcu reboot */
	if (gpio_get_value(commu_data->mcu_read_ready_gpio) == 1 &&
	    gpio_get_value(commu_data->wakeup_ap_gpio) == 1) {
		pr_err("%s wakeup_ap_gpio is high, mcu rebooting", __func__);
		return;
	}

	mutex_lock(&commu_data->commu_op_lock);
	if (!commu_data->initialed)
		goto err;
	__pm_stay_awake(&commu_data->wake_lock);
	gpio_direction_output(commu_data->ap_read_ready_gpio, 1);
	/* wait for */
	while (gpio_get_value(commu_data->wakeup_ap_gpio) == 1) {
		if (!commu_data->initialed)
			goto err;
		if (delay_times++ > COMMU_READ_MAX_DELAY_TIMES) {
			pr_err("%s delay times is out of range\n",
			       __func__);
			goto err;
		}
		udelay(COMMU_READ_DELAY_TIME_US);
	}
	ret = ext_sensorhub_mmc_sdio_get_xfercount(&xfercount);
	if (ret < 0 || xfercount == 0) {
		pr_err("%s get xfer count:%d, error %d\n",
		       __func__, xfercount, ret);
		goto err;
	}
	read_buf = kmalloc(xfercount, GFP_KERNEL);
	if (!read_buf)
		goto err;
	/* Execute Read Operation, sdio initial already */
	ret = ext_sensorhub_mmc_sdio_dev_read(read_buf, xfercount);
	if (ret < 0) {
		pr_err("%s read data from sdio error, ret:%d\n", __func__, ret);
		goto err;
	}
	ret = decode_frame_resp(read_buf, xfercount);
	if (ret < 0)
		pr_err("%s decode resp error: %d", __func__, ret);
err:
	gpio_direction_output(commu_data->ap_read_ready_gpio, 0);
	mutex_unlock(&commu_data->commu_op_lock);
	kfree(read_buf);
	__pm_relax(&commu_data->wake_lock);
	read_buf = NULL;
}

static void commu_init_work(struct work_struct *work)
{
	int ret;

	pr_info("%s begin to do communication init work check\n", __func__);
	struct commu_data *commu_data = container_of(work, struct commu_data,
		init_work);
	if (!commu_data) {
		pr_err("%s commu data NULL\n", __func__);
		return;
	}

	/* ap is writing, not a initial work */
	if (commu_data->initialed ||
	    gpio_get_value(commu_data->wakeup_mcu_gpio) == 1)
		return;

	pr_info("%s is initial work detect sdio card\n", __func__);
	__pm_stay_awake(&commu_data->wake_lock);
	ret = down_timeout(&commu_data->sr_wake_sema, 12 * HZ);
	if (ret == -ETIME) {
		pr_err("%s down sr wake sema timeout", __func__);
		__pm_relax(&commu_data->wake_lock);
		return;
	}
	up(&commu_data->sr_wake_sema);
	ext_sensorhub_detect_sdio_card();
	commu_data->initialed = true;
	__pm_relax(&commu_data->wake_lock);
}

void commu_suspend(void)
{
	if (down_interruptible(&g_commu_data.sr_wake_sema))
		pr_err("%s down sr wake sema error", __func__);
	notify_commu_wakeup(false);
}

void commu_resume(void)
{
	up(&g_commu_data.sr_wake_sema);
}

static struct syscore_ops dubai_pm_syscore_ops = {
	.resume = record_commu_wakeup,
};

int commu_probe_init(void)
{
	int status;
	struct commu_data *commu_data = &g_commu_data;

	sema_init(&commu_data->sr_wake_sema, 1);
	commu_data->initialed = false;
	pr_info("%s begin ext_commu_probe_init\n", __func__);
	mutex_init(&commu_data->commu_op_lock);

	status = commu_request_gpio(commu_data);
	if (status < 0)
		pr_err("%s request gpio error, status: %d", __func__, status);

	INIT_WORK(&commu_data->init_work, commu_init_work);
	INIT_WORK(&commu_data->wakeup_read_work, commu_wakeup_read_work);
	status = commu_request_irq(commu_data);
	if (status < 0)
		pr_err("%s request irq error, status: %d", __func__, status);

	wakeup_source_init(&commu_data->wake_lock, "mcu_commu");
	status = hisi_get_gpio_base();
	if (status < 0)
		return -EFAULT;
	/* SDIO probe */
	status = ext_sensorhub_sdio_func_probe();
	if (status < 0)
		pr_err("%s sdio probe error, status: %d", __func__, status);
	register_syscore_ops(&dubai_pm_syscore_ops);
	pr_info("%s end , status:%d\n", __func__, status);

	return status;
}

void commu_exit(void)
{
	struct commu_data *commu_data = &g_commu_data;
	/* sido exit */
	ext_sensorhub_sdio_exit();

	/* stop qpio irq */
	free_irq(commu_data->wake_irq, commu_data);

	if (commu_data->initialed)
		/* stop read work */
		cancel_work_sync(&commu_data->wakeup_read_work);

	/* stop init work */
	cancel_work_sync(&commu_data->init_work);

	/* gpio release */
	commu_release_gpio(commu_data);
}

static size_t calc_commu_write_len(size_t len)
{
	size_t send_len;
	size_t align_factor = COMMU_ALIGN_BYTES_COUNT;

	// 512 bytes
	if (len >= HISDIO_BLOCK_SIZE)
		align_factor = HISDIO_BLOCK_SIZE;

	if (len % align_factor == 0)
		send_len = len;
	else
		send_len = (len / align_factor + 1) * align_factor;

	return send_len;
}

ssize_t commu_write(const unsigned char *buf, size_t len)
{
	struct commu_data *commu_data;
	ssize_t	status;
	unsigned int write_retry_times = 0;
	unsigned int delay_times = 0;
	size_t send_len;

	/* send len may > data len,but must < FRAME_BUF_LEN(buf contains '0') */
	send_len = calc_commu_write_len(len);
	commu_data = &g_commu_data;
writeretry:
	mutex_lock(&commu_data->commu_op_lock);
	if (!commu_data->initialed) {
		status = -EREMOTEIO;
		goto err;
	}

	if (gpio_get_value(commu_data->mcu_read_ready_gpio) == 1) {
		write_retry_times++;
		mutex_unlock(&commu_data->commu_op_lock);
		msleep(COMMU_RETRY_INTERVAL_MS);
		if (write_retry_times <= COMMU_MAX_RETRY_TIMES) {
			goto writeretry;
		} else {
			pr_err("%s last package is writing\n",
			       __func__);
			return -ETIME;
		}
	}
	write_retry_times = 0;
	gpio_direction_output(commu_data->wakeup_mcu_gpio, 1);
	if (gpio_get_value(commu_data->wakeup_ap_gpio) == 1) {
		gpio_direction_output(commu_data->wakeup_mcu_gpio, 0);
		mutex_unlock(&commu_data->commu_op_lock);
		msleep(COMMU_RETRY_INTERVAL_MS);
		pr_err("communication bus is in compate mode\n");
		write_retry_times++;
		if (write_retry_times <= COMMU_MAX_RETRY_TIMES) {
			goto writeretry;
		} else {
			pr_err("%s bus compete times is out of range\n",
			       __func__);
			return -ETIME;
		}
	}

	while (gpio_get_value(commu_data->mcu_read_ready_gpio) != 1) {
		if (!commu_data->initialed) {
			status = -EREMOTEIO;
			goto err;
		}
		delay_times++;
		if (delay_times > COMMU_WRITE_MAX_DELAY_TIMES) {
			status = -ETIME;
			pr_err("%s delay times is out of range\n", __func__);
			goto err;
		}
		udelay(COMMU_WRITE_DELAY_TIME_US);
	}

	/* write to sdio */
	status = ext_sensorhub_mmc_sdio_dev_write(buf, send_len);

err:
	gpio_direction_output(commu_data->wakeup_mcu_gpio, 0);
	mutex_unlock(&commu_data->commu_op_lock);

	return status;
}
