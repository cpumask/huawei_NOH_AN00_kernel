/*
 * tc_client_sub_driver.h
 *
 * function declaration for preparing and organizing data for tc_client_driver
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
#ifndef _TC_NS_CLIENT_DRIVER_SUB_H_
#define _TC_NS_CLIENT_DRIVER_SUB_H_
#include <linux/highmem.h>
#include <linux/mm.h>
#include <linux/random.h>
#include <securec.h>
#include "teek_ns_client.h"
#include "tc_ns_client.h"

struct tc_ns_service *tc_find_service_in_dev(struct tc_ns_dev_file *dev,
	const unsigned char *uuid, int uuid_size);
struct tc_ns_service *tc_ref_service_in_dev(struct tc_ns_dev_file *dev, const unsigned char *uuid,
	int uuid_size, bool *is_full);
int add_service_to_dev(struct tc_ns_dev_file *dev, struct tc_ns_service *service);
void del_service_from_dev(struct tc_ns_dev_file *dev, struct tc_ns_service *service);
struct tc_ns_session *tc_find_session_withowner(struct list_head *session_list,
				  unsigned int session_id, struct tc_ns_dev_file *dev_file);
int close_session(struct tc_ns_dev_file *dev, struct tc_ns_session *session, const unsigned char *uuid,
	unsigned int uuid_len, unsigned int session_id);
uint32_t tc_ns_get_uid(void);
int get_pack_name_len(struct tc_ns_dev_file *dev_file, const uint8_t *cert_buffer,
	unsigned int cert_buffer_size);
int get_public_key_len(struct tc_ns_dev_file *dev_file, const uint8_t *cert_buffer,
	unsigned int cert_buffer_size);
bool is_valid_ta_size(const char *file_buffer, unsigned int file_size);
int tc_ns_need_load_image(unsigned int file_id, const unsigned char *uuid,
	unsigned int uuid_len);
int load_ta_image(struct tc_ns_dev_file *dev_file, struct tc_ns_client_context *context);
void release_free_session(struct tc_ns_dev_file *dev_file, struct tc_ns_client_context *context, struct tc_ns_session *session);
void close_session_in_service_list(struct tc_ns_dev_file *dev, struct tc_ns_service *service, uint32_t i);
void close_unclosed_session(struct tc_ns_dev_file *dev, uint32_t i);
int tc_ns_load_secfile(struct tc_ns_dev_file *dev_file, const void __user *argp);
#endif
