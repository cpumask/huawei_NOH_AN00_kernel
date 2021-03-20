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

#ifndef _AT_MSG_PRINT_H_
#define _AT_MSG_PRINT_H_

#include "vos.h"
#include "AtCtx.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)

enum {
    AT_MSG_SERV_STATE_NOT_SUPPORT,
    AT_MSG_SERV_STATE_SUPPORT
};
typedef VOS_UINT8 AT_MSG_ServStateUint8;

enum AT_MSG_PARSE_STATUS {
    AT_MSG_PARSE_STATUS_START,
    AT_MSG_PARSE_STATUS_LOWERDATA,
    AT_MSG_PARSE_STATUS_WAITUPPER,
    AT_MSG_PARSE_STATUS_UPPERDATA,
    AT_MSG_PARSE_STATUS_END,
    AT_MSG_PARSE_STATUS_BUTT
};
typedef VOS_UINT32 AT_MSG_PARSE_STATUS_UINT32;

typedef VOS_UINT32 (*AT_MSG_STATUS_CHK_FUNC)(TAF_UINT8);

/*
 * 3类型定义
 */
typedef struct {
    AT_MSG_ServStateUint8 smsMT;
    AT_MSG_ServStateUint8 smsMO;
    AT_MSG_ServStateUint8 smsBM;
} AT_MSG_Serv;

#define AT_MAX_TIMEZONE_VALUE 48

#define At_GetNumTypeFromAddrType(numType, addrType) ((numType) = (((addrType) >> 4) & 0x07))
#define At_GetNumPlanFromAddrType(numPlan, addrType) ((numPlan) = ((addrType) & 0x0f))

#define AT_MSG_TP_MTI_DELIVER 0x00        /* SMS-DELIVER       */
#define AT_MSG_TP_MTI_DELIVER_REPORT 0x00 /* SMS-DELIVER-REPORT */
#define AT_MSG_TP_MTI_STATUS_REPORT 0x02  /* SMS-STATUS-REPORT */
#define AT_MSG_TP_MTI_COMMAND 0x02        /* SMS-COMMAND       */
#define AT_MSG_TP_MTI_SUBMIT 0x01         /* SMS-SUBMIT        */
#define AT_MSG_TP_MTI_SUBMIT_REPORT 0x01  /* SMS-SUBMIT-REPORT */
#define AT_MSG_TP_MTI_RESERVE 0x03        /* RESERVE           */

/* 23040 9.2.3.1 bits no 0 and 1 of the first octet of all PDUs */
#define AT_GET_MSG_TP_MTI(ucMti, ucFo) ((ucMti) = ((ucFo) & AT_MSG_TP_MTI_MASK))

#define AT_GET_MSG_TP_RD(bFlag, fo) ((bFlag) = ((fo) & 0x04) ? TAF_TRUE : TAF_FALSE)
/* 23040 9.2.3.2 bit no 2 of the first octet of SMS DELIVER and SMS STATUS REPORT */
#define AT_GET_MSG_TP_MMS(bFlag, fo) ((bFlag) = ((fo) & 0x04) ? TAF_FALSE : TAF_TRUE)

/* 23040 9.2.3.26 bit no. 5 of the first octet of SMS STATUS REPORT */
#define AT_GET_MSG_TP_SRQ(bFlag, fo) ((bFlag) = ((fo) & 0x20) ? TAF_TRUE : TAF_FALSE)

/* 23040 9.2.3.4 bit no. 5 of the first octet of SMS DELIVER */
#define AT_GET_MSG_TP_SRI(bFlag, fo) ((bFlag) = ((fo) & 0x20) ? TAF_TRUE : TAF_FALSE)

/* 23040 9.2.3.5 bit no. 5 of the first octet of SMS SUBMIT and SMS COMMAND */
#define AT_GET_MSG_TP_SRR(bFlag, fo) ((bFlag) = ((fo) & 0x20) ? TAF_TRUE : TAF_FALSE)

#define AT_GET_MSG_TP_UDHI(bFlag, fo) ((bFlag) = ((fo) & 0x40) ? TAF_TRUE : TAF_FALSE)

#define AT_GET_MSG_TP_RP(bFlag, fo) ((bFlag) = ((fo) & 0x80) ? TAF_TRUE : TAF_FALSE)

/* 23040 9.2.3.1 bits no 0 and 1 of the first octet of all PDUs */
#define AT_SET_MSG_TP_MTI(fo, ucMti) ((fo) |= ((ucMti) & 0x03))
/* 23040 9.2.3.25  1 bit field located within bit 2 of the first octet of SMS SUBMIT */
#define AT_SET_MSG_TP_RD(fo, bFlag) ((fo) |= (VOS_UINT8)((TAF_TRUE == (bFlag)) ? 0x04 : 0))
/* 23040 9.2.3.2 bit no 2 of the first octet of SMS DELIVER and SMS STATUS REPORT */
#define AT_SET_MSG_TP_MMS(fo, bFlag) ((fo) |= (VOS_UINT8)((TAF_TRUE == (bFlag)) ? 0 : 0x04))
/* 23040 9.2.3.3 bit no 3 and 4 of the first octet of SMS SUBMIT */
#define AT_SET_MSG_TP_VPF(fo, ucVpf) ((fo) |= (((ucVpf) << 3) & 0x18))
/* 23040 9.2.3.26 bit no. 5 of the first octet of SMS STATUS REPORT */
#define AT_SET_MSG_TP_SRQ(fo, bFlag) ((fo) |= (VOS_UINT8)((TAF_TRUE == (bFlag)) ? 0x20 : 0))
/* 23040 9.2.3.4 bit no. 5 of the first octet of SMS DELIVER */
#define AT_SET_MSG_TP_SRI(fo, bFlag) ((fo) |= (VOS_UINT8)((TAF_TRUE == (bFlag)) ? 0x20 : 0))
/* 23040 9.2.3.5 bit no. 5 of the first octet of SMS SUBMIT and SMS COMMAND */
#define AT_SET_MSG_TP_SRR(fo, bFlag) ((fo) |= (VOS_UINT8)((TAF_TRUE == (bFlag)) ? 0x20 : 0))
/* 23040 9.2.3.23 1 bit field within bit 6 of the first octet of SMS SUBMIT SMS-SUBMIT-REPORT SMS
 * DELIVER,SMS-DELIVER-REPORT SMS-STATUS-REPORT SMS-COMMAND. */
#define AT_SET_MSG_TP_UDHI(fo, bFlag) ((fo) |= (VOS_UINT8)((TAF_TRUE == (bFlag)) ? 0x40 : 0))
/* 23040 9.2.3.17 1 bit field, located within bit no 7 of the first octet of both SMS DELIVER and SMS SUBMIT, */
#define AT_SET_MSG_TP_RP(fo, bFlag) ((fo) |= (VOS_UINT8)((TAF_TRUE == (bFlag)) ? 0x80 : 0))

/*
 * Description: 比较、匹配结束符
 * History:
 *  1.Date: 2005-04-19
 *    Author:
 *    Modification: Created function
 */
extern VOS_UINT32 At_CheckEnd(VOS_UINT8 Char);

/*
 * Description: 比较、匹配连接符
 * History:
 *  1.Date: 2005-04-19
 *    Author:
 *    Modification: Created function
 */
extern VOS_UINT32 At_CheckJuncture(VOS_UINT8 Char);


extern VOS_UINT32 At_ParseCsmpFo(VOS_UINT8 *fo);


VOS_UINT32 At_ParseCsmpVp(VOS_UINT8 indexNum, MN_MSG_ValidPeriod *vp);

VOS_UINT32 AT_AsciiNumberToBcd(const VOS_CHAR *pcAsciiNumber, VOS_UINT8 *bcdNumber, VOS_UINT8 *bcdLen);

VOS_UINT32 AT_BcdNumberToAscii(const VOS_UINT8 *bcdNumber, VOS_UINT8 bcdLen, VOS_CHAR *pcAsciiNumber);

VOS_VOID AT_JudgeIsPlusSignInDialString(const VOS_CHAR *pcAsciiNumber, VOS_UINT16 len, VOS_UINT8 *isExistPlusSign,
                                        VOS_UINT8 *plusSignLocation);

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* _AT_MSG_PRINT_H_ */
