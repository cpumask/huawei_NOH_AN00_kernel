/*
 * wireless_coil_test.h
 *
 * common interface, varibles, definition etc for wireless hardware test
 *
 * Copyright (c) 2020-2020 Huawei Technologies Co., Ltd.
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

#ifndef _WIRELESS_COIL_TEST_H_
#define _WIRELESS_COIL_TEST_H_

#define WIRELESS_COIL_TEST_SUCC               0
#define WIRELESS_COIL_TEST_FAIL               1
#define WIRELESS_COIL_TEST_RUNNING            2
#define WIRELESS_COIL_TEST_IIN_MIN            100

struct wireless_coil_dev {
	struct work_struct chk_coil_work;
	int test_result;
	bool test_busy;
};

void wireless_coil_test_start(void);
int wirless_coil_test_result(void);

#endif /* _WIRELESS_COIL_TEST_H_ */
