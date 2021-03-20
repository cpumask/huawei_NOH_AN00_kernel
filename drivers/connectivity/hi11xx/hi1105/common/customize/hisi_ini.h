

#ifndef __HISI_INI_H__
#define __HISI_INI_H__

/* 其他头文件包含 */
#include "hw_bfg_ps.h"
#include "bfgx_user_ctrl.h"
#include "plat_type.h"

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 35))
#ifdef _PRE_CONFIG_READ_DYNCALI_E2PROM

#else
#define HISI_NVRAM_SUPPORT
#endif
#define HISI_DTS_SUPPORT
#endif

/* 宏定义 */
#define INI_TIME_TEST

#define ini_min(_A, _B)        (((_A) < (_B)) ? (_A) : (_B))
#define ini_debug(fmt, arg...) \
    do {                                                                                   \
        if (g_hi110x_loglevel >= PLAT_LOG_DEBUG) {                                         \
            /*lint -e515*/                                                                 \
            printk(KERN_INFO "INI_DRV:D]%s:%d]" fmt "\n", __FUNCTION__, __LINE__, ##arg); \
            /*lint +e515*/                                                                 \
        }                                                                                  \
    } while (0)

#define ini_info(fmt, arg...)                                                              \
    do {                                                                                   \
        if (g_hi110x_loglevel >= PLAT_LOG_INFO) {                                          \
            /*lint -e515*/                                                                 \
            printk(KERN_INFO "INI_DRV:D]%s:%d]" fmt "\n", __FUNCTION__, __LINE__, ##arg); \
            /*lint +e515*/                                                                 \
        }                                                                                  \
    } while (0)

#define ini_warning(fmt, arg...)                                                             \
    do {                                                                                     \
        if (g_hi110x_loglevel >= PLAT_LOG_WARNING) {                                         \
            /*lint -e515*/                                                                   \
            printk(KERN_WARNING "INI_DRV:W]%s:%d]" fmt "\n", __FUNCTION__, __LINE__, ##arg); \
            /*lint +e515*/                                                                   \
        }                                                                                    \
    } while (0)

#define ini_error(fmt, arg...)                                                               \
    do {                                                                                     \
        if (g_hi110x_loglevel >= PLAT_LOG_ERR) {                                             \
            /*lint -e515*/                                                                   \
            printk(KERN_ERR "INI_DRV:E]%s:%d]" fmt "\n\n\n", __FUNCTION__, __LINE__, ##arg); \
            /*lint +e515*/                                                                   \
        }                                                                                    \
    } while (0)

#define STR_COUNTRY_CODE "country_code"

#define STR_COUNTRYCODE_SELFSTUDY "countrycode_selfstudy"

#define INI_KERNEL_READ_LEN            512
#define str_cmp_byte(dest, str, local) (((dest)[(local)]) == ((str)[(local)]))

#define HISI_CUST_NVRAM_READ  1
#define HISI_CUST_NVRAM_WRITE 0
#define HISI_CUST_NVRAM_NUM   340
#define HISI_CUST_NVRAM_LEN   104
#define HISI_CUST_NVRAM_NAME  "WINVRAM"

#define INI_MODU_WIFI         0x101
#define INI_MODU_GNSS         0x102
#define INI_MODU_BT           0x103
#define INI_MODU_FM           0x104
#define INI_MODU_PLAT         0x105
#define INI_MODU_HOST_VERSION 0x106
#define INI_MODU_WIFI_MAC     0x107
#define INI_MODU_COEXIST      0x108
#define INI_MODU_DEV_WIFI     0x109
#define INI_MODU_DEV_GNSS     0x10A
#define INI_MODU_DEV_BT       0x10B
#define INI_MODU_DEV_FM       0x10C
#define INI_MODU_DEV_BFG_PLAT 0x10D

#define CUST_MODU_NVRAM 0x200

#define INI_MODU_POWER_FCC  0xe1
#define INI_MODU_POWER_ETSI 0xe2
#define INI_MODU_POWER_JP   0xe3
#define INI_MODU_INVALID    0xff

#define INI_STR_WIFI_NORMAL     "[HOST_WIFI_NORMAL]"
#define INI_STR_GNSS_NORMAL     "[HOST_GNSS_NORMAL]"
#define INI_STR_BT_NORMAL       "[HOST_BT_NORMAL]"
#define INI_STR_FM_NORMAL       "[HOST_FM_NORMAL]"
#define INI_STR_PLAT            "[HOST_PLAT]"
#define INI_STR_WIFI_MAC        "[HOST_WIFI_MAC]"
#define INT_STR_HOST_VERSION    "[HOST_VERSION]"
#define INI_STR_COEXIST         "[HOST_COEXIST]"
#define INI_STR_DEVICE_WIFI     "[DEVICE_WIFI]"
#define INI_STR_DEVICE_GNSS     "[DEVICE_GNSS]"
#define INI_STR_DEVICE_BT       "[DEVICE_BT]"
#define INI_STR_DEVICE_FM       "[DEVICE_FM]"
#define INI_STR_DEVICE_BFG_PLAT "[DEVICE_BFG_PLAT]"
#define INI_STR_POWER_FCC       "[HOST_WIFI_POWER_FCC]"
#define INI_STR_POWER_ETSI      "[HOST_WIFI_POWER_ETSI]"
#define INI_STR_POWER_JP        "[HOST_WIFI_POWER_JP]"
#define INI_STR_POWER_COMMON    "[HOST_WIFI_POWER_COMMON]"

#define ini_init_mutex(mutex)   mutex_init(mutex)
#define ini_mutex_lock(mutex)   mutex_lock(mutex)
#define ini_mutex_unlock(mutex) mutex_unlock(mutex)

#define INI_STR_MODU_LEN    40
#define MAX_READ_LINE_NUM   192
#define INI_FILE_PATH_LEN   128
#define INI_READ_VALUE_LEN  64
#define INI_VERSION_STR_LEN 32

#define INI_SUCC   0
#define INI_FAILED (-1)

#define INI_FILE_TIMESPEC_UNRECONFIG    0
#define INI_FILE_TIMESPEC_RECONFIG      BIT0
#define INI_NVRAM_RECONFIG              BIT1
#define inf_file_get_ctime(file_dentry) ((file_dentry)->d_inode->i_ctime.tv_sec)

#define INI_TIMEVAL_ARRAY_SIZE          2

/* STRUCT Type Definition */
typedef struct ini_board_vervion {
    unsigned char board_version[INI_VERSION_STR_LEN];
} INI_BOARD_VERSION_STRU;

typedef struct ini_param_vervion {
    unsigned char param_version[INI_VERSION_STR_LEN];
} INI_PARAM_VERSION_STRU;

typedef struct file INI_FILE;

/* 全局变量声明 */
extern char g_ini_file_name[INI_FILE_PATH_LEN];
extern INI_PARAM_VERSION_STRU g_param_version;

/* 函数声明 */
extern int32 get_cust_conf_int32(int32 tag_index, int8 *puc_var, int32 *pul_value);
extern int32 get_cust_conf_string(int32 tag_index, int8 *puc_var, int8 *puc_value, uint32 size);
extern int32 find_download_channel(uint8 *buff, uint32 buf_len, int8 *puc_var);
#ifndef _PRE_CONFIG_READ_DYNCALI_E2PROM
extern int32 read_conf_from_nvram(uint8 *pc_out, uint32 size, uint32 nv_number, const char *nv_name);
#endif
extern int32 ini_cfg_init(void);
extern void ini_cfg_exit(void);
extern int32 ini_file_check_conf_update(void);
extern int8 *get_str_from_file(int8 *pc_file_path, const int8 * target_str);
#ifdef _PRE_CONFIG_READ_DYNCALI_E2PROM
#ifdef _PRE_PRODUCT_HI3751_PLATO
#define DrvEepromRead drv_eeprom_read
#endif
#ifdef _PRE_PRODUCT_ARMPC
#define DrvEepromRead drv_e2prom_read
#endif
extern int32 DrvEepromRead(uint8 *partName, uint32 offSet, int8 *rBuf, uint32 rSize);
extern int32 read_conf_from_eeprom(uint8 *pc_out, uint32 size, uint32 offset);
#endif
#ifdef _PRE_SUSPORT_OEMINFO
int32 read_conf_from_oeminfo(uint8 *pc_out, uint32 size, uint32 offset);
#endif
extern void read_tcxo_dcxo_ini_file(void);
#endif
