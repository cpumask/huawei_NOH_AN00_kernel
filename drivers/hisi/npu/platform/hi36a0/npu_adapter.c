/*
 * npu_adapter.c
 *
 * about npu adapter
 *
 * Copyright (c) 2012-2019 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */
#include "npu_adapter.h"

#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/io.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <asm/cacheflush.h>
#include <linux/dma-direction.h>
#include <linux/dma-buf.h>
#include <linux/kthread.h>
#include <linux/version.h>
#include <securec.h>
#include <linux/hisi/hisi_svm.h>

#include "hisi_lb.h"
#include "npu_platform_resource.h"
#include "npu_platform_register.h"
#include "npu_atf_subsys.h"
#include "npu_pm_framework.h"
#include "npu_hwts_plat.h"
#include "npu_hw_exp_irq.h"
#include "npu_shm.h"
#include "npu_svm.h"
#include "dts/npu_reg.h"
#include "npu_dpm.h"

tmp_log_buf_header_t *g_ts_buf_header_addr = NULL;
tmp_log_buf_header_t *g_aicpu_buf_header_addr = NULL;
struct task_struct *g_log_fetch_thread = NULL;

u32 g_tmp_log_switch = 0;

void npu_plat_enable_sdma_channel(u64 sdma_ch_base_addr)
{
	U_SQ_SID sdma_sq_sid = {{0}};
	U_CQ_SID sdma_cq_sid = {{0}};
	U_CH_CTRL sdma_ch_ctrl = {{0}};

	/* config sq/cq sid to bypass smmu */
	sdma_sq_sid.bits.sq_sid = SMMU_BYPASS_SID;
	sdma_cq_sid.bits.cq_sid = SMMU_BYPASS_SID;
	sdma_sq_sid.bits.sq_sub_sid = 0;
	sdma_cq_sid.bits.cq_sub_sid = 0;
	write_reg32_readback(sdma_ch_base_addr + AIC_SYSDMA_REG_SQ_SID_0_REG,
		sdma_sq_sid.u32);
	write_reg32_readback(sdma_ch_base_addr + AIC_SYSDMA_REG_CQ_SID_0_REG,
		sdma_cq_sid.u32);

	/* enable sdma channel */
	sdma_ch_ctrl.bits.cqe_fmt_sel = 0; /* hwts just adapt cqe 16B mode */
	sdma_ch_ctrl.bits.cq_size = SDMA_CQ_SIZE;
	sdma_ch_ctrl.bits.sq_size = SDMA_SQ_SIZE;
	sdma_ch_ctrl.bits.qos = SDMA_CHN_QOS;
	sdma_ch_ctrl.bits.ch_en = 1; /* enable sdma channel */
	write_reg32_readback(sdma_ch_base_addr + AIC_SYSDMA_REG_CH_CTRL_0_REG,
		sdma_ch_ctrl.u32);
}

int npu_plat_init_hwts_sdma_channel()
{
	struct devdrv_mem_desc *sdma_sq_desc = NULL;
	struct devdrv_platform_info *plat_info = NULL;
	u64 hwts_cfg_base = 0;
	u64 sdma_cfg_base = 0;
	u64 sdma_ch_base_addr = 0;
	u32 sq_base_addr_low = 0;
	u32 cq_base_addr_low = 0;
	u8 channel_id = 0;
	SOC_NPU_HWTS_HWTS_SDMA_NS_SQ_BASE_ADDR_CFG_UNION sdma_ns_sq_base_addr_cfg = {0};

	NPU_DRV_WARN("enter npu_plat_init_hwts_sdma_channel");

	plat_info = devdrv_plat_get_info();
	COND_RETURN_ERROR(plat_info == NULL, -1, "devdrv_plat_get_info failed");

	/* config hwts/sdma cfg base address */
	hwts_cfg_base = (u64) (uintptr_t) DEVDRV_PLAT_GET_REG_VADDR(plat_info, DEVDRV_REG_HWTS_BASE);
	sdma_cfg_base = (u64) (uintptr_t) DEVDRV_PLAT_GET_REG_VADDR(plat_info, DEVDRV_REG_SDMA_BASE);
	sdma_sq_desc = DEVDRV_PLAT_GET_SDMA_SQ_BUF(plat_info);/* reserved sdma sq memory */
	COND_RETURN_ERROR(sdma_sq_desc == NULL, -1, "ptr sdma_sq_desc is null");

	NPU_DRV_DEBUG("hwts_cfg_base 0x%llx, sdma_cfg_base 0x%llx, sdma_sq_base 0x%x",
		hwts_cfg_base, sdma_cfg_base, sdma_sq_desc->base);

	/* config hwts for sdma channel base address */
	sdma_ns_sq_base_addr_cfg.reg.sdma_ns_sq_base_addr = sdma_sq_desc->base; //sdma channel0 base addr
	sdma_ns_sq_base_addr_cfg.reg.sdma_ns_sq_shift = 0xF; /* 1<<sdma_ns_sq_shift: SDMA_SQ_ENTRIES * SDMA_SQE_SIZE */
	sdma_ns_sq_base_addr_cfg.reg.sdma_ns_sq_base_is_virtual = 0; /* 0: physic  1:virtual */
	write_reg64_readback(SOC_NPU_HWTS_HWTS_SDMA_NS_SQ_BASE_ADDR_CFG_ADDR(hwts_cfg_base),
		sdma_ns_sq_base_addr_cfg.value);

	for (channel_id = 0; channel_id < NPU_HWTS_SDMA_CHANNEL_NUM; channel_id++) {
		sdma_ch_base_addr = sdma_cfg_base +
			(channel_id * (0x1 << NPU_SDMA_CHANNEL_SHIFT));

		/* configure sdma sq base addr_reg */
		sq_base_addr_low = sdma_sq_desc->base +
			(channel_id * SDMA_SQ_ENTRIES * SDMA_SQE_SIZE);
		write_reg32_readback(sdma_ch_base_addr + AIC_SYSDMA_REG_SQ_BASE_L_0_REG,
			sq_base_addr_low);
		write_reg32_readback(sdma_ch_base_addr + AIC_SYSDMA_REG_SQ_BASE_H_0_REG,
			(u32)(1 << 31)); /* using 32 bit physic addr,p = 1 ssidv = 0 */

		/* configure sdma cq base addr_reg */
		cq_base_addr_low = NPU_HWTS_CFG_BASE + NPU_HWTS_SDMA_CQ_CFG_OFFSET +
			(channel_id * NPU_HWTS_SDMA_CQ_OFFSET);
		write_reg32_readback(sdma_ch_base_addr + AIC_SYSDMA_REG_CQ_BASE_L_0_REG,
			cq_base_addr_low);
		write_reg32_readback(sdma_ch_base_addr + AIC_SYSDMA_REG_CQ_BASE_H_0_REG,
			(u32)(1 << 31)); /* using 32 bit physic addr, p = 1 ssidv = 0 */

		npu_plat_enable_sdma_channel(sdma_ch_base_addr);
	}

	asm volatile("dsb st" : : : "memory");
	return 0;
}

int npu_plat_init_sdma(u64 is_secure)
{
	int ret;

	ret = npu_plat_init_hwts_sdma_channel();
	COND_RETURN_ERROR(ret != 0, ret, "init sdma channel failed, ret 0x%x\n", ret);

	ret = devdrv_init_sdma_tbu(is_secure, NPU_FLAGS_INIT_SYSDMA);
	COND_RETURN_ERROR(ret != 0, ret, "init sdma failed, ret 0x%x\n", ret);

	return 0;
}

int devdrv_plat_res_fw_proc(const struct file *fp, loff_t fsize, loff_t pos, u64 fw_load_addr)
{
	mm_segment_t old_fs;
	ssize_t ret;
	old_fs = get_fs();

	NPU_DRV_INFO("firmware is loading, addr 0x%llx, pos %d\n", fw_load_addr, (u32)pos);
	set_fs((mm_segment_t)KERNEL_DS);
	ret = vfs_read((struct file *)fp, (char*)(uintptr_t)fw_load_addr, fsize, &pos);
	set_fs(old_fs);
	if (ret < 0) {
		NPU_DRV_ERR("vfs_read ret %d\n", (u32)ret);
		return false;
	}

	return true;
}

int npu_plat_powerup_smmu(struct device *dev)
{
	int ret;
	ret = hisi_smmu_poweron(dev);
	if (ret != 0) {
		NPU_DRV_ERR("hisi_smmu_poweron failed\n");
		return ret;
	}

	ret = devdrv_plat_powerup_tbu();
	if (ret != 0) {
		NPU_DRV_WARN("devdrv_plat_powerup_tbu failed ret=%d\n", ret);
		return ret;
	}
	return 0;
}
int devdrv_plat_svm_bind(struct devdrv_dev_ctx *dev_ctx, struct task_struct *task, void **svm_dev)
{
	struct devdrv_platform_info *plat_info = NULL;
	plat_info = devdrv_plat_get_info();
	COND_RETURN_ERROR(plat_info == NULL, -1, "devdrv_plat_get_info failed\n");

	*svm_dev = (void*)hisi_svm_bind_task(DEVDRV_PLAT_GET_PDEV(plat_info), task);
	if (*svm_dev == NULL) {
		NPU_DRV_ERR("hisi_svm_bind_task failed \n");
		return -EBUSY; // likely bound by other process
	}
	return 0;
}

int npu_plat_poweroff_smmu(uint32_t devid)
{
	struct devdrv_platform_info *plat_info = NULL;
	plat_info = devdrv_plat_get_info();
	if (plat_info == NULL) {
		NPU_DRV_ERR("get plat_ops failed.\n");
		return -1;
	}
	devdrv_clear_pid_ssid_table(devid, 0);

	(void)hisi_smmu_poweroff(DEVDRV_PLAT_GET_PDEV(plat_info)); /* 0: npu */
	return 0;
}

int devdrv_plat_pm_powerup(struct devdrv_dev_ctx *dev_ctx, u32 work_mode)
{
	UNUSED(dev_ctx);
	UNUSED(work_mode);
	NPU_DRV_WARN("stub fuction\n");
	return 0;
}

int devdrv_plat_pm_powerdown(uint32_t devid, u32 is_secure, u32 *stage)
{
	UNUSED(devid);
	UNUSED(is_secure);
	UNUSED(stage);
	NPU_DRV_WARN("stub fuction\n");

	return 0;
}

int devdrv_plat_pm_open(uint32_t devid)
{
	if (devdrv_plat_ioremap(DEVDRV_REG_POWER_STATUS) != 0) {
		NPU_DRV_ERR("devdrv_plat_ioremap failed\n");
		return -1;
	}
	return 0;
}

int devdrv_plat_pm_release(uint32_t devid)
{
	int ret;

	devdrv_plat_iounmap(DEVDRV_REG_POWER_STATUS);
	ret = devdrv_clear_pid_ssid_table(devid, 1);
	if (ret != 0) {
		NPU_DRV_ERR("devdrv_clear_pid_ssid_table failed\n");
		return ret;
	}
	return 0;
}

int devdrv_plat_res_mailbox_send(void *mailbox, int mailbox_len, const void *message, int message_len)
{
	int ret;

	if (message_len > mailbox_len) {
		NPU_DRV_ERR("message len =%d, too long", message_len);
		return -1;
	}

	ret = memcpy_s(mailbox, mailbox_len, message, message_len);
	if (ret != 0)
		NPU_DRV_ERR("memcpy_s failed. ret=%d\n", ret);
	mb();
	return ret;
}

void __iomem* devdrv_plat_sram_remap(struct platform_device *pdev, resource_size_t sram_addr,
                                     resource_size_t sram_size)
{
	if (pdev == NULL) {
		NPU_DRV_ERR("pdev is NULL\n");
		return NULL;
	}
	return devm_ioremap_nocache(&pdev->dev, sram_addr, sram_size);
}

void devdrv_plat_sram_unmap(struct platform_device *pdev, void* sram_addr)
{
	if (pdev == NULL || sram_addr == NULL) {
		NPU_DRV_ERR("pdev or sram_addr is NULL \n");
		return;
	}
	devm_iounmap(&pdev->dev, (void __iomem*)sram_addr);
	return;
}

int npu_log_stop(void)
{
	int ret;

	iounmap(g_ts_buf_header_addr);
	ret = kthread_stop(g_log_fetch_thread);
	if (ret != 0)
		NPU_DRV_ERR("log thread stop fail, ret = %d", ret);

	return ret;
}

int devdrv_plat_res_ctrl_core(struct devdrv_dev_ctx *dev_ctx, u32 core_num)
{
	// do not support ctrl core
	UNUSED(dev_ctx);
	UNUSED(core_num);
	NPU_DRV_WARN("Temperature protection feature of this platform don't support control core num\n");
	return 0;
}

#define DEVDRV_NPU_IRQ_GIC_MAP_COLUMN       2
u32 g_devdrv_npu_irq_gic_map[][DEVDRV_NPU_IRQ_GIC_MAP_COLUMN] = {
	// irq_type,                 gic_num
	{DEVDRV_IRQ_CALC_CQ_UPDATE0, DEVDRV_NPU2ACPU_HW_EXP_IRQ_NS_2},
	{DEVDRV_IRQ_DFX_CQ_UPDATE,   DEVDRV_NPU2ACPU_HW_EXP_IRQ_NS_1},
	{DEVDRV_IRQ_MAILBOX_ACK,     DEVDRV_NPU2ACPU_HW_EXP_IRQ_NS_0}
};

int devdrv_plat_handle_irq_tophalf(u32 irq_index)
{
	int i;
	u32 gic_irq = 0;
	int map_len = sizeof(g_devdrv_npu_irq_gic_map)/(DEVDRV_NPU_IRQ_GIC_MAP_COLUMN * sizeof(u32));
	struct devdrv_platform_info *plat_info = devdrv_plat_get_info();
	COND_RETURN_ERROR(plat_info == NULL, -1, "devdrv_plat_get_info is NULL\n");

	for (i = 0; i < map_len; i++) {
		if (g_devdrv_npu_irq_gic_map[i][0] == irq_index) {
			gic_irq = g_devdrv_npu_irq_gic_map[i][1];
			break;
		}
	}
	COND_RETURN_ERROR(gic_irq == 0, -1, "invalide irq_index:%d\n", irq_index);

	devdrv_clr_hw_exp_irq_int((u64)(uintptr_t)DEVDRV_PLAT_GET_REG_VADDR(plat_info, DEVDRV_REG_HW_EXP_IRQ_NS_BASE), gic_irq);
	return 0;
}

int devdrv_plat_attach_sc(int fd, u64 offset, u64 size)
{
	int ret = 0;
	if (!npu_plat_is_support_sc()) {
		NPU_DRV_DEBUG("do not support sc\n");
		return 0;
	}

	/* syscahce attach interface with offset */
	NPU_DRV_INFO("fd = 0x%x, offset = 0x%lx, size = 0x%lx\n",fd, offset, size);
	ret = dma_buf_attach_lb(fd, PID_NPU, offset, (size_t)size);
	COND_RETURN_ERROR(ret != 0, -EINVAL, "fail to dma_buf_attach_lb, ret = %d\n", ret);

	return 0;
}

int devdrv_hisi_smmu_evt_register_notify(struct notifier_block *n)
{
	struct devdrv_platform_info *plat_info = devdrv_plat_get_info();
	COND_RETURN_ERROR(plat_info == NULL, -1, "devdrv_plat_get_info is NULL\n");

	return hisi_smmu_evt_register_notify(DEVDRV_PLAT_GET_PDEV(plat_info), n);
}

int devdrv_hisi_smmu_evt_unregister_notify(struct notifier_block *n)
{
	struct devdrv_platform_info *plat_info = devdrv_plat_get_info();
	COND_RETURN_ERROR(plat_info == NULL, -1, "devdrv_plat_get_info is NULL\n");

	return hisi_smmu_evt_unregister_notify(DEVDRV_PLAT_GET_PDEV(plat_info), n);

}

void npu_plat_aicore_pmu_config(struct devdrv_prof_info *profiling_info, uint32_t action, uint32_t register_index)
{
	SOC_NPU_AICORE_PMU_CTRL_UNION pmu_ctrl = {0};
	SOC_NPU_AICORE_PMU_START_CNT_CYC_UNION start_cnt = {0};
	SOC_NPU_AICORE_PMU_STOP_CNT_CYC_UNION stop_cnt = {0};
	SOC_NPU_AICORE_PMU_CNT0_IDX_UNION cnt0_idx = {0};
	SOC_NPU_AICORE_PMU_CNT1_IDX_UNION cnt1_idx = {0};
	SOC_NPU_AICORE_PMU_CNT2_IDX_UNION cnt2_idx = {0};
	SOC_NPU_AICORE_PMU_CNT3_IDX_UNION cnt3_idx = {0};
	SOC_NPU_AICORE_PMU_CNT4_IDX_UNION cnt4_idx = {0};
	SOC_NPU_AICORE_PMU_CNT5_IDX_UNION cnt5_idx = {0};
	SOC_NPU_AICORE_PMU_CNT6_IDX_UNION cnt6_idx = {0};
	SOC_NPU_AICORE_PMU_CNT7_IDX_UNION cnt7_idx = {0};
	u64 base_addr = 0;
	struct profiling_ai_core_config *cfg = NULL;

	struct devdrv_platform_info *plat_info = devdrv_plat_get_info();
	COND_RETURN_VOID(plat_info == NULL, "devdrv_plat_get_info failed");

	/* aicore1 base address */
	base_addr = (u64) (uintptr_t) DEVDRV_PLAT_GET_REG_VADDR(plat_info, register_index);

	cfg = &(profiling_info->head.manager.cfg.info.aicore);
	COND_RETURN_VOID(cfg == NULL, "aicore config ptr is null");
	COND_RETURN_VOID(cfg->eventNum == 0, "there has no aicore pmu items");

	NPU_DRV_INFO("npu plat config aicore pmu");
	/* disable aicore pmu for tsfw proc sq done */
	if (action == POWER_OFF) {
		pmu_ctrl.reg.pmu_en = 0x0; /* disable aicore pmu */
		pmu_ctrl.reg.sample_profile_mode = 0x0; /* disable sample profile mode */
		write_reg64_readback(SOC_NPU_AICORE_PMU_CTRL_ADDR(base_addr), pmu_ctrl.value);
	} else if (action == POWER_ON) {
		/* set aicore pmu cnt cyc */
		start_cnt.reg.pmu_start_cnt_cyc = 0x0;
		write_reg64_readback(SOC_NPU_AICORE_PMU_START_CNT_CYC_ADDR(base_addr), start_cnt.value);
		stop_cnt.reg.pmu_stop_cnt_cyc = 0xFFFFFFFF; /* max usigned int64 */
		write_reg64_readback(SOC_NPU_AICORE_PMU_STOP_CNT_CYC_ADDR(base_addr), stop_cnt.value);

		/* set aicore pmu event */
		cnt0_idx.reg.pmu_cnt0_idx = cfg->event[0];
		write_reg64_readback(SOC_NPU_AICORE_PMU_CNT0_IDX_ADDR(base_addr), cnt0_idx.value);
		cnt1_idx.reg.pmu_cnt1_idx = cfg->event[1];
		write_reg64_readback(SOC_NPU_AICORE_PMU_CNT1_IDX_ADDR(base_addr), cnt1_idx.value);
		cnt2_idx.reg.pmu_cnt2_idx = cfg->event[2];
		write_reg64_readback(SOC_NPU_AICORE_PMU_CNT2_IDX_ADDR(base_addr), cnt2_idx.value);
		cnt3_idx.reg.pmu_cnt3_idx = cfg->event[3];
		write_reg64_readback(SOC_NPU_AICORE_PMU_CNT3_IDX_ADDR(base_addr), cnt3_idx.value);
		cnt4_idx.reg.pmu_cnt4_idx = cfg->event[4];
		write_reg64_readback(SOC_NPU_AICORE_PMU_CNT4_IDX_ADDR(base_addr), cnt4_idx.value);
		cnt5_idx.reg.pmu_cnt5_idx = cfg->event[5];
		write_reg64_readback(SOC_NPU_AICORE_PMU_CNT5_IDX_ADDR(base_addr), cnt5_idx.value);
		cnt6_idx.reg.pmu_cnt6_idx = cfg->event[6];
		write_reg64_readback(SOC_NPU_AICORE_PMU_CNT6_IDX_ADDR(base_addr), cnt6_idx.value);
		cnt7_idx.reg.pmu_cnt7_idx = cfg->event[7];
		write_reg64_readback(SOC_NPU_AICORE_PMU_CNT7_IDX_ADDR(base_addr), cnt7_idx.value);

		/* set aicore pmu global enable */
		pmu_ctrl.reg.pmu_en = 0x1; /* enable aicore pmu */
		pmu_ctrl.reg.sample_profile_mode = 0x1; /* enable sample profile mode */
		write_reg64_readback(SOC_NPU_AICORE_PMU_CTRL_ADDR(base_addr), pmu_ctrl.value);
	}
}

int npu_plat_switch_hwts_aicore_pool(struct devdrv_dev_ctx *dev_ctx,
	struct npu_work_mode_info *work_mode_info, uint32_t power_status)
{
	int ret;
	struct devdrv_prof_info *profiling_info = NULL;
	npu_atf_hwts_aic_pool_switch hwts_aic_pool_swtich = {0};

	COND_RETURN_ERROR(dev_ctx == NULL, -1, "dev_ctx is null");
	COND_RETURN_ERROR(work_mode_info == NULL, -1, "work_mode_info is null");

	if (work_mode_info->work_mode == NPU_INIT) {
		NPU_DRV_INFO("work mode is npu init");
		return 0;
	}

	if (!npu_plat_is_support_ispnn() &&
		((work_mode_info->work_mode == NPU_ISPNN_SHARED) ||
		(work_mode_info->work_mode == NPU_ISPNN_SEPARATED))) {
		NPU_DRV_WARN("do not support ispnn");
		return -NOSUPPORT;
	}

	hwts_aic_pool_swtich.info.work_mode = work_mode_info->work_mode;
	hwts_aic_pool_swtich.info.work_mode_flags = work_mode_info->flags;
	hwts_aic_pool_swtich.info.power_status = power_status;
	if (npu_plat_aicore_get_disable_status(0))
		hwts_aic_pool_swtich.info.aic_status =
			npu_bitmap_set(hwts_aic_pool_swtich.info.aic_status, 0);
	if (npu_plat_aicore_get_disable_status(1))
		hwts_aic_pool_swtich.info.aic_status =
			npu_bitmap_set(hwts_aic_pool_swtich.info.aic_status, 1);

	NPU_DRV_WARN("work_mode 0x%x flags 0x%x power_status 0x%x aic_status 0x%x",
		work_mode_info->work_mode, work_mode_info->flags, power_status,
		hwts_aic_pool_swtich.info.aic_status);

	ret = npuatf_switch_hwts_aicore_pool(hwts_aic_pool_swtich.value);
	COND_RETURN_ERROR(ret != 0, ret,
		"hwts aicore pool switch failure, work_mode 0x%x flags 0x%x ret %d",
		work_mode_info->work_mode, work_mode_info->flags, ret);

	/* aicore pmu config for ispnn*/
	profiling_info = devdrv_calc_profiling_info(dev_ctx->devid);
	COND_RETURN_ERROR(profiling_info == NULL, -1, "profiling_info is null");
	if (((work_mode_info->work_mode == NPU_ISPNN_SHARED) ||
		(work_mode_info->work_mode == NPU_ISPNN_SEPARATED)) &&
		(work_mode_info->flags & NPU_DEV_WORKMODE_PROFILING)) {
		npu_plat_aicore_pmu_config(profiling_info,
			power_status, DEVDRV_REG_AIC1_BASE);
		if (work_mode_info->work_mode == NPU_ISPNN_SHARED)
			npu_plat_aicore_pmu_config(profiling_info,
				power_status, DEVDRV_REG_AIC0_BASE);
	}

	return 0;
}

static void npu_plat_pmu_enable(u64 aicore_addr)
{
	SOC_NPU_AICORE_PMU_CTRL_UNION pmu_ctrl = {0};
	SOC_NPU_AICORE_PMU_START_CNT_CYC_UNION start_cnt = {0};
	SOC_NPU_AICORE_PMU_STOP_CNT_CYC_UNION stop_cnt = {0};
	/* set aicore pmu cnt cyc */
	start_cnt.reg.pmu_start_cnt_cyc = 0x0;
	write_reg64_readback(SOC_NPU_AICORE_PMU_START_CNT_CYC_ADDR(aicore_addr), start_cnt.value);
	stop_cnt.reg.pmu_stop_cnt_cyc = 0xFFFFFFFFFFFFFFFFUL; /* max usigned int64 */
	write_reg64_readback(SOC_NPU_AICORE_PMU_STOP_CNT_CYC_ADDR(aicore_addr), stop_cnt.value);

	/* set aicore pmu global enable */
	pmu_ctrl.reg.pmu_en = 0x1; /* enable aicore pmu */
	pmu_ctrl.reg.sample_profile_mode = 0x1; /* enable sample profile mode */
	write_reg64_readback(SOC_NPU_AICORE_PMU_CTRL_ADDR(aicore_addr), pmu_ctrl.value);
}

void npu_plat_aicore_pmu_enable(uint32_t subip_set)
{
	u64 base_addr;
	struct devdrv_platform_info *plat_info = NULL;

	if (npu_dpm_enable_flag() == false)
		return;

	plat_info = devdrv_plat_get_info();
	COND_RETURN_VOID(plat_info == NULL, "devdrv_plat_get_info failed");

	if (BITMAP_GET(subip_set, NPU_AICORE0)) {
		/* aicore0 base address */
		base_addr = (u64) (uintptr_t) DEVDRV_PLAT_GET_REG_VADDR(plat_info, DEVDRV_REG_AIC0_BASE);
		/* set aicore pmu cnt cyc */
		npu_plat_pmu_enable(base_addr);
	}

	if (BITMAP_GET(subip_set, NPU_AICORE1)) {
		/* aicore1 base address */
		base_addr = (u64) (uintptr_t) DEVDRV_PLAT_GET_REG_VADDR(plat_info, DEVDRV_REG_AIC1_BASE);
		npu_plat_pmu_enable(base_addr);
	}

	NPU_DRV_DEBUG("exit npu_plat_aicore_pmu_enable");
	return;
}