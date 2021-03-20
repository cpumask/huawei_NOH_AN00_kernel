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

#ifndef _AT_PARSE_CMD_H_
#define _AT_PARSE_CMD_H_

#include "taf_type_def.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)

#define AT_CHECK_BASE_HEX 16
#define AT_CHECK_BASE_OCT 8
#define AT_CHECK_BASE_DEC 10

typedef enum {
    AT_NONE_STATE, /* ��ʼ״̬ */

    AT_B_CMD_NAME_STATE, /* AT����������״̬ */
    AT_B_CMD_PARA_STATE, /* AT�����������״̬ */

    AT_D_CMD_NAME_STATE,        /* AT D����������״̬ */
    AT_D_CMD_DIGIT_STATE,       /* AT D��������״̬ */
    AT_D_CMD_CHAR_STATE,        /* AT D������ĸ״̬ */
    AT_D_CMD_RIGHT_ARROW_STATE, /* AT D�����Ҽ�ͷ״̬ */
    AT_D_CMD_DIALSTRING_STATE,  /* AT D������ַ���״̬ */
    AT_D_CMD_SEMICOLON_STATE,   /* AT D����ֺ�״̬ */
    AT_D_CMD_CHAR_G_STATE,      /* AT D�����ַ�G״̬ */
    AT_D_CMD_CHAR_I_STATE,      /* AT D�����ַ�I״̬ */
    AT_D_CMD_LEFT_QUOT_STATE,   /* AT D����������״̬ */
    AT_D_CMD_RIGHT_QUOT_STATE,  /* AT D����������״̬ */

    AT_DM_CMD_NAME_STATE,
    AT_DM_CMD_STAR_STATE, /* AT D�����һ��*״̬ */
    AT_DM_CMD_WELL_STATE, /* AT D�����ַ�#״̬ */
    AT_DM_CMD_NUM_STATE,  /* AT D�����ַ�#״̬ */

    AT_S_CMD_NAME_STATE, /* AT S������״̬ */
    AT_S_CMD_SET_STATE,  /* AT S��������״̬ */
    AT_S_CMD_READ_STATE, /* AT S�����ѯ״̬ */
    AT_S_CMD_TEST_STATE, /* AT S�������״̬ */
    AT_S_CMD_PARA_STATE, /* AT S�������״̬ */

    AT_E_CMD_NAME_STATE,       /* AT��չ������״̬ */
    AT_E_CMD_SET_STATE,        /* AT��չ����Ⱥ�״̬ */
    AT_E_CMD_TEST_STATE,       /* AT��չ�����ѯ����״̬ */
    AT_E_CMD_READ_STATE,       /* AT��չ������Բ���״̬ */
    AT_E_CMD_PARA_STATE,       /* AT��չ�������״̬ */
    AT_E_CMD_COLON_STATE,      /* AT��չ�����״̬ */
    AT_E_CMD_LEFT_QUOT_STATE,  /* AT��չ����������״̬ */
    AT_E_CMD_RIGHT_QUOT_STATE, /* AT��չ����������״̬ */

    AT_PARA_LEFT_BRACKET_STATE,   /* ����ƥ��������״̬ */
    AT_PARA_NUM_STATE,            /* ����ƥ�����״̬ */
    AT_PARA_LETTER_STATE,         /* ����ƥ����ĸ״̬ */
    AT_PARA_NUM_COLON_STATE,      /* ����ƥ�����ֶ���״̬ */
    AT_PARA_NUM_SUB_STATE,        /* ����ƥ�䷶Χ����״̬ */
    AT_PARA_NUM_SUB_COLON_STATE,  /* ����ƥ�䷶Χ���ֶ���״̬ */
    AT_PARA_QUOT_COLON_STATE,     /* ����ƥ�����Ŷ���״̬ */
    AT_PARA_RIGHT_BRACKET_STATE,  /* ����ƥ��������״̬ */
    AT_PARA_SUB_STATE,            /* ����ƥ�䷶Χ״̬ */
    AT_PARA_LEFT_QUOT_STATE,      /* ����ƥ��������״̬ */
    AT_PARA_RIGHT_QUOT_STATE,     /* ����ƥ��������״̬ */
    AT_PARA_COLON_STATE,          /* ����ƥ�䶺��״̬ */
    AT_PARA_ZERO_STATE,           /* ����ƥ������0״̬ */
    AT_PARA_ZERO_SUB_STATE,       /* ����ƥ�䷶Χ����0״̬ */
    AT_PARA_HEX_STATE,            /* ����ƥ��ʮ������״̬, 0x��0X */
    AT_PARA_HEX_SUB_STATE,        /* ����ƥ�䷶Χʮ������״̬ */
    AT_PARA_HEX_NUM_STATE,        /* ����ƥʮ��������״̬ */
    AT_PARA_HEX_NUM_SUB_STATE,    /* ����ƥ�䷶Χʮ����������״̬ */
    AT_PARA_NO_QUOT_LETTER_STATE, /* ����ƥ����˫���Ű�����ĸ״̬ */

    AT_W_CMD_F_STATE,    /* AT��������&״̬ */
    AT_W_CMD_NAME_STATE, /* AT����������W״̬ */
    AT_W_CMD_PARA_STATE, /* AT����������W״̬ */

    AT_BUTT_STATE /* ��Ч״̬ */
} AT_StateType;

/* ģ���������士HEADER */

/* �ж���������ָ�� */
typedef TAF_UINT32 (*pATChkCharFuncType)(TAF_UINT8);

/* ��״̬�����ͣ�����ж����������ɹ�������뷵�ض�Ӧ����״̬ */
/*lint -e958 -e959 ;cause:64bit*/
typedef struct {
    pATChkCharFuncType funcName;  /*    �ж���������,����ɹ�������next_state */
    AT_StateType       nextState; /*    ��һ��״̬ */
} AT_SubState;
/*lint +e958 +e959 ;cause:64bit*/

/* ��ǰ״̬�����ͣ������״̬���ڵ�ǰ״̬��������Ӧ����״̬�� */
/*lint -e958 -e959 ;cause:64bit*/
typedef struct {
    AT_StateType        currState;   /* ��ǰ״̬ */
    const AT_SubState  *subStateTab; /* ��Ӧ����̬�� */
} AT_MainState;
/*lint +e958 +e959 ;cause:64bit*/

/*lint -esym(752,At_RangeCopy)*/
TAF_VOID At_RangeCopy(TAF_UINT8 *dst, TAF_UINT8 *begain, TAF_UINT8 *end);

/* ���ַ����е�ĳһ�ο�����ָ���� */
/*lint -esym(752,atRangeCopy)*/
VOS_VOID atRangeCopy(VOS_UINT8 *dst, VOS_UINT8 *begain, VOS_UINT8 *end);

/* ���ַ����е�ĳһ��ת���޷�������ֵ */
/*lint -esym(752,atRangeToU32)*/
VOS_UINT32 atRangeToU32(VOS_UINT8 *begain, VOS_UINT8 *end);

/*lint -esym(752,atFindNextSubState)*/
AT_StateType atFindNextSubState(const AT_SubState *subStateTab, VOS_UINT8 inputChar);

/*lint -esym(752,atFindNextMainState)*/
AT_StateType atFindNextMainState(const AT_MainState *mainStateTab, VOS_UINT8 inputChar, AT_StateType inputState);

/*lint -esym(752,atAuc2ul)*/
extern VOS_UINT32 atAuc2ul(VOS_UINT8 *nptr, VOS_UINT16 len, VOS_UINT32 *rtn);

/*lint -esym(752,At_ul2Auc)*/
VOS_VOID At_ul2Auc(VOS_UINT32 value, TAF_UINT16 len, VOS_UINT8 *rtn);

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* _AT_PARSE_CMD_H_ */