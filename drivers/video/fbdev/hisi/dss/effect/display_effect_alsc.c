/* Copyright (c) 2019-2020, Hisilicon Tech. Co., Ltd. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

/* When hisi_fb.h is included, display_effect_alsc.h will be included */
#include "hisi_fb.h"

static bool g_is_alsc_init;

static void alsc_block_param_config(struct dss_alsc *alsc, const struct hisi_dss_alsc* const in_alsc)
{
	int block_cnt = 0;

	for (; block_cnt < ALSC_BLOCK_LEN; ++block_cnt) {
		alsc->addr_block[block_cnt]   = in_alsc->addr_block[block_cnt];
		alsc->ave_block[block_cnt]    = in_alsc->ave_block[block_cnt];
		alsc->coef_block_r[block_cnt] = in_alsc->coef_block_r[block_cnt];
		alsc->coef_block_g[block_cnt] = in_alsc->coef_block_g[block_cnt];
		alsc->coef_block_b[block_cnt] = in_alsc->coef_block_b[block_cnt];
		alsc->coef_block_c[block_cnt] = in_alsc->coef_block_c[block_cnt];
	}
}

static void alsc_degamma_coef_config(struct dss_alsc *alsc, const struct hisi_dss_alsc* const in_alsc)
{
	int block_cnt = 0;

	for (; block_cnt < ALSC_DEGAMMA_COEF_LEN; ++block_cnt) {
		alsc->degamma_r_coef[block_cnt] = in_alsc->degamma_lut_r[block_cnt];
		alsc->degamma_g_coef[block_cnt] = in_alsc->degamma_lut_g[block_cnt];
		alsc->degamma_b_coef[block_cnt] = in_alsc->degamma_lut_b[block_cnt];
	}
}

static void alsc_param_debug_print(const struct dss_alsc* const alsc)
{
	HISI_FB_DEBUG("action=%u "
		"alsc_en_by_dirty_region_limit=%u "
		"degamma_en=%u "
		"degamma_lut_sel=%u "
		"alsc_en=%u "
		"sensor_channel=%u "
		"pic_size=%x "
		"addr=%x "
		"size=%x "
		"bl_param.coef_r2r=%u "
		"bl_param.coef_r2g=%u "
		"bl_param.coef_r2b=%u "
		"bl_param.coef_r2c=%u "
		"bl_param.coef_g2r=%u "
		"bl_param.coef_g2g=%u "
		"bl_param.coef_g2b=%u "
		"bl_param.coef_g2c=%u "
		"bl_param.coef_b2r=%u "
		"bl_param.coef_b2g=%u "
		"bl_param.coef_b2b=%u"
		"bl_param.coef_b2c=%u\n",
		alsc->action,
		alsc->alsc_en_by_dirty_region_limit,
		alsc->degamma_en,
		alsc->degamma_lut_sel,
		alsc->alsc_en,
		alsc->sensor_channel,
		alsc->pic_size,
		alsc->addr,
		alsc->size,
		alsc->bl_param.coef_r2r,
		alsc->bl_param.coef_r2g,
		alsc->bl_param.coef_r2b,
		alsc->bl_param.coef_r2c,
		alsc->bl_param.coef_g2r,
		alsc->bl_param.coef_g2g,
		alsc->bl_param.coef_g2b,
		alsc->bl_param.coef_g2c,
		alsc->bl_param.coef_b2r,
		alsc->bl_param.coef_b2g,
		alsc->bl_param.coef_b2b,
		alsc->bl_param.coef_b2c);
}

static void alsc_reg_param_config(struct hisi_fb_data_type *hisifd,
	const struct hisi_dss_alsc* const in_alsc)
{
	struct dss_alsc *alsc = &(hisifd->alsc);
	struct lcd_dirty_region_info* dirty_region_info = &(hisifd->panel_info.dirty_region_info);

	mutex_lock(&(alsc->alsc_lock));
	alsc->action |= BIT(ALSC_ENABLE);
	dirty_region_info->alsc.alsc_addr = in_alsc->addr;
	dirty_region_info->alsc.alsc_size = in_alsc->size;

	alsc->action            |= in_alsc->action;
	alsc->degamma_en        = in_alsc->alsc_en & dirty_region_info->alsc.alsc_en;
	alsc->degamma_lut_sel   = 0;
	alsc->alsc_en           = in_alsc->alsc_en & dirty_region_info->alsc.alsc_en;
	alsc->alsc_en_by_dirty_region_limit = in_alsc->alsc_en & dirty_region_info->alsc.alsc_en;
	alsc->sensor_channel    = in_alsc->sensor_channel;
	alsc->pic_size          = dirty_region_info->alsc.pic_size;
	alsc->addr              = in_alsc->addr;
	alsc->addr_default      = in_alsc->addr;
	alsc->size              = in_alsc->size;
	alsc->size_default      = in_alsc->size;
	alsc->bl_param.coef_r2r = in_alsc->bl_param.coef_r2r;
	alsc->bl_param.coef_r2g = in_alsc->bl_param.coef_r2g;
	alsc->bl_param.coef_r2b = in_alsc->bl_param.coef_r2b;
	alsc->bl_param.coef_r2c = in_alsc->bl_param.coef_r2c;
	alsc->bl_param.coef_g2r = in_alsc->bl_param.coef_g2r;
	alsc->bl_param.coef_g2g = in_alsc->bl_param.coef_g2g;
	alsc->bl_param.coef_g2b = in_alsc->bl_param.coef_g2b;
	alsc->bl_param.coef_g2c = in_alsc->bl_param.coef_g2c;
	alsc->bl_param.coef_b2r = in_alsc->bl_param.coef_b2r;
	alsc->bl_param.coef_b2g = in_alsc->bl_param.coef_b2g;
	alsc->bl_param.coef_b2b = in_alsc->bl_param.coef_b2b;
	alsc->bl_param.coef_b2c = in_alsc->bl_param.coef_b2c;

	alsc_block_param_config(alsc, in_alsc);
	alsc_degamma_coef_config(alsc, in_alsc);
	mutex_unlock(&(alsc->alsc_lock));
	HISI_FB_DEBUG("[ALSC]ALSC init or update param loaded\n");
}


static void alsc_block_param_set_reg(const struct dss_alsc *alsc, char __iomem *alsc_base)
{
	int block_cnt = 0;
	int block_offset = 0;

	for (; block_cnt < ALSC_BLOCK_LEN; ++block_cnt) {
		block_offset = 0x4 * block_cnt;
		outp32(alsc_base + ALSC_ADDR_BLOCK + block_offset, alsc->addr_block[block_cnt]);
		outp32(alsc_base + ALSC_AVE_BLOCK + block_offset, alsc->ave_block[block_cnt]);
		outp32(alsc_base + ALSC_COEF_BLOCK_R_OFFSET + block_offset, alsc->coef_block_r[block_cnt]);
		outp32(alsc_base + ALSC_COEF_BLOCK_G_OFFSET + block_offset, alsc->coef_block_g[block_cnt]);
		outp32(alsc_base + ALSC_COEF_BLOCK_B_OFFSET + block_offset, alsc->coef_block_b[block_cnt]);
		outp32(alsc_base + ALSC_COEF_BLOCK_C_OFFSET + block_offset, alsc->coef_block_c[block_cnt]);
	}
}

static void alsc_degamma_coef_init_set_reg(const struct dss_alsc *alsc, char __iomem *alsc_base)
{
	int block_cnt = 0;
	int block_offset = 0;
	uint32_t val = 0;

	for (; block_cnt < ALSC_DEGAMMA_COEF_LEN - 1; block_cnt += 2) {
		block_offset = 0x2 * block_cnt;
		val = (alsc->degamma_r_coef[block_cnt] & ALSC_DEGAMMA_VAL_MASK);
		val |= ((alsc->degamma_r_coef[block_cnt + 1] & ALSC_DEGAMMA_VAL_MASK) << 16);
		outp32(alsc_base + U_DEGAMMA_R_COEF + block_offset, val);

		val = (alsc->degamma_g_coef[block_cnt] & ALSC_DEGAMMA_VAL_MASK);
		val |= ((alsc->degamma_g_coef[block_cnt + 1] & ALSC_DEGAMMA_VAL_MASK) << 16);
		outp32(alsc_base + U_DEGAMMA_G_COEF + block_offset, val);

		val = (alsc->degamma_b_coef[block_cnt] & ALSC_DEGAMMA_VAL_MASK);
		val |= ((alsc->degamma_b_coef[block_cnt + 1] & ALSC_DEGAMMA_VAL_MASK) << 16);
		outp32(alsc_base + U_DEGAMMA_B_COEF + block_offset, val);
	}

	block_offset += 0x4; /* The 129th odd coef */
	outp32(alsc_base + U_DEGAMMA_R_COEF + block_offset,
		alsc->degamma_r_coef[block_cnt] & ALSC_DEGAMMA_VAL_MASK);
	outp32(alsc_base + U_DEGAMMA_G_COEF + block_offset,
		alsc->degamma_g_coef[block_cnt] & ALSC_DEGAMMA_VAL_MASK);
	outp32(alsc_base + U_DEGAMMA_B_COEF + block_offset,
		alsc->degamma_b_coef[block_cnt] & ALSC_DEGAMMA_VAL_MASK);
}

static void alsc_bl_param_set_reg(struct hisi_fb_data_type *hisifd)
{
	char __iomem *alsc_base = NULL;
	struct dss_alsc *alsc = NULL;

	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is NULL!\n");

	alsc_base = hisifd->dss_base + ALSC_OFFSET;
	alsc = &(hisifd->alsc);
	if (!IS_BIT_ENABLE(alsc->action, ALSC_BL_UPDATE) && !IS_BIT_ENABLE(alsc->action, ALSC_ENABLE))
		return;

	outp32(alsc_base + ALSC_COEF_R2R, alsc->bl_param.coef_r2r);
	outp32(alsc_base + ALSC_COEF_R2G, alsc->bl_param.coef_r2g);
	outp32(alsc_base + ALSC_COEF_R2B, alsc->bl_param.coef_r2b);
	outp32(alsc_base + ALSC_COEF_R2C, alsc->bl_param.coef_r2c);
	outp32(alsc_base + ALSC_COEF_G2R, alsc->bl_param.coef_g2r);
	outp32(alsc_base + ALSC_COEF_G2G, alsc->bl_param.coef_g2g);
	outp32(alsc_base + ALSC_COEF_G2B, alsc->bl_param.coef_g2b);
	outp32(alsc_base + ALSC_COEF_G2C, alsc->bl_param.coef_g2c);
	outp32(alsc_base + ALSC_COEF_B2R, alsc->bl_param.coef_b2r);
	outp32(alsc_base + ALSC_COEF_B2G, alsc->bl_param.coef_b2g);
	outp32(alsc_base + ALSC_COEF_B2B, alsc->bl_param.coef_b2b);
	outp32(alsc_base + ALSC_COEF_B2C, alsc->bl_param.coef_b2c);
}

static void alsc_init_set_reg(struct hisi_fb_data_type *hisifd)
{
	char __iomem *alsc_base = hisifd->dss_base + ALSC_OFFSET;
	struct dss_alsc *alsc = &(hisifd->alsc);

	outp32(alsc_base + ALSC_DEGAMMA_EN, 0);
	outp32(alsc_base + ALSC_EN, 0);
	outp32(alsc_base + ALSC_DEGAMMA_MEM_CTRL, 0x00000008); /* CH0 ALSC DEGAMMA MEM */
	outp32(alsc_base + ALSC_SENSOR_CHANNEL, alsc->sensor_channel);
	outp32(alsc_base + ALSC_PIC_SIZE, alsc->pic_size);
	outp32(alsc_base + ALSC_ADDR, alsc->addr);
	outp32(alsc_base + ALSC_SIZE, alsc->size);
	alsc_bl_param_set_reg(hisifd);

	alsc_block_param_set_reg(alsc, alsc_base);

	alsc_degamma_coef_init_set_reg(alsc, alsc_base);

	outp32(alsc_base + ALSC_DEGAMMA_EN, alsc->alsc_en);
	outp32(alsc_base + ALSC_EN, alsc->alsc_en);
}

static void alsc_frame_set_reg(struct hisi_fb_data_type *hisifd)
{
	char __iomem *alsc_base = hisifd->dss_base + ALSC_OFFSET;
	struct dss_alsc *alsc = &(hisifd->alsc);
	uint32_t val = 0;

	val = alsc->alsc_en & alsc->alsc_en_by_dirty_region_limit;
	if (val)
		alsc->status = FB_ENABLE_BIT(alsc->status, ALSC_WORKING);
	else
		alsc->status = FB_DISABLE_BIT(alsc->status, ALSC_WORKING);

	outp32(alsc_base + ALSC_PIC_SIZE, alsc->pic_size);
	outp32(alsc_base + ALSC_ADDR, alsc->addr);
	alsc_bl_param_set_reg(hisifd);
}

void hisi_alsc_set_reg(struct hisi_fb_data_type *hisifd)
{
	char __iomem *alsc_base = NULL;
	struct dss_alsc *alsc = NULL;

	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is NULL!\n");

	if (!g_enable_alsc || !(hisifd->panel_info.dirty_region_info.alsc.alsc_en)) {
		hisifd->set_reg(hisifd, hisifd->dss_base + ALSC_OFFSET + ALSC_DEGAMMA_EN, 0, 32, 0);
		hisifd->set_reg(hisifd, hisifd->dss_base + ALSC_OFFSET + ALSC_EN, 0, 32, 0);
		return;
	}

	alsc_base = hisifd->dss_base + ALSC_OFFSET;
	alsc = &(hisifd->alsc);

	mutex_lock(&(alsc->alsc_lock));

	if (likely(!IS_BIT_ENABLE(alsc->action, ALSC_ENABLE)))
		alsc_frame_set_reg(hisifd);
	else
		alsc_init_set_reg(hisifd);

	alsc->action = BIT(ALSC_NO_ACTION);

	alsc_param_debug_print(alsc);
	mutex_unlock(&(alsc->alsc_lock));
}

static bool is_rect_included(const dss_rect_t *alsc_rect, const dss_rect_t *dirty_rect)
{
	struct rect rect_a = {
		.x1 = alsc_rect->x,
		.y1 = alsc_rect->y,
		.x2 = alsc_rect->x + alsc_rect->w,
		.y2 = alsc_rect->y + alsc_rect->h,
	};

	struct rect rect_b = {
		.x1 = dirty_rect->x,
		.y1 = dirty_rect->y,
		.x2 = dirty_rect->x + dirty_rect->w,
		.y2 = dirty_rect->y + dirty_rect->h,
	};

	return (rect_a.x1 >= rect_b.x1 && rect_a.y1 >= rect_b.y1 &&
		rect_a.x2 <= rect_b.x2 && rect_a.y2 <= rect_b.y2);
}

void hisi_alsc_update_dirty_region_limit(struct hisi_fb_data_type *hisifd)
{
	struct dss_alsc *alsc = NULL;
	dss_rect_t dirty_rect;
	dss_rect_t alsc_rect;

	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is NULL!\n");

	if (!g_enable_alsc || !(hisifd->panel_info.dirty_region_info.alsc.alsc_en))
		return;

	alsc = &(hisifd->alsc);
	dirty_rect = hisifd->ov_req.dirty_rect;

	mutex_lock(&(alsc->alsc_lock));
	alsc->alsc_en_by_dirty_region_limit = 1;

	alsc->addr = hisifd->panel_info.dirty_region_info.alsc.alsc_addr;
	alsc->pic_size = hisifd->panel_info.dirty_region_info.alsc.pic_size;

	/* No dirty_region_updt, use default value stored in dirty_region_info */
	if (dirty_rect.w == 0 || dirty_rect.h == 0) {
		mutex_unlock(&(alsc->alsc_lock));
		HISI_FB_DEBUG("No dirty region, use default alsc region\n");
		return;
	}

	alsc_rect.x = alsc->addr & ALSC_ADDR_X_MASK;
	alsc_rect.y = (alsc->addr >> ALSC_ADDR_Y_SHIFT) & ALSC_ADDR_X_MASK;
	alsc_rect.w = (alsc->size & ALSC_SIZE_X_MASK) + 1;
	alsc_rect.h = ((alsc->size >> ALSC_SIZE_Y_SHIFT) & ALSC_SIZE_X_MASK) + 1;

	/* 1. If dirty region doesn't overlap alsc region, bypass alsc module */
	if (!is_rect_included(&alsc_rect, &dirty_rect)) {
		alsc->alsc_en_by_dirty_region_limit = 0;
		mutex_unlock(&(alsc->alsc_lock));
		HISI_FB_DEBUG("dirty region doesn't overlap alsc region, bypass alsc module\n");
		return;
	}

	/* 2. Dirty region overlaps alsc region, update alsc region */
	alsc_rect.x -= dirty_rect.x;
	alsc_rect.y -= dirty_rect.y;

	alsc->addr = (uint32_t)alsc_rect.x | ((uint32_t)alsc_rect.y << ALSC_ADDR_Y_SHIFT);
	alsc->pic_size = (uint32_t)(dirty_rect.h - 1) | ((uint32_t)(dirty_rect.w - 1) << ALSC_PIC_WIDTH_SHIFT);
	HISI_FB_DEBUG("[ALSC]ALSC dirty limit en=%u, addr=0x%x, size=0x%x, pic_size=0x%x"
		" dirty region[%u %u %u %u]\n",
		alsc->alsc_en_by_dirty_region_limit, alsc->addr, alsc->size, alsc->pic_size,
		dirty_rect.x, dirty_rect.y, dirty_rect.w, dirty_rect.h);
	mutex_unlock(&(alsc->alsc_lock));
}

static void alsc_free_list_node(struct dss_alsc_data *node)
{
	struct dss_alsc_data *p = node;
	struct dss_alsc_data *q = node;

	if (!node)
		return;

	while (p) {
		q = p->next;
		kfree(p);
		p = q;
	}
}

static void alsc_free_data_storage(struct hisi_fb_data_type *hisifd)
{
	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is NULL!\n");

	if (!hisifd->alsc.data_head)
		return;

	/* alsc_data hasn't been a cycle */
	/* head-->node1...-->tail
	 * head<--node1...<--tail
	 *  |                 |
	 *  V                 V
	 * NULL              NULL
	 */
	if (!(hisifd->alsc.data_head->prev)) {
		alsc_free_list_node(hisifd->alsc.data_head);
		return;
	}

	/* alsc_data has been a cycle */
	/* head-->node1...-->tail
	 * head<--node1...<--tail
	 *  |^                |^
	 *  ||________________||
	 *  |__________________|
	 */
	hisifd->alsc.data_head->prev->next = NULL;
	alsc_free_list_node(hisifd->alsc.data_head);
}

static int alsc_init_data_storage(struct hisi_fb_data_type *hisifd)
{
	struct dss_alsc_data *p = NULL;
	struct dss_alsc_data *head = NULL;
	int i = 1;

	hisi_check_and_return(!hisifd, -1, ERR, "hisifd is NULL!\n");

	head = (struct dss_alsc_data*)kzalloc(sizeof(struct dss_alsc_data), GFP_ATOMIC);

	if (!head) {
		HISI_FB_ERR("[ALSC]alsc_init_data_storage failed\n");
		return -1;
	}
	/* First node of alsc data cycle */
	hisifd->alsc.data_head = head;
	hisifd->alsc.data_tail = head;

	/* rest node of alsc data cycle except for last node */
	for (; i < ALSC_MAX_DATA_LEN - 1; ++i) {
		p = (struct dss_alsc_data*)kzalloc(sizeof(struct dss_alsc_data), GFP_ATOMIC);
		if (!p) {
			alsc_free_data_storage(hisifd);
			HISI_FB_ERR("[ALSC]alsc_init_data_storage failed\n");
			return -1;
		}
		head->next = p;
		p->prev = head;
		p->next = NULL;
		head = p;
	}

	/* last node of alsc data cycle */
	p = (struct dss_alsc_data*)kzalloc(sizeof(struct dss_alsc_data), GFP_ATOMIC);
	if (!p) {
		alsc_free_data_storage(hisifd);
		HISI_FB_ERR("[ALSC]alsc_init_data_storage failed\n");
		return -1;
	}
	head->next = p;
	p->prev = head;
	p->next = hisifd->alsc.data_head;
	hisifd->alsc.data_head->prev = p;
	HISI_FB_DEBUG("[ALSC]alsc_init_data_storage success\n");

	return 0;
}

void hisi_alsc_store_data(struct hisi_fb_data_type *hisifd, uint32_t isr_flag)
{
	static uint32_t control_status;
	struct dss_alsc_data *p_data = NULL;
	char __iomem *alsc_base = NULL;
	struct timespec64 ts;

	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is NULL!\n");

	if (!g_enable_alsc || !g_is_alsc_init || !(hisifd->panel_info.dirty_region_info.alsc.alsc_en))
		return;

	p_data = hisifd->alsc.data_head;
	alsc_base = hisifd->dss_base + ALSC_OFFSET;

	if (!p_data) {
		HISI_FB_ERR("[ALSC]alsc data storage is NULL\n");
		return;
	}

	if (isr_flag & BIT_VACTIVE0_START) {
		/* we don't have control_flag here cause Timestamp must
		 * occur first
		 */
		p_data->timestamp_l = inp32(alsc_base + ALSC_TIMESTAMP_L);
		p_data->timestamp_h = inp32(alsc_base + ALSC_TIMESTAMP_H);
		control_status = hisifd->alsc.status;
		HISI_FB_DEBUG("[ALSC]store timestamp [0x%x 0x%x]\n", p_data->timestamp_l, p_data->timestamp_h);
	}

	if ((isr_flag & BIT_VACTIVE0_END)) {
		if (!IS_BIT_ENABLE(control_status, ALSC_WORKING) || IS_BIT_ENABLE(control_status, ALSC_BLANK))
			return;

		/* we need control_status here cause Noise should send or not must judge after
		 * VACTIVE0_START occurs
		 */
		p_data->noise1 = inp32(alsc_base + ALSC_NOISE1);
		p_data->noise2 = inp32(alsc_base + ALSC_NOISE2);
		p_data->noise3 = inp32(alsc_base + ALSC_NOISE3);
		p_data->noise4 = inp32(alsc_base + ALSC_NOISE4);
		get_monotonic_boottime64(&ts);
		p_data->ktimestamp = (uint64_t)ts.tv_sec * NSEC_PER_SEC + (uint64_t)ts.tv_nsec;
		p_data->status = 1;
		HISI_FB_DEBUG("[ALSC]store noise [%u %u %u %u], ktimestamp=%llu\n",
			p_data->noise1, p_data->noise2, p_data->noise3, p_data->noise4, p_data->ktimestamp);

		/* Move to next node of the alsc data ring cycle */
		hisifd->alsc.data_head = p_data->next;

		/* start workqueue */
		queue_work(hisifd->alsc_send_data_wq, &hisifd->alsc_send_data_work);
	}
}

static int alsc_update_bl_param(struct hisi_fb_data_type *hisifd,
	const struct alsc_bl_param* const in_bl_param)
{
	struct dss_alsc *alsc = &(hisifd->alsc);

	mutex_lock(&(alsc->alsc_lock));
	alsc->action |= BIT(ALSC_BL_UPDATE);
	alsc->bl_update_to_hwc = 1;
	alsc->bl_param.coef_r2r = in_bl_param->coef_r2r;
	alsc->bl_param.coef_r2g = in_bl_param->coef_r2g;
	alsc->bl_param.coef_r2b = in_bl_param->coef_r2b;
	alsc->bl_param.coef_r2c = in_bl_param->coef_r2c;
	alsc->bl_param.coef_g2r = in_bl_param->coef_g2r;
	alsc->bl_param.coef_g2g = in_bl_param->coef_g2g;
	alsc->bl_param.coef_g2b = in_bl_param->coef_g2b;
	alsc->bl_param.coef_g2c = in_bl_param->coef_g2c;
	alsc->bl_param.coef_b2r = in_bl_param->coef_b2r;
	alsc->bl_param.coef_b2g = in_bl_param->coef_b2g;
	alsc->bl_param.coef_b2b = in_bl_param->coef_b2b;
	alsc->bl_param.coef_b2c = in_bl_param->coef_b2c;
	mutex_unlock(&(alsc->alsc_lock));

	return 0;
}

void hisi_alsc_send_data_work_func(struct work_struct *work)
{
	struct hisi_fb_data_type *hisifd = container_of(work, struct hisi_fb_data_type, alsc_send_data_work);
	struct dss_alsc *alsc = NULL;
	struct dss_alsc_data *node = NULL;
	struct alsc_noise data_to_send;

	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is NULL!\n");
	alsc = &(hisifd->alsc);

	mutex_lock(&(alsc->alsc_lock));
	node = alsc->data_tail;
	if (!node) {
		HISI_FB_ERR("[ALSC]ALSC data node is NULL\n");
		mutex_unlock(&(alsc->alsc_lock));
		return;
	}

	data_to_send.status = 1;
	data_to_send.noise1 = node->noise1;
	data_to_send.noise2 = node->noise2;
	data_to_send.noise3 = node->noise3;
	data_to_send.noise4 = node->noise4;
	data_to_send.timestamp = node->ktimestamp;

	/* data tail node move */
	hisifd->alsc.data_tail = hisifd->alsc.data_tail->next;
	mutex_unlock(&(alsc->alsc_lock));

	if (alsc->cb_func.send_data_func)
		alsc->cb_func.send_data_func(&data_to_send);
}

void hisi_alsc_blank(struct hisi_fb_data_type *hisifd, int blank_mode)
{
	struct dss_alsc *alsc = NULL;

	if (hisifd->index != PRIMARY_PANEL_IDX)
		return;

	if (!(hisifd->panel_info.dirty_region_info.alsc.alsc_en))
		return;

	alsc = &(hisifd->alsc);

	mutex_lock(&(alsc->alsc_lock));

	if (blank_mode == FB_BLANK_UNBLANK) {
		alsc->action |= BIT(ALSC_ENABLE);
		alsc->status = FB_DISABLE_BIT(alsc->status, ALSC_BLANK);
	} else {
		alsc->status = FB_ENABLE_BIT(alsc->status, ALSC_BLANK);
		alsc->data_tail = alsc->data_head;
	}

	mutex_unlock(&(alsc->alsc_lock));
}

void hisi_alsc_init(struct hisi_fb_data_type *hisifd)
{
	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is NULL!\n");

	if (!(hisifd->panel_info.dirty_region_info.alsc.alsc_en))
		return;

	hisifd->panel_info.dirty_region_info.alsc.pic_size =
		ALSC_PICTURE_SIZE(hisifd->panel_info.xres - 1, hisifd->panel_info.yres - 1);
	hisifd->panel_info.dirty_region_info.alsc.alsc_addr = hisifd->alsc.addr_default;
	hisifd->panel_info.dirty_region_info.alsc.alsc_size = hisifd->alsc.size_default;

	if (!g_is_alsc_init) {
		if (alsc_init_data_storage(hisifd))
			return;

		hisifd->alsc_send_data_wq = create_singlethread_workqueue("alsc_send_data_wq");
		if (hisifd->alsc_send_data_wq == NULL) {
			HISI_FB_ERR("[ALSC]fb%d, create alsc send data workqueue failed!\n", hisifd->index);
			return;
		}
		INIT_WORK(&hisifd->alsc_send_data_work, hisi_alsc_send_data_work_func);

		mutex_init(&(hisifd->alsc.alsc_lock));
		hisifd->alsc.status = BIT(ALSC_UNINIT);
		g_is_alsc_init = true;
		HISI_FB_INFO("[ALSC]fb%d, resources init success\n", hisifd->index);
	}
}

void hisi_alsc_deinit(struct hisi_fb_data_type *hisifd)
{
	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is NULL!\n");

	if (g_is_alsc_init) {
		destroy_workqueue(hisifd->alsc_send_data_wq);
		hisifd->alsc_send_data_wq = NULL;

		alsc_free_data_storage(hisifd);
		mutex_destroy(&(hisifd->alsc.alsc_lock));
	}
	g_is_alsc_init = false;
}

/* Interfaces' implementation exposed to input_hub */
int dss_alsc_init(const struct hisi_dss_alsc* const in_alsc)
{
	struct hisi_fb_data_type *fb0 = NULL;
	hisi_check_and_return(!in_alsc, -1, ERR, "in_alsc is NULL!\n");

	if (!g_enable_alsc)
		return -1;

	fb0 = hisifd_list[PRIMARY_PANEL_IDX];
	hisi_check_and_return(!fb0, -1, ERR, "hisifd is NULL!\n");

	if (!(fb0->panel_info.dirty_region_info.alsc.alsc_en)) {
		HISI_FB_DEBUG("[ALSC]alsc_en = %d\n", fb0->panel_info.dirty_region_info.alsc.alsc_en);
		return -1;
	}

	alsc_reg_param_config(fb0, in_alsc);

	return 0;
}

int dss_alsc_update_bl_param(const struct alsc_bl_param* const bl_param)
{
	struct hisi_fb_data_type *fb0 = NULL;
	hisi_check_and_return(!bl_param, -1, ERR, "bl_param is NULL!\n");

	if (!g_enable_alsc)
		return -1;

	fb0 = hisifd_list[PRIMARY_PANEL_IDX];
	hisi_check_and_return(!fb0, -1, ERR, "hisifd is NULL!\n");

	if (!(fb0->panel_info.dirty_region_info.alsc.alsc_en))
		return -1;

	if (!g_is_alsc_init) {
		HISI_FB_ERR("[ALSC]ALSC hasn't been initialized, wrong operation from input_hub\n");
		return -1;
	}
	HISI_FB_DEBUG("+\n");
	return alsc_update_bl_param(fb0, bl_param);
}

int dss_alsc_register_cb_func(void (*func)(struct alsc_noise*))
{
	struct hisi_fb_data_type *fb0 = NULL;
	struct dss_alsc *alsc = NULL;

	hisi_check_and_return(!func, -1, ERR, "func is NULL!\n");

	if (!g_enable_alsc)
		return -1;

	fb0 = hisifd_list[PRIMARY_PANEL_IDX];
	hisi_check_and_return(!fb0, -1, ERR, "hisifd is NULL!\n");

	if (!(fb0->panel_info.dirty_region_info.alsc.alsc_en))
		return -1;

	alsc = &(fb0->alsc);

	mutex_lock(&(alsc->alsc_lock));

	alsc->cb_func.send_data_func = func;

	mutex_unlock(&(alsc->alsc_lock));

	return 0;
}
