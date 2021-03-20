/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2019. All rights reserved.
 * Description: vcodec utils
 * Author:lijinwang     lijinwang@huawei.com
 * Create: 2019-12-13
 */

#ifndef TVP_ADAPTER_H
#define TVP_ADAPTER_H

#include "hi_type.h"
#include "drv_mem.h"

enum {
	HIVCODEC_CMD_ID_INIT = 1,
	HIVCODEC_CMD_ID_EXIT,
	HIVCODEC_CMD_ID_SUSPEND,
	HIVCODEC_CMD_ID_RESUME,
	HIVCODEC_CMD_ID_CONTROL,
	HIVCODEC_CMD_ID_RUN_PROCESS,
	HIVCODEC_CMD_ID_GET_IMAGE,
	HIVCODEC_CMD_ID_RELEASE_IMAGE,
	HIVCODEC_CMD_ID_CONFIG_INPUT_BUFFER,
	HIVCODEC_CMD_ID_READ_PROC,
	HIVCODEC_CMD_ID_WRITE_PROC,
	HIVCODEC_CMD_ID_MEM_CPY = 20,
	HIVCODEC_CMD_ID_CFG_MASTER,
};

enum sec_venc_state {
	SEC_VENC_OFF,
	SEC_VENC_ON,
};

HI_S32 init_kernel_ca(void);
HI_VOID deinit_kernel_ca(void);
HI_VOID config_master(enum sec_venc_state state, uint32_t core_id);

#endif
/** TVP_ADAPTER_H */
