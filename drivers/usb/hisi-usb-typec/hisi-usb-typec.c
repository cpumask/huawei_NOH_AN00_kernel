/*
 * Hisilicon USB Type-C framework.
 *
 * Copyright (c) 2019 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/platform_device.h>
#include <linux/of_platform.h>
#include <linux/spinlock.h>
#include <linux/notifier.h>
#include <linux/fb.h>
#include <linux/debugfs.h>

#include <huawei_platform/power/huawei_charger.h>
#include <chipset_common/hwpower/power_dsm.h>
#ifdef CONFIG_HUAWEI_DSM
#include <dsm/dsm_pub.h>
#endif
#include <huawei_platform/usb/hw_pd_dev.h>
#include <huawei_platform/power/boost_5v.h>
#include <huawei_platform/dp_aux_switch/dp_aux_switch.h>

#include <linux/hisi/usb/hisi_hifi_usb.h>
#include <linux/hisi/usb/tca.h>
#include <linux/hisi/usb/hisi_tcpm.h>
#include "hisi-usb-typec.h"

#include <securec.h>

#define ANALOG_SINGLE_CC_STS_MASK 0xF
#define ANALOG_CC2_STS_OFFSET 4
#define ANALOG_CC_WITH_UNDEFINED_RESISTANCE 1

#ifdef CONFIG_HISI_USB_TYPEC_DBG
#define D(format, arg...) \
		pr_info("[hisitypec][DBG][%s]" format, __func__, ##arg)
#else
#define D(format, arg...) do {} while (0)
#endif
#define I(format, arg...) \
		pr_info("[hisitypec][INF][%s]" format, __func__, ##arg)
#define E(format, arg...) \
		pr_err("[hisitypec][ERR][%s]" format, __func__, ##arg)

struct usb_typec {
	struct platform_device *pdev;
	struct tcpc_device *tcpc_dev;

	struct mutex lock;
	struct tcp_ny_vbus_state vbus_state;
	struct tcp_ny_typec_state typec_state;
	struct tcp_ny_pd_state pd_state;
	struct tcp_ny_mode_ctrl mode_ctrl;
	struct tcp_ny_ama_dp_state ama_dp_state;
	struct tcp_ny_ama_dp_attention ama_dp_attention;
	struct tcp_ny_ama_dp_hpd_state ama_dp_hpd_state;
	struct tcp_ny_uvdm uvdm_msg;

	uint16_t dock_svid;
	int rt_vbus_state;

	int power_role;
	int data_role;
	int vconn;
	int audio_accessory;

	bool direct_charge_cable;
	bool direct_charging;
	int pd_adapter_voltage;
	unsigned int bc12_type;

	uint8_t rt_cc1;
	uint8_t rt_cc2;
	unsigned long time_stamp_cc_alert;
	unsigned long time_stamp_typec_attached;
	unsigned long time_stamp_pd_attached;

	/* for handling tcpc notify */
	struct notifier_block tcpc_nb;
	struct list_head tcpc_notify_list;
	spinlock_t tcpc_notify_list_lock;
	unsigned int tcpc_notify_count;
	struct work_struct tcpc_notify_work;
	struct workqueue_struct *hisi_typec_wq;

	TCPC_MUX_CTRL_TYPE dp_mux_type;

	struct dentry *debugfs_root;

	struct notifier_block wakelock_control_nb;
	struct notifier_block bc12_nb;

	unsigned int suspend_count;
	unsigned int resume_count;
};

static struct usb_typec *_typec;

#ifdef CONFIG_HISI_DEBUG_FS
static inline char *charger_type_string(int charger_type)
{
	switch (charger_type) {
	case CHARGER_TYPE_NONE: return "none";
	case CHARGER_TYPE_SDP: return "sdp";
	case CHARGER_TYPE_DCP: return "dcp";
	case CHARGER_TYPE_CDP: return "cdp";
	case CHARGER_TYPE_UNKNOWN: return "unknown";
	case PLEASE_PROVIDE_POWER: return "sourcing vbus";
	default: return "unknown value";
	}
}

static inline char *remote_rp_level_string(unsigned char rp_level)
{
	switch (rp_level) {
	case TYPEC_CC_VOLT_OPEN: return "open";
	case TYPEC_CC_VOLT_RA: return "Ra";
	case TYPEC_CC_VOLT_RD: return "Rd";
	case TYPEC_CC_VOLT_SNK_DFT: return "Rp_Default";
	case TYPEC_CC_VOLT_SNK_1_5: return "Rp_1.5A";
	case TYPEC_CC_VOLT_SNK_3_0: return "Rp_3.0A";
	default: return "illegal value";
	}
}

static int usb_typec_status_show(struct seq_file *s, void *data)
{
	struct usb_typec *typec = s->private;

	if (!typec)
		return -ENOENT;

	seq_printf(s, "power_role             %s\n",
		   (typec->power_role == PD_ROLE_SOURCE) ? "SOURCE" :
		   (typec->power_role == PD_ROLE_SINK) ? "PD_SINK" : "SINK");
	seq_printf(s, "data_role              %s\n",
		   (typec->data_role == PD_ROLE_UFP) ? "UFP" : "DFP");
	seq_printf(s, "vconn                  %s\n",
		    typec->vconn ? "Y" : "N");
	seq_printf(s, "audio_accessory        %s\n",
		   typec->audio_accessory ? "Y" : "N");

	seq_printf(s, "orient                 %s\n",
		   typec->typec_state.polarity ? "fliped" : "normal");
	seq_printf(s, "chargerType            %s\n",
		   charger_type_string(typec->bc12_type));

	seq_printf(s, "tcpc_notify_count      %u\n", typec->tcpc_notify_count);
	seq_printf(s, "dp_mux_type            %u\n", typec->dp_mux_type);

	seq_printf(s, "direct_charge_cable    %s\n",
		   typec->direct_charge_cable ? "Y" : "N");
	seq_printf(s, "direct_charging        %s\n",
		   typec->direct_charging ? "Y" : "N");
	seq_printf(s, "suspend_count          %u\n", typec->suspend_count);
	seq_printf(s, "resume_count           %u\n", typec->resume_count);

	seq_printf(s, "remote rp level        %s\n",
		   remote_rp_level_string(typec->typec_state.rp_level));
	seq_printf(s, "dock_svid              0x%04x\n", typec->dock_svid);
	seq_printf(s, "pd_adapter_voltage     %d\n", typec->pd_adapter_voltage);

	seq_printf(s, "vbus.ma                %d\n", typec->vbus_state.ma);
	seq_printf(s, "vbus.mv                %d\n", typec->vbus_state.mv);
	seq_printf(s, "vbus.type              0x%x\n", typec->vbus_state.type);

	return 0;
}

static int usb_typec_status_open(struct inode *inode, struct file *file)
{
	return single_open(file, usb_typec_status_show, inode->i_private);
}

static const struct file_operations usb_typec_status_fops = {
	.open			= usb_typec_status_open,
	.read			= seq_read,
	.llseek			= seq_lseek,
	.release		= single_release,
};

static int usb_typec_perf_show(struct seq_file *s, void *data)
{
	struct usb_typec *typec = s->private;

	if (!typec)
		return -ENOENT;

	if (typec->data_role == PD_ROLE_DFP) {
		seq_printf(s, "attached.source     %u ms\n",
			jiffies_to_msecs(typec->time_stamp_typec_attached
					- typec->time_stamp_cc_alert));
	} else if (typec->data_role == PD_ROLE_UFP) {
		seq_printf(s, "attached.sink       %u ms\n",
			jiffies_to_msecs(typec->time_stamp_typec_attached
					- typec->time_stamp_cc_alert));
	}

	if (typec->pd_state.connected != HISI_PD_CONNECT_NONE) {
		seq_printf(s, "pd connect(%u)      %u ms\n",
			typec->pd_state.connected,
			jiffies_to_msecs(typec->time_stamp_pd_attached
					- typec->time_stamp_cc_alert));
	}

	return 0;
}

static int usb_typec_perf_open(struct inode *inode, struct file *file)
{
	return single_open(file, usb_typec_perf_show, inode->i_private);
}

static const struct file_operations usb_typec_perf_fops = {
	.open			= usb_typec_perf_open,
	.read			= seq_read,
	.llseek			= seq_lseek,
	.release		= single_release,
};

static int usb_typec_cable_show(struct seq_file *s, void *data)
{
#define PD_VDO_MAX_SIZE 7

	struct usb_typec *typec = s->private;
	uint32_t vdos[PD_VDO_MAX_SIZE] = {0};
	int i;
#ifdef CONFIG_USB_POWER_DELIVERY_SUPPORT
	int ret;
#endif

	if (!typec)
		return -ENOENT;

#ifdef CONFIG_USB_POWER_DELIVERY_SUPPORT
	ret = hisi_tcpm_discover_cable(typec->tcpc_dev, vdos, PD_VDO_MAX_SIZE);
	if (ret != TCPM_SUCCESS) {
		E("hisi_tcpm_discover_cable error ret %d\n", ret);
		return 0;
	}
#endif

	for (i = 0; i < PD_VDO_MAX_SIZE; i++)
		seq_printf(s, "vdo[%d] 0x%08x\n", i, vdos[i]);

	return 0;
}

static int usb_typec_cable_open(struct inode *inode, struct file *file)
{
	return single_open(file, usb_typec_cable_show, inode->i_private);
}

static const struct file_operations usb_typec_cable_fops = {
	.open = usb_typec_cable_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
};

static int usb_typec_debugfs_init(struct usb_typec *typec)
{
	struct dentry *root = NULL;
	struct dentry *file = NULL;

	root = debugfs_create_dir("hisi_usb_typec", usb_debug_root);
	if (IS_ERR_OR_NULL(root))
		return -ENOMEM;

	file = debugfs_create_file("status", S_IRUGO, root,
				typec, &usb_typec_status_fops);
	if (!file)
		goto err;

	file = debugfs_create_file("perf", S_IRUGO, root,
				typec, &usb_typec_perf_fops);
	if (!file)
		goto err;

	file = debugfs_create_file("cable", S_IRUGO, root,
				typec, &usb_typec_cable_fops);
	if (!file)
		goto err;

	typec->debugfs_root = root;
	return 0;

err:
	E("typec debugfs init failed\n");
	debugfs_remove_recursive(root);
	return -ENOMEM;
}

static void usb_typec_debugfs_exit(struct usb_typec *typec)
{
	if (!typec->debugfs_root)
		return;

	debugfs_remove_recursive(typec->debugfs_root);
}
#else
static inline int usb_typec_debugfs_init(struct usb_typec *typec) { return 0; }
static inline void usb_typec_debugfs_exit(struct usb_typec *typec) {}
#endif

int hisi_usb_typec_otg_pwr_src(void)
{
#ifdef CONFIG_TCPC_CLASS
	return pd_dpm_get_otg_channel();
#else
	return 0;
#endif
}

/*
 * Turn on/off vconn power.
 * enable:0 - off, 1 - on
 */
void hisi_usb_typec_set_vconn(int enable)
{
	struct usb_typec *typec = _typec;

	I(" Vconn enable: %d\n", enable);
	typec->vconn = enable;
#ifdef CONFIG_TCPC_CLASS
	pd_dpm_handle_pe_event(PD_DPM_PE_EVT_SOURCE_VCONN, &enable);
#endif
}

#ifdef CONFIG_TCPC_CLASS
static bool hisi_usb_typec_ready(struct usb_typec *typec)
{
	if (!typec || !typec->tcpc_dev) {
		E("hisi usb tcpc not ready\n");
		return false;
	}

	return true;
}

static void handle_typec_unattached(struct usb_typec *typec,
		struct tcp_ny_typec_state *typec_state)
{
	if (typec->audio_accessory)
		typec->audio_accessory = 0;

	if (typec->data_role == PD_ROLE_UFP)
		typec->data_role = PD_ROLE_UNATTACHED;
	else if (typec->data_role == PD_ROLE_DFP)
		typec->data_role = PD_ROLE_UNATTACHED;

	typec->power_role = PD_ROLE_UNATTACHED;

	typec->direct_charge_cable = false;
	typec->direct_charging = false;
}

static void handle_typec_attached_sink(struct usb_typec *typec,
		struct tcp_ny_typec_state *typec_state)
{
	if (typec->data_role == PD_ROLE_UFP) {
		D("Already UFP\n");
		return;
	}

	typec->data_role = PD_ROLE_UFP;
}

static void handle_typec_attached_source(struct usb_typec *typec,
		struct tcp_ny_typec_state *typec_state)
{
	if (typec->data_role == PD_ROLE_DFP) {
		D("Already DFP\n");
		return;
	}

	typec->data_role = PD_ROLE_DFP;
}

static inline void handle_typec_attached_audio_accessory(
		struct usb_typec *typec,
		struct tcp_ny_typec_state *typec_state)
{
	typec->audio_accessory = true;
}

static inline void handle_typec_attached_debug_accessory(
		struct usb_typec *typec,
		struct tcp_ny_typec_state *typec_state)
{
	D(" Dbg Acc\n");
}

static inline void handle_typec_attached_debug_accessory_sink(
		struct usb_typec *typec,
		struct tcp_ny_typec_state *typec_state)
{
	typec->data_role = PD_ROLE_UFP;
}

/*
 * Handle typec state change event
 */
static void hisi_usb_typec_state_change(struct tcp_ny_typec_state *typec_state)
{
	struct usb_typec *typec = _typec;
	int ret;

	mutex_lock(&typec->lock);
	I("tyec_state: %s --> %s / %s / %s\n",
		typec_attach_type_name(typec_state->old_state),
		typec_attach_type_name(typec_state->new_state),
		typec_state->polarity ? "fliped" : "normal",
		tcpm_cc_voltage_status_string(typec_state->rp_level));

	/* Save typec_state for futher use. */
	ret = memcpy_s(&typec->typec_state, sizeof(struct tcp_ny_typec_state),
		typec_state, sizeof(typec->typec_state));
	if (ret != EOK)
		E("memcpy_s failed\n");

	switch (typec_state->new_state) {
	case TYPEC_UNATTACHED:
		handle_typec_unattached(typec, typec_state);
		break;
	case TYPEC_ATTACHED_SNK:
		handle_typec_attached_sink(typec, typec_state);
		break;
	case TYPEC_ATTACHED_SRC:
		handle_typec_attached_source(typec, typec_state);
		break;
	case TYPEC_ATTACHED_AUDIO:
		handle_typec_attached_audio_accessory(typec, typec_state);
		break;
	case TYPEC_ATTACHED_DEBUG:
		handle_typec_attached_debug_accessory(typec, typec_state);
		break;
	case TYPEC_ATTACHED_DBGACC_SNK:
	case TYPEC_ATTACHED_CUSTOM_SRC:
		handle_typec_attached_debug_accessory_sink(typec, typec_state);
		break;
	case TYPEC_ATTACHED_VBUS_ONLY:
	case TYPEC_DETTACHED_VBUS_ONLY:
		break;
	default:
		E("unknown new_sate %u\n", typec_state->new_state);
		break;
	}

	typec->time_stamp_typec_attached = jiffies;

	mutex_unlock(&typec->lock);
}

/*
 * Handle PD state change, save PD state
 * pd_state:Should be one of hisi_pd_connect_result.
 */
static void hisi_usb_typec_pd_state_change(struct tcp_ny_pd_state *pd_state)
{
	struct usb_typec *typec = _typec;

	mutex_lock(&typec->lock);
	I("pd connect state: %u\n", pd_state->connected);
	typec->pd_state.connected = pd_state->connected;

	typec->time_stamp_pd_attached = jiffies;

	mutex_unlock(&typec->lock);
}

/*
 * Handle data role swap event
 */
static void hisi_usb_typec_data_role_swap(u8 role)
{
	struct usb_typec *typec = _typec;

	mutex_lock(&typec->lock);

	if (typec->data_role == PD_ROLE_UNATTACHED) {
		E("Unattached!\n");
		goto done;
	}

	if (typec->data_role == role) {
		D("Data role not change!\n");
		goto done;
	}

	I("new role: %s", role == PD_ROLE_DFP ? "PD_ROLE_DFP" : "PD_ROLE_UFP");
	if (role == PD_ROLE_DFP)
		typec->data_role = PD_ROLE_DFP;
	else
		typec->data_role = PD_ROLE_UFP;
done:
	mutex_unlock(&typec->lock);
}

/*
 * Handle source vbus operation
 */
static void hisi_usb_typec_source_vbus(struct tcp_ny_vbus_state *vbus_state)
{
	struct usb_typec *typec = _typec;
	int ret;

	mutex_lock(&typec->lock);
	I("vbus_state: %d %d 0x%02x\n", vbus_state->ma,
			vbus_state->mv, vbus_state->type);

	/* Must save vbus_state first */
	ret = memcpy_s(&typec->vbus_state, sizeof(struct tcp_ny_vbus_state),
		vbus_state, sizeof(typec->vbus_state));
	if (ret != EOK)
		E("memcpy_s failed\n");

	I("power role: %d, data role: %d\n",
			typec->power_role, typec->data_role);

	if (vbus_state->mv != 0)
		typec->power_role = PD_ROLE_SOURCE;

	mutex_unlock(&typec->lock);
}

/*
 * Handle sink vbus operation
 */
static void hisi_usb_typec_sink_vbus(struct tcp_ny_vbus_state *vbus_state)
{
	struct usb_typec *typec = _typec;

	mutex_lock(&typec->lock);

	/* save vbus_state. */
	typec->vbus_state.ma = vbus_state->ma;
	typec->vbus_state.mv = vbus_state->mv;
	typec->vbus_state.type = vbus_state->type;
	I("vbus_state: %d %d 0x%02x\n", vbus_state->ma,
			vbus_state->mv, vbus_state->type);
	I("power role: %d, data role: %d\n",
			typec->power_role, typec->data_role);

	if (vbus_state->mv != 0)
		typec->power_role = PD_ROLE_SINK;

	mutex_unlock(&typec->lock);
}

static void hisi_usb_typec_disable_vbus_control(
		struct tcp_ny_vbus_state *vbus_state)
{
	struct usb_typec *typec = _typec;

	mutex_lock(&typec->lock);

	typec->vbus_state.ma = vbus_state->ma;
	typec->vbus_state.mv = vbus_state->mv;
	typec->vbus_state.type = vbus_state->type;
	I("vbus_state: %d %d 0x%02x\n", vbus_state->ma,
			vbus_state->mv, vbus_state->type);

	typec->power_role = PD_ROLE_UNATTACHED;
	mutex_unlock(&typec->lock);
}
#endif

void hisi_usb_pd_dp_state_change(struct tcp_ny_ama_dp_state *ama_dp_state)
{
	struct usb_typec *typec = _typec;
#ifdef CONFIG_TCPC_CLASS
#ifdef CONFIG_CONTEXTHUB_PD
	struct pd_dpm_combphy_event event;
	int ret;
#endif
#endif

	I("%u %u 0x%x %u %u\n", ama_dp_state->sel_config, ama_dp_state->signal,
			ama_dp_state->pin_assignment, ama_dp_state->polarity,
			ama_dp_state->active);

	/* add aux switch */
	dp_aux_switch_op(ama_dp_state->polarity);

	/* add aux uart switch*/
	dp_aux_uart_switch_enable();

#ifndef MODE_DP_PIN_A
#define MODE_DP_PIN_A 0x01
#define MODE_DP_PIN_B 0x02
#define MODE_DP_PIN_C 0x04
#define MODE_DP_PIN_D 0x08
#define MODE_DP_PIN_E 0x10
#define MODE_DP_PIN_F 0x20
#else
#error
#endif

	mutex_lock(&typec->lock);

	if((MODE_DP_PIN_C == ama_dp_state->pin_assignment)
			|| (MODE_DP_PIN_E == ama_dp_state->pin_assignment))
		typec->dp_mux_type = TCPC_DP;
	else if((MODE_DP_PIN_D == ama_dp_state->pin_assignment)
			|| (MODE_DP_PIN_F == ama_dp_state->pin_assignment))
		typec->dp_mux_type = TCPC_USB31_AND_DP_2LINE;

	mutex_unlock(&typec->lock);

#ifdef CONFIG_TCPC_CLASS
#ifdef CONFIG_CONTEXTHUB_PD
	event.dev_type = TCA_ID_RISE_EVENT;
	event.irq_type = TCA_IRQ_HPD_OUT;
	event.mode_type = TCPC_NC;
	event.typec_orien = ama_dp_state->polarity;

	ret = pd_dpm_handle_combphy_event(event);
	if (ret)
		E("RISE EVENT ret %d\n", ret);

	pd_dpm_set_combphy_status(typec->dp_mux_type);

	event.dev_type = TCA_ID_FALL_EVENT;
	event.irq_type = TCA_IRQ_HPD_IN;
	event.mode_type = typec->dp_mux_type;
	event.typec_orien = ama_dp_state->polarity;
	ret = pd_dpm_handle_combphy_event(event);
	if (ret)
		E("FALL_EVENT ret %d\n", ret);
#endif
#endif
}

void hisi_usb_pd_dp_hpd_state_change(
		struct tcp_ny_ama_dp_hpd_state *ama_dp_hpd_state)
{
	struct usb_typec *typec = _typec;
#ifdef CONFIG_TCPC_CLASS
#ifdef CONFIG_CONTEXTHUB_PD
	struct pd_dpm_combphy_event event;
	int ret;
#endif
#endif

	I("irq %u, state %u\n", ama_dp_hpd_state->irq, ama_dp_hpd_state->state);
	mutex_lock(&typec->lock);

	typec->ama_dp_hpd_state.irq = ama_dp_hpd_state->irq;
	typec->ama_dp_hpd_state.state = ama_dp_hpd_state->state;

	mutex_unlock(&typec->lock);

#ifdef CONFIG_TCPC_CLASS
#ifdef CONFIG_CONTEXTHUB_PD
	event.dev_type = TCA_DP_IN;
	event.irq_type = TCA_IRQ_HPD_IN;
	event.mode_type = typec->dp_mux_type;
	event.typec_orien = typec->typec_state.polarity;

	if (!ama_dp_hpd_state->state) {
		event.dev_type = TCA_DP_OUT;
		event.irq_type = TCA_IRQ_HPD_OUT;
		ret = pd_dpm_handle_combphy_event(event);
		if (ret)
			E("DP_OUT ret %d\n", ret);

		pd_dpm_set_last_hpd_status(false);
		pd_dpm_send_event(DP_CABLE_OUT_EVENT);
	} else {
		event.dev_type = TCA_DP_IN;
		ret = pd_dpm_handle_combphy_event(event);
		if (ret)
			E("TCA_DP_IN ret %d\n", ret);

		pd_dpm_set_last_hpd_status(true);
		pd_dpm_send_event(DP_CABLE_IN_EVENT);
	}

	if (ama_dp_hpd_state->irq) {
		event.irq_type = TCA_IRQ_SHORT;
		ret = pd_dpm_handle_combphy_event(event);
		if (ret)
			E("IRQ_SHORT ret %d\n", ret);
	}
#endif
#endif
}

void hisi_usb_pd_ufp_update_dock_svid(uint16_t svid)
{
	struct usb_typec *typec = _typec;

	D("0x%04x\n", svid);
	typec->dock_svid = svid;
}

/*
 * vbus state, 0:invalid, 1:valid.
 */
void hisi_usb_typec_power_status_change(int vbus_state)
{
	struct usb_typec *typec = _typec;

	I("vbus_state %d\n", vbus_state);
	typec->rt_vbus_state = vbus_state;
}

/* Monitor cc status which pass through CCDebounce */
void hisi_usb_typec_cc_status_change(uint8_t cc1, uint8_t cc2)
{
#ifdef CONFIG_TCPC_CLASS
	pd_dpm_handle_pe_event(PD_DPM_PE_ABNORMAL_CC_CHANGE_HANDLER, NULL);
#endif
}

/*
 * transfer pid/vid to hw_pd
 * payload[0]: reserved
 * payload[1]: id header vdo
 * payload[2]: reserved
 * payload[3]: product vdo
 */
void hisi_usb_typec_dfp_inform_id(uint32_t *payload, uint32_t size)
{
	uint32_t bcd = 0;
	uint32_t vid = 0;
	uint32_t pid = 0;

	if (payload && size >= 4) {
		bcd = *(payload + 3) & PD_DPM_HW_PDO_MASK;
		vid = *(payload + 1) & PD_DPM_HW_PDO_MASK;
		pid = (*(payload + 3) >> PD_DPM_PDT_VID_OFFSET) &
			PD_DPM_HW_PDO_MASK;
	}
#ifdef CONFIG_TCPC_CLASS
	pd_set_product_id_info(vid, pid, bcd);
#endif
}

/*
 * Monitor the raw cc status
 */
void hisi_usb_typec_cc_alert(uint8_t cc1, uint8_t cc2)
{
	struct usb_typec *typec = _typec;

	/* for huawei type-c headset, wakeup AP. */
	hisi_usb_check_hifi_usb_status(HIFI_USB_TCPC);

	/* only record the time of first cc connection. */
	if ((typec->rt_cc1 == TYPEC_CC_VOLT_OPEN) &&
			(typec->rt_cc2 == TYPEC_CC_VOLT_OPEN)) {
		if ((cc1 != TYPEC_CC_VOLT_OPEN) ||
			(cc2 != TYPEC_CC_VOLT_OPEN)) {
			D("update time_stamp_cc_alert\n");
			typec->time_stamp_cc_alert = jiffies;
		}
	}

	/* record real cc status */
	typec->rt_cc1 = cc1;
	typec->rt_cc2 = cc2;
}

bool hisi_usb_typec_charger_type_pd(void)
{
#ifdef CONFIG_TCPC_CLASS
	return pd_dpm_get_pd_finish_flag();
#endif
	return false;
}

#define TYPEC_DSM_BUF_SIZE_256	256
void hisi_usb_typec_cc_ovp_dmd_report(void)
{
#ifdef CONFIG_HUAWEI_DSM
	int ret;
	char msg_buf[TYPEC_DSM_BUF_SIZE_256] = { 0 };

	ret = snprintf_s(msg_buf, TYPEC_DSM_BUF_SIZE_256,
			TYPEC_DSM_BUF_SIZE_256 - 1,
			"%s\n",
			"vbus ovp happened");
	if (ret < 0)
		E("Fill cc ovp dmd msg\n");

	power_dsm_dmd_report(POWER_DSM_BATTERY,
			ERROR_NO_TYPEC_CC_OVP, (void *)msg_buf);
#endif
}

struct tcpc_notify {
	struct list_head node;
	struct tcp_notify notify;
	unsigned long tcpc_notify_type;
};

#define TCPC_NOTIFY_MAX_COUNT 4096
static int queue_notify(struct usb_typec *typec, unsigned long action,
		const void *data)
{
	struct tcpc_notify *noti = NULL;

	if (typec->tcpc_notify_count > TCPC_NOTIFY_MAX_COUNT) {
		E("tcpc_notify_list too long, %u\n", typec->tcpc_notify_count);
		return -EBUSY;
	}

	noti = kzalloc(sizeof(*noti), GFP_KERNEL);
	if (!noti) {
		E("No memory!\n");
		return -ENOMEM;
	}

	noti->tcpc_notify_type = action;
	if (memcpy_s(&noti->notify, sizeof(struct tcp_notify),
			data, sizeof(noti->notify)) != EOK)
		E("memcpy_s failed\n");

	spin_lock(&typec->tcpc_notify_list_lock);
	list_add_tail(&noti->node, &typec->tcpc_notify_list);
	typec->tcpc_notify_count++;
	spin_unlock(&typec->tcpc_notify_list_lock);

	return 0;
}

struct tcpc_notify *get_notify(struct usb_typec *typec)
{
	struct tcpc_notify *noti = NULL;

	spin_lock(&typec->tcpc_notify_list_lock);
	noti = list_first_entry_or_null(&typec->tcpc_notify_list,
				struct tcpc_notify, node);

	if (noti) {
		list_del_init(&noti->node);
		typec->tcpc_notify_count--;
	}
	spin_unlock(&typec->tcpc_notify_list_lock);

	return noti;
}

static void free_notify(struct tcpc_notify *noti)
{
	kfree(noti);
}

#ifdef CONFIG_TCPC_CLASS
static int pd_dpm_wake_lock_call(struct notifier_block *dpm_nb,
		unsigned long event, void *data)
{
	struct usb_typec *typec = _typec;

	if (!hisi_usb_typec_ready(typec))
		return NOTIFY_OK;

	switch (event) {
	case PD_WAKE_LOCK:
		I("lock\n");
		hisi_tcpm_typec_set_wake_lock(typec->tcpc_dev, true);
		break;
	case PD_WAKE_UNLOCK:
		I("unlock\n");
		hisi_tcpm_typec_set_wake_lock(typec->tcpc_dev, false);
		break;
	default:
		E("unknown event (%lu)\n", event);
		break;
	}

	return NOTIFY_OK;
}

static void hisi_usb_typec_issue_hardreset(void *data)
{
	struct usb_typec *typec = _typec;
	int ret;

	if (!hisi_usb_typec_ready(typec))
		return;

#ifdef CONFIG_USB_POWER_DELIVERY_SUPPORT
	ret = hisi_tcpm_hard_reset(typec->tcpc_dev);
	if (ret != TCPM_SUCCESS)
		E("hisi_tcpm_hard_reset ret %d\n", ret);
#endif
}

static bool hisi_usb_pd_get_hw_dock_svid_exist(void *client)
{
	struct usb_typec *typec = _typec;

	if (!typec) {
		E("hisi-tcpc not ready\n");
		return false;
	}

	return (typec->dock_svid == PD_DPM_HW_DOCK_SVID);

}

static int hisi_usb_typec_mark_direct_charging(void *data, bool direct_charging)
{
	struct usb_typec *typec = _typec;

	if (!hisi_usb_typec_ready(typec))
		return -ENODEV;

	I("%s\n", direct_charging ? "true" : "false");
	(void)hisi_tcpm_typec_notify_direct_charge(typec->tcpc_dev,
				direct_charging);
	typec->direct_charging = direct_charging;

	return 1;
}

static void hisi_usb_typec_set_pd_adapter_voltage(void *data, int voltage_mv)
{
	struct usb_typec *typec = _typec;

	if (!hisi_usb_typec_ready(typec))
		return;

	I("%d mV\n", voltage_mv);
	typec->pd_adapter_voltage = voltage_mv;

#ifdef CONFIG_USB_POWER_DELIVERY_SUPPORT
	hisi_tcpm_request_voltage(typec->tcpc_dev, voltage_mv);
#endif
}


static void hisi_usb_typec_send_uvdm(void *client, uint32_t *data,
	uint8_t cnt, bool wait_resp)
{
#ifdef CONFIG_ADAPTER_PROTOCOL_UVDM
	struct usb_typec *typec = _typec;

	if (!hisi_usb_typec_ready(typec))
		return;

	hisi_tcpm_send_uvdm(typec->tcpc_dev, cnt, data, wait_resp);
#endif
}

static int hisi_usb_get_cc_state(void)
{
	struct usb_typec *typec = _typec;

	if (!hisi_usb_typec_ready(typec))
		return 0;

	return typec->rt_cc1 | (typec->rt_cc2 << 2);
}

static bool hisi_usb_check_cc_vbus_short(void)
{
	struct usb_typec *typec = _typec;
	uint8_t cc, cc1, cc2;

	if (!hisi_usb_typec_ready(typec))
		return false;

	cc = hisi_tcpc_get_cc_from_analog_ch(typec->tcpc_dev);
	cc1 = cc & ANALOG_SINGLE_CC_STS_MASK;
	cc2 = (cc >> ANALOG_CC2_STS_OFFSET) & ANALOG_SINGLE_CC_STS_MASK;

	I("analog CC status: 0x%x\n", cc);

	return (cc1 == ANALOG_CC_WITH_UNDEFINED_RESISTANCE
		|| cc2 == ANALOG_CC_WITH_UNDEFINED_RESISTANCE);
}

static void hisi_usb_set_cc_mode(int mode)
{
	struct usb_typec *typec = _typec;

	if (!hisi_usb_typec_ready(typec))
		return;

	hisi_tcpm_force_cc_mode(typec->tcpc_dev, mode);
}

static int hisi_usb_pd_dpm_data_role_swap(void *client)
{
	struct usb_typec *typec = _typec;

	if (!hisi_usb_typec_ready(typec))
		return -1;

	D("DataRole Swap\n");
	return hisi_tcpm_data_role_swap(typec->tcpc_dev);
}

static void hisi_usb_detect_emark_cable(void *client)
{
	struct usb_typec *typec = _typec;

	if (!hisi_usb_typec_ready(typec))
		return ;

	D("en emark_cable det\n");
	hisi_tcpm_detect_emark_cable(typec->tcpc_dev);
}

static void hisi_usb_force_enable_drp(int mode)
{
	struct usb_typec *typec = _typec;

	if (!hisi_usb_typec_ready(typec))
		return;

	if (typec->typec_state.new_state == TYPEC_UNATTACHED)
		hisi_tcpm_force_cc_mode(typec->tcpc_dev, mode);
}

static int hisi_usb_disable_pd(void *client, bool disable)
{
	struct usb_typec *typec = _typec;

	I("+ %u\n", disable);

	if (!hisi_usb_typec_ready(typec))
		return -1;

#ifdef CONFIG_USB_POWER_DELIVERY_SUPPORT
	if (disable)
		hisi_pd_put_cc_detached_event(typec->tcpc_dev);
#endif

	I("-\n");
	return 0;
}

static struct pd_dpm_ops hisi_device_pd_dpm_ops = {
	.pd_dpm_get_hw_dock_svid_exist = hisi_usb_pd_get_hw_dock_svid_exist,
	.pd_dpm_notify_direct_charge_status =
			hisi_usb_typec_mark_direct_charging,
	.pd_dpm_get_cc_state = hisi_usb_get_cc_state,
	.pd_dpm_set_cc_mode = hisi_usb_set_cc_mode,
	.pd_dpm_enable_drp = hisi_usb_force_enable_drp,
	.pd_dpm_disable_pd = hisi_usb_disable_pd,
	.pd_dpm_check_cc_vbus_short = hisi_usb_check_cc_vbus_short,
	.pd_dpm_detect_emark_cable = hisi_usb_detect_emark_cable,
	.data_role_swap = hisi_usb_pd_dpm_data_role_swap,
};

static struct pd_protocol_ops hisi_device_pd_protocol_ops = {
	.chip_name = "scharger_v600",
	.hard_reset_master = hisi_usb_typec_issue_hardreset,
	.set_output_voltage = hisi_usb_typec_set_pd_adapter_voltage,
};

static struct uvdm_protocol_ops hisi_device_uvdm_protocol_ops = {
	.chip_name = "scharger_v600",
	.send_data = hisi_usb_typec_send_uvdm,
};

/*
 * Check the cable for direct charge or not.
 */
int hisi_usb_typec_direct_charge_cable_detect(void)
{
	struct usb_typec *typec = _typec;
	uint8_t cc1, cc2;
	int ret;

	if (!hisi_usb_typec_ready(typec))
		return -1;

	ret = hisi_tcpm_inquire_remote_cc(typec->tcpc_dev, &cc1, &cc2, true);
	if (ret) {
		E("inquire remote cc failed\n");
		return -1;
	}

	if ((cc1 == PD_DPM_CC_VOLT_SNK_DFT) &&
			(cc2 == PD_DPM_CC_VOLT_SNK_DFT)) {
		I("using \"direct charge cable\" !\n");
		typec->direct_charge_cable = true;
		return 0;
	}

	I("not \"direct charge cable\" !\n");
	typec->direct_charge_cable = false;

	return -1;
}

static struct cc_check_ops direct_charge_cable_check_ops = {
	.is_cable_for_direct_charge = hisi_usb_typec_direct_charge_cable_detect,
};

static inline int __tcpc_notifier_work(struct tcpc_notify *noti)
{
	struct pd_dpm_typec_state tc_state = {0};
	struct pd_dpm_vbus_state vbus_state = {0};
	struct pd_dpm_swap_state swap_state = {0};
	struct pd_dpm_pd_state pd_state = {0};
	uint32_t cable_vdo = 0;

	I("tcpc_notify_type %lu\n", noti->tcpc_notify_type);

	switch (noti->tcpc_notify_type) {
	case TCP_NOTIFY_DIS_VBUS_CTRL:
		hisi_usb_typec_disable_vbus_control(&noti->notify.vbus_state);

		vbus_state.mv = noti->notify.vbus_state.mv;
		vbus_state.ma = noti->notify.vbus_state.ma;
		vbus_state.vbus_type = noti->notify.vbus_state.type;

		pd_dpm_handle_pe_event(PD_DPM_PE_EVT_DIS_VBUS_CTRL,
				(void *)&vbus_state);

		break;
	case TCP_NOTIFY_SOURCE_VBUS:
		hisi_usb_typec_source_vbus(&noti->notify.vbus_state);

		vbus_state.mv = noti->notify.vbus_state.mv;
		vbus_state.ma = noti->notify.vbus_state.ma;
		vbus_state.vbus_type = noti->notify.vbus_state.type;

		pd_dpm_handle_pe_event(PD_DPM_PE_EVT_SOURCE_VBUS,
				(void *)&vbus_state);

		break;
	case TCP_NOTIFY_SINK_VBUS:
		hisi_usb_typec_sink_vbus(&noti->notify.vbus_state);

		vbus_state.mv = noti->notify.vbus_state.mv;
		vbus_state.ma = noti->notify.vbus_state.ma;
		vbus_state.vbus_type = noti->notify.vbus_state.type;
		vbus_state.ext_power = noti->notify.vbus_state.ext_power;
		vbus_state.remote_rp_level =
				noti->notify.vbus_state.remote_rp_level;

		pd_dpm_handle_pe_event(PD_DPM_PE_EVT_SINK_VBUS,
				(void *)&vbus_state);

		break;
	case TCP_NOTIFY_PR_SWAP:
		swap_state.new_role = noti->notify.swap_state.new_role;
		pd_dpm_handle_pe_event(PD_DPM_PE_EVT_PR_SWAP, &swap_state);

		break;
	case TCP_NOTIFY_DR_SWAP:
		hisi_usb_typec_data_role_swap(noti->notify.swap_state.new_role);

		swap_state.new_role = noti->notify.swap_state.new_role;
		pd_dpm_handle_pe_event(PD_DPM_PE_EVT_DR_SWAP, &swap_state);

		break;
	case TCP_NOTIFY_TYPEC_STATE:
		hisi_usb_typec_state_change(&noti->notify.typec_state);

		tc_state.polarity = noti->notify.typec_state.polarity;
		tc_state.old_state = noti->notify.typec_state.old_state;
		tc_state.new_state = noti->notify.typec_state.new_state;

		pd_dpm_handle_pe_event(PD_DPM_PE_EVT_TYPEC_STATE,
				(void *)&tc_state);

		break;
	case TCP_NOTIFY_PD_STATE:
		hisi_usb_typec_pd_state_change(&noti->notify.pd_state);

		pd_state.connected = noti->notify.pd_state.connected;
		pd_dpm_handle_pe_event(PD_DPM_PE_EVT_PD_STATE,
				(void *)&pd_state);
		break;

	case TCP_NOTIFY_CABLE_VDO:
		cable_vdo = noti->notify.cable_vdo.vdo;
		pd_dpm_handle_pe_event(PD_DPM_PE_CABLE_VDO, (void *)&cable_vdo);
		break;

	default:
		break;
	}

	return 0;
}

/*
 * Register pd dpm ops for hw_pd driver.
 */
int hisi_usb_typec_register_pd_dpm(void)
{
	int ret = 0;
	struct usb_typec *typec = _typec;

	if (!_typec) {
		E("hisi usb typec is not ready\n");
		return -EPERM;
	}

	ret = pd_dpm_ops_register(&hisi_device_pd_dpm_ops, typec);
	if (ret) {
		I("Need not hisi pd\n");
		return -EBUSY;
	}

	ret = pd_protocol_ops_register(&hisi_device_pd_protocol_ops, typec);
	if (ret) {
		I("pd protocol register failed\n");
		return -EBUSY;
	}

	ret = uvdm_protocol_ops_register(&hisi_device_uvdm_protocol_ops, typec);
	if (ret)
		E("uvdm protocol register failed\n");

	ret = cc_check_ops_register(&direct_charge_cable_check_ops);
	if (ret) {
		E("cc_check_ops register failed!\n");
		return -EBUSY;
	}

	typec->wakelock_control_nb.notifier_call = pd_dpm_wake_lock_call;
	ret = register_pd_wake_unlock_notifier(&typec->wakelock_control_nb);
	if (ret)
		E("register_pd_wake_unlock_notifier ret %d\n", ret);

	return ret;
}

/*
 * Record optional_max_power by max_power value om pdo.
 * max_power:max power value from pdo.
 */
void hisi_usb_typec_max_power(int max_power)
{
	pd_dpm_set_max_power(max_power);
}

/*
 * Reset hw_pd product type when src startup.
 */
void hisi_usb_typec_reset_product(void)
{
	pd_set_product_type(PD_DPM_INVALID_VAL);
}

#else
static inline int __tcpc_notifier_work(struct tcpc_notify *noti)
{
	E("[Not support Hisi typeC] >> tcpc_notify_type %lu\n",
			noti->tcpc_notify_type);
	return 0;
}

int hisi_usb_typec_register_pd_dpm(void)
{
	E("Not Support Hisi PD\n");
	return -EPERM;
}

void hisi_usb_typec_max_power(int max_power)
{
}

void hisi_usb_typec_reset_product(void)
{
}
#endif

static void tcpc_notifier_work(struct work_struct *work)
{
	struct usb_typec *typec =
			container_of(work, struct usb_typec, tcpc_notify_work);
	struct tcpc_notify *noti = NULL;
	int ret;

	while (1) {
		noti = get_notify(typec);
		if (!noti)
			break;

		ret = __tcpc_notifier_work(noti);
		if (ret)
			D("__tcpc_notifier_work ret %d\n", ret);

		free_notify(noti);
	}
}

static int tcpc_notifier_call(struct notifier_block *nb,
			unsigned long action, void *data)
{
	struct usb_typec *typec = container_of(nb, struct usb_typec, tcpc_nb);
	int ret;

	I("action %lu\n", action);

	ret = queue_notify(typec, action, data);
	if (ret) {
		E("queue_notify failed!\n");
		return NOTIFY_DONE;
	}

	/* Returns %false if @work was already on a queue, %true otherwise. */
	ret = queue_work(typec->hisi_typec_wq, &typec->tcpc_notify_work);
	D("queue_work ret %d\n", ret);

	return NOTIFY_OK;
}

int hisi_usb_typec_bc12_notify(struct notifier_block *nb,
			unsigned long action, void *data)
{
	struct usb_typec *typec = _typec;

	if (!typec) {
		E("hisi usb typec is not ready\n");
		return -ENODEV;
	}

	typec->bc12_type = (unsigned int)action;

	return 0;
}

/*
 * Save tcpc_device pointer for futher use.
 * tcpc_dev:Pointer of tcpc_device structure.
 */
void hisi_usb_typec_register_tcpc_device(struct tcpc_device *tcpc_dev)
{
	struct usb_typec *typec = _typec;
	int ret;

	if (!_typec) {
		E("hisi usb typec is not ready\n");
		return;
	}

	typec->bc12_nb.notifier_call = hisi_usb_typec_bc12_notify;
	(void)hisi_charger_type_notifier_register(&typec->bc12_nb);

	/* save the tcpc handler */
	typec->tcpc_dev = tcpc_dev;

	INIT_LIST_HEAD(&typec->tcpc_notify_list);
	spin_lock_init(&typec->tcpc_notify_list_lock);
	typec->tcpc_notify_count = 0;
	INIT_WORK(&typec->tcpc_notify_work, tcpc_notifier_work);

	typec->tcpc_nb.notifier_call = tcpc_notifier_call;
	ret = hisi_tcpm_register_tcpc_dev_notifier(tcpc_dev, &typec->tcpc_nb);
	if (ret)
		E("register tcpc notifier failed ret %d\n", ret);

	ret = usb_typec_debugfs_init(typec);
	if (ret)
		E("debugfs init failed ret %d\n", ret);
}

static int typec_probe(struct platform_device *pdev)
{
	struct usb_typec *typec = NULL;
	int ret = 0;

	typec = devm_kzalloc(&pdev->dev, sizeof(*typec), GFP_KERNEL);
	if (!typec)
		return -ENOMEM;

	typec->power_role = PD_ROLE_UNATTACHED;
	typec->data_role = PD_ROLE_UNATTACHED;
	typec->vconn = PD_ROLE_VCONN_OFF;
	typec->audio_accessory = 0;

	mutex_init(&typec->lock);

	typec->hisi_typec_wq = create_singlethread_workqueue("hisi_usb_typec");

	_typec = typec;

	ret = of_platform_populate(pdev->dev.of_node, NULL, NULL, &pdev->dev);
	if (ret) {
		E("populate child failed ret %d\n", ret);
		_typec = NULL;
		return ret;
	}

	return 0;
}

static int typec_remove(struct platform_device *pdev)
{
	struct usb_typec *typec = _typec;

	of_platform_depopulate(&pdev->dev);
	_typec = NULL;
	usb_typec_debugfs_exit(typec);

	return 0;
}

#ifdef CONFIG_PM
static int typec_suspend(struct device *dev)
{
	struct usb_typec *typec = _typec;

	typec->suspend_count++;

	return 0;
}

static int typec_resume(struct device *dev)
{
	struct usb_typec *typec = _typec;

	typec->resume_count++;

	return 0;
}

static const struct dev_pm_ops typec_pm_ops = {
	SET_SYSTEM_SLEEP_PM_OPS(typec_suspend, typec_resume)
};
#define TYPEC_PM_OPS	(&typec_pm_ops)
#else
#define TYPEC_PM_OPS	(NULL)
#endif /* CONFIG_PM */

static const struct of_device_id typec_match_table[] = {
	{.compatible = "hisilicon,hisi-usb-typec",},
	{},
};

static struct platform_driver typec_driver = {
	.driver = {
		.name = "hisi-usb-typec",
		.owner = THIS_MODULE,
		.of_match_table = typec_match_table,
		.pm = TYPEC_PM_OPS,
	},
	.probe = typec_probe,
	.remove = typec_remove,
};

static int __init hisi_typec_init(void)
{
	return platform_driver_register(&typec_driver);
}

static void __exit hisi_typec_exit(void)
{
	platform_driver_unregister(&typec_driver);
}

subsys_initcall(hisi_typec_init);
module_exit(hisi_typec_exit);

MODULE_AUTHOR("Xiaowei Song <songxiaowei@hisilicon.com>");
MODULE_DESCRIPTION("Hisilicon USB Type-C Driver");
MODULE_LICENSE("GPL");
