#ifndef _TAF_DSP_PHY_
#define _TAF_DSP_PHY_

#ifdef MODEM_FUSION_VERSION
#include "v_basic_type_def.h"
#ifdef LPS_RTT
/* �����ʹ����Ϣͷ�����ͷ�ļ����� */
#include "mdrv_msg.h"
#ifndef VOS_MSG_HEADER
#define VOS_MSG_HEADER MDRV_MSG_HEAD
#endif
#endif /* End def LPS_RTT */
#else
#include "vos.h"
#endif

#endif
