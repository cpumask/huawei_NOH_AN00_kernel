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

/* 生产使用的加密结构 */

/* WIFI KEY字符串长度，与NVIM模块Ipcom_parameters_handle.h文件的 NV_WLKEY_LEN 保持一致 */
#define AT_NV_WLKEY_LEN 27

/* 鉴权模式字符串长度，与NVIM模块Ipcom_parameters_handle.h文件的 NV_WLAUTHMODE_LEN 保持一致 */
#define AT_NV_WLAUTHMODE_LEN 16

/* 加密模式字符串长度，与NVIM模块Ipcom_parameters_handle.h文件的 NV_ENCRYPTIONMODES_LEN 保持一致 */
#define AT_NV_ENCRYPTIONMODES_LEN 5

/* WPA的密码字符串长度，与NVIM模块Ipcom_parameters_handle.h文件的 NV_WLWPAPSK_LEN 保持一致 */
#define AT_NV_WLWPAPSK_LEN 65

/* NV项50012的数据结构nv_wifisec_type WIFI KEY后面的数据长度 */
#define AT_NV_WIFISEC_OTHERDATA_LEN 72

/* 单板侧支持的WIFI KEY个数 */
#define AT_WIWEP_CARD_WIFI_KEY_TOTAL 4

/* 工位侧支持的WIFI KEY个数 */
#define AT_WIWEP_TOOLS_WIFI_KEY_TOTAL 16

/* PHYNUM命令物理号类型MAC的物理号长度 */
#define AT_PHYNUM_MAC_LEN 12

/* PHYNUM命令物理号类型MAC的物理号与^WIFIGLOBAL命令匹配需要增加的冒号个数 */
#define AT_PHYNUM_MAC_COLON_NUM 5

#define AT_TSELRF_PATH_TOTAL 4

#define AT_WIFIGLOBAL_MAC_LEN_BETWEEN_COLONS 2 /* MAC地址中冒号之间的字符个数 */

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
    AT_NOT_SET_STATE            = 0, /* 未设置芯片模式、状态 */
    AT_NO_DX_SECU_DISABLE_STATE = 0, /* 无DX加密引擎非安全模式、状态 */
    AT_NO_DX_SECU_ENABLE_STATE  = 1, /* 无DX加密引擎安全模式、状态 */
    AT_DX_RMA_STATE             = 2, /* 基于DX加密引擎非安全模式、状态 */
    AT_DX_SECURE_STATE          = 3, /* 基于DX加密引擎安全模式、状态 */
    AT_SECURE_STATE_BUTT
};

enum AT_DRV_ReturnState {
    AT_DRV_NOT_SETTED_STATE = 0, /* 未设置芯片状态 */
    AT_DRV_STATE_SECURE     = 1, /* 安全状态 */
    AT_DRV_STATE_RMA        = 2, /* 非安全状态 */
    AT_DRV_STATE_BUTT
};

enum AT_SetState {
    AT_SET_SECURE_STATE = 1, /* 设置为安全状态 */
    AT_SET_RMA_STATE    = 2, /* 设置为非安全状态 */
    AT_SET_STATE_BUTT
};

#define SLT_SUCCESS 0
#define SLT_ERROR 1

#define AT_AUTHKEY_LEN 8   /* 64 Bits的AUTH KEY 码流长度 */
#define AT_KCE_LEN 16      /* 128bit KCE加解密key值长度 */
#define AT_SOCID_LEN 32    /* 256Bits的SOCID码流长度 */
#define AT_KEY_HASH_LEN 32 /* SOCID或者AUTHKEY的hash值长度 */
#define AT_KEYBUFF_LEN 32
#ifdef MBB_SLT
#define AT_BSN_LEN 16 /* BSN号码长度 */
#endif

#define AT_SECUREDEBUG_VALUE 3 /* 设置安全DEBUG授权由安全证书控制 */
#define AT_SECDBGRESET_VALUE 1 /* 对安全世界和Coresight 调试时，临时复位SecEngine */
#define AT_CSRESET_VALUE 1     /* 对安全世界和Coresight 调试时，临时复位SecEngine */
#define AT_DFTSEL_VALUE 1      /* 可以通过密码验证方式开启安全DFT功能 */
#define AT_DFTSEL_DISABLE_VALUE 3 /* 禁止DFT调试功能 */

/*
 * 由于 g_stATDislogPwd 中的 DIAG口的状态要放入备份NV列表; 而密码不用备份
 * 故将 g_stATDislogPwd 中的密码废弃, 仅使用其中的 DIAG 口状态;
 * 重新定义NV项用来保存密码
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
