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
 *  @file    mdrv_ipf_common.h
 *  @author  zhangsanmao
 *  @version v1.0
 *  @date    2019.12.17
 *  @note    修改记录(版本号|修订日期|说明)
 *  <ul><li>v1.0|2019.12.17|增加说明：该头文件5010融合架构不对外提供</li></ul>
 *  @since
 */



#ifndef __MDRV_IPF_COMMON_H__
#define __MDRV_IPF_COMMON_H__

#include <product_config.h>
#ifdef __cplusplus
extern "C"
{
#endif

/* error code definition */
#define IPF_SUCCESS             0
#define IPF_ERROR               -1
#define BSP_MODU_IPF 2
#define BSP_ERR_IPF_INVALID_PARA            BSP_DEF_ERR(BSP_MODU_IPF, 0)
#define BSP_ERR_IPF_BDQ_NOT_ENOUGH          BSP_DEF_ERR(BSP_MODU_IPF, 1)
#define BSP_ERR_IPF_CDQ_NOT_ENOUGH          BSP_DEF_ERR(BSP_MODU_IPF, 2)
#define BSP_ERR_IPF_RDQ_EMPTY               BSP_DEF_ERR(BSP_MODU_IPF, 3)
#define BSP_ERR_IPF_FILTER_NOT_ENOUGH       BSP_DEF_ERR(BSP_MODU_IPF, 4)
#define BSP_ERR_IPF_NOT_INIT                BSP_DEF_ERR(BSP_MODU_IPF, 5)
#define BSP_ERR_IPF_RESUME_TIMEOUT          BSP_DEF_ERR(BSP_MODU_IPF, 6)
#define BSP_ERR_IPF_SEMTAKE_TIMEOUT         BSP_DEF_ERR(BSP_MODU_IPF, 7)
#define BSP_ERR_IPF_ADQ0_NOT_ENOUGH         BSP_DEF_ERR(BSP_MODU_IPF, 8)
#define BSP_ERR_IPF_ADQ1_NOT_ENOUGH         BSP_DEF_ERR(BSP_MODU_IPF, 9)
#define BSP_ERR_IPF_ALLOC_MEM_FAIL          BSP_DEF_ERR(BSP_MODU_IPF, 10)
#define BSP_ERR_IPF_CCORE_RESETTING         BSP_DEF_ERR(BSP_MODU_IPF, 11)
#define BSP_ERR_IPF_INVALID_DLRD            BSP_DEF_ERR(BSP_MODU_IPF, 12)
#define BSP_ERR_IPF_INVALID_ULRD            BSP_DEF_ERR(BSP_MODU_IPF, 13)

#define IPF_UL_ADQ_LEN_EXP          (CONFIG_IPF_ADQ_LEN-1)
#define IPF_DL_ADQ_LEN_EXP          (CONFIG_IPF_ADQ_LEN)
#ifdef CONFIG_NEW_PLATFORM
#define IPF_ULBD_DESC_SIZE         (16*(1 << IPF_UL_ADQ_LEN_EXP))   /* ����BD���� */
#define IPF_ULRD_DESC_SIZE         (16*(1 << IPF_UL_ADQ_LEN_EXP))   /* ����BD���� */
#else
#define IPF_ULBD_DESC_SIZE         (64)   /* ����BD���� */
#define IPF_ULRD_DESC_SIZE         (64)   /* ����BD���� */
#endif
#define IPF_DLBD_DESC_SIZE         (16*(1 << IPF_DL_ADQ_LEN_EXP))   /* ����BD���� */
#define IPF_DLRD_DESC_SIZE         (16*(1 << IPF_DL_ADQ_LEN_EXP))   /* ����BD���� */
#define IPF_ULAD0_DESC_SIZE      (32*(1 << IPF_UL_ADQ_LEN_EXP))   /* ����ADQ0��AD���� */
#define IPF_ULAD1_DESC_SIZE      (32*(1 << IPF_UL_ADQ_LEN_EXP))   /* ����ADQ1��AD���� */
#define IPF_DLAD0_DESC_SIZE        (32*(1 << IPF_DL_ADQ_LEN_EXP))   /* ����ADQ0��AD���� */
#define IPF_DLAD1_DESC_SIZE        (32*(1 << IPF_DL_ADQ_LEN_EXP))   /* ����ADQ1��AD���� */

#define IPF_DLCD_DESC_SIZE         /*lint -save -e506 */ ((IPF_DLBD_DESC_SIZE>1024)?IPF_DLBD_DESC_SIZE:1024)/*lint -restore */
#define IPF_FILTER_CHAIN_MAX_NUM   (16)    /* IPF�������������� */

/*ADQʹ��ö��ֵ*/
typedef enum tagipf_adqen_e
{
    IPF_NONE_ADQ_EN = 0,    /*��ʹ��ADQ*/
    IPF_ONLY_ADQ0_EN = 1,   /*��ʹ��ADQ0*/
    IPF_ONLY_ADQ1_EN = 2,   /*��ʹ��ADQ1*/
    IPF_BOTH_ADQ_EN = 3,    /*ʹ��ADQ0��ADQ1*/

    IPF_ADQEN_BUTT          /*�߽�ֵ*/
}ipf_adqen_e;

/*ADSize*/
typedef enum tagipf_adsize_e
{
    IPF_ADSIZE_32 = 0,  /* AD���д�СΪ32 */
    IPF_ADSIZE_64,      /* AD���д�СΪ64 */
    IPF_ADSIZE_128,     /* AD���д�СΪ128 */
    IPF_ADSIZE_256,     /* AD���д�СΪ256*/

    IPF_ADSIZE_MAX
}ipf_adsize_e;

/* ����ģʽ */
typedef enum tagipf_mode_e
{
	IPF_MODE_FILTERANDTRANS,
	IPF_MODE_FILTERONLY,
    IPF_MODE_TRANSONLY,
    IPF_MODE_TRANSONLY2,
    IPF_MODE_MAX
}ipf_mode_e;

/*ADQ�ձ�־ö��ֵ*/
typedef enum tagipf_adq_empty_e
{
    IPF_EMPTY_ADQ0 = 0,
    IPF_EMPTY_ADQ1 = 1,
    IPF_EMPTY_ADQ = 2,
    IPF_EMPTY_MAX
}ipf_adq_empty_e;

/* IPF״̬��ʶ */
typedef enum tagipf_restore_state_e
{
    IPF_STATE_UNRESTORE = 0,   /* δ�ָ� */
    IPF_STATE_RESTORING,       /* �ָ��� */
    IPF_STATE_RESTORED,        /* �ѻָ� */

    IPF_STATE_MAX
}ipf_restore_state_e;

/* IP���� */
typedef enum tagipf_ip_type_e
{
    IPF_IPTYPE_V4 = 0,  /* IPV4 */
    IPF_IPTYPE_V6,      /* IPV6 */

    IPF_IPTYPE_MAX      /* �߽�ֵ */
}ipf_ip_type_e;

typedef enum tagipf_ad_type_e
{
    IPF_AD_0 = 0,        /* AD0 ʹ������AD����ʱΪ�̰����У�ʹ��һ��AD����ʱΪ��������*/
    IPF_AD_1,            /* AD1 ��������*/
    IPF_AD_MAX           /* �߽�ֵ */
}ipf_ad_type_e;

typedef enum tagipf_channel_type_e
{
    IPF_CHANNEL_UP	= 0,
    IPF_CHANNEL_DOWN,
    IPF_CHANNEL_MAX
}ipf_channel_type_e;

typedef enum tagipf_filter_chain_type_e
{
	IPF_MODEM0_ULFC = 0,
	IPF_MODEM1_ULFC,
    IPF_MODEM2_ULFC,
	IPF_MODEM0_DLFC,
	IPF_MODEM1_DLFC,
    IPF_MODEM2_DLFC,
    IPF_1XHRPD_ULFC,
    IPF_LTEV_DLFC,
    IPF_LTEV_ULFC,
	IPF_MODEM_MAX,
}ipf_filter_chain_type_e;

/* ͨ�����ƽṹ�� */
typedef struct tagipf_chl_ctrl_s
{
	int data_chain;
	int endian;
	ipf_mode_e ipf_mode;
    unsigned int wrr_alue;
} ipf_chl_ctrl_s;


/* ͳ�Ƽ�����Ϣ�ṹ�� */
typedef struct tagipf_filter_stat_s
{
    unsigned int ul_cnt0;
    unsigned int ul_cnt1;
    unsigned int ul_cnt2;
    unsigned int dl_cnt0;
    unsigned int dl_cnt1;
    unsigned int dl_cnt2;
}ipf_filter_stat_s;

struct mdrv_ipf_ops {
    int (*adq_empty_cb)(ipf_adq_empty_e adq_empty);
    int (*rx_complete_cb)(void);
};

#ifndef CONFIG_NEW_PLATFORM

typedef struct
{
    unsigned short len;         /* ��������Դ���� */
    unsigned short attribute;   /* ����IP������ */
	unsigned int data;        /* ��������Դ��ַ */
    unsigned short usr_field1;   /* Usr field ��1 ,͸�� */
    unsigned short reserved;    /* Ԥ�� */
    unsigned int   usr_field2;   /* Usr field ��2 ,͸�� */
    unsigned int   usr_field3;   /* Usr field ��3 ,͸��*/
}ipf_config_param_s;

/* RD������ */
typedef struct
{
    unsigned short attribute;
    unsigned short pkt_len;
	unsigned int in_ptr;
    unsigned int out_ptr;

    unsigned short result;
    unsigned short usr_field1;
    unsigned int usr_field2;
    unsigned int usr_field3;
}ipf_rd_desc_s;

/* BD������ */		
typedef struct tagipf_bd_desc_s		
{		
	unsigned short attribute;		
	unsigned short pkt_len;		
	unsigned int in_ptr;		
	unsigned int out_ptr;		
	unsigned short result;		
	unsigned short usr_field1;		
	unsigned int usr_field2;		
	unsigned int usr_field3;		
} ipf_bd_desc_s;

/* AD������ */
typedef struct tagipf_ad_desc_s
{
    unsigned int out_ptr0;
    unsigned int out_ptr1;
} ipf_ad_desc_s;

/* CD������ */
typedef struct tagipf_cd_desc_s
{
    unsigned short attribute;
    unsigned short pkt_len;
    unsigned int ptr;
}ipf_cd_desc_s;

#endif

#ifdef CONFIG_IPF
/*****************************************************************************
* �� �� ��  : mdrv_ipf_register_ops
*
* ��������  : ע��ops������Ŀǰ����������/�������ݴ�����жϻص�������ADQ��
*             �жϵ���Ӧ����
*
* �������  : struct mdrv_ipf_ops *ops  ��Ӧ�Ļص�������
* �������  :
*
* �� �� ֵ  : IPF_SUCCESS ���óɹ�
*             IPF_ERROR ����ʧ��
* ����˵��  :
*
*****************************************************************************/
int mdrv_ipf_register_ops(struct mdrv_ipf_ops *ops);
#else
static inline int mdrv_ipf_register_ops(struct mdrv_ipf_ops *ops)
{
    return 0;
}
#endif

#ifdef __cplusplus
}

#endif

#endif
