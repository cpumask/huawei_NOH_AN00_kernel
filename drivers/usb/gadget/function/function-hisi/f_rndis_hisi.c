/*
 * f_rndis_hisi.c
 *
 * rndis function support for configfs
 *
 * Copyright (c) 2018-2019 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * either version 2 of that License or (at your option) any later version.
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include <linux/hisi/usb/android_gadget.h>
#include "u_ether.h"

#define HOST_ADD_LEN 18
#define FILE_MODE 0644
#define MANUFEATURER_LEN 256

static u8 host_ethaddr_record[ETH_ALEN];
static char manufacturer[MANUFEATURER_LEN];
static bool wceis;
static u32 vendor_id;

static struct device *g_rndis_dev;

static ssize_t rndis_manufacturer_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%s\n", manufacturer);
}

static ssize_t rndis_manufacturer_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	int len;

	len = min(size, sizeof(manufacturer) - 1);

	strncpy(manufacturer, buf, len);

	manufacturer[len] = 0;

	return size;
}

static DEVICE_ATTR(manufacturer, FILE_MODE, rndis_manufacturer_show,
					rndis_manufacturer_store);

static ssize_t rndis_wceis_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%d\n", wceis);
}

static ssize_t rndis_wceis_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	u32 value = 0;
	int ret;

	ret = kstrtou32(buf, 0, &value);
	if (ret)
		return ret;

	wceis = !!value;
	return size;
}

static DEVICE_ATTR(wceis, FILE_MODE, rndis_wceis_show,
					rndis_wceis_store);

static ssize_t rndis_ethaddr_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%02x:%02x:%02x:%02x:%02x:%02x\n",
			host_ethaddr_record[0], host_ethaddr_record[1],
			host_ethaddr_record[2], host_ethaddr_record[3],
			host_ethaddr_record[4], host_ethaddr_record[5]);
}

static ssize_t rndis_ethaddr_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	int set_etheraddr[ETH_ALEN];
	int i;

	/* ETH_ALEN is 6 */
	if (sscanf(buf, "%02x:%02x:%02x:%02x:%02x:%02x\n",
		&set_etheraddr[0], &set_etheraddr[1],
		&set_etheraddr[2], &set_etheraddr[3],
		&set_etheraddr[4], &set_etheraddr[5]) == 6) {
		for (i = 0; i < ETH_ALEN; i++)
			host_ethaddr_record[i] = (u8)set_etheraddr[i];

		return size;
	}

	return -EINVAL;
}

static DEVICE_ATTR(ethaddr, FILE_MODE, rndis_ethaddr_show,
					       rndis_ethaddr_store);

static ssize_t rndis_vendorID_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%04x\n", vendor_id);
}

static ssize_t rndis_vendorID_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	unsigned int value = 0;

	if (kstrtou32(buf, 0, &value)) {
		pr_err("[%s]read vendorID from input failed\n", __func__);
		return -EINVAL;
	}

	vendor_id = value;
	return size;
}

static DEVICE_ATTR(vendorID, FILE_MODE, rndis_vendorID_show,
						rndis_vendorID_store);
static struct device_attribute *rndis_function_attributes[] = {
	&dev_attr_manufacturer,
	&dev_attr_wceis,
	&dev_attr_ethaddr,
	&dev_attr_vendorID,
	NULL
};

int create_rndis_device(void)
{
	struct device *dev = NULL;
	struct device_attribute **attrs = NULL;
	struct device_attribute *attr = NULL;
	int err = 0;

	dev = create_function_device("f_rndis");
	if (IS_ERR(dev))
		return PTR_ERR(dev);

	attrs = rndis_function_attributes;
	if (attrs != NULL) {
		attr = *attrs;
		while (attr != NULL && !err) {
			err = device_create_file(dev, attr);
			*attrs++;
			attr = *attrs;
		}
		if (err) {
			device_destroy(dev->class, dev->devt);
			return -EINVAL;
		}
	}

	g_rndis_dev = dev;

	return 0;
}

void destroy_rndis_device(void)
{
	if (g_rndis_dev != NULL) {
		device_destroy(g_rndis_dev->class, g_rndis_dev->devt);
		g_rndis_dev = NULL;
	}
}

static int get_ether_addr_str(u8 dev_addr[ETH_ALEN], char *str, int len)
{
	if (len < HOST_ADD_LEN)
		return -EINVAL;

	snprintf(str, len, "%pM", dev_addr);
	return HOST_ADD_LEN;
}

static void hisi_rndis_set_host_ethaddr(struct f_rndis_opts *opts)
{
	char addr[HOST_ADD_LEN];

	if (opts == NULL)
		return;

	/* using saved host ethernet address */
	if (get_ether_addr_str(host_ethaddr_record, addr, HOST_ADD_LEN) > 0) {
		if (!gether_set_host_addr(opts->net, addr))
			pr_info("[RNDIS]using random host ethernet address\n");
	}

	gether_get_host_addr_u8(opts->net, host_ethaddr_record);
}

static void hisi_rndis_nop_release(struct device *dev)
{
	dev_vdbg(dev, "%s\n", __func__);
}

static void hisi_rndis_opts_set(struct f_rndis_opts *opts)
{
	if (wceis) {
		/* "Wireless" RNDIS; auto-detected by Windows */
		rndis_iad_descriptor.bFunctionClass =
						USB_CLASS_WIRELESS_CONTROLLER;
		rndis_iad_descriptor.bFunctionSubClass = 0x01;
		rndis_iad_descriptor.bFunctionProtocol = 0x03;
		rndis_control_intf.bInterfaceClass =
						USB_CLASS_WIRELESS_CONTROLLER;
		rndis_control_intf.bInterfaceSubClass =  0x01;
		rndis_control_intf.bInterfaceProtocol =  0x03;
	}

	opts->class = rndis_iad_descriptor.bFunctionClass;
	opts->subclass = rndis_iad_descriptor.bFunctionSubClass;
	opts->protocol = rndis_iad_descriptor.bFunctionProtocol;
}

static struct usb_function_instance *rndis_alloc_inst(void)
{
	struct f_rndis_opts *opts = NULL;
	struct usb_os_desc *descs[1] = {0};
	char *names[1] = {0};
	struct config_group *rndis_interf_group = NULL;
	int ret;

	pr_info("%s:in\n", __func__);
	opts = kzalloc(sizeof(*opts), GFP_KERNEL);
	if (opts == NULL)
		return ERR_PTR(-ENOMEM);
	opts->rndis_os_desc.ext_compat_id = opts->rndis_ext_compat_id;

	mutex_init(&opts->lock);
	opts->func_inst.free_func_inst = rndis_free_inst;
	opts->manufacturer = manufacturer;
	opts->vendor_id = vendor_id;

	opts->net = gether_setup_name_default("rndis");
	if (IS_ERR(opts->net)) {
		struct net_device *net = opts->net;

		kfree(opts);
		return ERR_CAST(net);
	}

	hisi_rndis_set_host_ethaddr(opts);
	INIT_LIST_HEAD(&opts->rndis_os_desc.ext_prop);
	hisi_rndis_opts_set(opts);

	descs[0] = &opts->rndis_os_desc;
	names[0] = "rndis";
	config_group_init_type_name(&opts->func_inst.group, "", &rndis_func_type);
	rndis_interf_group =
		usb_os_desc_prepare_interf_dir(&opts->func_inst.group, 1, descs,
			names, THIS_MODULE);
	if (IS_ERR(rndis_interf_group)) {
		free_netdev(opts->net);
		kfree(opts);
		return ERR_CAST(rndis_interf_group);
	}

	opts->rndis_interf_group = rndis_interf_group;

	dev_set_name(&opts->dev, "rndis_opts");
	opts->dev.release = hisi_rndis_nop_release;
	ret = device_register(&opts->dev);
	if (ret) {
		free_netdev(opts->net);
		kfree(opts->rndis_interf_group);
		kfree(opts);
		return ERR_PTR(ret);
	}

	pr_info("%s:out\n", __func__);
	return &opts->func_inst;
}
