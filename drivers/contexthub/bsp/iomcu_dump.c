/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Contexthub dump process.
 * Author: Huawei
 * Create: 2019-06-10
 */
#include "securec.h"
#include "soc_mid.h"
#include <asm/cacheflush.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/fs.h>
#include <linux/hisi/hisi_rproc.h>
#include <linux/hisi/rdr_pub.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/of_platform.h>
#include <linux/pm_wakeup.h>
#include <linux/reboot.h>
#include <linux/regulator/consumer.h>
#include <linux/semaphore.h>
#include <linux/slab.h>
#include <linux/syscalls.h>
#include <linux/types.h>
#include <linux/uaccess.h>
#include <linux/version.h>
#include <linux/debugfs.h>
#include <securec.h>
#ifdef CONFIG_HISI_NOC_MODID_REGISTER
#include "hisi_noc_modid_para.h"
#endif
#include <linux/hisi/contexthub/iomcu_boot.h>
#include <linux/hisi/contexthub/iomcu_dump.h>
#include <linux/hisi/contexthub/iomcu_shmem.h>

#define DUMP_REGS_MAGIC 0x7a686f75
#define DUMP_REGS_SIZE 128
#define REG_NAME_SIZE 25
#define LOG_EXTRA_NUM 2
#define CUR_PATH_LEN 64

/***************__weak start****************/
uint32_t __weak need_set_3v_io_power = 0;
int __weak g_sensorhub_wdt_irq = -1;
uint32_t __weak need_set_3_1v_io_power;
uint32_t __weak need_set_3_2v_io_power;
uint32_t __weak no_need_sensor_ldo24;
uint32_t __weak need_reset_io_power;
int __weak g_iom3_state = IOM3_ST_NORMAL;
int __weak iom3_power_state = ST_POWERON;
struct regulator *__weak sensorhub_vddio;
void __weak enable_key_when_recovery_iom3(void)
{}
void __weak reset_logbuff(void)
{}
void __weak emg_flush_logbuff(void);
int __weak sensor_set_cfg_data(void);
int __weak send_fileid_to_mcu(void);
void __weak inputhub_route_exit(void);
/***************__weak end****************/

__weak BLOCKING_NOTIFIER_HEAD(iom3_recovery_notifier_list);

wait_queue_head_t __weak iom3_rec_waitq;
atomic_t __weak iom3_rec_state = ATOMIC_INIT(IOM3_RECOVERY_UNINIT);
struct rdr_register_module_result __weak current_sh_info;
struct completion __weak sensorhub_rdr_completion;
void __weak *g_sensorhub_extend_dump_buff = NULL;
uint8_t __weak *g_sensorhub_dump_buff = NULL;
uint32_t __weak g_enable_dump = 1;
uint32_t __weak g_dump_extend_size;
uint32_t __weak g_dump_region_list[DL_BOTTOM][DE_BOTTOM];
struct completion __weak iom3_rec_done;
static uint32_t *g_dp_regs = NULL;

static struct workqueue_struct *g_iom3_rec_wq = NULL;
static struct delayed_work g_iom3_rec_work;
static struct wakeup_source g_iom3_rec_wl;
static void __iomem *g_iomcu_cfg_base = NULL;
static u64 g_current_sh_core_id = RDR_IOM3; /* const */
static struct semaphore g_rdr_sh_sem;
static struct semaphore g_rdr_exce_sem;
static void __iomem *g_sysctrl_base = NULL;
static void __iomem *g_watchdog_base = NULL;
static unsigned int *g_dump_vir_addr = NULL;
static struct wakeup_source g_rdr_wl;
static int g_nmi_reg;
static char g_dump_dir[PATH_MAXLEN] = SH_DMP_DIR;
static char g_dump_fs[PATH_MAXLEN] = SH_DMP_FS;
static uint32_t g_dump_index = (uint32_t)-1;
static u64 g_rdr_core_mask = RDR_IOM3 | RDR_AP | RDR_LPM3;
static bool g_sensorhub_dump_reset = true;
static atomic_t g_dump_minready_done = ATOMIC_INIT(1);

static const char *const g_sh_reset_reasons[] = {
	"SH_FAULT_HARDFAULT", // 0
	"SH_FAULT_BUSFAULT",       "SH_FAULT_USAGEFAULT",
	"SH_FAULT_MEMFAULT",       "SH_FAULT_NMIFAULT",
	"SH_FAULT_ASSERT", // 5
	"UNKNOW DUMP FAULT",       "UNKNOW DUMP FAULT",
	"UNKNOW DUMP FAULT",       "UNKNOW DUMP FAULT",
	"UNKNOW DUMP FAULT", // 10
	"UNKNOW DUMP FAULT",       "UNKNOW DUMP FAULT",
	"UNKNOW DUMP FAULT",       "UNKNOW DUMP FAULT",
	"UNKNOW DUMP FAULT",      // 15
	"SH_FAULT_INTERNELFAULT", // 16
	"SH_FAULT_IPC_RX_TIMEOUT", "SH_FAULT_IPC_TX_TIMEOUT",
	"SH_FAULT_RESET",          "SH_FAULT_USER_DUMP",
	"SH_FAULT_RESUME",         "SH_FAULT_REDETECT",
	"SH_FAULT_PANIC",          "SH_FAULT_NOC",
	"SH_FAULT_EXP_BOTTOM", // also use as unknow dump
};

static const char * const g_sh_regs_name[] = {
	"SOURCE", "R0", "R1", "R2", "R3", "R4", "R5", "R6", "R7", "R8", "R9",
	"R10", "R11", "R12", "EXP_SP", "EXP_LR", "EXP_PC", "SAVED_PSR",
	"CFSR", "HFSR", "DFSR", "MMFAR", "BFAR", "AFSR", "PSP",
	"MSP", "ARADDR_CHK", "AWADDR_CHK", "PERI_USED", "AO_CNT",
};

static int iom3_need_recovery_for_dump(int modid, exp_source_t f);

static int get_watchdog_base(void)
{
	struct device_node *np = NULL;

	if (g_watchdog_base == NULL) {
		np = of_find_compatible_node(NULL, NULL, "hisilicon,iomcu-watchdog");
		if (np == NULL) {
			pr_err("can not find watchdog node\n");
			return -1;
		}

		g_watchdog_base = of_iomap(np, 0);
		if (g_watchdog_base == NULL) {
			pr_err("get g_watchdog_base  error !\n");
			return -1;
		}
	}
	return 0;
}

static int get_sysctrl_base(void)
{
	struct device_node *np = NULL;

	if (g_sysctrl_base == NULL) {
		np = of_find_compatible_node(NULL, NULL, "hisilicon,sysctrl");
		if (np == NULL) {
			pr_err("can not find  sysctrl node !\n");
			return -1;
		}

		g_sysctrl_base = of_iomap(np, 0);
		if (g_sysctrl_base == NULL) {
			pr_err("get g_sysctrl_base  error !\n");
			return -1;
		}
	}
	return 0;
}

static void __get_dump_region_list(struct device_node *dump_node,
	char *prop_name, dump_loc_t loc) {
	int i;
	int cnt;
	unsigned int de_index;
	struct property *prop = NULL;

	prop = of_find_property(dump_node, prop_name, NULL);
	pr_info("========= get dump region %s config from dts\n", prop_name);

	if ((prop == NULL) || (prop->value == NULL)) {
		pr_err("%s prop %s invalid!\n", __func__, prop_name);
		return;
	}
	cnt = prop->length / sizeof(uint32_t);
	if ((cnt > DE_BOTTOM) || (loc < DL_TCM) || (loc - DL_TCM >= DL_BOTTOM)) {
		pr_err("%s check cnt %d or loc %u fail!\n", __func__, cnt, (uint32_t)loc);
		return;
	}

	if (of_property_read_u32_array(dump_node, prop_name, g_dump_region_list[loc - DL_TCM], cnt)) {
		pr_err("%s read %s from dts fail!\n", __func__, prop_name);
		return;
	}
	for (i = 0; i < cnt; i++) {
		de_index = g_dump_region_list[loc - DL_TCM][i];
		if (de_index >= DE_BOTTOM) {
			pr_err("%s check de_index %u fail! loc %u, i %d\n", __func__, de_index, (uint32_t)loc, i);
			return;
		}
		g_smplat_scfg->dump_config.elements[de_index].loc = loc;
		pr_info("region [%d]\n", g_dump_region_list[loc - DL_TCM][i]);
	}
}

static void get_dump_config_from_dts(void)
{
	int ret;
	char *str_name = NULL;
	struct device_node *dump_node = NULL;

	dump_node = of_find_node_by_name(NULL, "sensorhub_dump");
	if (dump_node == NULL) {
		pr_err("%s failed to find dts node sensorhub_dump\n", __func__);
		goto OUT;
	}

	if (of_property_read_u32(dump_node, "enable", &g_enable_dump))
		pr_err("%s failed to find property enable\n", __func__);

	if (g_enable_dump == 0)
		goto OUT;

	__get_dump_region_list(dump_node, "tcm_regions", DL_TCM);

	if (of_property_read_u32(dump_node, "extend_region_size",
							 &g_dump_extend_size))
		pr_err("%s failed to find property extend_region_size\n", __func__);

	if (g_dump_extend_size)
		__get_dump_region_list(dump_node, "ext_regions", DL_EXT);

	if (of_property_read_string(dump_node, "dump_dir",(const char **)&str_name)) {
		pr_err("%s failed to find property dump_dir use default\n", __func__);
		ret = memset_s(g_dump_dir, sizeof(g_dump_dir), 0, sizeof(g_dump_dir));
		if (ret != EOK)
			pr_err("[%s]memset_s g_dump_dir fail[%d]\n", __func__, ret);
		ret = strncpy_s(g_dump_dir, PATH_MAXLEN, SH_DMP_DIR, PATH_MAXLEN - 1);
		if (ret != EOK)
			pr_err("strncpy_s error.\n");
	}

	if (of_property_read_string(dump_node, "dump_fs",(const char **)&str_name)) {
		pr_err("%s failed to find property dump_dir use default\n", __func__);
		ret = memset_s(g_dump_fs, sizeof(g_dump_fs), 0, sizeof(g_dump_fs));
		if (ret != EOK)
			pr_err("[%s]memset_s g_dump_dir fail[%d]\n", __func__, ret);

		ret = strncpy_s(g_dump_fs, PATH_MAXLEN, SH_DMP_FS, PATH_MAXLEN - 1);
		if (ret != EOK)
			pr_err("strncpy_s g_dump_fs error.\n");
	}

OUT:
	pr_info("DMPenabled[%d]ext size[0x%x]\n", g_enable_dump, g_dump_extend_size);
	pr_info("log dir is %s parent dir is %s\n", g_dump_dir, g_dump_dir);
	if (g_smplat_scfg != NULL)
		g_smplat_scfg->dump_config.enable = g_enable_dump;
}

int write_ramdump_info_to_sharemem(void)
{
	if (g_smplat_scfg == NULL) {
		pr_err("[%s] g_smplat_scfg is NULL\n", __func__);
		return -1;
	}

	g_smplat_scfg->dump_config.finish = SH_DUMP_INIT;
	g_smplat_scfg->dump_config.dump_addr = SENSORHUB_DUMP_BUFF_ADDR;
	g_smplat_scfg->dump_config.dump_size = SENSORHUB_DUMP_BUFF_SIZE;
	g_smplat_scfg->dump_config.ext_dump_addr = 0;
	g_smplat_scfg->dump_config.ext_dump_size = 0;
	g_smplat_scfg->dump_config.reason = 0;

	pr_info("%s dumpaddr low is %x, dumpaddr high is %x, dumplen is %x, end!\n",
			__func__, (u32)(g_smplat_scfg->dump_config.dump_addr),
			(u32)((g_smplat_scfg->dump_config.dump_addr) >> 32),
			g_smplat_scfg->dump_config.dump_size);
	return 0;
}

#ifdef CONFIG_CONTEXTHUB_BOOT_STAT
static void get_boot_stat(void)
{
	union sctrl_sh_boot sh_boot;

	if (g_sysctrl_base != NULL)
		sh_boot.value = readl(g_sysctrl_base + BOOT_STAT_REG_OFFSET);
	else
		sh_boot.value = 0;

	pr_info("%s: sensorhub boot_step=%u, boot_stat=%u\n", __func__, sh_boot.reg.boot_step, sh_boot.reg.boot_stat);
}

static void reset_boot_stat(void)
{
	if (g_sysctrl_base != NULL)
		writel(0, g_sysctrl_base + BOOT_STAT_REG_OFFSET);
}
#endif

static void __send_nmi(void)
{
#ifdef CONFIG_CONTEXTHUB_BOOT_STAT
	get_boot_stat();
	reset_boot_stat();
#endif
	pr_warn("%s: g_sysctrl_base is %pK, nmi_reg is 0x%x\n", __func__, g_sysctrl_base, g_nmi_reg);
	if (g_sysctrl_base != NULL && g_nmi_reg != 0) {
		writel(0x2, g_sysctrl_base + g_nmi_reg); /* 0x2 only here */
		pr_err("IOMCU NMI\n");
	}
}

static void notify_rdr_thread(void)
{ up(&g_rdr_sh_sem); }

#ifdef CONFIG_HISI_BB
static void sh_dump(u32 modid, u32 etype, u64 coreid, char *pathname, pfn_cb_dump_done pfn_cb)
{
	exp_source_t f = SH_FAULT_REACT;

#ifdef CONFIG_CONTEXTHUB_SWING
	if (modid == SENSORHUB_FDUL_MODID)
		f = SH_FAULT_NOC;
#endif
	pr_warn("%s: modid=0x%x, etype=0x%x, coreid=0x%llx, f=%d\n", __func__, modid, etype, coreid, f);

	if (modid_react_dump(modid))
		iom3_need_recovery_for_dump(modid, f);

	if (pfn_cb)
		pfn_cb(modid, g_current_sh_core_id);
}

static void sh_reset(u32 modid, u32 etype, u64 coreid)
{
	int ret;

	pr_info("%s\n", __func__);

	if ((g_dump_vir_addr != NULL) && (current_sh_info.log_len != 0)) {
		ret = memset_s(g_dump_vir_addr, current_sh_info.log_len, 0, current_sh_info.log_len);
		if (ret != EOK)
			pr_err("[%s]memset_s  fail[%d]\n", __func__, ret);
	}
}

static int clean_rdr_memory(struct rdr_register_module_result rdr_info)
{
	int ret;
	g_dump_vir_addr =
		(unsigned int *)hisi_bbox_map(rdr_info.log_addr, rdr_info.log_len);
	if (g_dump_vir_addr == NULL) {
		pr_err("hisi_bbox_map (%llx) failed in %s!\n", rdr_info.log_addr, __func__);
		return -1;
	}

	ret = memset_s(g_dump_vir_addr, rdr_info.log_len, 0, rdr_info.log_len);
	if (ret != EOK)
		pr_err("[%s]memset_s  fail[%d]\n", __func__, ret);
	return 0;
}

static int write_bbox_log_info_to_sharemem(struct rdr_register_module_result *rdr_info)
{
	if (g_smplat_scfg == NULL) {
		pr_err("write_bbox_log_info_to_sharemem: g_smplat_scfg is NULL\n");
		return -1;
	}

	if (rdr_info == NULL) {
		pr_err("write_bbox_log_info_to_sharemem: rdr_info is NULL\n");
		return -1;
	}

	g_smplat_scfg->bbox_conifg.log_addr = rdr_info->log_addr;
	g_smplat_scfg->bbox_conifg.log_size = rdr_info->log_len;

	pr_info("write_bbox_log_info_to_sharemem: bbox log addr: low 0x%x, high 0x%x, log len: 0x%x\n",
			(u32)(rdr_info->log_addr), (u32)((rdr_info->log_addr) >> 32), rdr_info->log_len);
	return 0;
}
static int rdr_sensorhub_register_core(void)
{
	struct rdr_module_ops_pub s_module_ops;
	int ret;

	pr_info("%s start!\n", __func__);

	s_module_ops.ops_dump = sh_dump;
	s_module_ops.ops_reset = sh_reset;

	ret = rdr_register_module_ops(g_current_sh_core_id, &s_module_ops,
								  &current_sh_info);
	if (ret != 0)
		return ret;

	ret = clean_rdr_memory(current_sh_info);
	if (ret != 0)
		return ret;

	ret = write_bbox_log_info_to_sharemem(&current_sh_info);
	if (ret != 0)
		return ret;

	pr_info("%s end!\n", __func__);
	return ret;
}

static const char sh_module_name[] = "RDR_IOM7";
static const char sh_module_desc[] = "RDR_IOM7 for watchdog timeout issue.";
static const char sh_module_user_desc[] = "RDR_IOM7 for user trigger dump.";
static const char sh_module_fdul_desc[] = "RDR_IOM7 for FD_UL trigger dump.";
static const char sh_module_tiny_desc[] = "RDR_IOM7 for NPU_TINY trigger dump.";

#ifdef CONFIG_CONTEXTHUB_SWING
#ifdef CONFIG_HISI_NOC_MODID_REGISTER
static void rdr_sh_fdul_e_callback(u32 modid, void *arg1)
{
	/* this callback is called by other core */
	return;
}
static void rdr_sh_npu_e_callback(u32 modid, void *arg1)
{
	/* this callback is called by other core */
	return;
}

#endif

static int swing_register_exception(void)
{
	struct rdr_exception_info_s einfo = {{0}};
	int ret;

	einfo.e_modid = SENSORHUB_FDUL_MODID;
	einfo.e_modid_end = SENSORHUB_FDUL_MODID;
	einfo.e_exce_type = IOM3_S_FDUL_EXCEPTION;
	einfo.e_notify_core_mask = g_rdr_core_mask;
        einfo.e_reset_core_mask = RDR_IOM3;
#ifdef CONFIG_HISI_NOC_MODID_REGISTER
        einfo.e_callback = rdr_sh_fdul_e_callback;
#endif
	einfo.e_process_priority = RDR_WARN;
	einfo.e_reboot_priority = RDR_REBOOT_NO;
	einfo.e_reentrant = RDR_REENTRANT_ALLOW;
	einfo.e_from_core = RDR_IOM3;
	einfo.e_upload_flag = RDR_UPLOAD_YES;
	einfo.e_save_log_flags = RDR_SAVE_LOGBUF;

	ret = memcpy_s(einfo.e_from_module, MODULE_NAME_LEN, sh_module_name,
		       sizeof(sh_module_name) > MODULE_NAME_LEN ? MODULE_NAME_LEN : sizeof(sh_module_name));
	if (ret != EOK) {
		pr_err("[%s]memcpy_s fail[%d]\n", __func__, ret);
		return -EINVAL;
	}

	ret = memcpy_s(einfo.e_desc,STR_EXCEPTIONDESC_MAXLEN, sh_module_fdul_desc, sizeof(sh_module_fdul_desc));
	if (ret != EOK) {
		pr_err("[%s]memcpy_s fail[%d]\n", __func__, ret);
		return -EINVAL;
	}
	ret = rdr_register_exception(&einfo);
	if (ret == 0) {
		pr_err("%s: register SENSORHUB_FDUL_MODID failed[%d]\n", __func__, ret);
		return -EINVAL;
	}
#ifdef CONFIG_CONTEXTHUB_SWING_20
	einfo.e_modid = SENSORHUB_TINY_MODID;
	einfo.e_modid_end = SENSORHUB_TINY_MODID;
	einfo.e_exce_type = IOM3_S_FDUL_EXCEPTION;
	einfo.e_notify_core_mask = g_rdr_core_mask;
	einfo.e_reset_core_mask = RDR_IOM3;
#ifdef CONFIG_HISI_NOC_MODID_REGISTER
	einfo.e_callback = rdr_sh_npu_e_callback;
#endif
	einfo.e_process_priority = RDR_WARN;
	einfo.e_reboot_priority = RDR_REBOOT_NO;
	einfo.e_reentrant = RDR_REENTRANT_ALLOW;
	einfo.e_from_core = RDR_IOM3;
	einfo.e_upload_flag = RDR_UPLOAD_YES;
	einfo.e_save_log_flags = RDR_SAVE_LOGBUF;

	ret = memcpy_s(einfo.e_from_module, MODULE_NAME_LEN, sh_module_name,
		       sizeof(sh_module_name) > MODULE_NAME_LEN ? MODULE_NAME_LEN : sizeof(sh_module_name));
	if (ret != EOK) {
		pr_err("[%s]memcpy_s fail[%d]\n", __func__, ret);
		return -EINVAL;
	}

	ret = memcpy_s(einfo.e_desc,STR_EXCEPTIONDESC_MAXLEN, sh_module_tiny_desc, sizeof(sh_module_tiny_desc));
	if (ret != EOK) {
		pr_err("[%s]memcpy_s fail[%d]\n", __func__, ret);
		return -EINVAL;
	}
	ret = rdr_register_exception(&einfo);
	if (ret == 0) {
		pr_err("%s: register SENSORHUB_TINY_MODID failed[%d]\n", __func__, ret);
		return -EINVAL;
	}
#endif
	return 0;
}
#endif /* CONFIG_CONTEXTHUB_SWING */

static int sh_rdr_register_exception(void)
{
	struct rdr_exception_info_s einfo;
	int ret;

	pr_info("%s start!\n", __func__);
	ret = memset_s(&einfo, sizeof(einfo), 0, sizeof(einfo));
	if (ret != EOK) {
		pr_err("[%s]memset_s fail[%d]\n", __func__, ret);
		return -EINVAL;
	}
	einfo.e_modid = SENSORHUB_MODID;
	einfo.e_modid_end = SENSORHUB_MODID;
	einfo.e_process_priority = RDR_WARN;
	einfo.e_reboot_priority = RDR_REBOOT_NO;
	einfo.e_notify_core_mask = g_rdr_core_mask;
	einfo.e_reentrant = RDR_REENTRANT_ALLOW;
	einfo.e_exce_type = IOM3_S_EXCEPTION;
	einfo.e_from_core = RDR_IOM3;
	einfo.e_upload_flag = RDR_UPLOAD_YES;
	einfo.e_save_log_flags = RDR_SAVE_LOGBUF;
	ret = memcpy_s(einfo.e_from_module, MODULE_NAME_LEN, sh_module_name,
		       sizeof(sh_module_name) > MODULE_NAME_LEN ? MODULE_NAME_LEN : sizeof(sh_module_name));
	if (ret != EOK) {
		pr_err("[%s]memcpy_s fail[%d]\n", __func__, ret);
		return -EINVAL;
	}
	ret = memcpy_s(einfo.e_desc, STR_EXCEPTIONDESC_MAXLEN, sh_module_desc,
		       (sizeof(sh_module_desc) > STR_EXCEPTIONDESC_MAXLEN) ?
		       STR_EXCEPTIONDESC_MAXLEN : sizeof(sh_module_desc));
	if (ret != EOK) {
		pr_err("[%s]memcpy_s fail[%d]\n", __func__, ret);
		return -EINVAL;
	}
	ret = rdr_register_exception(&einfo);
	if (ret == 0) {
		pr_err("%s: register SENSORHUB_MODID failed[%d]\n", __func__, ret);
		return -EINVAL;
	}

	einfo.e_modid = SENSORHUB_USER_MODID;
	einfo.e_modid_end = SENSORHUB_USER_MODID;
	einfo.e_exce_type = IOM3_S_USER_EXCEPTION;
	ret = memcpy_s(einfo.e_desc, STR_EXCEPTIONDESC_MAXLEN, sh_module_user_desc,
		       (sizeof(sh_module_user_desc) > STR_EXCEPTIONDESC_MAXLEN) ?
		       STR_EXCEPTIONDESC_MAXLEN : sizeof(sh_module_user_desc));
	if (ret != EOK) {
		pr_err("[%s]memcpy_s fail[%d]\n", __func__, ret);
		return -EINVAL;
	}
	ret = rdr_register_exception(&einfo);
	if (ret == 0) {
		pr_err("%s: register SENSORHUB_USER_MODID failed[%d]\n", __func__, ret);
		return -EINVAL;
	}

#ifdef CONFIG_CONTEXTHUB_SWING
	ret = swing_register_exception();
	if (ret != 0) {
		pr_err("%s: swing_register_exception failed[%d]\n", __func__, ret);
		return -EINVAL;
	}
#endif
	return 0;
}

#ifdef CONFIG_HISI_DEBUG_FS
#ifdef CONFIG_HISI_BB
static void sh_rdr_unregister_exception(void)
{
	(void)rdr_unregister_exception(SENSORHUB_MODID);
	(void)rdr_unregister_exception(SENSORHUB_USER_MODID);

#ifdef CONFIG_CONTEXTHUB_SWING
	(void)rdr_unregister_exception(SENSORHUB_FDUL_MODID);
#endif
}
#endif
#endif

static int sh_register_exception(void)
{
#ifdef CONFIG_CONTEXTHUB_SWING
#ifdef CONFIG_HISI_NOC_MODID_REGISTER
	struct noc_err_para_s noc_err_info;
#endif
#endif
	int ret = sh_rdr_register_exception();
	if (ret != 0)
		return ret;

#ifdef CONFIG_CONTEXTHUB_SWING
#ifdef CONFIG_HISI_NOC_MODID_REGISTER
	noc_err_info.masterid = SOC_FD_UL_MID;
	noc_err_info.targetflow = 0xFF; /* any target flow */
	noc_err_info.bus = NOC_ERRBUS_SYS_CONFIG;
	noc_modid_register(noc_err_info, SENSORHUB_FDUL_MODID);

#ifdef CONFIG_CONTEXTHUB_SWING_20
	noc_err_info.masterid = SOC_NPU_TINY_MID;
	noc_err_info.targetflow = 0xFF; /* any target flow */
	noc_err_info.bus = NOC_ERRBUS_NPU;
	noc_modid_register(noc_err_info, SENSORHUB_TINY_MODID);

	noc_err_info.masterid = SOC_NPU_TINY_MID;
	noc_err_info.targetflow = 0xFF; /* any target flow */
	noc_err_info.bus = NOC_ERRBUS_SYS_CONFIG;
	noc_modid_register(noc_err_info, SENSORHUB_TINY_MODID);
#endif
#endif
#endif

	return ret;
}
#endif // CONFIG_HISI_BB

static void wdt_stop(void)
{/*only here*/
	if (g_watchdog_base == NULL) {
		pr_err("[%s] g_watchdog_base is NULL!\n", __func__);
		return;
	}
	writel(REG_UNLOCK_KEY, g_watchdog_base + WDOGLOCK);
	writel(0x0, g_watchdog_base + WDOGCTRL);
	writel(0x1, g_watchdog_base + WDOGINTCLR);
	writel(0x01, g_watchdog_base + WDOGLOCK);
}

static irqreturn_t watchdog_handler(int irq, void *data)
{
	wdt_stop();
	pr_warning("%s start\n", __func__);
	__pm_stay_awake(&g_rdr_wl);
	/* release exception sem */
	up(&g_rdr_exce_sem);
	return IRQ_HANDLED;
}
/* lint -e501 */
static int __sh_create_dir(char *path, unsigned int len)
{
	int fd;
	mm_segment_t old_fs;

	if (len > CUR_PATH_LEN) {
		pr_err("invalid  parameter. len is %d\n", len);
		return -1;
	}
	if (path == NULL) {
		pr_err("null path\n");
		return -1;
	}

	old_fs = get_fs();
	set_fs(KERNEL_DS);
	fd = sys_access(path, 0);
	if (fd != 0) {
		pr_info("sh: need create dir %s\n", path);
		fd = sys_mkdir(path, DIR_LIMIT);
		if (fd < 0) {
			pr_err("sh: create dir %s failed! ret = %d\n", path, fd);
			set_fs(old_fs);
			return fd;
		}

		pr_info("sh: create dir %s successed [%d]!!!\n", path, fd);
	}
	set_fs(old_fs);
	return 0;
}

static int sh_wait_fs(const char *path)
{
	int fd = 0;
	int ret = 0;
	unsigned int wait_index = 0;
	unsigned int wait_max = 1000;
	mm_segment_t old_fs;

	old_fs = get_fs();
	set_fs(KERNEL_DS);

	do {
		fd = sys_access(path, 0);
		if (fd != 0) {
			msleep(10);
			pr_info("[%s] wait ...\n", __func__);
			wait_index++;
		}
	} while ((fd != 0) && (wait_index <= wait_max));
	if (fd != 0) {
		ret = -1;
		pr_err("[%s] cannot access path! errno %d\n", __func__, fd);
	}
	set_fs(old_fs);
	return ret;
}

static int sh_savebuf2fs(char *logpath, char *filename, void *buf, u32 len,
						 u32 is_append) {
	int ret = 0;
	int flags;
	struct file *fp = NULL;
	mm_segment_t old_fs;
	char path[PATH_MAXLEN];

	if ((logpath == NULL) || (filename == NULL) || (buf == NULL) || (len <= 0)) {
		pr_err("invalid  parameter. path:%pK, name:%pK buf:%pK len:0x%x\n", logpath,
			   filename, buf, len);
		ret = -1;
		goto FIN;
	}

	if (sprintf_s(path, PATH_MAXLEN, "%s/%s", logpath, filename) < 0) {
		pr_err("%s():path length is too large\n", __func__);
		return -1;
	}

	old_fs = get_fs();
	set_fs(KERNEL_DS);

	flags = O_CREAT | O_RDWR | (is_append ? O_APPEND : O_TRUNC);
	fp = filp_open(path, flags, FILE_LIMIT);
	if (IS_ERR(fp)) {
		set_fs(old_fs);
		pr_err("%s():create file %s err.\n", __func__, path);
		ret = -1;
		goto FIN;
	}

	vfs_llseek(fp, 0L, SEEK_END);
	ret = vfs_write(fp, buf, len, &(fp->f_pos));
	if (ret != len) {
		pr_err("%s:write file %s exception with ret %d.\n", __func__, path, ret);
		goto FS_CLOSE;
	}

	vfs_fsync(fp, 0);
FS_CLOSE:
	filp_close(fp, NULL);

	ret = (int)sys_chown((const char __user *)path, ROOT_UID, SYSTEM_GID);
	if (ret)
		pr_err("[%s], chown %s uid [%d] gid [%d] failed err [%d]!\n", __func__,
			   path, ROOT_UID, SYSTEM_GID, ret);

	set_fs(old_fs);
FIN:
	return ret;
}

static int sh_readfs2buf(char *logpath, char *filename, void *buf, u32 len)
{
	int ret = -1;
	int flags;
	struct file *fp = NULL;
	mm_segment_t old_fs;
	char path[PATH_MAXLEN];

	if ((logpath == NULL) || (filename == NULL) || (buf == NULL) || (len <= 0)) {
		pr_err("invalid  parameter. path:%pK, name:%pK buf:%pK len:0x%x\n", logpath,
			   filename, buf, len);
		goto FIN;
	}

	if (sprintf_s(path, PATH_MAXLEN, "%s/%s", logpath, filename) < 0) {
		pr_err("%s():path length is too large\n", __func__);
		return -1;
	}

	old_fs = get_fs();
	set_fs(KERNEL_DS);

	if (sys_access(path, 0) != 0)
		goto SET_OLD_FS;

	flags = O_RDWR;
	fp = filp_open(path, flags, FILE_LIMIT);
	if (IS_ERR(fp)) {
		pr_err("%s():open file %s err.\n", __func__, path);
		goto SET_OLD_FS;
	}

	vfs_llseek(fp, 0L, SEEK_SET);
	ret = vfs_read(fp, buf, len, &(fp->f_pos));
	if (ret != len) {
		pr_err("%s:read file %s exception with ret %d.\n", __func__, path, ret);
		ret = -1;
	}
	filp_close(fp, NULL);
SET_OLD_FS:
	set_fs(old_fs);
FIN:
	return ret;
}

static int sh_create_dir(const char *path)
{
	char cur_path[CUR_PATH_LEN];
	int index = 0;

	if (path == NULL) {
		pr_err("invalid  parameter. path:%pK\n", path);
		return -1;
	}
	(void)memset_s(cur_path, CUR_PATH_LEN, 0, CUR_PATH_LEN);
	if (*path != '/')
		return -1;
	cur_path[index++] = *path++;
	while ((*path != '\0') && (index < CUR_PATH_LEN)) {
		if (*path == '/')
			__sh_create_dir(cur_path, CUR_PATH_LEN);

		cur_path[index] = *path;
		path++;
		index++;
		if (index >= CUR_PATH_LEN) {
			pr_err("[%s]invalid index %d. path:%pK\n", __func__, index, path);
			return -1;
		}
	}
	return 0;
}

static int get_dump_reason_idx(void)
{
	if (g_smplat_scfg == NULL)
		return ARRAY_SIZE(g_sh_reset_reasons) - 1;
	if (g_smplat_scfg->dump_config.reason >= ARRAY_SIZE(g_sh_reset_reasons)) {
		return ARRAY_SIZE(g_sh_reset_reasons) - 1;
	} else {
		return g_smplat_scfg->dump_config.reason;
	}
}

static int write_sh_dump_history(void)
{
	int ret = 0;
	char buf[HISTORY_LOG_SIZE];
	struct kstat historylog_stat;
	mm_segment_t old_fs;
	char local_path[PATH_MAXLEN];
	char date[DATATIME_MAXLEN] = {0};

	pr_info("%s: write sensorhub dump history file\n", __func__);
	ret = memset_s(date, DATATIME_MAXLEN, 0, DATATIME_MAXLEN);
	if (ret != EOK)
		pr_err("[%s]memset_s date fail[%d]\n", __func__, ret);
#ifdef CONFIG_HISI_BB
	ret = snprintf_s(date, DATATIME_MAXLEN, DATATIME_MAXLEN - 1, "%s-%08lld", rdr_get_timestamp(),
			 rdr_get_tick());
	if (ret < 0)
		pr_err("[%s]sprintf_s date fail[%d]\n", __func__, ret);
#endif

	ret = memset_s(local_path, PATH_MAXLEN, 0, PATH_MAXLEN);
	if (ret != EOK)
		pr_err("[%s]memset_s local_path fail[%d]\n", __func__, ret);
	ret = sprintf_s(local_path, PATH_MAXLEN, "%s/%s", g_dump_dir, "history.log");
	if (ret < 0)
		pr_err("[%s]sprintf_s local_path fail[%d]\n", __func__, ret);
	old_fs = get_fs();
	set_fs(KERNEL_DS);

	/* check file size */
	if (vfs_stat(local_path, &historylog_stat) == 0 &&
		historylog_stat.size > HISTORY_LOG_MAX) {
		pr_info("truncate dump history log\n");
		sys_unlink(local_path); /* delete history.log */
	}

	set_fs(old_fs);
	/* write history file */
	ret = memset_s(buf, HISTORY_LOG_SIZE, 0, HISTORY_LOG_SIZE);
	if (ret != EOK)
		pr_err("[%s]memset_s local_path fail[%d]\n", __func__, ret);

	if (g_dp_regs != NULL) {
		if (snprintf_s(buf, sizeof(buf), sizeof(buf) - 1,
			"reason [%s], [%02d], time [%s], regs [pc-%x,lr-%x,fsr[c-%x,h-%x],far[m-%x,b-%x],peri-%x,ao-%x]\n",
			g_sh_reset_reasons[get_dump_reason_idx()], g_dump_index, date, g_dp_regs[EXP_PC],
			g_dp_regs[EXP_LR], g_dp_regs[CFSR], g_dp_regs[HFSR], g_dp_regs[MMFAR],
			g_dp_regs[BFAR], g_dp_regs[PERI_USED], g_dp_regs[AO_CNT]) < 0)
			pr_err("%s write dp regs history failed\n", __func__);
	} else {
		if (snprintf_s(buf, sizeof(buf), sizeof(buf) - 1, "reason [%s], [%02d], time [%s]\n",
			g_sh_reset_reasons[get_dump_reason_idx()], g_dump_index, date) < 0)
			pr_err("%s write history failed\n", __func__);
	}

	(void)sh_savebuf2fs(g_dump_dir, "history.log", buf, strlen(buf), 1);
	return ret;
}
/* lint +e501 */
static void get_max_dump_cnt(void)
{
	int ret;
	uint32_t index;

	/* find max index */
	ret = sh_readfs2buf(g_dump_dir, "dump_max", &index, sizeof(index));
	if (ret < 0) {
		g_dump_index = (uint32_t)-1;
	} else {
		g_dump_index = index;
	}
	g_dump_index++;
	if (g_dump_index == MAX_DUMP_CNT)
		g_dump_index = 0;
	sh_savebuf2fs(g_dump_dir, "dump_max", &g_dump_index, sizeof(g_dump_index),
				  0);
}

static int write_sh_dump_file(void)
{
	int ret;
	char date[DATATIME_MAXLEN];
	char path[PATH_MAXLEN];
	dump_zone_head_t *dzh = NULL;

	ret = memset_s(date, DATATIME_MAXLEN, 0, DATATIME_MAXLEN);
	if (ret != EOK)
		pr_err("[%s]memset_s data fail[%d]\n", __func__, ret);
#ifdef CONFIG_HISI_BB
	ret = snprintf_s(date, DATATIME_MAXLEN, DATATIME_MAXLEN - 1, "%s-%08lld", rdr_get_timestamp(),
			 rdr_get_tick());
	if (ret < 0)
		pr_err("[%s]sprintf_s DATATIME_MAXLEN fail[%d]\n", __func__, ret);
#endif

	ret = memset_s(path, PATH_MAXLEN, 0, PATH_MAXLEN);
	if (ret != EOK)
		pr_err("[%s]memset_s path fail[%d]\n", __func__, ret);

	ret = sprintf_s(path, PATH_MAXLEN, "sensorhub-%02d.dmp", g_dump_index);
	if (ret < 0)
		pr_err("[%s]sprintf_s PATH_MAXLEN fail[%d]\n", __func__, ret);
	pr_info("%s: write sensorhub dump  file %s\n", __func__, path);
	pr_err("sensorhub recovery source is %s\n",
		   g_sh_reset_reasons[get_dump_reason_idx()]);
#if (KERNEL_VERSION(4, 4, 0) > LINUX_VERSION_CODE)
	flush_cache_all();
#endif

	/* write share part */
	if ((g_sensorhub_dump_buff != NULL) && (g_smplat_scfg != NULL)) {
		dzh = (dump_zone_head_t *)g_sensorhub_dump_buff;
		sh_savebuf2fs(g_dump_dir, path, g_sensorhub_dump_buff,
					  min(g_smplat_scfg->dump_config.dump_size, dzh->len), 0);
	}
	// write extend part
	if ((g_sensorhub_extend_dump_buff != NULL) && (g_smplat_scfg != NULL)) {
		dzh = (dump_zone_head_t *)g_sensorhub_extend_dump_buff;
		sh_savebuf2fs(g_dump_dir, path, g_sensorhub_extend_dump_buff,
					  min(g_smplat_scfg->dump_config.ext_dump_size, dzh->len), 1);
	}
	return 0;
}

static void get_sh_dump_regs(void)
{
	dump_zone_head_t *dzh = NULL;
	uint32_t i, magic;
	int32_t words = 0;
	int32_t ret;
	char buf[DUMP_REGS_SIZE] = {0};

	dzh = (dump_zone_head_t *)g_sensorhub_dump_buff;
	g_dp_regs = (uint32_t *)(g_sensorhub_dump_buff +
		sizeof(dump_zone_head_t) + sizeof(struct dump_zone_element_t) * dzh->cnt);
	if ((uintptr_t)(g_dp_regs + MAGIC) > (uintptr_t)(g_sensorhub_dump_buff + SENSORHUB_DUMP_BUFF_SIZE)) {
		pr_err("%s: ramdump maybe failed\n", __func__);
		g_dp_regs = NULL;
		return;
	}
	magic = g_dp_regs[MAGIC];
	pr_info("%s: dump regs magic 0x%x dump addr 0x%pK dp regs addr 0x%pK\n",
		__func__, magic, g_sensorhub_dump_buff, g_dp_regs);
	if (magic != DUMP_REGS_MAGIC) {
		g_dp_regs = NULL;
		return;
	}

	/* output it to kmsg */
	for (i = SOURCE; i < MAGIC; i++) {
		ret = snprintf_s(buf + words, DUMP_REGS_SIZE - words, REG_NAME_SIZE,
			"%s--%08x, ", g_sh_regs_name[i], g_dp_regs[i]);
		if (ret < 0)
			return;
		words += ret;
		if (DUMP_REGS_SIZE - words < REG_NAME_SIZE + LOG_EXTRA_NUM) {
			buf[words] = '\n';
			buf[words + 1] = '\0';
			pr_info("%s", buf);
			words = 0;
		}
	}
}

int save_sh_dump_file(void)
{
	if (g_enable_dump == 0) {
		pr_info("%s skipped!\n", __func__);
		return 0;
	}
	if (sh_wait_fs(g_dump_fs) != 0) {
		pr_err("[%s] fs not ready!\n", __func__);
		return -1;
	}
	pr_info("%s fs ready\n", __func__);
	// check and create dump dir
	if (sh_create_dir(g_dump_dir)) {
		pr_err("%s failed to create dir %s\n", __func__, g_dump_dir);
		return -1;
	}
	get_max_dump_cnt();
	get_sh_dump_regs();
	// write history file
	write_sh_dump_history();
	// write dump file
	write_sh_dump_file();
	return 0;
}

static int rdr_sh_thread(void *arg)
{
	int timeout;

	pr_warning("%s start!\n", __func__);

	while (1) {
		timeout = 2000;
		down(&g_rdr_sh_sem);

		if (g_enable_dump) {
			pr_warning(" ===========dump sensorhub log start==========\n");
			while (g_smplat_scfg->dump_config.finish != SH_DUMP_FINISH && timeout--)
				mdelay(1);
			pr_warning(" ===========sensorhub dump finished==========\n");
			pr_warning("dump reason idx %d\n", g_smplat_scfg->dump_config.reason);
			/* write to fs */
			save_sh_dump_file();
			/* free buff */
			if (g_sensorhub_extend_dump_buff) {
				pr_info("%s free alloc\n", __func__);
				kfree(g_sensorhub_extend_dump_buff);
				g_sensorhub_extend_dump_buff = NULL;
			}
			pr_warning(" ===========dump sensorhub log end==========\n");
		}
		__pm_relax(&g_rdr_wl);
		complete_all(&sensorhub_rdr_completion);
#ifdef __LLT_UT__
	break;
#endif
	}

	return 0;
}
void free_sh_dump_buff(void)
{
	if (g_sensorhub_extend_dump_buff != NULL) {
		pr_info("%s free alloc\n", __func__);
		kfree(g_sensorhub_extend_dump_buff);
		g_sensorhub_extend_dump_buff = NULL;
	}
}

static int rdr_exce_thread(void *arg)
{
	pr_warning("%s start!\n", __func__);
	while (1) {
		down(&g_rdr_exce_sem);
		pr_warning(" ==============trigger exception==============\n");
		iom3_need_recovery(SENSORHUB_MODID, SH_FAULT_INTERNELFAULT);
#ifdef __LLT_UT__
	break;
#endif
	}

	return 0;
}

int register_iom3_recovery_notifier(struct notifier_block *nb)
{
	return blocking_notifier_chain_register(&iom3_recovery_notifier_list, nb);
}


static void notify_modem_when_iomcu_recovery_finish(void)
{
	uint16_t status = ST_RECOVERY_FINISH;
	write_info_t pkg_ap = {0};
	int ret;

	pr_info("notify_modem_when_iomcu_recovery_finish\n");

	pkg_ap.tag = TAG_SYS;
	pkg_ap.cmd = CMD_SYS_STATUSCHANGE_REQ;
	pkg_ap.wr_buf = &status;
	pkg_ap.wr_len = sizeof(status);
	ret = write_customize_cmd(&pkg_ap, NULL, false);
	if (ret != 0)
		pr_warn("[%s]write_customize_cmd err ret[%d]\n", __func__, ret);
}

#ifdef CONFIG_HISI_BB
/*
 * return value
 * 0:success
 * <0:error
 */
/* lint -e446 */
int rdr_sensorhub_init_early(void)
{
	int ret;
	/* register module. */
	ret = rdr_sensorhub_register_core();
	if (ret != 0)
		return ret;
	/* write ramdump info to share mem */
	ret = write_ramdump_info_to_sharemem();
	if (ret != 0)
		return ret;
	get_dump_config_from_dts();
	g_sensorhub_dump_buff =
		(uint8_t *)ioremap_wc(SENSORHUB_DUMP_BUFF_ADDR, SENSORHUB_DUMP_BUFF_SIZE);
	if (g_sensorhub_dump_buff == NULL) {
		pr_err("%s failed remap dump buff\n", __func__);
		return -EINVAL;
	}

	ret = sh_register_exception();
	if (ret != 0)
		pr_err("[%s] sh_register_exception failed, ret %d\n", __func__, ret);

	return ret;
}
/* lint +e446 */
#endif

static int sensorhub_panic_notify(struct notifier_block *nb,
	unsigned long action, void *data) {
	int timeout;

	pr_warning("%s start\n", __func__);
	timeout = 100;
	__send_nmi();
	pr_warning("sensorhub_panic_notify\n");

	while ((g_smplat_scfg->dump_config.finish != SH_DUMP_FINISH) && (timeout--))
		mdelay(1);
	pr_warning("%s done\n", __func__);
	return NOTIFY_OK;
}

static struct notifier_block sensorhub_panic_block = {
	.notifier_call = sensorhub_panic_notify,
};

static int get_nmi_offset(void)
{
	struct device_node *sh_node = NULL;

	sh_node = of_find_compatible_node(NULL, NULL, "huawei,sensorhub_nmi");
	if (!sh_node) {
		pr_err("%s, can not find node sensorhub_nmi \n", __func__);
		return -1;
	}

	if (of_property_read_u32(sh_node, "nmi_reg", &g_nmi_reg)) {
		pr_err("%s:read nmi reg err:value is %d \n", __func__, g_nmi_reg);
		return -1;
	}

	pr_info("%s arch nmi reg is 0x%x\n", __func__, g_nmi_reg);
	return 0;
}

static int get_iomcu_cfg_base(void)
{
	struct device_node *np = NULL;

	if (g_iomcu_cfg_base == NULL) {
		np = of_find_compatible_node(NULL, NULL, "hisilicon,iomcuctrl");
		if (!np) {
			pr_err("can not find  iomcuctrl node !\n");
			return -1;
		}

		g_iomcu_cfg_base = of_iomap(np, 0);
		if (g_iomcu_cfg_base == NULL) {
			pr_err("get g_iomcu_cfg_base  error !\n");
			return -1;
		}
	}

	return 0;
}

static int get_iomcu_wdt_irq_num(void)
{
	struct device_node *np = NULL;

	if (g_sensorhub_wdt_irq < 0) {
		np = of_find_compatible_node(NULL, NULL, "huawei,sensorhub_io");
		if (np == NULL) {
			pr_err("can not find  iomcuctrl node !\n");
			return -1;
		}
		g_sensorhub_wdt_irq = of_irq_get(np, 0);
	}

	return g_sensorhub_wdt_irq;
}

static void show_iom3_stat(void)
{
	if (g_iomcu_cfg_base == NULL) {
		pr_err("[%s]g_iomcu_cfg_base not init!\n", __func__);
		return;
	}

	pr_err("CLK_SEL:0x%x,DIV0:0x%x,DIV1:0x%x,CLKSTAT0:0x%x, RSTSTAT0:0x%x\n",
		readl(g_iomcu_cfg_base + IOMCU_CLK_SEL),
		readl(g_iomcu_cfg_base + IOMCU_CFG_DIV0),
		readl(g_iomcu_cfg_base + IOMCU_CFG_DIV1),
		readl(g_iomcu_cfg_base + CLKSTAT0_OFFSET),
		readl(g_iomcu_cfg_base + RSTSTAT0_OFFSET));
#ifdef CONFIG_CONTEXTHUB_BOOT_STAT
	get_boot_stat();
#endif
}

static void reset_i2c_0_controller(void)
{
	unsigned long flags;
	if (g_iomcu_cfg_base == NULL) {
		pr_err("[%s]g_iomcu_cfg_base not init!\n", __func__);
		return;
	}

	local_irq_save(flags);
	writel(I2C_0_RST_VAL, g_iomcu_cfg_base + RSTEN0_OFFSET);
	udelay(5);
	writel(I2C_0_RST_VAL, g_iomcu_cfg_base + RSTDIS0_OFFSET);
	local_irq_restore(flags);
}

static void reset_sensor_power(void)
{
	int ret = 0;

	if (no_need_sensor_ldo24) {
		pr_info("%s: no_need set ldo24\n", __func__);
		return;
	}
	if (!need_reset_io_power) {
		pr_warning("%s: no need to reset sensor power\n", __func__);
		return;
	}

	if (IS_ERR(sensorhub_vddio)) {
		pr_err("%s: regulator_get fail!\n", __func__);
		return;
	}
	ret = regulator_disable(sensorhub_vddio);
	if (ret < 0) {
		pr_err("failed to disable regulator sensorhub_vddio\n");
		return;
	}
	msleep(10);
	if (need_set_3v_io_power) {
		ret = regulator_set_voltage(sensorhub_vddio, SENSOR_VOLTAGE_3V, SENSOR_VOLTAGE_3V);
		if (ret < 0) {
			pr_err("failed to set sensorhub_vddio voltage to 3V\n");
			return;
		}
	}
	if (need_set_3_1v_io_power) {
		ret = regulator_set_voltage(sensorhub_vddio, SENSOR_VOLTAGE_3_1V, SENSOR_VOLTAGE_3_1V);
		if (ret < 0) {
			pr_err("failed to set sensorhub_vddio voltage to 3_1V\n");
			return;
		}
	}
	if (need_set_3_2v_io_power) {
		ret = regulator_set_voltage(sensorhub_vddio, SENSOR_VOLTAGE_3_2V,
									SENSOR_VOLTAGE_3_2V);
		if (ret < 0) {
			pr_err("failed to set sensorhub_vddio voltage to 3_2V\n");
			return;
		}
	}
	ret = regulator_enable(sensorhub_vddio);
	msleep(5);
	if (ret < 0) {
		pr_err("failed to enable regulator sensorhub_vddio\n");
		return;
	}
	pr_info("%s done\n", __func__);
}

static void iomcu_rdr_notifier(void)
{
	/* repeat send cmd */
	msleep(100); /* wait iom3 finish handle config-data */
	atomic_set(&iom3_rec_state, IOM3_RECOVERY_DOING);
	pr_info("%s IOM3_RECOVERY_DOING\n", __func__);
	blocking_notifier_call_chain(&iom3_recovery_notifier_list,
								 IOM3_RECOVERY_DOING, NULL);
	enable_key_when_recovery_iom3();
	notify_modem_when_iomcu_recovery_finish();
	/* recovery pdr */
	blocking_notifier_call_chain(&iom3_recovery_notifier_list,
								 IOM3_RECOVERY_3RD_DOING, NULL);
	pr_err("%s pdr recovery\n", __func__);
	atomic_set(&iom3_rec_state, IOM3_RECOVERY_IDLE);
	__pm_relax(&g_iom3_rec_wl);
	pr_err("%s finish recovery\n", __func__);
	blocking_notifier_call_chain(&iom3_recovery_notifier_list, IOM3_RECOVERY_IDLE,
								 NULL);
	pr_err("%s exit\n", __func__);
}


static void iomcu_reinit(void)
{
	show_iom3_stat(); /* only for IOM3 debug */
	reset_i2c_0_controller();
	reset_logbuff();
	write_ramdump_info_to_sharemem();
	write_timestamp_base_to_sharemem();
	msleep(5);
	atomic_set(&iom3_rec_state, IOM3_RECOVERY_MINISYS);
}

static void iom3_recovery_work(struct work_struct *work)
{
	int rec_nest_count = 0;
	int rc;
	u32 ack_buffer;
	u32 tx_buffer;

	pr_err("%s enter\n", __func__);
	__pm_stay_awake(&g_iom3_rec_wl);
	wait_for_completion(&sensorhub_rdr_completion);

recovery_iom3:
	if (rec_nest_count++ > IOM3_REC_NEST_MAX) {
		pr_err("unlucky recovery iom3 times exceed limit\n");
		atomic_set(&iom3_rec_state, IOM3_RECOVERY_FAILED);
		blocking_notifier_call_chain(&iom3_recovery_notifier_list,
		IOM3_RECOVERY_FAILED, NULL);
		atomic_set(&iom3_rec_state, IOM3_RECOVERY_IDLE);
		__pm_relax(&g_iom3_rec_wl);
		pr_err("%s exit\n", __func__);
		return;
	}

	/* fix bug nmi can't be clear by iomcu, or iomcu will not start correctly */
	if ((unsigned int)readl(g_sysctrl_base + g_nmi_reg) & 0x2)
		pr_err("%s nmi remain!\n", __func__);
	writel(0, g_sysctrl_base + g_nmi_reg);

	show_iom3_stat(); /* only for IOM3 debug */
	reset_sensor_power();
	// reload iom3 system
	tx_buffer = RELOAD_IOM3_CMD;
	rc = RPROC_ASYNC_SEND(ipc_ap_to_lpm_mbx, &tx_buffer, 1);
	if (rc) {
		pr_err("RPROC reload iom3 failed %d, nest_count %d\n", rc, rec_nest_count);
		goto recovery_iom3;
	}

	iomcu_reinit();

	/* startup iom3 system */
	reinit_completion(&iom3_rec_done);
	tx_buffer = STARTUP_IOM3_CMD;
	rc = RPROC_SYNC_SEND(ipc_ap_to_iom_mbx, &tx_buffer, 1, &ack_buffer, 1);
	if (rc) {
		pr_err("RPROC start iom3 failed %d, nest_count %d\n", rc, rec_nest_count);
		goto recovery_iom3;
	}

	pr_err("RPROC restart iom3 success\n");
	show_iom3_stat(); /* only for IOM3 debug */

	/* dynamic loading */
	if (!wait_for_completion_timeout(&iom3_rec_done, 5 * HZ)) {
		pr_err("wait for iom3 system ready timeout\n");
		msleep(1000);
		goto recovery_iom3;
	}

	iomcu_rdr_notifier();
}

static int __iom3_need_recovery(int modid, exp_source_t f, bool rdr_reset)
{
	int old_state;

	if (!g_sensorhub_dump_reset) {
		pr_info("%s: skip dump reset.\n", __func__);
		return 0;
	}

	old_state = atomic_cmpxchg(&iom3_rec_state, IOM3_RECOVERY_IDLE, IOM3_RECOVERY_START);
	pr_err("%s: recovery prev state %d, modid 0x%x, f %u, rdr_reset %u\n",
		__func__, old_state, modid, (uint8_t)f, (uint8_t)rdr_reset);

	/* prev state is IDLE start recovery progress */
	if (old_state == IOM3_RECOVERY_IDLE) {
		__pm_wakeup_event(&g_iom3_rec_wl, jiffies_to_msecs(10 * HZ));
		blocking_notifier_call_chain(&iom3_recovery_notifier_list,
		IOM3_RECOVERY_START, NULL);
		if (f > SH_FAULT_INTERNELFAULT)
			g_smplat_scfg->dump_config.reason = (uint8_t)f;

		/* flush old logs */
		emg_flush_logbuff();

		/* write extend dump config */
		if (g_enable_dump && g_dump_extend_size && !g_sensorhub_extend_dump_buff) {
			g_sensorhub_extend_dump_buff = kmalloc(g_dump_extend_size, GFP_KERNEL);
			pr_warning("%s alloc pages logic %pK phy addr 0x%lx\n", __func__,
					   g_sensorhub_extend_dump_buff,
					   virt_to_phys(g_sensorhub_extend_dump_buff));

			if (g_sensorhub_extend_dump_buff) {
				g_smplat_scfg->dump_config.ext_dump_addr =
					virt_to_phys(g_sensorhub_extend_dump_buff);
				g_smplat_scfg->dump_config.ext_dump_size = g_dump_extend_size;
				barrier();
			}
		}
#ifndef CONFIG_CONTEXTHUB_BOOT_STAT
#ifdef CONFIG_HISI_BB
		if (rdr_reset)
			rdr_system_error(modid, 0, 0);
#endif
#endif
		reinit_completion(&sensorhub_rdr_completion);
		__send_nmi();
		atomic_set(&g_dump_minready_done, 0);
		notify_rdr_thread();

#ifdef CONFIG_CONTEXTHUB_BOOT_STAT
		if (!wait_for_completion_timeout(&sensorhub_rdr_completion, 3 * HZ))
			pr_err("%s:wait_for_completion_timeout err\n", __func__);
#ifdef CONFIG_HISI_BB
		if (rdr_reset)
			rdr_system_error(modid, 0, 0);
#endif
#endif
		queue_delayed_work(g_iom3_rec_wq, &g_iom3_rec_work, 0);
	} else if ((f == SH_FAULT_INTERNELFAULT) &&
			   completion_done(&sensorhub_rdr_completion)) {
		__pm_relax(&g_rdr_wl);
	}
	return 0;
}

int iom3_need_recovery(int modid, exp_source_t f)
{
	return __iom3_need_recovery(modid, f, true);
}

static int iom3_need_recovery_for_dump(int modid, exp_source_t f)
{
	return __iom3_need_recovery(modid, f, false);
}

static int shb_recovery_notifier(struct notifier_block *nb, unsigned long foo,
								 void *bar) {
	/* prevent access the emmc now: */
	pr_info("%s (%lu) +\n", __func__, foo);
	switch (foo) {
	case IOM3_RECOVERY_START:
		/* fall-through */
	case IOM3_RECOVERY_MINISYS:
		g_iom3_state = IOM3_ST_RECOVERY;
		break;
	case IOM3_RECOVERY_DOING:
		/* fall-through */
	case IOM3_RECOVERY_3RD_DOING:
		g_iom3_state = IOM3_ST_REPEAT;
		break;
	case IOM3_RECOVERY_FAILED:
		pr_err("%s -recovery failed\n", __func__);
		/* fall-through */
	case IOM3_RECOVERY_IDLE:
		g_iom3_state = IOM3_ST_NORMAL;
		wake_up_all(&iom3_rec_waitq);
		break;
	default:
		pr_err("%s -unknow state %ld\n", __func__, foo);
		break;
	}
	pr_info("%s -\n", __func__);
	return 0;
}

static struct notifier_block recovery_notify = {
	.notifier_call = shb_recovery_notifier,
	.priority = -1,
};

static int rdr_sensorhub_init(void)
{
	int ret = 0;

#ifdef CONFIG_HISI_BB
	if (rdr_sensorhub_init_early() != 0) {
		pr_err("rdr_sensorhub_init_early faild.\n");
		ret = -EINVAL;
	}
#endif
	sema_init(&g_rdr_sh_sem, 0);
	if (!kthread_run(rdr_sh_thread, NULL, "rdr_sh_thread")) {
		pr_err("create thread rdr_sh_main_thread faild.\n");
		ret = -EINVAL;
		return ret;
	}
	sema_init(&g_rdr_exce_sem, 0);
	if (!kthread_run(rdr_exce_thread, NULL, "rdr_exce_thread")) {
		pr_err("create thread rdr_sh_exce_thread faild.\n");
		ret = -EINVAL;
		return ret;
	}
	if (get_sysctrl_base() != 0) {
		pr_err("get sysctrl addr faild.\n");
		ret = -EINVAL;
		return ret;
	}
	if (get_watchdog_base() != 0) {
		pr_err("get watchdog addr faild.\n");
		ret = -EINVAL;
		return ret;
	}

	if (get_iomcu_wdt_irq_num() < 0) {
		pr_err("%s g_sensorhub_wdt_irq get error!\n", __func__);
		return -EINVAL;
	}
	if (request_irq(g_sensorhub_wdt_irq, watchdog_handler, 0, "watchdog",
					NULL)) {
		pr_err("%s failure requesting watchdog irq!\n", __func__);
		return -EINVAL;
	}
	if (get_nmi_offset() != 0)
		pr_err("get_nmi_offset faild.\n");
	if (atomic_notifier_chain_register(&panic_notifier_list,
									   &sensorhub_panic_block))
		pr_err("%s sensorhub panic register failed !\n", __func__);
	wakeup_source_init(&g_rdr_wl, "rdr_sensorhub");
	init_completion(&sensorhub_rdr_completion);
	return ret;
}

#ifdef CONFIG_HISI_DEBUG_FS
#ifdef CONFIG_HISI_BB
static int sh_debugfs_u64_set(void *data, u64 val)
{
	int ret = 0;
	u64 mask_val;

	if (data == NULL || val == 0) {
		pr_warn("%s invalid para.\n", __func__);
		return -1;
	}

	mask_val = val & (RDR_AP | RDR_LPM3 | RDR_IOM3);

	pr_info("%s data=0x%llx, rdr mask=0x%llx, val=0x%llx, mask_val=0x%llx\n",
		__func__, *(u64 *)data, g_rdr_core_mask, val, mask_val);

	if (mask_val != g_rdr_core_mask) {
		*(u64 *)data = mask_val;
		sh_rdr_unregister_exception();
		ret = sh_rdr_register_exception();
		if (ret != 0) {
			pr_err("%s sh_register_exception failed[%d].\n", __func__, ret);
			return ret;
		}
	}

	return ret;
}

static int sh_debugfs_u64_get(void *data, u64 *val)
{
	if (data == NULL || val == NULL) {
		pr_warn("%s invalid para.\n", __func__);
		return -1;
	}

	pr_info("%s data=0x%llx, rdr mask=0x%llx.\n", __func__, *(u64 *)data, g_rdr_core_mask);
	*val = *(u64 *)data;
	return 0;
}

DEFINE_DEBUGFS_ATTRIBUTE(sh_fops_x64, sh_debugfs_u64_get, sh_debugfs_u64_set, "0x%016llx\n");

static struct dentry *sh_debugfs_create_x64(const char *name, umode_t mode,
				 struct dentry *parent, u64 *value)
{
	pr_info("%s enter.\n", __func__);
	return debugfs_create_file_unsafe(name, mode, parent, value, &sh_fops_x64);
}
#endif

static int rdr_create_file()
{
	struct dentry *ch_root = NULL;
	struct dentry *file_dentry = NULL;

	ch_root = debugfs_lookup("contexthub", NULL);
	if (IS_ERR_OR_NULL(ch_root)) {
		ch_root = debugfs_create_dir("contexthub", NULL);
		if (IS_ERR_OR_NULL(ch_root)) {
			pr_err("%s debugfs_create_dir contexthub failed, err %ld\n", __func__, PTR_ERR(ch_root));
			ch_root = NULL;
			goto ERR_CREATE_DBGDIR;
		}
	} else {
		pr_info("%s dir contexthub contexthub is already exist\n", __func__);
	}

	/* 0660 : mode = S_IRUSR | S_IWUSR | S_IWGRP | S_IRGRP */
	file_dentry = debugfs_create_bool("dump_reset", 0660, ch_root, &g_sensorhub_dump_reset);
	if (IS_ERR_OR_NULL(file_dentry)) {
		pr_err("%s debugfs_create_file dump_reset failed\n", __func__);
		goto ERR_CREATE_DBGFS;
	}

#ifdef CONFIG_HISI_BB
	/* 0660 : mode = S_IRUSR | S_IWUSR | S_IWGRP | S_IRGRP */
	file_dentry = sh_debugfs_create_x64("rdr_core_mask", 0660, ch_root, &g_rdr_core_mask);
	if (IS_ERR_OR_NULL(file_dentry)) {
		pr_err("%s debugfs_create_file rdr_core_mask failed\n", __func__);
		goto ERR_CREATE_DBGFS;
	}
#endif
	return 0;

ERR_CREATE_DBGFS:
	if (ch_root != NULL)
		debugfs_remove_recursive(ch_root);

ERR_CREATE_DBGDIR:
	return -1;
}
#endif

int iomcu_check_dump_status(void)
{
	int rec_state = atomic_read(&iom3_rec_state);
	int minready = atomic_read(&g_dump_minready_done);

	if (rec_state == IOM3_RECOVERY_START || rec_state == IOM3_RECOVERY_FAILED || minready == 0) {
		pr_warn("%s iomcu is dumping [%d]minready[%d]\n", __func__, rec_state, minready);
		return -ESPIPE;
	}

	return 0;
}

void iomcu_minready_done(void)
{
	atomic_set(&g_dump_minready_done, 1);
}

/* lint -e86 */
int iomcu_dump_init(void)
{
	int ret;

	if (get_iomcu_cfg_base())
		return -1;
	ret = rdr_sensorhub_init();
	if (ret < 0) {
		pr_err("%s rdr_sensorhub_init ret=%d\n", __func__, ret);
		return -EFAULT;
	}
	atomic_set(&iom3_rec_state, IOM3_RECOVERY_IDLE);
	g_iom3_rec_wq = create_singlethread_workqueue("g_iom3_rec_wq");
	if (!g_iom3_rec_wq) {
		pr_err("faild create iom3 recovery workqueue in %s!\n", __func__);
		return -1;
	}

	INIT_DELAYED_WORK((struct delayed_work *)(&g_iom3_rec_work), iom3_recovery_work);
	init_completion(&iom3_rec_done);
	wakeup_source_init(&g_iom3_rec_wl, "g_iom3_rec_wl");

	init_waitqueue_head(&iom3_rec_waitq);
	register_iom3_recovery_notifier(&recovery_notify);
#ifdef CONFIG_HISI_DEBUG_FS
	rdr_create_file();
#endif
	return 0;
}
/* lint +e86 */

void notify_recovery_done(void)
{
	complete(&iom3_rec_done);
}
