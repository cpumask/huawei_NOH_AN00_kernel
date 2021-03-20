/*
 * hisi_vcodec_vdec.h
 *
 * This is for vdec management
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

#ifndef HISI_VCODEC_VDEC
#define HISI_VCODEC_VDEC

#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include "drv_omxvdec.h"
#include "hi_type.h"

#define MAX_OPEN_COUNT 32
#define POWER_OFF_GAP_NUM 3
#define POWER_OFF_THRESHOLD 6500

typedef struct {
	hi_u64 vdec_open_timestamp_us;
	hi_u64 power_off_timestamp_us;
	hi_u64 power_off_on_times;
	hi_u64 power_off_duration_sum;
	hi_u32 power_off_duration[POWER_OFF_GAP_NUM];  /* store the duration between power off and power on. */
} vdec_power_info;

typedef struct {
	struct file *file;
} vdec_mode;

typedef struct {
	hi_u32 sec_chan_num;
	vdec_mode vdec_chan_info[MAX_OPEN_COUNT];
} vdec_chan_mode;

typedef struct {
	hi_u32 open_count;
	hi_u32 power_ref_cnt;
	atomic_t nor_chan_num;
	atomic_t sec_chan_num;
	struct mutex vdec_mutex;
	struct mutex vdec_mutex_scd;
	struct mutex vdec_mutex_vdh;
	struct mutex vdec_mutex_sec_scd;
	struct mutex vdec_mutex_sec_vdh;
	hi_bool device_locked;
	struct cdev cdev;
	struct device *device;
	vdec_chan_mode chan_mode;
	vdec_power_info vdec_power;
	bool power_state; /* false: power off; true: power on */
	spinlock_t power_state_spin_lock;
} vdec_entry;

enum ioctl_cid {
	IOCTL_SCD_PROC,
	IOCTL_IOMMU_MAP,
	IOCTL_IOMMU_UNMAP,
	IOCTL_GET_ACTIVE_REG,
	IOCTL_VDM_PROC,
	IOCTL_DEC_IRQ_QUERY,
	IOCTL_SET_CLK_RATE
};

typedef struct {
	hi_s32 share_fd;
	UADDR iova;
	hi_u64 vir_addr;
	hi_u32 iova_size;
} vdec_buffer_record;

hi_s32 vdec_check_power_on(void);
hi_s32 vdec_check_power_off(unsigned int cmd);
vdec_entry *vdec_get_entry(void);
#endif

