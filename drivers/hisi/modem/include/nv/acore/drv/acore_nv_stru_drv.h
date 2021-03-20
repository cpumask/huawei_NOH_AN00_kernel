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

#ifndef __DRV_NV_DEF_ACORE_H__
#define __DRV_NV_DEF_ACORE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include "acore_nv_id_drv.h"

/*
 * �ṹ��  : NV_SECBOOT_ENABLE_FLAG
 * ID˵��  : ID=50201
 * �ṹ˵��  : ��ʶ����汾�Ƿ���Ҫ������ȫ���ԣ�16�ֽڵ�������
 */
typedef struct
{
    /*
    * Range:[0,1]
    * 0���ǰ�ȫ���ܰ汾����ִ��EFUSE��д��
    * 1����ȫ���ܰ汾����ִ��EFUSE��д��
    */
    u16 usNVSecBootEnableFlag; 
}NV_SECBOOT_ENABLE_FLAG;

/* USB Feature for usb id:50075 */
/* �ṹ˵��  : USB��Ӳ�����Կ���NV���֧��MBB��Ʒ��Kirin960��ֻ��P531-bbit��̬��Ʒʹ�á� */
typedef struct
{
    /*
     * bit 0��wwan_flags���ж�PC�����Ƿ�Ϊ����������
     * 0��no��1��yes��
     * bit 1��bypass_mode��ѡ������ģʽ��
     * 0��stickģʽ��1��E5 ģʽ��
     * bit 2��vbus connect��vbus����оƬ��vbus���ţ�ͨ��Ϊ0�����ֳ���Ϊ1����
     * 0��disconnect��1��connect��
     * bit 3��sys_err_on_disable��USB��ö�ٿ�ά�ɲ⣬��������
     * 0��no��1��yes��
     * bit 4��phy_auto_pd����ʹ�õ�usb phy����ö����ɹر����ԣ����ܺ�hibernation_supportͬʱʹ�á�
     * bit5��force_highspeed��USB�Ƿ�ǿ������USB2.0���л���SGMII���ܣ�SGMII��V765֧�֣���
     * 0��no��1��yes��
     * bit6��usb shell���Ƿ�ʹ��usb shell��
     * 0��no��1��yes��
     * bit7��resv��
     */
    u8 flags;
    /*
     * bit 0: 0��usb���壬1��usbǿ�Ʒ���
     * bit 1: 0��linux rndis����ʹ��Ĭ��2K���ˮ�ߣ�1��linux rndis����ʹ��16Kˮ��
     */
    u8 reserve1;
    /*
     * usb hibernation���ܡ�֧��usb hibernation������acore��usb����ʱ����˯�ߡ�
     * 0��no��1��yes��
     */
    u8 hibernation_support;
    /*
     * PC���������͡�
     * 0��ECM������Ҫʹ�ô�����; 1��NCM������Ҫʹ�ô�������2������; 3: ����Linux PCԭ��ECM������Ҫ֧���ϱ�subclass 0x3��
     */
    u8 pc_driver;
    /*
     * USB��μ��ģʽ��
     * 0��pmu detect ͨ��PMU���USB��Σ�
     * 1��no detect �����м�⣬USB���ֳ�����
     * 2��car module detect����ģ��T-box���ģʽ��GPIO�����룬оƬvbus���γ���
     * 3: �Խ�linux pc
     */
    u8 detect_mode;
    /*
     * ���ְ汾��usb ip������ʹ��USB3.0��U1U2ʡ��ģʽʱ����ö�ٵ�оƬ�߼����⣬Ϊ�˹�����������Ӵ˺ꡣ
     * 0���ر�u1u2���ܣ�
     * 1��ʹ��u1u2����ʹ��synopsis�ṩ�Ĺ�ܷ�����
     * 2��ʹ��u1u2���ܣ���ʹ�ù�ܷ�����
     */
    u8 enable_u1u2_workaround;
    /*
     * ʹ��gpio�˿ڽ���usb id�ߵ͵�ƽ��⡣����OTGģʽ�е���̬ʶ��
     * 0���رռ�⣻
     * 1���򿪼�⡣
     */
    u8 usb_gpio_support:4;
    /*
     * USB��ģʽѡ��
     * 0��deviceģʽ��
     * 1��hostģʽ��
     * 2��otgģʽ��
     */
    u8 usb_mode:4;
    /*
     * bit[0:3]:����ģʽѡ��
     * 0��NCM�����Զ�Э��ģʽ������������Э��Ϊnth16����nth32��
     * 1��RNDIS����ģʽ��
     * 2��ECM����ģʽ��
     * 3��NCM����ǿ��ģʽ��ǿ�Ƶ���֧��nth16��
     * bit[4:7]:��������ѡ��Ĭ��ֵ0��ʹ��1��������ֻ��stickģʽ�µ�NCM/ECM֧�����4��������
     * 0��Ĭ��ֵ��ʹ��1��������
     * 1/2/3/4��ö�ٶ�Ӧ����������
     */
    u8 network_card;
} NV_USB_FEATURE;

/* SPE Feature for usb id HEX:0xd12c DEC:53548*/
/* �ṹ˵�����ڲ�ʹ�� */
typedef struct
{
    u32 smp_sch:1;
    u32 smp_hp:1;
    u32 mulwan:1;
    u32 ipf_extdesc:1;
    u32 ack_extdesc:1;
    u32 direct_fw:1;
    u32 reserve:26;
}DRV_SPE_FEATURE;

/* C�˵�����λ���迪����������NV�� = 0xd134
 * �ṹ˵��  : ����C�˵�����λ�����Ƿ�򿪣��Լ��Ƿ����������ril�ĵ�����λ����
 */
typedef struct
{
    /*
     * �Ƿ���C�˵�����λ���ԡ�
     * 0����
     * 1����
     */
    u32 is_feature_on:1;         
    /*
     * �Ƿ��������ril��C�˵�����λ����
     * 0����
     * 1����
     */
    u32 is_connected_ril:1;      
    u32 reserve:30;              /* bit 2-31*/
} DRV_CCORE_RESET_STRU;

/* NVID=0xd144 begin */
typedef struct
{
    u32 mdm_dump     :1;     /* modem_dump.bin */
    u32 mdm_sram     :1;     /* modem_sram.bin */
    u32 mdm_share    :1;     /* modem_share.bin */
    u32 mdm_ddr      :1;     /* modem_ddr.bin */
    u32 lphy_tcm     :1;     /* lphy_tcm.bin */
    u32 lpm3_tcm     :1;     /* lpm3_tcm.bin */
    u32 ap_etb       :1;     /* ap_etb.bin */
    u32 mdm_etb      :1;     /* modem_etb.bin */
    u32 reset_log    :1;     /*reset.log*/
    u32 cphy_tcm     :1;     /* cphy_tcm.bin */
    u32 mdm_secshare :1;     /* secshare.bin */
    u32 mdm_dts      :1;     /* modem_dts.bin */
    u32 llram_share  :1;     /* modem_llram.bin */
    u32 reserved2   :19;
}DUMP_FILE_BITS;

/*
 * �ṹ˵��  : ��ά�ɲ����õ�ǰ��Ҫ������쳣�ļ�NV��
 * ��NV��B5000ƽ̨�޸�Ϊ�ɶ������ԣ��ն˰����޸Ķ��ƣ�
 */
typedef struct
{
    union
    {
        u32         file_value;
        /*
         * bit0���Ƿ񱣴�modem_dump.bin�쳣�ļ���
         * bit1���Ƿ񱣴�modem_sram.bin��
         * bit2���Ƿ񱣴�modem_share.bin�쳣�ļ���
         * bit3���Ƿ񱣴�modem_ddr.bin�쳣�ļ���
         * bit4���Ƿ񱣴�lphy_tcm.bin�쳣�ļ���
         * bit5���Ƿ񱣴�lpm3_tcm.bin�쳣�ļ���
         * bit6���Ƿ񱣴�ap_etb.bin�쳣�ļ���
         * bit7���Ƿ񱣴�modem _etb.bin�쳣�ļ���
         * bit8: �Ƿ񱣴�reset.log�ļ�
         * bit9���Ƿ񱣴�cphy_tcm.bin�쳣�ļ���
         * bit10���Ƿ񱣴�modem_secshare.bin�쳣�ļ���
         * ����������
         */
        DUMP_FILE_BITS  file_bits;
    } file_list;
    u32             file_cnt; /* ��¼�쳣�ļ����� */
}DUMP_FILE_CFG_STRU;
/* �ṹ˵�����������߷�������⹦���Ƿ����Լ��������Ƿ�λ */
typedef struct
{
    u32 enable;        /* ���߹����Ĵ�������ʹ�ܱ�־, 0, ȥʹ��;  1, ʹ�� */
    u32 reset_enable;  /* ��λʹ�ܱ�־, 0, ȥʹ��; 1, ʹ�� */
}DRV_PDLOCK_AP_CFG_STRU;

/* dlock = 0xd155 */
/* �ṹ˵�������� */
typedef struct
{
    u32 enable;        /* ���߹����Ĵ�������ʹ�ܱ�־, 0, ȥʹ��;  1, ʹ�� */
    u32 reset_enable;  /* ��λʹ�ܱ�־, 0, ȥʹ��; 1, ʹ�� */
}DRV_DLOCK_CFG_STRU;

/*end rse mipi ���÷�ʽ*/

/* 0xD14d
 * �ṹ˵��  : ����NV log�Ƿ񱣴棬NV�쳣ʱ����ķ�ʽ��
 */
typedef struct
{
    /*
     * ���ֵ�ǰ�汾ΪMBB��Ʒ����Phone��Ʒ��
     * 0��Phone��Ʒ��
     * 1��MBB��Ʒ��
     */
    u32 product         :2;
    /*
     * ���NV�����Ƿ���Ҫ��λ���塣
     * 0������λ��
     * 1����λ��
     */
    u32 reset           :1;
    /*
     * �ڱ��ݷ������ݳ����������Ƿ񱣴汸�ݷ�������
     * 0��������
     * 1������
     */
    u32 save_bakup      :1;
    /*
     * �ڹ����������ݳ����������Ƿ񱣴汸�ݷ�������
     * 0��������
     * 1������
     */
    u32 save_image      :1;
    /*
     * ��NV DDR���ݳ����������Ƿ񱣴汸�ݷ�������
     * 0��������
     * 1������
     */
    u32 save_ddr        :1;
    /*
     * ��NV DDR ���ݳ����ҹ�������������Ȼ����ȷ�ĵ�������Ƿ���н����ָ�
     * 0�����ָ�
     * 1�����лָ�
     */
    u32 resume_bakup    :1;  
    /*
     * ��NV DDR ���ݳ����������Ƿ���н����ָ�
     * 0�����ָ�
     * 1�����лָ�
     */
    u32 resume_img      :1;  
    u32 reserved        :24;  /* Ԥ���� */
}NV_DEBUG_CFG_STRU;

/* NV ID = 0xd157 DVSʹ�ܿ��Ƽ�hpm��ֵ����
 * [NV elf������Ŀ] �ṹ�����ظ� xuziheng/306651 20171209
 */
#ifndef NV_DEFINE
typedef struct {
    u32 dvs_en;
    u32 hpm_delay;
}ST_DVS_CONFIG_STRU;
#endif

/* �ṹ˵��  : mbb��Ʒ����noc err��⹦���Ƿ���Լ�noc�쳣�Ƿ�λ��phone��̬�رգ�mbb��̬�򿪣��� */
typedef struct
{
    u32 enable;         /* mbb���noc�쳣��⹦���Ƿ�򿪣�0���رգ�1���� */
    u32 reset_enable;   /* Mbb noc�жϴ�����Ƿ�λ��0������λ��1����λ */
}DRV_NOC_AP_CFG_STRU;
/* �ṹ˵�����������߼�⹦���Ƿ����Լ��������Ƿ�λ */
typedef struct
{
    u32 enable;         /*mbb ap��bus err probe����ʹ�ܱ�־, 0, ȥʹ��;  1, ʹ��*/
	u32 reset_enable;   /*mbb ap�ิλʹ�ܱ�־, 0, ȥʹ��; 1, ʹ��*/
}DRV_BUS_ERR_AP_CFG_STRU;

/*
 * �ṹ˵��  : ��NV��Ĺ�����ʵ��DMSS�жϴ������̵�ʹ�ܿ��ƺʹ���DMSS�жϺ��Ƿ�λ�Ŀ��ƣ�
 * ��NV��ʹ������DMSSģ���ϵͳ�У�Ŀǰֻ֧��V765ƽ̨���ֻ�ƽ̨��֧�֡�
 */
typedef struct
{
    u32 enable;        /* enable����ʹ��DMSS�жϴ������̣���Ҫʹ�ܣ�����enableΪ0x1����ʹ������Ϊ0x0 */
    u32 reset_enable;  /* reset_enable���ڿ��ƴ���DMSS�жϺ��Ƿ�λ����Ҫ��λ������reset_enableΪ0x1������λ������Ϊ0x0 */
}DRV_DMSS_CFG_STRU;

/*amon���ID NV���� 0xd16e*/
typedef struct
{
    u32 id;
    u32 id_enable;      /*�Ƿ�������id�ļ�ع���*/
    u32 port;           /* ��ض˿� */
    u32 master_id;      /* ���masterid */
    char master_name[16];
    u32 start_addr;     /* �����ʼ��ַ */
    u32 end_addr;
    u32 opt_type;       /* ��صĲ������ͣ�01:����10:д��11:��д������ֵ:����� */
    u32 cnt_type;       /* ��λ��־��ƥ�䵽��������Ƿ�λ */
} AMON_AP_NV_ID_CFG;

typedef struct
{
    u32 amon_enable;
    AMON_AP_NV_ID_CFG id_cfg[8];
}AMON_AP_NV_CFG;

/* �ṹ˵��  : �ڲ�ʹ�� */
typedef struct
{
    AMON_AP_NV_CFG amon_nv_cfg[1];
}DRV_AMON_AP_NV_CFG_STRU;

/* socp rate ctrl */
/* �ṹ˵��  : NV������������SOCP������ϢNV�� */
typedef struct {
    u32 chan_enable;   /* ���������Ƿ�� */
    u32 rate_limits;    /* ����ͨ���ܵ��������ޣ���λMB/s */
    u32 rev;           /* ���� */
    u32 chan_rate_bits_l;   /* SOCP Դͨ����32��ͨ������ʹ��λ */
    u32 chan_rate_bits_h;   /* SOCP Դͨ����32��ͨ������ʹ��λ*/
}DRV_DIAG_RATE_STRU;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

