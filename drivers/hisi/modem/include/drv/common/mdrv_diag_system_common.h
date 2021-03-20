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

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#ifndef __MDRV_DIAG_SYSTEM_COMMON_H__
#define __MDRV_DIAG_SYSTEM_COMMON_H__

#include <stdarg.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*�c��PS��MODID ��DIAG_AIR_MSG_LOG_ID�ĵط����滻��DIAG_ID*/
#define DRV_DIAG_ID(module_id, log_type)   (unsigned int)(module_id | (log_type << 12))

#define DRV_DIAG_AIR_MSG_LOG_ID(module_id, is_up_link)  DRV_DIAG_ID(module_id, is_up_link) /*module_id��ӦPID*/

#define DRV_DIAG_GEN_LOG_MODULE(modemid, modetype, level)  \
               (((unsigned int)(modemid & 0xff) << 24)  \
              | ((unsigned int)(modetype & 0xf) << 16)  \
              | ((unsigned int)(level    & 0xf ) << 12))

#define DRV_DIAG_GEN_MODULE(modemid, modetype)  \
               (((unsigned int)(modemid & 0xff) << 24)  \
              | ((unsigned int)(modetype & 0xf) << 16))


#define DRV_DIAG_GEN_MODULE_EX(modemid, modetype, groupid)  \
                   (((unsigned int)(modemid & 0xff) << 24)  \
                  | ((unsigned int)(modetype & 0xf) << 16) \
                  | ((unsigned int)(groupid  & 0xf)  << 8))

#define DRV_DIAG_GEN_LOG_ID(filenum, linenum)   \
                ((((unsigned int)(filenum & 0XFFFF)) << 16)   \
                | ((unsigned int)(linenum & 0XFFFF)))

/* diag�Թ����ڴ���������� */
typedef enum
{
    POWER_ON_LOG_A = 0,
    DS_DATA_BUFFER_STATE = 1,
    POWER_ON_LOG_BUFF = 2,
}mdrv_diag_shm_type_e;

typedef enum
{
    EN_DIAG_PTR_BEGIN = 0,
    EN_DIAG_PTR_PPM_READDATA    = 1,
    EN_DIAG_PTR_CPM_COMRCV      = 2,
    EN_DIAG_PTR_SCM_SOFTDECODE  = 3,
    EN_DIAG_PTR_SCM_SELFTASK    = 4,
    EN_DIAG_PTR_SCM_RCVDATA     = 5,
    EN_DIAG_PTR_SCM_RCVDATA_SUCCESS = 6,
    EN_DIAG_PTR_SCM_DISPATCH    = 7,
    EN_DIAG_PTR_MSP_SERVICE_1   = 8,
    EN_DIAG_PTR_MSP_SERVICE_2   = 9,
    EN_DIAG_PTR_SERVICE_IN      = 0xA,
    EN_DIAG_PTR_SERVICE_1       = 0xB,
    EN_DIAG_PTR_MESSAGE_IN      = 0xC,
    EN_DIAG_PTR_MESSAGE_PROC    = 0xD,
    EN_DIAG_PTR_MSGMSP_IN       = 0xE,
    EN_DIAG_PTR_MSGMSP_TRANS    = 0xF,
    EN_DIAG_PTR_MSGMSP_PS_IN    = 0x10,
    EN_DIAG_PTR_MSGMSP_PS_OUT   = 0x11,
    EN_DIAG_PTR_MSGMSP_CONN_IN  = 0x12,
    EN_DIAG_PTR_MSGMSP_DISCONN_IN=0x13,
    EN_DIAG_PTR_MESSAGE_REPORT  = 0x14,
    EN_DIAG_PTR_SERVICE_PACKETDATA=0x15,
    EN_DIAG_PTR_SEND_CODER_SRC  = 0x16,
    EN_DIAG_PTR_SCM_CODER_SRC   = 0x17,
    EN_DIAG_PTR_SCM_CODER_DST_CB= 0x18,
    EN_DIAG_PTR_SCM_SENDTOUDI   = 0x19,
    EN_DIAG_PTR_CPM_COMSEND     = 0x1A,
    EN_DIAG_PTR_PPM_PORTSEND    = 0x1B,
    EN_DIAG_PTR_BSP_MSG_PROC    = 0x1C,
    EN_DIAG_PTR_TRANS_BBP_PROC  = 0x1D,
    EN_DIAG_PTR_BBP_MSG_PROC    = 0x1E,
    EN_DIAG_PTR_BBP_MSG_PROC_CNF        = 0x1F,
    EN_DIAG_PTR_BBP_GET_CHAN_SIZE_PROC  = 0x20,
    EN_DIAG_PTR_BBP_GET_CHAN_SIZE_CNF   = 0x21,
    EN_DIAG_PTR_BBP_SAMPLE_GEN_PROC     = 0x22,
    EN_DIAG_PTR_BBP_SAMPLE_GEN_CNF      = 0x23,
    EN_DIAG_PTR_BBP_GET_ADDR_PROC       = 0x24,
    EN_DIAG_PTR_BBP_GET_ADDR_PROC_CNF   = 0x25,
    EN_DIAG_PTR_BBP_GET_VERSION_PROC    = 0x26,
    EN_DIAG_PTR_BBP_GET_VERSION_CNF     = 0x27,
    EN_DIAG_PTR_BBP_ABLE_CHAN_PROC      = 0x28,
    EN_DIAG_PTR_BBP_ABLE_CHAN_CNF       = 0x29,
    EN_DIAG_PTR_APP_TRANS_PHY_PROC      = 0x2A,
    EN_DIAG_PTR_DSP_MSG_PROC            = 0x2B,
    EN_DIAG_PTR_DSP_MSG_CNF             = 0x2C,
    EN_DIAG_PTR_APP_TRANS_HIFI_PROC     = 0x2D,
    EN_DIAG_PTR_HIFI_MSG_PROC           = 0x2E,
    EN_DIAG_PTR_HIFI_MSG_CNF            = 0x2F,
    EN_DIAG_PTR_SOCP_VOTE_CNF_MSGPROC   = 0x30,
    EN_DIAG_PTR_APP_TRANS_MSG_PROC      = 0x31,
    EN_DIAG_PTR_FAIL_CMD_CNF            = 0x32,
    EN_DIAG_PTR_PRINT_CFG_PROC          = 0x33,
    EN_DIAG_PTR_PRINT_CFG_SUCESS_CNF    = 0x34,
    EN_DIAG_PTR_PRINT_CFG_FAIL_CNF      = 0x35,
    EN_DIAG_PTR_LAYER_CFG_PROC          = 0x36,
    EN_DIAG_PTR_LAYER_CFG_SUCESS_CNF    = 0x37,
    EN_DIAG_PTR_LAYER_CFG_FAIL_CNF      = 0x38,
    EN_DIAG_PTR_AIR_CFG_PROC            = 0x39,
    EN_DIAG_PTR_AIR_CFG_SUCESS_CNF      = 0x3A,
    EN_DIAG_PTR_AIR_CFG_FAIL_CNF        = 0x3b,
    EN_DIAG_PTR_EVENT_CFG_PROC          = 0x3C,
    EN_DIAG_PTR_EVENT_CFG_SUCESS_CNF    = 0x3D,
    EN_DIAG_PTR_EVENT_CFG_FAIL_CNF      = 0x3E,
    EN_DIAG_PTR_MSG_CFG_PROC            = 0x3F,
    EN_DIAG_PTR_MSG_CFG_SUCESS_CNF      = 0x40,
    EN_DIAG_PTR_MSG_CFG_FAIL_CNF        = 0x41,
    EN_DIAG_PTR_GTR_CFG_PROC            = 0x42,
    EN_DIAG_PTR_GTR_CFG_SUCESS_CNF      = 0x43,
    EN_DIAG_PTR_GTR_CFG_FAIL_CNF        = 0x44,
    EN_DIAG_PTR_GUGTR_CFG_PROC          = 0x45,
    EN_DIAG_PTR_GET_TIME_STAMP      = 0x46,
    EN_DIAG_PTR_GET_TIME_STAMP_CNF  = 0x47,
    EN_DIAG_PTR_GET_MODEM_NUM       = 0x48,
    EN_DIAG_PTR_GET_MODEM_NUM_CNF   = 0x49,
    EN_DIAG_PTR_GET_PID_TABLE       = 0x4A,
    EN_DIAG_PTR_GET_PID_TABLE_CNF   = 0x4B,
    EN_DIAG_PTR_TRANS_CNF_PROC      = 0x4C,
    EN_DIAG_PTR_FS_QUERY_PROC       = 0x4D,
    EN_DIAG_PTR_FS_SCAN_PROC        = 0x4E,
    EN_DIAG_PTR_FS_MAKE_DIR_PROC    = 0x4F,
    EN_DIAG_PTR_FS_OPEN_PROC        = 0x50,
    EN_DIAG_PTR_FS_IMPORT_PROC      = 0x51,
    EN_DIAG_PTR_FS_EXOPORT_PROC     = 0x52,
    EN_DIAG_PTR_FS_DELETE_PROC      = 0x53,
    EN_DIAG_PTR_FS_SPACE_PROC       = 0x54,
    EN_DIAG_PTR_NV_RD_PROC          = 0x55,
    EN_DIAG_PTR_NV_RD_SUCESS_CNF    = 0x56,
    EN_DIAG_PTR_NV_RD_FAIL_CNF      = 0x57,
    EN_DIAG_PTR_GET_NV_LIST_PROC    = 0x58,
    EN_DIAG_PTR_GET_NV_LIST_SUCESS_CNF = 0x59,
    EN_DIAG_PTR_GET_NV_LIST_FAIL_CNF   = 0x5A,
    EN_DIAG_PTR_GET_NV_RESUME_LIST_PROC         = 0x5B,
    EN_DIAG_PTR_GET_NV_RESUME_LIST_SUCESS_CNF  = 0x5C,
    EN_DIAG_PTR_GET_NV_RESUME_LIST_FAIL_CNF    = 0x5D,
    EN_DIAG_PTR_NV_WR_PROC          = 0x5E,
    EN_DIAG_PTR_NV_WR_SUCESS_CNF    = 0x5F,
    EN_DIAG_PTR_NV_WR_FAIL_CNF      = 0x60,
    EN_DIAG_PTR_AUTH_NV_CFG         = 0x61,
    EN_DIAG_PTR_NV_AUTH_PROC        = 0x62,
    EN_DIAG_PTR_NV_AUTH_FAIL_CNF    = 0x63,
    EN_DIAG_PTR_CFG_TRANS           = 0x64,
    EN_DIAG_PTR_CFG_TRANS_CNF       = 0x65,
    EN_DIAG_PTR_CFG_HIGHTS          = 0x66,
    EN_DIAG_PTR_CFG_HIGHTS_CNF      = 0x67,
    EN_DIAG_PTR_CFG_USERPLANE       = 0x68,
    EN_DIAG_PTR_CFG_USERPLANE_CNF   = 0x69,
    EN_DIAG_PTR_CFG_HIMS_VER_CNF    = 0x6a,
    EN_DIAG_PTR_CFG_END,

    EN_DIAG_PTR_ERR_BEGIN = 0x100,
    EN_DIAG_PTR_PPM_ERR1,
    EN_DIAG_PTR_PPM_ERR2,
    EN_DIAG_PTR_CPM_ERR1,
    EN_DIAG_PTR_CPM_ERR2,
    EN_DIAG_PTR_CPM_ERR3,
    EN_DIAG_PTR_SCM_ERR1,
    EN_DIAG_PTR_SCM_ERR2,
    EN_DIAG_PTR_SCM_ERR3,
    EN_DIAG_PTR_MSP_SERVICE_ERR1,
    EN_DIAG_PTR_MSP_SERVICE_ERR2,
    EN_DIAG_PTR_MSP_SERVICE_ERR3,
    EN_DIAG_PTR_DIAG_SERVICE_ERR1,
    EN_DIAG_PTR_DIAG_SERVICE_ERR2,
    EN_DIAG_PTR_DIAG_SERVICE_ERR3,
    EN_DIAG_PTR_DIAG_BUFFER_ERR1,
    EN_DIAG_PTR_DIAG_BUFFER_ERR2,
    EN_DIAG_PTR_HIMS_VER_CNF_FAIL,
    EN_DIAG_PTR_MAX
} diag_ptr_id_e;

enum mdrv_diag_port_type
{
    DIAGLOG_OM_PORT_TYPE_USB = 0,
    DIAGLOG_OM_PORT_TYPE_VCOM = 1,
    DIAGLOG_OM_PORT_TYPE_WIFI = 2,
    DIAGLOG_OM_PORT_TYPE_BUTT
};

enum mdrv_diag_poweron_log_profile
{
    DIAGLOG_POWER_LOG_PROFILE_OFF = 0,
    DIAGLOG_POWER_LOG_PROFILE_SIMPLE = 1,
    DIAGLOG_POWER_LOG_PROFILE_NORAML = 2,
    DIAGLOG_POWER_LOG_PROFILE_WHOLE  = 3,
    DIAGLOG_POWER_LOG_PROFILE_BUTT
};

/* throughput ����������*******************************************/

typedef enum
{
    EN_DIAG_THRPUT_DATA_CHN_ENC = 0,    /* ����ͨ����������� */

    EN_DIAG_THRPUT_DATA_CHN_PHY,        /* ����ͨ��д������ͨ�� */

    EN_DIAG_THRPUT_DATA_CHN_CB,         /* ��������ͨ���ص� */
    EN_DIAG_THRPUT_MAX
}diag_thrput_id_e;

/*diag mem**********************************************************************/
#ifdef ENABLE_DIAG_FIX_ADDR

#define DIAG_CODER_SRC_ACORE_CNF_PADDR      (DDR_SOCP_ADDR)
#define DIAG_CODER_SRC_ACORE_CNF_LENGTH     (1024*64)

#define DIAG_CODER_SRC_ACORE_IND_PADDR      (DIAG_CODER_SRC_ACORE_CNF_PADDR+DIAG_CODER_SRC_ACORE_CNF_LENGTH)
#define DIAG_CODER_SRC_ACORE_IND_LENGTH     (1024*1024)

#define DIAG_CODER_SRC_CCORE_CNF_PADDR      (DIAG_CODER_SRC_ACORE_IND_PADDR+DIAG_CODER_SRC_ACORE_IND_LENGTH)
#define DIAG_CODER_SRC_CCORE_CNF_LENGTH     (1024*64)

#define DIAG_CODER_SRC_CCORE_IND_PADDR      (DIAG_CODER_SRC_CCORE_CNF_PADDR+DIAG_CODER_SRC_CCORE_CNF_LENGTH)
#define DIAG_CODER_SRC_CCORE_IND_LENGTH     (1536*1024)

#define DIAG_CORER_SRC_DSP_PADDR            (DIAG_CODER_SRC_CCORE_IND_PADDR+DIAG_CODER_SRC_CCORE_IND_LENGTH)
#define DIAG_CORER_SRC_DSP_LENGTH           (1024*1024)

#define BBP_LOG0_MEM_ADDR                   (DIAG_CORER_SRC_DSP_PADDR+DIAG_CORER_SRC_DSP_LENGTH)
#define BBP_LOG0_MEM_SIZE                   (64*1024)

#define BBP_LOG1_MEM_ADDR                   (BBP_LOG0_MEM_ADDR + BBP_LOG0_MEM_SIZE)
#define BBP_LOG1_MEM_SIZE                   (8*1024)

#define SOCP_CODER_SRC_GUDSP_ADDR            (BBP_LOG1_MEM_ADDR+BBP_LOG1_MEM_SIZE)
#define SOCP_CODER_SRC_GUDSP_LENGTH          (128*1024)

#define SOCP_CODER_SRC_XDSP_ADDR             (SOCP_CODER_SRC_GUDSP_ADDR+SOCP_CODER_SRC_GUDSP_LENGTH)
#define SOCP_CODER_SRC_XDSP_LENGTH           (512*1024)

#define SOCP_CODER_SRC_HIFI_ADDR             (SOCP_CODER_SRC_XDSP_ADDR+SOCP_CODER_SRC_XDSP_LENGTH)
#define SOCP_CODER_SRC_HIFI_LENGTH           (32*1024)

#define SOCP_CODER_SRC_CCORE_HDS_ADDR        (SOCP_CODER_SRC_HIFI_ADDR+SOCP_CODER_SRC_HIFI_LENGTH)
#define SOCP_CODER_SRC_CCORE_HDS_LENGTH      (8*1024)

#define SOCP_CODER_SRC_ACORE_HDS_ADDR        (SOCP_CODER_SRC_CCORE_HDS_ADDR+SOCP_CODER_SRC_CCORE_HDS_LENGTH)
#define SOCP_CODER_SRC_ACORE_HDS_LENGTH      (8*1024)

#define BBP_SOCP_ADDR                       (DDR_SOCP_ADDR + 8*1024*1024)
#define BBP_SOCP_SIZE                       (DDR_SOCP_SIZE - 8*1024*1024)

#else

#ifdef DIAG_BBP_WRPTR_OFFSET
#define BBP_SOCP_PTR_ADDR               (DDR_SOCP_ADDR)
#define BBP_SOCP_PTR_SIZE               (128)
#define BBP_SOCP_BUS_ADDR               (BBP_SOCP_PTR_ADDR + BBP_SOCP_PTR_SIZE)
#define BBP_SOCP_BUS_SIZE               (16*1024*1024 - BBP_SOCP_PTR_SIZE)
#define BBP_SOCP_ADDR                   (BBP_SOCP_BUS_ADDR + BBP_SOCP_BUS_SIZE)
#define BBP_SOCP_SIZE                   (DDR_SOCP_SIZE - BBP_SOCP_BUS_SIZE - BBP_SOCP_PTR_SIZE)
#define BBP_BUS_MEM_SIZE                (0)
#else
#define BBP_SOCP_ADDR                   (DDR_SOCP_ADDR)
#define BBP_SOCP_SIZE                   (DDR_SOCP_SIZE)
#define BBP_BUS_MEM_SIZE                (64*1024)
#endif

#define BBP_LOG1_MEM_SIZE               (8*1024)

#ifdef DIAG_SYSTEM_5G
#define BBP_5G_BUS_MEM_SIZE             (64*1024)
#define BBP_RFIC0_BUS_MEM_SIZE          (64*1024)
#define BBP_RFIC1_BUS_MEM_SIZE          (64*1024)
#endif

/*diag ??DSP_PID_STARTUP?modem??ok��?*/
#define ID_MSG_DIAG_CMD_PHY_READY_REQ               (0x10010203)
#define ID_MSG_DIAG_CMD_PHY_READY_CNF               (ID_MSG_DIAG_CMD_PHY_READY_REQ)

#endif

/********************* BBP DS�ڴ����� **************************
* BBP LOG 0:  64K
* BBP LOG 1:   8K
* BBP DS   :  ���
* TOTAL SIZE: BBPDS + 120K
****************************************************************/
#define BBP_DS_MEM_ADDR                         	(BBP_SOCP_ADDR)
#define BBP_DS_MEM_SIZE                             (BBP_SOCP_SIZE)

/*error no begin**********************************************************************/
#define  ERR_MSP_SUCCESS                (0)
#define  ERR_MSP_FAILURE                (0xFFFFFFFFU)

#define  ERR_MSP_WAIT_ASYNC             (1)
#define  ERR_MSP_INVALID_ID             (2)
#define  ERR_MSP_NO_INITILIZATION       (3)
#define  ERR_MSP_NOT_FOUND              (4)
#define  ERR_MSP_FULL                   (7)
#define  ERR_MSP_INSUFFICIENT_BUFFER    (8)
#define  ERR_MSP_CONSUMED               (9)
#define  ERR_MSP_CONTINUE               (10)
#define  ERR_MSP_TOO_SMALL_BUFFER       (11)
#define  ERR_MSP_NOT_FREEE_SPACE        (12)
#define  ERR_MSP_NO_MORE_DATA           (13)
#define  ERR_MSP_MALLOC_FAILUE          (15)
#define  ERR_MSP_NOT_ENOUGH_MEMORY      (16)
#define  ERR_MSP_INVALID_HEAP_ADDR      (17)
#define  ERR_MSP_SHUTDOWN               (18) /* δ�ܹػ�*/
#define  ERR_MSP_NOT_CONNECT            (19) /* δ��ȷ����UE*/
#define  ERR_MSP_BUSY                   (20) /* ϵͳæ����ʹ�ó�ͻ*/
#define  ERR_MSP_PARA_NULL              (21) /* ����ָ��ΪNULL */


#define  ERR_MSP_UNAVAILABLE            (50)
#define  ERR_MSP_TIMEOUT                (51)
#define  ERR_MSP_INIT_FAILURE           (52)
#define  ERR_MSP_IO_REGISTER_FAILURE    (53)
#define  ERR_MSP_NO_REGISTER            (54)

#define  ERR_MSP_FLASH_ERASE_ERROR      (100)
#define  ERR_MSP_FLASH_WRITE_ERROR      (101)
#define  ERR_MSP_FLASH_ALIGN_ERROR      (102)             /* sector aligned addresses*/
#define  ERR_MSP_FLASH_PROTECT_ERROR    (103)
#define  ERR_MSP_FLASH_BIN_LOAD_NEED_FIRST_BLOCK    (104) /* while bin file load,req need first block*/
#define  ERR_MSP_DIAG_CMD_SIZE_INVALID              (106)


#define  ERR_MSP_CFG_RAM_SUCCESS         (201)
#define  ERR_MSP_CFG_PART_SUCCESS        (202)
#define  ERR_MSP_CFG_LOG_NOT_ALLOW       (203)
#define  ERR_MSP_DIAG_CYC_CMD_NOT_START  (204)
#define  ERR_MSP_DIAG_MSG_CFG_NOT_SET    (205)
#define  ERR_MSP_QUEUE_FULL              (300)
#define  ERR_MSP_QUEUE_NOT_EXIST         (301)
#define  ERR_MSP_INVALID_TASK_ID         (302)
#define  ERR_MSP_INVALID_TASK_QUEUE      (303)

#define  ERR_MSP_VCOM_WRITE_ERROR        (401)

/* AT�����룬���Ժ��ַ�����Ӧ*/
/* �ô������MSP�ڲ�������ͳد����*/
#define  ERR_MSP_NOT_EXIST               (500) /* �ַ�����"δ����"   ��Undone?*/
#define  ERR_MSP_UNKNOWN                 (501) /* �ַ�����"δ֪����" ��unknown error?*/
#define  ERR_MSP_INVALID_PARAMETER       (502) /* �ַ�����"��������Ƿ����������ںϷ���Χ�ȴ���" "invalid parameters"*/
#define  ERR_MSP_NV_NOT_SUPPORT_ID       (520) /* �ַ�����"NV����]""NVIM Not Exist"*/
#define  ERR_MSP_NV_ERROR_READ           (521) /* �ַ�����"NV��ȡʧ��" "Read NVIM Failure"*/
#define  ERR_MSP_NV_NOT_SUPPORT_LENTH    (522) /* �ַ�����"NVдʧ�ܣ����ȹ���""Write Error for Length Overflow"*/
#define  ERR_MSP_NV_BAD_BLOCK            (523) /* �ַ�����"NVдʧ�ܣ�Flash����" "Write Error for Flash Bad Block"*/
#define  ERR_MSP_NV_ERROR_WRITE          (524) /* �ַ�����"NVдʧ�ܣ���������" "Write Error for Unknown Reason"*/
#define  ERR_MSP_VCTCXO_OVER_HIGH        (525) /* �ַ�����"����VCTCXO��ѹ����" "Higher Voltage"	*/
#define  ERR_MSP_UE_MODE_ERR             (526) /* �ַ�����"����ģʽ����(��������ģʽ)"	"UE Mode Error"	*/
#define  ERR_MSP_NOT_SET_CHAN            (527) /* �ַ�����"û����������ŵ�""Not Set Appointed Channel"	*/
#define  ERR_MSP_OPEN_RFTX_ERROR         (528) /* �ַ�����"�򿪷����ʧ��""Open TX Transmitter Failure"	*/
#define  ERR_MSP_OPEN_RFRX_ERROR         (529) /* �ַ�����"�򿪽��ջ�ʧ��""Open RX Transmitter Failure"	*/
#define  ERR_MSP_OPEN_CHAN_ERROR         (530) /* �ַ�����"ͨ����ʧ��""Open Channel Failure"	*/
#define  ERR_MSP_CLOSE_CHAN_ERROR        (531) /* �ַ�����"ͨ���ر�ʧ��""Close Channel Failure"	*/
#define  ERR_MSP_NOT_SUPPORT             (532) /* �ַ�����"��֧�� */
#define  ERR_MSP_INVALID_OP              (533) /* �ַ�����"�Ƿ������������ȡ����NV��""Invalid Operation"	*/
#define  ERR_MSP_ENTER_BAND_ERROR        (534) /* �ַ�����"����Ƶ����Ϣ�޷���Ӧ" "Band No Match"	*/
#define  ERR_MSP_SET_CHAN_INFOR_ERROR    (535) /* �ַ�����"�����ŵ���Ϣʧ��" "Set Channel Information Failure"	*/
#define  ERR_MSP_FRE_CHANI_NOTMATCH      (536) /* �ַ�����"����Ƶ�κ��ŵ���Ϣ�޷����""Band And Channel Not Combinated"	*/
#define  ERR_MSP_SET_TX_POWER_FAILUE     (537) /* �ַ�����"����TX�ķ��书��ʧ��""Set TX Transmitter Power Error"	*/
#define  ERR_MSP_SET_PA_LEVEL_FAILUE     (538) /* �ַ�����"����PA��ʧ��""Set PA Level Failure"	*/
#define  ERR_MSP_NOT_SET_CURRENT_CHAN    (539) /* �ַ�����"δ���õ�ǰ�ŵ�"	"Not Set Current Channel"*/
#define  ERR_MSP_NOT_ABLETO_RD_APC       (540) /* �ַ�����"�޷���ȡ��ǰ���õ�APC��"	"APC Value Can't Be Read"*/
#define  ERR_MSP_SET_APC_ERROR           (541) /* �ַ�����"����APC����ʧ��"	"Write APC Failure"*/
#define  ERR_MSP_RD_APC_ERROR            (542) /* �ַ�����"��ȡAPC����ʧ��"	"Read APC Failure"*/
#define  ERR_MSP_SET_LINA_ERROR          (543) /* �ַ�����"����LNAʧ��"	"Set AAGC Failure"*/
#define  ERR_MSP_NOT_OPEN_RXRX           (544) /* �ַ�����"û�д򿪽���"	"RX Transmitter Not Open"*/
#define  ERR_MSP_NOT_OPEN_RXTX           (545) /* �ַ�����"û�д򿪷���"	"TX Transmitter Not Open"*/
#define  ERR_MSP_NO_SIGNAL               (546) /* �ַ�����"���ź�����򗴲ⲻ���Ņ�" "No Signal"	*/
#define  ERR_MSP_PHYNUM_LENGTH_ERROR     (547) 
#define  ERR_MSP_PHYNUM_INVALID          (548) /* �ַ�����"����Ų��Ϸ�" "Invalid PHY Number"	*/
#define  ERR_MSP_PHYNUM_TYPE_ERROR       (549) 
#define  ERR_MSP_PLATINFOR_NOTABLE_TORD  (550) /* �ַ�����"�޷���ȡƽ̨��Ϣ""Platform Information Can't Be Read"	*/
#define  ERR_MSP_WRT_NOT_UNLOCK          (551) /* �ַ�����"��ʾд����ʱ�����ݱ���δ����, �޷�������������"	"Not unlock, write error"*/
#define  ERR_MSP_CALLNUM_OVER20          (552) /* �ַ�����"����ĺŶζ���20��"	"Card Number Morethan 20"*/
#define  ERR_MSP_MNC_ENTER_OVERFLOW      (553) /* �ַ�����"<mnc-digital-num>���볬����Χ" "<mnc-digital-num> Enter Error"	*/
#define  ERR_MSP_RD_VER_FAILUE           (554) /* �ַ�����"�汾��Ϣ��ȡʧ��""Read VERSION Failure"	*/
#define  ERR_MSP_OPERTION_ERROR          (555) /* �ַ�����"����ʧ��""Operation Failure"*/
#define  ERR_MSP_PWD_ERR                 (556) /* �ַ�د: "�����������""Password Error"*/
#define  ERR_MSP_TIME_OUT                (557) /* �ַ�د: "��ʱ����" "Overtime"*/
#define  ERR_MSP_NO_MORE_MEMORY	         (558) /* �ڴ治�� "No Memory"*/
#define  ERR_MSP_SIMM_LOCK               (559) /* ������SIMLOCK����"Sim Lock Active"*/
#define  ERR_MSP_CLOSE_RFTX_ERROR        (560) /* �ַ�����"�رշ����ʧ��""Close TX Transmitter Failure"	*/
#define  ERR_MSP_CLOSE_RFRX_ERROR        (561) /* �ַ�����"�رս��ջ�ʧ��""Close RX Transmitter Failure"*/
#define  ERR_MSP_NV_DATA_INVALID         (562) /* �ַ�����"��ȡ��NV�����ݷǷ�""NV DATA INVALID"*/
#define  ERR_MSP_AT_CHANNEL_CLOSE        (563) /* DIAG/MUX��ATͨ���ر�*/
#define  ERR_MSP_AT_CHANNEL_BUSY         (564) /* DIAG/MUX��ATͨ��BUSY*/
#define  ERR_MSP_OS_MSG_BODY_NULL        (565) /* ���յ�����Ϣ��ΪNULL(��Ӧ��ΪNULL)*/
#define  ERR_MSP_AT_ACK_URC_INVALID      (566) /* AT ACK or URC invalid, such as size is zero*/
#define  ERR_MSP_PS_START_FAILURE        (567) /* START PS FAILURE*/
#define  ERR_MSP_PS_STOP_FAILURE         (568) /* STOP PS FAILURE*/
#define  ERR_MSP_PS_SET_APN_FAILURE      (569) /* SET APN FAILURE*/
#define  ERR_MSP_PS_SET_AUTH_FAILURE     (570) /* SET AUTH FAILURE*/
#define  ERR_MSP_PS_SET_BEARER_TYPE_FAILURE      (571) /* SET BEARER TYPE*/


#define  ERR_MSP_BNADSW_ERROR            (572) /* �ַ���: "BAND�л�����"*/
#define	 ERR_MSP_TXWAVE_ERROR	         (573) /* �ַ���: "���䲨�����ʹ���"*/
#define  ERR_MSP_AT_FW_TABLEID_EXIST     (574) /* client id �Ѵ�]*/
#define  ERR_MSP_START_TIMER_FAIL           (575)

/* add simm error number (���ނs/67193/2009-08-14)*/
/* 600 ~ 699 should be used by simm*/
#define  ERR_MSP_SIMM_NOT_INIT             (601)          /*SIMMδ��ʼ��*/
#define  ERR_MSP_SIMM_WRONG_PARA           (602)          /*��Ϊ�������󣬺���ִ��ʧ��*/
#define  ERR_MSP_SIMM_NOAPP                (603)          /*��Ϊû�п���Ӧ�ã�����ִ��ʧ��*/
#define  ERR_MSP_SIMM_NO_MEM               (604)          /*��Ϊ��ʼ��û����ɣ�����ִ��ʧ��*/
#define  ERR_MSP_SIMM_ALLOCMEM_FAILED      (605)          /*��Ϊ�ڴ����ʧ�ܣ�����ִ��ʧ��*/
#define  ERR_MSP_SIMM_SENDMSG_FAILED       (606)          /*��Ϊ��Ϣ��?ʧ�ܣ�����ִ��ʧ��*/
#define  ERR_MSP_SIMM_TIMEOUT              (607)          /*��ʱ,����ִ��ʧ��*/
#define  ERR_MSP_SIMM_NO_CARD              (608)          /*������λʧ��*/
#define  ERR_MSP_SIMM_SEM                  (609)          /*��������ź�������*/

#define  ERR_MSP_SIMM_QUEUE_FULL           (621)          /*��������*/
#define  ERR_MSP_SIMM_QUEUE_EMPTY          (622)          /*����Ϊ��*/
#define  ERR_MSP_SIMM_CMD_EXCEPTION        (623)          /*�����ʧ��*/
#define  ERR_MSP_SIMM_CMD_TIMEOUT          (624)          /*�����ʱ*/

#define  ERR_MSP_SIMM_DF_FCP_FAILED        (630)          /*DF�ļ�����ʧ��*/
#define  ERR_MSP_SIMM_PIDO_FAILED          (631)          /*PIDO����ʧ��*/
#define  ERR_MSP_SIMM_FCP_FAILED           (632)          /*EF�ļ�����ʧ��*/
#define  ERR_MSP_SIMM_EFDIR_FAILED         (632)          /*EF�ļ�����ʧ��*/

#define  ERR_MSP_SIMM_NEED_PIN             (640)
#define  ERR_MSP_SIMM_NEED_PIN2            (641)
#define  ERR_MSP_SIMM_NEED_PUK             (642)
#define  ERR_MSP_SIMM_NEED_PUK2            (643)

#define  ERR_MSP_SIMM_AUTH_MAC_FAILURE     (645)          /*��ȨMAC����*/
#define  ERR_MSP_SIMM_AUTH_SYNC_FAILURE    (646)          /*��ȨSYNC����*/
#define  ERR_MSP_SIMM_AUTH_OTHER_FAILURE   (647)          /*������Ȩ����*/
#define  ERR_MSP_SIMM_OTHER_FAILURE        (648)          /*������Ȩ����*/


#define  ERR_MSP_QUEUE_NOT_OPEN          (1000)
#define  ERR_MSP_QUEUE_UNAVAILABLE       (1001)
#define  ERR_MSP_QUEUE_SHUTDOWN          (1002)

#define  ERR_MSP_NV_ITEM_IS_EMPTY        (1100)
#define  ERR_MSP_NV_FLASH_FULL           (1103)

/* ��ӦBSP����m*/
#define  ERR_MSP_NV_GOOD_BLOCK           (1105)
#define  ERR_MSP_NV_ERROR_INIT           (1106)
#define  ERR_MSP_NV_ERROR_ARGS           (1107)
#define  ERR_MSP_NV_ERROR_MALLOC         (1108)
#define  ERR_MSP_NV_ERROR_ERASE          (1111)

/* ��������m*/
#define  ERR_MSP_SUM_ONLY_NVCOPY_OK      (1200)
#define  ERR_MSP_SUM_BACK_OK             (1201)
#define  ERR_MSP_SUM_BACK_FAIL           (1202)
#define  ERR_MSP_SUM_BACK_NO_VERSION     (1203)
#define  ERR_MSP_SUM_UPDATE_RESET_FAILUE (1204)

#define  ERR_MSP_NVIM_BACKUP_FILE_FAIL    (2001)
#define  ERR_MSP_NVIM_BACKUP_DATA_FAIL    (2002)
#define  ERR_MSP_NVIM_WRITE_FILE_FAIL     (2003)
#define  ERR_MSP_NVIM_UPDATA_DATA_FAIL    (2004)
#define  ERR_MSP_NVIM_UPDATA_FILE_FAIL    (2005)
#define  ERR_MSP_NVIM_SECTION_NOT_SUPPORT_UPDATA    2006
#define  ERR_MSP_NVIM_NOT_SUPPORT_WRITE    2007
#define  ERR_MSP_NVIM_NEED_BACK_ERR             2008

#define MSP_SD_OPEN_FILE_ERROR      (2200)
#define	MSP_SD_WRITE_FILE_ERROR     (2201)
#define	MSP_SD_SEEK_FILE_ERROR      (2202)
#define	MSP_SD_GET_STATE_ERROR      (2203)
#define MSP_SD_CREAT_DIR_ERROR      (2204)


/*CDM ERR NO. START*/

#define ERR_MSP_CDM_BEGIN               (0x3000)

#define ERR_MSP_CDM_ACPU_CODER_SRC_FAIL (ERR_MSP_CDM_BEGIN + 1)

#define ERR_MSP_CDM_END                 (0x3100)
/*CDM ERR NO. END*/

/*scm ERR NO. START*/
#define ERR_MSP_SCM_BEGIN               (0x5000)
#define ERR_MSP_SCM_START_SOCP_FAIL     (ERR_MSP_SCM_BEGIN + 1)

/*scm ERR NO. END*/

/* DIAG����������ظ��Ĵ����� 0x4000-0x4fff */
#define  ERR_MSP_DIAG_ERROR_BEGIN               (0x4000)
#define  ERR_MSP_DIAG_ACORE_ERROR               (ERR_MSP_DIAG_ERROR_BEGIN + 1)  /* A�˴���ʧ�� */
#define  ERR_MSP_DIAG_CCORE_ERROR               (ERR_MSP_DIAG_ERROR_BEGIN + 2)  /* C�˴���ʧ�� */
#define  ERR_MSP_DIAG_INVALID_CMD               (ERR_MSP_DIAG_ERROR_BEGIN + 3)  /* ����ʶ���������� */
#define  ERR_MSP_DIAG_ERRPID_CMD                (ERR_MSP_DIAG_ERROR_BEGIN + 4)  /* ͸������Ľ���PID��֧�� */
#define  ERR_MSP_DIAG_UNKNOWNPID_CMD            (ERR_MSP_DIAG_ERROR_BEGIN + 5)  /* ͸������Ľ���PIDδ֪ */
#define  ERR_MSP_DIAG_SAMPLE_START_FAIL         (ERR_MSP_DIAG_ERROR_BEGIN + 6)  /* ������������ʧ�ܴ���m */
#define  ERR_MSP_DIAG_TL_SEND_MSG_FAIL          (ERR_MSP_DIAG_ERROR_BEGIN + 7)  /* ������������TL��?��Ϣʧ�� */
#define  ERR_MSP_DIAG_GUC_SEND_MSG_FAIL         (ERR_MSP_DIAG_ERROR_BEGIN + 8)  /* ������������GUC��?��Ϣʧ�� */
#define  ERR_MSP_DIAG_EASYRF_SEND_MSG_FAIL      (ERR_MSP_DIAG_ERROR_BEGIN + 9)  /* ������������EasyRF��?��Ϣʧ�� */
#define  ERR_MSP_DIAG_ALLOC_MALLOC_FAIL         (ERR_MSP_DIAG_ERROR_BEGIN + 10)  /* �ڴ����ʧ�ܷ�?��Ϣʧ�� */
#define  ERR_MSP_DIAG_SEND_MSG_FAIL             (ERR_MSP_DIAG_ERROR_BEGIN + 11)  /* ��?��Ϣʧ�� */
#define  ERR_MSP_DIAG_NV_NUM_ERR                (ERR_MSP_DIAG_ERROR_BEGIN + 12)  /* nv id�������� */
#define  ERR_MSP_DIAG_GET_NVLEN_ERR             (ERR_MSP_DIAG_ERROR_BEGIN + 13)  /* nv id�������� */
#define  ERR_MSP_DIAG_GET_NV_LIST_ERR           (ERR_MSP_DIAG_ERROR_BEGIN + 14)  /* ��ȡnv id list���� */
#define  ERR_MSP_DIAG_WRITE_NV_ERR              (ERR_MSP_DIAG_ERROR_BEGIN + 15)  /* дNV���� */
#define  ERR_MSP_DIAG_NOT_AUTH_NV_ERR           (ERR_MSP_DIAG_ERROR_BEGIN + 16)  /* ��Nv�����Ա���д */
#define  ERR_MSP_DIAG_FLUSH_NV_ERR              (ERR_MSP_DIAG_ERROR_BEGIN + 17)  /* fush nv���� */
#define  ERR_MSP_DIAG_CB_NULL_ERR               (ERR_MSP_DIAG_ERROR_BEGIN + 18)  /* �ص�����Ϊ�մ��� */
#define  ERR_MSP_DIAG_WRITE_MAILBOX_ERR         (ERR_MSP_DIAG_ERROR_BEGIN + 19)  /* �ص�����Ϊ�մ��� */
#define  ERR_MSP_DIAG_CHANN_CONN_FAIL           (ERR_MSP_DIAG_ERROR_BEGIN + 20)
#define  ERR_MSP_DIAG_CONNECT_TIME_OUT          (ERR_MSP_DIAG_ERROR_BEGIN + 21)
#define  ERR_MSP_DIAG_REPEAT_REGISTER           (ERR_MSP_DIAG_ERROR_BEGIN + 22)
#define  ERR_MSP_INVALID_RESULT                 (ERR_MSP_DIAG_ERROR_BEGIN + 23)
#define  ERR_MSP_REPEAT_CONNECT_CNF             (ERR_MSP_DIAG_ERROR_BEGIN + 24)
#define  ERR_MSP_CONN_SN_ERROR                  (ERR_MSP_DIAG_ERROR_BEGIN + 25)
#define  ERR_MSP_CHNNEL_NUM_ERROR               (ERR_MSP_DIAG_ERROR_BEGIN + 26)
#define  ERR_MSP_INALID_LEN_ERROR               (ERR_MSP_DIAG_ERROR_BEGIN + 27) /* �����·������ݳ��ȷǛ� */
#define  ERR_MSP_MEMCPY_S_ERROR                 (ERR_MSP_DIAG_ERROR_BEGIN + 28) /* memcpy���� */
#define  ERR_MSP_MSG_ID_ERROR                   (ERR_MSP_DIAG_ERROR_BEGIN + 29) /* msgid���� */
#define  ERR_MSP_GET_VIRT_ADDR_FAIL             (ERR_MSP_DIAG_ERROR_BEGIN + 30)
#define  ERR_MSP_WRITE_DONE_FAIL                (ERR_MSP_DIAG_ERROR_BEGIN + 31)
#define  ERR_MSP_STOP_SOCP_FAIL                 (ERR_MSP_DIAG_ERROR_BEGIN + 33)
#define  ERR_MSP_GET_WRITE_BUFF_FAIL            (ERR_MSP_DIAG_ERROR_BEGIN + 34)
#define  ERR_MSP_CLEAR_BUFF_FAIL                (ERR_MSP_DIAG_ERROR_BEGIN + 35)
#define  ERR_MSP_CONNECT_AUTH_FAIL              (ERR_MSP_DIAG_ERROR_BEGIN + 36)
#define  ERR_MSP_GEN_AUTH_HASH_FAIL             (ERR_MSP_DIAG_ERROR_BEGIN + 37)
#define  ERR_MSP_REPEAT_AUTH_HASH_FAIL          (ERR_MSP_DIAG_ERROR_BEGIN + 38)
#define  ERR_MSP_DEC_RSA_DATA_FAIL              (ERR_MSP_DIAG_ERROR_BEGIN + 39)
#define  ERR_MSP_DEC_RSA_SIZE_FAIL              (ERR_MSP_DIAG_ERROR_BEGIN + 40)
#define  ERR_MSP_PORT_SWITCH_FAIL               (ERR_MSP_DIAG_ERROR_BEGIN + 41)
#define  ERR_MSP_PCDEV_LINK_DOWN                (ERR_MSP_DIAG_ERROR_BEGIN + 42)

/*error no end**********************************************************************/

typedef enum {
    DIAG_PHY_POWER_ON_DOING = 0x1010,
    DIAG_PHY_POWER_ON_SUCCESS = 0x1011,
    DIAG_PHY_POWER_ON_FAIL = 0x1012
}mdrv_diag_phy_state_e;


typedef enum {
    DIAGLOG_SRC_MNTN = 0,
    DIAGLOG_DST_MNTN = 1,
    DIAGLOG_MNTN_BUTT = 2,
}diag_mntn_e;

/* diagԴ��ά����Ϣ�ϱ�ͳһ�ṹʹ */
#define DIAGLOG_MNTN_SRC_INFO   unsigned int    ulChannelId;        /* ͨ��ID */\
                                char            chanName[12];       /* ͨ������ */\
                                unsigned int    ulDeltaTime;        /* �ϱ�ʱ���� slice */\
                                unsigned int    ulPackageLen;       /* �ϱ�ʱ������ϱ������ݰ��ܳ��� byte */\
                                unsigned int    ulPackageNum;       /* �ϱ�ʱ������ϱ������ݰ��ܳ��� */\
                                unsigned int    ulAbandonLen;       /* �ϱ�ʱ����ڱ��������ݳ��� byte*/\
                                unsigned int    ulAbandonNum;       /* �ϱ�ʱ����ڱ��������ݴ��� */\
                                unsigned int    ulThrputEnc;        /* �ϱ������ڱ���Դ������,byte/s */\
                                unsigned int    ulOverFlow50Num;    /* �ϱ������ڱ���Դbuffռ�ó���50%�Ĵ��� */\
                                unsigned int    ulOverFlow80Num;    /* �ϱ������ڱ���Դbuffռ�ó���80%�Ĵ��� */



typedef struct {
    unsigned int overflow_50_num;
    unsigned int overflow_80_num;
    unsigned int abandon_num;
    unsigned int abandon_len;
    unsigned int package_len;
    unsigned int package_num;
}diag_debug_info_s;

/**********************************IND start*********************/
typedef struct {
    unsigned int module;		/* MDRV_DIAG_GEN_MODULE*/
    unsigned int pid;
    unsigned int event_id;		/* �¼�ID */
    unsigned int length;
    void *data;
}diag_event_ind_s;

typedef struct
{
    unsigned int module;       /* MDRV_DIAG_GEN_MODULE*/
    unsigned int pid;
    unsigned int msg_id;
    unsigned int direction;
    unsigned int length;
    void *data;
}diag_air_ind_s;

typedef struct
{
    unsigned int module;
    unsigned int pid;
    unsigned int msg_id;
    unsigned int reserved;
    unsigned int length;
    void *data;
}diag_trans_ind_s;

typedef struct
{
    unsigned int module;
    unsigned int pid;
    unsigned int msg_id;
    unsigned int reserved;
    unsigned int length;
    void *data;
}diag_dt_ind_s;

typedef struct {
    unsigned int sender_cpu;
    unsigned int sender_pid;
    unsigned int receive_cpu;
    unsigned int receive_pid;
    unsigned int msg_len;
    unsigned int msg_id;
    unsigned int phy_state;
}mdrv_diag_phy_state_msg_s;

/**********************************IND end*********************/

/* �����Ϣ��Ϊ�ϲ��ṩ�Ĳ����ᖨ */
typedef struct
{
    unsigned int        ssid;         /* ���ݲ�����CPU ID */
    unsigned int        msg_type;      /* ������� */
    unsigned int        mode;         /* ģʽ */
    unsigned int        sub_type;      /* �����ͣ�DIAG_MSG_SUB_TYPE_unsigned int */
    unsigned int        direction;    /* �ϱ���Ϣ�ķ��� */
    unsigned int        modem_id;
    unsigned int        msg_id;        /* ʹ16λ�Д� */
    unsigned int        trans_id;      /* TransId */
}diag_cnf_info_s;

void mdrv_diag_ptr(diag_ptr_id_e type, unsigned int para_mark, unsigned int para0, unsigned int para1);
unsigned int mdrv_get_thrput_info(diag_thrput_id_e type);
unsigned int mdrv_diag_shared_mem_write(unsigned int type, unsigned int len, const char *data);
unsigned int mdrv_diag_shared_mem_read(unsigned int type);
unsigned int mdrv_diag_debug_file_header(const void *file);
void mdrv_diag_debug_file_tail(const void *file, char *file_path);

/*****************************************************************************
 �� �� ��     : mdrv_diag_report_log
 ��������  :
 �������  :
*****************************************************************************/
unsigned int mdrv_diag_report_log(unsigned int module_id, unsigned int pid, char *file_name, unsigned int line_num, const char* fmt, va_list arg);
/*****************************************************************************
 �� �� ��     : mdrv_diag_report_trans
 ��������  : �ṹ�������ϱ��Ӆ�(�滻ԭ����DIAG_ReportCommand)
 �������  : mdrv_diag_trans_ind_s->module( 31-24:modemid,23-16:modeid )
             mdrv_diag_trans_ind_s->msg_id(͸������ID)
             mdrv_diag_trans_ind_s->length(͸����Ϣ�ĳ���)
             mdrv_diag_trans_ind_s->data(͸����Ϣ)
*****************************************************************************/
unsigned int mdrv_diag_report_trans(diag_trans_ind_s *trans_msg);
/*****************************************************************************
 �� �� ��  : mdrv_diag_event_report
 ��������  : �¼��ϱ��ӿ�
 �������  : mdrv_diag_event_ind_s->module( 31-24:modemid,23-16:modeid,15-12:level,11-0:pid )
             mdrv_diag_event_ind_s->event_id(event ID)
             mdrv_diag_event_ind_s->length(event�ĳ���)
             mdrv_diag_event_ind_s->data(event��Ϣ)
*****************************************************************************/
unsigned int mdrv_diag_report_event(diag_event_ind_s *event_msg);
/*****************************************************************************
 �� �� ��  : mdrv_diag_air_report
 ��������  : �տ���Ϣ�ϱ��ӿڣ���PSʹ��(�滻ԭ����DIAG_ReportAirMessageLog)
 �������  : mdrv_diag_air_ind_s->module( 31-24:modemid,23-16:modeid,15-12:level,11-0:pid )
             mdrv_diag_air_ind_s->msg_id(�տ���ϢID)
             mdrv_diag_air_ind_s->direction(�տ���Ϣ�ķ���)
             mdrv_diag_air_ind_s->length(�տ���Ϣ�ĳ���)
             mdrv_diag_air_ind_s->data(�տ���Ϣ��Ϣ)
*****************************************************************************/
unsigned int mdrv_diag_report_air(diag_air_ind_s *air_msg);
/*****************************************************************************
 �� �� ��     : mdrv_diag_report_trace
 ��������  : �����Ϣ�ϱ��ӿ�
 �������  : msg_data(��׼��VOS��Ϣ�壬Դģ��?Ŀ��ģ����Ϣ����Ϣ���л�ȡ)
*****************************************************************************/
unsigned int mdrv_diag_report_trace(void *trace_msg, unsigned int modem_id);
/*****************************************************************************
 �� �� ��     : mdrv_diag_disconn_reset
 ��������  : ��λdiag�������
 �������  : void
*****************************************************************************/
void mdrv_diag_report_reset(void);
/*****************************************************************************
 �� �� ��     : mdrv_diag_reset_mntn_info
 ��������  : ��λdiagά��ͳ����Ϣ
 �������  : void
*****************************************************************************/
void mdrv_diag_reset_mntn_info(diag_mntn_e  type);
/*****************************************************************************
 �� �� ��     : mdrv_diag_get_mntn_info
 ��������  : ��ȡά��ͳ����Ϣ
 �������  : void
*****************************************************************************/
void* mdrv_diag_get_mntn_info(diag_mntn_e  type);
/*****************************************************************************
 �� �� ��     : mdrv_diag_report_msg_trans
 ��������  : ��ȡά��ͳ����Ϣ
 �������  : void
*****************************************************************************/
unsigned int mdrv_diag_report_msg_trans(diag_trans_ind_s *trans_msg, unsigned int cmd_id);
/*****************************************************************************
 �� �� ��     : mdrv_diag_report_msg_trans
 ��������  : ��ȡά��ͳ����Ϣ
 �������  : void
*****************************************************************************/
unsigned int mdrv_diag_report_cnf(diag_cnf_info_s *diag_info, void *cnf_msg, unsigned int len);
/*****************************************************************************
 �� �� ��     : diag_report_dt
 ��������  : ·����Ϣ�ϱ��ӿ�
 �������  : mdrv_diag_dt_ind_s->module( 31-24:modemid,23-16:modeid,15-12:level,11-8:groupid )
                          mdrv_diag_dt_ind_s->msg_id(·������ID)
                          mdrv_diag_dt_ind_s->length(·����Ϣ�ĳ���)
                          mdrv_diag_dt_ind_s->data(·����Ϣ)
*****************************************************************************/
unsigned int mdrv_diag_report_dt(diag_dt_ind_s *dt_msg);

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif

