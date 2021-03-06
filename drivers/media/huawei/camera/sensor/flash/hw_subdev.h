/*
 * hw_subdev.h
 *
 * Copyright (c) 2011-2020 Huawei Technologies Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */


#ifndef _HW_SUBDEV_H_
#define _HW_SUBDEV_H_

#include <media/v4l2-subdev.h>

struct hw_sd_subdev {
	struct v4l2_subdev sd;
	int close_seq;
};

#endif /* _HW_SUBDEV_H_ */
