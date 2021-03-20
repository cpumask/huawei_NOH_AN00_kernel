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

#ifndef _AT_MT_COMM_H_
#define _AT_MT_COMM_H_

#include "at_mt_interface.h"
#include "at_lte_ct_proc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)

#define AT_TSELRF_PATH_TOTAL_MT 16
#define AT_FCHAN_CHANNEL_NO 2
#define AT_TSELRF_PARA_MIN_NUM 1
#define AT_TSELRF_PARA_MAX_NUM 2
#define AT_TSELRF_PATH_ID 0
#define AT_TSELRF_GROUP 1

VOS_UINT32 At_SetFChanPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetFwavePara(VOS_UINT8 indexNum);
VOS_UINT32 At_SetFTxonPara(VOS_UINT8 indexNum);
VOS_UINT32 At_SetFRxonPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetTSelRfPara(VOS_UINT8 indexNum);
VOS_UINT32 At_SetFpaPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetDcxotempcompPara(VOS_UINT8 indexNum);
VOS_UINT32 At_SetDpdtPara(VOS_UINT8 indexNum);
VOS_UINT32 At_SetQryDpdtPara(VOS_UINT8 indexNum);
VOS_UINT32 At_SetTfDpdtPara(VOS_UINT8 indexNum);
VOS_UINT32 At_SetQryTfDpdtPara(VOS_UINT8 indexNum);
VOS_UINT32 At_QryFChanPara(VOS_UINT8 indexNum);
VOS_UINT32 At_QryFTxonPara(VOS_UINT8 indexNum);
VOS_UINT32 At_QryFRxonPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryFpowdetTPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryFPllStatusPara(VOS_UINT8 indexNum);
VOS_UINT32 At_QryFrssiPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryTSelRfPara(VOS_UINT8 indexNum);
VOS_UINT32 At_QryFpaPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryDcxotempcompPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetFAgcgainPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryFAgcgainPara(VOS_UINT8 indexNum);
#if (FEATURE_UE_MODE_NR == FEATURE_OFF)
extern VOS_UINT32 AT_SetPdmCtrlPara(VOS_UINT8 indexNum);

extern VOS_UINT32 At_QryCltInfo(VOS_UINT8 indexNum);

VOS_UINT32 AT_SetGlobalFchan(VOS_UINT8 ratMode);

VOS_UINT32 AT_SetTlRficSsiRdPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetFDac(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetMipiWrPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetSSIWrPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetSSIRdPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetPhyMipiWritePara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetMipiRdPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetMipiReadPara(VOS_UINT8 indexNum);
VOS_UINT32 At_SetCltPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryFDac(VOS_UINT8 indexNum);
VOS_UINT32 At_TestFdacPara(VOS_UINT8 indexNum);
VOS_UINT8  At_GetDspLoadMode(VOS_UINT32 ratMode);

VOS_BOOL AT_GetTseLrfLoadDspInfo(AT_TSELRF_PathUint32 path, DRV_AGENT_TselrfSetReq *tseLrf);

#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
VOS_UINT32 AT_SetWifiFwavePara(VOS_VOID);
VOS_UINT32 AT_ProcTSelRfWifiPara(VOS_VOID);
#endif

#else
VOS_VOID              AT_SetUartTestState(AT_UartTestStateUint8 uartTestState);
AT_UartTestStateUint8 AT_GetUartTestState(VOS_VOID);
VOS_VOID              AT_SetI2sTestState(AT_I2sTestStateUint8 i2sTestState);
AT_I2sTestStateUint8  AT_GetI2sTestState(VOS_VOID);
VOS_UINT32            AT_SetMipiOpeRatePara(VOS_UINT8 indexNum);
VOS_UINT32            AT_SetFRbInfoPara(VOS_UINT8 indexNum);
VOS_UINT32            AT_QryFtemprptPara(VOS_UINT8 indexNum);
VOS_UINT32            At_CovertAtModeToBbicCal(AT_DEVICE_CmdRatModeUint8 atMode, RAT_MODE_ENUM_UINT16 *bbicMode);
VOS_UINT32 At_CovertAtBandWidthToBbicCal(AT_BAND_WidthUint16 atBandWidth, BANDWIDTH_ENUM_UINT16 *bbicBandWidth);
VOS_UINT32 At_CovertAtBandWidthToValue(AT_BAND_WidthUint16 atBandWidth, AT_BAND_WidthValueUint32 *bandWidthValue);
VOS_UINT32 At_CovertAtScsToBbicCal(AT_SubCarrierSpacingUint8 atScs, NR_SCS_TYPE_COMM_ENUM_UINT8 *bbicScs);
VOS_UINT32 At_CovertRatModeToBbicCal(AT_CmdRatmodeUint8 ratMode, RAT_MODE_ENUM_UINT16 *bbicMode);
VOS_UINT32 At_CovertChannelTypeToBbicCal(AT_TEMP_ChannelTypeUint16           channelType,
                                         BBIC_TEMP_CHANNEL_TYPE_ENUM_UINT16 *bbicChannelType);
VOS_UINT32 At_CovertAtPaLevelToBbicCal(AT_CmdPalevelUint8 atPaLevel, BBIC_CAL_PA_MODE_ENUM_UINT8 *bbicPaLevel);
VOS_UINT32 AT_GetNrFreqOffset(VOS_UINT32 channelNo, AT_NR_FreqOffsetTable *nrFreqOffset);
VOS_VOID   AT_GetNrFreqFromUlChannelNo(VOS_UINT32 ulChannelNo, AT_NR_FreqOffsetTable *nrFreqOffset,
                                       AT_DSP_BandFreq *dspBandFreq, const AT_NR_BandInfo *bandInfo);
VOS_VOID   AT_GetNrFreqFromDlChannelNo(VOS_UINT32 dlChannelNo, AT_NR_FreqOffsetTable *nrFreqOffset,
                                       AT_DSP_BandFreq *dspBandFreq, const AT_NR_BandInfo *bandInfo);
VOS_UINT32 AT_GetNrFreq(VOS_UINT32 channelNo);
VOS_VOID   AT_GetLteFreqFromUlChannelNo(VOS_UINT32 ulChannelNo, AT_DSP_BandFreq *dspBandFreq,
                                        const AT_LTE_BandInfo *bandInfo);
VOS_VOID   AT_GetLteFreqFromDlChannelNo(VOS_UINT32 dlChannelNo, AT_DSP_BandFreq *dspBandFreq,
                                        const AT_LTE_BandInfo *bandInfo);
VOS_UINT32 AT_GetLteFreq(VOS_UINT32 channelNo);
VOS_UINT32 AT_GetWFreqFromUlChannelNo(VOS_UINT32 ulChannelNo, AT_DSP_BandFreq *dspBandFreq,
                                      const AT_W_BandInfo *bandInfo);
VOS_UINT32 AT_GetWFreqFromDlChannelNo(VOS_UINT32 dlChannelNo, AT_DSP_BandFreq *dspBandFreq,
                                      const AT_W_BandInfo *bandInfo);
VOS_UINT32 AT_GetWFreq(VOS_UINT32 channelNo);
VOS_UINT32 AT_GetGFreq(VOS_UINT32 channelNo);
VOS_UINT32 AT_GetCFreq(VOS_UINT32 channelNo);
VOS_UINT32 AT_GetFreq(VOS_UINT32 channelNo);
VOS_UINT32 AT_CheckNrFwaveTypePara(VOS_UINT32 para);
VOS_UINT32 AT_CheckLteFwaveTypePara(VOS_UINT32 para);
VOS_UINT32 AT_CheckCFwaveTypePara(VOS_UINT32 para);
VOS_UINT32 AT_CheckWFwaveTypePara(VOS_UINT32 para);
VOS_UINT32 AT_CheckGFwaveTypePara(VOS_UINT32 para);
VOS_UINT32 AT_CheckFwaveTypePara(VOS_UINT32 para);
VOS_UINT32 AT_CovertAtFwaveTypeToBbicCal(AT_FWAVE_TypeUint8 typeIn, MODU_TYPE_ENUM_UINT16 *typeOut);
VOS_UINT32 AT_CoverTselPathToPriOrDiv(AT_TSELRF_PathUint32 tselPath, AT_AntTypeUint8 *antType);

VOS_UINT32 At_GetBaseFreq(RAT_MODE_ENUM_UINT16 ratMode);

VOS_UINT32 At_SetTrxTasPara(VOS_UINT8 indexNum);

VOS_UINT32 At_QryTrxTasPara(VOS_UINT8 indexNum);

VOS_UINT16 AT_GetGsmUlPathByBandNo(VOS_UINT16 bandNo);
VOS_UINT32 At_SetRfIcMemTest(VOS_UINT8 indexNum);
VOS_UINT32 At_QryRfIcMemTest(VOS_UINT8 indexNum);
VOS_UINT32 At_SetFSerdesRt(VOS_UINT8 indexNum);
VOS_UINT32 At_QryFSerdesRt(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryRficDieIDExPara(VOS_UINT8 indexNum);
VOS_UINT32 At_SetSerdesAsyncTest(VOS_UINT8 indexNum);

#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
VOS_UINT32 At_SetFChanWifiProc(VOS_UINT32 band);
VOS_UINT32 AT_ProcTSelRfWifiPara(VOS_VOID);

#endif

#endif
VOS_UINT32 AT_SetFPllStatusQryPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_TestFPllStatusPara(VOS_UINT8 indexNum);

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
