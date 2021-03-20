/*
 * Copyright (c) 2019 Huawei Technologies Co., Ltd.
 *
 * Copyright (C) 2016 Richtek Technology Corp.
 * Author: TH <tsunghan_tsai@richtek.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#define LOG_TAG "[PD][tcpm.c]"

#include <linux/hisi/usb/hisi_tcpm.h>

#include <securec.h>

#include "include/tcpci.h"
#include "include/tcpci_typec.h"

#ifdef CONFIG_USB_POWER_DELIVERY_SUPPORT
#include "include/pd_core.h"
#include "include/pd_policy_engine.h"
#include "include/pd_dpm_core.h"
#endif

int hisi_tcpm_shutdown(struct tcpc_device *tcpc_dev)
{
	if (tcpc_dev->ops->deinit)
		tcpc_dev->ops->deinit(tcpc_dev);

	return 0;
}

int hisi_tcpm_inquire_remote_cc(struct tcpc_device *tcpc_dev,
		uint8_t *cc1, uint8_t *cc2, bool from_ic)
{
	int rv, t_cc1, t_cc2;

	if (from_ic) {
		if (tcpc_dev->ops && tcpc_dev->ops->get_cc) {
			rv = tcpc_dev->ops->get_cc(tcpc_dev, &t_cc1, &t_cc2);
			if (rv == 0) {
				*cc1 = (uint8_t)(uint32_t)t_cc1;
				*cc2 = (uint8_t)(uint32_t)t_cc2;
				return 0;
			}
		}
	}

	*cc1 = tcpc_dev->typec_remote_cc[0];
	*cc2 = tcpc_dev->typec_remote_cc[1];

	return 0;
}

void hisi_tcpm_force_cc_mode(struct tcpc_device *tcpc_dev, int mode)
{
	int pull = mode ? TYPEC_CC_DRP : TYPEC_CC_RD;

	I("-- mode:%d\n", mode);

	if ((pull == TYPEC_CC_DRP) &&
		(tcpci_ccdebouce_timer_enabled(tcpc_dev))) {
		I("CC Det, Bypass force DRP\n");
		return;
	}

	tcpci_set_cc(tcpc_dev, pull);

	if (pull == TYPEC_CC_RD)
		tcpc_dev->typec_role = TYPEC_ROLE_SNK;
	else
		tcpc_dev->typec_role = TYPEC_ROLE_TRY_SNK;
}

int hisi_tcpm_typec_set_wake_lock(struct tcpc_device *tcpc, bool wake_lock)
{
	if (wake_lock) {
		D("lock attach_wake_lock\n");
		__pm_stay_awake(&tcpc->attach_wake_lock);
	} else {
		D("unlock attach_wake_lock\n");
		__pm_relax(&tcpc->attach_wake_lock);
	}

	return 0;
}

int hisi_tcpm_typec_set_rp_level(struct tcpc_device *tcpc_dev, uint8_t level)
{
	uint8_t res;

	D("%u\n", level);
	if (level == 2)
		res = TYPEC_CC_RP_3_0;
	else if (level == 1)
		res = TYPEC_CC_RP_1_5;
	else
		res = TYPEC_CC_RP_DFT;

	return hisi_tcpc_typec_set_rp_level(tcpc_dev, res);
}

/*
 * Force role swap, include power and data role, I guess!
 */
int hisi_tcpm_typec_role_swap(struct tcpc_device *tcpc_dev)
{
	D("typec_attach_old %u\n", tcpc_dev->typec_attach_old);

	if (tcpc_dev->typec_attach_old == TYPEC_UNATTACHED)
		return TCPM_ERROR_UNATTACHED;

#ifdef CONFIG_TYPEC_CAP_ROLE_SWAP
	if (hisi_tcpc_typec_swap_role(tcpc_dev) == 0)
		return TCPM_SUCCESS;
#endif

	return TCPM_ERROR_UNSUPPORT;
}

/*
 * Vendor defined case!
 */
int hisi_tcpm_typec_notify_direct_charge(struct tcpc_device *tcpc_dev,
		bool direct_charge)
{
	if (tcpc_dev->typec_attach_old == TYPEC_UNATTACHED)
		return TCPM_ERROR_UNATTACHED;

	return hisi_tcpc_typec_set_direct_charge(tcpc_dev, direct_charge);
}

int hisi_tcpm_typec_change_role(struct tcpc_device *tcpc_dev,
		uint8_t typec_role)
{
	return hisi_tcpc_typec_change_role(tcpc_dev, typec_role);
}

/* Request TCPC to send PD Request */
#ifdef CONFIG_USB_POWER_DELIVERY_SUPPORT
int hisi_tcpm_power_role_swap(struct tcpc_device *tcpc_dev)
{
	bool ret = false;
	pd_port_t *pd_port = &tcpc_dev->pd_port;

	if (tcpc_dev->typec_attach_old == TYPEC_UNATTACHED) {
		D("TYPEC_UNATTACHED\n");
		return TCPM_ERROR_UNATTACHED;
	}

	if (!pd_port->pd_prev_connected) {
#ifdef CONFIG_USB_PD_AUTO_TYPEC_ROLE_SWAP
		return hisi_tcpm_typec_role_swap(tcpc_dev);
#else
		D("pd_prev_connected is 0!\n");
		return TCPM_ERROR_NO_PD_CONNECTED;
#endif
	}

	/* Put a request into pd state machine. */
	ret = pd_put_dpm_pd_request_event(pd_port, PD_DPM_PD_REQUEST_PR_SWAP);
	if (!ret)
		return TCPM_ERROR_PUT_EVENT;

	return TCPM_SUCCESS;
}
EXPORT_SYMBOL(hisi_tcpm_power_role_swap);

int hisi_tcpm_data_role_swap(struct tcpc_device *tcpc_dev)
{
	bool ret = false;
	pd_port_t *pd_port = &tcpc_dev->pd_port;

	if (tcpc_dev->typec_attach_old == TYPEC_UNATTACHED)
		return TCPM_ERROR_UNATTACHED;

	if (!pd_port->pd_prev_connected) {
#ifdef CONFIG_USB_PD_AUTO_TYPEC_ROLE_SWAP
		return hisi_tcpm_typec_role_swap(tcpc_dev);
#else
		return TCPM_ERROR_NO_PD_CONNECTED;
#endif
	}

	ret = pd_put_dpm_pd_request_event(pd_port, PD_DPM_PD_REQUEST_DR_SWAP);
	if (!ret)
		return TCPM_ERROR_PUT_EVENT;

	return TCPM_SUCCESS;
}
EXPORT_SYMBOL(hisi_tcpm_data_role_swap);

int hisi_tcpm_vconn_swap(struct tcpc_device *tcpc_dev)
{
	bool ret = false;
	pd_port_t *pd_port = &tcpc_dev->pd_port;

	if (tcpc_dev->typec_attach_old == TYPEC_UNATTACHED)
		return TCPM_ERROR_UNATTACHED;

	if (!pd_port->pd_prev_connected)
		return TCPM_ERROR_NO_PD_CONNECTED;

	ret = pd_put_dpm_pd_request_event(pd_port,
			PD_DPM_PD_REQUEST_VCONN_SWAP);
	if (!ret)
		return TCPM_ERROR_PUT_EVENT;

	return TCPM_SUCCESS;
}
EXPORT_SYMBOL(hisi_tcpm_vconn_swap);

int hisi_tcpm_soft_reset(struct tcpc_device *tcpc_dev)
{
	bool ret = false;
	pd_port_t *pd_port = &tcpc_dev->pd_port;

	if (tcpc_dev->typec_attach_old == TYPEC_UNATTACHED) {
		D("TYPEC_UNATTACHED\n");
		return TCPM_ERROR_UNATTACHED;
	}

	if (!pd_port->pd_prev_connected) {
		D("TCPM_ERROR_NO_PD_CONNECTED\n");
		return TCPM_ERROR_NO_PD_CONNECTED;
	}

	ret = pd_put_dpm_pd_request_event(pd_port, PD_DPM_PD_REQUEST_SOFTRESET);
	if (!ret)
		return TCPM_ERROR_PUT_EVENT;

	return TCPM_SUCCESS;
}
EXPORT_SYMBOL(hisi_tcpm_soft_reset);

int hisi_tcpm_hard_reset(struct tcpc_device *tcpc_dev)
{
	bool ret = false;
	pd_port_t *pd_port = &tcpc_dev->pd_port;

	if (tcpc_dev->typec_attach_old == TYPEC_UNATTACHED)
		return TCPM_ERROR_UNATTACHED;

	if (!pd_port->pd_prev_connected)
		return TCPM_ERROR_NO_PD_CONNECTED;

	ret = pd_put_dpm_pd_request_event(pd_port, PD_DPM_PD_REQUEST_HARDRESET);
	if (!ret)
		return TCPM_ERROR_PUT_EVENT;

	return TCPM_SUCCESS;
}
EXPORT_SYMBOL(hisi_tcpm_hard_reset);

int hisi_tcpm_get_source_cap(struct tcpc_device *tcpc_dev,
		struct tcpm_power_cap *cap)
{
	bool ret = false;
	pd_port_t *pd_port = &tcpc_dev->pd_port;

	if (tcpc_dev->typec_attach_old == TYPEC_UNATTACHED)
		return TCPM_ERROR_UNATTACHED;

	if (!pd_port->pd_prev_connected)
		return TCPM_ERROR_NO_PD_CONNECTED;

	if (cap == NULL) {
		ret = pd_put_dpm_pd_request_event(pd_port,
				PD_DPM_PD_REQUEST_GET_SOURCE_CAP);
		if (!ret)
			return TCPM_ERROR_PUT_EVENT;
	} else {
		mutex_lock(&pd_port->pd_lock);
		cap->cnt = pd_port->remote_src_cap.nr;
		if (memcpy_s(cap->pdos,
				sizeof(uint32_t) * TCPM_PDO_MAX_SIZE,
				pd_port->remote_src_cap.pdos,
				sizeof(uint32_t) * cap->cnt) != EOK)
			E("memcpy_s failed\n");
		mutex_unlock(&pd_port->pd_lock);
	}

	return TCPM_SUCCESS;
}
EXPORT_SYMBOL(hisi_tcpm_get_source_cap);

int hisi_tcpm_get_sink_cap(struct tcpc_device *tcpc_dev,
		struct tcpm_power_cap *cap)
{
	bool ret = false;
	pd_port_t *pd_port = &tcpc_dev->pd_port;

	if (tcpc_dev->typec_attach_old == TYPEC_UNATTACHED)
		return TCPM_ERROR_UNATTACHED;

	if (!pd_port->pd_prev_connected)
		return TCPM_ERROR_NO_PD_CONNECTED;

	if (cap == NULL) {
		ret = pd_put_dpm_pd_request_event(pd_port,
				PD_DPM_PD_REQUEST_GET_SINK_CAP);
		if (!ret)
			return TCPM_ERROR_PUT_EVENT;
	} else {
		mutex_lock(&pd_port->pd_lock);
		cap->cnt = pd_port->remote_snk_cap.nr;
		if (memcpy_s(cap->pdos,
				sizeof(uint32_t) * TCPM_PDO_MAX_SIZE,
				pd_port->remote_snk_cap.pdos,
				sizeof(uint32_t) * cap->cnt) != EOK)
			E("memcpy_s failed\n");
		mutex_unlock(&pd_port->pd_lock);
	}

	return TCPM_SUCCESS;
}
EXPORT_SYMBOL(hisi_tcpm_get_sink_cap);

int hisi_tcpm_get_local_sink_cap(struct tcpc_device *tcpc_dev,
		struct local_sink_cap *cap)
{
	int i;
	pd_port_t *pd_port = &tcpc_dev->pd_port;
	pd_port_power_caps *snk_cap = &pd_port->local_snk_cap;
	struct dpm_pdo_info_t sink;

	if (!pd_port->pd_prev_connected)
		return TCPM_ERROR_NO_PD_CONNECTED;

	for (i = 0; i < snk_cap->nr; i++) {
		hisi_dpm_extract_pdo_info(snk_cap->pdos[i], &sink);
		cap[i].mv = sink.vmin;
		cap[i].ma = sink.ma;
		cap[i].uw = sink.uw;
	}

	return TCPM_SUCCESS;
}
EXPORT_SYMBOL(hisi_tcpm_get_local_sink_cap);

int hisi_tcpm_request(struct tcpc_device *tcpc_dev, int mv, int ma)
{
	bool ret = false;
	pd_port_t *pd_port = &tcpc_dev->pd_port;

	if (tcpc_dev->typec_attach_old == TYPEC_UNATTACHED)
		return TCPM_ERROR_UNATTACHED;

	if (!pd_port->pd_prev_connected)
		return TCPM_ERROR_NO_PD_CONNECTED;

	mutex_lock(&pd_port->pd_lock);
	ret = hisi_pd_dpm_send_request(pd_port, mv, ma);
	mutex_unlock(&pd_port->pd_lock);

	if (!ret)
		return TCPM_ERROR_PUT_EVENT;

	return TCPM_SUCCESS;
}
EXPORT_SYMBOL(hisi_tcpm_request);

void hisi_tcpm_request_voltage(struct tcpc_device *tcpc_dev, int set_voltage)
{
	bool overload = false;
	int vol_mv = 0;
	int cur_ma = 0;
	int max_uw = 0;
	int ret, i;
	struct local_sink_cap sink_cap_info[TCPM_PDO_MAX_SIZE] = { {0} };

	D("+\n");
	ret = hisi_tcpm_get_local_sink_cap(tcpc_dev, sink_cap_info);
	if (ret != TCPM_SUCCESS) {
		D("hisi_tcpm_get_local_sink_cap ret %d\n", ret);
		return;
	}

	for (i = 0; i < TCPM_PDO_MAX_SIZE; i++) {
		if (sink_cap_info[i].mv > set_voltage)
			continue;
		overload = (sink_cap_info[i].uw > max_uw)
				|| ((sink_cap_info[i].uw == max_uw)
					&& (sink_cap_info[i].mv < vol_mv));
		if (overload) {
			max_uw = sink_cap_info[i].uw;
			vol_mv = sink_cap_info[i].mv;
			cur_ma = sink_cap_info[i].ma;
		}
	}

	if (!(vol_mv && cur_ma)) {
		D("vol or cur == 0\n");
		return;
	}

	ret = hisi_tcpm_request(tcpc_dev, vol_mv, cur_ma);
	if (ret != TCPM_SUCCESS)
		TYPEC_INFO("hisi_tcpm_request ret %d\n", ret);

	D("-\n");
}
EXPORT_SYMBOL(hisi_tcpm_request_voltage);

int hisi_tcpm_discover_cable(struct tcpc_device *tcpc_dev, uint32_t *vdos,
		unsigned int max_size)
{
	bool ret = false;
	pd_port_t *pd_port = &tcpc_dev->pd_port;

	D("+\n");
	if (tcpc_dev->typec_attach_old == TYPEC_UNATTACHED)
		return TCPM_ERROR_UNATTACHED;

	if (!pd_port->pd_prev_connected)
		return TCPM_ERROR_NO_PD_CONNECTED;

	if (vdos == NULL) {
		mutex_lock(&pd_port->pd_lock);
		pd_port->dpm_flags |= DPM_FLAGS_CHECK_CABLE_ID;
		ret = vdm_put_dpm_discover_cable_event(pd_port);
		mutex_unlock(&pd_port->pd_lock);

		if (!ret)
			return TCPM_ERROR_PUT_EVENT;
	} else {
		if (!pd_port->power_cable_present)
			return TCPM_ERROR_NO_POWER_CABLE;

		if (max_size > VDO_MAX_SIZE)
			max_size = VDO_MAX_SIZE;

		mutex_lock(&pd_port->pd_lock);
		if (memcpy_s(vdos,
				sizeof(uint32_t) * max_size,
				pd_port->cable_vdos,
				sizeof(uint32_t) * max_size) != EOK)
			E("memcpy_s failed\n");
		mutex_unlock(&pd_port->pd_lock);
	}

	D("-\n");
	return TCPM_SUCCESS;
}

void hisi_tcpm_detect_emark_cable(struct tcpc_device *tcpc_dev)
{
	hisi_pd_cable_flag_set(&tcpc_dev->pd_port);
	hisi_tcpm_data_role_swap(tcpc_dev);
	tcpc_dev->pd_port.dpm_flags |= DPM_FLAGS_CHECK_CABLE_ID_DFP;

	return;
}

#ifdef CONFIG_USB_PD_UVDM_SUPPORT
int hisi_tcpm_send_uvdm(struct tcpc_device *tcpc_dev,
		uint8_t cnt, uint32_t *data, bool wait_resp)
{
	bool ret = false;
	pd_port_t *pd_port = &tcpc_dev->pd_port;

	D("+\n");
	if (tcpc_dev->typec_attach_old == TYPEC_UNATTACHED)
		return TCPM_ERROR_UNATTACHED;

	if (!pd_port->pd_prev_connected)
		return TCPM_ERROR_NO_PD_CONNECTED;

	if (cnt > VDO_MAX_SIZE)
		return TCPM_ERROR_PARAMETER;

	mutex_lock(&pd_port->pd_lock);

	pd_port->uvdm_cnt = cnt;
	pd_port->uvdm_wait_resp = wait_resp;
	if (memcpy_s(pd_port->uvdm_data,
			sizeof(uint32_t) * VDO_MAX_SIZE,
			data,
			sizeof(uint32_t) * cnt) != EOK)
		E("memcpy_s failed\n");

	ret = vdm_put_dpm_vdm_request_event(
		pd_port, PD_DPM_VDM_REQUEST_UVDM);

	mutex_unlock(&pd_port->pd_lock);

	if (!ret)
		return TCPM_ERROR_PUT_EVENT;

	D("-\n");
	return 0;
}
EXPORT_SYMBOL(hisi_tcpm_send_uvdm);
#endif /* CONFIG_USB_PD_UVDM_SUPPORT */
#endif /* CONFIG_USB_POWER_DELIVERY_SUPPORT */

int hisi_tcpm_register_tcpc_dev_notifier(struct tcpc_device *tcp_dev,
		struct notifier_block *nb)
{
	return hisi_register_tcp_dev_notifier(tcp_dev, nb);
}
EXPORT_SYMBOL(hisi_tcpm_register_tcpc_dev_notifier);

int hisi_tcpm_unregister_tcpc_dev_notifier(struct tcpc_device *tcp_dev,
		struct notifier_block *nb)
{
	return hisi_unregister_tcp_dev_notifier(tcp_dev, nb);
}
EXPORT_SYMBOL(hisi_tcpm_unregister_tcpc_dev_notifier);
