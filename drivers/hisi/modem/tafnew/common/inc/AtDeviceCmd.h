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
#ifndef _ATDEVICECMD_H_
#define _ATDEVICECMD_H_

#include "v_typdef.h"
#include "ATCmdProc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)

/* ����ʹ�õļ��ܽṹ */

/* WIFI KEY�ַ������ȣ���NVIMģ��Ipcom_parameters_handle.h�ļ��� NV_WLKEY_LEN ����һ�� */
#define AT_NV_WLKEY_LEN 27

/* ��Ȩģʽ�ַ������ȣ���NVIMģ��Ipcom_parameters_handle.h�ļ��� NV_WLAUTHMODE_LEN ����һ�� */
#define AT_NV_WLAUTHMODE_LEN 16

/* ����ģʽ�ַ������ȣ���NVIMģ��Ipcom_parameters_handle.h�ļ��� NV_ENCRYPTIONMODES_LEN ����һ�� */
#define AT_NV_ENCRYPTIONMODES_LEN 5

/* WPA�������ַ������ȣ���NVIMģ��Ipcom_parameters_handle.h�ļ��� NV_WLWPAPSK_LEN ����һ�� */
#define AT_NV_WLWPAPSK_LEN 65

/* NV��50012�����ݽṹnv_wifisec_type WIFI KEY��������ݳ��� */
#define AT_NV_WIFISEC_OTHERDATA_LEN 72

/* �����֧�ֵ�WIFI KEY���� */
#define AT_WIWEP_CARD_WIFI_KEY_TOTAL 4

/* ��λ��֧�ֵ�WIFI KEY���� */
#define AT_WIWEP_TOOLS_WIFI_KEY_TOTAL 16

/* PHYNUM�������������MAC������ų��� */
#define AT_PHYNUM_MAC_LEN 12

/* PHYNUM�������������MAC���������^WIFIGLOBAL����ƥ����Ҫ���ӵ�ð�Ÿ��� */
#define AT_PHYNUM_MAC_COLON_NUM 5

#define AT_TSELRF_PATH_TOTAL 4

#define AT_WIFIGLOBAL_MAC_LEN_BETWEEN_COLONS 2 /* MAC��ַ��ð��֮����ַ����� */

#define BAND_WIDTH_NUMS 6

#define UIMID_DATA_LEN 4


enum AT_TbatOperationType {
    AT_TBAT_BATTERY_ANALOG_VOLTAGE  = 0,
    AT_TBAT_BATTERY_DIGITAL_VOLTAGE = 1,
    AT_TBAT_BATTERY_VOLUME          = 2,
    AT_TBAT_OPERATION_TYPE_BUTT
};
typedef VOS_UINT32 AT_TbatOperationTypeUint32;


enum AT_TbatOperationDirection {
    AT_TBAT_READ_FROM_UUT,
    AT_TBAT_SET_TO_UUT,
    AT_TBAT_OPERATION_DIRECTION_BUTT
};
typedef VOS_UINT32 AT_TbatOperationDirectionUint32;


enum AT_KeyType {
    AT_KEY_TYPE_DIEID    = 1,
    AT_KEY_TYPE_TBOX_SMS = 2,
    AT_KEY_TYPE_SOCID    = 3,
    AT_KEY_TYPE_AUTHKEY  = 4,
    AT_KEY_TYPE_BUTT
};

enum AT_SecureState {
    AT_NOT_SET_STATE            = 0, /* δ����оƬģʽ��״̬ */
    AT_NO_DX_SECU_DISABLE_STATE = 0, /* ��DX��������ǰ�ȫģʽ��״̬ */
    AT_NO_DX_SECU_ENABLE_STATE  = 1, /* ��DX�������氲ȫģʽ��״̬ */
    AT_DX_RMA_STATE             = 2, /* ����DX��������ǰ�ȫģʽ��״̬ */
    AT_DX_SECURE_STATE          = 3, /* ����DX�������氲ȫģʽ��״̬ */
    AT_SECURE_STATE_BUTT
};

enum AT_DRV_ReturnState {
    AT_DRV_NOT_SETTED_STATE = 0, /* δ����оƬ״̬ */
    AT_DRV_STATE_SECURE     = 1, /* ��ȫ״̬ */
    AT_DRV_STATE_RMA        = 2, /* �ǰ�ȫ״̬ */
    AT_DRV_STATE_BUTT
};

enum AT_SetState {
    AT_SET_SECURE_STATE = 1, /* ����Ϊ��ȫ״̬ */
    AT_SET_RMA_STATE    = 2, /* ����Ϊ�ǰ�ȫ״̬ */
    AT_SET_STATE_BUTT
};

#define SLT_SUCCESS 0
#define SLT_ERROR 1

#define AT_AUTHKEY_LEN 8   /* 64 Bits��AUTH KEY �������� */
#define AT_KCE_LEN 16      /* 128bit KCE�ӽ���keyֵ���� */
#define AT_SOCID_LEN 32    /* 256Bits��SOCID�������� */
#define AT_KEY_HASH_LEN 32 /* SOCID����AUTHKEY��hashֵ���� */
#define AT_KEYBUFF_LEN 32
#ifdef MBB_SLT
#define AT_BSN_LEN 16 /* BSN���볤�� */
#endif

#define AT_SECUREDEBUG_VALUE 3 /* ���ð�ȫDEBUG��Ȩ�ɰ�ȫ֤����� */
#define AT_SECDBGRESET_VALUE 1 /* �԰�ȫ�����Coresight ����ʱ����ʱ��λSecEngine */
#define AT_CSRESET_VALUE 1     /* �԰�ȫ�����Coresight ����ʱ����ʱ��λSecEngine */
#define AT_DFTSEL_VALUE 1      /* ����ͨ��������֤��ʽ������ȫDFT���� */
#define AT_DFTSEL_DISABLE_VALUE 3 /* ��ֹDFT���Թ��� */

/*
 * ���� g_stATDislogPwd �е� DIAG�ڵ�״̬Ҫ���뱸��NV�б�; �����벻�ñ���
 * �ʽ� g_stATDislogPwd �е��������, ��ʹ�����е� DIAG ��״̬;
 * ���¶���NV��������������
 */
extern VOS_INT8 g_atOpwordPwd[AT_OPWORD_PWD_LEN + 1];


typedef struct {
    VOS_UINT8 aucwlKeys[AT_NV_WLKEY_LEN];
} AT_WifisecWiwepInfo;

VOS_UINT32 AT_SetTbatPara(VOS_UINT8 indexNum);

VOS_UINT32 AT_QryTbatPara(VOS_UINT8 indexNum);

VOS_UINT32 AT_SetPstandbyPara(VOS_UINT8 indexNum);

VOS_UINT32 AT_SetWiwepPara(VOS_UINT8 indexNum);

VOS_UINT32 AT_QryWiwepPara(VOS_UINT8 indexNum);

VOS_UINT32 AT_TestWiwepPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_SetCmdlenPara(VOS_UINT8 indexNum);

VOS_UINT32 AT_QryCmdlenPara(VOS_UINT8 indexNum);

VOS_UINT32 AT_UpdateMacPara(VOS_UINT8 aucMac[], VOS_UINT16 macLength);

VOS_UINT32 AT_SetTmodeAutoPowerOff(VOS_UINT8 indexNum);

VOS_UINT32 AT_ExistSpecificPort(VOS_UINT8 portType);

VOS_UINT32 AT_OpenDiagPort(VOS_VOID);
VOS_UINT32 AT_CloseDiagPort(VOS_VOID);
VOS_UINT32 AT_CheckSetPortRight(VOS_UINT8 aucOldRewindPortStyle[], VOS_UINT8 aucNewRewindPortStyle[]);

VOS_UINT32 AT_TestHsicCmdPara(VOS_UINT8 indexNum);

VOS_UINT32 AT_TestSimlockUnlockPara(VOS_UINT8 indexNum);

VOS_UINT32 AT_SetNVReadPara(VOS_UINT8 clientId);
#ifdef FEATURE_AT_NV_WRITE_SUPPORT
VOS_UINT32 AT_SetNVWritePara(VOS_UINT8 clientId);
#endif

VOS_VOID At_RfPllStatusCnfProc(PHY_AT_RfPllStatusCnf *msg);

VOS_VOID At_RfFpowdetTCnfProc(PHY_AT_PowerDetCnf *msg);

VOS_UINT32 AT_NVWRGetParaInfo(AT_ParseParaType *para, VOS_UINT8 *pu8Data, VOS_UINT32 bufLen, VOS_UINT32 *len);

#ifdef FEATURE_AT_NV_WRITE_SUPPORT
VOS_UINT32 AT_SetNvwrSecCtrlPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvMtaNvwrSecCtrlSetCnf(struct MsgCB *msg);
VOS_UINT32 AT_QryNvwrSecCtrlPara(VOS_UINT8 indexNum);
#endif
VOS_BOOL AT_IsNVWRAllowedNvId(VOS_UINT16 nvId);

extern VOS_UINT32 AT_AsciiToHex(VOS_UINT8 *src, VOS_UINT8 *dst);

extern VOS_UINT32 AT_AsciiToHexCode_Revers(VOS_UINT8 *src, VOS_UINT16 dataLen, VOS_UINT8 *dst);

extern VOS_UINT32 AT_Hex2Ascii_Revers(VOS_UINT8 aucHex[], VOS_UINT32 length, VOS_UINT8 aucAscii[]);

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
extern VOS_UINT32 AT_SetMeidPara(VOS_UINT8 indexNum);

extern VOS_UINT32 AT_QryMeidPara(VOS_UINT8 indexNum);
#endif

VOS_UINT32 AT_QryRficDieIDPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvMtaRficDieIDQryCnf(struct MsgCB *msg);
VOS_UINT16 AT_RficDieIDOut(VOS_UINT8 *msg, VOS_UINT32 rficNum, VOS_UINT16 length, VOS_UINT8 indexNum);

VOS_UINT32 AT_QryRffeDieIDPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvMtaRffeDieIDQryCnf(struct MsgCB *msg);
VOS_UINT16 AT_RffeDieIDOut(VOS_UINT8 *msg, VOS_UINT32 rffeNum, VOS_UINT16 length, VOS_UINT8 indexNum);

VOS_UINT32 AT_SetPmuDieSNPara(VOS_UINT8 indexNum);

VOS_UINT32 AT_QryPmuDieSNPara(VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvMtaPmuDieSNQryCnf(struct MsgCB *msg);

VOS_UINT32 AT_SetTasTestCfg(VOS_UINT8 indexNum);
VOS_UINT32 AT_RcvMtaTasTestCfgCnf(struct MsgCB *msg);
#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
VOS_UINT32 AT_QryTasTestCfgPara(VOS_UINT8 indexNum);
#endif
VOS_UINT16 AT_TasTestOut(MTA_AT_TasTestQryCnf *mtaAtTasTestReqCnf);
VOS_UINT32 AT_RcvMtaTasTestQryCnf(struct MsgCB *msg);
#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
VOS_UINT32 AT_SetCdmaAttDiversitySwitch(VOS_UINT8 indexNum);
#endif

#if (FEATURE_DCXO_HI1102_SAMPLE_SHARE == FEATURE_ON)
VOS_UINT32 AT_SetDcxoQryPara(VOS_UINT8 indexNum);
#endif

VOS_UINT32 AT_RcvMtaSetMipiReadCnf(struct MsgCB *msg);

VOS_UINT32 AT_RcvMtaSetPhyMipiWriteCnf(struct MsgCB *msg);

#ifdef MBB_SLT
VOS_UINT32 AT_SetUsbTestPara(VOS_UINT8 indexNum);
#endif

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
    VOS_UINT32 AT_SetSltTestPara(VOS_UINT8 indexNum);
#endif

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of AtDeviceCmd.h */
