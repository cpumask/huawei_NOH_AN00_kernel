/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
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
#ifndef __GEN_MSG_H__
#define __GEN_MSG_H__

#include "ApsL4aInterface.h"
#include "at_common.h"
#include "vos.h"
#include "mdrv.h"
#include "app_rrc_interface.h"
#include "app_esm_interface.h"
#include "app_lmm_interface.h"

#include "app_rrc_lte_common_interface.h"
#include "app_l2_interface.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#pragma pack(push, 4)

#define MSP_UPGRADE_DLOAD_VER_LEN 30
#define MSP_UPGRADE_DLOAD_SOFTWARE_LEN 30
#define MSP_UPGRADE_DLOAD_ISO_LEN 100
#define MSP_UPGRADE_DLOAD_PRODUCTID_LEN 30

#define L4A_MAX_IPV6_PREFIX_NUM 6 /* ���IPv6��ַǰ׺���� */

/*
 * MSP �ڲ�����Ϣ����
 */

#if (VOS_OS_VER == VOS_WIN32)
#pragma warning(disable : 4200) /* disable nonstandard extension used : zero-sized array in struct/union. */
#pragma warning(disable : 4214) /* allows bitfield structure members to be of any integral type. */
#endif

/*
 * MUX��ؽӿ�
 * (0x00010000,0x00010050]
 */
#define ID_MSG_DIAG_CMD_REQ 0x00010001
#define ID_MSG_DIAG_CMD_CNF 0x00010002

/*
 * ATģ����Ϣ�ӿ�
 * [0x00010051,0x00010100)
 */

/*
 * DIAG ��Ϣ�ӿ�
 * [0x00010101,0x00010120)
 */
#define ID_MSG_DIAG_DRA_DSP_REQ 0x00010101
#define ID_MSG_DIAG_DRA_DSP_CNF 0x00010102 /* HAL_DRA --> DIAG */
#define ID_MSG_DIAG_TIMER_DSPTIMOUT_IND 0x00010103

#define ID_MSG_DIAG_SIMMTRANS_REQ 0x00010107
#define ID_MSG_DIAG_SIMMTRANS_CNF 0x00010108
#define ID_MSG_DIAG_SIMMTRANS_IND 0x00010109
#define ID_MSG_DIAG_AT_HSOADDR_IND 0x0001010A /* DIAG֪ͨAT������Ϣ */
#define ID_MSG_DIAG_AT_HSO_AT_REQ 0x0001010B  /* DIAG-->AT */
#define ID_MSG_AT_DIAG_HSO_AT_CNF 0x0001010C  /* AT  -->DIAG */
#define ID_MSG_AT_DIAG_HSO_AT_IND 0x0001010D  /* AT  -->DIAG */
#define ID_MSG_A_DIAG_HSO_CONN_IND 0x0001010E /* A DIAG  --> M DIAG */

/*
 * ���POMģ�����Ϣ����
 * [0x00010121,0x00010140)
 */
#define ID_MSG_SYM_SYSMODE_IND 0x00010121
#define ID_MSG_SYM_SET_TMODE_REQ 0x00010122
#define ID_MSG_SYM_SET_TMODE_CNF 0x00010123
#define ID_MSG_SUM_SOFTUPDATE_IND 0x00010128
#define ID_MSG_SUM_SOFTBACK_REQ 0x00010129
#define ID_MSG_SUM_SOFTBACK_CNF 0x0001012A
#define ID_MSG_SYM_GET_FTP_FILE_IND 0x0001012B
#define ID_MSG_SYM_SET_HPM_REQ 0x0001012C
#define ID_MSG_SYM_SET_HPM_CNF 0x0001012D
#define ID_MSG_SYM_RD_HPM_REQ 0x0001012E
#define ID_MSG_SYM_RD_HPM_CNF 0x0001012F
#define ID_MSG_SYM_ATF_REQ 0x00010130
#define ID_MSG_SYM_ATF_CNF 0x00010131
#define ID_MSG_SYM_QUERY_CARDLOCK_REQ 0x00010132
#define ID_MSG_SYM_QUERY_CARDLOCK_CNF 0x00010133

/*
 * ���FTMģ�����Ϣ����
 * [0x00010140,0x00010201)
 */
#define ID_MSG_FTM_SET_TXON_REQ 0x00010140
#define ID_MSG_FTM_SET_TXON_CNF 0x00010141
#define ID_MSG_FTM_RD_TXON_REQ 0x00010142
#define ID_MSG_FTM_RD_TXON_CNF 0x00010143
#define ID_MSG_FTM_SET_FCHAN_REQ 0x00010144
#define ID_MSG_FTM_SET_FCHAN_CNF 0x00010145
#define ID_MSG_FTM_RD_FCHAN_REQ 0x00010146
#define ID_MSG_FTM_RD_FCHAN_CNF 0x00010147
#define ID_MSG_FTM_SET_RXON_REQ 0x0001014e
#define ID_MSG_FTM_SET_RXON_CNF 0x0001014f
#define ID_MSG_FTM_RD_RXON_REQ 0x00010150
#define ID_MSG_FTM_RD_RXON_CNF 0x00010151
#define ID_MSG_FTM_SET_FWAVE_REQ 0x00010152
#define ID_MSG_FTM_SET_FWAVE_CNF 0x00010153
#define ID_MSG_FTM_SET_AAGC_REQ 0x00010156
#define ID_MSG_FTM_SET_AAGC_CNF 0x00010157
#define ID_MSG_FTM_RD_AAGC_REQ 0x00010158
#define ID_MSG_FTM_RD_AAGC_CNF 0x00010159
#define ID_MSG_FTM_FRSSI_REQ 0x0001015a
#define ID_MSG_FTM_FRSSI_CNF 0x0001015b
#define ID_MSG_FTM_OPENCHAN_REQ 0x0001015c
#define ID_MSG_FTM_OPENCHAN_CNF 0x0001015d

#define ID_MSG_FTM_TIME_OUT 0x0001015e

#define ID_MSG_FTM_PHY_POWER_REQ 0x0001015f
#define ID_MSG_FTM_PHY_POWER_CNF 0x00010160

#define ID_MSG_FTM_RD_FAGC_REQ 0x00010163
#define ID_MSG_FTM_RD_FAGC_CNF 0x00010164

/* �������۲�start */
#define ID_MSG_FTM_SET_SRXPOW_REQ 0x000101d3
#define ID_MSG_FTM_SET_SRXPOW_CNF 0x000101d4
#define ID_MSG_FTM_RD_SRXPOW_REQ 0x000101d5
#define ID_MSG_FTM_RD_SRXPOW_CNF 0x000101d6

#define ID_MSG_FTM_SET_SRXBLER_REQ 0x000101db
#define ID_MSG_FTM_SET_SRXBLER_CNF 0x000101dc
#define ID_MSG_FTM_TX_CLT_INFO_IND 0x000101dd

/* �������۲�end */

#define ID_MSG_FTM_SET_GODLOAD_REQ 0x000101f9
#define ID_MSG_FTM_SET_GODLOAD_CNF 0x000101fa

#define ID_MSG_FTM_SET_RESET_REQ 0x000101fb
#define ID_MSG_FTM_SET_RESET_CNF 0x000101fc

#define ID_MSG_FTM_SET_SDLOAD_REQ 0x000101ff
#define ID_MSG_FTM_SET_SDLOAD_CNF 0x00010200

#define ID_MSG_L1A_CT_IND 0x00010450

/* װ�� */
#define ID_MSG_FTM_SET_TSELRF_REQ 0x00010950
#define ID_MSG_FTM_SET_TSELRF_CNF 0x00010951
/* װ�� */
#define ID_MSG_FTM_SET_TBAT_REQ 0x0001095c
#define ID_MSG_FTM_SET_TBAT_CNF 0x0001095d

#define ID_MSG_FTM_RD_TBAT_REQ 0x0001095e
#define ID_MSG_FTM_RD_TBAT_CNF 0x0001095f
#define ID_MSG_FTM_TMODE_TO_SECONDARY 0x00010964
#define ID_MSG_FTM_TMODE_TO_SECONDARY_CNF 0x00010965

#define ID_MSG_FTM_FCHAN_TO_DSP_MAIN 0x00010966
#define ID_MSG_FTM_FCHAN_TO_DSP_SECONDARY 0x00010968
#define ID_MSG_FTM_POLLTEMPR_REQ 0x00010969

/* notice: lte msg id range from 0x00011000 to 0x00011FFF */

#define ID_MSG_TDS_SET_TSELRF_REQ 0x0001097B
#define ID_MSG_TDS_SET_TSELRF_CNF 0x0001097C
#define ID_MSG_TDS_RD_TSELRF_REQ 0x0001097D
#define ID_MSG_TDS_RD_TSELRF_CNF 0x0001097E
#define ID_MSG_TDS_SET_FCHAN_REQ 0x0001097F
#define ID_MSG_TDS_SET_FCHAN_CNF 0x00010980
#define ID_MSG_TDS_RD_FCHAN_REQ 0x00010981
#define ID_MSG_TDS_RD_FCHAN_CNF 0x00010982
#define ID_MSG_TDS_SET_FWAVE_REQ 0x00010983
#define ID_MSG_TDS_SET_FWAVE_CNF 0x00010984

#define ID_MSG_TDS_SET_FTXON_REQ 0x00010985
#define ID_MSG_TDS_SET_FTXON_CNF 0x00010986
#define ID_MSG_TDS_RD_FTXON_REQ 0x00010987
#define ID_MSG_TDS_RD_FTXON_CNF 0x00010988
#define ID_MSG_TDS_SET_FRXON_REQ 0x00010989
#define ID_MSG_TDS_SET_FRXON_CNF 0x0001098A
#define ID_MSG_TDS_RD_FRXON_REQ 0x0001098B
#define ID_MSG_TDS_RD_FRXON_CNF 0x0001098C
#define ID_MSG_TDS_RD_FRSSI_REQ 0x0001098D
#define ID_MSG_TDS_RD_FRSSI_CNF 0x0001098E

#define ID_MSG_TDS_REQ 0x00010991
#define ID_MSG_TDS_CNF 0x00010992

#define ID_MSG_FTM_SET_LTCOMMCMD_REQ 0x000109BD
#define ID_MSG_FTM_SET_LTCOMMCMD_CNF 0x000109BE
#define ID_MSG_FTM_RD_LTCOMMCMD_REQ 0x000109BF
#define ID_MSG_FTM_RD_LTCOMMCMD_CNF 0x000109C0

#define ID_MSG_FTM_RD_FPLLSTATUS_REQ 0x000109c1
#define ID_MSG_FTM_RD_FPLLSTATUS_CNF 0x000109c2

#define ID_MSG_L4A_MODE_IND 0x00011001
#define ID_MSG_L4A_READ_CEREG_CNF 0x00011002
#define ID_MSG_L4A_CGANS_IND 0x00011003

#define ID_MSG_DIAG_PSTRANS_REQ 0x00011004
#define ID_MSG_DIAG_PSTRANS_CNF 0x00011005
#define ID_MSG_DIAG_PSTRANS_IND 0x00011006

#define ID_MSG_L4A_CSQ_INFO_REQ 0x00011007
#define ID_MSG_L4A_CSQ_INFO_CNF 0x00011008
#define ID_MSG_L4A_RSSI_IND 0x00011009

#define ID_MSG_L4A_ANLEVEL_IND 0x0001100A
#define ID_MSG_L4A_ANQUERY_INFO_REQ 0x0001100B
#define ID_MSG_L4A_ANQUERY_INFO_CNF 0x0001100C

#define ID_MSG_L4A_LWCLASHQRY_REQ 0x0001100D
#define ID_MSG_L4A_LWCLASHQRY_CNF 0x0001100E
#define ID_MSG_L4A_LWCLASH_IND 0x0001100F

#define ID_MSG_L4A_CERSSI_REQ 0x00011010
#define ID_MSG_L4A_CERSSI_CNF 0x00011011
#define ID_MSG_L4A_CERSSI_IND 0x00011012

#define ID_MSG_L4A_CELL_ID_REQ 0x00011015
#define ID_MSG_L4A_CELL_ID_CNF 0x00011016

#define ID_MSG_L4A_LTE_TO_IDLE_REQ 0x00011017
#define ID_MSG_L4A_LTE_TO_IDLE_CNF 0x00011018

#define ID_MSG_L4A_LW_THRESHOLD_REQ 0x00011019
#define ID_MSG_L4A_LW_THRESHOLD_CNF 0x0001101A

#define ID_MSG_L4A_FAST_DORM_REQ 0x0001101B
#define ID_MSG_L4A_FAST_DORM_CNF 0x0001101C

#define ID_MSG_L4A_CELL_INFO_REQ 0x00011020
#define ID_MSG_L4A_CELL_INFO_CNF 0x00011021
#define ID_MSG_L4A_CELL_INFO_IND 0x00011022

#define ID_MSG_L4A_LTE_CS_REQ 0x00011023
#define ID_MSG_L4A_LTE_CS_CNF 0x00011024

#define ID_MSG_L4A_IND_CFG_REQ 0x00011025

#define ID_MSG_L4A_CERSSI_INQ_REQ 0x00011026
#define ID_MSG_L4A_CERSSI_INQ_CNF 0x00011027

#define ID_MSG_L4A_ISMCOEXSET_REQ 0x00011028
#define ID_MSG_L4A_ISMCOEXSET_CNF 0x00011029

#define ID_MSG_L4A_RADVER_SET_REQ 0x00011030
#define ID_MSG_L4A_RADVER_SET_CNF 0x00011031

#define ID_MSG_L4A_LCACELLQRY_REQ 0x00011032
#define ID_MSG_L4A_LCACELLQRY_CNF 0x00011033
#define ID_MSG_L4A_LCACELL_IND 0x00011034

#define ID_MSG_L4A_LCASCELLINFO_QRY_REQ 0x00011035
#define ID_MSG_L4A_LCASCELLINFO_QRY_CNF 0x00011036

#define ID_MSG_TDS_DIAG_REQ (0x00020001)
#define ID_MSG_TDS_DIAG_CNF (0x00020002)

/*
 * ���L1A��Ϣ�ӿڵ���ϢID���� [0x00010450,0x00010500)
 */

#define RSSI_POW_MAX_NUM 30
#define CT_F_FREQ_LIST_MAX_NUM 32

/*
 * MSP �ڲ���Ϣ��������
 */

/*
 * MUX��ؽӿ�
 * (0x00010000,0x00010050]
 */

/*
 * MSMģ�����Ϣ��������
 */

typedef struct {
    VOS_UINT16 index;
    VOS_UINT8  content[8];
} WIWEP_SUPPORT;

typedef struct {
    VOS_UINT32 bandValue;
    VOS_UINT8  bandStr[4];
} CLIP_SupportBands;

/* **************************************************************************** */
/* DLOADVER */
typedef struct {
    VOS_UINT32 reserved;
} FTM_RdDloadverReq;

typedef struct {
    VOS_UINT32 errCode;
    /* VOS_UINT32 ulLength; ������Ҳ���Ժ����� */
    VOS_CHAR ver[MSP_UPGRADE_DLOAD_VER_LEN + 1];
    VOS_CHAR padding;

} FTM_RdDloadverCnf;

/* DLOADINFO */
typedef struct {
    VOS_UINT32 reserved; /* �����ֶ� */
} FTM_RdDloadinfoReq;

/* DLOADINFO */
typedef struct {
    VOS_UINT32 reserved; /* �����ֶ� */
} FTM_RdSdloadReq;

typedef struct {
    VOS_UINT32 errCode;
    VOS_UINT32 retValue;
} FTM_SetSdloadCnf;

typedef struct {
    VOS_UINT32 errCode;
    /* VOS_UINT32 ulLength; ������Ҳ���Ժ����� */
    VOS_CHAR szSendSw[MSP_UPGRADE_DLOAD_SOFTWARE_LEN + 1];
    VOS_CHAR padding;
    VOS_CHAR szSendIso[MSP_UPGRADE_DLOAD_ISO_LEN + 1];
    VOS_CHAR padding1[3];
    VOS_CHAR szSendProductId[MSP_UPGRADE_DLOAD_PRODUCTID_LEN + 1];
    VOS_CHAR padding2;
    VOS_CHAR szSendDloadType[4];

} FTM_RdDloadinfoCnf;

/* AUTHORITYVER */
typedef struct {
    VOS_UINT32 reserved; /* �����ֶ� */
} FTM_RdAuthorityverReq;

typedef struct {
    VOS_UINT32 errCode;
    /* VOS_UINT32 ulLength; ������Ҳ���Ժ����� */
    VOS_CHAR szAuthorityVer[MSP_UPGRADE_DLOAD_VER_LEN + 1];
    VOS_CHAR padding;

} FTM_RdAuthorityverCnf;

/* AUTHORITYID */
typedef struct {
    VOS_UINT32 reserved; /* �����ֶ� */
} FTM_RdAuthorityidReq;

typedef struct {
    VOS_UINT32 errCode;
    /* VOS_UINT32 ulLength;������Ҳ���Ժ����� */
    VOS_CHAR szAuthorityId[MSP_UPGRADE_DLOAD_VER_LEN + 1];
    VOS_CHAR padding0;
    VOS_CHAR szAuthorityType[2];
    VOS_CHAR padding[2];

} FTM_RdAuthorityidCnf;

/* BOOTROMVER */
typedef struct {
    VOS_UINT32 reserved; /* �����ֶ� */
} FTM_RdBootromverReq;

typedef struct {
    VOS_UINT32 errCode;
    /* VOS_UINT32 ulLength; ������Ҳ���Ժ����� */
    VOS_CHAR szSendStr[MSP_UPGRADE_DLOAD_VER_LEN + 1];
    VOS_CHAR padding;

} FTM_RdBootromverCnf;

/* NVBACKUP */
typedef struct {
    VOS_UINT32 reserved; /* �����ֶ� */
} FTM_SetNvbackupReq;

typedef struct {
    VOS_UINT32 errCode;
    VOS_UINT32 retValue;

} FTM_SetNvbackupCnf;

/* NVRESTORE */
typedef struct {
    VOS_UINT32 reserved; /* �����ֶ� */
} FTM_SetNvrestoreReq;

typedef struct {
    VOS_UINT32 errCode;
    VOS_UINT32 retValue;

} FTM_SetNvrestoreCnf;

/* GODLOAD */
typedef struct {
    VOS_UINT32 nvBackupFlag;
} FTM_SetGodloadReq;

typedef struct {
    VOS_UINT32 errCode;

} FTM_SetGodloadCnf;

/* RESET */
typedef struct {
    VOS_UINT32 reserved; /* �����ֶ� */
} FTM_SetResetReq;

typedef struct {
    VOS_UINT32 errCode;

} FTM_SetResetCnf;

/* NVRSTSTTS */
typedef struct {
    VOS_UINT32 reserved; /* �����ֶ� */
} FTM_SetNvrststtsReq;

typedef struct {
    VOS_UINT32 errCode;
    VOS_CHAR   szNvRst[4];

} FTM_SetNvrststtsCnf;

/*
 * ����: ּ�ڸ���MUX DIAG �Ѿ������ð����� TO DO ...
 * ID: ID_MSG_SYM_SYSMODE_IND
 * REQ: NA
 * CNF: NA
 * IND: SYM_SysmodeInd
 * ˵��:
 */
typedef enum _POM_SYSMODE {
    EN_POM_SYSMODE_NORMAL = 0,
    EN_POM_SYSMODE_FTM,
    EN_POM_SYSMODE_ETM,
    EN_POM_SYSMODE_UNKNOWN
} SYM_SYSMODE;

typedef struct {
    SYM_SYSMODE sysMode;
} SYM_SysmodeInd;

/*
 * ����: ����TMODE�л�ʱ��ؽӿ� ^TMODE����У׼��SYM����
 *       SYM����ݲ������û��߲�����DSPУ׼
 * ID: ID_MSG_SYM_SET_TMODE_REQ,ID_MSG_SYM_SET_TMODE_CNF
 * REQ: SYM_SET_TmodeReq,SYM_RD_TmodeReq
 * CNF: SYM_SET_TmodeCnf,SYM_RD_TMODE_CNF_STRU
 * IND: NA
 * ˵��:
 */
enum {
    EN_SYM_TMODE_SIGNAL = 0,
    EN_SYM_TMODE_NONESIGNAL,
    EN_SYM_TMODE_LOAD,
    EN_SYM_TMODE_RESET,
    EN_SYM_TMODE_SHUTDOWN,
    EN_SYM_TMODE_SIGNAL_NOCARD = 11,

    EN_SYM_TMODE_OFFLINE      = 12,
    EN_SYM_TMODE_F_NONESIGNAL = 13, /* TMODE=6��Ϊ13 */

    EN_SYM_TMODE_SYN_NONESIGNAL = 14, /* LTE �������۲� */
    EN_SYM_TMODE_SET_SECONDARY  = 15,
    EN_SYM_TMODE_GU_BT          = 16,
    EN_SYM_TMODE_TDS_FAST_CT    = 17,
    EN_SYM_TMODE_TDS_BT         = 18,
    EN_SYM_TMODE_COMM_CT        = 19,
    EN_SYM_TMODE_UNKNOWN        = 0xffffffff
};
typedef VOS_UINT32 SYM_TMODE_ENUM;

enum {
    EN_SYM_TMODE_STA_DOING = 0,
    EN_SYM_TMODE_STA_OK,
    EN_SYM_TMODE_STA_ERR,
    EN_SYM_TMODE_STA_UNKNOWN = 0xffffffff
};
typedef VOS_UINT32 SYM_TMODE_STA_ENUM;

/* ���ýӿ� */
typedef struct {
    VOS_UINT8 wcdmaIsSet; /* 1,IsSet; 0,notSet; */
    VOS_UINT8 cdmaIsSet;
    VOS_UINT8 tdscdmaIsSet;
    VOS_UINT8 gsmIsSet;
    VOS_UINT8 edgeIsSet;
    VOS_UINT8 awsIsSet;
    VOS_UINT8 fddlteIsSet;
    VOS_UINT8 tddlteIsSet;
    VOS_UINT8 wifiIsSet;
    VOS_UINT8 rsv1;
    VOS_UINT8 rsv2;
    VOS_UINT8 rsv3;
} AT_TmodeRatFlag;

/* AT^PLATFORM? */
typedef struct {
    VOS_UINT16 provider;
    VOS_UINT16 subPlatForm;
} AT_PLATFORM_Nv;

/* ���ýӿ� */
typedef struct {
    /* �Ƿ���ӿ�����Ϣ���� TODO: ... (���ķ�/00149739/2010-05-07) */
    SYM_TMODE_ENUM tmodeMode;
} SYM_SET_TmodeReq;

typedef struct {
    /* �Ƿ���ӿ�����Ϣ���� TODO: ... (���ķ�/00149739/2010-05-07) */
    SYM_TMODE_ENUM     tmodeMode;   /* ����ģʽ */
    SYM_TMODE_STA_ENUM symTmodeSta; /* ����״̬ */

    /* ERR_SUCCESS(0):�ɹ��� */
    /* 1:�رշ�������� */
    /* 2:����ģʽ���� */
    /* 3:û����������ŵ� */
    /* 4:�򿪷����ʧ�� */
    /* 5:�������� */
    VOS_UINT32 errCode; /* ERR_SUCCESS:�ɹ���OTHER:ʧ�� */
} SYM_SET_TmodeCnf;

/* ��ѯ�ӿ� */
typedef struct {
    VOS_UINT32 reserved; /* �����ֶ� */
} SYM_RD_TmodeReq;

typedef struct {
    VOS_UINT32 tmode;
    VOS_UINT32 cnfRet;
} AT_TmodeResult;

typedef SYM_SET_TmodeCnf SYM_RD_TMODE_CNF_STRU;

/*
 * ���� : ^FAGC ��ȡRSRP�ӿ�
 * ID   : ID_MSG_FTM_RD_FAGC_REQ,ID_MSG_FTM_RD_FAGC_CNF
 * REQ  : FTM_RdFagcReq
 * CNF  : FTM_RdFagcCnf
 * IND  : NA
 * ˵�� :
 */

/* ��ȡ�ӿ� */
typedef struct {
    VOS_UINT32 reserved; /* �����ֶ� */
} FTM_RdFagcReq;

typedef struct {
    VOS_INT16  rsrp; /* ����ֵ */
    VOS_UINT16 rsd;

    /* ERR_MSP_SUCCESS �ɹ� */
    /* ��ʱ */
    /* δ�� */
    VOS_UINT32 errCode;
} FTM_RdFagcCnf;

/* ���У׼�������ݽṹ */
typedef struct {
    VOS_UINT16 minValue;
    VOS_UINT16 maxValue;
} MSP_VBAT_CALIBART_TYPE;

/*
 * ����: ^F ��ؽӿ�
 * ID: ID_MSG_SYM_ATF_REQ,ID_MSG_SYM_ATF_CNF
 * REQ: SYM_AtfReq,
 * CNF: SYM_AtfCnf
 * IND: NA
 * ˵��:
 */
typedef struct {
    VOS_UINT32 fpara; /* �����ֶ� */
} SYM_AtfReq;

typedef struct {
    VOS_UINT32 errCode; /* ERR_SUCCESS:�ɹ���OTHER:ʧ�� */
} SYM_AtfCnf;

/*
 * ���� : ^FTXON ��������ģʽ�£����ô򿪷����
 * ID   : ID_MSG_FTM_SET_TXON_REQ,ID_MSG_FTM_SET_TXON_CNF
 *        ID_MSG_FTM_RD_TXON_REQ,ID_MSG_FTM_RD_TXON_CNF
 * REQ  : FTM_SetTxonReq,FTM_RdTxonReq
 * CNF  : FTM_SetTxonCnf,FTM_RdTxonCnf
 * IND  : NA
 * ˵�� :
 */

/* TXON�������� */
enum {
    /* 0:��ʾ�رշ���� */
    EN_FTM_TXON_CLOSE = 0,

    /* 1:��ʾ�򿪷�����������������ƺ�RF����ͨ�� */
    EN_FTM_TXON_UL_RFBBP_OPEN,

    /* 2:��ʾֻ��RFIC Transmitter��PA �� */
    /* BB����ͨ�����򿪣�����VCTCXOУ׼ */
    EN_FTM_TXON_RF_PA_OPEN,
    EN_FTM_TXON_UNKNOWN = 0xFFFFFFFF
};
typedef VOS_UINT32 FTM_TXON_SWT_ENUM;


enum FTM_CltEnable {
    FTM_CLT_DISABLE = 0,
    FTM_CLT_ENABLE,

    FTM_CLT_BUTT
};
typedef VOS_UINT8 FTM_CltEnableUint8;

typedef struct {
    VOS_UINT32 reserved;
} FTM_RdFpllstatusReq;

typedef struct {
    VOS_UINT16 txStatus;
    VOS_UINT16 rxStatus;
    VOS_UINT32 errCode;
} FTM_RdFpllstatusCnf;

/* SET */
typedef struct {
    FTM_TXON_SWT_ENUM  swtich;
    FTM_CltEnableUint8 cltEnableFlg;
    VOS_UINT8          reserved[3];
} FTM_SetTxonReq;

typedef struct {
    /* ERR_SUCCESS(0):�ɹ��� */
    /* 1:�رշ�������� */
    /* 2:����ģʽ���� */
    /* 3:û����������ŵ� */
    /* 4:�򿪷����ʧ�� */
    /* 5:�������� */
    VOS_UINT32 errCode;
} FTM_SetTxonCnf;

/* READ */
typedef struct {
    VOS_UINT32 reserved; /* �����ֶ� */
} FTM_RdTxonReq;

typedef struct {
    FTM_TXON_SWT_ENUM swtich;

    /* ERR_SUCCESS(0):�ɹ��� */
    /* 1:�رշ�������� */
    /* 2:����ģʽ���� */
    /* 3:û����������ŵ� */
    /* 4:�򿪷����ʧ�� */
    /* 5:�������� */
    VOS_UINT32 errCode;
} FTM_RdTxonCnf;

/*
 * ����: ^FCHAN ���÷�������ŵ�
 * ID: ID_MSG_FTM_SET_FCHAN_REQ,ID_MSG_FTM_SET_FCHAN_CNF
 *     ID_MSG_FTM_RD_FCHAN_REQ,ID_MSG_FTM_RD_FCHAN_CNF
 * REQ: FTM_SetFchanReq,FTM_SetFchanCnf
 * CNF: FTM_RdFchanReq,FTM_RdFchanCnf
 * IND: NA
 * ˵��:
 */

enum _FCHAN_MODE {
    EN_FCHAN_MODE_WCDMA = 0,
    EN_FCHAN_MODE_CDMA,
    EN_FCHAN_MODE_TD_SCDMA,
    EN_FCHAN_MODE_TD_GSM,
    EN_FCHAN_MODE_TD_EDGE,
    EN_FCHAN_MODE_TD_AWS,
    EN_FCHAN_MODE_FDD_LTE,
    EN_FCHAN_MODE_TDD_LTE,
    EN_FCHAN_MODE_TD_SELF_DIFINE,
#if (FEATURE_LTEV == FEATURE_ON)
    EN_FCHAN_MODE_LTEV = 10,
#endif
    EN_FCHAN_MODE_UNKONWN = 0xFF
};
typedef VOS_UINT32 FCHAN_MODE_ENUM;

/* ��ȷ�� */
#define FTM_AT_FCHAN_BAND 8 /* 800M */
#define FTM_AT_FCHAN_BAND_MAX 14
#define FTM_DSP_FCHAN_BAND 20 /* 800M */

/* SET */
typedef struct {
    FCHAN_MODE_ENUM fchanMode;
    VOS_UINT32      channel; /* Ƶ�� */
    VOS_UINT8       band;
    VOS_UINT8       padding;
    VOS_UINT16      listeningPathFlg;
} FTM_SetFchanReq;

typedef struct {
    /* ERR_SUCCESS(0):�ɹ��� */
    /* 1:����ģʽ���� */
    /* 2:����Ƶ����Ϣ�޷���Ӧ */
    /* 3:�����ŵ���Ϣʧ�� */
    /* 4:����Ƶ�κ��ŵ���Ϣ�޷���� */
    /* 5:�������� */
    FCHAN_MODE_ENUM fchanMode;
    VOS_UINT32      errCode;
} FTM_SetFchanCnf;

/* READ */
typedef struct {
    VOS_UINT32 reserved; /* �����ֶ� */
} FTM_RdFchanReq;

#define MSP_FTM_FREQ_VALID 1
#define MSP_FTM_FREQ_INVALID 0

typedef struct {
    FCHAN_MODE_ENUM fchanMode;
    VOS_UINT8       band;
    VOS_UINT8       padding[3]; /* PADDING */

    /* װ��AT�����¶���, ��ѯʱ���к�����Ƶ�����ö��践�� */
    VOS_UINT32 ulChannel;
    VOS_UINT32 dlChannel;

    /* ERR_SUCCESS(0):�ɹ��� */
    /* 1:����ģʽ���� */
    /* 2:����Ƶ����Ϣ�޷���Ӧ */
    /* 3:�����ŵ���Ϣʧ�� */
    /* 4:����Ƶ�κ��ŵ���Ϣ�޷���� */
    /* 5:�������� */
    VOS_UINT32 errCode;
} FTM_RdFchanCnf;

/*
 * ���� : ^FVCTCXO ����VCTCXO�ĵ�ѹֵ
 * ID   : ID_MSG_FTM_SET_FVCTCXO_REQ,ID_MSG_FTM_SET_FVCTCXO_CNF
 *        ID_MSG_FTM_RD_FVCTCXO_REQ,ID_MSG_FTM_RD_FVCTCXO_CNF
 * REQ  : FTM_SET_FVCTCXO_REQ_STRU,FTM_RD_FVCTCXO_REQ_STRU
 * CNF  : FTM_SET_FVCTCXO_CNF_STRU,FTM_RD_FVCTCXO_CNF_STRU
 * IND  : NA
 * ˵�� :
 */

#define FTM_CALPDDCS_EXECUTING 1 /* IP2У׼ִ���� */
#define FTM_CALPDDCS_EXECUTED 0  /* IP2У׼ִ����� */

typedef struct {
    VOS_UINT16 pdAutoFlg; /* ���������ŵ���Χ�0��65535 */
    VOS_UINT16 rsv;       /* �����ֶ� */
} FTM_SET_FFcalpddcsReq;
typedef struct {
    /* U32_T ulMsgID;	�ϱ���ϢID */
    VOS_UINT16 dacCfg; /* ��У׼��õ�DAC���� */
    VOS_UINT16 vgaCfg; /* ��У׼��õ�VGA���� */
    VOS_UINT16 status; /* VGAУ׼�쳣��־ */
    VOS_UINT16 rsv;
    /*
     * ERR_SUCCESS(0):�ɹ���
     * 526    ����ģʽ����(��������ģʽ)
     * 527    û����������ŵ�
     * 545    û�д򿪷����
     * 566    û�д򿪽��ջ��Ŀ���У׼
     * 501    δ֪����
     */
    VOS_UINT32 errCode;
} FTM_RD_FFcalpddcsCnf;
// SET
typedef struct {
    VOS_UINT32 pdEnbFlg; /* ָʾPD�Ƿ�ʹ�� */
    VOS_UINT16 dacCfg;
    VOS_UINT16 vgaCfg;
} FTM_SET_FFpdpowsReq;
typedef struct {
    VOS_UINT32 errCode;
} FTM_SET_FFpdpowsCnf;

typedef struct {
    VOS_UINT16 errCode; /* ״̬ */
    VOS_UINT16 rsv;
    VOS_UINT16 pddcValue[CT_F_FREQ_LIST_MAX_NUM][RSSI_POW_MAX_NUM];
} FTM_RdFqpddcresCnf;

/*
 * ���� : RXON ��������ģʽ�£����ô򿪽��ջ�
 * ID   : ID_MSG_FTM_SET_RXON_REQ,ID_MSG_FTM_SET_RXON_CNF
 *        ID_MSG_FTM_RD_RXON_REQ,ID_MSG_FTM_RD_RXON_CNF
 * REQ  : FTM_SetRxonReq,FTM_RdRxonReq
 * CNF  : FTM_SetRxonCnf,FTM_RdRxonCnf
 * IND  : NA
 * ˵�� :
 */

/* RXON�������� */
enum {
    EN_FTM_RXON_CLOSE   = 0,         /* ���ջ��ر�     */
    EN_FTM_RXON_OPEN    = 1,         /* ���ջ���     */
    EN_FTM_RXON_UNKNOWN = 0xFFFFFFFF /* ���ջ�״̬δ֪ */
};
typedef VOS_UINT32 FTM_RXON_SWT_ENUM;

/* SET */
typedef struct {
    /* 0: ��ʾ�ر���˫ͨ�� */
    /* 1: ��ʾ����˫ͨ�� */
    VOS_UINT32 rxSwt;
} FTM_SetRxonReq;

typedef struct {
    /* ERR_SUCCESS(0):�ɹ��� */
    /* 1:����ģʽ���� */
    /* 2:û����������ŵ� */
    /* 3:�򿪽��ջ�ʧ�� */
    VOS_UINT32 errCode;
} FTM_SetRxonCnf;

/* READ */
typedef struct {
    VOS_UINT32 resved;
} FTM_RdRxonReq;

typedef struct {
    /* 0: ��ʾ�ر���˫ͨ�� */
    /* 1: ��ʾ����˫ͨ�� */
    VOS_UINT32 rxSwt;

    /* ERR_SUCCESS(0):�ɹ��� */
    /* 1:����ģʽ���� */
    /* 2:û����������ŵ� */
    /* 3:�򿪽��ջ�ʧ�� */
    VOS_UINT32 errCode;
} FTM_RdRxonCnf;

/*
 * ���� : ^RFANT ����RFͨ��ѡ��
 *        ����RXͨ����RTT�յ���ԭ���ѡ����Ҫ����RSSI������ϱ��Ľ���ͨ��
 * ID   : ID_MSG_FTM_SET_RXANT_REQ,ID_MSG_FTM_SET_RXANT_CNF
 *        ID_MSG_FTM_RD_RXANT_REQ,ID_MSG_FTM_RD_RXANT_CNF
 * REQ  : FTM_SetRxantReq,FTM_RdRxantReq
 * CNF  : FTM_SetRxantCnf,FTM_RdRxantCnf
 * IND  : NA
 * ˵�� :
 */

#define FTM_RXANT_TYPE_TX 0
#define FTM_RXANT_TYPE_RX 1
#define FTM_RXANT_TYPE_UNKNOWN 0xff

#define FTM_RXANT_CLOSE_DCHAN 0
#define FTM_RXANT_OPEN_CHAN1 1
#define FTM_RXANT_OPEN_CHAN2 2
#define FTM_RXANT_OPEN_DCHAN 3
#define FTM_RXANT_UNKNOWN 0xff

/* for AT^TSELRF  ѡ����Ƶͨ·ָ�� */
#define FTM_TSELRF_TD 6

#define FTM_TSELRF_WIFI 7
#define FTM_TSELRF_FDD_LTE_MAIN 9
#define FTM_TSELRF_FDD_LTE_SUB 10
#define FTM_TSELRF_FDD_LTE_MIMO 11
#define FTM_TSELRF_TDD_LTE_MAIN 12
#define FTM_TSELRF_TDD_LTE_SUB 13
#define FTM_TSELRF_TDD_LTE_MIMO 14

/* SET */
typedef struct {
    VOS_UINT8 chanelType;

    VOS_UINT8  chanelNo;
    VOS_UINT16 reserved;
} FTM_SetRxantReq;

typedef struct {
    /* ERR_SUCCESS(0):�ɹ��� */
    /* 1:ͨ����ʧ�� */
    /* 2:ͨ���ر�ʧ�� */
    /* 3:��֧�֣����ͨ��2�ķ���ͨ����DD800��Ŀ����1T2Rģʽ�� */
    /* 4:�������� */
    VOS_UINT32 errCode;
} FTM_SetRxantCnf;

/* READ */
typedef struct {
    VOS_UINT32 reserved;
} FTM_RdRxantReq;

typedef struct {
    VOS_UINT8 chanelNo;
    VOS_UINT8 padding[3];

    /* ERR_SUCCESS(0):�ɹ��� */
    /* 1:ͨ����ʧ�� */
    /* 2:ͨ���ر�ʧ�� */
    /* 3:��֧�֣����ͨ��2�ķ���ͨ����DD800��Ŀ����1T2Rģʽ�� */
    /* 4:�������� */
    VOS_UINT32 errCode;
} FTM_RdRxantCnf;

/* **************************************************************************** */
/* AT^TSELRF */
/* **************************************************************************** */
/* SET */
typedef struct {
    VOS_UINT8 path;

    VOS_UINT8 group;

    VOS_UINT16 reserved;
} FTM_SetTselrfReq;

typedef struct {
    /* ERR_SUCCESS(0):�ɹ��� */
    /* 1:ͨ����ʧ�� */
    /* 2:ͨ���ر�ʧ�� */
    /* 3:��֧�֣����ͨ��2�ķ���ͨ����DD800��Ŀ����1T2Rģʽ�� */
    /* 4:�������� */
    VOS_UINT32 errCode;
} FTM_SetTselrfCnf;

/* READ */
typedef struct {
    VOS_UINT32 reserved;
} FTM_RdTselrfReq;

typedef struct {
    VOS_UINT8  path;
    VOS_UINT8  group;
    VOS_UINT16 rsv;

    /* ERR_SUCCESS(0):�ɹ��� */
    /* 1:ͨ����ʧ�� */
    /* 2:ͨ���ر�ʧ�� */
    /* 3:��֧�֣����ͨ��2�ķ���ͨ����DD800��Ŀ����1T2Rģʽ�� */
    /* 4:�������� */
    VOS_UINT32 errCode;
} FTM_RdTselrfCnf;

/* SET */
typedef struct {
    /* 8-74 */
    VOS_UINT8 level;
    VOS_UINT8 padding[3];
} FTM_SetFpaReq;

typedef struct {
    /* ERR_SUCCESS(0):�ɹ��� */
    /* 1:����ģʽ���� */
    /* 2:û����������ŵ� */
    /* 3:����AAGCʧ�� */
    /* 4:�������� */
    VOS_UINT32 errCode;
} FTM_SetFpaCnf;

/* SET */
typedef struct {
    /* 8-74 */
    VOS_UINT16 txControlNum;
    VOS_UINT16 padding;
} FTM_SetFdacReq;

typedef struct {
    /* ERR_SUCCESS(0):�ɹ��� */
    /* 1:����ģʽ���� */
    /* 2:û����������ŵ� */
    /* 3:����AAGCʧ�� */
    /* 4:�������� */
    VOS_UINT32 errCode;
} FTM_SetFdacCnf;

/*
 * ����: ^FWAVE �������²������ã���λ 0.01db
 * ID: ID_MSG_FTM_SET_FWAVE_REQ ID_MSG_FTM_SET_FWAVE_CNF
 * REQ: FTM_SetFwaveReq,  FTM_RdFwaveReq
 * CNF: FTM_SetFwaveCnf,  FTM_RdFwaveCnf
 * IND: NA
 * ˵��:
 */
enum _FWAVE_TYPE {
    EN_FWAVE_TYPE_CW = 0, /* ����         */
    EN_FWAVE_TYPE_WCDMA,  /*  WCDMA���Ʋ� */
    EN_FWAVE_TYPE_GSM,    /*  GSM���Ʋ�   */
    EN_FWAVE_TYPE_EDGE,   /*  EDGE���Ʋ�  */
    EN_FWAVE_TYPE_WIFI,   /*  WIFI���Ʋ�  */
    EN_FWAVE_TYPE_LTE,    /*  LTE���Ʋ�   */
    EN_FWAVE_TYPE_CDMA,   /*  CDMA���Ʋ�  */
    EN_FWAVE_TYPE_TDS,    /*  TDS���Ʋ�   */
    EN_FWAVE_TYPE_UNKONWN = 0xFFFF
};
typedef VOS_UINT32 FWAVE_TYPE_ENUM;

// SET
typedef struct {
    VOS_UINT16 type;  /* �������� */
    VOS_UINT16 power; /* ���ι���  */
} FTM_SetFwaveReq;

typedef struct {
    VOS_UINT32 errCode;
} FTM_SetFwaveCnf;

// READ
typedef struct {
    VOS_UINT32 reserved;
} FTM_RdFwaveReq;

typedef struct {
    VOS_UINT16 type;  /* �������� */
    VOS_UINT16 power; /* ���ι���  */
} FTM_RdFwaveCnf;

/*
 * ���� : ^FLNA ����RFIC��AAGC�����棬��������AGC���У׼����λΪdB��
 *        RTT�յ���ԭ���RFIC��������������Ϊԭ��ָʾ������ֵ
 * ID   : ID_MSG_FTM_SET_AAGC_REQ,ID_MSG_FTM_SET_AAGC_CNF
 *        ID_MSG_FTM_RD_AAGC_REQ,ID_MSG_FTM_RD_AAGC_CNF
 * REQ  : FTM_SetAagcReq,FTM_RdAagcReq
 * CNF  : FTM_SetAagcCnf,FTM_RdAagcCnf
 * IND  : NA
 * ˵�� :
 */

/* SET */
typedef struct {
    /* 8-74 */
    VOS_UINT8 aggcLvl;
    VOS_UINT8 padding[3];
} FTM_SetAagcReq;

typedef struct {
    /* ERR_SUCCESS(0):�ɹ��� */
    /* 1:����ģʽ���� */
    /* 2:û����������ŵ� */
    /* 3:����AAGCʧ�� */
    /* 4:�������� */
    VOS_UINT32 errCode;
} FTM_SetAagcCnf;

/* READ */
typedef struct {
    VOS_UINT32 reserved;
} FTM_RdAagcReq;

typedef struct {
    /* 8-74 */
    VOS_UINT8 aggcLvl;
    VOS_UINT8 padding[3];

    /* ERR_SUCCESS(0):�ɹ��� */
    /* 1:����ģʽ���� */
    /* 2:û����������ŵ� */
    /* 3:����AAGCʧ�� */
    /* 4:�������� */
    VOS_UINT32 errCode;
} FTM_RdAagcCnf;

/*
 * AT^TBAT
 */
typedef struct {
    VOS_UINT8  type;
    VOS_UINT8  opr;
    VOS_UINT16 valueMin;
    VOS_UINT16 valueMax;
    VOS_UINT16 rsv;

} FTM_SetTbatReq;

typedef struct {
    VOS_UINT32 errCode;
} FTM_SetTbatCnf;

typedef struct {
    VOS_UINT8  type;
    VOS_UINT8  opr;
    VOS_UINT16 value;
} FTM_RdTbatReq;

typedef struct {
    VOS_UINT8  type;
    VOS_UINT8  rsv;
    VOS_UINT16 value;
    VOS_UINT32 errCode;
} FTM_RdTbatCnf;

typedef struct {
    VOS_UINT16 min;
    VOS_UINT16 max;
} FTM_VBAT_CALIBART_TYPE;

/* ******************************************************************************** */


typedef struct {
    VOS_UINT8  ulbcs;
    VOS_UINT8  ulbcl;
    VOS_UINT16 reserved;
    VOS_UINT32 result;
} DRV_GetCbcCnf;

/*
 * ����: ^FRSSI ����RFIC��AAGC�����棬��������AGC���У׼����λΪdB��
 *       RTT�յ���ԭ���RFIC��������������Ϊԭ��ָʾ������ֵ
 * ID: ID_MSG_FTM_FRSSI_REQ,ID_MSG_FTM_FRSSI_CNF
 * REQ: FTM_FRSSI_Req
 * CNF: FTM_FRSSI_Cnf
 * IND: NA
 * ˵��:
 */
typedef struct {
    /* ֻʹ�������� */
    VOS_UINT32 reserved;
} FTM_FRSSI_Req;

typedef struct {
    /* ȫ��������ֵ, ��λ1/8dBm, ����DSP�ӿ�, Ϊ��ֵ */
    VOS_INT32 value1;
    VOS_INT32 value2;
    VOS_INT32 value3;
    VOS_INT32 value4;
    VOS_INT32 value5;
    VOS_INT32 value6;
    VOS_INT32 value7;
    VOS_INT32 value8;

    /* ERR_SUCCESS(0):�ɹ��� */
    /* 1:����ģʽ���� */
    /* 2:û����������ŵ� */
    /* 3:û�д򿪽��ջ� */
    /* 4:���ź�������ⲻ���ź� */
    /* 5:�������� */
    VOS_UINT32 errCode;
} FTM_FRSSI_Cnf;


typedef struct {
    VOS_UINT32 msgId;              /* ��ϢID */
    VOS_INT16  gammaReal;          /* ����ϵ��ʵ�� */
    VOS_INT16  gammaImag;          /* ����ϵ���鲿 */
    VOS_UINT16 gammaAmpUc0;        /* פ����ⳡ��0����ϵ������ */
    VOS_UINT16 gammaAmpUc1;        /* פ����ⳡ��1����ϵ������ */
    VOS_UINT16 gammaAmpUc2;        /* פ����ⳡ��2����ϵ������ */
    VOS_UINT16 gammaAntCoarseTune; /* �ֵ����λ�� */
    VOS_UINT32 ulwFomcoarseTune;   /* �ֵ�FOMֵ */
    VOS_UINT16 cltAlgState;        /* �ջ��㷨����״̬ */
    VOS_UINT16 cltDetectCount;     /* �ջ������ܲ��� */
    VOS_UINT16 dac0;               /* DAC0 */
    VOS_UINT16 dac1;               /* DAC1 */
    VOS_UINT16 dac2;               /* DAC2 */
    VOS_UINT16 dac3;               /* DAC3 */
} FTM_CltInfoInd;

/*
 * ���� : SYM��FTM��������Ҫ��򿪷�����ģʽ
 * ID   : ID_MSG_FTM_OPENCHAN_REQ,ID_MSG_FTM_OPENCHAN_CNF
 * REQ  : FTM_OPENCHAN_Req
 * CNF  : FTM_OPENCHAN_Cnf
 * IND  : NA
 * ˵�� :
 */
/* ����������ģʽ */

/* EN_SYM_TMODE_NONESIGNAL ��ͨУ׼ */
/* EN_SYM_TMODE_F_NONESIGNAL ����У׼ */

typedef struct {
    /* ֻʹ�������� */
    /* EN_SYM_TMODE_NONESIGNAL */
    /* EN_SYM_TMODE_F_NONESIGNAL */
    VOS_UINT8 tmodeType;

    VOS_UINT8 padding[3];
} FTM_OPENCHAN_Req;

typedef struct {
    /* ERR_SUCCESS(0):�ɹ��� */
    /* 1:����ģʽ���� */
    /* 2:û����������ŵ� */
    /* 3:û�д򿪽��ջ� */
    /* 4:���ź�������ⲻ���ź� */
    /* 5:�������� */
    VOS_UINT32 errCode;
} FTM_OPENCHAN_Cnf;

/*
 * ����: SYM��FTM��������Ҫ��򿪻��߹رչ���
 * ID: ID_MSG_FTM_PHY_POWER_REQ,ID_MSG_FTM_PHY_POWER_CNF
 * REQ: FTM_PHY_PowerReq
 * CNF: FTM_PHY_PowerCnf
 * IND: NA
 * ˵��:
 */
typedef struct {
    /* ֻʹ�������� */
    VOS_UINT32 reserved;
} FTM_PHY_PowerReq;

typedef struct {
    VOS_UINT32 errCode;
} FTM_PHY_PowerCnf;

/*
 * FTMģ�����У׼��Ϣ��������
 */

/*
 * ���� : ^BANDSW ����Band�л�
 * ID   : ID_MSG_FTM_SET_BANDSW_REQ,ID_MSG_FTM_SET_BANDSW_CNF
 *        ID_MSG_FTM_RD_BANDSW_REQ,ID_MSG_FTM_RD_BANDSW_CNF
 * REQ  : FTM_SET_BANDSW_REQ_STRU,FTM_RD_BANDSW_REQ_STRU
 * CNF  : FTM_SET_BANDSW_CNF_STRU,FTM_RD_BANDSW_CNF_STRU
 * IND  : NA
 * ˵�� :
 */

#define FTM_MAX_BAND_NUM 141
#define FTM_MAX_TDS_BAND_NUM 6

#define CT_F_TXPOW_SET_MAX_NUM 30

#define FTM_CT_F_PA_SET_MAX_NUM 30

#define FTM_CT_F_AAGC_SET_MAX_NUM 30

#define TXWAVE_TYPE_CW 0
#define TXWAVE_TYPE_LTE 1

#define START_TRIGGER_TYPE_TX 0
#define START_TRIGGER_TYPE_RX 1
#define START_TRIGGER_TYPE_TXRX 2
#define START_TRIGGER_TYPE_MAXTX 3

typedef struct {
    VOS_UINT32 rsv; /* �����ֶ� */
} FTM_RdLtescinfoReq;

/* �������۲�start */
/* ^SSYNC SET */
typedef struct {
    VOS_UINT32 status;
    VOS_UINT32 errCode;
} FTM_RdSsyncCnf;

typedef struct {
    VOS_UINT32 bandwide;
    VOS_UINT32 errCode;
} FTM_RdStxbwCnf;

typedef struct {
    VOS_UINT32 stxChannel;
    VOS_UINT32 errCode;
} FTM_RdStxchanCnf;

typedef struct {
    VOS_UINT16 subFrameAssign;
    VOS_UINT16 subFramePattern;
    VOS_UINT32 errCode;
} FTM_RdSsubframeCnf;

/* ^SPARA SET */
typedef struct {
    VOS_UINT16 type;
    VOS_UINT16 value;
} FTM_SetSparaReq;

typedef struct {
    VOS_UINT16 type;
    VOS_UINT16 value;
    VOS_UINT32 errCode;

} FTM_RdSparaCnf;

typedef struct {
    VOS_UINT32     typeNum;
    FTM_RdSparaCnf spara[32];
    VOS_UINT32     errCode;
} FTM_RdSparaCnfTotal;

typedef struct {
    VOS_UINT16 segNum;
    VOS_UINT16 reserved;
    VOS_UINT32 errCode;
} FTM_RdSsegnumCnf;

typedef struct {
    VOS_UINT16 usmodulutionNum;
    VOS_UINT16 rsv;
    VOS_UINT16 usmodulationList[256];
    VOS_UINT32 errCode;
} FTM_RdStxmodusCnf;

typedef struct {
    VOS_UINT16 usrbNum;
    VOS_UINT16 rsv;
    VOS_UINT16 usrbList[256];
    VOS_UINT32 errCode;
} FTM_RdStxrbnumsCnf;

typedef struct {
    VOS_UINT16 usrbposNum;
    VOS_UINT16 rsv;
    VOS_UINT16 usrbposList[256];
    VOS_UINT32 errCode;
} FTM_RdStxrbpossCnf;

typedef struct {
    VOS_UINT16 uspowerNum;
    VOS_UINT16 rsv;
    VOS_INT16  uspowerList[256];
    VOS_UINT32 errCode;
} FTM_RdStxpowsCnf;

typedef struct {
    VOS_UINT16 ustypeNum;
    VOS_UINT16 rsv;
    VOS_UINT16 ustypeList[256];
    VOS_UINT32 errCode;
} FTM_RdStxchantypesCnf;

typedef struct {
    VOS_UINT16 ussegmentLen;
    VOS_UINT16 reserved;
    VOS_UINT32 errCode;
} FTM_RdSseglenCnf;

typedef struct {
    VOS_UINT16 switchFlag; /* ���������в���ʱ�����з����Ƿ�� 0�رգ�1�� */
    VOS_INT16  power;      /* ���������в���ʱ�����з��书�ʣ���Χ-40~23 */
    VOS_UINT16 mod;        /* ���������в���ʱ�����з���ȡֵ��Χ 0-2 ,0:QPSK,1:16QAM,2:64QAM */
    VOS_UINT16 rbNum;      /* ���������в���ʱ�����з���RB��������Χ1-100 */
    VOS_UINT16 rbPos;      /* ���������в���ʱ�����з���RBλ�ã���Χ0-99 */
    VOS_UINT16 reserved;
    VOS_UINT32 errCode;
} FTM_RdSrxsetCnf;

/* ^SRXPOW SET */
typedef struct {
    VOS_UINT16 switchFlag;
    VOS_INT16  usulPower;
    VOS_UINT16 moduType;
    VOS_UINT16 rbNum;
    VOS_UINT16 rbPos;
    VOS_UINT16 rsv;
} FTM_SetSrxpowReq;

typedef struct {
    VOS_UINT32 errCode;
} FTM_SetSrxpowCnf;

/* ^STXCHANTYPES RD */
typedef struct {
    VOS_UINT32 reserved;
} FTM_RdSrxpowReq;

typedef struct {
    VOS_UINT16 switchFlag;
    VOS_INT16  usulPower;
    VOS_UINT32 errCode;
} FTM_RdSrxpowCnf;

typedef struct {
    VOS_UINT16 ussubFrameNum;
    VOS_UINT16 reserved;
    VOS_UINT32 errCode;
} FTM_RdSrxsubfraCnf;

/* SRXBLER */
/* ^SRXBLER SET */
typedef struct {
    VOS_UINT16 usbler;
    VOS_UINT16 reserved;
} FTM_SetSrxblerReq;

typedef struct {
    VOS_UINT32 errCode;
} FTM_SetSrxblerCnf;

typedef struct {
    VOS_UINT16 usbler[2];
    VOS_INT16  snr[2];
    VOS_UINT32 errCode;
} FTM_RdSrxblerCnf;

// TDS BER RD
typedef struct {
    VOS_UINT32 ulbersum;
    VOS_UINT32 ulberErr;
    VOS_UINT32 errCode;
} FTM_RdStrxberCnf;

typedef struct {
    VOS_UINT16 type;
    VOS_UINT16 status;
    VOS_UINT32 errCode;
} FTM_RdSstartCnf;


typedef struct {
    VOS_UINT32 msgId;
    VOS_UINT32 rserved;
} MSP_PS_AtModeReq;

typedef struct {
    VOS_UINT32 msgId;
    VOS_UINT16 cellId;
    VOS_UINT16 freq;
} MSP_PS_SetSfnSyncReq;

typedef struct {
    VOS_UINT32 msgName;
    VOS_UINT32 ulresult;
} PS_MSP_SFN_SYNC_CNF;

typedef struct {
    VOS_UINT32 msgId;
    VOS_UINT16 type;
    VOS_UINT16 rsv;
} MSP_PS_SstopReq;

typedef struct {
    VOS_UINT32 msgId;
    VOS_UINT32 transType;
    VOS_UINT16 tpcStep;
    VOS_UINT16 rsv;
} MSP_PS_DpchReq;

typedef struct {
    VOS_UINT32 msgName;
    VOS_UINT32 status;
} PS_MSP_DPCH_CNF;

/* �������۲�end */

/*
 * DIAGģ����Ϣ��������
 */

/*
 * ����: DIAGΪPS��SIMM�ṩ��͸���ӿ�
 * ID: ID_MSG_DIAG_PSTRANS_REQ,ID_MSG_DIAG_PSTRANS_CNF,ID_MSG_DIAG_PSTRANS_IND
 * REQ: DIAG_PSTRANS_Req
 * CNF: DIAG_PSTRANS_CNF_STRU
 * IND: DIAG_PSTRANS_IND_STRU
 * ˵��:
 */

typedef struct {
    VOS_UINT32 cmdId;                             /* ͸�������� */
    VOS_UINT32 paramSize;                         /* ͸����������ĳ��� */
    VOS_UINT8  param[0]; /* �������������,��Ӧ͸������Ĳ��� */  //lint !e43
} DIAG_PSTRANS_Req;

/* DIAG<-L4A,SIMM  CNF/IND ���ݽṹ */
typedef struct {
    VOS_UINT32 cmdId;                                /* ͸��������(CNF) ���� �ϱ�����ID(IND ID) */
    VOS_UINT32 paramSize;                            /* ��������ĳ��� */
    VOS_UINT8  param[0]; /* ������������� ͸���ڲ��ṹMSP����֪ */  //lint !e43
} DIAG_PSTRANS_CNF_STRU, DIAG_PSTRANS_IND_STRU;

/*
 * ���� : DIAGΪPS��SIMM�ṩ��͸���ӿ�
 * ID   : ID_MSG_DIAG_SIMMTRANS_REQ,ID_MSG_DIAG_SIMMTRANS_CNF,ID_MSG_DIAG_SIMMTRANS_IND
 * REQ  : DIAG_SIMMTRANS_REQ_STRU
 * CNF  : DIAG_SIMMTRANS_CNF_STRU
 * IND  : DIAG_SIMMTRANS_IND_STRU
 * ˵�� :
 */
typedef DIAG_PSTRANS_Req      DIAG_SIMMTRANS_REQ_STRU;
typedef DIAG_PSTRANS_CNF_STRU DIAG_SIMMTRANS_CNF_STRU;
typedef DIAG_PSTRANS_IND_STRU DIAG_SIMMTRANS_IND_STRU;

/*
 * ���L1A��Ϣ�ӿڵ���Ϣ�������� [0x00010450,0x00010500)
 */

/*
 * ����: L1A���FTM����Ϣ�ӿڣ��洢RTT���صĽṹ��
 * ID: ID_MSG_L1A_CT_IND
 * REQ: NA
 * CNF: NA
 * IND: SIMM_STATUS_IND_STRU
 * ˵��:
 */
typedef struct {
    VOS_UINT32 dspId;       /* �洢��ؽṹ�� */
    VOS_UINT8  padding[0];  //lint !e43
} L1A_CT_Ind;

#define AT_CALIB_PARAM_APCOFF 6
#define AT_CALIB_PARAM_APCOFF_MAX 8
#define AT_CALIB_PARAM_APC 160
#define AT_CALIB_PARAM_APCFREQ 96
#define AT_CALIB_PARAM_AGC 32
#define AT_CALIB_PARAM_AGCFREQ 32
#define FTM_TDS_TX_LEVEL_NUM_MAX 3     /* TDS TX��λ���� */
#define FTM_TDS_RX_LEVEL_NUM_MAX 16    /* TDS RX��λ���� */
#define FTM_TDS_TX_CAL_POWERNUM_MAX 80 /* TDS TXУ׼���ʸ��� */
#define FTM_TDS_TX_CAL_FREQNUM_MAX 16  /* TDS TXУ׼Ƶ����� */
#define FTM_TDS_RX_CAL_FREQNUM_MAX 16  /* TDS RXУ׼Ƶ����� */
#define MAX_SCALIB_DATA_LEN 320

/*
 * ö��˵��: У׼���Ʋ�������
 */
typedef enum {
    AT_CALIB_OP_TYPE_CACHE, /* "CACHE", У׼����δ�����꣬���ѽ������ݷ������ݻ��� */
    AT_CALIB_OP_TYPE_USE,   /* "USE",  ���ѻ��������д��DHI ��֪ͨDSP ������������ */
    AT_CALIB_OP_TYPE_SAVE,  /* "SAVE", ���ѻ��������д��NANDFLASH ָ��λ�ã��ͷŽ��ջ��� */
    AT_CALIB_OP_TYPE_GET,   /* "GET",  ��ѯ��ǰ�����е�ָ�����͵����ݡ�����ȡ���棩 */
    AT_CALIB_OP_TYPE_READ,  /* "READ", ��ѯNANDFLASH �е�ָ�����͵����ݡ�����ȡNANDFLASH) */
    AT_CALIB_OP_TYPE_INI,   /* "INI",  ��ʼ�������е����� */
    AT_CALIB_OP_TYPE_BEGIN, /* "BEGIN",У׼��ʼ */
    AT_CALIB_OP_TYPE_END,   /* "END",  У׼���� */
    AT_CALIB_OP_TYPE_SET,   /* "SET",  ����DSPУ׼״̬ */
    AT_CALIB_OP_TYPE_BUTT
} AT_TDS_OptType;
typedef VOS_UINT32 AT_TDS_OptTypeUint32;

/*
 * ö��˵��: У׼������������
 */
typedef enum {
    AT_CALIB_BAND_TYPE_A = 1, /* "BAND A", */
    AT_CALIB_BAND_TYPE_E = 5, /* "BAND E", */
    AT_CALIB_BAND_TYPE_F = 6, /* "BAND F", */
    AT_CALIB_BAND_TYPE_BUTT
} AT_TDS_BandType;
typedef VOS_UINT32 AT_TDS_BandTypeUint32;

/*
 * ö��˵��: У׼������������
 */
typedef enum {
    AT_CALIB_DATA_TYPE_APCOFFSETFLAG, /* "APCOFFSETFLAG", */
    AT_CALIB_DATA_TYPE_APCOFFSET,     /* "APCOFFSET", */
    AT_CALIB_DATA_TYPE_APC,           /* "APC", */
    AT_CALIB_DATA_TYPE_APCFREQ,       /* "APCFREQ", */
    AT_CALIB_DATA_TYPE_AGC,           /* "AGC", */
    AT_CALIB_DATA_TYPE_AGCFREQ,       /* "AGCFREQ", */
    AT_CALIB_DATA_TYPE_BUTT
} AT_TDS_DataType;
typedef VOS_UINT32 AT_TDS_DataTypeUint32;

/*
 * �ṹ˵��: У׼����������ݽṹ
 */
typedef struct {
    VOS_UINT16 flag1; /* ��������У׼ */
    VOS_UINT16 flag2; /* ��λ��־ */
} FTM_ScalibApcoffsetSetting;

/*
 * �ṹ˵��: У׼����������ݽṹ
 */
typedef struct {
    /* reserved for CQUPT begin */
    VOS_UINT32 boardId;
    VOS_UINT16 serial[12]; /* SN */
    VOS_UINT16 imei[5];    /* IMEI. */
    VOS_UINT16 imeiSv[5];  /* IMEISV. */
    VOS_UINT16 armFile[10];
    VOS_UINT16 zsp1File[10];
    VOS_UINT16 zsp2File[10];
    VOS_UINT16 tdRfTabFlag1; /* ��Ƶ��ı�־1 */
    VOS_UINT16 reserved;
    VOS_UINT16 tdApcTab[80]; /* ��Ƶ�Զ����ʿ��Ʊ� */
    VOS_UINT16 tdAgcTab[6];  /* ��Ƶ�Զ������ */
    VOS_UINT16 tdApcOffset[6];
    VOS_UINT16 tdApcOffsetFlag;
    VOS_UINT16 tdAfcOffset; /* AFCУ׼���� */
    VOS_UINT32 tdDcOffset;  /* ֱ��У׼���� */
    VOS_UINT16 afcAgcDcFlag;
    VOS_UINT16 reserved1[3];
    VOS_UINT16 tdRfTabFlag2; /* ��Ƶ��ı�־2 */
    VOS_UINT16 reserved2[3];
    /* reserved for CQUPT end */
    /* added at 2012-6-11 begin */
    VOS_UINT16 apcOffFlag;
    VOS_UINT16 reserved3;
    VOS_UINT16 tdsTxPaPowerBandA[FTM_TDS_TX_LEVEL_NUM_MAX];
    VOS_UINT16 reserved4;
    VOS_UINT16 tdsTxPaPowerBandE[FTM_TDS_TX_LEVEL_NUM_MAX];
    VOS_UINT16 reserved5;
    VOS_UINT16 tdsTxPaPowerBandF[FTM_TDS_TX_LEVEL_NUM_MAX];
    VOS_UINT16 reserved6;
    VOS_UINT16 tdsTxApcCompBandA[FTM_TDS_TX_CAL_POWERNUM_MAX];
    VOS_UINT16 tdsTxApcCompBandE[FTM_TDS_TX_CAL_POWERNUM_MAX];
    VOS_UINT16 tdsTxApcCompBandF[FTM_TDS_TX_CAL_POWERNUM_MAX];
    VOS_UINT16 tdsTxApcFreqCompBandA[FTM_TDS_TX_LEVEL_NUM_MAX][FTM_TDS_TX_CAL_FREQNUM_MAX];
    VOS_UINT16 tdsTxApcFreqCompBandE[FTM_TDS_TX_LEVEL_NUM_MAX][FTM_TDS_TX_CAL_FREQNUM_MAX];
    VOS_UINT16 tdsTxApcFreqCompBandF[FTM_TDS_TX_LEVEL_NUM_MAX][FTM_TDS_TX_CAL_FREQNUM_MAX];
    VOS_UINT16 tdsRxAgcCompBandA[FTM_TDS_RX_LEVEL_NUM_MAX];
    VOS_UINT16 tdsRxAgcCompBandE[FTM_TDS_RX_LEVEL_NUM_MAX];
    VOS_UINT16 tdsRxAgcCompBandF[FTM_TDS_RX_LEVEL_NUM_MAX];
    VOS_UINT16 tdsRxAgcFreqCompBandA[FTM_TDS_RX_CAL_FREQNUM_MAX];
    VOS_UINT16 tdsRxAgcFreqCompBandE[FTM_TDS_RX_CAL_FREQNUM_MAX];
    VOS_UINT16 tdsRxAgcFreqCompBandF[FTM_TDS_RX_CAL_FREQNUM_MAX];
    /* added at 2012-6-11 end */
} FTM_ScalibSystemSetting;

/* MAXPOWER NV�ṹ */
typedef struct {
    VOS_INT16 asTxMaxPower[FTM_TDS_TX_LEVEL_NUM_MAX];
    VOS_INT16 asReserve;
} TDS_NV_PaPower;

typedef struct {
    VOS_UINT16 path;  /* ��Ƶͨ·��� */
    VOS_UINT16 group; /* path�Ĳ��� */
} TDS_SET_TselrfReq;

typedef struct {
    VOS_UINT16 mode; /* ģʽ */
    VOS_UINT16 band; /* band */
    VOS_UINT16 freq; /* Ƶ�� */
    VOS_UINT16 rsv;
} TDS_SET_FchanReq;

typedef struct {
    VOS_UINT16 type; /* ��������,0:���� */
    VOS_UINT16 amp;  /* ����Ĳ��εĹ��� */
} TDS_SET_FwaveReq;

typedef struct {
    VOS_UINT16 swtich; /* ��������,0:close;1:open */
    VOS_UINT16 rsv;
} TDS_SET_FtxonReq;

typedef struct {
    VOS_UINT16 swtich; /* ��������,0:close;1:open */
    VOS_UINT16 rsv;
} TDS_SET_FrxonReq;

typedef struct {
    VOS_UINT32 rsv;
} TDS_SET_FrssiReq;

typedef struct {
    VOS_UINT32 errCode;
} TDS_SET_Cnf;

typedef struct {
    VOS_INT32  value;   /* RSSIֵ */
    VOS_UINT32 errCode; /* ������ */
} TDS_SET_FrssiCnf;

/* adde by yangzhi for */
/*
 * ����:   ��ѯTDS��RFͨ·������^TSELRF
 * ID:     ID_MSG_TDS_FRSSI_REQ,ID_MSG_TDS_FRSSI_CNF
 * �ṹ:   ID_MSG_TDS_RD_TSELRF_REQ,ID_MSG_TDS_RD_TSELRF_CNF
 */
typedef struct {
    VOS_UINT16 chanelNo; /* DSP ���ù� */
    VOS_UINT16 rsv;
    VOS_UINT32 errCode; /* ������ */
} TDS_RdTselrfCnf;

/*
 * ����:   ��ѯTDS��ͨ�����ԣ�����^FCHAN
 * ID:     ID_MSG_TDS_RD_FCHAN_REQ,ID_MSG_TDS_RD_FCHAN_CNF
 * �ṹ:
 */
typedef struct {
    FCHAN_MODE_ENUM fchanMode;
    VOS_UINT16      band;
    VOS_UINT16      ulFReqValid; /* DSP ���ù� */
    VOS_UINT16      dlFreqValid; /* DSP ���ù� */
    VOS_UINT16      ulChannel;
    VOS_UINT16      dlChannel;
    VOS_UINT16      rsv;
    VOS_UINT32      errCode; /* ������ */
} TDS_RdFchanCnf;

/*
 * ����:   ��ѯTDS�ķ����ͨ�����ԣ�����^FTXON
 * ID:     ID_MSG_TDS_RD_FTXON_REQ,ID_MSG_TDS_RD_FTXON_CNF
 * �ṹ:
 */
typedef struct {
    FTM_TXON_SWT_ENUM swtich;
    VOS_UINT32        errCode;
} TDS_RdFtxonCnf;

/*
 * ����:   ��ѯTDS�Ľ��ջ�ͨ�����ԣ�����^FRXON
 * ID:     ID_MSG_TDS_RD_FRXON_REQ,ID_MSG_TDS_RD_FRXON_CNF
 * �ṹ:
 */
typedef struct {
    VOS_UINT32 rxSwt;
    VOS_UINT32 errCode;
} TDS_RdFrxonCnf;
#define FTM_STATUS_EXECUTING 0 /* IP2У׼ִ���� */
#define FTM_STATUS_EXECUTED 1  /* IP2У׼ִ����� */

/* IP2У׼���Ƶ���� */
#define FTM_CALIIP2_MAX_CHAN_NUM 4

/* DCOCУ׼AGC��λ���� */
#define FTM_CALIDCOCS_AGC_NUM 16

/* GainState���������� */
#define FTM_GAINSTATE_MAX_NUM 60
#define FTM_GAINSTATE_MAX_NUM_HI6362 (16 * 2 * 3) /* k3v5 */

/* DBB˥������������ */
#define FTM_DBBATT_MAX_NUM FTM_GAINSTATE_MAX_NUM

/* BB˥������������ */
#define FTM_BBATT_MAX_NUM FTM_GAINSTATE_MAX_NUM

/* AT^FCALIIPS */
typedef struct {
    VOS_UINT32 rsv;
} FTM_RdNopraraReq;

typedef struct {
    VOS_UINT16 chanNum;
    VOS_UINT16 chan[FTM_CALIIP2_MAX_CHAN_NUM];
} FTM_SetFcaliip2SReq;

typedef struct {
    VOS_UINT32 errCode;
    VOS_UINT16 status;
    VOS_UINT16 chanNum;
    VOS_UINT16 chan[FTM_CALIIP2_MAX_CHAN_NUM];
    VOS_UINT16 mainIDivOff[FTM_CALIIP2_MAX_CHAN_NUM];
    VOS_UINT16 mainQDivOff[FTM_CALIIP2_MAX_CHAN_NUM];
    VOS_UINT16 mainIDivOn[FTM_CALIIP2_MAX_CHAN_NUM];
    VOS_UINT16 mainQDivOn[FTM_CALIIP2_MAX_CHAN_NUM];
    VOS_UINT16 divI[FTM_CALIIP2_MAX_CHAN_NUM];
    VOS_UINT16 divQ[FTM_CALIIP2_MAX_CHAN_NUM];
} FTM_RdFcaliip2SCnf;

/* AT^FCALIDCOCS */
typedef struct {
    VOS_UINT16 channel;
    VOS_UINT16 rsv;
} FTM_SetFcalidcocsReq;

typedef struct {
    VOS_UINT32 errCode;
    VOS_UINT16 status;
    VOS_UINT16 channel;
    VOS_UINT16 noblkAnT1I[FTM_CALIDCOCS_AGC_NUM];
    VOS_UINT16 noblkAnT1Q[FTM_CALIDCOCS_AGC_NUM];
    VOS_UINT16 blkAnT1I[FTM_CALIDCOCS_AGC_NUM];
    VOS_UINT16 blkAnT1Q[FTM_CALIDCOCS_AGC_NUM];
    VOS_UINT16 noblkAnT2I[FTM_CALIDCOCS_AGC_NUM];
    VOS_UINT16 noblkAnT2Q[FTM_CALIDCOCS_AGC_NUM];
    VOS_UINT16 blkAnT2I[FTM_CALIDCOCS_AGC_NUM];
    VOS_UINT16 blkAnT2Q[FTM_CALIDCOCS_AGC_NUM];
} FTM_RdFcalidcocsCnf;

/* AT^FCALIIP2SMRF */
#define FTM_CT_MAX_RFIC_PATH_NUM 2
#define FTM_CT_MAX_RX_ANT_NUM 2

typedef struct {
    VOS_UINT16 chanNum;
    VOS_UINT16 band;
    VOS_UINT16 rfid;
    VOS_UINT16 chan[FTM_CALIIP2_MAX_CHAN_NUM];
} FTM_SetFcaliip2SmrfReq;

typedef struct {
    VOS_UINT16 validFlg;
    VOS_UINT16 rsv;
    VOS_UINT16 mainIDivOff[FTM_CALIIP2_MAX_CHAN_NUM];
    VOS_UINT16 mainQDivOff[FTM_CALIIP2_MAX_CHAN_NUM];
    VOS_UINT16 mainIDivOn[FTM_CALIIP2_MAX_CHAN_NUM];
    VOS_UINT16 mainQDivOn[FTM_CALIIP2_MAX_CHAN_NUM];
    VOS_UINT16 divI[FTM_CALIIP2_MAX_CHAN_NUM];
    VOS_UINT16 divQ[FTM_CALIIP2_MAX_CHAN_NUM];
} FTM_FCALIIP2SMRF_CAL_ITEM;

typedef struct {
    VOS_UINT32                errCode;
    VOS_UINT16                status;
    VOS_UINT16                chanNum;
    VOS_UINT16                chan[FTM_CALIIP2_MAX_CHAN_NUM];
    FTM_FCALIIP2SMRF_CAL_ITEM ip2CalRst[FTM_CT_MAX_RFIC_PATH_NUM];
} FTM_RdFcaliip2SmrfCnf;

/* AT^FCALIDCOCSMRF */
typedef struct {
    VOS_UINT16 channel; /* �ŵ��� */
    VOS_UINT16 band;    /* band   */
    VOS_UINT16 rfid;    /* ͨ���� */
    VOS_UINT16 rsv;
} FTM_SetFcalidcocsmrfReq;

typedef struct {
    VOS_UINT16 antCompDci[FTM_CALIDCOCS_AGC_NUM];
    VOS_UINT16 antCompDcq[FTM_CALIDCOCS_AGC_NUM];
} FTM_FcalidcocsmrfAntItem;

typedef struct {
    VOS_UINT16               validFlg;
    VOS_UINT16               validAntNum;
    FTM_FcalidcocsmrfAntItem antCal[FTM_CT_MAX_RX_ANT_NUM];
} FTM_FcalidcocsmrfDccompItem;

typedef struct {
    VOS_UINT32                  errCode;
    VOS_UINT16                  status;
    VOS_UINT16                  channel;
    FTM_FcalidcocsmrfDccompItem dcocCalRst[FTM_CT_MAX_RFIC_PATH_NUM];
} FTM_RdFcalidcocsmrfCnf;

/* AT^FGAINSTATES */
typedef struct {
    VOS_UINT16 gainStateNum;
    VOS_UINT16 rsv;
    VOS_UINT16 gainState[FTM_GAINSTATE_MAX_NUM_HI6362];
} FTM_SetFgainstatesReq;

/* AT^FDBBATTS */
typedef struct {
    VOS_UINT16 dbbCodeNum;
    VOS_UINT16 dbbCode;
} FTM_SetFdbbattsReq;

/* AT^FBBATTS */
typedef struct {
    VOS_UINT16 bbAttNum;
    VOS_UINT16 rsv;
    VOS_UINT16 bbAtt[FTM_BBATT_MAX_NUM];
} FTM_SetFbbattsReq;

/* AT^FCALITXIQS */
typedef struct {
    VOS_UINT16 channel;
    VOS_UINT16 band;
    VOS_UINT16 rfid;
    VOS_UINT16 rsv2;
} FTM_SetFcalitxiqsReq;

typedef struct {
    VOS_UINT32 errCode;
    VOS_UINT16 status;
    VOS_UINT16 amplitude;
    VOS_UINT16 phase;
    VOS_UINT16 dci;
    VOS_UINT16 dcq;
    VOS_UINT16 reserved;
} FTM_RdFcalitxiqsCnf;

#define FTM_MAX_COMM_CMD_LEN 1152
typedef enum {
    EN_LTCOMMCMD_DEST_LDSP_CT = 0,
    EN_LTCOMMCMD_DEST_LDSP_BT = 1,
    EN_LTCOMMCMD_DEST_TDSP_CT = 2,
    EN_LTCOMMCMD_DEST_TDSP_BT = 3,
} FTM_LTCOMMCMD_DEST;

typedef struct {
    FTM_LTCOMMCMD_DEST cmdDest;
    VOS_UINT32         dataLen;
    VOS_CHAR           data[0];  //lint !e43
} FTM_SetLtcommcmdReq;

typedef struct {
    VOS_UINT32 errCode;
    VOS_UINT32 dataLen;
    VOS_CHAR   data[0];  //lint !e43
} FTM_SetLtcommcmdCnf;

typedef struct {
    VOS_UINT32 errCode;
    VOS_UINT32 dataLen;
    /* ��������LMspLPhyInterface.hͷ�ļ��󣬻����𹤾߱��벻��������ֱ�������ݣ���С��Ҫ���MAX_COMM_CMD_LEN����һ�´� */
    VOS_CHAR data[1152];
} FTM_RdLtcommcmdCnf;

typedef struct {
    VOS_UINT32 msgId;                 /* ��Ϣ��ʶ */
    VOS_UINT32 dataLen;               /* ���ݳ��� */
    VOS_UINT8  data[0]; /* ����     */  //lint !e43
} FTM_PHY_CommCmdSetReq;

typedef struct {
    VOS_UINT32 msgId;                 /* ��Ϣ��ʶ */
    VOS_UINT32 dataLen;               /* ���ݳ��� */
    VOS_UINT8  data[0]; /* ����     */  //lint !e43
} PHY_FTM_CommCmdSetCnf;

typedef PHY_FTM_CommCmdSetCnf PHY_FTM_COMM_CMD_IND_STRU;

typedef struct {
    VOS_MSG_HEADER         /* _H2ASN_Skip */
    VOS_UINT32      msgId; /* _H2ASN_Skip */

    APS_L4A_Appctrl ctrl;
} L4A_COMM_Req;
typedef L4A_COMM_Req L4A_READ_LWCLASH_REQ_STRU;

typedef struct {
    VOS_UINT32 ulFreq;      /* ��������Ƶ�㵥λ:100Khz */
    VOS_UINT32 dlFreq;      /* ��������Ƶ�� ��λ:100Khz */
    VOS_UINT16 ulBandwidth; /* ���д��� */
    VOS_UINT16 dlBandwidth; /* ���д��� */
    VOS_UINT8  camped;      /* �Ƿ�פ�� */
    VOS_UINT8  state;       /* �Ƿ�Ϊ��ͻ״̬ */
    VOS_UINT8  band;        /* Ƶ��ָʾ */
    APP_RRC_AntennaMaxLayersMimoUint8 dlMimo;
} L4A_LWCLASH_Info;

typedef struct {
    VOS_MSG_HEADER               /* _H2ASN_Skip */
    VOS_UINT32         msgId;    /* _H2ASN_Skip */

    VOS_UINT16         clientId; /* �ͻ���ID */
    VOS_UINT8          opId;
    PS_BOOL_ENUM_UINT8 dl256QamFlag;
    VOS_UINT32         errorCode;

    L4A_LWCLASH_Info   lwclashInfo;
    VOS_UINT32         scellNum;
    RRC_APP_ScellInfo  scellInfo[LRRC_APP_LWCLASH_MAX_SCELL_NUM];
} L4A_READ_LwclashCnf;

typedef struct {
    VOS_MSG_HEADER            /* _H2ASN_Skip */
    VOS_UINT32         msgId; /* _H2ASN_Skip */

    VOS_UINT16         clientId; /* �ͻ���ID */
    VOS_UINT8          opId;     /* ������ID */
    PS_BOOL_ENUM_UINT8 dl256QamFlag;
    L4A_LWCLASH_Info   lwclashInfo;
    VOS_UINT32         scellNum;
    RRC_APP_ScellInfo  scellInfo[LRRC_APP_LWCLASH_MAX_SCELL_NUM];
} L4A_READ_LwclashInd;

typedef L4A_COMM_Req L4A_READ_LCACELL_REQ_STRU;

typedef struct {
    VOS_UINT8 ulConfigured; /* CA�����Ƿ����ã�0:δ���ã�1:������ */
    VOS_UINT8 dlConfigured; /* CA�����Ƿ����ã�0:δ���ã�1:������ */
    VOS_UINT8 actived;      /* SCell�Ƿ񱻼��0:ȥ���1:���� */
    VOS_UINT8 rsv;
} L4A_CACELL_Info;

typedef struct {
    VOS_MSG_HEADER                /* _H2ASN_Skip */
    APS_L4A_MsgIdUint32 msgId;    /* _H2ASN_Skip */

    VOS_UINT16          clientId; /* �ͻ���ID */
    VOS_UINT8           opId;
    VOS_UINT8           resv[1];
    VOS_UINT32          errorCode;

    L4A_CACELL_Info     lcacellInfo[CA_MAX_CELL_NUM];
} L4A_READ_LcacellCnf;

typedef struct {
    VOS_MSG_HEADER             /* _H2ASN_Skip */
    APS_L4A_MsgIdUint32 msgId; /* _H2ASN_Skip */

    VOS_UINT16          clientId; /* �ͻ���ID */
    VOS_UINT8           opId;     /* ������ID */
    VOS_UINT8           resv[1];  /* CID */
    L4A_CACELL_Info     lcacellInfo[CA_MAX_CELL_NUM];
} L4A_READ_LcacellInd;

/*
 * ����:   ^ISMCOEX
 * ID:
 * �ṹ:
 */
#define L4A_ISMCOEX_BANDWIDTH_NUM 6

typedef struct {
    VOS_UINT32 flag;
    VOS_UINT32 txBegin;
    VOS_UINT32 txEnd;
    VOS_INT32  txPower;
    VOS_UINT32 rxBegin;
    VOS_UINT32 rxEnd;
} ISMCOEX_INFO;

typedef struct {
    VOS_MSG_HEADER
    APS_L4A_MsgIdUint32 msgId;

    APS_L4A_Appctrl ctrl;

    ISMCOEX_INFO coex[L4A_ISMCOEX_BANDWIDTH_NUM];
} L4A_ISMCOEX_Req;

typedef struct {
    VOS_MSG_HEADER
    APS_L4A_MsgIdUint32 msgId;

    VOS_UINT16 clientId;
    VOS_UINT8  opId;
    VOS_UINT8  resv[1];
    VOS_UINT32 errorCode;
} L4A_ISMCOEX_Cnf;

typedef L4A_COMM_Req L4A_READ_LTECS_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER             /* _H2ASN_Skip */
    APS_L4A_MsgIdUint32 msgId; /* _H2ASN_Skip */

    VOS_UINT16          clientId;  /* �ͻ���ID */
    VOS_UINT8           opId;      /* ������ID */
    VOS_UINT8           reserved[1];
    VOS_UINT32          errorCode; /* ������ */

    APP_MM_LteCsInfo    ltecsInfo;
} L4A_READ_LtecsCnf;

typedef struct {
    VOS_MSG_HEADER             /* _H2ASN_Skip */
    APS_L4A_MsgIdUint32 msgId; /* _H2ASN_Skip */

    APS_L4A_Appctrl     ctrl;
    VOS_UINT8           n;
    VOS_UINT8           reserved[3];
} L4A_READ_CnmrReq;

typedef struct {
    VOS_MSG_HEADER             /* _H2ASN_Skip */
    APS_L4A_MsgIdUint32 msgId; /* _H2ASN_Skip */

    VOS_UINT16          clientId;  /* �ͻ���ID */
    VOS_UINT8           opId;      /* ������ID */
    VOS_UINT8           reserved[1];
    VOS_UINT32          errorCode; /* ������ */
    RRC_APP_LteCellInfo lteCelInfo;
} L4A_READ_CnmrCnf;

typedef struct {
    VOS_MSG_HEADER             /* _H2ASN_Skip */
    APS_L4A_MsgIdUint32 msgId; /* _H2ASN_Skip */

    APS_L4A_Appctrl     ctrl;
    VOS_UINT8           ucnDb;
    VOS_UINT8           minRptTimerInterval;
    VOS_UINT8           reserved[2];
} L4A_CSQ_InfoReq;

typedef struct {
    VOS_MSG_HEADER             /* _H2ASN_Skip */
    APS_L4A_MsgIdUint32 msgId; /* _H2ASN_Skip */

    VOS_UINT16          clientId;  /* �ͻ���ID */
    VOS_UINT8           opId;      /* ������ID */
    VOS_UINT8           reserved[1];
    VOS_UINT32          errorCode; /* ������ */

    VOS_INT16           rssi; /* Rssi */
    VOS_INT16           rsd;  /* ����ֶ� */
    VOS_INT16           rsrp; /* ��Χ��(-141,-44), 99Ϊ��Ч */
    VOS_INT16           rsrq; /* ��Χ��(-40, -6) , 99Ϊ��Ч */
    VOS_INT32           sinr; /* SINR RS_SNR */
    APP_RRC_CqiInfo     cqi;  /* CQI�������� */
} L4A_CSQ_InfoCnf;

typedef L4A_CSQ_InfoCnf L4A_CSQ_INFO_IND_STRU;

typedef struct {
    VOS_MSG_HEADER             /* _H2ASN_Skip */
    APS_L4A_MsgIdUint32 msgId; /* _H2ASN_Skip */

    VOS_UINT16          clientId;  /* �ͻ���ID */
    VOS_UINT8           opId;      /* ������ID */
    VOS_UINT8           reserved[1];
    VOS_UINT32          errorCode; /* ������ */

    VOS_INT16           rssi;  /* Rssi */
    VOS_INT16           level; /*  */
    VOS_INT16           rsrp;  /* ��Χ��(-141,-44), 99Ϊ��Ч */
    VOS_INT16           rsrq;  /* ��Χ��(-40, -6) , 99Ϊ��Ч */
} AT_AnlevelInfoCnf;

#define L4A_CURC_RPT_CFG_MAX_SIZE 8
typedef struct {
    VOS_UINT32 modeBitValid : 1;    /* save mode set command-- 1:valid 0:invalid */
    VOS_UINT32 modeBit : 1;         /* save mode set command-- 1:open 0:close */
    VOS_UINT32 lwurcBitValid : 1;   /* save lwclash set command- 1:valid 0:invalid */
    VOS_UINT32 lwurcBit : 1;        /* save lwclash set command- 1:open 0:close */
    VOS_UINT32 cerssiBitValid : 1;  /* save cerssi set command-- 1:valid 0:invalid */
    VOS_UINT32 cerssiBit : 1;       /* save cerssi set command-- 1:open 0:close */
    VOS_UINT32 rssiBitValid : 1;    /* save rssi set command--     1:valid 0:invalid */
    VOS_UINT32 rssiBit : 1;         /* save rssi set command--     1:open 0:close */
    VOS_UINT32 anlevelBitValid : 1; /* save anlevel set command--1:valid 0:invalid */
    VOS_UINT32 anlevelBit : 1;      /* save anlevel set command--1:open 0:close */
    VOS_UINT32 lcacellBitValid : 1; /* save lcacell set command--1:valid 0:invalid */
    VOS_UINT32 lcacellBit : 1;      /* save lcacell set command--1:open 0:close */
    VOS_UINT32 curcBitValid : 1;    /* save cruc=2                      --1:valid 0;invalid */
    VOS_UINT32 reserved : 21;
    VOS_UINT8  curcCfgReq[L4A_CURC_RPT_CFG_MAX_SIZE]; /* save curc����2 */
} L4A_IND_Cfg;

typedef struct {
    VOS_MSG_HEADER             /* _H2ASN_Skip */
    APS_L4A_MsgIdUint32 msgId; /* _H2ASN_Skip */

    APS_L4A_Appctrl     ctrl;
    VOS_UINT32          cellFlag;
} L4A_READ_CellInfoReq;

typedef struct {
    VOS_MSG_HEADER                /* _H2ASN_Skip */
    APS_L4A_MsgIdUint32    msgId; /* _H2ASN_Skip */

    VOS_UINT16             clientId;  /* �ͻ���ID */
    VOS_UINT8              opId;      /* ������ID */
    VOS_UINT8              reserved[1];
    VOS_UINT32             errorCode; /* ������ */
    LRRC_APP_NcellListInfo ncellListInfo;
} L4A_READ_CellInfoCnf;

typedef L4A_COMM_Req L4A_READ_CELL_ID_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER             /* _H2ASN_Skip */
    APS_L4A_MsgIdUint32 msgId; /* _H2ASN_Skip */

    VOS_UINT16          clientId; /* �ͻ���ID */
    VOS_UINT8           opId;     /* ������ID */
    VOS_UINT8           reserved[1];
    VOS_UINT32          errorCode; /* ������. */
    VOS_UINT16          mncNum;    /* ?? MNC ?? */
    VOS_UINT16          mcc;
    VOS_UINT16          mnc;
    VOS_UINT16          rev;
    VOS_UINT32          ci;  /* 0xFFFF???, */
    VOS_UINT16          pci; /* 0xFFFF???,??:(0,503) */
    VOS_UINT16          tac; /* TAC */
} L4A_READ_CellIdCnf;

typedef struct {
    VOS_UINT16 mcc; /* Mobile Country Code , 0x460 */
    VOS_UINT16 mnc; /* Mobile Network Code ,���MNCֻ����λ,��߰�λΪ0x0f */
} L4A_PLMN_Id;

typedef L4A_COMM_Req L4A_SET_LTE_TO_IDLE_REQ_STRU;

typedef struct {
    VOS_MSG_HEADER             /* _H2ASN_Skip */
    APS_L4A_MsgIdUint32 msgId; /* _H2ASN_Skip */

    APS_L4A_Appctrl     ctrl;
    VOS_UINT32          flag;
} L4A_LW_ThreasholdCfgReq;

typedef struct {
    VOS_MSG_HEADER             /* _H2ASN_Skip */
    APS_L4A_MsgIdUint32 msgId; /* _H2ASN_Skip */

    APS_L4A_Appctrl     ctrl;
    VOS_INT32           flag;
    VOS_INT32           timerLen;
} L4A_SET_FastDormReq;

typedef struct {
    VOS_MSG_HEADER             /* _H2ASN_Skip */
    APS_L4A_MsgIdUint32 msgId; /* _H2ASN_Skip */

    APS_L4A_Appctrl     ctrl;
    L4A_IND_Cfg         indCfg;
} L4A_IND_CfgReq;

typedef struct {
    VOS_MSG_HEADER             /* _H2ASN_Skip */
    APS_L4A_MsgIdUint32 msgId; /* _H2ASN_Skip */

    VOS_UINT16          clientId; /* �ͻ���ID */
    VOS_UINT8           opId;     /* ������ID */
    VOS_UINT8           reserved[1];
    VOS_UINT32          errorCode; /* ������ */
} L4A_AT_CnfHeader;

typedef struct {
    VOS_MSG_HEADER             /* _H2ASN_Skip */
    APS_L4A_MsgIdUint32 msgId; /* _H2ASN_Skip */

    APS_L4A_Appctrl     ctrl;
    VOS_UINT32          mod;
    VOS_UINT32          ver;
} L4A_SET_RadverReq;

typedef struct {
    VOS_MSG_HEADER                 /* _H2ASN_Skip */
    APS_L4A_MsgIdUint32 msgId; /* _H2ASN_Skip */

    VOS_UINT16          clientId; /* �ͻ���ID */
    VOS_UINT8           opId;     /* ������ID */
    VOS_UINT8           reserved;
    VOS_UINT32          errorCode; /* ������ */
} L4A_SET_RadverCnf;

typedef L4A_COMM_Req L4A_READ_ScellInfoReq;

typedef struct {
    VOS_MSG_HEADER
    APS_L4A_MsgIdUint32 msgId;
    VOS_UINT16          clientId;
    VOS_UINT8           opId;
    VOS_UINT8           reserved;
    VOS_UINT32          errorCode;
    VOS_UINT32          sCellCnt;
    RRC_SCELL_Info      sCellInfo[LRRC_SCELL_MAX_NUM];
} L4A_READ_ScellInfoCnf;

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* __GEN_MSG_H__ */
