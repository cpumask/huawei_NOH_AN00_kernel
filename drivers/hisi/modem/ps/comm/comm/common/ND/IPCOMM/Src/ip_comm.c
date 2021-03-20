/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:
 * Create: 2012/10/30
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 and
 *  only version 2 as published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the following license terms apply:
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *  1) Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2) Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3) Neither the name of Huawei nor the names of its contributors may
 *     be used to endorse or promote products derived from this software
 *     without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
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
 * 1 Include HeadFile
 */

#include "ip_comm.h"
#include "securec.h"
#include "mdrv.h"
#if (FEATURE_ON == FEATURE_LTE)
#include "msp_diag.h"
#endif
#include "ps_tag.h"
#include "ps_iface_global_def.h"
#include "ip_ipm_global.h"


#define THIS_MODU ps_nd
/*lint -e767*/
#define THIS_FILE_ID PS_FILE_ID_IPCOMM_C /* ���޸� */
/*lint +e767*/

/*
 * 2 Declare the Global Variable
 */

VOS_UINT8 g_ndAllNodesMulticaseAddr[IP_IPV6_ADDR_LEN] = {
    0xff, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01
};
VOS_UINT8 g_ndAllRoutersMulticaseAddr[IP_IPV6_ADDR_LEN] = {
    0xff, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02
};

VOS_UINT8 g_ndAllNodesMacAddr[IP_MAC_ADDR_LEN]   = { 0x33, 0x33, 0x00, 0x00, 0x00, 0x01 };
VOS_UINT8 g_ndAllRoutersMacAddr[IP_MAC_ADDR_LEN] = { 0x33, 0x33, 0x00, 0x00, 0x00, 0x02 };


/*
 * ��������: ����link-local��ַ
 * �޸���ʷ:
 *  1.��    ��: 2011��4��7��
 *    �޸�����: �����ɺ���
 *  2.��    ��: 2013��1��10��
 *    �޸�����: DSDA��������޸ġ�EpsIdΪ��2bitΪModemID����6bitΪEpsbId��
 */
VOS_UINT32 ND_CheckEpsIdValid(VOS_UINT8 exEpsId)
{
    VOS_UINT8  epsId;
    VOS_UINT16 modemId;

    modemId = ND_GET_MODEMID_FROM_EXEPSID(exEpsId);
    if (modemId >= MODEM_ID_BUTT) {
        return PS_FAIL;
    }

    epsId = ND_GET_EPSID_FROM_EXEPSID(exEpsId);
    if ((epsId < ND_MIN_EPSB_ID) || (epsId > ND_MAX_EPSB_ID)) {
        return IP_FAIL;
    }

    return IP_SUCC;
}

/*
 * ��������: ����link-local��ַ
 * �޸���ʷ:
 *  1.��    ��: 2011��4��7��
 *    �޸�����: �����ɺ���
 */
VOS_VOID IP_BuildLinkLocalAddr(VOS_UINT8 *linkLocalAddr, const VOS_UINT8 *ifId)
{
    errno_t rlt;
    /* ����link-local address */
    IP_SetUint16Data(linkLocalAddr, IP_IPV6_LINK_LOCAL_PREFIX);
    rlt = memcpy_s(&linkLocalAddr[ND_IPV6_IF_OFFSET], ND_IPV6_IF_LEN, ifId, ND_IPV6_IF_LEN);
    IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);
}

/*
 * ��������: ΪICMPv6��Ϣ����IP header����֧����չͷ
 * �޸���ʷ:
 *  1.��    ��: 2011��4��7��
 *    �޸�����: �����ɺ���
 */
VOS_VOID IP_BuildIPv6Header(VOS_UINT8 *ipv6, IP_Ipv6PseduoHeader *pseduoHeader,
                            const VOS_UINT8 *interfaceId, VOS_UINT32 upperLength)
{
    VOS_UINT8  linkLocalAddr[IP_IPV6_ADDR_LEN] = {0};
    VOS_UINT8 *linkLocalAddrCalc               = &linkLocalAddr[0];
    errno_t    rlt;
    /* ����link-local address */
    IP_SetUint16Data(linkLocalAddrCalc, IP_IPV6_LINK_LOCAL_PREFIX);
    rlt = memcpy_s(&linkLocalAddr[ND_IPV6_IF_OFFSET], ND_IPV6_IF_LEN, interfaceId, ND_IPV6_IF_LEN);
    IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);

    /* ����IP�汾�� */
    *ipv6 = IP_VERSION_6 << 4; /* 4:version */

    /* ����Payload��û����չͷ */
    IP_SetUint16Data(ipv6 + IP_IPV6_BASIC_HEAD_PAYLOAD_OFFSET, upperLength);

    /* ����next header */
    *(ipv6 + IP_IPV6_BASIC_HEAD_NEXT_HEAD_OFFSET) = IP_HEAD_PROTOCOL_ICMPV6;

    /* Hop Limit 255 */
    *(ipv6 + IP_IPV6_BASIC_HOP_LIMIT_OFFSET) = IP_IPV6_ND_HOP_LIMIT;

    rlt = memcpy_s(ipv6 + IP_IPV6_SRC_ADDR_OFFSET, IP_IPV6_ADDR_LEN, linkLocalAddr, IP_IPV6_ADDR_LEN);
    IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);

    rlt = memcpy_s(ipv6 + IP_IPV6_DST_ADDR_OFFSET, IP_IPV6_ADDR_LEN, g_ndAllRoutersMulticaseAddr,
                   IP_IPV6_ADDR_LEN);
    IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);

    /* Ϊ����checksum����pseudo-header */
    rlt = memcpy_s(pseduoHeader->ipSrc, IP_IPV6_ADDR_LEN, linkLocalAddr, IP_IPV6_ADDR_LEN);
    IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);

    rlt = memcpy_s(pseduoHeader->ipDes, IP_IPV6_ADDR_LEN, g_ndAllRoutersMulticaseAddr, IP_IPV6_ADDR_LEN);
    IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);
    IP_SetUint32Data((VOS_UINT8 *)&pseduoHeader->upperLength, upperLength);
    pseduoHeader->nextHead = IP_HEAD_PROTOCOL_ICMPV6;
}

/*
 * Function Name: IP_ND_FormIPv6HeaderMsg
 * Description: IPv6��ͷ����
 * Input: aucSrcIPAddr --- ԴIP��ַ
 *                   aucDstIPAddr --- Ŀ��IP��ַ
 *                   ulPduLen     --- ��Ч���س���
 * Output: pucData --- ���Ļ���ָ��
 *    1.      2011-04-07  Draft Enact
 */
/*lint -e438*/
VOS_VOID IP_ND_FormIPv6HeaderMsg(const ND_Ipv6AddrInfo srcIPAddr, const ND_Ipv6AddrInfo dstIPAddr,
                                 VOS_UINT32 pduLen, VOS_UINT8 *data, VOS_UINT8 type)
{
    errno_t rlt;
    /* �汾 */
    *data = (VOS_UINT8)((IP_VERSION_6 << IP_MOVEMENT_4_BITS) & IP_IPDATA_HIGH_4_BIT_MASK);
    data++;

    /* 3:�������͡�����ǩ */
    data += 3;

    /* ��Ч���س��� */
    IP_SetUint16Data(data, (VOS_UINT16)pduLen);
    data += 2; /* 2:len */

    /* ��һ��ͷ */
    *data = type;
    data++;

    /* ������ */
    *data = IP_IPV6_ND_HOP_LIMIT;
    data++;

    /* Դ��ַ */
    rlt = memcpy_s(data, IP_IPV6_ADDR_LEN, srcIPAddr.ipv6Addr, IP_IPV6_ADDR_LEN);
    IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);

    data += IP_IPV6_ADDR_LEN;

    /* Ŀ�ĵ�ַ */
    rlt = memcpy_s(data, IP_IPV6_ADDR_LEN, dstIPAddr.ipv6Addr, IP_IPV6_ADDR_LEN);
    IP_CHK_SEC_RETURN_NO_VAL(rlt != EOK);
}
/*lint +e438*/
/*
 * ��������: RA�Ϸ��Լ��
 * �޸���ʷ:
 *  1.��    ��: 2011��4��7��
 *    �޸�����: �����ɺ���
 */
IP_BoolUint8 IP_IsValidRAPacket(const VOS_UINT8 *ipMsg, VOS_UINT32 typeOffset)
{
    VOS_UINT32 tmp;

    IP_ASSERT_RTN(ipMsg != VOS_NULL_PTR, IP_FALSE);

    /* link-local source address */
    IP_GetUint16Data(tmp, ipMsg + IP_IPV6_SRC_ADDR_OFFSET);
    if ((tmp & IP_IPV6_LINK_LOCAL_MASK) != IP_IPV6_LINK_LOCAL_PREFIX) {
        IPND_ERROR_LOG1(ND_TASK_PID, "IP_IsValidRAPacket: Illegal source address:!", tmp);
        return IP_FALSE;
    }

    /* Hop Limit 255 */
    tmp = *(ipMsg + IP_IPV6_BASIC_HOP_LIMIT_OFFSET);
    if (tmp != IP_IPV6_ND_HOP_LIMIT) {
        IPND_ERROR_LOG1(ND_TASK_PID, "IP_IsValidRAPacket: Illegal Hop Limit:!", tmp);
        return IP_FALSE;
    }

    /* CODE 0 */
    tmp = *(ipMsg + typeOffset + IP_ICMPV6_CODE_OFFSET);
    if (tmp != IP_IPV6_ND_VALID_CODE) {
        IPND_ERROR_LOG1(ND_TASK_PID, "IP_IsValidRAPacket: Illegal ICMPv6 CODE:!", tmp);
        return IP_FALSE;
    }

    return IP_TRUE;
}

/*  ipv6 begin */
/*
 * Function Name: IP_GetIpv6UpLayerProtocol
 * Description: ��ȡIPv6�ϲ�Э��
 * Input: pucIpMsg---------------------IPv6���ݰ��׵�ַ
 *                   usPayLoad--------------------IPv6���ݰ�payload����
 * Output: pucNextHeader----------------��һͷ����ָ��
 *                   pulDecodedLen----------------�����볤��ָ��
 * Return: IP_ErrUint2
 *    1.      2011-03-25  Draft Enact
 */
STATIC IP_ErrUint2 IP_GetIpv6UpLayerProtocol(const VOS_UINT8 *ipMsg, VOS_UINT16 payLoad, VOS_UINT8 *nextHeader,
                                             VOS_UINT32 *decodedLen)
{
    const VOS_UINT8 *ipMsgTemp      = ipMsg;
    VOS_UINT8        nextHeaderCalc;
    VOS_UINT8        extHeaderLen;
    VOS_UINT32       decodeLen      = IP_NULL;

    IP_ASSERT_RTN(ipMsg != VOS_NULL_PTR, IP_FAIL);
    IP_ASSERT_RTN(nextHeader != VOS_NULL_PTR, IP_FAIL);
    IP_ASSERT_RTN(decodedLen != VOS_NULL_PTR, IP_FAIL);

    /* ��ȡnextheader */
    nextHeaderCalc = *(ipMsgTemp + IP_IPV6_BASIC_HEAD_NEXT_HEAD_OFFSET);

    ipMsgTemp = ipMsgTemp + IP_IPV6_HEAD_LEN;

    /* �����������ݰ�����С��ulIpMsgLen */
    while (decodeLen <= payLoad) {
        switch (nextHeaderCalc) {
            case IP_EXTENSION_HEADER_TYPE_FRAGMENT /* �ֶ�ͷ */:
            case IP_EXTENSION_HEADER_TYPE_AH /* AHͷ */:
            case IP_EXTENSION_HEADER_TYPE_ESP /* ESPͷ */:
                *nextHeader = nextHeaderCalc;
                *decodedLen = decodeLen + IP_IPV6_HEAD_LEN;
                return IP_SUCC;
            case IP_EXTENSION_HEADER_TYPE_HOPBYHOP /* ����ѡ��ͷ */:
            case IP_EXTENSION_HEADER_TYPE_DESTINATION /* Ŀ�ĵ�ѡ��ͷ */:
            case IP_EXTENSION_HEADER_TYPE_ROUTING /* ѡ·ͷ */:
                if (decodeLen + IP_IPV6_EXT_HEAD_LEN_OFFSET >= payLoad) {
                    IPND_WARNING_LOG(ND_TASK_PID, "IP_GetIpv6UpLayerProtocol: PayLoad Lenth is Error!");
                    return IP_FAIL;
                }

                /* ��ǰ��չͷ�е���һ�� */
                nextHeaderCalc = *(ipMsgTemp + IP_IPV6_EXT_HEAD_NEXT_HEAD_OFFSET);

                /* ��ȡ��չͷ�еĳ�����Ϣ����������չͷ */
                extHeaderLen = *(ipMsgTemp + IP_IPV6_EXT_HEAD_LEN_OFFSET);

                ipMsgTemp = ipMsgTemp + IP_GetExtensionLen(extHeaderLen);
                decodeLen  = decodeLen + IP_GetExtensionLen(extHeaderLen);
                if (payLoad < decodeLen) {
                    IPND_WARNING_LOG(ND_TASK_PID, "IP_GetIpv6UpLayerProtocol: Lenth Error!");
                    return IP_FAIL;
                }

                break;
            default:
                *nextHeader = nextHeaderCalc;
                *decodedLen = decodeLen + IP_IPV6_HEAD_LEN;
                return IP_SUCC;
        }
    }

    return IP_FAIL;
}

/*
 * Function Name: IP_AffirmNdParam
 * Description: ȷ��ND���Ĳ�������Ч��
 * Input: pucIpMsg ----------------  ����������ָ��
 *                   ulTypeOffset ------------  ICMPv6��ͷƫ����
 * Return: IP_ErrUint2
 *    1.      2011-04-12  Draft Enact
 */
STATIC IP_ErrUint2 IP_AffirmNdParam(const VOS_UINT8 *ipMsg, VOS_UINT32 typeOffset)
{
    VOS_UINT32 tmp;

    IP_ASSERT_RTN(ipMsg != VOS_NULL_PTR, PS_FALSE);

    /* Hop Limit 255 */
    tmp = *(ipMsg + IP_IPV6_BASIC_HOP_LIMIT_OFFSET);

    if (tmp != IP_IPV6_ND_HOP_LIMIT) {
        IPND_ERROR_LOG1(ND_TASK_PID, "IP_AffirmNdParam: Illegal Hop Limit:!", tmp);
        return IP_FAIL;
    }

    /* CODE 0 */
    tmp = *(ipMsg + typeOffset + IP_ICMPV6_CODE_OFFSET);
    if (tmp != IP_IPV6_ND_VALID_CODE) {
        IPND_INFO_LOG1(ND_TASK_PID, "IP_AffirmNdParam: Illegal ICMPv6 CODE:!", tmp);
        return IP_FAIL;
    }

    return IP_SUCC;
}

/*
 * Function Name: IP_IsIcmpv6Packet
 * Description: �ж��Ƿ�ΪICMPv6��
 * Input: pucIpMsg---------------------IP���ݰ��׵�ַ
 *                   ulIpMsgLen-------------------IP���ݰ�����
 * Output: pulDecodedLen----------------�����볤��ָ��
 * Return: IP_BoolUint8
 * History:
 *    1.      2011-03-25  Draft Enact
 */
VOS_UINT32 g_ipCommcnNd = 0;

STATIC IP_BoolUint8 IP_IsIcmpv6Packet(const VOS_UINT8 *ipMsg, VOS_UINT32 ipMsgLen, VOS_UINT32 *decodedLen)
{
    VOS_UINT8          protocol       = IP_NULL;
    VOS_UINT8          ipVersion;
    IP_ErrUint2        rslt;
    VOS_UINT32         decodedLenCalc = IP_NULL;
    VOS_UINT16         payLoad        = IP_NULL;

    IP_ASSERT_RTN(ipMsg != VOS_NULL_PTR, IP_FALSE);
    IP_ASSERT_RTN(decodedLen != VOS_NULL_PTR, IP_FALSE);

    /* ��ȡIP�汾�� */
    if (ipMsgLen == 0) {
        IPND_INFO_LOG(ND_TASK_PID, "\nIP_IsIcmpv6Packet: MsgLen Is ZERO!");
        return IP_FALSE;
    }

    ipVersion = IP_GetIpVersion(ipMsg);
    /* ����汾�Ų���IPV6������ICMPv6�� */
    if (ipVersion != IP_VERSION_6) {
        IPND_INFO_LOG1(ND_TASK_PID, "IP_IsIcmpv6Packet: IpVersion:", ipVersion);
        return IP_FALSE;
    }

    /* ��ȡPAYLOAD */
    if (ipMsgLen <= (IP_IPV6_BASIC_HEAD_PAYLOAD_OFFSET + 1)) {
        IPND_INFO_LOG(ND_TASK_PID, "\nIP_IsIcmpv6Packet: MsgLen Is Less Than IP_IPV6_BASIC_HEAD_PAYLOAD_OFFSET + 1!");
        return IP_FALSE;
    }
    IP_GetUint16Data(payLoad, ipMsg + IP_IPV6_BASIC_HEAD_PAYLOAD_OFFSET);

    /* ���ȺϷ���� */
    if (((VOS_UINT32)payLoad + IP_IPV6_HEAD_LEN) > ipMsgLen) {
        if (g_ipCommcnNd == 1) {
            PS_PRINTF_INFO("\n***********IP_IsIcmpv6Packet: %d %d***********\n", payLoad + IP_IPV6_HEAD_LEN,
                           ipMsgLen);

            PS_PRINTF_INFO("\n**********************\n\n");
        }
        IPND_WARNING_LOG2(ND_TASK_PID, "IP_IsIcmpv6Packet: PayLoad beyond the msg::!", (payLoad + IP_IPV6_HEAD_LEN),
                          ipMsgLen);
        return IP_FALSE;
    }

    /* ��ȡ�ϲ�Э��� */
    rslt = IP_GetIpv6UpLayerProtocol(ipMsg, payLoad, &protocol, &decodedLenCalc);
    if (rslt != IP_SUCC) {
        if (g_ipCommcnNd == 1) {
            PS_PRINTF_INFO("\n***********IP_IsIcmpv6Packet: %d %d***********\n", payLoad + IP_IPV6_HEAD_LEN,
                           ipMsgLen);

            PS_PRINTF_INFO("\n**********************\n\n");
        }
        IPND_WARNING_LOG(ND_TASK_PID, "IP_IsIcmpv6Packet: Fail to get upper-layer protocol!");
        return IP_FALSE;
    }

    /* �ж��ϲ�Э����Ƿ�ΪIcmpv6 */
    if (protocol != IP_HEAD_PROTOCOL_ICMPV6) {
        IPND_INFO_LOG1(ND_TASK_PID, "IP_IsIcmpv6Packet: Protocol:", protocol);
        return IP_FALSE;
    }

    *decodedLen = decodedLenCalc;
    return IP_TRUE;
}

/*
 * ��������: Ϊ����checksum����pseudo-header
 * �޸���ʷ:
 *  1.��    ��: 2011��4��7��
 *    �޸�����: �����ɺ���
 */
STATIC VOS_UINT32 IP_ConstructICMPv6PseudoHeader(const VOS_UINT8 *msgData, VOS_UINT32 typeOffset,
                                                 IP_Ipv6PseduoHeader *pseduoHeader)
{
    VOS_UINT32 length;
    VOS_UINT32 lengthTmp;
    errno_t    rlt;
    IP_ASSERT_RTN(msgData != VOS_NULL_PTR, 0);
    IP_ASSERT_RTN(pseduoHeader != VOS_NULL_PTR, 0);

    rlt = memcpy_s(pseduoHeader->ipSrc, IP_IPV6_ADDR_LEN, msgData + IP_IPV6_SRC_ADDR_OFFSET,
                   IP_IPV6_ADDR_LEN);
    IP_CHK_SEC_RETURN_VAL(rlt != EOK, 0);

    rlt = memcpy_s(pseduoHeader->ipDes, IP_IPV6_ADDR_LEN, msgData + IP_IPV6_DST_ADDR_OFFSET,
                   IP_IPV6_ADDR_LEN);
    IP_CHK_SEC_RETURN_VAL(rlt != EOK, 0);

    pseduoHeader->nextHead = IP_HEAD_PROTOCOL_ICMPV6;

    /* ��ȡPAYLOAD */
    IP_GetUint16Data(length, msgData + IP_IPV6_BASIC_HEAD_PAYLOAD_OFFSET);

    lengthTmp = (typeOffset - IP_IPV6_HEAD_LEN);
    if (length > lengthTmp) {
        length -= lengthTmp;
        IP_SetUint32Data((VOS_UINT8 *)&pseduoHeader->upperLength, length);
    } else {
        length = 0;
    }

    return length;
}

/*
 * ��������: ����16bit SUM
 * �޸���ʷ:
 *  1.��    ��: 2011��4��11��
 *    �޸�����: �����ɺ���
 */
STATIC VOS_UINT32 IP_Unit16Sum(const VOS_UINT8 *data, VOS_UINT32 len)
{
    VOS_UINT32  checkSum = 0;
    VOS_UINT16 *pBuffer  = VOS_NULL_PTR;

    IP_ASSERT_RTN(data != VOS_NULL_PTR, 0);
    IP_ASSERT_RTN(len != 0, 0);

    pBuffer = (VOS_UINT16 *)(VOS_VOID *)data;

    while (len > 1) {
        checkSum += *(pBuffer++);
        len -= 2; /* 2:len */
    }

    if (len != 0) {
        checkSum += *(VOS_UINT8 *)pBuffer;
    }

    return checkSum;
}

/*
 * ��������: ����pseduo-header��upper-layer protocal����checksum
 * �޸���ʷ:
 *  1.��    ��: 2011��4��7��
 *    �޸�����: �����ɺ���
 */
VOS_UINT16 IPv6_Checksum(const VOS_UINT8 *pseduoHeader, const VOS_UINT8 *data, VOS_UINT32 len)
{
    VOS_UINT32 checkSum = 0;

    IP_ASSERT_RTN(pseduoHeader != VOS_NULL_PTR, 0);
    IP_ASSERT_RTN(data != VOS_NULL_PTR, 0);
    IP_ASSERT_RTN(len != 0, 0);

    /* pseduo-header��� */
    checkSum += IP_Unit16Sum(pseduoHeader, sizeof(IP_Ipv6PseduoHeader));

    /* IPV6 Upper-layer package��� */
    checkSum += IP_Unit16Sum(data, len);

    while (checkSum >> 16) { /* 16:len */
        checkSum = (checkSum >> 16) + (checkSum & 0xffff); /* 16:len */
    }

    return (VOS_UINT16)(~checkSum);
}

/*
 * ��������: У��checksum
 * �޸���ʷ:
 *  1.��    ��: 2011��4��7��
 *    �޸�����: �����ɺ���
 */
STATIC IP_ErrUint2 IP_VerifyICMPv6(const VOS_UINT8 *ipMsg, VOS_UINT32 typeOffset)
{
    IP_Ipv6PseduoHeader pseduoHeader = {{IP_NULL}};
    VOS_UINT32          len;

    if ((ipMsg == VOS_NULL_PTR) || (typeOffset < IP_IPV6_HEAD_LEN)) {
        IPND_ERROR_LOG(ND_TASK_PID, "IP_VerifyICMPv6: Illegal parameter!");
        return IP_FAIL;
    }

    /* �ж��Ƿ���Ҫ���飬��û��Я��checksum����Ϊ�Ƿ������� */
    if ((*(ipMsg + typeOffset + IP_ICMPV6_CHECKSUM_OFFSET) == 0) &&
        (*(ipMsg + typeOffset + IP_ICMPV6_CHECKSUM_OFFSET + 1) == 0)) {
        IPND_WARNING_LOG(ND_TASK_PID, "IP_VerifyICMPv6: NONE CHECKSUM!");
        return IP_FAIL;
    }

    /* ����pseduo-header */
    if ((len = IP_ConstructICMPv6PseudoHeader(ipMsg, typeOffset, &pseduoHeader)) == 0) {
        return IP_FAIL;
    }

    /* У��ICMPv6�� */
    if (IPv6_Checksum((VOS_UINT8 *)(VOS_UINT_PTR)(&pseduoHeader), ipMsg + typeOffset, len) != 0) {
        IPND_WARNING_LOG(ND_TASK_PID, "IP_VerifyICMPv6: CHECKSUM Error!");
        return IP_FAIL;
    }

    return IP_SUCC;
}

/*
 * ��������: ����Ƿ��ǺϷ���ND package
 * �޸���ʷ:
 *  1.��    ��: 2011��4��7��
 *    �޸�����: �����ɺ���
 */
IP_BoolUint8 IP_IsValidNdMsg(const VOS_UINT8 *ipMsg, VOS_UINT32 ipMsgLen, VOS_UINT32 *typeOffset)
{
    VOS_UINT32 decodedLen;
    VOS_UINT8  icmpv6Type;

    IP_ASSERT_RTN(ipMsg != VOS_NULL_PTR, IP_FALSE);
    IP_ASSERT_RTN(typeOffset != VOS_NULL_PTR, IP_FALSE);

    /* �ж��Ƿ�ΪICMPv6�� */
    if (IP_IsIcmpv6Packet(ipMsg, ipMsgLen, &decodedLen) != IP_TRUE) {
        IPND_ERROR_LOG(ND_TASK_PID, "IP_IsValidNdMsg: Not Icmpv6 Packet");
        return IP_FALSE;
    }

    /* ���Ȳ�����Я��checksum����Ϊ�Ƿ������� */
    if (decodedLen + IP_ICMPV6_CHECKSUM_OFFSET + 1 >= ipMsgLen) {
        IPND_WARNING_LOG(ND_TASK_PID, "IP_VerifyICMPv6: NONE CHECKSUM!");
        return IP_FAIL;
    }

    if (IP_VerifyICMPv6(ipMsg, decodedLen) != IP_SUCC) {
        IPND_ERROR_LOG(ND_TASK_PID, "IP_IsValidNdMsg: Verify Fail");
        return IP_FALSE;
    }

    /* ȡICMPV6��Ϣ�е�TYPE�ֶ� */
    icmpv6Type = *(ipMsg + decodedLen);
    if (icmpv6Type != IP_ICMPV6_TYPE_ECHOREQUEST) {
        if (IP_SUCC != IP_AffirmNdParam(ipMsg, decodedLen)) {
            IPND_ERROR_LOG(ND_TASK_PID, "IP_IsValidNdMsg: ND Param Affirm Fail");
            return IP_FALSE;
        }
    }

    /* TYPE�Ƿ�λ��[128,137] */
    if ((icmpv6Type >= IP_ICMPV6_TYPE_ECHOREQUEST) && (icmpv6Type <= IP_ICMPV6_TYPE_REDIRECT)) {
        *typeOffset = decodedLen;
        return IP_TRUE;
    }

    IPND_ERROR_LOG1(ND_TASK_PID, "IP_IsValidNdMsg: Invalid ICMPv6 Msg:!", icmpv6Type);

    return IP_FALSE;
}

/*
 * Function Name: IP_BuildIcmpv6Checksum
 * Description: ����ICMPv6��ͷ��У���
 * Input: pucIpMsg ----------------  ����������ָ��
 *                   ulTypeOffset ------------  ICMPv6��ͷƫ����
 * Output: pucIpMsg ----------------  ��������ICMPv6��ͷλ�����У���
 * Return: IP_ErrUint2
 *    1.      2011-04-12  Draft Enact
 */
IP_ErrUint2 IP_BuildIcmpv6Checksum(VOS_UINT8 *ipMsg, VOS_UINT32 typeOffset)
{
    IP_Ipv6PseduoHeader pseduoHeader = {{IP_NULL}};
    VOS_UINT32          len;
    VOS_UINT16          checkSum;

    if ((ipMsg == VOS_NULL_PTR) || (typeOffset < IP_IPV6_HEAD_LEN)) {
        IPND_ERROR_LOG(ND_TASK_PID, "IP_BuildICMPv6Checksum: Illegal parameter!\n");
        return IP_FAIL;
    }

    /*lint -e679 -esym(679,*)*/
    /* ���ICMPv6У����ֶ� */
    ipMsg[typeOffset + IP_ICMPV6_CHECKSUM_OFFSET]     = 0;
    ipMsg[typeOffset + IP_ICMPV6_CHECKSUM_OFFSET + 1] = 0;
    /*lint -e679 +esym(679,*)*/

    /* ����pseduo-header */
    len = IP_ConstructICMPv6PseudoHeader(ipMsg, typeOffset, &pseduoHeader);
    if (len == 0) {
        return IP_FAIL;
    }

    /* ����ICMPv6��У��� */
    checkSum = IPv6_Checksum((VOS_UINT8 *)(VOS_UINT_PTR)(&pseduoHeader), ipMsg + typeOffset, len);

    /*lint -e679 -esym(679,*)*/
    *(VOS_UINT16 *)(VOS_VOID *)(&ipMsg[typeOffset + IP_ICMPV6_CHECKSUM_OFFSET]) = checkSum;
    /*lint -e679 +esym(679,*)*/

    return IP_SUCC;
}


/*
 * �޸���ʷ:
 * 1.��    ��: 2011��6��19��
 */
STATIC VOS_UINT32 TTF_NDIS_IP6_Checksum_Update(const void *pbuf, VOS_UINT32 bytes)
{
    VOS_UINT32  sum;
    VOS_UINT32  nwords;
    VOS_UINT16 *buf = VOS_NULL_PTR;

    buf   = (VOS_UINT16 *)pbuf;
    sum = 0;

    for (nwords = (VOS_UINT32)bytes >> 5; nwords > 0; nwords--) { /* 5:bit */
        sum += *buf++;
        sum += *buf++;
        sum += *buf++;
        sum += *buf++;
        sum += *buf++;
        sum += *buf++;
        sum += *buf++;
        sum += *buf++;
        sum += *buf++;
        sum += *buf++;
        sum += *buf++;
        sum += *buf++;
        sum += *buf++;
        sum += *buf++;
        sum += *buf++;
        sum += *buf++;
    }
    for (nwords = (VOS_UINT32)(bytes & 0x1f) >> 1; nwords > 0; nwords--) {
        sum += *buf++;
    }

    /* Add left-over byte, if any. */
    if (bytes & 0x01) {
        /*lint -e778*/
        sum += *buf & VOS_HTONS(0xff00);
        /*lint +e778*/
    }

    return sum;
}

/*
 * �޸���ʷ:
 * 1.��    ��: 2011��6��19��
 */
STATIC VOS_UINT32 TTF_NDIS_IP6_Pseudo_Header_Checksum_Update(const NDIS_Ipv6Addr *ip6SrcAddr,
                                                             const NDIS_Ipv6Addr *ip6DstAddr, VOS_UINT32 proto,
                                                             VOS_UINT16 len)
{
    VOS_UINT32 sum;

    sum = ip6SrcAddr->in6.addr16bit[0];
    sum += ip6SrcAddr->in6.addr16bit[1];
    sum += ip6SrcAddr->in6.addr16bit[2]; /* 2:addr */
    sum += ip6SrcAddr->in6.addr16bit[3]; /* 3:addr */
    sum += ip6SrcAddr->in6.addr16bit[4]; /* 4:addr */
    sum += ip6SrcAddr->in6.addr16bit[5]; /* 5:addr */
    sum += ip6SrcAddr->in6.addr16bit[6]; /* 6:addr */
    sum += ip6SrcAddr->in6.addr16bit[7]; /* 7:addr */

    sum += ip6DstAddr->in6.addr16bit[0];
    sum += ip6DstAddr->in6.addr16bit[1];
    sum += ip6DstAddr->in6.addr16bit[2]; /* 2:addr */
    sum += ip6DstAddr->in6.addr16bit[3]; /* 3:addr */
    sum += ip6DstAddr->in6.addr16bit[4]; /* 4:addr */
    sum += ip6DstAddr->in6.addr16bit[5]; /* 5:addr */
    sum += ip6DstAddr->in6.addr16bit[6]; /* 6:addr */
    sum += ip6DstAddr->in6.addr16bit[7]; /* 7:addr */

    sum += VOS_HTONS(len);
    sum += VOS_HTONS(proto);

    return sum;
}

/*
 * �޸���ʷ:
 * 1.��    ��: 2011��6��19��
 */
STATIC VOS_UINT16 TTF_NDIS_IP6_Checksum_Finish(VOS_UINT32 sum)
{
    /* Fold 32-bit sum to 16 bits. */
    while (sum >> 16) {
        sum = (sum & 0xffff) + (sum >> 16); /* 16:bit */
    }

    return (VOS_UINT16) ~(VOS_UINT16)sum;
}

/*
 * ��������: ����IP����CheckSum
 * �޸���ʷ:
 *  1.��    ��: 2011��5��24��
 *    �޸�����: �����ɺ���
 */
VOS_UINT16 TTF_NDIS_Ipv6_CalcCheckSum(VOS_UINT8 *pData, VOS_UINT16 len, const NDIS_Ipv6Addr *ip6SrcAddr,
                                      const NDIS_Ipv6Addr *ip6DstAddr, VOS_UINT32 proto)
{
    VOS_UINT32 checkSum;
    VOS_UINT32 pseudoHdrChksum;
    VOS_UINT32 finalChksum;
    VOS_UINT16 stdIntelChksum;

    checkSum        = TTF_NDIS_IP6_Checksum_Update(pData, len);
    pseudoHdrChksum = TTF_NDIS_IP6_Pseudo_Header_Checksum_Update(ip6SrcAddr, ip6DstAddr, proto, len);
    finalChksum     = checkSum + pseudoHdrChksum;
    stdIntelChksum  = TTF_NDIS_IP6_Checksum_Finish(finalChksum);

    return stdIntelChksum;
}

/*
 * ��������: Returns a specific option.
 * �޸���ʷ:
 *  1.��    ��: 2011��4��28��
 *    �޸�����: �����ɺ���
 */
VOS_UINT32 TTF_NDIS_Ipv6GetDhcpOption(IP_Ipv6PktDhcpOptHdr *pFirstOpt, VOS_UINT16 totOptLen,
                                      VOS_UINT16 ndOptCode, IP_Ipv6PktDhcpOptHdr **pOpt, VOS_INT32 lNumber)
{
    VOS_UINT16 optLen;

    *pOpt = pFirstOpt;

    /*
     * �����Խ��, ������usTotOptLen��usOptLen֮��ĳ��ȹ�ϵ
     * IPV6_PKT_DHCP_OPT_HDR_STRU�� OptCode��OptLen�ĳ���
     */
    /* 4��ʾ2���ֽڵ�option-code+2�ֽڵ�option-len */
    while (totOptLen >= 4) {
        optLen = VOS_NTOHS((*pOpt)->dhcpOptLen);
        if (optLen == 0) {
            return PS_FAIL;
        }

        /* 4:ʣ���ܳ���С�ڵ�ǰOption����, �˳� */
        if (totOptLen < (optLen + 4)) {
            break;
        }

        if (ndOptCode == VOS_NTOHS((*pOpt)->dhcpOptCode)) {
            if (lNumber == 0) {
                return PS_SUCC;
            }

            lNumber--;
        }

        totOptLen -= (optLen + 4); /* 4:len */
        *pOpt = (IP_Ipv6PktDhcpOptHdr *)(VOS_VOID *)(((VOS_UINT8 *)*pOpt + optLen) + 4); /* 4:len */
    }

    *pOpt = VOS_NULL_PTR;

    return PS_SUCC;
}

/*
 * ��������: ���IPͷ��
 * �޸���ʷ:
 *  1.��    ��: 2011��4��28��
 *    �޸�����: �����ɺ���
 */
VOS_UINT32 TTF_NDIS_InputUDPHead(VOS_UINT8 *pBuf, VOS_UINT16 srcPort, VOS_UINT16 dstPort, VOS_UINT16 udpDataLen)
{
    IP_UdpHeadInfo *udpHdr = (IP_UdpHeadInfo *)(VOS_VOID *)pBuf;
    if (udpDataLen < sizeof(IP_UdpHeadInfo)) {
        return VOS_ERR;
    }

    (VOS_VOID)memset_s(udpHdr, sizeof(IP_UdpHeadInfo), 0, sizeof(IP_UdpHeadInfo));

    udpHdr->srcPort = VOS_HTONS(srcPort);
    udpHdr->dstPort = VOS_HTONS(dstPort);
    udpHdr->udpLen     = VOS_HTONS(udpDataLen);
    udpHdr->udpCheckSum   = 0;

    return VOS_OK;
}

