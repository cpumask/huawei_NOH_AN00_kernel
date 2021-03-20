
#ifndef __BFGX_DATA_PARSE_H__
#define __BFGX_DATA_PARSE_H__

#include "hw_bfg_ps.h"

/* 宏定义 */
#define RX_PACKET_CORRECT  0
#define RX_PACKET_CONTINUE 1
#define RX_PACKET_ERR      (-1)

#define PACKET_START_SIGNAL 0x7e
#define PACKET_END_SIGNAL   0x7e

#define BT_TX_TIMES 5 /* times continue tx bt data */

#define SEPRETED_RX_PKT_SEQ_ERROR   0
#define SEPRETED_RX_PKT_SEQ_CORRECT 1

/* last words type in 8 byte packet head */
#define PACKET_RX_FUNC_LAST_WORDS    0x10 /* rx last words from device */
#define SYS_TOTAL_PACKET_LENTH       0x06 /* sys packet total lenth */
#define PACKET_RX_RPT_IND_LAST_WORDS 0x03 /* log pri id */
#define LAST_WORD_LEN                109
#define DEV_SEND_CHR_ERRNO_LEN       0x04 /* device send CHR errno lenth */

/* last word info index */
#define START_SIGNAL_LAST_WORDS         0
#define PACKET_RX_FUNC_INDEX_LAST_WORDS 1
#define RPT_IND_INDEX_LAST_WORDS        2
#define FAULT_TYPE_INDEX_LAST_WORDS     12
#define FAULT_REASON_INDEX_LAST_WORDS   16
#define LR_INDEX_LAST_WORDS             80
#define PC_INDEX_LAST_WORDS             84
#define PACKET_FRAMELEN_INDEX           4
#define PACKET_HEADER_LEN               8
#define PACKET_OFFSET_HEAD_INDEX        4
#define PACKET_OFFSET_HEAD_NEXT_INDEX   5

#define MEM_DUMP_RX_MAX_FRAME 4096 /* mem dump recv buf max lenth */
#define NOBT_FRAME_EXTRA_SPACE  6                              /* 预留6字节余量 */
#define MAX_NOBT_FRAME_LEN      (67 + NOBT_FRAME_EXTRA_SPACE)  /* FGNI max frame len */
#define MAX_LAST_WORD_FRAME_LEN (114 + NOBT_FRAME_EXTRA_SPACE) /* FGNI max frame len */

#define DEBUG_SKB_BUFF_LEN 2048
#define PUBLIC_BUF_MAX     (8 * 1024) /* public buf total lenth */

#define len_low_byte(len)  ((uint8)((len)&0xff))
#define len_high_byte(len) ((uint8)(((len)&0xff00) >> 8))

#define PS_HEAD_SIZE sizeof(struct ps_packet_head) /* 包头size */
#define PS_END_SIZE sizeof(struct ps_packet_end)   /* 包尾size */
#define PS_PKG_SIZE (PS_HEAD_SIZE + PS_END_SIZE)     /* 包头+包尾size */

/* 结构体定义 */
struct ps_packet_head {
    uint8 packet_start;
    uint8 func_type;
    uint16 packet_len;
};

struct ps_packet_end {
    uint8 packet_end;
};

typedef enum {
    PKG_NOT_SEPRETED = 0,
    PKG_SEPRETED = 1,
} pkg_sepreted_state;

struct ps_tx_manage_info {
    uint8 sepreted; /* 是否分包 */
    uint8 tx_queue; /* 发送队列 */
    uint8 start_pkg; /* 起始包 */
    uint8 common_pkg; /* 中间包 */
    uint8 last_pkg; /* 结束包 */
};

/* 全局变量声明 */
extern int32 (*st_tty_recv)(void *, const uint8 *, int32);
extern int32 (*me_tty_recv)(void *, const uint8 *, int32);

extern uint32 g_bfgx_rx_max_frame[BFGX_BUTT];
extern uint32 g_bfgx_rx_queue[BFGX_BUTT];

/* 函数声明 */
int32 ps_write_tty(struct ps_core_s *, const uint8 *, int32);
int32 ps_push_skb_queue(struct ps_core_s *ps_core_d, uint8 *buf_ptr, uint16 pkt_len, uint8 type);
int32 ps_core_recv(void *disc_data, const uint8 *data, int32 count);
int32 ps_skb_enqueue(struct ps_core_s *ps_core_d, struct sk_buff *skb, uint8 type);
struct sk_buff *ps_skb_dequeue(struct ps_core_s *ps_core_d, uint8 type);
int32 ps_skb_queue_len(struct ps_core_s *ps_core_d, uint8 type);
int32 ps_add_packet_head(uint8 *buf, uint8 type, uint16 lenth);
int32 ps_tx_sys_cmd(struct ps_core_s *ps_core_d, uint8 type, uint8 content);
int32 ps_tx_urgent_cmd(struct ps_core_s *ps_core_d, uint8 type, uint8 content);
int32 ps_h2d_tx_data(struct ps_core_s *ps_core_d, uint8 subsys, const int8 __user *buf, size_t count);
int32 ps_core_init(struct ps_core_s **, int);
int32 ps_core_exit(struct ps_core_s *, int);
void dump_uart_rx_buf(struct ps_core_s *ps_core_d);
void reset_uart_rx_buf(struct ps_core_s *ps_core_d);

#endif /* __BFGX_DATA_PARSE_H__ */
