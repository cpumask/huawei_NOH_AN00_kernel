

/* ͷ�ļ����� */
#include "plat_firmware.h"

#include <linux/moduleparam.h>
#include <linux/delay.h>
#include <linux/rtc.h>

#include "plat_debug.h"
#include "platform_common_clk.h"
#include "plat_uart.h"
#include "plat_cali.h"
#include "plat_pm.h"
#include "oal_sdio_host_if.h"
#include "oal_hcc_host_if.h"
#include "oam_ext_if.h"
#include "oal_kernel_file.h"
#include "bfgx_dev.h"
#include "bfgx_exception_rst.h"
#include "oal_util.h"
#include "securec.h"

/* ȫ�ֱ������� */
/* hi1103 pilot cfg�ļ�·�� */
uint8 *g_hi1103_pilot_cfg_patch_in_vendor[CFG_FILE_TOTAL] = {
    BFGX_AND_WIFI_CFG_HI1103_PILOT_PATH,
    WIFI_CFG_HI1103_PILOT_PATH,
    BFGX_CFG_HI1103_PILOT_PATH,
    RAM_CHECK_CFG_HI1103_PILOT_PATH,
};

/* hi1105 fpga cfg�ļ�·�� */
STATIC uint8 *g_hi1105_fpga_cfg_patch_in_vendor[CFG_FILE_TOTAL] = {
    BFGX_AND_WIFI_CFG_HI1105_FPGA_PATH,
    WIFI_CFG_HI1105_FPGA_PATH,
    BFGX_CFG_HI1105_FPGA_PATH,
    RAM_CHECK_CFG_HI1105_FPGA_PATH,
};

/* hi1105 asic cfg�ļ�·�� */
uint8 *g_hi1105_asic_cfg_patch_in_vendor[CFG_FILE_TOTAL] = {
    BFGX_AND_WIFI_CFG_HI1105_ASIC_PATH,
    WIFI_CFG_HI1105_ASIC_PATH,
    BFGX_CFG_HI1105_ASIC_PATH,
    RAM_CHECK_CFG_HI1105_ASIC_PATH,
};

/* hi1106 fpga cfg�ļ�·�� */
STATIC uint8 *g_hi1106_fpga_cfg_patch_in_vendor[CFG_FILE_TOTAL] = {
    BFGX_AND_WIFI_CFG_HI1106_FPGA_PATH,
    WIFI_CFG_HI1106_FPGA_PATH,
    BFGX_CFG_HI1106_FPGA_PATH,
    RAM_CHECK_CFG_HI1106_FPGA_PATH,
};

/* hi1106 pilot cfg�ļ�·�� */
STATIC uint8 *g_hi1106_asic_cfg_patch_in_vendor[CFG_FILE_TOTAL] = {
    BFGX_AND_WIFI_CFG_HI1106_ASIC_PATH,
    WIFI_CFG_HI1106_ASIC_PATH,
    BFGX_CFG_HI1106_ASIC_PATH,
    RAM_CHECK_CFG_HI1106_ASIC_PATH,
};

STATIC uint32 g_asic_type = HI1103_ASIC_PILOT;

uint8 **g_cfg_path = g_hi1103_pilot_cfg_patch_in_vendor;

/* �洢cfg�ļ���Ϣ������cfg�ļ�ʱ��ֵ�����ص�ʱ��ʹ�øñ��� */
FIRMWARE_GLOBALS_STRUCT g_cfg_info;

/* ����firmware file���ݵ�buffer���Ƚ��ļ��������buffer�У�Ȼ��������device buffer���� */
STATIC uint8 *g_firmware_down_buf = NULL;

/* DataBuf�ĳ��� */
STATIC uint32 g_firmware_down_buf_len = 0;

STATIC int32 exec_number_wmem_cmd(uint8 *key, uint8 *value);
STATIC int32 exec_number_sleep_cmd(uint8 *key, uint8 *value);
STATIC int32 exec_number_cali_count_cmd(uint8 *key, uint8 *value);
STATIC int32 exec_number_cali_bfgx_data(uint8 *key, uint8 *value);
STATIC int32 exec_number_cali_dcxo_data(uint8 *key, uint8 *value);
STATIC int32 exec_number_jump_cmd(uint8 *key, uint8 *value);
STATIC int32 exec_number_set_cmd(uint8 *key, uint8 *value);
STATIC int32 exec_number_rmem_cmd(uint8 *key, uint8 *value);

/* ����������� */
STATIC FIRMWARE_NUMBER_STRUCT g_number_type[] = {
    {WMEM_CMD_KEYWORD,  exec_number_wmem_cmd},
    {SLEEP_CMD_KEYWORD, exec_number_sleep_cmd},
    {CALI_COUNT_CMD_KEYWORD, exec_number_cali_count_cmd},
    {CALI_BFGX_DATA_CMD_KEYWORD, exec_number_cali_bfgx_data},
    {CALI_DCXO_DATA_CMD_KEYWORD, exec_number_cali_dcxo_data},
    {JUMP_CMD_KEYWORD, exec_number_jump_cmd},
    {SETPM_CMD_KEYWORD, exec_number_set_cmd},
    {SETBUCK_CMD_KEYWORD, exec_number_set_cmd},
    {SETSYSLDO_CMD_KEYWORD, exec_number_set_cmd},
    {SETNFCRETLDO_CMD_KEYWORD, exec_number_set_cmd},
    {SETPD_CMD_KEYWORD, exec_number_set_cmd},
    {SETNFCCRG_CMD_KEYWORD, exec_number_set_cmd},
    {SETABB_CMD_KEYWORD, exec_number_set_cmd},
    {SETTCXODIV_CMD_KEYWORD, exec_number_set_cmd},
    {RMEM_CMD_KEYWORD, exec_number_rmem_cmd}
};

/*
 * �� �� ��  : set_hi110x_asic_type
 * ��������  : ����hi1103 asic����(MPW2/PILOT)
 * �������  : 0-MPW2��1-PILOT
 */
STATIC void set_hi110x_asic_type(uint32 ul_asic_type)
{
    g_asic_type = ul_asic_type;
}

/*
 * �� �� ��  : get_hi110x_asic_type
 * ��������  : ��ȡhi1103 asic����(MPW2/PILOT)
 * �� �� ֵ  : 0-MPW2��1-PILOT
 */
uint32 get_hi110x_asic_type(void)
{
    return g_asic_type;
}

/*
 * �� �� ��  : read_msg
 * ��������  : host����device��������Ϣ
 * �������  : data: ������Ϣ��buffer
 *             len : ����buffer�ĳ���
 * �� �� ֵ  : -1��ʾʧ�ܣ����򷵻�ʵ�ʽ��յĳ���
 */
int32 read_msg(uint8 *data, int32 len)
{
    int32 l_len;
    hcc_bus *pst_bus = hcc_get_current_110x_bus();

    if (unlikely((data == NULL))) {
        ps_print_err("data is NULL\n ");
        return -EFAIL;
    }

    if (unlikely((pst_bus == NULL))) {
        ps_print_err("pst_bus is NULL\n ");
        return -EFAIL;
    }

    l_len = hcc_bus_patch_read(pst_bus, data, len, READ_MEG_TIMEOUT);
    ps_print_dbg("Receive l_len=[%d] \n", l_len);

    return l_len;
}

STATIC int32 read_msg_timeout(uint8 *data, int32 len, uint32 timeout)
{
    int32 l_len;
    hcc_bus *pst_bus = hcc_get_current_110x_bus();

    if (unlikely((data == NULL))) {
        ps_print_err("data is NULL\n ");
        return -EFAIL;
    }

    if (unlikely((pst_bus == NULL))) {
        ps_print_err("pst_bus is NULL\n ");
        return -EFAIL;
    }

    l_len = hcc_bus_patch_read(pst_bus, data, len, timeout);
    ps_print_dbg("Receive l_len=[%d], data = [%s]\n", l_len, data);

    return l_len;
}

/*
 * �� �� ��  : send_msg
 * ��������  : host��device������Ϣ
 * �������  : data: ����buffer
 *             len : �������ݵĳ���
 * �� �� ֵ  : -1��ʾʧ�ܣ����򷵻�ʵ�ʷ��͵ĳ���
 */
int32 send_msg(uint8 *data, int32 len)
{
    int32 l_ret;
    hcc_bus *pst_bus = hcc_get_current_110x_bus();

    if (unlikely((pst_bus == NULL))) {
        ps_print_err("pst_bus is NULL\n ");
        return -EFAIL;
    }

    ps_print_dbg("len = %d\n", len);
#ifdef HW_DEBUG
    const uint32 ul_max_print_len = 128;
    print_hex_dump_bytes("send_msg :", DUMP_PREFIX_ADDRESS, data,
                         (len < ul_max_print_len ? len : ul_max_print_len));
#endif
    l_ret = hcc_bus_patch_write(pst_bus, data, len);

    return l_ret;
}

/*
 * �� �� ��  : recv_expect_result
 * ��������  : ����host����device��ȷ���ص�����
 * �������  : expect: ����device��ȷ���ص�����
 * �� �� ֵ  : 0��ʾ�ɹ���-1��ʾʧ��
 */
int32 recv_expect_result(const uint8 *expect)
{
    uint8 auc_buf[RECV_BUF_LEN];
    int32 l_len;
    int32 i;

    if (!os_str_len(expect)) {
        ps_print_dbg("not wait device to respond!\n");
        return SUCC;
    }

    memset_s(auc_buf, RECV_BUF_LEN, 0, RECV_BUF_LEN);
    for (i = 0; i < HOST_DEV_TIMEOUT; i++) {
        l_len = read_msg(auc_buf, RECV_BUF_LEN);
        if (l_len < 0) {
            ps_print_err("recv result fail\n");
            continue;
        }

        if (!os_mem_cmp(auc_buf, expect, os_str_len(expect))) {
            ps_print_dbg(" send SUCC, expect [%s] ok\n", expect);
            return SUCC;
        } else {
            ps_print_warning(" error result[%s], expect [%s], read result again\n", auc_buf, expect);
        }
    }

    return -EFAIL;
}

STATIC int32 recv_expect_result_timeout(const uint8 *expect, uint32 timeout)
{
    uint8 auc_buf[RECV_BUF_LEN];
    int32 l_len;

    if (!os_str_len(expect)) {
        ps_print_dbg("not wait device to respond!\n");
        return SUCC;
    }

    memset_s(auc_buf, RECV_BUF_LEN, 0, RECV_BUF_LEN);
    l_len = read_msg_timeout(auc_buf, RECV_BUF_LEN, timeout);
    if (l_len < 0) {
        ps_print_err("recv result fail\n");
        return -EFAIL;
    }

    if (!os_mem_cmp(auc_buf, expect, os_str_len(expect))) {
        ps_print_dbg(" send SUCC, expect [%s] ok\n", expect);
        return SUCC;
    } else {
        ps_print_warning(" error result[%s], expect [%s], read result again\n", auc_buf, expect);
    }

    return -EFAIL;
}

/*
 * �� �� ��  : msg_send_and_recv_except
 * ��������  : host��device������Ϣ���ȴ�device������Ϣ
 * �������  : data  : ����buffer
 *             len   : �������ݵĳ���
 *             expect: ����device�ظ�������
 * �� �� ֵ  : -1��ʾʧ�ܣ�0��ʾ�ɹ�
 */
STATIC int32 msg_send_and_recv_except(uint8 *data, int32 len, const uint8 *expect)
{
    int32 i;
    int32 l_ret;

    for (i = 0; i < HOST_DEV_TIMEOUT; i++) {
        l_ret = send_msg(data, len);
        if (l_ret < 0) {
            continue;
        }

        l_ret = recv_expect_result(expect);
        if (l_ret == 0) {
            return SUCC;
        }
    }

    return -EFAIL;
}

/*
 * �� �� ��  : malloc_cmd_buf
 * ��������  : ����cfg�ļ����������Ľ��������g_st_cfg_infoȫ�ֱ�����
 * �������  : puc_cfg_info_buf: ������cfg�ļ����ݵ�buffer
 *             ul_index        : ��������������������ֵ
 * �� �� ֵ  : NULL��ʾ�����ڴ�ʧ�ܣ����򷵻�ָ�򱣴����cfg�ļ�����������׵�ַ
 */
STATIC void *malloc_cmd_buf(uint8 *puc_cfg_info_buf, uint32 ul_index)
{
    int32 l_len;
    uint8 *flag = NULL;
    uint8 *p_buf = NULL;

    if (puc_cfg_info_buf == NULL) {
        ps_print_err("malloc_cmd_buf: buf is NULL!\n");
        return NULL;
    }

    /* ͳ��������� */
    flag = puc_cfg_info_buf;
    g_cfg_info.al_count[ul_index] = 0;
    while (flag != NULL) {
        /* һ����ȷ�������н�����Ϊ ; */
        flag = os_str_chr(flag, CMD_LINE_SIGN);
        if (flag == NULL) {
            break;
        }
        g_cfg_info.al_count[ul_index]++;
        flag++;
    }
    ps_print_dbg("cfg file cmd count: al_count[%d] = %d\n", ul_index, g_cfg_info.al_count[ul_index]);

    /* ����洢����ռ� */
    l_len = ((g_cfg_info.al_count[ul_index]) + CFG_INFO_RESERVE_LEN) * sizeof(struct cmd_type_st);
    p_buf = os_kmalloc_gfp(l_len);
    if (p_buf == NULL) {
        ps_print_err("kmalloc cmd_type_st fail\n");
        return NULL;
    }
    memset_s((void *)p_buf, l_len, 0, l_len);

    return p_buf;
}

/*
 * �� �� ��  : delete_space
 * ��������  : ɾ���ַ������߶���Ŀո�
 * �������  : string: ԭʼ�ַ���
 *            len   : �ַ����ĳ���
 * �� �� ֵ  : ���󷵻�NULL�����򷵻�ɾ�����߿ո��Ժ��ַ������׵�ַ
 */
uint8 *delete_space(uint8 *string, int32 *len)
{
    int i;

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
        ps_print_err(" string is Space bar\n");
        return NULL;
    }
    /* ��for����м�ȥ1���������1 */
    *len = i + 1;

    /* ɾ��ͷ���Ŀո� */
    for (i = 0; i < *len; i++) {
        if (string[i] != COMPART_KEYWORD) {
            /* ��ȥ�ո�ĸ��� */
            *len = *len - i;
            return &string[i];
        }
    }

    return NULL;
}

/*
 * �� �� ��  : string_to_num
 * ��������  : ���ַ���ת����������
 * �������  : string:������ַ���
 * �������  : number:�ַ���ת���Ժ��������
 * �� �� ֵ  : 0��ʾ�ɹ���-1��ʾʧ��
 */
STATIC int32 string_to_num(uint8 *string, int32 *number)
{
    int32 i;
    int32 l_num;

    if (string == NULL) {
        ps_print_err("string is NULL!\n");
        return -EFAIL;
    }

    l_num = 0;
    for (i = 0; (string[i] >= '0') && (string[i] <= '9'); i++) {
        l_num = (l_num * 10) + (string[i] - '0'); /* �ַ���ת�����߼���Ҫ*10 */
    }

    *number = l_num;

    return SUCC;
}

/*
 * �� �� ��  : num_to_string
 * ��������  : ��������ת�����ַ���
 * �������  : number:�����������
 * �������  : number:������ת���Ժ���ַ���
 * �� �� ֵ  : 0��ʾ�ɹ���-1��ʾʧ��
 */
STATIC int32 num_to_string(uint8 *string, uint32 number)
{
    int32 i = 0;
    int32 j = 0;
    int32 tmp[INT32_STR_LEN];
    uint32 num = number;

    if (string == NULL) {
        ps_print_err("string is NULL!\n");
        return -EFAIL;
    }

    do {
        tmp[i] = num % 10; /* ��������ת�ַ����߼���Ҫ%10 */
        num = num / 10; /* ��������ת�ַ����߼���Ҫ/10 */
        i++;
    } while (num != 0);

    do {
        string[j] = tmp[i - 1 - j] + '0';
        j++;
    } while (j != i);

    string[j] = '\0';

    return SUCC;
}

/*
 * �� �� ��  : open_file_to_readm
 * ��������  : ���ļ�������read mem������������
 * �� �� ֵ  : ���ش��ļ���������
 */
STATIC OS_KERNEL_FILE_STRU *open_file_to_readm(uint8 *name)
{
    OS_KERNEL_FILE_STRU *fp = NULL;
    uint8 *file_name = NULL;
    mm_segment_t fs;

    if (name == NULL) {
        file_name = WIFI_DUMP_PATH "/readm_wifi";
    } else {
        file_name = name;
    }

    fs = get_fs();
    set_fs(KERNEL_DS);
    fp = filp_open(file_name, O_RDWR | O_CREAT, 0664); /* 0664Ȩ�ޣ�ӵ���߿ɶ�д */
    set_fs(fs);

    return fp;
}

/*
 * �� �� ��  : recv_device_mem
 * ��������  : ����device�����������ڴ棬���浽ָ�����ļ���
 * �������  : fp : �����ڴ���ļ�ָ��
 *             len: ��Ҫ������ڴ�ĳ���
 * �� �� ֵ  : -1��ʾʧ�ܣ����򷵻�ʵ�ʱ�����ڴ�ĳ���
 */
STATIC int32 recv_device_mem(OS_KERNEL_FILE_STRU *fp, uint8 *pucDataBuf, int32 len)
{
    int32 l_ret = -EFAIL;
    mm_segment_t fs;
    uint8 retry = 3;
    int32 lenbuf = 0;

    if (oal_is_err_or_null(fp)) {
        ps_print_err("fp is error,fp = 0x%p\n", fp);
        return -EFAIL;
    }

    if (pucDataBuf == NULL) {
        ps_print_err("pucDataBuf is NULL\n");
        return -EFAIL;
    }

    ps_print_dbg("expect recv len is [%d]\n", len);

    fs = get_fs();
    set_fs(KERNEL_DS);
    ps_print_dbg("pos = %d\n", (int)fp->f_pos);
    while (len > lenbuf) {
        l_ret = read_msg(pucDataBuf + lenbuf, len - lenbuf);
        if (l_ret > 0) {
            lenbuf += l_ret;
        } else {
            retry--;
            lenbuf = 0;
            if (retry == 0) {
                l_ret = -EFAIL;
                ps_print_err("time out\n");
                break;
            }
        }
    }

    if (len <= lenbuf) {
        oal_kernel_file_write(fp, pucDataBuf, len);
    }
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 35))
    vfs_fsync(fp, 0);
#else
    vfs_fsync(fp, fp->f_path.dentry, 0);
#endif
    set_fs(fs);

    return l_ret;
}

/*
 * �� �� ��  : check_version
 * ��������  : ���������device�汾�ţ������device�ϱ��İ汾�ź�host�İ汾���Ƿ�ƥ��
 * �� �� ֵ  : -1��ʾʧ�ܣ�0��ʾ�ɹ�
 */
STATIC int32 check_version(void)
{
    int32 l_ret;
    int32 l_len;
    int32 i;
    uint8 rec_buf[VERSION_LEN];

    for (i = 0; i < HOST_DEV_TIMEOUT; i++) {
        memset_s(rec_buf, VERSION_LEN, 0, VERSION_LEN);

        l_ret = memcpy_s(rec_buf, sizeof(rec_buf), (uint8 *)VER_CMD_KEYWORD, os_str_len(VER_CMD_KEYWORD));
        if (l_ret != EOK) {
            ps_print_err("rec_buf not enough\n");
            return -EFAIL;
        }
        l_len = os_str_len(VER_CMD_KEYWORD);

        rec_buf[l_len] = COMPART_KEYWORD;
        l_len++;

        l_ret = send_msg(rec_buf, l_len);
        if (l_ret < 0) {
            ps_print_err("send version fail![%d]\n", i);
            continue;
        }

        memset_s(g_cfg_info.auc_DevVersion, VERSION_LEN, 0, VERSION_LEN);
        memset_s(rec_buf, VERSION_LEN, 0, VERSION_LEN);
        msleep(1);

        l_ret = read_msg(rec_buf, VERSION_LEN);
        if (l_ret < 0) {
            ps_print_err("read version fail![%d]\n", i);
            continue;
        }

        memcpy_s(g_cfg_info.auc_DevVersion, VERSION_LEN, rec_buf, VERSION_LEN);

        if (!os_mem_cmp((int8 *)g_cfg_info.auc_DevVersion,
                        (int8 *)g_cfg_info.auc_CfgVersion,
                        os_str_len(g_cfg_info.auc_CfgVersion))) {
            ps_print_info("Device Version = [%s], CfgVersion = [%s].\n",
                          g_cfg_info.auc_DevVersion, g_cfg_info.auc_CfgVersion);
            return SUCC;
        } else {
            ps_print_err("ERROR version,Device Version = [%s], CfgVersion = [%s].\n",
                         g_cfg_info.auc_DevVersion, g_cfg_info.auc_CfgVersion);
        }
    }

    return -EFAIL;
}

/*
 * �� �� ��  : number_type_cmd_send
 * ��������  : ����number���͵���������͵�device
 * �������  : Key  : ����Ĺؼ���
 *             Value: ����Ĳ���
 * �� �� ֵ  : -1��ʾʧ�ܣ������ʾ�ɹ�
 */
int32 number_type_cmd_send(uint8 *Key, const char *Value)
{
    int32 l_ret;
    int32 data_len;
    int32 Value_len;
    int32 i;
    int32 n;
    uint8 auc_num[INT32_STR_LEN];
    uint8 buff_tx[SEND_BUF_LEN];

    Value_len = os_str_len((int8 *)Value);

    memset_s(auc_num, INT32_STR_LEN, 0, INT32_STR_LEN);
    memset_s(buff_tx, SEND_BUF_LEN, 0, SEND_BUF_LEN);

    data_len = 0;
    data_len = os_str_len(Key);
    l_ret = memcpy_s(buff_tx, sizeof(buff_tx), Key, data_len);
    if (l_ret != EOK) {
        ps_print_err("buff_tx not enough\n");
        return -EFAIL;
    }

    buff_tx[data_len] = COMPART_KEYWORD;
    data_len = data_len + 1;

    for (i = 0, n = 0; (i <= Value_len) && (n < INT32_STR_LEN); i++) {
        if ((Value[i] == ',') || (Value_len == i)) {
            ps_print_dbg("auc_num = %s, i = %d, n = %d\n", auc_num, i, n);
            if (n == 0) {
                continue;
            }
            l_ret = memcpy_s((uint8 *)&buff_tx[data_len], sizeof(buff_tx) - data_len, auc_num, n);
            if (l_ret != EOK) {
                ps_print_err("buff_tx not enough\n");
                return -EFAIL;
            }
            data_len = data_len + n;

            buff_tx[data_len] = COMPART_KEYWORD;
            data_len = data_len + 1;

            memset_s(auc_num, INT32_STR_LEN, 0, INT32_STR_LEN);
            n = 0;
        } else if (Value[i] == COMPART_KEYWORD) {
            continue;
        } else {
            auc_num[n] = Value[i];
            n++;
        }
    }

    l_ret = send_msg(buff_tx, data_len);

    return l_ret;
}

/*
 * �� �� ��  : update_device_cali_count
 * ��������  : ʹ��WRITEM�������device��У׼�������״��ϵ�ʱΪȫ0
 * �� �� ֵ  : -1��ʾʧ�ܣ�0��ʾ�ɹ�
 *             recv_expect_result
 */
STATIC int32 update_device_cali_count(uint8 *Key, uint8 *Value)
{
    int32 l_ret;
    uint32 len, Value_len;
    uint32 number = 0;
    uint8 *addr = NULL;
    uint8 buff_tx[SEND_BUF_LEN];

    /* �������Value�ַ��������Valueֻ��һ����ַ����ʽΪ"0xXXXXX" */
    /* ����Ժ����ʽΪ"���ݿ��,Ҫд�ĵ�ַ,Ҫд��ֵ"---"4,0xXXXX,value" */
    len = 0;
    memset_s(buff_tx, SEND_BUF_LEN, 0, SEND_BUF_LEN);

    /* buff_tx="" */
    buff_tx[len] = '4';
    len++;
    buff_tx[len] = ',';
    len++;

    /* buff_tx="4," */
    Value_len = os_str_len(Value);
    addr = delete_space(Value, &Value_len);
    if (addr == NULL) {
        ps_print_err("addr is NULL, Value[%s] Value_len[%d]", Value, Value_len);
        return -EFAIL;
    }
    l_ret = memcpy_s(&buff_tx[len], sizeof(buff_tx) - len, addr, Value_len);
    if (l_ret != EOK) {
        ps_print_err("buff_tx not enough\n");
        return -EFAIL;
    }
    len += Value_len;
    buff_tx[len] = ',';
    len++;

    /* buff_tx="4,0xXXX," */
    l_ret = get_cali_count(&number);
    l_ret += num_to_string(&buff_tx[len], number);

    /* ��ʱbuff_tx="4,0xXXX,value" */
    /* ʹ��WMEM_CMD_KEYWORD������device����У׼���� */
    l_ret += number_type_cmd_send(WMEM_CMD_KEYWORD, buff_tx);
    if (l_ret < 0) {
        ps_print_err("send key=[%s],value=[%s] fail\n", Key, buff_tx);
        return l_ret;
    }

    l_ret = recv_expect_result(MSG_FROM_DEV_WRITEM_OK);
    if (l_ret < 0) {
        ps_print_err("recv expect result fail!\n");
        return l_ret;
    }

    return SUCC;
}

/*
 * �� �� ��  : download_bfgx_cali_data
 * ��������  : ʹ��files�������bfgx��У׼����
 * �� �� ֵ  : -1��ʾʧ�ܣ�0��ʾ�ɹ�
 *             recv_expect_result
 */
STATIC int32 download_bfgx_cali_data(uint8 *Key, uint8 *Value)
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
    len = os_str_len(Key);
    l_ret = memcpy_s(buff_tx, sizeof(buff_tx), Key, len);
    if (l_ret != EOK) {
        ps_print_err("buff_tx not enough\n");
        return -EFAIL;
    }
    buff_tx[len] = COMPART_KEYWORD;
    len++;

    /* buff_tx="FILES " */
    buff_tx[len] = '1';
    len++;
    buff_tx[len] = COMPART_KEYWORD;
    len++;

    /* buff_tx="FILES 1 " */
    Value_len = os_str_len(Value);
    addr = delete_space(Value, &Value_len);
    if (addr == NULL) {
        ps_print_err("addr is NULL, Value[%s] Value_len[%d]", Value, Value_len);
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

    /* buff_tx="FILES 1 0xXXXX " */
    /* ���͵�ַ */
    l_ret = msg_send_and_recv_except(buff_tx, len, MSG_FROM_DEV_READY_OK);
    if (l_ret < 0) {
        ps_print_err("SEND [%s] addr error\n", Key);
        return -EFAIL;
    }

    /* ��ȡbfgxУ׼���� */
    l_ret = get_bfgx_cali_data(g_firmware_down_buf, &len, g_firmware_down_buf_len);
    if (l_ret < 0) {
        ps_print_err("get bfgx cali data failed, len=%d\n", len);
        return -EFAIL;
    }

    /* Wait at least 5 ms */
    oal_usleep_range(FILE_CMD_WAIT_TIME_MIN, FILE_CMD_WAIT_TIME_MAX);

    /* ����bfgxУ׼���� */
    l_ret = msg_send_and_recv_except(g_firmware_down_buf, len, MSG_FROM_DEV_FILES_OK);
    if (l_ret < 0) {
        ps_print_err("send bfgx cali data fail\n");
        return -EFAIL;
    }

    return SUCC;
}

/*
 * �� �� ��  : download_dcxo_cali_data
 * ��������  : ʹ��files�������dcxo��У׼����
 * �� �� ֵ  : -1��ʾʧ�ܣ�0��ʾ�ɹ�
 */
STATIC int32 download_dcxo_cali_data(uint8 *key, uint8 *value)
{
    int32 l_ret;
    uint32 len;
    uint32 value_len;
    uint8 *addr = NULL;
    uint8 *dcxo_buffer_addr = NULL;
    uint8 buff_tx[SEND_BUF_LEN];

    dcxo_buffer_addr = (uint8 *)get_dcxo_data_buf_addr();
    if (dcxo_buffer_addr == NULL) {
        ps_print_err("[dcxo] buffer alloc error");
        return -EFAIL;
    }

    /* �������Value�ַ��������Valueֻ��һ����ַ����ʽΪ"0xXXXXX" */
    /* ����Ժ����ʽΪ"FILES �ļ����� Ҫд�ĵ�ַ"---"FILES 1 0xXXXX " */
    memset_s(buff_tx, sizeof(buff_tx), 0, SEND_BUF_LEN);

    /* buff_tx="" */
    len = os_str_len(key);
    if (memcpy_s(buff_tx, sizeof(buff_tx), key, len) != EOK) {
        ps_print_err("memcpy_s faild");
        return -EFAIL;
    }
    buff_tx[len] = COMPART_KEYWORD;
    len++;

    /* buff_tx="FILES " */
    buff_tx[len] = '1';
    len++;
    buff_tx[len] = COMPART_KEYWORD;
    len++;

    /* buff_tx="FILES 1 " */
    value_len = os_str_len(value);
    addr = delete_space(value, &value_len);
    if (addr == NULL) {
        ps_print_err("addr is NULL, Value[%s] Value_len[%d]", value, value_len);
        return -EFAIL;
    }

    if (memcpy_s(&buff_tx[len], sizeof(buff_tx) - len, addr, value_len) != EOK) {
        ps_print_err("memcpy_s faild");
        return -EFAIL;
    }
    len += value_len;
    buff_tx[len] = COMPART_KEYWORD;
    len++;

    /* buff_tx="FILES 1 0xXXXX " */
    /* ���͵�ַ */
    l_ret = msg_send_and_recv_except(buff_tx, len, MSG_FROM_DEV_READY_OK);
    if (l_ret < 0) {
        ps_print_err("SEND [%s] addr error\n", key);
        return -EFAIL;
    }

    oal_usleep_range(FILE_CMD_WAIT_TIME_MIN, FILE_CMD_WAIT_TIME_MAX);

    /* ����dcxoУ׼���� */
    l_ret = msg_send_and_recv_except(dcxo_buffer_addr, DCXO_CALI_DATA_BUF_LEN, MSG_FROM_DEV_FILES_OK);
    if (l_ret < 0) {
        ps_print_err("send bfgx cali data fail\n");
        return -EFAIL;
    }

    return SUCC;
}

/*
 * �� �� ��  : parse_file_cmd
 * ��������  : ����file�������
 * �������  : string   : file����Ĳ���
 *             addr     : ���͵����ݵ�ַ
 *             file_path: �����ļ���·��
 * �� �� ֵ  : -1��ʾʧ�ܣ�0��ʾ�ɹ�
 */
STATIC int32 parse_file_cmd(uint8 *string, unsigned long *addr, int8 **file_path)
{
    uint8 *tmp = NULL;
    int32 count = 0;
    int8 *after = NULL;

    if (string == NULL || addr == NULL || file_path == NULL) {
        ps_print_err("param is error!\n");
        return -EFAIL;
    }

    /* ��÷��͵��ļ��ĸ������˴�����Ϊ1��string�ַ����ĸ�ʽ������"1,0xXXXXX,file_path" */
    tmp = string;
    while (*tmp == COMPART_KEYWORD) {
        tmp++;
    }
    string_to_num(tmp, &count);
    if (count != FILE_COUNT_PER_SEND) {
        ps_print_err("the count of send file must be 1, count = [%d]\n", count);
        return -EFAIL;
    }

    /* ��tmpָ���ַ������ĸ */
    tmp = os_str_chr(string, ',');
    if (tmp == NULL) {
        ps_print_err("param string is err!\n");
        return -EFAIL;
    } else {
        tmp++;
        while (*tmp == COMPART_KEYWORD) {
            tmp++;
        }
    }

    *addr = simple_strtoul(tmp, &after, 16); /* ���ַ���ת����16������ */

    ps_print_dbg("file to send addr:[0x%lx]\n", *addr);

    /* "1,0xXXXX,file_path"
     *         ^
     *       after
     */
    while (*after == COMPART_KEYWORD) {
        after++;
    }
    /* ����','�ַ� */
    after++;
    while (*after == COMPART_KEYWORD) {
        after++;
    }

    ps_print_dbg("after:[%s]\n", after);

    *file_path = after;

    return SUCC;
}

STATIC void oal_print_wcpu_reg(oal_uint32 *pst_buf, oal_uint32 ul_size)
{
    oal_int32 i = 0;
    oal_int32 remain = (oal_int32)ul_size; /* per dword */
    if (ul_size) {
        oam_error_log0(0, OAM_SF_ANY, "print wcpu registers:");
    }

    forever_loop() {
        if (remain >= 4) { /* ����Ҫ��ӡ��32bit�Ĵ����������ڵ���4ʱ��һ���Դ�ӡ4�� */
            oam_error_log4(0, OAM_SF_ANY, "wcpu_reg: %x %x %x %x",
                           *(pst_buf + i + 0), *(pst_buf + i + 1),
                           *(pst_buf + i + 2), *(pst_buf + i + 3)); /* ָ������ƶ�2,3 */
            i += 4; /* ��ӡ����+4 */
            remain -= 4; /* �Ĵ�������-4 */
        } else if (remain >= 3) { /* ����Ҫ��ӡ��32bit�Ĵ�����������3ʱ��һ���Դ�ӡ3�� */
            oam_error_log3(0, OAM_SF_ANY, "wcpu_reg: %x %x %x",
                           *(pst_buf + i + 0), *(pst_buf + i + 1),
                           *(pst_buf + i + 2)); /* ָ������ƶ�2 */
            i += 3; /* ��ӡ����+3 */
            remain -= 3; /* �Ĵ�������-3 */
        } else if (remain >= 2) { /* ����Ҫ��ӡ��32bit�Ĵ�����������2ʱ��һ���Դ�ӡ2�� */
            oam_error_log2(0, OAM_SF_ANY, "wcpu_reg: %x %x",
                           *(pst_buf + i + 0), *(pst_buf + i + 1));
            i += 2; /* ��ӡ����+2 */
            remain -= 2; /* �Ĵ�������-2 */
        } else if (remain >= 1) { /* ����Ҫ��ӡ��32bit�Ĵ�����������1ʱ��һ���Դ�ӡ1�� */
            oam_error_log1(0, OAM_SF_ANY, "wcpu_reg: %x",
                           *(pst_buf + i + 0));
            i += 1; /* ��ӡ����+1 */
            remain -= 1; /* �Ĵ�������-1 */
        } else {
            break;
        }
    }
}

#define READ_DEVICE_MAX_BUF_SIZE 128
/* read device reg by bootloader */
int32 read_device_reg16(uint32 address, uint16 *value)
{
    int32 ret, buf_len;
    const uint32 ul_read_msg_len = 4;
    const uint32 ul_dump_len = 8;
    uint8 buf_tx[READ_DEVICE_MAX_BUF_SIZE];
    uint8 buf_result[READ_DEVICE_MAX_BUF_SIZE];
    void *addr = (void *)buf_result;

    memset_s(buf_tx, READ_DEVICE_MAX_BUF_SIZE, 0, READ_DEVICE_MAX_BUF_SIZE);
    memset_s(buf_result, READ_DEVICE_MAX_BUF_SIZE, 0, READ_DEVICE_MAX_BUF_SIZE);

    buf_len = snprintf_s(buf_tx, sizeof(buf_tx), sizeof(buf_tx) - 1, "%s%c0x%x%c%d%c",
                         RMEM_CMD_KEYWORD,
                         COMPART_KEYWORD,
                         address,
                         COMPART_KEYWORD,
                         4, /* 4��ʾ���� */
                         COMPART_KEYWORD); /* ��� READM 0x... 4 ���������4��ʾ���� */
    if (buf_len < 0) {
        ps_print_err("log str format err line[%d]\n", __LINE__);
        return buf_len;
    }

    ps_print_info("%s", buf_tx);

    ret = send_msg(buf_tx, buf_len);
    if (ret < 0) {
        ps_print_err("send msg [%s] failed, ret=%d", buf_tx, ret);
        return ret;
    }

    ret = read_msg(buf_result, ul_read_msg_len);
    if (ret > 0) {
        /* �����ض����ڴ�,����С��ֱ��ת�� */
        *value = (uint16)oal_readl(addr);
        oal_print_hex_dump(buf_result, ul_dump_len, HEX_DUMP_GROUP_SIZE, "reg16: ");
        return 0;
    }

    ps_print_err("read_device_reg16 failed, ret=%d", ret);

    return -1;
}

/* write device regs by bootloader */
int32 write_device_reg16(uint32 address, uint16 value)
{
    int32 ret, buf_len;
    uint8 buf_tx[READ_DEVICE_MAX_BUF_SIZE];

    memset_s(buf_tx, sizeof(buf_tx), 0, sizeof(buf_tx));

    buf_len = snprintf_s(buf_tx, sizeof(buf_tx), sizeof(buf_tx) - 1, "%s%c2%c0x%x%c%d%c",
                         WMEM_CMD_KEYWORD,
                         COMPART_KEYWORD,
                         COMPART_KEYWORD,
                         address,
                         COMPART_KEYWORD,
                         value,
                         COMPART_KEYWORD);
    if (buf_len < 0) {
        ps_print_err("log str format err line[%d]\n", __LINE__);
        return buf_len;
    }

    ps_print_info("%s", buf_tx);

    ret = send_msg(buf_tx, buf_len);
    if (ret < 0) {
        ps_print_err("send msg [%s] failed, ret=%d", buf_tx, ret);
        return ret;
    }

    ret = recv_expect_result(MSG_FROM_DEV_WRITEM_OK);
    if (ret < 0) {
        ps_print_err("send msg [%s] recv failed, ret=%d", buf_tx, ret);
        return ret;
    }

    return 0;
}

STATIC int32 recv_device_memdump(uint8 *pucDataBuf, int32 len)
{
    int32 l_ret = -EFAIL;
    uint8 retry = 3;
    int32 lenbuf = 0;

    if (pucDataBuf == NULL) {
        ps_print_err("pucDataBuf is NULL\n");
        return -EFAIL;
    }

    ps_print_dbg("expect recv len is [%d]\n", len);

    while (len > lenbuf) {
        l_ret = read_msg(pucDataBuf + lenbuf, len - lenbuf);
        if (l_ret > 0) {
            lenbuf += l_ret;
        } else {
            retry--;
            lenbuf = 0;
            if (retry == 0) {
                l_ret = -EFAIL;
                ps_print_err("time out\n");
                break;
            }
        }
    }

    if (len <= lenbuf) {
        wifi_memdump_enquenue(pucDataBuf, len);
    }

    return l_ret;
}

static int32 sdio_read_device_mem(struct st_wifi_dump_mem_info *pst_mem_dump_info,
                                  uint8 *pucDataBuf, uint32 ulDataBufLen)
{
    uint8 buf_tx[SEND_BUF_LEN];
    int32 ret = 0;
    uint32 size = 0;
    uint32 offset;
    uint32 remainder = pst_mem_dump_info->size;

    offset = 0;
    while (remainder > 0) {
        memset_s(buf_tx, SEND_BUF_LEN, 0, SEND_BUF_LEN);

        size = min(remainder, ulDataBufLen);
        ret = snprintf_s(buf_tx, sizeof(buf_tx), sizeof(buf_tx) - 1, "%s%c0x%lx%c%d%c",
                         RMEM_CMD_KEYWORD,
                         COMPART_KEYWORD,
                         pst_mem_dump_info->mem_addr + offset,
                         COMPART_KEYWORD,
                         size,
                         COMPART_KEYWORD);
        if (ret < 0) {
            ps_print_err("log str format err line[%d]\n", __LINE__);
            break;
        }
        ps_print_dbg("read mem cmd:[%s]\n", buf_tx);
        send_msg(buf_tx, os_str_len(buf_tx));

        ret = recv_device_memdump(pucDataBuf, size);
        if (ret < 0) {
            ps_print_err("wifi mem dump fail, filename is [%s],ret=%d\n", pst_mem_dump_info->file_name, ret);
            break;
        }

#ifdef CONFIG_PRINTK
        if (offset == 0) {
            oal_int8 *pst_file_name = (pst_mem_dump_info->file_name ?
                                       ((oal_int8 *)pst_mem_dump_info->file_name) : (oal_int8 *)"default: ");
            if (!oal_strcmp("wifi_device_panic_mem", pst_file_name)) {
                if (size > CPU_PANIC_MEMDUMP_SIZE) {
                    oal_print_hex_dump(pucDataBuf, CPU_PANIC_MEMDUMP_SIZE, HEX_DUMP_GROUP_SIZE, pst_file_name);
                    /* ��Ĵ��������4�ֽڣ������������Ҫ��4 */
                    oal_print_wcpu_reg((oal_uint32 *)(pucDataBuf), CPU_PANIC_MEMDUMP_SIZE / 4);
                }
            }
        }
#endif

        offset += size;

        remainder -= size;
    }

    return ret;
}

/*
 * �� �� ��  : wifi_device_mem_dump
 * ��������  : firmware����ʱ��ȡwifi���ڴ�
 * �������  : pst_mem_dump_info  : ��Ҫ��ȡ���ڴ���Ϣ
 *             count              : ��Ҫ��ȡ���ڴ�����
 * �� �� ֵ  : -1��ʾʧ�ܣ�0��ʾ�ɹ�
 */
int32 wifi_device_mem_dump(struct st_wifi_dump_mem_info *pst_mem_dump_info, uint32 count)
{
    int32 ret = -EFAIL;
    uint32 i;
    uint8 *pucDataBuf = NULL;
    const uint32 ul_buff_size = 100;
    uint8 buff[ul_buff_size];
    uint32 *pcount = (uint32 *)&buff[0];
    uint32 sdio_transfer_limit = hcc_get_max_trans_size(hcc_get_110x_handler());

    if (!ps_is_device_log_enable()) {
        return 0;
    }

    sdio_transfer_limit = oal_min(PAGE_SIZE, sdio_transfer_limit);

    if (pst_mem_dump_info == NULL) {
        ps_print_err("pst_wifi_dump_info is NULL\n");
        return -EFAIL;
    }

    do {
        ps_print_info("try to malloc mem dump buf len is [%d]\n", sdio_transfer_limit);
        pucDataBuf = (uint8 *)os_kmalloc_gfp(sdio_transfer_limit);
        if (pucDataBuf == NULL) {
            ps_print_warning("malloc mem  len [%d] fail, continue to try in a smaller size\n", sdio_transfer_limit);
            sdio_transfer_limit = sdio_transfer_limit >> 1;
        }
    } while ((pucDataBuf == NULL) && (sdio_transfer_limit >= MIN_FIRMWARE_FILE_TX_BUF_LEN));

    if (pucDataBuf == NULL) {
        ps_print_err("pucDataBuf KMALLOC failed\n");
        return -EFAIL;
    }

    ps_print_info("mem dump data buf len is [%d]\n", sdio_transfer_limit);

    wifi_notice_hal_memdump();

    for (i = 0; i < count; i++) {
        *pcount = pst_mem_dump_info[i].size;
        ps_print_info("mem dump data size [%d]==> [%d]\n", *pcount, pst_mem_dump_info[i].size);
        wifi_memdump_enquenue(buff, 4); /* ��������sk_buff�Ĵ�СΪ4 */
        ret = sdio_read_device_mem(&pst_mem_dump_info[i], pucDataBuf, sdio_transfer_limit);
        if (ret < 0) {
            break;
        }
    }
    wifi_memdump_finish();

    os_mem_kfree(pucDataBuf);

    return ret;
}

STATIC int32 sdio_read_mem(uint8 *key, uint8 *value, bool is_wifi)
{
    int32 l_ret;
    uint32 size, readlen;
    int32 retry = 3;
    uint8 *flag;
    OS_KERNEL_FILE_STRU *fp = NULL;
    uint8 *pucDataBuf = NULL;
    uint32 sdio_transfer_limit = hcc_get_max_trans_size(hcc_get_110x_handler());

    /* ���ڴ��ȿ��ǳɹ���,ҳ��С������ڴ���������ɹ��� */
    sdio_transfer_limit = oal_min(PAGE_SIZE, sdio_transfer_limit);

    flag = os_str_chr(value, ',');
    if (flag == NULL) {
        ps_print_err("RECV LEN ERROR..\n");
        return -EFAIL;
    }
    flag++;
    ps_print_dbg("recv len [%s]\n", flag);
    while (*flag == COMPART_KEYWORD) {
        flag++;
    }

    string_to_num(flag, &size);

    do {
        ps_print_info("try to malloc sdio mem read buf len is [%d]\n", sdio_transfer_limit);
        pucDataBuf = (uint8 *)os_kmalloc_gfp(sdio_transfer_limit);
        if (pucDataBuf == NULL) {
            ps_print_warning("malloc mem len [%d] fail, continue to try in a smaller size\n", sdio_transfer_limit);
            sdio_transfer_limit = sdio_transfer_limit >> 1;
        }
    } while ((pucDataBuf == NULL) && (sdio_transfer_limit >= MIN_FIRMWARE_FILE_TX_BUF_LEN));

    if (pucDataBuf == NULL) {
        ps_print_err("pucDataBuf KMALLOC failed\n");
        return -EFAIL;
    }

    fp = open_file_to_readm(is_wifi == true ? WIFI_DUMP_PATH "/readm_wifi" : WIFI_DUMP_PATH "/readm_bfgx");
    if (IS_ERR(fp)) {
        ps_print_err("create file error,fp = 0x%p\n", fp);
        os_mem_kfree(pucDataBuf);
        return SUCC;
    }

    l_ret = number_type_cmd_send(key, value);
    if (l_ret < 0) {
        ps_print_err("send %s,%s fail \n", key, value);
        oal_file_close(fp);
        os_mem_kfree(pucDataBuf);
        return l_ret;
    }

    ps_print_dbg("recv len [%d]\n", size);
    while (size > 0) {
        readlen = min(size, sdio_transfer_limit);
        l_ret = recv_device_mem(fp, pucDataBuf, readlen);
        if (l_ret > 0) {
            size -= l_ret;
        } else {
            ps_print_err("read error retry:%d\n", retry);
            --retry;
            if (!retry) {
                ps_print_err("retry fail\n");
                break;
            }
        }
    }

    oal_file_close(fp);
    os_mem_kfree(pucDataBuf);

    return l_ret;
}

/*
 * �� �� ��  : exec_number_wmem_cmd
 * ��������  : дw mem���ڴ��ַ
 * �� �� ֵ  : 0��ʾ�ɹ�������ֵ��ʾʧ��
 */
STATIC int32 exec_number_wmem_cmd(uint8 *key, uint8 *value)
{
    int32 l_ret;

    if ((os_str_str((int8 *)value, (int8 *)STR_REG_PMU_CLK_REQ) != NULL) && pmu_clk_request_enable()) {
        ps_print_info("hi110x PMU clk request\n");
        return SUCC;
    }

    l_ret = number_type_cmd_send(key, value);
    if (l_ret < 0) {
        ps_print_err("send key=[%s],value=[%s] fail\n", key, value);
        return l_ret;
    }

    l_ret = recv_expect_result(MSG_FROM_DEV_WRITEM_OK);
    if (l_ret < 0) {
        ps_print_err("recv expect result fail!\n");
        return l_ret;
    }

    return SUCC;
}

/*
 * �� �� ��  : exec_number_sleep_cmd
 * ��������  : host sleep��ʱ
 * �� �� ֵ  : 0��ʾ�ɹ�������ֵ��ʾʧ��
 */
STATIC int32 exec_number_sleep_cmd(uint8 *key, uint8 *value)
{
    int32 delay_ms;

    delay_ms = simple_strtol(value, NULL, 10); /* ���ַ���ת����10������ */
    ps_print_info("firmware download delay %d ms\n", delay_ms);
    // ���ȴ�ʱ��5s����ֹcfg����ʱ��̫�����¼��س�ʱ
    if (delay_ms > 0 && delay_ms < 5000) { /* ��ʱ��ʱ����0��5000ms֮�� */
        msleep(delay_ms);
    } else {
        msleep(5); /* delay 5ms */
    }

    return SUCC;
}

/*
 * �� �� ��  : exec_number_cali_count_cmd
 * ��������  : ����У׼������device
 * �� �� ֵ  : 0��ʾ�ɹ�������ֵ��ʾʧ��
 */
STATIC int32 exec_number_cali_count_cmd(uint8 *key, uint8 *value)
{
    int32 ret;

    ret = update_device_cali_count(key, value);
    if (ret < 0) {
        ps_print_err("update device cali count fail\n");
    }
    return ret;
}

/*
 * �� �� ��  : exec_number_cali_bfgx_data
 * ��������  : ����bfgxУ׼����
 * �� �� ֵ  : 0��ʾ�ɹ�������ֵ��ʾʧ��
 */
STATIC int32 exec_number_cali_bfgx_data(uint8 *key, uint8 *value)
{
    int32 ret;

    if (oal_atomic_read(&g_ir_only_mode) != 0) {
        ps_print_info("ir only pass the download cali data cmd\n");
        return SUCC;
    }

    /* ����BFGX��У׼���� */
    ret = download_bfgx_cali_data(FILES_CMD_KEYWORD, value);
    if (ret < 0) {
        ps_print_err("download bfgx cali data fail\n");
    }

    return ret;
}

/*
 * �� �� ��  : exec_number_cali_bfgx_data
 * ��������  : ���ع�ʱ����ص�����
 * �� �� ֵ  : 0��ʾ�ɹ�������ֵ��ʾʧ��
 */
STATIC int32 exec_number_cali_dcxo_data(uint8 *key, uint8 *value)
{
    int32 ret;
    ps_print_dbg("download dcxo cali data begin\n");
    /* ����TCXO ��У׼������devcie */
    ret = download_dcxo_cali_data(FILES_CMD_KEYWORD, value);
    if (ret < 0) {
        ps_print_err("download dcxo cali data fail\n");
    }

    return ret;
}

/*
 * �� �� ��  : exec_number_jump_cmd
 * ��������  : ִ��deviceָ����ַ�ĺ���
 * �� �� ֵ  : 0��ʾ�ɹ�������ֵ��ʾʧ��
 */
STATIC int32 exec_number_jump_cmd(uint8 *key, uint8 *value)
{
    int32 ret;
    ret = number_type_cmd_send(key, value);
    if (ret < 0) {
        ps_print_err("send key=[%s],value=[%s] fail\n", key, value);
        return ret;
    }

    /* 100000ms timeout */
    ret = recv_expect_result_timeout(MSG_FROM_DEV_JUMP_OK, READ_MEG_JUMP_TIMEOUT);
    if (ret >= 0) {
        ps_print_info("JUMP success!\n");
    } else {
        ps_print_err("CMD JUMP timeout! l_ret=%d\n", ret);
    }

    return ret;
}

/*
 * �� �� ��  : exec_number_set_cmd
 * ��������  : ִ�����ò���
 * �� �� ֵ  : 0��ʾ�ɹ�������ֵ��ʾʧ��
 */
STATIC int32 exec_number_set_cmd(uint8 *key, uint8 *value)
{
    int32 ret;
    ret = number_type_cmd_send(key, value);
    if (ret < 0) {
        ps_print_err("send key=[%s],value=[%s] fail\n", key, value);
        return ret;
    }

    ret = recv_expect_result(MSG_FROM_DEV_SET_OK);
    if (ret < 0) {
        ps_print_err("recv expect result fail!\n");
        return ret;
    }

    return SUCC;
}

/*
 * �� �� ��  : exec_number_rmem_cmd
 * ��������  : ��device���ڴ�
 * �� �� ֵ  : 0��ʾ�ɹ�������ֵ��ʾʧ��
 */
STATIC int32 exec_number_rmem_cmd(uint8 *key, uint8 *value)
{
    return sdio_read_mem(key, value, true);
}

/*
 * �� �� ��  : exec_number_type_cmd
 * ��������  : ִ��number���͵�����
 * �������  : Key  : ����Ĺؼ���
 *             Value: ����Ĳ���
 * �� �� ֵ  : -1��ʾʧ�ܣ�0��ʾ�ɹ�
 */
STATIC int32 exec_number_type_cmd(uint8 *Key, uint8 *Value)
{
    int32 ret = -EFAIL;
    int32 loop;

    if (!os_mem_cmp(Key, VER_CMD_KEYWORD, os_str_len(VER_CMD_KEYWORD))) {
        ret = check_version();
        if (ret < 0) {
            ps_print_err("check version FAIL [%d]\n", ret);
            return -EFAIL;
        }
    }

    for (loop = 0; loop < oal_array_size(g_number_type); loop++) {
        if (!os_str_cmp(Key, g_number_type[loop].key)) {
            ret = (g_number_type[loop].cmd_exec)(Key, Value);
            break;
        }
    }

    return ret;
}

/*
 * �� �� ��  : exec_quit_type_cmd
 * ��������  : ִ��quit���͵�����
 * �� �� ֵ  : -1��ʾʧ�ܣ�0��ʾ�ɹ�
 */
STATIC int32 exec_quit_type_cmd(void)
{
    int32 ret;
    int32 l_len;
    const uint32 ul_buf_len = 8;
    uint8 buf[ul_buf_len];

    memset_s(buf, sizeof(buf), 0, sizeof(buf));

    ret = memcpy_s(buf, sizeof(buf), (uint8 *)QUIT_CMD_KEYWORD, os_str_len(QUIT_CMD_KEYWORD));
    if (ret != EOK) {
        ps_print_err("buf not enough\n");
        return -EFAIL;
    }
    l_len = os_str_len(QUIT_CMD_KEYWORD);

    buf[l_len] = COMPART_KEYWORD;
    l_len++;

    ret = msg_send_and_recv_except(buf, l_len, MSG_FROM_DEV_QUIT_OK);

    return ret;
}

STATIC int32 file_open_get_len(int8 *path, OS_KERNEL_FILE_STRU **fp, uint32 *file_len)
{
#define RETRY_CONT 3
    mm_segment_t fs;
    int32 i = 0;

    fs = get_fs();
    set_fs(KERNEL_DS);
    for (i = 0; i < RETRY_CONT; i++) {
        *fp = filp_open(path, O_RDONLY, 0);
        if (oal_is_err_or_null(*fp)) {
            ps_print_err("filp_open [%s] fail!!, fp=%pK, errno:%ld\n", path, *fp, PTR_ERR(*fp));
            *fp = NULL;
            continue;
        }
    }

    if (*fp == NULL) {
        ps_print_err("filp_open fail!!\n");
        set_fs(fs);
        return -OAL_ENOPERMI;
    }

    /* ��ȡfile�ļ���С */
    *file_len = vfs_llseek(*fp, 0, SEEK_END);
    if (*file_len <= 0) {
        ps_print_err("file size of %s is 0!!\n", path);
        filp_close(*fp, NULL);
        set_fs(fs);
        return -EFAIL;
    }

    /* �ָ�fp->f_pos���ļ���ͷ */
    vfs_llseek(*fp, 0, SEEK_SET);
    set_fs(fs);

    return SUCC;
}

/*
 * �� �� ��  : exec_file_type_cmd
 * ��������  : ִ��file���͵�����
 * �������  : Key  : ����Ĺؼ���
 *             Value: ����Ĳ���
 * �� �� ֵ  : -1��ʾʧ�ܣ�0��ʾ�ɹ�
 */
STATIC int32 exec_file_type_cmd(uint8 *Key, uint8 *Value)
{
    unsigned long addr;
    unsigned long addr_send;
    int8 *path = NULL;
    int32 ret;
    uint32 file_len;
    uint32 transmit_limit;
    uint32 per_send_len;
    uint32 send_count;
    int32 rdlen;
    uint32 i;
    uint32 offset = 0;
    uint8 buff_tx[SEND_BUF_LEN] = {0};
    OS_KERNEL_FILE_STRU *fp = NULL;
    hi110x_board_info *board_info = NULL;

    board_info = get_hi110x_board_info();
    if (board_info == NULL) {
        ps_print_err("board_info is null!\n");
        return -EFAIL;
    }

    if (oal_atomic_read(&g_ir_only_mode) != 0) {
        ps_print_info("ir only pass the download file cmd\n");
        return SUCC;
    }

    ret = parse_file_cmd(Value, &addr, &path);
    if (ret < 0) {
        ps_print_err("parse file cmd fail!\n");
        return ret;
    }

    ps_print_info("download firmware:%s addr:0x%x\n", path, (uint32)addr);

    ret = file_open_get_len(path, &fp, &file_len);
    if (ret < 0) {
        return ret;
    }

    ps_print_dbg("file len is [%d]\n", file_len);

    transmit_limit = g_firmware_down_buf_len;  // DataBufLen_etc�ڵ���ǰ��֤Ϊ����0
    per_send_len = (transmit_limit > file_len) ? file_len : transmit_limit;
    send_count = (file_len + per_send_len - 1) / per_send_len;

    for (i = 0; i < send_count; i++) {
        rdlen = oal_file_read_ext(fp, fp->f_pos, g_firmware_down_buf, per_send_len);
        if (rdlen > 0) {
            ps_print_dbg("len of kernel_read is [%d], i=%d\n", rdlen, i);
            fp->f_pos += rdlen;
        } else {
            ps_print_err("len of kernel_read is error! ret=[%d], i=%d\n", rdlen, i);
            oal_file_close(fp);
            if (rdlen < 0) {
                return rdlen;
            } else {
                return -EFAIL;
            }
        }

        addr_send = addr + offset;
        ps_print_dbg("send addr is [0x%lx], i=%d\n", addr_send, i);
        ret = snprintf_s(buff_tx, sizeof(buff_tx), sizeof(buff_tx) - 1, "%s%c%d%c0x%lx%c",
                         FILES_CMD_KEYWORD,
                         COMPART_KEYWORD,
                         FILE_COUNT_PER_SEND,
                         COMPART_KEYWORD,
                         addr_send,
                         COMPART_KEYWORD);
        if (ret < 0) {
            ps_print_err("log str format err line[%d]\n", __LINE__);
            break;
        }
        /* ���͵�ַ */
        ps_print_dbg("send file addr cmd is [%s]\n", buff_tx);
        ret = msg_send_and_recv_except(buff_tx, os_str_len(buff_tx), MSG_FROM_DEV_READY_OK);
        if (ret < 0) {
            ps_print_err("SEND [%s] error\n", buff_tx);
            oal_file_close(fp);
            return -EFAIL;
        }

        /* Wait at least 5 ms */
        oal_usleep_range(FILE_CMD_WAIT_TIME_MIN, FILE_CMD_WAIT_TIME_MAX);

        /* �����ļ����� */
        ret = msg_send_and_recv_except(g_firmware_down_buf, rdlen, MSG_FROM_DEV_FILES_OK);
        if (ret < 0) {
            ps_print_err(" sdio send data fail\n");
            oal_file_close(fp);
            return -EFAIL;
        }
        offset += rdlen;
    }

    oal_file_close(fp);

    /* ���͵ĳ���Ҫ���ļ��ĳ���һ�� */
    if (offset != file_len) {
        ps_print_err("file send len is err! send len is [%d], file len is [%d]\n", offset, file_len);
        return -EFAIL;
    }

    return SUCC;
}

/*
 * �� �� ��  : exec_shutdown_type_cmd
 * ��������  : ִ��shutdown cpu type������
 * �������  : which_cpu: Ҫ�رյ�cpu
 * �� �� ֵ  : -1��ʾʧ�ܣ��Ǹ�����ʾ�ɹ�
 */
STATIC int32 exec_shutdown_type_cmd(uint32 which_cpu)
{
    int32 ret = -EFAIL;
    uint8 Value_SHUTDOWN[SHUTDOWN_TX_CMD_LEN];

    if (which_cpu == DEV_WCPU) {
        ret = snprintf_s(Value_SHUTDOWN, sizeof(Value_SHUTDOWN), sizeof(Value_SHUTDOWN) - 1, "%d,%s,%d",
                         HOST_TO_DEVICE_CMD_HEAD, SOFT_WCPU_EN_ADDR, 0);
        if (ret < 0) {
            ps_print_err("log str format err line[%d]\n", __LINE__);
            return ret;
        }

        ret = number_type_cmd_send(WMEM_CMD_KEYWORD, Value_SHUTDOWN);
        if (ret < 0) {
            ps_print_err("send key=[%s],value=[%s] fail\n", SHUTDOWN_WIFI_CMD_KEYWORD, Value_SHUTDOWN);
            return ret;
        }
    } else if (which_cpu == DEV_BCPU) {
        ret = snprintf_s(Value_SHUTDOWN, sizeof(Value_SHUTDOWN), sizeof(Value_SHUTDOWN) - 1, "%d,%s,%d",
                         HOST_TO_DEVICE_CMD_HEAD, SOFT_BCPU_EN_ADDR, 0);
        if (ret < 0) {
            ps_print_err("log str format err line[%d]\n", __LINE__);
            return ret;
        }

        ret = number_type_cmd_send(WMEM_CMD_KEYWORD, Value_SHUTDOWN);
        if (ret < 0) {
            ps_print_err("send key=[%s],value=[%s] fail\n", SHUTDOWN_BFGX_CMD_KEYWORD, Value_SHUTDOWN);
            return ret;
        }

        ret = recv_expect_result(MSG_FROM_DEV_WRITEM_OK);
        if (ret < 0) {
            ps_print_err("recv expect result fail!\n");
            return ret;
        }

        ret = snprintf_s(Value_SHUTDOWN, sizeof(Value_SHUTDOWN), sizeof(Value_SHUTDOWN) - 1, "%d,%s,%d",
                         HOST_TO_DEVICE_CMD_HEAD, BCPU_DE_RESET_ADDR, 1);
        if (ret < 0) {
            ps_print_err("log str format err line[%d]\n", __LINE__);
            return ret;
        }

        ret = number_type_cmd_send(WMEM_CMD_KEYWORD, Value_SHUTDOWN);
        if (ret < 0) {
            ps_print_err("send key=[%s],value=[%s] fail\n", SHUTDOWN_BFGX_CMD_KEYWORD, Value_SHUTDOWN);
            return ret;
        }

        ret = recv_expect_result(MSG_FROM_DEV_WRITEM_OK);
        if (ret < 0) {
            ps_print_err("recv expect result fail!\n");
            return ret;
        }
    } else {
        ps_print_err("para is error, which_cpu=[%d]\n", which_cpu);
        return -EFAIL;
    }

    return SUCC;
}

/*
 * �� �� ��  : execute_download_cmd
 * ��������  : ִ��firmware download������
 * �������  : cmd_type: �������������
 *             cmd_name: ����Ĺؼ���
 *             cmd_para: ����Ĳ���
 * �� �� ֵ  : -1��ʾʧ�ܣ��Ǹ�����ʾ�ɹ�
 */
STATIC int32 execute_download_cmd(int32 cmd_type, uint8 *cmd_name, uint8 *cmd_para)
{
    int32 ret;

    switch (cmd_type) {
        case FILE_TYPE_CMD:
            ps_print_dbg(" command type FILE_TYPE_CMD\n");
            ret = exec_file_type_cmd(cmd_name, cmd_para);
            break;
        case NUM_TYPE_CMD:
            ps_print_dbg(" command type NUM_TYPE_CMD\n");
            ret = exec_number_type_cmd(cmd_name, cmd_para);
            break;
        case QUIT_TYPE_CMD:
            ps_print_dbg(" command type QUIT_TYPE_CMD\n");
            ret = exec_quit_type_cmd();
            break;
        case SHUTDOWN_WIFI_TYPE_CMD:
            ps_print_dbg(" command type SHUTDOWN_WIFI_TYPE_CMD\n");
            ret = exec_shutdown_type_cmd(DEV_WCPU);
            break;
        case SHUTDOWN_BFGX_TYPE_CMD:
            ps_print_dbg(" command type SHUTDOWN_BFGX_TYPE_CMD\n");
            ret = exec_shutdown_type_cmd(DEV_BCPU);
            break;

        default:
            ps_print_err("command type error[%d]\n", cmd_type);
            ret = -EFAIL;
            break;
    }

    return ret;
}

/*
 * �� �� ��  : firmware_read_cfg
 * ��������  : ��ȡcfg�ļ������ݣ��ŵ�������̬�����buffer��
 * �������  : puc_CfgPatch    : cfg�ļ���·��
 *             puc_read_buffer : ����cfg�ļ����ݵ�buffer
 * �� �� ֵ  : 0��ʾ�ɹ���-1��ʾʧ��
 */
int32 firmware_read_cfg(const char *puc_CfgPatch, uint8 *puc_read_buffer)
{
    OS_KERNEL_FILE_STRU *fp = NULL;
    int32 ret;
    mm_segment_t fs;

    if ((puc_CfgPatch == NULL) || (puc_read_buffer == NULL)) {
        ps_print_err("para is NULL\n");
        return -EFAIL;
    }

    fs = get_fs();
    set_fs(KERNEL_DS);
    fp = filp_open(puc_CfgPatch, O_RDONLY, 0);
    if (oal_is_err_or_null(fp)) {
        ps_print_err("open file %s fail, fp=%p, error %ld\n", puc_CfgPatch, fp, PTR_ERR(fp));
        set_fs(fs);
        fp = NULL;
        return -EFAIL;
    }

    memset_s(puc_read_buffer, READ_CFG_BUF_LEN, 0, READ_CFG_BUF_LEN);

    ret = oal_file_read_ext(fp, fp->f_pos, puc_read_buffer, READ_CFG_BUF_LEN);

    filp_close(fp, NULL);
    set_fs(fs);
    fp = NULL;

    return ret;
}


/*
 * �� �� ��  : firmware_parse_special_cmd
 * ��������  : ����cfg�ļ��еĲ���"="�ַ�������������:
               QUIT
               SHUTDOWN_WIFI
               SHUTDOWN_BFGX
               BUCK_MODE
 * �� �� ֵ  : ��������������,�������������;���򷵻�-1.
 */
STATIC int32 firmware_parse_special_cmd(uint8 *puc_cfg_buffer, uint8 *puc_cmd_name, uint32 cmd_name_len,
                                        uint8 *puc_cmd_para, uint32 cmd_para_len)
{
#define MAX_BUFF_LEN    100
#define BUCK_PARA_LEN   16

    int32 ret;

    /* �˳������� */
    if (os_str_str((int8 *)puc_cfg_buffer, QUIT_CMD_KEYWORD) != NULL) {
        return QUIT_TYPE_CMD;
    } else if (os_str_str((int8 *)puc_cfg_buffer, SHUTDOWN_WIFI_CMD_KEYWORD) != NULL) {
        return SHUTDOWN_WIFI_TYPE_CMD;
    } else if (os_str_str((int8 *)puc_cfg_buffer, SHUTDOWN_BFGX_CMD_KEYWORD) != NULL) {
        return SHUTDOWN_BFGX_TYPE_CMD;
    } else if (os_str_str((int8 *)puc_cfg_buffer, BUCK_MODE_CMD_KEYWORD) != NULL) {
        /*
         * 1105֧������buck������buck,CFG�ļ��н��йؼ��֣�ʵ��ֵ��ini�ļ��л�ȡ�����㶨�ƻ���
         * ini���ƻ���ʽ[buck_mode=2,0x50001850,value],����CFG�ļ���WRITEM���﷨,0x50001850Ϊ1105��BUCK_CUSTOM_REG
         * ��ַ, value����ʵ��Ҫ������:
         * ��1��   BIT[15,14]����ʾ�Ƿ��������buck
         *      2'b00:  ȫ����buck
         *      2'b01:  I2C���ƶ�������buck
         *      2'b10:  GPIO���ƶ�������buck
         *      2'b11:  host���ƹ�������buck��ѹ
         * ��2��   BIT[8]����ʾbuck_en,�ٵ���ѹ..
         * ��3��   BIT[7��0]: ����ͬ��Buck�����ͺ�
         */
        char buff[MAX_BUFF_LEN];
        char buck_para[BUCK_PARA_LEN];
        ps_print_info("buck_mode para,get from ini file\n");
        if (INI_FAILED == get_cust_conf_string(INI_MODU_PLAT, "buck_custom_addr", buff, sizeof(buff) - 1)) {
            ps_print_err("can't found buck_mode ini");
            return ERROR_TYPE_CMD;
        }

        oal_itoa(g_st_board_info.buck_param, buck_para, OAL_SIZEOF(buck_para));

        ret = strcat_s(buff, OAL_SIZEOF(buff), ",");
        if (ret != EOK) {
            ps_print_err("strcat_s failed!");
            return ERROR_TYPE_CMD;
        }
        ret = strcat_s(buff, OAL_SIZEOF(buff), buck_para);
        if (ret != EOK) {
            ps_print_err("strcat_s buck_para failed!");
            return ERROR_TYPE_CMD;
        }

        ret = memcpy_s(puc_cmd_para, cmd_para_len, buff, OAL_STRLEN(buff));
        if (ret != EOK) {
            ps_print_err("buck_mode para set error");
            return ERROR_TYPE_CMD;
        }

        ret = memcpy_s(puc_cmd_name, cmd_name_len, "WRITEM", 6); /* ��Ҫ�����ĸ���Ϊ6 */
        if (ret != EOK) {
            ps_print_err("buck_mode cmd name set error");
            return ERROR_TYPE_CMD;
        }

        return NUM_TYPE_CMD;
    }

    return ERROR_TYPE_CMD;
}

/*
 * �� �� ��  : firmware_parse_cmd
 * ��������  : ����cfg�ļ��е�����
 * �������  : puc_cfg_buffer: ����cfg�ļ����ݵ�buffer
 *             puc_cmd_name  : ��������Ժ�����ؼ��ֵ�buffer
 *             puc_cmd_para  : ��������Ժ����������buffer
 * �� �� ֵ  : �������������
 */
int32 firmware_parse_cmd(uint8 *puc_cfg_buffer, uint8 *puc_cmd_name, uint32 cmd_name_len,
                         uint8 *puc_cmd_para, uint32 cmd_para_len)
{
    int32 ret;
    int32 cmd_type;
    int32 l_cmdlen;
    int32 l_paralen;
    uint8 *begin = NULL;
    uint8 *end = NULL;
    uint8 *link = NULL;
    uint8 *handle = NULL;
    uint8 *handle_temp = NULL;

    begin = puc_cfg_buffer;
    if ((puc_cfg_buffer == NULL) || (puc_cmd_name == NULL) || (puc_cmd_para == NULL)) {
        ps_print_err("para is NULL\n");
        return ERROR_TYPE_CMD;
    }

    /* ע���� */
    if (puc_cfg_buffer[0] == '@') {
        return ERROR_TYPE_CMD;
    }

    /* �����У������˳�������, ���������ֶ�,��INI�ļ��ж�̬��ȡ���� */
    link = os_str_chr((int8 *)begin, '=');
    if (link == NULL) {
        return firmware_parse_special_cmd(puc_cfg_buffer, puc_cmd_name, cmd_name_len, puc_cmd_para, cmd_para_len);
    }

    /* �����У�û�н����� */
    end = os_str_chr(link, ';');
    if (end == NULL) {
        return ERROR_TYPE_CMD;
    }

    l_cmdlen = link - begin;

    /* ɾ���ؼ��ֵ����߿ո� */
    handle = delete_space((uint8 *)begin, &l_cmdlen);
    if (handle == NULL) {
        return ERROR_TYPE_CMD;
    }

    /* �ж��������� */
    if ((l_cmdlen >= os_str_len(FILE_TYPE_CMD_KEY)) &&
        !os_mem_cmp(handle, FILE_TYPE_CMD_KEY, os_str_len(FILE_TYPE_CMD_KEY))) {
        handle_temp = os_str_str(handle, FILE_TYPE_CMD_KEY);
        if (handle_temp == NULL) {
            ps_print_err("'ADDR_FILE_'is not handle child string, handle=%s", handle);
            return ERROR_TYPE_CMD;
        }
        handle = handle_temp + os_str_len(FILE_TYPE_CMD_KEY);
        l_cmdlen = l_cmdlen - os_str_len(FILE_TYPE_CMD_KEY);
        cmd_type = FILE_TYPE_CMD;
    } else if ((l_cmdlen >= os_str_len(NUM_TYPE_CMD_KEY)) &&
               !os_mem_cmp(handle, NUM_TYPE_CMD_KEY, os_str_len(NUM_TYPE_CMD_KEY))) {
        handle_temp = os_str_str(handle, NUM_TYPE_CMD_KEY);
        if (handle_temp == NULL) {
            ps_print_err("'PARA_' is not handle child string, handle=%s", handle);
            return ERROR_TYPE_CMD;
        }
        handle = handle_temp + os_str_len(NUM_TYPE_CMD_KEY);
        l_cmdlen = l_cmdlen - os_str_len(NUM_TYPE_CMD_KEY);
        cmd_type = NUM_TYPE_CMD;
    } else {
        return ERROR_TYPE_CMD;
    }

    ret = memcpy_s(puc_cmd_name, cmd_name_len, handle, l_cmdlen);
    if (ret != EOK) {
        ps_print_err("cmd len out range! ret = %d\n", ret);
        return ERROR_TYPE_CMD;
    }
    /* ɾ��ֵ���߿ո� */
    begin = link + 1;
    l_paralen = end - begin;

    handle = delete_space(begin, &l_paralen);
    if (handle == NULL) {
        return ERROR_TYPE_CMD;
    }
    ret = memcpy_s(puc_cmd_para, cmd_para_len, handle, l_paralen);
    if (ret != EOK) {
        ps_print_err("para len out of range!ret = %d\n", ret);
        return ERROR_TYPE_CMD;
    }

    return cmd_type;
}

STATIC int32 firmware_parse_cur_cmd(uint32 index, uint32 pos)
{
    int32 ret;
    uint32 cmd_para_len = 0;

    /* ��ȡ���ð汾�� */
    if (!os_mem_cmp(g_cfg_info.apst_cmd[index][pos].cmd_name,
                    VER_CMD_KEYWORD,
                    os_str_len(VER_CMD_KEYWORD))) {
        cmd_para_len = os_str_len(g_cfg_info.apst_cmd[index][pos].cmd_para);
        ret = memcpy_s(g_cfg_info.auc_CfgVersion, sizeof(g_cfg_info.auc_CfgVersion),
                       g_cfg_info.apst_cmd[index][pos].cmd_para, cmd_para_len);
        if (ret != EOK) {
            ps_print_dbg("cmd_para_len = %d over auc_CfgVersion length", cmd_para_len);
            return -EFAIL;
        }

        ps_print_dbg("g_CfgVersion = [%s].\n", g_cfg_info.auc_CfgVersion);
    } else if (!os_mem_cmp(g_cfg_info.apst_cmd[index][pos].cmd_name,
                           CALI_DCXO_DATA_CMD_KEYWORD,
                           os_str_len(CALI_DCXO_DATA_CMD_KEYWORD))) {
        if (!test_bit(DCXO_PARA_READ_OK, &g_dcxo_info.nv_init_flag)) {
            ret = read_dcxo_cali_data();
            if (ret < 0) {
                ps_print_err("read dcxo para from nv failed !\n");
                return -EFAIL;
            }
            set_bit(DCXO_PARA_READ_OK, &g_dcxo_info.nv_init_flag);
        } else {
            ps_print_dbg("dcxo para has already initialized, skip...\n");
        }
    }

    return SUCC;
}

/*
 * �� �� ��  : firmware_parse_cfg
 * ��������  : ����cfg�ļ����������Ľ��������g_st_cfg_infoȫ�ֱ�����
 * �������  : puc_cfg_info_buf: ������cfg�ļ����ݵ�buffer
 *             l_buf_len       : puc_cfg_info_buf�ĳ���
 *             ul_index        : ��������������������ֵ
 * �� �� ֵ  : 0��ʾ�ɹ���-1��ʾʧ��
 */
STATIC int32 firmware_parse_cfg(uint8 *puc_cfg_info_buf, int32 l_buf_len, uint32 ul_index)
{
    int32 i;
    int32 l_len;
    uint8 *flag = NULL;
    uint8 *begin = NULL;
    uint8 *end = NULL;
    int32 cmd_type;
    uint8 cmd_name[DOWNLOAD_CMD_LEN];
    uint8 cmd_para[DOWNLOAD_CMD_PARA_LEN];

    if (puc_cfg_info_buf == NULL) {
        ps_print_err("puc_cfg_info_buf is NULL!\n");
        return -EFAIL;
    }

    g_cfg_info.apst_cmd[ul_index] = (struct cmd_type_st *)malloc_cmd_buf(puc_cfg_info_buf, ul_index);
    if (g_cfg_info.apst_cmd[ul_index] == NULL) {
        ps_print_err(" malloc_cmd_buf fail!\n");
        return -EFAIL;
    }

    /* ����CMD BUF */
    flag = puc_cfg_info_buf;
    l_len = l_buf_len;
    i = 0;
    while ((i < g_cfg_info.al_count[ul_index]) && (flag < &puc_cfg_info_buf[l_len])) {
        /*
         * ��ȡ�����ļ��е�һ��,�����ļ�������unix��ʽ.
         * �����ļ��е�ĳһ�к����ַ� @ ����Ϊ����Ϊע����
         */
        begin = flag;
        end = os_str_chr(flag, '\n');
        if (end == NULL) { /* �ļ������һ�У�û�л��з� */
            ps_print_dbg("lost of new line!\n");
            end = &puc_cfg_info_buf[l_len];
        } else if (end == begin) { /* ����ֻ��һ�����з� */
            ps_print_dbg("blank line\n");
            flag = end + 1;
            continue;
        }
        *end = '\0';

        ps_print_dbg("operation string is [%s]\n", begin);

        memset_s(cmd_name, DOWNLOAD_CMD_LEN, 0, DOWNLOAD_CMD_LEN);
        memset_s(cmd_para, DOWNLOAD_CMD_PARA_LEN, 0, DOWNLOAD_CMD_PARA_LEN);

        cmd_type = firmware_parse_cmd(begin, cmd_name, sizeof(cmd_name), cmd_para, sizeof(cmd_para));

        ps_print_dbg("cmd type=[%d],cmd_name=[%s],cmd_para=[%s]\n", cmd_type, cmd_name, cmd_para);

        if (cmd_type != ERROR_TYPE_CMD) { /* ��ȷ���������ͣ����� */
            g_cfg_info.apst_cmd[ul_index][i].cmd_type = cmd_type;
            memcpy_s(g_cfg_info.apst_cmd[ul_index][i].cmd_name, DOWNLOAD_CMD_LEN, cmd_name, DOWNLOAD_CMD_LEN);
            memcpy_s(g_cfg_info.apst_cmd[ul_index][i].cmd_para, DOWNLOAD_CMD_PARA_LEN,
                     cmd_para, DOWNLOAD_CMD_PARA_LEN);

            firmware_parse_cur_cmd(ul_index, i);
            i++;
        }
        flag = end + 1;
    }

    /* ����ʵ������������޸����յ�������� */
    g_cfg_info.al_count[ul_index] = i;
    ps_print_info("effective cmd count: al_count[%d] = %d\n", ul_index, g_cfg_info.al_count[ul_index]);

    return SUCC;
}

/*
 * �� �� ��  : firmware_get_cfg
 * ��������  : ��ȡcfg�ļ����������������Ľ��������g_st_cfg_infoȫ�ֱ�����
 * �������  : puc_CfgPatch: cfg�ļ���·��
 *             ul_index     : ��������������������ֵ
 * �� �� ֵ  : 0��ʾ�ɹ���-1��ʾʧ��
 */
int32 firmware_get_cfg(uint8 *puc_CfgPatch, uint32 ul_index)
{
    uint8 *puc_read_cfg_buf = NULL;
    int32 l_readlen;
    int32 ret;

    if (puc_CfgPatch == NULL) {
        ps_print_err("cfg file path is null!\n");
        return -EFAIL;
    }

    /* cfg�ļ��޶���С��2048,���cfg�ļ��Ĵ�Сȷʵ����2048�������޸�READ_CFG_BUF_LEN��ֵ */
    puc_read_cfg_buf = os_kmalloc_gfp(READ_CFG_BUF_LEN);
    if (puc_read_cfg_buf == NULL) {
        ps_print_err("kmalloc READ_CFG_BUF fail!\n");
        return -EFAIL;
    }

    l_readlen = firmware_read_cfg(puc_CfgPatch, puc_read_cfg_buf);
    if (l_readlen < 0) {
        ps_print_err("read cfg error!\n");
        os_mem_kfree(puc_read_cfg_buf);
        puc_read_cfg_buf = NULL;
        return -EFAIL;
    } else if (l_readlen > READ_CFG_BUF_LEN - 1) {
        /* ��1��Ϊ��ȷ��cfg�ļ��ĳ��Ȳ�����READ_CFG_BUF_LEN����Ϊfirmware_read_cfg���ֻ���ȡREAD_CFG_BUF_LEN���ȵ����� */
        ps_print_err("cfg file [%s] larger than %d\n", puc_CfgPatch, READ_CFG_BUF_LEN);
        os_mem_kfree(puc_read_cfg_buf);
        puc_read_cfg_buf = NULL;
        return -EFAIL;
    } else {
        ps_print_dbg("read cfg file [%s] ok, size is [%d]\n", puc_CfgPatch, l_readlen);
    }

    ret = firmware_parse_cfg(puc_read_cfg_buf, l_readlen, ul_index);
    if (ret < 0) {
        ps_print_err("parse cfg error!\n");
    }

    os_mem_kfree(puc_read_cfg_buf);
    puc_read_cfg_buf = NULL;

    return ret;
}

STATIC int32_t firmware_download_buffer_alloc(void)
{
    hcc_bus *bus = NULL;
    STATIC uint64_t max_cost_time = 0;
    declare_time_cost_stru(cost);

    bus = hcc_get_current_110x_bus();
    if (bus == NULL) {
        ps_print_err("firmware curr bus is null\n");
        return -EFAIL;
    }

    oal_get_time_cost_start(cost);
    g_firmware_down_buf = (uint8_t *)oal_memtry_alloc(oal_min(bus->cap.max_trans_size, MAX_FIRMWARE_FILE_TX_BUF_LEN),
                                                      MIN_FIRMWARE_FILE_TX_BUF_LEN, &g_firmware_down_buf_len);
    g_firmware_down_buf_len = OAL_ROUND_DOWN(g_firmware_down_buf_len, 8); /* �����3bit����֤8�ֽڶ��� */
    if (g_firmware_down_buf == NULL || (g_firmware_down_buf_len == 0)) {
        ps_print_err("g_pucDataBuf KMALLOC failed, min request:%u\n", MIN_FIRMWARE_FILE_TX_BUF_LEN);
        return -ENOMEM;
    }

    oal_get_time_cost_end(cost);
    oal_calc_time_cost_sub(cost);
    if (time_cost_var_sub(cost) > max_cost_time) {
        max_cost_time = time_cost_var_sub(cost);
    }

    ps_print_info("download firmware file buf len is [%d], cost time %llu, max cost time %llu\n",
                  g_firmware_down_buf_len, time_cost_var_sub(cost), max_cost_time);
    return SUCC;
}

/*
 * �� �� ��  : firmware_download
 * ��������  : firmware����
 * �������  : ul_index: ��Ч�����������������
 * �� �� ֵ  : 0��ʾ�ɹ���-1��ʾʧ��
 */
int32 firmware_download(uint32 ul_index)
{
    int32 ret;
    int32 i;
    int32 l_cmd_type;
    uint8 *puc_cmd_name = NULL;
    uint8 *puc_cmd_para = NULL;

    if (ul_index >= CFG_FILE_TOTAL) {
        ps_print_err("ul_index [%d] is error!\n", ul_index);
        return -EFAIL;
    }

    ps_print_info("start download firmware, ul_index = [%d]\n", ul_index);

    if (g_cfg_info.al_count[ul_index] == 0) {
        ps_print_err("firmware download cmd count is 0, ul_index = [%d]\n", ul_index);
        return -EFAIL;
    }


    if (firmware_download_buffer_alloc() < 0) {
        return -ENOMEM;
    }

    for (i = 0; i < g_cfg_info.al_count[ul_index]; i++) {
        l_cmd_type = g_cfg_info.apst_cmd[ul_index][i].cmd_type;
        puc_cmd_name = g_cfg_info.apst_cmd[ul_index][i].cmd_name;
        puc_cmd_para = g_cfg_info.apst_cmd[ul_index][i].cmd_para;

        ps_print_dbg("cmd[%d]:type[%d], name[%s], para[%s]\n", i, l_cmd_type, puc_cmd_name, puc_cmd_para);

        ps_print_dbg("firmware down start cmd[%d]:type[%d], name[%s]\n", i, l_cmd_type, puc_cmd_name);

        ret = execute_download_cmd(l_cmd_type, puc_cmd_name, puc_cmd_para);
        if (ret < 0) {
            os_mem_kfree(g_firmware_down_buf);
            g_firmware_down_buf = NULL;
            ps_print_err("download firmware fail\n");

            return ret;
        }

        ps_print_dbg("firmware down finish cmd[%d]:type[%d], name[%s]\n", i, l_cmd_type, puc_cmd_name);
    }

    os_mem_kfree(g_firmware_down_buf);
    g_firmware_down_buf = NULL;

    ps_print_info("finish download firmware\n");

    return SUCC;
}

STATIC int32 print_firmware_download_cmd(uint32 ul_index)
{
    int32 i;
    int32 l_cmd_type;
    uint8 *puc_cmd_name = NULL;
    uint8 *puc_cmd_para = NULL;
    uint32 count;

    count = g_cfg_info.al_count[ul_index];
    ps_print_info("[%s] download cmd, total count is [%d]\n", g_cfg_path[ul_index], count);

    for (i = 0; i < count; i++) {
        l_cmd_type = g_cfg_info.apst_cmd[ul_index][i].cmd_type;
        puc_cmd_name = g_cfg_info.apst_cmd[ul_index][i].cmd_name;
        puc_cmd_para = g_cfg_info.apst_cmd[ul_index][i].cmd_para;

        ps_print_info("cmd[%d]:type[%d], name[%s], para[%s]\n", i, l_cmd_type, puc_cmd_name, puc_cmd_para);
    }

    return 0;
}

int32 print_cfg_file_cmd(void)
{
    int32 i;

    for (i = 0; i < CFG_FILE_TOTAL; i++) {
        print_firmware_download_cmd(i);
    }

    return 0;
}

/*
 * �� �� ��  : firmware_cfg_path_init
 * ��������  : ��ȡfirmware��cfg�ļ�·��
 * �� �� ֵ  : 0��ʾ�ɹ���-1��ʾʧ��
 */
STATIC int32 firmware_cfg_path_init(void)
{
    if (get_hi110x_subchip_type() == BOARD_VERSION_HI1103) {
        g_cfg_path = g_hi1103_pilot_cfg_patch_in_vendor;
        set_hi110x_asic_type(HI1103_ASIC_PILOT);
    } else if (get_hi110x_subchip_type() == BOARD_VERSION_HI1105) {
        if (hi110x_is_asic()) {
            g_cfg_path = g_hi1105_asic_cfg_patch_in_vendor;
            set_hi110x_asic_type(HI1105_ASIC);
        } else {
            g_cfg_path = g_hi1105_fpga_cfg_patch_in_vendor;
            set_hi110x_asic_type(HI1105_FPGA);
        }
    } else if (get_hi110x_subchip_type() == BOARD_VERSION_HI1106) {
        if (hi110x_is_asic()) {
            g_cfg_path = g_hi1106_asic_cfg_patch_in_vendor;
            set_hi110x_asic_type(HI1106_ASIC);
        } else {
            g_cfg_path = g_hi1106_fpga_cfg_patch_in_vendor;
            set_hi110x_asic_type(HI1106_FPGA);
        }
    }

    return SUCC;
}

/*
 * �� �� ��  : firmware_cfg_init
 * ��������  : firmware���ص�cfg�ļ���ʼ������ȡ������cfg�ļ����������Ľ��������
 *             cfg_infoȫ�ֱ�����
 */
int32 firmware_cfg_init(void)
{
    int32 ret;
    uint32 i;

    ret = firmware_cfg_path_init();
    if (ret != SUCC) {
        ps_print_err("firmware cfg path init fail!");
        return -EFAIL;
    }

     /* �������ڱ��湲ʱ��У׼���ݵ�buffer */
    ret = dcxo_data_buf_malloc();
    if (ret < 0) {
        ps_print_err("alloc dcxo data buf fail\n");
        return -EFAIL;
    }

    /* ����cfg�ļ� */
    for (i = 0; i < CFG_FILE_TOTAL; i++) {
        ret = firmware_get_cfg(g_cfg_path[i], i);
        if (ret < 0) {
            if (i == RAM_REG_TEST_CFG) {
                ps_print_warning("ram_reg_test_cfg maybe not exist, please check\n");
                continue;
            }

            ps_print_err("get cfg file [%s] fail\n", g_cfg_path[i]);
            goto cfg_file_init_fail;
        }
    }

    return SUCC;

cfg_file_init_fail:
    firmware_cfg_clear();

    return -EFAIL;
}

/*
 * �� �� ��  : firmware_cfg_clear
 * ��������  : �ͷ�firmware_cfg_initʱ������ڴ�
 * �� �� ֵ  : ���Ƿ���0����ʾ�ɹ�
 */
int32 firmware_cfg_clear(void)
{
    int32 i;

    for (i = 0; i < CFG_FILE_TOTAL; i++) {
        g_cfg_info.al_count[i] = 0;
        if (g_cfg_info.apst_cmd[i] != NULL) {
            os_mem_kfree(g_cfg_info.apst_cmd[i]);
            g_cfg_info.apst_cmd[i] = NULL;
        }
    }

    dcxo_data_buf_free();

    return SUCC;
}

#define TSENSOR_VALID_HIGHEST_TEMP 125
#define TSENSOR_VALID_LOWEST_TEMP  (-40)

#define DEVICE_MEM_CHECK_SUCC 0x000f
#define RESULT_DETAIL_REG     "0x5000001c,4"
#define RESULT_TIME_LOW_REG   "0x50000010,4"
#define RESULT_TIME_HIGH_REG  "0x50000014,4"
#define RESULT_TSENSOR_C0_REG "0x5000348c,4"
#define RESULT_TSENSOR_C1_REG "0x500034cc,4"
#define GET_MEM_CHECK_FLAG    "0x50000018,4"

#define PRO_RAM_TEST_CASE_NONE                                   0x0
#define PRO_RAM_TEST_CASE_TCM_SCAN                               0x1
#define PRO_RAM_TEST_CASE_RAM_ALL_SCAN                           0x2
#define PRO_RAM_TEST_CASE_REG_SCAN                               0x3
#define PRO_RAM_TEST_CASE_BT_EM_SCAN                             0x4
#define PRO_RAM_TEST_CASE_MBIST                                  0x5
#define PRO_RAM_TEST_CASE_MBIST_CLDO1_WL0_MAJORITY_FULL_TEST_NEW 0x6
#define PRO_RAM_TEST_CASE_MBIST_CLDO2_FULL_TEST_NEW              0x7
#define PRO_RAM_TEST_CASE_MBIST_CLDO1_OTHER_FULL_TEST_NEW        0x8
#define PRO_RAM_TEST_CASE_MBIST_CLDO1_WL0_320_FULL_TEST_NEW      0x9
#define PRO_RAM_TEST_CASE_MBIST_CLDO1_WL0_MAJORITY_TEST_NEW      0xa

#define PRO_DETAIL_RESULT_NOT_START                   0x0000
#define PRO_DETAIL_RESULT_SUCC                        0xf000
#define PRO_DETAIL_RESULT_RUNING                      0xffff
#define PRO_DETAIL_RESULT_CASE_0                      0x1
#define PRO_DETAIL_RESULT_CASE_1                      0x2
#define PRO_DETAIL_RESULT_CASE_2_1                    0x3
#define PRO_DETAIL_RESULT_CASE_2_2                    0x4
#define PRO_DETAIL_RESULT_CASE_3                      0x5
#define pro_detail_error_result(main_case, test_case) (0xf000 | ((main_case) << 8) | ((test_case)&0xff))
#define pro_get_detail_error_result_main(error)       (((error) >> 8) & 0xf)
#define pro_get_detail_error_result_sub(error)        (((error) >> 0) & 0xff)

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int tsensor_c0_auto_clr : 1;   /* [0]  */
        unsigned int tsensor_c0_rdy_auto : 1;   /* [1]  */
        unsigned int tsensor_c0_data_auto : 10; /* [11..2]  */
        unsigned int reserved : 4;              /* [15..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} TSENSOR_AUTO_STS;

typedef struct _mem_test_type_str_ {
    uint32 id;
    char *name;
} mem_test_type_str;

STATIC mem_test_type_str g_mem_test_main_type[] = {
    { PRO_DETAIL_RESULT_CASE_0,   "SYSTEM_INIT" },
    { PRO_DETAIL_RESULT_CASE_1,   "CASE_1" },
    { PRO_DETAIL_RESULT_CASE_2_1, "CASE_2_1" },
    { PRO_DETAIL_RESULT_CASE_2_2, "CASE_2_2" },
    { PRO_DETAIL_RESULT_CASE_3,   "CASE_3" },
};

STATIC mem_test_type_str g_mem_test_sub_type[] = {
    { PRO_RAM_TEST_CASE_NONE,                                   "NOT_OVER" },
    { PRO_RAM_TEST_CASE_TCM_SCAN,                               "TCM_SCAN" },
    { PRO_RAM_TEST_CASE_RAM_ALL_SCAN,                           "RAM_ALL_SCAN" },
    { PRO_RAM_TEST_CASE_REG_SCAN,                               "REG_SCAN" },
    { PRO_RAM_TEST_CASE_BT_EM_SCAN,                             "BT_EM_SCAN" },
    { PRO_RAM_TEST_CASE_MBIST,                                  "MBIST_INIT" },
    { PRO_RAM_TEST_CASE_MBIST_CLDO1_WL0_MAJORITY_FULL_TEST_NEW, "CLDO1_WL0_MAJORITY_FULL_TEST_NEW" },
    { PRO_RAM_TEST_CASE_MBIST_CLDO2_FULL_TEST_NEW,              "CLDO2_FULL_TEST_NEW" },
    { PRO_RAM_TEST_CASE_MBIST_CLDO1_OTHER_FULL_TEST_NEW,        "CLDO1_OTHER_FULL_TEST_NEW" },
    { PRO_RAM_TEST_CASE_MBIST_CLDO1_WL0_320_FULL_TEST_NEW,      "CLDO1_WL0_320_FULL_TEST_NEW" },
    { PRO_RAM_TEST_CASE_MBIST_CLDO1_WL0_MAJORITY_TEST_NEW,      "CLDO1_WL0_MAJORITY_TEST_NEW" },
};

STATIC char *get_memcheck_error_main_string(uint32 result)
{
    uint16 main_type;
    uint32 i;
    uint32 num = sizeof(g_mem_test_main_type) / sizeof(mem_test_type_str);
    main_type = pro_get_detail_error_result_main(result);

    for (i = 0; i < num; i++) {
        if (main_type == g_mem_test_main_type[i].id) {
            return g_mem_test_main_type[i].name;
        }
    }

    return NULL;
}

STATIC char *get_memcheck_error_sub_string(uint32 result)
{
    uint16 sub_type;
    uint32 i;
    uint32 num = sizeof(g_mem_test_sub_type) / sizeof(mem_test_type_str);
    sub_type = pro_get_detail_error_result_sub(result);
    for (i = 0; i < num; i++) {
        if (sub_type == g_mem_test_sub_type[i].id) {
            return g_mem_test_sub_type[i].name;
        }
    }

    return NULL;
}

int32 is_device_mem_test_succ(void)
{
    int32 ret;
    int32 test_flag = 0;

    ret = number_type_cmd_send(RMEM_CMD_KEYWORD, GET_MEM_CHECK_FLAG);
    if (ret < 0) {
        ps_print_warning("send cmd %s:%s fail,ret = %d\n", RMEM_CMD_KEYWORD, GET_MEM_CHECK_FLAG, ret);
        return -1;
    }

    ret = read_msg((uint8 *)&test_flag, sizeof(test_flag));
    if (ret < 0) {
        ps_print_warning("read device test flag fail, read_len = %d, return = %d\n", (int32)sizeof(test_flag), ret);
        return -1;
    }
    ps_print_warning("get device test flag:0x%x\n", test_flag);
    if (test_flag == DEVICE_MEM_CHECK_SUCC) {
        return 0;
    }
    return -1;
}

STATIC void print_device_ram_test_detail_result(int32 is_wcpu, uint32 result)
{
    char *case_name = NULL;
    char *main_str = NULL;
    char *sub_str = NULL;
    if (is_wcpu == true) {
        case_name = "[wcpu_memcheck]";
    } else {
        case_name = "[bcpu_memcheck]";
    }
    ps_print_warning("%s detail result=0x%x\n", case_name, result);
    if (result == PRO_DETAIL_RESULT_NOT_START) {
        ps_print_warning("%s didn't start run\n", case_name);
        return;
    } else if (result == PRO_DETAIL_RESULT_SUCC) {
        ps_print_warning("%s run succ\n", case_name);
        return;
    } else if (result == PRO_DETAIL_RESULT_RUNING) {
        ps_print_warning("%s still running\n", case_name);
        return;
    }

    main_str = get_memcheck_error_main_string(result);
    sub_str = get_memcheck_error_sub_string(result);

    ps_print_err("%s error found [%s:%s]\n",
                 case_name,
                 (main_str == NULL) ? "unkown" : main_str,
                 (sub_str == NULL) ? "unkown" : sub_str);
}

int32 get_device_ram_test_result(int32 is_wcpu, uint32 *cost)
{
    int32 ret;
    uint32 result = 0;
    hcc_bus *pst_bus;
    uint32 time_cost;

    *cost = 0;

    pst_bus = hcc_get_current_110x_bus();
    if (pst_bus == NULL) {
        ps_print_err("pst_bus is null");
        return -OAL_EFAIL;
    }

    if (pst_bus->bus_type != HCC_BUS_PCIE) {
        if (is_wcpu != true) {
            /* bcpu ����wmbist �ᵼ��sdio�ӿ��޷��ض������ֳ������ض� */
            ps_print_err("bcpu ram test can't read detail result");
            return -OAL_ENODEV;
        }
    }

    /* ʧ�ܺ��ȡ��ϸ�Ľ�� */
    ret = number_type_cmd_send(RMEM_CMD_KEYWORD, RESULT_DETAIL_REG);
    if (ret < 0) {
        ps_print_warning("send cmd %s:%s fail,ret = %d\n", RMEM_CMD_KEYWORD, RESULT_DETAIL_REG, ret);
        return -1;
    }

    ret = read_msg((uint8 *)&result, sizeof(result));
    if (ret < 0) {
        ps_print_warning("send cmd %s:%s read result failed,ret = %d\n", RMEM_CMD_KEYWORD, RESULT_DETAIL_REG, ret);
        return -1;
    }

    print_device_ram_test_detail_result(is_wcpu, result);

    ret = number_type_cmd_send(RMEM_CMD_KEYWORD, RESULT_TIME_LOW_REG);
    if (ret < 0) {
        ps_print_warning("send cmd %s:%s fail,ret = %d\n", RMEM_CMD_KEYWORD, RESULT_TIME_LOW_REG, ret);
        return -1;
    }

    ret = read_msg((uint8 *)&result, sizeof(result));
    if (ret < 0) {
        ps_print_warning("send cmd %s:%s read result failed,ret = %d\n", RMEM_CMD_KEYWORD, RESULT_TIME_LOW_REG, ret);
        return -1;
    }

    time_cost = (result & 0xffff);

    ret = number_type_cmd_send(RMEM_CMD_KEYWORD, RESULT_TIME_HIGH_REG);
    if (ret < 0) {
        ps_print_warning("send cmd %s:%s fail,ret = %d\n", RMEM_CMD_KEYWORD, RESULT_TIME_HIGH_REG, ret);
        return -1;
    }

    ret = read_msg((uint8 *)&result, sizeof(result));
    if (ret < 0) {
        ps_print_warning("send cmd %s:%s read result failed,ret = %d\n", RMEM_CMD_KEYWORD, RESULT_TIME_HIGH_REG, ret);
        return -1;
    }

    time_cost |= ((result & 0xffff) << 16); /* ����16λ */
    ps_print_warning("%s_ram_test_time_cost tick:%u  %u us\n",
                     (is_wcpu == true) ? "wcpu" : "bcpu", time_cost, time_cost * 31); /* time from 31+1 k */
    *cost = time_cost * 31; /* time_cost * 31 */

    /* tsensor read */
    if (g_ram_test_detail_tsensor_dump) {
        uint32 tsensor_c0 = 0;
        uint32 tsensor_c1 = 0;
        oal_uint16 us_reg_val;
        oal_uint16 us_temp_data;
        oal_int32 l_temp_val;
        TSENSOR_AUTO_STS *pun_tsensor_auto_sts = NULL;

        ret = number_type_cmd_send(RMEM_CMD_KEYWORD, RESULT_TSENSOR_C0_REG);
        if (ret < 0) {
            ps_print_warning("send cmd %s:%s fail,ret = %d\n", RMEM_CMD_KEYWORD, RESULT_TSENSOR_C0_REG, ret);
            return -1;
        }

        ret = read_msg((uint8 *)&tsensor_c0, sizeof(tsensor_c0));
        if (ret < 0) {
            ps_print_warning("send cmd %s:%s read result failed,ret = %d\n",
                             RMEM_CMD_KEYWORD, RESULT_TSENSOR_C0_REG, ret);
            return -1;
        }

        ret = number_type_cmd_send(RMEM_CMD_KEYWORD, RESULT_TSENSOR_C1_REG);
        if (ret < 0) {
            ps_print_warning("send cmd %s:%s fail,ret = %d\n", RMEM_CMD_KEYWORD, RESULT_TSENSOR_C1_REG, ret);
            return -1;
        }

        ret = read_msg((uint8 *)&tsensor_c1, sizeof(tsensor_c1));
        if (ret < 0) {
            ps_print_warning("send cmd %s:%s read result failed,ret = %d\n",
                             RMEM_CMD_KEYWORD, RESULT_TSENSOR_C1_REG, ret);
            return -1;
        }

        us_reg_val = tsensor_c0;
        pun_tsensor_auto_sts = (TSENSOR_AUTO_STS *)&us_reg_val;
        us_temp_data = pun_tsensor_auto_sts->bits.tsensor_c0_data_auto;
        l_temp_val = ((((oal_int32)us_temp_data - 118) * 165) / 815) - 40; /* �¶���ת����ʽl_val=(us-118)*165)/815)-40 */
        ps_print_info("[memcheck]TsensorC0, val 0x%x data,%u rdy,%u temp %d\n",
                      us_reg_val, pun_tsensor_auto_sts->bits.tsensor_c0_data_auto,
                      pun_tsensor_auto_sts->bits.tsensor_c0_rdy_auto, l_temp_val);
        if ((pun_tsensor_auto_sts->bits.tsensor_c0_rdy_auto) &&
            ((l_temp_val < TSENSOR_VALID_LOWEST_TEMP) || (l_temp_val > TSENSOR_VALID_HIGHEST_TEMP))) {
            ps_print_info("[memcheck]TsensorC0, invalid");
        }

        us_reg_val = tsensor_c1;
        pun_tsensor_auto_sts = (TSENSOR_AUTO_STS *)&us_reg_val;
        us_temp_data = pun_tsensor_auto_sts->bits.tsensor_c0_data_auto;
        l_temp_val = ((((oal_int32)us_temp_data - 118) * 165) / 815) - 40; /* �¶���ת����ʽl_val=(us-118)*165)/815)-40 */
        ps_print_info("[memcheck]TsensorC1, val 0x%x data,%u rdy,%u temp %d\n",
                      us_reg_val, pun_tsensor_auto_sts->bits.tsensor_c0_data_auto,
                      pun_tsensor_auto_sts->bits.tsensor_c0_rdy_auto, l_temp_val);
        if ((pun_tsensor_auto_sts->bits.tsensor_c0_rdy_auto) &&
            ((l_temp_val < TSENSOR_VALID_LOWEST_TEMP) || (l_temp_val > TSENSOR_VALID_HIGHEST_TEMP))) {
            ps_print_info("[memcheck]TsensorC1, invalid");
        }
    }

    return 0;
}

int32 get_device_test_mem(bool is_wifi)
{
    wlan_memdump_t *wlan_memdump_s = NULL;
    oal_int32 ret;
    const uint32 ul_buff_len = 100;
    uint8 buff[ul_buff_len];
    wlan_memdump_s = get_wlan_memdump_cfg();
    if (wlan_memdump_s == NULL) {
        ps_print_err("memdump cfg is NULL!\n");
        return -FAILURE;
    }
    ret = snprintf_s(buff, sizeof(buff), sizeof(buff) - 1, "0x%x,%d", wlan_memdump_s->addr, wlan_memdump_s->len);
    if (ret < 0) {
        ps_print_err("log str format err line[%d]\n", __LINE__);
        return ret;
    }
    if (sdio_read_mem(RMEM_CMD_KEYWORD, buff, is_wifi) >= 0) {
        ps_print_warning("read device mem succ\n");
    } else {
        ps_print_warning("read device mem fail\n");
        return -FAILURE;
    }
    return 0;
}
