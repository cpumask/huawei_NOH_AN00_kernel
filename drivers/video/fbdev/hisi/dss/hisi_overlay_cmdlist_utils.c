/* Copyright (c) 2013-2020, Hisilicon Tech. Co., Ltd. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#include "hisi_fb.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

#define CMDLIST_HEADER_LEN (SZ_1K)
#define CMDLIST_ITEM_LEN (SZ_8K)

struct list_item_param {
	uint8_t bw;
	uint8_t bs;
	uint32_t value;
	uint32_t new_addr;
	uint32_t old_addr;
	int condition;
};

struct dss_cmdlist_data *g_cmdlist_data;
uint32_t g_online_cmdlist_idxs;
uint32_t g_offline_cmdlist_idxs;

uint32_t g_cmdlist_chn_map[DSS_CMDLIST_MAX] = {
	DSS_CMDLIST_CHN_D2, DSS_CMDLIST_CHN_D3,
	DSS_CMDLIST_CHN_V0, DSS_CMDLIST_CHN_G0,
	DSS_CMDLIST_CHN_V1, DSS_CMDLIST_CHN_G1,
	DSS_CMDLIST_CHN_D0, DSS_CMDLIST_CHN_D1,
	DSS_CMDLIST_CHN_W0, DSS_CMDLIST_CHN_W1,
	DSS_CMDLIST_CHN_OV0, DSS_CMDLIST_CHN_OV1,
	DSS_CMDLIST_CHN_OV2, DSS_CMDLIST_CHN_OV3,
	DSS_CMDLIST_CHN_V2, DSS_CMDLIST_CHN_W2};

uint32_t g_cmdlist_eventlist_map[DSS_CMDLIST_MAX] = {
	DSS_EVENT_LIST_D2, DSS_EVENT_LIST_D3,
	DSS_EVENT_LIST_V0, DSS_EVENT_LIST_G0,
	DSS_EVENT_LIST_V1, DSS_EVENT_LIST_G1,
	DSS_EVENT_LIST_D0, DSS_EVENT_LIST_D1,
	DSS_EVENT_LIST_W0, DSS_EVENT_LIST_W1,
	DSS_EVENT_LIST_OV0, DSS_EVENT_LIST_OV1,
	DSS_EVENT_LIST_OV2, DSS_EVENT_LIST_OV3,
	DSS_EVENT_LIST_V2, DSS_EVENT_LIST_W2};

static uint32_t g_cmdlist_chnidx[DSS_CHN_MAX_DEFINE] = {
	DSS_CMDLIST_D2, DSS_CMDLIST_D3, DSS_CMDLIST_V0, DSS_CMDLIST_G0,
	DSS_CMDLIST_V1, DSS_CMDLIST_G1, DSS_CMDLIST_D0, DSS_CMDLIST_D1,
	DSS_CMDLIST_W0, DSS_CMDLIST_W1, DSS_CMDLIST_V2, DSS_CMDLIST_W2};

static uint32_t g_cmdlist_ovlidx[DSS_OVL_IDX_MAX] = {
	DSS_CMDLIST_OV0, DSS_CMDLIST_OV1,
	DSS_CMDLIST_OV2, DSS_CMDLIST_OV3};

int32_t hisi_get_cmdlist_idx_by_chnidx(uint32_t chnidx)
{
	if (unlikely(chnidx >= DSS_CHN_MAX_DEFINE)) {
		HISI_FB_ERR("chnidx %u is invalid!\n", chnidx);
		return -1;
	}

	return g_cmdlist_chnidx[chnidx];
}

int32_t hisi_get_cmdlist_idx_by_ovlidx(uint32_t ovlidx)
{
	if (unlikely(ovlidx >= DSS_OVL_IDX_MAX)) {
		HISI_FB_ERR("ovlidx %d is invalid!\n", ovlidx);
		return -1;
	}

	return g_cmdlist_ovlidx[ovlidx];
}

uint32_t hisi_get_cmdlist_chnidx(uint32_t poolidx)
{
	if (unlikely(poolidx >= DSS_CMDLIST_MAX)) {
		HISI_FB_ERR("poolidx %d is invalid!\n", poolidx);
		return DSS_CMDLIST_CHN_MAX;
	}

	return g_cmdlist_chn_map[poolidx];
}

#if !defined(CONFIG_HISI_DSS_CMDLIST_LAST_USED) || defined(CONFIG_HISI_FB_V320) || defined(CONFIG_HISI_FB_V330)
static uint32_t hisi_get_cmdlist_chnidxs_by_poolidxs(uint32_t poolidxs)
{
	int i;
	uint32_t cmdlist_idxs_temp = poolidxs;
	uint32_t cmdlist_chnidxs = 0;
	uint32_t cmdlist_chnidx;

	for (i = 0; i < HISI_DSS_CMDLIST_MAX; i++) {
		if ((cmdlist_idxs_temp & 0x1) == 0x1) {
			cmdlist_chnidx = hisi_get_cmdlist_chnidx(i);
			cmdlist_chnidxs |= (1 << cmdlist_chnidx);
		}

		cmdlist_idxs_temp = cmdlist_idxs_temp >> 1;
	}

	return cmdlist_chnidxs;
}
#endif

static int32_t hisi_get_eventlist_by_poolidx(uint32_t poolidx)
{
	if (unlikely(poolidx >= DSS_CMDLIST_MAX)) {
		HISI_FB_ERR("poolidx %d is invalid!\n", poolidx);
		return -1;
	}

	return g_cmdlist_eventlist_map[poolidx];
}

static int hisi_cmdlist_get_cmdlist_idxs_temp(dss_overlay_t *pov_req)
{
	uint32_t i;
	uint32_t k;
	uint32_t m;
	dss_layer_t *layer = NULL;
	dss_wb_layer_t *wb_layer = NULL;
	uint32_t cmdlist_idxs_temp = 0;
	dss_overlay_block_t *pov_h_block_infos = NULL;
	dss_overlay_block_t *pov_h_block = NULL;
	bool no_ovl_idx = false;

	pov_h_block_infos = (dss_overlay_block_t *)(uintptr_t)pov_req->ov_block_infos_ptr;
	for (m = 0; m < pov_req->ov_block_nums; m++) {
		pov_h_block = &(pov_h_block_infos[m]);
		for (i = 0; i < pov_h_block->layer_nums; i++) {
			layer = &(pov_h_block->layer_infos[i]);

			if (layer->need_cap & (CAP_BASE | CAP_DIM | CAP_PURE_COLOR))
				continue;

			cmdlist_idxs_temp |=
				(1 << (uint32_t)hisi_get_cmdlist_idx_by_chnidx((uint32_t)(layer->chn_idx)));
		}
	}

	if (pov_req->wb_enable == 1) {
		for (k = 0; k < pov_req->wb_layer_nums; k++) {
			wb_layer = &(pov_req->wb_layer_infos[k]);

			if (wb_layer->chn_idx == DSS_WCHN_W2)
				no_ovl_idx = true;

			cmdlist_idxs_temp |=
				(1 << (uint32_t)hisi_get_cmdlist_idx_by_chnidx((uint32_t)(wb_layer->chn_idx)));
		}
	}

	if (no_ovl_idx == false)
		cmdlist_idxs_temp |= (1 << (uint32_t)hisi_get_cmdlist_idx_by_ovlidx(pov_req->ovl_idx));

	return cmdlist_idxs_temp;
}

/* get cmdlist indexs */
int hisi_cmdlist_get_cmdlist_idxs(dss_overlay_t *pov_req,
	uint32_t *cmdlist_pre_idxs, uint32_t *cmdlist_idxs)
{
	uint32_t cmdlist_idxs_temp;

	cmdlist_idxs_temp = hisi_cmdlist_get_cmdlist_idxs_temp(pov_req);
	if (cmdlist_idxs_temp & (~HISI_DSS_CMDLIST_IDXS_MAX)) {
		HISI_FB_ERR("cmdlist_idxs_temp(0x%x) is invalid!\n", cmdlist_idxs_temp);
		return -EINVAL;
	}

	if (cmdlist_idxs && cmdlist_pre_idxs) {
		*cmdlist_idxs = cmdlist_idxs_temp;
		*cmdlist_pre_idxs &= (~(*cmdlist_idxs));
	} else if (cmdlist_idxs) {
		*cmdlist_idxs = cmdlist_idxs_temp;
	} else if (cmdlist_pre_idxs) {
		*cmdlist_pre_idxs = cmdlist_idxs_temp;
	} else {
		HISI_FB_ERR("cmdlist_idxs && cmdlist_pre_idxs is NULL!\n");
		return -EINVAL;
	}

	if (g_debug_ovl_cmdlist)
		HISI_FB_INFO("cmdlist_pre_idxs(0x%x), cmdlist_idxs(0x%x).\n",
			(cmdlist_pre_idxs ? *cmdlist_pre_idxs : 0),
			(cmdlist_idxs ? *cmdlist_idxs : 0));

	return 0;
}

uint32_t hisi_cmdlist_get_cmdlist_need_start(struct hisi_fb_data_type *hisifd, uint32_t cmdlist_idxs)
{
	uint32_t cmdlist_idxs_temp;

	if (!hisifd) {
		HISI_FB_ERR("hisifd is NULL\n");
		return 0;
	}

	cmdlist_idxs_temp = g_offline_cmdlist_idxs;
	g_offline_cmdlist_idxs |= cmdlist_idxs;
	cmdlist_idxs_temp = (g_offline_cmdlist_idxs & (~cmdlist_idxs_temp));

	cmdlist_idxs_temp |= (cmdlist_idxs & g_online_cmdlist_idxs);
	g_online_cmdlist_idxs &= (~cmdlist_idxs_temp);

	if (g_debug_ovl_cmdlist)
		HISI_FB_INFO("g_online_cmdlist_idxs=0x%x, cmdlist_idxs_need_start=0x%x\n",
			g_online_cmdlist_idxs, cmdlist_idxs_temp);

	return cmdlist_idxs_temp;
}

/*
 * data0: addr0[17:0]
 * data1: addr0[17:0] + addr1[5:0]
 * data2: addr0[17:0] + addr2[5:0]
 *
 * cnt[1:0]:
 * 2'b00: reg0
 * 2'b01: reg0, reg1
 * 2'b10: reg0, reg1, reg2
 * 2'b11: ((inp32(addr0) & data1) | data2) -> addr0
 */
bool hisi_check_cmdlist_paremeters_validate(uint8_t bw, uint8_t bs)
{
	if ((bs > 32) || (bw > 32) || ((bw + bs) > 32)) {
		HISI_FB_ERR("Can't do this,which may cause overflow.\n");
		return false;
	}

	return true;
}

static void list_item_set_reg(struct dss_cmdlist_node *node, struct list_item_param item_param)
{
	int index;
	uint64_t mask = ((uint64_t)1 << item_param.bw) - 1;

	index = node->item_index;
	if (item_param.bw != 32) {  /* 4bytes */
		if (node->item_flag != 0)
			index++;

		node->list_item[index].reg_addr.bits.add0 = item_param.new_addr;
		node->list_item[index].data0 = item_param.value;
		node->list_item[index].data1 = (uint32_t) ((~(mask << item_param.bs)) & 0xFFFFFFFF);
		node->list_item[index].data2 = (uint32_t) (((mask & item_param.value) << item_param.bs) & 0xFFFFFFFF);
		node->list_item[index].reg_addr.bits.cnt = 3;  /* number of config registers */
		node->item_flag = 3;  /* number of config registers */
	} else {
		if (node->item_flag == 0) {
			node->list_item[index].reg_addr.bits.add0 = item_param.new_addr;
			node->list_item[index].data0 = item_param.value;
			node->list_item[index].reg_addr.bits.cnt = 0;
			node->item_flag = 1;
		} else if (node->item_flag == 1 && item_param.condition) {
			node->list_item[index].reg_addr.bits.add1 = item_param.new_addr - item_param.old_addr;
			node->list_item[index].data1 = item_param.value;
			node->list_item[index].reg_addr.bits.cnt = 1;
			node->item_flag = 2;
		} else if (node->item_flag == 2 && item_param.condition) {
			node->list_item[index].reg_addr.bits.add2 = item_param.new_addr - item_param.old_addr;
			node->list_item[index].data2 = item_param.value;
			node->list_item[index].reg_addr.bits.cnt = 2;
			node->item_flag = 3;
		} else {
			index++;
			node->list_item[index].reg_addr.bits.add0 = item_param.new_addr;
			node->list_item[index].data0 = item_param.value;
			node->list_item[index].reg_addr.bits.cnt = 0;
			node->item_flag = 1;
		}
	}

	hisi_check_and_no_retval((index >= MAX_ITEM_INDEX), ERR, "index=%d is too large(1k)!\n", index);

	node->item_index = index;
	node->list_header->total_items.bits.count = node->item_index + 1;
}

void hisi_cmdlist_set_reg(struct hisi_fb_data_type *hisifd, char __iomem *addr,
	uint32_t value, uint8_t bw, uint8_t bs)
{
	struct dss_cmdlist_node *node = NULL;
	int cmdlist_idx;
	int index;
	uint64_t temp_addr = 0;
	struct list_item_param item_param = {0};

	item_param.bw = bw;
	item_param.bs = bs;
	item_param.value = value;

	hisi_check_and_no_retval(!addr, ERR, "addr is NULL!\n");
	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is NULL!\n");
	if (!hisi_check_cmdlist_paremeters_validate(bw, bs))
		return;

	cmdlist_idx = hisifd->cmdlist_idx;
	hisi_check_and_no_retval((cmdlist_idx < 0) || (cmdlist_idx >= HISI_DSS_CMDLIST_MAX), ERR,
		"cmdlist_idx is invalid\n");

	node = list_entry(hisifd->cmdlist_data->cmdlist_head_temp[cmdlist_idx].prev,
		struct dss_cmdlist_node, list_node);
	hisi_check_and_no_retval(!node, ERR, "node is NULL!\n");
	hisi_check_and_no_retval((node->node_type == CMDLIST_NODE_NOP), ERR,
		"can't set register value to NOP node!\n");

	index = node->item_index;
	if (hisifd->index == MEDIACOMMON_PANEL_IDX) {
		temp_addr = (uint64_t)(uintptr_t)(addr);
		item_param.new_addr = (uint32_t)(temp_addr & 0xFFFFFFFF);
	} else {
		item_param.new_addr = (uint32_t)(addr - hisifd->dss_base + hisifd->dss_base_phy);
	}

	item_param.new_addr = (item_param.new_addr >> 2) & CMDLIST_ADDR_OFFSET;
	item_param.old_addr = node->list_item[index].reg_addr.ul32 & CMDLIST_ADDR_OFFSET;
	item_param.condition = (((item_param.new_addr - item_param.old_addr) < MAX_ITEM_OFFSET) &&
		(item_param.new_addr >= item_param.old_addr));

	list_item_set_reg(node, item_param);
}

/*
 * flush cache for cmdlist, make sure that
 * cmdlist has writen through to memory before config register
 */
void hisi_cmdlist_flush_cache(struct hisi_fb_data_type *hisifd, uint32_t cmdlist_idxs)
{
	uint32_t i;
	uint32_t cmdlist_idxs_temp;
	struct dss_cmdlist_node *node = NULL;
	struct dss_cmdlist_node *_node_ = NULL;
	struct list_head *cmdlist_heads = NULL;

	if (!hisifd) {
		HISI_FB_ERR("hisifd is NULL!\n");
		return;
	}

	cmdlist_idxs_temp = cmdlist_idxs;

	for (i = 0; i < HISI_DSS_CMDLIST_MAX; i++) {
		if ((cmdlist_idxs_temp & 0x1) == 0x1) {
			cmdlist_heads = &(hisifd->cmdlist_data->cmdlist_head_temp[i]);
			if (!cmdlist_heads) {
				HISI_FB_ERR("cmdlist_data is NULL!\n");
				continue;
			}

			list_for_each_entry_safe_reverse(node, _node_, cmdlist_heads, list_node) {
				if (node) {
					dma_sync_single_for_device(NULL, node->header_phys,
						node->header_len, DMA_TO_DEVICE);
					dma_sync_single_for_device(NULL, node->item_phys,
						node->item_len, DMA_TO_DEVICE);
				}
			}
		}

		cmdlist_idxs_temp = cmdlist_idxs_temp >> 1;
	}
}

struct dss_cmdlist_node *hisi_cmdlist_node_alloc(struct hisi_fb_data_type *hisifd, size_t header_len,
	size_t item_len)
{
	struct dss_cmdlist_node *node = NULL;

	if (!hisifd) {
		HISI_FB_ERR("hisifd is NULL!\n");
		return NULL;
	}

	if (!hisifd->cmdlist_pool) {
		HISI_FB_ERR("fb%d, cmdlist_pool is NULL!\n", hisifd->index);
		return NULL;
	}

	node = (struct dss_cmdlist_node *)kzalloc(sizeof(struct dss_cmdlist_node), GFP_KERNEL);
	if (!node) {
		HISI_FB_ERR("failed to alloc struct dss_cmdlist_node!\n");
		return NULL;
	}

	memset(node, 0, sizeof(struct dss_cmdlist_node));

	node->header_len = header_len;
	node->item_len = item_len;

	/* alloc buffer for header */
	node->list_header = (struct cmd_header *)(uintptr_t)gen_pool_alloc(hisifd->cmdlist_pool, node->header_len);
	if (!node->list_header) {
		HISI_FB_ERR("fb%d, header gen_pool_alloc failed!\n", hisifd->index);
		goto err_header_alloc;
	}

	node->header_phys = gen_pool_virt_to_phys(hisifd->cmdlist_pool, (unsigned long)(uintptr_t)node->list_header);

	/* alloc buffer for items */
	node->list_item = (struct cmd_item *)(uintptr_t)gen_pool_alloc(hisifd->cmdlist_pool, node->item_len);
	if (!node->list_item) {
		HISI_FB_ERR("fb%d, item gen_pool_alloc failed!\n", hisifd->index);
		goto err_item_alloc;
	}

	node->item_phys = gen_pool_virt_to_phys(hisifd->cmdlist_pool, (unsigned long)(uintptr_t)node->list_item);

	memset(node->list_header, 0, node->header_len);
	memset(node->list_item, 0, node->item_len);

	/* fill node info */
	node->item_flag = 0;
	node->item_index = 0;

	node->is_used = 0;
	node->node_type = CMDLIST_NODE_NONE;
	return node;

err_item_alloc:
	if (node->list_header)
		gen_pool_free(hisifd->cmdlist_pool, (unsigned long)(uintptr_t)node->list_header, node->header_len);
err_header_alloc:
	if (node) {
		kfree(node);
		node = NULL;
	}

	return node;
}

void hisi_cmdlist_node_free(struct hisi_fb_data_type *hisifd, struct dss_cmdlist_node *node)
{
	if (!hisifd) {
		HISI_FB_ERR("hisifd is NULL!\n");
		return;
	}

	if (!node) {
		HISI_FB_ERR("node is NULL!\n");
		return;
	}

	if (hisifd->cmdlist_pool && node->list_header) {
		gen_pool_free(hisifd->cmdlist_pool, (unsigned long)(uintptr_t)node->list_header, node->header_len);
		node->list_header = NULL;
	}

	if (hisifd->cmdlist_pool && node->list_item) {
		gen_pool_free(hisifd->cmdlist_pool, (unsigned long)(uintptr_t)node->list_item, node->item_len);
		node->list_item = NULL;
	}

	kfree(node);
	node = NULL;
}

static struct dss_cmdlist_node *hisi_cmdlist_get_free_node(struct dss_cmdlist_node *node[], int *id)
{
	int i;

	for (i = 0; i < HISI_DSS_CMDLIST_NODE_MAX; i++) {
		if (node[i] && (node[i]->is_used == 0)) {
			node[i]->is_used = 1;
			*id = i + 1;
			return node[i];
		}
	}

	return NULL;
}

static bool hisi_cmdlist_addr_check(struct hisi_fb_data_type *hisifd, uint32_t *list_addr)
{
	if (!hisifd) {
		HISI_FB_ERR("hisifd is NULL\n");
		return false;
	}

	if ((*list_addr > (uint32_t)(hisifd->cmdlist_pool_phy_addr + hisifd->sum_cmdlist_pool_size)) ||
		(*list_addr < (uint32_t)hisifd->cmdlist_pool_phy_addr)) {
		HISI_FB_ERR("fb%d, cmdlist_addr is invalid, sum_cmdlist_pool_size=%zu.\n",
			hisifd->index, hisifd->sum_cmdlist_pool_size);
		*list_addr = hisifd->cmdlist_pool_phy_addr;
		return false;
	}

	return true;
}

int hisi_cmdlist_add_nop_node(struct hisi_fb_data_type *hisifd, uint32_t cmdlist_idxs,
	int pending, int reserved)
{
	struct dss_cmdlist_node *node = NULL;
	struct dss_cmdlist_node *first_node = NULL;
	uint32_t cmdlist_idxs_temp;
	int i;
	int id = 0;

	if (!hisifd) {
		HISI_FB_ERR("hisifd is NULL\n");
		return -EINVAL;
	}

	cmdlist_idxs_temp = cmdlist_idxs;

	for (i = 0; i < HISI_DSS_CMDLIST_MAX; i++) {
		if ((cmdlist_idxs_temp & 0x1) == 0x1) {
			node = hisi_cmdlist_get_free_node(hisifd->cmdlist_data->cmdlist_nodes_temp[i], &id);
			if (!node) {
				HISI_FB_ERR("failed to hisi_get_free_cmdlist_node!\n");
				return -EINVAL;
			}

			node->list_header->flag.bits.id = id;
			node->list_header->flag.bits.nop = 0x1;
			node->list_header->flag.bits.pending = pending ? 0x1 : 0x0;
			node->list_header->flag.bits.valid_flag = CMDLIST_NODE_VALID;
			node->list_header->flag.bits.last = 0;
			node->list_header->next_list = node->header_phys;
			hisi_cmdlist_addr_check(hisifd, &(node->list_header->next_list));

			node->is_used = 1;
			node->node_type = CMDLIST_NODE_NOP;
			node->reserved = reserved ? 0x1 : 0x0;

			/* add this nop to list */
			list_add_tail(&(node->list_node), &(hisifd->cmdlist_data->cmdlist_head_temp[i]));

			if (node->list_node.prev != &(hisifd->cmdlist_data->cmdlist_head_temp[i])) {
				struct dss_cmdlist_node *pre_node;

				first_node = list_first_entry(&(hisifd->cmdlist_data->cmdlist_head_temp[i]),
					struct dss_cmdlist_node, list_node);
				pre_node = list_entry(node->list_node.prev, struct dss_cmdlist_node, list_node);
				pre_node->list_header->next_list = first_node->header_phys;
				hisi_cmdlist_addr_check(hisifd, &(pre_node->list_header->next_list));

				if (node->list_header->flag.bits.pending == 0x1)
					pre_node->reserved = 0x0;

				pre_node->list_header->flag.bits.task_end = 0x1;
				node->list_header->flag.bits.task_end = 0x1;

				if (g_debug_ovl_cmdlist)
					HISI_FB_DEBUG("i = %d, next_list = 0x%x\n", i, (uint32_t)(node->header_phys));
			}
		}

		cmdlist_idxs_temp = cmdlist_idxs_temp >> 1;
	}

	return 0;
}

static void set_list_header_bits_event_list(struct dss_cmdlist_node *node, int remove,
	int i, int32_t event_list)
{
	if (i < DSS_CMDLIST_W0)
		node->list_header->flag.bits.event_list = remove ? 0x8 : (event_list);
	else if (i < DSS_CMDLIST_OV0)
		node->list_header->flag.bits.event_list = remove ? 0x8 : (event_list);
#if defined(CONFIG_HISI_FB_970) || defined(CONFIG_HISI_FB_V501) || \
	defined(CONFIG_HISI_FB_V510) || defined(CONFIG_HISI_FB_V600)
	else if (i == DSS_CMDLIST_V2)
		node->list_header->flag.bits.event_list = remove ? 0x8 : 0x16;
	else if (i == DSS_CMDLIST_W2)
		node->list_header->flag.bits.event_list = remove ? 0x8 : 0x20;
#endif
	else
		node->list_header->flag.bits.event_list = remove ? 0x8 : (event_list);
}

int hisi_cmdlist_add_new_node(struct hisi_fb_data_type *hisifd, uint32_t cmdlist_idxs,
	union cmd_flag flag, int remove)
{
	struct dss_cmdlist_node *node = NULL;
	uint32_t cmdlist_idxs_temp;
	int i;
	int id = 0;
	int32_t event_list;

	hisi_check_and_return(!hisifd, -EINVAL, ERR, "hisifd is NULL!\n");

	cmdlist_idxs_temp = cmdlist_idxs;

	for (i = 0; i < HISI_DSS_CMDLIST_MAX; i++) {
		if ((cmdlist_idxs_temp & 0x1) == 0x1) {
			node = hisi_cmdlist_get_free_node(hisifd->cmdlist_data->cmdlist_nodes_temp[i], &id);
			hisi_check_and_return(!node, -EINVAL, ERR, "failed to hisi_get_free_cmdnode!\n");

			/* fill the header and item info */
			node->list_header->flag.bits.id = id;
			node->list_header->flag.bits.pending = flag.bits.pending ? 0x1 : 0x0;

			event_list = hisi_get_eventlist_by_poolidx(i);

			set_list_header_bits_event_list(node, remove, i, event_list);

			node->list_header->flag.bits.task_end = flag.bits.task_end ? 0x1 : 0x0;
			node->list_header->flag.bits.last = flag.bits.last ? 0x1 : 0x0;

			node->list_header->flag.bits.valid_flag = CMDLIST_NODE_VALID;
			node->list_header->flag.bits.exec = 0x1;
			node->list_header->list_addr = node->item_phys;
			node->list_header->next_list = node->header_phys;

			hisi_cmdlist_addr_check(hisifd, &(node->list_header->list_addr));
			hisi_cmdlist_addr_check(hisifd, &(node->list_header->next_list));

			node->is_used = 1;
			node->node_type = CMDLIST_NODE_FRAME;
			node->item_flag = 0;
			node->reserved = 0;

			/* add this nop to list */
			list_add_tail(&(node->list_node), &(hisifd->cmdlist_data->cmdlist_head_temp[i]));

			if (node->list_node.prev != &(hisifd->cmdlist_data->cmdlist_head_temp[i])) {
				struct dss_cmdlist_node *pre_node;

				pre_node = list_entry(node->list_node.prev, struct dss_cmdlist_node, list_node);
				pre_node->list_header->next_list = node->header_phys;
				hisi_cmdlist_addr_check(hisifd, &(pre_node->list_header->next_list));
				pre_node->reserved = 0x0;
				if (g_debug_ovl_cmdlist)
					HISI_FB_DEBUG("i = %d, next_list = 0x%x\n",  i, (uint32_t)node->header_phys);
			}
		}

		cmdlist_idxs_temp  = cmdlist_idxs_temp >> 1;
	}

	return 0;
}

int hisi_cmdlist_del_all_node(struct list_head *cmdlist_heads)
{
	struct dss_cmdlist_node *node = NULL;
	struct dss_cmdlist_node *_node_ = NULL;

	if (!cmdlist_heads) {
		HISI_FB_ERR("cmdlist_heads is NULL\n");
		return -EINVAL;
	}

	list_for_each_entry_safe(node, _node_, cmdlist_heads, list_node) {
		if (node->reserved != 0x1) {
			list_del(&node->list_node);

			node->list_header->flag.bits.exec = 0;
			node->list_header->flag.bits.last = 1;
			node->list_header->flag.bits.nop = 0;
			node->list_header->flag.bits.interrupt = 0;
			node->list_header->flag.bits.pending = 0;
			node->list_header->flag.bits.id = 0;
			node->list_header->flag.bits.event_list = 0;
			node->list_header->flag.bits.qos = 0;
			node->list_header->flag.bits.task_end = 0;
			node->list_header->flag.bits.valid_flag = 0;
			node->list_header->total_items.ul32 = 0;

			memset(node->list_item, 0, CMDLIST_ITEM_LEN);

			node->item_index = 0;
			node->item_flag = 0;
			node->node_type = CMDLIST_NODE_NONE;
			node->is_used = 0;
		}
	}

	return 0;
}

int hisi_cmdlist_check_cmdlist_state(struct hisi_fb_data_type *hisifd, uint32_t cmdlist_idxs)
{
	char __iomem *cmdlist_base = NULL;
	uint32_t offset;
	uint32_t temp;
	uint32_t cmdlist_idxs_temp;
	int i;
	int delay_count = 0;
	bool is_timeout = true;
	int ret = 0;
	uint32_t cmdlist_chnidx;

	hisi_check_and_return(!hisifd, -EINVAL, ERR, "hisifd is NULL\n");

	if (hisifd->index == MEDIACOMMON_PANEL_IDX)
		cmdlist_base = hisifd->media_common_base + DSS_CMDLIST_OFFSET;
	else
		cmdlist_base = hisifd->dss_base + DSS_CMDLIST_OFFSET;


	offset = 0x40;
	cmdlist_idxs_temp = cmdlist_idxs;

	for (i = 0; i < HISI_DSS_CMDLIST_MAX; i++) {
		if ((cmdlist_idxs_temp & 0x1) == 0x1) {
			cmdlist_chnidx = hisi_get_cmdlist_chnidx(i);
			while (1) {
				temp = inp32(cmdlist_base + CMDLIST_CH0_STATUS + cmdlist_chnidx * offset);  /*lint !e529*/
#ifdef CONFIG_HISI_DSS_CMDLIST_LAST_USED
				if (((temp & 0xF) == 0x0) || delay_count > 5000) {  /* 5ms */
#else
				if (((temp & 0xF) == 0x7) || delay_count > 5000) {  /* 5ms */
#endif
					is_timeout = (delay_count > 5000) ? true : false;  /* 5ms */
					delay_count = 0;
					break;
				} else {
					udelay(1);  /* 1us */
					++delay_count;
				}
			}

			if (is_timeout) {
#ifdef CONFIG_HISI_DSS_CMDLIST_LAST_USED
				HISI_FB_ERR("cmdlist_ch%d not in idle state,ints=0x%x !\n", cmdlist_chnidx, temp);
#else
				HISI_FB_ERR("cmdlist_ch%d not in pending state,ints=0x%x !\n", cmdlist_chnidx, temp);
#endif
				ret = -1;
			}
		}

		cmdlist_idxs_temp = (cmdlist_idxs_temp >> 1);
	}

	return ret;
}

/*
 * stop the pending state for one new frame
 * if the current cmdlist status is e_status_wait.
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winteger-overflow"

static void hisi_cmdlist_chn_state_check(struct hisi_fb_data_type *hisifd,
	uint32_t cmdlist_idxs, uint32_t poolindex, const char __iomem *cmdlist_base)
{
	uint32_t temp;
	int delay_count = 0;
	bool is_timeout = true;
	uint32_t cmdlist_chnidx;

	cmdlist_chnidx = hisi_get_cmdlist_chnidx(poolindex);
	while (1) {
		temp = inp32(cmdlist_base + CMDLIST_CH0_STATUS + cmdlist_chnidx * 0x40); /*lint !e529*/
#ifdef CONFIG_HISI_DSS_CMDLIST_LAST_USED
		if (((temp & 0xF) == 0x0) || delay_count > 500) {  /* 500us */
#else
		if (((temp & 0xF) == 0x7) || delay_count > 500) {  /* 500us */
#endif
			is_timeout = (delay_count > 500) ? true : false;  /* 500us */
			break;
		}

		udelay(1);  /* 1us */
		++delay_count;
	}

	if (is_timeout) {
#ifndef CONFIG_HISI_DSS_CMDLIST_LAST_USED
		HISI_FB_ERR("cmdlist_ch%d not in pending state,ints=0x%x !\n", cmdlist_chnidx, temp);
#else
		HISI_FB_ERR("cmdlist_ch%d not in idle state,ints=0x%x !\n", cmdlist_chnidx, temp);
#endif

		if (g_debug_ovl_cmdlist)
			hisi_cmdlist_dump_all_node(hisifd, NULL, cmdlist_idxs);

		if (g_debug_ovl_offline_composer_hold)
			mdelay(HISI_DSS_COMPOSER_HOLD_TIME);
	}
}

int hisi_cmdlist_exec(struct hisi_fb_data_type *hisifd, uint32_t cmdlist_idxs)
{
	char __iomem *cmdlist_base = NULL;
	uint32_t cmdlist_idxs_temp = 0;
	int i;
#ifndef CONFIG_HISI_DSS_CMDLIST_LAST_USED
	uint32_t cmdlist_chnidxs;
#endif

	hisi_check_and_return(!hisifd, -EINVAL, ERR, "hisifd is NULL!\n");

	cmdlist_base = (hisifd->index == MEDIACOMMON_PANEL_IDX) ?
		(hisifd->media_common_base + DSS_CMDLIST_OFFSET) : (hisifd->dss_base + DSS_CMDLIST_OFFSET);

	cmdlist_idxs_temp = cmdlist_idxs;

	for (i = 0; i < HISI_DSS_CMDLIST_MAX; i++) {
		if ((cmdlist_idxs_temp & 0x1) == 0x1)
			hisi_cmdlist_chn_state_check(hisifd, cmdlist_idxs, i, cmdlist_base);

		cmdlist_idxs_temp = (cmdlist_idxs_temp >> 1);
	}

#ifndef CONFIG_HISI_DSS_CMDLIST_LAST_USED
	/* task continue */
	cmdlist_chnidxs = hisi_get_cmdlist_chnidxs_by_poolidxs(cmdlist_idxs);
	outp32(cmdlist_base + CMDLIST_TASK_CONTINUE, cmdlist_chnidxs);
	if (g_debug_set_reg_val)
		HISI_FB_INFO("writel: [0x%x] = 0x%x\n",
			(uint32_t)(cmdlist_base + CMDLIST_TASK_CONTINUE), cmdlist_chnidxs);
#endif

	return 0;
}

#pragma GCC diagnostic pop
/*
 * start cmdlist.
 * it will set cmdlist into pending state.
 */
extern uint32_t g_dss_module_ovl_base[DSS_MCTL_IDX_MAX][MODULE_OVL_MAX];

static uint32_t get_hisi_cmdlist_node_addr(struct hisi_fb_data_type *hisifd, int mctl_idx,
	uint32_t wb_compose_type, uint32_t cmdlist_index)
{
	struct dss_cmdlist_node *cmdlist_node = NULL;

	if (mctl_idx >= DSS_MCTL2)
		cmdlist_node = list_first_entry(
			&(hisifd->cmdlist_data_tmp[wb_compose_type]->cmdlist_head_temp[cmdlist_index]),
			struct dss_cmdlist_node, list_node);
	else
		cmdlist_node = list_first_entry(&(hisifd->cmdlist_data->cmdlist_head_temp[cmdlist_index]),
			struct dss_cmdlist_node, list_node);

	return cmdlist_node->header_phys;
}

static int config_hisi_cmdlist_init(struct hisi_fb_data_type *hisifd, int mctl_idx,
	uint32_t wb_compose_type, uint32_t *cmdlist_idxs_temp, uint32_t *temp)
{
	uint32_t i;
	uint32_t cmdlist_chnidx;
	int status_temp = 0;
	uint32_t ints_temp = 0;
	uint32_t list_addr = 0;
	uint32_t offset = 0;
	char __iomem *cmdlist_base = NULL;

	offset = 0x40; /* Indicates the offset value of a register */
	cmdlist_base = hisifd->dss_base + DSS_CMDLIST_OFFSET;
	for (i = 0; i < HISI_DSS_CMDLIST_MAX; i++) {
		if ((*cmdlist_idxs_temp & 0x1) == 0x1) {
			cmdlist_chnidx = hisi_get_cmdlist_chnidx(i);
			status_temp =  inp32(cmdlist_base + CMDLIST_CH0_STATUS + cmdlist_chnidx * offset);  /*lint !e529*/
			ints_temp = inp32(cmdlist_base + CMDLIST_CH0_INTS + cmdlist_chnidx * offset);  /*lint !e529*/

			list_addr = get_hisi_cmdlist_node_addr(hisifd, mctl_idx, wb_compose_type, i);
			if (g_debug_ovl_cmdlist)
				HISI_FB_INFO("list_addr:0x%x, i=%d, ints_temp=0x%x\n",
					list_addr, cmdlist_chnidx, ints_temp);

			if (hisi_cmdlist_addr_check(hisifd, &list_addr) == false)
				return -EINVAL;

			*temp |= (1 << cmdlist_chnidx);

			outp32(cmdlist_base + CMDLIST_ADDR_MASK_EN, BIT(cmdlist_chnidx));
			if (g_debug_set_reg_val)
				HISI_FB_INFO("writel: [%pK] = 0x%lx\n",
					cmdlist_base + CMDLIST_ADDR_MASK_EN, BIT(cmdlist_chnidx));

			if (mctl_idx <= DSS_MCTL1)
				set_reg(cmdlist_base + CMDLIST_CH0_CTRL + cmdlist_chnidx * offset, 0x1, 1, 6);
			else
				set_reg(cmdlist_base + CMDLIST_CH0_CTRL + cmdlist_chnidx * offset, 0x0, 1, 6);

			set_reg(cmdlist_base + CMDLIST_CH0_STAAD + cmdlist_chnidx * offset, list_addr, 32, 0);

			set_reg(cmdlist_base + CMDLIST_CH0_CTRL + cmdlist_chnidx * offset, 0x1, 1, 0);
			if ((mctl_idx <= DSS_MCTL1) && ((ints_temp & 0x2) == 0x2))
				set_reg(cmdlist_base + CMDLIST_SWRST, 0x1, 1, cmdlist_chnidx);

			if (mctl_idx >= DSS_MCTL2) {
				if ((((uint32_t)status_temp & 0xF) == 0x0) || ((ints_temp & 0x2) == 0x2))
					set_reg(cmdlist_base + CMDLIST_SWRST, 0x1, 1, cmdlist_chnidx);
				else
					HISI_FB_INFO("i=%d, status_temp=0x%x, ints_temp=0x%x\n",
						cmdlist_chnidx, status_temp, ints_temp);
			}
		}

		*cmdlist_idxs_temp = *cmdlist_idxs_temp >> 1;
	}
	return 0;
}

int hisi_cmdlist_config_start(struct hisi_fb_data_type *hisifd, int mctl_idx,
	uint32_t cmdlist_idxs, uint32_t wb_compose_type)
{
	char __iomem *mctl_base = NULL;
	char __iomem *cmdlist_base = NULL;
	uint32_t offset;
	uint32_t cmdlist_idxs_temp;
	uint32_t temp = 0;
	int i;
	uint32_t cmdlist_chnidx;

	hisi_check_and_return(!hisifd, -EINVAL, ERR, "hisifd is NULL!\n");
	hisi_check_and_return(((mctl_idx < 0) || (mctl_idx >= DSS_MCTL_IDX_MAX)), -EINVAL, ERR,
		"mctl_idx=%d is invalid.", mctl_idx);

	mctl_base = hisifd->dss_base + g_dss_module_ovl_base[mctl_idx][MODULE_MCTL_BASE];
	cmdlist_base = hisifd->dss_base + DSS_CMDLIST_OFFSET;
	offset = 0x40;
	cmdlist_idxs_temp = cmdlist_idxs;

	dsb(sy);
	if (config_hisi_cmdlist_init(hisifd, mctl_idx, wb_compose_type, &cmdlist_idxs_temp, &temp) != 0)
		return -EINVAL;

	outp32(cmdlist_base + CMDLIST_ADDR_MASK_DIS, temp);
	if (g_debug_set_reg_val)
		HISI_FB_INFO("writel: [%pK] = 0x%x\n", cmdlist_base + CMDLIST_ADDR_MASK_DIS, temp);

	cmdlist_idxs_temp = cmdlist_idxs;
	for (i = 0; i < HISI_DSS_CMDLIST_MAX; i++) {
		if ((cmdlist_idxs_temp & 0x1) == 0x1) {
			cmdlist_chnidx = hisi_get_cmdlist_chnidx(i);
			set_reg(cmdlist_base + CMDLIST_CH0_CTRL + cmdlist_chnidx * offset, mctl_idx, 3, 2);
		}

		cmdlist_idxs_temp = cmdlist_idxs_temp >> 1;
	}

	if (mctl_idx >= DSS_MCTL2) {
		set_reg(mctl_base + MCTL_CTL_ST_SEL, 0x1, 1, 0);
		set_reg(mctl_base + MCTL_CTL_SW_ST, 0x1, 1, 0);
	}

	return 0;
}

static void set_hisi_cmdlist_config_mif(struct hisi_fb_data_type *hisifd,
	uint32_t *cmdlist_idxs_temp, uint8_t flag)
{
	int i;
	char __iomem *tmp_base = NULL;

	for (i = 0; i < 4; i++) {  /* mif_sub_ch_nums = 4 */
		if ((*cmdlist_idxs_temp & 0x1) == 0x1) {
			tmp_base = hisifd->dss_module.mif_ch_base[i];
			if (tmp_base)
				set_reg(tmp_base + MIF_CTRL0, flag, 1, 0);
		}

		*cmdlist_idxs_temp = *cmdlist_idxs_temp >> 1;
	}
}

void hisi_cmdlist_config_mif_reset(struct hisi_fb_data_type *hisifd,
	dss_overlay_t *pov_req, uint32_t cmdlist_idxs, int mctl_idx)
{
	char __iomem *dss_base = NULL;
	uint32_t cmdlist_idxs_temp;
	int delay_count = 0;
	bool is_timeout = true;
	int i;
	int j = 0;
	uint32_t tmp = 0;
	int mif_nums_max = 0;

	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is NULL!\n");
	hisi_check_and_no_retval(!pov_req, ERR, "pov_req is NULL!\n");

	dss_base = hisifd->dss_base;

	mif_nums_max = (mctl_idx <= DSS_MCTL1) ? DSS_WCHN_W0 : DSS_CHN_MAX;

	if (mctl_idx != DSS_MCTL5) {
		/* check mif chn status & mif ctrl0: chn disable */
		cmdlist_idxs_temp = cmdlist_idxs;
		for (i = 0; i < mif_nums_max; i++) {
			if ((cmdlist_idxs_temp & 0x1) == 0x1) {
				is_timeout = false;

				while (1) {
					for (j = 1; j <= 4; j++)  /* mif_sub_ch_nums = 4 */
						tmp |= (uint32_t)inp32(dss_base + DSS_MIF_OFFSET +  /*lint -e529*/
							MIF_STAT1 + 0x10 * (i * 4 + j));

					if (((tmp & 0x1f) == 0x0) || delay_count > 500) {  /* 5ms */
						is_timeout = (delay_count > 500) ? true : false;
						delay_count = 0;
						break;
					} else {
						udelay(10);  /* 10us */
						++delay_count;
					}
				}

				if (is_timeout)
					HISI_FB_ERR("mif_ch%d MIF_STAT1=0x%x !\n", i, tmp);
			}

			cmdlist_idxs_temp = cmdlist_idxs_temp >> 1;
		}

		cmdlist_idxs_temp = cmdlist_idxs;
		set_hisi_cmdlist_config_mif(hisifd, &cmdlist_idxs_temp, 0x0);
		mdelay(5);  /* 5ms */
		/* mif ctrl0: chn enable */
		set_hisi_cmdlist_config_mif(hisifd, &cmdlist_idxs_temp, 0x1);
	}
}

static bool hisifb_mctl_clear_ack_timeout(const char __iomem *mctl_base)
{
	uint32_t mctl_status;
	int delay_count = 0;
	bool is_timeout = false;

	while (1) {
		mctl_status = inp32(mctl_base + MCTL_CTL_STATUS);
		if (((mctl_status & 0x10) == 0) || (delay_count > 500)) {  /* 500us */
			is_timeout = (delay_count > 500) ? true : false;
			break;
		} else {
			udelay(1);  /* 1us */
			++delay_count;
		}
	}

	return is_timeout;
}

#ifdef SUPPORT_DSI_VER_2_0
/*lint -e529*/
static void hisifb_mctl_sw_clr_interrupt(struct hisi_fb_data_type *hisifd)
{
	char __iomem *ldi_base = NULL;
	uint32_t tmp;
	uint32_t isr_s1;
	uint32_t isr_s2;

	if (hisifd->index == PRIMARY_PANEL_IDX) {
		isr_s1 = inp32(hisifd->dss_base + GLB_CPU_PDP_INTS);
		isr_s2 = inp32(hisifd->mipi_dsi0_base + MIPI_LDI_CPU_ITF_INTS);
		outp32(hisifd->mipi_dsi0_base + MIPI_LDI_CPU_ITF_INTS, isr_s2);
		if (is_dual_mipi_panel(hisifd)) {
			isr_s2 = inp32(hisifd->mipi_dsi1_base + MIPI_LDI_CPU_ITF_INTS);
			outp32(hisifd->mipi_dsi1_base + MIPI_LDI_CPU_ITF_INTS, isr_s2);
		}
		outp32(hisifd->dss_base + GLB_CPU_PDP_INTS, isr_s1);
		tmp = inp32(hisifd->mipi_dsi0_base + MIPI_LDI_CPU_ITF_INT_MSK);
		outp32(hisifd->mipi_dsi0_base + MIPI_LDI_CPU_ITF_INT_MSK, tmp);
		if (is_dual_mipi_panel(hisifd)) {
			tmp = inp32(hisifd->mipi_dsi1_base + MIPI_LDI_CPU_ITF_INT_MSK);
			outp32(hisifd->mipi_dsi1_base + MIPI_LDI_CPU_ITF_INT_MSK, tmp);
		}
	} else if (hisifd->index == EXTERNAL_PANEL_IDX) {
		isr_s1 = inp32(hisifd->dss_base + GLB_CPU_SDP_INTS);
		if (is_dp_panel(hisifd)) {
			ldi_base = hisifd->dss_base + DSS_LDI_DP_OFFSET;
			isr_s2 = inp32(ldi_base + LDI_CPU_ITF_INTS);
			outp32(ldi_base + LDI_CPU_ITF_INTS, isr_s2);
			outp32(hisifd->dss_base + GLB_CPU_SDP_INTS, isr_s1);
			tmp = inp32(ldi_base + LDI_CPU_ITF_INT_MSK);
			outp32(ldi_base + LDI_CPU_ITF_INT_MSK, tmp);

			ldi_base = hisifd->dss_base + DSS_LDI_DP1_OFFSET;
			isr_s2 = inp32(ldi_base + LDI_CPU_ITF_INTS);
			outp32(ldi_base + LDI_CPU_ITF_INTS, isr_s2);
			outp32(hisifd->dss_base + GLB_CPU_SDP_INTS, isr_s1);
			tmp = inp32(ldi_base + LDI_CPU_ITF_INT_MSK);
			outp32(ldi_base + LDI_CPU_ITF_INT_MSK, tmp);
		} else {
			ldi_base = hisifd->mipi_dsi1_base;
			isr_s2 = inp32(ldi_base + MIPI_LDI_CPU_ITF_INTS);
			outp32(ldi_base + MIPI_LDI_CPU_ITF_INTS, isr_s2);
			outp32(hisifd->dss_base + GLB_CPU_SDP_INTS, isr_s1);
			tmp = inp32(ldi_base + MIPI_LDI_CPU_ITF_INT_MSK);
			outp32(ldi_base + MIPI_LDI_CPU_ITF_INT_MSK, tmp);
		}
	}
}

static void hisifb_mctl_sw_clr(struct hisi_fb_data_type *hisifd,
	dss_overlay_t *pov_req, uint32_t cmdlist_idxs)
{
	int mctl_idx;
	uint32_t i = 0;
	char __iomem *mctl_base = NULL;

	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is NULL!\n");
	hisi_check_and_no_retval(!pov_req, ERR, "pov_req is NULL!\n");

	set_reg(hisifd->dss_base + DSS_CMDLIST_OFFSET + CMDLIST_SWRST, cmdlist_idxs, 32, 0);

	mctl_idx = pov_req->ovl_idx;
	hisi_check_and_no_retval(((mctl_idx < 0) || (mctl_idx >= DSS_MCTL_IDX_MAX)), ERR,
		"mctl_idx=%d is invalid.\n", mctl_idx);
	mctl_base = hisifd->dss_module.mctl_base[mctl_idx];
	if (mctl_base) {
		set_reg(mctl_base + MCTL_CTL_CLEAR, 0x1, 1, 0);
		if (hisifb_mctl_clear_ack_timeout(mctl_base)) {
			HISI_FB_ERR("clear_ack_timeout, mctl_status=0x%x!\n", inp32(mctl_base + MCTL_CTL_STATUS));
			for (i = 0; i < DSS_CHN_MAX_DEFINE; i++) {
				if (g_dss_module_base[i][MODULE_DMA] == 0)
					continue;
				HISI_FB_ERR("chn%d: [DMA_BUF_DBG0]=0x%x [DMA_BUF_DBG1]=0x%x MOD%d_STATUS=0x%x!\n", i,
					inp32(hisifd->dss_base + g_dss_module_base[i][MODULE_DMA] + DMA_BUF_DBG0),
					inp32(hisifd->dss_base + g_dss_module_base[i][MODULE_DMA] + DMA_BUF_DBG1),
					i, inp32(hisifd->dss_base + DSS_MCTRL_SYS_OFFSET + MCTL_MOD0_STATUS + i * 0x4));
			}
		}
	}

	if (is_mipi_cmd_panel(hisifd))
		ldi_data_gate(hisifd, false);

	if (is_dp_panel(hisifd))
		enable_ldi(hisifd);

	hisifb_mctl_sw_clr_interrupt(hisifd);
}

#endif
void hisi_mctl_ctl_clear(struct hisi_fb_data_type *hisifd, int mctl_idx)
{
	char __iomem *tmp_base = NULL;

	if (!hisifd) {
		HISI_FB_ERR("hisifd is NULL point!\n");
		return;
	}

	if ((mctl_idx < 0) || (mctl_idx >= DSS_MCTL_IDX_MAX)) {
		HISI_FB_ERR("mctl_idx=%d is invalid!\n", mctl_idx);
		return;
	}

	tmp_base = hisifd->dss_module.mctl_base[mctl_idx];
	if (tmp_base) {
		set_reg(tmp_base + MCTL_CTL_CLEAR, 0x1, 1, 0);
	}
}

#ifdef SUPPORT_CLR_CMDLIST_BY_HARDWARE
void hisi_cmdlist_config_reset(struct hisi_fb_data_type *hisifd,
	dss_overlay_t *pov_req, uint32_t cmdlist_idxs)
{
	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is NULL point!\n");
	HISI_FB_DEBUG("fb%d, +.\n", hisifd->index);
	hisi_check_and_no_retval(!pov_req, ERR, "pov_req is NULL point!\n");
	hisifb_mctl_sw_clr(hisifd, pov_req, cmdlist_idxs);
}
#endif


int hisi_cmdlist_config_stop(struct hisi_fb_data_type *hisifd, uint32_t cmdlist_pre_idxs)
{
	dss_overlay_t *pov_req = NULL;
	char __iomem *cmdlist_base = NULL;
	uint32_t i;
	uint32_t tmp;
	uint32_t offset;
	uint32_t cmdlist_chnidx;
	int ret = 0;
	union cmd_flag flag = {
		.bits.pending = 0,
		.bits.task_end = 1,
		.bits.last = 1,
	};

	if (!hisifd) {
		HISI_FB_ERR("hisifd is NULL\n");
		return -EINVAL;
	}

	pov_req = &(hisifd->ov_req);

	if ((pov_req->ovl_idx < 0) || pov_req->ovl_idx >= DSS_OVL_IDX_MAX) {
		HISI_FB_ERR("fb%d, invalid ovl_idx=%d!\n", hisifd->index, pov_req->ovl_idx);
		goto err_return;
	}

	cmdlist_base = hisifd->dss_base + DSS_CMDLIST_OFFSET;
	offset = 0x40;
	/* remove prev chn cmdlist */
	ret = hisi_cmdlist_add_new_node(hisifd, cmdlist_pre_idxs, flag, 1);
	if (ret != 0) {
		HISI_FB_ERR("fb%d, hisi_cmdlist_add_new_node err:%d\n", hisifd->index, ret);
		goto err_return;
	}

	for (i = 0; i < DSS_WCHN_W0; i++) {
		hisifd->cmdlist_idx = hisi_get_cmdlist_idx_by_chnidx(i);
		tmp = (0x1 << (uint32_t)(hisifd->cmdlist_idx));

		if ((cmdlist_pre_idxs & tmp) == tmp) {
			hisifd->set_reg(hisifd, hisifd->dss_module.mctl_base[pov_req->ovl_idx] +
				MCTL_CTL_MUTEX_RCH0 + i * 0x4, 0, 32, 0);
			cmdlist_chnidx = hisi_get_cmdlist_chnidx(hisifd->cmdlist_idx);
			hisifd->set_reg(hisifd, cmdlist_base + CMDLIST_CH0_CTRL + cmdlist_chnidx * offset, 0x6, 3, 2);
		}
	}

	if ((cmdlist_pre_idxs & (0x1 << DSS_CMDLIST_V2)) == (0x1 << DSS_CMDLIST_V2)) {
		hisifd->cmdlist_idx = DSS_CMDLIST_V2;
		hisifd->set_reg(hisifd, hisifd->dss_module.mctl_base[pov_req->ovl_idx] +
			MCTL_CTL_MUTEX_RCH8, 0, 32, 0);
		cmdlist_chnidx = hisi_get_cmdlist_chnidx(hisifd->cmdlist_idx);
		hisifd->set_reg(hisifd, cmdlist_base + CMDLIST_CH0_CTRL + cmdlist_chnidx * offset, 0x6, 3, 2);
	}

	return 0;

err_return:
	return ret;
}

void hisi_dss_cmdlist_qos_on(struct hisi_fb_data_type *hisifd)
{
	char __iomem *cmdlist_base = NULL;

	if (!hisifd) {
		HISI_FB_ERR("hisifd is NULL\n");
		return;
	}

	cmdlist_base = hisifd->dss_base + DSS_CMDLIST_OFFSET;
	set_reg(cmdlist_base + CMDLIST_CTRL, 0x3, 2, 4);
}

void hisi_dump_cmdlist_node_items(struct cmd_item *item, uint32_t count)
{
	uint32_t index;
	uint32_t addr;

	for (index = 0; index < count; index++) {
		addr = item[index].reg_addr.bits.add0;
		addr = addr & CMDLIST_ADDR_OFFSET;
		addr = addr << 2;
		HISI_FB_INFO("set addr:0x%x value:0x%x add1:0x%x value:0x%x add2:0x%x value:0x%x\n",
			addr, item[index].data0,
			item[index].reg_addr.bits.add1 << 2, item[index].data1,
			item[index].reg_addr.bits.add2 << 2, item[index].data2);
	}
}

static void hisi_dump_cmdlist_content(struct list_head *cmdlist_head, char *filename,
	int length_filename, uint32_t addr)
{
	struct dss_cmdlist_node *node = NULL;
	struct dss_cmdlist_node *_node_ = NULL;

	(void)length_filename;
	if (!cmdlist_head) {
		HISI_FB_ERR("cmdlist_head is NULL\n");
		return;
	}
	if (!filename) {
		HISI_FB_ERR("filename is NULL\n");
		return;
	}

	if (g_dump_cmdlist_content == 0)
		return;

	HISI_FB_INFO("%s\n", filename);

	list_for_each_entry_safe(node, _node_, cmdlist_head, list_node) {
		if (node->header_phys == addr)
			hisifb_save_file(filename, (char *)(node->list_header), CMDLIST_HEADER_LEN);

		if (node->item_phys == addr)
			hisifb_save_file(filename, (char *)(node->list_item), CMDLIST_ITEM_LEN);
	}
}

static void hisi_dump_cmdlist_one_node(struct list_head *cmdlist_head, uint32_t cmdlist_idx)
{
	struct dss_cmdlist_node *node = NULL;
	struct dss_cmdlist_node *_node_ = NULL;
	uint32_t count = 0;
	int i = 0;
	char filename[256] = {0};  /* the patch of dssdump.txt */

	if (!cmdlist_head) {
		HISI_FB_ERR("cmdlist_head is NULL\n");
		return;
	}

	list_for_each_entry_safe(node, _node_, cmdlist_head, list_node) {
		if (node->node_type == CMDLIST_NODE_NOP)
			HISI_FB_INFO("node type = NOP node\n");
		else if (node->node_type == CMDLIST_NODE_FRAME)
			HISI_FB_INFO("node type = Frame node\n");

		HISI_FB_INFO("\t qos  | flag | pending | tast_end | last  | event_list | list_addr | "
			"next_list  | count | id | is_used | reserved | cmdlist_idx\n");
		HISI_FB_INFO("\t------+---------+------------+------------+------------+------------\n");
		HISI_FB_INFO("\t 0x%2x | 0x%2x |0x%6x | 0x%5x | 0x%3x | 0x%8x | 0x%8x | 0x%8x | "
			"0x%3x | 0x%2x | 0x%2x | 0x%2x | 0x%2x\n",
			node->list_header->flag.bits.qos, node->list_header->flag.bits.valid_flag,
			node->list_header->flag.bits.pending, node->list_header->flag.bits.task_end,
			node->list_header->flag.bits.last, node->list_header->flag.bits.event_list,
			node->list_header->list_addr, node->list_header->next_list,
			node->list_header->total_items.bits.count, node->list_header->flag.bits.id,
			node->is_used, node->reserved, cmdlist_idx);

		if (i == 0) {
			snprintf(filename, 256, "/data/dssdump/list_start_0x%x.txt", (uint32_t)node->header_phys);
			hisi_dump_cmdlist_content(cmdlist_head, filename, ARRAY_SIZE(filename), node->header_phys);
		}

		count = node->list_header->total_items.bits.count;
		hisi_dump_cmdlist_node_items(node->list_item, count);

		i++;
	}
}

int hisi_cmdlist_dump_all_node(struct hisi_fb_data_type *hisifd, dss_overlay_t *pov_req,
	uint32_t cmdlist_idxs)
{
	int i;
	uint32_t cmdlist_idxs_temp;
	uint32_t wb_compose_type = 0;

	if (!hisifd) {
		HISI_FB_ERR("hisifd is NULL\n");
		return -EINVAL;
	}

	if (pov_req) {
		if (pov_req->wb_enable)
			wb_compose_type = pov_req->wb_compose_type;
	}

	cmdlist_idxs_temp = cmdlist_idxs;
	for (i = 0; i < HISI_DSS_CMDLIST_MAX; i++) {
		if (0x1 == (cmdlist_idxs_temp & 0x1)) {
			if (pov_req && pov_req->wb_enable) {
				if (hisifd->index == MEDIACOMMON_PANEL_IDX)
					hisi_dump_cmdlist_one_node(
						&(hisifd->media_common_cmdlist_data->cmdlist_head_temp[i]), i);
				else
					hisi_dump_cmdlist_one_node(
						&(hisifd->cmdlist_data_tmp[wb_compose_type]->cmdlist_head_temp[i]), i);
			} else {
				hisi_dump_cmdlist_one_node(&(hisifd->cmdlist_data->cmdlist_head_temp[i]), i);
			}
		}

		cmdlist_idxs_temp = cmdlist_idxs_temp >> 1;
	}

	return 0;
}

int hisi_cmdlist_del_node(struct hisi_fb_data_type *hisifd, dss_overlay_t *pov_req,
	uint32_t cmdlist_idxs)
{
	int i;
	uint32_t wb_compose_type = 0;
	uint32_t cmdlist_idxs_temp;

	if (!hisifd) {
		HISI_FB_ERR("hisifd is NULL\n");
		return -EINVAL;
	}

	if (pov_req) {
		if (pov_req->wb_enable)
			wb_compose_type = pov_req->wb_compose_type;
	}

	cmdlist_idxs_temp = cmdlist_idxs;
	for (i = 0; i < HISI_DSS_CMDLIST_MAX; i++) {
		if ((cmdlist_idxs_temp & 0x1) == 0x1) {
			if (pov_req && pov_req->wb_enable) {
				if (hisifd->index == MEDIACOMMON_PANEL_IDX)
					hisi_cmdlist_del_all_node(
						&(hisifd->media_common_cmdlist_data->cmdlist_head_temp[i]));
				else
					hisi_cmdlist_del_all_node(
						&(hisifd->cmdlist_data_tmp[wb_compose_type]->cmdlist_head_temp[i]));
			} else {
				if (hisifd->cmdlist_data)
					hisi_cmdlist_del_all_node(&(hisifd->cmdlist_data->cmdlist_head_temp[i]));
			}
		}

		cmdlist_idxs_temp = (cmdlist_idxs_temp >> 1);
	}

	return 0;
}

static void hisi_cmdlist_data_free(struct hisi_fb_data_type *hisifd, struct dss_cmdlist_data *cmdlist_data)
{
	int i;
	int j;

	if (!hisifd) {
		HISI_FB_ERR("hisifd is NULL\n");
		return;
	}
	if (!cmdlist_data) {
		HISI_FB_ERR("cmdlist_data is NULL\n");
		return;
	}

	for (i = 0; i < HISI_DSS_CMDLIST_MAX; i++) {
		for (j = 0; j < HISI_DSS_CMDLIST_NODE_MAX; j++) {
			hisi_cmdlist_node_free(hisifd, cmdlist_data->cmdlist_nodes_temp[i][j]);
			cmdlist_data->cmdlist_nodes_temp[i][j] = NULL;
		}
	}

	kfree(cmdlist_data);
	cmdlist_data = NULL;
}

static struct dss_cmdlist_data *hisi_cmdlist_data_alloc(struct hisi_fb_data_type *hisifd)
{
	int i;
	int j;
	struct dss_cmdlist_data *cmdlist_data = NULL;
	size_t header_len = roundup(CMDLIST_HEADER_LEN, PAGE_SIZE);
	size_t item_len = roundup(CMDLIST_ITEM_LEN, PAGE_SIZE);

	if (!hisifd) {
		HISI_FB_ERR("hisifd is NULL\n");
		return NULL;
	}

	cmdlist_data = (struct dss_cmdlist_data *)kmalloc(sizeof(struct dss_cmdlist_data), GFP_ATOMIC);
	if (cmdlist_data) {
		memset(cmdlist_data, 0, sizeof(struct dss_cmdlist_data));
	} else {
		HISI_FB_ERR("failed to kmalloc cmdlist_data!\n");
		return NULL;
	}

	for (i = 0; i < HISI_DSS_CMDLIST_MAX; i++) {
		INIT_LIST_HEAD(&(cmdlist_data->cmdlist_head_temp[i]));

		for (j = 0; j < HISI_DSS_CMDLIST_NODE_MAX; j++) {
			cmdlist_data->cmdlist_nodes_temp[i][j] = hisi_cmdlist_node_alloc(hisifd, header_len, item_len);
			if (cmdlist_data->cmdlist_nodes_temp[i][j] == NULL) {
				HISI_FB_ERR("failed to hisi_cmdlist_node_alloc!\n");
				hisi_cmdlist_data_free(hisifd, cmdlist_data);
				return NULL;
			}
		}
	}

	return cmdlist_data;
}

static struct dss_cmdlist_info *hisi_cmdlist_info_alloc(struct hisi_fb_data_type *hisifd)
{
	int i;
	struct dss_cmdlist_info *cmdlist_info = NULL;

	if (!hisifd) {
		HISI_FB_ERR("hisifd is NULL\n");
		return NULL;
	}

	cmdlist_info = (struct dss_cmdlist_info *)kmalloc(sizeof(struct dss_cmdlist_info), GFP_ATOMIC);
	if (cmdlist_info) {
		memset(cmdlist_info, 0, sizeof(struct dss_cmdlist_info));
	} else {
		HISI_FB_ERR("failed to kmalloc cmdlist_info!\n");
		return NULL;
	}

	sema_init(&(cmdlist_info->cmdlist_wb_common_sem), 1);

	for (i = 0; i < WB_TYPE_MAX; i++) {
		sema_init(&(cmdlist_info->cmdlist_wb_sem[i]), 1);
		init_waitqueue_head(&(cmdlist_info->cmdlist_wb_wq[i]));
		cmdlist_info->cmdlist_wb_done[i] = 0;
		cmdlist_info->cmdlist_wb_flag[i] = 0;
	}

	return cmdlist_info;
}

static struct dss_media_common_info *hisi_media_common_info_alloc(struct hisi_fb_data_type *hisifd)
{
	struct dss_media_common_info *mdc_info = NULL;

	if (!hisifd)
		HISI_FB_ERR("hisifd is null.\n");

	mdc_info = (struct dss_media_common_info *)kmalloc(sizeof(struct dss_media_common_info), GFP_ATOMIC);
	if (mdc_info) {
		memset(mdc_info, 0, sizeof(struct dss_media_common_info));
	} else {
		HISI_FB_ERR("failed to kmalloc copybit_info!\n");
		return NULL;
	}

	init_waitqueue_head(&(mdc_info->mdc_wq));
	mdc_info->mdc_done = 0;

	return mdc_info;
}

void hisi_cmdlist_data_get_online(struct hisi_fb_data_type *hisifd)
{
	int tmp;

	if (!hisifd) {
		HISI_FB_ERR("hisifd is NULL\n");
		return;
	}

	tmp = hisifd->frame_count % HISI_DSS_CMDLIST_DATA_MAX;
	hisifd->cmdlist_data = hisifd->cmdlist_data_tmp[tmp];
	hisi_cmdlist_del_node(hisifd, NULL, HISI_DSS_CMDLIST_IDXS_MAX);
}

void hisi_cmdlist_data_get_offline(struct hisi_fb_data_type *hisifd, dss_overlay_t *pov_req)
{
	if (!hisifd) {
		HISI_FB_ERR("hisifd is NULL\n");
		return;
	}
	if (!pov_req) {
		HISI_FB_ERR("pov_req is NULL\n");
		return;
	}

	if (pov_req->wb_compose_type == DSS_WB_COMPOSE_COPYBIT)
		hisifd->cmdlist_data = hisifd->cmdlist_data_tmp[1];
	else
		hisifd->cmdlist_data = hisifd->cmdlist_data_tmp[0];

	if (!hisifd->cmdlist_data)
		HISI_FB_ERR("fb%d, cmdlist_data is NULL!\n", hisifd->index);
}

static void set_hisi_cmdlist_pool_size(struct hisi_fb_data_type *hisifd)
{
	int i;
	int j;
	int dss_cmdlist_max = HISI_DSS_CMDLIST_MAX;
	size_t one_cmdlist_pool_size = 0;


	for (i = 0; i < dss_cmdlist_max; i++) {
		for (j = 0; j < HISI_DSS_CMDLIST_NODE_MAX; j++)
			one_cmdlist_pool_size += (roundup(CMDLIST_HEADER_LEN, PAGE_SIZE) +
				roundup(CMDLIST_ITEM_LEN, PAGE_SIZE));
	}

	if (hisifd->index == AUXILIARY_PANEL_IDX) {
		hisifd->sum_cmdlist_pool_size = 2 * one_cmdlist_pool_size;
	} else if (hisifd->index == MEDIACOMMON_PANEL_IDX) {
		hisifd->sum_cmdlist_pool_size = one_cmdlist_pool_size;
	} else if (hisifd->index == EXTERNAL_PANEL_IDX && !hisifd->panel_info.fake_external) {
		hisifd->sum_cmdlist_pool_size = HISI_DSS_CMDLIST_DATA_MAX * one_cmdlist_pool_size;
	} else {
		if (hisifd->index == PRIMARY_PANEL_IDX) {
			for (i = 0; i < HISI_DSS_CMDLIST_DATA_MAX; i++)
				hisifd->sum_cmdlist_pool_size += one_cmdlist_pool_size;
		}
	}
}

static int hisi_cmdlist_pool_init(struct hisi_fb_data_type *hisifd)
{
	int ret = 0;
	size_t tmp = 0;

	hisi_check_and_return(!hisifd, -EINVAL, ERR, "hisifd is NULL!\n");
	hisi_check_and_return(!hisifd->pdev, -EINVAL, ERR, "hisifd is NULL!\n");

	set_hisi_cmdlist_pool_size(hisifd);
	if (hisifd->sum_cmdlist_pool_size == 0)
		return 0;

	hisifd->sum_cmdlist_pool_size = roundup(hisifd->sum_cmdlist_pool_size, PAGE_SIZE);
	if (hisi_dss_alloc_cmdlist_buffer(hisifd)) {
		HISI_FB_ERR("dma alloc coherent cmdlist buffer failed!\n");
		return -ENOMEM;
	}

	HISI_FB_INFO("fb%d, sum_cmdlist_pool_size=%zu, tmp=%zu.\n", hisifd->index, hisifd->sum_cmdlist_pool_size, tmp);

	/* create cmdlist pool */
	hisifd->cmdlist_pool = gen_pool_create(PAGE_SHIFT, -1);
	if (!hisifd->cmdlist_pool) {
		HISI_FB_ERR("fb%d, cmdlist_pool gen_pool_create failed!", hisifd->index);
		ret = -ENOMEM;
		goto err_pool_create;
	}

	if (gen_pool_add_virt(hisifd->cmdlist_pool, (unsigned long)(uintptr_t)hisifd->cmdlist_pool_vir_addr,
			hisifd->cmdlist_pool_phy_addr, hisifd->sum_cmdlist_pool_size, -1) != 0) {
		HISI_FB_ERR("fb%d, cmdlist_pool gen_pool_add failed!\n", hisifd->index);
		goto err_pool_add;
	}

	return 0;

err_pool_add:
	if (hisifd->cmdlist_pool) {
		gen_pool_destroy(hisifd->cmdlist_pool);
		hisifd->cmdlist_pool = NULL;
	}
err_pool_create:
	hisi_dss_free_cmdlist_buffer(hisifd);
	return ret;
}

static void hisi_cmdlist_pool_deinit(struct hisi_fb_data_type *hisifd)
{
	if (!hisifd) {
		HISI_FB_ERR("hisifd is NULL!\n");
		return;
	}

	if (hisifd->cmdlist_pool) {
		gen_pool_destroy(hisifd->cmdlist_pool);
		hisifd->cmdlist_pool = NULL;
	}

	hisi_dss_free_cmdlist_buffer(hisifd);
}

static void deinit_hisi_ov_block_rects(struct hisi_fb_data_type *hisifd)
{
	int i;

	if (!hisifd) {
		HISI_FB_ERR("hisifd is NULL!\n");
		return;
	}

	for (i = 0; i < HISI_DSS_CMDLIST_BLOCK_MAX; i++) {
		if (hisifd->ov_block_rects[i]) {
			kfree(hisifd->ov_block_rects[i]);
			hisifd->ov_block_rects[i] = NULL;
		}
	}
}

int hisi_cmdlist_init(struct hisi_fb_data_type *hisifd)
{
	int ret;
	int i;

	if (!hisifd) {
		HISI_FB_ERR("hisifd is NULL\n");
		return -EINVAL;
	}

	ret = hisi_cmdlist_pool_init(hisifd);
	if (ret != 0) {
		HISI_FB_ERR("fb%d, hisi_cmdlist_pool_init failed!\n", hisifd->index);
		return -EINVAL;
	}

	for (i = 0; i < HISI_DSS_CMDLIST_BLOCK_MAX; i++) {
		hisifd->ov_block_rects[i] = (dss_rect_t *)kmalloc(sizeof(dss_rect_t), GFP_ATOMIC);
		if (!hisifd->ov_block_rects[i]) {
			HISI_FB_ERR("ov_block_rects[%d] failed to alloc!\n", i);
			goto err_deint;
		}
	}

	if (hisifd->index == AUXILIARY_PANEL_IDX) {
		hisifd->cmdlist_data_tmp[0] = hisi_cmdlist_data_alloc(hisifd);
		hisifd->cmdlist_data_tmp[1] = hisi_cmdlist_data_alloc(hisifd);
		hisifd->cmdlist_info = hisi_cmdlist_info_alloc(hisifd);
		if (!hisifd->cmdlist_data_tmp[0] || !hisifd->cmdlist_data_tmp[1] || !hisifd->cmdlist_info)
			goto err_deint;
	} else if (hisifd->index == MEDIACOMMON_PANEL_IDX) {
		hisifd->media_common_cmdlist_data = hisi_cmdlist_data_alloc(hisifd);
		hisifd->media_common_info = hisi_media_common_info_alloc(hisifd);
		if (!hisifd->media_common_cmdlist_data || !hisifd->media_common_info)
			goto err_deint;
	} else {
		if (hisifd->index == PRIMARY_PANEL_IDX ||
			(hisifd->index == EXTERNAL_PANEL_IDX && !hisifd->panel_info.fake_external)) {
			for (i = 0; i < HISI_DSS_CMDLIST_DATA_MAX; i++) {
				hisifd->cmdlist_data_tmp[i] = hisi_cmdlist_data_alloc(hisifd);
				if (!hisifd->cmdlist_data_tmp[i])
					goto err_deint;
			}
		}
	}

	hisifd->cmdlist_data = hisifd->cmdlist_data_tmp[0];
	hisifd->cmdlist_idx = -1;

	return 0;

err_deint:
	hisi_cmdlist_deinit(hisifd);
	return -EINVAL;
}

int hisi_cmdlist_deinit(struct hisi_fb_data_type *hisifd)
{
	int i = 0;

	hisi_check_and_return(!hisifd, -EINVAL, ERR, "hisifd is NULL!\n");

	if (hisifd->index == AUXILIARY_PANEL_IDX) {
		if (hisifd->cmdlist_info) {
			kfree(hisifd->cmdlist_info);
			hisifd->cmdlist_info = NULL;
		}

		if (hisifd->copybit_info) {
			kfree(hisifd->copybit_info);
			hisifd->copybit_info = NULL;
		}

		for (i = 0; i < HISI_DSS_CMDLIST_DATA_MAX; i++) {
			if (hisifd->cmdlist_data_tmp[i]) {
				hisi_cmdlist_data_free(hisifd, hisifd->cmdlist_data_tmp[i]);
				hisifd->cmdlist_data_tmp[i] = NULL;
			}
		}
	} else if (hisifd->index == MEDIACOMMON_PANEL_IDX) {
		if (hisifd->media_common_cmdlist_data) {
			kfree(hisifd->media_common_cmdlist_data);
			hisifd->media_common_cmdlist_data = NULL;
		}

		if (hisifd->media_common_info) {
			kfree(hisifd->media_common_info);
			hisifd->media_common_info = NULL;
		}
	} else {
		if (hisifd->index == PRIMARY_PANEL_IDX ||
			(hisifd->index == EXTERNAL_PANEL_IDX && !hisifd->panel_info.fake_external)) {
			for (i = 0; i < HISI_DSS_CMDLIST_DATA_MAX; i++) {
				if (hisifd->cmdlist_data_tmp[i]) {
					hisi_cmdlist_data_free(hisifd, hisifd->cmdlist_data_tmp[i]);
					hisifd->cmdlist_data_tmp[i] = NULL;
				}
			}
		}
	}

	deinit_hisi_ov_block_rects(hisifd);
	hisi_cmdlist_pool_deinit(hisifd);

	return 0;
}
#pragma GCC diagnostic pop

