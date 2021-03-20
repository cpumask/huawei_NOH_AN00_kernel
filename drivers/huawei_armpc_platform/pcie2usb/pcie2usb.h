#ifndef _LINUX_HW_USB_HUB_H
#define _LINUX_HW_USB_HUB_H


#define PCIE2USB_DEBUG

#ifndef PCIE2USB_DEBUG
#define pcie2usb_debug(fmt, args...)do {} while (0)
#define pcie2usb_info(fmt, args...) do {} while (0)
#define pcie2usb_warn(fmt, args...) do {} while (0)
#define pcie2usb_err(fmt, args...)  do {} while (0)
#else
#define pcie2usb_debug(fmt, args...)do { printk(KERN_DEBUG   "[pcie2usb]" fmt, ## args); } while (0)
#define pcie2usb_info(fmt, args...) do { printk(KERN_INFO    "[pcie2usb]" fmt, ## args); } while (0)
#define pcie2usb_warn(fmt, args...) do { printk(KERN_WARNING "[pcie2usb]" fmt, ## args); } while (0)
#define pcie2usb_err(fmt, args...)  do { printk(KERN_ERR     "[pcie2usb]" fmt, ## args); } while (0)
#endif

#endif
