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

#ifndef _AT_EXTEND_PRIVATE_CMD_H_
#define _AT_EXTEND_PRIVATE_CMD_H_

#include "vos.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)

#define AT_DISSD_OPEN 0
#define AT_DISSD_CLOSE 1

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
VOS_UINT32 AT_SetDissdCmd(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetSdrebootCmd(VOS_UINT8 indexNum);
#endif

#if (FEATURE_RSTRIGGER_SMS == FEATURE_ON)
VOS_UINT32 AT_TestRstriggerPara(VOS_UINT8 indexNum);
#endif
VOS_UINT32 AT_TestStsfPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_TestCpnnPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_TestOplPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_TestPnnPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_TestStgiPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_TestStgrPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_TestImsichgPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_TestSetPort(VOS_UINT8 indexNum);
VOS_UINT32 AT_TestIpv6capPara(VOS_UINT8 indexNum);
VOS_UINT32 At_TestCmlckPara(VOS_UINT8 indexNum);
#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
VOS_UINT32 AT_TestSyscfg(VOS_UINT8 indexNum);
#endif
VOS_UINT32 AT_TestSyscfgEx(VOS_UINT8 indexNum);
VOS_UINT32 At_RegisterExPrivateCmdTable(VOS_VOID);
VOS_UINT32 AT_TestApRptPortSelectPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_TestUsbSwitchPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_TestRsfrPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_TestRsfwPara(VOS_UINT8 indexNum);
#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
VOS_UINT32 AT_TestApSimStPara(VOS_UINT8 indexNum);
#endif

VOS_UINT32 AT_TestXcposrPara(VOS_UINT8 indexNum);
/*
 * ��������: �����������
 */
VOS_UINT32 AT_TestClprPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_TestBodySarWcdmaPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_TestBodySarGsmPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_TestNetScanPara(VOS_UINT8 indexNum);
#if (FEATURE_PROBE_FREQLOCK == FEATURE_ON)
VOS_UINT32 At_TestM2MFreqLock(VOS_UINT8 indexNum);
#endif

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of AtExtendPrivateCmd.h */
