/*
 * dec_dev.h
 *
 * This is for dec dev module intf.
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

#ifndef DEC_DEV_H
#define DEC_DEV_H

#include "dec_base.h"
#include "dec_hal.h"
#include "vfmw_osal.h"

typedef enum {
	DEC_DEV_STATE_NULL = 0,
	DEC_DEV_STATE_RUN,
	DEC_DEV_STATE_SUSPEND,
	DEC_DEV_STATE_MAX,
} dec_dev_state;

typedef struct {
	hi_s32 (*open)(void *dev);
	void (*close)(void *dev);
	void (*resume)(void *dev);
	void (*suspend)(void *dev);
	hi_s32 (*reset)(void *dev);
	void (*get_active_reg)(void *dev, hi_u16 *reg_id);
	hi_s32 (*check_int)(void *dev, hi_u16 reg_id);
	hi_s32 (*config_reg)(void *dev, void *task);
	hi_s32 (*start_reg)(void *dev, hi_u16 reg_id);
	hi_s32 (*cancel_reg)(void *dev, hi_u16 reg_id);
	hi_s32 (*get_msg)(void *dev, hi_u16 msg_id, void *msg);
	hi_s32 (*get_reg)(void *dev, hi_u16 reg_id, void *reg);
} dec_dev_ops;

typedef struct {
	hi_u8 used;
	hi_u16 reg_id;
	hi_u32 is_sec;
	hi_u32 base_ofs[2];
	UADDR reg_phy_addr;
	hi_u8 *reg_vir_addr;
} vdh_reg_info;

typedef struct {
	os_lock spin_lock;
	hi_u16 reg_num;
	hi_u16 next_pxp_reg;
	vdh_reg_info vdh_reg[VDH_REG_NUM];
} vdh_reg_pool_info;

typedef struct {
	vdh_reg_pool_info vdh_reg_pool;
	hi_s8 vdh_reg_id_verify;
} dec_dev_vdh;

typedef struct {
	UADDR reg_phy_addr;
	hi_u8 *reg_vir_addr;
} mdma_reg_info;

typedef struct {
	hi_u8 used;
	mdma_reg_info reg;
	hi_u32 irq_num;
	hi_u32 int_cnt;
	hi_u32 reg_cfg_cnt;
} dec_dev_mdma;

typedef struct {
	hi_u16 dev_id;
	dec_dev_type type;
	dec_dev_ops *ops;
	dec_dev_state state;
	dec_back_up back_up;
	void *handle;
	UADDR reg_phy_addr;
	hi_u32 reg_size;
	hi_u32 decoder_time;
	hi_u32 last_count_time;
	hi_u32 smmu_bypass;
	hi_u32 perf_enable;
	os_sema sema;
} dec_dev_info;

typedef struct {
	hi_u32 used;
	dev_cfg_ioctl dev_cfg_info;
	dec_back_up back_up;
} dev_back_up_info;

typedef struct {
	dev_back_up_info dev_back_up_list[VDH_REG_NUM];
	hi_u16 insert_index;
	hi_u16 get_index;
	os_event event;
	os_lock lock;
} dec_back_up_list;

typedef struct {
	dec_dev_info       dev_inst[DEC_DEV_NUM];
	dec_back_up_list   dev_irq_backup_list;
} dec_dev_pool;

hi_s32 dec_dev_get_dev(hi_u16 dev_id, dec_dev_info **dev);
dec_dev_pool *dec_dev_get_pool(void);
void dec_dev_init_entry(void);
void dec_dev_deinit_entry(void);

hi_s32 dec_dev_init(void *args);
void dec_dev_deinit(void);
hi_s32 get_cur_active_reg(void *dev_cfg);
hi_s32 dec_dev_config_reg(void *dev_cfg);

hi_s32 dec_dev_isr_state(hi_u16 dev_id);
hi_s32 dec_dev_isr(hi_u16 dev_id);
hi_s32 dec_dev_get_backup(void *dev_reg_cfg, void *backup_out);

void dec_dev_resume(void);
void dec_dev_suspend(void);

void dec_dev_write_store_info(hi_u32 pid);
hi_u32 dec_dev_read_store_info(void);

#endif
