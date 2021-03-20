/*
 * ring_buffer.c
 *
 * soundtrigger ring_buffer implement
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

#include <linux/slab.h>
#include "ring_buffer.h"
#include "soundtrigger_log.h"

struct ring_buffer {
	unsigned int size;
	unsigned int start;
	unsigned int count;
	unsigned int element_size;
	unsigned int reserved;
	char buffer[0];
};

struct ring_buffer *st_ring_buffer_init(void *ring_buffer_addr, int element_size, int element_cnt)
{
	struct ring_buffer *rb = ring_buffer_addr;

	if (rb == NULL)
		return NULL;

	memset(rb, 0, sizeof(struct ring_buffer) + element_size * element_cnt);

	rb->element_size = element_size;
	rb->size = element_cnt;
	rb->start = 0;
	rb->count = 0;

	return rb;
}

bool st_ring_buffer_is_empty(const struct ring_buffer *rb)
{
	return (rb->count == 0);
}

bool st_ring_buffer_is_full(const struct ring_buffer *rb)
{
	return (rb->count == rb->size);
}

int st_ring_buffer_get(struct ring_buffer *rb, void *element)
{
	int ret = 0;

	if (!st_ring_buffer_is_empty(rb)) {
		memcpy(element, rb->buffer + rb->start * rb->element_size, rb->element_size);/*lint !e679*/
		rb->start = (rb->start + 1) % rb->size;
		--rb->count;
		ret = rb->element_size;
	}

	return ret;
}

int st_ring_buffer_put(struct ring_buffer *rb, const void *element)
{
	int ret;
	int end = (rb->start + rb->count) % rb->size;

	memcpy(rb->buffer + end * rb->element_size, element, rb->element_size);/*lint !e679*/

	if (st_ring_buffer_is_full(rb)) {
		rb->start = (rb->start + 1) % rb->size; /* full, overwrite */
		ret = RING_BUFFER_OVERWRITE;
	} else {
		++rb->count;
		ret = RING_BUFFER_NORMAL;
	}

	return ret;
}

void st_ring_buffer_deinit(struct ring_buffer *rb)
{
	if (rb == NULL)
		return;

	memset(rb, 0, sizeof(struct ring_buffer) + rb->element_size * rb->size);
	rb = NULL;
}
