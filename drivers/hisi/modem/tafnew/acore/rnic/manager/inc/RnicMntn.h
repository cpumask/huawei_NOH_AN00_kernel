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

#ifndef __RNIC_MNTN_H__
#define __RNIC_MNTN_H__

#include "vos.h"
#include "ps_common_def.h"
#include "taf_diag_comm.h"
#include "RnicIntraMsg.h"
#include "rnic_dev_i.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)

#define RNIC_MNTN_COMM_MOUDLE_ID (MDRV_DIAG_GEN_MODULE(MODEM_ID_0, DIAG_MODE_COMM))

/* ���貦��ͳ����Ϣ */
#define RNIC_DBG_SEND_APP_DIALUP_SUCC_NUM(n) (g_rnicStats.sendAppDialUpSucc += (n))
#define RNIC_DBG_SEND_APP_DIALUP_FAIL_NUM(n) (g_rnicStats.sendAppDialUpFail += (n))
#define RNIC_DBG_SEND_APP_DIALDOWN_SUCC_NUM(n) (g_rnicStats.sendAppDialDownSucc += (n))
#define RNIC_DBG_SEND_APP_DIALDOWN_FAIL_NUM(n) (g_rnicStats.sendAppDialDownFail += (n))

/*
 * �ṹ˵��: RNIC��ά�ɲ⹫��ͷ�ṹ
 */
typedef struct {
    VOS_UINT8 ver;
    VOS_UINT8 reserved;

    VOS_UINT16 reserved0;
    VOS_UINT16 reserved1;
    VOS_UINT16 reserved2;

} RNIC_MntnCommHeader;

/*
 * �ṹ˵��: ���б���ͳ����Ϣ�ϱ��ṹ
 */
typedef struct {
    RNIC_MntnCommHeader commHeader;

    VOS_UINT8                rmNetId;
    VOS_UINT8                reserved0;
    VOS_UINT8                reserved1;
    VOS_UINT8                reserved2;
    struct rnic_data_stats_s dataStats;

} RNIC_MntnPktStats;

/*
 * �ṹ˵��: NAPIͳ����Ϣ�ϱ��ṹ
 */
typedef struct {
    RNIC_MntnCommHeader commHeader;

    VOS_UINT8                rmNetId;
    VOS_UINT8                reserved0;
    VOS_UINT8                reserved1;
    VOS_UINT8                reserved2;
    struct rnic_napi_stats_s napiStats;

} RNIC_MntnNaipInfo;

/*
 * �ṹ˵��: ���貦�ſ�ά�ɲ���Ϣ�ṹ
 */
typedef struct {
    VOS_MSG_HEADER               /* ��Ϣͷ */ /* _H2ASN_Skip */
    RNIC_IntraMsgIdUint32 msgId; /* ��ϢID */ /* _H2ASN_Skip */
} RNIC_MntnDialConnEvt;

/*
 * �ṹ˵��: ���貦�ſ�ά�ɲ���Ϣ�ṹ
 */
typedef struct {
    VOS_MSG_HEADER               /* ��Ϣͷ */ /* _H2ASN_Skip */
    RNIC_IntraMsgIdUint32 msgId; /* ��ϢID */ /* _H2ASN_Skip */
    VOS_UINT32            pktNum;
    VOS_UINT32            usrExistFlg;
} RNIC_MntnDialDisconnEvt;

/*
 * �ṹ˵��: RNICͳ����
 */
typedef struct {
    VOS_UINT32 sendAppDialUpSucc;   /* RNIC�ɹ��ϱ�APP���貦�� */
    VOS_UINT32 sendAppDialUpFail;   /* RNIC�ϱ�APP���貦��ʧ�� */
    VOS_UINT32 sendAppDialDownSucc; /* RNIC�ɹ��ϱ�APP�Ͽ����� */
    VOS_UINT32 sendAppDialDownFail; /* RNIC�ϱ�APP�Ͽ�����ʧ�� */

} RNIC_StatsInfo;

extern RNIC_StatsInfo g_rnicStats;

VOS_VOID RNIC_MntnTransReport(VOS_UINT32 msgId, VOS_VOID *data, VOS_UINT32 len);
VOS_VOID RNIC_MntnReportPktStats(VOS_UINT8 rmNetId);
VOS_VOID RNIC_MntnReportNapiInfo(VOS_UINT8 rmNetId);
VOS_VOID RNIC_MntnTraceDialConnEvt(VOS_VOID);
VOS_VOID RNIC_MntnTraceDialDisconnEvt(VOS_UINT32 pktNum, VOS_UINT32 usrExistFlg);

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of RnicMntn.h */
