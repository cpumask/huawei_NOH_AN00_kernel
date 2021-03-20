/*
 * dec_hal.c
 *
 * This is for dec hal module proc.
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

#include "dec_hal.h"
#include "dec_dev.h"
#include "dec_reg.h"
#include "vfmw_pdt.h"

#define DEC_IRQ_NUM 2
#define DEC_VDH_WAIT_COUNT 20
#define DEC_VDH_WAIT_TIME 5

typedef struct {
	hi_s32 irq_handle[DEC_IRQ_NUM];
	dec_dev_vdh vdh[VDH_NUM];
} dec_hal_ctx;

static dec_hal_ctx g_dec_hal_ctx;

static void dec_hal_create_vdh_inst(dec_dev_info *dev)
{
	void *vir = HI_NULL;

	vir = &g_dec_hal_ctx.vdh[dev->dev_id];

	dev->handle = vir;
	(void)memset_s(vir, sizeof(dec_dev_vdh), 0, sizeof(dec_dev_vdh));
}

static void dec_hal_destroy_vdh_inst(dec_dev_info *dev)
{
	dev->handle = HI_NULL;
}

static hi_s32 dec_hal_config_vdh(dec_dev_info *dev)
{
	hi_u32 i;
	UADDR phy_addr;
	hi_u8 *base_vir_addr = HI_NULL;
	dec_dev_vdh *vdh = (dec_dev_vdh *)dev->handle;
	hi_s32 is_sec = (get_current_sec_mode() == NO_SEC_MODE) ? 0 : 1;

	phy_addr = dev->reg_phy_addr;
	base_vir_addr = (hi_u8 *)OS_KMAP_REG(phy_addr, dev->reg_size);
	if (!base_vir_addr)
		return DEC_ERR;

	vdh->vdh_reg_pool.reg_num = VDH_REG_NUM;
	OS_SPIN_LOCK_INIT(&vdh->vdh_reg_pool.spin_lock);

	for (i = 0; i < vdh->vdh_reg_pool.reg_num; i++) {
		vdh->vdh_reg_pool.vdh_reg[i].used = 0;
		vdh->vdh_reg_pool.vdh_reg[i].reg_id = i;
		vdh->vdh_reg_pool.vdh_reg[i].is_sec = is_sec;
		vdh->vdh_reg_pool.vdh_reg[i].reg_phy_addr = phy_addr;
		vdh->vdh_reg_pool.vdh_reg[i].reg_vir_addr = base_vir_addr;
		vdh->vdh_reg_pool.vdh_reg[i].base_ofs[0] = OFS_BASE1;
		vdh->vdh_reg_pool.vdh_reg[i].base_ofs[1] = OFS_BASE2;

		dprint(PRN_DBG, "base 0x%x ofs1 0x%x ofs2 0x%x\n",
			base_vir_addr, vdh->vdh_reg_pool.vdh_reg[i].base_ofs[0],
			vdh->vdh_reg_pool.vdh_reg[i].base_ofs[1]);
	}

	return DEC_OK;
}

static void dec_hal_clear_vdh(dec_dev_info *dev)
{
	hi_u32 i;
	dec_dev_vdh *vdh = (dec_dev_vdh *)dev->handle;
	hi_u8 *base_vir_addr = HI_NULL;

	base_vir_addr = vdh->vdh_reg_pool.vdh_reg[0].reg_vir_addr;
	if (base_vir_addr)
		OS_KUNMAP_REG(base_vir_addr);

	for (i = 0; (i < vdh->vdh_reg_pool.reg_num) && (i < VDH_REG_NUM); i++) {
		vdh->vdh_reg_pool.vdh_reg[i].used = 0;
		vdh->vdh_reg_pool.vdh_reg[i].reg_phy_addr = 0;
		vdh->vdh_reg_pool.vdh_reg[i].reg_vir_addr = HI_NULL;
	}

	vdh->vdh_reg_pool.reg_num = 0;
	OS_SPIN_LOCK_EXIT(vdh->vdh_reg_pool.spin_lock);
}

void dec_hal_enable_vdh(void *dec_dev, hi_u16 *reg_id)
{
	hi_u32 pxp_reg_id = 0;
	hi_ulong lock_flag;
	vdh_reg_info *reg = HI_NULL;
	dec_dev_info *dev = (dec_dev_info *)dec_dev;
	dec_dev_vdh *vdh = (dec_dev_vdh *)dev->handle;
	vdh_reg_pool_info *reg_pool = &vdh->vdh_reg_pool;

	OS_SPIN_LOCK(reg_pool->spin_lock, &lock_flag);
	vdh->vdh_reg_id_verify = -1;
	OS_SPIN_UNLOCK(reg_pool->spin_lock, &lock_flag);
	dev->ops->get_reg(dev, 0, &reg);
	rd_vreg(reg->reg_vir_addr + reg->base_ofs[0], VREG_OFS_BSP_CUR_REG, pxp_reg_id);
	*reg_id = pxp_reg_id;

	if (pxp_reg_id < VDH_REG_NUM)
		reg_pool->next_pxp_reg = pxp_reg_id;
}

static void dec_hal_write_reg(vdh_reg_info *reg, dec_dev_cfg *dec_vdh_cfg)
{
	hi_s32 ofs;
	hi_u32 d32;
	hi_u32 std_cfg;
	hi_u16 dev_id = 0;
	hi_u32 pxp_num = VDH_PXP_NUM;
	hi_u32 *vir = HI_NULL;
	dec_dev_info *dev = HI_NULL;

	(void)dec_dev_get_dev(dev_id, &dev);

	vir = (hi_u32 *)(reg->reg_vir_addr + reg->base_ofs[0]);

	if (dec_vdh_cfg->is_frm_cpy == 1)
		pxp_num = 1;

	d32 = (pxp_num - 1) << 4;
	wr_vreg(reg->reg_vir_addr, VREG_OFS_PXP_CORE_EN, d32);

	d32 = 1;
	wr_vreg(vir, VREG_OFS_BSP_INTR_MSKSTATE, d32);

	d32 = 1;
	wr_vreg(vir, VREG_OFS_BSP_INTR_MASK, d32);

	d32 = 0;
	wr_vreg(vir, VREG_OFS_PXP_INTR_MASK, d32);

	d32 = 0;
	if (!dev->smmu_bypass)
		((vreg_coeff *)(&d32))->mfde_mmu_en = 1;
	else
		((vreg_coeff *)(&d32))->mfde_mmu_en = 0;

	wr_vreg(vir, VREG_OFS_VDH_COEFF, d32);

	ofs = VREG_OFS_BETWEEN_REG * reg->reg_id;

	vir = (hi_u32 *)(reg->reg_vir_addr + reg->base_ofs[1]);
	d32 = 0;
	((vreg_info_state *)(&d32))->low_latency_en = dec_vdh_cfg->is_slc_ldy;
	((vreg_info_state *)(&d32))->vdh_mdma_sel = dec_vdh_cfg->is_frm_cpy;
	((vreg_info_state *)(&d32))->int_report_en = 1;
	((vreg_info_state *)(&d32))->bsp_internal_ram = dec_vdh_cfg->vdh_in_ram;
	((vreg_info_state *)(&d32))->pxp_internal_ram = dec_vdh_cfg->vdh_in_ram;
	((vreg_info_state *)(&d32))->frame_indicator = dec_vdh_cfg->cfg_id;
	((vreg_info_state *)(&d32))->use_bsp_num = dec_vdh_cfg->list_num - 1;
	((vreg_info_state *)(&d32))->vdh_safe_flag = (reg->is_sec) ? 1 : 0;
	wr_vreg(vir, VREG_OFS_INFO_STATE + ofs, d32);

	d32 = hw_addr_shift(dec_vdh_cfg->pub_msg_phy_addr);
	wr_vreg(vir, VREG_OFS_AVM_ADDR_CNT + ofs, d32);

	d32 = 0;
	std_cfg = dec_vdh_cfg->std_cfg;
	((vreg_proc_state *)(&d32))->vid_std = std_cfg;
	wr_vreg(vir, VREG_OFS_PROCESS_STATE + ofs, d32);
}

hi_s32 dec_hal_open_vdh(void *dec_dev)
{
	dec_dev_info *dev = (dec_dev_info *)dec_dev;
	hi_u16 reg_id;

	dec_hal_create_vdh_inst(dev);

	if (dec_hal_config_vdh(dev) != DEC_OK)
		goto config_vdh_error;

	dec_hal_enable_vdh(dev, &reg_id);

	return DEC_OK;

config_vdh_error:
	dec_hal_destroy_vdh_inst(dev);

	dprint(PRN_ERROR, "failed\n");

	return DEC_ERR;
}

void dec_hal_close_vdh(void *dec_dev)
{
	dec_dev_info *dev = (dec_dev_info *)dec_dev;

	dec_hal_clear_vdh(dev);

	dec_hal_destroy_vdh_inst(dev);
}

hi_s32 dec_hal_reset_vdh(void *dec_dev)
{
	hi_u32 i;
	hi_u32 d32;
	hi_u32 rst_rq_reg_ofs;
	hi_u32 rst_ok_reg_ofs;
	hi_u32 int_mask[2];
	hi_ulong lock_flag;
	vdh_reg_info *reg = HI_NULL;
	dec_dev_vdh *vdh = HI_NULL;
	hi_u8 *rst_vir_addr = HI_NULL;
	vdh_reg_pool_info *reg_pool = HI_NULL;
	hi_u8 *reg_vir_addr = HI_NULL;
	hi_u32 pid_store = 0;

	dec_dev_info *dev = (dec_dev_info *)dec_dev;
	vfmw_global_info *glb_info = pdt_get_glb_info();

	rst_rq_reg_ofs = VDH_SRST_REQ_REG_OFS;
	rst_ok_reg_ofs = VDH_SRST_OK_REG_OFS;

	rst_vir_addr = glb_info->crg_reg_vaddr;
	vfmw_assert_ret(rst_vir_addr != HI_NULL, DEC_ERR);

	dev->ops->get_reg(dev, 0, &reg);
	reg_vir_addr = (hi_u8 *)(reg->reg_vir_addr + reg->base_ofs[0]);
	rd_vreg(reg_vir_addr, VREG_OFS_BSP_INTR_MASK, int_mask[0]);
	rd_vreg(reg_vir_addr, VREG_OFS_PXP_INTR_MASK, int_mask[1]);

	pid_store = dec_hal_read_store_info(dec_dev);

	rd_vreg(rst_vir_addr, rst_rq_reg_ofs, d32);
	((vcrg_vdh_srst_req *)(&d32))->vdh_mfde_srst_req = 1;
	wr_vreg(rst_vir_addr, rst_rq_reg_ofs, d32);

	for (i = 0; i < 1000; i++) {
		OS_UDELAY(1);
		rd_vreg(rst_vir_addr, rst_ok_reg_ofs, d32);
		if (((vcrg_vdh_srst_ok *)(&d32))->vdh_mfde_srst_ok == 1)
			break;
	}

	if (i == 1000)
		dprint(PRN_ERROR, "reset vdh failed\n");

	rd_vreg(rst_vir_addr, rst_rq_reg_ofs, d32);

	((vcrg_vdh_srst_req *)(&d32))->vdh_mfde_srst_req = 0;

	wr_vreg(rst_vir_addr, rst_rq_reg_ofs, d32);

	OS_MB();

	wr_vreg(reg_vir_addr, VREG_OFS_BSP_INTR_MASK, int_mask[0]);
	wr_vreg(reg_vir_addr, VREG_OFS_PXP_INTR_MASK, int_mask[1]);

	rd_vreg(reg_vir_addr, VREG_OFS_BSP_CUR_REG, d32);
	if (d32 != 0)
		dprint(PRN_ERROR, "reset already, but cur reg id not init to 0\n");

	if(pid_store != 0)
		dec_hal_write_store_info(dev, pid_store);

	vdh = (dec_dev_vdh *)dev->handle;
	reg_pool = &vdh->vdh_reg_pool;

	OS_SPIN_LOCK(reg_pool->spin_lock, &lock_flag);
	vdh->vdh_reg_id_verify = -1;
	for (i = 0; (i < reg_pool->reg_num) && (i < VDH_REG_NUM); i++)
		reg_pool->vdh_reg[i].used = 0;

	reg_pool->next_pxp_reg = 0;
	OS_SPIN_UNLOCK(reg_pool->spin_lock, &lock_flag);

	return DEC_OK;
}

void dec_hal_write_store_info(void *dec_dev, hi_u32 pid)
{
	dec_dev_info *dev = (dec_dev_info *)dec_dev;
	vdh_reg_info *reg = HI_NULL;
	hi_u8 *reg_vir_addr = HI_NULL;

	dev->ops->get_reg(dev, 0, &reg);
	reg_vir_addr = reg->reg_vir_addr;

	wr_vreg(reg_vir_addr, VREG_OFS_DEC_STORE, pid);
}

hi_u32 dec_hal_read_store_info(void *dec_dev)
{
	dec_dev_info *dev = (dec_dev_info *)dec_dev;
	hi_u32 pid = 0;
	vdh_reg_info *reg = HI_NULL;
	hi_u8 *reg_vir_addr = HI_NULL;

	dev->ops->get_reg(dev, 0, &reg);
	reg_vir_addr = reg->reg_vir_addr;

	rd_vreg(reg_vir_addr, VREG_OFS_DEC_STORE, pid);
	return pid;
}

hi_s32 dec_hal_config_reg_vdh(void *dec_dev, void *dec_vdh_cfg)
{
	hi_s32 ret;
	hi_ulong lock_flag;
	dec_dev_info *dev = (dec_dev_info *)dec_dev;
	dec_dev_vdh *vdh = (dec_dev_vdh *)dev->handle;
	dec_dev_cfg *dev_cfg = (dec_dev_cfg *)dec_vdh_cfg;
	vdh_reg_info *reg = HI_NULL;

	dprint(PRN_DBG, "reg_id is %hu, std_cfg %u, list_num %hu\n",
		dev_cfg->dev_cfg_info.reg_id, dev_cfg->std_cfg,
		dev_cfg->list_num);
	ret = dev->ops->get_reg(dev, dev_cfg->dev_cfg_info.reg_id, &reg);
	vfmw_assert_ret(ret == DEC_OK, DEC_ERR);

	dev->last_count_time = OS_GET_TIME_US();

	OS_SPIN_LOCK(vdh->vdh_reg_pool.spin_lock, &lock_flag);
	reg->used = 1;
	dec_hal_write_reg(reg, dev_cfg);
	OS_SPIN_UNLOCK(vdh->vdh_reg_pool.spin_lock, &lock_flag);

	return DEC_OK;
}

hi_s32 dec_hal_start_dec(void *dec_dev, hi_u16 reg_id)
{
	hi_u32 ofs;
	hi_s32 ret;
	dec_dev_info *dev = (dec_dev_info *)dec_dev;
	vdh_reg_info *reg = HI_NULL;

	ret = dev->ops->get_reg(dev, reg_id, &reg);
	vfmw_assert_ret(ret == DEC_OK, DEC_ERR);

	OS_MB();
	ofs = VREG_OFS_DEC_START + reg_id * VREG_OFS_BETWEEN_REG;
	wr_vreg(reg->reg_vir_addr + reg->base_ofs[1], ofs, 1);
	OS_MB();

	ofs = VREG_OFS_PXP_START + reg_id * VREG_OFS_BETWEEN_REG;
	wr_vreg(reg->reg_vir_addr + reg->base_ofs[1], ofs, 1);
	OS_MB();

	return DEC_OK;
}

hi_s32 dec_hal_check_int(void *dec_dev, hi_u16 reg_id)
{
	hi_u32 data;
	vdh_reg_info *reg = HI_NULL;
	hi_u32 *vir = HI_NULL;
	dec_dev_info *dev = (dec_dev_info *)dec_dev;

	dev->ops->get_reg(dev, reg_id, &reg);
	vir = (hi_u32 *)(reg->reg_vir_addr + reg->base_ofs[0]);

	rd_vreg(vir, VREG_OFS_PXP_INTR_STATE, data);
	if ((data >> 17) & 0x1)
		return DEC_OK;

	return DEC_ERR;
}

hi_s32 dec_hal_read_int_info(void *dec_dev, hi_u16 reg_id)
{
	hi_u32 data = 0;
	dec_dev_info *dev = HI_NULL;
	vdh_reg_info *reg = HI_NULL;
	mdma_reg_info *mdma = HI_NULL;
	hi_u32 *vir = HI_NULL;

	dev = (dec_dev_info *)dec_dev;
	(void)memset_s(&dev->back_up, sizeof(dec_back_up), 0, sizeof(dec_back_up));

	if (dev->type == DEC_DEV_TYPE_MDMA) {
		dev->ops->get_reg(dev, reg_id, &mdma);
		rd_vreg(mdma->reg_vir_addr, VREG_OFS_MDMA_INTR_STATE, data);
		dev->back_up.int_flag = data & 0x1;

		rd_vreg(mdma->reg_vir_addr, VREG_OFS_MDMA_UNID, data);
		dev->back_up.int_unid = data & 0xffff;
	} else {
		dev->ops->get_reg(dev, reg_id, &reg);
		vir = (hi_u32 *)(reg->reg_vir_addr + reg->base_ofs[0]);
		rd_vreg(vir, VREG_OFS_PXP_INTR_STATE, data);
		dev->back_up.int_flag = (data >> 17) & 0x1;
		dev->back_up.int_flag |= ((data >> 19) & 0x1) << 1;
		dev->back_up.int_unid = data & 0xffff;
		dev->back_up.err_flag = (data >> 18) & 0x1;
		dev->back_up.int_reg = (data >> 24) & 0x7;

		if (dev->back_up.err_flag)
			dev->back_up.rd_slc_msg = 1;

		rd_vreg(vir, VREG_OFS_BSP_CYCLES, data);
		dev->back_up.bsp_cycle = data;
		rd_vreg(vir, VREG_OFS_PXP_CYCLES, data);
		dev->back_up.pxp_cycle = data;
	}

	return DEC_OK;
}

hi_s32 dec_hal_clear_int_state(void *dec_dev, hi_u16 reg_id)
{
	hi_u32 data = 0;
	mdma_reg_info *mdma_reg = HI_NULL;
	vdh_reg_info *reg = HI_NULL;
	dec_dev_info *dev = (dec_dev_info *)dec_dev;

	if (dev->type == DEC_DEV_TYPE_MDMA) {
		data = 1;
		dev->ops->get_reg(dev, reg_id, &mdma_reg);
		wr_vreg(mdma_reg->reg_vir_addr, VREG_OFS_MDMA_INTR_CLR, data);
		OS_MB();
		return DEC_OK;
	}

	dev->ops->get_reg(dev, reg_id, &reg);

	if (dev->back_up.int_flag & 1)
		data |= 0x1;

	if (dev->back_up.int_flag & 2)
		data |= 0x1 << 2;

	wr_vreg(reg->reg_vir_addr + reg->base_ofs[0],
		VREG_OFS_PXP_INTR_MSKSTATE, data);
	OS_MB();

	return DEC_OK;
}

hi_s32 dec_hal_cancel_vdh(void *dec_dev, hi_u16 reg_id)
{
	hi_u32 i;
	hi_u32 d32;
	dec_dev_info *dev = (dec_dev_info *)dec_dev;
	vdh_reg_info *reg = HI_NULL;
	hi_u32 ofs = VREG_OFS_INFO_STATE + reg_id * VREG_OFS_BETWEEN_REG;

	dev->ops->get_reg(dev, reg_id, &reg);

	ofs += reg->base_ofs[1];
	rd_vreg(reg->reg_vir_addr, ofs, d32);

	((vreg_info_state *)(&d32))->dec_cancel = 1;
	wr_vreg(reg->reg_vir_addr, ofs, d32);
	OS_MB();

	for (i = 0; i < 10000; i++) {
		rd_vreg(reg->reg_vir_addr, ofs, d32);
		if (((vreg_info_state *)(&d32))->bsp_state == 1 &&
			((vreg_info_state *)(&d32))->pxp_state == 1)
			return DEC_OK;
		else
			OS_UDELAY(30);
	}

	dprint(PRN_ERROR, "failed\n");

	return DEC_ERR;
}

void dec_hal_suspend_vdh(void *dec_dev)
{
	hi_s32 rid;
	hi_ulong lock_flag;
	hi_s32 used_count = 0;
	hi_s32 wait_count = 0;
	dec_dev_info *dev = (dec_dev_info *)dec_dev;
	dec_dev_vdh *vdh = (dec_dev_vdh *)dev->handle;
	vdh_reg_pool_info *reg_pool = &vdh->vdh_reg_pool;

	do {
		if (wait_count > DEC_VDH_WAIT_COUNT && used_count > 0)
			dec_hal_reset_vdh(dev);

		for (rid = 0, used_count = 0; rid < VDH_REG_NUM; rid++) {
			OS_SPIN_LOCK(reg_pool->spin_lock, &lock_flag);
			if (reg_pool->vdh_reg[rid].used == 1)
				used_count++;

			OS_SPIN_UNLOCK(reg_pool->spin_lock, &lock_flag);
		}

		if (used_count == 0)
			break;

		wait_count++;
		OS_MSLEEP(DEC_VDH_WAIT_TIME);
	} while (used_count);
}

void dec_hal_resume_vdh(void *dec_dev)
{
	hi_u16 reg_id = 0;
	dec_dev_info *dev = (dec_dev_info *)dec_dev;

	dec_hal_enable_vdh(dev, &reg_id);
}
