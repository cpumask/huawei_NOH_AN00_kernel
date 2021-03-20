

/* ͷ�ļ����� */
#include "frw_event_deploy.h"
#include "frw_main.h"
#include "securec.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_FRW_EVENT_DEPLOY_C

/* ȫ�ֱ������� */
/* ipcȫ�ֽṹ��:ÿ����һ��,AMP����µ�ǰֻ����˫�˳��� */
OAL_STATIC frw_ipc_node_stru ipc_node[FRW_IPC_CORE_TYPE_BUTT];

/* ���˷��͸��Ӻ˵���Ϣ���� */
OAL_STATIC frw_ipc_msg_queue_stru queue_master_to_slave;

/* �Ӻ˷��͸����˵���Ϣ���� */
OAL_STATIC frw_ipc_msg_queue_stru queue_slave_to_master;

/* ����ʵ�� */
/* IPC�ӿ� */
OAL_STATIC oal_uint32 frw_ipc_init(oal_void);
OAL_STATIC oal_uint32 frw_ipc_init_master(frw_ipc_node_stru *pst_ipc_node);
OAL_STATIC oal_uint32 frw_ipc_init_slave(frw_ipc_node_stru *pst_ipc_node);
OAL_STATIC oal_uint32 frw_ipc_exit(oal_void);
OAL_STATIC oal_void frw_ipc_recv(frw_ipc_msg_mem_stru *pst_ipc_mem_msg);
OAL_STATIC oal_uint32 frw_ipc_send(frw_ipc_msg_mem_stru *pst_ipc_mem_msg);
OAL_STATIC oal_uint32 frw_ipc_send_connect_request(oal_void);
OAL_STATIC oal_uint32 frw_ipc_send_connect_response(oal_void);
OAL_STATIC oal_uint32 frw_ipc_send_inter_msg(oal_uint8 uc_msg_type, oal_uint8 *puc_data, oal_uint8 us_len);
OAL_STATIC oal_void frw_ipc_tx_complete(frw_ipc_msg_mem_stru *pst_msg);
OAL_STATIC oal_uint32 frw_ipc_event_queue_full(oal_void);
OAL_STATIC oal_uint32 frw_ipc_event_queue_empty(oal_void);

/* �¼�����ӿ� */
OAL_STATIC oal_uint32 frw_event_deploy_pipeline(oal_mem_stru *pst_mem_event,
                                                frw_event_deploy_enum_uint8 *en_deploy_result);

/*
 * �� �� ��  : frw_ipc_recv
 * ��������  : ��Ϣ���մ����������ж������Ļ��������ܼ���
 * �������  : pst_ipc_mem_msg: ��Ϣ�ṹ��ָ��
 * �� �� ֵ  : OAL_SUCC: �ɹ�; ����: ʧ��
 */
OAL_STATIC oal_void frw_ipc_recv(frw_ipc_msg_mem_stru *pst_ipc_mem_msg)
{
    frw_ipc_msg_header_stru *pst_header = NULL;  /* ��Ϣͷ���ṹ�� */
    frw_ipc_core_type_enum_uint8 en_type; /* CPU����: ���˻�Ӻ� */

    /* ��μ�� */
    if (OAL_UNLIKELY(pst_ipc_mem_msg == OAL_PTR_NULL)) {
        OAM_ERROR_LOG0(0, OAM_SF_FRW, "{frw_ipc_recv::OAL_PTR_NULL == pst_ipc_mem_msg}\r\n");
        return;
    }

    /* ��ȡ��Ϣͷ��, ��IPC����ʱ�Ѿ�Ų��puc_dataָ�룬�˴���ָ����ָ��IPCͷ */
    pst_header = (frw_ipc_msg_header_stru *)(pst_ipc_mem_msg->puc_data);

    if (OAL_UNLIKELY(pst_header == OAL_PTR_NULL ||
                     pst_header->uc_target_cpuid > FRW_IPC_CORE_NUM_BUTT ||
                     pst_header->uc_msg_type > FRW_IPC_MSG_TYPE_BUTT)) {
        OAM_ERROR_LOG0(0, OAM_SF_FRW, "{pst_header is incorrect.}");
        return;
    }

    /* ��ȡ��ǰ�ں���Ϣ */
    en_type = frw_ipc_get_core_type();

    /* �ж�seq number�Ƿ���ȷ */
    if (OAL_LIKELY(ipc_node[en_type].us_seq_num_rx_expect == pst_header->us_seq_number)) {
        ipc_node[en_type].us_seq_num_rx_expect++; /* �ñ���ֻ���������д */
    } else if (ipc_node[en_type].us_seq_num_rx_expect < pst_header->us_seq_number) {
        /* �����´��յ���ƥ��� */
        ipc_node[en_type].us_seq_num_rx_expect = pst_header->us_seq_number + 1;

#ifdef _PRE_DEBUG_MODE
        /* ��־���澯 */
        frw_ipc_log_recv_alarm(&ipc_node[en_type].st_log,
                               (pst_header->us_seq_number - ipc_node[en_type].us_seq_num_rx_expect));
#endif
    } else {
        /* ˵������Ҷ����� */
        /* �����´��յ���ƥ��� */
        ipc_node[en_type].us_seq_num_rx_expect = pst_header->us_seq_number + 1;
#ifdef _PRE_DEBUG_MODE
        /* ��־���澯���������㹫ʽ: ��ǰ���к�+���к����ֵ-���к�����ֵ+1 */
        frw_ipc_log_recv_alarm(&ipc_node[en_type].st_log,
                               (pst_header->us_seq_number + FRW_IPC_MAX_SEQ_NUMBER -
                               ipc_node[en_type].us_seq_num_rx_expect + 1));
#endif
    }

#ifdef _PRE_DEBUG_MODE
    /* ������־ */
    frw_ipc_log_recv(&ipc_node[en_type].st_log,
                     pst_header->us_seq_number,
                     pst_header->uc_target_cpuid,
                     pst_header->uc_msg_type);
#endif

    switch (pst_header->uc_msg_type) {
        case FRW_IPC_MSG_TYPE_EVENT:

            /* ��IPC����ʱ��ƫ�Ƶ�IPCͷ��λ�ã�����������Ҫ��ƫ�Ƶ��¼��ṹ��λ�� */
            pst_ipc_mem_msg->puc_data += FRW_IPC_MSG_HEADER_LENGTH;

            /* �¼�ֱ����Ӳ��� */
            frw_event_post_event(pst_ipc_mem_msg, OAL_GET_CORE_ID());

            break;

        case FRW_IPC_MSG_TYPE_TX_INT_ENABLE:
            /* �޸ķ���״̬���Ʊ�ʶ��ԭ�Ӳ��� */
            ipc_node[en_type].en_tx_int_ctl = FRW_IPC_TX_CTRL_ENABLED;

            break;

        case FRW_IPC_MSG_TYPE_TX_INT_DISENABLE:
            /* �޸ķ���״̬���Ʊ�ʶ��ԭ�Ӳ��� */
            ipc_node[en_type].en_tx_int_ctl = FRW_IPC_TX_CTRL_DISABLED;

            break;

        case FRW_IPC_MSG_TYPE_CONNECT_REQUEST:
            /* ����������Ӧ */
            frw_ipc_send_connect_response();

            break;

        case FRW_IPC_MSG_TYPE_CONNECT_RESPONSE:
            /* �޸�״̬��ԭ�Ӳ��� */
            ipc_node[en_type].en_states = FRW_IPC_CORE_STATE_CONNECTED;

            break;

        case FRW_IPC_MSG_TYPE_CONFIG_REQUEST:
        case FRW_IPC_MSG_TYPE_CONFIG_RESPONSE:
        case FRW_IPC_MSG_TYPE_ERROR_NOTICE:
        case FRW_IPC_MSG_TYPE_RESET_REQUEST:
        case FRW_IPC_MSG_TYPE_RESET_RESPONSE:
        case FRW_IPC_MSG_TYPE_OPEN_OAM:
        case FRW_IPC_MSG_TYPE_CLOSE_OAM:
        case FRW_IPC_MSG_TYPE_EXIT_REQUEST:
        case FRW_IPC_MSG_TYPE_EXIT_RESPONSE:
        default:

            break;
    }

    /* �ͷ���Ϣ */
    FRW_EVENT_FREE(pst_ipc_mem_msg);
}

/*
 * �� �� ��  : frw_ipc_send
 * ��������  : ����ӿ�, ���ͺ˼���Ϣ, ��Ϣ����Ϊ�¼�, ���øú���������ͷ��ڴ�
 * �������  : pst_ipc_mem_msg: ��Ϣ�ṹ��ָ��
 * �� �� ֵ  : OAL_SUCC: �ɹ�; ����: ʧ��
 */
OAL_STATIC oal_uint32 frw_ipc_send(frw_ipc_msg_mem_stru *pst_ipc_mem_msg)
{
    oal_uint32 ul_ret;
    frw_ipc_msg_header_stru *pst_header = NULL;   /* ��Ϣͷ���ṹ�� */
    frw_ipc_msg_queue_stru *pst_msg_queue = NULL; /* ��Ϣ���� */
    frw_ipc_core_type_enum_uint8 en_type;  /* CPU����: ���˻�Ӻ� */

    /* ��ȡ��ǰ�ں���Ϣ */
    en_type = frw_ipc_get_core_type();
    FRW_IPC_GET_MSG_QUEUE(pst_msg_queue, en_type);

    /* ��鵱ǰIPCģ��״̬ */
    if (ipc_node[en_type].en_states != FRW_IPC_CORE_STATE_CONNECTED) {
        OAM_WARNING_LOG2(0, OAM_SF_FRW,
            "{frw_ipc_send::FRW_IPC_CORE_STATE_CONNECTED != ipc_node[en_type].en_states. en_type:%d, en_states:%d}\r\n",
            en_type, ipc_node[en_type].en_states);
        return OAL_FAIL;
    }

    /* ��ȡIPCͷ�� */
    pst_header = (frw_ipc_msg_header_stru *)frw_ipc_get_header(pst_ipc_mem_msg);

    /* �˼���Ϣͷ����� */
    pst_header->us_seq_number = ipc_node[en_type].us_seq_num_tx_expect;
    pst_header->uc_target_cpuid = ipc_node[en_type].en_target_cpuid;
    pst_header->uc_msg_type = FRW_IPC_MSG_TYPE_EVENT;

    /* ���ͺ˼���Ϣ */
    ul_ret = frw_ipc_msg_queue_send(pst_msg_queue,
                                    pst_ipc_mem_msg,
                                    ipc_node[en_type].en_tx_int_ctl,
                                    ipc_node[en_type].en_target_cpuid);

    if (OAL_UNLIKELY(ul_ret == OAL_ERR_CODE_IPC_QUEUE_FULL)) {
#ifdef _PRE_DEBUG_MODE
        frw_ipc_log_send_alarm(&ipc_node[en_type].st_log);
#endif
        OAM_WARNING_LOG1(0, OAM_SF_FRW, "{frw_ipc_send::frw_ipc_msg_queue_send return err code: %d}",
                         OAL_ERR_CODE_IPC_QUEUE_FULL);
        return ul_ret;
    }

    if (OAL_UNLIKELY(ul_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_FRW, "{frw_ipc_send::frw_ipc_msg_queue_send return err code:%d}", ul_ret);
        return ul_ret;
    }

    /* �������к� */
    ipc_node[en_type].us_seq_num_tx_expect++;

    /* �����¼��ṹ�����ü��� */
    pst_ipc_mem_msg->uc_user_cnt++;

#ifdef _PRE_DEBUG_MODE
    /* ������־ */
    frw_ipc_log_send(&ipc_node[en_type].st_log,
                     pst_header->us_seq_number,
                     pst_header->uc_target_cpuid,
                     pst_header->uc_msg_type);
#endif

    return OAL_SUCC;
}

/*
 * �� �� ��  : frw_ipc_tx_complete
 * ��������  : ��������жϴ�����OFFLOAD����²��ã�OFFLOAD�������Ҫ��
 * �������  : pst_msg:��Ϣ�ṹ��ָ��
 * �� �� ֵ  : OAL_SUCC: �ɹ�; ����: ʧ��
 */
OAL_STATIC oal_void frw_ipc_tx_complete(frw_ipc_msg_mem_stru *pst_msg)
{
}

/*
 * �� �� ��  : frw_ipc_init
 * ��������  : ����ӿڣ�IPCģ���ʼ����OFFLOAD�������CPU�������˵��øú���
 * �� �� ֵ  : OAL_SUCC���ɹ���������ʧ��
 */
OAL_STATIC oal_uint32 frw_ipc_init(oal_void)
{
    oal_uint32 ul_ret;

    if (frw_ipc_get_core_type() == FRW_IPC_CORE_TYPE_MASTER) {
        ul_ret = frw_ipc_init_master(&ipc_node[FRW_IPC_CORE_TYPE_MASTER]);
    } else {
        ul_ret = frw_ipc_init_slave(&ipc_node[FRW_IPC_CORE_TYPE_SLAVE]);
    }

    if (OAL_UNLIKELY(ul_ret != OAL_SUCC)) {
        OAM_WARNING_LOG0(0, OAM_SF_FRW, "{frw ipc init fail.}");
        return ul_ret;
    }

    /* �������� */
    ul_ret = frw_ipc_send_connect_request();
    if (OAL_UNLIKELY(ul_ret != OAL_SUCC)) {
        OAM_WARNING_LOG0(0, OAM_SF_FRW, "{frw ipc connect fail.}");
        return ul_ret;
    }

    /* �����º���ע�ᵽ�¼�����ģ���� */
    frw_event_ipc_event_queue_full_register(frw_ipc_event_queue_full);
    frw_event_ipc_event_queue_empty_register(frw_ipc_event_queue_empty);

    return OAL_SUCC;
}

/*
 * �� �� ��  : frw_ipc_init_master
 * ��������  : ����IPCģ���ʼ����OFFLOAD�������CPU��WIFI�����øú���
 * �� �� ֵ  : OAL_SUCC���ɹ���������ʧ��
 */
OAL_STATIC oal_uint32 frw_ipc_init_master(frw_ipc_node_stru *pst_ipc_node)
{
    oal_uint32 ul_ret;

    /* ȫ�ֽṹ���ʼ�� */
    pst_ipc_node->en_cpuid = FRW_IPC_CORE_ID_MASTER;
    pst_ipc_node->en_target_cpuid = FRW_IPC_CORE_ID_SLAVE;
    pst_ipc_node->en_cpu_type = FRW_IPC_CORE_TYPE_MASTER;
    pst_ipc_node->us_seq_num_rx_expect = 0;
    pst_ipc_node->us_seq_num_tx_expect = 0;
    pst_ipc_node->en_states = FRW_IPC_CORE_STATE_INIT;
    pst_ipc_node->en_tx_int_ctl = FRW_IPC_TX_CTRL_ENABLED;

    /* ���������Ϣ���г�ʼ�� */
    /* ���Ͷ��г�ʼ�� */
    ul_ret = frw_ipc_msg_queue_init(&queue_master_to_slave,
                                    FRW_IPC_MASTER_TO_SLAVE_QUEUE_MAX_NUM);
    if (OAL_UNLIKELY(ul_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_FRW,
                         "{frw_ipc_init_master: frw_ipc_msg_queue_init queue_master_to_slave return err code:%d}",
                         ul_ret);
        return ul_ret;
    }

    /* ���ն��г�ʼ�� */
    ul_ret = frw_ipc_msg_queue_init(&queue_slave_to_master,
                                    FRW_IPC_SLAVE_TO_MASTER_QUEUE_MAX_NUM);
    if (OAL_UNLIKELY(ul_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_FRW,
                         "{frw_ipc_init_master:: frw_ipc_msg_queue_init queue_slave_to_master return err code:%d}",
                         ul_ret);
        return ul_ret;
    }

    /* �ж�ע�� */
    pst_ipc_node->st_irq_dev.p_irq_intr_func = frw_ipc_msg_queue_recv;
    pst_ipc_node->st_irq_dev.ul_irq = OAL_IRQ_NUM;
    pst_ipc_node->st_irq_dev.l_irq_type = OAL_SA_SHIRQ;
    pst_ipc_node->st_irq_dev.pc_name = "ipc";
    pst_ipc_node->st_irq_dev.p_drv_arg = (void *)&queue_slave_to_master;

    if (OAL_UNLIKELY(oal_irq_setup(&pst_ipc_node->st_irq_dev) != 0)) {
        OAM_WARNING_LOG0(0, OAM_SF_FRW, "{frw_ipc_msg_queue_setup_irq:: oal_irq_setup fail!}");
        return OAL_FAIL;
    }

    /* �ص�����ע�� */
    pst_ipc_node->st_ipc_msg_callback.p_rx_complete_func = frw_ipc_recv;
    pst_ipc_node->st_ipc_msg_callback.p_tx_complete_func = frw_ipc_tx_complete;
    ul_ret = frw_ipc_msg_queue_register_callback(&pst_ipc_node->st_ipc_msg_callback);
    if (OAL_UNLIKELY(ul_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_FRW,
                         "{frw_ipc_init_master: frw_ipc_msg_queue_register_callback return err code:%d}",
                         ul_ret);
        return ul_ret;
    }

#ifdef _PRE_DEBUG_MODE
    /* ��־��ʼ�� */
    ul_ret = frw_ipc_log_init(&pst_ipc_node->st_log);
    if (OAL_UNLIKELY(ul_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_FRW, "{frw_ipc_init_master: frw_ipc_log_init return err code:%d}", ul_ret);
        return ul_ret;
    }
#endif

    return OAL_SUCC;
}

/*
 * �� �� ��  : frw_ipc_init_slave
 * ��������  : �Ӻ�IPCģ���ʼ��: OFFLOAD����²���Ҫ��
 * �� �� ֵ  : OAL_SUCC���ɹ���������ʧ��
 */
OAL_STATIC oal_uint32 frw_ipc_init_slave(frw_ipc_node_stru *pst_ipc_node)
{
    oal_uint32 ul_ret;

    /* ȫ�ֽṹ���ʼ�� */
    pst_ipc_node->en_cpuid = (oal_uint8)OAL_GET_CORE_ID();
    pst_ipc_node->en_target_cpuid = FRW_IPC_CORE_ID_MASTER;
    pst_ipc_node->en_cpu_type = FRW_IPC_CORE_TYPE_SLAVE;
    pst_ipc_node->us_seq_num_rx_expect = 0;
    pst_ipc_node->us_seq_num_tx_expect = 0;
    pst_ipc_node->en_states = FRW_IPC_CORE_STATE_INIT;
    pst_ipc_node->en_tx_int_ctl = FRW_IPC_TX_CTRL_ENABLED;

    /* �ж�ע�� */
    pst_ipc_node->st_irq_dev.p_irq_intr_func = frw_ipc_msg_queue_recv;
    pst_ipc_node->st_irq_dev.ul_irq = OAL_IRQ_NUM;
    pst_ipc_node->st_irq_dev.l_irq_type = OAL_SA_SHIRQ;
    pst_ipc_node->st_irq_dev.pc_name = "ipc";
    pst_ipc_node->st_irq_dev.p_drv_arg = (void *)&queue_master_to_slave;

    if (OAL_UNLIKELY(oal_irq_setup(&pst_ipc_node->st_irq_dev) != 0)) {
        OAM_WARNING_LOG0(0, OAM_SF_FRW, "{frw_ipc_msg_queue_setup_irq: oal_irq_setup fail!}");
        return OAL_FAIL;
    }

    /* �ص�����ע�� */
    pst_ipc_node->st_ipc_msg_callback.p_rx_complete_func = frw_ipc_recv;
    pst_ipc_node->st_ipc_msg_callback.p_tx_complete_func = frw_ipc_tx_complete;
    ul_ret = frw_ipc_msg_queue_register_callback(&pst_ipc_node->st_ipc_msg_callback);
    if (OAL_UNLIKELY(ul_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_FRW,
                         "{frw_ipc_init_slave:: frw_ipc_msg_queue_register_callback return err code:%d}",
                         ul_ret);
        return ul_ret;
    }

#ifdef _PRE_DEBUG_MODE
    /* ��־��ʼ�� */
    ul_ret = frw_ipc_log_init(&pst_ipc_node->st_log);
    if (OAL_UNLIKELY(ul_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_FRW, "{frw_ipc_init_slave: frw_ipc_log_init return err code:%d}", ul_ret);
        return ul_ret;
    }
#endif

    return OAL_SUCC;
}

/*
 * �� �� ��  : frw_ipc_exit
 * ��������  : �������ͷ���Դ
 * �� �� ֵ  : OAL_SUCC���˳���Ӧ����ɹ����������˳���Ӧ����ʧ��
 */
OAL_STATIC oal_uint32 frw_ipc_exit(oal_void)
{
    oal_uint32 ul_ret;
    frw_ipc_core_type_enum_uint8 en_type; /* CPU����: ���˻�Ӻ� */

    /* ��ȡ��ǰ�ں���Ϣ */
    en_type = frw_ipc_get_core_type();

    /* �޸�IPCģ��״̬ */
    ipc_node[en_type].en_states = FRW_IPC_CORE_STATE_EXIT;

    /* ע���˼��ж� */
    oal_irq_free(&ipc_node[en_type].st_irq_dev);

    /* �ͷ���Ϣ���� */
    if (en_type == FRW_IPC_CORE_ID_MASTER) { /* ���� */
        /* ���Ͷ���ע�� */
        ul_ret = frw_ipc_msg_queue_destroy(&queue_master_to_slave);
        if (OAL_UNLIKELY(ul_ret != OAL_SUCC)) {
            OAM_WARNING_LOG1(0, OAM_SF_FRW,
                             "{frw_ipc_exit: frw_ipc_msg_queue_destroy queue_master_to_slave return err code:%d}",
                             ul_ret);
            return ul_ret;
        }

        /* ���ն���ע�� */
        ul_ret = frw_ipc_msg_queue_destroy(&queue_slave_to_master);
        if (OAL_UNLIKELY(ul_ret != OAL_SUCC)) {
            OAM_WARNING_LOG1(0, OAM_SF_FRW,
                             "frw_ipc_exit:: frw_ipc_msg_queue_destroy queue_slave_to_master return err code:%d",
                             ul_ret);
            return ul_ret;
        }
    }

#ifdef _PRE_DEBUG_MODE
    /* ��ӡ��־��Ϣ */
    frw_ipc_log_tx_print(&ipc_node[en_type].st_log);
    frw_ipc_log_rx_print(&ipc_node[en_type].st_log);

    /* �˳���־ģ�� */
    ul_ret = frw_ipc_log_exit(&ipc_node[en_type].st_log);
    if (OAL_UNLIKELY(ul_ret != OAL_SUCC)) {
        return ul_ret;
    }
#endif

    return OAL_SUCC;
}

/*
 * �� �� ��  : frw_ipc_send_connect_request
 * ��������  : �����첽��ʽ�������ӣ��ú���ֻ������������
 *             ���жϷ��������н��մ���������Ӧ�����ı�IPC״̬
 * �� �� ֵ  : OAL_SUCC��������������ɹ���������������������ʧ��
 */
OAL_STATIC oal_uint32 frw_ipc_send_connect_request(oal_void)
{
    oal_uint32 ul_ret;

    ul_ret = frw_ipc_send_inter_msg(FRW_IPC_MSG_TYPE_CONNECT_REQUEST,
                                    OAL_PTR_NULL,
                                    0);

    if (OAL_UNLIKELY(ul_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_FRW, "{frw_ipc_send_connect_request: frw_ipc_send_inter_msg return %d}", ul_ret);
        return ul_ret;
    }

    return OAL_SUCC;
}

/*
 * �� �� ��  : frw_ipc_send_connect_response
 * ��������  : �������ж������ģ������첽��ʽ�������ӣ��ú���ֻ����������Ӧ��
 * �� �� ֵ  : OAL_SUCC��������������ɹ���������������������ʧ��
 */
OAL_STATIC oal_uint32 frw_ipc_send_connect_response(oal_void)
{
    oal_uint32 ul_ret;

    ul_ret = frw_ipc_send_inter_msg(FRW_IPC_MSG_TYPE_CONNECT_RESPONSE, OAL_PTR_NULL, 0);

    if (OAL_UNLIKELY(ul_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_FRW, "{frw_ipc_send_connect_response: frw_ipc_send_inter_msg return %d}", ul_ret);
        return ul_ret;
    }

    return OAL_SUCC;
}

/*
 * �� �� ��  : frw_ipc_send_inter_msg
 * ��������  : ���ͺ˼�ͨ���ڲ���Ϣ
 * �������  : uc_msg_type: �ڲ���Ϣ����
 *             puc_data: ��Ҫ���͵�����
 *             us_len: ���ݳ���
 * �� �� ֵ  : OAL_SUCC�����ͳɹ���OAL_FAIL������ʧ��
 */
OAL_STATIC oal_uint32 frw_ipc_send_inter_msg(oal_uint8 uc_msg_type,
                                             oal_uint8 *puc_data,
                                             oal_uint8 us_len)
{
    oal_uint32 ul_ret;
    frw_ipc_msg_mem_stru *pst_msg_mem_stru = NULL; /* ��Ϣ�ڴ�� */
    frw_ipc_inter_msg_stru *pst_ipc_msg = NULL;    /* ��Ϣ�ṹ�� */
    frw_ipc_msg_queue_stru *pst_msg_queue = NULL;  /* ��Ϣ���� */
    frw_ipc_core_type_enum_uint8 en_type;   /* CPU����: ���˻�Ӻ� */
    oal_uint8 ul_length;

    /* ��ȡ��ǰ�ں���Ϣ */
    en_type = frw_ipc_get_core_type();
    FRW_IPC_GET_MSG_QUEUE(pst_msg_queue, en_type);

    /* ����˼��ڲ���Ϣ�ڴ� */
    // cppcheck-suppress * /* ����pst_ipc_msg������ָ�����ø澯 */
    ul_length = OAL_SIZEOF(frw_ipc_inter_msg_stru) - OAL_SIZEOF(pst_ipc_msg->auc_data) + us_len;
    pst_msg_mem_stru = FRW_EVENT_ALLOC(ul_length);
    if (OAL_UNLIKELY(pst_msg_mem_stru == OAL_PTR_NULL)) {
        OAM_ERROR_LOG0(0, OAM_SF_FRW, "{frw_ipc_send_inter_msg: FRW_EVENT_ALLOC fail!}");
        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }

    /* ��ȡ��Ϣ��ַ */
    pst_ipc_msg = (frw_ipc_inter_msg_stru *)frw_ipc_get_header(pst_msg_mem_stru);

    /* �����Ϣͷ */
    pst_ipc_msg->st_ipc_hdr.us_seq_number = ipc_node[en_type].us_seq_num_tx_expect;
    pst_ipc_msg->st_ipc_hdr.uc_target_cpuid = ipc_node[en_type].en_target_cpuid;
    pst_ipc_msg->st_ipc_hdr.uc_msg_type = uc_msg_type;
    pst_ipc_msg->ul_length = us_len;

    /* ������� */
    if (us_len > 0) {
        ul_ret = memcpy_s((oal_void *)pst_ipc_msg->auc_data, us_len,
                          (oal_void *)puc_data, us_len);
        if (ul_ret != EOK) {
            OAM_WARNING_LOG0(0, OAM_SF_FRW, "{frw_ipc_send_inter_msg: memcpy_s failed!}");
            FRW_EVENT_FREE(pst_msg_mem_stru);
            return OAL_FAIL;
        }
    }

    /* �����ڲ���Ϣ */
    ul_ret = frw_ipc_msg_queue_send(pst_msg_queue,
                                    pst_msg_mem_stru,
                                    ipc_node[en_type].en_tx_int_ctl,
                                    ipc_node[en_type].en_target_cpuid);

    if (OAL_UNLIKELY(ul_ret != OAL_SUCC)) {
        OAM_WARNING_LOG0(0, OAM_SF_FRW, "{send inter msg fail.}");
        FRW_EVENT_FREE(pst_msg_mem_stru);
        return ul_ret;
    }

#ifdef _PRE_DEBUG_MODE
    /* ������־ */
    frw_ipc_log_send(&ipc_node[en_type].st_log,
                     ipc_node[en_type].us_seq_num_tx_expect,
                     ipc_node[en_type].en_target_cpuid,
                     uc_msg_type);
#endif

    /* �������к� */
    ipc_node[en_type].us_seq_num_tx_expect++;

    return OAL_SUCC;
}

/*
 * �� �� ��  : frw_ipc_reset
 * ��������  : IPCģ�鸴λ
 * �� �� ֵ  : OAL_SUCC���ɹ���������ʧ��
 */
oal_uint32 frw_ipc_reset(oal_void)
{
    return OAL_SUCC;
}

/*
 * �� �� ��  : frw_ipc_smp_dispatch
 * ��������  : ��˱��ķַ��������
 * �������  : buf-����֡�ṹ��ָ��
 * �� �� ֵ  : OAL_SUCC���ɹ���������ʧ��
 */
oal_uint32 frw_ipc_smp_dispatch(oal_void *buf)
{
    return OAL_SUCC;
}

/*
 * �� �� ��  : frw_ipc_smp_ordering
 * ��������  : ���ı�����
 * �������  : buf-����֡�ṹ��ָ��
 * �� �� ֵ  : OAL_SUCC���ɹ���������ʧ��
 */
oal_uint32 frw_ipc_smp_ordering(oal_void *buf)
{
    return OAL_SUCC;
}

/*
 * �� �� ��  : frw_ipc_event_queue_full
 * ��������  : ���¼�����ģ�鷢�ֶ��и���ĳ����ֵʱ�����øýӿ�֪ͨ�Է��ں�
 * �� �� ֵ  : OAL_SUCC���ɹ���������ʧ��
 */
OAL_STATIC oal_uint32 frw_ipc_event_queue_full(oal_void)
{
    oal_uint32 ul_ret;

    ul_ret = frw_ipc_send_inter_msg(FRW_IPC_MSG_TYPE_TX_INT_DISENABLE, OAL_PTR_NULL, 0);

    if (OAL_UNLIKELY(ul_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_FRW, "{frw_ipc_event_queue_full: frw_ipc_send_inter_msg return err code:%d}",
                         ul_ret);
        return ul_ret;
    }

    return OAL_SUCC;
}

/*
 * �� �� ��  : frw_ipc_event_queue_empty
 * ��������  : ���¼�����ģ�鷢�ֶ��е���ĳ����ֵʱ�����øýӿ�֪ͨ�Է��ں�
 * �� �� ֵ  : OAL_SUCC���ɹ���������ʧ��
 */
OAL_STATIC oal_uint32 frw_ipc_event_queue_empty(oal_void)
{
    oal_uint32 ul_ret;

    ul_ret = frw_ipc_send_inter_msg(FRW_IPC_MSG_TYPE_TX_INT_ENABLE, OAL_PTR_NULL, 0);

    if (OAL_UNLIKELY(ul_ret != OAL_SUCC)) {
        OAM_WARNING_LOG1(0, OAM_SF_FRW, "{frw_ipc_event_queue_empty: frw_ipc_send_inter_msg return err code:%d}",
                         ul_ret);
        return ul_ret;
    }

    return OAL_SUCC;
}

/*
 * �� �� ��  : frw_event_deploy_init
 * ��������  : ����ӿڣ��¼�����ģ���ʼ���ӿ�
 * �� �� ֵ  : OAL_SUCC-�ɹ�; ����-ʧ��
 */
oal_uint32 frw_event_deploy_init(oal_void)
{
    oal_uint32 ul_ret;

    ul_ret = frw_ipc_init();
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG0(0, OAM_SF_FRW, "{frw event deploy init fail.}");
        return ul_ret;
    }

    /*
     * �¼�����ģ�����ݽṹ��ʼ��
     * ���ּ�����¼������վ��ⲿ��ʽ���г�ʼ��
     * �����, �����HMAC DMAC PIPELINE��ʽ����ע��frw_event_deploy_pipeline
     * �����, ����ǰ���CHIP,DEVICE,VAP��ʽ����ע��frw_event_deploy_owner
     */
    frw_event_deploy_register(frw_event_deploy_pipeline);

    return OAL_SUCC;
}

/*
 * �� �� ��  : frw_event_deploy_exit
 * ��������  : ����ӿڣ��¼�����ģ���˳��ӿ�
 * �� �� ֵ  : OAL_SUCC-�ɹ�; ����-ʧ��
 */
oal_uint32 frw_event_deploy_exit(oal_void)
{
    oal_uint32 ul_ret;

    ul_ret = frw_ipc_exit();
    if (ul_ret != OAL_SUCC) {
        OAM_WARNING_LOG0(0, OAM_SF_FRW, "{frw event deploy exit fail.}");
        return ul_ret;
    }
    return OAL_SUCC;
}

/*
 * �� �� ��  : frw_event_deploy_pipeline
 * ��������  : �ڲ��������¼�����ӿڣ��ҽӵ��¼�����ģ���У�����PIPELINE����
 * �������  : pst_mem_event: �¼��ڴ��ָ��
 * �������  : en_deploy: FRW_EVENT_DEPLOY_IPC-���¼���ҪIPC
 *                        FRW_EVENT_DEPLOY_POST-���¼���Ҫ�����
 * �� �� ֵ  : OAL_SUCC-�ɹ�; ����-ʧ��
 */
OAL_STATIC oal_uint32 frw_event_deploy_pipeline(frw_ipc_msg_mem_stru *pst_ipc_mem_msg,
                                                frw_event_deploy_enum_uint8 *en_deploy_result)
{
    frw_event_hdr_stru *pst_event_hdr = NULL;

    if (OAL_UNLIKELY(pst_ipc_mem_msg == OAL_PTR_NULL) || (en_deploy_result == OAL_PTR_NULL)) {
        OAM_ERROR_LOG2(0, OAM_SF_FRW,
                       "{frw_event_deploy_pipeline: pst_ipc_mem_msg/en_deploy_result null ptr err:%d, %d}",
                       (uintptr_t)pst_ipc_mem_msg, (uintptr_t)en_deploy_result);
        return OAL_ERR_CODE_PTR_NULL;
    }

    pst_event_hdr = (frw_event_hdr_stru *)pst_ipc_mem_msg->puc_data;

    if (FRW_EVENT_PIPELINE_STAGE_0 == pst_event_hdr->en_pipeline) {
        (*en_deploy_result) = FRW_EVENT_DEPLOY_NON_IPC;
    } else {
        (*en_deploy_result) = FRW_EVENT_DEPLOY_IPC;

        frw_ipc_send(pst_ipc_mem_msg);
    }

    return OAL_SUCC;
}
