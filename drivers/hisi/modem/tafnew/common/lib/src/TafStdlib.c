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

#include "TafStdlib.h"
#include "mn_error_code.h"
#include "mn_comm_api.h"
#include "mn_msg_api.h"
#include "MnMsgTs.h"
#include "securec.h"
#include "mdrv_crypto.h"


#define THIS_FILE_ID PS_FILE_ID_TAF_STD_LIB_C

LOCAL TAF_ERROR_CodeUint32 TAF_STD_DecodeUtf8CharacterFirstByte(const VOS_UINT8 *utf8FirstByte,
                                                                VOS_UINT16      *utf8CharacterContent,
                                                                VOS_UINT32      *utf8CharacterByteNum);

LOCAL VOS_UINT8 g_msgAsciiSfxDefAlpha[TAF_STD_MAX_GSM7BITDEFALPHA_NUM] = {
    '@',  163,  '$',  165,  232,  233,  249,  236,  242,  199, 0x0a, 216, 248, 0x0d, 197, 229, 0xff, '_', 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 198, 230,  223, 201, ' ',  '!', '"', '#',  164, '%',
    '&',  0x27, '(',  ')',  '*',  '+',  ',',  '-',  '.',  '/', '0',  '1', '2', '3',  '4', '5', '6',  '7', '8',
    '9',  ':',  ';',  '<',  '=',  '>',  '?',  161,  'A',  'B', 'C',  'D', 'E', 'F',  'G', 'H', 'I',  'J', 'K',
    'L',  'M',  'N',  'O',  'P',  'Q',  'R',  'S',  'T',  'U', 'V',  'W', 'X', 'Y',  'Z', 196, 214,  209, 220,
    167,  191,  'a',  'b',  'c',  'd',  'e',  'f',  'g',  'h', 'i',  'j', 'k', 'l',  'm', 'n', 'o',  'p', 'q',
    'r',  's',  't',  'u',  'v',  'w',  'x',  'y',  'z',  228, 246,  241, 252, 224
};

LOCAL VOS_UINT8 g_msgAsciiSfxDefAlphaExt[TAF_STD_MAX_GSM7BITDEFALPHA_NUM] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0c, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, '^',  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, '{',  '}',  0xff, 0xff, 0xff, 0xff, 0xff, '\\', 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, '[',  '~',  ']',  0xff, '|',  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
};

/* ��1980�꿪ʼ��������Ϊ��λ��ÿ������� */
static const VOS_UINT16 g_tafStdDaysElapsedOfALeapYearSetTab[] = {
    0,                    /* �� 0 (����) */
    366,                  /* �� 1        */
    366 + 365,            /* �� 2        */
    366 + 365 + 365,      /* �� 3        */
    366 + 365 + 365 + 365 /* ��Ԫ������  */
};

/* ƽ����ÿ�µ����� */
static const VOS_UINT16 g_tafStdNormMonthTab[] = {
    0,                                                        /* ---    */
    31,                                                       /* һ��   */
    31 + 28,                                                  /* ����   */
    31 + 28 + 31,                                             /* ����   */
    31 + 28 + 31 + 30,                                        /* ����   */
    31 + 28 + 31 + 30 + 31,                                   /* ����   */
    31 + 28 + 31 + 30 + 31 + 30,                              /* ����   */
    31 + 28 + 31 + 30 + 31 + 30 + 31,                         /* ����   */
    31 + 28 + 31 + 30 + 31 + 30 + 31 + 31,                    /* ����   */
    31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30,               /* ����   */
    31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31,          /* ʮ��   */
    31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30,     /* ʮһ�� */
    31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30 + 31 /* ʮ���� */
};

/* ������ÿ�µ����� */
static const VOS_UINT16 g_tafStdLeapMonthTab[] = {
    0,                                                        /* ---    */
    31,                                                       /* һ��   */
    31 + 29,                                                  /* ����   */
    31 + 29 + 31,                                             /* ����   */
    31 + 29 + 31 + 30,                                        /* ����   */
    31 + 29 + 31 + 30 + 31,                                   /* ����   */
    31 + 29 + 31 + 30 + 31 + 30,                              /* ����   */
    31 + 29 + 31 + 30 + 31 + 30 + 31,                         /* ����   */
    31 + 29 + 31 + 30 + 31 + 30 + 31 + 31,                    /* ����   */
    31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30,               /* ����   */
    31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31,          /* ʮ��   */
    31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30,     /* ʮһ�� */
    31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30 + 31 /* ʮ���� */
};

static const VOS_UINT16 g_monthDay[2][TAF_STD_MAX_MONTH_DAY_COUNT] = {
    { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 }, /* normal year */
    { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 }  /* leap year */
};


VOS_UINT32 TAF_STD_Itoa(VOS_UINT32 digit, VOS_CHAR *pcDigitStr, VOS_UINT32 stringlength, VOS_UINT32 *printLength)
{
    VOS_INT32 charNum;

    if (stringlength < 1) {
        MN_INFO_LOG("TAF_STD_Itoa stringlength less than 1");
        *printLength = 0;
        return VOS_FALSE;
    }

    charNum = snprintf_s(pcDigitStr, stringlength, stringlength - 1, "%d", (VOS_INT)digit);

    if (charNum < 0) {
        MN_INFO_LOG("TAF_STD_Itoa snprintf_s fail");

        *printLength = 0;

        return VOS_FALSE;
    }

    *printLength = (VOS_UINT32)charNum;

    return VOS_TRUE;
}



VOS_UINT32 TAF_STD_AsciiNum2HexString(VOS_UINT8 *src, VOS_UINT16 *srcLen)
{
    VOS_UINT16 chkLen;
    VOS_UINT8  tmp;
    VOS_UINT8  left;
    VOS_UINT16 srcLenTemp;
    VOS_UINT8 *dst = VOS_NULL_PTR;

    chkLen     = 0;
    tmp        = 0;
    left       = 0;
    srcLenTemp = *srcLen;
    dst        = src;

    /* ��������������ֽ��򷵻ش��� */
    if ((srcLenTemp % 2) != 0) {
        return VOS_FALSE;
    }

    while (chkLen < srcLenTemp) {
        /* the number is 0-9 */
        if (isdigit(src[chkLen])) {
            left = src[chkLen] - '0';
        }
        /* the number is a-f */
        else if ((src[chkLen] >= 'a') && (src[chkLen] <= 'f')) {
            left = (src[chkLen] - 'a') + 0x0a;
        }
        /* the number is A-F */
        else if ((src[chkLen] >= 'A') && (src[chkLen] <= 'F')) {
            left = (src[chkLen] - 'A') + 0x0a;
        } else {
            return VOS_FALSE;
        }

        dst[tmp] = 0xf0 & (left << 4);
        chkLen += 1;

        /* the number is 0-9 */
        if (isdigit(src[chkLen])) {
            dst[tmp] |= src[chkLen] - '0';
        }
        /* the number is a-f */
        else if ((src[chkLen] >= 'a') && (src[chkLen] <= 'f')) {
            dst[tmp] |= (src[chkLen] - 'a') + 0x0a;
        }
        /* the number is A-F */
        else if ((src[chkLen] >= 'A') && (src[chkLen] <= 'F')) {
            dst[tmp] |= (src[chkLen] - 'A') + 0x0a;
        } else {
            return VOS_FALSE;
        }

        chkLen += 1;
        tmp += 1;
    }

    *srcLen = srcLenTemp / 2;

    return VOS_TRUE;
}


VOS_UINT16 TAF_STD_HexAlpha2AsciiString(VOS_UINT8 *src, VOS_UINT16 srcLen, VOS_UINT8 *dst)
{
    VOS_UINT16 len;
    VOS_UINT16 chkLen;
    VOS_UINT8  high;
    VOS_UINT8  low;
    VOS_UINT8 *write = VOS_NULL_PTR;
    VOS_UINT8 *read  = VOS_NULL_PTR;

    write  = dst;
    read   = src;
    len    = 0;
    chkLen = 0;
    high   = 0;
    low    = 0;

    /* ɨ�������ִ� */
    while (chkLen++ < srcLen) {
        high = 0x0F & (*read >> 4);
        low  = 0x0F & *read;

        len += 2; /* ��¼���� */

        if (high <= 0x09) { /* 0-9 */
            *write++ = high + 0x30;
        } else if (high >= 0x0A) { /* A-F */
            *write++ = high + 0x37;
        } else {
        }

        if (low <= 0x09) { /* 0-9 */
            *write++ = low + 0x30;
        } else if (low >= 0x0A) { /* A-F */
            *write++ = low + 0x37;
        } else {
        }

        /* ��һ���ַ� */
        read++;
    }

    return len;
}


VOS_UINT32 TAF_STD_ConvertStrToDecInt(VOS_UINT8 *src, VOS_UINT32 srcLen, VOS_UINT32 *dec)
{
    VOS_UINT32 tmp;
    VOS_UINT32 ultotal;
    VOS_UINT8  length;

    tmp     = 0;
    ultotal = 0;
    length  = 0;

    tmp = (VOS_UINT32)*src++;

    while (length < srcLen) {
        length++;
        if ((tmp >= '0') && (tmp <= '9')) {
            tmp = tmp - '0';
        } else {
            return VOS_FALSE;
        }

        if (ultotal > 0x0FFFFFFF) {
            return VOS_FALSE;
        } else {
            ultotal = (ultotal * 10) + tmp;
            tmp     = (VOS_UINT32)(*(src));
            src++;
        }
    }

    *dec = ultotal;
    return VOS_TRUE;
}

/* MN_UnPack7Bit��MnMsgDecode.c�Ƶ����ļ�������ΪTAF_STD_UnPack7Bit */
/* MN_Pack7Bit��MnMsgEncode.c�Ƶ����ļ�������ΪTAF_STD_Pack7Bit */


MODULE_EXPORTED VOS_UINT32 TAF_STD_UnPack7Bit(const VOS_UINT8 *orgChar, VOS_UINT32 len, VOS_UINT8 fillBit,
                                              VOS_UINT8 *unPackedChar)
{
    /* ����ֽڵ�ַ */
    VOS_UINT32 pos = 0;
    /* ���λƫ�� */
    VOS_UINT32 offset;
    VOS_UINT32 loop;

    if ((orgChar == VOS_NULL_PTR) || (unPackedChar == VOS_NULL_PTR)) {
        return VOS_ERR;
    }

    /* ����Э��23040 9.2.3.24 UDHL��UDH������Fill Bits��SM��ȥ��Fill Bits�����SM(Unit: Septet),���Ի��SM�а����ַ����� */
    offset = fillBit % 8;

    /*
     * ��һ�����Ƴ���ǰ��Ч��ƫ��λulOffset���õ��ַ��ĵ�(8 - ulOffset)λ��
     * �ڶ�������(8 - ulOffset)С��7λ����Ҫ����һ��OCTET�л�ȡ��(7 - (8 - ulOffset))λ
     * ����������ȡ��һ������Դ���±�(ulPos)����Ҫȥ��������λ(ƫ��λulOffset)
     */
    for (loop = 0; loop < len; loop++) {
        unPackedChar[loop] = (VOS_UINT8)(orgChar[pos] >> offset);
        if (offset > 1) {
            /*lint -e701*/
            unPackedChar[loop] |= (VOS_UINT8)((orgChar[pos + 1UL] << (VOS_UINT32)(8 - offset)) & TAF_STD_7BIT_MASK);
            /*lint +e701*/
        } else {
            unPackedChar[loop] &= TAF_STD_7BIT_MASK;
        }

        pos += (offset + 7) / 8;
        offset = (offset + 7) % 8;
    }

    return VOS_OK;
}


MODULE_EXPORTED VOS_UINT32 TAF_STD_Pack7Bit(const VOS_UINT8 *orgChar, VOS_UINT32 len, VOS_UINT8 fillBit,
                                            VOS_UINT8 *packedChar, VOS_UINT32 *lenValue)
{
    /* ����ֽڵ�ַ */
    VOS_UINT32 pos = 0;
    /* ���λƫ�� */
    VOS_UINT32 offset;
    VOS_UINT32 loop;

    if ((orgChar == VOS_NULL_PTR) || (packedChar == VOS_NULL_PTR) || (lenValue == VOS_NULL_PTR)) {
        return VOS_ERR;
    }

    *lenValue = ((VOS_UINT32)(((len * 7) + fillBit) + 7) / 8);

    offset = fillBit % 8;

    /* bit 7   6   5   4   3   2   1   0 */
    /*    |digit1L|   |---ulOffset1---| */ /* ����ulOffset1λ */
    /*                |(0)  digit1H   | */ /* ����(8-ulOffset1λ) */
    /*    |-digit2L-  |   |-ulOffset2-| */ /* ƫ����Ϊ(8-1+ulOffset1)%8 */

    /*
     * ��һ�����ճ���ǰ�Ѿ�����ƫ��λulOffset����������Դ��ȡ��һ��OCTET����(8 - ulOffset)λ��
     * �ڶ�������(8 - ulOffset)С��7λ����Ҫ����ǰ����Դ�ַ����¸�(7 - (8 - ulOffset))λ��䵽Ŀ�����ݵ���һ��OCTET��
     * ����������ȡ��һ��Ŀ�����ݵ��±�(ulPos)���Ѿ���������λ(ƫ��λulOffset)
     */
    for (loop = 0; loop < len; loop++) {
        if ((orgChar[loop] & (~TAF_STD_7BIT_MASK)) != 0) {
            return VOS_ERR;
        }

        packedChar[pos] |= (VOS_UINT8)((VOS_UINT32)(orgChar[loop] & TAF_STD_7BIT_MASK) << offset);
        if (offset > 1) {
            packedChar[pos + 1UL] = (VOS_UINT8)((orgChar[loop] & TAF_STD_7BIT_MASK) >> (8 - offset));
        }

        pos += (offset + 7) / 8;
        offset = (offset + 7) % 8;
    }

    return VOS_OK;
}


VOS_UINT32 TAF_STD_ConvertBcdNumberToAscii(const VOS_UINT8 *bcdNumber, VOS_UINT8 bcdLen, VOS_CHAR *pcAsciiNumber,
                                           VOS_UINT32 asciiLen)
{
    VOS_UINT32 loop;
    VOS_UINT32 ret;
    VOS_UINT8  len;
    VOS_UINT8  bcdCode;

    if ((bcdNumber == VOS_NULL_PTR) || (pcAsciiNumber == VOS_NULL_PTR)) {
        return MN_ERR_NULLPTR;
    }

    /* ��������ַ�����ȥ����Ч��0XFF���� */
    while (bcdLen > 1) {
        if (bcdNumber[bcdLen - 1] == 0xFF) {
            bcdLen--;
        } else {
            break;
        }
    }

    if (bcdLen == 0) {
        return MN_ERR_INVALIDPARM;
    }

    /*
     * �ж�pucBcdAddress��ָ����ַ��������һ���ֽڵĸ�λ�Ƿ�Ϊ1111��
     * ����ǣ�˵������λ��Ϊ����������Ϊż��
     */
    if ((bcdNumber[bcdLen - 1] & 0xF0) == 0xF0) {
        len = (VOS_UINT8)((bcdLen * 2) - 1);
    } else {
        len = (VOS_UINT8)(bcdLen * 2);
    }

    if (len > asciiLen) {
        return MN_ERR_INVALIDPARM;
    }

    /* �������� */
    for (loop = 0; loop < len; loop++) {
        /* �жϵ�ǰ�����������λ���뻹��ż��λ���룬��0��ʼ����ż�� */
        if ((loop % 2) == 1) {
            /* ���������λ���룬��ȡ��4λ��ֵ */
            bcdCode = ((bcdNumber[(loop / 2)] >> 4) & 0x0F);
        } else {
            /* �����ż��λ���룬��ȡ��4λ��ֵ */
            bcdCode = (bcdNumber[(loop / 2)] & 0x0F);
        }

        /* ������������ת����Ascii����ʽ */
        ret = TAF_STD_ConvertBcdCodeToAscii(bcdCode, &(pcAsciiNumber[loop]));
        if (ret != MN_ERR_NO_ERROR) {
            return ret;
        }
    }

    pcAsciiNumber[loop] = '\0'; /* �ַ���ĩβΪ0 */

    return MN_ERR_NO_ERROR;
}


VOS_UINT32 TAF_STD_ConvertBcdCodeToAscii(VOS_UINT8 bcdCode, VOS_CHAR *pcAsciiCode)
{
    VOS_CHAR asciiCode;

    if (pcAsciiCode == VOS_NULL_PTR) {
        return MN_ERR_NULLPTR;
    }

    if (bcdCode <= 0x09) {
        asciiCode = (VOS_CHAR)(bcdCode + 0x30);
    } else if (bcdCode == 0x0A) {
        asciiCode = (VOS_CHAR)(bcdCode + 0x20); /* �ַ�'*' */
    } else if (bcdCode == 0x0B) {
        asciiCode = (VOS_CHAR)(bcdCode + 0x18); /* �ַ�'#' */
    } else if ((bcdCode == 0x0C) || (bcdCode == 0x0D) || (bcdCode == 0x0E)) {
        asciiCode = (VOS_CHAR)(bcdCode + 0x55); /* �ַ�'a', 'b', 'c' */
    } else {
        return MN_ERR_INVALID_BCD;
    }

    *pcAsciiCode = asciiCode;

    return MN_ERR_NO_ERROR;
}


VOS_UINT32 TAF_STD_ConvertAsciiNumberToBcd(const VOS_CHAR *pcAsciiNumber, VOS_UINT8 *bcdNumber, VOS_UINT8 *bcdLen)
{
    VOS_UINT32 ret;
    VOS_UINT8  loop;
    VOS_UINT8  bcdCode;

    if ((pcAsciiNumber == VOS_NULL_PTR) || (bcdNumber == VOS_NULL_PTR) || (bcdLen == VOS_NULL_PTR)) {
        return MN_ERR_NULLPTR;
    }

    for (loop = 0; pcAsciiNumber[loop] != '\0'; loop++) {
        ret = TAF_STD_ConvertAsciiCodeToBcd(pcAsciiNumber[loop], &bcdCode);
        if (ret != MN_ERR_NO_ERROR) {
            return ret;
        }

        /* ����ǰ��Ҫ����Ŀռ���0 */
        bcdNumber[(loop / 2)] &= (VOS_UINT8)(((loop % 2) == 1) ? 0x0F : 0xF0);

        /* ������������Ӧ�Ŀռ� */
        bcdNumber[(loop / 2)] |= (VOS_UINT8)(((loop % 2) == 1) ? ((bcdCode << 4) & 0xF0) : (bcdCode & 0x0F));
    }

    /* �������Ϊ�����������һ���ַ���Ҫ�� F */
    if ((loop % 2) == 1) {
        bcdNumber[(loop / 2)] |= (VOS_UINT8)0xF0;
    }

    *bcdLen = (loop + 1) / 2;

    return MN_ERR_NO_ERROR;
}


VOS_UINT32 TAF_STD_ConvertAsciiAddrToBcd(const MN_MSG_AsciiAddr *asciiAddr, MN_MSG_BcdAddr *bcdAddr)
{
    VOS_UINT32 ret;

    if ((asciiAddr == VOS_NULL_PTR) || (bcdAddr == VOS_NULL_PTR)) {
        return MN_ERR_NULLPTR;
    }

    MN_GET_ADDRTYPE(bcdAddr->addrType, asciiAddr->numType, asciiAddr->numPlan);

    ret = TAF_STD_ConvertAsciiNumberToBcd((VOS_CHAR *)(asciiAddr->asciiNum), bcdAddr->bcdNum, &bcdAddr->bcdLen);

    return ret;
}


VOS_UINT32 TAF_STD_ConvertAsciiCodeToBcd(VOS_CHAR asciiCode, VOS_UINT8 *bcdCode)
{
    if (bcdCode == VOS_NULL_PTR) {
        return MN_ERR_NULLPTR;
    }

    if ((asciiCode >= '0') && (asciiCode <= '9')) {
        *bcdCode = (VOS_UINT8)(asciiCode - '0');
    } else if (asciiCode == '*') {
        *bcdCode = 0x0a;
    } else if (asciiCode == '#') {
        *bcdCode = 0x0b;
    } else if (('a' == asciiCode) || ('b' == asciiCode) || ('c' == asciiCode)) {
        *bcdCode = (VOS_UINT8)((asciiCode - 'a') + 0x0c);
    } else {
        return MN_ERR_INVALID_ASCII;
    }

    return MN_ERR_NO_ERROR;
}


VOS_UINT8 TAF_STD_ConvertDeciDigitToBcd(VOS_UINT8 deciDigit, VOS_BOOL bReverseOrder)
{
    VOS_UINT8 bcd;

    if (bReverseOrder == VOS_TRUE) {
        /* ��4BIT�洢ʮ�������ĸ�λ */
        bcd = deciDigit / 10;
        /* ��4BIT�洢ʮ�������ĵ�λ */
        bcd |= (deciDigit % 10) << 4;
    } else {
        /* ��4BIT�洢ʮ�������ĵ�λ */
        bcd = deciDigit % 10;
        /* ��4BIT�洢ʮ�������ĸ�λ */
        bcd |= (VOS_UINT8)((deciDigit / 10) << 4);
    }

    return bcd;
}


VOS_UINT32 TAF_STD_ConvertBcdToDeciDigit(VOS_UINT8 bcdDigit, VOS_BOOL bReverseOrder, VOS_UINT8 *digit)
{
    VOS_UINT8  higher;
    VOS_UINT8  lower;
    VOS_UINT32 ret;

    higher = (bcdDigit >> 4) & 0x0f;
    lower  = bcdDigit & 0x0f;
    if ((higher > 9) || (lower > 9)) {
        ret = MN_ERR_INVALIDPARM;
    } else {
        ret = MN_ERR_NO_ERROR;
    }

    if (bReverseOrder == VOS_TRUE) {
        *digit = (VOS_UINT8)((VOS_UINT8)(lower * 10) + higher);
    } else {
        *digit = (VOS_UINT8)((VOS_UINT8)higher * 10) + lower;
    }

    return ret;
}


VOS_UINT32 TAF_STD_RemapAsciiToSelectTable(VOS_UINT8 asciiChar, const VOS_UINT8 *alphaTable, VOS_UINT32 alphaTableSize,
                                           VOS_UINT8 *alphaChar)
{
    VOS_UINT32 loop1;

    if (alphaTable == VOS_NULL_PTR) {
        return VOS_FALSE;
    }

    for (loop1 = 0; loop1 < alphaTableSize; loop1++) {
        if (alphaTable[loop1] == (asciiChar)) {
            *alphaChar = (VOS_UINT8)loop1;
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}


VOS_UINT32 TAF_STD_ConvertAsciiToDefAlpha(const VOS_UINT8 *asciiChar, VOS_UINT32 len, VOS_UINT8 *defAlpha,
                                          VOS_UINT32 *defAlphaLen, VOS_UINT32 defAlphaBuffLen)
{
    VOS_UINT32 loop2;
    VOS_UINT32 ret;
    VOS_UINT32 defAlphaIdx;

    if ((asciiChar == VOS_NULL_PTR) || (defAlpha == VOS_NULL_PTR) || (defAlphaLen == VOS_NULL_PTR)) {
        return MN_ERR_NULLPTR;
    }

    ret         = MN_ERR_NO_ERROR;
    defAlphaIdx = 0;
    /*
     * Ŀǰ�ο�����ƽ̨��֧��23038 6.2.1 GSM 7 bit Default Alphabet���д��ڵ��ַ�����
     * ��֧����չ���е��ַ���
     * ASCII����GSM 7 bit Default Alphabet�������ֵı�׼ASCII�����ת��ΪGSM 7 bit Default Alphabet��
     * �����ܹ�������ַ��Ѳ���ͨ��������δ���ԣ�
     * ASCII���������ַ�����{�����¼Ϊ00���ض�������@��������ʧ�ܣ�
     * �����ܹ�������ַ��Ѳ���ͨ��������δ���ԣ�
     * ��Ҫ�ο�����ƽ̨�Ĵ���
     */
    for (loop2 = 0; loop2 < len; loop2++) {
        /* 7BIT convert into ASCII ,consider later!!!!! */
        /* ������ */
        if (defAlphaIdx >= defAlphaBuffLen) {
            ret = MN_ERR_INVALIDPARM;
            break;
        }

        /* ʹ��Basic Character Set�� */
        if (TAF_STD_RemapAsciiToSelectTable(*asciiChar, g_msgAsciiSfxDefAlpha, TAF_STD_MAX_GSM7BITDEFALPHA_NUM,
                                            defAlpha) == VOS_TRUE) {
            defAlphaIdx++;
            defAlpha++;
        } else {
            /* ʣ�໺�泤�Ȳ���֧��ʹ����չ����� */
            if ((defAlphaIdx + 1) >= defAlphaBuffLen) {
                ret = MN_ERR_INVALIDPARM;
                MN_INFO_LOG("TAF_STD_ConvertAsciiToDefAlpha ExtTable need larger buffer");
                break;
            }

            /* ʹ��Basic Character Set Extension�� */
            if (TAF_STD_RemapAsciiToSelectTable(*asciiChar, g_msgAsciiSfxDefAlphaExt, TAF_STD_MAX_GSM7BITDEFALPHA_NUM,
                                                defAlpha + 1) == VOS_TRUE) {
                *defAlpha = 0x1B;
                defAlphaIdx += 2;
                defAlpha += 2;
            } else {
                /* ��չ�����Ҳ�޷����� */
                MN_INFO_LOG1("TAF_STD_ConvertAsciiToDefAlpha Encode Failed ", *asciiChar);
                ret = MN_ERR_INVALIDPARM;
            }
        }

        asciiChar++;
    }

    *defAlphaLen = defAlphaIdx;

    return ret;
}


VOS_VOID TAF_STD_ConvertDefAlphaToAscii(const VOS_UINT8 *defAlpha, VOS_UINT32 defAlphaLen, VOS_UINT8 *asciiChar,
                                        VOS_UINT32 *dstLen)
{
    VOS_UINT32 loop;
    VOS_UINT8  pos;
    VOS_UINT32 validLen = 0;
    VOS_UINT8  extFlg;

    if ((defAlpha == VOS_NULL_PTR) || (asciiChar == VOS_NULL_PTR)) {
        return;
    }

    extFlg = VOS_FALSE;

    /*
     * Ŀǰ�ο�����ƽ̨��֧��23038 6.2.1 GSM 7 bit Default Alphabet���д��ڵ��ַ�����
     * ��֧����չ���е��ַ���
     * ��ASCII�������ֵ�GSM 7 bit Default Alphabet����ת��Ϊ��׼ASCII�빩��ʾ�ã�
     * ��չ�ַ����ת��Ϊ0XFE����ͬ�豸�϶�����˽�н��ͣ�
     * �����Ǳ�׼�ַ�ת��Ϊ0XFF����ͬ�豸�϶�����˽�н��ͣ�
     * ��Ҫ�ο�����ƽ̨�Ĵ���
     */
    for (loop = 0; loop < defAlphaLen; loop++) {
        pos        = TAF_STD_7BIT_MASK & defAlpha[loop];
        *asciiChar = (extFlg == VOS_FALSE) ? g_msgAsciiSfxDefAlpha[pos] : g_msgAsciiSfxDefAlphaExt[pos];

        if ((*asciiChar) == TAF_STD_NOSTANDARD_ASCII_CODE) {
            /* ���޷���ʾ���ַ�ʹ�ÿո��滻 */
            extFlg       = VOS_FALSE;
            (*asciiChar) = ' ';
            asciiChar++;
            validLen++;
        } else if ((*asciiChar) == TAF_STD_GSM_7BIT_EXTENSION_FLAG) {
            /* ��һ��Default Alphaʹ����չ����� */
            (*asciiChar) = ' ';
            extFlg       = VOS_TRUE;
        } else {
            extFlg = VOS_FALSE;
            asciiChar++;
            validLen++;
        }
    }

    if (validLen < defAlphaLen) {
        *asciiChar = 0;
    }

    *dstLen = validLen;

    return;
}


VOS_UINT32 TAF_STD_ConvertBcdCodeToDtmf(VOS_UINT8 bcdCode, VOS_UINT8 *dtmfCode)
{
    VOS_UINT8 dtmfCodeTemp;

    if (dtmfCode == VOS_NULL_PTR) {
        return MN_ERR_NULLPTR;
    }

    /* ����'0' BCD:0x00, DTMF:0x0A */
    /* �ַ�'*' BCD:0x0A, DTMF:0x0B */
    /* �ַ�'#' BCD:0x0B, DTMF:0x0C */

    if (bcdCode == 0x00) {
        dtmfCodeTemp = 0x0A;
    } else if (bcdCode <= 0x09) {
        dtmfCodeTemp = bcdCode;
    } else if (bcdCode == 0x0A) { /* �ַ�'*' */
        dtmfCodeTemp = bcdCode + 1;
    } else if (bcdCode == 0x0B) {
        dtmfCodeTemp = bcdCode + 1; /* �ַ�'#' */
    } else {
        return MN_ERR_INVALID_BCD;
    }

    *dtmfCode = dtmfCodeTemp;

    return MN_ERR_NO_ERROR;
}


VOS_UINT32 TAF_STD_ConvertBcdNumberToDtmf(const VOS_UINT8 *bcdNumber, VOS_UINT8 bcdLen, VOS_UINT8 *dtmfNumber,
                                          VOS_UINT8 dtmfLen)
{
    VOS_UINT32 loop;
    VOS_UINT8  len;
    VOS_UINT8  bcdCode;
    VOS_UINT32 ret;
    VOS_UINT32 lenTemp;

    if ((bcdNumber == VOS_NULL_PTR) || (dtmfNumber == VOS_NULL_PTR)) {
        return MN_ERR_NULLPTR;
    }

    /* ��������ַ�����ȥ����Ч��0XFF���� */
    while (bcdLen > 1) {
        if (bcdNumber[bcdLen - 1] == 0xFF) {
            bcdLen--;
        } else {
            break;
        }
    }

    if (bcdLen == 0) {
        return MN_ERR_INVALIDPARM;
    }

    /*
     * �ж�pucBcdAddress��ָ����ַ��������һ���ֽڵĸ�λ�Ƿ�Ϊ1111��
     * ����ǣ�˵������λ��Ϊ����������Ϊż��
     */
    if ((bcdNumber[bcdLen - 1] & 0xF0) == 0xF0) {
        lenTemp = (VOS_UINT32)(bcdLen * 2) - 1;
    } else {
        lenTemp = (VOS_UINT32)(bcdLen * 2);
    }

    if (lenTemp > TAF_STD_UINT8_MAX) {
        return MN_ERR_INVALIDPARM;
    }
    len = (VOS_UINT8)lenTemp;

    /* ȷ�����ᳬ��Ŀ���ַ���鳤�ȣ�Ŀǰû�з��գ����ӱ��� */
    len = TAF_MIN(len, dtmfLen);

    /* �������� */
    for (loop = 0; loop < len; loop++) {
        /* �жϵ�ǰ�����������λ���뻹��ż��λ���룬��0��ʼ����ż�� */
        if ((loop % 2) == 1) {
            /* ���������λ���룬��ȡ��4λ��ֵ */
            bcdCode = ((bcdNumber[(loop / 2)] >> 4) & 0x0F);
        } else {
            /* �����ż��λ���룬��ȡ��4λ��ֵ */
            bcdCode = (bcdNumber[(loop / 2)] & 0x0F);
        }

        /* ������������ת����DTMF����ʽ */
        ret = TAF_STD_ConvertBcdCodeToDtmf(bcdCode, dtmfNumber);
        if (ret != MN_ERR_NO_ERROR) {
            return ret;
        }

        dtmfNumber++;
    }

    return MN_ERR_NO_ERROR;
}


LOCAL const VOS_UINT16* TAF_STD_GetDaysOfLeapMonthTabAddr(VOS_VOID)
{
    return g_tafStdLeapMonthTab;
}


LOCAL const VOS_UINT16* TAF_STD_GetDaysOfNormalMonthTabAddr(VOS_VOID)
{
    return g_tafStdNormMonthTab;
}


LOCAL const VOS_UINT16* TAF_STD_GetDaysElapsedOfALeapYearSet(VOS_VOID)
{
    return g_tafStdDaysElapsedOfALeapYearSetTab;
}


VOS_UINT32 TAF_STD_64Add32(VOS_UINT32 highAddend, VOS_UINT32 lowAddend, VOS_UINT32 addFactor, VOS_UINT32 *highRslt,
                           VOS_UINT32 *lowRslt)
{
    if (addFactor <= 0xFFFFFFFF - lowAddend) {
        *lowRslt  = lowAddend + addFactor;
        *highRslt = highAddend;
    } else {
        if (highAddend == 0xFFFFFFFF) {
            return VOS_ERR;
        }

        *highRslt = highAddend + 1;
        *lowRslt  = addFactor - (0xFFFFFFFF - lowAddend) - 1;
    }

    return VOS_OK;
}


VOS_UINT32 TAF_STD_64Sub32(VOS_UINT32 highMinuend, VOS_UINT32 lowMinuend, VOS_UINT32 subFactor, VOS_UINT32 *highRslt,
                           VOS_UINT32 *lowRslt)
{
    if (lowMinuend >= subFactor) {
        *highRslt = highMinuend;
        *lowRslt  = lowMinuend - subFactor;
    } else if (highMinuend >= 1) {
        *highRslt = highMinuend - 1;
        *lowRslt  = lowMinuend + (0xFFFFFFFF - subFactor) + 1;
    } else {
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_VOID TAF_STD_U64ToStr(VOS_UINT32 productHigh, VOS_UINT32 productLow, VOS_UINT8 *string)
{
    VOS_UINT32  quotientHigh;
    VOS_UINT32  quotientLow;
    VOS_UINT32  remainderValue;
    VOS_UINT32  rst;
    errno_t     returnValue;

    VOS_UINT8 strLen;

    strLen = 0;

    /* ���U64��ullNumΪ0�������⴦�� */
    if ((productHigh == 0) && (productLow == 0)) {
        string[0] = '0';
        string[1] = '\0';
        return;
    }

    while ((productHigh != 0) || (productLow != 0)) {
        quotientHigh = 0;
        quotientLow  = 0;

        rst = VOS_64Div32(productHigh, productLow, 10, &quotientHigh, &quotientLow, &remainderValue);
        if (rst == VOS_OK) {
            productHigh = quotientHigh;
            productLow  = quotientLow;

            if (strLen == 0) {
                *string = '0' + (VOS_UINT8)remainderValue;
                strLen++;
                continue;
            }
            /* 64λ��ת�����ַ������20���ַ����ų���һλ��ʣ��19 */
            returnValue = memmove_s((string + 1), TAF_MIN(strLen, 19), string, strLen);
            TAF_MEM_CHK_RTN_VAL(returnValue, strLen, strLen);
            *string = '0' + (VOS_UINT8)remainderValue;
            strLen++;
        } else {
            break;
        }
    }

    return;
}


VOS_UINT32 TAF_STD_ConvertTimeFromSecsToTimeZone(VOS_UINT32 highSystemTime, VOS_UINT32 lowSystemTime,
                                                 TAF_STD_TimeZoneType *universalTimeandLocalTimeZone)
{
    TAF_STD_TmpTimeZoneConvert tmpTimeZoneConvert;
    VOS_UINT16                 i;
    VOS_UINT16                 days;
    VOS_UINT32                 quadYyears;
    const VOS_UINT16           *monthTable       = VOS_NULL_PTR;
    const VOS_UINT16           *leapMonthTable   = VOS_NULL_PTR;
    const VOS_UINT16           *normalMonthTable = VOS_NULL_PTR;
    const VOS_UINT16           *daysElapsedTable = VOS_NULL_PTR;
    VOS_UINT32                 result;

    leapMonthTable = TAF_STD_GetDaysOfLeapMonthTabAddr();

    normalMonthTable = TAF_STD_GetDaysOfNormalMonthTabAddr();

    daysElapsedTable = TAF_STD_GetDaysElapsedOfALeapYearSet();

    (VOS_VOID)memset_s(&tmpTimeZoneConvert, sizeof(tmpTimeZoneConvert), 0x00, sizeof(tmpTimeZoneConvert));

    /* ʱ�任��ɴ�1980.1.1 ��ʼ������ */
    result = TAF_STD_64Add32(highSystemTime, lowSystemTime, (VOS_UINT32)TAF_STD_TIME_ZONE_OFFSET_S,
                             &(tmpTimeZoneConvert.highTmp), &(tmpTimeZoneConvert.lowTmp));
    if (result != VOS_OK) {
        return VOS_ERR;
    }

    /* ����ϵͳʱ����������㵱ǰʱ������� */
    result = VOS_64Div32(tmpTimeZoneConvert.highTmp, tmpTimeZoneConvert.lowTmp, TAF_STD_SECONDS_PER_MINUTE,
                         &(tmpTimeZoneConvert.quotientHigh), &(tmpTimeZoneConvert.quotientLow),
                         &(tmpTimeZoneConvert.remainder));
    if (result != VOS_OK) {
        return VOS_ERR;
    }
    universalTimeandLocalTimeZone->second = (VOS_UINT16)tmpTimeZoneConvert.remainder;

    /* ����ϵͳʱ����������㵱ǰʱ��ķ����� */
    result = VOS_64Div32(tmpTimeZoneConvert.quotientHigh, tmpTimeZoneConvert.quotientLow, TAF_STD_MINUTES_PER_HOUR,
                         &(tmpTimeZoneConvert.highTmp), &(tmpTimeZoneConvert.lowTmp), &(tmpTimeZoneConvert.remainder));
    if (result != VOS_OK) {
        return VOS_ERR;
    }
    universalTimeandLocalTimeZone->minute = (VOS_UINT16)tmpTimeZoneConvert.remainder;

    /* ����ϵͳʱ����������㵱ǰʱ���Сʱ */
    result = VOS_64Div32(tmpTimeZoneConvert.highTmp, tmpTimeZoneConvert.lowTmp, TAF_STD_HOURS_PER_DAY,
                         &(tmpTimeZoneConvert.quotientHigh), &(tmpTimeZoneConvert.quotientLow),
                         &(tmpTimeZoneConvert.remainder));
    if (result != VOS_OK) {
        return VOS_ERR;
    }
    universalTimeandLocalTimeZone->hour = (VOS_UINT16)tmpTimeZoneConvert.remainder;

    /* ������1980��ʼ��ȥ�˶������� */
    result = VOS_64Div32(tmpTimeZoneConvert.quotientHigh, tmpTimeZoneConvert.quotientLow, TAF_STD_TIME_ZONE_QUAD_YEAR,
                         &(tmpTimeZoneConvert.highTmp), &(tmpTimeZoneConvert.lowTmp), &(tmpTimeZoneConvert.remainder));
    if (result != VOS_OK) {
        return VOS_ERR;
    }
    days = (VOS_UINT16)tmpTimeZoneConvert.remainder;

    quadYyears = tmpTimeZoneConvert.lowTmp;

    universalTimeandLocalTimeZone->year = (VOS_UINT16)(TAF_STD_TIME_ZONE_BASE_YEAR + (4 * quadYyears));

    /* ���㵱ǰ�괦���ĸ������� */
    for (i = 0; i < TAF_STD_DAYS_ELAPSED_OF_A_LEAP_YEAR_SET_TAB_COUNT - 1; i++) {
        if (days < daysElapsedTable[i + 1]) {
            break;
        }
    }

    /* ��ȡ��ǰ���ȥ������ */
    days -= daysElapsedTable[i];

    /* ��������һ�� */
    universalTimeandLocalTimeZone->year += (VOS_UINT16)i;

    /* ��������·ݺ���һ�� */

    /* ��ȡ�·ݱ� */
    monthTable = (i == 0) ? leapMonthTable : normalMonthTable;

    /* �����·ݱ� */
    for (i = 0; i < TAF_STD_NORMAL_MONTH_TAB_COUNT - 1; i++) {
        if (days < monthTable[i + 1]) {
            break;
        }
    }

    /* �������ǰ����һ����. */
    universalTimeandLocalTimeZone->day = days - monthTable[i] + 1;

    /* ������·� */
    universalTimeandLocalTimeZone->month = (VOS_UINT16)i + 1;

    return VOS_OK;
}


VOS_VOID TAF_STD_ConvertSystemTimeToHighLow(VOS_UINT8 *sysTimeByte, VOS_UINT32 *highSystemTime,
                                            VOS_UINT32 *lowSystemTime)
{
    VOS_UINT32 tmp;
    VOS_UINT8 *addr = VOS_NULL_PTR;

    addr = sysTimeByte + 1;

    tmp = *addr++;
    tmp <<= 8;
    tmp |= *addr++;
    tmp <<= 8;
    tmp |= *addr++;
    tmp <<= 8;
    tmp |= *addr;

    /* ȡsystemtime�ĸ���λ */
    *highSystemTime = (VOS_UINT32)((sysTimeByte[0] & 0xF0) >> 4);

    /* ȡsystemtime�ĵ���λ */
    *lowSystemTime = ((((VOS_UINT32)(((VOS_UINT32)(sysTimeByte[0] & 0x0F)) << 28)) & 0xF0000000) |
                      ((VOS_UINT32)(tmp >> 4) & 0x0FFFFFFF));
}


VOS_UINT16 TAF_STD_TransformBcdMccToDeciDigit(VOS_UINT32 bcdMcc)
{
    /* 0x00 06 04 --> 460 */
    VOS_UINT16 mcc;

    mcc = (bcdMcc & 0xff) * 100 + ((bcdMcc >> 8) & 0xff) * 10 + ((bcdMcc >> 16) & 0xff);

    if (mcc > 999) {
        mcc = 0xffff;
    }

    return mcc;
}


VOS_UINT16 TAF_STD_TransformBcdMncToDeciDigit(VOS_UINT32 bcdMnc)
{
    /* 2λMncת����ʽ:0x00 0f 03 00 --> 03 */
    /* 3λMncת����ʽ:0x0f 03 00 00 --> 003 */
    VOS_UINT16 mnc;

    if ((bcdMnc & 0x0f0000) == 0x0f0000) {
        mnc = (bcdMnc & 0xff) * 10 + ((bcdMnc >> 8) & 0xff);

        if (mnc >= 100) {
            mnc = 0xffff;
        }
    } else {
        mnc = (bcdMnc & 0xff) * 100 + ((bcdMnc >> 8) & 0xff) * 10 + ((bcdMnc >> 16) & 0xff);

        if (mnc >= 1000) {
            mnc = 0xffff;
        }
    }

    return mnc;
}


VOS_UINT8 TAF_STD_TransformBcdImsi1112ToDeciDigit(VOS_UINT16 bcdImsi1112)
{
    /* 0x03 00 --> 3 */
    VOS_UINT16 imsi1112;

    imsi1112 = ((bcdImsi1112) & 0xff) * 10 + ((bcdImsi1112 >> 8) & 0xff);

    if (imsi1112 > 100) {
        imsi1112 = 0xffff;
    }

    return (VOS_UINT8)imsi1112;
}

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)

VOS_UINT16 TAF_STD_TransformCLBcdMncToDeciDigit(VOS_UINT16 bcdMnc)
{
    /* 0x03 00 --> 03 */
    VOS_UINT16 mnc;

    mnc = (bcdMnc & 0xff) * 10 + ((bcdMnc >> 8) & 0xff);

    if (mnc > 100) {
        mnc = 0xffff;
    }

    return mnc;
}

#endif

VOS_UINT32 TAF_STD_TransformDeciDigitToBcdMcc(VOS_UINT32 deciDigitMcc)
{
    /* 460 --->0x00 06 04 */
    VOS_UINT32 bcdMcc;

    if (deciDigitMcc != TAF_STD_INVALID_MCC) {
        bcdMcc = (deciDigitMcc % 1000) / 100;

        bcdMcc += (deciDigitMcc % 10) << 16;

        bcdMcc += ((deciDigitMcc % 100) / 10) << 8;

        return bcdMcc;
    } else {
        return TAF_STD_INVALID_MCC;
    }
}


/*lint -e701*/
VOS_UINT8 TAF_STD_ExtractBitStringToOctet(VOS_UINT8 *srcAddr, VOS_UINT8 offsetPos, VOS_UINT8 bitLen)
{
    VOS_UINT8 *buffOffset = VOS_NULL_PTR;
    VOS_INT16  iRemainBitLen;
    VOS_UINT8  extractByte;
    VOS_UINT8  remainLen;

    extractByte = 0;

    buffOffset = srcAddr;

    if (offsetPos > TAF_STD_BIT_LEN_8_BIT) {
        MN_INFO_LOG("TAF_STD_ExtractBitStringToOctet: ucOffsetPos big than 'TAF_STD_BIT_LEN_8_BIT' ");
        return extractByte;
    }

    iRemainBitLen = (VOS_INT16)(TAF_STD_BIT_LEN_8_BIT - (offsetPos + bitLen));

    /* ���һ���ֽ�����usOffsetPos��ʣ��bit�ĳ��ȴ��ڵ���usOffsetPos����ֱ���ڸ��ֽ��н���ȡֵ */
    /* ���һ���ֽ�����usOffsetPos��ʣ��bit�ĳ���С��usOffsetPos,����Ҫ����һ���ֽ��н���ȡֵ */
    if (iRemainBitLen >= 0) {
        extractByte = (VOS_UINT8)(((VOS_UINT8)((*buffOffset) << offsetPos)) >>
                                  ((VOS_UINT16)(offsetPos + iRemainBitLen)));
    } else {
        /* ����ʣ��λ */
        remainLen = (VOS_UINT8)(bitLen - (TAF_STD_BIT_LEN_8_BIT - offsetPos));

        extractByte = (VOS_UINT8)((VOS_UINT8)((*buffOffset) & (0xFF >> offsetPos)) << remainLen);

        buffOffset++;

        /* ���ֽ�ʣ��bit���� */
        extractByte |= (VOS_UINT8)(((*buffOffset) & 0xFF) >> (VOS_UINT8)(TAF_STD_BIT_LEN_8_BIT - remainLen));
    }

    return extractByte;
}


VOS_UINT32 TAF_STD_ExtractBitStringToDword(VOS_UINT8 *srcAddr, VOS_UINT8 offsetPos, VOS_UINT8 bitLen)
{
    VOS_UINT8 *tmpSrc = VOS_NULL_PTR;
    VOS_UINT32 extractWord;
    VOS_UINT16 remainBitLen;
    VOS_UINT32 indexNum;
    VOS_UINT16 byteNum;

    tmpSrc = srcAddr;
    extractWord = 0;
    if (offsetPos > TAF_STD_BIT_LEN_8_BIT) {
        MN_INFO_LOG("TAF_STD_ExtractBitStringToDword: ucOffsetPos big than 'TAF_STD_BIT_LEN_8_BIT' ");
        return extractWord;
    }

    if (bitLen > TAF_STD_BIT_LEN_32_BIT) {
        bitLen = TAF_STD_BIT_LEN_32_BIT;
    }

    if (bitLen <= TAF_STD_BIT_LEN_8_BIT) {
        /* ����bit lengthС�ڵ���8bit�Ĵ��� */
        extractWord = (VOS_UINT32)TAF_STD_ExtractBitStringToOctet(tmpSrc, offsetPos, bitLen);

        return extractWord;
    }

    /* �����ȥ��ǰ�ֽ���Чbit��ʣ��bit���� */
    remainBitLen = (VOS_UINT16)(bitLen - (TAF_STD_BIT_LEN_8_BIT - offsetPos));

    /* ��ȡ�׸��ֽ����ݲ��֣����Ƶ���Ӧ�ĸ�λ, �ճ�ʣ��bitλ */
    extractWord = (VOS_UINT32)((VOS_UINT32)((*tmpSrc) & (0xFF >> offsetPos)) << remainBitLen);

    /* ʣ�����������Ϊ1�ֽ� */
    byteNum = (remainBitLen / TAF_STD_BIT_LEN_8_BIT) + 1;

    for (indexNum = 0; indexNum < byteNum; indexNum++) {
        /* ָ����һ���ֽ� */
        tmpSrc++;

        /* ���ʣ�೤�� */
        if (remainBitLen > TAF_STD_BIT_LEN_8_BIT) {
            /* ����ʣ��δȡ���bit */
            remainBitLen -= TAF_STD_BIT_LEN_8_BIT;

            /* ���ƣ��ճ�ʣ��bitλ */
            extractWord |= (VOS_UINT32)((VOS_UINT8)(*tmpSrc) << remainBitLen);
        } else {
            /* ���ֽ�ʣ��bit���� */
            extractWord |= (VOS_UINT32)((*tmpSrc) >> (VOS_UINT16)(TAF_STD_BIT_LEN_8_BIT - remainBitLen));

            /* bit�Ѿ�ȡ�꣬��0 */
            remainBitLen = 0;
        }

        if (remainBitLen == 0) {
            break;
        }
    }

    /* ����ת�����ֽڵĴ��� */
    return extractWord;
}
/*lint +e701*/


VOS_UINT32 TAF_STD_ChangeEndian(VOS_UINT8 *srcStr, VOS_UINT32 strLen, const VOS_UINT8 *dstStr, VOS_UINT8 dataSize)
{
    VOS_UINT32 indexNum;
    VOS_UINT32 interOrder;
    VOS_UINT8  tmp;

    if ((srcStr == VOS_NULL_PTR) || (dstStr == VOS_NULL_PTR)) {
        return TAF_ERR_NULL_PTR;
    }

    MN_INFO_LOG2("TAF_STD_ChangeEndian SLen DSize", strLen, dataSize);

    if ((dataSize == 0) || (strLen < dataSize)) {
        return TAF_ERR_PARA_ERROR;
    }

    indexNum = 0;

    while (indexNum + dataSize <= strLen) {
        for (interOrder = 0; interOrder < ((VOS_UINT32)dataSize / 2); interOrder++) {
            tmp                                              = *(srcStr + indexNum + interOrder);
            *(srcStr + indexNum + interOrder)                = *(srcStr + indexNum + dataSize - interOrder - 1);
            *(srcStr + indexNum + dataSize - interOrder - 1) = tmp;
        }
        indexNum += dataSize;
    }

    return TAF_ERR_NO_ERROR;
}


VOS_UINT32 TAF_STD_EncodeUnicodeToUtf8(VOS_UINT16 unicode, VOS_UINT8 *utf8Str, VOS_UINT32 utf8Len)
{
    if ((unicode <= TAF_STD_UTF8_1BYTE_MAX) && (utf8Len >= 1)) {
        *utf8Str = (VOS_UINT8)unicode;

        return 1;
    } else if ((unicode <= TAF_STD_UTF8_2BYTE_MAX) && (utf8Len >= 2)) {
        *(utf8Str + 1) = (VOS_UINT8)(0x80 | (unicode & 0x3F));
        *utf8Str       = (VOS_UINT8)(0xC0 | (unicode >> 6));

        return 2;
    } else if (utf8Len >= 3) {
        *utf8Str       = (VOS_UINT8)(0xE0 | (unicode >> 12));
        *(utf8Str + 1) = (VOS_UINT8)(0x80 | ((unicode >> 6) & 0x3F));
        *(utf8Str + 2) = (VOS_UINT8)(0x80 | (unicode & 0x3F));

        return 3;
    } else {
        return 0;
    }
}

LOCAL TAF_ERROR_CodeUint32 TAF_STD_DecodeUtf8CharacterFirstByte(const VOS_UINT8 *utf8FirstByte,
                                                                VOS_UINT16      *utf8CharacterContent,
                                                                VOS_UINT32      *utf8CharacterByteNum)
{
    /* �����λbit������1�ĸ��� */
    for (*utf8CharacterByteNum = 0; *utf8CharacterByteNum < 8; (*utf8CharacterByteNum)++) {
        if (((*utf8FirstByte) & (0x80 >> (*utf8CharacterByteNum))) == 0) {
            break;
        }
    }

    /* ��֤����ͨ���ԣ�֧�ֽ���ѯutf8�ַ��������ֽ�����������ΪNULL���������ֽ��� */
    if (utf8CharacterContent == VOS_NULL_PTR) {
        return TAF_ERR_NO_ERROR;
    }

    /* ��֧�ֳ���4����4�������ֽ����һ���ַ���UTF8���� */
    if (*utf8CharacterByteNum > 3) {
        /* ����֡ͷ�쳣������Ŀǰ֧�ֵ�����ֽڸ���������ʧ�� */
        return TAF_DECODE_ERR_UTF8_BEYOND_MAX_BYTE_LIMIT;
    } else if (*utf8CharacterByteNum > 1) {
        /* utf8�ַ����ɶ���ֽ���� */
        *utf8CharacterContent = *utf8FirstByte & (0x7F >> (*utf8CharacterByteNum));
    } else if (*utf8CharacterByteNum == 0) {
        /* utf8�ַ��ɵ��ֽ���� */
        *utf8CharacterContent = *utf8FirstByte;

        *utf8CharacterByteNum = 1;
    } else {
        /* ����֡ͷ�쳣(bit7~bit6: 10)������ʧ�� */
        return TAF_DECODE_ERR_UTF8_ABNORMAL_BYTE_HEADER;
    }

    return TAF_ERR_NO_ERROR;
}


VOS_UINT32 TAF_STD_ConvertUtf8ToUcs2(const VOS_UINT8 *utf8Str, VOS_UINT32 utf8Len, VOS_UINT16 *ucs2Str,
                                     VOS_UINT32 ucs2BuffLen, VOS_UINT32 *ucs2Idx)
{
    VOS_UINT32 ucs2IdxTemp;
    VOS_UINT32 utf8Idx;
    VOS_UINT32 utf8Num;
    VOS_UINT32 rslt;
    VOS_UINT16 temp;
    VOS_UINT8  utf8Char;

    if ((ucs2Str == VOS_NULL_PTR) || (utf8Str == VOS_NULL_PTR)) {
        return TAF_ERR_NULL_PTR;
    }

    utf8Num     = 0;
    ucs2IdxTemp = 0;

    for (utf8Idx = 0; utf8Idx < utf8Len; utf8Idx++) {
        utf8Char = *(utf8Str + utf8Idx);

        /* Utf8��һ���ֽ� */
        if (utf8Num == 0) {
            rslt = TAF_STD_DecodeUtf8CharacterFirstByte(&utf8Char, ucs2Str + ucs2IdxTemp, &utf8Num);

            /* ͷ�ֽڽ���ͨ�ú����������utf8���ֽ���(����ͷ�ֽ�)������Ҫ��1 */
            utf8Num--;

            if (rslt != TAF_ERR_NO_ERROR) {
                return rslt;
            }
        } else {
            /* Utf8�����ֽ� */
            if ((utf8Char >> 6) == 0x2) {
                temp = (VOS_UINT16)(TAF_GET_HOST_UINT16(ucs2Str + ucs2IdxTemp) << 6) | (utf8Char & 0x3F);
                TAF_PUT_HOST_UINT16(ucs2Str + ucs2IdxTemp, temp);
                utf8Num--;
            } else {
                MN_INFO_LOG("TAF_STD_ConvertUtf8ToUcs2 Invaild follow Byte");

                /* ������֡ͷ�쳣������ʧ�� */
                return TAF_DECODE_ERR_UTF8_ABNORMAL_BYTE_CONTENT;
            }
        }

        /* ��ǰUtf8�ֽ���Utf8��������һ���ֽ� */
        if (utf8Num == 0) {
            ucs2IdxTemp++;
            if (ucs2IdxTemp >= ucs2BuffLen) {
                break;
            }
        }
    }

    *ucs2Idx = ucs2IdxTemp;

    return TAF_ERR_NO_ERROR;
}


VOS_UINT32 TAF_STD_ConvertUcs2ToUtf8(const VOS_UINT16 *ucs2Str, VOS_UINT32 ucs2Len, VOS_UINT8 *utf8Str,
                                     VOS_UINT32 utf8BuffLen)
{
    VOS_UINT32 ucs2Idx;
    VOS_UINT32 utf8Idx;
    VOS_UINT32 encodeLen;

    if ((ucs2Str == VOS_NULL_PTR) || (utf8Str == VOS_NULL_PTR)) {
        return 0;
    }

    utf8Idx = 0;

    for (ucs2Idx = 0; ucs2Idx < ucs2Len; ucs2Idx++) {
        encodeLen = TAF_STD_EncodeUnicodeToUtf8(TAF_GET_HOST_UINT16(ucs2Str + ucs2Idx),
                                                utf8Str + utf8Idx, utf8BuffLen - utf8Idx);
        utf8Idx += encodeLen;

        if (encodeLen == 0) {
            break;
        }
    }

    return utf8Idx;
}


VOS_UINT32 TAF_STD_ConvertUcs2To8Bit(const VOS_UINT16 *ucs2Str, VOS_UINT32 ucs2Len, VOS_UINT8 *puc8BitStr,
                                     VOS_UINT32 ul8BitBuffLen)
{
    VOS_UINT32 indexNum;
    VOS_UINT32 ul8BitLen;
    VOS_UINT32 ul8BitValue;

    ul8BitLen = ucs2Len > ul8BitBuffLen ? ul8BitBuffLen : ucs2Len;

    for (indexNum = 0; indexNum < ul8BitLen; indexNum++) {
        ul8BitValue = TAF_GET_HOST_UINT16(ucs2Str + indexNum);
        if (ul8BitValue > TAF_STD_UINT8_MAX) {
            MN_ERR_LOG1("TAF_STD_ConvertUcs2To8Bit: The number is big than '0xff' ", indexNum);
        }
        *(puc8BitStr + indexNum) = (VOS_UINT8)ul8BitValue;
    }

    return ul8BitLen;
}


VOS_UINT32 TAF_STD_Convert8BitToUcs2(const VOS_UINT8 *puc8BitStr, VOS_UINT32 ul8BitLen, VOS_UINT16 *ucs2Str,
                                     VOS_UINT32 ucs2BuffLen)
{
    VOS_UINT32 indexNum;
    VOS_UINT32 ucs2Len;

    ucs2Len = ul8BitLen < ucs2BuffLen ? ul8BitLen : ucs2BuffLen;

    for (indexNum = 0; indexNum < ucs2Len; indexNum++) {
        TAF_PUT_HOST_UINT16(ucs2Str + indexNum, (VOS_UINT16)(*(puc8BitStr + indexNum)));
    }

    return ucs2Len;
}

#if (OSA_CPU_CCPU == VOS_OSA_CPU)

VOS_UINT32 TAF_STD_ConvertStrToUcs2(TAF_STD_StrWithEncodingType *srcStr, VOS_UINT16 *ucs2Str, VOS_UINT32 ucs2BuffLen,
                                    VOS_UINT32 *ucs2Len)
{
    VOS_UINT8    *unpackBuff = VOS_NULL_PTR;
    errno_t       memResult;
    ModemIdUint16 modemId;

    if ((srcStr == VOS_NULL_PTR) || (ucs2Str == VOS_NULL_PTR) || (ucs2Len == VOS_NULL_PTR) ||
        (srcStr->str == VOS_NULL_PTR)) {
        return TAF_ERR_NULL_PTR;
    }

    MN_INFO_LOG1("TAF_STD_ConvertStrToUcs2: encoding", srcStr->coding);

    switch (srcStr->coding) {
        case TAF_STD_ENCODING_7BIT:
            /* 7bit: �Ƚ����8bit,�ٽ�8bit��Ϊ16bit��UCS2 */
            modemId = NAS_MULTIINSTANCE_GetCurrInstanceModemId(WUEPS_PID_TAF);

            unpackBuff = (VOS_UINT8 *)NAS_MULTIINSTANCE_MemAlloc(modemId, WUEPS_PID_TAF,
                                                                 ucs2BuffLen * sizeof(VOS_UINT8));

            if (unpackBuff == VOS_NULL_PTR) {
                return TAF_ERR_ERROR;
            }

            if (TAF_STD_UnPack7Bit(srcStr->str, ucs2BuffLen, 0, unpackBuff) == VOS_ERR) {
                NAS_MULTIINSTANCE_MemFree(modemId, WUEPS_PID_TAF, unpackBuff);
                MN_ERR_LOG("TAF_STD_ConvertStrToUcs2: Unpack fail");
                return TAF_ERR_ERROR;
            }

            *ucs2Len = ucs2BuffLen;

            /* ȥ��7Bitѹ��ʱ���λ */
            if ((*ucs2Len > 0) && ((unpackBuff[*ucs2Len - 1]) == 0x0d)) {
                (*ucs2Len)--;
            }

            /* DefaultAlpha->Ascii */
            TAF_STD_ConvertDefAlphaToAscii(unpackBuff, *ucs2Len, unpackBuff, ucs2Len);

            /* ���ֽ�ת˫�ֽ� */
            *ucs2Len = TAF_STD_Convert8BitToUcs2(unpackBuff, *ucs2Len, ucs2Str, ucs2BuffLen);

            NAS_MULTIINSTANCE_MemFree(modemId, WUEPS_PID_TAF, unpackBuff);
            break;

        case TAF_STD_ENCODING_8BIT:
            TAF_STD_Convert8BitToUcs2(srcStr->str, ucs2BuffLen, ucs2Str, ucs2BuffLen);
            *ucs2Len = srcStr->len;
            break;

        case TAF_STD_ENCODING_UCS2:
            if (srcStr->len > 0) {
                memResult = memcpy_s(ucs2Str, ucs2BuffLen, srcStr->str, srcStr->len);
                TAF_MEM_CHK_RTN_VAL(memResult, ucs2BuffLen, srcStr->len);
            }

            /* ����Ϊ��� �ڲ�ΪС�ˣ���Ҫת�� */
            (VOS_VOID)TAF_STD_ChangeEndian((VOS_UINT8 *)ucs2Str, ucs2BuffLen, (VOS_UINT8 *)ucs2Str, sizeof(VOS_UINT16));

            /* Ucs2LenָUINT16��UCS2��ĸ����� pstSrcStr->ulLenָ�����ֽ��� */
            *ucs2Len = srcStr->len / 2;
            break;

        case TAF_STD_ENCODING_UTF8:
            /* ���ӱ���ת��ʧ��ԭ��ֵ */
            return TAF_STD_ConvertUtf8ToUcs2(srcStr->str, srcStr->len, ucs2Str, ucs2BuffLen, ucs2Len);

        default:
            return TAF_ERR_ERROR;
    }

    return TAF_ERR_NO_ERROR;
}


VOS_UINT32 TAF_STD_ConvertUcs2ToStr(TAF_STD_StrWithEncodingType *dstStr, VOS_UINT16 *ucs2Str, VOS_UINT32 ucs2Len,
                                    VOS_UINT32 dstStrBuffLen)
{
    VOS_UINT8    *buffStr = VOS_NULL_PTR;
    VOS_UINT32    ucs2BuffLen;
    VOS_UINT32    allocBuffLen;
    VOS_UINT32    ul8BitLen;
    VOS_UINT32    rslt;
    errno_t       memResult;
    ModemIdUint16 modemId;

    if ((dstStr == VOS_NULL_PTR) || (ucs2Str == VOS_NULL_PTR) || (dstStr->str == VOS_NULL_PTR)) {
        return TAF_ERR_NULL_PTR;
    }

    MN_INFO_LOG1("TAF_STD_ConvertUcs2ToStr: encoding", dstStr->coding);

    dstStr->len = 0;
    (VOS_VOID)memset_s(dstStr->str, dstStrBuffLen, 0, dstStrBuffLen);

    switch (dstStr->coding) {
        case TAF_STD_ENCODING_UCS2:
            dstStr->len = TAF_MIN((ucs2Len * sizeof(VOS_UINT16)), dstStrBuffLen);
            if (dstStr->len > 0) {
                memResult = memcpy_s(dstStr->str, dstStrBuffLen, ucs2Str, dstStr->len);
                TAF_MEM_CHK_RTN_VAL(memResult, dstStrBuffLen, dstStr->len);
            }

            /* TODO: ����CHR�ϱ� */
            /* �ڲ���С���򣬿������ʱ����Ҫת�ɴ���� */
            rslt = TAF_STD_ChangeEndian(dstStr->str, dstStr->len, dstStr->str, sizeof(VOS_UINT16));

            return rslt;

        case TAF_STD_ENCODING_8BIT:
            dstStr->len = TAF_STD_ConvertUcs2To8Bit(ucs2Str, ucs2Len, dstStr->str, dstStrBuffLen);
            break;

        case TAF_STD_ENCODING_7BIT:
            /* 7Bit֧����չ��������2���ֽڱ�ʾһ��Unicode */
            ul8BitLen    = 0;
            ucs2BuffLen  = ucs2Len * 2;
            allocBuffLen = ucs2BuffLen * 2;

            modemId = NAS_MULTIINSTANCE_GetCurrInstanceModemId(WUEPS_PID_TAF);

            /* ����2������ֿ�ʹ�� */
            buffStr = (VOS_UINT8 *)NAS_MULTIINSTANCE_MemAlloc(modemId, WUEPS_PID_TAF, allocBuffLen * sizeof(VOS_UINT8));
            if (buffStr == VOS_NULL_PTR) {
                return TAF_ERR_ERROR;
            }
            (VOS_VOID)memset_s(buffStr, allocBuffLen * sizeof(VOS_UINT8), 0, allocBuffLen * sizeof(VOS_UINT8));

            /* ��ת��8Bit���� */
            ucs2Len = TAF_STD_ConvertUcs2To8Bit(ucs2Str, ucs2Len, buffStr, ucs2BuffLen);

            /* ת��DefaultAlpha��ʹ�ú��λ��� */
            if (TAF_STD_ConvertAsciiToDefAlpha(buffStr, ucs2Len, buffStr + ucs2BuffLen, &ul8BitLen, ucs2BuffLen) !=
                MN_ERR_NO_ERROR) {
                NAS_MULTIINSTANCE_MemFree(modemId, WUEPS_PID_TAF, buffStr);
                MN_ERR_LOG("TAF_STD_ConvertUcs2ToStr: Ascii->DefAlpha fail");
                return TAF_ERR_ERROR;
            }

            ul8BitLen = TAF_MIN(((dstStrBuffLen * TAF_STD_BIT_LEN_8_BIT) / TAF_STD_BIT_LEN_7_BIT), ul8BitLen);

            /* ѹ����7Bit���� */
            if (TAF_STD_Pack7Bit(buffStr + ucs2BuffLen, ul8BitLen, 0, dstStr->str, &(dstStr->len)) != VOS_OK) {
                NAS_MULTIINSTANCE_MemFree(modemId, WUEPS_PID_TAF, buffStr);
                MN_ERR_LOG("TAF_STD_ConvertUcs2ToStr: pack fail");
                return TAF_ERR_ERROR;
            }

            /* 23038 6.1.2.3.1�����7Bit���û��������Ҫ��CR���ⱻʶ���@ */
            if (ul8BitLen % 8 == 7) {
                *(dstStr->str + dstStr->len - 1) |= 0x1A;
            }

            NAS_MULTIINSTANCE_MemFree(modemId, WUEPS_PID_TAF, buffStr);
            break;

        case TAF_STD_ENCODING_UTF8:
            dstStr->len = TAF_STD_ConvertUcs2ToUtf8(ucs2Str, ucs2Len, dstStr->str, dstStrBuffLen);
            break;

        default:
            return TAF_ERR_ERROR;
    }

    return TAF_ERR_NO_ERROR;
}
#endif


VOS_UINT32 TAF_STD_IsSuitableEncodeForUcs2(const VOS_UINT16 *ucs2, VOS_UINT32 ucs2Len, TAF_STD_EncodingTypeUint8 coding)
{
    VOS_UINT32 indexNum;
    VOS_UINT32 decodeRet1;
    VOS_UINT32 decodeRet2;
    VOS_UINT8  tmp;
    VOS_UINT16 ucs2Char;

    if (ucs2 == VOS_NULL_PTR) {
        return TAF_ERR_NULL_PTR;
    }

    tmp = 0;

    for (indexNum = 0; indexNum < ucs2Len; indexNum++) {
        ucs2Char = ucs2[indexNum];

        switch (coding) {
            case TAF_STD_ENCODING_7BIT:
                decodeRet1 = TAF_STD_RemapAsciiToSelectTable((VOS_UINT8)ucs2Char, g_msgAsciiSfxDefAlpha,
                                                             TAF_STD_MAX_GSM7BITDEFALPHA_NUM, &tmp);
                decodeRet2 = TAF_STD_RemapAsciiToSelectTable((VOS_UINT8)ucs2Char, g_msgAsciiSfxDefAlphaExt,
                                                             TAF_STD_MAX_GSM7BITDEFALPHA_NUM, &tmp);
                if (((decodeRet1 == VOS_FALSE) && (decodeRet2 == VOS_FALSE)) || (ucs2Char > 0xFF)) {
                    return TAF_ERR_ERROR;
                }
                break;

            case TAF_STD_ENCODING_8BIT:
                if (ucs2Char > 0x7F) {
                    return TAF_ERR_ERROR;
                }
                break;

            case TAF_STD_ENCODING_UCS2:
            case TAF_STD_ENCODING_UTF8:
                return TAF_ERR_NO_ERROR;

            default:
                return TAF_ERR_ERROR;
        }
    }

    return TAF_ERR_NO_ERROR;
}

#if (OSA_CPU_CCPU == VOS_OSA_CPU)

VOS_UINT32 TAF_STD_ConvertStrEncodingType(TAF_STD_StrWithEncodingType *srcStr, TAF_STD_EncodingTypeUint8 dstCoding,
                                          VOS_UINT32 dstBuffLen, TAF_STD_StrWithEncodingType *dstStr)
{
    VOS_UINT16   *ucs2Str = VOS_NULL_PTR;
    VOS_UINT32    ucs2Len;
    VOS_UINT32    ucs2BuffLen;
    VOS_UINT32    rslt;
    ModemIdUint16 modemId;

    modemId = NAS_MULTIINSTANCE_GetCurrInstanceModemId(WUEPS_PID_TAF);

    if ((srcStr == VOS_NULL_PTR) || (dstStr == VOS_NULL_PTR) || (srcStr->str == VOS_NULL_PTR) ||
        (dstStr->str == VOS_NULL_PTR)) {
        return TAF_ERR_NULL_PTR;
    }

    /* ��������Unicode�ڴ��С */
    /* pstSrcStr->Lenָ�ֽ����� ulUcs2LenָUCS2�������� ǰ��UINT8 ����UINT16 */
    if (srcStr->coding == TAF_STD_ENCODING_7BIT) {
        ucs2Len = srcStr->len * 8 / 7;
    } else {
        ucs2Len = srcStr->len;
    }
    ucs2BuffLen = ucs2Len;

    ucs2Str = (VOS_UINT16 *)NAS_MULTIINSTANCE_MemAlloc(modemId, WUEPS_PID_TAF, ucs2BuffLen * sizeof(VOS_UINT16));
    if (ucs2Str == VOS_NULL_PTR) {
        return TAF_ERR_ERROR;
    }
    (VOS_VOID)memset_s(ucs2Str, ucs2BuffLen * sizeof(VOS_UINT16), 0, ucs2BuffLen * sizeof(VOS_UINT16));

    /* ����������תΪUnicode(UCS2)��ʽ */
    rslt = TAF_STD_ConvertStrToUcs2(srcStr, ucs2Str, ucs2BuffLen, &ucs2Len);

    if (rslt != TAF_ERR_NO_ERROR) {
        NAS_MULTIINSTANCE_MemFree(modemId, WUEPS_PID_TAF, ucs2Str); /*lint !e516 */
        return rslt;
    }

    /* �Ƿ���ת��Ŀ����� */
    if (TAF_STD_IsSuitableEncodeForUcs2(ucs2Str, ucs2Len, dstCoding) != TAF_ERR_NO_ERROR) {
        NAS_MULTIINSTANCE_MemFree(modemId, WUEPS_PID_TAF, ucs2Str); /*lint !e516 */
        MN_INFO_LOG("TAF_STD_ConvertStrEncodingType Dst Encoding cannot be");
        return TAF_ERR_PARA_ERROR;
    }
    dstStr->coding = dstCoding;

    /* ��Unicode(UCS2)����ת��ָ����ʽ */
    rslt = TAF_STD_ConvertUcs2ToStr(dstStr, ucs2Str, ucs2Len, dstBuffLen);

    if (rslt != TAF_ERR_NO_ERROR) {
        NAS_MULTIINSTANCE_MemFree(modemId, WUEPS_PID_TAF, ucs2Str); /*lint !e516 */
        return rslt;
    }

    NAS_MULTIINSTANCE_MemFree(modemId, WUEPS_PID_TAF, ucs2Str); /*lint !e516 */
    return TAF_ERR_NO_ERROR;
}
#endif

/*
 * ��鰲ȫ��������ֵ��������������ִ�и�λ����
 */
MODULE_EXPORTED VOS_VOID TAF_STD_MemFuncCheckResultContinue(VOS_INT32 returnValue, VOS_UINT32 destLen,
                                                            VOS_UINT32 srcLen, VOS_UINT32 fileIdAndLine)
{
    if (returnValue != EOK) {
        MN_ERR_LOG4("TAF_MemFunc_CheckReturnValue: error ", returnValue, destLen, srcLen, fileIdAndLine);
    }

    return;
}
/*
 * ����ַ�����ȫ��������ֵ��������������ִ�и�λ����
 */
MODULE_EXPORTED VOS_VOID TAF_STD_StrcpyFuncCheckResultContinue(VOS_INT32 returnValue, VOS_UINT32 destLen,
                                                               VOS_UINT32 srcLen, VOS_UINT32 fileIdAndLine)
{
    if (returnValue != EOK) {
        MN_ERR_LOG4("TAF_STD_StrcpyFuncCheckResultContinue:error", returnValue, destLen, srcLen, fileIdAndLine);
    }

    return;
}

MODULE_EXPORTED VOS_VOID TAF_STD_SprintfFuncCheckResultContinue(VOS_INT32 charNum, VOS_UINT32 destLen,
                                                                VOS_UINT32 srcLen, VOS_UINT32 fileIdAndLine)
{
    if (charNum == -1) {
        MN_ERR_LOG4("TAF_STD_SprintfFuncCheckResultContinue: error ", charNum, destLen, srcLen, fileIdAndLine);
    }

    return;
}
MODULE_EXPORTED VOS_VOID TAF_STD_StrtokFuncCheckResultContinue(VOS_UINT8 *strtoken, VOS_UINT32 fileIdAndLine)
{
    if (strtoken == NULL) {
        MN_ERR_LOG1("TAF_STD_StrtokFuncCheckResultContinue: error ", fileIdAndLine);
    }

    return;
}


MODULE_EXPORTED VOS_VOID TAF_STD_MemFunc_CheckReturnValue(VOS_INT32 returnValue, VOS_UINT32 destLen, VOS_UINT32 srcLen,
                                                          VOS_UINT32 fileIdAndLine)
{
    if (returnValue != EOK) {
        if (destLen > 0) {
            if (destLen > 0XFFFF) {
                MN_ERR_LOG1("TAF_STD_MemFunc_CheckReturnValue: the ulDestLen is overflow ", destLen);
                destLen = 0XFFFF;
            }
            if (srcLen > 0XFFFF) {
                MN_ERR_LOG1("TAF_STD_MemFunc_CheckReturnValue: the ulSrcLen is overflow ", srcLen);
                srcLen = 0XFFFF;
            }

#if ((VOS_OSA_CPU == OSA_CPU_CCPU) || ((!defined(CHOOSE_MODEM_USER)) && (FEATURE_MEMCPY_REBOOT_CFG == FEATURE_ON)))
            mdrv_om_system_error(TAF_REBOOT_MOD_ID_MEM, (VOS_INT32)(((destLen) << 16) | (srcLen)),
                                 (VOS_INT32)fileIdAndLine, 0, 0);
#endif
            MN_ERR_LOG("TAF_STD_MemFunc_CheckReturnValue ERR!");
        } else {
            MN_ERR_LOG1("TAF_STD_MemFunc_CheckReturnValue: the ulDestLen is zero ", destLen);
        }
    }

    return;
}


MODULE_EXPORTED VOS_UINT32 TAF_STD_MemFunc_Min(VOS_UINT32 destSize, VOS_UINT32 srcSize)
{
    VOS_UINT32 realSrcSize;

#if ((!defined(CHOOSE_MODEM_USER)) && (FEATURE_MEMCPY_REBOOT_CFG == FEATURE_ON))
    realSrcSize = srcSize;
#else
    realSrcSize = TAF_MIN(destSize, srcSize);
#endif

    return realSrcSize;
}


VOS_UINT32 TAF_STD_IsLeapYear(VOS_UINT16 year)
{
    if (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0)) {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_UINT32 TAF_STD_GetDaysForYear(VOS_UINT16 year)
{
    VOS_UINT32 days;

    days = TAF_STD_IsLeapYear(year) ? (366) : (365);

    return days;
}


VOS_VOID TAF_STD_SecondTimeToDateTime(VOS_UINT64 secondTime, TAF_STD_TimeZoneType *dateTime)
{
    VOS_UINT8  monLoop       = 0;
    VOS_UINT16 curYear       = 0;
    VOS_UINT32 rst           = 0;
    VOS_UINT32 daysOfCurYear = 0;
    VOS_UINT32 leftDays      = 0;
    VOS_UINT32 leftSeconds   = 0;

    VOS_UINT32 highSecondTmp = 0;
    VOS_UINT32 lowSecondTmp  = 0;
    VOS_UINT32 quotientHigh  = 0;
    VOS_UINT32 quotientLow   = 0;
    VOS_UINT32 rslt          = VOS_OK;

    /* ʱ�䳬��2099/12/31 23:59:59�򽫵�ǰʱ������Ϊ0s(1900/01/01 00:00:00) */
    if (secondTime > TAF_STD_MAX_DATE_TO_SECOND) {
        secondTime = 0;
    }

    /* 64λ�������㣬תΪ�ߵ�32λ������ */
    highSecondTmp = (VOS_UINT32)((secondTime >> 32) & 0xFFFFFFFF);
    lowSecondTmp = (VOS_UINT32)(secondTime & 0xFFFFFFFF);

    /* ����ʱ������������㵱ǰʱ������� */
    rslt = VOS_64Div32(highSecondTmp, lowSecondTmp, TAF_STD_DAY_TO_SECOND, &(quotientHigh), &(quotientLow),
                        &(leftSeconds));
    if (rslt != VOS_OK) {
        MN_ERR_LOG("TAF_STD_SecondTimeToDateTime: VOS_64Div32 para is invalid");
        return;
    }

    /* ������32λ�ɴ洢 */
    leftDays      = quotientLow;
    curYear       = TAF_STD_UTC_START_YEAR;
    daysOfCurYear = TAF_STD_GetDaysForYear(curYear);

    /* ������ */
    while (leftDays >= daysOfCurYear) {
        leftDays = leftDays - daysOfCurYear;
        curYear++;
        daysOfCurYear = TAF_STD_GetDaysForYear(curYear);
    }

    dateTime->year = curYear;

    /* �������� */
    rst = TAF_STD_IsLeapYear(curYear);

    for (monLoop = 1; monLoop < TAF_STD_MAX_MONTH_DAY_COUNT; monLoop++) {
        if (leftDays < g_monthDay[rst][monLoop]) {
            dateTime->month = monLoop;
            dateTime->day   = (VOS_UINT8)(leftDays - g_monthDay[rst][monLoop - 1] + 1);
            break;
        }
    }

    /* ����ʱ���� */
    dateTime->hour   = (VOS_UINT16)(leftSeconds / TAF_STD_HOUR_TO_SECOND);
    dateTime->minute = (VOS_UINT16)((leftSeconds % TAF_STD_HOUR_TO_SECOND) / TAF_STD_MIN_TO_SECOND);
    dateTime->second = (VOS_UINT16)(leftSeconds % TAF_STD_MIN_TO_SECOND);

    return;
}


VOS_UINT64 TAF_STD_DateTimeToSecondTime(TAF_STD_TimeZoneType *dateTime)
{
    VOS_UINT32 rst           = 0;
    VOS_UINT16 curYear       = 0;
    VOS_UINT32 daysOfCurYear = 0;
    VOS_UINT32 daysOfYears   = 0;
    VOS_UINT64 secondTime    = 0;

    /* 1900/01/01 00:00:00~2099/12/31 23:59:59Ϊ��Чʱ�䣬������Ч��Χ����Ϊ0s(1900/01/01 00:00:00) */
    if ((dateTime->year < TAF_STD_UTC_START_YEAR) || (dateTime->year >= TAF_STD_TIME_ZONE_MAX_YEAR) ||
        (dateTime->month == 0) || (dateTime->day == 0)) {
        MN_ERR_LOG("TAF_STD_DateTimeToSecondTime: The DateTime is invalid");
        return 0;
    }

    curYear = TAF_STD_UTC_START_YEAR;

    while (curYear < dateTime->year) {
        daysOfCurYear = TAF_STD_GetDaysForYear(curYear);
        daysOfYears += daysOfCurYear;
        curYear++;
    }

    rst = TAF_STD_IsLeapYear(curYear);

    daysOfYears += (g_monthDay[rst][dateTime->month - 1] + dateTime->day - 1);

    secondTime = (VOS_UINT64)(dateTime->hour * TAF_STD_HOUR_TO_SECOND + dateTime->minute * TAF_STD_MIN_TO_SECOND +
                              dateTime->second);

    secondTime += ((VOS_UINT64)daysOfYears * TAF_STD_DAY_TO_SECOND);

    return secondTime;
}



VOS_VOID TAF_STD_Get64BitSlice(VOS_UINT32 *highBitValue, VOS_UINT32 *lowBitValue)
{
    (VOS_VOID)mdrv_timer_get_accuracy_timestamp(highBitValue, lowBitValue);
}


VOS_BOOL TAF_STD_TestUintBit(VOS_UINT8 bitNum, VOS_UINT bitMaskValue)
{
    return (VOS_BOOL)(0x01UL & (bitMaskValue >> (bitNum & (TAF_STD_BITS_PER_UINT - 1))));
}


VOS_VOID TAF_STD_SetUintBit(VOS_UINT8 bitNum, VOS_UINT *bitMaskValue)
{
    (*bitMaskValue) |= ((VOS_UINT32)0x01UL << (bitNum & (TAF_STD_BITS_PER_UINT - 1)));
}


VOS_VOID TAF_STD_ClearUintBit(VOS_UINT8 bitNum, VOS_UINT *bitMaskValue)
{
    (*bitMaskValue) &= (~(0x01UL << (bitNum & (TAF_STD_BITS_PER_UINT - 1))));
}


VOS_UINT8 TAF_STD_GetUintBitNum(VOS_UINT bitMaskValue)
{
    VOS_UINT32 i;
    VOS_UINT8  bitNum;

    bitNum = 0;

    for (i = 0; i < TAF_STD_BITS_PER_UINT; i++) {
        if (TAF_STD_TestUintBit((VOS_UINT8)i, bitMaskValue)) {
            bitNum++;
        }
    }

    return bitNum;
}


LOCAL VOS_UINT32 TAF_STD_CompareTwoSNssaiWithoutMappedSNssai(const PS_S_NSSAI_STRU *srcSNssai,
    const PS_S_NSSAI_STRU *destSNssai)
{
    if (srcSNssai->ucSst != destSNssai->ucSst) {
        return VOS_FALSE;
    }

    if (srcSNssai->bitOpSd != destSNssai->bitOpSd) {
        return VOS_FALSE;
    }

    if (srcSNssai->bitOpSd == VOS_FALSE) {
        return VOS_TRUE;
    }

    if (srcSNssai->ulSd != destSNssai->ulSd) {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}


LOCAL VOS_UINT32 TAF_STD_IsMappedSNssaiMatchMappedSNssai(const PS_S_NSSAI_STRU *srcSNssai,
    const PS_S_NSSAI_STRU *destSNssai)
{
    if (srcSNssai->bitOpMappedSst != destSNssai->bitOpMappedSst) {
        return VOS_FALSE;
    }

    if (srcSNssai->bitOpMappedSst == VOS_FALSE) {
        return VOS_TRUE;
    }

    if (srcSNssai->ucMappedSst != destSNssai->ucMappedSst) {
        return VOS_FALSE;
    }

    if (srcSNssai->bitOpMappedSd != destSNssai->bitOpMappedSd) {
        return VOS_FALSE;
    }

    if (srcSNssai->bitOpMappedSd == VOS_FALSE) {
        return VOS_TRUE;
    }

    if (srcSNssai->ulMappedSd != destSNssai->ulMappedSd) {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}


VOS_UINT32 TAF_STD_CompareTwoSNssaiWithMappedSNssai(const PS_S_NSSAI_STRU *srcSNssai,
    const PS_S_NSSAI_STRU *destSNssai)
{
    if (TAF_STD_CompareTwoSNssaiWithoutMappedSNssai(srcSNssai, destSNssai) == VOS_FALSE) {
        return VOS_FALSE;
    }

    if (TAF_STD_IsMappedSNssaiMatchMappedSNssai(srcSNssai, destSNssai) == VOS_FALSE) {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}

