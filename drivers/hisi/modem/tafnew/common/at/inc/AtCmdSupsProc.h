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

#ifndef __ATCMDSUPSPROC_H__
#define __ATCMDSUPSPROC_H__

#include "AtCtx.h"
#include "AtParse.h"
#include "TafSsaApi.h"
#include "AtOutputProc.h"
#include "taf_msg_chk_api.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)

#define AT_PARA_NMEA_GPGSA "$GPGSA"
#define AT_PARA_NMEA_GPGGA "$GPGGA"
#define AT_PARA_NMEA_GPGSV "$GPGSV"
#define AT_PARA_NMEA_GPRMC "$GPRMC"
#define AT_PARA_NMEA_GPVTG "$GPVTG"
#define AT_PARA_NMEA_GPGLL "$GPGLL"
#define AT_PARA_NMEA_MAX_LEN 41
#define AT_PARA_NMEA_MIN_LEN 6

#define AT_PARA_CNAP_MAX_NAME_LEN 183

typedef VOS_VOID (*AT_SS_EVT_FUNC)(VOS_UINT8 ucIndex, TAF_Ctrl *pEvent);


/*lint -e958 -e959 64bit*/
typedef struct {
    TAF_SSA_EvtIdUint32 evtId;
    VOS_UINT32          msgLength;
    TAF_ChkMsgLenFunc   chkFunc;
    AT_SS_EVT_FUNC      evtFunc;
} AT_SS_EvtFuncTbl;
/*lint +e958 +e959 64bit*/

extern VOS_UINT32 AT_SetCmolrPara(VOS_UINT8 indexNum);
extern VOS_VOID   AT_RcvSsaSetLcsMolrCnf(VOS_UINT8 indexNum, TAF_Ctrl *event);
extern VOS_UINT32 AT_QryCmolrPara(VOS_UINT8 indexNum);
extern VOS_VOID   AT_RcvSsaGetLcsMolrCnf(VOS_UINT8 indexNum, TAF_Ctrl *event);
extern VOS_UINT32 AT_TestCmolrPara(VOS_UINT8 indexNum);
extern VOS_VOID   AT_RcvSsaLcsMolrNtf(VOS_UINT8 indexNum, TAF_Ctrl *event);
extern VOS_UINT32 AT_SetCmolrePara(VOS_UINT8 indexNum);
extern VOS_UINT32 AT_QryCmolrePara(VOS_UINT8 indexNum);
extern VOS_UINT32 AT_SetCmtlrPara(VOS_UINT8 indexNum);
extern VOS_VOID   AT_RcvSsaSetLcsMtlrCnf(VOS_UINT8 indexNum, TAF_Ctrl *event);
extern VOS_UINT32 AT_QryCmtlrPara(VOS_UINT8 indexNum);
extern VOS_VOID   AT_RcvSsaGetLcsMtlrCnf(VOS_UINT8 indexNum, TAF_Ctrl *event);
extern VOS_VOID   AT_RcvSsaLcsMtlrNtf(VOS_UINT8 indexNum, TAF_Ctrl *event);
extern VOS_UINT32 AT_SetCmtlraPara(VOS_UINT8 indexNum);
extern VOS_VOID   AT_RcvSsaSetLcsMtlraCnf(VOS_UINT8 indexNum, TAF_Ctrl *event);
extern VOS_UINT32 AT_QryCmtlraPara(VOS_UINT8 indexNum);
extern VOS_VOID   AT_RcvSsaGetLcsMtlraCnf(VOS_UINT8 indexNum, TAF_Ctrl *event);
extern VOS_UINT32 AT_TestCmtlraPara(VOS_UINT8 indexNum);
extern VOS_VOID   AT_RcvTafSsaEvt(TAF_SSA_Evt *event);

extern VOS_UINT32 AT_QryCnapExPara(VOS_UINT8 indexNum);

extern VOS_UINT32 AT_SetCnapPara(VOS_UINT8 indexNum);
extern VOS_UINT32 AT_QryCnapPara(VOS_UINT8 indexNum);
extern VOS_UINT32 AT_RcvTafCcmCnapInfoInd(struct MsgCB *msg);
extern TAF_VOID   AT_SsRspInterrogateCnfCnapProc(TAF_UINT8 indexNum, TAF_SS_CallIndependentEvent *event,
                                                 TAF_UINT32 *result, TAF_UINT16 *length);
extern VOS_VOID   AT_ReportCnapInfo(VOS_UINT8 indexNum, TAF_CALL_Cnap *nameIndicator);

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of AtCmdSupsProc.h */
