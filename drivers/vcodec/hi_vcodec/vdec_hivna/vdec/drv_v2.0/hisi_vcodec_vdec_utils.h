/*
 * hisi_vcodec_vdec_utils.h
 *
 * This is for vdec utils
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

#ifndef HISI_VCODEC_VDEC_UTILITES_H
#define HISI_VCODEC_VDEC_UTILITES_H

#include "hi_type.h"

#define FUNC_COST_RANGE 10
#define FUNC_CNT 2
#define FUNC_NAME_POWER_ON 0
#define FUNC_NAME_POWER_OFF 1

#define FUNC_COST_SEG_1 1000 /* function time cost (0,1]ms */
#define FUNC_COST_SEG_2 2500 /* function time cost (1,2.5]ms */
#define FUNC_COST_SEG_3 4000 /* function time cost (2.5,4]ms */
#define FUNC_COST_SEG_4 6000 /* function time cost (4,6]ms */
#define FUNC_COST_SEG_5 10000 /* function time cost (6,10]ms */
#define FUNC_COST_SEG_6 13000 /* function time cost (10,13]ms */

enum {
	TIME_COST_SEG_1 = 0,
	TIME_COST_SEG_2,
	TIME_COST_SEG_3,
	TIME_COST_SEG_4,
	TIME_COST_SEG_5,
	TIME_COST_SEG_6,
	TIME_COST_SEG_7,
	TIME_COST_SEG_8,
};

hi_u64 get_time_in_us(void);
void vdec_init_power_cost_record(void);
hi_u64 *vdec_get_power_on_cost_record(void);
hi_u64 *vdec_get_power_off_cost_record(void);
void vdec_print_power_statistics(hi_u64 *power_off_duration, hi_u64 *power_off_times);
void vdec_record_power_operation_cost(hi_u64 start_time, hi_u64 *time_cost);

#endif
