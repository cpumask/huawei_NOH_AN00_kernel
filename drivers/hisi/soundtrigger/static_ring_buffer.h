/*
 * static_ring_buffer.h
 *
 * soundtrigger ring buffer interfaces
 *
 * Copyright (c) 2014-2020 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#ifndef __STATIC_RING_BUFFER_H__
#define __STATIC_RING_BUFFER_H__

#include "ring_buffer.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

struct ring_buffer *static_ring_buffer_init(void *ring_buffer_addr, int element_size, int element_cnt);
int static_ring_buffer_is_empty(void);
int static_ring_buffer_is_full(void);
int static_ring_buffer_get(void *element);
int static_ring_buffer_put(const void *element);
void static_ring_buffer_deinit(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* __STATIC_RING_BUFFER_H__ */
