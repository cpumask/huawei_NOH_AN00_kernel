#ifndef DWC3_USB_INTERFACE_H
#define DWC3_USB_INTERFACE_H

#include <linux/notifier.h>
#include <linux/types.h>


#define SET_NBITS_MASK(start, end) (((2u << ((end) - (start))) - 1) << (start))
#define SET_BIT_MASK(bit) SET_NBITS_MASK(bit, bit)

#define DEVICE_EVENT_CONNECT_DONE	0
#define DEVICE_EVENT_PULLUP	1
#define DEVICE_EVENT_CMD_TMO	2
#define DEVICE_EVENT_SETCONFIG	3


struct usb3_core_ops {
	void (*disable_usb2phy_suspend)(void);
	void (*clear_svc_opp_per_hs)(void);
	void (*disable_rx_thres_cfg)(void);
	void (*set_svc_opp_per_hs_sep)(void);
	void (*adjust_dtout)(void);
	void (*force_disable_host_lpm)(void);
	void (*enable_hst_imm_retry)(void);
	void (*enable_p4_gate)(void);
	void (*disable_pipe_clock)(void);
	int (*cptest_next_pattern)(void);
	int (*enable_u3)(void);
	void (*dump_regs)(void);
#ifdef CONFIG_HISI_DEBUG_FS
	void (*link_state_print)(void);
#endif
	void (*logic_analyzer_trace_set)(u32 value);
};

void set_hisi_dwc3_power_flag(int val);
int get_hisi_dwc3_power_flag(void);
struct usb3_core_ops * get_usb3_core_ops(void);
void dwc3_lscdtimer_set(void);
int dwc3_compliance_mode_enable(void);

#if IS_ENABLED(CONFIG_USB_DWC3_GADGET) || IS_ENABLED(CONFIG_USB_DWC3_DUAL_ROLE)
int dwc3_device_event_notifier_register(struct notifier_block *nb);
int dwc3_device_event_notifier_unregister(struct notifier_block *nb);
#else
static inline int dwc3_device_event_notifier_register(struct notifier_block *nb)
{ return 0; }
static inline int dwc3_device_event_notifier_unregister(struct notifier_block *nb)
{ return 0; }
#endif

#endif /* dwc3_usb_interface.h */
