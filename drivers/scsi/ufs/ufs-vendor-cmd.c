/*
 * ufs-vendor-cmd.c
 *
 * ufs device command implements for different device vendor
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include "ufs-vendor-cmd.h"

#include <scsi/scsi_device.h>
#include <scsi/ufs/ufs.h>
#include <scsi/ufs/ioctl.h>
#include <asm/unaligned.h>

#include "ufshcd.h"
#include "ufs_func.h"
#include "ufshcd-kirin-interface.h"

/*
 * entries: 4B, max value 32
 * lba(4B), len(4B),
 * ...
 * lba(4B), len(4B)
 */
#define MAX_WRITE_PGR_SIZE (4 + 32 * 8)
/* HI1861 FSR info default size, 4k byte */
#define HI1861_FSR_INFO_SIZE 4096

static int ufshcd_tz_ctrl(struct scsi_device *sdev, int desc_id, uint8_t index)
{
	int err;
	int buf_len = 0;
	struct ufs_hba *hba = NULL;

	if (!sdev || (index != 0 && index != 1))
		return -EINVAL;

#ifdef CONFIG_HISI_DEBUG_FS
	if (desc_id < TZ_RETURN_FLAG || desc_id >= TZ_DESC_MAX)
#else
	if (desc_id != TZ_RETURN_FLAG && desc_id != TZ_FORCE_CLOSE_FLAG)
#endif
		return -EINVAL;

	hba = shost_priv(sdev->host);

	if (!strstarts(hba->model, UFS_MODEL_THOR920))
		return -EPERM;

	err = ufshcd_query_vcmd_retry(hba, UPIU_QUERY_OPCODE_TZ_CTRL,
		(u8)desc_id, index, 0, NULL, &buf_len);
	return err;
}

static int ufshcd_get_tz_info(struct ufs_hba *hba, u8 *buf, u32 buf_len)
{
	int ret;
	int buf_size = buf_len;

	if (hba->tz_version < TZ_VER_1_0)
		return -EPERM;

	ret = ufshcd_query_vcmd_retry(
		hba, UPIU_QUERY_OPCODE_READ_TZ_DESC, 0, 0, 0, buf, &buf_size);
	if (ret)
		dev_err(hba->dev, "%s: Failed get turbo zone info ret %d",
			__func__, ret);

	return ret;
}

static int ufshcd_tz_get_status(struct ufs_hba *hba, u8 *buf, u32 buf_len)
{
	*buf = hba->tz_version;
	return 0;
}

static int ufshcd_tz_cap_info(struct ufs_hba *hba, u8 *buf, u32 buf_len)
{
	int ret;
	int buf_size = buf_len;
	struct tz_cap_info *cap_info = (struct tz_cap_info *)buf;

	if (hba->tz_version < TZ_VER_2_0)
		return -EINVAL;

	ret = ufshcd_query_vcmd_retry(hba, UPIU_QUERY_OPCODE_VENDOR_READ,
				      (u8)QUERY_TZ_IDN_CAP_INFO, 0, 0, buf,
				      &buf_size);
	if (ret) {
		dev_err(hba->dev, "%s: Failed get turbo zone cap info ret %d",
			__func__, ret);
		return ret;
	}

	cap_info->marked_slc_blks = be32_to_cpu(cap_info->marked_slc_blks);
	cap_info->marked_tlc_blks = be32_to_cpu(cap_info->marked_tlc_blks);
	cap_info->remain_slc_blks = be32_to_cpu(cap_info->remain_slc_blks);
	cap_info->slc_exist_status = be32_to_cpu(cap_info->slc_exist_status);

	return 0;
}

static int ufshcd_tz_blk_info(struct ufs_hba *hba, u8 lun, u8 *buf, u32 buf_len)
{
	int ret;
	struct tz_blk_info *blk_info = (struct tz_blk_info *)buf;
	uint32_t buf_size = blk_info->buf_len;
	int query_len = blk_info->len;

	if (hba->tz_version < TZ_VER_2_0 || !blk_info->flags || !query_len)
		return -EINVAL;

	/* buffer size is 5 at least, to fill addr and lun */
	if (buf_size < (sizeof(u32) + 1))
		return -EINVAL;

	put_unaligned_le32(blk_info->addr, blk_info->flags);
	blk_info->flags[sizeof(u32)] = lun;
	ret = ufshcd_query_vcmd_retry(hba, UPIU_QUERY_OPCODE_VENDOR_READ,
				      (u8)QUERY_TZ_IDN_BLK_INFO, 0, 0,
				      blk_info->flags, &query_len);
	if (ret)
		dev_err(hba->dev, "%s: Failed get turbo zone blk info ret %d",
			__func__, ret);

	return ret;
}

/* This interface is offered to f2fs */
static int ufshcd_tz_query(struct scsi_device *sdev, u32 type, u8 *buf,
			   u32 buf_len)
{
	int ret;
	struct ufs_hba *hba = NULL;

	if (!sdev || !buf || !buf_len)
		return -EFAULT;

	hba = shost_priv(sdev->host);
	if (!hba)
		return -EINVAL;

	switch (type) {
	case TZ_READ_DESC:
		ret = ufshcd_get_tz_info(hba, buf, buf_len);
		break;
	case TZ_STATUS:
		ret = ufshcd_tz_get_status(hba, buf, buf_len);
		break;
	case TZ_CAP_INFO:
		ret = ufshcd_tz_cap_info(hba, buf, buf_len);
		break;
	case TZ_BLK_INFO:
		ret = ufshcd_tz_blk_info(hba, sdev->lun, buf, buf_len);
		break;
	default:
		ret = -EINVAL;
	}

	if (ret)
		dev_err(hba->dev, "%s: Failed get turbo zone info ret %d",
			__func__, ret);

	return ret;
}

void ufshcd_tz_op_register(struct ufs_hba *hba)
{
	struct scsi_host_template *sht = hba->host->hostt;

	if (!sht)
		return;

	sht->tz_ctrl = ufshcd_tz_ctrl;
	sht->get_tz_info = ufshcd_tz_query;
}

static int ufshcd_tz_set_group_num(struct ufs_hba *hba, u8 *buf, u32 buf_len)
{
	int ret;
	int buf_size = buf_len;

	ret = ufshcd_query_vcmd_retry(hba, UPIU_QUERY_OPCODE_VENDOR_WRITE,
				      (u8)SET_TZ_STREAM_ID, 0, 0, buf,
				      &buf_size);
	if (ret)
		dev_err(hba->dev, "%s: Failed set tz stream id ret %d",
			__func__, ret);

	return ret;
}

/* Set stream id for turbo zone 2.0 */
static void ufshcd_tz_init(struct ufs_hba *hba, uint32_t feature)
{
	int ret;
	uint32_t group_num = TZ_GROUP_NUM;

	hba->tz_version = TZ_INVALID;
	if (hba->manufacturer_id != UFS_VENDOR_HI1861)
		return;
	if (strstarts(hba->model, UFS_MODEL_THOR920)) {
		hba->tz_version = TZ_VER_1_0;
		return;
	}

	if (feature & VENDOR_FEATURE_TURBO_ZONE) {
		ret = ufshcd_tz_set_group_num(hba, (u8 *)&group_num,
					      sizeof(group_num));
		if (ret)
			return;
		hba->tz_version = TZ_VER_2_0;
		dev_info(hba->dev, "support turbo zone 2.0\n");
	}
}

static void ufshcd_order_preserving_init(struct ufs_hba *hba, uint32_t feature)
{
#ifdef CONFIG_HISI_BLOCK_ORDER_PRESERVING
	hba->host->order_enabled = 0;
	if (hba->manufacturer_id != UFS_VENDOR_HI1861)
		return;

	if (feature & VENDOR_FEATURE_ORDER) {
		hba->host->order_enabled = 1;
		dev_info(hba->dev, "support order feature\n");
	}
#endif
}

/* Parse 186x feature info, device descriptor offset 0xF0 */
static void ufshcd_vendor_feature_init(struct ufs_hba *hba,
				       struct ufs_dev_desc *dev_desc)
{
	ufshcd_tz_init(hba, dev_desc->vendor_feature);
	ufshcd_order_preserving_init(hba, dev_desc->vendor_feature);
}

void ufshcd_get_vendor_info(struct ufs_hba *hba, struct ufs_dev_desc *dev_desc)
{
	ufshcd_vendor_feature_init(hba, dev_desc);
}

static int
ufshcd_ioctl_query_vendor_write(struct ufs_hba *hba,
				struct ufs_ioctl_query_data *ioctl_data,
				const void __user *buffer)
{
	int err;
	int index = 0;
	u8 *desc_buf = NULL;
	u8 idn = ioctl_data->idn;
	u32 opcode = ioctl_data->opcode;
	int buf_size = ioctl_data->buf_size;

	if (buf_size <= 0)
		return -EINVAL;

	switch (idn) {
	case QUERY_VENDOR_WRITE_IDN_PGR:
		if (buf_size > MAX_WRITE_PGR_SIZE)
			buf_size = MAX_WRITE_PGR_SIZE;
		break;
	default:
		return -EINVAL;
	}

	desc_buf = kzalloc(buf_size, GFP_KERNEL);
	if (!desc_buf) {
		dev_err(hba->dev, "%s: alloc fail\n", __func__);
		return -ENOMEM;
	}

	err = copy_from_user(desc_buf, buffer + UFS_IOCTL_QUERY_DATA_SIZE,
			     buf_size);
	if (err) {
		dev_err(hba->dev, "%s: copy from user, err %d\n", __func__,
			err);
		goto out;
	}

	err = ufshcd_query_vcmd_retry(hba, opcode, idn, index, 0, desc_buf,
				      &buf_size);
	if (err)
		dev_err(hba->dev, "%s:  ret %d", __func__, err);
out:
	kfree(desc_buf);
	return err;
}

static int ufshcd_ioctl_query_tz_ctrl(struct ufs_hba *hba,
				      struct ufs_ioctl_query_data *ioctl_data,
				      const void __user *buffer)
{
	int err;
	u8 index = 0;
	u8 idn = ioctl_data->idn;
	u32 opcode = ioctl_data->opcode;
	int buf_size = 0;

	if (!strstarts(hba->model, UFS_MODEL_THOR920))
		return -EPERM;

	err = copy_from_user(&index, buffer + UFS_IOCTL_QUERY_DATA_SIZE,
			     sizeof(index));
	if (err) {
		dev_err(hba->dev, "%s: copy from user, err %d\n", __func__,
			err);
		return err;
	}

	err = ufshcd_query_vcmd_retry(hba, opcode, idn, index, 0, NULL,
				      &buf_size);
	if (err)
		dev_err(hba->dev, "%s:  ret %d", __func__, err);

	return err;
}

static int
ufshcd_ioctl_query_read_tz_desc(struct ufs_hba *hba,
				struct ufs_ioctl_query_data *ioctl_data,
				void __user *buffer)
{
	int err;
	u8 index = 0;
	u8 *desc_buf = NULL;
	u8 idn = ioctl_data->idn;
	u32 opcode = ioctl_data->opcode;
	int fsr_len = HI1861_FSR_INFO_SIZE;

	desc_buf = kzalloc(fsr_len, GFP_KERNEL);
	if (!desc_buf) {
		dev_err(hba->dev, "%s: alloc fail\n", __func__);
		return -ENOMEM;
	}

	err = ufshcd_query_vcmd_retry(hba, opcode, idn, index, 0, desc_buf,
				      &fsr_len);
	if (err) {
		dev_err(hba->dev, "%s:  ret %d", __func__, err);
		goto out;
	}

	ioctl_data->buf_size = (u16)(fsr_len < (int)HI1861_FSR_INFO_SIZE ?
					     fsr_len : HI1861_FSR_INFO_SIZE);
	err = copy_to_user(buffer + sizeof(struct ufs_ioctl_query_data),
			   desc_buf, ioctl_data->buf_size);
	if (err)
		dev_err(hba->dev, "%s: copy back to user err %d\n", __func__,
			err);
out:
	kfree(desc_buf);
	return err;
}

int ufshcd_ioctl_query_vcmd(struct ufs_hba *hba,
			    struct ufs_ioctl_query_data *ioctl_data,
			    void __user *buffer)
{
	int err;

	if (hba->manufacturer_id != UFS_VENDOR_HI1861)
		return -EINVAL;

	switch (ioctl_data->opcode) {
	case UPIU_QUERY_OPCODE_VENDOR_WRITE:
		err = ufshcd_ioctl_query_vendor_write(hba, ioctl_data, buffer);
		break;
	case UPIU_QUERY_OPCODE_TZ_CTRL:
		err = ufshcd_ioctl_query_tz_ctrl(hba, ioctl_data, buffer);
		break;
	case UPIU_QUERY_OPCODE_READ_TZ_DESC:
	case UPIU_QUERY_OPCODE_READ_HI1861_FSR:
		err = ufshcd_ioctl_query_read_tz_desc(hba, ioctl_data, buffer);
		break;
	default:
		err = -ENOIOCTLCMD;
		break;
	}

	return err;
}
