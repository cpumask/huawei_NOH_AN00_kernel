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
 * ��Ȩ���У�c����Ϊ�������޹�˾ 2008-2019
 * ��������: ��C�ļ�������---��ɻ������ݴ���ģ��ʵ��
 * ��������: 2008-5-15
 */

#include "si_pb.h"

#if (FEATURE_PHONE_USIM == FEATURE_OFF)
#include "taf_type_def.h"
#include "usimm_ps_interface.h"
#include "om_private.h"
#if (VOS_OSA_CPU == OSA_CPU_CCPU)
#include "usimm_api.h"
#endif
#endif

#if (VOS_OS_VER == VOS_WIN32)
#include "ut_mem.h"
#endif

#define THIS_FILE_ID PS_FILE_ID_PB_DATA_C
#define THIS_MODU mod_pam_pb

#if (FEATURE_PHONE_USIM == FEATURE_OFF)
/*
 * ��������:�����ڻ����ж�λ�绰�����б��е�ƫ��
 * �޸���ʷ:
 * 1. ��    ��: 2007��10��15��
 *    �޸�����: Creat
 */
VOS_UINT32 SI_PB_FindPBOffset(SI_PB_TypeUint32 pbType, VOS_UINT8 *offset)
{
    VOS_UINT8 i;

    for (i = 0; i < SI_PB_MAX_NUMBER; i++) {
        if (g_pbContent[i].pbType == pbType) { /* ��ǰ�Ĳ�ѯ����һ�� */
            PB_INFO_LOG("SI_PB_FindPBOffset Info: Locate the PhoneBook Accurately");

            *offset = i;

            return VOS_OK;
        }
    }

    PB_ERROR_LOG("SI_PB_FindPBOffset Error: The PhoneBook Info is Not Exist");

    return VOS_ERR; /* ��ǰδ�ҵ�ƫ�� */
}

/*
 * ��������:��λ��ǰ���ʵĵ绰���ڻ����е�λ��
 * �޸���ʷ:
 * 1. ��    ��: 2007��10��15��
 *    �޸�����: Creat
 */
VOS_UINT32 SI_PB_LocateRecord(SI_PB_TypeUint32 pbType, VOS_UINT16 index1, VOS_UINT16 index2,
                              VOS_UINT8 *number)
{
    VOS_UINT8  pbOffset;
    VOS_UINT32 result;

    result = SI_PB_FindPBOffset(pbType, &pbOffset); /* ���Ȳ�ѯ��ǰ�Ļ���λ�� */
    if (VOS_ERR == result) {
        PB_WARNING_LOG("SI_PB_LocateRecord: Locate PhoneBook Error");

        return TAF_ERR_UNSPECIFIED_ERROR;
    }

    if (PB_NOT_INITIALISED == g_pbContent[pbOffset].initialState) { /* ��ǰ�ĵ绰��û�г�ʼ����� */
        PB_ERROR_LOG("SI_PB_LocateRecord:The PhoneBook is Not Initializtion");

        return TAF_ERR_SIM_BUSY;
    }

    if (PB_FILE_NOT_EXIST == g_pbContent[pbOffset].initialState) { /* ��ǰ�ĵ绰��û�г�ʼ����� */
        PB_ERROR_LOG("SI_PB_LocateRecord:The PhoneBook is Not Exit");

        return TAF_ERR_FILE_NOT_EXIST;
    }

    if ((index1 > g_pbContent[pbOffset].totalNum) || (index2 > g_pbContent[pbOffset].totalNum) ||
        (index1 > index2)) { /* ��ǰ�������Ѿ�������Χ */
        PB_WARNING_LOG("SI_PB_LocateRecord: The index is Not in The Range of PhoneBook");

        return TAF_ERR_PB_WRONG_INDEX;
    }

    *number = pbOffset; /* ���ص�ǰ�ĵ绰������ƫ�� */

    return VOS_OK;
}

/*
 * ��������:���㻺�������Ŷ�Ӧ�ĵ绰���ļ���ID�ͼ�¼��
 * �޸���ʷ:
 * 1. ��    ��: 2007��10��15��
 *    �޸�����: Creat
 */
VOS_UINT32 SI_PB_CountADNRecordNum(VOS_UINT16 adnIndex, VOS_UINT16 *fileId, VOS_UINT8 *recordNum)
{
    VOS_UINT8  i;
    VOS_UINT16 temp = 0;

    for (i = 0; i < SI_PB_ADNMAX; i++) { /* ���ݵ�ǰ��ADN�б�����ѭ�� */
        if ((temp < adnIndex) && (adnIndex <= (g_pbCtrlInfo.adnInfo[i].recordNum + temp))) { /* ���������ļ��ķ�Χ�� */
            *fileId    = g_pbCtrlInfo.adnInfo[i].fileId; /* ���ص�ǰ���ļ�ID */
            *recordNum = (VOS_UINT8)(adnIndex - temp);        /* ���ص�ǰ�ļ�¼�� */

            return VOS_OK;
        } else {
            temp += g_pbCtrlInfo.adnInfo[i].recordNum; /* �������ۼ� */
        }
    }

    return VOS_ERR;
}

/*
 * ��������: ����XDN�绰�����ͷ��ض�Ӧ�绰����FID
 * �޸���ʷ:
 * 1. ��    ��: 2008��10��15��
 *    �޸�����: Creat
 */
VOS_UINT32 SI_PB_GetXDNFileID(VOS_UINT32 storage, VOS_UINT16 *fileId)
{
    VOS_UINT32 result = VOS_OK;

    switch (storage) {
        case SI_PB_STORAGE_ON:
            *fileId = EFMSISDN;
            break;
        case SI_PB_STORAGE_FD:
            *fileId = EFFDN;
            break;
        case SI_PB_STORAGE_BD:
            *fileId = EFBDN;
            break;
        default:
            result = VOS_ERR;
    }

    return result;
}

/*
 * ��������:BCD��ת����Ascii��
 * �޸���ʷ:
 * 1.��    ��: 2017��10��09��
 *   �޸�����: Creat
 */
VOS_VOID SI_PB_TransToAscii(VOS_UINT8 number, VOS_UINT8 *asciiNum)
{
    if (number <= 9) { /* ת������ */
        *asciiNum = number + 0x30;
    } else if (0x0A == number) { /* ת��*�ַ� */
        *asciiNum = 0x2a;
    } else if (0x0B == number) { /* ת��#�ַ� */
        *asciiNum = 0x23;
    } else if (0x0C == number) { /* ת��'P'�ַ� */
        *asciiNum = 0x50;
    } else if (0x0D == number) { /* ת��'?'�ַ� */
        *asciiNum = 0x3F;
    } else { /* ת����ĸ */
        *asciiNum = number + 0x57;
    }
}

/*
 * ��������:BCD��ת����Ascii��
 * �޸���ʷ:
 * 1. ��    ��: 2011��08��04��
 *    �޸�����: Creat
 */
VOS_VOID SI_PB_BcdToAscii(VOS_UINT8 bcdNumLen, const VOS_UINT8 *bcdNum, VOS_UINT32 asciiBuffSize, VOS_UINT8 *asciiNum,
                          VOS_UINT8 *len)
{
    VOS_UINT8 tmp;
    VOS_UINT8 tmpLen = 0;
    VOS_UINT8 firstNumber;
    VOS_UINT8 secondNumber;

    if ((bcdNumLen * 2) > asciiBuffSize) {
        *len = 0;

        return;
    }

    for (tmp = 0; tmp < bcdNumLen; tmp++) {
        if (0xFF == bcdNum[tmp]) {
            break;
        }

        firstNumber = (VOS_UINT8)(bcdNum[tmp] & 0x0F); /* ȡ���߰��ֽ� */

        secondNumber = (VOS_UINT8)((bcdNum[tmp] >> 4) & 0x0F); /* ȡ���Ͱ��ֽ� */

        SI_PB_TransToAscii(firstNumber, asciiNum);

        asciiNum++;

        tmpLen++;

        if (0x0F == secondNumber) {
            break;
        }

        SI_PB_TransToAscii(secondNumber, asciiNum);

        asciiNum++;

        tmpLen++;
    }

    *len = tmpLen;

    return;
}

/*
 * ��������:�����绰������
 * �޸���ʷ:
 * 1. ��    ��: 2007��10��15��
 *    �޸�����: Creat
 */
VOS_VOID SI_PB_DecodePBName(VOS_UINT8 nameMax, const VOS_UINT8 *name, VOS_UINT32 *alphaType, VOS_UINT8 *nameLen)
{
    VOS_UINT8 i = 0;

    /* ������UCS2 80 */
    if (SI_PB_ALPHATAG_TYPE_UCS2_80 == name[0]) {
        *alphaType = name[0];

        nameMax--;
        for (i = 0; i < (nameMax - (nameMax % 2)); i += 2) { /* ������ǰ���������� */
            if ((name[i + 1] == 0xFF) && (name[i + 2] == 0xFF)) {
                break;
            }
        }

        *nameLen = i; /* �������ȷ��� */

        return;
    }

    /* ������UCS2 81 */
    if (SI_PB_ALPHATAG_TYPE_UCS2_81 == name[0]) {
        *alphaType = name[0];

        /* TAG(81)+ length + 1bytes base address */
        if (nameMax >= 3) {
            if (name[1] > (nameMax - 3)) { /* �����ǰ������Ϣ������󳤶ȣ���ֱ�ӽ׽ض� */
                i = nameMax - 1;
            } else {
                i = name[1] + 2; /* ���㵱ǰ�������� */
            }
        }

        *nameLen = i; /* �������ȷ��� */

        return;
    }

    /* ������UCS2 82 */
    if (SI_PB_ALPHATAG_TYPE_UCS2_82 == name[0]) {
        *alphaType = name[0];

        /* TAG(82)+ length + 2bytes base address */
        if (nameMax >= 4) {
            if (name[1] > (nameMax - 4)) { /* �����ǰ������Ϣ������󳤶ȣ���ֱ�ӽ׽ض� */
                i = nameMax - 1;
            } else {
                i = name[1] + 3; /* ���㵱ǰ�������� */
            }
        }

        *nameLen = i; /* �������ȷ��� */

        return;
    }

    /* ������GSM��ʽ�洢 */
    *alphaType = SI_PB_ALPHATAG_TYPE_GSM;

    for (i = 0; i < nameMax; i++) { /* ������ǰ���������� */
        if (0xFF == name[i]) {
            break;
        }
    }

    *nameLen = i; /* �������ȷ��� */

    return;
}

/*
 * ��������: �Ƿ����EXT�ļ�
 * �޸���ʷ:
 * 1.��    ��: 2019��05��27��
 *   �޸�����: Creat
 */
VOS_UINT32 SI_PB_CheckExtRecord(VOS_UINT8 extRecord, VOS_UINT32 extInfoNum)
{
    if ((extRecord != 0xFF) && (extRecord != 0) && (extRecord <= g_extContent[extInfoNum].extTotalNum)) {
        return VOS_OK;
    }

    return VOS_ERR;
}

/*
 * ��������:ת����ǰ��һ���绰������Ϊָ�������ݽṹ
 * �޸���ʷ:
 * 1. ��    ��: 2007��10��15��
 *    �޸�����: Creat
 */
VOS_VOID SI_PB_TransPBFromate(const SI_PB_Content *pbContent, VOS_UINT16 pbIndex, const VOS_UINT8 *content,
                              SI_PB_Record *record)
{
    VOS_UINT32 result;
    VOS_UINT32 extInfoNum;
    VOS_UINT8  extRecord;
    VOS_UINT8  phoneNumber[SI_PB_NUM_LEN] = {0};
    VOS_UINT8 *extContent                 = VOS_NULL_PTR;
    VOS_UINT8  extNumLen;
    VOS_UINT8  xdnNumLen;
    errno_t    ret;

    result = SI_PB_CheckContentValidity(pbContent, content, pbContent->recordLen); /* ��鵱ǰ�������Ƿ���Ч */

    if (result != VOS_OK) {
        record->validFlag = SI_PB_CONTENT_INVALID; /* ��ǵ�ǰ��������Ч */

        /* ȫ��Ϊ0 */
    } else {
        record->validFlag = SI_PB_CONTENT_VALID; /* ��ǵ�ǰ��������Ч */

        SI_PB_DecodePBName(pbContent->nameLen, content, &record->alphaTagType, &record->alphaTagLength);

        if (record->alphaTagLength != 0x00) { /* ��ǰ������Ϊ�� */
            record->alphaTagLength = (VOS_UINT8)PAM_GetMin(record->alphaTagLength, SI_PB_ALPHATAG_MAX_LEN);

            if (record->alphaTagType == SI_PB_ALPHATAG_TYPE_GSM) { /* ����Ӣ��������ͷ��ʼ */
                ret = memcpy_s(record->alphaTag, SI_PB_ALPHATAG_MAX_LEN, content, record->alphaTagLength);
            } else { /* �������������ӵڶ����ֽڿ�ʼ */
                /* ucs2���룬�����ӳ����ֶο�ʼ */
                ret = memcpy_s(record->alphaTag, SI_PB_ALPHATAG_MAX_LEN, content + 1, record->alphaTagLength);
            }

            PAM_PRINT_SECFUN_RESULT(ret);
        }

        if ((content[pbContent->nameLen + 1] == 0xFF) && (content[pbContent->nameLen] == 0x0)) {
            record->numberType = PB_NUMBER_TYPE_NORMAL;
        } else {
            record->numberType = content[pbContent->nameLen + 1];
        }

        extRecord = ((PB_BDN == pbContent->pbType) ? content[pbContent->recordLen - 2]
                       : content[pbContent->recordLen - 1]);

        extInfoNum  = pbContent->extInfoNum;
        extContent = g_extContent[extInfoNum].extContent;

        result = SI_PB_CheckExtRecord(extRecord, extInfoNum);

        if (content[pbContent->nameLen] < 2) { /* ��ǰ����Ϊ�� */
            record->numberLength = 0;
        } else if ((result == VOS_OK) && (extContent != VOS_NULL_PTR)) {
            extContent += (extRecord - 1) * SI_PB_EXT_LEN;
            extNumLen = (VOS_UINT8)PAM_GetMin(*(extContent + 1), (SI_PB_NUM_LEN / 2));

            ret = memcpy_s(phoneNumber, SI_PB_NUM_LEN / 2, content + (pbContent->nameLen + 2),
                           SI_PB_NUM_LEN / 2);

            PAM_PRINT_SECFUN_RESULT(ret);

            ret = memcpy_s(phoneNumber + (SI_PB_NUM_LEN / 2), SI_PB_NUM_LEN / 2, extContent + 2,
                           SI_PB_NUM_LEN / 2);

            PAM_PRINT_SECFUN_RESULT(ret);

            SI_PB_BcdToAscii((VOS_UINT8)(extNumLen + (SI_PB_NUM_LEN / 2)), phoneNumber, SI_PB_PHONENUM_MAX_LEN,
                             record->number, &record->numberLength);
        } else { /* ������������ݿ�ʼ */
            xdnNumLen = ((content[pbContent->nameLen] - 1) > (SI_PB_NUM_LEN / 2))
                          ? (SI_PB_NUM_LEN / 2)
                          : (content[pbContent->nameLen] - 1);

            SI_PB_BcdToAscii(xdnNumLen, &content[pbContent->nameLen + 2], SI_PB_PHONENUM_MAX_LEN,
                             record->number, &record->numberLength);
        }
    }

    record->index = pbIndex;

    return;
}

/*
 * ��������: �������л�ȡָ��Bitλ��ֵ
 * �޸���ʷ:
 * 1. ��    ��: 2009��5��19��
 *    �޸�����: Creat
 */
VOS_UINT32 SI_PB_GetBitFromBuf(const VOS_UINT8 *dataBuf, VOS_UINT32 dataBufSize, VOS_UINT32 bitNo)
{
    VOS_UINT32 offset;
    VOS_UINT8  bit;

    if ((VOS_NULL_PTR == dataBuf) || (bitNo == 0)) {
        PB_ERROR_LOG("SI_PB_GetBitFromBuf: Input Null Ptr");

        return VOS_FALSE;
    }

    offset = (bitNo - 1) / 8;

    if (offset >= dataBufSize) {
        PB_ERROR_LOG("SI_PB_SetBitToBuf: offset is out of size");

        return VOS_FALSE;
    }

    bit = (VOS_UINT8)((bitNo - 1) % 8);

    return (dataBuf[offset] >> bit) & 0x1;
}

/*
 * ��������: ���ECC�����Ƿ�Ϊ��
 * �޸���ʷ:
 * 1. ��    ��: 2009��12��5��
 *    �޸�����: Creat
 */
VOS_UINT32 SI_PB_CheckEccValidity(VOS_UINT8 content)
{
    if (0xFF == content) {
        PB_INFO_LOG("SI_PB_CheckEccValidity: The ECC is Empty");

        return VOS_ERR;
    } else {
        PB_INFO_LOG("SI_PB_CheckEccValidity: The ECC is Not Empty");

        return VOS_OK;
    }
}

/*
 * ��������:�򵥼�鵱ǰ�ĵ绰�������Ƿ�Ϊ��
 * �޸���ʷ:
 * 1. ��    ��: 2007��10��15��
 *    �޸�����: Creat
 */
VOS_UINT32 SI_PB_CheckContentValidity(const SI_PB_Content *pbContent, const VOS_UINT8 *content, VOS_UINT32 contentSize)
{
    if ((VOS_NULL_PTR == content) || (VOS_NULL_PTR == pbContent)) {
        PB_ERROR_LOG("SI_PB_CheckContentValidity: Input NULL PTR");

        return VOS_ERR;
    }

    if (pbContent->nameLen >= contentSize) {
        PB_ERROR_LOG("SI_PB_CheckContentValidity: File Size is incorrect");

        return VOS_ERR;
    }

    if (((content[pbContent->nameLen] == 0) || (content[pbContent->nameLen] == 0xFF)) &&
        (content[0] == 0xFF)) { /* ��������ͺ����Ƿ�Ϊ�� */
        PB_INFO_LOG("SI_PB_CheckContentValidity: The PhoneBook Content is Empty");

        return VOS_ERR;
    } else {
        PB_INFO_LOG("SI_PB_CheckContentValidity: The PhoneBook Content is Not Empty");

        return VOS_OK;
    }
}

/*
 * ��������:�򵥼�鵱ǰ�ĵ绰�������Ƿ�Ϊ��
 * �޸���ʷ:
 * 1. ��    ��: 2007��10��15��
 *    �޸�����: Creat
 */
VOS_UINT32 SI_PB_CheckANRValidity(VOS_UINT8 value1, VOS_UINT8 value2)
{
    if ((value1 == 0xFF) || (value2 == 0xFF) || (value2 == 0)) { /* �������Ƿ�Ϊ�� */
        PB_INFO_LOG("SI_PB_CheckANRValidity: The PhoneBook Content is Empty");

        return VOS_ERR;
    } else {
        PB_INFO_LOG("SI_PB_CheckANRValidity: The PhoneBook Content is Not Empty");

        return VOS_OK;
    }
}

#if ((OSA_CPU_CCPU == VOS_OSA_CPU) || (defined(DMT)))
/*
 * ��������:���㵱ǰ���ļ���¼�Ŷ�Ӧ�Ļ���������
 * �޸���ʷ:
 * 1. ��    ��: 2007��10��15��
 *    �޸�����: Creat
 */
VOS_UINT32 SI_PB_CountXDNIndex(VOS_UINT32 pbType, VOS_UINT16 fileId, VOS_UINT8 recordNum, VOS_UINT16 *xdnIndex)
{
    VOS_UINT8  i;
    VOS_UINT16 tempIndex = 0;

    if (pbType != PB_ADN_CONTENT) {
        *xdnIndex = recordNum;

        return VOS_OK;
    }

    for (i = 0; i < SI_PB_ADNMAX; i++) {                        /* ���ݵ�ǰ��ADN �ļ��б�����ѭ�� */
        if (fileId == g_pbCtrlInfo.adnInfo[i].fileId) { /* ��ѯ��ǰ���ļ�ID �Ƿ����б��� */
            *xdnIndex = tempIndex + recordNum;                  /* ���㵱ǰ�ļ�¼�ŵ������� */

            return VOS_OK;
        } else {
            tempIndex += g_pbCtrlInfo.adnInfo[i].recordNum; /* ��������Ҫ�ۼ� */
        }
    }

    return VOS_ERR; /* ��ǰ�ļ��������ļ��б��� */
}

/*
 * ��������:����ADN��fid��ȡ��SFI
 * �޸���ʷ:
 * 1. ��    ��: 2009��06��08��
 *    �޸�����: Creat
 */
VOS_UINT32 SI_PB_GetADNSfi(VOS_UINT8 *sfi, VOS_UINT16 fileId)
{
    VOS_UINT32 i;

    for (i = 0; i < g_pbCtrlInfo.adnFileNum; i++) { /* ���ݵ�ǰ��ADN�б�����ѭ�� */
        if (fileId == g_pbCtrlInfo.adnInfo[i].fileId) {
            *sfi = g_pbCtrlInfo.adnInfo[i].sfi;

            return VOS_OK;
        }
    }

    return VOS_ERR;
}

/*
 * ��������: ����ļ��Ƿ���ADN�ļ�
 * �޸���ʷ:
 * 1. ��    ��: 2008��10��15��
 *    �޸�����: Creat
 */
VOS_UINT32 SI_PB_CheckADNFileID(VOS_UINT16 fileId)
{
    VOS_UINT8 j;

    for (j = 0; j < SI_PB_ADNMAX; j++) { /* �Ƚ��ļ�ID�Ƿ������ADN�б��� */
        if (fileId == g_pbCtrlInfo.adnInfo[j].fileId) {
            return VOS_OK;
        }
    }

    return VOS_ERR;
}

/*
 * ��������: ���FID��Ӧ�ĵ绰������
 * �޸���ʷ:
 * 1. ��    ��: 2008��10��15��
 *    �޸�����: Creat
 */
VOS_UINT32 SI_PB_GetXDNPBType(VOS_UINT32 *pbType, VOS_UINT16 fileId)
{
    VOS_UINT32 result = VOS_OK;

    switch (fileId) {
        case EFFDN:
            *pbType = PB_FDN_CONTENT;
            break;
        case EFBDN:
            *pbType = PB_BDN_CONTENT;
            break;
        case EFMSISDN:
            *pbType = PB_MSISDN_CONTENT;
            break;
        case EFSDN:
            *pbType = PB_SDN_CONTENT;
            break;
        default:
            result = SI_PB_CheckADNFileID(fileId); /* ����Ƿ���ADN��FID */
            if (VOS_OK == result) {
                *pbType = PB_ADN_CONTENT;
            } else {
                result = VOS_ERR;
            }
            break;
    }

    return result;
}

/*
 * ��������: ��ȡ�����ANR�ļ�������ANR�ļ�ID
 * �޸���ʷ:
 * 1. ��    ��: 2009��06��03��
 *    �޸�����: Creat
 */
VOS_UINT32 SI_PB_GetANRFid(VOS_UINT32 anrFileNum, VOS_UINT16 *anrFileId)
{
    VOS_UINT32 i;
    VOS_UINT32 j;
    VOS_UINT32 fileCount = 0;

    for (i = 0; i < g_pbCtrlInfo.adnFileNum; i++) {
        for (j = 0; j < SI_PB_ANRMAX; j++) {
            if (SI_PB_FILE_NOT_EXIST != g_pbCtrlInfo.anrInfo[i][j].anrFileId) {
                fileCount++;
            }

            if (anrFileNum == fileCount) {
                *anrFileId = g_pbCtrlInfo.anrInfo[i][j].anrFileId;

                return VOS_OK;
            }
        }
    }

    return VOS_ERR;
}

/*
 * ��������: ��ȡ�����ANR�ļ�ID��Ӧ�Ŀ����ļ��±�
 * �޸���ʷ:
 * 1. ��    ��: 2009��06��03��
 *    �޸�����: Creat
 */
VOS_UINT32 SI_PB_GetANRSuffix(VOS_UINT8 *xSuffix, VOS_UINT8 *ySuffix, VOS_UINT16 anrFileId)
{
    VOS_UINT8  i;
    VOS_UINT8  j;
    VOS_UINT32 adnFileNum;

    adnFileNum = (g_pbCtrlInfo.adnFileNum > SI_PB_ADNMAX) ? SI_PB_ADNMAX : g_pbCtrlInfo.adnFileNum;

    for (i = 0; i < adnFileNum; i++) {
        for (j = 0; j < SI_PB_ANRMAX; j++) {
            if (anrFileId == g_pbCtrlInfo.anrInfo[i][j].anrFileId) {
                *xSuffix = i;
                *ySuffix = j;

                return VOS_OK;
            }
        }
    }

    return VOS_ERR;
}

/*
 * ��������: ��ȡ�����ANR�ļ�ID��Ӧ��IAP
 * �޸���ʷ:
 * 1. ��    ��: 2009��06��03��
 *    �޸�����: Creat
 */
VOS_UINT32 SI_PB_GetIAPFidFromANR(VOS_UINT16 anrFileId, VOS_UINT16 *iapFileId)
{
    VOS_UINT32 i;
    VOS_UINT32 j;

    for (i = 0; i < g_pbCtrlInfo.adnFileNum; i++) {
        for (j = 0; j < SI_PB_ANRMAX; j++) {
            if (anrFileId == g_pbCtrlInfo.anrInfo[i][j].anrFileId) {
                *iapFileId = g_pbCtrlInfo.iapInfo[i].iapFileId;

                return VOS_OK;
            }
        }
    }

    return VOS_ERR;
}

/*
 * ��������: ��ȡ�����EML�ļ�ID��Ӧ��IAP
 * �޸���ʷ:
 * 1. ��    ��: 2009��06��03��
 *    �޸�����: Creat
 */
VOS_UINT32 SI_PB_GetIAPFidFromEML(VOS_UINT16 emlFileId, VOS_UINT16 *iapFileId)
{
    VOS_UINT32 i;

    for (i = 0; i < g_pbCtrlInfo.emlFileNum; i++) {
        if (emlFileId == g_pbCtrlInfo.emlInfo[i].emlFileId) {
            *iapFileId = g_pbCtrlInfo.iapInfo[i].iapFileId;

            return VOS_OK;
        }
    }

    return VOS_ERR;
}

/*
 * ��������: ���ݿ�����Ϣ�±��ANR�ļ�ID���ж��Ƿ�Ϊ���һ��
 * �޸���ʷ:
 * 1. ��    ��: 2009��06��03��
 *    �޸�����: Creat
 */
VOS_UINT32 SI_PB_GheckANRLast(VOS_UINT8 xSuffix, VOS_UINT8 ySuffix)
{
    if ((xSuffix + 1) == (VOS_UINT8)g_pbCtrlInfo.adnFileNum) {
        return VOS_OK;
    }

    if (SI_PB_FILE_NOT_EXIST == g_pbCtrlInfo.anrInfo[xSuffix + 1][ySuffix].anrFileId) {
        return VOS_OK;
    }

    return VOS_ERR;
}

/*
 * ��������:ͨ��ADN��ȡ��Email�ļ���ID
 * �޸���ʷ:
 * 1. ��    ��: 2009��06��10��
 *    �޸�����: Creat
 */
VOS_UINT32 SI_PB_GetEMLFIdFromADN(VOS_UINT16 *emlFileId, VOS_UINT16 aDNId)
{
    VOS_UINT32 i;

    for (i = 0; i < g_pbCtrlInfo.emlFileNum; i++) {
        if (aDNId == g_pbCtrlInfo.adnInfo[i].fileId) {
            *emlFileId = g_pbCtrlInfo.emlInfo[i].emlFileId;

            return VOS_OK;
        }
    }

    PB_NORMAL1_LOG("SI_PB_GetEMLFIdFromADN: Get ADN %d email Error", aDNId);

    return VOS_ERR;
}

/*
 * ��������:ͨ��ADN Fid ����ANR Fid
 * �޸���ʷ:
 * 1. ��    ��: 2009��06��08��
 *    �޸�����: Creat
 */
VOS_UINT32 SI_PB_GetANRFidFromADN(VOS_UINT8 aNROffset, VOS_UINT16 adfFileId, VOS_UINT16 *anrFileId)
{
    VOS_UINT32 i;

    for (i = 0; i < g_pbCtrlInfo.adnFileNum; i++) {
        if (adfFileId == g_pbCtrlInfo.adnInfo[i].fileId) {
            *anrFileId = g_pbCtrlInfo.anrInfo[i][aNROffset].anrFileId;

            return VOS_OK;
        }
    }

    return VOS_ERR;
}
/*
 * ��������:���������ANR�ļ�ID,��ȡ���м�¼��
 * �޸���ʷ:
 * 1. ��    ��: 2009��06��08��
 *    �޸�����: Creat
 */
VOS_UINT32 SI_PB_GetFreeANRRecordNum(VOS_UINT16 aNRFid, VOS_UINT8 *recordNum, VOS_UINT16 *iapFid)
{
    VOS_UINT32 i        = 0;
    VOS_UINT32 j        = 0;
    VOS_UINT32 k        = 0;
    VOS_UINT32 offset = 0;
    VOS_UINT32 offsetNew;
    VOS_UINT32 flag   = VOS_FALSE;

    for (i = 0; i < g_pbCtrlInfo.adnFileNum; i++) { /* ���ݵ�ǰ��ADN�б�����ѭ�� */
        for (j = 0; j < SI_PB_ANRMAX; j++) {
            if (aNRFid == g_pbCtrlInfo.anrInfo[i][j].anrFileId) {
                *iapFid = g_pbCtrlInfo.iapInfo[i].iapFileId;

                flag = VOS_TRUE;

                break;
            }
        }

        if (VOS_TRUE == flag) {
            break;
        }

        offset += (g_pbCtrlInfo.anrInfo[i][0].recordNum * g_pbCtrlInfo.anrInfo[i][0].recordLen);
    }

    if ((i == g_pbCtrlInfo.adnFileNum) || (SI_PB_ANRMAX <= j)) {
        return VOS_ERR;
    }

    for (k = 0; k < g_pbCtrlInfo.anrInfo[i][j].recordNum; k++) {
        offsetNew = offset + k * g_anrContent[j % SI_PB_ANRMAX].recordLen;

        if ((g_anrContent[j % SI_PB_ANRMAX].content == VOS_NULL_PTR) ||
            (g_anrContent[j % SI_PB_ANRMAX].contentSize <= (offsetNew + 1))) {
            PB_ERROR_LOG("SI_PB_GetFreeANRRecordNum: ANR content is NULL");

            return VOS_ERR;
        }

        if (VOS_OK != SI_PB_CheckANRValidity(g_anrContent[j % SI_PB_ANRMAX].content[offsetNew],
                                             g_anrContent[j % SI_PB_ANRMAX].content[offsetNew + 1])) {
            *recordNum = (VOS_UINT8)(k + 1);
            return VOS_OK;
        }
    }

    return VOS_ERR;
}
/*
 * ��������:���������Email�ļ�ID,��ȡ���м�¼��
 * �޸���ʷ:
 * 1. ��    ��: 2009��06��08��
 *    �޸�����: Creat
 */
VOS_UINT32 SI_PB_GetFreeEMLRecordNum(VOS_UINT16 eMLFid, VOS_UINT8 *recordNum, VOS_UINT16 *iapFid)
{
    VOS_UINT32 i        = 0;
    VOS_UINT32 j        = 0;
    VOS_UINT32 offset = 0;

    for (i = 0; i < g_pbCtrlInfo.emlFileNum; i++) { /* ���ݵ�ǰ��ADN�б�����ѭ�� */
        if (eMLFid == g_pbCtrlInfo.emlInfo[i].emlFileId) {
            *iapFid = g_pbCtrlInfo.iapInfo[i].iapFileId;
            break;
        }

        offset += (g_pbCtrlInfo.emlInfo[i].recordNum * g_pbCtrlInfo.emlInfo[i].recordLen);
    }

    if (i == g_pbCtrlInfo.emlFileNum) {
        return VOS_ERR;
    }

    for (j = 0; j < g_pbCtrlInfo.emlInfo[i].recordNum; j++) {
        if (0xFF == g_emlContent.content[offset + (j * g_emlContent.recordLen)]) {
            *recordNum = (VOS_UINT8)(j + 1);
            return VOS_OK;
        }
    }

    return VOS_ERR;
}

/*
 * ��������:���������Email�ļ�ID��Record����ȡEmail��������
 * �޸���ʷ:
 * 1. ��    ��: 2009��06��08��
 *    �޸�����: Creat
 */
VOS_UINT32 SI_PB_GetEMLRecord(VOS_UINT8 **ppEMLContent, VOS_UINT16 emlFileId, VOS_UINT8 recordNum,
                              VOS_UINT32 *freeSize)
{
    VOS_UINT32 i           = 0;
    VOS_UINT16 offset    = 0;
    VOS_UINT32 srcOffset = 0;

    for (i = 0; i < g_pbCtrlInfo.emlFileNum; i++) {
        if (emlFileId == g_pbCtrlInfo.emlInfo[i].emlFileId) {
            break;
        }
    }

    if ((i >= g_pbCtrlInfo.emlFileNum) || (VOS_NULL_PTR == g_emlContent.content)) {
        return VOS_ERR;
    }

    offset    = (VOS_UINT16)((i * g_pbCtrlInfo.emlInfo[i].recordNum) + recordNum);
    srcOffset = (offset - 1) * g_emlContent.recordLen;

    if (g_emlContent.contentSize < srcOffset) {
        return VOS_ERR;
    }

    *ppEMLContent = &g_emlContent.content[srcOffset];
    *freeSize  = g_emlContent.contentSize - srcOffset;

    return VOS_OK;
}

/*
 * ��������: ��EXT�ļ��Ļ������ҵ���һ������ʹ�õĿ��м�¼
 * �޸���ʷ:
 * 1. ��    ��: 2009��5��9��
 *    �޸�����: Creat
 */
VOS_UINT32 SI_PB_FindUnusedExtRecord(const SI_EXT_Content *extContent, VOS_UINT8 *record, VOS_UINT8 recordCount)
{
    VOS_UINT32 countFreeRec = 1;
    VOS_UINT8 *content       = VOS_NULL_PTR;
    VOS_UINT8 i;

    /* ��ʹ�ü�¼��������¼�����ʱ�����Ѿ�û�п���ʹ�õ�EXT��¼ */
    if (extContent->extTotalNum == extContent->extUsedNum) {
        *record = 0xFF;

        return VOS_ERR;
    }

    content = extContent->extContent;

    if (VOS_NULL_PTR == content) {
        *record = 0xFF;

        return VOS_ERR;
    }

    if (0xFF < extContent->extTotalNum) {
        PB_WARNING1_LOG("SI_PB_FindUnusedExtRecord: pstEXTContent->extTotalNum err", extContent->extTotalNum);

        return VOS_ERR;
    }

    /* ����EXT�ļ��ĵڶ����ֽ����ж��Ƿ�Ϊ�ռ�¼ */
    for (i = 0; i < extContent->extTotalNum; i++) {
        if (0xFF == content[1]) {
            if (countFreeRec == recordCount) {
                break;
            } else {
                countFreeRec++;
            }
        }

        content += SI_PB_EXT_LEN;
    }

    if (i < extContent->extTotalNum) {
        *record = i + 1;

        return VOS_OK;
    } else {
        *record = 0xFF;

        return VOS_ERR;
    }
}

/*
 * ��������: ������żλ���ϵ��ַ�
 * �޸���ʷ:
 * 1.��    ��: 2017��10��9��
 *   �޸�����: Creat
 */
VOS_UINT8 SI_PB_AsciiToBcd_Value(VOS_UINT8 flag, VOS_UINT8 para1, VOS_UINT8 para2)
{
    VOS_UINT8 result;

    /* ����λ���ϵ��ַ� */
    if (0 == flag) {
        result = para1;
    } else { /* ż��λ���ϵ��ַ� */
        result = (VOS_UINT8)((para1 << 4) | para2);
    }

    return result;
}

/*
 * ��������:ת��ASCII��ΪBCD��
 * �޸���ʷ:
 * 1. ��    ��: 2007��10��15��
 *    �޸�����: Creat
 */
VOS_VOID SI_PB_AsciiToBcd(const VOS_UINT8 *asciiNum, VOS_UINT8 asciiNumLen, VOS_UINT8 *bcdNum, VOS_UINT32 bcdBuffSize,
                          VOS_UINT8 *bcdNumLen)
{
    VOS_UINT8  i, k;
    VOS_UINT8 *temp = VOS_NULL_PTR;

    /* �����ж� */
    if (((asciiNumLen + 1) / 2) > bcdBuffSize) {
        *bcdNumLen = 0;

        return;
    }

    temp = bcdNum;

    for (i = 0; i < asciiNumLen; i++) {
        k = i % 2;

        if ((*(asciiNum + i) >= 0x30) && (*(asciiNum + i) <= 0x39)) { /* ת������ */
            *(temp + (i / 2)) = SI_PB_AsciiToBcd_Value(k, *(asciiNum + i) - 0x30, *(temp + (i / 2)));
        } else if ('*' == *(asciiNum + i)) { /* ת��*�ַ� */
            *(temp + (i / 2)) = SI_PB_AsciiToBcd_Value(k, 0xa, *(temp + (i / 2)));
        } else if ('#' == *(asciiNum + i)) { /* ת��#�ַ� */
            *(temp + (i / 2)) = SI_PB_AsciiToBcd_Value(k, 0xb, *(temp + (i / 2)));
        } else if ('?' == *(asciiNum + i)) { /* ת��?�ַ� */
            *(temp + (i / 2)) = SI_PB_AsciiToBcd_Value(k, 0xd, *(temp + (i / 2)));
        } else if (('P' == *(asciiNum + i)) || ('p' == *(asciiNum + i)) ||
                   (',' == *(asciiNum + i))) { /* ת��PAUSE�ַ� */
            *(temp + (i / 2)) = SI_PB_AsciiToBcd_Value(k, 0xc, *(temp + (i / 2)));
        } else { /* ����ʶ����ַ� */
            PB_WARNING_LOG("SI_PB_AsciiToBcd: The Char is Not Suspensory"); /* ��ӡ���� */
        }
    }

    *bcdNumLen = asciiNumLen / 2;

    if ((asciiNumLen % 2) == 1) { /* �����ֽں�����F */
        *(temp + (asciiNumLen / 2)) = 0xf0 | *(temp + (asciiNumLen / 2));

        (*bcdNumLen)++;
    }

    return;
}

/*
 * ��������:��SimpleTLV���ݽṹ�в�ѯ��ǰ��Tag������ʵ�����ݳ��ȵ�ƫ��
 * �޸���ʷ:
 * 1. ��    ��: 2009��05��23��
 *    �޸�����: Creat
 */
VOS_UINT32 SI_FindMultiTagInBERTLV(const VOS_UINT8 *type1TagAddr, VOS_UINT32 dataLen, VOS_UINT8 tag,
                                   VOS_UINT8 *offset, VOS_UINT8 tagCount)
{
    VOS_UINT32 i           = 0;
    const VOS_UINT8 *dataBuf  = VOS_NULL_PTR;
    VOS_UINT8  findCount = 0;

    dataBuf = type1TagAddr + 2;

    while (((i + 1) < dataLen) && (findCount < tagCount)) {
        if (dataBuf[i] == tag) { /* ���س����ֽ�ƫ�� */
            offset[findCount] = (VOS_UINT8)(i + 1);

            findCount++;
        }

        i += dataBuf[i + 1] + 2; /* ������һ��Tag */
    }

    return (0 == findCount) ? SI_PB_TAGNOTFOUND : findCount;
}

/*
 * ��������:��SimpleTLV���ݽṹ�в�ѯ��ǰ��Tag������ʵ�����ݳ��ȵ�ƫ��
 * �޸���ʷ:
 * 1. ��    ��: 2007��10��20��
 *    �޸�����: Creat
 */
VOS_UINT32 SI_FindTagInBERTLV(const VOS_UINT8 *dataBuf, VOS_UINT8 tag, VOS_UINT32 dataLen)
{
    VOS_UINT32 i = 0;

    while ((i + 1) < dataLen) {
        if (dataBuf[i] == tag) { /* ���س����ֽ�ƫ�� */
            return (i + 1);
        } else {
            i += dataBuf[i + 1] + 2; /* ������һ��Tag */
        }
    }

    return SI_PB_TAGNOTFOUND;
}

/*
 * ��������:��Type2 TLV�ṹ���ҵ���ӦTag����TLV�ļ��еڼ����ļ�
 * �޸���ʷ:
 * 1. ��    ��: 2009��06��05��
 *    �޸�����: Creat
 */
VOS_UINT32 SI_FindType2FileTagNum(const VOS_UINT8 *dataBuf, VOS_UINT8 tag, VOS_UINT32 dataLen)
{
    VOS_UINT32 i        = 0;
    VOS_UINT32 tagNum = 1;

    while ((i + 1) < dataLen) {
        if (dataBuf[i] == tag) { /* ����Tag Num */
            return tagNum;
        } else {
            tagNum++;

            i += dataBuf[i + 1] + 2; /* ������һ��Tag */
        }
    }

    return SI_PB_TAGNOTFOUND;
}

/*
 * ��������: ��A8��TAG�н���ADN�ļ���Ϣ
 * �޸���ʷ:
 * 1. ��    ��: 2009��6��15��
 *    �޸�����: Creat
 */
VOS_UINT32 SI_PB_DecodeEFPBR_AdnContent(const VOS_UINT8 *type1TagAddr, VOS_UINT32 spareBuffSize, VOS_UINT32 i)
{
    VOS_UINT32 offset;
    VOS_UINT32 validLen;

    validLen = PAM_GetMin(type1TagAddr[1], spareBuffSize - 2);

    offset = SI_FindTagInBERTLV(&type1TagAddr[2], EFADNDO_TAG, validLen); /* ��ѯ��ǰ��ADN�ļ�ID */

    if (SI_PB_TAGNOTFOUND == offset) { /* δ�ҵ���Ϣ */
        PB_ERROR_LOG("SI_PB_DecodeEFPBR_AdnContent: Could Not Find the EFADNDO_TAG Tag");

        return offset;
    }

    /* ȷ����FileId�ֶ� */
    if ((offset + 4) >= spareBuffSize) {
        return SI_PB_TAGNOTFOUND;
    }

    g_pbCtrlInfo.adnInfo[i].fileId = ((type1TagAddr[offset + 3] << 0x08) & 0xFF00) +
                                           type1TagAddr[offset + 4];

    /* �����SFI����Ҫ��¼���� */
    if (0x03 == type1TagAddr[offset + 2]) {
        /* ȷ����SFI�ֶ� */
        if ((offset + 5) < spareBuffSize) {
            g_pbCtrlInfo.adnInfo[i].sfi = type1TagAddr[offset + 5];
        }
    }

    g_pbCtrlInfo.adnFileNum++;

    return VOS_OK;
}

/*
 * ��������: ��A8��TAG�н���UID�ļ���Ϣ
 * �޸���ʷ:
 * 1. ��    ��: 2009��6��15��
 *    �޸�����: Creat
 */
VOS_VOID SI_PB_DecodeEFPBR_UidContent(const VOS_UINT8 *type1TagAddr, VOS_UINT32 spareBuffSize, VOS_UINT32 i)
{
    VOS_UINT32 offset;
    VOS_UINT32 validLen;

    validLen = PAM_GetMin(type1TagAddr[1], spareBuffSize - 2);

    offset = SI_FindTagInBERTLV(&type1TagAddr[2], EFUIDDO_TAG, validLen); /* ��ѯ��ǰ��UID�ļ�ID */

    if (SI_PB_TAGNOTFOUND == offset) { /* δ�ҵ���Ϣ */
        PB_WARNING_LOG("SI_PB_DecodeEFPBR_UidContent: Could Not Find the EFUIDDO_TAG Tag");

        g_pbCtrlInfo.uidInfo[i].fileId = 0xFFFF;
    } else {
        /* ȷ����FileId�ֶ� */
        if ((offset + 4) < spareBuffSize) {
            g_pbCtrlInfo.uidInfo[i].fileId = ((type1TagAddr[offset + 3] << 0x08) & 0xFF00) +
                                                   type1TagAddr[offset + 4];
        } else {
            g_pbCtrlInfo.uidInfo[i].fileId = 0xFFFF;
        }
    }
}

/*
 * ��������: ��A8��TAG�н���PBC�ļ���Ϣ
 * �޸���ʷ:
 * 1. ��    ��: 2009��6��15��
 *    �޸�����: Creat
 */
VOS_VOID SI_PB_DecodeEFPBR_PbcContent(const VOS_UINT8 *type1TagAddr, VOS_UINT32 spareBuffSize, VOS_UINT32 i)
{
    VOS_UINT32 offset;
    VOS_UINT32 validLen;

    validLen = PAM_GetMin(type1TagAddr[1], spareBuffSize - 2);

    offset = SI_FindTagInBERTLV(&type1TagAddr[2], EFPBCDO_TAG, validLen); /* ��ѯ��ǰ��PBC�ļ�ID */

    if (SI_PB_TAGNOTFOUND == offset) { /* δ�ҵ���Ϣ */
        PB_WARNING_LOG("SI_PB_DecodeEFPBR_PbcContent: Could Not Find the EFUIDDO_TAG Tag");

        g_pbCtrlInfo.pbCInfo[i].fileId = 0xFFFF;

        g_pbCStatus = VOS_FALSE;
    } else if (VOS_TRUE == SI_PB_CheckSupportAP()) {
        g_pbCtrlInfo.pbCInfo[i].fileId = 0xFFFF;

        g_pbCStatus = VOS_FALSE;
    } else {
        /* ȷ����FileId�ֶ� */
        if ((offset + 4) < spareBuffSize) {
            g_pbCtrlInfo.pbCInfo[i].fileId = ((type1TagAddr[offset + 3] << 0x08) & 0xFF00) +
                                                   type1TagAddr[offset + 4];
        } else {
            g_pbCtrlInfo.pbCInfo[i].fileId = 0xFFFF;

            g_pbCStatus = VOS_FALSE;
        }
    }
}

/*
 * ��������: ��A8��TAG�н���AIP�ļ���Ϣ
 * �޸���ʷ:
 * 1. ��    ��: 2009��6��15��
 *    �޸�����: Creat
 */
VOS_VOID SI_PB_DecodeEFPBR_IapContent(const VOS_UINT8 *type1TagAddr, VOS_UINT32 spareBuffSize, VOS_UINT32 i)
{
    VOS_UINT32 offset;
    VOS_UINT32 validLen;

    validLen = PAM_GetMin(type1TagAddr[1], spareBuffSize - 2);

    offset = SI_FindTagInBERTLV(&type1TagAddr[2], EFIAPDO_TAG, validLen); /* ��ѯ��ǰ��IAP�ļ�ID */

    if (SI_PB_TAGNOTFOUND == offset) { /* δ�ҵ���Ϣ */
        PB_WARNING_LOG("SI_PB_DecodeEFPBR_IapContent: Could Not Find the EFIAPDO_TAG Tag");
    } else {
        /* ȷ����FileId�ֶ� */
        if ((offset + 4) < spareBuffSize) {
            g_pbCtrlInfo.iapInfo[i].iapFileId = ((type1TagAddr[offset + 3] << 0x08) & 0xFF00) +
                                                      type1TagAddr[offset + 4];

            g_pbCtrlInfo.iapFileNum++;
        }
    }
}

/*
 * ��������: ��A8��TAG�н���ANR�ļ���Ϣ
 * �޸���ʷ:
 * 1. ��    ��: 2009��6��15��
 *    �޸�����: Creat
 */
VOS_UINT32 SI_PB_DecodeType1EFPBR_AnrContent(const VOS_UINT8 *type1TagAddr, VOS_UINT32 spareBuffSize, VOS_UINT32 i,
                                             VOS_UINT8 *offset, VOS_UINT8 dataLen)
{
    VOS_UINT32 anrCount; /* ��¼ÿ����¼�е�ANR���� */
    VOS_UINT32 anrOffset;
    VOS_UINT32 j;
    VOS_UINT32 validLen;

    validLen = PAM_GetMin(type1TagAddr[1], spareBuffSize - 2);

    anrCount = SI_FindMultiTagInBERTLV(type1TagAddr, validLen, (VOS_UINT8)EFANRDO_TAG, offset,
                                         dataLen); /* ��ѯ��ǰ��ANR�ļ� */

    if (SI_PB_TAGNOTFOUND == anrCount) { /* δ�ҵ���Ϣ */
        PB_NORMAL_LOG("SI_PB_DecodeType1EFPBR_AnrContent: Could Not Find the EFANRDO_TAG Tag");
    } else {
        anrCount = (anrCount > SI_PB_ANRMAX) ? SI_PB_ANRMAX : anrCount;

        for (j = 0; j < anrCount; j++) {
            anrOffset = offset[j];

            /* ȷ����FileId�ֶ� */
            if ((anrOffset + 4) >= spareBuffSize) {
                break;
            }

            g_pbCtrlInfo.anrInfo[i][j].anrFileId = ((type1TagAddr[anrOffset + 3] << 0x08) & 0xFF00) +
                                                         type1TagAddr[anrOffset + 4];
            g_pbCtrlInfo.anrInfo[i][j].anrType = PB_FILE_TYPE1;
            g_pbCtrlInfo.anrFileNum++;
        }
    }

    return anrCount;
}

/*
 * ��������: ��A9��TAG�н���ANR�ļ���Ϣ
 * �޸���ʷ:
 * 1. ��    ��: 2009��9��5��
 *    �޸�����: Creat
 */
VOS_VOID SI_PB_DecodeType2EFPBR_AnrContent(const VOS_UINT8 *type2TagAddr, VOS_UINT32 spareBuffSize, VOS_UINT32 i,
                                           VOS_UINT8 *offset, VOS_UINT8 dataLen)
{
    VOS_UINT32 anrCount; /* ��¼ÿ����¼�е�ANR���� */
    VOS_UINT32 anrOffset;
    VOS_UINT32 j;
    VOS_UINT32 validLen;

    /* ȷ��������TL+T�ֶ� */
    if (spareBuffSize < 3) {
        return;
    }

    validLen = PAM_GetMin(type2TagAddr[1], spareBuffSize - 2);

    anrCount = SI_FindMultiTagInBERTLV(type2TagAddr, validLen, (VOS_UINT8)EFANRDO_TAG, offset,
                                         dataLen); /* ��ѯ��ǰ��ANR�ļ� */

    if (SI_PB_TAGNOTFOUND == anrCount) { /* δ�ҵ���Ϣ */
        PB_NORMAL_LOG("SI_PB_DecodeType2EFPBR_AnrContent: Could Not Find the EFANRDO_TAG Tag");
    } else {
        anrCount = (anrCount > SI_PB_ANRMAX) ? SI_PB_ANRMAX : anrCount;

        for (j = 0; j < anrCount; j++) {
            anrOffset = offset[j];

            /* ȷ����FileId�ֶ� */
            if ((anrOffset + 4) >= spareBuffSize) {
                break;
            }

            g_pbCtrlInfo.anrInfo[i][j].anrFileId = ((type2TagAddr[anrOffset + 3] << 0x08) & 0xFF00) +
                                                         type2TagAddr[anrOffset + 4];
            g_pbCtrlInfo.anrInfo[i][j].anrType = PB_FILE_TYPE2;
            g_pbCtrlInfo.anrFileNum++;

            g_pbCtrlInfo.anrInfo[i][j].anrTagNum = SI_FindType2FileTagNum(type2TagAddr + 2, EFANRDO_TAG,
                                                                                validLen);
        }
    }

    return;
}

/*
 * ��������: ��AA��TAG�н���EXT1�ļ���Ϣ
 * �޸���ʷ:
 * 1. ��    ��: 2009��6��15��
 *    �޸�����: Creat
 */
VOS_VOID SI_PB_DecodeEFPBR_ExtContent(const VOS_UINT8 *type3TagAddr, VOS_UINT32 spareBuffSize)
{
    VOS_UINT32 offset;
    VOS_UINT32 validLen;

    /* ȷ��������TL+T�ֶ� */
    if (spareBuffSize < 3) {
        g_pbCtrlInfo.ext1FileId                 = 0xFFFF;
        g_extContent[PB_ADN_CONTENT].extFileId = 0xFFFF;
        g_extContent[PB_ADN_CONTENT].extFlag   = SI_PB_FILE_NOT_EXIST;

        return;
    }

    validLen = PAM_GetMin(type3TagAddr[1], spareBuffSize - 2);

    /* ���е�ADN�ļ���ֻ��Ӧ��ͬһ��EXT1�ļ�,ֻҪ��һ��PBR�ļ�¼���ҵ��Ϳ����� */
    offset = SI_FindTagInBERTLV(type3TagAddr + 2, EFEXT1DO_TAG, validLen); /* ��ѯ��ǰ��EXT�ļ�ID */

    /* δ�ҵ���Ϣ */
    if (SI_PB_TAGNOTFOUND == offset) {
        PB_WARNING_LOG("SI_PB_DecodeEFPBR_ExtContent: Could Not Find the EFEXTDO_TAG Tag");

        g_pbCtrlInfo.ext1FileId                 = 0xFFFF;
        g_extContent[PB_ADN_CONTENT].extFileId = 0xFFFF;
        g_extContent[PB_ADN_CONTENT].extFlag   = SI_PB_FILE_NOT_EXIST;
    } else if (SI_PB_FILE_EXIST == g_extContent[PB_ADN_CONTENT].extFlag) {
        /* do nothing */
    } else {
        /* ȷ����FileId�ֶ� */
        if ((offset + 4) < spareBuffSize) {
            g_pbCtrlInfo.ext1FileId = ((type3TagAddr[offset + 3] << 0x08) & 0xFF00) + type3TagAddr[offset + 4];
            g_extContent[PB_ADN_CONTENT].extFlag = SI_PB_FILE_EXIST;
        } else {
            g_pbCtrlInfo.ext1FileId                 = 0xFFFF;
            g_extContent[PB_ADN_CONTENT].extFileId = 0xFFFF;
            g_extContent[PB_ADN_CONTENT].extFlag   = SI_PB_FILE_NOT_EXIST;
        }
    }
}

/*
 * ��������: ��A9��TAG�н���EMAIL�ļ���Ϣ
 * �޸���ʷ:
 * 1. ��    ��: 2009��6��15��
 *    �޸�����: Creat
 */
VOS_VOID SI_PB_DecodeType2EFPBR_EmailContent(const VOS_UINT8 *type2TagAddr, VOS_UINT32 spareBuffSize, VOS_UINT32 i)
{
    VOS_UINT32 offset;
    VOS_UINT32 validLen;

    /* ȷ��������TL+T�ֶ� */
    if (spareBuffSize < 3) {
        return;
    }

    validLen = PAM_GetMin(type2TagAddr[1], spareBuffSize - 2);

    offset = SI_FindTagInBERTLV(type2TagAddr + 2, EFEMAILDO_TAG, validLen); /* ��ѯ��ǰ��EXT�ļ�ID */

    /* δ�ҵ���Ϣ */
    if (SI_PB_TAGNOTFOUND == offset) {
        PB_NORMAL_LOG("SI_PB_DecodeType2EFPBR_EmailContent: Could Not Find the EFEMAILDO_TAG Tag");
    } else {
        /* ȷ����FileId�ֶ� */
        if ((offset + 4) < spareBuffSize) {
            g_pbCtrlInfo.emlInfo[i].emlFileId = ((type2TagAddr[offset + 3] << 0x08) & 0xFF00) +
                                                      type2TagAddr[offset + 4];
            g_pbCtrlInfo.emlFileNum++;

            g_pbCtrlInfo.emlInfo[i].emlType = PB_FILE_TYPE2;

            g_pbCtrlInfo.emlInfo[i].emlTagNum = SI_FindType2FileTagNum(type2TagAddr + 2, EFEMAILDO_TAG,
                                                                             validLen);
        }
    }
}

/*
 * ��������: ��A8��TAG�н���EMAIL�ļ���Ϣ
 * �޸���ʷ:
 * 1. ��    ��: 2009��6��26��
 *    �޸�����: Creat
 */
VOS_UINT32 SI_PB_DecodeType1EFPBR_EmailContent(const VOS_UINT8 *type1TagAddr, VOS_UINT32 spareBuffSize, VOS_UINT32 i)
{
    VOS_UINT32 offset;
    VOS_UINT32 validLen;

    validLen = PAM_GetMin(type1TagAddr[1], spareBuffSize - 2);

    offset = SI_FindTagInBERTLV(type1TagAddr + 2, EFEMAILDO_TAG, validLen); /* ��ѯ��ǰ��EXT�ļ�ID */

    /* δ�ҵ���Ϣ */
    if (SI_PB_TAGNOTFOUND == offset) {
        PB_NORMAL_LOG("SI_PB_DecodeType1EFPBR_EmailContent: Could Not Find the EFEMAILDO_TAG Tag");

        return SI_PB_TAGNOTFOUND;
    }

    /* ȷ����FileId�ֶ� */
    if ((offset + 4) >= spareBuffSize) {
        return SI_PB_TAGNOTFOUND;
    }

    g_pbCtrlInfo.emlInfo[i].emlFileId = ((type1TagAddr[offset + 3] << 0x08) & 0xFF00) +
                                              type1TagAddr[offset + 4];
    g_pbCtrlInfo.emlFileNum++;

    g_pbCtrlInfo.emlInfo[i].emlType = PB_FILE_TYPE1;

    return VOS_OK;
}

/*
 * ��������: ����Ƿ����ظ����ļ�ID
 * �޸���ʷ:
 * 1.��    ��: 2019��6��13��
 *   �޸�����: Creat
 */
VOS_VOID SI_PB_DeleteSameFile(VOS_VOID)
{
    VOS_UINT32 i;
    VOS_UINT32 j;

    for (i = g_pbCtrlInfo.adnFileNum - 1; i != 0; i--) {
        for (j = 0; j < SI_PB_ANRMAX; j++) {
            if ((g_pbCtrlInfo.anrInfo[i][j].anrFileId == g_pbCtrlInfo.anrInfo[i - 1][j].anrFileId) &&
                (g_pbCtrlInfo.anrInfo[i][j].anrFileId != 0)) {
                g_pbCtrlInfo.anrInfo[i][j].anrFileId = 0;
                g_pbCtrlInfo.anrInfo[i][j].recordNum = 0;
                g_pbCtrlInfo.anrInfo[i][j].recordLen = 0;

                g_pbCtrlInfo.anrFileNum--;
            }
        }

        if ((g_pbCtrlInfo.emlInfo[i].emlFileId == g_pbCtrlInfo.emlInfo[i - 1].emlFileId) &&
            (g_pbCtrlInfo.emlInfo[i].emlFileId != 0)) {
            g_pbCtrlInfo.emlInfo[i].emlFileId = 0;
            g_pbCtrlInfo.emlInfo[i].recordLen = 0;
            g_pbCtrlInfo.emlInfo[i].recordNum = 0;

            g_pbCtrlInfo.emlFileNum--;
        }
    }

    return;
}

/*
 * ��������:����EFPBR�����ݣ���ȡADN /UID���ļ�ID
 * �޸���ʷ:
 * 1. ��    ��: 2007��10��15��
 *    �޸�����: Creat
 */
VOS_UINT32 SI_PB_DecodeEFPBR(VOS_UINT8 recordNum, VOS_UINT8 recordLen, const VOS_UINT8 *content)
{
    VOS_UINT32 i;
    VOS_UINT8 *ptemp        = (VOS_UINT8*)content;
    VOS_UINT8 *type1TagAddr = VOS_NULL_PTR;
    VOS_UINT8 *type2TagAddr = VOS_NULL_PTR;
    VOS_UINT8 *type3TagAddr = VOS_NULL_PTR;
    VOS_UINT8  aNROffset[SI_PB_ANR_MAX];
    VOS_UINT32 type1EMLFlag = VOS_OK;
    VOS_UINT32 type1ANRFlag = VOS_OK;
    VOS_UINT32 offset;

    g_pbCtrlInfo.adnFileNum = 0;

    for (i = 0; i < recordNum; i++, (ptemp += recordLen)) {
        if ((ptemp[0] & 0xF0) != 0xA0) { /* �жϵ�ǰ�ļ�¼�Ƿ���Ч */
            continue;
        }

        /* ����Tpye1�ļ���¼ */
        offset = SI_FindTagInBERTLV(ptemp, PB_FILE_TYPE1, recordLen);
        if (SI_PB_TAGNOTFOUND == offset) {
            PB_ERROR_LOG("SI_PB_DecodeEFPBR: Could Not Find the 0xA8 Tag");

            continue;
        }

        type1TagAddr = ptemp + (offset - 1);

        /* ȷ��������TL+T�ֶ� */
        if ((recordLen - (offset - 1)) < 3) {
            continue;
        }

        if (SI_PB_TAGNOTFOUND == SI_PB_DecodeEFPBR_AdnContent(type1TagAddr, (recordLen - (offset - 1)), i)) {
            continue;
        }

        type1EMLFlag = SI_PB_DecodeType1EFPBR_EmailContent(type1TagAddr, (recordLen - (offset - 1)), i);

        SI_PB_DecodeEFPBR_UidContent(type1TagAddr, (recordLen - (offset - 1)), i);

        SI_PB_DecodeEFPBR_PbcContent(type1TagAddr, (recordLen - (offset - 1)), i);

        SI_PB_DecodeEFPBR_IapContent(type1TagAddr, (recordLen - (offset - 1)), i);

        type1ANRFlag = SI_PB_DecodeType1EFPBR_AnrContent(type1TagAddr, (recordLen - (offset - 1)), i,
                                                           aNROffset, SI_PB_ANR_MAX);

        /* ����Tpye2�ļ���¼ */
        offset = SI_FindTagInBERTLV(ptemp, PB_FILE_TYPE2, recordLen);
        /* Type2 �����ļ������ڻ��ѽ�����Type1���� Email��Ϣ */
        if (SI_PB_TAGNOTFOUND == offset) {
            PB_NORMAL_LOG("SI_PB_DecodeEFPBR: Could Not Find the Type2 Tag");
        } else {
            type2TagAddr = (ptemp + offset) - 1;

            /* ȷ��������TL+T�ֶ� */
            if (SI_PB_TAGNOTFOUND == type1EMLFlag) {
                SI_PB_DecodeType2EFPBR_EmailContent(type2TagAddr, (recordLen - (offset - 1)), i);
            }

            if (SI_PB_TAGNOTFOUND == type1ANRFlag) {
                SI_PB_DecodeType2EFPBR_AnrContent(type2TagAddr, (recordLen - (offset - 1)), i, aNROffset,
                                                  SI_PB_ANR_MAX);
            }
        }

        /* ����Tpye3�ļ���¼ */
        offset = SI_FindTagInBERTLV(ptemp, PB_FILE_TYPE3, recordLen);
        if (SI_PB_TAGNOTFOUND == offset) {
            PB_ERROR_LOG("SI_PB_DecodeEFPBR: Could Not Find the AA Tag");

            g_pbCtrlInfo.ext1FileId                 = 0xFFFF;
            g_extContent[PB_ADN_CONTENT].extFileId = 0xFFFF;
            g_extContent[PB_ADN_CONTENT].extFlag   = SI_PB_FILE_NOT_EXIST;
        } else {
            type3TagAddr = (ptemp + offset) - 1;

            SI_PB_DecodeEFPBR_ExtContent(type3TagAddr, (recordLen - (offset - 1)));
        }
    }

    if (0 == g_pbCtrlInfo.adnFileNum) {
        return VOS_ERR;
    }

    SI_PB_DeleteSameFile();

    /* ����USIM�����绰���ļ��ĳ�ʼ�����Ȳ���SEARCH�ļ�ʵ�֣�
       ��ʼ�������г���SEARCH���ɹ�ʱתΪ������ȡ��ʼ�� */
    g_pbInitState.pbSearchState = PB_SEARCH_ENABLE;

    return VOS_OK;
}

/*
 * ��������: ����USIMMģ�������ϱ��Ŀ����ͣ�ȷ����һ����ʼ������
 * �޸���ʷ:
 * 1. ��    ��: 2008��10��31��
 *    �޸�����: Creat
 */
VOS_VOID SI_PB_DecodeEFPBCRecord(VOS_UINT8 content, VOS_UINT8 recordNum)
{
    if (1 == (content & 0x1)) {
        /* �ӵ�һ��Ԫ�ؿ�ʼ��¼ */
        g_pbInitState.pbCUpdate.record[++g_pbInitState.pbCUpdate.entryChangeNum] = recordNum;
    }

    return;
}

/*
 * ��������: ����USIMMģ�������ϱ��Ŀ����ͣ�ȷ����һ����ʼ������
 * �޸���ʷ:
 * 1. ��    ��: 2008��10��31��
 *    �޸�����: Creat
 */
VOS_VOID SI_PB_DecodeEFUIDRecord(VOS_UINT8 value1, VOS_UINT8 value2)
{
    VOS_UINT16 eFUIDValue = 0;

    eFUIDValue = value1;

    eFUIDValue <<= 8;

    eFUIDValue |= value2;

    if (eFUIDValue == 0xFFFF) {
        return;
    }

    if (g_pbCtrlInfo.uIDMaxValue < eFUIDValue) {
        g_pbCtrlInfo.uIDMaxValue = eFUIDValue;
    }

    return;
}

/*
 * ��������:�ж��Ƿ�ΪADN
 * �޸���ʷ:
 * 1. ��    ��: 2009��06��08��
 *    �޸�����: Creat
 */
VOS_UINT32 SI_PB_JudgeADNFid(VOS_UINT16 fileId)
{
    VOS_UINT32 i;

    for (i = 0; i < g_pbCtrlInfo.adnFileNum; i++) {
        if (fileId == g_pbCtrlInfo.adnInfo[i].fileId) {
            return VOS_OK;
        }
    }

    return VOS_ERR;
}

/*
 * ��������:�ж��Ƿ�Ϊָ���绰����EXT�ļ�
 * �޸���ʷ:
 * 1. ��    ��: 2009��06��08��
 *    �޸�����: Creat
 */
VOS_UINT32 SI_PB_JudgeEXTFid(VOS_UINT16 fileId, VOS_UINT16 offset)
{
    if (fileId == g_extContent[offset].extFileId) {
        return VOS_OK;
    }

    return VOS_ERR;
}

/*
 * ��������:�ж��Ƿ�ΪADN
 * �޸���ʷ:
 * 1. ��    ��: 2009��06��08��
 *    �޸�����: Creat
 */
VOS_UINT32 SI_PB_JudgeIAPFid(VOS_UINT16 fileId)
{
    VOS_UINT32 i;

    for (i = 0; i < g_pbCtrlInfo.adnFileNum; i++) {
        if (fileId == g_pbCtrlInfo.iapInfo[i].iapFileId) {
            return VOS_OK;
        }
    }

    return VOS_ERR;
}

/*
 * ��������:���ݼ�¼�Ŵ�����ṹgstPBReqUnit�л�ȡEXT�ļ��ĸ������󻺴�����
 * �޸���ʷ:
 * 1. ��    ��: 2009��06��08��
 *    �޸�����: Creat
 */
VOS_UINT32 SI_PB_GetEXTContentFromReq(VOS_UINT8 recordNum, VOS_UINT8 **ppucContent)
{
    VOS_UINT32 i;

    if (recordNum == g_pbReqUnit.extIndex) {
        *ppucContent = g_pbReqUnit.extContent;

        g_pbReqUnit.extIndex = 0; /* ��Ϊ��Чֵ */

        return VOS_OK;
    }

    for (i = 0; i < SI_PB_ANR_MAX; i++) {
        if (recordNum == g_pbReqUnit.spbReq.anrExtIndex[i]) {
            *ppucContent = g_pbReqUnit.spbReq.anrExtContent[i];

            g_pbReqUnit.spbReq.anrExtIndex[i] = 0; /* ��Ϊ��Чֵ */
            return VOS_OK;
        }
    }

    return VOS_ERR;
}

/*
 * ��������: ���������趨Bitλ��ֵ
 * �޸���ʷ:
 * 1. ��    ��: 2009��5��19��
 *    �޸�����: Creat
 */
VOS_VOID SI_PB_SetBitToBuf(VOS_UINT8 *dataBuf, VOS_UINT32 dataBufSize, VOS_UINT32 bitNo, VOS_UINT32 value)
{
    VOS_UINT32 offset;
    VOS_UINT8  bit;

    if ((VOS_NULL_PTR == dataBuf) || (bitNo == 0)) {
        PB_ERROR_LOG("SI_PB_SetBitToBuf: Input Null Ptr");

        return;
    }

    offset = (bitNo - 1) / 8;

    if (offset >= dataBufSize) {
        PB_ERROR_LOG("SI_PB_SetBitToBuf: offset is out of size");
        return;
    }

    bit = (VOS_UINT8)((bitNo - 1) % 8);

    if (0 != value) {
        dataBuf[offset] |= (VOS_UINT8)(((VOS_UINT32)0x1) << bit);
    } else {
        dataBuf[offset] &= (~(VOS_UINT8)(((VOS_UINT32)0x1) << bit));
    }

    return;
}

/*
 * ��������: �ȶ�FDN���볤��
 * �޸���ʷ:
 * 1.��    ��: 2012��2��22��
 *   �޸�����: Creat
 */
VOS_UINT32 SI_PB_CheckFdnNumLen(VOS_UINT32 fdnNumLen, VOS_UINT32 cmpNumLen)
{
    /* �洢��FDN���볤�ȴ���ҵ����룬����ʧ�� */
    if (fdnNumLen > cmpNumLen) {
        return VOS_ERR;
    }

    return VOS_OK;
}

/*
 * ��������: �ȶ�BCD��
 * �޸���ʷ:
 * 1.��    ��: 2012��3��8��
 *   �޸�����: Creat
 */
VOS_UINT32 SI_PB_BcdCompare(const VOS_UINT8 *bcdNumSrc, VOS_UINT8 bcdSrcLen, const VOS_UINT8 *fdnBcdNum,
                            VOS_UINT8 fdnBcdLen)
{
    VOS_UINT8 dstIndex;
    VOS_UINT8 fdnNumLen;
    VOS_UINT8 cmpNumLen;
    VOS_UINT8 fdnNum[SI_PB_NUM_LEN * 2] = {0};
    VOS_UINT8 cmpNum[SI_PB_NUM_LEN * 2] = {0};

    (VOS_VOID)memset_s(fdnNum, sizeof(fdnNum), 0xFF, sizeof(fdnNum));

    (VOS_VOID)memset_s(cmpNum, sizeof(cmpNum), 0xFF, sizeof(cmpNum));

    /* �绰���֧��40�����룬BCD��ĳ����20���ֽ� */
    if ((bcdSrcLen > SI_PB_NUM_LEN) || (fdnBcdLen > SI_PB_NUM_LEN)) {
        return VOS_ERR;
    }

    /* ������ĺ���ת����ASCII����ٽ��бȽ� */
    SI_PB_BcdToAscii(bcdSrcLen, bcdNumSrc, sizeof(cmpNum), cmpNum, &cmpNumLen);

    /* �������FDN����ת����ASCII����ٽ��бȽ� */
    SI_PB_BcdToAscii(fdnBcdLen, fdnBcdNum, sizeof(fdnNum), fdnNum, &fdnNumLen);

    if (VOS_OK != SI_PB_CheckFdnNumLen(fdnNumLen, cmpNumLen)) {
        return VOS_ERR;
    }

    /* ѭ���Ƚ����������ASCII�� */
    for (dstIndex = 0; dstIndex < fdnNumLen; dstIndex++) {
        /* ����'?'�����Ƚϣ��Ƚ���һ������.������������������ */
        if ((fdnNum[dstIndex] == cmpNum[dstIndex]) || ('?' == fdnNum[dstIndex])) {
            continue;
        } else {
            break;
        }
    }

    /* ����Ƚϵ���β�ж��ɹ� */
    if (fdnNumLen == dstIndex) {
        return VOS_OK;
    }

    return VOS_ERR;
}

/*
 * ��������: �ȶ�EXT2����
 * �޸���ʷ:
 * 1.��    ��: 2012��2��22��
 *   �޸�����: Creat
 */
VOS_UINT32 SI_PB_CheckEXT2(VOS_UINT8 extRecord, VOS_UINT32 extInfoNum, VOS_UINT8 *dstNum,
                           VOS_UINT8 contentSize, VOS_UINT8 *exNumLen)
{
    VOS_UINT8 *exTemp = VOS_NULL_PTR;
    VOS_UINT8  exTempNumLen;
    errno_t    ret;

    exTemp = g_extContent[extInfoNum].extContent;

    if (VOS_NULL_PTR == exTemp) {
        PB_ERROR_LOG("SI_PB_CheckFdn: g_extContent is null.\r\n");
        return VOS_ERR;
    }

    if ((extRecord > g_extContent[extInfoNum].extTotalNum) || (0 == extRecord)) {
        PB_ERROR1_LOG("SI_PB_CheckFdn: ucExtRecord: %d is wrong .\r\n", extRecord);
        return VOS_ERR;
    }

    exTemp += (extRecord - 1) * SI_PB_EXT_LEN;

    /* �ж�EX���볤�� */
    if ((0xFF == *(exTemp + 1)) || (0x0 == *(exTemp + 1))) {
        PB_ERROR_LOG("SI_PB_CheckFdn: EXT NUM Length is error.\r\n");
        return VOS_ERR;
    }

    exTempNumLen = (*(exTemp + 1) > (SI_PB_NUM_LEN / 2)) ? (SI_PB_NUM_LEN / 2) : *(exTemp + 1);

    ret = memcpy_s(dstNum, contentSize, (exTemp + 2), exTempNumLen);

    PAM_PRINT_SECFUN_RESULT(ret);

    *exNumLen = exTempNumLen;

    return VOS_OK;
}

/*
 * ��������: �ȶ�FDN����
 * �޸���ʷ:
 * 1.��    ��: 2012��2��22��
 *   �޸�����: Creat
 */
VOS_UINT32 SI_PB_CheckFdn(const VOS_UINT8 *num, VOS_UINT32 numLength)
{
    VOS_UINT32 result;
    VOS_UINT8  pbOffset = 0;
    VOS_UINT16 i;
    VOS_UINT8 *temp                    = VOS_NULL_PTR;
    VOS_UINT8 *fdnContent            = VOS_NULL_PTR;
    VOS_UINT8  number[SI_PB_NUM_LEN] = {0};
    VOS_UINT16 totalNum;
    VOS_UINT8  numLen;
    VOS_UINT8  exNumLen;
    VOS_UINT32 extInfoNum;
    VOS_UINT8  extRecord;
    errno_t    ret;

    (VOS_VOID)memset_s(number, SI_PB_NUM_LEN, 0xFF, SI_PB_NUM_LEN);
    numLen = 0;

    /* ��ǰ�绰�������ڻ��߳�ʼ��δ��� */
    result = SI_PB_LocateRecord(PB_FDN, 1, 1, &pbOffset);
    if (VOS_OK != result) {
        PB_ERROR_LOG("SI_PB_CheckFdn: SI_PB_LocateRecord Return Failed.\r\n");
        return VOS_ERR;
    }

    totalNum    = g_pbContent[pbOffset].totalNum;
    fdnContent = g_pbContent[pbOffset].content;

    if (VOS_NULL_PTR == fdnContent) {
        PB_ERROR_LOG("SI_PB_CheckFdn: g_pbContent is null.\r\n");
        return VOS_ERR;
    }

    extInfoNum = g_pbContent[pbOffset].extInfoNum;

    /* �ڵ绰����¼��ѭ�������ַ���ƥ�䣬һ���ɹ�����VOS_OK */
    for (i = 0; i < totalNum; i++) { /* �������ݽṹ��󳤶�ѭ�� */
        temp = fdnContent;
        temp += g_pbContent[pbOffset].nameLen;

        /* �����볤���Ƿ�Ϸ� */
        if ((0xFF == *temp) || (*temp < 2)) {
            fdnContent += g_pbContent[pbOffset].recordLen;

            continue;
        }

        numLen = *temp - 1;

        numLen = (numLen > (SI_PB_NUM_LEN / 2)) ? (SI_PB_NUM_LEN / 2) : numLen;

        (VOS_VOID)memset_s(number, SI_PB_NUM_LEN, 0xFF, SI_PB_NUM_LEN);

        ret = memcpy_s(number, SI_PB_NUM_LEN, temp + 2, numLen);

        PAM_PRINT_SECFUN_RESULT(ret);

        extRecord = fdnContent[g_pbContent[pbOffset].recordLen - 1];

        /* ����EX�ļ� */
        if (0xFF != extRecord) {
            if (SI_PB_CheckEXT2(extRecord, extInfoNum, &(number[numLen]), SI_PB_NUM_LEN - numLen,
                                &exNumLen) == VOS_OK) {
                numLen += exNumLen;
            }
        }

        /* �ȶ��ַ� */
        if (VOS_OK == SI_PB_BcdCompare(num, (VOS_UINT8)numLength, number, numLen)) {
            PB_ERROR_LOG("SI_PB_CheckFdn: SI_PB_BcdCompare ok");

            return VOS_OK;
        }

        fdnContent += g_pbContent[pbOffset].recordLen;
    }

    return VOS_ERR;
}

/*
 * ��������: ECALL���������
 * �޸���ʷ:
 * 1.��    ��: 2014��04��25��
 *   �޸�����: Creat
 */
/*lint -save -e958 */
VOS_UINT32 SI_PB_EcallNumberErrProc(SI_PB_StorateTypeUint32 storage, VOS_UINT8 listLen, const VOS_UINT8 *list,
                                    VOS_UINT8 *pbOffset)
{
    VOS_UINT8 i;
    VOS_UINT8 ecallPBOffset;

    if (VOS_OK != SI_PB_FindPBOffset(storage, pbOffset)) {
        PB_WARNING_LOG("SI_PB_EcallNumberErrProc: Locate PhoneBook Error");

        return TAF_ERR_ERROR;
    }

    ecallPBOffset = *pbOffset;

    /* ��ǰ�ĵ绰��û�г�ʼ����� */
    if (PB_INITIALISED != g_pbContent[ecallPBOffset].initialState) {
        PB_ERROR_LOG("SI_PB_EcallNumberErrProc:The PhoneBook is Not Initializtion");

        return TAF_ERR_SIM_BUSY;
    }

    /* ��ǰ�ĵ绰��û������ */
    if (VOS_NULL_PTR == g_pbContent[ecallPBOffset].content) {
        PB_ERROR_LOG("SI_PB_EcallNumberErrProc:The PhoneBook is Not Initializtion");

        return TAF_ERR_PB_NOT_INIT;
    }

    /* �����index���ܳ�������¼����Ϊ0 */
    for (i = 0; i < listLen; i++) {
        if ((list[i] > g_pbContent[ecallPBOffset].totalNum) || (VOS_NULL == list[i])) {
            PB_ERROR_LOG("SI_PB_EcallNumberErrProc:Wrong index");

            return TAF_ERR_PB_WRONG_INDEX;
        }
    }

    return TAF_ERR_NO_ERROR;
}
/*lint -restore */
/*
 * ��������: ��ȡECALL����
 * �޸���ʷ:
 * 1.��    ��: 2014��04��25��
 *   �޸�����: Creat
 */
/*lint -save -e958 */
VOS_VOID SI_PB_GetEcallNumber(const SI_PB_Content *xdnContent, SI_PB_EcallNum *ecallNum, VOS_UINT8 listLen,
                              const VOS_UINT8 *list)
{
    VOS_UINT8 *content    = VOS_NULL_PTR;
    VOS_UINT8 *tmp        = VOS_NULL_PTR;
    VOS_UINT8 *extContent = VOS_NULL_PTR;
    VOS_UINT32 extInfoNum;
    VOS_UINT8  recordLen;
    VOS_UINT8  exNumLen;
    VOS_UINT8  extRecord;
    VOS_UINT8  i;
    errno_t    ret;

    if (xdnContent->content == VOS_NULL_PTR) {
        PB_WARNING_LOG("SI_PB_GetEcallNumber: xdn content is NULL");

        return;
    }

    content  = xdnContent->content;
    recordLen = xdnContent->recordLen;

    for (i = 0; i < listLen; i++) {
        ecallNum->index = list[i];

        (VOS_VOID)memset_s(ecallNum->callNumber, SI_PB_NUM_LEN, 0xFF, SI_PB_NUM_LEN);
        /*lint -save -e961 */
        tmp = content + (xdnContent->recordLen) * (list[i] - 1);
        /*lint -restore */
        extRecord   = tmp[recordLen - 1];
        extInfoNum  = xdnContent->extInfoNum;
        extContent = g_extContent[extInfoNum].extContent;

        extRecord = ((VOS_NULL_PTR == extContent) ? 0xFF : (extRecord));

        if ((tmp[xdnContent->nameLen] < 2) || (0xFF == tmp[xdnContent->nameLen])) {
            ecallNum->length = 0;
        } else if ((0xFF != extRecord) && (0 != extRecord) && (VOS_NULL_PTR != extContent) &&
                   (extRecord <= g_extContent[extInfoNum].extTotalNum)) {
            extContent += (extRecord - 1) * SI_PB_EXT_LEN;
            exNumLen = (*(extContent + 1) > (SI_PB_NUM_LEN / 2)) ? (SI_PB_NUM_LEN / 2) : *(extContent + 1);

            ret = memcpy_s(ecallNum->callNumber, (SI_PB_NUM_LEN / 2), tmp + xdnContent->nameLen + 2,
                           (SI_PB_NUM_LEN / 2));

            PAM_PRINT_SECFUN_RESULT(ret);

            ret = memcpy_s(ecallNum->callNumber + (SI_PB_NUM_LEN / 2), (SI_PB_NUM_LEN / 2), extContent + 2,
                           (SI_PB_NUM_LEN / 2));

            PAM_PRINT_SECFUN_RESULT(ret);

            ecallNum->ton    = tmp[xdnContent->nameLen + 1];
            ecallNum->length = (SI_PB_NUM_LEN / 2) + exNumLen;
        } else {
            ret = memcpy_s(ecallNum->callNumber, (SI_PB_NUM_LEN / 2), tmp + xdnContent->nameLen + 2,
                           (SI_PB_NUM_LEN / 2));

            PAM_PRINT_SECFUN_RESULT(ret);

            ecallNum->ton    = tmp[xdnContent->nameLen + 1];
            ecallNum->length = ((tmp[xdnContent->nameLen] - 1) > (SI_PB_NUM_LEN / 2))
                                    ? (SI_PB_NUM_LEN / 2)
                                    : (tmp[xdnContent->nameLen] - 1);
        }

        ecallNum++;
    }

    return;
}
/*lint -restore */

/*
 * ��������: �Ƿ����EXT�ļ�
 * �޸���ʷ:
 * 1.��    ��: 2019��05��27��
 *   �޸�����: Creat
 */
VOS_UINT32 SI_PB_IsExsitExtFile(VOS_UINT8 extIndexValue, VOS_UINT32 extInfoNum)
{
    /* ����Ƿ���EXT�ļ�Ҫ��ȡ��XDN�ļ�����չ��¼�ļ�¼�Ų���Ϊ0��0xFF */
    if ((extIndexValue != 0xFF) && (extIndexValue != 0) &&
        (g_extContent[extInfoNum].extFlag != SI_PB_FILE_NOT_EXIST)) {
        return VOS_OK;
    }

    return VOS_ERR;
}

#endif

#if ((OSA_CPU_ACPU == VOS_OSA_CPU) || (defined(DMT)) || (FEATURE_ACORE_MODULE_TO_CCORE == FEATURE_ON))
/*
 * ��������:ת����ǰ��һ��ANR�绰��Ϊָ�������ݽṹ
 * �޸���ʷ:
 * 1. ��    ��: 2009��06��08��
 *    �޸�����: Creat
 */
VOS_VOID SI_PB_TransANRFromate(VOS_UINT8 aNROffset, const VOS_UINT8 *anrContent, VOS_UINT32 anrContentLen, SI_PB_Record *record)
{
    VOS_UINT32 result;
    VOS_UINT8  extRecord;
    VOS_UINT8  phoneNumber[SI_PB_NUM_LEN] = {0};
    VOS_UINT8 *extContent                 = VOS_NULL_PTR;
    VOS_UINT8  extNumLen;
    VOS_UINT8  anrNumLen;
    errno_t    ret;

    /* �����ж���Ҫ��ȥ2���ֽڵĿ�ѡ�ֶ� */
    if (anrContentLen < (SI_PB_ANR_LEN - 2)) {
        return;
    }

    result = SI_PB_CheckANRValidity(anrContent[0], anrContent[1]); /* ��鵱ǰ�������Ƿ���Ч */

    if (result != VOS_OK) {
        /* ȫ��Ϊ0 */
        record->additionNumber[aNROffset].numberLength = 0;
    } else {
        record->validFlag = SI_PB_CONTENT_VALID; /* ��ǵ�ǰ��������Ч */

        record->additionNumber[aNROffset].numberType = anrContent[2];

        extContent = g_extContent[PB_ADN_CONTENT].extContent;

        extRecord = ((VOS_NULL_PTR == extContent) ? 0xFF : anrContent[14]);

        if ((0xFF != extRecord) && (VOS_NULL_PTR != extContent) &&
            (extRecord <= g_extContent[PB_ADN_CONTENT].extTotalNum)) {
            extContent += (extRecord - 1) * SI_PB_EXT_LEN;
            extNumLen = (*(extContent + 1) > (SI_PB_NUM_LEN / 2)) ? (SI_PB_NUM_LEN / 2) : *(extContent + 1);

            ret = memcpy_s(phoneNumber, SI_PB_NUM_LEN, anrContent + 3, SI_PB_NUM_LEN / 2);

            PAM_PRINT_SECFUN_RESULT(ret);

            ret = memcpy_s(phoneNumber + (SI_PB_NUM_LEN / 2), SI_PB_NUM_LEN / 2, extContent + 2,
                           SI_PB_NUM_LEN / 2);

            PAM_PRINT_SECFUN_RESULT(ret);

            SI_PB_BcdToAscii((VOS_UINT8)(extNumLen + (SI_PB_NUM_LEN / 2)), phoneNumber,
                             sizeof(record->additionNumber[aNROffset].number),
                             record->additionNumber[aNROffset].number,
                             &record->additionNumber[aNROffset].numberLength);
        } else { /* ������������ݿ�ʼ */
            anrNumLen = ((anrContent[1] - 1) > (SI_PB_NUM_LEN / 2)) ? (SI_PB_NUM_LEN / 2) : (anrContent[1] - 1);

            SI_PB_BcdToAscii(anrNumLen, &anrContent[3], sizeof(record->additionNumber[aNROffset].number),
                             record->additionNumber[aNROffset].number,
                             &record->additionNumber[aNROffset].numberLength);
        }
    }

    return;
}

/*
 * ��������:ת��Email�ļ�����Ϊָ�������ݽṹ
 * �޸���ʷ:
 * 1. ��    ��: 2009��06��08��
 *    �޸�����: Creat
 */
VOS_VOID SI_PB_TransEMLFromate(VOS_UINT8 emailMaxLen, const VOS_UINT8 *emlContent, SI_PB_Record *record)
{
    VOS_UINT32 i;
    VOS_UINT8  emailLen = 0;

    for (i = 0; i < emailMaxLen; i++) {
        if (0xFF == emlContent[i]) {
            break;
        }

        emailLen++;
    }

    if (0 == emailLen) {
        record->email.emailLen = 0;
    } else {
        record->email.emailLen = emailLen;

        record->validFlag = SI_PB_CONTENT_VALID;

        if (memcpy_s(record->email.email, SI_PB_EMAIL_MAX_LEN, emlContent, emailLen) != EOK) {
            record->email.emailLen = 0;
            record->validFlag      = SI_PB_CONTENT_INVALID;

            PB_WARNING_LOG("SI_PB_TransEMLFromate: memcpy_s fail");
        }
    }

    return;
}

/*
 * ��������: ���������INDEX��ֵ�õ���Ӧ��EMAIL��ANR�ļ���ƫ�ƣ���1��ʼ
 * �޸���ʷ:
 * 1. ��    ��: 2009��5��19��
 *    �޸�����: Creat
 */
VOS_UINT32 SI_PB_GetFileCntFromIndex(VOS_UINT16 index, VOS_UINT8 *fileCnt)
{
    VOS_UINT32 i;
    VOS_UINT16 sum;

    sum = 0;

    if (SI_PB_ADNMAX < g_pbCtrlInfo.adnFileNum) {
        PB_WARNING1_LOG("SI_PB_SearchResultProc: g_pbCtrlInfo.adnFileNum err", g_pbCtrlInfo.adnFileNum);
        return VOS_ERR;
    }

    for (i = 0; i < g_pbCtrlInfo.adnFileNum; i++) {
        if ((index <= (sum + g_pbCtrlInfo.adnInfo[i].recordNum)) && (index > sum)) {
            break;
        } else {
            sum += g_pbCtrlInfo.adnInfo[i].recordNum;
        }
    }

    if (i >= g_pbCtrlInfo.adnFileNum) {
        *fileCnt = 0;
        return VOS_ERR;
    } else {
        *fileCnt = i + 1;
        return VOS_OK;
    }
}

#endif

#endif /* (FEATURE_PHONE_USIM == FEATURE_ON) */

