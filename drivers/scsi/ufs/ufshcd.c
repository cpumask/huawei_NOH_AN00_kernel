/*
 * Universal Flash Storage Host controller driver Core
 *
 * This code is based on drivers/scsi/ufs/ufshcd.c
 * Copyright (C) 2011-2013 Samsung India Software Operations
 * Copyright (c) 2013-2016, The Linux Foundation. All rights reserved.
 *
 * Authors:
 *	Santosh Yaraganavi <santosh.sy@samsung.com>
 *	Vinayak Holikatti <h.vinayak@samsung.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * See the COPYING file in the top-level directory or visit
 * <http://www.gnu.org/licenses/gpl-2.0.html>
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * This program is provided "AS IS" and "WITH ALL FAULTS" and
 * without warranty of any kind. You are solely responsible for
 * determining the appropriateness of using and distributing
 * the program and assume all risks associated with your exercise
 * of rights with respect to the program, including but not limited
 * to infringement of third party rights, the risks and costs of
 * program errors, damage to or loss of data, programs or equipment,
 * and unavailability or interruption of operations. Under no
 * circumstances will the contributor of this Program be liable for
 * any damages of any kind arising from your use or distribution of
 * this program.
 *
 * The Linux Foundation chooses to take subject only to the GPLv2
 * license terms, and distributes only under these terms.
 */

#define pr_fmt(fmt) "ufshcd :" fmt

#include <linux/async.h>
#include <linux/devfreq.h>
#include <linux/nls.h>
#include <linux/of.h>
#include <linux/blkdev.h>
#include <linux/bootdevice.h>
#include <scsi/ufs/ufs.h>
#include <scsi/scsi_device.h>
#include <scsi/scsi_host.h>
#include <scsi/scsi_devinfo.h>
#include <trace/events/block.h>
#include <linux/sched.h>
#include <trace/events/scsi.h>
#include <linux/kthread.h>
#include <linux/thermal.h>
#include <linux/sched.h>
#include <linux/hisi/rpmb.h>

#include <linux/cpu.h>
#include <linux/delay.h>
#include <asm/unaligned.h>

#include "ufshcd.h"
#include "ufs-kirin.h"
#include "unipro.h"
#include "ufs-hisi.h"
#include "ufs-hpb.h"

#define CREATE_TRACE_POINTS
#include <trace/events/ufs.h>

#include "ufs-quirks.h"
#include "ufs_debugfs.h"
#include "dsm_ufs.h"
#include "ufs_trace.h"
#include "ufs-vendor-mode.h"
#include "ufs-kirin-lib.h"
#include "ufs-hisi-hci.h"
#include "ufs-hisi-dump.h"
#include "ufshcd-kirin-interface.h"
#ifdef CONFIG_HISI_BOOTDEVICE
#include "ufs-rpmb.h"
#endif

#include <linux/hisi/pagecache_debug.h>

#ifdef CONFIG_HUAWEI_STORAGE_ROFA
#include <chipset_common/storage_rofa/storage_rofa.h>
#endif

#ifdef CONFIG_SCSI_UFS_ENHANCED_INLINE_CRYPTO_V3
#include <linux/hisi/hisi_fbe_ctrl.h>
#include <linux/fscrypt_common.h>
#endif
#ifdef CONFIG_HUAWEI_DSM_IOMT_UFS_HOST
#include <linux/iomt_host/dsm_iomt_ufs_host.h>
#endif
#define UFSHCD_ENABLE_INTRS	(UTP_TRANSFER_REQ_COMPL |\
				 UTP_TASK_REQ_COMPL |\
				 UTP_ERROR |\
				 UFSHCD_ERROR_MASK)
/* UIC command timeout, unit: ms */
#define UIC_CMD_TIMEOUT	500

/* NOP OUT retries waiting for NOP IN response */
#define NOP_OUT_RETRIES    3
/* Timeout after 200 msecs if NOP OUT hangs without response */
#define NOP_OUT_TIMEOUT    200 /* msecs */

/* Query request retries */
#define QUERY_REQ_RETRIES 3
/* Query request timeout */
#define QUERY_REQ_TIMEOUT 1500 /* 1.5 seconds */
/*
 * Query request timeout for fDeviceInit flag
 * fDeviceInit query response time for some devices is too large that default
 * QUERY_REQ_TIMEOUT may not be enough for such devices.
 */
#define QUERY_FDEVICEINIT_REQ_TIMEOUT 1600 /* msec */

/* Task management command timeout */
#define TM_CMD_TIMEOUT	100 /* msecs */

/* maximum number of retries for a general UIC command  */
#define UFS_UIC_COMMAND_RETRIES 3

/* maximum number of link-startup retries */
#define DME_LINKSTARTUP_RETRIES 3

/* Expose the flag value from utp_upiu_query.value */
#define MASK_QUERY_UPIU_FLAG_LOC 0xFF

/* Interrupt aggregation default timeout, unit: 40us */
#define INT_AGGR_DEF_TO	0x02
#define ufshcd_toggle_vreg(_dev, _vreg, _on)				\
	({                                                              \
		int _ret;                                               \
		if (_on)                                                \
			_ret = ufshcd_enable_vreg(_dev, _vreg);         \
		else                                                    \
			_ret = ufshcd_disable_vreg(_dev, _vreg);        \
		_ret;                                                   \
	})

enum {
	UFSHCD_MAX_CHANNEL	= 0,
	UFSHCD_MAX_ID		= 1,
#ifdef CONFIG_HISI_UFS_HC_CORE_UTR
	UFSHCD_CMD_PER_LUN	= 64,
	UFSHCD_CAN_QUEUE	= 64,
#else
	UFSHCD_CMD_PER_LUN	= 32,
	UFSHCD_CAN_QUEUE	= 32,
#endif
};

/* UFSHCD UIC layer error flags */
enum {
	UFSHCD_UIC_DL_PA_INIT_ERROR = (1 << 0), /* Data link layer error */
	UFSHCD_UIC_DL_NAC_RECEIVED_ERROR = (1 << 1), /* Data link layer error */
	UFSHCD_UIC_DL_TCx_REPLAY_ERROR = (1 << 2), /* Data link layer error */
	UFSHCD_UIC_NL_ERROR = (1 << 3), /* Network layer error */
	UFSHCD_UIC_TL_ERROR = (1 << 4), /* Transport Layer error */
	UFSHCD_UIC_DME_ERROR = (1 << 5), /* DME error */
};

/* Interrupt configuration options */
enum {
	UFSHCD_INT_DISABLE,
	UFSHCD_INT_ENABLE,
	UFSHCD_INT_CLEAR,
};

#define ufshcd_set_ufs_dev_active(h) \
	((h)->curr_dev_pwr_mode = UFS_ACTIVE_PWR_MODE)
#define ufshcd_set_ufs_dev_poweroff(h) \
	((h)->curr_dev_pwr_mode = UFS_POWERDOWN_PWR_MODE)
#define ufshcd_is_ufs_dev_active(h) \
	((h)->curr_dev_pwr_mode == UFS_ACTIVE_PWR_MODE)
#define ufshcd_is_ufs_dev_poweroff(h) \
	((h)->curr_dev_pwr_mode == UFS_POWERDOWN_PWR_MODE)

static struct ufs_pm_lvl_states ufs_pm_lvl_states[] = {
	{UFS_ACTIVE_PWR_MODE, UIC_LINK_ACTIVE_STATE},
	{UFS_ACTIVE_PWR_MODE, UIC_LINK_HIBERN8_STATE},
	{UFS_SLEEP_PWR_MODE, UIC_LINK_ACTIVE_STATE},
	{UFS_SLEEP_PWR_MODE, UIC_LINK_HIBERN8_STATE},
	{UFS_POWERDOWN_PWR_MODE, UIC_LINK_HIBERN8_STATE},
	{UFS_POWERDOWN_PWR_MODE, UIC_LINK_OFF_STATE},
};

static inline enum ufs_dev_pwr_mode
ufs_get_pm_lvl_to_dev_pwr_mode(enum ufs_pm_level lvl)
{
	return ufs_pm_lvl_states[lvl].dev_state;
}

static inline enum uic_link_state
ufs_get_pm_lvl_to_link_pwr_state(enum ufs_pm_level lvl)
{
	return ufs_pm_lvl_states[lvl].link_state;
}

static inline enum ufs_pm_level
ufs_get_desired_pm_lvl_for_dev_link_state(enum ufs_dev_pwr_mode dev_state,
					enum uic_link_state link_state)
{
	enum ufs_pm_level lvl;

	for (lvl = UFS_PM_LVL_0; lvl < UFS_PM_LVL_MAX; lvl++) {
		if ((ufs_pm_lvl_states[lvl].dev_state == dev_state) &&
			(ufs_pm_lvl_states[lvl].link_state == link_state))
			return lvl;
	}

	/* if no match found, return the level 0 */
	return UFS_PM_LVL_0;
}

static struct ufs_dev_fix ufs_fixups[] = {
	/* UFS cards deviations table */

	END_FIX
};

static void ufshcd_tmc_handler(struct ufs_hba *hba);
static void ufshcd_async_scan(void *data, async_cookie_t cookie);
static int ufshcd_reset_and_restore(struct ufs_hba *hba);
static int ufshcd_clear_tm_cmd(struct ufs_hba *hba, int tag);
static void ufshcd_hba_exit(struct ufs_hba *hba);
static int ufshcd_probe_hba(struct ufs_hba *hba);
static int __ufshcd_setup_clocks(struct ufs_hba *hba, bool on,
				 bool skip_ref_clk);
static int ufshcd_setup_clocks(struct ufs_hba *hba, bool on);
static int ufshcd_set_vccq_rail_unused(struct ufs_hba *hba, bool unused);
static int ufshcd_uic_hibern8_exit(struct ufs_hba *hba);
static int ufshcd_uic_hibern8_enter(struct ufs_hba *hba);
static inline void ufshcd_add_delay_before_dme_cmd(struct ufs_hba *hba);
static int ufshcd_host_reset_and_restore(struct ufs_hba *hba);
static void ufshcd_resume_clkscaling(struct ufs_hba *hba);
static void ufshcd_suspend_clkscaling(struct ufs_hba *hba);
static void __ufshcd_suspend_clkscaling(struct ufs_hba *hba);
static int ufshcd_scale_clks(struct ufs_hba *hba, bool scale_up);
#ifndef CONFIG_SCSI_UFS_INTR_HUB
static irqreturn_t ufshcd_intr(int irq, void *__hba);
#else
irqreturn_t ufshcd_intr(int irq, void *__hba);
#endif
static int ufshcd_config_pwr_mode(struct ufs_hba *hba,
		struct ufs_pa_layer_attr *desired_pwr_mode);
int ufshcd_change_power_mode(struct ufs_hba *hba,
			     struct ufs_pa_layer_attr *pwr_mode);
void ufs_get_geometry_info(struct ufs_hba *hba);
void ufs_get_device_health_info(struct ufs_hba *hba);
static int ufshcd_enable_ee(struct ufs_hba *hba, u16 mask);
static int ufshcd_disable_ee(struct ufs_hba *hba, u16 mask);
static void ufshpb_compose_upiu(struct ufs_hba *hba, struct ufshcd_lrb *lrbp);
/********************************************************************/
/****************************add by hisi,begin***********************/
/********************************************************************/
#ifdef CONFIG_HISI_UFS_MANUAL_BKOPS
#include "hisi-ufs-bkops.h"
#endif
#ifdef CONFIG_HISI_DEBUG_FS
#define HISI_UFS_BUG_ON(x) 	do {BUG_ON(x);} while(0)
#define HISI_UFS_BUG() 		do {BUG();} while(0)
#else
#define HISI_UFS_BUG_ON(x) 	do {} while(0)
#define HISI_UFS_BUG() 		do {} while(0)
#endif

#if (defined CONFIG_SCSI_UFS_GEMINI || defined CONFIG_SCSI_UFS_ARIES || \
	defined CONFIG_SCSI_UFS_LIBRA)
#define OLD_DEVICE_CONSTRAINT
#endif

/* HI1861 FSR info default size, 4k byte*/
#define HI1861_FSR_INFO_SIZE 4096
/* default value of auto suspend is 3000ms*/
/*#define UFSHCD_AUTO_SUSPEND_DELAY_MS 3000*/

#if defined(CONFIG_SCSI_UFS_HI1861_VCMD) && defined(CONFIG_HISI_DIEID)
static u8 *ufs_hixxxx_dieid;
static int is_fsr_read_failed;
#define UFS_CONTROLLER_DIEID_SIZE 32
#define UFS_FLASH_DIE_ID_SIZE 128
#define UFS_DIEID_NUM_SIZE 4
#define UFS_DIEID_NUM_SIZE_THOR920 1
#define UFS_DCIP_CRACK_EVER_SIZE 1
#define UFS_DCIP_CRACK_NOW_SIZE 1
#define UFS_NAND_CHIP_VER_SIZE 8
#define UFS_DIEID_TOTAL_SIZE 174
#define UFS_DIEID_BUFFER_SIZE 800
#define UFS_DIEID_CHIP_VER_OFFSET 4
#define UFS_DIEID_CONTROLLER_OFFSET 12
#define UFS_DIEID_FLASH_OFFSET 44
#define UFS_DCIP_CRACK_NOW_OFFSET 173
#define UFS_DCIP_CRACK_EVER_OFFSET 174
#define UFS_FLASH_VENDOR_T 0x98
#define UFS_FLASH_VENDOR_M 0x2c
#define UFS_FLASH_VENDOR_H 0xad
#endif

/*disable auto bkops on kirin*/
/*#define FEATURE_UFS_AUTO_BKOPS*/

/* define the max hardware slots for each CPU */
#define UFS_MQ_MAX_HARDWARE_SLOTS_PER_CPU 8

static struct wakeup_source ffu_lock;

/* DSM_UFS_TIMEOUT_SERIOUS */
static int ufs_timeout_lock = 0;
static int ufs_timeout_count = 0;
#define UFS_TIMEOUT_SERIOUS_THRESHOLD  3

#define ufshcd_hex_dump(prefix_str, buf, len)                                  \
	print_hex_dump(KERN_ERR, prefix_str, DUMP_PREFIX_OFFSET, 16, 4, buf,   \
		len, false)

static u32 ufs_query_desc_max_size[] = {
	QUERY_DESC_DEVICE_MAX_SIZE,
	QUERY_DESC_CONFIGURAION_MAX_SIZE,
	QUERY_DESC_UNIT_MAX_SIZE,
	QUERY_DESC_RFU_MAX_SIZE,
	QUERY_DESC_INTERCONNECT_MAX_SIZE,
	QUERY_DESC_STRING_MAX_SIZE,
	QUERY_DESC_RFU_MAX_SIZE,
	QUERY_DESC_GEOMETRY_MAX_SIZE,
	QUERY_DESC_POWER_MAX_SIZE,
	QUERY_DESC_HEALTH_MAX_SIZE,
	QUERY_DESC_RFU_MAX_SIZE,
};

#ifdef CONFIG_SCSI_UFS_HI1861_VCMD
struct remap_mapping{
	unsigned int size;
	unsigned int old_addr;
	unsigned int new_addr;
};
#endif

#ifdef CONFIG_HISI_SCSI_UFS_DUMP
extern unsigned int ufs_dump;
#endif

#define UFS_KIRIN_H8_TIMEOUT_RETRY_TIMES	2000
#define UFS_KIRIN_H8_TIMEOUT_RETRY_UDELAY	100
#define UFS_KIRIN_H8_OP_ENTER		true
#define UFS_KIRIN_H8_OP_EXIT		false

static int ufshcd_eh_host_reset_handler(struct scsi_cmnd *cmd);
static int ufshcd_get_max_pwr_mode(struct ufs_hba *hba);
static int ufshcd_verify_dev_init(struct ufs_hba *hba, int retries);
static void ufshcd_transfer_req_compl(struct ufs_hba *hba);
void ufshcd_init_pwr_info(struct ufs_hba *hba);
int ufshcd_read_fsr(struct ufs_hba *hba, u8 *buf, u32 size);
static inline void ufshcd_start_delay_work(struct ufs_hba *hba, struct scsi_cmnd *cmd);
static void ufshcd_print_host_state(struct ufs_hba *hba);
static void ufshcd_print_pwr_info(struct ufs_hba *hba);
static void ufshcd_print_host_regs(struct ufs_hba *hba);
static int __ufshcd_uic_hibern8_op_irq_safe(struct ufs_hba *hba, bool h8_op);
void ufshcd_config_adapt(struct ufs_hba *hba, struct ufs_pa_layer_attr *pwr_mode);
static void uhshcd_rsp_sense_data(struct ufs_hba *hba, struct ufshcd_lrb *lrbp, int scsi_status);
static void ufshcd_print_trs(struct ufs_hba *hba, unsigned long bitmap, bool pr_prdt);
static void __ufshcd_enable_dev_tmt_cnt(struct ufs_hba *hba);
static void ufshcd_err_handler_do_reset(struct ufs_hba *hba, bool err_xfer, bool err_tm, unsigned long *flags);
static void ufshcd_update_uic_reg_hist(struct ufs_uic_err_reg_hist *reg_hist, u32 reg);
static bool ufshcd_error_need_queue_work(struct ufs_hba *hba);
static void ufshcd_print_tmrs(struct ufs_hba *hba, unsigned long bitmap);
static void ufshcd_set_req_abort_skip(struct ufs_hba *hba, unsigned long bitmap);
static void ufshcd_disable_dev_tmt_cnt(struct ufs_hba *hba);
static void ufshcd_disable_pwm_cnt(struct ufs_hba *hba);
void ufs_idle_intr_toggle(struct ufs_hba *hba, int enable);
static void ufshcd_disable_auto_hibern8(struct ufs_hba *hba);
static void ufshcd_enable_auto_hibern8(struct ufs_hba *hba);
static void ufshcd_enable_pwm_cnt(struct ufs_hba *hba);
static void ufshcd_enable_dev_tmt_cnt(struct ufs_hba *hba);
static void ufshcd_clear_dbg_ufs_stats(struct ufs_hba *hba);
static void ufshcd_device_not_support_autoh8(struct ufs_hba *hba);
static inline void ufshcd_ue_clean(struct ufs_hba *hba);
static int ufshcd_ue_get(struct ufs_hba *hba);
static int ufshcd_send_scsi_sync_cache(struct ufs_hba *hba, struct scsi_device *sdp);
static int ufshcd_send_scsi_request_sense(struct ufs_hba *hba,
		struct scsi_device *sdp, unsigned int timeout, bool eh_handle);
static int ufshcd_send_scsi_ssu(struct ufs_hba *hba, struct scsi_device *sdp, unsigned char *cmd,
		unsigned int timeout, struct scsi_sense_hdr *sshdr);
static int ufshcd_send_scsi_sync_cache_init(void);
void ufshcd_init_ufs_temp_sys(struct ufs_hba *hba);
static void ufshcd_add_idle_intr_check_timer(struct ufs_hba *hba);
static void ufshcd_send_scsi_sync_cache_deinit(void);
#ifdef CONFIG_HISI_BOOTDEVICE
static int ufshcd_device_get_rev(const struct device *dev, char *rev);
#endif
static void ufshcd_dump_status(struct Scsi_Host *host, enum blk_dump_scene dump_type);
static int ufshcd_direct_flush(struct scsi_device *sdev);
void ufs_ffu_pm_runtime_delay_enable(struct work_struct *work);
static irqreturn_t ufshcd_timeout_handle_intr(int timer_irq, void *__hba);
static ssize_t config_desc_show(struct device *dev, struct device_attribute *attr, char *buf);
static ssize_t config_desc_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);
static ssize_t unique_number_show(struct device *dev, struct device_attribute *attr, char *buf);
static ssize_t man_id_show(struct device *dev, struct device_attribute *attr, char *buf);
static ssize_t shared_alloc_units_show(struct device *dev,
				       struct device_attribute *attr,
				       char *buf);
static ssize_t spec_version_show(struct device *dev, struct device_attribute *attr, char *buf);
#ifdef CONFIG_SCSI_UFS_INLINE_CRYPTO
static void ufshcd_prepare_req_desc_uie(struct ufs_hba *hba, struct ufshcd_lrb *lrbp);
static int ufshcd_hba_uie_init(struct ufs_hba *hba);
#endif

#ifdef CONFIG_HISI_SCSI_UFS_DUMP
static inline void ufshcd_dump_scsi_command(struct ufs_hba *hba, unsigned int task_tag);
#endif


#ifdef CONFIG_SCSI_UFS_HI1861_VCMD
void ufshcd_fsr_dump_handler(struct work_struct *work);
static void ufshcd_init_fsr_sys(struct ufs_hba *hba);

#ifdef CONFIG_HISI_DIEID
#define UFS_HIXXXX_PRODUCT_NAME                 "SS6100GBCV100"
#define HIXXXX_PROD_LEN                         (13)
static void ufshcd_ufs_set_dieid(struct ufs_hba *hba, struct ufs_dev_desc *dev_desc);
#endif
#endif

#ifdef CONFIG_SCSI_UFS_HS_ERROR_RECOVER
void ufs_recover_hs_mode(struct work_struct *work);
void ufshcd_check_init_mode(struct ufs_hba *hba, int err);
#endif
#ifdef CONFIG_SCSI_UFS_HI1861_VCMD
static void ufshcd_device_capbitlity_config(struct ufs_hba *hba);
#endif
#ifdef CONFIG_HYPERHOLD_CORE
static int ufshcd_get_health_info(struct scsi_device *sdev,
	u8 *pre_eol_info, u8 *life_time_est_a, u8 *life_time_est_b);
#endif

static void ufshcd_custom_upiu_build(struct ufs_hba *hba,
				     struct ufshcd_lrb *lrbp);
static void ufshcd_update_dev_cmd_lun(struct ufs_hba *hba,
				      struct ufshcd_lrb *lrbp);
static void hisi_ufs_parse_dev_desc(struct ufs_hba *hba,
				    struct ufs_dev_desc *dev_desc,
				    const u8 *desc_buf);
static void ufshcd_complete_time(struct ufs_hba *hba, struct ufshcd_lrb *lrbp,
				 uint8_t opcode);

DEVICE_ATTR_RW(config_desc);
DEVICE_ATTR_RO(unique_number);
DEVICE_ATTR_RO(man_id);
DEVICE_ATTR_RO(spec_version);
DEVICE_ATTR_RO(shared_alloc_units);
struct device_attribute *ufs_device_attrs[] = {
	&dev_attr_unique_number,      &dev_attr_config_desc,  &dev_attr_man_id,
	&dev_attr_shared_alloc_units, &dev_attr_spec_version, NULL
};
static void ufshcd_device_need_enable_autobkops(struct ufs_hba *hba);
static void ufshcd_check_disable_dev_tmt_cnt(struct ufs_hba *hba,
					     struct scsi_cmnd *cmd);
#ifdef CONFIG_SCSI_UFS_LUN_PROTECT
static int ufshcd_set_lun_protect(struct ufs_hba *hba);
#endif

/********************************************************************/
/*****************************add by hisi,end************************/
/********************************************************************/
static inline bool ufshcd_valid_tag(struct ufs_hba *hba, int tag)
{
	return tag >= 0 && tag < hba->nutrs;
}

#define UFS_ATTRIBUTE(_name, _uname)                                           \
	static ssize_t _name##_show(struct device *dev,                        \
				    struct device_attribute *attr, char *buf)  \
	{                                                                      \
		struct Scsi_Host *shost = class_to_shost(dev);                 \
		struct ufs_hba *hba = shost_priv(shost);                       \
		u32 value;                                                     \
		if (ufshcd_query_attr(hba, UPIU_QUERY_OPCODE_READ_ATTR,        \
				      QUERY_ATTR_IDN##_uname, 0, 0, &value))   \
			return -EINVAL;                                        \
		return sprintf(buf, "0x%08X\n", value); /*lint !e421*/         \
	}                                                                      \
	static DEVICE_ATTR_RO(_name)

UFS_ATTRIBUTE(wb_flush_status, _WB_FLUSH_STATUS); /*lint !e421*/
UFS_ATTRIBUTE(wb_available_buffer_size, _WB_AVAILABLE_BUFFER_SIZE); /*lint !e421*/
UFS_ATTRIBUTE(wb_buffer_lifetime_est, _WB_BUFFER_LIFETIME_EST); /*lint !e421*/
UFS_ATTRIBUTE(wb_current_buffer_size, _WB_CURRENT_BUFFER_SIZE); /*lint !e421*/

#define UFS_FLAG(_name, _uname)                                                \
	static ssize_t _name##_show(struct device *dev,                        \
				    struct device_attribute *attr, char *buf)  \
	{                                                                      \
		bool flag;                                                     \
		struct Scsi_Host *shost = class_to_shost(dev);                 \
		struct ufs_hba *hba = shost_priv(shost);                       \
		if (ufshcd_query_flag(hba, UPIU_QUERY_OPCODE_READ_FLAG,        \
				      QUERY_FLAG_IDN##_uname, &flag))          \
			return -EINVAL;                                        \
		return sprintf(buf, "%s\n",                                    \
			       flag ? "true" : "false"); /*lint !e421*/        \
	}                                                                      \
	static DEVICE_ATTR_RO(_name)

UFS_FLAG(wb_en, _WB_EN); /*lint !e421*/
UFS_FLAG(wb_flush_en, _WB_BUFF_FLUSH_EN); /*lint !e421*/
UFS_FLAG(wb_flush_during_hibern8, _WB_BUFF_FLUSH_DURING_HIBERN8); /*lint !e421*/

static ssize_t wb_permanent_disable_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t len);
static ssize_t wb_permanent_disable_show(struct device *dev,
	struct device_attribute *attr, char *buf);
DEVICE_ATTR_RW(wb_permanent_disable);

static struct device_attribute *ufs_host_attrs[] = {
	&dev_attr_wb_en,
	&dev_attr_wb_flush_en,
	&dev_attr_wb_flush_during_hibern8,
	&dev_attr_wb_flush_status,
	&dev_attr_wb_available_buffer_size,
	&dev_attr_wb_buffer_lifetime_est,
	&dev_attr_wb_current_buffer_size,
	&dev_attr_wb_permanent_disable,
	NULL
};

static bool ufshcd_wb_allowed(struct ufs_hba *hba)
{
	return (hba->ufs_device_spec_version >= UFS_DEVICE_SPEC_3_1 ||
		       hba->ufs_device_spec_version == UFS_DEVICE_SPEC_2_2) &&
	       hba->caps & UFSHCD_CAP_WRITE_BOOSTER &&
	       hba->d_ext_ufs_feature_sup & UFS_DEV_WRITE_BOOSTER_SUP &&
	       hba->wb_shared_alloc_units;
}

static u8 ufshcd_wb_map_selector(struct ufs_hba *hba, enum query_opcode opcode,
				 u8 idn, u8 index, u8 selector)
{
	if (!hba->force_selector)
		return selector;

	if (opcode == UPIU_QUERY_OPCODE_READ_DESC ||
	    opcode == UPIU_QUERY_OPCODE_WRITE_DESC)
		if (idn == QUERY_DESC_IDN_DEVICE ||
		    idn == QUERY_DESC_IDN_CONFIGURATION ||
		    idn == QUERY_DESC_IDN_GEOMETRY)
			goto satisfied;

	if (opcode == UPIU_QUERY_OPCODE_SET_FLAG ||
	    opcode == UPIU_QUERY_OPCODE_CLEAR_FLAG ||
	    opcode == UPIU_QUERY_OPCODE_TOGGLE_FLAG ||
	    opcode == UPIU_QUERY_OPCODE_READ_FLAG)
		if (idn == QUERY_FLAG_IDN_WB_EN ||
		    idn == QUERY_FLAG_IDN_WB_BUFF_FLUSH_EN ||
		    idn == QUERY_FLAG_IDN_WB_BUFF_FLUSH_DURING_HIBERN8)
			goto satisfied;

	if (opcode == UPIU_QUERY_OPCODE_READ_ATTR ||
	    opcode == UPIU_QUERY_OPCODE_WRITE_ATTR)
		if (idn == QUERY_ATTR_IDN_EE_CONTROL ||
		    idn == QUERY_ATTR_IDN_EE_STATUS ||
		    idn == QUERY_ATTR_IDN_WB_FLUSH_STATUS ||
		    idn == QUERY_ATTR_IDN_WB_AVAILABLE_BUFFER_SIZE ||
		    idn == QUERY_ATTR_IDN_WB_BUFFER_LIFETIME_EST ||
		    idn == QUERY_ATTR_IDN_WB_CURRENT_BUFFER_SIZE)
			goto satisfied;

	return selector;
satisfied:
	return 1;
}

static int ufshcd_wb_ctrl(struct ufs_hba *hba, bool enable)
{
	int ret;
	enum query_opcode opcode;

	if (enable)
		opcode = UPIU_QUERY_OPCODE_SET_FLAG;
	else
		opcode = UPIU_QUERY_OPCODE_CLEAR_FLAG;

	ret = ufshcd_query_flag_retry(hba, opcode, QUERY_FLAG_IDN_WB_EN, NULL);
	if (ret) {
		dev_err(hba->dev, "%s write booster %s failed %d\n", __func__,
			enable ? "enable" : "disable", ret);
		return ret;
	}

	dev_info(hba->dev, "%s write booster %s %d\n", __func__,
		 enable ? "enable" : "disable", ret);

	return ret;
}

static int ufshcd_wb_auto_flush_ctrl(struct ufs_hba *hba, bool enable)
{
	int ret;
	enum query_opcode opcode;

	if (enable)
		opcode = UPIU_QUERY_OPCODE_SET_FLAG;
	else
		opcode = UPIU_QUERY_OPCODE_CLEAR_FLAG;

	ret = ufshcd_query_flag_retry(
		hba, opcode, QUERY_FLAG_IDN_WB_BUFF_FLUSH_DURING_HIBERN8, NULL);
	if (ret) {
		dev_err(hba->dev, "%s %s failed %d\n", __func__,
			enable ? "enable" : "disable", ret);
		return ret;
	}

	dev_dbg(hba->dev, "%s auto flush %s %d\n", __func__,
		enable ? "enable" : "disable", ret);
	return ret;
}

static int ufshcd_wb_type_detect(struct ufs_hba *hba)
{
	if (hba->wb_type != WB_SINGLE_SHARED) {
		dev_err(hba->dev, "dedicated write booster mode not support\n");
		/* Dedicated mode not implemented */
		return -EINVAL;
	}

	dev_info(hba->dev, "single shared write booster found\n");
	return 0;
}

static void ufshcd_host_force_reset_sync(struct ufs_hba *hba)
{
	unsigned long flags;

	spin_lock_irqsave(hba->host->host_lock, flags);

	hba->ufshcd_state = UFSHCD_STATE_EH_SCHEDULED;
	hba->force_host_reset = true;

	if (!kthread_queue_work(&hba->eh_worker, &hba->eh_work))
		dev_err(hba->dev, "%s: queue hba->eh_worker failed !\n",
			__func__);

	spin_unlock_irqrestore(hba->host->host_lock, flags);

	kthread_flush_work(&hba->eh_work);
}

static void ufshcd_wb_toggle_fn(struct work_struct *work)
{
	int ret;
	u32 current_buffer;
	u32 available;
	unsigned long flags;
	struct ufs_hba *hba;

	hba = container_of(work, struct ufs_hba, wb_work);

	dev_info(hba->dev, "%s: write booster toggle issued", __func__);

	pm_runtime_get_sync(hba->dev);

	if (ufshcd_wb_ctrl(hba, false) || ufshcd_wb_ctrl(hba, true))
		/*
		 * Wait error handler to complete, else caller like suspend
		 * would entering suspend while a error handler executing in
		 * another worker. After eh, wb would be reenabled.
		 */
		ufshcd_host_force_reset_sync(hba);

	/*
	 * Error handler doesn't need here. If we failed to check buffer
	 * size, following io would cause a new exception handler which
	 * let us try here again.
	 */
	ret = ufshcd_query_attr_retry(hba, UPIU_QUERY_OPCODE_READ_ATTR,
				      QUERY_ATTR_IDN_WB_CURRENT_BUFFER_SIZE, 0,
				      0, &current_buffer);
	if (ret)
		goto out;

	ret = ufshcd_query_attr_retry(hba, UPIU_QUERY_OPCODE_READ_ATTR,
				      QUERY_ATTR_IDN_WB_AVAILABLE_BUFFER_SIZE, 0,
				      0, &available);
	if (ret)
		goto out;

	/*
	 * Maybe, there are not enough space to alloc a new write buffer, if so,
	 * disable exception to avoid en endless notification.
	 */
	if (current_buffer < hba->wb_shared_alloc_units ||
	    available <= WB_T_BUFFER_THRESHOLD) {
		ret = ufshcd_disable_ee(hba, MASK_EE_WRITEBOOSTER_EVENT);
		if (!ret) {
			spin_lock_irqsave(hba->host->host_lock, flags);
			hba->wb_exception_enabled = false;
			spin_unlock_irqrestore(hba->host->host_lock, flags);
		}
	}

out:
	pm_runtime_put_sync(hba->dev);

	spin_lock_irqsave(hba->host->host_lock, flags);
	hba->wb_work_sched = false;
	spin_unlock_irqrestore(hba->host->host_lock, flags);
}

static int ufshcd_wb_ctrl_exception(struct ufs_hba *hba, bool enable)
{
	int ret;
	unsigned long flags;

	/*
	 * Some device can alloc a new write buffer by toggle write booster.
	 * We apply a exception handler func for and only for those devices.
	 *
	 * If new strategy was to implement, the hba->wb_work would be the good
	 * place to put new function to, ufshcd_exception_event_handler would
	 * handle them the same way.
	 */
	if (hba->manufacturer_id != UFS_VENDOR_TOSHIBA)
		return 0;

	if (enable)
		ret = ufshcd_enable_ee(hba, MASK_EE_WRITEBOOSTER_EVENT);
	else
		ret = ufshcd_disable_ee(hba, MASK_EE_WRITEBOOSTER_EVENT);

	if (ret) {
		dev_err(hba->dev,
			"%s: failed to enable wb exception event %d\n",
			__func__, ret);
	} else {
		spin_lock_irqsave(hba->host->host_lock, flags);
		hba->wb_exception_enabled = enable;
		spin_unlock_irqrestore(hba->host->host_lock, flags);

		dev_info(hba->dev, "%s: wb exception event %s\n",
			 __func__, enable ? "enable" : "disable");
	}

	return ret;
}

static inline int ufshcd_wb_enable_exception(struct ufs_hba *hba)
{
	return ufshcd_wb_ctrl_exception(hba, true);
}

static void ufshcd_set_wb_permanent_disable(struct ufs_hba *hba)
{
	unsigned long flags;

	spin_lock_irqsave(hba->host->host_lock, flags);
	hba->wb_permanent_disabled = true;
	spin_unlock_irqrestore(hba->host->host_lock, flags);
}

static bool ufshcd_wb_permanent_disable(struct ufs_hba *hba)
{
	unsigned long flags;
	bool wb_permanent_disabled = false;

	spin_lock_irqsave(hba->host->host_lock, flags);
	wb_permanent_disabled = hba->wb_permanent_disabled;
	spin_unlock_irqrestore(hba->host->host_lock, flags);

	return wb_permanent_disabled;
}

static bool ufshcd_get_wb_work_sched(struct ufs_hba *hba)
{
	unsigned long flags;
	bool wb_work_sched = false;

	spin_lock_irqsave(hba->host->host_lock, flags);
	wb_work_sched = hba->wb_work_sched;
	spin_unlock_irqrestore(hba->host->host_lock, flags);

	return wb_work_sched;
}

static inline int ufshcd_wb_disable_exception(struct ufs_hba *hba)
{
	return ufshcd_wb_ctrl_exception(hba, false);
}

static inline struct ufs_hba *ufshcd_dev_to_hba(struct device *dev)
{
	if (!dev)
		return NULL;

	return shost_priv(class_to_shost(dev));
}

static ssize_t wb_permanent_disable_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t len)
{
	int ret = 0;
	struct ufs_hba *hba = ufshcd_dev_to_hba(dev);

	if (!hba)
		return -EFAULT;

	pm_runtime_get_sync(hba->dev);
	ufshcd_set_wb_permanent_disable(hba);
	if (!ufshcd_wb_allowed(hba)) {
		dev_info(hba->dev, "%s wb not allowed\n", __func__);

		goto out;
	}

	ret = ufshcd_wb_disable_exception(hba);
	if (ret) {
		dev_err(hba->dev, "%s wb disable exception fail\n", __func__);

		goto out;
	}

	/* when WRITEBOOSTER_EVENT is procing, caller will retry. */
	if (ufshcd_get_wb_work_sched(hba)) {
		ret = -EAGAIN;
		goto out;
	}

	ret = ufshcd_wb_ctrl(hba, false);
	if (ret) {
		dev_err(hba->dev, "%s wb disable fail\n", __func__);

		goto out;
	}
	dev_info(hba->dev, "%s wb permanent disable succ\n", __func__);
out:
	pm_runtime_put_sync(hba->dev);

	return !ret ? len : ret;
}

static ssize_t wb_permanent_disable_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	bool flag = false;
	struct ufs_hba *hba = ufshcd_dev_to_hba(dev);

	if (!hba)
		return snprintf(buf, PAGE_SIZE,
			"wb permanent disable hba is null\n");

	flag = ufshcd_wb_permanent_disable(hba);

	return snprintf(buf, PAGE_SIZE, "wb permanent disable : %s\n",
		flag ? "true" : "false");
}

static int ufshcd_vendor_feature_options_apply(struct ufs_hba *hba)
{
	int ret;
	uint32_t attr_val;

	if (hba->manufacturer_id != UFS_VENDOR_SAMSUNG)
		return 0;

	ret = ufshcd_query_attr(hba, UPIU_QUERY_OPCODE_READ_ATTR,
				QUERY_ATTR_IDN_VENDOR_FEATURE_OPTIONS, 0, 0,
				&attr_val);
	if (!ret && (attr_val & WB_VENDOR_FEATURE_SUPPORT))
		hba->force_selector = true;

	dev_info(hba->dev, "vendor feature options: 0x%x\n",
		 hba->force_selector);

	return ret;
}

static int ufshcd_wb_init(struct ufs_hba *hba)
{
	int ret;
	unsigned long flags;

	/*
	 * Put init once code here, as we need send query command to get device
	 * info which was not able to do in ufshcd_init. To avoid needless
	 * reinit of that, eh in progress was used.
	 */
	spin_lock_irqsave(hba->host->host_lock, flags);
	if (ufshcd_eh_in_progress(hba) || hba->pm_op_in_progress) {
		spin_unlock_irqrestore(hba->host->host_lock, flags);
		return 0;
	}
	spin_unlock_irqrestore(hba->host->host_lock, flags);

	INIT_WORK(&hba->wb_work, ufshcd_wb_toggle_fn);

	ret = ufshcd_vendor_feature_options_apply(hba);
	if (ret)
		return ret;

	return ufshcd_wb_type_detect(hba);
}

static int ufshcd_wb_config(struct ufs_hba *hba)
{
	int ret;

	if (!ufshcd_wb_allowed(hba))
		return 0;

	if (ufshcd_wb_permanent_disable(hba))
		return 0;

	ret = ufshcd_wb_init(hba);
	if (ret)
		return ret;

	ret = ufshcd_wb_ctrl(hba, true);
	if (ret)
		return ret;

	ret = ufshcd_wb_auto_flush_ctrl(hba, true);
	if (ret)
		return ret;

	return ufshcd_wb_enable_exception(hba);
}

/* replace non-printable or non-ASCII characters with spaces */
static inline void ufshcd_remove_non_printable(char *val)
{
	if (!val)
		return;

	if (*val < 0x20 || *val > 0x7e)
		*val = ' ';
}

static void ufshcd_add_command_trace(struct ufs_hba *hba,
		unsigned int tag, const char *str)
{
	sector_t lba = -1; /*lint !e570*/
	u8 opcode = 0;
	u32 intr;
#ifdef CONFIG_HISI_UFS_HC_CORE_UTR
	u64 doorbell;
#else
	u32 doorbell;
#endif
	struct ufshcd_lrb *lrbp;
	int transfer_len = -1;

	if (!trace_ufshcd_command_enabled())
		return;

	lrbp = &hba->lrb[tag];

	if (lrbp->cmd) { /* data phase exists */
		opcode = (u8)(*lrbp->cmd->cmnd);
		if ((opcode == READ_10) || (opcode == WRITE_10)) {
			/*
			 * Currently we only fully trace read(10) and write(10)
			 * commands
			 */
			if (lrbp->cmd->request && lrbp->cmd->request->bio)
				lba =
				  lrbp->cmd->request->bio->bi_iter.bi_sector;
			transfer_len = be32_to_cpu(
				lrbp->ucd_req_ptr->sc.exp_data_transfer_len);
		}
	}

	intr = ufshcd_readl(hba, REG_INTERRUPT_STATUS);
#ifdef CONFIG_HISI_UFS_HC_CORE_UTR
	doorbell = read_core_utr_doorbells(hba);
#else
	doorbell = ufshcd_readl(hba, REG_UTP_TRANSFER_REQ_DOOR_BELL);
#endif

    trace_ufshcd_command(dev_name(hba->dev), str, tag,
				doorbell, transfer_len, intr, lba, opcode);
}

static void ufshcd_print_clk_freqs(struct ufs_hba *hba)
{
	struct ufs_clk_info *clki;
	struct list_head *head = &hba->clk_list_head;

	if (list_empty(head))
		return;

	list_for_each_entry(clki, head, list) {
		if (!IS_ERR_OR_NULL(clki->clk) && clki->min_freq &&
				clki->max_freq)
			dev_err(hba->dev, "clk: %s, rate: %u\n",
					clki->name, clki->curr_freq);
	}
}

/*
 * ufshcd_wait_for_register - wait for register value to change
 * @hba - per-adapter interface
 * @reg - mmio register offset
 * @mask - mask to apply to read register value
 * @val - wait condition
 * @interval_us - polling interval in microsecs
 * @timeout_ms - timeout in millisecs
 * @can_sleep - perform sleep or just spin
 *
 * Returns -ETIMEDOUT on error, zero on success
 */
int ufshcd_wait_for_register(struct ufs_hba *hba, u32 reg, u32 mask,
				u32 val, unsigned long interval_us,
				unsigned long timeout_ms, bool can_sleep)
{
	int err = 0;
	unsigned long timeout = jiffies + msecs_to_jiffies(timeout_ms);

	/* ignore bits that we don't intend to wait on */
	val = val & mask;

	while ((ufshcd_readl(hba, reg) & mask) != val) {
		if (can_sleep)
			usleep_range(interval_us, interval_us + 50);
		else
			udelay(interval_us);
		if (time_after(jiffies, timeout)) {/*lint !e550*/
			if ((ufshcd_readl(hba, reg) & mask) != val)
				err = -ETIMEDOUT;
			break;
		}
	}

	return err;
}

/**
 * ufshcd_get_intr_mask - Get the interrupt bit mask
 * @hba - Pointer to adapter instance
 *
 * Returns interrupt bit mask per version
 */
static inline u32 ufshcd_get_intr_mask(struct ufs_hba *hba)
{
	u32 intr_mask = 0;

	switch (hba->ufs_version) {
	case UFSHCI_VERSION_10:
		intr_mask = INTERRUPT_MASK_ALL_VER_10;
		break;
	case UFSHCI_VERSION_11:
	case UFSHCI_VERSION_20:
		intr_mask = INTERRUPT_MASK_ALL_VER_11;
		break;
	case UFSHCI_VERSION_21:
	default:
		intr_mask = INTERRUPT_MASK_ALL_VER_21;
	}

	return intr_mask;
}

/**
 * ufshcd_get_ufs_version - Get the UFS version supported by the HBA
 * @hba - Pointer to adapter instance
 *
 * Returns UFSHCI version supported by the controller
 */
static inline u32 ufshcd_get_ufs_version(struct ufs_hba *hba)
{
	if (hba->quirks & UFSHCD_QUIRK_BROKEN_UFS_HCI_VERSION)
		return ufshcd_vops_get_ufs_hci_version(hba);

	return ufshcd_readl(hba, REG_UFS_VERSION);
}

/**
 * ufshcd_is_device_present - Check if any device connected to
 *			      the host controller
 * @hba: pointer to adapter instance
 *
 * Returns true if device present, false if no device detected
 */
static inline bool ufshcd_is_device_present(struct ufs_hba *hba)
{
	return (ufshcd_readl(hba, REG_CONTROLLER_STATUS) &
						DEVICE_PRESENT) ? true : false;
}

/**
 * ufshcd_get_tr_ocs - Get the UTRD Overall Command Status
 * @lrb: pointer to local command reference block
 *
 * This function is used to get the OCS field from UTRD
 * Returns the OCS field in the UTRD
 */
static inline int ufshcd_get_tr_ocs(struct ufshcd_lrb *lrbp)
{
	if (lrbp->is_hisi_utrd)
		return le32_to_cpu(
			       lrbp->hisi_utr_descriptor_ptr->header.dword_2) &
		       MASK_OCS;
	return le32_to_cpu(lrbp->utr_descriptor_ptr->header.dword_2) & MASK_OCS;
}

/**
 * ufshcd_get_tmr_ocs - Get the UTMRD Overall Command Status
 * @task_req_descp: pointer to utp_task_req_desc structure
 *
 * This function is used to get the OCS field from UTMRD
 * Returns the OCS field in the UTMRD
 */
static inline int
ufshcd_get_tmr_ocs(struct utp_task_req_desc *task_req_descp)
{
	return le32_to_cpu(task_req_descp->header.dword_2) & MASK_OCS;
}

/**
 * ufshcd_get_tm_free_slot - get a free slot for task management request
 * @hba: per adapter instance
 * @free_slot: pointer to variable with available slot value
 *
 * Get a free tag and lock it until ufshcd_put_tm_slot() is called.
 * Returns 0 if free slot is not available, else return 1 with tag value
 * in @free_slot.
 */
static bool ufshcd_get_tm_free_slot(struct ufs_hba *hba, int *free_slot)
{
	int tag;
	bool ret = false;

	if (!free_slot)
		goto out;

	do {
		tag = find_first_zero_bit(&hba->tm_slots_in_use, hba->nutmrs);
		if (tag >= hba->nutmrs)
			goto out;
	} while (test_and_set_bit_lock(tag, &hba->tm_slots_in_use));

	*free_slot = tag;
	ret = true;
out:
	return ret;
}

static inline void ufshcd_put_tm_slot(struct ufs_hba *hba, int slot)
{
	clear_bit_unlock(slot, &hba->tm_slots_in_use);
}

#ifdef CONFIG_HISI_UFS_HC_CORE_UTR
/**
 * ufshcd_utrl_clear - Clear a bit in UTRLCLR register
 * @hba: per adapter instance
 * @pos: position of the bit to be cleared
 */
static inline void ufshcd_utrl_clear(struct ufs_hba *hba, u32 pos)
{
	ufshcd_writel(hba, ~(1U << (pos % SLOT_NUM_EACH_CORE)),
		UFS_CORE_UTRLCLR(pos / SLOT_NUM_EACH_CORE));
}
#else
/**
 * ufshcd_utrl_clear - Clear a bit in UTRLCLR register
 * @hba: per adapter instance
 * @pos: position of the bit to be cleared
 */
static inline void ufshcd_utrl_clear(struct ufs_hba *hba, u32 pos)
{
	ufshcd_writel(hba, ~(1 << pos), REG_UTP_TRANSFER_REQ_LIST_CLEAR);
}
#endif

/**
 * ufshcd_outstanding_req_clear - Clear a bit in outstanding request field
 * @hba: per adapter instance
 * @tag: position of the bit to be cleared
 */
static inline void ufshcd_outstanding_req_clear(struct ufs_hba *hba, int tag)
{
	__clear_bit(tag, &hba->outstanding_reqs);
}

/**
 * ufshcd_get_lists_status - Check UCRDY, UTRLRDY and UTMRLRDY
 * @reg: Register value of host controller status
 *
 * Returns integer, 0 on Success and positive value if failed
 */
static inline int ufshcd_get_lists_status(u32 reg)
{
	return !((reg & UFSHCD_STATUS_READY) == UFSHCD_STATUS_READY);
}

/**
 * ufshcd_get_uic_cmd_result - Get the UIC command result
 * @hba: Pointer to adapter instance
 *
 * This function gets the result of UIC command completion
 * Returns 0 on success, non zero value on error
 */
static inline int ufshcd_get_uic_cmd_result(struct ufs_hba *hba)
{
	return ufshcd_readl(hba, REG_UIC_COMMAND_ARG_2) &
	       MASK_UIC_COMMAND_RESULT;
}

/**
 * ufshcd_get_dme_attr_val - Get the value of attribute returned by UIC command
 * @hba: Pointer to adapter instance
 *
 * This function gets UIC command argument3
 * Returns 0 on success, non zero value on error
 */
static inline u32 ufshcd_get_dme_attr_val(struct ufs_hba *hba)
{
	return ufshcd_readl(hba, REG_UIC_COMMAND_ARG_3);
}

/**
 * ufshcd_get_req_rsp - returns the TR response transaction type
 * @ucd_rsp_ptr: pointer to response UPIU
 */
static inline int
ufshcd_get_req_rsp(struct utp_upiu_rsp *ucd_rsp_ptr)
{
	return be32_to_cpu(ucd_rsp_ptr->header.dword_0) >> 24;
}

/**
 * ufshcd_get_rsp_upiu_result - Get the result from response UPIU
 * @ucd_rsp_ptr: pointer to response UPIU
 *
 * This function gets the response status and scsi_status from response UPIU
 * Returns the response result code.
 */
static inline int
ufshcd_get_rsp_upiu_result(struct utp_upiu_rsp *ucd_rsp_ptr)
{
	return be32_to_cpu(ucd_rsp_ptr->header.dword_1) & MASK_RSP_UPIU_RESULT;
}

/*
 * ufshcd_get_rsp_upiu_data_seg_len - Get the data segment length
 *				from response UPIU
 * @ucd_rsp_ptr: pointer to response UPIU
 *
 * Return the data segment length.
 */
static inline unsigned int
ufshcd_get_rsp_upiu_data_seg_len(struct utp_upiu_rsp *ucd_rsp_ptr)
{
	return be32_to_cpu(ucd_rsp_ptr->header.dword_2) &
		MASK_RSP_UPIU_DATA_SEG_LEN;
}

/**
 * ufshcd_is_exception_event - Check if the device raised an exception event
 * @ucd_rsp_ptr: pointer to response UPIU
 *
 * The function checks if the device raised an exception event indicated in
 * the Device Information field of response UPIU.
 *
 * Returns true if exception is raised, false otherwise.
 */
static inline bool ufshcd_is_exception_event(struct utp_upiu_rsp *ucd_rsp_ptr)
{
	return be32_to_cpu(ucd_rsp_ptr->header.dword_2) &
			MASK_RSP_EXCEPTION_EVENT ? true : false;
}

/**
 * ufshcd_reset_intr_aggr - Reset interrupt aggregation values.
 * @hba: per adapter instance
 */
static inline void
ufshcd_reset_intr_aggr(struct ufs_hba *hba)
{
	ufshcd_writel(hba, INT_AGGR_ENABLE |
		      INT_AGGR_COUNTER_AND_TIMER_RESET,
		      REG_UTP_TRANSFER_REQ_INT_AGG_CONTROL);
}

/**
 * ufshcd_config_intr_aggr - Configure interrupt aggregation values.
 * @hba: per adapter instance
 * @cnt: Interrupt aggregation counter threshold
 * @tmout: Interrupt aggregation timeout value
 */
static inline void
ufshcd_config_intr_aggr(struct ufs_hba *hba, u8 cnt, u8 tmout)
{
	ufshcd_writel(hba, INT_AGGR_ENABLE | INT_AGGR_PARAM_WRITE |
		      INT_AGGR_COUNTER_THLD_VAL(cnt) |
		      INT_AGGR_TIMEOUT_VAL(tmout),
		      REG_UTP_TRANSFER_REQ_INT_AGG_CONTROL);
}

/**
 * ufshcd_disable_intr_aggr - Disables interrupt aggregation.
 * @hba: per adapter instance
 */
static inline void ufshcd_disable_intr_aggr(struct ufs_hba *hba)
{
	ufshcd_writel(hba, 0, REG_UTP_TRANSFER_REQ_INT_AGG_CONTROL);
}

/**
 * ufshcd_enable_run_stop_reg - Enable run-stop registers,
 *			When run-stop registers are set to 1, it indicates the
 *			host controller that it can process the requests
 * @hba: per adapter instance
 */
static void ufshcd_enable_run_stop_reg(struct ufs_hba *hba)
{
	ufshcd_writel(hba, UTP_TASK_REQ_LIST_RUN_STOP_BIT,
		      REG_UTP_TASK_REQ_LIST_RUN_STOP);
#ifdef CONFIG_HISI_UFS_HC_CORE_UTR
	ufshcd_enable_core_run_stop_reg(hba);
#else
	ufshcd_writel(hba, UTP_TRANSFER_REQ_LIST_RUN_STOP_BIT,
		      REG_UTP_TRANSFER_REQ_LIST_RUN_STOP);
#endif
}


/**
 * ufshcd_is_hba_active - Get controller state
 * @hba: per adapter instance
 *
 * Returns false if controller is active, true otherwise
 */
static inline bool ufshcd_is_hba_active(struct ufs_hba *hba)
{
	return (ufshcd_readl(hba, REG_CONTROLLER_ENABLE) & CONTROLLER_ENABLE)
		? false : true;
}

static const char *ufschd_uic_link_state_to_string(
			enum uic_link_state state)
{
	switch (state) {
	case UIC_LINK_OFF_STATE:	return "OFF";
	case UIC_LINK_ACTIVE_STATE:	return "ACTIVE";
	case UIC_LINK_HIBERN8_STATE:	return "HIBERN8";
	default:			return "UNKNOWN";
	}
}

static const char *ufschd_ufs_dev_pwr_mode_to_string(
			enum ufs_dev_pwr_mode state)
{
	switch (state) {
	case UFS_ACTIVE_PWR_MODE:	return "ACTIVE";
	case UFS_SLEEP_PWR_MODE:	return "SLEEP";
	case UFS_POWERDOWN_PWR_MODE:	return "POWERDOWN";
	default:			return "UNKNOWN";
	}
}

u32 ufshcd_get_local_unipro_ver(struct ufs_hba *hba)
{
	/* HCI version 1.0 and 1.1 supports UniPro 1.41 */
	if ((hba->ufs_version == UFSHCI_VERSION_10) ||
	    (hba->ufs_version == UFSHCI_VERSION_11))
		return UFS_UNIPRO_VER_1_41;
	else
		return UFS_UNIPRO_VER_1_6;
}
EXPORT_SYMBOL(ufshcd_get_local_unipro_ver);

static bool ufshcd_is_unipro_pa_params_tuning_req(struct ufs_hba *hba)
{
	/*
	 * If both host and device support UniPro ver1.6 or later, PA layer
	 * parameters tuning happens during link startup itself.
	 *
	 * We can manually tune PA layer parameters if either host or device
	 * doesn't support UniPro ver 1.6 or later. But to keep manual tuning
	 * logic simple, we will only do manual tuning if local unipro version
	 * doesn't support ver1.6 or later.
	 */
	if (ufshcd_get_local_unipro_ver(hba) < UFS_UNIPRO_VER_1_6)
		return true;
	else
		return false;
}

static int ufshcd_scale_clks(struct ufs_hba *hba, bool scale_up)
{
	int ret = 0;
	struct ufs_clk_info *clki;
	struct list_head *head = &hba->clk_list_head;
	ktime_t start = ktime_get();
	bool clk_state_changed = false;

	if (list_empty(head))
		goto out;

	ret = ufshcd_vops_clk_scale_notify(hba, scale_up, PRE_CHANGE);
	if (ret)
		return ret;

	list_for_each_entry(clki, head, list) {
		if (!IS_ERR_OR_NULL(clki->clk)) {
			if (scale_up && clki->max_freq) {
				if (clki->curr_freq == clki->max_freq)
					continue;

				clk_state_changed = true;
				ret = clk_set_rate(clki->clk, clki->max_freq);
				if (ret) {
					dev_err(hba->dev, "%s: %s clk set rate(%dHz) failed, %d\n",
						__func__, clki->name,
						clki->max_freq, ret);
					break;
				}
				trace_ufshcd_clk_scaling(dev_name(hba->dev),
						"scaled up", clki->name,
						clki->curr_freq,
						clki->max_freq);
				clki->curr_freq = clki->max_freq;

			} else if (!scale_up && clki->min_freq) {
				if (clki->curr_freq == clki->min_freq)
					continue;

				clk_state_changed = true;
				ret = clk_set_rate(clki->clk, clki->min_freq);
				if (ret) {
					dev_err(hba->dev, "%s: %s clk set rate(%dHz) failed, %d\n",
						__func__, clki->name,
						clki->min_freq, ret);
					break;
				}
				trace_ufshcd_clk_scaling(dev_name(hba->dev),
						"scaled down", clki->name,
						clki->curr_freq,
						clki->min_freq);
				clki->curr_freq = clki->min_freq;
			}
		}
		dev_dbg(hba->dev, "%s: clk: %s, rate: %lu\n", __func__,
				clki->name, clk_get_rate(clki->clk));
	}

	ret = ufshcd_vops_clk_scale_notify(hba, scale_up, POST_CHANGE);

out:
	if (clk_state_changed)
		trace_ufshcd_profile_clk_scaling(dev_name(hba->dev),
			(scale_up ? "up" : "down"),
			ktime_to_us(ktime_sub(ktime_get(), start)), ret);
	return ret;
}

/**
 * ufshcd_is_devfreq_scaling_required - check if scaling is required or not
 * @hba: per adapter instance
 * @scale_up: True if scaling up and false if scaling down
 *
 * Returns true if scaling is required, false otherwise.
 */
static bool ufshcd_is_devfreq_scaling_required(struct ufs_hba *hba,
					       bool scale_up)
{
	struct ufs_clk_info *clki;
	struct list_head *head = &hba->clk_list_head;

	if (list_empty(head))
		return false;

	list_for_each_entry(clki, head, list) {
		if (!IS_ERR_OR_NULL(clki->clk)) {
			if (scale_up && clki->max_freq) {
				if (clki->curr_freq == clki->max_freq)
					continue;
				return true;
			} else if (!scale_up && clki->min_freq) {
				if (clki->curr_freq == clki->min_freq)
					continue;
				return true;
			}
		}
	}

	return false;
}

/**
 * ufshcd_scale_gear - scale up/down UFS gear
 * @hba: per adapter instance
 * @scale_up: True for scaling up gear and false for scaling down
 *
 * Returns 0 for success,
 * Returns -EBUSY if scaling can't happen at this time
 * Returns non-zero for any other errors
 */
static int ufshcd_scale_gear(struct ufs_hba *hba, bool scale_up)
{
	#define UFS_MIN_GEAR_TO_SCALE_DOWN	UFS_HS_G1
	int ret = 0;
	struct ufs_pa_layer_attr new_pwr_info;

	if (scale_up) {
		memcpy(&new_pwr_info, &hba->clk_scaling.saved_pwr_info.info,
		       sizeof(struct ufs_pa_layer_attr));
	} else {
		memcpy(&new_pwr_info, &hba->pwr_info,
		       sizeof(struct ufs_pa_layer_attr));

		if (hba->pwr_info.gear_tx > UFS_MIN_GEAR_TO_SCALE_DOWN
		    || hba->pwr_info.gear_rx > UFS_MIN_GEAR_TO_SCALE_DOWN) {
			/* save the current power mode */
			memcpy(&hba->clk_scaling.saved_pwr_info.info,
				&hba->pwr_info,
				sizeof(struct ufs_pa_layer_attr));

			/* scale down gear */
			new_pwr_info.gear_tx = UFS_MIN_GEAR_TO_SCALE_DOWN;
			new_pwr_info.gear_rx = UFS_MIN_GEAR_TO_SCALE_DOWN;
		}
	}

	/* check if the power mode needs to be changed or not? */
	ret = ufshcd_change_power_mode(hba, &new_pwr_info);

	if (ret)
		dev_err(hba->dev, "%s: failed err %d, old gear: (tx %d rx %d), new gear: (tx %d rx %d)",
			__func__, ret,
			hba->pwr_info.gear_tx, hba->pwr_info.gear_rx,
			new_pwr_info.gear_tx, new_pwr_info.gear_rx);

	return ret;
}

static int ufshcd_clock_scaling_prepare(struct ufs_hba *hba)
{
	#define DOORBELL_CLR_TOUT_US		(1000 * 1000) /* 1 sec */
	int ret = 0;
	/*
	 * make sure that there are no outstanding requests when
	 * clock scaling is in progress
	 */
	scsi_block_requests(hba->host);
	down_write(&hba->clk_scaling_lock);
	if (ufshcd_wait_for_doorbell_clr(hba, DOORBELL_CLR_TOUT_US)) {
		ret = -EBUSY;
		up_write(&hba->clk_scaling_lock);
		scsi_unblock_requests(hba->host);
	}

	return ret;
}

static void ufshcd_clock_scaling_unprepare(struct ufs_hba *hba)
{
	up_write(&hba->clk_scaling_lock);
	scsi_unblock_requests(hba->host);
}

/**
 * ufshcd_devfreq_scale - scale up/down UFS clocks and gear
 * @hba: per adapter instance
 * @scale_up: True for scaling up and false for scalin down
 *
 * Returns 0 for success,
 * Returns -EBUSY if scaling can't happen at this time
 * Returns non-zero for any other errors
 */
static int ufshcd_devfreq_scale(struct ufs_hba *hba, bool scale_up)
{
	int ret = 0;

	/* let's not get into low power until clock scaling is completed */
	ufshcd_hold(hba, false);

	ret = ufshcd_clock_scaling_prepare(hba);
	if (ret)
		return ret;

	/* scale down the gear before scaling down clocks */
	if (!scale_up) {
		ret = ufshcd_scale_gear(hba, false);
		if (ret)
			goto out;
	}

	ret = ufshcd_scale_clks(hba, scale_up);
	if (ret) {
		if (!scale_up)
			ufshcd_scale_gear(hba, true);
		goto out;
	}

	/* scale up the gear after scaling up clocks */
	if (scale_up) {
		ret = ufshcd_scale_gear(hba, true);
		if (ret) {
			ufshcd_scale_clks(hba, false);
			goto out;
		}
	}

	ret = ufshcd_vops_clk_scale_notify(hba, scale_up, POST_CHANGE);

out:
	ufshcd_clock_scaling_unprepare(hba);
	ufshcd_release(hba);
	return ret;
}

static void ufshcd_clk_scaling_suspend_work(struct work_struct *work)
{
	struct ufs_hba *hba = container_of(work, struct ufs_hba,
					   clk_scaling.suspend_work);
	unsigned long irq_flags;

	spin_lock_irqsave(hba->host->host_lock, irq_flags);
	if (hba->clk_scaling.active_reqs || hba->clk_scaling.is_suspended) {
		spin_unlock_irqrestore(hba->host->host_lock, irq_flags);
		return;
	}
	hba->clk_scaling.is_suspended = true;
	spin_unlock_irqrestore(hba->host->host_lock, irq_flags);

	__ufshcd_suspend_clkscaling(hba);
}

static void ufshcd_clk_scaling_resume_work(struct work_struct *work)
{
	struct ufs_hba *hba = container_of(work, struct ufs_hba,
					   clk_scaling.resume_work);
	unsigned long irq_flags;

	spin_lock_irqsave(hba->host->host_lock, irq_flags);
	if (!hba->clk_scaling.is_suspended) {
		spin_unlock_irqrestore(hba->host->host_lock, irq_flags);
		return;
	}
	hba->clk_scaling.is_suspended = false;
	spin_unlock_irqrestore(hba->host->host_lock, irq_flags);

	devfreq_resume_device(hba->devfreq);
}

static int ufshcd_devfreq_target(struct device *dev,
				unsigned long *freq, u32 flags)
{
	int ret = 0;
	struct ufs_hba *hba = dev_get_drvdata(dev);
	ktime_t start;
	bool scale_up, sched_clk_scaling_suspend_work = false;
	unsigned long irq_flags;

	if (!ufshcd_is_clkscaling_supported(hba))
		return -EINVAL;

	if ((*freq > 0) && (*freq < UINT_MAX)) {
		dev_err(hba->dev, "%s: invalid freq = %lu\n", __func__, *freq);
		return -EINVAL;
	}

	spin_lock_irqsave(hba->host->host_lock, irq_flags);
	if (ufshcd_eh_in_progress(hba)) {
		spin_unlock_irqrestore(hba->host->host_lock, irq_flags);
		return 0;
	}

	if (!hba->clk_scaling.active_reqs)
		sched_clk_scaling_suspend_work = true;

	scale_up = (*freq == UINT_MAX) ? true : false;
	if (!ufshcd_is_devfreq_scaling_required(hba, scale_up)) {
		spin_unlock_irqrestore(hba->host->host_lock, irq_flags);
		ret = 0;
		goto out; /* no state change required */
	}
	spin_unlock_irqrestore(hba->host->host_lock, irq_flags);

	start = ktime_get();
	ret = ufshcd_devfreq_scale(hba, scale_up);
	trace_ufshcd_profile_clk_scaling(dev_name(hba->dev),
		(scale_up ? "up" : "down"),
        ktime_to_us(ktime_sub(ktime_get(), start)), ret);
out:
	if (sched_clk_scaling_suspend_work)
		queue_work(hba->clk_scaling.workq,
			   &hba->clk_scaling.suspend_work);

	return ret;
}


static int ufshcd_devfreq_get_dev_status(struct device *dev,
		struct devfreq_dev_status *stat)
{
	struct ufs_hba *hba = dev_get_drvdata(dev);
	struct ufs_clk_scaling *scaling = &hba->clk_scaling;
	unsigned long flags;

	if (!ufshcd_is_clkscaling_supported(hba))
		return -EINVAL;

	memset(stat, 0, sizeof(*stat));

	spin_lock_irqsave(hba->host->host_lock, flags);
	if (!scaling->window_start_t)
		goto start_window;

	if (scaling->is_busy_started)
		scaling->tot_busy_t += ktime_to_us(ktime_sub(ktime_get(),
					scaling->busy_start_t));

	stat->total_time = jiffies_to_usecs((long)jiffies -
				(long)scaling->window_start_t);
	stat->busy_time = scaling->tot_busy_t;
start_window:
	scaling->window_start_t = jiffies;
	scaling->tot_busy_t = 0;

	if (hba->outstanding_reqs) {
		scaling->busy_start_t = ktime_get();
		scaling->is_busy_started = true;
	} else {
		scaling->busy_start_t = 0;
		scaling->is_busy_started = false;
	}
	spin_unlock_irqrestore(hba->host->host_lock, flags);
	return 0;
}

static struct devfreq_dev_profile ufs_devfreq_profile = {
	.polling_ms	= 100,
	.target		= ufshcd_devfreq_target,
	.get_dev_status	= ufshcd_devfreq_get_dev_status,
};

static void __ufshcd_suspend_clkscaling(struct ufs_hba *hba)
{
	unsigned long flags;

	devfreq_suspend_device(hba->devfreq);
	spin_lock_irqsave(hba->host->host_lock, flags);
	hba->clk_scaling.window_start_t = 0;
	spin_unlock_irqrestore(hba->host->host_lock, flags);
}

static void ufshcd_suspend_clkscaling(struct ufs_hba *hba)
{
	unsigned long flags;
	bool suspend = false;

	if (!ufshcd_is_clkscaling_supported(hba))
		return;

	spin_lock_irqsave(hba->host->host_lock, flags);
	if (!hba->clk_scaling.is_suspended) {
		suspend = true;
		hba->clk_scaling.is_suspended = true;
	}
	spin_unlock_irqrestore(hba->host->host_lock, flags);

	if (suspend)
		__ufshcd_suspend_clkscaling(hba);
}

static void ufshcd_resume_clkscaling(struct ufs_hba *hba)
{
	unsigned long flags;
	bool resume = false;

	if (!ufshcd_is_clkscaling_supported(hba))
		return;

	spin_lock_irqsave(hba->host->host_lock, flags);
	if (hba->clk_scaling.is_suspended) {
		resume = true;
		hba->clk_scaling.is_suspended = false;
	}
	spin_unlock_irqrestore(hba->host->host_lock, flags);

	if (resume)
		devfreq_resume_device(hba->devfreq);
}

static ssize_t ufshcd_clkscale_enable_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct ufs_hba *hba = dev_get_drvdata(dev);

	return snprintf(buf, PAGE_SIZE, "%d\n", hba->clk_scaling.is_allowed);
}

static ssize_t ufshcd_clkscale_enable_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct ufs_hba *hba = dev_get_drvdata(dev);
	u32 value;
	int err;

	if (kstrtou32(buf, 0, &value))
		return -EINVAL;

	value = !!value;
	if (value == hba->clk_scaling.is_allowed)
		goto out;

	pm_runtime_get_sync(hba->dev);
	ufshcd_hold(hba, false);

	cancel_work_sync(&hba->clk_scaling.suspend_work);
	cancel_work_sync(&hba->clk_scaling.resume_work);

	hba->clk_scaling.is_allowed = value;

	if (value) {
		ufshcd_resume_clkscaling(hba);
	} else {
		ufshcd_suspend_clkscaling(hba);
		err = ufshcd_devfreq_scale(hba, true);
		if (err)
			dev_err(hba->dev, "%s: failed to scale clocks up %d\n",
					__func__, err);
	}

	ufshcd_release(hba);
	pm_runtime_put_sync(hba->dev);
out:
	return count;
}

static void ufshcd_clkscaling_init_sysfs(struct ufs_hba *hba)
{
	hba->clk_scaling.enable_attr.show = ufshcd_clkscale_enable_show;
	hba->clk_scaling.enable_attr.store = ufshcd_clkscale_enable_store;
	sysfs_attr_init(&hba->clk_scaling.enable_attr.attr);
	hba->clk_scaling.enable_attr.attr.name = "clkscale_enable";
	hba->clk_scaling.enable_attr.attr.mode = 0644;
	if (device_create_file(hba->dev, &hba->clk_scaling.enable_attr))
		dev_err(hba->dev, "Failed to create sysfs for clkscale_enable\n");
}

static void ufshcd_ungate_work(struct work_struct *work) /*lint !e578*/
{
	int ret;
	unsigned long flags;
	struct ufs_hba *hba = container_of(work, struct ufs_hba,
			clk_gating.ungate_work);

	cancel_delayed_work_sync(&hba->clk_gating.gate_work);

	spin_lock_irqsave(hba->host->host_lock, flags);
	if (hba->clk_gating.state == CLKS_ON) {
		spin_unlock_irqrestore(hba->host->host_lock, flags);
		goto unblock_reqs;
	}

	spin_unlock_irqrestore(hba->host->host_lock, flags);
	ufshcd_setup_clocks(hba, true);

	/* Exit from hibern8 */
	if (ufshcd_can_hibern8_during_gating(hba)) {
		/* Prevent gating in this path */
		hba->clk_gating.is_suspended = true;
		if (ufshcd_is_link_hibern8(hba)) {
			ret = ufshcd_uic_hibern8_exit(hba);
			if (ret)
				dev_err(hba->dev, "%s: hibern8 exit failed %d\n",
					__func__, ret);
			else
				ufshcd_set_link_active(hba);
		}
		hba->clk_gating.is_suspended = false;
	}
unblock_reqs:
	scsi_unblock_requests(hba->host);
}

/**
 * ufshcd_hold - Enable clocks that were gated earlier due to ufshcd_release.
 * Also, exit from hibern8 mode and set the link as active.
 * @hba: per adapter instance
 * @async: This indicates whether caller should ungate clocks asynchronously.
 */
int ufshcd_hold(struct ufs_hba *hba, bool async)
{
	int rc = 0;
	unsigned long flags;

	if (!ufshcd_is_clkgating_allowed(hba))
		goto out;
	spin_lock_irqsave(hba->host->host_lock, flags);
	hba->clk_gating.active_reqs++;

	if (ufshcd_eh_in_progress(hba)) {
		spin_unlock_irqrestore(hba->host->host_lock, flags);
		return 0;
	}

start:
	switch (hba->clk_gating.state) {
	case CLKS_ON:
		/*
		 * Wait for the ungate work to complete if in progress.
		 * Though the clocks may be in ON state, the link could
		 * still be in hibner8 state if hibern8 is allowed
		 * during clock gating.
		 * Make sure we exit hibern8 state also in addition to
		 * clocks being ON.
		 */
		if (ufshcd_can_hibern8_during_gating(hba) &&
		    ufshcd_is_link_hibern8(hba)) {
			if (async) {
				rc = -EAGAIN;
				hba->clk_gating.active_reqs--;
				break;
			}
			spin_unlock_irqrestore(hba->host->host_lock, flags);
			flush_work(&hba->clk_gating.ungate_work);
			spin_lock_irqsave(hba->host->host_lock, flags);
			goto start;
		}
		break;
	case REQ_CLKS_OFF:
		if (cancel_delayed_work(&hba->clk_gating.gate_work)) {
			hba->clk_gating.state = CLKS_ON;

			trace_ufshcd_clk_gating(dev_name(hba->dev),
						hba->clk_gating.state);
			break;
		}
		/*
		 * If we are here, it means gating work is either done or
		 * currently running. Hence, fall through to cancel gating
		 * work and to enable clocks.
		 */
	case CLKS_OFF:
		scsi_block_requests(hba->host);
		hba->clk_gating.state = REQ_CLKS_ON;

		trace_ufshcd_clk_gating(dev_name(hba->dev),
					hba->clk_gating.state);
		schedule_work(&hba->clk_gating.ungate_work);
		/*
		 * fall through to check if we should wait for this
		 * work to be done or not.
		 */
	case REQ_CLKS_ON:
		if (async) {
			rc = -EAGAIN;
			hba->clk_gating.active_reqs--;
			break;
		}

		spin_unlock_irqrestore(hba->host->host_lock, flags);
		flush_work(&hba->clk_gating.ungate_work);
		/* Make sure state is CLKS_ON before returning */
		spin_lock_irqsave(hba->host->host_lock, flags);
		goto start;
	default:
		dev_err(hba->dev, "%s: clk gating is in invalid state %d\n",
				__func__, hba->clk_gating.state);
		break;
	}
	spin_unlock_irqrestore(hba->host->host_lock, flags);
out:
	return rc;
}

EXPORT_SYMBOL_GPL(ufshcd_hold);

static void ufshcd_gate_work(struct work_struct *work)
{
	struct ufs_hba *hba = container_of(work, struct ufs_hba,
			clk_gating.gate_work.work);
	unsigned long flags;

	spin_lock_irqsave(hba->host->host_lock, flags);
	/*
	 * In case you are here to cancel this work the gating state
	 * would be marked as REQ_CLKS_ON. In this case save time by
	 * skipping the gating work and exit after changing the clock
	 * state to CLKS_ON.
	 */
	if (hba->clk_gating.is_suspended ||
		(hba->clk_gating.state == REQ_CLKS_ON)) {
		hba->clk_gating.state = CLKS_ON;
		trace_ufshcd_clk_gating(dev_name(hba->dev),
					hba->clk_gating.state);
		goto rel_lock;
	}

	if (hba->clk_gating.active_reqs
		|| hba->ufshcd_state != UFSHCD_STATE_OPERATIONAL
		|| hba->lrb_in_use || hba->outstanding_tasks
		|| hba->active_uic_cmd || hba->uic_async_done)
		goto rel_lock;

	spin_unlock_irqrestore(hba->host->host_lock, flags);

	/* put the link into hibern8 mode before turning off clocks */
	if (ufshcd_can_hibern8_during_gating(hba)) {
		if (ufshcd_uic_hibern8_enter(hba)) {
		    ufshcd_update_error_stats(hba, UFS_ERR_HIBERN8_ENTER);
			hba->clk_gating.state = CLKS_ON;
			trace_ufshcd_clk_gating(dev_name(hba->dev),
						hba->clk_gating.state);
			goto out;
		}
		ufshcd_set_link_hibern8(hba);
	}

	if (!ufshcd_is_link_active(hba))
		ufshcd_setup_clocks(hba, false);
	else
		/* If link is active, device ref_clk can't be switched off */
		__ufshcd_setup_clocks(hba, false, true);

	/*
	 * In case you are here to cancel this work the gating state
	 * would be marked as REQ_CLKS_ON. In this case keep the state
	 * as REQ_CLKS_ON which would anyway imply that clocks are off
	 * and a request to turn them on is pending. By doing this way,
	 * we keep the state machine in tact and this would ultimately
	 * prevent from doing cancel work multiple times when there are
	 * new requests arriving before the current cancel work is done.
	 */
	spin_lock_irqsave(hba->host->host_lock, flags);
	if (hba->clk_gating.state == REQ_CLKS_OFF) {
		hba->clk_gating.state = CLKS_OFF;
		trace_ufshcd_clk_gating(dev_name(hba->dev),
					hba->clk_gating.state);
	}
rel_lock:
	spin_unlock_irqrestore(hba->host->host_lock, flags);
out:
	return;
}

/* host lock must be held before calling this variant */
static void __ufshcd_release(struct ufs_hba *hba)
{
	if (!ufshcd_is_clkgating_allowed(hba))
		return;

	hba->clk_gating.active_reqs--;

	if (hba->clk_gating.active_reqs || hba->clk_gating.is_suspended
		|| hba->ufshcd_state != UFSHCD_STATE_OPERATIONAL
		|| hba->lrb_in_use || hba->outstanding_tasks
		|| hba->active_uic_cmd || hba->uic_async_done
		|| ufshcd_eh_in_progress(hba))
		return;

	hba->clk_gating.state = REQ_CLKS_OFF;
	trace_ufshcd_clk_gating(dev_name(hba->dev), hba->clk_gating.state);
	schedule_delayed_work(&hba->clk_gating.gate_work,
			msecs_to_jiffies(hba->clk_gating.delay_ms));
}

void ufshcd_release(struct ufs_hba *hba)
{
	unsigned long flags;

	spin_lock_irqsave(hba->host->host_lock, flags);
	__ufshcd_release(hba);
	spin_unlock_irqrestore(hba->host->host_lock, flags);
}

EXPORT_SYMBOL_GPL(ufshcd_release);

static ssize_t ufshcd_clkgate_delay_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct ufs_hba *hba = dev_get_drvdata(dev);

	return snprintf(buf, PAGE_SIZE, "%lu\n", hba->clk_gating.delay_ms);
}

static ssize_t ufshcd_clkgate_delay_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct ufs_hba *hba = dev_get_drvdata(dev);
	unsigned long flags, value;

	if (kstrtoul(buf, 0, &value))
		return -EINVAL;

	spin_lock_irqsave(hba->host->host_lock, flags);
	hba->clk_gating.delay_ms = value;
	spin_unlock_irqrestore(hba->host->host_lock, flags);
	return count;
}

static ssize_t ufshcd_clkgate_enable_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct ufs_hba *hba = dev_get_drvdata(dev);

	return snprintf(buf, PAGE_SIZE, "%d\n", hba->clk_gating.is_enabled);
}

static ssize_t ufshcd_clkgate_enable_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct ufs_hba *hba = dev_get_drvdata(dev);
	unsigned long flags;
	u32 value;

	if (kstrtou32(buf, 0, &value))
		return -EINVAL;

	value = !!value;
	if (value == hba->clk_gating.is_enabled)
		goto out;

	if (value) {
		ufshcd_release(hba);
	} else {
		spin_lock_irqsave(hba->host->host_lock, flags);
		hba->clk_gating.active_reqs++;
		spin_unlock_irqrestore(hba->host->host_lock, flags);
	}

	hba->clk_gating.is_enabled = value;
out:
	return count;
}

static void ufshcd_init_clk_gating(struct ufs_hba *hba)
{
	if (!ufshcd_is_clkgating_allowed(hba))
		return;

	hba->clk_gating.delay_ms = 150;
	INIT_DELAYED_WORK(&hba->clk_gating.gate_work, ufshcd_gate_work);
	INIT_WORK(&hba->clk_gating.ungate_work, ufshcd_ungate_work);

	hba->clk_gating.is_enabled = true;

	hba->clk_gating.delay_attr.show = ufshcd_clkgate_delay_show;
	hba->clk_gating.delay_attr.store = ufshcd_clkgate_delay_store;
	sysfs_attr_init(&hba->clk_gating.delay_attr.attr);
	hba->clk_gating.delay_attr.attr.name = "clkgate_delay_ms";
	hba->clk_gating.delay_attr.attr.mode = 0644;
	if (device_create_file(hba->dev, &hba->clk_gating.delay_attr))
		dev_err(hba->dev, "Failed to create sysfs for clkgate_delay\n");

	hba->clk_gating.enable_attr.show = ufshcd_clkgate_enable_show;
	hba->clk_gating.enable_attr.store = ufshcd_clkgate_enable_store;
	sysfs_attr_init(&hba->clk_gating.enable_attr.attr);
	hba->clk_gating.enable_attr.attr.name = "clkgate_enable";
	hba->clk_gating.enable_attr.attr.mode = 0644;
	if (device_create_file(hba->dev, &hba->clk_gating.enable_attr))
		dev_err(hba->dev, "Failed to create sysfs for clkgate_enable\n");
}

static void ufshcd_exit_clk_gating(struct ufs_hba *hba)
{
	if (!ufshcd_is_clkgating_allowed(hba))
		return;
	device_remove_file(hba->dev, &hba->clk_gating.delay_attr);
	device_remove_file(hba->dev, &hba->clk_gating.enable_attr);
	cancel_work_sync(&hba->clk_gating.ungate_work);
	cancel_delayed_work_sync(&hba->clk_gating.gate_work);
}

/* Must be called with host lock acquired */
static void ufshcd_clk_scaling_start_busy(struct ufs_hba *hba)
{
	bool queue_resume_work = false;

	if (!ufshcd_is_clkscaling_supported(hba))
		return;

	if (!hba->clk_scaling.active_reqs++)
		queue_resume_work = true;

	if (!hba->clk_scaling.is_allowed || hba->pm_op_in_progress)
		return;

	if (queue_resume_work)
		queue_work(hba->clk_scaling.workq,
			   &hba->clk_scaling.resume_work);

	if (!hba->clk_scaling.window_start_t) {
		hba->clk_scaling.window_start_t = jiffies;
		hba->clk_scaling.tot_busy_t = 0;
		hba->clk_scaling.is_busy_started = false;
	}

	if (!hba->clk_scaling.is_busy_started) {
		hba->clk_scaling.busy_start_t = ktime_get();
		hba->clk_scaling.is_busy_started = true;
	}
}

static void ufshcd_clk_scaling_update_busy(struct ufs_hba *hba)
{
	struct ufs_clk_scaling *scaling = &hba->clk_scaling;

	if (!ufshcd_is_clkscaling_supported(hba))
		return;

	if (!hba->outstanding_reqs && scaling->is_busy_started) {
		scaling->tot_busy_t += ktime_to_us(ktime_sub(ktime_get(),
					scaling->busy_start_t));
		scaling->busy_start_t = 0;
		scaling->is_busy_started = false;
	}
}

/**
 * ufshcd_send_command - Send SCSI or device management commands
 * @hba: per adapter instance
 * @task_tag: Task tag of the command
 */
/*lint -save -e695*/
inline
void ufshcd_send_command(struct ufs_hba *hba, unsigned int task_tag)
{
	int ret = 0;
	struct ufshcd_lrb *lrbp = &hba->lrb[task_tag];

	lrbp->issue_time_stamp = ktime_get();
	lrbp->complete_time_stamp = ktime_set(0, 0);
	lrbp->opcode = 0;
	lrbp->req_tag = 0xFF;
	if (lrbp->cmd) {
		lrbp->opcode = lrbp->cmd->cmnd[0];
		if (lrbp->cmd->request)
			lrbp->req_tag = lrbp->cmd->request->tag;
	}

#ifdef CONFIG_HUAWEI_DSM_IOMT_UFS_HOST
	iomt_host_latency_cmd_start(lrbp->cmd, lrbp->issue_time_stamp);
#endif
	ufshcd_clk_scaling_start_busy(hba);
	__set_bit((int)task_tag, &hba->outstanding_reqs);
#ifdef CONFIG_HISI_UFS_HC_CORE_UTR
	ufshcd_writel(hba, 1 << (task_tag % SLOT_NUM_EACH_CORE),
		UFS_CORE_UTRLDBR(task_tag / SLOT_NUM_EACH_CORE));
#else
	ufshcd_writel(hba, 1 << task_tag, REG_UTP_TRANSFER_REQ_DOOR_BELL);
#endif

	ufsdbg_error_inject_dispatcher(hba,
		ERR_INJECT_DEDBUG_CTRL_INTR,
		ret,
		&ret);

	/* Make sure that doorbell is committed immediately */
	wmb();
	ufshcd_update_tag_stats(hba, task_tag);
	ufshcd_add_command_trace(hba, task_tag, "send");
}
/*lint -restore*/
EXPORT_SYMBOL(ufshcd_send_command);

/**
 * ufshcd_copy_sense_data - Copy sense data in case of check condition
 * @lrb - pointer to local reference block
 */
static inline void ufshcd_copy_sense_data(struct ufshcd_lrb *lrbp)
{
	int len;
	if (lrbp->sense_buffer &&
	    ufshcd_get_rsp_upiu_data_seg_len(lrbp->ucd_rsp_ptr)) {
		int len_to_copy;

		len = be16_to_cpu(lrbp->ucd_rsp_ptr->sr.sense_data_len);
		len_to_copy = min_t(int, RESPONSE_UPIU_SENSE_DATA_LENGTH, len);

		memcpy(lrbp->sense_buffer,
			lrbp->ucd_rsp_ptr->sr.sense_data,
			min_t(int, len_to_copy, UFSHCD_REQ_SENSE_SIZE)); /*lint !e670*/
		lrbp->saved_sense_len = len;
	}
}

/**
 * ufshcd_copy_query_response() - Copy the Query Response and the data
 * descriptor
 * @hba: per adapter instance
 * @lrb - pointer to local reference block
 */
static
int ufshcd_copy_query_response(struct ufs_hba *hba, struct ufshcd_lrb *lrbp)
{
	struct ufs_query_res *query_res = &hba->dev_cmd.query.response;

	memcpy(&query_res->upiu_res, &lrbp->ucd_rsp_ptr->qr, QUERY_OSF_SIZE);

	/* Get the descriptor */
	if (lrbp->ucd_rsp_ptr->qr.opcode == UPIU_QUERY_OPCODE_READ_DESC ||
	    lrbp->ucd_rsp_ptr->qr.opcode == UPIU_QUERY_OPCODE_READ_HI1861_FSR ||
	    lrbp->ucd_rsp_ptr->qr.opcode == UPIU_QUERY_OPCODE_READ_TZ_DESC ||
	    lrbp->ucd_rsp_ptr->qr.opcode == UPIU_QUERY_OPCODE_VENDOR_READ) {
		u8 *descp = (u8 *)lrbp->ucd_rsp_ptr +
				GENERAL_UPIU_REQUEST_SIZE;
		u16 resp_len;
		u16 buf_len;

		/* data segment length */
		resp_len = be32_to_cpu(lrbp->ucd_rsp_ptr->header.dword_2) &
						MASK_QUERY_DATA_SEG_LEN;
		buf_len = be16_to_cpu(
				hba->dev_cmd.query.request.upiu_req.length);
		if (likely(buf_len >= resp_len)) {
			memcpy(hba->dev_cmd.query.descriptor, descp, resp_len);
		} else {
			dev_err(hba->dev,
				"%s: Response size is bigger than buffer",
				__func__);
			return -EINVAL;
		}
	}

	return 0;
}

/**
 * ufshcd_hba_capabilities - Read controller capabilities
 * @hba: per adapter instance
 */
static inline void ufshcd_hba_capabilities(struct ufs_hba *hba)
{
	hba->capabilities = ufshcd_readl(hba, REG_CONTROLLER_CAPABILITIES);

#ifdef CONFIG_HISI_UFS_HC_CORE_UTR
	hba->nutrs = SLOT_NUM_EACH_CORE * CORE_NUM;
#else
	/* nutrs and nutmrs are 0 based values */
	hba->nutrs = (hba->capabilities & MASK_TRANSFER_REQUESTS_SLOTS) + 1;
#endif
	hba->nutmrs =
	((hba->capabilities & MASK_TASK_MANAGEMENT_REQUEST_SLOTS) >> 16) + 1;
}

/**
 * ufshcd_ready_for_uic_cmd - Check if controller is ready
 *                            to accept UIC commands
 * @hba: per adapter instance
 * Return true on success, else false
 */
static inline bool ufshcd_ready_for_uic_cmd(struct ufs_hba *hba)
{
	if (ufshcd_readl(hba, REG_CONTROLLER_STATUS) & UIC_COMMAND_READY)
		return true;
	else
		return false;
}

/**
 * ufshcd_get_upmcrs - Get the power mode change request status
 * @hba: Pointer to adapter instance
 *
 * This function gets the UPMCRS field of HCS register
 * Returns value of UPMCRS field
 */
static inline u8 ufshcd_get_upmcrs(struct ufs_hba *hba)
{
	return (ufshcd_readl(hba, REG_CONTROLLER_STATUS) >> 8) & 0x7;
}

/**
 * ufshcd_dispatch_uic_cmd - Dispatch UIC commands to unipro layers
 * @hba: per adapter instance
 * @uic_cmd: UIC command
 *
 * Mutex must be held.
 */
static inline void
ufshcd_dispatch_uic_cmd(struct ufs_hba *hba, struct uic_command *uic_cmd)
{
	WARN_ON(hba->active_uic_cmd); /*lint !e146 !e665*/

	hba->active_uic_cmd = uic_cmd;
	/* Write Args */
	ufshcd_writel(hba, uic_cmd->argument1, REG_UIC_COMMAND_ARG_1);
	ufshcd_writel(hba, uic_cmd->argument2, REG_UIC_COMMAND_ARG_2);
	ufshcd_writel(hba, uic_cmd->argument3, REG_UIC_COMMAND_ARG_3);

	/* Write UIC Cmd */
	ufshcd_writel(hba, uic_cmd->command & COMMAND_OPCODE_MASK,
		      REG_UIC_COMMAND);
}

/**
 * ufshcd_wait_for_uic_cmd - Wait complectioin of UIC command
 * @hba: per adapter instance
 * @uic_command: UIC command
 *
 * Must be called with mutex held.
 * Returns 0 only if success.
 */
static int
ufshcd_wait_for_uic_cmd(struct ufs_hba *hba, struct uic_command *uic_cmd)
{
	int ret;
	unsigned long flags;

	if (wait_for_completion_timeout(&uic_cmd->done,
					msecs_to_jiffies(UIC_CMD_TIMEOUT)))
		ret = uic_cmd->argument2 & MASK_UIC_COMMAND_RESULT;
	else
		ret = -ETIMEDOUT;

	spin_lock_irqsave(hba->host->host_lock, flags);
	hba->active_uic_cmd = NULL;
	spin_unlock_irqrestore(hba->host->host_lock, flags);

	return ret;
}

/**
 * __ufshcd_send_uic_cmd - Send UIC commands and retrieve the result
 * @hba: per adapter instance
 * @uic_cmd: UIC command
 * @completion: initialize the completion only if this is set to true
 *
 * Identical to ufshcd_send_uic_cmd() expect mutex. Must be called
 * with mutex held and host_lock locked.
 * Returns 0 only if success.
 */
static int
__ufshcd_send_uic_cmd(struct ufs_hba *hba, struct uic_command *uic_cmd,
		      bool completion)
{
	if (!ufshcd_ready_for_uic_cmd(hba)) {
		dev_err(hba->dev,
			"Controller not ready to accept UIC commands\n");
		return -EIO;
	}

	if (completion)
		init_completion(&uic_cmd->done);

	ufshcd_dispatch_uic_cmd(hba, uic_cmd);

	return 0;
}

/**
 * ufshcd_send_uic_cmd - Send UIC commands and retrieve the result
 * @hba: per adapter instance
 * @uic_cmd: UIC command
 *
 * Returns 0 only if success.
 */
static int
ufshcd_send_uic_cmd(struct ufs_hba *hba, struct uic_command *uic_cmd)
{
	int ret;
	unsigned long flags;

	mutex_lock(&hba->uic_cmd_mutex);
	ufshcd_add_delay_before_dme_cmd(hba);

	spin_lock_irqsave(hba->host->host_lock, flags);
	ret = __ufshcd_send_uic_cmd(hba, uic_cmd, true);
	spin_unlock_irqrestore(hba->host->host_lock, flags);
	if (!ret)
		ret = ufshcd_wait_for_uic_cmd(hba, uic_cmd);

	mutex_unlock(&hba->uic_cmd_mutex);

	if (ret) {
		spin_lock_irqsave(hba->host->host_lock, flags);
		dsm_ufs_update_error_info(hba, DSM_UFS_UIC_CMD_ERR);
		spin_unlock_irqrestore(hba->host->host_lock, flags);
		schedule_ufs_dsm_work(hba);
	}
	return ret;
}

/**
 * ufshcd_map_sg - Map scatter-gather list to prdt
 * @lrbp - pointer to local reference block
 *
 * Returns 0 in case of success, non-zero value in case of failure
 */
int ufshcd_map_sg(struct ufs_hba *hba, struct ufshcd_lrb *lrbp)
{
	struct ufshcd_sg_entry *prd_table;
	struct scatterlist *sg;
	struct scsi_cmnd *cmd;
	int sg_segments;
	int i;

	cmd = lrbp->cmd;
	sg_segments = scsi_dma_map(cmd);
	if (sg_segments < 0)
		return sg_segments;

	if (sg_segments) {
		if (hba->quirks & UFSHCD_QUIRK_PRDT_BYTE_GRAN) {
			if (lrbp->is_hisi_utrd)
				lrbp->hisi_utr_descriptor_ptr
					->prd_table_length = cpu_to_le16((u16)(
					sg_segments *
					sizeof(struct ufshcd_sg_entry)));
			else
				lrbp->utr_descriptor_ptr
					->prd_table_length = cpu_to_le16((u16)(
					sg_segments *
					sizeof(struct ufshcd_sg_entry)));
		} else {
			if (lrbp->is_hisi_utrd)
				lrbp->hisi_utr_descriptor_ptr
					->prd_table_length =
					cpu_to_le16((u16)(sg_segments));
			else
				lrbp->utr_descriptor_ptr->prd_table_length =
					cpu_to_le16((u16)(sg_segments));
		}

		prd_table = (struct ufshcd_sg_entry *)lrbp->ucd_prdt_ptr;

		scsi_for_each_sg(cmd, sg, sg_segments, i) {
			prd_table[i].size =
				cpu_to_le32(((u32)sg_dma_len(sg)) - 1);
			prd_table[i].base_addr =
				cpu_to_le32(lower_32_bits(sg->dma_address));
			prd_table[i].upper_addr =
				cpu_to_le32(upper_32_bits(sg->dma_address));
			prd_table[i].reserved = 0;
		}
	} else {
		if (lrbp->is_hisi_utrd)
			lrbp->hisi_utr_descriptor_ptr->prd_table_length = 0;
		else
			lrbp->utr_descriptor_ptr->prd_table_length = 0;
	}

	return 0;
}
EXPORT_SYMBOL(ufshcd_map_sg);

/**
 * ufshcd_enable_intr - enable interrupts
 * @hba: per adapter instance
 * @intrs: interrupt bits
 */
void ufshcd_enable_intr(struct ufs_hba *hba, u32 intrs)
{
	u32 set = ufshcd_readl(hba, REG_INTERRUPT_ENABLE);

	if (hba->ufs_version == UFSHCI_VERSION_10) {
		u32 rw;
		rw = set & INTERRUPT_MASK_RW_VER_10;
		set = rw | ((set ^ intrs) & intrs);
	} else {
		set |= intrs;
	}

	ufshcd_writel(hba, set, REG_INTERRUPT_ENABLE);
}

/**
 * ufshcd_disable_intr - disable interrupts
 * @hba: per adapter instance
 * @intrs: interrupt bits
 */
static void ufshcd_disable_intr(struct ufs_hba *hba, u32 intrs)
{
	u32 set = ufshcd_readl(hba, REG_INTERRUPT_ENABLE);

	if (hba->ufs_version == UFSHCI_VERSION_10) {
		u32 rw;
		rw = (set & INTERRUPT_MASK_RW_VER_10) &
			~(intrs & INTERRUPT_MASK_RW_VER_10);
		set = rw | ((set & intrs) & ~INTERRUPT_MASK_RW_VER_10);

	} else {
		set &= ~intrs;
	}

	ufshcd_writel(hba, set, REG_INTERRUPT_ENABLE);
}

/**
 * ufshcd_prepare_req_desc_hdr() - Fills the requests header
 * descriptor according to request
 * @lrbp: pointer to local reference block
 * @upiu_flags: flags required in the header
 * @cmd_dir: requests data direction
 */
static void ufshcd_prepare_req_desc_hdr(struct ufshcd_lrb *lrbp,
			u32 *upiu_flags, enum dma_data_direction cmd_dir)
{
	struct utp_transfer_req_desc *req_desc = lrbp->utr_descriptor_ptr;
	struct hisi_utp_transfer_req_desc *hisi_req_desc =
		lrbp->hisi_utr_descriptor_ptr;
	u32 data_direction;
	u32 dword_0;

	if (cmd_dir == DMA_FROM_DEVICE) {
		data_direction = UTP_DEVICE_TO_HOST;
		*upiu_flags = UPIU_CMD_FLAGS_READ;
	} else if (cmd_dir == DMA_TO_DEVICE) {
		data_direction = UTP_HOST_TO_DEVICE;
		*upiu_flags = UPIU_CMD_FLAGS_WRITE;
	} else {
		data_direction = UTP_NO_DATA_TRANSFER;
		*upiu_flags = UPIU_CMD_FLAGS_NONE;
	}
#ifdef CONFIG_HISI_BLK
	if(unlikely(lrbp->cmd && lrbp->cmd->request && req_hoq(lrbp->cmd->request)))
		*upiu_flags |= UPIU_TASK_ATTR_HEADQ;
	if(unlikely(lrbp->cmd && lrbp->cmd->request && req_cp(lrbp->cmd->request)))
		*upiu_flags |= UPIU_CMD_PRIO;
#endif
	/* UFSHCI only support UPIU_COMMAND_SET_TYPE_UFS */
	dword_0 = data_direction |
		  (UPIU_COMMAND_SET_TYPE_UFS << UPIU_COMMAND_TYPE_OFFSET);

	if (lrbp->intr_cmd)
		dword_0 |= UTP_REQ_DESC_INT_CMD;

	if (lrbp->is_hisi_utrd) {
		/* Transfer request descriptor header fields */
		hisi_req_desc->header.dword_0 = cpu_to_le32(dword_0);
		/* dword_1 is reserved, hence it is set to 0 */
		hisi_req_desc->header.dword_1 = 0;
		/*
		 * assigning invalid value for command status. Controller
		 * updates OCS on command completion, with the command
		 * status
		 */
		hisi_req_desc->header.dword_2 =
			cpu_to_le32(OCS_INVALID_COMMAND_STATUS);
		/* dword_3 is reserved, hence it is set to 0 */
		hisi_req_desc->header.dword_3 = 0;

		hisi_req_desc->prd_table_length = 0;
	} else {

		/* Transfer request descriptor header fields */
		req_desc->header.dword_0 = cpu_to_le32(dword_0);
		/* dword_1 is reserved, hence it is set to 0 */
		req_desc->header.dword_1 = 0;
		/*
		 * assigning invalid value for command status. Controller
		 * updates OCS on command completion, with the command
		 * status
		 */
		req_desc->header.dword_2 =
			cpu_to_le32(OCS_INVALID_COMMAND_STATUS);
		/* dword_3 is reserved, hence it is set to 0 */
		req_desc->header.dword_3 = 0;

		req_desc->prd_table_length = 0;
	}
}

/**
 * ufshcd_prepare_utp_scsi_cmd_upiu() - fills the utp_transfer_req_desc,
 * for scsi commands
 * @lrbp - local reference block pointer
 * @upiu_flags - flags
 */
static
void ufshcd_prepare_utp_scsi_cmd_upiu(struct ufshcd_lrb *lrbp, u32 upiu_flags)
{
	struct utp_upiu_req *ucd_req_ptr = lrbp->ucd_req_ptr;
	unsigned short cdb_len;

	/* command descriptor fields */
	ucd_req_ptr->header.dword_0 = UPIU_HEADER_DWORD(
				UPIU_TRANSACTION_COMMAND, upiu_flags,
				lrbp->lun, lrbp->task_tag);
	ucd_req_ptr->header.dword_1 = UPIU_HEADER_DWORD(
				UPIU_COMMAND_SET_TYPE_SCSI, 0, 0, 0);

	/* Total EHS length and Data segment length will be zero */
	ucd_req_ptr->header.dword_2 = 0;

	ucd_req_ptr->sc.exp_data_transfer_len =
		cpu_to_be32(lrbp->cmd->sdb.length);

	cdb_len = min_t(unsigned short, lrbp->cmd->cmd_len, MAX_CDB_SIZE);
	memset(ucd_req_ptr->sc.cdb, 0, MAX_CDB_SIZE);
	memcpy(ucd_req_ptr->sc.cdb, lrbp->cmd->cmnd, cdb_len);

	memset(lrbp->ucd_rsp_ptr, 0, sizeof(struct utp_upiu_rsp));
}

/**
 * ufshcd_prepare_utp_query_req_upiu() - fills the utp_transfer_req_desc,
 * for query requsts
 * @hba: UFS hba
 * @lrbp: local reference block pointer
 * @upiu_flags: flags
 */
static void ufshcd_prepare_utp_query_req_upiu(struct ufs_hba *hba,
				struct ufshcd_lrb *lrbp, u32 upiu_flags)
{
	struct utp_upiu_req *ucd_req_ptr = lrbp->ucd_req_ptr;
	struct ufs_query *query = &hba->dev_cmd.query;
	u16 len = be16_to_cpu(query->request.upiu_req.length);
	u8 *descp = (u8 *)lrbp->ucd_req_ptr + GENERAL_UPIU_REQUEST_SIZE;

	/* Query request header */
	ucd_req_ptr->header.dword_0 = UPIU_HEADER_DWORD(
			UPIU_TRANSACTION_QUERY_REQ, upiu_flags,
			lrbp->lun, lrbp->task_tag);
	ucd_req_ptr->header.dword_1 = UPIU_HEADER_DWORD(
			0, query->request.query_func, 0, 0);

	/* Data segment length only need for WRITE_DESC */
	if (query->request.upiu_req.opcode == UPIU_QUERY_OPCODE_WRITE_DESC ||
	    query->request.has_data)
		ucd_req_ptr->header.dword_2 =
			UPIU_HEADER_DWORD(0, 0, (len >> 8), (u8)len);
	else
		ucd_req_ptr->header.dword_2 = 0;

	/* Copy the Query Request buffer as is */
	memcpy(&ucd_req_ptr->qr, &query->request.upiu_req,
			QUERY_OSF_SIZE);

	/* Copy the Descriptor */
	if (query->request.upiu_req.opcode == UPIU_QUERY_OPCODE_WRITE_DESC ||
	    query->request.has_data)
		memcpy(descp, query->descriptor, len);

	memset(lrbp->ucd_rsp_ptr, 0, sizeof(struct utp_upiu_rsp));
}

static inline void ufshcd_prepare_utp_nop_upiu(struct ufshcd_lrb *lrbp)
{
	struct utp_upiu_req *ucd_req_ptr = lrbp->ucd_req_ptr;

	memset(ucd_req_ptr, 0, sizeof(struct utp_upiu_req));

	/* command descriptor fields */
	ucd_req_ptr->header.dword_0 =
		UPIU_HEADER_DWORD(
			UPIU_TRANSACTION_NOP_OUT, 0, 0, lrbp->task_tag);
	/* clear rest of the fields of basic header */
	ucd_req_ptr->header.dword_1 = 0;
	ucd_req_ptr->header.dword_2 = 0;

	memset(lrbp->ucd_rsp_ptr, 0, sizeof(struct utp_upiu_rsp));
}

/**
 * ufshcd_comp_devman_upiu - UFS Protocol Information Unit(UPIU)
 *			     for Device Management Purposes
 * @hba - per adapter instance
 * @lrb - pointer to local reference block
 */
static int ufshcd_comp_devman_upiu(struct ufs_hba *hba, struct ufshcd_lrb *lrbp)
{
	u32 upiu_flags;
	int ret = 0;

	if (hba->ufs_version == UFSHCI_VERSION_20)
		lrbp->command_type = UTP_CMD_TYPE_UFS_STORAGE;
	else
		lrbp->command_type = UTP_CMD_TYPE_DEV_MANAGE;

	ufshcd_prepare_req_desc_hdr(lrbp, &upiu_flags, DMA_NONE);
	if (hba->dev_cmd.type == DEV_CMD_TYPE_QUERY)
		ufshcd_prepare_utp_query_req_upiu(hba, lrbp, upiu_flags);
	else if (hba->dev_cmd.type == DEV_CMD_TYPE_NOP)
		ufshcd_prepare_utp_nop_upiu(lrbp);
	else
		ret = -EINVAL;

	return ret;
}

/**
 * ufshcd_comp_scsi_upiu - UFS Protocol Information Unit(UPIU)
 *			   for SCSI Purposes
 * @hba - per adapter instance
 * @lrb - pointer to local reference block
 */
static int ufshcd_comp_scsi_upiu(struct ufs_hba *hba, struct ufshcd_lrb *lrbp)
{
	u32 upiu_flags;
	int ret = 0;

	if (hba->ufs_version == UFSHCI_VERSION_20)
		lrbp->command_type = UTP_CMD_TYPE_UFS_STORAGE;
	else
		lrbp->command_type = UTP_CMD_TYPE_SCSI;

	if (likely(lrbp->cmd)) {
		ufshcd_prepare_req_desc_hdr(lrbp, &upiu_flags,
					    lrbp->cmd->sc_data_direction);
		ufshcd_prepare_utp_scsi_cmd_upiu(lrbp, upiu_flags);

		ufshpb_compose_upiu(hba, lrbp);
#ifdef CONFIG_SCSI_UFS_INLINE_CRYPTO
		ufshcd_prepare_req_desc_uie(hba, lrbp);
#endif
		ufshcd_custom_upiu_build(hba, lrbp);
	} else {
		ret = -EINVAL;
	}

	return ret;
}

/*
 * ufshcd_scsi_to_upiu_lun - maps scsi LUN to UPIU LUN
 * @scsi_lun: scsi LUN id
 *
 * Returns UPIU LUN id
 */
/*lint -save -e695*/
inline u8 ufshcd_scsi_to_upiu_lun(unsigned int scsi_lun)
{
	if (scsi_is_wlun(scsi_lun))
		return (scsi_lun & UFS_UPIU_MAX_UNIT_NUM_ID)
			| UFS_UPIU_WLUN_ID;
	else
		return scsi_lun & UFS_UPIU_MAX_UNIT_NUM_ID;
}
EXPORT_SYMBOL(ufshcd_scsi_to_upiu_lun);
/*lint -restore*/
/**
 * ufshcd_upiu_wlun_to_scsi_wlun - maps UPIU W-LUN id to SCSI W-LUN ID
 * @scsi_lun: UPIU W-LUN id
 *
 * Returns SCSI W-LUN id
 */
static inline u16 ufshcd_upiu_wlun_to_scsi_wlun(u8 upiu_wlun_id)
{
	return (upiu_wlun_id & ~UFS_UPIU_WLUN_ID) | SCSI_W_LUN_BASE;
}

int wait_for_ufs_all_complete(struct ufs_hba *hba, int timeout_ms)
{
	u64 tr_doobell;
	if (!hba)
		return -EINVAL;
	while (timeout_ms-- > 0) {
		tr_doobell = read_utr_doorbell(hba);
		if (!tr_doobell)
			return 0;
		udelay(1000);
	}
	dev_err(hba->dev, "%s: tr_doobell: 0x%lx\n", __func__, tr_doobell);
	return -ETIMEDOUT;
}

static int ufshcd_tag_alloc(struct Scsi_Host *host, struct ufs_hba *hba,
						struct scsi_cmnd *cmd)
{
	int tag;
#ifdef CONFIG_HISI_UFS_HC_CORE_UTR
	tag = get_tag_per_cpu(hba, qos_ufs_mq_get_send_cpu(cmd->request));
	if (tag < 0)
		return -EINVAL;
#else
	if (host->queue_quirk_flag & SHOST_QUIRK(SHOST_QUIRK_DRIVER_TAG_ALLOC)) {
		tag = (int)ffz(hba->lrb_in_use);
		if (tag >= hba->nutrs)
			return -EINVAL;
	} else {
		tag = cmd->request->tag;
	}
#endif
	cmd->tag = (unsigned char)tag;
	return 0;
}

#ifndef CONFIG_HISI_UFS_HC_CORE_UTR
static void ufshcd_validate_tag(struct Scsi_Host *host, struct ufs_hba *hba,
								struct scsi_cmnd *cmd)
{
	int tag;

	if (!(host->queue_quirk_flag & SHOST_QUIRK(SHOST_QUIRK_DRIVER_TAG_ALLOC))) {
		tag = cmd->request->tag;
		if (!ufshcd_valid_tag(hba, tag)) {
			dev_err(hba->dev,
				"%s: invalid command tag %d: cmd=0x%pK, cmd->request=0x%pK",
				__func__, tag, cmd, cmd->request);
			BUG(); /*lint !e146*/
		}
	}
}
#else
static void ufshcd_validate_tag(struct Scsi_Host *host, struct ufs_hba *hba,
							struct scsi_cmnd *cmd)
{
}
#endif

#ifdef CONFIG_SCSI_UFS_ENHANCED_INLINE_CRYPTO_V3
static int fbe3_block_in_driver(const struct request *request)
{
	int err = 0;
	bool is_encrypt_file = request->hisi_req.ci_key ? true : false;

	if (is_encrypt_file) {
		int file_encrypt_type = (u32)request->hisi_req.ci_key_index >>
					FILE_ENCRY_TYPE_BEGIN_BIT;
		u32 slot_id = (u32)request->hisi_req.ci_key_index &
			      FILE_ENCRY_TYPE_MASK;

		if (file_encrypt_type == ECE) {
			if (hisi_fbex_slot_clean(slot_id)) {
				pr_err("[FBE3]%s: key slot is cleared, illegal to send ECE IO\n",
				       __func__);
				err = -EOPNOTSUPP;
			}
		}
	}

	return err;
}
#endif

#ifdef CONFIG_HISI_BLOCK_ORDER_PRESERVING
#if defined(CONFIG_HISI_DEBUG_FS) || defined(CONFIG_HISI_BLK_DEBUG)
extern int hisi_blk_order_debug_en(void);
#endif

static int ufshcd_custom_upiu_order(struct utp_upiu_req *ucd_req_ptr,
				     struct request *req,
				     struct scsi_cmnd *scmd,
				     struct ufs_hba *hba)
{
	unsigned int wo_nr;

	if (unlikely(!req || !req->q))
		return 0;

#ifdef CONFIG_HISI_UFS_HC_CORE_UTR
	if (blk_queue_query_order_enable(req->q) &&
		(req_op(req) != REQ_OP_READ) && (!req_cp(req)) &&
		(hweight64(hba->outstanding_reqs) >= SPEC_SLOT_NUM))/*lint !e514*/
		return -EAGAIN;
#endif

	if (scsi_is_order_cmd(scmd)) {
		wo_nr = blk_req_get_order_nr(req, true);
		if (wo_nr) {
#if defined(CONFIG_HISI_DEBUG_FS) || defined(CONFIG_HISI_BLK_DEBUG)
			if (hisi_blk_order_debug_en())
				pr_err("cmd = 0x%x, order_nr = %u \r\n",
						scmd->cmnd[0], wo_nr);
#endif
			/* CDB 12-15 for Command Order */
			ucd_req_ptr->sc.cdb[12] = (unsigned char)(wo_nr);
			ucd_req_ptr->sc.cdb[13] = (unsigned char)(wo_nr >> 8);
			ucd_req_ptr->sc.cdb[14] = (unsigned char)(wo_nr >> 16);
			ucd_req_ptr->sc.cdb[15] = (unsigned char)(wo_nr >> 24);
		}
	}

	return 0;
}
#endif


/**
 * ufshcd_queuecommand - main entry point for SCSI requests
 * @cmd: command from SCSI Midlayer
 * @done: call back function
 *
 * Returns 0 for success, non-zero in case of failure
 */
static int ufshcd_queuecommand(struct Scsi_Host *host, struct scsi_cmnd *cmd)
{
	struct ufshcd_lrb *lrbp;
	struct ufs_hba *hba;
	unsigned long flags;
	int tag;
	int err = 0;

#ifdef CONFIG_SCSI_UFS_ENHANCED_INLINE_CRYPTO_V3
	err = fbe3_block_in_driver(cmd->request);
	if (err)
		goto out;
#endif

	hba = shost_priv(host);
	ufshcd_validate_tag(host, hba, cmd);
	spin_lock_irqsave(hba->host->host_lock, flags);
	if (ufshcd_tag_alloc(host, hba, cmd)) {
		err = SCSI_MLQUEUE_HOST_BUSY;
		goto out_unlock;
	}
	tag = (int)cmd->tag;
	switch (hba->ufshcd_state) {
	case UFSHCD_STATE_OPERATIONAL:
		break;
	case UFSHCD_STATE_FFU:
		if (cmd->cmnd[0] != WRITE_BUFFER ||
			(cmd->cmnd[1] & WB_MODE_MASK) != DOWNLOAD_MODE) {
			dev_dbg(hba->dev, "%s: ffu in progress\n", __func__);
			err = SCSI_MLQUEUE_HOST_BUSY;
			goto out_unlock;
		}
		break;
	case UFSHCD_STATE_EH_SCHEDULED:
	case UFSHCD_STATE_RESET:
		err = SCSI_MLQUEUE_HOST_BUSY;
		goto out_unlock;
	case UFSHCD_STATE_ERROR:
		set_host_byte(cmd, DID_ERROR);
		cmd->scsi_done(cmd);
		goto out_unlock;
	default:
		dev_WARN_ONCE(hba->dev, 1, "%s: invalid state %d\n",
				__func__, hba->ufshcd_state); /*lint !e146 !e665*/
		set_host_byte(cmd, DID_BAD_TARGET);
		cmd->scsi_done(cmd);
		goto out_unlock;
	}

	if (unlikely(cmd->cmnd[0] == WRITE_BUFFER &&
			(cmd->cmnd[1] & WB_MODE_MASK) == DOWNLOAD_MODE)) {
		hba->ufshcd_state = UFSHCD_STATE_FFU;
		/* wait for ufs all complete timeout time 1s */
		err = wait_for_ufs_all_complete(hba, 1000);
		dev_err(hba->dev,
			"%s: FFU process, wait_for_ufs_all_complete:%d",
			__func__, err);
	}

	/* if error handling is in progress, don't issue commands */
	if (ufshcd_eh_in_progress(hba)) {
		err = SCSI_MLQUEUE_HOST_BUSY;
		goto out_unlock;
	}

	if (!(host->queue_quirk_flag & SHOST_QUIRK(SHOST_QUIRK_DRIVER_TAG_ALLOC)))
		spin_unlock_irqrestore(hba->host->host_lock, flags);

	hba->req_abort_count = 0;

	/* acquire the tag to make sure device cmds don't use it */
	if (test_and_set_bit_lock(tag, &hba->lrb_in_use)) {
		/*
		 * Dev manage command in progress, requeue the command.
		 * Requeuing the command helps in cases where the request *may*
		 * find different tag instead of waiting for dev manage command
		 * completion.
		 */
		dev_warn(
		    hba->dev,
		    "%s: get tag(%d) failed, dev manage may in progress.\n",
		    __func__, tag);
		err = SCSI_MLQUEUE_HOST_BUSY;
		if (!(host->queue_quirk_flag & SHOST_QUIRK(SHOST_QUIRK_DRIVER_TAG_ALLOC)))
			goto out;
		else
			goto out_unlock;
	}

	if (host->queue_quirk_flag & SHOST_QUIRK(SHOST_QUIRK_DRIVER_TAG_ALLOC))
		spin_unlock_irqrestore(hba->host->host_lock, flags);

	do {
		spin_lock_irqsave(hba->host->host_lock, flags);
		if (hba->ufshcd_state == UFSHCD_STATE_RESET ||
			hba->ufshcd_state == UFSHCD_STATE_EH_SCHEDULED ||
			hba->force_host_reset) {
			dev_dbg(hba->dev, "%s: reset in progress\n", __func__);
			clear_bit_unlock(tag, &hba->lrb_in_use);
			err = SCSI_MLQUEUE_HOST_BUSY;
			goto out_unlock;
		}
		spin_unlock_irqrestore(hba->host->host_lock, flags);
	} while (0);

	err = pm_runtime_get_sync(hba->dev);
	if (err < 0) {
		clear_bit_unlock(tag, &hba->lrb_in_use);
		err = SCSI_MLQUEUE_HOST_BUSY;
		pm_runtime_put_sync(hba->dev);
		goto out;
	}

	spin_lock_irqsave(hba->host->host_lock, flags);
	if (hba->ufshcd_state == UFSHCD_STATE_RESET ||
	    hba->ufshcd_state == UFSHCD_STATE_EH_SCHEDULED) {
		dev_err(hba->dev, "%s: reset in progress after H8\n", __func__);
		clear_bit_unlock(tag, &hba->lrb_in_use);
		err = SCSI_MLQUEUE_HOST_BUSY;
		spin_unlock_irqrestore(hba->host->host_lock, flags);
		pm_runtime_put_sync(hba->dev);
		goto out;
	}
	spin_unlock_irqrestore(hba->host->host_lock, flags);

	WARN_ON(hba->is_hibernate); /*lint !e146 !e665*/

	lrbp = &hba->lrb[tag];

	lrbp->cmd = cmd;
	lrbp->sense_bufflen = UFSHCD_REQ_SENSE_SIZE;
	lrbp->sense_buffer = cmd->sense_buffer;
	lrbp->saved_sense_len = 0;
	lrbp->task_tag = tag;
	lrbp->lun = ufshcd_scsi_to_upiu_lun(cmd->device->lun);
	lrbp->intr_cmd = !ufshcd_is_intr_aggr_allowed(hba) ? true : false;
	lrbp->req_abort_skip = false;

	ufshcd_start_delay_work(hba, cmd);

	err = ufshcd_comp_scsi_upiu(hba, lrbp);
	if (err) {
		lrbp->cmd = NULL;
		clear_bit_unlock(tag, &hba->lrb_in_use);
		goto out;
	}

	err = ufshcd_map_sg(hba, lrbp);
	if (err) {
		lrbp->cmd = NULL;
		clear_bit_unlock(tag, &hba->lrb_in_use);
		goto out;
	}
	/* Make sure descriptors are ready before ringing the doorbell */
	wmb();

	/* issue command to the controller */
	spin_lock_irqsave(hba->host->host_lock, flags);
#ifdef CONFIG_HISI_UFS_HC
	if (ufshcd_is_ufs_dev_poweroff(hba)) {
		dev_err(hba->dev, "warnning send cmd in ufs shutdown\n");
		dump_stack();
		err = SCSI_MLQUEUE_HOST_BUSY;
		goto out_unlock;
	}
#endif
	while (hba->is_hibernate) {
		spin_unlock_irqrestore(hba->host->host_lock, flags);
		dev_info(hba->dev, "warning send cmd in H8, %s\n", __func__);
		WARN_ON(1); /*lint !e146 !e665*/
		err = ufshcd_uic_hibern8_exit(hba);
		if (err) {
			lrbp->cmd = NULL;
			clear_bit_unlock(tag, &hba->lrb_in_use);
			goto out;
		}
		spin_lock_irqsave(hba->host->host_lock, flags);
	}
#ifdef CONFIG_HISI_BLOCK_ORDER_PRESERVING
	/* it has to be put here, without any goto branches behind */
	if (ufshcd_custom_upiu_order(lrbp->ucd_req_ptr, lrbp->cmd->request,
				 lrbp->cmd, hba)) {
		clear_bit_unlock(tag, &hba->lrb_in_use);
		err = SCSI_MLQUEUE_HOST_BUSY;
		spin_unlock_irqrestore(hba->host->host_lock, flags);
		pm_runtime_put_sync(hba->dev);
		goto out;
	}
#endif
	ufshcd_check_disable_dev_tmt_cnt(hba, cmd);
#ifdef CONFIG_HISI_SCSI_UFS_DUMP
	ufshcd_dump_scsi_command(hba, tag);
#endif
#ifdef CONFIG_HISI_UFS_HC_CORE_UTR
	set_core_utr_slot_qos(hba, tag, cmd->request->hisi_req.hisi_rq_qos);
#endif /* CONFIG_HISI_UFS_HC_CORE_UTR */
	ufshcd_send_command(hba, tag);
out_unlock:
	spin_unlock_irqrestore(hba->host->host_lock, flags);

out:
	return err;
}

static int ufshcd_compose_dev_cmd(struct ufs_hba *hba,
		struct ufshcd_lrb *lrbp, enum dev_cmd_type cmd_type, int tag)
{
	lrbp->cmd = NULL;
	lrbp->sense_bufflen = 0;
	lrbp->sense_buffer = NULL;
	lrbp->saved_sense_len = 0;
	lrbp->task_tag = tag;
	lrbp->lun = 0; /* device management cmd is not specific to any LUN */
	lrbp->intr_cmd = true; /* No interrupt aggregation */
	hba->dev_cmd.type = cmd_type;
	/* some 186x vcmd need update lun */
	ufshcd_update_dev_cmd_lun(hba, lrbp);

	return ufshcd_comp_devman_upiu(hba, lrbp);
}

static int
ufshcd_clear_cmd(struct ufs_hba *hba, int tag)
{
	int err = 0;
	unsigned long flags;
#ifdef CONFIG_HISI_UFS_HC_CORE_UTR
	u32 mask = 1UL << (u32)(tag % SLOT_NUM_EACH_CORE);
#else
	u32 mask = 1U << (u32)tag;
#endif

	/* clear outstanding transaction before retry */
	spin_lock_irqsave(hba->host->host_lock, flags);
	ufshcd_utrl_clear(hba, tag);
	spin_unlock_irqrestore(hba->host->host_lock, flags);

	/*
	 * wait for for h/w to clear corresponding bit in door-bell.
	 * max. wait is 1 sec.
	 */
#ifdef CONFIG_HISI_UFS_HC_CORE_UTR
	err = ufshcd_wait_for_register(hba,
			UFS_CORE_UTRLDBR(tag / SLOT_NUM_EACH_CORE),
			mask, ~mask, 1000, 1000, true);
#else
	err = ufshcd_wait_for_register(hba,
			REG_UTP_TRANSFER_REQ_DOOR_BELL,
			mask, ~mask, 1000, 1000, true);
#endif

	return err;
}

static int
ufshcd_check_query_response(struct ufs_hba *hba, struct ufshcd_lrb *lrbp)
{
	struct ufs_query_res *query_res = &hba->dev_cmd.query.response;

	/* Get the UPIU response */
	query_res->response = ufshcd_get_rsp_upiu_result(lrbp->ucd_rsp_ptr) >>
				UPIU_RSP_CODE_OFFSET;
	return query_res->response;
}

/**
 * ufshcd_dev_cmd_completion() - handles device management command responses
 * @hba: per adapter instance
 * @lrbp: pointer to local reference block
 */
static int
ufshcd_dev_cmd_completion(struct ufs_hba *hba, struct ufshcd_lrb *lrbp)
{
	int resp;
	int err = 0;

	hba->ufs_stats.last_hibern8_exit_tstamp = ktime_set(0, 0);
	resp = ufshcd_get_req_rsp(lrbp->ucd_rsp_ptr);

	switch (resp) {
	case UPIU_TRANSACTION_NOP_IN:
		if (hba->dev_cmd.type != DEV_CMD_TYPE_NOP) {
			err = -EINVAL;
			dev_err(hba->dev, "%s: unexpected response %x\n",
					__func__, resp);
		}
		break;
	case UPIU_TRANSACTION_QUERY_RSP:
		err = ufshcd_check_query_response(hba, lrbp);
		if (!err)
			err = ufshcd_copy_query_response(hba, lrbp);
		break;
	case UPIU_TRANSACTION_REJECT_UPIU:
		/* TODO: handle Reject UPIU Response */
		err = -EPERM;
		dev_err(hba->dev, "%s: Reject UPIU not fully implemented\n",
				__func__);
		break;
	default:
		err = -EINVAL;
		dev_err(hba->dev, "%s: Invalid device management cmd response: %x\n",
				__func__, resp);
		break;
	}

	return err;
}

static int ufshcd_wait_for_dev_cmd(struct ufs_hba *hba,
		struct ufshcd_lrb *lrbp, int max_timeout)
{
	int err = 0;
	unsigned long time_left;
	unsigned long flags;

	time_left = wait_for_completion_timeout(hba->dev_cmd.complete,
			msecs_to_jiffies(max_timeout));

	/* Make sure descriptors are ready before ringing the doorbell */
	wmb();
	spin_lock_irqsave(hba->host->host_lock, flags);
	hba->dev_cmd.complete = NULL;
	if (likely(time_left)) {
		err = ufshcd_get_tr_ocs(lrbp);
		if (!err)
			err = ufshcd_dev_cmd_completion(hba, lrbp);
	}
	spin_unlock_irqrestore(hba->host->host_lock, flags);

	if (!time_left) {
		err = -ETIMEDOUT;
		dev_dbg(hba->dev, "%s: dev_cmd request timedout, tag %d\n",
			__func__, lrbp->task_tag);
		if (!ufshcd_clear_cmd(hba, lrbp->task_tag))
			/* successfully cleared the command, retry if needed */
			err = -EAGAIN;
		/*
		 * in case of an error, after clearing the doorbell,
		 * we also need to clear the outstanding_request
		 * field in hba
		 */
		ufshcd_outstanding_req_clear(hba, lrbp->task_tag);
	}

	return err;
}

/**
 * ufshcd_get_dev_cmd_tag - Get device management command tag
 * @hba: per-adapter instance
 * @tag: pointer to variable with available slot value
 *
 * Get a free slot and lock it until device management command
 * completes.
 *
 * Returns false if free slot is unavailable for locking, else
 * return true with tag value in @tag.
 */
bool ufshcd_get_dev_cmd_tag(struct ufs_hba *hba, int *tag_out)
{
	int tag;
	bool ret = false;
	unsigned long tmp;

	if (!tag_out)
		goto out;

	do {
		tmp = ~hba->lrb_in_use;
		tag = find_last_bit(&tmp, hba->nutrs);
		if (tag >= hba->nutrs)
			goto out;
	} while (test_and_set_bit_lock(tag, &hba->lrb_in_use));

	*tag_out = tag;
	ret = true;
out:
	return ret;
}
EXPORT_SYMBOL(ufshcd_get_dev_cmd_tag);

static inline void ufshcd_put_dev_cmd_tag(struct ufs_hba *hba, int tag)
{
	clear_bit_unlock(tag, &hba->lrb_in_use);
}

/**
 * ufshcd_exec_dev_cmd - API for sending device management requests
 * @hba - UFS hba
 * @cmd_type - specifies the type (NOP, Query...)
 * @timeout - time in seconds
 *
 * NOTE: Since there is only one available tag for device management commands,
 * it is expected you hold the hba->dev_cmd.lock mutex.
 */
int ufshcd_exec_dev_cmd(struct ufs_hba *hba,
		enum dev_cmd_type cmd_type, int timeout)
{
	struct ufshcd_lrb *lrbp;
	int err;
	int tag;
	struct completion wait;
	unsigned long flags;

	down_read(&hba->clk_scaling_lock);

	/*
	 * Get free slot, sleep if slots are unavailable.
	 * Even though we use wait_event() which sleeps indefinitely,
	 * the maximum wait time is bounded by SCSI request timeout.
	 */

	spin_lock_irqsave(hba->host->host_lock, flags);
	ufshcd_transfer_req_compl(hba);
	spin_unlock_irqrestore(hba->host->host_lock, flags);

	wait_event(hba->dev_cmd.tag_wq, ufshcd_get_dev_cmd_tag(hba, &tag));/*lint !e666*/

	pm_runtime_get_sync(hba->dev);

	init_completion(&wait);
	lrbp = &hba->lrb[tag];
	WARN_ON(lrbp->cmd); /*lint !e146 !e665 !e730*/

	err = ufshcd_compose_dev_cmd(hba, lrbp, cmd_type, tag);
	if (unlikely(err))
		goto out_put_tag;

	hba->dev_cmd.complete = &wait;
	/* Make sure descriptors are ready before ringing the doorbell */
	wmb();
	spin_lock_irqsave(hba->host->host_lock, flags);
	while (hba->is_hibernate) {
		spin_unlock_irqrestore(hba->host->host_lock, flags);
		dev_info(hba->dev, "warring send cmd in H8, %s\n", __func__);
		err = ufshcd_uic_hibern8_exit(hba);
		if (err)
			goto out_put_tag;
		spin_lock_irqsave(hba->host->host_lock, flags);
	}
	/*lint -restore*/
	ufshcd_vops_setup_xfer_req(hba, tag, (lrbp->cmd ? true : false));
	ufshcd_send_command(hba, tag);
	spin_unlock_irqrestore(hba->host->host_lock, flags);

	err = ufshcd_wait_for_dev_cmd(hba, lrbp, timeout);

out_put_tag:
	ufshcd_put_dev_cmd_tag(hba, tag);
	wake_up(&hba->dev_cmd.tag_wq);
	up_read(&hba->clk_scaling_lock);
	pm_runtime_mark_last_busy(hba->dev);
	pm_runtime_put_autosuspend(hba->dev);
	return err;
}

/**
 * ufshcd_init_query() - init the query response and request parameters
 * @hba: per-adapter instance
 * @request: address of the request pointer to be initialized
 * @response: address of the response pointer to be initialized
 * @opcode: operation to perform
 * @idn: flag idn to access
 * @index: LU number to access
 * @selector: query/flag/descriptor further identification
 */
inline void ufshcd_init_query(struct ufs_hba *hba,
		struct ufs_query_req **request, struct ufs_query_res **response,
		enum query_opcode opcode, u8 idn, u8 index, u8 selector)
{/*lint !e695*/
	*request = &hba->dev_cmd.query.request;
	*response = &hba->dev_cmd.query.response;
	memset(*request, 0, sizeof(struct ufs_query_req));
	memset(*response, 0, sizeof(struct ufs_query_res));
	(*request)->upiu_req.opcode = opcode;
	(*request)->upiu_req.idn = idn;
	(*request)->upiu_req.index = index;
	(*request)->upiu_req.selector =
		ufshcd_wb_map_selector(hba, opcode, idn, index, selector);
}

int ufshcd_query_flag_retry(struct ufs_hba *hba, enum query_opcode opcode,
			    enum flag_idn idn, bool *flag_res)
{
	int ret;
	int retries;

	for (retries = 0; retries < QUERY_REQ_RETRIES; retries++) {
		ret = ufshcd_query_flag(hba, opcode, idn, flag_res);
		if (ret)
			dev_dbg(hba->dev,
				"%s: failed with error %d, retries %d\n",
				__func__, ret, retries);
		else
			break;
	}

	if (ret)
		dev_err(hba->dev,
			"%s: query attribute, opcode %d, idn %d, failed with error %d after %d retires\n",
			__func__, opcode, idn, ret, retries);
	return ret;
}

/**
 * ufshcd_query_flag() - API function for sending flag query requests
 * hba: per-adapter instance
 * query_opcode: flag query to perform
 * idn: flag idn to access
 * flag_res: the flag value after the query request completes
 *
 * Returns 0 for success, non-zero in case of failure
 */
int ufshcd_query_flag(struct ufs_hba *hba, enum query_opcode opcode,
			enum flag_idn idn, bool *flag_res)
{
	struct ufs_query_req *request = NULL;
	struct ufs_query_res *response = NULL;
	int err, index = 0, selector = 0;
	int timeout = QUERY_REQ_TIMEOUT;

	BUG_ON(!hba);

	mutex_lock(&hba->dev_cmd.lock);
	ufshcd_init_query(hba, &request, &response, opcode, idn, index,
			selector);

	switch (opcode) {
	case UPIU_QUERY_OPCODE_SET_FLAG:
	case UPIU_QUERY_OPCODE_CLEAR_FLAG:
	case UPIU_QUERY_OPCODE_TOGGLE_FLAG:
		request->query_func = UPIU_QUERY_FUNC_STANDARD_WRITE_REQUEST;
		break;
	case UPIU_QUERY_OPCODE_READ_FLAG:
		request->query_func = UPIU_QUERY_FUNC_STANDARD_READ_REQUEST;
		if (!flag_res) {
			/* No dummy reads */
			dev_err(hba->dev, "%s: Invalid argument for read request\n",
					__func__);
			err = -EINVAL;
			goto out_unlock;
		}
		break;
	default:
		dev_err(hba->dev,
			"%s: Expected query flag opcode but got = %d\n",
			__func__, opcode);
		err = -EINVAL;
		goto out_unlock;
	}

	err = ufshcd_exec_dev_cmd(hba, DEV_CMD_TYPE_QUERY, timeout);

	if (err) {
		dev_err(hba->dev,
			"%s: Sending flag query for idn %d failed, err = %d\n",
			__func__, idn, err);
		goto out_unlock;
	}

	if (flag_res)
		*flag_res = (be32_to_cpu(response->upiu_res.value) &
				MASK_QUERY_UPIU_FLAG_LOC) & 0x1;

out_unlock:
	mutex_unlock(&hba->dev_cmd.lock);
	return err;
}
EXPORT_SYMBOL(ufshcd_query_flag);

/**
 * ufshcd_query_attr - API function for sending attribute requests
 * hba: per-adapter instance
 * opcode: attribute opcode
 * idn: attribute idn to access
 * index: index field
 * selector: selector field
 * attr_val: the attribute value after the query request completes
 *
 * Returns 0 for success, non-zero in case of failure
*/
int ufshcd_query_attr(struct ufs_hba *hba, enum query_opcode opcode,
	enum attr_idn idn, u8 index, u8 selector, u32 *attr_val)
{
	struct ufs_query_req *request = NULL;
	struct ufs_query_res *response = NULL;
	int err;

	BUG_ON(!hba);

	if (!attr_val) {
		dev_err(hba->dev, "%s: attribute value required for opcode 0x%x\n",
				__func__, opcode);
		err = -EINVAL;
		goto out;
	}

	mutex_lock(&hba->dev_cmd.lock);
	ufshcd_init_query(hba, &request, &response, opcode, idn, index,
			selector);

	switch (opcode) {
	case UPIU_QUERY_OPCODE_WRITE_ATTR:
		request->query_func = UPIU_QUERY_FUNC_STANDARD_WRITE_REQUEST;
		request->upiu_req.value = cpu_to_be32(*attr_val);
		break;
	case UPIU_QUERY_OPCODE_READ_ATTR:
		request->query_func = UPIU_QUERY_FUNC_STANDARD_READ_REQUEST;
		break;
	default:
		dev_err(hba->dev, "%s: Expected query attr opcode but got = 0x%.2x\n",
				__func__, opcode);
		err = -EINVAL;
		goto out_unlock;
	}

	err = ufshcd_exec_dev_cmd(hba, DEV_CMD_TYPE_QUERY, QUERY_REQ_TIMEOUT);

	if (err) {
		dev_err(hba->dev, "%s: opcode 0x%.2x for idn %d failed, index %d, err = %d\n",
				__func__, opcode, idn, index, err);
		goto out_unlock;
	}

	*attr_val = be32_to_cpu(response->upiu_res.value);

out_unlock:
	mutex_unlock(&hba->dev_cmd.lock);
out:
	return err;
}

/**
 * ufshcd_query_attr_retry() - API function for sending query
 * attribute with retries
 * @hba: per-adapter instance
 * @opcode: attribute opcode
 * @idn: attribute idn to access
 * @index: index field
 * @selector: selector field
 * @attr_val: the attribute value after the query request
 * completes
 *
 * Returns 0 for success, non-zero in case of failure
*/
int ufshcd_query_attr_retry(struct ufs_hba *hba, enum query_opcode opcode,
			    enum attr_idn idn, u8 index, u8 selector,
			    u32 *attr_val)
{
	int ret = 0;
	u32 retries;

	 for (retries = QUERY_REQ_RETRIES; retries > 0; retries--) {
		ret = ufshcd_query_attr(hba, opcode, idn, index,
						selector, attr_val);
		if (ret)
			dev_dbg(hba->dev, "%s: failed with error %d, retries %d\n",
				__func__, ret, retries);
		else
			break;
	}

	if (ret)
		dev_err(hba->dev,
			"%s: query attribute, idn %d, failed with error %d after %d retires\n",
			__func__, idn, ret, QUERY_REQ_RETRIES);
	return ret;
}

static int __ufshcd_query_descriptor(struct ufs_hba *hba,
			enum query_opcode opcode, enum desc_idn idn, u8 index,
			u8 selector, u8 *desc_buf, int *buf_len)
{
	struct ufs_query_req *request = NULL;
	struct ufs_query_res *response = NULL;
	int err;

	BUG_ON(!hba);

	if (!desc_buf) {
		dev_err(hba->dev, "%s: descriptor buffer required for opcode 0x%x\n",
				__func__, opcode);
		err = -EINVAL;
		goto out;
	}

	if (*buf_len < QUERY_DESC_MIN_SIZE || *buf_len > QUERY_DESC_MAX_SIZE) {
		dev_err(hba->dev, "%s: descriptor buffer size (%d) is out of range\n",
				__func__, *buf_len);
		err = -EINVAL;
		goto out;
	}

	mutex_lock(&hba->dev_cmd.lock);
	ufshcd_init_query(hba, &request, &response, opcode, idn, index,
			selector);
	hba->dev_cmd.query.descriptor = desc_buf;
	request->upiu_req.length = cpu_to_be16(*buf_len);

	switch (opcode) {
	case UPIU_QUERY_OPCODE_WRITE_DESC:
		request->query_func = UPIU_QUERY_FUNC_STANDARD_WRITE_REQUEST;
		break;
	case UPIU_QUERY_OPCODE_READ_DESC:
		request->query_func = UPIU_QUERY_FUNC_STANDARD_READ_REQUEST;
		break;
	default:
		dev_err(hba->dev,
				"%s: Expected query descriptor opcode but got = 0x%.2x\n",
				__func__, opcode);
		err = -EINVAL;
		goto out_unlock;
	}

	err = ufshcd_exec_dev_cmd(hba, DEV_CMD_TYPE_QUERY, QUERY_REQ_TIMEOUT);

	if (err) {
		dev_err(hba->dev, "%s: opcode 0x%.2x for idn %d failed, index %d, err = %d\n",
				__func__, opcode, idn, index, err);
		goto out_unlock;
	}

	*buf_len = be16_to_cpu(response->upiu_res.length);

out_unlock:
	hba->dev_cmd.query.descriptor = NULL;
	mutex_unlock(&hba->dev_cmd.lock);
out:
	return err;
}

/**
 * ufshcd_query_descriptor_retry - API function for sending descriptor
 * requests
 * hba: per-adapter instance
 * opcode: attribute opcode
 * idn: attribute idn to access
 * index: index field
 * selector: selector field
 * desc_buf: the buffer that contains the descriptor
 * buf_len: length parameter passed to the device
 *
 * Returns 0 for success, non-zero in case of failure.
 * The buf_len parameter will contain, on return, the length parameter
 * received on the response.
 */
int ufshcd_query_descriptor_retry(struct ufs_hba *hba,
					 enum query_opcode opcode,
					 enum desc_idn idn, u8 index,
					 u8 selector,
					 u8 *desc_buf, int *buf_len)
{
	int err;
	int retries;

	for (retries = QUERY_REQ_RETRIES; retries > 0; retries--) {
		err = __ufshcd_query_descriptor(hba, opcode, idn, index,
						selector, desc_buf, buf_len);
		if (!err || err == -EINVAL)
			break;
	}

	return err;
}
EXPORT_SYMBOL(ufshcd_query_descriptor_retry);

/**
 * ufshcd_read_desc_length - read the specified descriptor length from header
 * @hba: Pointer to adapter instance
 * @desc_id: descriptor idn value
 * @desc_index: descriptor index
 * @desc_length: pointer to variable to read the length of descriptor
 *
 * Return 0 in case of success, non-zero otherwise
 */
static int ufshcd_read_desc_length(struct ufs_hba *hba,
	enum desc_idn desc_id,
	int desc_index,
	int *desc_length)
{
	int ret;
	u8 header[QUERY_DESC_HDR_SIZE];
	int header_len = QUERY_DESC_HDR_SIZE;

	if (desc_id >= QUERY_DESC_IDN_MAX)
		return -EINVAL;

	ret = ufshcd_query_descriptor_retry(hba, UPIU_QUERY_OPCODE_READ_DESC,
					desc_id, desc_index, 0, header,
					&header_len);

	if (ret) {
		dev_err(hba->dev, "%s: Failed to get descriptor header id %d",
			__func__, desc_id);
		return ret;
	} else if (desc_id != header[QUERY_DESC_DESC_TYPE_OFFSET]) {
		dev_warn(hba->dev, "%s: descriptor header id %d and desc_id %d mismatch",
			__func__, header[QUERY_DESC_DESC_TYPE_OFFSET],
			desc_id);
		ret = -EINVAL;
	}

	*desc_length = header[QUERY_DESC_LENGTH_OFFSET];
	return ret;

}

/**
 * ufshcd_map_desc_id_to_length - map descriptor IDN to its length
 * @hba: Pointer to adapter instance
 * @desc_id: descriptor idn value
 * @desc_len: mapped desc length (out)
 *
 * Return 0 in case of success, non-zero otherwise
 */
int ufshcd_map_desc_id_to_length(struct ufs_hba *hba,
	enum desc_idn desc_id, int *desc_len)
{
	switch (desc_id) {
	case QUERY_DESC_IDN_DEVICE:
		*desc_len = hba->desc_size.dev_desc;
		break;
	case QUERY_DESC_IDN_POWER:
		*desc_len = hba->desc_size.pwr_desc;
		break;
	case QUERY_DESC_IDN_GEOMETRY:
		*desc_len = hba->desc_size.geom_desc;
		break;
	case QUERY_DESC_IDN_CONFIGURATION:
		*desc_len = hba->desc_size.conf_desc;
		break;
	case QUERY_DESC_IDN_UNIT:
		*desc_len = hba->desc_size.unit_desc;
		break;
	case QUERY_DESC_IDN_INTERCONNECT:
		*desc_len = hba->desc_size.interc_desc;
		break;
	case QUERY_DESC_IDN_STRING:
		*desc_len = QUERY_DESC_MAX_SIZE;
		break;
	case QUERY_DESC_IDN_RFU_0:
	case QUERY_DESC_IDN_RFU_1:
		*desc_len = 0;
		break;
	default:
		*desc_len = 0;
		return -EINVAL;
	}
	return 0;
}
EXPORT_SYMBOL(ufshcd_map_desc_id_to_length);

/**
 * ufshcd_read_desc_param - read the specified descriptor parameter
 * @hba: Pointer to adapter instance
 * @desc_id: descriptor idn value
 * @desc_index: descriptor index
 * @param_offset: offset of the parameter to read
 * @param_read_buf: pointer to buffer where parameter would be read
 * @param_size: sizeof(param_read_buf)
 *
 * Return 0 in case of success, non-zero otherwise
 */
static int ufshcd_read_desc_param(struct ufs_hba *hba,
				  enum desc_idn desc_id,
				  int desc_index,
				  u32 param_offset,
				  u8 *param_read_buf,
				  u32 param_size)
{
	int ret;
	u8 *desc_buf = NULL;
	u32 buff_len;
	bool is_kmalloc = true;

	/* safety checks */
	if (desc_id >= QUERY_DESC_IDN_MAX)
		return -EINVAL;

	buff_len = ufs_query_desc_max_size[desc_id];
	if ((param_offset + param_size) > buff_len)
		return -EINVAL;

	if (!param_offset && (param_size == buff_len)) {
		/* memory space already available to hold full descriptor */
		desc_buf = param_read_buf;
		is_kmalloc = false;
	} else {
		/* allocate memory to hold full descriptor */
		desc_buf = kzalloc(buff_len, GFP_KERNEL);
		if (!desc_buf)
			return -ENOMEM;
	}

	/* Request for full descriptor */
	ret = ufshcd_query_descriptor_retry(hba, UPIU_QUERY_OPCODE_READ_DESC,
					desc_id, desc_index, 0, desc_buf,
					&buff_len);

	if (ret || ((!hba->host->is_emulator) &&
			((buff_len > ufs_query_desc_max_size[desc_id]) ||
	    		(desc_buf[QUERY_DESC_DESC_TYPE_OFFSET] != desc_id)))) {
		dev_err(hba->dev, "%s: Failed reading descriptor. desc_id %d "
				  "param_offset %d buff_len %d ret %d",
			__func__, desc_id, param_offset, buff_len, ret);
		if (!ret)
			ret = -EINVAL;

		goto out;
	}

	if (is_kmalloc)
		memcpy(param_read_buf, &desc_buf[param_offset], param_size);
out:
	if (is_kmalloc)
		kfree(desc_buf);
	return ret;
}

static inline int ufshcd_read_desc(struct ufs_hba *hba,
				   enum desc_idn desc_id,
				   int desc_index,
				   u8 *buf,
				   u32 size)
{
	return ufshcd_read_desc_param(hba, desc_id, desc_index, 0, buf, size);
}

static inline int ufshcd_read_power_desc(struct ufs_hba *hba,
					 u8 *buf,
					 u32 size)
{
	return ufshcd_read_desc(hba, QUERY_DESC_IDN_POWER, 0, buf, size);
}

int ufshcd_read_device_desc(struct ufs_hba *hba, u8 *buf, u32 size)
{
	return ufshcd_read_desc(hba, QUERY_DESC_IDN_DEVICE, 0, buf, size);
}
EXPORT_SYMBOL(ufshcd_read_device_desc);


/**
 * ufshcd_read_string_desc - read string descriptor
 * @hba: pointer to adapter instance
 * @desc_index: descriptor index
 * @buf: pointer to buffer where descriptor would be read
 * @size: size of buf
 * @ascii: if true convert from unicode to ascii characters
 *
 * Return 0 in case of success, non-zero otherwise
 */
#define ASCII_STD true
int ufshcd_read_string_desc(struct ufs_hba *hba, int desc_index,
				   u8 *buf, u32 size, bool ascii)
{
	int err = 0;

	err = ufshcd_read_desc(hba,
				QUERY_DESC_IDN_STRING, desc_index, buf, size);

	if (err) {
		dev_err(hba->dev, "%s: reading String Desc failed after %d retries. err = %d\n",
			__func__, QUERY_REQ_RETRIES, err);
		goto out;
	}

	if (ascii) {
		int desc_len;
		unsigned int ascii_len;
		unsigned int i;
		char *buff_ascii = NULL;

		desc_len = buf[0];
		/* remove header and divide by 2 to move from UTF16 to UTF8 */
		ascii_len = (desc_len - QUERY_DESC_HDR_SIZE) / 2 + 1;
		if (size < ascii_len + QUERY_DESC_HDR_SIZE) {
			dev_err(hba->dev,
				"%s: buffer allocated size is too small\n",
				__func__);
			err = -ENOMEM;
			goto out;
		}

		buff_ascii = kzalloc(ascii_len, GFP_KERNEL);
		if (!buff_ascii) {
			dev_err(hba->dev, "%s: Failed allocating %d bytes\n",
				__func__, ascii_len);
			err = -ENOMEM;
			goto out;
		}

		/*
		 * the descriptor contains string in UTF16 format
		 * we need to convert to utf-8 so it can be displayed
		 */
		utf16s_to_utf8s((wchar_t *)&buf[QUERY_DESC_HDR_SIZE],
				desc_len - QUERY_DESC_HDR_SIZE,
				UTF16_BIG_ENDIAN, buff_ascii, ascii_len);

		/* replace non-printable or non-ASCII characters with spaces */
		for (i = 0; i < ascii_len; i++)
			ufshcd_remove_non_printable(&buff_ascii[i]);

		memset(buf + QUERY_DESC_HDR_SIZE, 0,
		       size - QUERY_DESC_HDR_SIZE);
		memcpy(buf + QUERY_DESC_HDR_SIZE, buff_ascii, ascii_len);
		buf[QUERY_DESC_LENGTH_OFFSET] = ascii_len + QUERY_DESC_HDR_SIZE;
		kfree(buff_ascii);
	}
out:
	return err;
}

/**
 * ufshcd_read_unit_desc_param - read the specified unit descriptor parameter
 * @hba: Pointer to adapter instance
 * @lun: lun id
 * @param_offset: offset of the parameter to read
 * @param_read_buf: pointer to buffer where parameter would be read
 * @param_size: sizeof(param_read_buf)
 *
 * Return 0 in case of success, non-zero otherwise
 */
int ufshcd_read_unit_desc_param(struct ufs_hba *hba,
					      int lun,
					      enum unit_desc_param param_offset,
					      u8 *param_read_buf,
					      u32 param_size)
{
	/*
	 * Unit descriptors are only available for general purpose LUs (LUN id
	 * from 0 to 7) and RPMB Well known LU.
	 */
	if (!ufs_is_valid_unit_desc_lun(lun))
		return -EOPNOTSUPP;

	return ufshcd_read_desc_param(hba, QUERY_DESC_IDN_UNIT, lun,
				      param_offset, param_read_buf, param_size);
}

static int ufshcd_hisi_ufs_hc_utrdl_alloc(struct ufs_hba *hba)
{
	size_t utrdl_size;

	utrdl_size = (sizeof(struct hisi_utp_transfer_req_desc) *
		      (unsigned int)hba->nutrs);
	hba->hisi_utrdl_base_addr = dmam_alloc_coherent(
		hba->dev, utrdl_size, &hba->utrdl_dma_addr, GFP_KERNEL);
	if (!hba->hisi_utrdl_base_addr ||
		WARN_ON((hba->utrdl_dma_addr & (PAGE_SIZE - 1)))) { /*lint !e146 !e665*/
		dev_err(hba->dev, "transfer descriptor memory allocation failed\n");
		return -ENOMEM;
	}

	return 0;
}
/**
 * ufshcd_memory_alloc - allocate memory for host memory space data structures
 * @hba: per adapter instance
 *
 * 1. Allocate DMA memory for Command Descriptor array
 *	Each command descriptor consist of Command UPIU, Response UPIU and PRDT
 * 2. Allocate DMA memory for UTP Transfer Request Descriptor List (UTRDL).
 * 3. Allocate DMA memory for UTP Task Management Request Descriptor List
 *	(UTMRDL)
 * 4. Allocate memory for local reference block(lrb).
 *
 * Returns 0 for success, non-zero in case of failure
 */
static int ufshcd_memory_alloc(struct ufs_hba *hba)
{
	size_t utmrdl_size, utrdl_size, ucdl_size;
	unsigned int ucd_num;

#ifdef CONFIG_HISI_UFS_HC_CORE_UTR
	ucd_num = CORE_SLOT_NUM;
	hba->nutrs = SPEC_SLOT_NUM + CORE_SLOT_NUM;
#else
	ucd_num = hba->nutrs;
#endif
	/* Allocate memory for UTP command descriptors */
	ucdl_size = sizeof(struct utp_transfer_cmd_desc) * ucd_num;
	hba->ucdl_base_addr = dmam_alloc_coherent(
		hba->dev, ucdl_size, &hba->ucdl_dma_addr, GFP_KERNEL);

	/*
	 * UFSHCI requires UTP command descriptor to be 128 byte aligned.
	 * make sure hba->ucdl_dma_addr is aligned to PAGE_SIZE
	 * if hba->ucdl_dma_addr is aligned to PAGE_SIZE, then it will
	 * be aligned to 128 bytes as well
	 */
	if (!hba->ucdl_base_addr ||
	    WARN_ON(hba->ucdl_dma_addr & (PAGE_SIZE - 1))) { /*lint !e146 !e665*/
		dev_err(hba->dev,
			"Command Descriptor Memory allocation failed\n");
		goto out;
	}

	/*
	 * Allocate memory for UTP Transfer descriptors
	 * UFSHCI requires 1024 byte alignment of UTRD
	 */
	if (ufshcd_is_hisi_ufs_hc(hba)) {
		if (ufshcd_hisi_ufs_hc_utrdl_alloc(hba))
			goto out;
	} else {
		utrdl_size =
			(sizeof(struct utp_transfer_req_desc) * hba->nutrs);
		hba->utrdl_base_addr = dmam_alloc_coherent(
			hba->dev, utrdl_size, &hba->utrdl_dma_addr, GFP_KERNEL);
		if (!hba->utrdl_base_addr ||/*lint !e548*/
			WARN_ON((hba->utrdl_dma_addr & (PAGE_SIZE - 1)))) { /*lint !e146 !e665 !e548*/
			dev_err(hba->dev, "Transfer Descriptor Memory allocation failed\n");
			goto out;
		}
	}
	/*
	 * Allocate memory for UTP Task Management descriptors
	 * UFSHCI requires 1024 byte alignment of UTMRD
	 */
	utmrdl_size = sizeof(struct utp_task_req_desc) * hba->nutmrs;
	hba->utmrdl_base_addr = dmam_alloc_coherent(hba->dev, utmrdl_size, &hba->utmrdl_dma_addr, GFP_KERNEL);
	if (!hba->utmrdl_base_addr ||/*lint !e548*/
	    WARN_ON(hba->utmrdl_dma_addr & (PAGE_SIZE - 1))) { /*lint !e146 !e665 !e548*/
		dev_err(hba->dev,
		"Task Management Descriptor Memory allocation failed\n");
		goto out;
	}

	/* Allocate memory for local reference block */
	hba->lrb = devm_kzalloc(hba->dev,
				hba->nutrs * sizeof(struct ufshcd_lrb), GFP_KERNEL);
	if (!hba->lrb) {/*lint !e548*/
		dev_err(hba->dev, "LRB Memory allocation failed\n");
		goto out;
	}

	return 0;
out:
	return -ENOMEM;
}

static void ufshcd_host_memory_configure(struct ufs_hba *hba)
{
	struct utp_transfer_cmd_desc *cmd_descp = NULL;
	struct utp_transfer_req_desc *utrdlp = NULL;
	dma_addr_t cmd_desc_dma_addr;
	dma_addr_t cmd_desc_element_addr;
	u16 response_offset;
	u16 prdt_offset;
	int cmd_desc_size;
	int i;

	if (ufshcd_is_hisi_ufs_hc(hba) && hba->vops &&
		hba->vops->ufshcd_hisi_host_memory_configure) {
		hba->vops->ufshcd_hisi_host_memory_configure(hba);
		return;
	}

	utrdlp = hba->utrdl_base_addr;
	cmd_descp = hba->ucdl_base_addr;

	response_offset =
		offsetof(struct utp_transfer_cmd_desc, response_upiu);
	prdt_offset =
		offsetof(struct utp_transfer_cmd_desc, prd_table);

	cmd_desc_size = sizeof(struct utp_transfer_cmd_desc);
	cmd_desc_dma_addr = hba->ucdl_dma_addr;

	for (i = 0; i < hba->nutrs; i++) {
		/* Configure UTRD with command descriptor base address */
		cmd_desc_element_addr = (cmd_desc_dma_addr +
					 (cmd_desc_size * i)); /*lint !e647*/
		utrdlp[i].command_desc_base_addr_lo =
			cpu_to_le32(lower_32_bits(cmd_desc_element_addr));
		utrdlp[i].command_desc_base_addr_hi =
			cpu_to_le32(upper_32_bits(cmd_desc_element_addr));

		/* Response upiu and prdt offset should be in double words */
		if (hba->quirks & UFSHCD_QUIRK_PRDT_BYTE_GRAN) {
			utrdlp[i].response_upiu_offset =
				cpu_to_le16(response_offset);
			utrdlp[i].prd_table_offset =
				cpu_to_le16(prdt_offset);
			utrdlp[i].response_upiu_length =
				cpu_to_le16(ALIGNED_UPIU_SIZE);
		} else {
			utrdlp[i].response_upiu_offset =
				cpu_to_le16((response_offset >> 2));
			utrdlp[i].prd_table_offset =
				cpu_to_le16((prdt_offset >> 2));
			utrdlp[i].response_upiu_length =
				cpu_to_le16(ALIGNED_UPIU_SIZE >> 2);
		}

		hba->lrb[i].utr_descriptor_ptr = (utrdlp + i);
		hba->lrb[i].utrd_dma_addr = hba->utrdl_dma_addr +
				(i * sizeof(struct utp_transfer_req_desc));
		hba->lrb[i].ucd_req_ptr =
			(struct utp_upiu_req *)(cmd_descp + i);
		hba->lrb[i].ucd_req_dma_addr = cmd_desc_element_addr;
		hba->lrb[i].ucd_rsp_ptr =
			(struct utp_upiu_rsp *)cmd_descp[i].response_upiu;
		hba->lrb[i].ucd_rsp_dma_addr = cmd_desc_element_addr +
				response_offset;
		hba->lrb[i].ucd_prdt_ptr =
			(struct ufshcd_sg_entry *)cmd_descp[i].prd_table;
		hba->lrb[i].ucd_prdt_dma_addr = cmd_desc_element_addr +
				prdt_offset;
	}
}

/**
 * ufshcd_dme_link_startup - Notify Unipro to perform link startup
 * @hba: per adapter instance
 *
 * UIC_CMD_DME_LINK_STARTUP command must be issued to Unipro layer,
 * in order to initialize the Unipro link startup procedure.
 * Once the Unipro links are up, the device connected to the controller
 * is detected.
 *
 * Returns 0 on success, non-zero value on failure
 */
static int ufshcd_dme_link_startup(struct ufs_hba *hba)
{
	struct uic_command uic_cmd = {0};
	int ret;

	if (ufshcd_is_hisi_ufs_hc(hba) && hba->vops &&
		hba->vops->hisi_dme_link_startup)
		return hba->vops->hisi_dme_link_startup(hba);

	uic_cmd.command = UIC_CMD_DME_LINK_STARTUP;

	ret = ufshcd_send_uic_cmd(hba, &uic_cmd);
	if (ret)
		dev_dbg(hba->dev,
			"dme-link-startup: error code %d\n", ret);
	return ret;
}

static inline void ufshcd_add_delay_before_dme_cmd(struct ufs_hba *hba)
{
	#define MIN_DELAY_BEFORE_DME_CMDS_US	1000
	unsigned long min_sleep_time_us;

	if (!(hba->quirks & UFSHCD_QUIRK_DELAY_BEFORE_DME_CMDS))
		return;

	/*
	 * last_dme_cmd_tstamp will be 0 only for 1st call to
	 * this function
	 */
	if (unlikely(!ktime_to_us(hba->last_dme_cmd_tstamp))) {
		min_sleep_time_us = MIN_DELAY_BEFORE_DME_CMDS_US;
	} else {
		unsigned long delta =
			(unsigned long) ktime_to_us(
				ktime_sub(ktime_get(),
				hba->last_dme_cmd_tstamp));

		if (delta < MIN_DELAY_BEFORE_DME_CMDS_US)
			min_sleep_time_us =
				MIN_DELAY_BEFORE_DME_CMDS_US - delta;
		else
			return; /* no more delay required */
	}

	/* allow sleep for extra 50us if needed */
	usleep_range(min_sleep_time_us, min_sleep_time_us + 50);
}

/**
 * ufshcd_dme_set_attr - UIC command for DME_SET, DME_PEER_SET
 * @hba: per adapter instance
 * @attr_sel: uic command argument1
 * @attr_set: attribute set type as uic command argument2
 * @mib_val: setting value as uic command argument3
 * @peer: indicate whether peer or local
 *
 * Returns 0 on success, non-zero value on failure
 */
int ufshcd_dme_set_attr(struct ufs_hba *hba, u32 attr_sel,
			u8 attr_set, u32 mib_val, u8 peer)
{
	struct uic_command uic_cmd = {0};
	static const char *const action[] = {
		"dme-set",
		"dme-peer-set"
	};
	const char *set = action[!!peer]; /*lint !e514*/
	int ret;
	int retries = UFS_UIC_COMMAND_RETRIES;

	uic_cmd.command = peer ?
		UIC_CMD_DME_PEER_SET : UIC_CMD_DME_SET;
	uic_cmd.argument1 = attr_sel;
	uic_cmd.argument2 = UIC_ARG_ATTR_TYPE(attr_set);
	uic_cmd.argument3 = mib_val;

	do {
		/* for peer attributes we retry upon failure */
		ret = ufshcd_send_uic_cmd(hba, &uic_cmd);
		if (ret)
			dev_dbg(hba->dev, "%s: attr-id 0x%x val 0x%x error code %d\n",
				set, UIC_GET_ATTR_ID(attr_sel), mib_val, ret);
	} while (ret && peer && --retries);
	if (ret)
		dev_err(hba->dev, "%s: attr-id 0x%x val 0x%x failed %d retries\n",
			set, UIC_GET_ATTR_ID(attr_sel), mib_val,
			UFS_UIC_COMMAND_RETRIES - retries);

	return ret;
}
EXPORT_SYMBOL_GPL(ufshcd_dme_set_attr);

/**
 * ufshcd_dme_get_attr - UIC command for DME_GET, DME_PEER_GET
 * @hba: per adapter instance
 * @attr_sel: uic command argument1
 * @mib_val: the value of the attribute as returned by the UIC command
 * @peer: indicate whether peer or local
 *
 * Returns 0 on success, non-zero value on failure
 */
int ufshcd_dme_get_attr(struct ufs_hba *hba, u32 attr_sel,
			u32 *mib_val, u8 peer)
{
	struct uic_command uic_cmd = {0};
	static const char *const action[] = {
		"dme-get",
		"dme-peer-get"
	};
	const char *get = action[!!peer]; /*lint !e514*/
	int ret;
	int retries = UFS_UIC_COMMAND_RETRIES;
	struct ufs_pa_layer_attr orig_pwr_info;
	struct ufs_pa_layer_attr temp_pwr_info;
	bool pwr_mode_change = false;

	if (peer && (hba->quirks & UFSHCD_QUIRK_DME_PEER_ACCESS_AUTO_MODE)) {
		orig_pwr_info = hba->pwr_info;
		temp_pwr_info = orig_pwr_info;

		if (orig_pwr_info.pwr_tx == FAST_MODE ||
		    orig_pwr_info.pwr_rx == FAST_MODE) {
			temp_pwr_info.pwr_tx = FASTAUTO_MODE;
			temp_pwr_info.pwr_rx = FASTAUTO_MODE;
			pwr_mode_change = true;
		} else if (orig_pwr_info.pwr_tx == SLOW_MODE ||
		    orig_pwr_info.pwr_rx == SLOW_MODE) {
			temp_pwr_info.pwr_tx = SLOWAUTO_MODE;
			temp_pwr_info.pwr_rx = SLOWAUTO_MODE;
			pwr_mode_change = true;
		}
		if (pwr_mode_change) {
			ret = ufshcd_change_power_mode(hba, &temp_pwr_info);
			if (ret)
				goto out;
		}
	}

	uic_cmd.command = peer ?
		UIC_CMD_DME_PEER_GET : UIC_CMD_DME_GET;
	uic_cmd.argument1 = attr_sel;

	do {
		/* for peer attributes we retry upon failure */
		ret = ufshcd_send_uic_cmd(hba, &uic_cmd);
		if (ret)
			dev_dbg(hba->dev, "%s: attr-id 0x%x error code %d\n",
				get, UIC_GET_ATTR_ID(attr_sel), ret);
	} while (ret && peer && --retries);

	if (ret)
		dev_err(hba->dev, "%s: attr-id 0x%x failed %d retries\n",
			get, UIC_GET_ATTR_ID(attr_sel),
			UFS_UIC_COMMAND_RETRIES - retries);

	if (mib_val && !ret)
		*mib_val = uic_cmd.argument3;

	if (peer && (hba->quirks & UFSHCD_QUIRK_DME_PEER_ACCESS_AUTO_MODE)
	    && pwr_mode_change) {
		ret = ufshcd_change_power_mode(hba, &orig_pwr_info);
		dev_err(hba->dev, "%s: ufshcd_change_power_mode return error %d \n", __func__, ret);
	}
out:
	return ret;
}
EXPORT_SYMBOL_GPL(ufshcd_dme_get_attr);

/**
 * ufshcd_uic_pwr_ctrl - executes UIC commands (which affects the link power
 * state) and waits for it to take effect.
 *
 * @hba: per adapter instance
 * @cmd: UIC command to execute
 *
 * DME operations like DME_SET(PA_PWRMODE), DME_HIBERNATE_ENTER &
 * DME_HIBERNATE_EXIT commands take some time to take its effect on both host
 * and device UniPro link and hence it's final completion would be indicated by
 * dedicated status bits in Interrupt Status register (UPMS, UHES, UHXS) in
 * addition to normal UIC command completion Status (UCCS). This function only
 * returns after the relevant status bits indicate the completion.
 *
 * Returns 0 on success, non-zero value on failure
 */
static int ufshcd_uic_pwr_ctrl(struct ufs_hba *hba, struct uic_command *cmd)
{
	struct completion uic_async_done;
	unsigned long flags;
	u8 status;
	int ret;


	mutex_lock(&hba->uic_cmd_mutex);
	init_completion(&uic_async_done);
	ufshcd_add_delay_before_dme_cmd(hba);

	spin_lock_irqsave(hba->host->host_lock, flags);
	if (((!hba->is_hibernate) && (cmd->command == UIC_CMD_DME_HIBER_EXIT)) ||
	    ((hba->is_hibernate) && (cmd->command == UIC_CMD_DME_HIBER_ENTER))) {
		dev_info(hba->dev, "more than one h8_uic: 0x%x\n", cmd->command);
		ret = 0;
		spin_unlock_irqrestore(hba->host->host_lock, flags);
		goto out;
	}
	if (cmd->command == UIC_CMD_DME_HIBER_ENTER) {
		ret = __ufshcd_wait_for_doorbell_clr(hba);
		if (ret) {
			dev_err(hba->dev, "wait doorbell clear timeout send H8 enter cmd\n");
			spin_unlock_irqrestore(hba->host->host_lock, flags);
			goto out;
		}
	}
	hba->uic_async_done = &uic_async_done;
	ret = __ufshcd_send_uic_cmd(hba, cmd,true);
	if ((!ret) && (cmd->command == UIC_CMD_DME_HIBER_ENTER))
		hba->is_hibernate = true;
	spin_unlock_irqrestore(hba->host->host_lock, flags);
	if (ret) {
		dev_err(hba->dev,
			"pwr ctrl cmd 0x%x with mode 0x%x uic error %d\n",
			cmd->command, cmd->argument3, ret);
		spin_lock_irqsave(hba->host->host_lock, flags);
		dsm_ufs_update_error_info(hba, DSM_UFS_UIC_CMD_ERR);
		spin_unlock_irqrestore(hba->host->host_lock, flags);
		schedule_ufs_dsm_work(hba);
		goto out;
	}
	ret = ufshcd_wait_for_uic_cmd(hba, cmd);
	if (ret) {
		dev_err(hba->dev,
			"pwr ctrl cmd 0x%x with mode 0x%x uic error %d\n",
			cmd->command, cmd->argument3, ret);
		spin_lock_irqsave(hba->host->host_lock, flags);
		dsm_ufs_update_error_info(hba, DSM_UFS_UIC_CMD_ERR);
		spin_unlock_irqrestore(hba->host->host_lock, flags);
		schedule_ufs_dsm_work(hba);
		goto out;
	}

	if (!wait_for_completion_timeout(hba->uic_async_done,
					 msecs_to_jiffies(UIC_CMD_TIMEOUT))) {
		dev_err(hba->dev,
			"pwr ctrl cmd 0x%x with mode 0x%x completion timeout\n",
			cmd->command, cmd->argument3);
		ret = -ETIMEDOUT;
		goto out;
	}

	spin_lock_irqsave(hba->host->host_lock, flags);
	status = ufshcd_get_upmcrs(hba);
	spin_unlock_irqrestore(hba->host->host_lock, flags);
	if (status != PWR_LOCAL) {
		dev_err(hba->dev,
			"pwr ctrl cmd 0x%x failed, host umpcrs:0x%x\n",
			cmd->command, status);
		ret = (status != PWR_OK) ? status : -1;
	}
out:
	if (ret) {
		ufshcd_print_host_state(hba);
		ufshcd_print_pwr_info(hba);
		ufshcd_print_host_regs(hba);
	}

	spin_lock_irqsave(hba->host->host_lock, flags);
	hba->active_uic_cmd = NULL;
	hba->uic_async_done = NULL;
	if ((!ret) && (cmd->command == UIC_CMD_DME_HIBER_EXIT))
		hba->is_hibernate = false;

	spin_unlock_irqrestore(hba->host->host_lock, flags);
	mutex_unlock(&hba->uic_cmd_mutex);

	return ret;
}

/**
 * ufshcd_uic_change_pwr_mode - Perform the UIC power mode chage
 *				using DME_SET primitives.
 * @hba: per adapter instance
 * @mode: powr mode value
 *
 * Returns 0 on success, non-zero value on failure
 */
static int ufshcd_uic_change_pwr_mode(struct ufs_hba *hba, u8 mode)
{
	struct uic_command uic_cmd = {0};
	int ret;

	if (ufshcd_is_hisi_ufs_hc(hba) && hba->vops &&
		hba->vops->ufshcd_hisi_uic_change_pwr_mode)
		return hba->vops->ufshcd_hisi_uic_change_pwr_mode(hba, mode);

	if (hba->quirks & UFSHCD_QUIRK_BROKEN_PA_RXHSUNTERMCAP) {
		ret = ufshcd_dme_set(hba,
				UIC_ARG_MIB_SEL(PA_RXHSUNTERMCAP, 0), 1);
		if (ret) {
			dev_err(hba->dev, "%s: failed to enable PA_RXHSUNTERMCAP ret %d\n",
						__func__, ret);
			goto out;
		}
	}

	uic_cmd.command = UIC_CMD_DME_SET;
	uic_cmd.argument1 = UIC_ARG_MIB(PA_PWRMODE);
	uic_cmd.argument3 = mode;
	ret = ufshcd_uic_pwr_ctrl(hba, &uic_cmd);

out:
	return ret;
}

static int ufshcd_uic_hibern8_enter(struct ufs_hba *hba)
{
	int ret;
	unsigned long flags;
	spin_lock_irqsave(hba->host->host_lock, flags);
	if (hba->is_hibernate) {
		dev_err(hba->dev, "warring!!!! enter H8 twice\n");
		ret = 0;
		goto out;
	}
	ret = __ufshcd_wait_for_doorbell_clr(hba);
	if (ret) {
		dev_err(hba->dev, "wait doorbell clear timeout before enter H8\n");
		goto out;
	}
	ret = __ufshcd_uic_hibern8_op_irq_safe(hba, UFS_KIRIN_H8_OP_ENTER);
	if (!ret)
		hba->is_hibernate = true;
	else
		dev_err(hba->dev, "enter H8 fail\n");
out:
	if (ret) {
		/* block commands from scsi mid-layer */
		scsi_block_requests(hba->host);

		ufshcd_print_host_regs(hba);
		ufshcd_print_host_state(hba);
		ufshcd_print_pwr_info(hba);

#ifdef CONFIG_SCSI_UFS_HS_ERROR_RECOVER
		hba->hs_single_lane = 0;
		hba->use_pwm_mode = 0;
#endif

		hba->ufshcd_state = UFSHCD_STATE_EH_SCHEDULED;
		hba->force_host_reset = 1;
		if (!kthread_queue_work(&hba->eh_worker, &hba->eh_work))
			dev_err(hba->dev, "%s: queue hba->eh_worker failed !\n", __func__);
	}
	spin_unlock_irqrestore(hba->host->host_lock, flags);
	return ret;
}

static int ufshcd_uic_hibern8_exit(struct ufs_hba *hba)
{
	int ret;
	unsigned long flags;
	spin_lock_irqsave(hba->host->host_lock, flags);
	if (!hba->is_hibernate) {
		dev_err(hba->dev, "warring!!!! exit H8 twice\n");
		ret = 0;
		goto out;
	}
	ret = __ufshcd_uic_hibern8_op_irq_safe(hba, UFS_KIRIN_H8_OP_EXIT);
	if (!ret)
		hba->is_hibernate = false;
	else
		dev_err(hba->dev, "exit H8 fail\n");
out:
	if (ret) {
		/* block commands from scsi mid-layer */
		scsi_block_requests(hba->host);
#ifdef CONFIG_SCSI_UFS_HS_ERROR_RECOVER
		hba->hs_single_lane = 0;
		hba->use_pwm_mode = 0;
#endif

		hba->ufshcd_state = UFSHCD_STATE_EH_SCHEDULED;
		hba->force_host_reset = 1;
		if (!kthread_queue_work(&hba->eh_worker, &hba->eh_work))
			dev_err(hba->dev, "%s: queue hba->eh_worker failed !\n", __func__);
	}
	spin_unlock_irqrestore(hba->host->host_lock, flags);
	return ret;
}

EXPORT_SYMBOL(ufshcd_uic_hibern8_exit);
/**
* ufshcd_init_pwr_info - setting the POR (power on reset)
* values in hba power info
* @hba: per-adapter instance
*/
void ufshcd_init_pwr_info(struct ufs_hba *hba)
{
	hba->pwr_info.gear_rx = UFS_PWM_G1;
	hba->pwr_info.gear_tx = UFS_PWM_G1;
	hba->pwr_info.lane_rx = 1;
	hba->pwr_info.lane_tx = 1;
	hba->pwr_info.pwr_rx = SLOWAUTO_MODE;
	hba->pwr_info.pwr_tx = SLOWAUTO_MODE;
	hba->pwr_info.hs_rate = 0;
}

/**
 * ufshcd_get_max_pwr_mode - reads the max power mode negotiated with device
 * @hba: per-adapter instance
 */
static int ufshcd_get_max_pwr_mode(struct ufs_hba *hba)
{
	struct ufs_pa_layer_attr *pwr_info = &hba->max_pwr_info.info;

	if (hba->max_pwr_info.is_valid)
		return 0;

	pwr_info->pwr_tx = FAST_MODE;
	pwr_info->pwr_rx = FAST_MODE;
	pwr_info->hs_rate = PA_HS_MODE_B;

	/* Get the connected lane count */
	ufshcd_dme_get(hba, UIC_ARG_MIB(PA_CONNECTEDRXDATALANES),
			&pwr_info->lane_rx);
	ufshcd_dme_get(hba, UIC_ARG_MIB(PA_CONNECTEDTXDATALANES),
			&pwr_info->lane_tx);

	if (!pwr_info->lane_rx || !pwr_info->lane_tx) {
		dev_err(hba->dev, "%s: invalid connected lanes value. rx=%d, tx=%d\n",
				__func__,
				pwr_info->lane_rx,
				pwr_info->lane_tx);
		return -EINVAL;
	}

	/*
	 * First, get the maximum gears of HS speed.
	 * If a zero value, it means there is no HSGEAR capability.
	 * Then, get the maximum gears of PWM speed.
	 */
	 /*lint -e835*/
	ufshcd_dme_get(hba, UIC_ARG_MIB(PA_MAXRXHSGEAR), &pwr_info->gear_rx);
	if (!pwr_info->gear_rx) {
		ufshcd_dme_get(hba, UIC_ARG_MIB(PA_MAXRXPWMGEAR),
				&pwr_info->gear_rx);
		if (!pwr_info->gear_rx) {
			dev_err(hba->dev, "%s: invalid max pwm rx gear read = %d\n",
				__func__, pwr_info->gear_rx);
			return -EINVAL;
		}
		pwr_info->pwr_rx = SLOW_MODE;
	}
#ifdef CONFIG_SCSI_UFS_GEMINI
	pwr_info->gear_tx = 3;
#else
	ufshcd_dme_peer_get(hba, UIC_ARG_MIB(PA_MAXRXHSGEAR),
			&pwr_info->gear_tx);
	if (!pwr_info->gear_tx) {
		ufshcd_dme_peer_get(hba, UIC_ARG_MIB(PA_MAXRXPWMGEAR),
				&pwr_info->gear_tx);
		if (!pwr_info->gear_tx) {
			dev_err(hba->dev, "%s: invalid max pwm tx gear read = %d\n",
				__func__, pwr_info->gear_tx);
			return -EINVAL;
		}
		pwr_info->pwr_tx = SLOW_MODE;
	}
#endif

	hba->max_pwr_info.is_valid = true;
	return 0;
}

int ufshcd_change_power_mode(struct ufs_hba *hba,
			     struct ufs_pa_layer_attr *pwr_mode)
{
	int ret;

	/* if already configured to the requested pwr_mode */
	if (pwr_mode->gear_rx == hba->pwr_info.gear_rx &&
	    pwr_mode->gear_tx == hba->pwr_info.gear_tx &&
	    pwr_mode->lane_rx == hba->pwr_info.lane_rx &&
	    pwr_mode->lane_tx == hba->pwr_info.lane_tx &&
	    pwr_mode->pwr_rx == hba->pwr_info.pwr_rx &&
	    pwr_mode->pwr_tx == hba->pwr_info.pwr_tx &&
	    pwr_mode->hs_rate == hba->pwr_info.hs_rate) {
		dev_dbg(hba->dev, "%s: power already configured\n", __func__);
		return 0;
	}
#ifdef CONFIG_HISI_UFS_HC
	if ((pwr_mode->pwr_rx == SLOW_MODE ||
	     pwr_mode->pwr_rx == SLOWAUTO_MODE) &&
	    pwr_mode->gear_rx > UFS_PWM_G1) {
		dev_info(hba->dev, "in PWM, hisi ufs only support gear 1\n");
		pwr_mode->gear_rx = UFS_PWM_G1;
	}
	if ((pwr_mode->pwr_tx == SLOW_MODE ||
	     pwr_mode->pwr_tx == SLOWAUTO_MODE) &&
	    pwr_mode->gear_tx > UFS_PWM_G1) {
		dev_info(hba->dev, "in PWM, hisi ufs only support gear 1\n");
		pwr_mode->gear_tx = UFS_PWM_G1;
	}
#endif

	/*
	 * Configure attributes for power mode change with below.
	 * - PA_RXGEAR, PA_ACTIVERXDATALANES, PA_RXTERMINATION,
	 * - PA_TXGEAR, PA_ACTIVETXDATALANES, PA_TXTERMINATION,
	 * - PA_HSSERIES
	 */
	ufshcd_dme_set(hba, UIC_ARG_MIB(PA_TXSKIP),
		0x0); /* Disable Skip Symbol Insertion */
	ufshcd_dme_set(hba, UIC_ARG_MIB(PA_RXGEAR), pwr_mode->gear_rx);
	ufshcd_dme_set(hba, UIC_ARG_MIB(PA_ACTIVERXDATALANES),
			pwr_mode->lane_rx);
	if (pwr_mode->pwr_rx == FASTAUTO_MODE ||
			pwr_mode->pwr_rx == FAST_MODE)
		ufshcd_dme_set(hba, UIC_ARG_MIB(PA_RXTERMINATION), TRUE);
	else
		ufshcd_dme_set(hba, UIC_ARG_MIB(PA_RXTERMINATION), FALSE);

	ufshcd_dme_set(hba, UIC_ARG_MIB(PA_TXGEAR), pwr_mode->gear_tx);
	ufshcd_dme_set(hba, UIC_ARG_MIB(PA_ACTIVETXDATALANES),
			pwr_mode->lane_tx);
	if ((hba->quirks & UFSHCD_QUIRK_UNIPRO_TERMINATION) &&
			(pwr_mode->pwr_tx == FASTAUTO_MODE ||
			pwr_mode->pwr_tx == FAST_MODE))
		ufshcd_dme_set(hba, UIC_ARG_MIB(PA_TXTERMINATION), TRUE);
	else
		ufshcd_dme_set(hba, UIC_ARG_MIB(PA_TXTERMINATION), FALSE);

	if (pwr_mode->pwr_rx == FASTAUTO_MODE ||
	    pwr_mode->pwr_tx == FASTAUTO_MODE ||
	    pwr_mode->pwr_rx == FAST_MODE ||
	    pwr_mode->pwr_tx == FAST_MODE) {
		if (hba->quirks & UFSHCD_QUIRK_UNIPRO_SCRAMBLING)
			ufshcd_dme_set(hba, UIC_ARG_MIB(PA_SCRAMBLING), TRUE); /* PA_Scrambling */
		ufshcd_dme_set(hba, UIC_ARG_MIB(PA_HSSERIES),
						pwr_mode->hs_rate);
	} else {
		ufshcd_dme_set(hba, UIC_ARG_MIB(PA_SCRAMBLING), FALSE); /* PA_Scrambling */
	}

	ufshcd_config_adapt(hba, pwr_mode);

	ret = ufshcd_uic_change_pwr_mode(
		hba, pwr_mode->pwr_rx << 4 | pwr_mode->pwr_tx);
	if (ret) {
		ufshcd_update_error_stats(hba, UFS_ERR_POWER_MODE_CHANGE);
		dev_err(hba->dev,
			"%s: power mode change failed %d\n", __func__, ret);
	} else {
		ufshcd_vops_pwr_change_notify(hba, POST_CHANGE, NULL,
								pwr_mode);

		memcpy(&hba->pwr_info, pwr_mode,
			sizeof(struct ufs_pa_layer_attr));
	}

	return ret;
}
EXPORT_SYMBOL(ufshcd_change_power_mode);
/**
 * ufshcd_config_pwr_mode - configure a new power mode
 * @hba: per-adapter instance
 * @desired_pwr_mode: desired power configuration
 */
static int ufshcd_config_pwr_mode(struct ufs_hba *hba,
		struct ufs_pa_layer_attr *desired_pwr_mode)
{
	struct ufs_pa_layer_attr final_params = { 0 };
	int ret;

	/* on Pisces Emu, pmc will timeout, skip pmc for temp*/
	if (ufshcd_is_hisi_ufs_hc(hba) &&
		unlikely(hba->host->is_emulator)) {
		dev_err(hba->dev, "skip pmc\n");
		return 0;
	}

	ret = ufshcd_vops_pwr_change_notify(hba, PRE_CHANGE,
					desired_pwr_mode, &final_params);

	if (ret)
		memcpy(&final_params, desired_pwr_mode, sizeof(final_params));

#ifdef CONFIG_HISI_UFS_HC
	ret = auto_k_enable_pmc_check(hba, final_params);
	if (ret)
		return ret;
#endif
	ret = ufshcd_change_power_mode(hba, &final_params);
	if (!ret)
		ufshcd_print_pwr_info(hba);

	return ret;
}

/**
 * ufshcd_complete_dev_init() - checks device readiness
 * hba: per-adapter instance
 *
 * Set fDeviceInit flag and poll until device toggles it.
 */
static int ufshcd_complete_dev_init(struct ufs_hba *hba)
{
	int i;
	int err;
	bool flag_res = 1;

	err = ufshcd_query_flag_retry(hba, UPIU_QUERY_OPCODE_SET_FLAG,
		QUERY_FLAG_IDN_FDEVICEINIT, NULL);
	if (err) {
		dev_err(hba->dev,
			"%s setting fDeviceInit flag failed with error %d\n",
			__func__, err);
		goto out;
	}

	/* poll for max. 100 iterations for fDeviceInit flag to clear */
	for (i = 0; i < 100 && !err && flag_res; i++) {
		err = ufshcd_query_flag_retry(hba, UPIU_QUERY_OPCODE_READ_FLAG,
			QUERY_FLAG_IDN_FDEVICEINIT, &flag_res);
                /* give device more time to complete initialization */
		if (flag_res)
			msleep(10);
	}

	if (err)
		dev_err(hba->dev,
			"%s reading fDeviceInit flag failed with error %d\n",
			__func__, err);
	else if (flag_res)
		dev_err(hba->dev,
			"%s fDeviceInit was not cleared by the device\n",
			__func__);

out:
	return err;
}

/**
 * ufshcd_make_hba_operational - Make UFS controller operational
 * @hba: per adapter instance
 *
 * To bring UFS host controller to operational state,
 * 1. Enable required interrupts
 * 2. Configure interrupt aggregation
 * 3. Program UTRL and UTMRL base address
 * 4. Configure run-stop-registers
 * Returns 0 on success, non-zero value on failure
 */
static int ufshcd_make_hba_operational(struct ufs_hba *hba)
{
#ifdef CONFIG_SCSI_UFS_INLINE_CRYPTO
	/* enable UFS inline enrypto if possible */
	if (ufshcd_hba_uie_init(hba)) {
		dev_err(hba->dev,
				"Controller uie init failed\n");
		return -EIO;
	}
#endif

	/* Enable required interrupts */
	ufshcd_enable_intr(hba, UFSHCD_ENABLE_INTRS);

#ifdef CONFIG_HISI_UFS_HC
	if (ufshcd_is_hisi_ufs_hc(hba)) {
		/* Enable required unipro and VS_IS interrupts */
		ufshcd_hisi_enable_unipro_intr(hba, DME_ENABLE_INTRS);
		ufshcd_enable_vs_intr(hba, UFS_VS_ENABLE_INTRS);
#ifdef CONFIG_HISI_UFS_HC_CORE_UTR
		ufshcd_enable_core_utr_intr(hba);
		config_hisi_tr_qos(hba);
		set_core_utr_qos_starve_timeout(hba, DEFAULT_CORE_TR_TIMEOUT);
#endif
		if (hba->caps & UFSHCD_CAP_PWM_DAEMON_INTR)
			ufshcd_hisi_enable_pwm_intr(hba);
		if (hba->caps & UFSHCD_CAP_DEV_TMT_INTR)
			ufshcd_hisi_enable_dev_tmt_intr(hba);
		/* when ufs reset, need enable idle intr again */
		if (!(hba->caps & UFSHCD_CAP_BROKEN_IDLE_INTR))
			ufshcd_enable_vs_intr(hba, IDLE_PREJUDGE_INTR);
	}
#endif

#ifdef CONFIG_SCSI_UFS_INLINE_CRYPTO
	/* enable intr CEFEE if the controller support inline encrypt */
	if (ufshcd_support_inline_encrypt(hba))
		ufshcd_enable_intr(hba, CRYPTO_ENGINE_FATAL_ERROR);
#endif

	/* Configure interrupt aggregation */
	if (ufshcd_is_intr_aggr_allowed(hba))
		ufshcd_config_intr_aggr(hba, hba->nutrs - 1, INT_AGGR_DEF_TO);
	else
		ufshcd_disable_intr_aggr(hba);

	/* Configure UTRL and UTMRL base address registers */
	ufshcd_writel(hba, lower_32_bits(hba->utrdl_dma_addr),
			REG_UTP_TRANSFER_REQ_LIST_BASE_L);
	ufshcd_writel(hba, upper_32_bits(hba->utrdl_dma_addr),
			REG_UTP_TRANSFER_REQ_LIST_BASE_H);
	ufshcd_writel(hba, lower_32_bits(hba->utmrdl_dma_addr),
			REG_UTP_TASK_REQ_LIST_BASE_L);
	ufshcd_writel(hba, upper_32_bits(hba->utmrdl_dma_addr),
			REG_UTP_TASK_REQ_LIST_BASE_H);

	/*
	 * Make sure base address and interrupt setup are updated before
	 * enabling the run/stop registers below.
	 */
	wmb();

	/*
	 * UCRDY, UTMRLDY and UTRLRDY bits must be 1
	 */
	if (!(ufshcd_get_lists_status(ufshcd_readl(hba, REG_CONTROLLER_STATUS)))) {
		ufshcd_enable_run_stop_reg(hba);
	} else {
		dev_err(hba->dev,
			"Host controller not ready to process requests");
		return -EIO;
	}

	return 0;
}

/**
 * ufshcd_hba_stop - Send controller to reset state
 * @hba: per adapter instance
 * @can_sleep: perform sleep or just spin
 */
static inline void ufshcd_hba_stop(struct ufs_hba *hba, bool can_sleep)
{
	int err;

	ufshcd_writel(hba, CONTROLLER_DISABLE,  REG_CONTROLLER_ENABLE);
	err = ufshcd_wait_for_register(hba, REG_CONTROLLER_ENABLE,
					CONTROLLER_ENABLE, CONTROLLER_DISABLE,
					10, 1, can_sleep);
	if (err)
		dev_err(hba->dev, "%s: Controller disable failed\n", __func__);
}

/**
 * ufshcd_hba_enable - initialize the controller
 * @hba: per adapter instance
 *
 * The controller resets itself and controller firmware initialization
 * sequence kicks off. When controller is ready it will set
 * the Host Controller Enable bit to 1.
 *
 * Returns 0 on success, non-zero value on failure
 */
int ufshcd_hba_enable(struct ufs_hba *hba)
{
	int retry;

	/*
	 * msleep of 1 and 5 used in this function might result in msleep(20),
	 * but it was necessary to send the UFS FPGA to reset mode during
	 * development and testing of this driver. msleep can be changed to
	 * mdelay and retry count can be reduced based on the controller.
	 */
	if (!ufshcd_is_hba_active(hba))
		/* change controller state to "reset state" */
		ufshcd_hba_stop(hba, true);

	/* UniPro link is disabled at this point */
	ufshcd_set_link_off(hba);

	ufshcd_vops_hce_enable_notify(hba, PRE_CHANGE);

	/* start controller initialization sequence */
	ufshcd_hba_start(hba);

	/*
	 * To initialize a UFS host controller HCE bit must be set to 1.
	 * During initialization the HCE bit value changes from 1->0->1.
	 * When the host controller completes initialization sequence
	 * it sets the value of HCE bit to 1. The same HCE bit is read back
	 * to check if the controller has completed initialization sequence.
	 * So without this delay the value HCE = 1, set in the previous
	 * instruction might be read back.
	 * This delay can be changed based on the controller.
	 */
	msleep(1);

	/* wait for the host controller to complete initialization */
	retry = 10;
	while (ufshcd_is_hba_active(hba)) {
		if (retry) {
			retry--;
		} else {
			dev_err(hba->dev,
				"Controller enable failed\n");
			return -EIO;
		}
		msleep(5);
	}

	/* enable UIC related interrupts */
	ufshcd_enable_intr(hba, UFSHCD_UIC_MASK);
#ifdef CONFIG_HISI_UFS_HC
	ufshcd_hisi_enable_unipro_intr(hba, LINKUP_CNF_INTR | PEER_ATTR_COMPL_INTR);
#endif

	ufshcd_vops_hce_enable_notify(hba, POST_CHANGE);

	return 0;
}

static int ufshcd_disable_tx_lcc(struct ufs_hba *hba, bool peer)
{
	int tx_lanes = 0, i, err = 0;

	if (!peer)
		err = ufshcd_dme_get(hba, UIC_ARG_MIB(PA_CONNECTEDTXDATALANES),
			       &tx_lanes);
	else
		err = ufshcd_dme_peer_get(hba, UIC_ARG_MIB(PA_CONNECTEDTXDATALANES),
				    &tx_lanes);

	if (err) {
		dev_err(hba->dev, "%s: %s return err %d\n", __func__,
			peer ? "ufshcd_dme_peer_get" : "ufshcd_dme_get", err);
		return err;
	}
	for (i = 0; i < tx_lanes; i++) {
		if (!peer)
			err = ufshcd_dme_set(hba,
				UIC_ARG_MIB_SEL(TX_LCC_ENABLE,
					UIC_ARG_MPHY_TX_GEN_SEL_INDEX(i)),
					0);
		else
			err = ufshcd_dme_peer_set(hba,
				UIC_ARG_MIB_SEL(TX_LCC_ENABLE,
					UIC_ARG_MPHY_TX_GEN_SEL_INDEX(i)),
					0);
		if (err) {
			dev_err(hba->dev, "%s: TX LCC Disable failed, peer = %d, lane = %d, err = %d",
				__func__, peer, i, err);
			break;
		}
	}

	return err;
}

static inline int ufshcd_disable_device_tx_lcc(struct ufs_hba *hba)
{
	return ufshcd_disable_tx_lcc(hba, true);
}

/**
 * ufshcd_link_startup - Initialize unipro link startup
 * @hba: per adapter instance
 *
 * Returns 0 for success, non-zero in case of failure
 */
static int ufshcd_link_startup(struct ufs_hba *hba)
{
	int ret;
	uint32_t reg;
	int device_present;
	int linkup_success;
	int retries = DME_LINKSTARTUP_RETRIES;

	ret = ufshcd_vops_link_startup_notify(hba, PRE_CHANGE);
	if (ret)
		dev_err(hba->dev, "%s: ufshcd_vops_link_startup_notify return err %d \n", __func__, ret);
	do {
		ret = ufshcd_dme_link_startup(hba);
		if (ret && hba->vops && hba->vops->dbg_hci_dump)
			hba->vops->dbg_hci_dump(hba);
		device_present = ufshcd_is_device_present(hba);
		linkup_success = !ret && device_present;

		if (linkup_success) {
			reg = ufshcd_readl(hba, REG_INTERRUPT_STATUS);
			if (reg & UIC_LINK_STARTUP) {
				/**
				 * link startup casue a line reset, UIC Error
				 * may be set, clear UE, ULSS and UECPA
				 */
				ufshcd_writel(hba, UIC_LINK_STARTUP | UIC_ERROR,
					      REG_INTERRUPT_STATUS);
				/* REG_UIC_ERROR_CODE_PHY_ADAPTER_LAYER rc */
				(void)ufshcd_readl(
				    hba, REG_UIC_ERROR_CODE_PHY_ADAPTER_LAYER);
			}

		} else {
			ufshcd_update_error_stats(hba, UFS_ERR_LINKSTARTUP);
			ret = ufshcd_wait_for_register(hba, REG_INTERRUPT_STATUS,
						 UIC_LINK_STARTUP,
						 UIC_LINK_STARTUP, 1000, 50, false);
			if (ret)
				dev_err(hba->dev, "%s: ufshcd_wait_for_register return err %d \n", __func__, ret);
		}

	} while (!linkup_success && --retries);

	/* return an err if link startup failed, skip going on */
	if (!retries && !linkup_success) {
		ret = -EIO;
		goto out;
	}
	/* Mark that link is up in PWM-G1, 1-lane, SLOW-AUTO mode */
	ufshcd_init_pwr_info(hba);
	ufshcd_print_pwr_info(hba);

	if (hba->quirks & UFSHCD_QUIRK_BROKEN_LCC) {
		ret = ufshcd_disable_device_tx_lcc(hba);
		if (ret)
			goto out;
	}

	/* Include any host controller configuration via UIC commands */
	ret = ufshcd_vops_link_startup_notify(hba, POST_CHANGE);
	if (ret)
		goto out;

	ret = ufshcd_make_hba_operational(hba);
out:
	if (ret) {
		dev_err(hba->dev, "link startup failed %d\n", ret);
		ufshcd_print_host_state(hba);
		ufshcd_print_pwr_info(hba);
		ufshcd_print_host_regs(hba);
	}
	return ret;
}

/**
 * ufshcd_verify_dev_init() - Verify device initialization
 * @hba: per-adapter instance
 *
 * Send NOP OUT UPIU and wait for NOP IN response to check whether the
 * device Transport Protocol (UTP) layer is ready after a reset.
 * If the UTP layer at the device side is not initialized, it may
 * not respond with NOP IN UPIU within timeout of %NOP_OUT_TIMEOUT
 * and we retry sending NOP OUT for %NOP_OUT_RETRIES iterations.
 */
static int ufshcd_verify_dev_init(struct ufs_hba *hba, int retries)
{
	int err = 0;
	int timeout = NOP_OUT_TIMEOUT;
	struct ufs_kirin_host *host = hba->priv;
	/* FPGA platform need more time, maybe the response interrupt come slowly */
	if (host->caps & USE_HISI_MPHY_TC) {
		timeout = timeout * 10;
	}

	mutex_lock(&hba->dev_cmd.lock);
	for (; retries > 0; retries--) {
#ifdef OLD_DEVICE_CONSTRAINT
		/* delay 15ms before NOP after link startup and every try */
		msleep(15);
#endif

		err = ufshcd_exec_dev_cmd(hba, DEV_CMD_TYPE_NOP,
					       timeout);

		if (err && hba->vops && hba->vops->dbg_hci_dump)
			hba->vops->dbg_hci_dump(hba);

		if (!err || err == -ETIMEDOUT)
			break;

		dev_dbg(hba->dev, "%s: error %d retrying\n", __func__, err);
	}
	mutex_unlock(&hba->dev_cmd.lock);

	if (err)
		dev_err(hba->dev, "%s: NOP OUT failed %d\n", __func__, err);

	return err;
}

/**
 * ufshcd_set_queue_depth - set lun queue depth
 * @sdev: pointer to SCSI device
 *
 * Read bLUQueueDepth value and activate scsi tagged command
 * queueing. For WLUN, queue depth is set to 1. For best-effort
 * cases (bLUQueueDepth = 0) the queue depth is set to a maximum
 * value that host can queue.
 */
static void ufshcd_set_queue_depth(struct scsi_device *sdev)
{
	int ret = 0;
	u8 lun_qdepth;
	struct ufs_hba *hba;

	hba = shost_priv(sdev->host);

	lun_qdepth = hba->nutrs;
	ret = ufshcd_read_unit_desc_param(hba,
					  ufshcd_scsi_to_upiu_lun(sdev->lun),
					  UNIT_DESC_PARAM_LU_Q_DEPTH,
					  &lun_qdepth,
					  sizeof(lun_qdepth));

	/* Some WLUN doesn't support unit descriptor */
	if (ret == -EOPNOTSUPP)
		lun_qdepth = 1;
	else if (!lun_qdepth)
		/* eventually, we can figure out the real queue depth */
		lun_qdepth = hba->nutrs;
	else
		lun_qdepth = min_t(int, lun_qdepth, hba->nutrs);

	dev_dbg(hba->dev, "%s: activate tcq with queue depth %d\n",
			__func__, lun_qdepth);

	if (sdev->host->queue_quirk_flag & SHOST_QUIRK(SHOST_QUIRK_HISI_UFS_MQ))
		lun_qdepth =
			(u8)((lun_qdepth == hba->nutrs) ? hba->host->can_queue :
							  lun_qdepth);

	scsi_change_queue_depth(sdev, lun_qdepth);
}

/*
 * ufshcd_get_lu_wp - returns the "b_lu_write_protect" from UNIT DESCRIPTOR
 * @hba: per-adapter instance
 * @lun: UFS device lun id
 * @b_lu_write_protect: pointer to buffer to hold the LU's write protect info
 *
 * Returns 0 in case of success and b_lu_write_protect status would be returned
 * @b_lu_write_protect parameter.
 * Returns -ENOTSUPP if reading b_lu_write_protect is not supported.
 * Returns -EINVAL in case of invalid parameters passed to this function.
 */
static int ufshcd_get_lu_wp(struct ufs_hba *hba,
			    u8 lun,
			    u8 *b_lu_write_protect)
{
	int ret;

	if (!b_lu_write_protect)
		ret = -EINVAL;
	/*
	 * According to UFS device spec, RPMB LU can't be write
	 * protected so skip reading bLUWriteProtect parameter for
	 * it. For other W-LUs, UNIT DESCRIPTOR is not available.
	 */
	else if (lun >= UFS_UPIU_MAX_GENERAL_LUN)
		ret = -ENOTSUPP;
	else
		ret = ufshcd_read_unit_desc_param(hba,
					  lun,
					  UNIT_DESC_PARAM_LU_WR_PROTECT,
					  b_lu_write_protect,
					  sizeof(*b_lu_write_protect));
	return ret;
}

/**
 * ufshcd_get_lu_power_on_wp_status - get LU's power on write protect
 * status
 * @hba: per-adapter instance
 * @sdev: pointer to SCSI device
 *
 */
static inline void ufshcd_get_lu_power_on_wp_status(struct ufs_hba *hba,
						    struct scsi_device *sdev)
{
	if (hba->dev_info.f_power_on_wp_en &&
	    !hba->dev_info.is_lu_power_on_wp) {
		u8 b_lu_write_protect;

		if (!ufshcd_get_lu_wp(hba, ufshcd_scsi_to_upiu_lun(sdev->lun),
				      &b_lu_write_protect) &&
		    (b_lu_write_protect == UFS_LU_POWER_ON_WP))
			hba->dev_info.is_lu_power_on_wp = true;
	}
}

/**
 * ufshcd_slave_alloc - handle initial SCSI device configurations
 * @sdev: pointer to SCSI device
 *
 * Returns success
 */
static int ufshcd_slave_alloc(struct scsi_device *sdev)
{
	struct ufs_hba *hba;

	hba = shost_priv(sdev->host);

	/* indicates tag support */
	sdev->tagged_supported = 1;

	/* Mode sense(6) is not supported by UFS, so use Mode sense(10) */
	sdev->use_10_for_ms = 1;

	/* allow SCSI layer to restart the device in case of errors */
	sdev->allow_restart = 1;

	/* REPORT SUPPORTED OPERATION CODES is not supported */
	sdev->no_report_opcodes = 1;

	/* WRITE_SAME command is not supported */
	sdev->no_write_same = 1;

	sdev->sdev_bflags |= BLIST_SKIP_VPD_PAGES;

	ufshcd_set_queue_depth(sdev);

	ufshcd_get_lu_power_on_wp_status(hba, sdev);

	return 0;
}

/**
 * ufshcd_change_queue_depth - change queue depth
 * @sdev: pointer to SCSI device
 * @depth: required depth to set
 *
 * Change queue depth and make sure the max. limits are not crossed.
 */
static int ufshcd_change_queue_depth(struct scsi_device *sdev, int depth)
{
	struct ufs_hba *hba = shost_priv(sdev->host);


	if (depth > hba->nutrs)
		depth = hba->nutrs;

	if (sdev->host->queue_quirk_flag & SHOST_QUIRK(SHOST_QUIRK_HISI_UFS_MQ))
		depth = (depth == hba->nutrs) ? hba->host->can_queue : depth;

	return scsi_change_queue_depth(sdev, depth);
}

/**
 * ufshcd_slave_configure - adjust SCSI device configurations
 * @sdev: pointer to SCSI device
 */
static int ufshcd_slave_configure(struct scsi_device *sdev)
{
	struct request_queue *q = sdev->request_queue;

	blk_queue_update_dma_pad(q, PRDT_DATA_BYTE_COUNT_PAD - 1);
	blk_queue_max_segment_size(q, PRDT_DATA_BYTE_COUNT_MAX);
#ifdef CONFIG_HISI_BLOCK_ORDER_PRESERVING
	blk_queue_order_enable(q, sdev->host->order_enabled);
#endif
#ifdef CONFIG_HISI_UFS_MANUAL_BKOPS
	hisi_ufs_manual_bkops_config(sdev);
#endif
	return 0;
}

/**
 * ufshcd_slave_destroy - remove SCSI device configurations
 * @sdev: pointer to SCSI device
 */
static void ufshcd_slave_destroy(struct scsi_device *sdev)
{
	struct ufs_hba *hba;

	hba = shost_priv(sdev->host);
	/* Drop the reference as it won't be needed anymore */
	if (ufshcd_scsi_to_upiu_lun(sdev->lun) == UFS_UPIU_UFS_DEVICE_WLUN) {
		unsigned long flags;

		spin_lock_irqsave(hba->host->host_lock, flags);
		hba->sdev_ufs_device = NULL;
		spin_unlock_irqrestore(hba->host->host_lock, flags);
	}
}

/**
 * ufshcd_task_req_compl - handle task management request completion
 * @hba: per adapter instance
 * @index: index of the completed request
 * @resp: task management service response
 *
 * Returns non-zero value on error, zero on success
 */
static int ufshcd_task_req_compl(struct ufs_hba *hba, u32 index, u8 *resp)
{
	struct utp_task_req_desc *task_req_descp;
	struct utp_upiu_task_rsp *task_rsp_upiup;
	unsigned long flags;
	int ocs_value;
	int task_result;

	spin_lock_irqsave(hba->host->host_lock, flags);

	/* Clear completed tasks from outstanding_tasks */
	__clear_bit(index, &hba->outstanding_tasks);

	task_req_descp = hba->utmrdl_base_addr;
	ocs_value = ufshcd_get_tmr_ocs(&task_req_descp[index]);
	ufsdbg_error_inject_dispatcher(hba,
		ERR_INJECT_TM_OCS,
		TM_OCS_SUCCESS,
		&ocs_value);
	if (ocs_value == OCS_SUCCESS) {
		task_rsp_upiup = (struct utp_upiu_task_rsp *)
				task_req_descp[index].task_rsp_upiu;
		task_result = be32_to_cpu(task_rsp_upiup->output_param1);
		task_result = task_result & MASK_TM_SERVICE_RESP;
		ufsdbg_error_inject_dispatcher(hba,
			ERR_INJECT_TM_RSP,
			UPIU_TASK_MANAGEMENT_FUNC_SUCCEEDED,
			&task_result);
		if (resp)
			*resp = (u8)task_result;
	} else {
		dev_err(hba->dev, "%s: failed, ocs = 0x%x\n",
				__func__, ocs_value);
	}
	spin_unlock_irqrestore(hba->host->host_lock, flags);

	return ocs_value;
}
/**
 * ufshcd_scsi_cmd_status - Update SCSI command result based on SCSI status
 * @lrb: pointer to local reference block of completed command
 * @scsi_status: SCSI command status
 *
 * Returns value base on SCSI command status
 */
static inline int
ufshcd_scsi_cmd_status(struct ufshcd_lrb *lrbp, int scsi_status)
{
	int result = 0;

	switch (scsi_status) {
	case SAM_STAT_CHECK_CONDITION:
		ufshcd_copy_sense_data(lrbp);
	case SAM_STAT_GOOD:/*lint !e616*/
		result |= DID_OK << 16 |
			  COMMAND_COMPLETE << 8 |
			  scsi_status;
		break;
	case SAM_STAT_TASK_SET_FULL:
	case SAM_STAT_BUSY:
	case SAM_STAT_TASK_ABORTED:
		ufshcd_copy_sense_data(lrbp);
		result |= scsi_status;
		break;
	default:
		result |= DID_ERROR << 16;
		break;
	} /* end of switch */

	return result;
}

/**
 * ufshcd_transfer_rsp_status - Get overall status of the response
 * @hba: per adapter instance
 * @lrb: pointer to local reference block of completed command
 *
 * Returns result of the command to notify SCSI midlayer
 */
static inline int
ufshcd_transfer_rsp_status(struct ufs_hba *hba, struct ufshcd_lrb *lrbp, bool
		in_irq)
{
	int result = 0;
	int scsi_status;
	int ocs;
	int ret = 0;

	/* overall command status of utrd */
	ocs = ufshcd_get_tr_ocs(lrbp);

	ufsdbg_error_inject_dispatcher(hba,
		ERR_INJECT_TRANSFER_OCS,
		OCS_SUCCESS,
		&ocs);
	switch (ocs) {
	case OCS_SUCCESS:
		result = ufshcd_get_req_rsp(lrbp->ucd_rsp_ptr);
		hba->ufs_stats.last_hibern8_exit_tstamp = ktime_set(0, 0);
		switch (result) {
		case UPIU_TRANSACTION_RESPONSE:
			/*
			 * get the response UPIU result to extract
			 * the SCSI command status
			 */
			result = ufshcd_get_rsp_upiu_result(lrbp->ucd_rsp_ptr);
			ufsdbg_error_inject_dispatcher(hba,
						       ERR_INJECT_TRANSFER_RSP,
						       result & 0xff, &result);
			ufsdbg_error_inject_dispatcher(hba,
						       ERR_INJECT_SCSI_STATUS,
						       result & 0xff00,
						       &result);
			/*
			 * get the result based on SCSI status response
			 * to notify the SCSI midlayer of the command status
			 */
			scsi_status = result & MASK_SCSI_STATUS;

			result = ufshcd_scsi_cmd_status(lrbp, scsi_status);

			if ((scsi_status != SAM_STAT_GOOD) &&
			    (result != DID_ERROR << 16)) {
				/*check sense data updata exp info*/
				uhshcd_rsp_sense_data(hba, lrbp, scsi_status);
			}

			if(in_irq && scsi_status == SAM_STAT_GOOD)
				ufshpb_check_rsp_upiu_device_mode(hba, lrbp);

			/*
			 * currently we are only supporting BKOPs exception
			 * events hence we can ignore BKOPs exception event
			 * during power management callbacks. BKOPs exception
			 * event is not expected to be raised in runtime suspend
			 * callback as it allows the urgent bkops.
			 * During system suspend, we are anyway forcefully
			 * disabling the bkops and if urgent bkops is needed
			 * it will be enabled on system resume. Long term
			 * solution could be to abort the system suspend if
			 * UFS device needs urgent BKOPs.
			 */
			if (!hba->pm_op_in_progress &&
			    ufshcd_is_exception_event(lrbp->ucd_rsp_ptr))
				schedule_work(&hba->eeh_work);

			if (likely(lrbp->cmd) && hba->ufs_hpb &&
			    (scsi_status == SAM_STAT_GOOD) &&
			    (hba->manufacturer_id != UFS_VENDOR_SAMSUNG)) {
				if (hba->ufs_hpb->ufshpb_state ==
				    UFSHPB_PRESENT)
					ufshpb_check_rsp_upiu(hba, lrbp);
			}
			break;
		case UPIU_TRANSACTION_REJECT_UPIU:
			/* TODO: handle Reject UPIU Response */
			result = DID_ERROR << 16;
#ifdef HPB_HISI_DEBUG_PRINT
			pr_err("%s: result %d, line %d\n", __func__, result, __LINE__);
#endif
			dev_err(hba->dev,
				"Reject UPIU not fully implemented\n");
			break;
		default:
			result = DID_ERROR << 16;
#ifdef HPB_HISI_DEBUG_PRINT
			pr_err("%s: result %d, line %d\n", __func__, result, __LINE__);
#endif
			dev_err(hba->dev,
				"Unexpected request response code = %x\n",
				result);
			ret = dsm_ufs_update_ocs_info(hba, DSM_UFS_UTP_ERR,
						      ocs);
			if (!ret)
				schedule_ufs_dsm_work(hba);
			break;
		}
		break;
	case OCS_ABORTED:
		result |= DID_REQUEUE << 16;
		break;
	case OCS_INVALID_COMMAND_STATUS:
		result |= DID_REQUEUE << 16;
		break;
	case OCS_INVALID_CMD_TABLE_ATTR:
	case OCS_INVALID_PRDT_ATTR:
	case OCS_MISMATCH_DATA_BUF_SIZE:
	case OCS_MISMATCH_RESP_UPIU_SIZE:
	case OCS_PEER_COMM_FAILURE:
	case OCS_FATAL_ERROR:
	default:
		result |= DID_ERROR << 16;
		dev_err(hba->dev, "OCS error from controller = %x\n", ocs);
		ret = dsm_ufs_update_ocs_info(hba, DSM_UFS_UTP_ERR, ocs);
		if (!ret)
			schedule_ufs_dsm_work(hba);
		ufshcd_print_host_regs(hba);
		ufshcd_print_host_state(hba);
		break;
	} /* end of switch */

	if (host_byte(result) != DID_OK)
		ufshcd_print_trs(hba, 1UL << lrbp->task_tag, true);

	return result;
}

/**
 * ufshcd_uic_cmd_compl - handle completion of uic command
 * @hba: per adapter instance
 * @intr_status: interrupt status generated by the controller
 */
static void ufshcd_uic_cmd_compl(struct ufs_hba *hba, u32 intr_status)
{
	if ((intr_status & UIC_COMMAND_COMPL) && hba->active_uic_cmd) {
		hba->active_uic_cmd->argument2 |=
			(u32)ufshcd_get_uic_cmd_result(hba);
		ufsdbg_error_inject_dispatcher(hba,
			ERR_INJECT_UIC_CMD_ERR,
			UIC_CMD_RESULT_SUCCESS,
			&hba->active_uic_cmd->argument2);
		hba->active_uic_cmd->argument3 =
			ufshcd_get_dme_attr_val(hba);
		complete(&hba->active_uic_cmd->done);
	}

	if ((intr_status & UFSHCD_UIC_PWR_MASK) && hba->uic_async_done)
		complete(hba->uic_async_done);
}

/**
 * __ufshcd_transfer_req_compl - handle SCSI and query command completion
 * @hba: per adapter instance
 * @completed_reqs: requests to complete
 */
void __ufshcd_transfer_req_compl(struct ufs_hba *hba,
					unsigned long _completed_reqs)
{
	struct ufshcd_lrb *lrbp;
	struct scsi_cmnd *cmd;
	int result, index;
	struct request *req;
	unsigned long completed_reqs = _completed_reqs;
	uint8_t opcode = 0;
#ifndef CONFIG_HISI_UFS_HC_CORE_UTR
check:
#endif
	for_each_set_bit(index, &completed_reqs, hba->nutrs) {
		lrbp = &hba->lrb[index];

		cmd = lrbp->cmd;

		if (cmd && lrbp->command_type != UTP_CMD_TYPE_DEV_MANAGE) {
			ufshcd_update_tag_stats_completion(hba, cmd);
			result = ufshcd_transfer_rsp_status(hba, lrbp, true);
			if (!(hba->host->queue_quirk_flag & SHOST_QUIRK(SHOST_QUIRK_UNMAP_IN_SOFTIRQ)))
				scsi_dma_unmap(cmd);
			cmd->result = result;
			/* Mark completed command as NULL in LRB */
			lrbp->cmd = NULL;
			clear_bit_unlock(index, &hba->lrb_in_use);
			req = cmd->request;
			if (!(hba->host->queue_quirk_flag & SHOST_QUIRK(SHOST_QUIRK_HISI_UFS_MQ))) {
				if (req && req->lat_hist_enabled) {
					/* Update IO svc time latency histogram */
					ktime_t completion;
					u_int64_t delta_us;

					completion = ktime_get();
					delta_us = ktime_us_delta(completion,
						req->lat_hist_io_start);
					blk_update_latency_hist(
						(rq_data_dir(req) == READ) ?
						&hba->io_lat_read : &hba->io_lat_write, delta_us);
				}
			}
			opcode = cmd->cmnd[0];
			/* Do not touch lrbp after scsi done */
			if (((opcode == WRITE_BUFFER) &&
			     ((cmd->cmnd[1] & WB_MODE_MASK) == DOWNLOAD_MODE)) ||
			    (opcode == UNMAP))
				__ufshcd_enable_dev_tmt_cnt(hba);
#ifdef CONFIG_HUAWEI_DSM_IOMT_UFS_HOST
			iomt_host_latency_cmd_end(hba->host, cmd);
#endif
			cmd->scsi_done(cmd);
			pm_runtime_mark_last_busy(hba->dev);
			pm_runtime_put_autosuspend(hba->dev);
		} else if (lrbp->command_type == UTP_CMD_TYPE_DEV_MANAGE ||
			   lrbp->command_type == UTP_CMD_TYPE_UFS_STORAGE) {
			if (hba->dev_cmd.complete)
				complete(hba->dev_cmd.complete);
		}
		ufshcd_complete_time(hba, lrbp, opcode);
		update_req_stats(hba, lrbp);
	}
	/* clear corresponding bits of completed commands */
	hba->outstanding_reqs ^= completed_reqs;
#ifndef CONFIG_HISI_UFS_HC_CORE_UTR
	if (hba->host->queue_quirk_flag & SHOST_QUIRK(SHOST_QUIRK_HISI_UFS_MQ)) {
		if (hba->outstanding_reqs) {
			completed_reqs = ufshcd_readl(hba,
				REG_UTP_TRANSFER_REQ_DOOR_BELL) ^ hba->outstanding_reqs;
			if(completed_reqs)
				goto check;
		}
	}
#endif
	ufshcd_clk_scaling_update_busy(hba);
	/* we might have free'd some tags above */
	wake_up(&hba->dev_cmd.tag_wq);
}

/**
 * ufshcd_transfer_req_compl - handle SCSI and query command completion
 * @hba: per adapter instance
 */
static void ufshcd_transfer_req_compl(struct ufs_hba *hba)
{
#ifdef CONFIG_HISI_UFS_HC_CORE_UTR
	u32 cpu;

	for (cpu = 0; cpu < UTR_CORE_NUM; cpu++)
		ufshcd_core_transfer_req_compl(hba, cpu);
#else
	unsigned long completed_reqs;
	u32 tr_doorbell;

	/* Resetting interrupt aggregation counters first and reading the
	 * DOOR_BELL afterward allows us to handle all the completed requests.
	 * In order to prevent other interrupts starvation the DB is read once
	 * after reset. The down side of this solution is the possibility of
	 * false interrupt if device completes another request after resetting
	 * aggregation and before reading the DB.
	 */
	if (ufshcd_is_intr_aggr_allowed(hba))
		ufshcd_reset_intr_aggr(hba);

	tr_doorbell = ufshcd_readl(hba, REG_UTP_TRANSFER_REQ_DOOR_BELL);
	completed_reqs = tr_doorbell ^ hba->outstanding_reqs;

	__ufshcd_transfer_req_compl(hba, completed_reqs);
#endif
}

/**
 * ufshcd_disable_ee - disable exception event
 * @hba: per-adapter instance
 * @mask: exception event to disable
 *
 * Disables exception event in the device so that the EVENT_ALERT
 * bit is not set.
 *
 * Returns zero on success, non-zero error value on failure.
 */
static int ufshcd_disable_ee(struct ufs_hba *hba, u16 mask)
{
	int err = 0;
	u32 val;

	if (!(hba->ee_ctrl_mask & mask))
		goto out;

	val = hba->ee_ctrl_mask & ~mask;
	val &= MASK_EE_STATUS;
	err = ufshcd_query_attr_retry(hba, UPIU_QUERY_OPCODE_WRITE_ATTR,
			QUERY_ATTR_IDN_EE_CONTROL, 0, 0, &val);
	if (!err)
		hba->ee_ctrl_mask &= ~mask;
out:
	return err;
}

/**
 * ufshcd_enable_ee - enable exception event
 * @hba: per-adapter instance
 * @mask: exception event to enable
 *
 * Enable corresponding exception event in the device to allow
 * device to alert host in critical scenarios.
 *
 * Returns zero on success, non-zero error value on failure.
 */
static int ufshcd_enable_ee(struct ufs_hba *hba, u16 mask)
{
	int err = 0;
	u32 val;

	if (hba->ee_ctrl_mask & mask)
		goto out;

	val = hba->ee_ctrl_mask | mask;
	val &= MASK_EE_STATUS;
	err = ufshcd_query_attr_retry(hba, UPIU_QUERY_OPCODE_WRITE_ATTR,
			QUERY_ATTR_IDN_EE_CONTROL, 0, 0, &val);
	if (!err)
		hba->ee_ctrl_mask |= mask;
out:
	return err;
}

/**
 * ufshcd_enable_auto_bkops - Allow device managed BKOPS
 * @hba: per-adapter instance
 *
 * Allow device to manage background operations on its own. Enabling
 * this might lead to inconsistent latencies during normal data transfers
 * as the device is allowed to manage its own way of handling background
 * operations.
 *
 * Returns zero on success, non-zero on failure.
 */
static int ufshcd_enable_auto_bkops(struct ufs_hba *hba)
{
	int err = 0;

	if (hba->auto_bkops_enabled)
		goto out;

	err = ufshcd_query_flag_retry(hba, UPIU_QUERY_OPCODE_SET_FLAG,
			QUERY_FLAG_IDN_BKOPS_EN, NULL);
	if (err) {
		dev_err(hba->dev, "%s: failed to enable bkops %d\n",
				__func__, err);
		goto out;
	}

	hba->auto_bkops_enabled = true;

	/* No need of URGENT_BKOPS exception from the device */
	err = ufshcd_disable_ee(hba, MASK_EE_URGENT_BKOPS);
	if (err)
		dev_err(hba->dev, "%s: failed to disable exception event %d\n",
				__func__, err);
out:
	return err;
}

/**
 * ufshcd_disable_auto_bkops - block device in doing background operations
 * @hba: per-adapter instance
 *
 * Disabling background operations improves command response latency but
 * has drawback of device moving into critical state where the device is
 * not-operable. Make sure to call ufshcd_enable_auto_bkops() whenever the
 * host is idle so that BKOPS are managed effectively without any negative
 * impacts.
 *
 * Returns zero on success, non-zero on failure.
 */
static int ufshcd_disable_auto_bkops(struct ufs_hba *hba)
{
	int err = 0;
	int ret = 0;

	if (!hba->auto_bkops_enabled)
		goto out;

	/*
	 * If host assisted BKOPs is to be enabled, make sure
	 * urgent bkops exception is allowed.
	 */
	err = ufshcd_enable_ee(hba, MASK_EE_URGENT_BKOPS);
	if (err) {
		dev_err(hba->dev, "%s: failed to enable exception event %d\n",
				__func__, err);
		goto out;
	}

	err = ufshcd_query_flag_retry(hba, UPIU_QUERY_OPCODE_CLEAR_FLAG,
			QUERY_FLAG_IDN_BKOPS_EN, NULL);
	if (err) {
		dev_err(hba->dev, "%s: failed to disable bkops %d\n",
				__func__, err);
		ret = ufshcd_disable_ee(hba, MASK_EE_URGENT_BKOPS);
		if (ret)
			dev_err(hba->dev, "%s: failed to disable exception event %d \n", __func__, err);
		goto out;
	}

	hba->auto_bkops_enabled = false;
	trace_ufshcd_auto_bkops_state(dev_name(hba->dev), "Disabled");
out:
	return err;
}

#ifdef FEATURE_UFS_AUTO_BKOPS
/**
 * ufshcd_force_reset_auto_bkops - force reset auto bkops state
 * @hba: per adapter instance
 *
 * After a device reset the device may toggle the BKOPS_EN flag
 * to default value. The s/w tracking variables should be updated
 * as well. This function would change the auto-bkops state based on
 * UFSHCD_CAP_KEEP_AUTO_BKOPS_ENABLED_EXCEPT_SUSPEND.
 */
static void  ufshcd_force_reset_auto_bkops(struct ufs_hba *hba)
{
	if (ufshcd_keep_autobkops_enabled_except_suspend(hba)) {
		hba->auto_bkops_enabled = false;
		hba->ee_ctrl_mask |= MASK_EE_URGENT_BKOPS;
		ufshcd_enable_auto_bkops(hba);
	} else {
		hba->auto_bkops_enabled = true;
		hba->ee_ctrl_mask &= ~MASK_EE_URGENT_BKOPS;
		ufshcd_disable_auto_bkops(hba);
	}
}
#endif

static inline int ufshcd_get_bkops_status(struct ufs_hba *hba, u32 *status)
{
	return ufshcd_query_attr_retry(hba, UPIU_QUERY_OPCODE_READ_ATTR,
			QUERY_ATTR_IDN_BKOPS_STATUS, 0, 0, status);
}
#ifdef FEATURE_UFS_AUTO_BKOPS

/**
 * ufshcd_bkops_ctrl - control the auto bkops based on current bkops status
 * @hba: per-adapter instance
 * @status: bkops_status value
 *
 * Read the bkops_status from the UFS device and Enable fBackgroundOpsEn
 * flag in the device to permit background operations if the device
 * bkops_status is greater than or equal to "status" argument passed to
 * this function, disable otherwise.
 *
 * Returns 0 for success, non-zero in case of failure.
 *
 * NOTE: Caller of this function can check the "hba->auto_bkops_enabled" flag
 * to know whether auto bkops is enabled or disabled after this function
 * returns control to it.
 */
static int ufshcd_bkops_ctrl(struct ufs_hba *hba,
			     enum bkops_status status)
{
	int err;
	u32 curr_status = 0;

	err = ufshcd_get_bkops_status(hba, &curr_status);
	if (err) {
		dev_err(hba->dev, "%s: failed to get BKOPS status %d\n",
				__func__, err);
		goto out;
	} else if (curr_status > BKOPS_STATUS_MAX) {
		dev_err(hba->dev, "%s: invalid BKOPS status %d\n",
				__func__, curr_status);
		err = -EINVAL;
		goto out;
	}

	if (curr_status >= status) {
		if (ufshcd_is_auto_hibern8_allowed(hba)) {
			ufshcd_disable_auto_hibern8(hba);
		}
		err = ufshcd_enable_auto_bkops(hba);
		if (err) {
			if (ufshcd_is_auto_hibern8_allowed(hba)) {
				ufshcd_enable_auto_hibern8(hba);
			}
		}
	} else {
		err = ufshcd_disable_auto_bkops(hba);
		if (!err) {
			if (ufshcd_is_auto_hibern8_allowed(hba)) {
				ufshcd_enable_auto_hibern8(hba);
			}
		}
	}
out:
	return err;
}

/**
 * ufshcd_urgent_bkops - handle urgent bkops exception event
 * @hba: per-adapter instance
 *
 * Enable fBackgroundOpsEn flag in the device to permit background
 * operations.
 *
 * If BKOPs is enabled, this function returns 0, 1 if the bkops in not enabled
 * and negative error value for any other failure.
 */
static int ufshcd_urgent_bkops(struct ufs_hba *hba)
{
	return ufshcd_bkops_ctrl(hba, hba->urgent_bkops_lvl);
}
#endif

static inline int ufshcd_get_ee_status(struct ufs_hba *hba, u32 *status)
{
	return ufshcd_query_attr_retry(hba, UPIU_QUERY_OPCODE_READ_ATTR,
			QUERY_ATTR_IDN_EE_STATUS, 0, 0, status);
}

static void ufshcd_bkops_exception_event_handler(struct ufs_hba *hba)
{
	int err;
	u32 curr_status = 0;

	if (hba->is_urgent_bkops_lvl_checked)
		goto enable_auto_bkops;

	err = ufshcd_get_bkops_status(hba, &curr_status);
	if (err) {
		dev_err(hba->dev, "%s: failed to get BKOPS status %d\n",
				__func__, err);
		goto out;
	}

	/*
	 * We are seeing that some devices are raising the urgent bkops
	 * exception events even when BKOPS status doesn't indicate performace
	 * impacted or critical. Handle these device by determining their urgent
	 * bkops status at runtime.
	 */
	if (curr_status < BKOPS_STATUS_PERF_IMPACT) {
		dev_err(hba->dev, "%s: device raised urgent BKOPS exception for bkops status %d\n",
				__func__, curr_status);
		/* update the current status as the urgent bkops level */
		hba->urgent_bkops_lvl = curr_status;
		hba->is_urgent_bkops_lvl_checked = true;
	}

enable_auto_bkops:
	err = ufshcd_enable_auto_bkops(hba);
out:
	if (err < 0)
		dev_err(hba->dev, "%s: failed to handle urgent bkops %d\n",
				__func__, err);
}

static void ufshcd_wb_exception_event_handler(struct ufs_hba *hba)
{
	unsigned long flags;

	spin_lock_irqsave(hba->host->host_lock, flags);

	if (!hba->wb_exception_enabled)
		goto out;

	if (hba->wb_work_sched)
		goto out;
	hba->wb_work_sched = true;

	schedule_work(&hba->wb_work);

out:
	spin_unlock_irqrestore(hba->host->host_lock, flags);
}

/**
 * ufshcd_exception_event_handler - handle exceptions raised by device
 * @work: pointer to work data
 *
 * Read bExceptionEventStatus attribute from the device and handle the
 * exception event accordingly.
 */
static void ufshcd_exception_event_handler(struct work_struct *work)
{
	struct ufs_hba *hba;
	int err;
	u32 status = 0;
	hba = container_of(work, struct ufs_hba, eeh_work);

	pm_runtime_get_sync(hba->dev);

	scsi_block_requests(hba->host);
	err = ufshcd_get_ee_status(hba, &status);
	if (err) {
		dev_err(hba->dev, "%s: failed to get exception status %d\n",
				__func__, err);
		goto out;
	}

	status &= hba->ee_ctrl_mask;

	if (status & MASK_EE_URGENT_BKOPS)
		ufshcd_bkops_exception_event_handler(hba);
	if (status & MASK_EE_WRITEBOOSTER_EVENT)
		ufshcd_wb_exception_event_handler(hba);

out:
	scsi_unblock_requests(hba->host);
	pm_runtime_put_sync(hba->dev);
	return;
}

static void ufshcd_complete_requests(struct ufs_hba *hba)
{
	ufshcd_transfer_req_compl(hba);
	ufshcd_tmc_handler(hba);
}

/**
 * ufshcd_quirk_dl_nac_errors - This function checks if error handling is
 *				to recover from the DL NAC errors or not.
 * @hba: per-adapter instance
 *
 * Returns true if error handling is required, false otherwise
 */
static bool ufshcd_quirk_dl_nac_errors(struct ufs_hba *hba)
{
	unsigned long flags;
	bool err_handling = true;

	spin_lock_irqsave(hba->host->host_lock, flags);
	/*
	 * UFS_DEVICE_QUIRK_RECOVERY_FROM_DL_NAC_ERRORS only workaround the
	 * device fatal error and/or DL NAC & REPLAY timeout errors.
	 */
	if (hba->saved_err & (CONTROLLER_FATAL_ERROR | SYSTEM_BUS_FATAL_ERROR))
		goto out;

	if ((hba->saved_err & DEVICE_FATAL_ERROR) ||
	    ((hba->saved_err & UIC_ERROR) &&
	     (hba->saved_uic_err & UFSHCD_UIC_DL_TCx_REPLAY_ERROR)))
		goto out;

	if ((hba->saved_err & UIC_ERROR) &&
	    (hba->saved_uic_err & UFSHCD_UIC_DL_NAC_RECEIVED_ERROR)) {
		int err;
		/*
		 * wait for 50ms to see if we can get any other errors or not.
		 */
		spin_unlock_irqrestore(hba->host->host_lock, flags);
		msleep(50);
		spin_lock_irqsave(hba->host->host_lock, flags);

		/*
		 * now check if we have got any other severe errors other than
		 * DL NAC error?
		 */
		if ((hba->saved_err & INT_FATAL_ERRORS) ||
		    ((hba->saved_err & UIC_ERROR) &&
		    (hba->saved_uic_err & ~UFSHCD_UIC_DL_NAC_RECEIVED_ERROR)))
			goto out;

		/*
		 * As DL NAC is the only error received so far, send out NOP
		 * command to confirm if link is still active or not.
		 *   - If we don't get any response then do error recovery.
		 *   - If we get response then clear the DL NAC error bit.
		 */

		spin_unlock_irqrestore(hba->host->host_lock, flags);
		err = ufshcd_verify_dev_init(hba, NOP_OUT_RETRIES);
		spin_lock_irqsave(hba->host->host_lock, flags);

		if (err)
			goto out;

		/* Link seems to be alive hence ignore the DL NAC errors */
		if (hba->saved_uic_err == UFSHCD_UIC_DL_NAC_RECEIVED_ERROR)
			hba->saved_err &= ~UIC_ERROR;
		/* clear NAC error */
		hba->saved_uic_err &= ~UFSHCD_UIC_DL_NAC_RECEIVED_ERROR;
		if (!hba->saved_uic_err) {
			err_handling = false;
			goto out;
		}
	}
out:
	spin_unlock_irqrestore(hba->host->host_lock, flags);
	return err_handling;
}

/**
 * ufshcd_err_handler - handle UFS errors that require s/w attention
 * @work: pointer to work structure
 */
static void ufshcd_err_handler(struct kthread_work *work)
{
	struct ufs_hba *hba;
	unsigned long flags;
	bool err_xfer = false, err_tm = false;
	int tag;
	bool needs_reset = false;

	hba = container_of(work, struct ufs_hba, eh_work);

	mutex_lock(&hba->eh_mutex);
	dev_err(hba->dev, "%s start.\n", __func__);

	pm_runtime_disable(hba->dev);
	pm_runtime_set_active(hba->dev);
	hba->error_handle_count++;

	spin_lock_irqsave(hba->host->host_lock, flags);

	/*
	 * avoid that initialization task and exception task are executed
	 * concurrently.
	 */
	if ((hba->ufshcd_state == UFSHCD_STATE_RESET) || (hba->ufs_init_completed == false))
		goto out;
	hba->ufshcd_state = UFSHCD_STATE_RESET;

	ufshcd_set_eh_in_progress(hba);

	/* Complete requests that have door-bell cleared by h/w */
	ufshcd_complete_requests(hba);

	if (hba->dev_quirks & UFS_DEVICE_QUIRK_RECOVERY_FROM_DL_NAC_ERRORS) {
		bool ret;

		spin_unlock_irqrestore(hba->host->host_lock, flags);
		/* release the lock as ufshcd_quirk_dl_nac_errors() may sleep */
		ret = ufshcd_quirk_dl_nac_errors(hba);
		spin_lock_irqsave(hba->host->host_lock, flags);
		if (!ret)
			goto skip_err_handling;
	}
	if ((hba->saved_err & INT_FATAL_ERRORS) || hba->force_host_reset ||
		((hba->saved_err & UIC_ERROR) &&
			(hba->saved_uic_err & (UFSHCD_UIC_DL_PA_INIT_ERROR))) ||
		(ufshcd_is_auto_hibern8_allowed(hba) &&
			(hba->saved_err &
				(UIC_HIBERNATE_ENTER | UIC_HIBERNATE_EXIT))))
		needs_reset = true;

	/*
	 * if host reset is required then skip clearing the pending
	 * transfers forcefully because they will automatically get
	 * cleared after link startup.
	 */
	if (needs_reset)
		goto skip_pending_xfer_clear;

	/* release lock as clear command might sleep */
	spin_unlock_irqrestore(hba->host->host_lock, flags);
	/* Clear pending transfer requests */
	for_each_set_bit(tag, &hba->outstanding_reqs, hba->nutrs) {
		if (ufshcd_clear_cmd(hba, tag)) {
			err_xfer = true;
			goto lock_skip_pending_xfer_clear;
		}
	}

	/* Clear pending task management requests */
	for_each_set_bit(tag, &hba->outstanding_tasks, hba->nutmrs) {
		if (ufshcd_clear_tm_cmd(hba, tag)) {
			err_tm = true;
			goto lock_skip_pending_xfer_clear;
		}
	}

lock_skip_pending_xfer_clear:
	spin_lock_irqsave(hba->host->host_lock, flags);

	/* Complete the requests that are cleared by s/w */
	ufshcd_complete_requests(hba);

	if (err_xfer || err_tm)
		needs_reset = true;

skip_pending_xfer_clear:
	/* Fatal errors need reset */
	if (needs_reset)
		ufshcd_err_handler_do_reset(hba, err_xfer, err_tm, &flags);

skip_err_handling:
	if (!needs_reset) {
		hba->ufshcd_state = UFSHCD_STATE_OPERATIONAL;
		if (hba->saved_err || hba->saved_uic_err)
			dev_err_ratelimited(hba->dev, "%s: exit: saved_err 0x%x saved_uic_err 0x%x",
			    __func__, hba->saved_err, hba->saved_uic_err);
	}

	ufshcd_clear_eh_in_progress(hba);
out:
	spin_unlock_irqrestore(hba->host->host_lock, flags);
	scsi_unblock_requests(hba->host);
	pm_runtime_enable(hba->dev);
	schedule_ufs_dsm_work(hba);

	mutex_unlock(&hba->eh_mutex);
}

/**
 * ufshcd_update_uic_error - check and set fatal UIC error flags.
 * @hba: per-adapter instance
 */
static void ufshcd_update_uic_error(struct ufs_hba *hba)
{
	u32 reg;
#ifdef CONFIG_SCSI_UFS_KIRIN_LINERESET_CHECK
	reg = hba->reg_uecpa;
	if (reg)
		ufshcd_update_uic_reg_hist(&hba->ufs_stats.pa_err, reg);
#else
	/* PHY layer lane error */
	reg = ufshcd_readl(hba, REG_UIC_ERROR_CODE_PHY_ADAPTER_LAYER);
	/* Ignore LINERESET indication, as this is not an error */
	if ((reg & UIC_PHY_ADAPTER_LAYER_ERROR) &&
			(reg & UIC_PHY_ADAPTER_LAYER_LANE_ERR_MASK)) {
		/*
		 * To know whether this error is fatal or not, DB timeout
		 * must be checked but this error is handled separately.
		 */
		dev_dbg(hba->dev, "%s: UIC Lane error reported\n", __func__);
		ufshcd_update_uic_reg_hist(&hba->ufs_stats.pa_err, reg);
	}
#endif


	/* PA_INIT_ERROR is fatal and needs UIC reset */
	reg = ufshcd_readl(hba, REG_UIC_ERROR_CODE_DATA_LINK_LAYER);
	if (reg)
		ufshcd_update_uic_reg_hist(&hba->ufs_stats.dl_err, reg);
	if (reg & UIC_DATA_LINK_LAYER_ERROR_PA_INIT)
		hba->uic_error |= UFSHCD_UIC_DL_PA_INIT_ERROR;
	else if (hba->dev_quirks &
		   UFS_DEVICE_QUIRK_RECOVERY_FROM_DL_NAC_ERRORS) {
		if (reg & UIC_DATA_LINK_LAYER_ERROR_NAC_RECEIVED)
			hba->uic_error |=
				UFSHCD_UIC_DL_NAC_RECEIVED_ERROR;
		else if (reg & UIC_DATA_LINK_LAYER_ERROR_TCx_REPLAY_TIMEOUT)
			hba->uic_error |= UFSHCD_UIC_DL_TCx_REPLAY_ERROR;
	}


	/* UIC NL/TL/DME errors needs software retry */
	reg = ufshcd_readl(hba, REG_UIC_ERROR_CODE_NETWORK_LAYER);
	if (reg) {
		ufshcd_update_uic_reg_hist(&hba->ufs_stats.nl_err, reg);
		hba->uic_error |= UFSHCD_UIC_NL_ERROR;
	}

	reg = ufshcd_readl(hba, REG_UIC_ERROR_CODE_TRANSPORT_LAYER);
	if (reg) {
		ufshcd_update_uic_reg_hist(&hba->ufs_stats.tl_err, reg);
		hba->uic_error |= UFSHCD_UIC_TL_ERROR;
	}

	reg = ufshcd_readl(hba, REG_UIC_ERROR_CODE_DME);
	if (reg) {
		ufshcd_update_uic_reg_hist(&hba->ufs_stats.dme_err, reg);
		hba->uic_error |= UFSHCD_UIC_DME_ERROR;
	}

	ufsdbg_error_inject_dispatcher(hba,
		ERR_INJECT_UIC_INTR_CAUSE,
		0,
		&hba->uic_error); /*lint !e64*/

	dsm_ufs_update_uic_info(hba, DSM_UFS_UIC_TRANS_ERR);

	if (hba->uic_error & UFSHCD_UIC_DL_PA_INIT_ERROR) {
		dsm_ufs_update_error_info(hba, DSM_UFS_CONTROLLER_ERR);
	}

	dev_dbg(hba->dev, "%s: UIC error flags = 0x%08x\n",
			__func__, hba->uic_error);
}

/**
 * ufshcd_check_errors - Check for errors that need s/w attention
 * @hba: per-adapter instance
 */
void ufshcd_check_errors(struct ufs_hba *hba)
{
	bool queue_eh_work = false;

	if (hba->is_hibernate) {
		dev_err(hba->dev, "%s err isr when hibernate, err is %u\n",
			__func__, hba->errors);
#ifdef CONFIG_SCSI_UFS_HS_ERROR_RECOVER
		hba->check_pwm_after_h8 = 10;
#endif
#ifdef CONFIG_HISI_UFS_HC
		dev_err(hba->dev, "not handle erros in h8 for hisi ufs\n");
		return;
#endif
	}

	queue_eh_work = ufshcd_error_need_queue_work(hba);

	if (hba->errors & UIC_ERROR) {
		hba->uic_error = 0;
		ufshcd_update_uic_error(hba);
		if (hba->uic_error)
			queue_eh_work = true;
		else
			schedule_ufs_dsm_work(hba);
	}

	if (queue_eh_work) {
		/*
		 * update the transfer error masks to sticky bits, let's do this
		 * irrespective of current ufshcd_state.
		 */
		hba->saved_err |= hba->errors;
		hba->saved_uic_err |= hba->uic_error;

		/* handle fatal errors only when link is functional */
		if (hba->ufshcd_state == UFSHCD_STATE_OPERATIONAL) {
			/* block commands from scsi mid-layer */
			scsi_block_requests(hba->host);

			hba->ufshcd_state = UFSHCD_STATE_EH_SCHEDULED;

			/* dump controller state before resetting */
			if ((hba->saved_err & (INT_FATAL_ERRORS | UIC_ERROR)) ||
				(ufshcd_is_auto_hibern8_allowed(hba) &&
					(hba->saved_err &
						(UIC_HIBERNATE_ENTER |
							UIC_HIBERNATE_EXIT)))
#ifdef CONFIG_HISI_UFS_HC
				|| (ufshcd_is_auto_hibern8_allowed(hba) &&
					   (hba->saved_err &
						   AH8_EXIT_REQ_CNF_FAIL_INTR))

#endif
					) {
				bool pr_prdt = !!(hba->saved_err &
						SYSTEM_BUS_FATAL_ERROR);

				dev_err(hba->dev, "%s: saved_err 0x%x saved_uic_err 0x%x\n",
					__func__, hba->saved_err,
					hba->saved_uic_err);

				ufshcd_print_host_regs(hba);
				ufshcd_print_pwr_info(hba);
				ufshcd_print_tmrs(hba, hba->outstanding_tasks);
				ufshcd_print_trs(hba, hba->outstanding_reqs,
							pr_prdt);
				dev_err(hba->dev, "%s: dump ended\n", __func__);
			}
			if (!kthread_queue_work(&hba->eh_worker, &hba->eh_work))
				dev_err(hba->dev, "%s: queue hba->eh_worker failed !\r\n", __func__);
		}
	}

	/*
	 * if (!queue_eh_work) -
	 * Other errors are either non-fatal where host recovers
	 * itself without s/w intervention or errors that will be
	 * handled by the SCSI core layer.
	 */
}

/**
 * ufshcd_tmc_handler - handle task management function completion
 * @hba: per adapter instance
 */
static void ufshcd_tmc_handler(struct ufs_hba *hba)
{
	u32 tm_doorbell;

	tm_doorbell = ufshcd_readl(hba, REG_UTP_TASK_REQ_DOOR_BELL);
	hba->tm_condition = tm_doorbell ^ hba->outstanding_tasks;
	wake_up(&hba->tm_wq);
}

/**
 * ufshcd_sl_intr - Interrupt service routine
 * @hba: per adapter instance
 * @intr_status: contains interrupts generated by the controller
 */
static void ufshcd_sl_intr(struct ufs_hba *hba, u32 intr_status)
{
	ufsdbg_error_inject_dispatcher(hba,
		ERR_INJECT_INTR,
		intr_status,
		&intr_status);
	hba->errors = UFSHCD_ERROR_MASK & intr_status;
	if (ufshcd_is_auto_hibern8_allowed(hba))
		hba->errors |= (intr_status &
				(UIC_HIBERNATE_ENTER | UIC_HIBERNATE_EXIT));

	if (hba->errors & UIC_ERROR) {
		ufshcd_disable_intr(hba, UIC_ERROR);
	}

#ifdef CONFIG_SCSI_UFS_INLINE_CRYPTO
	if (ufshcd_support_inline_encrypt(hba))
		hba->errors |= CRYPTO_ENGINE_FATAL_ERROR & intr_status;
#endif

	if (hba->errors)
		ufshcd_check_errors(hba);

	if (unlikely(intr_status & UTP_ERROR))
		dev_err(hba->dev, "%s, UTP ERROR occurs, intr 0x%x\n", __func__,
			intr_status);

	if (intr_status & UFSHCD_UIC_MASK)
		ufshcd_uic_cmd_compl(hba, intr_status);

	if (intr_status & UTP_TASK_REQ_COMPL)
		ufshcd_tmc_handler(hba);

	if (intr_status & UTP_TRANSFER_REQ_COMPL)
		ufshcd_transfer_req_compl(hba);
}

/**
 * ufshcd_intr - Main interrupt service routine
 * @irq: irq number
 * @__hba: pointer to adapter instance
 *
 * Returns IRQ_HANDLED - If interrupt is valid
 *		IRQ_NONE - If invalid interrupt
 */
#ifndef CONFIG_SCSI_UFS_INTR_HUB
static irqreturn_t ufshcd_intr(int irq, void *__hba)
#else
irqreturn_t ufshcd_intr(int irq, void *__hba)
#endif
{
	u32 intr_status, enabled_intr_status;
	irqreturn_t retval = IRQ_NONE;
	struct ufs_hba *hba = __hba;

	spin_lock(hba->host->host_lock);
	intr_status = ufshcd_readl(hba, REG_INTERRUPT_STATUS);
	ufshcd_hisi_is_intr_inject(hba, &intr_status);
	enabled_intr_status =
		intr_status & ufshcd_readl(hba, REG_INTERRUPT_ENABLE);

	hba->last_intr = intr_status;
	hba->last_intr_time_stamp = ktime_get();

	if (intr_status)
		ufshcd_writel(hba, intr_status, REG_INTERRUPT_STATUS);

	if (enabled_intr_status) {
		ufshcd_sl_intr(hba, enabled_intr_status);
		retval = IRQ_HANDLED;
	}

	if (ufshcd_hisi_vs_intr(hba) == IRQ_HANDLED)
		retval = IRQ_HANDLED;

	spin_unlock(hba->host->host_lock);
	return retval;
}

static int ufshcd_clear_tm_cmd(struct ufs_hba *hba, int tag)
{
	int err = 0;
	u32 mask = 1U << (u32)tag;
	unsigned long flags;

	if (!test_bit(tag, &hba->outstanding_tasks))
		goto out;

	spin_lock_irqsave(hba->host->host_lock, flags);
	ufshcd_writel(hba, ~(1U << (u32)tag), REG_UTP_TASK_REQ_LIST_CLEAR);
	spin_unlock_irqrestore(hba->host->host_lock, flags);

	/* poll for max. 1 sec to clear door bell register by h/w */
	err = ufshcd_wait_for_register(hba,
			REG_UTP_TASK_REQ_DOOR_BELL,
			mask, 0, 1000, 1000, true);
out:
	return err;
}

/**
 * ufshcd_issue_tm_cmd - issues task management commands to controller
 * @hba: per adapter instance
 * @lun_id: LUN ID to which TM command is sent
 * @task_id: task ID to which the TM command is applicable
 * @tm_function: task management function opcode
 * @tm_response: task management service response return value
 *
 * Returns non-zero value on error, zero on success.
 */
static int ufshcd_issue_tm_cmd(struct ufs_hba *hba, int lun_id, int task_id,
		u8 tm_function, u8 *tm_response)
{
	struct utp_task_req_desc *task_req_descp;
	struct utp_upiu_task_req *task_req_upiup;
	struct Scsi_Host *host;
	unsigned long flags;
	int free_slot;
	int err;
	int task_tag;

	host = hba->host;
	/*
	 * Get free slot, sleep if slots are unavailable.
	 * Even though we use wait_event() which sleeps indefinitely,
	 * the maximum wait time is bounded by %TM_CMD_TIMEOUT.
	 */
	wait_event(hba->tm_tag_wq,
	    ufshcd_get_tm_free_slot(hba, &free_slot));/*lint !e666*/
	pm_runtime_get_sync(hba->dev);


	spin_lock_irqsave(host->host_lock, flags);
	while (hba->is_hibernate) {
		spin_unlock_irqrestore(host->host_lock, flags);
		printk("warring send tm in H8, %s\n", __func__);
		err = ufshcd_uic_hibern8_exit(hba);
		if (err)
			return err;
		spin_lock_irqsave(host->host_lock, flags);
	}
	task_req_descp = hba->utmrdl_base_addr;
	task_req_descp += free_slot;

	/* Configure task request descriptor */
	task_req_descp->header.dword_0 = cpu_to_le32(UTP_REQ_DESC_INT_CMD);
	task_req_descp->header.dword_2 =
			cpu_to_le32(OCS_INVALID_COMMAND_STATUS);

	/* Configure task request UPIU */
	task_req_upiup =
		(struct utp_upiu_task_req *) task_req_descp->task_req_upiu;
	task_tag = hba->nutrs + free_slot;
	task_req_upiup->header.dword_0 =
		UPIU_HEADER_DWORD(UPIU_TRANSACTION_TASK_REQ, 0,
					      lun_id, task_tag);
	task_req_upiup->header.dword_1 =
		UPIU_HEADER_DWORD(0, tm_function, 0, 0);
	/*
	 * The host shall provide the same value for LUN field in the basic
	 * header and for Input Parameter.
	 */
	task_req_upiup->input_param1 = cpu_to_be32(lun_id);
	task_req_upiup->input_param2 = cpu_to_be32(task_id);

	ufshcd_vops_setup_task_mgmt(hba, free_slot, tm_function);

	/* send command to the controller */
	__set_bit(free_slot, &hba->outstanding_tasks);

	/* Make sure descriptors are ready before ringing the task doorbell */
	wmb();

	ufshcd_writel(hba, 1 << free_slot, REG_UTP_TASK_REQ_DOOR_BELL);
	/* Make sure that doorbell is committed immediately */
	wmb();

	spin_unlock_irqrestore(host->host_lock, flags);

	/* wait until the task management command is completed */
	/*lint -save -e666*/
	err = wait_event_timeout(hba->tm_wq,
			test_bit(free_slot, &hba->tm_condition),
			msecs_to_jiffies(TM_CMD_TIMEOUT));
	/*lint -restore*/
	if (!err) {
		dev_err(hba->dev, "%s: task management cmd 0x%.2x timed-out\n",
				__func__, tm_function);
		if (ufshcd_clear_tm_cmd(hba, free_slot))
			dev_WARN(hba->dev, "%s: unable clear tm cmd (slot %d) after timeout\n",
					__func__, free_slot); /*lint !e146* !e665*/
		err = -ETIMEDOUT;
	} else {
		err = ufshcd_task_req_compl(hba, free_slot, tm_response);
	}

	clear_bit(free_slot, &hba->tm_condition);
	ufshcd_put_tm_slot(hba, free_slot);
	wake_up(&hba->tm_tag_wq);

	pm_runtime_mark_last_busy(hba->dev);
	pm_runtime_put_autosuspend(hba->dev);
	return err;
}

/**
 * ufshcd_eh_device_reset_handler - device reset handler registered to
 *                                    scsi layer.
 * @cmd: SCSI command pointer
 *
 * Returns SUCCESS/FAILED
 */
static int ufshcd_eh_device_reset_handler(struct scsi_cmnd *cmd)
{
	struct Scsi_Host *host;
	struct ufs_hba *hba;
	unsigned int tag;
	int pos;
	int err;
	u8 resp = 0xF;
	struct ufshcd_lrb *lrbp;
	unsigned long flags;

	host = cmd->device->host;
	hba = shost_priv(host);
	if (!host->use_blk_mq)
		tag = cmd->request->tag;
	else
		tag = cmd->tag;

	dev_err(hba->dev, "%s: occurs\n", __func__);

	lrbp = &hba->lrb[tag];
	err = ufshcd_issue_tm_cmd(hba, lrbp->lun, 0, UFS_LOGICAL_RESET, &resp);
	if (err || resp != UPIU_TASK_MANAGEMENT_FUNC_COMPL) {
		if (!err)
			err = resp;
		goto out;
	}

	/* clear the commands that were pending for corresponding LUN */
	for_each_set_bit(pos, &hba->outstanding_reqs, hba->nutrs) {
		if (hba->lrb[pos].lun == lrbp->lun) {
			err = ufshcd_clear_cmd(hba, pos);
			if (err)
				break;
		}
	}
	spin_lock_irqsave(host->host_lock, flags);
	ufshcd_transfer_req_compl(hba);
	spin_unlock_irqrestore(host->host_lock, flags);
out:
	hba->req_abort_count = 0;
	if (!err) {
		err = SUCCESS;
	} else {
		dev_err(hba->dev, "%s: failed with err %d\n", __func__, err);
		err = FAILED;
	}
	return err;
}

static unsigned int ufshcd_get_tag_from_cmd(struct Scsi_Host *host, struct ufs_hba *hba,
											struct scsi_cmnd *cmd)
{
	unsigned int tag;
#ifdef CONFIG_HISI_UFS_HC_CORE_UTR
	tag = (unsigned int)cmd->tag;
#else
	if (!(host->queue_quirk_flag & SHOST_QUIRK(SHOST_QUIRK_DRIVER_TAG_ALLOC)))
		tag = (unsigned int)cmd->request->tag;
	else
		tag = (unsigned int)cmd->tag;
#endif

	if (!ufshcd_valid_tag(hba, tag)) {
		dev_err(hba->dev,
			"%s: invalid command tag %d: cmd=0x%pK, cmd->request=0x%pK",
			__func__, tag, cmd, cmd->request);
		BUG();  /*lint !e146*/
	}

	return tag;
}
/**
 * ufshcd_abort - abort a specific command
 * @cmd: SCSI command pointer
 *
 * Abort the pending command in device by sending UFS_ABORT_TASK task management
 * command, and in host controller by clearing the door-bell register. There can
 * be race between controller sending the command to the device while abort is
 * issued. To avoid that, first issue UFS_QUERY_TASK to check if the command is
 * really issued and then try to abort it.
 *
 * Returns SUCCESS/FAILED
 */
static int ufshcd_abort(struct scsi_cmnd *cmd)
{
	struct Scsi_Host *host;
	struct ufs_hba *hba;
	unsigned long flags;
	unsigned int tag;
	int err = 0;
	int poll_cnt;
	u8 resp = 0xF;
	struct ufshcd_lrb *lrbp;
	u32 reg;

	host = cmd->device->host;
	hba = shost_priv(host);
	tag = ufshcd_get_tag_from_cmd(host, hba, cmd);
	lrbp = &hba->lrb[tag];
	ufshcd_update_error_stats(hba, UFS_ERR_TASK_ABORT);

	/*
	 * Task abort to the device W-LUN is illegal. When this command
	 * will fail, due to spec violation, scsi err handling next step
	 * will be to send LU reset which, again, is a spec violation.
	 * To avoid these unnecessary/illegal step we skip to the last error
	 * handling stage: reset and restore.
	 */
	if (lrbp->lun == UFS_UPIU_UFS_DEVICE_WLUN)
		return ufshcd_eh_host_reset_handler(cmd);

	ufshcd_update_error_stats(hba, UFS_ERR_TASK_ABORT);

#ifdef CONFIG_HISI_UFS_HC_CORE_UTR
	reg = ufshcd_readl(hba, UFS_CORE_UTRLDBR(tag / SLOT_NUM_EACH_CORE));
#else
	reg = ufshcd_readl(hba, REG_UTP_TRANSFER_REQ_DOOR_BELL);
#endif
	/* If command is already aborted/completed, return SUCCESS */
	if (!(test_bit(tag, &hba->outstanding_reqs))) {
		dev_err(hba->dev,
			"%s: cmd at tag %d already completed, outstanding=0x%lx, doorbell=0x%x\n",
			__func__, tag, hba->outstanding_reqs, reg);
		goto out;
	}

#ifdef CONFIG_HISI_UFS_HC_CORE_UTR
	if (!(reg & (1 << (tag % SLOT_NUM_EACH_CORE)))) {
#else
	if (!(reg & (1 << tag))) {
#endif
		dev_err(hba->dev,
		"%s: cmd was completed, but without a notifying intr, tag = %d",
		__func__, tag);
		ufshcd_print_host_regs(hba);
		ufshcd_print_trs(hba, 1UL << tag, false);

		/* The Doorbell register has been cleared. But the CPU has not
		 * received the interrupt, so it panic instantaneously to check
		 * the interrupt sequence and task sequence.
		 */
		HISI_UFS_BUG();  /*lint !e146*/
	}

	/* Print Transfer Request of aborted task */
	dev_err(hba->dev, "%s: Device abort task at tag %d\n", __func__, tag);
	/*
	 * Print detailed info about aborted request.
	 * As more than one request might get aborted at the same time,
	 * print full information only for the first aborted request in order
	 * to reduce repeated printouts. For other aborted requests only print
	 * basic details.
	 */
	scsi_print_command(hba->lrb[tag].cmd);
	if (!hba->req_abort_count) {
		ufshcd_print_host_regs(hba);
		ufshcd_print_host_state(hba);
		ufshcd_print_pwr_info(hba);
	}
	ufshcd_print_trs(hba, 1UL << tag, !hba->req_abort_count);

	hba->req_abort_count++;

	/* Skip task abort in case previous aborts failed and report failure */
	if (lrbp->req_abort_skip) {
		err = -EIO;
		goto out;
	}

	for (poll_cnt = 100; poll_cnt; poll_cnt--) {
		err = ufshcd_issue_tm_cmd(hba, lrbp->lun, lrbp->task_tag,
				UFS_QUERY_TASK, &resp);
		if (!err && resp == UPIU_TASK_MANAGEMENT_FUNC_SUCCEEDED) {
			/* cmd pending in the device */
			dev_err(hba->dev, "%s: cmd pending in the device. tag = %d\n",
				__func__, tag);
			break;
		} else if (!err && resp == UPIU_TASK_MANAGEMENT_FUNC_COMPL) {
			/*
			 * cmd not pending in the device, check if it is
			 * in transition.
			 */
			dev_err(hba->dev, "%s: cmd at tag %d not pending in the device.\n",
				__func__, tag);
#ifdef CONFIG_HISI_UFS_HC_CORE_UTR
			reg = ufshcd_readl(hba, UFS_CORE_UTRLDBR(tag / SLOT_NUM_EACH_CORE));
			if (reg & (1 << (tag % SLOT_NUM_EACH_CORE))) {
#else
			reg = ufshcd_readl(hba, REG_UTP_TRANSFER_REQ_DOOR_BELL);
			if (reg & (1 << tag)) {
#endif
				/* sleep for max. 200us to stabilize */
				usleep_range(100, 200);
				continue;
			}
			/* command completed already */
			dev_err(hba->dev, "%s: cmd at tag %d successfully cleared from DB.\n",
				__func__, tag);
			goto out;
		} else {
			dev_err(hba->dev,
				"%s: no response from device. tag = %d, err %d\n",
				__func__, tag, err);
			if (!err)
				err = resp; /* service response error */
			goto out;
		}
	}

	if (!poll_cnt) {
		err = -EBUSY;
		goto out;
	}

	err = ufshcd_issue_tm_cmd(hba, lrbp->lun, lrbp->task_tag,
			UFS_ABORT_TASK, &resp);
	if (err || resp != UPIU_TASK_MANAGEMENT_FUNC_COMPL) {
		if (!err) {
			err = resp; /* service response error */
			dev_err(hba->dev, "%s: issued. tag = %d, err %d\n",
				__func__, tag, err);
		}
		goto out;
	}

	err = ufshcd_clear_cmd(hba, tag);
	if (err) {
		dev_err(hba->dev, "%s: Failed clearing cmd at tag %d, err %d\n",
			__func__, tag, err);
		goto out;
	}

	scsi_dma_unmap(cmd);

	spin_lock_irqsave(host->host_lock, flags);
	ufshcd_outstanding_req_clear(hba, tag);
	hba->lrb[tag].cmd = NULL;
	spin_unlock_irqrestore(host->host_lock, flags);

	clear_bit_unlock(tag, &hba->lrb_in_use);
	wake_up(&hba->dev_cmd.tag_wq);

out:
	if (!err) {
		err = SUCCESS;
	} else {
		dev_err(hba->dev, "%s: failed with err %d\n", __func__, err);
		ufshcd_set_req_abort_skip(hba, hba->outstanding_reqs);
		err = FAILED;
	}

	return err;
}

/**
 * ufshcd_host_reset_and_restore - reset and restore host controller
 * @hba: per-adapter instance
 *
 * Note that host controller reset may issue DME_RESET to
 * local and remote (device) Uni-Pro stack and the attributes
 * are reset to default state.
 *
 * In Fact, for Kirin SoC, ufshcd_host_reset_and_restore
 * contians a HCE reset and a FULL hardware reset
 *
 * Returns zero on success, non-zero on failure
 */
static int __ufshcd_host_reset_and_restore(struct ufs_hba *hba,
					   bool device_reset)
{
	int err;
	unsigned long flags;

	/* Reset the host controller */
	spin_lock_irqsave(hba->host->host_lock, flags);
	ufshcd_hba_stop(hba, false);
	spin_unlock_irqrestore(hba->host->host_lock, flags);

	if (device_reset)
		ufshcd_vops_device_reset(hba);

	err = ufshcd_hba_enable(hba);
	if (err)
		goto out;

	/* Establish the link again and restore the device */
	err = ufshcd_probe_hba(hba);

out:
	if (err)
		dev_err(hba->dev, "%s: Host init failed %d\n", __func__, err);

	return err;
}

/**
 * ufshcd_reset_and_restore - reset and re-initialize host/device
 * @hba: per-adapter instance
 *
 * Reset and recover device, host and re-establish link. This
 * is helpful to recover the communication in fatal error conditions.
 *
 * Returns zero on success, non-zero on failure
 */
static int ufshcd_reset_and_restore(struct ufs_hba *hba)
{
	int err = 0;
	unsigned long flags;

	hba->ufs_reset_retries = hba->reset_retry_max;
	pm_runtime_get_sync(hba->dev);

	/*
	 * ufs reset takes no affect with sys debug intr, which includes
	 * but not limited with pwm timeout, device timeout, device idle timeout
	 * and so on.
	 * In an error handler, host may stay in pwm for a little while, and
	 * also a doorbell timeout can occur. These intr handler need an another
	 * error handler, which may conflicts with this executting one.
	 */
	ufshcd_disable_dev_tmt_cnt(hba);
	ufshcd_disable_pwm_cnt(hba);
	ufs_idle_intr_toggle(hba, 0);

	if (ufshcd_is_auto_hibern8_allowed(hba))
		ufshcd_disable_auto_hibern8(hba);

	do {
		err = ufshcd_host_reset_and_restore(hba);
#ifdef CONFIG_SCSI_UFS_HS_ERROR_RECOVER
		ufshcd_check_init_mode(hba, err);
#endif
	} while (err && --hba->ufs_reset_retries);

	hba->ufs_reset_retries = hba->reset_retry_max;
	if (ufshcd_is_auto_hibern8_allowed(hba))
		ufshcd_enable_auto_hibern8(hba);

	ufs_idle_intr_toggle(hba, 1);
	ufshcd_enable_pwm_cnt(hba);
	ufshcd_enable_dev_tmt_cnt(hba);

	pm_runtime_put_sync(hba->dev);

	/*
	 * After reset the door-bell might be cleared, complete
	 * outstanding requests in s/w here.
	 */
	spin_lock_irqsave(hba->host->host_lock, flags);
	ufshcd_transfer_req_compl(hba);
	ufshcd_tmc_handler(hba);
	spin_unlock_irqrestore(hba->host->host_lock, flags);

	/*
	 * We retry 7 times, and can not fix err condition. No need keep system
	 * running on, or useful logbuf will flush over.
	 */
	if (err || hba->ufshcd_state == UFSHCD_STATE_ERROR)
		BUG();  /*lint !e146*/

	return err;
}

/**
 * ufshcd_eh_host_reset_handler - host reset handler registered to scsi layer
 * @cmd - SCSI command pointer
 *
 * Returns SUCCESS/FAILED
 */
static int ufshcd_eh_host_reset_handler(struct scsi_cmnd *cmd)
{
	int err;
	unsigned long flags;
	struct ufs_hba *hba;

	hba = shost_priv(cmd->device->host);

	dev_err(hba->dev, "%s occurs\n", __func__);

	/*
	 * Check if there is any race with fatal error handling.
	 * If so, wait for it to complete. Even though fatal error
	 * handling does reset and restore in some cases, don't assume
	 * anything out of it. We are just avoiding race here.
	 */
	do {
		mutex_lock(&hba->eh_mutex);
		spin_lock_irqsave(hba->host->host_lock, flags);
		if (!(hba->force_host_reset ||
			    hba->ufshcd_state == UFSHCD_STATE_RESET ||
			    hba->ufshcd_state == UFSHCD_STATE_EH_SCHEDULED))
			break;
		spin_unlock_irqrestore(hba->host->host_lock, flags);
		mutex_unlock(&hba->eh_mutex);
		dev_err(hba->dev, "%s: reset in progress\n", __func__);
		kthread_flush_work(&hba->eh_work);
	} while (1);

	hba->ufshcd_state = UFSHCD_STATE_RESET;
	ufshcd_set_eh_in_progress(hba);
	spin_unlock_irqrestore(hba->host->host_lock, flags);

	err = ufshcd_reset_and_restore(hba);

	spin_lock_irqsave(hba->host->host_lock, flags);
	if (!err) {
		err = SUCCESS;
		hba->ufshcd_state = UFSHCD_STATE_OPERATIONAL;
	} else {
		err = FAILED;
		hba->ufshcd_state = UFSHCD_STATE_ERROR;
	}
	ufshcd_clear_eh_in_progress(hba);
	spin_unlock_irqrestore(hba->host->host_lock, flags);

	pm_runtime_set_active(hba->dev);
	dev_err(hba->dev, "%s: 0x%x\n", __func__, err);
	mutex_unlock(&hba->eh_mutex);

	return err;
}

/**
 * ufshcd_get_max_icc_level - calculate the ICC level
 * @sup_curr_uA: max. current supported by the regulator
 * @start_scan: row at the desc table to start scan from
 * @buff: power descriptor buffer
 *
 * Returns calculated max ICC level for specific regulator
 */
static u32 ufshcd_get_max_icc_level(int sup_curr_uA, u32 start_scan, const char *buff)
{
	int i;
	int curr_uA;
	u16 data;
	u16 unit;

	for (i = start_scan; i >= 0; i--) {
		data = be16_to_cpup((__be16 *)&buff[2 * i]); /*lint !e679*/
		unit = (data & ATTR_ICC_LVL_UNIT_MASK) >>
						ATTR_ICC_LVL_UNIT_OFFSET;
		curr_uA = data & ATTR_ICC_LVL_VALUE_MASK;
		switch (unit) {
		case UFSHCD_NANO_AMP:
			curr_uA = curr_uA / 1000;
			break;
		case UFSHCD_MILI_AMP:
			curr_uA = curr_uA * 1000;
			break;
		case UFSHCD_AMP:
			curr_uA = curr_uA * 1000 * 1000;
			break;
		case UFSHCD_MICRO_AMP:
		default:
			break;
		}
		if (sup_curr_uA >= curr_uA)
			break;
	}
	if (i < 0) {
		i = 0;
		pr_err("%s: Couldn't find valid icc_level = %d", __func__, i);
	}

	return (u32)i;
}

/**
 * ufshcd_calc_icc_level - calculate the max ICC level
 * In case regulators are not initialized we'll return 0
 * @hba: per-adapter instance
 * @desc_buf: power descriptor buffer to extract ICC levels from.
 * @len: length of desc_buff
 *
 * Returns calculated ICC level
 */
static u32 ufshcd_find_max_sup_active_icc_level(struct ufs_hba *hba,
							u8 *desc_buf, int len)
{
	u32 icc_level = 0;

	if (!hba->vreg_info.vcc || !hba->vreg_info.vccq ||
						!hba->vreg_info.vccq2) {
		dev_err(hba->dev,
			"%s: Regulator capability was not set, actvIccLevel=%d",
							__func__, icc_level);
		goto out;
	}

	if (hba->vreg_info.vcc && hba->vreg_info.vcc->max_ua)
		icc_level = ufshcd_get_max_icc_level(
				hba->vreg_info.vcc->max_ua,
				POWER_DESC_MAX_ACTV_ICC_LVLS - 1,
				&desc_buf[PWR_DESC_ACTIVE_LVLS_VCC_0]);

	if (hba->vreg_info.vccq && hba->vreg_info.vccq->max_ua)
		icc_level = ufshcd_get_max_icc_level(
				hba->vreg_info.vccq->max_ua,
				icc_level,
				&desc_buf[PWR_DESC_ACTIVE_LVLS_VCCQ_0]);

	if (hba->vreg_info.vccq2 && hba->vreg_info.vccq2->max_ua)
		icc_level = ufshcd_get_max_icc_level(
				hba->vreg_info.vccq2->max_ua,
				icc_level,
				&desc_buf[PWR_DESC_ACTIVE_LVLS_VCCQ2_0]);
out:
	return icc_level;
}

static int ufshcd_init_icc_levels(struct ufs_hba *hba)
{
	int ret;
	int buff_len = hba->desc_size.pwr_desc;
	u8 desc_buf[hba->desc_size.pwr_desc];

	ret = ufshcd_read_power_desc(hba, desc_buf, buff_len);
	if (ret) {
		dev_err(hba->dev,
			"%s: Failed reading power descriptor.len = %d ret = %d",
			__func__, buff_len, ret);
		goto out;
	}

	hba->init_prefetch_data.icc_level =
			ufshcd_find_max_sup_active_icc_level(hba,
			desc_buf, buff_len);
	dev_dbg(hba->dev, "%s: setting icc_level 0x%x",
			__func__, hba->init_prefetch_data.icc_level);

	ret = ufshcd_query_attr_retry(hba, UPIU_QUERY_OPCODE_WRITE_ATTR,
		QUERY_ATTR_IDN_ACTIVE_ICC_LVL, 0, 0,
		&hba->init_prefetch_data.icc_level);

	if (ret)
		dev_err(hba->dev,
			"%s: Failed configuring bActiveICCLevel = %d ret = %d",
			__func__, hba->init_prefetch_data.icc_level , ret);

out:
	return ret;
}

/**
 * ufshcd_scsi_add_wlus - Adds required W-LUs
 * @hba: per-adapter instance
 *
 * UFS device specification requires the UFS devices to support 4 well known
 * logical units:
 *	"REPORT_LUNS" (address: 01h)
 *	"UFS Device" (address: 50h)
 *	"RPMB" (address: 44h)
 *	"BOOT" (address: 30h)
 * UFS device's power management needs to be controlled by "POWER CONDITION"
 * field of SSU (START STOP UNIT) command. But this "power condition" field
 * will take effect only when its sent to "UFS device" well known logical unit
 * hence we require the scsi_device instance to represent this logical unit in
 * order for the UFS host driver to send the SSU command for power management.

 * We also require the scsi_device instance for "RPMB" (Replay Protected Memory
 * Block) LU so user space process can control this LU. User space may also
 * want to have access to BOOT LU.

 * This function adds scsi device instances for each of all well known LUs
 * (except "REPORT LUNS" LU).
 *
 * Returns zero on success (all required W-LUs are added successfully),
 * non-zero error value on failure (if failed to add any of the required W-LU).
 */
int ufshcd_scsi_add_wlus(struct ufs_hba *hba)
{
	int ret = 0;
	struct scsi_device *sdev_rpmb;
	struct scsi_device *sdev_boot;

	hba->sdev_ufs_device = __scsi_add_device(hba->host, 0, 0,
		ufshcd_upiu_wlun_to_scsi_wlun(UFS_UPIU_UFS_DEVICE_WLUN), NULL);
	if (IS_ERR(hba->sdev_ufs_device)) {
		ret = PTR_ERR(hba->sdev_ufs_device);
		hba->sdev_ufs_device = NULL;
		goto out;
	}
	scsi_device_put(hba->sdev_ufs_device);

	if (likely(!hba->host->is_emulator)) {
		sdev_boot = __scsi_add_device(hba->host, 0, 0,
								ufshcd_upiu_wlun_to_scsi_wlun
								(UFS_UPIU_BOOT_WLUN), NULL);
		if (IS_ERR(sdev_boot)) {
			ret = PTR_ERR(sdev_boot);
			goto remove_sdev_ufs_device;
		}
		scsi_device_put(sdev_boot);

		sdev_rpmb = __scsi_add_device(hba->host, 0, 0,
								ufshcd_upiu_wlun_to_scsi_wlun
								(UFS_UPIU_RPMB_WLUN), NULL);
		if (IS_ERR(sdev_rpmb)) {
			ret = PTR_ERR(sdev_rpmb);
			goto remove_sdev_boot;
		}
		scsi_device_put(sdev_rpmb);
	}
	goto out;

remove_sdev_boot:
	scsi_remove_device(sdev_boot);
remove_sdev_ufs_device:
	scsi_remove_device(hba->sdev_ufs_device);
out:
	return ret;
}

static int ufs_get_device_desc(struct ufs_hba *hba,
			       struct ufs_dev_desc *dev_desc)
{
	int err;
	size_t buff_len;
	u8 model_index;
	u8 *desc_buf = NULL;

	buff_len = max_t(size_t, hba->desc_size.dev_desc,
			 QUERY_DESC_MAX_SIZE + 1);
	desc_buf = kzalloc(buff_len, GFP_KERNEL);
	if (!desc_buf)
		return -ENOMEM;

	err = ufshcd_read_device_desc(hba, desc_buf, hba->desc_size.dev_desc);
	if (err)
		goto out;

	/*
	 * getting vendor (manufacturerID) and Bank Index in big endian
	 * format
	 */
	dev_desc->wmanufacturerid = desc_buf[DEVICE_DESC_PARAM_MANF_ID] << 8 |
				     desc_buf[DEVICE_DESC_PARAM_MANF_ID + 1];
	model_index = desc_buf[DEVICE_DESC_PARAM_PRDCT_NAME];
	hisi_ufs_parse_dev_desc(hba, dev_desc, desc_buf);

	/* Zero-pad entire buffer for string termination. */
	memset(desc_buf, 0, buff_len);

	err = ufshcd_read_string_desc(hba, model_index, desc_buf,
				      QUERY_DESC_STRING_MAX_SIZE, ASCII_STD);
	if (err)
		goto out;

	desc_buf[QUERY_DESC_MAX_SIZE] = '\0';
	strlcpy(dev_desc->model, (desc_buf + QUERY_DESC_HDR_SIZE),
		min_t(uint8_t, desc_buf[QUERY_DESC_LENGTH_OFFSET],
		      sizeof(dev_desc->model)));
	/* Null terminate the model string */
	dev_desc->model[MAX_MODEL_LEN] = '\0';

	err = ufshcd_vops_get_device_info(hba, dev_desc, desc_buf);
	if (err)
		pr_err("%s err %d\n", __func__, err);
out:
	kfree(desc_buf);
	return err;
}

static void ufs_fixup_device_setup(struct ufs_hba *hba,
				   struct ufs_dev_desc *dev_desc)
{
	struct ufs_dev_fix *f;

	for (f = ufs_fixups; f->quirk; f++) { /*lint !e661*/
		if ((f->card.wmanufacturerid == dev_desc->wmanufacturerid ||
		     f->card.wmanufacturerid == UFS_ANY_VENDOR) &&
		    (STR_PRFX_EQUAL(f->card.model, dev_desc->model) ||
		     !strcmp(f->card.model, UFS_ANY_MODEL)))
			hba->dev_quirks |= f->quirk;
	}
}

/**
 * ufshcd_tune_pa_tactivate - Tunes PA_TActivate of local UniPro
 * @hba: per-adapter instance
 *
 * PA_TActivate parameter can be tuned manually if UniPro version is less than
 * 1.61. PA_TActivate needs to be greater than or equal to peerM-PHY's
 * RX_MIN_ACTIVATETIME_CAPABILITY attribute. This optimal value can help reduce
 * the hibern8 exit latency.
 *
 * Returns zero on success, non-zero error value on failure.
 */
static int ufshcd_tune_pa_tactivate(struct ufs_hba *hba)
{
	int ret = 0;
	u32 peer_rx_min_activatetime = 0, tuned_pa_tactivate;

	ret = ufshcd_dme_peer_get(hba,
				  UIC_ARG_MIB_SEL(
					RX_MIN_ACTIVATETIME_CAPABILITY,
					UIC_ARG_MPHY_RX_GEN_SEL_INDEX(0)),
				  &peer_rx_min_activatetime);
	if (ret)
		goto out;

	/* make sure proper unit conversion is applied */
	tuned_pa_tactivate =
		((peer_rx_min_activatetime * RX_MIN_ACTIVATETIME_UNIT_US)
		 / PA_TACTIVATE_TIME_UNIT_US);
	ret = ufshcd_dme_set(hba, UIC_ARG_MIB(PA_TACTIVATE),
			     tuned_pa_tactivate);

out:
	return ret;
}

/**
 * ufshcd_tune_pa_hibern8time - Tunes PA_Hibern8Time of local UniPro
 * @hba: per-adapter instance
 *
 * PA_Hibern8Time parameter can be tuned manually if UniPro version is less than
 * 1.61. PA_Hibern8Time needs to be maximum of local M-PHY's
 * TX_HIBERN8TIME_CAPABILITY & peer M-PHY's RX_HIBERN8TIME_CAPABILITY.
 * This optimal value can help reduce the hibern8 exit latency.
 *
 * Returns zero on success, non-zero error value on failure.
 */
static int ufshcd_tune_pa_hibern8time(struct ufs_hba *hba)
{
	int ret = 0;
	u32 local_tx_hibern8_time_cap = 0, peer_rx_hibern8_time_cap = 0;
	u32 max_hibern8_time, tuned_pa_hibern8time;

	ret = ufshcd_dme_get(hba,
			     UIC_ARG_MIB_SEL(TX_HIBERN8TIME_CAPABILITY,
					UIC_ARG_MPHY_TX_GEN_SEL_INDEX(0)),
				  &local_tx_hibern8_time_cap);
	if (ret)
		goto out;

	ret = ufshcd_dme_peer_get(hba,
				  UIC_ARG_MIB_SEL(RX_HIBERN8TIME_CAPABILITY,
					UIC_ARG_MPHY_RX_GEN_SEL_INDEX(0)),
				  &peer_rx_hibern8_time_cap);
	if (ret)
		goto out;

	max_hibern8_time = max(local_tx_hibern8_time_cap,
			       peer_rx_hibern8_time_cap);
	/* make sure proper unit conversion is applied */
	tuned_pa_hibern8time = ((max_hibern8_time * HIBERN8TIME_UNIT_US)
				/ PA_HIBERN8_TIME_UNIT_US);
	ret = ufshcd_dme_set(hba, UIC_ARG_MIB(PA_HIBERN8TIME),
			     tuned_pa_hibern8time);
out:
	return ret;
}

/**
 * ufshcd_quirk_tune_host_pa_tactivate - Ensures that host PA_TACTIVATE is
 * less than device PA_TACTIVATE time.
 * @hba: per-adapter instance
 *
 * Some UFS devices require host PA_TACTIVATE to be lower than device
 * PA_TACTIVATE, we need to enable UFS_DEVICE_QUIRK_HOST_PA_TACTIVATE quirk
 * for such devices.
 *
 * Returns zero on success, non-zero error value on failure.
 */
static int ufshcd_quirk_tune_host_pa_tactivate(struct ufs_hba *hba)
{
	int ret = 0;
	u32 granularity = 0;
	u32 peer_granularity = 0;
	u32 pa_tactivate = 0;
	u32 peer_pa_tactivate = 0;
	u32 pa_tactivate_us, peer_pa_tactivate_us;
	u8 gran_to_us_table[] = {1, 4, 8, 16, 32, 100};

	ret = ufshcd_dme_get(hba, UIC_ARG_MIB(PA_GRANULARITY),
				  &granularity);
	if (ret)
		goto out;

	ret = ufshcd_dme_peer_get(hba, UIC_ARG_MIB(PA_GRANULARITY),
				  &peer_granularity);
	if (ret)
		goto out;

	if ((granularity < PA_GRANULARITY_MIN_VAL) ||
	    (granularity > PA_GRANULARITY_MAX_VAL)) {
		dev_err(hba->dev, "%s: invalid host PA_GRANULARITY %d",
			__func__, granularity);
		return -EINVAL;
	}

	if ((peer_granularity < PA_GRANULARITY_MIN_VAL) ||
	    (peer_granularity > PA_GRANULARITY_MAX_VAL)) {
		dev_err(hba->dev, "%s: invalid device PA_GRANULARITY %d",
			__func__, peer_granularity);
		return -EINVAL;
	}

	ret = ufshcd_dme_get(hba, UIC_ARG_MIB(PA_TACTIVATE), &pa_tactivate);
	if (ret)
		goto out;

	ret = ufshcd_dme_peer_get(hba, UIC_ARG_MIB(PA_TACTIVATE),
				  &peer_pa_tactivate);
	if (ret)
		goto out;

	pa_tactivate_us = pa_tactivate * gran_to_us_table[granularity - 1];
	peer_pa_tactivate_us = peer_pa_tactivate *
			     gran_to_us_table[peer_granularity - 1];

	if (pa_tactivate_us > peer_pa_tactivate_us) {
		u32 new_peer_pa_tactivate;

		new_peer_pa_tactivate = pa_tactivate_us /
				      gran_to_us_table[peer_granularity - 1];
		new_peer_pa_tactivate++;
		ret = ufshcd_dme_peer_set(hba, UIC_ARG_MIB(PA_TACTIVATE),
					  new_peer_pa_tactivate);
	}

out:
	return ret;
}

static void ufshcd_tune_unipro_params(struct ufs_hba *hba)
{
	if (ufshcd_is_unipro_pa_params_tuning_req(hba)) {
		ufshcd_tune_pa_tactivate(hba);
		ufshcd_tune_pa_hibern8time(hba);
	}

	if (hba->dev_quirks & UFS_DEVICE_QUIRK_PA_TACTIVATE)
		/* set 1ms timeout for PA_TACTIVATE */
		ufshcd_dme_set(hba, UIC_ARG_MIB(PA_TACTIVATE), 10);

	if (hba->dev_quirks & UFS_DEVICE_QUIRK_HOST_PA_TACTIVATE)
		ufshcd_quirk_tune_host_pa_tactivate(hba);

	ufshcd_vops_apply_dev_quirks(hba);
}

static void ufshcd_clear_dbg_ufs_stats(struct ufs_hba *hba)
{
	int err_reg_hist_size = sizeof(struct ufs_uic_err_reg_hist);

	hba->ufs_stats.hibern8_exit_cnt = 0;
	hba->ufs_stats.last_hibern8_exit_tstamp = ktime_set(0, 0);

	memset(&hba->ufs_stats.pa_err, 0, err_reg_hist_size);
	memset(&hba->ufs_stats.dl_err, 0, err_reg_hist_size);
	memset(&hba->ufs_stats.nl_err, 0, err_reg_hist_size);
	memset(&hba->ufs_stats.tl_err, 0, err_reg_hist_size);
	memset(&hba->ufs_stats.dme_err, 0, err_reg_hist_size);

	hba->req_abort_count = 0;
}

static void ufshcd_init_desc_sizes(struct ufs_hba *hba)
{
	int err;

	err = ufshcd_read_desc_length(hba, QUERY_DESC_IDN_DEVICE, 0,
		&hba->desc_size.dev_desc);
	if (err)
		hba->desc_size.dev_desc = QUERY_DESC_DEVICE_DEF_SIZE;

	err = ufshcd_read_desc_length(hba, QUERY_DESC_IDN_POWER, 0,
		&hba->desc_size.pwr_desc);
	if (err)
		hba->desc_size.pwr_desc = QUERY_DESC_POWER_DEF_SIZE;

	err = ufshcd_read_desc_length(hba, QUERY_DESC_IDN_INTERCONNECT, 0,
		&hba->desc_size.interc_desc);
	if (err)
		hba->desc_size.interc_desc = QUERY_DESC_INTERCONNECT_DEF_SIZE;

	err = ufshcd_read_desc_length(hba, QUERY_DESC_IDN_CONFIGURATION, 0,
		&hba->desc_size.conf_desc);
	if (err)
		hba->desc_size.conf_desc = QUERY_DESC_CONFIGURATION_DEF_SIZE;

	err = ufshcd_read_desc_length(hba, QUERY_DESC_IDN_UNIT, 0,
		&hba->desc_size.unit_desc);
	if (err)
		hba->desc_size.unit_desc = QUERY_DESC_UNIT_DEF_SIZE;

	err = ufshcd_read_desc_length(hba, QUERY_DESC_IDN_GEOMETRY, 0,
		&hba->desc_size.geom_desc);
	if (err)
		hba->desc_size.geom_desc = QUERY_DESC_GEOMETRY_DEF_SIZE;
}

static void ufshcd_def_desc_sizes(struct ufs_hba *hba)
{
	hba->desc_size.dev_desc = QUERY_DESC_DEVICE_DEF_SIZE;
	hba->desc_size.pwr_desc = QUERY_DESC_POWER_DEF_SIZE;
	hba->desc_size.interc_desc = QUERY_DESC_INTERCONNECT_DEF_SIZE;
	hba->desc_size.conf_desc = QUERY_DESC_CONFIGURATION_DEF_SIZE;
	hba->desc_size.unit_desc = QUERY_DESC_UNIT_DEF_SIZE;
	hba->desc_size.geom_desc = QUERY_DESC_GEOMETRY_DEF_SIZE;
}

/**
 * ufshcd_probe_hba - probe hba to detect device and initialize
 * @hba: per-adapter instance
 *
 * Execute link-startup and verify device initialization
 */
static int ufshcd_probe_hba(struct ufs_hba *hba)
{
	struct ufs_dev_desc card = {0};
	int ret;
	ktime_t start = ktime_get();
#ifdef CONFIG_HUAWEI_UFS_DSM
	u32 avail_lane_rx, avail_lane_tx;
#endif
	unsigned long flags;

	if(dsm_ufs_disable_uic_err()){
		dev_err(hba->dev, "dsm uic disable bit already be set.\n");
	};
	ret = ufshcd_link_startup(hba);
	if (ret)
		goto out;

	ufshcd_init_pwr_info(hba);
	/* set the default level for urgent bkops */
	hba->urgent_bkops_lvl = BKOPS_STATUS_PERF_IMPACT;
	hba->is_urgent_bkops_lvl_checked = false;

	/* Debug counters initialization */
	ufshcd_clear_dbg_ufs_stats(hba);

	/* UniPro link is active now */
	ufshcd_set_link_active(hba);
	hba->is_hibernate = false;

	ret = ufshcd_verify_dev_init(hba, NOP_OUT_RETRIES);
	if (ret)
		goto out;


	ret = ufshcd_complete_dev_init(hba);

	if (ret)
		goto out;

	/* Init check for device descriptor sizes */
	ufshcd_init_desc_sizes(hba);

	ret = ufs_get_device_desc(hba, &card);
	if (ret) {
		dev_err(hba->dev, "%s: Failed getting device info. err = %d\n",
			__func__, ret);
		goto out;
	}
	/* HI1861 not support autoH8 currently */
	ufshcd_device_not_support_autoh8(hba);

	ufs_get_geometry_info(hba);
	if (likely(!hba->host->is_emulator))
		ufs_get_device_health_info(hba);

	ufs_fixup_device_setup(hba, &card);
	ufshcd_tune_unipro_params(hba);

	ret = ufshcd_set_vccq_rail_unused(hba,
		(hba->dev_quirks & UFS_DEVICE_NO_VCCQ) ? true : false);
	if (ret)
		goto out;

#ifdef CONFIG_HISI_AB_PARTITION
	ufs_get_boot_partition_type(hba);
#endif
	/* UFS device is also active now */
	ufshcd_set_ufs_dev_active(hba);
#ifdef FEATURE_UFS_AUTO_BKOPS
	ufshcd_force_reset_auto_bkops(hba);
#endif

	/*
	 * Background operations need to be enabled for Sandisk ufs device.
	 * Hynix use manual bkops. And others are enabled by default.
	 */
	ufshcd_device_need_enable_autobkops(hba);

#ifdef CONFIG_HISI_UFS_HC
	if (ufshcd_is_hisi_ufs_hc(hba))
		ufshcd_enable_clock_gating_autodiv(hba);
#endif

#ifdef CONFIG_HISI_BOOTDEVICE
	if (get_bootdevice_type() == BOOT_DEVICE_UFS) {
		set_bootdevice_rev_handler(ufshcd_device_get_rev);
		ufs_get_rpmb_info(hba);
		dev_err(hba->dev, "---------ufs rpmb info----------\n");
		dev_err(hba->dev, "rpmb_region_enable:0x%x\n",get_rpmb_region_enable());
		dev_err(hba->dev, "rpmb_total_blks:%llu\n",get_rpmb_total_blks());
	}
#endif

	hba->wlun_dev_clr_ua = true;

	if (ufshcd_get_max_pwr_mode(hba)) {
		dev_err(hba->dev,
			"%s: Failed getting max supported power mode\n",
			__func__);
	} else {
		ret = ufshcd_config_pwr_mode(hba, &hba->max_pwr_info.info);
		if (ret) {
			dev_err(hba->dev, "%s: Failed setting power mode, err = %d\n",
					__func__, ret);
			goto out;
		}
	}

	/* clean ufs ue intr */
	ufshcd_ue_clean(hba);

	/* confirm ufs works well after switch to hs mode */
	ret = ufshcd_verify_dev_init(hba, 1);
	if (ret)
		goto out;
#ifdef CONFIG_HISI_UFS_HC
	ufshcd_ue_clean(hba);
#endif

#ifdef CONFIG_SCSI_UFS_LUN_PROTECT
	ret = ufshcd_set_lun_protect(hba);
	if (ret)
		goto out;
#endif

#ifdef CONFIG_HUAWEI_UFS_DSM
	ufshcd_dme_peer_get(hba, UIC_ARG_MIB(PA_AVAILRXDATALANES),
			&avail_lane_rx);
	ufshcd_dme_peer_get(hba, UIC_ARG_MIB(PA_AVAILTXDATALANES),
			&avail_lane_tx);
	if ((hba->max_pwr_info.info.lane_rx < avail_lane_rx) ||
		(hba->max_pwr_info.info.lane_tx < avail_lane_tx)) {
		dev_err(hba->dev, "ufs line number is less than avail "
			"rx=%d, tx=%d, avail_rx=%d, avail_tx=%d\n",
			hba->max_pwr_info.info.lane_rx,
			hba->max_pwr_info.info.lane_tx,
			avail_lane_rx, avail_lane_tx);
		spin_lock_irqsave(hba->host->host_lock, flags);
		dsm_ufs_update_error_info(hba, DSM_UFS_LINKUP_ERR);
		spin_unlock_irqrestore(hba->host->host_lock, flags);
		schedule_ufs_dsm_work(hba);
	}
#endif

#if defined(CONFIG_SCSI_UFS_HI1861_VCMD) && defined(CONFIG_HISI_DIEID)
	ufshcd_ufs_set_dieid(hba, &card);
#endif

#ifdef CONFIG_SCSI_UFS_HI1861_VCMD
	ufshcd_device_capbitlity_config(hba);
#endif

	ret = ufshcd_wb_config(hba);
	if (ret)
		goto out;

	hba->force_host_reset = false;
	/* set the state as operational after switching to desired gear */
	hba->ufshcd_state = UFSHCD_STATE_OPERATIONAL;

#ifdef CONFIG_SCSI_UFS_KIRIN_LINERESET_CHECK
	if (hba->bg_task_enable && hba->vops && hba->vops->background_thread) {
		if (!hba->background_task) {
			hba->background_task = kthread_run(hba->vops->background_thread,
				hba, "ufs_bg_thread");
			if (IS_ERR(hba->background_task))
				dev_err(hba->dev, "background_thread create fail! \r\n", __func__);
		}
	}
#endif

	/*
	 * If we are in error handling context or in power management callbacks
	 * context, no need to scan the host
	 */
	if (!ufshcd_eh_in_progress(hba) && !hba->pm_op_in_progress) {
		bool flag;

		/* clear any previous UFS device information */
		memset(&hba->dev_info, 0, sizeof(hba->dev_info));
		if (!ufshcd_query_flag_retry(hba, UPIU_QUERY_OPCODE_READ_FLAG,
				QUERY_FLAG_IDN_PWR_ON_WPE, &flag))
			hba->dev_info.f_power_on_wp_en = flag;

		if (!hba->host->is_emulator && !hba->is_init_prefetch) {
			ret = ufshcd_init_icc_levels(hba);
			if (ret)
				goto out;
		}

		/* Add required well known logical units to scsi mid layer */
		ret = ufshcd_scsi_add_wlus(hba);
		if (ret)
			goto out;

		/* Initialize devfreq after UFS device is detected */
		if (ufshcd_is_clkscaling_supported(hba)) {
			memcpy(&hba->clk_scaling.saved_pwr_info.info,
				&hba->pwr_info,
				sizeof(struct ufs_pa_layer_attr));
			hba->clk_scaling.saved_pwr_info.is_valid = true;
			if (!hba->devfreq) {
				hba->devfreq = devm_devfreq_add_device(hba->dev,
							&ufs_devfreq_profile,
							"simple_ondemand",
							NULL);
				if (IS_ERR(hba->devfreq)) {
					ret = PTR_ERR(hba->devfreq);
					dev_err(hba->dev, "Unable to register with devfreq %d\n",
							ret);
					goto out;
				}
			}
			hba->clk_scaling.is_allowed = true;
		}
		/* get ufs ue intr status */
		ret = ufshcd_ue_get(hba);
		if (ret)
			goto out;


		scsi_scan_host(hba->host);
		pm_runtime_put_sync(hba->dev);

		if (ufshcd_is_auto_hibern8_allowed(hba))
			ufshcd_enable_auto_hibern8(hba);

		if (hba->caps & UFSHCD_CAP_PWM_DAEMON_INTR)
			ufshcd_enable_pwm_cnt(hba);

		if (hba->caps & UFSHCD_CAP_DEV_TMT_INTR)
			ufshcd_enable_dev_tmt_cnt(hba);
	} else {
		/* get ufs ue intr status */
		ret = ufshcd_ue_get(hba);
		if (ret)
			goto out;
	}

	dsm_ufs_enable_uic_err(hba);

	if (!hba->is_init_prefetch)
		hba->is_init_prefetch = true;

	ufs_idle_intr_toggle(hba, 1);

	if (feature_open(hba) && ufshpb_support(hba) && is_hpb_opened_card(hba))
		(void)hpb_init(hba);
out:
	trace_ufshcd_init(dev_name(hba->dev), ret,
			  ktime_to_us(ktime_sub(ktime_get(), start)),
			  hba->curr_dev_pwr_mode, hba->uic_link_state);
	return ret;
}

static enum blk_eh_timer_return ufshcd_eh_timed_out(struct scsi_cmnd *scmd)
{
	unsigned long flags;
	struct Scsi_Host *host;
	struct ufs_hba *hba;
	int index;
	bool found = false;
	int tag = -1;

	if (!scmd || !scmd->device || !scmd->device->host)
		return BLK_EH_NOT_HANDLED;

	host = scmd->device->host;
	hba = shost_priv(host);
	if (!hba)
		return BLK_EH_NOT_HANDLED;

	spin_lock_irqsave(host->host_lock, flags);

	for_each_set_bit(index, &hba->outstanding_reqs, hba->nutrs) {
		if (hba->lrb[index].cmd == scmd) {
			found = true;
			tag = index;
			break;
		}
	}
	spin_unlock_irqrestore(host->host_lock, flags);

	if( true == found ) {
		dsm_ufs_update_upiu_info(hba, tag, DSM_UFS_TIMEOUT_ERR);

		/*
		* Report DSM_UFS_TIMEOUT_SERIOUS when doorbell timeout has
		* been recovered by ufshcd_host_reset_and_restore over
		* UFS_TIMEOUT_SERIOUS_THRESHOLD times. ufs_timeout_count
		* will not increase if ufshcd_host_reset_and_restore not
		* called.
		*/
		if (ufs_timeout_lock != 1) {
			ufs_timeout_lock = 1;
			ufs_timeout_count++;
		}
		if (ufs_timeout_count > UFS_TIMEOUT_SERIOUS_THRESHOLD)
			dsm_ufs_update_upiu_info(hba, tag, DSM_UFS_TIMEOUT_SERIOUS);

		schedule_ufs_dsm_work(hba);
	} else {
		dev_warn( hba->dev,\
			"scsi cmd[%x] with tag[%x] is timeout which can't be found.",
			scmd->cmnd[0], scmd->request->tag );
	}
	/*
	 * Bypass SCSI error handling and reset the block layer timer if this
	 * SCSI command was not actually dispatched to UFS driver, otherwise
	 * let SCSI layer handle the error as usual.
	 */
	return (enum blk_eh_timer_return)(found ? BLK_EH_NOT_HANDLED : BLK_EH_RESET_TIMER);
}

#ifdef CONFIG_HISI_BLOCK_ORDER_PRESERVING
static int ufshcd_direct_req_sense(struct scsi_device *sdp, unsigned int timeout, bool eh_handle);
#endif

static struct scsi_host_template ufshcd_driver_template = {
	.module			= THIS_MODULE,
	.name			= UFSHCD,
	.proc_name		= UFSHCD,
	.queuecommand		= ufshcd_queuecommand,
	.slave_alloc		= ufshcd_slave_alloc,
	.slave_configure	= ufshcd_slave_configure,
	.slave_destroy		= ufshcd_slave_destroy,
	.change_queue_depth	= ufshcd_change_queue_depth,
	.eh_abort_handler	= ufshcd_abort,
	.eh_device_reset_handler = ufshcd_eh_device_reset_handler,
	.eh_host_reset_handler   = ufshcd_eh_host_reset_handler,
	.eh_timed_out		= ufshcd_eh_timed_out,
#ifdef CONFIG_HISI_BLK
	.dump_status		= ufshcd_dump_status,
#ifdef CONFIG_HYPERHOLD_CORE
	.get_health_info	= ufshcd_get_health_info,
#endif
	.direct_flush		= ufshcd_direct_flush,
#ifdef CONFIG_HISI_BLOCK_ORDER_PRESERVING
	.send_request_sense_directly = ufshcd_direct_req_sense,
#endif
#endif
	.this_id		= -1,
#ifdef CONFIG_SCSI_UFS_CUST_MAX_SECTORS
	.max_sectors		= 4096,
	.sg_tablesize		= UFS_SG_MAX_COUNT,
#else
	.sg_tablesize		= SG_ALL,
#endif
	.cmd_per_lun		= UFSHCD_CMD_PER_LUN,
	.can_queue		= UFSHCD_CAN_QUEUE,
	.max_host_blocked	= 1,
	.shost_attrs            = ufs_host_attrs,
	.sdev_attrs		= ufs_device_attrs,
	.track_queue_depth	= 1,
	.skip_settle_delay	= 1,
};

static int ufshcd_config_vreg_load(struct device *dev, struct ufs_vreg *vreg,
				   int ua)
{
	int ret;

	if (!vreg)
		return 0;

	/*
	 * "set_load" operation shall be required on those regulators
	 * which specifically configured current limitation. Otherwise
	 * zero max_ua may cause unexpected behavior when regulator is
	 * enabled or set as high power mode.
	 */
	if (!vreg->max_ua)
		return 0;

	ret = regulator_set_load(vreg->reg, ua);
	if (ret < 0) {
		dev_err(dev, "%s: %s set load (ua=%d) failed, err=%d\n",
				__func__, vreg->name, ua, ret);
	}

	return ret;
}

static inline int ufshcd_config_vreg_lpm(struct ufs_hba *hba,
					 struct ufs_vreg *vreg)
{
	if (!vreg)
		return 0;
	else if (vreg->unused)
		return 0;
	else
		return ufshcd_config_vreg_load(hba->dev, vreg,
					       UFS_VREG_LPM_LOAD_UA);
}

static inline int ufshcd_config_vreg_hpm(struct ufs_hba *hba,
					 struct ufs_vreg *vreg)
{
	if (!vreg)
		return 0;
	else if (vreg->unused)
		return 0;
	else
		return ufshcd_config_vreg_load(hba->dev, vreg, vreg->max_ua);
}

static int ufshcd_config_vreg(struct device *dev,
		struct ufs_vreg *vreg, bool on)
{
	int ret = 0;
	struct regulator *reg = NULL;
	const char *name = NULL;
	int min_uv, uA_load;

	BUG_ON(!vreg);

	reg = vreg->reg;
	name = vreg->name;

	if (regulator_count_voltages(reg) > 0) {
		if (vreg->min_uv && vreg->max_uv) {
			min_uv = on ? vreg->min_uv : 0;
			ret = regulator_set_voltage(reg, min_uv, vreg->max_uv);
			if (ret) {
				dev_err(dev,
					"%s: %s set voltage failed, err=%d\n",
					__func__, name, ret);
				goto out;
			}
		}

		uA_load = on ? vreg->max_ua : 0;
		ret = ufshcd_config_vreg_load(dev, vreg, uA_load);
		if (ret)
			goto out;
	}
out:
	return ret;
}

static int ufshcd_enable_vreg(struct device *dev, struct ufs_vreg *vreg)
{
	int ret = 0;

	if (!vreg)
		goto out;
	else if (vreg->enabled || vreg->unused)
		goto out;

	ret = ufshcd_config_vreg(dev, vreg, true);
	if (!ret)
		ret = regulator_enable(vreg->reg);

	if (!ret)
		vreg->enabled = true;
	else
		dev_err(dev, "%s: %s enable failed, err=%d\n",
				__func__, vreg->name, ret);
out:
	return ret;
}

static int ufshcd_disable_vreg(struct device *dev, struct ufs_vreg *vreg)
{
	int ret = 0;

	if (!vreg)
		goto out;
	else if (!vreg->enabled || vreg->unused)
		goto out;

	ret = regulator_disable(vreg->reg);

	if (!ret) {
		/* ignore errors on applying disable config */
		ufshcd_config_vreg(dev, vreg, false);
		vreg->enabled = false;
	} else {
		dev_err(dev, "%s: %s disable failed, err=%d\n",
				__func__, vreg->name, ret);
	}
out:
	return ret;
}

static int ufshcd_setup_vreg(struct ufs_hba *hba, bool on)
{
	int ret = 0;
	struct device *dev = hba->dev;
	struct ufs_vreg_info *info = &hba->vreg_info;

	ret = ufshcd_toggle_vreg(dev, info->vcc, on);
	if (ret)
		goto out;

	ret = ufshcd_toggle_vreg(dev, info->vccq, on);
	if (ret)
		goto out;

	ret = ufshcd_toggle_vreg(dev, info->vccq2, on);
	if (ret)
		goto out;

out:
	if (ret) {
		ufshcd_toggle_vreg(dev, info->vccq2, false);
		ufshcd_toggle_vreg(dev, info->vccq, false);
		ufshcd_toggle_vreg(dev, info->vcc, false);
	}
	return ret;
}

static int ufshcd_setup_hba_vreg(struct ufs_hba *hba, bool on)
{
	struct ufs_vreg_info *info = &hba->vreg_info;

	return ufshcd_toggle_vreg(hba->dev, info->vdd_hba, on);
}

static int ufshcd_get_vreg(struct device *dev, struct ufs_vreg *vreg)
{
	int ret = 0;

	if (!vreg)
		goto out;

	vreg->reg = devm_regulator_get(dev, vreg->name);
	if (IS_ERR(vreg->reg)) {
		ret = PTR_ERR(vreg->reg);
		dev_err(dev, "%s: %s get failed, err=%d\n",
				__func__, vreg->name, ret);
	}
out:
	return ret;
}

static int ufshcd_init_vreg(struct ufs_hba *hba)
{
	int ret = 0;
	struct device *dev = hba->dev;
	struct ufs_vreg_info *info = &hba->vreg_info;

	ret = ufshcd_get_vreg(dev, info->vcc);
	if (ret)
		goto out;

	ret = ufshcd_get_vreg(dev, info->vccq);
	if (ret)
		goto out;

	ret = ufshcd_get_vreg(dev, info->vccq2);
out:
	return ret;
}

static int ufshcd_init_hba_vreg(struct ufs_hba *hba)
{
	struct ufs_vreg_info *info = &hba->vreg_info;

	if (info)
		return ufshcd_get_vreg(hba->dev, info->vdd_hba);

	return 0;
}

static int ufshcd_set_vccq_rail_unused(struct ufs_hba *hba, bool unused)
{
	int ret = 0;
	struct ufs_vreg_info *info = &hba->vreg_info;

	if (!info->vccq)
		goto out;

	if (unused) {
		/* shut off the rail here */
		ret = ufshcd_toggle_vreg(hba->dev, info->vccq, false);
		/*
		 * Mark this rail as no longer used, so it doesn't get enabled
		 * later by mistake
		 */
		if (!ret)
			info->vccq->unused = true;
	} else {
		/*
		 * rail should have been already enabled hence just make sure
		 * that unused flag is cleared.
		 */
		info->vccq->unused = false;
	}
out:
	return ret;
}

static int __ufshcd_setup_clocks(struct ufs_hba *hba, bool on,
					bool skip_ref_clk)
{
	int ret = 0;
	struct ufs_clk_info *clki = NULL;
	struct list_head *head = &hba->clk_list_head;
	unsigned long flags;
	ktime_t start = ktime_get();
	bool clk_state_changed = false;

	if (list_empty(head))
		goto out;

	/*
	 * vendor specific setup_clocks ops may depend on clocks managed by
	 * this standard driver hence call the vendor specific setup_clocks
	 * before disabling the clocks managed here.
	 */
	if (!on) {
		ret = ufshcd_vops_setup_clocks(hba, on, PRE_CHANGE);
		if (ret)
			return ret;
	}

	list_for_each_entry(clki, head, list) {
		if (!IS_ERR_OR_NULL(clki->clk)) {
			if (skip_ref_clk && !strcmp(clki->name, "ref_clk"))
				continue;

			clk_state_changed = on ^ clki->enabled; /*lint !e514*/
			if (on && !clki->enabled) {
				ret = clk_prepare_enable(clki->clk);
				if (ret) {
					dev_err(hba->dev, "%s: %s prepare enable failed, %d\n",
						__func__, clki->name, ret);
					goto out;
				}
			} else if (!on && clki->enabled) {
				clk_disable_unprepare(clki->clk);
			}
			clki->enabled = on;
			dev_dbg(hba->dev, "%s: clk: %s %sabled\n", __func__,
					clki->name, on ? "en" : "dis");
		}
	}

	/*
	 * vendor specific setup_clocks ops may depend on clocks managed by
	 * this standard driver hence call the vendor specific setup_clocks
	 * after enabling the clocks managed here.
	 */
	if (on) {
		ret = ufshcd_vops_setup_clocks(hba, on, POST_CHANGE);
		if (ret)
			return ret;
	}

out:
	if (ret) {
		list_for_each_entry(clki, head, list) {
			if (!IS_ERR_OR_NULL(clki->clk) && clki->enabled)
				clk_disable_unprepare(clki->clk);
		}
	} else if (!ret && on) {
		spin_lock_irqsave(hba->host->host_lock, flags);
		hba->clk_gating.state = CLKS_ON;
		trace_ufshcd_clk_gating(dev_name(hba->dev),
					hba->clk_gating.state);
		spin_unlock_irqrestore(hba->host->host_lock, flags);
	}

	if (clk_state_changed)
        trace_ufshcd_profile_clk_gating(dev_name(hba->dev),
			(on ? "on" : "off"),
			ktime_to_us(ktime_sub(ktime_get(), start)), ret);
	return ret;
}

static int ufshcd_setup_clocks(struct ufs_hba *hba, bool on)
{
	return  __ufshcd_setup_clocks(hba, on, false);
}

static int ufshcd_init_clocks(struct ufs_hba *hba)
{
	int ret = 0;
	struct ufs_clk_info *clki;
	struct device *dev = hba->dev;
	struct list_head *head = &hba->clk_list_head;

	if (list_empty(head))
		goto out;

	list_for_each_entry(clki, head, list) {
		if (!clki->name)
			continue;

		clki->clk = devm_clk_get(dev, clki->name);
		if (IS_ERR(clki->clk)) {
			ret = PTR_ERR(clki->clk);
			dev_err(dev, "%s: %s clk get failed, %d\n",
					__func__, clki->name, ret);
			goto out;
		}

		if (clki->max_freq) {
			ret = clk_set_rate(clki->clk, clki->max_freq);
			if (ret) {
				dev_err(hba->dev, "%s: %s clk set rate(%dHz) failed, %d\n",
					__func__, clki->name,
					clki->max_freq, ret);
				goto out;
			}
			clki->curr_freq = clki->max_freq;
		}
		dev_dbg(dev, "%s: clk: %s, rate: %lu\n", __func__,
				clki->name, clk_get_rate(clki->clk));
	}
out:
	return ret;
}

static int ufshcd_variant_hba_init(struct ufs_hba *hba)
{
	int err = 0;

	if (!hba->vops)
		goto out;

	err = ufshcd_vops_init(hba);
	if (err)
		goto out;

	err = ufshcd_vops_setup_regulators(hba, true);
	if (err)
		goto out_exit;

	goto out;

out_exit:
	ufshcd_vops_exit(hba);
out:
	if (err)
		dev_err(hba->dev, "%s: variant %s init failed err %d\n",
			__func__, ufshcd_get_var_name(hba), err);
	return err;
}

static void ufshcd_variant_hba_exit(struct ufs_hba *hba)
{
	int ret;
	if (!hba->vops)
		return;

	ret = ufshcd_vops_setup_regulators(hba, false);
	if (ret)
		dev_err(hba->dev, "%s ufshcd_vops_setup_regulators with err %d \r\n", __func__, ret);

	ufshcd_vops_exit(hba);
}

static int ufshcd_hba_init(struct ufs_hba *hba)
{
	int err;

	/*
	 * Handle host controller power separately from the UFS device power
	 * rails as it will help controlling the UFS host controller power
	 * collapse easily which is different than UFS device power collapse.
	 * Also, enable the host controller power before we go ahead with rest
	 * of the initialization here.
	 */
	err = ufshcd_init_hba_vreg(hba);
	if (err)
		goto out;

	err = ufshcd_setup_hba_vreg(hba, true);
	if (err)
		goto out;

	err = ufshcd_init_clocks(hba);
	if (err)
		goto out_disable_hba_vreg;

	err = ufshcd_setup_clocks(hba, true);
	if (err)
		goto out_disable_hba_vreg;

	err = ufshcd_init_vreg(hba);
	if (err)
		goto out_disable_clks;

	err = ufshcd_setup_vreg(hba, true);
	if (err)
		goto out_disable_clks;

	err = ufshcd_variant_hba_init(hba);
	if (err)
		goto out_disable_vreg;

	hba->is_powered = true;
	goto out;

out_disable_vreg:
	ufshcd_setup_vreg(hba, false);
out_disable_clks:
	ufshcd_setup_clocks(hba, false);
out_disable_hba_vreg:
	ufshcd_setup_hba_vreg(hba, false);
out:
	return err;
}

static void ufshcd_hba_exit(struct ufs_hba *hba)
{
	if (hba->is_powered) {
		ufshcd_variant_hba_exit(hba);
		ufshcd_setup_vreg(hba, false);
		ufshcd_suspend_clkscaling(hba);
		if (ufshcd_is_clkscaling_supported(hba)) {
			if (hba->devfreq)
				ufshcd_suspend_clkscaling(hba);
			destroy_workqueue(hba->clk_scaling.workq);
		}
		ufshcd_setup_clocks(hba, false);
		ufshcd_setup_hba_vreg(hba, false);
		hba->is_powered = false;
	}
}

static int ufshcd_send_request_sense(struct ufs_hba *hba, struct scsi_device *sdp)
{
	unsigned char cmd[6] = {REQUEST_SENSE,
				0,
				0,
				0,
				UFSHCD_REQ_SENSE_SIZE,
				0};
	char *buffer;
	int ret;

	buffer = kzalloc(UFSHCD_REQ_SENSE_SIZE, GFP_KERNEL);
	if (!buffer) {
		ret = -ENOMEM;
		goto out;
	}

	ret = scsi_execute(sdp, cmd, DMA_FROM_DEVICE, buffer,
			UFSHCD_REQ_SENSE_SIZE, NULL, NULL,
			msecs_to_jiffies(1000), 3, 0, RQF_PM, NULL);
	if (ret)
		dev_err(hba->dev, "%s: failed with err %d\n", __func__, ret);

	kfree(buffer);
out:
	return ret;
}

/**
 * ufshcd_set_dev_pwr_mode - sends START STOP UNIT command to set device
 *			     power mode
 * @hba: per adapter instance
 * @pwr_mode: device power mode to set
 *
 * Returns 0 if requested power mode is set successfully
 * Returns non-zero if failed to set the requested power mode
 */
static int ufshcd_set_dev_pwr_mode(struct ufs_hba *hba,
				     enum ufs_dev_pwr_mode pwr_mode)
{
	unsigned char cmd[6] = { START_STOP };
	struct scsi_sense_hdr sshdr = {0};
	struct scsi_device *sdp;
	unsigned long flags;
	int ret;

	spin_lock_irqsave(hba->host->host_lock, flags);
	sdp = hba->sdev_ufs_device;
	if (sdp) {
		ret = scsi_device_get(sdp);
		if (!ret && !scsi_device_online(sdp)) {
			ret = -ENODEV;
			scsi_device_put(sdp);
		}
	} else {
		ret = -ENODEV;
	}
	spin_unlock_irqrestore(hba->host->host_lock, flags);

	if (ret)
		return ret;

	/*
	 * Sync cache for ufs device, MQ freeze queue after sync cache which
	 * will cause a new request sending to Low layer between sync cache and
	 * freezed queue, then the last new request may lost its data without
	 * another sync cache
	 */
	if (hba->caps & UFSHCD_CAP_SSU_BY_SELF &&
	    (pwr_mode == UFS_SLEEP_PWR_MODE || pwr_mode == UFS_POWERDOWN_PWR_MODE)) {
		if (pwr_mode != UFS_SLEEP_PWR_MODE)
	    		dev_err(hba->dev, "UFS MQ: <%s> call ufshcd_send_scsi_sync_cache pwr_mode = %d\r\n", __func__, pwr_mode);
		ret = ufshcd_send_scsi_sync_cache(hba, sdp);
		if (ret)
			goto out;
	}

	/*
	 * If scsi commands fail, the scsi mid-layer schedules scsi error-
	 * handling, which would wait for host to be resumed. Since we know
	 * we are functional while we are here, skip host resume in error
	 * handling context.
	 */
	hba->host->eh_noresume = 1;
	if (hba->wlun_dev_clr_ua) {
		if (hba->caps & UFSHCD_CAP_SSU_BY_SELF)
			ret = ufshcd_send_scsi_request_sense(hba, sdp, 1500, true);
		else
			ret = ufshcd_send_request_sense(hba, sdp);
		if (ret)
			goto out;
		/* Unit attention condition is cleared now */
		hba->wlun_dev_clr_ua = false;
	}

	cmd[4] = pwr_mode << 4;

	/*
	 * Current function would be generally called from the power management
	 * callbacks hence set the REQ_PM flag so that it doesn't resume the
	 * already suspended childs.
	 */
	if (hba->caps & UFSHCD_CAP_SSU_BY_SELF)
		ret = ufshcd_send_scsi_ssu(hba, sdp, cmd, 1500,&sshdr);
	else
		ret = scsi_execute(sdp, cmd, DMA_NONE, NULL, 0, NULL, &sshdr,
				START_STOP_TIMEOUT, 0, 0, RQF_PM, NULL);
	if (ret) {
		sdev_printk(KERN_WARNING, sdp,
			    "START_STOP failed for power mode: %d, result %x\n",
			    pwr_mode, ret);
		if (driver_byte(ret) & DRIVER_SENSE)
			scsi_print_sense_hdr(sdp, NULL, &sshdr);
	}

	if (!ret) {
		hba->curr_dev_pwr_mode = pwr_mode;
		/* diable/enable vcc */
		ufshcd_vops_vcc_power_on_off(hba);
	}

out:
	scsi_device_put(sdp);
	hba->host->eh_noresume = 0;
	return ret;
}

static int ufshcd_link_state_transition(struct ufs_hba *hba,
					enum uic_link_state req_link_state,
					int check_for_bkops)
{
	int ret = 0;

	if (req_link_state == hba->uic_link_state)
		return 0;

	if (req_link_state == UIC_LINK_HIBERN8_STATE) {
		ret = ufshcd_uic_hibern8_enter(hba);
		if (!ret)
			ufshcd_set_link_hibern8(hba);
		else
			goto out;
	}
	/*
	 * If autobkops is enabled, link can't be turned off because
	 * turning off the link would also turn off the device.
	 */
	else if ((req_link_state == UIC_LINK_OFF_STATE) &&
		   (!check_for_bkops || (check_for_bkops &&
		    !hba->auto_bkops_enabled))) {
		/*
		 * Let's make sure that link is in low power mode, we are doing
		 * this currently by putting the link in Hibern8. Otherway to
		 * put the link in low power mode is to send the DME end point
		 * to device and then send the DME reset command to local
		 * unipro. But putting the link in hibern8 is much faster.
		 */
		ret = ufshcd_uic_hibern8_enter(hba);
		if (ret)
			goto out;
		/*
		 * Change controller state to "reset state" which
		 * should also put the link in off/reset state
		 */
		ufshcd_hba_stop(hba, true);
		/*
		 * TODO: Check if we need any delay to make sure that
		 * controller is reset
		 */
		ufshcd_set_link_off(hba);
	}

out:
	return ret;
}

static void ufshcd_vreg_set_lpm(struct ufs_hba *hba)
{
	/*
	 * It seems some UFS devices may keep drawing more than sleep current
	 * (atleast for 500us) from UFS rails (especially from VCCQ rail).
	 * To avoid this situation, add 2ms delay before putting these UFS
	 * rails in LPM mode.
	 */
	if (!ufshcd_is_link_active(hba) &&
	    hba->dev_quirks & UFS_DEVICE_QUIRK_DELAY_BEFORE_LPM)
		usleep_range(2000, 2100);

	/*
	 * If UFS device is either in UFS_Sleep turn off VCC rail to save some
	 * power.
	 *
	 * If UFS device and link is in OFF state, all power supplies (VCC,
	 * VCCQ, VCCQ2) can be turned off if power on write protect is not
	 * required. If UFS link is inactive (Hibern8 or OFF state) and device
	 * is in sleep state, put VCCQ & VCCQ2 rails in LPM mode.
	 *
	 * Ignore the error returned by ufshcd_toggle_vreg() as device is anyway
	 * in low power state which would save some power.
	 */
	if (ufshcd_is_ufs_dev_poweroff(hba) && ufshcd_is_link_off(hba) &&
	    !hba->dev_info.is_lu_power_on_wp) {
		ufshcd_setup_vreg(hba, false);
	} else if (!ufshcd_is_ufs_dev_active(hba)) {
		ufshcd_toggle_vreg(hba->dev, hba->vreg_info.vcc, false);
		if (!ufshcd_is_link_active(hba)) {
			ufshcd_config_vreg_lpm(hba, hba->vreg_info.vccq);
			ufshcd_config_vreg_lpm(hba, hba->vreg_info.vccq2);
		}
	}
}

static int ufshcd_vreg_set_hpm(struct ufs_hba *hba)
{
	int ret = 0;

	if (ufshcd_is_ufs_dev_poweroff(hba) && ufshcd_is_link_off(hba) &&
	    !hba->dev_info.is_lu_power_on_wp) {
		ret = ufshcd_setup_vreg(hba, true);
	} else if (!ufshcd_is_ufs_dev_active(hba)) {
		if (!ret && !ufshcd_is_link_active(hba)) {
			ret = ufshcd_config_vreg_hpm(hba, hba->vreg_info.vccq);
			if (ret)
				goto vcc_disable;
			ret = ufshcd_config_vreg_hpm(hba, hba->vreg_info.vccq2);
			if (ret)
				goto vccq_lpm;
		}
		ret = ufshcd_toggle_vreg(hba->dev, hba->vreg_info.vcc, true);
	}
	goto out;

vccq_lpm:
	ufshcd_config_vreg_lpm(hba, hba->vreg_info.vccq);
vcc_disable:
	ufshcd_toggle_vreg(hba->dev, hba->vreg_info.vcc, false);
out:
	return ret;
}

static void ufshcd_hba_vreg_set_lpm(struct ufs_hba *hba)
{
	if (ufshcd_is_link_off(hba))
		ufshcd_setup_hba_vreg(hba, false);
}

static void ufshcd_hba_vreg_set_hpm(struct ufs_hba *hba)
{
	if (ufshcd_is_link_off(hba))
		ufshcd_setup_hba_vreg(hba, true);
}

/**
 * ufshcd_suspend - helper function for suspend operations
 * @hba: per adapter instance
 * @pm_op: desired low power operation type
 *
 * This function will try to put the UFS device and link into low power
 * mode based on the "rpm_lvl" (Runtime PM level) or "spm_lvl"
 * (System PM level).
 *
 * If this function is called during shutdown, it will make sure that
 * both UFS device and UFS link is powered off.
 *
 * NOTE: UFS device & link must be active before we enter in this function.
 *
 * Returns 0 for success and non-zero for failure
 */
static int ufshcd_suspend(struct ufs_hba *hba, enum ufs_pm_op pm_op)
{
	int ret = 0;
	enum ufs_pm_level pm_lvl;
	enum ufs_dev_pwr_mode req_dev_pwr_mode;
	enum uic_link_state req_link_state;

	hba->pm_op_in_progress = 1;
	if (!ufshcd_is_shutdown_pm(pm_op)) {
		pm_lvl = ufshcd_is_runtime_pm(pm_op) ?
			 hba->rpm_lvl : hba->spm_lvl;
		req_dev_pwr_mode = ufs_get_pm_lvl_to_dev_pwr_mode(pm_lvl);
		req_link_state = ufs_get_pm_lvl_to_link_pwr_state(pm_lvl);
	} else {
		req_dev_pwr_mode = UFS_POWERDOWN_PWR_MODE;
		req_link_state = UIC_LINK_HIBERN8_STATE;
	}

	if (!ufshcd_is_runtime_pm(pm_op)) {
		hba->in_suspend = true;
		suspend_wait_hpb(hba);
	}

	if (req_dev_pwr_mode == UFS_ACTIVE_PWR_MODE &&
			req_link_state == UIC_LINK_ACTIVE_STATE) {
		goto disable_clks;
	}

	if ((req_dev_pwr_mode == hba->curr_dev_pwr_mode) &&
	    (req_link_state == hba->uic_link_state))
		goto out;

	/* UFS device & link must be active before we enter in this function */
	if (!ufshcd_is_ufs_dev_active(hba) || !ufshcd_is_link_active(hba)) {
		ret = -EINVAL;
		goto out;
	}

	if (ufshcd_is_auto_hibern8_allowed(hba))
		ufshcd_disable_auto_hibern8(hba);

#ifdef FEATURE_UFS_AUTO_BKOPS
	if (ufshcd_is_runtime_pm(pm_op)) {
		if (ufshcd_can_autobkops_during_suspend(hba)) {
			/*
			 * The device is idle with no requests in the queue,
			 * allow background operations if bkops status shows
			 * that performance might be impacted.
			 */
			ret = ufshcd_urgent_bkops(hba);
			if (ret)
				goto enable_gating;
		} else {
			/* make sure that auto bkops is disabled */
			if (!ufshcd_disable_auto_bkops(hba)) {
				if (ufshcd_is_auto_hibern8_allowed(hba)) {
					ufshcd_enable_auto_hibern8(hba);
				}
			}
		}
		if (ufshcd_is_auto_hibern8_allowed(hba)) {
			goto out;
		}
	}
#endif
	if ((req_dev_pwr_mode != hba->curr_dev_pwr_mode) &&
	     ((ufshcd_is_runtime_pm(pm_op) && !hba->auto_bkops_enabled) ||
	       !ufshcd_is_runtime_pm(pm_op))) {
		/*
		 * Autobkops need to be enable for Sandisk ufs device, so it
		 * should not be disabled.
		 */
		if (hba->manufacturer_id != UFS_VENDOR_SANDISK)
			ufshcd_disable_auto_bkops(hba);

		/* avoid write booster exception handler alive in suspend */
		flush_work(&hba->wb_work);

		ret = ufshcd_set_dev_pwr_mode(hba, req_dev_pwr_mode);
		if (ret)
			goto enable_gating;
	}

	if (hba->vops && hba->vops->suspend_before_set_link_state) {
		ret = hba->vops->suspend_before_set_link_state(hba, pm_op);
		if (ret)
			goto set_dev_active;
	}
	ret = ufshcd_link_state_transition(hba, req_link_state, 1);
	if (ret)
		goto vops_resume_after_set_link_state;

	ufshcd_vreg_set_lpm(hba);

disable_clks:
	ufs_idle_intr_toggle(hba, 0);

	/*
	 * Call vendor specific suspend callback. As these callbacks may access
	 * vendor specific host controller register space call them before the
	 * host clocks are ON.
	 */
	ret = ufshcd_vops_suspend(hba, pm_op);
	if (ret)
		goto set_link_active;


	/* Put the host controller in low power mode if possible */
	ufshcd_hba_vreg_set_lpm(hba);
	goto out;

set_link_active:
	ufshcd_vreg_set_hpm(hba);
	if (ufshcd_is_link_hibern8(hba) && !ufshcd_uic_hibern8_exit(hba))
		ufshcd_set_link_active(hba);
	else if (ufshcd_is_link_off(hba)) {
		ufshcd_update_error_stats(hba, UFS_ERR_VOPS_SUSPEND);
		ret = ufshcd_host_reset_and_restore(hba);
	}
vops_resume_after_set_link_state:
	if (hba->vops && hba->vops->resume_after_set_link_state)
		hba->vops->resume_after_set_link_state(hba, pm_op);
set_dev_active:
	if (!ufshcd_is_runtime_pm(pm_op)) {
		if (!ufshcd_set_dev_pwr_mode(hba, UFS_ACTIVE_PWR_MODE)) {
			if (hba->manufacturer_id != UFS_VENDOR_SANDISK)
				ufshcd_disable_auto_bkops(hba);
		}
	}
enable_gating:
	if (ufshcd_is_auto_hibern8_allowed(hba))
		ufshcd_enable_auto_hibern8(hba);
out:
	hba->pm_op_in_progress = 0;

	if (ret)
		ufshcd_update_error_stats(hba, UFS_ERR_SUSPEND);

	return ret;
}

/**
 * ufshcd_resume - helper function for resume operations
 * @hba: per adapter instance
 * @pm_op: runtime PM or system PM
 *
 * This function basically brings the UFS device, UniPro link and controller
 * to active state.
 *
 * Returns 0 for success and non-zero for failure
 */
static int ufshcd_resume(struct ufs_hba *hba, enum ufs_pm_op pm_op)
{
	int ret = 0;

	hba->pm_op_in_progress = 1;
	ufshcd_hba_vreg_set_hpm(hba);
	/* Make sure clocks are enabled before accessing controller */
	ret = ufshcd_setup_clocks(hba, true);
	if (ret)
		goto out;

	/*
	 * Call vendor specific resume callback. As these callbacks may access
	 * vendor specific host controller register space call them when the
	 * host clocks are ON.
	 */
	ret = ufshcd_vops_resume(hba, pm_op);
	if (ret)
		goto disable_vreg;

	if (ufshcd_is_link_hibern8(hba)) {
		ret = ufshcd_uic_hibern8_exit(hba);
		if (!ret) {
			ufshcd_set_link_active(hba);
		} else {
			if (!ufshcd_is_runtime_pm(pm_op)) {
				kthread_flush_work(&hba->eh_work);
				dev_err(hba->dev, "flush error handle work done\n");
				ret = 0;
			} else {
				goto out;
			}
		}
	} else if (ufshcd_is_link_off(hba)) {
		ret = ufshcd_host_reset_and_restore(hba);
		/*
		 * ufshcd_host_reset_and_restore() should have already
		 * set the link state as active
		 */
		if (ret || !ufshcd_is_link_active(hba))
			goto vendor_suspend;
	}

	if (hba->vops && hba->vops->resume_after_set_link_state) {
		ret = hba->vops->resume_after_set_link_state(hba, pm_op);
		if (ret)
			goto vendor_suspend;
	}

	if ((!ufshcd_is_ufs_dev_active(hba)) && (!ufshcd_is_runtime_pm(pm_op))) {
		ret = ufshcd_set_dev_pwr_mode(hba, UFS_ACTIVE_PWR_MODE);
		if (ret) {
			kthread_flush_work(&hba->eh_work);
			dev_err(hba->dev, "ssu fail flush error handle work done\n");
			ret = 0;
		}
	}

	if (ufshcd_is_auto_hibern8_allowed(hba)) {
		ufshcd_enable_auto_hibern8(hba);
	}
#ifdef FEATURE_UFS_AUTO_BKOPS
	/*
	 * If BKOPs operations are urgently needed at this moment then
	 * keep auto-bkops enabled or else disable it.
	 */
	ret = ufshcd_urgent_bkops(hba);
#endif
	ufs_idle_intr_toggle(hba, 1);

	goto out;

vendor_suspend:

	ufshcd_vops_suspend_before_set_link_state(hba, pm_op);
	ufshcd_vops_suspend(hba, pm_op);

disable_vreg:
	ufshcd_vreg_set_lpm(hba);
out:
	hba->pm_op_in_progress = 0;

	if (ret)
		ufshcd_update_error_stats(hba, UFS_ERR_RESUME);
	else
		hba->in_suspend = false;

	return ret;
}

/**
 * ufshcd_system_suspend - system suspend routine
 * @hba: per adapter instance
 * @pm_op: runtime PM or system PM
 *
 * Check the description of ufshcd_suspend() function for more details.
 *
 * Returns 0 for success and non-zero for failure
 */
int ufshcd_system_suspend(struct ufs_hba *hba)
{
	int ret = 0;
	ktime_t start = ktime_get();

	if (!hba || !hba->is_powered)
		return 0;

#ifdef CONFIG_SCSI_UFS_HS_ERROR_RECOVER
	if (hba->disable_suspend)
		return -1;
#endif
	if ((ufs_get_pm_lvl_to_dev_pwr_mode(hba->spm_lvl) ==
	     hba->curr_dev_pwr_mode) &&
	    (ufs_get_pm_lvl_to_link_pwr_state(hba->spm_lvl) ==
	     hba->uic_link_state))
		goto out;

	if (pm_runtime_suspended(hba->dev)) {
		/*
		 * UFS device and/or UFS link low power states during runtime
		 * suspend seems to be different than what is expected during
		 * system suspend. Hence runtime resume the devic & link and
		 * let the system suspend low power states to take effect.
		 * TODO: If resume takes longer time, we might have optimize
		 * it in future by not resuming everything if possible.
		 */
		ret = ufshcd_runtime_resume(hba);
		if (ret)
			goto out;
	}

	ret = ufshcd_suspend(hba, UFS_SYSTEM_PM);
out:
        trace_ufshcd_system_suspend(dev_name(hba->dev), ret,
	    ktime_to_us(ktime_sub(ktime_get(), start)),
	    hba->curr_dev_pwr_mode, hba->uic_link_state);
	if (!ret)
		hba->is_sys_suspended = true;
	return ret;
}
EXPORT_SYMBOL(ufshcd_system_suspend);

/**
 * ufshcd_system_resume - system resume routine
 * @hba: per adapter instance
 *
 * Returns 0 for success and non-zero for failure
 */

int ufshcd_system_resume(struct ufs_hba *hba)
{
	int ret = 0;
	ktime_t start = ktime_get();

	if (!hba)
		return -EINVAL;

	if (!hba->is_powered)
		/*
		 * Let the runtime resume take care of resuming
		 * if runtime suspended.
		 */
		goto out;
	else
		ret = ufshcd_resume(hba, UFS_SYSTEM_PM);
	if (!ret)
		hba->is_sys_suspended = false;
out:
        trace_ufshcd_system_resume(dev_name(hba->dev), ret,
		ktime_to_us(ktime_sub(ktime_get(), start)),
		hba->curr_dev_pwr_mode, hba->uic_link_state);
	return ret;
}
EXPORT_SYMBOL(ufshcd_system_resume);

/**
 * ufshcd_runtime_suspend - runtime suspend routine
 * @hba: per adapter instance
 *
 * Check the description of ufshcd_suspend() function for more details.
 *
 * Returns 0 for success and non-zero for failure
 */
int ufshcd_runtime_suspend(struct ufs_hba *hba)
{
	int ret = 0;
	ktime_t start = ktime_get();

	if (!hba)
		return -EINVAL;

	if (!hba->is_powered)
		goto out;
	else
		ret = ufshcd_suspend(hba, UFS_RUNTIME_PM);
out:
        trace_ufshcd_runtime_suspend(dev_name(hba->dev), ret,
		ktime_to_us(ktime_sub(ktime_get(), start)),
		hba->curr_dev_pwr_mode, hba->uic_link_state);
	return ret;
}
EXPORT_SYMBOL(ufshcd_runtime_suspend);

/**
 * ufshcd_runtime_resume - runtime resume routine
 * @hba: per adapter instance
 *
 * This function basically brings the UFS device, UniPro link and controller
 * to active state. Following operations are done in this function:
 *
 * 1. Turn on all the controller related clocks
 * 2. Bring the UniPro link out of Hibernate state
 * 3. If UFS device is in sleep state, turn ON VCC rail and bring the UFS device
 *    to active state.
 * 4. If auto-bkops is enabled on the device, disable it.
 *
 * So following would be the possible power state after this function return
 * successfully:
 *	S1: UFS device in Active state with VCC rail ON
 *	    UniPro link in Active state
 *	    All the UFS/UniPro controller clocks are ON
 *
 * Returns 0 for success and non-zero for failure
 */
int ufshcd_runtime_resume(struct ufs_hba *hba)
{
	if (!hba || !hba->is_powered)
		return 0;
	if (hba->ufshcd_state == UFSHCD_STATE_RESET)
		return -EIO;
	return ufshcd_resume(hba, UFS_RUNTIME_PM);
}
EXPORT_SYMBOL(ufshcd_runtime_resume);

int ufshcd_runtime_idle(struct ufs_hba *hba)
{
	return 0;
}
EXPORT_SYMBOL(ufshcd_runtime_idle);

static inline ssize_t ufshcd_pm_lvl_store(struct device *dev,
					   struct device_attribute *attr,
					   const char *buf, size_t count,
					   bool rpm)
{
	struct ufs_hba *hba = dev_get_drvdata(dev);
	unsigned long flags, value;

	if (kstrtoul(buf, 0, &value))
		return -EINVAL;

	if (value >= UFS_PM_LVL_MAX)
		return -EINVAL;

	spin_lock_irqsave(hba->host->host_lock, flags);
	if (rpm)
		hba->rpm_lvl = value;
	else
		hba->spm_lvl = value;
	spin_unlock_irqrestore(hba->host->host_lock, flags);
	return count;
}

static ssize_t ufshcd_rpm_lvl_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct ufs_hba *hba = dev_get_drvdata(dev);
	int curr_len;
	u8 lvl;

	if (hba->rpm_lvl >= UFS_PM_LVL_MAX)
		return 0;

	curr_len = snprintf(buf, PAGE_SIZE,
			    "\nCurrent Runtime PM level [%d] => dev_state [%s] link_state [%s]\n",
			    hba->rpm_lvl,
			    ufschd_ufs_dev_pwr_mode_to_string(
				ufs_pm_lvl_states[hba->rpm_lvl].dev_state),
			    ufschd_uic_link_state_to_string(
				ufs_pm_lvl_states[hba->rpm_lvl].link_state));

	curr_len += snprintf((buf + curr_len), (PAGE_SIZE - curr_len),
			     "\nAll available Runtime PM levels info:\n");
	for (lvl = UFS_PM_LVL_0; lvl < UFS_PM_LVL_MAX; lvl++)
		curr_len += snprintf((buf + curr_len), (PAGE_SIZE - curr_len),
				     "\tRuntime PM level [%d] => dev_state [%s] link_state [%s]\n",
				    lvl,
				    ufschd_ufs_dev_pwr_mode_to_string(
					ufs_pm_lvl_states[lvl].dev_state),
				    ufschd_uic_link_state_to_string(
					ufs_pm_lvl_states[lvl].link_state));

	return curr_len;
}

static ssize_t ufshcd_rpm_lvl_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	return ufshcd_pm_lvl_store(dev, attr, buf, count, true);
}

static void ufshcd_add_rpm_lvl_sysfs_nodes(struct ufs_hba *hba)
{
	hba->rpm_lvl_attr.show = ufshcd_rpm_lvl_show;
	hba->rpm_lvl_attr.store = ufshcd_rpm_lvl_store;
	sysfs_attr_init(&hba->rpm_lvl_attr.attr);
	hba->rpm_lvl_attr.attr.name = "rpm_lvl";
	hba->rpm_lvl_attr.attr.mode = 0644;
	if (device_create_file(hba->dev, &hba->rpm_lvl_attr))
		dev_err(hba->dev, "Failed to create sysfs for rpm_lvl\n");
}

static ssize_t ufshcd_spm_lvl_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct ufs_hba *hba = dev_get_drvdata(dev);
	int curr_len;
	u8 lvl;

	if (hba->spm_lvl >= UFS_PM_LVL_MAX)
		return 0;

	curr_len = snprintf(buf, PAGE_SIZE,
			    "\nCurrent System PM level [%d] => dev_state [%s] link_state [%s]\n",
			    hba->spm_lvl,
			    ufschd_ufs_dev_pwr_mode_to_string(
				ufs_pm_lvl_states[hba->spm_lvl].dev_state),
			    ufschd_uic_link_state_to_string(
				ufs_pm_lvl_states[hba->spm_lvl].link_state));

	curr_len += snprintf((buf + curr_len), (PAGE_SIZE - curr_len),
			     "\nAll available System PM levels info:\n");
	for (lvl = UFS_PM_LVL_0; lvl < UFS_PM_LVL_MAX; lvl++)
		curr_len += snprintf((buf + curr_len), (PAGE_SIZE - curr_len),
				     "\tSystem PM level [%u] => dev_state [%s] link_state [%s]\n",
				    lvl,
				    ufschd_ufs_dev_pwr_mode_to_string(
					ufs_pm_lvl_states[lvl].dev_state),
				    ufschd_uic_link_state_to_string(
					ufs_pm_lvl_states[lvl].link_state));

	return curr_len;
}

static ssize_t ufshcd_spm_lvl_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	return ufshcd_pm_lvl_store(dev, attr, buf, count, false);
}

static void ufshcd_add_spm_lvl_sysfs_nodes(struct ufs_hba *hba)
{
	hba->spm_lvl_attr.show = ufshcd_spm_lvl_show;
	hba->spm_lvl_attr.store = ufshcd_spm_lvl_store;
	sysfs_attr_init(&hba->spm_lvl_attr.attr);
	hba->spm_lvl_attr.attr.name = "spm_lvl";
	hba->spm_lvl_attr.attr.mode = 0644;
	if (device_create_file(hba->dev, &hba->spm_lvl_attr))
		dev_err(hba->dev, "Failed to create sysfs for spm_lvl\n");
}

static inline void ufshcd_add_sysfs_nodes(struct ufs_hba *hba)
{
	ufshcd_add_rpm_lvl_sysfs_nodes(hba);
	ufshcd_add_spm_lvl_sysfs_nodes(hba);
}

static inline void ufshcd_remove_sysfs_nodes(struct ufs_hba *hba)
{
	device_remove_file(hba->dev, &hba->rpm_lvl_attr);
	device_remove_file(hba->dev, &hba->spm_lvl_attr);
}

/**
 * ufshcd_shutdown - shutdown routine
 * @hba: per adapter instance
 *
 * This function would power off both UFS device and UFS link.
 *
 * Returns 0 always to allow force shutdown even in case of errors.
 */
int ufshcd_shutdown(struct ufs_hba *hba)
{
	int ret = 0;

	if (!hba->is_powered)
		goto out;
	if (ufshcd_is_ufs_dev_poweroff(hba) && ufshcd_is_link_off(hba))
		goto out;

	if (pm_runtime_suspended(hba->dev)) {
		ret = ufshcd_runtime_resume(hba);
		if (ret)
			goto out;
	}

	ret = ufshcd_suspend(hba, UFS_SHUTDOWN_PM);
out:
	if (ret)
		dev_err(hba->dev, "%s failed, err %d\n", __func__, ret);
	/* allow force shutdown even in case of errors */
	return 0;
}
EXPORT_SYMBOL(ufshcd_shutdown);

/**
 * ufshcd_remove - de-allocate SCSI host and host memory space
 *		data structure memory
 * @hba - per adapter instance
 */
void ufshcd_remove(struct ufs_hba *hba)
{
	ufshcd_remove_sysfs_nodes(hba);
#ifdef CONFIG_HUAWEI_DSM_IOMT_UFS_HOST
	dsm_iomt_ufs_host_exit(hba->host);
#endif
	scsi_remove_host(hba->host);
	/* disable interrupts */
	ufshcd_disable_intr(hba, hba->intr_mask);
	ufshcd_hba_stop(hba, true);

	wakeup_source_trash(&ffu_lock);

	ufshcd_exit_clk_gating(hba);
	if (ufshcd_is_clkscaling_supported(hba))
		device_remove_file(hba->dev, &hba->clk_scaling.enable_attr);
	ufshcd_hba_exit(hba);
	ufs_fault_inject_fs_remove();
	ufsdbg_remove_debugfs(hba);
}
EXPORT_SYMBOL_GPL(ufshcd_remove);

/**
 * ufshcd_dealloc_host - deallocate Host Bus Adapter (HBA)
 * @hba: pointer to Host Bus Adapter (HBA)
 */
void ufshcd_dealloc_host(struct ufs_hba *hba)
{
	scsi_host_put(hba->host);
}
EXPORT_SYMBOL_GPL(ufshcd_dealloc_host);

/**
 * ufshcd_set_dma_mask - Set dma mask based on the controller
 *			 addressing capability
 * @hba: per adapter instance
 *
 * Returns 0 for success, non-zero for failure
 */
static int ufshcd_set_dma_mask(struct ufs_hba *hba)
{
	if (hba->capabilities & MASK_64_ADDRESSING_SUPPORT) {
		if (!dma_set_mask_and_coherent(hba->dev, ~0ULL))/*lint !e648 !e598*/
			return 0;
	}
	return dma_set_mask_and_coherent(hba->dev, DMA_BIT_MASK(32));
}

/**
 * ufshcd_alloc_host - allocate Host Bus Adapter (HBA)
 * @dev: pointer to device handle
 * @hba_handle: driver private handle
 * Returns 0 on success, non-zero value on failure
 */
int ufshcd_alloc_host(struct device *dev, struct ufs_hba **hba_handle)
{
	struct Scsi_Host *host;
	struct ufs_hba *hba;
	int err = 0;

	if (!dev) {
		dev_err(dev,
		"Invalid memory reference for dev is NULL\n");
		err = -ENODEV;
		goto out_error;
	}

	host = scsi_host_alloc(&ufshcd_driver_template,
				sizeof(struct ufs_hba));
	if (!host) {
		dev_err(dev, "scsi_host_alloc failed\n");
		err = -ENOMEM;
		goto out_error;
	}
	hba = shost_priv(host);
	hba->host = host;
	hba->dev = dev;
	*hba_handle = hba;
#ifdef CONFIG_HUAWEI_DSM_IOMT_UFS_HOST
	dsm_iomt_hba_host_pre_init(hba);
#endif
	INIT_LIST_HEAD(&hba->clk_list_head);

out_error:
	return err;
}
EXPORT_SYMBOL(ufshcd_alloc_host);

static void ufshcd_hisi_mq_init(struct Scsi_Host *host)
{
	host->use_blk_mq = 1;
	host->queue_quirk_flag |= SHOST_QUIRK(SHOST_QUIRK_UNMAP_IN_SOFTIRQ);
	host->queue_quirk_flag |= SHOST_QUIRK(SHOST_QUIRK_SCSI_QUIESCE_IN_LLD);
	host->queue_quirk_flag |= SHOST_QUIRK(SHOST_QUIRK_BUSY_IDLE_ENABLE);
	host->queue_quirk_flag |= SHOST_QUIRK(SHOST_QUIRK_IO_LATENCY_WARNING);
	host->queue_quirk_flag |= SHOST_QUIRK(SHOST_QUIRK_FLUSH_REDUCING);
	if (host->use_blk_mq) {
		host->queue_quirk_flag |= SHOST_QUIRK(SHOST_QUIRK_HISI_UFS_MQ);
		if (host->queue_quirk_flag &
			SHOST_QUIRK(SHOST_QUIRK_HISI_UFS_MQ)) {
			host->queue_quirk_flag |=
				SHOST_QUIRK(SHOST_QUIRK_DRIVER_TAG_ALLOC);
			host->nr_hw_queues = 1;
			host->mq_queue_depth = 192;
			host->mq_reserved_queue_depth = 64;
			host->mq_high_prio_queue_depth = 64;
			host->can_queue =
				host->mq_queue_depth * (int)host->nr_hw_queues;
		}
	}
}

/**
 * ufshcd_init - Driver initialization routine
 * @hba: per-adapter instance
 * @mmio_base: base register address
 * @irq: Interrupt line of device
 * Returns 0 on success, non-zero value on failure
 */
int ufshcd_init(struct ufs_hba *hba, void __iomem *mmio_base, unsigned int irq, int timer_irq)
{
	int err;
	struct Scsi_Host *host = hba->host;
	struct device *dev = hba->dev;

	if (!mmio_base) {
		dev_err(hba->dev,
		"Invalid memory reference for mmio_base is NULL\n");
		err = -ENODEV;
		goto out_error_directly;
	}

	mutex_init(&hba->eh_mutex);
	dsm_ufs_init(hba);
	hba->mmio_base = mmio_base;
	hba->irq = irq;
	hba->timer_irq = timer_irq;

	err = ufshcd_send_scsi_sync_cache_init();
	if(err)
		goto out_error;
	/* Set descriptor lengths to specification defaults */
	ufshcd_def_desc_sizes(hba);

	err = ufshcd_hba_init(hba);
	if (err)
		goto out_error;

	/* Read capabilities registers */
	ufshcd_hba_capabilities(hba);

	/* Get UFS version supported by the controller */
	hba->ufs_version = ufshcd_get_ufs_version(hba);

	if ((hba->ufs_version != UFSHCI_VERSION_10) &&
	    (hba->ufs_version != UFSHCI_VERSION_11) &&
	    (hba->ufs_version != UFSHCI_VERSION_20) &&
	    (hba->ufs_version != UFSHCI_VERSION_21) &&
	    (hba->ufs_version != UFSHCI_VERSION_30))
		dev_err(hba->dev, "invalid UFS version 0x%x\n",
			hba->ufs_version);

	/* Get Interrupt bit mask per version */
	hba->intr_mask = ufshcd_get_intr_mask(hba);

	err = ufshcd_set_dma_mask(hba);
	if (err) {
		dev_err(hba->dev, "set dma mask failed\n");
		goto out_disable;
	}

	/* Allocate memory for host memory space */
	err = ufshcd_memory_alloc(hba);
	if (err) {
		dev_err(hba->dev, "Memory allocation failed\n");
		goto out_disable;
	}

	/* Configure LRB */
	ufshcd_host_memory_configure(hba);
	host->can_queue = hba->nutrs;
#ifdef CONFIG_SCSI_UFS_HISI_UFS_MQ_DEFAULT
	ufshcd_hisi_mq_init(host);
#endif /* CONFIG_SCSI_UFS_HISI_UFS_MQ_DEFAULT */

#ifdef CONFIG_HISI_UFS_HC_CORE_UTR
	core_utr_qos_ctrl_init(hba);
#endif
	host->cmd_per_lun = hba->nutrs;
	host->max_id = UFSHCD_MAX_ID;
	if (likely(!host->is_emulator))
		host->max_lun = UFS_MAX_LUNS;
	else
		host->max_lun = 4;
	host->max_channel = UFSHCD_MAX_CHANNEL;
	host->unique_id = host->host_no;
	host->max_cmd_len = MAX_CDB_SIZE;
	host->set_dbd_for_caching = 1;

#ifdef CONFIG_HISI_UFS_MANUAL_BKOPS
	host->hisi_dev_quirk_flag |= SHOST_HISI_DEV_QUIRK(SHOST_QUIRK_BKOPS_ENABLE);
#endif
	hba->autoh8_disable_depth = 1;
	hba->ahit = UFS_AHIT_AUTOH8_TIMER;

	hba->dev_tmt_disable_depth = 1;

	hba->max_pwr_info.is_valid = false;

#ifdef CONFIG_SCSI_UFS_HS_ERROR_RECOVER
	hba->init_retry = 6;
	hba->v_tx = 0;
	hba->v_rx = 0;
#endif

	/* Initailize wait queue for task management */
	init_waitqueue_head(&hba->tm_wq);
	init_waitqueue_head(&hba->tm_tag_wq);

#ifdef CONFIG_SCSI_UFS_HS_ERROR_RECOVER
	wakeup_source_init(&hba->recover_wake_lock, "ufs_recover");
	INIT_WORK(&hba->recover_hs_work, ufs_recover_hs_mode);
#endif

	/* Initialize work queues */

	kthread_init_worker(&hba->eh_worker);
	hba->eh_worker_task = kthread_run(
		kthread_worker_fn, &hba->eh_worker, "ufs_eh_worker");
	kthread_init_work(&hba->eh_work, ufshcd_err_handler);

	INIT_WORK(&hba->eeh_work, ufshcd_exception_event_handler);
	if (likely(!hba->host->is_emulator)) {
		INIT_WORK(&hba->rpmb_pm_work, ufs_rpmb_pm_runtime_delay_enable);
		INIT_WORK(&hba->ffu_pm_work, ufs_ffu_pm_runtime_delay_enable);
	}
#ifdef CONFIG_SCSI_UFS_HI1861_VCMD
	INIT_WORK(&hba->fsr_work, ufshcd_fsr_dump_handler);
#endif
	/* Initialize UIC command mutex */
	mutex_init(&hba->uic_cmd_mutex);

	/* Initialize mutex for device management commands */
	mutex_init(&hba->dev_cmd.lock);

	init_rwsem(&hba->clk_scaling_lock);

	/* Initialize device management tag acquire wait queue */
	init_waitqueue_head(&hba->dev_cmd.tag_wq);

	ufshcd_init_clk_gating(hba);

	/*
	 * In order to avoid any spurious interrupt immediately after
	 * registering UFS controller interrupt handler, clear any pending UFS
	 * interrupt status and disable all the UFS interrupts.
	 */
	ufshcd_writel(hba, ufshcd_readl(hba, REG_INTERRUPT_STATUS),
		      REG_INTERRUPT_STATUS);
	ufshcd_writel(hba, 0, REG_INTERRUPT_ENABLE);
	/*
	 * Make sure that UFS interrupts are disabled and any pending interrupt
	 * status is cleared before registering UFS interrupt handler.
	 */
	mb();
	wakeup_source_init(&ffu_lock, "ffu_wakelock");

#ifdef CONFIG_SCSI_UFS_HI1861_VCMD
	ufshcd_init_fsr_sys(hba);
#endif
	ufshcd_init_ufs_temp_sys(hba);
#ifndef CONFIG_SCSI_UFS_INTR_HUB
	/* IRQ registration */
	err = devm_request_irq(dev, irq, ufshcd_intr, IRQF_SHARED, UFSHCD, hba);
	if (err) {
		dev_err(hba->dev, "request irq failed\n");
		goto exit_gating;
	}
#endif

	if (timer_irq >= 0) {
		err = devm_request_irq(dev, (unsigned int)timer_irq,
			ufshcd_timeout_handle_intr, IRQF_SHARED, "ufshcd_timer",
			hba);
		if (!err) {
			if (!(hba->caps & UFSHCD_CAP_BROKEN_IDLE_INTR)) {
				hba->idle_timeout_val = UFSHCD_IDLE_TIMEOUT_DEFAULT_VAL;
				host->queue_quirk_flag |= SHOST_QUIRK(SHOST_QUIRK_BUSY_IDLE_INTR_ENABLE);
				hba->ufs_idle_intr_en = 1;
				ufshcd_add_idle_intr_check_timer(hba);
			}
		} else {
			dev_err(hba->dev, "request timer irq failed\n");
			goto exit_gating;
		}
	}

#ifdef CONFIG_HISI_UFS_HC
	if (!(hba->caps & UFSHCD_CAP_BROKEN_IDLE_INTR)) {
		hba->idle_timeout_val = UFSHCD_IDLE_TIMEOUT_DEFAULT_VAL;
		host->queue_quirk_flag |=
			SHOST_QUIRK(SHOST_QUIRK_BUSY_IDLE_INTR_ENABLE);
		hba->ufs_idle_intr_en = 1;
		ufshcd_enable_vs_intr(hba, IDLE_PREJUDGE_INTR);
		ufshcd_add_idle_intr_check_timer(hba);
	}
	if (ufshcd_is_hisi_ufs_hc(hba)) {
#ifdef CONFIG_SCSI_UFS_INTR_HUB
		if (hba->ufs_intr_hub_irq) {
			err = devm_request_irq(dev, hba->ufs_intr_hub_irq,
					       ufshcd_ufs_intr_hub_intr,
					       IRQF_SHARED,
					       "ufshcd_ufs_intr_hub_irq", hba);
			if (err) {
				dev_err(hba->dev,
					"request ufs intr hub irq failed\n");
				goto exit_gating;
			}
		}
#else
		/* IRQ registration */
		if (hba->unipro_irq >= 0) {
			err = devm_request_irq(dev, hba->unipro_irq,
				ufshcd_hisi_unipro_intr, IRQF_SHARED,
				"ufshcd_hisi_unipro", hba);
			if (err) {
				dev_err(hba->dev, "request irq failed\n");
				goto exit_gating;
			}
		}

		if (hba->fatal_err_irq >= 0) {
			err = devm_request_irq(dev, hba->fatal_err_irq,
				ufshcd_hisi_fatal_err_intr, IRQF_SHARED,
				"ufshcd_hisi_fatal_err", hba);
			if (err) {
				dev_err(hba->dev,
					"request ufs fatal err irq failed\n");
				goto exit_gating;
			}
		}
#endif

#ifdef CONFIG_HISI_UFS_HC_CORE_UTR
		err = request_irq_for_core_irq(hba, dev);
		if (err) {
			dev_err(hba->dev, "request core irq failed\n");
			goto exit_gating;
		}
#endif
	}
#endif /* CONFIG_HISI_UFS_HC */

	err = scsi_add_host(host, hba->dev);
	if (err) {
		dev_err(hba->dev, "scsi_add_host failed\n");
		goto exit_gating;
	}
#ifdef CONFIG_HUAWEI_DSM_IOMT_UFS_HOST
	dsm_iomt_ufs_host_init(hba->host);
#endif
	if (ufshcd_is_clkscaling_supported(hba)) {
		char wq_name[sizeof("ufs_clkscaling_00")];

		INIT_WORK(&hba->clk_scaling.suspend_work,
			  ufshcd_clk_scaling_suspend_work);
		INIT_WORK(&hba->clk_scaling.resume_work,
			  ufshcd_clk_scaling_resume_work);

		snprintf(wq_name, sizeof(wq_name), "ufs_clkscaling_%d",
			 host->host_no);
		hba->clk_scaling.workq = create_singlethread_workqueue(wq_name);

		ufshcd_clkscaling_init_sysfs(hba);
	}

	/* Prohibit triggering pm_runtime operations when excute scsi_add_host. */
	pm_runtime_enable(hba->dev);

	/* Hold auto suspend until async scan completes */
	pm_runtime_get_sync(dev);
	/*
	 * We are assuming that device wasn't put in sleep/power-down
	 * state exclusively during the boot stage before kernel.
	 * This assumption helps avoid doing link startup twice during
	 * ufshcd_probe_hba().
	 */
	ufshcd_set_ufs_dev_active(hba);
	async_schedule(ufshcd_async_scan, hba);

	ufs_trace_fd_init();
	ufsdbg_add_debugfs(hba);
	ufs_fault_inject_fs_setup();
	ufshcd_add_sysfs_nodes(hba);

	return 0;

exit_gating:
	wakeup_source_trash(&ffu_lock);
	ufshcd_exit_clk_gating(hba);
out_disable:
	hba->is_irq_enabled = false;
	ufshcd_hba_exit(hba);
out_error:
	ufshcd_send_scsi_sync_cache_deinit();
out_error_directly:
	return err;
}
EXPORT_SYMBOL_GPL(ufshcd_init);

MODULE_AUTHOR("Santosh Yaragnavi <santosh.sy@samsung.com>");
MODULE_AUTHOR("Vinayak Holikatti <h.vinayak@samsung.com>");
MODULE_DESCRIPTION("Generic UFS host controller driver Core");
MODULE_LICENSE("GPL");
MODULE_VERSION(UFSHCD_DRIVER_VERSION);
/********************************************************************/
/****************************add by hisi,begin***********************/
/********************************************************************/
static void ufshcd_print_uic_err_hist(struct ufs_hba *hba,
		struct ufs_uic_err_reg_hist *err_hist, char *err_name)
{
	int i;

	for (i = 0; i < UIC_ERR_REG_HIST_LENGTH; i++) {
		int p = (i + err_hist->pos - 1) % UIC_ERR_REG_HIST_LENGTH;

		if (err_hist->reg[p] == 0)
			continue;
		dev_err(hba->dev, "%s[%d] = 0x%x at %lld us\n", err_name, i,
			err_hist->reg[p], ktime_to_us(err_hist->tstamp[p]));
	}
}

static void ufshcd_print_host_regs(struct ufs_hba *hba)
{
	if (hba->vops && hba->vops->dbg_hci_dump)
		hba->vops->dbg_hci_dump(hba);
	if (ufshcd_is_hisi_ufs_hc(hba) && hba->vops &&
	    hba->vops->dbg_hisi_dme_dump)
		hba->vops->dbg_hisi_dme_dump(hba);
#ifdef CONFIG_HISI_UFS_HC
	if (hba->vops && hba->vops->dbg_uic_dump)
		hba->vops->dbg_uic_dump(hba);
#endif
	/*
	 * hex_dump reads its data without the readl macro. This might
	 * cause inconsistency issues on some platform, as the printed
	 * values may be from cache and not the most recent value.
	 * To know whether you are looking at an un-cached version verify
	 * that IORESOURCE_MEM flag is on when xxx_get_resource() is invoked
	 * during platform/pci probe function.
	 */
	ufshcd_hex_dump("host regs: ", hba->mmio_base, UFSHCI_REG_SPACE_SIZE);
	dev_err(hba->dev, "hba->ufs_version = 0x%x, hba->capabilities = 0x%x\n",
		hba->ufs_version, hba->capabilities);
	dev_err(hba->dev,
		"hba->outstanding_reqs = 0x%x, hba->outstanding_tasks = 0x%x\n",
		(u32)hba->outstanding_reqs, (u32)hba->outstanding_tasks);
	dev_err(hba->dev,
		"last_hibern8_exit_tstamp at %lld us, hibern8_exit_cnt = %d\n",
		ktime_to_us(hba->ufs_stats.last_hibern8_exit_tstamp),
		hba->ufs_stats.hibern8_exit_cnt);

	ufshcd_print_uic_err_hist(hba, &hba->ufs_stats.pa_err, "pa_err");
	ufshcd_print_uic_err_hist(hba, &hba->ufs_stats.dl_err, "dl_err");
	ufshcd_print_uic_err_hist(hba, &hba->ufs_stats.nl_err, "nl_err");
	ufshcd_print_uic_err_hist(hba, &hba->ufs_stats.tl_err, "tl_err");
	ufshcd_print_uic_err_hist(hba, &hba->ufs_stats.dme_err, "dme_err");

	ufshcd_print_clk_freqs(hba);

	if (hba->vops && hba->vops->dbg_register_dump)
		hba->vops->dbg_register_dump(hba);
}
#ifdef CONFIG_HISI_DEBUG_FS
static
void ufshcd_print_trs(struct ufs_hba *hba, unsigned long bitmap, bool pr_prdt)
{
	struct ufshcd_lrb *lrbp;
	int prdt_length;
	int tag;

	dev_err(hba->dev, "current ktime %lld us\n", ktime_to_us(ktime_get()));
	for_each_set_bit(tag, &bitmap, hba->nutrs) {
		lrbp = &hba->lrb[tag];

		dev_err(hba->dev, "UPIU[%d] - issue time %lld us\n",
				tag, ktime_to_us(lrbp->issue_time_stamp));
		dev_err(hba->dev, "UPIU[%d] - complete time %lld us\n",
				tag, ktime_to_us(lrbp->complete_time_stamp));
		dev_err(hba->dev,
			"UPIU[%d] - Transfer Request Descriptor phys@0x%llx\n",
			tag, (u64)lrbp->utrd_dma_addr);

		if (ufshcd_is_hisi_ufs_hc(hba))
			ufshcd_hex_dump("UPIU TRD: ",
				lrbp->hisi_utr_descriptor_ptr,
				sizeof(struct hisi_utp_transfer_req_desc));
		else
			ufshcd_hex_dump("UPIU TRD: ", lrbp->utr_descriptor_ptr,
				sizeof(struct utp_transfer_req_desc));

		dev_err(hba->dev, "UPIU[%d] - Request UPIU phys@0x%llx\n", tag,
			(u64)lrbp->ucd_req_dma_addr);
		ufshcd_hex_dump("UPIU REQ: ", lrbp->ucd_req_ptr,
				sizeof(struct utp_upiu_req));
		dev_err(hba->dev, "UPIU[%d] - Response UPIU phys@0x%llx\n", tag,
			(u64)lrbp->ucd_rsp_dma_addr);
		ufshcd_hex_dump("UPIU RSP: ", lrbp->ucd_rsp_ptr,
				sizeof(struct utp_upiu_rsp));

		if (ufshcd_is_hisi_ufs_hc(hba))
			prdt_length = le16_to_cpu(lrbp->hisi_utr_descriptor_ptr
							  ->prd_table_length);
		else
			prdt_length = le16_to_cpu(
				lrbp->utr_descriptor_ptr->prd_table_length);
		dev_err(hba->dev,
			"UPIU[%d] - PRDT - %d entries  phys@0x%llx\n",
			tag, prdt_length,
			(u64)lrbp->ucd_prdt_dma_addr);

#ifdef CONFIG_UFSHCD_DUMP_PRDT
		if (pr_prdt)
			ufshcd_hex_dump("UPIU PRDT: ", lrbp->ucd_prdt_ptr,
				sizeof(struct ufshcd_sg_entry) * prdt_length);
#endif
	}
}

static void ufshcd_print_tmrs(struct ufs_hba *hba, unsigned long bitmap)
{
	struct utp_task_req_desc *tmrdp;
	int tag;

	for_each_set_bit(tag, &bitmap, hba->nutmrs) {
		tmrdp = &hba->utmrdl_base_addr[tag];
		dev_err(hba->dev, "TM[%d] - Task Management Header\n", tag);
		ufshcd_hex_dump("TM TRD: ", &tmrdp->header,
				sizeof(struct request_desc_header));
		dev_err(hba->dev, "TM[%d] - Task Management Request UPIU\n",
				tag);
		ufshcd_hex_dump("TM REQ: ", tmrdp->task_req_upiu,
				sizeof(struct utp_upiu_req));
		dev_err(hba->dev, "TM[%d] - Task Management Response UPIU\n",
				tag);
		ufshcd_hex_dump("TM RSP: ", tmrdp->task_rsp_upiu,
				sizeof(struct utp_task_req_desc));
	}
}
#else
static
void ufshcd_print_trs(struct ufs_hba *hba, unsigned long bitmap, bool pr_prdt)
{
	return;
}

static void ufshcd_print_tmrs(struct ufs_hba *hba, unsigned long bitmap)
{
	return;
}
#endif
static void ufshcd_print_host_state(struct ufs_hba *hba)
{
	dev_err(hba->dev, "UFS Host state=%d\n", hba->ufshcd_state);
	dev_err(hba->dev, "lrb in use=0x%lx, outstanding reqs=0x%lx tasks=0x%lx\n",
		hba->lrb_in_use, hba->outstanding_reqs, hba->outstanding_tasks);
	dev_err(hba->dev, "saved_err=0x%x, saved_uic_err=0x%x\n",
		hba->saved_err, hba->saved_uic_err);
	dev_err(hba->dev, "Device power mode=%d, UIC link state=%d\n",
		hba->curr_dev_pwr_mode, hba->uic_link_state);
	dev_err(hba->dev, "PM in progress=%d, sys. suspended=%d\n",
		hba->pm_op_in_progress, hba->is_sys_suspended);
	dev_err(hba->dev, "Auto BKOPS=%d, Host self-block=%d\n",
		hba->auto_bkops_enabled, hba->host->host_self_blocked);
	dev_err(hba->dev, "Clk gate=%d\n", hba->clk_gating.state);
	dev_err(hba->dev, "error handling flags=0x%x, req. abort count=%d\n",
		hba->eh_flags, hba->req_abort_count);
	dev_err(hba->dev, "Host capabilities=0x%x, caps=0x%x\n",
		hba->capabilities, hba->caps);
	dev_err(hba->dev, "quirks=0x%x, dev. quirks=0x%x\n", hba->quirks,
		hba->dev_quirks);
}

/**
 * ufshcd_print_pwr_info - print power params as saved in hba
 * power info
 * @hba: per-adapter instance
 */
static void ufshcd_print_pwr_info(struct ufs_hba *hba)
{
	static const char * const names[] = {
		"INVALID MODE",
		"FAST MODE",
		"SLOW_MODE",
		"INVALID MODE",
		"FASTAUTO_MODE",
		"SLOWAUTO_MODE",
		"INVALID MODE",
	};

	dev_err(hba->dev, "%s:[RX, TX]: gear=[%d, %d], lane[%d, %d], pwr[%s, %s], rate = %d\n",
		 __func__,
		 hba->pwr_info.gear_rx, hba->pwr_info.gear_tx,
		 hba->pwr_info.lane_rx, hba->pwr_info.lane_tx,
		 names[hba->pwr_info.pwr_rx],
		 names[hba->pwr_info.pwr_tx],
		 hba->pwr_info.hs_rate);
}

static void ufshcd_enable_auto_hibern8(struct ufs_hba *hba)
{
	unsigned long flags;

	spin_lock_irqsave(hba->host->host_lock, flags);
	if (hba->autoh8_disable_depth > 0) {
		hba->autoh8_disable_depth--;
	} else {
		dev_err(hba->dev, "unblance auto hibern8 enabled\n");
		HISI_UFS_BUG(); /*lint !e146*/
	}
	if (hba->autoh8_disable_depth == 0) {
		ufshcd_writel(hba, hba->ahit, REG_CONTROLLER_AHIT);
		if (ufshcd_is_hisi_ufs_hc(hba) && hba->vops &&
			hba->vops->ufshcd_hisi_enable_auto_hibern8) {
			hba->vops->ufshcd_hisi_enable_auto_hibern8(hba);
		}
	}
	spin_unlock_irqrestore(hba->host->host_lock, flags);
}

static void ufshcd_disable_auto_hibern8(struct ufs_hba *hba)
{
	unsigned long flags;


	spin_lock_irqsave(hba->host->host_lock, flags);
	if (hba->autoh8_disable_depth > 0) {
		hba->autoh8_disable_depth++;
	} else {
		hba->autoh8_disable_depth++;
		ufshcd_writel(hba, 0, REG_CONTROLLER_AHIT);

		if (ufshcd_is_hisi_ufs_hc(hba) && hba->vops &&
			hba->vops->ufshcd_hisi_disable_auto_hibern8) {
			hba->vops->ufshcd_hisi_disable_auto_hibern8(hba);
		}
	}
	spin_unlock_irqrestore(hba->host->host_lock, flags);
}

void ufshcd_set_auto_hibern8_delay(struct ufs_hba *hba, unsigned int value)
{
	unsigned long flags;

	spin_lock_irqsave(hba->host->host_lock, flags);
	hba->ahit = value;
	if (hba->autoh8_disable_depth == 0)
		ufshcd_writel(hba, hba->ahit, REG_CONTROLLER_AHIT);
	spin_unlock_irqrestore(hba->host->host_lock, flags);
}

static void ufshcd_enable_pwm_cnt(struct ufs_hba *hba)
{
	unsigned long flags;
#ifndef CONFIG_HISI_UFS_HC
	struct ufs_kirin_host *host = (struct ufs_kirin_host *)hba->priv;
#endif

	if (!(hba->caps & UFSHCD_CAP_PWM_DAEMON_INTR))
		return;

	spin_lock_irqsave(hba->host->host_lock, flags);
#ifdef CONFIG_HISI_UFS_HC
	ufshcd_hisi_enable_unipro_intr(hba, HSH8ENT_LR_INTR);
#else
	ufs_sys_ctrl_writel(host, 1000 * 1000, UFS_PWM_COUNTER);
	ufs_sys_ctrl_clr_bits(host, BIT_UFS_PWM_CNT_INT_MASK, UFS_DEBUG_CTRL);
	ufs_sys_ctrl_set_bits(host, BIT_UFS_PWM_CNT_EN, UFS_DEBUG_CTRL);
#endif
	spin_unlock_irqrestore(hba->host->host_lock, flags);
}

void __ufshcd_disable_pwm_cnt(struct ufs_hba *hba)
{
#ifdef CONFIG_HISI_UFS_HC
	ufshcd_hisi_disable_unipro_intr(hba, HSH8ENT_LR_INTR);
#else
	struct ufs_kirin_host *host = (struct ufs_kirin_host *)hba->priv;

	ufs_sys_ctrl_clr_bits(host, BIT_UFS_PWM_CNT_EN, UFS_DEBUG_CTRL);
	ufs_sys_ctrl_set_bits(host, BIT_UFS_PWM_CNT_INT_MASK, UFS_DEBUG_CTRL);
	ufs_sys_ctrl_writel(host, 1, UFS_PWM_COUNTER_CLR);
#endif
}

static void ufshcd_disable_pwm_cnt(struct ufs_hba *hba)
{
	unsigned long flags;

	if (!(hba->caps & UFSHCD_CAP_PWM_DAEMON_INTR))
		return;

	spin_lock_irqsave(hba->host->host_lock, flags);
	__ufshcd_disable_pwm_cnt(hba);
	spin_unlock_irqrestore(hba->host->host_lock, flags);
}

static void __ufshcd_enable_dev_tmt_cnt(struct ufs_hba *hba)
{

	if (!(hba->caps & UFSHCD_CAP_DEV_TMT_INTR))
		return;

	if (hba->dev_tmt_disable_depth > 0) {
		hba->dev_tmt_disable_depth--;
	} else {
		dev_err(hba->dev, "unblance device timeout intr enabled\n");
		HISI_UFS_BUG(); /*lint !e146*/
	}

	if (hba->dev_tmt_disable_depth == 0) {
#ifdef CONFIG_HISI_UFS_HC
		u32 reg_val = 0;

		reg_val = ufshcd_readl(hba, UFS_PROC_MODE_CFG);
		reg_val |= CFG_RX_CPORT_IDLE_CHK_EN;

		/* set dev_tmt timeout trsh, 0xC8(200) mean 200 * 10ms */
		reg_val &= ~CFG_RX_CPORT_IDLE_TIMEOUT_TRSH;
		reg_val |= (DEV_TMT_VAL << BIT_SHIFT_DEV_TMT_TRSH);
		ufshcd_writel(hba, reg_val, UFS_PROC_MODE_CFG);
		/* set io timeout 5s */
		reg_val = IO_TIMEOUT_TRSH_VAL & MASK_CFG_IO_TIMEOUT_TRSH;
		reg_val |= MASK_CFG_IO_TIMEOUT_CHECK_EN;
		ufshcd_writel(hba, reg_val, UFS_IO_TIMEOUT_CHECK);
#else
		struct ufs_kirin_host *host =
			(struct ufs_kirin_host *)hba->priv;
		/* most of all, one request can not execute more than 2s */
		ufs_sys_ctrl_writel(host, 2000 * 1000, UFS_DEV_TMT_COUNTER);
		ufs_sys_ctrl_set_bits(
			host, BIT_UFS_DEV_TMT_CNT_EN, UFS_DEBUG_CTRL);
		ufs_sys_ctrl_clr_bits(
			host, BIT_UFS_DEV_TMT_CNT_MASK, UFS_DEBUG_CTRL);
#endif
	}
}

static void ufshcd_enable_dev_tmt_cnt(struct ufs_hba *hba)
{
	unsigned long flags;

	if (!(hba->caps & UFSHCD_CAP_DEV_TMT_INTR))
		return;

	spin_lock_irqsave(hba->host->host_lock, flags);
	__ufshcd_enable_dev_tmt_cnt(hba);
	spin_unlock_irqrestore(hba->host->host_lock, flags);
}

void __ufshcd_disable_dev_tmt_cnt(struct ufs_hba *hba)
{

#ifdef CONFIG_HISI_UFS_HC
	u32 reg_val = 0;

	reg_val = ufshcd_readl(hba, UFS_PROC_MODE_CFG);
	reg_val &= ~CFG_RX_CPORT_IDLE_CHK_EN;
	ufshcd_writel(hba, reg_val, UFS_PROC_MODE_CFG);

	reg_val = ufshcd_readl(hba, UFS_IO_TIMEOUT_CHECK);
	reg_val &= ~MASK_CFG_IO_TIMEOUT_CHECK_EN;
	ufshcd_writel(hba, reg_val, UFS_IO_TIMEOUT_CHECK);
#else
	struct ufs_kirin_host *host = (struct ufs_kirin_host *)hba->priv;
	ufs_sys_ctrl_clr_bits(host, BIT_UFS_DEV_TMT_CNT_EN, UFS_DEBUG_CTRL);
	ufs_sys_ctrl_set_bits(host, BIT_UFS_DEV_TMT_CNT_MASK, UFS_DEBUG_CTRL);
	ufs_sys_ctrl_writel(host, 1, UFS_DEV_TMT_COUNTER_CLR);
#endif
}

static void ufshcd_disable_dev_tmt_cnt_no_irq(struct ufs_hba *hba)
{
	if (hba->dev_tmt_disable_depth > 0) {
		hba->dev_tmt_disable_depth++;
		return;
	}
	hba->dev_tmt_disable_depth++;
	__ufshcd_disable_dev_tmt_cnt(hba);
}

static void ufshcd_disable_dev_tmt_cnt(struct ufs_hba *hba)
{
	unsigned long flags;

	if (!(hba->caps & UFSHCD_CAP_DEV_TMT_INTR))
		return;

	spin_lock_irqsave(hba->host->host_lock, flags);
	ufshcd_disable_dev_tmt_cnt_no_irq(hba);
	spin_unlock_irqrestore(hba->host->host_lock, flags);
}

void ufshcd_disable_run_stop_reg(struct ufs_hba *hba)
{
	ufshcd_writel(hba, 0, REG_UTP_TASK_REQ_LIST_RUN_STOP);
#ifdef CONFIG_HISI_UFS_HC_CORE_UTR
	ufshcd_disable_core_run_stop_reg(hba);
#else
	ufshcd_writel(hba, 0, REG_UTP_TRANSFER_REQ_LIST_RUN_STOP);
#endif
}
/**
 * ufshcd_support_inline_encrypt - Check if controller supports
 *                            UFS inline encrypt
 * @hba: per adapter instance
 */
#ifdef CONFIG_SCSI_UFS_INLINE_CRYPTO
bool ufshcd_support_inline_encrypt(struct ufs_hba *hba)
{
	if (hba->capabilities & MASK_INLINE_ENCRYPTO_SUPPORT)
		return true;
	else
		return false;
}

static int ufshcd_hba_uie_init(struct ufs_hba *hba)
{
	int err = 0;
	pr_err("ufshcd_hba_uie_init \r\n");

	if (!ufshcd_support_inline_encrypt(hba))
		return 0;

	pr_err("ufshcd_hba_uie_init support_inline_encrypt\r\n");

	if (hba->vops && hba->vops->uie_config_init) {
		err = hba->vops->uie_config_init(hba);
		if (!err)
			hba->host->crypto_enabled = 1;
		pr_err("ufshcd_hba_uie_init support_inline_encrypt 2\r\n");
	}

	return err;
}
#endif

#ifdef CONFIG_HISI_SCSI_UFS_DUMP
static inline void ufshcd_dump_scsi_command(struct ufs_hba *hba, unsigned int task_tag)
{
	if (unlikely(ufs_dump)) {
		switch ((int)hba->lrb[task_tag].cmd->cmnd[0]) {
		case 0x28:
			printk(KERN_DEBUG "ufs read10 addr:%d, length:%d\n",
				(int)hba->lrb[task_tag].cmd->cmnd[2] * 0x1000000 +
				(int)hba->lrb[task_tag].cmd->cmnd[3] * 0x10000 +
				(int)hba->lrb[task_tag].cmd->cmnd[4] * 0x100 +
				(int)hba->lrb[task_tag].cmd->cmnd[5] * 0x1,
				(int)hba->lrb[task_tag].cmd->cmnd[7] * 0x100 +
				(int)hba->lrb[task_tag].cmd->cmnd[8] * 0x1);
			break;
		case 0x2A:
			printk(KERN_DEBUG "ufs write10 addr:%d, length:%d\n",
				(int)hba->lrb[task_tag].cmd->cmnd[2] * 0x1000000 +
				(int)hba->lrb[task_tag].cmd->cmnd[3] * 0x10000 +
				(int)hba->lrb[task_tag].cmd->cmnd[4] * 0x100 +
				(int)hba->lrb[task_tag].cmd->cmnd[5] * 0x1,
				(int)hba->lrb[task_tag].cmd->cmnd[7] * 0x100 +
				(int)hba->lrb[task_tag].cmd->cmnd[8] * 0x1);
			break;
		case 0x35:
			printk(KERN_DEBUG "ufs sync10 addr:%d, block num:%d\n",
				(int)hba->lrb[task_tag].cmd->cmnd[2] * 0x1000000 +
				(int)hba->lrb[task_tag].cmd->cmnd[3] * 0x10000 +
				(int)hba->lrb[task_tag].cmd->cmnd[4] * 0x100 +
				(int)hba->lrb[task_tag].cmd->cmnd[5] * 0x1,
				(int)hba->lrb[task_tag].cmd->cmnd[7] * 0x100 +
				(int)hba->lrb[task_tag].cmd->cmnd[8] * 0x1);
			break;
		case 0x42:
			printk(KERN_DEBUG "ufs unmap(discard)\n");
			break;
		default:
			printk(KERN_DEBUG "ufs cmd (0x%x)\n",
				hba->lrb[task_tag].cmd->cmnd[0]);
			break;
		}
	}
}
#endif

/**
 * ufshcd_prepare_req_desc_uie() - fills the utp_transfer_req_desc,
 * for UFS inline encrypt func
 * @hba: UFS hba
 * @lrbp: local reference block pointer
 */
#ifdef CONFIG_SCSI_UFS_INLINE_CRYPTO
static
void ufshcd_prepare_req_desc_uie(struct ufs_hba *hba, struct ufshcd_lrb *lrbp)
{
	if (ufshcd_support_inline_encrypt(hba)) {
		if (hba->vops && hba->vops->uie_utrd_pre)
			hba->vops->uie_utrd_pre(hba, lrbp);
	}
}
#endif

static void ufshpb_compose_upiu(struct ufs_hba *hba, struct ufshcd_lrb *lrbp)
{
#ifdef HPB_HISI_DEBUG_PRINT
	lrbp->find_time = 0;
	lrbp->start_time = ktime_get();
#endif

	if (hba->ufs_hpb && (hba->ufs_hpb->ufshpb_state == UFSHPB_PRESENT)) {
#ifdef HPB_HISI_DEBUG_PRINT
		pr_err("%s, start use ufshpb\n", __func__);
#endif
		if (hba->manufacturer_id == UFS_VENDOR_SAMSUNG)
			ufshpb_prep_for_dev_mode(hba, lrbp);
		else
			ufshpb_prep_fn_condense(hba, lrbp);
	}
}

/**
 * ufshcd_compose_upiu - form UFS Protocol Information Unit(UPIU)
 * @hba - per adapter instance
 * @lrb - pointer to local reference block
 */
int ufshcd_compose_upiu(struct ufs_hba *hba, struct ufshcd_lrb *lrbp)
{
	u32 upiu_flags = 0;
	int ret = 0;

	switch (lrbp->command_type) {
	case UTP_CMD_TYPE_SCSI:
		if (likely(lrbp->cmd)) {
			ufshcd_prepare_req_desc_hdr(lrbp, &upiu_flags,
				lrbp->cmd->sc_data_direction);
			ufshcd_prepare_utp_scsi_cmd_upiu(lrbp, upiu_flags);
#ifdef CONFIG_SCSI_UFS_INLINE_CRYPTO
			ufshcd_prepare_req_desc_uie(hba, lrbp);
#endif
			ufshpb_compose_upiu(hba, lrbp);
		} else {
			ret = -EINVAL;
		}
		break;
	case UTP_CMD_TYPE_DEV_MANAGE:
		ufshcd_prepare_req_desc_hdr(lrbp, &upiu_flags, DMA_NONE);
		if (hba->dev_cmd.type == DEV_CMD_TYPE_QUERY)
			ufshcd_prepare_utp_query_req_upiu(
					hba, lrbp, upiu_flags);
		else if (hba->dev_cmd.type == DEV_CMD_TYPE_NOP)
			ufshcd_prepare_utp_nop_upiu(lrbp);
		else
			ret = -EINVAL;
		break;
	case UTP_CMD_TYPE_UFS:
		/* For UFS native command implementation */
		ret = -ENOTSUPP;
		dev_err(hba->dev, "%s: UFS native command are not supported\n",
			__func__);
		break;
	default:
		ret = -ENOTSUPP;
		dev_err(hba->dev, "%s: unknown command type: 0x%x\n",
				__func__, lrbp->command_type);
		break;
	} /* end of switch */

	return ret;
}
EXPORT_SYMBOL(ufshcd_compose_upiu);

#ifdef CONFIG_SCSI_UFS_HS_ERROR_RECOVER
static void ufs_recover_hs_mode_reinit(struct ufs_hba *hba)
{
	unsigned long flags;

	spin_lock_irqsave(hba->host->host_lock, flags);
	hba->ufshcd_state = UFSHCD_STATE_EH_SCHEDULED;
	hba->force_host_reset = true;
	if (!kthread_queue_work(&hba->eh_worker, &hba->eh_work))
		dev_err(hba->dev, "%s: queue hba->eh_worker\n", __func__);
	hba->disable_suspend = 0;
	spin_unlock_irqrestore(hba->host->host_lock, flags);
	pm_runtime_put_sync(hba->dev);
	__pm_relax(&hba->recover_wake_lock);
}

void ufs_recover_hs_mode(struct work_struct *work)
{
	unsigned long flags;
	u32 tm_doorbell;
#ifdef CONFIG_HISI_UFS_HC_CORE_UTR
	u64 tr_doorbell;
#else
	u32 tr_doorbell;
#endif
	struct ufs_hba *hba;
	int ret;
	unsigned long timeout;

	hba = container_of(work, struct ufs_hba, recover_hs_work);

	__pm_stay_awake(&hba->recover_wake_lock);
	ret = pm_runtime_get_sync(hba->dev);
	if (ret < 0) {
		pm_runtime_put_sync(hba->dev);
		goto wake_unlock;
	}

	/* block request*/
	scsi_block_requests(hba->host);
	/* confirm no request */
	timeout = jiffies + msecs_to_jiffies(DOORBELL_TIMEOUT_MS);
	do {
		if (time_after(jiffies, timeout)) {
			dev_err(hba->dev, "%s: wait ufs host free timeout, lrb_in_use 0x%lx\n",
					__func__, hba->lrb_in_use);
			break;
		}
		spin_lock_irqsave(hba->host->host_lock, flags);
		if (hba->is_sys_suspended || hba->pm_op_in_progress) {
			spin_unlock_irqrestore(hba->host->host_lock, flags);
			msleep(5);
			continue;
		}
		tm_doorbell = ufshcd_readl(hba, REG_UTP_TASK_REQ_DOOR_BELL);
#ifdef CONFIG_HISI_UFS_HC_CORE_UTR
		tr_doorbell = read_core_utr_doorbells(hba);
#else
		tr_doorbell = ufshcd_readl(hba, REG_UTP_TRANSFER_REQ_DOOR_BELL);
#endif
		if (hba->outstanding_reqs || tm_doorbell || tr_doorbell) {
			spin_unlock_irqrestore(hba->host->host_lock, flags);
			msleep(5);
			continue;
		} else {
			spin_unlock_irqrestore(hba->host->host_lock, flags);
			break;
		}
	} while(1);

	spin_lock_irqsave(hba->host->host_lock, flags);
	hba->hs_single_lane = 0;
	hba->use_pwm_mode = 0;
	spin_unlock_irqrestore(hba->host->host_lock, flags);
	ret = ufshcd_get_max_pwr_mode(hba);
	if (ret) {
		dev_err(hba->dev,
			"%s: Failed getting max supported power mode\n",
			__func__);
		goto reinit;
	}

	ret = ufshcd_config_pwr_mode(hba, &hba->max_pwr_info.info);
	if (ret) {
		dev_err(hba->dev, "%s: Failed setting power mode, err = %d\n",
				__func__, ret);
		goto reinit;
	}

	/* confirm ufs works well after switch to hs mode */
	ret = ufshcd_verify_dev_init(hba, 1);
	if (ret) {
		dev_err(hba->dev, "%s: Failed nop out, err = %d\n",
				__func__, ret);
		goto reinit;
	}

	hba->disable_suspend = 0;
	pm_runtime_allow(hba->dev);
	dev_err(hba->dev, "enable ufs pm runtime in hs\n");
	scsi_unblock_requests(hba->host);
	pm_runtime_put_sync(hba->dev);
wake_unlock:
	__pm_relax(&hba->recover_wake_lock);
	return;
reinit:
	ufs_recover_hs_mode_reinit(hba);
	return;
}
#endif /* CONFIG_SCSI_UFS_HS_ERROR_RECOVER */

#ifdef CONFIG_SCSI_UFS_HI1861_VCMD
void ufshcd_fsr_dump_handler(struct work_struct *work)
{
	struct ufs_hba *hba;
	u8 *fbuf = NULL;
	int i = 0;
	int ret = 0;

	hba = container_of(work, struct ufs_hba, fsr_work);

	if (UFS_VENDOR_HI1861 != hba->manufacturer_id)
		return;
	/* allocate memory to hold full descriptor */
	fbuf = kmalloc(HI1861_FSR_INFO_SIZE, GFP_KERNEL);
	if (!fbuf) {
		return;

	}
	memset(fbuf, 0, HI1861_FSR_INFO_SIZE);

	ret = ufshcd_read_fsr(hba, fbuf, HI1861_FSR_INFO_SIZE);
	if (ret) {
		kfree(fbuf);
		dev_err(hba->dev, "[%s]READ FSR FAILED\n", __func__);
		return;
	}
#ifdef CONFIG_HISI_DEBUG_FS
	dev_err(hba->dev, "===============UFS HI1861 FSR INFO===============\n");
#endif
	/*lint -save -e661 -e662*/
	for (i = 0 ; i < HI1861_FSR_INFO_SIZE; i = i + 16) {
		dev_err(hba->dev, "0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n",
		*(fbuf + i + 0), *(fbuf + i + 1), *(fbuf + i + 2), *(fbuf + i + 3),
		*(fbuf + i + 4), *(fbuf + i + 5), *(fbuf + i + 6), *(fbuf + i + 7),
		*(fbuf + i + 8), *(fbuf + i + 9), *(fbuf + i + 10), *(fbuf + i + 11),
		*(fbuf + i + 12), *(fbuf + i + 13), *(fbuf + i + 14), *(fbuf + i + 15));
	}
	/*lint -restore*/
	kfree(fbuf);
}
#endif

/*lint -restore*/
/**
 * ufs_ffu_pm_runtime_delay_enable - when ffu_pm_work in workqueue is scheduled, after 30 allow pm_runtime
 * and unlock the wake lock
* @work: pointer to work structure
 *
 */
void ufs_ffu_pm_runtime_delay_enable(struct work_struct *work)
{
	struct ufs_hba *hba;
	int err;
	unsigned long flags;

	/*lint -e826*/
	hba = container_of(work, struct ufs_hba, ffu_pm_work);
	if (!hba) {
		pr_err( "%s hba get error\n", __func__);
		return;
	}
	/*lint -e826*/
	/* follow scsi command timeout value 2s */
	msleep(2000);
	mutex_lock(&hba->eh_mutex);
	spin_lock_irqsave(hba->host->host_lock, flags);
	hba->ufshcd_state = UFSHCD_STATE_RESET;
	ufshcd_set_eh_in_progress(hba);
	spin_unlock_irqrestore(hba->host->host_lock, flags);

	err = ufshcd_reset_and_restore(hba);

	spin_lock_irqsave(hba->host->host_lock, flags);
	if (!err)
		hba->ufshcd_state = UFSHCD_STATE_OPERATIONAL;
	else
		hba->ufshcd_state = UFSHCD_STATE_ERROR;
	ufshcd_clear_eh_in_progress(hba);
	spin_unlock_irqrestore(hba->host->host_lock, flags);

	pm_runtime_set_active(hba->dev);
	mutex_unlock(&hba->eh_mutex);
	/* wait 27s, this msleep does not effect ffu reset time */
	msleep(27000);

	if (ffu_lock.active) {
		/*lint -save -e455*/
		__pm_relax(&ffu_lock);
		/*lint -restore*/
		dev_err(hba->dev, "ffu unlock wake lock.\n");
	}
	if (ufshcd_is_auto_hibern8_allowed(hba))
		ufshcd_enable_auto_hibern8(hba);
	else
		pm_runtime_allow(hba->dev);
}
/**
 * ufs_ffu_pm_runtime_delay_process - ffu-write buffer request issue, forbid pm_runtime
 * and lock the wake lock, forbid system suspend.ffu work in queue is scheduled
 * @hba: pointer to adapter instance
 *
 */
/*lint -save -e456 -e454*/
void ufs_ffu_pm_runtime_delay_process(struct ufs_hba *hba)
{
	if (ufshcd_is_auto_hibern8_allowed(hba))
		ufshcd_disable_auto_hibern8(hba);
	else
		pm_runtime_forbid(hba->dev);
	if (!ffu_lock.active) {
		__pm_stay_awake(&ffu_lock);
		dev_err(hba->dev, "ffu lock wake lock.\n");
	}
	schedule_work(&hba->ffu_pm_work);
}

static inline void ufshcd_start_delay_work(struct ufs_hba *hba,
		struct scsi_cmnd *cmd)
{
	if (likely(!hba->host->is_emulator)) {

		/* rpmb request issue, and pm_runtime delay time reset longer,
		 * after 1 second pm_runtime delay time recover
		 */
		if (((UFS_UPIU_RPMB_WLUN & ~UFS_UPIU_WLUN_ID) | SCSI_W_LUN_BASE)
		    == cmd->device->lun) {
			ufs_rpmb_pm_runtime_delay_process(hba);
			g_rpmb_ufs_start_time = hisi_getcurtime();
		}
		/* UFS supports only the MODE field value 0Eh: Download
		 * microcode with offsets, save, and defer active
		 */
		if (unlikely((cmd->cmnd[0] == WRITE_BUFFER) &&
				((cmd->cmnd[1] & WB_MODE_MASK) == DOWNLOAD_MODE)))
			ufs_ffu_pm_runtime_delay_process(hba);

	}
}

static void ufshcd_check_disable_dev_tmt_cnt(struct ufs_hba *hba,
					     struct scsi_cmnd *cmd)
{
	if ((unlikely((cmd->cmnd[0] == WRITE_BUFFER) &&
		      ((cmd->cmnd[1] & WB_MODE_MASK) == DOWNLOAD_MODE))) ||
	    (cmd->cmnd[0] == UNMAP))
		ufshcd_disable_dev_tmt_cnt_no_irq(hba);
}

int ufshcd_query_attr_safe(struct ufs_hba *hba, enum query_opcode opcode,
	enum attr_idn idn, u8 index, u8 selector, u32 *attr_val)
{
	int err;
	unsigned long flags;

	spin_lock_irqsave(hba->host->host_lock, flags);
	if (hba->ufshcd_state != UFSHCD_STATE_OPERATIONAL) {
		spin_unlock_irqrestore(hba->host->host_lock, flags);
		return -EBUSY;
	}
	spin_unlock_irqrestore(hba->host->host_lock, flags);

	/* hold error handler, reset may cause send command a noc error */
	mutex_lock(&hba->eh_mutex);

	err = ufshcd_query_attr(hba, opcode, idn, index, selector, attr_val);

	mutex_unlock(&hba->eh_mutex);
	return err;
}

#ifdef CONFIG_SCSI_UFS_HI1861_VCMD
static int __ufshcd_query_vcmd(struct ufs_hba *hba, enum query_opcode opcode,
	u8 idn, u8 index, u8 selector, u8 *desc_buf, int *buf_len)
{
	struct ufs_query_req *request = NULL;
	struct ufs_query_res *response = NULL;
	int err;

	BUG_ON(!hba);

	if (*buf_len > HI1861_FSR_INFO_SIZE) {
		dev_err(hba->dev, "%s: desc fsr buffer size (%d) is out of range\n",
				__func__, *buf_len);
		err = -EINVAL;
		goto out;
	}
	mutex_lock(&hba->dev_cmd.lock);
	ufshcd_init_query(hba, &request, &response, opcode, idn, index,
			selector);
	hba->dev_cmd.query.descriptor = desc_buf;
	request->upiu_req.length = cpu_to_be16(*buf_len);

	switch (opcode) {
	case UPIU_QUERY_OPCODE_READ_HI1861_FSR:
	case UPIU_QUERY_OPCODE_READ_TZ_DESC:
		request->query_func = UPIU_QUERY_FUNC_STANDARD_READ_REQUEST;
		break;
	case UPIU_QUERY_OPCODE_TZ_CTRL:
		request->query_func = UPIU_QUERY_FUNC_STANDARD_WRITE_REQUEST;
		break;
	case UPIU_QUERY_OPCODE_VENDOR_WRITE:
		request->query_func = UPIU_QUERY_FUNC_STANDARD_WRITE_REQUEST;
		if (idn == SET_TZ_STREAM_ID ||
		    idn == QUERY_VENDOR_WRITE_IDN_PGR)
			request->has_data = true;
		break;
	case UPIU_QUERY_OPCODE_VENDOR_READ:
		request->query_func = UPIU_QUERY_FUNC_STANDARD_READ_REQUEST;
		if (idn == (enum desc_idn)QUERY_TZ_IDN_BLK_INFO) {
			request->upiu_req.value = get_unaligned_be32(desc_buf);
			request->lun = desc_buf[sizeof(u32)];
		}
		break;
	default:
		dev_err(hba->dev,
				"%s: Expected query fsr desc opcode but got = 0x%.2x\n",
				__func__, opcode);
		err = -EINVAL;
		goto out_unlock;
	}
	err = ufshcd_exec_dev_cmd(hba, DEV_CMD_TYPE_QUERY, QUERY_REQ_TIMEOUT);

	if (err) {
		dev_err(hba->dev, "%s: opcode 0x%.2x for idn 0x%x failed, err = %d\n",
				__func__, opcode, idn, err);
		goto out_unlock;
	}

	hba->dev_cmd.query.descriptor = NULL;
	*buf_len = be16_to_cpu(response->upiu_res.length);

out_unlock:
	mutex_unlock(&hba->dev_cmd.lock);
out:

	return err;
}

/**
* ufshcd_query_vcmd_retry -query devvice desc for vcmd info
* @hba: Pointer to adapter instance
* @opcode: the query opcode to vcmd info
* @idn:the address setting to device desc vcmd
* @index:the address setting to device desc vcmd
* @selector:the address setting to device desc vcmd
* @desc_buf:the buf for get vcmd
* @buf_len:the buf size, just like 4k byte
*/
int ufshcd_query_vcmd_retry(struct ufs_hba *hba, enum query_opcode opcode,
			    u8 idn, u8 index, u8 selector, u8 *desc_buf,
			    int *buf_len)
{
	int err = 0;
	int retries;

	for (retries = QUERY_REQ_RETRIES; retries > 0; retries--) {
		err = __ufshcd_query_vcmd(hba, opcode, idn, index, selector,
					  desc_buf, buf_len);
		if (!err || err == -EINVAL) {
			break;
		}
	}

	return err;
}
EXPORT_SYMBOL(ufshcd_query_vcmd_retry);

static int ufshcd_read_fsr_info(struct ufs_hba *hba,
				u8 desc_id,
				int desc_index,
				u8 *param_read_buf,
				u32 param_size)
{
	int ret;
	u8 *desc_buf = NULL;
	int buff_len;

	if (UFS_VENDOR_HI1861 != hba->manufacturer_id)
		return -EINVAL;
	/* safety checks */
	if (desc_id >= QUERY_DESC_IDN_MAX)
		return -EINVAL;
	if (!param_read_buf)
		return -ENOMEM;
	if (!param_size)
		return -EINVAL;
	else
		buff_len = param_size;
	/* allocate memory to hold full descriptor */
	desc_buf = kmalloc(buff_len, GFP_KERNEL);
	if (!desc_buf)
		return -ENOMEM;
	memset(desc_buf, 0, buff_len);

	ret = ufshcd_query_vcmd_retry(hba, UPIU_QUERY_OPCODE_READ_HI1861_FSR,
				      desc_id, desc_index, 0, desc_buf,
				      &buff_len);
	if (ret) {
		dev_err(hba->dev, "%s: Failed reading FSR. desc_id %d "
				  "buff_len %d ret %d",
			__func__, desc_id, buff_len, ret);

		goto out;
	}
	memcpy(param_read_buf, desc_buf, buff_len);
out:
	kfree(desc_buf);

	return ret;
}

int ufshcd_read_fsr(struct ufs_hba *hba, u8 *buf, u32 size)
{
	return ufshcd_read_fsr_info(hba, 0, 0, buf, size);
}
/**
* ufshcd_get_fsr_command -scsi layer get fsr func
* @hba: Pointer to adapter instance
* @buf: the buf pointer to fsr info
* @size:the buf size, just like 4k byte
*/
int ufshcd_get_fsr_command(struct ufs_hba *hba, u8 *buf,
				u32 size)
{
	if (!hba) {
		pr_err( "%s shost_priv host failed\n", __func__);
		return -1;
	}
	return ufshcd_read_fsr_info(hba, (enum desc_idn)0, 0, buf, size);
}
EXPORT_SYMBOL(ufshcd_get_fsr_command);

static ssize_t ufshcd_fsr_info_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct ufs_hba *hba = dev_get_drvdata(dev);
	/* allocate memory to hold full descriptor */
	u8 *fbuf = NULL;
	int len = 0;
	int i = 0;
	int ret = 0;

	if (UFS_VENDOR_HI1861 != hba->manufacturer_id)
		return 0;
	fbuf = kmalloc(HI1861_FSR_INFO_SIZE, GFP_KERNEL);
	if (!fbuf) {
		return -ENOMEM;
	}
	memset(fbuf, 0, HI1861_FSR_INFO_SIZE);
	ret = ufshcd_read_fsr(hba, fbuf, HI1861_FSR_INFO_SIZE);
	if (ret) {
		kfree(fbuf);
		dev_err(hba->dev, "[%s]READ FSR FAILED\n", __func__);
		return ret;
	}
	/*lint -save -e661 -e662*/
	for (i = 0; i < HI1861_FSR_INFO_SIZE; i = i + 16) {
		len += snprintf(buf + len, PAGE_SIZE - len ,
	"0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n",
		*(fbuf + i + 0), *(fbuf + i + 1), *(fbuf + i + 2), *(fbuf + i + 3),
		*(fbuf + i + 4), *(fbuf + i + 5), *(fbuf + i + 6), *(fbuf + i + 7),
		*(fbuf + i + 8), *(fbuf + i + 9), *(fbuf + i + 10), *(fbuf + i + 11),
		*(fbuf + i + 12), *(fbuf + i + 13), *(fbuf + i + 14), *(fbuf + i + 15));
	}
	/*lint -restore*/
	kfree(fbuf);

	return len;
}

/**
* ufshcd_init_fsr_sys -init fsr attr node
* @hba: Pointer to adapter instance
* the node only to be read
*/
static void ufshcd_init_fsr_sys(struct ufs_hba *hba)
{
	hba->ufs_fsr.fsr_attr.show = ufshcd_fsr_info_show;
	sysfs_attr_init(&hba->ufs_fsr.fsr_attr.attr);
	hba->ufs_fsr.fsr_attr.attr.name = "ufs_fsr";
	hba->ufs_fsr.fsr_attr.attr.mode = S_IRUSR | S_IRGRP;
	if (device_create_file(hba->dev, &hba->ufs_fsr.fsr_attr))
		dev_err(hba->dev, "Failed to create sysfs for ufs fsrs\n");
}

#ifdef CONFIG_HISI_DIEID
static void ufshcd_ufs_set_dieid(struct ufs_hba *hba, struct ufs_dev_desc *dev_desc)
{
	/* allocate memory to hold full descriptor */
	u8 *fbuf = NULL;
	int ret = 0;

	if (hba->manufacturer_id != UFS_VENDOR_HI1861)
		return;

	if (ufs_hixxxx_dieid == NULL)
		ufs_hixxxx_dieid = devm_kzalloc(hba->dev,
			UFS_DIEID_TOTAL_SIZE * sizeof(u8), GFP_KERNEL);
	if (!ufs_hixxxx_dieid)
		return;

	memset(ufs_hixxxx_dieid, 0, UFS_DIEID_TOTAL_SIZE);

	fbuf = kmalloc(HI1861_FSR_INFO_SIZE, GFP_KERNEL);
	if (!fbuf)
		return;

	memset(fbuf, 0, HI1861_FSR_INFO_SIZE);

	ret = ufshcd_read_fsr(hba, fbuf, HI1861_FSR_INFO_SIZE);
	if (ret) {
		is_fsr_read_failed = 1;
		dev_err(hba->dev, "[%s]READ FSR FAILED\n", __func__);
		goto out;
	}

	ret = strncmp(UFS_HIXXXX_PRODUCT_NAME, dev_desc->model, HIXXXX_PROD_LEN);

	if (ret != 0) {
		/* after hi1861 ver. */
		memcpy(ufs_hixxxx_dieid, fbuf + 16, UFS_DIEID_NUM_SIZE_THOR920);
		memcpy(ufs_hixxxx_dieid + UFS_DIEID_CHIP_VER_OFFSET,
			fbuf + 36, UFS_NAND_CHIP_VER_SIZE);
		memcpy(ufs_hixxxx_dieid + UFS_DIEID_CONTROLLER_OFFSET,
			fbuf + 256, UFS_CONTROLLER_DIEID_SIZE);
		memcpy(ufs_hixxxx_dieid + UFS_DIEID_FLASH_OFFSET,
			fbuf + 448, UFS_FLASH_DIE_ID_SIZE);
		memcpy(ufs_hixxxx_dieid + UFS_DCIP_CRACK_NOW_OFFSET,
			fbuf + 440, UFS_DCIP_CRACK_NOW_SIZE);
		memcpy(ufs_hixxxx_dieid + UFS_DCIP_CRACK_EVER_OFFSET,
			fbuf + 441, UFS_DCIP_CRACK_EVER_SIZE);
	} else {
		/* hi1861 ver. */
		memcpy(ufs_hixxxx_dieid, fbuf + 12, UFS_DIEID_NUM_SIZE);
		memcpy(ufs_hixxxx_dieid + UFS_DIEID_CHIP_VER_OFFSET,
			fbuf + 28, UFS_NAND_CHIP_VER_SIZE);
		memcpy(ufs_hixxxx_dieid + UFS_DIEID_CONTROLLER_OFFSET,
			fbuf + 1692, UFS_CONTROLLER_DIEID_SIZE);
		memcpy(ufs_hixxxx_dieid + UFS_DIEID_FLASH_OFFSET,
			fbuf + 1900, UFS_FLASH_DIE_ID_SIZE);
	}
out:
	kfree(fbuf);
}

static int hisi_ufs_get_flash_dieid(
	char *dieid, u32 offset, u32 dieid_num, u8 vendor_id, u32 *flash_id)
{
	int len = 0;
	int i = 0;
	int j = 0;
	int flag = 0;
	int ret = 0;
	dieid += offset;
	/**
	*T vendor flash id, the length is 32B.As is required,
	*the output flash ids need to formatted in hex with appropriate prefix
	*eg:\r\nDIEID_UFS_FLASH_B:0x00CD...\r\n
	*   \r\nDIEID_UFS_FLASH_C:0xAC3D...\r\n
	*/
	/*lint -save -e574 -e679 */
	if (offset > UFS_DIEID_BUFFER_SIZE)
		return -EINVAL;

	if ((vendor_id == UFS_FLASH_VENDOR_T) || (vendor_id == UFS_FLASH_VENDOR_H)) {
		for (i = 0; i < dieid_num; i++) {
			ret = snprintf(dieid + len,/* [false alarm]:<UFS_DIEID_BUFFER_SIZE - len - offset> is normal */
					UFS_DIEID_BUFFER_SIZE - len - offset,
					"\r\nDIEID_UFS_FLASH_%c:0x%08X%08X%08X%08X\r\n",
					'B' + i,
					*(flash_id + i * 4),
					*(flash_id + i * 4 + 1),
					*(flash_id + i * 4 + 2),
					*(flash_id + i * 4 + 3));

			if (ret <= 0)
				return -2;
			len += ret;
		}
	}
	/**
	*M vendor flash id's length is 16B. however,
	*the required length is 32B,so the lower 16B is zero padded
	*eg:\r\nDIEID_UFS_FLASH_B:0x00CD...00000000000...\r\n
	*   \r\nDIEID_UFS_FLASH_C:0xAC3D...00000000000...\r\n
	*/
	else if (vendor_id == UFS_FLASH_VENDOR_M) {
		for (i = 0; i < dieid_num; i++) {
			if (4 == dieid_num && (2 == i || 3 == i)) {
				i += 2;
				flag = 1;
			}

			ret = snprintf(dieid + len,
					UFS_DIEID_BUFFER_SIZE - len - offset,
					"\r\nDIEID_UFS_FLASH_%c:0x%08X%08X%08X%08X00000000000000000000000000000000\r\n",
					'B' + j++,
					*(flash_id + i * 4),
					*(flash_id + i * 4 + 1),
					*(flash_id + i * 4 + 2),
					*(flash_id + i * 4 + 3));

			if (ret <= 0)
				return -2;
			len += ret;

			if (flag) {
				flag = 0;
				i -= 2;
			}
		}
	} else
		return -2;
	/*lint -restore*/

	return 0;
}
#endif
#endif

#ifdef CONFIG_HISI_DIEID
int hisi_ufs_get_dieid(char *dieid, unsigned int len)
{
#ifdef CONFIG_SCSI_UFS_HI1861_VCMD
	int length = 0;
	int ret = 0;
	u32 dieid_num = 0;
	u8 vendor_id = 0;
	u32 *controller_id = NULL;
	u32 *flash_id = NULL;
	u8 dieCrackNow = 0;
	u8 dieCrackEver = 0;
	char buf[UFS_DIEID_BUFFER_SIZE] = {0};

	if (dieid == NULL || ufs_hixxxx_dieid == NULL)
		return -2;
	if (is_fsr_read_failed)
		return -1;

	dieid_num = *(u32 *)ufs_hixxxx_dieid;
	vendor_id = *(u8 *)(ufs_hixxxx_dieid + UFS_DIEID_CHIP_VER_OFFSET);
	controller_id = (u32 *)(ufs_hixxxx_dieid + UFS_DIEID_CONTROLLER_OFFSET);
	flash_id = (u32 *)(ufs_hixxxx_dieid + UFS_DIEID_FLASH_OFFSET);
	dieCrackNow = *(u8 *)(ufs_hixxxx_dieid + UFS_DCIP_CRACK_NOW_OFFSET);
	dieCrackEver = *(u8 *)(ufs_hixxxx_dieid + UFS_DCIP_CRACK_EVER_OFFSET);

	ret = snprintf(buf, UFS_DIEID_BUFFER_SIZE,
			"\r\nDIEID_UFS_CONTROLLER_A:0x%08X%08X%08X%08X%08X%08X%08X%08X\r\n",
			*controller_id, *(controller_id + 1),
			*(controller_id + 2), *(controller_id + 3),
			*(controller_id + 4), *(controller_id + 5),
			*(controller_id + 6), *(controller_id + 7));
	if (ret <= 0)
		return -2;
	length += ret;

	ret = hisi_ufs_get_flash_dieid(
		buf, length, dieid_num, vendor_id, flash_id);
	if (ret != 0)
		return ret;

	length = strlen(buf);

	ret = snprintf(buf + length, UFS_DIEID_BUFFER_SIZE - length,
		"\r\nCRACK_NOW:0x%08X\r\n\r\nCRACK_EVER:0x%08X\r\n",
		dieCrackNow, dieCrackEver);

	if (ret <= 0)
		return -2;

	if (len >= strlen(buf))
		strncat(dieid, buf, strlen(buf));
	else
		return strlen(buf);

	return 0;
#else
	return -1;
#endif
}
#endif

static ssize_t ufshcd_ufs_temp_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	int len = 0;
	struct ufs_hba *hba = dev_get_drvdata(dev);

	hba->ufs_temp.temp = get_soc_temp() / 1000;
	if (hba->ufs_temp.temp <= 0) {
		dsm_ufs_update_error_info(hba, DSM_UFS_TEMP_LOW_ERR);
		schedule_ufs_dsm_work(hba);
	}
	dev_info(hba->dev, "UFS temp is %d\n", hba->ufs_temp.temp);
	len = snprintf(buf, PAGE_SIZE, "%d\n", hba->ufs_temp.temp);
	return len;
}

void ufshcd_init_ufs_temp_sys(struct ufs_hba *hba)
{
	hba->ufs_temp.temp_attr.show = ufshcd_ufs_temp_show;
	sysfs_attr_init(&hba->ufs_temp.temp_attr.attr);
	hba->ufs_temp.temp_attr.attr.name = "ufs_temp";
	hba->ufs_temp.temp_attr.attr.mode = S_IRUSR | S_IRGRP;
	if (device_create_file(hba->dev, &hba->ufs_temp.temp_attr))
		dev_err(hba->dev, "Failed to create sysfs for ufs_temp\n");
}
int ufshcd_read_geometry_desc(struct ufs_hba *hba, u8 *buf, u32 size)
{
		return ufshcd_read_desc(hba, QUERY_DESC_IDN_GEOMETRY, 0, buf, size);
}

int ufshcd_read_device_health_desc(struct ufs_hba *hba,
					u8 *buf, u32 size)
{
		return ufshcd_read_desc(hba, QUERY_DESC_IDN_HEALTH, 0, buf, size);
}

#ifdef CONFIG_HYPERHOLD_CORE
static int ufshcd_get_health_info(struct scsi_device *sdev,
	u8 *pre_eol_info, u8 *life_time_est_a, u8 *life_time_est_b)
{
	int ret;
	struct ufs_hba *hba = NULL;
	u8 buff[QUERY_DESC_HEALTH_MAX_SIZE];

	if ((!sdev) || (!pre_eol_info) || (!life_time_est_a) ||
		(!life_time_est_b))
		return -EFAULT;

	hba = shost_priv(sdev->host);
	if (!hba)
		return -EFAULT;

	ret = ufshcd_read_device_health_desc(hba, buff,
		QUERY_DESC_HEALTH_MAX_SIZE);
	if (ret) {
		dev_err(hba->dev, "%s: Failed getting device health info\n",
			__func__);
		return ret;
	}

	*pre_eol_info = buff[HEALTH_DEVICE_DESC_PARAM_PREEOL];
	*life_time_est_a = buff[HEALTH_DEVICE_DESC_PARAM_LIFETIMEA];
	*life_time_est_b = buff[HEALTH_DEVICE_DESC_PARAM_LIFETIMEB];

	return 0;
}
#endif

static void __ufshcd_print_doorbell(struct ufs_hba *hba, u32 tm_doorbell,
#ifdef CONFIG_HISI_UFS_HC_CORE_UTR
		u64 tr_doorbell, char *s)
#else
		u32 tr_doorbell, char *s)
#endif
{
	dev_err(hba->dev, "wait door bell clean %s:tm_doorbell:0x%x, "
#ifdef CONFIG_HISI_UFS_HC_CORE_UTR
			  "tr_doorbell:0x%lx\n", s, tm_doorbell, tr_doorbell);
#else
			  "tr_doorbell:0x%x\n", s, tm_doorbell, tr_doorbell);
#endif
}
int __ufshcd_wait_for_doorbell_clr(struct ufs_hba *hba)
{
#ifdef CONFIG_HISI_UFS_HC_CORE_UTR
	u32 tm_doorbell, wait_timeout_us;
	u64 tr_doorbell;
#else
	u32 tr_doorbell, tm_doorbell, wait_timeout_us;
#endif
	int ret = 0;
	ktime_t start = ktime_get();

	wait_timeout_us = 2000000;
	tm_doorbell = ufshcd_readl(hba, REG_UTP_TASK_REQ_DOOR_BELL);
#ifdef CONFIG_HISI_UFS_HC_CORE_UTR
	tr_doorbell = read_core_utr_doorbells(hba);
#else
	tr_doorbell = ufshcd_readl(hba, REG_UTP_TRANSFER_REQ_DOOR_BELL);
#endif
	if ((!tm_doorbell) && (!tr_doorbell))
		return 0;

	__ufshcd_print_doorbell(hba, tm_doorbell, tr_doorbell, "begin");
	/*
	 * Wait for all the outstanding tasks/transfer requests.
	 * Verify by checking the doorbell registers are clear.
	 */
	do {
		tm_doorbell = ufshcd_readl(hba, REG_UTP_TASK_REQ_DOOR_BELL);
#ifdef CONFIG_HISI_UFS_HC_CORE_UTR
		tr_doorbell = read_core_utr_doorbells(hba);
#else
		tr_doorbell = ufshcd_readl(hba, REG_UTP_TRANSFER_REQ_DOOR_BELL);
#endif
		/*lint -save -e446*/
		if (ktime_to_us(ktime_sub(ktime_get(), start)) >
			wait_timeout_us) {
			ret = -EIO;
			dev_err(hba->dev, "wait doorbell clean timeout\n");
			hisi_ufs_key_reg_dump(hba);
			break;
		}
		/*lint -restore*/
	} while (tm_doorbell || tr_doorbell);
	__ufshcd_print_doorbell(hba, tm_doorbell, tr_doorbell, "end");
	return ret;
}

int ufshcd_wait_for_doorbell_clr(struct ufs_hba *hba, u64 wait_timeout_us)
{
	unsigned long flags;
	int ret = 0;
	u32 tm_doorbell;
#ifdef CONFIG_HISI_UFS_HC_CORE_UTR
	u64 tr_doorbell;
#else
	u32 tr_doorbell;
#endif
	bool timeout = false, do_last_check = false;
	ktime_t start = ktime_get();

	spin_lock_irqsave(hba->host->host_lock, flags);
	if (hba->ufshcd_state != UFSHCD_STATE_OPERATIONAL) {
		ret = -EBUSY;
		goto out;
	}

	/*
	 * Wait for all the outstanding tasks/transfer requests.
	 * Verify by checking the doorbell registers are clear.
	 */
	do {
		tm_doorbell = ufshcd_readl(hba, REG_UTP_TASK_REQ_DOOR_BELL);
#ifdef CONFIG_HISI_UFS_HC_CORE_UTR
		tr_doorbell = read_core_utr_doorbells(hba);
#else
		tr_doorbell = ufshcd_readl(hba, REG_UTP_TRANSFER_REQ_DOOR_BELL);
#endif
		if (!tm_doorbell && !tr_doorbell) {
			timeout = false;
			break;
		} else if (do_last_check) {
			break;
		}

		spin_unlock_irqrestore(hba->host->host_lock, flags);
		schedule();
		/*lint -save -e446 -e574*/
		if (ktime_to_us(ktime_sub(ktime_get(), start)) >
		    wait_timeout_us) {
			timeout = true;
			do_last_check = true;
		}
		/*lint -restore*/
		spin_lock_irqsave(hba->host->host_lock, flags);
	} while (tm_doorbell || tr_doorbell);

	if (timeout) {
		dev_err(hba->dev,
			"%s: timedout waiting for doorbell to clear (tm=0x%x, tr=0x%lx)\n",
			__func__, tm_doorbell, (u64)tr_doorbell);
		ret = -EBUSY;
	}

out:
	spin_unlock_irqrestore(hba->host->host_lock, flags);
	return ret;
}

#ifndef CONFIG_SCSI_UFS_ENHANCED_INLINE_CRYPTO_V2
#ifdef CONFIG_SCSI_UFS_INLINE_CRYPTO
int ufshcd_keyregs_remap_wc(struct ufs_hba *hba,
	resource_size_t hci_reg_base)
{
	u32 ccap;
	u8 cfgptr;
	u8 cfgc;

	void __iomem *key_reg_base;

	if (ufshcd_support_inline_encrypt(hba)) {
		ccap = ufshcd_readl(hba, 0x100);
		cfgptr = (u8)(ccap >> 24);
		cfgc = (u8)((ccap & 0x0000FF00) >> 8);
		dev_info(hba->dev, "%s: ccap=0x%x, cfgptr=0x%x, cfgc=0x%x\n",
			__func__, ccap, cfgptr, cfgc);

		key_reg_base = ioremap_wc((phys_addr_t)(hci_reg_base + cfgptr * 0x100),
				(size_t)((cfgc + 1) * 0x80));
		if (!key_reg_base) {
			dev_err(hba->dev, "key regs remap error!\n");
			return -ENOMEM;
		}

		hba->key_reg_base = key_reg_base;
	}

	return 0;
}
#endif
#endif

static int __ufshcd_uic_hibern8_op_irq_safe(struct ufs_hba *hba, bool h8_op)
{
	u32 value;
	u32 ie_value;
	int ret;
	u32 mask;
	u8 status;
	/* emulator have no real MPHY */
	if (unlikely(hba->host->is_emulator))
		return 0;
	if (ufshcd_is_hisi_ufs_hc(hba) && hba->vops &&
		hba->vops->ufshcd_hisi_hibern8_op_irq_safe) {
		return hba->vops->ufshcd_hisi_hibern8_op_irq_safe(hba, h8_op);
	}
	/* step 1: close interrupt and save interrupt value */
	ie_value = ufshcd_readl(hba, REG_INTERRUPT_ENABLE);
	ufshcd_writel(hba, 0, REG_INTERRUPT_ENABLE);

	/* step 2: send uic cmd */
	ret = ufshcd_wait_for_register_poll(hba, REG_CONTROLLER_STATUS,
					    UIC_COMMAND_READY,
					    UFS_KIRIN_H8_TIMEOUT_RETRY_TIMES,
					    UFS_KIRIN_H8_TIMEOUT_RETRY_UDELAY);
	if (ret) {
		dev_err(hba->dev, "%s wait for uic ready timeout\n", __func__);
		goto out;
	}

	ufshcd_writel(hba, 0, REG_UIC_COMMAND_ARG_1);
	ufshcd_writel(hba, 0, REG_UIC_COMMAND_ARG_2);
	ufshcd_writel(hba, 0, REG_UIC_COMMAND_ARG_3);
	if (h8_op) /* UFS_KIRIN_H8_OP_ENTER */
		ufshcd_writel(hba, UIC_CMD_DME_HIBER_ENTER, REG_UIC_COMMAND);
	else /* UFS_KIRIN_H8_OP_EXIT */
		ufshcd_writel(hba, UIC_CMD_DME_HIBER_EXIT, REG_UIC_COMMAND);

	ret = ufshcd_wait_for_register_poll(hba, REG_INTERRUPT_STATUS,
					    UIC_COMMAND_COMPL,
					    UFS_KIRIN_H8_TIMEOUT_RETRY_TIMES,
					    UFS_KIRIN_H8_TIMEOUT_RETRY_UDELAY);
	if (ret) {
		dev_err(hba->dev, "[%s]wait UCCS time out\n",
			h8_op ? "ENTER_H8" : "EXIT_H8");
		ret = -EIO;
		goto out;
	}
	ufshcd_writel(hba, UIC_COMMAND_COMPL, REG_INTERRUPT_STATUS);

	value = ufshcd_readl(hba, REG_UIC_COMMAND_ARG_2);
	if (0 != (value & 0xFF)) {
		dev_err(hba->dev, "[%s]uic err, arg2 is 0x%x\n",
			h8_op ? "ENTER_H8" : "EXIT_H8", value);
		ret = -EIO;
		goto out;
	}

	/* step 3: check UHES/UHXS */
	if (h8_op) /* UFS_KIRIN_H8_OP_ENTER */
		mask = UIC_HIBERNATE_ENTER;
	else /* UFS_KIRIN_H8_OP_EXIT */
		mask = UIC_HIBERNATE_EXIT;
	ret = ufshcd_wait_for_register_poll(hba, REG_INTERRUPT_STATUS, mask,
					    UFS_KIRIN_H8_TIMEOUT_RETRY_TIMES,
					    UFS_KIRIN_H8_TIMEOUT_RETRY_UDELAY);
	if (ret) {
		dev_err(hba->dev, "[%s]wait UHES time out\n",
			h8_op ? "ENTER_H8" : "EXIT_H8");
		ret = -EIO;
		goto out;
	}
	ufshcd_writel(hba, mask, REG_INTERRUPT_STATUS);

	/* step 4: check UPMCRS */
	status = ufshcd_get_upmcrs(hba);
	if (status != PWR_LOCAL) {
		dev_err(hba->dev,
			"[%s]pwr ctrl cmd failed, host umpcrs:0x%x\n",
			h8_op ? "ENTER_H8" : "EXIT_H8", status);
		ret = (status != PWR_OK) ? -EIO : -1;
		goto out;
	}

#ifdef CONFIG_SCSI_UFS_HS_ERROR_RECOVER
	if ((!h8_op) && hba->check_pwm_after_h8) {
		if (!hba->vops->get_pwr_by_debug_register) {
			dev_err(hba->dev, "no check pwm op\n");
			hba->check_pwm_after_h8 = 0;
		} else {
			value = hba->vops->get_pwr_by_debug_register(hba);
			if (value == SLOW) {
				dev_err(hba->dev, "ufs pwr = 0x%x after H8\n", value);
				hba->check_pwm_after_h8 = 0;
				ufshcd_init_pwr_info(hba);
				if (!work_busy(&hba->recover_hs_work))
					schedule_work(&hba->recover_hs_work);
				else
					dev_err(hba->dev, "%s:recover_hs_work is runing \n", __func__);
			} else {
				hba->check_pwm_after_h8--;
			}
			dev_err(hba->dev, "check pwr after H8, %d times remain\n",
					hba->check_pwm_after_h8);
		}
	}
#endif
out:

	ufsdbg_error_inject_dispatcher(hba,
		ERR_INJECT_PWR_MODE_CHANGE_ERR,
		0,
		&ret);

	if (ret) {
		ufshcd_writel(hba, UIC_ERROR, REG_INTERRUPT_STATUS);
		dsm_ufs_update_error_info(hba, DSM_UFS_ENTER_OR_EXIT_H8_ERR);
		schedule_ufs_dsm_work(hba);
	}
	ufshcd_writel(hba, ie_value, REG_INTERRUPT_ENABLE);
	return ret;
}
void ufshcd_config_adapt(struct ufs_hba *hba, struct ufs_pa_layer_attr *pwr_mode)
{
	int unipro_ver;

	ufshcd_dme_get(hba, UIC_ARG_MIB(PA_REMOTEVERINFO),
			&unipro_ver);

	if (((unsigned int)unipro_ver & 0x0F) < UNIPRO_VER_1_8)
		return;

	ufshcd_dme_set(hba, UIC_ARG_MIB(PA_TXHSADAPTTYPE), PA_HS_APT_NO);
#ifdef CONFIG_HISI_UFS_HC
	if ((hba->manufacturer_id == UFS_VENDOR_TOSHIBA) ||
	    (hba->manufacturer_id == UFS_VENDOR_SKHYNIX) ||
	    (hba->manufacturer_id == UFS_VENDOR_SAMSUNG)) {
		if ((pwr_mode->pwr_rx == FASTAUTO_MODE ||
		     pwr_mode->pwr_rx == FAST_MODE) &&
		    (pwr_mode->gear_rx >= UFS_HS_G4))
			ufshcd_dme_set(hba, UIC_ARG_MIB(PA_TXHSADAPTTYPE),
				       PA_HS_APT_INITIAL);
	}
#else
	if ((hba->manufacturer_id == UFS_VENDOR_TOSHIBA) ||
	    (hba->manufacturer_id == UFS_VENDOR_MICRON) ||
	    (hba->manufacturer_id == UFS_VENDOR_SAMSUNG)) {
		if ((pwr_mode->pwr_rx == FASTAUTO_MODE ||
		     pwr_mode->pwr_rx == FAST_MODE) &&
		    (pwr_mode->gear_rx >= UFS_HS_G4))
			ufshcd_dme_set(hba, UIC_ARG_MIB(PA_TXHSADAPTTYPE),
				       PA_HS_APT_INITIAL);
	}
#endif
}

/**
 *uhshcd_rsp_sense_data - check rsp sense_data
 * @hba: per adapter instance
 * @lrb: pointer to local reference block of completed command
 * @scsi_status: the result based on SCSI status response
 */
static void
uhshcd_rsp_sense_data(struct ufs_hba *hba, struct ufshcd_lrb *lrbp, int scsi_status)
{
	if ((lrbp->ucd_rsp_ptr->sr.sense_data[2] & 0xf) == HARDWARE_ERROR) {
		dsm_ufs_update_scsi_info(lrbp, scsi_status, DSM_UFS_SCSI_CMD_ERR);
		schedule_ufs_dsm_work(hba);
		ufs_rdr_hardware_err(hba, lrbp);
	} else if ((lrbp->ucd_rsp_ptr->sr.sense_data[2] & 0xf) == MEDIUM_ERROR) {
		dsm_ufs_update_scsi_info(lrbp, scsi_status, DSM_UFS_DEV_INTERNEL_ERR);
		schedule_ufs_dsm_work(hba);
		ufs_rdr_hardware_err(hba, lrbp);
	}
#ifdef CONFIG_SCSI_UFS_HI1861_VCMD
	if (UFS_VENDOR_HI1861 == hba->manufacturer_id) {
		if ((lrbp->ucd_rsp_ptr->sr.sense_data[2] & 0xf) == MEDIUM_ERROR &&
			(lrbp->ucd_rsp_ptr->sr.sense_data[12] & 0xff) == 0x03 &&
			(lrbp->ucd_rsp_ptr->sr.sense_data[13] & 0xff) == 0) {
			dev_err(hba->dev,"1861 write fault\n");
			BUG();
		}
		if ((lrbp->ucd_rsp_ptr->sr.sense_data[2] & 0xf) == HI1861_INTERNEL &&
			(lrbp->ucd_rsp_ptr->sr.sense_data[12] & 0xff) == 0x80 &&
			(lrbp->ucd_rsp_ptr->sr.sense_data[13] & 0xff) == 0) {
			dsm_ufs_update_scsi_info(lrbp, scsi_status, DSM_UFS_HI1861_INTERNEL_ERR);
			schedule_ufs_dsm_work(hba);
			schedule_work(&hba->fsr_work);
		}
	}
#endif
}

static void ufshcd_err_handler_do_reset(
	struct ufs_hba *hba, bool err_xfer, bool err_tm, unsigned long *flags)
{
	int err = 0;
	unsigned long max_doorbells;
	unsigned int nutrs = (unsigned int)(hba->nutrs);

	max_doorbells = (1UL << nutrs) - 1;

	if (hba->saved_err & INT_FATAL_ERRORS ||
		(ufshcd_is_auto_hibern8_allowed(hba) &&
			(hba->saved_err &
				(UIC_HIBERNATE_ENTER | UIC_HIBERNATE_EXIT))))
		ufshcd_update_error_stats(hba, UFS_ERR_INT_FATAL_ERRORS);

	if (hba->saved_err & UIC_ERROR)
		ufshcd_update_error_stats(hba, UFS_ERR_INT_UIC_ERROR);

	if (err_xfer || err_tm)
		ufshcd_update_error_stats(hba, UFS_ERR_CLEAR_PEND_XFER_TM);

	/*
	 * ufshcd_reset_and_restore() does the link reinitialization
	 * which will need atleast one empty doorbell slot to send the
	 * device management commands (NOP and query commands).
	 * If there is no slot empty at this moment then free up last
	 * slot forcefully.
	 */
	if (hba->outstanding_reqs == max_doorbells)
		__ufshcd_transfer_req_compl(hba, (1UL << (nutrs - 1)));

	spin_unlock_irqrestore(hba->host->host_lock, *flags);
	err = ufshcd_reset_and_restore(hba);
	spin_lock_irqsave(hba->host->host_lock, *flags);
	if (err) {
		dev_err(hba->dev, "%s: reset and restore failed\n", __func__);
		hba->ufshcd_state = UFSHCD_STATE_ERROR;
	}
	/*
	 * Inform scsi mid-layer that we did reset and allow to handle
	 * Unit Attention properly.
	 */
	scsi_report_bus_reset(hba->host, 0);
	hba->saved_err = 0;
	hba->saved_uic_err = 0;
	hba->force_host_reset = false;
}

static void ufshcd_update_uic_reg_hist(struct ufs_uic_err_reg_hist *reg_hist,
		u32 reg)
{
	reg_hist->reg[reg_hist->pos] = reg;
	reg_hist->tstamp[reg_hist->pos] = ktime_get();
	reg_hist->pos = (reg_hist->pos + 1) % UIC_ERR_REG_HIST_LENGTH;
}

static bool ufshcd_error_need_queue_work(struct ufs_hba *hba)
{
	bool queue_eh_work = false;

	if (hba->errors & INT_FATAL_ERRORS ||
		(ufshcd_is_auto_hibern8_allowed(hba) &&
			(hba->errors &
				(UIC_HIBERNATE_ENTER | UIC_HIBERNATE_EXIT)))) {
		queue_eh_work = true;
		if (ufshcd_is_auto_hibern8_allowed(hba) &&
			(hba->errors &
				(UIC_HIBERNATE_ENTER | UIC_HIBERNATE_EXIT)))
			dsm_ufs_update_error_info(
				hba, DSM_UFS_ENTER_OR_EXIT_H8_ERR);
		if (hba->errors & CONTROLLER_FATAL_ERROR)
			dsm_ufs_update_error_info(hba, DSM_UFS_CONTROLLER_ERR);
		if (hba->errors & DEVICE_FATAL_ERROR)
			dsm_ufs_update_error_info(hba, DSM_UFS_DEV_ERR);
		if (hba->errors & SYSTEM_BUS_FATAL_ERROR)
			dsm_ufs_update_error_info(hba, DSM_UFS_SYSBUS_ERR);
#ifdef CONFIG_SCSI_UFS_INLINE_CRYPTO
		if (hba->errors & CRYPTO_ENGINE_FATAL_ERROR)
			dsm_ufs_updata_ice_info(hba);
#endif
	}
#ifdef CONFIG_HISI_UFS_HC
	if (ufshcd_is_auto_hibern8_allowed(hba) &&
		(hba->errors & AH8_EXIT_REQ_CNF_FAIL_INTR)) {
		queue_eh_work = true;
		dsm_ufs_update_error_info(hba, DSM_UFS_ENTER_OR_EXIT_H8_ERR);
	}
#endif

	return queue_eh_work;
}

static void ufs_idle_intr_check_timer_expire(unsigned long data)
{
	struct ufs_hba *hba = (struct ufs_hba *)(uintptr_t)data;

	dev_err(hba->dev, "Haven't got Idle interrupt over %d seconds\n",
		    hba->idle_intr_check_timer_threshold / 1000);
}

/**
 * ufs_idle_intr_toggle - enable or disable UFS idle interrupt
 * @hba: per adapter instance
 * @enable: 0: disable, other: enable;
 */
void ufs_idle_intr_toggle(struct ufs_hba *hba, int enable)
{
	unsigned long flags;
#ifndef CONFIG_HISI_UFS_HC
	struct ufs_kirin_host *host = (struct ufs_kirin_host *)hba->priv;
#endif

	if (!(hba->ufs_idle_intr_en))
		return;

	spin_lock_irqsave(hba->host->host_lock, flags);
	hba->idle_intr_disabled = !enable;
	if (enable) {
#ifdef CONFIG_HISI_UFS_HC
		ufshcd_writel(hba, hba->idle_timeout_val / 1000,
			UFS_CFG_IDLE_TIME_THRESHOLD);
		ufshcd_hisi_enable_idle_tmt_cnt(hba);
#else
		ufs_sys_ctrl_set_bits(
			host, BIT_UFS_IDLE_CNT_TIMEOUT_MASK, UFS_DEBUG_CTRL);
		ufs_sys_ctrl_set_bits(host, BIT_UFS_IDLE_CNT_TIMEOUT_CLR,
			UFS_IDLE_CONUTER_CLR);

		/* Set idle timeout value */
		ufs_sys_ctrl_writel(
			host, hba->idle_timeout_val, UFS_IDLE_CONUTER);

		ufs_sys_ctrl_set_bits(
			host, BIT_UFS_IDLE_CNT_EN, UFS_DEBUG_CTRL);
		ufs_sys_ctrl_clr_bits(
			host, BIT_UFS_IDLE_CNT_TIMEOUT_MASK, UFS_DEBUG_CTRL);
#endif
	} else {
#ifdef CONFIG_HISI_UFS_HC
		ufshcd_hisi_disable_idle_tmt_cnt(hba);
#else
		ufs_sys_ctrl_set_bits(host, BIT_UFS_IDLE_CNT_TIMEOUT_MASK, UFS_DEBUG_CTRL);
		ufs_sys_ctrl_clr_bits(host, BIT_UFS_IDLE_CNT_EN, UFS_DEBUG_CTRL);
		ufs_sys_ctrl_writel(host, 1, UFS_IDLE_CONUTER_CLR);
#endif
	}
	spin_unlock_irqrestore(hba->host->host_lock, flags);
}

/**
 * ufshcd_idle_handler - UFS idle interrupt service routine
 * @hba: per adapter instance
 */
void ufshcd_idle_handler(struct ufs_hba *hba)
{
	struct blk_dev_lld *lld = &(hba->host->tag_set.lld_func);

#ifdef CONFIG_HISI_DEBUG_FS
	static DEFINE_RATELIMIT_STATE(idle_print_rs, (30 * HZ), 1);
#ifdef CONFIG_HISI_UFS_HC_CORE_UTR
	u64 utp_tx_doorbell;
#else
	u32 utp_tx_doorbell;
#endif
	u32 utp_task_doorbell;

	if (!hba->idle_intr_disabled) {
#ifdef CONFIG_HISI_UFS_HC_CORE_UTR
		utp_tx_doorbell = read_core_utr_doorbells(hba);
#else
		utp_tx_doorbell = ufshcd_readl(hba, REG_UTP_TRANSFER_REQ_DOOR_BELL);
#endif
		utp_task_doorbell = ufshcd_readl(hba, REG_UTP_TASK_REQ_DOOR_BELL);
		if (__ratelimit(&idle_print_rs)) {
			if (utp_tx_doorbell || utp_task_doorbell)
				dev_err(hba->dev, "%s,Got Idle interrupt while utp_tx_doorbell: 0x%lx, utp_task_doorbell: 0x%x\n",
						__func__, (u64)utp_tx_doorbell, utp_task_doorbell);
			else if (unlikely(hba->ufs_idle_intr_verify))
				dev_info(hba->dev, "%s, Idle interrupt, all the doorbell is 0\n", __func__);
		}
	}

	mod_timer(&hba->idle_intr_check_timer, jiffies + msecs_to_jiffies(hba->idle_intr_check_timer_threshold));
#endif /* CONFIG_HISI_DEBUG_FS */


	blk_lld_idle_notify(lld);
}

/**
 * ufshcd_timeout_handle_intr - UFS timeout interrupts service routine,
 *     including Idle_intr, device_stat_timeout_intr, UFS_LA_intr, pwm_timeout_intr and auto-H8_timeout_intr.
 * @timer_irq: the interrupt line.
 * @hba: per adapter instance
 */
static irqreturn_t ufshcd_timeout_handle_intr(int timer_irq, void *__hba)
{
	struct ufs_hba *hba = __hba;
	struct ufs_kirin_host *host;
	u32 debug_stat;

	host = (struct ufs_kirin_host *)hba->priv;

	spin_lock(hba->host->host_lock);

	/* PWM and Device Timeout handler */
	debug_stat = ufs_sys_ctrl_readl(host, UFS_DEBUG_STAT);
	if (debug_stat & (BIT_UFS_PWR_INTR | BIT_UFS_DEV_TMT_INTR)) {
		if (hba->ufshcd_state == UFSHCD_STATE_OPERATIONAL) {

			dev_err(hba->dev, "%s, debug_stat 0x%x\n", __func__,
				debug_stat);
			__ufshcd_disable_dev_tmt_cnt(hba);
			__ufshcd_disable_pwm_cnt(hba);

			/* block commands from scsi mid-layer */
			scsi_block_requests(hba->host);

			hba->force_host_reset = true;
			hba->ufshcd_state = UFSHCD_STATE_EH_SCHEDULED;
			kthread_queue_work(&hba->eh_worker, &hba->eh_work);

		}
	}

	/* Idle Timeout handler */
	if (likely(hba->ufs_idle_intr_en)) {
		if (!hba->idle_intr_disabled) {
			debug_stat = ufs_sys_ctrl_readl(host, UFS_DEBUG_STAT);
			if (debug_stat & BIT_UFS_IDLE_TIMEOUT) {
				ufs_sys_ctrl_writel(host, 1, UFS_IDLE_CONUTER_CLR);
				ufshcd_idle_handler(hba);
			}
		} else {
			ufshcd_idle_handler(hba);
		}
	}

	spin_unlock(hba->host->host_lock);

	return IRQ_HANDLED;
}

/**
 * ufshcd_add_idle_intr_check_timer - if Idle interrupt is never raised for UFSHCD_IDLE_INTR_CHECK_INTERVAL,
 *     we think something goes wrong.
 * @hba: per adapter instance
 */
static void ufshcd_add_idle_intr_check_timer(struct ufs_hba *hba)
{
	hba->idle_intr_check_timer_threshold = UFSHCD_IDLE_INTR_CHECK_INTERVAL;
	init_timer(&hba->idle_intr_check_timer);
	hba->idle_intr_check_timer.data = (unsigned long)(uintptr_t)hba;
	hba->idle_intr_check_timer.function = ufs_idle_intr_check_timer_expire;
	mod_timer(&hba->idle_intr_check_timer, jiffies + msecs_to_jiffies(hba->idle_intr_check_timer_threshold));
}

static void ufshcd_set_req_abort_skip(struct ufs_hba *hba, unsigned long bitmap)
{
	struct ufshcd_lrb *lrbp;
	int tag;

	for_each_set_bit(tag, &bitmap, hba->nutrs) {
		lrbp = &hba->lrb[tag];
		lrbp->req_abort_skip = true;
	}
}

static int ufshcd_host_reset_and_restore(struct ufs_hba *hba)
{
	int err = -EAGAIN;

	/* DSM_UFS_TIMEOUT_SERIOUS */
	ufs_timeout_lock = 0;

	dev_err(hba->dev, "%s\n", __func__);

	if (!(hba->quirks & UFSHCD_QUIRK_BROKEN_HCE))
		err = __ufshcd_host_reset_and_restore(hba, true);

	if (err) {
		dev_err(hba->dev, "hci needs a full reset\n");
		ufshcd_vops_full_reset(hba);
		err = __ufshcd_host_reset_and_restore(hba, false);
	}

	if (err && hba->vops && hba->vops->dbg_uic_dump)
		hba->vops->dbg_uic_dump(hba);
#ifdef CONFIG_HISI_BLOCK_ORDER_PRESERVING
	blk_order_nr_reset(&hba->host->tag_set);
#endif
	return err;
}

#ifdef CONFIG_SCSI_UFS_HS_ERROR_RECOVER
void ufshcd_count_vol(int retry, int* v_tx, int* v_rx)
{
	if (retry / 3 >= 2) {
		*v_rx = 0;
		*v_tx = 0;
	} else if (retry % 2 == 1) {
		*v_tx = (retry / 2) + 1;
		*v_rx = 0;
	} else if (retry % 2 == 0) {
		*v_tx = 0;
		*v_rx = (retry / 2) + 1;
	}
	pr_err("ufs retry: %d count v_tx:%d v_rx:%d\n", retry, *v_tx, *v_rx);
}


void ufshcd_check_init_mode(struct ufs_hba *hba, int err)
{
	if (err) {
		if (hba->init_retry > 0) {
			hba->init_retry--;
			ufshcd_count_vol(hba->init_retry, &hba->v_tx, &hba->v_rx);
		} else {
			hba->init_retry = 6;
		}
	} else {
		if (hba->use_pwm_mode) {
			if (!work_busy(&hba->recover_hs_work)) {
				pm_runtime_forbid(hba->dev);
				hba->disable_suspend = 1;
				dev_err(hba->dev, "forbid ufs pm runtime in pwm\n");
				schedule_work(&hba->recover_hs_work);
			} else {
				dev_err(hba->dev, "%s:recover_hs_work is runing \n", __func__);
			}
		}
	}
}
#endif

static inline void ufshcd_ue_clean(struct ufs_hba *hba)
{
	unsigned long flags;

	spin_lock_irqsave(hba->host->host_lock, flags);
	ufshcd_writel(hba, UIC_ERROR, REG_INTERRUPT_STATUS);
	spin_unlock_irqrestore(hba->host->host_lock, flags);
}

static int ufshcd_ue_get(struct ufs_hba *hba)
{
	u32 value;
	int ret;
	unsigned long flags;

	spin_lock_irqsave(hba->host->host_lock, flags);
	value = ufshcd_readl(hba, REG_INTERRUPT_STATUS);
	spin_unlock_irqrestore(hba->host->host_lock, flags);
	if (value & UIC_ERROR) {
		dev_err(hba->dev, "ufs ue happened in nop. intr = 0x%x\n", value);
		ret = -1;
	} else {
		ret = 0;
	}

	return ret;
}

static void ufshcd_device_not_support_autoh8(struct ufs_hba *hba)
{
	if ((hba->manufacturer_id == UFS_VENDOR_HI1861) &&
		(strstarts(hba->model, UFS_MODEL_SS6100))) {
		if (ufshcd_is_auto_hibern8_allowed(hba) &&
			(!(hba->caps & DISABLE_UFS_PMRUNTIME))) {
			ufshcd_disable_auto_hibern8(hba);
			pm_runtime_allow(hba->dev);
			hba->caps &= (u32)(~UFSHCD_CAP_AUTO_HIBERN8);
			dev_err(hba->dev, "not support autoH8\n");
		}
	}
}

#ifdef CONFIG_HISI_BOOTDEVICE
static int ufshcd_device_get_rev(const struct device *dev, char *rev)
{
	struct ufs_hba *hba;
	int ret;
	const int buffer_len = 36;
	char buffer[buffer_len];
	unsigned char cmd[6] = {INQUIRY, 0, 0, 0,
				(char)buffer_len, 0};

	hba = (struct ufs_hba *)dev_get_drvdata(dev);
	if (!hba)
		return -EINVAL;

	ret = scsi_execute_req(hba->sdev_ufs_device, cmd,
				DMA_FROM_DEVICE, buffer,
				buffer_len, NULL,
				msecs_to_jiffies(30000), 3, NULL);
	if (ret)
		dev_err(hba->dev, "%s: failed with err %d\n",
			__func__, ret);
	else
		snprintf(rev, 5, "%.4s", buffer + 32);

	return ret;
}
#endif

/**
 * ufshcd_async_scan - asynchronous execution for probing hba
 * @data: data pointer to pass to this function
 * @cookie: cookie data
 */
static int __ufshcd_async_scan(struct ufs_hba *hba)
{
	int err;

	/* Host controller enable */
	err = ufshcd_hba_enable(hba);
	if (err) {
		dev_err(hba->dev, "Host controller enable failed\n");
	} else {
		err = ufshcd_probe_hba(hba);
	}

	return err;
}

static void ufshcd_async_scan(void *data, async_cookie_t cookie)
{
	int err;
	struct ufs_hba *hba = (struct ufs_hba *)data;

	hba->ufs_init_completed = false;
	hba->ufs_init_retries = MAX_HOST_INIT_RETRIES;

	err = __ufshcd_async_scan(hba);
	if(!err)
		goto out;

	hba->ufs_init_retries--;

	do {
		/*
		 * We reach here because ufs scan failed
		 * Change controller state to "reset state" which
		 * should also put the link in off/reset state
		 * ufshcd_set_link_off does not need in probe I think
		 */
		ufshcd_hba_stop(hba,true);

		/*
		 * HCE reset needs relink startup, give device a hardware reset
		 */
		ufshcd_vops_device_reset(hba);

		err = __ufshcd_async_scan(hba);

		if (err)
			dev_err(hba->dev, "host init failed, HCE/Device reset "
					  "err %d, retry %d\n",
				err, hba->ufs_init_retries);

		/*
		 * HCE reset failed. We must trigger a hardware reset
		 * which includes clk/regulator init and device reset
		 */
		if (err) {
			ufshcd_vops_full_reset(hba);
			err = __ufshcd_async_scan(hba);
		}

		if (err)
			dev_err(hba->dev, "host init failed, full reset err "
					  "%d, retry %d\n",
				err, hba->ufs_init_retries);
#ifdef CONFIG_SCSI_UFS_HS_ERROR_RECOVER
		ufshcd_check_init_mode(hba, err);
#endif
	} while (err && --hba->ufs_init_retries);

out:
	/*
	 * Recover the value of hba->ufs_init_retries,so that avoid affecting the
	 * condition about vcc power on then off.
	 */
	hba->ufs_init_retries = MAX_HOST_INIT_RETRIES;

	/*
	 * If we failed to initialize the device or the device is not
	 * present, turn off the power/clocks etc.
	 */
	if (err && !ufshcd_eh_in_progress(hba) && !hba->pm_op_in_progress) {
		pm_runtime_put_sync(hba->dev);
		ufshcd_hba_exit(hba);
	}
	hba->ufs_init_completed = true;
}

static ssize_t unique_number_show(struct device *dev,
					 struct device_attribute *attr,
					 char *buf)
{
	struct scsi_device *sdev = to_scsi_device(dev);
	struct ufs_hba *hba = shost_priv(sdev->host);
	int curr_len;

	curr_len = snprintf(
	    buf, PAGE_SIZE,
	    "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%04x%04x\n",
	    hba->unique_number.serial_number[0],
	    hba->unique_number.serial_number[1],
	    hba->unique_number.serial_number[2],
	    hba->unique_number.serial_number[3],
	    hba->unique_number.serial_number[4],
	    hba->unique_number.serial_number[5],
	    hba->unique_number.serial_number[6],
	    hba->unique_number.serial_number[7],
	    hba->unique_number.serial_number[8],
	    hba->unique_number.serial_number[9],
	    hba->unique_number.serial_number[10],
	    hba->unique_number.serial_number[11],
	    hba->unique_number.manufacturer_date,
	    hba->unique_number.manufacturer_id);

	return curr_len;
}

static ssize_t
man_id_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct scsi_device *sdev = to_scsi_device(dev);
	struct ufs_hba *hba = shost_priv(sdev->host);
	int curr_len;

	curr_len = snprintf(buf, PAGE_SIZE, "%04x\n", hba->manufacturer_id);

	return curr_len;
}
static ssize_t
spec_version_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct scsi_device *sdev = to_scsi_device(dev);
	struct ufs_hba *hba = shost_priv(sdev->host);
	return snprintf(buf, PAGE_SIZE, "%x\n", hba->ufs_device_spec_version);
}
static ssize_t shared_alloc_units_show(struct device *dev,
				       struct device_attribute *attr, char *buf)
{
	struct scsi_device *sdev = to_scsi_device(dev);
	struct ufs_hba *hba = shost_priv(sdev->host);
	return snprintf(buf, PAGE_SIZE, "0x%x\n", hba->wb_shared_alloc_units);
}
static ssize_t config_desc_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct scsi_device *sdev = to_scsi_device(dev);
	struct ufs_hba *hba = shost_priv(sdev->host);
	u32 value = 0;
	int ret;
	ret = ufshcd_query_attr_retry(hba, UPIU_QUERY_OPCODE_READ_ATTR,
				      QUERY_ATTR_IDN_CONF_DESC_LOCK, 0, 0,
				      &value);

	if (ret) {
		dev_err(hba->dev, "%s read config desc lock failed \n",
			__func__);
		return -EBUSY;
	}
	return snprintf(buf, sizeof(u32), "%d\n", value);
}

static ssize_t config_desc_store(struct device *dev,
				 struct device_attribute *attr, const char *buf,
				 size_t count)
{
	u32 value = 0;
	u32 origin_attr = 0;
	int ret;
	struct scsi_device *sdev = to_scsi_device(dev);
	struct ufs_hba *hba = shost_priv(sdev->host);

	ret = kstrtouint(buf, 0, &value);
	if (ret || value != 1) {
		dev_err(hba->dev, "%s invalid value ret %d value %d \n",
			__func__, ret, value);
		return -EINVAL;
	}
	ret = ufshcd_query_attr_retry(hba, UPIU_QUERY_OPCODE_READ_ATTR,
				      QUERY_ATTR_IDN_CONF_DESC_LOCK, 0, 0,
				      &origin_attr);

	if (ret) {
		dev_err(hba->dev, "%s read config desc lock failed \n",
			__func__);
		return -EBUSY;
	}
	dev_err(hba->dev, "%s config desc is %d\n", __func__, origin_attr);
	if (origin_attr == 1) {
		dev_err(hba->dev, "%s: config desc lock already locked \n",
			__func__);
		return -EPERM;
	}
	/*enable config desc lock*/
	ret = ufshcd_query_attr_retry(hba, UPIU_QUERY_OPCODE_WRITE_ATTR,
				      QUERY_ATTR_IDN_CONF_DESC_LOCK, 0, 0,
				      &value);
	if (ret) {
		dev_err(hba->dev, "%s enable config desc lock failed \n",
			__func__);
		return -EBUSY;
	}
	/*if success return count*/
	return count;
}

static int ufshcd_direct_flush(struct scsi_device *sdev);
static void ufshcd_dump_status(
	struct Scsi_Host *host, enum blk_dump_scene dump_type)
{
	struct ufs_hba *hba = shost_priv(host);
	struct ufshcd_lrb *lrbp;
	int tag;

#ifdef CONFIG_HISI_BLK
	dev_err(hba->dev, "ufshcd: lrb_in_use = 0x%lx\n", hba->lrb_in_use);
	if (dump_type != BLK_DUMP_PANIC)
		return;

	dev_err(hba->dev, "current ktime %lld, error_handle = %u\n",
		ktime_to_us(ktime_get()), hba->error_handle_count);
#ifdef CONFIG_HISI_UFS_HC
	dev_err(hba->dev,
		"last_intr = 0x%x, last_vs_intr = 0x%x, last_unipro_intr = 0x%x, last_fatal = 0x%x(%lld)\n",
		hba->last_intr, hba->last_vs_intr, hba->last_unipro_intr,
		hba->last_fatal_intr, hba->last_fatal_intr_time_stamp);
	dev_err(hba->dev, "last_core = 0x%x, last_core_intr = 0x%x(%lld)\n",
		hba->last_core, hba->last_core_intr,
		hba->last_core_intr_time_stamp);
#endif
	for_each_set_bit (tag, &hba->outstanding_reqs, hba->nutrs) {
		lrbp = &hba->lrb[tag];
		dev_err(hba->dev,
			"UPIU[%d] - tag %d - opcode 0x%x - issue time %lld - complete time %lld\n",
			tag, lrbp->req_tag, lrbp->opcode,
			ktime_to_us(lrbp->issue_time_stamp),
			ktime_to_us(lrbp->complete_time_stamp));
	}
#endif
}

/**
 * ufshcd_compose_scsi_cmd - filling scsi_cmnd with already info
 * @cmd: command from SCSI Midlayer
 * @done: call back function
 *
 * Until now, we do not filling all the infomation for scsi_cmnd, we just use
 * this for direct send cmd to ufs driver, skip block layer and scsi layer
 *
 * Returns 0 for success, non-zero in case of failure
 */
static void ufshcd_compose_scsi_cmd(struct scsi_cmnd *cmd,
				    struct scsi_device *device,
				    unsigned char *cdb,
				    unsigned char *sense_buffer,
				    enum dma_data_direction sc_data_direction,
				    struct scatterlist *sglist,
				    unsigned int nseg,
				    unsigned int sg_len)
{
	cmd->device = device;
	cmd->cmnd = cdb;
	cmd->cmd_len = COMMAND_SIZE(cdb[0]);
#ifdef CONFIG_HUAWEI_UFS_VENDOR_MODE
	if(((cdb[0]>>5)&7) >= UFS_IOCTL_VENDOR_CMD)
		cmd->cmd_len = UFS_IOCTL_VENDOR_CDB_LEN;
#endif
	cmd->sense_buffer = sense_buffer;
	cmd->sc_data_direction = sc_data_direction;

	cmd->sdb.table.sgl = sglist;
	cmd->sdb.table.nents = nseg;
	cmd->sdb.length = sg_len;
}

/**
 * ufshcd_queuecommand_directly - API for sending scsi cmd directly, of course
 * skip error handler of scsi
 * @hba - UFS hba
 * @cmd - scsi_cmnd
 * @timeout - time in jiffies
 *
 * NOTE: We use device management tag and mutext lock, without this, we must
 * define a new wait, and complete it in scsi_done
 * Since there is only one available tag for device management commands,
 * it is expected you hold the hba->dev_cmd.lock mutex.
 * This function may sleep.
 */
static int __ufshcd_queuecommand_directly(struct ufs_hba *hba,
					  struct scsi_cmnd *cmd,
					  unsigned int timeout)
{
	int err;
	int tag;
	unsigned long flags;
	unsigned long time_left;
	struct ufshcd_lrb *lrbp;
	struct completion wait;


	init_completion(&wait);
	mutex_lock(&hba->dev_cmd.lock);
	hba->dev_cmd.complete = &wait;

	err = pm_runtime_get_sync(hba->dev);
	if (err < 0) {
		spin_lock_irqsave(hba->host->host_lock, flags);
		hba->dev_cmd.complete = NULL;
		spin_unlock_irqrestore(hba->host->host_lock,
				       flags);
		goto unlock;
	}
	/*lint -save -e666 */
	wait_event(hba->dev_cmd.tag_wq,
		   ufshcd_get_dev_cmd_tag(hba, &tag));
	/*lint -restore*/
	lrbp = &hba->lrb[tag];

	WARN_ON(lrbp->cmd); /*lint !e146 !e665*/
	lrbp->cmd = cmd;
	lrbp->sense_bufflen = SCSI_SENSE_BUFFERSIZE;
	lrbp->sense_buffer = cmd->sense_buffer;
	lrbp->saved_sense_len = 0;
	lrbp->task_tag = tag;
	lrbp->lun = ufshcd_scsi_to_upiu_lun((unsigned int)cmd->device->lun);
	lrbp->intr_cmd = !ufshcd_is_intr_aggr_allowed(hba) ? true : false;
	lrbp->command_type = UTP_CMD_TYPE_SCSI;

	/* form UPIU before issuing the command */
	err = ufshcd_compose_upiu(hba, lrbp);
	if (err)
		goto out;

	/* Black Magic, dont touch unless you want a BUG */
	lrbp->command_type = UTP_CMD_TYPE_DEV_MANAGE;
	err = ufshcd_map_sg(hba, lrbp);
	if (err)
		goto out;

	/* Make sure descriptors are ready before ringing the doorbell */
	wmb();

	spin_lock_irqsave(hba->host->host_lock, flags);
#ifdef CONFIG_HISI_SCSI_UFS_DUMP
	ufshcd_dump_scsi_command(hba, (unsigned int)tag);
#endif
	/* issue command to the controller */
	ufshcd_send_command(hba, (unsigned int)tag);
	spin_unlock_irqrestore(hba->host->host_lock, flags);

	time_left = wait_for_completion_timeout(hba->dev_cmd.complete,
						msecs_to_jiffies(timeout));

	spin_lock_irqsave(hba->host->host_lock, flags);
	hba->dev_cmd.complete = NULL;
	spin_unlock_irqrestore(hba->host->host_lock, flags);

	if (likely(time_left)) {
		err = ufshcd_transfer_rsp_status(hba, lrbp, false);
	} else {
		err = -ETIMEDOUT;
		dev_err(hba->dev, "%s: scsi request timedout, tag %d\n",
			__func__, lrbp->task_tag);
		if (!ufshcd_clear_cmd(hba, lrbp->task_tag))
			err = -EAGAIN;
		ufshcd_outstanding_req_clear(hba, lrbp->task_tag);
	}

out:
	lrbp->cmd = NULL;
	ufshcd_put_dev_cmd_tag(hba, tag);
	wake_up(&hba->dev_cmd.tag_wq);
unlock:
	pm_runtime_put_sync(hba->dev);
	mutex_unlock(&hba->dev_cmd.lock);
	return err;
}

static int ufshcd_queuecommand_directly(struct ufs_hba *hba,
					struct scsi_cmnd *cmd,
					unsigned int timeout, bool eh_handle)
{
	int err, retry = 1;
	unsigned long flags;
	bool needs_flush = false;

start:
	spin_lock_irqsave(hba->host->host_lock, flags);
	if (hba->force_host_reset || hba->ufshcd_state == UFSHCD_STATE_RESET ||
		hba->ufshcd_state == UFSHCD_STATE_EH_SCHEDULED)
		needs_flush = true;
	spin_unlock_irqrestore(hba->host->host_lock, flags);

	if (needs_flush)
		kthread_flush_work(&hba->eh_work);

	/* Assume flush work makes ufshcd works well, or return error */
	spin_lock_irqsave(hba->host->host_lock, flags);
	if (hba->ufshcd_state != UFSHCD_STATE_OPERATIONAL) {
		err = SCSI_MLQUEUE_HOST_BUSY;
		spin_unlock_irqrestore(hba->host->host_lock, flags);
		return err;
	}
	spin_unlock_irqrestore(hba->host->host_lock, flags);

	err = __ufshcd_queuecommand_directly(hba, cmd, timeout);

	if (err && eh_handle) {
		spin_lock_irqsave(hba->host->host_lock, flags);
		hba->force_host_reset = true;
		hba->ufshcd_state = UFSHCD_STATE_EH_SCHEDULED;

		if (!kthread_queue_work(&hba->eh_worker, &hba->eh_work))
			dev_err(hba->dev, "%s: queue hba->eh_worker fail! \r\n", __func__);

		spin_unlock_irqrestore(hba->host->host_lock, flags);
		if (retry-- > 0)
			goto start;
	}

	return err;
}

static int compose_lrbp(struct ufs_hba *hba, struct scsi_cmnd *cmd, int tag, struct ufshcd_lrb *lrbp)
{
	int err;

	lrbp->cmd = cmd;
	lrbp->sense_bufflen = SCSI_SENSE_BUFFERSIZE;
	lrbp->sense_buffer = cmd->sense_buffer;
	lrbp->saved_sense_len = 0;
	lrbp->task_tag = tag;
	lrbp->lun = ufshcd_scsi_to_upiu_lun((unsigned int)cmd->device->lun);
	lrbp->intr_cmd = !ufshcd_is_intr_aggr_allowed(hba) ? true : false;
	lrbp->command_type = UTP_CMD_TYPE_SCSI;
	err = ufshcd_compose_upiu(hba, lrbp);
	if (err)
		return err;
	lrbp->command_type = UTP_CMD_TYPE_DEV_MANAGE;

	return 0;
}

int ufstt_do_scsi_cmd(
	struct ufs_hba *hba, struct scsi_cmnd *cmd, unsigned int timeout, ktime_t start_time)
{
	int err;
	int tag;
	unsigned long flags;
	unsigned long time_left;
	struct ufshcd_lrb *lrbp = NULL;
	struct completion wait;

	init_completion(&wait);
	mutex_lock(&hba->dev_cmd.lock);
	hba->dev_cmd.complete = &wait;
	err = pm_runtime_get_sync(hba->dev);
	if (err < 0) {/*lint !e548*/
		dev_err(hba->dev, "%s, failed to exit from hibernate, %d\n", __func__, err);
		spin_lock_irqsave(hba->host->host_lock, flags);
		hba->dev_cmd.complete = NULL;
		spin_unlock_irqrestore(hba->host->host_lock, flags);
		goto unlock;
	}
	wait_event(hba->dev_cmd.tag_wq, ufshcd_get_dev_cmd_tag(hba, &tag));/*lint !e666*/
	lrbp = &hba->lrb[tag];
	WARN_ON(lrbp->cmd);
	err = compose_lrbp(hba, cmd, tag, lrbp);
	if (err)
		goto out;
	err = ufshcd_map_sg(hba, lrbp);
	if (err)
		goto out;
	wmb();
	spin_lock_irqsave(hba->host->host_lock, flags);
	ufshcd_send_command(hba, (unsigned int)tag);
	spin_unlock_irqrestore(hba->host->host_lock, flags);
	time_left = wait_for_completion_timeout(
		hba->dev_cmd.complete, msecs_to_jiffies(timeout));
	spin_lock_irqsave(hba->host->host_lock, flags);
	hba->dev_cmd.complete = NULL;
	spin_unlock_irqrestore(hba->host->host_lock, flags);
	if (likely(time_left)) {
		err = ufshcd_transfer_rsp_status(hba, lrbp, false);
		if (err)
			hba->ufshpb_check_rsp_err++;

	}
	if (!time_left) {
		err = -ETIMEDOUT;
		hba->ufshpb_scsi_timeout++;
		if (!ufshcd_clear_cmd(hba, lrbp->task_tag))
			err = -EAGAIN;
		ufshcd_outstanding_req_clear(hba, lrbp->task_tag);
	}
out:
	lrbp->cmd = NULL;
	ufshcd_put_dev_cmd_tag(hba, tag);
	wake_up(&hba->dev_cmd.tag_wq);
unlock:
	pm_runtime_put_sync(hba->dev);
	mutex_unlock(&hba->dev_cmd.lock);
	return err;
}

static int ufshcd_send_scsi_ssu(struct ufs_hba *hba,
				struct scsi_device *sdp,
				unsigned char *cmd,
				unsigned int timeout,
				struct scsi_sense_hdr *sshdr)
{
	int ret;
	struct scsi_cmnd cmnd;
	unsigned char *buffer;

	buffer = kzalloc((size_t)SCSI_SENSE_BUFFERSIZE, GFP_KERNEL);
	if (!buffer) {
		ret = -ENOMEM;
		goto out;
	}

	memset(&cmnd, 0, sizeof(cmnd));

	ufshcd_compose_scsi_cmd(&cmnd, sdp, cmd, buffer, DMA_NONE, NULL, 0, 0);

	ret = ufshcd_queuecommand_directly(hba, &cmnd, timeout, true);
	if (ret)
		dev_err(hba->dev, "%s: failed with err %d\n", __func__, ret);

	kfree(buffer);
out:
	return ret;
}

static int ufshcd_send_scsi_request_sense(struct ufs_hba *hba,
		struct scsi_device *sdp, unsigned int timeout, bool eh_handle)
{
	int ret;
	unsigned char *buffer = NULL;
	unsigned char *dma_buf = NULL;
	struct scatterlist sglist;
	struct scsi_cmnd cmnd;

	unsigned char cmd[6] = {REQUEST_SENSE,	 0, 0, 0,
				SCSI_SENSE_BUFFERSIZE, 0};

	buffer = kzalloc((size_t)SCSI_SENSE_BUFFERSIZE, GFP_KERNEL);
	if (!buffer) {
		ret = -ENOMEM;
		goto out;
	}

	dma_buf = kzalloc((size_t)PAGE_SIZE, GFP_KERNEL);
	if (!dma_buf) {
		ret = -ENOMEM;
		goto free_buffer;
	}

	sg_init_one(&sglist, dma_buf, (unsigned int)PAGE_SIZE);

	memset(&cmnd, 0, sizeof(cmnd));

	ufshcd_compose_scsi_cmd(&cmnd, sdp, cmd, buffer, DMA_FROM_DEVICE,
				&sglist, 1, (unsigned int)PAGE_SIZE);

	ret = ufshcd_queuecommand_directly(hba, &cmnd, timeout, eh_handle);
	if (ret)
		dev_err(hba->dev, "%s: failed with err %d\n", __func__, ret);

	kfree(dma_buf);
free_buffer:
	kfree(buffer);
out:
	return ret;
}

#ifdef CONFIG_HISI_BLOCK_ORDER_PRESERVING
static int ufshcd_direct_req_sense(struct scsi_device *sdp,
				unsigned int timeout, bool eh_handle)
{
	int ret;
	struct ufs_hba *hba;
	struct Scsi_Host *shost;

	shost = sdp->host;
	hba = shost_priv(shost);

	ret = ufshcd_send_scsi_request_sense(hba, sdp, timeout, eh_handle);
	if (ret)
		dev_err(hba->dev, "%s: failed with err %d\n", __func__, ret);

	return ret;
}
#endif


#ifdef CONFIG_HUAWEI_UFS_VENDOR_MODE
int ufshcd_send_vendor_scsi_cmd(struct ufs_hba *hba,
		struct scsi_device *sdp, unsigned char* cdb, void* buf)
{
	int ret;
	unsigned char *dma_buf;
	struct scatterlist sglist;
	struct scsi_cmnd cmnd;

	dma_buf = kzalloc((size_t)PAGE_SIZE, GFP_ATOMIC);
	if (!dma_buf) {
		ret = -ENOMEM;
		goto out;
	}
	sg_init_one(&sglist, dma_buf, (unsigned int)PAGE_SIZE);

	memset(&cmnd, 0, sizeof(cmnd));

	ufshcd_compose_scsi_cmd(&cmnd, sdp, cdb, NULL, DMA_FROM_DEVICE,
				&sglist, 1, (unsigned int)PAGE_SIZE);

	ret = ufshcd_queuecommand_directly(hba, &cmnd, (unsigned int)1000, false);
	if(buf)
		memcpy(buf, dma_buf, PAGE_SIZE);
	if (ret)
		pr_err("%s: failed with err %d\n", __func__, ret);

	kfree(dma_buf);
out:
	return ret;
}
#endif

/*
 * Sync cache for already knowed fixed lun (0-4)
 * If failed, then failed, skip SCSI layer means skip exception handler
 */
static int ufshcd_send_scsi_sync_cache(struct ufs_hba *hba,
				       struct scsi_device *sdp)
{
	int ret = 0;
	int j;
	unsigned int i;
	unsigned char *buffer = NULL;
	struct scsi_cmnd cmnd;
	struct scsi_device* psdev = NULL;

	unsigned char cmd[10] = {0};
	cmd[0] = SYNCHRONIZE_CACHE;

	if (!sdp)
		return -ENODEV;
	psdev = (struct scsi_device*)kzalloc(sizeof(struct scsi_device),GFP_KERNEL);
	if (!psdev){
		buffer = NULL;
		ret = -ENOMEM;
		goto out;
	}
	buffer = kzalloc((size_t)SCSI_SENSE_BUFFERSIZE, GFP_KERNEL);
	if (!buffer) {
		ret = -ENOMEM;
		goto out;
	}

	memset(&cmnd, 0, sizeof(cmnd));

	/* find scsi_host by device well know's host member */
	psdev->host = sdp->host;

	for (i = 3,j=0; j < 4; j++,i--) {
#ifdef CONFIG_SCSI_UFS_LUN_PROTECT
		/*
		 * LU0/LU1/LU2 are write-protected, and sync-cache cannot be
		 * executed.
		 */
		if ((i == 0) || (i == 1) || (i == 2))
			continue;
#endif
		psdev->lun = i;
		ufshcd_compose_scsi_cmd(&cmnd, psdev, cmd, buffer, DMA_NONE,
					NULL, 0, 0);

		ret = ufshcd_queuecommand_directly(hba, &cmnd, (unsigned int)1500, true);
		if (ret) {
			dev_err(hba->dev, "%s: failed for lun %llu, ret = %d\n", __func__,
			       psdev->lun, ret);
			goto out;
		}
	}

out:
	if(psdev)
		kfree(psdev);
	if(buffer)
		kfree(buffer);
	return ret;
}

static unsigned char *sync_cache_buffer = NULL;
static int ufshcd_send_scsi_sync_cache_init(void)
{
	if (likely(!sync_cache_buffer)) {
		sync_cache_buffer = kzalloc((size_t)SCSI_SENSE_BUFFERSIZE, GFP_KERNEL);
		if (unlikely(!sync_cache_buffer))
			return -ENOMEM;
	}
	return 0;
}

static void ufshcd_send_scsi_sync_cache_deinit(void)
{
	if (likely(sync_cache_buffer)) {
		kfree(sync_cache_buffer);
		sync_cache_buffer = NULL;
	}
}

static void ufshcd_sync_cache_setup_lrbp(struct ufshcd_lrb *lrbp,
		struct scsi_cmnd *cmd, int tag)
{
	lrbp->cmd = cmd;
	lrbp->sense_bufflen = SCSI_SENSE_BUFFERSIZE;
	lrbp->sense_buffer = cmd->sense_buffer;
	lrbp->saved_sense_len = 0;
	lrbp->task_tag = tag;
	lrbp->lun = ufshcd_scsi_to_upiu_lun((unsigned int)cmd->device->lun);
	lrbp->intr_cmd = false;
	lrbp->command_type = UTP_CMD_TYPE_SCSI;
}

static int ufshcd_sync_cache_irq_safe(struct ufs_hba *hba,
					struct scsi_cmnd *cmd,	unsigned int query_intr_timeout)
{
	int tag;
	unsigned long flags;
	struct ufshcd_lrb *lrbp = NULL;
	volatile u32 ie_value = 0;
	volatile u32 tr_doorbell = 0;

	pm_runtime_get_sync(hba->dev);

	spin_lock_irqsave(hba->host->host_lock, flags);
	scsi_block_requests(hba->host);
	if (__ufshcd_wait_for_doorbell_clr(hba)) {
		dev_err(hba->dev, "wait doorbell clear timeout\n");
		goto unlock_unblock_and_enable_intr;
	}

	ie_value = ufshcd_readl(hba, REG_INTERRUPT_ENABLE);
	if (ie_value)
		ufshcd_writel(hba, 0, REG_INTERRUPT_ENABLE);

	tag = (int)ffz(hba->lrb_in_use);
	if (tag >= hba->nutrs)
		goto unlock_unblock_and_enable_intr;

	__set_bit(tag, &hba->lrb_in_use);
	lrbp = &hba->lrb[tag];
	ufshcd_sync_cache_setup_lrbp(lrbp, cmd, tag);
	if (ufshcd_compose_upiu(hba, lrbp))
		goto unlock_unblock_and_enable_intr;
	/* Make sure descriptors are ready before ringing the doorbell */
	wmb();
	/* issue command to the controller */
	__set_bit(tag, &hba->outstanding_reqs);
#ifdef CONFIG_HISI_UFS_HC_CORE_UTR
	ufshcd_writel(hba, 1 << ((unsigned int)tag % SLOT_NUM_EACH_CORE),
		      UFS_CORE_UTRLDBR(tag / SLOT_NUM_EACH_CORE));
#else
	ufshcd_writel(hba, 1 << (unsigned int)tag, REG_UTP_TRANSFER_REQ_DOOR_BELL);
#endif
	/* Make sure that doorbell is committed immediately */
	wmb();
	while (query_intr_timeout-- > 0) {
#ifdef CONFIG_HISI_UFS_HC_CORE_UTR
		tr_doorbell = ufshcd_readl(hba, UFS_CORE_UTRLDBR(tag / SLOT_NUM_EACH_CORE));
		if (!(tr_doorbell & (1U << (tag % SLOT_NUM_EACH_CORE)))) {
#else
		tr_doorbell = ufshcd_readl(hba, REG_UTP_TRANSFER_REQ_DOOR_BELL);
		if (!(tr_doorbell & (1U << (unsigned int)tag))) {
#endif
			hba->outstanding_reqs ^= (1UL << (unsigned int)tag);
			goto scsi_cmd_deinit;
		}
		udelay(50);
	}
scsi_cmd_deinit:
	lrbp->cmd = NULL;
	__clear_bit(tag, &hba->lrb_in_use);
unlock_unblock_and_enable_intr:
	if (ie_value)
		ufshcd_writel(hba, ie_value, REG_INTERRUPT_ENABLE);
	spin_unlock_irqrestore(hba->host->host_lock, flags);
	scsi_unblock_requests(hba->host);
	pm_runtime_put_autosuspend(hba->dev);
	return 0;
}

static int ufshcd_send_scsi_sync_cache_on_specified_disk(struct ufs_hba *hba, struct scsi_device *sdp)
{
	int ret;
	struct scsi_cmnd cmnd;
	unsigned char cmd[10] = {0};
	cmd[0] = SYNCHRONIZE_CACHE;
	memset(&cmnd, 0, sizeof(cmnd));
	ufshcd_compose_scsi_cmd(&cmnd, sdp, cmd, sync_cache_buffer, DMA_NONE, NULL, 0, 0);
	ret = ufshcd_sync_cache_irq_safe(hba, &cmnd, (unsigned int)100000);
	dev_err(hba->dev, "UFS:<%s> Emergency sync cache lun=%llu ret = %d \r\n", __func__, sdp->lun, ret);
	return ret;
}

static int ufshcd_trylock_hostlock(struct ufs_hba *hba,unsigned long* flags)
{
	int locked = 0;
	unsigned int trycount = 100000;
	do{
		locked = spin_trylock_irqsave(hba->host->host_lock, *flags);/*lint  !e666*/
		if(locked)
			break;
		udelay(10);
	}while(--trycount>0);
	return locked;
}

static int ufshcd_direct_flush(struct scsi_device *sdev)
{
	int ret;
	unsigned long flags;
	struct Scsi_Host *host = NULL;
	struct ufs_hba *hba = NULL;
	if (!sdev ) {
		pr_err("%s, sdev is null!\n",__func__);
		return -ENODEV;
	}
	host = sdev->host;
	hba = shost_priv(host);

	if (!hba ) {
		pr_err( "%s, hba is null!\n",__func__);
		return -ENODEV;
	}

	if (hba->is_sys_suspended || hba->pm_op_in_progress){/*If ufs is suspended, it's uncessary to flush the cache*/
		dev_err(hba->dev, "%s, sys has suspended!\n",__func__);
		return 0;
	}
	if (!ufshcd_trylock_hostlock(hba, &flags)) {/*Get hostlock timeout, the abnormal context may have the locker*/
		dev_err(hba->dev, "%s, can't get the hostlock!\n",__func__);
		return -EIO;
	}
	ret = scsi_device_get(sdev);
	if (!ret && !scsi_device_online(sdev)) {
		dev_err(hba->dev, "%s, scsi_device_get error or device not online, %d\n",__func__, ret);
		ret = -ENODEV;
	}
	if (hba->ufshcd_state != UFSHCD_STATE_OPERATIONAL) {
		dev_err(hba->dev, "%s, ufshcd_state isn't operational, ufshcd_state = %d\n",__func__, hba->ufshcd_state);
		ret = SCSI_MLQUEUE_HOST_BUSY;
	}
	spin_unlock_irqrestore(hba->host->host_lock, flags);
	if (ret) {
		scsi_device_put(sdev);
		return ret;
	}
	ret = ufshcd_send_scsi_sync_cache_on_specified_disk(hba, sdev);
	scsi_device_put(sdev);
	return ret;
}

static void ufshcd_device_need_enable_autobkops(struct ufs_hba *hba)
{
	if (hba->manufacturer_id == UFS_VENDOR_SANDISK) {
		hba->auto_bkops_enabled = false;
		hba->ee_ctrl_mask |= MASK_EE_URGENT_BKOPS;
		ufshcd_enable_auto_bkops(hba);
	}
}

int hpb_reset_process(struct ufs_hba *hba, bool *hpb_after_reset)
{
	int ret;
	int retry_count = 0;

	*hpb_after_reset = true;
	ret = ufshcd_query_flag(hba, UPIU_QUERY_OPCODE_SET_FLAG,
				QUERY_FLAG_IDN_HPB_RESET, NULL);
	if (!ret)
		pr_err("%s, reset hpb error, ret %x\n", __func__, ret);

	while (*hpb_after_reset) {
		retry_count++;
		ufshcd_query_flag_retry(hba, UPIU_QUERY_OPCODE_READ_FLAG,
					QUERY_FLAG_IDN_HPB_RESET, hpb_after_reset);

		hba->ufshpb_reset_count++;
		if (retry_count >= HPB_RESET_MAX_COUNT) {
			pr_err("%s, device hpb reset processing\n", __func__);
			return -EINVAL;
		}
	}
	pr_err("%s, device hpb resetting is done\n", __func__);
	return 0;
}

void ufs_get_geometry_info(struct ufs_hba *hba)
{
	int err;
	uint8_t desc_buf[QUERY_DESC_GEOMETRY_MAX_SIZE];
	u64 total_raw_device_capacity;
#ifdef CONFIG_HISI_BOOTDEVICE
	u8 rpmb_read_write_size = 0;
	u64 rpmb_read_frame_support = 0;
	u64 rpmb_write_frame_support = 0;
#endif
	err =
	    ufshcd_read_geometry_desc(hba, desc_buf, QUERY_DESC_GEOMETRY_MAX_SIZE);
	if (err) {
		dev_err(hba->dev, "%s: Failed getting geometry info\n", __func__);
		goto out;
	}
	total_raw_device_capacity =
		(u64)desc_buf[GEOMETRY_DESC_TOTAL_DEVICE_CAPACITY + 0] << 56 |
		(u64)desc_buf[GEOMETRY_DESC_TOTAL_DEVICE_CAPACITY + 1] << 48 |
		(u64)desc_buf[GEOMETRY_DESC_TOTAL_DEVICE_CAPACITY + 2] << 40 |
		(u64)desc_buf[GEOMETRY_DESC_TOTAL_DEVICE_CAPACITY + 3] << 32 |
		(u64)desc_buf[GEOMETRY_DESC_TOTAL_DEVICE_CAPACITY + 4] << 24 |
		(u64)desc_buf[GEOMETRY_DESC_TOTAL_DEVICE_CAPACITY + 5] << 16 |
		(u64)desc_buf[GEOMETRY_DESC_TOTAL_DEVICE_CAPACITY + 6] << 8 |
		(u64)desc_buf[GEOMETRY_DESC_TOTAL_DEVICE_CAPACITY + 7] << 0;

#ifdef CONFIG_HISI_BOOTDEVICE
	set_bootdevice_size(total_raw_device_capacity);
	rpmb_read_write_size = (u8)desc_buf[GEOMETRY_DESC_RPMB_READ_WRITE_SIZE];
	/*we set rpmb support frame, now max is 64*/
	if(rpmb_read_write_size > MAX_FRAME_BIT){
		dev_err(hba->dev, "%s: rpmb_read_write_size is 0x%x, and large than 64, we set default value is 64\n", __func__,rpmb_read_write_size);
		rpmb_read_write_size = MAX_FRAME_BIT;
	}
	rpmb_read_frame_support = (((uint64_t)1 << (rpmb_read_write_size - 1)) -1) | (((uint64_t)1 << (rpmb_read_write_size - 1)));
	rpmb_write_frame_support = (((uint64_t)1 << (rpmb_read_write_size - 1)) -1) | (((uint64_t)1 << (rpmb_read_write_size - 1)));
	set_rpmb_read_frame_support(rpmb_read_frame_support);
	set_rpmb_write_frame_support(rpmb_write_frame_support);
#endif
	if (hba->host->queue_quirk_flag &
		SHOST_QUIRK(SHOST_QUIRK_IO_LATENCY_PROTECTION))
		blk_mq_tagset_vl_setup(&hba->host->tag_set,
			total_raw_device_capacity);
out:
	return;
}

void ufs_get_device_health_info(struct ufs_hba *hba)
{
	int err;
	uint8_t desc_buf[QUERY_DESC_HEALTH_MAX_SIZE];
	u8 pre_eol_info;
	u8 life_time_est_typ_a;
	u8 life_time_est_typ_b;

	err =
	    ufshcd_read_device_health_desc(hba, desc_buf, QUERY_DESC_HEALTH_MAX_SIZE);
	if (err) {
		dev_err(hba->dev, "%s: Failed getting device health info\n", __func__);
		goto out;
	}

	pre_eol_info = desc_buf[HEALTH_DEVICE_DESC_PARAM_PREEOL];
	life_time_est_typ_a = desc_buf[HEALTH_DEVICE_DESC_PARAM_LIFETIMEA];
	life_time_est_typ_b = desc_buf[HEALTH_DEVICE_DESC_PARAM_LIFETIMEB];

	if (strstr(saved_command_line, "androidboot.swtype=factory") &&
	    (life_time_est_typ_a > 1 || life_time_est_typ_b > 1)) {
		dsm_ufs_update_error_info(hba, DSM_UFS_LIFETIME_EXCCED_ERR);
		dev_err(hba->dev, "%s: life_time_est_typ_a = %d, life_time_est_typ_b = %d\n",
		        __func__,
		        life_time_est_typ_a,
		        life_time_est_typ_b);
		dsm_ufs_update_lifetime_info(life_time_est_typ_a, life_time_est_typ_b);
		if (dsm_ufs_enabled())
			schedule_work(&hba->dsm_work);
	}
#ifdef CONFIG_HISI_BOOTDEVICE
	set_bootdevice_pre_eol_info(pre_eol_info);
	set_bootdevice_life_time_est_typ_a(life_time_est_typ_a);
	set_bootdevice_life_time_est_typ_b(life_time_est_typ_b);
#endif

#if defined(CONFIG_HISI_BOOTDEVICE) && defined(CONFIG_HUAWEI_STORAGE_ROFA)
	if (get_bootdevice_type() == BOOT_DEVICE_UFS)
		storage_rochk_record_bootdevice_pre_eol_info(pre_eol_info);
#endif

out:
	return;
}

bool ufshcd_is_hisi_ufs_hc(struct ufs_hba *hba)
{
	if (hba->use_hisi_ufs_hc)
		return true;
	else
		return false;
}

/*
 * ufshcd_set_lun_protect() - set device write protect
 * hba: per-adapter instance
 *
 * Set fPowerOnWPEn flag and poll until device toggles it.
 */
#ifdef CONFIG_SCSI_UFS_LUN_PROTECT
static int ufshcd_set_lun_protect(struct ufs_hba *hba)
{
	int err;
	bool flag_res = false;
	unsigned int enter_recovery;
	char *pstr = NULL;

	pstr = strstr(saved_command_line, "enter_recovery=");
	if (pstr && sscanf(pstr, "enter_recovery=%u", &enter_recovery) == 1) {
		if (enter_recovery == 1) {
			dev_err(hba->dev, "set LU protect enter recovery\n");
			err = 0;
			goto out;
		}
	}

	err = ufshcd_query_flag_retry(hba, UPIU_QUERY_OPCODE_SET_FLAG,
			QUERY_FLAG_IDN_PWR_ON_WPE, NULL);
	if (err) {
		dev_err(hba->dev,
			"%s setting protect flag failed with error %d\n",
			__func__, err);
		goto out;
	}
	/* UFS needs 1ms to take affect for enabling lun protect. */
	msleep(1);
	err = ufshcd_query_flag_retry(hba, UPIU_QUERY_OPCODE_READ_FLAG,
			QUERY_FLAG_IDN_PWR_ON_WPE, &flag_res);
	if (err)
		dev_err(hba->dev,
			"%s reading fPowerOnWPEn failed with error %d\n",
			__func__, err);
	else if (flag_res)
		dev_err(hba->dev, "set LU protect sucess\n");
	else
		dev_err(hba->dev, "set LU protect fail\n");

out:
	return err;
}
#endif

#ifdef CONFIG_SCSI_UFS_HI1861_VCMD
/*
 * ufshcd_device_capbitlity_config - device need config capability.
 * @hba - Pointer to adapter instance
 */
static void ufshcd_device_capbitlity_config(struct ufs_hba *hba)
{
	int ret;
	int buf_len = 0;

	if (!(hba->quirks & UFSHCD_QUIRK_DEVICE_CONFIG))
		return;

	ret = ufshcd_query_vcmd_retry(hba, UPIU_QUERY_OPCODE_VENDOR_WRITE,
				      (u8)DEVICE_CAPABILITY_FLAG, 0, 0, NULL,
				      &buf_len);

	dev_err(hba->dev, "%s: ret=0x%x.\n", __func__, ret);
}
#endif

static void ufshcd_update_dev_cmd_lun(struct ufs_hba *hba,
				      struct ufshcd_lrb *lrbp)
{
	struct ufs_query_req *request = &hba->dev_cmd.query.request;

	if (request->upiu_req.opcode == UPIU_QUERY_OPCODE_VENDOR_READ &&
	    request->upiu_req.idn == QUERY_TZ_IDN_BLK_INFO)
		lrbp->lun = request->lun;
}

static void hisi_ufs_parse_dev_desc(struct ufs_hba *hba,
				    struct ufs_dev_desc *dev_desc,
				    const u8 *desc_buf)
{
	dev_desc->spec_version =
		get_unaligned_be16(&desc_buf[DEVICE_DESC_PARAM_SPEC_VER]);
	dev_desc->wmanufacturer_date =
		get_unaligned_be16(&desc_buf[DEVICE_DESC_PARAM_MANF_DATE]);
	dev_desc->serial_num_index = desc_buf[DEVICE_DESC_PARAM_SN];
	if (hba->desc_size.dev_desc >= DEVICE_DESC_PARAM_FEATURE)
		dev_desc->vendor_feature = get_unaligned_be32(
			&desc_buf[DEVICE_DESC_PARAM_FEATURE]);

	hba->manufacturer_id = dev_desc->wmanufacturerid;
	hba->manufacturer_date = dev_desc->wmanufacturer_date;
	hba->ufs_device_spec_version = dev_desc->spec_version;

	dev_err(hba->dev, "ufs spec version: 0x%x\n",
		hba->ufs_device_spec_version);

	if (hba->ufs_device_spec_version >= UFS_DEVICE_SPEC_3_1 ||
		hba->ufs_device_spec_version == UFS_DEVICE_SPEC_2_2) {
		hba->wb_type = desc_buf[DEVICE_DESC_PARAM_WB_TYPE];
		hba->wb_shared_alloc_units = get_unaligned_be32(
			desc_buf + DEVICE_DESC_PARAM_WB_SHARED_ALLOC_UNITS);
		dev_err(hba->dev, "write shared alloc units = 0x%x\n",
			hba->wb_shared_alloc_units);

		hba->d_ext_ufs_feature_sup = get_unaligned_be32(
			desc_buf + DEVICE_DESC_PARAM_EXT_UFS_FEATURE_SUP);
		dev_err(hba->dev, "d_ext_ufs_feature_sup = 0x%x\n",
			hba->d_ext_ufs_feature_sup);
	}
}

static void ufshcd_custom_upiu_turbo_zone(struct ufs_hba *hba,
					  struct ufshcd_lrb *lrbp)
{
	struct scsi_cmnd *cmd = lrbp->cmd;

	if (unlikely(!cmd->request))
		return;
	/*
	 * set cmd byte 6 to turbo zone group number
	 * custom value: 10001b to 11111b
	 */
	if (req_tz(cmd->request) &&
	    (cmd->cmnd[0] == WRITE_10 || cmd->cmnd[0] == READ_10))
		lrbp->ucd_req_ptr->sc.cdb[6] = TZ_GROUP_NUM;
}

static void ufshcd_custom_upiu_build(struct ufs_hba *hba,
				     struct ufshcd_lrb *lrbp)
{
	ufshcd_custom_upiu_turbo_zone(hba, lrbp);
}

static void ufshcd_complete_time(struct ufs_hba *hba, struct ufshcd_lrb *lrbp,
				 uint8_t opcode)
{
	s64 cost_time;

	lrbp->complete_time_stamp = ktime_get();
	cost_time = ktime_ms_delta(lrbp->complete_time_stamp,
				   lrbp->issue_time_stamp);
	/* if request costs 2000ms, print it */
	if (cost_time >= 2000)
		dev_info(hba->dev,
			"opcode 0x%x, cost %lldms, issue %lld, complete %lld\n",
			opcode, cost_time, lrbp->issue_time_stamp,
			lrbp->complete_time_stamp);
}

/********************************************************************/
/*****************************add by hisi,end************************/
/********************************************************************/
