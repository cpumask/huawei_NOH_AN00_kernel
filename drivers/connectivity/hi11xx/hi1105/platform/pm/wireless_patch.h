

#ifndef __WIRELESS_PATCH_H__
#define __WIRELESS_PATCH_H__

#include "hw_bfg_ps.h"
#include "plat_debug.h"
#include "plat_firmware.h"

#define os_init_waitqueue_head(wq)                               init_waitqueue_head(wq)
#define os_wait_event_interruptible(wq, condition)               wait_event_interruptible(wq, condition)
#define os_wait_event_interruptible_timeout(wq, condition, time) wait_event_interruptible_timeout(wq, condition, time)
#define os_waitqueue_active(wq)                                  waitqueue_active(wq)
#define os_wake_up_interruptible(wq)                             wake_up_interruptible(wq)

#define SOH 0x01 /* 开始字符 */
#define EOT 0x04 /* 发送完成 */
#define ACK 0x06 /* 正确接收应答 */
#define NAK 0x15 /* 校验错误重新发送，通讯开始时用于接收方协商累加校验 */
#define CAN 0x18 /* 结束下载 */

#define PATCH_INTEROP_TIMEOUT HZ

#define CRC_TABLE_SIZE 256

#define DATA_BUF_LEN 128

#define CFG_PACH_LEN 64

#define UART_CFG_FILE_FPGA_HI1105  "/vendor/firmware/hi1105/fpga/uart_cfg"
#define UART_CFG_FILE_PILOT_HI1105 "/vendor/firmware/hi1105/pilot/uart_cfg"

#define UART_CFG_FILE_FPGA_HI1106  "/vendor/firmware/hi1106/fpga/uart_cfg"
#define UART_CFG_FILE_PILOT_HI1106 "/vendor/firmware/hi1106/pilot/uart_cfg"

#define BRT_CMD_KEYWORD "BAUDRATE"

#define CMD_LEN  32
#define PARA_LEN 128

#define PATCH_SEND_CAN_UART                                \
    do {                                                   \
        patch_send_char(CAN, NO_RESPONSE);                 \
    } while (0)
#define PATCH_SEND_EOT_UART                                \
    do {                                                   \
        patch_send_char(EOT, WAIT_RESPONSE);               \
    } while (0)
#define PATCH_SEND_A_UART                                  \
    do {                                                   \
        patch_send_char('a', NO_RESPONSE);                 \
    } while (0)
#define PATCH_SEND_N_UART                                  \
    do {                                                   \
        patch_send_char('n', NO_RESPONSE);                 \
    } while (0)

#define MSG_FORM_DRV_G 'G'
#define MSG_FORM_DRV_C 'C'
#define MSG_FORM_DRV_A 'a'
#define MSG_FORM_DRV_N 'n'

#define READ_PATCH_BUF_LEN        (1024 * 32)
#define READ_DATA_BUF_LEN         (1024 * 50)
#define READ_DATA_REALLOC_BUF_LEN (1024 * 4)

#define XMODE_DATA_LEN 1024

#define XMODEM_PAK_LEN (XMODE_DATA_LEN + 5)

/* Enum Type Definition */
enum PATCH_WAIT_RESPONSE_ENUM {
    NO_RESPONSE = 0, /* 不等待device响应 */
    WAIT_RESPONSE    /* 等待device响应 */
};

typedef wait_queue_head_t OS_WAIT_QUEUE_HEAD_T_STRU;

typedef struct patch_globals {
    uint8 auc_Cfgpath[CFG_PACH_LEN];
    uint8 auc_CfgVersion[VERSION_LEN];
    uint8 auc_DevVersion[VERSION_LEN];

    uint8 auc_Recvbuf1[RECV_BUF_LEN];
    int32 l_Recvbuf1_len;

    uint8 auc_Recvbuf2[RECV_BUF_LEN];
    int32 l_Recvbuf2_len;

    int32 l_count;
    struct cmd_type_st *pst_cmd;

    OS_WAIT_QUEUE_HEAD_T_STRU *pst_wait;
} PATCH_GLOBALS_STUR;

/* xmodem每包数据的结构，CRC校验 */
typedef struct xmodem_crc_pkt {
    int8 Head;                        /* 开始字符 */
    int8 PacketNum;                   /* 包序号 */
    int8 PacketAnt;                   /* 包序号补码 */
    int8 packet_data[XMODE_DATA_LEN]; /* 数据 */
    int8 CRCValue_H;                  /* CRC校验码高位 */
    int8 CRCValue_L;                  /* CRC校验码低位 */
} XMODEM_CRC_PKT_STRU;

/* xmodem每包数据的结构，CRC校验 */
typedef struct xmodem_head_pkt {
    int8 Head;      /* 开始字符 */
    int8 PacketNum; /* 包序号 */
    int8 PacketAnt; /* 包序号补码 */
} XMODEM_HEAD_PKT_STRU;

typedef struct ringbuf {
    uint8 *pbufstart;
    uint8 *pbufend;
    uint8 *phead;
    uint8 *ptail;
} RINGBUF_STRU;

/* Global Variable Declaring */
extern RINGBUF_STRU g_stringbuf;
extern uint8 *g_data_buf;
extern PATCH_GLOBALS_STUR g_patch_ctrl;

/* Function Declare */
extern int bfg_patch_download_function(void);
extern int32 patch_send_char(int8 num, int32 wait);
extern int32 bfg_patch_recv(const uint8 *data, int32 count);
extern int32 ps_patch_write(uint8 *data, int32 count);
extern int32 ps_recv_patch(void *disc_data, const uint8 *data, int32 count);

#endif /* end of oam_kernel.h */
