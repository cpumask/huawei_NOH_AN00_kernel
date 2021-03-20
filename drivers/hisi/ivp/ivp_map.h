/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: this file define fd maps
 * Author: donghaixu
 * Create: 2020-05-25
 */

#ifndef _IVP_MAP_H_
#define _IVP_MAP_H_

#include <linux/device.h>
#include "ivp.h"

int ivp_map_hidl_fd(struct device *dev, struct ivp_map_info *map_buf);
int ivp_unmap_hidl_fd(struct device *dev,
		struct ivp_map_info *map_buf);
#ifdef SEC_IVP_V300
int ivp_add_dma_ref(int fd);
int ivp_init_map_info(void);
void ivp_clear_map_info(void);
int ivp_init_sec_fd(struct ivp_fd_info *info);
int ivp_deinit_sec_fd(struct ivp_fd_info *info);
#endif

#endif /* _IVP_MAP_H_ */
