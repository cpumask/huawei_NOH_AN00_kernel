/*
 * hisi-usb-internal.c
 *
 * utilityies for hifi-usb
 *
 * Copyright (c) 2019-2019 Huawei Technologies Co., Ltd.
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

#include "hifi-usb-internal.h"

#include <linux/kernel.h>
#include <linux/usb.h>

#include "proxy-hcd.h"

#define DBG(format, arg...) pr_debug("[phcd][DBG][%s]" format, __func__, ##arg)
#define INFO(format, arg...) pr_info("[phcd][INFO][%s]" format, __func__, ##arg)
#define ERR(format, arg...) pr_err("[phcd][ERR][%s]" format, __func__, ##arg)
static bool is_huawei_c_earphone(struct usb_device *udev)
{
	if (udev->parent == NULL) {
		WARN_ON(1);
		return false;
	}
	if (udev->parent->parent != NULL) {
		WARN_ON(1);
		return false;
	}

	if ((le16_to_cpu(udev->descriptor.idVendor) != HUAWEI_EARPHONE_VENDOR_ID)
			|| (le16_to_cpu(udev->descriptor.idProduct) != HUAWEI_EARPHONE_PRODUCT_ID))
		return false;

	return true;
}

void hifi_usb_wait_for_free_dev(struct hifi_usb_proxy *proxy)
{
	long ret;
	struct proxy_hcd *phcd = NULL;

	if (proxy == NULL)
		return;

	phcd = client_to_phcd(proxy->client);
	if (phcd == NULL || phcd->phcd_udev.udev == NULL)
		return;

	if (!is_huawei_c_earphone(phcd->phcd_udev.udev)) {
		INFO("it is not hw earphone, do not wait\n");
		return;
	}

	/*
	 * Make USB Core to unloading the usb_device
	 * HiFi USB Controller support only one port
	 * Port bitmap bit 1 means port 1
	 */
	proxy_port_disconnect(proxy->client, 1 << 1);

	ret = wait_event_timeout(proxy->free_dev_wait_queue,
			(proxy->slot_id == -1), HZ);
	if (ret == 0) {
		INFO("wait for free_dev timeout\n");
		proxy->slot_id = -1;
	}
}

void hifi_usb_port_disconnect(struct hifi_usb_proxy *proxy)
{
	struct client_ref *client_ref = NULL;

	if (proxy == NULL || proxy->client == NULL)
		return;

	client_ref = &proxy->client->client_ref;

	init_completion(&client_ref->kill_completion);
	client_ref_kill(client_ref);

	/*
	 * Make USB Core to unloading the usb_device
	 * HiFi USB Controller support only one port
	 * Port bitmap bit 1 means port 1
	 */
	proxy_port_disconnect(proxy->client, 1 << 1);

	if (wait_for_completion_timeout(&client_ref->kill_completion,
					CLIENT_REF_KILL_SYNC_TIMEOUT) == 0) {
		pr_err("[%s]wait for client killed timeout\n", __func__);
		WARN_ON(1);
	} else {
		WARN_ON(!client_ref_is_zero(client_ref));
	}
}
