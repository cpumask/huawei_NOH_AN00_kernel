

#ifndef __OAL_EXT_IF_H__
#define __OAL_EXT_IF_H__
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* ����ͷ�ļ����� */
#include "oal_types.h"
#include "oal_util.h"
#include "oal_hardware.h"
#include "oal_schedule.h"
#include "oal_bus_if.h"
#include "oal_mem.h"
#include "oal_net.h"
#include "oal_list.h"
#include "oal_queue.h"
#include "oal_workqueue.h"
#include "arch/oal_ext_if.h"
#include "oal_thread.h"

#if (!defined(_PRE_PRODUCT_ID_HI110X_DEV))
#include "oal_aes.h"
#include "oal_gpio.h"
#endif

/* infusion����Ԥ�����֧�ֲ��ã��ڴ˶����֧��infusion��飬��ʽ���벻��Ҫ */
#ifdef _PRE_INFUSION_CHECK
#include "oal_infusion.h"
#endif
/* end infusion */
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE) && (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include "plat_exception_rst.h"
#endif
#include "oal_fsm.h"

/* ö�ٶ��� */
typedef enum {
    OAL_TRACE_ENTER_FUNC,
    OAL_TRACE_EXIT_FUNC,

    OAL_TRACE_DIRECT_BUTT
} oal_trace_direction_enum;
typedef oal_uint8 oal_trace_direction_enum_uint8;

#if (_PRE_WLAN_FEATURE_BLACKLIST_LEVEL != _PRE_WLAN_FEATURE_BLACKLIST_NONE)
/* ������ģʽ */
typedef enum {
    CS_BLACKLIST_MODE_NONE,  /* �ر�         */
    CS_BLACKLIST_MODE_BLACK, /* ������       */
    CS_BLACKLIST_MODE_WHITE, /* ������       */

    CS_BLACKLIST_MODE_BUTT
} cs_blacklist_mode_enum;
typedef oal_uint8 cs_blacklist_mode_enum_uint8;
#endif

#ifdef _PRE_WLAN_REPORT_WIFI_ABNORMAL
// �ϱ������쳣״̬���������ϲ���
typedef enum {
    OAL_ABNORMAL_FRW_TIMER_BROKEN = 0,  // frw��ʱ������
    OAL_ABNORMAL_OTHER = 1,             // �����쳣

    OAL_ABNORMAL_BUTT
} oal_wifi_abnormal_reason_enum;

typedef enum {
    OAL_ACTION_RESTART_VAP = 0,  // ����vap���ϲ���δʵ��
    OAL_ACTION_REBOOT = 1,       // ��������

    OAL_ACTION_BUTT
} oal_product_action_enum;

#define OAL_REPORT_WIFI_ABNORMAL(_l_reason, _l_action, _p_arg, _l_size)
#endif

typedef enum {
    OAL_WIFI_STA_LEAVE = 0,  // STA �뿪
    OAL_WIFI_STA_JOIN = 1,   // STA ����

    OAL_WIFI_BUTT
} oal_wifi_sta_action_report_enum;
#define OAL_WIFI_REPORT_STA_ACTION(_ul_ifindex, _ul_eventID, _p_arg, _l_size)

/* ȫ�ֱ������� */
extern oal_void *pst_5115_sys_ctl;

/* �������� */
extern oal_int32 oal_main_init(oal_void);
extern oal_void oal_main_exit(oal_void);
extern oal_uint8 oal_chip_get_device_num(oal_uint32 ul_chip_ver);
extern oal_uint8 oal_board_get_service_vap_start_id(oal_void);

#ifdef _PRE_DEBUG_PROFILING
typedef int (*cyg_check_hook_t)(long this_func, long call_func, long direction);
extern void __cyg_profile_func_register(cyg_check_hook_t hook);
#endif
#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif
#endif /* end of oal_ext_if.h */
