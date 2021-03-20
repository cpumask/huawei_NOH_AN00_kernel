

#ifndef __MSPC_SMX_H__
#define __MSPC_SMX_H__

#include <linux/types.h>

#define SMX_ENABLE        0x5A5AA5A5
#define SMX_DISABLE       0xA5A55A5A

int32_t mspc_get_smx_status(void);
int32_t mspc_disable_smx(void);

#endif /* __MSPC_SMX_H__ */
