

/* ͷ�ļ����� */
#include "wireless_patch.h"

#include "plat_type.h"
#include "plat_uart.h"
#include "bfgx_dev.h"
#include "oal_kernel_file.h"
#include "oal_util.h"
#include "plat_pm.h"
#include "bfgx_data_parse.h"
#include "plat_cali.h"
#include "securec.h"

/* ȫ�ֱ������� */
PATCH_GLOBALS_STUR g_patch_ctrl;
RINGBUF_STRU g_stringbuf;
uint8 *g_data_buf;

/* xmodem ���� */
uint8 g_xmodem_index = 1;

unsigned short g_crc_table[CRC_TABLE_SIZE] = {
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

/*
 * Prototype    : ps_patch_to_nomal
 * Description  : from download patch state to normal state
 */
static void ps_patch_to_nomal(void)
{
    struct ps_core_s *ps_core_d = NULL;
    ps_print_info("%s", __func__);
    ps_core_d = NULL;
    ps_get_core_reference(&ps_core_d, BUART);
    if (ps_core_d == NULL) {
        ps_print_err("ps_core_d is NULL");
        return;
    }

    /* change function pointer, pointer to ps_core_recv */
    st_tty_recv = ps_core_recv;
    /* init variable for rx data */
    ps_core_d->rx_pkt_total_len = 0;
    ps_core_d->rx_have_recv_pkt_len = 0;
    ps_core_d->rx_have_del_public_len = 0;
    ps_core_d->rx_decode_public_buf_ptr = ps_core_d->rx_public_buf_org_ptr;
    return;
}

/*
 * Prototype    : ps_patch_write
 * Description  : functions called from pm drivers,used download patch data
 */
int32 ps_patch_write(uint8 *data, int32 count)
{
    struct ps_core_s *ps_core_d = NULL;
    int32 len;

    PS_PRINT_FUNCTION_NAME;

    ps_core_d = NULL;
    ps_get_core_reference(&ps_core_d, BUART);
    if (unlikely((ps_core_d == NULL) || (ps_core_d->tty == NULL))) {
        ps_print_err(" tty unavailable to perform write");
        return -EINVAL;
    }
    /* write to uart */
    len = ps_write_tty(ps_core_d, data, count);
    return len;
}

/*
 * Prototype    : ps_recv_patch
 * Description  : PS's pm receive function.this function is called when download patch.
 * input        : data -> recive data ptr   from UART TTY
 *                count -> recive data count from UART TTY
 */
int32 ps_recv_patch(void *disc_data, const uint8 *data, int32 count)
{
    struct ps_core_s *ps_core_d = NULL;

    PS_PRINT_FUNCTION_NAME;

    ps_core_d = (struct ps_core_s *)disc_data;
    if (unlikely((data == NULL) || (disc_data == NULL) || (ps_core_d->pm_data == NULL) ||
                 (ps_core_d->pm_data->recv_patch == NULL))) {
        ps_print_err(" received null from TTY ");
        return -EINVAL;
    }

    /* tx data to hw-pm */
    if (ps_core_d->pm_data->recv_patch(data, count) < 0) {
        ps_print_err(" %s-err", __func__);
        return -EPERM;
    }
    return 0;
}

/*
 * Prototype    : pm_uart_set_baudrate
 * Description  : set baudrate of uart0 when download patch
 * Input        : int64 baudrate: the baudrate want to set
 * Return       : 0 means succeed,-1 means failed.
 */
int32 pm_uart_set_baudrate(int64 baudrate)
{
    struct ps_core_s *ps_core_d = NULL;

    ps_get_core_reference(&ps_core_d, BUART);
    if (unlikely(ps_core_d == NULL || ps_core_d->pm_data == NULL)) {
        ps_print_err("ps_core_d is err\n");
        return -1;
    }

    ps_print_info("set uart baudrate to %ld\n", baudrate);

    /* call interface supplied by 3 in 1 */
    if (ps_core_d->pm_data->change_baud_rate != NULL) {
        /* set uart baudrate */
        return ps_core_d->pm_data->change_baud_rate(ps_core_d, baudrate, FLOW_CTRL_ENABLE);
    }

    ps_print_err("change_baud_rate is NULL!\n");

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
        dataTmp = (uint8)(crc >> 8); /* crc����8λ */
        crc = crc << 8; /* crc����8λ */
        crc = crc ^ g_crc_table[dataTmp ^ (*data)];
        length--;
        data++;
    }
    return crc;
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
    uint32 ulheadtoendlen;
    uint8 *ptail = NULL;
    if (unlikely((data == NULL))) {
        ps_print_err("data is NULL\n ");
        return -EFAIL;
    }
    if ((g_stringbuf.pbufstart == NULL) || (g_stringbuf.pbufend < g_stringbuf.pbufstart)) {
        ps_print_err("buf is NULL,write failed\n ");
        ps_print_err("pbufstart=%p,pbufend=%p\n", g_stringbuf.pbufstart, g_stringbuf.pbufend);
        return -EFAIL;
    }
    ptail = g_stringbuf.ptail;
    if ((g_stringbuf.phead < g_stringbuf.pbufstart) ||
        (g_stringbuf.phead > g_stringbuf.pbufend) ||
        (ptail < g_stringbuf.pbufstart) ||
        (ptail > g_stringbuf.pbufend)) {
        ps_print_err("phead or ptail is out of range,write failed\n");
        ps_print_err("pbufstart=%p,pbufend=%p\n", g_stringbuf.pbufstart, g_stringbuf.pbufend);
        ps_print_err("phead=%p,ptail=%p\n", g_stringbuf.phead, g_stringbuf.ptail);
        return -EFAIL;
    }
    ulbuflen = g_stringbuf.pbufend - g_stringbuf.pbufstart;
    ps_print_dbg("len [%d],\n ", len);

    /* phead and ptail are in the same cycle */
    if (g_stringbuf.phead >= ptail) {
        /* still in the same cycle */
        if ((g_stringbuf.pbufend - g_stringbuf.phead) >= len) {
            memcpy_s(g_stringbuf.phead, g_stringbuf.pbufend - g_stringbuf.phead, data, len);
            g_stringbuf.phead += len;
            if (os_waitqueue_active(g_patch_ctrl.pst_wait)) {
                os_wake_up_interruptible(g_patch_ctrl.pst_wait);
                ps_print_dbg("wake up ok");
            }
        } else if ((ulbuflen - (g_stringbuf.phead - ptail)) > len) {
            ulheadtoendlen = g_stringbuf.pbufend - g_stringbuf.phead;
            memcpy_s(g_stringbuf.phead, ulheadtoendlen, data, ulheadtoendlen);
            memcpy_s(g_stringbuf.pbufstart, g_stringbuf.ptail - g_stringbuf.pbufstart,
                     data + ulheadtoendlen, len - ulheadtoendlen);
            g_stringbuf.phead = g_stringbuf.pbufstart + (len - ulheadtoendlen);
            ps_print_info("phead back\n");
            if (os_waitqueue_active(g_patch_ctrl.pst_wait)) {
                os_wake_up_interruptible(g_patch_ctrl.pst_wait);
                ps_print_dbg("wake up ok");
            }
        } else {
            ps_print_err("Not enough mem,len=%d.\n ", len);
        }
    } else if((ptail - g_stringbuf.phead - 1) > len) {
        /* phead is in the next cycle */
        /* "ptail - phead = 1" means the buffer is full */
        memcpy_s(g_stringbuf.phead, ptail - g_stringbuf.phead - 1, data, len);
        g_stringbuf.phead += len;
        if (os_waitqueue_active(g_patch_ctrl.pst_wait)) {
            os_wake_up_interruptible(g_patch_ctrl.pst_wait);
            ps_print_dbg("wake up ok");
        }
    } else {
        ps_print_err("Not enough mem,len=%d.\n ", len);
    }

    if (g_stringbuf.phead >= g_stringbuf.pbufend) {
        g_stringbuf.phead = g_stringbuf.pbufstart;
        ps_print_info("phead back\n");
    }
    return SUCC;
}

/*
 * Prototype    : bfg_patch_recv
 * Description  : function for bfg patch receive
 * Input        : uint8 *data: address of data
 *                int32 count: length of data
 */
int32 bfg_patch_recv(const uint8 *data, int32 count)
{
    int32 ret;

    /* this function should be called after patch_init(uart), otherwise maybe null pointer */
    ret = uart_recv_data(data, count);

    return ret;
}

int32 read_msg_t(uint8 *data, int32 len)
{
    int32 l_len;
    uint32 ultailtoendlen;
    uint8 *phead = NULL;

    if (unlikely((data == NULL))) {
        ps_print_err("data is NULL\n ");
        return -EFAIL;
    }

    if ((g_stringbuf.pbufstart == NULL) || (g_stringbuf.pbufstart > g_stringbuf.pbufend)) {
        ps_print_err("buf is NULL,read failed\n ");
        ps_print_err("pbufstart=%p,pbufend=%p\n", g_stringbuf.pbufstart, g_stringbuf.pbufend);
        return -EFAIL;
    }

    if ((g_stringbuf.phead < g_stringbuf.pbufstart) ||
        (g_stringbuf.phead > g_stringbuf.pbufend) ||
        (g_stringbuf.ptail < g_stringbuf.pbufstart) ||
        (g_stringbuf.ptail > g_stringbuf.pbufend)) {
        ps_print_err("phead or ptail is out of range, read failed\n");
        ps_print_err("pbufstart=%p,pbufend=%p\n", g_stringbuf.pbufstart, g_stringbuf.pbufend);
        ps_print_err("phead=%p,ptail=%p\n", g_stringbuf.phead, g_stringbuf.ptail);
        return -EFAIL;
    }

    os_wait_event_interruptible_timeout(*g_patch_ctrl.pst_wait,
                                        (g_stringbuf.phead != g_stringbuf.ptail),
                                        PATCH_INTEROP_TIMEOUT);
    phead = g_stringbuf.phead;
    /* phead and ptail are in the same cycle */
    if (phead > g_stringbuf.ptail) {
        if ((phead - g_stringbuf.ptail) > len) {
            memcpy_s(data, len, g_stringbuf.ptail, len);
            l_len = len;
            g_stringbuf.ptail += len;
        } else {
            /* not enough data */
            l_len = phead - g_stringbuf.ptail;
            memcpy_s(data, len, g_stringbuf.ptail, l_len);
            g_stringbuf.ptail += l_len;
        }
    } else if (phead < g_stringbuf.ptail) {
        /* phead is in the next cycle */
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
        ps_print_warning("No data.\n");
    }
    if (g_stringbuf.ptail >= g_stringbuf.pbufend) {
        g_stringbuf.ptail = g_stringbuf.pbufstart;
    }

    return l_len;
}

/*
 * Prototype    : pm_uart_send
 * Description  : uart patch transmit function
 * Input        : uint8 *date: address of data
 *                int32 len: length of data
 * Return       : length which has been sent
 */
int32 pm_uart_send(uint8 *data, int32 len)
{
    uint16 count = 0;

    struct pm_drv_data *pm_data = pm_get_drvdata(BUART);
    if (pm_data == NULL) {
        ps_print_err("pm_data is NULL!\n");
        return -1;
    }

    if (pm_data->write_patch == NULL) {
        ps_print_err("bfg_write_patch is NULL!\n");
        return -1;
    }

    while (1) {
        /* this function return the length has been sent */
        count = pm_data->write_patch(data, len);
        /* data has been sent over and return */
        if (count == len) {
            return len;
        }

        /* data has not been sent over, we will send again */
        data = data + count;
        len = len - count;
        msleep(1);
    }
}

/*
 * Prototype    : send_msg
 * Description  : send message to device,by sdio or uart
 */
int32 send_msg_t(uint8 *data, int32 len)
{
    return pm_uart_send(data, len);
}

/*
 * Prototype    : recv_expect_result
 * Description  : receive result form device
 */
int32 recv_expect_result_t(uint8 expect)
{
    uint8 auc_buf[RECV_BUF_LEN];
    int32 l_len;
    int32 i;

    ps_print_dbg(" entry\n");

    memset_s(auc_buf, RECV_BUF_LEN, 0, RECV_BUF_LEN);
    for (i = 0; i < HOST_DEV_TIMEOUT; i++) {
        l_len = read_msg_t(auc_buf, 1);
        if (l_len < 0) {
            ps_print_err("recv result fail\n");
            return -EFAIL;
        }
        if (auc_buf[0] == expect) {
            ps_print_dbg(" send SUCC [%x]\n", expect);
            return SUCC;
        } else if ((auc_buf[0] == MSG_FORM_DRV_N) || (auc_buf[0] == NAK) || (auc_buf[0] == MSG_FORM_DRV_C)) {
            /*
             * NAK: �ļ�����ʱ�ط���ʶ
             * MSG_FORM_DRV_N:�����ط���ʶ
             */
            ps_print_err(" send again [0x%x]\n", auc_buf[0]);
            return -EFAIL;
        } else {
            /* ���ڴ���Ľ������ʮ�εĻ��ᣬ */
            PATCH_SEND_N_UART;
            ps_print_warning(" error result[0x%x], expect [0x%x], read result again\n", auc_buf[0], expect);
        }
    }
    return -EFAIL;
}

/*
 * Prototype    : patch_device_respond
 * Description  : wait respond form device
 */
int32 patch_device_respond(void)
{
    int32 l_ret;
    int32 i;

    for (i = 0; i < HOST_DEV_TIMEOUT; i++) {
        memset_s(g_patch_ctrl.auc_DevVersion, VERSION_LEN, 0, VERSION_LEN);
        msleep(1);
        l_ret = read_msg_t(g_patch_ctrl.auc_DevVersion, VERSION_LEN);
        if (l_ret < 0) {
            ps_print_err("read fail![%d]\n", i);
            continue;
        } else if (!os_mem_cmp((int8 *)g_patch_ctrl.auc_DevVersion,
                               (int8 *)g_patch_ctrl.auc_CfgVersion,
                               os_str_len(g_patch_ctrl.auc_CfgVersion))) {
            ps_print_info("Device Version = [%s], CfgVersion = [%s].\n",
                          g_patch_ctrl.auc_DevVersion, g_patch_ctrl.auc_CfgVersion);
            return SUCC;
        } else {
            ps_print_info("Device Version = [%s], CfgVersion = [%s].\n",
                          g_patch_ctrl.auc_DevVersion, g_patch_ctrl.auc_CfgVersion);
        }
    }

    ps_print_err("read device version fail![%d]\n", i);

    return -EFAIL;
}

/*
 * Prototype    : patch_send
 * Description  : send message to device,by sdio or uart
 */
int32 patch_send(uint8 *data, int32 len, uint8 expect)
{
    int32 i;
    int32 l_ret;

    for (i = 0; i < HOST_DEV_TIMEOUT; i++) {
        l_ret = send_msg_t(data, len);
        if (l_ret < 0) {
            continue;
        }

        l_ret = recv_expect_result_t(expect);
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

    if (data == NULL) {
        return -EFAIL;
    }

    l_datalen = len;
    flag = data;

    while (l_datalen > 0) {
        l_sendlen = oal_min(XMODE_DATA_LEN, l_datalen);
        l_datalen = l_datalen - l_sendlen;
        st_patch_pkt.Head = SOH;
        st_patch_pkt.PacketNum = g_xmodem_index;

        /* ���ݳ��Ȳ���128�� */
        if (l_sendlen < XMODE_DATA_LEN) {
            ps_print_dbg("data_len  %d\n", l_sendlen);
            memset_s(&flag[l_sendlen], (XMODE_DATA_LEN - l_sendlen), 0x00, (XMODE_DATA_LEN - l_sendlen));
        }
        us_CRCValue = do_crc_table_1(flag, XMODE_DATA_LEN);
        CRCValue_H = (us_CRCValue & 0xFF00) >> 8; /* us_CRCValue����0xFF00������8λ */
        CRCValue_L = us_CRCValue & 0xFF;

        st_patch_pkt.PacketAnt = ~(st_patch_pkt.PacketNum);

        for (i = 0; i < HOST_DEV_TIMEOUT; i++) {
            l_ret = pm_uart_send((uint8 *)&st_patch_pkt, sizeof(st_patch_pkt));
            l_ret = pm_uart_send(flag, XMODE_DATA_LEN);
            l_ret = pm_uart_send(&CRCValue_H, 1);
            l_ret = pm_uart_send(&CRCValue_L, 1);

            l_ret = recv_expect_result_t(expect);
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

        g_xmodem_index++;
    }

    return SUCC;
}

int32 patch_string_to_num(uint8 *string)
{
    int32 i;
    int32 l_num;

    if (string == NULL) {
        return -EFAIL;
    }

    l_num = 0;
    for (i = 0; (string[i] >= '0') && (string[i] <= '9'); i++) {
        l_num = (l_num * 10) + (string[i] - '0'); /* �ַ���ת���ֵ��߼���Ҫ��l_num*10 */
    }

    return l_num;
}

/*
 * Prototype    : patch_wait_g_form_dev
 * Description  : wait go'command form device
 */
int32 patch_wait_g_form_dev(void)
{
    int32 l_ret;
    int32 i;

    for (i = 0; i < HOST_DEV_TIMEOUT; i++) {
        l_ret = recv_expect_result_t(MSG_FORM_DRV_G);
        if (l_ret == 0) {
            ps_print_dbg(" device finish G\n");
            return SUCC;
        }
    }

    ps_print_err("recv g FAIL\n");
    return -EFAIL;
}

int32 patch_wait_g_retry_form_dev(void)
{
    int32 l_ret;
    int32 i;

    for (i = 0; i < HOST_DEV_TIMEOUT; i++) {
        l_ret = recv_expect_result_t(MSG_FORM_DRV_G);
        if (l_ret == 0) {
            ps_print_dbg(" device finish G\n");
            return SUCC;
        } else {
            PATCH_SEND_N_UART;
            ps_print_warning("receive G failed\n");
        }
    }
    return -EFAIL;
}

/*
 * Prototype    : patch_send_char
 * Description  : send char to device
 */
int32 patch_send_char(int8 num, int32 wait)
{
    int32 l_ret;
    const uint32 ul_buf_size = 8;
    uint8 auc_buf[ul_buf_size];
    int32 i;

    memset_s(auc_buf, sizeof(auc_buf), num, sizeof(auc_buf));
    ps_print_dbg("send [0x%x], wait[%d]\n", num, wait);
    for (i = 0; i < HOST_DEV_TIMEOUT; i++) {
        /*
         * sdio �ӿڷ���ʱ�������ֽڶ��룬�����ĸ�
         * uart �ӿڷ���ʱ��ֻ����һ��
         */
        l_ret = send_msg_t(auc_buf, 1);
        if (l_ret < 0) {
            ps_print_err("Send fail\n");
            return l_ret;
        }

        if (wait == WAIT_RESPONSE) {
            l_ret = recv_expect_result_t(ACK);
            if (l_ret < 0) {
                continue;
            }
        }

        return l_ret;
    }

    return -EFAIL;
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
        ps_print_err("buf/fp is NULL\n");
        return -EFAIL;
    }

    rdlen = oal_file_read_ext(fp, fp->f_pos, buf, len);
    if (rdlen > 0) {
        fp->f_pos += rdlen;
    }

    return rdlen;
}

/*
 * Prototype    : patch_down_file
 * Description  : begin download patch file
 */
int32 patch_down_file(const uint8 *puc_file)
{
    OS_KERNEL_FILE_STRU *fp = NULL;
    uint8 *auc_buf;
    int32 l_len;
    int32 l_ret;
    int32 l_count;
    mm_segment_t fs;

    if (puc_file == NULL) {
        return -EFAIL;
    }

    fs = get_fs();
    set_fs(KERNEL_DS);
    fp = filp_open(puc_file, O_RDONLY, 0);
    if (IS_ERR(fp)) {
        set_fs(fs);
        fp = NULL;
        ps_print_err("filp_open %s fail!!\n", puc_file);
        return -EFAIL;
    }

    if (g_data_buf == NULL) {
        filp_close(fp, NULL);
        set_fs(fs);
        fp = NULL;
        return -EFAIL;
    }

    l_count = 1;
    g_xmodem_index = 1;
    while (1) {
        l_len = patch_read_patch(g_data_buf, READ_PATCH_BUF_LEN, fp);
        ps_print_dbg("kernel_read len[%d] [%d]\n", l_len, l_count);
        /* ������ȡ�ļ� */
        if ((l_len > 0) && (l_len <= READ_PATCH_BUF_LEN)) {
            l_ret = patch_xmodem_send(g_data_buf, l_len, ACK);
            if (l_ret == -EFAIL) {
                ps_print_err(" uart send data[%d] fail\n", l_count);
                break;
            }
        } else if (l_len == 0) {
            /* �ļ��Ѿ���ȡ��� */        
            PATCH_SEND_EOT_UART;
            g_xmodem_index = 1;
            l_ret = SUCC;

            ps_print_dbg("read file[%d] [%d] send EOT\n", l_count, l_len);
            break;
        } else {
            /* ��ȡ�ļ����� */
            PATCH_SEND_CAN_UART;
            g_xmodem_index = 1;
            l_ret = -EFAIL;
            ps_print_err("read file[%d] [%d]\n", l_count, l_len);
            break;
        }
        l_count++;
    }

    auc_buf = NULL;

    filp_close(fp, NULL);
    set_fs(fs);
    fp = NULL;

    return l_ret;
}

/*
 * Prototype    : patch_readm_fileopen
 * Description  : creat and open file to save mem
 */
OS_KERNEL_FILE_STRU *patch_readm_fileopen(void)
{
    OS_KERNEL_FILE_STRU *fp = NULL;
    mm_segment_t fs;
    struct timeval tv;
    struct rtc_time tm;
    const uint32 ul_filename_len = 50;
    char filename[ul_filename_len];
    int ret;

    do_gettimeofday(&tv);
    rtc_time_to_tm(tv.tv_sec, &tm);

    ps_print_info("%4d-%02d-%02d  %02d:%02d:%02d\n",
                  tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec); /* ת���ɵ�ǰʱ�� */
    ret = snprintf_s(filename, sizeof(filename), sizeof(filename) - 1,
                     "/data/log/hi110x/readm_bfg%04d%02d%02d%02d%02d%02d.bin",
                     tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
                     tm.tm_hour, tm.tm_min, tm.tm_sec); /* ת���ɵ�ǰʱ�� */
    if (ret < 0) {
        ps_print_err("log str format err line[%d]\n", __LINE__);
        return NULL;
    }

    ps_print_info("filename = %s", filename);
    fs = get_fs();
    set_fs(KERNEL_DS);
    fp = filp_open(filename, O_RDWR | O_CREAT, 0644);
    set_fs(fs);

    return fp;
}

/*
 * Prototype    : patch_recv_mem
 * Description  : receive memory information form device
 */
int32 patch_recv_mem(OS_KERNEL_FILE_STRU *fp, int32 len)
{
    uint8 *pdatabuf = NULL;
    int32 l_ret;
    mm_segment_t fs;
    int32 lenbuf = 0;

    if (oal_is_err_or_null(fp)) {
        ps_print_err("fp is error,fp = 0x%p\n", fp);
        return -EFAIL;
    }

    if (len == 0) { /* 0 is invalid */
        ps_print_warning("len=%d is invalid, force to 4 bytes\n", len);
        len = 4; /* 4 bytes at least */
    }

    pdatabuf = os_kmalloc_gfp(len);
    if (pdatabuf == NULL) {
        return -EFAIL;
    }

    memset_s(pdatabuf, len, 0, len);

    fs = get_fs();
    set_fs(KERNEL_DS);

    while (len > lenbuf) {
        l_ret = read_msg_t(pdatabuf + lenbuf, len - lenbuf);
        if (l_ret > 0) {
            lenbuf += l_ret;
        } else {
            ps_print_err("time out\n");
            break;
        }
    }

    if (len <= lenbuf) {
        oal_kernel_file_write(fp, pdatabuf, len);
    }
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 35))
    vfs_fsync(fp, 0);
#else
    vfs_fsync(fp, fp->f_path.dentry, 0);
#endif

    set_fs(fs);
    ringbuf_flush();
    os_mem_kfree(pdatabuf);

    return l_ret;
}

/*
 * Prototype    : patch_int_para_send
 * Description  : down integer to device
 */
int32 patch_int_para_send(uint8 *name, uint8 *Value)
{
    int32 l_ret;
    int32 data_len;
    int32 Value_len;
    int32 i;
    int32 n;
    uint8 auc_num[INT32_STR_LEN];
    uint8 data[DATA_BUF_LEN];

    Value_len = os_str_len((int8 *)Value);

    memset_s(auc_num, INT32_STR_LEN, 0, INT32_STR_LEN);
    memset_s(data, DATA_BUF_LEN, 0, DATA_BUF_LEN);

    data_len = 0;
    data_len = os_str_len(name);
    l_ret = memcpy_s(data, sizeof(data), name, data_len);
    if (l_ret != EOK) {
        ps_print_err("data buff not enough\n");
        return -EFAIL;
    }

    data[data_len] = COMPART_KEYWORD;
    data_len = data_len + 1;

    for (i = 0, n = 0; (i <= Value_len) && (n < INT32_STR_LEN); i++) {
        if ((Value[i] == ',') || (Value_len == i)) {
            ps_print_dbg("auc_num = %s, i = %d, n = %d\n", auc_num, i, n);
            if (n == 0) {
                continue;
            }
            l_ret = memcpy_s((uint8 *)&data[data_len], sizeof(data) - data_len, auc_num, n);
            if (l_ret != EOK) {
                ps_print_err("data buff not enough\n");
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

    ps_print_dbg("data_len = %d, \n", data_len);
    ps_print_dbg("data = %s, \n", data);

    ringbuf_flush();

    l_ret = send_msg_t(data, data_len);

    return l_ret;
}

int32 patch_number_version_cmd(uint8 *key, uint8 *value)
{
    int32 l_ret;
    int32 i;
    int8 version_buff[VERSION_LEN] = {0};

    l_ret = snprintf_s(version_buff, sizeof(version_buff), sizeof(version_buff) - 1, "%s%c",
                       VER_CMD_KEYWORD, COMPART_KEYWORD);
    if (l_ret < 0) {
        ps_print_err("log str format err line[%d]\n", __LINE__);
        return l_ret;
    }

    for (i = 0; i < HOST_DEV_TIMEOUT; i++) {
        l_ret = send_msg_t(version_buff, VERSION_LEN);
        if (l_ret < 0) {
            continue;
        }

        l_ret = patch_device_respond();
        if (l_ret == 0) {
            return SUCC;
        }
    }

    return l_ret;
}

STATIC int32 patch_number_wmem_cmd(uint8 *key, uint8 *value)
{
    int32 ret;

    ret = patch_int_para_send(key, value);
    if (ret < 0) {
        ps_print_err("send %s,%s fail \n", key, value);
        return ret;
    }

    /* G ��device ��Ӧ������ɱ�־ */
    ret = patch_wait_g_form_dev();
    if (ret >= 0) {
        ps_print_info("patch write mem %s success!\n", value);
    } else {
        ps_print_err("patch write mem %s fail! ret=%d\n", value, ret);
    }

    return ret;
}

STATIC int32 patch_number_jump_cmd(uint8 *key, uint8 *value)
{
    int32 ret;

    ret = patch_int_para_send(key, value);
    if (ret < 0) {
        ps_print_err("send %s,%s fail \n", key, value);
        return ret;
    }

    /* G ��device ��Ӧ������ɱ�־ */
    ret = patch_wait_g_form_dev();
    if (ret >= 0) {
        ps_print_info("JUMP %s success!\n", value);
    } else {
        ps_print_err("JUMP %s timeout! ret=%d\n", value, ret);
    }

    return ret;
}

STATIC int32 patch_number_rmem_cmd(uint8 *key, uint8 *value)
{
    int32 ret;
    int32 num;
    uint8 *flag = NULL;
    OS_KERNEL_FILE_STRU *fp = NULL;

    fp = patch_readm_fileopen();
    if (IS_ERR(fp)) {
        return SUCC;
    }

    ret = patch_int_para_send(key, value);
    if (ret < 0) {
        ps_print_err("send %s,%s fail \n", key, value);
        oal_file_close(fp);
        return ret;
    }

    flag = os_str_chr(value, ',');
    if (flag == NULL) {
        ps_print_err("RECV LEN ERROR..\n");
        oal_file_close(fp);
        return -EFAIL;
    }
    flag++;
    while (*flag == COMPART_KEYWORD) {
        flag++;
    }
    num = patch_string_to_num(flag);

    ps_print_info("recv len [%d]\n", num);

    ret = patch_recv_mem(fp, num);
    oal_file_close(fp);

    return ret;
}

STATIC int32 patch_number_baud_cmd(uint8 *key, uint8 *value)
{
    int32 ret;
    int32 num;

    /* �޸Ĳ����� */
    ret = patch_int_para_send(key, value);
    if (ret < 0) {
        ps_print_err("send %s,%s fail \n", key, value);
        return ret;
    }

    ret = recv_expect_result_t(MSG_FORM_DRV_A);
    if (ret < 0) {
        ps_print_err("send change baudrate fail\n");
        return ret;
    }

    num = patch_string_to_num(value);
    ps_print_info("change baudrate to:%d\n", num);

    /* ���ӵ����޸Ĳ����ʺ��� */
    ret = pm_uart_set_baudrate(num);
    if (ret < 0) {
        ps_print_err("modify baudrate fail!!\n");
        return -EFAIL;
    }

    ringbuf_flush();
    oal_usleep_range(10000, 11000); /* ˯��ʱ����10000��11000us֮�� */

    /* G ��device ��Ӧ������ɱ�־ */
    ret = patch_wait_g_retry_form_dev();

    return ret;
}

STATIC int32 patch_down_cali_data(void)
{
    int32 l_len;
    int32 l_ret;

    if (g_data_buf == NULL) {
        return -EFAIL;
    }

    /* ��ȡbfgxУ׼���� */
    l_ret = get_bfgx_cali_data(g_data_buf, &l_len, READ_PATCH_BUF_LEN);
    if (l_ret < 0) {
        ps_print_err("get bfgx cali data failed, len=%d\n", l_len);
        return -EFAIL;
    }

    /* Wait at least 5 ms */
    oal_usleep_range(FILE_CMD_WAIT_TIME_MIN, FILE_CMD_WAIT_TIME_MAX);

    l_ret = patch_xmodem_send(g_data_buf, l_len, ACK);
    if (l_ret == -EFAIL) {
        ps_print_err("uart send data fail\n");
        return l_ret;
    }

    /* �ļ��Ѿ���ȡ��� */        
    PATCH_SEND_EOT_UART;

    /* G �� DEVICE �����Ӧ������־ */
    l_ret = patch_wait_g_form_dev();

    return l_ret;
}

STATIC int32 bfgx_cali_data_download(uint8 *key, uint8 *value)
{
    int32 l_ret;
    uint32 len;
    uint32 Value_len;
    uint8 *addr = NULL;
    uint8 buff_tx[SEND_BUF_LEN];

    /* �������Value�ַ��������Valueֻ��һ����ַ����ʽΪ"0xXXXXX" */
    /* ����Ժ����ʽΪ"FILES �ļ����� Ҫд�ĵ�ַ"---"FILES 1 0xXXXX " */
    memset_s(buff_tx, SEND_BUF_LEN, 0, SEND_BUF_LEN);

    /* buff_tx="" */
    len = os_str_len(key);
    l_ret = memcpy_s(buff_tx, sizeof(buff_tx), key, len);
    if (l_ret != EOK) {
        ps_print_err("buff_tx not enough\n");
        return -EFAIL;
    }
    buff_tx[len] = COMPART_KEYWORD;
    len++;

    /* buff_tx="FILES " */
    Value_len = os_str_len(value);
    addr = delete_space(value, &Value_len);
    if (addr == NULL) {
        ps_print_err("addr is NULL, Value[%s] Value_len[%d]", value, Value_len);
        return -EFAIL;
    }
    l_ret = memcpy_s(&buff_tx[len], sizeof(buff_tx) - len, addr, Value_len);
    if (l_ret != EOK) {
        ps_print_err("buff_tx not enough\n");
        return -EFAIL;
    }
    len += Value_len;
    buff_tx[len] = COMPART_KEYWORD;
    len++;

    ps_print_info("download bfgx cali data addr:%s\n", addr);

    /* buff_tx="FILES 0xXXXX " */
    /* ���͵�ַ */
    l_ret = patch_send(buff_tx, len, MSG_FORM_DRV_C);
    if (l_ret < 0) {
        ps_print_err(" SEND file addr error\n");

        return -EFAIL;
    }

    /* ��ȡbfgxУ׼���� */
    l_ret = patch_down_cali_data();
    if (l_ret < 0) {
        ps_print_err("send cali data error\n");

        return l_ret;
    }

    return SUCC;
}

STATIC int32 patch_number_cali_bfgx_data(uint8 *key, uint8 *value)
{
    int32 ret;

    /* ����BFGX��У׼���� */
    ret = bfgx_cali_data_download(FILES_CMD_KEYWORD, value);
    if (ret < 0) {
        ps_print_err("download bfgx cali data fail\n");
    }

    return ret;
}

/* ����������� */
STATIC FIRMWARE_NUMBER_STRUCT g_number_cmd_type[] = {
    {VER_CMD_KEYWORD,   patch_number_version_cmd},
    {WMEM_CMD_KEYWORD,  patch_number_wmem_cmd},
    {BRT_CMD_KEYWORD,   patch_number_baud_cmd},
    {CALI_BFGX_DATA_CMD_KEYWORD, patch_number_cali_bfgx_data},
    {JUMP_CMD_KEYWORD,  patch_number_jump_cmd},
    {RMEM_CMD_KEYWORD,  patch_number_rmem_cmd}
};

int32 patch_number_type(uint8 *Key, uint8 *Value)
{
    int32 ret = -EFAIL;
    int32 loop;

    for (loop = 0; loop < oal_array_size(g_number_cmd_type); loop++) {
        if (!os_str_cmp(Key, g_number_cmd_type[loop].key)) {
            ret = (g_number_cmd_type[loop].cmd_exec)(Key, Value);
            break;
        }
    }

    return ret;
}

/*
 * Prototype    : patch_quit_type
 * Description  : down quit command
 */
int32 patch_quit_type(void)
{
    int32 l_ret;
    int32 l_len;
    const uint32 ul_buf_len = 8;
    uint8 buf[ul_buf_len];
    ps_print_dbg("entry\n");

    memset_s(buf, sizeof(buf), 0, sizeof(buf));

    l_ret = memcpy_s(buf, sizeof(buf), (uint8 *)QUIT_CMD_KEYWORD, os_str_len(QUIT_CMD_KEYWORD));
    if (l_ret != EOK) {
        ps_print_err("buf not enough\n");
        return -EFAIL;
    }
    l_len = os_str_len(QUIT_CMD_KEYWORD);

    buf[l_len] = COMPART_KEYWORD;
    l_len++;

    ps_patch_to_nomal();

    l_ret = send_msg_t(buf, l_len);

    return l_ret;
}

int32 patch_file_addr_send(uint8 *data, int32 data_len, const char *file_path)
{
    int32 l_ret;

    l_ret = patch_send(data, data_len, MSG_FORM_DRV_C);
    if (l_ret < 0) {
        ps_print_err(" SEND file addr error\n");

        return -EFAIL;
    }

    ps_print_dbg("file path is %s\n", file_path);

    l_ret = patch_down_file(file_path);
    ps_print_dbg("patch_down_file:%d", l_ret);
    if (l_ret < 0) {
        ps_print_err(" SEND file error\n");

        return l_ret;
    }
    /* G �� DEVICE �����Ӧ������־ */
    l_ret = patch_wait_g_form_dev();

    return l_ret;
}

/*
 * Prototype    : patch_file_type
 * Description  : down addr and file
 */
int32 patch_file_type(uint8 *Key, const char *Value)
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

    ps_print_dbg("Key = %s, Value = %s\n", Key, Value);

    /*
     * ���ݹؼ��ֵ����һ���ַ���ȷ�����͵�ַ֮��device�ķ���ֵ
     * ���������ļ��Ĺؼ��ֲ��������޸�
     */
    memset_s(data, DATA_BUF_LEN, 0, DATA_BUF_LEN);
    data_len = os_str_len(Key);
    l_ret = memcpy_s(data, sizeof(data), Key, data_len);
    if (l_ret != EOK) {
        ps_print_err("data not enough\n");
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
        ps_print_err("para [%s] not begin with 1", tmp1);
        return -EFAIL;
    }
    tmp = os_str_chr(tmp1, ',');
    if (tmp == NULL) {
        ps_print_err("has no ',' string:[%s]", tmp);
        return -EFAIL;
    }
    tmp++;
    ps_print_dbg("tmp is %s\n", tmp);

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
        ps_print_err("data buff not enough\n");
        return -EFAIL;
    }
    data_len = data_len + n;

    data[data_len] = COMPART_KEYWORD;
    data_len++;

    ps_print_dbg("data is %s\n", data);

    /* ɾ��ͷ���Ŀո� */
    l_len = os_str_len((int8 *)tmp);
    for (i = i + 1; i < l_len; i++) {
        /* ���ݾ���·�������·�� */
        if ((tmp[i] == '/') || (tmp[i] == '.')) {
            break;
        }
    }

    l_ret = patch_file_addr_send(data, data_len, &tmp[i]);
    return l_ret;
}

void *patch_malloc_cmd_buf(uint8 *buf)
{
    int32 l_len;
    uint8 *flag = NULL;
    uint8 *p_buf = NULL;

    /* ͳ��������� */
    flag = buf;
    g_patch_ctrl.l_count = 0;
    while (flag != NULL) {
        /* һ����ȷ�������н�����Ϊ ; */
        flag = os_str_chr(flag, CMD_LINE_SIGN);
        if (flag == NULL) {
            break;
        }
        g_patch_ctrl.l_count++;
        flag++;
    }

    ps_print_info("l_count = %d\n", g_patch_ctrl.l_count);

    /* ����洢����ռ� */
    l_len = (g_patch_ctrl.l_count + CFG_INFO_RESERVE_LEN) * sizeof(struct cmd_type_st);
    p_buf = os_kmalloc_gfp(l_len);
    if (p_buf == NULL) {
        ps_print_err("kmalloc cmd_type_st fail\n");
        return NULL;
    }
    memset_s((void *)p_buf, l_len, 0, l_len);

    return p_buf;
}

/*
 * Prototype    : patch_parse_cfg
 * Description  : parse config file
 */
int32 patch_parse_cfg(uint8 *buf, int32 buf_len)
{
    int32 i;
    int32 l_len;
    uint8 *flag = NULL;
    uint8 *begin = NULL;
    uint8 *end = NULL;
    int32 cmd_type;
    uint8 cmd_name[DOWNLOAD_CMD_LEN];
    uint8 cmd_para[DOWNLOAD_CMD_PARA_LEN];
    uint32 cmd_para_len;
    int32 ret;

    if (buf == NULL) {
        ps_print_err("buf is NULL!\n");
        return -EFAIL;
    }

    g_patch_ctrl.pst_cmd = (struct cmd_type_st *)patch_malloc_cmd_buf(buf);
    if (g_patch_ctrl.pst_cmd == NULL) {
        ps_print_err(" patch_malloc_cmd_buf fail\n");
        return -EFAIL;
    }

    /* ����CMD BUF */
    flag = buf;
    l_len = buf_len;
    i = 0;
    while ((i < g_patch_ctrl.l_count) && (flag < &buf[l_len])) {
        /*
         * ��ȡ�����ļ��е�һ��,�����ļ�������unix��ʽ.
         * �����ļ��е�ĳһ�к����ַ� @ ����Ϊ����Ϊע����
         */
        begin = flag;
        end = os_str_chr(flag, '\n');
        if (end == NULL) { /* �ļ������һ�У�û�л��з� */
            ps_print_dbg("lost of new line!\n");
            end = &buf[l_len];
        } else if (end == begin) { /* ����ֻ��һ�����з� */
            ps_print_dbg("blank line\n");
            flag = end + 1;
            continue;
        }
        *end = '\0';

        ps_print_info("operation string is [%s]\n", begin);

        memset_s(cmd_name, DOWNLOAD_CMD_LEN, 0, DOWNLOAD_CMD_LEN);
        memset_s(cmd_para, DOWNLOAD_CMD_PARA_LEN, 0, DOWNLOAD_CMD_PARA_LEN);

        cmd_type = firmware_parse_cmd(begin, cmd_name, sizeof(cmd_name), cmd_para, sizeof(cmd_para));

        ps_print_info("cmd type=[%d],cmd_name=[%s],cmd_para=[%s]\n", cmd_type, cmd_name, cmd_para);

        if (cmd_type != ERROR_TYPE_CMD) { /* ��ȷ���������ͣ����� */
            g_patch_ctrl.pst_cmd[i].cmd_type = cmd_type;
            memcpy_s(g_patch_ctrl.pst_cmd[i].cmd_name, DOWNLOAD_CMD_LEN, cmd_name, DOWNLOAD_CMD_LEN);
            memcpy_s(g_patch_ctrl.pst_cmd[i].cmd_para, DOWNLOAD_CMD_PARA_LEN,
                     cmd_para, DOWNLOAD_CMD_PARA_LEN);
            /* ��ȡ���ð汾�� */
            if (!os_mem_cmp(g_patch_ctrl.pst_cmd[i].cmd_name,
                            VER_CMD_KEYWORD,
                            os_str_len(VER_CMD_KEYWORD))) {
                cmd_para_len = os_str_len(g_patch_ctrl.pst_cmd[i].cmd_para);
                ret = memcpy_s(g_patch_ctrl.auc_CfgVersion, sizeof(g_patch_ctrl.auc_CfgVersion),
                               g_patch_ctrl.pst_cmd[i].cmd_para, cmd_para_len);
                if (ret != EOK) {
                    ps_print_err("cmd_para_len = %d over auc_CfgVersion length\n", cmd_para_len);
                    return -EFAIL;
                }

                ps_print_dbg("g_CfgVersion = [%s],[%s]\n",
                             g_patch_ctrl.auc_CfgVersion, g_patch_ctrl.pst_cmd[i].cmd_para);
            }
            i++;
        }
        flag = end + 1;
    }

    /* ����ʵ������������޸����յ�������� */
    g_patch_ctrl.l_count = i;
    ps_print_info("cmd count[%d]\n", g_patch_ctrl.l_count);

    return SUCC;
}

/*
 * Prototype    : patch_get_cfg
 * Description  : get patch config command
 */
int32 patch_get_cfg(const char *cfg)
{
    uint8 *buf = NULL;
    int32 l_readlen;
    int32 l_ret;

    if (cfg == NULL) {
        ps_print_err("cfg file path null!\n");
        return -EFAIL;
    }

    buf = os_kmalloc_gfp(READ_CFG_BUF_LEN);
    if (buf == NULL) {
        ps_print_err("malloc READ_CFG_BUF fail!\n");
        return -EFAIL;
    }

    /* cfg�ļ��޶���С��2048,���cfg�ļ��Ĵ�Сȷʵ����2048�������޸�READ_CFG_BUF_LEN��ֵ */
    l_readlen = firmware_read_cfg(cfg, buf);
    if (l_readlen < 0) {
        ps_print_err("read cfg error\n");
        os_mem_kfree(buf);
        buf = NULL;
        return -EFAIL;
    } else if (l_readlen > READ_CFG_BUF_LEN - 1) {   
        /* ��1��Ϊ��ȷ��cfg�ļ��ĳ��Ȳ�����READ_CFG_BUF_LEN����Ϊfirmware_read_cfg���ֻ���ȡREAD_CFG_BUF_LEN���ȵ����� */
        ps_print_err("cfg file [%s] larger than %d\n", cfg, READ_CFG_BUF_LEN);
        os_mem_kfree(buf);
        buf = NULL;
        return -EFAIL;
    } else {
        ps_print_info("read cfg file [%s] ok, size is [%d]\n", cfg, l_readlen);
    }

    l_ret = patch_parse_cfg(buf, l_readlen);
    if (l_ret < 0) {
        ps_print_err("parse cfg error\n");
    }

    os_mem_kfree(buf);
    buf = NULL;

    return l_ret;
}

/*
 * Prototype    : patch_download_info
 * Description  : download patch
 */
int32 patch_execute_cmd(int32 cmd_type, uint8 *cmd_name, uint8 *cmd_para)
{
    int32 l_ret;

    /* ����ϴβ����������������ݣ���ȡ���ʱ�Գ���Ϊ�жϣ�buf�Ͳ�������� */
    g_patch_ctrl.l_Recvbuf1_len = 0;
    g_patch_ctrl.l_Recvbuf2_len = 0;

    switch (cmd_type) {
        case FILE_TYPE_CMD:
            ps_print_dbg(" command type FILE_TYPE_CMD\n");
            l_ret = patch_file_type(cmd_name, cmd_para);
            break;
        case NUM_TYPE_CMD:
            ps_print_dbg(" command type NUM_TYPE_CMD\n");
            l_ret = patch_number_type(cmd_name, cmd_para);

            break;
        case QUIT_TYPE_CMD:
            ps_print_dbg(" command type QUIT_TYPE_CMD\n");
            l_ret = patch_quit_type();
            break;

        default:
            ps_print_err("command type error[%d]\n", cmd_type);
            l_ret = -EFAIL;
            break;
    }

    return l_ret;
}

/*
 * Prototype    : patch_download_patch
 * Description  : download patch
 */
int32 patch_download_patch(void)
{
    int32 l_ret;
    int32 i;
    uint32 ul_alloc_len = READ_DATA_BUF_LEN;

    g_stringbuf.pbufstart = kmalloc(ul_alloc_len, GFP_KERNEL);
    if (g_stringbuf.pbufstart == NULL) {
        ul_alloc_len = READ_DATA_REALLOC_BUF_LEN;
        g_stringbuf.pbufstart = kmalloc(ul_alloc_len, GFP_KERNEL);
        if (g_stringbuf.pbufstart == NULL) {
            ps_print_err("ringbuf KMALLOC SIZE(%d) failed.\n", ul_alloc_len);
            g_stringbuf.pbufstart = g_patch_ctrl.auc_Recvbuf1;
            g_stringbuf.pbufend = RECV_BUF_LEN + g_stringbuf.pbufstart;

            return -EFAIL;
        }
    }

    ps_print_info("ringbuf kmalloc size(%d) suc.\n", ul_alloc_len);
    g_stringbuf.pbufend = ul_alloc_len + g_stringbuf.pbufstart;

    g_stringbuf.phead = g_stringbuf.pbufstart;
    g_stringbuf.ptail = g_stringbuf.pbufstart;

    /* ִ������:: ������û�ж��꣬����Ǵ�������  */
    for (i = 0; i < g_patch_ctrl.l_count; i++) {
        ps_print_info("cmd[%d]type[%d], name[%s], para[%s]\n",
                      i,
                      g_patch_ctrl.pst_cmd[i].cmd_type,
                      g_patch_ctrl.pst_cmd[i].cmd_name,
                      g_patch_ctrl.pst_cmd[i].cmd_para);

        l_ret = patch_execute_cmd(g_patch_ctrl.pst_cmd[i].cmd_type,
                                  g_patch_ctrl.pst_cmd[i].cmd_name,
                                  g_patch_ctrl.pst_cmd[i].cmd_para);
        if (l_ret < 0) {
            kfree(g_stringbuf.pbufstart);
            g_stringbuf.pbufstart = NULL;
            g_stringbuf.pbufend = NULL;
            g_stringbuf.phead = NULL;
            g_stringbuf.ptail = NULL;
            return l_ret;
        }
    }

    kfree(g_stringbuf.pbufstart);
    g_stringbuf.pbufstart = NULL;
    g_stringbuf.pbufend = NULL;
    g_stringbuf.phead = NULL;
    g_stringbuf.ptail = NULL;

    return SUCC;
}

STATIC void uart_cfg_file_init(void)
{
    int32 l_subchip_type = get_hi110x_subchip_type();
    if (l_subchip_type == BOARD_VERSION_HI1105) {
        if (!hi110x_is_asic()) {
            memcpy_s(g_patch_ctrl.auc_Cfgpath, sizeof(g_patch_ctrl.auc_Cfgpath),
                     UART_CFG_FILE_FPGA_HI1105, os_str_len(UART_CFG_FILE_FPGA_HI1105));
        } else {
            memcpy_s(g_patch_ctrl.auc_Cfgpath, sizeof(g_patch_ctrl.auc_Cfgpath),
                     UART_CFG_FILE_PILOT_HI1105, os_str_len(UART_CFG_FILE_PILOT_HI1105));
        }
    } else if (l_subchip_type == BOARD_VERSION_HI1106) {
        if (!hi110x_is_asic()) {
            memcpy_s(g_patch_ctrl.auc_Cfgpath, sizeof(g_patch_ctrl.auc_Cfgpath),
                     UART_CFG_FILE_FPGA_HI1106, os_str_len(UART_CFG_FILE_FPGA_HI1106));
        } else {
            memcpy_s(g_patch_ctrl.auc_Cfgpath, sizeof(g_patch_ctrl.auc_Cfgpath),
                     UART_CFG_FILE_PILOT_HI1106, os_str_len(UART_CFG_FILE_PILOT_HI1106));
        }
    } else {
        ps_print_err("subchip %d unsupport uart download\n", l_subchip_type);
    }
}

/*
 * Prototype    : patch_init
 * Description  : patch module initialization
 */
int32 patch_init(void)
{
    int32 l_ret;

    uart_cfg_file_init();

    g_patch_ctrl.pst_wait = os_kmalloc_gfp(sizeof(OS_WAIT_QUEUE_HEAD_T_STRU));
    if (g_patch_ctrl.pst_wait == NULL) {
        ps_print_err("malloc wait queue fail, size:%lu\n", sizeof(OS_WAIT_QUEUE_HEAD_T_STRU));
        return -EFAIL;
    }

    os_init_waitqueue_head(g_patch_ctrl.pst_wait);

    l_ret = patch_get_cfg(g_patch_ctrl.auc_Cfgpath);
    if (l_ret < 0) {
        ps_print_err("get [%s] command is fail\n", g_patch_ctrl.auc_Cfgpath);
        return -EFAIL;
    }

    if (g_data_buf == NULL) {
        g_data_buf = os_kmalloc_gfp(READ_PATCH_BUF_LEN);
        if (g_data_buf == NULL) {
            ps_print_err("DataBuf KMALLOC failed");
            g_data_buf = NULL;
            return -EFAIL;
        } else {
            ps_print_dbg("DataBuf KMALLOC succ");
        }
    }

    return SUCC;
}

/*
 * Prototype    : patch_exit
 * Description  : patch module exit
 */
int32 patch_exit(void)
{
    g_patch_ctrl.l_count = 0;
    if (g_patch_ctrl.pst_cmd != NULL) {
        os_mem_kfree(g_patch_ctrl.pst_cmd);
        g_patch_ctrl.pst_cmd = NULL;
    }

    if (g_patch_ctrl.pst_wait != NULL) {
        os_mem_kfree(g_patch_ctrl.pst_wait);
        g_patch_ctrl.pst_wait = NULL;
    }

    if (g_data_buf != NULL) {
        os_mem_kfree(g_data_buf);
        g_data_buf = NULL;
    }

    return SUCC;
}

/*
 * Prototype    : bfg_patch_download_function
 * Description  : download bfg patch
 * Return       : 0 means succeed,-1 means failed
 */
int bfg_patch_download_function(void)
{
#define RETRY_CNT 3
    int32 ret;
    int32 counter = 0;
    ktime_t start_time, end_time, trans_time;
    static unsigned long long max_time = 0;
    static unsigned long long count = 0;
    unsigned long long total_time = 0;
    struct ps_core_s *ps_core_d = NULL;

    ps_get_core_reference(&ps_core_d, BUART);
    if (unlikely(ps_core_d == NULL)) {
        ps_print_err("ps_core_d is err\n");
        return -1;
    }

    ps_print_dbg("enter\n");
    start_time = ktime_get();

    /* patch_init(uart) function should just be called once */
    ret = patch_init();
    if (ret) {
        ps_print_err("patch modem init failed, ret:%d!\n", ret);
        return ret;
    }

    /* bfg patch download, three times for fail */
    for (counter = 0; counter < RETRY_CNT; counter++) {
        ret = patch_download_patch();
        if (ret) {
            ps_print_err("bfg patch download fail, and reset power!\n");
            ps_uart_state_dump(ps_core_d);
        } else {
            end_time = ktime_get();
            trans_time = ktime_sub(end_time, start_time);
            total_time = (unsigned long long)ktime_to_us(trans_time);
            if (total_time > max_time) {
                max_time = total_time;
            }

            ps_print_warning("download bfg patch succ,count [%llu], current time [%llu]us, max time [%llu]us\n",
                             count, total_time, max_time);
            /* download patch successfully */
            return ret;
        }
    }

    /* going to exception */
    ps_print_err("bfg patch download has failed finally!\n");
    return ret;
}

