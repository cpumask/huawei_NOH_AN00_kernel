
#ifndef __MSPC_AT_H__
#define __MSPC_AT_H__
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/device.h>

/*
 * @brief: MSPC at ctrl device node store function.
 * @param[in]: dev - device node.
 * @param[in]: attr - device attribute.
 * @param[in]: buf - MSPC at ctrl input buffer, which save request content.
 * @param[in]: count - MSPC at ctrl input buffer length.
 * @return: if success return count, others are failure.
 */
ssize_t mspc_at_ctrl_store(struct device *dev, struct device_attribute *attr,
			   const char *buf, size_t count);

/*
 * @brief: MSPC at ctrl device node show function.
 * @param[in]: dev - device node.
 * @param[in]: attr - device attribute.
 * @param[out]: buf - MSPC at ctrl output buffer which save response content.
 * @return: the data length written to the buffer.
 */
ssize_t mspc_at_ctrl_show(struct device *dev, struct device_attribute *attr, char *buf);

#endif /* __MSPC_AT_H__ */