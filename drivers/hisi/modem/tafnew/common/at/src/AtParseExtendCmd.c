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

#include "AtParseExtendCmd.h"
#include "securec.h"
#include "ATCmdProc.h"
#include "AtCheckFunc.h"
#include "AtParseCmd.h"


/*
 * Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
 */
#define THIS_FILE_ID PS_FILE_ID_AT_PARSEEXTENDCMD_C

/* AT��չ�����ʼ״̬״̬�� */
static const AT_SubState g_atECmdNoneStateTab[] = {
    /* �����ǰ״̬��ATCMD_T_STATE��atCheckCharPlus�ɹ��������AT_E_CMD_NAME_STATE */
    { At_CheckCharPlus, AT_E_CMD_NAME_STATE },

    /* ��״̬����� */
    { NULL, AT_BUTT_STATE },
};

/* AT��չ������״̬�� */
static const AT_SubState g_atECmdNameStateTab[] = {
    /* �����ǰ״̬��AT_E_CMD_NAME_STATE��atCheck_char�ɹ��������AT_E_CMD_NAME_STATE */
    { At_CheckChar, AT_E_CMD_NAME_STATE },

    /* �����ǰ״̬��AT_E_CMD_NAME_STATE��atCheck_equ�ɹ��������AT_E_CMD_SET_STATE */
    { At_CheckEqu, AT_E_CMD_SET_STATE },

    /* �����ǰ״̬��AT_E_CMD_NAME_STATE��atCheck_req�ɹ��������AT_E_CMD_READ_STATE */
    { At_CheckReq, AT_E_CMD_READ_STATE },

    /* ��״̬����� */
    { NULL, AT_BUTT_STATE },
};

/* AT��չ����Ⱥ�״̬�� */
static const AT_SubState g_atECmdSetStateTab[] = {
    /* �����ǰ״̬��AT_E_CMD_SET_STATE��atCheck_req�ɹ��������AT_E_CMD_TEST_STATE */
    { At_CheckReq, AT_E_CMD_TEST_STATE },

    /* �����ǰ״̬��AT_E_CMD_SET_STATE��atCheck_char�ɹ��������AT_E_CMD_PARA_STATE */
    { At_CheckChar, AT_E_CMD_PARA_STATE },

    /* �����ǰ״̬��AT_E_CMD_SET_STATE��atCheck_colon�ɹ��������AT_E_CMD_COLON_STATE */
    { atCheckComma, AT_E_CMD_COLON_STATE },

    /* �����ǰ״̬��AT_E_CMD_SET_STATE��atCheck_quot�ɹ��������AT_E_CMD_LEFT_QUOT_STATE */
    { At_CheckQuot, AT_E_CMD_LEFT_QUOT_STATE },

    /* ��״̬����� */
    { NULL, AT_BUTT_STATE },
};

/* AT��չ�������״̬�� */
static const AT_SubState g_atECmdParaStateTab[] = {
    /* �����ǰ״̬��AT_E_CMD_PARA_STATE��atCheck_char�ɹ��������AT_E_CMD_PARA_STATE */
    { At_CheckChar, AT_E_CMD_PARA_STATE },

    /* �����ǰ״̬��AT_E_CMD_PARA_STATE��atCheck_char�ɹ��������AT_E_CMD_PARA_STATE */
    { atCheckblank, AT_E_CMD_PARA_STATE },

    /* �����ǰ״̬��AT_E_CMD_PARA_STATE��atCheckColon�ɹ��������AT_E_CMD_PARA_STATE */
    { atCheckColon, AT_E_CMD_PARA_STATE },

    /* �����ǰ״̬��AT_E_CMD_PARA_STATE��atCheckComma�ɹ��������AT_E_CMD_COLON_STATE */
    { atCheckComma, AT_E_CMD_COLON_STATE },

    /* ��״̬����� */
    { NULL, AT_BUTT_STATE },
};

/* AT��չ�����״̬�� */
static const AT_SubState g_atECmdColonStateTab[] = {
    /* �����ǰ״̬��AT_E_CMD_COLON_STATE��atCheck_char�ɹ��������AT_E_CMD_PARA_STATE */
    { At_CheckChar, AT_E_CMD_PARA_STATE },

    /* �����ǰ״̬��AT_E_CMD_COLON_STATE��atCheck_colon�ɹ��������AT_E_CMD_COLON_STATE */
    { atCheckComma, AT_E_CMD_COLON_STATE },

    /* �����ǰ״̬��AT_E_CMD_COLON_STATE��atCheck_quot�ɹ��������AT_E_CMD_LEFT_QUOT_STATE */
    { At_CheckQuot, AT_E_CMD_LEFT_QUOT_STATE },

    /* ��״̬����� */
    { NULL, AT_BUTT_STATE },
};

/* AT��չ����������״̬�� */
static const AT_SubState g_atECmdLeftQuotStateTab[] = {
    /* �����ǰ״̬��AT_E_CMD_LEFT_QUOT_STATE��atCheck_char�ɹ��������AT_E_CMD_LEFT_QUOT_STATE */
    { At_CheckNoQuot, AT_E_CMD_LEFT_QUOT_STATE },

    /* �����ǰ״̬��AT_E_CMD_LEFT_QUOT_STATE��atCheck_quot�ɹ��������AT_E_CMD_RIGHT_QUOT_STATE */
    { At_CheckQuot, AT_E_CMD_RIGHT_QUOT_STATE },

    /* ��״̬����� */
    { NULL, AT_BUTT_STATE },
};

/* AT��չ����������״̬�� */
static const AT_SubState g_atECmdRightQuotStateTab[] = {
    /* �����ǰ״̬��AT_E_CMD_RIGHT_QUOT_STATE��atCheck_colon�ɹ��������AT_E_CMD_COLON_STATE */
    { atCheckComma, AT_E_CMD_COLON_STATE },

    /* �����ǰ״̬��AT_E_CMD_RIGHT_QUOT_STATE��atCheck_char�ɹ��������AT_E_CMD_RIGHT_QUOT_STATE */
    { At_CheckChar, AT_E_CMD_RIGHT_QUOT_STATE },

    /* �����ǰ״̬��AT_E_CMD_RIGHT_QUOT_STATE��atCheck_quot�ɹ��������AT_E_CMD_RIGHT_QUOT_STATE */
    { At_CheckQuot, AT_E_CMD_RIGHT_QUOT_STATE },

    /* ��״̬����� */
    { NULL, AT_BUTT_STATE },
};

/* AT������״̬�� */
static const AT_MainState g_atECmdMainStateTab[] = {
    /* �����ǰ״̬��AT_NONE_STATE�������g_atECmdNoneStateTab��״̬�� */
    { AT_NONE_STATE, g_atECmdNoneStateTab },

    /* �����ǰ״̬��AT_E_CMD_NAME_STATE�������g_atECmdNameStateTab��״̬�� */
    { AT_E_CMD_NAME_STATE, g_atECmdNameStateTab },

    /* �����ǰ״̬��AT_E_CMD_SET_STATE�������g_atECmdSetStateTab��״̬�� */
    { AT_E_CMD_SET_STATE, g_atECmdSetStateTab },

    /* �����ǰ״̬��AT_E_CMD_PARA_STATE�������g_atECmdParaStateTab��״̬�� */
    { AT_E_CMD_PARA_STATE, g_atECmdParaStateTab },

    /* �����ǰ״̬��AT_E_CMD_COLON_STATE�������g_atECmdColonStateTab��״̬�� */
    { AT_E_CMD_COLON_STATE, g_atECmdColonStateTab },

    /* �����ǰ״̬��AT_E_CMD_LEFT_QUOT_STATE�������g_atECmdLeftQuotStateTab��״̬�� */
    { AT_E_CMD_LEFT_QUOT_STATE, g_atECmdLeftQuotStateTab },

    /* �����ǰ״̬��AT_E_CMD_RIGHT_QUOT_STATE�������g_atECmdRightQuotStateTab��״̬�� */
    { AT_E_CMD_RIGHT_QUOT_STATE, g_atECmdRightQuotStateTab },

    /* ��״̬����� */
    { AT_BUTT_STATE, NULL },
};



VOS_UINT32 atParseExtendCmd(VOS_UINT8 *data, VOS_UINT16 len)
{
    AT_StateType currState = AT_NONE_STATE; /*  ���ó�ʼ״̬ */
    AT_StateType newState  = AT_NONE_STATE; /*  ���ó�ʼ״̬ */
    VOS_UINT8   *currPtr   = data;          /*  ָ��ǰ���ڴ�����ַ� */
    VOS_UINT8   *copyPtr   = data;          /*  �������ݵ���ʼָ�� */
    VOS_UINT16   length    = 0;             /*  ��¼��ǰ�Ѿ�������ַ����� */

    /* ���η����ַ����е�ÿ���ַ� */
    while (length++ < len) {
        /* ���αȽ�ÿ���ַ� */
        currState = newState; /* ��ǰ״̬����Ϊ��״̬ */

        /*  ���ݵ�ǰ������ַ��͵�ǰ״̬���õ���״̬ */
        newState = atFindNextMainState(g_atECmdMainStateTab, *currPtr, currState);

        switch (newState) {
            /* ״̬���� */
            case AT_E_CMD_NAME_STATE:                                     /* ��չ������״̬ */
                if (currState != newState) {                              /* ��״̬���ֲ����ڵ�ǰ״̬ */
                    g_atParseCmd.cmdFmtType = AT_EXTEND_CMD_TYPE;         /* ������������ */
                    g_atParseCmd.cmdOptType = AT_CMD_OPT_SET_CMD_NO_PARA; /* ��������������� */

                    copyPtr = currPtr; /* ׼��������չ���������� */
                }
                break;

            case AT_E_CMD_SET_STATE: /* AT��չ����Ⱥ�״̬ */
                /* �洢��������ȫ�ֱ����� */
                g_atParseCmd.cmdOptType         = AT_CMD_OPT_SET_PARA_CMD; /* ��������������� */
                g_atParseCmd.cmdName.cmdNameLen = (VOS_UINT16)(currPtr - copyPtr);
                if (g_atParseCmd.cmdName.cmdNameLen < sizeof(g_atParseCmd.cmdName.cmdName)) {
                    atRangeCopy(g_atParseCmd.cmdName.cmdName, copyPtr, currPtr);
                } else {
                    return AT_ERROR; /* ���ش��� */
                }
                break;

            case AT_E_CMD_PARA_STATE:        /* AT��չ�������״̬ */
            case AT_E_CMD_LEFT_QUOT_STATE:   /* AT��չ����������״̬ */
                if (currState != newState) { /* ��״̬���ֲ����ڵ�ǰ״̬ */
                    copyPtr = currPtr;       /* ׼�����ղ������� */
                }
                break;

            case AT_E_CMD_COLON_STATE:                                              /* AT��չ�����״̬ */
                if ((currState != newState) && (currState != AT_E_CMD_SET_STATE)) { /* ��״̬���ֲ����ڵ�ǰ״̬ */
                    /* �洢������ȫ�ֱ����� */
                    if (atfwParseSaveParam(copyPtr, (VOS_UINT16)(currPtr - copyPtr)) != ERR_MSP_SUCCESS) {
                        return AT_ERROR;
                    }

                } else {
                    /* ������������,���������� */
                    if (atfwParseSaveParam(copyPtr, 0) != ERR_MSP_SUCCESS) {
                        return AT_ERROR;
                    }
                }
                break;

            case AT_E_CMD_READ_STATE: /* AT��չ�����ѯ����״̬ */

                /* �洢��������ȫ�ֱ����� */
                g_atParseCmd.cmdName.cmdNameLen = (VOS_UINT16)(currPtr - copyPtr);
                if (g_atParseCmd.cmdName.cmdNameLen < sizeof(g_atParseCmd.cmdName.cmdName)) {
                    atRangeCopy(g_atParseCmd.cmdName.cmdName, copyPtr, currPtr);
                } else {
                    return AT_ERROR; /* ���ش��� */
                }

                g_atParseCmd.cmdOptType = AT_CMD_OPT_READ_CMD; /* ��������������� */

                break;

            case AT_E_CMD_TEST_STATE:                          /* AT��չ������Բ���״̬ */
                g_atParseCmd.cmdOptType = AT_CMD_OPT_TEST_CMD; /* ��������������� */
                break;

            case AT_BUTT_STATE:  /* ��Ч״̬ */
                return AT_ERROR; /* ���ش��� */

            default:
                break;
        }
        currPtr++; /* ����������һ���ַ� */
    }

    switch (newState) {
        /* ��������״̬�ж� */
        case AT_E_CMD_PARA_STATE:       /* AT��չ�������״̬ */
        case AT_E_CMD_RIGHT_QUOT_STATE: /* AT��չ����������״̬ */

            /* �洢������ȫ�ֱ����� */
            if (atfwParseSaveParam(copyPtr, (VOS_UINT16)(currPtr - copyPtr)) != ERR_MSP_SUCCESS) {
                return AT_ERROR;
            }

            break;

        case AT_E_CMD_NAME_STATE: /* AT��չ�����ѯ����״̬ */

            /* �洢��������ȫ�ֱ����� */
            g_atParseCmd.cmdName.cmdNameLen = (VOS_UINT16)(currPtr - copyPtr);
            if (g_atParseCmd.cmdName.cmdNameLen < sizeof(g_atParseCmd.cmdName.cmdName)) {
                atRangeCopy(g_atParseCmd.cmdName.cmdName, copyPtr, currPtr);
            } else {
                return AT_ERROR; /* ���ش��� */
            }

            g_atParseCmd.cmdOptType = AT_CMD_OPT_SET_CMD_NO_PARA; /* ��������������� */

            break;

        case AT_E_CMD_COLON_STATE: /* AT�����������޲εĶ��� */

            if (atfwParseSaveParam(copyPtr, 0) != ERR_MSP_SUCCESS) {
                return AT_ERROR;
            }

            break;

        case AT_E_CMD_SET_STATE:  /* AT��չ����Ⱥ�״̬ */
        case AT_E_CMD_TEST_STATE: /* AT��չ�����ѯ����״̬ */
        case AT_E_CMD_READ_STATE: /* AT��չ������Բ���״̬ */
            break;

        default:
            return AT_ERROR; /* ���ش��� */
    }
    return AT_SUCCESS; /*  ������ȷ */
}


VOS_UINT32 AT_ParseSetDockCmd(VOS_UINT8 *data, VOS_UINT16 len)
{
    VOS_INT8            ret;
    VOS_UINT16          cmdlen;
    VOS_UINT32          pos;
    errno_t             memResult;
    VOS_UINT8          *dataPara = VOS_NULL_PTR;
    AT_ParseCmdNameType atCmdName;

    (VOS_VOID)memset_s(&atCmdName, sizeof(atCmdName), 0x00, sizeof(atCmdName));

    if (len == 0) {
        return AT_FAILURE;
    }

    dataPara = (VOS_UINT8 *)PS_MEM_ALLOC(WUEPS_PID_AT, len);

    if (dataPara == VOS_NULL_PTR) {
        AT_ERR_LOG("AT_ParseSetDockCmd: pucDataPara Memory malloc failed!");
        return AT_FAILURE;
    }

    (VOS_VOID)memset_s(dataPara, len, 0x00, len);

    memResult = memcpy_s(dataPara, len, data, len);
    TAF_MEM_CHK_RTN_VAL(memResult, len, len);

    /* ȥ���ַ����е��˸�� */
    if (At_ScanDelChar(dataPara, &len) == AT_FAILURE) {
        PS_MEM_FREE(WUEPS_PID_AT, dataPara);
        return AT_FAILURE;
    }

    /* ��������ַ�������С��"AT^DOCK="����ֱ�ӷ���AT_FAILURE */
    cmdlen = (VOS_UINT16)VOS_StrLen("AT^DOCK=");
    if (len < cmdlen) {
        PS_MEM_FREE(WUEPS_PID_AT, dataPara);
        return AT_FAILURE;
    }

    /* AT����ͷ�ַ�ת��д */
    At_UpString(dataPara, cmdlen);

    /* ��������ַ���ͷ������"AT^DOCK="ֱ�ӷ���AT_FAILURE */
    ret = VOS_StrNiCmp((VOS_CHAR *)dataPara, "AT^DOCK=", cmdlen);
    if (ret != 0) {
        PS_MEM_FREE(WUEPS_PID_AT, dataPara);
        return AT_FAILURE;
    }

    /* ��ȡ����(����������ǰ׺AT)���Ƽ����� */
    pos                  = VOS_StrLen("AT");
    atCmdName.cmdNameLen = (VOS_UINT16)VOS_StrLen("^DOCK");
    memResult = memcpy_s(atCmdName.cmdName, sizeof(atCmdName.cmdName), (dataPara + pos), atCmdName.cmdNameLen);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(atCmdName.cmdName), atCmdName.cmdNameLen);
    atCmdName.cmdName[atCmdName.cmdNameLen] = '\0';
    pos += atCmdName.cmdNameLen;

    pos += VOS_StrLen("=");

    /* ��ȡ����Ĳ����ַ��������� */
    g_atParaList[0].paraLen = len - (VOS_UINT16)pos;
    if (g_atParaList[0].paraLen > 0) {
        memResult = memcpy_s(g_atParaList[0].para, sizeof(g_atParaList[0].para), (dataPara + pos),
                             g_atParaList[0].paraLen);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(g_atParaList[0].para), g_atParaList[0].paraLen);
    }

    /* �����������ͣ��������ͺͲ������� */
    g_atParaIndex           = 1;
    g_atParseCmd.cmdOptType = AT_CMD_OPT_SET_PARA_CMD;
    g_atCmdFmtType          = AT_EXTEND_CMD_TYPE;

    PS_MEM_FREE(WUEPS_PID_AT, dataPara);
    return AT_SUCCESS;
}

