

#ifndef __OAL_MEM_H__
#define __OAL_MEM_H__

/* ����ͷ�ļ����� */
#include "platform_spec.h"
#include "oal_cache.h"
#include "oal_list.h"
#include "oal_schedule.h"
#include "oal_net.h"
#include "oal_util.h"
#include "arch/oal_mem.h"

#if (_PRE_OS_VERSION == _PRE_OS_VERSION_LINUX)
#include <linux/kallsyms.h>
#endif
/* �궨�� */
#define oal_mem_netbuf_alloc(_uc_out_subpool_id, _us_len, _uc_netbuf_priority) \
    oal_netbuf_alloc(_us_len, 0, WLAN_MEM_NETBUF_ALIGN)

#define oal_mem_netbuf_trace(_pst_netbuf, _uc_lock)

/* Local Mem ���нӿڵ� _uc_lock ����������Ч 2016/8/18 */
/* ��netbuf�ڴ����������ڴ�صĶ����ṩ�ӿ� host��device���ýӿ� */
#define oal_mem_alloc_m(_en_pool_id, _us_len, _uc_lock) \
    oal_mem_alloc(THIS_FILE_ID, __LINE__, _en_pool_id, _us_len, _uc_lock)
#define oal_mem_free_m(_p_data, _uc_lock) \
    oal_mem_free(THIS_FILE_ID, __LINE__, _p_data, _uc_lock)

#define oal_get_mem_flag(_p_data, _puc_state)

#define oal_mem_free_array_m(_p_data, _num) \
    oal_mem_free_array(THIS_FILE_ID, __LINE__, _p_data, _num)

#define oal_mem_incr_user_m(_pst_mem, _uc_lock) \
    oal_mem_incr_user(THIS_FILE_ID, __LINE__, _pst_mem, _uc_lock)

#define oal_mem_alloc_enhanced_m(_en_pool_id, _us_len, _uc_lock) \
    oal_mem_alloc_enhanced(THIS_FILE_ID, __LINE__, _en_pool_id, _us_len, _uc_lock)

#define oal_mem_free_enhanced_m(_pst_mem, _uc_lock) \
    oal_mem_free_enhanced(THIS_FILE_ID, __LINE__, _pst_mem, _uc_lock)

/* host����ڴ���Ϣ�ӿ� */
#define oal_mem_info_print(_pool_id)

#define oal_mem_trace_m(_p_data, _uc_lock) \
    oal_mem_trace(THIS_FILE_ID, __LINE__, _p_data, _uc_lock)

#define oal_mem_trace_enhanced_m(_pst_mem, _uc_lock) \
    oal_mem_trace_enhanced(THIS_FILE_ID, __LINE__, _pst_mem, _uc_lock)

/* ����enhanced���͵�����ӿ����ͷŽӿڣ�ÿһ���ڴ�鶼����һ��4�ֽڵ�ͷ���� */
/* ����ָ���ڴ�����ṹ��oal_mem_struc�������ڴ��Ľṹ������ʾ��           */
/*                                                                           */
/* +-------------------+------------------------------------------+---------+ */
/* | oal_mem_stru addr |                    payload               | dog tag | */
/* +-------------------+------------------------------------------+---------+ */
/* |      4/8 byte       |                                          | 4 byte  | */
/* +-------------------+------------------------------------------+---------+ */
#define OAL_DOG_TAG 0x5b3a293e /* ���ƣ����ڼ���ڴ�Խ�� */

#ifdef CONFIG_ARM64
#define OAL_DOG_TAG_SIZE 8 /* ���ƴ�С(8�ֽ�) */
#else
#define OAL_DOG_TAG_SIZE 4 /* ���ƴ�С(4�ֽ�) */
#endif
#define OAL_NETBUF_MAINTAINS_SIZE 24 /* alloc_file_id(4B) + alloc_line_num(4B) + alloc_timestamp(4B) + */

#define OAL_NETBUF_DEFAULT_HEADROOM 256 /* ����netbufʱ��Ĭ�ϵ�headroom */
#define OAL_MAX_TX_DSCR_CNT_IN_LIST 100
#define OAL_TX_DSCR_ITEM_NUM        200
#define OAL_TX_DSCR_RCD_TAIL_CNT    20

#if (_PRE_OS_VERSION == _PRE_OS_VERSION_LINUX)
#define OAL_MEM_SPRINT_SYMBOL_SIZE KSYM_SYMBOL_LEN
#else
#define OAL_MEM_SPRINT_SYMBOL_SIZE 1
#endif
/* ö�ٶ��� */
/*
 * ö����  : oal_mem_pool_id_enum_uint8
 * ö��˵��: HOST���ڴ��ID
 */
typedef enum {
    OAL_MEM_POOL_ID_EVENT = 0,       /* �¼��ڴ�� */
    OAL_MEM_POOL_ID_SHARED_DATA_PKT, /* ��������֡�ڴ�� */
    OAL_MEM_POOL_ID_SHARED_MGMT_PKT, /* �������֡�ڴ�� */
    OAL_MEM_POOL_ID_LOCAL,           /* ���ر����ڴ��  */
    OAL_MEM_POOL_ID_SHARED_DSCR,     /* �����������ڴ�� */
    OAL_MEM_POOL_ID_TX_RING,         /* Tx TID Ring�ڴ�� */

    OAL_MEM_POOL_ID_SDT_NETBUF, /* SDT netbuf�ڴ�� */

    OAL_MEM_POOL_ID_BUTT
} oal_mem_pool_id_enum;
typedef uint8_t oal_mem_pool_id_enum_uint8;
/*
 * ö����  : oal_netbuf_priority_enum_uint8
 * ö��˵��: netbuf���ȼ�
 */
typedef enum {
    OAL_NETBUF_PRIORITY_LOW = 0,   // ������ȼ�,���ܿ������
    OAL_NETBUF_PRIORITY_MID = 1,   // �м����ȼ������Կ�����룬�����������������NƬ
    OAL_NETBUF_PRIORITY_HIGH = 2,  // ������ȼ������Կ�����룬�ҿ��������������NƬ

    OAL_NETBUF_PRIORITY_BUTT
} oal_netbuf_priority_enum;
typedef uint8_t oal_netbuf_priority_enum_uint8;

/*
 * ö����  : oal_netbuf_id_enum_uint8
 * ö��˵��: �����ṩnetbuf�ڴ��ID
 */
typedef enum {
    OAL_NORMAL_NETBUF = 0, /* ������֡����֡�ڴ�� */
    OAL_MGMT_NETBUF = 1,   /* ����֡�ڴ�� */
    OAL_OAM_NETBUF = 2,    /* SDT�ڴ�� */

    OAL_NETBUF_POOL_ID_BUTT
} oal_netbuf_id_enum;
typedef uint8_t oal_netbuf_id_enum_uint8;

#define OAL_MEM_POOL_ID_HI_LOCAL      OAL_MEM_POOL_ID_LOCAL
#define OAL_MEM_POOL_ID_HI_READ_LOCAL OAL_MEM_POOL_ID_LOCAL

/* STRUCT���� */
typedef struct {
    uint32_t (*p_mempool_info_func)(uint8_t en_pool_id,
                                      uint16_t us_mem_total_cnt,
                                      uint16_t us_mem_used_cnt,
                                      uint8_t uc_subpool_id,
                                      uint16_t us_total_cnt,
                                      uint16_t us_free_cnt);
    uint32_t (*p_memblock_info_func)(uint8_t *puc_origin_data,
                                       uint8_t uc_user_cnt,
                                       oal_mem_pool_id_enum_uint8 en_pool_id,
                                       uint8_t uc_subpool_id,
                                       uint16_t us_len,
                                       uint32_t ul_alloc_file_id,
                                       uint32_t ul_alloc_line_num);
} oal_mempool_info_to_sdt_stru;

/*
 * �ṹ��  : oal_mem_netbuf_info_stru
 * �ṹ˵��: netbuf�ڴ��ά��ṹ��
 */
typedef struct {
    uint32_t ul_dog_tag;          /* ���ƣ������ڴ�дԽ���� */
    uint32_t ul_alloc_file_id;    /* ����netbuf�ڴ�ʱ���ļ�ID */
    uint32_t ul_alloc_line_num;   /* ����netbuf�ڴ�ʱ���к� */
    uint32_t ul_alloc_time_stamp; /* ����netbuf�ڴ�ʱ��ʱ��� */
    uint32_t ul_trace_file_id;    /* netbuf�ڴ��ڹؼ�·���ϵ��ļ�ID */
    uint32_t ul_trace_line_num;   /* netbuf�ڴ��ڹؼ�·���ϵ��к� */
    uint32_t ul_trace_time_stamp; /* netbuf�ڴ��ڹؼ�·���ϵ�ʱ��� */
} oal_mem_netbuf_info_stru;

/*
 * �ṹ��  : oal_mem_stru
 * �ṹ˵��: �ڴ��ṹ��
 */
struct oal_mem_stru_tag {
    uint8_t *puc_data;        /* ������ݵ�ָ�� */
    uint8_t *puc_origin_data; /* ��¼���ݵ�ԭʼָ�� */

    uint16_t us_len;                              /* �ڴ��ĳ��� */
    uint8_t uc_user_cnt : 4;                      /* ���뱾�ڴ����û����� */
    oal_mem_state_enum_uint8 en_mem_state_flag : 4; /* �ڴ��״̬ */
    oal_mem_pool_id_enum_uint8 en_pool_id : 4;      /* ���ڴ�������һ���ڴ�� */
    uint8_t uc_subpool_id : 4;                    /* ���ڴ���������һ�����ڴ�� */

    uintptr_t ul_return_addr; /* alloc�ڴ�ĺ�����ַ ���� */

/* �ڴ������Ϣ */
    uint32_t ul_alloc_core_id[WLAN_MEM_MAX_USERS_NUM];    /* �����ڴ�ʱ���ں�ID */
    uint32_t ul_alloc_file_id[WLAN_MEM_MAX_USERS_NUM];    /* �����ڴ�ʱ���ļ�ID */
    uint32_t ul_alloc_line_num[WLAN_MEM_MAX_USERS_NUM];   /* �����ڴ�ʱ���к� */
    uint32_t ul_alloc_time_stamp[WLAN_MEM_MAX_USERS_NUM]; /* �����ڴ�ʱ��ʱ��� */
    uint32_t ul_trace_file_id;                            /* �ڴ��ڹؼ�·���ϵ��ļ�ID */
    uint32_t ul_trace_line_num;                           /* �ڴ��ڹؼ�·���ϵ��к� */
    uint32_t ul_trace_time_stamp;                         /* �ڴ��ڹؼ�·���ϵ�ʱ��� */
} OAL_CACHELINE_ALIGNED;
typedef struct oal_mem_stru_tag oal_mem_stru;

/*
 * �ṹ��  : oal_mem_subpool_stru
 * �ṹ˵��: ���ڴ�ؽṹ��
 */
typedef struct {
    oal_spin_lock_stru st_spinlock;
    uint16_t us_len;      /* �����ڴ�ص��ڴ�鳤�� */
    uint16_t us_free_cnt; /* �����ڴ�ؿ����ڴ���� */

    /* ��¼oal_mem_stru�����ڴ��������ջ��Ԫ�أ�������Ϊoal_mem_struָ�� */
    void **ppst_free_stack;

    uint16_t us_total_cnt; /* �����ڴ���ڴ������ */
    uint8_t auc_resv[2];
} oal_mem_subpool_stru;

/*
 * �ṹ��  : oal_mem_pool_stru
 * �ṹ˵��: �ڴ�ؽṹ��
 */
typedef struct {
    uint16_t us_max_byte_len; /* ���ڴ�ؿɷ����ڴ����󳤶� */
    uint8_t uc_subpool_cnt;   /* ���ڴ��һ���ж������ڴ�� */
    uint8_t uc_resv;
    /* ���ڴ������������ */
    oal_mem_subpool_stru ast_subpool_table[WLAN_MEM_MAX_SUBPOOL_NUM];

    uint16_t us_mem_used_cnt;  /* ���ڴ�������ڴ�� */
    uint16_t us_mem_total_cnt; /* ���ڴ��һ���ж����ڴ�� */
    oal_mem_stru *pst_mem_start_addr;
} oal_mem_pool_stru;

/*
 * �ṹ��  : oal_mem_subpool_cfg_stru
 * �ṹ˵��: ���ڴ�����ýṹ��
 */
typedef struct {
    uint16_t us_size; /* �����ڴ���ڴ���С */
    uint16_t us_cnt;  /* �����ڴ���ڴ����� */
} oal_mem_subpool_cfg_stru;

/*
 * �ṹ��  : oal_mem_pool_cfg_stru
 * �ṹ˵��: �ڴ�����ýṹ��
 */
typedef struct {
    oal_mem_pool_id_enum_uint8 en_pool_id; /* �ڴ��ID */
    uint8_t uc_subpool_cnt;              /* ���ڴ�����ڴ�ظ��� */

    uint8_t auc_resv[2]; /* ����2���ֽ� */

    /* ָ��ÿһ���ڴ�ؾ���������Ϣ */
    oal_mem_subpool_cfg_stru *pst_subpool_cfg_info;
} oal_mem_pool_cfg_stru;

/* �������� */
extern void *oal_mem_alloc(uint32_t ul_file_id,
                           uint32_t ul_line_num,
                           oal_mem_pool_id_enum_uint8 en_pool_id,
                           uint16_t us_len,
                           uint8_t uc_lock);
extern uint32_t oal_mem_free(uint32_t ul_file_id, uint32_t ul_line_num, void *pst_mem, uint8_t uc_lock);

extern uint32_t oal_mem_free_array(uint32_t ul_file_id,
                                   uint32_t ul_line_num,
                                   void *p_data,
                                   uint8_t uc_mem_num);

extern oal_mem_stru *oal_mem_alloc_enhanced(uint32_t ul_file_id,
                                            uint32_t ul_line_num,
                                            oal_mem_pool_id_enum_uint8 en_pool_id,
                                            uint16_t us_len,
                                            uint8_t uc_lock);
extern uint32_t oal_mem_free_enhanced(uint32_t ul_file_id, uint32_t ul_line_num,
                                      oal_mem_stru *p_data, uint8_t uc_lock);
extern uint32_t oal_mem_init_pool(void);
extern void oal_mem_exit(void);
extern uint32_t oal_mem_incr_user(uint32_t ul_file_id,
                                  uint32_t ul_line_num,
                                  oal_mem_stru *pst_mem,
                                  uint8_t uc_lock);
extern void oal_mem_info(oal_mem_pool_id_enum_uint8 en_pool_id);
extern void oal_mem_leak(oal_mem_pool_id_enum_uint8 en_pool_id);

extern void oal_mem_start_stat(void);
extern uint32_t oal_mem_end_stat(void);

extern uint32_t oal_mem_trace_enhanced(uint32_t ul_file_id,
                                       uint32_t ul_line_num,
                                       oal_mem_stru *pst_mem,
                                       uint8_t uc_lock);
extern uint32_t oal_mem_trace(uint32_t ul_file_id,
                              uint32_t ul_line_num,
                              const void *p_data,
                              uint8_t uc_lock);

/* device ����ϵͳ�� host��ĺ������ */
extern oal_mem_pool_stru *oal_mem_get_pool(oal_mem_pool_id_enum_uint8 en_pool_id);

extern oal_mem_pool_cfg_stru *oal_mem_get_pool_cfg_table(oal_mem_pool_id_enum_uint8 en_pool_id);
extern oal_netbuf_stru *oal_mem_sdt_netbuf_alloc(uint16_t us_len, uint8_t uc_lock);
extern uint32_t oal_mem_sdt_netbuf_free(oal_netbuf_stru *pst_netbuf, uint8_t uc_lock);
extern oal_netbuf_stru *oal_netbuf_duplicate(oal_netbuf_stru *pst_src_netbuf,
                                             uint8_t uc_out_subpool_id,
                                             uint32_t ul_add_head_room,
                                             uint32_t ul_add_tail_room);

extern void oal_mem_print_normal_pool_info(oal_mem_pool_id_enum_uint8 en_pool_id);
extern void oal_mem_print_pool_info(void);
extern void oal_mem_print_funcname(uintptr_t func_addr);

#define oal_netbuf_duplicate_normal(pst_src_netbuf, ul_add_head_room, ul_add_tail_room) \
    oal_netbuf_duplicate(pst_src_netbuf, OAL_NORMAL_NETBUF, ul_add_head_room, ul_add_tail_room)

#endif /* end of oal_mm.h */

