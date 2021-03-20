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

#include "AtParseSCmd.h"
#include "AtCheckFunc.h"

#include "msp_errno.h"


/* AT S�����ʼ״̬�� */
static const AT_SubState g_atSCmdNoneStateTab[] = {
    /* �����ǰ״̬��ATCMD_NONE_STATE��atCheckCharS�ɹ��������AT_S_CMD_NAME_STATE */
    { At_CheckCharS, AT_S_CMD_NAME_STATE },

    /* ��״̬����� */
    { NULL, AT_BUTT_STATE },
};

/* AT S������״̬�� */
static const AT_SubState g_atSCmdNameStateTab[] = {
    /* �����ǰ״̬��AT_S_CMD_NAME_STATE��atCheck_num�ɹ��������AT_S_CMD_NAME_STATE */
    { At_CheckDigit, AT_S_CMD_NAME_STATE },

    /* �����ǰ״̬��AT_S_CMD_NAME_STATE��atCheck_equ�ɹ��������AT_S_CMD_SET_STATE */
    { At_CheckEqu, AT_S_CMD_SET_STATE },

    /* �����ǰ״̬��AT_S_CMD_NAME_STATE��atCheck_req�ɹ��������AT_S_CMD_READ_STATE */
    { At_CheckReq, AT_S_CMD_READ_STATE },

    /* ��״̬����� */
    { NULL, AT_BUTT_STATE },
};

/* AT S��������״̬�� */
static const AT_SubState g_atSCmdSetStateTab[] = {
    /* �����ǰ״̬��AT_S_CMD_SET_STATE��atCheck_num�ɹ��������AT_S_CMD_PARA_STATE */
    { At_CheckDigit, AT_S_CMD_PARA_STATE },

    /* �����ǰ״̬��AT_S_CMD_SET_STATE��atCheck_req�ɹ��������AT_S_CMD_TEST_STATE */
    { At_CheckReq, AT_S_CMD_TEST_STATE },

    /* ��״̬����� */
    { NULL, AT_BUTT_STATE },
};

/* AT S�������״̬�� */
static const AT_SubState g_atSCmdParaStateTab[] = {
    /* �����ǰ״̬��AT_S_CMD_PARA_STATE��atCheck_num�ɹ��������AT_S_CMD_PARA_STATE */
    { At_CheckDigit, AT_S_CMD_PARA_STATE },

    /* ��״̬����� */
    { NULL, AT_BUTT_STATE },
};

/* AT S������״̬�� */
static const AT_MainState g_atSCmdMainStateTab[] = {
    /* �����ǰ״̬��AT_NONE_STATE�������g_atSCmdNoneStateTab��״̬�� */
    { AT_NONE_STATE, g_atSCmdNoneStateTab },

    /* �����ǰ״̬��ATCMD_B_CMD_STATE�������g_atSCmdNameStateTab��״̬�� */
    { AT_S_CMD_NAME_STATE, g_atSCmdNameStateTab },

    /* �����ǰ״̬��AT_S_CMD_SET_STATE�������g_atSCmdSetStateTab��״̬�� */
    { AT_S_CMD_SET_STATE, g_atSCmdSetStateTab },

    /* �����ǰ״̬��AT_S_CMD_PARA_STATE�������g_atSCmdParaStateTab��״̬�� */
    { AT_S_CMD_PARA_STATE, g_atSCmdParaStateTab },

    /* ��״̬����� */
    { AT_BUTT_STATE, NULL },
};



VOS_UINT32 atParseSCmd(VOS_UINT8 *data, VOS_UINT16 len)
{
    AT_StateType currState = AT_NONE_STATE; /*  ���ó�ʼ״̬ */
    AT_StateType newState  = AT_NONE_STATE; /*  ���ó�ʼ״̬ */
    VOS_UINT8   *currPtr   = data;          /*  ָ��ǰ���ڴ�����ַ� */
    VOS_UINT8   *copyPtr   = data;          /*  �������ݵ���ʼָ�� */
    VOS_UINT16   length    = 0;             /*  ��¼��ǰ�Ѿ�������ַ����� */

    /* ���η����ַ����е�ÿ���ַ� */
    /* ���αȽ�ÿ���ַ� */
    while ((length++ < len) && (g_atParseCmd.paraIndex < AT_MAX_PARA_NUMBER)) {
        currState = newState; /*  ��ǰ״̬����Ϊ��״̬ */

        /*  ���ݵ�ǰ������ַ��͵�ǰ״̬���õ���״̬ */
        newState = atFindNextMainState(g_atSCmdMainStateTab, *currPtr, currState);

        switch (newState) {
            /*  ״̬���� */
            case AT_S_CMD_NAME_STATE:                                     /*  S������״̬ */
                if (currState != newState) {                              /*  ��״̬���ֲ����ڵ�ǰ״̬ */
                    g_atParseCmd.cmdFmtType = AT_BASIC_CMD_TYPE;          /*  ������������ */
                    g_atParseCmd.cmdOptType = AT_CMD_OPT_SET_CMD_NO_PARA; /*  ��������������� */
                    copyPtr                 = currPtr;                    /*  ׼������S������ */
                }
                break;

            case AT_S_CMD_SET_STATE: /* AT S����Ⱥ�״̬ */

                /* �洢��������ȫ�ֱ����� */
                g_atParseCmd.cmdOptType = AT_CMD_OPT_SET_PARA_CMD; /* ��������������� */

                g_atParseCmd.cmdName.cmdNameLen = (VOS_UINT16)(currPtr - copyPtr);
                if (g_atParseCmd.cmdName.cmdNameLen < sizeof(g_atParseCmd.cmdName.cmdName)) {
                    atRangeCopy(g_atParseCmd.cmdName.cmdName, copyPtr, currPtr);
                } else {
                    return AT_ERROR; /* ���ش��� */
                }
                break;

            case AT_S_CMD_PARA_STATE:        /* AT S�������״̬ */
                if (currState != newState) { /* ��״̬���ֲ����ڵ�ǰ״̬ */
                    copyPtr = currPtr;       /* ׼������ S���������� */
                }
                break;

            case AT_S_CMD_READ_STATE: /* AT S�����ѯ����״̬ */

                /* �洢��������ȫ�ֱ����� */
                g_atParseCmd.cmdName.cmdNameLen = (VOS_UINT16)(currPtr - copyPtr);
                if (g_atParseCmd.cmdName.cmdNameLen < sizeof(g_atParseCmd.cmdName.cmdName)) {
                    atRangeCopy(g_atParseCmd.cmdName.cmdName, copyPtr, currPtr);
                } else {
                    return AT_ERROR; /* ���ش��� */
                }

                g_atParseCmd.cmdOptType = AT_CMD_OPT_READ_CMD; /* ��������������� */

                break;

            case AT_S_CMD_TEST_STATE:                          /* AT S������Բ���״̬ */
                g_atParseCmd.cmdOptType = AT_CMD_OPT_TEST_CMD; /* ��������������� */
                break;

            default:
                return AT_ERROR; /* ���ش��� */
        }
        currPtr++; /*  ����������һ���ַ� */
    }

    switch (newState) {
        /*  ��������״̬�ж� */
        case AT_S_CMD_PARA_STATE: /* AT S�������״̬ */

            /* �洢������ȫ�ֱ����� */
            if (atfwParseSaveParam(copyPtr, (VOS_UINT16)(currPtr - copyPtr)) != ERR_MSP_SUCCESS) {
                return AT_ERROR;
            }

            break;

        case AT_S_CMD_NAME_STATE: /* AT S����״̬ */
            /* �洢��������ȫ�ֱ����� */
            g_atParseCmd.cmdName.cmdNameLen = (VOS_UINT16)(currPtr - copyPtr);
            if (g_atParseCmd.cmdName.cmdNameLen < sizeof(g_atParseCmd.cmdName.cmdName)) {
                atRangeCopy(g_atParseCmd.cmdName.cmdName, copyPtr, currPtr);
            } else {
                return AT_ERROR; /* ���ش��� */
            }
            break;

        case AT_S_CMD_SET_STATE:  /* ��Ч״̬ */
        case AT_S_CMD_READ_STATE: /* ��ʼ״̬ */
        case AT_S_CMD_TEST_STATE: /* ��ʼ״̬ */
            break;

        default:
            return AT_ERROR; /* ���ش��� */
    }
    return AT_SUCCESS; /*  ������ȷ */
}

