

#ifndef __BFGX_DEV_H__
#define __BFGX_DEV_H__

/* 其他头文件包含 */
#include "hw_bfg_ps.h"

/* 宏定义 */
#define DTS_COMP_HI110X_PS_NAME    "hisilicon,hisi_bfgx"

#define BT_IOCTL_FAKE_CLOSE_CMD     100
#define BT_IOCTL_HCISETPROTO        101
#define BT_IOCTL_HCIUNSETPROTO      102
#define BT_IOCTL_OPEN               103
#define BT_IOCTL_RELEASE            104

#define BT_FAKE_CLOSE               1
#define BT_REAL_CLOSE               0

#define BT_TYPE_DATA_LEN           1

/* timeout for gnss read */
#define GNSS_SET_READ_TIME     1
#define GNSS_READ_DEFAULT_TIME 1000
#define GNSS_MAX_READ_TIME     10000

/* gnss update para */
#define PLAT_GNSS_MAGIC             'w'
#define PLAT_GNSS_DCXO_SET_PARA_CMD _IOW(PLAT_GNSS_MAGIC, 1, int)
#define PLAT_GNSS_REFCLK_PARA_CMD   _IOW(PLAT_GNSS_MAGIC, 3, int)

/* timeout for fm read */
#define FM_SET_READ_TIME     1
#define FM_READ_DEFAULT_TIME 1000
#define FM_MAX_READ_TIME     10000

/* timeout for dbg read */
#define DBG_READ_DEFAULT_TIME 5000

#define UART_LOOP_RESUME_DEVICE_DATA_HANDLER 0
#define UART_LOOP_SET_DEVICE_DATA_HANDLER    1

#define UART_LOOP_MAX_PKT_LEN 2048 /* device buf len is 2400 */

/* 结构体定义 */
struct bt_data_combination {
    uint8 len;
    uint8 type;
    uint16 resverd;
};

typedef struct uart_loop_user_cfg {
    uint32 loop_count;
    uint16 pkt_len;
    uint16 cpu_fre;
    uint32 uart_loop_enable;
    uint32 uart_loop_tx_random_enable;
} uart_loop_cfg;

typedef struct uart_loop_test {
    uart_loop_cfg *test_cfg;
    uint8 *tx_buf;
    uint8 *rx_buf;
    uint16 rx_pkt_len;
    uint16 result;
    struct completion set_done;
    struct completion loop_test_done;
} uart_loop_test_struct;

/* 全局变量声明 */
extern const uint8 *g_bfgx_subsys_name[BFGX_BUTT];

extern uint32 g_gnss_me_thread_status;
extern uint32 g_gnss_lppe_thread_status;
extern atomic_t g_ir_only_mode;

extern uart_loop_cfg g_uart_loop_test_cfg;
extern uart_loop_test_struct *g_uart_loop_test_info;

/* 函数声明 */
int32 uart_loop_test(void);
int32 uart_loop_test_recv_pkt(struct ps_core_s *ps_core_d, const uint8 *buf_ptr, uint16 pkt_len);

int32 ps_get_core_reference(struct ps_core_s **core_data, int type);
int32 ps_get_plat_reference(struct ps_plat_s **plat_data);
struct sk_buff *ps_alloc_skb(uint16 len);
void ps_kfree_skb(struct ps_core_s *ps_core_d, uint8 type);
int32 alloc_seperted_rx_buf(struct ps_core_s *ps_core_d, uint8 subsys, uint32 len, uint8 alloctype);
void free_seperted_rx_buf(struct ps_core_s *ps_core_d, uint8 subsys, uint8 alloctype);
bool ps_chk_tx_queue_empty(struct ps_core_s *ps_core_d);
bool ps_chk_bfg_active(struct ps_core_s *ps_core_d);
int32 plat_bfgx_dump_rotate_cmd_read(struct ps_core_s *ps_core_d, uint64 arg);
bool ps_chk_only_gnss_and_cldslp(struct ps_core_s *ps_core_d);
bool ps_bfg_subsys_active(uint32 subsys);
void ps_set_device_log_status(bool status);
bool ps_is_device_log_enable(void);
int32 prepare_to_visit_node(struct ps_core_s *ps_core_d);
int32 post_to_visit_node(struct ps_core_s *ps_core_d);
int32 bfgx_open_cmd_send(uint32 subsys, uart_enum uart);
int32 bfgx_close_cmd_send(uint32 subsys, uart_enum uart);
int32 uart_bfgx_close_cmd(void);
int32 hw_ir_only_open_other_subsys(void);
int32 ps_restore_skbqueue(struct ps_core_s *ps_core_d, struct sk_buff *skb, uint8 type);
int32 hw_bfgx_open(uint32 subsys);
int32 hw_bfgx_close(uint32 subsys);
int hw_ps_init(void);
void hw_ps_exit(void);
#ifdef _PRE_CONFIG_HISI_S3S4_POWER_STATE
extern int64 hw_bt_ioctl(struct file *file, uint32 cmd, uint64 arg);
#endif
#endif /* __BFGX_DEV_H__ */
