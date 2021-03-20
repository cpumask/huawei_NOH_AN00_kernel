
#ifndef __ALG_EXT_IF_H__
#define __ALG_EXT_IF_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "oal_types.h"
#include "hal_ext_if.h"
#include "dmac_ext_if.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_ALG_EXT_IF_H
extern oal_int32 alg_main_init(oal_void);
extern oal_void alg_main_exit(oal_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of alg_ext_if.h */
