

#ifndef __HMAC_AUTO_DDR_FREQ_H__
#define __HMAC_AUTO_DDR_FREQ_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 1 ����ͷ�ļ����� */
#include "oal_ext_if.h"
#include "mac_vap.h"
#include "dmac_ext_if.h"
#include "hmac_vap.h"
#include "hmac_config.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_AUTO_DDR_FREQ_H

/* 2 �궨�� */
#define HMAC_DDR_MAX_FREQ 2133000000 /* ���ں�����ϴ�ֵ, ȷ��DDR���ܵ����Ƶ�� */

/* 3 ö�ٶ��� */
typedef enum {
    HMAC_AUTO_DDR_FREQ_ADD_REQUEST,
    HMAC_AUTO_DDR_FREQ_REMOVE_REQUEST,
    HMAC_AUTO_DDR_FREQ_RETURN,

    HMAC_AUTO_DDR_FREQ_UPDATE_BUTT,
} hmac_auto_ddr_freq_update_enum;
/* 4 ȫ�ֱ������� */
/* 5 ��Ϣͷ���� */
/* 6 ��Ϣ���� */
/* 7 STRUCT���� */
typedef struct {
    oal_uint32 ul_auto_ddr_freq_pps_th_high;     /* ʹ�ܸ�DDRƵ�ʵ� pps��ֵ */
    oal_uint32 ul_auto_ddr_freq_pps_th_low;      /* ʹ�ܵ�DDRƵ�ʵ� pps��ֵ */
    oal_bool_enum_uint8 en_ddr_freq_updated;     /* �Ƿ������ں�����DDRƵ�� */
    oal_bool_enum_uint8 en_auto_ddr_freq_switch; /* ��̬DDR��Ƶ�����ܿ��� */
    struct pm_qos_request *pst_auto_ddr_freq;    /* DDRƵ������ṹ�� */
} hmac_auto_ddr_freq_mgmt_stru;

extern volatile hmac_auto_ddr_freq_mgmt_stru g_st_auto_ddr_freq_mgmt;

/* 8 UNION���� */
/* 9 OTHERS���� */
/* 10 �������� */
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION) && (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1102A_HOST)
extern oal_void hmac_auto_ddr_init(oal_void);
extern oal_void hmac_auto_ddr_exit(oal_void);
extern oal_void hmac_auto_set_ddr_freq(oal_uint32 ul_total_pps);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hmac_arp_offload.h */
