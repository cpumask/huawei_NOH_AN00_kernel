/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Operations Related to cmdlist
 * Create: 2019-12-28
 */
#ifndef __DRV_CMDLIST_H__
#define __DRV_CMDLIST_H__

#include "drv_common.h"

enum cmdlist_type {
	CMDLIST_NORMAL = 0,
	CMDLIST_PROTECT,
	CMDLIST_BUTT
};

/* The start physical address, iova address, and size of the mmap memory must be 4 KB aligned. */
#define MIN_PAGE_ALIGN_SIZE PAGE_SIZE
#define CMDLIST_BUFFER_SIZE (2 * MIN_PAGE_ALIGN_SIZE)
#define CMDLIST_ALIGN_SIZE 128
#define CMDLIST_RESERVE_MEM_SIZE 256
#define CMDLIST_MAX_HEAD_NUM (CMDLIST_BUTT + 1)

/* Records the current location information. */
struct cmdlist_cfg {
	HI_U32* base_addr;
	HI_U64 iova_addr;
	/* Offset of the configuration area relative to the start memory.The span is 4 bytes. */
	HI_U32 cfg_offset;
	HI_U32 cfg_len0;
	HI_U32 cfg_len1;
};

/* The cmdlist memory is divided into three parts:
 1.software usage area
 2.cmdlist data dump area (The length of each frame is the same)
 3.cmdlist configuration area (The length of each frame is different)
   3.1.len0 (Before the venc eop, run the following command)
   3.2.len1 (After the venc eop, run the following command)
*/
struct cmdlist_node {
	struct list_head list;

	struct channel_info channel_info;
	struct venc_fifo_buffer *buffer;

	void *virt_addr; /* cmdlist data dump area virtual address */
	HI_U64 iova_addr;   /* cmdlist data dump area iova address */

	HI_U32 cfg_len;     /* cmdlist configuration area length */

	/* Record the virtual address of the memory
	  where the address and length of the cmdlist configuration area of the previous frame are located.
	  After the current frame is configured, update the previous frame. */
	HI_U32 *pre_cmdlist_addr;
	HI_U32 *pre_len_addr;
};

struct cmdlist_head {
	struct list_head list;
	struct cmdlist_cfg cfg;
	HI_BOOL is_used;
	HI_U64 start_time;
	HI_U32 num;
};

struct cmdlist_manager {
	struct cmdlist_head head[CMDLIST_MAX_HEAD_NUM];
	HI_U32 cur_idx;
	HI_U32 wait_idx[CMDLIST_BUTT];
	struct mutex lock; /* Ensure that the list cannot be modified. */
};

#ifdef SUPPORT_VENC_FREQ_CHANGE
extern HI_U32 g_venc_freq;
extern struct mutex g_venc_freq_mutex;
#endif

void cmdlist_init_ops(void);

#endif
