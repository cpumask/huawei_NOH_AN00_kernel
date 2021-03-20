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

#include "AtCmdSimProc.h"
#include "securec.h"
#include "AtEventReport.h"
#include "nv_stru_sys.h"
#include "nv_stru_pam.h"


/*
 * 协议栈打印打点方式下的.C文件宏定义
 */
#define THIS_FILE_ID PS_FILE_ID_AT_CMD_SIM_PROC_C

#define AT_SIMSLOT_MAX_PARA_NUM 3
#define AT_SIMSLOT_MODEM0 0
#define AT_SIMSLOT_MODEM1 1
#define AT_SIMSLOT_MODEM2 2
#define AT_SIMSLOT_MULTI_MODEM_CARD_NUM 3
#define AT_SIMSLOT_OTHER_MODEM_CARD_NUM 2

#define AT_SCICHG_MAX_PARA_NUM 3
#define AT_SCICHG_MODEM0 0
#define AT_SCICHG_MODEM1 1
#define AT_SCICHG_MODEM2 2

#define AT_CRLA_MAX_PARA_NUM 8
#define AT_CRLA_PARA_GETRSP_MIN_NUM 2
#define AT_CRLA_PARA_UPDATE_BINARY_MIN_NUM 6
#define AT_CRLA_PARA_SEARCH_RECORD_MIN_NUM 7
#define AT_CRLA_COMMAND 1
#define AT_CRLA_FILEID 2
#define AT_CRLA_P1 3
#define AT_CRLA_P2 4
#define AT_CRLA_P3 5
#define AT_CRLA_DATA 6
#define AT_CRLA_PATHID 7

#define AT_CGLA_PARA_NUM 3
#define AT_CGLA_SESSIONID 0
#define AT_CGLA_LENGTH 1
#define AT_CGLA_COMMAND 2

#define AT_HVSST_PARA_NUM 2
#define AT_SILENTPIN_PARA_NUM 3
#define AT_SILENTPIN_PARA_CRYTOPIN  0 /* SERDESTESTASYNC的第一个参数CRYTOPIN */
#define AT_SILENTPIN_PARA_PINIV     1 /* SERDESTESTASYNC的第二个参数PINIV */
#define AT_SILENTPIN_PARA_HMACVALUE 2 /* SERDESTESTASYNC的第三个参数HMACVALUE */
#define AT_ESIMSWITCH_PARA_NUM 2


VOS_UINT32 At_IsSimSlotAllowed(VOS_UINT32 modem0Slot, VOS_UINT32 modem1Slot, VOS_UINT32 modem2Slot)
{
#if (MULTI_MODEM_NUMBER == 3)
    /* 三Modem,配置单卡模式 */
    if ((modem1Slot == modem2Slot) && (modem1Slot == SI_PIH_CARD_SLOT_2) && (modem0Slot <= SI_PIH_CARD_SLOT_1)) {
        return VOS_TRUE;
    }
    /* 三Modem,任意两个Modem不能同时配置为同一卡槽 */
    if ((modem0Slot == modem1Slot) || (modem1Slot == modem2Slot) || (modem0Slot == modem2Slot)) {
        return VOS_FALSE;
    }
#else
    /* 双Modem,两个Modem不能同时配置为同一卡槽 */
    if (modem0Slot == modem1Slot) {
        return VOS_FALSE;
    }
#endif

    return VOS_TRUE;
}


VOS_UINT32 At_SetSIMSlotPara(VOS_UINT8 indexNum)
{
#if (MULTI_MODEM_NUMBER != 1)
    TAF_NvSciCfg sCICfg;
#endif
    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数过多 */
    if (g_atParaIndex > AT_SIMSLOT_MAX_PARA_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数检查 */
    if ((g_atParaList[AT_SIMSLOT_MODEM0].paraLen == 0) || (g_atParaList[AT_SIMSLOT_MODEM1].paraLen == 0)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 单modem不支持切换卡槽 */
#if (MULTI_MODEM_NUMBER == 1)
    return AT_CME_OPERATION_NOT_ALLOWED;
#else

    /* 三卡形态第3个参数不能为空，其余形态默认为卡槽2 */
#if (MULTI_MODEM_NUMBER == 3)
    if (g_atParaList[AT_SIMSLOT_MODEM2].paraLen == 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }
#else
    g_atParaList[AT_SIMSLOT_MODEM2].paraValue = SI_PIH_CARD_SLOT_2;
#endif

    /* 参数检查 */
    if (At_IsSimSlotAllowed(g_atParaList[AT_SIMSLOT_MODEM0].paraValue, g_atParaList[AT_SIMSLOT_MODEM1].paraValue,
                            g_atParaList[AT_SIMSLOT_MODEM2].paraValue) == VOS_FALSE) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 从NV中读取当前SIM卡的SCI配置 */
    (VOS_VOID)memset_s(&sCICfg, sizeof(sCICfg), 0x00, sizeof(sCICfg));

    if (TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_SCI_DSDA_CFG, &sCICfg, sizeof(sCICfg)) != NV_OK) {
        AT_ERR_LOG("At_SetSIMSlotPara: NV_ITEM_SCI_DSDA_CFG read fail!");
        return AT_ERROR;
    }

    /*
     * 根据用户设置的值修改card0位和card1位的值，在NV项中，这两项对应的bit位和取值含义如下:
     * card0: bit[8-10]：卡槽0使用的SCI接口
     *     0：使用SCI0（默认值）
     *     1：使用SCI1
     *     2：使用SCI2
     *     其余值：无效
     * card1:bit[11-13]：卡1槽使用的SCI接口
     *     0：使用SCI0
     *     1：使用SCI1（默认值）
     *     2：使用SCI2
     *     其余值：无效
     * card2:bit[14-16]：卡2槽使用的SCI接口
     *     0：使用SCI0
     *     1：使用SCI1
     *     2：使用SCI2（默认值）
     *     其余值：无效
     */
    sCICfg.bitCard0 = g_atParaList[AT_SIMSLOT_MODEM0].paraValue;
    sCICfg.bitCard1 = g_atParaList[AT_SIMSLOT_MODEM1].paraValue;

    /* 针对双卡形态增加保护，bitCard2使用NV默认值，与底软处理适配 */
#if (MULTI_MODEM_NUMBER == 3)
    sCICfg.bitCard2   = g_atParaList[AT_SIMSLOT_MODEM2].paraValue;
    sCICfg.bitCardNum = AT_SIMSLOT_MULTI_MODEM_CARD_NUM;
#else
    sCICfg.bitCardNum = AT_SIMSLOT_OTHER_MODEM_CARD_NUM;
#endif

    sCICfg.bitReserved0 = 0;
    sCICfg.bitReserved1 = 0;

    /* 将设置的SCI值保存到NV中 */
    if (TAF_ACORE_NV_WRITE(MODEM_ID_0, NV_ITEM_SCI_DSDA_CFG, (VOS_UINT8 *)&sCICfg, sizeof(sCICfg)) != NV_OK) {
        AT_ERR_LOG("At_SetSIMSlotPara: NV_ITEM_SCI_DSDA_CFG write failed");
        return AT_ERROR;
    }

    return AT_OK;
#endif
}


VOS_UINT32 At_QrySIMSlotPara(VOS_UINT8 indexNum)
{
    TAF_NvSciCfg sCICfg;
    VOS_UINT16   length;

    /* 从NV中读取当前SIM卡的SCI配置 */
    (VOS_VOID)memset_s(&sCICfg, sizeof(sCICfg), 0x00, sizeof(sCICfg));

    if (TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_SCI_DSDA_CFG, &sCICfg, sizeof(sCICfg)) != NV_OK) {
        AT_ERR_LOG("At_QrySIMSlotPara: NV_ITEM_SCI_DSDA_CFG read fail!");
        g_atSendDataBuff.bufLen = 0;
        return AT_ERROR;
    }

    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: %d,%d", g_parseContext[indexNum].cmdElement->cmdName, sCICfg.bitCard0,
        sCICfg.bitCard1);

#if (MULTI_MODEM_NUMBER == 3)
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", sCICfg.bitCard2);
#endif

    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}


VOS_UINT32 At_SetHvsstPara(VOS_UINT8 indexNum)
{
    VOS_UINT32      result;
    SI_PIH_HvsstSet hvSStSet;

    /* 命令类型检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数过多 */
    if (g_atParaIndex != AT_HVSST_PARA_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数检查 */
    if ((g_atParaList[0].paraLen == 0) || (g_atParaList[1].paraLen == 0)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    (VOS_VOID)memset_s(&hvSStSet, sizeof(hvSStSet), 0x00, sizeof(hvSStSet));

    hvSStSet.index  = (VOS_UINT8)g_atParaList[0].paraValue;
    hvSStSet.simSet = (VOS_UINT8)g_atParaList[1].paraValue;

    result = SI_PIH_HvSstSet(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId, &hvSStSet);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("At_SetHvsstPara: SI_PIH_HvSstSet fail.");
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_HVSST_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 At_SetPassThrough(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if ((g_atParaIndex != 1) || (g_atParaList[0].paraLen != 1)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    result = SI_PIH_PassThroughSet(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
        g_atParaList[0].paraValue);
    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("At_SetPassThrough: SI_PIH_PassThroughSet fail");
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_PASSTHROUGH_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 At_SetPassThroughCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length)
{
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_PASSTHROUGH_SET) {
        AT_WARN_LOG("At_SetPassThroughCnf: CmdCurrentOpt is not AT_CMD_PASSTHROUGH_SET");
        return AT_ERROR;
    }

    return AT_OK;
}


VOS_UINT32 At_QryPassThrough(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    result = SI_PIH_PassThroughQuery(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId);
    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("At_QryPassThrough: SI_PIH_PassThroughQuery fail");
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_PASSTHROUGH_QRY;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 At_QryPassThroughCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length)
{
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_PASSTHROUGH_QRY) {
        AT_WARN_LOG("At_QryPassThroughCnf: CmdCurrentOpt is not AT_CMD_PASSTHROUGH_QRY");
        return AT_ERROR;
    }

    (*length) += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
        event->pihEvent.passThroughState);

    return AT_OK;
}

#if ((FEATURE_SINGLEMODEM_DUALSLOT == FEATURE_ON) && (MULTI_MODEM_NUMBER == 1))
/*
 * 功能描述: (AT^SINGLEMODEMDUALSLOT)单MODEM，多卡槽场景设置卡槽与USIM任务对应关系
 */
VOS_UINT32 At_SetSingleModemDualSlot(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if ((g_atParaIndex != 1) || (g_atParaList[0].paraLen != 1)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    result = SI_PIH_SingleModemDualSlotSet(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
        g_atParaList[0].paraValue);
    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("At_SetSingleModemDualSlot: SI_PIH_SingleModemDualSlotSet fail");
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_SINGLEMODEMDUALSLOT_SET;

    return AT_WAIT_ASYNC_RETURN;
}

/*
 * 功能描述: (AT^SINGLEMODEMDUALSLOT)状态查询
 */
VOS_UINT32 At_QrySingleModemDualSlot(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    result = SI_PIH_SingleModemDualSlotQuery(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId);
    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("At_QrySingleModemDualSlot: SI_PIH_SingleModemDualSlotQuery fail");
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_SINGLEMODEMDUALSLOT_QRY;

    return AT_WAIT_ASYNC_RETURN;
}

/*
 * 功能描述: SI_PIH_EVENT_SINGLEMODEMDUALSLOT_SET_CNF的处理
 */
VOS_UINT32 At_SetSingleModemDualSlotCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length)
{
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_SINGLEMODEMDUALSLOT_SET) {
        AT_WARN_LOG("At_SetSingleModemDualSlotCnf: CmdCurrentOpt is not AT_CMD_SINGLEMODEMDUALSLOT_SET");
        return AT_ERROR;
    }

    return AT_OK;
}

/*
 * 功能描述: SI_PIH_EVENT_SINGLEMODEMDUALSLOT_QUERY_CNF的处理
 */
VOS_UINT32 At_QrySingleModemDualSlotCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length)
{
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_SINGLEMODEMDUALSLOT_QRY) {
        AT_WARN_LOG("At_QrySingleModemDualSlotCnf: CmdCurrentOpt is not AT_CMD_SINGLEMODEMDUALSLOT_QRY");
        return AT_ERROR;
    }

    (*length) += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
        event->pihEvent.singleModemSlotCfg);

    return AT_OK;
}
#endif

#if ((FEATURE_PHONE_SC == FEATURE_ON) && (FEATURE_PHONE_USIM == FEATURE_ON))

VOS_UINT32 At_SetSilentPin(VOS_UINT8 indexNum)
{
    errno_t          memResult;
    VOS_UINT32       result;
    VOS_UINT16       length;
    SI_PIH_CryptoPin cryptoPin;

    /* 命令类型检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_WARN_LOG("At_SetSilentPinInfo: CmdOptType fail.");

        return AT_CME_INCORRECT_PARAMETERS;
    }

    /*  参数不为3 */
    if (g_atParaIndex != AT_SILENTPIN_PARA_NUM) {
        AT_WARN_LOG1("At_SetSilentPinInfo: para num  %d.", g_atParaIndex);

        return AT_CME_INCORRECT_PARAMETERS;
    }

    if ((g_atParaList[AT_SILENTPIN_PARA_CRYTOPIN].paraLen != (DRV_AGENT_PIN_CRYPTO_DATA_LEN * 2)) ||
        (g_atParaList[AT_SILENTPIN_PARA_PINIV].paraLen != (DRV_AGENT_PIN_CRYPTO_IV_LEN * 2)) ||
        (g_atParaList[AT_SILENTPIN_PARA_HMACVALUE].paraLen != (DRV_AGENT_HMAC_DATA_LEN * 2))) {
        AT_WARN_LOG2("At_SetSilentPinInfo: 0 %d %d.", g_atParaList[AT_SILENTPIN_PARA_CRYTOPIN].paraLen,
                     g_atParaList[AT_SILENTPIN_PARA_PINIV].paraLen);

        return AT_CME_INCORRECT_PARAMETERS;
    }

    (VOS_VOID)memset_s(&cryptoPin, sizeof(cryptoPin), 0, sizeof(cryptoPin));

    /* 将密文PIN字符串参数转换为码流 */
    length = g_atParaList[AT_SILENTPIN_PARA_CRYTOPIN].paraLen;
    result = At_AsciiNum2HexString(g_atParaList[AT_SILENTPIN_PARA_CRYTOPIN].para, &length);
    if ((result != AT_SUCCESS) || (length != DRV_AGENT_PIN_CRYPTO_DATA_LEN)) {
        AT_WARN_LOG1("At_SetSilentPinInfo: Encpin fail %d.", length);

        return AT_CME_INCORRECT_PARAMETERS;
    }

    memResult = memcpy_s(cryptoPin.cryptoPin, DRV_AGENT_PIN_CRYPTO_DATA_LEN,
                         g_atParaList[AT_SILENTPIN_PARA_CRYTOPIN].para, DRV_AGENT_PIN_CRYPTO_DATA_LEN);
    TAF_MEM_CHK_RTN_VAL(memResult, DRV_AGENT_PIN_CRYPTO_DATA_LEN, DRV_AGENT_PIN_CRYPTO_DATA_LEN);

    /* 将IV字符串参数转换为码流 */
    length = g_atParaList[AT_SILENTPIN_PARA_PINIV].paraLen;
    result = At_AsciiNum2HexString(g_atParaList[AT_SILENTPIN_PARA_PINIV].para, &length);
    if ((result != AT_SUCCESS) || (length != DRV_AGENT_PIN_CRYPTO_IV_LEN)) {
        AT_WARN_LOG1("At_SetSilentPinInfo: IV Len fail %d.", length);

        return AT_CME_INCORRECT_PARAMETERS;
    }

    memResult = memcpy_s(cryptoPin.pinIv, DRV_AGENT_PIN_CRYPTO_IV_LEN, g_atParaList[AT_SILENTPIN_PARA_PINIV].para,
                         DRV_AGENT_PIN_CRYPTO_IV_LEN);
    TAF_MEM_CHK_RTN_VAL(memResult, DRV_AGENT_PIN_CRYPTO_IV_LEN, DRV_AGENT_PIN_CRYPTO_IV_LEN);

    /* 将HMAC字符串参数转换为码流 */
    length = g_atParaList[AT_SILENTPIN_PARA_HMACVALUE].paraLen;
    result = At_AsciiNum2HexString(g_atParaList[AT_SILENTPIN_PARA_HMACVALUE].para, &length);
    if ((result != AT_SUCCESS) || (length != DRV_AGENT_HMAC_DATA_LEN)) {
        AT_WARN_LOG1("At_SetSilentPinInfo: hmac Len fail %d.", length);

        return AT_CME_INCORRECT_PARAMETERS;
    }

    memResult = memcpy_s(cryptoPin.hmacValue, DRV_AGENT_HMAC_DATA_LEN,
                         g_atParaList[AT_SILENTPIN_PARA_HMACVALUE].para, DRV_AGENT_HMAC_DATA_LEN);
    TAF_MEM_CHK_RTN_VAL(memResult, DRV_AGENT_HMAC_DATA_LEN, DRV_AGENT_HMAC_DATA_LEN);

    result = SI_PIH_SetSilentPinReq(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId, &cryptoPin);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("At_SetSilentPinInfo: SI_PIH_SetSilentPinReq fail.");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_SILENTPIN_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_VOID At_ClearPIN(VOS_UINT8 *mem, VOS_UINT8 len)
{
    VOS_UINT32 i;

    for (i = 0; i < len; i++) {
        mem[i] = 0XFF;
    }

    return;
}


VOS_UINT32 At_SetSilentPinInfo(VOS_UINT8 indexNum)
{
    VOS_UINT8  pin[TAF_PH_PINCODELENMAX + 1] = {0};
    VOS_UINT32 result;
    errno_t    memResult;

    /* 命令类型检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数不为1 */
    if (g_atParaIndex != 1) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数检查 */
    if ((g_atParaList[0].paraLen > TAF_PH_PINCODELENMAX) || (g_atParaList[0].paraLen < TAF_PH_PINCODELENMIN)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    (VOS_VOID)memset_s(pin, TAF_PH_PINCODELENMAX, 0xFF, TAF_PH_PINCODELENMAX);

    memResult = memcpy_s(pin, TAF_PH_PINCODELENMAX, g_atParaList[0].para, g_atParaList[0].paraLen);
    TAF_MEM_CHK_RTN_VAL(memResult, TAF_PH_PINCODELENMAX, g_atParaList[0].paraLen);

    result = SI_PIH_GetSilentPinInfoReq(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId, pin,
                                        TAF_PH_PINCODELENMAX);
    /* 敏感信息置0xFF */
    At_ClearPIN(pin, TAF_PH_PINCODELENMAX);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("At_SetSilentPinInfo: SI_PIH_SetSilentPinReq fail.");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_SILENTPININFO_SET;

    return AT_WAIT_ASYNC_RETURN;
}
#endif


VOS_UINT32 At_SetEsimCleanProfile(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    /* 命令类型检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_CMD_NO_PARA) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    result = SI_EMAT_EsimCleanProfile(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("At_SetEsimCleanProfile: Set Esim Clean fail.");

        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_ESIMCLEAN_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 At_QryEsimCheckProfile(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    /* 命令类型检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    result = SI_EMAT_EsimCheckProfile(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("At_QryEsimCheckProfile: Set Esim Switch fail.");

        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_ESIMCHECK_QRY;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 At_QryEsimEid(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    /* 命令类型检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    result = SI_EMAT_EsimEidQuery(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("At_QryEsimEid: Get eSIM EID fail.");

        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_ESIMEID_QRY;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 At_QryEsimPKID(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    /* 命令类型检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    result = SI_EMAT_EsimPKIDQuery(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("At_QryEsimPKID: Get eSIM PKID fail.");

        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_ESIMPKID_QRY;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 At_SetEsimSwitchPara(VOS_UINT8 indexNum)
{
    VOS_UINT32           result;
    SI_PIH_EsimSwitchSet esimSwitchSet = {0};

    /* 命令类型检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数过多 */
    if (g_atParaIndex != AT_ESIMSWITCH_PARA_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数检查 */
    if ((g_atParaList[0].paraLen == 0) || (g_atParaList[1].paraLen == 0)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    esimSwitchSet.slot     = (VOS_UINT8)g_atParaList[0].paraValue;
    esimSwitchSet.cardType = (VOS_UINT8)g_atParaList[1].paraValue;

    result = SI_PIH_EsimSwitchSetFunc(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId, &esimSwitchSet);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("At_SetEsimSwitchPara: Set Esim Switch fail.");

        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_ESIMSWITCH_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 At_QryEsimSwitch(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    result = SI_PIH_EsimSwitchQuery(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("At_QryEsimSwitch: Qry Esim Switch fail.");

        return AT_ERROR;
    }

    /* 设置当前操作类型 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_ESIMSWITCH_QRY;

    /* 返回命令处理挂起状态 */
    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 At_QryHvsstPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    result = SI_PIH_HvSstQuery(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("AT_QryPortAttribSetPara: SI_PIH_HvSstQuery fail.");
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_HVSST_QRY;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 At_HvsstQueryCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length)
{
    ModemIdUint16        modemId;
    VOS_UINT32           rslt;
    TAF_NvSciCfg         sCICfg;
    VOS_UINT32           slot;
    SI_PIH_SimIndexUint8 simIndex;

    rslt = AT_GetModemIdFromClient(indexNum, &modemId);

    if (rslt != VOS_OK) {
        AT_ERR_LOG("At_HvsstQueryCnf: Get modem id fail.");
        return AT_ERROR;
    }

    /* 判断当前操作类型是否为AT_CMD_HVSST_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_HVSST_QRY) {
        AT_WARN_LOG("At_HvsstQueryCnf : CmdCurrentOpt is not AT_CMD_HVSST_QRY!");
        return AT_ERROR;
    }

    /* 从NV中读取当前SIM卡的SCI配置 */
    (VOS_VOID)memset_s(&sCICfg, sizeof(sCICfg), 0x00, sizeof(sCICfg));

    if (TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_SCI_DSDA_CFG, &sCICfg, sizeof(sCICfg)) != NV_OK) {
        AT_ERR_LOG("At_HvsstQueryCnf: NV_ITEM_SCI_DSDA_CFG read fail!");
        return AT_ERROR;
    }

    if (modemId == MODEM_ID_0) {
        slot = sCICfg.bitCard0;
    } else if (modemId == MODEM_ID_1) {
        slot = sCICfg.bitCard1;
    } else {
        slot = sCICfg.bitCard2;
    }

    if (event->pihEvent.hvsstQueryCnf.vSimState == SI_PIH_SIM_ENABLE) {
        simIndex = SI_PIH_SIM_VIRT_SIM1;
    } else {
        simIndex = SI_PIH_SIM_REAL_SIM1;
    }

    (*length) += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + (*length), "%s: %d,%d,%d,%d", g_parseContext[indexNum].cmdElement->cmdName,
        simIndex, 1, slot, event->pihEvent.hvsstQueryCnf.cardUse);

    return AT_OK;
}


VOS_UINT32 At_SetSciChgPara(VOS_UINT8 indexNum)
{
#if (MULTI_MODEM_NUMBER != 1)
    VOS_UINT32 result;
#endif
    /* 命令类型检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数过多 */
    if (g_atParaIndex > AT_SCICHG_MAX_PARA_NUM) {
        return AT_TOO_MANY_PARA;
    }

    /* 参数检查 */
    if ((g_atParaList[AT_SCICHG_MODEM0].paraLen == 0) || (g_atParaList[AT_SCICHG_MODEM1].paraLen == 0)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 单modem不支持卡槽切换 */
#if (MULTI_MODEM_NUMBER == 1)
    return AT_CME_OPERATION_NOT_ALLOWED;
#else

    /* 三卡形态第3个参数不能为空，其余形态默认为卡槽2 */
#if (MULTI_MODEM_NUMBER == 3)
    if (g_atParaList[AT_SCICHG_MODEM2].paraLen == 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 任意两个Modem不能同时配置为同一卡槽 */
    if ((g_atParaList[AT_SCICHG_MODEM0].paraValue == g_atParaList[AT_SCICHG_MODEM1].paraValue) ||
        (g_atParaList[AT_SCICHG_MODEM0].paraValue == g_atParaList[AT_SCICHG_MODEM2].paraValue) ||
        (g_atParaList[AT_SCICHG_MODEM1].paraValue == g_atParaList[AT_SCICHG_MODEM2].paraValue)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }
#else
    g_atParaList[AT_SCICHG_MODEM2].paraValue = SI_PIH_CARD_SLOT_2;

    /* 任意两个Modem不能同时配置为同一卡槽 */
    if (g_atParaList[AT_SCICHG_MODEM0].paraValue == g_atParaList[AT_SCICHG_MODEM1].paraValue) {
        return AT_CME_INCORRECT_PARAMETERS;
    }
#endif

    result = SI_PIH_SciCfgSet(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                              g_atParaList[AT_SCICHG_MODEM0].paraValue, g_atParaList[AT_SCICHG_MODEM1].paraValue,
                              g_atParaList[AT_SCICHG_MODEM2].paraValue);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("At_SetSciChgPara: SI_PIH_HvSstSet fail.");
        return AT_CME_PHONE_FAILURE;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_SCICHG_SET;

    return AT_WAIT_ASYNC_RETURN;
#endif
}


VOS_UINT32 At_SetBwtPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    /* 命令类型检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数过多 */
    if (g_atParaIndex != 1) {
        return AT_TOO_MANY_PARA;
    }

    /* 参数检查 */
    if (g_atParaList[0].paraLen == 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    result = SI_PIH_BwtSet(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                           (VOS_UINT16)g_atParaList[0].paraValue);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("At_SetBwtPara: SI_PIH_HvSstSet fail.");

        return AT_CME_PHONE_FAILURE;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_BWT_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 At_QrySciChgPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    result = SI_PIH_SciCfgQuery(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("At_QrySciChgPara: SI_PIH_SciCfgQuery fail.");
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_SCICHG_QRY;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 At_SciCfgQueryCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length)
{
    /* 判断当前操作类型是否为AT_CMD_SCICHG_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_SCICHG_QRY) {
        AT_WARN_LOG("At_SciCfgQueryCnf : CmdCurrentOpt is not AT_CMD_SCICHG_QRY!");
        return AT_ERROR;
    }

    (*length) += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: %d,%d", g_parseContext[indexNum].cmdElement->cmdName,
        event->pihEvent.sciCfgCnf.card0Slot, event->pihEvent.sciCfgCnf.card1Slot);

#if (MULTI_MODEM_NUMBER == 3)
    (*length) += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + (*length), ",%d", event->pihEvent.sciCfgCnf.card2Slot);
#endif

    return AT_OK;
}


VOS_UINT32 At_ProcPihFndBndCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length)
{
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CLCK_PIN_HANDLE) {
        AT_WARN_LOG("At_ProcPihFndBndCnf : CmdCurrentOpt is not AT_CMD_CLCK_PIN_HANDLE!");
        return AT_ERROR;
    }

    /* 如果是状态查询命令 */
    if (event->pihEvent.fdnCnf.fdnCmd == SI_PIH_FDN_BDN_QUERY) {
        (*length) += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + (*length), "%s", g_atCrLf);
        (*length) += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + (*length), "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
        (*length) += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + (*length), "%d", event->pihEvent.fdnCnf.fdnState);
    }

    return AT_OK;
}


VOS_UINT32 At_ProcPihGenericAccessCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length)
{
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CSIM_SET) {
        AT_WARN_LOG("At_ProcPihGenericAccessCnf : CmdCurrentOpt is not AT_CMD_CSIM_SET!");
        return AT_ERROR;
    }

    (*length) += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + (*length), "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
    /* <length>, */
    (*length) += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + (*length), "%d,\"",
        (event->pihEvent.gAccessCnf.len + AT_DOUBLE_QUO_MARKS_LENGTH) * AT_DOUBLE_LENGTH);
    if (event->pihEvent.gAccessCnf.len != 0) {
        /* <command>, */
        (*length) += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                         (TAF_UINT8 *)g_atSndCodeAddress + (*length),
                                                         event->pihEvent.gAccessCnf.command,
                                                         event->pihEvent.gAccessCnf.len);
    }
    /* SW1 */
    (*length) += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                     (TAF_UINT8 *)g_atSndCodeAddress + (*length),
                                                     &event->pihEvent.gAccessCnf.sw1, sizeof(TAF_UINT8));
    /* SW1 */
    (*length) += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                     (TAF_UINT8 *)g_atSndCodeAddress + (*length),
                                                     &event->pihEvent.gAccessCnf.sw2, sizeof(TAF_UINT8));
    (*length) += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                                                   (TAF_CHAR *)g_atSndCodeAddress + (*length), "\"");

    return AT_OK;
}


VOS_UINT32 At_ProcPihIsdbAccessCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length)
{
    /* 判断当前操作类型是否为AT_CMD_CISA_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CISA_SET) {
        AT_WARN_LOG("At_ProcPihIsdbAccessCnf : CmdCurrentOpt is not AT_CMD_CISA_SET!");
        return AT_ERROR;
    }

    event->pihEvent.isdbAccessCnf.len = TAF_MIN(event->pihEvent.isdbAccessCnf.len, SI_APDU_MAX_LEN);

    (*length) += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + (*length), "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

    /* <length>, */
    (*length) += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + (*length), "%d,\"",
        (event->pihEvent.isdbAccessCnf.len + AT_DOUBLE_QUO_MARKS_LENGTH) * AT_DOUBLE_LENGTH);
    if (event->pihEvent.isdbAccessCnf.len != 0) {
        /* <command>, */
        (*length) += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                         (TAF_UINT8 *)g_atSndCodeAddress + (*length),
                                                         event->pihEvent.isdbAccessCnf.command,
                                                         event->pihEvent.isdbAccessCnf.len);
    }

    /* SW1 */
    (*length) += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                     (TAF_UINT8 *)g_atSndCodeAddress + (*length),
                                                     &event->pihEvent.isdbAccessCnf.sw1, sizeof(TAF_UINT8));

    /* SW2 */
    (*length) += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                     (TAF_UINT8 *)g_atSndCodeAddress + (*length),
                                                     &event->pihEvent.isdbAccessCnf.sw2, sizeof(TAF_UINT8));
    (*length) += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + (*length), "\"");

    return AT_OK;
}


VOS_UINT32 At_ProcPihCchoSetCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length)
{
    /* 判断当前操作类型是否为AT_CMD_CCHO_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CCHO_SET) {
        AT_WARN_LOG("At_ProcPihCchoSetCnf : CmdCurrentOpt is not AT_CMD_CCHO_SET!");
        return AT_ERROR;
    }

    (*length) += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + (*length), "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
    /* <sessionid>, */
    (*length) += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + (*length), "%u", event->pihEvent.openChannelCnf.sessionID);

    return AT_OK;
}


VOS_UINT32 At_ProcPihCchpSetCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length)
{
    /* 判断当前操作类型是否为AT_CMD_CCHP_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CCHP_SET) {
        AT_WARN_LOG("At_ProcPihCchpSetCnf : CmdCurrentOpt is not AT_CMD_CCHP_SET!");
        return AT_ERROR;
    }

    (*length) += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + (*length), "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
    /* <sessionid>, */
    (*length) += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + (*length), "%u", event->pihEvent.openChannelCnf.sessionID);

    return AT_OK;
}


VOS_UINT32 At_ProcPihPrivateCchoSetCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length)
{
    /* 判断当前操作类型是否为AT_CMD_PRIVATECCHO_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_PRIVATECCHO_SET) {
        AT_WARN_LOG("At_ProcPihPrivateCchoSetCnf : CmdCurrentOpt is not AT_CMD_PRIVATECCHO_SET!");

        return AT_ERROR;
    }

    event->pihEvent.openChannelCnf.rspDataLen = TAF_MIN(event->pihEvent.openChannelCnf.rspDataLen, SI_APDU_MAX_LEN);

    (*length) += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + (*length), "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
    /* <sessionid> */
    (*length) += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + (*length), "%u,", event->pihEvent.openChannelCnf.sessionID);
    /* <response len> */
    (*length) += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + (*length), "%d,\"",
        (event->pihEvent.openChannelCnf.rspDataLen + AT_DOUBLE_QUO_MARKS_LENGTH) * AT_DOUBLE_LENGTH);
    /* <response data> */
    (*length) += (VOS_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                     (TAF_UINT8 *)g_atSndCodeAddress + (*length),
                                                     (TAF_UINT8 *)event->pihEvent.openChannelCnf.rspDate,
                                                     event->pihEvent.openChannelCnf.rspDataLen);
    /* SW1 */
    (*length) += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                     (TAF_UINT8 *)g_atSndCodeAddress + (*length),
                                                     &event->pihEvent.openChannelCnf.sw1, sizeof(TAF_UINT8));
    /* SW2 */
    (*length) += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                     (TAF_UINT8 *)g_atSndCodeAddress + (*length),
                                                     &event->pihEvent.openChannelCnf.sw2, sizeof(TAF_UINT8));
    (*length) += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                                                   (TAF_CHAR *)g_atSndCodeAddress + (*length), "\"");

    return AT_OK;
}


VOS_UINT32 At_ProcPihPrivateCchpSetCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *len)
{
    /* 判断当前操作类型是否为AT_CMD_CCHP_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_PRIVATECCHP_SET) {
        AT_WARN_LOG("At_ProcPihPrivateCchpSetCnf : CmdCurrentOpt is not AT_CMD_PRIVATECCHP_SET!");

        return AT_ERROR;
    }

    (*len) += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + (*len), "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
    /* <sessionid> */
    (*len) += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + (*len), "%u,", event->pihEvent.openChannelCnf.sessionID);
    /* <response len> */
    (*len) += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + (*len), "%d,\"",
        (event->pihEvent.openChannelCnf.rspDataLen + AT_DOUBLE_QUO_MARKS_LENGTH) * AT_DOUBLE_LENGTH);
    /* <response data> */
    (*len) += (VOS_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                  (TAF_UINT8 *)g_atSndCodeAddress + (*len),
                                                  (TAF_UINT8 *)event->pihEvent.openChannelCnf.rspDate,
                                                  event->pihEvent.openChannelCnf.rspDataLen);
    /* SW1 */
    (*len) += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                  (TAF_UINT8 *)g_atSndCodeAddress + (*len),
                                                  &event->pihEvent.openChannelCnf.sw1, sizeof(TAF_UINT8));
    /* SW2 */
    (*len) += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                  (TAF_UINT8 *)g_atSndCodeAddress + (*len),
                                                  &event->pihEvent.openChannelCnf.sw2, sizeof(TAF_UINT8));
    (*len) += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                                                (TAF_CHAR *)g_atSndCodeAddress + (*len), "\"");

    return AT_OK;
}


VOS_UINT32 At_ProcPihCchcSetCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length)
{
    /* 判断当前操作类型是否为AT_CMD_CCHC_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CCHC_SET) {
        AT_WARN_LOG("At_ProcPihCchcSetCnf : CmdCurrentOpt is not AT_CMD_CCHC_SET!");
        return AT_ERROR;
    }

    return AT_OK;
}


VOS_UINT32 At_ProcPihSciCfgSetCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length)
{
    /* 判断当前操作类型是否为AT_CMD_SCICHG_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_SCICHG_SET) {
        AT_WARN_LOG("At_ProcPihSciCfgSetCnf : CmdCurrentOpt is not AT_CMD_SCICHG_SET!");
        return AT_ERROR;
    }

    return AT_OK;
}


VOS_UINT32 At_ProcPihBwtSetCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length)
{
    /* 判断当前操作类型是否为AT_CMD_BWT_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_BWT_SET) {
        AT_WARN_LOG("At_ProcPihBwtSetCnf : CmdCurrentOpt is not AT_CMD_BWT_SET!");
        return AT_ERROR;
    }

    return AT_OK;
}


VOS_UINT32 At_ProcPihHvsstSetCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length)
{
    /* 判断当前操作类型是否为AT_CMD_HVSST_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_HVSST_SET) {
        AT_WARN_LOG("At_ProcPihHvsstSetCnf : CmdCurrentOpt is not AT_CMD_HVSST_SET!");
        return AT_ERROR;
    }

    return AT_OK;
}


VOS_UINT32 At_ProcPihCglaSetCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length)
{
    /* 判断当前操作类型是否为AT_CMD_CGLA_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CGLA_SET) {
        AT_WARN_LOG("At_ProcPihCglaSetCnf : CmdCurrentOpt is not AT_CMD_CGLA_SET!");
        return AT_ERROR;
    }

    (*length) += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + (*length), "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

    /* <length>, */
    (*length) += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + (*length), "%d,\"",
        (event->pihEvent.cglaCmdCnf.len + AT_DOUBLE_QUO_MARKS_LENGTH) * AT_DOUBLE_LENGTH);
    if (event->pihEvent.cglaCmdCnf.len != 0) {
        /* <command>, */
        (*length) += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                         (TAF_UINT8 *)g_atSndCodeAddress + (*length),
                                                         event->pihEvent.cglaCmdCnf.command,
                                                         event->pihEvent.cglaCmdCnf.len);
    }

    /* SW1 */
    (*length) += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                     (TAF_UINT8 *)g_atSndCodeAddress + (*length),
                                                     &event->pihEvent.cglaCmdCnf.sw1, sizeof(TAF_UINT8));

    /* SW2 */
    (*length) += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                     (TAF_UINT8 *)g_atSndCodeAddress + (*length),
                                                     &event->pihEvent.cglaCmdCnf.sw2, sizeof(TAF_UINT8));
    (*length) += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                                                   (TAF_CHAR *)g_atSndCodeAddress + (*length), "\"");

    return AT_OK;
}


VOS_UINT32 At_ProcPihCardAtrQryCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length)
{
    /* 判断当前操作类型是否为AT_CMD_CARD_ATR_READ */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CARD_ATR_READ) {
        AT_WARN_LOG("At_ProcPihCardAtrQryCnf : CmdCurrentOpt is not AT_CMD_CARD_ATR_READ!");
        return AT_ERROR;
    }

    event->pihEvent.atrQryCnf.len = TAF_MIN(event->pihEvent.atrQryCnf.len, SI_ATR_MAX_LEN);

    (*length) += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + (*length), "%s:\"", g_parseContext[indexNum].cmdElement->cmdName);

    (*length) += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                     (TAF_UINT8 *)g_atSndCodeAddress + (*length),
                                                     event->pihEvent.atrQryCnf.command,
                                                     (VOS_UINT16)event->pihEvent.atrQryCnf.len);

    (*length) += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                                                   (TAF_CHAR *)g_atSndCodeAddress + (*length), "\"");

    return AT_OK;
}


VOS_UINT32 At_ProcPihCardTypeQryCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length)
{
    /* 判断当前操作类型是否为AT_CMD_CARDTYPE_QUERY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CARDTYPE_QUERY) {
        AT_WARN_LOG("At_ProcPihCardTypeQryCnf : CmdCurrentOpt is not AT_CMD_CARDTYPE_QUERY!");
        return AT_ERROR;
    }

    (*length) += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + (*length), "%s: %d, %d, %d", g_parseContext[indexNum].cmdElement->cmdName,
        event->pihEvent.cardTypeCnf.mode, event->pihEvent.cardTypeCnf.hasCModule,
        event->pihEvent.cardTypeCnf.hasGModule);

    return AT_OK;
}


VOS_UINT32 At_ProcPihCardTypeExQryCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length)
{
    /* 判断当前操作类型是否为AT_CMD_CARDTYPEEX_QUERY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CARDTYPEEX_QUERY) {
        AT_WARN_LOG("At_ProcPihCardTypeExQryCnf : CmdCurrentOpt is not AT_CMD_CARDTYPEEX_QUERY!");
        return AT_ERROR;
    }

    (*length) += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + (*length), "%s: %d, %d, %d", g_parseContext[indexNum].cmdElement->cmdName,
        event->pihEvent.cardTypeCnf.mode, event->pihEvent.cardTypeCnf.hasCModule,
        event->pihEvent.cardTypeCnf.hasGModule);

    return AT_OK;
}


VOS_UINT32 At_ProcPihCardVoltageQryCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length)
{
    /* 判断当前操作类型是否为AT_CMD_CARDVOLTAGE_QUERY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CARDVOLTAGE_QUERY) {
        AT_WARN_LOG("At_ProcPihCardVoltageQryCnf : CmdCurrentOpt is not AT_CMD_CARDVOLTAGE_QUERY!");
        return AT_ERROR;
    }

    (*length) += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s: %d, %x", g_parseContext[indexNum].cmdElement->cmdName,
        event->pihEvent.cardVoltageCnf.voltage, event->pihEvent.cardVoltageCnf.charaByte);

    return AT_OK;
}


VOS_UINT32 At_ProcPihPrivateCglaSetCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length)
{
    /* 判断当前操作类型是否为AT_CMD_PRIVATECGLA_REQ */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_PRIVATECGLA_REQ) {
        AT_WARN_LOG("At_ProcPihPrivateCglaSetCnf : CmdCurrentOpt is not AT_CMD_PRIVATECGLA_REQ!");
        return AT_ERROR;
    }

    event->pihEvent.cglaHandleCnf.len = TAF_MIN(event->pihEvent.cglaHandleCnf.len, SI_PRIVATECGLA_APDU_MAX_LEN);

    /* 非最后一条打印上报IND，需要在行首添加回车换行 */
    if (event->pihEvent.cglaHandleCnf.lastDataFlag != VOS_TRUE) {
        (*length) += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "\r\n");
    }

    /* ^CGLA: <flag>,<length>,"[<command>]<SW1><SW2>" */
    (*length) +=
        (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + (*length), "%s: %d,%d,\"", g_parseContext[indexNum].cmdElement->cmdName,
            event->pihEvent.cglaHandleCnf.lastDataFlag,
            (event->pihEvent.cglaHandleCnf.len + AT_DOUBLE_QUO_MARKS_LENGTH) * AT_DOUBLE_LENGTH);

    if (event->pihEvent.cglaHandleCnf.len != 0) {
        /* <command>, */
        (*length) += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                         (TAF_UINT8 *)g_atSndCodeAddress + (*length),
                                                         event->pihEvent.cglaHandleCnf.command,
                                                         event->pihEvent.cglaHandleCnf.len);
    }

    /* <SW1> */
    (*length) += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                     (TAF_UINT8 *)g_atSndCodeAddress + (*length),
                                                     &event->pihEvent.cglaHandleCnf.sw1, (VOS_UINT16)sizeof(TAF_UINT8));

    /* <SW2> */
    (*length) += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                     (TAF_UINT8 *)g_atSndCodeAddress + (*length),
                                                     &event->pihEvent.cglaHandleCnf.sw2, (VOS_UINT16)sizeof(TAF_UINT8));
    (*length) += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + (*length), "\"");

    return AT_OK;
}


VOS_UINT32 At_ProcPihCrsmSetCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length)
{
    /* 判断当前操作类型是否为AT_CMD_CRSM_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CRSM_SET) {
        AT_WARN_LOG("At_ProcPihCrsmSetCnf : CmdCurrentOpt is not AT_CMD_CRSM_SET!");
        return AT_ERROR;
    }

    event->pihEvent.rAccessCnf.len = TAF_MIN(event->pihEvent.rAccessCnf.len, USIMM_APDU_RSP_MAX_LEN);

    (*length) += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + (*length), "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
    /* <sw1, sw2>, */
    (*length) += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + (*length), "%d,%d", event->pihEvent.rAccessCnf.sw1,
        event->pihEvent.rAccessCnf.sw2);

    (*length) += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + (*length), ",\"");

    if (event->pihEvent.rAccessCnf.len != 0) {
        /* <response> */
        (*length) += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                         (TAF_UINT8 *)g_atSndCodeAddress + (*length),
                                                         event->pihEvent.rAccessCnf.content,
                                                         event->pihEvent.rAccessCnf.len);
    }

    (*length) += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + (*length), "\"");

    return AT_OK;
}


VOS_UINT32 At_ProcPihCrlaSetCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length)
{
    /* 判断当前操作类型是否为AT_CMD_CRLA_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CRLA_SET) {
        AT_WARN_LOG("At_ProcPihCrlaSetCnf: CmdCurrentOpt is not AT_CMD_CRLA_SET!");
        return AT_ERROR;
    }

    event->pihEvent.rAccessCnf.len = TAF_MIN(event->pihEvent.rAccessCnf.len, USIMM_APDU_RSP_MAX_LEN);

    (*length) += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + (*length), "%s: ", g_parseContext[indexNum].cmdElement->cmdName);
    /* <sw1, sw2>, */
    (*length) += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + (*length), "%d,%d", event->pihEvent.rAccessCnf.sw1,
        event->pihEvent.rAccessCnf.sw2);

    (*length) += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + (*length), ",\"");

    if (event->pihEvent.rAccessCnf.len != 0) {
        /* <response> */
        (*length) += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                         (TAF_UINT8 *)g_atSndCodeAddress + (*length),
                                                         event->pihEvent.rAccessCnf.content,
                                                         event->pihEvent.rAccessCnf.len);
    }

    (*length) += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + (*length), "\"");

    return AT_OK;
}


VOS_UINT32 At_ProcPihSessionQryCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length)
{
    /* 判断当前操作类型是否为AT_CMD_CARDSESSION_QRY */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CARDSESSION_QRY) {
        AT_WARN_LOG("At_ProcPihSessionQryCnf: CmdCurrentOpt is not AT_CMD_CARDSESSION_QRY!");
        return AT_ERROR;
    }

    (*length) += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + (*length), "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

    /* <CSIM,USIM,ISIM> */
    (*length) += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + (*length), "CSIM,%d,", event->pihEvent.sessionID[USIMM_CDMA_APP]);
    (*length) += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + (*length), "USIM,%d,", event->pihEvent.sessionID[USIMM_GUTL_APP]);
    (*length) += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + (*length), "ISIM,%d", event->pihEvent.sessionID[USIMM_IMS_APP]);

    return AT_OK;
}


VOS_UINT32 At_ProcPihCimiQryCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length)
{
    /* 判断当前操作类型是否为AT_CMD_CIMI_READ */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CIMI_READ) {
        AT_WARN_LOG("At_ProcPihCimiQryCnf: CmdCurrentOpt is not AT_CMD_CIMI_READ!");
        return AT_ERROR;
    }

    /* buf从外部接口返回的，增加结束符，防止缓冲区溢出 */
    event->pihEvent.imsi.imsi[SI_IMSI_MAX_LEN - 1] = '\0';

    g_logPrivacyAtCmd = TAF_LOG_PRIVACY_AT_CMD_CIMI;
    (*length) += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + (*length), "%s", event->pihEvent.imsi.imsi);

    return AT_OK;
}


VOS_UINT32 At_ProcPihCcimiQryCnf(VOS_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length)
{
    /* 判断当前操作类型是否为AT_CMD_CCIMI_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CCIMI_SET) {
        AT_WARN_LOG("At_ProcPihCcimiQryCnf: CmdCurrentOpt is not AT_CMD_CCIMI_SET!");
        return AT_ERROR;
    }

    /* buf从外部接口返回的，增加结束符，防止缓冲区溢出 */
    event->pihEvent.imsi.imsi[SI_IMSI_MAX_LEN - 1] = '\0';

    g_logPrivacyAtCmd = TAF_LOG_PRIVACY_AT_CMD_CIMI;
    (*length) += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + (*length), "%s", event->pihEvent.imsi.imsi);

    return AT_OK;
}


#if (FEATURE_VSIM == FEATURE_ON)
#if (FEATURE_VSIM_ICC_SEC_CHANNEL == FEATURE_ON)

VOS_UINT32 At_QryIccVsimVer(VOS_UINT8 indexNum)
{
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "^ICCVSIMVER: %u", SI_PIH_GetSecIccVsimVer());
    return AT_OK;
}


VOS_UINT32 At_QryHvCheckCardPara(VOS_UINT8 indexNum)
{
    if (SI_PIH_HvCheckCardQuery(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId) == AT_SUCCESS) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_HVSCONT_READ;

        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    }

    return AT_ERROR;
}

#endif
#endif /* end of (FEATURE_VSIM == FEATURE_ON) */

#if (FEATURE_IMS == FEATURE_ON)

VOS_UINT32 AT_UiccAuthCnf(TAF_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length)
{
    /* 判断当前操作类型是否为AT_CMD_UICCAUTH_SET/AT_CMD_KSNAFAUTH_SET */
    if ((g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_UICCAUTH_SET) &&
        (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_KSNAFAUTH_SET)) {
        AT_WARN_LOG("AT_UiccAuthCnf : CmdCurrentOpt is not AT_CMD_UICCAUTH_SET/AT_CMD_KSNAFAUTH_SET!");
        return AT_ERROR;
    }

    if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_UICCAUTH_SET) {
        (*length) += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + (*length), "^UICCAUTH:");

        /* <result> */
        (*length) += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + (*length), "%d", event->pihEvent.uiccAuthCnf.status);

        if (event->pihEvent.uiccAuthCnf.status == SI_PIH_AUTH_SUCCESS) {
            /* ,<Res> */
            (*length) += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + (*length), ",\"");
            (*length) += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                             (TAF_UINT8 *)g_atSndCodeAddress + (*length),
                                                             &event->pihEvent.uiccAuthCnf.akaData.authRes[1],
                                                             event->pihEvent.uiccAuthCnf.akaData.authRes[0]);
            (*length) += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + (*length), "\"");

            if (event->pihEvent.uiccAuthCnf.authType == SI_PIH_UICCAUTH_AKA) {
                /* ,<ck> */
                (*length) += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + (*length), ",\"");
                (*length) += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                                 (TAF_UINT8 *)g_atSndCodeAddress + (*length),
                                                                 &event->pihEvent.uiccAuthCnf.akaData.ck[1],
                                                                 event->pihEvent.uiccAuthCnf.akaData.ck[0]);
                (*length) += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + (*length), "\"");

                /* ,<ik> */
                (*length) += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + (*length), ",\"");
                (*length) += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                                 (TAF_UINT8 *)g_atSndCodeAddress + (*length),
                                                                 &event->pihEvent.uiccAuthCnf.akaData.ik[1],
                                                                 event->pihEvent.uiccAuthCnf.akaData.ik[0]);
                (*length) += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                    (VOS_CHAR *)g_atSndCodeAddress + (*length), "\"");
            }
        }

        if (event->pihEvent.uiccAuthCnf.status == SI_PIH_AUTH_SYNC) {
            /* ,"","","",<autn> */
            (*length) += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + (*length), ",\"\",\"\",\"\",\"");
            (*length) += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                             (TAF_UINT8 *)g_atSndCodeAddress + (*length),
                                                             &event->pihEvent.uiccAuthCnf.akaData.auts[1],
                                                             event->pihEvent.uiccAuthCnf.akaData.auts[0]);
            (*length) += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + (*length), "\"");
        }
    }

    if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_KSNAFAUTH_SET) {
        (*length) += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + (*length), "^KSNAFAUTH:");

        /* <status> */
        (*length) += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + (*length), "%d", event->pihEvent.uiccAuthCnf.status);

        if (event->pihEvent.uiccAuthCnf.nAFData.ksExtNaf[0] != VOS_NULL) {
            /* ,<ks_Naf> */
            (*length) += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + (*length), ",\"");
            (*length) += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                             (TAF_UINT8 *)g_atSndCodeAddress + (*length),
                                                             &event->pihEvent.uiccAuthCnf.nAFData.ksExtNaf[1],
                                                             event->pihEvent.uiccAuthCnf.nAFData.ksExtNaf[0]);
            (*length) += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + (*length), "\"");
        }
    }

    return AT_OK;
}


VOS_UINT32 AT_UiccAccessFileCnf(TAF_UINT8 indexNum, SI_PIH_EventInfo *event, VOS_UINT16 *length)
{
    /* 判断当前操作类型是否为AT_CMD_CURSM_SET */
    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CURSM_SET) {
        AT_WARN_LOG("AT_UiccAccessFileCnf : CmdCurrentOpt is not AT_CMD_CURSM_SET!");
        return AT_ERROR;
    }

    if ((event->pihEvent.uiccAcsFileCnf.dataLen != 0) &&
        (event->pihEvent.uiccAcsFileCnf.cmdType == SI_PIH_ACCESS_READ)) {
        (*length) += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + (*length), "^CURSM:");

        (*length) += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + (*length), "\"");
        (*length) += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                         (TAF_UINT8 *)g_atSndCodeAddress + (*length),
                                                         event->pihEvent.uiccAcsFileCnf.command,
                                                         (VOS_UINT16)event->pihEvent.uiccAcsFileCnf.dataLen);
        (*length) += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + (*length), "\"");
    }

    return AT_OK;
}
#endif

/*
 * Function:       At_CrlaFilePathCheck
 * Description:    执行CRLA命令输入的<path>参数(文件路径进行检查)
 * Return:         AT_SUCCESS：成功，其他为失败
 * History:
 * 1.Date: 2012-05-04
 *   Modification: Created function
 */
TAF_UINT32 At_CrlaFilePathCheck(TAF_UINT32 efId, TAF_UINT8 *filePath, TAF_UINT16 *pathLen)
{
    errno_t    memResult;
    TAF_UINT32 pathInvalidFlg;
    TAF_UINT16 len;
    TAF_UINT16 path[USIMM_MAX_PATH_LEN]    = {0};
    TAF_UINT16 tmpPath[USIMM_MAX_PATH_LEN] = {0};
    TAF_UINT16 pathLenTemp;
    TAF_UINT32 i;

    pathLenTemp = *pathLen;
    len         = 0;

    if (pathLenTemp > USIMM_MAX_PATH_LEN) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    for (i = 0; i < (pathLenTemp / sizeof(TAF_UINT16)); i++) {
        tmpPath[i] = (TAF_UINT16)((filePath[i * sizeof(TAF_UINT16)] << 0x08) & AT_SIM_FILE_PATH_MASK) +
                     filePath[(i * sizeof(TAF_UINT16)) + 1];
    }

    /* 如果路径不是以3F00开始，需要添加3F00作开头 */
    if (tmpPath[0] != MFID) {
        if (pathLenTemp == USIMM_MAX_PATH_LEN) {
            return AT_CME_INCORRECT_PARAMETERS;
        }

        path[0] = MFID;

        len++;
    }

    if (pathLenTemp > 0) {
        memResult = memcpy_s(&path[len], sizeof(TAF_UINT16) * (USIMM_MAX_PATH_LEN - len), tmpPath, pathLenTemp);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(TAF_UINT16) * (USIMM_MAX_PATH_LEN - len), pathLenTemp);
    }

    len += (pathLenTemp / sizeof(TAF_UINT16));

    if ((efId & AT_SIM_FILE_PATH_MASK) == EFIDUNDERMF) {
        len = 1;
    }
    /* 4F文件要在5F下，路径长度为3 */
    else if ((efId & AT_SIM_FILE_PATH_MASK) == EFIDUNDERMFDFDF) {
        pathInvalidFlg = (len != AT_SIM_FILE_PATH_LEN_3) ||
                         ((path[AT_SIM_FILE_PATH_INDEX_1] & AT_SIM_FILE_PATH_MASK) != DFIDUNDERMF) ||
                         ((path[AT_SIM_FILE_PATH_INDEX_2] & AT_SIM_FILE_PATH_MASK) != DFIDUNDERMFDF);

        if (pathInvalidFlg == VOS_TRUE) {
            return AT_CME_INCORRECT_PARAMETERS;
        }
    }
    /* 6F文件要在7F下，路径长度为2 */
    else if ((efId & AT_SIM_FILE_PATH_MASK) == EFIDUNDERMFDF) {
        if ((len != AT_SIM_FILE_PATH_LEN_2) ||
            ((path[AT_SIM_FILE_PATH_INDEX_1] & AT_SIM_FILE_PATH_MASK) != DFIDUNDERMF)) {
            return AT_CME_INCORRECT_PARAMETERS;
        }
    } else {
    }

    *pathLen = len;

    if (len > 0) {
        memResult = memcpy_s(filePath, AT_PARA_MAX_LEN + 1, path, (VOS_SIZE_T)(len * AT_DOUBLE_LENGTH));
        TAF_MEM_CHK_RTN_VAL(memResult, AT_PARA_MAX_LEN + 1, (VOS_SIZE_T)(len * AT_DOUBLE_LENGTH));
    }

    return AT_SUCCESS;
}

/*
 * Function:       At_CrlaApduParaCheck
 * Description:    执行CRLA命令输入的参数的匹配检查
 * Return:         AT_SUCCESS：成功，其他为失败
 * History:
 * 1.Date: 2012-05-04
 *   Modification: Created function
 */
TAF_UINT32 At_CrlaApduParaCheck(VOS_VOID)
{
    TAF_UINT16 fileTag;

    /* 命令类型参数检查，第二个参数不能为空 */
    if (g_atParaList[AT_CRLA_COMMAND].paraLen == 0) {
        AT_ERR_LOG("At_CrlaApduParaCheck: command type null");

        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 除STATUS命令外，文件ID输入不能为空 */
    if ((g_atParaList[AT_CRLA_FILEID].paraValue == 0) && (g_atParaList[AT_CRLA_COMMAND].paraValue != USIMM_STATUS)) {
        AT_ERR_LOG("At_CrlaApduParaCheck: File Id null.");

        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 获取文件ID前两位 */
    fileTag = (g_atParaList[AT_CRLA_FILEID].paraValue >> 8) & (0x00FF);

    /* 输入的文件ID必须是EF文件，前两位不可以是3F/5F/7F */
    if ((fileTag == MFLAB) || (fileTag == DFUNDERMFLAB) || (fileTag == DFUNDERDFLAB)) {
        AT_ERR_LOG("At_CrlaApduParaCheck: File Id error.");

        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* <P1><P2><P3>这三个参数全部为空 */
    if ((g_atParaList[AT_CRLA_P1].paraLen == 0) && (g_atParaList[AT_CRLA_P2].paraLen == 0) &&
        (g_atParaList[AT_CRLA_P3].paraLen == 0)) {
        return AT_SUCCESS;
    }

    /* <P1><P2><P3>这三个参数全部不为空 */
    if ((g_atParaList[AT_CRLA_P1].paraLen != 0) && (g_atParaList[AT_CRLA_P2].paraLen != 0) &&
        (g_atParaList[AT_CRLA_P3].paraLen != 0)) {
        return AT_SUCCESS;
    }

    /* 其它情况下属于输入AT命令参数不完整 */
    return AT_CME_INCORRECT_PARAMETERS;
}

/*
 * Function:       At_CrlaFilePathParse
 * Description:    执行CRSM命令输入的路径进行解析
 * Return:          AT_SUCCESS：成功，其他为失败
 * History:
 * 1.Date: 2012-05-04
 *   Modification: Created function
 */
TAF_UINT32 At_CrlaFilePathParse(SI_PIH_Crla *command)
{
    errno_t    memResult;
    TAF_UINT32 result;

    /* 如果词法解析器解析第八个参数为空，说明没有文件路径输入，直接返回成功 */
    if ((g_atParaList[AT_CRLA_PATHID].paraLen == 0) && (command->efId != VOS_NULL_WORD)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 在转换前输入的文件路径长度必须是4的整数倍 */
    if ((g_atParaList[AT_CRLA_PATHID].paraLen % 4) != 0) {
        AT_ERR_LOG("At_CrlaFilePathParse: Path error");

        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 将输入的字符串转换成十六进制数组 */
    if (At_AsciiNum2HexString(g_atParaList[AT_CRLA_PATHID].para, &g_atParaList[AT_CRLA_PATHID].paraLen) == AT_FAILURE) {
        AT_ERR_LOG("At_CrlaFilePathParse: At_AsciiNum2HexString error.");

        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 如果有填写文件ID和路径，要做文件路径检查，输入的路径长度以U16为单位 */
    result = At_CrlaFilePathCheck((TAF_UINT16)g_atParaList[AT_CRLA_FILEID].paraValue, g_atParaList[AT_CRLA_PATHID].para,
                                  &g_atParaList[AT_CRLA_PATHID].paraLen);

    if (result != AT_SUCCESS) {
        AT_ERR_LOG("At_CrlaFilePathParse: At_CrsmFilePathCheck error.");

        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 填充文件路径和长度 */
    command->pathLen = g_atParaList[AT_CRLA_PATHID].paraLen;

    /* 文件路径长度是U16为单位的，路径拷贝的长度要乘2 */
    if (((g_atParaList[AT_CRLA_PATHID].paraLen) > 0) &&
        ((VOS_SIZE_T)(g_atParaList[AT_CRLA_PATHID].paraLen * sizeof(VOS_UINT16)) <= sizeof(command->path))) {
        memResult = memcpy_s(command->path, sizeof(command->path), g_atParaList[AT_CRLA_PATHID].para,
                             (VOS_SIZE_T)(g_atParaList[AT_CRLA_PATHID].paraLen * sizeof(VOS_UINT16)));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(command->path),
                            (VOS_SIZE_T)(g_atParaList[AT_CRLA_PATHID].paraLen * sizeof(VOS_UINT16)));
    }

    return AT_SUCCESS;
}

/*
 * Function:       At_CrlaParaStatusCheck
 * Description:    执行CRLA命令对STATUS命令的参数检查
 * Return:          AT_SUCCESS：成功，其他为失败
 * History:
 * 1.Date: 2012-05-04
 *   Modification: Created function
 */
TAF_UINT32 At_CrlaParaStatusCheck(SI_PIH_Crla *command)
{
    /* STATUS命令如果没有输入文件ID，就不需要做选文件操作，直接发STATUS命令 */
    if (g_atParaList[AT_CRLA_FILEID].paraValue == 0) {
        command->efId = VOS_NULL_WORD;
    } else {
        command->efId = (TAF_UINT16)g_atParaList[AT_CRLA_FILEID].paraValue;
    }

    /* 填写数据结构中的<P1><P2><P3>对应的IE项 */
    command->p1      = (TAF_UINT8)g_atParaList[AT_CRLA_P1].paraValue;
    command->p2      = (TAF_UINT8)g_atParaList[AT_CRLA_P2].paraValue;
    command->p3      = (TAF_UINT8)g_atParaList[AT_CRLA_P3].paraValue;
    command->cmdType = USIMM_STATUS;

    return At_CrlaFilePathParse(command);
}

/*
 * Function:       At_CrlaParaReadBinaryCheck
 * Description:    执行CRLA命令对Read Binary命令的参数检查
 * Return:          AT_SUCCESS：成功，其他为失败
 * History:
 * 1.Date: 2012-05-04
 *   Modification: Created function
 */
TAF_UINT32 At_CrlaParaReadBinaryCheck(SI_PIH_Crla *command)
{
    /* 填写数据结构中的<P1><P2><P3>对应的IE项 */
    command->p1      = (TAF_UINT8)g_atParaList[AT_CRLA_P1].paraValue;
    command->p2      = (TAF_UINT8)g_atParaList[AT_CRLA_P2].paraValue;
    command->p3      = (TAF_UINT8)g_atParaList[AT_CRLA_P3].paraValue;
    command->efId    = (TAF_UINT16)g_atParaList[AT_CRLA_FILEID].paraValue;
    command->cmdType = USIMM_READ_BINARY;

    /* 如果有输入文件路径需要检查输入参数 */
    return At_CrlaFilePathParse(command);
}

/*
 * Function:       At_CrlaParaReadRecordCheck
 * Description:    执行CRLA命令对Read Record的参数检查
 * Return:         AT_SUCCESS：成功，其他为失败
 * History:
 * 1.Date: 2012-05-04
 *   Modification: Created function
 */
TAF_UINT32 At_CrlaParaReadRecordCheck(SI_PIH_Crla *command)
{
    /* 填写数据结构中的<P1><P2><P3>对应的IE项 */
    command->p1      = (TAF_UINT8)g_atParaList[AT_CRLA_P1].paraValue;
    command->p2      = (TAF_UINT8)g_atParaList[AT_CRLA_P2].paraValue;
    command->p3      = (TAF_UINT8)g_atParaList[AT_CRLA_P3].paraValue;
    command->efId    = (TAF_UINT16)g_atParaList[AT_CRLA_FILEID].paraValue;
    command->cmdType = USIMM_READ_RECORD;

    /* 如果有输入文件路径需要检查输入参数 */
    return At_CrlaFilePathParse(command);
}


VOS_UINT32 At_CrlaParaGetRspCheck(SI_PIH_Crla *command)
{
    /* 参数个数不能少于2个，至少要有命令类型和文件ID */
    if (g_atParaIndex < AT_CRLA_PARA_GETRSP_MIN_NUM) {
        AT_ERR_LOG("At_CrlaParaGetRspCheck: Para less than 2.");

        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 填写数据结构中的<P1><P2><P3>对应的IE项 */
    command->p1      = (TAF_UINT8)g_atParaList[AT_CRLA_P1].paraValue;
    command->p2      = (TAF_UINT8)g_atParaList[AT_CRLA_P2].paraValue;
    command->p3      = (TAF_UINT8)g_atParaList[AT_CRLA_P3].paraValue;
    command->efId    = (TAF_UINT16)g_atParaList[AT_CRLA_FILEID].paraValue;
    command->cmdType = USIMM_GET_RESPONSE;

    /* 如果有输入文件路径需要检查输入参数 */
    return At_CrlaFilePathParse(command);
}


VOS_UINT32 At_CrlaParaUpdateBinaryCheck(SI_PIH_Crla *command)
{
    errno_t memResult;
    /* Update Binary命令至少要有6个参数，可以没有文件路径 */
    if (g_atParaIndex < AT_CRLA_PARA_UPDATE_BINARY_MIN_NUM) {
        AT_ERR_LOG("At_CrlaParaUpdateBinaryCheck: Para less than 6.");

        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 填写数据结构中的<P1><P2><P3>对应的IE项 */
    command->p1      = (TAF_UINT8)g_atParaList[AT_CRLA_P1].paraValue;
    command->p2      = (TAF_UINT8)g_atParaList[AT_CRLA_P2].paraValue;
    command->p3      = (TAF_UINT8)g_atParaList[AT_CRLA_P3].paraValue;
    command->efId    = (TAF_UINT16)g_atParaList[AT_CRLA_FILEID].paraValue;
    command->cmdType = USIMM_UPDATE_BINARY;

    /* 第六个参数输入的<data>字符串在转换前数据长度必须是2的倍数且不能为0 */
    if (((g_atParaList[AT_CRLA_DATA].paraLen % 2) != 0) || (g_atParaList[AT_CRLA_DATA].paraLen == 0)) {
        AT_ERR_LOG("At_CrlaParaUpdateBinaryCheck: <data> error.");

        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (At_AsciiNum2HexString(g_atParaList[AT_CRLA_DATA].para, &g_atParaList[AT_CRLA_DATA].paraLen) == AT_FAILURE) {
        AT_ERR_LOG("At_CrlaParaUpdateBinaryCheck: At_AsciiNum2HexString fail.");

        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaList[AT_CRLA_DATA].paraLen > sizeof(command->content)) {
        AT_ERR_LOG("At_CrlaParaUpdateBinaryCheck: g_atParaList[6] too long");

        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 设置<data>，其长度由<data>参数输入确定，P3参数照常下发，不关心<data>的长度是否和P3的值匹配 */
    memResult = memcpy_s((TAF_VOID *)command->content, sizeof(command->content),
                         (TAF_VOID *)g_atParaList[AT_CRLA_DATA].para, g_atParaList[AT_CRLA_DATA].paraLen);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(command->content), g_atParaList[AT_CRLA_DATA].paraLen);

    return At_CrlaFilePathParse(command);
}


VOS_UINT32 At_CrlaParaUpdateRecordCheck(SI_PIH_Crla *command)
{
    errno_t memResult;

    /* Update Binary命令至少要有6个参数，可以没有文件路径 */
    if (g_atParaIndex < AT_CRLA_PARA_UPDATE_BINARY_MIN_NUM) {
        AT_ERR_LOG("At_CrlaParaUpdateRecordCheck: Para less than 6.");

        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 填写数据结构中的<P1><P2><P3>对应的IE项 */
    command->p1      = (TAF_UINT8)g_atParaList[AT_CRLA_P1].paraValue;
    command->p2      = (TAF_UINT8)g_atParaList[AT_CRLA_P2].paraValue;
    command->p3      = (TAF_UINT8)g_atParaList[AT_CRLA_P3].paraValue;
    command->efId    = (TAF_UINT16)g_atParaList[AT_CRLA_FILEID].paraValue;
    command->cmdType = USIMM_UPDATE_RECORD;

    /* 第六个参数输入的<data>字符串数据长度必须是2的倍数且不能为0 */
    if (((g_atParaList[AT_CRLA_DATA].paraLen % 2) != 0) || (g_atParaList[AT_CRLA_DATA].paraLen == 0)) {
        AT_ERR_LOG("At_CrlaParaUpdateRecordCheck: <data> error.");

        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (At_AsciiNum2HexString(g_atParaList[AT_CRLA_DATA].para, &g_atParaList[AT_CRLA_DATA].paraLen) == AT_FAILURE) {
        AT_ERR_LOG("At_CrlaParaUpdateRecordCheck: At_AsciiNum2HexString fail.");

        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 防止因为数据长度过长导致单板复位 */
    if (g_atParaList[AT_CRLA_DATA].paraLen > sizeof(command->content)) {
        AT_ERR_LOG("At_CrlaParaUpdateRecordCheck: g_atParaList[6] too long");

        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 设置<data>，其长度由<data>参数输入确定，P3参数照常下发，不关心<data>的长度是否和P3的值匹配 */
    memResult = memcpy_s((TAF_VOID *)command->content, sizeof(command->content),
                         (TAF_VOID *)g_atParaList[AT_CRLA_DATA].para,
                         g_atParaList[AT_CRLA_DATA].paraLen);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(command->content), g_atParaList[AT_CRLA_DATA].paraLen);

    return At_CrlaFilePathParse(command);
}


VOS_UINT32 At_CrlaParaSearchRecordCheck(SI_PIH_Crla *command)
{
    /* Search Record命令至少要有8个参数 */
    if (g_atParaIndex < AT_CRLA_PARA_SEARCH_RECORD_MIN_NUM) {
        AT_ERR_LOG("At_CrlaParaSearchRecordCheck: Para less than 8.");

        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 填写数据结构中的<P1><P2><P3> */
    command->p1 = (TAF_UINT8)g_atParaList[AT_CRLA_P1].paraValue;
    command->p2 = (TAF_UINT8)g_atParaList[AT_CRLA_P2].paraValue;
    command->p3 = (TAF_UINT8)g_atParaList[AT_CRLA_P3].paraValue;

    /* 填写数据结构中的<fileid> */
    command->efId = (TAF_UINT16)g_atParaList[AT_CRLA_FILEID].paraValue;

    /* 填写数据结构中的<command> */
    command->cmdType = USIMM_SEARCH_RECORD;

    /* 第七个参数输入的<data>字符串数据长度必须是2的倍数且不能为0 */
    if (((g_atParaList[AT_CRLA_DATA].paraLen % 2) != 0) || (g_atParaList[AT_CRLA_DATA].paraLen == 0)) {
        AT_ERR_LOG("At_CrlaParaSearchRecordCheck: <data> error.");

        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (At_AsciiNum2HexString(g_atParaList[AT_CRLA_DATA].para, &g_atParaList[AT_CRLA_DATA].paraLen) == AT_FAILURE) {
        AT_ERR_LOG("At_CrlaParaSearchRecordCheck: At_AsciiNum2HexString fail.");

        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 防止因为数据长度过长导致单板复位 */
    if (g_atParaList[AT_CRLA_DATA].paraLen > sizeof(command->content)) {
        AT_ERR_LOG("At_CrlaParaSearchRecordCheck: g_atParaList[6] too long");

        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 设置<data>，其长度由<data>参数输入确定，P3参数照常下发，不关心<data>的长度是否和P3的值匹配 */
    if (memcpy_s((TAF_VOID *)command->content, sizeof(command->content), (TAF_VOID *)g_atParaList[AT_CRLA_DATA].para,
                 g_atParaList[AT_CRLA_DATA].paraLen) != EOK) {
        AT_ERR_LOG("At_CrlaParaSearchRecordCheck: memcpy_s fail");

        return AT_CME_INCORRECT_PARAMETERS;
    }

    return At_CrlaFilePathParse(command);
}


TAF_UINT32 At_SetCrlaPara(TAF_UINT8 indexNum)
{
    SI_PIH_Crla command;
    TAF_UINT32  result;

    /* 参数过多 */
    if (g_atParaIndex > AT_CRLA_MAX_PARA_NUM) {
        AT_ERR_LOG("At_SetCrlaPara: too many para");

        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 如果有输入<P1><P2><P3>，这三个参数不能只填写部分参数 */
    if (At_CrlaApduParaCheck() != AT_SUCCESS) {
        AT_ERR_LOG("At_SetCrlaPara: At_CrlaApduParaCheck fail.");

        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 初始化 */
    (VOS_VOID)memset_s(&command, sizeof(command), 0x00, sizeof(command));

    command.sessionID = g_atParaList[0].paraValue;

    switch (g_atParaList[1].paraValue) {
        case USIMM_STATUS:
            result = At_CrlaParaStatusCheck(&command);
            break;
        case USIMM_READ_BINARY:
            result = At_CrlaParaReadBinaryCheck(&command);
            break;
        case USIMM_READ_RECORD:
            result = At_CrlaParaReadRecordCheck(&command);
            break;
        case USIMM_GET_RESPONSE:
            result = At_CrlaParaGetRspCheck(&command);
            break;
        case USIMM_UPDATE_BINARY:
            result = At_CrlaParaUpdateBinaryCheck(&command);
            break;
        case USIMM_UPDATE_RECORD:
            result = At_CrlaParaUpdateRecordCheck(&command);
            break;
        case USIMM_SEARCH_RECORD:
            result = At_CrlaParaSearchRecordCheck(&command);
            break;
        default:
            return AT_CME_INCORRECT_PARAMETERS;
    }

    if (result != AT_SUCCESS) {
        AT_ERR_LOG("At_SetCrlaPara: para parse fail");

        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 执行命令操作 */
    if (SI_PIH_CrlaSetReq(g_atClientTab[indexNum].clientId, 0, &command) == TAF_SUCCESS) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CRLA_SET;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 At_QryCardSession(VOS_UINT8 indexNum)
{
    if (SI_PIH_CardSessionQuery(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId) == TAF_SUCCESS) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CARDSESSION_QRY;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    }

    AT_WARN_LOG("At_QryCardSession: SI_PIH_CardSessionQuery fail.");

    /* 返回命令处理挂起状态 */
    return AT_ERROR;
}


TAF_UINT32 At_QryCardVoltagePara(TAF_UINT8 indexNum)
{
    VOS_UINT32 result;

    result = SI_PIH_CardVoltageQuery(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId);

    if (result != TAF_SUCCESS) {
        AT_WARN_LOG("At_QryCardVoltagePara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CARDVOLTAGE_QUERY;

    return AT_WAIT_ASYNC_RETURN;
}


TAF_UINT32 At_SetPrivateCglaPara(TAF_UINT8 indexNum)
{
    SI_PIH_CglaCommand cglaCmd;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数过多 */
    if (g_atParaIndex != AT_CGLA_PARA_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* <length>需要为2的整数倍 */
    if ((g_atParaList[AT_CGLA_LENGTH].paraValue % 2) != 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 字符串长度不为2的整数倍 */
    if ((g_atParaList[AT_CGLA_COMMAND].paraLen % 2) != 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 将字符串转换为16进制数组 */
    if (At_AsciiNum2HexString(g_atParaList[AT_CGLA_COMMAND].para,
                              &g_atParaList[AT_CGLA_COMMAND].paraLen) == AT_FAILURE) {
        AT_ERR_LOG("At_SetCglaCmdPara: At_AsciiNum2HexString fail.");

        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* length字段是实际命令长度的2倍 */
    if (g_atParaList[AT_CGLA_LENGTH].paraValue !=(TAF_UINT32)(g_atParaList[AT_CGLA_COMMAND].paraLen * AT_CONST_NUM_2)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    cglaCmd.sessionID = g_atParaList[AT_CGLA_SESSIONID].paraValue;
    cglaCmd.len       = g_atParaList[AT_CGLA_COMMAND].paraLen;
    cglaCmd.command   = g_atParaList[AT_CGLA_COMMAND].para;

    /* 执行命令操作 */
    if (SI_PIH_PrivateCglaSetReq(g_atClientTab[indexNum].clientId, 0, &cglaCmd) == AT_SUCCESS) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_PRIVATECGLA_REQ;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    }

    return AT_ERROR;
}

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)

TAF_UINT32 AT_SetPrfApp(AT_CARDAPP_Uint32 cardApp, USIMM_NvCardappUint32 cardAPP, ModemIdUint16 modemId)
{
    USIMM_AppPriorityConfig appInfo;
    TAF_UINT32              appHit      = 0;
    TAF_UINT32              appOrderPos = 0;
    TAF_UINT32              rslt;
    TAF_UINT32              i;
    errno_t                 returnValue;

    (VOS_VOID)memset_s(&appInfo, (VOS_SIZE_T)sizeof(appInfo), 0x00, (VOS_SIZE_T)sizeof(appInfo));

    rslt = TAF_ACORE_NV_READ(modemId, NV_ITEM_USIM_APP_PRIORITY_CFG, &appInfo, sizeof(USIMM_AppPriorityConfig));

    if (rslt != NV_OK) {
        AT_ERR_LOG("AT_SetPrfApp: Get NV_ITEM_USIM_APP_PRIORITY_CFG fail.");

        return AT_ERROR;
    }

    /* 防止NV数据异常导致越界访问 */
    if (appInfo.appNum > AT_ARRAY_SIZE(appInfo.appList)) {
        AT_WARN_LOG("AT_SetPrfApp: Get NV_ITEM_USIM_APP_PRIORITY_CFG success, but ucAppNum invalid.");

        return AT_ERROR;
    }

    /* 设置CDMA应用优先 */
    for (i = 0; i < appInfo.appNum; i++) {
        if (cardAPP == appInfo.appList[i]) {
            appHit      = VOS_TRUE;
            appOrderPos = i;
            break;
        }
    }

    if (appHit == VOS_FALSE) {
        /* 没有找到，插入到最前边 */
        if (appInfo.appNum >= USIMM_NV_CARDAPP_BUTT) {
            rslt = AT_ERROR;
        } else {
            if (appInfo.appNum > 0) {
                returnValue = memmove_s(&appInfo.appList[1], sizeof(appInfo.appList) - 1 * sizeof(appInfo.appList[1]),
                                        &appInfo.appList[0], sizeof(VOS_UINT32) * appInfo.appNum);
                TAF_MEM_CHK_RTN_VAL(returnValue, sizeof(appInfo.appList) - 1 * sizeof(appInfo.appList[1]),
                                    sizeof(VOS_UINT32) * appInfo.appNum);
            }

            appInfo.appList[0] = cardAPP;
            appInfo.appNum++;

            rslt = TAF_ACORE_NV_WRITE(modemId, NV_ITEM_USIM_APP_PRIORITY_CFG, (VOS_UINT8 *)&appInfo,
                                      sizeof(USIMM_AppPriorityConfig));
        }
    } else {
        if (appOrderPos != 0) {
            /* 从第一个起，往后移动i个 */
            returnValue = memmove_s(&appInfo.appList[1], sizeof(appInfo.appList) - 1 * sizeof(appInfo.appList[1]),
                                    &appInfo.appList[0], (sizeof(VOS_UINT32) * appOrderPos));
            TAF_MEM_CHK_RTN_VAL(returnValue, sizeof(appInfo.appList) - 1 * sizeof(appInfo.appList[1]),
                                sizeof(VOS_UINT32) * appOrderPos);

            appInfo.appList[0] = cardAPP;

            rslt = TAF_ACORE_NV_WRITE(modemId, NV_ITEM_USIM_APP_PRIORITY_CFG, (VOS_UINT8 *)&appInfo,
                                      sizeof(USIMM_AppPriorityConfig));
        }
    }

    if (rslt != NV_OK) {
        AT_ERR_LOG("TAF_ACORE_NV_WRITE: Write NV Fail.");
        return AT_ERROR;
    }

    return AT_OK;
}


TAF_UINT32 At_SetPrfAppPara(TAF_UINT8 indexNum)
{
    ModemIdUint16 modemId;
    TAF_UINT32    rslt = AT_OK;

    /* 参数过多 */
    if (g_atParaIndex > 1) {
        AT_ERR_LOG("At_SetPrfAppPara: too many para");

        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 根据AT命令通道号得到MODEM ID */
    if (AT_GetModemIdFromClient(indexNum, &modemId) != VOS_OK) {
        AT_ERR_LOG("AT_SetRATCombinePara: Get modem id fail.");

        return AT_ERROR;
    }

    if (g_atParaList[0].paraValue == AT_PREFER_APP_CDMA) {
        rslt = AT_SetPrfApp(AT_PREFER_APP_CDMA, USIMM_NV_CDMA_APP, modemId);
    } else {
        rslt = AT_SetPrfApp(AT_PREFER_APP_GUTL, USIMM_NV_GUTL_APP, modemId);
    }

    return rslt;
}


TAF_UINT32 At_QryPrfAppPara(TAF_UINT8 indexNum)
{
    TAF_UINT32              i;
    TAF_UINT32              cdmaHit;
    TAF_UINT32              gutlHit;
    USIMM_AppPriorityConfig appInfo;
    ModemIdUint16           modemId;
    VOS_UINT16              length;

    /* 根据AT命令通道号得到MODEM ID */
    if (AT_GetModemIdFromClient(indexNum, &modemId) != VOS_OK) {
        AT_ERR_LOG("At_QryPrfAppPara: Get modem id fail.");

        return AT_ERROR;
    }

    (VOS_VOID)memset_s(&appInfo, (VOS_SIZE_T)sizeof(appInfo), 0x00, (VOS_SIZE_T)sizeof(appInfo));

    if (TAF_ACORE_NV_READ(modemId, NV_ITEM_USIM_APP_PRIORITY_CFG, &appInfo, sizeof(USIMM_AppPriorityConfig)) != NV_OK) {
        AT_ERR_LOG("At_QryPrfAppPara: Get NV_ITEM_USIM_APP_PRIORITY_CFG fail.");

        return AT_ERROR;
    }

    /* 防止NV数据异常导致越界访问 */
    if (appInfo.appNum > AT_ARRAY_SIZE(appInfo.appList)) {
        AT_WARN_LOG("At_QryPrfAppPara: Get NV_ITEM_USIM_APP_PRIORITY_CFG success, but ucAppNum invalid.");

        appInfo.appNum = AT_ARRAY_SIZE(appInfo.appList);
    }

    cdmaHit = VOS_FALSE;
    gutlHit = VOS_FALSE;

    /* 查找CDMA和GUTL应用在NV项中的位置 */
    for (i = 0; i < appInfo.appNum; i++) {
        if (appInfo.appList[i] == USIMM_NV_GUTL_APP) {
            gutlHit = VOS_TRUE;

            break;
        }

        if (appInfo.appList[i] == USIMM_NV_CDMA_APP) {
            cdmaHit = VOS_TRUE;

            break;
        }
    }

    if (gutlHit == VOS_TRUE) {
        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: 1", g_parseContext[indexNum].cmdElement->cmdName);

        g_atSendDataBuff.bufLen = length;

        return AT_OK;
    } else if (cdmaHit == VOS_TRUE) {
        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: 0", g_parseContext[indexNum].cmdElement->cmdName);

        g_atSendDataBuff.bufLen = length;

        return AT_OK;
    } else {
        return AT_ERROR;
    }
}


TAF_UINT32 At_TestPrfAppPara(TAF_UINT8 indexNum)
{
    VOS_UINT16 length;

    length = 0;

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: (0,1)", g_parseContext[indexNum].cmdElement->cmdName);

    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}


TAF_UINT32 AT_SetUiccPrfApp(AT_CARDAPP_Uint32 cardApp, USIMM_NvCardappUint32 cardAPP, ModemIdUint16 modemId)
{
    USIMM_AppPriorityConfig appInfo;
    TAF_UINT32              appHit      = 0;
    TAF_UINT32              appOrderPos = 0;
    TAF_UINT32              rslt;
    TAF_UINT32              i;
    errno_t                 returnValue;

    (VOS_VOID)memset_s(&appInfo, (VOS_SIZE_T)sizeof(appInfo), 0x00, (VOS_SIZE_T)sizeof(appInfo));

    rslt = TAF_ACORE_NV_READ(modemId, NV_ITEM_USIM_UICC_APP_PRIORITY_CFG, &appInfo, sizeof(USIMM_AppPriorityConfig));

    if (rslt != NV_OK) {
        AT_ERR_LOG("AT_SetUiccPrfApp: Get NV_ITEM_USIM_UICC_APP_PRIORITY_CFG fail.");

        return AT_ERROR;
    }

    /* 防止NV数据异常导致越界访问 */
    if (appInfo.appNum > AT_ARRAY_SIZE(appInfo.appList)) {
        AT_WARN_LOG("AT_SetUiccPrfApp: Get NV_ITEM_USIM_UICC_APP_PRIORITY_CFG success, but ucAppNum invalid.");

        return AT_ERROR;
    }

    /* 设置CDMA应用优先 */
    for (i = 0; i < appInfo.appNum; i++) {
        if (cardAPP == appInfo.appList[i]) {
            appHit      = VOS_TRUE;
            appOrderPos = i;
            break;
        }
    }

    if (appHit == VOS_FALSE) {
        /* 没有找到，插入到最前边 */
        if (appInfo.appNum >= USIMM_NV_CARDAPP_BUTT) {
            rslt = AT_ERROR;
        } else {
            if (appInfo.appNum > 0) {
                returnValue = memmove_s(&appInfo.appList[1], sizeof(appInfo.appList) - 1 * sizeof(appInfo.appList[1]),
                                        &appInfo.appList[0], (sizeof(VOS_UINT32) * appInfo.appNum));
                TAF_MEM_CHK_RTN_VAL(returnValue, sizeof(appInfo.appList) - 1 * sizeof(appInfo.appList[1]),
                                    sizeof(VOS_UINT32) * appInfo.appNum);
            }
            appInfo.appList[0] = cardAPP;
            appInfo.appNum++;

            rslt = TAF_ACORE_NV_WRITE(modemId, NV_ITEM_USIM_UICC_APP_PRIORITY_CFG, (VOS_UINT8 *)&appInfo,
                                      sizeof(USIMM_AppPriorityConfig));
        }
    } else {
        if (appOrderPos != 0) {
            /* 从第一个起，往后移动i个 */
            returnValue = memmove_s(&appInfo.appList[1], sizeof(appInfo.appList) - 1 * sizeof(appInfo.appList[1]),
                                    &appInfo.appList[0], (sizeof(VOS_UINT32) * appOrderPos));
            TAF_MEM_CHK_RTN_VAL(returnValue, sizeof(appInfo.appList) - 1 * sizeof(appInfo.appList[1]),
                                sizeof(VOS_UINT32) * appOrderPos);

            appInfo.appList[0] = cardAPP;

            rslt = TAF_ACORE_NV_WRITE(modemId, NV_ITEM_USIM_UICC_APP_PRIORITY_CFG, (VOS_UINT8 *)&appInfo,
                                      sizeof(USIMM_AppPriorityConfig));
        }
    }

    if (rslt != NV_OK) {
        AT_ERR_LOG("TAF_ACORE_NV_WRITE: Write NV Fail.");
        return AT_ERROR;
    }

    return AT_OK;
}


TAF_UINT32 At_SetUiccPrfAppPara(TAF_UINT8 indexNum)
{
    ModemIdUint16 modemId;
    TAF_UINT32    rslt = AT_OK;

    /* 参数过多 */
    if (g_atParaIndex > 1) {
        AT_ERR_LOG("At_SetUiccPrfAppPara: too many para");

        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 根据AT命令通道号得到MODEM ID */
    if (AT_GetModemIdFromClient(indexNum, &modemId) != VOS_OK) {
        AT_ERR_LOG("At_SetUiccPrfAppPara: Get modem id fail.");

        return AT_ERROR;
    }

    if (g_atParaList[0].paraValue == AT_PREFER_APP_CDMA) {
        rslt = AT_SetUiccPrfApp(AT_PREFER_APP_CDMA, USIMM_NV_CDMA_APP, modemId);
    } else {
        rslt = AT_SetUiccPrfApp(AT_PREFER_APP_GUTL, USIMM_NV_GUTL_APP, modemId);
    }

    return rslt;
}


TAF_UINT32 At_QryUiccPrfAppPara(TAF_UINT8 indexNum)
{
    TAF_UINT32              i;
    TAF_UINT32              cdmaHit;
    TAF_UINT32              gutlHit;
    USIMM_AppPriorityConfig appInfo;
    ModemIdUint16           modemId;
    VOS_UINT16              length;

    /* 根据AT命令通道号得到MODEM ID */
    if (AT_GetModemIdFromClient(indexNum, &modemId) != VOS_OK) {
        AT_ERR_LOG("At_QryUiccPrfAppPara: Get modem id fail.");

        return AT_ERROR;
    }

    (VOS_VOID)memset_s(&appInfo, (VOS_SIZE_T)sizeof(appInfo), 0x00, (VOS_SIZE_T)sizeof(appInfo));

    if (TAF_ACORE_NV_READ(modemId, NV_ITEM_USIM_UICC_APP_PRIORITY_CFG, &appInfo, sizeof(USIMM_AppPriorityConfig)) !=
        NV_OK) {
        AT_ERR_LOG("At_QryUiccPrfAppPara: Get NV_ITEM_USIM_UICC_APP_PRIORITY_CFG fail.");

        return AT_ERROR;
    }

    /* 防止NV数据异常导致越界访问 */
    if (appInfo.appNum > AT_ARRAY_SIZE(appInfo.appList)) {
        AT_WARN_LOG("At_QryUiccPrfAppPara: Get NV_ITEM_USIM_UICC_APP_PRIORITY_CFG success, but ucAppNum invalid.");

        appInfo.appNum = AT_ARRAY_SIZE(appInfo.appList);
    }

    cdmaHit = VOS_FALSE;
    gutlHit = VOS_FALSE;

    /* 查找CDMA和GUTL应用在NV项中谁优先 */
    for (i = 0; i < appInfo.appNum; i++) {
        if (appInfo.appList[i] == USIMM_NV_GUTL_APP) {
            gutlHit = VOS_TRUE;

            break;
        }

        if (appInfo.appList[i] == USIMM_NV_CDMA_APP) {
            cdmaHit = VOS_TRUE;

            break;
        }
    }

    if (gutlHit == VOS_TRUE) {
        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: 1", g_parseContext[indexNum].cmdElement->cmdName);

        g_atSendDataBuff.bufLen = length;

        return AT_OK;
    } else if (cdmaHit == VOS_TRUE) {
        length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: 0", g_parseContext[indexNum].cmdElement->cmdName);

        g_atSendDataBuff.bufLen = length;

        return AT_OK;
    } else {
        return AT_ERROR;
    }
}


TAF_UINT32 At_TestUiccPrfAppPara(TAF_UINT8 indexNum)
{
    VOS_UINT16 length;

    length = 0;

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s: (0,1)", g_parseContext[indexNum].cmdElement->cmdName);

    g_atSendDataBuff.bufLen = length;

    return AT_OK;
}
#endif

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
/*
 * 功能描述: ^CCIMI
 * 修改历史:
 *  1.日    期: 2015年6月17日
 *    修改内容: 新生成函数
 */
TAF_UINT32 At_SetCCimiPara(TAF_UINT8 indexNum)
{
    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_CMD_NO_PARA) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 执行命令操作 */
    if (SI_PIH_CCimiSetReq(g_atClientTab[indexNum].clientId, 0) == AT_SUCCESS) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CCIMI_SET;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    } else {
        return AT_ERROR;
    }
}
#endif
/*
 * 功能描述: ^USIMMEX
 * 修改历史:
 *  1.日    期: 2016年2月22日
 *    修改内容: 新生成函数
 */
TAF_UINT16 At_CardErrorInfoInd(TAF_UINT8 indexNum, SI_PIH_EventInfo *event)
{
    VOS_UINT16 length = 0;
    VOS_UINT32 i;

    if (event->pihEvent.usimmErrorInd.errNum == VOS_NULL) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s^USIMMEX: NULL\r\n", g_atCrLf);

        return length;
    }

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s^USIMMEX: ", g_atCrLf);

    event->pihEvent.usimmErrorInd.errNum = AT_MIN(event->pihEvent.usimmErrorInd.errNum, USIMM_ERROR_INFO_MAX);
    for (i = 0; i < event->pihEvent.usimmErrorInd.errNum; i++) {
        if (i == 0) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "0x%X", event->pihEvent.usimmErrorInd.errInfo[i]);
        } else {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",0x%X", event->pihEvent.usimmErrorInd.errInfo[i]);
        }
    }

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);

    return length;
}

/*
 * 功能描述: ^USIMICCID
 * 修改历史:
 *  1.日    期: 2016年2月22日
 *    修改内容: 新生成函数
 */
TAF_UINT16 At_CardIccidInfoInd(TAF_UINT8 indexNum, SI_PIH_EventInfo *event)
{
    VOS_UINT16 length = 0;

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s^USIMICCID: ", g_atCrLf);

    length += (TAF_UINT16)AT_Hex2AsciiStrLowHalfFirst(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                      (TAF_UINT8 *)g_atSndCodeAddress + length,
                                                      (TAF_UINT8 *)event->pihEvent.iccidContent, USIMM_ICCID_FILE_LEN);

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);

    return length;
}


TAF_UINT16 At_CardStatusInd(TAF_UINT8 indexNum, SI_PIH_EventInfo *event)
{
    VOS_UINT16 length = 0;
    VOS_UINT16 tmpLen;

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s^CARDSTATUSIND: ", g_atCrLf);

    /* 卡状态输出时，跳过消息头和消息名称 */
    tmpLen = (VOS_MSG_HEAD_LENGTH + sizeof(TAF_UINT32));

    length += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                  (TAF_UINT8 *)g_atSndCodeAddress + length,
                                                  (((TAF_UINT8 *)&event->pihEvent.cardStatusInd) + tmpLen),
                                                  (sizeof(USIMM_CardStatusInd) - tmpLen));

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);

    return length;
}


TAF_UINT16 At_SimHotPlugStatusInd(TAF_UINT8 indexNum, SI_PIH_EventInfo *event)
{
    VOS_UINT16 length = 0;

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "%s^SIMHOTPLUG: %d%s", g_atCrLf, event->pihEvent.simHotPlugStatus,
        g_atCrLf);
    return length;
}


TAF_UINT16 At_PrintPrivateCglaResult(TAF_UINT8 indexNum, SI_PIH_EventInfo *event)
{
    VOS_UINT16 length = 0;

    /* 非最后一条打印上报IND，需要在行首添加回车换行 */
    if (event->pihEvent.cglaHandleCnf.lastDataFlag != VOS_TRUE) {
        length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "\r\n");
    }

    /* ^CGLA: <flag>,<length>,"[<command>]<SW1><SW2>" */
    length +=
        (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + length, "%s: %d,%d,\"", g_parseContext[indexNum].cmdElement->cmdName,
            event->pihEvent.cglaHandleCnf.lastDataFlag,
            (event->pihEvent.cglaHandleCnf.len + AT_DOUBLE_QUO_MARKS_LENGTH) * AT_DOUBLE_LENGTH);

    if (event->pihEvent.cglaHandleCnf.len != 0) {
        /* <command>, */
        length += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                      (TAF_UINT8 *)g_atSndCodeAddress + length,
                                                      event->pihEvent.cglaHandleCnf.command,
                                                      event->pihEvent.cglaHandleCnf.len);
    }

    /* <SW1> */
    length += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                  (TAF_UINT8 *)g_atSndCodeAddress + length,
                                                  &event->pihEvent.cglaHandleCnf.sw1, (VOS_UINT16)sizeof(TAF_UINT8));

    /* <SW2> */
    length += (TAF_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (TAF_INT8 *)g_atSndCodeAddress,
                                                  (TAF_UINT8 *)g_atSndCodeAddress + length,
                                                  &event->pihEvent.cglaHandleCnf.sw2, (VOS_UINT16)sizeof(TAF_UINT8));
    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + length, "\"");

    return length;
}


TAF_UINT16 At_SWCheckStatusInd(SI_PIH_EventInfo *event)
{
    VOS_UINT16 length = 0;

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s^SWCHECK: %d%s", g_atCrLf, event->pihEvent.apduSWCheckResult, g_atCrLf);
    return length;
}


TAF_UINT16 At_UpdateFileAtInd(SI_PIH_EventInfo *event)
{
    VOS_UINT16 length = 0;

    length += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s^CARDFETCHIND%s", g_atCrLf, g_atCrLf);
    return length;
}

#if (FEATURE_MBB_CUST == FEATURE_ON)
#if (FEATURE_PHONE_USIM == FEATURE_OFF)
/*
 * 功能描述: ^SIMREFRESH
 * 修改历史:
 *  1.日    期: 2020年5月18日
 *    修改内容: 新生成函数
 */
TAF_UINT16 AT_SimRefreshInd(SI_PIH_EventInfo *event)
{
    TAF_UINT16 length;

    length = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s^SIMREFRESH: %d%s", g_atCrLf, event->pihEvent.refreshType, g_atCrLf);
    return length;
}

/*
 * 功能描述: ^NOCARD
 * 修改历史:
 *  1.日    期: 2020年5月21日
 *    修改内容: 新生成函数
 */
VOS_UINT32 AT_ProcNoCardSetCnf(VOS_UINT8 index, SI_PIH_EventInfo *event, VOS_UINT16 *length)
{
    VOS_UINT32 result;
    AT_USIM_InfoCtx *usimInfoCtx = VOS_NULL_PTR;
    ModemIdUint16 modemId;

    if ((event == VOS_NULL_PTR) || (length == VOS_NULL_PTR) ||
        (g_atClientTab[index].cmdCurrentOpt != AT_CMD_NOCARD_SET)) {
        return AT_ERROR;
    }

    result = AT_GetModemIdFromClient(index, &modemId);
    if ((result != VOS_OK) || (event->pihError != VOS_OK)) {
        return AT_ERROR;
    }

    /* 更新当前NOCARD的状态 */
    usimInfoCtx = AT_GetUsimInfoCtxFromModemId(modemId);
    usimInfoCtx->noCardMode = event->pihEvent.noCardMode;
    return AT_OK;
}

#endif
#endif

