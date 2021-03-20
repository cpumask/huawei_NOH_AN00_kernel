/*
 * gpio key driver
 *
 * Copyright (C) Huawei Technologies Co., Ltd. 2016-2019. All rights reserved.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include "hisi_gpio_key.h"

#define INT_STR_LEN 8
#define MAX_BUF_LEN 64
#define WRITE_BUF_LEN 8

struct hisi_key_common_data g_key_common_data;

static int of_get_key_gpio(struct device_node *np, const char *propname,
	int prop_index, int gpio_index, enum of_gpio_flags *flags)
{
	int ret;

#ifdef CONFIG_GPIO_LOGIC
	ret = of_get_gpio_by_prop(np, propname, prop_index, gpio_index, flags);
#else
	ret = of_get_named_gpio(np, propname, prop_index);
#endif

	return ret;
}

static int hisi_gpio_key_open(struct input_dev *dev)
{
	return 0;
}

static void hisi_gpio_key_close(struct input_dev *dev)
{
}

#ifdef CONFIG_KEY_MASK
static ssize_t key_mask_read(
	struct file *file, char __user *user_buf, size_t count, loff_t *ppos)
{
	char buf[INT_STR_LEN] = {0};
	int ret;

	if (!user_buf) {
		pr_err("[gpiokey]user_buf is NULL!\n");
		return -EINVAL;
	}

	ret = snprintf_s(
		buf, sizeof(buf), sizeof(buf) - 1, "%d",
			g_key_common_data.key_mask_flag);
	if (ret < 0) {
		pr_err("[%s]snprintf_s failed. ret=%d key_mask read:%d\n",
			__func__, ret, g_key_common_data.key_mask_flag);
		return ret;
	}

	pr_info("[%s]key_mask read:%d\n", __func__,
		g_key_common_data.key_mask_flag);

	return simple_read_from_buffer(user_buf, count, ppos, buf, strlen(buf));
}

static ssize_t key_mask_write(struct file *file, const char __user *user_buf,
	size_t count, loff_t *ppos)
{
	char buff[MAX_BUF_LEN] = {0};
	int mask = 0;
	size_t size;

	size = min_t(size_t, count, WRITE_BUF_LEN);

	if (!user_buf) {
		pr_err("[gpiokey]user_buf is NULL!\n");
		return -EINVAL;
	}

	if (copy_from_user(buff, user_buf, size)) {
		pr_err("[gpiokey]copy from user fail!\n");
		return 0;
	}

	if (sscanf_s(buff, "%d", &mask) != 1) {
		pr_err("[gpiokey]sscanf_s fail!\n");
		return 0;
	}

	if ((mask == KEY_ENABLE_MASK) ||
		(mask == KEY_DISABLE_MASK))
		g_key_common_data.key_mask_ctl = mask;

	pr_info("[%s]key_mask write:%d,%d\n", __func__, mask,
		g_key_common_data.key_mask_ctl);

	return count;
}

static const struct file_operations key_mask_fops = {
	.read = key_mask_read,
	.write = key_mask_write,
};
#endif

static int hisi_gpio_key_new_input_dev(struct hisi_key_devices *key_dev)
{
	int ret = 0;

	key_dev->input_dev = input_allocate_device();
	if (!key_dev->input_dev) {
		dev_err(&key_dev->pdev->dev, "%s Failed to allocate struct input_dev!\n",
			key_dev->devices_names);
		return -ENOMEM;
	}

	key_dev->input_dev->name = key_dev->devices_names;
	key_dev->input_dev->id.bustype = BUS_HOST;
	key_dev->input_dev->dev.parent = &key_dev->pdev->dev;

	set_bit(EV_KEY, key_dev->input_dev->evbit);
	set_bit(EV_SYN, key_dev->input_dev->evbit);

	key_dev->key_set_bit(key_dev);

	key_dev->input_dev->open = hisi_gpio_key_open;
	key_dev->input_dev->close = hisi_gpio_key_close;

	input_set_drvdata(key_dev->input_dev, key_dev);

	ret = input_register_device(key_dev->input_dev);
	if (ret) {
		dev_err(&key_dev->pdev->dev, "%s Failed to register input device!\n",
			key_dev->devices_names);
		goto err;
	}

	device_init_wakeup(&key_dev->pdev->dev, TRUE);

	return 0;

err:
	input_free_device(key_dev->input_dev);
	return ret;
}

static int hisi_gpio_key_init_work(struct hisi_key_devices *hisi_key_dev)
{
	/* initial work before we use it. */
	INIT_DELAYED_WORK(&(hisi_key_dev->gpio_key_work),
		hisi_key_dev->key_work);

	wakeup_source_init(&(hisi_key_dev->gpio_key_lock),
		hisi_key_dev->work_init_name);

	return 0;
}

static int hisi_gpio_init_pin(struct hisi_key_devices *hisi_key_dev)
{
	enum of_gpio_flags flags;

	hisi_key_dev->hisi_key_num = of_get_key_gpio(
		hisi_key_dev->pdev->dev.of_node, hisi_key_dev->key_node_name,
			0, 0, &flags);
	if (!gpio_is_valid(hisi_key_dev->hisi_key_num)) {
		dev_err(&hisi_key_dev->pdev->dev, "gpio of %s isn't valid, chk DTS\n",
			hisi_key_dev->devices_names);
		return -EINVAL;
	}

	return 0;
}

static int hisi_gpio_key_gpio_to_irq(struct hisi_key_devices *hisi_key_dev)
{
	int err;

	if (!gpio_is_valid(hisi_key_dev->hisi_key_num)) {
		dev_err(&hisi_key_dev->pdev->dev, "gpio:%u is invalid\n",
			hisi_key_dev->hisi_key_num);
		return -EINVAL;
	}

	err = devm_gpio_request(&hisi_key_dev->pdev->dev,
		hisi_key_dev->hisi_key_num, hisi_key_dev->gpio_to_irq_name);
	if (err < 0) {
		dev_err(&hisi_key_dev->pdev->dev, "err %d  Fail request gpio:%u---gpio_to_irq_name:%s\n",
			err, hisi_key_dev->hisi_key_num,
				hisi_key_dev->gpio_to_irq_name);
		return err;
	}

	err = gpio_direction_input(hisi_key_dev->hisi_key_num);
	if (err < 0) {
		dev_err(&hisi_key_dev->pdev->dev, "Set %s direction failed!\n",
			hisi_key_dev->gpio_to_irq_name);
		return err;
	}

	hisi_key_dev->hisi_key_irq = gpio_to_irq(hisi_key_dev->hisi_key_num);
	if (hisi_key_dev->hisi_key_irq < 0) {
		dev_err(&hisi_key_dev->pdev->dev,
			"Get gpio key press irq fail, %s:%u!\n",
				hisi_key_dev->gpio_to_irq_name,
					hisi_key_dev->hisi_key_num);
		return -EINVAL;
	}

	return 0;
}

static int hisi_gpio_req_irq(struct hisi_key_devices *hisi_key_dev,
	struct hisi_key_common_data *key_common_data)
{
	int err;
	u32 *irq_arr = key_common_data->irq_arr;
	int irq_arr_size = ARRAY_SIZE(key_common_data->irq_arr);

	if (!gpio_is_valid(hisi_key_dev->hisi_key_num))
		return -EINVAL;

	if (key_common_data->irq_cnt >= irq_arr_size) {
		dev_err(&hisi_key_dev->pdev->dev, "irq_cnt is out of bounds, irq:%d key:%d\n",
			hisi_key_dev->hisi_key_irq, hisi_key_dev->hisi_key_num);
		return -EINVAL;
	}

	err = hisi_key_dev->hisi_key_req_irq(hisi_key_dev);
	if (err)
		return err;

	irq_arr[key_common_data->irq_cnt] = hisi_key_dev->hisi_key_irq;
	key_common_data->irq_cnt++;

	return 0;
}

static void hisi_gpio_free_irqs(struct hisi_key_devices *hisi_key_dev,
	struct hisi_key_common_data *key_common_data)
{
	int i, cnt;

	cnt = min_t(int, MAX_KEY_IRQ_NUM, key_common_data->irq_cnt);
	for (i = 0; i < cnt; i++)
		free_irq(key_common_data->irq_arr[i], hisi_key_dev);
}

static int hisi_gpio_key_create_node(void)
{
	/* 0660 is S_IRGRP | S_IWGRP | S_IRUSR | S_IWUSR */
	pr_err("%s enter\n", __func__);
	if (!proc_create("key_mask", 0660, NULL, &key_mask_fops))
		pr_err("[%s]:failed to create key_mask proc\n", __func__);

	return 0;
}

struct hisi_key_common_data *hisi_get_common_data(void)
{
	return &g_key_common_data;
}

int hisi_gpio_key_get_of_node(const struct of_device_id *hs_gpio_key_match,
	struct platform_device *pdev)
{
	if (!of_match_node(hs_gpio_key_match, pdev->dev.of_node)) {
		dev_err(&pdev->dev, "dev node is not match. exiting\n");
		return -ENODEV;
	}

	return 0;
}

int hisi_key_register_dev(struct hisi_key_devices *hisi_key_dev)
{
	int ret = 0;

	g_key_common_data.key_mask_ctl = KEY_ENABLE_MASK;
	g_key_common_data.key_mask_flag = KEY_ENABLE_MASK;

	ret = hisi_gpio_init_pin(hisi_key_dev);
	if (ret) {
		dev_err(&hisi_key_dev->pdev->dev, "%s init_pin fail\n",
			hisi_key_dev->devices_names);
		return -EINVAL;
	}

	ret = hisi_gpio_key_new_input_dev(hisi_key_dev);
	if (ret) {
		dev_err(&hisi_key_dev->pdev->dev, "%s new_input_dev fail\n",
			hisi_key_dev->devices_names);
		return -EINVAL;
	}

	hisi_gpio_key_init_work(hisi_key_dev);

	ret = hisi_gpio_key_gpio_to_irq(hisi_key_dev);
	if (ret) {
		dev_err(&hisi_key_dev->pdev->dev, "%s key_gpio_to_irq fail\n",
			hisi_key_dev->devices_names);
		goto req_irq_err;
	}

	ret = hisi_gpio_req_irq(hisi_key_dev, &g_key_common_data);
	if (ret) {
		dev_err(&hisi_key_dev->pdev->dev, "%s hisi_gpio_req_irq fail\n",
			hisi_key_dev->devices_names);
		goto req_irq_err;
	}

#ifdef CONFIG_KEY_MASK
	ret = hisi_gpio_key_create_node();
	if (ret) {
		dev_err(&hisi_key_dev->pdev->dev, "%s hisi_gpio_key_init_last_step fail\n",
			hisi_key_dev->devices_names);
		goto create_node_err;
	}
#endif

	return 0;

#ifdef CONFIG_KEY_MASK
create_node_err:
	hisi_gpio_free_irqs(hisi_key_dev, &g_key_common_data);
#endif
req_irq_err:
	wakeup_source_trash(&(hisi_key_dev->gpio_key_lock));
	input_unregister_device(hisi_key_dev->input_dev);
	return -ENODEV;
}

int hisi_key_unregister_dev(struct hisi_key_devices *hisi_key_dev)
{
	int ret;

	if (!hisi_key_dev) {
		pr_err("get invalid hisi_key_dev pointer\n");
		return -ENODEV;
	}

	free_irq(hisi_key_dev->hisi_key_irq, hisi_key_dev);
	gpio_free((unsigned int)hisi_key_dev->hisi_key_num);

	hisi_gpio_free_irqs(hisi_key_dev, &g_key_common_data);

	ret = cancel_delayed_work(&(hisi_key_dev->gpio_key_work));
	if (!ret)
		pr_err("%s:cancel gpio key up work failed\n", __func__);

	wakeup_source_trash(&(hisi_key_dev->gpio_key_lock));

	input_unregister_device(hisi_key_dev->input_dev);
#ifdef CONFIG_KEY_MASK
	if (g_key_common_data.irq_cnt == 0)
		remove_proc_entry("key_mask", NULL);
#endif
	return 0;
}
