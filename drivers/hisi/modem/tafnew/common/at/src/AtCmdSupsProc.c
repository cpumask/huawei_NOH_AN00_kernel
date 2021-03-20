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

#include "AtCmdSupsProc.h"
#include "securec.h"
#include "ATCmdProc.h"
#include "TafStdlib.h"
#include "AtEventReport.h"
#include "taf_msg_chk.h"


/*
 * 协议栈打印打点方式下的.C文件宏定义
 */
#define THIS_FILE_ID PS_FILE_ID_AT_CMD_SUPS_PROC_C

#define AT_CMOLR_MAX_PARA_NUM 15
#define AT_CMOLR_HOR_ACC_SET 2
#define AT_CMOLR_HOR_ACC 3
#define AT_CMOLR_VER_REQ 4
#define AT_CMOLR_VER_ACC_SET 5
#define AT_CMOLR_VER_ACC 6
#define AT_CMOLR_VEL_REQ 7
#define AT_CMOLR_REP_MODE 8
#define AT_CMOLR_TIMEOUT 9
#define AT_CMOLR_INTERVAL 10
#define AT_CMOLR_SHAPE_REP 11
#define AT_CMOLR_PLANE 12
#define AT_CMOLR_NMEA_REP 13
#define AT_CMOLR_THIRD_PARTY_ADDRESS 14

#define AT_CMTLRA_PARA_NUM 2

#define AT_CMOLR_HOR_ACC_SET_INDEX 2
#define AT_CMOLR_HOR_ACC_INDEX 3
#define AT_CMOLR_VER_REQ_INDEX 4
#define AT_CMOLR_VER_ACC_SET_INDEX 5
#define AT_CMOLR_VER_ACC_INDEX 6
#define AT_CMOLR_VEL_REQ_INDEX 7
#define AT_CMOLR_REP_MODE_INDEX 8
#define AT_CMOLR_TIMEOUT_INDEX 9
#define AT_CMOLR_INTERVAL_INDEX 10
#define AT_CMOLR_SHAPE_REP_INDEX 11
#define AT_CMOLR_PLANE_INDEX 12
#define AT_CMOLR_NMEA_REP_INDEX 13
#define AT_CMOLR_THIRD_PARTY_ADDRESS_INDEX 14
#define AT_CMD_CNAP_NO_OP_SS_STATUS 2

STATIC VOS_UINT32 AT_ChkTafSsaMolrNtfMsgLen(const MSG_Header *msgHeader);

static const AT_SS_EvtFuncTbl g_atSsEvtFuncTbl[] = {
    /* 事件ID */ /* 消息处理函数 */
    { ID_TAF_SSA_SET_LCS_MOLR_CNF,  sizeof(TAF_SSA_Evt) + sizeof(TAF_SSA_SetLcsMolrCnf) - 4,
      VOS_NULL_PTR, AT_RcvSsaSetLcsMolrCnf },
    { ID_TAF_SSA_GET_LCS_MOLR_CNF,  sizeof(TAF_SSA_Evt) + sizeof(TAF_SSA_GetLcsMolrSnf) - 4,
      VOS_NULL_PTR, AT_RcvSsaGetLcsMolrCnf },
    { ID_TAF_SSA_LCS_MOLR_NTF,      0, AT_ChkTafSsaMolrNtfMsgLen, AT_RcvSsaLcsMolrNtf },
    { ID_TAF_SSA_SET_LCS_MTLR_CNF,  sizeof(TAF_SSA_Evt) + sizeof(TAF_SSA_SetLcsMtlrCnf) - 4,
      VOS_NULL_PTR, AT_RcvSsaSetLcsMtlrCnf },
    { ID_TAF_SSA_GET_LCS_MTLR_CNF,  sizeof(TAF_SSA_Evt) + sizeof(TAF_SSA_GetLcsMtlrCnf) - 4,
      VOS_NULL_PTR, AT_RcvSsaGetLcsMtlrCnf },
    { ID_TAF_SSA_LCS_MTLR_NTF,      sizeof(TAF_SSA_Evt) + sizeof(TAF_SSA_LcsMtlrNtf) - 4,
      VOS_NULL_PTR, AT_RcvSsaLcsMtlrNtf },
    { ID_TAF_SSA_SET_LCS_MTLRA_CNF, sizeof(TAF_SSA_Evt) + sizeof(TAF_SSA_SetLcsMtlraCnf) - 4,
      VOS_NULL_PTR, AT_RcvSsaSetLcsMtlraCnf },
    { ID_TAF_SSA_GET_LCS_MTLRA_CNF, sizeof(TAF_SSA_Evt) + sizeof(TAF_SSA_GetLcsMtlraCnf) - 4,
      VOS_NULL_PTR, AT_RcvSsaGetLcsMtlraCnf },
};

STATIC VOS_UINT32 AT_ChkTafSsaMolrNtfMsgLen(const MSG_Header *msgHeader)
{
    const TAF_SSA_Evt        *msg = VOS_NULL_PTR;
    const TAF_SSA_LcsMolrNtf *ntf = VOS_NULL_PTR;
    VOS_UINT32 minLen;
    VOS_UINT32 expectedLen;

    /* 8表示TAF_SSA_LcsMolrNtf.locationStr[8]的大小 */
    minLen = sizeof(TAF_SSA_Evt) + sizeof(TAF_SSA_LcsMolrNtf) -
        8 - VOS_MSG_HEAD_LENGTH - TAF_DEFAULT_CONTENT_LEN;
    if (VOS_GET_MSG_LEN(msgHeader) < minLen) {
        return VOS_FALSE;
    }

    msg = (const TAF_SSA_Evt*)msgHeader;
    ntf = (const TAF_SSA_LcsMolrNtf*)(msg->content);
    if (ntf->locationStrLen < sizeof(ntf->locationStr)) {
        expectedLen = minLen + sizeof(ntf->locationStr);
    } else {
        expectedLen = minLen + ntf->locationStrLen;
    }
    return TAF_ChkMsgLenWithExpectedLen(msgHeader, expectedLen);
}


VOS_UINT32 AT_ConvertTafSsaErrorCode(VOS_UINT8 indexNum, TAF_ERROR_CodeUint32 errorCode)
{
    VOS_UINT32 result;

    if (errorCode == TAF_ERR_NO_ERROR) {
        return AT_OK;
    }

    result = AT_ERROR;

    if ((errorCode >= TAF_ERR_LCS_BASE) && (errorCode <= TAF_ERR_LCS_UNKNOWN_ERROR)) {
        result = errorCode - TAF_ERR_LCS_BASE + AT_CMOLRE_ERR_ENUM_BEGIN + 1;
        return result;
    }

    result = At_ChgTafErrorCode(indexNum, errorCode);

    return result;
}


VOS_VOID AT_FillCmolrParaEnable(TAF_SSA_LcsMolrParaSet *molrPara)
{
    /* <enable> */
    if ((g_atParaIndex == 0) || (g_atParaList[0].paraLen == 0)) {
        molrPara->enable = TAF_SSA_LCS_MOLR_ENABLE_TYPE_DISABLE;
    } else {
        molrPara->enable = (VOS_UINT8)g_atParaList[0].paraValue;
    }

    return;
}


VOS_VOID AT_FillCmolrParaMethod(TAF_SSA_LcsMolrParaSet *molrPara)
{
    if ((g_atParaIndex <= 1) || (g_atParaList[1].paraLen == 0)) {
        return;
    }

    /* <method> */
    molrPara->opMethod = VOS_TRUE;
    molrPara->method   = (VOS_UINT8)g_atParaList[1].paraValue;

    return;
}


VOS_UINT32 AT_FillCmolrParaHorAcc(TAF_SSA_LcsMolrParaSet *molrPara)
{
    if ((g_atParaIndex <= AT_CMOLR_HOR_ACC_SET_INDEX) || (g_atParaList[AT_CMOLR_HOR_ACC_SET].paraLen == 0)) {
        return VOS_OK;
    }

    /* <hor-acc-set> */
    molrPara->opHorAccSet = VOS_TRUE;
    molrPara->horAccSet   = (VOS_UINT8)g_atParaList[AT_CMOLR_HOR_ACC_SET].paraValue;

    if (molrPara->horAccSet == LCS_HOR_ACC_SET_PARAM) {
        /* <hor-acc> */
        /* 要求水平精度，但没有下发水平精度系数，返回失败 */
        if ((g_atParaIndex <= AT_CMOLR_HOR_ACC_INDEX) || (g_atParaList[AT_CMOLR_HOR_ACC].paraLen == 0)) {
            AT_WARN_LOG("AT_FillCmolrParaPartI: <hor-acc> is required!");
            return VOS_ERR;
        }

        molrPara->horAcc = (VOS_UINT8)g_atParaList[AT_CMOLR_HOR_ACC].paraValue;
    }

    return VOS_OK;
}


VOS_UINT32 AT_FillCmolrParaVerReq(TAF_SSA_LcsMolrParaSet *molrPara)
{
    if ((g_atParaIndex <= AT_CMOLR_VER_REQ_INDEX) || (g_atParaList[AT_CMOLR_VER_REQ].paraLen == 0)) {
        return VOS_OK;
    }

    /* <ver-req> */
    molrPara->opVerReq = VOS_TRUE;
    molrPara->verReq   = (VOS_UINT8)g_atParaList[AT_CMOLR_VER_REQ].paraValue;

    if ((molrPara->verReq != LCS_VER_REQUESTED) ||
        (g_atParaIndex <= AT_CMOLR_VER_ACC_SET_INDEX) || (g_atParaList[AT_CMOLR_VER_ACC_SET].paraLen == 0)) {
        return VOS_OK;
    }

    /* <ver-acc-set> */
    molrPara->opVerAccSet = VOS_TRUE;
    molrPara->verAccSet   = (VOS_UINT8)g_atParaList[AT_CMOLR_VER_ACC_SET].paraValue;

    if (molrPara->verAccSet == LCS_VER_ACC_SET_PARAM) {
        /* <ver-acc> */
        /* 要求垂直精度，但没有下发垂直精度系数，返回失败 */
        if ((g_atParaIndex <= AT_CMOLR_VER_ACC_INDEX) || (g_atParaList[AT_CMOLR_VER_ACC].paraLen == 0)) {
            AT_WARN_LOG("AT_FillCmolrParaVerReq: <ver-acc> is required!");
            return VOS_ERR;
        }

        molrPara->verAcc = (VOS_UINT8)g_atParaList[AT_CMOLR_VER_ACC].paraValue;
    }

    return VOS_OK;
}


VOS_VOID AT_FillCmolrParaVelReq(TAF_SSA_LcsMolrParaSet *molrPara)
{
    if ((g_atParaIndex <= AT_CMOLR_VEL_REQ_INDEX) || (g_atParaList[AT_CMOLR_VEL_REQ].paraLen == 0)) {
        return;
    }

    /* <vel-req> */
    molrPara->opVelReq = VOS_TRUE;
    molrPara->velReq   = (VOS_UINT8)g_atParaList[AT_CMOLR_VEL_REQ].paraValue;

    return;
}


VOS_UINT32 AT_FillCmolrParaRepMode(TAF_SSA_LcsMolrParaSet *molrPara)
{
    if ((g_atParaIndex <= AT_CMOLR_REP_MODE_INDEX) || (g_atParaList[AT_CMOLR_REP_MODE].paraLen == 0)) {
        return VOS_OK;
    }

    /* <rep-mode> */
    molrPara->opRepMode = VOS_TRUE;
    molrPara->repMode   = (VOS_UINT8)g_atParaList[AT_CMOLR_REP_MODE].paraValue;

    if (g_atParaIndex <= AT_CMOLR_TIMEOUT_INDEX) {
        return VOS_OK;
    }

    /* <timeout> */
    if (g_atParaList[AT_CMOLR_TIMEOUT].paraLen != 0) {
        molrPara->opTimeout = VOS_TRUE;
        molrPara->timeOut   = (VOS_UINT16)g_atParaList[AT_CMOLR_TIMEOUT].paraValue;
    }

    /* <interval> */
    if (molrPara->repMode == LCS_REP_MODE_PERIODIC_RPT) {
        if ((g_atParaIndex > AT_CMOLR_INTERVAL_INDEX) && (g_atParaList[AT_CMOLR_INTERVAL].paraLen != 0)) {
            molrPara->opInterval = VOS_TRUE;
            molrPara->interval   = (VOS_UINT16)g_atParaList[AT_CMOLR_INTERVAL].paraValue;
        }
    }

    if ((molrPara->opTimeout == VOS_TRUE) && (molrPara->opInterval == VOS_TRUE) &&
        (molrPara->interval < molrPara->timeOut)) {
        AT_WARN_LOG("AT_FillCmolrParaPartI: <interval> is too short!");
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_VOID AT_FillCmolrParaShapeRep(TAF_SSA_LcsMolrParaSet *molrPara)
{
    if ((g_atParaIndex <= AT_CMOLR_SHAPE_REP_INDEX) || (g_atParaList[AT_CMOLR_SHAPE_REP].paraLen == 0)) {
        return;
    }

    /* <shape-rep> */
    molrPara->opShapeRep = VOS_TRUE;
    molrPara->shapeRep   = (VOS_UINT8)g_atParaList[AT_CMOLR_SHAPE_REP].paraValue;

    return;
}


VOS_VOID AT_FillCmolrParaPlane(TAF_SSA_LcsMolrParaSet *molrPara)
{
    if ((g_atParaIndex <= AT_CMOLR_PLANE_INDEX) || (g_atParaList[AT_CMOLR_PLANE].paraLen == 0)) {
        return;
    }

    /* <plane> */
    molrPara->opPlane = VOS_TRUE;
    molrPara->plane   = (VOS_UINT8)g_atParaList[AT_CMOLR_PLANE].paraValue;

    return;
}


VOS_UINT32 AT_Str2NmeaRep(VOS_UINT16 length, VOS_CHAR *pcStr, TAF_SSA_LcsNmeaRep *nmeaRep)
{
    errno_t    memResult;
    VOS_UINT32 num;
    VOS_UINT32 loop;
    VOS_CHAR  *pcTemp = VOS_NULL_PTR;
    VOS_CHAR   srcStr[AT_PARA_NMEA_MAX_LEN + 1];

    (VOS_VOID)memset_s(srcStr, sizeof(srcStr), 0x00, sizeof(srcStr));
    (VOS_VOID)memset_s(nmeaRep, sizeof(TAF_SSA_LcsNmeaRep), 0x00, sizeof(TAF_SSA_LcsNmeaRep));

    if (length > AT_PARA_NMEA_MAX_LEN) {
        AT_WARN_LOG("AT_Str2NmeaRep: NMEA format string is too long!");
        return VOS_ERR;
    }

    num = ((VOS_UINT32)length + 1) / (AT_PARA_NMEA_MIN_LEN + 1);

    if (num == 0) {
        AT_WARN_LOG("AT_Str2NmeaRep: NMEA format string is too short!");
        return VOS_ERR;
    }

    if (length > 0) {
        memResult = memcpy_s(srcStr, sizeof(srcStr), pcStr, length);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(srcStr), length);
    }
    pcTemp = srcStr;

    for (loop = 0; loop < num; loop++) {
        /* $GPGGA */
        if (VOS_MemCmp(pcTemp, AT_PARA_NMEA_GPGGA, AT_PARA_NMEA_MIN_LEN) == 0) {
            nmeaRep->gpgga = VOS_TRUE;
        }
        /* $GPRMC */
        else if (VOS_MemCmp(pcTemp, AT_PARA_NMEA_GPRMC, AT_PARA_NMEA_MIN_LEN) == 0) {
            nmeaRep->gprmc = VOS_TRUE;
        }
        /* $GPGLL */
        else if (VOS_MemCmp(pcTemp, AT_PARA_NMEA_GPGLL, AT_PARA_NMEA_MIN_LEN) == 0) {
            nmeaRep->gpgll = VOS_TRUE;
        }
        else {
            AT_WARN_LOG("AT_Str2NmeaRep: NMEA format string is incorrect!");
            return VOS_ERR;
        }

        pcTemp += (AT_PARA_NMEA_MIN_LEN + 1);
    }

    return VOS_OK;
}


VOS_UINT32 AT_FillCmolrParaNmeaRep(TAF_SSA_LcsMolrParaSet *molrPara)
{
    if ((g_atParaIndex <= AT_CMOLR_NMEA_REP_INDEX) || (g_atParaList[AT_CMOLR_NMEA_REP].paraLen == 0)) {
        return VOS_OK;
    }

    /* <NMEA-rep> */
    if ((molrPara->enable == TAF_SSA_LCS_MOLR_ENABLE_TYPE_NMEA) ||
        (molrPara->enable == TAF_SSA_LCS_MOLR_ENABLE_TYPE_NMEA_GAD)) {
        molrPara->opNmeaRep = VOS_TRUE;
        if (AT_Str2NmeaRep(g_atParaList[AT_CMOLR_NMEA_REP].paraLen, (VOS_CHAR *)g_atParaList[AT_CMOLR_NMEA_REP].para,
                           &(molrPara->nmeaRep)) != VOS_OK) {
            return VOS_ERR;
        }
    }

    return VOS_OK;
}


VOS_UINT32 AT_FillCmolrParaThdPtyAddr(TAF_SSA_LcsMolrParaSet *molrPara)
{
    errno_t memResult;
    if ((g_atParaIndex <= AT_CMOLR_THIRD_PARTY_ADDRESS_INDEX) ||
        (g_atParaList[AT_CMOLR_THIRD_PARTY_ADDRESS].paraLen == 0)) {
        return VOS_OK;
    }

    /* <third-party-address> */
    if (g_atParaList[AT_CMOLR_THIRD_PARTY_ADDRESS].paraLen > TAF_SSA_LCS_THIRD_PARTY_ADDR_MAX_LEN) {
        AT_WARN_LOG("AT_FillCmolrParaPartIII: <third-party-address> is too long!");
        return VOS_ERR;
    }

    molrPara->opThirdPartyAddr = VOS_TRUE;
    memResult                  = memcpy_s(molrPara->thirdPartyAddr, sizeof(molrPara->thirdPartyAddr),
                                          (VOS_CHAR *)g_atParaList[AT_CMOLR_THIRD_PARTY_ADDRESS].para,
                                          g_atParaList[AT_CMOLR_THIRD_PARTY_ADDRESS].paraLen);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(molrPara->thirdPartyAddr),
                        g_atParaList[AT_CMOLR_THIRD_PARTY_ADDRESS].paraLen);

    return VOS_OK;
}


VOS_UINT32 AT_FillCmolrPara(TAF_SSA_LcsMolrParaSet *molrPara)
{
    (VOS_VOID)memset_s(molrPara, sizeof(TAF_SSA_LcsMolrParaSet), 0x00, sizeof(TAF_SSA_LcsMolrParaSet));

    /*
     * +CMOLR=[<enable>[,<method>[,<hor-acc-set>[,<hor-acc>[,<ver-req>
     * [,<ver-acc-set>[,<ver-acc>[,<vel-req> [,<rep-mode>[,<timeout>
     * [,<interval>[,<shape-rep>[,<plane>[,<NMEA-rep>
     * [,<third-party-address>]]]]]]]]]]]]]]]]
     */

    /* 参数过多，返回失败 */
    if (g_atParaIndex > AT_CMOLR_MAX_PARA_NUM) {
        AT_WARN_LOG("AT_FillCmolrPara: too many parameters!");
        return VOS_ERR;
    }

    AT_FillCmolrParaEnable(molrPara);

    AT_FillCmolrParaMethod(molrPara);

    if (AT_FillCmolrParaHorAcc(molrPara) != VOS_OK) {
        return VOS_ERR;
    }

    if (AT_FillCmolrParaVerReq(molrPara) != VOS_OK) {
        return VOS_ERR;
    }

    AT_FillCmolrParaVelReq(molrPara);

    if (AT_FillCmolrParaRepMode(molrPara) != VOS_OK) {
        return VOS_ERR;
    }

    AT_FillCmolrParaShapeRep(molrPara);

    AT_FillCmolrParaPlane(molrPara);

    if (AT_FillCmolrParaNmeaRep(molrPara) != VOS_OK) {
        return VOS_ERR;
    }

    if (AT_FillCmolrParaThdPtyAddr(molrPara) != VOS_OK) {
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 AT_SetCmolrPara(VOS_UINT8 indexNum)
{
    TAF_SSA_LcsMolrParaSet molrPara;

    (VOS_VOID)memset_s(&molrPara, sizeof(molrPara), 0x00, sizeof(molrPara));

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_WARN_LOG("AT_SetCmolrPara: Option Type Incrrect!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (AT_FillCmolrPara(&molrPara) != VOS_OK) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 执行命令操作 */
    TAF_SSA_SetCmolrInfo(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId, &molrPara);

    /* 设置当前操作类型 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CMOLR_SET;

    /* 命令处理挂起状态 */
    return AT_WAIT_ASYNC_RETURN;
}


VOS_VOID AT_RcvSsaSetLcsMolrCnf(VOS_UINT8 indexNum, TAF_Ctrl *event)
{
    TAF_SSA_SetLcsMolrCnf *setLcsMolrCnf = VOS_NULL_PTR;
    VOS_UINT32             result;

    /* 不能为广播通道 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvSsaSetLcsMolrCnf: WARNING:AT_BROADCAST_INDEX!");
        return;
    }

    /* 判断当前操作类型是否为AT_CMD_CMOLR_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CMOLR_SET) {
        AT_WARN_LOG("AT_RcvSsaSetLcsMolrCnf: WARNING:Not AT_CMD_CMOLR_SET!");
        return;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    setLcsMolrCnf = (TAF_SSA_SetLcsMolrCnf *)event;
    result        = AT_ConvertTafSsaErrorCode(indexNum, setLcsMolrCnf->result);

    if (result != AT_OK) {
        AT_WARN_LOG("AT_RcvSsaSetLcsMolrCnf: WARNING:Set ^CMOLR Failed!");
    }

    /* 输出结果 */
    g_atSendDataBuff.bufLen = 0;
    At_FormatResultData(indexNum, result);

    return;
}


VOS_UINT32 AT_QryCmolrPara(VOS_UINT8 indexNum)
{
    /* 执行查询操作 */
    TAF_SSA_GetCmolrInfo(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId);

    /* 设置当前操作类型 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CMOLR_READ;

    /* 命令处理挂起状态 */
    return AT_WAIT_ASYNC_RETURN;
}


VOS_VOID AT_RcvSsaGetLcsMolrCnf(VOS_UINT8 indexNum, TAF_Ctrl *event)
{
    TAF_SSA_GetLcsMolrSnf *getLcsMolrCnf = VOS_NULL_PTR;
    VOS_UINT32             result;
    VOS_BOOL               bNmeaFlg      = VOS_FALSE;
    errno_t                memResult;
    VOS_UINT16             length        = 0;
    VOS_UINT8              tPAStr[LCS_CLIENT_EXID_MAX_LEN + 1];

    /* 不能为广播通道 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvSsaGetLcsMolrCnf: WARNING:AT_BROADCAST_INDEX!");
        return;
    }

    /* 判断当前操作类型是否为AT_CMD_CMOLR_READ */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CMOLR_READ) {
        AT_WARN_LOG("AT_RcvSsaGetLcsMolrCnf: WARNING:Not AT_CMD_CMOLR_READ!");
        return;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    getLcsMolrCnf = (TAF_SSA_GetLcsMolrSnf *)event;
    result        = AT_ConvertTafSsaErrorCode(indexNum, getLcsMolrCnf->result);

    /* 判断查询操作是否成功 */
    if (result == AT_OK) {
        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %d,%d,%d,", g_parseContext[indexNum].cmdElement->cmdName,
            getLcsMolrCnf->enable, getLcsMolrCnf->molrPara.method, getLcsMolrCnf->molrPara.horAccSet);

        if (getLcsMolrCnf->molrPara.horAccSet == LCS_HOR_ACC_SET_PARAM) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%d", getLcsMolrCnf->molrPara.horAcc);
        }

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d,", getLcsMolrCnf->molrPara.verReq);

        if (getLcsMolrCnf->molrPara.verReq == LCS_VER_REQUESTED) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%d,", getLcsMolrCnf->molrPara.verAccSet);

            if (getLcsMolrCnf->molrPara.verAccSet == LCS_VER_ACC_SET_PARAM) {
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, "%d", getLcsMolrCnf->molrPara.verAcc);
            }
        } else {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",");
        }

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d,%d,%d,", getLcsMolrCnf->molrPara.velReq,
            getLcsMolrCnf->molrPara.repMode, getLcsMolrCnf->molrPara.timeOut);

        if (getLcsMolrCnf->molrPara.repMode == LCS_REP_MODE_PERIODIC_RPT) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%d", getLcsMolrCnf->molrPara.interval);
        }

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d,%d,", getLcsMolrCnf->molrPara.u.shapeRep,
            getLcsMolrCnf->plane);

        if ((getLcsMolrCnf->enable == TAF_SSA_LCS_MOLR_ENABLE_TYPE_NMEA) ||
            (getLcsMolrCnf->enable == TAF_SSA_LCS_MOLR_ENABLE_TYPE_NMEA_GAD)) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "\"");

            if (getLcsMolrCnf->nmeaRep.gpgga == VOS_TRUE) {
                bNmeaFlg = VOS_TRUE;
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, "%s,", AT_PARA_NMEA_GPGGA);
            }

            if (getLcsMolrCnf->nmeaRep.gprmc == VOS_TRUE) {
                bNmeaFlg = VOS_TRUE;
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, "%s,", AT_PARA_NMEA_GPRMC);
            }

            if (getLcsMolrCnf->nmeaRep.gpgll == VOS_TRUE) {
                bNmeaFlg = VOS_TRUE;
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, "%s,", AT_PARA_NMEA_GPGLL);
            }
            /* 删除多打印的一个字符 */
            length -= 1;

            if (bNmeaFlg == VOS_TRUE) {
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, "\"");
            }
        }

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",");

        if (((getLcsMolrCnf->molrPara.method == LCS_MOLR_METHOD_TRANSFER_TP_ADDR) ||
             (getLcsMolrCnf->molrPara.method == LCS_MOLR_METHOD_RETRIEVAL_TP_ADDR)) &&
            ((getLcsMolrCnf->molrPara.tPAddr.length > 0) &&
             (getLcsMolrCnf->molrPara.tPAddr.length <= LCS_CLIENT_EXID_MAX_LEN))) {
            memResult = memset_s(tPAStr, sizeof(tPAStr), 0x00, LCS_CLIENT_EXID_MAX_LEN + 1);
            TAF_MEM_CHK_RTN_VAL(memResult, sizeof(tPAStr), LCS_CLIENT_EXID_MAX_LEN + 1);
            memResult = memcpy_s(tPAStr, sizeof(tPAStr), getLcsMolrCnf->molrPara.tPAddr.value,
                                 getLcsMolrCnf->molrPara.tPAddr.length);
            TAF_MEM_CHK_RTN_VAL(memResult, sizeof(tPAStr), getLcsMolrCnf->molrPara.tPAddr.length);

            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "\"%s\"", tPAStr);
        }
    }

    /* 输出结果 */
    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, result);

    return;
}


VOS_UINT32 AT_TestCmolrPara(VOS_UINT8 indexNum)
{
    /* 输出测试命令结果 */
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress,
        "%s: (0-3),(0-6),(0,1),(0-127),(0,1),(0,1),(0-127),(0-4),"
        "(0,1),(1-65535),(1-65535),(1-127),(0,1),"
        /* "\"$GPGSA,$GPGGA,$GPGSV,$GPRMC,$GPVTG,$GPGLL\"," */
        "\"$GPGGA,$GPRMC,$GPGLL\","
        "\"<third-party-address>\"", /* <third-party-address>参数暂不确定格式 */
        g_parseContext[indexNum].cmdElement->cmdName);

    return AT_OK;
}


VOS_VOID AT_RcvSsaLcsMolrNtf(VOS_UINT8 indexNum, TAF_Ctrl *event)
{
    TAF_SSA_LcsMolrNtf *lcsMolrNtf    = VOS_NULL_PTR;
    AT_ModemAgpsCtx    *agpsCtx       = VOS_NULL_PTR;
    VOS_CHAR           *pcLocationStr = VOS_NULL_PTR;
    VOS_UINT32          result;
    VOS_UINT32          returnCodeIndex;
    VOS_UINT16          length;
    errno_t             stringRet;

    /* 不能为广播通道 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvSsaLcsMolrNtf: WARNING:AT_BROADCAST_INDEX!");
        return;
    }

    lcsMolrNtf      = (TAF_SSA_LcsMolrNtf *)event;
    agpsCtx         = AT_GetModemAgpsCtxAddrFromClientId(g_atClientTab[indexNum].clientId);
    result          = AT_ConvertTafSsaErrorCode(indexNum, lcsMolrNtf->result);
    returnCodeIndex = 0;
    length          = 0;

    /* 判断查询操作是否成功 */
    if (result == AT_OK) {
        pcLocationStr = (VOS_CHAR *)PS_MEM_ALLOC(WUEPS_PID_AT, lcsMolrNtf->locationStrLen + 1);
        if (pcLocationStr == VOS_NULL_PTR) {
            AT_ERR_LOG("AT_RcvSsaLcsMolrNtf: Alloc Memory Failed!");
            return;
        }
        (VOS_VOID)memset_s(pcLocationStr, lcsMolrNtf->locationStrLen + 1, 0x00, lcsMolrNtf->locationStrLen + 1);

        if (lcsMolrNtf->locationStrLen > 0) {
            stringRet = strncpy_s(pcLocationStr, lcsMolrNtf->locationStrLen + 1, lcsMolrNtf->locationStr,
                                  lcsMolrNtf->locationStrLen);
            TAF_STRCPY_CHK_RTN_VAL_CONTINUE(stringRet, lcsMolrNtf->locationStrLen + 1, lcsMolrNtf->locationStrLen);
        }
        pcLocationStr[lcsMolrNtf->locationStrLen] = '\0';

        if (lcsMolrNtf->rptTypeChoice == TAF_SSA_LCS_MOLR_RPT_NMEA) {
            /* 输出+CMOLRN定位信息 */
            length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s%s\"%s\"%s", g_atCrLf, g_atStringTab[AT_STRING_CMOLRN].text,
                pcLocationStr, g_atCrLf);
        } else {
            /* 输出+CMOLRG定位信息 */
            length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s%s%s%s", g_atCrLf, g_atStringTab[AT_STRING_CMOLRG].text,
                pcLocationStr, g_atCrLf);
        }

        PS_MEM_FREE(WUEPS_PID_AT, pcLocationStr);
    } else {
        if (AT_GetReturnCodeId(result, &returnCodeIndex) != VOS_OK) {
            AT_ERR_LOG("AT_RcvSsaLcsMolrNtf: result code indexNum is err!");
            return;
        }

        /* 输出+CMOLRE错误码 */
        if (agpsCtx->cmolreType == AT_CMOLRE_NUMERIC) {
            length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s%s%s%s", g_atCrLf, g_atStringTab[AT_STRING_CMOLRE].text,
                (VOS_CHAR *)g_atReturnCodeTab[returnCodeIndex].result[0], g_atCrLf);
        } else {
            length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s%s%s%s", g_atCrLf, g_atStringTab[AT_STRING_CMOLRE].text,
                (VOS_CHAR *)g_atReturnCodeTab[returnCodeIndex].result[1], g_atCrLf);
        }
    }

    /* 输出结果到AT通道 */
    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return;
}


VOS_UINT32 AT_SetCmolrePara(VOS_UINT8 indexNum)
{
    AT_ModemAgpsCtx *agpsCtx = VOS_NULL_PTR;

    agpsCtx = AT_GetModemAgpsCtxAddrFromClientId(g_atClientTab[indexNum].clientId);

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_WARN_LOG("AT_SetCmolrePara: Option Type Incrrect!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数过多，返回失败 */
    if (g_atParaIndex > 1) {
        AT_WARN_LOG("AT_SetCmolrePara: too many parameters!");
        return AT_TOO_MANY_PARA;
    }

    /* 执行命令操作 */
    if (g_atParaList[0].paraLen == 0) {
        agpsCtx->cmolreType = AT_CMOLRE_NUMERIC;
    } else {
        agpsCtx->cmolreType = (VOS_UINT8)g_atParaList[0].paraValue;
    }

    /* 命令返回OK */
    return AT_OK;
}


VOS_UINT32 AT_QryCmolrePara(VOS_UINT8 indexNum)
{
    AT_ModemAgpsCtx *agpsCtx = VOS_NULL_PTR;

    agpsCtx = AT_GetModemAgpsCtxAddrFromClientId(g_atClientTab[indexNum].clientId);

    /* 输出查询结果 */
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName, agpsCtx->cmolreType);

    /* 命令返回OK */
    return AT_OK;
}


VOS_UINT32 AT_SetCmtlrPara(VOS_UINT8 indexNum)
{
    TAF_SSA_LcsMtlrSubscribeUint8 subscribe;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_WARN_LOG("AT_SetCmtlrPara: Option Type Incrrect!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数过多，返回失败 */
    if (g_atParaIndex > 1) {
        AT_WARN_LOG("AT_SetCmtlrPara: too many parameters!");
        return AT_TOO_MANY_PARA;
    }

    /* 执行命令操作 */
    if (g_atParaList[0].paraLen == 0) {
        subscribe = TAF_SSA_LCS_MOLR_ENABLE_TYPE_DISABLE;
    } else {
        subscribe = (VOS_UINT8)g_atParaList[0].paraValue;
    }
    TAF_SSA_SetCmtlrInfo(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId, subscribe);

    /* 设置当前操作类型 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CMTLR_SET;

    /* 命令处理挂起状态 */
    return AT_WAIT_ASYNC_RETURN;
}


VOS_VOID AT_RcvSsaSetLcsMtlrCnf(VOS_UINT8 indexNum, TAF_Ctrl *event)
{
    TAF_SSA_SetLcsMtlrCnf *setLcsMtlrCnf = VOS_NULL_PTR;
    VOS_UINT32             result;

    /* 不能为广播通道 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvSsaSetLcsMtlrCnf: WARNING:AT_BROADCAST_INDEX!");
        return;
    }

    /* 判断当前操作类型是否为AT_CMD_CMTLR_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CMTLR_SET) {
        AT_WARN_LOG("AT_RcvSsaSetLcsMtlrCnf: WARNING:Not AT_CMD_CMTLR_SET!");
        return;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    setLcsMtlrCnf = (TAF_SSA_SetLcsMtlrCnf *)event;
    result        = AT_ConvertTafSsaErrorCode(indexNum, setLcsMtlrCnf->result);

    if (result != AT_OK) {
        AT_WARN_LOG("AT_RcvSsaSetLcsMtlrCnf: WARNING:Set ^CMTLR Failed!");
    }

    /* 输出结果 */
    g_atSendDataBuff.bufLen = 0;
    At_FormatResultData(indexNum, result);

    return;
}


VOS_UINT32 AT_QryCmtlrPara(VOS_UINT8 indexNum)
{
    /* 执行查询操作 */
    TAF_SSA_GetCmtlrInfo(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId);

    /* 设置当前操作类型 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CMTLR_READ;

    /* 命令处理挂起状态 */
    return AT_WAIT_ASYNC_RETURN;
}


VOS_VOID AT_RcvSsaGetLcsMtlrCnf(VOS_UINT8 indexNum, TAF_Ctrl *event)
{
    TAF_SSA_GetLcsMtlrCnf *getLcsMtlrCnf = VOS_NULL_PTR;
    VOS_UINT32             result;
    VOS_UINT16             length;

    /* 不能为广播通道 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvSsaGetLcsMtlrCnf: WARNING:AT_BROADCAST_INDEX!");
        return;
    }

    /* 判断当前操作类型是否为AT_CMD_CMTLR_READ */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CMTLR_READ) {
        AT_WARN_LOG("AT_RcvSsaGetLcsMtlrCnf: WARNING:Not AT_CMD_CMTLR_READ!");
        return;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    getLcsMtlrCnf = (TAF_SSA_GetLcsMtlrCnf *)event;
    result        = AT_ConvertTafSsaErrorCode(indexNum, getLcsMtlrCnf->result);
    length        = 0;

    /* 判断查询操作是否成功 */
    if (result == AT_OK) {
        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            getLcsMtlrCnf->subscribe);
    }

    /* 输出结果 */
    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, result);

    return;
}


VOS_VOID AT_RcvSsaLcsMtlrNtf(VOS_UINT8 indexNum, TAF_Ctrl *event)
{
    TAF_SSA_LcsMtlrNtf *lcsMtlrNtf = VOS_NULL_PTR;
    VOS_UINT32          tmpStrLen;
    VOS_UINT32          i;
    VOS_UINT16          length;

    /* 只能为广播通道 */
    if (!AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvSsaLcsMtlrNtf: WARNING:Not AT_BROADCAST_INDEX!");
        return;
    }

    lcsMtlrNtf = (TAF_SSA_LcsMtlrNtf *)event;
    length     = 0;

    /*
     * +CMTLR: <handle-id>,<notification-type>,<location-type>,
     * [<client-external-id>],[<client-name>][,<plane>]
     */
    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s%s%d,%d,%d,", g_atCrLf, g_atStringTab[AT_STRING_CMTLR].text,
        lcsMtlrNtf->mtlrPara.handleId, lcsMtlrNtf->mtlrPara.ntfType, lcsMtlrNtf->mtlrPara.locationType);

    if (lcsMtlrNtf->mtlrPara.opClientExId == VOS_TRUE) {
        tmpStrLen = lcsMtlrNtf->mtlrPara.clientExId.length <= LCS_CLIENT_EXID_MAX_LEN ?
                        lcsMtlrNtf->mtlrPara.clientExId.length :
                        LCS_CLIENT_EXID_MAX_LEN;

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "\"");

        for (i = 0; i < tmpStrLen; i++) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%02X", lcsMtlrNtf->mtlrPara.clientExId.value[i]);
        }

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "\"");
    }

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, ",");

    if (lcsMtlrNtf->mtlrPara.opClientName == VOS_TRUE) {
        tmpStrLen = lcsMtlrNtf->mtlrPara.clientName.length <= LCS_CLIENT_NAME_MAX_LEN ?
                        lcsMtlrNtf->mtlrPara.clientName.length :
                        LCS_CLIENT_NAME_MAX_LEN;

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "\"");

        for (i = 0; i < tmpStrLen; i++) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%02X", lcsMtlrNtf->mtlrPara.clientName.value[i]);
        }

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "\"");
    }

    if (lcsMtlrNtf->mtlrPara.opPlane == VOS_TRUE) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", lcsMtlrNtf->mtlrPara.plane);
    }

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "\r\n");

    /* 输出结果到AT通道 */
    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return;
}


VOS_UINT32 AT_SetCmtlraPara(VOS_UINT8 indexNum)
{
    TAF_SSA_LcsMtlraParaSet cmtlraPara;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_WARN_LOG("AT_SetCmtlraPara: Option Type Incrrect!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数判断 */
    if (g_atParaIndex != AT_CMTLRA_PARA_NUM) {
        AT_WARN_LOG("AT_SetCmtlraPara: Incorrect Parameter Num!");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 执行命令操作 */
    (VOS_VOID)memset_s(&cmtlraPara, sizeof(cmtlraPara), 0x00, sizeof(cmtlraPara));
    cmtlraPara.allow    = (VOS_UINT8)g_atParaList[0].paraValue;
    cmtlraPara.handleId = (VOS_UINT8)g_atParaList[1].paraValue;

    TAF_SSA_SetCmtlraInfo(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId, &cmtlraPara);

    /* 设置当前操作类型 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CMTLRA_SET;

    /* 命令处理挂起状态 */
    return AT_WAIT_ASYNC_RETURN;
}


VOS_VOID AT_RcvSsaSetLcsMtlraCnf(VOS_UINT8 indexNum, TAF_Ctrl *event)
{
    TAF_SSA_SetLcsMtlraCnf *setLcsMtlraCnf = VOS_NULL_PTR;
    VOS_UINT32              result;

    /* 不能为广播通道 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvSsaSetLcsMtlraCnf: WARNING:AT_BROADCAST_INDEX!");
        return;
    }

    /* 判断当前操作类型是否为AT_CMD_CMTLRA_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CMTLRA_SET) {
        AT_WARN_LOG("AT_RcvSsaSetLcsMtlraCnf: WARNING:Not AT_CMD_CMTLRA_SET!");
        return;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    setLcsMtlraCnf = (TAF_SSA_SetLcsMtlraCnf *)event;
    result         = AT_ConvertTafSsaErrorCode(indexNum, setLcsMtlraCnf->result);

    if (result != AT_OK) {
        AT_WARN_LOG("AT_RcvSsaSetLcsMtlraCnf: WARNING:Set ^CMTLRA Failed!");
    }

    /* 输出结果 */
    g_atSendDataBuff.bufLen = 0;
    At_FormatResultData(indexNum, result);

    return;
}


VOS_UINT32 AT_QryCmtlraPara(VOS_UINT8 indexNum)
{
    /* 执行查询操作 */
    TAF_SSA_GetCmtlraInfo(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId);

    /* 设置当前操作类型 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CMTLRA_READ;

    /* 命令处理挂起状态 */
    return AT_WAIT_ASYNC_RETURN;
}


VOS_VOID AT_RcvSsaGetLcsMtlraCnf(VOS_UINT8 indexNum, TAF_Ctrl *event)
{
    TAF_SSA_GetLcsMtlraCnf *getLcsMtlraCnf = VOS_NULL_PTR;
    VOS_UINT32              result;
    VOS_UINT16              length;
    VOS_UINT32              i;
    VOS_UINT8               cnt;

    /* 不能为广播通道 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvSsaGetLcsMtlraCnf: WARNING:AT_BROADCAST_INDEX!");
        return;
    }

    /* 判断当前操作类型是否为AT_CMD_CMTLRA_READ */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CMTLRA_READ) {
        AT_WARN_LOG("AT_RcvSsaGetLcsMtlraCnf: WARNING:Not AT_CMD_CMTLRA_READ!");
        return;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    getLcsMtlraCnf = (TAF_SSA_GetLcsMtlraCnf *)event;
    result         = AT_ConvertTafSsaErrorCode(indexNum, getLcsMtlraCnf->result);
    length         = 0;

    /* 判断查询操作是否成功 */
    if (result == AT_OK) {
        cnt = (getLcsMtlraCnf->cnt > TAF_SSA_LCS_MTLR_MAX_NUM) ? TAF_SSA_LCS_MTLR_MAX_NUM : getLcsMtlraCnf->cnt;
        for (i = 0; i < cnt; i++) {
            if (i != 0) {
                length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + length, "%s", (VOS_CHAR *)g_atCrLf);
            }

            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s: %d,%d", g_parseContext[indexNum].cmdElement->cmdName,
                getLcsMtlraCnf->allow[i], getLcsMtlraCnf->handleId[i]);
        }
    }

    /* 输出结果 */
    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, result);

    return;
}


VOS_UINT32 AT_TestCmtlraPara(VOS_UINT8 indexNum)
{
    /* 输出测试命令结果 */
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: (0,1)", g_parseContext[indexNum].cmdElement->cmdName);

    return AT_OK;
}


VOS_VOID AT_RcvTafSsaEvt(TAF_SSA_Evt *event)
{
    MN_AT_IndEvt  *msg      = VOS_NULL_PTR;
    TAF_Ctrl      *ctrl     = VOS_NULL_PTR;
    AT_SS_EVT_FUNC evtFunc  = VOS_NULL_PTR;
    VOS_UINT8      indexNum = 0;
    VOS_UINT32     i;

    /* 判断事件是否是历史SSA代码上报 */
    if (event->evtExt != 0) {
        msg = (MN_AT_IndEvt *)event;
        At_SsMsgProc(msg->content, msg->len);
        return;
    }

    /* 初始化 */
    ctrl = (TAF_Ctrl *)(event->content);

    if (At_ClientIdToUserId(ctrl->clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafSsaEvt: At_ClientIdToUserId FAILURE");
        return;
    }

    /* 在事件处理表中查找处理函数 */
    for (i = 0; i < AT_ARRAY_SIZE(g_atSsEvtFuncTbl); i++) {
        if (event->evtId == g_atSsEvtFuncTbl[i].evtId) {
            if (TAF_RunChkMsgLenFunc((const MSG_Header *)event, g_atSsEvtFuncTbl[i].msgLength,
                    g_atSsEvtFuncTbl[i].chkFunc) == VOS_FALSE) {
                AT_ERR_LOG("AT_RcvTafSsaEvt: Check MsgLength Err");
                return;
            }
            /* 事件ID匹配 */
            evtFunc = g_atSsEvtFuncTbl[i].evtFunc;
            break;
        }
    }

    /* 如果处理函数存在则调用 */
    if (evtFunc != VOS_NULL_PTR) {
        evtFunc(indexNum, (TAF_Ctrl *)event->content);
    } else {
        AT_ERR_LOG1("AT_RcvTafSsaEvt: Unexpected event received! <EvtId>", event->evtId);
    }

    return;
}


VOS_UINT32 AT_QryCnapExPara(VOS_UINT8 indexNum)
{
    VOS_UINT32    ret;
    TAF_Ctrl      ctrl;
    ModemIdUint16 modemId;

    (VOS_VOID)memset_s(&ctrl, sizeof(ctrl), 0x00, sizeof(ctrl));

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    ctrl.moduleId = WUEPS_PID_AT;
    ctrl.clientId = g_atClientTab[indexNum].clientId;
    ctrl.opId     = g_atClientTab[indexNum].opId;

    if (AT_GetModemIdFromClient(g_atClientTab[indexNum].clientId, &modemId) != VOS_OK) {
        return AT_ERROR;
    }

    /* 发送跨核消息ID_TAF_CCM_CNAP_QRY_REQ到C核 */
    ret = TAF_CCM_CallCommonReq(&ctrl, VOS_NULL_PTR, ID_TAF_CCM_CNAP_QRY_REQ, 0, modemId);

    if (ret != VOS_OK) {
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CNAPEX_QRY;

    return AT_WAIT_ASYNC_RETURN;
}


LOCAL VOS_VOID AT_CnapConvertNameStr(TAF_CALL_Cnap *nameIndicator, VOS_UINT8 *nameStr, VOS_UINT8 nameStrMaxLen)
{
    errno_t    memResult;
    VOS_UINT32 ascIILen;

    (VOS_VOID)memset_s(nameStr, nameStrMaxLen, 0x00, nameStrMaxLen);
    ascIILen = 0;

    /* 7bit转换为8bit */
    if (nameIndicator->dcs == TAF_CALL_DCS_7BIT) {
        /* 7bit转换为8bit */
        if (nameStrMaxLen < nameIndicator->length) {
            AT_WARN_LOG("AT_CnapConvertNameStr: NameStr Space Too Short!");
            return;
        }

        if (TAF_STD_UnPack7Bit(nameIndicator->nameStr, nameIndicator->length, 0, nameStr) != VOS_OK) {
            AT_WARN_LOG("AT_CnapConvertNameStr: TAF_STD_UnPack7Bit Err!");
            return;
        }

        /* Default Alphabet转换为ASCII码 */
        TAF_STD_ConvertDefAlphaToAscii(nameStr, nameIndicator->length, nameStr, &ascIILen);

        if (ascIILen < nameIndicator->length) {
            AT_WARN_LOG("AT_CnapConvertNameStr: TAF_STD_ConvertDefAlphaToAscii Err!");
            return;
        }
    } else {
        if (nameIndicator->length > 0) {
            memResult = memcpy_s(nameStr, nameStrMaxLen, nameIndicator->nameStr,
                                 TAF_MIN(nameIndicator->length, TAF_CALL_CNAP_NAME_STR_MAX_LENGTH));
            TAF_MEM_CHK_RTN_VAL(memResult, nameStrMaxLen,
                                TAF_MIN(nameIndicator->length, TAF_CALL_CNAP_NAME_STR_MAX_LENGTH));
        }
    }

    return;
}


VOS_UINT32 AT_RcvTafCcmCnapQryCnf(struct MsgCB *msg)
{
    TAF_CCM_CnapQryCnf *cnapQryCnf = VOS_NULL_PTR;
    VOS_UINT16          length;
    VOS_UINT8           indexNum;
    VOS_UINT8           nameStr[AT_PARA_CNAP_MAX_NAME_LEN];

    indexNum = 0;
    length   = 0;

    cnapQryCnf = (TAF_CCM_CnapQryCnf *)msg;

    /* 通过ClientId获取ucIndex */
    if (At_ClientIdToUserId(cnapQryCnf->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafCallCnapQryCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    /* 广播消息不处理 */
    if (AT_IS_BROADCAST_CLIENT_INDEX(indexNum)) {
        AT_WARN_LOG("AT_RcvTafCallCnapQryCnf: WARNING:AT_BROADCAST_INDEX!");
        return VOS_ERR;
    }

    /* 判断当前操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CNAPEX_QRY) {
        AT_WARN_LOG("AT_RcvTafCallCnapQryCnf: WARNING:Not AT_CMD_CNAPEX_QRY!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(indexNum);

    if (cnapQryCnf->nameIndicator.existFlag == VOS_TRUE) {
        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: \"", g_parseContext[indexNum].cmdElement->cmdName);

        if ((cnapQryCnf->nameIndicator.length > 0) &&
            (cnapQryCnf->nameIndicator.length < TAF_CALL_CNAP_NAME_STR_MAX_LENGTH) &&
            (cnapQryCnf->nameIndicator.dcs < TAF_CALL_DCS_BUTT)) {
            /* UCS2格式直接打印输出 */
            if (cnapQryCnf->nameIndicator.dcs == TAF_CALL_DCS_UCS2) {
                if ((cnapQryCnf->nameIndicator.length * AT_DOUBLE_LENGTH) < TAF_CALL_CNAP_NAME_STR_MAX_LENGTH) {
                    length += (VOS_UINT16)At_Unicode2UnicodePrint(AT_CMD_MAX_LEN, (VOS_INT8 *)g_atSndCodeAddress,
                                                              g_atSndCodeAddress + length,
                                                              cnapQryCnf->nameIndicator.nameStr,
                                                              cnapQryCnf->nameIndicator.length * AT_DOUBLE_LENGTH);
                }
            } else {
                AT_CnapConvertNameStr(&cnapQryCnf->nameIndicator, nameStr, AT_PARA_CNAP_MAX_NAME_LEN);

                /* AscII转换为UCS2并打印输出 */
                length += (TAF_UINT16)At_Ascii2UnicodePrint(AT_CMD_MAX_LEN, (VOS_INT8 *)g_atSndCodeAddress,
                                                            g_atSndCodeAddress + length, nameStr,
                                                            (VOS_UINT16)VOS_StrLen((VOS_CHAR *)nameStr));
            }
        }

        /* <CNI_Validity> */
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "\",%d", cnapQryCnf->nameIndicator.cniValidity);
    }

    g_atSendDataBuff.bufLen = length;

    At_FormatResultData(indexNum, AT_OK);

    return VOS_OK;
}


VOS_UINT32 AT_SetCnapPara(VOS_UINT8 indexNum)
{
    AT_ModemSsCtx *ssCtx = VOS_NULL_PTR;

    ssCtx = AT_GetModemSsCtxAddrFromClientId(indexNum);

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数过多 */
    if (g_atParaIndex > 1) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数不为空 */
    if (g_atParaList[0].paraLen != 0) {
        ssCtx->cnapType = (AT_CnapTypeUint8)g_atParaList[0].paraValue;
    } else {
        ssCtx->cnapType = AT_CNAP_DISABLE_TYPE;
    }

    return AT_OK;
}


TAF_UINT32 AT_QryCnapPara(VOS_UINT8 indexNum)
{
    TAF_SS_InterrogatessReq cnapQry;

    /* 初始化 */
    (VOS_VOID)memset_s(&cnapQry, (VOS_SIZE_T)sizeof(cnapQry), 0x00, (VOS_SIZE_T)sizeof(cnapQry));

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 设置SsCode */
    cnapQry.ssCode = TAF_CNAP_SS_CODE;

    if (TAF_InterrogateSSReq(g_atClientTab[indexNum].clientId, 0, &cnapQry) == AT_SUCCESS) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CNAP_QRY;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    } else {
        return AT_ERROR;
    }
}


TAF_VOID AT_SsRspInterrogateCnfCnapProc(TAF_UINT8 indexNum, TAF_SS_CallIndependentEvent *event, TAF_UINT32 *result,
                                        TAF_UINT16 *length)
{
    AT_ModemSsCtx *ssCtx = VOS_NULL_PTR;
    TAF_UINT8      tmp   = 0;

    ssCtx = AT_GetModemSsCtxAddrFromClientId(indexNum);

    /* 需要首先判断错误码 */
    if (event->opError == 1) {
        *result = At_ChgTafErrorCode(indexNum, event->errorCode);
        return;
    }

    /* 判断当前操作类型 */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CNAP_QRY) {
        AT_WARN_LOG("AT_SsRspInterrogateCnfCnapProc: WARNING: Not AT_CMD_CNAP_QRY!");
        return;
    }

    /* 查到状态 */
    if (event->opSsStatus == 1) {
        tmp = (TAF_SS_PROVISIONED_STATUS_MASK & event->ssStatus) ? 1 : 0;
    }
    /* 没有查到状态 */
    else {
        tmp = AT_CMD_CNAP_NO_OP_SS_STATUS;
    }

    /* +CNAP: <n>,<m> */
    *length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + *length, "%s: %d,%d", g_parseContext[indexNum].cmdElement->cmdName,
        ssCtx->cnapType, tmp);

    *result = AT_OK;
    return;
}


VOS_VOID AT_ReportCnapInfo(VOS_UINT8 indexNum, TAF_CALL_Cnap *nameIndicator)
{
    AT_ModemSsCtx *ssCtx = VOS_NULL_PTR;
    VOS_UINT8      dstName[AT_PARA_CNAP_MAX_NAME_LEN];

    ssCtx = AT_GetModemSsCtxAddrFromClientId(indexNum);
    (VOS_VOID)memset_s(dstName, (VOS_SIZE_T)sizeof(dstName), 0, (VOS_SIZE_T)sizeof(dstName));

    if ((ssCtx->cnapType != AT_CNAP_ENABLE_TYPE) || (nameIndicator->existFlag == VOS_FALSE)) {
        return;
    }

    /* 7bit和8bit转换后上报 */
    if ((nameIndicator->dcs == TAF_CALL_DCS_7BIT) || (nameIndicator->dcs == TAF_CALL_DCS_8BIT)) {
        AT_CnapConvertNameStr(nameIndicator, dstName, AT_PARA_CNAP_MAX_NAME_LEN);
    } else {
        AT_NORM_LOG1("AT_ReportCnapInfo: Dcs:", nameIndicator->dcs);
    }

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s+CNAP: \"%s\",%d%s", g_atCrLf, (VOS_CHAR *)dstName,
        nameIndicator->cniValidity, g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, g_atSendDataBuff.bufLen);
    return;
}

VOS_UINT32 AT_RcvTafCcmCnapInfoInd(struct MsgCB *msg)
{
    TAF_CCM_CnapInfoInd *cnapInfoInd = VOS_NULL_PTR;
    VOS_UINT8            indexNum    = 0;

    cnapInfoInd = (TAF_CCM_CnapInfoInd *)msg;

    /* 通过clientid获取index */
    if (At_ClientIdToUserId(cnapInfoInd->ctrl.clientId, &indexNum) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafCallCnapInfoInd:WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    AT_ReportCnapInfo(indexNum, &cnapInfoInd->nameIndicator);

    return VOS_OK;
}

