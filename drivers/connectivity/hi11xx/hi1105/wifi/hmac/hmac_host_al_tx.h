

#ifndef __HMAC_HOST_AL_TX_H__
#define __HMAC_HOST_AL_TX_H__

/* 1 ����ͷ�ļ����� */
#include "mac_common.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_DRIVER_HMAC_TX_MSDU_DSCR_H

extern hmac_msdu_info_ring_stru g_always_tx_ring;
oal_uint32 hmac_always_tx_proc(uint8_t *param);


#endif
