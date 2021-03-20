/*
 * static_ring_buffer.c
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

#include "static_ring_buffer.h"

static struct ring_buffer *g_rb;

struct ring_buffer *static_ring_buffer_init(void *ring_buffer_addr, int element_size, int element_cnt)
{
	g_rb = st_ring_buffer_init(ring_buffer_addr, element_size, element_cnt);

	return g_rb;
}

int static_ring_buffer_is_empty(void)
{
	return st_ring_buffer_is_empty(g_rb);
}

int static_ring_buffer_is_full(void)
{
	return st_ring_buffer_is_full(g_rb);
}

int static_ring_buffer_get(void *element)
{
	return st_ring_buffer_get(g_rb, element);
}

int static_ring_buffer_put(const void *element)
{
	return st_ring_buffer_put(g_rb, element);
}

void static_ring_buffer_deinit(void)
{
	st_ring_buffer_deinit(g_rb);
}
