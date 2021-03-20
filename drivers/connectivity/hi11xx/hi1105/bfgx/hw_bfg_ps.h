

#ifndef __HW_BFG_PS_H__
#define __HW_BFG_PS_H__

/* ����ͷ�ļ����� */
#include <linux/skbuff.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/debugfs.h>
#include <linux/workqueue.h>
#include "plat_type.h"

/* �궨�� */
#define BT_TX_MAX_FRAME   3000  /* tx bt data max lenth */
#define GNSS_TX_MAX_FRAME 30000 /* gnss send buf max lenth */
#define FM_TX_MAX_FRAME   512   /* fm cmd data max lenth */
#define NFC_TX_MAX_FRAME  512   /* nfc cmd data max lenth */
#define IR_TX_MAX_FRAME   5120  /* ir cmd data max lenth */
#define DBG_TX_MAX_FRAME  10000 /* dbg cmd data max lenth */

#define BT_RX_MAX_FRAME                1500  /* RX bt data max lenth */
#define FM_RX_MAX_FRAME                512   /* fm recv buf max lenth */
#define NFC_RX_MAX_FRAME               512   /* nfc recv buf max lenth */
#define IR_RX_MAX_FRAME                5120  /* ir recv buf max lenth */
#define DBG_RX_MAX_FRAME               10000 /* dbg recv buf max lenth */
#define GNSS_RX_MAX_FRAME              30000 /* gnss recv buf max lenth */

/* skb has skb_shared_info, and SKB_DATA_ALIGN */
#define GNSS_RX_SEPERATE_FRAME_MAX_LEN (4 * 1024 - SKB_DATA_ALIGN(sizeof(struct skb_shared_info)) - SKB_DATA_ALIGN(1))

#define PS_TX_PACKET_LIMIT   200

#define HISI_UART_DEV_NAME_LEN 32

/* define for tx and rx packet queue */
#define TX_URGENT_QUEUE 0
#define TX_HIGH_QUEUE   1
#define TX_LOW_QUEUE    2
#define RX_GNSS_QUEUE   3
#define RX_FM_QUEUE     4
#define RX_BT_QUEUE     5
#define RX_DBG_QUEUE    6
#define RX_NFC_QUEUE    7
#define RX_IR_QUEUE     8

/* max number for tx and rx packet queue */
#define TX_HIGH_QUE_MAX_NUM 50
#define TX_LOW_QUE_MAX_NUM  250
#define RX_GNSS_QUE_MAX_NUM 50
#define RX_FM_QUE_MAX_NUM   50
#define RX_BT_QUE_MAX_NUM   50
#define RX_DBG_QUE_MAX_NUM  50
#define RX_NFC_QUE_MAX_NUM  50
#define RX_IR_QUE_MAX_NUM   50

/* timeout when after download patch */
#define WAIT_BFGX_BOOTOK_TIME 1000
#define WAIT_BT_OPEN_TIME     3000
#define WAIT_BT_CLOSE_TIME    3000
#define WAIT_FM_OPEN_TIME     3000
#define WAIT_FM_CLOSE_TIME    3000
#define WAIT_GNSS_OPEN_TIME   3000
#define WAIT_GNSS_CLOSE_TIME  3000
#define WAIT_IR_OPEN_TIME     3000
#define WAIT_IR_CLOSE_TIME    3000
#define WAIT_NFC_OPEN_TIME    1000
#define WAIT_NFC_CLOSE_TIME   2000

#define BFG_POWER_GPIO_DOWN 0
#define BFG_POWER_GPIO_UP   1

#define NONEED_SET_FLAG 0
#define NEED_SET_FLAG   1

#define DEV_THREAD_EXIT 0
#define DEV_THREAD_INIT 1

#define SYS_INF_HEARTBEAT_CMD_BASE 128

enum BFGN_DATA_MSG_TYPE_ENUM {
    SYS_MSG = 0x00,         /* ϵͳ������Ϣ */
    BT_MSG = 0x01,          /* BT������Ϣ */
    GNSS_FIRST_MSG = 0x02,  /* GNSS������Ϣ����һ���ֶ���Ϣ */
    GNSS_COMMON_MSG = 0x03, /* GNSS������Ϣ���м�ֶ���Ϣ */
    GNSS_LAST_MSG = 0x04,   /* GNSS������Ϣ�����һ���ֶ���Ϣ */
    FM_FIRST_MSG = 0x05,    /* FM������Ϣ����һ���ֶ���Ϣ */
    FM_COMMON_MSG = 0x06,   /* FM������Ϣ���м�ֶ���Ϣ */
    FM_LAST_MSG = 0x07,     /* FM������Ϣ�����һ���ֶ���Ϣ */
    IR_FIRST_MSG = 0x08,    /* ���⴮����Ϣ����һ���ֶ���Ϣ */
    IR_COMMON_MSG = 0x09,   /* ���⴮����Ϣ���м�ֶ���Ϣ */
    IR_LAST_MSG = 0x0A,     /* ���⴮����Ϣ�����һ���ֶ���Ϣ */
    NFC_FIRST_MSG = 0x0B,   /* NFC������Ϣ����һ���ֶ���Ϣ */
    NFC_COMMON_MSG = 0x0C,  /* NFC������Ϣ���м�ֶ���Ϣ */
    NFC_LAST_MSG = 0x0D,    /* NFC������Ϣ�����һ���ֶ���Ϣ */
    OML_MSG = 0x0E,         /* ��ά�ɲ���Ϣ */
    MEM_DUMP_SIZE = 0x0f,   /* bfgx�쳣ʱ��Ҫdump��mem������Ϣ */
    MEM_DUMP = 0x10,        /* bfgx�쳣ʱ���ڴ�dump��Ϣ */
    WIFI_MEM_DUMP = 0x11,   /* UART READ WIFI MEM���ڴ�dump��Ϣ */
    BFGX_CALI_MSG = 0x12,   /* BFGX У׼�����ϱ� */

    MSG_BUTT,
};

/* below is tx sys content define */
enum platform_event_enum {
    SYS_CFG_OPEN_BT = 0,
    SYS_CFG_CLOSE_BT = 1,
    SYS_CFG_OPEN_GNSS = 2,
    SYS_CFG_CLOSE_GNSS = 3,
    SYS_CFG_OPEN_FM = 4,
    SYS_CFG_CLOSE_FM = 5,
#ifdef HAVE_HISI_NFC
    SYS_CFG_OPEN_NFC = 6,
    SYS_CFG_CLOSE_NFC = 7,
#else
    PL_FAKE_CLOSE_BT_EVT = 7,
#endif
    SYS_CFG_OPEN_IR = 8,
    SYS_CFG_CLOSE_IR = 9,
    SYS_CFG_OPEN_WIFI = 0xa,        /* hostͨ��uart��WCPU */
    SYS_CFG_CLOSE_WIFI = 0xb,       /* hostͨ��uart�ر�WCPU */
    SYS_CFG_READ_STACK = 0xc,       /* hostͨ��uart��ջ */
    SYS_CFG_QUERY_RF_TEMP = 0xd,    /* hostͨ��uart��ѯrf�¶� */
    SYS_CFG_ALLOWDEV_SLP = 0xe,     /* host allow device sleep */
    SYS_CFG_DISALLOW_SLP = 0xf,     /* host disallow device sleep */
    SYS_CFG_SHUTDOWN_SLP = 0x10,    /* shutdown bcpu */
    SYS_CFG_PL_ENABLE_PM = 0x11,    /* enable plat dev lowpower feature */
    SYS_CFG_PL_DISABLE_PM = 0x12,   /* disable plat dev lowpower feature */
    SYS_CFG_BT_ENABLE_PM = 0x13,    /* enable BT dev lowpower feature */
    SYS_CFG_BT_DISABLE_PM = 0x14,   /* disable BT dev lowpower feature */
    SYS_CFG_GNSS_ENABLE_PM = 0x15,  /* enable GNSS dev lowpower feature */
    SYS_CFG_GNSS_DISABLE_PM = 0x16, /* disable GNSS dev lowpower feature */
    SYS_CFG_NFC_ENABLE_PM = 0x17,   /* enable NFC dev lowpower feature */
    SYS_CFG_NFC_DISABLE_PM = 0x18,  /* disable NFC dev lowpower feature */

    SYS_CFG_DEV_PANIC = 0x19,       /* cause device into exception */
    SYS_CFG_DUMP_RESET_WCPU = 0x1a, /* hostͨ��uart�����縴λWCPU */

    SYS_CFG_HALT_WCPU = 0x1b,          /* halt WCPU */
    SYS_CFG_READ_WLAN_PUB_REG = 0x1c,  /* ��ȡwcpu�Ĺ����Ĵ��� */
    SYS_CFG_READ_WLAN_PRIV_REG = 0x1d, /* ��ȡwcpu��˽�мĴ��� */
    SYS_CFG_READ_WLAN_MEM = 0x1e,      /* ��ȡwcpu��mem */
    SYS_CFG_EXCEP_RESET_WCPU = 0x1f,   /* wifi DFR WCPU��λ */

    SYS_CFG_SET_UART_LOOP_HANDLER = 0x20, /* ����device��uart���ش����� */
    SYS_CFG_SET_UART_LOOP_FINISH = 0x21,  /* UART����test ok���ָ�gnss��Ϣ������ */

    PL_PM_DEBUG = 0x22,          /* �͹���ά�⿪�� */
    PL_BAUT_CHG_REQ_ACK = 0x23,  /* �������л�����ACK */
    PL_BAUT_CHG_COMPLETE = 0x24, /* �������л���� */

    SYS_CFG_NOTIFY_WIFI_OPEN = 0x25,  /* wifi open  ֪ͨ */
    SYS_CFG_NOTIFY_WIFI_CLOSE = 0x26, /* wifi close ֪ͨ */

    PL_EVT_BUTT
};

/* below is rx sys content define from device */
enum SYS_INF_MSG_VALUE_ENUM {
    SYS_INF_PF_INIT = 0x00,                /* ƽ̨�����ʼ����� */
    SYS_INF_BT_INIT = 0x01,                /* BT�����ʼ����� */
    SYS_INF_GNSS_INIT = 0x02,              /* GNSS�����ʼ����� */
    SYS_INF_FM_INIT = 0x03,                /* FM�����ʼ����� */
    SYS_INF_BT_DISABLE = 0x04,             /* BT���� */
    SYS_INF_GNSS_DISABLE = 0x05,           /* GNSS���� */
    SYS_INF_FM_DISABLE = 0x06,             /* FM���� */
    SYS_INF_BT_EXIT = 0x07,                /* BT�˳� */
    SYS_INF_GNSS_EXIT = 0x08,              /* GNSS�˳� */
    SYS_INF_FM_EXIT = 0x09,                /* FM�˳� */
    SYS_INF_GNSS_WAIT_DOWNLOAD = 0x0A,     /* �ȴ�GNSS�������� */
    SYS_INF_GNSS_DOWNLOAD_COMPLETE = 0x0B, /* GNSS����������� */
    SYS_INF_BFG_HEART_BEAT = 0x0C,         /* �����ź� */
    SYS_INF_DEV_AGREE_HOST_SLP = 0x0D,     /* device�ظ�host��˯ */
    SYS_INF_DEV_NOAGREE_HOST_SLP = 0x0E,   /* device�ظ�host����˯ */
    SYS_INF_WIFI_OPEN = 0x0F,              /* WCPU�ϵ���� */
    SYS_INF_IR_INIT = 0x10,                /* IR�����ʼ����� */
    SYS_INF_IR_EXIT = 0x11,                /* IR�˳� */
#ifdef HAVE_HISI_NFC
    SYS_INF_NFC_INIT      =  0x12,         /* NFC�����ʼ����� */
    SYS_INF_NFC_EXIT      =  0x13,         /* NFC�˳� */
#else
    SYS_INF_FAKE_CLOSE_BT =  0x13,         /* FAKE CLOSE BT��ACK */
#endif
    SYS_INF_WIFI_CLOSE = 0x14,             /* WCPU�µ���� */
    SYS_INF_RF_TEMP_NORMAL = 0x15,         /* RF�¶����� */
    SYS_INF_RF_TEMP_OVERHEAT = 0x16,       /* RF�¶ȹ��� */
    SYS_INF_MEM_DUMP_COMPLETE = 0x17,      /* bfgx�쳣ʱ��MEM DUMP����� */
    SYS_INF_WIFI_MEM_DUMP_COMPLETE = 0X18, /* bfgx�쳣ʱ��MEM DUMP����� */
    SYS_INF_UART_HALT_WCPU = 0x19,         /* uart halt wcpu ok */
    SYS_INF_UART_LOOP_SET_DONE = 0x1a,     /* device ����uart����ok */
    SYS_INF_CHR_ERRNO_REPORT = 0x1b,       /* device��host�ϱ�CHR�쳣�� */

    SYS_INF_RSV0 = 0x1c,                   /* rsv */
    SYS_INF_RSV1 = 0x1d,                   /* rsv */
    SYS_INF_RSV2 = 0x1e,                   /* rsv */

    SYS_INF_GNSS_LPPE_INIT = 0x1F,     /* 1103 GNSS �������̳߳�ʼ����� */
    SYS_INF_GNSS_LPPE_EXIT = 0x20,     /* 1103 GNSS �������߳��˳� */
    SYS_INF_GNSS_TRICKLE_SLEEP = 0x21, /* 1103 GNSS_TRICKLE_SLEEP */

    SYS_INF_BUTT, /* ö�ٶ�������ܳ���127����Ϊ128~255����������������ʱ��� */
};

typedef enum {
    BFGX_BT = 0x00,
    BFGX_FM = 0x01,
    BFGX_GNSS = 0x02,
    BFGX_IR = 0x03,
    BFGX_NFC = 0x04,
    BFGX_BUTT = 0x05,
} bfgx_subsys_type_enum;

#define WIFI_SUBSYS        BFGX_BUTT           /* wifi��BFG��ϵͳͳһ��� */
#define PLAT_SUBSYS        (WIFI_SUBSYS + 1)

#define HI110X_BT_SUB_MASK    (1<<BFGX_BT)
#define HI110X_FM_SUB_MASK    (1<<BFGX_FM)
#define HI110X_GNSS_SUB_MASK  (1<<BFGX_GNSS)
#define HI110X_IR_SUB_MASK    (1<<BFGX_IR)
#define HI110X_NFC_SUB_MASK   (1<<BFGX_NFC)
#define HI110X_WIFI_SUB_MASK  (1<<WIFI_SUBSYS)
#define HI110X_PLAT_SUB_MASK  (1<<PLAT_SUBSYS)

typedef enum {
    POWER_STATE_SHUTDOWN = 0,
    POWER_STATE_OPEN = 1,
    POWER_STATE_BUTT = 2,
} power_state_enum;

typedef enum {
    RX_SEQ_NULL = 0,
    RX_SEQ_START = 1,
    RX_SEQ_INT = 2,
    RX_SEQ_LAST = 3,

    RX_SEQ_BUTT,
} bfgx_rx_sepreted_pkt_type;

typedef enum {
    KZALLOC = 0,
    VMALLOC = 1,

    ALLOC_BUFF,
} alloc_type;

typedef enum {
    GNSS_SEPER_TAG_INIT,
    GNSS_SEPER_TAG_LAST,
} gnss_seperate_tag;

typedef enum {
    SSI_MEM_TEST,
    SSI_FILE_TEST,
    SSI_TEXT_BUTT,
} ssi_test_type;

typedef enum {
    BUART = 0,
    GUART = 1,
    UART_BUTT,
} uart_enum;

struct bfgx_sepreted_rx_st {
    spinlock_t sepreted_rx_lock;
    uint8 *rx_buf_ptr;
    uint8 *rx_buf_org_ptr;
    uint16 rx_pkt_len;
    uint16 rx_buf_all_len;
    uint8 rx_prev_seq;
};

struct st_bfgx_data {
    spinlock_t rx_lock;
    struct sk_buff_head rx_queue;
    struct completion wait_opened;
    struct completion wait_closed;
    struct bfgx_sepreted_rx_st sepreted_rx; /* bfgx�ְ��������ݽṹ */
    wait_queue_head_t rx_wait;              /* wait queue for rx packet */
    atomic_t subsys_state;
    const char *name;
    uint16 read_delay;
    uint64 rx_pkt_num;
    uint64 tx_pkt_num;
};

/*
 * struct ps_core_s - PS core internal structure
 * @tty: tty provided by the TTY core for line disciplines.
 * @txq: the list of skbs which needs to be sent onto the TTY.
 * @pm_data: reference to the parent encapsulating structure.
 *
 */
struct ps_core_s {
    void *ps_plat;
    struct pm_drv_data *pm_data;;
    struct tty_struct *tty;
    struct st_bfgx_data bfgx_info[BFGX_BUTT];
    /* queue head define for all sk_buff */
    struct sk_buff_head tx_urgent_seq;
    struct sk_buff_head tx_high_seq;
    struct sk_buff_head tx_low_seq;

    struct sk_buff_head rx_dbg_seq;
    /* wait queue for rx packet */
    wait_queue_head_t rx_dbg_wait;
    /* force tx workqueue exit, release tty mutex */
    atomic_t force_tx_exit;
    atomic_t dbg_func_has_open;
    atomic_t dbg_read_delay;

    /* define for new workqueue */
    struct workqueue_struct *ps_tx_workqueue;
    /* define new work struct */
    struct work_struct tx_skb_work;

    spinlock_t rx_lock;

    /* mark user doing something on node */
    atomic_t node_visit_flag;

    /* define variable for decode rx data from uart */
    uint8 rx_pkt_type;
    uint16 rx_pkt_total_len;
    uint16 rx_have_recv_pkt_len;
    uint16 rx_have_del_public_len;
    uint8 *rx_decode_tty_ptr;
    uint8 *rx_public_buf_org_ptr;
    uint8 *rx_decode_public_buf_ptr;

    uint8 tty_have_open;
    uint8 dev_index;

    uint64 rx_pkt_sys;
    uint64 rx_pkt_oml;

    /* ͳ��tty���ݵ��շ� */
    struct mutex tty_mutex;
    atomic_t tty_tx_cnt;
    atomic_t tty_rx_cnt;

    // ��SR������ʹ��
    struct semaphore sr_wake_sema;
    atomic_t sr_cnt;
};

/*
 * struct ps_plat_s - the PM internal data, embedded as the
 *  platform's drv data. One for each PS device in the system.
 * @pm_pdev: the platform device added in one of the board-XX.c file
 *  in arch/XX/ directory, 1 for each PS device.
 * @core_data: PS core's data, which mainly is the tty's disc_data
 *
 */
struct ps_plat_s {
    struct platform_device *pm_pdev;
    struct ps_core_s *core_data;
    uint8 dev_name[HISI_UART_DEV_NAME_LEN];
    uint64 baud_rate;
    uint8 flow_cntrl;
    uint16 ldisc_num;
};

/*
 * struct hw_ps_plat_data - platform data shared between PS driver and
 *  platform specific board file which adds the PS device.
 * @dev_name: The UART/TTY name to which chip is interfaced. (eg: /dev/ttyS1)
 * @flow_cntrl: Should always be 1, since UART's CTS/RTS is used for PM
 *  purposes.
 * @baud_rate: The baud rate supported by the Host UART controller, this will
 *  be shared across with the chip via a HCI VS command from User-Space Init
 *  Mgr application.
 * @suspend:
 * @resume: legacy PM routines hooked to platform specific board file, so as
 *  to take chip-host interface specific action.
 * @set_bt_power: set gpio BT_EN to high.
 */
struct hw_ps_plat_data {
    int32 (*suspend)(struct platform_device *, pm_message_t);
    int32 (*resume)(struct platform_device *);
    uint8 dev_name[HISI_UART_DEV_NAME_LEN]; /* uart name */
    uint64 baud_rate;
    uint8 flow_cntrl; /* flow control flag */
    uint16 ldisc_num;
    uint8 fill_byte2;
};

#endif /* HW_BFG_PS_H */
