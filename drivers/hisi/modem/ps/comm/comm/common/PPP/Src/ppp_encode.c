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

#include "ppp_encode.h"
#include "ppp_fcs.h"


#define THIS_FILE_ID PS_FILE_ID_PPP_ENCODE_C

typedef struct {
    PPP_EncOutput    *output;
    VOS_UINT32        accMap;
    VOS_UINT16        fcs;
    VOS_UINT16        maxSegCnt;
    PPP_EncOutputSeg *writeSeg;
} PPP_EncBufMgm;

/* д��һ����ͨ�ַ���������ת�崦���FCS���� */
/*lint -e{730}*/
static inline VOS_VOID PPP_EncWriteNonEscC2Buf(VOS_UINT8 data, PPP_EncBufMgm *buf)
{
    if (buf->writeSeg->used < buf->writeSeg->seg.size) {
        *(buf->writeSeg->seg.data + buf->writeSeg->used++) = data;
    } else {
        buf->writeSeg       = &(buf->output->outSegs[buf->output->segCnt % buf->maxSegCnt]);
        buf->writeSeg->used = 0;
        buf->output->segCnt++;
        *(buf->writeSeg->seg.data + buf->writeSeg->used++) = data;
    }
}

/* д��һ���ַ�������Ҫת����ַ�����ת�崦�� */
/*lint -e{730}*/
static inline VOS_VOID PPP_EncWriteC2Buf(VOS_UINT8 ch, PPP_EncBufMgm *buf)
{
    /* ת���ַ����� */
    if (((ch < PPP_XOR_C) && ((buf->accMap & (1u << ch)) != 0)) || (ch == PPP_SYN_C) || (ch == PPP_ESC_C)) {
        PPP_EncWriteNonEscC2Buf(PPP_ESC_C, buf);
        PPP_EncWriteNonEscC2Buf((ch ^ PPP_XOR_C), buf);
    } else {
        PPP_EncWriteNonEscC2Buf(ch, buf);
    }
}

/* ��һд�����ַ���ÿ���ַ�����FCS���� */
static inline VOS_VOID PPP_EncWriteData2Buf(const VOS_UINT8 *data, VOS_UINT32 dataLen, PPP_EncBufMgm *buf)
{
    VOS_UINT32 loop;
    VOS_UINT8  cur;

    for (loop = 0; loop < dataLen; ++loop) {
        cur = *(data + loop);
        PPP_EncWriteC2Buf(cur, buf);
        buf->fcs = PPP_FcsCalOne(buf->fcs, cur);
    }
}

/* д��FCS�ֶ� */
static inline VOS_VOID PPP_EncWriteFcs2Buf(PPP_EncBufMgm *buf)
{
    VOS_UINT16 fcs;

    fcs = ~(buf->fcs);

    /* ����Э��:fcs����С����д */
    PPP_EncWriteC2Buf((VOS_UINT8)fcs, buf);
    PPP_EncWriteC2Buf((VOS_UINT8)(fcs >> 8), buf);
}

/* д���ַ��+������+Э���� */
static inline VOS_VOID PPP_EncWriteHdr2Buf(VOS_BOOL acf, VOS_BOOL pf, VOS_UINT16 proto, PPP_EncBufMgm *buf)
{
    VOS_UINT32 tmpDataLen = 0;
    VOS_UINT8  tmpData[4]; /* ��ַ��+������+Э�����4�ֽ� */

    /* ׼����ַ��Ϳ�����ͷ����LCPЭ�鲻��ѹ�� */
    if ((acf == VOS_FALSE) || (proto == PPP_LCP_P)) {
        tmpData[0] = PPP_ADDR_C;
        tmpData[1] = PPP_CTRL_C;
        tmpDataLen += PPP_ACF_LEN;
    }

    /* ׼��Э����ͷ�� */
    if ((pf == VOS_TRUE) && ((proto >> 8) == 0)) {
        tmpData[tmpDataLen] = (VOS_UINT8)proto;
        tmpDataLen += PPP_CPF_LEN;
    } else {
        tmpData[tmpDataLen]     = (VOS_UINT8)(proto >> 8);
        tmpData[tmpDataLen + 1] = (VOS_UINT8)proto;
        tmpDataLen += PPP_PF_LEN;
    }

    /* ���Э��ͷ������ַ��+������+Э���� */
    PPP_EncWriteData2Buf(tmpData, tmpDataLen, buf);
}

/* ����ʱ���ڴ��ͷŴ��� */
static inline VOS_VOID PPP_EncErrProc(PPP_EncCfg *cfg, PPP_EncBufMgm *buf)
{
    VOS_UINT32 loop;

    /* �˴������ڴ�ʧ�ܲ��ͷţ����ͳһ������ؽӿ��ͷ� */
    for (loop = 0; loop < buf->maxSegCnt; ++loop) {
        if ((cfg->free != VOS_NULL_PTR) && (buf->output->outSegs[loop].seg.raw != VOS_NULL_PTR)) {
            cfg->free(&(buf->output->outSegs[loop].seg));
        }
    }
}

/* �������ʱ��δʹ���ڴ��ͷŴ��� */
static inline VOS_VOID PPP_EncClearUnuseMem(PPP_EncCfg *cfg, PPP_EncBufMgm *buf)
{
    VOS_UINT32 loop;

    /* �˴������ڴ�ʧ�ܲ��ͷţ����ͳһ������ؽӿ��ͷ� */
    for (loop = 0; loop < buf->maxSegCnt; ++loop) {
        if ((cfg->free != VOS_NULL_PTR) && (buf->output->outSegs[loop].used == 0)) {
            cfg->free(&(buf->output->outSegs[loop].seg));
        }
    }
}

/* �����ڴ����봦��������������ڴ� */
static inline VOS_UINT32 PPP_EncPrepareMem(PPP_EncCfg *cfg, const PPP_EncInput *input, PPP_EncBufMgm *buf)
{
    const VOS_UINT32 appendLen = 13; /* ����ͷβ0x7E+��ַ��+������+Э����+FCS */
    VOS_UINT32       encMaxLen;
    VOS_UINT32       segLen[PPP_ENC_MAX_SEGS];
    VOS_UINT32       loop;
    VOS_UINT32       loopMax;
    VOS_UINT32       ret;

    /* ��װ����ܵ�������ݳ���(2*ԭʼ���ݳ���+13B) */
    encMaxLen = (2 * input->dataLen + appendLen);
    if (encMaxLen > (cfg->memMaxLen * PPP_ENC_MAX_SEGS)) {
        return VOS_ERR;
    }

    if (encMaxLen > cfg->memMaxLen) {
        loopMax   = PPP_ENC_MAX_SEGS;
        segLen[0] = cfg->memMaxLen;
        segLen[1] = (encMaxLen - cfg->memMaxLen);
    } else {
        loopMax   = (PPP_ENC_MAX_SEGS - 1);
        segLen[0] = encMaxLen;
    }

    for (loop = 0; loop < loopMax; ++loop) {
        ret = VOS_ERR;
        if (cfg->alloc != VOS_NULL_PTR) {
            ret = cfg->alloc(segLen[loop], &(buf->output->outSegs[loop].seg));
        }

        /* �˴������ڴ�ʧ�ܲ��ͷţ����ͳһ������ؽӿ��ͷ� */
        if (ret != VOS_OK) {
            return VOS_ERR;
        }
        buf->maxSegCnt++;
        /* ���ڴ�����������У�� */
        if (buf->output->outSegs[loop].seg.size < segLen[loop]) {
            return VOS_ERR;
        }
    }

    buf->writeSeg       = &(buf->output->outSegs[0]);
    buf->output->segCnt = 1;
    return VOS_OK;
}

/* ���봦��������VOS_OKʱ��output�еĽ��������Ч�� */
VOS_UINT32 PPP_EncProc(PPP_EncCfg *cfg, const PPP_EncInput *input, PPP_EncOutput *output)
{
    PPP_EncBufMgm buf;
    VOS_UINT32    ret;
    VOS_UINT32    loop;

    /* ��ʼ������������ */
    output->segCnt = 0;
    for (loop = 0; loop < PPP_ENC_MAX_SEGS; ++loop) {
        output->outSegs[loop].used = 0;
        PPP_COMM_CLEAR_UPLAYER_MEM(&(output->outSegs[loop].seg));
    }
    buf.output    = output;
    buf.accMap    = ((input->proto == PPP_LCP_P) ? PPP_DEFAULT_ACCMAP : cfg->accMap);
    buf.fcs       = PPP_INIT_FCS;
    buf.maxSegCnt = 0;

    /* ׼���ڴ�,ע��˴�׼�����ڴ����ܲ�û��ʹ���� */
    ret = PPP_EncPrepareMem(cfg, input, &buf);
    if (ret != VOS_OK) {
        PPP_EncErrProc(cfg, &buf);
        return VOS_ERR;
    }

    /* ��д��ʼ0x7E */
    PPP_EncWriteNonEscC2Buf(PPP_SYN_C, &buf);

    /* ���Э��ͷ������ַ��+������+Э���� */
    PPP_EncWriteHdr2Buf(cfg->acf, cfg->pcf, input->proto, &buf);

    /* ��䱨������ */
    PPP_EncWriteData2Buf(input->data, input->dataLen, &buf);

    /* ��дFCS */
    PPP_EncWriteFcs2Buf(&buf);

    /* ��д��β0x7E */
    PPP_EncWriteNonEscC2Buf(PPP_SYN_C, &buf);

    /* �п���Ԥ���������飬�˴�ֻ����һ�飬�ͷ�δʹ�õ��ڴ� */
    PPP_EncClearUnuseMem(cfg, &buf);
    return VOS_OK;
}

