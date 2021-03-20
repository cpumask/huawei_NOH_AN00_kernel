/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: als para table ams source file
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
#include "als_para_table_ams.h"
#include <securec.h>

tcs3701_als_para_table tcs3708_als_para_diff_tp_color_table[] = {
	{ OCEAN, V3, DEFAULT_TPLCD, WHITE,
	  { 50, 638, 469, 5886, -907, 985, 0, 638, 469, 5886, -907, 985,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 6035, 2486,
	    2763, 1090, 4000, 250 }
	},
	{ OCEAN, V3, DEFAULT_TPLCD, GRAY,
	  { 50, 638, 469, 5886, -907, 985, 0, 638, 469, 5886, -907, 985,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 6035, 2486,
	    2763, 1090, 4000, 250 }
	},
	{ OCEAN, V3, DEFAULT_TPLCD, BLACK,
	  { 50, 638, 469, 5886, -907, 985, 0, 638, 469, 5886, -907, 985,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 6035, 2486,
	    2763, 1090, 4000, 250 }
	},
	{ OCEAN, V3, DEFAULT_TPLCD, BLACK2,
	  { 50, 638, 469, 5886, -907, 985, 0, 638, 469, 5886, -907, 985,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 6035, 2486,
	    2763, 1090, 4000, 250 }
	},
	{ OCEAN, V3, DEFAULT_TPLCD, GOLD,
	  { 50, 638, 469, 5886, -907, 985, 0, 638, 469, 5886, -907, 985,
	    0, 1, 20, -1000, 0, 0, 2776, 12000, 0, 0, -317, 1, 20, 0, 6035, 2486,
	    2763, 1090, 4000, 250 }
	},
};

tmd2745_als_para_table tmd2745_als_para_diff_tp_color_table[] = {
	/*
	 * tp_color reserved for future use
	 * AMS TMD2745: Extend-Data Format
	 * {D_Factor, B_Coef, C_Coef, D_Coef, is_min_algo, is_auto_gain}
	 * Here use WAS as an example
	 */
	{ WAS, V4, DEFAULT_TPLCD, 0,
	  { 538, 95, 1729, 190, 0, 0, 0, 0, 0, 0 }
	},

	{ COL, V4, TS_PANEL_UNKNOWN, 0,
	  { 510, 142, 803, 60, 0, 0, 493, 482, 5000, 200 }
	},
	{ COL, V4, TS_PANEL_OFILIM, 0,
	  { 510, 142, 803, 60, 0, 0, 494, 482, 5000, 200 }
	},
	{ COL, V4, TS_PANEL_TRULY, 0,
	  { 510, 142, 803, 60, 0, 0, 495, 482, 5000, 200 }
	},
	{ COL, V4, TS_PANEL_LENS, 0,
	  { 510, 142, 803, 60, 0, 0, 496, 482, 5000, 200 }
	},
};

tcs3707_als_para_table tcs3707_tp_color_table[] = {
	{ OTHER, OTHER, DEFAULT_TPLCD, OTHER,
	 { 100, 799, 127, -220, 1781, 60, 0, 800, 117, -114, 2016, -804,
	   0, 18, 1624, 0, 0, 2767, 20000, 0, 0, -143, 18, 7466, 3744, 2528,
	   1631, 10000, 100 } },
	{ MARX, V3, DEFAULT_TPLCD, WHITE,
	 { 100, 799, 127, -220, 1781, 60, 0, 800, 117, -114, 2016, -804,
	   0, 18, 1624, 0, 0, 2767, 20000, 0, 0, -143, 18, 7466, 3744, 2528,
	   1631, 10000, 100 } },
	{ MARX, V3, DEFAULT_TPLCD, BLACK,
	 { 100, 800, 218, -220, 752, -446, 0, 1088, 127, -704, 2917, -1483,
	   0, 20, 12000, 0, 0, 1911, 12000, 0, 0, 3470, 20, 10335, 5755, 3287,
	   2011, 10000, 100 } },
	{ WGR, V3, DEFAULT_TPLCD, WHITE,
		{ 100, 536, 569, -862, 5457, -3863, 0, 715, 235, -1135, 2801, 1015,
		0, 28, 15641, 0, 0, 744, 15250, 0, 0, 1450, 28, 5677, 2889, 1845,
		1214, 30000, 0 } },
	{ WGR, V3, DEFAULT_TPLCD, BLACK,
		{ 100, 720, 569, -862, 5457, -3863, 0, 823, 235, -1135, 2801, 1015,
		0, 32, 20615, 0, 0, 954, 9000, 0, 0, 5700, 32, 2765, 1496, 908,
		603, 30000, 0 } },
	{ SCMR, V3, DEFAULT_TPLCD, WHITE,
		{ 100, 800, 185, -20, 224, -669, 0, 800, 229, -148, 35, -145, 0,
		28, 6826, 0, 0, 2225, 12000, 0, 0, 1642, 28,
		21002, 10023, 8061, 4746, 10000, 100 } },
	{ SCMR, V3, DEFAULT_TPLCD, BLACK,
		{ 100, 800, -389, 493, 1604, -1888, 0, 800, 167, -358, 382, 159, 0,
		8207, 9075, 0, 0, 2089, 12000, 0, 0, 3061, 8207,
		9298, 4860, 3390, 1732, 20000, 100 } },
	{ KRJ, V3, DEFAULT_TPLCD, WHITE,
		{ 100, 1030, 108, -10, 542, -839, 0, 811, -300, 7, 1701, -705, 0, 30,
		5892, 0, 0, 2335, 612, 0, 0, 5565, 30,
		12638, 6632, 4196, 2761, 10000, 100 } },
	{ KRJ, V3, DEFAULT_TPLCD, BLACK,
		{ 100, 747, 372, -431, 1036, -812, 0, 800, 301, -379, 636, -282,
		0, 20, 9729, 0, 0, 2122, 3904, 0, 0, 4922, 20, 9257, 5474, 2835,
		1678, 10000, 100 } },
};

tcs3701_als_para_table *als_get_tcs3708_table_by_id(uint32_t id)
{
	if (id >= ARRAY_SIZE(tcs3708_als_para_diff_tp_color_table))
		return NULL;
	return &(tcs3708_als_para_diff_tp_color_table[id]);
}

tcs3701_als_para_table *als_get_tcs3708_first_table(void)
{
	return &(tcs3708_als_para_diff_tp_color_table[0]);
}

uint32_t als_get_tcs3708_table_count(void)
{
	return ARRAY_SIZE(tcs3708_als_para_diff_tp_color_table);
}

tmd2745_als_para_table *als_get_tmd2745_table_by_id(uint32_t id)
{
	if (id >= ARRAY_SIZE(tmd2745_als_para_diff_tp_color_table))
		return NULL;
	return &(tmd2745_als_para_diff_tp_color_table[id]);
}

tmd2745_als_para_table *als_get_tmd2745_first_table(void)
{
	return &(tmd2745_als_para_diff_tp_color_table[0]);
}

uint32_t als_get_tmd2745_table_count(void)
{
	return ARRAY_SIZE(tmd2745_als_para_diff_tp_color_table);
}

tcs3707_als_para_table *als_get_tcs3707_table_by_id(uint32_t id)
{
	if (id >= ARRAY_SIZE(tcs3707_tp_color_table))
		return NULL;
	return &(tcs3707_tp_color_table[id]);
}

tcs3707_als_para_table *als_get_tcs3707_first_table(void)
{
	return &(tcs3707_tp_color_table[0]);
}

uint32_t als_get_tcs3707_table_count(void)
{
	return ARRAY_SIZE(tcs3707_tp_color_table);
}
