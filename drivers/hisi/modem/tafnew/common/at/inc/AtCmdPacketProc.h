/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
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

#ifndef __ATCMDPACKETPROC_H__
#define __ATCMDPACKETPROC_H__

#include "AtCtx.h"
#include "AtParse.h"
#include "AtRnicInterface.h"
#include "AtDataProc.h"
#include "taf_iface_api.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)

/*
 * 结构说明: DIAL类型
 */
enum AT_DIALTYPE {
    AT_DIALTYPE_APP  = 1,
    AT_DIALTYPE_NDIS = 2,

    AT_DIALTYPE_BUTT
};

typedef VOS_UINT32 AT_DIALTYPE_Uint32;

VOS_UINT32 At_SetDhcpPara(VOS_UINT8 indexNum);
VOS_UINT32 At_QryDhcpPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_TestDhcpPara(VOS_UINT8 indexNum);

VOS_UINT32 AT_SetDhcpv6Para(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryDhcpv6Para(VOS_UINT8 indexNum);
VOS_UINT32 AT_TestDhcpv6Para(VOS_UINT8 indexNum);

VOS_UINT32 AT_SetApRaInfoPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryApRaInfoPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_TestApRaInfoPara(VOS_UINT8 indexNum);

VOS_UINT32 AT_SetApLanAddrPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryApLanAddrPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_TestApLanAddrPara(VOS_UINT8 indexNum);

VOS_UINT32 AT_SetIPv6TempAddrPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_TestIPv6TempAddrPara(VOS_UINT8 indexNum);

VOS_UINT32 AT_SetCgpiafPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryCgpiafPara(VOS_UINT8 indexNum);

VOS_UINT32 At_SetApConnStPara(VOS_UINT8 indexNum);
VOS_UINT32 At_QryApConnStPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_TestApConnStPara(VOS_UINT8 indexNum);

VOS_UINT32 AT_SetApDsFlowRptCfgPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryApDsFlowRptCfgPara(VOS_UINT8 indexNum);

VOS_UINT32 AT_SetDsFlowNvWriteCfgPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryDsFlowNvWriteCfgPara(VOS_UINT8 indexNum);

#if (FEATURE_IMS == FEATURE_ON)
VOS_UINT32 AT_SetImsPdpCfg(VOS_UINT8 indexNum);
#endif

VOS_UINT32 AT_SetCgerepPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryCgerepPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetCindPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryCindPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_TestCindPara(VOS_UINT8 indexNum);

VOS_UINT32 AT_SetDataSwitchStatus(VOS_UINT8 indexNum);

VOS_UINT32 AT_QryDataSwitchStatus(VOS_UINT8 indexNum);

VOS_UINT32 AT_SetDataRoamSwitchStatus(VOS_UINT8 indexNum);

VOS_UINT32 AT_QryDataRoamSwitchStatus(VOS_UINT8 indexNum);

VOS_UINT32 At_SetUsbTetherInfo(VOS_UINT8 indexNum);
VOS_UINT32 At_TestUsbTetherInfo(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetRmnetCfg(VOS_UINT8 indexNum);

VOS_UINT32 At_QryDconnStatPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_TestDconnStatPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryNdisStatPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetCgmtuPara(VOS_UINT8 indexNum);

VOS_UINT32 AT_SetRoamPdpTypePara(VOS_UINT8 indexNum);

#if (FEATURE_UE_MODE_NR == FEATURE_ON)

VOS_UINT32 AT_SetCpolicyRptPara(VOS_UINT8 indexNum);

VOS_UINT32 AT_GetCpolicyCodePara(VOS_UINT8 indexNum);
#endif

#if (FEATURE_IMS == FEATURE_ON)
VOS_UINT32 AT_SetEmcStatus(VOS_UINT8 indexNum);
#endif

#pragma pack(pop)


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of AtCmdPacketProc.h */
