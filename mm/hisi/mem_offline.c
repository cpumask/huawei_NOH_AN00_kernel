/*
 * Copyright (c) 2019, Huawei Technologies Co. Ltd. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/of.h>
#include <linux/of_fdt.h>
#include <linux/memory.h>
#include <linux/module.h>
#include <linux/libfdt.h>
#include <linux/memblock.h>
#include <linux/platform_device.h>
#include <linux/device.h>
#include <linux/mm.h>
#include <linux/sched/mm.h>
#include <linux/compaction.h>
#include <linux/slab.h>
#include <linux/rmap.h>
#include <linux/mm_inline.h>
#include <linux/io.h>
#include <linux/suspend.h>
#include <linux/oom.h>
#include <asm/tlbflush.h>
#include <linux/hugetlb.h>
#include <linux/ktime.h>
#include <linux/sort.h>
#include <linux/highmem.h>
#include <linux/delay.h>
#include <soc_acpu_baseaddr_interface.h>
#include <soc_sctrl_interface.h>

#define DDR_OFFLINE_FLAG_OFFSET 16
#define DDR_OFFLINE_FLAG_MASK BIT(16)
#define MODULE_CLASS_NAME "mem-offline"
#define MODULE_LOG "mem-offline: "
#define BUF_LEN 100
#define MAX_RECLAIM_TASKS 100
#define ANONPAGES_RECLAIM_PERIOD 3
/* max number of pages to be reclaimed in a single run */
#define PER_SWAP_SIZE 25600

struct section_stat {
	unsigned long success_count;
	unsigned long fail_count;

	unsigned long prepare_time;
	unsigned long best_prepare_time;
	unsigned long worst_prepare_time;
	unsigned long avg_prepare_time;
	unsigned long total_prepare_time;

	unsigned long migrate_time;
	unsigned long best_migrate_time;
	unsigned long worst_migrate_time;
	unsigned long avg_migrate_time;
	unsigned long total_migrate_time;

	unsigned long best_time;
	unsigned long worst_time;
	unsigned long avg_time;
	unsigned long total_time;
};

struct selected_task {
	struct task_struct *p;
	int tasksize;
	short oom_score_adj;
};

enum memory_states {
	MEMORY_ONLINE,
	MEMORY_OFFLINE,
	MAX_STATE,
};

static unsigned long start_section_nr, end_section_nr;
static unsigned int sections_per_block;
static struct memblock_region offline_memblock_region;
static struct kobject *mem_kobj;
static char __iomem *sctrl_base;
static struct section_stat *mem_info;
static unsigned long lowmem_deathpending_timeout;
static unsigned int memory_offline_enable;
static short kill_process_adj;
static unsigned long filepages_reclaim_watermark = 10 * SZ_1M;
static unsigned long anonpages_reclaim_watermark = 200 * SZ_1M;

static void mem_offline_notify_ddrc_msg(int value)
{
	u32 tmp;
	u32 flag;

	flag = value ? 0 : 1;

	if (sctrl_base) {
		tmp = (unsigned int)readl(SOC_SCTRL_SCBAKDATA7_ADDR(sctrl_base));
		tmp &= ~DDR_OFFLINE_FLAG_MASK;
		tmp |= (flag << DDR_OFFLINE_FLAG_OFFSET);
		writel(tmp, SOC_SCTRL_SCBAKDATA7_ADDR(sctrl_base));
	}
}

void mem_offline_record_success_stat(unsigned long sec, int mode)
{
	unsigned int total_sec = end_section_nr - start_section_nr + 1;
	unsigned int blk_nr = (sec - start_section_nr + mode * total_sec) /
				sections_per_block;

	if (sec > end_section_nr)
		return;

	if (((mem_info[blk_nr].prepare_time + mem_info[blk_nr].migrate_time) <
		mem_info[blk_nr].best_time) || !mem_info[blk_nr].best_time) {
		mem_info[blk_nr].best_time = mem_info[blk_nr].prepare_time +
				mem_info[blk_nr].migrate_time;
		mem_info[blk_nr].best_prepare_time =
				mem_info[blk_nr].prepare_time;
		mem_info[blk_nr].best_migrate_time =
				mem_info[blk_nr].migrate_time;
	}

	if ((mem_info[blk_nr].prepare_time + mem_info[blk_nr].migrate_time) >
			mem_info[blk_nr].worst_time) {
		mem_info[blk_nr].worst_time = mem_info[blk_nr].prepare_time +
				mem_info[blk_nr].migrate_time;
		mem_info[blk_nr].worst_prepare_time =
			mem_info[blk_nr].prepare_time;
		mem_info[blk_nr].worst_migrate_time =
			mem_info[mode].migrate_time;
	}

	++mem_info[blk_nr].success_count;
	if (mem_info[blk_nr].fail_count)
		--mem_info[blk_nr].fail_count;

	mem_info[blk_nr].total_time += (mem_info[blk_nr].prepare_time +
			mem_info[blk_nr].migrate_time);
	mem_info[blk_nr].total_prepare_time += mem_info[blk_nr].prepare_time;
	mem_info[blk_nr].total_migrate_time += mem_info[blk_nr].migrate_time;

	mem_info[blk_nr].avg_time =
	    mem_info[blk_nr].total_time / mem_info[blk_nr].success_count;
	mem_info[blk_nr].avg_prepare_time =
	mem_info[blk_nr].total_prepare_time / mem_info[blk_nr].success_count;
	mem_info[blk_nr].avg_migrate_time =
	mem_info[blk_nr].total_migrate_time / mem_info[blk_nr].success_count;
}

void mem_offline_record_fail_stat(int mode)
{
	unsigned int blk_start = start_section_nr / sections_per_block;
	unsigned int blk_end = end_section_nr / sections_per_block;
	unsigned int idx = blk_end - blk_start + 1;
	int i;

	for (i = 0; i <= blk_end - blk_start; i++)
		++mem_info[i + mode * idx].fail_count;
}

void mem_offline_record_prepare_stat(ktime_t delay, int mode)
{
	unsigned int blk_start = start_section_nr / sections_per_block;
	unsigned int blk_end = end_section_nr / sections_per_block;
	unsigned int idx = blk_end - blk_start + 1;
	int i;

	for (i = 0; i <= blk_end - blk_start; i++)
		mem_info[i + mode * idx].prepare_time = delay;
}

static int mem_offline_event_callback(struct notifier_block *self,
				unsigned long action, void *arg)
{
	struct memory_notify *mn = arg;
	unsigned long start, end, sec_nr;
	static ktime_t cur;
	ktime_t delay;
	unsigned int idx = end_section_nr - start_section_nr + 1;

	start = SECTION_ALIGN_DOWN(mn->start_pfn);
	end = SECTION_ALIGN_UP(mn->start_pfn + mn->nr_pages);

	if (start != mn->start_pfn || end != mn->start_pfn + mn->nr_pages) {
		pr_err(MODULE_LOG "start pfn = %lu end pfn = %lu\n",
			mn->start_pfn, mn->start_pfn + mn->nr_pages);
		return -EINVAL;
	}
	sec_nr = pfn_to_section_nr(start);

	if (sec_nr > end_section_nr || sec_nr < start_section_nr) {
		if (action == MEM_ONLINE || action == MEM_OFFLINE)
			pr_err(MODULE_LOG "%s mem%d not our block and do nothing\n",
			  action == MEM_ONLINE ? "Onlined" : "Offlined", sec_nr);
		return NOTIFY_OK;
	}

	switch (action) {
	case MEM_GOING_ONLINE:
		++mem_info[(sec_nr - start_section_nr + MEMORY_ONLINE *
			   idx) / sections_per_block].fail_count;
		cur = ktime_get();
		break;
	case MEM_ONLINE:
		pr_info(MODULE_LOG "onlined memory block mem%lu\n", sec_nr);
		delay = ktime_ms_delta(ktime_get(), cur);
		mem_info[(sec_nr - start_section_nr + MEMORY_ONLINE *
			   idx) / sections_per_block].migrate_time = delay;
		mem_offline_record_success_stat(sec_nr, MEMORY_ONLINE);
		cur = 0;
		break;
	case MEM_GOING_OFFLINE:
		++mem_info[(sec_nr - start_section_nr + MEMORY_OFFLINE *
			   idx) / sections_per_block].fail_count;
		cur = ktime_get();
		break;
	case MEM_OFFLINE:
		pr_info(MODULE_LOG "offlined memory block mem%lu\n", sec_nr);
		delay = ktime_ms_delta(ktime_get(), cur);
		mem_info[(sec_nr - start_section_nr + MEMORY_OFFLINE *
			   idx) / sections_per_block].migrate_time = delay;
		mem_offline_record_success_stat(sec_nr, MEMORY_OFFLINE);
		cur = 0;
		break;
	default:
		break;
	}

	return NOTIFY_OK;
}

static unsigned long get_dma_mem_free(void)
{
	unsigned long free_pages;
	struct zone *zones = NODE_DATA(node)->node_zones;

	free_pages = zone_page_state(&zones[ZONE_DMA], NR_FREE_PAGES);

	return free_pages;
}

static unsigned long get_movable_mem_used(void)
{
	unsigned long used_pages;
	struct zone *zones = NODE_DATA(node)->node_zones;

	used_pages = (offline_memblock_region.size >> PAGE_SHIFT) -
		zone_page_state(&zones[ZONE_MOVABLE], NR_FREE_PAGES);

	return used_pages;
}

bool is_enough_free_pages(void)
{
	unsigned long dma_zone_free;
	unsigned long movable_zone_used;

	dma_zone_free = get_dma_mem_free();
	movable_zone_used = get_movable_mem_used();

	return dma_zone_free > movable_zone_used ? true : false;
}

static int memory_block_offline_device(void)
{
	struct memory_block *mem = NULL;
	unsigned long section_nr;
	int ret = -1;

	if (!is_enough_free_pages()) {
		pr_err(MODULE_LOG "not enough dma zone free pages for migrate");
		show_mem(0, NULL);
		mem_offline_record_fail_stat(MEMORY_OFFLINE);
		return -ENOMEM;
	}

	ret = lock_device_hotplug_sysfs();
	if (ret) {
		pr_err(MODULE_LOG "lock device hotplug sysfs failed\n");
		mem_offline_record_fail_stat(MEMORY_OFFLINE);
		return ret;
	}

	for (section_nr = start_section_nr; section_nr <= end_section_nr;
			section_nr += sections_per_block) {
		if (!present_section_nr(section_nr))
			continue;

		mem = find_memory_block(__nr_to_section(section_nr));
		if (!mem) {
			pr_err(MODULE_LOG "section %u find no memory block\n",
				section_nr);
			continue;
		}

		mem_hotplug_begin();
		/*
		 * kobject_uevent will cause a NETLINK wake up event,
		 * so suppress it.
		 */
		dev_set_uevent_suppress(&mem->dev, true);
		ret = device_offline(&mem->dev);
		dev_set_uevent_suppress(&mem->dev, false);
		mem_hotplug_done();
		if (ret < 0) {
			pr_err(MODULE_LOG "section %d device offline failed\n",
				section_nr);
			break;
		}
	}
	unlock_device_hotplug();

	return ret;
}

/*
 * Current we don't want that memory offline suspend/resume fail
 * block the system's sleep or wake up. So no matter what,
 * we return success, but print out the corresponding information.
 */
static int mem_offline_suspend(struct device *dev)
{
	int errcode;

	pr_info(MODULE_LOG "enter %s\n", __func__);
	if (!memory_offline_enable) {
		pr_info(MODULE_LOG "status is disable, no need to offline\n");
		errcode = 1;
		goto out;
	}
	errcode = memory_block_offline_device();

out:
	mem_offline_notify_ddrc_msg(errcode);
	pr_info(MODULE_LOG "out %s\n", __func__);
	return 0;
}

static void memory_block_online_device(void)
{
	struct memory_block *mem = NULL;
	unsigned long section_nr;
	int ret;

	ret = lock_device_hotplug_sysfs();
	if (ret) {
		pr_err(MODULE_LOG "lock device hotplug sysfs failed\n");
		mem_offline_record_fail_stat(MEMORY_ONLINE);
		return;
	}

	for (section_nr = start_section_nr; section_nr <= end_section_nr;
			section_nr += sections_per_block) {
		if (!present_section_nr(section_nr))
			continue;

		mem = find_memory_block(__nr_to_section(section_nr));
		if (!mem) {
			pr_err(MODULE_LOG "section %u find no memory block\n",
				section_nr);
			continue;
		}

		mem_hotplug_begin();
		dev_set_uevent_suppress(&mem->dev, true);
		mem->online_type = MMOP_ONLINE_KEEP;
		ret = device_online(&mem->dev);
		dev_set_uevent_suppress(&mem->dev, false);
		mem_hotplug_done();
		/*
		 * online memory block as much as possible,
		 * those who are already online will return to 1
		 * and without any effect.
		 */
		if (ret < 0)
			pr_err(MODULE_LOG "section %d device online failed\n",
				section_nr);
	}

	unlock_device_hotplug();
}

static int mem_offline_resume(struct device *dev)
{
	pr_info(MODULE_LOG "enter %s\n", __func__);
	if (!memory_offline_enable) {
		pr_info(MODULE_LOG "status is disable, no need to online\n");
		goto out;
	}
	mem_offline_record_prepare_stat(0, MEMORY_ONLINE);
	memory_block_online_device();

out:
	pr_info(MODULE_LOG "out %s\n", __func__);
	return 0;
}

static void mem_offline_selected(struct task_struct *selected,
			unsigned long selected_tasksize)
{
	task_lock(selected);
	send_sig(SIGKILL, selected, 0);
	if (selected->mm)
		task_set_lmk_waiting(selected);
	task_unlock(selected);
	lowmem_deathpending_timeout = jiffies + HZ;
	pr_info(MODULE_LOG "Killing '%s' (%d), tgid=%d, adj %hd"
		  " to free %ldkB on behalf of '%s' (%d)\n",
		  selected->comm, selected->pid, selected->tgid,
		  selected->signal->oom_score_adj,
		  selected_tasksize * (long)(PAGE_SIZE / 1024),
		  current->comm, current->pid);
}

static struct task_struct *mem_offline_selected_kill_task(void)
{
	struct task_struct *tsk = NULL;
	struct task_struct *p = NULL;
	struct task_struct *selected = NULL;
	short oom_score_adj;
	short selected_oom_score_adj = kill_process_adj;
	unsigned long selected_tasksize = 0;
	unsigned long tasksize = 0;

	for_each_process(tsk) {
		if (tsk->flags & PF_KTHREAD)
			continue;

		p = find_lock_task_mm(tsk);
		if (!p)
			continue;

		if (task_lmk_waiting(p)) {
			if (time_after_eq(jiffies,
				lowmem_deathpending_timeout))
				pr_err(MODULE_LOG "Killing '%s' (%d), tgid=%d timeout\n",
				    p->comm, p->pid, p->tgid);
			task_unlock(p);
			continue;
		}

		oom_score_adj = p->signal->oom_score_adj;
		if (oom_score_adj < kill_process_adj) {
			task_unlock(p);
			continue;
		}

		tasksize = get_mm_rss(p->mm);
		task_unlock(p);

		if (tasksize <= 0)
			continue;

		if (selected) {
			if (oom_score_adj < selected_oom_score_adj)
				continue;
			if (oom_score_adj == selected_oom_score_adj &&
				tasksize <= selected_tasksize)
				continue;
		}
		selected = p;
		selected_tasksize = tasksize;
		selected_oom_score_adj = oom_score_adj;
	}

	return selected;
}

static void mem_offline_direct_kill(void)
{
	struct task_struct *selected = NULL;
	unsigned long selected_tasksize = 0;
	int max_retry = 100;

	rcu_read_lock();
	while (1) {
		if (is_enough_free_pages()) {
			pr_info(MODULE_LOG "no need to direct kill to free memory\n");
			break;
		}

		if (!(--max_retry)) {
			pr_err(MODULE_LOG "direct kill timeout\n");
			break;
		}

		selected = mem_offline_selected_kill_task();
		if (selected) {
			selected_tasksize = get_mm_rss(selected->mm);
			mem_offline_selected(selected, selected_tasksize);
			selected = NULL;
		} else {
			pr_err(MODULE_LOG "there is no process to kill...\n");
			break;
		}
	}
	rcu_read_unlock();
}

int selected_task_cmp(const void *a, const void *b)
{
	const struct selected_task *x = a;
	const struct selected_task *y = b;
	int ret;

	ret = x->tasksize < y->tasksize ? -1 : 1;

	return ret;
}

static int test_task_flag(struct task_struct *p, int flag)
{
	struct task_struct *t = p;

	rcu_read_lock();
	for_each_thread(p, t) {
		task_lock(t);
		if (test_tsk_thread_flag(t, flag)) {
			task_unlock(t);
			rcu_read_unlock();
			return 1;
		}
		task_unlock(t);
	}
	rcu_read_unlock();

	return 0;
}

static int mem_offline_select_reclaim_task(struct selected_task *swap_task,
				int page_type)
{
	struct task_struct *tsk = NULL;
	struct task_struct *p = NULL;
	int tast_num = 0;
	int tasksize;
	int i;
	short oom_score_adj;

	rcu_read_lock();
	for_each_process(tsk) {
		if (tsk->flags & PF_KTHREAD)
			continue;

		if (test_task_flag(tsk, TIF_MEMDIE))
			continue;

		p = find_lock_task_mm(tsk);
		if (!p)
			continue;

		oom_score_adj = p->signal->oom_score_adj;

		tasksize = get_mm_counter(p->mm, page_type);
		task_unlock(p);

		if (tasksize <= 0)
			continue;

		if (tast_num == MAX_RECLAIM_TASKS) {
			sort(&swap_task[0], MAX_RECLAIM_TASKS,
					sizeof(struct selected_task),
					&selected_task_cmp, NULL);
			if (tasksize < swap_task[0].tasksize)
				continue;
			swap_task[0].p = p;
			swap_task[0].oom_score_adj = oom_score_adj;
			swap_task[0].tasksize = tasksize;
		} else {
			swap_task[tast_num].p = p;
			swap_task[tast_num].oom_score_adj = oom_score_adj;
			swap_task[tast_num].tasksize = tasksize;
			tast_num++;
		}
	}

	for (i = 0; i < tast_num; i++)
		get_task_struct(swap_task[i].p);

	rcu_read_unlock();

	return tast_num;
}

static bool check_page_valid(struct reclaim_param *rp, struct page *page)
{
	if (!page)
		return false;

	if (page_zonenum(page) == ZONE_DMA)
		return false;

	/* we don't reclaim page in active lru list */
	if (rp->inactive_lru && (PageActive(page) ||
	    PageUnevictable(page)))
		return false;

	if (isolate_lru_page(page))
		return false;

	/*
	 * MADV_FREE clears pte dirty bit and then marks the page
	 * lazyfree (clear SwapBacked). Inbetween if this lazyfreed page
	 * is touched by user then it becomes dirty.  PPR in
	 * shrink_page_list in try_to_unmap finds the page dirty, marks
	 * it back as PageSwapBacked and skips reclaim. This can cause
	 * isolated count mismatch.
	 */
	if (PageAnon(page) && !PageSwapBacked(page)) {
		putback_lru_page(page);
		return false;
	}
	return true;
}

static int mem_offline_reclaim_pte_range(pmd_t *pmd,
	unsigned long addr, unsigned long end, struct mm_walk *walk)
{
	struct reclaim_param *rp = walk->private;
	struct vm_area_struct *vma = rp->vma;
	pte_t *pte = NULL;
	pte_t ptent;
	spinlock_t *ptl = NULL;
	struct page *page = NULL;
	LIST_HEAD(page_list);
	int isolated;
	int reclaimed;

	split_huge_pmd(vma, pmd, addr);
	if (pmd_trans_unstable(pmd) || !rp->nr_to_reclaim)
		return 0;
cont:
	isolated = 0;
	pte = pte_offset_map_lock(vma->vm_mm, pmd, addr, &ptl);
	for (; addr != end; pte++, addr += PAGE_SIZE) {
		ptent = *pte;
		if (!pte_present(ptent))
			continue;

		page = vm_normal_page(vma, addr, ptent);
		if (!check_page_valid(rp, page))
			continue;

		list_add(&page->lru, &page_list);
		inc_node_page_state(page, NR_ISOLATED_ANON +
				page_is_file_cache(page));

		isolated++;
		rp->nr_scanned++;
		if (isolated >= SWAP_CLUSTER_MAX || !rp->nr_to_reclaim)
			break;
	}
	pte_unmap_unlock(pte - 1, ptl);

#ifdef CONFIG_HISI_SWAP_ZDATA
	reclaimed = reclaim_pages_from_list(&page_list, vma,
				false, &rp->nr_writedblock);
#else
	reclaimed = reclaim_pages_from_list(&page_list, vma);
#endif
	rp->nr_reclaimed += reclaimed;
	rp->nr_to_reclaim -= reclaimed;
	if (rp->nr_to_reclaim < 0)
		rp->nr_to_reclaim = 0;

	if (rp->nr_to_reclaim && addr != end)
		goto cont;

	cond_resched();
	return 0;
}

static unsigned long get_movable_mem_filepages(void)
{
	unsigned long filepages;
	struct zone *zones = NODE_DATA(node)->node_zones;

	filepages = zone_page_state(&zones[ZONE_MOVABLE], NR_ZONE_ACTIVE_FILE) +
		zone_page_state(&zones[ZONE_MOVABLE], NR_ZONE_INACTIVE_FILE);

	return filepages;
}

struct reclaim_param reclaim_task_file(struct task_struct *task,
		int nr_to_reclaim)
{
	struct mm_struct *mm = NULL;
	struct vm_area_struct *vma = NULL;
	struct mm_walk reclaim_walk = {};
	struct reclaim_param rp = {
		.nr_to_reclaim = nr_to_reclaim,
		.nr_reclaimed = 0,
#ifdef CONFIG_HISI_SWAP_ZDATA
		.nr_writedblock = 0,
#endif
	};
	int ret;

	get_task_struct(task);
	mm = get_task_mm(task);
	if (!mm)
		goto out;

	reclaim_walk.mm = mm;
	reclaim_walk.pmd_entry = mem_offline_reclaim_pte_range;
	reclaim_walk.private = &rp;

	down_read(&mm->mmap_sem);
	for (vma = mm->mmap; vma; vma = vma->vm_next) {
		if (is_vm_hugetlb_page(vma))
			continue;

		if (!vma->vm_file)
			continue;

		if (!rp.nr_to_reclaim)
			break;

		rp.vma = vma;
		ret = walk_page_range(vma->vm_start, vma->vm_end,
			&reclaim_walk);
		if (ret)
			break;
	}

	flush_tlb_mm(mm);
	up_read(&mm->mmap_sem);
	mmput(mm);
out:
	put_task_struct(task);
	return rp;
}

static void mem_offline_task_reclaim_file(void)
{
	struct selected_task reclaim_task[MAX_RECLAIM_TASKS] = {{0, 0, 0},};
	int task_num, i;
	int total_sz = 0;
	struct reclaim_param rp;
	int total_scan = 0;
	int total_reclaimed = 0;
	ktime_t cur;
	ktime_t delay;

	cur = ktime_get();
	if (get_movable_mem_filepages() <
		(filepages_reclaim_watermark >> PAGE_SHIFT)) {
		pr_err(MODULE_LOG "no need to reclaim task file\n");
		return;
	}

	task_num = mem_offline_select_reclaim_task(reclaim_task, MM_FILEPAGES);
	if (task_num <= 0) {
		pr_err(MODULE_LOG "selected reclaim task is 0\n");
		return;
	}

	for (i = 0; i < task_num; i++)
		total_sz += reclaim_task[i].tasksize;

	if (total_sz == 0) {
		pr_err(MODULE_LOG "total_sz is 0\n");
		return;
	}

	while (task_num--) {
		rp = reclaim_task_file(reclaim_task[task_num].p, INT_MAX);
		total_scan += rp.nr_scanned;
		total_reclaimed += rp.nr_reclaimed;
		if (get_movable_mem_filepages() <
			(filepages_reclaim_watermark >> PAGE_SHIFT)) {
			pr_info(MODULE_LOG "reclaim enough task file and out\n");
			break;
		}
		put_task_struct(reclaim_task[task_num].p);
	}

	delay = ktime_ms_delta(ktime_get(), cur);
	pr_debug(MODULE_LOG "reclaim task file time cost=%lldms, scan=%d, reclaim=%d\n",
			delay, total_scan, total_reclaimed);
}

static int mem_offline_prepare(struct device *dev)
{
	ktime_t cur;
	ktime_t delay;

	pr_info(MODULE_LOG "enter %s\n", __func__);
	if (!memory_offline_enable) {
		pr_info(MODULE_LOG "status is disable, no need to prepare\n");
		goto out;
	}

#ifdef CONFIG_HISI_DEBUG_FS
	show_mem(0, NULL);
#endif
	cur = ktime_get();
	/* step1: kill to free memory */
	mem_offline_direct_kill();

	/* step2: drop cache */
	mem_offline_drop_pagecache();
	drop_slab();

	/* step3: task reclaim */
	mem_offline_task_reclaim_file();

	delay = ktime_ms_delta(ktime_get(), cur);
	mem_offline_record_prepare_stat(delay, MEMORY_OFFLINE);
	pr_debug(MODULE_LOG "suspend prepare time cost=%lldms\n", delay);

#ifdef CONFIG_HISI_DEBUG_FS
	show_mem(0, NULL);
#endif

out:
	pr_info(MODULE_LOG "out %s\n", __func__);
	return 0;
}

static void mem_offline_notify_ddrc_init(void)
{
	sctrl_base = ioremap_wc((phys_addr_t)SOC_ACPU_SCTRL_BASE_ADDR,
			sizeof(unsigned long));
}

static ssize_t mem_offline_perf_stats_show(struct kobject *kobj,
				struct kobj_attribute *attr, char *buf)
{
	unsigned int blk_start = start_section_nr / sections_per_block;
	unsigned int blk_end = end_section_nr / sections_per_block;
	unsigned int idx = blk_end - blk_start + 1;
	unsigned int char_count = 0;
	unsigned int i, j;

	for (i = 0; i <= blk_end - blk_start; i++) {
		for (j = 0; j < MAX_STATE; j++) {
			char_count += snprintf(buf + char_count, BUF_LEN,
				"\n%s%d %s\t\tSUCCESS COUNT:%lu\t\tFAIL COUNT:%lu\t\t\n",
				"MEM", blk_start + i,
				j == 0 ? "ONLINE" : "OFFLINE",
				mem_info[i + j * idx].success_count,
				mem_info[i + j * idx].fail_count);
			char_count += snprintf(buf + char_count, BUF_LEN, "\n");

			char_count += snprintf(buf + char_count, BUF_LEN,
				"Best time\t\tPrepare time\t\tMigrate time\t\t\t\n");

			char_count += snprintf(buf + char_count, BUF_LEN,
				"%lums\t\t\t%lums\t\t\t%lums\t\t\t\n",
				mem_info[i + j * idx].best_time,
				mem_info[i + j * idx].best_prepare_time,
				mem_info[i + j * idx].best_migrate_time);
			char_count += snprintf(buf + char_count, BUF_LEN, "\n");

			char_count += snprintf(buf + char_count, BUF_LEN,
				"Worst time\t\tPrepare time\t\tMigrate time\t\t\t\n");

			char_count += snprintf(buf + char_count, BUF_LEN,
				"%lums\t\t\t%lums\t\t\t%lums\t\t\t\n",
				mem_info[i + j * idx].worst_time,
				mem_info[i + j * idx].worst_prepare_time,
				mem_info[i + j * idx].worst_migrate_time);
			char_count += snprintf(buf + char_count, BUF_LEN, "\n");

			char_count += snprintf(buf + char_count, BUF_LEN,
				"Avg time\t\tPrepare time\t\tCopy time\t\t\t\n");

			char_count += snprintf(buf + char_count, BUF_LEN,
				"%lums\t\t\t%lums\t\t\t%lums\t\t\t\n",
				mem_info[i + j * idx].avg_time,
				mem_info[i + j * idx].avg_prepare_time,
				mem_info[i + j * idx].avg_migrate_time);
			char_count += snprintf(buf + char_count, BUF_LEN, "\n");
		}
	}

	return char_count;
}

static struct kobj_attribute offline_perf_stats_attr =
	__ATTR(perf_stats, 0444, mem_offline_perf_stats_show, NULL);

static ssize_t mem_offline_enable_show(struct kobject *kobj,
				struct kobj_attribute *attr, char *buf)
{
	return snprintf(buf, BUF_LEN, "%u\n", memory_offline_enable);
}

static ssize_t mem_offline_enable_store(struct kobject *kobj,
		struct kobj_attribute *attr, const char *buf, size_t count)
{
	int ret;
	u32 val;

	ret = kstrtou32(buf, 0, &val);
	if (ret)
		return ret;

	/*
	 * memory offline value is 0 or 1.
	 */
	if (val != 0 && val != 1)
		return -EINVAL;

	memory_offline_enable = val;

	return count;
}

static struct kobj_attribute offline_enable_attr =
	__ATTR(enable, 0644, mem_offline_enable_show,
	mem_offline_enable_store);

#ifdef CONFIG_HISI_DEBUG_FS
static ssize_t mem_offline_kill_adj_show(struct kobject *kobj,
				struct kobj_attribute *attr, char *buf)
{
	return snprintf(buf, BUF_LEN, "%d\n", kill_process_adj);
}

static ssize_t mem_offline_kill_adj_store(struct kobject *kobj,
		struct kobj_attribute *attr, const char *buf, size_t count)
{
	int ret;
	int val;

	ret = kstrtos32(buf, 0, &val);
	if (ret)
		return ret;

	/*
	 * max adj is -1000~1000, else is invalid.
	 */
	if (val > 1000 || val < -1000)
		return -EINVAL;

	kill_process_adj = val;

	return count;
}

static struct kobj_attribute offline_kill_adj_attr =
		__ATTR(kill_adj, 0644, mem_offline_kill_adj_show,
		mem_offline_kill_adj_store);

static ssize_t mem_offline_anon_reclaim_watermark_show(struct kobject *kobj,
				struct kobj_attribute *attr, char *buf)
{
	return snprintf(buf, BUF_LEN, "%lu\n", anonpages_reclaim_watermark /
				SZ_1M);
}

static ssize_t mem_offline_anon_reclaim_watermark_store(struct kobject *kobj,
		struct kobj_attribute *attr, const char *buf, size_t count)
{
	int ret;
	u32 val;

	ret = kstrtou32(buf, 0, &val);
	if (ret)
		return ret;

	/*
	 * anon reclaim watermark limit to 100M ~500M.
	 */
	if (val > 500 || val < 100)
		return -EINVAL;

	anonpages_reclaim_watermark = val * SZ_1M;

	return count;
}

static struct kobj_attribute offline_anon_reclaim_watermark_attr =
		__ATTR(anon_reclaim_watermark, 0644,
		mem_offline_anon_reclaim_watermark_show,
		mem_offline_anon_reclaim_watermark_store);

static ssize_t mem_offline_file_reclaim_watermark_show(struct kobject *kobj,
				struct kobj_attribute *attr, char *buf)
{
	return snprintf(buf, BUF_LEN, "%lu\n", filepages_reclaim_watermark /
				SZ_1M);
}

static ssize_t mem_offline_file_reclaim_watermark_store(struct kobject *kobj,
		struct kobj_attribute *attr, const char *buf, size_t count)
{
	int ret;
	u32 val;

	ret = kstrtou32(buf, 0, &val);
	if (ret)
		return ret;

	/*
	 * file reclaim watermark limit to 1M ~500M.
	 */
	if (val > 500 || val < 1)
		return -EINVAL;

	filepages_reclaim_watermark = val * SZ_1M;

	return count;
}

static struct kobj_attribute offline_file_reclaim_watermark_attr =
		__ATTR(file_reclaim_watermark, 0644,
		mem_offline_file_reclaim_watermark_show,
		mem_offline_file_reclaim_watermark_store);
#endif

static struct attribute *mem_root_attrs[] = {
		&offline_perf_stats_attr.attr,
		&offline_enable_attr.attr,
#ifdef CONFIG_HISI_DEBUG_FS
		&offline_kill_adj_attr.attr,
		&offline_anon_reclaim_watermark_attr.attr,
		&offline_file_reclaim_watermark_attr.attr,
#endif
		NULL,
};

static struct attribute_group mem_attr_group = {
	.attrs = mem_root_attrs,
};

static int mem_offline_sysfs_init(void)
{
	unsigned int total_blks = (end_section_nr - start_section_nr + 1) /
				sections_per_block;

	mem_kobj = kobject_create_and_add(MODULE_CLASS_NAME, kernel_kobj);
	if (!mem_kobj)
		return -ENOMEM;

	if (sysfs_create_group(mem_kobj, &mem_attr_group))
		kobject_put(mem_kobj);

	mem_info = kzalloc(sizeof(*mem_info) * total_blks * MAX_STATE,
						GFP_KERNEL);
	if (!mem_info)
		return -ENOMEM;

	return 0;
}

static int mem_offline_hotplug_blocks(void)
{
	unsigned long memblock, pfn;
	unsigned long memblock_start_pfn, memblock_end_pfn;
	unsigned int nid;
	phys_addr_t phys_addr;
	int ret;
	int memblock_num = 0;

	memblock_start_pfn = __phys_to_pfn(offline_memblock_region.base);
	memblock_end_pfn = __phys_to_pfn(offline_memblock_region.base +
					offline_memblock_region.size - 1);

	start_section_nr = pfn_to_section_nr(memblock_start_pfn);
	end_section_nr = pfn_to_section_nr(memblock_end_pfn);

	for (memblock = start_section_nr; memblock <= end_section_nr;
			memblock += sections_per_block) {
		pfn = section_nr_to_pfn(memblock);
		phys_addr = __pfn_to_phys(pfn);
		nid = memory_add_physaddr_to_nid(phys_addr);
		/* if some memblock add fail, ignore it only */
		ret = add_memory(nid, phys_addr,
				 MIN_MEMORY_BLOCK_SIZE * sections_per_block);
		if (ret)
			pr_err(MODULE_LOG "add memory block mem%lu failed\n",
				  memblock);
		else
			memblock_num++;
	}

	return memblock_num;
}

static void mem_offline_swap_fn(void)
{
	struct selected_task swap_task[MAX_RECLAIM_TASKS] = {{0, 0, 0},};
	int task_num, i;
	struct reclaim_param rp;
	int total_scan = 0;
	int total_reclaimed = 0;
	int nr_to_reclaim;
	int total_sz = 0;
	ktime_t cur;
	ktime_t delay;

	cur = ktime_get();
	task_num = mem_offline_select_reclaim_task(swap_task, MM_ANONPAGES);
	if (task_num <= 0) {
		pr_err(MODULE_LOG "selected swap task is 0\n");
		return;
	}

	for (i = 0; i < task_num; i++)
		total_sz += swap_task[i].tasksize;

	if (total_sz == 0) {
		pr_err(MODULE_LOG "total_sz is 0\n");
		return;
	}

	while (task_num--) {
		nr_to_reclaim =
		    (swap_task[task_num].tasksize * PER_SWAP_SIZE) / total_sz;
		if (!nr_to_reclaim)
			nr_to_reclaim = 1;

		rp = reclaim_task_anon(swap_task[task_num].p, nr_to_reclaim);
		total_scan += rp.nr_scanned;
		total_reclaimed += rp.nr_reclaimed;
		put_task_struct(swap_task[task_num].p);
	}

	delay = ktime_ms_delta(ktime_get(), cur);
	pr_debug(MODULE_LOG "anonpages swap time cost=%lldms, scan=%d, reclaim=%d\n",
		delay, total_scan, total_reclaimed);
}

static int mem_offline_swap_thread(void *data)
{
	unsigned long anonpages;

	while (!kthread_should_stop()) {
		anonpages = global_node_page_state(NR_ACTIVE_ANON) +
			global_node_page_state(NR_INACTIVE_ANON);

		if ((anonpages * PAGE_SIZE) >= anonpages_reclaim_watermark) {
#ifdef CONFIG_HISI_DEBUG_FS
			show_mem(0, NULL);
#endif
			mem_offline_swap_fn();
#ifdef CONFIG_HISI_DEBUG_FS
			show_mem(0, NULL);
#endif
		}
		ssleep(ANONPAGES_RECLAIM_PERIOD);
	}

	return 0;
}

static int mem_offline_dt_para_check(phys_addr_t base, phys_addr_t size)
{
	if (!base || !size)
		return -EINVAL;

	if (base & (((PAGES_PER_SECTION * sections_per_block)
			<< PAGE_SHIFT) - 1))
		return -EINVAL;

	if (size & (((PAGES_PER_SECTION * sections_per_block)
			<< PAGE_SHIFT) - 1))
		return -EINVAL;

	return 0;
}

static int mem_offline_parse_dt(struct platform_device *pdev)
{
	struct device_node *node = pdev->dev.of_node;
	phys_addr_t base, size;
	const __be32 *prop = NULL;
	int len;
	int t_len = (dt_root_addr_cells + dt_root_size_cells) * sizeof(__be32);

	prop = of_get_property(node, "reg", &len);
	if (!prop) {
		pr_err(MODULE_LOG "reg property not found in DT\n");
		return -ENOENT;
	}

	if (len && len % t_len != 0) {
		pr_err(MODULE_LOG "invalid reg property\n");
		return -EINVAL;
	}

	base = dt_mem_next_cell(dt_root_addr_cells, &prop);
	size = dt_mem_next_cell(dt_root_size_cells, &prop);

	if (mem_offline_dt_para_check(base, size)) {
		pr_err(MODULE_LOG "invalid offline memory base or size:"
			   " base 0x%llx, size 0x%llx\n", base, size);
		return -EINVAL;
	}

	offline_memblock_region.base = base;
	offline_memblock_region.size = size;

	return 0;
}

static struct notifier_block hotplug_memory_callback_nb = {
	.notifier_call = mem_offline_event_callback,
	.priority = 0,
};

static int mem_offline_driver_probe(struct platform_device *pdev)
{
	unsigned long block_size;
	int ret;
	int memblock_num;
	struct task_struct *reclaim_anonpages_task = NULL;

	block_size = memory_block_size_bytes();
	sections_per_block = block_size / MIN_MEMORY_BLOCK_SIZE;

	ret = mem_offline_parse_dt(pdev);
	if (ret) {
		pr_err(MODULE_LOG "memory dt parse failed\n");
		return -ENODEV;
	}

	reclaim_anonpages_task = kthread_run(mem_offline_swap_thread, NULL,
				    "mem_offline_swap");
	if (IS_ERR(reclaim_anonpages_task)) {
		ret = PTR_ERR(reclaim_anonpages_task);
		pr_err(MODULE_LOG "failed to create reclaim anonpages"
			  " task err %d\n", ret);
		return ret;
	}

	memblock_num = mem_offline_hotplug_blocks();
	if (!memblock_num) {
		pr_err(MODULE_LOG "memory online hotplug blocks failed\n");
		return -ENODEV;
	}

	ret = mem_offline_sysfs_init();
	if (ret) {
		pr_err(MODULE_LOG "memory sysfs init failed\n");
		return -ENODEV;
	}

	mem_offline_notify_ddrc_init();

	ret = register_hotmemory_notifier(&hotplug_memory_callback_nb);
	if (ret) {
		pr_err(MODULE_LOG "registering memory hotplug notifier failed\n");
		return -ENODEV;
	}

	pr_info(MODULE_LOG "added memory blocks ranging from mem%lu - mem%lu\n",
			start_section_nr, end_section_nr);

	return 0;
}

static const struct of_device_id mem_offline_match_table[] = {
	{.compatible = "hisi,mem-offline"},
	{}
};

static const struct dev_pm_ops mem_offline_pm_ops = {
	.prepare	= mem_offline_prepare,
	.suspend	= mem_offline_suspend,
	.resume		= mem_offline_resume,
};

static struct platform_driver mem_offline_driver = {
	.probe = mem_offline_driver_probe,
	.driver = {
		.name = "mem_offline",
		.pm = &mem_offline_pm_ops,
		.of_match_table = mem_offline_match_table,
		.owner = THIS_MODULE,
	},
};

static int __init mem_module_init(void)
{
	return platform_driver_register(&mem_offline_driver);
}

subsys_initcall(mem_module_init);
