
#ifndef _DWC3_HISI_H_
#define _DWC3_HISI_H_

#include <linux/clk.h>
#include <linux/completion.h>
#include <linux/dcache.h>
#include <linux/hisi/usb/hisi_usb.h>
#include <linux/kfifo.h>
#include <linux/platform_device.h>
#include <linux/pm_wakeup.h>
#include <linux/regulator/consumer.h>

/*
 * hisi dwc3 otg bc registers
 */
#define USBOTG3_CTRL0		0x00		/* ѡ����ƼĴ���0 */
#define USBOTG3_CTRL1		0x04		/* ѡ����ƼĴ���1 */
#define USBOTG3_CTRL2		0x08		/* ѡ����ƼĴ���2 */
#define USBOTG3_CTRL3		0x0C		/* ѡ����ƼĴ���3 */
#define USBOTG3_CTRL4		0x10		/* ѡ����ƼĴ���4 */
#define USBOTG3_CTRL5		0x14		/* ѡ����ƼĴ���5 */
#define USBOTG3_CTRL6		0x18		/* ѡ����ƼĴ���6 */
#define USBOTG3_CTRL7		0x1C		/* ѡ����ƼĴ���7 */
#define USBOTG3_STS0		0x20		/* ״̬�Ĵ���0 */
#define USBOTG3_STS1		0x24		/* ״̬�Ĵ���1 */
#define USBOTG3_STS2		0x28		/* ״̬�Ĵ���2 */
#define USBOTG3_STS3		0x2C		/* ״̬�Ĵ���3 */
#define RAM_CTRL		0x40		/* RAM���ƼĴ��� */
#define USBOTG3_STS4		0x44		/* ״̬�Ĵ���4 */
#define USB3PHY_CTRL		0x48		/* PHY���ƼĴ��� */
#define USB3PHY_STS		0x4C		/* PHY״̬�Ĵ��� */
#define USB3PHY_CR_STS		0x50		/* PHY�ڲ��Ĵ���״̬ */
#define USB3PHY_CR_CTRL		0x54		/* PHY�ڲ��Ĵ������� */
#define USB3_RES		0x58		/* USBԤ���Ĵ��� */

/* USTOTG3_CTRL0 */
# define USBOTG3CTRL0_SESSVLD_SEL              (1 << 14)
# define USBOTG3CTRL0_SC_SESSVLD               (1 << 13)
# define USBOTG3CTRL0_POWERPRESENT_SEL         (1 << 12)
# define USBOTG3CTRL0_SC_POWERPRESENT          (1 << 11)
# define USBOTG3CTRL0_BVALID_SEL               (1 << 10)
# define USBOTG3CTRL0_SC_BVALID                (1 << 9)
# define USBOTG3CTRL0_AVALID_SEL               (1 << 8)
# define USBOTG3CTRL0_SC_AVALID                (1 << 7)
# define USBOTG3CTRL0_VBUSVALID_SEL            (1 << 6)
# define USBOTG3CTRL0_DRVVBUS                  (1 << 5)
# define USBOTG3CTRL0_DRVVBUS_SEL              (1 << 4)
# define USBOTG3CTRL0_IDDIG                    (1 << 3)
# define USBOTG3CTRL0_IDDIG_SEL                (1 << 2)
# define USBOTG3CTRL0_IDPULLUP                 (1 << 1)
# define USBOTG3CTRL0_IDPULLUP_SEL             (1 << 0)

/* USTOTG3_CTRL2 */
# define USBOTG3CTRL2_POWERDOWN_HSP             (1 << 0)
# define USBOTG3CTRL2_POWERDOWN_SSP             (1 << 1)

/* USBOTG3_CTRL3 */
# define USBOTG3_CTRL3_VBUSVLDEXT	(1 << 6)
# define USBOTG3_CTRL3_VBUSVLDEXTSEL	(1 << 5)
# define USBOTG3_CTRL3_TXBITSTUFFEHN	(1 << 4)
# define USBOTG3_CTRL3_TXBITSTUFFEN	(1 << 3)
# define USBOTG3_CTRL3_RETENABLEN	(1 << 2)
# define USBOTG3_CTRL3_OTGDISABLE	(1 << 1)
# define USBOTG3_CTRL3_COMMONONN	(1 << 0)

/* USBOTG3_CTRL4 */
# define USBOTG3_CTRL4_TXVREFTUNE(x)            (((x) << 22) & (0xf << 22))
# define USBOTG3_CTRL4_TXRISETUNE(x)            (((x) << 20) & (3 << 20))
# define USBOTG3_CTRL4_TXRESTUNE(x)             (((x) << 18) & (3 << 18))
# define USBOTG3_CTRL4_TXPREEMPPULSETUNE        (1 << 17)
# define USBOTG3_CTRL4_TXPREEMPAMPTUNE(x)       (((x) << 15) & (3 << 15))
# define USBOTG3_CTRL4_TXHSXVTUNE(x)            (((x) << 13) & (3 << 13))
# define USBOTG3_CTRL4_TXFSLSTUNE(x)            (((x) << 9) & (0xf << 9))
# define USBOTG3_CTRL4_SQRXTUNE(x)              (((x) << 6) & (7 << 6))
# define USBOTG3_CTRL4_OTGTUNE_MASK             (7 << 3)
# define USBOTG3_CTRL4_OTGTUNE(x)               (((x) << 3) & USBOTG3_CTRL4_OTGTUNE_MASK)
# define USBOTG3_CTRL4_COMPDISTUNE_MASK         7
# define USBOTG3_CTRL4_COMPDISTUNE(x)           ((x) & USBOTG3_CTRL4_COMPDISTUNE_MASK)

# define USBOTG3_CTRL7_REF_SSP_EN				(1 << 16)

/* USBOTG3_CTRL6 */
#define TX_VBOOST_LVL_MASK			7
#define TX_VBOOST_LVL(x)			((x) & TX_VBOOST_LVL_MASK)

/* USBPHY vboost lvl default value */
#define VBOOST_LVL_DEFAULT_PARAM	(5)

#define usb_dbg(format, arg...)    \
	do {                 \
		printk(KERN_DEBUG "[USB3][%s]"format, __func__, ##arg); \
	} while (0)

#define usb_info(format, arg...)    \
	do {                 \
		printk(KERN_INFO "[USB3][%s]"format, __func__, ##arg); \
	} while (0)

#define usb_err(format, arg...)    \
	do {                 \
		printk(KERN_ERR "[USB3][%s]"format, __func__, ##arg); \
	} while (0)

enum usb_power_state {
	USB_POWER_OFF = 0,
	USB_POWER_ON = 1,
	USB_POWER_HOLD = 2,
};

enum usb_connect_state {
	USB_CONNECT_DCP = 0xEE,	/* connect a charger */
	USB_CONNECT_HOST = 0xFF,	/* usb state change form hifi to host */
};

#define MAX_HISI_USB_EVENT_COUNT 16

struct hisi_dwc3_device {
	struct platform_device *pdev;

	enum usb_state state;
	enum hisi_charger_type charger_type;
	enum hisi_charger_type fake_charger_type;

	DECLARE_KFIFO(event_fifo, struct hisi_usb_event,
		      MAX_HISI_USB_EVENT_COUNT);
	enum otg_dev_event_type last_event;
	spinlock_t event_lock;

	/* save time stamp, for time consuming checking */
	unsigned long start_device_time_stamp;
	unsigned long start_host_time_stamp;
	unsigned long stop_host_time_stamp;
	unsigned long start_hifiusb_time_stamp;
	unsigned long stop_hifiusb_time_stamp;
	unsigned long start_device_complete_time_stamp;
	unsigned long start_host_complete_time_stamp;
	unsigned long stop_host_complete_time_stamp;
	unsigned long start_hifiusb_complete_time_stamp;
	unsigned long stop_hifiusb_complete_time_stamp;

	unsigned long device_add_time_stamp;

	struct mutex lock;
	struct wakeup_source wake_lock;
	struct work_struct event_work;
	struct work_struct speed_change_work;

	u32 eye_diagram_param;	/* this param will be set to USBOTG3_CTRL4 */
	u32 eye_diagram_host_param;
	u32 usb3_phy_tx_vboost_lvl;

	u32 fpga_flag;
	int fpga_usb_mode_gpio;
	int fpga_otg_drv_vbus_gpio;
	int fpga_phy_reset_gpio;
	int fpga_phy_switch_gpio;

	int eventmask;
	u32 dma_mask_bit;

	/* for bc again */
	u32 bc_again_flag;
	u32 bc_unknown_again_flag;
	unsigned int bc_again_delay_time;
	spinlock_t bc_again_lock;
	struct delayed_work bc_again_work;
	struct notifier_block event_nb;
	struct notifier_block xhci_nb;
	unsigned int vdp_src_enable;

	u32 support_dp;
	struct usb3_core_ops *core_ops;
	const struct hisi_usb_phy *usb_phy;

	bool use_new_frame;
	struct phy *usb2_phy;
	struct phy *usb3_phy;
	struct clk **controller_clks;
	int num_controller_clks;
	struct hisi_usb_reg_cfg *controller_reset;
	struct hisi_usb_reg_cfg *controller_unreset;

	bool notify_speed;
	unsigned int speed;
	unsigned int suspend_error_flag;
	unsigned quirk_enable_hst_imm_retry:1;
	unsigned quirk_disable_rx_thres_cfg:1;
	unsigned quirk_disable_usb2phy_suspend:1;
	unsigned quirk_clear_svc_opp_per_hs:1;
	unsigned quirk_set_svc_opp_per_hs_sep:1;
	unsigned quirk_adjust_dtout:1;
	unsigned quirk_force_disable_host_lpm:1;
	unsigned quirk_enable_p4_gate:1;
	unsigned quirk_keep_u2_power_suspend:1;
	unsigned int hifi_ip_first;
	TYPEC_PLUG_ORIEN_E plug_orien;
	TCPC_MUX_CTRL_TYPE mode_type;

	struct extcon_dev *edev;
	u32 vdp_src_disable;
	bool usb_support_s3_wakeup;

	struct dentry *debug_root;
};

struct hisi_usb_phy {
	void __iomem *otg_bc_reg_base;

	int (*init)(unsigned int support_dp, unsigned int eye_diagram_param,
		unsigned int usb3_phy_tx_vboost_lvl);
	int (*shutdown)(unsigned int support_dp);
	int (*shared_phy_init)(unsigned int support_dp, unsigned int eye_diagram_param,
			unsigned int combophy_flag);
	int (*shared_phy_shutdown)(unsigned int support_dp,
			unsigned int combophy_flag, unsigned int keep_power);
	void (*notify_speed)(unsigned int speed);
	void (*disable_usb3)(void);
	int (*get_term)(u32 *term);
	int (*set_term)(u32 term);
	void __iomem *(*get_bc_ctrl_reg)(void);
};

void hisi_usb_unreset_phy_if_fpga(void);
void hisi_usb_switch_sharedphy_if_fpga(int to_hifi);
int hisi_dwc3_is_powerdown(void);
enum usb_device_speed hisi_dwc3_get_dt_host_maxspeed(void);

/*
 * hisi usb
 */
extern struct hisi_dwc3_device *hisi_dwc3_dev;
void hisi_dwc3_platform_host_quirks(void);
void hisi_dwc3_platform_device_quirks(void);
int hisi_dwc3_is_fpga(void);
void hisi_dwc3_wake_lock(struct hisi_dwc3_device *hisi_dwc3);
void hisi_dwc3_wake_unlock(struct hisi_dwc3_device *hisi_dwc3);
void dwc3_core_disable_pipe_clock(void);
int dwc3_core_enable_u3(void);
int hisi_usb_dwc3_register_phy(const struct hisi_usb_phy *phy);
int hisi_usb_dwc3_unregister_phy(const struct hisi_usb_phy *phy);

#endif /* _DWC3_HISI_H_ */
