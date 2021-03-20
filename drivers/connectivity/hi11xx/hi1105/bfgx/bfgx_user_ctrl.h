

#ifndef __BFGX_USER_CTRL_H__
#define __BFGX_USER_CTRL_H__

/* ����ͷ�ļ����� */
#include "plat_type.h"
#include "hisi_customize_wifi.h"

/* �궨�� */
#define DEV_SW_STR_BFGX     "@DEV_SW_VERSION_BFGX:"
#define DEV_SW_STR_WIFI     "@DEV_SW_VERSION_WIFI:"

#define userctl_kfree(p)           \
    do {                           \
        if (likely((p) != NULL)) { \
            kfree(p);              \
            (p) = NULL;            \
        }                          \
    } while (0)

enum PLAT_LOGLEVLE {
    PLAT_LOG_ALERT = 0,
    PLAT_LOG_ERR = 1,
    PLAT_LOG_WARNING = 2,
    PLAT_LOG_INFO = 3,
    PLAT_LOG_DEBUG = 4,
};

enum BUG_ON_CTRL {
    BUG_ON_DISABLE = 0,
    BUG_ON_ENABLE = 1,
};

extern int32 g_hi110x_loglevel;

/* EXTERN FUNCTION */
extern int32 bfgx_user_ctrl_init(void);
extern void bfgx_user_ctrl_exit(void);
#endif
