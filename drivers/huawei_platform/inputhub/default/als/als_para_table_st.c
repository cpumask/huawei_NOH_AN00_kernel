/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: als para table st source file
 * Author: linjianpeng <linjianpeng1@huawei.com>
 * Create: 2020-05-25
 */

#include <linux/module.h>
#include <linux/err.h>
#include <linux/types.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/rtc.h>
#include <linux/of_device.h>
#include <linux/slab.h>
#include "contexthub_route.h"
#include "contexthub_boot.h"
#include "contexthub_recovery.h"
#include "protocol.h"
#include "sensor_config.h"
#include "sensor_detect.h"
#include "sensor_sysfs.h"
#include "als_tp_color.h"
#include "als_channel.h"
#include "als_para_table_st.h"
#include <securec.h>

vd6281_als_para_table vd6281_als_para_diff_tp_color_table[] = {
	{ LAYA, V4, DEFAULT_TPLCD, BLACK,
	  {25727, 4044, 9520, 0, 4589, 0, 4000, 250 }
	},
};

vd6281_als_para_table *als_get_vd6281_table_by_id(uint32_t id)
{
	if (id >= ARRAY_SIZE(vd6281_als_para_diff_tp_color_table))
		return NULL;
	return &(vd6281_als_para_diff_tp_color_table[id]);
}

vd6281_als_para_table *als_get_vd6281_first_table(void)
{
	return &(vd6281_als_para_diff_tp_color_table[0]);
}

uint32_t als_get_vd6281_table_count(void)
{
	return ARRAY_SIZE(vd6281_als_para_diff_tp_color_table);
}
