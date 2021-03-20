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

#include "AtDataProc.h"
#include "securec.h"
#include "ATCmdProc.h"
#include "AtTafAgentInterface.h"
#include "AtCtxPacket.h"
#include "AtCmdPacketProc.h"
#include "mn_comm_api.h"
#include "dms_port_i.h"
#include "at_mbb_common.h"
#include "at_mdrv_interface.h"
#include "taf_msg_chk_api.h"


/*
 * Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
 */
#define THIS_FILE_ID PS_FILE_ID_AT_DATAPROC_C
#define IPV4_ADDR_MAX_VALUE 255
#define IPV6_ADDR_MAX_VALUE 255
#define AT_PS_DIAL_PARAM_APN 2
#define AT_PS_DIAL_PARAM_IP_ADDR 6
#define AT_PS_DIAL_PARAM_USER_NAME 3
#define AT_PS_DIAL_PARAM_PASS_WORD 4
#define AT_PS_DIAL_PARAM_RAT_TYPE 7
#define AT_PS_USR_DIAL_AUTH_TYPE 5
#define AT_IPV6_STR_HEX_RADIX 16
#define AT_IPV6_STR_DEC_RADIX 10
#define AT_ITOA_HEX_RADIX 16
#define AT_ITOA_DEC_RADIX 10
#define AT_IP_HEAD_LEN 5
#define AT_EVENT_INFO_SIZE 4
#define AT_PHONE_DIAL_MAX_PARA_NUM 8
#define AT_WEBUI_DIAL_MAX_PARA_NUM 7
#define AT_NDIS_DIAL_MAX_PARA_NUM 7
#define AT_BYTE_TO_BITS_LENS 8
#define AT_IPV4_ADDR_INDEX_1 0
#define AT_IPV4_ADDR_INDEX_2 1
#define AT_IPV4_ADDR_INDEX_3 2
#define AT_IPV4_ADDR_INDEX_4 3
#define AT_BYTE_LOCAL_IPV6_MAX_OFFSET 7 /* ����IPV6�����ƫ���� */

/* HiLinkģʽ: ����ģʽ������ģʽ */
AT_HilinkModeUint8 g_hiLinkMode = AT_HILINK_NORMAL_MODE;

/* ����ָ����FC ID��Ӧ��FC Pri */
AT_FcidMap g_fcIdMaptoFcPri[FC_ID_BUTT];

const AT_ChdataRnicRmnetId g_atChdataRnicRmNetIdTab[] = {
    { AT_CH_DATA_CHANNEL_ID_0, RNIC_DEV_ID_BUTT, PS_IFACE_ID_NDIS0, { 0, 0 }},
    { AT_CH_DATA_CHANNEL_ID_1, RNIC_DEV_ID_RMNET0, PS_IFACE_ID_RMNET0, { 0, 0 }},
    { AT_CH_DATA_CHANNEL_ID_2, RNIC_DEV_ID_RMNET1, PS_IFACE_ID_RMNET1, { 0, 0 }},
    { AT_CH_DATA_CHANNEL_ID_3, RNIC_DEV_ID_RMNET2, PS_IFACE_ID_RMNET2, { 0, 0 }},
    { AT_CH_DATA_CHANNEL_ID_4, RNIC_DEV_ID_RMNET3, PS_IFACE_ID_RMNET3, { 0, 0 }},
    { AT_CH_DATA_CHANNEL_ID_5, RNIC_DEV_ID_RMNET4, PS_IFACE_ID_RMNET4, { 0, 0 }},
    { AT_CH_DATA_CHANNEL_ID_6, RNIC_DEV_ID_RMNET5, PS_IFACE_ID_RMNET5, { 0, 0 }},
    { AT_CH_DATA_CHANNEL_ID_7, RNIC_DEV_ID_RMNET6, PS_IFACE_ID_RMNET6, { 0, 0 }},
    { AT_CH_DATA_CHANNEL_ID_8, RNIC_DEV_ID_RMNET7, PS_IFACE_ID_RMNET7, { 0, 0 }},
    { AT_CH_DATA_CHANNEL_ID_9, RNIC_DEV_ID_RMNET8, PS_IFACE_ID_RMNET8, { 0, 0 }},
    { AT_CH_DATA_CHANNEL_ID_10, RNIC_DEV_ID_RMNET9, PS_IFACE_ID_RMNET9, { 0, 0 }},
    { AT_CH_DATA_CHANNEL_ID_11, RNIC_DEV_ID_RMNET10, PS_IFACE_ID_RMNET10, { 0, 0 }},
    { AT_CH_DATA_CHANNEL_ID_12, RNIC_DEV_ID_RMNET11, PS_IFACE_ID_RMNET11, { 0, 0 }},
#if (FEATURE_MULTI_NCM == FEATURE_ON)
    { AT_CH_DATA_CHANNEL_ID_13, RNIC_DEV_ID_BUTT, PS_IFACE_ID_NDIS1, { 0, 0 }},
    { AT_CH_DATA_CHANNEL_ID_14, RNIC_DEV_ID_BUTT, PS_IFACE_ID_NDIS2, { 0, 0 }},
    { AT_CH_DATA_CHANNEL_ID_15, RNIC_DEV_ID_BUTT, PS_IFACE_ID_NDIS3, { 0, 0 }}
#endif
};

static const AT_PS_RmnetIfaceId g_atPsIfaceIdRmNetIdTab[] = {
    { PS_IFACE_ID_RMNET0, RNIC_DEV_ID_RMNET0, 0, 0 },   { PS_IFACE_ID_RMNET1, RNIC_DEV_ID_RMNET1, 0, 0 },
    { PS_IFACE_ID_RMNET2, RNIC_DEV_ID_RMNET2, 0, 0 },   { PS_IFACE_ID_RMNET3, RNIC_DEV_ID_RMNET3, 0, 0 },
    { PS_IFACE_ID_RMNET4, RNIC_DEV_ID_RMNET4, 0, 0 },   { PS_IFACE_ID_RMNET5, RNIC_DEV_ID_RMNET5, 0, 0 },
    { PS_IFACE_ID_RMNET6, RNIC_DEV_ID_RMNET6, 0, 0 },   { PS_IFACE_ID_RMNET7, RNIC_DEV_ID_RMNET7, 0, 0 },
    { PS_IFACE_ID_RMNET8, RNIC_DEV_ID_RMNET8, 0, 0 },   { PS_IFACE_ID_RMNET9, RNIC_DEV_ID_RMNET9, 0, 0 },
    { PS_IFACE_ID_RMNET10, RNIC_DEV_ID_RMNET10, 0, 0 }, { PS_IFACE_ID_RMNET11, RNIC_DEV_ID_RMNET11, 0, 0 },
    { PS_IFACE_ID_NDIS0, RNIC_DEV_ID_BUTT, 0, 0 },
#if (FEATURE_MULTI_NCM == FEATURE_ON)
    { PS_IFACE_ID_NDIS1, RNIC_DEV_ID_BUTT, 0, 0 },
    { PS_IFACE_ID_NDIS2, RNIC_DEV_ID_BUTT, 0, 0 },
    { PS_IFACE_ID_NDIS3, RNIC_DEV_ID_BUTT, 0, 0 }
#endif
};

static const AT_PS_FcIfaceId g_atPsFcIfaceIdTab[] = {
    /* APP��IFACE ID��� */
    { FC_ID_NIC_1, PS_IFACE_ID_RMNET0, 0, 0 },
    { FC_ID_NIC_2, PS_IFACE_ID_RMNET1, 0, 0 },
    { FC_ID_NIC_3, PS_IFACE_ID_RMNET2, 0, 0 },
    { FC_ID_NIC_4, PS_IFACE_ID_RMNET3, 0, 0 },
    { FC_ID_NIC_5, PS_IFACE_ID_RMNET4, 0, 0 },
    { FC_ID_NIC_6, PS_IFACE_ID_RMNET5, 0, 0 },
    { FC_ID_NIC_7, PS_IFACE_ID_RMNET6, 0, 0 },
    { FC_ID_NIC_8, PS_IFACE_ID_RMNET7, 0, 0 },
    { FC_ID_NIC_9, PS_IFACE_ID_RMNET8, 0, 0 },
    { FC_ID_NIC_10, PS_IFACE_ID_RMNET9, 0, 0 },
    { FC_ID_NIC_11, PS_IFACE_ID_RMNET10, 0, 0 },
    { FC_ID_NIC_12, PS_IFACE_ID_RMNET11, 0, 0 },
    /* NDIS��IFACE ID��� */
    { FC_ID_NIC_1, PS_IFACE_ID_NDIS0, 0, 0 },
#if (FEATURE_MULTI_NCM == FEATURE_ON)
    { FC_ID_NIC_2, PS_IFACE_ID_NDIS1, 0, 0 },
    { FC_ID_NIC_3, PS_IFACE_ID_NDIS2, 0, 0 },
    { FC_ID_NIC_4, PS_IFACE_ID_NDIS3, 0, 0 },
#endif
};

static const AT_PS_ReportIfaceResult g_atRptIfaceResultTab[] = {
    /* ��ϢID */ /* ��Ϣ������ */
    { TAF_IFACE_USER_TYPE_APP, AT_PS_ProcAppIfaceStatus },
    { TAF_IFACE_USER_TYPE_NDIS, AT_PS_ProcNdisIfaceStatus },
};

static const AT_PS_EvtFuncTbl g_atIfaceEvtFuncTbl[] = {
    { ID_EVT_TAF_IFACE_UP_CNF, sizeof(TAF_PS_Evt) + sizeof(TAF_IFACE_UpCnf) - 4,
      VOS_NULL_PTR, AT_RcvTafIfaceEvtIfaceUpCnf },
    { ID_EVT_TAF_IFACE_DOWN_CNF, sizeof(TAF_PS_Evt) + sizeof(TAF_IFACE_DownCnf) - 4,
      VOS_NULL_PTR, AT_RcvTafIfaceEvtIfaceDownCnf },
    { ID_EVT_TAF_IFACE_STATUS_IND, sizeof(TAF_PS_Evt) + sizeof(TAF_IFACE_StatusInd) - 4,
      VOS_NULL_PTR, AT_RcvTafIfaceEvtIfaceStatusInd },
    { ID_EVT_TAF_IFACE_DATA_CHANNEL_STATE_IND, sizeof(TAF_PS_Evt) + sizeof(TAF_IFACE_DataChannelStateInd) - 4,
      VOS_NULL_PTR, AT_RcvTafIfaceEvtDataChannelStateInd },
    { ID_EVT_TAF_IFACE_USBNET_OPER_IND, sizeof(TAF_PS_Evt) + sizeof(TAF_IFACE_UsbnetOperInd) - 4,
      VOS_NULL_PTR, AT_RcvTafIfaceEvtUsbNetOperInd },
    { ID_EVT_TAF_IFACE_REG_FC_IND, sizeof(TAF_PS_Evt) + sizeof(TAF_IFACE_RegFcInd) - 4,
      VOS_NULL_PTR, AT_RcvTafIfaceEvtRegFcInd },
    { ID_EVT_TAF_IFACE_DEREG_FC_IND, sizeof(TAF_PS_Evt) + sizeof(TAF_IFACE_DeregFcInd) - 4,
      VOS_NULL_PTR, AT_RcvTafIfaceEvtDeRegFcInd },
    { ID_EVT_TAF_IFACE_GET_DYNAMIC_PARA_CNF, sizeof(TAF_PS_Evt) + sizeof(TAF_IFACE_GetDynamicParaCnf) - 4,
      VOS_NULL_PTR, AT_RcvTafIfaceEvtDyamicParaCnf },
    { ID_EVT_TAF_IFACE_RAB_INFO_IND, sizeof(TAF_PS_Evt) + sizeof(TAF_IFACE_RabInfoInd) - 4,
      VOS_NULL_PTR, AT_RcvTafIfaceEvtRabInfoInd },
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    { ID_EVT_TAF_IFACE_IP_CHANGE_IND, sizeof(TAF_PS_Evt) + sizeof(TAF_IFACE_IpChangeInd) - 4,
      VOS_NULL_PTR, AT_RcvTafIfaceEvtIpChangeInd },
#endif
};

static const AT_PS_DynamicInfoReportFuncTbl g_atDynamicInfoReportFuncTbl[] = {
    { AT_CMD_IPV6TEMPADDR_SET, AT_PS_ReportSetIpv6TempAddrRst },
    { AT_CMD_DHCP_SET, AT_PS_ReportSetDhcpRst },
    { AT_CMD_DHCP_QRY, AT_PS_ReportQryDhcpRst },
    { AT_CMD_DHCPV6_SET, AT_PS_ReportSetDhcpv6Rst },
    { AT_CMD_DHCPV6_QRY, AT_PS_ReportQryDhcpv6Rst },
    { AT_CMD_APRAINFO_SET, AT_PS_ReportSetApRaInfoRst },
    { AT_CMD_APRAINFO_QRY, AT_PS_ReportQryApRaInfoRst },
    { AT_CMD_APLANADDR_SET, AT_PS_ReportSetApLanAddrRst },
    { AT_CMD_APLANADDR_QRY, AT_PS_ReportQryApLanAddrRst },
    { AT_CMD_APCONNST_SET, AT_PS_ReportSetApConnStRst },
    { AT_CMD_APCONNST_QRY, AT_PS_ReportQryApConnStRst },
    { AT_CMD_DCONNSTAT_QRY, AT_PS_ReportQryDconnStatRst },
    { AT_CMD_DCONNSTAT_TEST, AT_PS_ReportTestDconnStatRst },
    { AT_CMD_NDISSTATQRY_QRY, AT_PS_ReportQryNdisStatRst },
    { AT_CMD_CGMTU_SET, AT_PS_ReportSetCgmtuRst },
};

/*
 * 3 ��������������
 */


VOS_UINT32 AT_Ipv4AddrAtoi(VOS_CHAR *pcString, VOS_UINT8 *number, VOS_UINT32 numBufLen)
{
    errno_t    memResult;
    VOS_UINT32 i = 0;
    VOS_UINT32 strLen;
    VOS_UINT32 numLen  = 0;
    VOS_UINT32 dotNum  = 0;
    VOS_UINT32 valTmp  = 0;
    VOS_UINT8  addr[TAF_IPV4_ADDR_LEN] = {0};

    if ((pcString == VOS_NULL_PTR) || (number == VOS_NULL_PTR)) {
        return VOS_ERR;
    }

    if (numBufLen < TAF_IPV4_ADDR_LEN) {
        return VOS_ERR;
    }

    strLen = VOS_StrLen(pcString);

    if (strLen > VOS_StrLen("255.255.255.255")) {
        AT_NORM_LOG("AT_Ipv4AddrAtoi: PCSCF IPV4 address length out of range");
        return VOS_ERR;
    }

    for (i = 0; i < strLen; i++) {
        if ((pcString[i] >= '0') && (pcString[i] <= '9')) {
            valTmp = (valTmp * 10) + (pcString[i] - '0'); /* ��10��Ϊ�˽��ַ���ת����ʮ������ */

            numLen++;
            continue;
        } else if (pcString[i] == '.') {
            if ((numLen == 0) || (numLen > 3)) { /* '.'�ָ��IPV6��ַ���ָ�ʽ���Ϊ255��������3����� */
                AT_NORM_LOG("AT_Ipv4AddrAtoi: the number between dots is out of range");
                return VOS_ERR;
            }

            if (valTmp > IPV4_ADDR_MAX_VALUE) {
                AT_NORM_LOG("AT_Ipv4AddrAtoi: the number is larger than 255");
                return VOS_ERR;
            }

            addr[dotNum] = (VOS_UINT8)valTmp;

            valTmp = 0;
            numLen = 0;

            /* ͳ��'.'�ĸ��� */
            dotNum++;
            if (dotNum >= TAF_IPV4_ADDR_LEN) {
                AT_NORM_LOG("AT_Ipv4AddrAtoi: dot num is more than 3, return ERROR");
                return VOS_ERR;
            }
            continue;
        } else {
            AT_NORM_LOG("AT_Ipv4AddrAtoi: character not number nor dot, return ERROR");
            /* ����ֱֵ�ӷ���ʧ�� */
            return VOS_ERR;
        }
    }

    /* �������3��'.'�򷵻�ʧ�� */
    if (dotNum != 3) {
        AT_NORM_LOG("AT_Ipv4AddrAtoi: dot number is not 3");
        return VOS_ERR;
    }

    /* �������ַ���ȡֵ */
    if (valTmp > IPV4_ADDR_MAX_VALUE) {
        AT_NORM_LOG("AT_Ipv4AddrAtoi: last number is larger than 255");
        return VOS_ERR;
    }

    addr[dotNum] = (VOS_UINT8)valTmp;

    memResult = memcpy_s(number, numBufLen, addr, sizeof(addr));
    TAF_MEM_CHK_RTN_VAL(memResult, numBufLen, sizeof(addr));

    return VOS_OK;
}


VOS_UINT32 AT_Ipv4AddrItoa(VOS_CHAR *pcString, VOS_UINT32 strLen, VOS_UINT8 *number)
{
    errno_t memResult;
    VOS_INT printResult;

    if ((pcString == VOS_NULL_PTR) || (number == VOS_NULL_PTR)) {
        return VOS_ERR;
    }

    if (strLen < TAF_MAX_IPV4_ADDR_STR_LEN) {
        return VOS_ERR;
    }

    memResult = memset_s(pcString, strLen, 0x00, strLen);
    TAF_MEM_CHK_RTN_VAL(memResult, strLen, strLen);

    printResult = snprintf_s(pcString, strLen, strLen - 1, "%d.%d.%d.%d", number[AT_IPV4_ADDR_INDEX_1],
                             number[AT_IPV4_ADDR_INDEX_2], number[AT_IPV4_ADDR_INDEX_3], number[AT_IPV4_ADDR_INDEX_4]);
    TAF_SPRINTF_CHK_RTN_VAL_CONTINUE(printResult, strLen, strLen - 1);
    return VOS_OK;
}


VOS_UINT32 AT_Ipv4Addr2Str(VOS_CHAR *pcString, VOS_UINT32 strBuflen, VOS_UINT8 *number, VOS_UINT32 addrNumCnt)
{
    errno_t memResult;
    VOS_INT printResult;

    if ((pcString == VOS_NULL_PTR) || (number == VOS_NULL_PTR)) {
        return VOS_ERR;
    }

    if ((strBuflen < TAF_MAX_IPV4_ADDR_STR_LEN) || (addrNumCnt < TAF_IPV4_ADDR_LEN)) {
        return VOS_ERR;
    }

    memResult = memset_s(pcString, strBuflen, 0x00, TAF_MAX_IPV4_ADDR_STR_LEN);
    TAF_MEM_CHK_RTN_VAL(memResult, strBuflen, TAF_MAX_IPV4_ADDR_STR_LEN);

    /* ��������ַΪ�գ��򷵻ؿ��ַ��� */
    if ((number[AT_IPV4_ADDR_INDEX_1] == 0) && (number[AT_IPV4_ADDR_INDEX_2] == 0) &&
        (number[AT_IPV4_ADDR_INDEX_3] == 0) && (number[AT_IPV4_ADDR_INDEX_4] == 0)) {
        return VOS_OK;
    }

    printResult = snprintf_s(pcString, strBuflen, strBuflen - 1, "%d.%d.%d.%d", number[AT_IPV4_ADDR_INDEX_1],
                             number[AT_IPV4_ADDR_INDEX_2], number[AT_IPV4_ADDR_INDEX_3], number[AT_IPV4_ADDR_INDEX_4]);
    TAF_SPRINTF_CHK_RTN_VAL_CONTINUE(printResult, strBuflen, strBuflen - 1);

    return VOS_OK;
}


VOS_VOID AT_PcscfIpv4Addr2Str(VOS_CHAR *pcString, VOS_UINT32 strBufLen, VOS_UINT8 *number)
{
    errno_t memResult;
    VOS_INT printResult;

    if (strBufLen < TAF_MAX_IPV4_ADDR_STR_LEN) {
        return;
    }

    memResult = memset_s(pcString, strBufLen, 0, strBufLen);
    TAF_MEM_CHK_RTN_VAL(memResult, strBufLen, strBufLen);

    printResult = snprintf_s(pcString, strBufLen, strBufLen - 1, "%d.%d.%d.%d", number[AT_IPV4_ADDR_INDEX_1],
                             number[AT_IPV4_ADDR_INDEX_2], number[AT_IPV4_ADDR_INDEX_3], number[AT_IPV4_ADDR_INDEX_4]);
    TAF_SPRINTF_CHK_RTN_VAL_CONTINUE(printResult, strBufLen, strBufLen - 1);
}


VOS_UINT32 AT_Ipv6AddrAtoi(VOS_CHAR *pcString, VOS_UINT8 *number, VOS_UINT32 ipv6BufLen)
{
    errno_t    memResult;
    VOS_UINT32 i = 0;
    VOS_UINT32 strLen;
    VOS_UINT32 numLen                  = 0;
    VOS_UINT32 dotNum                  = 0;
    VOS_UINT32 valTmp                  = 0;
    VOS_UINT8  addr[TAF_IPV6_ADDR_LEN] = {0};

    if ((pcString == VOS_NULL_PTR) || (number == VOS_NULL_PTR)) {
        return VOS_ERR;
    }

    if (ipv6BufLen < TAF_IPV6_ADDR_LEN) {
        return VOS_ERR;
    }

    strLen = VOS_StrLen(pcString);

    if (strLen > VOS_StrLen("255.255.255.255.255.255.255.255.255.255.255.255.255.255.255.255")) {
        return VOS_ERR;
    }

    for (i = 0; i < strLen; i++) {
        if ((pcString[i] >= '0') && (pcString[i] <= '9')) {
            valTmp = (valTmp * 10) + (pcString[i] - '0'); /* ��10��Ϊ�˽��ַ���ת����ʮ������ */

            numLen++;
            continue;
        } else if (pcString[i] == '.') {
            if ((numLen == 0) || (numLen > 3)) { /* '.'�ָ��IPV6��ַ���ָ�ʽ���Ϊ255��������3����� */
                return VOS_ERR;
            }

            if (valTmp > IPV6_ADDR_MAX_VALUE) {
                return VOS_ERR;
            }

            addr[dotNum] = (VOS_UINT8)valTmp;

            valTmp = 0;
            numLen = 0;

            /* ͳ��'.'�ĸ��� */
            dotNum++;
            if (dotNum >= TAF_IPV6_ADDR_LEN) {
                return VOS_ERR;
            }

            continue;
        } else {
            /* ����ֱֵ�ӷ���ʧ�� */
            return VOS_ERR;
        }
    }

    /* �������3��'.'�򷵻�ʧ�� */
    if ((TAF_IPV6_ADDR_LEN - 1) != dotNum) {
        return VOS_ERR;
    }

    /* �������ַ���ȡֵ */
    if (valTmp > IPV6_ADDR_MAX_VALUE) {
        return VOS_ERR;
    }

    addr[dotNum] = (VOS_UINT8)valTmp;

    memResult = memcpy_s(number, ipv6BufLen, addr, TAF_IPV6_ADDR_LEN);
    TAF_MEM_CHK_RTN_VAL(memResult, ipv6BufLen, TAF_IPV6_ADDR_LEN);

    return VOS_OK;
}


VOS_VOID AT_Ipv6LenStrToAddrProcCompressed(VOS_UINT8 *str, VOS_UINT8 colonCount, VOS_UINT8 dotCount,
                                           VOS_UINT8 strlength, VOS_UINT8 idxPos)
{
    VOS_UINT8 i;

    /* ���ַ�����ѹ��λ�ÿ�ʼ˳������ƶ� */
    for (i = strlength; i >= idxPos; i--) {
        if (dotCount != AT_MAX_IPV6_STR_DOT_NUM) {
            str[i + AT_MAX_IPV6_STR_COLON_NUM - colonCount] = str[i];
        } else {
            str[i + AT_MAX_IPV4V6_STR_COLON_NUM - colonCount] = str[i];
        }
    }

    /* ����ѹ����ð�� */
    if (dotCount != AT_MAX_IPV6_STR_DOT_NUM) {
        for (i = idxPos; i < (idxPos + AT_MAX_IPV6_STR_COLON_NUM - colonCount); i++) {
            str[i + 1] = ':';
        }
    } else {
        for (i = idxPos; i < (idxPos + AT_MAX_IPV4V6_STR_COLON_NUM - colonCount); i++) {
            str[i + 1] = ':';
        }
    }

    return;
}


VOS_UINT32 AT_Ipv6LenStrToAddrAccess(VOS_UINT8 *str, VOS_UINT8 *colonCount, VOS_UINT8 *dotCount, VOS_UINT8 *strlength,
                                     VOS_UINT8 *idxPos)
{
    VOS_UINT32 i;

    for (i = 0; ((i < TAF_MAX_IPV6_ADDR_COLON_STR_LEN) && (str[i] != '\0')); i++) {
        if ((str[i] < '0' || str[i] > '9') && (str[i] < 'A' || str[i] > 'F') && (str[i] != '.') && (str[i] != ':')) {
            return VOS_ERR;
        }
        /* ȡ�ò���ð������λ�� */
        if ((i > 0) && (str[i - 1] == ':') && (str[i] == ':')) {
            *idxPos = (VOS_UINT8)i;
        }

        /* ͳ��ð�Ÿ��� */
        if (str[i] == ':') {
            (*colonCount)++;
        }

        /* ͳ�Ƶ�Ÿ��� */
        if (str[i] == '.') {
            (*dotCount)++;
        }
    }

    *strlength = (VOS_UINT8)i;

    return VOS_OK;
}


VOS_UINT32 AT_PcscfIpv6StrToAddr(VOS_UINT8 *str, VOS_UINT8 *ipAddr, VOS_UINT8 colonCount)
{
    VOS_UINT32 i;
    VOS_UINT32 j;
    VOS_UINT16 value;     /* Ipv6ʮ������ת���� */
    VOS_UINT8  valueTemp; /* Ipv4ʮ������ת���� */
    VOS_UINT32 numLen;

    value     = 0;
    valueTemp = 0;
    j         = 0;
    numLen    = 0;

    if ((str == VOS_NULL_PTR) || (ipAddr == VOS_NULL_PTR)) {
        AT_NORM_LOG("AT_PcscfIpv6StrToAddr: input ptr is NULL, return ERROR");
        return VOS_ERR;
    }

    for (i = 0; ((i < TAF_MAX_IPV6_ADDR_COLON_STR_LEN) && (str[i] != '\0')); i++) {
        /* ƥ���ַ� */
        if (str[i] != ':') {
            value <<= 4;

            if ((str[i] >= '0') && (str[i] <= '9')) {
                /* ʮ���Ƹ�ʽת�� */
                value += (VOS_UINT16)(str[i] - '0');
                /* ��10��Ϊ�˻�ø�λ�������ӵ�λ��0x30Ϊchar�͵�'0' */
                valueTemp = (VOS_UINT8)((valueTemp * 10) + (str[i] - 0x30));
            } else {
                /* ��ʮ������"A"��"F"ת��Ϊʮ���ƣ���Ҫ��ĩβ��10 */
                value += (VOS_UINT16)((str[i] - 'A') + AT_DECIMAL_BASE_NUM);
            }

            numLen++;
        }
        /* ƥ�䵽ð�� */
        else {
            /* ð��֮����ַ�����4������Ϊ��ʽ���� */
            if (numLen > 4) {
                AT_ERR_LOG("AT_PcscfIpv6StrToAddr: the number of char betwwen colons is more than 4, return ERROR");
                return VOS_ERR;
            }

            /* IPV6ʮ������ȡ�߰�λ���� */
            ipAddr[j] = (VOS_UINT8)((value >> 8) & 0x00FF);
            j++;
            /* IPV6ʮ������ȡ�Ͱ�λ���� */
            ipAddr[j] = (VOS_UINT8)(value & 0x00FF);
            j++;
            value  = 0;
            numLen = 0;
        }
    }

    /* ƥ�����һ��ת�� */
    if (colonCount == AT_MAX_IPV6_STR_COLON_NUM) {
        ipAddr[j] = (VOS_UINT8)((value >> 8) & 0x00FF);
        j++;
        ipAddr[j] = (VOS_UINT8)(value & 0x00FF);
    }

    return VOS_OK;
}


VOS_UINT32 AT_CheckPcscfIpv6Addr(VOS_CHAR *ipv6Str, VOS_UINT32 *portExistFlg)
{
    VOS_CHAR *ipv6Start = VOS_NULL_PTR;
    VOS_CHAR *ipv6End   = VOS_NULL_PTR;

    if ((ipv6Str == VOS_NULL_PTR) || (portExistFlg == VOS_NULL_PTR)) {
        AT_NORM_LOG("AT_CheckPcscfIpv6Addr: input ptr is NULL, return ERROR");
        return VOS_ERR;
    }

    ipv6End   = VOS_StrStr(ipv6Str, "]");
    ipv6Start = VOS_StrStr(ipv6Str, "[");

    if ((ipv6End == VOS_NULL_PTR) && (ipv6Start == VOS_NULL_PTR)) {
        AT_NORM_LOG("AT_CheckPcscfIpv6Addr: NO [ ] symbol in IPV6 addr, port not exists");
        *portExistFlg = VOS_FALSE;
        return VOS_OK;
    }

    if ((ipv6End != VOS_NULL_PTR) && (ipv6Start != VOS_NULL_PTR) && (ipv6End > ipv6Start)) {
        if ((ipv6End - ipv6Start) > TAF_MAX_IPV6_ADDR_COLON_STR_LEN) {
            AT_ERR_LOG("AT_CheckPcscfIpv6Addr: length of IPV6 addr in [] is larger than 39, return ERROR");
            return VOS_ERR;
        }

        AT_NORM_LOG("AT_CheckPcscfIpv6Addr: Have both [ ] symbol in IPV6 addr");
        *portExistFlg = VOS_TRUE;

        return VOS_OK;
    }

    AT_ERR_LOG("AT_CheckPcscfIpv6Addr: IPV6 addr format incorrect");
    return VOS_ERR;
}


VOS_UINT32 AT_ParsePortFromPcscfIpv6Addr(VOS_UINT8 *str, VOS_UINT8 *ipv6Addr, VOS_UINT32 ipv6BufLen,
                                         VOS_UINT32 *portExistFlg, VOS_UINT32 *portNum)
{
    VOS_CHAR *ipv6Start = VOS_NULL_PTR;
    VOS_CHAR *ipv6End   = VOS_NULL_PTR;
    VOS_CHAR *ipv6Str   = VOS_NULL_PTR;
    VOS_CHAR *pcStrPort = VOS_NULL_PTR;
    errno_t   memResult;

    if ((str == VOS_NULL_PTR) || (ipv6Addr == VOS_NULL_PTR) || (portExistFlg == VOS_NULL_PTR) ||
        (portNum == VOS_NULL_PTR)) {
        AT_ERR_LOG("AT_ParsePortFromPcscfIpv6Addr: input ptr is NULL, return ERROR");
        return VOS_ERR;
    }

    if (AT_CheckPcscfIpv6Addr((VOS_CHAR *)str, portExistFlg) != VOS_OK) {
        AT_ERR_LOG("AT_ParsePortFromPcscfIpv6Addr: AT_CheckPcscfIpv6Addr FAIL, return ERROR");
        return VOS_ERR;
    }

    /* [ipv6]:port��ʽ���ݴ�����ȡipv6�˿ں� */
    ipv6Str   = (VOS_CHAR *)str;
    ipv6End   = VOS_StrStr(ipv6Str, "]");
    ipv6Start = VOS_StrStr(ipv6Str, "[");

    if (*portExistFlg == VOS_TRUE) {
        if ((ipv6End == VOS_NULL_PTR) || (ipv6Start == VOS_NULL_PTR)) {
            AT_ERR_LOG("AT_ParsePortFromPcscfIpv6Addr: NO [ ] symbol in IPV6 addr, return ERROR");
            return VOS_ERR;
        }

        /* �����������ڵ�IPV6��ַ��Ϣ���Ƴ��˿ں� */
        if (ipv6End > (ipv6Start + 1)) {
            memResult = memcpy_s(ipv6Addr, ipv6BufLen, ipv6Start + 1, (VOS_UINT32)((ipv6End - ipv6Start) - 1));
            TAF_MEM_CHK_RTN_VAL(memResult, ipv6BufLen, (VOS_UINT32)((ipv6End - ipv6Start) - 1));
        }

        /* ��¼�ָ���ַ�Ͷ˿ڵ�ð�ŵ�ַ */
        pcStrPort = VOS_StrStr(ipv6End, ":");

        /* [ipv6]:port��ʽû��ð�ţ�����ERROR */
        if (pcStrPort == VOS_NULL_PTR) {
            *portExistFlg = VOS_FALSE;
            AT_ERR_LOG("AT_ParsePortFromPcscfIpv6Addr: IPV6 Port colon missed, return ERROR ");
            return VOS_ERR;
        }

        /* ð��ǰ�治��"]"������ERROR */
        if (pcStrPort != ipv6End + 1) {
            *portExistFlg = VOS_FALSE;
            AT_ERR_LOG("AT_ParsePortFromPcscfIpv6Addr: IPV6 Port colon location incorrect, return ERROR ");
            return VOS_ERR;
        }

        /* [ipv6]:port��ʽ�˿ڽ������󣬷���ERROR */
        if (AT_PortAtoI(pcStrPort + 1, portNum) != VOS_OK) {
            *portExistFlg = VOS_FALSE;
            AT_ERR_LOG("AT_ParsePortFromPcscfIpv6Addr: IPV6 Port decode ERROR");
            return VOS_ERR;
        }

        *portExistFlg = VOS_TRUE;
    } else {
        /* û�ж˿ںţ�������ַ���ֻ����IPV6��ַ */
        memResult = memcpy_s(ipv6Addr, ipv6BufLen, str, TAF_MAX_IPV6_ADDR_COLON_STR_LEN);
        TAF_MEM_CHK_RTN_VAL(memResult, ipv6BufLen, TAF_MAX_IPV6_ADDR_COLON_STR_LEN);
    }

    return VOS_OK;
}


VOS_UINT32 AT_ParseAddrFromPcscfIpv6Addr(VOS_UINT8 *str, VOS_UINT32 strLen, VOS_UINT8 *ipAddr)
{
    VOS_UINT8  colonCount; /* �ַ�����ð�Ÿ��� */
    VOS_UINT8  dotCount;   /* �ַ����е�Ÿ��� */
    VOS_UINT8  strlength;  /* �ַ������� */
    VOS_UINT8  idxPos;     /* ��Ҫ����ð�ŵ�λ�� */
    VOS_UINT32 result;

    colonCount = 0;
    dotCount   = 0;
    strlength  = 0;
    idxPos     = 0xFF;

    if ((str == VOS_NULL_PTR) || (ipAddr == VOS_NULL_PTR)) {
        AT_NORM_LOG("AT_ParseAddrFromPcscfIpv6Addr: input ptr is NULL, return ERROR");
        return VOS_ERR;
    }

    /* ����IPV6��ַ�ַ��� */
    if (AT_Ipv6LenStrToAddrAccess(str, &colonCount, &dotCount, &strlength, &idxPos) != VOS_OK) {
        AT_ERR_LOG("AT_ParseAddrFromPcscfIpv6Addr: AT_Ipv6LenStrToAddrAccess FAIL, return ERROR");
        return VOS_ERR;
    }

    /* ��֧��IPV4IPV6����͵ĸ�ʽ */
    if (dotCount != 0) {
        AT_ERR_LOG("AT_ParseAddrFromPcscfIpv6Addr: There have dot symbol in address format, return ERROR");
        return VOS_ERR;
    }

    /* �ַ���Ϊ�շ���ʧ�� */
    if (strlength == 0) {
        AT_ERR_LOG("AT_ParseAddrFromPcscfIpv6Addr: IP address length is 0, return ERROR");
        return VOS_ERR;
    }

    /* ð�Ÿ�������7�򷵻�ʧ�� */
    if (colonCount > AT_MAX_IPV6_STR_COLON_NUM) {
        AT_ERR_LOG("AT_ParseAddrFromPcscfIpv6Addr: IPV6 address Colon number is larger than 7, return ERROR");
        return VOS_ERR;
    }

    if (colonCount == AT_MAX_IPV6_STR_COLON_NUM) {
        /* ��ѹ����ʽ���� */
        result = AT_PcscfIpv6StrToAddr(str, ipAddr, colonCount);
    } else {
        if (idxPos != 0xFF) {
            /* ѹ����ʽ���� */
            AT_Ipv6LenStrToAddrProcCompressed(str, colonCount, dotCount, strlength, idxPos);
            /* ӳ��IPV6��ַ��ʽ */
            result = AT_PcscfIpv6StrToAddr(str, ipAddr, AT_MAX_IPV6_STR_COLON_NUM);
        }
        /* ѹ��IPV6��ַ���Ҳ����������ڵ�ð�ţ���ʽ���� */
        else {
            AT_ERR_LOG(
                "AT_ParseAddrFromPcscfIpv6Addr: Can not find two consecutive colons in compressed IPV6 address , return ERROR");
            return VOS_ERR;
        }
    }

    return result;
}


VOS_UINT32 AT_Ipv6PcscfDataToAddr(VOS_UINT8 *str, VOS_UINT8 *ipAddr, VOS_UINT32 *portExistFlg, VOS_UINT32 *portNum

)
{
    VOS_UINT8 strTmp[TAF_MAX_IPV6_ADDR_COLON_STR_LEN];

    (VOS_VOID)memset_s(strTmp, TAF_MAX_IPV6_ADDR_COLON_STR_LEN, 0, TAF_MAX_IPV6_ADDR_COLON_STR_LEN);

    if ((str == VOS_NULL_PTR) || (ipAddr == VOS_NULL_PTR) || (portExistFlg == VOS_NULL_PTR) ||
        (portNum == VOS_NULL_PTR)) {
        AT_NORM_LOG("AT_Ipv6PcscfDataToAddr: input ptr is NULL, return ERROR");
        return VOS_ERR;
    }

    if (AT_ParsePortFromPcscfIpv6Addr(str, strTmp, sizeof(strTmp), portExistFlg, portNum) != VOS_OK) {
        AT_ERR_LOG("AT_Ipv6PcscfDataToAddr: AT_ParsePortFromPcscfIpv6Addr FAIL, return ERROR");
        return VOS_ERR;
    }

    /* ��IPV6��ַ��ʽת��Ϊ��д */
    VOS_StrToUpper((VOS_CHAR *)strTmp);

    if (AT_ParseAddrFromPcscfIpv6Addr(strTmp, sizeof(strTmp), ipAddr) != VOS_OK) {
        AT_ERR_LOG("AT_Ipv6PcscfDataToAddr: AT_ParseAddrFromPcscfIpv6Addr FAIL, return ERROR");
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 AT_Ipv6AddrToStr(VOS_UINT8 aucAddrStr[], VOS_UINT8 aucIpAddr[], AT_IPV6_StrTypeUint8 ipStrType)
{
    VOS_UINT8 *buffer = VOS_NULL_PTR;
    VOS_UINT16 addrValue[AT_IPV6_STR_MAX_TOKENS];
    VOS_UINT16 addrNum;
    VOS_UINT8  delimiter;
    VOS_UINT8  tokensNum;
    VOS_UINT8  radix;
    VOS_UINT32 i;

    buffer = aucAddrStr;

    (VOS_VOID)memset_s(addrValue, sizeof(addrValue), 0x00, sizeof(addrValue));

    /* ����IP�ַ�����ʽ�������, ���ö�Ӧ��ת������ */
    switch (ipStrType) {
        case AT_IPV6_STR_TYPE_HEX:
            delimiter = AT_IPV6_STR_DELIMITER;
            tokensNum = AT_IPV6_ADDR_HEX_TOKEN_NUM;
            radix     = AT_IPV6_STR_HEX_RADIX;
            break;

        case AT_IPV6_STR_TYPE_DEC:
            delimiter = AT_IPV4_STR_DELIMITER;
            tokensNum = AT_IPV6_STR_MAX_TOKENS;
            radix     = AT_IPV6_STR_DEC_RADIX;
            break;

        default:
            return VOS_ERR;
    }

    /* ����IP�ַ�����ʽ�������, ��ȡ�ֶε�IP��ַ��ֵ */
    for (i = 0; i < tokensNum; i++) {
        addrNum = *aucIpAddr++;

        if (ipStrType == AT_IPV6_STR_TYPE_HEX) {
            addrNum <<= 8;
            addrNum |= *aucIpAddr++;
        }

        addrValue[i] = addrNum;
    }

    /* ����IP��ַ�ֶ�, �����зָ�����ǵ�IP��ַ�ַ��� */
    for (i = 0; i < tokensNum; i++) {
        buffer = (VOS_UINT8 *)AT_Itoa(addrValue[i], (VOS_CHAR *)buffer, radix,
                                      (TAF_MAX_IPV6_ADDR_DOT_STR_LEN - (VOS_UINT32)(aucAddrStr - buffer)));
        *buffer++ = delimiter;
    }

    /* ȡ�����һ���ָ���, �����ַ��������� */
    if (aucAddrStr != buffer) {
        --buffer;
        *buffer = '\0';
    }

    return VOS_OK;
}


VOS_UINT32 AT_Ipv6Addr2DecString(VOS_CHAR *pcIpv6FormatStr, VOS_UINT8 aucIpv6Addr[])
{
    VOS_UINT32 length;
    VOS_UINT32 loop;
    VOS_INT    iRslt;

    length = 0;

    /* ѭ����ӡ10���Ƶ��IPv6��ַ */
    for (loop = 0; loop < AT_IPV6_ADDR_DEC_TOKEN_NUM; loop++) {
        /* ��ӡ��ָ��� */
        if (loop != 0) {
            *(pcIpv6FormatStr + length) = AT_IP_STR_DOT_DELIMITER;
            length++;
        }

        iRslt = snprintf_s(pcIpv6FormatStr + length, AT_IPV6_ADDR_DEC_FORMAT_STR_LEN - length,
                           (AT_IPV6_ADDR_DEC_FORMAT_STR_LEN - length) - 1, "%d", aucIpv6Addr[loop]);

        if (iRslt <= 0) {
            AT_WARN_LOG("AT_Ipv6Addr2DecString: Print IPv6 Addr Failed!");
            return 0;
        }

        length += iRslt;
    }

    return length;
}


VOS_VOID AT_ConvertIpv6AddrToHexAddrAndGetMaxZeroCnt(VOS_UINT8 aucIpv6Addr[], VOS_UINT16 ausIpv6HexAddr[],
                                                     VOS_UINT32 *zeroStartIndex, VOS_UINT32 *zeroMaxCnt)
{
    VOS_UINT32 loop;
    VOS_UINT32 zeroTmpIndex;
    VOS_UINT32 zeroTmpCnt;

    zeroTmpIndex = 0;
    zeroTmpCnt   = 0;

    /* ѭ��ת��IPv6��ʽ��ַ����ͳ���������� */
    for (loop = 0; loop < AT_IPV6_ADDR_HEX_TOKEN_NUM; loop++) {
        /* �ϲ��ֽ� */
        ausIpv6HexAddr[loop] = *(aucIpv6Addr + loop + loop);
        ausIpv6HexAddr[loop] <<= 8;
        ausIpv6HexAddr[loop] |= *(aucIpv6Addr + loop + loop + 1);

        if (ausIpv6HexAddr[loop] == 0) {
            /* ���16�����ֶ�Ϊ0�����¼��ʱcnt��Index */
            if (zeroTmpCnt == 0) {
                zeroTmpIndex = loop;
            }

            zeroTmpCnt++;
        } else {
            /* ���16�����ֶβ�Ϊ0�����ж��Ƿ������������¼ */
            if (zeroTmpCnt > *zeroMaxCnt) {
                *zeroStartIndex = zeroTmpIndex;
                *zeroMaxCnt     = zeroTmpCnt;
            }

            zeroTmpCnt   = 0;
            zeroTmpIndex = 0;
        }
    }

    /* �ж��Ƿ������������¼ */
    if (zeroTmpCnt > *zeroMaxCnt) {
        *zeroStartIndex = zeroTmpIndex;
        *zeroMaxCnt     = zeroTmpCnt;
    }

    return;
}


VOS_UINT32 AT_Ipv6Addr2HexString(VOS_CHAR *pcIpv6FormatStr, VOS_UINT8 aucIpv6Addr[])
{
    VOS_UINT32    length;
    AT_CommPsCtx *commPsCtx   = VOS_NULL_PTR;
    VOS_CHAR     *pcFormatStr = VOS_NULL_PTR;
    VOS_UINT32    loop;
    VOS_UINT16    ipv6HexAddr[AT_IPV6_ADDR_HEX_TOKEN_NUM];
    VOS_UINT32    zeroStartIndex;
    VOS_UINT32    zeroMaxCnt;
    VOS_INT       iRslt;

    /* �ֲ�������ʼ�� */
    length         = 0;
    commPsCtx      = AT_GetCommPsCtxAddr();
    zeroStartIndex = 0;
    zeroMaxCnt     = 0;
    (VOS_VOID)memset_s(ipv6HexAddr, sizeof(ipv6HexAddr), 0, sizeof(ipv6HexAddr));

    /* �����Ƿ���ǰ����ȷ�ϴ�ӡ��ʽ */
    pcFormatStr = (commPsCtx->opIpv6LeadingZeros == VOS_FALSE) ? "%04X" : "%X";

    /* ת��IPv6��ַΪ16Bit HEX���ͣ���ͳ�������������� */
    AT_ConvertIpv6AddrToHexAddrAndGetMaxZeroCnt(aucIpv6Addr, ipv6HexAddr, &zeroStartIndex, &zeroMaxCnt);

    /* ѭ����ӡ16���Ƶ��IPv6��ַ */
    for (loop = 0; loop < AT_IPV6_ADDR_HEX_TOKEN_NUM; loop++) {
        /* ������ѹ�����ܣ��Ҵ�����������㣬��ѹ����ӡ */
        if ((commPsCtx->opIpv6CompressZeros != VOS_FALSE) && (zeroMaxCnt > 0)) {
            /* ��һ��0����ӡð�� */
            if (zeroStartIndex == loop) {
                *(pcIpv6FormatStr + length) = AT_IP_STR_COLON_DELIMITER;
                length++;
                continue;
            }

            /* ����0����ӡ */
            if ((loop > zeroStartIndex) && (loop < (zeroStartIndex + zeroMaxCnt))) {
                /* ���һλΪ0����Ҫ���ӡһ��ð�� */
                if (loop == (AT_IPV6_ADDR_HEX_TOKEN_NUM - 1)) {
                    *(pcIpv6FormatStr + length) = AT_IP_STR_COLON_DELIMITER;
                    length++;
                }

                continue;
            }
        }

        /* ��ӡð�ŷָ��� */
        if (loop != 0) {
            *(pcIpv6FormatStr + length) = AT_IP_STR_COLON_DELIMITER;
            length++;
        }

        iRslt = snprintf_s(pcIpv6FormatStr + length, AT_IPV6_ADDR_COLON_FORMAT_STR_LEN - length,
                           (AT_IPV6_ADDR_COLON_FORMAT_STR_LEN - length) - 1, pcFormatStr, ipv6HexAddr[loop]);

        if (iRslt <= 0) {
            AT_WARN_LOG("AT_Ipv6Addr2HexString: Print IPv6 Addr Failed!");
            return 0;
        }

        length += iRslt;
    }

    return length;
}


VOS_UINT32 AT_Ipv6AddrMask2FormatString(VOS_CHAR *pcIpv6FormatStr, VOS_UINT8 aucIpv6Addr[], VOS_UINT8 aucIpv6Mask[])
{
    VOS_UINT32    length;
    AT_CommPsCtx *commPsCtx = VOS_NULL_PTR;
    VOS_INT       iRslt;

    /* �ֲ�������ʼ�� */
    length    = 0;
    commPsCtx = AT_GetCommPsCtxAddr();
    iRslt     = 0;

    /* ����Ҫ��IPv6��ַ����������ӡIPv6�������� */
    if (aucIpv6Addr == VOS_NULL_PTR) {
        AT_WARN_LOG("AT_Ipv6AddrMask2FormatString: No IPv6 Address!");
        *pcIpv6FormatStr = '\0';
        return 0;
    }

    if (commPsCtx->opIpv6AddrFormat == VOS_FALSE) {
        /* 10���Ƶ�ָ�ʽ��ӡIPv6��ַ */
        length = AT_Ipv6Addr2DecString(pcIpv6FormatStr, aucIpv6Addr);

        /* ��ӡ�������� */
        if (aucIpv6Mask != VOS_NULL_PTR) {
            /* ʹ�õ�ָ�IP��ַ���������� */
            *(pcIpv6FormatStr + length) = AT_IP_STR_DOT_DELIMITER;
            length++;

            /* 10���Ƶ�ָ�ʽ��ӡIPv6�������� */
            length += AT_Ipv6Addr2DecString(pcIpv6FormatStr + length, aucIpv6Mask);
        }
    } else {
        /* 16����ð�ŷָ���ʽ��ӡIPv6��ַ */
        length = AT_Ipv6Addr2HexString(pcIpv6FormatStr, aucIpv6Addr);

        /* ��ӡ�������� */
        if (aucIpv6Mask != VOS_NULL_PTR) {
            /* �ж����������ʽ */
            if (commPsCtx->opIpv6SubnetNotation == VOS_FALSE) {
                /* �������������ַ��IPv6��ַͨ���ո�ָ� */
                /* ʹ�õ�ָ�IP��ַ���������� */
                *(pcIpv6FormatStr + length) = ' ';
                length++;

                /* 10���Ƶ�ָ�ʽ��ӡIPv6�������� */
                length += AT_Ipv6Addr2HexString(pcIpv6FormatStr + length, aucIpv6Mask);
            } else {
                /* б�߷ָ�����ǰ׺��������IPv6��ַ */
                /* ʹ��б�߷ָ�IP��ַ���������� */
                *(pcIpv6FormatStr + length) = '/';
                length++;

                iRslt = snprintf_s(pcIpv6FormatStr + length, AT_IPV6_ADDR_MASK_FORMAT_STR_LEN - length,
                                   (AT_IPV6_ADDR_MASK_FORMAT_STR_LEN - length) - 1, "%d",
                                   AT_CalcIpv6PrefixLength(aucIpv6Mask, TAF_IPV6_ADDR_LEN));
                if (iRslt <= 0) {
                    AT_WARN_LOG("AT_Ipv6AddrMask2FormatString: Print IPv6 Subnet Failed!");
                    *pcIpv6FormatStr = '\0';
                    return 0;
                }

                length += iRslt;
            }
        }
    }

    /* ��󲹳��ַ��������� */
    *(pcIpv6FormatStr + length) = '\0';

    return length;
}


VOS_UINT8 AT_CalcIpv6PrefixLength(VOS_UINT8 *localIpv6Mask, VOS_UINT32 ipv6MaskLen)
{
    VOS_UINT32 i          = 0;
    VOS_UINT32 j          = 0;
    VOS_UINT8  maskLength = 0;

    if (localIpv6Mask == VOS_NULL_PTR) {
        return maskLength;
    }

    for (i = 0; i < TAF_MIN(AT_IPV6_STR_MAX_TOKENS, ipv6MaskLen); i++) {
        if (*(localIpv6Mask + i) == 0xFF) {
            maskLength = maskLength + AT_BYTE_TO_BITS_LENS;
        } else {
            for (j = 0; j < AT_BYTE_TO_BITS_LENS; j++) {
                if (((*(localIpv6Mask + i)) & ((VOS_UINT32)1 << (AT_BYTE_LOCAL_IPV6_MAX_OFFSET - j))) != 0) {
                    maskLength++;
                } else {
                    break;
                }
            }
            break;
        }
    }

    return maskLength;
}


VOS_VOID AT_GetIpv6MaskByPrefixLength(VOS_UINT8 localIpv6Prefix, VOS_UINT8 *localIpv6Mask)
{
    VOS_UINT8  num;
    VOS_UINT32 i = 0;

    num = localIpv6Prefix / AT_BYTE_TO_BITS_LENS;

    if (localIpv6Mask == VOS_NULL_PTR) {
        return;
    }

    for (i = 0; i < num; i++) {
        *(localIpv6Mask + i) = 0xFF;
    }

    num = localIpv6Prefix % AT_BYTE_TO_BITS_LENS;

    if (num != 0) {
        *(localIpv6Mask + i) = 0xFF & ((VOS_UINT32)0xFF << (AT_BYTE_TO_BITS_LENS - num));
    }

    return;
}


VOS_UINT32 AT_FindIpv6AddrZeroFieldsToBeCompressed(VOS_UINT8 *zeroFieldStart, VOS_UINT8 *zeroFieldCount,
                                                   VOS_UINT16 ausAddrValue[], VOS_UINT8 tokensNum)
{
    VOS_UINT8 start;
    VOS_UINT8 count;
    VOS_UINT8 i;

    start = 0xFF;
    count = 0;

    for (i = 0; i < tokensNum - 1; i++) {
        if ((ausAddrValue[i] == 0x0000) && (ausAddrValue[i + 1] == 0x0000)) {
            /* ��¼��ֵ����Ϊ0��IP��ַ����ʼλ�� */
            if (start == 0xFF) {
                start = i;
            }

            /* ������ֵ����Ϊ0��IP��ַ�θ��� */
            count++;
        } else {
            /* ���´�ѹ����IP��ַ��λ��, �Լ�IP��ַ�θ��� */
            if (start != 0xFF) {
                if (count > *zeroFieldCount) {
                    *zeroFieldStart = start;
                    *zeroFieldCount = count;
                }

                start = 0xFF;
                count = 0;
            }
        }
    }

    /*
     * ��ֵ����Ϊ0��IP��ַ���ڽ�βʱ, ��Ҫ����һ�δ�ѹ����IP��ַ��λ��,
     * �Լ�IP��ַ�θ���
     */
    if (start != 0xFF) {
        if (count > *zeroFieldCount) {
            *zeroFieldStart = start;
            *zeroFieldCount = count;
        }
    }

    return VOS_OK;
}


VOS_UINT32 AT_ConvertIpv6AddrToCompressedStr(VOS_UINT8 aucAddrStr[], VOS_UINT8 aucIpAddr[], VOS_UINT8 tokensNum)
{
    VOS_UINT8 *buffer = VOS_NULL_PTR;
    VOS_UINT16 addrValue[TAF_IPV6_STR_RFC2373_TOKENS]; /* TAF_IPV6_STR_RFC2373_TOKENS]; */
    VOS_UINT16 addrNum;
    VOS_UINT8  delimiter;
    VOS_UINT8  radix;
    VOS_UINT8  zeroFieldStart;
    VOS_UINT8  zeroFieldCount;
    VOS_UINT32 i;

    (VOS_VOID)memset_s(addrValue, sizeof(addrValue), 0x00, sizeof(addrValue));

    buffer         = aucAddrStr;
    delimiter      = TAF_IPV6_STR_DELIMITER;
    radix          = AT_IPV6_STR_HEX_RADIX;
    zeroFieldStart = 0xFF;
    zeroFieldCount = 0;

    /* ����IP�ַ�����ʽ�������, ��ȡ�ֶε�IP��ַ��ֵ */
    for (i = 0; i < tokensNum; i++) {
        addrNum = *aucIpAddr++;

        addrNum <<= 8;
        addrNum |= *aucIpAddr++;

        addrValue[i] = addrNum;
    }

    /* �ҳ���Ҫʹ��"::"��ʾ��IP��ַ�ε���ʼλ��  */
    AT_FindIpv6AddrZeroFieldsToBeCompressed(&zeroFieldStart, &zeroFieldCount, addrValue, tokensNum);

    /* ����IP��ַ�ֶ�, �����зָ�����ǵ�IP��ַ�ַ��� */
    for (i = 0; i < tokensNum; i++) {
        if (zeroFieldStart == i) {
            *buffer++ = delimiter;

            i += zeroFieldCount;

            /* ����ѵ�IP��ַ�ֶε����һ��, ��Ҫ����ָ��� */
            if ((tokensNum - 1) == i) {
                *buffer++ = delimiter;
            }
        } else {
            /* �����IP��ַ�ֶεĵ�һ��, ����Ҫ����ָ��� */
            if (i != 0) {
                *buffer++ = delimiter;
            }
            buffer = (VOS_UINT8 *)AT_Itoa(addrValue[i], (VOS_CHAR *)buffer, radix,
                                          (TAF_MAX_IPV6_ADDR_COLON_STR_LEN - (VOS_UINT32)(aucAddrStr - buffer)));
        }
    }

    /* �����ַ��������� */
    if (aucAddrStr != buffer) {
        *buffer = '\0';
    }

    return VOS_OK;
}


VOS_UINT32 AT_PortAtoI(VOS_CHAR *pcString, VOS_UINT32 *value)
{
    VOS_CHAR  *tmp = VOS_NULL_PTR;
    VOS_UINT32 ret;

    if ((pcString == VOS_NULL_PTR) || (value == VOS_NULL_PTR)) {
        AT_ERR_LOG("AT_PortAtoI: pcString or pulValue is NULL, return ERROR");
        return VOS_ERR;
    }

    tmp = pcString;
    ret = 0;

    *value = 0;

    for (tmp = pcString; *tmp != '\0'; tmp++) {
        /* ������, �򷵻�ʧ�� */
        if ((*tmp < '0') || (*tmp > '9')) {
            AT_ERR_LOG("AT_PortAtoI: Not all number type in pcString , return ERROR");
            return VOS_ERR;
        }

        ret = (ret * 10) + (*tmp - '0'); /* ��10��Ϊ�˽��ַ���ת����ʮ������ */

        if (ret > IMS_PCSCF_ADDR_PORT_MAX) {
            AT_ERR_LOG("AT_PortAtoI: Port number is larger than 65535, return ERROR");
            return VOS_ERR;
        }
    }

    if ((ret > 0) && (ret <= IMS_PCSCF_ADDR_PORT_MAX)) {
        *value = (VOS_UINT32)ret;

        return VOS_OK;
    }

    AT_ERR_LOG("AT_PortAtoI: return ERROR");
    return VOS_ERR;
}


VOS_UINT64 AT_AtoI(VOS_CHAR *string)
{
    VOS_CHAR  *pcTmp = VOS_NULL_PTR;
    VOS_UINT64 ret;

    pcTmp = string;
    ret   = 0;

    for (pcTmp = string; *pcTmp != '\0'; pcTmp++) {
        /* ������,�򲻴��� */
        if ((*pcTmp < '0') || (*pcTmp > '9')) {
            continue;
        }

        ret = (ret * 10) + (*pcTmp - '0');  /* ��10��Ϊ�˽��ַ���ת����ʮ������ */
    }

    return ret;
}


VOS_CHAR* AT_Itoa(VOS_UINT16 value, VOS_CHAR *pcStr, VOS_UINT16 radix, VOS_UINT32 length)
{
    VOS_INT32 bufLen = 0;
    if (radix == AT_ITOA_HEX_RADIX) {
        if (length > 0) {
            bufLen = snprintf_s(pcStr, length, length - 1, "%x", value);
        }
    } else if (radix == AT_ITOA_DEC_RADIX) {
        if (length > 0) {
            bufLen = snprintf_s(pcStr, length, length - 1, "%d", value);
        }
    } else {
    }

    TAF_SPRINTF_CHK_RTN_VAL_CONTINUE(bufLen, length, length - 1);

    if (bufLen > 0) {
        pcStr += bufLen;
    }
    return pcStr;
}


VOS_INT32 AT_AtoInt(VOS_CHAR *string, VOS_INT32 *out)
{
    VOS_CHAR *pcTmp = VOS_NULL_PTR;
    VOS_INT32 flag; /* negative flag */

    pcTmp = string;
    flag  = 0;

    if (*pcTmp == '-') {
        flag = VOS_TRUE;
        pcTmp++;
    }

    for (; *pcTmp != '\0'; pcTmp++) {
        /* ������, ֱ�ӷ��ش��� */
        if ((*pcTmp < '0') || (*pcTmp > '9')) {
            return VOS_ERR;
        }

        *out = (*out * 10) + (*pcTmp - '0'); /* ��10��Ϊ�˽��ַ���ת����ʮ������ */
    }

    if (flag == VOS_TRUE) {
        *out = (0 - (*out));
    }

    return VOS_OK;
}


VOS_VOID AT_GetDhcpPara(AT_DHCP_Para *config, TAF_IFACE_Ipv4DhcpParam *ipv4Dhcp)
{
    /* ��DHCP����ת��Ϊ������ */
    config->dhcpCfg.iPAddr     = VOS_HTONL(ipv4Dhcp->addr);
    config->dhcpCfg.subNetMask = VOS_HTONL(ipv4Dhcp->netMask);
    config->dhcpCfg.gateWay    = VOS_HTONL(ipv4Dhcp->gateWay);
    config->dhcpCfg.primDNS    = VOS_HTONL(ipv4Dhcp->primDNS);
    config->dhcpCfg.sndDNS     = VOS_HTONL(ipv4Dhcp->secDNS);

    return;
}


VOS_UINT16 AT_CalcIpHdrCRC16(VOS_UINT8 *data, VOS_UINT16 size)
{
    VOS_UINT8  *buffer = data;
    VOS_UINT32  checkSum;

    checkSum = 0;

    while (size > 1) {
        checkSum += TAF_GET_HOST_UINT16(buffer);
        buffer += sizeof(VOS_UINT16);
        size -= sizeof(VOS_UINT16);
    }

    if (size) {
#if (VOS_LITTLE_ENDIAN == VOS_BYTE_ORDER)
        checkSum += *buffer;
#else
        checkSum += 0 | ((*buffer) << 8);
#endif
    }

    checkSum = (checkSum >> 16) + (checkSum & 0xffff);
    checkSum += (checkSum >> 16);

    return (VOS_UINT16)(~checkSum);
}


ATTRIBUTE_NO_SANITIZE_RUNTIME VOS_UINT32 AT_BuildUdpHdr(AT_UdpPacketFormat *udpPkt, VOS_UINT16 len, VOS_UINT32 srcAddr,
                                                        VOS_UINT32 dstAddr, VOS_UINT16 srcPort, VOS_UINT16 dstPort)
{
    static VOS_UINT16 identification = 0;

    /* ���ָ��Ϸ��� */
    if (udpPkt == VOS_NULL_PTR) {
        return VOS_ERR;
    }

    /* ��дIPͷ */
    udpPkt->ipHdr.ucIpVer        = AT_IP_VERSION;
    udpPkt->ipHdr.ucIpHdrLen     = AT_IP_HEAD_LEN;
    udpPkt->ipHdr.serviceType    = 0x00;
    udpPkt->ipHdr.totalLen       = VOS_HTONS(len + AT_IP_HDR_LEN + AT_UDP_HDR_LEN);
    udpPkt->ipHdr.identification = VOS_HTONS(identification);
    udpPkt->ipHdr.offset         = 0;
    udpPkt->ipHdr.tTL            = AT_IP_DEF_TTL;
    udpPkt->ipHdr.protocol       = AT_IPPROTO_UDP;
    udpPkt->ipHdr.srcAddr        = VOS_HTONL(srcAddr);
    udpPkt->ipHdr.dstAddr        = VOS_HTONL(dstAddr);
    udpPkt->ipHdr.checkSum       = AT_CalcIpHdrCRC16((VOS_UINT8 *)&udpPkt->ipHdr, AT_IP_HDR_LEN);

    /* ��дUDPͷ */
    udpPkt->udpHdr.srcPort  = VOS_HTONS(srcPort);
    udpPkt->udpHdr.dstPort  = VOS_HTONS(dstPort);
    udpPkt->udpHdr.len      = VOS_HTONS(len + AT_UDP_HDR_LEN);
    udpPkt->udpHdr.checksum = 0;

    identification++;

    return VOS_OK;
}

VOS_UINT32 AT_GetConnectRateIndex(VOS_UINT8 subSysMode)
{
    VOS_UINT32             nvDialRateIndex;

    switch (subSysMode) {
        case TAF_SYS_SUBMODE_GSM:
        case TAF_SYS_SUBMODE_GPRS:
            nvDialRateIndex = g_dialConnectDisplayRate.gprsConnectRate;
            break;

        case TAF_SYS_SUBMODE_EDGE:
            nvDialRateIndex = g_dialConnectDisplayRate.edgeConnectRate;
            break;

        case TAF_SYS_SUBMODE_WCDMA:
            nvDialRateIndex = g_dialConnectDisplayRate.wcdmaConnectRate;
            break;

        case TAF_SYS_SUBMODE_HSDPA:
        case TAF_SYS_SUBMODE_HSDPA_HSUPA:
            nvDialRateIndex = g_dialConnectDisplayRate.dpaConnectRate;
            break;

        default:
            nvDialRateIndex = 0;
            break;
    }
    return nvDialRateIndex;
}

VOS_UINT32 AT_GetDisplayRate(VOS_UINT16 clientId, AT_DisplayRate *speed)
{
    const VOS_UINT8       *dlSpeed = VOS_NULL_PTR;
    const VOS_UINT8       *ulSpeed = VOS_NULL_PTR;
    PPP_RateDisplayUint32  rateDisplay;
    TAF_AGENT_SysMode      sysMode;
    VOS_UINT32             ret;
    VOS_UINT32             dlSpeedLen;
    VOS_UINT32             ulSpeedLen;
    VOS_UINT32             nvDialRateIndex;
    errno_t                memResult;
    VOS_UINT8              dlCategoryIndex;
    VOS_UINT8              ulCategoryIndex;

    /* ������ʼ�� */
    (VOS_VOID)memset_s(&sysMode, sizeof(sysMode), 0x00, sizeof(sysMode));

    /* ��C�˻�ȡucRatType��ucSysSubMode */
    ret = TAF_AGENT_GetSysMode(clientId, &sysMode);

    if (ret != VOS_OK) {
        sysMode.ratType    = TAF_PH_INFO_GSM_RAT;
        sysMode.sysSubMode = TAF_SYS_SUBMODE_BUTT;
    }

    nvDialRateIndex = AT_GetConnectRateIndex(sysMode.sysSubMode);

    if ((nvDialRateIndex == 0) || (nvDialRateIndex > AT_DIAL_RATE_DISPALY_NV_ARRAY_LEN)) {
        if ((sysMode.ratType == TAF_PH_INFO_WCDMA_RAT) || (sysMode.ratType == TAF_PH_INFO_TD_SCDMA_RAT)) {
            rateDisplay = AT_GetRateDisplayIndexForWcdma(&g_atDlRateCategory);
            dlSpeed = (VOS_UINT8 *)AT_GetPppDialRateDisplay(rateDisplay);
            ulSpeed = (VOS_UINT8 *)AT_GetPppDialRateDisplay(rateDisplay);
        }
#if (FEATURE_LTE == FEATURE_ON)
        else if (sysMode.ratType == TAF_PH_INFO_LTE_RAT) {
            dlCategoryIndex = AT_GetLteUeDlCategoryIndex();
            ulCategoryIndex = AT_GetLteUeUlCategoryIndex();

            dlSpeed = (VOS_UINT8 *)AT_GetLteRateDisplay(dlCategoryIndex).strDlSpeed;
            ulSpeed = (VOS_UINT8 *)AT_GetLteRateDisplay(ulCategoryIndex).strUlSpeed;
        }
#endif
        else {
            rateDisplay = AT_GetRateDisplayIndexForGsm(&sysMode);
            dlSpeed = (VOS_UINT8 *)AT_GetPppDialRateDisplay(rateDisplay);
            ulSpeed = (VOS_UINT8 *)AT_GetPppDialRateDisplay(rateDisplay);
        }
    } else {
        dlSpeed = (VOS_UINT8 *)AT_GetDialRateDisplayNv(nvDialRateIndex - 1);
        ulSpeed = (VOS_UINT8 *)AT_GetDialRateDisplayNv(nvDialRateIndex - 1);
    }

    dlSpeedLen = VOS_StrLen((TAF_CHAR *)dlSpeed);
    ulSpeedLen = VOS_StrLen((TAF_CHAR *)ulSpeed);

    /* ��CONNECT����������Ϣ */
    if (dlSpeedLen > 0) {
        memResult = memcpy_s(speed->dlSpeed, AT_AP_SPEED_STRLEN + 1, dlSpeed, (VOS_UINT16)dlSpeedLen);
        TAF_MEM_CHK_RTN_VAL(memResult, AT_AP_SPEED_STRLEN + 1, (VOS_UINT16)dlSpeedLen);
    }
    if (ulSpeedLen > 0) {
        memResult = memcpy_s(speed->ulSpeed, AT_AP_SPEED_STRLEN + 1, ulSpeed, (VOS_UINT16)ulSpeedLen);
        TAF_MEM_CHK_RTN_VAL(memResult, AT_AP_SPEED_STRLEN + 1, (VOS_UINT16)ulSpeedLen);
    }
    speed->dlSpeed[dlSpeedLen] = '\0';
    speed->ulSpeed[ulSpeedLen] = '\0';

    return VOS_OK;
}


VOS_UINT32 AT_Get3gppSmCauseByPsCause(TAF_PS_CauseUint32 cause)
{
    VOS_UINT32 ul3gppSmCause;

    if ((cause >= TAF_PS_CAUSE_SM_NW_SECTION_BEGIN) && (cause <= TAF_PS_CAUSE_SM_NW_SECTION_END)) {
        ul3gppSmCause = cause - TAF_PS_CAUSE_SM_NW_SECTION_BEGIN;
    }
    /* E5�����翨��PDP DEACTIVEʱ�ϱ������36��ԭ��ֵ */
    else if (cause == TAF_PS_CAUSE_SUCCESS) {
        ul3gppSmCause = TAF_PS_CAUSE_SM_NW_REGULAR_DEACTIVATION - TAF_PS_CAUSE_SM_NW_SECTION_BEGIN;
    } else {
        ul3gppSmCause = AT_NDISSTAT_ERR_UNKNOWN;
    }

    return ul3gppSmCause;
}


VOS_UINT32 AT_IsNdisIface(VOS_UINT8 ifaceId)
{
    switch (ifaceId) {
        case PS_IFACE_ID_NDIS0:
#if (FEATURE_MULTI_NCM == FEATURE_ON)
        case PS_IFACE_ID_NDIS1:
        case PS_IFACE_ID_NDIS2:
        case PS_IFACE_ID_NDIS3:
#endif
            return VOS_TRUE;
        default:
            return VOS_FALSE;
    }
}


LOCAL VOS_VOID AT_ProcNdisIfaceDown(const AT_PS_DataChanlCfg *chanCfg)
{
    TAF_IFACE_Deactive ifaceDown;
    TAF_Ctrl           ctrl;

    (VOS_VOID)memset_s(&ifaceDown, sizeof(ifaceDown), 0x00, sizeof(ifaceDown));
    (VOS_VOID)memset_s(&ctrl, sizeof(ctrl), 0x00, sizeof(ctrl));

    ifaceDown.ifaceId  = (VOS_UINT8)chanCfg->ifaceId;
    ifaceDown.cause    = TAF_PS_CALL_END_CAUSE_NORMAL;
    ifaceDown.userType = TAF_IFACE_USER_TYPE_NDIS;

    /* ������ƽṹ�� */
    (VOS_VOID)AT_PS_BuildIfaceCtrl(WUEPS_PID_AT, g_atClientTab[chanCfg->portIndex].clientId, 0, &ctrl);
    (VOS_VOID)TAF_IFACE_Down(&ctrl, &ifaceDown);
}


VOS_VOID AT_ProcNdisDisconnectInd(VOS_UINT32 ifaceId, VOS_UINT32 specIfaceFlag)
{
    AT_PS_DataChanlCfg *chanCfg = VOS_NULL_PTR;
    VOS_UINT32          i;

    for (i = 1; i <= TAF_MAX_CID; i++) {
        chanCfg = AT_PS_GetDataChanlCfg(AT_NDIS_GET_CLIENT_ID(), (VOS_UINT8)i);

        if ((chanCfg->used == VOS_FALSE) || (chanCfg->portIndex >= AT_CLIENT_ID_BUTT)) {
            continue;
        }
        if (AT_IsNdisIface((VOS_UINT8)chanCfg->ifaceId) == VOS_FALSE) {
            continue;
        }
        /* usb�Ͽ� ȥ��������ndis���� ������������ֻ�Ͽ��ض����� */
        if ((specIfaceFlag == VOS_FALSE) || (chanCfg->ifaceId == ifaceId)) {
            AT_ProcNdisIfaceDown(chanCfg);
        }
    }

    return;
}


VOS_UINT32 AT_ResetFlowCtl(VOS_UINT32 param1, VOS_UINT32 param2)
{
    VOS_UINT8     rabIdIndex;
    VOS_UINT32    rabIdMask;
    VOS_UINT32    ret;
    FC_IdUint8    fcId;
    ModemIdUint16 modemId;

    fcId      = (FC_IdUint8)param2;
    modemId   = g_fcIdMaptoFcPri[fcId].modemId;
    rabIdMask = g_fcIdMaptoFcPri[fcId].rabIdMask;

    if (g_fcIdMaptoFcPri[fcId].used == VOS_TRUE) {
        for (rabIdIndex = AT_PS_MIN_RABID; rabIdIndex <= AT_PS_MAX_RABID; rabIdIndex++) {
            if (((rabIdMask >> rabIdIndex) & 0x1) == 1) {
#if (FEATURE_ACPU_FC_POINT_REG == FEATURE_ON)
                FC_ChannelMapDelete(rabIdIndex, modemId);
#endif
            }
        }

        ret = FC_DeRegPoint(fcId, modemId);
        if (ret != VOS_OK) {
            AT_ERR_LOG("AT_ResetFlowCtl: ERROR: DeReg point failed.");
            return VOS_ERR;
        }

        g_fcIdMaptoFcPri[fcId].used      = VOS_FALSE;
        g_fcIdMaptoFcPri[fcId].fcPri     = FC_PRI_BUTT;
        g_fcIdMaptoFcPri[fcId].rabIdMask = 0;
        g_fcIdMaptoFcPri[fcId].modemId   = MODEM_ID_BUTT;
    }

    return VOS_OK;
}


FC_PriTypeUint8 AT_GetFCPriFromQos(const TAF_UMTS_Qos *umtsQos)
{
    FC_PriTypeUint8 fCPri;
    VOS_UINT8       trafficClass;

    /* ��ʼ�� */
    fCPri = FC_PRI_FOR_PDN_NONGBR;

    trafficClass = umtsQos->trafficClass;

    /* ����QOS trafficClass������ȡQCI */
    if (trafficClass == AT_QOS_TRAFFIC_CLASS_CONVERSATIONAL) {
        fCPri = FC_PRI_FOR_PDN_GBR;
    } else if (trafficClass == AT_QOS_TRAFFIC_CLASS_STREAMING) {
        fCPri = FC_PRI_FOR_PDN_GBR;
    } else if (trafficClass == AT_QOS_TRAFFIC_CLASS_INTERACTIVE) {
        fCPri = FC_PRI_FOR_PDN_NONGBR;
    } else {
        fCPri = FC_PRI_FOR_PDN_LOWEST;
    }

    return fCPri;
}


VOS_VOID AT_SendRelPppReq(VOS_UINT16 pppId, PPP_AtCtrlOperTypeUint32 operType)
{
    PPP_RcvAtCtrlOperEvent(pppId, operType);
    PPP_RcvAtCtrlOperEvent(pppId, PPP_AT_CTRL_HDLC_DISABLE);
}


 VOS_VOID AT_ProcPppDialCnf(TAF_PS_CauseUint32 cause, VOS_UINT8 indexNum)
{
    VOS_UINT32 result;

    result = AT_FAILURE;
    /* MODEM���Ŵ����� */
    if (AT_IsPppUser(indexNum) == VOS_FALSE) {
        return;
    }

    if (cause == TAF_PS_CAUSE_SUCCESS) {
        return;
    }

    /* ��¼PS����д����� */
    AT_PS_SetPsCallErrCause(indexNum, cause);

    if ((g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_D_PPP_CALL_SET) ||
        (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_PPP_ORG_SET)) {
        result = AT_NO_CARRIER;

        AT_SendRelPppReq(g_atClientTab[indexNum].pppId, PPP_AT_CTRL_REL_PPP_REQ);

        /* ��������ģʽ */
        DMS_PORT_ResumeCmdMode(g_atClientTab[indexNum].portNo);
    } else if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_D_IP_CALL_SET) {
        if (cause == TAF_PS_CAUSE_PDP_ACTIVATE_LIMIT) {
            result = AT_CME_PDP_ACT_LIMIT;
        } else {
            result = AT_ERROR;
        }

        AT_SendRelPppReq(g_atClientTab[indexNum].pppId, PPP_AT_CTRL_REL_PPP_RAW_REQ);
    } else {
        ;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, result);
}


VOS_VOID AT_ModemPsRspPdpDeactivatedEvtProc(VOS_UINT8 indexNum, TAF_PS_CallPdpDeactivateCnf *event)
{
    VOS_UINT32              aTHCmdFlg;
    VOS_UINT32              result;
    VOS_UINT16              length;
    DMS_PortIdUint16        portId;
    DMS_PortModeUint8       mode;
    DMS_PortDataModeUint8   dataMode;

    length    = 0;
    result    = AT_FAILURE;
    aTHCmdFlg = (AT_PS_GET_CURR_CMD_OPT(indexNum) == AT_CMD_H_PS_SET) ? VOS_TRUE : VOS_FALSE;

    portId   = AT_GetDmsPortIdByClientId((AT_ClientIdUint16)indexNum);;
    mode     = DMS_PORT_GetMode(portId);
    dataMode = DMS_PORT_GetDataMode(portId);

    /* ȥע��Modem�˿ڵ����ص� */
    AT_DeRegModemPsDataFCPoint(indexNum, event->rabId);

    if (dataMode == DMS_PORT_DATA_PPP) {
        /* �ͷ�PPPʵ�� & HDLCȥʹ�� */
        AT_SendRelPppReq(g_atClientTab[indexNum].pppId, PPP_AT_CTRL_REL_PPP_REQ);

        if (mode == DMS_PORT_MODE_ONLINE_DATA) {
            /*
             * �������ⵥAT2D13296���ڱ���ȥ�����龰�£���PPP������
             * PPP_AT_CTRL_REL_PPP_REQ�󣬲�������������̬�����ǵ�
             * ��PPP��ӦAT_PPP_PROTOCOL_REL_IND_MSG֮������������̬
             */
            /* ��������ʱ�������ڵȴ�PPP��ӦAT_PPP_PROTOCOL_REL_IND_MSG */
            AT_STOP_TIMER_CMD_READY(indexNum);

            if (At_StartTimer(AT_PPP_PROTOCOL_REL_TIME, indexNum) != AT_SUCCESS) {
                AT_ERR_LOG("At_UsbModemStatusPreProc:ERROR:Start Timer fail");
            }

            /* ���õ�ǰ�������� */
            g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_WAIT_PPP_PROTOCOL_REL_SET;

            return;
        }
    } else if (dataMode == DMS_PORT_DATA_PPP_RAW) {
        /* �ͷ�PPPʵ�� & HDLCȥʹ�� */
        AT_SendRelPppReq(g_atClientTab[indexNum].pppId, PPP_AT_CTRL_REL_PPP_RAW_REQ);
    } else {
        /* ��������ģʽ�����ô��� */
        AT_WARN_LOG("TAF_PS_EVT_PDP_DEACTIVATED OTHER MODE");
    }

    /* ��������ģʽ */
    DMS_PORT_ResumeCmdMode(portId);

    if (aTHCmdFlg == VOS_TRUE) {
        result = AT_OK;
    } else {
        result = AT_NO_CARRIER;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);
    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, result);

    /* ATH�Ͽ�PPP����ʱ, ���DCD�ź�ģʽΪCONNECT ON, ��Ҫ����DCD�ź� */
    if ((result == AT_OK) && (DMS_PORT_IsDcdModeConnectOn() == VOS_TRUE)) {
        DMS_PORT_DeassertDcd(portId);
    }

    return;
}


VOS_VOID AT_ModemPsRspPdpActEvtRejProc(VOS_UINT8 indexNum, TAF_PS_CallPdpActivateRej *event)
{
    VOS_UINT32 result;
    VOS_UINT16 length;

    result = AT_FAILURE;
    length = 0;

    if ((g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_D_PPP_CALL_SET) ||
        (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_PPP_ORG_SET)) {
        result = AT_NO_CARRIER;

        AT_SendRelPppReq(g_atClientTab[indexNum].pppId, PPP_AT_CTRL_REL_PPP_REQ);

        /* ��������ģʽ */
        DMS_PORT_ResumeCmdMode(g_atClientTab[indexNum].portNo);

    } else if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_D_IP_CALL_SET) {
        result = AT_ERROR;
        AT_SendRelPppReq(g_atClientTab[indexNum].pppId, PPP_AT_CTRL_REL_PPP_RAW_REQ);
    } else {
        result = AT_ERROR;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);
    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, result);

    return;
}


VOS_VOID AT_FillPppIndConfigInfoPara(AT_PPP_IndConfigInfo *pppIndConfigInfo, TAF_PS_CallPdpActivateCnf *event)
{
    errno_t memResult;
    /* ��дIP��ַ */
    memResult = memcpy_s(pppIndConfigInfo->ipAddr, sizeof(pppIndConfigInfo->ipAddr), event->pdpAddr.ipv4Addr,
                         TAF_IPV4_ADDR_LEN);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(pppIndConfigInfo->ipAddr), TAF_IPV4_ADDR_LEN);

    /* ��дDNS��ַ */
    if (event->dns.bitOpPrimDnsAddr == VOS_TRUE) {
        pppIndConfigInfo->pcoIpv4Item.opPriDns = VOS_TRUE;

        memResult = memcpy_s(pppIndConfigInfo->pcoIpv4Item.priDns, sizeof(pppIndConfigInfo->pcoIpv4Item.priDns),
                             event->dns.primDnsAddr, TAF_IPV4_ADDR_LEN);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(pppIndConfigInfo->pcoIpv4Item.priDns), TAF_IPV4_ADDR_LEN);
    }

    if (event->dns.bitOpSecDnsAddr == VOS_TRUE) {
        pppIndConfigInfo->pcoIpv4Item.opSecDns = VOS_TRUE;

        memResult = memcpy_s(pppIndConfigInfo->pcoIpv4Item.secDns, sizeof(pppIndConfigInfo->pcoIpv4Item.secDns),
                             event->dns.secDnsAddr, TAF_IPV4_ADDR_LEN);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(pppIndConfigInfo->pcoIpv4Item.secDns), TAF_IPV4_ADDR_LEN);
    }

    /* ��дNBNS��ַ */
    if (event->nbns.bitOpPrimNbnsAddr == VOS_TRUE) {
        pppIndConfigInfo->pcoIpv4Item.opPriNbns = VOS_TRUE;

        memResult = memcpy_s(pppIndConfigInfo->pcoIpv4Item.priNbns, sizeof(pppIndConfigInfo->pcoIpv4Item.priNbns),
                             event->nbns.primNbnsAddr, TAF_IPV4_ADDR_LEN);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(pppIndConfigInfo->pcoIpv4Item.priNbns), TAF_IPV4_ADDR_LEN);
    }

    if (event->nbns.bitOpSecNbnsAddr == VOS_TRUE) {
        pppIndConfigInfo->pcoIpv4Item.opSecNbns = VOS_TRUE;

        memResult = memcpy_s(pppIndConfigInfo->pcoIpv4Item.secNbns, sizeof(pppIndConfigInfo->pcoIpv4Item.secNbns),
                             event->nbns.secNbnsAddr, TAF_IPV4_ADDR_LEN);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(pppIndConfigInfo->pcoIpv4Item.secNbns), TAF_IPV4_ADDR_LEN);
    }

    /* ��дGATE WAY��ַ */
    if (event->gateWay.bitOpGateWayAddr == VOS_TRUE) {
        pppIndConfigInfo->pcoIpv4Item.opGateWay = VOS_TRUE;

        memResult = memcpy_s(pppIndConfigInfo->pcoIpv4Item.gateWay, sizeof(pppIndConfigInfo->pcoIpv4Item.gateWay),
                             event->gateWay.gateWayAddr, TAF_IPV4_ADDR_LEN);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(pppIndConfigInfo->pcoIpv4Item.gateWay), TAF_IPV4_ADDR_LEN);
    }
}


VOS_UINT32 AT_RegModemPsDataFCPoint(VOS_UINT8 indexNum, TAF_PS_CallPdpActivateCnf *event, FC_IdUint8 fcId)
{
    FC_RegPointPara regFcPoint;
    VOS_UINT32      ret;
    FC_PriTypeUint8 fcPri;
    ModemIdUint16   modemId;
    AT_UartCtx     *uartCtx = VOS_NULL_PTR;

    uartCtx = AT_GetUartCtxAddr();

    /* UART�˿����عر�ʱ��ע�����ص� */
    if ((AT_CheckHsUartUser(indexNum) == VOS_TRUE) && (uartCtx->flowCtrl.dteByDce == AT_UART_FC_DTE_BY_DCE_NONE)) {
        return VOS_ERR;
    }

    modemId = MODEM_ID_0;

    (VOS_VOID)memset_s(&regFcPoint, sizeof(regFcPoint), 0x00, sizeof(regFcPoint));

    ret = AT_GetModemIdFromClient(event->ctrl.clientId, &modemId);

    if (ret != VOS_OK) {
        AT_ERR_LOG("AT_RegModemPsDataFCPoint: Get modem id fail.");
        return VOS_ERR;
    }

#if (FEATURE_ACPU_FC_POINT_REG == FEATURE_ON)
    /* ����ͨ����RABIDӳ���ϵ */
    FC_ChannelMapCreate(fcId, event->rabId, modemId);
#endif

    regFcPoint.fcId = fcId;

    /* ����������������ȼ�RAB QoS���ȼ�������,���ȼ��ı�ʱ����Ҫ�ı����ȼ� */
    /*
     * FC_PRI_3        ��������ȼ��ĳ���
     * FC_PRI_4        ��NONGBR����
     * FC_PRI_5        ��GBR����
     */
    if (event->opUmtsQos == TAF_USED) {
        fcPri = AT_GetFCPriFromQos(&event->umtsQos);
    } else {
        fcPri = FC_PRI_FOR_PDN_NONGBR;
    }
    regFcPoint.fcPri    = fcPri;
    regFcPoint.policyId = FC_POLICY_ID_MEM;
#if (FEATURE_AT_HSUART == FEATURE_ON)
    regFcPoint.clrFunc = (indexNum == AT_CLIENT_ID_HSUART) ? DMS_PORT_StopHsuartFlowCtrl : DMS_PORT_StopModemFlowCtrl;
    regFcPoint.setFunc = (indexNum == AT_CLIENT_ID_HSUART) ? DMS_PORT_StartHsuartFlowCtrl : DMS_PORT_StartModemFlowCtrl;
#else
    regFcPoint.clrFunc = DMS_PORT_StopModemFlowCtrl;
    regFcPoint.setFunc = DMS_PORT_StartModemFlowCtrl;
#endif
    regFcPoint.param1  = (VOS_UINT32)DMS_PORT_GetPortHandle(g_atClientTab[indexNum].portNo);
    regFcPoint.modemId = modemId;
    regFcPoint.param2  = fcId;
    regFcPoint.rstFunc = AT_ResetFlowCtl;

    /* ע�����ص�,��Ҫ�ֱ�ע��MEM,CPU,CDS��GPRS�� */
    ret = FC_RegPoint(&regFcPoint);
    if (ret != VOS_OK) {
        AT_ERR_LOG("AT_RegModemFCPoint: ERROR: FC RegPoint MEM Failed.");
        return VOS_ERR;
    }

    regFcPoint.policyId = FC_POLICY_ID_CPU_A;
    ret                 = FC_RegPoint(&regFcPoint);
    if (ret != VOS_OK) {
        AT_ERR_LOG("AT_RegModemFCPoint: ERROR: FC RegPoint A CPU Failed.");
        return VOS_ERR;
    }

    regFcPoint.policyId = FC_POLICY_ID_CDS;
    ret                 = FC_RegPoint(&regFcPoint);
    if (ret != VOS_OK) {
        return VOS_ERR;
    }

    regFcPoint.policyId = FC_POLICY_ID_GPRS;
    ret                 = FC_RegPoint(&regFcPoint);
    if (ret != VOS_OK) {
        AT_ERR_LOG("AT_RegModemFCPoint: ERROR: FC RegPoint GPRS Failed.");
        return VOS_ERR;
    }

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    regFcPoint.policyId = FC_POLICY_ID_CDMA;
    ret                 = FC_RegPoint(&regFcPoint);
    if (ret != VOS_OK) {
        AT_ERR_LOG("AT_RegModemFCPoint: ERROR: reg CDMA point Failed.");
        return VOS_ERR;
    }
#endif

    /* ����FCID��FC Pri��ӳ���ϵ */
    g_fcIdMaptoFcPri[FC_ID_MODEM].used  = VOS_TRUE;
    g_fcIdMaptoFcPri[FC_ID_MODEM].fcPri = fcPri;
    /* ��һ�������϶��RABID�������������Ҫ�����RABID��¼���� */
    g_fcIdMaptoFcPri[FC_ID_MODEM].rabIdMask |= ((VOS_UINT32)1 << (event->rabId));
    g_fcIdMaptoFcPri[FC_ID_MODEM].modemId = modemId;

    /* ��������Ϣ */
    AT_MntnTraceRegFcPoint(indexNum, AT_FC_POINT_TYPE_MODEM_PS);

    return VOS_OK;
}


VOS_UINT32 AT_DeRegModemPsDataFCPoint(VOS_UINT8 indexNum, VOS_UINT8 rabId)
{
    VOS_UINT32    ret;
    ModemIdUint16 modemId;
    AT_UartCtx   *uartCtx = VOS_NULL_PTR;

    uartCtx = AT_GetUartCtxAddr();

    /* UART�˿����عر�ʱ��ע�����ص� */
    if ((AT_CheckHsUartUser(indexNum) == VOS_TRUE) && (uartCtx->flowCtrl.dteByDce == AT_UART_FC_DTE_BY_DCE_NONE)) {
        return VOS_ERR;
    }

    modemId = MODEM_ID_0;

    ret = AT_GetModemIdFromClient(indexNum, &modemId);
    if (ret != VOS_OK) {
        AT_ERR_LOG("AT_DeRegModemPsDataFCPoint: Get modem id fail.");
        return VOS_ERR;
    }
#if (FEATURE_ACPU_FC_POINT_REG == FEATURE_ON)
    /* ɾ������ģ��ӳ���ϵ */
    FC_ChannelMapDelete(rabId, modemId);
#endif

    ret = FC_DeRegPoint(FC_ID_MODEM, modemId);
    if (ret != VOS_OK) {
        AT_ERR_LOG("AT_DeRegModemPsDataFCPoint: ERROR: FC DeRegPoint Failed.");
        return VOS_ERR;
    }

    /* ���FCID��FC Pri��ӳ���ϵ */
    g_fcIdMaptoFcPri[FC_ID_MODEM].used  = VOS_FALSE;
    g_fcIdMaptoFcPri[FC_ID_MODEM].fcPri = FC_PRI_BUTT;
    /* ��һ�������϶��RABID�������������Ҫ����Ӧ��RABID��������� */
    g_fcIdMaptoFcPri[FC_ID_MODEM].rabIdMask &= ~((VOS_UINT32)1 << rabId);
    g_fcIdMaptoFcPri[FC_ID_MODEM].modemId = MODEM_ID_BUTT;

    /* ��������Ϣ */
    AT_MntnTraceDeregFcPoint(indexNum, AT_FC_POINT_TYPE_MODEM_PS);

    return VOS_OK;
}


VOS_VOID AT_ModemPsRspPdpActEvtCnfProc(VOS_UINT8 indexNum, TAF_PS_CallPdpActivateCnf *event)
{
    AT_PPP_IndConfigInfo pppIndConfigInfo;
    DMS_PortIdUint16     portId;
    VOS_UINT16           pppId;

    /* ע��Modem�˿ڵ����ص� */
    AT_RegModemPsDataFCPoint(indexNum, event, FC_ID_MODEM);

    /* ��ʼ�� */
    (VOS_VOID)memset_s(&pppIndConfigInfo, sizeof(pppIndConfigInfo), 0x00, sizeof(pppIndConfigInfo));
    portId = g_atClientTab[indexNum].portNo;
    pppId  = AT_PS_GET_PPPID(indexNum);

    /* �������������� */
    AT_STOP_TIMER_CMD_READY(indexNum);

#if (FEATURE_DATA_SERVICE_NEW_PLATFORM == FEATURE_ON)
    g_atClientTab[indexNum].ifaceId = PS_IFACE_ID_PPP0;
#endif

    if (event->pdpAddr.pdpType == TAF_PDP_PPP) {
        /* ע��ppp�����պ��� */
        DMS_PORT_SwitchToPppDataMode(portId, DMS_PORT_DATA_PPP_RAW, PPP_PullRawDataEvent, pppId);

        /* ע���������ݷ��ͺ��� */
        PPP_RegDlDataCallback(g_atClientTab[indexNum].pppId);

        At_FormatResultData(indexNum, AT_CONNECT);
    } else if (event->pdpAddr.pdpType == TAF_PDP_IPV4) {
        /* ��дIP��ַ, DNS, NBNS */
        AT_FillPppIndConfigInfoPara(&pppIndConfigInfo, event);

        /* ��AUTH��IPCP֡����PPP����: */
        PPPA_RcvConfigInfoInd(g_atClientTab[indexNum].pppId, (PPPA_PdpActiveResult *)&pppIndConfigInfo);
    } else {
        /* �������Ͳ������� */
    }

    return;
}

VOS_UINT8 AT_IsPppUser(VOS_UINT8 indexNum)
{
    if ((g_atClientTab[indexNum].userType == AT_MODEM_USER) ||
        (g_atClientTab[indexNum].userType == AT_HSUART_USER)) {
        return VOS_TRUE;
    }

#if (FEATURE_IOT_HSUART_CMUX == FEATURE_ON)
    if (AT_CheckCmuxUser(indexNum) == VOS_TRUE) {
        return VOS_TRUE;
    }
#endif

    return VOS_FALSE;
}

VOS_VOID AT_ModemPsRspPdpDeactEvtCnfProc(VOS_UINT8 indexNum, TAF_PS_CallPdpDeactivateCnf *event)
{
    AT_ClientManage        *clientManage = VOS_NULL_PTR;
    VOS_UINT32              modemUsrFlg;
    VOS_UINT32              aTHCmdFlg;
    VOS_UINT32              result;
    VOS_UINT16              length;
    DMS_PortDataModeUint8   dataMode;

    modemUsrFlg = AT_CheckModemUser(indexNum);
    length      = 0;
    result      = AT_FAILURE;
    clientManage = AT_GetClientManage(indexNum);
    aTHCmdFlg   = (clientManage->cmdCurrentOpt == AT_CMD_H_PS_SET) ? VOS_TRUE : VOS_FALSE;
    dataMode    = DMS_PORT_GetDataMode(clientManage->portNo);

    if (AT_IsPppUser(indexNum) == VOS_FALSE) {
        return;
    }

    if (dataMode == DMS_PORT_DATA_PPP) {
        /* �ͷ�PPPʵ�� & HDLCȥʹ�� */
        AT_SendRelPppReq(clientManage->pppId, PPP_AT_CTRL_REL_PPP_REQ);
    } else if (dataMode == DMS_PORT_DATA_PPP_RAW) {
        /* �ͷ�PPPʵ�� & HDLCȥʹ�� */
        AT_SendRelPppReq(clientManage->pppId, PPP_AT_CTRL_REL_PPP_RAW_REQ);
    } else {
        /* ��������ģʽ�����ô��� */
        AT_WARN_LOG("TAF_PS_EVT_PDP_DEACTIVE_CNF OTHER MODE");
    }

    /* ȥע��Modem�˿ڵ����ص� */
    AT_DeRegModemPsDataFCPoint(indexNum, event->rabId);

    /* ��������ģʽ */
    DMS_PORT_ResumeCmdMode(clientManage->portNo);

    /*
     * Ϊ�˹��Linux��̨���β���ʧ�����⣬�������¹�ܷ���:
     * PDP�����Ͽ����̽����󣬽��յ�TAF_PS_EVT_PDP_DEACTIVE_CNF�¼����жϲ���
     * ϵͳ�Ƿ�ΪLinux����������ԭ�����̴������ǣ����ٷ���"NO CARRIER"
     */
    if ((DRV_GET_LINUXSYSTYPE() == VOS_OK) && (modemUsrFlg == VOS_TRUE)) {
        /*
         * ԭ�������У���At_FormatResultData�����ڲ���������"NO CARRIER"֮��
         * ��Ҫ��DCD���ͣ��ڴ˹�ܷ����У�������"NO CARRIER"����DCD�źŵ�����
         * �����Ծ���Ҫ����
         */
        AT_STOP_TIMER_CMD_READY(indexNum);
        DMS_PORT_DeassertDcd(clientManage->portNo);
        return;
    }

    if (aTHCmdFlg == VOS_TRUE) {
        result = AT_OK;
    } else {
        result = AT_NO_CARRIER;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);
    g_atSendDataBuff.bufLen = length;
    At_FormatResultData(indexNum, result);

    /* ATH�Ͽ�PPP����ʱ, ���DCD�ź�ģʽΪCONNECT ON, ��Ҫ����DCD�ź� */
    if ((result == AT_OK) && (DMS_PORT_IsDcdModeConnectOn() == VOS_TRUE)) {
        DMS_PORT_DeassertDcd(clientManage->portNo);
    }

    return;
}
#if (FEATURE_IOT_HSUART_CMUX == FEATURE_ON)
VOS_VOID AT_CmuxProcCallEndCnfEvent(VOS_UINT8 indexNum, VOS_UINT32 result)
{
    /*���ò��ųɹ��ı�־λ*/
    VOS_INT32 dlc = DMS_PORT_GetCmuxActivePppPstaskDlc();
    if (dlc > 0) {
        dlc--;
        DMS_PORT_SetCmuxActivePppPstaskDlc(dlc);
    }
    if (!((AT_CheckCmuxUser(indexNum) == VOS_TRUE) && (g_atClientTab[indexNum].ctrlId != indexNum))) {
        At_FormatResultData(indexNum, result);
    } else {
        At_FormatResultData(indexNum, AT_NO_CARRIER);
    }
}
#endif

VOS_VOID AT_ModemProcCallEndCnfEvent(TAF_PS_CauseUint32 cause, VOS_UINT8 indexNum)
{
    VOS_UINT32              modemUsrFlg;
    VOS_UINT32              aTHCmdFlg;
    VOS_UINT32              result;
    VOS_UINT16              length;
    DMS_PortIdUint16        portId;
    DMS_PortDataModeUint8   dataMode;

    modemUsrFlg = AT_CheckModemUser(indexNum);
    result      = AT_FAILURE;
    length      = 0;
    aTHCmdFlg   = (AT_PS_GET_CURR_CMD_OPT(indexNum) == AT_CMD_H_PS_SET) ? VOS_TRUE : VOS_FALSE;
    portId      = AT_GetDmsPortIdByClientId(indexNum);
    dataMode    = DMS_PORT_GetDataMode(portId);

    /* ��鵱ǰ�û��Ĳ������� */
    if ((AT_PS_GET_CURR_CMD_OPT(indexNum) != AT_CMD_PS_DATA_CALL_END_SET) &&
        (AT_PS_GET_CURR_CMD_OPT(indexNum) != AT_CMD_H_PS_SET)) {
        return;
    }

    /* PS��������ڴ���Ͽ�����, ֱ�ӷ��� */
    if (cause == TAF_ERR_NO_ERROR) {
        return;
    }

    /* �ͷ�PPPʵ�� & HDLCȥʹ�� */
    if (dataMode == DMS_PORT_DATA_PPP) {
        AT_SendRelPppReq(AT_PS_GET_PPPID(indexNum), PPP_AT_CTRL_REL_PPP_REQ);
    } else if (dataMode == DMS_PORT_DATA_PPP_RAW) {
        AT_SendRelPppReq(AT_PS_GET_PPPID(indexNum), PPP_AT_CTRL_REL_PPP_RAW_REQ);
    } else {
        ;
    }

    /* ��������ģʽ */
    DMS_PORT_ResumeCmdMode(portId);

    /*
     * Ϊ�˹��Linux��̨���β���ʧ�����⣬�������¹�ܷ���:
     * PDP�����Ͽ����̽����󣬽��յ�TAF_PS_EVT_PDP_DEACTIVE_CNF�¼����жϲ���
     * ϵͳ�Ƿ�ΪLinux����������ԭ�����̴������ǣ����ٷ���"NO CARRIER"
     */
    if ((DRV_GET_LINUXSYSTYPE() == VOS_OK) && (modemUsrFlg == VOS_TRUE)) {
        /*
         * ԭ�������У���At_FormatResultData�����ڲ���������"NO CARRIER"֮��
         * ��Ҫ��DCD���ͣ��ڴ˹�ܷ����У�������"NO CARRIER"����DCD�źŵ�����
         * �����Ծ���Ҫ����
         */
        AT_STOP_TIMER_CMD_READY(indexNum);
        DMS_PORT_DeassertDcd(portId);
        return;
    }

    if (aTHCmdFlg == VOS_TRUE) {
        result = AT_OK;
    } else {
        result = AT_NO_CARRIER;
    }

    AT_STOP_TIMER_CMD_READY(indexNum);
    g_atSendDataBuff.bufLen = length;

#if (FEATURE_IOT_HSUART_CMUX == FEATURE_ON)
    AT_CmuxProcCallEndCnfEvent(indexNum, result);
#else
    At_FormatResultData(indexNum, result);
#endif



    /* ATH�Ͽ�PPP����ʱ, ���DCD�ź�ģʽΪCONNECT ON, ��Ҫ����DCD�ź� */
    if ((result == AT_OK) && (DMS_PORT_IsDcdModeConnectOn() == VOS_TRUE)) {
        DMS_PORT_DeassertDcd(portId);
    }

    return;
}


VOS_VOID AT_AnswerPdpActInd(VOS_UINT8 indexNum, TAF_PS_CallPdpActivateCnf *event)
{
    VOS_ULONG               userData;
    VOS_UINT32              rslt;
    VOS_UINT16              pppId;
    DMS_PortIdUint16        portId;

    userData = (VOS_ULONG)g_atClientTab[indexNum].portNo;
    portId   = g_atClientTab[indexNum].portNo;
    pppId = 0;

    if (event->pdpAddr.pdpType == TAF_PDP_PPP) {
        if (PPPA_CreateRawDataPppReq(&pppId, userData, DMS_PORT_SendPppPacket) != VOS_OK) {
            rslt = AT_ERROR;
        } else {
            /* ��¼PPP id��Index�Ķ�Ӧ��ϵ */
            g_atPppIndexTab[pppId] = indexNum;

            /* ����PPP id */
            g_atClientTab[indexNum].pppId = pppId;

            /* ע��Modem�˿ڵ����ص� */
            AT_RegModemPsDataFCPoint(indexNum, event, FC_ID_MODEM);

            DMS_PORT_SwitchToPppDataMode(portId, DMS_PORT_DATA_PPP_RAW, PPP_PullRawDataEvent, pppId);

            /* ע���������ݷ��ͺ��� */
            PPP_RegDlDataCallback(pppId);

            rslt = AT_CONNECT;
        }

        AT_STOP_TIMER_CMD_READY(indexNum);
        At_FormatResultData(indexNum, rslt);
    } else {
        /* ��������.... */
        AT_STOP_TIMER_CMD_READY(indexNum);
        At_FormatResultData(indexNum, AT_CONNECT);
    }

    return;
}


MODULE_EXPORTED VOS_UINT32 At_RcvTeConfigInfoReq(VOS_UINT16 pppId, AT_PPP_ReqConfigInfo *pppReqConfigInfo)
{
    if (pppId >= AT_MAX_CLIENT_NUM) {
        AT_WARN_LOG("At_RcvTeConfigInfoReq usPppId Wrong");
        return AT_FAILURE;
    }

    if (pppReqConfigInfo == TAF_NULL_PTR) {
        AT_WARN_LOG("At_RcvTeConfigInfoReq pPppReqConfigInfo NULL");
        return AT_FAILURE;
    }

    if (g_atClientTab[g_atPppIndexTab[pppId]].cmdCurrentOpt != AT_CMD_D_PPP_CALL_SET) {
        AT_WARN_LOG("At_RcvTeConfigInfoReq NOT AT_CMD_D_PPP_CALL_SET");
        return AT_FAILURE;
    }

    if (TAF_PS_PPP_DIAL_UP(WUEPS_PID_AT, AT_PS_BuildExClientId(g_atClientTab[g_atPppIndexTab[pppId]].clientId), 0,
                           g_atClientTab[g_atPppIndexTab[pppId]].cid,
                           (TAF_PPP_ReqConfigInfo *)pppReqConfigInfo) == VOS_OK) {
        /* ����ʱ�� */
        if (At_StartTimer(AT_ACT_PDP_TIME, g_atPppIndexTab[pppId]) != AT_SUCCESS) {
            AT_ERR_LOG("At_RcvTeConfigInfoReq:ERROR:Start Timer");
            return AT_FAILURE;
        }

        /* ���õ�ǰ�������� */
        g_atClientTab[g_atPppIndexTab[pppId]].cmdCurrentOpt = AT_CMD_PPP_ORG_SET;

        return AT_SUCCESS;
    } else {
        return AT_FAILURE;
    }
}


MODULE_EXPORTED VOS_UINT32 At_RcvPppReleaseInd(VOS_UINT16 pppId)
{
    TAF_UINT8          eventInfo[AT_EVENT_INFO_SIZE];
    AT_PPP_ReleaseInd *msg = VOS_NULL_PTR;
    VOS_UINT32         length;

    if (pppId >= AT_MAX_CLIENT_NUM) {
        AT_WARN_LOG("At_RcvPppReleaseInd usPppId Wrong");
        return AT_FAILURE;
    }

    /* EVENT- At_RcvPppReleaseInd:usPppId / g_atPppIndexTab[usPppId] */
    eventInfo[AT_EVENT_INFO_ARRAY_INDEX_0] = (TAF_UINT8)(pppId >> 8);
    eventInfo[AT_EVENT_INFO_ARRAY_INDEX_1] = (TAF_UINT8)pppId;
    eventInfo[AT_EVENT_INFO_ARRAY_INDEX_2] = g_atPppIndexTab[pppId];
    AT_EventReport(WUEPS_PID_AT, NAS_OM_EVENT_DTE_RELEASE_PPP, eventInfo, (VOS_UINT32)sizeof(eventInfo));

    /* ��ATģ�鷢��AT_PPP_RELEASE_IND_MSG */
    length = sizeof(AT_PPP_ReleaseInd) - VOS_MSG_HEAD_LENGTH;
    /*lint -save -e516 */
    msg = (AT_PPP_ReleaseInd *)TAF_AllocMsgWithoutHeaderLen(PS_PID_APP_PPP, length); /*lint !e830*/
    /*lint -restore */
    if (msg == VOS_NULL_PTR) {
        /* ��ӡ������Ϣ---������Ϣ��ʧ�� */
        AT_ERR_LOG("At_RcvPppReleaseInd:ERROR:Allocates a message packet for AT_PPP_RELEASE_IND_MSG_STRU msg FAIL!");
        return AT_FAILURE;
    }

    /* ��д��Ϣͷ */
    TAF_CfgMsgHdr((MsgBlock *)msg, PS_PID_APP_PPP, WUEPS_PID_AT, length);

    msg->msgHeader.msgName = AT_PPP_RELEASE_IND_MSG;
    /* ��д��Ϣ�� */
    msg->clientId = g_atPppIndexTab[pppId];

    /* ���͸���Ϣ */
    if (TAF_TraceAndSendMsg(PS_PID_APP_PPP, msg) != VOS_OK) {
        /* ��ӡ������Ϣ---������Ϣʧ�� */
        AT_WARN_LOG("At_RcvPppReleaseInd:WARNING:SEND AT_PPP_RELEASE_IND_MSG msg FAIL!");
        return AT_FAILURE;
    } else {
        /* ��ӡ������Ϣ---��������Ϣ */
        AT_WARN_LOG("At_RcvPppReleaseInd:NORMAL:SEND AT_PPP_RELEASE_IND_MSG Msg");
        return AT_SUCCESS;
    }
}


VOS_VOID At_PppReleaseIndProc(AT_ClientIdUint16 clientId)
{
    if (clientId >= AT_MAX_CLIENT_NUM) {
        AT_WARN_LOG("At_PppReleaseIndProc:ERROR:ucIndex is abnormal!");
        return;
    }

    if (g_atClientTab[clientId].cmdCurrentOpt == AT_CMD_WAIT_PPP_PROTOCOL_REL_SET) {
        /* ��ATͨ���л�Ϊ����ģʽ */
        DMS_PORT_ResumeCmdMode(g_atClientTab[clientId].portNo);

        /* ֹͣ��ʱ�� */
        AT_STOP_TIMER_CMD_READY(clientId);

        /* �ظ�NO CARRIER */
        g_atSendDataBuff.bufLen = 0;
        At_FormatResultData((VOS_UINT8)clientId, AT_NO_CARRIER);

        return;
    }

    if (g_atClientTab[clientId].cmdCurrentOpt == AT_CMD_PS_DATA_CALL_END_SET) {
        return;
    }

    if (TAF_PS_PPP_DIAL_DOWN(WUEPS_PID_AT, AT_PS_BuildExClientId(clientId), 0,
                       g_atClientTab[clientId].cid) == VOS_OK) {
        /* ����ʱ�� */
        if (At_StartTimer(AT_DETACT_PDP_TIME, (VOS_UINT8)clientId) != AT_SUCCESS) {
            AT_ERR_LOG("At_PppReleaseIndProc:ERROR:Start Timer");
            return;
        }

        /* ���õ�ǰ�������� */
        g_atClientTab[clientId].cmdCurrentOpt = AT_CMD_PS_DATA_CALL_END_SET;
    }

    return;
}

/*
 * Description: ����֪Rab ID��ȡPPP ID
 *  1.Date: 2008-03-05
 *    Modification: Created function
 */
TAF_UINT32 At_PsRab2PppId(TAF_UINT8 exRabId, TAF_UINT16 *pppId)
{
    TAF_UINT16 pppIdLoop;
    TAF_UINT8  indexNum;

    if (pppId == VOS_NULL_PTR) {
        AT_WARN_LOG("At_PsRab2PppId, pusPppId NULL");
        return TAF_FAILURE;
    }

    /* ͨ��PPP ID������ң�Ч�ʻ�Ƚϸ� */
    for (pppIdLoop = 1; pppIdLoop <= PPP_MAX_ID_NUM; pppIdLoop++) {
        indexNum = g_atPppIndexTab[pppIdLoop];

        if (indexNum >= AT_MAX_CLIENT_NUM) {
            continue;
        }

        if (g_atClientTab[indexNum].used != AT_CLIENT_USED) {
            continue;
        }

        if (DMS_PORT_IsPppPacketTransMode(g_atClientTab[indexNum].portNo) == VOS_FALSE) {
            continue;
        }

        if (g_atClientTab[indexNum].exPsRabId == exRabId) {
            *pppId = pppIdLoop; /* ���ؽ�� */
            return TAF_SUCCESS;
        }
    }

    AT_LOG1("AT, At_PsRab2PppId, WARNING, Get PppId from Rab <1> Fail", exRabId);

    return TAF_FAILURE;
} /* At_PsRab2PppId */

/*
 * Description: ����֪PPP ID��ȡRab ID
 *  1.Date: 2008-03-05
 *    Modification: Created function
 */
TAF_UINT32 At_PppId2PsRab(TAF_UINT16 pppId, TAF_UINT8 *exRabId)
{
    TAF_UINT8 indexNum;

    if ((pppId < 1) || (pppId > PPP_MAX_ID_NUM)) {
        TAF_LOG1(WUEPS_PID_AT, 0, PS_LOG_LEVEL_WARNING, "AT, At_PppId2PsRab, WARNING, PppId <1> Wrong", pppId);
        return TAF_FAILURE;
    }

    if (exRabId == TAF_NULL_PTR) {
        AT_WARN_LOG("AT, At_PppId2PsRab, WARNING, pucRabId NULL");
        return TAF_FAILURE;
    }

    indexNum = g_atPppIndexTab[pppId];

    if (indexNum >= AT_MAX_CLIENT_NUM) {
        AT_LOG1("AT, At_PppId2PsRab, WARNING, indexNum <1> Wrong", indexNum);
        return TAF_FAILURE;
    }

    if (DMS_PORT_IsPppPacketTransMode(g_atClientTab[indexNum].portNo) == VOS_FALSE) {
        AT_LOG1("AT, At_PppId2PsRab, WARNING, port not in ppp packet trans mode", indexNum);
        return TAF_FAILURE;
    }

    *exRabId = g_atClientTab[indexNum].exPsRabId;

    return TAF_SUCCESS;
} /* At_PppId2PsRab */

#if (FEATURE_DATA_SERVICE_NEW_PLATFORM == FEATURE_ON)

VOS_UINT32 At_IfaceId2PppId(VOS_UINT8 ifaceId, VOS_UINT16 *pppId)
{
    VOS_UINT16 pppIdLoop;
    VOS_UINT8  indexNum;

    if (pppId == VOS_NULL_PTR) {
        AT_WARN_LOG("At_IfaceId2PppId, pusPppId NULL");
        return VOS_ERR;
    }

    /* ͨ��PPP ID������ң�Ч�ʻ�Ƚϸ� */
    for (pppIdLoop = 1; pppIdLoop <= PPP_MAX_ID_NUM; pppIdLoop++) {
        indexNum = g_atPppIndexTab[pppIdLoop];

        if (indexNum >= AT_MAX_CLIENT_NUM) {
            continue;
        }

        if (g_atClientTab[indexNum].used != AT_CLIENT_USED) {
            continue;
        }

        if (DMS_PORT_IsPppPacketTransMode(g_atClientTab[indexNum].portNo) == VOS_FALSE) {
            continue;
        }

        if (g_atClientTab[indexNum].ifaceId == ifaceId) {
            *pppId = pppIdLoop; /* ���ؽ�� */
            return VOS_OK;
        }
    }

    AT_LOG1("AT, At_IfaceId2PppId, WARNING, Get PppId from Rab <1> Fail", ifaceId);
    return VOS_ERR;
}


MODULE_EXPORTED VOS_UINT32 At_PppId2IfaceId(VOS_UINT16 pppId, VOS_UINT8 *ifaceId)
{
    VOS_UINT8 indexNum;

    if ((pppId < 1) || (pppId > PPP_MAX_ID_NUM)) {
        TAF_LOG1(WUEPS_PID_AT, 0, PS_LOG_LEVEL_WARNING, "AT, At_PppId2IfaceId, WARNING, PppId <1> Wrong", pppId);
        return VOS_ERR;
    }

    if (ifaceId == VOS_NULL_PTR) {
        AT_WARN_LOG("AT, At_PppId2IfaceId, WARNING, pucIfaceId NULL");
        return VOS_ERR;
    }

    indexNum = g_atPppIndexTab[pppId];
    if (indexNum >= AT_MAX_CLIENT_NUM) {
        AT_ERR_LOG1("AT, At_PppId2IfaceId, WARNING, indexNum <1> Wrong", indexNum);
        return VOS_ERR;
    }

    if (DMS_PORT_IsPppPacketTransMode(g_atClientTab[indexNum].portNo) == VOS_FALSE) {
        AT_ERR_LOG("AT, At_PppId2IfaceId, WARNING, not in Ppp Packet Trans Mode");
        return VOS_ERR;
    }

    *ifaceId = g_atClientTab[indexNum].ifaceId;

    return VOS_OK;
}
#endif


VOS_UINT32 AT_GetLanAddr32(VOS_UINT8 *addr)
{
    VOS_UINT32 addrPara;

    addrPara = *addr++;
    addrPara <<= 8;
    addrPara |= *addr++;
    addrPara <<= 8;
    addrPara |= *addr++;
    addrPara <<= 8;
    addrPara |= *addr;

    return addrPara;
}


VOS_UINT32 AT_GetFcPriFromMap(const FC_IdUint8 fcId, AT_FcidMap *fcIdMap)
{
    if (fcId >= FC_ID_BUTT) {
        return VOS_ERR;
    }

    if ((fcId == FC_ID_MODEM) || ((fcId >= FC_ID_NIC_1) && (fcId <= FC_ID_NIC_12)) ||
        ((fcId >= FC_ID_DIPC_1) && (fcId <= FC_ID_DIPC_3))) {
        fcIdMap->used  = g_fcIdMaptoFcPri[fcId].used;
        fcIdMap->fcPri = g_fcIdMaptoFcPri[fcId].fcPri;

        return VOS_OK;
    }

    return VOS_ERR;
}


VOS_VOID AT_ChangeFCPoint(TAF_Ctrl *ctrl, FC_PriTypeUint8 fCPri, FC_IdUint8 fcId)
{
    VOS_UINT32    ret;
    ModemIdUint16 modemId;

    modemId = MODEM_ID_0;

    ret = AT_GetModemIdFromClient(ctrl->clientId, &modemId);

    if (ret != VOS_OK) {
        AT_ERR_LOG("AT_ChangeFCPoint: Get modem id fail.");
        return;
    }

    if (fcId != FC_ID_BUTT) {
        ret = FC_ChangePoint(fcId, FC_POLICY_ID_MEM, fCPri, modemId);
        if (ret != VOS_OK) {
            AT_ERR_LOG("AT_ChangeFCPoint: ERROR: Change FC Point Failed.");
        }

        ret = FC_ChangePoint(fcId, FC_POLICY_ID_CPU_A, fCPri, modemId);
        if (ret != VOS_OK) {
            AT_ERR_LOG("AT_ChangeFCPoint: ERROR: Change FC Point Failed.");
        }

        ret = FC_ChangePoint(fcId, FC_POLICY_ID_CDS, fCPri, modemId);
        if (ret != VOS_OK) {
            AT_ERR_LOG("AT_ChangeFCPoint: ERROR: Change FC Point Failed.");
        }

        ret = FC_ChangePoint(fcId, FC_POLICY_ID_GPRS, fCPri, modemId);
        if (ret != VOS_OK) {
            AT_ERR_LOG("AT_ChangeFCPoint: ERROR: Change FC Point Failed.");
        }
    }

    return;
}


VOS_VOID AT_NotifyFcWhenPdpModify(TAF_PS_CallPdpModifyCnf *event, FC_IdUint8 fcId)
{
    FC_PriTypeUint8 fCPriCurrent;
    AT_FcidMap      fCPriOrg;

    if (AT_GetFcPriFromMap(fcId, &fCPriOrg) == VOS_OK) {
        if (event->opUmtsQos == TAF_USED) {
            fCPriCurrent = AT_GetFCPriFromQos(&event->umtsQos);
        } else {
            fCPriCurrent = FC_PRI_FOR_PDN_NONGBR;
        }

        if ((fCPriOrg.used == VOS_TRUE) && (fCPriCurrent > fCPriOrg.fcPri)) {
            /* ���ݷ���QOS���ı����ص�����ȼ� */
            AT_ChangeFCPoint(&event->ctrl, fCPriCurrent, fcId);
        }
    }

    return;
}


VOS_VOID AT_PS_SetPsCallErrCause(VOS_UINT16 clientId, TAF_PS_CauseUint32 psErrCause)
{
    AT_ModemPsCtx *psModemCtx = VOS_NULL_PTR;

    psModemCtx = AT_GetModemPsCtxAddrFromClientId(clientId);

    psModemCtx->psErrCause = psErrCause;

    return;
}


TAF_PS_CauseUint32 AT_PS_GetPsCallErrCause(VOS_UINT16 clientId)
{
    AT_ModemPsCtx *psModemCtx = VOS_NULL_PTR;

    psModemCtx = AT_GetModemPsCtxAddrFromClientId(clientId);

    return psModemCtx->psErrCause;
}


VOS_VOID AT_PS_AddIpAddrMap(VOS_UINT16 clientId, TAF_PS_CallPdpActivateCnf *event)
{
    AT_ModemPsCtx *psModemCtx = VOS_NULL_PTR;
    VOS_UINT32     ipAddr;

    psModemCtx = AT_GetModemPsCtxAddrFromClientId(clientId);

#if (FEATURE_DATA_SERVICE_NEW_PLATFORM == FEATURE_ON)
    if (event->ifaceId >= PS_IFACE_ID_BUTT) {
        return;
    }
#else
    if (!AT_PS_IS_RABID_VALID(event->rabId)) {
        return;
    }
#endif

    if (event->opPdpAddr) {
        ipAddr = AT_GetLanAddr32(event->pdpAddr.ipv4Addr);
#if (FEATURE_DATA_SERVICE_NEW_PLATFORM == FEATURE_ON)
        psModemCtx->ipAddrIfaceIdMap[event->ifaceId] = ipAddr;
#else
        psModemCtx->ipAddrRabIdMap[event->rabId - AT_PS_RABID_OFFSET] = ipAddr;
#endif
    }

    return;
}


VOS_VOID AT_PS_DeleteIpAddrMap(VOS_UINT16 clientId, TAF_PS_CallPdpDeactivateCnf *event)
{
    AT_ModemPsCtx *psModemCtx = VOS_NULL_PTR;

    psModemCtx = AT_GetModemPsCtxAddrFromClientId(clientId);

#if (FEATURE_DATA_SERVICE_NEW_PLATFORM == FEATURE_ON)
    if (event->ifaceId >= PS_IFACE_ID_BUTT) {
        return;
    }

    if ((event->primFlag == VOS_TRUE) && ((event->pdpType & TAF_PDP_IPV4) == TAF_PDP_IPV4)) {
        psModemCtx->ipAddrIfaceIdMap[event->ifaceId] = 0;
    }
#else
    if (!AT_PS_IS_RABID_VALID(event->rabId)) {
        return;
    }

    psModemCtx->ipAddrRabIdMap[event->rabId - AT_PS_RABID_OFFSET] = 0;
#endif

    return;
}

#if (FEATURE_DATA_SERVICE_NEW_PLATFORM == FEATURE_ON)

VOS_UINT32 AT_PS_GetIpAddrByIfaceId(const VOS_UINT16 clientId, const VOS_UINT8 ifaceId)
{
    AT_ModemPsCtx *psModemCtx = VOS_NULL_PTR;

    psModemCtx = AT_GetModemPsCtxAddrFromClientId(clientId);

    return psModemCtx->ipAddrIfaceIdMap[ifaceId];
}
#else

VOS_UINT32 AT_PS_GetIpAddrByRabId(VOS_UINT16 clientId, VOS_UINT8 rabId)
{
    AT_ModemPsCtx *psModemCtx = VOS_NULL_PTR;
    VOS_UINT32 ipAddr;

    psModemCtx = AT_GetModemPsCtxAddrFromClientId(clientId);
    ipAddr = 0;

    if (AT_PS_IS_RABID_VALID(rabId)) {
        ipAddr = psModemCtx->ipAddrRabIdMap[rabId - AT_PS_RABID_OFFSET];
    }

    return ipAddr;
}
#endif


VOS_UINT32 AT_PS_GetChDataValueFromRnicRmNetId(RNIC_DEV_ID_ENUM_UINT8   rnicRmNetId,
                                               AT_CH_DataChannelUint32 *dataChannelId)
{
    VOS_UINT32 i;
    CONST AT_ChdataRnicRmnetId *chdataRnicRmNetIdTab;

    chdataRnicRmNetIdTab = AT_PS_GET_CHDATA_RNIC_RMNET_ID_TBL_PTR();

    for (i = 0; i < AT_PS_GET_CHDATA_RNIC_RMNET_ID_TBL_SIZE(); i++) {
        if (rnicRmNetId == chdataRnicRmNetIdTab[i].rnicRmNetId) {
            *dataChannelId = chdataRnicRmNetIdTab[i].chdataValue;
            break;
        }
    }

    if (i >= AT_PS_GET_CHDATA_RNIC_RMNET_ID_TBL_SIZE()) {
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 AT_PS_GetChDataValueFromIfaceId(PS_IFACE_IdUint8 ifaceId, AT_CH_DataChannelUint32 *dataChannelId)
{
    VOS_UINT32 i;
    CONST AT_ChdataRnicRmnetId *chdataRnicRmNetIdTab;

    chdataRnicRmNetIdTab = AT_PS_GET_CHDATA_RNIC_RMNET_ID_TBL_PTR();

    for (i = 0; i < AT_PS_GET_CHDATA_RNIC_RMNET_ID_TBL_SIZE(); i++) {
        if (ifaceId == chdataRnicRmNetIdTab[i].ifaceId) {
            *dataChannelId = chdataRnicRmNetIdTab[i].chdataValue;
            break;
        }
    }

    if (i >= AT_PS_GET_CHDATA_RNIC_RMNET_ID_TBL_SIZE()) {
        return VOS_ERR;
    }

    return VOS_OK;
}


CONST AT_ChdataRnicRmnetId* AT_PS_GetChDataCfgByChannelId(AT_CH_DataChannelUint32 dataChannelId)
{
    CONST AT_ChdataRnicRmnetId *chdataRnicRmNetIdTab = VOS_NULL_PTR;
    CONST AT_ChdataRnicRmnetId *chDataConfig         = VOS_NULL_PTR;
    VOS_UINT32                  i;

    chdataRnicRmNetIdTab = AT_PS_GET_CHDATA_RNIC_RMNET_ID_TBL_PTR();

    /*  �����ж����ܱ�֤enDataChannelId����Ч�ԣ�����RM NET IDһ�����ڱ����ҵ� */
    for (i = 0; i < AT_PS_GET_CHDATA_RNIC_RMNET_ID_TBL_SIZE(); i++) {
        if (dataChannelId == chdataRnicRmNetIdTab[i].chdataValue) {
            chDataConfig = &chdataRnicRmNetIdTab[i];
            break;
        }
    }

    return chDataConfig;
}


RNIC_DEV_ID_ENUM_UINT8 AT_PS_GetRmnetIdFromIfaceId(const PS_IFACE_IdUint8 ifaceId)
{
    VOS_UINT32 i;
    CONST AT_PS_RmnetIfaceId *rmnetIfaceIdTab;

    rmnetIfaceIdTab = AT_PS_RMNET_IFACE_ID_TBL_PTR();

    for (i = 0; i < AT_PS_RMNET_IFACE_ID_TBL_SIZE(); i++) {
        if (ifaceId == rmnetIfaceIdTab[i].ifaceId) {
            return rmnetIfaceIdTab[i].rmNetId;
        }
    }

    AT_WARN_LOG("AT_PS_GetRmnetIdFromIfaceId: not find rmnet id");
    return RNIC_DEV_ID_BUTT;
}


PS_IFACE_IdUint8 AT_PS_GetIfaceIdFromRmnetId(const RNIC_DEV_ID_ENUM_UINT8 rmnetId)
{
    VOS_UINT32 i;
    CONST AT_PS_RmnetIfaceId *rmnetIfaceIdTab;
    PS_IFACE_IdUint8          ifaceId;

    rmnetIfaceIdTab = AT_PS_RMNET_IFACE_ID_TBL_PTR();
    ifaceId         = PS_IFACE_ID_BUTT;

    for (i = 0; i < AT_PS_RMNET_IFACE_ID_TBL_SIZE(); i++) {
        if (rmnetId == rmnetIfaceIdTab[i].rmNetId) {
            ifaceId = rmnetIfaceIdTab[i].ifaceId;
        }
    }

    return ifaceId;
}


FC_IdUint8 AT_PS_GetFcIdByIfaceId(const VOS_UINT32 ifaceId)
{
    VOS_UINT32       i;
    const AT_PS_FcIfaceId *fcIfaceIdTab = VOS_NULL_PTR;

    fcIfaceIdTab = AT_PS_GET_FC_IFACE_ID_TBL_PTR();

    for (i = 0; i < AT_PS_GET_FC_IFACE_ID_TBL_SIZE(); i++) {
        if (fcIfaceIdTab[i].ifaceId == ifaceId) {
            return fcIfaceIdTab[i].fcId;
        }
    }

    AT_WARN_LOG("AT_PS_GetFcIdFromRnidRmNetId: WARNING: data channel id is abnormal.");
    return FC_ID_BUTT;
}
#if ((FEATURE_SHARE_APN == FEATURE_ON) && (FEATURE_DATA_SERVICE_NEW_PLATFORM == FEATURE_ON))

VOS_UINT32 AT_PS_CheckIfaceMatchWithClient(VOS_UINT16 clientId, VOS_UINT8 ifaceId, VOS_UINT32 usedFlg)
{
    TAF_IFACE_UserTypeUint8 userType;

    userType = AT_PS_GetUserType((VOS_UINT8)clientId);

    /* ӳ���ϵδ���ó���Ĭ��ƥ�� */
    if (usedFlg == VOS_FALSE) {
        return VOS_TRUE;
    }

    if (userType == TAF_IFACE_USER_TYPE_NDIS) {
        if (AT_IsNdisIface(ifaceId) == VOS_TRUE) {
            return VOS_TRUE;
        }
    } else if (userType == TAF_IFACE_USER_TYPE_APP) {
        if ((ifaceId >= PS_IFACE_ID_RMNET0) && (ifaceId <= PS_IFACE_ID_RMNET11)) {
            return VOS_TRUE;
        }
    } else {
        /* �����˿� Ĭ��ƥ�� */
        return VOS_TRUE;
    }
    return VOS_FALSE;
}
#endif

AT_PS_DataChanlCfg* AT_PS_GetDataChanlCfg(VOS_UINT16 clientId, VOS_UINT8 cid)
{
    AT_CommPsCtx *psCtx = VOS_NULL_PTR;
    VOS_UINT32    rslt;
    VOS_UINT32    i;
    ModemIdUint16 modemId;

    modemId = MODEM_ID_0;
    psCtx   = AT_GetCommPsCtxAddr();
    rslt    = AT_GetModemIdFromClient(clientId, &modemId);

    if (rslt != VOS_OK) {
        AT_ERR_LOG("AT_PS_GetDataChanlCfg: Get modem id fail.");
        return &(psCtx->channelCfg[AT_CH_DATA_CHANNEL_BUTT]);
    }

    for (i = 0; i < AT_CH_DATA_CHANNEL_BUTT; i++) {
        if ((psCtx->channelCfg[i].cid == cid) && (psCtx->channelCfg[i].modemId == modemId)) {
            /* ����APN�����´��ڵ���cid ��Ӧ���ͨ�� ��Ҫ����ͨ�������ж� */
#if ((FEATURE_SHARE_APN == FEATURE_ON) && (FEATURE_DATA_SERVICE_NEW_PLATFORM == FEATURE_ON))
            if (AT_PS_CheckIfaceMatchWithClient(clientId, (VOS_UINT8)(psCtx->channelCfg[i].ifaceId),
                    psCtx->channelCfg[i].used) == VOS_FALSE) {
                continue;
            }
#endif
            return &(psCtx->channelCfg[i]);
        }
    }

    return &(psCtx->channelCfg[AT_CH_DATA_CHANNEL_BUTT]);
}


TAF_PS_CallEndCauseUint8 AT_PS_TransCallEndCause(VOS_UINT8 connectType)
{
    TAF_PS_CallEndCauseUint8 cause;

    cause = TAF_PS_CALL_END_CAUSE_NORMAL;

    if (connectType == TAF_PS_CALL_TYPE_DOWN_FORCE) {
        cause = TAF_PS_CALL_END_CAUSE_FORCE;
    }

    return cause;
}



VOS_UINT32 AT_PS_AppSetFlowCtrl(VOS_UINT32 param1, VOS_UINT32 param2)
{
    VOS_UINT32 rslt;

    rslt = RNIC_StartFlowCtrl((VOS_UINT8)param1);

    if (rslt != VOS_OK) {
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 AT_PS_AppClearFlowCtrl(VOS_UINT32 param1, VOS_UINT32 param2)
{
    VOS_UINT32 rslt;

    rslt = RNIC_StopFlowCtrl((VOS_UINT8)param1);

    if (rslt != VOS_OK) {
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 AT_PS_EnableNdisFlowCtl(VOS_UINT32 param1, VOS_UINT32 param2)
{
    /* ͨ��udi_ioctl����ʹ������ */
    VOS_UINT32 enbflg = DMS_NCM_FLOW_CTRL_ENABLE;

    return DMS_PORT_ChangeFlowCtrl(DMS_PORT_NCM_DATA, enbflg);
}


VOS_UINT32 AT_PS_DisableNdisFlowCtl(VOS_UINT32 param1, VOS_UINT32 param2)
{
    /* ͨ��udi_ioctl����ȥʹ������ */
    VOS_UINT32 enbflg = DMS_NCM_FLOW_CTRL_DISABLE;

    return DMS_PORT_ChangeFlowCtrl(DMS_PORT_NCM_DATA, enbflg);
}


VOS_VOID AT_PS_ActiveUsbNet(VOS_UINT8  ifaceId)
{
    VOS_UINT32       linkstus;
    VOS_UINT32       rtn;
    VOS_UINT32       downBitrate;
    VOS_UINT32       upBitrate;
    AT_DisplayRate   speed;
    DMS_PortIdUint16 portId;

    (VOS_VOID)memset_s(&speed, sizeof(speed), 0x00, (VOS_SIZE_T)(sizeof(speed)));

    if (AT_GetDisplayRate(AT_CLIENT_ID_NDIS, &speed) != VOS_OK) {
        AT_ERR_LOG("AT_PS_ActiveUsbNet : ERROR : AT_GetDisplayRate Error!");
    }
    /* ������ʳ���U32�ķ�Χ��ȡ���ֵ0xffffffff */
    downBitrate = (AT_AtoI((VOS_CHAR *)speed.dlSpeed) >= 0xffffffff) ?
                                    0xffffffff :
                                    (VOS_UINT32)AT_AtoI((VOS_CHAR *)speed.dlSpeed);
    upBitrate = (AT_AtoI((VOS_CHAR *)speed.ulSpeed) >= 0xffffffff) ?
                                  0xffffffff :
                                  (VOS_UINT32)AT_AtoI((VOS_CHAR *)speed.ulSpeed);

    if (AT_IsNdisIface(ifaceId) == VOS_FALSE) {
        AT_ERR_LOG1("AT_PS_ActiveUsbNet, not NDIS iface", ifaceId);
        return;
    }

    portId = DMS_PORT_NCM_DATA + (ifaceId - PS_IFACE_ID_NDIS0);

    rtn = DMS_PORT_ChangeConnSpeed(portId, downBitrate, upBitrate);
    if (rtn != VOS_OK) {
        AT_ERR_LOG("AT_PS_ActiveUsbNet, Ctrl Speed Fail!");
        return;
    }

    linkstus = DMS_NCM_CONNECTION_LINKUP;
    rtn       = DMS_PORT_ChangeConnLinkState(portId, linkstus);
    if (rtn != VOS_OK) {
        AT_ERR_LOG("AT_PS_ActiveUsbNet, Active usb net Fail!");
        return;
    }

    return;
}


VOS_VOID AT_PS_DeActiveUsbNet(VOS_UINT8  ifaceId)
{
    DMS_PortIdUint16 portId;
    VOS_UINT32 linkstus;
    VOS_UINT32 rtn;

    /* ȥ����Ѻ�BSPȷ�ϣ����������ȥ�����ȥ���û��Ӱ�� */
    linkstus = DMS_NCM_CONNECTION_LINKDOWN;

    if (AT_IsNdisIface(ifaceId) == VOS_FALSE) {
        AT_ERR_LOG1("AT_PS_DeActiveUsbNet, not NDIS iface", ifaceId);
        return;
    }

    portId = DMS_PORT_NCM_DATA + (ifaceId - PS_IFACE_ID_NDIS0);

    rtn = DMS_PORT_ChangeConnLinkState(portId, linkstus);
    if (rtn != VOS_OK) {
        AT_ERR_LOG("AT_PS_DeActiveUsbNet, Deactive usb net Fail!");
        return;
    }

    return;
}


VOS_UINT32 AT_PS_IsIpv6CapabilityValid(VOS_UINT8 capability)
{
    if ((capability == AT_IPV6_CAPABILITY_IPV4_ONLY) || (capability == AT_IPV6_CAPABILITY_IPV6_ONLY) ||
        (capability == AT_IPV6_CAPABILITY_IPV4V6_OVER_ONE_PDP)) {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_UINT32 AT_PS_ProcCallModifyEvent(VOS_UINT8 indexNum, TAF_PS_CallPdpModifyCnf *event)
{
    AT_PS_DataChanlCfg *chanCfg = VOS_NULL_PTR;
    FC_IdUint8          defaultFcId;

    chanCfg = AT_PS_GetDataChanlCfg(indexNum, event->cid);

    if ((chanCfg->used != VOS_TRUE) || (chanCfg->ifaceActFlg != VOS_TRUE)) {
        AT_ERR_LOG("AT_PS_ProcCallModifyEvent, ps call not activate!");
        return VOS_ERR;
    }

    /* ��ȡ����ID��Ӧ��FC ID */
    defaultFcId = AT_PS_GetFcIdByIfaceId(chanCfg->ifaceId);

    if (defaultFcId >= FC_ID_BUTT) {
        return VOS_ERR;
    }

    AT_NotifyFcWhenPdpModify(event, defaultFcId);

    return VOS_OK;
}


TAF_IFACE_UserTypeUint8 AT_PS_GetUserType(const VOS_UINT8 indexNum)
{
    TAF_IFACE_UserTypeUint8 userType;
    VOS_UINT8              *systemAppConfig = VOS_NULL_PTR;

    userType        = TAF_IFACE_USER_TYPE_BUTT;
    systemAppConfig = AT_GetSystemAppConfigAddr();

    switch (g_atClientTab[indexNum].userType) {
        case AT_NDIS_USER:
            userType = TAF_IFACE_USER_TYPE_NDIS;
            break;

        case AT_APP_USER:
            userType = TAF_IFACE_USER_TYPE_APP;
            break;

        case AT_USBCOM_USER:
        case AT_UART_USER:
#if (FEATURE_IOT_HSUART_CMUX == FEATURE_ON)
        case AT_CMUXAT_USER:
        case AT_CMUXMDM_USER:
        case AT_CMUXEXT_USER:
        case AT_CMUXGPS_USER:
#endif
            if ((AT_GetPcuiPsCallFlag() == VOS_TRUE) || (*systemAppConfig == SYSTEM_APP_WEBUI)) {
                userType = TAF_IFACE_USER_TYPE_APP;
                break;
            }

            userType = TAF_IFACE_USER_TYPE_NDIS;
            break;

        case AT_CTR_USER:
            if (AT_GetCtrlPsCallFlag() == VOS_TRUE) {
                userType = TAF_IFACE_USER_TYPE_APP;
                break;
            }

            if (*systemAppConfig != SYSTEM_APP_WEBUI) {
                userType = TAF_IFACE_USER_TYPE_NDIS;
                break;
            }
            break;

        case AT_PCUI2_USER:
            if (AT_GetPcui2PsCallFlag() == VOS_TRUE) {
                userType = TAF_IFACE_USER_TYPE_APP;
                break;
            }

            if (*systemAppConfig != SYSTEM_APP_WEBUI) {
                userType = TAF_IFACE_USER_TYPE_NDIS;
                break;
            }
            break;

        default:
            AT_WARN_LOG("AT_PS_GetUserType: UserType Is Invalid!");
            userType = TAF_IFACE_USER_TYPE_BUTT;
            break;
    }

    return userType;
}


VOS_UINT32 AT_PS_CheckDialParamCnt(TAF_IFACE_UserTypeUint8 userType)
{
    VOS_UINT8 *systemAppConfig = VOS_NULL_PTR;

    systemAppConfig = AT_GetSystemAppConfigAddr();

    /* ���������� */
    if (userType == TAF_IFACE_USER_TYPE_APP) {
        if (*systemAppConfig != SYSTEM_APP_WEBUI) {
            if (g_atParaIndex > AT_PHONE_DIAL_MAX_PARA_NUM) {
                AT_NORM_LOG1("AT_PS_CheckDialParamCnt: Phone Dial Parameter number is .\n", g_atParaIndex);
                return AT_TOO_MANY_PARA;
            }
        } else {
            if (g_atParaIndex > AT_WEBUI_DIAL_MAX_PARA_NUM) {
                AT_NORM_LOG1("AT_PS_CheckDialParamCnt: WEBUI APP Dial Parameter number is .\n", g_atParaIndex);
                return AT_TOO_MANY_PARA;
            }
        }

        return AT_SUCCESS;
    }

    if (userType == TAF_IFACE_USER_TYPE_NDIS) {
        /* ����MBB��Ʒ��Ҫ����չ��7�� */
        if (g_atParaIndex > AT_NDIS_DIAL_MAX_PARA_NUM) {
            AT_NORM_LOG1("AT_PS_CheckDialParamCnt: NDIS Dial Parameter number is .\n", g_atParaIndex);
            return AT_TOO_MANY_PARA;
        } else {
            return AT_SUCCESS;
        }
    }

    AT_WARN_LOG("AT_PS_ValidateDialParam: User Type is invalid.\n");

    return AT_ERROR;
}


VOS_UINT32 AT_PS_CheckDialParamApn(VOS_UINT16 clientId)
{
    /* ��� APN */
    if (g_atParaList[AT_PS_DIAL_PARAM_APN].paraLen != 0) {
        /* APN���ȼ�� */
        if (g_atParaList[AT_PS_DIAL_PARAM_APN].paraLen > TAF_MAX_APN_LEN) {
            AT_NORM_LOG("AT_PS_CheckDialParamApn: APN is too long.");
            return AT_CME_INCORRECT_PARAMETERS;
        }

        /* APN��ʽ��� */
        if (AT_CheckApnFormat(g_atParaList[AT_PS_DIAL_PARAM_APN].para,
                              g_atParaList[AT_PS_DIAL_PARAM_APN].paraLen, clientId) != VOS_OK) {
            AT_NORM_LOG("AT_PS_CheckDialParamApn: Format of APN is wrong.");
            return AT_CME_INCORRECT_PARAMETERS;
        }
    }

    return AT_SUCCESS;
}


VOS_UINT32 AT_PS_CheckDialParamIpAddr(VOS_VOID)
{
    VOS_UINT8 ipv4Addr[TAF_IPV4_ADDR_LEN];

    (VOS_VOID)memset_s(ipv4Addr, sizeof(ipv4Addr), 0x00, sizeof(ipv4Addr));

    /* ip addr��� */
    if (g_atParaList[AT_PS_DIAL_PARAM_IP_ADDR].paraLen > (TAF_MAX_IPV4_ADDR_STR_LEN - 1)) {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (g_atParaList[AT_PS_DIAL_PARAM_IP_ADDR].paraLen > 0) {
        if (AT_Ipv4AddrAtoi((VOS_CHAR *)g_atParaList[AT_PS_DIAL_PARAM_IP_ADDR].para, ipv4Addr,
                            sizeof(ipv4Addr)) != VOS_OK) {
            return AT_CME_INCORRECT_PARAMETERS;
        }
    }

    return AT_SUCCESS;
}


VOS_UINT32 AT_PS_GenIfaceId(const VOS_UINT8 indexNum, const PS_IFACE_IdUint8 beginIfaceId,
                            const PS_IFACE_IdUint8 endIfaceId, VOS_UINT8 cid)
{
    AT_CommPsCtx           *psCtx = VOS_NULL_PTR;
    AT_CH_DataChannelUint32 dataChannelId;
    VOS_UINT32              i;
    VOS_UINT32              rslt;
    ModemIdUint16           modemId;

    modemId = MODEM_ID_0;
    if (AT_GetModemIdFromClient(indexNum, &modemId) != VOS_OK) {
        AT_WARN_LOG("AT_PS_GenIfaceId: get modem id error!");
        return AT_ERROR;
    }

    /* E5��̬APP���Ͳ��ſ��Բ���Ҫ^CHDATA���� */
    for (i = beginIfaceId; i <= endIfaceId; i++) {
        dataChannelId = AT_CH_DATA_CHANNEL_BUTT;

        rslt = AT_PS_GetChDataValueFromIfaceId((VOS_UINT8)i, &dataChannelId);

        if (rslt == VOS_ERR) {
            continue;
        }

        if (dataChannelId >= AT_CH_DATA_CHANNEL_BUTT) {
            continue;
        }

        psCtx = AT_GetCommPsCtxAddr();

        if (psCtx->channelCfg[dataChannelId].used == VOS_TRUE) {
            continue;
        }

        psCtx->channelCfg[dataChannelId].used    = VOS_TRUE;
        psCtx->channelCfg[dataChannelId].rmNetId = AT_PS_GetRmnetIdFromIfaceId((VOS_UINT8)i);
        psCtx->channelCfg[dataChannelId].ifaceId = i;
        psCtx->channelCfg[dataChannelId].cid     = cid;
        psCtx->channelCfg[dataChannelId].modemId = modemId;

        return AT_SUCCESS;
    }

    AT_NORM_LOG("AT_PS_GenIfaceId: IFACEID is all used!");
    return AT_ERROR;
}


VOS_UINT32 AT_PS_CheckDialParamDataChanl(const VOS_UINT8 indexNum, const TAF_IFACE_UserTypeUint8 userType)
{
    AT_PS_DataChanlCfg *chanCfg         = VOS_NULL_PTR;
    VOS_UINT8          *systemAppConfig = VOS_NULL_PTR;
    PS_IFACE_IdUint8    beginIfaceId;
    PS_IFACE_IdUint8    endIfaceId;

    systemAppConfig = AT_GetSystemAppConfigAddr();
    chanCfg         = AT_PS_GetDataChanlCfg(indexNum, (VOS_UINT8)g_atParaList[0].paraValue);

    /* �ֻ���̬������ʹ��^CHDATA���� */
    if ((userType == TAF_IFACE_USER_TYPE_APP) && (*systemAppConfig != SYSTEM_APP_WEBUI)) {
        /* ���ͨ��ӳ�� */
        if ((chanCfg->used == VOS_FALSE) || (chanCfg->rmNetId == AT_PS_INVALID_RMNET_ID)) {
            AT_NORM_LOG2("AT_PS_CheckDialParamDataChanl: Used is .RmNetId is .\n", chanCfg->used, chanCfg->rmNetId);
            return AT_CME_INCORRECT_PARAMETERS;
        }

        return AT_SUCCESS;
    }

    /* ����ʱ�����ֻ���̬�Ĳ������ʹ����^CHDATA���ã���ֱ��ʹ��������Ϣ��������Ҫ�������IFACEID */
    if (g_atParaList[1].paraValue == TAF_PS_CALL_TYPE_UP) {
        if (chanCfg->used == VOS_FALSE) {
            if (userType == TAF_IFACE_USER_TYPE_NDIS) {
                /* NDIS���͵Ĳ���Ĭ��ֱ��ʹ��PS_IFACE_ID_NDIS0 */
                beginIfaceId = PS_IFACE_ID_NDIS0;
#if (FEATURE_MULTI_NCM == FEATURE_ON)
                endIfaceId   = PS_IFACE_ID_NDIS3;
#else
                endIfaceId   = PS_IFACE_ID_NDIS0;
#endif
            } else {
                /* E5��̬APP���Ͳ��ŷ�ΧΪPS_IFACE_ID_RMNET0~PS_IFACE_ID_RMNET7 */
                beginIfaceId = PS_IFACE_ID_RMNET0;
                endIfaceId   = PS_IFACE_ID_RMNET7;
            }

            return AT_PS_GenIfaceId(indexNum, beginIfaceId, endIfaceId, (VOS_UINT8)g_atParaList[0].paraValue);
        }
    }

    return AT_SUCCESS;
}


VOS_UINT32 AT_PS_ValidateDialParam(const VOS_UINT8 indexNum, const TAF_IFACE_UserTypeUint8 userType)
{
    VOS_UINT32 rst;

    /* ����������� */
    if (g_atParseCmd.cmdOptType == AT_CMD_OPT_SET_CMD_NO_PARA) {
        AT_NORM_LOG("AT_PS_ValidateDialParam: No parameter input.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ���������� */
    rst = AT_PS_CheckDialParamCnt(userType);
    if (rst != AT_SUCCESS) {
        return rst;
    }

    /* ��� CID */
    if (g_atParaList[0].paraLen == 0) {
        AT_NORM_LOG("AT_PS_ValidateDialParam: Missing CID.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ���� CONN: �ò�������ʡ��, 1��ʾ��������, 0��ʾ�Ͽ��Ͽ����� */
    if (g_atParaList[1].paraLen == 0) {
        AT_NORM_LOG("AT_PS_ValidateDialParam: Missing connect state.");
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ��� APN */
    rst = AT_PS_CheckDialParamApn(g_atClientTab[indexNum].clientId);
    if (rst != AT_SUCCESS) {
        return rst;
    }

    /* ��� Username */
    if (g_atParaList[AT_PS_DIAL_PARAM_USER_NAME].paraLen > TAF_MAX_AUTHDATA_USERNAME_LEN) {
        AT_NORM_LOG1("AT_PS_ValidateDialParam: Username length is.\n",
                     g_atParaList[AT_PS_DIAL_PARAM_USER_NAME].paraLen);
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ��� Password */
    if (g_atParaList[AT_PS_DIAL_PARAM_PASS_WORD].paraLen > TAF_MAX_AUTHDATA_PASSWORD_LEN) {
        AT_NORM_LOG1("AT_PS_ValidateDialParam: Password length is.\n",
                     g_atParaList[AT_PS_DIAL_PARAM_PASS_WORD].paraLen);
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* ip addr��� */
    rst = AT_PS_CheckDialParamIpAddr();
    if (rst != AT_SUCCESS) {
        AT_NORM_LOG("AT_PS_ValidateDialParam: ip addr is invalid.");
        return rst;
    }

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    if (g_atParaList[AT_PS_DIAL_PARAM_RAT_TYPE].paraLen > 0) {
        if (AT_PS_CheckDialRatType(indexNum,
                                   (VOS_UINT8)g_atParaList[AT_PS_DIAL_PARAM_RAT_TYPE].paraValue) != VOS_TRUE) {
            AT_NORM_LOG1("AT_PS_ValidateDialParam: DialRatType is.\n",
                         g_atParaList[AT_PS_DIAL_PARAM_RAT_TYPE].paraValue);
            return AT_CME_INCORRECT_PARAMETERS;
        }
    }
#endif

    /* ���ͨ��ӳ�� */
    rst = AT_PS_CheckDialParamDataChanl(indexNum, userType);
    if (rst != AT_SUCCESS) {
        return rst;
    }

    return AT_SUCCESS;
}


VOS_UINT16 AT_PS_BuildExClientId(VOS_UINT16 clientId)
{
    ModemIdUint16 modemId;

    modemId = MODEM_ID_0;

    if (AT_GetModemIdFromClient(clientId, &modemId) != VOS_OK) {
        modemId = MODEM_ID_BUTT;
    }

    return TAF_PS_BUILD_EXCLIENTID(modemId, clientId);
}

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)

VOS_UINT32 AT_PS_CheckDialRatType(VOS_UINT8 indexNum, VOS_UINT8 bitRatType)
{
    if (At_CheckCurrRatModeIsCL(indexNum) == VOS_TRUE) {
        switch (bitRatType) {
            case AT_PS_DIAL_RAT_TYPE_NO_ASTRICT:
            case AT_PS_DIAL_RAT_TYPE_1X_OR_HRPD:
            case AT_PS_DIAL_RAT_TYPE_LTE_OR_EHRPD:
                return VOS_TRUE;

            default:
                AT_NORM_LOG1("AT_PS_CheckDialRatType: Rat Type Error.\n", bitRatType);
                return VOS_FALSE;
        }
    } else {
        AT_NORM_LOG("AT_PS_CheckDialRatType: Not CL mode.\n");
        return VOS_FALSE;
    }
}
#endif


VOS_VOID AT_PS_ReportImsCtrlMsgu(VOS_UINT8 indexNum, AT_IMS_CtrlMsgReceiveModuleUint8 module, VOS_UINT32 msgLen,
                                 VOS_UINT8 *dst)
{
    /* ����ֲ����� */
    VOS_UINT16 length;

    length = 0;

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s^IMSCTRLMSGU: %d,%d,\"", g_atCrLf, module, msgLen);

    length += (VOS_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (VOS_INT8 *)g_atSndCodeAddress,
                                                  (VOS_UINT8 *)g_atSndCodeAddress + length, dst, (VOS_UINT16)msgLen);

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "\"%s", g_atCrLf);

    /* ����At_SendResultData���������� */
    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return;
}
#if (FEATURE_IMS == FEATURE_ON)

VOS_VOID AT_PS_ReportImsaFusioncCallCtrlMsgu(VOS_UINT8 indexNum, VOS_UINT8 srcId, VOS_UINT32 msgLen, VOS_UINT8 *msgData)
{
    /* ����ֲ����� */
    VOS_UINT16 length;

    length = 0;

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s^FUSIONCALLRAWU: %d,%d,\"", g_atCrLf, srcId, msgLen);

    length += (VOS_UINT16)At_HexAlpha2AsciiString(AT_CMD_MAX_LEN, (VOS_INT8 *)g_atSndCodeAddress,
                                                  (VOS_UINT8 *)g_atSndCodeAddress + length, msgData, (VOS_UINT16)msgLen);

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "\"%s", g_atCrLf);

    At_SendResultData(indexNum, g_atSndCodeAddress, length);

    return;
}
#endif


VOS_UINT32 AT_PS_BuildIfaceCtrl(const VOS_UINT32 moduleId, const VOS_UINT16 portClientId, const VOS_UINT8 opId,
                                TAF_Ctrl *ctrl)
{
    ModemIdUint16 modemId;

    modemId = MODEM_ID_0;

    /* ��ȡclient id��Ӧ��Modem Id */
    if (AT_GetModemIdFromClient(portClientId, &modemId) == VOS_ERR) {
        AT_ERR_LOG("AT_PS_BuildIfaceCtrl:AT_GetModemIdFromClient is error");
        return VOS_ERR;
    }

    ctrl->moduleId = moduleId;
    ctrl->clientId = AT_PS_BuildExClientId(portClientId);
    ctrl->opId     = opId;

    return VOS_OK;
}


VOS_VOID AT_PS_GetUsrDialAuthType(VOS_UINT8 indexNum, TAF_IFACE_UserTypeUint8 userType,
                                  TAF_IFACE_DialParam *usrDialParam)
{
#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    VOS_UINT8 *systemAppConfig = VOS_NULL_PTR;

    systemAppConfig = AT_GetSystemAppConfigAddr();

    /* AUTH TYPE */
    if ((At_CheckCurrRatModeIsCL(indexNum) == VOS_TRUE) && (*systemAppConfig != SYSTEM_APP_WEBUI) &&
        (userType == TAF_IFACE_USER_TYPE_APP)) {
        usrDialParam->authType = AT_ClGetPdpAuthType(g_atParaList[AT_PS_USR_DIAL_AUTH_TYPE].paraValue,
                                                     g_atParaList[AT_PS_USR_DIAL_AUTH_TYPE].paraLen);
    } else
#endif
    {
        if (g_atParaList[AT_PS_USR_DIAL_AUTH_TYPE].paraLen > 0) {
            usrDialParam->authType = AT_CtrlGetPDPAuthType(g_atParaList[AT_PS_USR_DIAL_AUTH_TYPE].paraValue,
                                                           g_atParaList[AT_PS_USR_DIAL_AUTH_TYPE].paraLen);
        } else {
            usrDialParam->authType = TAF_PDP_AUTH_TYPE_NONE;

            /* ����û��������볤�Ⱦ���Ϊ0, �Ҽ�Ȩ����δ����, ��Ĭ��ʹ��CHAP���� */
            if ((g_atParaList[AT_PS_DIAL_PARAM_USER_NAME].paraLen != 0) &&
                (g_atParaList[AT_PS_DIAL_PARAM_PASS_WORD].paraLen != 0)) {
                usrDialParam->authType = TAF_PDP_AUTH_TYPE_CHAP;
            }
        }
    }

    return;
}

#if (FEATURE_UE_MODE_NR == FEATURE_ON)

VOS_VOID AT_PS_GetUsrDialNrPara(TAF_PDP_PrimContext *pdpCtxInfo, TAF_IFACE_DialParam *usrDialParam)
{
    usrDialParam->sscMode       = pdpCtxInfo->sscMode;
    usrDialParam->alwaysonType  = pdpCtxInfo->alwaysOnInd;
    usrDialParam->multiHomeIPv6 = pdpCtxInfo->mh6Pdu;
    usrDialParam->rQosFlag      = pdpCtxInfo->rQosInd;
    usrDialParam->multiHomeIPv6 = pdpCtxInfo->mh6Pdu;
    usrDialParam->accessDomain  = pdpCtxInfo->prefAccessType;

    /* �����Ƭ��Ϣ */
    if (pdpCtxInfo->sNssai.ucSst != TAF_PS_SNSSAI_SST_INVALID) {
        usrDialParam->bitOpSNssai = VOS_TRUE;
        usrDialParam->sNssai      = pdpCtxInfo->sNssai;
    }
}
#endif



LOCAL VOS_UINT32 AT_CheckPdpTypeAndUserType(TAF_PDP_TypeUint8 pdpType, TAF_IFACE_UserTypeUint8 userType)
{
    switch (pdpType) {
        case TAF_PDP_IPV4:
        case TAF_PDP_IPV6:
        case TAF_PDP_IPV4V6:
            return AT_CheckIpv6Capability(pdpType);

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
        case TAF_PDP_ETHERNET:
            /* Ŀǰ��APPͨ��֧��Ethernet���Ͳ��� */
            if ((AT_GetEthernetCap() == VOS_TRUE) && (userType == TAF_IFACE_USER_TYPE_APP)) {
                return VOS_OK;
            }
            break;
#endif
        default :
            break;
    }

    return VOS_ERR;
}


VOS_UINT32 AT_PS_GetUsrDialParam(const VOS_UINT8 indexNum, const TAF_IFACE_UserTypeUint8 userType,
                                 TAF_IFACE_DialParam *usrDialParam)
{
    /* �ɵ����߱�֤��κͳ�����Ч�� */
    VOS_UINT8          *systemAppConfig = VOS_NULL_PTR;
    TAF_PDP_PrimContext pdpCtxInfo;
    VOS_UINT32          rslt;
    errno_t             memResult;

    /* User Type */
    usrDialParam->userType = userType;

    /* CID */
    usrDialParam->cid = (VOS_UINT8)g_atParaList[0].paraValue;

    /* APN */
    usrDialParam->apnLen = (VOS_UINT8)g_atParaList[AT_PS_DIAL_PARAM_APN].paraLen;
    if ((g_atParaList[AT_PS_DIAL_PARAM_APN].paraLen > 0) &&
        (g_atParaList[AT_PS_DIAL_PARAM_APN].paraLen <= sizeof(usrDialParam->apn))) {
        memResult = memcpy_s(usrDialParam->apn, sizeof(usrDialParam->apn), g_atParaList[AT_PS_DIAL_PARAM_APN].para,
                             g_atParaList[AT_PS_DIAL_PARAM_APN].paraLen);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(usrDialParam->apn), g_atParaList[AT_PS_DIAL_PARAM_APN].paraLen);
    }

    /* Username */
    usrDialParam->usernameLen = (VOS_UINT8)g_atParaList[AT_PS_DIAL_PARAM_USER_NAME].paraLen;
    if ((g_atParaList[AT_PS_DIAL_PARAM_USER_NAME].paraLen > 0) &&
        (g_atParaList[AT_PS_DIAL_PARAM_USER_NAME].paraLen <= sizeof(usrDialParam->username))) {
        memResult = memcpy_s(usrDialParam->username, sizeof(usrDialParam->username),
                             g_atParaList[AT_PS_DIAL_PARAM_USER_NAME].para,
                             g_atParaList[AT_PS_DIAL_PARAM_USER_NAME].paraLen);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(usrDialParam->username),
                            g_atParaList[AT_PS_DIAL_PARAM_USER_NAME].paraLen);
    }

    /* Password */
    usrDialParam->passwordLen = (VOS_UINT8)g_atParaList[AT_PS_DIAL_PARAM_PASS_WORD].paraLen;
    if ((g_atParaList[AT_PS_DIAL_PARAM_PASS_WORD].paraLen > 0) &&
        (g_atParaList[AT_PS_DIAL_PARAM_PASS_WORD].paraLen <= sizeof(usrDialParam->password))) {
        memResult = memcpy_s(usrDialParam->password, sizeof(usrDialParam->password),
                             g_atParaList[AT_PS_DIAL_PARAM_PASS_WORD].para,
                             g_atParaList[AT_PS_DIAL_PARAM_PASS_WORD].paraLen);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(usrDialParam->password),
                            g_atParaList[AT_PS_DIAL_PARAM_PASS_WORD].paraLen);
    }

    /* AUTH TYPE */
    AT_PS_GetUsrDialAuthType(indexNum, userType, usrDialParam);

    /* ADDR */
    /* ֻ��E5���ź�NDIS���Ų���Ҫ��� */
    systemAppConfig = AT_GetSystemAppConfigAddr();
    if ((((userType == TAF_IFACE_USER_TYPE_APP) && (*systemAppConfig == SYSTEM_APP_WEBUI)) ||
         (userType == TAF_IFACE_USER_TYPE_NDIS)) &&
        ((g_atParaList[AT_PS_DIAL_PARAM_IP_ADDR].paraLen > 0) &&
         (g_atParaList[AT_PS_DIAL_PARAM_IP_ADDR].paraLen <= TAF_MAX_IPV4_ADDR_STR_LEN))) {
        usrDialParam->opIPv4ValidFlag = VOS_TRUE;
        memResult = memcpy_s((VOS_CHAR *)usrDialParam->ipv4Addr, sizeof(usrDialParam->ipv4Addr),
                             (VOS_CHAR *)g_atParaList[AT_PS_DIAL_PARAM_IP_ADDR].para,
                             g_atParaList[AT_PS_DIAL_PARAM_IP_ADDR].paraLen);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(usrDialParam->ipv4Addr), g_atParaList[AT_PS_DIAL_PARAM_IP_ADDR].paraLen);
    }

    /* PDN TYPE */
    (VOS_VOID)memset_s(&pdpCtxInfo, sizeof(pdpCtxInfo), 0x00, sizeof(pdpCtxInfo));
    rslt = TAF_AGENT_GetPdpCidPara(&pdpCtxInfo, indexNum, usrDialParam->cid);

    if ((rslt == VOS_OK) && (AT_CheckPdpTypeAndUserType(pdpCtxInfo.pdpAddr.pdpType, userType) == VOS_OK)) {
        usrDialParam->pdpType = pdpCtxInfo.pdpAddr.pdpType;
    } else {
        if (AT_GetModemMbbCustCtxAddrFromClientId(indexNum)->dialStickFlg == VOS_TRUE) {
            AT_GetMbbUsrDialPdpType(usrDialParam);
        } else {
            AT_LOG1("AT_PS_GetUsrDialParam: PDP type is not supported.", pdpCtxInfo.pdpAddr.pdpType);
            return VOS_ERR;
        }
    }

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    if (g_atParaList[AT_PS_DIAL_PARAM_RAT_TYPE].paraLen > 0) {
        usrDialParam->ratType = (VOS_UINT8)g_atParaList[AT_PS_DIAL_PARAM_RAT_TYPE].paraValue;
    }

#endif

#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    AT_PS_GetUsrDialNrPara(&pdpCtxInfo, usrDialParam);
#endif

    return VOS_OK;
}


VOS_UINT32 AT_PS_ProcIfaceUp(const VOS_UINT8 indexNum, const TAF_IFACE_UserTypeUint8 userType)
{
    AT_PS_DataChanlCfg *chanCfg = VOS_NULL_PTR;
    TAF_Ctrl            ctrl;
    TAF_IFACE_Active    ifaceUp;

    (VOS_VOID)memset_s(&ctrl, sizeof(ctrl), 0x00, sizeof(ctrl));

    /* ������ƽṹ�� */
    if (AT_PS_BuildIfaceCtrl(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0, &ctrl) == VOS_ERR) {
        AT_PS_SetPsCallErrCause(indexNum, TAF_PS_CAUSE_UNKNOWN);
        return AT_ERROR;
    }

    (VOS_VOID)memset_s(&ifaceUp, sizeof(ifaceUp), 0x00, sizeof(ifaceUp));

    /* ����������Ϣ��ֵ */
    if (AT_PS_GetUsrDialParam(indexNum, userType, &(ifaceUp.usrDialParam)) == VOS_ERR) {
        AT_ERR_LOG("AT_PS_ProcIfaceUp: AT_PS_GetUsrDialParam is error.");
        AT_PS_SetPsCallErrCause(indexNum, TAF_PS_CAUSE_UNKNOWN);
        (VOS_VOID)memset_s(&ifaceUp, sizeof(ifaceUp), 0x00, sizeof(ifaceUp));
        return AT_ERROR;
    }

    chanCfg         = AT_PS_GetDataChanlCfg(indexNum, (VOS_UINT8)g_atParaList[0].paraValue);
    ifaceUp.ifaceId = (VOS_UINT8)chanCfg->ifaceId;

    /* ��������������� */
    if (TAF_IFACE_Up(&ctrl, &ifaceUp) != VOS_OK) {
        AT_ERR_LOG("AT_PS_ProcIfaceUp: TAF_IFACE_Up is error.");
        AT_PS_SetPsCallErrCause(indexNum, TAF_PS_CAUSE_UNKNOWN);
        (VOS_VOID)memset_s(&ifaceUp, sizeof(ifaceUp), 0x00, sizeof(ifaceUp));
        return AT_ERROR;
    }

    /* ���淢��IPFACE UP��PortIndex */
    chanCfg->portIndex = indexNum;
    (VOS_VOID)memset_s(&ifaceUp, sizeof(ifaceUp), 0x00, sizeof(ifaceUp));
    return AT_SUCCESS;
}


VOS_UINT32 AT_PS_ProcIfaceDown(const VOS_UINT8 indexNum, const TAF_IFACE_UserTypeUint8 userType)
{
    AT_PS_DataChanlCfg *chanCfg = VOS_NULL_PTR;
    TAF_Ctrl            ctrl;
    TAF_IFACE_Deactive  ifaceDown;

    chanCfg = AT_PS_GetDataChanlCfg(indexNum, (VOS_UINT8)g_atParaList[0].paraValue);

    if ((chanCfg->used == VOS_FALSE) || (chanCfg->ifaceId == AT_PS_INVALID_IFACE_ID)) {
        AT_NORM_LOG("AT_PS_ProcIfaceDown: Iface is not act.");
        return AT_ERROR;
    }

    (VOS_VOID)memset_s(&ctrl, sizeof(ctrl), 0x00, sizeof(ctrl));

    /* ������ƽṹ�� */
    if (AT_PS_BuildIfaceCtrl(WUEPS_PID_AT, g_atClientTab[indexNum].clientId, 0, &ctrl) == VOS_ERR) {
        return AT_ERROR;
    }

    /* ����ȥ������Ϣ��ֵ */
    (VOS_VOID)memset_s(&ifaceDown, sizeof(ifaceDown), 0x00, sizeof(ifaceDown));
    ifaceDown.ifaceId  = (VOS_UINT8)chanCfg->ifaceId;
    ifaceDown.cause    = AT_PS_TransCallEndCause((VOS_UINT8)g_atParaList[1].paraValue);
    ifaceDown.userType = userType;

    /* ��������ȥ������� */
    if (TAF_IFACE_Down(&ctrl, &ifaceDown) != VOS_OK) {
        AT_ERR_LOG("AT_PS_ProcIfaceDown: TAF_IFACE_Down is error.");
        return AT_ERROR;
    }

    return AT_SUCCESS;
}


VOS_UINT32 AT_PS_ProcIfaceCmd(const VOS_UINT8 indexNum, const TAF_IFACE_UserTypeUint8 userType)
{
    VOS_UINT8          *systemAppConfig = VOS_NULL_PTR;
    AT_PS_DataChanlCfg *chanCfg         = VOS_NULL_PTR;
    VOS_UINT32          rst;

    if (g_atParaList[1].paraValue == TAF_PS_CALL_TYPE_UP) {
        systemAppConfig = AT_GetSystemAppConfigAddr();

        if ((userType == TAF_IFACE_USER_TYPE_APP) && (*systemAppConfig == SYSTEM_APP_WEBUI) &&
            (g_hiLinkMode == AT_HILINK_GATEWAY_MODE)) {
            /* ��¼PS����д����� */
            AT_PS_SetPsCallErrCause(indexNum, TAF_PS_CAUSE_UNKNOWN);

            /* ���ͨ��ӳ�� */
            chanCfg = AT_PS_GetDataChanlCfg(indexNum, (VOS_UINT8)g_atParaList[0].paraValue);
            AT_CleanDataChannelCfg(chanCfg);

            return AT_ERROR;
        }

        rst = AT_PS_ProcIfaceUp(indexNum, userType);

        /* MBB��̬����Ҫ���ͨ��ӳ�� */
        if ((rst != AT_SUCCESS) && (*systemAppConfig != SYSTEM_APP_ANDROID)) {
            /* ���ͨ��ӳ�� */
            chanCfg = AT_PS_GetDataChanlCfg(indexNum, (VOS_UINT8)g_atParaList[0].paraValue);
            AT_CleanDataChannelCfg(chanCfg);
        }
    } else {
        rst = AT_PS_ProcIfaceDown(indexNum, userType);
    }

    if (rst == AT_SUCCESS) {
        /* ���õ�ǰ�������� */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_NDISDUP_SET;

        /* ������������״̬ */
        rst = AT_WAIT_ASYNC_RETURN;
    }

    return rst;
}


VOS_UINT32 AT_PS_ProcMapconMsg(const VOS_UINT16 clientId, const AT_MAPCON_CTRL_MSG_STRU *atMapConMsgPara)
{
    TAF_Ctrl          ctrl;
    TAF_PS_EpdgCtrl   epdgCtrl;
    VOS_UINT_PTR      msgContext;
    VOS_UINT32        msgType;
    errno_t           memResult;

    msgType = TAF_GET_HOST_UINT32(atMapConMsgPara->msgContext);
    msgContext = (VOS_UINT_PTR)atMapConMsgPara->msgContext;
    (VOS_VOID)memset_s(&ctrl, sizeof(ctrl), 0x00, sizeof(ctrl));
    (VOS_VOID)memset_s(&epdgCtrl, sizeof(epdgCtrl), 0x00, sizeof(epdgCtrl));

    AT_NORM_LOG1("AT_PS_ProcMapconMsg: Msg Type is ", msgType);

    if (AT_PS_BuildIfaceCtrl(WUEPS_PID_AT, clientId, 0, &ctrl) == VOS_ERR) {
        return VOS_ERR;
    }

    switch (msgType) {
        case ID_WIFI_IMSA_IMS_PDN_ACTIVATE_CNF:

            if (atMapConMsgPara->msgLen != sizeof(TAF_PS_EpdgActCnfInfo)) {
                AT_ERR_LOG("AT_PS_ProcMapconMsg: activate cnf length is error!");
                return VOS_ERR;
            }

            epdgCtrl.opActCnf = VOS_TRUE;

            memResult = memcpy_s(&(epdgCtrl.actCnfInfo), sizeof(epdgCtrl.actCnfInfo), (VOS_UINT8*)msgContext,
                                 sizeof(TAF_PS_EpdgActCnfInfo));
            TAF_MEM_CHK_RTN_VAL(memResult, sizeof(epdgCtrl.actCnfInfo), sizeof(TAF_PS_EpdgActCnfInfo));
            break;

        case ID_WIFI_IMSA_IMS_PDN_DEACTIVATE_CNF:

            if (atMapConMsgPara->msgLen != sizeof(TAF_PS_EpdgDeactCnfInfo)) {
                AT_ERR_LOG("AT_PS_ProcMapconMsg: deactivate cnf length is error!");
                return VOS_ERR;
            }

            epdgCtrl.opDeActCnf = VOS_TRUE;

            memResult = memcpy_s(&(epdgCtrl.deActCnfInfo), sizeof(epdgCtrl.deActCnfInfo),
                                 (VOS_UINT8*)msgContext, sizeof(TAF_PS_EpdgDeactCnfInfo));
            TAF_MEM_CHK_RTN_VAL(memResult, sizeof(epdgCtrl.deActCnfInfo), sizeof(TAF_PS_EpdgDeactCnfInfo));
            break;

        case ID_WIFI_IMSA_IMS_PDN_DEACTIVATE_IND:

            if (atMapConMsgPara->msgLen != sizeof(TAF_PS_EpdgDeactIndInfo)) {
                AT_ERR_LOG("AT_PS_ProcMapconMsg: deactivate ind length is error!");
                return VOS_ERR;
            }

            epdgCtrl.opDeActInd = VOS_TRUE;

            memResult = memcpy_s(&(epdgCtrl.deActIndInfo), sizeof(epdgCtrl.deActIndInfo),
                                 (VOS_UINT8*)msgContext, sizeof(TAF_PS_EpdgDeactIndInfo));
            TAF_MEM_CHK_RTN_VAL(memResult, sizeof(epdgCtrl.deActIndInfo), sizeof(TAF_PS_EpdgDeactIndInfo));
            break;

        default:
            AT_ERR_LOG("AT_PS_ProcMapconMsg: Msg Type is error!");
            return VOS_ERR;
    }

    return TAF_PS_EpdgCtrlMsg(&ctrl, &epdgCtrl);
}


VOS_VOID AT_PS_ReportAppIfaceUp(const TAF_IFACE_StatusInd *ifaceStatus)
{
    VOS_UINT16 length;

    length = 0;

    if ((ifaceStatus->pdpType & TAF_PDP_IPV4) == TAF_PDP_IPV4) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s^DCONN:%d,\"IPV4\"%s", g_atCrLf, ifaceStatus->cid, g_atCrLf);
    }

    if ((ifaceStatus->pdpType & TAF_PDP_IPV6) == TAF_PDP_IPV6) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s^DCONN:%d,\"IPV6\"%s", g_atCrLf, ifaceStatus->cid, g_atCrLf);
    }

    if (ifaceStatus->pdpType == TAF_PDP_ETHERNET) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s^DCONN:%d,\"Ethernet\"%s", g_atCrLf, ifaceStatus->cid,
            g_atCrLf);
    }

    At_SendResultData((VOS_UINT8)ifaceStatus->ctrl.clientId, g_atSndCodeAddress, length);

    return;
}


LOCAL VOS_VOID AT_ReportDownExPara(const TAF_IFACE_StatusInd *ifaceStatus, VOS_UINT16 *length)
{
    /* ^DEND:<CID>,<CAUSE>,<IP-TYPE>[,<back-off-timer>,<allowed_ssc_mode>] */
    if ((ifaceStatus->opBackOffTimer == VOS_TRUE) && (ifaceStatus->opAllowedSscMode == VOS_TRUE)) {
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, ",%d,0x%02x%s", ifaceStatus->backOffTimer,
            ifaceStatus->allowedSscMode, g_atCrLf);
    } else if (ifaceStatus->opBackOffTimer == VOS_TRUE) {
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, ",%d,%s", ifaceStatus->backOffTimer, g_atCrLf);
    } else if (ifaceStatus->opAllowedSscMode == VOS_TRUE) {
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, ",,0x%02x%s", ifaceStatus->allowedSscMode, g_atCrLf);
    } else {
        *length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + *length, "%s", g_atCrLf);
    }
}


VOS_VOID AT_PS_ReportAppIfaceDown(const TAF_IFACE_StatusInd *ifaceStatus)
{
    VOS_UINT16 length = 0;

    if ((ifaceStatus->pdpType & TAF_PDP_IPV4) == TAF_PDP_IPV4) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s^DEND:%d,%d,\"IPV4\"", g_atCrLf, ifaceStatus->cid,
            ifaceStatus->cause);

        AT_ReportDownExPara(ifaceStatus, &length);
    }

    if ((ifaceStatus->pdpType & TAF_PDP_IPV6) == TAF_PDP_IPV6) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s^DEND:%d,%d,\"IPV6\"", g_atCrLf, ifaceStatus->cid,
            ifaceStatus->cause);

        AT_ReportDownExPara(ifaceStatus, &length);
    }

    if (ifaceStatus->pdpType == TAF_PDP_ETHERNET) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, "%s^DEND:%d,%d,\"Ethernet\"", g_atCrLf, ifaceStatus->cid,
            ifaceStatus->cause);

        AT_ReportDownExPara(ifaceStatus, &length);
    }

    At_SendResultData((VOS_UINT8)ifaceStatus->ctrl.clientId, g_atSndCodeAddress, length);
}


VOS_VOID AT_PS_ReportNdisIfaceStat(const TAF_IFACE_StatusInd *ifaceStatus)
{
    VOS_UINT32 ul3gppSmCause = AT_Get3gppSmCauseByPsCause(ifaceStatus->cause);
    VOS_UINT16 length = 0;

    if ((ifaceStatus->pdpType & TAF_PDP_IPV4) == TAF_PDP_IPV4) {
        if (ifaceStatus->status == TAF_IFACE_STATUS_DEACT) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s:0,%d,,\"IPV4\"%s", g_atCrLf,
                g_atStringTab[AT_STRING_NDISSTAT].text, ul3gppSmCause, g_atCrLf);
        } else {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s:1,,,\"IPV4\"%s", g_atCrLf,
                g_atStringTab[AT_STRING_NDISSTAT].text, g_atCrLf);
        }
    }

    if ((ifaceStatus->pdpType & TAF_PDP_IPV6) == TAF_PDP_IPV6) {
        if (ifaceStatus->status == TAF_IFACE_STATUS_DEACT) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s:0,%d,,\"IPV6\"%s", g_atCrLf,
                g_atStringTab[AT_STRING_NDISSTAT].text, ul3gppSmCause, g_atCrLf);
        } else {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s:1,,,\"IPV6\"%s", g_atCrLf,
                g_atStringTab[AT_STRING_NDISSTAT].text, g_atCrLf);
        }
    }

    if (ifaceStatus->pdpType == TAF_PDP_ETHERNET) {
        if (ifaceStatus->status == TAF_IFACE_STATUS_DEACT) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s:0,%d,,\"Ethernet\"%s", g_atCrLf,
                g_atStringTab[AT_STRING_NDISSTAT].text, ul3gppSmCause, g_atCrLf);
        } else {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s:1,,,\"Ethernet\"%s", g_atCrLf,
                g_atStringTab[AT_STRING_NDISSTAT].text, g_atCrLf);
        }
    }

    At_SendResultData((VOS_UINT8)ifaceStatus->ctrl.clientId, g_atSndCodeAddress, length);
}


VOS_VOID AT_PS_ReportNdisIfaceStatEx(const TAF_IFACE_StatusInd *ifaceStatus)
{
    VOS_UINT32 ul3gppSmCause = AT_Get3gppSmCauseByPsCause(ifaceStatus->cause);
    VOS_UINT16 length = 0;

    if ((ifaceStatus->pdpType & TAF_PDP_IPV4) == TAF_PDP_IPV4) {
        if (ifaceStatus->status == TAF_IFACE_STATUS_DEACT) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s:%d,0,%d,,\"IPV4\"%s", g_atCrLf,
                g_atStringTab[AT_STRING_NDISSTATEX].text, ifaceStatus->cid, ul3gppSmCause, g_atCrLf);
        } else {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s:%d,1,,,\"IPV4\"%s", g_atCrLf,
                g_atStringTab[AT_STRING_NDISSTATEX].text, ifaceStatus->cid, g_atCrLf);
        }
    }

    if ((ifaceStatus->pdpType & TAF_PDP_IPV6) == TAF_PDP_IPV6) {
        if (ifaceStatus->status == TAF_IFACE_STATUS_DEACT) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s:%d,0,%d,,\"IPV6\"%s", g_atCrLf,
                g_atStringTab[AT_STRING_NDISSTATEX].text, ifaceStatus->cid, ul3gppSmCause, g_atCrLf);
        } else {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s:%d,1,,,\"IPV6\"%s", g_atCrLf,
                g_atStringTab[AT_STRING_NDISSTATEX].text, ifaceStatus->cid, g_atCrLf);
        }
    }

    if (ifaceStatus->pdpType == TAF_PDP_ETHERNET) {
        if (ifaceStatus->status == TAF_IFACE_STATUS_DEACT) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s:%d,0,%d,,\"Ethernet\"%s", g_atCrLf,
                g_atStringTab[AT_STRING_NDISSTATEX].text, ifaceStatus->cid, ul3gppSmCause, g_atCrLf);
        } else {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s:%d,1,,,\"Ethernet\"%s", g_atCrLf,
                g_atStringTab[AT_STRING_NDISSTATEX].text, ifaceStatus->cid, g_atCrLf);
        }
    }

    At_SendResultData((VOS_UINT8)ifaceStatus->ctrl.clientId, g_atSndCodeAddress, length);
}

#if (FEATURE_MULTI_NCM == FEATURE_ON)

VOS_VOID AT_PS_ReportMultiNdisIfaceStat(const TAF_IFACE_StatusInd *ifaceStatus)
{
    VOS_UINT32 ul3gppSmCause = AT_Get3gppSmCauseByPsCause(ifaceStatus->cause);
    VOS_UINT16 length = 0;

    if ((ifaceStatus->pdpType & TAF_PDP_IPV4) == TAF_PDP_IPV4) {
        if (ifaceStatus->status == TAF_IFACE_STATUS_DEACT) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s:%d,0,%d,,\"IPV4\"%s", g_atCrLf,
                g_atStringTab[AT_STRING_NDISSTAT].text, ifaceStatus->cid, ul3gppSmCause, g_atCrLf);
        } else {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s:%d,1,,,\"IPV4\"%s", g_atCrLf,
                g_atStringTab[AT_STRING_NDISSTAT].text, ifaceStatus->cid, g_atCrLf);
        }
    }

    if ((ifaceStatus->pdpType & TAF_PDP_IPV6) == TAF_PDP_IPV6) {
        if (ifaceStatus->status == TAF_IFACE_STATUS_DEACT) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s:%d,0,%d,,\"IPV6\"%s", g_atCrLf,
                g_atStringTab[AT_STRING_NDISSTAT].text, ifaceStatus->cid, ul3gppSmCause, g_atCrLf);
        } else {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s:%d,1,,,\"IPV6\"%s", g_atCrLf,
                g_atStringTab[AT_STRING_NDISSTAT].text, ifaceStatus->cid, g_atCrLf);
        }
    }

    if (ifaceStatus->pdpType == TAF_PDP_ETHERNET) {
        if (ifaceStatus->status == TAF_IFACE_STATUS_DEACT) {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s:%d,0,%d,,\"Ethernet\"%s", g_atCrLf,
                g_atStringTab[AT_STRING_NDISSTAT].text, ifaceStatus->cid, ul3gppSmCause, g_atCrLf);
        } else {
            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s:%d,1,,,\"Ethernet\"%s", g_atCrLf,
                g_atStringTab[AT_STRING_NDISSTAT].text, ifaceStatus->cid, g_atCrLf);
        }
    }

    At_SendResultData((VOS_UINT8)ifaceStatus->ctrl.clientId, g_atSndCodeAddress, length);
}
#endif


VOS_VOID AT_PS_ProcAppIfaceStatus(TAF_IFACE_StatusInd *ifaceStatus)
{
    VOS_UINT8 *systemAppConfig = VOS_NULL_PTR;

    systemAppConfig = AT_GetSystemAppConfigAddr();

    /* ����E5�����翨��E355����̬�Ĳ��� */
    /* PCUI���·��ϱ�^NDISSTAT��APP���·��ϱ�^NDISSTATEX */
    if ((*systemAppConfig) == SYSTEM_APP_WEBUI) {
        if ((ifaceStatus->status == TAF_IFACE_STATUS_ACT) || (ifaceStatus->status == TAF_IFACE_STATUS_DEACT)) {
            if (AT_CheckAppUser((VOS_UINT8)ifaceStatus->ctrl.clientId) == VOS_TRUE) {
                AT_PS_ReportNdisIfaceStatEx(ifaceStatus);
                return;
            }

            AT_PS_ReportNdisIfaceStat(ifaceStatus);
        }

        return;
    }

    if (ifaceStatus->status == TAF_IFACE_STATUS_ACT) {
        AT_PS_ReportAppIfaceUp(ifaceStatus);
    }

    if (ifaceStatus->status == TAF_IFACE_STATUS_DEACT) {
        AT_PS_ReportAppIfaceDown(ifaceStatus);
    }

    return;
}


VOS_VOID AT_PS_ProcNdisIfaceStatus(TAF_IFACE_StatusInd *ifaceStatus)
{
    if ((ifaceStatus->status == TAF_IFACE_STATUS_ACT) || (ifaceStatus->status == TAF_IFACE_STATUS_DEACT)) {
#if (FEATURE_MULTI_NCM == FEATURE_OFF)
        AT_PS_ReportNdisIfaceStat(ifaceStatus);
#else
        AT_PS_ReportMultiNdisIfaceStat(ifaceStatus);
#endif
    }

    return;
}


AT_PS_RPT_IFACE_RSLT_FUNC AT_PS_GetRptIfaceResultFunc(const TAF_IFACE_UserTypeUint8 userType)
{
    const AT_PS_ReportIfaceResult *rptIfaceRsltFuncTblPtr = VOS_NULL_PTR;
    AT_PS_RPT_IFACE_RSLT_FUNC      rptIfaceRsltFunc       = VOS_NULL_PTR;
    VOS_UINT32                     cnt;

    rptIfaceRsltFuncTblPtr = AT_PS_GET_RPT_IFACE_RSLT_FUNC_TBL_PTR();

    /* �û�����ƥ�� */
    for (cnt = 0; cnt < AT_PS_GET_RPT_IFACE_RSLT_FUNC_TBL_SIZE(); cnt++) {
        if (userType == rptIfaceRsltFuncTblPtr[cnt].userType) {
            rptIfaceRsltFunc = rptIfaceRsltFuncTblPtr[cnt].rptIfaceRsltFunc;
            break;
        }
    }

    return rptIfaceRsltFunc;
}


VOS_VOID AT_PS_ChangeFcPoint(const ModemIdUint16 modemId, const FC_PriTypeUint8 fCPri, const FC_IdUint8 fcId)
{
    if (fcId != FC_ID_BUTT) {
        if (FC_ChangePoint(fcId, FC_POLICY_ID_MEM, fCPri, modemId) != VOS_OK) {
            AT_ERR_LOG("AT_PS_ChangeFcPoint: ERROR: Change FC Point Failed.");
        }

        if (FC_ChangePoint(fcId, FC_POLICY_ID_CPU_A, fCPri, modemId) != VOS_OK) {
            AT_ERR_LOG("AT_PS_ChangeFcPoint: ERROR: Change FC Point Failed.");
        }

        if (FC_ChangePoint(fcId, FC_POLICY_ID_CDS, fCPri, modemId) != VOS_OK) {
            AT_ERR_LOG("AT_PS_ChangeFcPoint: ERROR: Change FC Point Failed.");
        }

        if (FC_ChangePoint(fcId, FC_POLICY_ID_GPRS, fCPri, modemId) != VOS_OK) {
            AT_ERR_LOG("AT_PS_ChangeFcPoint: ERROR: Change FC Point Failed.");
        }
    }

    return;
}


VOS_VOID AT_PS_RegAppFcPoint(const FC_IdUint8 fcId, const ModemIdUint16 modemId, const TAF_IFACE_RegFcInd *regFcInd)
{
    FC_RegPointPara regFcPoint;
    FC_PriTypeUint8 defaultFcPri;

    defaultFcPri = FC_PRI_FOR_PDN_LOWEST;
    (VOS_VOID)memset_s(&regFcPoint, sizeof(regFcPoint), 0x00, sizeof(regFcPoint));

#if (FEATURE_ACPU_FC_POINT_REG == FEATURE_ON)
    /* ����ͨ����RABIDӳ���ϵ */
    FC_ChannelMapCreate(fcId, regFcInd->rabId, modemId);
#endif

    /* ����������������ȼ�RAB QoS���ȼ�������,���ȼ��ı�ʱ����Ҫ�ı����ȼ� */
    /*
     * FC_PRI_3        ��������ȼ��ĳ���
     * FC_PRI_4        ��NONGBR����
     * FC_PRI_5        ��GBR����
     */
    regFcPoint.fcId  = fcId;
    regFcPoint.fcPri = defaultFcPri;

    regFcPoint.modemId = modemId;
    regFcPoint.clrFunc = AT_PS_AppClearFlowCtrl;
    regFcPoint.setFunc = AT_PS_AppSetFlowCtrl;

    /* Paramter1����ΪRmNetId, Paramter2����ΪFCID */
    regFcPoint.param1  = AT_PS_GetRmnetIdFromIfaceId(regFcInd->ifaceId);
    regFcPoint.param2  = fcId;
    regFcPoint.rstFunc = AT_ResetFlowCtl;

    /* ע�����ص�, ��Ҫ�ֱ�ע��MEM��CDS */
    regFcPoint.policyId = FC_POLICY_ID_MEM;
    if (FC_RegPoint(&regFcPoint) != VOS_OK) {
        AT_ERR_LOG("AT_PS_RegAppFcPoint: ERROR: reg mem point Failed.");
        return;
    }

    regFcPoint.policyId = FC_POLICY_ID_CDS;
    if (FC_RegPoint(&regFcPoint) != VOS_OK) {
        AT_ERR_LOG("AT_PS_RegAppFcPoint: ERROR: reg CDS point Failed.");
        return;
    }

    /* ����FCID��FC Pri��ӳ���ϵ */
    g_fcIdMaptoFcPri[fcId].used  = VOS_TRUE;
    g_fcIdMaptoFcPri[fcId].fcPri = defaultFcPri;
    g_fcIdMaptoFcPri[fcId].rabIdMask |= ((VOS_UINT32)1 << (regFcInd->rabId));
    g_fcIdMaptoFcPri[fcId].modemId = modemId;

    /* ��������Ϣ */
    AT_MntnTraceRegFcPoint((VOS_UINT8)regFcInd->ctrl.clientId, AT_FC_POINT_TYPE_RMNET);

    return;
}


VOS_UINT32 AT_PS_DeRegAppFcPoint(const FC_IdUint8 fcId, const ModemIdUint16 modemId,
                                 const TAF_IFACE_DeregFcInd *deRegFcInd)
{
    if ((g_fcIdMaptoFcPri[fcId].rabIdMask & (0x01UL << deRegFcInd->rabId)) == 0) {
        AT_ERR_LOG1("AT_PS_DeRegAppFcPoint: RabId err, RabIdMask :", g_fcIdMaptoFcPri[fcId].rabIdMask);
        return VOS_ERR;
    }

#if (FEATURE_ACPU_FC_POINT_REG == FEATURE_ON)
    /* �ڵ���FC_DeRegPointǰ,�ȵ���FC_ChannelMapDelete */
    FC_ChannelMapDelete(deRegFcInd->rabId, modemId);
#endif

    if (FC_DeRegPoint(fcId, modemId) != VOS_OK) {
        AT_ERR_LOG("AT_PS_DeRegAppFcPoint: ERROR: de reg point Failed.");
        return VOS_ERR;
    }

    /* ���FCID��FC Pri��ӳ���ϵ */
    g_fcIdMaptoFcPri[fcId].rabIdMask &= ~((VOS_UINT32)1 << (deRegFcInd->rabId));

    if (g_fcIdMaptoFcPri[fcId].rabIdMask == 0) {
        g_fcIdMaptoFcPri[fcId].used    = VOS_FALSE;
        g_fcIdMaptoFcPri[fcId].fcPri   = FC_PRI_BUTT;
        g_fcIdMaptoFcPri[fcId].modemId = MODEM_ID_BUTT;
    }

    /* ��������Ϣ */
    AT_MntnTraceDeregFcPoint((VOS_UINT8)deRegFcInd->ctrl.clientId, AT_FC_POINT_TYPE_RMNET);

    return VOS_OK;
}


VOS_UINT32 AT_PS_RegNdisFcPoint(const TAF_IFACE_RegFcInd *regFcInd, const FC_IdUint8 fcId, const ModemIdUint16 modemId)
{
    FC_RegPointPara regFcPoint;
    FC_PriTypeUint8 fCPri;

    fCPri = FC_PRI_FOR_PDN_NONGBR;
    (VOS_VOID)memset_s(&regFcPoint, sizeof(regFcPoint), 0x00, sizeof(regFcPoint));

#if (FEATURE_ACPU_FC_POINT_REG == FEATURE_ON)
    /* ����ͨ����RABIDӳ���ϵ */
    FC_ChannelMapCreate(fcId, regFcInd->rabId, modemId);
#endif

    regFcPoint.fcId = fcId;

    /* ����������������ȼ�RAB QoS���ȼ�������,���ȼ��ı�ʱ����Ҫ�ı����ȼ� */
    /*
     * FC_PRI_3        ��������ȼ��ĳ���
     * FC_PRI_4        ��NONGBR����
     * FC_PRI_5        ��GBR����
     */

    if (regFcInd->opUmtsQos == TAF_USED) {
        fCPri = AT_GetFCPriFromQos(&regFcInd->umtsQos);
    }

    regFcPoint.fcPri    = fCPri;
    regFcPoint.policyId = FC_POLICY_ID_MEM;
    regFcPoint.modemId  = modemId;
    regFcPoint.clrFunc  = AT_PS_DisableNdisFlowCtl;
    regFcPoint.setFunc  = AT_PS_EnableNdisFlowCtl;
    regFcPoint.rstFunc  = AT_ResetFlowCtl;
    regFcPoint.param2   = fcId;
    regFcPoint.param1   = (VOS_UINT32)regFcInd->ctrl.clientId;

    /* ע�����ص�,��Ҫ�ֱ�ע��MEM,CPU,CDS��GPRS�� */
    if (FC_RegPoint(&regFcPoint) != VOS_OK) {
        AT_ERR_LOG("AT_PS_RegNdisFcPoint: ERROR: reg mem point Failed.");
        return VOS_ERR;
    }

    regFcPoint.policyId = FC_POLICY_ID_CPU_A;
    if (FC_RegPoint(&regFcPoint) != VOS_OK) {
        AT_ERR_LOG("AT_PS_RegNdisFcPoint: ERROR: reg a cpu point Failed.");
        return VOS_ERR;
    }

    regFcPoint.policyId = FC_POLICY_ID_CDS;
    if (FC_RegPoint(&regFcPoint) != VOS_OK) {
        AT_ERR_LOG("AT_PS_RegNdisFcPoint: ERROR: reg cds point Failed.");
        return VOS_ERR;
    }

    regFcPoint.policyId = FC_POLICY_ID_GPRS;
    if (FC_RegPoint(&regFcPoint) != VOS_OK) {
        AT_ERR_LOG("AT_PS_RegNdisFcPoint: ERROR: reg gprs point Failed.");
        return VOS_ERR;
    }

    /* ����FCID��FC Pri��ӳ���ϵ */
    g_fcIdMaptoFcPri[fcId].used  = VOS_TRUE;
    g_fcIdMaptoFcPri[fcId].fcPri = fCPri;
    g_fcIdMaptoFcPri[fcId].rabIdMask |= ((VOS_UINT32)1 << (regFcInd->rabId));
    g_fcIdMaptoFcPri[fcId].modemId = modemId;

    /* ��������Ϣ */
    AT_MntnTraceRegFcPoint((VOS_UINT8)regFcInd->ctrl.clientId, AT_FC_POINT_TYPE_NDIS);

    return VOS_OK;
}


VOS_UINT32 AT_PS_DeRegNdisFcPoint(const FC_IdUint8 fcId, const ModemIdUint16 modemId,
                                  const TAF_IFACE_DeregFcInd *deRegFcInd)
{
    if ((g_fcIdMaptoFcPri[fcId].rabIdMask & (0x01UL << deRegFcInd->rabId)) == 0) {
        AT_ERR_LOG1("AT_PS_DeRegNdisFcPoint: RabId err, RabIdMask :", g_fcIdMaptoFcPri[fcId].rabIdMask);
        return VOS_ERR;
    }

#if (FEATURE_ACPU_FC_POINT_REG == FEATURE_ON)
    /* �ڵ���FC_DeRegPointǰ,�ȵ���FC_ChannelMapDelete */
    FC_ChannelMapDelete(deRegFcInd->rabId, modemId);
#endif

    if (FC_DeRegPoint(fcId, modemId) != VOS_OK) {
        AT_ERR_LOG("AT_PS_DeRegNdisFcPoint: ERROR: de reg point Failed.");
        return VOS_ERR;
    }

    /* ���FCID��FC Pri��ӳ���ϵ */
    g_fcIdMaptoFcPri[fcId].rabIdMask &= ~((VOS_UINT32)1 << (deRegFcInd->rabId));

    if (g_fcIdMaptoFcPri[fcId].rabIdMask == 0) {
        g_fcIdMaptoFcPri[fcId].used    = VOS_FALSE;
        g_fcIdMaptoFcPri[fcId].fcPri   = FC_PRI_BUTT;
        g_fcIdMaptoFcPri[fcId].modemId = MODEM_ID_BUTT;
    }

    /* ��������Ϣ */
    AT_MntnTraceDeregFcPoint((VOS_UINT8)deRegFcInd->ctrl.clientId, AT_FC_POINT_TYPE_NDIS);

    return VOS_OK;
}


VOS_VOID AT_PS_ProcAppRegFcPoint(const TAF_IFACE_RegFcInd *regFcInd)
{
    AT_FcidMap    fCPriOrg;
    ModemIdUint16 modemId;
    FC_IdUint8    defaultFcId;

    modemId = MODEM_ID_0;

    if (AT_GetModemIdFromClient(regFcInd->ctrl.clientId, &modemId) != VOS_OK) {
        AT_ERR_LOG("AT_PS_ProcAppRegFcPoint: Get modem id fail.");
        return;
    }

    (VOS_VOID)memset_s(&fCPriOrg, sizeof(fCPriOrg), 0x00, sizeof(fCPriOrg));
    defaultFcId = AT_PS_GetFcIdByIfaceId(regFcInd->ifaceId);

    if (AT_GetFcPriFromMap(defaultFcId, &fCPriOrg) == VOS_OK) {
        /* ���FC IDδע�ᣬ��ôע������ص㡣Ŀǰֻ֧��һ������. */
        if ((fCPriOrg.used != VOS_TRUE) ||
            ((g_fcIdMaptoFcPri[defaultFcId].rabIdMask & (0x01UL << regFcInd->rabId)) == 0)) {
            /* ע��APP����ʹ�õ����ص�(Ĭ��ʹ������1) */
            AT_PS_RegAppFcPoint(defaultFcId, modemId, regFcInd);
        } else {
            /* APP����ֻʹ����͵�����QOS���ȼ�FC_PRI_FOR_PDN_LOWEST */
            AT_NORM_LOG("AT_PS_ProcAppRegFcPoint: No need to change the default QOS priority.");
        }
    }

    return;
}


VOS_VOID AT_PS_ProcAppDeRegFcPoint(const TAF_IFACE_DeregFcInd *deRegFcInd)
{
    FC_IdUint8    defaultFcId;
    ModemIdUint16 modemId;

    modemId = MODEM_ID_0;

    if (AT_GetModemIdFromClient(deRegFcInd->ctrl.clientId, &modemId) != VOS_OK) {
        AT_ERR_LOG("AT_PS_ProcAppDeRegFcPoint: Get modem id fail.");
        return;
    }

    defaultFcId = AT_PS_GetFcIdByIfaceId(deRegFcInd->ifaceId);

    if (defaultFcId < FC_ID_BUTT) {
        /* ȥע��APP����ʹ�õ����ص� */
        AT_PS_DeRegAppFcPoint(defaultFcId, modemId, deRegFcInd);
    }

    return;
}


VOS_VOID AT_PS_ProcNdisRegFcPoint(const TAF_IFACE_RegFcInd *regFcInd)
{
    AT_FcidMap      fCPriOrg;
    FC_PriTypeUint8 fCPriCurrent;
    ModemIdUint16   modemId;
    FC_IdUint8      defaultFcId;

    modemId = MODEM_ID_0;

    if (AT_GetModemIdFromClient(regFcInd->ctrl.clientId, &modemId) != VOS_OK) {
        AT_ERR_LOG("AT_PS_ProcNdisRegFcPoint:Get Modem Id fail");
        return;
    }

    defaultFcId = AT_PS_GetFcIdByIfaceId(regFcInd->ifaceId);
    (VOS_VOID)memset_s(&fCPriOrg, sizeof(fCPriOrg), 0x00, sizeof(fCPriOrg));

    if (AT_GetFcPriFromMap(defaultFcId, &fCPriOrg) == VOS_OK) {
        /* ���FC IDδע�ᣬ��ôע������ص㡣Ŀǰֻ֧��һ������. */
        if ((fCPriOrg.used != VOS_TRUE) ||
            ((g_fcIdMaptoFcPri[defaultFcId].rabIdMask & (0x01UL << regFcInd->rabId)) == 0)) {
            /* ע��NDIS�˿ڵ����ص� */
            AT_PS_RegNdisFcPoint(regFcInd, defaultFcId, modemId);
        } else {
            AT_NORM_LOG("AT_PS_ProcNdisRegFcPoint: has already reg");
            fCPriCurrent = FC_PRI_FOR_PDN_NONGBR;

            if (regFcInd->opUmtsQos == TAF_USED) {
                fCPriCurrent = AT_GetFCPriFromQos(&regFcInd->umtsQos);
            }

            /* �����ǰFC���ȼ���֮ǰ���ص�FC���ȼ��ߣ���ô�������ȼ��� */
            if (fCPriCurrent > fCPriOrg.fcPri) {
                AT_PS_ChangeFcPoint(modemId, fCPriCurrent, defaultFcId);
            }
        }
    }

    return;
}


VOS_VOID AT_PS_ProcNdisDeRegFcPoint(TAF_IFACE_DeregFcInd *deRegFcInd)
{
    ModemIdUint16 modemId;
    FC_IdUint8    defaultFcId;

    modemId = MODEM_ID_0;

    if (AT_GetModemIdFromClient(deRegFcInd->ctrl.clientId, &modemId) != VOS_OK) {
        AT_ERR_LOG("AT_PS_ProcNdisDeRegFcPoint:Get Modem Id fail");
        return;
    }

    defaultFcId = AT_PS_GetFcIdByIfaceId(deRegFcInd->ifaceId);

    if (defaultFcId < FC_ID_BUTT) {
        /* ȥע��NDIS�˿ڵ����ص� */
        AT_PS_DeRegNdisFcPoint(defaultFcId, modemId, deRegFcInd);
    }

    return;
}


VOS_VOID AT_PS_SendNdisIPv4IfaceUpCfgInd(const DSM_NDIS_IfaceUpInd *rcvMsg)
{
    errno_t memResult;
    DMS_PortIdUint16 portId;
#if (FEATURE_DATA_SERVICE_NEW_PLATFORM == FEATURE_ON)
    AT_NDIS_IfaceUpConfigInd *ndisCfgUp = VOS_NULL_PTR;

    if (AT_IsNdisIface(rcvMsg->ifaceId) == VOS_FALSE) {
        AT_ERR_LOG1("AT_PS_SendNdisIPv4IfaceUpCfgInd, not NDIS iface", rcvMsg->ifaceId);
        return;
    }

    ndisCfgUp = (AT_NDIS_IfaceUpConfigInd *)AT_ALLOC_MSG_WITH_HDR(sizeof(AT_NDIS_IfaceUpConfigInd));
#else
    AT_NDIS_PdnInfoCfgReq *ndisCfgUp = VOS_NULL_PTR;

    ndisCfgUp = (AT_NDIS_PdnInfoCfgReq *)AT_ALLOC_MSG_WITH_HDR(sizeof(AT_NDIS_PdnInfoCfgReq));
#endif

    if (ndisCfgUp == VOS_NULL_PTR) {
        AT_WARN_LOG("AT_PS_SendNdisIPv4IfaceUpCfgInd: alloc msg fail!");
        return;
    }

    /* ��ʼ����Ϣ */
    memResult = memset_s(AT_GET_MSG_ENTITY(ndisCfgUp), AT_GET_MSG_LENGTH(ndisCfgUp), 0x00,
                         AT_GET_MSG_LENGTH(ndisCfgUp));
    TAF_MEM_CHK_RTN_VAL(memResult, AT_GET_MSG_LENGTH(ndisCfgUp), AT_GET_MSG_LENGTH(ndisCfgUp));

#if (FEATURE_DATA_SERVICE_NEW_PLATFORM == FEATURE_ON)
    /* ��д��Ϣͷ */
    AT_CFG_NDIS_MSG_HDR(ndisCfgUp, ID_AT_NDIS_IFACE_UP_CONFIG_IND);

    /* ��д��Ϣ�� */
    ndisCfgUp->iFaceId                    = rcvMsg->ifaceId;
    ndisCfgUp->ipv4IfaceInfo.ifaceId      = rcvMsg->ifaceId;
    ndisCfgUp->ipv4IfaceInfo.pduSessionId = rcvMsg->ipv4PdnInfo.sessionId;
    ndisCfgUp->ipv4IfaceInfo.fcHead       = AT_GET_IFACE_FC_HEAD_BY_MODEMID(rcvMsg->modemId);
    ndisCfgUp->ipv4IfaceInfo.modemId      = (VOS_UINT8)rcvMsg->modemId;
    portId = DMS_PORT_NCM_DATA + (rcvMsg->ifaceId - PS_IFACE_ID_NDIS0);
#else
    /* ��д��Ϣͷ */
    AT_CFG_NDIS_MSG_HDR(ndisCfgUp, ID_AT_NDIS_PDNINFO_CFG_REQ);

    /* ��д��Ϣ�� */
    ndisCfgUp->rabId = rcvMsg->rabId;
    ndisCfgUp->modemId = rcvMsg->modemId;
    ndisCfgUp->iSpePort = AT_GetCommPsCtxAddr()->spePort;
    ndisCfgUp->ipfFlag = AT_GetCommPsCtxAddr()->ipfPortFlg;
    portId = DMS_PORT_NCM_DATA;
#endif

    ndisCfgUp->opIpv4PdnInfo = VOS_TRUE;
    ndisCfgUp->handle        = DMS_PORT_GetPortHandle(portId);

    /* ��дIPv4��ַ */
    if (rcvMsg->ipv4PdnInfo.opPdnAddr == VOS_TRUE) {
        ndisCfgUp->ipv4PdnInfo.opPdnAddr = VOS_TRUE;
        memResult = memcpy_s(ndisCfgUp->ipv4PdnInfo.PdnAddrInfo.ipv4AddrU8,
                             sizeof(ndisCfgUp->ipv4PdnInfo.PdnAddrInfo.ipv4AddrU8),
                             rcvMsg->ipv4PdnInfo.pdnAddrInfo.ipV4Addr, DSM_NDIS_IPV4_ADDR_LENGTH);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(ndisCfgUp->ipv4PdnInfo.PdnAddrInfo.ipv4AddrU8),
                            DSM_NDIS_IPV4_ADDR_LENGTH);
    }

    /* ��д�����ַ */
    memResult = memcpy_s(ndisCfgUp->ipv4PdnInfo.subnetMask.ipv4AddrU8,
                         sizeof(ndisCfgUp->ipv4PdnInfo.subnetMask.ipv4AddrU8), rcvMsg->ipv4PdnInfo.subnetMask.ipV4Addr,
                         DSM_NDIS_IPV4_ADDR_LENGTH);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(ndisCfgUp->ipv4PdnInfo.subnetMask.ipv4AddrU8), DSM_NDIS_IPV4_ADDR_LENGTH);

    /* ��д���ص�ַ */
    memResult = memcpy_s(ndisCfgUp->ipv4PdnInfo.gateWayAddrInfo.ipv4AddrU8,
                         sizeof(ndisCfgUp->ipv4PdnInfo.gateWayAddrInfo.ipv4AddrU8),
                         rcvMsg->ipv4PdnInfo.gateWayAddrInfo.ipV4Addr, DSM_NDIS_IPV4_ADDR_LENGTH);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(ndisCfgUp->ipv4PdnInfo.gateWayAddrInfo.ipv4AddrU8),
                        DSM_NDIS_IPV4_ADDR_LENGTH);

    /* ��д��DNS��ַ */
    if (rcvMsg->ipv4PdnInfo.opDnsPrim != 0) {
        ndisCfgUp->ipv4PdnInfo.opDnsPrim = VOS_TRUE;
        memResult = memcpy_s(ndisCfgUp->ipv4PdnInfo.dnsPrimAddrInfo.ipv4AddrU8,
                             sizeof(ndisCfgUp->ipv4PdnInfo.dnsPrimAddrInfo.ipv4AddrU8),
                             rcvMsg->ipv4PdnInfo.dnsPrimAddrInfo.ipV4Addr, DSM_NDIS_IPV4_ADDR_LENGTH);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(ndisCfgUp->ipv4PdnInfo.dnsPrimAddrInfo.ipv4AddrU8),
                            DSM_NDIS_IPV4_ADDR_LENGTH);
    }

    /* ��д��DNS��ַ */
    if (rcvMsg->ipv4PdnInfo.opDnsSec != 0) {
        ndisCfgUp->ipv4PdnInfo.opDnsSec = VOS_TRUE;
        memResult = memcpy_s(ndisCfgUp->ipv4PdnInfo.dnsSecAddrInfo.ipv4AddrU8,
                             sizeof(ndisCfgUp->ipv4PdnInfo.dnsSecAddrInfo.ipv4AddrU8),
                             rcvMsg->ipv4PdnInfo.dnsSecAddrInfo.ipV4Addr, DSM_NDIS_IPV4_ADDR_LENGTH);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(ndisCfgUp->ipv4PdnInfo.dnsSecAddrInfo.ipv4AddrU8),
                            DSM_NDIS_IPV4_ADDR_LENGTH);
    }

    if (rcvMsg->ipv4PdnInfo.opPcscfPrim == VOS_TRUE) {
        ndisCfgUp->ipv4PdnInfo.opPcscfPrim = VOS_TRUE;
        memResult = memcpy_s(ndisCfgUp->ipv4PdnInfo.pcscfPrimAddrInfo.ipv4AddrU8,
                             sizeof(ndisCfgUp->ipv4PdnInfo.pcscfPrimAddrInfo.ipv4AddrU8),
                             rcvMsg->ipv4PdnInfo.pcscfPrimAddrInfo.ipV4Addr, DSM_NDIS_IPV4_ADDR_LENGTH);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(ndisCfgUp->ipv4PdnInfo.pcscfPrimAddrInfo.ipv4AddrU8),
                            DSM_NDIS_IPV4_ADDR_LENGTH);
    }

    if (rcvMsg->ipv4PdnInfo.opPcscfSec == VOS_TRUE) {
        ndisCfgUp->ipv4PdnInfo.opPcscfSec = VOS_TRUE;
        memResult = memcpy_s(ndisCfgUp->ipv4PdnInfo.pcscfSecAddrInfo.ipv4AddrU8,
                             sizeof(ndisCfgUp->ipv4PdnInfo.pcscfSecAddrInfo.ipv4AddrU8),
                             rcvMsg->ipv4PdnInfo.pcscfSecAddrInfo.ipV4Addr, DSM_NDIS_IPV4_ADDR_LENGTH);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(ndisCfgUp->ipv4PdnInfo.pcscfSecAddrInfo.ipv4AddrU8),
                            DSM_NDIS_IPV4_ADDR_LENGTH);
    }

    /* ������Ϣ */
    (VOS_VOID)TAF_TraceAndSendMsg(WUEPS_PID_AT, ndisCfgUp);

    return;
}


VOS_VOID AT_PS_SendNdisIPv6IfaceUpCfgInd(const DSM_NDIS_IfaceUpInd *rcvMsg)
{
    errno_t memResult;
    DMS_PortIdUint16 portId;
#if (FEATURE_DATA_SERVICE_NEW_PLATFORM == FEATURE_ON)
    AT_NDIS_IfaceUpConfigInd *ndisCfgUp = VOS_NULL_PTR;

    if (AT_IsNdisIface(rcvMsg->ifaceId) == VOS_FALSE) {
        AT_ERR_LOG1("AT_PS_SendNdisIPv6IfaceUpCfgInd, not NDIS iface", rcvMsg->ifaceId);
        return;
    }

    ndisCfgUp = (AT_NDIS_IfaceUpConfigInd *)AT_ALLOC_MSG_WITH_HDR(sizeof(AT_NDIS_IfaceUpConfigInd));
#else
    AT_NDIS_PdnInfoCfgReq *ndisCfgUp = VOS_NULL_PTR;

    ndisCfgUp = (AT_NDIS_PdnInfoCfgReq *)AT_ALLOC_MSG_WITH_HDR(sizeof(AT_NDIS_PdnInfoCfgReq));
#endif

    if (ndisCfgUp == VOS_NULL_PTR) {
        AT_WARN_LOG("AT_PS_SendNdisIPv6IfaceUpCfgInd: alloc msg fail!");
        return;
    }

    /* ��ʼ����Ϣ */
    memResult = memset_s(AT_GET_MSG_ENTITY(ndisCfgUp), AT_GET_MSG_LENGTH(ndisCfgUp), 0x00,
                         AT_GET_MSG_LENGTH(ndisCfgUp));
    TAF_MEM_CHK_RTN_VAL(memResult, AT_GET_MSG_LENGTH(ndisCfgUp), AT_GET_MSG_LENGTH(ndisCfgUp));

#if (FEATURE_DATA_SERVICE_NEW_PLATFORM == FEATURE_ON)
    /* ��д��Ϣͷ */
    AT_CFG_NDIS_MSG_HDR(ndisCfgUp, ID_AT_NDIS_IFACE_UP_CONFIG_IND);

    /* ��д��Ϣ�� */
    ndisCfgUp->iFaceId                    = rcvMsg->ifaceId;
    ndisCfgUp->ipv6IfaceInfo.ifaceId      = rcvMsg->ifaceId;
    ndisCfgUp->ipv6IfaceInfo.pduSessionId = rcvMsg->ipv6PdnInfo.pduSessionId;
    ndisCfgUp->ipv6IfaceInfo.fcHead       = AT_GET_IFACE_FC_HEAD_BY_MODEMID(rcvMsg->modemId);
    ndisCfgUp->ipv6IfaceInfo.modemId      = (VOS_UINT8)rcvMsg->modemId;
    portId = DMS_PORT_NCM_DATA + (rcvMsg->ifaceId - PS_IFACE_ID_NDIS0);
#else
    /* ��д��Ϣͷ */
    AT_CFG_NDIS_MSG_HDR(ndisCfgUp, ID_AT_NDIS_PDNINFO_CFG_REQ);

    /* ��д��Ϣ�� */
    ndisCfgUp->rabId = rcvMsg->rabId;
    ndisCfgUp->modemId = rcvMsg->modemId;
    ndisCfgUp->iSpePort = AT_GetCommPsCtxAddr()->spePort;
    ndisCfgUp->ipfFlag = AT_GetCommPsCtxAddr()->ipfPortFlg;
    portId = DMS_PORT_NCM_DATA;
#endif

    ndisCfgUp->opIpv6PdnInfo = VOS_TRUE;
    ndisCfgUp->handle        = DMS_PORT_GetPortHandle(portId);

    /* �������DNS */
    ndisCfgUp->ipv6PdnInfo.dnsSer.serNum = 0;
    if (rcvMsg->ipv6PdnInfo.dnsSer.serNum >= 1) {
        memResult = memcpy_s(ndisCfgUp->ipv6PdnInfo.dnsSer.priServer, sizeof(ndisCfgUp->ipv6PdnInfo.dnsSer.priServer),
                             rcvMsg->ipv6PdnInfo.dnsSer.priServer, TAF_IPV6_ADDR_LEN);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(ndisCfgUp->ipv6PdnInfo.dnsSer.priServer), TAF_IPV6_ADDR_LEN);
        ndisCfgUp->ipv6PdnInfo.dnsSer.serNum += 1;
    }

    /* IPV6 DNS��������������2�� */
    if (rcvMsg->ipv6PdnInfo.dnsSer.serNum >= 2) {
        memResult = memcpy_s(ndisCfgUp->ipv6PdnInfo.dnsSer.secServer, sizeof(ndisCfgUp->ipv6PdnInfo.dnsSer.secServer),
                             rcvMsg->ipv6PdnInfo.dnsSer.secServer, TAF_IPV6_ADDR_LEN);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(ndisCfgUp->ipv6PdnInfo.dnsSer.secServer), TAF_IPV6_ADDR_LEN);
        ndisCfgUp->ipv6PdnInfo.dnsSer.serNum += 1;
    }

    /* ���MTU */
    if (rcvMsg->ipv6PdnInfo.bitOpMtu == VOS_TRUE) {
        ndisCfgUp->ipv6PdnInfo.OpMtu = VOS_TRUE;
        ndisCfgUp->ipv6PdnInfo.Mtu   = rcvMsg->ipv6PdnInfo.mtu;
    }

    ndisCfgUp->ipv6PdnInfo.curHopLimit = rcvMsg->ipv6PdnInfo.bitCurHopLimit;
    ndisCfgUp->ipv6PdnInfo.ipv6MValue  = rcvMsg->ipv6PdnInfo.bitM;
    ndisCfgUp->ipv6PdnInfo.ipv6OValue  = rcvMsg->ipv6PdnInfo.bitO;
    ndisCfgUp->ipv6PdnInfo.prefixNum   = rcvMsg->ipv6PdnInfo.prefixNum;
    memResult = memcpy_s(ndisCfgUp->ipv6PdnInfo.prefixList, sizeof(ndisCfgUp->ipv6PdnInfo.prefixList),
                         rcvMsg->ipv6PdnInfo.prefixList, sizeof(TAF_PDP_Ipv6Prefix) * TAF_MAX_PREFIX_NUM_IN_RA);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(ndisCfgUp->ipv6PdnInfo.prefixList),
                        sizeof(TAF_PDP_Ipv6Prefix) * TAF_MAX_PREFIX_NUM_IN_RA);

    /* ��дINTERFACE��ȡIPV6��ַ�ĺ�8�ֽ�����дINTERFACE */
    memResult = memcpy_s(ndisCfgUp->ipv6PdnInfo.interfaceId, sizeof(ndisCfgUp->ipv6PdnInfo.interfaceId),
                         rcvMsg->ipv6PdnInfo.interfaceId, sizeof(VOS_UINT8) * AT_NDIS_IPV6_IFID_LENGTH);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(ndisCfgUp->ipv6PdnInfo.interfaceId),
                        sizeof(VOS_UINT8) * AT_NDIS_IPV6_IFID_LENGTH);

    /* �������PCSCF��ַ  */
    ndisCfgUp->ipv6PdnInfo.pcscfSer.serNum = 0;
    if (rcvMsg->ipv6PdnInfo.pcscfSer.serNum > 0) {
        ndisCfgUp->ipv6PdnInfo.pcscfSer.serNum++;

        memResult = memcpy_s(ndisCfgUp->ipv6PdnInfo.pcscfSer.priServer,
                             sizeof(ndisCfgUp->ipv6PdnInfo.pcscfSer.priServer), rcvMsg->ipv6PdnInfo.pcscfSer.priServer,
                             sizeof(rcvMsg->ipv6PdnInfo.pcscfSer.priServer));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(ndisCfgUp->ipv6PdnInfo.pcscfSer.priServer),
                            sizeof(rcvMsg->ipv6PdnInfo.pcscfSer.priServer));
    }

    if (rcvMsg->ipv6PdnInfo.pcscfSer.serNum > 1) {
        ndisCfgUp->ipv6PdnInfo.pcscfSer.serNum++;

        memResult = memcpy_s(ndisCfgUp->ipv6PdnInfo.pcscfSer.secServer,
                             sizeof(ndisCfgUp->ipv6PdnInfo.pcscfSer.secServer), rcvMsg->ipv6PdnInfo.pcscfSer.secServer,
                             sizeof(rcvMsg->ipv6PdnInfo.pcscfSer.secServer));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(ndisCfgUp->ipv6PdnInfo.pcscfSer.secServer),
                            sizeof(rcvMsg->ipv6PdnInfo.pcscfSer.secServer));
    }

    /* ������Ϣ */
    (VOS_VOID)TAF_TraceAndSendMsg(WUEPS_PID_AT, ndisCfgUp);

    return;
}


VOS_VOID AT_PS_ProcNdisIfaceUpCfg(const DSM_NDIS_IfaceUpInd *rcvMsg)
{
    if (rcvMsg->opIpv4PdnInfo == VOS_TRUE) {
        AT_PS_SendNdisIPv4IfaceUpCfgInd(rcvMsg);
    }

    if (rcvMsg->opIpv6PdnInfo == VOS_TRUE) {
        AT_PS_SendNdisIPv6IfaceUpCfgInd(rcvMsg);
    }

    return;
}


VOS_VOID AT_PS_ProcNdisIfaceDownCfg(const DSM_NDIS_IfaceDownInd *rcvMsg)
{
    errno_t memResult;
#if (FEATURE_DATA_SERVICE_NEW_PLATFORM == FEATURE_ON)
    AT_NDIS_IfaceDownConfigInd *ndisCfgDown = VOS_NULL_PTR;

    ndisCfgDown = (AT_NDIS_IfaceDownConfigInd *)AT_ALLOC_MSG_WITH_HDR(sizeof(AT_NDIS_IfaceDownConfigInd));
#else
    AT_NDIS_PdnInfoRelReq *ndisCfgDown = VOS_NULL_PTR;

    ndisCfgDown = (AT_NDIS_PdnInfoRelReq *)AT_ALLOC_MSG_WITH_HDR(sizeof(AT_NDIS_PdnInfoRelReq));
#endif

    if (ndisCfgDown == VOS_NULL_PTR) {
        AT_WARN_LOG("AT_PS_ProcNdisIfaceDownCfg: alloc msg fail!");
        return;
    }

    /* ��ʼ����Ϣ */
    memResult = memset_s(AT_GET_MSG_ENTITY(ndisCfgDown), AT_GET_MSG_LENGTH(ndisCfgDown), 0x00,
                         AT_GET_MSG_LENGTH(ndisCfgDown));
    TAF_MEM_CHK_RTN_VAL(memResult, AT_GET_MSG_LENGTH(ndisCfgDown), AT_GET_MSG_LENGTH(ndisCfgDown));

#if (FEATURE_DATA_SERVICE_NEW_PLATFORM == FEATURE_ON)
    /* ��д��Ϣͷ */
    AT_CFG_NDIS_MSG_HDR(ndisCfgDown, ID_AT_NDIS_IFACE_DOWN_CONFIG_IND);

    /* ��д��Ϣ�� */
    ndisCfgDown->opIpv4PdnInfo = rcvMsg->opIpv4PdnInfo;
    ndisCfgDown->opIpv6PdnInfo = rcvMsg->opIpv6PdnInfo;
    ndisCfgDown->iFaceId       = rcvMsg->ifaceId;
#else
    /* ��д��Ϣͷ */
    AT_CFG_NDIS_MSG_HDR(ndisCfgDown, ID_AT_NDIS_PDNINFO_REL_REQ);

    /* ��д��Ϣ�� */
    ndisCfgDown->modemId = rcvMsg->modemId;
    ndisCfgDown->rabId = rcvMsg->rabId;
#endif

    /* ������Ϣ */
    (VOS_VOID)TAF_TraceAndSendMsg(WUEPS_PID_AT, ndisCfgDown);

    return;
}


VOS_VOID AT_PS_ProcNdisConfigIpv6Dns(const DSM_NDIS_ConfigIpv6DnsInd *rcvMsg)
{
    VOS_UINT8          *ipv6DnsInfo = VOS_NULL_PTR;
    VOS_UINT32          rslt;
    errno_t             memResult;
    VOS_UINT32          len;
    DMS_PortIdUint16    portId;

    if (AT_IsNdisIface(rcvMsg->ifaceId) == VOS_FALSE) {
        AT_ERR_LOG1("AT_PS_ProcNdisConfigIpv6Dns, not NDIS iface", rcvMsg->ifaceId);
        return;
    }

    portId = DMS_PORT_NCM_DATA + (rcvMsg->ifaceId - PS_IFACE_ID_NDIS0);

    ipv6DnsInfo = (unsigned char *)PS_MEM_ALLOC(WUEPS_PID_AT, DMS_NCM_IPV6_DNS_LEN);

    if (ipv6DnsInfo == VOS_NULL_PTR) {
        AT_ERR_LOG("AT_PS_ProcNdisConfigIpv6Dns:Invalid stIPv6Dns.ipv6_dns_info");
        return;
    }

    (VOS_VOID)memset_s(ipv6DnsInfo, DMS_NCM_IPV6_DNS_LEN, 0x00, DMS_NCM_IPV6_DNS_LEN);

    /* �ϱ��������DNS���ȹ̶�Ϊ32(Primary DNS LEN + Secondary DNS LEN) */
    len = DMS_NCM_IPV6_DNS_LEN;

    /* �����DNS����Ҫ����DRV�Ľӿ��ϱ�DNS��PC */
    if (rcvMsg->opIpv6PriDns == VOS_TRUE) {
        memResult = memcpy_s(ipv6DnsInfo, DMS_NCM_IPV6_DNS_LEN, rcvMsg->ipv6PrimDns, AT_MAX_IPV6_DNS_LEN);
        TAF_MEM_CHK_RTN_VAL(memResult, DMS_NCM_IPV6_DNS_LEN, AT_MAX_IPV6_DNS_LEN);
    }

    if (rcvMsg->opIpv6SecDns == VOS_TRUE) {
        memResult = memcpy_s(ipv6DnsInfo + AT_MAX_IPV6_DNS_LEN, DMS_NCM_IPV6_DNS_LEN - AT_MAX_IPV6_DNS_LEN,
                             rcvMsg->ipv6SecDns, AT_MAX_IPV6_DNS_LEN);
        TAF_MEM_CHK_RTN_VAL(memResult, DMS_NCM_IPV6_DNS_LEN - AT_MAX_IPV6_DNS_LEN, AT_MAX_IPV6_DNS_LEN);
    }

    /* ���õ�������DNS��Ϣ */
    rslt = DMS_PORT_SetIpv6Dns(portId, ipv6DnsInfo, len);
    if (rslt != VOS_OK) {
        AT_ERR_LOG("AT_PS_ProcNdisConfigIpv6Dns, DRV_UDI_IOCTL Fail!");
    }

    /* �ͷ�������ڴ� */
    PS_MEM_FREE(WUEPS_PID_AT, ipv6DnsInfo);
    return;
}


VOS_VOID AT_RcvTafIfaceEvt(TAF_PS_Evt *evt)
{
    MN_PS_EVT_FUNC evtFunc = VOS_NULL_PTR;
    TAF_Ctrl      *ctrl    = VOS_NULL_PTR;
    VOS_UINT32     i;
    VOS_UINT32     result;
    VOS_UINT8      portIndex;

    /* ��ʼ�� */
    ctrl      = (TAF_Ctrl *)(evt->content);
    result    = VOS_ERR;
    portIndex = 0;

    if (At_ClientIdToUserId(ctrl->clientId, &portIndex) == AT_FAILURE) {
        AT_WARN_LOG("AT_RcvTafIfaceEvt: usPortClientId At_ClientIdToUserId FAILURE");
        return;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(portIndex)) {
        /*
         * �㲥IDNEX��������Ϊ�����±�ʹ�ã���Ҫ���¼�����������ϸ�˶ԣ���������Խ�硣
         * Ŀǰû�й㲥�¼�������ϸ�˶ԣ�
         */
        AT_WARN_LOG("AT_RcvTafIfaceEvt: AT_BROADCAST_INDEX,but not Broadcast Event.");
        return;
    }

    /* ���¼�������в��Ҵ����� */
    for (i = 0; i < AT_ARRAY_SIZE(g_atIfaceEvtFuncTbl); i++) {
        if (evt->evtId == g_atIfaceEvtFuncTbl[i].evtId) {
            if (TAF_RunChkMsgLenFunc((const MSG_Header *)evt, g_atIfaceEvtFuncTbl[i].msgLen,
                    g_atIfaceEvtFuncTbl[i].chkFunc) == VOS_FALSE) {
                AT_ERR_LOG("AT_RcvTafIfaceEvt: Check MsgLength Err");
                return;
            }
            /* �¼�IDƥ�� */
            evtFunc = g_atIfaceEvtFuncTbl[i].evtFunc;
            break;
        }
    }

    /* ������������������ */
    if (evtFunc != VOS_NULL_PTR) {
        result = evtFunc(portIndex, (TAF_Ctrl *)evt->content);
    }

    if (result != VOS_OK) {
        AT_ERR_LOG1("AT_RcvTafIfaceEvt: Can not handle this message! <MsgId>", evt->evtId);
    }

    return;
}


VOS_UINT32 AT_RcvTafIfaceEvtIfaceUpCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_IFACE_UpCnf    *ifaceUpCnf  = VOS_NULL_PTR;
    AT_PS_DataChanlCfg *dataChanCfg = VOS_NULL_PTR;
    VOS_UINT32          result;

    result     = AT_OK;
    ifaceUpCnf = (TAF_IFACE_UpCnf *)evtInfo;

#if ((FEATURE_SHARE_APN == FEATURE_ON) && (FEATURE_DATA_SERVICE_NEW_PLATFORM == FEATURE_ON))
    if ((g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_D_PPP_CALL_SET) ||
        (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_PPP_ORG_SET) ||
        (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_D_IP_CALL_SET)) {
        AT_ProcPppDialCnf(ifaceUpCnf->cause, indexNum);
        return VOS_OK;
    }
#endif

    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_NDISDUP_SET) {
        AT_WARN_LOG("AT_RcvTafIfaceEvtIfaceUpCnf : Current Option is not AT_CMD_NDISDUP_SET.");
        return VOS_ERR;
    }

    if (ifaceUpCnf->cause != TAF_PS_CAUSE_SUCCESS) {
        AT_PS_SetPsCallErrCause(indexNum, ifaceUpCnf->cause);

        result = AT_ERROR;

        if (ifaceUpCnf->cause == TAF_PS_CAUSE_PDP_ACTIVATE_LIMIT) {
            result = AT_CME_PDP_ACT_LIMIT;
        }

        if (ifaceUpCnf->cid <= TAF_MAX_CID) {
            dataChanCfg            = AT_PS_GetDataChanlCfg(indexNum, ifaceUpCnf->cid);
            dataChanCfg->portIndex = AT_CLIENT_ID_BUTT;
        }

        AT_WARN_LOG1("AT_RcvTafIfaceEvtIfaceUpCnf: <enCause> is ", ifaceUpCnf->cause);
    }

    /* ��������״̬ */
    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafIfaceEvtIfaceDownCnf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_IFACE_DownCnf *ifaceDownCnf = VOS_NULL_PTR;
    VOS_UINT32         result;

    result       = AT_OK;
    ifaceDownCnf = (TAF_IFACE_DownCnf *)evtInfo;

#if ((FEATURE_SHARE_APN == FEATURE_ON) && (FEATURE_DATA_SERVICE_NEW_PLATFORM == FEATURE_ON))
    if ((AT_PS_GET_CURR_CMD_OPT(indexNum) == AT_CMD_PS_DATA_CALL_END_SET) ||
        (AT_PS_GET_CURR_CMD_OPT(indexNum) == AT_CMD_H_PS_SET)) {
        /* ������ʹ��ԭ��callEnd���� */
        AT_ModemProcCallEndCnfEvent(ifaceDownCnf->cause, indexNum);
        return VOS_OK;
    }
#endif

    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_NDISDUP_SET) {
        AT_WARN_LOG("AT_RcvTafIfaceEvtIfaceDownCnf : Current Option is not AT_CMD_NDISDUP_SET.");
        return VOS_ERR;
    }


    if (ifaceDownCnf->cause != TAF_PS_CAUSE_SUCCESS) {
        AT_WARN_LOG1("AT_RcvTafIfaceEvtIfaceDownCnf: <enCause> is ", ifaceDownCnf->cause);
        result = AT_ERROR;
    }

    /* ��������״̬ */
    AT_STOP_TIMER_CMD_READY(indexNum);
    At_FormatResultData(indexNum, result);

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafIfaceEvtIfaceStatusInd(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_IFACE_StatusInd      *ifaceStatusInd = VOS_NULL_PTR;
    AT_PS_RPT_IFACE_RSLT_FUNC rptIfaceRsltFunc;

    ifaceStatusInd = (TAF_IFACE_StatusInd *)evtInfo;

    if (ifaceStatusInd->status == TAF_IFACE_STATUS_ACT) {
        AT_PS_SetPsCallErrCause(ifaceStatusInd->ctrl.clientId, TAF_PS_CAUSE_SUCCESS);
    }

    if (ifaceStatusInd->status == TAF_IFACE_STATUS_DEACT) {
        AT_PS_SetPsCallErrCause(ifaceStatusInd->ctrl.clientId, ifaceStatusInd->cause);
    }

    /* ���¼�������в��Ҵ����� */
    rptIfaceRsltFunc = AT_PS_GetRptIfaceResultFunc(ifaceStatusInd->userType);

    /* ������������������ */
    if (rptIfaceRsltFunc != VOS_NULL_PTR) {
        rptIfaceRsltFunc(ifaceStatusInd);
    } else {
        AT_ERR_LOG("AT_RcvTafIfaceEvtIfaceStatusInd:ERROR: User type or enStatus is invalid!");
    }

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafIfaceEvtDataChannelStateInd(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_IFACE_DataChannelStateInd *dataChannelStateInd = VOS_NULL_PTR;
    AT_PS_DataChanlCfg            *chanCfg             = VOS_NULL_PTR;
    VOS_UINT8                      cid;

    dataChannelStateInd = (TAF_IFACE_DataChannelStateInd *)evtInfo;
    cid                 = dataChannelStateInd->cid;

    /* ���CID�Ϸ��� */
    if (cid > TAF_MAX_CID) {
        AT_ERR_LOG1("AT_RcvTafIfaceEvtDataChannelStateInd, WARNING, CID error:%d", cid);
        return VOS_ERR;
    }

    chanCfg = AT_PS_GetDataChanlCfg(dataChannelStateInd->ctrl.clientId, cid);

    /* ��������δʹ�� */
    if (chanCfg->used == VOS_FALSE) {
        AT_WARN_LOG("AT_RcvTafIfaceEvtDataChannelStateInd: Channel is not config!");
        AT_CleanDataChannelCfg(chanCfg);
        return VOS_ERR;
    }

    if (dataChannelStateInd->opActFlg == VOS_TRUE) {
        /* ��IFACE�����־���� */
        chanCfg->ifaceActFlg = VOS_TRUE;
        return VOS_OK;
    }

    if (dataChannelStateInd->opCleanFlg == VOS_TRUE) {
        /* ���CID������ͨ����ӳ���ϵ */
        AT_CleanDataChannelCfg(chanCfg);
        return VOS_OK;
    }

    AT_WARN_LOG("AT_RcvTafIfaceEvtDataChannelStateInd: state is not act or clean!");
    return VOS_ERR;
}


VOS_UINT32 AT_RcvTafIfaceEvtUsbNetOperInd(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_IFACE_UsbnetOperInd *usbNetOperInd = VOS_NULL_PTR;

    usbNetOperInd = (TAF_IFACE_UsbnetOperInd *)evtInfo;

    if (usbNetOperInd->opActUsbNet == VOS_TRUE) {
        AT_PS_ActiveUsbNet(usbNetOperInd->ifaceId);
        return VOS_OK;
    }

    if (usbNetOperInd->opDeactUsbNet == VOS_TRUE) {
        AT_PS_DeActiveUsbNet(usbNetOperInd->ifaceId);
        return VOS_OK;
    }

    AT_WARN_LOG("AT_RcvTafIfaceEvtUsbNetOperInd: oper is not act or deact usb net!");
    return VOS_ERR;
}


VOS_UINT32 AT_RcvTafIfaceEvtRegFcInd(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_IFACE_RegFcInd *regFcInd = VOS_NULL_PTR;

    regFcInd = (TAF_IFACE_RegFcInd *)evtInfo;

    if (!AT_PS_IS_RABID_VALID(regFcInd->rabId)) {
        AT_ERR_LOG("AT_RcvTafIfaceEvtRegFcInd:ERROR: RABID is invalid!");
        return VOS_ERR;
    }

    /* APP PS CALL���� */
    if (regFcInd->userType == TAF_IFACE_USER_TYPE_APP) {
        AT_PS_ProcAppRegFcPoint(regFcInd);
        return VOS_OK;
    }

    /* NDIS PS CALL���� */
    if (regFcInd->userType == TAF_IFACE_USER_TYPE_NDIS) {
        AT_PS_ProcNdisRegFcPoint(regFcInd);
        return VOS_OK;
    }

    AT_WARN_LOG("AT_RcvTafIfaceEvtRegFcInd: User Type is not APP or NDIS!");
    return VOS_ERR;
}


VOS_UINT32 AT_RcvTafIfaceEvtDeRegFcInd(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_IFACE_DeregFcInd *deRegFcInd = VOS_NULL_PTR;

    deRegFcInd = (TAF_IFACE_DeregFcInd *)evtInfo;

    if (!AT_PS_IS_RABID_VALID(deRegFcInd->rabId)) {
        AT_ERR_LOG("AT_RcvTafIfaceEvtDeRegFcInd:ERROR: RABID is invalid!");
        return VOS_ERR;
    }

    /* APP PS CALL���� */
    if (deRegFcInd->userType == TAF_IFACE_USER_TYPE_APP) {
        AT_PS_ProcAppDeRegFcPoint(deRegFcInd);
        return VOS_OK;
    }

    /* NDIS PS CALL���� */
    if (deRegFcInd->userType == TAF_IFACE_USER_TYPE_NDIS) {
        AT_PS_ProcNdisDeRegFcPoint(deRegFcInd);
        return VOS_OK;
    }

    AT_WARN_LOG("AT_RcvTafIfaceEvtDeRegFcInd: User Type is not APP or NDIS!");
    return VOS_ERR;
}


VOS_UINT32 AT_RcvTafIfaceEvtRabInfoInd(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_IFACE_RabInfoInd *rabInfoInd = VOS_NULL_PTR;
    AT_ModemPsCtx        *psModemCtx = VOS_NULL_PTR;
    ModemIdUint16         modemId;

    modemId = MODEM_ID_0;

    if (AT_GetModemIdFromClient(indexNum, &modemId) != VOS_OK) {
        AT_ERR_LOG("AT_RcvTafIfaceEvtRabInfoInd: Get modem id fail.");
        return VOS_ERR;
    }

    rabInfoInd = (TAF_IFACE_RabInfoInd *)evtInfo;

    if (!AT_PS_IS_RABID_VALID(rabInfoInd->newRabId)) {
        AT_ERR_LOG("AT_RcvTafIfaceEvtRabInfoInd: New RabId is invalid.");
        return VOS_ERR;
    }

#if (FEATURE_DATA_SERVICE_NEW_PLATFORM == FEATURE_ON)
    if (rabInfoInd->ifaceId >= PS_IFACE_ID_BUTT) {
        AT_ERR_LOG("AT_RcvTafIfaceEvtRabInfoInd: enIfaceId is invalid.");
        return VOS_ERR;
    }
#endif

    psModemCtx = AT_GetModemPsCtxAddrFromClientId(indexNum);

    switch (rabInfoInd->operType) {
        case TAF_IFACE_RAB_OPER_ADD:
            /* ����Ϊ��չRABID ���� modemId + rabId */
            g_atClientTab[indexNum].exPsRabId = AT_BUILD_EXRABID(modemId, rabInfoInd->newRabId);

            if (rabInfoInd->opPdpAddr) {
#if (FEATURE_DATA_SERVICE_NEW_PLATFORM == FEATURE_ON)
                psModemCtx->ipAddrIfaceIdMap[rabInfoInd->ifaceId] = rabInfoInd->ipAddr;
#else
                psModemCtx->ipAddrRabIdMap[rabInfoInd->newRabId - AT_PS_RABID_OFFSET] = rabInfoInd->ipAddr;
#endif
            }
            break;

        case TAF_IFACE_RAB_OPER_DELETE:
            g_atClientTab[indexNum].exPsRabId = 0;
#if (FEATURE_DATA_SERVICE_NEW_PLATFORM == FEATURE_ON)
            if ((rabInfoInd->pdpType & TAF_PDP_IPV4) == TAF_PDP_IPV4) {
                psModemCtx->ipAddrIfaceIdMap[rabInfoInd->ifaceId] = 0;
            }
#else
            psModemCtx->ipAddrRabIdMap[rabInfoInd->newRabId - AT_PS_RABID_OFFSET] = 0;
#endif
            break;

        case TAF_IFACE_RAB_OPER_CHANGE:
            if (!AT_PS_IS_RABID_VALID(rabInfoInd->oldRabId)) {
                AT_ERR_LOG("AT_RcvTafIfaceEvtRabInfoInd: Old RabId is invalid.");
                return VOS_ERR;
            }

            /* ����Ϊ��չRABID ���� modemId + rabId */
            g_atClientTab[indexNum].exPsRabId = AT_BUILD_EXRABID(modemId, rabInfoInd->newRabId);

#if (FEATURE_DATA_SERVICE_NEW_PLATFORM != FEATURE_ON)
            psModemCtx->ipAddrRabIdMap[rabInfoInd->newRabId - AT_PS_RABID_OFFSET] =
                psModemCtx->ipAddrRabIdMap[rabInfoInd->oldRabId - AT_PS_RABID_OFFSET];
            psModemCtx->ipAddrRabIdMap[rabInfoInd->oldRabId - AT_PS_RABID_OFFSET] = 0;
#endif

            break;

        default:
            AT_WARN_LOG("AT_RcvTafIfaceEvtRabInfoInd: enOperType is invalid!");
            break;
    }

    return VOS_OK;
}


VOS_UINT32 AT_RcvTafPsEvtEpdgCtrluNtf(VOS_UINT8 indexNum, TAF_Ctrl *evtInfo)
{
    TAF_PS_EpdgCtrluNtf *epdgCtrluNtf = VOS_NULL_PTR;
    VOS_UINT8            broadCastIndex;

    epdgCtrluNtf = (TAF_PS_EpdgCtrluNtf *)evtInfo;

    if (At_ClientIdToUserBroadCastId(epdgCtrluNtf->ctrl.clientId, &broadCastIndex) != AT_SUCCESS) {
        AT_WARN_LOG("AT_RcvTafPsEvtEpdgCtrluNtf: At_ClientIdToUserBroadCastId is err!");
        return VOS_ERR;
    }

    if (epdgCtrluNtf->epdgCtrlu.opActReq == VOS_TRUE) {
        AT_PS_ReportImsCtrlMsgu(broadCastIndex, AT_IMS_CTRL_MSG_RECEIVE_MODULE_NON_IMSA,
                                (VOS_UINT32)(sizeof(TAF_PS_EpdgActReqInfo)),
                                (VOS_UINT8 *)(&(epdgCtrluNtf->epdgCtrlu.actReqInfo)));
        return VOS_OK;
    }

    if (epdgCtrluNtf->epdgCtrlu.opDeActReq == VOS_TRUE) {
        AT_PS_ReportImsCtrlMsgu(broadCastIndex, AT_IMS_CTRL_MSG_RECEIVE_MODULE_NON_IMSA,
                                (VOS_UINT32)(sizeof(TAF_PS_EpdgDeactReqInfo)),
                                (VOS_UINT8 *)(&(epdgCtrluNtf->epdgCtrlu.deActReqInfo)));
        return VOS_OK;
    }

    AT_WARN_LOG("AT_RcvTafPsEvtEpdgCtrluNtf: not ACT or DEACT REQ!");
    return VOS_ERR;
}


VOS_UINT32 AT_RcvTafIfaceEvtDyamicParaCnf(VOS_UINT8 indexNum, TAF_Ctrl *evt)
{
    MN_PS_EVT_FUNC evtFunc = VOS_NULL_PTR;
    VOS_UINT32     i;
    VOS_UINT32     result;

    /* ���¼�������в��Ҵ����� */
    for (i = 0; i < AT_ARRAY_SIZE(g_atDynamicInfoReportFuncTbl); i++) {
        if (g_atClientTab[indexNum].cmdCurrentOpt == g_atDynamicInfoReportFuncTbl[i].cmdCurrentOpt) {
            /* �¼�IDƥ�� */
            evtFunc = g_atDynamicInfoReportFuncTbl[i].evtFunc;
            break;
        }
    }

    /* ������������������ */
    result = VOS_ERR;
    if (evtFunc != VOS_NULL_PTR) {
        result = evtFunc(indexNum, evt);
    }

    if (result != VOS_OK) {
        AT_ERR_LOG1("AT_RcvTafIfaceEvtDyamicInfoCnf: Can not handle this message! <CmdCurrentOpt>",
                    g_atClientTab[indexNum].cmdCurrentOpt);
    }

    return result;
}


VOS_UINT32 AT_RcvDmsSwitchGwModeEvent(struct MsgCB *msg)
{
    struct DMS_PORT_SubscripEventNotify   *subscripEvent = VOS_NULL_PTR;
    VOS_UINT8                             *systemAppConfig = VOS_NULL_PTR;
    AT_PS_DataChanlCfg                    *chanCfg         = VOS_NULL_PTR;
    TAF_IFACE_Deactive                     ifaceDown;
    TAF_Ctrl                               ctrl;
    VOS_UINT32                             i;

    subscripEvent = (struct DMS_PORT_SubscripEventNotify *)msg;

    if (subscripEvent->clientId != AT_CLIENT_ID_APP) {
        AT_ERR_LOG1("AT_RcvDmsSwitchGwModeEvent clientid uncorrect.", subscripEvent->clientId);
        return VOS_ERR;
    }

    g_hiLinkMode     = AT_HILINK_GATEWAY_MODE;
    systemAppConfig = AT_GetSystemAppConfigAddr();

    if (*systemAppConfig == SYSTEM_APP_WEBUI) {
        for (i = 1; i <= TAF_MAX_CID; i++) {
            chanCfg = AT_PS_GetDataChanlCfg(g_atClientTab[AT_CLIENT_ID_APP].clientId, (VOS_UINT8)i);

            if ((chanCfg->used == VOS_FALSE) || (chanCfg->portIndex >= AT_CLIENT_ID_BUTT)) {
                continue;
            }

            if ((chanCfg->ifaceId >= PS_IFACE_ID_RMNET0) && (chanCfg->ifaceId <= PS_IFACE_ID_RMNET2)) {
                (VOS_VOID)memset_s(&ifaceDown, sizeof(ifaceDown), 0x00, sizeof(ifaceDown));
                (VOS_VOID)memset_s(&ctrl, sizeof(ctrl), 0x00, sizeof(ctrl));

                ifaceDown.ifaceId  = (VOS_UINT8)chanCfg->ifaceId;
                ifaceDown.cause    = TAF_PS_CALL_END_CAUSE_NORMAL;
                ifaceDown.userType = TAF_IFACE_USER_TYPE_APP;

                /* ������ƽṹ�� */
                (VOS_VOID)AT_PS_BuildIfaceCtrl(WUEPS_PID_AT, g_atClientTab[chanCfg->portIndex].clientId, 0,
                                               &ctrl);
                (VOS_VOID)TAF_IFACE_Down(&ctrl, &ifaceDown);
            }
        }
    }

    return VOS_OK;
}

#if (FEATURE_UE_MODE_NR == FEATURE_ON)


VOS_VOID AT_PS_ReportIpChange(VOS_UINT8 idx, TAF_IFACE_IpChangeInd *ipChangeInd)
{
    VOS_UINT16 length;
    VOS_UINT8  ipv6AddrStr[TAF_MAX_IPV6_ADDR_COLON_STR_LEN];

    length = 0;
    (VOS_VOID)memset_s(ipv6AddrStr, sizeof(ipv6AddrStr), 0x00, sizeof(ipv6AddrStr));

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s^IPCHANGE: %d,%d", g_atCrLf, ipChangeInd->cid,
        ipChangeInd->operateType);
    /* <PDP_type> */
    if (ipChangeInd->pdpType == TAF_PDP_IPV4) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_IP].text);
    } else if (ipChangeInd->pdpType == TAF_PDP_IPV6) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%s,", g_atStringTab[AT_STRING_IPV6].text);
    } else {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", g_atStringTab[AT_STRING_IPV4V6].text);
    }

    if ((ipChangeInd->pdpType & TAF_PDP_IPV4) == TAF_PDP_IPV4) {
        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%08X", ipChangeInd->ipv4Addr);
    }

    if ((ipChangeInd->pdpType & TAF_PDP_IPV6) == TAF_PDP_IPV6) {
        /* ת��IPV6��ַ */
        AT_ConvertIpv6AddrToCompressedStr(ipv6AddrStr, ipChangeInd->ipv6Addr, TAF_IPV6_STR_RFC2373_TOKENS);

        length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
            (VOS_CHAR *)g_atSndCodeAddress + length, ",%s", ipv6AddrStr);
    }

    length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
        (VOS_CHAR *)g_atSndCodeAddress + length, "%s", g_atCrLf);

    At_SendResultData(idx, g_atSndCodeAddress, length);
}


VOS_UINT32 AT_RcvTafIfaceEvtIpChangeInd(VOS_UINT8 idx, TAF_Ctrl *evtInfo)
{
    TAF_IFACE_IpChangeInd *ipChangeInd = VOS_NULL_PTR;
    AT_ModemPsCtx         *atPsCtx     = VOS_NULL_PTR;
    ModemIdUint16          modemId;

    modemId = MODEM_ID_0;

    if (AT_GetModemIdFromClient(idx, &modemId) != VOS_OK) {
        AT_ERR_LOG("AT_RcvTafIfaceEvtIpChangeInd: Get modem id fail.");
        return VOS_ERR;
    }

    ipChangeInd = (TAF_IFACE_IpChangeInd *)evtInfo;

    /* ע:����Ϣֻ������ƽ̨ʹ�� */
    if (ipChangeInd->ifaceId >= PS_IFACE_ID_BUTT) {
        AT_ERR_LOG("AT_RcvTafIfaceEvtIpChangeInd: ifaceId is invalid.");
        return VOS_ERR;
    }

    atPsCtx = AT_GetModemPsCtxAddrFromClientId(idx);

    switch (ipChangeInd->operateType) {
        case TAF_IFACE_IP_CHANGE_OPER_ADD:
            /* ���ع�PPP����ʱ���ٿ����Ƿ����g_atClientTab[index].ucExPsRabId */
#if (FEATURE_DATA_SERVICE_NEW_PLATFORM == FEATURE_ON)
            if ((ipChangeInd->pdpType & TAF_PDP_IPV4) == TAF_PDP_IPV4) {
                /* ΪCSDN������������ݰ�ʱʹ�ã�����IPv4���� */
                atPsCtx->ipAddrIfaceIdMap[ipChangeInd->ifaceId] = ipChangeInd->ipv4Addr;
            }
#endif
            AT_PS_ReportIpChange(idx, ipChangeInd);
            break;

        case TAF_IFACE_IP_CHANGE_OPER_DELETE:
            AT_PS_ReportIpChange(idx, ipChangeInd);
            break;

        case TAF_IFACE_IP_CHANGE_OPER_CHANGE:
            /* CHANGE���͵��ݲ�֧�֣�������MHV6ʱʵ�� */
        default:
            AT_WARN_LOG("AT_RcvTafIfaceEvtIpChangeInd: operateType is invalid!");
            break;
    }

    return VOS_OK;
}

#endif

