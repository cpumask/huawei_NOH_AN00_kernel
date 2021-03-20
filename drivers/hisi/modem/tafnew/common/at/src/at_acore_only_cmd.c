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

#include "at_acore_only_cmd.h"
#include "securec.h"

#include "ATCmdProc.h"
#include "AtDeviceCmd.h"
#include "AtCheckFunc.h"
#include "AtTafAgentInterface.h"
#include "at_lte_common.h"
#include "mn_comm_api.h"
#include "at_mdrv_interface.h"

#if (VOS_OS_VER == VOS_LINUX)
#include <linux/kconfig.h>
#include <linux/pm_wakeup.h>
#ifdef CONFIG_HISI_BALONG_MODEM
#include <linux/mtd/hisi_nve_interface.h>
#if (FEATURE_AGPS_GPIO == FEATURE_ON)
#include <linux/version.h>
#if (KERNEL_VERSION(3, 13, 0) <= LINUX_VERSION_CODE)
#include <huawei_platform/connectivity/huawei_gps.h>
#else
#include <linux/huawei/gps/huawei_gps.h>
#endif /* #if (KERNEL_VERSION(3, 13, 0) <= LINUX_VERSION_CODE) */
#endif /* #if (FEATURE_AGPS_GPIO == FEATURE_ON) */
#endif /* #ifdef CONFIG_HISI_BALONG_MODEM */
#else
#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
#include "Linuxstub.h"
#endif
#endif /* #if (VOS_OS_VER == VOS_LINUX) */


#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)
#if (VOS_OS_VER == VOS_LINUX)
#include <linux/pm_wakeup.h>
#else
#include "Linuxstub.h"
#endif
#endif

#include "securec.h"


#define THIS_FILE_ID PS_FILE_ID_AT_ACORE_ONLY_CMD_C
#define AT_SET_LOAD_TASK_DELAY_TIME 500
#define AT_WIRX_ONOFF 0
#define AT_WIPOW_PARA_NUM 1
#define AT_WIPOW_WIFI_DBM_PERCENT 0

#define AT_SD_DATA_BYTE_CONTENT_IS_HEX_00 0 /* �ֽ�����Ϊ0x00 */
#define AT_SD_DATA_BYTE_CONTENT_IS_HEX_55 1 /* �ֽ�����Ϊ0x55 */
#define AT_SD_DATA_BYTE_CONTENT_IS_HEX_AA 2 /* �ֽ�����Ϊ0xAA */
#define AT_SD_DATA_BYTE_CONTENT_IS_HEX_FF 3 /* �ֽ�����Ϊ0xFF */

#if (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_OFF)

VOS_UINT g_mcastWifiRxPkts;

/*
 * 5010��ATģ��������ƽ̨�Ϸֱ�����C��\A�ˣ�����AT����ֻ����A����Ч��
 * ����������ڽ���A����Ч��AT������ñ��AT����ֻ�е�ATģ�������A��ʱ����Ч
 */
AT_ParCmdElement g_atAcoreOnlyCmdTbl[] = {
    { AT_CMD_GPIO,
      At_SetGpioPara, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, AT_TestGpioPara, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (TAF_UINT8 *)"^GPIO", (VOS_UINT8 *)"(0-65535),(0,1),(0-1000)" },

    { AT_CMD_DLOADINFO,
      VOS_NULL_PTR, AT_NOT_SET_TIME, atQryDLoadInfo, AT_UPGRADE_TIME_5S, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_ERROR, CMD_TBL_PIN_IS_LOCKED | CMD_TBL_E5_IS_LOCKED,
      (VOS_UINT8 *)"^DLOADINFO", VOS_NULL_PTR },

/* HUAWEI �ƶ������Ʒ�ն��豸һ�������ӿڹ淶V1.07.doc �ĵ��漰���������δ��һ: V700R001�汾ʹ��LTE��ʵ�֣�V300R002�汾ʹ��GUģ��ʵ�� */
    { AT_CMD_DLOADVER,
      VOS_NULL_PTR, AT_NOT_SET_TIME, atQryDLoadVer, AT_UPGRADE_TIME_5S, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_ERROR, CMD_TBL_PIN_IS_LOCKED | CMD_TBL_E5_IS_LOCKED,
      (VOS_UINT8 *)"^DLOADVER", VOS_NULL_PTR },

    { AT_CMD_SECURESTATE,
      AT_SetSecureStatePara, AT_NOT_SET_TIME, AT_QrySecureStatePara, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^SECURESTATE", (VOS_UINT8 *)"(1)" },

    { AT_CMD_SECDBGSTATE,
      AT_SetSecDbgStatePara, AT_NOT_SET_TIME, AT_QrySecDbgStatePara, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^SECDBGSTATE", (VOS_UINT8 *)"(3)" },

    { AT_CMD_SOCID,
      VOS_NULL_PTR, AT_NOT_SET_TIME, AT_QrySocidPara, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^SOCID", (VOS_UINT8 *)"(@SocidString)" },

    { AT_CMD_CBC,
      At_SetCbcPara, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED | CMD_TBL_CLAC_IS_INVISIBLE_STICK,
      (VOS_UINT8 *)"+CBC", (VOS_UINT8 *)"(0-3),(0-100)" },

    { AT_CMD_TCHRINFO,
      VOS_NULL_PTR, AT_NOT_SET_TIME, AT_QryChrgInfoPara, AT_NOT_SET_TIME, At_CmdTestProcERROR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^TCHRINFO", VOS_NULL_PTR },

    { AT_CMD_RESET,
      atSetReset, AT_UPGRADE_TIME_5S, VOS_NULL_PTR, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_ERROR, CMD_TBL_PIN_IS_LOCKED | CMD_TBL_E5_IS_LOCKED,
      (VOS_UINT8 *)"^RESET", VOS_NULL_PTR },

    { AT_CMD_NVRESTORE,
      atSetNVRestore, AT_UPGRADE_TIME_50S, VOS_NULL_PTR, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_ERROR, CMD_TBL_PIN_IS_LOCKED | CMD_TBL_E5_IS_LOCKED,
      (VOS_UINT8 *)"^NVRESTORE", VOS_NULL_PTR },

    { AT_CMD_SD,
      At_SetSD, AT_NOT_SET_TIME, At_QrySD, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_SD_CARD_OTHER_ERR, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^SD", VOS_NULL_PTR },

    { AT_CMD_CGPSCLOCK,
      AT_SetCgpsClockPara, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^CGPSCLOCK", (VOS_UINT8 *)"(0,1),(0-4)" },

    { AT_CMD_SECUBOOT,
      At_SetSecuBootPara, AT_SET_PARA_TIME, At_QrySecuBootPara, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^SECUBOOT", (VOS_UINT8 *)"(0-3)" },

    { AT_CMD_SECUBOOTFEATURE,
      VOS_NULL_PTR, AT_NOT_SET_TIME, At_QrySecuBootFeaturePara, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^SECUBOOTFEATURE", (VOS_UINT8 *)"(0-3)" },

    /* V7R1Ҫ��ʹ��"=?"��ѯ���ʹ��״̬ */
    { AT_CMD_TCHRENABLE,
      AT_SetChrgEnablePara, AT_NOT_SET_TIME, AT_QryChrgEnablePara, AT_NOT_SET_TIME, AT_TestChrgEnablePara, AT_SET_PARA_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^TCHRENABLE", (VOS_UINT8 *)"(0,1,4)" },

    { AT_CMD_WIPARANGE,
      AT_SetWifiPaRangePara, AT_NOT_SET_TIME, AT_QryWifiPaRangePara, AT_NOT_SET_TIME, AT_TestWifiPaRangePara, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^WIPARANGE", (VOS_UINT8 *)"(@gainmode)" },

    { AT_CMD_WIINFO,
      AT_SetWifiInfoPara, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, At_CmdTestProcERROR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_ERROR, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^WIINFO", (VOS_UINT8 *)"(0,1),(0,1)" },

    { AT_CMD_WIRPCKG,
      AT_SetWiFiPacketPara, AT_NOT_SET_TIME, AT_QryWiFiPacketPara, AT_NOT_SET_TIME, At_CmdTestProcERROR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^WIRPCKG", (VOS_UINT8 *)"(0)" },

    { AT_CMD_WITX,
      AT_SetWiFiTxPara, AT_NOT_SET_TIME, AT_QryWiFiTxPara, AT_NOT_SET_TIME, At_CmdTestProcERROR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^WITX", (VOS_UINT8 *)"(0,1)" },

    { AT_CMD_WIRX,
      AT_SetWiFiRxPara, AT_NOT_SET_TIME, AT_QryWiFiRxPara, AT_NOT_SET_TIME, At_CmdTestProcERROR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^WIRX", (VOS_UINT8 *)"(0,1),(@smac),(@dmac)" },

    { AT_CMD_WIENABLE,
      AT_SetWiFiEnablePara, AT_NOT_SET_TIME, AT_QryWiFiEnablePara, AT_NOT_SET_TIME, At_CmdTestProcERROR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^WIENABLE", (VOS_UINT8 *)"(0,1,2)" },

    { AT_CMD_WIMODE,
      AT_SetWiFiModePara, AT_NOT_SET_TIME, AT_QryWiFiModePara, AT_NOT_SET_TIME, At_CmdTestProcERROR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^WIMODE", (VOS_UINT8 *)"(0,1,2,3,4)" },

    { AT_CMD_WIFREQ,
      AT_SetWiFiFreqPara, AT_NOT_SET_TIME, AT_QryWiFiFreqPara, AT_NOT_SET_TIME, At_CmdTestProcERROR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^WIFREQ", (VOS_UINT8 *)"(0-65535),(@offset)" },

    { AT_CMD_WIPOW,
      AT_SetWiFiPowerPara, AT_NOT_SET_TIME, AT_QryWiFiPowerPara, AT_NOT_SET_TIME, At_CmdTestProcERROR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^WIPOW", (VOS_UINT8 *)"(0-65535)" },

    { AT_CMD_WIBAND,
      AT_SetWiFiBandPara, AT_NOT_SET_TIME, AT_QryWiFiBandPara, AT_NOT_SET_TIME, At_CmdTestProcERROR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^WIBAND", (VOS_UINT8 *)"(0,1)" },

    { AT_CMD_WIRATE,
      AT_SetWiFiRatePara, AT_NOT_SET_TIME, AT_QryWiFiRatePara, AT_NOT_SET_TIME, At_CmdTestProcERROR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^WIDATARATE", (VOS_UINT8 *)"(0-65535)" },

    { AT_CMD_SSID,
      AT_SetWiFiSsidPara, AT_NOT_SET_TIME, AT_QryWiFiSsidPara, AT_NOT_SET_TIME, AT_TestSsidPara, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^SSID", (VOS_UINT8 *)"(0-3),(@SSID)" },

    { AT_CMD_WIKEY,
      AT_SetWiFiKeyPara, AT_NOT_SET_TIME, AT_QryWiFiKeyPara, AT_NOT_SET_TIME, AT_TestWikeyPara, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^WIKEY", (VOS_UINT8 *)"(0-3),(@WIKEY)" },

    { AT_CMD_WILOG,
      AT_SetWiFiLogPara, AT_NOT_SET_TIME, AT_QryWiFiLogPara, AT_NOT_SET_TIME, At_CmdTestProcERROR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^WILOG", (VOS_UINT8 *)"(0,1),(@content)" },


#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
    { AT_CMD_WAKELOCK,
      At_SetWakelock, AT_NOT_SET_TIME, AT_QryWakelock, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^WAKELOCK", (VOS_UINT8 *)"(0,1)" },

    { AT_CMD_CCPUNIDDISABLE,
      At_SetCcpuNidDisablePara, AT_SET_PARA_TIME, At_QryCcpuNidDisablePara, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^CCPUNIDDISABLE", (VOS_UINT8 *)"(0,1)" },

    { AT_CMD_ACPUNIDDISABLE,
      At_SetAcpuNidDisablePara, AT_SET_PARA_TIME, At_QryAcpuNidDisablePara, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^ACPUNIDDISABLE", (VOS_UINT8 *)"(0,1)" },

    { AT_CMD_GODLOAD,
      atSetGodLoad, AT_UPGRADE_TIME_5S, VOS_NULL_PTR, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_ERROR, CMD_TBL_PIN_IS_LOCKED | CMD_TBL_E5_IS_LOCKED,
      (VOS_UINT8 *)"^GODLOAD", VOS_NULL_PTR },

    { AT_CMD_KCE,
      AT_SetKcePara, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^KCE", (VOS_UINT8 *)"(@KceString)" },

    { AT_CMD_APBATLVL,
      AT_SetApbatlvlPara, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^APBATLVL", VOS_NULL_PTR },

    { AT_CMD_SETKEY,
      At_SetKeyPara, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^SETKEY", (VOS_UINT8 *)"(1-4)" },

    { AT_CMD_GETKEYINFO,
      At_GetKeyInfoPara, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^GETKEYINFO", (VOS_UINT8 *)"(1-4)" },

    { AT_CMD_AUTHKEYRD,
      AT_SetAuthKeyRdPara, AT_NOT_SET_TIME, AT_QryAuthKeyRdPara, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^AUTHKEYRD", (VOS_UINT8 *)"(0,1)" },
#endif
};


VOS_UINT32 At_RegisterAcoreOnlyCmdTable(VOS_VOID)
{
    return AT_RegisterCmdTable(g_atAcoreOnlyCmdTbl, sizeof(g_atAcoreOnlyCmdTbl) / sizeof(g_atAcoreOnlyCmdTbl[0]));
}


VOS_UINT32 At_SetGpioPara(VOS_UINT8 indexNum)
{
    VOS_UINT8 group;
    VOS_UINT8 pin;
    VOS_UINT8 value;

    /* �����֧��HSIC���ԣ���֧��GPIO�л� */
    if (AT_IsMiscSupport(BSP_MODULE_TYPE_HSIC) != BSP_MODULE_SUPPORT) {
        return AT_ERROR;
    }

    /* para too many */
    if (g_atParaIndex > 3) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    group = (VOS_UINT8)(g_atParaList[0].paraValue / GPIO_GROUP_BASE);
    pin   = (VOS_UINT8)(g_atParaList[0].paraValue % GPIO_GROUP_BASE);

    if (AT_SetDrvGpio(group, pin, (VOS_UINT8)g_atParaList[1].paraValue) != VOS_OK) {
        return AT_ERROR;
    }

    VOS_TaskDelay(g_atParaList[2].paraValue);

    /* ���GPIO���ú���Ҫ����ƽ��ת���� */
    value = (g_atParaList[1].paraValue == DRV_GPIO_HIGH) ? DRV_GPIO_LOW : DRV_GPIO_HIGH;

    if (AT_SetDrvGpio(group, pin, value) != VOS_OK) {
        return AT_ERROR;
    }

    return AT_OK;
}


VOS_UINT32 AT_TestGpioPara(VOS_UINT8 indexNum)
{
    /* �����֧��HSIC���ԣ���֧��^GPIO�������� */
    if (AT_IsMiscSupport(BSP_MODULE_TYPE_HSIC) != BSP_MODULE_SUPPORT) {
        return AT_ERROR;
    }

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: (0-65535),(0,1),(0-1000)", g_parseContext[indexNum].cmdElement->cmdName);

    return AT_OK;
}


VOS_INT32 AT_SetSecDbgStateVal(VOS_VOID)
{
    VOS_INT32 result;
    VOS_INT32 qryResult;

    /* 1.ִ��SECUREDEBUG=3,���ð�ȫDEBUG��Ȩ�ɰ�ȫ֤����� */
    result = AT_EfuseIoctl(MDRV_EFUSE_IOCTL_CMD_SET_SECUREDEBUG, AT_SECUREDEBUG_VALUE, NULL, 0);

    /*
     * ���ýӿ�ֻ�����ַ���ֵ��<0,ִ�д���=0��ִ����ȷ��=1���ظ����ã�
     * ��������ֻ�жϷ��ش������
     */
    if (result < MDRV_OK) {
        AT_WARN_LOG("AT_SetSecDbgStateVal: set SECUREDEBUG state error.");
        return AT_ERROR;
    }

    /* ������ɺ��������Ƿ�ɹ� */
    qryResult = AT_EfuseIoctl(MDRV_EFUSE_IOCTL_CMD_GET_SECUREDEBUG, 0, NULL, 0);

    if (qryResult != AT_SECUREDEBUG_VALUE) {
        AT_WARN_LOG("AT_SetSecDbgStateVal: set SECUREDEBUG state not correct.");
        return AT_ERROR;
    }

    /*
     * 2.ִ��SECDBGRESET=1��CSRESET=1,�ڶ԰�ȫ�����Coresight����ʱ��
     * ��ʱ��λSecEngine
     */
    /* 2.1  SECDBGRESET=1 */
    result = AT_EfuseIoctl(MDRV_EFUSE_IOCTL_CMD_SET_SECDBGRESET, AT_SECDBGRESET_VALUE, NULL, 0);

    if (result < MDRV_OK) {
        AT_WARN_LOG("AT_SetSecDbgStateVal: set SECDBGRESET state error.");
        return AT_ERROR;
    }

    /* ������ɺ��������Ƿ�ɹ� */
    qryResult = AT_EfuseIoctl(MDRV_EFUSE_IOCTL_CMD_GET_SECDBGRESET, 0, NULL, 0);

    if (qryResult != AT_SECDBGRESET_VALUE) {
        AT_WARN_LOG("AT_SetSecDbgStateVal: set SECDBGRESET state not correct.");
        return AT_ERROR;
    }

    /* 2.2  CSRESET=1 */
    result = AT_EfuseIoctl(MDRV_EFUSE_IOCTL_CMD_SET_CSRESET, AT_CSRESET_VALUE, NULL, 0);
    if (result < MDRV_OK) {
        AT_WARN_LOG("AT_SetSecDbgStateVal: set CSRESET state error.");
        return AT_ERROR;
    }

    /* ������ɺ��������Ƿ�ɹ� */
    qryResult = AT_EfuseIoctl(MDRV_EFUSE_IOCTL_CMD_GET_CSRESET, 0, NULL, 0);

    if (qryResult != AT_CSRESET_VALUE) {
        AT_WARN_LOG("AT_SetSecDbgStateVal: set CSRESET state not correct.");
        return AT_ERROR;
    }

    AT_NORM_LOG("AT_SetSecDbgStateVal: set state correct.");
    return AT_OK;
}

/* �������û���ѯ������Ϣ�����ڷ��ص���ͺ�̨�汾�š���Ʒ�ͺ����ơ�����������Ϣ */
VOS_UINT32 atQryDLoadInfo(VOS_UINT8 clientId)
{
    DLOAD_VER_INFO_S *dloadVerInfo = VOS_NULL_PTR;

    dloadVerInfo = AT_GetDloadInfo();

    if (dloadVerInfo == NULL) {
        CmdErrProc(clientId, ERR_MSP_FAILURE, 0, NULL);
        return AT_ERROR;
    }

    if ((dloadVerInfo->softwarever == NULL) || (dloadVerInfo->productname == NULL) || (dloadVerInfo->isover == NULL)) {
        CmdErrProc(clientId, ERR_MSP_FAILURE, 0, NULL);
        return AT_ERROR;
    }

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)(g_atSndCodeAddress),
        (VOS_CHAR *)(g_atSndCodeAddress + g_atSendDataBuff.bufLen), "\rswver:%s\r\n", dloadVerInfo->softwarever);

    g_atSendDataBuff.bufLen += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)(g_atSndCodeAddress),
        (VOS_CHAR *)(g_atSndCodeAddress + g_atSendDataBuff.bufLen), "\r\nisover:%s\r\n", dloadVerInfo->isover);

    g_atSendDataBuff.bufLen += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)(g_atSndCodeAddress),
        (VOS_CHAR *)(g_atSndCodeAddress + g_atSendDataBuff.bufLen), "\r\nproduct name:%s\r\n",
        dloadVerInfo->productname);

    return AT_OK;
}

VOS_UINT32 atQryDLoadVer(VOS_UINT8 clientId)
{
    DLOAD_VER_INFO_S *dloadVerInfo = VOS_NULL_PTR;

    dloadVerInfo = AT_GetDloadInfo();

    if (dloadVerInfo == NULL) {
        CmdErrProc(clientId, ERR_MSP_FAILURE, 0, NULL);
        return AT_ERROR;
    }

    if (dloadVerInfo->dloadver == NULL) {
        CmdErrProc(clientId, ERR_MSP_FAILURE, 0, NULL);
        return AT_ERROR;
    }

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "\r%s\r", dloadVerInfo->dloadver);

    return AT_OK;
}


VOS_UINT32 AT_SetSecureStatePara(VOS_UINT8 indexNum)
{
    VOS_INT iRst;

    /* ������� */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* �����Ϸ��Լ�� */
    if ((g_atParaIndex != 1) || (g_atParaList[0].paraLen == 0)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ���õ����ṩ�Ľӿ�ʵ�����ò��� */
    iRst = AT_EfuseIoctl(MDRV_EFUSE_IOCTL_CMD_SET_SECURESTATE, (VOS_INT)g_atParaList[0].paraValue, VOS_NULL_PTR, 0);

    /* ���ݵ���ӿڷ��صĽ���������ý�� */
    /* ������1ʱ�����ظ�����,����0ʱ���óɹ�,�����������ʧ�� */
    if (iRst == AT_EFUSE_REPEAT) {
        return AT_ERROR;
    } else if (iRst == AT_EFUSE_OK) {
        return AT_OK;
    } else {
        AT_WARN_LOG("AT_SetSecureStatePara : Set Secure state req failed.");
    }

    return AT_CME_UNKNOWN;
}


VOS_UINT32 AT_QrySecureStatePara(VOS_UINT8 indexNum)
{
    VOS_INT    iResult;
    VOS_UINT16 length;

    /* ������� */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    length = 0;

    /* ���õ����ṩ�Ľ�ڲ�ѯ */
    iResult = AT_EfuseIoctl(MDRV_EFUSE_IOCTL_CMD_GET_SECURESTATE, 0, VOS_NULL_PTR, 0);

    /* �����쳣��ѯ��� */
    /* ��ѯʧ�� */
    if (iResult < AT_SECURE_STATE_NOT_SET) {
        return AT_ERROR;
    }

    /* ��ѯ����쳣 */
    if (iResult > AT_SECURE_STATE_RMA) {
        return AT_CME_UNKNOWN;
    }

    /* ��ӡ��� */
    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName, iResult);

    g_atSendDataBuff.bufLen = length;
    return AT_OK;
}


VOS_UINT32 AT_SetSecDbgStatePara(VOS_UINT8 indexNum)
{
    VOS_INT32 result;

    if ((g_atParaIndex != 1) || (g_atParaList[0].paraLen == 0)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ��ȫ״̬�Ѿ���Fastboot�����ã�����check�£��Ա�֤��������OK */
    result = AT_EfuseIoctl(MDRV_EFUSE_IOCTL_CMD_GET_SECURESTATE, 0, NULL, 0);

    if (result != AT_DRV_STATE_SECURE) {
        AT_WARN_LOG("AT_SetSecDbgStatePara: chip don't set to SECURE state.");
        return AT_ERROR;
    }

    if (AT_SetSecDbgStateVal() != AT_OK) {
        return AT_ERROR;
    }

    return AT_OK;
}


VOS_UINT32 AT_QrySecDbgStatePara(VOS_UINT8 indexNum)
{
    VOS_INT32 secureDebugVal;

    secureDebugVal = AT_EfuseIoctl(MDRV_EFUSE_IOCTL_CMD_GET_SECUREDEBUG, 0, NULL, 0);

    if (secureDebugVal < MDRV_OK) {
        AT_WARN_LOG("AT_QrySecDbgStatePara: get SECUREDEBUG error.");
        return AT_ERROR;
    }

    /* ��ӡ��� */
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName, secureDebugVal);

    return AT_OK;
}


VOS_UINT32 AT_QrySocidPara(VOS_UINT8 indexNum)
{
    VOS_INT    iResult;
    VOS_UINT16 length;
    VOS_UINT8  socid[AT_DRV_SOCID_LEN];
    VOS_UINT32 i;

    /* �ֲ�������ʼ�� */
    memset_s(socid, sizeof(socid), 0x00, AT_DRV_SOCID_LEN);
    length = 0;

    /* ������� */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ���õ����ṩ�Ľ�ڲ�ѯ */

    iResult = AT_EfuseIoctl(MDRV_EFUSE_IOCTL_CMD_GET_SOCID, 0, socid, (VOS_INT)(AT_DRV_SOCID_LEN / sizeof(VOS_UINT32)));

    /* �����쳣��ѯ��� */
    if (iResult != AT_EFUSE_OK) {
        return AT_ERROR;
    }

    /* ������ */
    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: ", g_parseContext[indexNum].cmdElement->cmdName);

    /* ��16����������� */
    for (i = 0; i < AT_DRV_SOCID_LEN; i++) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%02X", socid[i]);
    }

    g_atSendDataBuff.bufLen = length;
    return AT_OK;
}


VOS_UINT32 At_SetCbcPara(VOS_UINT8 indexNum)
{
    if (AT_IsMiscSupport(BSP_MODULE_TYPE_CHARGE) == BSP_MODULE_SUPPORT) {
        /* E5 ֱ�ӵ��������Ľӿ�ʵ�� */
        VOS_UINT8 bcs;
        VOS_UINT8 bcl;

        /* ������� */
        if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_CMD_NO_PARA) {
            return AT_CME_INCORRECT_PARAMETERS;
        }

        if (AT_GetMiscCbcState(&bcs, &bcl) != VOS_OK) {
            return AT_ERROR;
        }

        g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "%s: %d,%d", g_parseContext[indexNum].cmdElement->cmdName, bcs, bcl);
        return AT_OK;

    } else {
        /* ������� */
        if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_CMD_NO_PARA) {
            return AT_CME_INCORRECT_PARAMETERS;
        }

        /* ִ��������� */
        if (TAF_MMA_QryBatteryCapacityReq(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0) == VOS_TRUE) {
            /* ���õ�ǰ�������� */
            g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CBC_SET;
            return AT_WAIT_ASYNC_RETURN; /* ������������״̬ */
        } else {
            return AT_ERROR;
        }
    }
}


VOS_UINT32 AT_QryChrgInfoPara(VOS_UINT8 indexNum)
{
    VOS_INT32 chargeState;

    if (AT_IsMiscSupport(BSP_MODULE_TYPE_CHARGE) == BSP_MODULE_UNSUPPORT) {
        return AT_ERROR;
    }

    /* ���������ӿڻ�ȡ���״̬ */
    chargeState = AT_GetMiscChargingStatus();

    /* ��ӡ��� */
    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)(g_atSndCodeAddress), "%s:%d", g_parseContext[indexNum].cmdElement->cmdName, chargeState);

    return AT_OK;
}

// *****************************************************************************
// ��������: atSetReset
// ��������: ������������ "^RESET"
// ����˵��:
//   ulIndex [in] �û�����
// �� �� ֵ:
//    TODO: ...
// ����Ҫ��: TODO: ...
// ���þ���: TODO: ...

//  2.��    ��   : 2015��01��26��

// *****************************************************************************
VOS_UINT32 atSetReset(VOS_UINT8 clientId)
{
    g_atSendDataBuff.bufLen = 0;

    At_FormatResultData(clientId, AT_OK);

    VOS_TaskDelay(300); /* wait 300 tick */

    AT_SysbootRestart();

    return AT_SUCCESS;
}

/* NV�ָ����� */
VOS_UINT32 atSetNVRestore(VOS_UINT8 clientId)
{
    VOS_UINT32 rst;
    g_atSendDataBuff.bufLen = 0;

    rst = TAF_ACORE_NV_UPGRADE_RESTORE();
    if (rst != ERR_MSP_SUCCESS) {
        CmdErrProc(clientId, rst, 0, NULL);
        return AT_ERROR;
    }
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "\r%d\r", rst);

    return AT_OK;
}


VOS_UINT32 At_ConvertSDErrToAtErrCode(VOS_UINT32 sDOprtErr)
{
    VOS_UINT32 atSDErrCode;

    switch (sDOprtErr) {
        case AT_SD_ERR_CRAD_NOT_EXIST:
            atSDErrCode = AT_SD_CARD_NOT_EXIST;
            break;

        case AT_SD_ERR_INIT_FAILED:
            atSDErrCode = AT_SD_CARD_INIT_FAILED;
            break;

        case AT_SD_ERR_OPRT_NOT_ALLOWED:
            atSDErrCode = AT_SD_CARD_OPRT_NOT_ALLOWED;
            break;

        case AT_SD_ERR_ADDR_ERR:
            atSDErrCode = AT_SD_CARD_ADDR_ERR;
            break;

        default:
            atSDErrCode = AT_SD_CARD_OTHER_ERR;
            break;
    }

    return atSDErrCode;
}


VOS_BOOL At_IsArrayContentValueEquToPara(VOS_UINT8 value, VOS_UINT32 arrLen, VOS_UINT8 aucContent[])
{
    VOS_UINT32 i;

    for (i = 0; i < arrLen; i++) {
        if (value != aucContent[i]) {
            break;
        }
    }
    if (i != arrLen) {
        return VOS_FALSE;
    } else {
        return VOS_TRUE;
    }
}


VOS_UINT32 At_SDOprtRead(VOS_UINT8 indexNum)
{
    VOS_UINT32 oprtErr;
    VOS_UINT32 oprtRslt;
    VOS_UINT32 addr;
    VOS_UINT8 *sdData = VOS_NULL_PTR;
    VOS_UINT16 length;
    VOS_BOOL   bSameContent;
    VOS_UINT8  printData;

    if (g_atParaIndex != AT_SD_READ_PARA_VALID_NUM) {
        return AT_SD_CARD_OTHER_ERR;
    }

    /* �������ڱ���ָ����ַ��ʼ��512���ֽڵ��ڴ� */
    sdData = (VOS_UINT8 *)PS_MEM_ALLOC(WUEPS_PID_AT, AT_SD_DATA_UNIT_LEN);
    if (sdData == VOS_NULL_PTR) {
        AT_ERR_LOG("At_SD_Read: Fail to Alloc memory.");
        return AT_SD_CARD_OTHER_ERR;
    }
    (VOS_VOID)memset_s(sdData, AT_SD_DATA_UNIT_LEN, 0, AT_SD_DATA_UNIT_LEN);

    addr = g_atParaList[1].paraValue;

    oprtRslt = AT_SdProcess(SD_MMC_OPRT_READ, addr, 0, sdData, &oprtErr);

    printData = AT_SD_DATA_BYTE_CONTENT_IS_HEX_00;

    if (oprtRslt == VOS_OK) {
        oprtErr = AT_DEVICE_ERROR_BEGIN;

        /* ָ����ַ��ʼ��512���ֽ������Ƿ���ͬ */
        bSameContent = At_IsArrayContentValueEquToPara(sdData[0], AT_SD_DATA_UNIT_LEN, sdData);

        if (bSameContent == VOS_TRUE) {
            switch (sdData[0]) {
                case 0x00:
                    printData = AT_SD_DATA_BYTE_CONTENT_IS_HEX_00;
                    break;
                case 0x55:
                    printData = AT_SD_DATA_BYTE_CONTENT_IS_HEX_55;
                    break;
                case 0xAA:
                    printData = AT_SD_DATA_BYTE_CONTENT_IS_HEX_AA;
                    break;
                case 0xFF:
                    printData = AT_SD_DATA_BYTE_CONTENT_IS_HEX_FF;
                    break;
                default:
                    oprtErr = AT_SD_CARD_OTHER_ERR;
                    break;
            }
        } else {
            oprtErr = AT_SD_CARD_OTHER_ERR;
        }

        PS_MEM_FREE(WUEPS_PID_AT, sdData);
        sdData = VOS_NULL_PTR;

        if (oprtErr == AT_DEVICE_ERROR_BEGIN) {
            /* �������أ���ӡ��Ӧ����ʾ��ʽ */
            length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName, printData);

            g_atSendDataBuff.bufLen = length;
            return AT_OK;
        } else {
            /* ת��Ϊ��Ӧ�������ӡ */
            return At_ConvertSDErrToAtErrCode(oprtErr);
        }

    } else {
        PS_MEM_FREE(WUEPS_PID_AT, sdData);
        sdData = VOS_NULL_PTR;
        return At_ConvertSDErrToAtErrCode(oprtErr);
    }
}

VOS_UINT32 At_SDOprtWrtEraseFmt(TAF_UINT8 indexNum)
{
    VOS_UINT32 oprtErr;
    VOS_UINT32 oprtRslt;
    VOS_UINT32 addr;
    VOS_UINT32 data;

    switch (g_atParaList[0].paraValue) {
        case AT_SD_OPRT_FORMAT:
            /* ��SD�����и�ʽ������ */
            oprtRslt = AT_SdProcess(SD_MMC_OPRT_FORMAT, 0, 0, VOS_NULL_PTR, &oprtErr);
            break;

        case AT_SD_OPRT_ERASE:
            /* ֻ��һ������ʱ����������SD��; �������ʱ��ɾ��ָ����ַ��SD������  */
            if (g_atParaIndex == 1) {
                oprtRslt = AT_SdProcess(SD_MMC_OPRT_ERASE_ALL, 0, 0, VOS_NULL_PTR, &oprtErr);
            } else {
                addr     = g_atParaList[1].paraValue;
                oprtRslt = AT_SdProcess(SD_MMC_OPRT_ERASE_SPEC_ADDR, addr, 0, VOS_NULL_PTR, &oprtErr);
            }
            break;

        case AT_SD_OPRT_WRITE:
            /* ��SD����ָ����ַ����д���� */
            if (g_atParaIndex == AT_SD_WRITE_PARA_VALID_NUM) {
                addr     = g_atParaList[1].paraValue;
                data     = g_atParaList[AT_SD_DATA].paraValue;
                oprtRslt = AT_SdProcess(SD_MMC_OPRT_WRITE, addr, data, VOS_NULL_PTR, &oprtErr);
            } else {
                return AT_SD_CARD_OTHER_ERR;
            }
            break;

        default:
            return AT_SD_CARD_OPRT_NOT_ALLOWED;
    }

    if (oprtRslt != VOS_OK) {
        return At_ConvertSDErrToAtErrCode(oprtErr);
    }

    return AT_OK;
}


VOS_UINT32 AT_CheckSDParam()
{
    VOS_UINT32 i;
    TAF_UINT32 ret;

    /* ÿ�����������ֵ, �ò�������ʱ���صĴ�����*/
    VOS_UINT32 paramChecklist[AT_SD_PARAM_MAX_NUM][2] = {
        { AT_SD_MAX_OPR_NUM,        AT_SD_CARD_OPRT_NOT_ALLOWED },
        { AT_SD_MAX_ADDRESS_NUM,    AT_SD_CARD_ADDR_ERR },
        { AT_SD_MAX_DATA_NUM,       AT_SD_CARD_OTHER_ERR }};

    /* �������� */
    if (g_atParaIndex > AT_SD_PARAM_MAX_NUM) {
        return AT_SD_CARD_OTHER_ERR;
    }

    for (i = 0; i < g_atParaIndex; i++) {

        /* ����������� */
        ret = At_CheckNumString(g_atParaList[i].para, g_atParaList[i].paraLen);
        if (ret != AT_SUCCESS) {
            return AT_SD_CARD_OTHER_ERR;
        }

        /* ת��g_atParaList[0].ParaValue */
        ret = At_Auc2ul(g_atParaList[i].para, g_atParaList[i].paraLen, &g_atParaList[i].paraValue);
        if (ret != AT_SUCCESS) {
            return AT_SD_CARD_OTHER_ERR;
        }

        if (g_atParaList[i].paraValue > paramChecklist[i][0]) {
            return paramChecklist[i][1];
        }
    }
    return AT_SUCCESS;
}


VOS_UINT32 At_SetSD(VOS_UINT8 indexNum)
{
    VOS_UINT32 currentOprtStaus;
    VOS_UINT32 ret;
    VOS_INT32  currentSDStaus;

    /* ���������޲���ʱ��AT^SDΪ��ѯSD����λ״̬ */
    if (g_atParseCmd.cmdOptType == AT_CMD_OPT_SET_CMD_NO_PARA) {
        currentSDStaus = AT_GetSdStatus();
        if (currentSDStaus == AT_SD_STATUS_EXIST) {
            return AT_OK;
        } else {
            return AT_ERROR;
        }
    }

    ret = AT_CheckSDParam();
    if (ret != AT_SUCCESS) {
        return ret;
    }

    /* SD������Ϊ���з�ʽ����ѯ��ǰ�Ƿ��н��еĲ�������ȷ���Ƿ�ɽ����µ����ò��� */
    currentOprtStaus = AT_GetSdOpptStatus();

    /* �ϴεĲ�����δ���� ���������²��� */
    if (currentOprtStaus == AT_SD_OPRT_RSLT_NOT_FINISH) {
        return AT_SD_CARD_OTHER_ERR;
    }

    /*  ��ǰ��ִ��״̬���ɽ����µĶ���д����ʽ������ */
    if (g_atParaList[0].paraValue == AT_SD_OPRT_READ) {
        return At_SDOprtRead(indexNum);
    } else {
        return At_SDOprtWrtEraseFmt(indexNum);
    }
}


VOS_UINT32 At_QrySD(VOS_UINT8 indexNum)
{
    VOS_UINT32 currentOprtStaus;

    currentOprtStaus = AT_GetSdOpptStatus();

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName, currentOprtStaus);
    return AT_OK;
}

#if (FEATURE_AGPS == FEATURE_ON)
#if (FEATURE_AGPS_GPIO == FEATURE_ON)

gps_rat_mode_enum AT_SysModeToGpsRatMode(TAF_SysModeUint8 sysMode)
{
    gps_rat_mode_enum ratMode;

    ratMode = gps_rat_mode_butt;

    switch (sysMode) {
        case TAF_PH_INFO_GSM_RAT:
            ratMode = gps_rat_mode_gsm;
            break;

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
        case TAF_PH_INFO_CDMA_1X_RAT:
        case TAF_PH_INFO_HRPD_RAT:
        case TAF_PH_INFO_HYBRID_RAT:
            ratMode = gps_rat_mode_cdma;
            break;
#endif

        case TAF_PH_INFO_WCDMA_RAT:
            ratMode = gps_rat_mode_wcdma;
            break;

        case TAF_PH_INFO_TD_SCDMA_RAT:
            ratMode = gps_rat_mode_tdscdma;
            break;

        case TAF_PH_INFO_LTE_RAT:
        case TAF_PH_INFO_SVLTE_SRLTE_RAT:
            ratMode = gps_rat_mode_lte;
            break;

        default:
            ratMode = gps_rat_mode_butt;
            break;
    }

    return ratMode;
}


gps_rat_mode_enum AT_MtaAtRatModeToGpsRatMode(MTA_AT_RatModeUint8 mtaAtRatMode)
{
    gps_rat_mode_enum ratMode;

    ratMode = gps_rat_mode_butt;

    switch (mtaAtRatMode) {
        case MTA_AT_RAT_MODE_GSM:
            ratMode = gps_rat_mode_gsm;
            break;

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
        case MTA_AT_RAT_MODE_CDMA_1X:
        case MTA_AT_RAT_MODE_HRPD:
            ratMode = gps_rat_mode_cdma;
            break;
#endif

        case MTA_AT_RAT_MODE_WCDMA:
            ratMode = gps_rat_mode_wcdma;
            break;

        case MTA_AT_RAT_MODE_TDSCDMA:
            ratMode = gps_rat_mode_tdscdma;
            break;

        case MTA_AT_RAT_MODE_LTE:
            ratMode = gps_rat_mode_lte;
            break;

        default:
            ratMode = gps_rat_mode_butt;
            break;
    }

    return ratMode;
}
#endif /* #if (FEATURE_AGPS_GPIO == FEATURE_ON) */


VOS_INT AT_SetCgpsClock(VOS_BOOL bEnableFlg, MTA_AT_ModemIdUint8 modemId, MTA_AT_RatModeUint8 sysMode)
{
#if (FEATURE_AGPS_GPIO == FEATURE_ON)
    gps_modem_id_enum gpsModemId;
    gps_rat_mode_enum ratMode;

    switch (modemId) {
        case MTA_AT_MODEM_ID_0:
            gpsModemId = gps_modem_id_0;
            break;

        case MTA_AT_MODEM_ID_1:
            gpsModemId = gps_modem_id_1;
            break;

        case MTA_AT_MODEM_ID_2:
            gpsModemId = gps_modem_id_2;
            break;

        default:
            gpsModemId = gps_modem_id_0;
            break;
    }

    ratMode = AT_MtaAtRatModeToGpsRatMode(sysMode);

    return set_gps_ref_clk_enable((bool)bEnableFlg, gpsModemId, ratMode);
#else
    AT_ERR_LOG("AT_SetCgpsClock: MBB not support ^CGPSCLOCK!");
    return VOS_ERR;
#endif
}


VOS_INT AT_CfgGpsRefClk(VOS_BOOL bEnableFlg, ModemIdUint16 modemId, TAF_SysModeUint8 sysMode)
{
#if (FEATURE_AGPS_GPIO == FEATURE_ON)
    gps_modem_id_enum gpsModemId;
    gps_rat_mode_enum ratMode;

    switch (modemId) {
        case MODEM_ID_0:
            gpsModemId = gps_modem_id_0;
            break;

        case MODEM_ID_1:
            gpsModemId = gps_modem_id_1;
            break;

        case MODEM_ID_2:
            gpsModemId = gps_modem_id_2;
            break;

        default:
            gpsModemId = gps_modem_id_0;
            break;
    }

    ratMode = AT_SysModeToGpsRatMode(sysMode);

    return set_gps_ref_clk_enable((bool)bEnableFlg, gpsModemId, ratMode);
#else
    AT_ERR_LOG("AT_CfgGpsRefClk: MBB not support ^CGPSCLOCK!");
    return VOS_ERR;
#endif
}


TAF_NV_GpsChipTypeUint8 At_GetGpsTypeFromNv(VOS_UINT32 modemId)
{
    TAF_NVIM_GpsCustCfg nvimGpsCustCfg;
    VOS_UINT32          ret;

    memset_s(&nvimGpsCustCfg, sizeof(nvimGpsCustCfg), 0x00, sizeof(TAF_NVIM_GpsCustCfg));

    ret = TAF_ACORE_NV_READ(modemId, NV_ITEM_GPS_CUST_CFG, &nvimGpsCustCfg, sizeof(nvimGpsCustCfg));
    if (ret != NV_OK) {
        AT_WARN_LOG("At_GetGpsTypeFromNv(): Read NV_ITEM_GPS_CUST_CFG failed! ");
        return TAF_NV_GPS_CHIP_BUTT;
    }

    if (nvimGpsCustCfg.gpsChipType >= TAF_NV_GPS_CHIP_BUTT) {
        AT_WARN_LOG("At_GetGpsTypeFromNv():WARNING: NV parameter Error!");
        return TAF_NV_GPS_CHIP_BUTT;
    }

    return nvimGpsCustCfg.gpsChipType;
}


VOS_UINT32 AT_SetCgpsClockForBroadcom(VOS_UINT8 indexNum, ModemIdUint16 modemId)
{
    TAF_AGENT_SysMode sysMode;
    VOS_BOOL          bEnableflg;

    bEnableflg = VOS_FALSE;

    memset_s(&sysMode, sizeof(sysMode), 0x00, sizeof(sysMode));

    if (TAF_AGENT_GetSysMode(g_atClientTab[indexNum].clientId, &sysMode) != VOS_OK) {
        sysMode.ratType    = TAF_PH_INFO_NONE_RAT;
        sysMode.sysSubMode = TAF_SYS_SUBMODE_BUTT;
    }

#if (FEATURE_UE_MODE_CDMA != FEATURE_ON)
    /* cdma��δ��ʱ����ATģ������cdmaģ��������gpsģ���·�butt��������ֹ���� */
    if ((sysMode.ratType == TAF_PH_INFO_CDMA_1X_RAT) || (sysMode.ratType == TAF_PH_INFO_HRPD_RAT) ||
        (sysMode.ratType == TAF_PH_INFO_HYBRID_RAT)) {
        AT_ERR_LOG("AT_SetCgpsClockForBroadcom: rat invalid!");
        return AT_CME_INCORRECT_PARAMETERS;
    }
#endif

    if (g_atParaList[0].paraValue == 1) {
        bEnableflg = VOS_TRUE;
    } else if (g_atParaList[0].paraValue == 0) {
        bEnableflg = VOS_FALSE;
    } else {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (AT_CfgGpsRefClk(bEnableflg, modemId, sysMode.ratType) != VOS_OK) {
        AT_ERR_LOG("AT_SetCgpsClockPara: AT_CfgGpsRefClk failed!");
        return AT_ERROR;
    }

    return AT_OK;
}


VOS_UINT32 AT_SetCgpsClockForHisi1102(VOS_UINT8 indexNum)
{
    VOS_UINT32 ret;

    ret = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, 0, ID_AT_MTA_CGPSCLOCK_SET_REQ, VOS_NULL_PTR, 0,
                                 I0_UEPS_PID_MTA);
    if (ret != TAF_SUCCESS) {
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CGPSCLOCK_SET;

    return AT_WAIT_ASYNC_RETURN;
}

#endif /* #if (FEATURE_AGPS == FEATURE_ON) */


VOS_UINT32 AT_SetCgpsClockPara(VOS_UINT8 indexNum)
{
#if (FEATURE_AGPS == FEATURE_ON)
    VOS_UINT32              ret;
    ModemIdUint16           modemId;
    TAF_NV_GpsChipTypeUint8 gpsChipType;
    MTA_AT_RatModeUint8     sysMode;

    modemId     = MODEM_ID_BUTT;
    gpsChipType = TAF_NV_GPS_CHIP_BUTT;
    ret         = AT_ERROR;

    /* ������� */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ����������� */
    if ((g_atParaIndex != 1) && (g_atParaIndex != 2)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (AT_GetModemIdFromClient(g_atClientTab[indexNum].clientId, &modemId) != VOS_OK) {
        modemId = MODEM_ID_0;
    }

    if (g_atParaIndex == 1) {
        gpsChipType = At_GetGpsTypeFromNv(modemId);
        if (gpsChipType == TAF_NV_GPS_CHIP_HISI1102) {
            AT_SetCgpsCLockEnableFlgByModemId(modemId, (VOS_UINT8)g_atParaList[0].paraValue);
            ret = AT_SetCgpsClockForHisi1102(indexNum);
        } else {
            ret = AT_SetCgpsClockForBroadcom(indexNum, modemId);
        }
    } else {
        if ((g_atParaList[0].paraLen == 0) || (g_atParaList[1].paraLen == 0)) {
            return AT_CME_INCORRECT_PARAMETERS;
        }
        sysMode = (MTA_AT_RatModeUint8)g_atParaList[1].paraValue;

#if (FEATURE_UE_MODE_CDMA != FEATURE_ON)
        /* cdma��δ��ʱ����ATģ������cdmaģ��������gpsģ���·�butt��������ֹ���� */
        if ((sysMode == MTA_AT_RAT_MODE_CDMA_1X) || (sysMode == MTA_AT_RAT_MODE_HRPD)) {
            AT_ERR_LOG("AT_SetCgpsClockPara: rat invalid!");
            return AT_CME_INCORRECT_PARAMETERS;
        }
#endif
        if (AT_SetCgpsClock((VOS_BOOL)g_atParaList[0].paraValue, (MTA_AT_ModemIdUint8)modemId, sysMode) != VOS_OK) {
            ret = AT_ERROR;
        } else {
            ret = AT_OK;
        }
    }
    return ret;
#else
    AT_ERR_LOG("AT_SetCgpsClockPara: AGPS feature not support!");
    return AT_ERROR;
#endif
}




VOS_UINT32 At_SetSecuBootPara(VOS_UINT8 indexNum)
{
    if ((g_atParaIndex != 1) || (g_atParaList[0].paraLen == 0)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaList[0].paraValue > AT_DX_SECURE_STATE) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    switch (g_atParaList[0].paraValue) {
        case AT_NO_DX_SECU_ENABLE_STATE: {
            if (AT_CryptoStartSecure() != MDRV_OK) {
                AT_WARN_LOG("At_SetSecuBootPara: AT_CryptoStartSecure() failed");
                return AT_ERROR;
            }
            break;
        }
        case AT_DX_RMA_STATE: {
            /* �Ȼ�ȡ��оƬ״̬������Ѿ�����ΪRMA״̬��ֱ�ӷ���ok */
            if (AT_EfuseIoctl(MDRV_EFUSE_IOCTL_CMD_GET_SECURESTATE, 0, NULL, 0) == AT_DRV_STATE_RMA) {
                AT_WARN_LOG("At_SetSecuBootPara: chip is already set to RMA state");
                return AT_OK;
            }

            if (AT_EfuseIoctl(MDRV_EFUSE_IOCTL_CMD_SET_SECURESTATE, AT_SET_RMA_STATE, NULL, 0) != MDRV_OK) {
                AT_WARN_LOG("At_SetSecuBootPara: SET_SECURE_DISABLED_STATE  failed");
                return AT_ERROR;
            }
            break;
        }
        case AT_DX_SECURE_STATE: {
            /* ��ȫ״̬�Ѿ���Fastboot�����ã�����check�£��Ա�֤��������OK */
            if (AT_EfuseIoctl(MDRV_EFUSE_IOCTL_CMD_GET_SECURESTATE, 0, NULL, 0) != AT_DRV_STATE_SECURE) {
                AT_WARN_LOG("At_SetSecuBootPara: chip don't set to SECURE state");
                return AT_ERROR;
            }
            if (AT_SetSecDbgStateVal() != AT_OK) {
                return AT_ERROR;
            }
            break;
        }
        default: {
            return AT_CME_INCORRECT_PARAMETERS;
        }
    }

    return AT_OK;
}


VOS_UINT32 At_QrySecuBootPara(VOS_UINT8 indexNum)
{
    VOS_UINT8 secBootStartedFlag = 0;
#if (FEATURE_DX_SECBOOT == FEATURE_ON)
    VOS_INT32 dXSecBootFlag;
#endif

    if (AT_IsCryptoSecStarted(&secBootStartedFlag) != MDRV_OK) {
        AT_WARN_LOG("At_QrySecuBootPara: get mdrv_crypto_sec_started() failed");
        return AT_ERROR;
    }

/* ����֧��DX��ȫ����ģ���Ҫ��ѯоƬ״̬ */
#if (FEATURE_DX_SECBOOT == FEATURE_ON)
    dXSecBootFlag = AT_EfuseIoctl(MDRV_EFUSE_IOCTL_CMD_GET_SECURESTATE, 0, NULL, 0);
    if (dXSecBootFlag < 0) {
        AT_WARN_LOG("At_QrySecuBootPara: MDRV_EFUSE_IOCTL_CMD_GET_SECURESTATE failed");
        return AT_ERROR;
    }
    if (dXSecBootFlag == AT_DRV_STATE_RMA) {
        secBootStartedFlag = AT_DX_RMA_STATE;
    } else if (dXSecBootFlag == AT_DRV_STATE_SECURE) {
        secBootStartedFlag = AT_DX_SECURE_STATE;
    } else {
        secBootStartedFlag = AT_NOT_SET_STATE;
    }
#endif

    /* ��ӡ��� */
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName, secBootStartedFlag);

    return AT_OK;
}

VOS_UINT32 At_QrySecuBootFeaturePara(VOS_UINT8 indexNum)
{
    VOS_UINT8 secBootSupportedFlag;
    secBootSupportedFlag = 0;

    /* ���õ���ӿ� */
    if (AT_IsCryptoSecbootSupported(&secBootSupportedFlag) != MDRV_OK) {
        AT_WARN_LOG("At_QrySecuBootFeaturePara: get AT_IsCryptoSecbootSupported() failed");
        return AT_ERROR;
    }

#if (FEATURE_DX_SECBOOT == FEATURE_ON)
    if (secBootSupportedFlag == 0) {
        secBootSupportedFlag = AT_DX_RMA_STATE;
    } else {
        secBootSupportedFlag = AT_DX_SECURE_STATE;
    }
#endif

    /* ��ӡ��� */
    g_atSendDataBuff.bufLen =
        (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            secBootSupportedFlag);

    return AT_OK;
}


VOS_UINT32 AT_SetChrgEnablePara(VOS_UINT8 indexNum)
{
    VOS_UINT32 ret;

    ret = AT_IsMiscSupport(BSP_MODULE_TYPE_CHARGE);

    if (ret == BSP_MODULE_UNSUPPORT) {
        return AT_ERROR;
    }

    /* �������� */
    if (g_atParaIndex > 1) {
        return AT_TOO_MANY_PARA;
    }

    /* �������ȹ��� */
    if (g_atParaList[0].paraLen != 1) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if ((g_atParaList[0].paraValue == 0) || (g_atParaList[0].paraValue == 1)) {
        /* ���������ӿ�ʹ�ܻ�ȥʹ�ܳ�� */
        AT_SetMiscChargeState((unsigned long)g_atParaList[0].paraValue);
    } else {
        /* ���ò���ӿ� */
        AT_SplyMiscBattery();
    }

    return AT_OK;
}


VOS_UINT32 AT_QryChrgEnablePara(VOS_UINT8 indexNum)
{
    VOS_INT32 chargeEnable;

    if (AT_IsMiscSupport(BSP_MODULE_TYPE_CHARGE) == BSP_MODULE_UNSUPPORT) {
        return AT_ERROR;
    }

    /* ���������ӿڻ�ȡ���״̬ */

    chargeEnable = AT_GetMiscChargeState();
    /* ��ӡ��� */
    /* ֻ��TRUE/FLASE�ķ���ֵ����Ч�� */
    if ((chargeEnable == TRUE) || (chargeEnable == FALSE)) {
        g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName, chargeEnable);

        return AT_OK;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_TestChrgEnablePara(VOS_UINT8 indexNum)
{
    VOS_INT32 chargeEnable;

    chargeEnable = AT_GetMiscChargeState(); /* BSP_TBAT_CHRStGet()) */
    /* ֻ��TRUE/FLASE�ķ���ֵ����Ч�� */
    if ((chargeEnable == TRUE) || (chargeEnable == FALSE)) {
        g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "%s:%d", "^TCHRENABLE", chargeEnable);
        return AT_OK;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_SetWifiPaRangePara(VOS_UINT8 indexNum)
{
    AT_WifiModeUint8 wifiMode;

    /* ��ʼ�� */
    wifiMode = (VOS_UINT8)AT_GetWifiPAMode();

    if (AT_IsMiscSupport(BSP_MODULE_TYPE_WIFI) == BSP_MODULE_UNSUPPORT) {
        return AT_ERROR;
    }

    /* �������ȹ��� */
    if (g_atParaList[0].paraLen != 1) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ������� */
    if (g_atParaList[0].para[0] == 'h') {
        if (wifiMode == AT_WIFI_MODE_ONLY_NOPA) {
            return AT_ERROR;
        }

        if (AT_SetWifiPaMode(AT_WIFI_MODE_ONLY_PA) != VOS_OK) {
            return AT_ERROR;
        }
    } else if (g_atParaList[0].para[0] == 'l') {
        if (wifiMode == AT_WIFI_MODE_ONLY_PA) {
            return AT_ERROR;
        }

        if (AT_SetWifiPaMode(AT_WIFI_MODE_ONLY_NOPA) != VOS_OK) {
            return AT_ERROR;
        }
    } else {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    return AT_OK;
}


VOS_UINT32 AT_QryWifiPaRangePara(VOS_UINT8 indexNum)
{
    AT_WifiModeUint8 curWifiMode;

    if (AT_IsMiscSupport(BSP_MODULE_TYPE_WIFI) == BSP_MODULE_UNSUPPORT) {
        return AT_ERROR;
    }

    /* ���õ����ṩ��ѯ�ӿڻ�ȡ��ǰWIFIģʽ */
    curWifiMode = (VOS_UINT8)AT_GetWifiPACurMode();

    if (curWifiMode == AT_WIFI_MODE_ONLY_PA) {
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s:%s", g_parseContext[indexNum].cmdElement->cmdName, "h");
    } else if (curWifiMode == AT_WIFI_MODE_ONLY_NOPA) {
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s:%s", g_parseContext[indexNum].cmdElement->cmdName, "l");
    } else {
        return AT_ERROR;
    }

    return AT_OK;
}


VOS_UINT32 AT_TestWifiPaRangePara(VOS_UINT8 indexNum)
{
    AT_WifiModeUint8 wifiMode;

    if (AT_IsMiscSupport(BSP_MODULE_TYPE_WIFI) == BSP_MODULE_UNSUPPORT) {
        return AT_ERROR;
    }

    /* ��ʼ�� */
    wifiMode = (VOS_UINT8)AT_GetWifiPAMode();

    /* ��ѯ����֧�ֵ�ģʽ�����ֻ֧��PAģʽ��û��NO PAģʽ����ֻ����h���ɣ��������ģʽ��֧�֣��򷵻�h,l�� */
    if (wifiMode == AT_WIFI_MODE_ONLY_PA) {
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s:%s", g_parseContext[indexNum].cmdElement->cmdName, "h");
    } else if (wifiMode == AT_WIFI_MODE_ONLY_NOPA) {
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s:%s", g_parseContext[indexNum].cmdElement->cmdName, "l");
    } else if (wifiMode == AT_WIFI_MODE_PA_NOPA) {
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s:%s", g_parseContext[indexNum].cmdElement->cmdName, "h,l");
    } else {
        return AT_ERROR;
    }

    return AT_OK;
}


VOS_VOID AT_PrintWifiChannelInfo(AT_WIFI_Info *wifiInfo, VOS_UINT8 indexNum)
{
    VOS_UINT32 loopIndex;
    VOS_UINT16 length;

    /* ��ʼ�� */
    loopIndex = 0;
    length    = g_atSendDataBuff.bufLen;

    /* ����֧��802.11b��ʽ, ���802.11b��ʽ֧�ֵ��ŵ��� */
    if (wifiInfo->opbSupport == VOS_TRUE) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s:%s", g_parseContext[indexNum].cmdElement->cmdName, "b");

        wifiInfo->bLowChannel  = AT_MIN(wifiInfo->bLowChannel, AT_WIFI_CHANNEL_MAX_NUM);
        wifiInfo->bHighChannel = AT_MIN(wifiInfo->bHighChannel, AT_WIFI_CHANNEL_MAX_NUM);
        for (loopIndex = wifiInfo->bLowChannel; loopIndex <= wifiInfo->bHighChannel; loopIndex++) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", loopIndex);
        }
        if ((wifiInfo->opgSupport == VOS_TRUE) || (wifiInfo->opnSupport == VOS_TRUE)) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
        }
    }

    /* ����֧��802.11g��ʽ, ���802.11g��ʽ֧�ֵ��ŵ��� */
    if (wifiInfo->opgSupport == VOS_TRUE) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s:%s", g_parseContext[indexNum].cmdElement->cmdName, "g");

        wifiInfo->gLowChannel  = AT_MIN(wifiInfo->gLowChannel, AT_WIFI_CHANNEL_MAX_NUM);
        wifiInfo->gHighChannel = AT_MIN(wifiInfo->gHighChannel, AT_WIFI_CHANNEL_MAX_NUM);
        for (loopIndex = wifiInfo->gLowChannel; loopIndex <= wifiInfo->gHighChannel; loopIndex++) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", loopIndex);
        }
        if (wifiInfo->opnSupport == VOS_TRUE) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);
        }
    }

    /* ����֧��802.11n��ʽ, ���802.11n��ʽ֧�ֵ��ŵ��� */
    if (wifiInfo->opnSupport == VOS_TRUE) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s:%s", g_parseContext[indexNum].cmdElement->cmdName, "n");

        wifiInfo->nLowChannel  = AT_MIN(wifiInfo->nLowChannel, AT_WIFI_CHANNEL_MAX_NUM);
        wifiInfo->nHighChannel = AT_MIN(wifiInfo->nHighChannel, AT_WIFI_CHANNEL_MAX_NUM);
        for (loopIndex = wifiInfo->nLowChannel; loopIndex <= wifiInfo->nHighChannel; loopIndex++) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, ",%d", loopIndex);
        }
    }

    g_atSendDataBuff.bufLen += length;
    return;
}


VOS_VOID AT_PrintWifibPowerInfo(AT_WIFI_Info *wifiInfo, VOS_UINT8 wifiMode, VOS_UINT8 indexNum)
{
    /* ���802.11b��ʽ�������� */
    g_atSendDataBuff.bufLen += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen,
        "%s:%s", g_parseContext[indexNum].cmdElement->cmdName, "b");
    if ((wifiMode == AT_WIFI_MODE_ONLY_PA) || (wifiMode == AT_WIFI_MODE_PA_NOPA)) {
        g_atSendDataBuff.bufLen += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, ",%d", wifiInfo->bPower[0]);
    }
    if ((wifiMode == AT_WIFI_MODE_ONLY_NOPA) || (wifiMode == AT_WIFI_MODE_PA_NOPA)) {
        g_atSendDataBuff.bufLen += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, ",%d", wifiInfo->bPower[1]);
    }
}


VOS_VOID AT_PrintWifigPowerInfo(AT_WIFI_Info *wifiInfo, VOS_UINT8 wifiMode, VOS_UINT8 indexNum)
{
    /* ���802.11g��ʽ�������� */
    g_atSendDataBuff.bufLen += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen,
        "%s:%s", g_parseContext[indexNum].cmdElement->cmdName, "g");
    if ((wifiMode == AT_WIFI_MODE_ONLY_PA) || (wifiMode == AT_WIFI_MODE_PA_NOPA)) {
        g_atSendDataBuff.bufLen += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, ",%d", wifiInfo->gPower[0]);
    }
    if ((wifiMode == AT_WIFI_MODE_ONLY_NOPA) || (wifiMode == AT_WIFI_MODE_PA_NOPA)) {
        g_atSendDataBuff.bufLen += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, ",%d", wifiInfo->gPower[1]);
    }
}


VOS_VOID AT_PrintWifinPowerInfo(AT_WIFI_Info *wifiInfo, VOS_UINT8 wifiMode, VOS_UINT8 indexNum)
{
    /* ���802.11n��ʽ�������� */
    g_atSendDataBuff.bufLen += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen,
        "%s:%s", g_parseContext[indexNum].cmdElement->cmdName, "n");
    if ((wifiMode == AT_WIFI_MODE_ONLY_PA) || (wifiMode == AT_WIFI_MODE_PA_NOPA)) {
        g_atSendDataBuff.bufLen += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, ",%d", wifiInfo->nPower[0]);
    }
    if ((wifiMode == AT_WIFI_MODE_ONLY_NOPA) || (wifiMode == AT_WIFI_MODE_PA_NOPA)) {
        g_atSendDataBuff.bufLen += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, ",%d", wifiInfo->nPower[1]);
    }
}


VOS_VOID AT_PrintWifiPowerInfo(AT_WIFI_Info *wifiInfo, VOS_UINT8 indexNum)
{
    AT_WifiModeUint8 wifiMode;

    /* ��ʼ�� */
    wifiMode = (VOS_UINT8)AT_GetWifiPAMode();

    /* ����֧��802.11b��ʽ, ����������� */
    if (wifiInfo->opbSupport == VOS_TRUE) {
        AT_PrintWifibPowerInfo(wifiInfo, wifiMode, indexNum);
        if ((wifiInfo->opgSupport == VOS_TRUE) || (wifiInfo->opnSupport == VOS_TRUE)) {
            g_atSendDataBuff.bufLen += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN,
                (VOS_CHAR *)g_atSndCodeAddress, (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, "%s",
                g_atCrLf);
        }
    }

    /* ����֧��802.11g��ʽ, ����������� */
    if (wifiInfo->opgSupport == VOS_TRUE) {
        AT_PrintWifigPowerInfo(wifiInfo, wifiMode, indexNum);
        if (wifiInfo->opnSupport == VOS_TRUE) {
            g_atSendDataBuff.bufLen += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN,
                (VOS_CHAR *)g_atSndCodeAddress, (VOS_CHAR *)g_atSndCodeAddress + g_atSendDataBuff.bufLen, "%s",
                g_atCrLf);
        }
    }

    /* ����֧��802.11n��ʽ, ����������� */
    if (wifiInfo->opnSupport == VOS_TRUE) {
        AT_PrintWifinPowerInfo(wifiInfo, wifiMode, indexNum);
    }

    return;
}



VOS_UINT32 AT_SetWifiInfoPara(VOS_UINT8 indexNum)
{
#if (FEATURE_LTE == FEATURE_ON)
    VOS_UINT16      len;
    const VOS_CHAR *string = "1,2,3,4,5,6,7,8,9,10,11,12,13";

    len = 0;

    if (g_atParaIndex > AT_WIINFO_PARA_MAX_NUM) {
        return AT_ERROR;
    }

    if (g_atParaList[0].paraValue == 0) {
        /* 0,0 */
        if (g_atParaList[1].paraValue == 0) {
            len += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + len, "^WIINFO:b,%s%s^WIINFO:g,%s%s^WIINFO:n,%s", string, g_atCrLf,
                string, g_atCrLf, string);
        } else { /* 0,1 */
            len += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + len, "^WIINFO:n,%s", string);
        }
    } else {
        /* 1,0 */
        if (g_atParaList[1].paraValue == 0) {
            len += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + len, "^WIINFO:b,130%s^WIINFO:g,80%s^WIINFO:n,80", g_atCrLf, g_atCrLf);
        } else { /* 1,1 */
            len += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + len, "^WIINFO:n,80");
        }
    }
    g_atSendDataBuff.bufLen = len;

    return AT_OK;
#else

    AT_WIFI_Info wifiInfo = {0};

    if (AT_IsMiscSupport(BSP_MODULE_TYPE_WIFI) == BSP_MODULE_UNSUPPORT) {
        return AT_ERROR;
    }

    /* ��ʼ�� */
    g_atSendDataBuff.bufLen = 0;

    /* �������� */
    if (g_atParaIndex > 1) {
        return AT_ERROR;
    }

    /* �������ȹ��� */
    if (g_atParaList[0].paraLen != 1) {
        return AT_ERROR;
    }

    /* ��ȡWIFI INFO��Ӧ��NV�� */
    if (TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_WIFI_INFO, &wifiInfo, sizeof(AT_WIFI_Info)) != VOS_OK) {
        AT_WARN_LOG("AT_SetWifiInfoPara:READ NV ERROR!");
        return AT_ERROR;
    }

    /* ��ѯ֧�ֵ��ŵ��Ż��������� */
    if (g_atParaList[0].paraValue == 0) {
        /* �������ʽ֧�ֵ��ŵ��� */
        AT_PrintWifiChannelInfo(&wifiInfo, indexNum);
    } else {
        /* �������ʽ���������� */
        AT_PrintWifiPowerInfo(&wifiInfo, indexNum);
    }

    return AT_OK;
#endif
}


VOS_UINT32 AT_SetWiFiPacketPara(VOS_UINT8 indexNum)
{
    VOS_UINT *atUcastWifiRxPkts = VOS_NULL_PTR;

    if (AT_IsMiscSupport(BSP_MODULE_TYPE_WIFI) == BSP_MODULE_UNSUPPORT) {
        return AT_ERROR;
    }
    /* �������� */
    if (g_atParaIndex > 1) {
        return AT_TOO_MANY_PARA;
    }

    /* �������ȹ��� */
    if (g_atParaList[0].paraLen != 1) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* TODO: */
    /* Ŀǰ���ø÷�ʽ���� */
    atUcastWifiRxPkts = AT_GetUcastWifiRxPkts();
    AT_GetWifiRxPacketReport(atUcastWifiRxPkts, &g_mcastWifiRxPkts);

    return AT_OK;
}


VOS_UINT32 AT_QryWiFiPacketPara(VOS_UINT8 indexNum)
{
    VOS_UINT  *atUcastWifiRxPkts = VOS_NULL_PTR;
    VOS_UINT   ucastWifiRxPkts;
    VOS_UINT   mcastWifiRxPkts;
    VOS_UINT32 wifiRxPkts;

    atUcastWifiRxPkts = AT_GetUcastWifiRxPkts();

    if (AT_IsMiscSupport(BSP_MODULE_TYPE_WIFI) == BSP_MODULE_UNSUPPORT) {
        return AT_ERROR;
    }

    /* �жϽ��ջ��Ƿ�� */
    if (AT_GetWifiTcmdMode() != AT_WIFI_RX_MODE) {
        AT_WARN_LOG("AT_QryWiFiPacketPara: Not Rx Mode.");
        return AT_ERROR;
    }

    /* ���������ӿڲ�ѯ�������� */
    AT_GetWifiRxPacketReport(&ucastWifiRxPkts, &mcastWifiRxPkts);
    wifiRxPkts = (ucastWifiRxPkts - *atUcastWifiRxPkts);

    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s:%d,%d", g_parseContext[indexNum].cmdElement->cmdName, wifiRxPkts, 0);

    return AT_OK;
}


VOS_UINT32 AT_SetWiFiTxPara(VOS_UINT8 indexNum)
{
#if (FEATURE_LTE == FEATURE_ON)
    VOS_CHAR   acCmd[AT_ACCMD_ARRAY_MAX_LEN] = {0};
    VOS_UINT32 atWifiRF;
    VOS_UINT32 atWifiFreq;
    VOS_UINT32 atWifiMode;
#endif

    if (AT_IsMiscSupport(BSP_MODULE_TYPE_WIFI) == BSP_MODULE_UNSUPPORT) {
        return AT_ERROR;
    }

    /* �������� */
    if (g_atParaIndex > 1) {
        return AT_TOO_MANY_PARA;
    }
#if (FEATURE_LTE == FEATURE_ON)
    atWifiMode = AT_GetWifiMode();
    if (atWifiMode == AT_WIFI_SUPPORT_MODE_CW) {
        if (g_atParaList[0].paraValue == AT_WIFI_TX_OFF) {
            AT_WIFI_TEST_CMD_CALL("wl fqacurcy 0");
        } else {
            AT_WIFI_TEST_CMD_CALL("wl up");
            AT_WIFI_TEST_CMD_CALL("wl band b");
            AT_WIFI_TEST_CMD_CALL("wl out");
            AT_WIFI_TEST_CMD_CALL("wl phy_txpwrctrl 0");

            atWifiRF = AT_GetWifiRF();
            if (atWifiRF == 0) {
                AT_WIFI_TEST_CMD_CALL("wl phy_txpwrindex 0 127");
            } else {
                AT_WIFI_TEST_CMD_CALL("wl phy_txpwrindex 127 0");
            }

            AT_WIFI_TEST_CMD_CALL("wl phy_txpwrctrl 1");
            atWifiFreq = AT_GetWifiFreq();
            snprintf_s(acCmd, sizeof(acCmd), sizeof(acCmd) - 1, "wl fqacurcy %d", atWifiFreq);

            AT_WIFI_TEST_CMD_CALL(acCmd);
        }
    } else {
#endif
        /* ����Ϊ��رշ����������Ϊ1�򿪷���� */
        if (g_atParaList[0].paraValue == AT_WIFI_TX_OFF) {
            AT_WIFI_TEST_CMD_CALL("wl pkteng_stop tx");
        } else {
            /* ���õ���ӿ� */
            AT_WIFI_TEST_CMD_CALL("wl down");

            AT_WIFI_TEST_CMD_CALL("wl mpc 0");

            AT_WIFI_TEST_CMD_CALL("wl country ALL");

            AT_WIFI_TEST_CMD_CALL("wl frameburst 1");

            AT_WIFI_TEST_CMD_CALL("wl scansuppress 1");

            AT_WIFI_TEST_CMD_CALL("wl up");

            AT_WIFI_TEST_CMD_CALL("wl pkteng_start 00:11:22:33:44:55 tx 100 1500 0");

#if (FEATURE_LTE == FEATURE_ON)
            AT_WIFI_TEST_CMD_CALL("wl phy_forcecal 1");
#endif
        }
#if (FEATURE_LTE == FEATURE_ON)
    }
#endif
    return AT_OK;
}


VOS_UINT32 AT_SetWiFiRxPara(VOS_UINT8 indexNum)
{
    VOS_UINT *atUcastWifiRxPkts = VOS_NULL_PTR;
    VOS_CHAR  acCmd[AT_ACCMD_ARRAY_MAX_LEN]        = {0};
    VOS_INT32 bufLen;

    if (AT_IsMiscSupport(BSP_MODULE_TYPE_WIFI) == BSP_MODULE_UNSUPPORT) {
        return AT_ERROR;
    }

    /* �������� */
    if (g_atParaIndex > AT_WIRX_PARA_MAX_NUM) {
        return AT_TOO_MANY_PARA;
    }

    /* ����Ϊ��رս��ջ�������Ϊ1�򿪽��ջ� */
    if (g_atParaList[AT_WIRX_ONOFF].paraValue == AT_WIFI_RX_OFF) {
        /* ���������رս��ջ� */
        AT_WIFI_TEST_CMD_CALL("wl pkteng_stop rx");
    } else {
        AT_WIFI_TEST_CMD_CALL("wl down");

        AT_WIFI_TEST_CMD_CALL("wl mpc 0");

        AT_WIFI_TEST_CMD_CALL("wl country ALL");

        AT_WIFI_TEST_CMD_CALL("wl frameburst 1");

        AT_WIFI_TEST_CMD_CALL("wl scansuppress 1");

        AT_WIFI_TEST_CMD_CALL("wl up");

        bufLen = snprintf_s(acCmd, sizeof(acCmd), sizeof(acCmd) - 1,
                            "wl pkteng_start %c%c:%c%c:%c%c:%c%c:%c%c:%c%c rx async 0 0",
                            g_atParaList[AT_WIRX_SOURCE_MAC].para[AT_COMMAND_PARA_INDEX_0],
                            g_atParaList[AT_WIRX_SOURCE_MAC].para[AT_COMMAND_PARA_INDEX_1],
                            g_atParaList[AT_WIRX_SOURCE_MAC].para[AT_COMMAND_PARA_INDEX_2],
                            g_atParaList[AT_WIRX_SOURCE_MAC].para[AT_COMMAND_PARA_INDEX_3],
                            g_atParaList[AT_WIRX_SOURCE_MAC].para[AT_COMMAND_PARA_INDEX_4],
                            g_atParaList[AT_WIRX_SOURCE_MAC].para[AT_COMMAND_PARA_INDEX_5],
                            g_atParaList[AT_WIRX_SOURCE_MAC].para[AT_COMMAND_PARA_INDEX_6],
                            g_atParaList[AT_WIRX_SOURCE_MAC].para[AT_COMMAND_PARA_INDEX_7],
                            g_atParaList[AT_WIRX_SOURCE_MAC].para[AT_COMMAND_PARA_INDEX_8],
                            g_atParaList[AT_WIRX_SOURCE_MAC].para[AT_COMMAND_PARA_INDEX_9],
                            g_atParaList[AT_WIRX_SOURCE_MAC].para[AT_COMMAND_PARA_INDEX_10],
                            g_atParaList[AT_WIRX_SOURCE_MAC].para[AT_COMMAND_PARA_INDEX_11]);
        TAF_SPRINTF_CHK_RTN_VAL_CONTINUE(bufLen, sizeof(acCmd), sizeof(acCmd) - 1);

        AT_WIFI_TEST_CMD_CALL(acCmd);

        atUcastWifiRxPkts = AT_GetUcastWifiRxPkts();
        AT_GetWifiRxPacketReport(atUcastWifiRxPkts, &g_mcastWifiRxPkts);
    }

    return AT_OK;
}


VOS_UINT32 AT_QryWiFiTxPara(VOS_UINT8 indexNum)
{
    VOS_UINT8 wifiTxStatus;

    if (AT_IsMiscSupport(BSP_MODULE_TYPE_WIFI) == BSP_MODULE_UNSUPPORT) {
        return AT_ERROR;
    }

    /* ���������ӿڲ�ѯ��ǰWiFi��ģʽ */
    if (AT_GetWifiTcmdMode() == AT_WIFI_TX_MODE) {
        wifiTxStatus = AT_WIFI_TX_ON;
    } else {
        wifiTxStatus = AT_WIFI_TX_OFF;
    }

    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName, wifiTxStatus);
    return AT_OK;
}

VOS_UINT32 AT_QryWiFiRxPara(VOS_UINT8 indexNum)
{
    VOS_UINT8 wifiRxStatus;

    if (AT_IsMiscSupport(BSP_MODULE_TYPE_WIFI) == BSP_MODULE_UNSUPPORT) {
        return AT_ERROR;
    }

    /* ���������ӿڲ�ѯ��ǰWiFi��ģʽ */
    if (AT_GetWifiTcmdMode() == AT_WIFI_RX_MODE) {
        wifiRxStatus = AT_WIFI_TX_ON;
    } else {
        wifiRxStatus = AT_WIFI_TX_OFF;
    }

    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName, wifiRxStatus);
    return AT_OK;
}


VOS_UINT32 AT_SetWiFiEnablePara(VOS_UINT8 indexNum)
{
    VOS_UINT32 rst;

    rst = VOS_OK;

    /* �������� */
    if (g_atParaIndex > 1) {
        return AT_TOO_MANY_PARA;
    }

    if (g_atParaList[0].paraValue == 0) {
        /* �ر�WIFI */
        rst = (VOS_UINT32)AT_WifiPowerShutdown();
    } else if (g_atParaList[0].paraValue == 1) {
    } else {
#if (FEATURE_LTE == FEATURE_ON)

        VOS_TaskDelay(5500); /* ������Ҫ����Ҫ5.5s���ӳ� 5500ms = 5.5s */

#endif

        if (AT_WifiPowerShutdown() != VOS_OK) {
            return AT_ERROR;
        }

#if (FEATURE_LTE == FEATURE_ON)

        VOS_TaskDelay(1000); /* ������Ҫ����Ҫ1���ӳ� 1000ms = 1s */

#endif

        /* ����ģʽ��WIFI */
        rst = (VOS_UINT32)AT_WifiPowerStart();
    }

    if (rst != VOS_OK) {
        return AT_ERROR;
    }

    return AT_OK;
}


VOS_UINT32 AT_QryWiFiEnablePara(VOS_UINT8 indexNum)
{
    /* ���������ӿڲ�ѯ��ǰWiFi��״̬ */
    VOS_UINT32 wifiStatus;

    if (AT_IsMiscSupport(BSP_MODULE_TYPE_WIFI) == BSP_MODULE_UNSUPPORT) {
        return AT_ERROR;
    }

    wifiStatus = (VOS_UINT32)AT_GetWifiStatus();

    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName, wifiStatus);
    return AT_OK;
}



VOS_UINT32 AT_SetWiFiModePara(VOS_UINT8 indexNum)
{
    /* �������� */
    if (g_atParaIndex > 1) {
        return AT_TOO_MANY_PARA;
    }

    if (AT_IsMiscSupport(BSP_MODULE_TYPE_WIFI) == BSP_MODULE_UNSUPPORT) {
        return AT_ERROR;
    }

    AT_WIFI_TEST_CMD_CALL("wl down");

    if (g_atParaList[0].paraValue == AT_WIFI_SUPPORT_MODE_B) {
        AT_WIFI_TEST_CMD_CALL("wl nmode 0");
        AT_WIFI_TEST_CMD_CALL("wl gmode 0");
    } else if (g_atParaList[0].paraValue == AT_WIFI_SUPPORT_MODE_G) {
        AT_WIFI_TEST_CMD_CALL("wl nmode 0");
        AT_WIFI_TEST_CMD_CALL("wl gmode 2");
    } else {
        AT_WIFI_TEST_CMD_CALL("wl nmode 1");
        AT_WIFI_TEST_CMD_CALL("wl gmode 1");
    }

    AT_WIFI_TEST_CMD_CALL("wl up");

    /* ����ȫ�ֱ�����ѱ���ѯ */
    AT_SetWifiMode(g_atParaList[0].paraValue);

    return AT_OK;
}


VOS_UINT32 AT_QryWiFiModePara(VOS_UINT8 indexNum)
{
    VOS_UINT32 atWifiMode;

    if (AT_IsMiscSupport(BSP_MODULE_TYPE_WIFI) == BSP_MODULE_UNSUPPORT) {
        return AT_ERROR;
    }

    atWifiMode = AT_GetWifiMode();
    /* WIFIģ��ֻ֧��B/G/Nģʽ */
    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName, atWifiMode);
    return AT_OK;
}


VOS_UINT32 AT_SetWiFiFreqPara(VOS_UINT8 indexNum)
{
    VOS_CHAR   acCmd[AT_ACCMD_ARRAY_MAX_LEN] = {0};
    VOS_UINT32 wifiFreq;
    VOS_UINT32 i;
    VOS_INT32  bufLen;
    VOS_UINT32 channel[] = {
        2412, 2417, 2422, 2427, 2432, 2437, 2442, 2447, 2452, 2457, 2462, 2467, 2472, 2484
    };

    /* �������� */
    if (g_atParaIndex > 1) {
        return AT_TOO_MANY_PARA;
    }

    if (AT_IsMiscSupport(BSP_MODULE_TYPE_WIFI) == BSP_MODULE_UNSUPPORT) {
        return AT_ERROR;
    }

    /* �����ŵ�  */
    wifiFreq = 0;
    for (i = 0; i < (sizeof(channel) / sizeof(VOS_UINT32)); i++) {
        if (g_atParaList[0].paraValue == channel[i]) {
            wifiFreq = (i + 1);
            break;
        }
    }

    if (wifiFreq == 0) {
        AT_WARN_LOG("AT_SetWiFiFreqPara: freq ERROR");
        return AT_ERROR;
    }

    /* ���õ���ӿ� */
    AT_WIFI_TEST_CMD_CALL("wl down");

    bufLen = snprintf_s(acCmd, sizeof(acCmd), sizeof(acCmd) - 1, "wl channel %d", wifiFreq);
    TAF_SPRINTF_CHK_RTN_VAL_CONTINUE(bufLen, sizeof(acCmd), sizeof(acCmd) - 1);

    AT_WIFI_TEST_CMD_CALL(acCmd);

    AT_WIFI_TEST_CMD_CALL("wl up");

    /* ����ȫ�ֱ�����ѱ���ѯ */
    AT_SetWifiFreq(wifiFreq);

    return AT_OK;
}


VOS_UINT32 AT_QryWiFiFreqPara(VOS_UINT8 indexNum)
{
    VOS_UINT32 atWifiFreq;

    atWifiFreq = AT_GetWifiFreq();
    if (AT_IsMiscSupport(BSP_MODULE_TYPE_WIFI) == BSP_MODULE_UNSUPPORT) {
        return AT_ERROR;
    }

    /* ��ѯ����ֵ */
    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName, atWifiFreq);
    return AT_OK;
}


VOS_UINT32 AT_SetWiFiPowerPara(VOS_UINT8 indexNum)
{
    VOS_CHAR  acCmd[AT_ACCMD_ARRAY_MAX_LEN] = {0};
    VOS_INT32 wifiPower;
    VOS_INT32 atWifiPower;
    VOS_INT printResult;

    /* �������� */
    if (g_atParaIndex > AT_WIPOW_PARA_NUM) {
        return AT_TOO_MANY_PARA;
    }

    if (AT_IsMiscSupport(BSP_MODULE_TYPE_WIFI) == BSP_MODULE_UNSUPPORT) {
        return AT_ERROR;
    }

    AT_SetWifiPower((VOS_INT32)g_atParaList[AT_WIPOW_WIFI_DBM_PERCENT].paraValue);

    atWifiPower = AT_GetWifiPower();
    wifiPower   = atWifiPower / 100; /* WiFi���书�ʣ���λΪ0.01dBm����Ҫ��wifi���书�ʳ�100 */

    if (wifiPower < AT_WIFI_POWER_MIN) {
        wifiPower = AT_WIFI_POWER_MIN;
    }

    if (wifiPower > AT_WIFI_POWER_MAX) {
        wifiPower = AT_WIFI_POWER_MAX;
    }

    printResult = snprintf_s(acCmd, sizeof(acCmd), sizeof(acCmd) - 1, "wl txpwr1 -o -d %d", wifiPower);
    TAF_SPRINTF_CHK_RTN_VAL_CONTINUE(printResult, sizeof(acCmd), sizeof(acCmd) - 1);

    /* ���õ���ӿ� */
    AT_WIFI_TEST_CMD_CALL(acCmd);

    return AT_OK;
}


VOS_UINT32 AT_QryWiFiPowerPara(VOS_UINT8 indexNum)
{
    VOS_INT32 atWifiPower;

    atWifiPower = AT_GetWifiPower();
    if (AT_IsMiscSupport(BSP_MODULE_TYPE_WIFI) == BSP_MODULE_UNSUPPORT) {
        return AT_ERROR;
    }

    /* ��ѯ����ֵ */
    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName, atWifiPower);
    return AT_OK;
}



VOS_UINT32 AT_SetWiFiBandPara(VOS_UINT8 indexNum)
{
    /* �������� */
    if (g_atParaIndex > 1) {
        return AT_TOO_MANY_PARA;
    }

    if (AT_IsMiscSupport(BSP_MODULE_TYPE_WIFI) == BSP_MODULE_UNSUPPORT) {
        return AT_ERROR;
    }

    /* ĿǰWifiֻ֧��20M��40M����ERROR */
    if (g_atParaList[0].paraValue != AT_WIFI_BAND_20M) {
        return AT_ERROR;
    }

    return AT_OK;
}


VOS_UINT32 AT_TransferWifiRate(VOS_UINT32 inRate, VOS_UINT32 *outRate)
{
    VOS_UINT32 indexNum;
    /* WIFI nģʽ AT^WIDATARATE���õ�����ֵ��WL��������ֵ�Ķ�Ӧ�� */
    VOS_UINT32 atWifiNRateTable[AT_WIFI_N_RATE_NUM] = { 650, 1300, 1950, 2600, 3900, 5200, 5850, 6500 };

    for (indexNum = 0; indexNum < AT_WIFI_N_RATE_NUM; indexNum++) {
        if (atWifiNRateTable[indexNum] == inRate) {
            *outRate = indexNum;
            break;
        }
    }

    if (indexNum >= AT_WIFI_N_RATE_NUM) {
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 AT_SetWiFiRatePara(VOS_UINT8 indexNum)
{
    VOS_CHAR   acCmd[AT_ACCMD_ARRAY_MAX_LEN] = {0};
    VOS_UINT32 wifiRate;
    VOS_INT32  bufLen;
    VOS_UINT32 atWifiMode;

#if (FEATURE_LTE == FEATURE_ON)
#else
    VOS_UINT32 nRate;
    VOS_UINT32 rslt;
#endif
    /* �������� */
    if (g_atParaIndex > AT_WIDATARATE_PARA_NUM) {
        return AT_TOO_MANY_PARA;
    }

    if (AT_IsMiscSupport(BSP_MODULE_TYPE_WIFI) == BSP_MODULE_UNSUPPORT) {
        return AT_ERROR;
    }

    /* WiFi���ʣ���λΪ0.01Mb/s, ��Ҫ��wifi���ʳ�100 */
    wifiRate   = g_atParaList[AT_WIDATARATE_RATE].paraValue / 100;
    atWifiMode = AT_GetWifiMode();

    if (atWifiMode == AT_WIFI_SUPPORT_MODE_N) {
#if (FEATURE_LTE == FEATURE_ON)
        bufLen = snprintf_s(acCmd, sizeof(acCmd), sizeof(acCmd) - 1, "wl nrate -m %d", AT_WIFI_N_RATE_MAX_VALUE);
        TAF_SPRINTF_CHK_RTN_VAL_CONTINUE(bufLen, sizeof(acCmd), sizeof(acCmd) - 1);
#else
        rslt = AT_TransferWifiRate(g_atParaList[AT_WIDATARATE_RATE].paraValue, &nRate);

        if (rslt == VOS_OK) {
            bufLen = snprintf_s(acCmd, sizeof(acCmd), sizeof(acCmd) - 1, "wl nrate -m %d", nRate);
            TAF_SPRINTF_CHK_RTN_VAL_CONTINUE(bufLen, sizeof(acCmd), sizeof(acCmd) - 1);
        } else {
            return AT_ERROR;
        }
#endif
    } else if (atWifiMode == AT_WIFI_SUPPORT_MODE_G) {
#if (FEATURE_LTE == FEATURE_ON)
        bufLen = snprintf_s(acCmd, sizeof(acCmd), sizeof(acCmd) - 1, "wl bg_rate %d", wifiRate);
        TAF_SPRINTF_CHK_RTN_VAL_CONTINUE(bufLen, sizeof(acCmd), sizeof(acCmd) - 1);
#else
        bufLen = snprintf_s(acCmd, sizeof(acCmd), sizeof(acCmd) - 1, "wl rate %d", wifiRate);
        TAF_SPRINTF_CHK_RTN_VAL_CONTINUE(bufLen, sizeof(acCmd), sizeof(acCmd) - 1);
#endif
    } else if (atWifiMode == AT_WIFI_SUPPORT_MODE_B) {
#if (FEATURE_LTE == FEATURE_ON)
        bufLen = snprintf_s(acCmd, sizeof(acCmd), sizeof(acCmd) - 1, "wl bg_rate %d", wifiRate);
        TAF_SPRINTF_CHK_RTN_VAL_CONTINUE(bufLen, sizeof(acCmd), sizeof(acCmd) - 1);
#else
        bufLen = snprintf_s(acCmd, sizeof(acCmd), sizeof(acCmd) - 1, "wl rate %d", wifiRate);
        TAF_SPRINTF_CHK_RTN_VAL_CONTINUE(bufLen, sizeof(acCmd), sizeof(acCmd) - 1);
#endif
    } else {
        AT_WARN_LOG("AT_SetWiFiRatePara: wifimode ERROR");
        return AT_ERROR;
    }

    AT_WIFI_TEST_CMD_CALL(acCmd);

    /* ����ȫ�ֱ�����ѱ���ѯ */
    AT_SetWifiRate(g_atParaList[AT_WIDATARATE_RATE].paraValue);

    return AT_OK;
}


VOS_UINT32 AT_WiFiLogParaPreCheck(VOS_VOID)
{
    if (AT_IsMiscSupport(BSP_MODULE_TYPE_WIFI) == BSP_MODULE_UNSUPPORT) {
        return AT_ERROR;
    }

    /* �������� */
    if (g_atParaIndex > AT_WILOG_PARA_MAX_NUM) {
        return AT_TOO_MANY_PARA;
    }

    return AT_OK;
}

VOS_UINT32 AT_SetWiFiLogSSID(TAF_AT_MultiWifiSsid *wifiSsid)
{
    errno_t memResult;

    /* �������ȹ��� */
    if (g_atParaList[1].paraLen >= AT_WIFI_SSID_LEN_MAX) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ��ȡWIFI KEY��Ӧ��NV�� */
    if (TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_MULTI_WIFI_STATUS_SSID, wifiSsid, sizeof(TAF_AT_MultiWifiSsid)) !=
        VOS_OK) {
        AT_WARN_LOG("AT_SetWiFiSsidPara:READ NV ERROR");
        return AT_ERROR;
    } else {
        (VOS_VOID)memset_s(wifiSsid->wifiSsid[0], AT_WIFI_SSID_LEN_MAX, 0x00, AT_WIFI_SSID_LEN_MAX);

        if (g_atParaList[1].paraLen > 0) {
            memResult = memcpy_s(wifiSsid->wifiSsid[0], AT_WIFI_SSID_LEN_MAX, g_atParaList[1].para,
                                 g_atParaList[1].paraLen);
            TAF_MEM_CHK_RTN_VAL(memResult, AT_WIFI_SSID_LEN_MAX, g_atParaList[1].paraLen);
        }

        wifiSsid->wifiSsid[0][g_atParaList[1].paraLen] = '\0';

        /* д��WIFI SSID��Ӧ��NV�� */
        if (TAF_ACORE_NV_WRITE(MODEM_ID_0, NV_ITEM_MULTI_WIFI_STATUS_SSID, (VOS_UINT8 *)wifiSsid,
                               sizeof(TAF_AT_MultiWifiSsid)) != VOS_OK) {
            AT_WARN_LOG("AT_SetWiFiSsidPara:WRITE NV ERROR");
            return AT_ERROR;
        }
    }

    return AT_OK;
}


VOS_UINT32 AT_SetWiFiLogPara(VOS_UINT8 indexNum)
{
    TAF_AT_MultiWifiSsid wifiSsid;
    TAF_AT_MultiWifiSec  wifiKey;
    errno_t              memResult;
    VOS_UINT32           ret;

    ret = AT_WiFiLogParaPreCheck();
    if (ret != AT_OK) {
        return ret;
    }

    (VOS_VOID)memset_s(&wifiKey, sizeof(wifiKey), 0x00, sizeof(wifiKey));
    (VOS_VOID)memset_s(&wifiSsid, sizeof(wifiSsid), 0x00, sizeof(wifiSsid));

    /* ����WIFI SSID */
    if (g_atParaList[0].paraValue == 0) {
        ret = AT_SetWiFiLogSSID(&wifiSsid);
        if (ret != AT_OK) {
            return ret;
        }
    } else {
        /* ��ȡWIFI KEY��Ӧ��NV�� */
        if (TAF_ACORE_NV_READ_IN_CCORE(MODEM_ID_0, NV_ITEM_MULTI_WIFI_KEY, (VOS_UINT8 *)&wifiKey,
                                       sizeof(TAF_AT_MultiWifiSec)) != VOS_OK) {
            AT_WARN_LOG("AT_SetWiFiKeyPara:READ NV ERROR");
            return AT_ERROR;
        } else {
            /* �������ȹ��� */
            if (g_atParaList[1].paraLen >= AT_WIFI_KEY_LEN_MAX) {
                return AT_CME_INCORRECT_PARAMETERS;
            }

            /* ����indexд���Ӧ��KEY */
            switch (wifiKey.wifiWepKeyIndex[0]) {
                case AT_WIFI_WEP_KEY1:
                    if (g_atParaList[1].paraLen > 0) {
                        memResult = memcpy_s(wifiKey.wifiWepKey1[0], sizeof(wifiKey.wifiWepKey1), g_atParaList[1].para,
                                             g_atParaList[1].paraLen);
                        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(wifiKey.wifiWepKey1), g_atParaList[1].paraLen);
                    }
                    wifiKey.wifiWepKey1[0][g_atParaList[1].paraLen] = '\0';
                    break;

                case AT_WIFI_WEP_KEY2:
                    if (g_atParaList[1].paraLen > 0) {
                        memResult = memcpy_s(wifiKey.wifiWepKey2[0], sizeof(wifiKey.wifiWepKey2), g_atParaList[1].para,
                                             g_atParaList[1].paraLen);
                        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(wifiKey.wifiWepKey2), g_atParaList[1].paraLen);
                    }
                    wifiKey.wifiWepKey2[0][g_atParaList[1].paraLen] = '\0';
                    break;

                case AT_WIFI_WEP_KEY3:
                    if (g_atParaList[1].paraLen > 0) {
                        memResult = memcpy_s(wifiKey.wifiWepKey3[0], sizeof(wifiKey.wifiWepKey3), g_atParaList[1].para,
                                             g_atParaList[1].paraLen);
                        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(wifiKey.wifiWepKey3), g_atParaList[1].paraLen);
                    }
                    wifiKey.wifiWepKey3[0][g_atParaList[1].paraLen] = '\0';
                    break;

                case AT_WIFI_WEP_KEY4:
                    if (g_atParaList[1].paraLen > 0) {
                        memResult = memcpy_s(wifiKey.wifiWepKey4[0], sizeof(wifiKey.wifiWepKey4), g_atParaList[1].para,
                                             g_atParaList[1].paraLen);
                        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(wifiKey.wifiWepKey4), g_atParaList[1].paraLen);
                    }
                    wifiKey.wifiWepKey4[0][g_atParaList[1].paraLen] = '\0';
                    break;

                default:
                    break;
            }

            if (TAF_ACORE_NV_WRITE(MODEM_ID_0, NV_ITEM_MULTI_WIFI_KEY, (VOS_UINT8 *)&wifiKey,
                                   sizeof(TAF_AT_MultiWifiSec)) != VOS_OK) {
                AT_WARN_LOG("AT_SetWiFiKeyPara:WRITE NV ERROR");
                return AT_ERROR;
            }
        }
    }

    return AT_OK;
}


VOS_UINT32 AT_SetWiFiSsidPara(VOS_UINT8 indexNum)
{
    TAF_AT_MultiWifiSsid wifiSsid;
    errno_t              memResult;
    VOS_UINT8            group;

    if (AT_IsMiscSupport(BSP_MODULE_TYPE_WIFI) == BSP_MODULE_UNSUPPORT) {
        return AT_ERROR;
    }

    /* �������� */
    if (g_atParaIndex > AT_SSID_PARA_MAX_NUM) {
        return AT_TOO_MANY_PARA;
    }

    /* �������ȹ��� */
    if (g_atParaList[1].paraLen >= AT_WIFI_SSID_LEN_MAX) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* SSID���4�� */
    if (g_atParaList[0].paraValue >= AT_WIFI_MAX_SSID_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atDataLocked == VOS_TRUE) {
        return AT_ERROR;
    }

    (VOS_VOID)memset_s(&wifiSsid, sizeof(wifiSsid), 0x00, sizeof(wifiSsid));

    group = (VOS_UINT8)g_atParaList[0].paraValue;

    /* ��ȡWIFI KEY��Ӧ��NV�� */
    if (TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_MULTI_WIFI_STATUS_SSID, &wifiSsid, sizeof(TAF_AT_MultiWifiSsid)) !=
        VOS_OK) {
        AT_WARN_LOG("AT_SetWiFiSsidPara:READ NV ERROR");
        return AT_ERROR;
    } else {
        if (g_atParaList[1].paraLen > 0) {
            memResult = memcpy_s(&(wifiSsid.wifiSsid[group][0]), AT_WIFI_SSID_LEN_MAX, g_atParaList[1].para,
                                 g_atParaList[1].paraLen);
            TAF_MEM_CHK_RTN_VAL(memResult, AT_WIFI_SSID_LEN_MAX, g_atParaList[1].paraLen);
        }
        wifiSsid.wifiSsid[group][g_atParaList[1].paraLen] = '\0';

        /* д��WIFI SSID��Ӧ��NV�� */
        if (TAF_ACORE_NV_WRITE(MODEM_ID_0, NV_ITEM_MULTI_WIFI_STATUS_SSID, (VOS_UINT8 *)&wifiSsid,
                               sizeof(TAF_AT_MultiWifiSsid)) != VOS_OK) {
            AT_WARN_LOG("AT_SetWiFiSsidPara:WRITE NV ERROR");
            return AT_ERROR;
        }
    }

    return AT_OK;
}

VOS_UINT32 AT_SetWiFiKeyPara(VOS_UINT8 indexNum)
{
    TAF_AT_MultiWifiSec wifiKey;
    errno_t             memResult;
    VOS_UINT8           group;

    if (AT_IsMiscSupport(BSP_MODULE_TYPE_WIFI) == BSP_MODULE_UNSUPPORT) {
        return AT_ERROR;
    }

    /* �������� */
    if (g_atParaIndex > AT_WIKEY_PARA_MAX_NUM) {
        return AT_TOO_MANY_PARA;
    }

    /* �������ȹ��� */
    if (g_atParaList[1].paraLen > AT_WIFI_WLWPAPSK_LEN) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atDataLocked == VOS_TRUE) {
        return AT_ERROR;
    }

    /* ����4��SSID */
    if (g_atParaList[0].paraValue >= AT_WIFI_MAX_SSID_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    (VOS_VOID)memset_s(&wifiKey, sizeof(wifiKey), 0x00, sizeof(wifiKey));

    group = (VOS_UINT8)g_atParaList[0].paraValue;

    /* ��ȡWIFI KEY��Ӧ��NV�� */
    if (TAF_ACORE_NV_READ_IN_CCORE(MODEM_ID_0, NV_ITEM_MULTI_WIFI_KEY, (VOS_UINT8 *)&wifiKey,
                                   sizeof(TAF_AT_MultiWifiSec)) != NV_OK) {
        AT_WARN_LOG("AT_SetWiFiKeyPara:READ NV ERROR");
        return AT_ERROR;
    } else {
        /* д��KEY */
        if (g_atParaList[1].paraLen > 0) {
            memResult = memcpy_s(&(wifiKey.wifiWpapsk[group][0]), AT_WIFI_WLWPAPSK_LEN, g_atParaList[1].para,
                                 g_atParaList[1].paraLen);
            TAF_MEM_CHK_RTN_VAL(memResult, AT_WIFI_WLWPAPSK_LEN, g_atParaList[1].paraLen);
        }
        wifiKey.wifiWpapsk[group][g_atParaList[1].paraLen] = '\0';

        if (TAF_ACORE_NV_WRITE(MODEM_ID_0, NV_ITEM_MULTI_WIFI_KEY, (VOS_UINT8 *)&wifiKey,
                               sizeof(TAF_AT_MultiWifiSec)) != NV_OK) {
            AT_WARN_LOG("AT_SetWiFiKeyPara:WRITE NV ERROR");
            return AT_ERROR;
        }
    }

    return AT_OK;
}

VOS_UINT32 AT_QryWiFiBandPara(VOS_UINT8 indexNum)
{
    if (AT_IsMiscSupport(BSP_MODULE_TYPE_WIFI) == BSP_MODULE_UNSUPPORT) {
        return AT_ERROR;
    }

    /* Ŀǰֻ֧��20M���� */
    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName, AT_WIFI_BAND_20M);
    return AT_OK;
}


VOS_UINT32 AT_QryWiFiRatePara(VOS_UINT8 indexNum)
{
    VOS_UINT32 atWifiRate;

    atWifiRate = AT_GetWifiRate();
    if (AT_IsMiscSupport(BSP_MODULE_TYPE_WIFI) == BSP_MODULE_UNSUPPORT) {
        return AT_ERROR;
    }

    /* ��ѯ����ֵ */
    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName, atWifiRate);
    return AT_OK;
}


VOS_UINT32 AT_QryWiFiSsidPara(VOS_UINT8 indexNum)
{
    TAF_AT_MultiWifiSsid wifiSsid;
    VOS_UINT16           len;
    VOS_UINT8            ssidNum;
    VOS_UINT32           loop;
    VOS_UINT8            ssidLen[AT_WIFI_MAX_SSID_NUM];

    if (AT_IsMiscSupport(BSP_MODULE_TYPE_WIFI) == BSP_MODULE_UNSUPPORT) {
        return AT_ERROR;
    }

    (VOS_VOID)memset_s(&wifiSsid, sizeof(wifiSsid), 0x00, sizeof(wifiSsid));

    /* ��ȡWIFI SSID��Ӧ��NV�� */
    if (TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_MULTI_WIFI_STATUS_SSID, &wifiSsid, sizeof(TAF_AT_MultiWifiSsid)) !=
        VOS_OK) {
        AT_WARN_LOG("AT_SetWiFiSsidPara:READ NV ERROR");
        return AT_ERROR;
    }

    ssidNum = 0;
    (VOS_VOID)memset_s(ssidLen, sizeof(ssidLen), 0x00, sizeof(ssidLen));

    for (loop = 0; loop < AT_WIFI_MAX_SSID_NUM; loop++) {
        ssidLen[loop] = (VOS_UINT8)VOS_StrLen((VOS_CHAR *)wifiSsid.wifiSsid[loop]);

        if (ssidLen[loop] != 0) {
            ssidNum++;
        }
    }

    len = 0;
    len += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + len, "%s:%d%s", g_parseContext[indexNum].cmdElement->cmdName, ssidNum,
        g_atCrLf);

    for (loop = 0; loop < AT_WIFI_MAX_SSID_NUM; loop++) {
        if (ssidLen[loop] != 0) {
            len += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + len, "%s:%d,%s%s", g_parseContext[indexNum].cmdElement->cmdName, loop,
                wifiSsid.wifiSsid[loop], g_atCrLf);
        }
    }

    g_atSendDataBuff.bufLen = len;

    return AT_OK;
}


VOS_UINT32 AT_QryWiFiLogPara(VOS_UINT8 indexNum)
{
    TAF_AT_MultiWifiSsid wifiSsid;
    TAF_AT_MultiWifiSec  wifiSec;
    VOS_UINT16           len;

    if (AT_IsMiscSupport(BSP_MODULE_TYPE_WIFI) == BSP_MODULE_UNSUPPORT) {
        return AT_ERROR;
    }

    (VOS_VOID)memset_s(&wifiSec, sizeof(wifiSec), 0x00, sizeof(wifiSec));
    (VOS_VOID)memset_s(&wifiSsid, sizeof(wifiSsid), 0x00, sizeof(wifiSsid));

    /* ��ȡWIFI SSID��Ӧ��NV�� */
    if (TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_MULTI_WIFI_STATUS_SSID, &wifiSsid, sizeof(TAF_AT_MultiWifiSsid)) !=
        VOS_OK) {
        AT_WARN_LOG("AT_QryWiFiLogPara:READ NV FAIL");
        return AT_ERROR;
    }

    len = 0;

    len = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress + len, "%s:0,%s%s", g_parseContext[indexNum].cmdElement->cmdName,
        wifiSsid.wifiSsid[0], g_atCrLf);

    /* ��ȡWIFI key��Ӧ��NV�� */
    if (TAF_ACORE_NV_READ_IN_CCORE(MODEM_ID_0, NV_ITEM_MULTI_WIFI_KEY, (VOS_UINT8 *)&wifiSec,
                                   sizeof(TAF_AT_MultiWifiSec)) != VOS_OK) {
        AT_WARN_LOG("AT_QryWiFiLogPara:READ NV FAIL");
        return AT_ERROR;
    }

    /* KEY1��Ӧ��NV���ձ�ʾKEY1��Ч */
    if (VOS_StrLen((VOS_CHAR *)wifiSec.wifiWepKey1[0]) != 0) {
        len += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + len, "%s:1,%s%s", g_parseContext[indexNum].cmdElement->cmdName,
            wifiSec.wifiWepKey1[0], g_atCrLf);
    }

    /* KEY2��Ӧ��NV���ձ�ʾKEY2��Ч */
    if (VOS_StrLen((VOS_CHAR *)wifiSec.wifiWepKey2[0]) != 0) {
        len += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + len, "%s:2,%s%s", g_parseContext[indexNum].cmdElement->cmdName,
            wifiSec.wifiWepKey2[0], g_atCrLf);
    }
    /* KEY3��Ӧ��NV���ձ�ʾKEY3��Ч */
    if (VOS_StrLen((VOS_CHAR *)wifiSec.wifiWepKey3[0]) != 0) {
        len += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + len, "%s:3,%s%s", g_parseContext[indexNum].cmdElement->cmdName,
            wifiSec.wifiWepKey3[0], g_atCrLf);
    }
    /* KEY4��Ӧ��NV���ձ�ʾKEY3��Ч */
    if (VOS_StrLen((VOS_CHAR *)wifiSec.wifiWepKey4[0]) != 0) {
        len += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress + len, "%s:4,%s%s", g_parseContext[indexNum].cmdElement->cmdName,
            wifiSec.wifiWepKey4[0], g_atCrLf);
    }

    g_atSendDataBuff.bufLen = len;

    return AT_OK;
}

VOS_UINT32 AT_QryWiFiKeyPara(VOS_UINT8 indexNum)
{
    TAF_AT_MultiWifiSec wifiSec;
    VOS_UINT8           wifiKeyNum;
    VOS_UINT16          len;
    VOS_UINT8           wpapskLen[AT_WIFI_MAX_SSID_NUM];
    VOS_UINT32          loop;

    if (AT_IsMiscSupport(BSP_MODULE_TYPE_WIFI) == BSP_MODULE_UNSUPPORT) {
        return AT_ERROR;
    }

    (VOS_VOID)memset_s(&wifiSec, sizeof(wifiSec), 0x00, sizeof(wifiSec));

    /* ��ȡWIFI key��Ӧ��NV�� */
    if (TAF_ACORE_NV_READ_IN_CCORE(MODEM_ID_0, NV_ITEM_MULTI_WIFI_KEY, (VOS_UINT8 *)&wifiSec,
                                   sizeof(TAF_AT_MultiWifiSec)) != VOS_OK) {
        AT_WARN_LOG("AT_QryWiFiSsidPara:READ NV FAIL");
        return AT_ERROR;
    }

    wifiKeyNum = 0;
    (VOS_VOID)memset_s(wpapskLen, sizeof(wpapskLen), 0x00, sizeof(wpapskLen));

    for (loop = 0; loop < AT_WIFI_MAX_SSID_NUM; loop++) {
        /* KEY1��Ӧ��NV���ձ�ʾKEY1��Ч */
        wpapskLen[loop] = (VOS_UINT8)VOS_StrLen((VOS_CHAR *)wifiSec.wifiWpapsk[loop]);

        if (wpapskLen[loop] != 0) {
            wifiKeyNum++;
        }
    }

    /* δ����ʱ����Ҫ�����Ѷ���0�� */
    if (g_atDataLocked == VOS_TRUE) {
        wifiKeyNum = 0;
        (VOS_VOID)memset_s(wpapskLen, sizeof(wpapskLen), 0x00, sizeof(wpapskLen));
    }

    len = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s:%d%s", g_parseContext[indexNum].cmdElement->cmdName, wifiKeyNum, g_atCrLf);

    for (loop = 0; loop < AT_WIFI_MAX_SSID_NUM; loop++) {
        if (wpapskLen[loop] != 0) {
            len += (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
                (TAF_CHAR *)g_atSndCodeAddress + len, "%s:%d,%s%s", g_parseContext[indexNum].cmdElement->cmdName, loop,
                wifiSec.wifiWpapsk[loop], g_atCrLf);
        }
    }

    g_atSendDataBuff.bufLen = len;

    return AT_OK;
}


VOS_UINT32 AT_TestSsidPara(VOS_UINT8 indexNum)
{
    if (AT_IsMiscSupport(BSP_MODULE_TYPE_WIFI) == BSP_MODULE_UNSUPPORT) {
        return AT_ERROR;
    }

    g_atSendDataBuff.bufLen =
        (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName,
            AT_WIFI_MAX_SSID_NUM);
    return AT_OK;
}


VOS_UINT32 AT_TestWikeyPara(VOS_UINT8 indexNum)
{
    if (AT_IsMiscSupport(BSP_MODULE_TYPE_WIFI) == BSP_MODULE_UNSUPPORT) {
        return AT_ERROR;
    }

    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
        (TAF_CHAR *)g_atSndCodeAddress, "%s:%d", g_parseContext[indexNum].cmdElement->cmdName, AT_WIFI_KEY_NUM);
    return AT_OK;
}


#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))

VOS_UINT32 AT_QryWakelock(VOS_UINT8 indexNum)
{
    VOS_UINT32 wakelockStatus;

    /* ������� */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    if (g_atWakeLock.active) {
        wakelockStatus = 1;
    } else {
        wakelockStatus = 0;
    }

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", "^WAKELOCK", wakelockStatus, g_atCrLf);

    return AT_OK;
}


VOS_UINT32 At_SetWakelock(VOS_UINT8 indexNum)
{
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_NORM_LOG("At_SetWakelock arguments found, return error!!\n ");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaIndex > 1) {
        AT_NORM_LOG1("At_SetWakelock: the number of parameter is error ", (VOS_INT32)g_atParaIndex);
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaList[0].paraLen == 0) {
        AT_NORM_LOG("At_SetWakelock: parameter len is 0 ");

        return AT_ERROR;
    }

    if ((g_atParaList[0].paraValue != 0) && (g_atParaList[0].paraValue != 1)) {
        AT_NORM_LOG("At_SetWakelock: parameter invalid");
        return AT_ERROR;
    }

    if (g_atParaList[0].paraValue == 0) {
        AT_WakeUnLock(&g_atWakeLock);
    }

    if (g_atParaList[0].paraValue == 1) {
        AT_WakeLock(&g_atWakeLock);
    }

    return AT_OK;
}


VOS_UINT32 At_QryCcpuNidDisablePara(VOS_UINT8 indexNum)
{
    VOS_INT32  ret;
    VOS_UINT32 result = AT_OK;

    /* ������� */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    /* ����DRV�ӿ� */
    ret = AT_EfuseIoctl(MDRV_EFUSE_IOCTL_CMD_GET_CCPUNIDEN, 0, VOS_NULL_PTR, 0);

    /* С��0��ʾʧ�ܣ�0��ʾ����û��д����1��ʾ������д�� */
    if ((ret == 0) || (ret == 1)) {
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName, ret);
        result                  = AT_OK;
    } else {
        result = AT_ERROR;
    }

    At_FormatResultData(indexNum, result);
    return VOS_OK;
}


VOS_UINT32 At_QryAcpuNidDisablePara(VOS_UINT8 indexNum)
{
    VOS_INT32  ret;
    VOS_UINT32 result = AT_OK;

    /* ������� */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    /* ����DRV�ӿ� */
    ret = AT_EfuseIoctl(MDRV_EFUSE_IOCTL_CMD_GET_ACPUNIDEN, 0, VOS_NULL_PTR, 0);

    /* С��0��ʾʧ�ܣ�0��ʾ����û��д����1��ʾ������д�� */
    if ((ret == 0) || (ret == 1)) {
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName, ret);
        result                  = AT_OK;
    } else {
        result = AT_ERROR;
    }

    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 At_SetCcpuNidDisablePara(VOS_UINT8 indexNum)
{
    VOS_INT32 ret;
    /* ������� */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_WARN_LOG("At_SetCcpuNidDisablePara : Current Option is not AT_CMD_OPT_SET_PARA_CMD.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* �жϲ������� */
    if ((g_atParaIndex != 1) || (g_atParaList[0].paraLen == 0)) {
        /* ������������ */
        AT_WARN_LOG("At_SetCcpuNidDisablePara : Para wrong.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ����DRV�ӿ� */
    ret = AT_EfuseIoctl(MDRV_EFUSE_IOCTL_CMD_SET_CCPUNIDEN, (int)g_atParaList[0].paraValue, VOS_NULL_PTR, 0);

    /* �ж��Ƿ�ִ�гɹ� */
    if ((ret == AT_EFUSE_OK) || (ret == AT_EFUSE_REPEAT)) {
        return AT_OK;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 At_SetAcpuNidDisablePara(VOS_UINT8 indexNum)
{
    VOS_INT32 ret;
    /* ������� */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        AT_WARN_LOG("At_SetAcpuNidDisablePara : Current Option is not AT_CMD_OPT_SET_PARA_CMD.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* �жϲ������� */
    if ((g_atParaIndex != 1) || (g_atParaList[0].paraLen == 0)) {
        /* ������������ */
        AT_WARN_LOG("At_SetAcpuNidDisablePara : Para wrong.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ����DRV�ӿ� */
    ret = AT_EfuseIoctl(MDRV_EFUSE_IOCTL_CMD_SET_ACPUNIDEN, (int)g_atParaList[0].paraValue, VOS_NULL_PTR, 0);

    /* �ж��Ƿ�ִ�гɹ� */
    if ((ret == AT_EFUSE_OK) || (ret == AT_EFUSE_REPEAT)) {
        return AT_OK;
    } else {
        return AT_ERROR;
    }
}

// �л�������ģʽ����������󽫽�������ģʽ
VOS_UINT32 atSetGodLoad(VOS_UINT8 clientId)
{
    g_atSendDataBuff.bufLen = 0;

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "OK");
    AT_SetDloadCurMode(DLOAD_MODE_DOWNLOAD);

    VOS_TaskDelay(AT_SET_LOAD_TASK_DELAY_TIME);

    /*lint -e40 */
#if (VOS_OS_VER != VOS_WIN32)
    AT_SetDloadSoftload(true);
#endif
    /*lint +e40 */

#if (VOS_WIN32 == VOS_OS_VER || OSA_CPU_CCPU == VOS_OSA_CPU)
    AT_SysbootShutdown(0);
#else
    AT_SysbootShutdown();
#endif

    return AT_OK;
}


VOS_UINT32 AT_SetKcePara(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;
    VOS_INT    iRst;

    /* ������� */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* �����Ϸ��Լ�� */
    if (g_atParaIndex != 1) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* Asciiת�ֽ��� */
    result = At_AsciiNum2HexString(g_atParaList[0].para, &g_atParaList[0].paraLen);

    if (result != AT_SUCCESS) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ���õ����ṩ�Ľӿ�ʵ�����ò��� */
    iRst = AT_EfuseIoctl(MDRV_EFUSE_IOCTL_CMD_SET_KCE, 0, g_atParaList[0].para,
                         (VOS_INT)(g_atParaList[0].paraLen / sizeof(VOS_UINT32)));

    /* ���ݵ���ӿڷ��صĽ���������ý�� */
    /* ������1ʱ�����ظ�����,����0ʱ���óɹ�,�����������ʧ�� */
    if (iRst == AT_EFUSE_REPEAT) {
        return AT_ERROR;
    } else if (iRst == AT_EFUSE_OK) {
        return AT_OK;
    } else {
        AT_WARN_LOG("AT_SetSecureStatePara : Set KCE req failed.");
    }

    return AT_CME_UNKNOWN;
}


VOS_UINT32 AT_SetApbatlvlPara(VOS_UINT8 indexNum)
{
    BATT_STATE_S batLvl;
    VOS_UINT32   chargeState;

    if (AT_IsMiscSupport(BSP_MODULE_TYPE_CHARGE) == BSP_MODULE_UNSUPPORT) {
        return AT_ERROR;
    }

    /* Arguments found, return error. */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_CMD_NO_PARA) {
        PS_PRINTF_WARNING("AT^APBATLVL arguments found, return error!!\n ");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ���ýӿڻ�ȡ������ */
    if (AT_GetMiscBatteryState(&batLvl) != VOS_OK) {
        return AT_ERROR;
    }

    /* ���ֳ���δ���״̬ */
    if ((batLvl.charging_state == CHARGING_UP) || (batLvl.charging_state == CHARGING_DOWN)) {
        chargeState = 1;
    } else if ((batLvl.charging_state == NO_CHARGING_UP) || (batLvl.charging_state == NO_CHARGING_DOWN)) {
        chargeState = 0;
    } else {
        return AT_ERROR;
    }

    /* ���˵�ص�������ֵ */
    if ((batLvl.battery_level < BATT_LOW_POWER) || (batLvl.battery_level >= BATT_LEVEL_MAX)) {
        return AT_ERROR;
    }

    g_atSendDataBuff.bufLen =
        (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "%s:%d,%d", g_parseContext[indexNum].cmdElement->cmdName, chargeState,
            batLvl.battery_level);

    return AT_OK;
}


VOS_UINT32 At_GetKeyInfoPara(VOS_UINT8 indexNum)
{
    VOS_INT    iResult;
    VOS_UINT16 length;
    VOS_UINT16 keyLen;
    VOS_UINT8  auckeybuf[AT_KEYBUFF_LEN];
    VOS_UINT8  hashbuf[AT_KEY_HASH_LEN];
    VOS_UINT32 i;

    if (g_atParaIndex != 1 || (g_atParaList[0].paraLen == 0)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaList[0].paraValue >= AT_KEY_TYPE_BUTT) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* �ֲ�������ʼ�� */
    (VOS_VOID)memset_s(hashbuf, (VOS_SIZE_T)sizeof(hashbuf), 0x00, AT_KEY_HASH_LEN);
    (VOS_VOID)memset_s(auckeybuf, (VOS_SIZE_T)sizeof(auckeybuf), 0x00, AT_KEYBUFF_LEN);

    switch (g_atParaList[0].paraValue) {
        case AT_KEY_TYPE_DIEID:
        case AT_KEY_TYPE_TBOX_SMS: {
            /* ��׮���ݲ����� */
            return AT_OK;
        }
        case AT_KEY_TYPE_SOCID: {
            keyLen  = AT_SOCID_LEN;
            iResult = AT_EfuseIoctl(MDRV_EFUSE_IOCTL_CMD_GET_SOCID, 0, auckeybuf,
                                    (VOS_INT)(AT_SOCID_LEN / sizeof(VOS_UINT32)));

            /* �����쳣��ѯ��� */
            if (iResult != MDRV_OK) {
                AT_WARN_LOG("At_QryGetKeyInfoPara:get soc id error.\n");
                return AT_ERROR;
            }
            break;
        }
        case AT_KEY_TYPE_AUTHKEY: {
            keyLen  = AT_AUTHKEY_LEN;
            iResult = AT_EfuseIoctl(MDRV_EFUSE_IOCTL_CMD_GET_AUTHKEY, 0, auckeybuf,
                                    (VOS_INT)(AT_AUTHKEY_LEN / sizeof(VOS_UINT32)));
            if (iResult != MDRV_OK) {
                AT_WARN_LOG("At_QryGetKeyInfoPara:get authkey error.\n");
                return AT_ERROR;
            }
            break;
        }
        default: {
            return AT_CME_INCORRECT_PARAMETERS;
        }
    }

    /* ��ӡ���AT���� */
    length = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s:%s", g_parseContext[indexNum].cmdElement->cmdName, g_atCrLf);

    /* key info */
    for (i = 0; i < keyLen; i++) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%02X", auckeybuf[i]);
    }
    /* key len ��"%02x"��ʽ�����02 ��ʾ������λ��ǰ�油0���,�������ӣ�����*2 */
    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, ",%d,", keyLen * 2);
    /* key hash */
    for (i = 0; i < AT_KEY_HASH_LEN; i++) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%02x", hashbuf[i]);
    }
    g_atSendDataBuff.bufLen = length;
    return AT_OK;
}

VOS_UINT32 At_SetKeyPara(VOS_UINT8 indexNum)
{
    VOS_INT32 ret;
    VOS_UINT8 keyBuf[AT_AUTHKEY_LEN];

    if ((g_atParaIndex != 1) || (g_atParaList[0].paraLen == 0)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaList[0].paraValue >= AT_KEY_TYPE_BUTT) {
        AT_WARN_LOG("At_SetKeyPara: not support the para,pls check\n");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    (VOS_VOID)memset_s(keyBuf, (VOS_SIZE_T)sizeof(keyBuf), 0x00, (VOS_SIZE_T)sizeof(keyBuf));

    switch (g_atParaList[0].paraValue) {
        case AT_KEY_TYPE_DIEID:
        case AT_KEY_TYPE_TBOX_SMS:
        case AT_KEY_TYPE_SOCID:
            break;
        case AT_KEY_TYPE_AUTHKEY: {
#if (VOS_OS_VER == VOS_LINUX)
            get_random_bytes(keyBuf, AT_AUTHKEY_LEN);
#endif

            ret = AT_EfuseIoctl(MDRV_EFUSE_IOCTL_CMD_SET_AUTHKEY, 0, keyBuf,
                                (VOS_INT)(AT_AUTHKEY_LEN / sizeof(VOS_UINT32)));
            if (ret != MDRV_OK) {
                AT_WARN_LOG("At_SetKeyPara: mdrv_set_authkey error.\n");
                return AT_ERROR;
            }
            break;
        }
        default: {
            AT_WARN_LOG("At_SetKeyPara: para is error,pls check.\n");
            return AT_CME_INCORRECT_PARAMETERS;
        }
    }
    return AT_OK;
}


VOS_UINT32 AT_SetAuthKeyRdPara(VOS_UINT8 indexNum)
{
    VOS_INT32 result;
    VOS_INT32 qryResult;
    VOS_INT32 setParaVal;

    if ((g_atParaIndex != 1) || (g_atParaList[0].paraLen == 0)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    setParaVal = (VOS_INT32)g_atParaList[0].paraValue;

    result = AT_EfuseIoctl(MDRV_EFUSE_IOCTL_CMD_SET_AUTHKEYRD, setParaVal, NULL, 0);
    if (result < MDRV_OK) {
        AT_WARN_LOG("AT_SetAuthKeyRdPara: set AUTHKEYRD error.");
        return AT_ERROR;
    }

    /* ����ظ����ô�ӡlog��ʾ */
    if (result != MDRV_OK) {
        AT_WARN_LOG("AT_SetAuthKeyRdPara: set AUTHKEYRD repeatly.");
    }

    /* ������ɺ��������Ƿ�ɹ� */
    qryResult = AT_EfuseIoctl(MDRV_EFUSE_IOCTL_CMD_GET_AUTHKEYRD, 0, NULL, 0);

    if (qryResult != setParaVal) {
        AT_WARN_LOG("AT_SetAuthKeyRdPara: set AUTHKEYRD value not correct.");
        return AT_ERROR;
    }

    return AT_OK;
}


VOS_UINT32 AT_QryAuthKeyRdPara(VOS_UINT8 indexNum)
{
    VOS_INT32 authKeyRdVal;

    authKeyRdVal = AT_EfuseIoctl(MDRV_EFUSE_IOCTL_CMD_GET_AUTHKEYRD, 0, NULL, 0);

    if (authKeyRdVal < MDRV_OK) {
        AT_WARN_LOG("AT_QryAuthKeyRdPara: get AUTHKEYRD error.");
        return AT_ERROR;
    }

    /* ��ӡ��� */
    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName, authKeyRdVal);

    return AT_OK;
}
#endif

#endif


