/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
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

#ifndef __NV_STRU_PAM_H__
#define __NV_STRU_PAM_H__

#include "vos.h"
#include "nv_id_pam.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push, 4)

#define ESN_NV_DATA_LEN (8)
#define MEID_NV_DATA_LEN (8)

/*
 * ����˵��: ����˿ں�ö��ֵ
 */
enum AT_PHY_Port {
    AT_UART_PORT = 0,
    AT_PCUI_PORT,
    AT_CTRL_PORT,
    AT_HSUART_PORT,
    AT_PCUI2_PORT,
    AT_PORT_BUTT
};
typedef VOS_UINT32 AT_PHY_PortUint32;

enum {
    CPM_IND_PORT = AT_PORT_BUTT, /* OM�����ϱ��˿� */
    CPM_CFG_PORT,                /* OM���ö˿� */
    CPM_SD_PORT,
    CPM_WIFI_OM_IND_PORT, /* WIFI��OM�����ϱ��˿� */
    CPM_WIFI_OM_CFG_PORT, /* WIFI��OM�����·��˿� */
    CPM_WIFI_AT_PORT,     /* WIFI��AT�˿� */
    CPM_HSIC_IND_PORT,
    CPM_HSIC_CFG_PORT,
    CPM_VCOM_IND_PORT, /* VCOM��OM�����ϱ��ӿ� */
    CPM_VCOM_CFG_PORT, /* VCOM��OM���ýӿ� */
    CPM_FS_PORT,
    CPM_PORT_BUTT
};
typedef VOS_UINT32 CPM_PHY_PortUint32;


enum {
    CPM_CBT_PORT_USB,
    CPM_CBT_PORT_VCOM,
    CPM_CBT_PORT_SOCKET,
    CBP_CBT_PORT_BUTT
};
typedef VOS_UINT32 CPM_CBT_PortUint32;

enum {
    CPM_OM_PORT_TYPE_USB,
    CPM_OM_PORT_TYPE_VCOM,
    CPM_OM_PORT_TYPE_WIFI,
    CPM_OM_PORT_TYPE_SD,
    CPM_OM_PORT_TYPE_FS,
    CPM_OM_PORT_TYPE_HSIC,
    CBP_OM_PORT_TYPE_BUTT
};
typedef VOS_UINT32 CPM_OM_PortUint32;


enum ESN_MEID_Flag {
    ESN_MEID_DISABLE     = 0,
    ESN_ME_ENABLE_FLAG   = 1,
    MEID_ME_ENABLE_FLAG  = 2,
    ESN_MEID_ENABLE_BOTH = 3,
    ESN_MEID_ENABLE_FLAG_BUTT
};
typedef VOS_UINT8 ESN_MEID_FlagUint8;


typedef struct {
    VOS_UINT8  tempOverCount;      /* �����澯������������ */
    VOS_UINT8  tempRsumeCount;     /* �ڵ͹���ģʽ��澯ģʽ���¶��������ڸ澯�ŵļ��޴��� */
    VOS_UINT8  tempCloseCount;     /* ������ػ������������� */
    VOS_UINT8  tempPowerOffCount; /* �����µ������������� */
    VOS_UINT32 waitTimer;         /* �ȴ���ʱ��ʱ�� */
} TEMP_PARA_Ctrl;


typedef struct {
    /*
     * 0�����±����رգ�
     * 1�����±����򿪡�
     */
    VOS_UINT32 isEnable;
    /*
     * �¶ȼ�������ID����ʾʹ��������¶���Ϊ�±������롣���뽫��ֵ��Ϊ��������ײ�������¶Ȼ�ȡ�ӿ��У���ȡ�¶ȣ���Ϊ�¶ȱ������о����ݡ�
     * HKADC_TEMP_BATTERY = 0
     * HKADC_TEMP_PA0 = 1
     * HKADC_TEMP_PA1 = 2
     * HKIADC_TEMP_SIM_CARD = 6
     * ֵ6��ʾ����SIM���¶����±���ֵ0��ʾ���õ���¶����±���ֵ1��2��ʾ����PA0��PA1�¶����±������ݲ�Ʒ��ƿ���ֻ��һ��PA����
     */
    VOS_UINT32 location;
    VOS_INT32  powerOffThreshold;    /* �����µ����ޡ�����¶ȼ���ΪSIM�����أ���λΪ1�棻����¶ȼ���ΪPA����λΪ0.1�档�˲�������E5 ��Ч���������Ҫ����������뽫���ֵ����Ϊ����� */
    VOS_INT32  closeAdcThreshold;    /* ������ػ����ޡ�����¶ȼ���ΪSIM�����أ���λΪ1�棻����¶ȼ���ΪPA����λΪ0.1�档�������Ҫ����������뽫���ֵ����Ϊ����� */
    VOS_INT32  alarmAdcThreshold;    /* ���¸澯���ޡ�����¶ȼ���ΪSIM�����أ���λΪ1�棻����¶ȼ���ΪPA����λΪ0.1�档�������Ҫ����������뽫���ֵ����Ϊ����� */
    VOS_INT32  resumeAdcThreshold;   /* �ָ����ޡ�����¶ȼ���ΪSIM�����أ���λΪ1�棻����¶ȼ���ΪPA����λΪ0.1�档 */
    VOS_INT32  sIMPowerOffThreshold; /* E5��̬�£�����SIM �����±���������ֵ����λΪ1�档 */
    TEMP_PARA_Ctrl tempCtrlPara;     /* ÿ���������� */
} SPY_TempProtectNv;


typedef struct {
    /*
     * OMʹ�õ�ͨ������ǰ��USB��VCOM��Ч��
     * 0��USB
     * 1��VCOM
     * 2��WIFI
     * 3��SD��
     * 4���ļ�ϵͳ
     * 5��HSIC
     */
    CPM_OM_PortUint32   portNum;
    /*
     * У׼ʹ�õ�ͨ����
     * 0��USB
     * 1��VCOM
     * 2��WIFI,wifi ������Ϊ��ȫԭ��Ĭ�ϲ����룬��Ҫ�����Ժ����±���汾
     */
    CPM_CBT_PortUint32  cbtPortNum;
} OM_ChannlePortCfg;


enum USIMM_NvCardapp {
    USIMM_NV_GUTL_APP     = 0x00000000,
    USIMM_NV_IMS_APP      = 0x00000001,
    USIMM_NV_CDMA_APP     = 0x00000002,
    USIMM_NV_CARDAPP_BUTT = 0x00000003
};
typedef VOS_UINT32 USIMM_NvCardappUint32;


typedef struct {
    VOS_UINT8                    appNum;  /* ��Ҫ��ʼ����Ӧ�ø�����Ŀǰ���ֵΪ3 */
    VOS_UINT8                    rsv[3];  /* Ԥ���ֶ� */
    USIMM_NvCardappUint32        appList[5];  /* ��ʼ��Ӧ���б���ʼ�����ȼ��Ӹߵ��� */
} USIMM_AppPriorityConfig;


typedef struct {
    /*
     * 0�������滻����
     * ����0x5a5a��IMEI_0�滻IMEI_2����
     */
    VOS_UINT16 imei0ReplaseImei2;
    VOS_UINT16 rsv;  /* Ԥ�� */
} NV_SC_PersCtrl;


typedef struct {
    /*
     * 0���ر�USIM����ģʽ��
     * 1������USIM����ģʽ��
     */
    VOS_UINT16 flag;
    VOS_UINT16 timerValue;  /* USIMģ���ʼ���ӳ�ʱ�䣬�Ѳ�ʹ�á� */
} USIMM_CmdDebugMode;


typedef struct {
    /*
     * ESN��MEID�Ƿ���Ч��ǣ�
     * 0��ESN��MEID����Ч��
     * 1����ESN��Ч��
     * 2����MEID��Ч��
     * 3��ESN��MEID����Ч��
     */
    ESN_MEID_FlagUint8       esnMeIDFlag;
    VOS_UINT8                rsv;  /* Ԥ���ֽ� */
    VOS_UINT8                esn[ESN_NV_DATA_LEN];   /* ESN���ݣ���һ���ֽڱ�ʾ������Ϣ�� */
    VOS_UINT8                meId[MEID_NV_DATA_LEN]; /* MEID���ݣ���һ���ֽڱ�ʾ������Ϣ�� */
    VOS_UINT8                rfu[6];  /* Ԥ���ֽ� */
} NV_ESN_Meid;


typedef VOS_UINT32 SI_PB_StorateTypeUint32;
/* �ṹ˵��  : USIM���绰��������Ϣ�� */
typedef struct {
    /*
     * 0��δ�޶����ͣ�
     * 1��SM��
     * 2��ME��
     * 3��SM��ME���ɣ�
     * 4��ON��
     * 5��FD��
     * 6��EN��
     * 7��BD��
     * �˲���Ŀǰ��ʹ�á�
     */
    SI_PB_StorateTypeUint32 curMaterial;
    VOS_UINT8               passwords[8];  /* �洢�����룬Ŀǰ��ʹ�á� */
    VOS_UINT16              used;  /* ME�洢����ʹ�ü�¼����Ŀǰ��ʹ�á� */
    VOS_UINT16              total;  /* ME�洢�����֧�ּ�¼����Ŀǰ��ʹ�á� */
    VOS_UINT8               anrMaxNum; /* ��չ�绰��֧�ֵ�ANR�ļ��������������֧��3������С֧��1���� */
    /*
     * ��չ�绰���Ƿ�֧��EMAIL�ļ���
     * 0����֧�֣�
     * 1��֧�֡�
     */
    VOS_UINT8               emailFlag;
    /*
     * �Ƿ�����չ�绰�����ܡ�
     * 0����������
     * 1��������
     */
    VOS_UINT8               spbFlag;
    VOS_UINT8               rsv;  /* ������ */
} SI_PB_NVCtrlInfo;


typedef struct {
    VOS_UINT8 profileLen;  /* ����ָʾ�� */
    VOS_UINT8 profile[47];  /* profile���ݡ� */
} USIMM_UsimProfile;

typedef struct {
    VOS_UINT8 profileLen;  /* ����ָʾ�� */
    VOS_UINT8 profile[31];  /* profile���ݡ� */
} USIMM_SimProfile;

/* �ṹ˵��  : ��������STK������ز����� */
typedef struct {
    VOS_UINT8               funcEnable;  /* STK�Ƿ񼤻 */
    VOS_UINT8               timer;  /* Ĭ�ϵ�STK��ʱʱ�䡣 */
    VOS_UINT8               terminalType;  /* �ն����ͣ���δʹ�á� */
    VOS_UINT8               rsv;  /* Ԥ���ֽڣ������á� */
    USIMM_UsimProfile       usimProfile;  /* USIM Profile������Ϣ */
    USIMM_SimProfile        simProfile;  /* SIM Profile������Ϣ */
} USIMM_STK_CFG_STRU;

typedef USIMM_STK_CFG_STRU USIMM_StkCfg;


typedef struct {
    /*
     * bit[0-2] : ֧��SCI�ĸ�����
     * bit[3-7] : ������
     * bit[8-10] : modem 0ʹ�õ�SCI�ӿڣ�0��SCI0 1��SCI1��2��SCI2������ֵ:�࿨���ݲ�֧�� ��
     * bit[11-13] : modem 0ʹ�õ�SCI�ӿڣ�0��SCI0 1��SCI1��2��SCI2������ֵ:�࿨���ݲ�֧�� ��
     * bit[14-16] : modem 0ʹ�õ�SCI�ӿڣ�0��SCI0 1��SCI1��2��SCI2������ֵ:�࿨���ݲ�֧�� ��
     * bit[17-31] : ���� ��
     */
    VOS_UINT32 value;
} NV_SCI_Cfg;

enum {
    STK_CMD_NONEED_REPORT_APP = 0,
    STK_CMD_NEED_REPORT_APP = 1,
    STK_CMD_REPORT_APP_BUTT
};

/*
 * ����˵��: NV_STK_CmdReportAppCfg�ṹ
 * �ṹ˵��: ���ƿ��ϱ������������Ƿ���Ҫ֪ͨ��APP
 */
typedef struct {
    VOS_UINT32 value; /* 0:Ĭ��ֵ������Ҫ�ϱ�APP��1:��Ҫ�ϱ�APP */
} NV_STK_CmdReportAppCfg;

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
