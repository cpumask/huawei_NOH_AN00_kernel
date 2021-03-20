
#include "at_ltev_msg_proc.h"
#include "ATCmdProc.h"
#include "vos.h"
#include "ps_common_def.h"
#include "securec.h"
#include "mn_comm_api.h"

#define THIS_FILE_ID PS_FILE_ID_TAF_V2X_API_C

#if (FEATURE_LTEV == FEATURE_ON)


VOS_UINT32 VRRC_SetGnssInfo(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId)
{
    /* 发送消息给VRRC模块 */
    VOS_UINT32      rslt;
    AT_VRRC_ReqMsg *msg = VOS_NULL_PTR;

    /* 申请消息 */
    msg = (AT_VRRC_ReqMsg *)TAF_AllocMsgWithoutHeaderLen(WUEPS_PID_AT, sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH);
    if (msg == VOS_NULL_PTR) {
        AT_ERR_LOG("VRRC_SetGnssInfo: ALLOC MSG FAIL.");
        return VOS_ERR;
    }

    (VOS_VOID)memset_s((VOS_UINT8 *)msg + VOS_MSG_HEAD_LENGTH, sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH, 0x00,
                       sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH);

    msg->clientId      = clientId;
    msg->opId          = opId;
    msg->msgName       = VRRC_MSG_GNSS_TEST_START_REQ;
    msg->ulSenderPid   = WUEPS_PID_AT;
    msg->ulReceiverPid = AT_GetDestPid(clientId, I0_PS_PID_VERRC);

    rslt = TAF_TraceAndSendMsg(WUEPS_PID_AT, msg);
    if (rslt != VOS_OK) {
        AT_ERR_LOG("VRRC_SetGnssInfo: SEND MSG FAIL.");
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 VRRC_QryGnssInfo(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId)
{
    /* 发送消息给VRRC模块 */
    VOS_UINT32      rslt;
    AT_VRRC_ReqMsg *msg = VOS_NULL_PTR;

    /* 申请消息 */
    msg = (AT_VRRC_ReqMsg *)TAF_AllocMsgWithoutHeaderLen(WUEPS_PID_AT, sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH);
    if (msg == VOS_NULL_PTR) {
        AT_ERR_LOG("VRRC_QryGnssInfo: ALLOC MSG FAIL.");
        return VOS_ERR;
    }

    (VOS_VOID)memset_s((VOS_UINT8 *)msg + VOS_MSG_HEAD_LENGTH, sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH, 0x00,
                         sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH);

    msg->clientId      = clientId;
    msg->opId          = opId;
    msg->msgName       = VRRC_MSG_GNSS_INFO_QRY_REQ;
    msg->ulSenderPid   = WUEPS_PID_AT;
    msg->ulReceiverPid = AT_GetDestPid(clientId, I0_PS_PID_VERRC);

    rslt = TAF_TraceAndSendMsg(WUEPS_PID_AT, msg);
    if (rslt != VOS_OK) {
        AT_ERR_LOG("VRRC_QryGnssInfo: SEND MSG FAIL.");
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 VRRC_QrySideLinkInfo(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId)
{
    /* 发送消息给VRRC模块 */
    VOS_UINT32      rslt;
    AT_VRRC_ReqMsg *msg = VOS_NULL_PTR;

    /* Allocating memory for message */
    msg = (AT_VRRC_ReqMsg *)TAF_AllocMsgWithoutHeaderLen(WUEPS_PID_AT, sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH);
    if (msg == VOS_NULL_PTR) {
        return VOS_ERR;
    }

    (VOS_VOID)memset_s((VOS_UINT8 *)msg + VOS_MSG_HEAD_LENGTH, sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH, 0x00,
                       sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH);

    msg->clientId      = clientId;
    msg->opId          = opId;
    msg->msgName       = VRRC_MSG_RSU_SLINFO_QRY_REQ;
    msg->ulSenderPid   = WUEPS_PID_AT;
    msg->ulReceiverPid = AT_GetDestPid(clientId, I0_PS_PID_VERRC);

    rslt = TAF_TraceAndSendMsg(WUEPS_PID_AT, msg);
    if (rslt != VOS_OK) {
        AT_ERR_LOG("VRRC_QrySideLinkInfo: SEND MSG FAIL");
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 VRRC_SetSideLinkInfo(VOS_UINT32 senderPid, MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId,
                                const AT_VRRC_Slinfo *setParams)
{
    /* 发送消息给VRRC模块 */
    VOS_UINT32      rslt;
    AT_VRRC_ReqMsg *msg    = VOS_NULL_PTR;
    AT_VRRC_Slinfo *params = VOS_NULL_PTR;
    VOS_UINT32      length;

    /* Allocating memory for message */
    length = sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH - sizeof(VOS_UINT32) + sizeof(AT_VRRC_Slinfo);
    msg    = (AT_VRRC_ReqMsg *)TAF_AllocMsgWithoutHeaderLen(WUEPS_PID_AT, length);
    if (msg == VOS_NULL_PTR) {
        return VOS_ERR;
    }

    (VOS_VOID)memset_s((VOS_UINT8 *)msg + VOS_MSG_HEAD_LENGTH, length, 0x00, length);

    msg->clientId      = clientId;
    msg->opId          = opId;
    msg->msgName       = VRRC_MSG_RSU_SLINFO_SET_REQ;
    msg->ulSenderPid   = WUEPS_PID_AT;
    msg->ulReceiverPid = AT_GetDestPid(clientId, I0_PS_PID_VERRC);

    params                   = (AT_VRRC_Slinfo *)msg->content;
    params->frequencyBand    = setParams->frequencyBand;
    params->bandWidth        = setParams->bandWidth;
    params->centralFrequency = setParams->centralFrequency;

    rslt = TAF_TraceAndSendMsg(WUEPS_PID_AT, msg);
    if (rslt != VOS_OK) {
        AT_ERR_LOG("VRRC_QrySideLinkInfo: SEND MSG FAIL");
        return VOS_ERR;
    }

    return VOS_OK;
}

VOS_UINT32 VRRC_QryRsuPhyr(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId)
{
    /* 发送消息给VRRC模块 */
    VOS_UINT32      rslt;
    AT_VRRC_ReqMsg *msg = VOS_NULL_PTR;

    /* 申请消息 */
    msg = (AT_VRRC_ReqMsg *)TAF_AllocMsgWithoutHeaderLen(WUEPS_PID_AT, sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH);
    if (msg == VOS_NULL_PTR) {
        AT_ERR_LOG("VRRC_QryRsuPhyr: ALLOC MSG FAIL.");
        return VOS_ERR;
    }

    (VOS_VOID)memset_s((VOS_UINT8 *)msg + VOS_MSG_HEAD_LENGTH, sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH, 0x00,
                       sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH);

    msg->clientId      = clientId;
    msg->opId          = opId;
    msg->msgName       = VRRC_MSG_RSU_PHYR_QRY_REQ;
    msg->ulSenderPid   = WUEPS_PID_AT;
    msg->ulReceiverPid = AT_GetDestPid(clientId, I0_PS_PID_VERRC);

    rslt = TAF_TraceAndSendMsg(WUEPS_PID_AT, msg);
    if (rslt != VOS_OK) {
        AT_ERR_LOG("VRRC_QryRsuPhyr: SEND MSG FAIL.");
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 VRRC_SetRsuPhyr(VOS_UINT32 ulSenderPid, MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId,
                           const AT_VRRC_PhyrSet *setParams)
{
    /* 发送给VRRC模块的消息 */
    VOS_UINT32       rslt;
    AT_VRRC_ReqMsg  *msg    = VOS_NULL_PTR;
    AT_VRRC_PhyrSet *params = VOS_NULL_PTR;
    VOS_UINT32       length;
    errno_t          memResult;

    /* 申请消息内存空间 */
    length = sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH - sizeof(VOS_UINT32) + sizeof(AT_VRRC_PhyrSet);

    msg = (AT_VRRC_ReqMsg *)TAF_AllocMsgWithoutHeaderLen(WUEPS_PID_AT, length);

    if (msg == VOS_NULL_PTR) {
        return VOS_ERR;
    }

    (VOS_VOID)memset_s((VOS_UINT8 *)msg + VOS_MSG_HEAD_LENGTH, length, 0x00, length);

    msg->clientId      = clientId;
    msg->opId          = opId;
    msg->msgName       = VRRC_MSG_RSU_PHYR_SET_REQ;
    msg->ulSenderPid   = WUEPS_PID_AT;
    msg->ulReceiverPid = AT_GetDestPid(clientId, I0_PS_PID_VERRC);

    params    = (AT_VRRC_PhyrSet *)msg->content;
    memResult = memcpy_s(params, sizeof(AT_VRRC_PhyrSet), setParams, sizeof(AT_VRRC_PhyrSet));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(AT_VRRC_PhyrSet), sizeof(AT_VRRC_PhyrSet));

    rslt = TAF_TraceAndSendMsg(WUEPS_PID_AT, msg);

    if (rslt != VOS_OK) {
        AT_ERR_LOG("VRRC_SetRsuPhyr:SEND MSG FAIL");
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 VRRC_QryRsuVphyStat(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId)
{
    /* 发送消息给VRRC模块 */
    VOS_UINT32      rslt;
    AT_VRRC_ReqMsg *msg = VOS_NULL_PTR;

    /* 申请消息 */
    msg = (AT_VRRC_ReqMsg *)TAF_AllocMsgWithoutHeaderLen(WUEPS_PID_AT, sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH);
    if (msg == VOS_NULL_PTR) {
        AT_ERR_LOG("VRRC_QryRsuVphyStat: ALLOC MSG FAIL.");
        return VOS_ERR;
    }

    (VOS_VOID)memset_s((VOS_UINT8 *)msg + VOS_MSG_HEAD_LENGTH, sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH, 0x00,
                       sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH);

    msg->clientId      = clientId;
    msg->opId          = opId;
    msg->msgName       = VRRC_MSG_RSU_VPHYSTAT_QRY_REQ;
    msg->ulSenderPid   = WUEPS_PID_AT;
    msg->ulReceiverPid = AT_GetDestPid(clientId, I0_PS_PID_VERRC);

    rslt = TAF_TraceAndSendMsg(WUEPS_PID_AT, msg);
    if (rslt != VOS_OK) {
        AT_ERR_LOG("VRRC_QryRsuVphyStat: SEND MSG FAIL!");
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 VRRC_SetRsuVPhyStatClr(VOS_UINT32 ulSenderPid, MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId,
                                  const AT_VRRC_VphystatClrSet *setParams)
{
    /* 发送给VRRC模块的消息 */
    VOS_UINT32              rslt;
    AT_VRRC_ReqMsg         *msg    = VOS_NULL_PTR;
    AT_VRRC_VphystatClrSet *params = VOS_NULL_PTR;
    VOS_UINT32              length;
    errno_t                 memResult;

    /* 申请消息内存空间 */
    length = sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH - sizeof(VOS_UINT32) + sizeof(AT_VRRC_VphystatClrSet);

    msg = (AT_VRRC_ReqMsg *)TAF_AllocMsgWithoutHeaderLen(WUEPS_PID_AT, length);

    if (msg == VOS_NULL_PTR) {
        return VOS_ERR;
    }

    (VOS_VOID)memset_s((VOS_UINT8 *)msg + VOS_MSG_HEAD_LENGTH, length, 0x00, length);

    msg->clientId      = clientId;
    msg->opId          = opId;
    msg->msgName       = VRRC_MSG_RSU_VPHYSTAT_CLR_SET_REQ;
    msg->ulSenderPid   = WUEPS_PID_AT;
    msg->ulReceiverPid = AT_GetDestPid(clientId, I0_PS_PID_VERRC);

    params    = (AT_VRRC_VphystatClrSet *)msg->content;
    memResult = memcpy_s(params, sizeof(AT_VRRC_VphystatClrSet), setParams, sizeof(AT_VRRC_VphystatClrSet));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(AT_VRRC_VphystatClrSet), sizeof(AT_VRRC_VphystatClrSet));

    rslt = TAF_TraceAndSendMsg(WUEPS_PID_AT, msg);

    if (rslt != VOS_OK) {
        AT_ERR_LOG("VRRC_SetRsuVPhyStatClr:SEND MSG FAIL");
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 VRRC_QryRsuVSnrRsrp(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId)
{
    /* 发送消息给VRRC模块 */
    VOS_UINT32      rslt;
    AT_VRRC_ReqMsg *msg = VOS_NULL_PTR;
    errno_t         memResult;

    /* 申请消息 */
    msg = (AT_VRRC_ReqMsg *)TAF_AllocMsgWithoutHeaderLen(WUEPS_PID_AT, sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH);
    if (msg == VOS_NULL_PTR) {
        AT_ERR_LOG("VRRC_QryRsuVSnrRsrp: ALLOC MSG FAIL.");
        return VOS_ERR;
    }

    memResult = memset_s((VOS_UINT8 *)msg + VOS_MSG_HEAD_LENGTH, sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH, 0x00,
                         sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH,
                        sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH);

    msg->clientId      = clientId;
    msg->opId          = opId;
    msg->msgName       = VRRC_MSG_RSU_VSNRRSRP_QRY_REQ;
    msg->ulSenderPid   = WUEPS_PID_AT;
    msg->ulReceiverPid = AT_GetDestPid(clientId, I0_PS_PID_VERRC);

    rslt = TAF_TraceAndSendMsg(WUEPS_PID_AT, msg);
    if (rslt != VOS_OK) {
        AT_ERR_LOG("VRRC_QryRsuVSnrRsrp: SEND MSG FAIL!");
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 VRRC_QryRsuV2xRssi(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId)
{
    /* 发送消息给VRRC模块 */
    VOS_UINT32      rslt;
    AT_VRRC_ReqMsg *msg = VOS_NULL_PTR;
    errno_t         memResult;

    /* 申请消息 */
    msg = (AT_VRRC_ReqMsg *)TAF_AllocMsgWithoutHeaderLen(WUEPS_PID_AT, sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH);
    if (msg == VOS_NULL_PTR) {
        AT_ERR_LOG("VRRC_QryRsuV2xRssi: ALLOC MSG FAIL.");
        return VOS_ERR;
    }

    memResult = memset_s((VOS_UINT8 *)msg + VOS_MSG_HEAD_LENGTH, sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH, 0x00,
                         sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH,
                        sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH);

    msg->clientId      = clientId;
    msg->opId          = opId;
    msg->msgName       = VRRC_MSG_RSU_RSSI_QRY_REQ;
    msg->ulSenderPid   = WUEPS_PID_AT;
    msg->ulReceiverPid = AT_GetDestPid(clientId, I0_PS_PID_VERRC);

    rslt = TAF_TraceAndSendMsg(WUEPS_PID_AT, msg);
    if (rslt != VOS_OK) {
        AT_ERR_LOG("VRRC_QryRsuV2xRssi: SEND MSG FAIL!");
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 VMAC_QryVRssi(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId)
{
    /* 发送消息给VMAC模块 */
    VOS_UINT32      rslt;
    AT_VMAC_ReqMsg *msg = VOS_NULL_PTR;
    errno_t         memResult;

    /* 申请消息 */
    msg = (AT_VMAC_ReqMsg *)TAF_AllocMsgWithoutHeaderLen(WUEPS_PID_AT, sizeof(AT_VMAC_ReqMsg) - VOS_MSG_HEAD_LENGTH);
    if (msg == VOS_NULL_PTR) {
        AT_ERR_LOG("VMAC_QryVRssi: ALLOC MSG FAIL.");
        return VOS_ERR;
    }

    memResult = memset_s((VOS_UINT8 *)msg + VOS_MSG_HEAD_LENGTH, sizeof(AT_VMAC_ReqMsg) - VOS_MSG_HEAD_LENGTH, 0x00,
                         sizeof(AT_VMAC_ReqMsg) - VOS_MSG_HEAD_LENGTH);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(AT_VMAC_ReqMsg) - VOS_MSG_HEAD_LENGTH,
                        sizeof(AT_VMAC_ReqMsg) - VOS_MSG_HEAD_LENGTH);

    msg->clientId      = clientId;
    msg->opId          = opId;
    msg->msgName       = VMAC_MSG_RSU_VRSSI_QRY_REQ;
    msg->ulSenderPid   = WUEPS_PID_AT;
    msg->ulReceiverPid = AT_GetDestPid(clientId, I0_PS_PID_VMAC_UL);

    rslt = TAF_TraceAndSendMsg(WUEPS_PID_AT, msg);

    if (rslt != VOS_OK) {
        AT_ERR_LOG("VMAC_QryVRssi: SEND MSG FAIL.");
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 VPDCP_QryPtrRpt(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId)
{
    /* 发送消息给VMAC模块 */
    VOS_UINT32       rslt;
    AT_VPDCP_ReqMsg *msg = VOS_NULL_PTR;
    errno_t          memResult;

    /* 申请消息 */
    msg = (AT_VPDCP_ReqMsg *)TAF_AllocMsgWithoutHeaderLen(WUEPS_PID_AT, sizeof(AT_VPDCP_ReqMsg) - VOS_MSG_HEAD_LENGTH);
    if (msg == VOS_NULL_PTR) {
        AT_ERR_LOG("VPDCP_QryPtrRpt: ALLOC MSG FAIL.");
        return VOS_ERR;
    }

    memResult = memset_s((VOS_UINT8 *)msg + VOS_MSG_HEAD_LENGTH, sizeof(AT_VPDCP_ReqMsg) - VOS_MSG_HEAD_LENGTH, 0x00,
                         sizeof(AT_VPDCP_ReqMsg) - VOS_MSG_HEAD_LENGTH);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(AT_VPDCP_ReqMsg) - VOS_MSG_HEAD_LENGTH,
                        sizeof(AT_VPDCP_ReqMsg) - VOS_MSG_HEAD_LENGTH);

    msg->clientId      = clientId;
    msg->opId          = opId;
    msg->msgName       = VPDCP_MSG_RSU_PTRRPT_QRY_REQ;
    msg->ulSenderPid   = WUEPS_PID_AT;
    msg->ulReceiverPid = AT_GetDestPid(clientId, I0_PS_PID_VPDCP_UL);

    rslt = TAF_TraceAndSendMsg(WUEPS_PID_AT, msg);

    if (rslt != VOS_OK) {
        AT_ERR_LOG("VPDCP_QryPtrRpt: SEND MSG FAIL.");
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 VPDCP_SetPtrRpt(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId)
{
    /* 发送消息给VRRC模块 */
    VOS_UINT32       rslt;
    AT_VPDCP_ReqMsg *msg = VOS_NULL_PTR;
    VOS_UINT32       length;
    errno_t          memResult;

    /* Allocating memory for message */
    length = sizeof(AT_VPDCP_ReqMsg) - VOS_MSG_HEAD_LENGTH;
    msg    = (AT_VPDCP_ReqMsg *)TAF_AllocMsgWithoutHeaderLen(WUEPS_PID_AT, length);
    if (msg == VOS_NULL_PTR) {
        return VOS_ERR;
    }

    memResult = memset_s((VOS_UINT8 *)msg + VOS_MSG_HEAD_LENGTH, length, 0x00, length);
    TAF_MEM_CHK_RTN_VAL(memResult, length, length);

    msg->clientId      = clientId;
    msg->opId          = opId;
    msg->msgName       = VPDCP_MSG_RSU_PTRRPT_SET_REQ;
    msg->ulSenderPid   = WUEPS_PID_AT;
    msg->ulReceiverPid = AT_GetDestPid(clientId, I0_PS_PID_VPDCP_UL);
    msg->content[0]    = 1;

    rslt = TAF_TraceAndSendMsg(WUEPS_PID_AT, msg);
    if (rslt != VOS_OK) {
        AT_ERR_LOG("VPDCP_SetPtrRpt: SEND MSG FAIL");
        return VOS_ERR;
    }

    return VOS_OK;
}

VOS_UINT32 VRRC_QryV2xResPoolPara(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId,
                                  const AT_VRRC_RppcfgQry *pstQryParams)
{
    /* 发送消息给VRRC模块 */
    VOS_UINT32         rslt;
    AT_VRRC_ReqMsg    *msg    = VOS_NULL_PTR;
    AT_VRRC_RppcfgQry *params = VOS_NULL_PTR;
    VOS_UINT32         length;
    errno_t            memResult;

    /* Allocating memory for message */
    length = sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH - sizeof(VOS_UINT32) + sizeof(AT_VRRC_RppcfgQry);
    msg    = (AT_VRRC_ReqMsg *)TAF_AllocMsgWithoutHeaderLen(WUEPS_PID_AT, length);
    if (msg == VOS_NULL_PTR) {
        return VOS_ERR;
    }

    memResult = memset_s((VOS_UINT8 *)msg + VOS_MSG_HEAD_LENGTH, length, 0x00, length);
    TAF_MEM_CHK_RTN_VAL(memResult, length, length);

    msg->clientId      = clientId;
    msg->opId          = opId;
    msg->msgName       = VRRC_MSG_RPPCFG_QRY_REQ;
    msg->ulSenderPid   = WUEPS_PID_AT;
    msg->ulReceiverPid = AT_GetDestPid(clientId, I0_PS_PID_VERRC);

    params    = (AT_VRRC_RppcfgQry *)msg->content;
    memResult = memcpy_s(params, sizeof(AT_VRRC_RppcfgQry), pstQryParams, sizeof(AT_VRRC_RppcfgQry));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(AT_VRRC_RppcfgQry), sizeof(AT_VRRC_RppcfgQry));

    rslt = TAF_TraceAndSendMsg(WUEPS_PID_AT, msg);
    if (rslt != VOS_OK) {
        AT_ERR_LOG("VRRC_QryV2xResPoolPara: SEND MSG FAIL");
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 VRRC_SetV2xResPoolPara(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId, const AT_VRRC_RppcfgSet *setParams)
{
    /* 发送消息给VRRC模块 */
    VOS_UINT32         rslt;
    AT_VRRC_ReqMsg    *msg    = VOS_NULL_PTR;
    AT_VRRC_RppcfgSet *params = VOS_NULL_PTR;
    VOS_UINT32         length;
    errno_t            memResult;

    /* Allocating memory for message */
    length = sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH - sizeof(VOS_UINT32) + sizeof(AT_VRRC_RppcfgSet);
    msg    = (AT_VRRC_ReqMsg *)TAF_AllocMsgWithoutHeaderLen(WUEPS_PID_AT, length);
    if (msg == VOS_NULL_PTR) {
        return VOS_ERR;
    }

    memResult = memset_s((VOS_UINT8 *)msg + VOS_MSG_HEAD_LENGTH, length, 0x00, length);
    TAF_MEM_CHK_RTN_VAL(memResult, length, length);

    msg->clientId      = clientId;
    msg->opId          = opId;
    msg->msgName       = VRRC_MSG_RPPCFG_SET_REQ;
    msg->ulSenderPid   = WUEPS_PID_AT;
    msg->ulReceiverPid = AT_GetDestPid(clientId, I0_PS_PID_VERRC);

    params = (AT_VRRC_RppcfgSet *)msg->content;

    memResult = memcpy_s(params, sizeof(AT_VRRC_RppcfgSet), setParams, sizeof(AT_VRRC_RppcfgSet));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(AT_VRRC_RppcfgSet), sizeof(AT_VRRC_RppcfgSet));

    rslt = TAF_TraceAndSendMsg(WUEPS_PID_AT, msg);
    if (rslt != VOS_OK) {
        AT_ERR_LOG("VRRC_SetV2xResPoolPara: SEND MSG FAIL");
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 VRRC_GetGnssInfo(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId)
{
    /* 发送消息给VRRC模块 */
    VOS_UINT32      rslt;
    AT_VRRC_ReqMsg *msg = VOS_NULL_PTR;
    errno_t         memResult;

    /* 申请消息 */
    msg = (AT_VRRC_ReqMsg *)TAF_AllocMsgWithoutHeaderLen(WUEPS_PID_AT, sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH);
    if (msg == VOS_NULL_PTR) {
        AT_ERR_LOG("VRRC_GetGnssInfo: ALLOC MSG FAIL.");
        return VOS_ERR;
    }

    memResult = memset_s((VOS_UINT8 *)msg + VOS_MSG_HEAD_LENGTH, sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH, 0x00,
                         sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH,
                        sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH);

    msg->clientId      = clientId;
    msg->opId          = opId;
    msg->msgName       = VRRC_MSG_GNSS_INFO_GET_REQ;
    msg->ulSenderPid   = WUEPS_PID_AT;
    msg->ulReceiverPid = AT_GetDestPid(clientId, I0_PS_PID_VERRC);

    rslt = TAF_TraceAndSendMsg(WUEPS_PID_AT, msg);
    if (rslt != VOS_OK) {
        AT_ERR_LOG("VRRC_GetGnssInfo: SEND MSG FAIL.");
        return VOS_ERR;
    }

    return VOS_OK;
}

VOS_UINT32 VRRC_SetTxPower(VOS_UINT32 ulSenderPid, MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId,
                           const AT_VRRC_Set_Tx_PowerPara *setParams)
{
    /* 发送给VRRC模块的消息 */
    AT_VRRC_ReqMsg           *msg    = VOS_NULL_PTR;
    AT_VRRC_Set_Tx_PowerPara *params = VOS_NULL_PTR;
    VOS_UINT32                rslt;
    VOS_UINT32                length;
    errno_t                   memResult;

    /* 申请消息内存空间 */
    length = sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH - sizeof(VOS_UINT32) + sizeof(AT_VRRC_Set_Tx_PowerPara);

    msg = (AT_VRRC_ReqMsg *)PS_ALLOC_MSG(WUEPS_PID_AT, length);

    if (msg == VOS_NULL_PTR) {
        return VOS_ERR;
    }

    memResult = memset_s((VOS_UINT8 *)msg + VOS_MSG_HEAD_LENGTH, length, 0x00, length);
    TAF_MEM_CHK_RTN_VAL(memResult, length, length);

    msg->clientId      = clientId;
    msg->opId          = opId;
    msg->msgName       = AT_VRRC_SET_TX_POWER_REQ;
    msg->ulSenderPid   = WUEPS_PID_AT;
    msg->ulReceiverPid = AT_GetDestPid(clientId, I0_PS_PID_VERRC);

    params    = (AT_VRRC_Set_Tx_PowerPara *)msg->content;
    memResult = memcpy_s(params, sizeof(AT_VRRC_Set_Tx_PowerPara), setParams, sizeof(AT_VRRC_Set_Tx_PowerPara));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(AT_VRRC_Set_Tx_PowerPara), sizeof(AT_VRRC_Set_Tx_PowerPara));

    rslt = PS_SEND_MSG(WUEPS_PID_AT, msg);

    if (rslt != VOS_OK) {
        AT_ERR_LOG("VRRC_SetRsuPhyr:SEND MSG FAIL");
        return VOS_ERR;
    }

    return VOS_OK;
}


#if (FEATURE_LTEV_WL == FEATURE_ON)

VOS_UINT32 VRRC_QryRsuSyncStat(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId)
{
    /* 发送消息给VRRC模块 */
    VOS_UINT32      rslt;
    AT_VRRC_ReqMsg *msg = VOS_NULL_PTR;
    errno_t         memResult;
    /* 申请消息 */
    msg = (AT_VRRC_ReqMsg *)TAF_AllocMsgWithoutHeaderLen(WUEPS_PID_AT, sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH);
    if (msg == VOS_NULL_PTR) {
        AT_ERR_LOG("VRRC_QryRsuSyncStat: ALLOC MSG FAIL.");
        return VOS_ERR;
    }

    memResult = memset_s((VOS_UINT8 *)msg + VOS_MSG_HEAD_LENGTH, sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH, 0x00,
                         sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH,
                        sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH);

    msg->clientId      = clientId;
    msg->opId          = opId;
    msg->msgName       = VRRC_MSG_RSU_SYNCST_QRY_REQ;
    msg->ulSenderPid   = WUEPS_PID_AT;
    msg->ulReceiverPid = AT_GetDestPid(clientId, I0_PS_PID_VSYNC);

    rslt = TAF_TraceAndSendMsg(WUEPS_PID_AT, msg);
    if (rslt != VOS_OK) {
        AT_ERR_LOG("VRRC_QryRsuSyncStat: SEND MSG FAIL.");
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 VRRC_QryGnssSyncStat(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId)
{
    /* 发送消息给VRRC模块 */
    VOS_UINT32      rslt;
    AT_VRRC_ReqMsg *msg = VOS_NULL_PTR;
    errno_t         memResult;

    /* 申请消息 */
    msg = (AT_VRRC_ReqMsg *)TAF_AllocMsgWithoutHeaderLen(WUEPS_PID_AT, sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH);
    if (msg == VOS_NULL_PTR) {
        AT_ERR_LOG("VRRC_QryGnssSyncStat: ALLOC MSG FAIL.");
        return VOS_ERR;
    }

    memResult = memset_s((VOS_UINT8 *)msg + VOS_MSG_HEAD_LENGTH, sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH, 0x00,
                         sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH,
                        sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH);

    msg->clientId      = clientId;
    msg->opId          = opId;
    msg->msgName       = VRRC_MSG_RSU_GNSSSYNCST_QRY_REQ;
    msg->ulSenderPid   = WUEPS_PID_AT;
    msg->ulReceiverPid = AT_GetDestPid(clientId, I0_PS_PID_VSYNC);

    rslt = TAF_TraceAndSendMsg(WUEPS_PID_AT, msg);
    if (rslt != VOS_OK) {
        AT_ERR_LOG("VRRC_QryGnssSyncStat: SEND MSG FAIL.");
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 VMAC_SetMcgCfgPara(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId, const AT_VMAC_McgCfgSet *setParams)
{
    VOS_UINT32         rslt;
    VOS_UINT32         length;
    AT_VMAC_ReqMsg    *msg    = VOS_NULL_PTR;
    AT_VMAC_McgCfgSet *params = VOS_NULL_PTR;
    errno_t            memResult;

    /* Allocating memory for message */
    length = sizeof(AT_VMAC_ReqMsg) - VOS_MSG_HEAD_LENGTH - sizeof(VOS_UINT32) + sizeof(AT_VMAC_McgCfgSet);
    msg    = (AT_VMAC_ReqMsg *)TAF_AllocMsgWithoutHeaderLen(WUEPS_PID_AT, length);
    if (msg == VOS_NULL_PTR) {
        return VOS_ERR;
    }

    memResult = memset_s((VOS_UINT8 *)msg + VOS_MSG_HEAD_LENGTH, length, 0x00, length);
    TAF_MEM_CHK_RTN_VAL(memResult, length, length);

    msg->clientId      = clientId;
    msg->opId          = opId;
    msg->msgName       = VMAC_MSG_RSU_MCG_CFG_SET_REQ;
    msg->ulSenderPid   = WUEPS_PID_AT;
    msg->ulReceiverPid = AT_GetDestPid(clientId, I0_PS_PID_VMAC_UL);

    params = (AT_VMAC_McgCfgSet *)msg->content;

    memResult = memcpy_s(params, sizeof(AT_VMAC_McgCfgSet), setParams, sizeof(AT_VMAC_McgCfgSet));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(AT_VMAC_McgCfgSet), sizeof(AT_VMAC_McgCfgSet));

    rslt = TAF_TraceAndSendMsg(WUEPS_PID_AT, msg);
    if (rslt != VOS_OK) {
        AT_ERR_LOG("VMAC_SetMcgCfgPara: SEND MSG FAIL");
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 VMAC_SetDstIdTMPara(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId, const AT_VMAC_DstidtmSet *setParams)
{
    VOS_UINT32          rslt;
    VOS_UINT32          length;
    AT_VMAC_ReqMsg     *msg    = VOS_NULL_PTR;
    AT_VMAC_DstidtmSet *params = VOS_NULL_PTR;
    errno_t             memResult;

    /* Allocating memory for message */
    length = sizeof(AT_VMAC_ReqMsg) - VOS_MSG_HEAD_LENGTH - sizeof(VOS_UINT32) + sizeof(AT_VMAC_DstidtmSet);
    msg    = (AT_VMAC_ReqMsg *)TAF_AllocMsgWithoutHeaderLen(WUEPS_PID_AT, length);
    if (msg == VOS_NULL_PTR) {
        return VOS_ERR;
    }

    memResult = memset_s((VOS_UINT8 *)msg + VOS_MSG_HEAD_LENGTH, length, 0x00, length);
    TAF_MEM_CHK_RTN_VAL(memResult, length, length);

    msg->clientId      = clientId;
    msg->opId          = opId;
    msg->msgName       = VMAC_MSG_RSU_DSTIDTM_SET_REQ;
    msg->ulSenderPid   = WUEPS_PID_AT;
    msg->ulReceiverPid = AT_GetDestPid(clientId, I0_PS_PID_VMAC_UL);

    params = (AT_VMAC_DstidtmSet *)msg->content;

    memResult = memcpy_s(params, sizeof(AT_VMAC_DstidtmSet), setParams, sizeof(AT_VMAC_DstidtmSet));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(AT_VMAC_DstidtmSet), sizeof(AT_VMAC_DstidtmSet));

    rslt = TAF_TraceAndSendMsg(WUEPS_PID_AT, msg);
    if (rslt != VOS_OK) {
        AT_ERR_LOG("VMAC_SetDstIdTMPara: SEND MSG FAIL");
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 VRRC_SYNC_SetCfgPara(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId, const AT_VRRC_Pc5syncSet *setParams)
{
    VOS_UINT32          rslt;
    VOS_UINT32          length;
    AT_VRRC_ReqMsg     *msg    = VOS_NULL_PTR;
    AT_VRRC_Pc5syncSet *params = VOS_NULL_PTR;
    errno_t             memResult;

    /* Allocating memory for message */
    length = sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH - sizeof(VOS_UINT32) + sizeof(AT_VRRC_Pc5syncSet);
    msg    = (AT_VRRC_ReqMsg *)TAF_AllocMsgWithoutHeaderLen(WUEPS_PID_AT, length);
    if (msg == VOS_NULL_PTR) {
        return VOS_ERR;
    }

    memResult = memset_s((VOS_UINT8 *)msg + VOS_MSG_HEAD_LENGTH, length, 0x00, length);
    TAF_MEM_CHK_RTN_VAL(memResult, length, length);

    msg->clientId      = clientId;
    msg->opId          = opId;
    msg->msgName       = VRRC_MSG_PC5SYNC_SET_REQ;
    msg->ulSenderPid   = WUEPS_PID_AT;
    msg->ulReceiverPid = AT_GetDestPid(clientId, I0_PS_PID_VSYNC);

    params = (AT_VRRC_Pc5syncSet *)msg->content;

    memResult = memcpy_s(params, sizeof(AT_VRRC_Pc5syncSet), setParams, sizeof(AT_VRRC_Pc5syncSet));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(AT_VRRC_Pc5syncSet), sizeof(AT_VRRC_Pc5syncSet));

    rslt = TAF_TraceAndSendMsg(WUEPS_PID_AT, msg);
    if (rslt != VOS_OK) {
        AT_ERR_LOG("VRRC_SYNC_SetCfgPara: SEND MSG FAIL");
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 VRRC_SYNC_QryCfgPara(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId)
{
    /* 发送消息给VRRC模块 */
    VOS_UINT32      rslt;
    AT_VRRC_ReqMsg *msg = VOS_NULL_PTR;
    errno_t         memResult;

    /* 申请消息 */
    msg = (AT_VRRC_ReqMsg *)TAF_AllocMsgWithoutHeaderLen(WUEPS_PID_AT, sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH);
    if (msg == VOS_NULL_PTR) {
        AT_ERR_LOG("VRRC_SYNC_QryCfgPara: ALLOC MSG FAIL.");
        return VOS_ERR;
    }

    memResult = memset_s((VOS_UINT8 *)msg + VOS_MSG_HEAD_LENGTH, sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH, 0x00,
                         sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH,
                        sizeof(AT_VRRC_ReqMsg) - VOS_MSG_HEAD_LENGTH);

    msg->clientId      = clientId;
    msg->opId          = opId;
    msg->msgName       = VRRC_MSG_PC5SYNC_QRY_REQ;
    msg->ulSenderPid   = WUEPS_PID_AT;
    msg->ulReceiverPid = AT_GetDestPid(clientId, I0_PS_PID_VSYNC);

    rslt = TAF_TraceAndSendMsg(WUEPS_PID_AT, msg);
    if (rslt != VOS_OK) {
        AT_ERR_LOG("VRRC_SYNC_QryCfgPara: SEND MSG FAIL.");
        return VOS_ERR;
    }

    return VOS_OK;
}
#endif
#endif

