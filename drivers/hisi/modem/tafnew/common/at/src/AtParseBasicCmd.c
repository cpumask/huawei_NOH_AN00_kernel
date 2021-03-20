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
#include "AtParseBasicCmd.h"
#include "AtCheckFunc.h"

#include "msp_errno.h"


/* AT�����ʼ��״̬�� */
static const AT_SubState g_atBCmdNoneStateTab[] = {
    /* �����ǰ״̬��ATCMD_NONE_STATE��atCheckBasicCmdName�ɹ��������AT_B_CMD_NAME_STATE */
    { atCheckBasicCmdName, AT_B_CMD_NAME_STATE },

    /* ��״̬����� */
    { NULL, AT_BUTT_STATE },
};

/* AT����������״̬�� */
static const AT_SubState g_atBCmdNameStateTab[] = {
    /* �����ǰ״̬��AT_B_CMD_NAME_STATE��atCheckBasicCmdName�ɹ��������AT_B_CMD_NAME_STATE */
    { atCheckBasicCmdName, AT_B_CMD_NAME_STATE },

    /* �����ǰ״̬��AT_B_CMD_NAME_STATE��At_CheckDigit�ɹ��������AT_B_CMD_PARA_STATE */
    { At_CheckDigit, AT_B_CMD_PARA_STATE },

    /* ��״̬����� */
    { NULL, AT_BUTT_STATE },
};

/* AT�����������״̬�� */
static const AT_SubState g_atBCmdParaStateTab[] = {
    /* �����ǰ״̬��AT_B_CMD_NAME_STATE��At_CheckDigit�ɹ��������AT_B_CMD_PARA_STATE */
    { At_CheckDigit, AT_B_CMD_PARA_STATE },

    /* ��״̬����� */
    { NULL, AT_BUTT_STATE },
};

/* AT����������״̬�� */
static const AT_MainState g_atBCmdMainStateTab[] = {
    /* �����ǰ״̬��AT_NONE_STATE�������g_atBCmdNoneStateTab��״̬�� */
    { AT_NONE_STATE, g_atBCmdNoneStateTab },

    /* �����ǰ״̬��AT_B_CMD_NAME_STATE�������g_atBCmdNameStateTab��״̬�� */
    { AT_B_CMD_NAME_STATE, g_atBCmdNameStateTab },

    /* �����ǰ״̬��AT_B_CMD_PARA_STATE�������g_atBCmdParaStateTab��״̬�� */
    { AT_B_CMD_PARA_STATE, g_atBCmdParaStateTab },

    /* ��״̬����� */
    { AT_BUTT_STATE, NULL },
};



VOS_UINT32 atParseBasicCmd(VOS_UINT8 *data, VOS_UINT16 len)
{
    AT_StateType currState = AT_NONE_STATE; /*  ���ó�ʼ״̬ */
    AT_StateType newState  = AT_NONE_STATE; /*  ���ó�ʼ״̬ */
    VOS_UINT8   *currPtr   = data;          /*  ָ��ǰ���ڴ�����ַ� */
    VOS_UINT8   *copyPtr   = data;          /*  �������ݵ���ʼָ�� */
    VOS_UINT16   length    = 0;             /*  ��¼��ǰ�Ѿ�������ַ����� */

    if (atCheckBasicCmdName(*currPtr) == AT_FAILURE) {
        return AT_ERROR;
    }

    /* ���η����ַ����е�ÿ���ַ� */
    /* ���αȽ�ÿ���ַ� */
    while ((length++ < len) && (g_atParseCmd.paraIndex < AT_MAX_PARA_NUMBER)) {
        currState = newState; /*  ��ǰ״̬����Ϊ��״̬ */

        /*  ���ݵ�ǰ������ַ��͵�ǰ״̬���õ���״̬ */
        newState = atFindNextMainState(g_atBCmdMainStateTab, *currPtr, currState);

        switch (newState) {
            /*  ״̬���� */
            case AT_B_CMD_NAME_STATE:                                     /*  ����������״̬ */
                if (currState != newState) {                              /*  ��״̬���ֲ����ڵ�ǰ״̬ */
                    g_atParseCmd.cmdFmtType = AT_BASIC_CMD_TYPE;          /*  ������������ */
                    g_atParseCmd.cmdOptType = AT_CMD_OPT_SET_CMD_NO_PARA; /*  ��������������� */
                    copyPtr                 = currPtr;                    /*  ׼�����ջ������������� */
                }
                break;

            case AT_B_CMD_PARA_STATE:        /* AT�����������״̬ */
                if (currState != newState) { /* ��״̬���ֲ����ڵ�ǰ״̬ */
                    /* �洢��������ȫ�ֱ����� */
                    g_atParseCmd.cmdName.cmdNameLen = (VOS_UINT16)(currPtr - copyPtr);
                    if (g_atParseCmd.cmdName.cmdNameLen < sizeof(g_atParseCmd.cmdName.cmdName)) {
                        atRangeCopy(g_atParseCmd.cmdName.cmdName, copyPtr, currPtr);
                    } else {
                        return AT_FAILURE; /* ���ش��� */
                    }

                    g_atParseCmd.cmdOptType = AT_CMD_OPT_SET_PARA_CMD; /* ��������������� */
                    copyPtr                 = currPtr;                 /* ׼�����ղ��� */
                }
                break;

            default:
                return AT_FAILURE; /* ���ش��� */
        }
        currPtr++; /*  ����������һ���ַ� */
    }

    switch (newState) {
        /*  ��������״̬�ж� */
        case AT_B_CMD_NAME_STATE: /*  ����������״̬ */

            /* �洢��������ȫ�ֱ����� */
            g_atParseCmd.cmdName.cmdNameLen = (VOS_UINT16)(currPtr - copyPtr);
            if (g_atParseCmd.cmdName.cmdNameLen < sizeof(g_atParseCmd.cmdName.cmdName)) {
                atRangeCopy(g_atParseCmd.cmdName.cmdName, copyPtr, currPtr);
            } else {
                return AT_FAILURE; /* ���ش��� */
            }
            break;

        case AT_B_CMD_PARA_STATE: /*  ����������״̬ */
            /* �洢������ȫ�ֱ����� */
            if (atfwParseSaveParam(copyPtr, (VOS_UINT16)(currPtr - copyPtr)) != ERR_MSP_SUCCESS) {
                return AT_FAILURE;
            }

            break;

        default:               /* ��������״̬ */
            return AT_FAILURE; /* ���ش��� */
    }

    return AT_SUCCESS; /*  ������ȷ */
}

