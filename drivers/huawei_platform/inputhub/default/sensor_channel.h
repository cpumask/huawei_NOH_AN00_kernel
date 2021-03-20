/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: sensor channel header file
 * Author: linjianpeng <linjianpeng1@huawei.com>
 * Create: 2020-05-25
 */

#ifndef __SENSOR_CHANNEL_H__
#define __SENSOR_CHANNEL_H__

extern char sensor_chip_info[SENSOR_MAX][MAX_CHIP_INFO_LEN];
extern t_ap_sensor_ops_record all_ap_sensor_operations[TAG_SENSOR_END];
extern int flag_for_sensor_test;

extern int send_vibrator_calibrate_data_to_mcu(void);
extern int send_tof_calibrate_data_to_mcu(void);

#endif
