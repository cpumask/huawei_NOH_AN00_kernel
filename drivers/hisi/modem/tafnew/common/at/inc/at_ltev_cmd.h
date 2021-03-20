/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 * foss@huawei.com
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 * * This program is free software; you can redistribute it and/or modify
 * * it under the terms of the GNU General Public License version 2 and
 * * only version 2 as published by the Free Software Foundation.
 * *
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * * GNU General Public License for more details.
 * *
 * * You should have received a copy of the GNU General Public License
 * * along with this program; if not, write to the Free Software
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the following license terms apply:
 *
 * * Redistribution and use in source and binary forms, with or without
 * * modification, are permitted provided that the following conditions
 * * are met:
 * * 1) Redistributions of source code must retain the above copyright
 * *    notice, this list of conditions and the following disclaimer.
 * * 2) Redistributions in binary form must reproduce the above copyright
 * *    notice, this list of conditions and the following disclaimer in the
 * *    documentation and/or other materials provided with the distribution.
 * * 3) Neither the name of Huawei nor the names of its contributors may
 * *    be used to endorse or promote products derived from this software
 * *    without specific prior written permission.
 *
 * * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef _AT_LTEV_CMD_H_
#define _AT_LTEV_CMD_H_

#include "taf_v2x_api.h"
#include "AtParse.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)
#if (FEATURE_LTEV == FEATURE_ON)
#define AT_CATM_PARA_MAX_NUM 2
#define AT_CCUTLE_PARA_MAX_NUM 5
#define AT_CCUTLE_TEST_MODE_CLOSE_STATUS 0
#define AT_CCUTLE_COMMUNICATION_DIRECTION 1
#define AT_CCUTLE_MONITOR_LIST_FORMAT 2
#define AT_CCUTLE_MONITOR_LIST_LEN 3
#define AT_CCUTLE_MONITOR_LIST_CONTEXT 4
#define AT_CV2XDTS_PARA_MAX_NUM 3
#define AT_CV2XDTS_SEND_DATA_ACTION 0
#define AT_CV2XDTS_SEND_DATA_SIZE 1
#define AT_CV2XDTS_SEND_DATA_PERIODICITY 2

#define AT_SLINFO_PARA_MAX_NUM 3
#define AT_SLINFO_FREQUENCY_BAND_INDEX 0
#define AT_SLINFO_BAND_WIDTHh_INDEX 1
#define AT_SLINFO_CENTRAL_FREQUENCY_INDEX 2

#define AT_PHYR_PARA_MAX_NUM 5
#define AT_PHYR_MAX_TXPWR_INDEX 0
#define AT_PHYR_MIN_MCS_INDEX 1
#define AT_PHYR_MAX_MCS_INDEX 2
#define AT_PHYR_MIN_SUBCHN_INDEX 3
#define AT_PHYR_MAX_SUBCHN_INDEX 4

#define AT_RPPCFG_SET_PARAM_NUM 10
#define AT_RPPCFG_QRY_PARAM_NUM 2
#define AT_RPPCFG_CLEAR_RESPOOL_CMD 3
#define AT_RPPCFG_GROUP_TYPE_INDEX 0
#define AT_RPPCFG_RT_TYPE_INDEX 1
#define AT_RPPCFG_POOL_ID_INDEX 2
#define AT_RPPCFG_SL_OFFSET_INDEX 3
#define AT_RPPCFG_SUB_CHAN_BITMAP_INDEX 4
#define AT_RPPCFG_ADJACENCY_INDEX 5
#define AT_RPPCFG_SUB_CHN_SIZE_INDEX 6
#define AT_RPPCFG_SUB_CHN_NUM_INDEX 7
#define AT_RPPCFG_SUB_CHN_STARTRB_INDEX 8
#define AT_RPPCFG_PSCCH_POOL_STARTRB_INDEX 9
#define AT_RPPCFG_POOL_NUM_INDEX 10

#define AT_SENDDATA_PARAM_NUM 6
#define AT_SENDDATA_SARB_START_INDEX 0
#define AT_SENDDATA_DARB_NUM_INDEX 1
#define AT_SENDDATA_MCS_INDEX 2
#define AT_SENDDATA_PERIOD_INDEX 3
#define AT_SENDDATA_SEND_FRMMASK_INDEX 4
#define AT_SENDDATA_FLAG_INDEX 5
#define AT_CV2XL2ID_PARA_NUM 2


VOS_UINT32 AT_SetCatm(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryCatm(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetCcutle(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryCcutle(VOS_UINT8 indexNum);
VOS_UINT32 AT_TestCcutle(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetCuspcreq(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetCutcr(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetCcbrreq(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetCv2xdts(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryCv2xdts(VOS_UINT8 indexNum);
VOS_UINT32 AT_TestCv2xdts(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryVsyncsrc(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetVsyncsrcrpt(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryVsyncsrcrpt(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetVsyncmode(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryVsyncmode(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryCbr(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetCv2xL2Id(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryCv2xL2Id(VOS_UINT8 indexNum);

VOS_VOID AT_InitV2xRespoolParams(AT_VRRC_RppcfgSet *params);
#endif
#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif
