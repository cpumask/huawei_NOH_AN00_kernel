

#ifndef __OAL_DATA_STRU_H__
#define __OAL_DATA_STRU_H__

/* ����ͷ�ļ����� */
#include "oal_types.h"
#include "oal_mem.h"
#include "oam_wdk.h"
#include "securec.h"
#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_OAML_QUEUE_H

/* �궨�� */
#define OAL_QUEUE_DESTROY

/* �ж�x�Ƿ���2�������� */
#define OAL_IS_NOT_POW_OF_2(_x) ((_x) & ((_x) - 1))

/* STRUCT���� */
typedef struct {
    oal_uint8 uc_element_cnt;  /* ���������Ѿ���ŵ�Ԫ�ظ��� */
    oal_uint8 uc_max_elements; /* �����������ܴ�ŵ����Ԫ�ظ��� */
    oal_uint8 uc_tail_index;   /* ָ����һ��Ԫ�����λ�õ����� */
    oal_uint8 uc_head_index;   /* ָ��ǰԪ�س���λ�õ����� */
    uintptr_t *pul_buf;        /* ���л��� */
} oal_queue_stru;

typedef struct {
    oal_uint16 us_element_cnt;  /* ���������Ѿ���ŵ�Ԫ�ظ��� */
    oal_uint16 us_max_elements; /* �����������ܴ�ŵ����Ԫ�ظ��� */
    oal_uint16 us_tail_index;   /* ָ����һ��Ԫ�����λ�õ����� */
    oal_uint16 us_head_index;   /* ָ��ǰԪ�س���λ�õ����� */
    uintptr_t *pul_buf;         /* ���л��� */
} oal_queue_stru_16;

/*
 * �� �� ��  : oal_queue_set
 * ��������  : ���ö��в���
 * �������  : pst_queue      : ����ָ��
 *             pul_buf        : ָ����л�������ָ��
 *             us_max_elements: ���Ԫ�ظ���
 */
OAL_STATIC OAL_INLINE oal_void oal_queue_set(oal_queue_stru *pst_queue, oal_uint *pul_buf, oal_uint8 uc_max_elements)
{
    pst_queue->pul_buf = (uintptr_t *)pul_buf;

    pst_queue->uc_tail_index = 0;
    pst_queue->uc_head_index = 0;
    pst_queue->uc_element_cnt = 0;
    pst_queue->uc_max_elements = uc_max_elements;
}

OAL_STATIC OAL_INLINE oal_void oal_queue_set_16(oal_queue_stru_16 *pst_queue, oal_uint *pul_buf,
                                                oal_uint16 us_max_elements)
{
    pst_queue->pul_buf = (uintptr_t *)pul_buf;

    pst_queue->us_tail_index = 0;
    pst_queue->us_head_index = 0;
    pst_queue->us_element_cnt = 0;
    pst_queue->us_max_elements = us_max_elements;
}

/*
 * �� �� ��  : oal_queue_init
 * ��������  : ���г�ʼ��, uc_max_events������2����������
 * �������  : pst_queue      : ����ָ��
 *             uc_max_events: ���Ԫ�ظ���
 */
OAL_STATIC OAL_INLINE oal_uint32 oal_queue_init(oal_queue_stru *pst_queue, oal_uint8 uc_max_events)
{
    oal_uint *pul_buf = NULL;

    if (uc_max_events == 0) {
        return OAL_SUCC;
    } else {
        if (OAL_UNLIKELY(OAL_IS_NOT_POW_OF_2(uc_max_events))) {
            return OAL_ERR_CODE_CONFIG_UNSUPPORT;
        }

        pul_buf = (oal_uint *)OAL_MEM_ALLOC (OAL_MEM_POOL_ID_LOCAL,
                                             (oal_uint16)(uc_max_events * OAL_SIZEOF(oal_uint)),
                                             OAL_TRUE);
        if (OAL_UNLIKELY(pul_buf == OAL_PTR_NULL)) {
            return OAL_ERR_CODE_ALLOC_MEM_FAIL;
        }

        memset_s(pul_buf, uc_max_events * sizeof(oal_uint), 0, uc_max_events * sizeof(oal_uint));
        oal_queue_set(pst_queue, pul_buf, uc_max_events);

        return OAL_SUCC;
    }
}

/*
 * �� �� ��  : oal_queue_destroy
 * ��������  : ���ٶ���
 * �������  : pst_queue: ����ָ��
 */
OAL_STATIC OAL_INLINE oal_void oal_queue_destroy(oal_queue_stru *pst_queue)
{
    if (pst_queue == OAL_PTR_NULL) {
        return;
    }

    if (pst_queue->pul_buf == OAL_PTR_NULL) {
        return;
    }

    OAL_MEM_FREE(pst_queue->pul_buf, OAL_TRUE);

    oal_queue_set(pst_queue, OAL_PTR_NULL, 0);
}

/*
 * �� �� ��  : oal_queue_enqueue
 * ��������  : Ԫ�����
 * �������  : pst_queue: ����ָ��
 *             p_element: Ԫ��ָ��
 * �� �� ֵ  : OAL_SUCC ������������
 */
OAL_STATIC OAL_INLINE oal_uint32 oal_queue_enqueue(oal_queue_stru *pst_queue, oal_void *p_element)
{
    oal_uint8 uc_tail_index;

    /* �쳣: �������� */
    if (pst_queue->uc_element_cnt == pst_queue->uc_max_elements) {
        return OAL_FAIL;
    }

    uc_tail_index = pst_queue->uc_tail_index;

    /* ��Ԫ�صĵ�ַ�����ڶ����� */
    pst_queue->pul_buf[uc_tail_index] = (uintptr_t)p_element;

    uc_tail_index++;

    pst_queue->uc_tail_index = ((uc_tail_index >= pst_queue->uc_max_elements) ? 0 : uc_tail_index);

    pst_queue->uc_element_cnt++;

    return OAL_SUCC;
}

OAL_STATIC OAL_INLINE oal_uint32 oal_queue_enqueue_16(oal_queue_stru_16 *pst_queue, oal_void *p_element)
{
    oal_uint16 us_tail_index;

    /* �쳣: �������� */
    if (pst_queue->us_element_cnt == pst_queue->us_max_elements) {
        return OAL_FAIL;
    }

    us_tail_index = pst_queue->us_tail_index;

    /* ��Ԫ�صĵ�ַ�����ڶ����� */
    pst_queue->pul_buf[us_tail_index] = (uintptr_t)p_element;

    us_tail_index++;

    pst_queue->us_tail_index = ((us_tail_index >= pst_queue->us_max_elements) ? 0 : us_tail_index);

    pst_queue->us_element_cnt++;

    return OAL_SUCC;
}

/*
 * �� �� ��  : oal_queue_dequeue
 * ��������  : Ԫ�س���
 * �������  : pst_queue: ����ָ��
 * �� �� ֵ  : �ɹ�: �¼�ָ��
 */
OAL_STATIC OAL_INLINE oal_void *oal_queue_dequeue(oal_queue_stru *pst_queue)
{
    oal_uint8 uc_head_index;
    oal_void *p_element = NULL;

    /* �쳣: ����Ϊ�� */
    if (pst_queue->uc_element_cnt == 0) {
        return OAL_PTR_NULL;
    }

    uc_head_index = pst_queue->uc_head_index;

    p_element = (oal_void *)pst_queue->pul_buf[uc_head_index];

    uc_head_index++;

    pst_queue->uc_head_index = ((uc_head_index >= pst_queue->uc_max_elements) ? 0 : uc_head_index);
    pst_queue->uc_element_cnt--;

    return p_element;
}

OAL_STATIC OAL_INLINE oal_void *oal_queue_dequeue_16(oal_queue_stru_16 *pst_queue)
{
    oal_uint16 us_head_index;
    oal_void *p_element = NULL;

    /* �쳣: ����Ϊ�� */
    if (pst_queue->us_element_cnt == 0) {
        return OAL_PTR_NULL;
    }

    us_head_index = pst_queue->us_head_index;

    p_element = (oal_void *)pst_queue->pul_buf[us_head_index];

    us_head_index++;

    pst_queue->us_head_index = ((us_head_index >= pst_queue->us_max_elements) ? 0 : us_head_index);
    pst_queue->us_element_cnt--;

    return p_element;
}

/*
 * �� �� ��  : oal_queue_get_length
 * ��������  : ��ȡ�������Ѿ���ŵ�Ԫ�ظ���
 * �������  : pst_queue: ����ָ��
 * �� �� ֵ  : �����д�ŵ��¼�����
 */
OAL_STATIC OAL_INLINE oal_uint8 oal_queue_get_length(oal_queue_stru *pst_queue)
{
    if (pst_queue == OAL_PTR_NULL) {
        return 0;
    }

    return pst_queue->uc_element_cnt;
}

#endif /* end of oal_data_stru.h */
