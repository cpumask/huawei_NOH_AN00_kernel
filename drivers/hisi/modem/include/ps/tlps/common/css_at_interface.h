/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
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
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, 
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

#ifndef CSS_AT_INTERFACE_H
#define CSS_AT_INTERFACE_H

#include "vos.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(4)

#define MCC_INFO_VERSION_LEN (9)
#define AT_CSS_MAX_MCC_ID_NUM (17)
#define AT_CSS_BLOCK_CELL_LIST_VERSION_LEN (9)
/* add for CloudLine Feature 2017-6-28 begin */
#define AT_CSS_CLOUD_LINE_VERSION_LEN (9) /* �ƶ�Ԥ�ø�����·�汾���ַ������� */
#define AT_CSS_PLMN_MAX_LINE_NUM (64)     /* �ƶ�Ԥ�ø�����·һ��PLMN�°����������·���� */
#define AT_CSS_TACLAC_MAX_LINE_NUM 6   /* �ƶ�Ԥ�ø�����·һ��TAC/LAC����Ӧ�������·���� */
/* add for CloudLine Feature 2017-6-28 end */

/* Added for EMC, 2018-1-9, begin */
#define AT_CSS_MRU_MAX_NUM (10) /* MRU���Դ洢�������� */
/* Added for EMC, 2018-1-9, end */
/* plmn deployment, add begin, 2019-11-06 */
#define AT_CSS_PLMN_DEPLOYMENT_VERSION_LEN 9 /* ��Ԥ�ò������԰汾���ַ������� */
#define AT_CSS_MSG_DATALEN_MAX_SIZE 1500 /* AT->CSS��ϢЯ����dataLen���ֵ */
/* plmn deployment, add end, 2019-11-25 */
#define AT_CSS_MSG_DATALEN_REPORT_MAX_SIZE 500 /* CSS->AT��ϢЯ����dataLen���ֵ */
#define AT_CSS_CLOUD_LINE_INVALID_LINE_INDEX 0xFFFF /* AP����������lineIdΪ65535��ʾ��plmn��û�и�����·��Ϣ */
#define AT_CSS_CLOUD_LINE_NSA_CELL_MAX_SCG_NUM 3 /* ����Ԥ��·����Ϣ����NSAС�����SCG���� */
#define AT_CSS_CLOUD_LINE_MAX_LINE_NUM 6 /* CSS���洢6����·����ϸ��Ϣ */
#define AT_CSS_CLOUD_LINE_MAX_NODE_NUM 130 /* ÿ����·�����130���ڵ� */
#define AT_CSS_CLOUD_LINE_MAX_CELL_NUM 7 /* ÿ���ڵ������7��С��������LTE/SA/NSA�������͵�С�� */
#define AT_CSS_CLOUD_LINE_MAX_NSA_CELL_NUM 2 /* ÿ���ڵ��7��С�������2��NSAС�� */


enum CSS_AT_MsgType {
    /* AT->CSS */
    ID_AT_CSS_MCC_INFO_SET_REQ          = 0x0001, /* _H2ASN_MsgChoice AT_CSS_MccInfoSetReq */
    ID_AT_CSS_MCC_VERSION_INFO_REQ      = 0x0002, /* _H2ASN_MsgChoice AT_CSS_MccVersionInfoReq */
    ID_AT_CSS_BLOCK_CELL_LIST_SET_REQ   = 0x0003, /* _H2ASN_MsgChoice AT_CSS_BlockCellListSetReq */
    ID_AT_CSS_BLOCK_CELL_LIST_QUERY_REQ = 0x0004, /* _H2ASN_MsgChoice AT_CSS_BlockCellListQueryReq */
    /* Added for EMC, 2018-1-9, begin */
    /* Added for EMC, 2018-1-9, end */
    /* plmn deployment, add begin, 2019-11-06 */
    ID_AT_CSS_PLMN_DEPLOYMENT_INFO_SET_REQ = 0x000b, /* _H2ASN_MsgChoice AT_CSS_PlmnDeploymentInfoSetReq */
    /* plmn deployment, add end, 2019-11-25 */
    ID_AT_CSS_CLOUD_DATA_SET_REQ = 0x000c, /* _H2ASN_MsgChoice AT_CSS_CloudDataSetReq */
    ID_AT_CSS_SCREEN_STATUS_IND  = 0x000d, /* _H2ASN_MsgChoice AT_CSS_ScreenStatusInd */

    /* CSS->AT */
    ID_CSS_AT_MCC_INFO_SET_CNF          = 0x1001, /* _H2ASN_MsgChoice CSS_AT_MccInfoSetCnf */
    ID_CSS_AT_MCC_VERSION_INFO_CNF      = 0x1002, /* _H2ASN_MsgChoice CSS_AT_MccVersionInfoCnf */
    ID_CSS_AT_QUERY_MCC_INFO_NOTIFY     = 0x1003, /* _H2ASN_MsgChoice CSS_AT_QueryMccInfoNotify */
    ID_CSS_AT_BLOCK_CELL_LIST_SET_CNF   = 0x1004, /* _H2ASN_MsgChoice CSS_AT_BlockCellListSetCnf */
    ID_CSS_AT_BLOCK_CELL_LIST_QUERY_CNF = 0x1005, /* _H2ASN_MsgChoice CSS_AT_BlockCellListQueryCnf */
    ID_CSS_AT_BLOCK_CELL_MCC_NOTIFY     = 0x1006, /* _H2ASN_MsgChoice CSS_AT_BlockCellMccNotify */
    /* plmn deployment, add begin, 2019-11-06 */
    ID_CSS_AT_PLMN_DEPLOYMENT_INFO_SET_CNF = 0x100f, /* _H2ASN_MsgChoice CSS_AT_PlmnDeploymentInfoSetCnf */
    /* plmn deployment, add end, 2019-11-06 */
    ID_CSS_AT_CLOUD_DATA_SET_CNF = 0x1010, /* _H2ASN_MsgChoice CSS_AT_CloudDataSetCnf */
    ID_CSS_AT_CLOUD_DATA_REPORT  = 0x1011, /* _H2ASN_MsgChoice CSS_AT_CloudDataReport */

    ID_CSS_AT_MSG_BUTT
};
typedef VOS_UINT32 CSS_AT_MsgTypeUint32;

enum AT_CSS_Rat {
    AT_CSS_RAT_TYPE_GSM = 0, /* GSM���뼼�� */
    AT_CSS_RAT_TYPE_WCDMA,   /* WCDMA���뼼�� */
    AT_CSS_RAT_TYPE_LTE,     /* LTE���뼼�� */

    AT_CSS_RAT_TYPE_BUTT
};
typedef VOS_UINT8 AT_CSS_RatUint8;

enum AT_CSS_SetMccOperateType {
    AT_CSS_SET_MCC_TYPE_ADD_MCC = 0,    /* ����MCC */
    AT_CSS_SET_MCC_TYPE_DELETE_ALL_MCC, /* ɾ������MCC��Ϣ */
    AT_CSS_SET_MCC_TYPE_DELETE_ONE_MCC, /* ɾ��һ��MCC��Ϣ */

    AT_CSS_SET_MCC_TYPE_BUTT
};
typedef VOS_UINT8 AT_CSS_SetMccOperateTypeUint8;


enum AT_CSS_BlockCellListOperateType {
    AT_CSS_BLOCK_CELL_LIST_ADD_ONE = 0, /* ����һ��BLOCK CELL��Ϣ */
    AT_CSS_BLOCK_CELL_LIST_DELETE_ALL,  /* ɾ������BLOCK CELL��Ϣ */

    AT_CSS_BLOCK_CELL_LIST_TYPE_BUTT
};
typedef VOS_UINT8 AT_CSS_BlockCellListOperateTypeUint8;

/* add for CloudLine Feature 2017-6-29 begin */
/* �ṹ˵��: ������������ϸ��������� */
enum AT_CSS_CloudLineOperationType {
    AT_CSS_CLOUD_LINE_OPERATION_TYPE_ADD = 0,    /* ���� */
    AT_CSS_CLOUD_LINE_OPERATION_TYPE_DELETE_ALL, /* ɾ�� */

    AT_CSS_CLOUD_LINE_OPERATION_TYPE_BUTT
};
typedef VOS_UINT8 AT_CSS_CloudLineOperationTypeUint8;

/* �ṹ˵��: ����������ϸ��Ϣ������ */
enum AT_CSS_LineDetailInfoType {
    AT_CSS_LINE_DETAIL_INFO_TACLAC_PAIR = 0, /* TACLAC�� */
    AT_CSS_LINE_DETAIL_INFO_GSM_HO_PATH,     /* GSM HO��CSFBԤ��·�� */
    AT_CSS_LINE_DETAIL_INFO_GSM_HO_BAR,      /* GSM HO BAR·�� */
    /* add for Cloudline Lte Feature 2018-4-16 begin */
    AT_CSS_LINE_DETAIL_INFO_LTE_SA_NSA_HO_PATH, /* LTE/SA/NSA����Ԥ��·�� */
    /* add for Cloudline Lte Feature 2018-4-16 end */
    AT_CSS_LINE_DETAIL_INFO_NO_INFO = 0xFF /* ��ʾû����Ϣ��Ϣ��ֹͣ���� */
};
typedef VOS_UINT8 AT_CSS_LineDetailInfoTypeUint8;
/* add for CloudLine Feature 2017-6-29 end */

/* �ṹ˵��: AT���õ��ƶ������������� */
enum AT_CSS_CloudDataType {
    AT_CSS_CLOUD_DATA_TYPE_CLOUD_LINE = 0, /* ����Ԥ��·�� */
    AT_CSS_CLOUD_DATA_TYPE_BUTT
};
typedef VOS_UINT32 AT_CSS_CloudDataTypeUint32;

/* �ṹ˵��: ��ͨ���������� */
enum AT_CSS_CloudLineDataType {
    AT_CSS_CLOUD_LINE_DATA_TYPE_LINE_INDEX  = 0, /* AP�·�����Ԥ��·�������� */
    AT_CSS_CLOUD_LINE_DATA_TYPE_LINE_DETAIL = 1, /* AP�·�����Ԥ��·����ϸ�� */
    AT_CSS_CLOUD_LINE_DATA_TYPE_REPORT_PLMN = 2, /* MODEM�ϱ���ǰPLMN */
    AT_CSS_CLOUD_LINE_DATA_TYPE_REPORT_LINE = 3, /* MODEM�ϱ���ǰ��·��Ϣ */
    AT_CSS_CLOUD_LINE_DATA_TYPE_REPORT_NODE = 4, /* MODEM�ϱ���ǰNODE�ڵ���Ϣ */

    AT_CSS_CLOUD_LINE_DATA_TYPE_BUTT
};
typedef VOS_UINT32 AT_CSS_CloudLineDataTypeUint32;

/* �ṹ˵��: ������״̬ */
enum AT_CSS_ScreenStatusType {
    AT_CSS_SCREEN_STATUS_OFF = 0, /* ���� */
    AT_CSS_SCREEN_STATUS_ON  = 1, /* ���� */
    AT_CSS_SCREEN_STATUS_BUFF
};
typedef VOS_UINT8 AT_CSS_ScreenStatusTypeUint8;

typedef struct {
    /*
     * MCC��aucMcc[2]�еĴ�Ÿ�ʽ,mccΪ460:
     *              ||(BIT8)|(BIT7)|(BIT6)|(BIT5)|(BIT4)|(BIT3)|(BIT2)|(BIT1)
     *     ---------------------------------------------------------------------------
     * aucMcc[0]    ||    MCC digit 2 = 6        |           MCC digit 1 = 4
     *     ---------------------------------------------------------------------------
     * aucMcc[1]    ||    ��Ч                   |           MCC digit 3 = 0
     */
    VOS_UINT8 mcc[2]; /* MCC ID,��2�ֽ� */
    VOS_UINT8 rsv[2]; /* �����ֶΣ�����Ϊ2,����4�ֽڶ��� */
} AT_CSS_MccId;


typedef struct {
    VOS_UINT32 mcc;
    VOS_UINT32 mnc;
} CSS_AT_PlmnId;

typedef struct {
    VOS_UINT32 freqHigh;
    VOS_UINT32 freqLow;
} AT_CSS_FreqRange;

typedef struct {
    VOS_MSG_HEADER        /* _H2ASN_Skip */
    VOS_UINT32     msgId; /* _H2ASN_Skip */
    VOS_UINT16     clientId;
    VOS_UINT8      rsv[2]; /* �����ֶΣ�����Ϊ2,����4�ֽڶ��� */
} AT_CSS_MccVersionInfoReq;

typedef struct {
    VOS_MSG_HEADER        /* _H2ASN_Skip */
    VOS_UINT32     msgId; /* _H2ASN_Skip */
    VOS_UINT16     clientId;
    VOS_UINT8      rsv[1]; /* �����ֶΣ�����Ϊ1,����4�ֽڶ��� */
    VOS_UINT8      versionId[MCC_INFO_VERSION_LEN]; /* �汾�ţ��̶�Ϊxx.xx.xxx */
} CSS_AT_MccVersionInfoCnf;


typedef struct {
    VOS_MSG_HEADER        /* _H2ASN_Skip */
    VOS_UINT32     msgId; /* _H2ASN_Skip */
    /* add for fill right clientId 2016-02-23, begin  */
    MODEM_ID_ENUM_UINT16 modemId;
    /* add for fill right clientId 2016-02-23, end  */
    VOS_UINT16                    clientId;
    VOS_UINT8                     seq;                             /* ��ˮ�� */
    AT_CSS_SetMccOperateTypeUint8 operateType;                     /* �������� */
    VOS_UINT8                     versionId[MCC_INFO_VERSION_LEN]; /* �汾�ţ��̶�Ϊxx.xx.xxx */
    /* modified for fill right clientId 2016-02-23, begin  */
    VOS_UINT8 rsv[1]; /* �����ֶΣ�����Ϊ1,����4�ֽڶ��� */
    /* modified for fill right clientId 2016-02-23, end  */

    /*
     * 1)aucMccINfoBuff��洢����MCC����Ϣ���洢������ʵ��С��ulMccInfoBuffLen���¼���ֽ�����
     * 2)aucMccINfoBuff��ĸ�ʽΪAT_CSS_MCC_INFO_STRU�ṹ�ĸ�ʽ������MNC����,
     *   BAND������Ԥ��Ƶ�θ�����Ԥ��Ƶ��ĸ������ǿɱ�ġ�
     * typedef struct
     * {
     *     VOS_UINT8                           supportFlag;    // 1:��ʾ֧��GSM 2:��ʾ֧��WCDMA 4:��ʾ֧��LTE�����߿����������
     *     AT_CSS_FreqRange              freqRange;
     * } AT_CSS_FreqRangeWithRat;
     * typedef struct
     * {
     *     VOS_UINT8                                   bandInd;
     *     VOS_UINT8                                   breqRangNum;
     *     //����������ucFreqRangNum��AT_CSS_FreqRangeWithRat�ṹ
     *       ��FreqRange��Ϣ�����û��FreqRange������Ҫ��ucFreqRangNum��Ϊ0
     *     AT_CSS_FreqRangeWithRat             freqRangeArray[ucFreqRangNum];
     *     VOS_UINT8                                   preConfigFreqNum;
     *     //����������ucPreConfigFreqNum��AT_CSS_FreqRangeWithRat�ṹ
     *       ��PreConfigFreq��Ϣ�����û��PreConfigFreq������Ҫ��ucPreConfigFreqNum��Ϊ0
     *     AT_CSS_FreqRangeWithRat             preConfigFreqArray[ucPreConfigFreqNum];      //Ԥ��Ƶ���б�
     * }AT_CSS_BandInfo;
     * typedef struct
     * {
     *     MNC��aucMnc[2]�еĴ�Ÿ�ʽ��mncΪ01:
     *         ---------------------------------------------------------------------------
     *                  ||(BIT8)|(BIT7)|(BIT6)|(BIT5)|(BIT4)|(BIT3)|(BIT2)|(BIT1)
     *         ---------------------------------------------------------------------------
     *     aucMnc[0]    ||    MNC digit 3 = f        |           ��Ч
     *         ---------------------------------------------------------------------------
     *     aucMnc[1]    ||    MNC digit 2 = 1        |           MNC digit 1 = 0
     *         ---------------------------------------------------------------------------
     *     VOS_UINT8                           aucMnc[2];
     *     VOS_UINT8                           ucBandCount;// BAND�ĸ���
     *     //����������ucBandCount��band����Ϣ�����û��BAND������Ҫ��ucBandCount��Ϊ0
     *     AT_CSS_BandInfo               bandArray[ucBandCount];
     * }AT_CSS_MncInfo;
     * typedef struct
     * {
     *     MCC��aucMcc[2]�еĴ�Ÿ�ʽ,mccΪ460:
     *         ---------------------------------------------------------------------------
     *                  ||(BIT8)|(BIT7)|(BIT6)|(BIT5)|(BIT4)|(BIT3)|(BIT2)|(BIT1)
     *         ---------------------------------------------------------------------------
     *     aucMcc[0]    ||    MCC digit 2 = 6        |           MCC digit 1 = 4
     *         ---------------------------------------------------------------------------
     *     aucMcc[1]    ||    ��Ч                   |           MCC digit 3 = 0
     *         ---------------------------------------------------------------------------
     *     VOS_UINT8                           aucMcc[2];
     *     VOS_UINT8                           ucMncCount;// MNC�ĸ���
     *     //����������ucMncCount��AT_CSS_MncInfo�ṹ��mnc��Ϣ�����û��mnc������Ҫ��ucMncCount��Ϊ0
     *     AT_CSS_MncInfo                mncAarry[ucMncCount];
     * }AT_CSS_MCC_INFO_STRU;
     * 3)aucMccINfoBuff�д洢��Ϣ�ĸ�ʽAP��CSSֱ�ӶԽӣ�AT��������޸ģ�AP���γ������ĸ�ʽ��
     *   Ȼ��ת���ַ�����ʽ����AT��AT���ַ�����ʽ��ԭ������������Ȼ�󷢸�CSS��
     *   ����AP�γ�ĳһ���ֽ�Ϊ0x22��Ȼ��ת��Ϊ�ַ���'22'��AT�յ�����ת��0x22;
     * 4)aucMccINfoBuff�еĸ�ʽΪС�ˣ�
     * 5)ulMccInfoBuffLen���ܳ���1.6K��
     */
    VOS_UINT32 mccInfoBuffLen;
    VOS_UINT8  mccInfoBuff[4]; /* bugǰ4�ֽ� */
} AT_CSS_MccInfoSetReq;


typedef struct {
    VOS_MSG_HEADER                              /* _H2ASN_Skip */
        VOS_UINT32                       msgId; /* _H2ASN_Skip */
    VOS_UINT16                           clientId;
    VOS_UINT8                            seq;                                           /* ��ˮ�� */
    AT_CSS_BlockCellListOperateTypeUint8 operateType;                                   /* �������� */
    VOS_UINT8                            versionId[AT_CSS_BLOCK_CELL_LIST_VERSION_LEN]; /* �汾�ţ��̶�Ϊxx.xx.xxx */
    AT_CSS_RatUint8                      ratType;                                       /* ϵͳ��ʽ */
    VOS_UINT8                            rsv[2]; /* �����ֶΣ�����Ϊ2,����4�ֽڶ��� */

    /*
     * 1)aucBlockCellListBuff��洢����BLOCK CELL����Ϣ���洢������ʵ��С��blockCellListBuffLen���¼���ֽ�����
     * 2)aucBlockCellListBuff��ĸ�ʽΪAT_CSS_GsmCloudBlockcell�ṹ�ĸ�ʽ
     * ��������BS1,BS2��BS3�ı����ʽ��ͬ������AT_CSS_GsmCloudBlockcell�ṹ�ĸ�ʽ���룺
     * ����ucPairCount��astBlockListArray�ǿ��Ա��
     * typedef struct
     * {
     *     CSS_PlmnId    plmn;
     *     VOS_UNIT8           pairCount;
     *     //����������ucPairCount��AT_CSS_GsmCloudBlockcellPair�ṹ��pair��Ϣ��
     *       ���û�������mccû��BLOCK CELL��Ϣ��AP����Ҫ�·���MCC����Ϣ��Modem��Ϊ��MCCû��BLOCK CELL��Ϣ��
     *       ��������Ҫ���ϱ�^REPORTBLOCKCELLMCC����ȡBLOCK CELL
     *     AT_CSS_GsmCloudBlockcellPair blockListPairArray[ucPairCount];
     * }AT_CSS_GsmCloudBlockcell;
     * typedef struct
     * {
     *     VOS_UNIT32  ulMcc; //Modem�ڲ�plmn��ʽ;����460---0x00000604
     *     VOS_UNIT32  ulMnc; //Modem�ڲ�plmn��ʽ;���� 01---0x000F0100
     * }CSS_PlmnId;
     * typedef struct
     * {
     *     VOS_UNIT16  usSourceCellLai;
     *     VOS_UNIT16  usSourceCellId;
     *     VOS_UNIT16  usBlockCellArfcn; //������GSM��Ƶ����Ϣ������0x1029��ʾGSM900Ƶ��,41Ƶ��
     *     VOS_UNIT8   ucBlockCellBsic;  //����ncc,bcc;����0x14��ʾ<2,4>
     *     VOS_UNIT8   ucBlockCellReduceVal;//��Ա�BLOCK CELL�ͷ�ʱ��������dbֵ
     * }AT_CSS_GsmCloudBlockcellPair;
     */

    VOS_UINT32 blockCellListBuffLen;
    VOS_UINT8  blockCellListBuff[4]; /* bufǰ4�ֽ� */
} AT_CSS_BlockCellListSetReq;


typedef struct {
    VOS_MSG_HEADER        /* _H2ASN_Skip */
    VOS_UINT32     msgId; /* _H2ASN_Skip */
    VOS_UINT16     clientId;
    VOS_UINT8      rsv[2]; /* �����ֶΣ�����Ϊ2,����4�ֽڶ��� */
} AT_CSS_BlockCellListQueryReq;

typedef struct {
    VOS_MSG_HEADER        /* _H2ASN_Skip */
    VOS_UINT32     msgId; /* _H2ASN_Skip */
    VOS_UINT16     clientId;
    VOS_UINT8      seq; /* ��ˮ�� */
    VOS_UINT8      rsv[1]; /* �����ֶΣ�����Ϊ1,����4�ֽڶ��� */

    VOS_UINT32 result; /* 0��ʾ�ɹ���1��ʾʧ�� */
} CSS_AT_MccInfoSetCnf;

typedef struct {
    VOS_MSG_HEADER        /* _H2ASN_Skip */
    VOS_UINT32     msgId; /* _H2ASN_Skip */
    VOS_UINT16     clientId;
    VOS_UINT8      rsv[1]; /* �����ֶΣ�����Ϊ1,����4�ֽڶ��� */
    VOS_UINT8      versionId[MCC_INFO_VERSION_LEN]; /* �汾�ţ��̶�Ϊxx.xx.xxx */
    VOS_UINT32     mccNum;                          /* MCC ���� */
    AT_CSS_MccId   mccId[AT_CSS_MAX_MCC_ID_NUM];    /* MCC ID�б� */
} CSS_AT_QueryMccInfoNotify;


typedef struct {
    VOS_UINT32 mccNum;                     /* MCC ���� */
    VOS_UINT32 mcc[AT_CSS_MAX_MCC_ID_NUM]; /* MCC �б�,Modem�ڲ�plmn��ʽ;����460---0x00000604 */
} CSS_AT_BlockCellMccInfo;


typedef struct {
    VOS_MSG_HEADER        /* _H2ASN_Skip */
    VOS_UINT32     msgId; /* _H2ASN_Skip */
    VOS_UINT16     clientId;
    VOS_UINT8      seq;   /* ��ˮ�� */
    VOS_UINT8      reserved;
    VOS_UINT32     result; /* VOS_OK��ʾ�ɹ���VOS_ERR��ʾʧ�� */
} CSS_AT_BlockCellListSetCnf;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    VOS_UINT32              msgId; /* _H2ASN_Skip */
    VOS_UINT16              clientId;
    VOS_UINT8               reserved;
    VOS_UINT8               versionId[AT_CSS_BLOCK_CELL_LIST_VERSION_LEN]; /* �汾�ţ��̶�Ϊxx.xx.xxx */
    CSS_AT_BlockCellMccInfo mccInfo;                                       /* BLOCK CELLMCC��Ϣ */
} CSS_AT_BlockCellListQueryCnf;


typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    VOS_UINT32              msgId; /* _H2ASN_Skip */
    VOS_UINT16              clientId;
    VOS_UINT8               reserved;
    VOS_UINT8               versionId[AT_CSS_BLOCK_CELL_LIST_VERSION_LEN]; /* �汾�ţ��̶�Ϊxx.xx.xxx */
    CSS_AT_BlockCellMccInfo mccInfo;                                       /* BLOCK CELLMCC��Ϣ */
} CSS_AT_BlockCellMccNotify;


/* add for CloudLine Feature 2017-6-29 begin */



typedef struct {
    VOS_MSG_HEADER                                            /* _H2ASN_Skip */
    VOS_UINT32 msgId;                                         /* _H2ASN_Skip */
    VOS_UINT16     clientId;                                  /* _H2ASN_Skip */
    VOS_UINT16     lineNum;                                   /* ��·���� */
    VOS_UINT16     lineIndexList[AT_CSS_TACLAC_MAX_LINE_NUM]; /* ��·�б� */
    CSS_AT_PlmnId  plmnId;                                    /* plmn id */
} CSS_AT_LineIndexNotify;

/* add for CloudLine Feature 2017-6-29 end */


typedef struct {
    VOS_UINT8       entry;
    AT_CSS_RatUint8 rat;
    VOS_UINT16      bandId;
    CSS_AT_PlmnId   plmnId;
} AT_CSS_Mru;
/* plmn deployment, add begin, 2019-11-06 */

typedef struct {
    VOS_MSG_HEADER              /* _H2ASN_Skip */
    CSS_AT_MsgTypeUint32 msgId; /* _H2ASN_Skip */
    VOS_UINT16           clientId;
    MODEM_ID_ENUM_UINT16 modemId;
    VOS_UINT8            seq; /* ��ˮ�� */
    VOS_UINT8            versionId[AT_CSS_PLMN_DEPLOYMENT_VERSION_LEN]; /* �汾�ţ��̶�Ϊxx.xx.xxx */
    VOS_UINT8            rsv[2]; /* �����ֶΣ�����Ϊ2,����4�ֽڶ��� */

    /*
     * 1)data��洢������Ԥ��PLMN�������Ե���Ϣ���洢������ʵ��С��dataLen���¼���ֽ�����
     * 2)data��ĸ�ʽΪCSS_PlmnDeploymentInfo�ṹ�ĸ�ʽ:
     *   ��plmnNumΪ0ʱ�������·�����Ĭ�ϲ��ԡ����ظ��·�Ĭ�ϲ���ʱ������ĸ���ǰ��ġ�
     *   CSS�յ���ͬ���ԵĲ�ͬPLMNʱ������PLMN���������յ���ͬ���Ե���ͬPLMNʱ���޳��ظ��ġ�
     *   CSS���ڴ��Լ�����������·����������ã����ڵ�ǰ��ʶ��Ĳ��Զ�������֤�����ݡ�
     * 3)data�д洢��Ϣ�ĸ�ʽAP��CSSֱ�ӶԽӣ�AT��������޸ģ�AP���γ�CSS_PlmnDeploymentInfo�ĸ�ʽ��
     *   Ȼ��ת���ַ�����ʽ����AT��AT���ַ������ݲ����ģ�ֻ�ǽ��ַ�����ʽ��ԭ����������ת����CSS��
     *   ����AP�γ�ĳһ���ֽ�Ϊ0x22��Ȼ��ת��Ϊ�ַ���'22'��AT�յ�����ת��0x22��
     * 4)data�еĸ�ʽΪС�ˣ�
     * 5)dataLen���ܳ���1.6K��
     *
     * enum CSS_PlmnDeploymentType {
     *     CSS_PLMN_DEPLOY_LTE_ONLY = 0,
     *     CSS_PLMN_DEPLOY_NSA_ONLY,
     *     CSS_PLMN_DEPLOY_SA_PREFER,
     *     CSS_PLMN_DEPLOY_NSA_PREFER,
     *
     *     CSS_PLMN_DEPLOY_BUTT
     * };
     * typedef VOS_UINT8 CSS_PlmnDeploymentTypeUint8;
     *
     * typedef struct {
     *     CSS_PlmnDeploymentTypeUint8 plmnDeployType;
     *     VOS_UINT32                  plmnNum;
     *     CSS_PlmnId                  plmn[plmnNum];
     * } CSS_NV_PlmnDeploymentInfo;
     */
    VOS_UINT32 dataLen;
    VOS_UINT8  data[4]; /* dataǰ4�ֽ� */
} AT_CSS_PlmnDeploymentInfoSetReq;


typedef struct {
    VOS_MSG_HEADER               /* _H2ASN_Skip */
    CSS_AT_MsgTypeUint32 msgId;  /* _H2ASN_Skip */
    VOS_UINT32           result; /* VOS_OK��ʾ�ɹ���VOS_ERR��ʾʧ�� */
    VOS_UINT16           clientId;
    VOS_UINT8            rsv[2]; /* �����ֶΣ�����Ϊ2,����4�ֽڶ��� */
} CSS_AT_PlmnDeploymentInfoSetCnf;
/* plmn deployment, add end, 2019-11-06 */

/*
 * �ṹ˵��:APͨ������Ϣ��CSS�·���ͨ������
 */
typedef struct {
    VOS_MSG_HEADER              /* _H2ASN_Skip */
    CSS_AT_MsgTypeUint32 msgId; /* _H2ASN_Skip */
    ModemIdUint16        modemId;
    VOS_UINT16           clientId;
    VOS_UINT32           dataLen;
    VOS_UINT8            data[4]; /* ��Ϣǰ4�ֽ� */
} AT_CSS_CloudDataSetReq;

/*
 * �ṹ˵��:CSSͨ������Ϣ��ӦAP�·���ͨ�����ݵĽ��
 */
typedef struct {
    VOS_MSG_HEADER              /* _H2ASN_Skip */
    CSS_AT_MsgTypeUint32 msgId; /* _H2ASN_Skip */
    VOS_UINT16           clientId;
    VOS_UINT8            reserve[2]; /* �����ֶΣ�����Ϊ2,����4�ֽڶ��� */
    VOS_UINT32           result;
} CSS_AT_CloudDataSetCnf;

/*
 * �ṹ˵��:CSSͨ������Ϣ��AP������ͨ������
 */
typedef struct {
    VOS_MSG_HEADER              /* _H2ASN_Skip */
    CSS_AT_MsgTypeUint32 msgId; /* _H2ASN_Skip */
    VOS_UINT16           clientId;
    VOS_UINT8            reserve[2]; /* �����ֶΣ�����Ϊ2,����4�ֽڶ��� */
    VOS_UINT32           dataLen;
    VOS_UINT8            data[4]; /* ��Ϣǰ4�ֽ� */
} CSS_AT_CloudDataReport;

/*
 * �ṹ˵��:ATͨ������Ϣ�ṹ֪ͨCSS������״̬
 */
typedef struct {
    VOS_MSG_HEADER              /* _H2ASN_Skip */
    CSS_AT_MsgTypeUint32 msgId; /* _H2ASN_Skip */
    VOS_UINT8            status;
    VOS_UINT8            reserve[3]; /* �����ֶΣ�����Ϊ3,����4�ֽڶ��� */
} AT_CSS_ScreenStatusInd;

/* ASN�����ṹ */
typedef struct {
    VOS_UINT32 msgId; /* _H2ASN_MsgChoice_Export CSS_AT_MsgTypeUint32  */
    VOS_UINT8  msg[4]; /* ��Ϣǰ4�ֽ� */
    /* _H2ASN_MsgChoice_When_Comment          CSS_AT_MSG_TYPE_ENUM_UINT32 */
} CSS_AT_InterfaceMsgData;
/* _H2ASN_Length UINT32 */

typedef struct {
    VOS_MSG_HEADER
    CSS_AT_InterfaceMsgData msgData;
} CSS_AT_InterfaceMsg;

#if ((VOS_OS_VER == VOS_WIN32) || (VOS_OS_VER == VOS_NUCLEUS))
#pragma pack()
#else
#pragma pack(0)
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
