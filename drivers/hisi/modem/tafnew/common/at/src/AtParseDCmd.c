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

#include "AtParseDCmd.h"
#include "securec.h"
#include "AtCheckFunc.h"
#include "AtMntn.h"

#include "msp_errno.h"

#include "at_common.h"
#include "ps_tag.h"


/* AT�����ʼ״̬�� */
static const AT_SubState g_atDCmdNoneStateTab[] = {
    /* �����ǰ״̬��ATCMD_NONE_STATE��atCheckCharD�ɹ��������AT_D_CMD_NAME_STATE */
    { At_CheckCharD, AT_D_CMD_NAME_STATE },

    /* ��״̬����� */
    { NULL, AT_BUTT_STATE },
};

/* AT D����������״̬�� */
static const AT_SubState g_atDCmdNameStateTab[] = {
    /* �����ǰ״̬��AT_D_CMD_NAME_STATE��At_CheckCharRightArrow�ɹ��������AT_D_CMD_RIGHT_ARROW_STATE */
    { At_CheckCharRightArrow, AT_D_CMD_RIGHT_ARROW_STATE },

    /* �����ǰ״̬��AT_D_CMD_NAME_STATE��atCheckDailString�ɹ��������AT_D_CMD_DIALSTRING_STATE */
    { At_CheckDialNum, AT_D_CMD_DIALSTRING_STATE },

    /* ��״̬����� */
    { NULL, AT_BUTT_STATE },
};

/* AT D������ַ���״̬�� */
static const AT_SubState g_atDCmdDialstringStateTab[] = {
    /* �����ǰ״̬��AT_D_CMD_DIALSTRING_STATE��atCheckDailString�ɹ��������AT_D_CMD_DIALSTRING_STATE */
    { At_CheckDialNum, AT_D_CMD_DIALSTRING_STATE },

    /* �����ǰ״̬��AT_D_CMD_DIALSTRING_STATE��At_CheckCharI�ɹ��������AT_D_CMD_CHAR_I_STATE */
    { At_CheckCharI, AT_D_CMD_CHAR_I_STATE },

    /* �����ǰ״̬��AT_D_CMD_DIALSTRING_STATE��At_CheckCharG�ɹ��������AT_D_CMD_CHAR_G_STATE */
    { At_CheckCharG, AT_D_CMD_CHAR_G_STATE },

    /* �����ǰ״̬��AT_D_CMD_DIALSTRING_STATE��At_CheckSemicolon�ɹ��������AT_D_CMD_SEMICOLON_STATE */
    { At_CheckSemicolon, AT_D_CMD_SEMICOLON_STATE },

    /* ��״̬����� */
    { NULL, AT_BUTT_STATE },
};

/* AT D�����ַ�I״̬�� */
static const AT_SubState g_atDCmdCharIStateTab[] = {
    /* �����ǰ״̬��AT_D_CMD_CHAR_I_STATE��At_CheckCharG�ɹ��������AT_D_CMD_CHAR_G_STATE */
    { At_CheckCharG, AT_D_CMD_CHAR_G_STATE },

    /* �����ǰ״̬��AT_D_CMD_CHAR_I_STATE��At_CheckSemicolon�ɹ��������AT_D_CMD_SEMICOLON_STATE */
    { At_CheckSemicolon, AT_D_CMD_SEMICOLON_STATE },

    /* ��״̬����� */
    { NULL, AT_BUTT_STATE },
};

/* AT D�����ַ�G״̬�� */
static const AT_SubState g_atDCmdCharGStateTab[] = {
    /* �����ǰ״̬��AT_D_CMD_CHAR_G_STATE��At_CheckSemicolon�ɹ��������AT_D_CMD_SEMICOLON_STATE */
    { At_CheckSemicolon, AT_D_CMD_SEMICOLON_STATE },

    /* ��״̬����� */
    { NULL, AT_BUTT_STATE },
};

/* AT D�����Ҽ�ͷ״̬�� */
static const AT_SubState g_atDCmdRightArrowStateTab[] = {
    /* �����ǰ״̬��AT_D_CMD_RIGHT_ARROW_STATE��atCheck_quot�ɹ��������AT_D_CMD_LEFT_QUOT_STATE */
    { At_CheckQuot, AT_D_CMD_LEFT_QUOT_STATE },

    /* �����ǰ״̬��AT_D_CMD_RIGHT_ARROW_STATE��atCheck_num�ɹ��������AT_D_CMD_DIGIT_STATE */
    { At_CheckDigit, AT_D_CMD_DIGIT_STATE },

    /* �����ǰ״̬��AT_D_CMD_RIGHT_ARROW_STATE��At_CheckUpLetter�ɹ��������AT_D_CMD_CHAR_STATE */
    { At_CheckUpLetter, AT_D_CMD_CHAR_STATE },

    /* ��״̬����� */
    { NULL, AT_BUTT_STATE },
};

/* AT D������ĸ״̬�� */
static const AT_SubState g_atDCmdCharStateTab[] = {
    /* �����ǰ״̬��AT_D_CMD_CHAR_STATE��atCheckCharStr�ɹ��������AT_D_CMD_CHAR_STATE */
    { At_CheckUpLetter, AT_D_CMD_CHAR_STATE },

    /* �����ǰ״̬��AT_D_CMD_CHAR_STATE��atCheck_quot�ɹ��������AT_D_CMD_RIGHT_QUOT_STATE */
    { At_CheckDigit, AT_D_CMD_DIGIT_STATE },

    /* ��״̬����� */
    { NULL, AT_BUTT_STATE },
};

/* AT D����������״̬�� */
static const AT_SubState g_atDCmdLeftQuotStateTab[] = {
    /* �����ǰ״̬��AT_D_CMD_LEFT_QUOT_STATE��atCheckCharStr�ɹ��������AT_D_CMD_LEFT_QUOT_STATE */
    { At_CheckNoQuot, AT_D_CMD_LEFT_QUOT_STATE },

    /* �����ǰ״̬��AT_D_CMD_LEFT_QUOT_STATE��atCheck_quot�ɹ��������AT_D_CMD_RIGHT_QUOT_STATE */
    { At_CheckQuot, AT_D_CMD_RIGHT_QUOT_STATE },

    /* ��״̬����� */
    { NULL, AT_BUTT_STATE },
};

/* AT D����������״̬�� */
static const AT_SubState g_atDCmdRightQuotStateTab[] = {
    /* �����ǰ״̬��AT_D_CMD_RIGHT_ARROW_STATE��atCheck_num�ɹ��������AT_D_CMD_DIGIT_STATE */
    { At_CheckDigit, AT_D_CMD_DIGIT_STATE },

    /* �����ǰ״̬��AT_D_CMD_DIALSTRING_STATE��At_CheckCharI�ɹ��������AT_D_CMD_CHAR_I_STATE */
    { At_CheckCharI, AT_D_CMD_CHAR_I_STATE },

    /* �����ǰ״̬��AT_D_CMD_DIALSTRING_STATE��At_CheckCharG�ɹ��������AT_D_CMD_CHAR_G_STATE */
    { At_CheckCharG, AT_D_CMD_CHAR_G_STATE },

    /* �����ǰ״̬��AT_D_CMD_DIALSTRING_STATE��At_CheckSemicolon�ɹ��������AT_D_CMD_SEMICOLON_STATE */
    { At_CheckSemicolon, AT_D_CMD_SEMICOLON_STATE },

    /* ��״̬����� */
    { NULL, AT_BUTT_STATE },
};

/* AT������״̬�� */
static const AT_MainState g_atDCmdMainStateTab[] = {
    /* �����ǰ״̬��AT_NONE_STATE�������g_atDCmdNoneStateTab��״̬�� */
    { AT_NONE_STATE, g_atDCmdNoneStateTab },

    /* �����ǰ״̬��AT_D_CMD_NAME_STATE�������g_atDCmdNameStateTab��״̬�� */
    { AT_D_CMD_NAME_STATE, g_atDCmdNameStateTab },

    /* �����ǰ״̬��AT_D_CMD_DIALSTRING_STATE�������g_atDCmdDialstringStateTab��״̬�� */
    { AT_D_CMD_DIALSTRING_STATE, g_atDCmdDialstringStateTab },

    /* �����ǰ״̬��AT_D_CMD_CHAR_I_STATE�������g_atDCmdCharIStateTab��״̬�� */
    { AT_D_CMD_CHAR_I_STATE, g_atDCmdCharIStateTab },

    /* �����ǰ״̬��AT_D_CMD_CHAR_G_STATE�������g_atDCmdCharGStateTab��״̬�� */
    { AT_D_CMD_CHAR_G_STATE, g_atDCmdCharGStateTab },

    /* �����ǰ״̬��AT_D_CMD_CHAR_STATE�������g_atDCmdCharStateTab��״̬�� */
    { AT_D_CMD_CHAR_STATE, g_atDCmdCharStateTab },

    /* �����ǰ״̬��AT_D_CMD_RIGHT_ARROW_STATE�������g_atDCmdRightArrowStateTab��״̬�� */
    { AT_D_CMD_RIGHT_ARROW_STATE, g_atDCmdRightArrowStateTab },

    /* �����ǰ״̬��AT_D_CMD_LEFT_QUOT_STATE�������g_atDCmdLeftQuotStateTab��״̬�� */
    { AT_D_CMD_LEFT_QUOT_STATE, g_atDCmdLeftQuotStateTab },

    /* �����ǰ״̬��AT_D_CMD_RIGHT_QUOT_STATE�������g_atDCmdRightQuotStateTab��״̬�� */
    { AT_D_CMD_RIGHT_QUOT_STATE, g_atDCmdRightQuotStateTab },

    /* �����ǰ״̬��AT_D_CMD_DIGIT_STATE�������AT_D_CMD_DIGIT_STATE_TAB��״̬�� */
    { AT_D_CMD_DIGIT_STATE, g_atDCmdRightQuotStateTab },

    /* ��״̬����� */
    { AT_BUTT_STATE, NULL },
};



LOCAL VOS_UINT32 atParseDCmd(VOS_UINT8 *data, VOS_UINT16 len)
{
    AT_StateType currState = AT_NONE_STATE; /*  ���ó�ʼ״̬ */
    AT_StateType newState  = AT_NONE_STATE; /*  ���ó�ʼ״̬ */
    VOS_UINT8   *currPtr   = data;          /*  ָ��ǰ���ڴ�����ַ� */
    VOS_UINT8   *copyPtr   = data;          /*  �������ݵ���ʼָ�� */
    VOS_UINT16   length    = 0;             /*  ��¼��ǰ�Ѿ�������ַ����� */

    /* ���η����ַ����е�ÿ���ַ� */
    /* ���αȽ�ÿ���ַ� */
    while ((length++ < len) && (g_atParseCmd.paraIndex < AT_MAX_PARA_NUMBER)) {
        currState = newState; /* ��ǰ״̬����Ϊ��״̬ */

        /* ���ݵ�ǰ������ַ��͵�ǰ״̬���õ���״̬ */
        newState = atFindNextMainState(g_atDCmdMainStateTab, *currPtr, currState);

        switch (newState) {
            /* ״̬���� */
            case AT_D_CMD_NAME_STATE:                                 /* AT D����������״̬ */
                g_atParseCmd.cmdFmtType = AT_BASIC_CMD_TYPE;          /* ������������ */
                g_atParseCmd.cmdOptType = AT_CMD_OPT_SET_CMD_NO_PARA; /* ��������������� */
                copyPtr                 = currPtr;                    /* ׼������D���������� */
                break;

            case AT_D_CMD_RIGHT_ARROW_STATE: /* AT D�����Ҽ�ͷ״̬ */
            case AT_D_CMD_DIALSTRING_STATE:  /* AT D������ַ���״̬ */
                if (currState != newState) { /* ��״̬���ֲ����ڵ�ǰ״̬ */
                    g_atParseCmd.cmdName.cmdNameLen = (VOS_UINT16)(currPtr - copyPtr);
                    if (g_atParseCmd.cmdName.cmdNameLen < sizeof(g_atParseCmd.cmdName.cmdName)) {
                        atRangeCopy(g_atParseCmd.cmdName.cmdName, copyPtr, currPtr);
                    } else {
                        return AT_FAILURE; /* ���ش��� */
                    }
                    copyPtr = currPtr; /* ׼�����ղ��� */
                }
                break;

            case AT_D_CMD_DIGIT_STATE:     /* AT D��������״̬ */
            case AT_D_CMD_LEFT_QUOT_STATE: /* AT D����������״̬ */
            case AT_D_CMD_CHAR_STATE:
                if (currState != newState) { /* ��״̬���ֲ����ڵ�ǰ״̬ */
                    /* �洢������ȫ�ֱ����� */
                    if (atfwParseSaveParam(copyPtr, (VOS_UINT16)(currPtr - copyPtr)) != ERR_MSP_SUCCESS) {
                        return AT_FAILURE;
                    }

                    copyPtr = currPtr; /* ׼��������һ������ */
                }
                break;

            case AT_D_CMD_CHAR_I_STATE:    /* AT D�����ַ�I״̬ */
            case AT_D_CMD_CHAR_G_STATE:    /* AT D�����ַ�G״̬ */
            case AT_D_CMD_SEMICOLON_STATE: /* AT D����ֺ�״̬ */
                /* �洢������ȫ�ֱ����� */
                if (atfwParseSaveParam(copyPtr, (VOS_UINT16)(currPtr - copyPtr)) != ERR_MSP_SUCCESS) {
                    return AT_FAILURE;
                }

                copyPtr = currPtr; /* ׼��������һ������ */
                break;

            case AT_BUTT_STATE:    /* ��Ч״̬ */
                return AT_FAILURE; /* ���ش��� */

            default:
                break;
        }
        currPtr++; /* ����������һ���ַ� */
    }

    switch (newState) {
        /* ��������״̬�ж� */
        case AT_D_CMD_SEMICOLON_STATE: /* AT D����ֺ�״̬ */
            break;

        case AT_D_CMD_DIALSTRING_STATE: /* AT D������ַ���״̬ */
        case AT_D_CMD_DIGIT_STATE:      /* AT D��������״̬ */
        case AT_D_CMD_RIGHT_QUOT_STATE: /* AT D����������״̬ */
        case AT_D_CMD_CHAR_G_STATE:     /* AT D�����ַ�G״̬ */
        case AT_D_CMD_CHAR_I_STATE:     /* AT D�����ַ�I״̬ */
            /* �洢������ȫ�ֱ����� */
            if (atfwParseSaveParam(copyPtr, (VOS_UINT16)(currPtr - copyPtr)) != ERR_MSP_SUCCESS) {
                return AT_FAILURE;
            }

            break;

        default:
            return AT_FAILURE; /* ���ش��� */
    }
    return AT_SUCCESS; /* ������ȷ */
}


VOS_VOID atCdataParseDCmd(VOS_UINT8 *data, VOS_UINT16 len)
{
    g_atParseCmd.cmdFmtType = AT_BASIC_CMD_TYPE;          /* ������������ */
    g_atParseCmd.cmdOptType = AT_CMD_OPT_SET_CMD_NO_PARA; /* ��������������� */

    /* ����D���������� */
    g_atParseCmd.cmdName.cmdNameLen = 1;
    g_atParseCmd.cmdName.cmdName[0] = *data;
}



LOCAL VOS_VOID atScanDcmdModifier(VOS_UINT8 *data, VOS_UINT16 *len)
{
    VOS_UINT16 chkLen  = 0;
    VOS_UINT16 lenTemp = 0;
    VOS_UINT8 *write   = data;
    VOS_UINT8 *read    = data;

    /* ���������� */
    while (chkLen++ < *len) {
        /* ɾ�����η���',','T','P','!','W','@' */
        if ((*read != ',') && (*read != 'T') && (*read != 'P') && (*read != '!') && (*read != 'W') && (*read != '@')) {
            *write++ = *read;
            lenTemp++;
        }
        read++;
    }

    *len = lenTemp;
    return;
}


LOCAL VOS_VOID atScanDmcmdModifier(VOS_UINT8 *data, VOS_UINT16 *len)
{
    VOS_UINT8         *write   = VOS_NULL_PTR;
    VOS_UINT8         *read    = VOS_NULL_PTR;
    VOS_UINT8         *tmp     = VOS_NULL_PTR;
    VOS_UINT8         *dataTmp = VOS_NULL_PTR;
    const VOS_UINT8    str[]   = "*99**PPP";
    VOS_UINT16         lenTemp = 0;
    VOS_UINT16         strLen  = 0;

    dataTmp = (VOS_UINT8 *)AT_MALLOC(*len + 1);
    if (dataTmp == VOS_NULL_PTR) {
        PS_PRINTF_WARNING("<atScanDmcmdModifier> AT_MALLOC Failed!\n");
        return;
    }

    (VOS_VOID)memset_s(dataTmp, *len + 1, 0x00, *len + 1);

    atRangeCopy(dataTmp, data, (data + *len));

    /* ��֤�ַ����н����� */
    dataTmp[*len] = 0;

    read  = dataTmp;
    write = dataTmp;

    tmp = (VOS_UINT8 *)AT_STRSTR((VOS_CHAR *)dataTmp, (VOS_CHAR *)str);
    if (tmp != NULL) {
        strLen = (VOS_UINT16)AT_STRLEN((VOS_CHAR *)str);
        /* ���ǰһ����� */
        while (read < tmp) {
            /* ɾ�����η���',','T','P','!','W','@' */
            if ((*read != ',') && (*read != 'T') && (*read != 'P') && (*read != '!') && (*read != 'W') &&
                (*read != '@')) {
                *write++ = *read;
                lenTemp++;
            }
            read++;
        }
        while (read < (tmp + strLen)) {
            *write++ = *read++;
            lenTemp++;
        }
        while (read < (dataTmp + (*len))) {
            /* ɾ�����η���',','T','P','!','W','@' */
            if ((*read != ',') && (*read != 'T') && (*read != 'P') && (*read != '!') && (*read != 'W') &&
                (*read != '@')) {
                *write++ = *read;
                lenTemp++;
            }
            read++;
        }
    } else {
        /* ���������� */
        while (read < (dataTmp + (*len))) {
            /* ɾ�����η���',','T','P','!','W','@' */
            if ((*read != ',') && (*read != 'T') && (*read != 'P') && (*read != '!') && (*read != 'W') &&
                (*read != '@')) {
                *write++ = *read;
                lenTemp++;
            }
            read++;
        }
    }

    atRangeCopy(data, dataTmp, (dataTmp + *len));

    *len = lenTemp;

    AT_FREE(dataTmp);
    return;
}


LOCAL VOS_UINT32 ScanQuateChar(VOS_UINT8 *data, VOS_UINT16 *len)
{
    VOS_UINT8 *write   = data;
    VOS_UINT8 *read    = data;
    VOS_UINT16 chkLen  = 0;
    VOS_UINT16 lenTemp = 0;

    if (*len == 0) {
        return AT_FAILURE;
    }

    while (chkLen++ < *len) {
        /* ȥ������ */
        if (*read != '"') {
            *write++ = *read;
            lenTemp++;
        }
        read++;
    }
    *len = lenTemp;

    return AT_SUCCESS;
}


VOS_UINT32 ParseDCmdPreProc(VOS_UINT8 *data, VOS_UINT16 len)
{
    VOS_UINT16 tmpLen = len;
    VOS_UINT8 *tmp    = data;
    VOS_UINT32 result = AT_FAILURE;

    /* Ϊ�˹��LINUX�²��������д�������(ATDT"*99#")�����⣬������ȥ�� */
    ScanQuateChar(tmp, &tmpLen);

    {
        if (At_CheckCharWell(tmp[tmpLen - 1]) == AT_SUCCESS) { /* ���������#�Ž�β */
            atScanDmcmdModifier(tmp, &tmpLen);                 /* ɨ�����η� */

            /*
             * ��ΪҪ��������������gstATCmdName.usCmdNameLen���Ա�����ƥ�䣬
             * ���ԣ������ݿ����������б�ĵ�һ��������
             */
            if (atfwParseSaveParam(tmp, tmpLen) != ERR_MSP_SUCCESS) {
                return AT_CME_DIAL_STRING_TOO_LONG;
            }

            result = atParseDMCmd(tmp, tmpLen); /* ������ֺ� */
            if (result == AT_FAILURE) {
                return AT_ERROR;
            }
        }
        /* ����#777,�ж�"#777"�����ַ� */
        else if (AT_CheckStrStartWith(tmp, tmpLen, (VOS_UINT8 *)"D#777", AT_CDATA_DIAL_777_LEN + 1)) {
            result = AT_SaveCdataDialParam(tmp, tmpLen);

            if (result == AT_FAILURE) {
                return AT_ERROR;
            }

            atCdataParseDCmd(tmp, tmpLen);
        } else {
            if (atCheckRightArrowStr(tmp, tmpLen) == AT_FAILURE) {
                atScanDcmdModifier(tmp, &tmpLen); /* ɨ�����η� */
            }

            /*
             * ��ΪҪ��������������gstATCmdName.usCmdNameLen���Ա�����ƥ�䣬
             * ���ԣ������ݿ����������б�ĵ�һ��������
             */
            if (atfwParseSaveParam(tmp, tmpLen) != ERR_MSP_SUCCESS) {
                return AT_CME_DIAL_STRING_TOO_LONG;
            }

            result = atParseDCmd(tmp, tmpLen); /* D���� */

            if (result == AT_FAILURE) {
                return AT_ERROR;
            }
        }
    }

    return result;
}

