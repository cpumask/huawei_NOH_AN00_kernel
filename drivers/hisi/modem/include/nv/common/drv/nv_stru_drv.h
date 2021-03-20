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


#ifndef __DRV_NV_DEF_H__
#define __DRV_NV_DEF_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include "nv_id_drv.h"

#ifndef __KERNEL__

#ifndef __u8_defined
#define __u8_defined
typedef signed char          s8;//lint !e761
typedef unsigned char        u8;//lint !e761
typedef signed short         s16;//lint !e761
typedef unsigned short       u16;//lint !e761
typedef signed int           s32;//lint !e761
typedef unsigned int         u32;//lint !e761
typedef signed long long     s64;//lint !e761
typedef unsigned long long   u64;//lint !e761
#endif

#endif
/*lint --e{959}*/
/*
 * �ṹ��    : nv_protocol_base_type
 * �ṹ˵��  : nv_protocol_base_type�ṹ ID= en_NV_Item_Modem_Log_Path 148
            MBB��̬ modem log·��������������������̬��modem log·����
 */
/*NV ID = 50018*/
#pragma pack(push)
#pragma pack(1)
/* �ṹ˵��  : ����汾�š� */
    typedef struct
    {
        s32        nvStatus;  /* ��ǰNV�Ƿ���Ч�� */
        s8         nv_version_info[30];  /* ����汾����Ϣ�� */
    }NV_SW_VER_STRU;
#pragma pack(pop)

/* NV ID  = 0xd10b */
/* �ṹ˵��  : ����hotplug���Կ��أ���һ��32bit�ĺ��reserved��8��bit�У�ʹ��һ��bit��ʹ�ú�reserved��bit����Ϊ7�� */
typedef struct ST_PWC_SWITCH_STRU_S {
    /*����NV���ڵ͹��ĵ�������ƣ�������ЩBIT��ʱδ�ã�������ʱ�������Ϊ׼ȷ������*/
    u32 deepsleep  :1; /* Ccpu��˯���� 1��ʹ�ܣ�0����ʹ�� */
    u32 lightsleep :1; /* δʹ�á� */
    u32 dfs        :1; /* ��Ƶ���� 1��ʹ�ܣ�0����ʹ�� */
    u32 hifi       :1; /* δʹ�á� */
    u32 drxAbb     :1; /* δʹ�á� */
    u32 drxZspCore :1; /* δʹ�á� */
    u32 drxZspPll  :1; /* δʹ�á� */
    u32 drxWLBbpPll  :1; /* δʹ�á� */
    u32 drxGBbpPll   :1; /* δʹ�á� */
    u32 drxRf      :1; /* δʹ�á� */
    u32 drxPa      :1; /* δʹ�á� */
    u32 drxGuBbpPd   :1; /* δʹ�á� */
    u32 drxDspPd     :1; /* δʹ�á� */
    u32 drxLBbpPd    :1; /* δʹ�á� */
    u32 drxPmuEco    :1; /* δʹ�á� */
    u32 drxPeriPd    :1; /* δʹ�á� */
    u32 l2cache_mntn  :1; /* δʹ�á� */
    u32 bugChk     :1; /* δʹ�á� */
    u32 pmuSwitch     :1; /* δʹ�á� */
    u32 drxLdsp      :1;  /* δʹ�á� */
    u32 matserTDSpd  :1; /* δʹ�á� */
    u32 tdsClk    :1;  /* δʹ�á� */
    u32 slaveTDSpd   :1; /* δʹ�á� */
    u32 slow         :1; /* δʹ�á� */
    u32 ccpu_hotplug_suspend      :1;/*bit24 �˷����κ�ʱ�����γ���cpu��suspend��������ʱ�ϳ�*/
    u32 ccpu_hotplug_crg      :1;  /* Ccpu�Ȳ�η���2����.�ε�cpuʱ�����γ���cpu�������Ӻ�ˢcache�������Ͻ�ʡʱ�䡣���ǵ͹���˯��ʱ��Ҫ�Ȱѱ��γ���cpu���� */
    u32 ccpu_tickless          :1;  /* Ccpu��tickless���ء� */
    u32 rsv  :4;
    u32 ddrdfs    :1; /* Ddrdfs���ƿ��ء� */

    /*����NV����DEBUG���µ�Ϳ�����*/
    u32 drx_pa_pd        :1; /* bit0 ���ڿ���PA�����µ�*/
    u32 drx_rfic_pd      :1; /* bit1 ���ڿ���RFIC�����µ�*/
    u32 drx_rfclk_pd     :1; /* bit2 ���ڿ���RFIC CLK�����µ�*/
    u32 drx_fem_pd       :1; /* bit3 ���ڿ���FEM�����µ�*/
    u32 drx_cbbe16_pd    :1; /* bit4 ���ڿ���CBBE16�����µ�*/
    u32 drx_bbe16_pd     :1; /* bit5 ���ڿ���BBE16�����µ�*/
    u32 drx_abb_pd       :1; /* bit6 ���ڿ���ABB�����µ�*/
    u32 drx_bbp_init     :1; /* bit7 ���ڿ���BBPĬ�Ͻ��ӵ�ȫ������*/
    u32 drx_bbppwr_pd    :1; /* bit8 ���ڿ���BBP��Դ�е����µ�*/
    u32 drx_bbpclk_pd    :1; /* bit9 ���ڿ���BBPʱ���еĿ�����*/
    u32 drx_bbp_pll      :1; /* bit10 ���ڿ���BBP_PLL�Ŀ�����*/
    u32 drx_cbbe16_pll   :1; /* bit11 ���ڿ���CBBE16_PLL�Ŀ�����*/
    u32 drx_bbe16_pll    :1; /* bit12 ���ڿ���BBE16_PLL�Ŀ�����*/
    u32 drx_bbp_reserved :1; /* bit13 bbpԤ��*/
    u32 drx_abb_gpll     :1; /* bit14 ���ڿ���ABB_GPLL�Ŀ�����*/
    u32 drx_abb_scpll    :1; /* bit15 ���ڿ���ABB_SCPLL�Ŀ�����*/
    u32 drx_abb_reserved1:1; /* abbԤ����δʹ�á� */
    u32 drx_abb_reserved2:1; /* abbԤ����δʹ�á� */
    u32 reserved2        :14; /* bit18-31 δ��*/
}ST_PWC_SWITCH_STRU;

/* �ṹ˵��  : ��NVʵ�ֶ�nr�͹������ԵĿ��ؿ��ơ� */
typedef struct ST_PWC_NR_POWERCTRL_STRU_S {

    u8 nrcpudeepsleep;  /* ����NR R8 CPU�͹������Կ��� */
    u8 l2hacdeepsleep;  /* ����L2HAC R8 CPU�͹������Կ��� */
    u8 cpubusdfs;  /* ��Ƶ��ѹ���� */
    u8 hotplug;  /* CPU �Ȳ�Σ���ǰ���Բ��ô� */
    u8 tickless;  /* Tickless����ǰ���Բ��� */
    u8 drxnrbbpinit;  /* nrbbp��Ƭ��ʼ����׮ */
    u8 dxrbbpaxidfs;  /* nrbbp�Զ���Ƶ��׮ */
    u8 drxnrbbpclk;  /* ���� */
    u8 drxnrbbppll;  /* bbppll���ش�׮ */
    u8 drxnrbbppmu;  /* nrbbp���µ��׮ */
    u8 drxbbainit;  /* ���� */
    u8 drxbbapwr;  /* bba�ϵ��׮ */
    u8 drxbbaclk;  /* ���� */
    u8 drxbbapll;  /* ���� */
    u8 drxbbapmu;  /* ���� */
    u8 drxl1c;  /* ���� */
    u8 drxl1cpll;  /* ���� */
    u8 reserved;  /* ���� */
}ST_PWC_NR_POWERCTRL_STRU;

/* �ṹ˵����DDR�±�NV */
typedef struct ST_DDR_TMON_PROTECT_STRU_S{
	u32 ddr_tmon_protect_on :1; /* DDR�±����أ�1:����0:�� */
	u32 ddr_vote_dvfs_alone  :1; /* DDR��Ƶ�Ƿ����±�Ӱ�죬1:�±���Ӱ�죬0:��Ƶ�����±����� */
	u32 reserved1 : 6;
	u32 ddr_tmon_protect_enter_threshold :3; //�±������threshold
	u32 ddr_tmon_protect_exit_threshold :3; //�±��˳�threshold
	u32 ddr_tmon_protect_freq_threshold:2;//�����±���DDR�������Ƶ��
	u32 ddr_tmon_protect_upper:3; //ddr ���¸�λ����
	u32 ddr_tmon_protect_downer:3; //ddr���¸�λ����
	u32 reserved2:10;
}ST_DDR_TMON_PROTECT_STRU;

/* �ṹ˵����DRV PM CHR�ϱ����Կ���NV */
typedef struct ST_CHR_REPORT_STRU_S {
    /*
     * 0��DRV PM CHR�ϱ����Թرգ�
     * ��������˯���ϱ��쳣��ֵ����λΪ���ӡ�
     */
    u32 pm_monitor_time; /*pm monitor time,Unit:minute,1 means if cp not sleep ,CHR will report the pm state*/
    u32 reserved1;
    u32 reserved2;
    u32 reserved3;
}ST_CHR_REPORT_STRU;

/* NV ID = 0xd10c */
/* �ṹ˵��  : ���ڵ͹��Ķ�̬��Ƶ�� */
typedef struct ST_PWC_DFS_STRU_S {
    u32 CcpuUpLimit;  /* C�˸����ϵ���ֵ��Ĭ��80 ����������Ҫ��Ƶ���ϵ��� */
    u32 CcpuDownLimit;  /* C�˸����µ���ֵ��Ĭ��55 ����������Ҫ��Ƶ���µ��� */
    u32 CcpuUpNum;  /* C���������ڸ������޴�����Ĭ��1,��������Ҫ��Ƶ���ϵ��� */
    u32 CcpuDownNum;  /* C���������ڸ������޴�����Ĭ��3,��������Ҫ��Ƶ���µ��� */
    u32 AcpuUpLimit;  /* A�˸����ϵ���ֵ��Ĭ��80 ����������Ҫ��Ƶ���ϵ��� */
    u32 AcpuDownLimit;  /* A�˸����µ���ֵ��Ĭ��55 ����������Ҫ��Ƶ���µ��� */
    u32 AcpuUpNum;  /* A���������ڸ������޴�����Ĭ��1����������Ҫ��Ƶ���ϵ��� */
    u32 AcpuDownNum;  /* A���������ڸ������޴�����Ĭ��3����������Ҫ��Ƶ���µ��� */
    u32 DFSTimerLen;  /* ��ʱ��ѯ����ʱ�䣬Ĭ��400 ��tick��1tick=10ms��*/
    u32 DFSHifiLoad; /* ���� */
    /*
     * Dfsʹ�����ֲ��ԣ�����Phone��̬ʹ�á�
     * bit0:1ʹ��200ms��Ƶ�����ԣ�bit0:0ʹ��4s��Ƶ�����ԣ�
     * bit1:0���ر�DDR��Ƶ��1����DDR��Ƶ��
     */
    u32 Strategy;
    u32 DFSDdrUpLimit;  /* DDR��Ƶ�������ޣ�����Phone��̬ʹ�á� */
    u32 DFSDdrDownLimit;  /* DDR��Ƶ�������ޣ�����Phone��̬ʹ�á� */
    u32 DFSDdrprofile;  /* DDR��Ƶ���profileֵ������Phone��̬ʹ�á� */
    u32 reserved;  /* ������չNV�������ڱ�ʾ˯�߻��Ѻ�����õ�profileֵ������MBB��ز�Ʒʹ�á� */
}ST_PWC_DFS_STRU;

/*ID=0xd111 begin */
typedef struct
{
    /*
     * Dumpģʽ��
     * 00: excdump��
     * 01: usbdump��
     * 1x: no dump��
     */
    u32 dump_switch    : 2;
    u32 ARMexc         : 1; /* ARM�쳣��⿪�ء� */
    u32 stackFlow      : 1; /* ��ջ�����⿪�ء� */
    u32 taskSwitch     : 1; /* �����л���¼���ء� */
    u32 intSwitch      : 1; /* �жϼ�¼���ء� */
    u32 intLock        : 1; /* ���жϼ�¼���ء� */
    u32 appRegSave1    : 1; /* ACORE�Ĵ�����1��¼���ء� */
    u32 appRegSave2    : 1; /* ACORE�Ĵ�����2��¼���ء� */
    u32 appRegSave3    : 1; /* ACORE�Ĵ�����3��¼���ء� */
    u32 commRegSave1   : 1; /* CCORE�Ĵ�����1��¼���ء� */
    u32 commRegSave2   : 1; /* CCORE�Ĵ�����2��¼���ء� */
    u32 commRegSave3   : 1; /* CCORE�Ĵ�����3��¼���ء� */
    u32 sysErrReboot   : 1; /* SystemError��λ���ء� */
    u32 reset_log      : 1; /* ǿ�Ƽ�¼���أ���δʹ�á� */
    u32 fetal_err      : 1; /* ǿ�Ƽ�¼���أ���δʹ�á� */
    u32 log_ctrl       : 2; /* log���ء� */
    u32 dumpTextClip   : 1; /* ddr����ʱtext�βü�����*/
	u32 secDump        : 1;
    u32 mccnt          : 1;
    u32 pccnt          : 1;
    u32 rrt            : 1;
    u32 reserved1      : 9;  /* ������ */
} DUMP_CFG_STRU;

/*
 * �ṹ��    : NV_DCXO_C1C2_CAL_RESULT_STRU
 * �ṹ˵��  : NVID DCXO C1C2У׼���
 */
typedef struct
{
    u16 shwC2fix;  /* �ֵ�����ֵ����λ0-15��Ч��Լ3ppm/���� */
    u16 shwC1fix;  /* ϸ������ֵ����λ0-255��Ч��Լ0.3ppm/���� */
}NV_FAC_DCXO_C1C2_CAL_RESULT_STRU;

/* �ṹ˵��  : �������ƿ�ά�ɲ⹦�ܡ� */
typedef struct
{

    u32 thread0_channel    : 1;
    u32 thread1_channel         : 1;
    u32 thread2_channel      : 1;
    u32 thread3_channel     : 1;
    u32 thread4_channel      : 1;
    u32 thread5_channel        : 1;
    u32 thread6_channel    : 1;
    u32 thread7_channel    : 1;
    u32 thread8_channel    : 1;
    u32 thread9_channel   : 1;
    u32 thread10_channel   : 1;
    u32 thread11_channel   : 1;
    u32 thread12_channel   : 1;
    u32 thread13_channel      : 1;
    u32 thread14_channel      : 1;
    u32 thread15_channel       : 1;
    u32 reserved               : 16;  /* ������ */

} DUMP_SCHDULE_CTRL_STRU;


/* �ṹ˵��  : �������ƿ�ά�ɲ⹦�ܡ� */
typedef struct
{
    union
    {
        u32         uintValue;
        DUMP_CFG_STRU   Bits;
    } dump_cfg;
    union
    {
        u32         thread_cfg;
        DUMP_SCHDULE_CTRL_STRU   thread_bits;
    } schudule_cfg;

    u32 appRegAddr1;    /* ACORE����Ĵ������ַ1*/
    u32 appRegSize1;    /* ACORE����Ĵ����鳤��1*/
    u32 appRegAddr2;    /* ACORE����Ĵ������ַ2*/
    u32 appRegSize2;    /* ACORE����Ĵ����鳤��2*/
    u32 appRegAddr3;    /* ACORE����Ĵ������ַ3*/
    u32 appRegSize3;    /* ACORE����Ĵ����鳤��3*/

    u32 commRegAddr1;   /* CCORE����Ĵ������ַ1*/
    u32 commRegSize1;   /* CCORE����Ĵ����鳤��1*/
    u32 commRegAddr2;   /* CCORE����Ĵ������ַ2*/
    u32 commRegSize2;   /* CCORE����Ĵ����鳤��2*/
    u32 commRegAddr3;   /* CCORE����Ĵ������ַ3*/
    u32 commRegSize3;   /* CCORE����Ĵ����鳤��3*/

    /*
     * Trace�ɼ����ء�
     * 1:��������Trace��
     * ��1:����������Trace��
     */
    u32 traceOnstartFlag;
    /*
     * Trace�ɼ����á�
     * bit1���ɼ�app�ˣ�
     * bit2���ɼ�modem�ˡ�
     */
    u32 traceCoreSet;
    /*
     * �����߹��Ĺ��ܿ��ء�
     * 0�����������������߹������ܣ�
     * ��0���������������߹������ܡ�
     */
    u32 busErrFlagSet;
} NV_DUMP_STRU;
/*ID=0xd111 end */

/*NV ID = 0xd115 start*/
/* �ṹ˵��  : ����Ӳ���汾�š� */
typedef struct {
    u32 index;           /*Ӳ���汾����ֵ(��汾��1+��汾��2)�����ֲ�ͬ��Ʒ*/
    u32 hwIdSub;        /*Ӳ���Ӱ汾�ţ����ֲ�Ʒ�Ĳ�ͬ�İ汾*/
    char  name[32];           /*�ڲ���Ʒ��*/
    char    namePlus[32];       /*�ڲ���Ʒ��PLUS*/
    char    hwVer[32];          /*Ӳ���汾����*/
    char    dloadId[32];        /*������ʹ�õ�����*/
    char    productId[32];      /*�ⲿ��Ʒ��*/
}PRODUCT_INFO_NV_STRU;

/* product support module nv define */
/* �ṹ˵��  : ������ʶ��Ʒ���Ӳ��ģ���Ƿ�֧�֣�1��֧�֣�0��֧�֡�
 * ����Ʒ��̬�����ڸò�Ʒ��NV DIFF�ļ��У�����Ӳ��ID���������Ƿ�֧��ĳ��ģ�飬Ҳ�ɾݴ���չ��Ӧ��Ʒ��̬���е�����ģ�顣
 */
typedef struct
{
    /*
     * �Ƿ�֧��SD����
     * 1��֧�֣�0����֧�֡�
     */
    u32 sdcard      : 1;
    u32 charge      : 1;  /* �Ƿ�֧�ֳ�硣 */
    u32 wifi        : 1;  /* �Ƿ�֧��Wi-Fi�� */
    u32 oled        : 1;  /* �Ƿ�֧��LCD��OLED������ */
    u32 hifi        : 1;  /* �Ƿ�֧��Hi-Fi����ģ�顣 */
    u32 onoff       : 1;  /* �Ƿ�֧�ֿ��ػ������� */
    u32 hsic        : 1;  /* �Ƿ�HSIC�ӿڡ� */
    u32 localflash  : 1;  /* �Ƿ�֧�ֱ���Flash�洢�� */
    u32 reserved    : 24;  /* ����bitλ������չʹ�á� */
} DRV_MODULE_SUPPORT_STRU;

/*�ṹ˵�������ؼƵ�ѹ������У׼����*/
typedef struct
{
    u32 v_offset_a;         /* ��ѹУ׼���Բ��� */
    s32 v_offset_b;         /* ��ѹУ׼����ƫ��*/
    u32 c_offset_a;         /* ����У׼���Բ��� */
    s32 c_offset_b;         /* ����У׼����ƫ�� */
}COUL_CALI_NV_TYPE;

/* E5���ڸ��� */
/* �ṹ˵��  : ���˴��ںŷ��䣬���⴮�ں�����AT�ڿ��ر�־�� */
typedef struct
{
    u32 wait_usr_sele_uart : 1;  /* [bit 0-0]1: await user's command for a moment; 0: do not wait */
    u32 a_core_uart_num    : 2;  /* �����A�˵������ںš� */
    u32 c_core_uart_num    : 2;  /* �����C�˵������ںš� */
    u32 m_core_uart_num    : 2;  /* �����M�˵������ںš� */
    /*
     * A-Shell���ر�־��
     * 0���أ�1������
     */
    u32 a_shell            : 1;
    /*
     * C-Shell���ر�־��
     * 0���أ�1������
     */
    u32 c_shell            : 1;
    /*
     * ����AT�ڿ��ر�־��
     * 0���أ�1������
     */
    u32 uart_at            : 1;
    /*
     * �����Ƿ����cshell��log��A�������ڡ�
     * 0���ر�cshell��auart��·����
     * 1����cshell��auart·����
     */
    u32 extendedbits       : 22;
}DRV_UART_SHELL_FLAG;

/*
 * �ṹ˵��  : ���Ź���ʼ������NV��
 * ��NV��B5000ƽ̨�޸�Ϊ�ɶ������ԣ��ն˰����޸Ķ��ƣ�
 */
typedef struct
{
    /*
     * ����WDT�Ƿ�򿪡�
     * 1���򿪣�0���رա�
     */
    u32 wdt_enable;
    u32 wdt_timeout;  /* WDT��ʱʱ�䡣Ĭ��30s�� */
    u32 wdt_keepalive_ctime;  /* WDTι��ʱ������Ĭ��15s�� */
    u32 wdt_suspend_timerout;  /* Suspend��WDT��ʱʱ��Ĭ��150s�� */
    u32 wdt_reserve;  /* WDT����� */
}DRV_WDT_INIT_PARA_STRU;

/* pm om���� NV�� = 0xd145 */
/*
 * �ṹ˵��  : 1�����ÿ��Ƶ͹��Ŀ�ά�ɲ�ʹ�õ�memory���ͣ�
 * 2���͹��Ŀ�ά�ɲ⹦���Ƿ���
 * 3�����ø����˵�log�����ļ���¼�ķ�ֵ��
 */
typedef struct
{
    /*
     * ���ƻ�����ʹ�ù����ڴ滹���������͵�memory��
     * 0��ʹ��Ԥ�ȷ����256KB����DDR��
     * 1������������ά�ɲ��ڴ棨��socpʹ�õ�DDR�ռ䣩��
     */
    u32 mem_ctrl:1;
    u32 reserved:31;      /* ����bitλ:���� */
    u8  log_threshold[4]; /* �����˵�log�����ļ���¼�ķ�ֵ����������index��Ӧ�����˵�ipc��ţ���ֵ�԰ٷֱȱ�ʾ�� */
    /*
     * ���Ƹ�ģ���Ƿ����͹���log��¼���ܣ�ÿ��bit��ʾ��ģ��״̬��
     * 0���ر�
     * 1����
     */
    u8  mod_sw[8];
}DRV_PM_OM_CFG_STRU;

/* watchpoint = 0xd148 begin */
typedef struct
{
    u32 enable;     /* ʹ�ܱ�־��0��ȥʹ�ܣ�1��ʹ�ܣ� ����ñ�־Ϊȥʹ�ܣ����ý�ֱ�Ӻ��� */
    u32 type;       /* �������: 1������2��д��3����д */
    u32 start_addr; /* �����ʼ��ַ */
    u32 end_addr;   /* ��ؽ�����ַ */
}WATCHPOINT_CFG;
typedef struct
{
    u32 enable;     /* ʹ�ܱ�־��0��ȥʹ�ܣ�1��ʹ�ܣ� ����ñ�־Ϊȥʹ�ܣ����ý�ֱ�Ӻ��� */
    u32 type;       /* �������: 1������2��д��3����д */
    u32 start_addr_low; /* �����ʼ��ַ��32bit */
    u32 start_addr_high; /* �����ʼ��ַ��32bit */
    u32 end_addr_low;   /* ��ؽ�����ַ��32bit */
    u32 end_addr_high; /* ��ؽ�����ַ��32bit */
}WATCHPOINT_CFG64;
/* �ṹ˵��������watchpoint������NV���ֹ�޸� */
typedef struct
{
    WATCHPOINT_CFG64  ap_cfg[4];  /* A�����ã����֧��4��watchpoint */
    WATCHPOINT_CFG  cp_cfg[4];  /* C�����ã����֧��4��watchpoint */
}DRV_WATCHPOINT_CFG_STRU;
/* watchpoint = 0xd148 end */

/* 0xD194, for tsensor start*/
struct DRV_TSENSOR_NV_TEMP_UNIT {
    s32 high_temp;
    s32 low_temp;
};

struct DRV_TSENSOR_NV_TEMP_INFO {
    u32 temp_unit_nums;                                     /*�����¶���ֵʵ�ʵ�Ԫ��*/
    struct DRV_TSENSOR_NV_TEMP_UNIT temp_threshold_init[5]; /*���֧��5����¶���ֵ*/
};
/* �ṹ˵�����±�����NV���� */
typedef struct
{
    u32 enable_reset_hreshold;                                         /*ʹ�ܸ�λ���ޱ�־,0,ȥʹ�ܣ�1,ʹ��*/
    int reset_threshold_temp;                                          /*��λ�¶���ֵ*/
    u32 timer_value_s;                                                 /*��ʱ����ʱֵ*/
    u32 ltemp_out_time;                                                /*�����±��˳�ʱ��*/
    u32 enable_print_temp;                                             /*ʹ��10s��ӡһ���¶�*/
    u32 enable_htemp_protect;                                          /*�����±�ʹ�ܱ�־, 0, ȥʹ��;  1, ʹ��*/
    struct DRV_TSENSOR_NV_TEMP_INFO htemp_threshold;   /*�����±���ֵ*/
    u32 enable_ltemp_protect;                                          /*�����±�ʹ�ܱ�־, 0, ȥʹ��;  1, ʹ��*/
    struct DRV_TSENSOR_NV_TEMP_INFO ltemp_threshold;   /*�����±���ֵ*/
}DRV_TSENSOR_TEMP_PROTECT_CONTROL_NV_STRU;
/* 0xD194, for tsensor end*/

/* 0xd19a, for pcie speed ctrl  start*/
/* �ṹ˵����PCIE���ٺ͵͹��Ŀ��Ƶ�NV */
typedef struct
{
    u32 pcie_speed_ctrl_flag;    /*PCIE �ٶȿ��Ʊ�־,0,ȥʹ�ܣ�1,ʹ��*/
    u32 pcie_pm_ctrl_flag;       /*PCIE �͹��Ŀ��Ʊ�־,0,ȥʹ�ܣ�1,ʹ��*/
    u32 pcie_init_speed_mode;    /*��ʼ��ʱ��PCIE �ٶ�*/
    u32 pcie_pm_speed_mode;      /*�͹��Ļָ�ʱPCIE���õ��ٶ�*/
    u32 pcie_speed_work_delay;   /* ������PCIE �ٶȵ���ʱwork ����ʱʱ��ʱ����ms */

    u32 gen1_volume;    /*PCIE GEN1ģʽ��������ֵ��С*/
    u32 gen2_volume;    /*PCIE GEN2ģʽ��������ֵ��С*/
    u32 gen3_volume;    /*PCIE GEN3ģʽ��������ֵ��С*/
    u32 pcie_l1ss_enable_flag;          /*PCIE L1SS Enable ,  0,ȥʹ�ܣ�1,ʹ��*/

}DRV_PCIE_SPEED_CTRL_STRU;
/* 0xd19a, for pcie speed ctrl  end*/

/* �ṹ˵���� ���Ƹ���uart���������Ƿ�򿪣����ݶ˿�ת��ashell��cshell�����߲���ʹ�� */
typedef struct
{
    /*
     * ���Ƹ���uart���������Ƿ�򿪡�
     * 0���رգ�
     * 1���򿪡�
     */
    u32 DialupEnableCFG;
    /*
    * ��NV������711 hsuart����ҵ��NAS���������д�뵽NV���У���λ��
    * hsuart����ȡNVֵ����hsuart�˿��л�����Ӧ�˿ڡ�
    * 0��hsuartΪ�����˿ڣ�
    * 0x5A5A5A5A��hsuartΪAShell�˿ڣ�
    * 0xA5A5A5A5��hsuartΪCShell�˿ڡ�
    */
    u32 DialupACShellCFG;
}DRV_DIALUP_HSUART_STRU;

/* TEST support module nv define */
/* �ṹ˵��  : ��NVʵ�ֿ��Ƹ���ģ���Ƿ�򿪵Ĺ��ܣ�1���򿪣�0���رա� */
typedef struct
{
    u32 lcd         : 1;  /* �Ƿ�֧��lcdģ�飬1: support; 0: not support */
    u32 emi         : 1;  /* �Ƿ�֧��emiģ�� */
    u32 pwm         : 1; /* �Ƿ�֧��pwmģ�� */
    u32 i2c         : 1; /* �Ƿ�֧��i2cģ�� */
    u32 leds        : 1;  /* �Ƿ�֧��ledsģ�� */
    u32 reserved    : 27;
} DRV_MODULE_TEST_STRU;

/* �ṹ˵��  : NV�Թ��� */
typedef struct
{
    /*
     * ��NV���ݳ��ִ���ʱ�Ĵ���ʽ��
     * 1������ģʽ��
     * 2���û�ģʽ��
     */
    u32 ulResumeMode;
}NV_SELF_CTRL_STRU;

/* 0xd158 */
/* �ṹ˵��  : ����pmu�ӿ�������ssi�ӿڻ���spmi�ӿڡ� */
typedef struct
{
    u32 pmu_enable_cfg;     /* pmu �򿪹رտ���NV�� */
    /*
     * pmu ssi spmiЭ��ѡ��
     * 0��ssi�ӿڣ�1��spmi�ӿ�
     */
    u32 protocol_sel;
    u32 reserved;  /* ������ */
}DRV_PMU_CFG_STRU;


/* 0xd159 */
/* �ṹ˵��  : ��NVʵ����modem �����׶����� serdes ʱ�ӵĲ����������������ܡ� */
typedef struct
{
    /*
     * Bit 7 ���λ 0��ͨ��nv ���� serdes ʱ�Ӳ��� mask�����Ҫ�޸� ���Σ���Ҫͬʱ�޸� mask Ϊ 1��
     * Bit 0 ���λ 0��serdes����Ϊ������1�� serdes����Ϊ���Ҳ�
     */
    u8 clk_type;
    /*
     * Bit 7 ���λ 0��ͨ��nv ���� serdes ʱ���������� mask�����Ҫ�޸� ���Σ���Ҫͬʱ�޸� mask 1��
     * Bit 0-bit6 ����serdes ʱ������������ֵ
     */
    u8 serdes_clk_drv;
    u8 reserved1;
    u8 reserved2;
}DRV_PMU_CLK_STRU;

/* 0xd183 */
typedef struct
{
	u32 sim_volt_flag;
	u32 reserved;
}DRV_NV_PMU_TYPE;

/* 0xd168 */
/* �ṹ˵��  : ��C10/C20  ������˫��coresight���ܵ�ʹ����ETB���ݱ����Ƿ�ʹ�ܡ� */
typedef struct
{
    u32 ap_enable:1;        /* Ap coresightʹ�ܿ��� 1��ʹ�ܣ�0����ʹ�� */
    u32 cp_enable:1;        /* CP coersightʹ�ܿ���1��ʹ�ܣ�0����ʹ�� */
    u32 ap_store :1;        /*AP coresight store flag*/
    u32 cp_store :1;        /* Cp etb���ݱ������1��ʹ�ܣ�0����ʹ�� */
    u32 reserved :28;
}DRV_CORESIGHT_CFG_STRU;

/* �ṹ˵��  : M3�Ͽ��Ƶ͹��ĵ�NV�� */
typedef struct
{
    /*
     * M3�Ƿ��Normal wfi��
     * 0��ʾ������
     * 1��ʾ����
     */
    u8 normalwfi_flag;
    /*
     * M3�Ƿ����˯�ı�ǡ�
     * 0��ʾ������
     * 1��ʾ����
     */
    u8 deepsleep_flag;
    /*
     * M3 �Ƿ���Buck3��������̡�
     * 0��ʾBuck3�����磻
     * 1��ʾBuck3���硣
     */
    u8 buck3off_flag;
    /*
     * M3 ���Buck3������Ļ��������Ƿ���硣
     * 0��ʾ���費���磻
     * 1��ʾ������磨ע�⣬���Buck3����Ļ�����NV���ò������ã�Buck3���磬����һ�����磩��
     */
    u8 peridown_flag;
    u32 deepsleep_Tth;  /* C����˯ʱ����ֵ�� */
    u32 TLbbp_Tth;  /* C����˯��BBP�Ĵ�������ֵ�� */
}DRV_NV_PM_TYPE;

/* �ṹ˵��  : ��NVʵ�ֶ�R8 mem retention���ԵĿ��ؿ��ơ� */
typedef struct
{
    u32 lr_retention_enable;  /* 4G R8 MEM retention���ؿ��� */
    u32 nr_retention_enable;  /* 5G  R8 MEM retention���ؿ��� */
    u32 l2hac_retention_enable;  /* L2HAC  R8 MEM retention���ؿ���,��ǰ��δ���ã�HAC mem��NR ��ʹ��nr_retention_enableͳһ���� */
	u32 default_mode;  /* Nvδ������£�Ĭ��ģʽ�����Ϊ1����Ĭ��Ϊshut downģʽ�����Ϊ1����Ĭ��Ϊretentionģʽ */
    u32 reserved1;
}ST_PWC_RETENTION_STRU;

/* �ṹ˵��  : ��NVʵ�ֶԵ͹���ά��debug���ԵĿ��ؿ��ơ� */
typedef struct
{
    u32 lpmcu_lockirq_syserr:1; /* if lockirq time length greater than threshold,this cfg decide whether system error*/
    u32 lrcpu_pminfo_report2hids:1; /*if set,report lr pm info to hids,include dpm,pm,wakelock,rsracc etc*/
    u32 nrcpu_pminfo_report2hids:1; /*if set,report nr pm info to hids,include dpm,pm,wakelock,rsracc etc*/
    u32 l2hac_pminfo_report2hids:1; /*if set,report l2hac pm info to hids,include dpm,pm,wakelock,rsracc etc*/
    u32 rsv:28;
    u32 lpmcu_irq_monitor_threshold; /*record lpmcu lockirq time length threshold,slice number*/
    u32 pm_stay_wake_time_threshold; /* ccpu���ֻ���ʱ����ֵ����������ֵ����hids��ӡά����Ϣ */
    u32 ccpu_sleep_check_time; /*M3 check if response ccpu pd irq late,unit is slice*/
    u32 nrwakeup_time_threshold; /*M3 check if nrwakeup time over threshold,unit is slice*/
    u32 lrwakeup_time_threshold; /*M3 check if lrwakeup time over threshold,unit is slice*/
    u32 rsv5;
    u32 rsv6;
    u32 rsv7;
    u32 rsv8;
}ST_PMOM_DEBUG_STRU;

/* �ṹ˵��  : ��NVʵ�ֿ���HKADCУ׼���԰汾�� */
typedef struct
{
    u8 version; /* ���汾��Ϊ1ʱ��hkadcʹ���µ�У׼���� */
    u8 rsv1;
    u8 rsv2;
    u8 rsv3;
}DRV_HKADC_CAL_VERSION;


/* �ṹ˵��  : ����ָʾά�⣨DIAG��ʹ�õ�Ĭ��ͨ�����͡��� */
typedef struct
{
    /*
     * 0��USBͨ��
     * 1��VCOM
     */
    u32 enPortNum;
}DIAG_CHANNLE_PORT_CFG_STRU;
/* �ṹ˵����deflate ���Կ��� */
typedef struct {
u32 deflate_enable:1;      /*0 deflate���Թر� 1 deflate���Դ�*/
u32 reservd:31;
}DRV_DEFLATE_CFG_STRU;

/* �ṹ˵������������ */
typedef struct {
    u32 iqi_enable:1;      /*0 iqi���Թر� 1 iqi���Դ�*/
    u32 serial_enable:1;   /*serial ���� 1�� 0 �ر�*/
    //u32 debug_enable:1;    /*debug����ʹ��*/
    u32 reservd:30;
}DRV_IQI_CFG_STRU;

/*
 * �ṹ˵��  : ��NVʵ�ֶԸ�������п���log֪ͨ�͸��������log�������õĹ���
 */
typedef struct
{
    /*
     * 0���رտ���LOG���ԣ�
     * 1���򿪿���LOG���ԡ�
     */
    u8  cMasterSwitch;
    /*
     * 0������LOG�ڴ治���ã�
     * 1������LOG�ڴ���á�
     */
    u8  cBufUsable;
    /*
     * 0������LOG�ڴ治ʹ�ܣ�
     * 1������LOG�ڴ�ʹ�ܡ�
     */
    u8  cBufEnable;
    /*
     * 0������LOG�ر�
     * 1�����򼶱�LOG��
     * 2����������LOG
     * 3��ȫLOG
     */
    u8  cswACPUBsp;
    /*
     * 0������LOG�ر�
     * 1�����򼶱�LOG��
     * 2����������LOG
     * 3��ȫLOG
     */
    u8  cswACPUDiag;
    /*
     * 0������LOG�ر�
     * 1�����򼶱�LOG��
     * 2����������LOG
     * 3��ȫLOG
     */
    u8  cswACPUHifi;
    /*
     * 0������LOG�ر�
     * 1�����򼶱�LOG��
     * 2����������LOG
     * 3��ȫLOG
     */
    u8  cswLRMBsp;
    /*
     * 0������LOG�ر�
     * 1�����򼶱�LOG��
     * 2����������LOG
     * 3��ȫLOG
     */
    u8  cswLRMDiag;
    /*
     * 0������LOG�ر�
     * 1�����򼶱�LOG��
     * 2����������LOG
     * 3��ȫLOG
     */
    u8  cswLRMTLPhy;
    /*
     * 0������LOG�ر�
     * 1�����򼶱�LOG��
     * 2����������LOG
     * 3��ȫLOG
     */
    u8  cswLRMGUPhy;
    u8  cswLRMCPhy;     /* Range:[0,3] 4G Modem CPHY����log profile */
    /*
     * 0������LOG�ر�
     * 1�����򼶱�LOG��
     * 2����������LOG
     * 3��ȫLOG
     */
    u8  cswLRMEasyRf;
    /*
     * 0������LOG�ر�
     * 1�����򼶱�LOG��
     * 2����������LOG
     * 3��ȫLOG
     */
    u8  cswNRMBsp;
    /*
     * 0������LOG�ر�
     * 1�����򼶱�LOG��
     * 2����������LOG
     * 3��ȫLOG
     */
    u8  cswNRMDiag;
    /*
     * 0������LOG�ر�
     * 1�����򼶱�LOG��
     * 2����������LOG
     * 3��ȫLOG
     */
    u8  cswNRMHAC;
    /*
     * 0������LOG�ر�
     * 1�����򼶱�LOG��
     * 2����������LOG
     * 3��ȫLOG
     */
    u8  cswNRMPhy;
    /*
     * 0������LOG�ر�
     * 1�����򼶱�LOG��
     * 2����������LOG
     * 3��ȫLOG
     */
    u8  cswNRMHL1C;
    /*
     * 0������LOG�ر�
     * 1�����򼶱�LOG��
     * 2����������LOG
     * 3��ȫLOG
     */
    u8  cswNRMPDE;
    u8  cReserved[6];   /* �����ֶ� */
}DRV_NV_POWER_ON_LOG_SWITCH_STRU;


typedef struct
{
    s32 temperature;
    u16 code;
    u16 reserved;
} DRV_CONVERT_TABLE_TYPE;
#define XO_TABLE_SIZE 166
/* �ṹ˵�����ڲ�ʹ�� */
typedef struct
{
    DRV_CONVERT_TABLE_TYPE convert_table[XO_TABLE_SIZE];
} DRV_XO_CONVERT_TABLE;
#define PA_TABLE_SIZE 32
/* �ṹ˵�����ڲ�ʹ�� */
typedef struct
{
    DRV_CONVERT_TABLE_TYPE convert_table[PA_TABLE_SIZE];
} DRV_PA_CONVERT_TABLE;


#ifndef LPHY_UT_MODE//lint !e553
typedef struct convert_table
{
    s32 temperature;
    u16 code;
    u16 reserved;
} convert_table;
#define XO_TBL_SIZE 166
typedef struct xo_convert_table_array
{
    convert_table convert_table[XO_TBL_SIZE];
} xo_convert_table_array;
#define PA_TBL_SIZE 32
typedef struct pa_convert_table_array
{
    convert_table convert_table[PA_TBL_SIZE];
} pa_convert_table_array;
#endif
/*NVID = 50019 for HiBurn tool update*/
typedef struct
{
    /*
    0 : fastbootֱ��������kernel
    1 : ����ͣ����fastboot
    */
    u8 update_mode_flag;
    u8 reserved1;
    u8 reserved2;
    u8 reserved3;
}NV_HIBURN_CONFIG_STRU;

/* �ṹ˵������ NV ʵ�ֿ��� digital power monitor ������Ϲ��ܿ��ض��� */
typedef struct
{
    /* ������Ͽ���,0�ر�,1����,Ĭ�Ϲر� */
    u32 enable;
    /* ������ϼĴ�����ȡ����,��λ��ms,Ĭ����1000 */
    u32 task_delay_ms;
    u32 reserved1;
    u32 reserved2;
}DRV_POWER_MONITOR_CFG;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
