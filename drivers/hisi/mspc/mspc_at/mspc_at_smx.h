
#ifndef __MSPC_AT_SMX_H__
#define __MSPC_AT_SMX_H__

#include <linux/types.h>
#include <linux/kernel.h>
#include <mspc_errno.h>

/*
 * @brief: Set MSPC smx enable and disable.
 * @param[in]: data - smx setting parameter, the parameter indicates whether to enable or disable SMX.
 * @param[in]: resp - MSPC at response callback function, not currently used.
 * @return: if success return MSPC_OK, others are failure.
 */
int32_t mspc_at_set_smx(const char *data, int32_t (*resp)(const char *buf, size_t len));

/*
 * @brief: Get MSPC smx status.
 * @param[in]: data - not currently used.
 * @param[in]: resp - MSPC at response callback function.
 * @return: if success return MSPC_OK, others are failure.
 */
int32_t mspc_at_get_smx(const char *data, int32_t (*resp)(const char *buf, size_t len));

#endif /* __MSPC_AT_SMX_H__ */
