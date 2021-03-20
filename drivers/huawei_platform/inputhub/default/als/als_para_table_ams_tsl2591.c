/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: als para table ams tsl2591 source file
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
#include "als_para_table_ams_tsl2591.h"
#include <securec.h>

/*
 * Tsl2591 als para means:
 * phone_type, phone_version, touch_panel_manufacture, phone_type,
 * D_Factor, B_Cofe, C_Cofe, D_Cofe, E_Cofe,F_Cofe(low 16bit), F_Cofe(high 16bit),ir_threshold
 * is_min_algo, smooth_high_threshold, smooth_low_threshold, middle_visible_data, middle_ref_data,
 * g_compensate, offset_max, offset_min, dark_cali_lux_thresold, dark_cali_slope_threshold,
 * min_brightness
 */
tsl2591_als_para_table tsl2591_als_para_diff_tp_color_table[] = {
	{ TNY, V4, TS_PANEL_UNKNOWN, BLACK,
	  { 4458, 1549, 3490, 6031, 2771, 6040, 0, 500, 630, 200,
	    1305, 212, 58, 0, 0, 30000, 100, 0, 0, 0, 0, 0 }
	},

	{ TNY, VN1, TS_PANEL_UNKNOWN, BLACK,
	  { 7686, 1549, 3490, 6031, 2771, 6040, 0, 500, 630, 200,
	    491, 86, 58, 0, 0, 10000, 100, 0, 0, 0, 0, 0 }
	},

	{ EVER, V3, TS_PANEL_UNKNOWN, BLACK,
	  { 7793, 1604, 1898, 3197, 1833, 3079, 0, 500, 0, 0, 702,
	    115, 100, 0, 0, 4000, 250, 0, 0, 0, 0, 0 }
	},
	{ EVER, VN1, TS_PANEL_UNKNOWN, BLACK,
	  { 6683, 1551, 2041, 3393, 1833, 3027, 0, 500, 0, 0, 770,
	    132, 100, 0, 0, 4000, 250, 0, 0, 0, 0, 0 }
	},

	{ PCT, V4, TS_PANEL_UNKNOWN, BLACK,
	  { 25132, 1615, 4725, 8316, 5069, 11557, 0, 270, 590, 250,
	    104, 22, 58, 0, 0, 7500, 100, 0, 0, 0, 0, 0 }
	},

	{ PCT, VN1, TS_PANEL_UNKNOWN, BLACK,
	  { 25132, 1615, 4725, 8316, 5069, 11557, 0, 270, 590, 250,
	    104, 22, 58, 0, 0, 7500, 100, 0, 0, 0, 0, 0 }
	},

	{ YAL, V3, TM_TPLCD, BLACK,
	  { 13552, 1630, 3299, 6004, 3607, 10822, 0, 470, 420, 180,
	    240, 38, 56, 0, 0, 7500, 100, 180, 55, 25, 0, 0 }
	},

	{ YAL, V3, CTC_TPLCD, BLACK,
	  { 8465, 1600, 7480, 13762, 6727, 21864, 0, 500, 530, 90,
	    240, 38, 58, 0, 0, 7500, 100, 180, 55, 25, 0, 0 }
	},

	{ YAL, V3, LG_TPLCD, BLACK,
	  { 8170, 1650, 7576, 13652, 9900, 22274, 0, 500, 620, 280,
	    180, 28, 57, 0, 0, 7500, 100, 180, 55, 25, 0, 0 }
	},

	{ YAL, V3, BOE_TPLCD, BLACK,
	  { 2337, 1600, 6813, 11378, 2836, -28361, -1, 500, 380, 180,
	    395, 71, 57, 0, 0, 7500, 100, 180, 55, 25, 0, 0 }
	},

	{ YAL, V3, TS_PANEL_UNKNOWN, BLACK,
	  { 13552, 1630, 3299, 6004, 3607, 10822, 0, 470, 420, 180,
	    240, 38, 56, 0, 0, 7500, 100, 180, 55, 25, 0, 0 }
	},

	{ HLK, V4, AUO_TPLCD, OTHER,
	  { 7754, 1631, 2963, 5333, 3158, 8211, 0, 470, 530, 180, 491,
	    86, 57, 0, 0, 5000, 300, 0, 0, 0, 0, 0 }
	},

	{ HLK, V4, BOE_TPLCD, OTHER,
	  { 5703, 1540, 2192, 3376, 4956, 14372, 0, 500, 275, 245, 491,
	    86, 69, 0, 0, 5000, 300, 0, 0, 0, 0, 0 }
	},

	{ HLK, V4, INX_TPLCD, OTHER,
	  { 8485, 1590, 3616, 6472, 3079, 8005, 0, 500, 530, 180, 491,
	    86, 59, 0, 0, 5000, 300, 0, 0, 0, 0, 0 }
	},

	{ HLK, V4, TCL_TPLCD, OTHER,
	  { 7754, 1631, 2963, 5333, 3158, 8211, 0, 470, 530, 180, 491,
	    86, 57, 0, 0, 5000, 300, 0, 0, 0, 0, 0 }
	},

	{ HLK, V4, TM_TPLCD, OTHER,
	  { 7754, 1631, 2963, 5333, 3158, 8211, 0, 470, 530, 180, 491,
	    86, 57, 0, 0, 5000, 300, 0, 0, 0, 0, 0 }
	},

	{ HLK, V4, TS_PANEL_UNKNOWN, OTHER,
	  { 7754, 1631, 2963, 5333, 3158, 8211, 0, 470, 530, 180, 491,
	    86, 57, 0, 0, 5000, 300, 0, 0, 0, 0, 0 }
	},

	{ OXF, V3, TS_PANEL_UNKNOWN, OTHER,
	  { 6614, 1610, 4717, 9222, 10709, -23769, 0, 400, 380, 180, 605,
	    124, 58, 25, 57, 10000, 100, 600, 200, 25, 5500, 250 }
	},

	{ OXF, VN1, TM_TPLCD, OTHER,
	  { 9326, 1610, 4610, 8380, 6186, 22270, 0, 530, 460, 100, 225,
	    38, 58, 40, 30, 22500, 300, 600, 200, 25, 5500, 250 }
	},

	{ OXF, VN1, BOE_TPLCD, OTHER,
	  { 20349, 1620, 3724, 6796, 4851, 17465, 0, 500, 380, 100, 134,
	    25, 60, 45, 19, 13500, 180, 600, 200, 25, 5500, 250 }
	},

	{ OXF, VN1, AUO_TPLCD, OTHER,
	  { 9665, 1610, 3630, 6406, 4188, 13820, 0, 450, 500, 60, 280,
	    56, 58, 40, 40, 30000, 400, 600, 200, 25, 5500, 250 }
	},

	{ OXF, VN1, TCL_TPLCD, OTHER,
	  { 13813, 1570, 4071, 7206, 4482, 14789, 0, 450, 500, 60, 200,
	    33, 56, 40, 27, 20250, 270, 600, 200, 25, 5500, 250  }
	},

	{ WLZ, V3, TS_PANEL_UNKNOWN, OTHER,
	  { 6614, 1610, 4717, 9222, 10709, -23769, 0, 400, 380, 180, 605,
	    124, 58, 25, 57, 10000, 100, 600, 200, 25, 5500, 250 }
	},

	{ WLZ, VN1, TM_TPLCD, OTHER,
	  { 9326, 1610, 4610, 8380, 6186, 22270, 0, 530, 460, 100, 225,
	    38, 58, 40, 30, 22500, 300, 600, 200, 25, 5500, 250  }
	},

	{ WLZ, VN1, BOE_TPLCD, OTHER,
	  { 20349, 1620, 3724, 6796, 4851, 17465, 0, 500, 380, 100, 134,
	    25, 60, 45, 19, 13500, 180, 600, 200, 25, 5500, 250  }
	},

	{ WLZ, VN1, AUO_TPLCD, OTHER,
	  { 9665, 1610, 3630, 6406, 4188, 13820, 0, 450, 500, 60, 280,
	    56, 58, 40, 40, 30000, 400, 600, 200, 25, 5500, 250  }
	},

	{ WLZ, VN1, TCL_TPLCD, OTHER,
	  { 13813, 1570, 4071, 7206, 4482, 14789, 0, 450, 500, 60, 200,
	    33, 56, 40, 27, 20250, 270, 600, 200, 25, 5500, 250  }
	},

	{ JEF, V3, TS_PANEL_UNKNOWN, OTHER,
	  { 2177, 1510, 7194, 12791, 6645, 18607, 0, 450, 380, 200, 691,
	    94, 53, 40, 103, 10000, 100, 150, 200, 25 }
	},

	{ BMH, V3, TS_PANEL_UNKNOWN, OTHER,
	  { 2177, 1510, 7194, 12791, 6645, 18607, 0, 450, 380, 200, 691,
	    94, 53, 40, 103, 10000, 100, 150, 200, 25 }
	},

	{ TETON, V3, TS_PANEL_UNKNOWN, OTHER,
	  { 13938, 1590, 16036, 26804, 115, 1072, 0, 386, 380, 200, 5518,
	    27216, 59, 40, 24, 10000, 100, 600, 230, 25, 5000, 250}
	},

	{ TETON, V3, TS_PANEL_UNKNOWN, OTHER,
	  { 13938, 1590, 16036, 26804, 115, 1072, 0, 386, 380, 200, 5518,
	    27216, 59, 40, 24, 10000, 100, 600, 230, 25, 5000, 250}

	},

	{ TETON, V3, TS_PANEL_UNKNOWN, OTHER,
	  { 13938, 1590, 16036, 26804, 115, 1072, 0, 386, 380, 200, 5518,
	    27216, 59, 40, 24, 10000, 100, 600, 230, 25, 5000, 250}
	},

	{ TETON, V3, TS_PANEL_UNKNOWN, OTHER,
	  { 13938, 1590, 16036, 26804, 115, 1072, 0, 386, 380, 200, 5518,
	    27216, 59, 40, 24, 10000, 100, 600, 230, 25, 5000, 250}
	},

	{ TETON, V3, TS_PANEL_UNKNOWN, OTHER,
	  { 13938, 1590, 16036, 26804, 115, 1072, 0, 386, 380, 200, 5518,
	    27216, 59, 40, 24, 10000, 100, 600, 230, 25, 5000, 250}
	},

	{ TETON, V3, TS_PANEL_UNKNOWN, OTHER,
	  { 13938, 1590, 16036, 26804, 115, 1072, 0, 386, 380, 200, 5518,
	    27216, 59, 40, 24, 10000, 100, 600, 230, 25, 5000, 250}
	},

	{ TEL, V3, TCL_TPLCD, OTHER,
	  { 21394, 1660, 2889, 5432, 2891, 7806, 0, 500, 380, 200, 193,
	    25, 57, 40, 24, 10000, 500, 600, 230, 25, 5000, 250}
	},

	{ TEL, V3, TM_TPLCD, OTHER,
	  { 8699, 1510, 3072, 5069, 6214, 16156, 0, 500, 380, 200, 193,
	    25, 60, 40, 24, 10000, 500, 600, 230, 25, 5000, 250}
	},

	{ TEL, V3, BOE_TPLCD, OTHER,
	  { 14704, 1600, 2630, 4538, 4224, 12250, 0, 450, 380, 200, 193,
	    25, 57, 40, 24, 10000, 500, 600, 230, 25, 5000, 250}
	},

	{ TEL, V3, INX_TPLCD, OTHER,
	  { 16021, 1610, 3550, 6478, 3720, 9671, 0, 450, 380, 200, 193,
	    25, 58, 40, 24, 10000, 500, 600, 230, 25, 5000, 250}
	},

	{ TEL, V3, BOE_TPLCD2, OTHER,
	  { 14704, 1600, 2630, 4538, 4224, 12250, 0, 450, 380, 200, 193,
	    25, 57, 40, 24, 10000, 500, 600, 230, 25, 5000, 250}
	},

	{ TEL, V3, TS_PANEL_UNKNOWN, OTHER,
	  { 14704, 1600, 2630, 4538, 4224, 12250, 0, 450, 380, 200, 193,
	    25, 57, 40, 24, 10000, 500, 600, 230, 25, 5000, 250}
	},

	{ MXW, V3, TS_PANEL_UNKNOWN, OTHER,
	  { 9336, 1450, 13362, 22515, 12539, -22902, 0, 500, 380, 200, 193,
	    32, 58, 0, 0, 10000, 100, 400, 155, 25, 0, 0}
	},

	{ JSC, V3, TS_PANEL_UNKNOWN, OTHER,
	  { 2177, 1510, 7194, 12791, 6645, 18607, 0, 450, 380, 200, 691,
	    94, 53, 40, 103, 6500, 100, 150, 200, 25 }
	},
};

tsl2591_als_para_table *als_get_tsl2591_table_by_id(uint32_t id)
{
	if (id >= ARRAY_SIZE(tsl2591_als_para_diff_tp_color_table))
		return NULL;
	return &(tsl2591_als_para_diff_tp_color_table[id]);
}

tsl2591_als_para_table *als_get_tsl2591_first_table(void)
{
	return &(tsl2591_als_para_diff_tp_color_table[0]);
}

uint32_t als_get_tsl2591_table_count(void)
{
	return ARRAY_SIZE(tsl2591_als_para_diff_tp_color_table);
}
