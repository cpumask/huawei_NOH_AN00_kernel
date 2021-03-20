/*
 * gp_op.h
 *
 * function declaration for alloc global operation and pass params to TEE.
 *
 * Copyright (c) 2012-2019 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#ifndef _GP_OPS_H_
#define _GP_OPS_H_
#include "tc_ns_client.h"
#include "teek_ns_client.h"

struct tc_call_params {
	struct tc_ns_dev_file *dev_file;
	struct tc_ns_client_context *client_context;
	struct tc_ns_session *session;
	struct tc_ns_temp_buf *local_temp_buffer;
	unsigned int tmp_buf_size;
};

int write_to_client(void __user *dest, size_t dest_size,
	const void *src, size_t size, uint8_t kernel_api);
int read_from_client(void *dest, size_t dest_size,
	const void __user *src, size_t size, uint8_t kernel_api);
int tc_user_param_valid(struct tc_ns_client_context *client_context, unsigned int index);
int tc_client_call(struct tc_ns_client_context *client_context,
	struct tc_ns_dev_file *dev_file, struct tc_ns_session *session, uint8_t flags);

#endif
