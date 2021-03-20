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
#include <linux/of_platform.h>
#include <linux/module.h>
#include <linux/clk.h>
#include <linux/fs.h>
#include <osl_malloc.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <asm/uaccess.h>
#include <mdrv.h>
#include <of.h>
#include <osl_thread.h>
#include <linux/cdev.h>
#include <linux/module.h>
#include <osl_bio.h>
#include <securec.h>
#include "mdrv_diag_system_common.h"
#include "bsp_hds_service.h"
#include "bsp_hds_ind.h"
#include "bsp_slice.h"
#include "bsp_diag.h"

#undef THIS_MODU
#define THIS_MODU mod_hds

u32 g_print_init_state = PRINTLOG_CHN_UNINIT;
u32 g_trans_init_state = TRANSLOG_CHN_UNINIT;

print_report_hook g_bsp_print_hook = NULL;

#if ((FEATURE_HDS_PRINTLOG == FEATURE_ON) || (FEATURE_HDS_TRANSLOG == FEATURE_ON))

void bsp_print_level_cfg(u32 *level)
{
    if ((*level) >= BSP_LOG_LEVEL_ERROR) {
        (*level) = 1;
    } else if (BSP_LOG_LEVEL_WARNING == (*level)) {
        (*level) = 2;
    } else if (BSP_LOG_LEVEL_NOTICE == (*level)) {
        (*level) = 3;
    } else if ((BSP_LOG_LEVEL_INFO == (*level)) || (BSP_LOG_LEVEL_DEBUG == (*level))) {
        (*level) = 4;
    }
}

/*lint -save -e550 */
#if (FEATURE_HDS_TRANSLOG == FEATURE_ON)
s32 bsp_transreport(struct hds_trans_ind *pstData)
{
    s32 ret;

    /* �жϹ�������״̬������״̬ */
    if (0 == g_translog_conn) {
        hds_debug("hids not conn(%d)!\n", g_translog_conn);
        return HDS_TRANS_SW_ERR;
    }

    /* ��μ�� */
    if ((NULL == pstData) || (NULL == pstData->pData) || (0 == pstData->ulLength) ||
        ((pstData->ulLength) > (TRANSLOG_MAX_HIDS_BUFF_LEN - 1))) {
        hds_debug("pstdata err!\n");
        return HDS_ERR;
    }
    ret = mdrv_diag_report_trans((diag_trans_ind_s *)pstData);
    if (ret) {
        hds_debug("write transdata error!\n");
        return ret;
    }
    return HDS_TRANS_RE_SUCC;
}
#endif
/*lint -restore +e550 */

/*
 * new start
 */
void match_type(u32 *level)
{
    switch (*level) {
        case BSP_HIDS_LEVEL_ERROR:
            *level = BSP_LOG_LEVEL_ERROR;
            break;
        case BSP_HIDS_LEVEL_WARNING:
            *level = BSP_LOG_LEVEL_WARNING;
            break;
        case BSP_HIDS_LEVEL_NOTICE:
            *level = BSP_LOG_LEVEL_NOTICE;
            break;
        case BSP_HIDS_LEVEL_INFO:
            *level = BSP_LOG_LEVEL_INFO;
            break;
        default:
            *level = BSP_LOG_LEVEL_DEBUG;
    }
    return;
}
/*
 * ��������: �����ӡ�������ӿ�
 * �������:  mod_id: ���ģ��
 *            print_level: ��ӡ����
 *            fmt :��ӡ�������
 * �������: ��
 * �� �� ֵ: �ɹ�����0
 */
/*lint -save -e530 -e830 -e64*/
int bsp_trace_to_hids(u32 module_id, u32 level, const char *print_buff, va_list arglist)
{
    int ret = HDS_PRINT_SW_ERR;
    match_type(&level);

    if (level < g_printlog_level)
        return HDS_PRINT_SW_ERR;

    /* �жϹ�������״̬������״̬ */
    if ((1 == g_printlog_conn) && (1 == g_printlog_enable)) {
        ret = bsp_diag_report_log(module_id, DIAG_DRV_HDS_PID, NULL, 0, print_buff, arglist);
        if (ret) {
            hds_debug("print data report fail,ret = 0x%x!\n", ret);
        }
        return ret;
    } else {
        return ret;
    }
}
/*lint -restore +e550 */

#endif

/*
 * ��������: ��ʼ������
 * �������: ��
 * �� �� ֵ: �ɹ�����ʶ��
 * ��    ��: 2016��8��10��
 */
int bsp_hds_init(void)
{
    bsp_hds_service_init();

    hds_printf("[init] start!\n");

#if (FEATURE_HDS_PRINTLOG == FEATURE_ON)
    bsp_log_string_pull_hook((bsp_log_string_pull_func)bsp_trace_to_hids);
    g_print_init_state = PRINTLOG_CHN_INIT;
#endif

#if (FEATURE_HDS_TRANSLOG == FEATURE_ON)
    g_trans_init_state = TRANSLOG_CHN_INIT;
#endif

    hds_printf("[init] ok!\n");
    return HDS_OK;
}

#ifndef CONFIG_HISI_BALONG_MODEM_MODULE
/*lint --e{528}*/
module_init(bsp_hds_init);
#endif
EXPORT_SYMBOL(bsp_transreport);
