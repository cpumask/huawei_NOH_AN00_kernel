/*
 *  drivers/misc/inputhub/inputhub_route.h
 *  Sensor Hub Channel driver
 *
 *  Copyright (C) 2012 Huawei, Inc.
 *  Author: qindiwen <inputhub@huawei.com>
 *
 */
#ifndef __LINUX_INPUTHUB_ROUTE_H__
#define __LINUX_INPUTHUB_ROUTE_H__
#include "protocol.h"
#include <linux/version.h>
#include <huawei_platform/log/hw_log.h>
#include <linux/completion.h>
#include <linux/mutex.h>
#include <linux/workqueue.h>
#include <huawei_platform/inputhub/sensorhub.h>
#include <linux/hisi/hisi_rproc.h>

#define HWLOG_TAG sensorhub
HWLOG_REGIST();


#ifdef CONFIG_INPUTHUB_30
#include <linux/hisi/contexthub/iomcu_ipc.h>
#else

#define IOM3_ST_NORMAL			0
#define IOM3_ST_RECOVERY		1
#define IOM3_ST_REPEAT			2
#define MAX_SEND_LEN 32
#endif
/* max gap is 600ms */
#define MAX_SYSCOUNT_TIME_GAP 600000000LL
/* limit is 1 hour */
#define SYSCOUNT_DMD_LIMIT (3600 * 1000000000LL)
#define LENGTH_SIZE sizeof(unsigned int)
#define HEAD_SIZE (LENGTH_SIZE + TIMESTAMP_SIZE)
/* k3v5 report stairs level,record_count base on 1000 */
#define EXT_PEDO_VERSION_2 2000
#define ALS_RESET_COUNT 1
#define PS_RESET_COUNT 1
#define TP_RESET_COUNT 5
#define RESET_REFRESH_PERIOD (3600 * 8)
#define READ_CURRENT_INTERVAL 500
#define LOG_LEVEL_FATAL 0
/* Number of z-axis samples required by FingerSense at 1.6KHz ODR */
#define FINGERSENSE_DATA_NSAMPLES 128
#define HALL_ONE_DATA_NUM 4 /* number type of one ext_hall data */
#define HALL_DATA_NUM 3
#define MAX_EXT_HALL_VALUE 3


struct hw_ap_cp_route_t {
	unsigned int channel_id;
	int (*icc_open)(unsigned int channel_id, void *read_cb);
	int (*icc_read)(unsigned int channel_id, unsigned char *pdata,
		int size);
	int (*icc_write)(unsigned int channel_id, unsigned char *pdata,
		int size);
};

enum ext_hall_sensor_type {
	EXT_HALL_TYPE_START = 0,
	SLIDE_HALL_TYPE = 1,
	HUB_FOLD_HALL_TYPE = 2,
	EXT_HALL_TYPE_END,
};

struct link_package {
	int partial_order;
	char link_buffer[MAX_PKT_LENGTH_AP];
	int total_pkg_len;
	int offset;
};

struct mcu_notifier_work {
	struct work_struct worker;
	void *data;
};

struct mcu_notifier_node {
	int tag;
	int cmd;
	int (*notify)(const pkt_header_t *data);
	struct list_head entry;
};

struct mcu_notifier {
	struct list_head head;
	spinlock_t lock;
	struct workqueue_struct *mcu_notifier_wq;
};

#define OFFSET(struct_t, member) \
		offsetof(struct_t, member)
#define OFFSET_OF_END_MEM(struct_t, member) \
		((unsigned long)(&(((struct_t *)0)->member) + 1))
#define OFFSET_INTERVAL(struct_t, member1, member2) \
		(OFFSET_OF_END_MEM(struct_t, member2) - \
		OFFSET_OF_END_MEM(struct_t, member1))

typedef enum {
	DMD_CASE_ALS_NEED_RESET_POWER,
	DMD_CASE_PS_NEED_RESET_POWER,
	DMD_CASE_TP_NEED_RESET_POWER,
} sensor_reset_power;

enum port {
	ROUTE_SHB_PORT = 0x01,
	ROUTE_MOTION_PORT = 0x02,
	ROUTE_CA_PORT = 0x03,
	ROUTE_FHB_PORT = 0x04,
	ROUTE_FHB_UD_PORT = 0x05,
	ROUTE_KB_PORT = 0x06,
};

/* value length increase to 16 */
struct sensor_data {
	unsigned short type;
	unsigned short length;
	union{
		int value[16]; /*xyza...*/
		struct{
			int serial;
			int data_type;
			int sensor_type;
			int info[13];
		};
	};
};

struct t_sensor_get_data {
	atomic_t reading;
	struct completion complete;
	struct sensor_data data;
};
#define SELFTEST_RESULT_MAXLEN 5
struct sensor_status {
	/*
	 * record whether sensor is in activate status,
	 * already opened and setdelay
	 */
	int status[TAG_SENSOR_END];
	/* record sensor delay time */
	int delay[TAG_SENSOR_END];
	/* record whether sensor was opened */
	int opened[TAG_SENSOR_END];
	int batch_cnt[TAG_SENSOR_END];
	char gyro_selfTest_result[5];
	char mag_selfTest_result[5];
	char accel_selfTest_result[5];
	char connectivity_selfTest_result[5];
	char handpress_selfTest_result[5];
	char selftest_result[TAG_SENSOR_END][SELFTEST_RESULT_MAXLEN];
	int gyro_ois_status;
	struct t_sensor_get_data get_data[SENSORHUB_TYPE_END];
};

typedef struct {
	int for_alignment;
	union {
		char effect_addr[sizeof(int)];
		int pkg_length;
	};
	int64_t timestamp;
} t_head;

struct type_record {
	const pkt_header_t *pkt_info;
	struct read_info *rd;
	struct completion resp_complete;
	struct mutex lock_mutex;
	spinlock_t lock_spin;
};

struct iom7_log_work {
	void *log_p;
	struct delayed_work log_work;
};

struct inputhub_buffer_pos {
	char *pos;
	unsigned int buffer_size;
};

/*
 *Every route item can be used by one reader and one writer.
 */
struct inputhub_route_table {
	unsigned short port;
	struct inputhub_buffer_pos phead; /* point to the head of buffer */
	struct inputhub_buffer_pos pRead; /* point to read position of buffer */
	struct inputhub_buffer_pos pWrite; /* point to write position of buffer */
	wait_queue_head_t read_wait; /* to block read when no data in buffer */
	atomic_t data_ready;
	spinlock_t buffer_spin_lock; /* for read write buffer */
};

#ifndef CONFIG_INPUTHUB_30
typedef struct write_info {
	int tag;
	int cmd;
	const void *wr_buf; /* maybe NULL */
	int wr_len; /* maybe zero */
} write_info_t;

typedef struct read_info {
	int errno;
	int data_length;
	char data[MAX_PKT_LENGTH];
} read_info_t;
#endif

typedef struct pkt_subcmd_para {
	pkt_header_resp_t hd;
	uint32_t subcmd;
	char data[MAX_PKT_LENGTH];
} __packed pkt_subcmd_para_t;

typedef struct sensor_operation {
	int (*enable)(bool enable);
	int (*setdelay)(int ms);
} sensor_operation_t;

typedef struct ap_sensor_ops_record {
	sensor_operation_t ops;
	bool work_on_ap;
} t_ap_sensor_ops_record;

typedef bool(*t_match) (void *priv, const pkt_header_t *pkt);
struct mcu_event_waiter {
	struct completion complete;
	t_match match;
	void *out_data;
	int out_data_len;
	void *priv;
	struct list_head entry;
};

struct iom7_charging_work {
	uint32_t event;
	struct work_struct charging_work;
};

#ifdef DSS_ALSC_NOISE
struct als_noise_t {
	uint32_t status;
	uint32_t noise1;
	uint32_t noise2;
	uint32_t noise3;
	uint32_t noise4;
	uint32_t rsv;
	uint64_t timestamp;
};
#endif

static inline bool match_tag_cmd(void *priv, const pkt_header_t *recv)
{
	pkt_header_t *send = (pkt_header_t *) priv;

	return (send->tag == recv->tag) && ((send->cmd + 1) == recv->cmd) &&
		(send->tranid == recv->tranid);
}
#ifndef CONFIG_INPUTHUB_30
#define WAIT_FOR_MCU_RESP(trigger, match, wait_ms, out_data, out_data_len,\
		priv) \
({\
	int __ret = 0;\
	struct mcu_event_waiter waiter;\
	init_wait_node_add_list(&waiter, match, out_data, out_data_len,\
		(void *)priv);\
	reinit_completion(&waiter.complete);\
	trigger;\
	__ret = wait_for_completion_timeout(&waiter.complete,\
		msecs_to_jiffies(wait_ms));\
	list_del_mcu_event_waiter(&waiter);\
	__ret;\
})

#define WAIT_FOR_MCU_RESP_DATA_AFTER_SEND(send_pkt, trigger, wait_ms,\
		out_data, out_data_len) \
WAIT_FOR_MCU_RESP(trigger, match_tag_cmd, wait_ms,\
		out_data, out_data_len, send_pkt)

#define WAIT_FOR_MCU_RESP_AFTER_SEND(send_pkt, trigger, wait_ms) \
WAIT_FOR_MCU_RESP_DATA_AFTER_SEND(send_pkt, trigger, wait_ms, NULL, 0)
#endif
extern char *obj_tag_str[];
extern struct sensor_status sensor_status;
extern struct config_on_ddr *g_config_on_ddr;
extern wait_queue_head_t iom3_rec_waitq;
extern struct ipc_debug ipc_debug_info;
extern volatile int hall_value;
extern struct completion iom3_reboot;
extern int stop_auto_accel;
extern int stop_auto_als;
extern int stop_auto_ps;
extern rproc_id_t ipc_ap_to_iom_mbx;
extern int hall_sen_type;
extern int hall_number;
#ifdef CONFIG_HUAWEI_DSM
extern struct dsm_client *shb_dclient;
#endif
extern unsigned int sensor_read_number[];
extern int iom3_power_state;
extern struct completion iom3_resume_mini;
extern struct completion iom3_resume_all;
extern atomic_t iom3_rec_state;
extern uint32_t need_reset_io_power;
extern uint8_t tag_to_hal_sensor_type[TAG_SENSOR_END];
extern struct hw_ap_cp_route_t icc_route;
#ifdef DSS_ALSC_NOISE
extern struct hisi_dss_alsc hisi_dss_alsc_init;
#endif
extern t_ap_sensor_ops_record all_ap_sensor_operations[TAG_SENSOR_END];

extern int ak8789_register_report_data(int ms);
extern int color_sensor_enable(bool enable);
#ifdef CONFIG_HUAWEI_SHB_THP
extern int thp_log_save(const char *head);
extern int thp_send_sync_info_to_ap(const char *head);
extern int thp_send_volumn_to_drv(const char *head);
extern int thp_send_event_to_drv(const char *data);
#endif
/*called by sensorhub or tp modules.*/
extern int inputhub_route_open(unsigned short port);
extern void inputhub_route_close(unsigned short port);
extern ssize_t inputhub_route_read(unsigned short port, char __user *buf,
		size_t count);
/*called by inputhub_mcu module or test module.*/
extern void inputhub_route_init(void);
extern void inputhub_route_exit(void);
extern ssize_t inputhub_route_write(unsigned short port,
		char *buf, size_t count);
#ifndef CONFIG_INPUTHUB_30
extern int inputhub_route_recv_mcu_data(const char *buf, unsigned int length);
#endif
extern int inputhub_recv_msg_app_hander(const pkt_header_t *head, bool is_notifier);
void inputhub_pm_callback(pkt_header_t *head);
extern int write_customize_cmd(const struct write_info *wr,
		struct read_info *rd, bool is_lock);
extern int register_mcu_event_notifier(int tag, int cmd,
		int (*notify)(const pkt_header_t *head));
extern int unregister_mcu_event_notifier(int tag,
		int cmd, int (*notify)(const pkt_header_t *head));
extern int inputhub_sensor_enable(int tag, bool enable);
extern int inputhub_sensor_enable_stepcounter(bool enable,
		type_step_counter_t steptype);
extern int inputhub_sensor_setdelay(int tag, interval_param_t *interval_param);
extern int inputhub_sensor_enable_nolock(int tag, bool enable);
extern int inputhub_sensor_setdelay_nolock(int tag,
		interval_param_t *interval_param);
#ifndef CONFIG_INPUTHUB_30
extern int inputhub_mcu_write_cmd(const void *buf, unsigned int length);
#endif
extern void inputhub_update_info(const void *buf,int ret,bool is_in_recovery);
extern int register_ap_sensor_operations(int tag, sensor_operation_t *ops);
extern int unregister_ap_sensor_operations(int tag);
extern ssize_t inputhub_route_write_batch(unsigned short port, char *buf,
		size_t count, int64_t timestamp);
extern bool ap_sensor_enable(int tag, bool enable);
extern bool ap_sensor_setdelay(int tag, int ms);
extern int report_sensor_event(int tag, int value[], int length);
#ifndef CONFIG_INPUTHUB_30
extern void init_wait_node_add_list(struct mcu_event_waiter *waiter,
		t_match match, void *out_data, int out_data_len, void *priv);
extern void list_del_mcu_event_waiter(struct mcu_event_waiter *self);
#endif
int send_modem_cmd_to_hub(const void *buf, unsigned int length);
void tell_screen_status_to_mcu(uint8_t status);
int send_app_config_cmd(int tag, void *app_config, bool use_lock);
int get_fold_hinge_status(void);
#ifdef CONFIG_INPUTHUB_20
extern int ap_hall_report(int value);
#else /* CONFIG_INPUTHUB_20 = 0 */
static inline int ap_hall_report(int value)
{
	return 0;
}

#endif /* CONFIG_INPUTHUB_20 */
#endif /* __LINUX_INPUTHUB_ROUTE_H__ */
