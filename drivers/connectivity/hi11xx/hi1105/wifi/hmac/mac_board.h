

#ifndef __MAC_BOARD_H__
#define __MAC_BOARD_H__

/* 1 ����ͷ�ļ����� */
#include "oal_ext_if.h"
#include "oam_ext_if.h"
#include "wlan_spec.h"
#include "frw_ext_if.h"
#include "mac_device.h"
#include "mac_resource.h"
#include "mac_vap.h"
#include "mac_common.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_MAC_BOARD_H

/* 2 �궨�� */
/* DFX �궨�� */
#if ((_PRE_OS_VERSION_WIN32 == _PRE_OS_VERSION) ||    \
    (_PRE_OS_VERSION_WIN32_RAW == _PRE_OS_VERSION)) &&  \
    (!defined(_PRE_PC_LINT))
#define DFX_GET_PERFORMANCE_LOG_SWITCH_ENABLE(_uc_type) (0)
#define DFX_SET_PERFORMANCE_LOG_SWITCH_ENABLE(_uc_type, _uc_value)
#else
#define DFX_GET_PERFORMANCE_LOG_SWITCH_ENABLE(_uc_type)            dfx_get_performance_log_switch_enable(_uc_type)
#define DFX_SET_PERFORMANCE_LOG_SWITCH_ENABLE(_uc_type, _uc_value)  \
    dfx_set_performance_log_switch_enable(_uc_type, _uc_value)
#endif

/* 3 ö�ٶ��� */
/* DFX����ö�ٶ��� */
#ifdef _PRE_WLAN_DFT_STAT
typedef enum {
    DFX_PERFORMANCE_TX_LOG,
    DFX_PERFORMANCE_DUMP,
    DFX_PERFORMANCE_REV1,
    DFX_PERFORMANCE_REV2,
    DFX_PERFORMANCE_LOG_BUTT,
} dfx_performance_log_switch_enum;
typedef uint8_t dfx_performance_log_switch_enum_uint8;
#endif

typedef enum {
    BOARD_VER_HI1151 = 0,
    BOARD_VER_HI1102,
    BOARD_VER_HI1103,
    BOARD_VER_BUTT
} hisi_device_board_enum;
typedef uint8_t hisi_device_board_enum_uint8;

/* 4 ȫ�ֱ������� */
/* HOST CRX�ӱ� */
extern frw_event_sub_table_item_stru g_ast_dmac_host_crx_table[HMAC_TO_DMAC_SYN_BUTT];

/* DMACģ�飬HOST_DRX�¼�������ע��ṹ���� */
extern frw_event_sub_table_item_stru g_ast_dmac_tx_host_drx[DMAC_TX_HOST_DRX_BUTT];

/* DMACģ�飬WLAN_DTX�¼�������ע��ṹ���� */
extern frw_event_sub_table_item_stru g_ast_dmac_tx_wlan_dtx[DMAC_TX_WLAN_DTX_BUTT];

/* DMACģ�飬WLAN_CTX�¼�������ע��ṹ���� */
extern frw_event_sub_table_item_stru g_ast_dmac_wlan_ctx_event_sub_table[DMAC_WLAN_CTX_EVENT_SUB_TYPE_BUTT];

/* DMACģ��,WLAN_DRX�¼�������ע��ṹ���� */
extern frw_event_sub_table_item_stru g_ast_dmac_wlan_drx_event_sub_table[HAL_WLAN_DRX_EVENT_SUB_TYPE_BUTT];

/* DMACģ�飬�����ȼ��¼�������ע��ṹ�嶨�� */
extern frw_event_sub_table_item_stru g_ast_dmac_high_prio_event_sub_table[HAL_EVENT_DMAC_HIGH_PRIO_SUB_TYPE_BUTT];

/* DMACģ��,WLAN_CRX�¼�������ע��ṹ���� */
extern frw_event_sub_table_item_stru g_ast_dmac_wlan_crx_event_sub_table[HAL_WLAN_CRX_EVENT_SUB_TYPE_BUTT];

/* DMACģ�飬TX_COMP�¼�������ע��ṹ���� */
extern frw_event_sub_table_item_stru g_ast_dmac_tx_comp_event_sub_table[HAL_TX_COMP_SUB_TYPE_BUTT];

/* WLAN_DTX �¼������ͱ� */
extern frw_event_sub_table_item_stru g_ast_hmac_wlan_dtx_event_sub_table[DMAC_TX_WLAN_DTX_BUTT];

/* HMACģ�� WLAN_DRX�¼�������ע��ṹ���� */
extern frw_event_sub_table_item_stru g_ast_hmac_wlan_drx_event_sub_table[DMAC_WLAN_DRX_EVENT_SUB_TYPE_BUTT];

/* HMACģ�� WLAN_CRX�¼�������ע��ṹ���� */
extern frw_event_sub_table_item_stru g_ast_hmac_wlan_crx_event_sub_table[DMAC_WLAN_CRX_EVENT_SUB_TYPE_BUTT];

/* HMACģ�� ����HOST��������¼�������ע��ṹ���� */
extern frw_event_sub_table_item_stru g_ast_hmac_wlan_ctx_event_sub_table[DMAC_TO_HMAC_SYN_BUTT];

/* HMACģ�� MISC��ɢ�¼�������ע��ṹ���� */
extern frw_event_sub_table_item_stru g_ast_hmac_wlan_misc_event_sub_table[DMAC_MISC_SUB_TYPE_BUTT];

extern frw_event_sub_table_item_stru g_ast_hmac_ddr_drx_sub_table[HAL_WLAN_DDR_DRX_EVENT_SUB_TYPE_BUTT];

/* 5 ��Ϣͷ���� */
/* 6 ��Ϣ���� */
/* 7 STRUCT���� */
/* 8 UNION���� */
/* 9 OTHERS���� */
/* 10 �������� */
/* DFX���ú������� */
#ifdef _PRE_WLAN_DFT_STAT
uint32_t dfx_get_performance_log_switch_enable(
    dfx_performance_log_switch_enum_uint8 uc_performance_log_switch_type);
void dfx_set_performance_log_switch_enable(
    dfx_performance_log_switch_enum_uint8 uc_performance_log_switch_type, uint8_t uc_value);
#endif

void event_fsm_unregister(void);

void event_fsm_table_register(void);

#endif /* end of mac_board */



