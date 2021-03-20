

#ifndef __PLAT_MAIN_H__
#define __PLAT_MAIN_H__

/* ����ͷ�ļ����� */
#include "oal_ext_if.h"
#include "oam_ext_if.h"
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include "sdt_drv.h"
#endif
#include "frw_ext_if.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_PLAT_MAIN_H

/* �������� */
extern oal_int32 plat_init(oal_void);
extern oal_void plat_exit(oal_void);

#endif
