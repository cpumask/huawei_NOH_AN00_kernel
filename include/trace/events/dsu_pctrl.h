/*
 * dsu_pctrl.h
 *
 * ARM DynamIQ Shared Unit (DSU)
 *
 * Copyright (c) 2014-2020 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */
#undef TRACE_SYSTEM
#define TRACE_SYSTEM dsu_pctrl

#if !defined(_TRACE_DSU_PCTRL_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_DSU_PCTRL_H

#include <linux/tracepoint.h>

TRACE_EVENT(dsu_pctrl_dev_status,
	TP_PROTO(int dsu_id, unsigned long hits, unsigned long misses,
		 unsigned long freq, unsigned long busy_time,
		 unsigned long total_time),
	TP_ARGS(dsu_id, hits, misses, freq, busy_time, total_time),
	TP_STRUCT__entry(
		__field(int, dsu_id)
		__field(unsigned long, hits)
		__field(unsigned long, misses)
		__field(unsigned long, freq)
		__field(unsigned long, busy_time)
		__field(unsigned long, total_time)
	),
	TP_fast_assign(
		__entry->dsu_id = dsu_id;
		__entry->hits = hits;
		__entry->misses = misses;
		__entry->freq = freq;
		__entry->busy_time = busy_time;
		__entry->total_time = total_time;
	),

	TP_printk("dsu_dev_id=%d hits=%lu misses=%lu cur_port=%lu busy_time=%lu total_time=%lu",
		  __entry->dsu_id, __entry->hits, __entry->misses,
		  __entry->freq, __entry->busy_time, __entry->total_time)
);

TRACE_EVENT(dsu_pctrl_set_active_portions,
	TP_PROTO(unsigned long portions, unsigned long portion_active),
	TP_ARGS(portions, portion_active),
	TP_STRUCT__entry(
		__field(unsigned long, portions)
		__field(unsigned long, portion_active)
	),
	TP_fast_assign(
		__entry->portions = portions;
		__entry->portion_active = portion_active;
	),

	TP_printk("portions=%lu portion_active=0x%lx",
		  __entry->portions, __entry->portion_active)
);

TRACE_EVENT(dsu_pctrl_read_perf_counters,
	TP_PROTO(int cpu, unsigned long l3d_refill_cnt, unsigned long l3d_cnt),
	TP_ARGS(cpu, l3d_refill_cnt, l3d_cnt),
	TP_STRUCT__entry(
		__field(int, cpu)
		__field(unsigned long, l3d_refill_cnt)
		__field(unsigned long, l3d_cnt)
	),
	TP_fast_assign(
		__entry->cpu = cpu;
		__entry->l3d_refill_cnt = l3d_refill_cnt;
		__entry->l3d_cnt = l3d_cnt;
	),

	TP_printk("cpu=%d l3d_refill_cnt=%lu, l3d_cnt = %lu",
		  __entry->cpu, __entry->l3d_refill_cnt, __entry->l3d_cnt)
);

TRACE_EVENT(dsu_pctrl_get_cnt,
	TP_PROTO(unsigned long total_refill, unsigned long total_access),
	TP_ARGS(total_refill, total_access),
	TP_STRUCT__entry(
		__field(unsigned long, total_refill)
		__field(unsigned long, total_access)
	),
	TP_fast_assign(
		__entry->total_refill = total_refill;
		__entry->total_access = total_access;
	),

	TP_printk("total_refill=%lu total_access=%lu",
		  __entry->total_refill, __entry->total_access)
);

TRACE_EVENT(dsu_pctrl_target_acp,
	TP_PROTO(const char *acp, int id),
	TP_ARGS(acp, id),
	TP_STRUCT__entry(
		__string(acp, acp)
		__field(int, id)
	),
	TP_fast_assign(
		__assign_str(acp, acp);
		__entry->id = id;
	),

	TP_printk("acp status=%s id=%d",
		  __get_str(acp), __entry->id)
);

#endif /* _TRACE_DSU_PCTRL_H */

/* This part must be outside protection */
#include <trace/define_trace.h>
