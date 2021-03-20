

#ifndef __OAL_FSM_H__
#define __OAL_FSM_H__

/* �궨�� */
#define OAL_FSM_MAX_NAME   8 /* ״̬������󳤶� */
#define OAL_FSM_MAX_STATES 100
#define OAL_FSM_MAX_EVENTS 100
#define OAL_FSM_STATE_NONE 255 /* invalid state */
#define OAL_FSM_EVENT_NONE 255 /* invalid event */

/* ״̬��Ϣ�ṹ���� */
typedef struct __oal_fsm_state_info {
    uint32_t state;                                /* ״̬ID */
    const char *name;                            /* ״̬�� */
    void (*oal_fsm_entry)(void *p_ctx); /* ���뱾״̬�Ĵ���ص�����ָ�� */
    void (*oal_fsm_exit)(void *p_ctx);  /* �˳���״̬�Ĵ���ص�����ָ�� */
    /* ��״̬�µ��¼�����ص�����ָ�� */
    uint32_t (*oal_fsm_event)(void *p_ctx, uint16_t event, uint16_t event_data_len, void *event_data);
} oal_fsm_state_info;

/* ״̬���ṹ���� */
typedef struct __oal_fsm {
    uint8_t uc_name[OAL_FSM_MAX_NAME]; /* ״̬������ */
    uint8_t uc_cur_state;              /* ��ǰ״̬ */
    uint8_t uc_prev_state;             /* ǰһ״̬������״̬�л��¼���״̬ */
    uint8_t uc_num_states;             /* ״̬����״̬���� */
    uint8_t uc_rsv[1];
    const oal_fsm_state_info *p_state_info;
    void *p_ctx;          /* �����ģ�ָ��״̬��ʵ��ӵ���� */
    void *p_oshandler;    /* ownerָ�룬ָ��VAP����device,�ɾ����״̬������ */
    uint16_t us_last_event; /* �������¼� */
    uint8_t uc_rsv1[2];
} oal_fsm_stru;

/* �������� */
extern uint32_t oal_fsm_create(void *p_oshandle, /* ״̬��owner��ָ�룬�Ե͹���״̬����ָ��VAP�ṹ */
                                 const uint8_t *p_name,                           /* ״̬�������� */
                                 void *p_ctx,                                   /* ״̬��context */
                                 oal_fsm_stru *pst_oal_fsm,                         /* oal״̬������ */
                                 uint8_t uc_init_state,                           /* ��ʼ״̬ */
                                 const oal_fsm_state_info *p_state_info,            /* ״̬��ʵ��ָ�� */
                                 uint8_t uc_num_states);                          /* ��״̬����״̬���� */

extern uint32_t oal_fsm_trans_to_state(oal_fsm_stru *p_fsm, uint8_t uc_state);

extern uint32_t oal_fsm_event_dispatch(oal_fsm_stru *p_fsm, uint16_t us_event,
                                         uint16_t us_event_data_len, void *p_event_data);

#endif
