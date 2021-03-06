

#ifndef _TCP_CROSSLAYER_H
#define _TCP_CROSSLAYER_H

#include <linux/skbuff.h>
#include <linux/types.h>
#include <linux/time.h>
#include <linux/timex.h>
#include <net/sock.h>
#include <net/netlink.h>
#include <net/tcp.h>

#ifdef CONFIG_HW_CROSSLAYER_OPT

#define ASPEN_MONITOR_BUF_MAX 128
#define ASPEN_MONITOR_RANGE_COUNT 2
#define ASPEN_MONITOR_TYPE_COUNT 4

#define NS_TO_US 1000
#define NS_TO_S 1000000000
#define HASH_MASK 32
#define HASH_SCALE 18
#define HASH_HIGH_LIMIT 8192UL
#define COPY_LEN 4
#define RTT_BIT 3

enum {
	ASPEN_EMERG = 1,
	ASPEN_ALERT,
	ASPEN_CRIT,
	ASPEN_ERR,
	ASPEN_WARNING,
	ASPEN_NOTICE,
	ASPEN_INFO,
	ASPEN_DEBUG,
	ASPEN_ALL
};

enum tcp_undo_from_state {
	FROM_RECOVERY = 0,
	FROM_LOSS = 1
};

enum aspen_monitor_ports_enum {
	PORT_5000 = 5000,
	PORT_5001 = 5001,
	PORT_BUTT
};

extern unsigned int aspen_log_level;

#define aspen_emerg(msg, ...) do { if (aspen_log_level >= ASPEN_EMERG) \
	printk(KERN_EMERG "[%s:%d %s]: "msg"\n", kbasename(__FILE__), \
		__LINE__, __func__, ## __VA_ARGS__); } while (0)

#define aspen_alert(msg, ...) do { if (aspen_log_level >= ASPEN_ALERT) \
	printk(KERN_ALERT "[%s:%d %s]: "msg"\n", kbasename(__FILE__), \
		__LINE__, __func__, ## __VA_ARGS__); } while (0)

#define aspen_crit(msg, ...) do { if (aspen_log_level >= ASPEN_CRIT) \
	printk(KERN_CRIT "[%s:%d %s]: "msg"\n", kbasename(__FILE__), \
		__LINE__, __func__, ## __VA_ARGS__); } while (0)

#define aspen_err(msg, ...) do { if (aspen_log_level >= ASPEN_ERR) \
	printk(KERN_ERR "[%s:%d %s]: "msg"\n", kbasename(__FILE__), \
		__LINE__, __func__, ## __VA_ARGS__); } while (0)

#define aspen_warning(msg, ...) do { if (aspen_log_level >= ASPEN_WARNING) \
	printk(KERN_WARNING "[%s:%d %s]: "msg"\n", kbasename(__FILE__), \
		__LINE__, __func__, ## __VA_ARGS__); } while (0)

#ifdef CONFIG_HW_CROSSLAYER_OPT_DBG_MODULE
#define aspen_notice(msg, ...) do { if (aspen_log_level >= ASPEN_NOTICE) \
	printk(KERN_NOTICE "[%s:%d %s]: "msg"\n", kbasename(__FILE__), \
		__LINE__, __func__, ## __VA_ARGS__); } while (0)

#define aspen_info(msg, ...) do { if (aspen_log_level >= ASPEN_INFO) \
	printk(KERN_INFO "[%s:%d %s]: "msg"\n", kbasename(__FILE__), \
		__LINE__, __func__, ## __VA_ARGS__); } while (0)

#define aspen_debug(msg, ...) do { if (aspen_log_level >= ASPEN_DEBUG) \
	printk(KERN_DEBUG "[%s:%d %s]: "msg"\n", kbasename(__FILE__), \
		__LINE__, __func__, ## __VA_ARGS__); } while (0)
#else /* CONFIG_HW_CROSSLAYER_OPT_DBG_MODULE */
#define aspen_notice(msg, ...) do { } while (0)

#define aspen_info(msg, ...) do { } while (0)

#define aspen_debug(msg, ...) do { } while (0)
#endif /* CONFIG_HW_CROSSLAYER_OPT_DBG_MODULE */

/* Switch of crosslayer dropped notification */
extern int aspen_tcp_cdn;

struct aspen_cdn_info {
	unsigned int seq;
	unsigned short port;
	unsigned short type;
};

struct aspen_cdn_hashtable {
	struct hlist_head head;
	spinlock_t lock;
};

struct aspen_cdn_hashbucket {
	struct hlist_node node;
	struct sock *sk;
	int refs;
	unsigned short port;
};

#ifdef CONFIG_HW_CROSSLAYER_OPT_DBG_MODULE
struct tcp_sender_monitor {
	u32 version;
	u32 snd_id;
	struct timeval tv;
	u64 uptime_sec;
	u32 uptime_usec;
	char type[ASPEN_MONITOR_TYPE_COUNT];
	u32 saddr;
	u32 daddr;
	u16 sport;
	u16 dport;
	u32 seq;
	u32 end_seq;
	u32 start_seq;
	u32 tsval;
	u32 tsecr;
	u32 cwnd;
	u32 cwnd_remain;
	u32 ssthresh;
	u32 snd_wnd_remain;
	u32 fast_rexmit_cnts;
	u32 timeout_rexmit_cnts;
	u32 nowrtt;
	u32 rto;
	u32 mss_cache;
	u32 undo_modem_drop_marker;
	u32 modem_drop_rexmit_cnts;
	u32 undo_modem_drop_cnts;
	u8 ca_state;
};

struct aspen_monitor {
	atomic_t cdn_hash_slub_cnts;
	atomic_t cdn_drop_slub_cnts;
	atomic_t cdn_sock_hold_cnts;
	atomic_t cdn_sock_put_cnts;
};

struct aspen_netlink_entry {
	int userland_pid;
	int (*callback_send_to_userland) (struct tcp_sender_monitor *msg);
};

/* Aspen netlink entry */
extern struct aspen_netlink_entry aspen_nl_entry;

extern struct monitor_ports {
	seqlock_t lock;
	int range[ASPEN_MONITOR_RANGE_COUNT];
} aspen_monitor_ports;
#endif /* CONFIG_HW_CROSSLAYER_OPT_DBG_MODULE */

extern int aspen_init_hashtable(void);
extern void aspen_mark_hashtable(struct sock *sk, unsigned short int port);
extern void aspen_unmark_hashtable(struct sock *sk);
extern void aspen_crosslayer_recovery(void *ptr, int length);
extern void aspen_tcp_crosslayer_retransmit(struct sock *sk);
extern bool aspen_tcp_try_undo_modem_drop(struct sock *sk,
	enum tcp_undo_from_state state);
#ifdef CONFIG_HW_CROSSLAYER_OPT_DBG_MODULE
#define aspen_inc_to_rexmit_cnts(sk) ((sk)->timeout_rexmit_cnts++)
#define aspen_inc_fast_rexmit_cnts(sk) ((sk)->fast_rexmit_cnts++)
#define aspen_set_rtt_us(sk) ((sk)->nowrtt = (u32)seq_rtt_us)
extern int proc_aspen_monitor_port_range(struct ctl_table *table, int write,
	void __user *buffer, size_t *lenp, loff_t *ppos);
extern int proc_aspen_monitor_info(struct ctl_table *ctl, int write,
	void __user *buffer, size_t *lenp, loff_t *ppos);
extern void aspen_collect_tcp_sender_monitor(struct sock *sk,
	struct sk_buff *skb, u32 tsval, u32 tsecr,
	u32 cwnd_remain, u32 snd_wnd_remain);
#endif /* CONFIG_HW_CROSSLAYER_OPT_DBG_MODULE */
#endif /* CONFIG_HW_CROSSLAYER_OPT */
#endif

