

#ifndef __HMAC_DSCR_TH_OPT_H__
#define __HMAC_DSCR_TH_OPT_H__

/* 1 ����ͷ�ļ����� */
#include "oal_ext_if.h"
#include "hmac_device.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_DSCR_TH_OPT_H
/* 2 �궨�� */
#define HMAC_LARGE_DSCR_TH_LEVEL  6                              /* ������������������С */
#define HMAC_SMALL_DSCR_TH_LEVEL  6                              /* С�����������������С */
#define HMAC_LARGE_DSCR_PPS_LEVEL (HMAC_LARGE_DSCR_TH_LEVEL - 1) /* ���trx PPS��ֵ�����С */
#define HMAC_SMALL_DSCR_PPS_LEVEL (HMAC_SMALL_DSCR_TH_LEVEL - 1) /* С��trx PPS��ֵ�����С */
#define HMAC_DSCR_TH_UPDATE_AMSDU_TH   200                       /* ����amsdu tcp ack���ڸ�ֵ, �����ô������������ */
#define HMAC_LARGE_DSCR_TH_LIMIT_OTHER 42                        /* �Ƿ��ͳ����´�����������������0λ���� */
#define HMAC_LARGE_DSCR_TH_LIMIT_TX    20                        /* ���ͳ����´�����������������0λ���� */
#define HMAC_TX_PPS_RATIO              4                         /* tx pps��rx pps�������ڴ���ֵ����Ϊ��ǰΪ���ͳ��� */
/* 3 ö�ٶ��� */
/* 4 ȫ�ֱ������� */
extern oal_uint32 g_ul_large_start_th;
extern oal_uint32 g_ul_large_interval;
extern oal_uint32 g_ul_small_start_th;
extern oal_uint32 g_ul_small_interval;

/* 5 ��Ϣͷ���� */
/* 6 ��Ϣ���� */
/* 7 STRUCT���� */
/* 8 UNION���� */
/* 9 OTHERS���� */
/* 10 �������� */
extern oal_void hmac_rx_dscr_th_opt(oal_uint32 ul_tx_throughput_mbps,
                                    oal_uint32 ul_rx_throughput_mbps,
                                    oal_uint32 ul_tx_pps,
                                    oal_uint32 ul_rx_pps,
                                    oal_uint32 ul_rx_amsdu_tcp_ack_pps);
extern oal_void hmac_rx_dscr_th_init(hmac_device_stru *pst_hmac_device);
extern oal_void hmac_rx_dscr_th_deinit(hmac_device_stru *pst_hmac_device);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
