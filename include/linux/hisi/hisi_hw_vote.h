/*
 * hisi_hw_vote.h
 *
 * hisi hwvote share
 *
 * Copyright (c) 2017-2020 Huawei Technologies Co., Ltd.
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
#ifndef __HISI_HW_VOTE_H__
#define __HISI_HW_VOTE_H__

struct hv_reg_cfg {
	void __iomem *reg;
	u32 rd_mask;
	u32 wr_mask;
};

struct hv_channel {
	const char *name;
	void __iomem *base;
	u32 ratio;
	struct hv_reg_cfg result;
	u32 hvdev_num;
	struct hvdev *hvdev_head;
};

struct hvdev {
	const char *name;
	struct hv_channel *parent;
	struct hv_reg_cfg vote;
	struct device *dev;
	u32 last_set;
};

extern struct hvdev *hisi_hvdev_register(struct device *dev,
					 const char *ch_name,
					 const char *vsrc);
extern int hisi_hvdev_remove(struct hvdev *hvdev);
extern int hisi_hv_set_freq(struct hvdev *hvdev, unsigned int freq_khz);
extern int hisi_hv_get_freq(struct hvdev *hvdev, unsigned int *freq_khz);
extern int hisi_hv_get_result(struct hvdev *hvdev, unsigned int *freq_khz);
extern int hisi_hv_get_last(struct hvdev *hvdev, unsigned int *freq_khz);

#endif /* __HISI_HW_VOTE_H__ */
