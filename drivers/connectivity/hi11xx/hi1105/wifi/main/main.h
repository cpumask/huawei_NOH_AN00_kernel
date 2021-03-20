

#ifndef __MAIN_H__
#define __MAIN_H__

// 此处不加extern "C" UT编译不过
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include "oal_ext_if.h"
#include "oam_ext_if.h"
#include "frw_ext_if.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_MAIN_H

extern oal_completion g_wlan_cali_completed;
int32_t hi110x_host_main_init(void);
void hi110x_host_main_exit(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
