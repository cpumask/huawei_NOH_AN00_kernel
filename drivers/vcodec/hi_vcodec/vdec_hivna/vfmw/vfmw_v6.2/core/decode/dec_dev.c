/*
 * dec_dev.c
 *
 * This is for dec dev module proc.
 *
 * Copyright (c) 2019-2020 Huawei Technologies CO., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include "dec_dev.h"
#include "dec_hal.h"
#include "vfmw_pdt.h"

dec_dev_pool g_dev_pool_entry;

static hi_s32 dec_dev_get_reg_vdh(void *dev, hi_u16 reg_id, void *reg);

static dec_dev_ops g_dec_dev_ops[] = {
	/* MFDE */
	{
		.open = dec_hal_open_vdh,
		.close = dec_hal_close_vdh,
		.reset = dec_hal_reset_vdh,
		.get_active_reg = dec_hal_enable_vdh,
		.start_reg = dec_hal_start_dec,
		.cancel_reg = dec_hal_cancel_vdh,
		.config_reg = dec_hal_config_reg_vdh,
		.check_int  = dec_hal_check_int,
		.get_reg = dec_dev_get_reg_vdh,
		.suspend = dec_hal_suspend_vdh,
		.resume = dec_hal_resume_vdh,
	},
};

static void dec_dev_init_dev_pool(void *args)
{
	dec_dev_info *dev = HI_NULL;
	hi_u16 id = get_current_sec_mode() == SEC_MODE ? 1 : 0;
	vfmw_module_reg_info *reg_info = (vfmw_module_reg_info *)args;

	if (dec_dev_get_dev(id, &dev) == DEC_OK) {
		dev->dev_id = id;
		dev->type = DEC_DEV_TYPE_VDH;
		dev->reg_phy_addr = reg_info->reg_phy_addr;
		dev->reg_size = reg_info->reg_range;
		dev->ops = &g_dec_dev_ops[(hi_s16)DEC_DEV_TYPE_VDH - 1];
		dev->last_count_time = 0;
		OS_SEMA_INIT(&(dev->sema));
	}
}

static void dec_dev_deinit_dev_pool(void)
{
	dec_dev_info *dev = HI_NULL;
	hi_u16 id = get_current_sec_mode() == SEC_MODE ? 1 : 0;

	if (dec_dev_get_dev(id, &dev) == DEC_OK) {
		OS_SEMA_EXIT(dev->sema);
		(void)memset_s(dev, sizeof(dec_dev_info), 0, sizeof(dec_dev_info));
	}
}

static hi_s32 dec_dev_get_reg_vdh(void *dec_dev, hi_u16 reg_id, void *reg)
{
	dec_dev_info *dev = (dec_dev_info *)dec_dev;
	dec_dev_vdh *vdh = (dec_dev_vdh *)dev->handle;
	vdh_reg_pool_info *reg_pool = &vdh->vdh_reg_pool;

	vdh_reg_info **vdh_reg = (vdh_reg_info **)reg;

	if (reg_id >= reg_pool->reg_num)
		return DEC_ERR;

	*vdh_reg = &reg_pool->vdh_reg[reg_id];

	return DEC_OK;
}

static hi_s16 dec_dev_get_cur_pxp_reg(dec_dev_info *dev, hi_u16 *reg_id)
{
	dec_dev_vdh *vdh = (dec_dev_vdh *)dev->handle;
	vdh_reg_pool_info *reg_pool = &vdh->vdh_reg_pool;
	hi_s16 ret = DEC_ERR;

	if (dev->type == DEC_DEV_TYPE_MDMA) {
		*reg_id = 0;
		ret = DEC_OK;
	} else {
		hi_u16 rid = reg_pool->next_pxp_reg;

		if (reg_pool->vdh_reg[rid].used == 1) {
			*reg_id = rid;
			ret = DEC_OK;
		}
	}

	return ret;
}

static hi_s16 dec_dev_update_pxp_reg(dec_dev_info *dev)
{
	dec_dev_vdh *vdh = (dec_dev_vdh *)dev->handle;
	vdh_reg_pool_info *reg_pool = &vdh->vdh_reg_pool;
	hi_s16 ret = DEC_ERR;

	if (dev->type == DEC_DEV_TYPE_MDMA) {
		ret = DEC_OK;
	} else if (dev->type == DEC_DEV_TYPE_VDH) {
		hi_u16 rid = reg_pool->next_pxp_reg;

		if (reg_pool->vdh_reg[rid].used == 1) {
			reg_pool->next_pxp_reg = (rid + 1) % reg_pool->reg_num;
			ret = DEC_OK;
		}
	}

	return ret;
}

static hi_s32 dec_hal_insert_reglist(dev_cfg_ioctl *dev_cfg_info,
	dec_back_up *back_up)
{
	dec_dev_pool *dev_pool = HI_NULL;
	dec_back_up_list *dev_irq_back_up = HI_NULL;
	hi_s32 ret;
	hi_ulong lock_flag;

	dev_pool = dec_dev_get_pool();
	dev_irq_back_up = &dev_pool->dev_irq_backup_list;
	OS_SPIN_LOCK(dev_irq_back_up->lock, &lock_flag);

	if (dev_irq_back_up->insert_index >= VDH_REG_NUM) {
		dprint(PRN_ERROR, "failed, insert_index = %hu\n",
			dev_irq_back_up->insert_index);
		ret = DEC_ERR;
		goto exit;
	}

	if (dev_irq_back_up->dev_back_up_list[dev_irq_back_up->insert_index].used == 1) {
		dprint(PRN_ERROR, "failed, list full\n");
		ret = DEC_ERR;
		goto exit;
	}

	dev_irq_back_up->dev_back_up_list[dev_irq_back_up->insert_index].used = 1;
	(void)memcpy_s(&dev_irq_back_up->dev_back_up_list[dev_irq_back_up->insert_index].dev_cfg_info,
		sizeof(dev_cfg_ioctl), dev_cfg_info, sizeof(dev_cfg_ioctl));
	(void)memcpy_s(&dev_irq_back_up->dev_back_up_list[dev_irq_back_up->insert_index].back_up,
		sizeof(dec_back_up), back_up, sizeof(dec_back_up));
	dev_irq_back_up->insert_index =
		(dev_irq_back_up->insert_index + 1) % VDH_REG_NUM;
	ret = DEC_OK;

exit:
	OS_SPIN_UNLOCK(dev_irq_back_up->lock, &lock_flag);
	if (ret == DEC_OK)
		OS_EVENT_GIVE(dev_irq_back_up->event);

	return ret;
}

static hi_s32 dec_dev_get_reglist(dec_back_up *back_up)
{
	dec_dev_pool *dev_pool = HI_NULL;
	dec_back_up_list *dev_irq_back_up = HI_NULL;
	hi_s32 ret;
	hi_ulong lock_flag;

	dev_pool = dec_dev_get_pool();
	dev_irq_back_up = &dev_pool->dev_irq_backup_list;
	OS_SPIN_LOCK(dev_irq_back_up->lock, &lock_flag);
	if (dev_irq_back_up->get_index >= VDH_REG_NUM) {
		dprint(PRN_ERROR, "failed, get_index = %hu\n", dev_irq_back_up->get_index);
		ret = DEC_ERR;
		goto exit;
	}

	if (dev_irq_back_up->dev_back_up_list[dev_irq_back_up->get_index].used == 0) {
		ret = DEC_ERR;
		goto exit;
	}

	(void)memcpy_s(back_up, sizeof(dec_back_up),
		&dev_irq_back_up->dev_back_up_list[dev_irq_back_up->get_index].back_up,
		sizeof(dec_back_up));
	dev_irq_back_up->dev_back_up_list[dev_irq_back_up->get_index].used = 0;
	dev_irq_back_up->get_index = (dev_irq_back_up->get_index + 1) % VDH_REG_NUM;
	ret = DEC_OK;

exit:
	OS_SPIN_UNLOCK(dev_irq_back_up->lock, &lock_flag);

	return ret;
}

static hi_s32 dec_dev_release_reg(dec_dev_info *dev, hi_u16 reg_id)
{
	dec_dev_vdh *vdh = HI_NULL;
	dec_dev_mdma *mdma = HI_NULL;
	hi_ulong lock_flag;

	if (dev->type == DEC_DEV_TYPE_MDMA) {
		mdma = (dec_dev_mdma *)dev->handle;
		mdma->used = 0;
		return DEC_OK;
	}

	vdh = (dec_dev_vdh *)dev->handle;
	OS_SPIN_LOCK(vdh->vdh_reg_pool.spin_lock, &lock_flag);
	vdh->vdh_reg_pool.vdh_reg[reg_id].used = 0;
	OS_SPIN_UNLOCK(vdh->vdh_reg_pool.spin_lock, &lock_flag);

	return DEC_OK;
}

static void dec_dev_pxp_int_process(hi_u16 dev_id)
{
	dec_dev_info *dev = HI_NULL;
	dev_cfg_ioctl dev_cfg_info;
	hi_u16 reg_id = 0;

	if (dec_dev_get_dev(dev_id, &dev) != DEC_OK) {
		dprint(PRN_ERROR, "get_dev failed\n");
		return;
	}

	if (!dev->handle) {
		dprint(PRN_ERROR, "dev->handle null\n");
		return;
	}

	if (dec_dev_get_cur_pxp_reg(dev, &reg_id) != DEC_OK) {
		dprint(PRN_ERROR, "get current pxp reg failed\n");
		return;
	}

	dec_hal_read_int_info(dev, reg_id);

	dev->decoder_time = OS_GET_TIME_US() - dev->last_count_time;

	if (dev->back_up.err_flag || dev->perf_enable)
		dprint(PRN_ALWS,
			"int: reg_id: %hu, dec_err: %u, frame number: %u, dec_time %u us\n",
			reg_id, dev->back_up.err_flag, dev->back_up.int_unid,
			dev->decoder_time);

	dec_hal_clear_int_state(dev, reg_id);

	dev_cfg_info.dev_id = dev_id;
	dev_cfg_info.type = dev->type;
	dev_cfg_info.reg_id = reg_id;
	dec_hal_insert_reglist(&dev_cfg_info, &dev->back_up);

	dec_dev_update_pxp_reg(dev);
	dec_dev_release_reg(dev, reg_id);
}

dec_dev_pool *dec_dev_get_pool(void)
{
	return &g_dev_pool_entry;
}

hi_s32 dec_dev_get_dev(hi_u16 dev_id, dec_dev_info **dev)
{
	dec_dev_pool *dev_pool = HI_NULL;

	if (dev_id >= DEC_DEV_NUM) {
		dprint(PRN_ERROR, "dev_id is error %hu\n", dev_id);
		return DEC_ERR;
	}

	dev_pool = dec_dev_get_pool();
	*dev = &(dev_pool->dev_inst[dev_id]);

	return DEC_OK;
}

static void dec_dev_init_source(void)
{
	dec_dev_pool *dev_pool = HI_NULL;

	dev_pool = dec_dev_get_pool();

	(void)memset_s(&dev_pool->dev_irq_backup_list, sizeof(dec_back_up_list),
		0, sizeof(dec_back_up_list));
	OS_EVENT_INIT(&dev_pool->dev_irq_backup_list.event, 0);
	OS_SPIN_LOCK_INIT(&dev_pool->dev_irq_backup_list.lock);
}

static void dec_dev_deinit_source(void)
{
	dec_dev_pool *dev_pool = HI_NULL;

	dev_pool = dec_dev_get_pool();

	OS_EVENT_EXIT(dev_pool->dev_irq_backup_list.event);
	OS_SPIN_LOCK_EXIT(dev_pool->dev_irq_backup_list.lock);
	(void)memset_s(&dev_pool->dev_irq_backup_list, sizeof(dec_back_up_list),
		0, sizeof(dec_back_up_list));
}

static hi_s32 dec_dev_open(void)
{
	dec_dev_info *dev = HI_NULL;
	hi_u16 dev_id;
	hi_s32 ret;

	dev_id = get_current_sec_mode() == NO_SEC_MODE ? 0 : 1;

	ret = dec_dev_get_dev(dev_id, &dev);
	if (ret != DEC_OK) {
		dprint(PRN_ERROR, "get dec dev failed.");
		return DEC_ERR;
	}

	vfmw_assert_ret(dev->state == DEC_DEV_STATE_NULL, DEC_ERR);
	vfmw_assert_ret(dev->ops != HI_NULL, DEC_ERR);
	vfmw_assert_ret(dev->ops->open != HI_NULL, DEC_ERR);

	if (dev->ops->open(dev) != DEC_OK) {
		dprint(PRN_ERROR, "open dec dev failed.");
		return DEC_ERR;
	}

	dev->state = DEC_DEV_STATE_RUN;

	return DEC_OK;
}

static void dec_dev_close(void)
{
	hi_u16 dev_id;
	dec_dev_info *dev = HI_NULL;
	hi_s32 ret;

	dev_id = get_current_sec_mode() == NO_SEC_MODE ? 0 : 1;
	ret = dec_dev_get_dev(dev_id, &dev);
	if (ret != DEC_OK) {
		dprint(PRN_ERROR, "get dev failed.");
		return;
	}

	if (dev->state == DEC_DEV_STATE_NULL) {
		dprint(PRN_ERROR, "dec dev state error\n");
		return;
	}

	dev->ops->close(dev);
	dev->state = DEC_DEV_STATE_NULL;
}

void dec_dev_init_entry(void)
{
	dec_dev_info *dev = HI_NULL;
	hi_u16 id = get_current_sec_mode() == SEC_MODE ? 1 : 0;

	if (dec_dev_get_dev(id, &dev) == DEC_OK) {
		(void)memset_s(dev, sizeof(dec_dev_info), 0, sizeof(dec_dev_info));
		dev->state = DEC_DEV_STATE_NULL;
	}
}

void dec_dev_deinit_entry(void)
{
	dec_dev_info *dev = HI_NULL;
	hi_u16 id = get_current_sec_mode() == SEC_MODE ? 1 : 0;

	if (dec_dev_get_dev(id, &dev) == DEC_OK)
		(void)memset_s(dev, sizeof(dec_dev_info), 0, sizeof(dec_dev_info));
}

hi_s32 dec_dev_init(void *args)
{
	hi_s32 ret = DEC_OK;

	dec_dev_init_dev_pool(args);
	dec_dev_init_source();

	if (dec_dev_open() != DEC_OK) {
		ret = DEC_ERR;
		goto out;
	}

	return ret;
out:
	dec_dev_deinit_source();
	dec_dev_deinit_dev_pool();

	return ret;
}

void dec_dev_deinit(void)
{
	dec_dev_close();
	dec_dev_deinit_source();
	dec_dev_deinit_dev_pool();
}

hi_s32 get_cur_active_reg(void *dev_cfg)
{
	hi_u16 dev_id = get_current_sec_mode() == SEC_MODE ? 1 : 0;
	dec_dev_info *dev = HI_NULL;
	dec_dev_cfg *cfg = (dec_dev_cfg *)dev_cfg;

	if (dec_dev_get_dev(dev_id, &dev))
		return DEC_ERR;

	vfmw_assert_ret(dev->state == DEC_DEV_STATE_RUN, DEC_ERR);
	dev->ops->get_active_reg(dev, &cfg->dev_cfg_info.reg_id);

	return DEC_OK;
}

hi_s32 dec_dev_config_reg(void *dev_cfg)
{
	hi_u16 dev_id = get_current_sec_mode() == SEC_MODE ? 1 : 0;
	dec_dev_info *dev = HI_NULL;
	dec_dev_cfg *cfg = (dec_dev_cfg *)dev_cfg;

	if (dec_dev_get_dev(dev_id, &dev))
		return DEC_ERR;

	vfmw_assert_ret(dev->state == DEC_DEV_STATE_RUN, DEC_ERR);

	if (dev->ops->config_reg(dev, dev_cfg)) {
		dprint(PRN_FATAL, "config vdh failed\n");
		return DEC_ERR;
	}

	if (dev->ops->start_reg(dev, cfg->dev_cfg_info.reg_id)) {
		dprint(PRN_FATAL, "start vdh failed\n");
		return DEC_ERR;
	}

	return DEC_OK;
}

hi_s32 dec_dev_isr_state(hi_u16 dev_id)
{
	dec_dev_info *dev = HI_NULL;

	if (dec_dev_get_dev(dev_id, &dev))
		return DEC_ERR;

	return dev->ops->check_int(dev, 0);
}

hi_s32 dec_dev_isr(hi_u16 dev_id)
{
	const hi_s32 irq_handled = 1;

	dec_dev_pxp_int_process(dev_id);

	return irq_handled;
}

static hi_s32 dec_dev_no_block_wait_vdh(dec_dev_cfg *cfg, dec_back_up *back_up,
	dec_dev_info *dev)
{
	vfmw_global_info *glb_info = pdt_get_glb_info();
	hi_s32 vdh_time_out = (glb_info->is_fpga ? VDH_FPGA_TIME_OUT : VDH_TIME_OUT);

	back_up->is_need_wait = 0;
	if (cfg->cur_time > vdh_time_out / 2) { // call delay 2ms
		dev->ops->reset(dev);
		return DEC_ERR;
	}
	if (dec_dev_get_reglist(back_up) != DEC_OK)
		back_up->is_need_wait = 1;

	return DEC_OK;
}

static hi_s32 dec_dev_block_wait_vdh(dec_dev_cfg *cfg, dec_back_up *back_up,
	dec_dev_info *dev)
{
	dec_back_up_list *dev_irq_back_up = HI_NULL;
	dec_dev_pool *dev_pool = HI_NULL;
	vfmw_global_info *glb_info = pdt_get_glb_info();
	hi_s32 vdh_time_out = (glb_info->is_fpga ? VDH_FPGA_TIME_OUT : VDH_TIME_OUT);

	dev_pool = dec_dev_get_pool();
	dev_irq_back_up = &dev_pool->dev_irq_backup_list;

	back_up->is_need_wait = 0;
	while (dec_dev_get_reglist(back_up) != DEC_OK) {
		if (OS_EVENT_WAIT(dev_irq_back_up->event, vdh_time_out) != OSAL_OK) {
			dprint(PRN_FATAL, "decoder timeout, dec reset now\n");
			dev->ops->reset(dev);
			return DEC_ERR;
		}
	}

	return DEC_OK;
}

hi_s32 dec_dev_get_backup(void *dev_cfg, void *read_backup)
{
	dec_dev_cfg *cfg = (dec_dev_cfg *)dev_cfg;
	dec_dev_info *dev = HI_NULL;
	dec_back_up *back_up = (dec_back_up *)read_backup;
	hi_u16 dev_id = get_current_sec_mode() == SEC_MODE ? 1 : 0;

	if (dec_dev_get_dev(dev_id, &dev) != DEC_OK) {
		dprint(PRN_FATAL, "get dec dev failed\n");
		return DEC_ERR;
	}

	vfmw_assert_ret(dev->state == DEC_DEV_STATE_RUN, DEC_ERR);

	if (get_current_sec_mode() ==  NO_SEC_MODE)
		return dec_dev_block_wait_vdh(cfg, back_up, dev);
	else
		return dec_dev_no_block_wait_vdh(cfg, back_up, dev);
}

void dec_dev_suspend(void)
{
	dec_dev_info *dev = HI_NULL;
	hi_u16 dev_id;

	for (dev_id = 0; dev_id < DEC_DEV_NUM; dev_id++) {
		if (dec_dev_get_dev(dev_id, &dev) != DEC_OK)
			continue;

		if (dev->state == DEC_DEV_STATE_NULL)
			continue;

		if (dev->ops->suspend)
			dev->ops->suspend(dev);

		dev->state = DEC_DEV_STATE_SUSPEND;
	}
}

void dec_dev_resume(void)
{
	dec_dev_info *dev = HI_NULL;
	hi_u16 dev_id;

	for (dev_id = 0; dev_id < DEC_DEV_NUM; dev_id++) {
		if (dec_dev_get_dev(dev_id, &dev) != DEC_OK)
			continue;

		if (dev->state == DEC_DEV_STATE_NULL)
			continue;

		if (dev->ops->resume)
			dev->ops->resume(dev);

		dev->state = DEC_DEV_STATE_RUN;
	}
}

void dec_dev_write_store_info(hi_u32 pid)
{
	dec_dev_info *dev = HI_NULL;
	hi_u16 dev_id = get_current_sec_mode() == SEC_MODE ? 1 : 0;
	if (dec_dev_get_dev(dev_id, &dev) != DEC_OK) {
		dprint(PRN_FATAL, "get dec dev failed\n");
		return;
	}

	dec_hal_write_store_info(dev, pid);
}

hi_u32 dec_dev_read_store_info(void)
{
	dec_dev_info *dev = HI_NULL;
	hi_u16 dev_id = get_current_sec_mode() == SEC_MODE ? 1 : 0;
	if (dec_dev_get_dev(dev_id, &dev) != DEC_OK) {
		dprint(PRN_FATAL, "get dec dev failed\n");
		return 0;
	}

	return dec_hal_read_store_info(dev);
}

