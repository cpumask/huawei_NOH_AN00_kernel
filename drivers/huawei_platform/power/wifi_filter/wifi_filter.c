


/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/file.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/err.h>
#include <linux/fs.h>
#include <linux/fdtable.h>
#include <linux/tcp.h>

#include <net/sock.h>

#include <net/tcp.h>
#include <net/inet_sock.h>

#include <huawei_platform/power/wifi_filter/wifi_filter.h>
#include <linux/kthread.h>

#include <huawei_platform/connectivity/hw_connectivity.h>
#include <linux/fb.h>

#include <linux/time.h>
#include <linux/timex.h>
#include <linux/rtc.h>
/******************************************************************************
   2 �궨��
******************************************************************************/
#define DOZABLE_NAME    "fw_dozable"
#define WLAN_NAME       "wlan0"
#define ITEM_COUNT_MAX_BRODCOM    (1<<5) //32
#define ITEM_COUNT_MAX_HISI       (1<<6) //64

#define ITEM_SCREENOFF_MAX        (1<<3) //8

#define NETBIOS_PORT              (137)
#define SPECIAL_PORT_NUM          (0)
#define CHIP_TYPE_HI110X          "hisi"
#define LOG_TAG                   "wifi_filter"

#define DEBUG                     1
#define INFO                      1

#define FILTER_LOGD(fmt, ...) \
    do { \
        if (DEBUG) { \
            printk(KERN_DEBUG "["LOG_TAG"] %s: "fmt"\n", __func__, ##__VA_ARGS__); \
        } \
    } while (0)

#define FILTER_LOGI(fmt, ...) \
    do { \
        if (INFO) { \
            printk(KERN_INFO "["LOG_TAG"] %s: "fmt"\n", __func__, ##__VA_ARGS__); \
        } \
    } while (0)

#define FILTER_LOGE(fmt, ...) \
    do { \
        printk(KERN_ERR "["LOG_TAG"] %s: "fmt"\n", __func__, ##__VA_ARGS__); \
    } while (0)
/*****************************************************************************
  3 ��������
*****************************************************************************/
static int hw_add_filter_items(hw_wifi_filter_item *items, int count);
static int hw_clear_all_item(void);

static inline struct ipt_entry *
get_entry(const void *base, unsigned int offset)
{
    return (struct ipt_entry *)(base + offset);
}

static inline const struct xt_entry_target *
ipt_get_target_c(const struct ipt_entry *e)
{
    return ipt_get_target((struct ipt_entry *)e);
}
/******************************************************************************
   4 ˽�ж���
******************************************************************************/
enum WakeCondition{
    WAKE_INIT = 0,
    WAKE_ADD,
    WAKE_SCRN_ON,
    WAKE_SCRN_OFF,
    WAKE_EXIT_DOZE,
    WAKE_ENTER_DOZE,
    WAKE_EXIT_NIGHT,
    WAKE_ENTER_NIGHT
};
enum FilterType{
    TYPE_HEAD = 0,
    TYPE_ICMP = 1,
    TYPE_TAIL
};
static struct task_struct *WifiFilterThread = NULL;
static wait_queue_head_t mythread_wq;
static int wake_up_condition = WAKE_INIT;
static bool bIsSupportWifiFilter = false;
static bool bDozeEnable = false;
static bool bScreenoff = false;
static struct hw_wlan_filter_ops gWlanFilterOps;

static int g_item_cnt_max = 0;

typedef struct {
    unsigned short scrnoff_item_cnt;
    unsigned short scrnoff_index;
    unsigned short doze_item_cnt;
    unsigned short doze_index;
    hw_wifi_filter_item *p_items_data;
    spinlock_t item_lock;
}wifi_filter_item_info;

static wifi_filter_item_info g_item_info;
struct timer_list mytimer;
static bool bIsNight = false;
/******************************************************************************
   5 ȫ�ֱ�������
******************************************************************************/

/******************************************************************************
   6 ����ʵ��
******************************************************************************/
static void timer_callback(unsigned long data)
{
    switch (data) {
        case 0:
            mytimer.data = 1;//1:now is day,0:now is night
            mytimer.expires = jiffies + 18*60*60*HZ;//18 hours before 24:0:0
            wake_up_condition = WAKE_EXIT_NIGHT;
            break;
        case 1:
            mytimer.data = 0;
            mytimer.expires = jiffies + 6*60*60*HZ;//6 hours before 06:0:0
            wake_up_condition = WAKE_ENTER_NIGHT;
            break;
        default:
            break;
    }
    add_timer(&mytimer);
    wake_up_interruptible(&mythread_wq);
}

/*
 * get screen state
 */
static int wifi_filter_fb_notifier_cb(struct notifier_block *self,
    unsigned long event, void *data)
{
    struct fb_event *fb_event = NULL;
    int *blank = NULL;

    if (!bIsSupportWifiFilter) {
        return NOTIFY_DONE;
    }
    if (NULL == data) {
        return NOTIFY_DONE;
    }

    fb_event = data;
    blank = fb_event->data;

    if (NULL == blank) {
        return NOTIFY_DONE;
    }

    if (event == FB_EVENT_BLANK)
    {
        if (*blank == FB_BLANK_UNBLANK){            //screenon
            FILTER_LOGI("screenon");
            bScreenoff = false;
            wake_up_condition = WAKE_SCRN_ON;
            wake_up_interruptible(&mythread_wq);
        }else if (*blank == FB_BLANK_POWERDOWN){    //screenoff
            FILTER_LOGI("screenoff");
            bScreenoff = true;
            wake_up_condition = WAKE_SCRN_OFF;
            wake_up_interruptible(&mythread_wq);
        }else{
            FILTER_LOGI("other ");
        }
    }

    return NOTIFY_DONE;
}

static struct notifier_block wifi_filter_fb_notifier = {
    .notifier_call = wifi_filter_fb_notifier_cb,
};

static void reg_fb_notification(void)
{
    fb_register_client(&wifi_filter_fb_notifier);
}

static void unreg_fb_notification(void)
{
    fb_unregister_client(&wifi_filter_fb_notifier);
}

int hw_register_wlan_filter(struct hw_wlan_filter_ops *ops)
{
    int interval;
    struct timex txc;
    struct rtc_time tm;
    if ( NULL == ops ) {
        return -1;
    }
    spin_lock_init(&g_item_info.item_lock);

    gWlanFilterOps.add_filter_items     = ops->add_filter_items;
    gWlanFilterOps.clear_filters        = ops->clear_filters;
    gWlanFilterOps.get_filter_pkg_stat  = ops->get_filter_pkg_stat;
    gWlanFilterOps.set_filter_enable    = ops->set_filter_enable;
    gWlanFilterOps.set_filter_enable_ex = ops->set_filter_enable_ex;
    /*
     * get wifi chip type
     */
    if (isMyConnectivityChip(CHIP_TYPE_HI110X)) {
        FILTER_LOGD("this is hisi chip");
        g_item_cnt_max = ITEM_COUNT_MAX_HISI;
    } else {
        g_item_cnt_max = ITEM_COUNT_MAX_BRODCOM;
    }
    spin_lock_bh(&g_item_info.item_lock);
    g_item_info.p_items_data = (hw_wifi_filter_item *)kzalloc(
            sizeof(hw_wifi_filter_item)*g_item_cnt_max, GFP_ATOMIC);
    if (NULL == g_item_info.p_items_data) {
        FILTER_LOGE("malloc failed!");
        spin_unlock_bh(&g_item_info.item_lock);
        return -1;
    }
    spin_unlock_bh(&g_item_info.item_lock);
    bIsSupportWifiFilter = true;

    mytimer.function = timer_callback;
    do_gettimeofday(&(txc.time));
	if ( sys_tz.tz_minuteswest == 0 )
		txc.time.tv_sec -= -8*60*60;
    rtc_time_to_tm(txc.time.tv_sec,&tm);
    FILTER_LOGI("local time is %04d-%02d-%02d %02d:%02d:%02d",
        tm.tm_year+1900,tm.tm_mon+1,tm.tm_mday,tm.tm_hour,tm.tm_min,tm.tm_sec);
    if(0<=tm.tm_hour&&tm.tm_hour<6) {
        mytimer.data = 0;
		bIsNight = true;
        interval = 6*60*60-(tm.tm_hour*3600+tm.tm_min*60+tm.tm_sec);
    } else {
        mytimer.data = 1;
		bIsNight = false;
        interval = 24*60*60-(tm.tm_hour*3600+tm.tm_min*60+tm.tm_sec);
    }
    FILTER_LOGE("interval is %d s",interval);
    mytimer.expires = jiffies + interval*HZ;
    add_timer(&mytimer);
    return 0;
}
EXPORT_SYMBOL(hw_register_wlan_filter);

int hw_unregister_wlan_filter()
{
    gWlanFilterOps.add_filter_items     = NULL;
    gWlanFilterOps.clear_filters        = NULL;
    gWlanFilterOps.get_filter_pkg_stat  = NULL;
    gWlanFilterOps.set_filter_enable    = NULL;
    gWlanFilterOps.set_filter_enable_ex = NULL;
    del_timer_sync(&mytimer);
    FILTER_LOGI("hw_unregister_wlan_filter");
    spin_lock_bh(&g_item_info.item_lock);
    if (g_item_info.p_items_data != NULL) {
        kfree(g_item_info.p_items_data);
        g_item_info.p_items_data = NULL;
    }
    bIsSupportWifiFilter = false;
    spin_unlock_bh(&g_item_info.item_lock);
    return 0;
}
EXPORT_SYMBOL(hw_unregister_wlan_filter);

void get_wifi_filter_info(struct sk_buff *skb, hw_wifi_filter_item *pItem)
{
    const struct iphdr *ip = NULL;

    if (NULL == skb) {
        FILTER_LOGE("skb is null");
        return;
    }
    if (NULL == pItem) {
        FILTER_LOGE("pItem is null");
        return;
    }
    ip = ip_hdr(skb);
    if (NULL == ip) {
        FILTER_LOGE("ip is null");
        return;
    }

    if (ip->protocol == IPPROTO_TCP) {
		FILTER_LOGI("tcp dest=%d, source=%d",  ntohs(tcp_hdr(skb)->dest),ntohs(tcp_hdr(skb)->source));
        pItem->port = tcp_hdr(skb)->dest;
        pItem->protocol = IPPROTO_TCP;
        pItem->filter_cnt = 0;
    } else if (ip->protocol == IPPROTO_UDP) {
		FILTER_LOGI("udp dest=%d,source=%d", ntohs(udp_hdr(skb)->dest), ntohs(udp_hdr(skb)->source));
        pItem->port = udp_hdr(skb)->dest;
        pItem->protocol = IPPROTO_UDP;
        pItem->filter_cnt = 0;
    } else {
        printk("other protocol");
        return;
    }
}

static bool add_to_screenoff_table(struct sk_buff *skb)
{
    hw_wifi_filter_item item;

    if (g_item_info.p_items_data == NULL) {
        return false;
    }
    memset(&item, 0, sizeof(hw_wifi_filter_item));

    get_wifi_filter_info(skb, &item);
	FILTER_LOGI("scrnoff_index=%d",g_item_info.scrnoff_index);

	if (g_item_info.scrnoff_index >= ITEM_SCREENOFF_MAX) {
		FILTER_LOGE("add_to_screenoff_table too much!");
		return false;
	}

    memcpy(&g_item_info.p_items_data[g_item_info.scrnoff_index++],
        &item,sizeof(hw_wifi_filter_item));

    if (g_item_info.scrnoff_index < ITEM_SCREENOFF_MAX) {
        if (g_item_info.scrnoff_item_cnt < ITEM_SCREENOFF_MAX) {
            g_item_info.scrnoff_item_cnt = g_item_info.scrnoff_index;
        }
    } else {
        g_item_info.scrnoff_item_cnt = ITEM_SCREENOFF_MAX;
        g_item_info.scrnoff_index = 0;
    }
    return true;
}

static bool add_to_doze_table(struct sk_buff *skb)
{
    hw_wifi_filter_item item;

    if (g_item_info.p_items_data == NULL) {
        return false;
    }
    memset(&item, 0, sizeof(hw_wifi_filter_item));
    get_wifi_filter_info(skb, &item);

	if (g_item_info.scrnoff_item_cnt + g_item_info.doze_index >= g_item_cnt_max) {
		FILTER_LOGE("add_to_doze_table too much!");
		return false;
	}

    memcpy(&g_item_info.p_items_data[g_item_info.scrnoff_item_cnt+g_item_info.doze_index],
        &item,sizeof(hw_wifi_filter_item));
    g_item_info.doze_index++;
    /*
     * in order to save space,doze item max count along with screenoff item count
     */
    if (g_item_info.doze_index < (g_item_cnt_max - g_item_info.scrnoff_item_cnt)) {
        if (g_item_info.doze_item_cnt < (g_item_cnt_max - g_item_info.scrnoff_item_cnt)) {
            g_item_info.doze_item_cnt = g_item_info.doze_index;
        }
    } else {
        g_item_info.doze_item_cnt = (g_item_cnt_max - g_item_info.scrnoff_item_cnt);
        g_item_info.doze_index = 0;
    }
    return true;
}

void get_filter_info(
    struct sk_buff *skb,
    const struct nf_hook_state *state,
    unsigned int hook,
    const struct xt_table_info *private,
    const struct ipt_entry *e)
{
    bool success = false;
    spin_lock_bh(&g_item_info.item_lock);

    if (!bIsSupportWifiFilter) {
        FILTER_LOGD("wifi filter is not support");
        spin_unlock_bh(&g_item_info.item_lock);
        return;
    }
    if (!bDozeEnable){
        spin_unlock_bh(&g_item_info.item_lock);
        return;
    }
    if (NULL == state) {
        FILTER_LOGE("state is null");
        spin_unlock_bh(&g_item_info.item_lock);
        return;
    }
    if (hook != NF_INET_LOCAL_IN) {
        FILTER_LOGD("hook is %d", hook);
        spin_unlock_bh(&g_item_info.item_lock);
        return;
    }

    success = add_to_doze_table(skb);
    if (!success) {
        spin_unlock_bh(&g_item_info.item_lock);
        return;
    }

    spin_unlock_bh(&g_item_info.item_lock);

    wake_up_condition = WAKE_ADD;
    wake_up_interruptible(&mythread_wq);
}

void get_filter_infoEx(struct sk_buff *skb)
{
    bool success = false;
    spin_lock_bh(&g_item_info.item_lock);

    if (!bIsSupportWifiFilter) {
        FILTER_LOGD("wifi filter is not support");
        spin_unlock_bh(&g_item_info.item_lock);
        return;
    }
    if (skb->dev!= NULL){
        if (strncmp(skb->dev->name,WLAN_NAME,strlen(WLAN_NAME))!=0){
            spin_unlock_bh(&g_item_info.item_lock);
            return;
        }
    }
    if (!bScreenoff) {
        spin_unlock_bh(&g_item_info.item_lock);
        return;
    }
    if (!bDozeEnable){
        FILTER_LOGD("screen if off,doze is not enable");
        //add to screenoff table
        success = add_to_screenoff_table(skb);
    } else {
        //add to doze table
        success = add_to_doze_table(skb);
    }
    if (!success) {
        spin_unlock_bh(&g_item_info.item_lock);
        return;
    }
    spin_unlock_bh(&g_item_info.item_lock);
    wake_up_condition = WAKE_ADD;
    wake_up_interruptible(&mythread_wq);

}


static int hw_add_filter_items(hw_wifi_filter_item *items, int count)
{
    if ( NULL == gWlanFilterOps.add_filter_items )
    {
        return -1;
    }
	FILTER_LOGI("hw_add_filter_items count=%d", count);

    return gWlanFilterOps.add_filter_items(items, count);
}

static int hw_clear_all_item(void)
{
    if ( NULL == gWlanFilterOps.clear_filters )
    {
        return -1;
    }

    spin_lock_bh(&g_item_info.item_lock);

    g_item_info.scrnoff_item_cnt = 0;
    g_item_info.scrnoff_index = 0;
    g_item_info.doze_item_cnt = 0;
    g_item_info.doze_index = 0;
    if (g_item_info.p_items_data != NULL) {
        memset(&g_item_info.p_items_data[0],0,sizeof(hw_wifi_filter_item)*g_item_cnt_max);
    }

    spin_unlock_bh(&g_item_info.item_lock);

    FILTER_LOGD("hw_clear_filter_item");
    return gWlanFilterOps.clear_filters();
}
/*
 * In fact,clear doze item same with add screenoff item
 */
static int hw_clear_doze_item(void)
{
    if ( NULL == gWlanFilterOps.clear_filters )
    {
        return -1;
    }
    if (g_item_info.p_items_data == NULL) {
        return -1;
    }
    gWlanFilterOps.clear_filters();
    if (g_item_info.scrnoff_item_cnt != 0){
        hw_add_filter_items(&g_item_info.p_items_data[0], g_item_info.scrnoff_item_cnt);
    }
    spin_lock_bh(&g_item_info.item_lock);
    g_item_info.doze_item_cnt = 0;
    g_item_info.doze_index = 0;

    spin_unlock_bh(&g_item_info.item_lock);
    return 0;
}

int hw_set_net_filter_enable(int enable)
{
    int count = 0;
    int i;
    FILTER_LOGD("doze enable=%d", enable);

    if (!enable) {
        if(gWlanFilterOps.get_filter_pkg_stat != NULL)
        {
            gWlanFilterOps.get_filter_pkg_stat(
                &g_item_info.p_items_data[g_item_info.scrnoff_item_cnt],
                g_item_info.doze_item_cnt,&count);
            FILTER_LOGD("item count=%d",count);
            for (i = 0; i < count; i++)
            {
                FILTER_LOGD("i = %d,filter packet count=%d",i,g_item_info.p_items_data[i].filter_cnt);
            }
        }
        wake_up_condition = WAKE_EXIT_DOZE;
        wake_up_interruptible(&mythread_wq);
    } else {
        wake_up_condition = WAKE_ENTER_DOZE;
        wake_up_interruptible(&mythread_wq);
    }
    return 0;
}
EXPORT_SYMBOL(hw_set_net_filter_enable);

static int wifi_filter_threadfn(void *data)
{
    while(1)
    {
        wait_event_interruptible(mythread_wq, wake_up_condition);
		FILTER_LOGI("filter thread wake_up_condition=%d",wake_up_condition);
        switch(wake_up_condition) {
        case WAKE_ADD:
            if (g_item_info.p_items_data != NULL) {
                hw_add_filter_items(&g_item_info.p_items_data[0],
                    g_item_info.scrnoff_item_cnt + g_item_info.doze_item_cnt);
            }
            break;
        case WAKE_SCRN_ON:
            if (gWlanFilterOps.set_filter_enable != NULL) {
                gWlanFilterOps.set_filter_enable(false);
            }
            hw_clear_all_item();
            break;
        case WAKE_SCRN_OFF:
            if (gWlanFilterOps.set_filter_enable != NULL) {
                gWlanFilterOps.set_filter_enable(true);
            }
            break;
        case WAKE_EXIT_DOZE:
            bDozeEnable = false;
            hw_clear_doze_item();
            if (gWlanFilterOps.set_filter_enable_ex != NULL) {
                FILTER_LOGD("doze set_filter_enable_ex false");
                gWlanFilterOps.set_filter_enable_ex(TYPE_ICMP,false);
            }
            break;
        case WAKE_ENTER_DOZE:
            bDozeEnable = true;
            if (gWlanFilterOps.set_filter_enable_ex != NULL) {
                FILTER_LOGD("doze set_filter_enable_ex true");
                gWlanFilterOps.set_filter_enable_ex(TYPE_ICMP,true);
            }
            break;
        case WAKE_ENTER_NIGHT:
            bIsNight = true;
            if (!bDozeEnable) {
                break;
            }
            if (gWlanFilterOps.set_filter_enable_ex != NULL) {
                FILTER_LOGD("night set_filter_enable_ex true");
                gWlanFilterOps.set_filter_enable_ex(TYPE_ICMP,true);
            }
            break;
        case WAKE_EXIT_NIGHT:
            bIsNight = false;
            if (gWlanFilterOps.set_filter_enable_ex != NULL) {
                FILTER_LOGD("night set_filter_enable_ex false");
                gWlanFilterOps.set_filter_enable_ex(TYPE_ICMP,false);
            }
            break;
        default:
            break;
        }
        wake_up_condition = WAKE_INIT;
    }
    return 0;
}

static int __init init_kthread(void)
{
    int err;
    init_timer(&mytimer);
    init_waitqueue_head(&mythread_wq);
    WifiFilterThread = kthread_run(wifi_filter_threadfn,NULL,"wifi_filter_thread");
    if (IS_ERR(WifiFilterThread))
    {
        FILTER_LOGE("create new kernel thread failed");
        err = PTR_ERR(WifiFilterThread);
        WifiFilterThread = NULL;
        return err;
    }
    reg_fb_notification();
    return 0;
}

static void __exit exit_kthread(void)
{
    unreg_fb_notification();
    if(WifiFilterThread)
    {
        FILTER_LOGI("stop MyThread");
        kthread_stop(WifiFilterThread);
    }
}

late_initcall(init_kthread);
module_exit(exit_kthread);


MODULE_AUTHOR("z00220931");
MODULE_LICENSE("GPL");
