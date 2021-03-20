/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
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


#ifndef __MSP_DIAG_H__
#define __MSP_DIAG_H__

#include <msp_diag_comm.h>

#ifdef __cplusplus
extern "C"
{
#endif


/* diag debug switch */
typedef struct
{
    VOS_UINT16 usMntnSwitch;    /* 1 �� 0�ر� */
    VOS_UINT16 usMntnTime;      /*�ϱ�����*/
}DIAG_CMD_LOG_DIAG_MNTN_REQ_STRU;

typedef struct
{
    DIAG_CMD_LOG_DIAG_MNTN_REQ_STRU stMntnInfo;
    VOS_UINT16 usHl1c;
    VOS_UINT16 usTlPhy;
    VOS_UINT16 usRfDsp;
    VOS_UINT16 usCPhy;
    VOS_UINT16 usGuPhyM0;
    VOS_UINT16 usGuPhyM1;
    VOS_UINT16 usGuPhyM2;
    VOS_UINT16 usResved;
}DIAG_MNTN_REQ_STRU;

/*****************************************************************************
 Function Name   : mdrv_diag_log_port_switch
 Description     : �˿��л�����(�ṩ��taf����at^logport�е���)
 Input           : VOS_UINT32 ulPhyPort: ���л�������˿�����(USB or VCOM)
                   VOS_BOOL ulEffect: �Ƿ�������Ч
 Return          : VOS_OK:success
                   VOS_ERR: error
                   ERR_MSP_AT_CHANNEL_BUSY:diag����״̬�²������USB�л���VCOM

*****************************************************************************/
unsigned int mdrv_diag_log_port_switch(unsigned int phy_port, unsigned int effect);

#ifdef __cplusplus
}
#endif
#endif
