

/* ͷ�ļ����� */
#include "plat_cali.h"

#include "plat_firmware.h"
#include "plat_debug.h"
#include "bfgx_dev.h"
#include "plat_pm.h"

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 35))
#ifndef _PRE_NO_HISI_NVRAM
#define HISI_NVRAM_SUPPORT
#endif
#endif

/* ����У׼���ݵ�buf */
STATIC oal_uint8 *g_cali_data_buf = NULL; /* 03 wifiУ׼���� */
oal_uint8 g_netdev_is_open = OAL_FALSE;
oal_uint32 g_cali_update_channel_info = 0;

/* add for hi1103 bfgx */
struct completion g_cali_recv_done;
STATIC oal_uint8 *g_bfgx_cali_data_buf = NULL;

/* ���岻�ܳ���BFGX_BT_CUST_INI_SIZE/4 (128) ���ʹ�� { NULL, 0 } ��β */
STATIC bfgx_ini_cmd g_bfgx_ini_config_cmd_1103[BFGX_BT_CUST_INI_SIZE / 4] = {
    { "bt_maxpower",                       0 },
    { "bt_edrpow_offset",                  0 },
    { "bt_blepow_offset",                  0 },
    { "bt_cali_txpwr_pa_ref_num",          0 },
    { "bt_cali_txpwr_pa_ref_band1",        0 },
    { "bt_cali_txpwr_pa_ref_band2",        0 },
    { "bt_cali_txpwr_pa_ref_band3",        0 },
    { "bt_cali_txpwr_pa_ref_band4",        0 },
    { "bt_cali_txpwr_pa_ref_band5",        0 },
    { "bt_cali_txpwr_pa_ref_band6",        0 },
    { "bt_cali_txpwr_pa_ref_band7",        0 },
    { "bt_cali_txpwr_pa_ref_band8",        0 },
    { "bt_cali_txpwr_pa_fre1",             0 },
    { "bt_cali_txpwr_pa_fre2",             0 },
    { "bt_cali_txpwr_pa_fre3",             0 },
    { "bt_cali_txpwr_pa_fre4",             0 },
    { "bt_cali_txpwr_pa_fre5",             0 },
    { "bt_cali_txpwr_pa_fre6",             0 },
    { "bt_cali_txpwr_pa_fre7",             0 },
    { "bt_cali_txpwr_pa_fre8",             0 },
    { "bt_cali_bt_tone_amp_grade",         0 },
    { "bt_rxdc_band",                      0 },
    { "bt_dbb_scaling_saturation",         0 },
    { "bt_productline_upccode_search_max", 0 },
    { "bt_productline_upccode_search_min", 0 },
    { "bt_dynamicsarctrl_bt",              0 },
    { "bt_powoffsbt",                      0 },
    { "bt_elna_2g_bt",                     0 },
    { "bt_rxisobtelnabyp",                 0 },
    { "bt_rxgainbtelna",                   0 },
    { "bt_rxbtextloss",                    0 },
    { "bt_elna_on2off_time_ns",            0 },
    { "bt_elna_off2on_time_ns",            0 },
    { "bt_hipower_mode",                   0 },
    { "bt_fem_control",                    0 },
    { "bt_feature_32k_clock",              0 },
    { "bt_feature_log",                    0 },
    { "bt_cali_swtich_all",                0 },
    { "bt_ant_num_bt",                     0 },
    { "bt_power_level_control",            0 },
    { "bt_country_code",                   0 },
    { "bt_reserved1",                      0 },
    { "bt_reserved2",                      0 },
    { "bt_reserved3",                      0 },
    { "bt_dedicated_antenna",              0 },
    { "bt_reserved5",                      0 },
    { "bt_reserved6",                      0 },
    { "bt_reserved7",                      0 },
    { "bt_reserved8",                      0 },
    { "bt_reserved9",                      0 },
    { "bt_reserved10",                     0 },

    { NULL, 0 }
};

/* ���岻�ܳ���BFGX_BT_CUST_INI_SIZE/4 (128) ���ʹ�� { NULL, 0 } ��β */
STATIC bfgx_ini_cmd g_bfgx_ini_config_cmd_1105[BFGX_BT_CUST_INI_SIZE / 4] = {
    { "bt_maxpower",                       0 },
    { "bt_edrpow_offset",                  0 },
    { "bt_blepow_offset",                  0 },
    { "bt_cali_txpwr_pa_ref_num",          0 },
    { "bt_cali_txpwr_pa_ref_band1",        0 },
    { "bt_cali_txpwr_pa_ref_band2",        0 },
    { "bt_cali_txpwr_pa_ref_band3",        0 },
    { "bt_cali_txpwr_pa_ref_band4",        0 },
    { "bt_cali_txpwr_pa_ref_band5",        0 },
    { "bt_cali_txpwr_pa_ref_band6",        0 },
    { "bt_cali_txpwr_pa_ref_band7",        0 },
    { "bt_cali_txpwr_pa_ref_band8",        0 },
    { "bt_cali_txpwr_pa_fre1",             0 },
    { "bt_cali_txpwr_pa_fre2",             0 },
    { "bt_cali_txpwr_pa_fre3",             0 },
    { "bt_cali_txpwr_pa_fre4",             0 },
    { "bt_cali_txpwr_pa_fre5",             0 },
    { "bt_cali_txpwr_pa_fre6",             0 },
    { "bt_cali_txpwr_pa_fre7",             0 },
    { "bt_cali_txpwr_pa_fre8",             0 },
    { "bt_cali_bt_tone_amp_grade",         0 },
    { "bt_rxdc_band",                      0 },
    { "bt_dbb_scaling_saturation",         0 },
    { "bt_productline_upccode_search_max", 0 },
    { "bt_productline_upccode_search_min", 0 },
    { "bt_dynamicsarctrl_bt",              0 },
    { "bt_powoffsbt",                      0 },
    { "bt_elna_2g_bt",                     0 },
    { "bt_rxisobtelnabyp",                 0 },
    { "bt_rxgainbtelna",                   0 },
    { "bt_rxbtextloss",                    0 },
    { "bt_elna_on2off_time_ns",            0 },
    { "bt_elna_off2on_time_ns",            0 },
    { "bt_hipower_mode",                   0 },
    { "bt_fem_control",                    0 },
    { "bt_feature_32k_clock",              0 },
    { "bt_feature_log",                    0 },
    { "bt_cali_switch_all",                0 },
    { "bt_ant_num_bt",                     0 },
    { "bt_power_level_control",            0 },
    { "bt_country_code",                   0 },
    { "bt_power_idle_voltage",             0 },
    { "bt_power_tx_voltage",               0 },
    { "bt_power_rx_voltage",               0 },
    { "bt_power_sleep_voltage",            0 },
    { "bt_power_idle_current",             0 },
    { "bt_power_tx_current",               0 },
    { "bt_power_rx_current",               0 },
    { "bt_power_sleep_current",            0 },
    { "bt_20dbm_txpwr_cali_num",           0 },
    { "bt_20dbm_txpwr_cali_freq_ref1",     0 },
    { "bt_20dbm_txpwr_cali_freq_ref2",     0 },
    { "bt_20dbm_txpwr_cali_freq_ref3",     0 },
    { "bt_20dbm_txpwr_cali_freq_ref4",     0 },
    { "bt_20dbm_txpwr_cali_freq_ref5",     0 },
    { "bt_20dbm_txpwr_cali_freq_ref6",     0 },
    { "bt_20dbm_txpwr_cali_freq_ref7",     0 },
    { "bt_20dbm_txpwr_cali_freq_ref8",     0 },
    { "bt_20dbm_txpwr_cali_freq_ref9",     0 },
    { "bt_20dbm_txpwr_cali_freq_ref10",    0 },
    { "bt_20dbm_txpwr_cali_freq_ref11",    0 },
    { "bt_20dbm_txpwr_cali_freq_ref12",    0 },
    { "bt_20dbm_txpwr_cali_freq_ref13",    0 },
    { "bt_reserved1",                      0 },
    { "bt_20dbm_txpwr_adjust",             0 },
    { "bt_feature_config",                 0 },
    { "bt_dedicated_antenna",              0 },
    { "bt_reserved5",                      0 },
    { "bt_reserved6",                      0 },
    { "bt_reserved7",                      0 },
    { "bt_reserved8",                      0 },
    { "bt_reserved9",                      0 },
    { "bt_reserved10",                     0 },

    { NULL, 0 }
};

/* ���岻�ܳ���BFGX_BT_CUST_INI_SIZE/4 (128) */
STATIC int32 g_bfgx_cust_ini_data[BFGX_BT_CUST_INI_SIZE / 4] = {0};

/*
 * �� �� ��  : get_cali_count
 * ��������  : ����У׼�Ĵ������״ο���У׼ʱΪ0���Ժ����
 * �������  : uint32 *count:���ú�������У׼�����ĵ�ַ
 * �������  : count:�Կ����������Ѿ�У׼�Ĵ���
 * �� �� ֵ  : 0��ʾ�ɹ���-1��ʾʧ��
 */
oal_int32 get_cali_count(oal_uint32 *count)
{
    if (count == NULL) {
        ps_print_err("count is NULL\n");
        return -EFAIL;
    }

    *count = g_cali_update_channel_info;

    ps_print_warning("cali update info is [%d]\r\n", g_cali_update_channel_info);

    return SUCC;
}

#ifdef HISI_NVRAM_SUPPORT
/*
 * �� �� ��  : bfgx_get_nv_data_buf
 * ��������  : ���ر���bfgx nv���ݵ��ڴ��ַ
 * �������  : nv buf�ĳ���
 * �� �� ֵ  : bfgx nv����buf�ĵ�ַ��Ҳ������NULL
 */
STATIC void *bfgx_get_nv_data_buf(uint32 *pul_len)
{
    bfgx_cali_data_stru *pst_bfgx_cali_data_buf = NULL;

    if (g_bfgx_cali_data_buf == NULL) {
        return NULL;
    }

    pst_bfgx_cali_data_buf = (bfgx_cali_data_stru *)g_bfgx_cali_data_buf;

    *pul_len = sizeof(pst_bfgx_cali_data_buf->auc_nv_data);

    ps_print_info("bfgx nv buf size is %d\n", *pul_len);

    return pst_bfgx_cali_data_buf->auc_nv_data;
}

/*
 * �� �� ��  : bfgx_nv_data_init
 * ��������  : bt У׼NV��ȡ
 */
STATIC oal_int32 bfgx_nv_data_init(void)
{
    int32 l_ret;
    int8 *pst_buf;
    uint32 ul_len;

    oal_uint8 bt_cal_nvram_tmp[OAL_BT_NVRAM_DATA_LENGTH];

    l_ret = read_conf_from_nvram(bt_cal_nvram_tmp, OAL_BT_NVRAM_DATA_LENGTH, OAL_BT_NVRAM_NUMBER, OAL_BT_NVRAM_NAME);
    if (l_ret != INI_SUCC) {
        ps_print_err("bfgx_nv_data_init::BT read NV error!");
        // last byte of NV ram is used to mark if NV ram is failed to read.
        bt_cal_nvram_tmp[OAL_BT_NVRAM_DATA_LENGTH - 1] = OAL_TRUE;
    } else {
        // last byte of NV ram is used to mark if NV ram is failed to read.
        bt_cal_nvram_tmp[OAL_BT_NVRAM_DATA_LENGTH - 1] = OAL_FALSE;
    }

    pst_buf = bfgx_get_nv_data_buf(&ul_len);
    if (pst_buf == NULL) {
        ps_print_err("get bfgx nv buf fail!");
        return INI_FAILED;
    }
    if (ul_len < OAL_BT_NVRAM_DATA_LENGTH) {
        ps_print_err("get bfgx nv buf size %d, NV data size is %d!", ul_len, OAL_BT_NVRAM_DATA_LENGTH);
        return INI_FAILED;
    }

    l_ret = memcpy_s(pst_buf, ul_len, bt_cal_nvram_tmp, OAL_BT_NVRAM_DATA_LENGTH);
    if (l_ret != EOK) {
        ps_print_err("bfgx_nv_data_init FAILED!");
        return INI_FAILED;
    }
    ps_print_info("bfgx_nv_data_init SUCCESS");
    return INI_SUCC;
}
#endif

/*
 * �� �� ��  : hi1103_get_bfgx_cali_data
 * ��������  : ���ر���bfgxУ׼���ݵ��ڴ��׵�ַ�Լ�����
 * �������  : uint8  *buf:���ú�������bfgxУ׼���ݵ��׵�ַ
 *             uint32 *len:���ú�������bfgxУ׼�����ڴ泤�ȵĵ�ַ
 *             uint32 buf_len:buf�ĳ���
 * �� �� ֵ  : 0��ʾ�ɹ���-1��ʾʧ��
 */
STATIC int32 hi1103_get_bfgx_cali_data(oal_uint8 *buf, oal_uint32 *len, oal_uint32 buf_len)
{
    oal_uint32 bfgx_cali_data_len;

    ps_print_info("%s\n", __func__);

    if (unlikely(buf == NULL)) {
        ps_print_err("buf is NULL\n");
        return -EFAIL;
    }

    if (unlikely(len == NULL)) {
        ps_print_err("len is NULL\n");
        return -EFAIL;
    }

    if (unlikely(g_bfgx_cali_data_buf == NULL)) {
        ps_print_err("g_bfgx_cali_data_buf is NULL\n");
        return -EFAIL;
    }

#ifdef HISI_NVRAM_SUPPORT
    if (bfgx_nv_data_init() != OAL_SUCC) {
        ps_print_err("bfgx nv data init fail!\n");
    }
#endif

    bfgx_cali_data_len = sizeof(bfgx_cali_data_stru);
    if (buf_len < bfgx_cali_data_len) {
        ps_print_err("bfgx cali buf len[%d] is smaller than struct size[%d]\n", buf_len, bfgx_cali_data_len);
        return -EFAIL;
    }

    memcpy_s(buf, buf_len, g_bfgx_cali_data_buf, bfgx_cali_data_len);
    *len = bfgx_cali_data_len;

    return SUCC;
}

/*
 * �� �� ��  : get_bfgx_cali_data
 * ��������  : ���ر���bfgxУ׼���ݵ��ڴ��׵�ַ�Լ�����
 * �������  : uint8  *buf:���ú�������bfgxУ׼���ݵ��׵�ַ
 *             uint32 *len:���ú�������bfgxУ׼�����ڴ泤�ȵĵ�ַ
 *             uint32 buf_len:buf�ĳ���
 * �� �� ֵ  : 0��ʾ�ɹ���-1��ʾʧ��
 */
int32 get_bfgx_cali_data(oal_uint8 *buf, oal_uint32 *len, oal_uint32 buf_len)
{
    oal_int32 l_subchip_type = get_hi110x_subchip_type();

    switch (l_subchip_type) {
        case BOARD_VERSION_HI1103:
        case BOARD_VERSION_HI1105:
        case BOARD_VERSION_HI1106:
            return hi1103_get_bfgx_cali_data(buf, len, buf_len);
        default:
            ps_print_err("subchip type error! subchip=%d\n", l_subchip_type);
            return -EFAIL;
    }
}

/*
 * �� �� ��  : get_cali_data_buf_addr
 * ��������  : ���ر���У׼���ݵ��ڴ��ַ
 */
void *get_cali_data_buf_addr(void)
{
    return g_cali_data_buf;
}

EXPORT_SYMBOL(get_cali_data_buf_addr);
EXPORT_SYMBOL(g_netdev_is_open);
EXPORT_SYMBOL(g_cali_update_channel_info);

/*
 * �� �� ��  : wifi_get_bfgx_rc_data_buf_addr
 * ��������  : ���ر���wifi rc codeУ׼���ݵ��ڴ��ַ
 */
void *wifi_get_bfgx_rc_data_buf_addr(uint32 *pul_len)
{
    bfgx_cali_data_stru *pst_bfgx_cali_buf = NULL;

    if (g_bfgx_cali_data_buf == NULL) {
        return NULL;
    }

    pst_bfgx_cali_buf = (bfgx_cali_data_stru *)g_bfgx_cali_data_buf;
    *pul_len = sizeof(pst_bfgx_cali_buf->auc_wifi_rc_code_data);

    ps_print_info("wifi cali size is %d\n", *pul_len);

    return pst_bfgx_cali_buf->auc_wifi_rc_code_data;
}

EXPORT_SYMBOL(wifi_get_bfgx_rc_data_buf_addr);

/*
 * �� �� ��  : wifi_get_bt_cali_data_buf
 * ��������  : ���ر���wifi cali data for bt���ݵ��ڴ��ַ
 * �������  : wifi cali data for bt����buf�ĳ���
 * �� �� ֵ  : wifi cali data for bt����buf�ĵ�ַ��Ҳ������NULL
 */
void *wifi_get_bt_cali_data_buf(uint32 *pul_len)
{
    bfgx_cali_data_stru *pst_bfgx_cali_data_buf = NULL;

    if (g_bfgx_cali_data_buf == NULL) {
        return NULL;
    }

    pst_bfgx_cali_data_buf = (bfgx_cali_data_stru *)g_bfgx_cali_data_buf;

    *pul_len = sizeof(pst_bfgx_cali_data_buf->auc_wifi_cali_for_bt_data);

    ps_print_info("bfgx wifi cali data for bt buf size is %d\n", *pul_len);

    return pst_bfgx_cali_data_buf->auc_wifi_cali_for_bt_data;
}

EXPORT_SYMBOL(wifi_get_bt_cali_data_buf);

/*
 * �� �� ��  : bfgx_get_cali_data_buf
 * ��������  : ���ر���bfgx btУ׼���ݵ��ڴ��ַ
 * �������  : bt У׼���� buf�ĳ���
 * �� �� ֵ  : bfgx btУ׼buf�ĵ�ַ��Ҳ������NULL
 */
void *bfgx_get_cali_data_buf(uint32 *pul_len)
{
    bfgx_cali_data_stru *pst_bfgx_cali_data_buf = NULL;

    if (g_bfgx_cali_data_buf == NULL) {
        return NULL;
    }

    pst_bfgx_cali_data_buf = (bfgx_cali_data_stru *)g_bfgx_cali_data_buf;

    *pul_len = sizeof(pst_bfgx_cali_data_buf->auc_bfgx_data);

    ps_print_info("bfgx bt cali data buf size is %d\n", *pul_len);

    return pst_bfgx_cali_data_buf->auc_bfgx_data;
}

/*
 * �� �� ��  : bfgx_get_cust_ini_data_buf
 * ��������  : ���ر���bfgx ini���ƻ����ݵ��ڴ��ַ
 * �������  : bfgx ini���ƻ�����buf�ĳ���
 * �� �� ֵ  : bfgx ini����buf�ĵ�ַ��Ҳ������NULL
 */
STATIC void *bfgx_get_cust_ini_data_buf(uint32 *pul_len)
{
    bfgx_cali_data_stru *pst_bfgx_cali_data_buf = NULL;

    if (g_bfgx_cali_data_buf == NULL) {
        return NULL;
    }

    pst_bfgx_cali_data_buf = (bfgx_cali_data_stru *)g_bfgx_cali_data_buf;

    *pul_len = sizeof(pst_bfgx_cali_data_buf->auc_bt_cust_ini_data);

    ps_print_info("bfgx cust ini buf size is %d\n", *pul_len);

    return pst_bfgx_cali_data_buf->auc_bt_cust_ini_data;
}

void plat_bfgx_cali_data_test(void)
{
    bfgx_cali_data_stru *pst_cali_data = NULL;
    oal_uint32 *p_test = NULL;
    oal_uint32 count;
    oal_uint32 i;

    pst_cali_data = (bfgx_cali_data_stru *)bfgx_get_cali_data_buf(&count);
    if (pst_cali_data == NULL) {
        ps_print_err("get_cali_data_buf_addr failed\n");
        return;
    }

    p_test = (oal_uint32 *)pst_cali_data;
    count = count / sizeof(oal_uint32);

    for (i = 0; i < count; i++) {
        p_test[i] = i;
    }

    return;
}

/*
 * �� �� ��  : cali_data_buf_malloc
 * ��������  : ���䱣��У׼���ݵ��ڴ�
 * �� �� ֵ  : 0��ʾ����ɹ���-1��ʾ����ʧ��
 */
oal_int32 cali_data_buf_malloc(void)
{
    oal_uint8 *buffer = NULL;
    oal_uint32 ul_buffer_len;

    if (get_hi110x_subchip_type() == BOARD_VERSION_HI1106) {
        ul_buffer_len = HI1106_MIMO_CALI_DATA_STRU_LEN;
    } else if (get_hi110x_subchip_type() == BOARD_VERSION_HI1105) {
        ul_buffer_len = OAL_MIMO_CALI_DATA_STRU_LEN;
    } else {
        ul_buffer_len = OAL_DOUBLE_CALI_DATA_STRU_LEN;
    }

    if (g_cali_data_buf == NULL) {
        buffer = (oal_uint8 *)os_kzalloc_gfp(ul_buffer_len);
        if (buffer == NULL) {
            ps_print_err("malloc for g_cali_data_buf fail\n");
            return -EFAIL;
        }
        g_cali_data_buf = buffer;
        memset_s(g_cali_data_buf, ul_buffer_len, 0, ul_buffer_len);
    } else {
        ps_print_info("g_cali_data_buf size %d alloc in sysfs init\n", ul_buffer_len);
    }

    if (g_bfgx_cali_data_buf == NULL) {
        buffer = (oal_uint8 *)os_kzalloc_gfp(BFGX_CALI_DATA_BUF_LEN);
        if (buffer == NULL) {
            os_mem_kfree(g_cali_data_buf);
            g_cali_data_buf = NULL;
            ps_print_err("malloc for g_bfgx_cali_data_buf fail\n");
            return -EFAIL;
        }
        g_bfgx_cali_data_buf = buffer;
    } else {
        ps_print_info("g_bfgx_cali_data_buf alloc in sysfs init\n");
    }

    return SUCC;
}

/*
 * �� �� ��  : cali_data_buf_free
 * ��������  : �ͷű���У׼���ݵ��ڴ�
 */
void cali_data_buf_free(void)
{
    if (g_cali_data_buf != NULL) {
        os_mem_kfree(g_cali_data_buf);
    }
    g_cali_data_buf = NULL;

    if (g_bfgx_cali_data_buf != NULL) {
        os_mem_kfree(g_bfgx_cali_data_buf);
    }
    g_bfgx_cali_data_buf = NULL;
}

/*
 * �� �� ��  : wait_bfgx_cali_data
 * ��������  : �ȴ�����device���͵�У׼����
 * �� �� ֵ  : 0��ʾ�ɹ���-1��ʾʧ��
 */
STATIC int32 wait_bfgx_cali_data(void)
{
#define WAIT_BFGX_CALI_DATA_TIME 2000
    uint64 timeleft;

    timeleft = wait_for_completion_timeout(&g_cali_recv_done, msecs_to_jiffies(WAIT_BFGX_CALI_DATA_TIME));
    if (!timeleft) {
        ps_print_err("wait bfgx cali data timeout\n");
        return -ETIMEDOUT;
    }

    return 0;
}

/*
 * �� �� ��  : bfgx_cust_ini_init
 * ��������  : btУ׼���ƻ����ʼ��
 * �� �� ֵ  : 0��ʾ�ɹ���-1��ʾʧ��
 */
STATIC int32 bfgx_cust_ini_init(void)
{
    int32 i;
    int32 l_ret = INI_FAILED;
    int32 l_cfg_value;
    int32 l_ori_val;
    int8 *pst_buf = NULL;
    uint32 ul_len;
    int32 l_subchip_type = get_hi110x_subchip_type();
    bfgx_ini_cmd *p_bfgx_ini_cmd = NULL;

    switch (l_subchip_type) {
        case BOARD_VERSION_HI1103:
            p_bfgx_ini_cmd = (bfgx_ini_cmd *)&g_bfgx_ini_config_cmd_1103;
            ps_print_dbg("bfgx load ini 1103");
            break;
        case BOARD_VERSION_HI1105:
        case BOARD_VERSION_HI1106:
            p_bfgx_ini_cmd = (bfgx_ini_cmd *)&g_bfgx_ini_config_cmd_1105;
            ps_print_dbg("bfgx load ini 1105");
            break;
        default:
            ps_print_err("bfgx load ini error: No corresponding chip was found, chiptype %d", l_subchip_type);
            return INI_FAILED;
    }

    for (i = 0; i < (BFGX_BT_CUST_INI_SIZE / 4); i++) { /* 512/4ת��Ϊ�ֽ� */
        /* ���ƻ���Ŀ�б������� null ��β */
        if (p_bfgx_ini_cmd[i].name == NULL) {
            ps_print_info("bfgx ini load end count: %d", i);
            break;
        }

        l_ori_val = p_bfgx_ini_cmd[i].init_value;

        /* ��ȡini������ֵ */
        l_ret = get_cust_conf_int32(INI_MODU_DEV_BT, p_bfgx_ini_cmd[i].name, &l_cfg_value);
        if (l_ret == INI_FAILED) {
            g_bfgx_cust_ini_data[i] = l_ori_val;
            ps_print_dbg("bfgx read ini file failed cfg_id[%d],default value[%d]!", i, l_ori_val);
            continue;
        }

        g_bfgx_cust_ini_data[i] = l_cfg_value;

        ps_print_info("bfgx ini init [id:%d] [%s] changed from [%d]to[%d]",
                      i, p_bfgx_ini_cmd[i].name, l_ori_val, l_cfg_value);
    }

    pst_buf = bfgx_get_cust_ini_data_buf(&ul_len);
    if (pst_buf == NULL) {
        ps_print_err("get cust ini buf fail!");
        return INI_FAILED;
    }

    memcpy_s(pst_buf, ul_len, g_bfgx_cust_ini_data, sizeof(g_bfgx_cust_ini_data));

    return INI_SUCC;
}

/*
 * �� �� ��  : bfgx_customize_init
 * ��������  : bfgx���ƻ����ʼ������ȡini�����ļ�����ȡnv������
 * �� �� ֵ  : 0��ʾ�ɹ���-1��ʾʧ��
 */
int32 bfgx_customize_init(void)
{
    int32 ret;

    /* �������ڱ���У׼���ݵ�buffer */
    ret = cali_data_buf_malloc();
    if (ret != OAL_SUCC) {
        ps_print_err("alloc cali data buf fail\n");
        return INI_FAILED;
    }

    init_completion(&g_cali_recv_done);

    ret = bfgx_cust_ini_init();
    if (ret != OAL_SUCC) {
        ps_print_err("bfgx ini init fail!\n");
        cali_data_buf_free();
        return INI_FAILED;
    }

#ifdef HISI_NVRAM_SUPPORT
    ret = bfgx_nv_data_init();
    if (ret != OAL_SUCC) {
        ps_print_err("bfgx nv data init fail!\n");
        cali_data_buf_free();
        return INI_FAILED;
    }
#endif

    return INI_SUCC;
}

/*
 * �� �� ��  : bfgx_cali_data_init
 * ��������  : ������bt������btУ׼
 * �� �� ֵ  : 0��ʾ�ɹ���-1��ʾʧ��
 */
oal_int32 bfgx_cali_data_init(void)
{
    int32 ret = 0;
    static uint32 cali_flag = 0;
    struct ps_core_s *ps_core_d = NULL;
    struct st_bfgx_data *pst_bfgx_data = NULL;
    struct pm_top* pm_top_data = pm_get_top();

    ps_print_info("%s\n", __func__);

    /* У׼ֻ�ڿ���ʱִ��һ�Σ�OAM���ܱ�ɱ�����������Լӱ�־���� */
    if (cali_flag != 0) {
        ps_print_info("bfgx cali data has inited\n");
        return 0;
    }

    cali_flag++;

    ps_get_core_reference(&ps_core_d, BUART);
    if (unlikely(ps_core_d == NULL)) {
        ps_print_err("ps_core_d is NULL\n");
        return OAL_FAIL;
    }

    mutex_lock(&(pm_top_data->host_mutex));

    pst_bfgx_data = &ps_core_d->bfgx_info[BFGX_BT];
    if (atomic_read(&pst_bfgx_data->subsys_state) == POWER_STATE_OPEN) {
        ps_print_warning("%s has opened! ignore bfgx cali!\n", g_bfgx_subsys_name[BFGX_BT]);
        goto open_fail;
    }

    ret = hw_bfgx_open(BFGX_BT);
    if (ret != SUCC) {
        ps_print_err("bfgx cali, open bt fail\n");
        goto open_fail;
    }

    ret = wait_bfgx_cali_data();
    if (ret != SUCC) {
        goto timeout;
    }

    ret = hw_bfgx_close(BFGX_BT);
    if (ret != SUCC) {
        ps_print_err("bfgx cali, clsoe bt fail\n");
        goto close_fail;
    }

    mutex_unlock(&(pm_top_data->host_mutex));

    return ret;

timeout:
    hw_bfgx_close(BFGX_BT);
close_fail:
open_fail:
    mutex_unlock(&(pm_top_data->host_mutex));
    return ret;
}
