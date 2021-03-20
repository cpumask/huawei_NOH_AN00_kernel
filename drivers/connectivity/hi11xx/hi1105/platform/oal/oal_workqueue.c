

/* ͷ�ļ����� */
#include "oal_workqueue.h"

#include "oal_ext_if.h"
#include "oam_ext_if.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_OAL_WORKQUEUE_C

/* ȫ�ֱ������� */
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
OAL_STATIC oal_workqueue_stru *g_oal_workqueue; /* oal��������ȫ�ֱ��� */

int32_t oal_workqueue_schedule(oal_work_stru *pst_work)
{
    if (g_oal_workqueue == OAL_PTR_NULL) {
        oal_io_print("g_oal_workqueue is null.\n");
        return OAL_ERR_CODE_PTR_NULL;
    }
    return oal_queue_work(g_oal_workqueue, pst_work);
}

int32_t oal_workqueue_delay_schedule(oal_delayed_work *pst_work, unsigned long delay)
{
    if (g_oal_workqueue == OAL_PTR_NULL) {
        oal_io_print("g_oal_workqueue is null.\n");
        return OAL_ERR_CODE_PTR_NULL;
    }
    return oal_queue_delayed_work(g_oal_workqueue, pst_work, delay);
}

/*
 * �� �� ��  : oal_workqueue_init
 * ��������  : oal workqueue��ʼ��
 */
uint32_t oal_workqueue_init(void)
{
    g_oal_workqueue = oal_create_singlethread_workqueue_m("g_oal_workqueue");
    if (g_oal_workqueue == OAL_PTR_NULL) {
        oal_io_print("oal_workqueue_init: create oal workqueue failed.\n");
        return OAL_ERR_CODE_PTR_NULL;
    }

    return OAL_SUCC;
}

/*
 * �� �� ��  : oal_workqueue_exit
 * ��������  : oal workqueue�˳�����
 */
uint32_t oal_workqueue_exit(void)
{
    /* ɾ���������� */
    oal_destroy_workqueue(g_oal_workqueue);

    return OAL_SUCC;
}
oal_module_symbol(oal_workqueue_schedule);

oal_module_symbol(oal_workqueue_delay_schedule);

oal_module_license("GPL");

#endif
