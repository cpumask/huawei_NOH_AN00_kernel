#ifndef _HISI_USB_DEBUG_H_
#define _HISI_USB_DEBUG_H_

#include <linux/device.h>
#include <linux/kernel.h>
#include "dwc3-hisi.h"

typedef ssize_t (*hiusb_debug_show_ops)(void *, char *, size_t);
typedef ssize_t (*hiusb_debug_store_ops)(void *, const char *, size_t);

#ifdef CONFIG_HISI_DEBUG_FS
int create_attr_file(struct hisi_dwc3_device *hisi_dwc3);
void remove_attr_file(struct hisi_dwc3_device *hisi_dwc3);
#else
static inline int create_attr_file(struct hisi_dwc3_device *hisi_dwc3)
{
	return 0;
}
static inline void remove_attr_file(struct hisi_dwc3_device *hisi_dwc3) {}
#endif
#endif /* _HISI_USB_DEBUG_H_ */
