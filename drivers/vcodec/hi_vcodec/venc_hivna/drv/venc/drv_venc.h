
#ifndef __DRV_VENC_H__
#define __DRV_VENC_H__

#include "hi_type.h"

#ifdef SUPPORT_VENC_FREQ_CHANGE
extern HI_U32 g_venc_freq;
extern struct mutex g_venc_freq_mutex;
#endif

void venc_init_ops(void);

#endif //__DRV_VENC_H__

