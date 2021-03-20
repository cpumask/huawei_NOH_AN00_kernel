/*
 * coul_test.h
 *
 * test for coul driver
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

#ifndef _COUL_TEST_H_
#define _COUL_TEST_H_

enum coul_test_flag {
	COUL_TEST_BAT_EXIST = 0x0001,
	COUL_TEST_BAT_CAPACITY = 0x0002,
};

struct coul_test_info {
	unsigned long flag;
	int bat_exist;
	int bat_capacity;
};

struct coul_test_dev {
	struct coul_test_info info;
};

#if (defined(CONFIG_HUAWEI_POWER_DEBUG) && defined(CONFIG_HUAWEI_COUL))
struct coul_test_info *coul_test_get_info_data(void);
#else
static inline struct coul_test_info *coul_test_get_info_data(void)
{
	return NULL;
}
#endif /* CONFIG_HUAWEI_POWER_DEBUG && CONFIG_HUAWEI_COUL */

#endif /* _COUL_TEST_H_ */
