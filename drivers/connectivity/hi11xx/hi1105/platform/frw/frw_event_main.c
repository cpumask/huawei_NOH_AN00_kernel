

/* ͷ�ļ����� */
#include "oam_ext_if.h"
#include "frw_task.h"
#include "frw_main.h"
#include "frw_event_sched.h"
#include "frw_event_main.h"

#include "oal_kernel_file.h"
#include "securec.h"
#include "frw_timer.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_FRW_EVENT_MAIN_C

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
OAL_STATIC frw_event_cfg_stru g_event_queue_cfg_table[] = WLAN_FRW_EVENT_CFG_TABLE;

/* �¼�����ʵ�� */
frw_event_mgmt_stru g_event_manager[WLAN_FRW_MAX_NUM_CORES];

/* �¼���ȫ�ֱ��� */
frw_event_table_item_stru g_event_table[FRW_EVENT_TABLE_MAX_ITEMS];

/* IPCע�����ʵ�� */
OAL_STATIC frw_event_ipc_register_stru g_ipc_register;

#ifdef _PRE_OAL_FEATURE_TASK_NEST_LOCK
/* smp os use the task lock to protect the event process */
oal_task_lock_stru g_event_task_lock;
oal_module_symbol(g_event_task_lock);
#endif

#ifdef _PRE_CONFIG_HISI_PANIC_DUMP_SUPPORT
#ifdef _PRE_FRW_EVENT_PROCESS_TRACE_DEBUG
OAL_STATIC oal_int32 frw_trace_print_event_item(frw_event_trace_item_stru *pst_event_trace,
                                                char *buf, oal_int32 buf_len)
{
    oal_int32 ret;
    oal_ulong rem_nsec;
    oal_uint64 timestamp = pst_event_trace->timestamp;

    rem_nsec = do_div(timestamp, 1000000000);   /* 1000000000:����ת��Ϊ���� */
    ret = snprintf_s(buf, buf_len, buf_len - 1, "%u,%u,%u,%u,%5lu.%06lu\n",
                     pst_event_trace->st_event_seg.uc_vap_id,
                     pst_event_trace->st_event_seg.en_pipeline,
                     pst_event_trace->st_event_seg.en_type,
                     pst_event_trace->st_event_seg.uc_sub_type,
                     (oal_ulong)timestamp,
                     rem_nsec / 1000);      /* 1000:������ת��Ϊ΢�� */
    if (ret < 0) {
        oal_io_print("log str format err line[%d]\n", __LINE__);
    }
    return ret;
}
#endif

OAL_STATIC oal_int32 frw_print_panic_stat(oal_void *data, char *buf, oal_int32 buf_len)
{
    oal_int32 ret;
    oal_int32 count = 0;
    oal_uint32 ul_core_id;
    oal_uint32 i;

    oal_reference(data);
#ifdef _PRE_OAL_FEATURE_TASK_NEST_LOCK
    if (g_event_task_lock.claimer) {
        ret = snprintf_s(buf + count, buf_len - count, buf_len - count - 1, "frw task lock claimer:%s\n",
                         g_event_task_lock.claimer->comm);
        if (ret < 0) {
            oal_io_print("log str format err line[%d]\n", __LINE__);
            return count;
        }
        count += ret;
    }
#endif
#ifdef _PRE_FRW_EVENT_PROCESS_TRACE_DEBUG
    for (ul_core_id = 0; ul_core_id < WLAN_FRW_MAX_NUM_CORES; ul_core_id++) {
        ret = snprintf_s(buf + count, buf_len - count, buf_len - count - 1, "last pc:%s,line:%d\n",
                         g_event_manager[ul_core_id].pst_frw_trace->pst_func_name,
                         g_event_manager[ul_core_id].pst_frw_trace->line_num);
        if (ret < 0) {
            oal_io_print("log str format err line[%d]\n", __LINE__);
            return count;
        }
        count += ret;

#if (_PRE_FRW_FEATURE_PROCCESS_ENTITY_TYPE == _PRE_FRW_FEATURE_PROCCESS_ENTITY_THREAD)
        ret = snprintf_s(buf + count, buf_len - count, buf_len - count - 1,
                         "task thread total cnt:%u,event cnt:%u,empty max count:%u\n",
                         g_st_event_task[ul_core_id].ul_total_loop_cnt,
                         g_st_event_task[ul_core_id].ul_total_event_cnt,
                         g_st_event_task[ul_core_id].ul_max_empty_count);
        if (ret < 0) {
            oal_io_print("log str format err line[%d]\n", __LINE__);
            return count;
        }
        count += ret;
#endif

        ret = snprintf_s(buf + count, buf_len - count, buf_len - count - 1, "frw event trace buff:\n");
        if (ret < 0) {
            oal_io_print("log str format err line[%d]\n", __LINE__);
            return count;
        }
        count += ret;

#if defined(_PRE_FRW_TIMER_BIND_CPU) && defined(CONFIG_NR_CPUS)
        do {
            oal_uint32 cpu_id;
            for (cpu_id = 0; cpu_id < CONFIG_NR_CPUS; cpu_id++) {
                if (g_frw_timer_cpu_count[cpu_id]) {
                    ret = snprintf_s(buf + count, buf_len - count, buf_len - count - 1, "[cpu:%u]count:%u\n",
                                     cpu_id, g_frw_timer_cpu_count[cpu_id]);
                    if (ret < 0) {
                        oal_io_print("log str format err line[%d]\n", __LINE__);
                        return count;
                    }
                    count += ret;
                }
            }
        } while (0);
#endif

        if (g_event_manager[ul_core_id].pst_frw_trace->ul_over_flag == 1) {
            /* overturn */
            for (i = g_event_manager[ul_core_id].pst_frw_trace->ul_current_pos;
                 i < CONFIG_FRW_MAX_TRACE_EVENT_NUMS; i++) {
                ret = frw_trace_print_event_item(&g_event_manager[ul_core_id].pst_frw_trace->st_trace_item[i],
                                                 buf + count, buf_len - count);
                if (ret < 0) {
                    oal_io_print("log str format err line[%d]\n", __LINE__);
                    return count;
                }
                count += ret;
            }
        }

        i = 0;
        for (i = 0; i < g_event_manager[ul_core_id].pst_frw_trace->ul_current_pos; i++) {
            ret = frw_trace_print_event_item(&g_event_manager[ul_core_id].pst_frw_trace->st_trace_item[i],
                                             buf + count, buf_len - count);
            if (ret < 0) {
                oal_io_print("log str format err line[%d]\n", __LINE__);
                return count;
            }
            count += ret;
        }
    }
#else
    oal_reference(i);
    oal_reference(ul_core_id);
    oal_reference(ret);
    oal_reference(count);
#endif
    return count;
}
OAL_STATIC declare_wifi_panic_stru(frw_panic_stat, frw_print_panic_stat);
#endif

OAL_STATIC oal_uint32 frw_event_init_event_queue(oal_void)
{
    oal_uint32 ul_core_id;
    oal_uint16 us_qid;
    oal_uint32 ul_ret;

    for (ul_core_id = 0; ul_core_id < WLAN_FRW_MAX_NUM_CORES; ul_core_id++) {
        /* ѭ����ʼ���¼����� */
        for (us_qid = 0; us_qid < FRW_EVENT_MAX_NUM_QUEUES; us_qid++) {
            ul_ret = frw_event_queue_init(&g_event_manager[ul_core_id].st_event_queue[us_qid],
                                          g_event_queue_cfg_table[us_qid].uc_weight,
                                          g_event_queue_cfg_table[us_qid].en_policy,
                                          FRW_EVENT_QUEUE_STATE_INACTIVE,
                                          g_event_queue_cfg_table[us_qid].uc_max_events);
            if (oal_unlikely(ul_ret != OAL_SUCC)) {
                oam_warning_log1(0, OAM_SF_FRW,
                                 "{frw_event_init_event_queue, frw_event_queue_init return != OAL_SUCC!%d}",
                                 ul_ret);
                return ul_ret;
            }
        }
    }

    return OAL_SUCC;
}

OAL_STATIC oal_void frw_event_destroy_event_queue(oal_uint32 ul_core_id)
{
    oal_uint16 us_qid;

    /* ѭ�������¼����� */
    for (us_qid = 0; us_qid < FRW_EVENT_MAX_NUM_QUEUES; us_qid++) {
        frw_event_queue_destroy(&g_event_manager[ul_core_id].st_event_queue[us_qid]);
    }
}

/*
 * �� �� ��  : frw_event_init_sched
 * ��������  : ��ʼ��������
 */
OAL_STATIC oal_uint32 frw_event_init_sched(oal_void)
{
    oal_uint32 ul_core_id;
    oal_uint16 us_qid;
    oal_uint32 ul_ret;

    for (ul_core_id = 0; ul_core_id < WLAN_FRW_MAX_NUM_CORES; ul_core_id++) {
        /* ѭ����ʼ�������� */
        for (us_qid = 0; us_qid < FRW_SCHED_POLICY_BUTT; us_qid++) {
            ul_ret = frw_event_sched_init(&g_event_manager[ul_core_id].st_sched_queue[us_qid]);
            if (oal_unlikely(ul_ret != OAL_SUCC)) {
                oam_warning_log1(0, OAM_SF_FRW,
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
        g_event_manager[ul_core_id].pst_frw_trace =
            (frw_event_trace_stru *)vmalloc(OAL_SIZEOF(frw_event_trace_stru));
        if (g_event_manager[ul_core_id].pst_frw_trace == NULL) {
            oal_io_print("frw_event_init_sched coreid:%u, alloc frw event trace %u bytes failed! \n",
                         ul_core_id,
                         (oal_uint32)OAL_SIZEOF(frw_event_trace_stru));
            return OAL_ERR_CODE_PTR_NULL;
        }
        memset_s((oal_void *)g_event_manager[ul_core_id].pst_frw_trace,
                 OAL_SIZEOF(frw_event_trace_stru), 0, OAL_SIZEOF(frw_event_trace_stru));
    }
    return OAL_SUCC;
}

OAL_STATIC oal_void frw_event_trace_exit(oal_void)
{
    oal_uint32 ul_core_id;
    for (ul_core_id = 0; ul_core_id < WLAN_FRW_MAX_NUM_CORES; ul_core_id++) {
        if (g_event_manager[ul_core_id].pst_frw_trace != NULL) {
            vfree(g_event_manager[ul_core_id].pst_frw_trace);
            g_event_manager[ul_core_id].pst_frw_trace = NULL;
        }
    }
}
#endif

/*
 * �� �� ��  : frw_event_dispatch_event
 * ��������  : �¼��ַ��ӿ�(�ַ��¼����˼�ͨѶ���¼����С����߲��Ѱ����Ӧ�¼�������)
 * �������  : pst_event_mem: ָ���¼��ڴ���ָ��
 */
oal_uint32 frw_event_dispatch_event(frw_event_mem_stru *pst_event_mem)
{
#if (_PRE_MULTI_CORE_MODE_PIPELINE_AMP == _PRE_MULTI_CORE_MODE)
    frw_event_deploy_enum_uint8 en_deploy;
    oal_uint32 ul_ret;
#endif
    if (oal_unlikely(pst_event_mem == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_FRW, "{frw_event_dispatch_event: pst_event_mem is null ptr!}");
        return OAL_ERR_CODE_PTR_NULL;
    }

#if (_PRE_MULTI_CORE_MODE_PIPELINE_AMP == _PRE_MULTI_CORE_MODE)
    /* ���û�п����˼�ͨ�ţ�������¼��ֶκŴ����¼�(��ӻ���ִ����Ӧ�Ĵ�����) */
    if (st_ipc_register.p_frw_event_deploy_pipeline_func == OAL_PTR_NULL) {
        return frw_event_process(pst_event_mem);
    }

    ul_ret = st_ipc_register.p_frw_event_deploy_pipeline_func(pst_event_mem, &en_deploy);
    if (ul_ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_FRW,
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
        oal_io_print("buf is null r failed!%s\n", __FUNCTION__);
        return 0;
    }

    if (attr == NULL) {
        oal_io_print("attr is null r failed!%s\n", __FUNCTION__);
        return 0;
    }

    if (dev == NULL) {
        oal_io_print("dev is null r failed!%s\n", __FUNCTION__);
        return 0;
    }

    ret += frw_print_panic_stat(NULL, buf, PAGE_SIZE - ret);
    return ret;
}
STATIC struct kobj_attribute g_dev_attr_event_trace =
    __ATTR(event_trace, S_IRUGO, frw_get_event_trace, NULL);

OAL_STATIC struct attribute *g_frw_sysfs_entries[] = {
    &g_dev_attr_event_trace.attr,
    NULL
};

OAL_STATIC struct attribute_group g_frw_attribute_group = {
    .name = "frw",
    .attrs = g_frw_sysfs_entries,
};

OAL_STATIC oal_int32 frw_sysfs_entry_init(oal_void)
{
    oal_int32 ret;
    oal_kobject *pst_root_object = NULL;
    pst_root_object = oal_get_sysfs_root_object();
    if (pst_root_object == NULL) {
        oam_error_log0(0, OAM_SF_ANY, "{frw_sysfs_entry_init::get sysfs root object failed!}");
        return -OAL_EFAIL;
    }

    ret = oal_debug_sysfs_create_group(pst_root_object, &g_frw_attribute_group);
    if (ret) {
        oam_error_log0(0, OAM_SF_ANY, "{frw_sysfs_entry_init::sysfs create group failed!}");
        return ret;
    }
    return OAL_SUCC;
}

OAL_STATIC oal_int32 frw_sysfs_entry_exit(oal_void)
{
    oal_kobject *pst_root_object = NULL;
    pst_root_object = oal_get_sysfs_root_object();
    if (pst_root_object != NULL) {
        oal_debug_sysfs_remove_group(pst_root_object, &g_frw_attribute_group);
    }

    return OAL_SUCC;
}
#endif

oal_uint32 frw_event_init(oal_void)
{
    oal_uint32 ul_ret;

    memset_s(&g_ipc_register, OAL_SIZEOF(g_ipc_register), 0, OAL_SIZEOF(g_ipc_register));

#ifdef _PRE_OAL_FEATURE_TASK_NEST_LOCK
    oal_smp_task_lock_init(&g_event_task_lock);
#endif

    /* ��ʼ���¼����� */
    ul_ret = frw_event_init_event_queue();
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_FRW, "{frw_event_init, frw_event_init_event_queue != OAL_SUCC!%d}", ul_ret);
        return ul_ret;
    }

    /* ��ʼ�������� */
    ul_ret = frw_event_init_sched();
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_FRW, "frw_event_init, frw_event_init_sched != OAL_SUCC!%d", ul_ret);
        return ul_ret;
    }

#ifdef _PRE_FRW_EVENT_PROCESS_TRACE_DEBUG
    ul_ret = frw_event_trace_init();
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_error_log1(0, OAM_SF_FRW, "frw_event_init, frw_event_trace_init != OAL_SUCC!%d", ul_ret);
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

/*
 * �� �� ��  : frw_event_queue_enqueue
 * ��������  : ���¼��ڴ������Ӧ���¼�����
 * �������  : pst_event_mem: ָ���¼��ڴ���ָ��
 */
oal_uint32 frw_event_queue_enqueue(frw_event_queue_stru *pst_event_queue, frw_event_mem_stru *pst_event_mem)
{
    oal_uint32 ul_ret;
    oal_uint ul_irq_flag;

    if (oal_unlikely((pst_event_queue == NULL) || (pst_event_mem == NULL))) {
        oal_warn_on(1);
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
 */
frw_event_mem_stru *frw_event_queue_dequeue(frw_event_queue_stru *pst_event_queue)
{
    frw_event_mem_stru *pst_event_mem = NULL;
    oal_uint ul_irq_flag;

    if (oal_unlikely(pst_event_queue == NULL)) {
        oal_warn_on(1);
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
 */
oal_uint32 frw_event_post_event(frw_event_mem_stru *pst_event_mem, oal_uint32 ul_core_id)
{
    oal_uint16 us_qid;
    frw_event_mgmt_stru *pst_event_mgmt = NULL;
    frw_event_queue_stru *pst_event_queue = NULL;
    oal_uint32 ul_ret;
    frw_event_hdr_stru *pst_event_hdr = NULL;
    frw_event_sched_queue_stru *pst_sched_queue = NULL;

    if (oal_unlikely(pst_event_mem == NULL)) {
        oal_warn_on(1);
        return OAL_FAIL;
    }

    /* ��ȡ�¼�����ID */
    ul_ret = frw_event_to_qid(pst_event_mem, &us_qid);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_warning_log1(0, OAM_SF_FRW, "{frw_event_post_event, frw_event_to_qid return != OAL_SUCC!%d}", ul_ret);
        return ul_ret;
    }

    if (oal_unlikely(ul_core_id >= WLAN_FRW_MAX_NUM_CORES)) {
        oam_error_log1(0, OAM_SF_FRW, "{frw_event_post_event, array overflow!%d}", ul_core_id);
        return OAL_ERR_CODE_ARRAY_OVERFLOW;
    }

    /* ���ݺ˺� + ����ID���ҵ���Ӧ���¼����� */
    pst_event_mgmt = &g_event_manager[ul_core_id];

    pst_event_queue = &pst_event_mgmt->st_event_queue[us_qid];

    /* ���policy */
    if (oal_unlikely(pst_event_queue->en_policy >= FRW_SCHED_POLICY_BUTT)) {
        oam_error_log1(0, OAM_SF_FRW, "{frw_event_post_event, array overflow!%d}", pst_event_queue->en_policy);
        return OAL_ERR_CODE_ARRAY_OVERFLOW;
    }

    /* ��ȡ���ȶ��� */
    pst_sched_queue = &pst_event_mgmt->st_sched_queue[pst_event_queue->en_policy];

    /* ��ȡ�����ã���ֹenqueue��ȡ������֮�䱻�ͷ� */
    pst_event_mem->uc_user_cnt++;

    /* �¼���� */
    ul_ret = frw_event_queue_enqueue(pst_event_queue, pst_event_mem);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        pst_event_hdr = (frw_event_hdr_stru *)(frw_get_event_data(pst_event_mem));
        oam_warning_log4(0, OAM_SF_FRW,
                         "frw_event_post_event:: enqueue fail. core %d, type %d, sub type %d, pipeline %d ",
                         ul_core_id,
                         pst_event_hdr->en_type,
                         pst_event_hdr->uc_sub_type,
                         pst_event_hdr->en_pipeline);

        oam_warning_log4(0, OAM_SF_FRW, "event info: type: %d, sub type: %d, pipeline: %d,max num:%d",
                         pst_event_hdr->en_type,
                         pst_event_hdr->uc_sub_type,
                         pst_event_hdr->en_pipeline,
                         pst_event_queue->st_queue.uc_max_elements);

        /* �ͷ��¼��ڴ����� */
        frw_event_free_m(pst_event_mem);

        return ul_ret;
    }

    /* �����������Ȳ��ԣ����¼����м���ɵ��ȶ��� */
    ul_ret = frw_event_sched_activate_queue(pst_sched_queue, pst_event_queue);
    if (oal_unlikely(ul_ret != OAL_SUCC)) {
        oam_error_log1(0, OAM_SF_FRW,
                       "{frw_event_post_event, frw_event_sched_activate_queue return != OAL_SUCC! %d}",
                       ul_ret);
        return ul_ret;
    }

    frw_task_sched(ul_core_id);

    return OAL_SUCC;
}

/*
 * �� �� ��  : frw_event_table_register
 * ��������  : ע����Ӧ�¼���Ӧ���¼�������
 * �������  : en_type:       �¼�����
             en_pipeline:   �¼��ֶκ�
             pst_sub_table: �¼��ӱ�ָ��
 */
oal_void frw_event_table_register(frw_event_type_enum_uint8 en_type,
                                  frw_event_pipeline_enum en_pipeline,
                                  frw_event_sub_table_item_stru *pst_sub_table)
{
    oal_uint8 uc_index;

    if (oal_unlikely(pst_sub_table == OAL_PTR_NULL)) {
        oam_error_log0(0, OAM_SF_FRW, "{frw_event_table_register: pst_sub_table is null ptr!}");
        return;
    }

    /* �����¼����ͼ��ֶκż����¼������� */
    uc_index = (((oal_uint8)en_type << 1) | ((oal_uint8)en_pipeline & 0x01));

    if (oal_unlikely(uc_index >= FRW_EVENT_TABLE_MAX_ITEMS)) {
        oam_error_log1(0, OAM_SF_FRW, "{frw_event_table_register, array overflow! %d}", uc_index);
        return;
    }

    g_event_table[uc_index].pst_sub_table = pst_sub_table;
}

/*
 * �� �� ��  : frw_event_deploy_register
 * ��������  : ��event deployģ��ע���¼�����ӿ�
 * �������  : p_func: �¼�����ӿ�
 */
oal_void frw_event_deploy_register(oal_uint32 (*p_func)(frw_event_mem_stru *pst_event_mem,
                                                        frw_event_deploy_enum_uint8 *pen_deploy_result))
{
    if (oal_unlikely(p_func == NULL)) {
        oal_warn_on(1);
        return;
    }
    g_ipc_register.p_frw_event_deploy_pipeline_func = p_func;
}

/*
 * �� �� ��  : frw_event_ipc_event_queue_full_register
 * ��������  : ��IPCģ��ע��˼��ж�Ƶ�ȹ���ӿ�
 * �������  : p_func: �˼��ж�Ƶ�ȹ���ӿ�
 */
oal_void frw_event_ipc_event_queue_full_register(oal_uint32 (*p_func)(oal_void))
{
    if (oal_unlikely(p_func == NULL)) {
        oal_warn_on(1);
        return;
    }
    g_ipc_register.p_frw_ipc_event_queue_full_func = p_func;
}

/*
 * �� �� ��  : frw_event_ipc_event_queue_empty_register
 * ��������  : ��IPCģ��ע��˼��ж�Ƶ�ȹ���ӿ�
 * �������  : p_func: �˼��ж�Ƶ�ȹ���ӿ�
 */
oal_void frw_event_ipc_event_queue_empty_register(oal_uint32 (*p_func)(oal_void))
{
    if (oal_unlikely(p_func == NULL)) {
        oal_warn_on(1);
        return;
    }
    g_ipc_register.p_frw_ipc_event_queue_empty_func = p_func;
}

oal_void frw_event_process_all_event(oal_uint ui_data)
{
    oal_uint32 ul_core_id;
    frw_event_mem_stru *pst_event_mem = NULL;
    frw_event_sched_queue_stru *pst_sched_queue = NULL;
    frw_event_hdr_stru *pst_event_hrd = NULL;
    oal_uint32 ul_mac_process_event = FRW_PROCESS_MAX_EVENT;

    /* ��ȡ�˺� */
    ul_core_id = oal_get_core_id();
    if (oal_unlikely(ul_core_id >= WLAN_FRW_MAX_NUM_CORES)) {
        oam_error_log1(0, OAM_SF_FRW, "{frw_event_process_all_event, array overflow! %d}", ul_core_id);
        return;
    }

    pst_sched_queue = g_event_manager[ul_core_id].st_sched_queue;

    /* �����¼�����ģ�飬ѡ��һ���¼� */
    pst_event_mem = (frw_event_mem_stru *)frw_event_schedule(pst_sched_queue);
    while (pst_event_mem != OAL_PTR_NULL) {
        /* ��ȡ�¼�ͷ�ṹ */
        pst_event_hrd = (frw_event_hdr_stru *)frw_get_event_data(pst_event_mem);

#ifdef _PRE_FRW_EVENT_PROCESS_TRACE_DEBUG
        /* trace the event serial */
        frw_event_trace(pst_event_mem, ul_core_id);
#endif
        /* �����¼��ҵ���Ӧ���¼������� */
        frw_event_task_lock();
        frw_event_lookup_process_entry(pst_event_mem, pst_event_hrd);
        frw_event_task_unlock();

        /* �ͷ��¼��ڴ� */
        frw_event_free_m(pst_event_mem);
#if (_PRE_FRW_FEATURE_PROCCESS_ENTITY_TYPE == _PRE_FRW_FEATURE_PROCCESS_ENTITY_THREAD)
        if (oal_likely(ul_core_id < WLAN_FRW_MAX_NUM_CORES)) {
            g_st_event_task[ul_core_id].ul_total_event_cnt++;
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

    /* ����ÿ���˵�ÿ��vap��Ӧ���¼����� */
    for (ul_core_id = 0; ul_core_id < WLAN_FRW_MAX_NUM_CORES; ul_core_id++) {
        for (uc_vap_id = 0; uc_vap_id < WLAN_VAP_SUPPORT_MAX_NUM_LIMIT; uc_vap_id++) {
            us_qid = uc_vap_id * FRW_EVENT_TYPE_BUTT + uc_event_type;

            /* ���ݺ˺� + ����ID���ҵ���Ӧ���¼����� */
            pst_event_mgmt = &g_event_manager[ul_core_id];
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
                frw_event_free_m(pst_event_mem);

                ul_event_succ++;
            }

            /* ����¼����б�գ���Ҫ����ӵ��ȶ�����ɾ���������¼�����״̬��Ϊ����Ծ(���ɱ�����) */
            if (pst_event_queue->st_queue.uc_element_cnt == 0) {
                frw_event_sched_deactivate_queue(
                    &g_event_manager[ul_core_id].st_sched_queue[pst_event_queue->en_policy], pst_event_queue);
            }
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

    oal_io_print("==================event==================\n");
    oal_io_print("type     : [%02X]\n", pst_event_hdr->en_type);
    oal_io_print("sub type : [%02X]\n", pst_event_hdr->uc_sub_type);
    oal_io_print("length   : [%X]\n", pst_event_hdr->us_length);
    oal_io_print("pipeline : [%02X]\n", pst_event_hdr->en_pipeline);
    oal_io_print("chip id  : [%02X]\n", pst_event_hdr->uc_chip_id);
    oal_io_print("device id: [%02X]\n", pst_event_hdr->uc_device_id);
    oal_io_print("vap id   : [%02X]\n", pst_event_hdr->uc_vap_id);

    oal_io_print("payload: \n");

    for (ul_loop = 0; ul_loop < ul_event_length; ul_loop += sizeof(oal_uint32)) {
        oal_io_print("%02X %02X %02X %02X\n", puc_payload[ul_loop], puc_payload[ul_loop + 1],
                     puc_payload[ul_loop + 2], puc_payload[ul_loop + 3]);   /* 1��2��3: ���ֽ�Ϊ��λ��ӡ�¼����� */
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

        oal_io_print("frw event info:vap %d,type = %d,subtype=%d,pipe=%d, user_cnt: %u, pool_id: %u, \
                     subpool_id: %u, len: %u.\n",
                     pst_event->st_event_hdr.uc_vap_id,
                     pst_event->st_event_hdr.en_type,
                     pst_event->st_event_hdr.uc_sub_type,
                     pst_event->st_event_hdr.en_pipeline,
                     pst_event_mem->uc_user_cnt,
                     pst_event_mem->en_pool_id,
                     pst_event_mem->uc_subpool_id,
                     pst_event_mem->us_len);
    }
}

/*
 * �� �� ��  : frw_event_queue_info
 * ��������  : ���¼������е��¼������Լ�ÿ���¼����¼�ͷ��Ϣ�ϱ�.
 *             �ӵ��ȶ����ҵ�ÿһ�������¼����¼����У�Ȼ���ȡ�¼��������õ�ÿһ��
 *             �¼����¼�ͷ��Ϣ
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
    ul_core_id = oal_get_core_id();
    oal_io_print("frw_event_queue_info get core id is %d.\n", ul_core_id);

    for (ul_core_id = 0; ul_core_id < WLAN_FRW_MAX_NUM_CORES; ul_core_id++) {
        oal_io_print("-------------frw_event_queue_info core id is %d--------------.\n", ul_core_id);
        for (us_qid = 0; us_qid < FRW_EVENT_MAX_NUM_QUEUES; us_qid++) {
            pst_event_queue = &g_event_manager[ul_core_id].st_event_queue[us_qid];
            oal_spin_lock_irq_save(&pst_event_queue->st_lock, &ul_irq_flag);

            if (pst_event_queue->st_queue.uc_element_cnt != 0) {
                oal_io_print("qid %d,state %d, event num %d, max event num %d, weigt_cnt %d,head idx %d,tail idx %d, \
                             prev=0x%p, next=0x%p\n",
                             us_qid, pst_event_queue->en_state, pst_event_queue->st_queue.uc_element_cnt,
                             pst_event_queue->st_queue.uc_max_elements,
                             pst_event_queue->uc_weight, pst_event_queue->st_queue.uc_head_index,
                             pst_event_queue->st_queue.uc_tail_index,
                             pst_event_queue->st_list.pst_prev, pst_event_queue->st_list.pst_next);
                frw_event_get_info_from_event_queue(pst_event_queue);
            }
            oal_spin_unlock_irq_restore(&pst_event_queue->st_lock, &ul_irq_flag);
        }
        /* ���ݺ˺ţ��ҵ���Ӧ���¼�����ṹ�� */
        pst_event_mgmt = &g_event_manager[ul_core_id];

        /* ������ȡ���ȶ��� */
        for (us_qid = 0; us_qid < FRW_SCHED_POLICY_BUTT; us_qid++) {
            /* ��ȡ�¼�����ṹ���еĵ��ȶ��� */
            pst_sched_queue = &pst_event_mgmt->st_sched_queue[us_qid];
            oal_spin_lock_irq_save(&pst_sched_queue->st_lock, &ul_irq_flag);
            /* ��ȡ���ȶ�����ÿ���¼����е�ÿ���¼�����Ϣ */
            if (!oal_dlist_is_empty(&pst_sched_queue->st_head)) {
                /* ��ȡ���ȶ����е�ÿһ���¼����� */
                oal_dlist_search_for_each(pst_dlist, &pst_sched_queue->st_head)
                {
                    pst_event_queue = oal_dlist_get_entry(pst_dlist, frw_event_queue_stru, st_list);

                    /* ��ȡ������ÿһ���¼����¼�ͷ��Ϣ */
                    oal_spin_lock(&pst_event_queue->st_lock);
                    frw_event_get_info_from_event_queue(pst_event_queue);
                    oal_spin_unlock(&pst_event_queue->st_lock);
                }
            } else {
                oal_io_print("Schedule queue %d empty\n", us_qid);
            }
            oal_spin_unlock_irq_restore(&pst_sched_queue->st_lock, &ul_irq_flag);
        }
    }

    return OAL_SUCC;
}

/*
 * �� �� ��  : frw_event_vap_pause_event
 * ��������  : �����ض�VAP�������¼����е�VAP״̬Ϊ��ͣ��ֹͣ���ȣ�����������
 */
oal_void frw_event_vap_pause_event(oal_uint8 uc_vap_id)
{
    oal_uint32 ul_core_id;
    oal_uint16 us_qid;
    frw_event_mgmt_stru *pst_event_mgmt = NULL;
    frw_event_queue_stru *pst_event_queue = NULL;
    frw_event_sched_queue_stru *pst_sched_queue = NULL;

    ul_core_id = oal_get_core_id();
    if (oal_unlikely(ul_core_id >= WLAN_FRW_MAX_NUM_CORES)) {
        oam_error_log1(0, OAM_SF_FRW, "{frw_event_process_all_event, array overflow!%d}", ul_core_id);
        return;
    }

    /* ���ݺ˺ţ��ҵ���Ӧ���¼����� */
    pst_event_mgmt = &g_event_manager[ul_core_id];

    /* ���ݶ���ID���ҵ���Ӧ��VAP�ĵ�һ���¼����� */
    pst_event_queue = &pst_event_mgmt->st_event_queue[uc_vap_id * FRW_EVENT_TYPE_BUTT];

    /* ����¼������Ѿ���pause�Ļ���ֱ�ӷ��أ���Ȼѭ���е��ȶ�����Ȩ�ػ��ظ���ȥ�¼����е�Ȩ�� */
    if (pst_event_queue->en_vap_state == FRW_VAP_STATE_PAUSE) {
        return;
    }

    for (us_qid = 0; us_qid < FRW_EVENT_TYPE_BUTT; us_qid++) {
        /* ���ݶ���ID���ҵ���Ӧ���¼����� */
        pst_event_queue = &pst_event_mgmt->st_event_queue[uc_vap_id * FRW_EVENT_TYPE_BUTT + us_qid];
        pst_sched_queue = &g_event_manager[ul_core_id].st_sched_queue[pst_event_queue->en_policy];

        frw_event_sched_pause_queue(pst_sched_queue, pst_event_queue);
    }
}

/*
 * �� �� ��  : frw_event_vap_resume_event
 * ��������  : �����ض�VAP�������¼����е�VAP״̬Ϊ�ָ����ɵ���
 */
oal_void frw_event_vap_resume_event(oal_uint8 uc_vap_id)
{
    oal_uint32 ul_core_id;
    oal_uint16 us_qid;
    frw_event_mgmt_stru *pst_event_mgmt = NULL;
    frw_event_queue_stru *pst_event_queue = NULL;
    frw_event_sched_queue_stru *pst_sched_queue = NULL;

    ul_core_id = oal_get_core_id();
    if (oal_unlikely(ul_core_id >= WLAN_FRW_MAX_NUM_CORES)) {
        oam_error_log1(0, OAM_SF_FRW, "{frw_event_process_all_event, array overflow!%d}", ul_core_id);
        return;
    }

    /* ���ݺ˺ţ��ҵ���Ӧ���¼����� */
    pst_event_mgmt = &g_event_manager[ul_core_id];

    /* ���ݶ���ID���ҵ���Ӧ��VAP�ĵ�һ���¼����� */
    pst_event_queue = &pst_event_mgmt->st_event_queue[uc_vap_id * FRW_EVENT_TYPE_BUTT];

    /* ����¼������Ѿ���resume�Ļ���ֱ�ӷ��أ���Ȼѭ���е��ȶ�����Ȩ�ػ��ظ���ȥ�¼����е�Ȩ�� */
    if (pst_event_queue->en_vap_state == FRW_VAP_STATE_RESUME) {
        return;
    }

    for (us_qid = 0; us_qid < FRW_EVENT_TYPE_BUTT; us_qid++) {
        /* ���ݶ���ID���ҵ���Ӧ���¼����� */
        pst_event_queue = &pst_event_mgmt->st_event_queue[uc_vap_id * FRW_EVENT_TYPE_BUTT + us_qid];
        pst_sched_queue = &g_event_manager[ul_core_id].st_sched_queue[pst_event_queue->en_policy];

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
    ul_core_id = oal_get_core_id();
    if (oal_unlikely(ul_core_id >= WLAN_FRW_MAX_NUM_CORES)) {
        oam_error_log1(0, OAM_SF_FRW, "{frw_event_vap_flush_event, array overflow!%d}", ul_core_id);
        return OAL_ERR_CODE_ARRAY_OVERFLOW;
    }

    if (en_event_type == FRW_EVENT_TYPE_WLAN_TX_COMP) {
        uc_vap_id = 0;
    }

    us_qid = uc_vap_id * FRW_EVENT_TYPE_BUTT + en_event_type;

    /* ���ݺ˺� + ����ID���ҵ���Ӧ���¼����� */
    pst_event_mgmt = &g_event_manager[ul_core_id];
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
        frw_event_free_m(pst_event_mem);
    }

    /* ���¼������Ѿ���գ���Ҫ����ӵ��ȶ�����ɾ���������¼�����״̬��Ϊ����Ծ(���ɱ�����) */
    if (pst_event_queue->st_queue.uc_element_cnt == 0) {
        frw_event_sched_deactivate_queue(
            &g_event_manager[ul_core_id].st_sched_queue[pst_event_queue->en_policy], pst_event_queue);
    } else {
        oam_error_log1(uc_vap_id, OAM_SF_FRW, "{flush vap event failed, left!=0: type=%d}", en_event_type);
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
    if (oal_unlikely((ul_core_id >= WLAN_FRW_MAX_NUM_CORES) || (en_policy >= FRW_SCHED_POLICY_BUTT))) {
        return OAL_PTR_NULL;
    }

    return &(g_event_manager[ul_core_id].st_sched_queue[en_policy]);
}

/*
 * �� �� ��  : frw_is_event_queue_empty
 * ��������  : �ж�����VAP��Ӧ���¼������Ƿ�Ϊ��
 */
oal_bool_enum_uint8 frw_is_event_queue_empty(frw_event_type_enum_uint8 uc_event_type)
{
    oal_uint32 ul_core_id;
    oal_uint8 uc_vap_id;
    oal_uint16 us_qid;
    frw_event_mgmt_stru *pst_event_mgmt = NULL;
    frw_event_queue_stru *pst_event_queue = NULL;

    /* ��ȡ�˺� */
    ul_core_id = oal_get_core_id();
    if (oal_unlikely(ul_core_id >= WLAN_FRW_MAX_NUM_CORES)) {
        oam_error_log1(0, OAM_SF_FRW, "{frw_event_post_event, core id = %d overflow!}", ul_core_id);

        return OAL_ERR_CODE_ARRAY_OVERFLOW;
    }

    pst_event_mgmt = &g_event_manager[ul_core_id];

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
 */
oal_bool_enum_uint8 frw_is_vap_event_queue_empty(oal_uint32 ul_core_id, oal_uint8 uc_vap_id, oal_uint8 event_type)
{
    frw_event_mgmt_stru *pst_event_mgmt;
    frw_event_queue_stru *pst_event_queue;
    oal_uint16 us_qid;

#if (_PRE_OS_VERSION_WIN32_RAW == _PRE_OS_VERSION)
    us_qid = (oal_uint16)event_type;
#else
    us_qid = (oal_uint16)(uc_vap_id * FRW_EVENT_TYPE_BUTT + event_type);
#endif

    /* ���ݺ˺� + ����ID���ҵ���Ӧ���¼����� */
    pst_event_mgmt = &g_event_manager[ul_core_id];

    pst_event_queue = &pst_event_mgmt->st_event_queue[us_qid];

    if (pst_event_queue->st_queue.uc_element_cnt != 0) {
        return OAL_FALSE;
    }

    return OAL_TRUE;
}

/*
 * �� �� ��  : frw_task_thread_condition_check
 * ��������  : �ж��Ƿ����¼���Ҫ����
 */
oal_uint8 frw_task_thread_condition_check(oal_uint32 ul_core_id)
{
    /*
     * ����OAL_TRUE
     *  1.���ȶ��зǿ�
     *  2.���ȶ������з�pause�Ķ���
     */
    oal_uint8 sched_policy;
    oal_uint ul_irq_flag = 0;
    oal_dlist_head_stru *pst_list = NULL;
    frw_event_sched_queue_stru *pst_sched_queue = NULL;
    frw_event_queue_stru *pst_event_queue = NULL;

    pst_sched_queue = g_event_manager[ul_core_id].st_sched_queue;

    for (sched_policy = 0; sched_policy < FRW_SCHED_POLICY_BUTT; sched_policy++) {
        oal_spin_lock_irq_save(&pst_sched_queue[sched_policy].st_lock, &ul_irq_flag);
        /* ���������������� */
        oal_dlist_search_for_each(pst_list, &pst_sched_queue[sched_policy].st_head)
        {
            pst_event_queue = oal_dlist_get_entry(pst_list, frw_event_queue_stru, st_list);
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
oal_module_symbol(frw_event_dispatch_event);
oal_module_symbol(frw_event_post_event);
oal_module_symbol(frw_event_table_register);
oal_module_symbol(frw_event_dump_event);
oal_module_symbol(frw_event_process_all_event);
oal_module_symbol(frw_event_flush_event_queue);
oal_module_symbol(frw_event_queue_info);
oal_module_symbol(frw_event_vap_pause_event);
oal_module_symbol(frw_event_vap_resume_event);
oal_module_symbol(frw_event_vap_flush_event);
oal_module_symbol(frw_event_get_sched_queue);

oal_module_symbol(frw_is_event_queue_empty);
oal_module_symbol(frw_event_sub_rx_adapt_table_init);
