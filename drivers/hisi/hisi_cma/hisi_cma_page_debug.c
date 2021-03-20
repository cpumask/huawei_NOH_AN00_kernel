#define pr_fmt(fmt) "hisi_cma_page_debug: " fmt

#include <linux/mm.h>
#include <linux/cma.h>
#include <linux/slab.h>
#include <linux/err.h>
#include <linux/sizes.h>
#include <linux/kernel.h>
#include <linux/sched/task.h>
#include <log/log_usertype.h>
#include <linux/hisi/util.h>
#include <linux/hisi/hisi_cma_debug.h>
#include <linux/of_fdt.h>


#include "cma.h"
#include "internal.h"

#define pfn_to_trace(pfn, traced_cma) ((traced_cma)->cma_traced_pages \
		+ ((pfn) - (traced_cma)->cma_spfn))
#define CMA_TRACE_PAGE_MAX_CNT 32768
static DEFINE_MUTEX(cma_debug_lock);
static LIST_HEAD(cma_debug_list);

static struct cma_debug_traced_cma *within_trace_cma(unsigned long pfn)
{
	struct list_head *p = NULL;
	struct list_head *n = NULL;
	struct cma_debug_traced_cma *traced_cma = NULL;

	list_for_each_safe(p, n, &cma_debug_list) {
		traced_cma = list_entry(p, struct cma_debug_traced_cma,
				cma_debug_node);
		if (!traced_cma)
			continue;

		if (pfn >= traced_cma->cma_spfn && pfn < traced_cma->cma_epfn)
			return traced_cma;
	}

	return NULL;
}

static inline u64 cma_get_and_add_curobj(struct cma_debug_page *map)
{
	u64 ret = (u64)atomic64_read(&map->cur_obj);

	atomic64_inc(&map->cur_obj);
	return ret;
}

void cma_set_flag(struct cma *cma, unsigned long node)
{
	if (check_himntn(HIMNTN_CMA_TRACE) &&
		of_get_flat_dt_prop(node, "cma-traced", NULL))
		cma->flag = true;
	else
		cma->flag = false;
}

void cma_add_debug_list(struct cma *cma)
{
	struct cma_debug_page *cma_debug_pages = NULL;
	struct cma_debug_traced_cma *traced_cma = NULL;
	unsigned long trace_pgcnt;
	size_t dbg_info_s;

	if (!get_himntn_cma_trace_flag())
		return;

	trace_pgcnt = min_t(unsigned long, CMA_TRACE_PAGE_MAX_CNT, cma->count);
	dbg_info_s = trace_pgcnt * sizeof(struct cma_debug_page);
	cma_debug_pages = vzalloc(dbg_info_s);
	if (!cma_debug_pages)
		return;

	traced_cma = vzalloc(sizeof(struct cma_debug_traced_cma));
	if (!traced_cma) {
		vfree(cma_debug_pages);
		return;
	}

	traced_cma->cma_epfn = cma->base_pfn + trace_pgcnt;
	traced_cma->cma_spfn = cma->base_pfn;
	traced_cma->cma_traced_pages = cma_debug_pages;
	mutex_lock(&cma_debug_lock);
	list_add(&traced_cma->cma_debug_node, &cma_debug_list);
	mutex_unlock(&cma_debug_lock);
	pr_err("add cma trace [%lx~%lx] 0x%zx 0x%zx 0x%zx\n",
		traced_cma->cma_spfn,
		traced_cma->cma_epfn, dbg_info_s,
		sizeof(struct cma_debug_page),
		sizeof(struct cma_debug_task));
}

static inline int cma_get_task_count(struct cma_debug_task *entry)
{
	return atomic_read(&entry->count);
}

static int cma_task_is_saved_and_set_count(unsigned long pfn,
						  struct cma_debug_page *map,
						  int nr,
						  int set_flag)
{
	int i = 0;
	int flag = false;

	if (set_flag) {
		/* task set page_flag , clear all other tasks' trace count */
		for (i = 0; i < HISI_CMA_TRACE_MASTER_MAX; i++) {
			if (!flag && !strncmp(current->comm,
						map->entries[i].comm,
						TASK_COMM_LEN)) {
				atomic_set(&map->entries[i].count, nr);
				flag = true;
			} else {
				/* set other tasks' count to zero */
				atomic_set(&map->entries[i].count, 0);
			}
		}
	} else {
		for (i = 0; i < HISI_CMA_TRACE_MASTER_MAX; i++) {
			if (!strncmp(current->comm,
					map->entries[i].comm,
					TASK_COMM_LEN)) {
				atomic_add(nr, &map->entries[i].count);
				return true;
			}
		}
	}

	return flag;
}

static void cma_debug_clear_page_tsk_count(unsigned long pfn)
{
	struct cma_debug_page *map = NULL;
	struct cma_debug_traced_cma *traced_cma = NULL;
	int i = 0;

	traced_cma = within_trace_cma(pfn);
	if (!traced_cma)
		return;

	map = pfn_to_trace(pfn, traced_cma);

	for (i = 0; i < HISI_CMA_TRACE_MASTER_MAX; i++)
		atomic_set(&map->entries[i].count, 0);
}

void cma_page_count_stack_saved(struct page *page, int nr, int set_flag)
{
	struct stack_trace trace;
	struct cma_debug_page *map = NULL;
	struct cma_debug_traced_cma *traced_cma = NULL;
	int retry_times = 0;
	u64 obj;
	u64 entry_id;
	unsigned long pfn;

	if (!page)
		return;

	pfn = page_to_pfn(page);

	traced_cma = within_trace_cma(pfn);
	if (!traced_cma)
		return;

	/* clear all tasks' record count when page->_refcount is 0 and return */
	if (!atomic_read(&page->_refcount)) {
		cma_debug_clear_page_tsk_count(pfn);
		return;
	}

	map = pfn_to_trace(pfn, traced_cma);

	/* if task had been saved, only change count */
	if (cma_task_is_saved_and_set_count(pfn, map, nr, set_flag))
		return;

retry:
	obj = cma_get_and_add_curobj(map);
	entry_id = obj % HISI_CMA_TRACE_MASTER_MAX;
	/* try to find nonzero obj to trace */
	if (cma_get_task_count(&map->entries[entry_id]) &&
		retry_times < HISI_CMA_TRACE_MASTER_MAX) {
		retry_times++;
		goto retry;
	}

	map->entries[entry_id].page_pid = current->pid;
	memcpy(map->entries[entry_id].comm, current->comm, TASK_COMM_LEN);
	atomic_set(&map->entries[entry_id].count, nr);
	trace.nr_entries = 0;
	trace.max_entries = HISI_CMA_TRACE_FRAME_MAX;
	trace.entries = (unsigned long *)map->entries[entry_id].func;
	trace.skip = 1;
	save_stack_trace(&trace);
}
EXPORT_SYMBOL(cma_page_count_stack_saved);//lint !e546 !e580

void cma_print_page_stack(unsigned long pfn)
{
	int i, j;
	struct cma_debug_page *map = NULL;
	struct cma_debug_traced_cma *traced_cma = NULL;

	traced_cma = within_trace_cma(pfn);
	if (!traced_cma) {
		pr_err("not in traced cma, pfn:%lu\n", pfn);
		return;
	}

	map = pfn_to_trace(pfn, traced_cma);
	if (!map) {
		pr_err("pfn is not traced,pfn:%lu\n", pfn);
		return;
	}

	pr_err("=========cma_debug pfn[%lu] start:=======\n", pfn);

	for (i = 0; i < HISI_CMA_TRACE_MASTER_MAX; i++) {
		if (!cma_get_task_count(&map->entries[i]))
			continue;

		pr_err("---------------show stack <%d>-------------\n", i);
		pr_err("pfn:%lu, pid:%d, task:%s, cnt:%d, pgcnt:%d, mapcnt:%d, pgflag:0x%lx\n",
				pfn, map->entries[i].page_pid,
				map->entries[i].comm,
				cma_get_task_count(&map->entries[i]),
				atomic_read(&pfn_to_page(pfn)->_refcount),
				page_mapcount(pfn_to_page(pfn)),
				pfn_to_page(pfn)->flags);

		for (j = 0; j < HISI_CMA_TRACE_FRAME_MAX; j++) {
			if ((uintptr_t)map->entries[i].func[j] == (uintptr_t)ULONG_MAX ||
				!(map->entries[i].func[j]))
				break;
			pr_err("%pF\n", map->entries[i].func[j]);
		}
	}

	pr_err("========cma_debug pfn[%lu]end=========\n", pfn);

}

void show_cma_debug_trace(unsigned long s, unsigned long e)
{
	unsigned long pfn;

	for (pfn = s; pfn < e; pfn++)
		cma_print_page_stack(pfn);
	return;
}

void dump_cma_debug_task(struct cma *cma)
{
	unsigned long pfn;

	if (!get_himntn_cma_trace_flag())
		return;

	for (pfn = cma->base_pfn; pfn < (cma->base_pfn + cma->count); pfn++)
		if (PageCmaPin(pfn_to_page(pfn))) {
			cma_print_page_stack(pfn);
			ClearPageCmaPin(pfn_to_page(pfn));
		}
}

