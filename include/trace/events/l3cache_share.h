/*
 * l3_share.h
 *
 * L3Cache share
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
#define TRACE_SYSTEM l3cache_share

#if !defined(_TRACE_L3CACHE_SHARE_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_L3CACHE_SHARE_H

#include <linux/tracepoint.h>

TRACE_EVENT(l3_cache_request_enter,
	TP_PROTO(int request_id, unsigned int size, unsigned int g_acp_flag),
	TP_ARGS(request_id, size, g_acp_flag),
	TP_STRUCT__entry(
		__field(int, request_id)
		__field(unsigned int, size)
		__field(unsigned int, g_acp_flag)
	),
	TP_fast_assign(
		__entry->request_id = request_id;
		__entry->size = size;
		__entry->g_acp_flag = g_acp_flag;
	),

	TP_printk("enter request id=%d size=%d g_acp_flag=0x%x",
		  __entry->request_id, __entry->size, __entry->g_acp_flag)
);

TRACE_EVENT(l3_cache_request_succ,
	TP_PROTO(int request_id, unsigned int g_acp_flag),
	TP_ARGS(request_id, g_acp_flag),
	TP_STRUCT__entry(
		__field(int, request_id)
		__field(unsigned int, g_acp_flag)
	),
	TP_fast_assign(
		__entry->request_id = request_id;
		__entry->g_acp_flag = g_acp_flag;
	),

	TP_printk("succ request id=%d g_acp_flag=0x%x",
		  __entry->request_id, __entry->g_acp_flag)
);

TRACE_EVENT(l3_cache_release_enter,
	TP_PROTO(int request_id, unsigned int g_acp_flag),
	TP_ARGS(request_id, g_acp_flag),
	TP_STRUCT__entry(
		__field(int, request_id)
		__field(unsigned int, g_acp_flag)
	),
	TP_fast_assign(
		__entry->request_id = request_id;
		__entry->g_acp_flag = g_acp_flag;
	),

	TP_printk("enter release id=%d g_acp_flag=0x%x",
		  __entry->request_id, __entry->g_acp_flag)
);

TRACE_EVENT(l3_cache_release_succ,
	TP_PROTO(int request_id, unsigned int g_acp_flag),
	TP_ARGS(request_id, g_acp_flag),
	TP_STRUCT__entry(
		__field(int, request_id)
		__field(unsigned int, g_acp_flag)
	),
	TP_fast_assign(
		__entry->request_id = request_id;
		__entry->g_acp_flag = g_acp_flag;
	),

	TP_printk("succ release id=%d g_acp_flag=0x%x",
		  __entry->request_id, __entry->g_acp_flag)
);

TRACE_EVENT(l3_cache_share_perf,
	TP_PROTO(const char *acp, int request_id, unsigned int g_acp_flag),
	TP_ARGS(acp, request_id, g_acp_flag),
	TP_STRUCT__entry(
		__string(acp, acp)
		__field(int, request_id)
		__field(unsigned int, g_acp_flag)
	),
	TP_fast_assign(
		__assign_str(acp, acp);
		__entry->request_id = request_id;
		__entry->g_acp_flag = g_acp_flag;
	),

	TP_printk("acp=%s id=%d g_acp_flag=0x%x",
		  __get_str(acp), __entry->request_id, __entry->g_acp_flag)
);

#endif /* _TRACE_L3CACHE_SHARE_H */

/* This part must be outside protection */
#include <trace/define_trace.h>
