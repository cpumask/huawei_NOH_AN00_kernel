/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: ps route header file
 * Author: linjianpeng <linjianpeng1@huawei.com>
 * Create: 2020-05-25
 */

#ifndef __PS_ROUTE_H__
#define __PS_ROUTE_H__

void ps_telecall_status_change(unsigned long value);
int send_ps_lcd_freq_to_mcu(int tag, uint32_t subcmd, const void *data,
	int length, bool is_recovery);
void recv_lcd_freq_process(struct work_struct *ps_work);
void ps_lcd_freq_queue_work(uint32_t lcd_freq);
void send_lcd_freq_to_sensorhub(uint32_t lcd_freq);
ssize_t show_ps_sensorlist_info(struct device *dev,
	struct device_attribute *attr, char *buf);
ssize_t show_ps_offset_data(struct device *dev,
	struct device_attribute *attr, char *buf);
ssize_t attr_ps_calibrate_after_sale_show(struct device *dev,
	struct device_attribute *attr, char *buf);
int sleeve_test_ps_prepare(int ps_config);
int ps_ud_workqueue_init(void);

#endif
