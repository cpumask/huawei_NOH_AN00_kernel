/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2019. All rights reserved.
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

#include "ppp_decode.h"
#include "ppp_comm_def.h"
#include "ppp_fcs.h"
#include "securec.h"
#include "ttf_util.h"


#define THIS_FILE_ID PS_FILE_ID_PPP_DECODE_C

/* Э����ѹ��1��FCS��С2 */
#define PPP_FRM_MIN_LEN 4

typedef VOS_VOID (*PPP_DecProcDataState)(PPP_DecCtrl *ctrl, VOS_UINT8 cur);

/* �Ѿ��ҵ�����7Eͷ��7Eβ��֡, �����ݲ��Ϸ���������֡��������ڴ�����ʹ�� */
static VOS_VOID PPP_DecReInitBuf(PPP_DecCtrl *ctrl)
{
    ctrl->used    = 0;
    ctrl->fcs     = PPP_INIT_FCS;
    ctrl->escMode = VOS_FALSE;
    ctrl->getHead = VOS_FALSE;
}

/* �Ѿ��ҵ�����7Eͷ��7Eβ��֡�������ݺϷ����ݽ����ϲ㴦�������ڴ��ָ����� */
static VOS_VOID PPP_DecClearBuf(PPP_DecCtrl *ctrl)
{
    ctrl->used    = 0;
    ctrl->fcs     = PPP_INIT_FCS;
    ctrl->escMode = VOS_FALSE;
    ctrl->getHead = VOS_FALSE;

    ctrl->buf.raw  = VOS_NULL_PTR;
    ctrl->buf.data = VOS_NULL_PTR;
    ctrl->buf.size = 0;
}

/* �Ѿ��ҵ�����7Eͷ��7Eβ��֡��������ͨ�������ݽ����ϲ�Я���Ĺ������� */
static VOS_VOID PPP_DecFillNewFrmInfo(const PPP_DecCtrl *ctrl, PPP_DecFrame *frm)
{
    frm->buf      = ctrl->buf;
    frm->used     = ctrl->used;
    frm->proto    = ctrl->proto;
    frm->pppId    = ctrl->transData;
}

/* �Ѿ��ҵ�����7Eͷ��7Eβ��֡���� */
static VOS_VOID PPP_DecProcNewFrm(PPP_DecCtrl *ctrl)
{
    PPP_DecFrame newFrm;

    /* FCSУ��ʧ�ܣ������뵽���ڴ�������´�ʹ�� */
    if (ctrl->fcs != PPP_LAST_FCS) {
        PPP_DecReInitBuf(ctrl);
        return;
    }

    /* ֡���ȷǷ��������뵽���ڴ�������´� */
    if (ctrl->getHead == VOS_FALSE) {
        PPP_DecReInitBuf(ctrl);
        return;
    }

    /* ȥ����FCS�ֶκ󣬵ݽ����߲㣬�ڴ��ɸ߲��ͷ� */
    ctrl->used -= PPP_FCS_LEN;
    if (ctrl->ops.deliver != VOS_NULL_PTR) {
        PPP_DecFillNewFrmInfo(ctrl, &newFrm);
        PPP_DecClearBuf(ctrl);
        ctrl->ops.deliver(&newFrm);
        return;
    }

    /* �����ϲ�Ӧ���ߵ��÷�֧ */
    PPP_DecReInitBuf(ctrl);
}

/* ���������ڴ棬������֡���� */
/*lint -e{730}*/
static VOS_VOID PPP_DecSaveData2Buf(PPP_DecCtrl *ctrl, VOS_UINT8 data)
{
    if (ctrl->used < ctrl->buf.size) {
        /* �����ַ�����ͬʱ����FCS */
        *(ctrl->buf.data + ctrl->used++) = data;
        ctrl->fcs                        = PPP_FcsCalOne(ctrl->fcs, data);
    } else {
        /* ���ݳ��ȳ�����ǰ��������������FCS�Ƿ�������������ʱ������֡ */
        ctrl->fcs = PPP_INIT_FCS;
    }
}

static VOS_UINT16 PPP_DecProtoType(PPP_DecCtrl *ctrl, VOS_UINT8* headerLen)
{
    VOS_UINT16 acf;
    VOS_UINT16 proto;
    VOS_UINT8  hdrLen   = 0; /* ��ַ��+������+Э���򳤶ȣ�ȡֵ��Χ1~4 */
    VOS_UINT8  hdrAcLen = 0; /* ��ַ��+�����򳤶ȣ�ȡֵ��Χ0~2 */

    /* ��ַ����ѹ�������¸��򳤶�Ϊ0�����򳤶�Ϊ2 */
    acf = (VOS_UINT16)((*(ctrl->buf.data) << 8) | *(ctrl->buf.data + 1));
    if (acf == PPP_ACF_S) {
        hdrLen += PPP_ACF_LEN;
        hdrAcLen += PPP_ACF_LEN;
    }

    /* Э������ѹ�������¸��򳤶�Ϊ1�����򳤶�Ϊ2 */
    if ((*(ctrl->buf.data + hdrLen) & PPP_PF_MASK) != 0) {
        hdrLen += PPP_CPF_LEN;
        proto = *(ctrl->buf.data + hdrAcLen);
    } else {
        hdrLen += PPP_PF_LEN;
        proto = (VOS_UINT16)((*(ctrl->buf.data + hdrAcLen) << 8) | *(ctrl->buf.data + hdrAcLen + 1));
    }

    *headerLen = (ctrl->ops.ipMode == VOS_TRUE) ? hdrLen : hdrAcLen;
    return proto;
}

static VOS_VOID PPP_DecDataHeader(PPP_DecCtrl *ctrl)
{
    VOS_UINT8 headerLen = 0;
    
    if ((ctrl->getHead == VOS_FALSE) && (ctrl->used == PPP_FRM_MIN_LEN)) {
        ctrl->getHead = VOS_TRUE;
        ctrl->proto = PPP_DecProtoType(ctrl, &headerLen);
        if (headerLen < ctrl->used) {
            TTF_SF_LOG(memmove_s(ctrl->buf.data, ctrl->buf.size, ctrl->buf.data + headerLen, (ctrl->used - headerLen)));
        }
        ctrl->used -= headerLen;
    }
}

/* �Ѿ�������֡״̬���� */
static VOS_VOID PPP_DecProcInFrm(PPP_DecCtrl *ctrl, VOS_UINT8 cur)
{
    VOS_UINT8 data;

    if (cur == PPP_SYN_C) {
        PPP_DecProcNewFrm(ctrl);
        ctrl->state   = PPP_DEC_SYN;
    } else {
        /* ת���ַ�ǰ׺������ת��״̬,��һ���ַ��ٴ��� */
        if ((cur == PPP_ESC_C) && (ctrl->escMode != VOS_TRUE)) {
            ctrl->escMode = VOS_TRUE;
            return;
        }

        /* ת��״̬���ַ����� */
        if (ctrl->escMode == VOS_TRUE) {
            data          = (cur ^ PPP_XOR_C);
            ctrl->escMode = VOS_FALSE;
        } else {
            data = cur;
        }

        /* �����ַ�����ͬʱ����FCS */
        PPP_DecSaveData2Buf(ctrl, data);

        PPP_DecDataHeader(ctrl);
    }
}

/* ����״̬��ֻ��0x7E�ᱻ���� */
static VOS_VOID PPP_DecProcInNull(PPP_DecCtrl *ctrl, VOS_UINT8 cur)
{
    if (cur == PPP_SYN_C) {
        ctrl->state = PPP_DEC_SYN;
    }
}

/* �յ�0x7E״̬��ֻ�з�0x7E�ᱻ���� */
static VOS_UINT32 PPP_DecProcInSyn(PPP_DecCtrl *ctrl, VOS_UINT8 cur)
{
    if (cur == PPP_SYN_C) {
        return VOS_OK;
    }

    /* ֻ�ڵ�ǰ֡�ĵ�һ�����ݱ���ʱ���������ڴ棬�����ܱ�֤����ɹ� */
    if (ctrl->buf.data == VOS_NULL_PTR) {
        if (ctrl->ops.alloc != VOS_NULL_PTR) {
            ctrl->ops.alloc(ctrl->ops.memMaxLen, &ctrl->buf);
        }
    }
    if (ctrl->buf.data == VOS_NULL_PTR) {
        return VOS_ERR;
    }
    ctrl->state = PPP_DEC_FRM;
    PPP_DecProcInFrm(ctrl, cur);

    return VOS_OK;
}

/* PPP���봦�����õ㱣֤��������Ч�� */
/*lint -e{730}*/
VOS_UINT32 PPP_DecProc(PPP_DecCtrl *ctrl, const VOS_UINT8 *data, VOS_UINT16 dataLen, VOS_UINT32 transData)
{
    VOS_UINT32 loop;

    ctrl->transData = transData;
    for (loop = 0; loop < dataLen; ++loop) {
        if (ctrl->state == PPP_DEC_FRM) {
            PPP_DecProcInFrm(ctrl, data[loop]);
        } else if (ctrl->state == PPP_DEC_SYN) {
            if (PPP_DecProcInSyn(ctrl, data[loop]) != VOS_OK) {
                return VOS_ERR;
            }
        } else {
            PPP_DecProcInNull(ctrl, data[loop]);
        }
    }

    return VOS_OK;
}

/* PPP����ʵ���ʼ�������õ㱣֤��������Ч�� */
static inline VOS_VOID PPP_DecCtrlInit(PPP_DecCtrl *ctrl, const PPP_DecCfg *ops, PPP_DecStateUint32 state)
{
    ctrl->state   = state;
    ctrl->escMode = VOS_FALSE;
    ctrl->ops     = *ops;
    ctrl->used    = 0;
    ctrl->fcs     = PPP_INIT_FCS;
    ctrl->getHead = VOS_FALSE;
    ctrl->proto   = 0;
    PPP_COMM_CLEAR_UPLAYER_MEM(&ctrl->buf);
}

/* PPP����ʵ���ʼ�������õ㱣֤��������Ч�� */
VOS_VOID PPP_DecCtrlCreate(PPP_DecCtrl *ctrl, const PPP_DecCfg *ops)
{
    PPP_DecCtrlInit(ctrl, ops, PPP_DEC_NULL);
}

/* PPP����ʵ���ͷţ����õ㱣֤��������Ч�� */
VOS_VOID PPP_DecCtrlRelease(PPP_DecCtrl *ctrl)
{
    PPP_DecCfg nullOp = { VOS_NULL_PTR, VOS_NULL_PTR, VOS_NULL_PTR, 0 };

    /* ʵ���ͷ�ʱ�������ڴ��ͷ� */
    if ((ctrl->ops.free != VOS_NULL_PTR) && (ctrl->buf.raw != VOS_NULL_PTR)) {
        ctrl->ops.free(&(ctrl->buf));
    }

    PPP_DecCtrlInit(ctrl, &nullOp, PPP_DEC_BUTT);
}

