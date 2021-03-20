

#ifndef __CHR_USER_H__
#define __CHR_USER_H__

/* Other Include Head File */
#include "chr_errno.h"

/* �궨�� */
/* �����ϱ� */
/* оƬУ׼�쳣�¼��ϱ� */
#define CHR_CHIP_CALI_ERR_REPORT_EVENTID 909003001
/* оƬƽ̨�쳣�¼��ϱ� */
#define CHR_PLATFORM_EXCEPTION_EVENTID 909003002
/* оƬ����ʧ���¼��ϱ� */
#define CHR_WIFI_CONNECT_FAIL_REPORT_EVENTID 909003003
/* оƬ�����Ͽ��¼��ϱ� */
#define CHR_WIFI_DISCONNECT_REPORT_EVENTID 909003004
/* wifi�¿�ʱ���ϱ� */
#define CHR_WIFI_TEMP_PROTECT_REPORT_EVENTID 909003005
/* wifi PLL ʧ�� */
#define CHR_WIFI_PLL_LOST_REPORT_EVENTID 909003006
/* wifi ����ʧ���¼��ϱ� */
#define CHR_WIFI_ROAM_FAIL_REPORT_EVENTID 909009102
/* wifi ����ͳ���¼��ϱ� */
#define CHR_WIFI_ROAM_INFO_REPORT_EVENTID 909009105
/* wifi 11ax����ͳ���ϱ� */
#define CHR_WIFI_OFDMA_STAT_EVENTID 909009126
/* wifi �����ϱ� */
#define CHR_WIFI_PERIODIC_REPORT_EVENTID 909009521
/* PC����������ϱ� */
#define CHR_WIFI_COUNTRYCODE_UPDATE_EVENTID 609003001
/* PC IWAR RR���������ϱ� */
#define CHR_WIFI_IWARE_RR_EVENTID 609003002
/* PC E2PROM�ϱ� */
#define CHR_READ_EEPROM_ERROR_EVENTID 609009001
/* PC S3&S4 CHR�ϱ� */
#define CHR_PLATFORM_S3S4_EVENTID 609009002
/* HiD2D ACS����ͳ����ϢCHR�ϱ� */
#define CHR_HID2D_ACS_INFO_REPORT_EVENTID 909001011

/* ������ѯ */
/* wifi��������ͳ���ϱ� */
#define CHR_WIFI_POW_RATE_COUNT_QUERY_EVENTID 909009519
/* wifi����ʱ��ͳ���ϱ� */
#define CHR_WIFI_WORK_TIME_INFO_EVENTID 909009520
/* wifi����ʱ��ϸ�ֺ�ʵ���շ���ͳ���ϱ� */
#define CHR_WIFI_WORKTIME_AND_TX_RX_COUNT_INFO_EVENTID 909009599

/* wifi�򿪹ر�ʧ�� */
#define CHR_WIFI_OPEN_CLOSE_FAIL_QUERY_EVENTID 909002021
/* wifi�쳣�Ͽ� */
#define CHR_WIFI_DISCONNECT_QUERY_EVENTID 909002022
/* wifi�����쳣 */
#define CHR_WIFI_CONNECT_FAIL_QUERY_EVENTID 909002023
/* wifi����ʧ�� */
#define CHR_WIFI_WEB_FAIL_QUERY_EVENTID 909002024
/* wifi������ */
#define CHR_WIFI_WEB_SLOW_QUERY_EVENTID 909002025
/* wifi�ϱ�beacon */
#define CHR_WIFI_BEACON_EVENTID 909002027
/* wifi�ϱ�twt������Ϣ */
#define CHR_WIFI_TWT_QUERY_EVENTID 909009127

/* BT����쳣�ϱ� */
#define CHR_BT_CHIP_SOFT_ERROR_EVENTID 913900003

#define CHR_EXCEPTION_MODULE_NAME_LEN 10
#define CHR_EXCEPTION_ERROR_CODE_LEN  20

/* STRUCT���� */
typedef struct chr_chip_excption_event_info_tag {
    uint32_t ul_module;
    uint32_t ul_plant;
    uint32_t ul_subplant;
    uint32_t ul_errid;
} chr_platform_exception_event_info_stru;

typedef struct chr_scan_exception_event_info_tag {
    int sub_event_id;
    char module_name[CHR_EXCEPTION_MODULE_NAME_LEN];
    char error_code[CHR_EXCEPTION_ERROR_CODE_LEN];
} chr_scan_exception_event_info_stru;

typedef struct chr_connect_exception_event_info_tag {
    int sub_event_id;
    char platform_module_name[CHR_EXCEPTION_MODULE_NAME_LEN];
    char error_code[CHR_EXCEPTION_ERROR_CODE_LEN];
} chr_connect_exception_event_info_stru;

#ifdef _PRE_CONFIG_HW_CHR
#define CHR_ERR_DATA_MAX_NUM 0x20
#define CHR_ERR_DATA_MAX_LEN (OAL_SIZEOF(oal_uint32) * CHR_ERR_DATA_MAX_NUM)
typedef struct {
    uint32_t errno;
    uint16_t errlen;
    uint16_t flag : 1;
    uint16_t resv : 15;
} chr_errno_with_arg_stru;

typedef enum {
    CHR_DEVICE = 0x0,
    CHR_HOST = 0x1,
    CHR_DEVICE_P2P = 0x2,
    CHR_HOST_P2P = 0x3,
    CHR_DEVICE_AP = 0x4,
    CHR_HOST_AP = 0x5,
    /* chr��չר��:����flags */
    CHR_REPORT_EXT_WIFI_INFO = 0x8000,
    /* �����ϲ�,����chr�ϱ�����flag */
    CHR_REPORT_FINISH = 0xFFFF
}CHR_REPORT_FLAGS_ENUM;
typedef uint16_t chr_report_flags_enum_uint16;

typedef uint32 (*chr_get_wifi_info)(uint32);

extern void chr_dev_exception_callback(void *buff, uint16 len);
extern int32 __chr_exception_para(uint32 chr_errno, uint8 *chr_ptr, uint16 chr_len);
extern int32 __chr_exception_para_q(uint32 chr_errno, chr_report_flags_enum_uint16 chr_flag,
    uint8 *chr_ptr, uint16 chr_len);
extern void chr_host_callback_register(chr_get_wifi_info pfunc);
extern void chr_host_callback_unregister(void);

#define chr_exception(errno)                         __chr_exception_para(errno, NULL, 0)
#define chr_exception_p(chr_errno, chr_ptr, chr_len) __chr_exception_para(chr_errno, chr_ptr, chr_len)
#define chr_exception_q(chr_errno, chr_flag, chr_ptr, chr_len) __chr_exception_para_q(chr_errno, chr_flag, chr_ptr, chr_len)
#define chr_exception_report(excption_event, module, plant, subplant, errid)                 \
    do {                                                                                     \
        chr_platform_exception_event_info_stru chr_platform_exception_event_info;            \
        chr_platform_exception_event_info.ul_module = module;                                \
        chr_platform_exception_event_info.ul_plant = plant;                                  \
        chr_platform_exception_event_info.ul_subplant = subplant;                            \
        chr_platform_exception_event_info.ul_errid = errid;                                  \
        chr_exception_p(excption_event, (oal_uint8 *)(&chr_platform_exception_event_info),   \
                        OAL_SIZEOF(chr_platform_exception_event_info_stru));                 \
    } while (0)
#else
#define chr_exception(chr_errno)
#define chr_exception_p(chr_errno, chr_ptr, chr_len)
#define chr_exception_q(chr_errno, chr_flag, chr_ptr, chr_len)
#define chr_exception_report(excption_event, module, plant, subplant, errid)
#endif
#endif
