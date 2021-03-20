

/* 头文件包含 */
#include <linux/version.h>

#include <linux/module.h>
#include <linux/kernel.h>
#ifdef CONFIG_OF
#include <linux/of.h>
#endif
#include <linux/time.h>
#include <linux/fs.h>

#include "hisi_ini.h"
#include "board.h"
#include "oal_schedule.h"
#ifdef HISI_DTS_SUPPORT
#include "board.h"
#endif

#ifdef HISI_NVRAM_SUPPORT
#include <linux/mtd/hisi_nve_interface.h>
#endif

#include "oal_util.h"
#include "securec.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HISI_INI_C

/* 全局变量定义 */
#define CUST_PATH_INI_CONN "/data/vendor/cust_conn/ini_cfg" /* 某运营商在不同产品的差异配置 */
/* mutex for open ini file */
STATIC struct mutex g_ini_file_mutex;
#if (_PRE_TARGET_PRODUCT_TYPE_E5 == _PRE_CONFIG_TARGET_PRODUCT)
int8 g_ini_file_name[INI_FILE_PATH_LEN] = "/system/bin/wifi_hisi/cfg_e5_hisi.ini";
#elif (_PRE_TARGET_PRODUCT_TYPE_ONT == _PRE_CONFIG_TARGET_PRODUCT)
int8 g_ini_file_name[INI_FILE_PATH_LEN] = "/var/cfg_ont_hisi.ini";
#elif (_PRE_TARGET_PRODUCT_TYPE_5630HERA == _PRE_CONFIG_TARGET_PRODUCT)
int8 g_ini_file_name[INI_FILE_PATH_LEN] = "/etc/cfg_hera_hisi.ini";
#else
int8 g_ini_file_name[INI_FILE_PATH_LEN] = "/system/bin/wifi_hisi/cfg_e5_hisi.ini";
#endif
STATIC int8 g_ini_conn_file_name[INI_FILE_PATH_LEN] = {0};
#define INI_FILE_PATH (g_ini_file_name)

#ifdef _PRE_PRODUCT_HI1620S_KUNPENG
#define INI_PATH_INI_CONN "/vendor/etc/cfg_udp_1103_mpw2.ini"
#endif

#ifdef _PRE_SUSPORT_OEMINFO
#define WIFI_CALI_OEMINFO_PATH  "/data/vendor/wifi/wifi_calibrate.conf"
#endif

INI_PARAM_VERSION_STRU g_param_version = {{0}};

STATIC int64 g_ini_file_time = -1;

#ifdef _PRE_CONFIG_READ_DYNCALI_E2PROM
#define HI1103_TEL_HUAWEI_NV_WINVRAM_LENGTH 128
#define EEPROM_WIFI_CALI_PARTNAME    "WLAN"
#endif

/*
 *  Prototype    : ko_read_line
 *  Description  : read one non-empty line .ini file
 */
STATIC int32 ko_read_line(INI_FILE *fp, char *addr, int32 buf_len)
{
    int32 l_ret;
    int8 auc_tmp[MAX_READ_LINE_NUM] = {0};
    int32 cnt;
    int32 cnt_limit;

    l_ret = oal_file_read_ext(fp, fp->f_pos, auc_tmp, MAX_READ_LINE_NUM);
    if (l_ret < 0) {
#ifdef _PRE_CONFIG_INI_PRINT_ERROR_TO_DEBUG
        ini_debug("kernel_line read l_ret < 0");
#else
        ini_error("kernel_line read l_ret < 0");
#endif
        return INI_FAILED;
    } else if (l_ret == 0) {
        /* end of file */
        return 0;
    }

    cnt = 0;
    /* -2预留换行符和结束符 */
    cnt_limit = (buf_len - 2) > MAX_READ_LINE_NUM ? MAX_READ_LINE_NUM : (buf_len - 2);
    while ((cnt < cnt_limit) && (auc_tmp[cnt] != '\n')) {
        *addr++ = auc_tmp[cnt++];
    }
    *addr++ = '\n';
    *addr = '\0';

    /* change file pos to next line */
    fp->f_pos += (cnt + 1);

    return l_ret;
}

STATIC INI_FILE *ini_file_open(int8 *filename, int8 *para)
{
    INI_FILE *fp = NULL;
    mm_segment_t fs;

    unref_param(para);

    fs = get_fs();
    set_fs(KERNEL_DS);
    fp = (INI_FILE *)filp_open(filename, O_RDONLY, 0);
    set_fs(fs);
    if (oal_is_err_or_null(fp)) {
        fp = NULL;
    }

    return fp;
}

STATIC int32 ini_file_close(INI_FILE *fp)
{
    mm_segment_t fs;

    fs = get_fs();
    set_fs(KERNEL_DS);
    filp_close(fp, NULL);
    set_fs(fs);
    fp = NULL;
    return INI_SUCC;
}

STATIC bool ini_file_exist(int8 *file_path)
{
    INI_FILE *fp = NULL;

    if (file_path == NULL) {
        ini_error("para file_path is NULL\n");
        return false;
    }

    fp = ini_file_open(file_path, "rt");
    if (fp == NULL) {
        ini_debug("%s not exist\n", file_path);
        return false;
    }

    ini_file_close(fp);

    ini_debug("%s exist\n", file_path);

    return true;
}

/*
 *  Prototype    : ini_file_seek
 *  Description  : set f_pos in curr file ptr
 */
STATIC int32 ini_file_seek(INI_FILE *fp, long fp_pos)
{
    fp->f_pos += fp_pos;
    return INI_SUCC;
}

STATIC int32 ini_readline_func(INI_FILE *fp, int8 *rd_buf, uint32 buf_len)
{
    int8 auc_tmp[MAX_READ_LINE_NUM];
    int32 ret;

    memset_s(auc_tmp, sizeof(auc_tmp), 0, sizeof(auc_tmp));
    ret = ko_read_line(fp, auc_tmp, sizeof(auc_tmp));
    if (ret == INI_FAILED) {
#ifdef _PRE_CONFIG_INI_PRINT_ERROR_TO_DEBUG
        ini_debug("ko_read_line failed!!!");
#else
        ini_error("ko_read_line failed!!!");
#endif
        return INI_FAILED;
    } else if (ret == 0) {
#ifdef _PRE_CONFIG_INI_PRINT_ERROR_TO_DEBUG
        ini_debug("end of .ini file!!!");
#else
        ini_error("end of .ini file!!!");
#endif
        return INI_FAILED;
    }

    ret = strcpy_s(rd_buf, buf_len, auc_tmp);
    if (ret != EOK) {
        ini_error("ini_readline_func: strcpy_s failed.");
        return INI_FAILED;
    }

    return INI_SUCC;
}

STATIC int32 ini_check_str(INI_FILE *fp, int8 *auc_tmp, uint32 buf_len, const char *puc_var)
{
    uint16 auc_len;
    uint16 curr_var_len;
    uint16 search_var_len;

    if ((fp == NULL) || (puc_var == NULL) || (puc_var[0] == '\0')) {
        ini_error("check if puc_var is NULL or blank");
        return INI_FAILED;
    }

    do {
        auc_len = (uint16)strlen(auc_tmp);
        curr_var_len = 0;

        while ((curr_var_len < buf_len) && (auc_tmp[curr_var_len] != '\r') &&
               (auc_tmp[curr_var_len] != '\n') && (auc_tmp[curr_var_len] != 0)) {
            curr_var_len++;
        }

        if ((auc_tmp[0] == '#') || (auc_tmp[0] == ' ') || (auc_tmp[0] == '\n') || (auc_tmp[0] == '\r')) {
            break;
        }
        search_var_len = (uint16)strlen(puc_var);
        if (search_var_len > curr_var_len) {
            break;
        }
        if (strncmp(auc_tmp, puc_var, search_var_len) == 0) {
            return INI_SUCC;
        } else {
            break;
        }
    } while (0);

    if (ini_file_seek(fp, -auc_len) == INI_FAILED) {
        ini_error("file seek failed!!!");
        return INI_FAILED;
    }
    if (ini_file_seek(fp, curr_var_len + 1) == INI_FAILED) {
        ini_error("file seek failed!!!");
        return INI_FAILED;
    }
    if (((curr_var_len + 1) < buf_len) && (auc_tmp[curr_var_len + 1] == '\n')) {
        if (ini_file_seek(fp, 1) == INI_FAILED) {
            ini_error("file seek failed!!!");
            return INI_FAILED;
        }
    }

    return INI_FAILED;
}

/*
 *  Prototype    : ini_check_value
 *  Description  : check the value of config behind =
 */
STATIC int32 ini_check_value(int8 *puc_value, uint32 value_len)
{
    uint32 cnt;
    const uint32 ul_value_min_len = 2;

    if (value_len < ul_value_min_len) {
        ini_error("ini_check_value fail, puc_value length %u < 2(min len)\n", value_len);
        return INI_FAILED;
    }

    if (puc_value[0] == ' ' || puc_value[0] == '\r' || puc_value[0] == '\n' || puc_value[value_len - 1] == ' ') {
        puc_value[0] = '\0';
        ini_error("::%s has blank space or is blank::", puc_value);
        return INI_FAILED;
    }

    /* 替换尾部空格和换行符为'\0' */
    cnt = value_len - 1;
    while ((cnt != 0) && (puc_value[cnt] == '\n' || puc_value[cnt] == '\r' || puc_value[cnt] == ' ')) {
        puc_value[cnt--] = '\0';
    }

    return INI_SUCC;
}

STATIC int32 is_modu_exist(INI_FILE *fp, const char *modu_name)
{
    int32 ret = INI_FAILED;
    int8 auc_tmp[MAX_READ_LINE_NUM] = {0};
    /* find the value of mode var, such as ini_wifi_mode
     * every mode except PLAT mode has only one mode var */
    for (;;) {
        ret = ini_readline_func(fp, auc_tmp, sizeof(auc_tmp));
        if (ret == INI_FAILED) {
#ifdef _PRE_CONFIG_INI_PRINT_ERROR_TO_DEBUG
            ini_debug("have end of .ini file!!!");
#else
            ini_error("have end of .ini file!!!");
#endif
            return INI_FAILED;
        }

        if (strstr(auc_tmp, INI_STR_DEVICE_BFG_PLAT) != NULL) {
#ifdef _PRE_CONFIG_INI_PRINT_ERROR_TO_DEBUG
            ini_debug("not find %s!!!", modu_name);
#else
            ini_error("not find %s!!!", modu_name);
#endif
            return INI_FAILED;
        }

        ret = ini_check_str(fp, auc_tmp, sizeof(auc_tmp), modu_name);
        if (ret == INI_SUCC) {
            ini_debug("have found %s", modu_name);
            break;
        } else {
            continue;
        }
    }

    return ret;
}

/*
 *  Prototype    : ini_find_modu
 *  Description  : find moduler by mode value puc_value
 *
 */
STATIC int32 ini_find_modu(INI_FILE *fp, int32 tag_index, int8 *puc_var, int8 *puc_value)
{
    int8 auc_modu[INI_STR_MODU_LEN] = {0};
    int32 ret = INI_FAILED;

    unref_param(puc_var);
    unref_param(puc_value);

    switch (tag_index) {
        case INI_MODU_WIFI:
            ret = strcpy_s(auc_modu, sizeof(auc_modu), INI_STR_WIFI_NORMAL);
            break;
        case INI_MODU_POWER_FCC:
            ret = strcpy_s(auc_modu, sizeof(auc_modu), INI_STR_POWER_FCC);
            break;
        case INI_MODU_POWER_ETSI:
            ret = strcpy_s(auc_modu, sizeof(auc_modu), INI_STR_POWER_ETSI);
            break;
        case INI_MODU_POWER_JP:
            ret = strcpy_s(auc_modu, sizeof(auc_modu), INI_STR_POWER_JP);
            break;
        case INI_MODU_GNSS:
            ret = strcpy_s(auc_modu, sizeof(auc_modu), INI_STR_GNSS_NORMAL);
            break;
        case INI_MODU_BT:
            ret = strcpy_s(auc_modu, sizeof(auc_modu), INI_STR_BT_NORMAL);
            break;
        case INI_MODU_FM:
            ret = strcpy_s(auc_modu, sizeof(auc_modu), INI_STR_FM_NORMAL);
            break;
        case INI_MODU_PLAT:
            ret = strcpy_s(auc_modu, sizeof(auc_modu), INI_STR_PLAT);
            break;
        case INI_MODU_HOST_VERSION:
            ret = strcpy_s(auc_modu, sizeof(auc_modu), INT_STR_HOST_VERSION);
            break;
        case INI_MODU_WIFI_MAC:
            ret = strcpy_s(auc_modu, sizeof(auc_modu), INI_STR_WIFI_MAC);
            break;
        case INI_MODU_COEXIST:
            ret = strcpy_s(auc_modu, sizeof(auc_modu), INI_STR_COEXIST);
            break;
        case INI_MODU_DEV_WIFI:
            ret = strcpy_s(auc_modu, sizeof(auc_modu), INI_STR_DEVICE_WIFI);
            break;
        case INI_MODU_DEV_GNSS:
            ret = strcpy_s(auc_modu, sizeof(auc_modu), INI_STR_DEVICE_GNSS);
            break;
        case INI_MODU_DEV_BT:
            ret = strcpy_s(auc_modu, sizeof(auc_modu), INI_STR_DEVICE_BT);
            break;
        case INI_MODU_DEV_FM:
            ret = strcpy_s(auc_modu, sizeof(auc_modu), INI_STR_DEVICE_FM);
            break;
        case INI_MODU_DEV_BFG_PLAT:
            ret = strcpy_s(auc_modu, sizeof(auc_modu), INI_STR_DEVICE_BFG_PLAT);
            break;
        default:
            ini_error("not suport tag type:%x!!!", tag_index);
            break;
    }
    if (ret != EOK) {
        oam_error_log2(0, OAM_SF_INI, "ini_find_modu:find failed tag type:%x!!! errno:%d", tag_index, ret);
        return INI_FAILED;
    }

    return is_modu_exist(fp, auc_modu);
}

/*
 *  Prototype    : ini_find_var
 *  Description  : find difference mode variable value, and return puc_value
 */
STATIC int32 ini_find_var(INI_FILE *fp, int32 tag_index, int8 *puc_var, int8 *puc_value, uint32 size)
{
    int32 ret;
    int8 auc_tmp[MAX_READ_LINE_NUM + 1] = {0};
    size_t search_var_len;

    /* find the modu of var, such as [HOST_WIFI_NORMAL] of wifi moduler */
    ret = ini_find_modu(fp, tag_index, puc_var, puc_value);
    if (ret == INI_FAILED) {
        return INI_FAILED;
    }

    /* find the var in modu, such as [HOST_WIFI_NORMAL] of wifi moduler */
    for (;;) {
        ret = ini_readline_func(fp, auc_tmp, sizeof(auc_tmp));
        if (ret == INI_FAILED) {
#ifdef _PRE_CONFIG_INI_PRINT_ERROR_TO_DEBUG
            ini_debug("have end of .ini file!!!");
#else
            ini_error("have end of .ini file!!!");
#endif
            return INI_FAILED;
        }

        if (auc_tmp[0] == '[') {
#ifdef _PRE_CONFIG_INI_PRINT_ERROR_TO_DEBUG
            ini_debug("not find %s!!!, check if var in correct mode", puc_var);

#else
            ini_debug("not find %s!!!, check if var in correct mode", puc_var);
#endif
            return INI_FAILED;
        }

        search_var_len = strlen(puc_var);
        ret = ini_check_str(fp, auc_tmp, sizeof(auc_tmp), puc_var);
        if ((ret == INI_SUCC) && (auc_tmp[search_var_len] == '=')) {
            if (strncpy_s(puc_value, size, &auc_tmp[search_var_len + 1], size - 1)) {
                oam_error_log0(0, OAM_SF_INI, "ini_find_var:strncpy_s space is not enough");
                continue;
            }
            break;
        } else {
            continue;
        }
    }

    return INI_SUCC;
}

int32 find_download_channel(uint8 *buff, uint32 buf_len, int8 *puc_var)
{
    INI_FILE *fp = NULL;
    int8 version_buff[DOWNLOAD_CHANNEL_LEN] = {0};
    int32 l_ret;

    ini_mutex_lock(&g_ini_file_mutex);
    ini_info("ini file_name is %s", INI_FILE_PATH);
    fp = ini_file_open(INI_FILE_PATH, "rt");
    if (fp == 0) {
        fp = NULL;
        ini_error("open %s failed!!!", INI_FILE_PATH);
        goto open_ini_file_fail;
    }

    /* find wlan download channel */
    l_ret = ini_find_var(fp, INI_MODU_PLAT, puc_var, version_buff, DOWNLOAD_CHANNEL_LEN);
    if (l_ret == INI_FAILED) {
        version_buff[0] = '\0';
        goto read_ini_var_fail;
    }
    if (ini_check_value(version_buff, strlen(version_buff)) == INI_FAILED) {
        goto read_ini_var_fail;
    }
    if (strcpy_s((int8 *)buff, buf_len, version_buff) != EOK) {
        oam_error_log0(0, OAM_SF_INI, "find_download_channel:strcpy_s space is not enough");
        goto read_ini_var_fail;
    }
    buff[buf_len - 1] = '\0';

    fp->f_pos = 0;
    ini_file_close(fp);
    ini_mutex_unlock(&g_ini_file_mutex);
    return INI_SUCC;

read_ini_var_fail:
    fp->f_pos = 0;
    ini_file_close(fp);
open_ini_file_fail:
    ini_mutex_unlock(&g_ini_file_mutex);
    return INI_FAILED;
}

#ifdef _PRE_SUSPORT_OEMINFO
STATIC int32 get_wifi_calibrate_param(uint32 datalen, uint32 offset, uint8 *cali_param)
{
    int32 ret;
    int wifi_cal_len = 0;
    uint32 i;
    oal_int8 data[HI1103_TEL_HUAWEI_NV_WINVRAM_LENGTH];
    oal_file_stru *file = NULL;

    ret = memset_s(data, HI1103_TEL_HUAWEI_NV_WINVRAM_LENGTH, 0, HI1103_TEL_HUAWEI_NV_WINVRAM_LENGTH);
    if (ret != EOK) {
        ini_error("memset wifi cali data failed.\n");
        return INI_FAILED;
    }

    file = oal_file_open_readonly(WIFI_CALI_OEMINFO_PATH);
    if (file == NULL) {
        return INI_FAILED;
    }
    oal_file_read_ext(file, (loff_t)offset, data, datalen);
    oal_file_close(file);

    for (i = 0; i < datalen; i++) {
        if (data[i] == '\0') {
            wifi_cal_len = i + 1;
            break;
        }
    }
    ret = memcpy_s(cali_param, wifi_cal_len, data, wifi_cal_len);
    if (ret != EOK) {
        ini_error("memcpy wifi cal data failed\n");
        return INI_FAILED;
    }
    return INI_SUCC;
}
#endif

STATIC int32 ini_find_var_value_by_path(int8 *path, int32 tag_index, int8 *puc_var, int8 *puc_value, uint32 size)
{
    INI_FILE *fp = NULL;

#ifdef INI_TIME_TEST
    struct timeval tv0;
    struct timeval tv1;
#endif

    int32 l_ret;

    if (puc_var == NULL || puc_var[0] == '\0' || puc_value == NULL) {
        ini_error("check if puc_var and puc_value is NULL or blank");
        return INI_FAILED;
    }

#ifdef INI_TIME_TEST
    do_gettimeofday(&tv0);
#endif

    ini_mutex_lock(&g_ini_file_mutex);

    fp = ini_file_open(path, "rt");
    if (fp == 0) {
        ini_error("open %s failed!!!", path);
        ini_mutex_unlock(&g_ini_file_mutex);
        return INI_FAILED;
    }

    /* find puc_var in .ini return puc_value */
    l_ret = ini_find_var(fp, tag_index, puc_var, puc_value, size);
    if (l_ret == INI_FAILED) {
        puc_value[0] = '\0';
        ini_file_close(fp);
        ini_mutex_unlock(&g_ini_file_mutex);
        return INI_FAILED;
    }

#ifdef INI_TIME_TEST
    do_gettimeofday(&tv1);
    ini_debug("time take = %ld", (tv1.tv_sec - tv0.tv_sec) * 1000 + (tv1.tv_usec - tv0.tv_usec) / 1000);
#endif

    ini_file_close(fp);
    ini_mutex_unlock(&g_ini_file_mutex);

    /* check blank space of puc_value */
    if (ini_check_value(puc_value, strlen(puc_value)) == INI_SUCC) {
        ini_debug("::return %s:%s::", puc_var, puc_value);
        return INI_SUCC;
    }

    return INI_FAILED;
}

/*
 *  Prototype    : ini_find_var_value
 *  Description  : get var value from .ini file
 */
STATIC int32 ini_find_var_value(int32 tag_index, int8 *puc_var, int8 *puc_value, uint32 size)
{
    /* read spec if exist */
    if (ini_file_exist(g_ini_conn_file_name)) {
        if (ini_find_var_value_by_path(g_ini_conn_file_name, tag_index, puc_var, puc_value, size) == INI_SUCC) {
            return INI_SUCC;
        }
    }

    if (ini_file_exist(INI_FILE_PATH) == 0) {
        ini_error(" %s not exist!!!", INI_FILE_PATH);
        chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_PLAT, CHR_LAYER_DRV,
                             CHR_PLT_DRV_EVENT_INI, CHR_PLAT_DRV_ERROR_INI_MISS);
        return INI_FAILED;
    }

    return ini_find_var_value_by_path(INI_FILE_PATH, tag_index, puc_var, puc_value, size);
}

#ifdef HISI_NVRAM_SUPPORT
/*
 *  Prototype    : read_conf_from_nvram
 *  Description  : read nv buff from nvram
 *
 */
int32 read_conf_from_nvram(uint8 *pc_out, uint32 size, uint32 nv_number, const char *nv_name)
{
    struct hisi_nve_info_user info;
    int32 ret;

    memset_s(&info, sizeof(info), 0, sizeof(info));
    memset_s(pc_out, size, 0, size);
    if (strcpy_s(info.nv_name, sizeof(info.nv_name), nv_name) != EOK) {
        ini_error("read nvm failed nv_name size[%lu] less than input[%s]", sizeof(info.nv_name), nv_name);
        oam_error_log1(0, OAM_SF_INI, "read_conf_from_nvram:read nvm failed nv_name size[%lu] less than input",
                       sizeof(info.nv_name));
        return INI_FAILED;
    }
    info.nv_name[strlen(HISI_CUST_NVRAM_NAME)] = '\0';
    info.nv_number = nv_number;
    info.valid_size = HISI_CUST_NVRAM_LEN;
    info.nv_operation = HISI_CUST_NVRAM_READ;

    ret = hisi_nve_direct_access(&info);
    if (size > sizeof(info.nv_data) || size <= OAL_STRLEN(info.nv_data)) {
        ini_error("read nvm item[%s] fail, lenth[%d] longer than input[%d]",
                  nv_name, (uint32)OAL_STRLEN(info.nv_data), size);
        return INI_FAILED;
    }
    if (ret == INI_SUCC) {
        if (memcpy_s(pc_out, size, info.nv_data, sizeof(info.nv_data)) != EOK) {
            ini_error("read nvm{%s}lenth[%d] longer than input[%d]",
                      info.nv_data, (uint32)OAL_STRLEN(info.nv_data), size);
            oam_error_log2(0, OAM_SF_INI, "read_conf_from_nvram:read nvm lenth[%d] longer than input[%d]",
                           (uint32)OAL_STRLEN(info.nv_data), size);
            return INI_FAILED;
        }
        oal_io_print("read_conf_from_nvram::nvram id[%d] nv name[%s] get data{%s}, size[%d]\r\n!",
                     nv_number, nv_name, info.nv_data, size);
    } else {
        ini_error("read nvm [%d] %s failed", nv_number, nv_name);
        return INI_FAILED;
    }

    return INI_SUCC;
}

/*
 *  Prototype    : write_conf_to_nvram
 *  Description  : change value and write to file
 */
int32 write_conf_to_nvram(int8 *name, int8 *pc_arr)
{
    struct hisi_nve_info_user info;
    int32 ret;

    unref_param(name);

    memset_s(&info, sizeof(info), 0, sizeof(info));
    if (strcpy_s(info.nv_name, sizeof(info.nv_name), HISI_CUST_NVRAM_NAME) != EOK) {
        oam_error_log1(0, OAM_SF_INI,
                       "write_conf_to_nvram:write_conf_to_nvram failed nv_name size[%lu] less than input[WINVRAM]",
                       sizeof(info.nv_name));
        return INI_FAILED;
    }
    info.nv_name[sizeof(info.nv_name) - 1] = '\0';
    info.nv_number = HISI_CUST_NVRAM_NUM;
    info.valid_size = HISI_CUST_NVRAM_LEN;
    info.nv_operation = HISI_CUST_NVRAM_WRITE;
    if (memcpy_s(info.nv_data, sizeof(info.nv_data), pc_arr, HISI_CUST_NVRAM_LEN) != EOK) {
        oam_error_log0(0, OAM_SF_INI, "write_conf_to_nvram:write nvm[memcpy_s] failed");
        return INI_FAILED;
    }

    ret = hisi_nve_direct_access(&info);
    if (ret < -1) {
        ini_error("write nvm failed");
        return INI_FAILED;
    }

    return INI_SUCC;
}
#else

#ifndef _PRE_CONFIG_READ_DYNCALI_E2PROM
int32 read_conf_from_nvram(uint8 *pc_out, uint32 size, uint32 nv_number,  const char *nv_name)
{
    char buf[128];
    if (sprintf_s(buf, sizeof(buf), "%s=0", nv_name) < 0) {
        ini_error("sprintf_s failed\n");
        return INI_FAILED;
    }

    if (memcpy_s(pc_out, size, buf, size) != EOK) {
        ini_error("memcpy_s failed\n");
        return INI_FAILED;
    }
    return INI_SUCC;
}
#endif

int32 write_conf_to_nvram(int8 *name, int8 *pc_arr)
{
    return INI_SUCC;
}

#ifdef _PRE_SUSPORT_OEMINFO
int32 read_conf_from_oeminfo(uint8 *pc_out, uint32 size, uint32 offset)
{
    uint8 eep_buff[HI1103_TEL_HUAWEI_NV_WINVRAM_LENGTH + 1] = {0};
    int32 ret = 0;
    uint32 i = 0;

    ret = memset_s((void *)pc_out, size, 0, size);
    if (ret != EOK) {
        ini_error("memset_s failed %d", ret);
        return INI_FAILED;
    }

    ret = get_wifi_calibrate_param(HI1103_TEL_HUAWEI_NV_WINVRAM_LENGTH, offset, eep_buff);
    if (ret < 0) {
        chr_exception_report(CHR_READ_EEPROM_ERROR_EVENTID, CHR_SYSTEM_WIFI, CHR_LAYER_DRV,
                             CHR_PLT_DRV_EVENT_INIT, CHR_PLAT_DRV_ERROR_EEPROM_READ_INIT);
        ini_error("oeminfo read failed %d", ret);
        return INI_FAILED;
    }

    for (i = 0; i < HI1103_TEL_HUAWEI_NV_WINVRAM_LENGTH; i++) {
        if (eep_buff[i] == ';') {
            eep_buff[i + 1] = 0;
            break;
        } else if (eep_buff[i] == 0xff) {
            eep_buff[i] = 0;
            break;
        }
    }
    ret = memcpy_s(pc_out, size, eep_buff, oal_min(size, i));
    if (ret != EOK) {
        ini_error("memcpy_s failed %d", ret);
        return INI_FAILED;
    }
    return INI_SUCC;
}
#endif

int32 read_conf_from_eeprom(uint8 *pc_out, uint32 size, uint32 offset)
{
    uint8 eep_buff[HI1103_TEL_HUAWEI_NV_WINVRAM_LENGTH + 1] = {0};
    int32 ret;
    uint32 i = 0;

    ret = memset_s((void *)pc_out, size, 0, size);
    if (ret != EOK) {
        ini_error("memset_s failed %d", ret);
        return INI_FAILED;
    }

    ret = DrvEepromRead(EEPROM_WIFI_CALI_PARTNAME, offset,  eep_buff,  HI1103_TEL_HUAWEI_NV_WINVRAM_LENGTH);
    if (ret < 0) {
        chr_exception_report(CHR_READ_EEPROM_ERROR_EVENTID, CHR_SYSTEM_WIFI, CHR_LAYER_DRV,
                             CHR_PLT_DRV_EVENT_INIT, CHR_PLAT_DRV_ERROR_EEPROM_READ_INIT);
        ini_error("eeprom read failed %d", ret);
        return INI_FAILED;
    }

    for (i = 0; i < HI1103_TEL_HUAWEI_NV_WINVRAM_LENGTH; i++) {
        if (eep_buff[i] == ';') {
            eep_buff[i + 1] = 0;
            break;
        } else if (eep_buff[i] == 0xff) {
            eep_buff[i] = 0;
            break;
        }
    }
    ret = memcpy_s(pc_out, size, eep_buff, oal_min(size, i));
    if (ret != EOK) {
        ini_error("memcpy_s failed %d", ret);
        return INI_FAILED;
    }
    return INI_SUCC;
}

#endif

/*
 *  Prototype    : get_cust_conf_string
 *  Description  : get config form *.ini file or dts(kernel)
 */
int32 get_cust_conf_string(int32 tag_index, int8 *puc_var, int8 *puc_value, uint32 size)
{
    memset_s(puc_value, size, 0, size);
    return ini_find_var_value(tag_index, puc_var, puc_value, size);
}

int32 set_cust_conf_string(int32 tag_index, int8 *name, int8 *var)
{
    int32 ret;

    if (tag_index != CUST_MODU_NVRAM) {
        ini_error("NOT SUPPORT MODU TO WRITE");
        return INI_FAILED;
    }

#ifdef HISI_NVRAM_SUPPORT
    ret = write_conf_to_nvram(name, var);
#else
    ret = INI_FAILED;
#endif
    return ret;
}

/*
 *  Prototype    : get_cust_conf_int32
 *  Description  : get config form *.ini file or dts(kernel)
 */
int32 get_cust_conf_int32(int32 tag_index, int8 *puc_var, int32 *puc_value)
{
    int32 ret;
    int8 out_str[INI_READ_VALUE_LEN] = {0};

    ret = ini_find_var_value(tag_index, puc_var, out_str, sizeof(out_str));
    if (ret < 0) {
        /* ini_find_var_value has error log, delete this log */
        ini_debug("cust modu didn't get var of %s.", puc_var);
        return INI_FAILED;
    }

    if (!strncmp(out_str, "0x", strlen("0x")) || !strncmp(out_str, "0X", strlen("0X"))) {
        ini_debug("get hex of:%s.", puc_var);
        ret = sscanf_s(out_str, "%x", puc_value);
    } else {
        ret = sscanf_s(out_str, "%d", puc_value);
    }

    if (ret <= 0) {
        ini_error("%s trans to int failed", puc_var);
        return INI_FAILED;
    }

    ini_debug("conf %s get vale:%d", puc_var, *puc_value);

    return INI_SUCC;
}

STATIC int32 get_ini_file(int8 *file_path, INI_FILE **fp)
{
    if (file_path == NULL) {
        ini_info("para file_path is NULL\n");
        return INI_FAILED;
    }

    *fp = ini_file_open(file_path, "rt");
    if (*fp == NULL) {
        ini_info("inifile %s not exist\n", file_path);
        return INI_FAILED;
    }

    return INI_SUCC;
}

/*
 *  Prototype    : ini_file_check_timespec
 *  Description  : get *.ini file timespec
 */
STATIC int32 ini_file_check_timespec(INI_FILE *fp)
{
    if (fp == NULL) {
        ini_error("para file is NULL\n");
        return INI_FILE_TIMESPEC_UNRECONFIG;
    }

    if (fp->f_path.dentry == NULL) {
        ini_error("file dentry is NULL\n");
        return INI_FILE_TIMESPEC_UNRECONFIG;
    }

    if (g_ini_file_time != inf_file_get_ctime(fp->f_path.dentry)) {
        ini_info("ini_file time_secs changed from [%ld]to[%ld]\n",
                 g_ini_file_time, (int64)inf_file_get_ctime(fp->f_path.dentry));
        g_ini_file_time = inf_file_get_ctime(fp->f_path.dentry);

        return INI_FILE_TIMESPEC_RECONFIG;
    } else {
        ini_info("ini file is not upadted time_secs[%ld]\n", g_ini_file_time);
        return INI_FILE_TIMESPEC_UNRECONFIG;
    }
}

/*
 *  Prototype    : ini_file_check_conf_update
 *  Description  : check *.ini file is updated or not
 */
int32 ini_file_check_conf_update(void)
{
    INI_FILE *fp = NULL;
    int32 ret;

    /* read spec if exist */
    if ((get_ini_file(g_ini_conn_file_name, &fp) == INI_SUCC) &&
        (ini_file_check_timespec(fp) == INI_FILE_TIMESPEC_RECONFIG)) {
        ini_info("%s ini file is updated\n", g_ini_conn_file_name);
        ret = INI_FILE_TIMESPEC_RECONFIG;
    } else if ((get_ini_file(INI_FILE_PATH, &fp) == INI_SUCC) &&
               (ini_file_check_timespec(fp) == INI_FILE_TIMESPEC_RECONFIG)) {
        ini_info("%s ini file is updated\n", INI_FILE_PATH);
        ret = INI_FILE_TIMESPEC_RECONFIG;
    } else {
        ini_info("no ini file is updated\n");
        ret = INI_FILE_TIMESPEC_UNRECONFIG;
    }

    if (fp != NULL) {
        ini_file_close(fp);
    }

    return ret;
}

#ifdef HISI_DTS_SUPPORT
int32 get_ini_file_name_from_dts(int8 *dts_prop, int8 *prop_value, uint32 size)
{
#ifdef _PRE_CONFIG_USE_DTS
    return g_st_board_info.bd_ops.get_ini_file_name_from_dts(dts_prop, prop_value, size);
#endif
    return INI_SUCC;
}
#endif

/*
 * 函 数 名 : bin_mem_check
 * 功能描述  : 二进制内存检查
 * 输入参数  : int8 *pc_dest需要对比的内存地址
 *             int8 *pc_src比较的内存地址
 *             int16 us_lenth需要比较的内存长度
 */
STATIC int32 bin_mem_check(int8 *pc_dest, const int8 *pc_src, uint16 us_lenth)
{
    int16 loop;
    if (pc_dest == NULL || pc_src == NULL) {
        ini_error("pointer is NULL!");
        return INI_FAILED;
    }

    if (us_lenth == 0) {
        return INI_SUCC;
    }

    for (loop = 0; loop < us_lenth; loop++) {
        if (pc_dest[loop] != pc_src[loop]) {
            return INI_FAILED;
        }
    }
    return INI_SUCC;
}

STATIC int8 *search_target_str(int8* read_buf, uint32 buf_len, const int8 *target_str)
{
    int32 loop;
    uint16 target_len = OAL_STRLEN(target_str);

    for (loop = 0; loop < buf_len - target_len; loop++) {
        /* 判断首尾减少bin_mem_check调用次数 */
        if (!bin_mem_check(&read_buf[loop], target_str, target_len)) {
            return &read_buf[loop];
        }
    }
    return NULL;
}


/*
 * 函 数 名  : get_str_from_file
 * 功能描述  : 从文件中获取固定字符串，保存到另外的位置
 * 输入参数  : char *file_path 文件绝对路径
 *             const int8 *target_str 查找内容
 */
int8 *get_str_from_file(int8 *pc_file_path, const int8 * target_str)
{
    INI_FILE *fp = NULL;
    int32 read_bytes, str_len;
    int8 read_buf[INI_KERNEL_READ_LEN + 1] = {0};
    int8 *version_str = NULL;
    uint8 target_len;
    ini_info("%s", __func__);

    if (unlikely(pc_file_path == NULL || target_str == NULL)) {
        ini_error("arg is NULL!");
        return NULL;
    }

    target_len = OAL_STRLEN(target_str);
    if (target_len == 0) {
        ini_error("target_str is empty!");
        return NULL;
    }

    fp = ini_file_open(pc_file_path, "rt");
    if (unlikely(fp == NULL)) {
        ini_error("open file %s fail!", pc_file_path);
        return NULL;
    }
    ini_info("open file %s success to find str \"%s\"!", pc_file_path, target_str);

    /* 由于每次比较都会留uc_str_check_len不比较所以不是0 */
    do {
        read_bytes = oal_file_read_ext(fp, fp->f_pos, read_buf, INI_KERNEL_READ_LEN);
        if (read_bytes <= target_len) {
            ini_file_close(fp);
            ini_error("cann't find device sw version string in %s!", pc_file_path);
            return NULL;
        }

        read_buf[read_bytes] = '\0';
        version_str = search_target_str(read_buf, read_bytes, target_str);
        if (version_str != NULL) {
            fp->f_pos += (version_str - read_buf);
            fp->f_pos += target_len;
            memset_s(read_buf, sizeof(read_buf), 0, sizeof(read_buf));
            /* 读取到‘\n’或者最大192B数据到ac_read_buf */
            ko_read_line(fp, read_buf, sizeof(read_buf));
            break;
        } else {
            fp->f_pos += (read_bytes - target_len);
        }
    } while (1);

    str_len = OAL_STRLEN(read_buf);
    version_str = (int8 *)kmalloc(str_len + 1, GFP_KERNEL);
    if (unlikely(version_str == NULL)) {
        ini_error("find device sw version, but memory alloc fail!");
    }else {
        memcpy_s(version_str, str_len + 1, read_buf, str_len + 1);
        ini_info("find device sw version :%s", version_str);
    }

    ini_file_close(fp);
    return version_str;
}

#ifdef _PRE_PRODUCT_HI1620S_KUNPENG
int ini_cfg_init(void)
{
    ini_info("hi110x ini config search init!\n");

    snprintf_s(g_ini_file_name, sizeof(g_ini_file_name), sizeof(g_ini_file_name) - 1, "%s", INI_PATH_INI_CONN);
    oal_io_print("ini_file_name@ %s\n", g_ini_file_name);
    g_st_board_info.ini_file_name = g_ini_file_name;

    return INI_SUCC;
}
#else

int ini_cfg_init(void)
{
#ifdef HISI_DTS_SUPPORT
    int32 ret;
    int8 auc_dts_ini_path[INI_FILE_PATH_LEN] = {0};
#endif

    ini_info("hi110x ini config search init!\n");

#ifdef HISI_DTS_SUPPORT
    ret = get_ini_file_name_from_dts(PROC_NAME_INI_FILE_NAME, auc_dts_ini_path, sizeof(auc_dts_ini_path));
    if (ret < 0) {
        ini_error("can't find dts proc %s\n", PROC_NAME_INI_FILE_NAME);
        return INI_FAILED;
    }
#endif
    ini_init_mutex(&g_ini_file_mutex);

#ifdef HISI_DTS_SUPPORT
    if (snprintf_s(g_ini_file_name, sizeof(g_ini_file_name),
                   sizeof(g_ini_file_name) - 1, "%s", auc_dts_ini_path) < 0) {
        ini_error("space is not enough\n");
        return INI_FAILED;
    }
    g_st_board_info.ini_file_name = g_ini_file_name;
    /* Note:"symbol snprintf()"has arg.count conflict(5 vs 4) */
    /*lint -e515*/
#endif
    if (snprintf_s(g_ini_conn_file_name, sizeof(g_ini_conn_file_name),
                   sizeof(g_ini_conn_file_name) - 1, "%s", CUST_PATH_INI_CONN) < 0) {
        ini_error("space is not enough\n");
        return INI_FAILED;
    }
    oal_io_print("ini_file_name@%s\n", g_ini_file_name);

#ifdef HISI_DTS_SUPPORT
    /*lint +e515*/
    ini_info("%s@%s\n", PROC_NAME_INI_FILE_NAME, g_ini_file_name);
#else
    ini_info("ini_file_name@%s\n", g_ini_file_name);
#endif

    // 读取共时钟配置
    read_tcxo_dcxo_ini_file();
    return INI_SUCC;
}
#endif

void ini_cfg_exit(void)
{
    ini_info("hi110x ini config search exit!\n");
}

