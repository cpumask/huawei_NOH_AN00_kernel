/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: s4 feature
 * Author: Dizhe Mao maodizhe1@huawei.com
 * Create: 2020-06-09
 */
#ifndef TEE_S4_H
#define TEE_S4_H
#include <linux/platform_device.h>

int tc_s4_pm_suspend(struct device *dev);
int tc_s4_pm_resume(struct device *dev);
#endif
