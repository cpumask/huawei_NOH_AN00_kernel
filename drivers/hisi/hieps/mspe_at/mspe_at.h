/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: msp engine at command common interface.
 * Author : kirin sec engine plat2
 * Create: 2020/4/9
 */
#ifndef __MSPE_AT_H__
#define __MSPE_AT_H__
#include <linux/types.h>
#include <linux/device.h>

ssize_t mspe_at_ctrl_store(struct device *dev, struct device_attribute *attr,
			   const char *buf, size_t count);

ssize_t mspe_at_ctrl_show(struct device *dev, struct device_attribute *attr, char *buf);

#endif /* __MSPE_AT_H__ */
