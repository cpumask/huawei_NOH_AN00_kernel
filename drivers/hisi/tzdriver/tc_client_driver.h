/*
 * tc_client_driver.h
 *
 * function declaration for proc open,close session and invoke
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
#ifndef _TC_NS_CLIENT_DRIVER_H_
#define _TC_NS_CLIENT_DRIVER_H_

#include <linux/list.h>
#include "teek_ns_client.h"

struct tc_ns_dev_list *get_dev_list(void);
void get_service_struct(struct tc_ns_service *service);
void put_service_struct(struct tc_ns_service *service);
void dump_services_status(const char *param);
int tc_ns_load_image(struct tc_ns_dev_file *dev_file, char *file_buffer, unsigned int file_size);
struct tc_ns_session *tc_find_session_by_uuid(unsigned int dev_file_id,
	const struct tc_ns_smc_cmd *cmd);
void get_session_struct(struct tc_ns_session *session);
void put_session_struct(struct tc_ns_session *session);

#ifdef CONFIG_ACPI
int get_acpi_tz_irq(void);
#endif

#endif
