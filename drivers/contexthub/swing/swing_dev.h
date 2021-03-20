/*
 * Copyright (C) Huawei Tech. Co. Ltd. 2017-2019. All rights reserved.
 * Description: dev drvier to communicate with sensorhub swing app
 * Author: Huawei
 * Create: 2017.12.05
 */

#ifndef __LINUX_CONTEXTHUB_SWING_H__
#define __LINUX_CONTEXTHUB_SWING_H__
#include <linux/types.h>

/* ioctl cmd define */
#define SWING_IO                         0xB1

#define SWING_IOCTL_SWING_OPEN         _IOW(SWING_IO, 0xD1, short)
#define SWING_IOCTL_SWING_CLOSE        _IOW(SWING_IO, 0xD2, short)
#define SWING_IOCTL_FUSION_EN          _IOW(SWING_IO, 0xD3, short)
#define SWING_IOCTL_FUSION_SET         _IOW(SWING_IO, 0xD4, short)
#ifdef CONFIG_CONTEXTHUB_SWING_20
#define SWING_IOCTL_FUSION_GETS        _IOW(SWING_IO, 0xD5, short)
#define SWING_IOCTL_FUSION_SETS        _IOW(SWING_IO, 0xD6, short)
#endif
#define SWING_IOCTL_ION_REF_ATTACH     _IOW(SWING_IO, 0xD7, short)
#define SWING_IOCTL_ION_REF_DETACH     _IOW(SWING_IO, 0xD8, short)

#define IOMCU_SYSTEMCACHE_HINT         8

typedef struct {
	struct dma_buf *buf;
	u64 id;
	int fd;
} swing_ion_info_t;

typedef struct {
	u32 fusion_id;
	u32 en;
} swing_fusion_en_t;

typedef struct {
	u32 ret_code;
	u32 fusion_id;
} swing_fusion_en_resp_t;

typedef struct {
	swing_fusion_en_t en;
	swing_fusion_en_resp_t en_resp;
} swing_fusion_en_param_t;

typedef struct {
	u32 fusion_id;
	u32 data_type;
	u32 data_addr;
	u32 data_len;
} swing_fusion_set_t;

typedef struct {
	u32 ret_code;
	u32 fusion_id;
} swing_fusion_set_resp_t;

typedef struct {
	swing_fusion_set_t set;
	swing_fusion_set_resp_t set_resp;
} swing_fusion_set_param_t;

typedef struct {
	u32 fusion_id;
	u32 notify_type;
	u32 notify_len;
	u32 reserved;
} swing_upload_t;

#ifdef CONFIG_CONTEXTHUB_SWING_20
typedef struct {
	u32 fusion_id;
	u32 data_type;
	u32 sub_type;
	u32 data_len;
	u32 context;
	u32 reserved;
	u64 data_addr;
} swing_fusion_gets_t;

typedef struct {
	u32 ret_code;
	u32 fusion_id;
	u32 context;
	u32 data_type;
	u32 sub_type;
	u32 data_len;
	u64 data_addr;
} swing_fusion_gets_resp_t;

typedef struct {
	swing_fusion_gets_t gets_param;
	swing_fusion_gets_resp_t gets_resp;
} swing_fusion_gets_param_t;

typedef struct {
	u32 fusion_id;
	u32 data_type;
	u32 data_len;
	u32 reserved;
	u64 data_addr;
} swing_fusion_sets_t;

typedef struct {
	u32 ret_code;
	u32 fusion_id;
	u32 data_type;
	u32 data_len;
} swing_fusion_sets_resp_t;

typedef struct {
	swing_fusion_sets_t sets;
	swing_fusion_sets_resp_t sets_resp;
} swing_fusion_sets_param_t;
#endif

#endif
