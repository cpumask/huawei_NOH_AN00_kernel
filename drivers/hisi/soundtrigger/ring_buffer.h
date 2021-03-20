/*
 * ring_buffer.c
 *
 * soundtrigger ring buffer implement
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

#ifndef __RING_BUFFER_H__
#define __RING_BUFFER_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include <stdbool.h>

enum ring_buffer_ret_code {
	RING_BUFFER_NORMAL,
	RING_BUFFER_BAD_PARAM,
	RING_BUFFER_ALLOCATE_FAIL,
	RING_BUFFER_EMPTY,
	RING_BUFFER_OVERWRITE,
	RING_BUFFER_NO_SKIP
};

struct ring_buffer *st_ring_buffer_init(void *ring_buffer_addr, int element_size, int element_cnt);
bool st_ring_buffer_is_empty(const struct ring_buffer *rb);
bool st_ring_buffer_is_full(const struct ring_buffer *rb);
int st_ring_buffer_get(struct ring_buffer *rb, void *element);
int st_ring_buffer_put(struct ring_buffer *rb, const void *element);
void st_ring_buffer_deinit(struct ring_buffer *rb);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* __RING_BUFFER_H__ */
