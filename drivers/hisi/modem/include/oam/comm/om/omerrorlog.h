/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
 */

#ifndef __OM_ERRORLOG_H__
#define __OM_ERRORLOG_H__

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "vos.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef WIN32
#pragma warning(disable:4200) /* zero-sized array in struct/union */
#endif

#pragma pack(4)

/*****************************************************************************
  2 �궨��
*****************************************************************************/

/* ����ERR log �ϱ���� */
#define     ID_OM_ERR_LOG_CTRL_IND          (0x9001)
#define     ID_OM_ERR_LOG_REPORT_REQ        (0x9002)
#define     ID_OM_ERR_LOG_REPORT_CNF        (0x9003)



/* ƽ̨�����������ϱ���Ϣ���� */
#define     ID_OM_FAULT_ERR_LOG_IND         (0x9009) /* ����ô��Ժ���ܲ�ֹý���飬���������飬���и��� */

/* ƽ̨���澯�����ϱ���Ϣ���� */
#define     ID_OM_ALARM_ERR_LOG_IND         (0x900A)

/* ҵ����Ϣ�ռ���� */
#define     ID_OM_INFO_CLT_REPORT_REQ       (0x9010)
#define     ID_OM_INFO_CLT_REPORT_CNF       (0x9011)

#define     ID_OM_MTA_NV_REFRESH_RSP        (0x2002) /* ��ֹOM->MTA������Ϣ��ID_MTA_NV_REFRESH_RSPȡֵ�ظ�*/

/* OM�ڲ�a�˺�c��ʹ�� */
#define OM_ACPU_REPORT_BLACKLIST_NAME 0x8001
#define OM_ACPU_REPORT_PRIORITY_NAME 0x8002
#define OM_ACPU_REPORT_PERIOD_NAME 0x8003
/* 3 ö�ٶ��� */
typedef enum {
    OM_ERR_LOG_MSG_SET_ERR_REPORT = 0x01, /* �ϲ㴥�������ϱ� */
    OM_ERR_LOG_MSG_ERR_REPORT_END = 0x02, /* �����ϱ����� */
    OM_ERR_LOG_MSG_ON_OFF = 0x03,         /* ���������Ϳ��� */
    OM_ERR_LOG_MSG_ERR_REPORT = 0x04,     /* �����ϱ� */
    OM_ERR_LOG_MSG_FTM_REPORT = 0x05,     /* ����ģʽ�����ϱ� */
    OM_ERR_LOG_MSG_RESULT = 0x06,         /* �ϱ�Result */
    OM_ERR_LOG_MSG_FTM_REQ = 0x07,        /* ����ģʽ���� */
    OM_ERR_LOG_MSG_FTM_CNF = 0x08,        /* ����ģʽ��Ӧ */

    OM_ERR_LOG_MSG_FAULT_REPORT = 0x09, /* ƽ̨�����������ϱ� */

    OM_ERR_LOG_MSG_ALARM_REPORT = 0x0A, /* ƽ̨���澯�����ϱ� */

    OM_ERR_LOG_MSG_INFO_CLT_REQ = 0x10, /* ��Ϣ�ռ����� */
    OM_ERR_LOG_MSG_INFO_CLT_CNF = 0x11, /* ��Ϣ�ռ���Ӧ */
    OM_ERR_LOG_MSG_INFO_CLT_END = 0x12,

    OM_ERR_LOG_MSG_SMARTCDRX_REPORT = 0x13, /* SmartCDRXά����Ϣ�����ϱ� */
    OM_ERR_LOG_MSG_BLACK_LIST = 0x14,
    OM_ERR_LOG_MSG_PRIORITY_CFG = 0x15,
    OM_ERR_LOG_MSG_PERIOD_CFG = 0x16,
    OM_ERR_LOG_MSG_CFG_CNF = 0x17,

    OM_ERR_LOG_MSG_TYPE_BUTT
} chr_msg_type_e;
/* ���ر��� */
typedef enum {
    OM_APP_STATUS_CLOSE = 0x00,
    OM_APP_STATUS_OPEN = 0x01,

    OM_APP_STATUS_BUTT
} chr_app_switch_e;

typedef enum {
    OM_ERR_LOG_MOUDLE_ID_GUNAS = 0x020001,   /* GU NAS */
    OM_ERR_LOG_MOUDLE_ID_GAS = 0x020002,     /* GAS */
    OM_ERR_LOG_MOUDLE_ID_WAS = 0x020003,     /* WAS */
    OM_ERR_LOG_MOUDLE_ID_GUL2 = 0x020004,    /* GUL2 */
    OM_ERR_LOG_MOUDLE_ID_GUPHY = 0x020005,   /* GUPHY */
    OM_ERR_LOG_MOUDLE_ID_USIMM = 0x020006,   /* USIMM */
    OM_ERR_LOG_MOUDLE_ID_DRV_SCI = 0x020007, /* SCI DRV */
    OM_ERR_LOG_MOUDLE_ID_HIFI = 0x020008,    /* HIFI */
    OM_ERR_LOG_MOUDLE_ID_RRM = 0x020009,     /* RRM */
    OM_ERR_LOG_MOUDLE_ID_VISP = 0x02000A,    /* VISP CHR */

    OM_ERR_LOG_MOUDLE_ID_GNAS = 0x020011, /* G NAS */
    OM_ERR_LOG_MOUDLE_ID_GL2 = 0x020014,  /* GL2 */
    OM_ERR_LOG_MOUDLE_ID_GPHY = 0x020015, /* GPHY */

    OM_ERR_LOG_MOUDLE_ID_UNAS = 0x020021, /* U NAS */
    OM_ERR_LOG_MOUDLE_ID_UL2 = 0x020024,  /* UL2 */
    OM_ERR_LOG_MOUDLE_ID_UPHY = 0x020025, /* UPHY */

    OM_ERR_LOG_MOUDLE_ID_LMM = 0x020030,   /* LTE EMM */
    OM_ERR_LOG_MOUDLE_ID_ESM = 0x020031,   /* LTE ESM */
    OM_ERR_LOG_MOUDLE_ID_LSMS = 0x020032,  /* LTE SMS(����GU SMS������ʹ�ã�Ԥ��) */
    OM_ERR_LOG_MOUDLE_ID_LPP = 0x020033,   /* LTE LPP */
    OM_ERR_LOG_MOUDLE_ID_LRRC = 0x020034,  /* LTE RRC */
    OM_ERR_LOG_MOUDLE_ID_LTEL2 = 0x020035, /* LTEL2 */
    OM_ERR_LOG_MOUDLE_ID_LPHY = 0x020036,  /* LTE PHY */

    OM_ERR_LOG_MOUDLE_ID_TDS_MM = 0x020040,  /* TDS MM */
    OM_ERR_LOG_MOUDLE_ID_TDS_GMM = 0x020041, /* TDS GMM */
    OM_ERR_LOG_MOUDLE_ID_TDS_CC = 0x020042,  /* TDS CC */
    OM_ERR_LOG_MOUDLE_ID_TDS_SM = 0x020043,  /* TDS SM */
    OM_ERR_LOG_MOUDLE_ID_TDS_SMS = 0x020044, /* TDS SMS(����GU SMS������ʹ�ã�Ԥ��) */
    OM_ERR_LOG_MOUDLE_ID_TDS_LPP = 0x020045, /* TDS LPP */
    OM_ERR_LOG_MOUDLE_ID_TRRC = 0x020046,    /* TDS RRC */
    OM_ERR_LOG_MOUDLE_ID_TDSL2 = 0x020047,   /* TDSL2 */
    OM_ERR_LOG_MOUDLE_ID_TDS_PHY = 0x020048, /* TDS PHY */
    OM_ERR_LOG_MOUDLE_ID_LPSOM = 0x020049,   /* LPS OM */

    /* ����ģ�鲻�߱��շ���Ϣ���ܣ������COMģ����ת */
    OM_ERR_LOG_MOUDLE_ID_IMS = 0x020050,     /* IMS */
    OM_ERR_LOG_MOUDLE_ID_IMSA = 0x020051,    /* IMSA */
    OM_ERR_LOG_MOUDLE_ID_BBA_CHR = 0x020052, /* BBA CHR */

    OM_ERR_LOG_MOUDLE_ID_CNAS = 0x020060,    /* CNAS */
    OM_ERR_LOG_MOUDLE_ID_CAS = 0x020061,     /* CAS */
    OM_ERR_LOG_MOUDLE_ID_CL2 = 0x020062,     /* CL2 */
    OM_ERR_LOG_MOUDLE_ID_CPROC = 0x020063,   /* CPROC */
    OM_ERR_LOG_MOUDLE_ID_CSDR = 0x020064,    /* CSDR */
    OM_ERR_LOG_MOUDLE_ID_CHIFI = 0x020065,   /* CHIFI */
    OM_ERR_LOG_MOUDLE_ID_PPPC = 0x020066,    /* PPPC */
    OM_ERR_LOG_MOUDLE_ID_DRV = 0x020067,     /* DRV ���ڵ���͹����ϱ� */
    OM_ERR_LOG_MOUDLE_ID_CHR_STA = 0x020068, /* OMά�� */
    OM_ERR_LOG_MOUDLE_ID_NRRC = 0x020069,    /* NRRC */
    OM_ERR_LOG_MOUDLE_ID_NRL2    = 0x02006A,   /* NRL2 */

    OM_ERR_LOG_MOUDLE_ID_NRMM = 0x020070, /* NR MM */

    OM_ERR_LOG_MOUDLE_ID_NRSM = 0x020071, /* NR SM */

    OM_ERR_LOG_MOUDLE_ID_NR_AGENT = 0x020080, /* NPHY AGENT */

    OM_ERR_LOG_MOUDLE_ID_BUTT
} chr_module_id_e;

/* ������,��ap֮��Ĵ����� */
typedef enum {
    OM_APP_MSG_OK = 0x00,
    OM_APP_VCOM_DEV_INDEX_ERR = 0x01,
    OM_APP_MSG_LENGTH_ERR = 0x02,
    OM_APP_MSG_TYPE_ERR = 0x03,
    OM_APP_SEND_MODEM_ID_ERR = 0x04,
    OM_APP_MEM_ALLOC_MSG_ERR = 0x06,
    OM_APP_READ_NV_ERR = 0x07,
    OM_APP_GET_NVID_ERR = 0x08,
    OM_APP_MSG_MODULE_ID_ERR = 0x09,
    OM_APP_REPORT_NOT_FINISH_ERR = 0x0A,
    OM_APP_ERRLOG_START_TIMER_ERR = 0x0B,
    OM_APP_MEM_ALLOC_ERR = 0x0C,
    OM_APP_PARAM_INAVALID = 0x0D,
    OM_APP_SEND_CCPU_FAIL = 0x0E,
    OM_APP_SAVE_LIST_FAIL = 0x0F,

    OM_APP_SEND_RESULT_BUTT
} chr_app_send_result_e;

/* ������,��IMS֮��Ĵ����� */
typedef enum {
    OM_IMS_MSG_OK = 0x00,
    OM_IMS_MSG_TYPE_ERR = 0x01,
    OM_IMS_MSG_LENGTH_ERR = 0x02,
    OM_IMS_MSG_MODULE_ID_ERR = 0x03,
    OM_IMS_MSG_ALLOC_MSG_ERR = 0x04,
    OM_IMS_MSG_NULL = 0x05,
    OM_IMS_SEND_RESULT_BUTT
} chr_ims_send_result_e;

/* ����������ϱ�OM���ظ�������Ĵ����� */
typedef enum {
    CHR_BLACK_LIST_NOT_ALLOW = 0x100,
    CHR_THRESHOLD_NOT_ALLOW = 0x101,
    CHR_MSG_NAME_ERR = 0x102,
    CHR_INPUT_PARAM_NULL = 0x103,
    CHR_NV_READ_FAIL = 0x104,
    CHR_NV_SWITCH_CLOSE = 0x105,

    CHR_REPORT_RESULT_BUTT
} chr_report_result_e;

typedef enum {
    OM_INFO_CLT_SCENE_TYPE_FREQ_BAND = 0x4001,
    OM_INFO_CLT_SCENE_TYPE_NETWORK_INFO = 0x4002,

    OM_INFO_CLT_SCENE_TYPE_STATS_INFO = 0x4003,

    OM_INFO_CLT_SCENE_TYPE_BUTT
} chr_clt_scene_type_e;

/* COMM�ṩ������API�ӿ������֪ͨע��ӿ���ʽ */
typedef unsigned int (*chr_om_getdata_func_t)(unsigned int proj_mod, void *data, unsigned int len);

/*****************************************************************************
  4 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  5 ��Ϣͷ����
*****************************************************************************/


/*****************************************************************************
  6 ��Ϣ����
*****************************************************************************/


/*****************************************************************************
  7 STRUCT����
*****************************************************************************/


/*****************************************************************************
  APP��OM�ӿ�STRUCT����
*****************************************************************************/
/* APP-->OM error log ͨ����Ϣͷ��OM˽�ã������������ʹ�� */
typedef struct {
    unsigned int msgtype;
    unsigned int msgsn;
    unsigned int msglen;
} chr_alarm_msg_head_s;

/* APP����Error log�ϱ�type=0x01 App->OM */
typedef struct {
    chr_alarm_msg_head_s chr_om_header;
    unsigned int moduleid;
    unsigned short modemid;
    unsigned short faultid; /* ���ϳ������ */
} chr_app_om_req_s;

/* Result To App type=0x06  OM -> App */
typedef struct {
    chr_alarm_msg_head_s chr_om_header;
    unsigned int moduleid;
    unsigned short modemid;
    unsigned short resv;
    unsigned int status; /* result */
} chr_app_result_cnf_s;

/* Result To App type=0x17  OM -> App */
typedef struct {
    chr_alarm_msg_head_s chr_om_header;
    unsigned int status; /* result,app����������״̬��ֻ���type=0x17 */
} chr_app_result_cfg_cnf_s;

/* Error log �ϱ���� type=0x02 OM-> APP */
typedef struct {
    chr_alarm_msg_head_s chr_om_header;
    unsigned int moduleid;
    unsigned short modemid;
    unsigned short faultid;
    unsigned int report_status; /* result */
    unsigned int low_slice;     /* ��¼�����ϱ�����ʱ��ʱ��� */
    unsigned int high_slice;
} chr_app_report_status_s;

/* APP-->OM  type=0x13  �������·��� OM */
typedef struct {
    unsigned int moduleid; /* module id */
    unsigned short alarmid;      /*  alarm id */
    unsigned short alarm_type;
} chr_list_info_s;

typedef struct {
    chr_alarm_msg_head_s chr_om_header;
    chr_list_info_s black_list[0]; /*lint !e43 */
} chr_app_black_list_s;

/* APP-->OM  type=0x14  ���ȼ������·��� OM */
typedef struct {
    unsigned int moduleid;    /*  module id */
    unsigned short alarmid;    /*  alarm id */
    unsigned short priority;   /*  ���ȼ� */
    unsigned short alarm_type; /*  alarm type */
    unsigned short resv;
} chr_priority_info_s;

typedef struct {
    chr_alarm_msg_head_s chr_om_header;
    unsigned char packt_sn;           /* sn�ţ���ʶΪ�ڼ������ݣ� ��0��ʼ�����һ������ֵ����Ϊ0xFF */
    unsigned char count;              /* ��ǰ����AlarmID���ø��� */
    unsigned short resv;              /* Ԥ�� */
    chr_priority_info_s prior_map[0]; /*lint !e43 */
} chr_app_priority_cfg_s;

/* APP-->OM  type=0x15  ���������·��� OM */
typedef struct {
    unsigned int moduleid; /* module id */
    unsigned int count;    /* ������1���ȼ������ϱ����� */
} chr_period_cfg_s;

typedef struct {
    chr_alarm_msg_head_s chr_om_header;
    unsigned char msg_sn;           /* sn�ţ���ʶΪ�ڼ������ݣ� ��0��ʼ�����һ������ֵ����Ϊ0xFF */
    unsigned char count;            /*  ��ǰ����AlarmID���ø��� */
    unsigned char period;           /* �ϱ����ڣ��Ե�һ��Ϊ׼����λСʱ */
    unsigned char ucResved;         /*  Ԥ�� */
    chr_period_cfg_s period_map[0]; /*lint !e43 */
} chr_app_period_cfg_s;

/* OM�͸�����ӿ�STRUCT���� */

/* �·��������������ERR log�ϱ����� OM->���� */
typedef struct {
    VOS_MSG_HEADER
    unsigned int msg_name;
    unsigned short modemid;
    unsigned short faultid;
    unsigned short alarmid;
    unsigned short resv;
    unsigned int msgsn;

} chr_log_report_req_s;

/* OM��IMSͨ��Э��ͷ */
typedef struct {
    unsigned int moduleid;
    unsigned short faultid;
    unsigned short alarmid;
    unsigned short almtype;
    unsigned short resv;
    unsigned int msgtype;
    unsigned int msgsn;
    unsigned int rptlen;
} chr_ims_msg_head_s;

/* �����-->OM��ͨ��Э��ͷ */
typedef struct {
    VOS_MSG_HEADER
    unsigned int msgname;
    unsigned int moduleid;
    unsigned short faultid; /* �����ϱ�ʱ��GUTL����ע ,xģ����Ҫ��д */
    unsigned short alarmid;
    unsigned short alarm_type; /* �����ϱ�ʱ������ע */
    unsigned short resv;
} chr_rcv_data_header_s;

/* �������ѯ�����ϱ�CNF & ���������ϱ����� ����� -> OM */
typedef struct {
    chr_rcv_data_header_s chr_om_header;
    unsigned int msgtype;
    unsigned int msgsn;
    unsigned int rptlen;
    unsigned char data[4]; /* �ϱ������� */
} chr_fault_ind_s;

/* Error Log Payload header, OM����ע */
typedef struct {
    unsigned int moduleid;
    unsigned short modemid;
    unsigned short alarmid;
    unsigned short alarm_level;
    unsigned short alarm_type;
    unsigned int low_slice;
    unsigned int high_slice;
    unsigned int alarm_len;
} chr_data_header_s;

/* ���ظ�������ĺ��������ȼ��б� */
typedef struct {
    unsigned short alarm_type;
    unsigned short alarmid;

} chr_get_blacklist_s;

/* OM��IMS����Ľӿ�STRUCT���� */
/* OM --> IMS, REQ �� ����ʹ��OM->������Ľṹ���������һ������ */
typedef struct {
    chr_alarm_msg_head_s chr_om_header;
    unsigned int moduleid;
    unsigned short modemid;
    unsigned short faultid; /* ���ϳ������ */
    unsigned short alarmid;
    unsigned short resv;
    unsigned int msgsn;
} chr_app_req_ccpu_s;

/* �ṹ˵��  : OM����Errlog���������� */
typedef struct {
    unsigned int project_module; /* �豸�� */
    chr_om_getdata_func_t send_func;   /* �����ע�� �������ݺ��� */
} chr_register_proj_ctx_s;


/*
 * �ṹ��    : OM_ERRLOG_TRANS_MSG_STRU
 * �ṹ˵��  : Error Log ��Ϣ�����ṹ��
 */
typedef struct {
    unsigned char func_type;
    unsigned char cpuid;
    unsigned short lenth;
    unsigned int sn;         /* Sequence Number for Trace, Event, OTA msg. */
    unsigned int time_stamp; /* CPU time coming from ARM. */
    unsigned short prim_id;
    unsigned short tools_id;
    unsigned int date_type;
    unsigned char data[4];
} chr_trans_msg_s;

/* CLT_INFO�ƶ�Ƶ�㣬�ӿ�STRUCT���� */

/* APP����Info Clt�ϱ�type=0x10 App->OM */
typedef struct {
    chr_alarm_msg_head_s chr_om_header;
    unsigned short modemid;
    unsigned short infoid;
    unsigned int msglen;
    unsigned char msgdata[4];
} chr_app_info_clt_req_s;

/* ������ϱ�����Ϣ�ռ� ���� -> OM */
typedef struct {
    VOS_MSG_HEADER
    unsigned int msg_name;
    unsigned int msg_type;
    unsigned int msg_sn;
    unsigned int rpt_len;
    unsigned char data[4];
} chr_info_clt_report_cnf_s;
/* Info Clt Payload header */
typedef struct {
    unsigned int moduleid;
    unsigned short modemid;
    unsigned short scenetype;
    unsigned int msglen;
} chr_info_clt_header_s;

typedef struct {
    unsigned int mcc;
    unsigned int mnc;
} chr_info_clk_plmn_id_s;

/* �·������������Ϣ�ռ��ϱ����� OM->���� */
typedef struct {
    VOS_MSG_HEADER
    unsigned int msgname;
    unsigned short modemid;
    unsigned short scene_type;
    unsigned int msg_len;
    unsigned char data[4];
} chr_info_clt_report_req_s;

/* Modem errlog report, modem chr to ap interface define begin */
typedef struct {
    unsigned char product_name[20];
    unsigned short sub_eventid;
    unsigned char sub_event_cause;
    unsigned char log_version;
    unsigned int en_modem_hids_log; /* �Ƿ�ץȡHiDS Log */
} chr_modemlog_header_s;

/* Modem errlog report, modem chr to ap interface define end */
/* 8 UNION���� */

/* 9 OTHERS���� */

/* 10 �������� */
/*
 * ����������OM Ccpu���ո�����ϱ�����ģʽ���ݵ�APP�ص�����ע��ӿ�
 * ���������enProjectModule        ���ص��齨��ţ��̶�ΪOM_VOLTE_MOUDLE_PROJECT
 *           pFuncProjectCallbak    ���ص�����ָ��
 * ���������NA
 * ����ֵ��VOS_OK/VOS_ERR
 * �޸���ʷ��
 * 1.  ���ڣ�2014��1��2��
 * �޸����ݣ������ɺ���
 *     1������ģʽ�����ϱ�����֪ͨ�������Data�����������OmHeard��OMPayload
 *     2������ģʽ�����ϱ���Ϣ֪ͨ�������Data�����������OmHeard��OMPayload
 */
extern unsigned int mdrv_chr_register_getdata(unsigned int project_module,
                                                      chr_om_getdata_func_t project_callback_func);

/*
 * ����������OM Ccpu���ո�����ϱ�����ģʽ����
 * ���������enProjectModule    ���齨���
 *           pData              ����������
 *           ulLen              ���������ݳ���
 * ���������NA
 * ����ֵ��VOS_OK/VOS_ERR
 * �޸���ʷ��
 * 1.  ���ڣ�2014��1��2��
 * �޸����ݣ������ɺ���
 * ˵����pData��OMHeader��MsgType��ʼ���൱�ڵ������Ԥ������Header���֣�����Header���ֵ�SN�ֶ���COMM��д
 */
extern unsigned int mdrv_chr_getdata(unsigned int project_module, void *data, unsigned int len);
/*
 * ������������������ȡ�б�
 * ���������ulMsgModuleID          ���ص��齨���
 *           pstBlackList           ���������б�
 * ���������NA
 * ����ֵ��VOS_OK/VOS_ERR
 * �޸���ʷ��
 * 1.  ���ڣ�2017��11��28��
 * �޸����ݣ������ɺ���
 */
extern unsigned int mdrv_chr_getblacklist(unsigned int moduleid, chr_get_blacklist_s *blacklist,
                                                   unsigned int maxlen);
/*
 * �����������ṩ������������ϱ����ýӿ�,ע��˽ӿڻ��ͷŸ�����ڴ棬���������Ҫ�ͷ�
 * ����������ϱ���Ϣ
 * �޸���ʷ��
 * 1.  ���ڣ�2017��11��28��
 * �޸����ݣ������ɺ���
 */
extern unsigned int mdrv_chr_msgreport(chr_fault_ind_s *chr_msg);
/*
 * �������������ڷ��ظ�����Ƿ��ں���������,���ؽ�����ڻ��ǲ��ں�����
 * ���������ulMsgModuleID
 *           usAlarmId
 *           usAlmType��ֱͨ��д6����ֱͨ6֮���ֵ
 * ����ֵ: FALSE    0�����ں�������
 *         TRUE     1���ٺ�������
 * �޸���ʷ��
 * 1.  ���ڣ�2017��12��5��
 * �޸����ݣ������ɺ���
 */
extern unsigned int mdrv_chr_checkblacklist(unsigned int moduleid, unsigned short alarmid,
                                                     unsigned short alarm_type);



#if ((VOS_OS_VER == VOS_WIN32) || (VOS_OS_VER == VOS_NUCLEUS))
#pragma pack()
#else
#pragma pack(0)
#endif

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of omerrorlog.h */


