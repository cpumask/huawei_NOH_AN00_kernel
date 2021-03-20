/* Copyright (c) Hisilicon Technologies Co., Ltd. 2013-2019. All rights reserved.
 * FileName: ion_cpa_heap.c
 * Description: This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation;
 * either version 2 of the License,
 * or (at your option) any later version.
 */

#include <linux/device.h>
#include <linux/dma-mapping.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/freezer.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/moduleparam.h>
#include <linux/of_address.h>
#include <linux/oom.h>
#include <linux/slab.h>
#include <linux/swap.h>
#include <linux/sysctl.h>
#include <linux/workqueue.h>
#include <linux/compaction.h>
#include <linux/hisi/hisi_ion.h>

#include <asm/cacheflush.h>
#include <asm/tlbflush.h>

#include <teek_client_api.h>
#include <teek_client_id.h>
#include <teek_client_constants.h>

#include "ion.h"
#include "hisi_ion_priv.h"

#include "hisi/ion_tee_op.h"

#define CPA_SIZE_320M_PAGE		81920
#define CPA_MAX_KILL_NUM		20
#define CPA_WATER_MARK_RATIO		50
#define CPA_RK_RATIO			5
#define CPA_RK_RATIO_BY_ZRAM		5
#define CPA_ERR_LVL			0
#define CPA_DBG_LVL			1
#define CPA_KP_FREEMEM_MODEL		0
#define CPA_KP_ZRAM_MODEL		1

#define TOTAL_RAM_PAGES_1G	(1 << 18)

static unsigned int orders[] = {9, 0};

#define NUM_ORDERS    ARRAY_SIZE(orders)

struct cpa_page_info {
	struct list_head pages;
	unsigned long num;
};

struct ion_cpa_heap {
	struct ion_heap heap;
	size_t heap_size;
	atomic64_t alloc_size;
	struct ion_page_pool *pools[NUM_ORDERS];
	struct device *dev;
	/* heap mutex */
	struct mutex mutex;
	TEEC_Context *context;
	TEEC_Session *session;
	int TA_init;
};

struct cpa_work_data {
	struct work_struct work;
	struct ion_cpa_heap *cpa_heap;
};

#define cpa_debug(level, x...)			\
	do {					\
		if (cpa_debug_level >= (level))	\
			pr_info(x);		\
	} while (0)

#define to_cpa_heap(x) container_of(x, struct ion_cpa_heap, heap)

static unsigned int zram_percentage = 100;
static short cpa_process_adj = 500;
static int cpa_debug_level;
static gfp_t high_order_gfp_flags = (GFP_KERNEL | __GFP_ZERO | __GFP_NOWARN |
				     __GFP_NORETRY) & ~__GFP_DIRECT_RECLAIM;
static gfp_t low_order_gfp_flags = GFP_KERNEL | __GFP_NOWARN;
static struct cpa_work_data *cpa_mem_work_data;

static void cpa_dump_tasks(void)
{
	struct task_struct *p = NULL;
	struct task_struct *task = NULL;
	short tsk_oom_adj = 0;
	unsigned long tsk_nr_ptes;
	int task_state = 0;
	char frozen_mark = ' ';

	pr_info("[ pid ]   uid  tgid total_vm    rss nptes  swap   adj s name\n");

	rcu_read_lock();
	for_each_process(p) {
		task = find_lock_task_mm(p);
		if (!task)
			continue;

		tsk_oom_adj = task->signal->oom_score_adj;
		tsk_nr_ptes = (unsigned long)atomic_long_read(&task->mm->nr_ptes);

		task_state = (int)__get_task_state(task);
		frozen_mark = frozen(task) ? '*' : ' ';

		pr_info("[%5d] %5d %5d %8lu %6lu %5lu %5lu %5hd %c %s%c\n",
			task->pid, from_kuid(&init_user_ns, task_uid(task)),
			task->tgid, task->mm->total_vm, get_mm_rss(task->mm),
			tsk_nr_ptes,
			get_mm_counter(task->mm, MM_SWAPENTS),
			tsk_oom_adj,
			task_state,
			task->comm,
			frozen_mark);
		task_unlock(task);
	}
	rcu_read_unlock();
}

static void cpa_show_mem(int debug_lev, char *show_msg)
{
	cpa_debug(debug_lev, "CPA show mem: %s\n", show_msg);
	if (debug_lev <= cpa_debug_level)
		show_mem(0, NULL);
}

static unsigned long cpa_free_mem_page(void)
{
	unsigned long free_pages = global_zone_page_state(NR_FREE_PAGES);

	if (free_pages > totalreserve_pages)
		free_pages -= totalreserve_pages;

	return free_pages;
}

static inline unsigned long get_free_page(void)
{
	return cpa_free_mem_page();
}

static inline unsigned long get_file_page(void)
{
	return global_node_page_state(NR_FILE_PAGES) -
		    global_node_page_state(NR_SHMEM) -
		    global_node_page_state(NR_UNEVICTABLE) -
		    total_swapcache_pages();
}

static inline unsigned long get_anon_page(void)
{
	return global_node_page_state(NR_ACTIVE_ANON) +
			global_node_page_state(NR_INACTIVE_ANON);
}

static inline unsigned long get_zram_page(void)
{
	return (unsigned long)(total_swap_pages - get_nr_swap_pages());
}

static void cpa_direct_dropcache(void)
{
	cpa_drop_pagecache();
	drop_slab();
}

/* Compact all nodes in the system */
static void cpa_direct_compact(int model, unsigned int order,
				unsigned long nrpages)
{
	cpa_compact_nodes(model, (int)order, nrpages);
}

static unsigned long cpa_reclaim_anon2zram(unsigned long nr_to_reclaim)
{
	return cpa_shrink_all_memory(nr_to_reclaim);
}

static void cpa_selected(struct task_struct *selected,
			unsigned long selected_task_size)
{
	unsigned long other_free = cpa_free_mem_page();
	unsigned long other_file = get_file_page();
	unsigned long other_anon = get_anon_page();

	task_lock(selected);
	send_sig(SIGKILL, selected, 0);
	task_unlock(selected);

	cpa_debug(CPA_DBG_LVL, "%s : Killing '%s' (%d), tgid=%d, adj %hd\n"
		  "   to free %ldkB on behalf of '%s' (%d) when\n"
		  "   free %lukB  file %lukB  anon %lukB\n",
		  __func__,
		  selected->comm, selected->pid, selected->tgid,
		  selected->signal->oom_score_adj,
		  selected_task_size * (long)(PAGE_SIZE / 1024),
		  current->comm, current->pid,
		  other_free * (PAGE_SIZE / 1024),
		  other_file * (PAGE_SIZE / 1024),
		  other_anon * (PAGE_SIZE / 1024));
}

static unsigned long cpa_direct_kill(int model, unsigned long reclaim_by_kill)
{
	struct task_struct *p = NULL;
	struct task_struct *tsk = NULL;
	struct task_struct *selected = NULL;
	short oom_score_adj = 0;
	unsigned long selected_task_size = 0;
	int nr_to_kill = CPA_MAX_KILL_NUM;
	unsigned long task_size = 0;
	unsigned long rem = 0;

	rcu_read_lock();
	do {
		for_each_process(tsk) {
			if (tsk->flags & PF_KTHREAD)
				continue;

			p = find_lock_task_mm(tsk);
			if (!p)
				continue;

			if (selected == p) {
				task_unlock(p);
				continue;
			}

			if (test_tsk_thread_flag(p, TIF_MEMDIE)) {
				task_unlock(p);
				continue;
			}

			if ((unsigned long)p->state & TASK_UNINTERRUPTIBLE) {
				task_unlock(p);
				cpa_debug(CPA_DBG_LVL, "[%s]filter D state process: %d (%s) state:0x%lx\n",
					__func__, p->pid, p->comm, p->state);
				continue;
			}

			oom_score_adj = p->signal->oom_score_adj;
			if (oom_score_adj < cpa_process_adj) {
				task_unlock(p);
				continue;
			}

			if (!model)
				task_size = get_mm_rss(p->mm);
			else
				task_size = get_mm_counter(p->mm, MM_SWAPENTS);

			task_unlock(p);

			if (task_size <= 0)
				continue;

			if (selected) {
				if (task_size < selected_task_size)
					continue;
			}
			oom_score_adj = 0;
			selected = p;
			selected_task_size = task_size;
		}

		if (selected) {
			cpa_selected(selected, selected_task_size);
			rem += selected_task_size;
		}

		p = NULL;
		tsk = NULL;
		selected_task_size = 0;
	} while ((rem < reclaim_by_kill) && (--nr_to_kill));

	rcu_read_unlock();

	return rem;
}

static inline unsigned int order_to_size(unsigned int order)
{
	return (PAGE_SIZE << order);
}

static int order_to_index(unsigned int order)
{
	unsigned int i;

	for (i = 0; i < NUM_ORDERS; i++)
		if (order == orders[i])
			return i;

	return -1;
}

static struct page *alloc_buffer_page(struct ion_cpa_heap *cpa_heap,
				unsigned int order, unsigned int is_nofail)
{
	struct ion_page_pool *pool = NULL;
	struct page *page = NULL;
	int index = order_to_index(order);
	if (index < 0)
		return NULL;

	pool = cpa_heap->pools[index];
	if (is_nofail && (order <= orders[1]))
		pool->gfp_mask = (pool->gfp_mask | __GFP_NOFAIL) &
							(~__GFP_NORETRY);

	/*
	 * if need to use page pool;
	 * now using the alloc func as below:
	 * page = ion_page_pool_alloc(pool);
	 */
	page = ion_page_pool_alloc(pool);

	return page;
}

static void free_buffer_page(struct ion_cpa_heap *cpa_heap,
			     struct ion_buffer *buffer, struct page *page)
{
	struct ion_page_pool *pool = NULL;
	unsigned int order = compound_order(page);
	int index = order_to_index(order);
	if (index < 0)
		return;

	pool = cpa_heap->pools[index];

	/*
	 * if need to use page pool;
	 * now using the alloc func as below:
	 * ion_page_pool_free(pool, page);
	 */
	__free_pages(page, pool->order);
}

static struct page *alloc_largest_available(struct ion_cpa_heap *cpa_heap,
					    unsigned long ac_sz,
					    unsigned int max_order,
					    unsigned int is_nofail)
{
	unsigned int i;
	struct page *page = NULL;

	for (i = 0; i < NUM_ORDERS; i++) {
		if (ac_sz < order_to_size(orders[i]))
			continue;
		if (max_order < orders[i])
			continue;

		page = alloc_buffer_page(cpa_heap, orders[i], is_nofail);
		if (!page)
			continue;

		return page;
	}

	return NULL;
}

static unsigned long cpa_alloc_large(struct ion_cpa_heap *cpa_heap,
				     struct cpa_page_info *info,
				     unsigned long size,
				     unsigned int is_nofail)
{
	struct page *page = NULL;
	unsigned int max_order = orders[0];
	unsigned long size_remaining = size;

	while (size_remaining > 0) {
		page = alloc_largest_available(cpa_heap,
					       size_remaining,
					       max_order,
					       is_nofail);
		if (!page)
			break;
		max_order = compound_order(page);
		list_add_tail(&page->lru, &info->pages);
		info->num++;
		size_remaining -= (PAGE_SIZE << max_order);
	}

	return (size - size_remaining);
}

static int __cpa_heap_input_check(struct ion_cpa_heap *cpa_heap,
				  unsigned long size)
{
	long int alloc_size;

	alloc_size = atomic64_read(&cpa_heap->alloc_size);
	if ((alloc_size + size) > cpa_heap->heap_size) {
		pr_err("alloc size = 0x%lx, size = 0x%lx, heap size = 0x%lx\n",
		       alloc_size, size,
		       cpa_heap->heap_size);
		return -EINVAL;
	}

	if (size > SZ_64M) {
		pr_err("size too large! size 0x%lx, per_alloc_sz 0x4000000(64M)\n",
		       size);
		return -EINVAL;
	}

	return 0;
}

static int change_cpa_prop(struct ion_cpa_heap *cpa_heap,
			   struct ion_buffer *buffer, u32 cmd)
{
	struct sg_table *table = buffer->sg_table;
	struct scatterlist *sg = NULL;
	struct page *page = NULL;
	struct tz_pageinfo *pageinfo = NULL;
	struct mem_chunk_list mcl;
	unsigned int nents = table->nents;
	unsigned int i = 0;
	int ret = 0;

	if (!cpa_heap->TA_init) {
		pr_err("[%s] TA not inited.\n", __func__);
		return -EINVAL;
	}

	if (cmd == ION_SEC_CMD_ALLOC) {
		pageinfo = kcalloc(nents, sizeof(*pageinfo), GFP_KERNEL);
		if (!pageinfo)
			return -ENOMEM;

		for_each_sg(table->sgl, sg, table->nents, i) {
			page = sg_page(sg);
			pageinfo[i].addr = page_to_phys(page);
			pageinfo[i].nr_pages = sg->length / PAGE_SIZE;
		}

		mcl.phys_addr = (void *)pageinfo;
		mcl.nents = nents;
		mcl.protect_id = SEC_TASK_DRM;
	} else if (cmd == ION_SEC_CMD_FREE) {
		mcl.protect_id = SEC_TASK_DRM;
		mcl.buff_id = buffer->id;
		mcl.phys_addr = NULL;
	} else {
		pr_err("[%s] Error cmd\n", __func__);
		return -EINVAL;
	}

	ret = secmem_tee_exec_cmd(cpa_heap->session, &mcl, cmd);
	if (ret) {
		pr_err("[%s] Exec cmd[%d] fail\n", __func__, cmd);
		ret = -EINVAL;
	} else {
		if (cmd == ION_SEC_CMD_ALLOC)
			buffer->id = mcl.buff_id;
	}

	if (pageinfo)
		kfree(pageinfo);

	return ret;
}

static int cpa_propare_session(struct ion_cpa_heap *cpa_heap)
{
	int ret = 0;

	if (!cpa_heap->TA_init) {
		ret = secmem_tee_init(cpa_heap->context,
				      cpa_heap->session, TEE_SECMEM_NAME);
		if (ret) {
			pr_err("[%s] TA init failed\n", __func__);
			goto err;
		}

		cpa_heap->TA_init = 1;
	}

err:
	return ret;
}

static inline unsigned long cpa_water_mark_page(struct ion_cpa_heap *cpa_heap)
{
	unsigned long alloc_size = (unsigned long)atomic64_read(&cpa_heap->alloc_size);

	return (((cpa_heap->heap_size - alloc_size) * 100 / CPA_WATER_MARK_RATIO) >> PAGE_SHIFT);
}

static unsigned long get_zram_water_mark_page(void)
{
	unsigned int cpa_zram_ratio;

	if (total_swap_pages > (totalram_pages * zram_percentage / 100))
		cpa_zram_ratio = 20;
	else
		cpa_zram_ratio = 75;

	cpa_debug(CPA_DBG_LVL, "[%s] cpa_zram_ratio: %u\n",
			__func__, cpa_zram_ratio);

	return (total_swap_pages * cpa_zram_ratio / 100);
}

static void cpa_prepare_memory(struct ion_cpa_heap *cpa_heap,
				int compact_model,
				unsigned long water_mark_page,
				unsigned long tokill_rem_page,
				unsigned long toreclaim_page,
				unsigned long *kill_rem_page,
				unsigned long *reclaimed_page)
{
	unsigned long free_page;
	unsigned long file_page;
	unsigned long file_water_mark_page;
	unsigned long zram_page;
	unsigned long zram_water_mark_page;
	unsigned long nr_expect;

	cpa_show_mem(CPA_DBG_LVL, "start cpa prepare memory\n");

	/**
	 * 1.dropcache
	 */
	file_water_mark_page = CPA_SIZE_320M_PAGE;
	free_page = get_free_page();
	file_page = get_file_page();

	cpa_debug(CPA_DBG_LVL, "[%s] step dropcache before, free_page: %lu, water_mark_page: %lu; file_page: %lu, file_page_water_mark: %lu\n",
		  __func__, free_page, water_mark_page,
		  file_page, file_water_mark_page);

	if ((free_page < water_mark_page) &&
	    (file_page > file_water_mark_page)) {

		cpa_direct_dropcache();

		cpa_show_mem(CPA_DBG_LVL, "after dropcache\n");
	}

	/**
	 * 2.kill process
	 */
	zram_page = get_zram_page();
	zram_water_mark_page = get_zram_water_mark_page();

	cpa_debug(CPA_DBG_LVL, "[%s] step kill proc before, zram_page: %lu, zram_water_mark_page: %lu\n",
		  __func__, zram_page, zram_water_mark_page);

	if (zram_page > zram_water_mark_page) {

		*kill_rem_page = cpa_direct_kill(CPA_KP_ZRAM_MODEL,
						 tokill_rem_page);
		if (!*kill_rem_page) {
			cpa_show_mem(CPA_ERR_LVL,
				     "prepare memory kill process fail\n");
			cpa_dump_tasks();
		}

		cpa_show_mem(CPA_DBG_LVL, "after kill process\n");
	}

	/**
	 * 3.reclaim memory
	 *
	*/
	free_page = get_free_page();
	cpa_debug(CPA_DBG_LVL, "[%s] step reclaim before, free_page: %lu, water_mark_page: %lu\n",
		  __func__, free_page, water_mark_page);

	if (free_page < water_mark_page) {

		*reclaimed_page = cpa_reclaim_anon2zram(toreclaim_page);
		if (!*reclaimed_page)
			cpa_show_mem(CPA_ERR_LVL,
				     "prepare memory reclaim memory fail\n");

		cpa_show_mem(CPA_DBG_LVL, "after reclaim\n");
	}

	/*
	 * 4.compact nodes
	 */
	if (*kill_rem_page || *reclaimed_page) {
		nr_expect = toreclaim_page >> orders[1];
		cpa_debug(CPA_DBG_LVL, "[%s] compact nr_expect page: %lu\n",
			__func__, nr_expect);

		cpa_direct_compact(compact_model, orders[1], nr_expect);
	}

	cpa_show_mem(CPA_DBG_LVL, "finish cpa prepare memory\n");
}

static void cpa_prepare_memory_for_work(struct work_struct *work)
{
	unsigned long water_mark_page = 0;
	unsigned long free_page = 0;
	unsigned long tokill_rem_page = 0;
	unsigned long toreclaim_page = 0;
	unsigned long kill_rem_page = 0;
	unsigned long reclaimed_page = 0;
	struct cpa_work_data *work_data = container_of(work, struct cpa_work_data, work);

	water_mark_page = cpa_water_mark_page(work_data->cpa_heap);

	free_page = get_free_page();
	if (free_page < water_mark_page)
		toreclaim_page = water_mark_page - free_page;

	tokill_rem_page = CPA_RK_RATIO_BY_ZRAM * toreclaim_page;

	cpa_prepare_memory(work_data->cpa_heap, 1, water_mark_page,
			    tokill_rem_page, toreclaim_page,
			   &kill_rem_page, &reclaimed_page);

	cpa_debug(CPA_DBG_LVL, "[%s] kill process free mempage: %lu, reclaimed mempage: %lu\n",
		  __func__, kill_rem_page, reclaimed_page);
}

static void cpa_prepare_memory_for_alloc(struct ion_cpa_heap *cpa_heap,
					unsigned long size,
					unsigned long *kill_rem_page,
					unsigned long *reclaimed_page)
{
	unsigned long water_mark_page;
	unsigned long tokill_rem_page;
	unsigned long toreclaim_page;

	water_mark_page = cpa_water_mark_page(cpa_heap);
	tokill_rem_page = (size / PAGE_SIZE);
	toreclaim_page = tokill_rem_page * 100 / CPA_WATER_MARK_RATIO;

	cpa_prepare_memory(cpa_heap, 0, water_mark_page,
			   tokill_rem_page, toreclaim_page,
			   kill_rem_page, reclaimed_page);

	cpa_debug(CPA_DBG_LVL, "[%s] kill process free mempage:%lu, reclaimed mempage:%lu\n",
			__func__, *kill_rem_page, *reclaimed_page);
}

static int do_size_remaining(unsigned long size_remaining, unsigned long size,
					struct ion_cpa_heap *cpa_heap, struct cpa_page_info *info, bool flag,
					unsigned long kill_rem_page, unsigned long reclaimed_page)
{
	unsigned long retry_to_rk_page = 0;
	unsigned long retry_kill_rem_page = 0;
	unsigned long retry_reclaimed_page = 0;
	unsigned long nr_expect = 0;

	if (size_remaining) {

		retry_to_rk_page = size / PAGE_SIZE;

		retry_kill_rem_page = cpa_direct_kill(CPA_KP_FREEMEM_MODEL,
						      retry_to_rk_page);
		if (!retry_kill_rem_page)
			cpa_debug(CPA_ERR_LVL, "[%s]retry kill process but no free\n",
				  __func__);

		retry_reclaimed_page = cpa_reclaim_anon2zram(retry_to_rk_page);
		if (!retry_reclaimed_page)
			cpa_debug(CPA_ERR_LVL, "[%s]retry alloc to reclaim page: no mempage to reclaim\n",
				  __func__);
		nr_expect = retry_to_rk_page >> orders[1];
		cpa_direct_compact(1, orders[1], nr_expect);

		size_remaining -= cpa_alloc_large(cpa_heap, info,
						  size_remaining, flag);
	}

	if (size_remaining) {
		pr_err("[%s] No enough memory to alloc! size_remaining: %lx Byte\n",
		       __func__, size_remaining);

		pr_err("[%s] prepare memory argument show kill freedpage: %lu, reclaimed mempage: %lu\n",
		       __func__, kill_rem_page, reclaimed_page);

		pr_err("[%s] retry alloc -- need free mempage:%lu; kill proc freepage: %lu, reclaimed mempage: %lu\n",
		       __func__, retry_to_rk_page,
		       retry_kill_rem_page, retry_reclaimed_page);

		cpa_show_mem(CPA_ERR_LVL, "cpa alloc fail\n");

		retry_kill_rem_page = cpa_direct_kill(CPA_KP_FREEMEM_MODEL,
					CPA_RK_RATIO * retry_to_rk_page);
		cpa_debug(CPA_ERR_LVL, "[%s]No enough mem kill process to free mempage: %lu, free mempage: %lu\n",
			__func__, (CPA_RK_RATIO * retry_to_rk_page),
			  retry_kill_rem_page);

		queue_work(system_power_efficient_wq, &cpa_mem_work_data->work);

		return -1;
	}

	return 0;
}

static int do_cpa_prop(struct ion_buffer *buffer, struct sg_table *table,
				struct ion_cpa_heap *cpa_heap)
{
	int ret;

#ifdef CONFIG_HISI_ION_FLUSH_CACHE_ALL
	ion_flush_all_cpus_caches_raw();
#else
	ion_flush_all_cpus_caches();
#endif
	buffer->sg_table = table;

	ret = change_cpa_prop(cpa_heap, buffer, ION_SEC_CMD_ALLOC);
	if (ret)
		return-1;

	return 0;
}

static int ion_cpa_allocate(struct ion_heap *heap,
			    struct ion_buffer *buffer,
			    unsigned long size, unsigned long flags)
{
	struct page *page = NULL;
	struct page *tmp_page = NULL;
	struct scatterlist *sg = NULL;
	struct sg_table *table = NULL;
	struct ion_cpa_heap *cpa_heap = to_cpa_heap(heap);
	struct cpa_page_info info;
	unsigned long kill_rem_page = 0;
	unsigned long reclaimed_page = 0;
	/*lint -e666 */
	unsigned long size_remaining = ALIGN(size, order_to_size(orders[1]));
	/*lint +e666 */
#ifdef CONFIG_NEED_CHANGE_MAPPING
	unsigned int i = 0;
#endif
	int ret = 0;
	bool flag = false;

	cpa_debug(CPA_DBG_LVL, "cpa alloc enter [%s] -- alloc size: 0x%lx, heap_id: %u, cpa heap all alloc_size: 0x%lx\n",
		  __func__, size, heap->id,
		  atomic64_read(&cpa_heap->alloc_size));

	mutex_lock(&cpa_heap->mutex);

	ret = __cpa_heap_input_check(cpa_heap, size);
	if (ret) {
		pr_err("cpa alloc input check: params err!\n");
		goto out;
	}

	info.num = 0;
	INIT_LIST_HEAD(&info.pages);

	queue_work(system_power_efficient_wq, &cpa_mem_work_data->work);

	/*
	 * 1.cpa prepare session for the first time
	 */
	ret = cpa_propare_session(cpa_heap);
	if (ret)
		goto out;

	/*
	 * 2.cpa prepare memory for remain size alloc
	 */
	cpa_prepare_memory_for_alloc(cpa_heap, size_remaining,
				     &kill_rem_page, &reclaimed_page);

	size_remaining -= cpa_alloc_large(cpa_heap,
					  &info, size_remaining, flag);
	ret = do_size_remaining(size_remaining, size, cpa_heap, &info, flag,
					kill_rem_page, reclaimed_page);
	if (ret) {
		ret = -1;
		goto free_pages;
	}

	table = kzalloc(sizeof(*table), GFP_KERNEL);
	if (!table) {
		ret = -1;
		goto free_pages;
	}

	ret = sg_alloc_table(table, info.num, GFP_KERNEL);
	if (ret) {
		ret = -1;
		goto free_table;
	}

	sg = table->sgl;
	list_for_each_entry_safe(page, tmp_page, &info.pages, lru) {
#ifdef CONFIG_NEED_CHANGE_MAPPING
		change_secpage_range(page_to_phys(page),
				     (unsigned long)page_address(page),
				     PAGE_SIZE << compound_order(page),
				     __pgprot(PROT_DEVICE_nGnRE));
#endif
		sg_set_page(sg, page, PAGE_SIZE << compound_order(page), 0);
		sg = sg_next(sg);
		list_del(&page->lru);
	}

#ifdef CONFIG_NEED_CHANGE_MAPPING
	flush_tlb_all();
#endif
	ret = do_cpa_prop(buffer, table, cpa_heap);
	if(ret) {
		ret = -1;
		goto free_sg_table;
	}

	atomic64_add(size, &cpa_heap->alloc_size);
	mutex_unlock(&cpa_heap->mutex);
	cpa_debug(CPA_DBG_LVL, "cpa alloc succ -- alloc size: 0x%lx , have already alloc size: 0x%lx\n",
		size, atomic64_read(&cpa_heap->alloc_size));
	return 0;

free_sg_table:
#ifdef CONFIG_NEED_CHANGE_MAPPING
	for_each_sg(table->sgl, sg, table->nents, i) {
		change_secpage_range(page_to_phys(sg_page(sg)),
				     (unsigned long)page_address(sg_page(sg)),
				     sg->length, PAGE_KERNEL);
		flush_tlb_all();
	}
	flush_tlb_all();
#endif
	sg_free_table(table);

free_table:
	kfree(table);

free_pages:
	list_for_each_entry_safe(page, tmp_page, &info.pages, lru)
		free_buffer_page(cpa_heap, buffer, page);
out:
	mutex_unlock(&cpa_heap->mutex);

	cpa_debug(CPA_DBG_LVL, "CPA alloc failed!");

	return ret;
}

static void cpa_free(struct ion_buffer *buffer)
{
	struct ion_cpa_heap *cpa_heap = to_cpa_heap(buffer->heap);
	struct sg_table *table = buffer->sg_table;
	struct scatterlist *sg = NULL;
	unsigned long size = 0;
	unsigned int i = 0;

	/* add to cpa pool */
	for_each_sg(table->sgl, sg, table->nents, i) {
#ifdef CONFIG_NEED_CHANGE_MAPPING
		change_secpage_range(page_to_phys(sg_page(sg)),
				     (unsigned long)page_address(sg_page(sg)),
				     sg->length, PAGE_KERNEL);
		flush_tlb_all();
#endif
		free_buffer_page(cpa_heap, buffer, sg_page(sg));
	}

	/* shrink to buddy */
	size = buffer->size;
	atomic64_sub(size, &cpa_heap->alloc_size);
	if (!atomic64_read(&cpa_heap->alloc_size)) {
		for (i = 0; i < NUM_ORDERS; i++)
			(void)ion_page_pool_shrink(cpa_heap->pools[i],
				0, (int)(cpa_heap->heap_size >> PAGE_SHIFT));
	}
}

static void ion_cpa_free(struct ion_buffer *buffer)
{
	struct ion_cpa_heap *cpa_heap = to_cpa_heap(buffer->heap);
	struct sg_table *table = buffer->sg_table;
	unsigned long size = buffer->size;
	int ret = 0;
	int is_lock_recursive;

	is_lock_recursive = hisi_ion_mutex_lock_recursive(&cpa_heap->mutex);

	ret = change_cpa_prop(cpa_heap, buffer, ION_SEC_CMD_FREE);
	if (ret) {
		hisi_ion_mutex_unlock_recursive(&cpa_heap->mutex,
				is_lock_recursive);
		pr_err("cpa release MPU protect fail! Need check DRM runtime\n");
		return;
	}

	cpa_free(buffer);
#ifdef CONFIG_NEED_CHANGE_MAPPING
	flush_tlb_all();
#endif
	sg_free_table(table);
	kfree(table);

	hisi_ion_mutex_unlock_recursive(&cpa_heap->mutex,
			is_lock_recursive);

	cpa_debug(CPA_DBG_LVL, "cpa free succ -- free size: 0x%lx, cpa heap size: 0x%lx, have already alloc size: 0x%lx\n",
		  size, cpa_heap->heap_size,
		  atomic64_read(&cpa_heap->alloc_size));
}

int ion_cpa_heap_phys(struct ion_heap *heap, struct ion_buffer *buffer,
		      phys_addr_t *addr, size_t *len)
{
	if (heap->type != ION_HEAP_TYPE_CPA)
		return -EINVAL;

	*addr = buffer->id;
	*len = buffer->size;

	return 0;
}

static struct ion_heap_ops ion_cpa_ops = {
	.allocate = ion_cpa_allocate,
	.free = ion_cpa_free,
	/*
	 * if need to use page pool;
	 * now using the alloc func as below:
	 *
	 * .shrink = ion_cpa_heap_shrink,
	 */
};

static int cpa_parse_dt(struct device *dev, struct ion_platform_heap *data,
			struct ion_cpa_heap *cpa_heap)
{
	struct device_node *nd = NULL;
	int ret = 0;

	nd = of_get_child_by_name(dev->of_node, data->name);
	if (!nd) {
		pr_err("cpa can't of_get_child_by_name %s\n", data->name);
		ret = -EINVAL;
	}

	return ret;
}

static void ion_cpa_heap_destroy_pools(struct ion_page_pool **pools)
{
	unsigned int i;

	for (i = 0; i < NUM_ORDERS; i++)
		if (pools[i])
			ion_page_pool_destroy(pools[i]);
}

static int ion_cpa_heap_create_pools(struct ion_page_pool **pools)
{
	unsigned int i;

	for (i = 0; i < NUM_ORDERS; i++) {
		struct ion_page_pool *pool = NULL;
		gfp_t gfp_flags = low_order_gfp_flags;

		if (orders[i] >= 8)
			gfp_flags = high_order_gfp_flags & ~__GFP_RECLAIMABLE;

		pool = ion_page_pool_create(gfp_flags, orders[i]);
		if (!pool)
			goto err_create_pool;
		pools[i] = pool;
	}

	return 0;

err_create_pool:
	ion_cpa_heap_destroy_pools(pools);

	return -ENOMEM;
}

static void ion_cpa_heap_init_properties(struct ion_cpa_heap *cpa_heap, struct device **dev,
		struct ion_platform_heap *data)
{

	cpa_heap->heap.ops = &ion_cpa_ops;
	cpa_heap->dev = data->priv;
	cpa_heap->heap.type = ION_HEAP_TYPE_CPA;
	cpa_heap->heap_size = data->size;
	cpa_heap->heap.flags |= ION_HEAP_FLAG_DEFER_FREE;
	*dev = data->priv;
}

static void ion_cpa_init_pool_order()
{
	/*
	 * TOTAL_RAM_PAGES_1G means the RAM SIZE
	 * is aligned according to 1G size
	 *
	 * 8 * TOTAL_RAM_PAGES_1G means 8G DDR size
	 * 4 * TOTAL_RAM_PAGES_1G means 4G DDR size
	 *
	 * And order[1] indicate that we may alloc 2^N(6\5\4)
	 * continuous pages luckily
	 */
	if (totalram_pages > 8 * TOTAL_RAM_PAGES_1G)
		orders[1] = 6;
	else if (totalram_pages > 4 * TOTAL_RAM_PAGES_1G)
		orders[1] = 5;
	else
		orders[1] = 4;
}

static void ion_cpa_init_zram_percentage(void)
{
	/*
	 * zram_percentage, a water_mark for cpa_zram_ratio,
	 * was used to judge whether hyperhold is enable or not.
	 * when hyperhold is enable, zram increased 4G for all platform.
	 * when hyperhold enable, zram was reconfiged as follow:
	 * DDR 6G: 6G zram
	 * DDR 8G: 8G zram
	 * DDR 12G: 8G zram
	 * accroding to orders[1], considering some reasons, we use this
	 * way to adapt to cpa_kill_process_water_mark.
	 */
	switch (orders[1]) {
	case 4:
	case 5:
		zram_percentage = 75;
		break;
	case 6:
		zram_percentage = 50;
		break;
	default:
		break;
	}
}

struct ion_heap *ion_cpa_heap_create(struct ion_platform_heap *data)
{
	struct ion_cpa_heap *cpa_heap = NULL;
	struct cpa_work_data *cpa_work_data = NULL;
	struct device *dev = NULL;
	int ret;

	cpa_heap = kzalloc(sizeof(*cpa_heap), GFP_KERNEL);
	if (!cpa_heap)
		return ERR_PTR(-ENOMEM);

	cpa_work_data = kzalloc(sizeof(*cpa_work_data), GFP_KERNEL);
	if (!cpa_work_data)
		goto free_workdata;

	INIT_WORK(&(cpa_work_data->work), cpa_prepare_memory_for_work);

	mutex_init(&cpa_heap->mutex);

	ion_cpa_heap_init_properties(cpa_heap, &dev, data);

	ret = cpa_parse_dt(dev, data, cpa_heap);
	if (ret)
		goto free_heap;

	ion_cpa_init_pool_order();
	ion_cpa_init_zram_percentage();

	if (ion_cpa_heap_create_pools(cpa_heap->pools))
		goto free_heap;

	cpa_heap->context = kzalloc(sizeof(TEEC_Context), GFP_KERNEL);
	if (!cpa_heap->context)
		goto destroy_pools;
	cpa_heap->session = kzalloc(sizeof(TEEC_Session), GFP_KERNEL);
	if (!cpa_heap->session)
		goto free_context;

	pr_err("cpa heap info %s:\n"
		  "\t\t\t\t heap id : %u\n"
		  "\t\t\t\t heap size : %lu MB\n",
		  data->name,
		  data->id,
		  cpa_heap->heap_size / SZ_1M);

	cpa_work_data->cpa_heap = cpa_heap;
	cpa_mem_work_data = cpa_work_data;

	return &cpa_heap->heap;

free_context:
	if (cpa_heap->context)
		kfree(cpa_heap->context);
destroy_pools:
	ion_cpa_heap_destroy_pools(cpa_heap->pools);
free_heap:
	mutex_destroy(&cpa_heap->mutex);
	kfree(cpa_work_data);
free_workdata:
	kfree(cpa_heap);

	return ERR_PTR(-ENOMEM);
}
module_param_named(cpa_debug_level, cpa_debug_level, int, 0640);
