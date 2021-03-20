/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: mipi switch between AP and MCU
 * Author: lijiawen
 * Create: 2020-04-20
 */
#include "hisi_fb.h"

void mipi_to_ap(struct hisi_fb_data_type *hisifd);

void mipi_to_mcu(struct hisi_fb_data_type *hisifd);

void mipi_switch_release(struct platform_device* pdev);

void mipi_switch_init(struct platform_device* pdev);
