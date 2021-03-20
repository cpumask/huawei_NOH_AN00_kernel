/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: ps channel header file
 * Author: linjianpeng <linjianpeng1@huawei.com>
 * Create: 2020-05-25
 */

#ifndef __PS_CHANNEL_H__
#define __PS_CHANNEL_H__

#include "ps_detect.h"
#include "ps_sysfs.h"
#include "ps_route.h"

int send_ps_calibrate_data_to_mcu(void);
void reset_ps_calibrate_data(void);

#endif
