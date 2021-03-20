

#ifndef __OAM_LOG_H__
#define __OAM_LOG_H__

/* ����ͷ�ļ����� */
#include "oal_ext_if.h"

/* �궨�� */
#define OAM_LOG_PARAM_MAX_NUM     4   /* �ɴ�ӡ���Ĳ������� */
#define OAM_LOG_PRINT_DATA_LENGTH 512 /* ÿ��д���ļ�����󳤶� */

#define OAM_LOG_VAP_INDEX_INTERVAL 2 /* �����������Ͳ�Ʒ�ϲ�������������� */

/* STRUCT���� */
typedef struct {
    oal_uint32 bit16_file_id : 16;
    oal_uint32 bit8_feature_id : 8;
    oal_uint32 bit4_vap_id : 4;
    oal_uint32 bit4_log_level : 4;
} om_log_wifi_para_stru;

/* �������� */
extern oal_uint32 oam_log_init(oal_void);

#endif /* end of oam_log.h */
