/* Copyright (c) 2013-2020, Hisilicon Tech. Co., Ltd. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#include "hisi_fb.h"
#include "hisi_fb_sysfs.h"


/*******************************************************************************
 * fb sys fs
 */
void hisifb_sysfs_init(struct hisi_fb_data_type *hisifd)
{
	int i;

	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is NULL\n");

	hisifd->sysfs_index = 0;
	for (i = 0; i < HISI_FB_SYSFS_ATTRS_NUM; i++)
		hisifd->sysfs_attrs[i] = NULL;

	hisifd->sysfs_attr_group.attrs = hisifd->sysfs_attrs;
}

void hisifb_sysfs_attrs_append(struct hisi_fb_data_type *hisifd, struct attribute *attr)
{
	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is NULL\n");

	hisi_check_and_no_retval(!attr, ERR, "attr is NULL\n");

	if (hisifd->sysfs_index >= HISI_FB_SYSFS_ATTRS_NUM) {
		HISI_FB_ERR("fb%d, sysfs_atts_num %d is out of range %d!\n",
			hisifd->index, hisifd->sysfs_index, HISI_FB_SYSFS_ATTRS_NUM);
		return;
	}

	hisifd->sysfs_attrs[hisifd->sysfs_index] = attr;
	hisifd->sysfs_index++;
}

int hisifb_sysfs_create(struct platform_device *pdev)
{
	int ret;
	struct hisi_fb_data_type *hisifd = NULL;

	hisi_check_and_return(!pdev, -EINVAL, ERR, "pdev is NULL\n");

	hisifd = platform_get_drvdata(pdev);
	hisi_check_and_return(!hisifd, -EINVAL, ERR, "hisifd is NULL\n");

	ret = sysfs_create_group(&hisifd->fbi->dev->kobj, &(hisifd->sysfs_attr_group));
	if (ret)
		HISI_FB_ERR("fb%d sysfs group creation failed, error=%d!\n",
			hisifd->index, ret);


	return ret;
}

void hisifb_sysfs_remove(struct platform_device *pdev)
{
	struct hisi_fb_data_type *hisifd = NULL;

	hisi_check_and_no_retval(!pdev, ERR, "pdev is NULL\n");

	hisifd = platform_get_drvdata(pdev);
	hisi_check_and_no_retval(!hisifd, ERR, "hisifd is NULL\n");

	sysfs_remove_group(&hisifd->fbi->dev->kobj, &(hisifd->sysfs_attr_group));


	hisifb_sysfs_init(hisifd);
}
