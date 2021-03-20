/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Operations Related to cmdlist
 * Create: 2019-12-28
 */
#include "drv_cmdlist.h"
#include <linux/list.h>
#include <linux/workqueue.h>
#include "hal_venc.h"
#include "drv_common.h"
#include "venc_regulator.h"
#include "drv_mem.h"

static struct cmdlist_manager g_cmdlist_manager;
static struct mem_pool *g_cmdlist_pool;

static struct workqueue_struct *g_encode_work_queue;
static struct work_struct g_encode_work;

#ifdef SUPPORT_VENC_FREQ_CHANGE
static HI_U32 g_current_venc_freq = 0;
#endif

static struct cmdlist_node *cmdlist_alloc_node(void)
{
	HI_U64 iova_addr;
	HI_U32 align_size = 0;
	HI_S32 ret = HI_FAILURE;
	struct cmdlist_node *node = NULL;

	ret = OSAL_WAIT_EVENT_TIMEOUT(&g_cmdlist_pool->event,
			!queue_is_empty(g_cmdlist_pool), ENCODE_DONE_TIMEOUT_MS); /*lint !e578 !e666*/
	if (ret != HI_SUCCESS) {
		HI_FATAL_VENC("wait alloc buffer timeout");
		return NULL;
	}

	node = (struct cmdlist_node *)drv_mem_pool_alloc(g_cmdlist_pool, &iova_addr);
	if (node == NULL) {
		HI_FATAL_VENC("alloc cmdlist node memory fail");
		return NULL;
	}

	align_size = ALIGN_UP(sizeof(struct cmdlist_node), CMDLIST_ALIGN_SIZE);
	node->virt_addr = (void*)(uintptr_t)((HI_U64)(uintptr_t)node + align_size);
	node->iova_addr = iova_addr + align_size;

	return node;
}

static void cmdlist_free_node(struct cmdlist_node *node)
{
	HI_U32 offset;
	HI_U32 align_size;
	HI_U64 iova_addr;

	if (node == NULL) {
		HI_FATAL_VENC("cmdlist node is null");
		return;
	}

	align_size = ALIGN_UP(sizeof(struct cmdlist_node), CMDLIST_ALIGN_SIZE);
	offset = (HI_U64)(uintptr_t)node->virt_addr - (HI_U64)(uintptr_t)node;
	if (node->iova_addr < offset) {
		HI_FATAL_VENC("iova_addr addr invalid, free cmdlist node memory fail");
		return;
	}

	iova_addr = node->iova_addr - offset;

	drv_mem_pool_free(g_cmdlist_pool, node, iova_addr);
	/* wake up preprocess: The memory pool has free nodes. */
	venc_drv_osal_give_event(&g_cmdlist_pool->event);
}

static struct cmdlist_head *cmdlist_get_head(HI_U32 type)
{
	HI_U32 i;
	HI_U32 wait_idx = g_cmdlist_manager.wait_idx[type];
	struct cmdlist_head *node = NULL;

	if (wait_idx < CMDLIST_MAX_HEAD_NUM) {
		node = &g_cmdlist_manager.head[wait_idx];
		return node;
	}

	for (i = 0; i < CMDLIST_MAX_HEAD_NUM; i++) {
		if (!g_cmdlist_manager.head[i].is_used) {
			node = &g_cmdlist_manager.head[i];
			node->is_used = HI_TRUE;
			g_cmdlist_manager.wait_idx[type] = i;
			return node;
		}
	}

	return NULL;
}

static void cmdlist_put_head(struct cmdlist_head *head)
{
	if (!list_empty(&head->list))
		HI_FATAL_VENC("current head is not empty");

	INIT_LIST_HEAD(&head->list);
	head->num = 0;
	head->is_used = HI_FALSE;
}

static HI_S32 cmdlist_update_hardware_status(struct encode_info *info)
{
	HI_S32 ret = HI_SUCCESS;
	venc_entry_t *venc = platform_get_drvdata(venc_get_device());

	ret = hi_venc_down_interruptible(&venc->hw_sem);
	if (ret) {
		HI_FATAL_VENC("encode: down interruptible failed");
		return ret;
	}

	if (info->is_reset == HI_TRUE) {
		if (venc_regulator_reset() != HI_SUCCESS) {
			HI_ERR_VENC("reset failed");
			hi_venc_up_interruptible(&venc->hw_sem);
			return HI_FAILURE;
		}
	}

#ifdef SUPPORT_VENC_FREQ_CHANGE
	mutex_lock(&g_venc_freq_mutex);
	if (g_current_venc_freq != g_venc_freq) {
		info->clock_info.is_set_clock = HI_TRUE;
		g_current_venc_freq = g_venc_freq;
	}
	mutex_unlock(&g_venc_freq_mutex);
#endif

	if (info->clock_info.is_set_clock == HI_TRUE) {
		ret = venc_regulator_update(&info->clock_info);
		if (ret != HI_SUCCESS)
			HI_WARN_VENC("update clock info failed");
	}

	hi_venc_up_interruptible(&venc->hw_sem);

	return ret;
}

static void cmdlist_cfg_smmu(HI_S32 core_id, HI_S32 type)
{
	HI_BOOL is_protected = (type == CMDLIST_PROTECT);
	venc_entry_t *venc = platform_get_drvdata(venc_get_device());
	struct venc_context *ctx = &venc->ctx[core_id];

	if (is_protected != ctx->is_protected)
		ctx->first_cfg_flag = HI_TRUE;

	if (ctx->first_cfg_flag) {
		venc_smmu_init(is_protected, core_id);
		ctx->first_cfg_flag = HI_FALSE;
	}

	venc_smmu_cfg(NULL, ctx->reg_base);

	ctx->is_protected = is_protected;
}

static HI_S32 cmdlist_encode(HI_S32 core_id, HI_U32 type)
{
	struct cmdlist_head *head = NULL;
	HI_S32 ret = HI_SUCCESS;
	HI_U32 wait_idx = g_cmdlist_manager.wait_idx[type];
	HI_U32 cur_idx = g_cmdlist_manager.cur_idx;
	venc_entry_t *venc = platform_get_drvdata(venc_get_device());

	mutex_lock(&g_cmdlist_manager.lock);

	if (wait_idx >= CMDLIST_MAX_HEAD_NUM) {
		HI_DBG_VENC("no new node is added to the list");
		goto exit;
	}

	if (cur_idx < CMDLIST_MAX_HEAD_NUM) {
		HI_FATAL_VENC("cur num is %u, core id %u, is_used %u",
			g_cmdlist_manager.head[cur_idx].num,
			core_id, g_cmdlist_manager.head[cur_idx].is_used);
		ret = HI_FAILURE;
		goto exit;
	}

	head = &g_cmdlist_manager.head[wait_idx];
	if (list_empty(&head->list)) {
		HI_FATAL_VENC("current list is empty");
		ret = HI_FAILURE;
		goto exit;
	}

	HI_DBG_VENC("status %u, cur_idx %u, type %u, wait_idx %u",
		venc->ctx[core_id].status, cur_idx, type, wait_idx);

	g_cmdlist_manager.cur_idx = wait_idx;
	g_cmdlist_manager.wait_idx[type] = CMDLIST_MAX_HEAD_NUM;

	cmdlist_cfg_smmu(core_id, type);

	osal_add_timer(&venc->ctx[core_id].timer, INTERRUPT_TIMEOUT_MS);
	/* start cmdlist */
	venc->ctx[core_id].status = VENC_BUSY;
	head->start_time = osal_get_sys_time_in_us();

	HI_DBG_VENC("start encode head num %u, type %u", head->num, type);

	hal_cmdlist_encode(head, venc->ctx[core_id].reg_base);

exit:
	mutex_unlock(&g_cmdlist_manager.lock);

	return ret;
}

static void cmdlist_encode_handle(struct work_struct *work)
{
	HI_S32 ret;
	HI_S32 core_id;
	HI_U32 i;
	venc_entry_t *venc = platform_get_drvdata(venc_get_device());

	ret = hi_venc_down_interruptible(&venc->hw_sem);
	if (ret) {
		HI_FATAL_VENC("encode: down interruptible failed");
		return;
	}

	for (i = 0; i < CMDLIST_BUTT; i++) {
		/* wait cmdlist available */
		core_id = venc_regulator_select_idle_core(&venc->event);
		ret = venc_check_coreid(core_id);
		if (ret != HI_SUCCESS) {
			HI_ERR_VENC("CORE_ERROR:invalid core ID is %d", core_id);
			break;
		}

		ret = cmdlist_encode(core_id, i);
		if (ret != HI_SUCCESS)
			HI_ERR_VENC("start encode failed");
	}

	hi_venc_up_interruptible(&venc->hw_sem);
}

static void cmdlist_process_readback_info(struct cmdlist_head *head,
	HI_U32 *reg_base, U_FUNC_VCPI_INTSTAT int_status)
{
	struct cmdlist_node *cmdlist_node = NULL;
	struct cmdlist_node *tmp_cmdlist_node = NULL;
	struct encode_done_info info = {0};
	vedu_osal_event_t *event = NULL;
	HI_S32 ret;

	if (list_empty(&head->list))
		return;

#ifdef SLICE_INT_EN
	/* only process slice end */
	if (int_status.bits.vcpi_int_vedu_slice_end
		&& !int_status.bits.vcpi_soft_int1
		&& !int_status.bits.vcpi_soft_int0) {
		cmdlist_node = list_first_entry(&head->list, struct cmdlist_node, list);
		(void)memcpy_s(&info.channel_info, sizeof(struct channel_info), &cmdlist_node->channel_info, sizeof(struct channel_info));
		venc_hal_get_slice_reg(&info.stream_info, reg_base);
		ret = push(cmdlist_node->buffer, &info);
		if (ret != HI_SUCCESS)
			HI_ERR_VENC("write buffer fail");
		/* wake up postprocess */
		venc_drv_osal_give_event(&cmdlist_node->buffer->event);
	}
#endif

	/* process softint0/softint1/time out */
	list_for_each_entry_safe(cmdlist_node, tmp_cmdlist_node, &head->list, list) {

		if (!(int_status.bits.vcpi_int_vedu_timeout
			|| int_status.bits.vcpi_soft_int1
			|| hal_cmdlist_is_encode_done((HI_U32 *)cmdlist_node->virt_addr)))
			break;

		(void)memset_s(&info, sizeof(struct encode_done_info), 0, sizeof(struct encode_done_info));
		(void)memcpy_s(&info.channel_info, sizeof(struct channel_info), &cmdlist_node->channel_info, sizeof(struct channel_info));

		if (int_status.bits.vcpi_int_vedu_timeout)
			info.is_timeout = HI_TRUE;
		else
			hal_cmdlist_get_reg(&info.stream_info, (HI_U32 *)cmdlist_node->virt_addr);

		ret = push(cmdlist_node->buffer, &info);
		if (ret != HI_SUCCESS)
			HI_ERR_VENC("write buffer fail");

		list_del(&cmdlist_node->list);
		event = &cmdlist_node->buffer->event;
		cmdlist_free_node(cmdlist_node);
		/* wake up postprocess */
		venc_drv_osal_give_event(event);
	}
}

static HI_S32 cmdlist_init(void)
{
	HI_U32 i;
	HI_S32 ret;
	struct platform_device *venc_dev = venc_get_device();

	g_cmdlist_pool = drv_mem_create_pool(&venc_dev->dev, CMDLIST_BUFFER_SIZE,
						CMDLIST_BUFFER_NUM, MIN_PAGE_ALIGN_SIZE);
	if (g_cmdlist_pool == NULL) {
		HI_FATAL_VENC("create cmdlist pool failed");
		return HI_FAILURE;
	}

	g_encode_work_queue = create_singlethread_workqueue("encode_work");
	if (g_encode_work_queue == NULL) {
		HI_FATAL_VENC("create encode work queue failed");
		drv_mem_destory_pool(g_cmdlist_pool);
		return HI_FAILURE;
	}
	INIT_WORK(&g_encode_work, cmdlist_encode_handle);

	ret = drv_mem_init();
	if (ret != HI_SUCCESS) {
		destroy_workqueue(g_encode_work_queue);
		drv_mem_destory_pool(g_cmdlist_pool);
		return HI_FAILURE;
	}

	for (i = 0; i < CMDLIST_MAX_HEAD_NUM; i++) {
		INIT_LIST_HEAD(&g_cmdlist_manager.head[i].list);
		g_cmdlist_manager.head[i].is_used = HI_FALSE;
		g_cmdlist_manager.head[i].num = 0;
	}

	for (i = 0; i < CMDLIST_BUTT; i++)
		g_cmdlist_manager.wait_idx[i] = CMDLIST_MAX_HEAD_NUM;

	g_cmdlist_manager.cur_idx = CMDLIST_MAX_HEAD_NUM;
	mutex_init(&g_cmdlist_manager.lock);

	return ret;
}

static void cmdlist_deinit(void)
{
	cancel_work(&g_encode_work);
	flush_workqueue(g_encode_work_queue);
	destroy_workqueue(g_encode_work_queue);
	drv_mem_destory_pool(g_cmdlist_pool);
	drv_mem_exit();
}

static HI_S32 cmdlist_prepare_encode(struct encode_info *info, struct venc_fifo_buffer *buffer)
{
	struct cmdlist_head *head = NULL;
	struct cmdlist_node *cur_node = NULL;
	HI_S32 ret = HI_SUCCESS;
	HI_U32 type;

	if (cmdlist_update_hardware_status(info) != HI_SUCCESS)
		return HI_FAILURE;

	mutex_lock(&g_cmdlist_manager.lock);

	type = info->is_protected ? CMDLIST_PROTECT : CMDLIST_NORMAL;

	head = cmdlist_get_head(type);
	if (head == NULL) {
		HI_FATAL_VENC("get instance head fail, current type is %d", type);
		ret = HI_FAILURE;
		goto exit;
	}

	cur_node = cmdlist_alloc_node();
	if (cur_node == NULL) {
		/* If the list contains other nodes, head cannot be directly released. */
		if (list_empty(&head->list)) {
			cmdlist_put_head(head);
			g_cmdlist_manager.wait_idx[type] = CMDLIST_MAX_HEAD_NUM;
		}
		HI_FATAL_VENC("alloc cmdlist node fail");
		ret = HI_FAILURE;
		goto exit;
	}

	cur_node->buffer = buffer;

	(void)memcpy_s(&cur_node->channel_info, sizeof(struct channel_info), &info->channel, sizeof(struct channel_info));

	hal_cmdlist_cfg(head, cur_node, info);

	list_add_tail(&cur_node->list, &head->list);

	head->num++;

	queue_work(g_encode_work_queue, &g_encode_work);

exit:
	mutex_unlock(&g_cmdlist_manager.lock);
	return ret;
}

static void cmdlist_encode_done(struct venc_context *ctx)
{
	U_FUNC_VCPI_INTSTAT int_status;
	struct cmdlist_head *head = NULL;
	S_HEVC_AVC_REGS_TYPE *reg_base = NULL;
	HI_U32 cur_idx = g_cmdlist_manager.cur_idx;
	venc_entry_t *venc = platform_get_drvdata(venc_get_device());
	unsigned long flag;

	reg_base = (S_HEVC_AVC_REGS_TYPE *)ctx->reg_base;
	int_status.u32 = reg_base->FUNC_VCPI_INTSTAT.u32;

	if (!int_status.bits.vcpi_soft_int0
		&& !int_status.bits.vcpi_soft_int1
		&& !int_status.bits.vcpi_int_vedu_slice_end) {
		HI_WARN_VENC("not support isr %x", int_status.u32);
		reg_base->VEDU_VCPI_INTCLR.u32 = int_status.u32;
		return;
	}

	spin_lock_irqsave(&ctx->lock, flag);
	if (ctx->status != VENC_BUSY) {
		spin_unlock_irqrestore(&ctx->lock, flag);
		HI_FATAL_VENC("current core status invalid, core status: %d, isr status %x", ctx->status, int_status.u32);
		reg_base->VEDU_VCPI_INTCLR.u32 = int_status.u32;
		return;
	}

	if (cur_idx >= CMDLIST_MAX_HEAD_NUM) {
		spin_unlock_irqrestore(&ctx->lock, flag);
		HI_FATAL_VENC("not find current head");
		reg_base->VEDU_VCPI_INTCLR.u32 = int_status.u32;
		return;
	}

	head = &g_cmdlist_manager.head[cur_idx];
	cmdlist_process_readback_info(head, ctx->reg_base, int_status);

	/* process softint0 */
	if (!int_status.bits.vcpi_soft_int1
		&& !int_status.bits.vcpi_int_vedu_slice_end) {
		spin_unlock_irqrestore(&ctx->lock, flag);
		/* reset timer */
		osal_add_timer(&ctx->timer, INTERRUPT_TIMEOUT_MS);
		reg_base->VEDU_VCPI_INTCLR.u32 = int_status.u32;
		return;
	}

	/* process softint1 */
	if (int_status.bits.vcpi_soft_int1) {

#ifdef VENC_DEBUG_ENABLE
		if (venc->debug_flag & (1LL << PRINT_ENC_COST_TIME))
			HI_INFO_VENC("cmdlist num %u, hardware cost time %llu us", head->num,
					osal_get_during_time(head->start_time));
#endif
		cmdlist_put_head(head);
		g_cmdlist_manager.cur_idx = CMDLIST_MAX_HEAD_NUM;
		if (osal_del_timer(&ctx->timer, HI_FALSE) != HI_SUCCESS)
			HI_FATAL_VENC("delete timer fail");
		ctx->status = VENC_IDLE;

		/* wake up cmdlist encode work queue */
		venc_drv_osal_give_event(&venc->event);
	}

	reg_base->VEDU_VCPI_INTCLR.u32 = int_status.u32;
	spin_unlock_irqrestore(&ctx->lock, flag);
}

static void cmdlist_encode_timeout(unsigned long core_id)
{
	HI_S32 ret;
	U_FUNC_VCPI_INTSTAT int_status;
	HI_U32 cur_idx = g_cmdlist_manager.cur_idx;
	struct cmdlist_head *head = NULL;
	struct venc_context *ctx = NULL;
	venc_entry_t *venc = platform_get_drvdata(venc_get_device());
	unsigned long flag;

	HI_WARN_VENC("coreid:%d encode timeout", (HI_S32)core_id);

	ret = venc_check_coreid((HI_S32)core_id);
	if (ret != HI_SUCCESS) {
		HI_ERR_VENC("CORE_ERROR:invalid core ID is %d", (HI_S32)core_id);
		return;
	}

	ctx = &venc->ctx[core_id];

	spin_lock_irqsave(&ctx->lock, flag);
	if (ctx->status != VENC_BUSY) {
		spin_unlock_irqrestore(&ctx->lock, flag);
		HI_FATAL_VENC("core %d: current core status invalid, status: %d",
				(HI_S32)core_id, ctx->status);
		return;
	}

	if (cur_idx >= CMDLIST_MAX_HEAD_NUM) {
		spin_unlock_irqrestore(&ctx->lock, flag);
		HI_WARN_VENC("not find current head");
		return;
	}

	head = &g_cmdlist_manager.head[cur_idx];
	int_status.u32 = 0;
	int_status.bits.vcpi_int_vedu_timeout = 1;
	cmdlist_process_readback_info(head, ctx->reg_base, int_status);

	cmdlist_put_head(head);
	g_cmdlist_manager.cur_idx = CMDLIST_MAX_HEAD_NUM;
	ctx->status = VENC_TIME_OUT;

	/* wake up cmdlist encode work queue */
	venc_drv_osal_give_event(&venc->event);
	spin_unlock_irqrestore(&ctx->lock, flag);
}

void cmdlist_init_ops(void)
{
	venc_entry_t *venc = platform_get_drvdata(venc_get_device());

	venc->ops.init = cmdlist_init;
	venc->ops.deinit = cmdlist_deinit;
	venc->ops.encode = cmdlist_prepare_encode;
	venc->ops.encode_done = cmdlist_encode_done;
	venc->ops.encode_timeout = cmdlist_encode_timeout;

	HI_INFO_VENC("config register by cmdlist");
}
