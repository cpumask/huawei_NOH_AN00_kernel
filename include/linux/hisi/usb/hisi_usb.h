#ifndef _HISI_USB_H_
#define _HISI_USB_H_

#include <linux/usb.h>
#include <linux/hisi/usb/hisi_hifi_usb.h>
#include <linux/hisi/usb/tca.h>

#include <huawei_platform/usb/hw_usb.h>

enum usb_state {
	USB_STATE_UNKNOWN = 0,
	USB_STATE_OFF,
	USB_STATE_DEVICE,
	USB_STATE_HOST,
	USB_STATE_HIFI_USB,
	USB_STATE_HIFI_USB_HIBERNATE,
	USB_STATE_ILLEGAL,
};

enum hisi_charger_type {
	CHARGER_TYPE_SDP = 0,		/* Standard Downstreame Port */
	CHARGER_TYPE_CDP,		/* Charging Downstreame Port */
	CHARGER_TYPE_DCP,		/* Dedicate Charging Port */
	CHARGER_TYPE_UNKNOWN,		/* non-standard */
	CHARGER_TYPE_NONE,		/* not connected */

	/* other messages */
	PLEASE_PROVIDE_POWER,		/* host mode, provide power */
	CHARGER_TYPE_ILLEGAL,		/* illegal type */
};

/**
 * event types passed to hisi_usb_otg_event().
 */
enum otg_dev_event_type {
	CHARGER_CONNECT_EVENT = 0,
	CHARGER_DISCONNECT_EVENT,
	ID_FALL_EVENT,
	ID_RISE_EVENT,
	DP_OUT,
	DP_IN,
	START_HIFI_USB,
	START_HIFI_USB_RESET_VBUS,
	STOP_HIFI_USB,
	STOP_HIFI_USB_RESET_VBUS,
	HIFI_USB_HIBERNATE,
	HIFI_USB_WAKEUP,
	DISABLE_USB3_PORT,
	NONE_EVENT,
	MAX_EVENT_TYPE = BITS_PER_LONG,
};

enum hisi_usb_event_flag {
	EVENT_CB_AT_PREPARE = 1u,
	EVENT_CB_AT_HANDLE = (1u << 1),
	EVENT_CB_AT_COMPLETE = (1u << 2),
	PD_EVENT = (1u << 8),
};

/* size of struct hiusb_event must be a power of 2 for kfifo */
#define SIZE_HISI_USB_EVENT 32
struct hisi_usb_event {
	enum otg_dev_event_type type;
	u32 param1;
	u32 param2;
	u32 flags;
	void (*callback)(struct hisi_usb_event *event);
	void *content;
};


#if defined(CONFIG_USB_DWC3)
int hisi_charger_type_notifier_register(struct notifier_block *nb);
int hisi_charger_type_notifier_unregister(struct notifier_block *nb);
enum hisi_charger_type hisi_get_charger_type(void);

/**
 * Queue a event to be processed.
 * @evnet: the event to be processed.
 *
 * The event will be added to tail of a queue, and processed in a work.
 *
 * Return: 0 means the event added sucessfully. others means event was rejected.
 */
int hisi_usb_queue_event(struct hisi_usb_event *event);
int hisi_usb_otg_event(enum otg_dev_event_type);

void hisi_usb_otg_bc_again(void);
int hisi_usb_otg_irq_notifier_register(struct notifier_block *nb);
int hisi_usb_otg_irq_notifier_unregister(struct notifier_block *nb);
int hisi_usb_wakeup_hifi_usb(void);
int hisi_usb_otg_use_hifi_ip_first(void);
int hisi_usb_otg_get_typec_orien(void);
void hisi_usb_cancel_bc_again(int sync);
bool hisi_usb_state_is_host(void);
#else
static inline int hisi_charger_type_notifier_register(
		struct notifier_block *nb){return 0;}
static inline int hisi_charger_type_notifier_unregister(
		struct notifier_block *nb){return 0;}
static inline enum hisi_charger_type hisi_get_charger_type(void)
{
	return CHARGER_TYPE_NONE;
}
static inline int hisi_usb_otg_event(enum otg_dev_event_type event_type)
{
	return 0;
}
static inline int hisi_usb_otg_event_sync(TCPC_MUX_CTRL_TYPE mode_type,
		enum otg_dev_event_type event_type, TYPEC_PLUG_ORIEN_E typec_orien)
{
	return 0;
}
static inline void hisi_usb_otg_bc_again(void)
{
}
static inline int hisi_usb_otg_irq_notifier_register(
	struct notifier_block *nb){return 0;}
static inline int hisi_usb_otg_irq_notifier_unregister(
	struct notifier_block *nb){return 0;}
static inline int hisi_usb_wakeup_hifi_usb(void){return 0;}
static inline int hisi_usb_otg_use_hifi_ip_first(void){return 0;}
static inline int hisi_usb_otg_get_typec_orien(void){return 0;}
static inline void hisi_usb_cancel_bc_again(int sync)
{
}

static inline bool hisi_usb_state_is_host(void)
{
	return false;
}
#endif /* CONFIG_USB_DWC3 */

static inline int hisi_usb_id_change(enum otg_dev_event_type event)
{
	if ((event == ID_FALL_EVENT) || (event == ID_RISE_EVENT))
		return hisi_usb_otg_event(event);
	else
		return 0;
}

#if defined(CONFIG_CONTEXTHUB_PD) && !defined(CONFIG_HISI_COMBOPHY)
extern u32 usb31phy_cr_read(u32 addr);
extern int usb31phy_cr_write(u32 addr, u32 value);
#else
static inline u32 usb31phy_cr_read(u32 addr) {return 0;}
static inline int usb31phy_cr_write(u32 addr, u32 value) {return 0;}
#endif /*CONFIG_CONTEXTHUB_PD */

#endif /* _HISI_USB_H_*/
