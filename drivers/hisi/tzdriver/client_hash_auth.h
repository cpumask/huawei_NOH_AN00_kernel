/*
 * client_hash_auth.h
 *
 * function definition for CA code hash auth
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

#ifndef _CLIENT_HASH_CALC_H_
#define _CLIENT_HASH_CALC_H_

#include "tc_ns_client.h"
#include "teek_ns_client.h"

#ifdef CONFIG_CLIENT_AUTH
#include "auth_base_impl.h"

int calc_client_auth_hash(struct tc_ns_dev_file *dev_file,
	struct tc_ns_client_context *context, struct tc_ns_session *session);
#else

static inline int calc_client_auth_hash(struct tc_ns_dev_file *dev_file,
	struct tc_ns_client_context *context, struct tc_ns_session *session)
{
	return 0;
}

#endif

#endif
