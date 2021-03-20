/* Copyright (C) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
 * foss@huawei.com
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General
 * Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the
 * implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the
 * Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the
 * following license terms apply:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that
 * the following conditions
 * are met:
 * 1) Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the
 *    following disclaimer.
 * 2) Redistributions in binary form must reproduce the above copyright
 *    notice, this
 *    list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 * 3) Neither the name of Huawei nor the names of its contributors may
 *    be used to endorse or
 *    promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/* lint -save -e537 -e734 -e813 -e958 -e718 -e746 */
#include "at_lte_ct_ex_proc.h"
#include "osm.h"
#include "gen_msg.h"

#include "at_lte_common.h"
#include "ATCmdProc.h"

#define AT_DIGIT_STR_MAX_NUM 5
#define AT_STRING_MAX_LEN 2048
#define AT_SIGNED_INTERGER_MAX_VALUE 0X7FFF

/*
 * 功能描述: 根据用户输入，解析参数列表并初始化相应的结构
 * 使用约束:
 * 1、只接受十进制数字字符串作为输入
 * 2、0作为数字开头，以及空格个数在满足所有checklist的条件下不做限制
 * 字符串解析Checklist:
 * 1、检查字符串总长度是否合法
 * 2、检查是否有非期待字符(空格和数字以外)
 * 3、检查字符串解析出的参数取值是否符合要求，包括数字字符串长度不能超过5个
 * 避免用于 000000123 ，这种非法输入
 * 4、检查字符串解析出的参数个数是否和用户输入的一致
 */
VOS_UINT32 initParaListS16(AT_ParseParaType *para, VOS_UINT16 listLen, VOS_INT16 *list)
{
    VOS_UINT32 tmp;
    VOS_UINT8 *paraData   = para->para;
    VOS_UINT8  digitNum   = 0;     /* 记录数字字符个数，不能超过5 */
    VOS_INT16  digitValue = 0;     /* 记录数字数值大小，不能超过65535 */
    VOS_UINT16 tmpListNum = 0;     /* 记录解析出来的数字个数，不能超过16 */
    VOS_BOOL   bDigit     = FALSE; /* 是否是数字 */
    VOS_BOOL   bNegative  = FALSE; /* 是否负数 */
    VOS_UINT32 rst        = ERR_MSP_SUCCESS;

    if ((para == NULL) || (para->paraLen > AT_STRING_MAX_LEN)) {
        /* 1、检查字符串总长度是否合法 */
        return ERR_MSP_INVALID_PARAMETER;
    }

    for (tmp = 0; tmp < para->paraLen; tmp++) {
        VOS_UINT8 chr = *paraData;

        if (isdigit(chr)) {
            /* 第一个字节为数字或者前面有空格 */
            if (!bDigit) {
                bDigit     = TRUE;
                digitNum   = 0;
                digitValue = 0;

                /* 4、检查字符串解析出的参数个数是否和用户输入的一致 */
                if (++tmpListNum > listLen) {
                    rst = ERR_MSP_INVALID_PARAMETER;
                    break;
                }
            }

            /* 3、检查字符串解析出的参数取值是否符合要求，包括数字字符串长度不能超过5个 */
            if ((++digitNum > AT_DIGIT_STR_MAX_NUM) ||
                ((AT_SIGNED_INTERGER_MAX_VALUE - digitValue * AT_DECIMAL_BASE_NUM) <
                (chr - AT_HEX_CONVERT_ASCII_NUM_BASE_VALUE))) {
                rst = ERR_MSP_INVALID_PARAMETER;
                break;
            }

            digitValue = (VOS_INT16)(digitValue * AT_DECIMAL_BASE_NUM +
                                     (chr - AT_HEX_CONVERT_ASCII_NUM_BASE_VALUE));

            list[tmpListNum - 1] = (VOS_INT16)((bNegative == FALSE) ? (digitValue) : (digitValue * (-1)));
        } else if (isspace(chr)) {
            /* 单个'-'符号的异常处理 */
            if (!bDigit && bNegative) {
                break;
            }

            bDigit    = FALSE;
            bNegative = FALSE;

            paraData++;
            continue;
        } else if ((chr == '-') && !bDigit && !bNegative) {
            bNegative = TRUE;

            paraData++;
            continue;
        } else {
            /* 2、检查是否有非期待字符(空格和数字以外) */
            rst = ERR_MSP_INVALID_PARAMETER;
            break;
        }
        paraData++;
    }

    /* 4、强制检测:检查字符串解析出的参数个数是否和用户输入的一致 */
    if (tmpListNum != listLen) {
        rst = ERR_MSP_INVALID_PARAMETER;
    }

    return rst;
}


/*lint -restore*/
