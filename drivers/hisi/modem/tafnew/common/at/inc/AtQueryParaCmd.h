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
#ifndef __ATQUERYPARACMD_H__
#define __ATQUERYPARACMD_H__

#include "AtCtx.h"
#include "vos.h"
#include "mn_msg_api.h"
#include "at_mta_interface.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)

VOS_UINT32 AT_QryGTimerPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryRsimPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryBatVolPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_DeciDigit2Ascii(VOS_UINT8 aucDeciDigit[], VOS_UINT32 length, VOS_UINT8 aucAscii[]);
VOS_VOID   AT_ConvertImsiDigit2String(VOS_UINT8 *imsi, VOS_UINT8 *imsiString, VOS_UINT32 strBufLen);
VOS_UINT32 AT_GetPhynumMac(VOS_UINT8 aucMac[], VOS_UINT32 macLen);
VOS_UINT32 AT_QryPhyNumPara(VOS_UINT8 indexNum);
#if (FEATURE_RSTRIGGER_SMS == FEATURE_ON)
VOS_UINT32 AT_WriteActiveMessage(ModemIdUint16 modemId, MN_MSG_ActiveMessage *orgActiveMessageInfo,
                                 MN_MSG_ActiveMessage *activeMessageInfo);
#endif
TAF_UINT32 AT_QryRstriggerPara(TAF_UINT8 indexNum);
TAF_UINT32 At_QryClipPara(TAF_UINT8 indexNum);
TAF_UINT32 At_QryClirPara(TAF_UINT8 indexNum);
TAF_UINT32 At_QryColpPara(TAF_UINT8 indexNum);
TAF_UINT32 At_QryS0Para(TAF_UINT8 indexNum);
TAF_UINT32 At_QryS3Para(TAF_UINT8 indexNum);
TAF_UINT32 At_QryS4Para(TAF_UINT8 indexNum);
TAF_UINT32 At_QryS5Para(TAF_UINT8 indexNum);
TAF_UINT32 At_QryS6Para(TAF_UINT8 indexNum);
TAF_UINT32 At_QryS7Para(TAF_UINT8 indexNum);
TAF_UINT32 At_QryCusdPara(TAF_UINT8 indexNum);
TAF_UINT32 At_QryCcwaPara(TAF_UINT8 indexNum);
TAF_UINT32 At_QryCpinPara(TAF_UINT8 indexNum);
TAF_UINT32 At_QryIccidPara(TAF_UINT8 indexNum);
TAF_UINT32 At_QryCardTypePara(TAF_UINT8 indexNum);
TAF_UINT32 At_QryCardTypeExPara(TAF_UINT8 indexNum);
TAF_UINT32 At_QryPNNPara(TAF_UINT8 indexNum);
TAF_UINT32 At_QryCPNNPara(TAF_UINT8 indexNum);
TAF_UINT32 At_QryOPLPara(TAF_UINT8 indexNum);
TAF_UINT32 At_QryCardlockPara(TAF_UINT8 indexNum);
TAF_UINT32 At_QryCpin2Para(TAF_UINT8 indexNum);
TAF_UINT32 At_QryCpbsPara(TAF_UINT8 indexNum);
TAF_UINT32 At_QryCfunPara(TAF_UINT8 indexNum);
#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
extern TAF_UINT32 At_QryCpamPara(TAF_UINT8 indexNum);
#endif
TAF_UINT32 At_QryStsfPara(TAF_UINT8 indexNum);
#if ((FEATURE_PHONE_USIM == FEATURE_OFF) || (defined(DMT)) || (defined(__PC_UT__)))
TAF_UINT32 At_QryStgiPara(TAF_UINT8 indexNum);
#endif
TAF_UINT32 At_QryImsichgPara(TAF_UINT8 indexNum);
TAF_UINT32 At_QryCgclassPara(TAF_UINT8 indexNum);
TAF_UINT32 At_QryCopsPara(TAF_UINT8 indexNum);
VOS_UINT32 At_QryHplmnPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryDplmnListPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryBorderInfoPara(VOS_UINT8 indexNum);
#if (FEATURE_CSG == FEATURE_ON)
VOS_UINT32 At_QryCampCsgIdInfo(VOS_UINT8 indexNum);
#endif
TAF_UINT32 At_QryCgcattPara(TAF_UINT8 indexNum);
TAF_UINT32 At_QryCgattPara(TAF_UINT8 indexNum);
TAF_UINT32 AT_QryCgdnsPara(TAF_UINT8 indexNum);
TAF_UINT32 AT_QryCgautoPara(TAF_UINT8 indexNum);
TAF_UINT32 AT_QryCgtftPara(TAF_UINT8 indexNum);
TAF_UINT32 AT_QryCgactPara(TAF_UINT8 indexNum);
VOS_UINT32 AT_QryCgdcontPara(VOS_UINT8 indexNum);
TAF_UINT32 AT_QryCgeqreqPara(TAF_UINT8 indexNum);
TAF_UINT32 At_QryCgeqminPara(TAF_UINT8 indexNum);
VOS_UINT32 AT_QryCgdscontPara(VOS_UINT8 indexNum);
TAF_UINT32 At_QryCrcPara(TAF_UINT8 indexNum);
TAF_UINT32 At_QryCbstPara(TAF_UINT8 indexNum);
TAF_UINT32 At_QryCmodPara(TAF_UINT8 indexNum);
TAF_UINT32 At_QryCstaPara(TAF_UINT8 indexNum);
TAF_UINT32 At_QryCcugPara(TAF_UINT8 indexNum);
TAF_UINT32 At_QryCssnPara(TAF_UINT8 indexNum);
TAF_UINT32 At_QryCnmiPara(TAF_UINT8 indexNum);
TAF_UINT32 At_QryCmgfPara(TAF_UINT8 indexNum);
TAF_UINT32 At_QryCscaPara(TAF_UINT8 indexNum);
TAF_UINT32 At_QryCsmsPara(TAF_UINT8 indexNum);
TAF_UINT32 At_QryCsmpPara(TAF_UINT8 indexNum);
TAF_UINT32 At_QryCpmsPara(TAF_UINT8 indexNum);
TAF_UINT32 At_QryCgsmsPara(TAF_UINT8 indexNum);
TAF_UINT32 At_QryCmgdPara(TAF_UINT8 indexNum);
TAF_UINT32 At_QryCmmsPara(TAF_UINT8 indexNum);
TAF_UINT32 At_QryCscbPara(TAF_UINT8 indexNum);
TAF_UINT32 At_QryCgregPara(TAF_UINT8 indexNum);
#if (FEATURE_LTE == FEATURE_ON)
VOS_UINT32 At_QryCeregPara(VOS_UINT8 indexNum);
#endif
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
VOS_UINT32 At_QryC5gregPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryLendcPara(VOS_UINT8 indexNum);
#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
VOS_UINT32 AT_QryNrFreqLockPara(VOS_UINT8 indexNum);
#endif

VOS_UINT32 AT_QryC5gqosPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryC5gPNssaiPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryC5gNssaiPara(VOS_UINT8 indexNum);
#endif
VOS_UINT32 AT_QryRrcStatPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryTxPowerPara(VOS_UINT8 indexNum);
#ifndef MODEM_FUSION_VERSION
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
VOS_UINT32 AT_QryNtxPowerPara(VOS_UINT8 indexNum);
#endif
#endif
VOS_UINT32 AT_QryHfreqInfoPara(VOS_UINT8 indexNum);
TAF_UINT32 At_QryCregPara(TAF_UINT8 indexNum);
TAF_UINT32 At_QryCsdhPara(TAF_UINT8 indexNum);
TAF_UINT32 At_QryCscsPara(TAF_UINT8 indexNum);
TAF_UINT32 At_QryCmeePara(TAF_UINT8 indexNum);
TAF_UINT32 At_QryFPlmnPara(TAF_UINT8 indexNum);
#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
VOS_UINT32 At_QryMaxFreelockSizePara(VOS_UINT8 indexNum);
#endif
#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
VOS_UINT32 At_QryCpuLoadPara(VOS_UINT8 indexNum);
#endif
VOS_UINT32 At_QryLcStartTimePara(VOS_UINT8 indexNum);
TAF_UINT32 At_QryDialModePara(TAF_UINT8 indexNum);
TAF_UINT32 At_QryCurcPara(TAF_UINT8 indexNum);
TAF_UINT32 At_QryTimePara(TAF_UINT8 indexNum);
TAF_UINT32 At_QryCtzrPara(TAF_UINT8 indexNum);
VOS_UINT32 AT_QryTimeQryPara(VOS_UINT8 indexNum);
VOS_UINT16 AT_PrintNwTimeInfo(NAS_MM_InfoInd *mmInfo, CONST_T VOS_CHAR *pcCmd, VOS_UINT32 nwSecond);
TAF_UINT32 At_QryQuickStart(TAF_UINT8 indexNum);
TAF_UINT32 At_QryAutoAttach(TAF_UINT8 indexNum);
VOS_UINT32 AT_QrySysCfgExPara(VOS_UINT8 indexNum);
#if (FEATURE_LTE == FEATURE_ON)
TAF_UINT32 At_QryCemode(TAF_UINT8 indexNum);
TAF_UINT32 At_QryLtecsInfo(TAF_UINT8 indexNum);
#endif
VOS_UINT32 At_QryClvlPara(VOS_UINT8 indexNum);
VOS_UINT32 At_QryVMSETPara(VOS_UINT8 indexNum);
#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
TAF_UINT32 At_QryRRCVersion(TAF_UINT8 indexNum);
#endif
TAF_UINT32 At_QryCSNR(TAF_UINT8 indexNum);
#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
TAF_UINT32 At_QryFreqLock(TAF_UINT8 indexNum);
#endif
#if (FEATURE_PROBE_FREQLOCK == FEATURE_ON)
VOS_UINT32 At_QryM2MFreqLock(VOS_UINT8 indexNum);
#endif
TAF_UINT32 At_QryU2DiagPara(TAF_UINT8 indexNum);
VOS_UINT32 At_QryPort(VOS_UINT8 indexNum);
TAF_UINT32 At_QryPcscInfo(TAF_UINT8 indexNum);
TAF_UINT32 At_QryCellSearch(TAF_UINT8 indexNum);
TAF_UINT32 At_QryGetportmodePara(TAF_UINT8 indexNum);
VOS_UINT32 At_QryCvoicePara(VOS_UINT8 indexNum);
VOS_UINT32 At_QryDdsetexPara(VOS_UINT8 indexNum);
TAF_UINT32 At_QryCmsrPara(TAF_UINT8 indexNum);
TAF_UINT32 At_QryUssdModePara(TAF_UINT8 indexNum);
TAF_UINT32 At_QryAdcTempPara(TAF_UINT8 indexNum);
#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
TAF_UINT32 At_QryCommDebugPara(TAF_UINT8 indexNum);
#endif

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
TAF_UINT32 At_QryYjcxPara(TAF_UINT8 indexNum);
#endif
VOS_UINT32              At_QryCplsPara(VOS_UINT8 indexNum);
TAF_UINT32              At_QryCpolPara(TAF_UINT8 indexNum);
TAF_UINT32              AT_QryAuthdataPara(TAF_UINT8 indexNum);
VOS_UINT32              AT_QryIpv6CapPara(VOS_UINT8 indexNum);
TAF_UINT32              At_QryCrpnPara(TAF_UINT8 indexNum);
TAF_UINT32              AT_QryAlsPara(TAF_UINT8 indexNum);
VOS_VOID                At_CovertMsInternalRxDivParaToUserSet(VOS_UINT16 curBandSwitch, VOS_UINT32 *userDivBandsLow,
                                                              VOS_UINT32 *userDivBandsHigh);
TAF_UINT32              At_QryRxDiv(TAF_UINT8 indexNum);
VOS_UINT32              At_QryCuus1Para(VOS_UINT8 indexNum);
VOS_UINT32              At_QryApHplmn(VOS_UINT8 indexNum);
VOS_UINT32              AT_QryAnQuery(VOS_UINT8 indexNum);
AT_CmdAntennaLevelUint8 AT_CalculateAntennaLevel(VOS_INT16 rscp, VOS_INT16 ecio);
VOS_VOID                AT_GetSmoothAntennaLevel(VOS_UINT8 indexNum, AT_CmdAntennaLevelUint8 level);
#if (FEATURE_LTE == FEATURE_ON)
TAF_VOID AT_CalculateLTESignalValue(VOS_INT16 *psRssi, VOS_UINT8 *level, VOS_INT16 *rsrp, VOS_INT16 *rsrq);
#endif
TAF_UINT32 At_QryGlastErrPara(TAF_UINT8 indexNum);
#if (FEATURE_LTEV == FEATURE_ON)
TAF_UINT32 At_QryVModePara(VOS_UINT8 indexNum);
#endif
VOS_UINT32 At_QryPlatForm(VOS_UINT8 indexNum);
VOS_UINT32 At_QryDataLock(VOS_UINT8 indexNum);

VOS_UINT32 At_QryVersion(VOS_UINT8 indexNum);
VOS_UINT32 At_QryBsn(VOS_UINT8 indexNum);
VOS_UINT32 At_QryCsVer(VOS_UINT8 indexNum);
VOS_UINT32 At_QryQosPara(VOS_UINT8 indexNum);
VOS_UINT32 At_QrySDomainPara(VOS_UINT8 indexNum);
VOS_UINT32 At_QryGPIOPL(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryMDatePara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryFacInfoPara(VOS_UINT8 indexNum);
VOS_UINT32 At_ReadDpaCatFromNV(VOS_UINT8 *dpaRate);
#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
VOS_UINT32 At_QryDpaCat(VOS_UINT8 indexNum);
#endif
VOS_UINT32 AT_ReadRrcVerFromNV(VOS_UINT8 *rrcVer);
VOS_UINT32 AT_QryHspaSpt(VOS_UINT8 indexNum);
VOS_UINT32 At_QryCallSrvPara(VOS_UINT8 indexNum);
VOS_UINT32 At_QryCsdfltPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_OutputCsdfltDefault(VOS_UINT8 indexNum, VOS_BOOL bSimlockEnableFlg);
VOS_UINT32 At_SimlockPlmnNumToAscii(const VOS_UINT8 *plmnRange, VOS_UINT8 plmnRangeLen, VOS_UINT8 *asciiStr);
VOS_UINT32 At_QrySimLockPlmnInfo(VOS_UINT8 indexNum);
VOS_UINT32 At_QryMaxLockTimes(VOS_UINT8 indexNum);
VOS_UINT32 At_QryAppWronReg(VOS_UINT8 indexNum);
TAF_UINT32 At_QryNdisdupPara(TAF_UINT8 indexNum);
TAF_UINT32 At_QryApDialModePara(TAF_UINT8 indexNum);
VOS_UINT32 AT_QryAppdmverPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryDislogPara(VOS_UINT8 indexNum);
#if (FEATURE_SECURITY_SHELL == FEATURE_ON)
VOS_UINT32 AT_QryShellPara(VOS_UINT8 indexNum);
#endif
VOS_UINT32 AT_QryWifiGlobalMacPara(VOS_UINT8 indexNum);
TAF_UINT32 AT_QryFlashInfoPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryAuthverPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryProdTypePara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryTmmiPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_GetWcdmaBandStr(VOS_UINT8 *gsmBandstr, VOS_UINT32 gsmBandLen, AT_UE_BandCapaSt *bandCapa);
VOS_UINT32 AT_GetWcdmaDivBandStr(VOS_UINT8 *gsmBandstr, VOS_UINT32 gsmBandLen);
VOS_UINT32 AT_GetGsmBandStr(VOS_UINT8 *gsmBandstr, VOS_UINT32 gsmBandLen, AT_UE_BandCapaSt *bandCapa);
VOS_UINT32 AT_QryFeaturePara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryProdNamePara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryEqverPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryApRptSrvUrlPara(VOS_UINT8 indexNum);

VOS_UINT32 AT_QryApXmlInfoTypePara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryApXmlRptFlagPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryFastDormPara(TAF_UINT8 indexNum);
#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
VOS_UINT32 AT_QryAcpuMemInfoPara(TAF_UINT8 indexNum);
#endif
VOS_UINT32 AT_QryCipherPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryLocinfoPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryNvResumePara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryNvBackupStatusPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryNandBadBlockPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryNandDevInfoPara(VOS_UINT8 indexNum);

VOS_UINT32 AT_QryChipTempPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryApRptPortSelectPara(VOS_UINT8 indexNum);
VOS_UINT32 At_QryUsbSwitchPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryAntState(VOS_UINT8 indexNum);
VOS_UINT32 AT_QrySARReduction(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryCposrPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryXcposrPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryXcposrRptPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryCPsErrPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryCmutPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryCCsErrPara(VOS_UINT8 indexNum);
VOS_UINT32 At_QryCerssiPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryCbgPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryAcInfoPara(VOS_UINT8 indexNum);
VOS_UINT32 At_QryMmPlmnInfoPara(VOS_UINT8 indexNum);
VOS_UINT32 At_QryEonsUcs2Para(VOS_UINT8 indexNum);
VOS_UINT32 At_QryPlmnPara(VOS_UINT8 indexNum);
VOS_UINT32 At_QryXlemaPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryBodySarOnPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryBodySarWcdmaPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryBodySarGsmPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryNvLoadPara(VOS_UINT8 indexNum);
VOS_UINT32 At_QryIMEIVerifyPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryNCellMonitorPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryUserSrvStatePara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryRefclkfreqPara(VOS_UINT8 indexNum);
VOS_UINT32 At_QryHandleDect(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryLogNvePara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryClccPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryUserCfgOPlmnPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryModemStatusPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryRATCombinePara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryMipiClkValue(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryBestFreqPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryCclkPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryEmcCallBack(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryTTYModePara(VOS_UINT8 indexNum);
VOS_UINT32 At_QryRatRfSwitch(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryTransModePara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryMccFreqPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryBlockCellListPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryUECenterPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryAfcClkInfo(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryFratIgnitionInfo(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryPacspPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryFclassPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryGciPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryRsrp(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryRsrq(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryCsdfPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryRejInfoPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryLteAttachInfoPara(VOS_UINT8 indexNum);

#if (FEATURE_LTE == FEATURE_ON)
VOS_UINT32 AT_QryRsrpCfgPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryRscpCfgPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryEcioCfgPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryPdprofmodPara(VOS_UINT8 indexNum);
VOS_UINT32 At_QryCecellidPara(VOS_UINT8 indexNum);
#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
VOS_UINT32 At_QrySfm(VOS_UINT8 indexNum);
#endif
#endif

#if (FEATURE_PHONE_SC == FEATURE_ON)
VOS_UINT32 AT_QryPhoneSimlockInfoPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QrySimlockDataReadPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryModemScIdPara(VOS_UINT8 indexNum);
#endif

#if (VOS_WIN32 == VOS_OS_VER)
VOS_UINT32 At_QryTinTypeStub(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryPsRegisterContainDrxStub(VOS_UINT8 indexNum);
#endif

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
VOS_UINT32 At_QryCserssiPara(VOS_UINT8 indexNum);
VOS_UINT32 At_QryNrBandBlockListPara(VOS_UINT8 atIndex);
VOS_UINT32 AT_QryLadnInfoPara(VOS_UINT8 idx);
#endif

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
VOS_UINT32 AT_QryCLteRoamAllowPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryCFreqLockInfo(VOS_UINT8 indexNum);
#endif

#if (FEATURE_IMS == FEATURE_ON)
VOS_UINT32 AT_QryCiregPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryCirepPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryImsRegDomainPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryImsDomainCfgPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryImsEmcRdpPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryClccEconfInfo(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryEconfErrPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryImsSwitchPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryCevdpPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryRoamImsServicePara(VOS_UINT8 indexNum);
#endif

#if (FEATURE_AT_HSUART == FEATURE_ON)
VOS_UINT32 AT_QryIprPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryIcfPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryIfcPara(VOS_UINT8 indexNum);
#endif

#if (FEATURE_HUAWEI_VP == FEATURE_ON)
VOS_UINT32 AT_QryVoicePreferPara(VOS_UINT8 indexNum);
#endif

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
VOS_UINT32 AT_QryCLocInfo(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryPrivacyModePreferred(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryCdmaCsqPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryCtaPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryProRevInUse(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryCasState(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryProGetEsn(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryProGetMeid(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryHighVer(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryCdmaDormTimerVal(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryHdrCsqPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryCurrSidNid(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryCtRoamInfo(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryPRLID(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryNoCardMode(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryRatCombinedMode(VOS_UINT8 indexNum);
#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
VOS_UINT32 AT_Qry1xChanPara(VOS_UINT8 indexNum);
#endif
#endif

#if ((FEATURE_UE_MODE_CDMA == FEATURE_ON) && (FEATURE_CHINA_TELECOM_VOICE_ENCRYPT == FEATURE_ON))
VOS_UINT32 AT_QryEncryptCallCap(VOS_UINT8 indexNum);
#if (FEATURE_CHINA_TELECOM_VOICE_ENCRYPT_TEST_MODE == FEATURE_ON)
VOS_UINT32 AT_QryEncryptCallRandom(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryEncryptCallKmc(VOS_UINT8 indexNum);
VOS_UINT32 AT_QryEccTestMode(VOS_UINT8 indexNum);
#endif
#endif

VOS_UINT32 AT_QrySinglePdnSwitch(VOS_UINT8 indexNum);

#if (FEATURE_MBB_CUST == FEATURE_ON)
#if (FEATURE_PHONE_USIM == FEATURE_OFF)
VOS_UINT32 AT_QueryNoCard(VOS_UINT8 indexNum);
#endif
#endif

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of AtQueryParaCmd.h */
