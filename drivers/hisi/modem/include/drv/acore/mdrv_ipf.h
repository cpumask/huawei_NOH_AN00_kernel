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

/**
 *  @brief   765等老平台ipf acore对外头文件，5010融合架构不对外提供。
 *  @file    mdrv_ipf.h
 *  @author  zhangsanmao
 *  @version v1.0
 *  @date    2019.12.17
 *  @note    修改记录(版本号|修订日期|说明)
 *  <ul><li>v1.0|2019.12.17|增加说明：该头文件5010融合架构不对外提供</li></ul>
 *  @since
 */



#ifndef __MDRV_ACORE_IPF_H__
#define __MDRV_ACORE_IPF_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "mdrv_ipf_comm.h"

#ifdef CONFIG_NEW_PLATFORM
typedef	void*	modem_phy_addr;

typedef struct
{
    unsigned short len;         /* ��������Դ���� */
    unsigned short int_en;         /* �ж�ʹ�� */
    unsigned short fc_head;        /* ����IP��������ͷ�� */
	unsigned short mode;
	modem_phy_addr data;        /* ��������Դ��ַ */
    unsigned short usr_field1;   /* Usr field ��1 ,͸�� */
    unsigned int   usr_field2;   /* Usr field ��2 ,͸�� */
    unsigned int   usr_field3;   /* Usr field ��3 ,͸��*/
}ipf_config_param_s;

/* RD������ */
typedef struct
{
    unsigned short attribute;
    unsigned short fc_head;
    unsigned short pkt_len;
    modem_phy_addr in_ptr;
    modem_phy_addr out_ptr;
    unsigned short result;
    unsigned short usr_field1;
    unsigned int usr_field2;
    unsigned int usr_field3;
}ipf_rd_desc_s;

/* AD������ */
typedef struct tagipf_ad_desc_s
{
    modem_phy_addr out_ptr0;
    modem_phy_addr out_ptr1;
} ipf_ad_desc_s;

#endif

typedef ipf_config_param_s ipf_config_ulparam_s;
typedef ipf_config_param_s ipf_config_dlparam_s;

typedef int (*bsp_ipf_wakeup_dl_cb)(void);
typedef int (*bsp_ipf_adq_empty_dl_cb)(ipf_adq_empty_e adq_empty);

#ifdef CONFIG_IPF
/*****************************************************************************
* �� �� ��  : mdrv_ipf_config_ulbd
*
* ��������  : ������������
*
* �������  : unsigned int u32Num     ��Ҫ���õ�BD��Ŀ
*             ipf_config_ulparam_s* pstUlPara ���ò����ṹ������ָ��
* �������  :
*
* �� �� ֵ  : IPF_SUCCESS ���óɹ�
*             IPF_ERROR ����ʧ��
* ����˵��  :
*
*****************************************************************************/
int mdrv_ipf_config_ulbd(unsigned int num, ipf_config_ulparam_s*ul_para);

/*****************************************************************************
* �� �� ��  : mdrv_ipf_get_ulbd_num
*
* ��������  : ��ȡ����BDQ�п��е�BD��
*
* �������  :
* �������  :
*
* �� �� ֵ  : ���е�BD��Ŀ����ΧΪ0~BD_DESC_SIZE
*
* ����˵��  :
*
*****************************************************************************/
unsigned int mdrv_ipf_get_ulbd_num(void);

/*****************************************************************************
* �� �� ��  : mdrv_ipf_get_uldesc_num
*
* ��������  : ��ȡ���п������õİ���
*
* �������  : void
* �������  :
*
* �� �� ֵ  : ������ɶ�ͷ�����Ŀ��ϴ��İ���
*
* ����˵��  :
*
*****************************************************************************/
unsigned int mdrv_ipf_get_uldesc_num(void);

/*****************************************************************************
* �� �� ��  : mdrv_ipf_get_ulrd_num
*
* ��������  : ��ȡ����RD��Ŀ
*
* �������  : void
* �������  :
*
* �� �� ֵ  : ����Rd��Ŀ
*
* ����˵��  :
*
*****************************************************************************/
unsigned int mdrv_ipf_get_ulrd_num (void);

/*****************************************************************************
* �� �� ��  : mdrv_ipf_get_dlrd
*
* ��������  : ��ȡ����RD
*
* �������  : unsigned int* pu32Num  ���Է��ص�����RD��Ŀ
*             ipf_rd_desc_s *pstRd  RD�������ṹ���׵�ַ
* �������  :
*
* �� �� ֵ  : ʵ�ʷ��ص�RD��Ŀ
*
* ����˵��  :
*
*****************************************************************************/
void mdrv_ipf_get_dlrd (unsigned int  * num, ipf_rd_desc_s *rd);

/*****************************************************************************
* �� �� ��  : mdrv_ipf_get_dlrd_num
*
* ��������  : ��ȡ����RD��Ŀ
*
* �������  : void
* �������  :
*
* �� �� ֵ  : ����Rd��Ŀ
*
* ����˵��  :
*
*****************************************************************************/
unsigned int mdrv_ipf_get_dlrd_num (void);

/*****************************************************************************
* �� �� ��  : mdrv_ipf_config_dlad
*
* ��������  : �����е�AD�����µ��ڴ滺����
*
* �������  : ipf_ad_type_e eAdType   AD���кţ�0���̰����У���1���������У�
*             unsigned int u32AdNum    ��Ҫ���õĵ�AD��Ŀ
*             ipf_ad_desc_s *pstAdDesc ����ռ��Ӧ�����ݽṹ�׵�ַ
* �������  :
*
* �� �� ֵ  : IPF_SUCCESS ���óɹ�
*             IPF_INVALID_PARA ���������Ч
*
* ����˵��  :
*
*****************************************************************************/
int mdrv_ipf_config_dlad(ipf_ad_type_e ad_type, unsigned int ad_num, ipf_ad_desc_s * ad_desc);

/*****************************************************************************
* �� �� ��  : mdrv_ipf_get_dlad_num
*
* ��������  : ��ȡ���У�A�ˣ����У�ָ��Ļ������Ѿ���ʹ�ã�AD��Ŀ
*
* �������  : unsigned int* pu32AD0Num  ���ؿ��е�AD0����Ŀ
*             unsigned int* pu32AD1Num  ���ؿ��е�AD1����Ŀ
* �������  :
*
* �� �� ֵ  : IPF_SUCCESS ���óɹ�
*             IPF_INVALID_PARA ���������Ч
* ����˵��  :
*
*****************************************************************************/
int mdrv_ipf_get_dlad_num (unsigned int* ad0_num, unsigned int* ad1_num);


/*****************************************************************************
* �� �� ��  : mdrv_ipf_reinit_dlreg
*
* ��������  : CCore������λ�ָ�ʱ������IPF�Ĵ���
*
* �������  : void
* �������  :
*
* �� �� ֵ  :
*
* ����˵��  :
*
*****************************************************************************/
void mdrv_ipf_reinit_dlreg(void);

/*****************************************************************************
* �� �� ��  : mdrv_ipf_get_used_dlad
*
* ��������  : ��ȡδ��ʹ�õ�����AD�Ա�ADS�ͷ�
*
* �������  : ipf_ad_type_e eAdType  AD���ͣ�0���̰����У���1���������У�
*             unsigned int * pu32AdNum    AD��Ŀ
*             ipf_ad_desc_s * pstAdDesc ��Ҫ�ͷŵ�AD�����׵�ַ
* �������  :
*
* �� �� ֵ  : IPF_SUCCESS����������
*             IPF_ERROR��ʧ�ܷ��أ��ȴ���ʱIPFͨ��ʼ��BUSY
*             IPF_INVALID_PARA����γ���
* ����˵��  : ADS��ccore������λ��������Ҫ���øú������ͷŷ��ص�AD�����������ڴ�й¶
*
*****************************************************************************/
int mdrv_ipf_get_used_dlad(ipf_ad_type_e ad_type, unsigned int * ad_num, ipf_ad_desc_s * ad_esc);
#else
static inline int mdrv_ipf_config_ulbd(unsigned int num, ipf_config_ulparam_s* ul_para)
{
    return 0;
}

static inline unsigned int mdrv_ipf_get_ulbd_num(void)
{
    return 0;
}

static inline unsigned int mdrv_ipf_get_uldesc_num(void)
{
    return 0;
}

static inline unsigned int mdrv_ipf_get_ulrd_num (void)
{
    return 0;
}

static inline void mdrv_ipf_get_dlrd (unsigned int  * num, ipf_rd_desc_s *rd)
{
    return;
}

static inline unsigned int mdrv_ipf_get_dlrd_num (void)
{
    return 0;
}

static inline int mdrv_ipf_config_dlad(ipf_ad_type_e ad_type, unsigned int ad_num, ipf_ad_desc_s * ad_desc)
{
    return 0;
}

static inline int mdrv_ipf_get_dlad_num (unsigned int* ad0_num, unsigned int* ad1_num)
{
    return 0;
}

static inline void mdrv_ipf_reinit_dlreg(void)
{
    return;
}

static inline int mdrv_ipf_get_used_dlad(ipf_ad_type_e ad_type, unsigned int * ad_num, ipf_ad_desc_s * ad_desc)
{
    return 0;
}
#endif
#ifdef __cplusplus
}
#endif

#endif
