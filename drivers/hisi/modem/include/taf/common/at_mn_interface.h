/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
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

#ifndef AT_MN_INTERFACE_H
#define AT_MN_INTERFACE_H

#include "product_config.h"
#include "vos.h"
#include "PsTypeDef.h"
#include "ps_common_def.h"
#include "v_msg.h"
#include "taf_type_def.h"
#include "taf_api.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)

#define MN_APP_REQ_DEFAULT_CONTENT_LEN 4

#define AT_MN_MSG_HEADER                \
    VOS_MSG_HEADER                      \
    AT_MN_MsgtypeUint16 enMsgId;        \
    VOS_UINT8           aucReserved[2]; \
    AT_APPCTRL          stAppCtrl;


enum AT_MN_Msgtype {
    /* ��Ϣ���� */ /* ��ϢID */ /* ��ע */
    /* AT��CALLģ�����Ϣ */
    MN_APP_MSG_CLASS_CALL = TAF_CALL_APP_MSG_BASE,

    /* ��Ϣ�Ѿ�����CCM���ͣ��˴��Ѿ�����Ҫ */
    /* ƽ�Ƶ�CCM��ʹ������ϢID */

    /* AT��MSGģ�����Ϣ */
    MN_APP_MSG_CLASS_MSG = TAF_MSG_APP_MSG_BASE,
    /* _H2ASN_MsgChoice MN_MSG_SendParm */
    MN_MSG_MSGTYPE_SEND_RPDATA_DIRECT = 0x1001,
    /* _H2ASN_MsgChoice MN_MSG_SendFrommemParm */
    MN_MSG_MSGTYPE_SEND_RPDATA_FROMMEM = 0x1002,
    /* _H2ASN_MsgChoice MN_MSG_SendAckParm */
    MN_MSG_MSGTYPE_SEND_RPRPT = 0x1003,
    /* _H2ASN_MsgChoice MN_MSG_WriteParm */
    MN_MSG_MSGTYPE_WRITE = 0x1004,
    /* _H2ASN_MsgChoice MN_MSG_ReadParm */
    MN_MSG_MSGTYPE_READ = 0x1005,
    /* _H2ASN_MsgChoice MN_MSG_ListParm */
    MN_MSG_MSGTYPE_LIST = 0x1006,
    /* _H2ASN_MsgChoice MN_MSG_DeleteParam */
    MN_MSG_MSGTYPE_DELETE = 0x1007,
    /* _H2ASN_MsgChoice MN_MSG_MEM_STORE_ENUM_U8 */
    MN_MSG_MSGTYPE_DELETE_TEST = 0x1008,
    /* _H2ASN_MsgChoice MN_MSG_WriteSrvParam */
    MN_MSG_MSGTYPE_WRITE_SRV_PARA = 0x1009,
    /* _H2ASN_MsgChoice MN_MSG_ReadCommParam */
    MN_MSG_MSGTYPE_READ_SRV_PARA = 0x100a,
    /* _H2ASN_MsgChoice MN_MSG_DeleteParam */
    MN_MSG_MSGTYPE_DELETE_SRV_PARA = 0x100b,
    /* _H2ASN_MsgChoice MN_MSG_ModifyStatusParm */
    MN_MSG_MSGTYPE_MODIFY_STATUS = 0x100c,
    /* _H2ASN_MsgChoice MN_MSG_SetRcvmsgPathParm */
    MN_MSG_MSGTYPE_SET_RCVPATH = 0x100d,
    /* _H2ASN_MsgChoice MN_MSG_GetRcvmsgPathParm */
    MN_MSG_MSGTYPE_GET_RCVPATH = 0x100e,
    /* _H2ASN_MsgChoice MN_MSG_GetStorageStatusParm */
    MN_MSG_MSGTYPE_GET_STORAGESTATUS = 0x100f,
    /* _H2ASN_MsgChoice MN_MSG_ReadCommParam */
    MN_MSG_MSGTYPE_READ_STARPT = 0x1010,
    /* _H2ASN_MsgChoice MN_MSG_DeleteParam */
    MN_MSG_MSGTYPE_DELETE_STARPT = 0x1011,
    /* _H2ASN_MsgChoice MN_MSG_SendParm */
    MN_MSG_MSGTYPE_GET_STARPT_FROMMOMSG = 0x1012,
    /* _H2ASN_MsgChoice MN_MSG_SendParm */
    MN_MSG_MSGTYPE_GET_MOMSG_FROMSTARPT = 0x1013,
    /* _H2ASN_MsgChoice MN_MSG_SetMemstatusParm */
    MN_MSG_MSGTYPE_SET_MEMSTATUS = 0x1014,
    /* _H2ASN_MsgChoice MN_MSG_SetLinkCtrl */
    MN_MSG_MSGTYPE_SET_RELAY_LINK_CTRL = 0x1015,
    MN_MSG_MSGTYPE_GET_RELAY_LINK_CTRL = 0x1016, /* _H2ASN_MsgChoice  */
    /* _H2ASN_MsgChoice MN_MSG_StubMsg */
    MN_MSG_MSGTYPE_STUB = 0x1017,
#if ((FEATURE_ON == FEATURE_GCBS) || (FEATURE_ON == FEATURE_WCBS))
    MN_MSG_MSGTYPE_GET_ALLCBMIDS = 0x1018, /* _H2ASN_MsgChoice  */
    /* _H2ASN_MsgChoice MN_MSG_CBMIR_LIST_STRU */
    MN_MSG_MSGTYPE_ADD_CBMIDS = 0x1019,
    /* _H2ASN_MsgChoice MN_MSG_CBMIR_LIST_STRU */
    MN_MSG_MSGTYPE_DELETE_CBMIDS = 0x101a,
    MN_MSG_MSGTYPE_EMPTY_CBMIDS  = 0x101b, /* _H2ASN_MsgChoice  */
#endif
    /* _H2ASN_MsgChoice MN_MSG_SetSmsSendDomain */
    MN_MSG_MSGTYPE_SET_SMS_SEND_DOMAIN = 0x101c,
    MN_MSG_MSGTYPE_GET_SMS_SEND_DOMAIN = 0x101d, /* _H2ASN_MsgChoice  */
#if (FEATURE_MBB_CUST == FEATURE_ON)
    MN_MSG_MSGTYPE_ABORT_RPDATA_DIRECT = 0x1101,
#endif
    MN_MSG_MSGTYPE_MAX,

    /* AT��SSAģ�����Ϣ */
    MN_APP_MSG_CLASS_SSA = TAF_SSA_APP_MSG_BASE,
    /* ����ҵ�������ʱ����Ϣ���� */
    TAF_MSG_REGISTERSS_MSG,
    TAF_MSG_ERASESS_MSG,
    TAF_MSG_ACTIVATESS_MSG,
    TAF_MSG_DEACTIVATESS_MSG,
    TAF_MSG_INTERROGATESS_MSG,
    TAF_MSG_REGPWD_MSG,
    TAF_MSG_PROCESS_USS_MSG,
    TAF_MSG_ERASECCENTRY_MSG,
    TAF_MSG_RLEASE_MSG,
    TAF_MSG_SET_USSDMODE_MSG,
#if (FEATURE_MBB_CUST == FEATURE_ON)
    TAF_MSG_ABORT_SS_MSG,
#endif
    /* AT��MMAģ�����Ϣ */
    MN_APP_MSG_CLASS_MMA = TAF_MMA_APP_MSG_BASE,

    /* AT��MNģ������ò��������Ϣ��δ�Ż����֣����Ż���ɾ�� */
    MN_APP_MSG_CLASS_SET = TAF_APP_SET_MSG_BASE,

    /* AT��MNģ����ѯ���������Ϣ��δ�Ż����֣����Ż���ɾ�� */
    MN_APP_MSG_CLASS_QRY = TAF_APP_QRY_MSG_BASE,

    /* AT��PS����ģ�����Ϣ */
    MN_APP_MSG_CLASS_PS = TAF_APS_MSG_BASE,

    /* �ⲿ����Ϣ�Ķ���Ǩ�Ƶ�TafAppAps.h�ļ��� */

    MN_APP_MSG_CLASS_MASK = TAF_IMSA_MSG_MASK
};
typedef VOS_UINT16 AT_MN_MsgtypeUint16;

typedef AT_MN_MsgtypeUint16 MN_MSG_MsgtypeUint16;

typedef VOS_UINT32 MN_CALL_AppReqUint32;

/*
 * ö��˵��: AT��CCģ����ԭ��
 */
enum AT_CC_Msgtype {
    /* ��Ϣ���� */ /* ��ϢID */ /* ��ע */
    /* AT����CC����Ϣ */
    AT_CC_MSG_STATE_QRY_REQ = 0x0000,

    /* CC����AT����Ϣ */
    AT_CC_MSG_STATE_QRY_CNF = 0x0001,

    AT_CC_MSG_BUTT

};
typedef VOS_UINT16 AT_CC_MsgtypeUint16;

#define ECALL_MAX_DIAL_NUM_LEN (18)

/*
 * �ṹ˵��: AT APP ���ƽṹ
 */
typedef struct {
    VOS_UINT16 clientId;
    VOS_UINT8  opId;
    VOS_UINT8  reserved[1];
} AT_APPCTRL;

/* MN����AT���¼��ṹ */
typedef struct {
    VOS_MSG_HEADER          /* _H2ASN_Skip */
    VOS_UINT16                              msgName; /* _H2ASN_Skip */ /* ��ϢID */
    VOS_UINT8                               reserved1[2];
    MN_CLIENT_ID_T                          clientId;
    VOS_UINT16                              len;
    VOS_UINT8 TAF_COMM_ATTRIBUTE_ALIGNED4   content[4]; /* �޸������Сʱ��ͬ���޸�TAF_DEFAULT_CONTENT_LEN�� */
} MN_AT_IndEvt;

typedef struct {
    MN_CLIENT_ID_T      clientId;
    VOS_UINT8           opId;
    TAF_PARA_SET_RESULT rslt;
    TAF_PARA_TYPE       paraType;
    VOS_UINT8           rsv[3];
} TAF_SetRslt;

typedef struct {
    MN_CLIENT_ID_T      clientId;
    VOS_UINT8           opId;
    TAF_PARA_TYPE       queryType;
    VOS_UINT16          errorCode;
    VOS_UINT16          paraLen;
    VOS_UINT8           para[0];    //lint !e43
} TAF_QryRslt;

typedef struct {
    MN_CLIENT_ID_T clientId;
    VOS_UINT8      domain;
    VOS_UINT8      rabId;
    VOS_UINT8      status;
    VOS_UINT8      cause;
    VOS_UINT8      rsv[2];
} TAF_DataStatusInd;

/*
 * �ṹ˵��: ��ά�ɲ�LOG��Ϣ�ṹ
 */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32 msgName;
    ModemIdUint16 modemId;
    VOS_UINT8     reserve[2];
    VOS_UINT32    len;
    VOS_CHAR log[4]; /* ��ӡ�ַ��� */
} TAF_MntnLogPrint;

/*
 * H2ASN������Ϣ�ṹ����
 */
typedef struct {
    /* _H2ASN_MsgChoice_Export AT_MN_MSGTYPE_ENUM_U32 */
    AT_MN_MsgtypeUint16 msgID;
    /* Ϊ�˼���NAS����Ϣͷ���壬����ת��ASN.1ʹ��AT_MN_MSGTYPE_ENUM_U32 */

    VOS_UINT8 msgBlock[2];
    /*
     * _H2ASN_MsgChoice_When_Comment          AT_MN_MSGTYPE_ENUM_U32
     */
} AT_MN_MsgData;
/* _H2ASN_Length UINT32 */

typedef struct {
    VOS_MSG_HEADER
    AT_MN_MsgData msgData;
} AtMnInterface_Msg;

/*
 * ö����: MN_APP_ReqMsg
 * �ṹ˵��: ����APP��������Ϣ�ṹ
 */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT16                            msgName;     /* ��Ϣ�� */
    VOS_UINT8                             reserve1[2]; /* ���� */
    MN_CLIENT_ID_T                        clientId;    /* Client ID */
    MN_OPERATION_ID_T                     opId;        /* Operation ID */
    VOS_UINT8                             reserve2[1]; /* ���� */
    VOS_UINT8 TAF_COMM_ATTRIBUTE_ALIGNED4 content[4];  /* �޸������Сʱ��ͬ��֪���޸�TAF_DEFAULT_CONTENT_LEN�� */
} MN_APP_ReqMsg;

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of at_mn_interface.h */
