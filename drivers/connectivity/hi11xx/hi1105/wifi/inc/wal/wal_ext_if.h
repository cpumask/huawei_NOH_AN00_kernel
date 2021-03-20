

#ifndef __WAL_EXT_IF_H__
#define __WAL_EXT_IF_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 1 ����ͷ�ļ����� */
#include "oal_types.h"
#include "oal_schedule.h"
#include "wlan_types.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_WAL_EXT_IF_H

/* 2 �궨�� */
#define WAL_SDT_MEM_MAX_LEN 32 /* SDT��д�ڴ����󳤶� */

/* 3 ö�ٶ��� */
/* offloadʱ����ʾ��һ���˵�ö�� */
typedef enum {
    WAL_OFFLOAD_CORE_MODE_HMAC,
    WAL_OFFLOAD_CORE_MODE_DMAC,

    WAL_OFFLOAD_CORE_MODE_BUTT
} wal_offload_core_mode_enum;
typedef uint8_t wal_offload_core_mode_enum_uint8;
/* 4 ȫ�ֱ������� */
/* 5 ��Ϣͷ���� */
/* 6 ��Ϣ���� */
/* 7 STRUCT���� */
/* SDT�ϴ��ӿڵ�ע�ắ������ */
typedef int32_t (*wal_sdt_up_func)(oal_netbuf_stru *pst_netbuf);

/*lint -e958*/ /*lint -e959*/
/* ��ȡ������ȫ�ֱ����Ľṹ�� */
#define WAL_GLB_VAR_NAME_LEN (31)
#define WAL_GLB_VAR_VAL_LEN  (128)
typedef struct {
    uint8_t uc_mode; /* ��дģʽ */
    uint16_t us_len; /* ��д���� */
    uint8_t en_offload_core_mode;
    uint8_t auc_global_value_name[WAL_GLB_VAR_NAME_LEN + 1];
    uint8_t auc_global_value[WAL_GLB_VAR_VAL_LEN];
} wal_sdt_global_var_stru;
/*lint -e958*/ /*lint -e959*/
/* ��ȡ�����üĴ����Ľṹ�� */
typedef struct {
    uint8_t auc_vapname[16];
    uint32_t ul_addr;
    uint16_t us_len;
    uint8_t uc_mode;
    uint8_t auc_resv[1];
    uint32_t ul_reg_val;
} wal_sdt_reg_frame_stru;

/* ��ȡ�������ڴ�Ľṹ�� */
typedef struct {
    uintptr_t ul_addr;
    uint16_t us_len;
    uint8_t uc_mode;
    wal_offload_core_mode_enum_uint8 en_offload_core_mode;
    uint8_t auc_data[WAL_SDT_MEM_MAX_LEN];
} wal_sdt_mem_frame_stru;

/* ά�⣬����ĳһ�־���event�ϱ����صĲ����ṹ�� */
typedef struct {
    int32_t l_event_type;
    int32_t l_param;
} wal_specific_event_type_param_stru;

/* 8 UNION���� */
/* 9 OTHERS���� */
extern uint8_t g_custom_cali_done; /* ��ʶ�Ƿ����ϵ��һ��У׼ */

/* 10 �������� */
int32_t wal_main_init(void);
void wal_main_exit(void);
int32_t wal_recv_memory_cmd(uint8_t *puc_buf, uint16_t us_len);
int32_t wal_recv_global_var_cmd(uint8_t *puc_buf, uint16_t us_len);
int32_t wal_recv_reg_cmd(uint8_t *puc_buf, uint16_t us_len);
int32_t wal_recv_config_cmd(uint8_t *puc_buf, uint16_t us_len);

uint32_t wal_hipriv_get_mac_addr(int8_t *pc_param,
                                              uint8_t auc_mac_addr[],
                                              uint32_t *pul_total_offset);

uint32_t wal_hipriv_register_inetaddr_notifier(void);
uint32_t wal_hipriv_unregister_inetaddr_notifier(void);
uint32_t wal_hipriv_register_inet6addr_notifier(void);
uint32_t wal_hipriv_unregister_inet6addr_notifier(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of wal_ext_if.h */
