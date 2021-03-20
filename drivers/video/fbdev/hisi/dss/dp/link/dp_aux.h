/*
 * Copyright (c) 2016 Synopsys, Inc.
 *
 * Synopsys DP TX Linux Software Driver and documentation (hereinafter,
 * "Software") is an Unsupported proprietary work of Synopsys, Inc. unless
 * otherwise expressly agreed to in writing between Synopsys and you.
 *
 * The Software IS NOT an item of Licensed Software or Licensed Product under
 * any End User Software License Agreement or Agreement for Licensed Product
 * with Synopsys or any supplement thereto. You are permitted to use and
 * redistribute this Software in source and binary forms, with or without
 * modification, provided that redistributions of source code must retain this
 * notice. You may not view, use, disclose, copy or distribute this file or
 * any information contained herein except pursuant to this license grant from
 * Synopsys. If you do not agree with this notice, including the disclaimer
 * below, then you are not authorized to use the Software.
 *
 * THIS SOFTWARE IS BEING DISTRIBUTED BY SYNOPSYS SOLELY ON AN "AS IS" BASIS
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE HEREBY DISCLAIMED. IN NO EVENT SHALL SYNOPSYS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 */

/*
 * Copyright (c) 2020 Hisilicon Tech. Co., Ltd. Integrated into the Hisilicon display system.
 */

#ifndef __DP_AUX_H__
#define __DP_AUX_H__

#include <linux/kernel.h>

#define MAX_AUX_RETRY_COUNT 11
#define AUX_RETRY_DELAY_TIME 100
struct dp_ctrl;

/* native read/write 1 byte over aux */
int dptx_read_dpcd(struct dp_ctrl *dptx, uint32_t addr, uint8_t *byte);
int dptx_write_dpcd(struct dp_ctrl *dptx, uint32_t addr, uint8_t byte);
/* native read/write multi-bytes over aux, for DPCD */
int dptx_read_bytes_from_dpcd(struct dp_ctrl *dptx,
	uint32_t reg_addr, uint8_t *bytes, uint32_t len);
int dptx_write_bytes_to_dpcd(struct dp_ctrl *dptx,
	uint32_t reg_addr, uint8_t *bytes, uint32_t len);
/* i2c read/write multi-bytes over aux, for EDID */
int dptx_read_bytes_from_i2c(struct dp_ctrl *dptx,
	uint32_t device_addr, uint8_t *bytes, uint32_t len);
int dptx_write_bytes_to_i2c(struct dp_ctrl *dptx,
	uint32_t device_addr, uint8_t *bytes, uint32_t len);
/* i2c write only address over aux */
int dptx_i2c_address_only(struct dp_ctrl *dptx, uint32_t device_addr);

#endif /* DP_AUX_H */

