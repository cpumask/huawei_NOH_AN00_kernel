/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2015-2019. All rights reserved.
 * Description: recovery misc
 * Author: jimingquan
 * Create: 2015-05-19
 */

#include <linux/module.h>
#include <dsm/dsm_pub.h>
#include <huawei_platform/log/hw_log.h>

#define HWLOG_TAG    DSM_RECOVERY
HWLOG_REGIST();

#define DSM_LOG_INFO(x...)      _hwlog_info(HWLOG_TAG, ##x)
#define DSM_LOG_ERR(x...)       _hwlog_err(HWLOG_TAG, ##x)
#define DSM_LOG_DEBUG(x...)     _hwlog_debug(HWLOG_TAG, ##x)

struct dsm_dev dsm_recovery = {
	.name = "dsm_recovery",
	.device_name = NULL,
	.ic_name = NULL,
	.module_name = NULL,
	.fops = NULL,
	.buff_size = 1024,
};

struct dsm_client *recovery_client;

static int __init dsm_recovery_init(void)
{
	if (recovery_client == NULL)
		recovery_client = dsm_register_client(&dsm_recovery);
	if (recovery_client == NULL)
		DSM_LOG_ERR("recovery_client reg failed\n");
	return 0;
}

module_init(dsm_recovery_init);
