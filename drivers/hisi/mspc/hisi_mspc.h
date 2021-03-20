

#ifndef __HISI_MSPC_H__
#define __HISI_MSPC_H__

#include <linux/types.h>

#define MSPC_MODULE_UNREADY             0xC8723B6D
#define MSPC_MODULE_READY               0x378DC492

int32_t mspc_get_init_status(void);

#endif /*  __HISI_MSPC_H__ */
