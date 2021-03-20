#undef TRACE_SYSTEM
#define TRACE_SYSTEM l3cache_extension

#if !defined(_TRACE_L3CACHE_EXTENSION_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_L3CACHE_EXTENSION_H

#include <linux/tracepoint.h>

TRACE_EVENT(l3extension_switch,
	TP_PROTO(const char *on),
	TP_ARGS(on),
	TP_STRUCT__entry(
		__string(on, on)
	),
	TP_fast_assign(
		__assign_str(on, on);
	),

	TP_printk("switch = %s",
		  __get_str(on))
);

TRACE_EVENT(update_polling_info,
	TP_PROTO(unsigned long miss_bw, unsigned int hit_rate,
		 unsigned long delta),
	TP_ARGS(miss_bw, hit_rate, delta),
	TP_STRUCT__entry(
		__field(unsigned long, miss_bw)
		__field(unsigned int, hit_rate)
		__field(unsigned long, delta)
	),
	TP_fast_assign(
		__entry->miss_bw = miss_bw;
		__entry->hit_rate = hit_rate;
		__entry->delta = delta;
	),

	TP_printk("miss_bw = %lu, hit_rate = %u, delta = %lu us",
		  __entry->miss_bw, __entry->hit_rate, __entry->delta)
);

#endif /* _TRACE_L3CACHE_EXTENSION_H */

/* This part must be outside protection */
#include <trace/define_trace.h>
