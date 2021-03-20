/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description:  this file implements api to cfg ivp core hardware to power up,
 * run, run stall, load firmware, etc
 * Author: chenweiyu
 * Create: 2019-07-30
 */
#include "ivp_manager.h"
#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/platform_device.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/dma-buf.h>
#include <linux/interrupt.h>
#include <linux/dma-mapping.h>
#include <linux/delay.h>
#include <linux/version.h>
#include <linux/pm_wakeup.h>
#include <linux/ion.h>
#include <linux/hisi/hisi_ion.h>
#include <linux/hisi-iommu.h>
#include <linux/syscalls.h>
#include <linux/clk-provider.h>
#include <linux/bitops.h>
#include "securec.h"
#include "ivp.h"
#include "ivp_log.h"
#include "ivp_ioctl.h"
#ifdef CONFIG_IVP_SMMU_V3
#include "ivp_smmuv3.h"
#include "ivp_atf_subsys.h"
#else
#include "ivp_smmu.h"
#endif
#include "ivp_reg.h"

#ifdef SEC_IVP_ENABLE
#include "ivp_sec.h"
#endif
#ifdef IVP_RDR_SUPPORT
#include "ivp_rdr.h"
#endif

#define DEAD_FLAG 0xDEADBEEF
#define SIZE_16K  (16 * 1024)

void ivp_reg_write(struct ivp_device *pdev, unsigned int off, u32 val)
{
	char __iomem *reg = NULL;

	if (!pdev) {
		ivp_err("invalid param pdev");
		return;
	}

	reg = pdev->io_res.cfg_base_addr + off;

	writel(val, reg);
}

/* read ivp cfg reg */
u32 ivp_reg_read(struct ivp_device *pdev, unsigned int off)
{
	char __iomem *reg = NULL;
	u32 val;

	if (!pdev) {
		ivp_err("invalid param pdev");
		return 0;
	}

	reg = pdev->io_res.cfg_base_addr + off;
	val = readl(reg);
	return val;
}

static void ivp_pericrg_reg_write(struct ivp_device *pdev,
		unsigned int off, u32 val)
{
	char __iomem *reg = pdev->io_res.pericrg_base_addr + off;
	writel(val, reg);
}

/* read peri cfg reg for ivp */
static u32 ivp_pericrg_reg_read(struct ivp_device *pdev,
		unsigned int off)
{
	char __iomem *reg = pdev->io_res.pericrg_base_addr + off;
	u32 val = readl(reg);
	return val;
}

/******************************************************
 *     read/write watch dog reg need unlock first     *
 *****************************************************/
static void ivp_wdg_reg_write(struct ivp_device *pdev,
		unsigned int off, u32 val)
{
	char __iomem *reg = pdev->io_res.cfg_base_addr
		+ IVP_WDG_REG_BASE_OFFSET + off;
	writel(val, reg);
}

static u32 ivp_wdg_reg_read(struct ivp_device *pdev, unsigned int off)
{
	char __iomem *reg = pdev->io_res.cfg_base_addr
		+ IVP_WDG_REG_BASE_OFFSET + off;
	u32 val = readl(reg);
	return val;
}

static u32 ivp_pctrl_reg_read(struct ivp_device *pdev, unsigned int off)
{
	char __iomem *reg = pdev->io_res.pctrl_base_addr + off;
	u32 val = readl(reg);
	return val;
}

static void ivp_gic_reg_write(struct ivp_device *pdev,
		unsigned int off, u32 val)
{
	char __iomem *reg = pdev->io_res.gic_base_addr;
	writel(val, reg);
}

void ivp_hw_clr_wdg_irq(struct ivp_device *pdev)
{
	if(!pdev) {
		ivp_err("invalid param pdev");
		return;
	}
	// unlock reg
	ivp_wdg_reg_write(pdev, WDG_REG_OFF_WDLOCK, WDG_REG_UNLOCK_KEY);

	// clear irq
	ivp_wdg_reg_write(pdev, WDG_REG_OFF_WDINTCLR, 1);

	// disable irq
	ivp_wdg_reg_write(pdev, WDG_REG_OFF_WDCONTROL, 0);

	// lock reg
	ivp_wdg_reg_write(pdev, WDG_REG_OFF_WDLOCK, WDG_REG_LOCK_KEY);
}

void ivp_hw_set_ocdhalt_on_reset(struct ivp_device *pdev, int mode)
{
	if(!pdev) {
		ivp_err("invalid param pdev");
		return;
	}
	ivp_reg_write(pdev, IVP_REG_OFF_OCDHALTONRESET, (unsigned int)mode);
}

void ivp_hw_set_bootmode(struct ivp_device *pdev, int mode)
{
	u32 val = (u32)mode;

	if(!pdev) {
		ivp_err("invalid param pdev");
		return;
	}
	ivp_reg_write(pdev, IVP_REG_OFF_STATVECTORSEL, val & 0x01);
}

void ivp_hw_clockgate(struct ivp_device *pdev, int state)
{
	u32 val = (u32)state;

	if(!pdev) {
		ivp_err("invalid param pdev");
		return;
	}
	ivp_reg_write(pdev, IVP_REG_OFF_DSPCORE_GATE, val & 0x01);
}

void ivp_hw_disable_reset(struct ivp_device *pdev)
{
	if(!pdev) {
		ivp_err("invalid param pdev");
		return;
	}
	ivp_reg_write(pdev, IVP_REG_OFF_DSP_CORE_RESET_DIS, 0x04);
	ivp_reg_write(pdev, IVP_REG_OFF_DSP_CORE_RESET_DIS, 0x01);
	ivp_reg_write(pdev, IVP_REG_OFF_DSP_CORE_RESET_DIS, 0x02);
}

void ivp_hw_runstall(struct ivp_device *pdev, int mode)
{
	u32 val = (u32)mode;

	if(!pdev) {
		ivp_err("invalid param pdev");
		return;
	}
	ivp_reg_write(pdev, IVP_REG_OFF_RUNSTALL, val & 0x01);
}

int ivp_hw_query_runstall(struct ivp_device *pdev)
{
	if(!pdev) {
		ivp_err("invalid param pdev");
		return -ENODEV;
	}
	return (int)ivp_reg_read(pdev, IVP_REG_OFF_RUNSTALL);
}

void ivp_hw_trigger_NMI(struct ivp_device *pdev)
{
	if(!pdev) {
		ivp_err("invalid param pdev");
		return;
	}
	/* risedge triger.triger 0->1->0 */
	ivp_reg_write(pdev, IVP_REG_OFF_NMI, 0);
	ivp_reg_write(pdev, IVP_REG_OFF_NMI, 1);
	ivp_reg_write(pdev, IVP_REG_OFF_NMI, 0);
}

int ivp_hw_query_waitmode(struct ivp_device *pdev)
{
	if(!pdev) {
		ivp_err("invalid param pdev");
		return -ENODEV;
	}
	return (int)ivp_reg_read(pdev, IVP_REG_OFF_PWAITMODE);
}

static void ivp_hw_set_all_bus_gate_clock(struct ivp_device *pdev, int mode)
{
	if (mode == IVP_ENABLE)
		ivp_reg_write(pdev, IVP_REG_OFF_BUS_GATE_CLOCK,
			IVP_BUS_GATE_CLK_ENABLE);
	else
		ivp_reg_write(pdev, IVP_REG_OFF_BUS_GATE_CLOCK,
			IVP_BUS_GATE_CLK_DISABLE);
}

static void ivp_hw_set_all_smmu_awake_bypass(struct ivp_device *pdev, int mode)
{
	if (mode == IVP_ENABLE)
		ivp_reg_write(pdev, IVP_REG_OFF_SMMU_AWAKEBYPASS,
			IVP_SMMU_AWAKEBYPASS_ENABLE);
	else
		ivp_reg_write(pdev, IVP_REG_OFF_SMMU_AWAKEBYPASS,
			IVP_SMMU_AWAKEBYPASS_DISABLE);
}

static void ivp_hw_set_peri_autodiv(struct ivp_device *pdev, int mode)
{
	unsigned int enable = 0;

	ivp_info("set mode to:%d", mode);
	enable = ivp_pericrg_reg_read(pdev, PERICRG_REG_OFF_PERI_AUTODIV0);
	if (mode == IVP_ENABLE) {
		ivp_pericrg_reg_write(pdev, PERICRG_REG_OFF_PEREN6,
				GATE_CLK_AUTODIV_IVP);
		ivp_pericrg_reg_write(pdev, PERICRG_REG_OFF_PERI_AUTODIV0,
				enable &
				~(IVP_DW_AXI_M2_ST_BYPASS |
					IVP_DW_AXI_M1_ST_BYPASS |
					IVP_PWAITMODE_BYPASS |
					IVP_DIV_AUTO_REDUCE_BYPASS));
	} else {
		ivp_pericrg_reg_write(pdev, PERICRG_REG_OFF_PERDIS6,
				GATE_CLK_AUTODIV_IVP);
	}
}


void ivp_dev_set_dynamic_clk(struct ivp_device *pdev, unsigned int mode)
{
	if (mode >= IVP_MODE_INVALID || !pdev) {
		ivp_err("Invalid input param");
		return;
	}

	if (mode) {
		/* bus gate clock enable. */
		ivp_hw_set_all_bus_gate_clock(pdev, IVP_ENABLE);
		/* pericrg. */
		ivp_hw_set_peri_autodiv(pdev, IVP_ENABLE);
		/* smmu bypass enable. */
		ivp_hw_set_all_smmu_awake_bypass(pdev, IVP_DISABLE);

	} else {
		/* smmu bypass disable. */
		ivp_hw_set_all_smmu_awake_bypass(pdev, IVP_ENABLE);
		/* pericrg. */
		ivp_hw_set_peri_autodiv(pdev, IVP_DISABLE);
		/* bus gate clock disable. */
		ivp_hw_set_all_bus_gate_clock(pdev, IVP_DISABLE);
	}
}


void ivp_dev_stop(struct ivp_device *pdev)
{
	ivp_hw_runstall(pdev, IVP_RUNSTALL_STALL);
}

long ivp_dev_suspend(struct ivp_device *pdev)
{
	unsigned long irq_status = 0;
	u32 wfi;
	u32 binterrupt;
	u32 wdg_enable;

	if (!pdev) {
		ivp_err("invalid param pdev");
		return -ENODEV;
	}

	local_irq_save(irq_status);
	wfi = ivp_reg_read(pdev, IVP_REG_OFF_PWAITMODE);
	binterrupt = ivp_reg_read(pdev, IVP_REG_OFF_BINTERRUPT);
	wdg_enable = ivp_wdg_reg_read(pdev, WDG_REG_OFF_WDCONTROL);
	if (wfi == 1 && binterrupt == 0 && wdg_enable == 0) {
		ivp_hw_runstall(pdev, IVP_RUNSTALL_STALL);
		local_irq_restore(irq_status);
		return EOK;
	}
	local_irq_restore(irq_status);

	if (wfi == 1 && (binterrupt != 0 || wdg_enable != 0))
		ivp_warn("Suspend on wrong status, binterrupt=%u wdgenable=%u",
			binterrupt, wdg_enable);
	return -EINVAL;
}

void ivp_dev_resume(struct ivp_device *pdev)
{
	ivp_dbg("enter");

	if (ivp_hw_query_runstall(pdev) == IVP_RUNSTALL_RUN)
		return;

	ivp_hw_runstall(pdev, IVP_RUNSTALL_RUN);
}

void ivp_dev_run(struct ivp_device *pdev)
{
	int i = 0;

	if (ivp_hw_query_runstall(pdev) == IVP_RUNSTALL_RUN)
		return;

	ivp_hw_runstall(pdev, IVP_RUNSTALL_RUN);

	while ((ivp_reg_read(pdev, IVP_REG_OFF_RESEVER_REG) != IVP_INIT_SUCCESS) &&
			(i < IVP_INIT_RESULT_CHECK_TIMES)) {
		udelay(IVP_STATUS_CHECK_DELAY_US);
		i++;
		if (i >= IVP_INIT_RESULT_CHECK_TIMES) {
			ivp_err("ivp init fails value:0x%x",
				ivp_reg_read(pdev, IVP_REG_OFF_RESEVER_REG));
			return;
		}
	}

	ivp_info("ivp init success:0x%x cycle:%d",
			ivp_reg_read(pdev, IVP_REG_OFF_RESEVER_REG), i);
}

int ivp_dev_keep_on_wdg(struct ivp_device *pdev)
{
	if (!pdev) {
		ivp_err("input param pdev is null");
		return -ENODEV;
	}

	if (down_interruptible(&pdev->ivp_comm.wdg_sem)) {
		ivp_info("interrupt");
		return -EINTR;
	}

	if (atomic_read(&pdev->ivp_comm.wdg_sleep)) {
		ivp_info("watchdog sleeped");
		return -EAGAIN;
	}

	return 0;
}

void ivp_dev_sleep_wdg(struct ivp_device *pdev)
{
	if (!pdev) {
		ivp_err("input param pdev is null");
		return;
	}

	atomic_set(&pdev->ivp_comm.wdg_sleep, 1);
	up(&pdev->ivp_comm.wdg_sem);
}

int ivp_dev_smmu_reset(struct ivp_device *pdev)
{
	unsigned int status = 0;
	int ret = 0;
	int count = SMMU_RESET_WAIT_TIME;

	ivp_info("enter");
	ivp_reg_write(pdev, IVP_REG_OFF_SMMU_RST_EN,
			BIT_SMMU_CRST_EN | BIT_SMMU_BRST_EN);
	udelay(10);
	ivp_reg_write(pdev, IVP_REG_OFF_SMMU_RST_DIS,
			BIT_SMMU_CRST_DIS | BIT_SMMU_BRST_DIS);

	while (count--) {
		status = ivp_reg_read(pdev, IVP_REG_OFF_SMMU_RST_ST);
		if ((status & BIT_SMMU_CRST_DIS) &&
				(status & BIT_SMMU_BRST_DIS))
			break;
		udelay(1);
	}

	if (count <= 0) {
		ret = -1;
		ivp_err("Reset smmu fail.");
	}

	return ret;
}

int ivp_dev_smmu_init(struct ivp_device *pdev)
{
	int ret = 0;
	struct ivp_smmu_dev *smmu_dev = NULL;
	if (!pdev) {
		ivp_err("input param pdev is null");
		return -ENODEV;
	}
	smmu_dev = pdev->ivp_comm.smmu_dev;

	if (!smmu_dev) {
		ivp_err("input param smmu dev is null");
		return -ENODEV;
	}

	ivp_info("enter");
	/* reset smmu */

	/* enable smmu */
#ifdef CONFIG_IVP_SMMU_V3
	ret = ivp_smmu_trans_enable(smmu_dev);
	if (pdev->ivp_comm.ivp_secmode == NOSEC_MODE) {
		ret += ivpatf_tbu_signal_cfg_secmod(pdev->core_id,
			pdev->sid, pdev->ssid, pdev->ivp_comm.ivp_secmode);
#ifdef SEC_IVP_V300
#ifdef SEC_IVP_ENABLE
	} else {
		ret += ivpatf_tbu_signal_cfg_secmod(pdev->core_id,
			SECMODE_SID, SECMODE_SSID, pdev->ivp_comm.ivp_secmode);
#endif
#endif
	}
#endif
	if (ret)
		ivp_err("smmu enable trans ret:%d", ret);
	return ret;
}

int ivp_dev_smmu_deinit(struct ivp_device *pdev)
{
	int ret = 0;
	struct ivp_smmu_dev *smmu_dev = NULL;
	if (!pdev) {
		ivp_err("input param pdev is null");
		return -ENODEV;
	}
	smmu_dev = pdev->ivp_comm.smmu_dev;

	if (!smmu_dev) {
		ivp_err("input param smmu dev is null");
		return -ENODEV;
	}

	ivp_info("enter");
	ret = ivp_smmu_trans_disable(smmu_dev);
	if (ret)
		ivp_err("Enable trans failed.[%d]", ret);

	return ret;
}


irqreturn_t ivp_wdg_irq_handler(int irq, void *dev_id)
{
	struct ivp_device *pdev = NULL;

	if (dev_id == NULL) {
		ivp_err("input param dev_id is NULL");
		return IRQ_NONE;
	}

	pdev = (struct ivp_device *)dev_id;

	ivp_warn("=========WDG IRQ Trigger=============");
	/* Clear IRQ */
	ivp_hw_clr_wdg_irq(pdev);

	up(&pdev->ivp_comm.wdg_sem);
	ivp_warn("=========WDG IRQ LEAVE===============");

	ivp_dsm_error_notify(DSM_IVP_WATCH_ERROR_NO);

	return IRQ_HANDLED;
}

static void ivp_parse_dwaxi_info(struct ivp_device *pdev)
{
	u32 val;
	u32 bits_val;
	u32 offset;

	offset = PCTRL_REG_OFF_PERI_STAT4;
	val = ivp_pctrl_reg_read(pdev, offset);

	ivp_dbg("pctrl reg:%#x = %#x", offset, val);

	bits_val = (val & BIT(18)) >> 18;
	if (bits_val == 0)
		ivp_warn("BIT[18] : %#x", bits_val);
	else
		ivp_warn("BIT[18] : %#x", bits_val);

	bits_val = (val & BIT(17)) >> 17;
	if (bits_val == 0)
		ivp_warn("BIT[17] : %#x, dlock write", bits_val);
	else
		ivp_warn("BIT[17] : %#x, dlock read", bits_val);

	bits_val = (val & 0x1e000) >> 13;
	ivp_warn("ivp32 dlock id[%#x]", bits_val);

	bits_val = (val & 0x1c00) >> 10;
	ivp_warn("ivp32 dlock slv[%#x]", bits_val);

	offset = IVP_REG_OFF_SMMU_RST_ST;
	val = ivp_reg_read(pdev, offset);
	ivp_warn("ivp reg:%#x = %#x", offset, val);

	offset = IVP_REG_OFF_TIMER_WDG_RST_STATUS;
	val = ivp_reg_read(pdev, offset);
	ivp_warn("ivp:%#x = %#x", offset, val);

	offset = IVP_REG_OFF_DSP_CORE_RESET_STATUS;
	val = ivp_reg_read(pdev, offset);
	ivp_warn("ivp:%#x = %#x", offset, val);
}

irqreturn_t ivp_dwaxi_irq_handler(int irq, void *dev_id)
{
	struct ivp_device *pdev = (struct ivp_device *)dev_id;
	if (!pdev) {
		ivp_err("invalid param pdev");
		return IRQ_NONE;
	}

	ivp_warn("==========DWAXI IRQ Trigger===============");
	ivp_warn("dwaxi triggled!SMMU maybe in reset status");
	//clear dwaxi irq
	ivp_gic_reg_write(pdev,
			GIC_REG_OFF_GICD_ICENABLERN, 0x80000);
	ivp_parse_dwaxi_info(pdev);
	ivp_warn("==========DWAXI IRQ LEAVE=================");

	ivp_dsm_error_notify(DSM_IVP_DWAXI_ERROR_NO);

	return IRQ_HANDLED;
}

#ifdef CONFIG_OF
static inline int ivp_setup_one_onchipmem_section(
	struct ivp_sect_info *sect, struct device_node *np)
{
	const char *name = of_node_full_name(np);
	unsigned int settings[3] = {0};
	unsigned int len = strlen(name);
	errno_t ret;

	if (of_property_read_u32_array(np, OF_IVP_SECTION_NAME,
				settings, ARRAY_SIZE(settings))) {
		ivp_err("read reg fail.");
		return -EINVAL;
	}

	len = (len >= sizeof(sect->name)) ? (sizeof(sect->name) - 1) : len;
	ret = strncpy_s(sect->name, (sizeof(sect->name) - 1), name, len);
	if (ret != EOK) {
		ivp_err("strncpy_s fail, ret [%d]", ret);
		return -EINVAL;
	}
	sect->name[len] = '\0';
	sect->ivp_addr  = settings[0];
	sect->acpu_addr = settings[1];
	sect->len       = settings[2];

	return 0;
}

int ivp_setup_onchipmem_sections(
	struct platform_device *plat_devp, struct ivp_device *pdev)
{
	struct device_node *parrent = NULL;
	struct device_node *child = NULL;
	size_t i = 0;
	size_t sects_size = 0;
	errno_t ret;

	if (!plat_devp || !pdev) {
		ivp_err("pointer is NULL.");
		return -EINVAL;
	}

	parrent = of_get_child_by_name(
		plat_devp->dev.of_node, OF_IVP_SECTION_NODE_NAME);

	if (!parrent) {
		ivp_err("Failed to get mem parrent node.");
		return -ENODEV;
	}

	pdev->ivp_comm.sect_count = of_get_child_count(parrent);
	sects_size = sizeof(struct ivp_sect_info) * pdev->ivp_comm.sect_count;
	pdev->ivp_comm.sects = (struct ivp_sect_info *)kzalloc(sects_size, GFP_KERNEL);
	if (!pdev->ivp_comm.sects) {
		ivp_err("kmalloc sects fail.");
		return -ENOMEM;
	}

	pdev->ivp_comm.sec_sects =
		(struct ivp_sect_info *)kzalloc(sects_size, GFP_KERNEL);
	if (!pdev->ivp_comm.sec_sects) {
		ivp_err("kmalloc sec_sects fail.");
		goto err_sects;
	}

	ivp_info("section count:%d.", pdev->ivp_comm.sect_count);

	for_each_child_of_node(parrent, child) {
		if (ivp_setup_one_onchipmem_section(&pdev->ivp_comm.sects[i], child)) {
			ivp_err("setup %lu section fail", i);
			goto err_out;
		}

		if (!strncmp(pdev->ivp_comm.sects[i].name, OF_IVP_DDR_MEM_NAME,
				sizeof(OF_IVP_DDR_MEM_NAME)) ||
		    !strncmp(pdev->ivp_comm.sects[i].name, OF_IVP_SHARE_MEM_NAME,
				sizeof(OF_IVP_SHARE_MEM_NAME)) ||
		    !strncmp(pdev->ivp_comm.sects[i].name, OF_IVP_LOG_MEM_NAME,
				sizeof(OF_IVP_LOG_MEM_NAME)))
			pdev->ivp_comm.ivp_meminddr_len += pdev->ivp_comm.sects[i].len;
		i++;
	}
	ret =
		memcpy_s(pdev->ivp_comm.sec_sects, sects_size, pdev->ivp_comm.sects, sects_size);
	if (ret != EOK) {
		ivp_err("memcpy_s fail, ret [%d]", ret);
		goto err_out;
	}
	/*lint -restore*/
	return 0;

err_out:
	kfree(pdev->ivp_comm.sec_sects);
	pdev->ivp_comm.sec_sects = NULL;
err_sects:
	kfree(pdev->ivp_comm.sects);
	pdev->ivp_comm.sects = NULL;
	pdev->ivp_comm.sect_count = 0;
	return -EFAULT;
}
#endif

int ivp_setup_irq(
	struct platform_device *plat_devp, struct ivp_device *ivp_devp)
{
	int irq;

	if(!plat_devp || !ivp_devp) {
		ivp_err("invalid param pdev");
		return -EINVAL;
	}

	irq = platform_get_irq(plat_devp, 0);

	if (irq < 0) {
		ivp_err("Get irq fail!");
		return -EINVAL;
	}

	ivp_devp->ivp_comm.wdg_irq = (unsigned int)irq;
	ivp_info("Get irq: %d.", irq);

	irq = platform_get_irq(plat_devp, 1);

	if (irq < 0) {
		ivp_err("Get irq fail!");
		return -EINVAL;
	}

	ivp_devp->ivp_comm.dwaxi_dlock_irq = (unsigned int)irq;
	ivp_info("Get irq: %d.", irq);
	return 0;
}


int ivp_setup_smmu_dev(struct ivp_device *ivp_devp)
{
	struct ivp_smmu_dev *smmu_dev = NULL;

	if(!ivp_devp) {
		ivp_err("invalid param pdev");
		return -EINVAL;
	}

#ifdef IVP_DUAL_CORE
	smmu_dev = ivp_smmu_get_device(ivp_devp->core_id);
#else
	smmu_dev = ivp_smmu_get_device(0UL);
#endif

	if (!smmu_dev) {
		ivp_err("Failed to get ivp smmu dev!");
		return -ENODEV;
	}
	ivp_devp->ivp_comm.smmu_dev = smmu_dev;
	return 0;
}

void ivp_release_iores(struct platform_device *plat_devp)
{
	struct ivp_device *pdev =
		(struct ivp_device *)platform_get_drvdata(plat_devp);

	ivp_info("enter");
	if (!pdev) {
		ivp_err("pdev is invalid");
		return;
	}


	if (pdev->io_res.gic_base_addr) {
		devm_iounmap(&plat_devp->dev, pdev->io_res.gic_base_addr);
		pdev->io_res.gic_base_addr = NULL;
	}

	if (pdev->io_res.pericrg_base_addr) {
		devm_iounmap(&plat_devp->dev, pdev->io_res.pericrg_base_addr);
		pdev->io_res.pericrg_base_addr = NULL;
	}

	if (pdev->io_res.pctrl_base_addr) {
		devm_iounmap(&plat_devp->dev, pdev->io_res.pctrl_base_addr);
		pdev->io_res.pctrl_base_addr = NULL;
	}

	if (pdev->io_res.cfg_base_addr) {
		devm_iounmap(&plat_devp->dev, pdev->io_res.cfg_base_addr);
		pdev->io_res.cfg_base_addr = NULL;
	}
}

int ivp_init_reg_res(struct platform_device *plat_devp,
		struct ivp_device *pdev)
{
	struct resource *mem_res = NULL;

	if (!plat_devp || !pdev) {
		ivp_err("input param plat_devp or pdev is null");
		return -ENODEV;
	}

	mem_res = platform_get_resource(plat_devp,
			IORESOURCE_MEM, 0); /*lint -save -e838*/
	if (!mem_res) {
		ivp_err("Get cfg res fail!");
		goto err_exit;
	}

	pdev->io_res.cfg_base_addr = devm_ioremap(
		&plat_devp->dev, mem_res->start, resource_size(mem_res));
	if (!pdev->io_res.cfg_base_addr) {
		ivp_err("Map cfg reg failed!");
		goto err_exit;
	}

	mem_res = platform_get_resource(plat_devp, IORESOURCE_MEM, 1);
	if (!mem_res) {
		ivp_err("Get pctrl res failed!");
		goto err_exit;
	}

	pdev->io_res.pctrl_base_addr = devm_ioremap(
		&plat_devp->dev, mem_res->start, resource_size(mem_res));
	if (!pdev->io_res.pctrl_base_addr) {
		ivp_err("Map pctrl reg failed!");
		goto err_exit;
	}

	mem_res = platform_get_resource(plat_devp, IORESOURCE_MEM, 2);
	if (!mem_res) {
		ivp_err("Get preicrg res failed!");
		goto err_exit;
	}
	pdev->io_res.pericrg_base_addr = devm_ioremap(
		&plat_devp->dev, mem_res->start, resource_size(mem_res));
	if (!pdev->io_res.pericrg_base_addr) {
		ivp_err("Map pericrg res failed!");
		goto err_exit;
	}

	mem_res = platform_get_resource(plat_devp, IORESOURCE_MEM, 3);
	if (!mem_res) {
		ivp_err("Get gic res failed");
		goto err_exit;
	}

	pdev->io_res.gic_base_addr = devm_ioremap(&plat_devp->dev,
			GIC_IRQ_CLEAR_REG, 4); /*lint -save -e747*/
	if (!pdev->io_res.gic_base_addr) {
		ivp_err("Map gic res failed!");
		goto err_exit;
	}
	return 0;
err_exit:
	ivp_release_iores(plat_devp);
	return -EFAULT;
}
void ivp_dump_status(struct ivp_device *pdev)
{
	unsigned int cur_pc;
	unsigned int waiti;
	unsigned int runstall;
	unsigned int binterrupt;

	if (!pdev) {
		ivp_err("input param pdev is invalid");
		return;
	}

	if (atomic_read(&pdev->ivp_comm.poweron_success) != 0) {
		ivp_err("ivp has powered off!");
		return;
	}

	waiti = (u32)ivp_hw_query_waitmode(pdev);
	runstall = (u32)ivp_hw_query_runstall(pdev);
	binterrupt = ivp_reg_read(pdev, IVP_REG_OFF_BINTERRUPT);
	/* get pc, wfi, runstall. interrupt, or others status */
	ivp_reg_write(pdev, IVP_REG_OFF_OCDHALTONRESET, IVP_DEBUG_ENABLE);
	cur_pc = ivp_reg_read(pdev, IVP_REG_OFF_IVP_DEBUG_SIG_PC);

	ivp_err("Dsp curr pc = 0x%x, waiti = 0x%x, runstall = 0x%x, bint = 0x%x",
		cur_pc, waiti, runstall, binterrupt);
}

void *ivp_vmap(phys_addr_t paddr, size_t size, unsigned int *offset)
{
	unsigned int i;
	void *vaddr = NULL;
	pgprot_t pgprot;
	unsigned int pages_count;
	struct page **pages = NULL;

	*offset = paddr & ~PAGE_MASK;
	paddr &= PAGE_MASK;
	pages_count = PAGE_ALIGN(size + *offset) / PAGE_SIZE;

	pages = kzalloc(sizeof(struct page *) * pages_count, GFP_KERNEL);
	if (!pages)
		return NULL;

	pgprot = pgprot_writecombine(PAGE_KERNEL);

	for (i = 0; i < pages_count; i++) {
		*(pages + i) = phys_to_page(
			(phys_addr_t)(paddr + PAGE_SIZE * i));
	}

	vaddr = vmap(pages, pages_count, VM_MAP, pgprot);
	kfree(pages);
	if (!vaddr)
		return NULL;

	return (char *)vaddr + *offset;
}

bool is_ivp_in_secmode(struct ivp_device *pdev)
{
	return ((pdev->ivp_comm.ivp_sec_support == 1) &&
		(pdev->ivp_comm.ivp_secmode == SECURE_MODE));
}

int ivp_init_pri(struct platform_device *pdev, struct ivp_device *ivp_devp)
{
	int ret;

	if (!pdev || !ivp_devp) {
		ivp_err("invalid input param pdev or ivp_devp");
		return -EINVAL;
	}
	ret = ivp_setup_regulator(pdev, ivp_devp);
	if (ret) {
		ivp_err("setup regulator failed, ret:%d", ret);
		return ret;
	}

	ret = ivp_setup_clk(pdev, ivp_devp);
	if (ret) {
		ivp_err("setup clk failed, ret:%d", ret);
		return ret;
	}

	ret = ivp_get_memory_section(pdev, ivp_devp);
	if (ret) {
		ivp_err("get memory section failed, ret:%d", ret);
		return ret;
	}

#ifdef SEC_IVP_ENABLE
	ret = ivp_get_secure_attribute(pdev, ivp_devp);
	if (ret) {
		ivp_err("get_secure_attribute failed, ret:%d", ret);
		ivp_free_memory_section(ivp_devp);
	}
#endif

#ifdef IVP_RDR_SUPPORT
	ret = ivp_rdr_init(ivp_devp);
	if (ret) {
		ivp_err("rdr init  failed, ret:%d", ret);
		ivp_free_memory_section(ivp_devp);
		return ret;
	}
#endif

	return ret;
}

void ivp_deinit_pri(struct ivp_device *ivp_devp)
{
	if (!ivp_devp) {
		ivp_err("invalid input param ivp_devp");
		return;
	}
	ivp_free_memory_section(ivp_devp);
#ifdef IVP_RDR_SUPPORT
	ivp_rdr_deinit();
#endif
}

int ivp_init_resethandler(struct ivp_device *ivp_devp)
{
	unsigned int ivp_addr;

	if (!ivp_devp) {
		ivp_err("invalid input param ivp_devp");
		return -EINVAL;
	}
#ifdef SEC_IVP_ENABLE
	if (ivp_devp->ivp_comm.ivp_secmode == SECURE_MODE) {
		atomic_set(&ivp_devp->sec_dev->ivp_image_success, 0);
	} else {
#endif
		/* init code to remap address */
#ifdef IVP_DUAL_CORE
		ivp_info("reset iram for ivp %u", ivp_devp->core_id);
		ivp_addr = ivp_devp->ivp_comm.sects[IRAM_SECT_INDEX].ivp_addr;
#else
		ivp_addr = ivp_devp->ivp_comm.sects[IRAM_SECTION_INDEX].ivp_addr;
#endif
		ivp_devp->ivp_comm.iram = ioremap(ivp_addr, SIZE_16K); /*lint !e446*/
		if (!ivp_devp->ivp_comm.iram) {
			ivp_err("Can't map ivp base address");
			return -ENODEV;
		}
		iowrite32(DEAD_FLAG, ivp_devp->ivp_comm.iram);
#ifdef SEC_IVP_ENABLE
	}
#endif
	return 0;
}

int ivp_check_resethandler(IS_CONST struct ivp_device *ivp_devp)
{
	int inited = 0;
	u32 flag;

	if (!ivp_devp) {
		ivp_err("invalid input param ivp_devp");
		return -EINVAL;
	}

	/* check init code in remap address */
#ifdef SEC_IVP_ENABLE
	if (ivp_devp->ivp_comm.ivp_secmode == SECURE_MODE) {
		if (atomic_read(&ivp_devp->sec_dev->ivp_image_success) == 1)
			inited = 1;
	} else {
#endif
		flag = 0;
		if (ivp_devp->ivp_comm.iram)
			flag = ioread32(ivp_devp->ivp_comm.iram);
		if (flag != DEAD_FLAG)
			inited = 1;
#ifdef SEC_IVP_ENABLE
	}
#endif

	return inited;
}

void ivp_deinit_resethandler(struct ivp_device *ivp_devp)
{
	if (!ivp_devp) {
		ivp_err("invalid input param ivp_devp");
		return;
	}

#ifdef SEC_IVP_ENABLE
	if (ivp_devp->ivp_comm.ivp_secmode == SECURE_MODE) {
		atomic_set(&ivp_devp->sec_dev->ivp_image_success, 0);
	} else {
#endif
		/* deinit remap address */
		if (ivp_devp->ivp_comm.iram) {
			iounmap(ivp_devp->ivp_comm.iram);
			ivp_devp->ivp_comm.iram = NULL;
		}
#ifdef SEC_IVP_ENABLE
	}
#endif
}

int ivp_sec_loadimage(IS_CONST struct ivp_device *ivp_devp)
{
#ifdef SEC_IVP_ENABLE
	if (!ivp_devp) {
		ivp_err("invalid input param ivp_devp");
		return -EINVAL;
	}
	return ivp_sec_load(ivp_devp);
#else
	ivp_err("not support sec ivp");
	return 0;
#endif
}

void ivp_dev_hwa_enable(struct ivp_device *ivp_devp)
{
	if (!ivp_devp) {
		ivp_err("invalid input param ivp_devp");
		return;
	}
	/* enable apb gate clock, watdog, timer */
	ivp_info("ivp will enable hwa");
	ivp_reg_write(ivp_devp,
		IVP_REG_OFF_APB_GATE_CLOCK, IVP_APB_GATE_CLOCK_VAL);
	ivp_reg_write(ivp_devp,
		IVP_REG_OFF_TIMER_WDG_RST_DIS, IVP_TIMER_WDG_RST_DIS_VAL);
}

void ivp_hw_enable_reset(struct ivp_device *ivp_devp)
{
	if (!ivp_devp) {
		ivp_err("invalid input param ivp_devp");
		return;
	}
	ivp_reg_write(ivp_devp,
		IVP_REG_OFF_DSP_CORE_RESET_EN, RST_IVP32_PROCESSOR_EN);
	ivp_reg_write(ivp_devp,
		IVP_REG_OFF_DSP_CORE_RESET_EN, RST_IVP32_DEBUG_EN);
	ivp_reg_write(ivp_devp,
		IVP_REG_OFF_DSP_CORE_RESET_EN, RST_IVP32_JTAG_EN);
}
