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
#include "socp.h"

#ifdef __cplusplus
extern "C" {
#endif


socp_attr_s g_socp_info = {SOCP_MEM_VERSION, {0, 0, 0, 0, 0}};
#if 0
extern unsigned long simple_strtoul(const char *cp, char **endp, unsigned int base);
/*****************************************************************************
* �� �� ��  : socp_logbuffer_sizeparse
*
* ��������  : �ڴ������׶ν�CMD LINE�е�BUFFER��С������������
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ  : ��
*****************************************************************************/

static int __init socp_mem_size_parse(char *str)
{

    /* Buffer�Ĵ�С��ByteΪ��λ��ԭ���ϲ�����200M����С��1M */
    g_socp_info.socp_mem_info.buffer_size = (u32)simple_strtoul(str, NULL, 0);
    return 0;
}
early_param("mdmlogsize", socp_mem_size_parse);

/*****************************************************************************
* �� �� ��  : socp_logbuffer_timeparse
*
* ��������  : �ڴ������׶ν�CMD LINE�е�TIMEOUT��С������������
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ  : ��
*****************************************************************************/
static int __init socp_threshold_timeout_parse(char *str)
{

    /* �����ַ�������Ϊ��λ����Ҫ��ת���ɺ��룬����Ϊ1�룬������20���� */
    g_socp_info.socp_mem_info.threshold_timeout = (u32)simple_strtoul(str, NULL,0);

    return 0;
}
early_param("mdmlogtime", socp_threshold_timeout_parse);
/*****************************************************************************
* �� �� ��  : socp_logbuffer_addrparse
*
* ��������  : �ڴ������׶ν�CMD LINE�еĻ���ַ������������
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ  : ��
*****************************************************************************/
static int __init socp_mem_addr_parse(char *str)
{

    g_socp_info.socp_mem_info.base_addr = simple_strtoul(str, NULL, 0);

    return 0;
}
early_param("mdmlogbase", socp_mem_addr_parse);
#endif

/*****************************************************************************
* �� �� ��  : socp_logbuffer_logcfg
*
* ��������  : �ڴ������׶ν�CMD LINE�е�50M����ʹ�ܲ�����������
*
* �������  : �����ַ���
*
* �������  : ��
*
* �� �� ֵ  : �ɹ����
*****************************************************************************/
static int __init socp_rsv_mem_enable(char *str)
{

    g_socp_info.socp_mem_info.rsv_mem_usable = (unsigned int )simple_strtoul(str, NULL, 0);

    return 0;
}
early_param("modemlog_enable", socp_rsv_mem_enable);


/*****************************************************************************
* �� �� ��  : socp_logbuffer_sizeparse
*
* ��������  : �ڴ������׶ν�CMD LINE�е�BUFFER��С������������
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ  : ��
*****************************************************************************/
static int __init socp_bbp_mem_enable(char *str)
{

    g_socp_info.socp_mem_info.bbp_mem_flag = (u32)simple_strtoul(str, NULL, 0);
    return 0;
}

early_param("modem_socp_enable",socp_bbp_mem_enable);

/*****************************************************************************
* �� �� ��  : bsp_socp_get_mem_info
* ��������  : ��ȡԤ���ڴ���Ϣ�ṹ
* �������  : ��
* �������  : ��
* �� �� ֵ  : SOCPԤ���ڴ���Ϣ��ַ
*****************************************************************************/
socp_attr_s *bsp_socp_get_mem_info(void)
{
    return &g_socp_info;
}
EXPORT_SYMBOL(bsp_socp_get_mem_info);

/*****************************************************************************
* �� �� ��  : bsp_socp_ds_mem_enable
*
* ��������  : ��ȡ�����ڴ�Ԥ������
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ  : ��
*****************************************************************************/
u32 bsp_socp_ds_mem_enable(void)
{
    return g_socp_info.socp_mem_info.bbp_mem_flag;
}
EXPORT_SYMBOL(bsp_socp_ds_mem_enable);

/*****************************************************************************
* �� �� ��  : socp_reserve_area
*
* ��������  : ��ȡϵͳԤ����base��size
*
* �������  : �ڴ�ṹ�����
*
* �������  : ��
*
* �� �� ֵ  : �ɹ����
*****************************************************************************/
static int socp_reserve_area(struct reserved_mem *rmem)
{
	#if 0
    char *status;

    status = (char *)of_get_flat_dt_prop(rmem->fdt_node, "status", NULL);
    if (status && (strncmp(status, "ok", strlen("ok")) != 0))
    {
        printk(KERN_ERR"[%s]:status is %s!\n", __FUNCTION__,status);
		return 0;
    }
	#endif
	
	if(g_socp_info.socp_mem_info.rsv_mem_usable) {
		g_socp_info.socp_mem_info.buffer_size = (unsigned int)rmem->size;
		g_socp_info.socp_mem_info.base_addr  = rmem->base;

		printk(KERN_ERR"[%s]:get mdmlog reserved mem\n",__FUNCTION__);		
	}
	else {
		printk(KERN_ERR"[%s]:cmdline modem_socp_enable is 0\n",__FUNCTION__);
	}
	
    return 0;
}

/*lint -save -e611*/
RESERVEDMEM_OF_DECLARE(modem_socp, "hisi_mdmlog", (reservedmem_of_init_fn)socp_reserve_area);
/*lint -restore +e611*/

#ifdef __cplusplus
}
#endif
