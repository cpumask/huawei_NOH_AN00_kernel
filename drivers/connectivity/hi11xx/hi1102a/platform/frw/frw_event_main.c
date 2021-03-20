

/* ͷ�ļ����� */
#include "frw_event_main.h"

#include "platform_spec.h"
#include "oam_ext_if.h"
#include "frw_task.h"
#include "frw_main.h"
#include "frw_event_sched.h"
#include "hal_ext_if.h"
#include "oal_kernel_file.h"
#include "securec.h"
#include "frw_timer.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_FRW_EVENT_MAIN_C

/* STRUCT���� */
/*
 * �ṹ��  : frw_event_cfg_stru
 * �ṹ˵��: �¼�����������Ϣ�ṹ��
 */
typedef struct {
    oal_uint8 uc_weight;                   /* ����Ȩ�� */
    oal_uint8 uc_max_events;               /* �����������ɵ�����¼����� */
    frw_sched_policy_enum_uint8 en_policy; /* �����������Ȳ���(�����ȼ�����ͨ���ȼ�) */
    oal_uint8 auc_resv;
} frw_event_cfg_stru;

/*
 * �ṹ��  : frw_event_ipc_register_stru
 * �ṹ˵��: IPCģ��ע��ṹ��
 */
typedef struct {
    oal_uint32 (*p_frw_event_deploy_pipeline_func)(frw_event_mem_stru *pst_event_mem, oal_uint8 *puc_deploy_result);
    oal_uint32 (*p_frw_ipc_event_queue_full_func)(oal_void);
    oal_uint32 (*p_frw_ipc_event_queue_empty_func)(oal_void);
} frw_event_ipc_register_stru;

/* �¼�����������Ϣȫ�ֱ��� */
OAL_STATIC frw_event_cfg_stru event_queue_cfg_table[] = WLAN_FRW_EVENT_CFG_TABLE;

/* �¼�����ʵ�� */
frw_event_mgmt_stru event_manager[WLAN_FRW_MAX_NUM_CORES];

/* �¼���ȫ�ֱ��� */
frw_event_table_item_stru event_table[FRW_EVENT_TABLE_MAX_ITEMS];

/* IPCע�����ʵ�� */
OAL_STATIC frw_event_ipc_register_stru ipc_register;

#if defined(_PRE_DEBUG_MODE)
#if defined(_PRE_EVENT_TIME_TRACK_DEBUG)
frw_event_track_time_stru event_time_track[FRW_RX_EVENT_TRACK_NUM];
oal_uint32 rx_event_idx = 0;
#endif
oal_bool_enum_uint8 en_event_track_switch = OAL_TRUE;
oal_uint32 schedule_idx = 0;
oal_uint32 aul_schedule_time[FRW_RX_EVENT_TRACK_NUM] = {0};
oal_uint32 mac_process_event;  // for debug, adjust the event cnt per frw task scheldued
#endif

#ifdef _PRE_OAL_FEATURE_TASK_NEST_LOCK
/* smp os use the task lock to protect the event process */
oal_task_lock_stru frw_event_task_lock_entity;
oal_module_symbol(frw_event_task_lock_entity);
#endif

#ifdef _PRE_CONFIG_HISI_PANIC_DUMP_SUPPORT
#ifdef _PRE_FRW_EVENT_PROCESS_TRACE_DEBUG
OAL_STATIC oal_int32 frw_trace_print_event_item(frw_event_trace_item_stru *pst_event_trace,
                                                char *buf, oal_int32 buf_len)
{
    oal_int32 ret;
    oal_ulong rem_nsec;
    oal_uint64 timestamp = pst_event_trace->timestamp;

    rem_nsec = do_div(timestamp, 1000000000);
    ret = snprintf_s(buf, buf_len, buf_len - 1, "%u,%u,%u,%u,%5lu.%06lu\n",
                     pst_event_trace->st_event_seg.uc_vap_id,
                     pst_event_trace->st_event_seg.en_pipeline,
                     pst_event_trace->st_event_seg.en_type,
                     pst_event_trace->st_event_seg.uc_sub_type,
                     (oal_ulong)timestamp,
                     rem_nsec / 1000);
    if (ret < 0) {
        OAL_IO_PRINT("log str format err line[%d]\n", __LINE__);
    }
    return ret;
}
#endif

OAL_STATIC oal_int32 frw_print_cpu_count_stat(char *buf, oal_int32 buf_len)
{
#if defined(_PRE_FRW_TIMER_BIND_CPU) && defined(CONFIG_NR_CPUS)
    do {
        oal_uint32 cpu_id;
        oal_int32 count = 0;
        oal_int32 ret;
        for (cpu_id = 0; cpu_id < CONFIG_NR_CPUS; cpu_id++) {
            if (frw_timer_cpu_count[cpu_id]) {
                ret = snprintf_s(buf + count, buf_len - count, buf_len - count - 1, "[cpu:%u]count:%u\n",
                                 cpu_id, frw_timer_cpu_count[cpu_id]);
                if (ret < 0) {
                    OAL_IO_PRINT("log str format err line[%d]\n", __LINE__);
                    return count;
                }
                count += ret;
            }
        }
        return count;
    } while (0);
#else
    return 0;
#endif
}

OAL_STATIC oal_int32 frw_print_panic_stat(oal_void *data, char *buf, oal_int32 buf_len)
{
    oal_int32 ret;
    oal_int32 count = 0;
    oal_uint32 ul_core_id;
    oal_uint32 i;

    OAL_REFERENCE(data);
#ifdef _PRE_OAL_FEATURE_TASK_NEST_LOCK
    if (frw_event_task_lock_entity.claimer) {
        ret = snprintf_s(buf + count, buf_len - count, buf_len - count - 1, "frw task lock claimer:%s\n",
                         frw_event_task_lock_entity.claimer->comm);
        if (ret < 0) {
            OAL_IO_PRINT("log str format err line[%d]\n", __LINE__);
            return count;
        }
        count += ret;
    }
#endif
#ifdef _PRE_FRW_EVENT_PROCESS_TRACE_DEBUG
    for (ul_core_id = 0; ul_core_id < WLAN_FRW_MAX_NUM_CORES; ul_core_id++) {
        ret = snprintf_s(buf + count, buf_len - count, buf_len - count - 1, "last pc:%s,line:%d\n",
                         event_manager[ul_core_id].pst_frw_trace->pst_func_name,
                         event_manager[ul_core_id].pst_frw_trace->line_num);
        if (ret < 0) {
            OAL_IO_PRINT("log str format err line[%d]\n", __LINE__);
            return count;
        }
        count += ret;

#if (_PRE_FRW_FEATURE_PROCCESS_ENTITY_TYPE == _PRE_FRW_FEATURE_PROCCESS_ENTITY_THREAD)
        ret = snprintf_s(buf + count, buf_len - count, buf_len - count - 1,
                         "task thread total cnt:%u,event cnt:%u,empty max count:%u\n",
                         event_task[ul_core_id].ul_total_loop_cnt,
                         event_task[ul_core_id].ul_total_event_cnt,
                         event_task[ul_core_id].ul_max_empty_count);
        if (ret < 0) {
            OAL_IO_PRINT("log str format err line[%d]\n", __LINE__);
            return count;
        }
        count += ret;
#endif

        ret = snprintf_s(buf + count, buf_len - count, buf_len - count - 1, "frw event trace buff:\n");
        if (ret < 0) {
            OAL_IO_PRINT("log str format err line[%d]\n", __LINE__);
            return count;
        }
        count += ret;

        count += frw_print_cpu_count_stat(buf + count, buf_len - count);

        if (event_manager[ul_core_id].pst_frw_trace->ul_over_flag == 1) {
            /* overturn */
            for (i = event_manager[ul_core_id].pst_frw_trace->ul_current_pos;
                 i < CONFIG_FRW_MAX_TRACE_EVENT_NUMS; i++) {
                ret = frw_trace_print_event_item(&event_manager[ul_core_id].pst_frw_trace->st_trace_item[i],
                                                 buf + count, buf_len - count);
                if (ret < 0) {
                    OAL_IO_PRINT("log str format err line[%d]\n", __LINE__);
                    return count;
                }
                count += ret;
            }
        }

        i = 0;
        for (i = 0; i < event_manager[ul_core_id].pst_frw_trace->ul_current_pos; i++) {
            ret = frw_trace_print_event_item(&event_manager[ul_core_id].pst_frw_trace->st_trace_item[i],
                                             buf + count, buf_len - count);
            if (ret < 0) {
                OAL_IO_PRINT("log str format err line[%d]\n", __LINE__);
                return count;
            }
            count += ret;
        }
    }
#else
    OAL_REFERENCE(i);
    OAL_REFERENCE(ul_core_id);
    OAL_REFERENCE(ret);
    OAL_REFERENCE(count);
#endif
    return count;
}
OAL_STATIC DECLARE_WIFI_PANIC_STRU(frw_panic_stat, frw_print_panic_stat);
#endif

/*
 * �� �� ��  : frw_event_init_event_queue
 * ��������  : ��ʼ���¼�����
 * �� �� ֵ  : OAL_SUCC ������������
 */
OAL_STATIC oal_uint32 frw_event_init_event_queue(oal_void)
{
    oal_uint32 ul_core_id;
    oal_uint16 us_qid;
    oal_uint32 ul_ret;

    for (ul_core_id = 0; ul_core_id < WLAN_FRW_MAX_NUM_CORES; ul_core_id++) {
        /* ѭ����ʼ���¼����� */
        for (us_qid = 0; us_qid < FRW_EVENT_MAX_NUM_QUEUES; us_qid++) {
            ul_ret = frw_event_queue_init(&event_manager[ul_core_id].st_event_queue[us_qid],
                                          event_queue_cfg_table[us_qid].uc_weight,
                                          event_queue_cfg_table[us_qid].en_policy,
                                          FRW_EVENT_QUEUE_STATE_INACTIVE,
                                          event_queue_cfg_table[us_qid].uc_max_events);

            if (OAL_UNLIKELY(ul_ret != OAL_SUCC)) {
                OAM_WARNING_LOG1(0, OAM_SF_FRW,
                                 "{frw_event_init_event_queue, frw_event_queue_init return != OAL_SUCC!%d}",
                                 ul_ret);
                return ul_ret;
            }
        }
    }

    return OAL_SUCC;
}

/*
 * �� �� ��  : frw_event_destroy_event_queue
 * ��������  : �����¼�����
 * �������  : ul_core_id: �˺�
 */
OAL_STATIC oal_void frw_event_destroy_event_queue(oal_uint32 ul_core_id)
{
    oal_uint16 us_qid;

    /* ѭ�������¼����� */
    for (us_qid = 0; us_qid < FRW_EVENT_MAX_NUM_QUEUES; us_qid++) {
        frw_event_queue_destroy(&event_manager[ul_core_id].st_event_queue[us_qid]);
    }
}

/*
 * �� �� ��  : frw_event_init_sched
 * ��������  : ��ʼ��������
 * �� �� ֵ  : OAL_SUCC ������������
 */
OAL_STATIC oal_uint32 frw_event_init_sched(oal_void)
{
    oal_uint32 ul_core_id;
    oal_uint16 us_qid;
    oal_uint32 ul_ret;

    for (ul_core_id = 0; ul_core_id < WLAN_FRW_MAX_NUM_CORES; ul_core_id++) {
        /* ѭ����ʼ�������� */
        for (us_qid = 0; us_qid < FRW_SCHED_POLICY_BUTT; us_qid++) {
            ul_ret = frw_event_sched_init(&event_manager[ul_core_id].st_sched_queue[us_qid]);

            if (OAL_UNLIKELY(ul_ret != OAL_SUCC)) {
                OAM_WARNING_LOG1(0, OAM_SF_FRW,
                                 "{frw_event_init_sched, frw_event_sched_init return != OAL_SUCC!%d}",
                                 ul_ret);
                return ul_ret;
            }
        }
    }

    return OAL_SUCC;
}

#ifdef _PRE_FRW_EVENT_PROCESS_TRACE_DEBUG

OAL_STATIC oal_uint32 frw_event_trace_init(oal_void)
{
    oal_uint32 ul_core_id;
    for (ul_core_id = 0; ul_core_id < WLAN_FRW_MAX_NUM_CORES; ul_core_id++) {
        event_manager[ul_core_id].pst_frw_trace = (frw_event_trace_stru *)vmalloc(OAL_SIZEOF(frw_event_trace_stru));
        if (event_manager[ul_core_id].pst_frw_trace == NULL) {
            OAL_IO_PRINT("frw_event_init_sched coreid:%u, alloc frw event trace %u bytes failed! \n",
                         ul_core_id,
                         (oal_uint32)OAL_SIZEOF(frw_event_trace_stru));
            return OAL_ERR_CODE_PTR_NULL;
        }
        memset_s((oal_void *)event_manager[ul_core_id].pst_frw_trace,
                 OAL_SIZEOF(frw_event_trace_stru), 0, OAL_SIZEOF(frw_event_trace_stru));
    }
    return OAL_SUCC;
}

OAL_STATIC oal_void frw_event_trace_exit(oal_void)
{
    oal_uint32 ul_core_id;
    for (ul_core_id = 0; ul_core_id < WLAN_FRW_MAX_NUM_CORES; ul_core_id++) {
        if (event_manager[ul_core_id].pst_frw_trace != NULL) {
            vfree(event_manager[ul_core_id].pst_frw_trace);
            event_manager[ul_core_id].pst_frw_trace = NULL;
        }
    }
}
#endif

/*
 * �� �� ��  : frw_event_dispatch_event
 * ��������  : �¼��ַ��ӿ�(�ַ��¼����˼�ͨѶ���¼����С����߲��Ѱ����Ӧ�¼�������)
 * �������  : pst_event_mem: ָ���¼��ڴ���ָ��
 * �� �� ֵ  : OAL_SUCC ������������
 */
oal_uint32 frw_event_dispatch_event(frw_event_mem_stru *pst_event_mem)
{
#if (_PRE_MULTI_CORE_MODE_PIPELINE_AMP == _PRE_MULTI_CORE_MODE)
    frw_event_deploy_enum_uint8 en_deploy;
    oal_uint32 ul_ret;
#endif

#ifdef _PRE_DEBUG_MODE
    oal_uint32 ul_dog_tag;
#endif

    if (OAL_UNLIKELY(pst_event_mem == OAL_PTR_NULL)) {
        OAM_ERROR_LOG0(0, OAM_SF_FRW, "{frw_event_dispatch_event: pst_event_mem is null ptr!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* �쳣: �ڴ�дԽ�� */
#ifdef _PRE_DEBUG_MODE
    ul_dog_tag = (*((oal_uint32 *)(pst_event_mem->puc_origin_data + pst_event_mem->us_len - OAL_DOG_TAG_SIZE)));
    if (ul_dog_tag != OAL_DOG_TAG) {
        OAM_ERROR_LOG1(0, OAM_SF_FRW, "frw_event_dispatch_event, ul_dog_tag changed is[%d]changed", ul_dog_tag);
        return OAL_ERR_CODE_OAL_MEM_DOG_TAG;
    }
    frw_event_report(pst_event_mem);
#endif

#if (_PRE_MULTI_CORE_MODE_PIPELINE_AMP == _PRE_MULTI_CORE_MODE)
    /* ���û�п����˼�ͨ�ţ�������¼��ֶκŴ����¼�(��ӻ���ִ����Ӧ�Ĵ�����) */
    if (st_ipc_register.p_frw_event_deploy_pipeline_func == OAL_PTR_NULL) {
        return frw_event_process(pst_event_mem);
    }

    ul_ret = st_ipc_register.p_frw_event_deploy_pipeline_func(pst_event_mem, &en_deploy);
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG1(0, OAM_SF_FRW,
                         "{frw_event_dispatch_event, p_frw_event_deploy_pipeline_func return != OAL_SUCC!%d}",
                         ul_ret);
        return ul_ret;
    }

    /* ���Ϊ�˼�ͨ�ţ���ֱ�ӷ��سɹ������򣬸����¼��ֶκŴ����¼� */
    if (en_deploy == FRW_EVENT_DEPLOY_IPC) {
        return OAL_SUCC;
    }
#endif

    return frw_event_process(pst_event_mem);
}

#if defined(_PRE_CONFIG_CONN_HISI_SYSFS_SUPPORT) && defined(_PRE_CONFIG_HISI_PANIC_DUMP_SUPPORT)
OAL_STATIC ssize_t frw_get_event_trace(struct kobject *dev, struct kobj_attribute *attr, char *buf)
{
    int ret = 0;

    if (buf == NULL) {
        OAL_IO_PRINT("buf is null r failed!%s\n", __FUNCTION__);
        return 0;
    }

    if (attr == NULL) {
        OAL_IO_PRINT("attr is null r failed!%s\n", __FUNCTION__);
        return 0;
    }

    if (dev == NULL) {
        OAL_IO_PRINT("dev is null r failed!%s\n", __FUNCTION__);
        return 0;
    }

    ret += frw_print_panic_stat(NULL, buf, PAGE_SIZE - ret);
    return ret;
}
OAL_STATIC struct kobj_attribute dev_attr_event_trace =
    __ATTR(event_trace, S_IRUGO, frw_get_event_trace, NULL);

OAL_STATIC struct attribute *frw_sysfs_entries[] = {
    &dev_attr_event_trace.attr,
    NULL
};

OAL_STATIC struct attribute_group frw_attribute_group = {
    .name = "frw",
    .attrs = frw_sysfs_entries,
};

OAL_STATIC oal_int32 frw_sysfs_entry_init(oal_void)
{
    oal_int32 ret;
    oal_kobject *pst_root_object = NULL;
    pst_root_object = oal_get_sysfs_root_object();
    if (pst_root_object == NULL) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{frw_sysfs_entry_init::get sysfs root object failed!}");
        return -OAL_EFAIL;
    }

    ret = oal_debug_sysfs_create_group(pst_root_object, &frw_attribute_group);
    if (ret) {
        OAM_ERROR_LOG0(0, OAM_SF_ANY, "{frw_sysfs_entry_init::sysfs create group failed!}");
        return ret;
    }
    return OAL_SUCC;
}

OAL_STATIC oal_int32 frw_sysfs_entry_exit(oal_void)
{
    oal_kobject *pst_root_object = NULL;
    pst_root_object = oal_get_sysfs_root_object();
    if (pst_root_object != NULL) {
        oal_debug_sysfs_remove_group(pst_root_object, &frw_attribute_group);
    }

    return OAL_SUCC;
}
#endif

/*
 * �� �� ��  : frw_event_init
 * ��������  : �¼�����ģ���ʼ�������
 * �� �� ֵ  : OAL_SUCC ������������
 */
oal_uint32 frw_event_init(oal_void)
{
    oal_uint32 ul_ret;

    memset_s(&ipc_register, OAL_SIZEOF(ipc_register), 0, OAL_SIZEOF(ipc_register));
#if defined(_PRE_DEBUG_MODE) && defined(_PRE_EVENT_TIME_TRACK_DEBUG)

    memset_s(&event_time_track, FRW_RX_EVENT_TRACK_NUM * OAL_SIZEOF(frw_event_track_time_stru),
             0, FRW_RX_EVENT_TRACK_NUM * OAL_SIZEOF(frw_event_track_time_stru));
#endif

#ifdef _PRE_DEBUG_MODE
    mac_process_event = 0;
#endif

#ifdef _PRE_OAL_FEATURE_TASK_NEST_LOCK
    oal_smp_task_lock_init(&frw_event_task_lock_entity);
#endif

    /* ��ʼ���¼����� */
    ul_ret = frw_event_init_event_queue();
    if (OAL_UNLIKELY(ul_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_FRW, "{frw_event_init, frw_event_init_event_queue != OAL_SUCC!%d}", ul_ret);
        return ul_ret;
    }

    /* ��ʼ�������� */
    ul_ret = frw_event_init_sched();
    if (OAL_UNLIKELY(ul_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_FRW, "frw_event_init, frw_event_init_sched != OAL_SUCC!%d", ul_ret);
        return ul_ret;
    }

#ifdef _PRE_FRW_EVENT_PROCESS_TRACE_DEBUG
    ul_ret = frw_event_trace_init();
    if (OAL_UNLIKELY(ul_ret != OAL_SUCC)) {
        OAM_ERROR_LOG1(0, OAM_SF_FRW, "frw_event_init, frw_event_trace_init != OAL_SUCC!%d", ul_ret);
        return ul_ret;
    }
#endif

#ifdef _PRE_CONFIG_HISI_PANIC_DUMP_SUPPORT
    hwifi_panic_log_register(&frw_panic_stat, NULL);
#endif

    frw_task_event_handler_register(frw_event_process_all_event);

#if defined(_PRE_CONFIG_CONN_HISI_SYSFS_SUPPORT) && defined(_PRE_CONFIG_HISI_PANIC_DUMP_SUPPORT)
    frw_sysfs_entry_init();
#endif

    return OAL_SUCC;
}

/*
 * �� �� ��  : frw_event_exit
 * ��������  : �¼�����ģ��ж�ؽӿ�
 * �� �� ֵ  : OAL_SUCC ������������
 */
oal_uint32 frw_event_exit(oal_void)
{
    oal_uint32 ul_core_id;
#if defined(_PRE_CONFIG_CONN_HISI_SYSFS_SUPPORT) && defined(_PRE_CONFIG_HISI_PANIC_DUMP_SUPPORT)
    frw_sysfs_entry_exit();
#endif

#ifdef _PRE_FRW_EVENT_PROCESS_TRACE_DEBUG
    frw_event_trace_exit();
#endif

    for (ul_core_id = 0; ul_core_id < WLAN_FRW_MAX_NUM_CORES; ul_core_id++) {
        /* �����¼����� */
        frw_event_destroy_event_queue(ul_core_id);
    }

    return OAL_SUCC;
}

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
oal_void frw_event_sub_rx_adapt_table_init(frw_event_sub_table_item_stru *pst_sub_table, oal_uint32 ul_table_nums,
                                           frw_event_mem_stru *(*p_rx_adapt_func)(frw_event_mem_stru *))
{
    oal_uint32 i;
    frw_event_sub_table_item_stru *pst_curr_table = NULL;
    for (i = 0; i < ul_table_nums; i++) {
        pst_curr_table = pst_sub_table + i;
        pst_curr_table->p_rx_adapt_func = p_rx_adapt_func;
    }
}
#endif

/*
 * �� �� ��  : frw_event_queue_enqueue
 * ��������  : ���¼��ڴ������Ӧ���¼�����
 * �������  : pst_event_mem: ָ���¼��ڴ���ָ��
 * �� �� ֵ  : OAL_SUCC ������������
 */
oal_uint32 frw_event_queue_enqueue(frw_event_queue_stru *pst_event_queue, frw_event_mem_stru *pst_event_mem)
{
    oal_uint32 ul_ret;
    oal_uint ul_irq_flag;

    if (OAL_UNLIKELY((pst_event_queue == NULL) || (pst_event_mem == NULL))) {
        OAL_WARN_ON(1);
        return OAL_FAIL;
    }

    oal_spin_lock_irq_save(&pst_event_queue->st_lock, &ul_irq_flag);
    ul_ret = oal_queue_enqueue(&pst_event_queue->st_queue, (void *)pst_event_mem);
    oal_spin_unlock_irq_restore(&pst_event_queue->st_lock, &ul_irq_flag);
    return ul_ret;
}

/*
 * �� �� ��  : frw_event_queue_dequeue
 * ��������  : �¼��ڴ����
 * �������  : pst_event_queue: �¼�����
 * �� �� ֵ  : OAL_SUCC ������������
 */
frw_event_mem_stru *frw_event_queue_dequeue(frw_event_queue_stru *pst_event_queue)
{
    frw_event_mem_stru *pst_event_mem = NULL;
    oal_uint ul_irq_flag;

    if (OAL_UNLIKELY(pst_event_queue == NULL)) {
        OAL_WARN_ON(1);
        return NULL;
    }

    oal_spin_lock_irq_save(&pst_event_queue->st_lock, &ul_irq_flag);
    pst_event_mem = (frw_event_mem_stru *)oal_queue_dequeue(&pst_event_queue->st_queue);
    oal_spin_unlock_irq_restore(&pst_event_queue->st_lock, &ul_irq_flag);
    return pst_event_mem;
}

/*
 * �� �� ��  : frw_event_post_event
 * ��������  : ���¼��ڴ������Ӧ���¼�����
 * �������  : pst_event_mem: ָ���¼��ڴ���ָ��
 * �� �� ֵ  : OAL_SUCC ������������
 */
oal_uint32 frw_event_post_event(frw_event_mem_stru *pst_event_mem, oal_uint32 ul_core_id)
{
    oal_uint16 us_qid;
    frw_event_mgmt_stru *pst_event_mgmt = NULL;
    frw_event_queue_stru *pst_event_queue = NULL;
    oal_uint32 ul_ret;
    frw_event_hdr_stru *pst_event_hdr = NULL;
    frw_event_sched_queue_stru *pst_sched_queue = NULL;

    if (OAL_UNLIKELY(pst_event_mem == NULL)) {
        OAL_WARN_ON(1);
        return OAL_FAIL;
    }

    /* ��ȡ�¼�����ID */
    ul_ret = frw_event_to_qid(pst_event_mem, &us_qid);
    if (OAL_UNLIKELY(ul_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_FRW, "{frw_event_post_event, frw_event_to_qid return != OAL_SUCC!%d}", ul_ret);
        return ul_ret;
    }

    if (OAL_UNLIKELY(ul_core_id >= WLAN_FRW_MAX_NUM_CORES)) {
        OAM_ERROR_LOG1(0, OAM_SF_FRW, "{frw_event_post_event, array overflow!%d}", ul_core_id);
        return OAL_ERR_CODE_ARRAY_OVERFLOW;
    }

    /* ���ݺ˺� + ����ID���ҵ���Ӧ���¼����� */
    pst_event_mgmt = &event_manager[ul_core_id];

    pst_event_queue = &pst_event_mgmt->st_event_queue[us_qid];

    /* ���policy */
    if (OAL_UNLIKELY(pst_event_queue->en_policy >= FRW_SCHED_POLICY_BUTT)) {
        OAM_ERROR_LOG1(0, OAM_SF_FRW, "{frw_event_post_event, array overflow!%d}", pst_event_queue->en_policy);
        return OAL_ERR_CODE_ARRAY_OVERFLOW;
    }

    /* ��ȡ���ȶ��� */
    pst_sched_queue = &pst_event_mgmt->st_sched_queue[pst_event_queue->en_policy];

    /* �¼��ڴ����ü�����1 */
#ifdef _PRE_DEBUG_MODE
    /* �쳣: ���ڴ���ϵĹ����û�����Ϊ���ֵ */
    if (OAL_UNLIKELY((oal_uint16)(pst_event_mem->uc_user_cnt + 1) > WLAN_MEM_MAX_USERS_NUM)) {
        OAM_WARNING_LOG1(0, OAM_SF_FRW, "{pst_event_mem->uc_user_cnt is too large.%d}", pst_event_mem->uc_user_cnt);
        return OAL_ERR_CODE_ARRAY_OVERFLOW;
    }
#endif
    /* ��ȡ�����ã���ֹenqueue��ȡ������֮�䱻�ͷ� */
    pst_event_mem->uc_user_cnt++;

    /* �¼���� */
    ul_ret = frw_event_queue_enqueue(pst_event_queue, pst_event_mem);
    if (OAL_UNLIKELY(ul_ret != OAL_SUCC)) {
        pst_event_hdr = (frw_event_hdr_stru *)(frw_get_event_data(pst_event_mem));
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
        OAM_WARNING_LOG4(0, OAM_SF_FRW,
                         "frw_event_post_event:: enqueue fail. core %d, type %d, sub type %d, pipeline %d ",
                         ul_core_id,
                         pst_event_hdr->en_type,
                         pst_event_hdr->uc_sub_type,
                         pst_event_hdr->en_pipeline);

        OAM_WARNING_LOG4(0, OAM_SF_FRW, "event info: type: %d, sub type: %d, pipeline: %d,max num:%d",
                         pst_event_hdr->en_type,
                         pst_event_hdr->uc_sub_type,
                         pst_event_hdr->en_pipeline,
                         pst_event_queue->st_queue.uc_max_elements);
#else
        /* ��CPU��̬��Ƶ������������¼����ʧ�ܣ���ʱ�޸�Ϊwarning */
        OAM_WARNING_LOG4(0, OAM_SF_FRW,
                         "frw_event_post_event:: enqueue fail. core %d, type %d, sub type %d, pipeline %d ",
                         ul_core_id,
                         pst_event_hdr->en_type,
                         pst_event_hdr->uc_sub_type,
                         pst_event_hdr->en_pipeline);

        OAM_WARNING_LOG4(0, OAM_SF_FRW, "event info: type: %d, sub type: %d, pipeline: %d,max num:%d",
                         pst_event_hdr->en_type,
                         pst_event_hdr->uc_sub_type,
                         pst_event_hdr->en_pipeline,
                         pst_event_queue->st_queue.uc_max_elements);

        /* ������mac error�����ά��ѶϢ������ʲô�����¶������ */
        if ((pst_event_hdr->en_type == FRW_EVENT_TYPE_HIGH_PRIO)
            && (pst_event_hdr->uc_sub_type == HAL_EVENT_ERROR_IRQ_MAC_ERROR)
            && (pst_event_hdr->en_pipeline == FRW_EVENT_PIPELINE_STAGE_0)) {
            frw_event_stru *pst_event;
            hal_error_irq_event_stru *pst_error_irq_event;
            oal_uint32 ul_error1_irq_state = 0;
            pst_event = frw_get_event_stru(pst_event_mem);
            pst_error_irq_event = (hal_error_irq_event_stru *)(pst_event->auc_event_data);
            ul_error1_irq_state = pst_error_irq_event->st_error_state.ul_error1_val;
            OAM_ERROR_LOG1(0, OAM_SF_FRW, "mac error event enqueue fail: error status:0x%08x", ul_error1_irq_state);
        }

#endif
        /* �ͷ��¼��ڴ����� */
        FRW_EVENT_FREE(pst_event_mem);

        return ul_ret;
    }

    /* �����������Ȳ��ԣ����¼����м���ɵ��ȶ��� */
    ul_ret = frw_event_sched_activate_queue(pst_sched_queue, pst_event_queue);
    if (OAL_UNLIKELY(ul_ret != OAL_SUCC)) {
        OAM_ERROR_LOG1(0, OAM_SF_FRW,
                       "{frw_event_post_event, frw_event_sched_activate_queue return != OAL_SUCC! %d}",
                       ul_ret);
        return ul_ret;
    }

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#ifdef _PRE_DEBUG_MODE
#if (_PRE_FRW_FEATURE_PROCCESS_ENTITY_TYPE == _PRE_FRW_FEATURE_PROCCESS_ENTITY_TASKLET)
    if (en_event_track_switch == OAL_TRUE) {
        if (!oal_task_is_scheduled(&event_task[ul_core_id].st_event_tasklet)) {
            aul_schedule_time[schedule_idx] = oal_5115timer_get_10ns();
        }
    }
#endif
#endif
#endif

    frw_task_sched(ul_core_id);

    return OAL_SUCC;
}

/*
 * �� �� ��  : frw_event_table_register
 * ��������  : ע����Ӧ�¼���Ӧ���¼�������
 * �������  : en_type:       �¼�����
 *             en_pipeline:   �¼��ֶκ�
 *             pst_sub_table: �¼��ӱ�ָ��
 */
oal_void frw_event_table_register(frw_event_type_enum_uint8 en_type,
                                  frw_event_pipeline_enum en_pipeline,
                                  frw_event_sub_table_item_stru *pst_sub_table)
{
    oal_uint8 uc_index;

    if (OAL_UNLIKELY(pst_sub_table == OAL_PTR_NULL)) {
        OAM_ERROR_LOG0(0, OAM_SF_FRW, "{frw_event_table_register: pst_sub_table is null ptr!}");
        return;
    }

    /* �����¼����ͼ��ֶκż����¼������� */
    uc_index = (((oal_uint8)en_type << 1) | ((oal_uint8)en_pipeline & 0x01));

    if (OAL_UNLIKELY(uc_index >= FRW_EVENT_TABLE_MAX_ITEMS)) {
        OAM_ERROR_LOG1(0, OAM_SF_FRW, "{frw_event_table_register, array overflow! %d}", uc_index);
        return;
    }

    event_table[uc_index].pst_sub_table = pst_sub_table;
}

/*
 * �� �� ��  : frw_event_deploy_register
 * ��������  : ��event deployģ��ע���¼�����ӿ�
 * �������  : p_func: �¼�����ӿ�
 */
oal_void frw_event_deploy_register(oal_uint32 (*p_func)(frw_event_mem_stru *pst_event_mem,
                                                        frw_event_deploy_enum_uint8 *pen_deploy_result))
{
    if (OAL_UNLIKELY(p_func == NULL)) {
        OAL_WARN_ON(1);
        return;
    }
    ipc_register.p_frw_event_deploy_pipeline_func = p_func;
}

/*
 * �� �� ��  : frw_event_ipc_event_queue_full_register
 * ��������  : ��IPCģ��ע��˼��ж�Ƶ�ȹ���ӿ�
 * �������  : p_func: �˼��ж�Ƶ�ȹ���ӿ�
 */
oal_void frw_event_ipc_event_queue_full_register(oal_uint32 (*p_func)(oal_void))
{
    if (OAL_UNLIKELY(p_func == NULL)) {
        OAL_WARN_ON(1);
        return;
    }
    ipc_register.p_frw_ipc_event_queue_full_func = p_func;
}

/*
 * �� �� ��  : frw_event_ipc_event_queue_empty_register
 * ��������  : ��IPCģ��ע��˼��ж�Ƶ�ȹ���ӿ�
 * �������  : p_func: �˼��ж�Ƶ�ȹ���ӿ�
 */
oal_void frw_event_ipc_event_queue_empty_register(oal_uint32 (*p_func)(oal_void))
{
    if (OAL_UNLIKELY(p_func == NULL)) {
        OAL_WARN_ON(1);
        return;
    }
    ipc_register.p_frw_ipc_event_queue_empty_func = p_func;
}

/*
 * �� �� ��  : frw_event_process_all_event
 * ��������  : �����¼������е������¼�
 */
oal_void frw_event_process_all_event(oal_uint ui_data)
{
    oal_uint32 ul_core_id;
    frw_event_mem_stru *pst_event_mem = NULL;
    frw_event_sched_queue_stru *pst_sched_queue = NULL;
    frw_event_hdr_stru *pst_event_hrd = NULL;
    oal_uint32 ul_mac_process_event = FRW_PROCESS_MAX_EVENT;
    const oal_uint32 ul_frw_max_event = 500;
#ifdef _PRE_FEATURE_WAVEAPP_CLASSIFY
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC != _PRE_MULTI_CORE_MODE)
    frw_event_stru *pst_event = NULL;
    hal_wlan_rx_event_stru *pst_wlan_rx_event = NULL;
    hal_to_dmac_device_stru *pst_device = NULL;
#endif
#endif

#ifdef _PRE_DEBUG_MODE
#if defined(_PRE_EVENT_TIME_TRACK_DEBUG)
    oal_uint32 ul_timestamp_start;
    oal_uint32 ul_timestamp_end;
    oal_uint32 ul_event_index;
#endif
    if (en_event_track_switch == OAL_TRUE) {
        aul_schedule_time[schedule_idx] = aul_schedule_time[schedule_idx] - oal_5115timer_get_10ns();
        OAL_INCR(schedule_idx, FRW_RX_EVENT_TRACK_NUM);
    }
#endif

    /* ��ȡ�˺� */
    ul_core_id = OAL_GET_CORE_ID();

    if (OAL_UNLIKELY(ul_core_id >= WLAN_FRW_MAX_NUM_CORES)) {
        OAM_ERROR_LOG1(0, OAM_SF_FRW, "{frw_event_process_all_event, array overflow! %d}", ul_core_id);
        return;
    }

#ifdef _PRE_DEBUG_MODE
    if ((mac_process_event) && (mac_process_event <= ul_frw_max_event)) {
        ul_mac_process_event = mac_process_event;
    }
#endif

    pst_sched_queue = event_manager[ul_core_id].st_sched_queue;

    /* �����¼�����ģ�飬ѡ��һ���¼� */
    pst_event_mem = (frw_event_mem_stru *)frw_event_schedule(pst_sched_queue);

#if defined(_PRE_DEBUG_MODE) && defined(_PRE_EVENT_TIME_TRACK_DEBUG)

    if (en_event_track_switch == OAL_TRUE) {
        OAL_INCR(rx_event_idx, FRW_RX_EVENT_TRACK_NUM);
        event_time_track[rx_event_idx].ul_event_cnt = 0;
    }

#endif

    while (pst_event_mem != OAL_PTR_NULL) {
        /* ��ȡ�¼�ͷ�ṹ */
        pst_event_hrd = (frw_event_hdr_stru *)frw_get_event_data(pst_event_mem);
#if defined(_PRE_DEBUG_MODE) && defined(_PRE_EVENT_TIME_TRACK_DEBUG)
        ul_timestamp_start = oal_5115timer_get_10ns();
#endif

#ifdef _PRE_FEATURE_WAVEAPP_CLASSIFY
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC != _PRE_MULTI_CORE_MODE)
        if (pst_event_hrd->en_type == FRW_EVENT_TYPE_WLAN_DRX) {
            /* ��ȡ�¼�ͷ���¼��ṹ��ָ�� */
            pst_event = frw_get_event_stru(pst_event_mem);
            pst_wlan_rx_event = (hal_wlan_rx_event_stru *)(pst_event->auc_event_data);
            pst_device = pst_wlan_rx_event->pst_hal_device;

            if (OAL_UNLIKELY(pst_device == OAL_PTR_NULL)) {
                OAM_ERROR_LOG0(0, OAM_SF_RX, "{frw_event_process_all_event::pst_device null.}");
                break;
            }

            if (pst_device->en_test_is_on_waveapp_flag == OAL_TRUE) {
                ul_mac_process_event = 1;
            }
        }
#endif
#endif

#ifdef _PRE_FRW_EVENT_PROCESS_TRACE_DEBUG
        /* trace the event serial */
        frw_event_trace(pst_event_mem, ul_core_id);
#endif
        /* �����¼��ҵ���Ӧ���¼������� */
        frw_event_task_lock();
        frw_event_lookup_process_entry(pst_event_mem, pst_event_hrd);
        frw_event_task_unlock();

#if defined(_PRE_DEBUG_MODE) && defined(_PRE_EVENT_TIME_TRACK_DEBUG)
        if (en_event_track_switch == OAL_TRUE) {
            ul_timestamp_end = oal_5115timer_get_10ns();
            ul_event_index = event_time_track[rx_event_idx].ul_event_cnt;
            if (ul_event_index < FRW_EVENT_TRACK_NUM) {
                event_time_track[rx_event_idx].aul_event_time[ul_event_index] = ul_timestamp_start -
                                                                                           ul_timestamp_end;
                event_time_track[rx_event_idx].us_event_type[ul_event_index] = pst_event_hrd->en_type;
                event_time_track[rx_event_idx].us_event_sub_type[ul_event_index] = pst_event_hrd->uc_sub_type;
            }
            event_time_track[rx_event_idx].ul_event_cnt++;
        }

#endif
        /* �ͷ��¼��ڴ� */
        FRW_EVENT_FREE(pst_event_mem);
#if (_PRE_FRW_FEATURE_PROCCESS_ENTITY_TYPE == _PRE_FRW_FEATURE_PROCCESS_ENTITY_THREAD)
        if (OAL_LIKELY(ul_core_id < WLAN_FRW_MAX_NUM_CORES)) {
            event_task[ul_core_id].ul_total_event_cnt++;
        }
#endif

#ifdef _PRE_FRW_EVENT_PROCESS_TRACE_DEBUG
        frw_event_last_pc_trace(__FUNCTION__, __LINE__, ul_core_id);
#endif

        if (--ul_mac_process_event) {
            /* �����¼�����ģ�飬ѡ��һ���¼� */
            pst_event_mem = (frw_event_mem_stru *)frw_event_schedule(pst_sched_queue);
        } else {
            frw_task_sched(ul_core_id);
            break;
        }
    }

#ifdef _PRE_FRW_EVENT_PROCESS_TRACE_DEBUG
    frw_event_last_pc_trace(__FUNCTION__, __LINE__, ul_core_id);
#endif
}

/*
 * �� �� ��  : frw_event_flush_event_queue
 * ��������  : ���ĳ���¼������е������¼�
 * �� �� ֵ  : OAL_SUCC ������������
 */
oal_uint32 frw_event_flush_event_queue(frw_event_type_enum_uint8 uc_event_type)
{
    oal_uint32 ul_core_id;
    oal_uint16 us_qid;
    oal_uint8 uc_vap_id;
    frw_event_mgmt_stru *pst_event_mgmt = NULL;
    frw_event_queue_stru *pst_event_queue = NULL;
    frw_event_mem_stru *pst_event_mem = NULL;
    frw_event_hdr_stru *pst_event_hrd = NULL;
    oal_uint32 ul_event_succ = 0;
    ;

    /* ����ÿ���˵�ÿ��vap��Ӧ���¼����� */
    for (ul_core_id = 0; ul_core_id < WLAN_FRW_MAX_NUM_CORES; ul_core_id++) {
        for (uc_vap_id = 0; uc_vap_id < WLAN_VAP_SUPPORT_MAX_NUM_LIMIT; uc_vap_id++) {
            us_qid = uc_vap_id * FRW_EVENT_TYPE_BUTT + uc_event_type;

            /* ���ݺ˺� + ����ID���ҵ���Ӧ���¼����� */
            pst_event_mgmt = &event_manager[ul_core_id];
            pst_event_queue = &pst_event_mgmt->st_event_queue[us_qid];

            /* flush���е�event */
            while (pst_event_queue->st_queue.uc_element_cnt != 0) {
                pst_event_mem = (frw_event_mem_stru *)frw_event_queue_dequeue(pst_event_queue);
                if (pst_event_mem == OAL_PTR_NULL) {
                    continue;
                }

                /* ��ȡ�¼�ͷ�ṹ */
                pst_event_hrd = (frw_event_hdr_stru *)frw_get_event_data(pst_event_mem);

                /* �����¼��ҵ���Ӧ���¼������� */
                frw_event_lookup_process_entry(pst_event_mem, pst_event_hrd);

                /* �ͷ��¼��ڴ� */
                FRW_EVENT_FREE(pst_event_mem);

                ul_event_succ++;
            }
#if 1
            /* ����¼����б�գ���Ҫ����ӵ��ȶ�����ɾ���������¼�����״̬��Ϊ����Ծ(���ɱ�����) */
            if (pst_event_queue->st_queue.uc_element_cnt == 0) {
                frw_event_sched_deactivate_queue(&event_manager[ul_core_id].st_sched_queue[pst_event_queue->en_policy],
                                                 pst_event_queue);
            }
#endif
        }
    }

    return ul_event_succ;
}

/*
 * �� �� ��  : frw_event_dump_event
 * ��������  : ��ӡ�¼�
 * �������  : puc_event: �¼��ṹ���׵�ַ
 */
oal_void frw_event_dump_event(oal_uint8 *puc_event)
{
    frw_event_stru *pst_event = (frw_event_stru *)puc_event;
    frw_event_hdr_stru *pst_event_hdr = &pst_event->st_event_hdr;
    oal_uint8 *puc_payload = pst_event->auc_event_data;
    oal_uint32 ul_event_length = pst_event_hdr->us_length - FRW_EVENT_HDR_LEN;
    oal_uint32 ul_loop;

    OAL_IO_PRINT("==================event==================\n");
    OAL_IO_PRINT("type     : [%02X]\n", pst_event_hdr->en_type);
    OAL_IO_PRINT("sub type : [%02X]\n", pst_event_hdr->uc_sub_type);
    OAL_IO_PRINT("length   : [%X]\n", pst_event_hdr->us_length);
    OAL_IO_PRINT("pipeline : [%02X]\n", pst_event_hdr->en_pipeline);
    OAL_IO_PRINT("chip id  : [%02X]\n", pst_event_hdr->uc_chip_id);
    OAL_IO_PRINT("device id: [%02X]\n", pst_event_hdr->uc_device_id);
    OAL_IO_PRINT("vap id   : [%02X]\n", pst_event_hdr->uc_vap_id);

    OAL_IO_PRINT("payload: \n");

    for (ul_loop = 0; ul_loop < ul_event_length; ul_loop += sizeof(oal_uint32)) { /* ÿ��ƫ��4�ֽ� */
        OAL_IO_PRINT("%02X %02X %02X %02X\n", puc_payload[ul_loop], puc_payload[ul_loop + 1],
                     puc_payload[ul_loop + 2], puc_payload[ul_loop + 3]);
    }
}

/*
 * �� �� ��  : frw_event_get_info_from_event_queue
 * ��������  : ���¼������л�ȡÿһ���¼����¼�ͷ��Ϣ
 * �������  : pst_event_queue: �¼�����
 */
OAL_STATIC oal_void frw_event_get_info_from_event_queue(frw_event_queue_stru *pst_event_queue)
{
    frw_event_stru *pst_event = NULL;
    oal_queue_stru *pst_queue = NULL;
    frw_event_mem_stru *pst_event_mem = NULL;
    oal_uint8 uc_loop;

    pst_queue = &pst_event_queue->st_queue;

    for (uc_loop = 0; uc_loop < pst_queue->uc_element_cnt; uc_loop++) {
        pst_event_mem = (frw_event_mem_stru *)pst_queue->pul_buf[uc_loop];
        pst_event = frw_get_event_stru(pst_event_mem);

#ifdef _PRE_DEBUG_MODE
        OAL_IO_PRINT("frw event info:vap %d user_cnt: %u, pool_id: %u, subpool_id: %u, len: %u, "
                     "alloc_core_id = %u, alloc_file_id: %u, alloc_line_num: %u, alloc_time_stamp: %u, "
                     "trace_file_id: %u, trace_line_num: %u, trace_time_stamp: %u.\n",
                     pst_event->st_event_hdr.uc_vap_id,
                     pst_event_mem->uc_user_cnt,
                     pst_event_mem->en_pool_id,
                     pst_event_mem->uc_subpool_id,
                     pst_event_mem->us_len,
                     pst_event_mem->ul_alloc_core_id[0],
                     pst_event_mem->ul_alloc_file_id[0],
                     pst_event_mem->ul_alloc_line_num[0],
                     pst_event_mem->ul_alloc_time_stamp[0],
                     pst_event_mem->ul_trace_file_id,
                     pst_event_mem->ul_trace_line_num,
                     pst_event_mem->ul_trace_time_stamp);
#else
        OAL_IO_PRINT("frw event info:vap %d,type = %d,subtype=%d,pipe=%d, user_cnt: %u, pool_id: %u, subpool_id: %u, len: %u.\n",
                     pst_event->st_event_hdr.uc_vap_id,
                     pst_event->st_event_hdr.en_type,
                     pst_event->st_event_hdr.uc_sub_type,
                     pst_event->st_event_hdr.en_pipeline,
                     pst_event_mem->uc_user_cnt,
                     pst_event_mem->en_pool_id,
                     pst_event_mem->uc_subpool_id,
                     pst_event_mem->us_len);
#endif
    }
}

/*
 * �� �� ��  : frw_event_queue_info
 * ��������  : ���¼������е��¼������Լ�ÿ���¼����¼�ͷ��Ϣ�ϱ�.
 *             �ӵ��ȶ����ҵ�ÿһ�������¼����¼����У�Ȼ���ȡ�¼��������õ�ÿһ��
 *             �¼����¼�ͷ��Ϣ
 * �� �� ֵ  : �ɹ�����ʧ����
 */
oal_uint32 frw_event_queue_info(oal_void)
{
    oal_uint32 ul_core_id;
    oal_uint16 us_qid;
    frw_event_sched_queue_stru *pst_sched_queue = NULL;
    frw_event_queue_stru *pst_event_queue = NULL;
    frw_event_mgmt_stru *pst_event_mgmt = NULL;
    oal_dlist_head_stru *pst_dlist = NULL;
    oal_uint ul_irq_flag;

    /* ��ȡ�˺� */
    ul_core_id = OAL_GET_CORE_ID();
    OAL_IO_PRINT("frw_event_queue_info get core id is %d.\n", ul_core_id);

    for (ul_core_id = 0; ul_core_id < WLAN_FRW_MAX_NUM_CORES; ul_core_id++) {
        OAL_IO_PRINT("-------------frw_event_queue_info core id is %d--------------.\n", ul_core_id);
        for (us_qid = 0; us_qid < FRW_EVENT_MAX_NUM_QUEUES; us_qid++) {
            pst_event_queue = &event_manager[ul_core_id].st_event_queue[us_qid];
            oal_spin_lock_irq_save(&pst_event_queue->st_lock, &ul_irq_flag);

            if (pst_event_queue->st_queue.uc_element_cnt != 0) {
                OAL_IO_PRINT("qid %d,state %d, event num %d, max event num %d, weigt_cnt %d,head idx %d,tail idx %d,prev=0x%p,next=0x%p\n",
                             us_qid, pst_event_queue->en_state,
                             pst_event_queue->st_queue.uc_element_cnt,
                             pst_event_queue->st_queue.uc_max_elements,
                             pst_event_queue->uc_weight,
                             pst_event_queue->st_queue.uc_head_index,
                             pst_event_queue->st_queue.uc_tail_index,
                             pst_event_queue->st_list.pst_prev,
                             pst_event_queue->st_list.pst_next);
                frw_event_get_info_from_event_queue(pst_event_queue);
            }
            oal_spin_unlock_irq_restore(&pst_event_queue->st_lock, &ul_irq_flag);
        }
        /* ���ݺ˺ţ��ҵ���Ӧ���¼�����ṹ�� */
        pst_event_mgmt = &event_manager[ul_core_id];

        /* ������ȡ���ȶ��� */
        for (us_qid = 0; us_qid < FRW_SCHED_POLICY_BUTT; us_qid++) {
            /* ��ȡ�¼�����ṹ���еĵ��ȶ��� */
            pst_sched_queue = &pst_event_mgmt->st_sched_queue[us_qid];
            oal_spin_lock_irq_save(&pst_sched_queue->st_lock, &ul_irq_flag);
            /* ��ȡ���ȶ�����ÿ���¼����е�ÿ���¼�����Ϣ */
            if (!oal_dlist_is_empty(&pst_sched_queue->st_head)) {
                /* ��ȡ���ȶ����е�ÿһ���¼����� */
                OAL_DLIST_SEARCH_FOR_EACH(pst_dlist, &pst_sched_queue->st_head)
                {
                    pst_event_queue = OAL_DLIST_GET_ENTRY(pst_dlist, frw_event_queue_stru, st_list);

                    /* ��ȡ������ÿһ���¼����¼�ͷ��Ϣ */
                    oal_spin_lock(&pst_event_queue->st_lock);
                    frw_event_get_info_from_event_queue(pst_event_queue);
                    oal_spin_unlock(&pst_event_queue->st_lock);
                }
            } else {
                OAL_IO_PRINT("Schedule queue %d empty\n", us_qid);
            }
            oal_spin_unlock_irq_restore(&pst_sched_queue->st_lock, &ul_irq_flag);
        }
    }

    return OAL_SUCC;
}

/*
 * �� �� ��  : frw_event_vap_pause_event
 * ��������  : �����ض�VAP�������¼����е�VAP״̬Ϊ��ͣ��ֹͣ���ȣ�����������
 * �������  : uc_vap_id: VAP IDֵ
 * �� �� ֵ  : OAL_SUCC����OAL_FAIL
 */
oal_void frw_event_vap_pause_event(oal_uint8 uc_vap_id)
{
    oal_uint32 ul_core_id;
    oal_uint16 us_qid;
    frw_event_mgmt_stru *pst_event_mgmt = NULL;
    frw_event_queue_stru *pst_event_queue = NULL;
    frw_event_sched_queue_stru *pst_sched_queue = NULL;

    ul_core_id = OAL_GET_CORE_ID();
    if (OAL_UNLIKELY(ul_core_id >= WLAN_FRW_MAX_NUM_CORES)) {
        OAM_ERROR_LOG1(0, OAM_SF_FRW, "{frw_event_process_all_event, array overflow!%d}", ul_core_id);
        return;
    }

    /* ���ݺ˺ţ��ҵ���Ӧ���¼����� */
    pst_event_mgmt = &event_manager[ul_core_id];

    /* ���ݶ���ID���ҵ���Ӧ��VAP�ĵ�һ���¼����� */
    pst_event_queue = &pst_event_mgmt->st_event_queue[uc_vap_id * FRW_EVENT_TYPE_BUTT];

    /* ����¼������Ѿ���pause�Ļ���ֱ�ӷ��أ���Ȼѭ���е��ȶ�����Ȩ�ػ��ظ���ȥ�¼����е�Ȩ�� */
    if (pst_event_queue->en_vap_state == FRW_VAP_STATE_PAUSE) {
        return;
    }

    for (us_qid = 0; us_qid < FRW_EVENT_TYPE_BUTT; us_qid++) {
        /* ���ݶ���ID���ҵ���Ӧ���¼����� */
        pst_event_queue = &pst_event_mgmt->st_event_queue[uc_vap_id * FRW_EVENT_TYPE_BUTT + us_qid];
        pst_sched_queue = &event_manager[ul_core_id].st_sched_queue[pst_event_queue->en_policy];

        frw_event_sched_pause_queue(pst_sched_queue, pst_event_queue);
    }
}

/*
 * �� �� ��  : frw_event_vap_resume_event
 * ��������  : �����ض�VAP�������¼����е�VAP״̬Ϊ�ָ����ɵ���
 * �������  : uc_vap_id: VAP IDֵ
 */
oal_void frw_event_vap_resume_event(oal_uint8 uc_vap_id)
{
    oal_uint32 ul_core_id;
    oal_uint16 us_qid;
    frw_event_mgmt_stru *pst_event_mgmt = NULL;
    frw_event_queue_stru *pst_event_queue = NULL;
    frw_event_sched_queue_stru *pst_sched_queue = NULL;

    ul_core_id = OAL_GET_CORE_ID();
    if (OAL_UNLIKELY(ul_core_id >= WLAN_FRW_MAX_NUM_CORES)) {
        OAM_ERROR_LOG1(0, OAM_SF_FRW, "{frw_event_process_all_event, array overflow!%d}", ul_core_id);
        return;
    }

    /* ���ݺ˺ţ��ҵ���Ӧ���¼����� */
    pst_event_mgmt = &event_manager[ul_core_id];

    /* ���ݶ���ID���ҵ���Ӧ��VAP�ĵ�һ���¼����� */
    pst_event_queue = &pst_event_mgmt->st_event_queue[uc_vap_id * FRW_EVENT_TYPE_BUTT];

    /* ����¼������Ѿ���resume�Ļ���ֱ�ӷ��أ���Ȼѭ���е��ȶ�����Ȩ�ػ��ظ���ȥ�¼����е�Ȩ�� */
    if (pst_event_queue->en_vap_state == FRW_VAP_STATE_RESUME) {
        return;
    }

    for (us_qid = 0; us_qid < FRW_EVENT_TYPE_BUTT; us_qid++) {
        /* ���ݶ���ID���ҵ���Ӧ���¼����� */
        pst_event_queue = &pst_event_mgmt->st_event_queue[uc_vap_id * FRW_EVENT_TYPE_BUTT + us_qid];
        pst_sched_queue = &event_manager[ul_core_id].st_sched_queue[pst_event_queue->en_policy];

        frw_event_sched_resume_queue(pst_sched_queue, pst_event_queue);
    }

    /* �����߳� */
    frw_task_sched(ul_core_id);
}

/*
 * �� �� ��  : frw_event_vap_flush_event
 * ��������  : ��ˢָ��VAP��ָ���¼����͵������¼���ͬʱ����ָ���Ƕ�����Щ�¼�����ȫ������
 * �������  : uc_vap_id:     VAP IDֵ
 *             en_event_type: �¼�����
 *             en_drop:       �¼�����(1)���ߴ���(0)
 * �� �� ֵ  : OAL_SUCC��OAL_FAIL������������
 */
oal_uint32 frw_event_vap_flush_event(oal_uint8 uc_vap_id,
                                     frw_event_type_enum_uint8 en_event_type,
                                     oal_bool_enum_uint8 en_drop)
{
    oal_uint32 ul_core_id;
    oal_uint16 us_qid;
    frw_event_mgmt_stru *pst_event_mgmt = NULL;
    frw_event_queue_stru *pst_event_queue = NULL;
    frw_event_mem_stru *pst_event_mem = NULL;
    frw_event_hdr_stru *pst_event_hrd = NULL;

    /* ��ȡ�˺� */
    ul_core_id = OAL_GET_CORE_ID();
    if (OAL_UNLIKELY(ul_core_id >= WLAN_FRW_MAX_NUM_CORES)) {
        OAM_ERROR_LOG1(0, OAM_SF_FRW, "{frw_event_vap_flush_event, array overflow!%d}", ul_core_id);
        return OAL_ERR_CODE_ARRAY_OVERFLOW;
    }

    if (en_event_type == FRW_EVENT_TYPE_WLAN_TX_COMP) {
        uc_vap_id = 0;
    }

    us_qid = uc_vap_id * FRW_EVENT_TYPE_BUTT + en_event_type;

    /* ���ݺ˺� + ����ID���ҵ���Ӧ���¼����� */
    pst_event_mgmt = &event_manager[ul_core_id];
    pst_event_queue = &pst_event_mgmt->st_event_queue[us_qid];

    /* ����¼����б���Ϊ�գ�û���¼������ڵ��ȶ��У����ش��� */
    if (pst_event_queue->st_queue.uc_element_cnt == 0) {
        return OAL_FAIL;
    }

    /* flush���е�event */
    while (pst_event_queue->st_queue.uc_element_cnt != 0) {
        pst_event_mem = (frw_event_mem_stru *)frw_event_queue_dequeue(pst_event_queue);
        if (pst_event_mem == OAL_PTR_NULL) {
            return OAL_ERR_CODE_PTR_NULL;
        }

        /* �����¼�������ֱ���ͷ��¼��ڴ�������¼� */
        if (en_drop == 0) {
            /* ��ȡ�¼�ͷ�ṹ */
            pst_event_hrd = (frw_event_hdr_stru *)frw_get_event_data(pst_event_mem);

            /* �����¼��ҵ���Ӧ���¼������� */
            frw_event_lookup_process_entry(pst_event_mem, pst_event_hrd);
        }

        /* �ͷ��¼��ڴ� */
        FRW_EVENT_FREE(pst_event_mem);
    }

    /* ���¼������Ѿ���գ���Ҫ����ӵ��ȶ�����ɾ���������¼�����״̬��Ϊ����Ծ(���ɱ�����) */
    if (pst_event_queue->st_queue.uc_element_cnt == 0) {
        frw_event_sched_deactivate_queue(&event_manager[ul_core_id].st_sched_queue[pst_event_queue->en_policy],
                                         pst_event_queue);
    } else {
        OAM_ERROR_LOG1(uc_vap_id, OAM_SF_FRW, "{flush vap event failed, left!=0: type=%d}", en_event_type);
    }

    return OAL_SUCC;
}

/*
 * �� �� ��  : frw_event_get_sched_queue
 * ��������  : ����VAP��ͣ���ָ��ͳ�ˢ��������IT���ԣ���ȡfrw_event_main.c�ж����ȫ���¼��ĵ��ȶ���
 * �������  : ul_core_id: �˺�
 *             en_policy:  ���Ȳ���
 * �� �� ֵ  : ���ȶ���ָ��
 */
frw_event_sched_queue_stru *frw_event_get_sched_queue(oal_uint32 ul_core_id, frw_sched_policy_enum_uint8 en_policy)
{
    if (OAL_UNLIKELY((ul_core_id >= WLAN_FRW_MAX_NUM_CORES) || (en_policy >= FRW_SCHED_POLICY_BUTT))) {
        return OAL_PTR_NULL;
    }

    return &(event_manager[ul_core_id].st_sched_queue[en_policy]);
}

/*
 * �� �� ��  : frw_is_event_queue_empty
 * ��������  : �ж�����VAP��Ӧ���¼������Ƿ�Ϊ��
 * �������  : uc_event_type: �¼�����
 * �� �� ֵ  : OAL_TRUE:  ��
 */
oal_bool_enum_uint8 frw_is_event_queue_empty(frw_event_type_enum_uint8 uc_event_type)
{
    oal_uint32 ul_core_id;
    oal_uint8 uc_vap_id;
    oal_uint16 us_qid;
    frw_event_mgmt_stru *pst_event_mgmt = NULL;
    frw_event_queue_stru *pst_event_queue = NULL;

    /* ��ȡ�˺� */
    ul_core_id = OAL_GET_CORE_ID();
    if (OAL_UNLIKELY(ul_core_id >= WLAN_FRW_MAX_NUM_CORES)) {
        OAM_ERROR_LOG1(0, OAM_SF_FRW, "{frw_event_post_event, core id = %d overflow!}", ul_core_id);

        return OAL_ERR_CODE_ARRAY_OVERFLOW;
    }

    pst_event_mgmt = &event_manager[ul_core_id];

    /* �����ú���ÿ��VAP��Ӧ���¼����У� */
    for (uc_vap_id = 0; uc_vap_id < WLAN_VAP_SUPPORT_MAX_NUM_LIMIT; uc_vap_id++) {
        us_qid = uc_vap_id * FRW_EVENT_TYPE_BUTT + uc_event_type;

        /* ���ݺ˺� + ����ID���ҵ���Ӧ���¼����� */
        pst_event_queue = &pst_event_mgmt->st_event_queue[us_qid];

        if (pst_event_queue->st_queue.uc_element_cnt != 0) {
            return OAL_FALSE;
        }
    }

    return OAL_TRUE;
}

/*
 * �� �� ��  : frw_is_vap_event_queue_empty
 * ��������  : ���ݺ�id���¼����ͣ��ж�vap�¼������Ƿ��
 * �������  : ul_core_id: ��id; event_type:  �¼�ID;
 * �� �� ֵ  : OAL_TRUE ������������
 */
oal_bool_enum_uint8 frw_is_vap_event_queue_empty(oal_uint32 ul_core_id, oal_uint8 uc_vap_id, oal_uint8 event_type)
{
    frw_event_mgmt_stru *pst_event_mgmt;
    frw_event_queue_stru *pst_event_queue;
    oal_uint16 us_qid;

#if (_PRE_OS_VERSION_RAW == _PRE_OS_VERSION) || (_PRE_OS_VERSION_WIN32_RAW == _PRE_OS_VERSION)
    us_qid = (oal_uint16)event_type;
#else
    us_qid = (oal_uint16)(uc_vap_id * FRW_EVENT_TYPE_BUTT + event_type);
#endif

    /* ���ݺ˺� + ����ID���ҵ���Ӧ���¼����� */
    pst_event_mgmt = &event_manager[ul_core_id];

    pst_event_queue = &pst_event_mgmt->st_event_queue[us_qid];

    if (pst_event_queue->st_queue.uc_element_cnt != 0) {
        return OAL_FALSE;
    }

    return OAL_TRUE;
}

/*
 * �� �� ��  : frw_task_thread_condition_check
 * ��������  : �ж��Ƿ����¼���Ҫ����
 * �������  : ��id
 * �� �� ֵ  : OAL_TRUE
 */
oal_uint8 frw_task_thread_condition_check(oal_uint32 ul_core_id)
{
    /* ����OAL_TRUE
     * 1.���ȶ��зǿ�
     *  2.���ȶ������з�pause�Ķ���
     */
    oal_uint8 sched_policy;
    oal_uint ul_irq_flag = 0;
    oal_dlist_head_stru *pst_list = NULL;
    frw_event_sched_queue_stru *pst_sched_queue = NULL;
    frw_event_queue_stru *pst_event_queue = NULL;

    pst_sched_queue = event_manager[ul_core_id].st_sched_queue;

    for (sched_policy = 0; sched_policy < FRW_SCHED_POLICY_BUTT; sched_policy++) {
        oal_spin_lock_irq_save(&pst_sched_queue[sched_policy].st_lock, &ul_irq_flag);
        /* ���������������� */
        OAL_DLIST_SEARCH_FOR_EACH(pst_list, &pst_sched_queue[sched_policy].st_head)
        {
            pst_event_queue = OAL_DLIST_GET_ENTRY(pst_list, frw_event_queue_stru, st_list);
            if (pst_event_queue->st_queue.uc_element_cnt == 0) {
                continue;
            }

            /* ����¼����е�vap_stateΪ��ͣ����������������ѡ��һ���¼����� */
            if (pst_event_queue->en_vap_state == FRW_VAP_STATE_PAUSE) {
                continue;
            }
            /* �ҵ��¼����зǿ� */
            oal_spin_unlock_irq_restore(&pst_sched_queue[sched_policy].st_lock, &ul_irq_flag);
            return OAL_TRUE;
        }
        oal_spin_unlock_irq_restore(&pst_sched_queue[sched_policy].st_lock, &ul_irq_flag);
    }
    /* �շ���OAL_FALSE */
    return OAL_FALSE;
}

/*lint -e578*/ /*lint -e19*/
oal_module_symbol(frw_event_alloc);
oal_module_symbol(frw_event_free);
oal_module_symbol(frw_event_dispatch_event);
oal_module_symbol(frw_event_post_event);
oal_module_symbol(frw_event_table_register);
oal_module_symbol(frw_event_dump_event);
oal_module_symbol(frw_event_process_all_event);
oal_module_symbol(frw_event_flush_event_queue);
oal_module_symbol(frw_event_queue_info);
oal_module_symbol(frw_event_get_info_from_event_queue);
oal_module_symbol(frw_event_vap_pause_event);
oal_module_symbol(frw_event_vap_resume_event);
oal_module_symbol(frw_event_vap_flush_event);
oal_module_symbol(frw_event_get_sched_queue);

oal_module_symbol(frw_is_event_queue_empty);
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC == _PRE_MULTI_CORE_MODE)
oal_module_symbol(frw_event_sub_rx_adapt_table_init);
#endif
#if defined(_PRE_DEBUG_MODE) && defined(_PRE_EVENT_TIME_TRACK_DEBUG)
oal_module_symbol(event_time_track);
oal_module_symbol(rx_event_idx);
#endif
#ifdef _PRE_DEBUG_MODE
oal_module_symbol(en_event_track_switch);
oal_module_symbol(schedule_idx);
oal_module_symbol(aul_schedule_time);
oal_module_symbol(mac_process_event);
#endif
