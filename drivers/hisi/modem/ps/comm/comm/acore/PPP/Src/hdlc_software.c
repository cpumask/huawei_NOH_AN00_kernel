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

#include "hdlc_software.h"
#include "ttf_comm.h"
#include "gucttf_tag.h"
#include "ppp_convert.h"
#include "hdlc_interface.h"
#include "ppp_input.h"
#include "ppp_public.h"


#define THIS_FILE_ID PS_FILE_ID_HDLC_SOFTWARE_C
#define THIS_MODU mod_ppp

typedef struct {
    VOS_UINT32 uplinkIpDataProcCnt;  /* ����IP���ݰ�������� */
    VOS_UINT32 uplinkPppDataProcCnt; /* ���м�PPP֡������� */

    VOS_UINT32 downlinkIpDataProcCnt;  /* ����IP���ݰ�������� */
    VOS_UINT32 downlinkPppDataProcCnt; /* ���м�PPP֡������� */

    VOS_UINT32 maxCntOnce; /* PPPһ����ദ��Ľ����� */
} PPP_HDLC_SoftDataProcStat;

PPP_HDLC_SoftDataProcStat g_pppHdlcSoftStat = {0};
VOS_BOOL                  g_pppIsIpMode     = VOS_TRUE;

__STATIC_ PPP_ConvCtrl g_pppaConvEntity = {0};


VOS_UINT32 PPP_HDLC_Encap(PPP_Id pppId, PPP_Zc *data, VOS_BOOL isPppMode)
{
    VOS_UINT16       proto;
    const VOS_UINT8 *head    = PPP_ZC_GET_DATA_PTR(data);
    VOS_UINT8        headLen = 1; /* Э�鳤����СΪ1 */
    PPP_EncInput     input;
    PPP_EncOutput    output = {0};
    VOS_UINT32       loop;
    PPP_Zc          *sendData = VOS_NULL_PTR;

    if (isPppMode == VOS_TRUE) {
        if (PPP_ZC_GET_DATA_LEN(data) < sizeof(proto)) {
            PPP_MNTN_LOG(LOG_LEVEL_WARNING, "wrong data len!");
            return VOS_ERR;
        }
        /* Э��涨Э���bit0����Ϊ1, bit8����Ϊ0, �����һ���ֽڵ�bit0����1˵��û�н���Э����ѹ�� */
        proto = head[0];
        if ((proto & 0x01) != 0x01) {
            proto = (VOS_UINT16)(proto << 8 | head[headLen]);
            headLen++;
        }
        PPP_ZC_REMOVE_HDR(data, headLen);
    } else {
        proto = PPPA_IP;
    }

    input.data    = PPP_ZC_GET_DATA_PTR(data);
    input.dataLen = PPP_ZC_GET_DATA_LEN(data);
    input.proto   = proto;
    if (PPP_ConvEncode(&g_pppaConvEntity, &input, &output) != VOS_OK) {
        PPP_MNTN_LOG(LOG_LEVEL_WARNING, "Encap PPP Fail!");
        return VOS_ERR;
    }

    for (loop = 0; loop < output.segCnt; loop++) {
        sendData = (PPP_Zc*)output.outSegs[loop].seg.raw;
        PPP_ZC_SET_DATA_LEN(sendData, output.outSegs[loop].used);
        PPPA_SendDlDataToAT(pppId, sendData);
    }

    return VOS_OK;
}

VOS_BOOL PPP_HDLC_SoftProcData(PPP_Id pppId, PPP_ZcQueue *dataQ)
{
    PPP_Zc           *mem     = VOS_NULL_PTR;
    VOS_UINT32        dealCnt = 0;
    PPP_DataTypeUint8 dataType;
    VOS_BOOL          delayProc = VOS_TRUE;
    VOS_UINT32        procRslt  = VOS_OK;

    /* һ����ദ�� PPP_ONCE_DEAL_MAX_CNT ���ڵ� */
    for (dealCnt = 0; dealCnt < PPP_ONCE_DEAL_MAX_CNT; dealCnt++) {
        mem = (PPP_Zc*)PPP_ZC_DEQUEUE_HEAD(dataQ);

        /* ����Ϊ�ղ���Ҫ�������� */
        if (mem == VOS_NULL_PTR) {
            delayProc = VOS_FALSE;
            break;
        }

        /* ����ý��(�����ͷŶ����Ѿ��ڸ��������ڲ���ɣ��������ͷŽ��) */
        dataType = (PPP_ZC_GET_DATA_APP(mem) & 0x00FF);

        switch (dataType) {
            case PPP_PULL_PACKET_TYPE:
                g_pppHdlcSoftStat.uplinkIpDataProcCnt++;
                procRslt = PPP_ConvDecode(&g_pppaConvEntity, PPP_ZC_GET_DATA_PTR(mem), PPP_ZC_GET_DATA_LEN(mem), pppId);
                break;
            case PPP_PUSH_PACKET_TYPE:
                g_pppHdlcSoftStat.downlinkIpDataProcCnt++;
                procRslt = PPP_HDLC_Encap(pppId, mem, VOS_FALSE);
                break;

            case PPP_PULL_RAW_DATA_TYPE:
                g_pppHdlcSoftStat.uplinkPppDataProcCnt++;
                procRslt = PPP_ConvDecode(&g_pppaConvEntity, PPP_ZC_GET_DATA_PTR(mem), PPP_ZC_GET_DATA_LEN(mem), pppId);
                break;

            case PPP_PUSH_RAW_DATA_TYPE:
                g_pppHdlcSoftStat.downlinkPppDataProcCnt++;
                procRslt = PPP_HDLC_Encap(pppId, mem, VOS_TRUE);
                break;

            default:
                PPP_MNTN_LOG1(LOG_LEVEL_WARNING, "PPP_ProcDataNotify, WARNING, ucDataType <1> is Abnormal!", dataType);
                break;
        }
        PPP_MemFree(mem);
        if (procRslt != VOS_OK) {
            break;
        }
    }

    if (dealCnt > g_pppHdlcSoftStat.maxCntOnce) {
        g_pppHdlcSoftStat.maxCntOnce = dealCnt;
    }
    /* һ�δ���ڵ����ﵽ PPP_ONCE_DEAL_MAX_CNT ��Ҫ�ӳٴ������ */
    return delayProc;
}

VOS_UINT32 PPPA_DefMemAllocCB(VOS_UINT32 len, PPP_UplayerMem *buf)
{
    PPP_Zc *mem = PPP_MemAlloc((VOS_UINT16)len, PPP_ZC_UL_RESERVE_LEN);
    if (mem == VOS_NULL_PTR) {
        return VOS_ERR;
    }
    buf->data = PPP_ZC_GET_DATA_PTR(mem);
    buf->raw  = mem;
    buf->size = len;
    return VOS_OK;
}

VOS_UINT32 PPPA_FrmMemAllocCB(VOS_UINT32 len, PPP_UplayerMem *buf)
{
    PPP_Zc *mem = PPP_MemAlloc((VOS_UINT16)len, PPP_ZC_DL_RESERVE_LEN);
    if (mem == VOS_NULL_PTR) {
        return VOS_ERR;
    }
    buf->data = PPP_ZC_GET_DATA_PTR(mem);
    buf->raw  = mem;
    buf->size = len;
    return VOS_OK;
}

VOS_VOID PPPA_DefMemFreeCB(PPP_UplayerMem *buf)
{
    PPP_MemFree((PPP_Zc*)buf->raw);
}

VOS_VOID PPPA_DefRsltProcCB(PPP_DecFrame *frm)
{
    const VOS_UINT16 pppId = (VOS_UINT16)frm->pppId;
    PPP_Zc *mem = (PPP_Zc*)frm->buf.raw;

    PPP_ZC_SET_DATA_LEN(mem, frm->used);

    if (g_pppIsIpMode == VOS_TRUE) {
        PPP_HDLC_ProcIpModeUlData(pppId, mem, frm->proto);
    } else {
        /* PPPģʽ�·��͵���PPP���ģ����Э��������0 */
        PPP_SendUlDataToUu(pppId, mem, 0);
    }
}

VOS_VOID PPP_HDLC_SetUp(PPP_Id pppId, VOS_BOOL isIpMode)
{
    PPP_ConvOps convOps;

    convOps.decCfg.alloc     = PPPA_DefMemAllocCB;
    convOps.decCfg.free      = PPPA_DefMemFreeCB;
    convOps.decCfg.deliver   = PPPA_DefRsltProcCB;
    convOps.decCfg.memMaxLen = IMM_MAX_ETH_FRAME_LEN - PPP_ZC_UL_RESERVE_LEN;
    convOps.decCfg.ipMode    = isIpMode;
    convOps.encCfg.alloc     = PPPA_FrmMemAllocCB;
    convOps.encCfg.free      = PPPA_DefMemFreeCB;
    convOps.encCfg.accMap    = PPPA_DEFAULT_ACCM;
    convOps.encCfg.acf       = VOS_FALSE;
    convOps.encCfg.pcf       = VOS_FALSE;
    convOps.encCfg.memMaxLen = IMM_MAX_ETH_FRAME_LEN;
    PPP_ConvCreate(&g_pppaConvEntity, &convOps);

    g_pppIsIpMode = isIpMode;
}

VOS_VOID PPP_HDLC_UpdateCfg(PPP_Id pppId, VOS_BOOL acf, VOS_BOOL pcf, VOS_UINT32 accm)
{
    PPP_ConvOps ops;

    PPP_ConvGetCfg(&g_pppaConvEntity, &ops);
    ops.encCfg.acf    = acf;
    ops.encCfg.pcf    = pcf;
    ops.encCfg.accMap = accm;

    PPP_ConvReCfg(&g_pppaConvEntity, &ops);
}

VOS_VOID PPP_HDLC_Release(PPP_Id pppId)
{
    (VOS_VOID)PPP_ConvRelease(&g_pppaConvEntity);
}

