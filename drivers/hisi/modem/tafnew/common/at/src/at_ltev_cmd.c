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

#include "at_ltev_cmd.h"
#include "securec.h"
#include "taf_type_def.h"
#include "ATCmdProc.h"
#include "mn_comm_api.h"
#include "AtTestParaCmd.h"
#include "at_ltev_msg_proc.h"
#include "AtDataProc.h"

#if (FEATURE_LTEV == FEATURE_ON)
#if (FEATURE_LTEV_WL == FEATURE_ON)
#include "nv_stru_drv.h"
#include "at_mta_interface.h"
#endif
#endif


#define THIS_FILE_ID PS_FILE_ID_AT_LTEV_CMD_C
#define AT_MCGCFG_PARA_MAX_NUM       15
#define AT_MCGCFG_PARA_GROUP_ID      0 /* MCGCFG的第一个参数GROUP_ID */
#define AT_MCGCFG_PARA_SELF_RSU_ID   1 /* MCGCFG的第二个参数SELF_RSU_ID */
#define AT_MCGCFG_PARA_SELF_RSU_TYPE 2 /* MCGCFG的第三个参数SELF_RSU_TYPE */
#define AT_MCGCFG_PARA_INDOOR_RSU_ID 3 /* MCGCFG的第四个参数INDOOR_RSU_ID */
#define AT_MCGCFG_PARA_RSU_LIST_NUM  4 /* MCGCFG的第五个参数RSU_LIST_NUM */
#define AT_MCGCFG_PARA_RSU_ID_LIST   5 /* MCGCFG的第六个参数RSU_ID_LIST */
#define AT_PC5SYNC_PARA_MAX_NUM           10
#define AT_PC5SYNC_PARA_SYNC_SWITCH          0 /* PC5SYNC的第一个参数SYNC_SWITCH */
#define AT_PC5SYNC_PARA_SYNC_DFN_SWITCH      1 /* PC5SYNC的第二个参数SYNC_DFN_SWITCH */
#define AT_PC5SYNC_PARA_GNSS_VALID_TIMER_LEN 2 /* PC5SYNC的第三个参数GNSS_VALID_TIMER_LEN */
#define AT_PC5SYNC_PARA_SYNC_OFFSET_IND1     3 /* PC5SYNC的第四个参数SYNC_OFFSET_IND1  */
#define AT_PC5SYNC_PARA_SYNC_OFFSET_IND2     4 /* PC5SYNC的第五个参数SYNC_OFFSET_IND2 */
#define AT_PC5SYNC_PARA_SYNC_OFFSET_IND3     5 /* PC5SYNC的第六个参数SYNC_OFFSET_IND3 */
#define AT_PC5SYNC_PARA_SYNC_TX_THRESH_OOC   6 /* PC5SYNC的第七个参数SYNC_TX_THRESH_OOC */
#define AT_PC5SYNC_PARA_FLT_COEFFICIENT      7 /* PC5SYNC的第八个参数FLT_COEFFICIENT */
#define AT_PC5SYNC_PARA_SYNC_REF_MIN_HYST    8 /* PC5SYNC的第九个参数SYNC_REF_MIN_HYST */
#define AT_PC5SYNC_PARA_SYNC_REF_DIFF_HYST   9 /* PC5SYNC的第十个参数SYNC_REF_DIFF_HYST */
#define AT_PC5SYNC_PARA_SYNC_SWITCH_INVALID_VAL     0 /* PC5SYNC的第一个参数SYNC_SWITCH的无效取值 */
#define AT_PC5SYNC_PARA_SYNC_DFN_SWITCH_INVALID_VAL 1 /* PC5SYNC的第二个参数SYNC_DFN_SWITCH的无效取值 */
#define AT_LEDTEST_CMD_TIMER_LEN   1000
#define AT_LEDTEST_PARA_MAX_NUM    1
#define AT_LEDTEST_PARA_LEDTEST    0 /* LEDTEST的第一个参数LEDTEST */
#define AT_LEDTEST_PARA_LEDTEST_ON 1
#define AT_FILERD_PARA_NUM    1
#define AT_FILERD_PARA_AINDEX 0 /* FILERD的第一个参数AINDEX */
#define AT_FILEWR_PARA_NUM    2
#define AT_FILEWR_PARA_AINDEX 0 /* FILEWR的第一个参数AINDEX */
#define AT_FILEWR_PARA_INFO   1 /* FILEWR的第二个参数INFO */
#define AT_SFPTEST_CMD_TIMER_LEN 500
#define AT_SETGPIO_PARA_NUM   2
#define AT_SETGPIO_PARA_GPIO  0 /* SETGPIO的第一个参数GPIO */
#define AT_SETGPIO_PARA_VALUE 1 /* SETGPIO的第二个参数VALUE */
#define AT_CATM_PARA_STATE 0 /* CATM的第一个参数STATE */
#define AT_CATM_PARA_TYPE  1 /* CATM的第二个参数TYPE */
#define AT_VSYNCSRCRPT_PARA_MAX_NUM 1
#define AT_VSYNCSRCRPT_PARA_STATUS  0 /* VSYNCSRCRPT的第一个参数STATUS */
#define AT_VSYNCMODE_PARA_MAX_NUM 1
#define AT_VSYNCMODE_PARA_MODE    0 /* VSYNCMODE的第一个参数MODE */
#define AT_PTRRPT_PARA_NUM 1
#define AT_VPHYSTATCLR_PARA_NUM               1
#define AT_VPHYSTATCLR_PARA_VPHY_STAT_CLR_FLG 0 /* VPHYSTATCLR的第一个参数VPHY_STAT_CLR_FLG */
#define AT_SFPSWITCH_PARA_NUM 1
#define AT_GPIOHEATSET_PARA_NUM       1
#define AT_GPIOHEATSET_PARA_GPIO_TEST 0 /* GPIOHEATSET的第一个参数GPIO_TEST */
#define AT_HKADCTEST_PARA_MAX_NUM       1
#define AT_HKADCTEST_PARA_HKADC_CHANNEL 0 /* HKADCTEST的第一个参数HKADC_CHANNEL */
#define AT_QRYGPIO_PARA_MAX_NUM 1
#define AT_QRYGPIO_PARA_GPIO    0 /* QRYGPIO的第一个参数GPIO */
#define AT_SETBOOTMODE_PARA_NUM                1
#define AT_SETBOOTMODE_PARA_TRACE_ONSTART_FLAG 0 /* SETBOOTMODE的第一个参数TRACE_ONSTART_FLAG */
#define AT_MULTIUPGMODE_PARA_NUM      1
#define AT_MULTIUPGMODE_PARA_RESERVE2 0 /* MULTIUPGMODE的第一个参数RESERVE2 */

#if (FEATURE_LTEV == FEATURE_ON)
AT_ParCmdElement g_atLtevCmdTbl[] = {
    { AT_CMD_CATM,
      AT_SetCatm, AT_SET_PARA_TIME, AT_QryCatm, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"+CATM", (VOS_UINT8 *)"(0,1),(1)" },
    { AT_CMD_CV2XL2ID,
      AT_SetCv2xL2Id, AT_SET_PARA_TIME, AT_QryCv2xL2Id, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^CV2XL2ID", (VOS_UINT8 *)"(0-16777215),(0-16777215)" },
    { AT_CMD_CCUTLE,
      AT_SetCcutle, AT_SET_PARA_TIME, AT_QryCcutle, AT_QRY_PARA_TIME, AT_TestCcutle, AT_TEST_PARA_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"+CCUTLE", (VOS_UINT8 *)"(0,1),(0,1),(1),(0-16),(str)" },
    { AT_CMD_CUSPCREQ,
      AT_SetCuspcreq, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"+CUSPCREQ", VOS_NULL_PTR },
    { AT_CMD_CUTCR,
      AT_SetCutcr, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"+CUTCR", VOS_NULL_PTR },
    { AT_CMD_CCBRREQ,
      AT_SetCcbrreq, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"+CCBRREQ", VOS_NULL_PTR },
    { AT_CMD_CV2XDTS,
      AT_SetCv2xdts, AT_SET_PARA_TIME, AT_QryCv2xdts, AT_QRY_PARA_TIME, AT_TestCv2xdts, AT_TEST_PARA_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"+CV2XDTS", (VOS_UINT8 *)"(0,1),(0-8176),(10-10000)" },
    { AT_CMD_VSYNCSRC,
      VOS_NULL_PTR, AT_SET_PARA_TIME, AT_QryVsyncsrc, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^VSYNCSRC", VOS_NULL_PTR },
    { AT_CMD_VSYNCSRCRPT,
      AT_SetVsyncsrcrpt, AT_SET_PARA_TIME, AT_QryVsyncsrcrpt, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^VSYNCSRCRPT", (VOS_UINT8 *)"(0,1)" },
    { AT_CMD_VSYNCMODE,
      AT_SetVsyncmode, AT_SET_PARA_TIME, AT_QryVsyncmode, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^VSYNCMODE", (VOS_UINT8 *)"(0,1)" },
    { AT_CMD_CBR,
      VOS_NULL_PTR, AT_SET_PARA_TIME, AT_QryCbr, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^CBR", VOS_NULL_PTR },

    { AT_CMD_SLINFO,
      AT_SetSideLinkInfo, AT_SET_PARA_TIME, AT_QrySideLinkInfo, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^SLINFO", (VOS_UINT8 *)"(47),(3,5),(54540-55239)" },

    { AT_CMD_PHYR,
      AT_SetPhyr, AT_SET_PARA_TIME, AT_QryPhyr, AT_QRY_PARA_TIME, AT_TestPhyr, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^PHYR", (VOS_UINT8 *)"(@para),(0-31),(0-31),(1-20),(1-20)" },

    { AT_CMD_PTRRPT,
      AT_SetPtrRpt, AT_SET_PARA_TIME, AT_QryPtrRpt, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^PTRRPT", (VOS_UINT8 *)"(1)" },

    { AT_CMD_RPPCFG,
      AT_SetV2xResPoolPara, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, AT_TestRPPCfg, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^RPPCFG", (VOS_UINT8 *)"(0,1,3),(0,1),(0-15),(0-10239),(str),(0,1),(0-18),(0-6),(0-99),(0-99)" },

    { AT_CMD_GNSSTEST,
      AT_SetGnssInfo, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, At_CmdTestProcOK, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^GNSSTEST", TAF_NULL_PTR },

    { AT_CMD_QRYDATA,
      VOS_NULL_PTR, AT_NOT_SET_TIME, AT_QryData, AT_QRY_PARA_TIME, AT_TestQryData, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^QUERYDATA", (VOS_UINT8 *)"(0,1),(0,1),(0-50)" },

    { AT_CMD_GNSSINFO,
      AT_GnssInfo, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, At_CmdTestProcOK, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^GNSSINFO", VOS_NULL_PTR },

    { AT_CMD_VPHYSTAT,
      VOS_NULL_PTR, AT_SET_PARA_TIME, AT_QryVPhyStat, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^VPHYSTAT", VOS_NULL_PTR },

    { AT_CMD_VPHYSTATCLR,
      AT_SetVPhyStatClr, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^VPHYSTATCLR", (VOS_UINT8 *)"(0)" },

    { AT_CMD_VSNRRSRP,
      VOS_NULL_PTR, AT_SET_PARA_TIME, AT_QryVSnrRsrp, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^VSNRRSRP", VOS_NULL_PTR },

    { AT_CMD_V2XRSSI,
      VOS_NULL_PTR, AT_SET_PARA_TIME, AT_QryV2xRssi, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^V2XRSSI", VOS_NULL_PTR },

    { AT_CMD_VRSSI,
      VOS_NULL_PTR, AT_NOT_SET_TIME, AT_QryVRssi, AT_SET_PARA_TIME, AT_TestVRssi, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^VRSSI", (VOS_UINT8 *)"(0-9)" },

    { AT_CMD_SENDDATA,
      AT_SendData, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, At_CmdTestProcOK, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^SENDDATA", (VOS_UINT8 *)"(0-99),(0-99),(0-17),(20,50,100,200,300,400,500,600,700,800,900,1000),(0-1023),(0,1)" },

    { AT_CMD_VTXPOWER,
      AT_SetVTxPower, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^VTXPOWER", (VOS_UINT8 *)"(@Power)" },
#if (FEATURE_LTEV_WL == FEATURE_ON)
    { AT_CMD_PC5SYNC,
      AT_SetPc5Sync, AT_SET_PARA_TIME, AT_QryPc5Sync, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^PC5SYNC", (VOS_UINT8 *)"(0,1),(0,1),(1-20),(0-159),(0-159),(0-160),(0-11),(0-14),(0-4),(0-5)" },

    { AT_CMD_MCGCFG,
      AT_SetRsuMcgCfg, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^MCGCFG", (VOS_UINT8 *)"(0-16777215),(0-16777215),(0,1),(0-16777215),(0-10),(0-16777215),(0-16777215),(0-16777215),(0-16777215),(0-16777215),(0-16777215),(0-16777215),(0-16777215),(0-16777215),(0-16777215)" },

    { AT_CMD_FILEWR,
      AT_WriteFile, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, At_CmdTestProcOK, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^FILEWR", (VOS_UINT8 *)"(str),(str)" },

    { AT_CMD_FILERD,
      AT_ReadFile, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, At_CmdTestProcOK, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^FILERD", (VOS_UINT8 *)"(str)" },

    { AT_CMD_LEDTEST,
      AT_SetLedTest, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^LEDTEST", (VOS_UINT8 *)"(0,1)" },

    { AT_CMD_GPIO_TEST,
      AT_SetGpioTest, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^GPIOTEST", VOS_NULL_PTR },

    { AT_CMD_ANTTEST,
      AT_AntTest, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^ANTTEST", VOS_NULL_PTR },

    { AT_CMD_GE_TEST,
      AT_GeTest, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^GETEST", VOS_NULL_PTR },

    { AT_CMD_SFP_TEST,
      AT_SfpTest, AT_SET_PARA_TIME, AT_QrySfpTest, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^SFPTEST", VOS_NULL_PTR },

    { AT_CMD_SFP_SWITCH,
      AT_SfpSwitch, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^SFPSWITCH", (VOS_UINT8 *)"(0,1)" },

    { AT_CMD_GPIO_HEAT_SET,
      AT_SetGpioHeatSet, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^GPIOHEATSET", (VOS_UINT8 *)"(0,1)" },

    { AT_CMD_HKADCTEST,
      AT_HkadcTest, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^HKADCTEST", (VOS_UINT8 *)"(0-15)" },

    { AT_CMD_QRYGPIO,
      AT_QryGpio, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, At_CmdTestProcOK, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^QRYGPIO", (VOS_UINT8 *)"(0-65535)" },

    { AT_CMD_SETGPIO,
      AT_SetGpio, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, At_CmdTestProcOK, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^SETGPIO", (VOS_UINT8 *)"(0-65535),(0,1)" },

    { AT_CMD_SETBOOTMODE,
      AT_SetBootMode, AT_SET_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, At_CmdTestProcOK, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^SETBOOTMODE", (VOS_UINT8 *)"(0,1)" },

    { AT_CMD_MULTIUPG_MODE,
      AT_SetMultiupgMode, AT_SET_PARA_TIME, AT_QryMultiupgMode, AT_QRY_PARA_TIME, At_CmdTestProcOK, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^MULTIUPGMODE", (VOS_UINT8 *)"(0-1)" },

    { AT_CMD_QRYTEMPPRT,
      VOS_NULL_PTR, AT_NOT_SET_TIME, At_QryRsuTempPara, AT_QRY_PARA_TIME, At_CmdTestProcOK, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^RSUTEMPPRT", VOS_NULL_PTR },

    { AT_CMD_RSUSYNCST,
      VOS_NULL_PTR, AT_NOT_SET_TIME, AT_QrySyncSt, AT_QRY_PARA_TIME, AT_TestSyncSt, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^RSUSYNCST", (VOS_UINT8 *)"(0-2),(0,1)" },

    { AT_CMD_GNSSSYNCST,
      VOS_NULL_PTR, AT_NOT_SET_TIME, AT_QryGnssSyncSt, AT_QRY_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
      VOS_NULL_PTR, AT_NOT_SET_TIME,
      AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
      (VOS_UINT8 *)"^RSUGNSSCST", VOS_NULL_PTR },

#endif
};

VOS_UINT32 AT_SndVtcMsg(MN_CLIENT_ID_T clientId, MN_OPERATION_ID_T opId, AT_VTC_MsgTypeUint32 msgName,
                        const VOS_UINT8 *data, VOS_UINT32 dataSize)
{
    AT_VTC_MsgReq *msg = VOS_NULL_PTR;
    errno_t        memResult;

    msg = (AT_VTC_MsgReq *)TAF_AllocMsgWithoutHeaderLen(WUEPS_PID_AT, sizeof(AT_VTC_MsgReq) - VOS_MSG_HEAD_LENGTH + dataSize);
    if (msg == VOS_NULL_PTR) {
        return VOS_ERR;
    }
    TAF_CfgMsgHdr((MsgBlock *)msg, WUEPS_PID_AT, I0_PS_PID_VTC, sizeof(AT_VTC_MsgReq) - VOS_MSG_HEAD_LENGTH + dataSize);
    msg->msgName  = msgName;
    msg->clientId = clientId;
    msg->opId     = opId;

    /* 入参data不为空，且dataSize不为0，说明除消息ID外，还有其他数据需要拷贝 */
    if (dataSize != 0 && data != VOS_NULL_PTR) {
        memResult = memcpy_s(msg->content, dataSize, data, dataSize);
        TAF_MEM_CHK_RTN_VAL(memResult, dataSize, dataSize);
    }
    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, msg) != VOS_OK) {
        return VOS_ERR;
    }
    return VOS_OK;
}


VOS_UINT32 At_RegisterLtevCmdTable(VOS_VOID)
{
    return AT_RegisterCmdTable(g_atLtevCmdTbl, sizeof(g_atLtevCmdTbl) / sizeof(g_atLtevCmdTbl[0]));
}


VOS_UINT32 AT_SetCatm(VOS_UINT8 indexNum)
{
    AT_VTC_TestModeActiveStatePara para = {0};
    VOS_UINT32                     result;

    if (g_atParaIndex > AT_CATM_PARA_MAX_NUM || g_atParaList[AT_CATM_PARA_STATE].paraLen == 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }
    para.state = (AT_VTC_TestModeActiveStateUint8)g_atParaList[AT_CATM_PARA_STATE].paraValue;

    /* 激活测试模式，要求携带两个参数，第二个参数指定测试模式类型 */
    if (para.state == AT_VTC_TEST_MODE_STATE_ACTIVATED) {
        if (g_atParaList[AT_CATM_PARA_TYPE].paraLen == 0) {
            return AT_CME_INCORRECT_PARAMETERS;
        }
        para.type = (AT_VTC_TestModeTypeUint8)g_atParaList[AT_CATM_PARA_TYPE].paraValue;
    }
    result = AT_SndVtcMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                          AT_VTC_TEST_MODE_ACTIVE_STATE_SET_REQ, (VOS_UINT8 *)&para,
                          sizeof(AT_VTC_TestModeActiveStatePara));
    if (result == VOS_OK) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CATM_SET;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_QryCatm(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    result = AT_SndVtcMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                          AT_VTC_TEST_MODE_ACTIVE_STATE_QRY_REQ, VOS_NULL_PTR, 0);
    if (result == VOS_OK) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CATM_QRY;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_SetCcutle(VOS_UINT8 indexNum)
{
    AT_VTC_TestModeECloseStatePara para = {0};
    errno_t                        memResult;
    VOS_UINT32                     result;

    if (g_atParaIndex > AT_CCUTLE_PARA_MAX_NUM || g_atParaList[AT_CCUTLE_TEST_MODE_CLOSE_STATUS].paraLen == 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }
    para.status = (AT_VTC_TestModeCloseStatusUint8)g_atParaList[AT_CCUTLE_TEST_MODE_CLOSE_STATUS].paraValue;
    if (para.status == AT_VTC_TEST_MODE_STATUS_CLOSE) {
        if (g_atParaList[AT_CCUTLE_COMMUNICATION_DIRECTION].paraLen == 0) {
            return AT_CME_INCORRECT_PARAMETERS;
        }
        para.direction = (AT_VTC_CommunicationDirectionUint8)g_atParaList[AT_CCUTLE_COMMUNICATION_DIRECTION].paraValue;
        if (para.direction == AT_VTC_COMMUNICATION_DIRECTION_RECEIVE) {
            if (g_atParaList[AT_CCUTLE_MONITOR_LIST_FORMAT].paraLen == 0 ||
                g_atParaList[AT_CCUTLE_MONITOR_LIST_LEN].paraLen == 0) {
                return AT_CME_INCORRECT_PARAMETERS;
            }
            para.format = (AT_VTC_MonitorListFormatUint8)g_atParaList[AT_CCUTLE_MONITOR_LIST_FORMAT].paraValue;
            para.length = g_atParaList[AT_CCUTLE_MONITOR_LIST_LEN].paraValue;
            /* 第四个参数描述Layer-2 ID数量，由于每个Layer-2 ID占用6个字符，所以第四个参数和第五个参数存在6倍关系 */
            if (g_atParaList[AT_CCUTLE_MONITOR_LIST_CONTEXT].paraLen !=
                para.length * AT_CCUTLE_MONITOR_LIST_CONTEXT_TO_ID) {
                return AT_CME_INCORRECT_PARAMETERS;
            }
            memResult = memcpy_s(para.monitorList, sizeof(para.monitorList),
                                 g_atParaList[AT_CCUTLE_MONITOR_LIST_CONTEXT].para,
                                 g_atParaList[AT_CCUTLE_MONITOR_LIST_CONTEXT].paraLen);
            TAF_MEM_CHK_RTN_VAL(memResult, sizeof(para.monitorList), g_atParaList[AT_CCUTLE_MONITOR_LIST_CONTEXT].paraLen);
        }
    }
    result = AT_SndVtcMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                          AT_VTC_TEST_LOOP_MODE_E_CLOSE_STATE_SET_REQ, (VOS_UINT8 *)&para,
                          sizeof(AT_VTC_TestModeECloseStatePara));
    if (result == VOS_OK) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CCUTLE_SET;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}

VOS_UINT32 AT_QryCcutle(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    result = AT_SndVtcMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                          AT_VTC_TEST_LOOP_MODE_E_CLOSE_STATE_QRY_REQ, VOS_NULL_PTR, 0);
    if (result == VOS_OK) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CCUTLE_QRY;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}

VOS_UINT32 AT_TestCcutle(VOS_UINT8 indexNum)
{
    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: (0,1),(0,1),(1),(16)", g_parseContext[indexNum].cmdElement->cmdName);
    return AT_OK;
}


VOS_UINT32 AT_SetCuspcreq(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    /* AT命令格式没有问号，但根据协议实际是一个查询命令 */
    result = AT_SndVtcMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                          AT_VTC_SIDELINK_PACKET_COUNTER_QRY_REQ, VOS_NULL_PTR, 0);
    if (result == VOS_OK) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CUSPCREQ_QRY;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_SetCutcr(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    result = AT_SndVtcMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId, AT_VTC_RESET_UTC_TIME_SET_REQ,
                          VOS_NULL_PTR, 0);
    if (result == VOS_OK) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CUTCR_SET;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_SetCcbrreq(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    /* AT命令格式没有问号，但根据协议实际是一个查询命令 */
    result = AT_SndVtcMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId, AT_VTC_CBR_QRY_REQ,
                          VOS_NULL_PTR, 0);
    if (result == VOS_OK) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CCBRREQ_QRY;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_QryCbr(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId, AT_VMAC_CBR_QRY_REQ,
                                    VOS_NULL_PTR, 0, I0_PS_PID_VMAC_UL);
    if (result == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CBR_QRY;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_SetCv2xdts(VOS_UINT8 indexNum)
{
    AT_VTC_SendingDataActionPara para = {0};
    VOS_UINT32                   result;

    if (g_atParaIndex > AT_CV2XDTS_PARA_MAX_NUM || g_atParaList[AT_CV2XDTS_SEND_DATA_ACTION].paraLen == 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }
    para.action = (AT_VTC_SendingDataActionUint8)g_atParaList[AT_CV2XDTS_SEND_DATA_ACTION].paraValue;
    if (para.action == AT_VTC_SENDING_DATA_ACTION_START) {
        if (g_atParaList[AT_CV2XDTS_SEND_DATA_SIZE].paraLen == 0 ||
            g_atParaList[AT_CV2XDTS_SEND_DATA_PERIODICITY].paraLen == 0) {
            return AT_CME_INCORRECT_PARAMETERS;
        }
        para.dataSize    = g_atParaList[AT_CV2XDTS_SEND_DATA_SIZE].paraValue;
        para.periodicity = g_atParaList[AT_CV2XDTS_SEND_DATA_PERIODICITY].paraValue;
    }
    result = AT_SndVtcMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                          AT_VTC_SENDING_DATA_ACTION_SET_REQ, (VOS_UINT8 *)&para, sizeof(AT_VTC_SendingDataActionPara));
    if (result == VOS_OK) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CV2XDTS_SET;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_QryCv2xdts(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    result = AT_SndVtcMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                          AT_VTC_SENDING_DATA_ACTION_QRY_REQ, VOS_NULL_PTR, 0);
    if (result == VOS_OK) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CV2XDTS_QRY;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}

VOS_UINT32 AT_TestCv2xdts(VOS_UINT8 indexNum)
{
    g_atSendDataBuff.bufLen = (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: (0,1),(8176),(10)", g_parseContext[indexNum].cmdElement->cmdName);
    return AT_OK;
}

VOS_UINT32 AT_QryVsyncsrc(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    AT_VRRC_SYNC_SOURCE_QRY_REQ, VOS_NULL_PTR, 0, I0_PS_PID_VERRC);
    if (result == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_VSYNCSRC_QRY;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_SetVsyncsrcrpt(VOS_UINT8 indexNum)
{
    AT_VRRC_SyncSourceRptSwitchPara para = {0};
    VOS_UINT32                      result;

    if (g_atParaIndex > AT_VSYNCSRCRPT_PARA_MAX_NUM || g_atParaList[AT_VSYNCSRCRPT_PARA_STATUS].paraLen == 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }
    para.status = (AT_VRRC_SyncSourceRptSwitchUint8)g_atParaList[AT_VSYNCSRCRPT_PARA_STATUS].paraValue;
    result      = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
        AT_VRRC_SYNC_SOURCE_RPT_SET_REQ, (VOS_UINT8 *)&para, sizeof(para), I0_PS_PID_VERRC);
    if (result == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_VSYNCSRCRPT_SET;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_QryVsyncsrcrpt(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    AT_VRRC_SYNC_SOURCE_RPT_QRY_REQ, VOS_NULL_PTR, 0, I0_PS_PID_VERRC);
    if (result == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_VSYNCSRCRPT_QRY;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_SetVsyncmode(VOS_UINT8 indexNum)
{
    AT_VRRC_SyncModePara para = {0};
    VOS_UINT32           result;

    if (g_atParaIndex > AT_VSYNCMODE_PARA_MAX_NUM || g_atParaList[AT_VSYNCMODE_PARA_MODE].paraLen == 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }
    para.mode = (AT_VRRC_SyncModeUint8)g_atParaList[AT_VSYNCMODE_PARA_MODE].paraValue;
    result    = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
        AT_VRRC_SYNC_MODE_SET_REQ, (VOS_UINT8 *)&para, sizeof(para), I0_PS_PID_VERRC);
    if (result == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_VSYNCMODE_SET;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_QryVsyncmode(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                    AT_VRRC_SYNC_MODE_QRY_REQ, VOS_NULL_PTR, 0, I0_PS_PID_VERRC);
    if (result == TAF_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_VSYNCMODE_QRY;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_SetSideLinkInfo(VOS_UINT8 indexNum)
{
    VOS_UINT32     ret;
    AT_VRRC_Slinfo params;
    errno_t        memResult;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数数量错误 */
    if (g_atParaIndex != AT_SLINFO_PARA_MAX_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数为空 */
    if ((g_atParaList[AT_SLINFO_FREQUENCY_BAND_INDEX].paraLen == 0) ||
        (g_atParaList[AT_SLINFO_BAND_WIDTHh_INDEX].paraLen == 0) ||
        (g_atParaList[AT_SLINFO_CENTRAL_FREQUENCY_INDEX].paraLen == 0)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    memResult = memset_s(&params, sizeof(params), 0, sizeof(AT_VRRC_Slinfo));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(params), sizeof(AT_VRRC_Slinfo));

    params.frequencyBand    = g_atParaList[AT_SLINFO_FREQUENCY_BAND_INDEX].paraValue;
    params.bandWidth        = g_atParaList[AT_SLINFO_BAND_WIDTHh_INDEX].paraValue;
    params.centralFrequency = g_atParaList[AT_SLINFO_CENTRAL_FREQUENCY_INDEX].paraValue;
    /* 执行命令操作 */
    ret = VRRC_SetSideLinkInfo(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId, &params);
    if (ret != VOS_OK) {
        AT_WARN_LOG("AT_SetSideLinkInfo AT_ERROR\n");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_SLINFO_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QrySideLinkInfo(VOS_UINT8 indexNum)
{
    VOS_UINT32 ret;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    ret = VRRC_QrySideLinkInfo(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId);
    if (ret != VOS_OK) {
        AT_WARN_LOG("AT_QrySideLinkInfo AT_ERROR\n");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_SLINFO_QRY;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_SetPhyr(VOS_UINT8 indexNum)
{
    VOS_UINT32      rst;
    AT_VRRC_PhyrSet params;
    VOS_INT32       value = 0;
    errno_t         memResult;

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数检查 */
    if (g_atParaIndex != AT_PHYR_PARA_MAX_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }
    AT_AtoInt((VOS_CHAR *)g_atParaList[AT_PHYR_MAX_TXPWR_INDEX].para, &value);

    if (value > AT_SET_PHYR_TX_POWER_MAX || value < AT_SET_PHYR_TX_POWER_MIN) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    memResult = memset_s(&params, sizeof(params), 0, sizeof(AT_VRRC_PhyrSet));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(params), sizeof(AT_VRRC_PhyrSet));

    params.maxTxPwr  = (VOS_INT8)value;
    params.minMcs    = (VOS_UINT8)g_atParaList[AT_PHYR_MIN_MCS_INDEX].paraValue;
    params.maxMcs    = (VOS_UINT8)g_atParaList[AT_PHYR_MAX_MCS_INDEX].paraValue;
    params.minSubChn = (VOS_UINT8)g_atParaList[AT_PHYR_MIN_SUBCHN_INDEX].paraValue;
    params.maxSubChn = (VOS_UINT8)g_atParaList[AT_PHYR_MAX_SUBCHN_INDEX].paraValue;

    if ((params.minMcs > params.maxMcs) || (params.minSubChn > params.maxSubChn)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }
    rst = VRRC_SetRsuPhyr(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId, &params);

    if (rst != VOS_OK) {
        AT_WARN_LOG("AT_SetPhyr AT_ERROR\n");
        return AT_ERROR;
    }
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_PHYP_SET;
    return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
}


VOS_UINT32 AT_QryPhyr(VOS_UINT8 indexNum)
{
    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (VRRC_QryRsuPhyr(g_atClientTab[indexNum].clientId, 0) == VOS_OK) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_PHYP_QRY;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    }
    AT_WARN_LOG("AT_QryPhpy AT_ERROR\n");
    return AT_ERROR;
}


VOS_UINT32 AT_TestPhyr(VOS_UINT8 indexNum)
{
    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_TEST_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: (-30-23),(0-31),(0-31),(1-20),(1-20)",
        g_parseContext[indexNum].cmdElement->cmdName);

    return AT_OK;
}


VOS_UINT32 AT_SetPtrRpt(VOS_UINT8 indexNum)
{
    VOS_UINT32 rst;

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数过多 */
    if (g_atParaIndex != AT_PTRRPT_PARA_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaList[0].paraLen == 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 执行命令操作 */
    rst = VPDCP_SetPtrRpt(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId);

    if (rst != VOS_OK) {
        return AT_ERROR;
    }
    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_PTRRPT_SET;
    return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
}


VOS_UINT32 AT_QryPtrRpt(VOS_UINT8 indexNum)
{
    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (VPDCP_QryPtrRpt(g_atClientTab[indexNum].clientId, 0) == VOS_OK) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_PTRRPT_QRY;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    }

    AT_WARN_LOG("AT_QryPtrRpt AT_ERROR\n");
    return AT_ERROR;
}


VOS_UINT32 AT_QryV2xResPoolPara(VOS_UINT8 indexNum)
{
    VOS_UINT32        ret;
    AT_VRRC_RppcfgQry params;
    errno_t           memResult;

    memResult = memset_s(&params, sizeof(params), 0, sizeof(AT_VRRC_RppcfgQry));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(params), sizeof(AT_VRRC_RppcfgQry));

    params.groupType = g_atParaList[AT_RPPCFG_GROUP_TYPE_INDEX].paraValue;
    params.rtType    = g_atParaList[AT_RPPCFG_RT_TYPE_INDEX].paraValue;

    ret = VRRC_QryV2xResPoolPara(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId, &params);
    if (ret != VOS_OK) {
        AT_WARN_LOG("AT_QryV2xResPoolPara AT_ERROR\n");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_RPPCFG_QRY;

    return AT_WAIT_ASYNC_RETURN;
}

TAF_UINT32 At_AsciiNum2Uint8(TAF_UINT8 *src, TAF_UINT16 *srcLen)
{
    TAF_UINT16 chkLen = 0;
    TAF_UINT16 tmp    = 0;
    TAF_UINT16 mod    = 0;
    TAF_UINT8  left   = 0;
    TAF_UINT8  value  = 0;
    TAF_UINT16 len    = *srcLen;
    TAF_UINT8 *dst    = src;

    while (chkLen < len) {
        if ((chkLen != 0) && (mod % AT_UINT_BYTES_LEN) == 0) {
            tmp++;
            mod   = 0;
            value = 0;
        }

        /* 字符串只会是0或1组成的 */
        if ((src[chkLen] == '0') || (src[chkLen] == '1')) {
            left = src[chkLen] - '0';
        } else {
            return AT_FAILURE;
        }

        value |= (TAF_UINT8)(left << (AT_UINT_DISPLACE_MAX - mod));
        dst[tmp] = value;
        mod++;
        chkLen++;
    }

    *srcLen = (0 == len % AT_UINT_BYTES_LEN) ? (len / AT_UINT_BYTES_LEN) : (len / AT_UINT_BYTES_LEN + 1);
    return AT_SUCCESS;
}

VOS_VOID AT_InitV2xRespoolParams(AT_VRRC_RppcfgSet *params)
{
    params->groupType         = g_atParaList[AT_RPPCFG_GROUP_TYPE_INDEX].paraValue;
    params->rtType            = g_atParaList[AT_RPPCFG_RT_TYPE_INDEX].paraValue;
    params->poolId            = g_atParaList[AT_RPPCFG_POOL_ID_INDEX].paraValue;
    params->slOffsetIndicator = g_atParaList[AT_RPPCFG_SL_OFFSET_INDEX].paraValue;

    params->adjacency        = g_atParaList[AT_RPPCFG_ADJACENCY_INDEX].paraValue;
    params->subChnSize       = g_atParaList[AT_RPPCFG_SUB_CHN_SIZE_INDEX].paraValue;
    params->subChnNum        = g_atParaList[AT_RPPCFG_SUB_CHN_NUM_INDEX].paraValue;
    params->subChnStartRB    = g_atParaList[AT_RPPCFG_SUB_CHN_STARTRB_INDEX].paraValue;
    params->pscchPoolStartRB = g_atParaList[AT_RPPCFG_PSCCH_POOL_STARTRB_INDEX].paraValue;
    params->poolNum          = AT_RPPCFG_GROUP_RESPOOL_NUM;

    params->bitmapLen = (VOS_UINT8)g_atParaList[AT_RPPCFG_SUB_CHAN_BITMAP_INDEX].paraLen;
    return;
}

VOS_UINT32 AT_SetV2xResPoolPara(VOS_UINT8 indexNum)
{
    VOS_UINT32        ret;
    AT_VRRC_RppcfgSet params;
    errno_t           memResult;

    if (g_atParaIndex == AT_RPPCFG_QRY_PARAM_NUM) {
        return AT_QryV2xResPoolPara(indexNum);
    }
    if (g_atParaIndex != AT_RPPCFG_SET_PARAM_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }
    memResult = memset_s(&params, sizeof(params), 0, sizeof(AT_VRRC_RppcfgSet));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(params), sizeof(AT_VRRC_RppcfgSet));

    if (g_atParaList[AT_RPPCFG_GROUP_TYPE_INDEX].paraValue == AT_RPPCFG_CLEAR_RESPOOL_CMD) {
        params.groupType = g_atParaList[AT_RPPCFG_GROUP_TYPE_INDEX].paraValue;
        ret = VRRC_SetV2xResPoolPara(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId, &params);
        if (ret != VOS_OK) {
            return AT_ERROR;
        }
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_RPPCFG_SET;
        return AT_WAIT_ASYNC_RETURN;
    }

    AT_InitV2xRespoolParams(&params);
    if ((params.bitmapLen != AT_RPPCFG_BITMAP_LEN_MIN_VALUE) && (params.bitmapLen != AT_RPPCFG_BITMAP_LEN_MID_VALUE) &&
        (params.bitmapLen != AT_RPPCFG_BITMAP_LEN_MAX_VALUE)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (((params.rtType == 0) && (params.poolId >= AT_RPPCFG_GROUP_RESPOOL_NUM)) ||
        ((params.rtType == 1) && (params.poolId >= AT_RPPCFG_GROUP_RESPOOL_NUM_SUM))) {
        return AT_ERROR;
    }
    if (AT_FAILURE == At_AsciiNum2Uint8(g_atParaList[AT_RPPCFG_SUB_CHAN_BITMAP_INDEX].para,
                                        &(g_atParaList[AT_RPPCFG_SUB_CHAN_BITMAP_INDEX].paraLen))) {
        return AT_ERROR;
    }
    memResult = memcpy_s(&(params.subChanBitmap[0]), (VOS_SIZE_T)sizeof(params.subChanBitmap) / sizeof(VOS_UINT8),
                         g_atParaList[AT_RPPCFG_SUB_CHAN_BITMAP_INDEX].para,
                         g_atParaList[AT_RPPCFG_SUB_CHAN_BITMAP_INDEX].paraLen);
    TAF_MEM_CHK_RTN_VAL(memResult, (VOS_SIZE_T)sizeof(params.subChanBitmap) / sizeof(VOS_UINT8),
                        g_atParaList[AT_RPPCFG_SUB_CHAN_BITMAP_INDEX].paraLen);
    /* 执行命令操作 */
    ret = VRRC_SetV2xResPoolPara(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId, &params);
    if (ret != VOS_OK) {
        AT_WARN_LOG("AT_SetV2xResPoolPara AT_ERROR\n");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_RPPCFG_SET;
    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_TestRPPCfg(VOS_UINT8 indexNum)
{
    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_TEST_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    g_atSendDataBuff.bufLen =
        (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress,
            "%s: (0,1,3),(0,1),(0-15),(0-10239),(str),(0,1),(0-18),(0-6),(0-99),(0-99)",
            g_parseContext[indexNum].cmdElement->cmdName);

    return AT_OK;
}


VOS_UINT32 AT_SetGnssInfo(VOS_UINT8 indexNum)
{
    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_CMD_NO_PARA) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (VRRC_SetGnssInfo(g_atClientTab[indexNum].clientId, 0) == VOS_OK) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_GNSS_SET;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    } else {
        AT_WARN_LOG("AT_QryGnssInfo AT_ERROR\n");
        return AT_ERROR;
    }
}


VOS_UINT32 AT_QryData(VOS_UINT8 indexNum)
{
    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (VRRC_QryGnssInfo(g_atClientTab[indexNum].clientId, 0) == VOS_OK) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_DATA_QRY;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    }

    AT_WARN_LOG("AT_QryGnssInfo AT_ERROR\n");
    return AT_ERROR;
}


VOS_UINT32 AT_TestQryData(VOS_UINT8 indexNum)
{
    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_TEST_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: (0,1),(0,1),(0,1)", g_parseContext[indexNum].cmdElement->cmdName);

    return AT_OK;
}


VOS_UINT32 AT_GnssInfo(VOS_UINT8 indexNum)
{
    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_CMD_NO_PARA) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (VRRC_GetGnssInfo(g_atClientTab[indexNum].clientId, 0) == VOS_OK) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_GNSSINFO_QRY;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    } else {
        AT_WARN_LOG("AT_GnssInfo AT_ERROR\n");
        return AT_ERROR;
    }
}


VOS_UINT32 AT_QryVPhyStat(VOS_UINT8 indexNum)
{
    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (VRRC_QryRsuVphyStat(g_atClientTab[indexNum].clientId, 0) == VOS_OK) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_VPHYSTAT_QRY;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    } else {
        AT_WARN_LOG("AT_QryVPhyStat: AT_ERROR!\n");
        return AT_ERROR;
    }
}


VOS_UINT32 AT_SetVPhyStatClr(VOS_UINT8 indexNum)
{
    VOS_UINT32             rst;
    AT_VRRC_VphystatClrSet params;
    errno_t                memResult;

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数检查 */
    if (g_atParaIndex != AT_VPHYSTATCLR_PARA_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    memResult = memset_s(&params, sizeof(params), 0, sizeof(AT_VRRC_VphystatClrSet));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(params), sizeof(AT_VRRC_VphystatClrSet));
    params.vphyStatClrFlg = (VOS_UINT8)g_atParaList[AT_VPHYSTATCLR_PARA_VPHY_STAT_CLR_FLG].paraValue;
    rst = VRRC_SetRsuVPhyStatClr(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId, &params);

    if (rst == VOS_OK) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_VPHYSTATCLR_SET;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_QryVSnrRsrp(VOS_UINT8 indexNum)
{
    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (VRRC_QryRsuVSnrRsrp(g_atClientTab[indexNum].clientId, 0) == VOS_OK) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_VSNRRSRP_QRY;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_QryV2xRssi(VOS_UINT8 indexNum)
{
    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    if (VRRC_QryRsuV2xRssi(g_atClientTab[indexNum].clientId, 0) == VOS_OK) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_V2XRSSI_QRY;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    } else {
        AT_WARN_LOG("AT_QryV2xRssi AT_ERROR!\n");
        return AT_ERROR;
    }
}


VOS_UINT32 AT_QryVRssi(VOS_UINT8 indexNum)
{
    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    if (VMAC_QryVRssi(g_atClientTab[indexNum].clientId, 0) == VOS_OK) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_VRSSI_QRY;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    }

    AT_WARN_LOG("AT_QryVRssi AT_ERROR\n");
    return AT_ERROR;
}


VOS_UINT32 AT_TestVRssi(VOS_UINT8 indexNum)
{
    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_TEST_CMD) {
        return AT_ERROR;
    }

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: (0-9)", g_parseContext[indexNum].cmdElement->cmdName);

    return AT_OK;
}


VOS_UINT32 AT_SendData(VOS_UINT8 indexNum)
{
    VOS_UINT32              ret;
    AT_VTC_SendVMacDataPara params;
    errno_t                 memResult;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数数量错误 */
    if (g_atParaIndex != AT_SENDDATA_PARAM_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数为空 */
    if ((g_atParaList[AT_SENDDATA_SARB_START_INDEX].paraLen) == 0 ||
        (g_atParaList[AT_SENDDATA_DARB_NUM_INDEX].paraLen == 0) || (g_atParaList[AT_SENDDATA_MCS_INDEX].paraLen) == 0 ||
        (g_atParaList[AT_SENDDATA_PERIOD_INDEX].paraLen == 0) ||
        (g_atParaList[AT_SENDDATA_SEND_FRMMASK_INDEX].paraLen) == 0 ||
        (g_atParaList[AT_SENDDATA_FLAG_INDEX].paraLen == 0)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    memResult = memset_s(&params, sizeof(params), 0, sizeof(AT_VTC_SendVMacDataPara));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(params), sizeof(AT_VTC_SendVMacDataPara));

    params.saRbStart   = (VOS_UINT8)g_atParaList[AT_SENDDATA_SARB_START_INDEX].paraValue;
    params.daRbNum     = (VOS_UINT8)g_atParaList[AT_SENDDATA_DARB_NUM_INDEX].paraValue;
    params.mcs         = (VOS_UINT8)g_atParaList[AT_SENDDATA_MCS_INDEX].paraValue;
    params.period      = (VOS_UINT16)g_atParaList[AT_SENDDATA_PERIOD_INDEX].paraValue;
    params.sendFrmMask = (VOS_UINT16)g_atParaList[AT_SENDDATA_SEND_FRMMASK_INDEX].paraValue;
    params.flag        = (VOS_UINT8)g_atParaList[AT_SENDDATA_FLAG_INDEX].paraValue;

    /* 执行命令操作 */
    ret = AT_SndVtcMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId, AT_VTC_PC5_SEND_DATA_REQ,
                       (VOS_UINT8 *)&params, sizeof(AT_VTC_SendVMacDataPara));
    if (ret != VOS_OK) {
        AT_WARN_LOG("AT_SendData AT_ERROR\n");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_SENDDATA_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_SetCv2xL2Id(VOS_UINT8 indexNum)
{
    AT_VTC_TestModeModifyL2IdPara para = {0};
    VOS_UINT32 result;

    if (g_atParaIndex != AT_CV2XL2ID_PARA_NUM || g_atParaList[0].paraLen == 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }
    para.srcId = g_atParaList[0].paraValue;
    para.dstId = g_atParaList[1].paraValue;

    result = AT_SndVtcMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                          AT_VTC_LAYER_TWO_ID_SET_REQ, (VOS_UINT8 *)&para,
                          sizeof(AT_VTC_TestModeModifyL2IdPara));
    if (result == VOS_OK) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CV2XL2ID_SET;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_QryCv2xL2Id(VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    result = AT_SndVtcMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                          AT_VTC_LAYER_TWO_ID_QRY_REQ, VOS_NULL_PTR, 0);
    if (result == VOS_OK) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CV2XL2ID_QRY;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_GetValidTxPowerParam(VOS_INT16 * dbmNum)
{
    VOS_UINT32 cellPower = 0;

    /* 功率长度大于7或参数为空，表示无效参数 */
    if ((g_atParaList[0].paraLen > AT_TX_POWER_PARA_LEN_MAX) || (g_atParaList[0].paraLen == 0)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 字符串参数为'-'，表示输入为负数 */
    if (g_atParaList[0].para[0] == '-') {
        if (atAuc2ul(&g_atParaList[0].para[1], g_atParaList[0].paraLen - 1, &cellPower) != AT_SUCCESS) {
            return AT_CME_INCORRECT_PARAMETERS;
        }

        /* 由于功率值为负，所以转换下 */
        *dbmNum = -(VOS_INT16)cellPower;
    }else {
        if (atAuc2ul(&g_atParaList[0].para[0], g_atParaList[0].paraLen, &cellPower) != AT_SUCCESS) {
            return AT_CME_INCORRECT_PARAMETERS;
        }

        *dbmNum = (VOS_INT16)cellPower;
    }

    /* LTE-V输出功率范围为[-62dBm, 30dBm], 按照0.125dBm精度控制，dbmNum的范围为[-496, 240] */
    if ((*dbmNum > AT_SET_TX_POWER_MAX) || (*dbmNum < AT_SET_TX_POWER_MIN)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    return AT_OK;
}


VOS_UINT32 AT_GetValidPowerPara(AT_VRRC_Set_Tx_PowerPara * powerSetPara)
{
    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数数量错误 */
    if (g_atParaIndex != 1) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 从AT命令中获取输出功率 */
    if (AT_GetValidTxPowerParam(&(powerSetPara->setTxPower)) != AT_OK) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    return AT_OK;
}


VOS_UINT32 AT_SetVTxPower(VOS_UINT8 indexNum)
{
    VOS_UINT32                          rst;
    AT_VRRC_Set_Tx_PowerPara            params;
    errno_t                             memResult;

    memResult = memset_s(&params, sizeof(params), 0, sizeof(AT_VRRC_Set_Tx_PowerPara));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(params), sizeof(AT_VRRC_Set_Tx_PowerPara));

    /* 取出AT命令中的数值，正数负数都有 */
    rst = AT_GetValidPowerPara(&params);
    if (rst != AT_OK) {
        AT_WARN_LOG("AT_SetVTxPower INCORRECT_PARAMETERS\n");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    rst = VRRC_SetTxPower(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId, &params);
    if (rst != VOS_OK) {
        AT_WARN_LOG("AT_SetVTxPower AT_ERROR\n");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_VTXPOWER_SET;
    return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
}

#if (FEATURE_LTEV_WL == FEATURE_ON)

VOS_UINT32 AT_SetPc5Sync(VOS_UINT8 indexNum)
{
    VOS_UINT32         ret;
    AT_VRRC_Pc5syncSet params;
    errno_t            memResult;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数数量错误 */
    if (g_atParaIndex > AT_PC5SYNC_PARA_MAX_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    memResult = memset_s(&params, sizeof(params), 0, sizeof(AT_VRRC_Pc5syncSet));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(params), sizeof(AT_VRRC_Pc5syncSet));

    params.syncSwitch    = (VOS_UINT8)g_atParaList[AT_PC5SYNC_PARA_SYNC_SWITCH].paraValue;
    params.syncDfnSwitch = (VOS_UINT8)g_atParaList[AT_PC5SYNC_PARA_SYNC_DFN_SWITCH].paraValue;
    if ((params.syncSwitch == AT_PC5SYNC_PARA_SYNC_SWITCH_INVALID_VAL) &&
        (params.syncDfnSwitch == AT_PC5SYNC_PARA_SYNC_DFN_SWITCH_INVALID_VAL)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }
    params.gnssValidTimerLen = (VOS_UINT8)g_atParaList[AT_PC5SYNC_PARA_GNSS_VALID_TIMER_LEN].paraValue;
    params.syncOffsetInd1    = (VOS_UINT8)g_atParaList[AT_PC5SYNC_PARA_SYNC_OFFSET_IND1].paraValue;
    params.syncOffsetInd2    = (VOS_UINT8)g_atParaList[AT_PC5SYNC_PARA_SYNC_OFFSET_IND2].paraValue;
    params.syncOffsetInd3    = (VOS_UINT8)g_atParaList[AT_PC5SYNC_PARA_SYNC_OFFSET_IND3].paraValue;
    params.syncTxThreshOoc   = (VOS_UINT8)g_atParaList[AT_PC5SYNC_PARA_SYNC_TX_THRESH_OOC].paraValue;
    params.fltCoefficient    = (VOS_UINT8)g_atParaList[AT_PC5SYNC_PARA_FLT_COEFFICIENT].paraValue;
    params.syncRefMinHyst    = (VOS_UINT8)g_atParaList[AT_PC5SYNC_PARA_SYNC_REF_MIN_HYST].paraValue;
    params.syncRefDiffHyst   = (VOS_UINT8)g_atParaList[AT_PC5SYNC_PARA_SYNC_REF_DIFF_HYST].paraValue;

    /* 执行命令操作 */
    ret = VRRC_SYNC_SetCfgPara(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId, &params);
    if (ret != VOS_OK) {
        AT_WARN_LOG("AT_SetPc5Sync AT_ERROR\n");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_PC5SYNC_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_QryPc5Sync(VOS_UINT8 indexNum)
{
    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (VRRC_SYNC_QryCfgPara(g_atClientTab[indexNum].clientId, 0) == VOS_OK) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_PC5SYNC_QRY;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    }

    AT_WARN_LOG("AT_QryPc5Sync AT_ERROR\n");
    return AT_ERROR;
}


VOS_UINT32 AT_SetRsuMcgCfg(VOS_UINT8 indexNum)
{
    VOS_UINT32        ret;
    VOS_UINT32        loop, tmpIdx;
    AT_VMAC_McgCfgSet params;
    errno_t           memResult;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数数量错误 */
    if (g_atParaIndex > AT_MCGCFG_PARA_MAX_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    memResult = memset_s(&params, sizeof(params), 0, sizeof(AT_VMAC_McgCfgSet));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(params), sizeof(AT_VMAC_McgCfgSet));

    params.groupId     = g_atParaList[AT_MCGCFG_PARA_GROUP_ID].paraValue;
    params.selfRsuId   = g_atParaList[AT_MCGCFG_PARA_SELF_RSU_ID].paraValue;
    params.selfRsuType = g_atParaList[AT_MCGCFG_PARA_SELF_RSU_TYPE].paraValue;
    params.indoorRsuId = g_atParaList[AT_MCGCFG_PARA_INDOOR_RSU_ID].paraValue;
    params.rsuListNum  = g_atParaList[AT_MCGCFG_PARA_RSU_LIST_NUM].paraValue;

    tmpIdx = AT_MCGCFG_PARA_RSU_ID_LIST;
    for (loop = 0; loop < params.rsuListNum; loop++) {
        params.rsuIdList[loop] = g_atParaList[tmpIdx].paraValue;
        tmpIdx++;
    }

    /* 执行命令操作 */
    ret = VMAC_SetMcgCfgPara(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId, &params);
    if (ret != VOS_OK) {
        AT_WARN_LOG("AT_SetRsuMcgCfg AT_ERROR\n");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_MCGCFG_SET;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_WriteFile(VOS_UINT8 indexNum)
{
    VOS_UINT32       rst;
    AT_MTA_FileWrReq atCmd;
    errno_t          memResult;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数检查 */
    if (g_atParaIndex != AT_FILEWR_PARA_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数内容检查 */
    if (g_atParaList[AT_FILEWR_PARA_AINDEX].paraLen == 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    memResult = memset_s(&atCmd, sizeof(atCmd), 0x00, sizeof(AT_MTA_FileWrReq));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(atCmd), sizeof(AT_MTA_FileWrReq));

    if (g_atParaList[AT_FILEWR_PARA_AINDEX].paraLen != 0) {
        memResult = memcpy_s(atCmd.index, sizeof(atCmd.index), g_atParaList[AT_FILEWR_PARA_AINDEX].para,
                             g_atParaList[AT_FILEWR_PARA_AINDEX].paraLen);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(atCmd.index), g_atParaList[AT_FILEWR_PARA_AINDEX].paraLen);
    }

    if (g_atParaList[AT_FILEWR_PARA_INFO].paraLen != 0) {
        memResult = memcpy_s(atCmd.info, sizeof(atCmd.info), g_atParaList[AT_FILEWR_PARA_INFO].para,
                             g_atParaList[AT_FILEWR_PARA_INFO].paraLen);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(atCmd.info), g_atParaList[AT_FILEWR_PARA_INFO].paraLen);
    }

    atCmd.indexLen = g_atParaList[AT_FILEWR_PARA_AINDEX].paraLen;
    atCmd.infoLen  = g_atParaList[AT_FILEWR_PARA_INFO].paraLen;

    /* 发送消息 ID_AT_MTA_FILE_WRITE_REQ 给 AT MTA 处理 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, At_GetOpId(), ID_AT_MTA_FILE_WRITE_REQ,
                                 (VOS_VOID *)&atCmd, sizeof(AT_MTA_FileWrReq), I0_UEPS_PID_MTA);

    if (rst == AT_SUCCESS) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_FILE_WRITE;
        return AT_WAIT_ASYNC_RETURN;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_ReadFile(VOS_UINT8 indexNum)
{
    VOS_UINT32       rst;
    AT_MTA_FileRdReq atCmd;
    errno_t          memResult;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数检查 */
    if (g_atParaIndex != AT_FILERD_PARA_NUM) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数内容检查 */
    if (g_atParaList[AT_FILERD_PARA_AINDEX].paraLen == 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    memResult = memset_s(&atCmd, sizeof(atCmd), 0x00, sizeof(AT_MTA_FileRdReq));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(atCmd), sizeof(AT_MTA_FileRdReq));

    if (g_atParaList[AT_FILERD_PARA_AINDEX].paraLen != 0) {
        memResult = memcpy_s(atCmd.index, sizeof(atCmd.index), g_atParaList[AT_FILERD_PARA_AINDEX].para,
                             g_atParaList[AT_FILERD_PARA_AINDEX].paraLen);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(atCmd.index), g_atParaList[AT_FILERD_PARA_AINDEX].paraLen);
    }

    atCmd.indexLen = g_atParaList[AT_FILERD_PARA_AINDEX].paraLen;

    /* 发送消息 ID_AT_MTA_FILE_READ_REQ 给 AT MTA 处理 */
    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, At_GetOpId(), ID_AT_MTA_FILE_READ_REQ,
                                 (VOS_VOID *)&atCmd, sizeof(AT_MTA_FileRdReq), I0_UEPS_PID_MTA);

    if (rst == AT_SUCCESS) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_FILE_READ;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    } else {
        return AT_ERROR;
    }
}


VOS_VOID AT_RcvTiLedTestExpired(REL_TimerMsgBlock *pstMsg)
{
    VOS_UINT32              rst;
    VOS_UINT8               indexNum;
    DRV_AGENT_LedTestSetReq ledTest;

    ledTest.ledTest = (VOS_UINT8)g_atParaList[AT_LEDTEST_PARA_LEDTEST].paraValue;

    indexNum = (VOS_UINT8)((pstMsg->name) >> 12);

    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 DRV_AGENT_LEDTEST_SET_REQ, (VOS_UINT8 *)&ledTest, sizeof(DRV_AGENT_LedTestSetReq),
                                 I0_WUEPS_PID_DRV_AGENT);

    if (rst == VOS_OK) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_LEDTEST_SET;
    } else {
        AT_WARN_LOG("AT_RcvTiLedTestExpired AT_ERROR\n");
    }
}

HTIMER     g_ledTest = VOS_NULL_PTR;
VOS_UINT32 AT_SetLedTest(VOS_UINT8 indexNum)
{
    VOS_UINT32              rst;
    DRV_AGENT_LedTestSetReq ledTest;
    VOS_UINT32              timerRst;
    VOS_UINT32              timerName;
    VOS_UINT32              tempIndex;

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数过多 */
    if (g_atParaIndex > AT_LEDTEST_PARA_MAX_NUM) {
        return AT_TOO_MANY_PARA;
    }

    if (g_atParaList[AT_LEDTEST_PARA_LEDTEST].paraLen == 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    ledTest.ledTest = (VOS_UINT8)g_atParaList[AT_LEDTEST_PARA_LEDTEST].paraValue;

    tempIndex = (VOS_UINT32)indexNum;
    timerName = LEDTEST_TIMER_NAME;
    timerName |= AT_INTERNAL_PROCESS_TYPE;
    timerName |= (tempIndex << 12);

    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 DRV_AGENT_LEDTEST_SET_REQ, (VOS_UINT8 *)&ledTest, sizeof(ledTest), I0_WUEPS_PID_DRV_AGENT);

    if (rst != VOS_OK) {
        AT_ERR_LOG("AT_SetLedTest AT_ERROR\n");
        return AT_ERROR;
    }

    if (ledTest.ledTest == AT_LEDTEST_PARA_LEDTEST_ON) {
        // 启动定时器
        timerRst = AT_StartRelTimer(&g_ledTest, AT_LEDTEST_CMD_TIMER_LEN, timerName, 0, VOS_RELTIMER_LOOP);

        if (timerRst == VOS_OK) {
            g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_LEDTEST_SET;
            return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
        } else {
            AT_WARN_LOG("AT_SetLedTest AT_ERROR\n");
            return AT_ERROR;
        }
    } else {
        // 关闭定时器
        timerRst = AT_StopRelTimer(timerName, &g_ledTest);

        if (timerRst == VOS_OK) {
            return AT_OK; /* 停止定时器成功 */
        } else {
            AT_WARN_LOG("AT_SetLedTest STOP TIMER ERROR\n");
            return AT_ERROR;
        }
    }
}


VOS_UINT32 AT_SetGpioTest(VOS_UINT8 indexNum)
{
    VOS_UINT32 rst;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_CMD_NO_PARA) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 DRV_AGENT_GPIOTEST_SET_REQ, VOS_NULL_PTR, 0, I0_WUEPS_PID_DRV_AGENT);

    if (rst == VOS_OK) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_GPIOTEST_SET;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_AntTest(VOS_UINT8 indexNum)
{
    VOS_UINT32 rst;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_CMD_NO_PARA) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 DRV_AGENT_ANTTEST_QRY_REQ, VOS_NULL_PTR, 0, I0_WUEPS_PID_DRV_AGENT);

    if (rst == VOS_OK) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_ANTTEST_SET;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_GeTest(VOS_UINT8 indexNum)
{
    VOS_UINT32 rst;

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_CMD_NO_PARA) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    rst = (VOS_UINT32)stmmac_ge_loop_test();

    if (rst == 0) {
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName, rst);

        return AT_OK;
    } else {
        AT_WARN_LOG("AT_GeTest AT_ERROR\n");
        return AT_ERROR;
    }
}

VOS_VOID AT_RcvTiSfpTestExpired(REL_TimerMsgBlock *pstMsg)
{
    if (0 == stmmac_phy_loop_test()) {
        AT_WARN_LOG("AT_SfpTest AT_OK\n");
    } else {
        AT_WARN_LOG("AT_SfpTest AT_ERROR\n");
    }
}


VOS_UINT32 AT_SfpTest(VOS_UINT8 indexNum)
{
    VOS_UINT32 timerName;

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_CMD_NO_PARA) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    timerName = SFPTEST_TIMER_NAME;
    timerName |= AT_INTERNAL_PROCESS_TYPE;

    // 启动定时器
    HTIMER sfpTestTimer = VOS_NULL_PTR;
    if (AT_StartRelTimer(&sfpTestTimer, AT_SFPTEST_CMD_TIMER_LEN, timerName, 0, VOS_RELTIMER_NOLOOP) == VOS_OK) {
        return AT_OK;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_QrySfpTest(VOS_UINT8 indexNum)
{
    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    g_atSendDataBuff.bufLen =
        (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            stmmac_sfp_loop_test_result());

    return AT_OK;
}


VOS_UINT32 AT_SfpSwitch(VOS_UINT8 indexNum)
{
    VOS_UINT32 rst;

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数过多 */
    if (g_atParaIndex != AT_SFPSWITCH_PARA_NUM) {
        return AT_TOO_MANY_PARA;
    }

    rst = (VOS_UINT32)stmmac_phy_medium_autodetect((VOS_INT32)g_atParaList[0].paraValue);

    if (rst == 0) {
        g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName, rst);

        return AT_OK;
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_SetGpioHeatSet(VOS_UINT8 indexNum)
{
    VOS_UINT32               rst;
    DRV_AGENT_GpioTestSetReq gpioTest;

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数过多 */
    if (g_atParaIndex != AT_GPIOHEATSET_PARA_NUM) {
        return AT_TOO_MANY_PARA;
    }

    gpioTest.gpioTest = (VOS_UINT8)g_atParaList[AT_GPIOHEATSET_PARA_GPIO_TEST].paraValue;

    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 DRV_AGENT_GPIOHEAT_SET_REQ, (VOS_UINT8 *)&gpioTest, sizeof(gpioTest), I0_WUEPS_PID_DRV_AGENT);

    if (rst == VOS_OK) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_TEMPHEATTEST_SET;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_HkadcTest(VOS_UINT8 indexNum)
{
    VOS_UINT32                rst;
    DRV_AGENT_HkadctestSetReq hkadcTest;

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数过多 */
    if (g_atParaIndex > AT_HKADCTEST_PARA_MAX_NUM) {
        return AT_TOO_MANY_PARA;
    }

    if (g_atParaList[AT_HKADCTEST_PARA_HKADC_CHANNEL].paraLen == 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    hkadcTest.hkadcChannel = (VOS_UINT8)g_atParaList[AT_HKADCTEST_PARA_HKADC_CHANNEL].paraValue;

    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 DRV_AGENT_HKADCTEST_SET_REQ, (VOS_UINT8 *)&hkadcTest, sizeof(hkadcTest), I0_WUEPS_PID_DRV_AGENT);

    if (rst == VOS_OK) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_HKADCTEST_SET;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_QryGpio(VOS_UINT8 indexNum)
{
    VOS_UINT32           rst;
    DRV_AGENT_GpioQryReq gpio;

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数过多 */
    if (g_atParaIndex > AT_QRYGPIO_PARA_MAX_NUM) {
        return AT_TOO_MANY_PARA;
    }

    if (g_atParaList[AT_QRYGPIO_PARA_GPIO].paraLen == 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    gpio.gpio = (VOS_UINT16)g_atParaList[AT_QRYGPIO_PARA_GPIO].paraValue;

    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId, DRV_AGENT_GPIO_QRY_REQ,
                                 (VOS_UINT8 *)&gpio, sizeof(DRV_AGENT_GpioQryReq), I0_WUEPS_PID_DRV_AGENT);

    if (rst == VOS_OK) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_GPIO_QRY;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_SetGpio(VOS_UINT8 indexNum)
{
    VOS_UINT32           rst;
    DRV_AGENT_GpioSetReq gpioSetReq;

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数错误 */
    if (g_atParaIndex != AT_SETGPIO_PARA_NUM) {
        return AT_TOO_MANY_PARA;
    }

    if (g_atParaList[AT_SETGPIO_PARA_GPIO].paraLen == 0 || g_atParaList[AT_SETGPIO_PARA_VALUE].paraLen == 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    gpioSetReq.gpio  = (VOS_UINT16)g_atParaList[AT_SETGPIO_PARA_GPIO].paraValue;
    gpioSetReq.value = (VOS_UINT16)g_atParaList[AT_SETGPIO_PARA_VALUE].paraValue;

    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId, DRV_AGENT_GPIO_SET_REQ,
                                 (VOS_UINT8 *)&gpioSetReq, sizeof(gpioSetReq), I0_WUEPS_PID_DRV_AGENT);

    if (rst == VOS_OK) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_GPIO_SET;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_SetBootMode(VOS_UINT8 indexNum)
{
    VOS_UINT32   rst;
    NV_DUMP_STRU dumpcfg;
    errno_t      memResult;

    memResult = memset_s(&dumpcfg, sizeof(dumpcfg), 0x00, sizeof(dumpcfg));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(dumpcfg), sizeof(dumpcfg));

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }
    /* 参数错误 */
    if (g_atParaIndex != AT_SETBOOTMODE_PARA_NUM) {
        return AT_TOO_MANY_PARA;
    }

    if (g_atParaList[AT_SETBOOTMODE_PARA_TRACE_ONSTART_FLAG].paraLen == 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (NV_OK != TAF_ACORE_NV_READ(MODEM_ID_0, NV_ID_DRV_DUMP, &dumpcfg, sizeof(dumpcfg))) {
        AT_ERR_LOG("AT_SetBootMode: Read NV fail!\n");
        return AT_ERROR;
    }

    dumpcfg.traceOnstartFlag = g_atParaList[AT_SETBOOTMODE_PARA_TRACE_ONSTART_FLAG].paraValue;

    if (NV_OK != TAF_ACORE_NV_WRITE_NO_CHECK(MODEM_ID_0, NV_ID_DRV_DUMP, (VOS_UINT8 *)&(dumpcfg), sizeof(dumpcfg))) {
        AT_ERR_LOG("AT_SetBootMode: Write NV fail!");
        return AT_ERROR;
    }

    TAF_ACORE_NV_FLUSH();

    TAF_ACORE_NV_UPGRADE_BACKUP(EN_NVM_BACKUP_FILE);

    rst = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                 DRV_AGENT_BOOTMODE_SET_REQ, VOS_NULL_PTR, 0, I0_WUEPS_PID_DRV_AGENT);

    if (rst == VOS_OK) {
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_BOOTMODE_SET;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    } else {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_SetMultiupgMode(VOS_UINT8 indexNum)
{
    USB_ENUM_Status      multiUpgCfg;
    errno_t              memResult;

    memResult = memset_s(&multiUpgCfg, sizeof(multiUpgCfg), 0x00, sizeof(multiUpgCfg));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(multiUpgCfg), sizeof(multiUpgCfg));

    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_SET_PARA_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }
    /* 参数错误 */
    if (g_atParaIndex != AT_MULTIUPGMODE_PARA_NUM) {
        return AT_TOO_MANY_PARA;
    }

    if (g_atParaList[AT_MULTIUPGMODE_PARA_RESERVE2].paraLen == 0) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (NV_OK != TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_USB_ENUM_STATUS, &multiUpgCfg, sizeof(multiUpgCfg))) {
        AT_ERR_LOG("AT_SetMultiupgMode: Read NV fail!\n");
        return AT_ERROR;
    }

    multiUpgCfg.reserve2 = g_atParaList[AT_MULTIUPGMODE_PARA_RESERVE2].paraValue;

    if (NV_OK !=
        TAF_ACORE_NV_WRITE_NO_CHECK(MODEM_ID_0, NV_ITEM_USB_ENUM_STATUS, (VOS_UINT8 *)&(multiUpgCfg), sizeof(multiUpgCfg))) {
        AT_ERR_LOG("AT_SetMultiupgMode: Write NV fail!");
        return AT_ERROR;
    }

    return AT_OK;
}


VOS_UINT32 AT_QryMultiupgMode(VOS_UINT8 indexNum)
{
    USB_ENUM_Status      multiUpgCfg;
    errno_t              memResult;

    memResult = memset_s(&multiUpgCfg, sizeof(multiUpgCfg), 0x00, sizeof(multiUpgCfg));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(multiUpgCfg), sizeof(multiUpgCfg));

    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_ERROR;
    }

    if (NV_OK != TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_USB_ENUM_STATUS, &multiUpgCfg, sizeof(multiUpgCfg))) {
        AT_ERR_LOG("AT_QryMultiupgMode: Read NV fail!\n");
        return AT_ERROR;
    }

    g_atSendDataBuff.bufLen =
        (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "%s: %d", g_parseContext[indexNum].cmdElement->cmdName,
            multiUpgCfg.reserve2);

    return AT_OK;
}


VOS_UINT32 At_QryRsuTempPara(TAF_UINT8 indexNum)
{
    SPY_TempProtectNv        tempProtectNv;
    errno_t                  memResult;

    memResult = memset_s(&tempProtectNv, sizeof(tempProtectNv), 0x00, sizeof(tempProtectNv));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(tempProtectNv), sizeof(tempProtectNv));
    /* 读取热保护门限值的NV项 */
    if (NV_OK != TAF_ACORE_NV_READ(MODEM_ID_0, NV_ITEM_USIM_TEMP_PROTECT_NEW, (VOS_VOID *)&tempProtectNv,
                                   sizeof(SPY_TempProtectNv))) {
        AT_ERR_LOG("At_QryRsuTempPara: TAF_ACORE_NV_READ en_NV_Item_USIM_TEMP_PROTECT Fail!");

        return AT_ERROR;
    }

    g_atSendDataBuff.bufLen =
        (TAF_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (TAF_CHAR *)g_atSndCodeAddress,
            (TAF_CHAR *)g_atSndCodeAddress, "^RSUTEMPPRT: %d,%d,%d,%d", tempProtectNv.isEnable,
            tempProtectNv.powerOffThreshold, tempProtectNv.closeAdcThreshold, tempProtectNv.alarmAdcThreshold);

    return AT_OK;
}


VOS_UINT32 AT_QrySyncSt(VOS_UINT8 indexNum)
{
    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (VRRC_QryRsuSyncStat(g_atClientTab[indexNum].clientId, 0) == VOS_OK) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_SYNCST_QRY;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    }

    return AT_ERROR;
}


VOS_UINT32 AT_TestSyncSt(VOS_UINT8 indexNum)
{
    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_TEST_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    g_atSendDataBuff.bufLen = (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress, "%s: (0-2),(0,1)", g_parseContext[indexNum].cmdElement->cmdName);

    return AT_OK;
}


VOS_UINT32 AT_QryGnssSyncSt(VOS_UINT8 indexNum)
{
    /* 参数检查 */
    if (g_atParseCmd.cmdOptType != AT_CMD_OPT_READ_CMD) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (VRRC_QryGnssSyncStat(g_atClientTab[indexNum].clientId, 0) == VOS_OK) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_GNSSSYNCST_QRY;
        return AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    }

    return AT_ERROR;
}

#endif

#endif

