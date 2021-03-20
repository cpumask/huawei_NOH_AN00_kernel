/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: airpress channel header file
 * Author: linjianpeng <linjianpeng1@huawei.com>
 * Create: 2020-05-25
 */

#ifndef __AIRPRESS_CHANNEL_H__
#define __AIRPRESS_CHANNEL_H__

#include "airpress_detect.h"
#include "airpress_sysfs.h"
#include "airpress_route.h"

int send_airpress_calibrate_data_to_mcu(void);
void reset_airpress_calibrate_data(void);

#endif
