#ifndef _LINUX_HW_USB_HUB_H
#define _LINUX_HW_USB_HUB_H


#define USB_HUB_DEBUG

#ifndef USB_HUB_DEBUG
#define usbhub_debug(fmt, args...)do {} while (0)
#define usbhub_info(fmt, args...) do {} while (0)
#define usbhub_warn(fmt, args...) do {} while (0)
#define usbhub_err(fmt, args...)  do {} while (0)
#else
#define usbhub_debug(fmt, args...)do { printk(KERN_DEBUG   "[usb_hub]" fmt, ## args); } while (0)
#define usbhub_info(fmt, args...) do { printk(KERN_INFO    "[usb_hub]" fmt, ## args); } while (0)
#define usbhub_warn(fmt, args...) do { printk(KERN_WARNING "[usb_hub]" fmt, ## args); } while (0)
#define usbhub_err(fmt, args...)  do { printk(KERN_ERR     "[usb_hub]" fmt, ## args); } while (0)
#endif

#endif
