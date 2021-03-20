/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2006-2019. All rights reserved.
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

#include "ttf_util.h"
#include "PsTypeDef.h"
#include "ttf_comm.h"
#include "securec.h"
#include "mdrv.h"


#define THIS_FILE_ID PS_FILE_ID_TTF_UTIL_C

#define TTF_REG_MAX_BIT 31

#pragma pack(push, 4)

/*  TTF A�� PSACORE_MEM_SET ��ȫ������λ��Ϣ����ṹ */
typedef struct {
    VOS_UINT8 *toSet;
    VOS_UINT32 destSize;
    VOS_INT8   setChar;
    VOS_UINT8  rsv[3];
    VOS_UINT32 count;
} TtfAccoreMemSetSoftRebootStru;

/* TTF A�� PSACORE_MEM_CPY ��ȫ������λ��Ϣ����ṹ */
typedef struct {
    VOS_UINT8 *dest;
    VOS_UINT32 destSize;
    VOS_UINT8 *src;
    VOS_UINT32 count;
} TtfAccoreMemCpySoftRebootStru;

/* TTF A�� PSACORE_MEM_MOVE ��ȫ������λ��Ϣ����ṹ */
typedef struct {
    VOS_UINT8 *dest;
    VOS_UINT32 destSize;
    VOS_UINT8 *src;
    VOS_UINT32 count;
} TtfAccoreMemMoveSoftRebootStru;

#pragma pack(pop)

/*
 * ��ʼ�����нڵ㣬��ʹ�øýڵ�֮ǰ��������øú�����ʼ��
 * item �������нڵ������ link -- ���нڵ�
 */
VOS_VOID TTF_QLink(VOS_UINT32 pid, VOS_VOID *item, TtfQLink *link)
{
    if (link == VOS_NULL_PTR) {
        return;
    }

    link->next = VOS_NULL_PTR;
}

VOS_VOID TTF_QInit(VOS_UINT32 pid, TtfQ *queue)
{
    if (queue == VOS_NULL_PTR) {
        return;
    }

    queue->hdr.head = (TtfQLink*)&(queue->hdr);
    queue->hdr.tail = (TtfQLink*)&(queue->hdr);
    queue->cnt      = 0;
}

VOS_UINT32 TTF_QIn(VOS_UINT32 pid, TtfQ *queue, TtfQLink *link)
{
    if ((queue == VOS_NULL_PTR) || (link == VOS_NULL_PTR)) {
        return VOS_ERR;
    }

    link->next            = (TtfQLink*)&(queue->hdr);
    queue->hdr.tail->next = link;
    queue->hdr.tail       = link;
    queue->cnt++;

    return VOS_OK;
}

/*
 * ���ݳ����У����س��ӵ����ݵ�ָ��, ע��˺�����TTF_QueuePeek()������
 */
VOS_VOID* TTF_QOut(VOS_UINT32 pid, TtfQ *queue)
{
    TtfQLink *link = VOS_NULL_PTR;
    TtfQLink *ret  = VOS_NULL_PTR;

    if (queue == VOS_NULL_PTR) {
        return VOS_NULL_PTR;
    }

    link = queue->hdr.head;

    if (queue->cnt > 0) {
        queue->hdr.head = link->next;

        if (link->next == (TtfQLink*)queue) {
            queue->hdr.tail = (TtfQLink*)&(queue->hdr);
        }

        queue->cnt--;

        link->next = VOS_NULL_PTR;
        ret        = link;
    }

    return (VOS_VOID*)ret;
}

/*
 * ����ָ�����ȵ��ڴ浥Ԫ�����ݸ���TTF_BLK_ST�ṹ֮��
 * Len: Ҫ������ڴ浥Ԫ�Ĵ�С���ֽ�Ϊ��λ
 */
TtfMBuf* TTF_MbufNew(VOS_UINT32 pid, VOS_UINT16 len)
{
    TtfMBuf   *mbuf    = VOS_NULL_PTR;
    VOS_UINT32 mbufLen = sizeof(TtfMBuf) + len;

    if (len == 0) {
        return VOS_NULL_PTR;
    }

    mbuf = (TtfMBuf*)VOS_MemAlloc(pid, DYNAMIC_MEM_PT, mbufLen);
    if (mbuf == VOS_NULL_PTR) {
        return VOS_NULL_PTR;
    }

    (VOS_VOID)memset_s(mbuf, mbufLen, 0, mbufLen);
    TTF_QLink(pid, mbuf, &(mbuf->link));
    mbuf->next = VOS_NULL_PTR;
    mbuf->len  = len;
    mbuf->used = 0;
    mbuf->data = (VOS_UINT8*)(mbuf + 1);

    return mbuf;
}

void TTF_MbufFree(VOS_UINT32 pid, TtfMBuf *mbuf)
{
    TtfMBuf *next = VOS_NULL_PTR;

    while (mbuf != VOS_NULL_PTR) {
        next = mbuf->next;
        (VOS_VOID)VOS_MemFree(pid, mbuf);
        mbuf = next;
    }
}

VOS_UINT16 TTF_MbufGetLen(VOS_UINT32 pid, TtfMBuf *mbuf)
{
    VOS_UINT16 len = 0;
    TtfMBuf   *tmp = mbuf;

    while (tmp != VOS_NULL_PTR) {
        len += tmp->used;
        tmp = tmp->next;
    }

    return len;
}

/*
 * set value from startBit to endBit
 * startBit: startBit in 32bit,lower edge
 * endBit: endBit in 32bit,upper edge
 */
VOS_UINT32 TTF_SetByBit(VOS_UINT32 pid, VOS_UINT32 orgValue, VOS_UINT8 startBit, VOS_UINT8 endBit, VOS_UINT32 setValue)
{
    VOS_UINT32 orgMask;
    VOS_UINT32 dataMask;

    if ((startBit > endBit) || (startBit > TTF_REG_MAX_BIT) || (endBit > TTF_REG_MAX_BIT)) {
        TTF_LOG4(
            pid, DIAG_MODE_COMM, PS_PRINT_WARNING, "TTF_SetByBit para err\r\n", orgValue, startBit, endBit, setValue);
        return orgValue;
    }

    /* to get the mask form startBit to endbit */
    dataMask = 0xFFFFFFFF;
    dataMask = dataMask >> startBit;
    dataMask = dataMask << (TTF_REG_MAX_BIT - (endBit - startBit));
    dataMask = dataMask >> (TTF_REG_MAX_BIT - endBit);
    orgMask  = ~dataMask;

    /* set 0 from startBit to endBit */
    orgValue &= orgMask;

    /* move setValue to position */
    setValue = setValue << startBit;
    setValue &= dataMask;

    /* set value to reg */
    setValue |= orgValue;

    return setValue;
}

/*
 * read value  from startBit to endBit
 * startBit: startBit in 32bit,lower edge.
 * endBit:  endBit in 32bit,upper edge
 */
MODULE_EXPORTED VOS_UINT32 TTF_GetByBit(VOS_UINT32 pid, VOS_UINT32 orgValue, VOS_UINT8 startBit, VOS_UINT8 endBit)
{
    VOS_UINT32 orgMask = 0xFFFFFFFF;

    if ((startBit > endBit) || (startBit > TTF_REG_MAX_BIT) || (endBit > TTF_REG_MAX_BIT)) {
        TTF_LOG3(pid, DIAG_MODE_COMM, PS_PRINT_WARNING,
            "TTF_GetByBit para err, orgValue <1>, startBit <2>, endBit <3>\r\n", orgValue, startBit, endBit);
        return 0;
    }

    /* to get the mask form startBit to endbit */
    orgMask = orgMask >> startBit;
    orgMask = orgMask << (TTF_REG_MAX_BIT - (endBit - startBit));
    orgMask = orgMask >> (TTF_REG_MAX_BIT - endBit);

    /* get value from startBit to endBit */
    orgValue &= orgMask;

    return orgValue >> startBit;
}

/* ��Ԫ������ΪVOS_UINT16�����飬������������ */
MODULE_EXPORTED VOS_VOID TTF_InsertSortAsc16bit(
    VOS_UINT32 pid, VOS_UINT16 sortElement[], VOS_UINT32 elementCnt, VOS_UINT32 maxCnt)
{
    VOS_UINT16 tempElement;
    VOS_UINT32 loop;
    VOS_UINT32 prevPos;

    if (elementCnt == 0) {
        return;
    }

    if (elementCnt > maxCnt) {
        TTF_LOG2(
            pid, 0, PS_PRINT_NORMAL, "TTF_InsertSortAsc16bit::elementCnt is more than MaxCnt!", elementCnt, maxCnt);
        elementCnt = maxCnt;
    }

    for (loop = 1; loop < elementCnt; loop++) {
        if (sortElement[loop] >= sortElement[loop - 1]) {
            continue;
        }

        tempElement = sortElement[loop];
        prevPos     = loop;

        do {
            sortElement[prevPos] = sortElement[prevPos - 1];
            prevPos--;
            if (prevPos == 0) {
                break;
            }
        } while (tempElement < sortElement[prevPos - 1]);

        sortElement[prevPos] = tempElement;
    }
}

/* ��Ԫ������ΪUINT16�����飬�Ƴ���ͬ,�����������������ģ��Ҵ���ԭʼ�����Сֵ */
MODULE_EXPORTED VOS_VOID TTF_RemoveDupElement16bit(
    VOS_UINT32 pid, VOS_UINT16 sortedElement[], VOS_UINT32 *elementCnt, VOS_UINT32 maxCnt)
{
    VOS_UINT32 loop;
    VOS_UINT32 filterAfterCnt;

    if (*elementCnt <= 1) {
        return;
    }

    if (*elementCnt > maxCnt) {
        TTF_LOG2(pid, 0, PS_PRINT_NORMAL, "TTF_RemoveDupElement16bit:: elementCnt invalid", *elementCnt, maxCnt);
        *elementCnt = maxCnt;
    }

    filterAfterCnt = 1;
    for (loop = 1; loop < *elementCnt; loop++) {
        if (sortedElement[filterAfterCnt - 1] != sortedElement[loop]) {
            sortedElement[filterAfterCnt] = sortedElement[loop];
            filterAfterCnt++;
        }
    }

    *elementCnt = filterAfterCnt;

    return;
}

MODULE_EXPORTED VOS_UINT32 TTF_Sum(const VOS_UINT32 *data, VOS_UINT32 num)
{
    VOS_UINT32 total = 0;

    while (num > 0) {
        total += *data;
        data++;
        num--;
    }
    return total;
}

/*
 * �����ݿ����Զ�Ӧ�Ļ���Buffer��
 * buf : ��Ӧ����Buffer����ʼ��ַ �� buf��С
 * offset  :����ʱ��ƫ�Ƶ�ַ
 * srcData : ����Դ���ݵ�ַ
 * dataLen : �������ݵ�Ŀ��UPA����Ļ���ַ
 */
MODULE_EXPORTED VOS_VOID TTF_RingBufWrite(VOS_UINT32 pid, TtfRingBuf *buf, VOS_UINT16 offset,
    const VOS_UINT8 *srcData, VOS_UINT16 dataLen)
{
    VOS_UINT32 leftLen;
    VOS_UINT8 *dstData = VOS_NULL_PTR;

    if ((dataLen >= buf->size) || (offset >= buf->size) || (srcData == VOS_NULL_PTR)) {
        TTF_LOG3(pid, PS_SUBMOD_NULL, PS_PRINT_WARNING, "TTF_RingBufWrite, ulDataLen <1> wrong with ulModLen <2>",
            dataLen, offset, buf->size);
        return;
    }

    leftLen = buf->size - offset;
    dstData = buf->baseAddr + offset;

    if (leftLen >= dataLen) {
        TTF_SF_CHK(memcpy_s(dstData, leftLen, srcData, dataLen));
    } else {
        TTF_SF_CHK(memcpy_s(dstData, leftLen, srcData, leftLen));
        TTF_SF_CHK(memcpy_s(buf->baseAddr, buf->size, srcData + leftLen, dataLen - leftLen));
    }
}

/*
 * �����ݴӶ�Ӧ�Ļ���Buffer�п�������
 * buf:  ��ӦҪ�����Ļ���Buffer����ʼ��ַ �� buf��С
 * offset : ��ӦҪ����ʱ��ƫ�Ƶ�ַ
 * dstData : ������Ŀ�����ݵ�ַ
 * dataLen : Ҫ�������ݵĳ���
 */
VOS_VOID TTF_RingBufRead(VOS_UINT32 pid, TtfRingBuf *buf, VOS_UINT32 offset, VOS_UINT8 *dstData, VOS_UINT16 dataLen)
{
    VOS_UINT32 leftLen;
    VOS_UINT8 *srcData = VOS_NULL_PTR;

    if ((dataLen >= buf->size) || (offset >= buf->size) || (dstData == VOS_NULL_PTR)) {
        TTF_LOG3(pid, PS_SUBMOD_NULL, PS_PRINT_WARNING, "TTF_RingBufRead, data len error.", dataLen, offset, buf->size);
        return;
    }

    leftLen = buf->size - offset;
    srcData = buf->baseAddr + offset;

    if (dataLen > leftLen) {
        TTF_SF_CHK(memcpy_s(dstData, dataLen, srcData, leftLen));
        TTF_SF_CHK(memcpy_s((dstData + leftLen), (dataLen - leftLen), buf->baseAddr, (dataLen - leftLen)));
    } else {
        TTF_SF_CHK(memcpy_s(dstData, dataLen, srcData, dataLen));
    }
}

/*
 * TTF ��ȫ��������ֵУ�飬У��ʧ�ܻᴥ����λ��
 * ֧�ֺ����� memset_s memcpy_s memmove_s
 */
MODULE_EXPORTED VOS_VOID TTF_CheckSafeFuncRslt(VOS_BOOL result, VOS_UINT32 fileNo, VOS_UINT32 lineNo)
{
    if (result) {
        mdrv_om_system_error((VOS_INT32)TTF_MEM_CPY_FAIL_ERROR, (VOS_INT32)fileNo, (VOS_INT32)lineNo, VOS_NULL_PTR, 0);
    }
}

/*
 * TTF Acore��ȫ��������ֵУ�飬У��ʧ�ܻ��ӡ���󣬲��ܸ�λ��
 * ֧�ֺ����� memset_s memcpy_s memmove_s
 */
MODULE_EXPORTED VOS_VOID TTF_RecordSafeFuncFailLog(VOS_BOOL result, VOS_UINT32 fileNo, VOS_UINT32 lineNo)
{
    if (result) {
        TTF_LOG3(UEPS_PID_PSCOMM, 0, PS_PRINT_ERROR, "SafeFunc return value check fail!", result, fileNo, lineNo);
    }
}

/*lint -restore */

