#include <linux/slab.h>
#include <linux/suspend.h>
#include <linux/wakeup_reason.h>
#include <net/ip.h>
#include <uapi/linux/udp.h>

#include <huawei_platform/log/hwlog_kernel.h>

#include "buffered_log_sender.h"
#include "dubai_utils.h"

#define MAX_WS_NAME_LEN						64
#define MAX_WS_NAME_COUNT					128
#define WAKEUP_TAG_SIZE						48
#define WAKEUP_MSG_SIZE						128
#define WAKEUP_SOURCE_SIZE					32
#define KERNEL_WAKEUP_TAG					"DUBAI_TAG_KERNEL_WAKEUP"
#define AP_IRQ_TAG							"DUBAI_TAG_AP_WAKE_IRQ"
#define APP_WAKEUP_DELAY_MS					500

struct dubai_ws_lasting_name {
	char name[MAX_WS_NAME_LEN];
} __packed;

struct dubai_ws_lasting_transmit {
	long long timestamp;
	int count;
	char data[0];
} __packed;

struct dubai_rtc_timer_info {
	char name[TASK_COMM_LEN];
	pid_t pid;
} __packed;

struct dubai_wakeup_entry {
	char tag[WAKEUP_TAG_SIZE];
	char msg[WAKEUP_MSG_SIZE];
	struct list_head list;
};

static LIST_HEAD(wakeup_list);
static struct dubai_rtc_timer_info rtc_timer_info;
static char suspend_abort_reason[MAX_SUSPEND_ABORT_LEN];
static const char *hisi_rtc_wakeup_source[] = {"RTC0", "intr_rtc0"};
static const char *mtk_rtc_wakeup_source[] = {"R12_EINT_EVENT_B"};
static atomic_t rtc_irq_state = ATOMIC_INIT(0);

#ifdef CONFIG_HISI_TIME
static unsigned char last_wakeup_source[WAKEUP_SOURCE_SIZE];
static unsigned long last_app_wakeup_time;
static unsigned long last_wakeup_time;
static int last_wakeup_gpio;
#endif /* CONFIG_HISI_TIME */

void dubai_log_packet_wakeup_stats(const char *tag, const char *key, int value)
{
	if (!dubai_is_beta_user() || !tag || !key)
		return;

	HWDUBAI_LOGE(tag, "%s=%d", key, value);
}
EXPORT_SYMBOL(dubai_log_packet_wakeup_stats);

static void dubai_parse_skb(const struct sk_buff *skb, int *protocol, int *port)
{
	const struct iphdr *iph = NULL;
	struct udphdr hdr;
	struct udphdr *hp = NULL;

	if (!skb || !protocol || !port)
		return;

	iph = ip_hdr(skb);
	if (!iph)
		return;

	*protocol = iph->protocol;
	if ((iph->protocol == IPPROTO_TCP) || (iph->protocol == IPPROTO_UDP)) {
		hp = skb_header_pointer(skb, ip_hdrlen(skb), sizeof(hdr), &hdr);
		if (!hp)
			return;
		*port = (int)ntohs(hp->dest);
	}
}

#ifdef CONFIG_HISI_TIME
static void dubai_set_wakeup_time(const char *source, int gpio)
{
	if (!dubai_is_beta_user() || !source)
		return;

	memset(last_wakeup_source, 0, sizeof(unsigned char));
	strncpy(last_wakeup_source, source, WAKEUP_SOURCE_SIZE - 1);
	last_wakeup_gpio = gpio;
	last_wakeup_time = hisi_getcurtime() / NSEC_PER_MSEC;
}

void dubai_send_app_wakeup(const struct sk_buff *skb, unsigned int hook, unsigned int pf, int uid, int pid)
{
	int protocol = -1;
	int port = -1;

	if (!dubai_is_beta_user())
		return;

	if (!skb || (pf != NFPROTO_IPV4) || (uid < 0) || (pid < 0))
		return;

	if ((last_wakeup_time <= 0)
		|| (last_wakeup_time == last_app_wakeup_time)
		|| ((hisi_getcurtime() / NSEC_PER_MSEC - last_wakeup_time) > APP_WAKEUP_DELAY_MS))
		return;

	last_app_wakeup_time = last_wakeup_time;

	dubai_parse_skb(skb, &protocol, &port);
	if ((protocol < 0) || (port < 0))
		return;

	HWDUBAI_LOGE("DUBAI_TAG_APP_WAKEUP", "uid=%d pid=%d protocol=%d port=%d source=%s gpio=%d",
		uid, pid, protocol, port, last_wakeup_source, last_wakeup_gpio);
}
EXPORT_SYMBOL(dubai_send_app_wakeup);
#else /* !CONFIG_HISI_TIME */
static void dubai_set_wakeup_time(const char __always_unused *source, int __always_unused gpio)
{
}

void dubai_send_app_wakeup(const struct sk_buff __always_unused *skb, unsigned int __always_unused hook,
	unsigned int __always_unused pf, int __always_unused uid, int __always_unused pid)
{
}
EXPORT_SYMBOL(dubai_send_app_wakeup);
#endif /* !CONFIG_HISI_TIME */

/*
 * Caution: It's dangerous to use HWDUBAI_LOG in this function,
 * because it's in the SR process, and the HWDUBAI_LOG will wake up the kworker thread that will open irq
 */
void dubai_update_suspend_abort_reason(const char *reason)
{
	size_t suspend_abort_reason_len, new_reason_len;

	if (!reason)
		return;

	suspend_abort_reason_len = strlen(suspend_abort_reason);
	new_reason_len = strlen(reason);
	if ((suspend_abort_reason_len + new_reason_len) >= (MAX_SUSPEND_ABORT_LEN - 1))
		return;

	strncat(suspend_abort_reason, reason, new_reason_len);
	suspend_abort_reason[suspend_abort_reason_len + new_reason_len] = ' ';
	suspend_abort_reason[MAX_SUSPEND_ABORT_LEN - 1] = '\0';
}

/*
 * Caution: It's dangerous to use HWDUBAI_LOG in this function,
 * because it's in the SR process, and the HWDUBAI_LOG will wake up the kworker thread that will open irq
 */
void dubai_update_wakeup_info(const char *tag, const char *fmt, ...)
{
	va_list args;
	struct dubai_wakeup_entry *entry = NULL;
	const char *source = NULL;
	int gpio = -1;

	if (!tag || (strlen(tag) >= WAKEUP_TAG_SIZE)) {
		dubai_err("Invalid parameter");
		return;
	}

	entry = kzalloc(sizeof(struct dubai_wakeup_entry), GFP_ATOMIC);
	if (!entry) {
		dubai_err("Failed to allocate memory");
		return;
	}

	strncpy(entry->tag, tag, WAKEUP_TAG_SIZE - 1);
	va_start(args, fmt);
	vscnprintf(entry->msg, WAKEUP_MSG_SIZE, fmt, args);
	list_add_tail(&entry->list, &wakeup_list);
	if (!strcmp(entry->tag, KERNEL_WAKEUP_TAG)) {
		source = va_arg(args, const char *);
		gpio = va_arg(args, int);
		dubai_set_wakeup_time(source, gpio);
	}
	va_end(args);

	atomic_set(&rtc_irq_state, 0);
}
EXPORT_SYMBOL(dubai_update_wakeup_info);

void dubai_handle_rtc_irq(void)
{
	atomic_set(&rtc_irq_state, 1);
}
EXPORT_SYMBOL(dubai_handle_rtc_irq);

void dubai_set_rtc_timer(const char *name, int pid)
{
	if (!name || (pid <= 0)) {
		dubai_err("Invalid parameter, pid is %d", pid);
		return;
	}

	memcpy(rtc_timer_info.name, name, TASK_COMM_LEN - 1);
	rtc_timer_info.pid = pid;
}
EXPORT_SYMBOL(dubai_set_rtc_timer);

int dubai_get_ws_lasting_name(void __user * argp)
{
	int ret = 0;
	long long timestamp, size = 0;
	char *data = NULL;
	struct dubai_ws_lasting_transmit *transmit = NULL;
	struct buffered_log_entry *log_entry = NULL;

	ret = get_timestamp_value(argp, &timestamp);
	if (ret < 0) {
		dubai_err("Failed to get timestamp");
		return ret;
	}

	size = cal_log_entry_len(struct dubai_ws_lasting_transmit, struct dubai_ws_lasting_name, MAX_WS_NAME_COUNT);
	log_entry = create_buffered_log_entry(size, BUFFERED_LOG_MAGIC_WS_LASTING_NAME);
	if (!log_entry) {
		dubai_err("Failed to create buffered log entry");
		ret = -ENOMEM;
		return ret;
	}
	transmit = (struct dubai_ws_lasting_transmit *)log_entry->data;
	transmit->timestamp = timestamp;
	transmit->count = MAX_WS_NAME_COUNT;
	data = transmit->data;

	ret = wakeup_source_getlastingname(data, MAX_WS_NAME_LEN, MAX_WS_NAME_COUNT);
	if ((ret <= 0) || (ret > MAX_WS_NAME_COUNT)) {
		dubai_err("Fail to call wakeup_source_getlastingname.");
		ret = -ENOMEM;
		goto exit;
	}

	transmit->count = ret;
	log_entry->length = cal_log_entry_len(struct dubai_ws_lasting_transmit,
				struct dubai_ws_lasting_name, transmit->count);
	ret = send_buffered_log(log_entry);
	if (ret < 0)
		dubai_err("Failed to send wakeup source log entry");

exit:
	free_buffered_log_entry(log_entry);

	return ret;
}

static bool is_hisi_rtc_wakeup(const struct dubai_wakeup_entry *entry)
{
	size_t i;

	if (!strcmp(entry->tag, KERNEL_WAKEUP_TAG) && (rtc_timer_info.pid > 0)) {
		for (i = 0; i < sizeof(hisi_rtc_wakeup_source) / sizeof(const char *); i++) {
			if (strstr(entry->msg, hisi_rtc_wakeup_source[i]))
				return true;
		}
	}

	return false;
}

static bool is_mtk_rtc_wakeup(const struct dubai_wakeup_entry *entry)
{
	size_t i;

	if (!strcmp(entry->tag, AP_IRQ_TAG) && atomic_read(&rtc_irq_state) && (rtc_timer_info.pid > 0)) {
		for (i = 0; i < sizeof(mtk_rtc_wakeup_source) / sizeof(const char *); i++) {
			if (strstr(entry->msg, mtk_rtc_wakeup_source[i]))
				return true;
		}
	}

	return false;
}

static void dubai_send_wakeup_info(void)
{
	struct dubai_wakeup_entry *entry = NULL, *tmp = NULL;

	list_for_each_entry_safe(entry, tmp, &wakeup_list, list) {
		if (is_hisi_rtc_wakeup(entry)) {
			HWDUBAI_LOGE("DUBAI_TAG_RTC_WAKEUP", "name=%s pid=%d", rtc_timer_info.name, rtc_timer_info.pid);
		} else if (is_mtk_rtc_wakeup(entry)) {
			HWDUBAI_LOGE("DUBAI_TAG_RTC_WAKEUP", "name=%s pid=%d", rtc_timer_info.name, rtc_timer_info.pid);
			HWDUBAI_LOGE(AP_IRQ_TAG, "name=RTC0"); // also log ap irq to dubaid
		} else {
			HWDUBAI_LOGE(entry->tag, "%s", entry->msg);
		}
		list_del_init(&entry->list);
		kfree(entry);
	}
	memset(&rtc_timer_info, 0, sizeof(struct dubai_rtc_timer_info));
}

static void dubai_send_suspend_abort_reason(void)
{
	if (strlen(suspend_abort_reason))
		HWDUBAI_LOGE("DUBAI_TAG_SUSPEND_ABORT", "reason=%s", suspend_abort_reason);
	memset(suspend_abort_reason, 0, MAX_SUSPEND_ABORT_LEN);
}

static int dubai_pm_notify(struct notifier_block __always_unused *nb, unsigned long mode, void __always_unused *data)
{
	switch (mode) {
	case PM_SUSPEND_PREPARE:
		HWDUBAI_LOGE("DUBAI_TAG_KERNEL_SUSPEND", "");
		break;
	case PM_POST_SUSPEND:
		dubai_send_suspend_abort_reason();
		dubai_send_wakeup_info();
		HWDUBAI_LOGE("DUBAI_TAG_KERNEL_RESUME", "");
		break;
	default:
		break;
	}

	return 0;
}

static struct notifier_block dubai_pm_nb = {
	.notifier_call = dubai_pm_notify,
};

void dubai_sr_stats_init(void)
{
	memset(&rtc_timer_info, 0, sizeof(struct dubai_rtc_timer_info));
	register_pm_notifier(&dubai_pm_nb);
}

void dubai_sr_stats_exit(void)
{
	struct dubai_wakeup_entry *wakeup = NULL, *wakeup_temp = NULL;

	list_for_each_entry_safe(wakeup, wakeup_temp, &wakeup_list, list) {
		list_del_init(&wakeup->list);
		kfree(wakeup);
	}

	unregister_pm_notifier(&dubai_pm_nb);
}
