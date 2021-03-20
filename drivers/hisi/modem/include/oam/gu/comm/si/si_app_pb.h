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

/*
 * ��Ȩ���У�c����Ϊ�������޹�˾ 2012-2019
 * ��������: �绰���ĵ�ͷ�ļ�
 * ��������: 2006��11��14��
 */

#ifndef __SIAPPPB_H__
#define __SIAPPPB_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include "si_typedef.h"
#include "vos.h"
#include "usimm_ps_interface.h"
#include "nv_stru_pam.h"

#if (OSA_CPU_NRCPU != VOS_OSA_CPU)
#include "taf_oam_interface.h"
#endif

#if (OSA_CPU_CCPU == VOS_OSA_CPU)
#include "ccore_nv_stru_pam.h"
#endif

#pragma pack(push, 4)

#if (FEATURE_PHONE_USIM == FEATURE_ON)

#define SI_PB_PHONENUM_MAX_LEN 40  /* ���ؽṹ�к������󳤶ȣ���ָ�ֽ��� */
#define SI_PB_ALPHATAG_MAX_LEN 242 /* ���ؽṹ�����ֵ���󳤶ȣ���ָ�ֽ��� */

#define SI_PB_EMAIL_MAX_LEN 64 /* ���ؽṹ���ʼ�����󳤶ȣ���ָ�ֽ��� */

#define SI_PB_NUMBER_LEN 20 /* Э��ջ�·���BCD���볤�� */

/* ���ױ��Ƕ��Ƴ���5������ʱ�޸�Ϊ֧��16�� */
#define USIM_MAX_ECC_RECORDS 16
#define USIM_ECC_LEN 3

typedef SI_UINT32  SI_PB_AlphaTagTypeUint32;


/* ���ֵı������ͣ�16-bit universal multiple-octet coded character set (ISO/IEC10646 [32]) */
#define SI_PB_ALPHATAG_TYPE_UCS2_80     0x80
#define SI_PB_ALPHATAG_TYPE_UCS2_81 0x81
#define SI_PB_ALPHATAG_TYPE_UCS2_82 0x82
#define SI_PB_ALPHATAG_TYPE_GSM 0x00 /* ���ֵı������ͣ�GSM 7 bit default alphabet (3GPP TS 23.038) */
#define SI_PB_ALPHATAG_TYPE_UCS2 0x80

#define SI_PB_CONTENT_VALID 0x01
#define SI_PB_CONTENT_INVALID 0x00

#define SI_PB_LAST_TAG_TRUE 1

#define SI_PB_ALPHATAG_TRANSFER_TAG 0x1B

enum PB_FdnCheckRslt {
    PB_FDN_CHECK_SUCC = 0,
    PB_FDN_CHECK_NUM1_FAIL = 1,
    PB_FDN_CHECK_NUM2_FAIL = 2,
    PB_FDN_CHECK_BOTH_NUM_FAIL = 3,
    PB_FDN_CHECK_RSLT_BUTT
};
typedef VOS_UINT32 PB_FdnCheckRsltUint32;

/*
 * ����˵��: PBģ��������Ϣ����
 */
enum SI_PB_Req {
    SI_PB_READ_REQ = 0,       /* ���뱾��ȡ����           */
    SI_PB_SET_REQ = 1,        /* ���뱾��������           */
    SI_PB_MODIFY_REQ = 2,     /* ���뱾�޸�����           */
    SI_PB_DELETE_REQ = 3,     /* ���뱾ɾ������           */
    SI_PB_QUERY_REQ = 4,      /* ���뱾��ѯ����           */
    SI_PB_ADD_REQ = 5,        /* ���뱾�������           */
    SI_PB_SEARCH_REQ = 6,     /* ���뱾��������           */
    SI_PB_SREAD_REQ = 7,      /* ���Ϻ��뱾��ȡ����       */
    SI_PB_SMODIFY_REQ = 8,    /* ���Ϻ��뱾�޸�����       */
    SI_PB_SADD_REQ = 9,       /* ���Ϻ��뱾�������       */
    SI_PB_UPDATE_AGOBAL = 10, /* ����Acpu��ȫ�ֱ�������   */
    SI_PB_UPDATE_CURPB = 11,  /* ���µ�ǰ�绰������       */
    SI_PB_FDN_CHECK_REQ = 12, /* FDN����ƥ������          */
    SI_PB_ECALL_QRY_REQ = 13, /* ECALL����ƥ������        */

    SI_PB_REQ_BUTT
};
typedef SI_UINT32 SI_PB_ReqUint32;

/*
 * ����˵��: PBģ��ظ����ϱ���Ϣ����
 */
enum SI_PB_Cnf {
    SI_PB_EVENT_INFO_IND = 0,        /* ���뱾��Ϣ�¼��ϱ�           */
    SI_PB_EVENT_READ_CNF = 1,        /* ���뱾��ȡ����ظ�           */
    SI_PB_EVENT_QUERY_CNF = 2,       /* ���뱾��ѯ����ظ�           */
    SI_PB_EVENT_SET_CNF = 3,         /* ���뱾���ý���ظ�           */
    SI_PB_EVENT_ADD_CNF = 4,         /* ���뱾��ӽ���ظ�           */
    SI_PB_EVENT_MODIFY_CNF = 5,      /* ���뱾�޸Ľ���ظ�           */
    SI_PB_EVENT_DELETE_CNF = 6,      /* ���뱾ɾ������ظ�           */
    SI_PB_EVENT_SEARCH_CNF = 7,      /* ���뱾��������ظ�           */
    SI_PB_EVENT_SREAD_CNF = 8,       /* ���Ϻ��뱾��ȡ����ظ�       */
    SI_PB_EVENT_SADD_CNF = 9,        /* ���Ϻ��뱾��ӽ���ظ�       */
    SI_PB_EVENT_SMODIFY_CNF = 10,    /* ���Ϻ��뱾�޸Ľ���ظ�       */
    SI_PB_EVENT_FDNCHECK_CNF = 11,   /* FDN����ƥ�����ظ�          */
    SI_PB_EVENT_ECALLQUERY_CNF = 12, /* ECALL�����ѯ����ظ�        */
    SI_PB_EVENT_ECALLINIT_IND = 13,  /* ECALL�����ʼ������ϱ�      */
    SI_PB_EVENT_BUTT
};

typedef VOS_UINT32 SI_PB_CnfUint32;

/*
 * ����˵��: PBģ��洢������
 */
enum SI_PB_Storage {
    SI_PB_STORAGE_UNSPECIFIED = 0, /* �洢�����Ͳ�ָ������������ȷ���������� */
    SI_PB_STORAGE_SM = 1,          /* �洢������ΪADN */
    SI_PB_STORAGE_ME = 2,          /* �洢������ΪNV */
    SI_PB_STORAGE_BOTH = 3,        /* �洢������ΪADN��NV */
    SI_PB_STORAGE_ON = 4,          /* �洢������ΪMSISDN */
    SI_PB_STORAGE_FD = 5,          /* �洢������ΪFDN */
    SI_PB_STORAGE_EC = 6,          /* �洢������ΪECC */
    SI_PB_STORAGE_BD = 7,          /* �洢������ΪBDN */
    SI_PB_STORAGE_SD = 8,          /* �洢������ΪSDN */
    SI_PB_STORAGE_BUTT
};

typedef VOS_UINT32 SI_PB_StorageUint32;

/* �绰����������ö�� */
enum PB_NumberType {
    PB_NUMBER_TYPE_NORMAL = 129,        /* ��ͨ�������� */
    PB_NUMBER_TYPE_INTERNATIONAL = 145, /* ���ʺ������� */
    PB_NUMBER_TYPE_BUTT
};

/* �����ݿ���Ʒ֧�ֵ绰����ѯ���� */
typedef struct {
    SI_UINT16 inUsedNum;    /* ��ʹ�ü�¼��Ŀ */
    SI_UINT16 totalNum;     /* �ܼ�¼��Ŀ */
    SI_UINT16 textLen;      /* ֧�ֵ�text����󳤶� */
    SI_UINT16 numLen;       /* ֧�ֵ�number����󳤶� */
    SI_UINT16 emailTextLen; /* Emai���� */
    SI_UINT16 anrNumberLen; /* ANR���볤�� */
} SI_PB_EventQueryCnf;

typedef struct {
    SI_UINT16 adnRecordNum;    /* ADN��¼���� */
    SI_UINT16 adnTextLen;      /* ADN�����ֶγ��� */
    SI_UINT16 adnNumberLen;    /* ADN���볤�� */
    SI_UINT16 fdnRecordNum;    /* FDN��¼���� */
    SI_UINT16 fdnTextLen;      /* FDN�����ֶγ��� */
    SI_UINT16 fdnNumberLen;    /* FDN���볤�� */
    SI_UINT16 bdnRecordNum;    /* BDN��¼���� */
    SI_UINT16 bdnTextLen;      /* BDN�����ֶγ��� */
    SI_UINT16 bdnNumberLen;    /* BDN���볤�� */
    SI_UINT16 msisdnRecordNum; /* MSISDN��¼���� */
    SI_UINT16 msisdnTextLen;   /* MSISDN�����ֶγ��� */
    SI_UINT16 msisdnNumberLen; /* MSISDN���볤�� */
    SI_UINT16 emailTextLen;    /* EMAIL���� */
    SI_UINT16 anrNumberLen;    /* ANR���볤�� */
    SI_UINT16 fdnState;        /* FDNʹ��״̬ */
    SI_UINT16 bdnState;        /* BDNʹ��״̬ */
    SI_UINT16 cardType;        /* �����ͣ�0 SIM����1 USIM�� */
} SI_PB_EventInfoInd;

typedef struct {
    SI_UINT16 inUsedNum;    /* ��ʹ�ü�¼��Ŀ */
    SI_UINT16 totalNum;     /* �ܼ�¼��Ŀ */
    SI_UINT16 textLen;      /* ֧�ֵ�text����󳤶� */
    SI_UINT16 numLen;       /* ֧�ֵ�number����󳤶� */
    SI_UINT16 emailTextLen; /* Emai���� */
    SI_UINT16 anrNumberLen; /* ANR���볤�� */
} SI_PB_EventSetCnf;

typedef struct {
    SI_UINT8 numberType;                         /* �������ͣ�145��129 */
    SI_UINT8 numberLength;                       /* ���볤�ȣ��ǲ�����NumberType�ĳ��� */
    SI_UINT8 number[SI_PB_PHONENUM_MAX_LEN + 2]; /* ���룬����ASCII�ı��뷽ʽ */
} SI_PB_AdditionNum;

typedef struct {
    SI_UINT32 emailLen;                  /* Email���� */
    SI_UINT8 email[SI_PB_EMAIL_MAX_LEN]; /* Email������ASCII�ı��뷽ʽ */
} SI_PB_Email;

typedef struct {
    SI_UINT16 index;                           /* ��¼��index */
    SI_UINT8 alphaTag[SI_PB_ALPHATAG_MAX_LEN]; /* ��¼�������ֶΰ����������� */
    SI_UINT8 alphaTagLength;                   /* ���ֳ���, �ֽ����������UNICODE���������������ָʾ�ֽ� */
    SI_UINT8 numberType;                       /* �������ͣ�145��129 */
    SI_UINT8 numberLength;                     /* ���볤�ȣ��ǲ�����NumberType�ĳ��� */
    SI_UINT8 validFlag;                        /* ��¼��Ч��־ */
    SI_UINT8 number[SI_PB_PHONENUM_MAX_LEN];   /* ����1������ASCII�ı��뷽ʽ */
    SI_PB_AlphaTagTypeUint32 alphaTagType;     /* �������� */
    SI_PB_AdditionNum additionNumber[3];
    SI_PB_Email email;
} SI_PB_Record;

typedef struct {
    SI_UINT16 recordNum; /* ��¼����Ŀ */
    SI_UINT16 rsv;
    SI_PB_Record pbRecord; /* ��һ����¼�� */
} SI_PB_EventReadCnf;

typedef struct {
    SI_UINT16 recordNum; /* ��¼����Ŀ */
    SI_UINT16 rsv;
    SI_PB_Record pbRecord; /* ��һ����¼�� */
} SI_PB_EventSearchCnf;

typedef struct {
    SI_UINT16 index; /* ������ӵļ�¼������ */
} SI_PB_EventAddCnf;

typedef struct {
    SI_UINT16 clientId;
    SI_UINT8 opId;
    SI_UINT8 reserve;
    SI_UINT32 pbEventType;
    SI_UINT32 pbError;
    SI_PB_StorateTypeUint32 storage;
    SI_UINT32 pbLastTag;
    union {
        SI_PB_EventQueryCnf pbQueryCnf;
        SI_PB_EventInfoInd pbInfoInd;
        SI_PB_EventSetCnf pbSetCnf;
        SI_PB_EventReadCnf pbReadCnf;
        SI_PB_EventAddCnf pbAddCnf;
        SI_PB_EventSearchCnf pbSearchCnf;
    } pbEvent;
} SI_PB_EventInfo;

typedef struct {
    SI_BOOL isESCExist;             /* ESC�ֽ��Ƿ���ڱ�־ */
    SI_UINT8 eccCode[USIM_ECC_LEN]; /* Emergency Call Code */
    SI_UINT8 esc;                   /* ESC�ֶ�ֵ */
    SI_UINT8 len;                   /* ESC�ַ����� */
    SI_UINT8 alphaIdentifier[253];  /* ESC�ַ����� */
    SI_UINT8 reserved[2];
} SI_PB_EccRecord;

typedef struct {
    SI_BOOL eccExists;   /* ECC�ļ��Ƿ���� */
    SI_UINT32 reocrdNum; /* ECC�н������к���ĸ��� */
    SI_PB_EccRecord eccRecord[USIM_MAX_ECC_RECORDS];
} SI_PB_EccData;

typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32 msgName; /* ��Ϣ��   */
} PBMsgBlock;

/*
 * ����˵��: FDN����ṹ
 */
typedef struct {
    VOS_UINT32 num1Len;
    VOS_UINT8 num1[SI_PB_NUMBER_LEN];
    VOS_UINT32 num2Len;
    VOS_UINT8 num2[SI_PB_NUMBER_LEN];
} SI_PB_FdnNum;

/*
 * ����˵��: FDN����ƥ��������Ϣ�ṹ
 */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32 msgName; /* ��Ϣ�� */
    USIMM_CardAppUint32 appType;
    VOS_UINT32 sendPara;
    VOS_UINT32 contextIndex;
    SI_PB_FdnNum fdnNum; /* FDN����ṹ */
} SI_PB_FdnCheckReq;

/*
 * ����˵��: FDN���������Ϣ�ṹ
 */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32 msgName;           /* ��Ϣ�� */
    USIMM_CardAppUint32 appType;  /* APP���� */
    VOS_UINT32 sendPara;          /* ͸������ */
    VOS_UINT32 contextIndex;      /* indexֵ */
    PB_FdnCheckRsltUint32 result; /* ��֤��� */
} PB_FdnCheckCnf;

/*
 * ����˵��: Ecall��ȡFDN��SDN��¼������
 */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32 msgName;          /* ��Ϣ�� */
    USIMM_CardAppUint32 appType; /* APP���� */
    SI_PB_StorageUint32 storage; /* �洢�����ͣ�ֻ����SI_PB_STORAGE_FD��SI_PB_STORAGE_SD */
    VOS_UINT8 listLen;           /* ��ȡ��¼�����ĳ��� */
    VOS_UINT8 list[3];           /* ��ȡ��¼���������� */
} SI_PB_EcallQryReq;

/*
 * ����˵��: ecall test/recfg number��Ϣ�ṹ��
 * 1. ��    ��: 2014��04��25��
 */
typedef struct {
    VOS_UINT8 ton;    /* �������� */
    VOS_UINT8 length; /* BCD��ĳ��� */
    VOS_UINT8 index;  /* ��1��ʼ */
    VOS_UINT8 rsv[1];
    VOS_UINT8 callNumber[SI_PB_PHONENUM_MAX_LEN / 2];
} SI_PB_EcallNum;

/*
 * ����˵��: ��ѯECALL�ĺ���ظ�
 * 1. ��    ��: 2014��04��25��
 */

typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32 msgName;          /* ��Ϣ�� */
    VOS_UINT32 rslt;             /* VOS_OK: ��ȡ�ɹ�;VOS_ERR:��ȡʧ��(������������Ƿ����ȡʧ��) */
    USIMM_CardAppUint32 appType; /* APP���� */
    SI_PB_StorageUint32 storage; /* �绰������,��ʱ��ָ��SDN��FDN */
    VOS_UINT32 recordNum;        /* ��ȡ�绰����¼���ܼ�¼�� */
    SI_PB_EcallNum eCallNumber[2];
} SI_PB_EcallQryCnf;

/*
 * ����˵��: ECALL�����ʼ��ָʾ��Ϣ
 */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32 msgName;          /* ��Ϣ�� */
    USIMM_CardAppUint32 appType; /* APP���� */
    VOS_UINT16 fdnRecordNum;     /* FDN�ܼ�¼�� */
    VOS_UINT16 sdnRecordNum;     /* SDN�ܼ�¼�� */
} SI_PB_EcallInitInd;

extern SI_UINT32 SI_PB_Read(SI_UINT16 clientId, SI_UINT8 opId, SI_PB_StorateTypeUint32 storage, SI_UINT16 index1,
    SI_UINT16 index2);

extern SI_UINT32 SI_PB_Set(SI_UINT16 clientId, SI_UINT8 opId, SI_PB_StorateTypeUint32 storage);

extern SI_UINT32 SI_PB_Modify(SI_UINT16 clientId, SI_UINT8 opId, SI_PB_StorateTypeUint32 storage, SI_PB_Record *record);

extern SI_UINT32 SI_PB_SModify(SI_UINT16 clientId, SI_UINT8 opId, SI_PB_StorateTypeUint32 storage,
    SI_PB_Record *record);

extern SI_UINT32 SI_PB_SRead(SI_UINT16 clientId, SI_UINT8 opId, SI_PB_StorateTypeUint32 storage, SI_UINT16 index1,
    SI_UINT16 index2);

extern SI_UINT32 SI_PB_Delete(SI_UINT16 clientId, SI_UINT8 opId, SI_PB_StorateTypeUint32 storage, SI_UINT16 pbIndex);

extern SI_UINT32 SI_PB_Add(SI_UINT16 clientId, SI_UINT8 opId, SI_PB_StorateTypeUint32 storage, SI_PB_Record *record);

extern SI_UINT32 SI_PB_SAdd(SI_UINT16 clientId, SI_UINT8 opId, SI_PB_StorateTypeUint32 storage, SI_PB_Record *record);

extern SI_UINT32 SI_PB_Search(SI_UINT16 clientId, SI_UINT8 opId, SI_PB_StorateTypeUint32 storage, SI_UINT8 length,
    SI_UINT8 *content);

extern SI_UINT32 SI_PB_Query(SI_UINT16 clientId, SI_UINT8 opId);

#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)
extern VOS_UINT32 SI_PB_GetXeccNumber(SI_PIH_CardSlotUint32 slotId, SI_PB_EccData *eccData);
#endif

extern VOS_UINT32 WuepsPBPidInit(enum VOS_InitPhaseDefine initPhrase);

extern VOS_UINT32 I1_WuepsPBPidInit(enum VOS_InitPhaseDefine initPhrase);

extern VOS_UINT32 I2_WuepsPBPidInit(enum VOS_InitPhaseDefine initPhrase);

extern VOS_VOID SI_PB_InitContent(VOS_UINT8 len, VOS_UINT8 value, VOS_UINT8 *mem);

extern VOS_VOID I0_SI_PB_PidMsgProc(struct MsgCB *pbMsg);

extern VOS_VOID I1_SI_PB_PidMsgProc(struct MsgCB *pbMsg);

extern VOS_VOID I2_SI_PB_PidMsgProc(struct MsgCB *pbMsg);

extern VOS_UINT32 SI_PB_GetStorateType(VOS_VOID);

extern VOS_UINT32 SI_PB_GetSPBFlag(VOS_VOID);

#else /* (FEATURE_PHONE_USIM == FEATURE_ON) */

#define SI_PB_PHONENUM_MAX_LEN 40  /* ���ؽṹ�к������󳤶ȣ���ָ�ֽ��� */
#define SI_PB_ALPHATAG_MAX_LEN 242 /* ���ؽṹ�����ֵ���󳤶ȣ���ָ�ֽ��� */

#define SI_PB_EMAIL_MAX_LEN 64 /* ���ؽṹ���ʼ�����󳤶ȣ���ָ�ֽ��� */

#define SI_PB_NUMBER_LEN (20) /* Э��ջ�·���BCD���볤�� */

/* ���ױ��Ƕ��Ƴ���5������ʱ�޸�Ϊ֧��16�� */
#define USIM_MAX_ECC_RECORDS 16
#define USIM_ECC_LEN 3

typedef SI_UINT32  SI_PB_AlphaTagTypeUint32;
#define SI_PB_ALPHATAG_TYPE_UCS2_80     0x80            /* ���ֵı������ͣ�16-bit universal multiple-octet coded character set (ISO/IEC10646 [32]) */
#define SI_PB_ALPHATAG_TYPE_UCS2_81     0x81
#define SI_PB_ALPHATAG_TYPE_UCS2_82     0x82
#define SI_PB_ALPHATAG_TYPE_GSM         0x00            /* ���ֵı������ͣ�GSM 7 bit default alphabet (3GPP TS 23.038) */
#define SI_PB_ALPHATAG_TYPE_UCS2        0x80

#define SI_PB_CONTENT_VALID 0x01
#define SI_PB_CONTENT_INVALID 0x00

#define SI_PB_LAST_TAG_TRUE 1

#define SI_PB_ALPHATAG_TRANSFER_TAG 0x1B

enum PB_FdnCheckRslt {
    PB_FDN_CHECK_SUCC          = 0,
    PB_FDN_CHECK_NUM1_FAIL     = 1,
    PB_FDN_CHECK_NUM2_FAIL     = 2,
    PB_FDN_CHECK_BOTH_NUM_FAIL = 3,
    PB_FDN_CHECK_RSLT_BUTT
};
typedef VOS_UINT32 PB_FdnCheckRsltUint32;

/*
 * ����˵��: PBģ��������Ϣ����
 */
enum SI_PB_Req {
    SI_PB_READ_REQ      = 0,  /* ���뱾��ȡ����           */
    SI_PB_SET_REQ       = 1,  /* ���뱾��������           */
    SI_PB_MODIFY_REQ    = 2,  /* ���뱾�޸�����           */
    SI_PB_DELETE_REQ    = 3,  /* ���뱾ɾ������           */
    SI_PB_QUERY_REQ     = 4,  /* ���뱾��ѯ����           */
    SI_PB_ADD_REQ       = 5,  /* ���뱾�������           */
    SI_PB_SEARCH_REQ    = 6,  /* ���뱾��������           */
    SI_PB_SREAD_REQ     = 7,  /* ���Ϻ��뱾��ȡ����       */
    SI_PB_SMODIFY_REQ   = 8,  /* ���Ϻ��뱾�޸�����       */
    SI_PB_SADD_REQ      = 9,  /* ���Ϻ��뱾�������       */
    SI_PB_UPDATE_AGOBAL = 10, /* ����Acpu��ȫ�ֱ�������   */
    SI_PB_UPDATE_CURPB  = 11, /* ���µ�ǰ�绰������       */
    SI_PB_FDN_CHECK_REQ = 12, /* FDN����ƥ������          */
    SI_PB_ECALL_QRY_REQ = 13, /* ECALL����ƥ������        */

    SI_PB_REQ_BUTT
};
typedef SI_UINT32 SI_PB_ReqUint32;

/*
 * ����˵��: PBģ��ظ����ϱ���Ϣ����
 */
enum SI_PB_Cnf {
    SI_PB_EVENT_INFO_IND       = 0,  /* ���뱾��Ϣ�¼��ϱ�           */
    SI_PB_EVENT_READ_CNF       = 1,  /* ���뱾��ȡ����ظ�           */
    SI_PB_EVENT_QUERY_CNF      = 2,  /* ���뱾��ѯ����ظ�           */
    SI_PB_EVENT_SET_CNF        = 3,  /* ���뱾���ý���ظ�           */
    SI_PB_EVENT_ADD_CNF        = 4,  /* ���뱾��ӽ���ظ�           */
    SI_PB_EVENT_MODIFY_CNF     = 5,  /* ���뱾�޸Ľ���ظ�           */
    SI_PB_EVENT_DELETE_CNF     = 6,  /* ���뱾ɾ������ظ�           */
    SI_PB_EVENT_SEARCH_CNF     = 7,  /* ���뱾��������ظ�           */
    SI_PB_EVENT_SREAD_CNF      = 8,  /* ���Ϻ��뱾��ȡ����ظ�       */
    SI_PB_EVENT_SADD_CNF       = 9,  /* ���Ϻ��뱾��ӽ���ظ�       */
    SI_PB_EVENT_SMODIFY_CNF    = 10, /* ���Ϻ��뱾�޸Ľ���ظ�       */
    SI_PB_EVENT_FDNCHECK_CNF   = 11, /* FDN����ƥ�����ظ�          */
    SI_PB_EVENT_ECALLQUERY_CNF = 12, /* ECALL�����ѯ����ظ�        */
    SI_PB_EVENT_ECALLINIT_IND  = 13, /* ECALL�����ʼ������ϱ�      */
    SI_PB_EVENT_BUTT
};

typedef VOS_UINT32 SI_PB_CnfUint32;

/*
 * ����˵��: PBģ��洢������
 */
enum SI_PB_Storage {
    SI_PB_STORAGE_UNSPECIFIED = 0, /* �洢�����Ͳ�ָ������������ȷ���������� */
    SI_PB_STORAGE_SM          = 1, /* �洢������ΪADN */
    SI_PB_STORAGE_ME          = 2, /* �洢������ΪNV */
    SI_PB_STORAGE_BOTH        = 3, /* �洢������ΪADN��NV */
    SI_PB_STORAGE_ON          = 4, /* �洢������ΪMSISDN */
    SI_PB_STORAGE_FD          = 5, /* �洢������ΪFDN */
    SI_PB_STORAGE_EC          = 6, /* �洢������ΪECC */
    SI_PB_STORAGE_BD          = 7, /* �洢������ΪBDN */
    SI_PB_STORAGE_SD          = 8, /* �洢������ΪSDN */
    SI_PB_STORAGE_BUTT
};

typedef VOS_UINT32 SI_PB_StorageUint32;

/* �绰����������ö�� */
enum PB_NumberType {
    PB_NUMBER_TYPE_NORMAL        = 129, /* ��ͨ�������� */
    PB_NUMBER_TYPE_INTERNATIONAL = 145, /* ���ʺ������� */
    PB_NUMBER_TYPE_BUTT
};

/* �����ݿ���Ʒ֧�ֵ绰����ѯ���� */
typedef struct {
    SI_UINT16 inUsedNum;    /* ��ʹ�ü�¼��Ŀ */
    SI_UINT16 totalNum;     /* �ܼ�¼��Ŀ */
    SI_UINT16 textLen;      /* ֧�ֵ�text����󳤶� */
    SI_UINT16 numLen;       /* ֧�ֵ�number����󳤶� */
    SI_UINT16 emailTextLen; /* Emai���� */
    SI_UINT16 anrNumberLen; /* ANR���볤�� */
} SI_PB_EventQueryCnf;

typedef struct {
    SI_UINT16 adnRecordNum;    /* ADN��¼���� */
    SI_UINT16 adnTextLen;      /* ADN�����ֶγ��� */
    SI_UINT16 adnNumberLen;    /* ADN���볤�� */
    SI_UINT16 fdnRecordNum;    /* FDN��¼���� */
    SI_UINT16 fdnTextLen;      /* FDN�����ֶγ��� */
    SI_UINT16 fdnNumberLen;    /* FDN���볤�� */
    SI_UINT16 bdnRecordNum;    /* BDN��¼���� */
    SI_UINT16 bdnTextLen;      /* BDN�����ֶγ��� */
    SI_UINT16 bdnNumberLen;    /* BDN���볤�� */
    SI_UINT16 msisdnRecordNum; /* MSISDN��¼���� */
    SI_UINT16 msisdnTextLen;   /* MSISDN�����ֶγ��� */
    SI_UINT16 msisdnNumberLen; /* MSISDN���볤�� */
    SI_UINT16 emailTextLen;    /* EMAIL���� */
    SI_UINT16 anrNumberLen;    /* ANR���볤�� */
    SI_UINT16 fdnState;        /* FDNʹ��״̬ */
    SI_UINT16 bdnState;        /* BDNʹ��״̬ */
    SI_UINT16 cardType;        /* �����ͣ�0 SIM����1 USIM�� */
} SI_PB_EventInfoInd;

typedef struct {
    SI_UINT16 inUsedNum;    /* ��ʹ�ü�¼��Ŀ */
    SI_UINT16 totalNum;     /* �ܼ�¼��Ŀ */
    SI_UINT16 textLen;      /* ֧�ֵ�text����󳤶� */
    SI_UINT16 numLen;       /* ֧�ֵ�number����󳤶� */
    SI_UINT16 emailTextLen; /* Emai���� */
    SI_UINT16 anrNumberLen; /* ANR���볤�� */
} SI_PB_EventSetCnf;

typedef struct {
    SI_UINT8 numberType;                         /* �������ͣ�145��129 */
    SI_UINT8 numberLength;                       /* ���볤�ȣ��ǲ�����NumberType�ĳ��� */
    SI_UINT8 number[SI_PB_PHONENUM_MAX_LEN + 2]; /* ���룬����ASCII�ı��뷽ʽ */
} SI_PB_AdditionNum;

typedef struct {
    SI_UINT32 emailLen;                   /* Email���� */
    SI_UINT8  email[SI_PB_EMAIL_MAX_LEN]; /* Email������ASCII�ı��뷽ʽ */
} SI_PB_Email;

typedef struct {
    SI_UINT16           index;                            /* ��¼��index */
    SI_UINT8            alphaTag[SI_PB_ALPHATAG_MAX_LEN]; /* ��¼�������ֶΰ����������� */
    SI_UINT8            alphaTagLength;                   /* ���ֳ���, �ֽ����������UNICODE���������������ָʾ�ֽ� */
    SI_UINT8            numberType;                       /* �������ͣ�145��129 */
    SI_UINT8            numberLength;                     /* ���볤�ȣ��ǲ�����NumberType�ĳ��� */
    SI_UINT8            validFlag;                        /* ��¼��Ч��־ */
    SI_UINT8            number[SI_PB_PHONENUM_MAX_LEN];   /* ����1������ASCII�ı��뷽ʽ */
    SI_PB_AlphaTagTypeUint32 alphaTagType;                     /* �������� */
    SI_PB_AdditionNum   additionNumber[3];
    SI_PB_Email         email;
} SI_PB_Record;

typedef struct {
    SI_UINT16    recordNum; /* ��¼����Ŀ */
    SI_UINT16    rsv;
    SI_PB_Record pbRecord; /* ��һ����¼�� */
} SI_PB_EventReadCnf;

typedef struct {
    SI_UINT16    recordNum; /* ��¼����Ŀ */
    SI_UINT16    rsv;
    SI_PB_Record pbRecord; /* ��һ����¼�� */
} SI_PB_EventSearchCnf;

typedef struct {
    SI_UINT16 index; /* ������ӵļ�¼������ */
} SI_PB_EventAddCnf;

typedef struct {
    SI_UINT16          clientId;
    SI_UINT8           opId;
    SI_UINT8           reserve;
    SI_UINT32          pbEventType;
    SI_UINT32          pbError;
    SI_PB_StorateTypeUint32 storage;
    SI_UINT32          pbLastTag;
    union {
        SI_PB_EventQueryCnf  pbQueryCnf;
        SI_PB_EventInfoInd   pbInfoInd;
        SI_PB_EventSetCnf    pbSetCnf;
        SI_PB_EventReadCnf   pbReadCnf;
        SI_PB_EventAddCnf    pbAddCnf;
        SI_PB_EventSearchCnf pbSearchCnf;
    } pbEvent;
} SI_PB_EventInfo;

typedef struct {
    SI_BOOL  bESC;                  /* ESC�ֽ��Ƿ���ڱ�־ */
    SI_UINT8 eccCode[USIM_ECC_LEN]; /* Emergency Call Code */
    SI_UINT8 esc;                   /* ESC�ֶ�ֵ */
    SI_UINT8 len;                   /* ESC�ַ����� */
    SI_UINT8 alphaIdentifier[253];  /* ESC�ַ����� */
    SI_UINT8 reserved[2];
} SI_PB_EccRecord;

typedef struct {
    SI_BOOL         eccExists; /* ECC�ļ��Ƿ���� */
    SI_UINT32       reocrdNum;  /* ECC�н������к���ĸ��� */
    SI_PB_EccRecord eccRecord[USIM_MAX_ECC_RECORDS];
} SI_PB_EccData;

typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32 msgName; /* ��Ϣ��   */
} PBMsgBlock;

/*
 * ����˵��: FDN����ṹ
 */
typedef struct {
    VOS_UINT32 num1Len;
    VOS_UINT8  num1[SI_PB_NUMBER_LEN];
    VOS_UINT32 num2Len;
    VOS_UINT8  num2[SI_PB_NUMBER_LEN];
} SI_PB_FdnNum;

/*
 * ����˵��: FDN����ƥ��������Ϣ�ṹ
 */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32          msgName; /* ��Ϣ�� */
    USIMM_CardAppUint32 appType;
    VOS_UINT32          sendPara;
    VOS_UINT32          contextIndex;
    SI_PB_FdnNum        fdnNum; /* FDN����ṹ */
} SI_PB_FdnCheckReq;

/*
 * ����˵��: FDN���������Ϣ�ṹ
 */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32            msgName;      /* ��Ϣ�� */
    USIMM_CardAppUint32   appType;      /* APP���� */
    VOS_UINT32            sendPara;     /* ͸������ */
    VOS_UINT32            contextIndex; /* indexֵ */
    PB_FdnCheckRsltUint32 result;       /* ��֤��� */
} PB_FdnCheckCnf;

/*
 * ����˵��: Ecall��ȡFDN��SDN��¼������
 */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32          msgName; /* ��Ϣ�� */
    USIMM_CardAppUint32 appType; /* APP���� */
    SI_PB_StorageUint32 storage; /* �洢�����ͣ�ֻ����SI_PB_STORAGE_FD��SI_PB_STORAGE_SD */
    VOS_UINT8           listLen; /* ��ȡ��¼�����ĳ��� */
    VOS_UINT8           list[3]; /* ��ȡ��¼���������� */
} SI_PB_EcallQryReq;

/*
 * ����˵��: ecall test/recfg number��Ϣ�ṹ��
 * 1. ��    ��: 2014��04��25��
 */
typedef struct {
    VOS_UINT8 ton;    /* �������� */
    VOS_UINT8 length; /* BCD��ĳ��� */
    VOS_UINT8 index;  /* ��1��ʼ */
    VOS_UINT8 rsv[1];
    VOS_UINT8 callNumber[SI_PB_PHONENUM_MAX_LEN / 2];
} SI_PB_EcallNum;

/*
 * ����˵��: ��ѯECALL�ĺ���ظ�
 * 1. ��    ��: 2014��04��25��
 */

typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32          msgName;   /* ��Ϣ�� */
    VOS_UINT32          rslt;      /* VOS_OK: ��ȡ�ɹ�;VOS_ERR:��ȡʧ��(������������Ƿ����ȡʧ��) */
    USIMM_CardAppUint32 appType;   /* APP���� */
    SI_PB_StorageUint32 storage;   /* �绰������,��ʱ��ָ��SDN��FDN */
    VOS_UINT32          recordNum; /* ��ȡ�绰����¼���ܼ�¼�� */
    SI_PB_EcallNum      eCallNumber[2];
} SI_PB_EcallQryCnf;

/*
 * ����˵��: ECALL�����ʼ��ָʾ��Ϣ
 */
typedef struct {
    VOS_MSG_HEADER
    VOS_UINT32          msgName;      /* ��Ϣ�� */
    USIMM_CardAppUint32 appType;      /* APP���� */
    VOS_UINT16          fdnRecordNum; /* FDN�ܼ�¼�� */
    VOS_UINT16          sdnRecordNum; /* SDN�ܼ�¼�� */
} SI_PB_EcallInitInd;

extern SI_UINT32 SI_PB_Read(SI_UINT16 clientId, SI_UINT8 opId, SI_PB_StorateTypeUint32 storage, SI_UINT16 index1,
                            SI_UINT16 index2);

extern SI_UINT32 SI_PB_Set(SI_UINT16 clientId, SI_UINT8 opId, SI_PB_StorateTypeUint32 storage);

extern SI_UINT32 SI_PB_Modify(SI_UINT16 clientId, SI_UINT8 opId, SI_PB_StorateTypeUint32 storage, SI_PB_Record *record);

extern SI_UINT32 SI_PB_SModify(SI_UINT16 clientId, SI_UINT8 opId, SI_PB_StorateTypeUint32 storage, SI_PB_Record *record);

extern SI_UINT32 SI_PB_SRead(SI_UINT16 clientId, SI_UINT8 opId, SI_PB_StorateTypeUint32 storage, SI_UINT16 index1,
                             SI_UINT16 index2);

extern SI_UINT32 SI_PB_Delete(SI_UINT16 clientId, SI_UINT8 opId, SI_PB_StorateTypeUint32 storage, SI_UINT16 pbIndex);

extern SI_UINT32 SI_PB_Add(SI_UINT16 clientId, SI_UINT8 opId, SI_PB_StorateTypeUint32 storage, SI_PB_Record *record);

extern SI_UINT32 SI_PB_SAdd(SI_UINT16 clientId, SI_UINT8 opId, SI_PB_StorateTypeUint32 storage, SI_PB_Record *record);

extern SI_UINT32 SI_PB_Search(SI_UINT16 clientId, SI_UINT8 opId, SI_PB_StorateTypeUint32 storage, SI_UINT8 length,
                              SI_UINT8 *content);

extern SI_UINT32  SI_PB_Query(SI_UINT16 clientId, SI_UINT8 opId);

#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)
extern VOS_UINT32 SI_PB_GetXeccNumber(SI_PB_EccData *eccData);
#endif

extern VOS_UINT32 WuepsPBPidInit(enum VOS_InitPhaseDefine initPhrase);

extern VOS_VOID SI_PB_InitContent(VOS_UINT8 len, VOS_UINT8 value, VOS_UINT8 *mem);

extern VOS_VOID I0_SI_PB_PidMsgProc(struct MsgCB *pbMsg);

extern VOS_UINT32 SI_PB_GetStorateType(VOS_VOID);

extern VOS_UINT32 SI_PB_GetSPBFlag(VOS_VOID);

#endif /* (FEATURE_PHONE_USIM == FEATURE_ON) */

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
