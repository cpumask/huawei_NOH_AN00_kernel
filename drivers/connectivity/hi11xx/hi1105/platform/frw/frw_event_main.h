

#ifndef __FRW_EVENT_MAIN_H__
#define __FRW_EVENT_MAIN_H__

/* ����ͷ�ļ����� */
#include "oal_ext_if.h"
#include "oam_ext_if.h"
#include "frw_ext_if.h"
#include "frw_main.h"
#include "frw_event_sched.h"
#include "wlan_types.h"
#include "securec.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_FRW_EVENT_MAIN_H

/*
 * ö����  : frw_event_deploy_enum_uint8
 * ö��˵��: �¼���������
 */
typedef enum {
    FRW_EVENT_DEPLOY_NON_IPC = 0, /* �Ǻ˼�ͨ�� */
    FRW_EVENT_DEPLOY_IPC,         /* �˼�ͨѶ */

    FRW_EVENT_DEPLOY_BUTT
} frw_event_deploy_enum;
typedef oal_uint8 frw_event_deploy_enum_uint8;

/* �¼���������� */
/* һ�����͵��¼���Ӧ������������¼���Ĵ�СΪ�¼����͵�2�� */
#define FRW_EVENT_TABLE_MAX_ITEMS (FRW_EVENT_TYPE_BUTT * 2)

/* ȫ�ֱ������� */
#ifdef _PRE_FRW_EVENT_PROCESS_TRACE_DEBUG
typedef struct _frw_event_segment_stru_ {
    frw_event_type_enum_uint8 en_type;         /* �¼����� */
    oal_uint8 uc_sub_type;                     /* �¼������� */
    frw_event_pipeline_enum_uint8 en_pipeline; /* �¼��ֶκ� */
    oal_uint8 uc_vap_id;                       /* VAP ID */
} frw_event_segment_stru;

typedef struct _frw_event_trace_item_stru_ {
    frw_event_segment_stru st_event_seg;
    oal_uint64 timestamp;
} frw_event_trace_item_stru;

#define CONFIG_FRW_MAX_TRACE_EVENT_NUMS 50
typedef struct _frw_event_trace_stru_ {
    frw_event_trace_item_stru st_trace_item[CONFIG_FRW_MAX_TRACE_EVENT_NUMS];
    oal_uint32 ul_current_pos;
    oal_uint32 ul_over_flag;
    const char *pst_func_name;
    oal_int32 line_num;
} frw_event_trace_stru;
#endif

/*
 * �ṹ��  : frw_event_mgmt_stru
 * �ṹ˵��: �¼�����ṹ��
 */
typedef struct _frw_event_mgmt_stru_ {
    frw_event_queue_stru st_event_queue[FRW_EVENT_MAX_NUM_QUEUES];    /* �¼����� */
    frw_event_sched_queue_stru st_sched_queue[FRW_SCHED_POLICY_BUTT]; /* �ɵ��ȶ��� */
#ifdef _PRE_FRW_EVENT_PROCESS_TRACE_DEBUG
    frw_event_trace_stru *pst_frw_trace;
#endif
} frw_event_mgmt_stru;
extern frw_event_table_item_stru g_event_table[FRW_EVENT_TABLE_MAX_ITEMS];

extern frw_event_mgmt_stru g_event_manager[WLAN_FRW_MAX_NUM_CORES];

/* �������� */
extern oal_uint32 frw_event_init(oal_void);
extern oal_uint32 frw_event_exit(oal_void);
extern oal_uint32 frw_event_queue_enqueue(frw_event_queue_stru *pst_event_queue, frw_event_mem_stru *pst_event_mem);
extern frw_event_mem_stru *frw_event_queue_dequeue(frw_event_queue_stru *pst_event_queue);
extern oal_uint32 frw_event_post_event(frw_event_mem_stru *pst_event_mem, oal_uint32 ul_core_id);
extern oal_void frw_event_deploy_register(oal_uint32 (*p_func)(frw_event_mem_stru *pst_event_mem,
                                                oal_uint8 *puc_deploy_result));
extern oal_void frw_event_ipc_event_queue_full_register (oal_uint32(*p_func)(oal_void));
extern oal_void frw_event_ipc_event_queue_empty_register (oal_uint32(*p_func)(oal_void));
extern frw_event_sched_queue_stru *frw_event_get_sched_queue(oal_uint32 ul_core_id,
                                                                 frw_sched_policy_enum_uint8 en_policy);
extern oal_void frw_event_vap_pause_event(oal_uint8 uc_vap_id);
extern oal_void frw_event_vap_resume_event(oal_uint8 uc_vap_id);
extern oal_uint32 frw_event_vap_flush_event(oal_uint8 uc_vap_id, frw_event_type_enum_uint8 en_event_type,
                                                oal_bool_enum_uint8 en_drop);
extern oal_uint32 frw_event_queue_info(oal_void);

/*
 * �� �� ��  : frw_event_to_qid
 * ��������  : �����¼����ݻ�ȡ��Ӧ���¼�����ID
 * �������  : pst_event_mem: ָ���¼��ڴ���ָ��
 * �������  : pus_qid      : ����ID
 */
OAL_STATIC OAL_INLINE oal_uint32 frw_event_to_qid(frw_event_mem_stru *pst_event_mem, oal_uint16 *pus_qid)
{
    oal_uint16 us_qid;
    frw_event_hdr_stru *pst_event_hrd = NULL;

    if (oal_unlikely((pst_event_mem == NULL) || (pus_qid == NULL))) {
        oal_warn_on(1);
        return OAL_FAIL;
    }

    /* ��ȡ�¼�ͷ�ṹ */
    pst_event_hrd = (frw_event_hdr_stru *)pst_event_mem->puc_data;

    us_qid = pst_event_hrd->uc_vap_id * FRW_EVENT_TYPE_BUTT + pst_event_hrd->en_type;

    /* �쳣: ����ID�������ֵ */
    if ((us_qid >= FRW_EVENT_MAX_NUM_QUEUES)) {
        oam_error_log4(0, OAM_SF_FRW,
                       "{frw_event_to_qid, array overflow! us_qid[%d], vap_id[%d], en_type[%d], sub_type[%d]}",
                       us_qid, pst_event_hrd->uc_vap_id, pst_event_hrd->en_type, pst_event_hrd->uc_sub_type);
        return OAL_ERR_CODE_ARRAY_OVERFLOW;
    }

    *pus_qid = us_qid;

    return OAL_SUCC;
}

OAL_STATIC OAL_INLINE oal_uint32 frw_event_process_for_trx_adapt(
                                    frw_event_table_item_stru *pst_frw_event_table, 
                                    oal_uint8 uc_sub_type,
                                    frw_event_mem_stru *pst_event_mem,
                                    oal_uint8 uc_index)
{
    frw_event_mem_stru *pst_tmp_event_mem = NULL;
    oal_uint32 ul_ret = 0;

    /* For tx adapt */
    if (pst_frw_event_table->pst_sub_table[uc_sub_type].p_tx_adapt_func != OAL_PTR_NULL) {
        ul_ret = pst_frw_event_table->pst_sub_table[uc_sub_type].p_tx_adapt_func(pst_event_mem);
        if (ul_ret != OAL_SUCC) {
            oam_error_log3(0, OAM_SF_FRW,
                "{_frw_event_lookup_process_entry_::tx adapt process failed!sub_type[0x%x],index[0x%x],ret[%d].}",
                uc_sub_type, uc_index, ul_ret);
            return ul_ret;
        }

        return OAL_SUCC;
    }

    /* For rx adapt */
    if (pst_frw_event_table->pst_sub_table[uc_sub_type].p_rx_adapt_func != OAL_PTR_NULL) {
        /* rx adapt first */
        pst_tmp_event_mem = pst_frw_event_table->pst_sub_table[uc_sub_type].p_rx_adapt_func(pst_event_mem);
        if ((pst_tmp_event_mem != OAL_PTR_NULL) &&
            (pst_frw_event_table->pst_sub_table[uc_sub_type].p_func != OAL_PTR_NULL)) {
            /* then call action frame */
            pst_frw_event_table->pst_sub_table[uc_sub_type].p_func(pst_tmp_event_mem);
            frw_event_free_m(pst_tmp_event_mem);
        } else {
            oam_error_log4(0, OAM_SF_FRW,
                "{frw_event_lookup_process_entry::rx adapt process failed! sub_type[0x%x], index[0x%x].[%d][%d]}",
                uc_sub_type, uc_index, (uintptr_t)pst_tmp_event_mem,
                (uintptr_t)pst_frw_event_table->pst_sub_table[uc_sub_type].p_func);

            if (pst_tmp_event_mem != OAL_PTR_NULL) {
                frw_event_free_m(pst_tmp_event_mem);
            }
            return OAL_ERR_CODE_PTR_NULL;
        }
    }

    return OAL_SUCC;
}


/*
 * �� �� ��  : frw_event_lookup_process_entry
 * ��������  : �����¼����ͣ��������Լ��ֶκţ��ҵ���Ӧ�¼�������
 */
OAL_STATIC OAL_INLINE oal_uint32 _frw_event_lookup_process_entry_(frw_event_mem_stru *pst_event_mem,
                                                                  frw_event_hdr_stru *pst_event_hrd)
{
    oal_uint8 uc_index;
    oal_uint8 uc_sub_type;
    const oal_int32 l_dump_event_len = 64;  /* 64:�����쳣ʱ����¼��ڴ��ǰ64�ֽ� */
    const oal_int32 l_dump_head_len = (oal_int32)(FRW_EVENT_HDR_LEN + 20);  /* 20:����ά���ֶγ��� */
    frw_event_table_item_stru *pst_frw_event_table = NULL;

    uc_sub_type = pst_event_hrd->uc_sub_type;
    /* �����¼����ͼ��ֶκż����¼������� */
    uc_index = (oal_uint8)((pst_event_hrd->en_type << 1) | (pst_event_hrd->en_pipeline & 0x01));
    if (oal_unlikely(uc_index >= FRW_EVENT_TABLE_MAX_ITEMS)) {
        oam_error_log3(0, OAM_SF_FRW,
                       "{frw_event_lookup_process_entry::array overflow! type[0x%x], sub_type[0x%x], pipeline[0x%x]}",
                       pst_event_hrd->en_type, uc_sub_type, pst_event_hrd->en_pipeline);
        return OAL_ERR_CODE_ARRAY_OVERFLOW;
    }

    pst_frw_event_table = &g_event_table[uc_index];
    if (pst_frw_event_table->pst_sub_table == OAL_PTR_NULL) {
        oam_error_log2(0, OAM_SF_FRW,
                       "{frw_event_lookup_process_entry::pst_sub_table is NULL! sub_type[0x%x], index[0x%x].}",
                       uc_sub_type, uc_index);

        /* ��ӡ��ǰ�¼��ڴ�Ĳ������� */
        oal_print_hex_dump(frw_get_event_data(pst_event_mem), l_dump_head_len,
                           HEX_DUMP_GROUP_SIZE, "current event mem info:");

        /* ��ӡ��ǰ�¼��ڴ��ǰ64�ֽ����� */
        oal_print_hex_dump(frw_get_event_data(pst_event_mem) + l_dump_event_len,
                           l_dump_event_len, HEX_DUMP_GROUP_SIZE, "64bytes mem info in front of this mem:");

        return OAL_ERR_CODE_PTR_NULL;
    }

    /* For pipeline 0 type */
    if (pst_event_hrd->en_pipeline == 0) {
        if (pst_frw_event_table->pst_sub_table[uc_sub_type].p_func != OAL_PTR_NULL) {
            pst_frw_event_table->pst_sub_table[uc_sub_type].p_func(pst_event_mem);
            return OAL_SUCC;
        } else {
            oam_error_log2(0, OAM_SF_FRW,
                "{frw_event_lookup_process_entry::p_func is NULL! sub_type[0x%x], index[0x%x], pipeline=0.}",
                uc_sub_type, uc_index);
            return OAL_ERR_CODE_PTR_NULL;
        }
    }

    /* For pipeline 1 type */
    if (oal_unlikely((pst_frw_event_table->pst_sub_table[uc_sub_type].p_tx_adapt_func == OAL_PTR_NULL) &&
                     (pst_frw_event_table->pst_sub_table[uc_sub_type].p_rx_adapt_func == OAL_PTR_NULL))) {
        oam_error_log2(0, OAM_SF_FRW,
            "{frw_event_lookup_process_entry::tx and rx adapt pointer all NULL! sub_type[0x%x], index[0x%x].}",
            uc_sub_type, uc_index);
        return OAL_ERR_CODE_PTR_NULL;
    }

    return frw_event_process_for_trx_adapt(pst_frw_event_table, uc_sub_type, pst_event_mem, uc_index);
}

OAL_STATIC OAL_INLINE oal_uint32 frw_event_lookup_process_entry(frw_event_mem_stru *pst_event_mem,
                                                                frw_event_hdr_stru *pst_event_hrd)
{
    oal_uint32 ul_ret;

    if (oal_unlikely((pst_event_mem == NULL) || (pst_event_hrd == NULL))) {
        oal_warn_on(1);
        return OAL_FAIL;
    }

    ul_ret = _frw_event_lookup_process_entry_(pst_event_mem, pst_event_hrd);
    return ul_ret;
}

/*
 * �� �� ��  : frw_event_report
 * ��������  : ��WiTP�ڴ����֮����¼��ϱ�SDT
 * �������  : pst_event_mem: ָ���¼��ڴ���ָ��
 */
OAL_STATIC OAL_INLINE oal_void frw_event_report(frw_event_mem_stru *pst_event_mem)
{
    frw_event_stru *pst_event = NULL;
    oal_uint8 auc_event[OAM_EVENT_INFO_MAX_LEN] = {0};

    if (oal_unlikely(pst_event_mem == NULL)) {
        oal_warn_on(1);
        return;
    }

    pst_event = frw_get_event_stru(pst_event_mem);
    /* �����¼�ͷ */
    if (memcpy_s((oal_void *)auc_event, OAL_SIZEOF(auc_event),
                 (const oal_void *)&pst_event->st_event_hdr, OAL_SIZEOF(frw_event_hdr_stru)) != EOK) {
        oam_error_log2(0, OAM_SF_FRW, "memcpy_s error, destlen=%u, srclen=%u\n ",
                       OAL_SIZEOF(auc_event), OAL_SIZEOF(frw_event_hdr_stru));
        return;
    }

    frw_event_internal(BROADCAST_MACADDR, 0, OAM_EVENT_INTERNAL, auc_event, OAL_SIZEOF(frw_event_hdr_stru));
}

/*
 * �� �� ��  : frw_event_process
 * ��������  : �����¼��ֶκţ������¼�
 * �������  : pst_event_mem: ָ���¼��ڴ���ָ��
 */
OAL_STATIC OAL_INLINE oal_uint32 frw_event_process(frw_event_mem_stru *pst_event_mem)
{
    frw_event_hdr_stru *pst_event_hrd = NULL;
    oal_uint32 ul_core_id;

    if (oal_unlikely(pst_event_mem == NULL)) {
        oal_warn_on(1);
        return OAL_FAIL;
    }

    /* ��ȡ�¼�ͷ�ṹ */
    pst_event_hrd = (frw_event_hdr_stru *)pst_event_mem->puc_data;

    if (oal_unlikely(pst_event_hrd->en_pipeline >= FRW_EVENT_PIPELINE_STAGE_BUTT)) {
        return OAL_ERR_CODE_ARRAY_OVERFLOW;
    }

    /* ���piplelineΪ0�����¼���ӡ����򣬸����¼����ͣ��������Լ��ֶκţ�ִ����Ӧ���¼������� */
    if (pst_event_hrd->en_pipeline == FRW_EVENT_PIPELINE_STAGE_0) {
        ul_core_id = oal_get_core_id();
        return frw_event_post_event(pst_event_mem, ul_core_id);
    }

    return frw_event_lookup_process_entry(pst_event_mem, pst_event_hrd);
}

#ifdef _PRE_FRW_EVENT_PROCESS_TRACE_DEBUG
OAL_STATIC OAL_INLINE oal_void frw_event_last_pc_trace(const char *pst_func_name, oal_int32 line_num,
                                                       oal_uint32 ul_core_id)
{
    if (oal_unlikely(pst_func_name == NULL)) {
        oal_warn_on(1);
        return;
    }

    if (oal_warn_on(ul_core_id >= WLAN_FRW_MAX_NUM_CORES)) {
        oam_error_log2(0, OAM_SF_FRW, "{frw_event_last_pc_trace: ul_core_id:%d beyond limit:%d}",
                       ul_core_id, WLAN_FRW_MAX_NUM_CORES);
        return;
    }
    g_event_manager[ul_core_id].pst_frw_trace->pst_func_name = pst_func_name;
    g_event_manager[ul_core_id].pst_frw_trace->line_num = line_num;
}

OAL_STATIC OAL_INLINE oal_void frw_event_trace(frw_event_mem_stru *pst_event_mem, oal_uint32 ul_core_id)
{
    oal_uint32 ul_pos;
    frw_event_trace_item_stru *pst_trace_item = NULL;
    frw_event_hdr_stru *pst_event_hdr = NULL;

    if (oal_unlikely(pst_event_mem == NULL)) {
        oal_warn_on(1);
        return;
    }

    pst_event_hdr = frw_get_event_hdr(pst_event_mem);
    ul_pos = g_event_manager[ul_core_id].pst_frw_trace->ul_current_pos;

    if (oal_warn_on(ul_pos >= CONFIG_FRW_MAX_TRACE_EVENT_NUMS)) {
        oam_error_log2(0, OAM_SF_FRW, "{frw_event_last_pc_trace: ul_core_id:%d beyond limit:%d}",
                       ul_pos, CONFIG_FRW_MAX_TRACE_EVENT_NUMS);
        return;
    };

    if (oal_warn_on(ul_core_id >= WLAN_FRW_MAX_NUM_CORES)) {
        oam_error_log2(0, OAM_SF_FRW, "{frw_event_last_pc_trace: ul_core_id:%d beyond limit:%d}",
                       ul_core_id, WLAN_FRW_MAX_NUM_CORES);
        return;
    };

    pst_trace_item = &(g_event_manager[ul_core_id].pst_frw_trace->st_trace_item[ul_pos]);
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 35))
    pst_trace_item->timestamp = local_clock();
#else
    pst_trace_item->timestamp = 0;
#endif
    pst_trace_item->st_event_seg.en_type = pst_event_hdr->en_type;
    pst_trace_item->st_event_seg.uc_sub_type = pst_event_hdr->uc_sub_type;
    pst_trace_item->st_event_seg.uc_vap_id = pst_event_hdr->uc_vap_id;
    pst_trace_item->st_event_seg.en_pipeline = pst_event_hdr->en_pipeline;

    if ((++ul_pos) >= CONFIG_FRW_MAX_TRACE_EVENT_NUMS) {
        g_event_manager[ul_core_id].pst_frw_trace->ul_current_pos = 0;
        g_event_manager[ul_core_id].pst_frw_trace->ul_over_flag = 1;
    } else {
        g_event_manager[ul_core_id].pst_frw_trace->ul_current_pos++;
    }
}
#endif

#ifdef _PRE_OAL_FEATURE_TASK_NEST_LOCK
extern oal_task_lock_stru g_event_task_lock;
#define frw_event_task_lock()                    \
    do {                                         \
        oal_smp_task_lock(&g_event_task_lock); \
    } while (0)
#define frw_event_task_unlock()                    \
    do {                                           \
        oal_smp_task_unlock(&g_event_task_lock); \
    } while (0)
#else
#define frw_event_task_lock() \
    do {                      \
    } while (0)
#define frw_event_task_unlock() \
    do {                        \
    } while (0)
#endif

#endif /* end of frw_event_main.h */
