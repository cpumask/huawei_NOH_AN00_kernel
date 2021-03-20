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

#ifndef _ATCHECKFUNC_H_
#define _ATCHECKFUNC_H_

#include "AtParse.h"
#include "ps_common_def.h"
#include "AtMntn.h"
#include "AtSetParaCmd.h"

#define AT_GNA_E5_MAC_ADDR_LEN_BETWEEN_COLONS 2 /* MAC��ַ��ð��֮����ַ����� */
#define AT_GNA_E5_MAC_ADDR_COLON_NUM 5          /* MAC��ַ��ð�ŵĸ��� */
#define AT_MAC_ADDR_LEN 32                      /* MAC��ַ���� AT_GNA_E5_MAC_ADDR_LEN ->AT_MAC_ADDR_LEN */
#define AT_CDATA_DIAL_777_LEN 4
#define AT_DATA_DIAL_LEN 3

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)

/* ģ���������士HEADER */

TAF_UINT32 At_CheckSemicolon(TAF_UINT8 Char);
TAF_UINT32 At_CheckDialNum(TAF_UINT8 Char);
TAF_UINT32 At_CheckDmChar(TAF_UINT8 Char);
TAF_UINT32 At_CheckCharStar(TAF_UINT8 Char);
TAF_UINT32 At_CheckCharWell(TAF_UINT8 Char);
TAF_UINT32 At_CheckCharRightArrow(TAF_UINT8 Char);
TAF_UINT32 At_CheckCharG(TAF_UINT8 Char);
TAF_UINT32 At_CheckCharI(TAF_UINT8 Char);
TAF_UINT32 At_CheckCharS(TAF_UINT8 Char);
TAF_UINT32 At_CheckCharD(TAF_UINT8 Char);
TAF_UINT32 At_CheckBasicCmdName(TAF_UINT8 Char);

TAF_UINT32 At_CheckCharPlus(TAF_UINT8 Char);
TAF_UINT32 At_CheckLeftBracket(TAF_UINT8 Char);
TAF_UINT32 At_CheckRightBracket(TAF_UINT8 Char);
TAF_UINT32 At_CheckDigit(TAF_UINT8 Char);
TAF_UINT32 At_CheckLetter(TAF_UINT8 Char);
TAF_UINT32 At_CheckUpLetter(TAF_UINT8 Char);
TAF_UINT32 At_CheckChar(TAF_UINT8 Char);
TAF_UINT32 At_CheckAlpha(TAF_UINT8 Char);
TAF_UINT32 At_CheckColon(TAF_UINT8 Char);
TAF_UINT32 At_CheckCharSub(TAF_UINT8 Char);
TAF_UINT32 At_CheckQuot(TAF_UINT8 Char);
TAF_UINT32 At_CheckHorizontalLine(TAF_UINT8 Char);
TAF_UINT32 At_CheckEqu(TAF_UINT8 Char);
TAF_UINT32 At_CheckReq(TAF_UINT8 Char);
TAF_UINT32 At_CheckNoQuot(TAF_UINT8 Char);
TAF_UINT32 At_CheckHorizontalLinePara(AT_ParseParaType *para);
TAF_UINT32 At_CheckHexStrWithoutQuotationPara(AT_ParseParaType *para);
TAF_UINT32 At_CheckNumPara(AT_ParseParaType *para);
TAF_UINT32 At_CheckCharPara(AT_ParseParaType *para);
TAF_UINT32 At_CheckNumString(TAF_UINT8 *data, TAF_UINT16 len);
TAF_UINT32 At_CheckNumCharString(TAF_UINT8 *data, TAF_UINT16 len);
TAF_UINT32 At_CheckMmiString(TAF_UINT8 *data, TAF_UINT16 len);
TAF_UINT32 At_CheckChar0x26(TAF_UINT8 Char);
TAF_UINT32 At_CheckCharE(TAF_UINT8 Char);
TAF_UINT32 At_CheckChar1(TAF_UINT8 Char);
TAF_UINT32 At_CheckChar0(TAF_UINT8 Char);
TAF_UINT32 At_CheckCharV(TAF_UINT8 Char);
TAF_UINT32 At_CheckCharF(TAF_UINT8 Char);

TAF_UINT32 At_CheckRightArrowStr(TAF_UINT8 *data, TAF_UINT16 len);

VOS_UINT32 AT_CheckDateFormat(VOS_UINT8 *dateStr, VOS_UINT32 dateStrLen);

VOS_UINT32 AT_GetDate(VOS_UINT8 *dateStr, VOS_UINT32 dateStrLen, AT_DATE *date);

VOS_UINT32 AT_CheckDate(AT_DATE *date);

VOS_UINT32 At_CheckAndParsePara(AT_ParseParaType *atPara, VOS_UINT8 *begain, VOS_UINT8 *end);

VOS_UINT32 AT_CheckStrStartWith(VOS_UINT8 *lineStr,
                                VOS_UINT16 lineStrLen, VOS_UINT8 *pefixStr, VOS_UINT16 pefixStrLen
);

/* �Ƚϡ�ƥ����������� */
VOS_UINT32 atCheckBasicCmdName(VOS_UINT8 Char);

/* �Ƚϡ�ƥ��ո� */
VOS_UINT32 atCheckblank(VOS_UINT8 Char);

/* �Ƚϡ�ƥ�䶺�� */
VOS_UINT32 atCheckComma(VOS_UINT8 Char);

/* �Ƚϡ�ƥ��ð�� */
VOS_UINT32 atCheckColon(VOS_UINT8 Char);

VOS_UINT32 atCheckChar0x2f(VOS_UINT8 Char);

/* �Ƚϡ�ƥ���ַ�+ */
VOS_UINT32 atCheckCharPlus(VOS_UINT8 Char);

VOS_UINT32 atCheckChar(VOS_UINT8 Char);

VOS_UINT32 atCheckEqu(VOS_UINT8 Char);

VOS_UINT32 atCheckReq(VOS_UINT8 Char);

VOS_UINT32 atCheckQuot(VOS_UINT8 Char);

VOS_UINT32 atCheckNoQuot(VOS_UINT8 Char);

/* �Ƚϡ�ƥ���ַ������� */
VOS_UINT32 At_CheckStringPara(AT_ParseParaType *para);

/* �Ƚϡ�ƥ����ֵ���͵Ĳ��� */
VOS_UINT32 atCheckNumPara(AT_ParseParaType *para);

/* �Ƚϡ�ƥ���ַ���Χ���͵Ĳ��� */
VOS_UINT32 atCheckCharPara(AT_ParseParaType *para);

/* �Ƚϡ�ƥ������ */
VOS_UINT32 atCheckNumString(VOS_UINT8 *data, VOS_UINT16 len);

/* �ж��ַ������Ƿ���"*"��"#" */
VOS_UINT32 atCheckMmiString(VOS_UINT8 *data, VOS_UINT16 len);

/* �ж��ַ������Ƿ���">" */
VOS_UINT32 atCheckRightArrowStr(VOS_UINT8 *data, VOS_UINT16 len);

/*
 * ��������: �Ƚϡ�ƥ����˫���Ű����ַ�������
 */
extern VOS_UINT32 atCheckNoQuotStringPara(AT_ParseParaType *para);

/*
 * ��������: �Ƚϡ�ƥ����˫���Ű�����ĸ��ʼ
 */
extern VOS_UINT32 atNoQuotLetter(VOS_UINT8 Char);

/*
 * ��������: �Ƚϡ�ƥ��ʮ������(0x��0X)
 */
extern VOS_UINT32 atCheckHex(VOS_UINT8 Char);

/*
 * ��������: �Ƚϡ�ƥ��ʮ����������
 */
extern VOS_UINT32 atCheckHexNum(VOS_UINT8 Char);

extern VOS_UINT32 atfwParseSaveParam(VOS_UINT8 *stringBuf, VOS_UINT16 len);

extern VOS_UINT32 AT_SaveCdataDialParam(VOS_UINT8 *stringBuf, VOS_UINT16 len);

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* __AT_CHECK_FUNC_H__ */
