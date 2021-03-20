

/* Header File Including */
#include "wireless_patch.h"

#include <linux/init.h>
#include <linux/module.h>
#include <linux/poll.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/firmware.h>
#include <linux/fcntl.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/miscdevice.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/moduleparam.h>
#include <linux/time.h>
#include <linux/delay.h>
#include <linux/rtc.h>
#include <linux/syscalls.h>
#include <linux/dma-mapping.h>
#include <linux/stat.h>
#include <linux/string.h>

#include "plat_pm.h"
#include "securec.h"

/* Global Variable Definition */
PATCH_GLOBALS_STUR g_global[ENUM_INFO_TOTAL];
RINGBUF_STRU g_stringbuf;
uint8 *g_pucDataBuf_t;

/* xmodem ���� */
OAL_STATIC uint8 g_xmodem_packet_index = 1;

/* log ��ӡ���� */
OAL_STATIC int32 g_debuglevel_patch = 2;

int32 g_usemalloc = 0;

OAL_STATIC unsigned short g_CRC_table[CRC_TABLE_SIZE] = {
    /* CRC ��ʽ�� */
    0X0000, 0X1021, 0X2042, 0X3063, 0X4084, 0X50A5, 0X60C6, 0X70E7,
    0X8108, 0X9129, 0XA14A, 0XB16B, 0XC18C, 0XD1AD, 0XE1CE, 0XF1EF,
    0X1231, 0X0210, 0X3273, 0X2252, 0X52B5, 0X4294, 0X72F7, 0X62D6,
    0X9339, 0X8318, 0XB37B, 0XA35A, 0XD3BD, 0XC39C, 0XF3FF, 0XE3DE,
    0X2462, 0X3443, 0X0420, 0X1401, 0X64E6, 0X74C7, 0X44A4, 0X5485,
    0XA56A, 0XB54B, 0X8528, 0X9509, 0XE5EE, 0XF5CF, 0XC5AC, 0XD58D,
    0X3653, 0X2672, 0X1611, 0X0630, 0X76D7, 0X66F6, 0X5695, 0X46B4,
    0XB75B, 0XA77A, 0X9719, 0X8738, 0XF7DF, 0XE7FE, 0XD79D, 0XC7BC,
    0X48C4, 0X58E5, 0X6886, 0X78A7, 0X0840, 0X1861, 0X2802, 0X3823,
    0XC9CC, 0XD9ED, 0XE98E, 0XF9AF, 0X8948, 0X9969, 0XA90A, 0XB92B,
    0X5AF5, 0X4AD4, 0X7AB7, 0X6A96, 0X1A71, 0X0A50, 0X3A33, 0X2A12,
    0XDBFD, 0XCBDC, 0XFBBF, 0XEB9E, 0X9B79, 0X8B58, 0XBB3B, 0XAB1A,
    0X6CA6, 0X7C87, 0X4CE4, 0X5CC5, 0X2C22, 0X3C03, 0X0C60, 0X1C41,
    0XEDAE, 0XFD8F, 0XCDEC, 0XDDCD, 0XAD2A, 0XBD0B, 0X8D68, 0X9D49,
    0X7E97, 0X6EB6, 0X5ED5, 0X4EF4, 0X3E13, 0X2E32, 0X1E51, 0X0E70,
    0XFF9F, 0XEFBE, 0XDFDD, 0XCFFC, 0XBF1B, 0XAF3A, 0X9F59, 0X8F78,
    0X9188, 0X81A9, 0XB1CA, 0XA1EB, 0XD10C, 0XC12D, 0XF14E, 0XE16F,
    0X1080, 0X00A1, 0X30C2, 0X20E3, 0X5004, 0X4025, 0X7046, 0X6067,
    0X83B9, 0X9398, 0XA3FB, 0XB3DA, 0XC33D, 0XD31C, 0XE37F, 0XF35E,
    0X02B1, 0X1290, 0X22F3, 0X32D2, 0X4235, 0X5214, 0X6277, 0X7256,
    0XB5EA, 0XA5CB, 0X95A8, 0X8589, 0XF56E, 0XE54F, 0XD52C, 0XC50D,
    0X34E2, 0X24C3, 0X14A0, 0X0481, 0X7466, 0X6447, 0X5424, 0X4405,
    0XA7DB, 0XB7FA, 0X8799, 0X97B8, 0XE75F, 0XF77E, 0XC71D, 0XD73C,
    0X26D3, 0X36F2, 0X0691, 0X16B0, 0X6657, 0X7676, 0X4615, 0X5634,
    0XD94C, 0XC96D, 0XF90E, 0XE92F, 0X99C8, 0X89E9, 0XB98A, 0XA9AB,
    0X5844, 0X4865, 0X7806, 0X6827, 0X18C0, 0X08E1, 0X3882, 0X28A3,
    0XCB7D, 0XDB5C, 0XEB3F, 0XFB1E, 0X8BF9, 0X9BD8, 0XABBB, 0XBB9A,
    0X4A75, 0X5A54, 0X6A37, 0X7A16, 0X0AF1, 0X1AD0, 0X2AB3, 0X3A92,
    0XFD2E, 0XED0F, 0XDD6C, 0XCD4D, 0XBDAA, 0XAD8B, 0X9DE8, 0X8DC9,
    0X7C26, 0X6C07, 0X5C64, 0X4C45, 0X3CA2, 0X2C83, 0X1CE0, 0X0CC1,
    0XEF1F, 0XFF3E, 0XCF5D, 0XDF7C, 0XAF9B, 0XBFBA, 0X8FD9, 0X9FF8,
    0X6E17, 0X7E36, 0X4E55, 0X5E74, 0X2E93, 0X3EB2, 0X0ED1, 0X1EF0
};

uint8 g_powerpin_state;
oal_debug_module_param(g_xmodem_packet_index, int, 0);

/*
 * Prototype    : pm_uart_set_baudrate
 * Description  : set baudrate of uart0 when download patch
 * Input        : int64 baudrate: the baudrate want to set
 * Return       : 0 means succeed,-1 means failed.
 */
int32 pm_uart_set_baudrate(int64 baudrate)
{
    /* get platform driver data */
    struct pm_drv_data *pm_data = pm_get_drvdata();
    if (pm_data == NULL) {
        PS_PRINT_ERR("pm_data is NULL!\n");
        return -1;
    }

    PS_PRINT_DBG("set uart baudrate to %ld\n", baudrate);

    /* call interface supplied by 3 in 1 */
    if (pm_data->ps_pm_interface->change_baud_rate != NULL) {
        /* set uart baudrate */
        return pm_data->ps_pm_interface->change_baud_rate(baudrate, FLOW_CTRL_ENABLE);
    }

    PS_PRINT_ERR("change_baud_rate is NULL!\n");

    return -1;
}

/*
 * Prototype    : do_crc_table_1
 * Description  : CRCУ��
 */
unsigned short do_crc_table_1(uint8 *data, uint16 length)
{
    uint16 crc = 0;
    uint8 dataTmp;
    while (length > 0) {
        dataTmp = (uint8)(crc >> 8);
        crc = crc << 8;
        crc = crc ^ g_CRC_table[dataTmp ^ (*data)];
        length--;
        data++;
    }
    return crc;
}

uint16 patch_Crc16(int8 *ptr, uint16 l_count)
{
    uint16 us_crc;
    int32 i;
    const uint32 ul_loop_times = 8;

    us_crc = 0;

    while (l_count--) {
        us_crc = us_crc ^ (int)*ptr++ << 8;

        for (i = 0; i < ul_loop_times; i++) {
            if (us_crc & 0x8000) {
                us_crc = us_crc << 1 ^ 0x1021;
            } else {
                us_crc = us_crc << 1;
            }
        }
    }

    return (us_crc & 0xFFFF);
}

/*
 * Prototype    : patch_send
 * Description  : send message to device,by sdio or uart
 */
int32 patch_send(uint8 *data, int32 len, uint8 expect, int32 type)
{
    int32 i;
    int32 l_ret;

    PS_PRINT_DBG("len = %d, data = %x %x %x %x %x %x %x %x \n",
                 len, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);

    for (i = 0; i < HOST_DEV_TIMEOUT; i++) {
        l_ret = send_msg_t(data, len, type);
        if (l_ret < 0) {
            continue;
        }

        l_ret = recv_expect_result_t(expect, type);
        if (l_ret == 0) {
            return SUCC;
        }
    }
    return -EFAIL;
}

/*
 * Prototype    : patch_xmodem_send
 * Description  : xmodem protocol encapsulation for down file
 */
int32 patch_xmodem_send(uint8 *data, int32 len, uint8 expect)
{
    XMODEM_HEAD_PKT_STRU st_patch_pkt;
    uint16 us_CRCValue;
    int32 l_ret;
    int32 l_sendlen;
    int32 l_datalen;
    int32 i;
    uint8 *flag = NULL;
    uint8 CRCValue_H;
    uint8 CRCValue_L;

    PS_PRINT_DBG("enter");

    if (data == NULL) {
        return -EFAIL;
    }

    l_datalen = len;
    flag = data;

    while (l_datalen > 0) {
        l_sendlen = MIN(XMODE_DATA_LEN, l_datalen);
        l_datalen = l_datalen - l_sendlen;
        st_patch_pkt.Head = SOH;
        st_patch_pkt.PacketNum = g_xmodem_packet_index;

        /* ���ݳ��Ȳ���128�� */
        if (l_sendlen < XMODE_DATA_LEN) {
            PS_PRINT_DBG("data_len  %d\n", l_sendlen);
            memset_s(&flag[l_sendlen], (XMODE_DATA_LEN - l_sendlen), 0x00, (XMODE_DATA_LEN - l_sendlen));
        }
        us_CRCValue = do_crc_table_1(flag, XMODE_DATA_LEN);
        CRCValue_H = (us_CRCValue & 0xFF00) >> 8;
        CRCValue_L = us_CRCValue & 0xFF;

        st_patch_pkt.PacketAnt = ~(st_patch_pkt.PacketNum);

        for (i = 0; i < HOST_DEV_TIMEOUT; i++) {
            l_ret = pm_uart_send((uint8 *)&st_patch_pkt, sizeof(st_patch_pkt));
            l_ret = pm_uart_send(flag, XMODE_DATA_LEN);
            l_ret = pm_uart_send(&CRCValue_H, 1);
            l_ret = pm_uart_send(&CRCValue_L, 1);

            l_ret = recv_expect_result_t(expect, ENUM_INFO_UART);
            if (l_ret < 0) {
                continue;
            }

            if (l_ret == SUCC) {
                break;
            }
        }
        if (i >= HOST_DEV_TIMEOUT) {
            return -EFAIL;
        }
        flag = flag + l_sendlen;

        g_xmodem_packet_index++;
    }

    return SUCC;
}

/*
 * Prototype    : patch_send_file
 * Description  : send file to device,by sdio or uart
 */
int32 patch_send_file(uint8 *data, int32 len, uint8 expect, int32 type)
{
    int32 l_ret;
    PS_PRINT_DBG("len = %d, data = %x %x %x %x %x %x %x %x \n",
                 len, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);

    switch (type) {
        case ENUM_INFO_SDIO: {
            l_ret = patch_send(data, len, expect, type);
        }
        break;

        case ENUM_INFO_UART: /* uart READ */
        {
            l_ret = patch_xmodem_send(data, len, expect);
        }
        break;

        default:

            l_ret = -1;
            PS_PRINT_ERR("info type error[%d]!!!\n", type);
            break;
    }

    return l_ret;
}

/*
 * Prototype    : patch_send_data
 * Description  : send data to device,by sdio or uart
 */
int32 patch_send_data(uint8 *data, int32 len, uint8 expect, int32 type)
{
    return patch_send(data, len, expect, type);
}

static void ringbuf_flush(void)
{
    /* writing maybe still work when interrupt by flush  */
    g_stringbuf.ptail = g_stringbuf.phead;
}

/*
 * Prototype    : uart_recv_data
 * Description  : receiver data form device,by uart interface
 */
int32 uart_recv_data(const uint8 *data, int32 len)
{
    uint32 ulbuflen;
    uint32 ulheadtoendlen = 0;
    uint8 *ptail = NULL;
    if (unlikely((data == NULL))) {
        PS_PRINT_ERR("data is NULL\n ");
        return -EFAIL;
    }
    if ((g_stringbuf.pbufstart == NULL) || (g_stringbuf.pbufstart > g_stringbuf.pbufend)) {
        PS_PRINT_ERR("buf is NULL,write failed\n ");
        PS_PRINT_ERR("pbufstart=%p,pbufend=%p\n", g_stringbuf.pbufstart, g_stringbuf.pbufend);
        return -EFAIL;
    }
    ptail = g_stringbuf.ptail;
    if ((g_stringbuf.phead < g_stringbuf.pbufstart) ||
        (g_stringbuf.phead > g_stringbuf.pbufend) ||
        (ptail < g_stringbuf.pbufstart) ||
        (ptail > g_stringbuf.pbufend)) {
        PS_PRINT_ERR("phead or ptail is out of range,write failed\n");
        PS_PRINT_ERR("pbufstart=%p,pbufend=%p\n", g_stringbuf.pbufstart, g_stringbuf.pbufend);
        PS_PRINT_ERR("phead=%p,ptail=%p\n", g_stringbuf.phead, g_stringbuf.ptail);
        return -EFAIL;
    }
    ulbuflen = g_stringbuf.pbufend - g_stringbuf.pbufstart;
    PS_PRINT_DBG("len [%d],\n ", len);

    /* phead and ptail are in the same cycle */
    if (g_stringbuf.phead >= ptail) {
        /* still in the same cycle */
        if ((g_stringbuf.pbufend - g_stringbuf.phead) >= len) {
            memcpy_s(g_stringbuf.phead, g_stringbuf.pbufend - g_stringbuf.phead, data, len);
            g_stringbuf.phead += len;
            if (OS_WAITQUEUE_ACTIVE(g_global[ENUM_INFO_UART].pst_wait)) {
                OS_WAKE_UP_INTERRUPTIBLE(g_global[ENUM_INFO_UART].pst_wait);
                PS_PRINT_DBG("wake up ok");
            }
        } else if ((ulbuflen - (g_stringbuf.phead - ptail)) > len) {
            ulheadtoendlen = g_stringbuf.pbufend - g_stringbuf.phead;
            memcpy_s(g_stringbuf.phead, ulheadtoendlen, data, ulheadtoendlen);
            memcpy_s(g_stringbuf.pbufstart, g_stringbuf.tail - g_stringbuf.pbufstart,
                     data + ulheadtoendlen, len - ulheadtoendlen);
            g_stringbuf.phead = g_stringbuf.pbufstart + (len - ulheadtoendlen);
            ps_print_info("phead back\n");
            if (OS_WAITQUEUE_ACTIVE(g_global[ENUM_INFO_UART].pst_wait)) {
                OS_WAKE_UP_INTERRUPTIBLE(g_global[ENUM_INFO_UART].pst_wait);
                PS_PRINT_DBG("wake up ok");
            }
        } else {
            PS_PRINT_ERR("Not enough mem,len=%d.\n ", len);
        }
    /* phead is in the next cycle */
    } else if ((ptail - g_stringbuf.phead - 1) > len) { /* "ptail - phead = 1" means the buffer is full */
        memcpy_s(g_stringbuf.phead, ptail - g_stringbuf.phead - 1, data, len);
        g_stringbuf.phead += len;
        if (OS_WAITQUEUE_ACTIVE(g_global[ENUM_INFO_UART].pst_wait)) {
            OS_WAKE_UP_INTERRUPTIBLE(g_global[ENUM_INFO_UART].pst_wait);
            PS_PRINT_DBG("wake up ok");
        }
    } else {
        PS_PRINT_ERR("Not enough mem,len=%d.\n ", len);
    }

    if (g_stringbuf.phead >= g_stringbuf.pbufend) {
        g_stringbuf.phead = g_stringbuf.pbufstart;
        ps_print_info("phead back\n");
    }
    return SUCC;
}

int32 read_msg_t(uint8 *data, int32 len, int32 type)
{
    int32 l_len = -1;
    uint32 ultailtoendlen;
    uint8 *phead = NULL;
    PS_PRINT_DBG(" entry\n");
    if (unlikely((data == NULL))) {
        PS_PRINT_ERR("data is NULL\n ");
        return -EFAIL;
    }
    if (type == ENUM_INFO_UART) {
        if ((g_stringbuf.pbufstart == NULL) || (g_stringbuf.pbufstart > g_stringbuf.pbufend)) {
            PS_PRINT_ERR("buf is NULL,read failed\n ");
            PS_PRINT_ERR("pbufstart=%p,pbufend=%p\n", g_stringbuf.pbufstart, g_stringbuf.pbufend);
            return -EFAIL;
        }
        if ((g_stringbuf.phead < g_stringbuf.pbufstart) ||
            (g_stringbuf.phead > g_stringbuf.pbufend) ||
            (g_stringbuf.ptail < g_stringbuf.pbufstart) ||
            (g_stringbuf.ptail > g_stringbuf.pbufend)) {
            PS_PRINT_ERR("phead or ptail is out of range, read failed\n");
            PS_PRINT_ERR("pbufstart=%p,pbufend=%p\n", g_stringbuf.pbufstart, g_stringbuf.pbufend);
            PS_PRINT_ERR("phead=%p,ptail=%p\n", g_stringbuf.phead, g_stringbuf.ptail);
            return -EFAIL;
        }
    }
    switch (type) {
        case ENUM_INFO_SDIO:
            break;
        case ENUM_INFO_UART: {
            OS_WAIT_EVENT_INTERRUPTIBLE_TIMEOUT(*g_global[ENUM_INFO_UART].pst_wait,
                                                (g_stringbuf.phead != g_stringbuf.ptail),
                                                PATCH_INTEROP_TIMEOUT);
            phead = g_stringbuf.phead;
            /* phead and ptail are in the same cycle */
            if (phead > g_stringbuf.ptail) {
                if ((phead - g_stringbuf.ptail) > len) {
                    memcpy_s(data, len, g_stringbuf.ptail, len);
                    l_len = len;
                    g_stringbuf.ptail += len;
                } else { /* not enough data */
                    l_len = phead - g_stringbuf.ptail;
                    memcpy_s(data, len, g_stringbuf.ptail, l_len);
                    g_stringbuf.ptail += l_len;
                }
            /* phead is in the next cycle */
            } else if (phead < g_stringbuf.ptail) {
                ultailtoendlen = g_stringbuf.pbufend - g_stringbuf.ptail;
                if (ultailtoendlen > len) {
                    memcpy_s(data, len, g_stringbuf.ptail, len);
                    l_len = len;
                    g_stringbuf.ptail += len;
                } else {
                    memcpy_s(data, len, g_stringbuf.ptail, ultailtoendlen);
                    if ((phead - g_stringbuf.pbufstart) > (len - ultailtoendlen)) {
                        memcpy_s(data + ultailtoendlen, len - ultailtoendlen,
                                 g_stringbuf.pbufstart, len - ultailtoendlen);
                        g_stringbuf.ptail = g_stringbuf.pbufstart + (len - ultailtoendlen);
                        l_len = len;
                        ps_print_info("ptail back\n");
                    } else {
                        memcpy_s(data + ultailtoendlen, len - ultailtoendlen,
                                 g_stringbuf.pbufstart, phead - g_stringbuf.pbufstart);
                        l_len = ultailtoendlen + (phead - g_stringbuf.pbufstart);
                        g_stringbuf.ptail = phead;
                    }
                }
            } else {
                l_len = -1;
                PS_PRINT_WARNING("No data.\n");
            }
            if (g_stringbuf.ptail >= g_stringbuf.pbufend) {
                g_stringbuf.ptail = g_stringbuf.pbufstart;
            }
        }
        break;

        default:

            l_len = -1;
            PS_PRINT_ERR("info type error[%d]!!!\n", type);
            break;
    }

    return l_len;
}

/*
 * Prototype    : send_msg_t
 * Description  : send message to device,by sdio or uart
 */
int32 send_msg_t(uint8 *data, int32 len, int32 type)
{
    int32 l_ret = -1;
    PS_PRINT_DBG("len = %d, data = %x %x %x %x %x %x %x %x \n",
                 len, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);

    switch (type) {
        case ENUM_INFO_SDIO: 
            break;

        case ENUM_INFO_UART:
            l_ret = pm_uart_send(data, len);
            break;

        default:
            l_ret = -EFAIL;
            PS_PRINT_ERR("info type error[%d]!!!\n", type);
            break;
    }

    return l_ret;
}

/*
 * Prototype    : recv_expect_result_t
 * Description  : receive result form device
 */
int32 recv_expect_result_t(uint8 expect, int32 type)
{
    uint8 auc_buf[RECV_BUF_LEN];
    int32 l_len;
    int32 i;

    PS_PRINT_DBG(" entry\n");

    memset_s(auc_buf, RECV_BUF_LEN, 0, RECV_BUF_LEN);
    for (i = 0; i < HOST_DEV_TIMEOUT; i++) {
        l_len = read_msg_t(auc_buf, 1, type);
        if (l_len < 0) {
            PS_PRINT_ERR("recv result fail\n");
            return -EFAIL;
        }
        if (auc_buf[0] == expect) {
            PS_PRINT_DBG(" send SUCC [%x]\n", expect);
            return SUCC;
        }
        /*
         * NAK: �ļ�����ʱ�ط���ʶ
         * MSG_FORM_DRV_N:�����ط���ʶ
         */
        else if ((auc_buf[0] == MSG_FORM_DRV_N) || (auc_buf[0] == NAK) || (auc_buf[0] == MSG_FORM_DRV_C)) {
            PS_PRINT_ERR(" send again [0x%x]\n", auc_buf[0]);
            return -EFAIL;
        } else {
            /* ���ڴ���Ľ������ʮ�εĻ��ᣬ */
            if (type == ENUM_INFO_SDIO) {
                PATCH_SEND_N_SDIO;
            } else {
                PATCH_SEND_N_UART;
            }
            PS_PRINT_WARNING(" error result[0x%x], expect [0x%x], read result again\n", auc_buf[0], expect);
        }
    }
    return -EFAIL;
}

/*
 * Prototype    : patch_string_to_num
 * Description  : string to number
 * Input        : int8 *string
 */
int32 patch_string_to_num(uint8 *string)
{
    int32 i;
    int32 l_num;
    PS_PRINT_DBG(" entry\n");
    if (string == NULL) {
        return -EFAIL;
    }

    l_num = 0;
    for (i = 0; (string[i] >= '0') && (string[i] <= '9'); i++) {
        l_num = (l_num * 10) + (string[i] - '0');  /* �ַ���ת���ֵ��߼���Ҫ */
    }

    return l_num;
}

/*
 * Prototype    : patch_wait_g_form_dev
 * Description  : wait go'command form device
 */
int32 patch_wait_g_form_dev(int32 type)
{
    int32 l_ret;
    int32 i;

    PS_PRINT_DBG(" entry\n");

    for (i = 0; i < HOST_DEV_TIMEOUT; i++) {
        l_ret = recv_expect_result_t(MSG_FORM_DRV_G, type);
        if (l_ret == 0) {
            PS_PRINT_DBG(" device finish G\n");
            return SUCC;
        }
    }
    return -EFAIL;
}

/*
 * Prototype    : patch_wait_g_retry_form_dev
 * Description  : wait go'command form device
 */
int32 patch_wait_g_retry_form_dev(int32 type)
{
    int32 l_ret;
    int32 i;

    PS_PRINT_DBG(" entry\n");

    for (i = 0; i < HOST_DEV_TIMEOUT; i++) {
        l_ret = recv_expect_result_t(MSG_FORM_DRV_G, type);
        if (l_ret == 0) {
            PS_PRINT_DBG(" device finish G\n");
            return SUCC;
        } else {
            if (type == ENUM_INFO_UART) {
                PATCH_SEND_N_UART;
            }
            PS_PRINT_WARNING("receive G failed\n");
        }
    }
    return -EFAIL;
}

/*
 * Prototype    : patch_send_char
 * Description  : send char to device
 */
int32 patch_send_char(int8 num, int32 wait, int32 type)
{
    int32 l_ret;
    const uint32 ul_buf_size = 8;
    uint8 auc_buf[ul_buf_size];
    int32 i;
    PS_PRINT_DBG("enter");

    memset_s(auc_buf, sizeof(auc_buf), num, sizeof(auc_buf));
    PS_PRINT_DBG("send [0x%x], wait[%d]\n", num, wait);
    for (i = 0; i < HOST_DEV_TIMEOUT; i++) {
        /*
         * sdio �ӿڷ���ʱ�������ֽڶ��룬�����ĸ�
         * uart �ӿڷ���ʱ��ֻ����һ��
         */
        l_ret = send_msg_t(auc_buf, 1, type);
        if (l_ret < 0) {
            PS_PRINT_ERR("Send fail\n");
            return l_ret;
        }

        if (wait == WAIT_RESPONSE) {
            l_ret = recv_expect_result_t(ACK, type);
            if (l_ret < 0) {
                continue;
            }
        }

        return l_ret;
    }

    return -EFAIL;
    ;
}

/*
 * Prototype    : patch_read_patch
 * Description  : read patch
 */
int32 patch_read_patch(int8 *buf, int32 len, OS_KERNEL_FILE_STRU *fp)
{
    int32 rdlen;

    if ((IS_ERR(fp)) || (buf == NULL)) {
        fp = NULL;
        PS_PRINT_ERR("buf/fp is NULL\n");
        return -EFAIL;
    }

    rdlen = kernel_read(fp, fp->f_pos, buf, len);
    if (rdlen > 0) {
        fp->f_pos += rdlen;
    }

    return rdlen;
}

/*
 * Prototype    : patch_down_file
 * Description  : begin download patch file
 */
int32 patch_down_file(uint8 *puc_file, int32 type)
{
    uint8 *auc_buf;
    int32 l_len;
    int32 l_ret;
    int32 l_count;
    mm_segment_t fs;
    OS_KERNEL_FILE_STRU *fp = NULL;

    PS_PRINT_DBG("enter");

    if (puc_file == NULL) {
        return -EFAIL;
    }

    fs = get_fs();
    set_fs(KERNEL_DS);
    fp = filp_open(puc_file, O_RDONLY, 0);
    if (IS_ERR(fp)) {
        set_fs(fs);
        fp = NULL;
        PS_PRINT_ERR("filp_open %s fail!!\n", puc_file);
        return -EFAIL;
    }

    if (g_pucDataBuf_t == NULL) {
        filp_close(fp, NULL);
        set_fs(fs);
        fp = NULL;
        return -EFAIL;
    }

    l_count = 1;
    g_xmodem_packet_index = 1;
    while (1) {
        l_len = patch_read_patch(g_pucDataBuf_t, READ_PATCH_BUF_LEN, fp);

        PS_PRINT_DBG("kernel_read len[%d] [%d]\n", l_len, l_count);
        /* ������ȡ�ļ� */
        if ((l_len > 0) && (l_len <= READ_PATCH_BUF_LEN)) {
            l_ret = patch_send_file(g_pucDataBuf_t, l_len, ACK, type);
            PS_PRINT_DBG("patch_send_file:%d", l_ret);
            if (l_ret == -EFAIL) {
                PS_PRINT_ERR(" uart send data[%d] fail\n", l_count);
                break;
            }
        } else if (l_len == 0) { /* �ļ��Ѿ���ȡ��� */
            if (type == ENUM_INFO_SDIO) {
                PATCH_SEND_EOT_SDIO;
            } else {
                PATCH_SEND_EOT_UART;
                g_xmodem_packet_index = 1;
            }
            l_ret = SUCC;

            PS_PRINT_DBG("read file[%d] [%d] send EOT\n", l_count, l_len);
            break;
        } else { /* ��ȡ�ļ����� */
            if (type == ENUM_INFO_SDIO) {
                PATCH_SEND_CAN_SDIO;
            } else {
                PATCH_SEND_CAN_UART;
                g_xmodem_packet_index = 1;
            }
            l_ret = -EFAIL;
            PS_PRINT_ERR("read file[%d] [%d]\n", l_count, l_len);
            break;
        }
        l_count++;
    }
 
    auc_buf = NULL;

    filp_close(fp, NULL);
    set_fs(fs);
    fp = NULL;
    PS_PRINT_DBG("%s send finish\n", puc_file);

    return l_ret;
}

/*
 * Prototype    : patch_readm_fileopen
 * Description  : creat and open file to save mem
 */
OS_KERNEL_FILE_STRU *patch_readm_fileopen(int32 type)
{
    OS_KERNEL_FILE_STRU *fp = NULL;
    struct timeval tv;
    struct rtc_time tm;
    const uint32 ul_filename_len = 50;
    char filename[ul_filename_len];
    do_gettimeofday(&tv);
    rtc_time_to_tm(tv.tv_sec, &tm);
    int ret;

    ps_print_info("%4d-%02d-%02d  %02d:%02d:%02d\n",
                  tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec); /* ת���ɵ�ǰʱ�� */
    if (type == ENUM_INFO_SDIO) {
        ret = snprintf_s(filename, sizeof(filename), sizeof(filename) - 1,
                         "/data/memdump/readm_wifi%04d%02d%02d%02d%02d%02d.bin",
                         tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
                         tm.tm_hour, tm.tm_min, tm.tm_sec); /* ת���ɵ�ǰʱ�� */
        if (ret < 0) {
            PS_PRINT_ERR("log str format err line[%d]\n", __LINE__);
            return NULL;
        }
    } else {
        ret = snprintf_s(filename, sizeof(filename), sizeof(filename) - 1,
                         "/data/memdump/readm_bfg%04d%02d%02d%02d%02d%02d.bin",
                         tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
                         tm.tm_hour, tm.tm_min, tm.tm_sec); /* ת���ɵ�ǰʱ�� */
        if (ret < 0) {
            PS_PRINT_ERR("log str format err line[%d]\n", __LINE__);
            return NULL;
        }
    }
    ps_print_info("filename = %s", filename);
    fp = filp_open(filename, O_RDWR | O_CREAT, 0644);
    return fp;
}

/*
 * Prototype    : patch_recv_mem
 * Description  : receive memory information form device
 */
int32 patch_recv_mem(OS_KERNEL_FILE_STRU *fp, int32 len, int32 type)
{
    uint8 *pdatabuf = NULL;
    uint8 buf;
    int32 l_ret;
    int32 count;
    mm_segment_t fs;
    uint8 retry = 3;
    int32 lenbuf = 0;
    if (IS_ERR(fp)) {
        PS_PRINT_ERR("fp is error,fp = 0x%p\n", fp);
        return -EFAIL;
    }

    PS_PRINT_DBG("expect recv len is [%d]\n", len);
    count = 0;

    l_ret = read_msg_t(&buf, 1, type);
    pdatabuf = OS_KMALLOC_GFP(len);
    if (pdatabuf == NULL) {
        filp_close(fp, NULL);
        return -EFAIL;
    }
    memset_s(pdatabuf, len, 0, len);
    if (buf != 'a') {
        PS_PRINT_DBG("receive a fail, readmem anyway\n");
    }

    fs = get_fs();
    set_fs(KERNEL_DS);
    l_ret = vfs_llseek(fp, 0, SEEK_END);
 
    while (len > lenbuf) { // RECV_BUF_LEN
        l_ret = read_msg_t(pdatabuf + lenbuf, len - lenbuf, type);
        if (l_ret > 0) {
            lenbuf += l_ret;
        } else {
            retry--;
            lenbuf = 0;
            if (0 == retry) {
                l_ret = -EFAIL;
                PS_PRINT_ERR("time out\n");
                break;
            }
            if (type == ENUM_INFO_SDIO) {
                PATCH_SEND_N_SDIO;
            } else {
                ringbuf_flush();
                PATCH_SEND_N_UART;
            }
        }
    }
    if (len <= lenbuf) {
        vfs_write(fp, pdatabuf, len, &fp->f_pos);
    }
    filp_close(fp, NULL);
    set_fs(fs);
    if (type == ENUM_INFO_SDIO) {
        PATCH_SEND_A_SDIO;
    } else {
        ringbuf_flush();
        PATCH_SEND_A_UART;
    }
    OS_MEM_KFREE(pdatabuf);
    return l_ret;
}

/*
 * Prototype    : patch_int_para_send
 * Description  : down integer to device
 */
int32 patch_int_para_send(uint8 *name, uint8 *Value, int32 type)
{
    int32 l_ret;
    int32 data_len;
    int32 Value_len;
    int32 i;
    int32 n;
    uint8 auc_num[INT32_STR_LEN];
    uint8 data[DATA_BUF_LEN];

    PS_PRINT_DBG("entry ");

    Value_len = OS_STR_LEN((int8 *)Value);

    memset_s(auc_num, INT32_STR_LEN, 0, INT32_STR_LEN);
    memset_s(data, DATA_BUF_LEN, 0, DATA_BUF_LEN);

    data_len = 0;
    data_len = OS_STR_LEN(name);
    l_ret = memcpy_s(data, sizeof(data), name, data_len);
    if (l_ret != EOK) {
        PS_PRINT_ERR("data buff not enough\n");
        return -EFAIL;
    }

    data[data_len] = COMPART_KEYWORD;
    data_len = data_len + 1;

    for (i = 0, n = 0; (i <= Value_len) && (n < INT32_STR_LEN); i++) {
        if ((Value[i] == ',') || (Value_len == i)) {
            PS_PRINT_DBG("auc_num = %s, i = %d, n = %d\n", auc_num, i, n);
            if (n == 0) {
                continue;
            }
            l_ret = memcpy_s((uint8 *)&data[data_len], sizeof(data) - data_len, auc_num, n);
            if (l_ret != EOK) {
                PS_PRINT_ERR("data buff not enough\n");
                return -EFAIL;
            }
            data_len = data_len + n;

            data[data_len] = COMPART_KEYWORD;
            data_len = data_len + 1;

            memset_s(auc_num, INT32_STR_LEN, 0, INT32_STR_LEN);
            n = 0;
        } else if (Value[i] == 0x20) {
            continue;
        } else {
            auc_num[n] = Value[i];
            n++;
        }
    }

    PS_PRINT_DBG("data_len = %d, \n", data_len);
    PS_PRINT_DBG("data = %s, \n", data);
 
    if (type == ENUM_INFO_UART) {
        ringbuf_flush();
    }
    l_ret = send_msg_t(data, data_len, type);

    return l_ret;
}

int32 patch_number_type(uint8 *Key, uint8 *Value, int32 type)
{
    int32 l_ret = -EFAIL;
    int32 num;
    uint8 *flag = NULL;
    uint8 pinstate;
    OS_KERNEL_FILE_STRU *fp = NULL;

    PS_PRINT_DBG("entry");

    if (!OS_MEM_CMP(Key, VER_CMD_KEYWORD, OS_STR_LEN(VER_CMD_KEYWORD))) {
        PS_PRINT_DBG("version \n");
        return SUCC;
    }

    if (!OS_STR_CMP((int8 *)Key, PLL_CMD_KEYWORD) || !OS_STR_CMP((int8 *)Key, JUMP_CMD_KEYWORD) ||
        !OS_STR_CMP((int8 *)Key, WMEM_CMD_KEYWORD)) {
        l_ret = patch_int_para_send(Key, Value, type);
        if (l_ret < 0) {
            PS_PRINT_ERR("send %s,%s fail \n", Key, Value);
            return l_ret;
        }
        /* G ��device ��Ӧ������ɱ�־ */
        PS_PRINT_DBG("recv g form device\n");
        l_ret = patch_wait_g_form_dev(type);
        if (l_ret < 0) {
            PS_PRINT_ERR("recv g FAIL [%d]\n", l_ret);
            return -EFAIL;
        }
    } else if (!OS_STR_CMP((int8 *)Key, BRT_CMD_KEYWORD)) {
        /* �޸Ĳ����� */
        l_ret = patch_int_para_send(Key, Value, type);
        if (l_ret < 0) {
            PS_PRINT_ERR("send %s,%s fail \n", Key, Value);
            return l_ret;
        }
        PS_PRINT_DBG("change baudrate\n");
        msleep(1); 

        num = patch_string_to_num(Value);

        /* ���ӵ����޸Ĳ����ʺ��� */
        l_ret = pm_uart_set_baudrate(num);
        if (l_ret < 0) {
            PS_PRINT_ERR(" modify baudrate fail!!\n");
            return -EFAIL;
        }
        ringbuf_flush();
        msleep(10);
        PATCH_SEND_N_UART;
        /* G ��device ��Ӧ������ɱ�־ */
        PS_PRINT_DBG("recv g form device\n");
        l_ret = patch_wait_g_retry_form_dev(type);
        if (l_ret < 0) {
            return -EFAIL;
        }
    } else if (!OS_STR_CMP((int8 *)Key, RMEM_CMD_KEYWORD)) {
        /* not enough mem to read mem */
        if ((g_usemalloc == 0) && (type == ENUM_INFO_UART)) {
            return SUCC;
        }
        g_powerpin_state &= PINSTATE_MASK;
        pinstate = (g_powerpin_state & (g_powerpin_state >> PINSTATE_NUM));
        /* disabled */
        if ((pinstate == PINENABLE) ||
            ((pinstate & BFG_PINDISABLE) == 0 && (type == ENUM_INFO_UART)) ||
            ((pinstate & WLAN_PINDISABLE) == 0 && (type == ENUM_INFO_SDIO))) {
            ps_print_info("pinstart bfg_pindisable\n");
            return SUCC;
        }
        fp = patch_readm_fileopen(type);
        if (IS_ERR(fp)) {
            return SUCC;
        }
        l_ret = patch_int_para_send(Key, Value, type);
        if (l_ret < 0) {
            PS_PRINT_ERR("send %s,%s fail \n", Key, Value);
            filp_close(fp, NULL);
            return l_ret;
        }
        PS_PRINT_DBG("recv mem form device\n");
        flag = OS_STR_CHR(Value, ',');
        if (flag == NULL) {
            PS_PRINT_ERR("RECV LEN ERROR..\n");
            filp_close(fp, NULL);
            return -EFAIL;
        }
        flag++;
        PS_PRINT_DBG("recv len [%s]\n", flag);
        while (*flag == COMPART_KEYWORD) {
            flag++;
        }
        num = patch_string_to_num(flag);

        PS_PRINT_DBG("recv len [%d]\n", num);

        l_ret = patch_recv_mem(fp, num, type);
    }
    return l_ret;
}

/*
 * Prototype    : patch_quit_type
 * Description  : down quit command
 */
int32 patch_quit_type(int32 type)
{
    int32 l_ret;
    int32 l_len;
    const uint32 ul_buf_len = 8;
    uint8 buf[ul_buf_len];
    PS_PRINT_DBG("entry\n");

    memset_s(buf, sizeof(buf), 0, sizeof(buf));

    l_ret = memcpy_s(buf, sizeof(buf), (uint8 *)QUIT_CMD, OS_STR_LEN(QUIT_CMD));
    if (l_ret != EOK) {
        PS_PRINT_ERR("buf not enough\n");
        return -EFAIL;
    }
    l_len = OS_STR_LEN(QUIT_CMD);

    buf[l_len] = COMPART_KEYWORD;
    l_len++;

    if (type == ENUM_INFO_UART) {
        ps_patch_to_nomal();
    }
    l_ret = send_msg_t(buf, l_len, type);

    return l_ret;
}

/*
 * Prototype    : patch_file_addr_send
 * Description  : down addr and file
 */
int32 patch_file_addr_send(uint8 *data, int32 data_len, const char *file_path, int32 type)
{
    int32 l_ret;

    l_ret = patch_send_data(data, data_len, MSG_FORM_DRV_C, type);
    if (l_ret < 0) {
        PS_PRINT_ERR(" SEND file addr error\n");

        return -EFAIL;
    }

    PS_PRINT_DBG("file path is %s\n", file_path);

    l_ret = patch_down_file(file_path, type);
    PS_PRINT_DBG("patch_down_file:%d", l_ret);
    if (l_ret < 0) {
        PS_PRINT_ERR(" SEND file error\n");

        return l_ret;
    }
    /* G �� DEVICE �����Ӧ������־ */
    l_ret = patch_wait_g_form_dev(type);

    return l_ret;
}

/*
 * Prototype    : patch_file_type
 * Description  : down addr and file
 */
int32 patch_file_type(uint8 *Key, const char *Value, int32 type)
{
    int32 i;
    int32 n;
    int32 l_ret;
    int32 l_len;
    uint8 auc_addr[INT32_STR_LEN];
    uint8 data[DATA_BUF_LEN];
    int32 data_len;
    const char *tmp = NULL;
    const char *tmp1 = NULL;

    PS_PRINT_DBG("Key = %s, Value = %s\n", Key, Value);

    /*
     * ���ݹؼ��ֵ����һ���ַ���ȷ�����͵�ַ֮��device�ķ���ֵ
     * ���������ļ��Ĺؼ��ֲ��������޸�
     */
    memset_s(data, DATA_BUF_LEN, 0, DATA_BUF_LEN);
    data_len = OS_STR_LEN(Key);
    l_ret = memcpy_s(data, sizeof(data), Key, data_len);
    if (l_ret != EOK) {
        PS_PRINT_ERR("data not enough\n");
        return -EFAIL;
    }

    data[data_len] = COMPART_KEYWORD;
    data_len++;

    /* ����wifibootloader�������� */
    tmp1 = Value;
    while (*tmp1 == ' ') {
        tmp1++;
    }
    if (*tmp1 != '1') {
        PS_PRINT_ERR("para [%s] not begin with 1", tmp1);
        return -EFAIL;
    }
    tmp = OS_STR_CHR(tmp1, ',');
    if (tmp == NULL) {
        PS_PRINT_ERR("has no ',' string:[%s]", tmp);
        return -EFAIL;
    }
    tmp++;
    PS_PRINT_DBG("tmp is %s\n", tmp);

    memset_s(auc_addr, INT32_STR_LEN, 0, INT32_STR_LEN);
    for (i = 0, n = 0; tmp[i] != ',' && n < INT32_STR_LEN; i++) {
        if ((tmp[i] == ',') || (tmp[i] == COMPART_KEYWORD)) {
            break;
        } else {
            auc_addr[n] = tmp[i];
            n++;
        }
    }
    l_ret = memcpy_s((uint8 *)&data[data_len], sizeof(data) - data_len, auc_addr, n);
    if (l_ret != EOK) {
        PS_PRINT_ERR("data buff not enough\n");
        return -EFAIL;
    }
    data_len = data_len + n;

    data[data_len] = COMPART_KEYWORD;
    data_len++;

    PS_PRINT_DBG("data is %s\n", data);

    /* ɾ��ͷ���Ŀո� */
    l_len = OS_STR_LEN((int8 *)tmp);
    for (i = i + 1; i < l_len; i++) {
        if ((tmp[i] == '/') || (tmp[i] == '.')) { /* ���ݾ���·�������·�� */
            break;
        }
    }

    l_ret = patch_file_addr_send(data, data_len, &tmp[i], type);
    return l_ret;
}
/*
 * Prototype    : patch_device_respond
 * Description  : wait respond form device
 */
int32 patch_device_respond(int32 type)
{
    int32 l_ret;
    int32 i;

    PS_PRINT_DBG("entry");
    for (i = 0; i < HOST_DEV_TIMEOUT; i++) {
        if (type == ENUM_INFO_SDIO) {
            //            PATCH_SEND_N_SDIO;
        } else {
            PATCH_SEND_N_UART;
        }
        memset_s(g_global[type].auc_DevVersion, VERSION_LEN, 0, VERSION_LEN);
        msleep(1);
        l_ret = read_msg_t(g_global[type].auc_DevVersion, VERSION_LEN, type);
        if (l_ret < 0) {
            PS_PRINT_ERR("read fail![%d]\n", i);
            continue;
        } else if (!OS_MEM_CMP((int8 *)g_global[type].auc_DevVersion,
                               (int8 *)g_global[type].auc_CfgVersion,
                               OS_STR_LEN(g_global[type].auc_CfgVersion))) {
            if (type == ENUM_INFO_SDIO) {
                PATCH_SEND_A_SDIO;
                msleep(10);
            } else {
                PATCH_SEND_A_UART;
            }
            ps_print_info("Device Version = [%s], CfgVersion = [%s].\n",
                          g_global[type].auc_DevVersion, g_global[type].auc_CfgVersion);
            return SUCC;
        } else {
            if (type == ENUM_INFO_SDIO) {
                //                PATCH_SEND_N_SDIO;
            } else {
                PATCH_SEND_N_UART;
            }

            ps_print_info("Device Version = [%s], CfgVersion = [%s].\n",
                          g_global[type].auc_DevVersion, g_global[type].auc_CfgVersion);
        }
    }

    PS_PRINT_ERR("read device version fail![%d]\n", i);

    return -EFAIL;
}

/*
 * Prototype    : patch_del_space
 * Description  : delete space
 */
uint8 *patch_del_space(uint8 *string, int32 *len)
{
    int i;
    PS_PRINT_DBG("entry");
    if ((string == NULL) || (len == NULL)) {
        return NULL;
    }

    /* ɾ��β���Ŀո� */
    for (i = *len - 1; i >= 0; i--) {
        if (string[i] != COMPART_KEYWORD) {
            break;
        }
        string[i] = '\0';
    }
    /* ���� */
    if (i < 0) {
        PS_PRINT_ERR(" string is Space bar\n");
        return NULL;
    }
    /* ��for����м�ȥ1���������1 */
    *len = i + 1;

    /* ɾ��ͷ���Ŀո� */
    for (i = 0; i < *len; i++) {
        if (string[i] != ' ') {
            /* ��ȥ�ո�ĸ��� */
            *len = *len - i;
            return &string[i];
        }
    }

    return NULL;
}

/*
 * Prototype    : patch_read_cfg
 * Description  : read config file
 */
int32 patch_read_cfg(uint8 *cfg_path, uint8 *read_buf)
{
    mm_segment_t fs;
    int32 l_ret;
    OS_KERNEL_FILE_STRU *fp = NULL;

    PS_PRINT_DBG("entry\n");
    if ((cfg_path == NULL) || (read_buf == NULL)) {
        PS_PRINT_ERR("para is NULL\n");
        return -EFAIL;
    }

    fs = get_fs();
    set_fs(KERNEL_DS);
    fp = filp_open(cfg_path, O_RDONLY, 0);
    if (IS_ERR(fp)) {
        set_fs(fs);
        fp = NULL;
        PS_PRINT_ERR("filp_open %s fail\n", cfg_path);

        return -EFAIL;
    }

    memset_s(read_buf, READ_CFG_BUF_LEN, 0, READ_CFG_BUF_LEN);
    l_ret = kernel_read(fp, fp->f_pos, read_buf, READ_CFG_BUF_LEN);

    filp_close(fp, NULL);
    set_fs(fs);
    fp = NULL;

    return l_ret;
}

/*
 * Prototype    : patch_parse_cmd
 * Description  : parse config command
 */
int32 patch_parse_cmd(uint8 *buf, uint8 *cmd_name, uint32 cmd_name_len, uint8 *cmd_para, uint32 cmd_para_len)
{
    int32 ret;
    int32 l_ret;
    int32 l_cmdlen;
    int32 l_paralen;
    uint8 *begin;
    uint8 *end = NULL;
    uint8 *link = NULL;
    uint8 *handle = NULL;

    begin = buf;
    if ((buf == NULL) || (cmd_name == NULL) || (cmd_para == NULL)) {
        PS_PRINT_ERR("buf/cmd_name/cmd_para is NULL\n");
        return ERROR_TYPE_CMD;
    }

    /* ע���� */
    if (buf[0] == '@') {
        return ERROR_TYPE_CMD;
    }

    /* �����У������˳������� */
    link = OS_STR_CHR((int8 *)begin, '=');
    if (link == NULL) {
        /* �˳������� */
        if (NULL != OS_STR_STR((int8 *)buf, QUIT_CMD_KEYWORD)) {
            return QUIT_TYPE_CMD;
        }

        return ERROR_TYPE_CMD;
    }

    /* �����У�û�н����� */
    end = OS_STR_CHR(link, ';');
    if (end == NULL) {
        return ERROR_TYPE_CMD;
    }

    l_cmdlen = link - begin;

    /* ɾ���ؼ��ֵ����߿ո� */
    handle = patch_del_space((uint8 *)begin, &l_cmdlen);
    if (handle == NULL) {
        return ERROR_TYPE_CMD;
    }

    /* �ж��������� */
    if (!OS_MEM_CMP(handle, (uint8 *)FILE_TYPE_CMD_KEY, OS_STR_LEN((uint8 *)FILE_TYPE_CMD_KEY))) {
        handle = OS_STR_STR(handle, (uint8 *)FILE_TYPE_CMD_KEY) + OS_STR_LEN(FILE_TYPE_CMD_KEY);
        l_cmdlen = l_cmdlen - OS_STR_LEN(FILE_TYPE_CMD_KEY);
        l_ret = FILE_TYPE_CMD;
    } else if (!OS_MEM_CMP(handle, (uint8 *)NUM_TYPE_CMD_KEY, OS_STR_LEN(NUM_TYPE_CMD_KEY))) {
        handle = OS_STR_STR(handle, (uint8 *)NUM_TYPE_CMD_KEY) + OS_STR_LEN(NUM_TYPE_CMD_KEY);
        l_cmdlen = l_cmdlen - OS_STR_LEN(NUM_TYPE_CMD_KEY);
        l_ret = NUM_TYPE_CMD;
    } else {
        return ERROR_TYPE_CMD;
    }
    l_ret = memcpy_s(cmd_name, cmd_name_len, handle, l_cmdlen);
    if (l_ret != EOK) {
        PS_PRINT_ERR("cmd_name buf not enough\n");
        return -EFAIL;
    }

    /* ɾ��ֵ���߿ո� */
    begin = link + 1;
    l_paralen = end - begin;

    handle = patch_del_space((uint8 *)begin, &l_paralen);
    if (handle == NULL) {
        return ERROR_TYPE_CMD;
    }
    l_ret = memcpy_s(cmd_para, cmd_para_len, handle, l_paralen);
    if (l_ret != EOK) {
        PS_PRINT_ERR("cmd_para buf not enough\n");
        return -EFAIL;
    }

    return SUCC;
}

/*
 * Prototype    : patch_malloc_cmd_buf
 * Description  : malloc cmd buf
 */
void *patch_malloc_cmd_buf(uint8 *buf, int32 type)
{
    int32 l_len;
    uint8 *flag;
    uint8 *p_buf = NULL;

    /* ͳ��������� */
    flag = buf;
    g_global[type].l_count = 0;
    while (flag != NULL) {
        /* һ����ȷ�������н�����Ϊ ; */
        flag = OS_STR_CHR(flag, CMD_LINE_SIGN);
        if (flag == NULL) {
            break;
        }
        g_global[type].l_count++;
        flag++;
    }
    PS_PRINT_DBG("l_count = %d\n", g_global[type].l_count);

    /* ����洢����ռ� */
    /* ��ʵ�ʶ��һ������ռ䡣�����ķ�ֹ�����ļ����һ��������˳����� */
    l_len = (g_global[type].l_count + 1) * sizeof(struct cmd_type_st);
    p_buf = OS_KMALLOC_GFP(l_len);
    if (p_buf == NULL) {
        PS_PRINT_ERR("kmalloc cmd_type_st fail\n");
        return NULL;
    }
    memset_s((void *)p_buf, l_len, 0, l_len);

    return p_buf;
}

/*
 * Prototype    : patch_parse_cfg
 * Description  : parse config file
 */
int32 patch_parse_cfg(uint8 *buf, int32 buf_len, int32 type)
{
    int32 i;
    int32 l_len;
    uint8 *flag = NULL;
    uint8 *begin = NULL;
    uint8 *end = NULL;
    int32 cmd_type;
    uint8 cmd_name[CMD_LEN];
    uint8 cmd_para[PARA_LEN];

    g_global[type].pst_cmd = (struct cmd_type_st *)patch_malloc_cmd_buf(buf, type);
    if (g_global[type].pst_cmd == NULL) {
        PS_PRINT_ERR(" patch_malloc_cmd_buf fail\n");
        return -EFAIL;
    }

    /* ����CMD BUF */
    flag = buf;
    /* ����Ĵ洢����ռ乻�ã�bufû�ж�ȡ��� */ 
    l_len = buf_len;
    i = 0;
    while ((i < g_global[type].l_count) && (flag < &buf[l_len])) {
        /*
         * ��ȡ�����ļ��е�һ��,�����ļ�������unix��ʽ.
         * �����ļ��е�ĳһ�к����ַ� @ ����Ϊ����Ϊע����
         */
        begin = flag;
        end = OS_STR_CHR(flag, '\n');
        if (end == NULL) {
            ps_print_info("end is null\n");
            break;
        }
        if (end == begin) { /* ����ֻ��һ�����з� */
            PS_PRINT_DBG("blank line\n");
            flag = end + 1;
            continue;
        }
        *end = '\0';
        PS_PRINT_DBG("operation string is [%s]\n", begin);
        memset_s(cmd_name, CMD_LEN, 0, CMD_LEN);
        memset_s(cmd_para, PARA_LEN, 0, PARA_LEN);

        cmd_type = patch_parse_cmd(begin, cmd_name, sizeof(cmd_name), cmd_para, sizeof(cmd_para));
        if (cmd_type != ERROR_TYPE_CMD) { /* ��ȷ���������ͣ����� */ 
            g_global[type].pst_cmd[i].cmd_type = cmd_type;
            memcpy_s(g_global[type].pst_cmd[i].cmd_name, sizeof(g_global[type].pst_cmd[i].cmd_name),
                     cmd_name, CMD_LEN);
            memcpy_s(g_global[type].pst_cmd[i].cmd_para, sizeof(g_global[type].pst_cmd[i].cmd_para),
                     cmd_para, PARA_LEN);
            /* ��ȡ���ð汾�� */
            if (!OS_MEM_CMP(g_global[type].pst_cmd[i].cmd_name,
                            VER_CMD_KEYWORD,
                            OS_STR_LEN(VER_CMD_KEYWORD))) {
                memcpy_s(g_global[type].auc_CfgVersion, sizeof(g_global[type].auc_CfgVersion),
                         g_global[type].pst_cmd[i].cmd_para, VERSION_LEN);
                PS_PRINT_DBG("g_CfgVersion = [%s],[%s], \n",
                             g_global[type].auc_CfgVersion, g_global[type].pst_cmd[i].cmd_para);
            }
            i++;
            PS_PRINT_DBG("cmd_name:%s,cmd_para:%s", cmd_name, cmd_para);
        }
        flag = end + 1;
    }
    PS_PRINT_DBG("Read cmd OK\n");

    /* ������һ��������˳����������һ���˳����� */
    if (g_global[type].pst_cmd[i - 1].cmd_type != QUIT_TYPE_CMD) {
        g_global[type].pst_cmd[i].cmd_type = QUIT_TYPE_CMD;
        i++; /* ����ͳ��������� */
    }

    /* ����ʵ������������޸����յ�������� */
    g_global[type].l_count = i;
    PS_PRINT_DBG("type[%d], cmd count[%d]\n", type, g_global[type].l_count);

    return SUCC;
}

/*
 * Prototype    : patch_get_cfg
 * Description  : get patch config command
 */
int32 patch_get_cfg(uint8 *cfg, int32 type)
{
    uint8 *buf;
    int32 l_readlen;
    int32 l_ret;

    buf = OS_KMALLOC_GFP(READ_CFG_BUF_LEN);
    if (buf == NULL) {
        return -EFAIL;
    }

    /* �����ļ�����С��1024 */
    l_readlen = patch_read_cfg(cfg, buf);
    if (l_readlen < 0) {
        PS_PRINT_ERR("read cfg error\n");
        OS_MEM_KFREE(buf);
        buf = NULL;
        return -EFAIL;
    }

    l_ret = patch_parse_cfg(buf, l_readlen, type);
    if (l_ret < 0) {
        PS_PRINT_ERR("parse cfg error\n");
    }

    OS_MEM_KFREE(buf);
    buf = NULL;
    return l_ret;
}
EXPORT_SYMBOL(patch_get_cfg);
/*
 * Prototype    : patch_execute_cmd
 * Description  : execute cmd
 */
int32 patch_execute_cmd(int32 cmd_type, uint8 *cmd_name, uint8 *cmd_para, int32 type)
{
    int32 l_ret;

    /* ����ϴβ����������������ݣ���ȡ���ʱ�Գ���Ϊ�жϣ�buf�Ͳ�������� */
    g_global[type].l_Recvbuf1_len = 0;
    g_global[type].l_Recvbuf2_len = 0;

    switch (cmd_type) {
        case FILE_TYPE_CMD:
            PS_PRINT_DBG(" command type FILE_TYPE_CMD\n");
            l_ret = patch_file_type(cmd_name, cmd_para, type);
            break;
        case NUM_TYPE_CMD:
            PS_PRINT_DBG(" command type NUM_TYPE_CMD\n");
            l_ret = patch_number_type(cmd_name, cmd_para, type);

            break;
        case QUIT_TYPE_CMD:
            PS_PRINT_DBG(" command type QUIT_TYPE_CMD\n");
            l_ret = patch_quit_type(type);
            break;

        default:
            PS_PRINT_ERR("command type error[%d]\n", cmd_type);
            l_ret = -EFAIL;
            break;
    }

    return l_ret;
}

/*
 * Prototype    : patch_download_export
 * Description  : download export
 */
int32 patch_download_export(uint8 *keyword, int32 type)
{
    int32 i;
    int32 l_ret;

    for (i = 0; i < g_global[type].l_count; i++) {
        if (!OS_STR_CMP(g_global[type].pst_cmd[i].cmd_name,
                        keyword)) {
            break;
        }
    }
    if (i >= g_global[type].l_count) {
        return -EFAIL;
    }
    l_ret = patch_execute_cmd(g_global[type].pst_cmd[i].cmd_type,
                              g_global[type].pst_cmd[i].cmd_name,
                              g_global[type].pst_cmd[i].cmd_para,
                              type);
    return l_ret;
}
EXPORT_SYMBOL(patch_download_export);

int32 patch_check_version(int32 type)
{
    int32 l_ret;
    int32 i = 0;
    int8 version_buff[VERSION_LEN] = {0};
    l_ret = snprintf_s(version_buff, sizeof(version_buff), sizeof(version_buff) - 1, "%s%c",
                       VER_CMD_KEYWORD, COMPART_KEYWORD);
    if (l_ret < 0) {
        PS_PRINT_ERR("log str format err line[%d]\n", __LINE__);
        return l_ret;
    }

    PS_PRINT_DBG("check version cmd:%s", version_buff);
    for (i = 0; i < HOST_DEV_TIMEOUT; i++) {
        l_ret = send_msg_t(version_buff, VERSION_LEN, type);
        if (l_ret < 0) {
            continue; 
        }

        l_ret = patch_device_respond(type);
        if (l_ret == 0) {
            return SUCC;
        }
    }
    return l_ret;
}
/*
 * Prototype    : patch_download_patch
 * Description  : download patch
 */
int32 patch_download_patch(int32 type)
{
    int32 l_ret;
    int32 i;
    uint32 ul_alloc_len = READ_DATA_BUF_LEN;
 
    ps_print_info("type:%d\n", type);
    if (type == ENUM_INFO_UART) {
        g_stringbuf.pbufstart = kmalloc(ul_alloc_len, GFP_KERNEL);
        if (g_stringbuf.pbufstart == NULL) {
            ul_alloc_len = READ_DATA_REALLOC_BUF_LEN;
            g_stringbuf.pbufstart = kmalloc(ul_alloc_len, GFP_KERNEL);
            if (g_stringbuf.pbufstart == NULL) {
                g_usemalloc = 0;
                PS_PRINT_ERR("ringbuf KMALLOC SIZE(%d) failed.\n", ul_alloc_len);
                g_stringbuf.pbufstart = g_global[type].auc_Recvbuf1;
                g_stringbuf.pbufend = RECV_BUF_LEN + g_stringbuf.pbufstart;

                return -EFAIL;
            }

            g_powerpin_state &= 0xFF - BFG_PINDISABLE;
            ps_print_info("ringbuf kmalloc size(%d) suc.\n", ul_alloc_len);
        }

        g_stringbuf.pbufend = ul_alloc_len + g_stringbuf.pbufstart;
        g_usemalloc = 1;

        g_stringbuf.phead = g_stringbuf.pbufstart;
        g_stringbuf.ptail = g_stringbuf.pbufstart;
    }

    l_ret = patch_check_version(type);
    if (l_ret < 0) {
        PS_PRINT_ERR("checkout device version error");

        if ((type == ENUM_INFO_UART) && (g_usemalloc == 1)) { 
            kfree(g_stringbuf.pbufstart);
            g_stringbuf.pbufstart = NULL;
            g_stringbuf.pbufend = NULL;
            g_stringbuf.phead = NULL;
            g_stringbuf.ptail = NULL;
        }
        return -EFAIL;
    }

    /* ִ������:: ������û�ж��꣬����Ǵ�������  */
    for (i = 0; i < g_global[type].l_count; i++) {
        PS_PRINT_DBG("cmd[%d]type[%d], name[%s], para[%s]\n",
                     i,
                     g_global[type].pst_cmd[i].cmd_type,
                     g_global[type].pst_cmd[i].cmd_name,
                     g_global[type].pst_cmd[i].cmd_para);

        l_ret = patch_execute_cmd(g_global[type].pst_cmd[i].cmd_type,
                                  g_global[type].pst_cmd[i].cmd_name,
                                  g_global[type].pst_cmd[i].cmd_para,
                                  type);
        if (l_ret < 0) {
            if ((type == ENUM_INFO_UART) && (g_usemalloc == 1)) { 
                kfree(g_stringbuf.pbufstart);
                g_stringbuf.pbufstart = NULL;
                g_stringbuf.pbufend = NULL;
                g_stringbuf.phead = NULL;
                g_stringbuf.ptail = NULL;
                g_usemalloc = 0;
            }
            if (type == ENUM_INFO_SDIO) {
                g_powerpin_state &= 0xFF - WLAN_PINDISABLE;
            } else if (type == ENUM_INFO_UART) {
                g_powerpin_state &= 0xFF - BFG_PINDISABLE;
            }
            return l_ret;
        }
    }
    if (type == ENUM_INFO_SDIO) {
        g_powerpin_state &= 0xFF - WLAN_PINDISABLE;
    } else if (type == ENUM_INFO_UART) {
        g_powerpin_state &= 0xFF - BFG_PINDISABLE;
    }
    PS_PRINT_DBG("DOWN PATCH SUCC\n");

    if ((type == ENUM_INFO_UART) && (g_usemalloc == 1)) { 
        kfree(g_stringbuf.pbufstart);
        g_usemalloc = 0;
    }
    g_stringbuf.pbufstart = NULL;
    g_stringbuf.pbufend = NULL;
    g_stringbuf.phead = NULL;
    g_stringbuf.ptail = NULL;
    return SUCC;
}
EXPORT_SYMBOL(patch_download_patch);
/*
 * Prototype    : create_recover_flag
 * Description  : create recover flag file at /data/hwlogdir
 * Return Value : int32. if file create succ or is exist return 0,
 *              : else return 1;
 */
int32 create_recover_flag(void)
{
    struct file *fp = NULL;

    if (is_file_exist(SDIO_RECOVER_FLAG_FILE)) {
        PS_PRINT_DBG("file %s is already exist\n", SDIO_RECOVER_FLAG_FILE);
        return 0;
    } else {
        fp = filp_open(SDIO_RECOVER_FLAG_FILE, O_RDWR | O_CREAT, 0664);
        if (IS_ERR(fp)) {
            return 1;
        }
        filp_close(fp, NULL);
    }
    PS_PRINT_DBG("create/open file %s succ\n", SDIO_RECOVER_FLAG_FILE);
    return 0;
}
EXPORT_SYMBOL(create_recover_flag);

/*
 * Prototype    : is_file_exist
 * Description  : judge if file is exist in filesystem.
 * Return Value : int32. if file is exist return 1, else return 0;
 */
int32 is_file_exist(char *filename)
{
    int ret;
    mm_segment_t old_fs = get_fs();
    struct kstat kst;

    set_fs(KERNEL_DS);
    if (vfs_stat(filename, &kst)) {
        PS_PRINT_WARNING("file %s not exist\n", filename);
        ret = 0;
    } else {
        PS_PRINT_DBG("file %s is exist\n", filename);
        ret = 1;
    }
    set_fs(old_fs);
    return ret;
}

/*
 * Prototype    : patch_init
 * Description  : patch module initialization
 */
int32 patch_init(int32 type)
{
    int32 l_ret;

    ps_print_info("entry\n");

    if (type == ENUM_INFO_SDIO) {
        if (is_file_exist(SDIO_RECOVER_FLAG_FILE) && is_file_exist(SDIO_RECOVER_CFG_FILE)) {
            PS_PRINT_WARNING("use recover_cfg file\n");
            memcpy_s(g_global[ENUM_INFO_SDIO].auc_Cfgpath, sizeof(g_global[ENUM_INFO_SDIO].auc_Cfgpath),
                     SDIO_RECOVER_CFG_FILE, OS_STR_LEN(SDIO_RECOVER_CFG_FILE));
        } else {
            PS_PRINT_WARNING("use normal cfg_file\n");
            memcpy_s(g_global[ENUM_INFO_SDIO].auc_Cfgpath, sizeof(g_global[ENUM_INFO_SDIO].auc_Cfgpath),
                     SDIO_CFG_FILE, OS_STR_LEN(SDIO_CFG_FILE));
        }
    } else {
        memcpy_s(g_global[ENUM_INFO_UART].auc_Cfgpath, sizeof(g_global[ENUM_INFO_UART].auc_Cfgpath),
                 UART_CFG_FILE, OS_STR_LEN(UART_CFG_FILE));
    }

    g_global[type].pst_wait = OS_KMALLOC_GFP(sizeof(OS_WAIT_QUEUE_HEAD_T_STRU));
    if (g_global[type].pst_wait == NULL) {
        return -EFAIL;
    }
    OS_INIT_WAITQUEUE_HEAD(g_global[type].pst_wait);

    l_ret = patch_get_cfg(g_global[type].auc_Cfgpath, type);
    if (l_ret < 0) {
        PS_PRINT_ERR("get [%s] command is fail\n", g_global[type].auc_Cfgpath);
        return -EFAIL;
    }

    if (g_pucDataBuf_t == NULL) {
        g_pucDataBuf_t = OS_KMALLOC_GFP(READ_PATCH_BUF_LEN);
        if (g_pucDataBuf_t == NULL) {
            PS_PRINT_ERR("g_pucDataBuf_etc KMALLOC failed");
            g_pucDataBuf_t = NULL;
            return -EFAIL;
        } else {
            PS_PRINT_DBG("g_pucDataBuf_etc KMALLOC succ");
        }
    }
    return SUCC;
}

EXPORT_SYMBOL(patch_init);

/*
 * Prototype    : patch_exit
 * Description  : patch module exit
 */
int32 patch_exit(void)
{
    int32 i;
    for (i = 0; i < ENUM_INFO_TOTAL; i++) {
        g_global[i].l_count = 0;
        if (g_global[i].pst_cmd != NULL) {
            OS_MEM_KFREE(g_global[i].pst_cmd);
            g_global[i].pst_cmd = NULL;
        }

        if (g_global[i].pst_wait != NULL) {
            OS_MEM_KFREE(g_global[i].pst_wait);
            g_global[i].pst_wait = NULL;
        }
    }

    if (g_pucDataBuf_t != NULL) {
        OS_MEM_KFREE(g_pucDataBuf_t);
        g_pucDataBuf_t = NULL;
    }

    return SUCC;
}
EXPORT_SYMBOL(patch_exit);
