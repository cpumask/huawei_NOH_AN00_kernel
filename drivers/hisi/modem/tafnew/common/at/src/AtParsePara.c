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

#include "AtParsePara.h"
#include "securec.h"
#include "AtCheckFunc.h"


#define THIS_FILE_ID PS_FILE_ID_AT_PARSEPARA_C

/* ����ƥ���ʼ��״̬�� */
static const AT_SubState g_atParaNoneStateTab[] = {
    /* �����ǰ״̬��AT_PARA_NONE_STATE��atCheckLeftBracket�ɹ��������AT_PARA_LEFT_BRACKET_STATE */
    { At_CheckLeftBracket, AT_PARA_LEFT_BRACKET_STATE },

    /* ��״̬����� */
    { NULL, AT_BUTT_STATE },
};

/* ����ƥ��������״̬�� */
static const AT_SubState g_atParaLeftBracketStateTab[] = {
    /* �����ǰ״̬��AT_PARA_LEFT_BRACKET_STATE��atCheckZero�ɹ��������AT_PARA_ZERO_STATE */
    { At_CheckChar0, AT_PARA_ZERO_STATE },

    /* �����ǰ״̬��AT_PARA_LEFT_BRACKET_STATE��atCheckDigit�ɹ��������AT_PARA_NUM_STATE */
    { At_CheckDigit, AT_PARA_NUM_STATE },

    /* �����ǰ״̬��AT_PARA_LEFT_BRACKET_STATE��at_NoQuotLetter�ɹ��������AT_PARA_NO_QUOT_LETTER_STATE */
    { atNoQuotLetter, AT_PARA_NO_QUOT_LETTER_STATE },

    /* �����ǰ״̬��AT_PARA_LEFT_BRACKET_STATE��atCheckLetter�ɹ��������AT_PARA_LETTER_STATE */
    { At_CheckLetter, AT_PARA_LETTER_STATE },

    /* �����ǰ״̬��AT_PARA_LEFT_BRACKET_STATE��atCheckQuot�ɹ��������AT_PARA_LEFT_QUOT_STATE */
    { At_CheckQuot, AT_PARA_LEFT_QUOT_STATE },

    /* ��״̬����� */
    { NULL, AT_BUTT_STATE },
};

/* ����ƥ�����״̬�� */
static const AT_SubState g_atParaNumStateTab[] = {
    /* �����ǰ״̬��AT_PARA_NUM_STATE��atCheckDigit�ɹ��������AT_PARA_NUM_STATE */
    { At_CheckDigit, AT_PARA_NUM_STATE },

    /* �����ǰ״̬��AT_PARA_NUM_STATE��atCheckColon�ɹ��������AT_PARA_NUM_COLON_STATE */
    { atCheckComma, AT_PARA_NUM_COLON_STATE },

    /* �����ǰ״̬��AT_PARA_NUM_STATE��atCheckRightBracket�ɹ��������AT_PARA_RIGHT_BRACKET_STATE */
    { At_CheckRightBracket, AT_PARA_RIGHT_BRACKET_STATE },

    /* �����ǰ״̬��AT_PARA_NUM_STATE��atCheckCharSub�ɹ��������AT_PARA_SUB_STATE */
    { At_CheckCharSub, AT_PARA_SUB_STATE },

    /* ��״̬����� */
    { NULL, AT_BUTT_STATE },
};

/* ����ƥ�䷶Χ״̬�� */
static const AT_SubState g_atParaSubStateTab[] = {
    /* �����ǰ״̬��AT_PARA_SUB_STATE��atCheckZero�ɹ��������AT_PARA_ZERO_SUB_STATE */
    { At_CheckChar0, AT_PARA_ZERO_SUB_STATE },

    /* �����ǰ״̬��AT_PARA_SUB_STATE��atCheckDigit�ɹ��������AT_PARA_NUM_SUB_STATE */
    { At_CheckDigit, AT_PARA_NUM_SUB_STATE },

    /* ��״̬����� */
    { NULL, AT_BUTT_STATE },
};

/* ����ƥ�䷶Χ����״̬�� */
static const AT_SubState g_atParaNumSubStateTab[] = {
    /* �����ǰ״̬��AT_PARA_NUM_SUB_STATE��atCheckDigit�ɹ��������AT_PARA_NUM_SUB_STATE */
    { At_CheckDigit, AT_PARA_NUM_SUB_STATE },

    /* �����ǰ״̬��AT_PARA_NUM_SUB_STATE��atCheckColon�ɹ��������AT_PARA_NUM_SUB_COLON_STATE */
    { atCheckComma, AT_PARA_NUM_SUB_COLON_STATE },

    /* �����ǰ״̬��AT_PARA_NUM_SUB_STATE��atCheckRightBracket�ɹ��������AT_PARA_RIGHT_BRACKET_STATE */
    { At_CheckRightBracket, AT_PARA_RIGHT_BRACKET_STATE },

    /* ��״̬����� */
    { NULL, AT_BUTT_STATE },
};

/* ����ƥ�䷶Χ���ֶ���״̬�� */
static const AT_SubState g_atParaNumSubColonStateTab[] = {
    /* �����ǰ״̬��AT_PARA_NUM_SUB_COLON_STATE��atCheckZero�ɹ��������AT_PARA_ZERO_STATE */
    { At_CheckChar0, AT_PARA_ZERO_STATE },

    /* �����ǰ״̬��AT_PARA_NUM_SUB_COLON_STATE��atCheckDigit�ɹ��������AT_PARA_NUM_STATE */
    { At_CheckDigit, AT_PARA_NUM_STATE },

    /* ��״̬����� */
    { NULL, AT_BUTT_STATE },
};

/* ����ƥ��������״̬�� */
static const AT_SubState g_atParaRightBracketStateTab[] = {
    /* �����ǰ״̬��AT_PARA_RIGHT_BRACKET_STATE��atCheckColon�ɹ��������AT_PARA_COLON_STATE */
    { atCheckComma, AT_PARA_COLON_STATE },

    /* ��״̬����� */
    { NULL, AT_BUTT_STATE },
};

/* ����ƥ�䶺��״̬�� */
static const AT_SubState g_atParaColonStateTab[] = {
    /* �����ǰ״̬��AT_PARA_COLON_STATE��atCheckLeftBracket�ɹ��������AT_PARA_LEFT_BRACKET_STATE */
    { At_CheckLeftBracket, AT_PARA_LEFT_BRACKET_STATE },

    /* ��״̬����� */
    { NULL, AT_BUTT_STATE },
};

/* ����ƥ��������״̬�� */
static const AT_SubState g_atParaLeftQuotStateTab[] = {
    /* �����ǰ״̬��AT_PARA_LEFT_QUOT_STATE��atCheckNoQuot�ɹ��������AT_PARA_LEFT_QUOT_STATE */
    { At_CheckNoQuot, AT_PARA_LEFT_QUOT_STATE },

    /* �����ǰ״̬��AT_PARA_LEFT_QUOT_STATE��atCheckQuot�ɹ��������AT_PARA_RIGHT_QUOT_STATE */
    { At_CheckQuot, AT_PARA_RIGHT_QUOT_STATE },

    /* ��״̬����� */
    { NULL, AT_BUTT_STATE },
};

/* ����ƥ��������״̬�� */
static const AT_SubState g_atParaRightQuotStateTab[] = {
    /* �����ǰ״̬��AT_PARA_RIGHT_QUOT_STATE��atCheckRightBracket�ɹ��������AT_PARA_RIGHT_BRACKET_STATE */
    { At_CheckRightBracket, AT_PARA_RIGHT_BRACKET_STATE },

    /* �����ǰ״̬��AT_PARA_RIGHT_QUOT_STATE��atCheckColon�ɹ��������AT_PARA_QUOT_COLON_STATE */
    { atCheckComma, AT_PARA_QUOT_COLON_STATE },

    /* ��״̬����� */
    { NULL, AT_BUTT_STATE },
};

/* ����ƥ����ĸ״̬�� */
static const AT_SubState g_atParaLetterStateTab[] = {
    /* �����ǰ״̬��AT_PARA_LETTER_STATE��atCheckLetter�ɹ��������AT_PARA_LETTER_STATE */
    { At_CheckLetter, AT_PARA_LETTER_STATE },

    /* �����ǰ״̬��AT_PARA_LETTER_STATE��atCheckRightBracket�ɹ��������AT_PARA_RIGHT_BRACKET_STATE */
    { At_CheckRightBracket, AT_PARA_RIGHT_BRACKET_STATE },

    /* �����ǰ״̬��AT_PARA_LETTER_STATE��atCheckCharSub�ɹ��������AT_PARA_LETTER_STATE */
    { At_CheckCharSub, AT_PARA_LETTER_STATE },

    /* �����ǰ״̬��AT_PARA_LETTER_STATE��atCheckColon�ɹ��������AT_PARA_LETTER_STATE */
    { atCheckColon, AT_PARA_LETTER_STATE },

    /* �����ǰ״̬��AT_PARA_LETTER_STATE��atCheckblank�ɹ��������AT_PARA_LETTER_STATE */
    { atCheckblank, AT_PARA_LETTER_STATE },

    /* ��״̬����� */
    { NULL, AT_BUTT_STATE },
};

/* ����ƥ�����ֶ���״̬�� */
static const AT_SubState g_atParaNumColonStateTab[] = {
    /* �����ǰ״̬��AT_PARA_NUM_COLON_STATE��atCheckZero�ɹ��������AT_PARA_ZERO_STATE */
    { At_CheckChar0, AT_PARA_ZERO_STATE },

    /* �����ǰ״̬��AT_PARA_NUM_COLON_STATE��atCheckDigit�ɹ��������AT_PARA_NUM_STATE */
    { At_CheckDigit, AT_PARA_NUM_STATE },

    /* ��״̬����� */
    { NULL, AT_BUTT_STATE },
};

/* ����ƥ�����Ŷ���״̬�� */
static const AT_SubState g_atParaQuotColonStateTab[] = {
    /* �����ǰ״̬��AT_PARA_QUOT_COLON_STATE��atCheckQuot�ɹ��������AT_PARA_LEFT_QUOT_STATE */
    { At_CheckQuot, AT_PARA_LEFT_QUOT_STATE },

    /* ��״̬����� */
    { NULL, AT_BUTT_STATE },
};

/* ֧�ְ˽��ơ�ʮ�����Ʋ��� BEGIN */

/* ����ƥ������0״̬�� */
static const AT_SubState g_atParaZeroStateTab[] = {
    /* �����ǰ״̬��AT_PARA_ZERO_STATE��at_CheckHex�ɹ��������AT_PARA_HEX_STATE */
    { atCheckHex, AT_PARA_HEX_STATE },

    /* �����ǰ״̬��AT_PARA_ZERO_STATE��atCheckDigit�ɹ��������AT_PARA_NUM_STATE */
    { At_CheckDigit, AT_PARA_NUM_STATE },

    /* �����ǰ״̬��AT_PARA_ZERO_STATE��atCheckColon�ɹ��������AT_PARA_NUM_COLON_STATE */
    { atCheckComma, AT_PARA_NUM_COLON_STATE },

    /* �����ǰ״̬��AT_PARA_ZERO_STATE��atCheckRightBracket�ɹ��������AT_PARA_RIGHT_BRACKET_STATE */
    { At_CheckRightBracket, AT_PARA_RIGHT_BRACKET_STATE },

    /* �����ǰ״̬��AT_PARA_ZERO_STATE��atCheckCharSub�ɹ��������AT_PARA_SUB_STATE */
    { At_CheckCharSub, AT_PARA_SUB_STATE },

    /* ��״̬����� */
    { NULL, AT_BUTT_STATE },
};

/* ����ƥ��ʮ������״̬�� */
static const AT_SubState g_atParaHexStateTab[] = {
    /* �����ǰ״̬��AT_PARA_HEX_STATE��at_CheckHex�ɹ��������AT_PARA_HEX_NUM_STATE */
    { atCheckHexNum, AT_PARA_HEX_NUM_STATE },

    /* ��״̬����� */
    { NULL, AT_BUTT_STATE },
};

/* ����ƥ��ʮ����������״̬�� */
static const AT_SubState g_atParaHexNumStateTab[] = {
    /* �����ǰ״̬��AT_PARA_ZERO_STATE��at_CheckHex�ɹ��������AT_PARA_HEX_STATE */
    { atCheckHexNum, AT_PARA_HEX_NUM_STATE },

    /* �����ǰ״̬��AT_PARA_ZERO_STATE��atCheckColon�ɹ��������AT_PARA_NUM_COLON_STATE */
    { atCheckComma, AT_PARA_NUM_COLON_STATE },

    /* �����ǰ״̬��AT_PARA_ZERO_STATE��atCheckRightBracket�ɹ��������AT_PARA_RIGHT_BRACKET_STATE */
    { At_CheckRightBracket, AT_PARA_RIGHT_BRACKET_STATE },

    /* �����ǰ״̬��AT_PARA_ZERO_STATE��atCheckCharSub�ɹ��������AT_PARA_SUB_STATE */
    { At_CheckCharSub, AT_PARA_SUB_STATE },

    /* ��״̬����� */
    { NULL, AT_BUTT_STATE },
};

/* ����ƥ�䷶Χ����0״̬�� */
static const AT_SubState g_atParaZeroSubStateTab[] = {
    /* �����ǰ״̬��AT_PARA_ZERO_SUB_STATE��at_CheckHex�ɹ��������AT_PARA_HEX_SUB_STATE */
    { atCheckHex, AT_PARA_HEX_SUB_STATE },

    /* �����ǰ״̬��AT_PARA_NUM_SUB_STATE��atCheckDigit�ɹ��������AT_PARA_NUM_SUB_STATE */
    { At_CheckDigit, AT_PARA_NUM_SUB_STATE },

    /* �����ǰ״̬��AT_PARA_NUM_SUB_STATE��atCheckColon�ɹ��������AT_PARA_NUM_SUB_COLON_STATE */
    { atCheckComma, AT_PARA_NUM_SUB_COLON_STATE },

    /* �����ǰ״̬��AT_PARA_NUM_SUB_STATE��atCheckRightBracket�ɹ��������AT_PARA_RIGHT_BRACKET_STATE */
    { At_CheckRightBracket, AT_PARA_RIGHT_BRACKET_STATE },

    /* ��״̬����� */
    { NULL, AT_BUTT_STATE },
};

/* ����ƥ�䷶Χʮ������״̬�� */
static const AT_SubState g_atParaHexSubStateTab[] = {
    /* �����ǰ״̬��AT_PARA_HEX_SUB_STATE��at_CheckHex�ɹ��������AT_PARA_HEX_NUM_SUB_STATE */
    { atCheckHexNum, AT_PARA_HEX_NUM_SUB_STATE },

    /* ��״̬����� */
    { NULL, AT_BUTT_STATE },
};

/* ����ƥ�䷶Χʮ����������״̬�� */
static const AT_SubState g_atParaHexNumSubStateTab[] = {
    /* �����ǰ״̬��AT_PARA_HEX_NUM_SUB_STATE��at_CheckHexNum�ɹ��������AT_PARA_HEX_NUM_SUB_STATE */
    { atCheckHexNum, AT_PARA_HEX_NUM_SUB_STATE },

    /* �����ǰ״̬��AT_PARA_NUM_SUB_STATE��atCheckColon�ɹ��������AT_PARA_NUM_SUB_COLON_STATE */
    { atCheckComma, AT_PARA_NUM_SUB_COLON_STATE },

    /* �����ǰ״̬��AT_PARA_NUM_SUB_STATE��atCheckRightBracket�ɹ��������AT_PARA_RIGHT_BRACKET_STATE */
    { At_CheckRightBracket, AT_PARA_RIGHT_BRACKET_STATE },

    /* ��״̬����� */
    { NULL, AT_BUTT_STATE },
};

/* ֧�ְ˽��ơ�ʮ�����Ʋ��� END */

/* ֧����˫���Ű����ַ��� BEGIN */

/* ����ƥ����˫���Ű�����ĸ״̬�� */
static const AT_SubState g_atParaNoQuotLetterStateTab[] = {
    /* �����ǰ״̬��AT_PARA_NO_QUOT_LETTER_STATE��atCheckLetter�ɹ��������AT_PARA_NO_QUOT_LETTER_STATE */
    { At_CheckLetter, AT_PARA_NO_QUOT_LETTER_STATE },

    /* �����ǰ״̬��AT_PARA_NO_QUOT_LETTER_STATE��atCheckRightBracket�ɹ��������AT_PARA_RIGHT_BRACKET_STATE */
    { At_CheckRightBracket, AT_PARA_RIGHT_BRACKET_STATE },

    /* ��״̬����� */
    { NULL, AT_BUTT_STATE },
};

/* ֧����˫���Ű����ַ��� END */

/* ����ƥ����״̬�� */
static const AT_MainState g_atMainParaStateTab[] = {
    /* �����ǰ״̬��AT_NONE_STATE�������g_atParaNoneStateTab��״̬�� */
    { AT_NONE_STATE, g_atParaNoneStateTab },

    /* �����ǰ״̬��AT_PARA_LEFT_BRACKET_STATE�������g_atParaLeftBracketStateTab��״̬�� */
    { AT_PARA_LEFT_BRACKET_STATE, g_atParaLeftBracketStateTab },

    /* �����ǰ״̬��AT_PARA_RIGHT_BRACKET_STATE�������g_atParaRightBracketStateTab��״̬�� */
    { AT_PARA_RIGHT_BRACKET_STATE, g_atParaRightBracketStateTab },

    /* �����ǰ״̬��AT_PARA_LETTER_STATE�������g_atParaLetterStateTab��״̬�� */
    { AT_PARA_LETTER_STATE, g_atParaLetterStateTab },

    /* �����ǰ״̬��AT_PARA_NUM_STATE�������g_atParaNumStateTab��״̬�� */
    { AT_PARA_NUM_STATE, g_atParaNumStateTab },

    /* �����ǰ״̬��AT_PARA_NUM_COLON_STATE�������g_atParaNumColonStateTab��״̬�� */
    { AT_PARA_NUM_COLON_STATE, g_atParaNumColonStateTab },

    /* �����ǰ״̬��AT_PARA_SUB_STATE�������g_atParaSubStateTab��״̬�� */
    { AT_PARA_SUB_STATE, g_atParaSubStateTab },

    /* �����ǰ״̬��AT_PARA_NUM_SUB_STATE�������g_atParaNumSubStateTab��״̬�� */
    { AT_PARA_NUM_SUB_STATE, g_atParaNumSubStateTab },

    /* �����ǰ״̬��AT_PARA_NUM_SUB_COLON_STATE�������g_atParaNumSubColonStateTab��״̬�� */
    { AT_PARA_NUM_SUB_COLON_STATE, g_atParaNumSubColonStateTab },

    /* �����ǰ״̬��AT_PARA_COLON_STATE�������g_atParaColonStateTab��״̬�� */
    { AT_PARA_COLON_STATE, g_atParaColonStateTab },

    /* �����ǰ״̬��AT_PARA_LEFT_QUOT_STATE�������g_atParaLeftQuotStateTab��״̬�� */
    { AT_PARA_LEFT_QUOT_STATE, g_atParaLeftQuotStateTab },

    /* �����ǰ״̬��AT_PARA_RIGHT_QUOT_STATE�������g_atParaRightQuotStateTab��״̬�� */
    { AT_PARA_RIGHT_QUOT_STATE, g_atParaRightQuotStateTab },

    /* �����ǰ״̬��AT_PARA_QUOT_COLON_STATE�������g_atParaQuotColonStateTab��״̬�� */
    { AT_PARA_QUOT_COLON_STATE, g_atParaQuotColonStateTab },

    /* �����ǰ״̬��AT_PARA_ZERO_STATE�������g_atParaZeroStateTab��״̬�� */
    { AT_PARA_ZERO_STATE, g_atParaZeroStateTab },

    /* �����ǰ״̬��AT_PARA_ZERO_SUB_STATE�������g_atParaZeroSubStateTab��״̬�� */
    { AT_PARA_ZERO_SUB_STATE, g_atParaZeroSubStateTab },

    /* �����ǰ״̬��AT_PARA_HEX_STATE�������g_atParaHexStateTab��״̬�� */
    { AT_PARA_HEX_STATE, g_atParaHexStateTab },

    /* �����ǰ״̬��AT_PARA_HEX_NUM_STATE�������g_atParaHexNumStateTab��״̬�� */
    { AT_PARA_HEX_NUM_STATE, g_atParaHexNumStateTab },

    /* �����ǰ״̬��AT_PARA_HEX_SUB_STATE�������g_atParaHexSubStateTab��״̬�� */
    { AT_PARA_HEX_SUB_STATE, g_atParaHexSubStateTab },

    /* �����ǰ״̬��AT_PARA_HEX_NUM_SUB_STATE�������g_atParaHexNumSubStateTab��״̬�� */
    { AT_PARA_HEX_NUM_SUB_STATE, g_atParaHexNumSubStateTab },

    /* �����ǰ״̬��AT_PARA_NO_QUOT_LETTER_STATE�������g_atParaNoQuotLetterStateTab��״̬�� */
    { AT_PARA_NO_QUOT_LETTER_STATE, g_atParaNoQuotLetterStateTab },

    /* ��״̬����� */
    { AT_BUTT_STATE, NULL },
};

pAtChkFuncType g_atCheckFunc = NULL;



VOS_UINT32 atCmparePara(VOS_VOID)
{
    /* ���������� */
    if (g_atCheckFunc != NULL) {
        /* �������б��Ӧ�Ĳ����Ƿ���� */
        if (g_atParaList[g_atParseCmd.paraCheckIndex].paraLen > 0) {
            /* ������������� */
            if (g_atCheckFunc(&g_atParaList[g_atParseCmd.paraCheckIndex]) != AT_SUCCESS) {
                return AT_FAILURE; /*  ���ش��� */
            }
        }

        /* ��¼�Ѽ�������ȫ�ֱ�����1 */
        g_atParseCmd.paraCheckIndex++;

        /* ��գ��Ա��Ƚ���һ������,���򣬶ԱȲ����������� */
        if (g_atParseCmd.paraNumRangeIndex != 0) {
            g_atParseCmd.paraNumRangeIndex = 0;
            (VOS_VOID)memset_s(g_atParseCmd.paraNumRange, sizeof(g_atParseCmd.paraNumRange), 0x00,
                     sizeof(g_atParseCmd.paraNumRange));
        }

        if (g_atParseCmd.paraStrRangeIndex != 0) {
            g_atParseCmd.paraStrRangeIndex = 0;
            (VOS_VOID)memset_s(g_atParseCmd.strRange, sizeof(g_atParseCmd.strRange), 0x00, sizeof(g_atParseCmd.strRange));
        }

        g_atCheckFunc = NULL;

        return AT_SUCCESS; /*  ������ȷ */
    } else {
        return AT_FAILURE; /*  ���ش��� */
    }
}



VOS_UINT32 atParsePara(VOS_UINT8 *data, VOS_UINT16 len)
{
    VOS_UINT16   length    = 0;             /*  ��¼��ǰ�Ѿ�������ַ����� */
    VOS_UINT8   *currPtr   = data;          /*  ָ��ǰ���ڴ�����ַ� */
    VOS_UINT8   *copyPtr   = data;          /*  �������ݵ���ʼָ�� */
    AT_StateType currState = AT_NONE_STATE; /*  ���ó�ʼ״̬ */
    AT_StateType newState  = AT_NONE_STATE; /*  ���ó�ʼ״̬ */

    /* ���η����ַ����е�ÿ���ַ� */
    /* ���αȽ�ÿ���ַ� */
    while ((length++ < len) && (g_atParseCmd.paraNumRangeIndex < AT_MAX_PARA_NUMBER) &&
           (g_atParseCmd.paraStrRangeIndex < AT_MAX_PARA_NUMBER)) {
        currState = newState; /*  ��ǰ״̬����Ϊ��״̬ */

        /* ���ݵ�ǰ������ַ��͵�ǰ״̬���õ���״̬ */
        newState = atFindNextMainState(g_atMainParaStateTab, *currPtr, currState);

        switch (newState) {
            /* ״̬���� */
            case AT_PARA_NUM_STATE:            /* ����״̬ */
            case AT_PARA_LETTER_STATE:         /* ��ĸ״̬ */
            case AT_PARA_LEFT_QUOT_STATE:      /* ����������״̬ */
            case AT_PARA_NUM_SUB_STATE:        /* ��Χ����״̬ */
            case AT_PARA_ZERO_STATE:           /* ����0״̬ */
            case AT_PARA_ZERO_SUB_STATE:       /* ��Χ����0״̬ */
            case AT_PARA_NO_QUOT_LETTER_STATE: /* ��˫���Ű�����ĸ״̬ */
                if (currState != newState) {   /* ��״̬���ֲ����ڵ�ǰ״̬ */
                    copyPtr = currPtr;         /*  ׼�����ղ��� */
                }
                break;

            case AT_PARA_HEX_STATE: /* ʮ������״̬ */
                /* continue */
            case AT_PARA_HEX_NUM_STATE: /* ʮ����������״̬ */
                /* continue */
            case AT_PARA_HEX_SUB_STATE: /* ƥ�䷶Χʮ������״̬ */
                /* continue */
            case AT_PARA_HEX_NUM_SUB_STATE: /* ƥ�䷶Χʮ����������״̬ */
                break;

            case AT_PARA_QUOT_COLON_STATE: /* ���Ŷ���״̬ */

                /* �������ַ������ű��б� */
                /* �ַ�����������1 */

                if ((VOS_UINT32)(currPtr - copyPtr) < sizeof(g_atParseCmd.strRange[g_atParseCmd.paraStrRangeIndex])) {
                    atRangeCopy(g_atParseCmd.strRange[g_atParseCmd.paraStrRangeIndex++], copyPtr, currPtr);
                } else {
                    return AT_FAILURE; /* ���ش��� */
                }
                break;

            case AT_PARA_NUM_SUB_COLON_STATE:

                /* ת�����ֿ��������ֲ����ű��б���Ͻ� */
                /* ���ֲ���������1 */
                g_atParseCmd.paraNumRange[g_atParseCmd.paraNumRangeIndex++].big = atRangeToU32(copyPtr, currPtr);

                break;

            case AT_PARA_NUM_COLON_STATE:

                /* ת�����ֿ��������ֲ����ű��б���½� */
                g_atParseCmd.paraNumRange[g_atParseCmd.paraNumRangeIndex].small = atRangeToU32(copyPtr, currPtr);

                /* �Ͻ�����½� */
                g_atParseCmd.paraNumRange[g_atParseCmd.paraNumRangeIndex].big =
                    g_atParseCmd.paraNumRange[g_atParseCmd.paraNumRangeIndex].small;

                /* ���ֲ���������1 */
                g_atParseCmd.paraNumRangeIndex++;

                break;

            case AT_PARA_SUB_STATE:

                /* ת�����ֿ��������ֲ����ű��б���½� */
                g_atParseCmd.paraNumRange[g_atParseCmd.paraNumRangeIndex].small = atRangeToU32(copyPtr, currPtr);

                break;

            case AT_PARA_RIGHT_BRACKET_STATE: /* ����ƥ��������״̬ */
                switch (currState) {
                    case AT_PARA_NUM_SUB_STATE: /* ����ƥ�䷶Χ����״̬ */
                        /* continue */
                    case AT_PARA_ZERO_SUB_STATE:
                        /* continue */
                    case AT_PARA_HEX_NUM_SUB_STATE:

                        /* ת�����ֿ��������ֲ����ű��б���Ͻ� */
                        /* ���ֲ���������1 */
                        g_atParseCmd.paraNumRange[g_atParseCmd.paraNumRangeIndex++].big = atRangeToU32(copyPtr,
                                                                                                       currPtr);

                        g_atCheckFunc = atCheckNumPara; /* ���ò����ȽϺ��� */

                        break;

                    case AT_PARA_NUM_STATE:
                        /* continue */
                    case AT_PARA_ZERO_STATE:
                        /* continue */
                    case AT_PARA_HEX_NUM_STATE:

                        /* ת�����ֿ��������ֲ����ű��б���½� */
                        g_atParseCmd.paraNumRange[g_atParseCmd.paraNumRangeIndex].small = atRangeToU32(copyPtr,
                                                                                                       currPtr);

                        /* �Ͻ�����½� */
                        g_atParseCmd.paraNumRange[g_atParseCmd.paraNumRangeIndex].big =
                            g_atParseCmd.paraNumRange[g_atParseCmd.paraNumRangeIndex].small;

                        /* ���ֲ���������1 */
                        g_atParseCmd.paraNumRangeIndex++;

                        g_atCheckFunc = atCheckNumPara; /* ���ò����ȽϺ��� */

                        break;

                    case AT_PARA_LETTER_STATE:

                        /* �������ַ������ű��б� */
                        if ((VOS_UINT32)(currPtr - copyPtr) <
                            sizeof(g_atParseCmd.strRange[g_atParseCmd.paraStrRangeIndex])) {
                            atRangeCopy(g_atParseCmd.strRange[g_atParseCmd.paraStrRangeIndex++], copyPtr, currPtr);
                        } else {
                            return AT_FAILURE; /* ���ش��� */
                        }

                        /* ���ò����ȽϺ��� */
                        g_atCheckFunc = At_CheckStringPara;

                        break;

                    case AT_PARA_NO_QUOT_LETTER_STATE: /* ��˫���Ű�����ĸ״̬ */

                        /* �������ַ������ű��б� */
                        if ((VOS_UINT32)(currPtr - copyPtr) <
                            sizeof(g_atParseCmd.strRange[g_atParseCmd.paraStrRangeIndex])) {
                            atRangeCopy(g_atParseCmd.strRange[g_atParseCmd.paraStrRangeIndex], copyPtr, currPtr);
                        } else {
                            return AT_FAILURE; /* ���ش��� */
                        }

                        /* ���ò����ȽϺ��� */
                        g_atCheckFunc = atCheckNoQuotStringPara;

                        break;

                    default: /* AT_PARA_RIGHT_QUOT_STATE */

                        /* �������ַ������ű��б� */
                        if ((VOS_UINT32)(currPtr - copyPtr) <
                            sizeof(g_atParseCmd.strRange[g_atParseCmd.paraStrRangeIndex])) {
                            atRangeCopy(g_atParseCmd.strRange[g_atParseCmd.paraStrRangeIndex++], copyPtr, currPtr);
                        } else {
                            return AT_FAILURE; /* ���ش��� */
                        }

                        /* ���ò����ȽϺ��� */
                        g_atCheckFunc = atCheckCharPara;

                        break;
                }

                if (atCmparePara() == AT_FAILURE) { /* �����Ƚ� */
                    return AT_FAILURE;              /* ���ش��� */
                }
                break;

            case AT_BUTT_STATE:    /* ��Ч״̬ */
                return AT_FAILURE; /* ���ش��� */

            default:
                break;
        }
        currPtr++; /*  ����������һ���ַ� */
    }

    if (newState == AT_NONE_STATE) { /* ��ʼ״̬ */
        return AT_FAILURE;           /* ���ش��� */
    }

    return AT_SUCCESS; /*  ������ȷ */
}

