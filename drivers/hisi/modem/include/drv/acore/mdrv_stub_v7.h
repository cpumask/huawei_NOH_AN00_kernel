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
 *  @brief   acore历史桩接口对外公共头文件
 *  @file    mdrv_stub_v7.h
 *  @author  
 *  @version v1.0
 *  @date    2019.11.26
 *  @note    修改记录(版本号|修订日期|说明)
 *  <ul><li>v1.0|2019.11.26|头文件注释整改</li></ul>
 *  @since   DRV1.0
 *  @todo 文件待清理
 */
#ifndef __MDRV_ACORE_STUB_V7_H__
#define __MDRV_ACORE_STUB_V7_H__
#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief BSP 动态内存分配
 *
 * @par 描述:
 * BSP 动态内存分配
 *
 * @attention
 *
 * @param[in] sz:分配的大小(byte)
 * @param[in] flags: 内存属性(暂不使用,预留)
 *
 * @retval 指针型，分配出来的内存指针
 *
 * @todo 桩接口，待删除
 */
static inline void* BSP_MALLOC(unsigned int u32Size, mem_pool_type_e enFlags) {
    (void)u32Size;
    (void)enFlags;
    return 0;
}

/**
 * @brief BSP 动态内存释放
 *
 * @par 描述:
 * BSP 动态内存释放
 *
 * @attention
 *
 * @param[in] ptr: 动态内存指针
 *
 * @retval 无
 *
 * @todo 桩接口，待删除
 */
static inline void  BSP_FREE(void* pMem) 
{
}

/**
 * @brief 获取释放内存大小
 *
 * @par 描述:
 * 获取释放内存大小
 *
 * @attention 无
 *
 * @param 无
 *
 * @retval 释放内存大小
 *
 * @todo 桩接口，待删除
 */
static inline unsigned int FREE_MEM_SIZE_GET(void)
{
    return 0;
}
/**
 * @brief 获取释放内存大小
 *
 * @par 描述:
 * 获取释放内存大小
 *
 * @attention 无
 *
 * @param 无
 *
 * @retval 释放内存大小
 *
 * @todo 桩接口，待删除
 */
static inline int BSP_DLOAD_GetDloadType(void)
{
    return 1;
}

/*liuyi++ only for nas marenkun temp*/

/**
 * @brief 设置timer4作为唤醒源
 *
 * @par 描述:
 * 设置timer4作为唤醒源
 *
 * @attention 无
 *
 * @param 无
 *
 * @retval 无
 *
 * @todo 桩接口，待删除
 */
static inline void BSP_PWC_SetTimer4WakeSrc(void)
{
    return;
}

/**
 * @brief 设置timer4不作为唤醒源
 *
 * @par 描述:
 * 设置timer4不作为唤醒源
 *
 * @attention 无
 *
 * @param 无
 *
 * @retval 无
 *
 * @todo 桩接口，待删除
 */
static inline void BSP_PWC_DelTimer4WakeSrc(void)
{
    return;
}

/**
 * @brief AT^PSTANDBY
 *
 * @par 描述:
 * AT^PSTANDBY
 *
 * @attention 无
 *
 * @todo 桩接口，待删除
 */
static inline unsigned int DRV_PWRCTRL_STANDBYSTATEACPU(unsigned int ulStandbyTime, unsigned int ulSwitchtime)
{
    (void)ulStandbyTime;
    (void)ulSwitchtime;
    return 0;
}

/*dload begin*/

/* Begin: DRV为OAM实现NV备份恢复流程而提供的接口 */
/**
 * @brief OAM通过该接口告诉底软NV升级成功还是失败
 *
 * @par 描述:
 * OAM通过该接口告诉底软NV升级成功还是失败
 *
 * @attention 
 * <ul><li>V3R3 SFT 和 V9 BBIT 打桩</li></ul>
 * <ul><li>V9 SFT  实现</li></ul>
 * <ul><li>如果NV升级成功，底软往fastboot共享区置成功标识</li></ul>
 * <ul><li>如果NV升级失败，底软往fastboot共享区置失败标识</li></ul>
 *
 * @param[in] status：0 表示NV升级成功;其他值 表示NV升级失败
 *
 * @retval 0:  操作成功；-1：操作失败。
 *
 * @todo 桩接口，待删除
 */
static inline int DRV_UPGRADE_NV_SET_RESULT(int status)
{
    (void)status; 
    return 0;
}

/**
 * @brief 对文件进行校验
 *
 * @par 描述:
 * 对文件进行校验
 *
 * @attention 
 * <ul><li>目前条件不具备，直接打桩，以后实现
 *
 * @param[in] pnvFilePath：待校验文件的路径
 *
 * @retval 0 :文件校验正确   -1：文件校验失败
 *
 * @todo 桩接口，待删除
 */
static inline int DRV_NV_FILE_SEC_CHECK(char * pnvFilePath)
{
    (void)pnvFilePath;
    return 0;
}

/**
 * @brief 从Flash中的NV项备份区读取数据，实现NV项的恢复功能。
 *
 * @par 描述:
 * 从Flash中的NV项备份区读取数据，实现NV项的恢复功能。
 *
 * @attention 
 * <ul><li>V3R3 SFT 和 V9 BBIT 保持原样
 * <ul><li>V9 SFT  将调用nand接口修改为调用emmc接口读写NV备份区
 *
 * @param[in]  len：从NV项备份区起始处开始，需要读取的NV项长度（字节数）。
 * @param[out] pRamAddr：目的RAM地址，用于存放读出的NV项数据
 *
 * @retval 0 :操作成功；   -1：操作失败。
 *
 * @todo 桩接口，待删除
 */
static inline int DRV_NVBACKUP_READ(unsigned char *pRamAddr, unsigned int len)
{
    (void)pRamAddr;
    (void)len;
    return 0;
}

/********************************************************************************************************
 函 数 名  : BSP_DLOAD_NVBackupWrite
 功能描述  : 将特定数据写入Flash中的NV项备份区，实现NV项的备份功能。
 输入参数  : pRamAddr：源RAM地址，用于存放需要写入的NV项数据。
             len：从NV项备份区起始处开始，需要写入的NV项长度（字节数）。
 输出参数  : 无。
 返 回 值  : 0:  操作成功；
             -1：操作失败。
 其他注意  : V3R3 SFT 和 V9 BBIT 保持原样
             V9 SFT  将调用nand接口修改为调用emmc接口读写NV备份区

 修改历史  :
 1.日    期   : 2012年12月6日
   作    者   : 蔡喜 00220237
   修改内容   : 修改函数
********************************************************************************************************/

static inline int DRV_NVBACKUP_WRITE(unsigned char *pRamAddr, unsigned int len)
{
    (void)pRamAddr;
    (void)len;
    return 0;
}
/**@todo 桩接口，待删除*/
#define UART_IOCTL_GET_RD_BUFF       0x7F001011
/**@todo 桩接口，待删除*/
#define UART_IOCTL_SET_READ_CB       0x7F001001

/**@todo 桩接口，待删除*/
typedef enum
{
    WWAN_WCDMA    = 1,/*WCDMA模式*/
    WWAN_CDMA    = 2  /*CDMA模式*/
}NDIS_WWAN_MODE;

/*管脚信号的定义*/
#define IO_CTRL_FC                      (0x02)
#define IO_CTRL_DSR                     (0x20)
#define IO_CTRL_DTR                     (0x01)
#define IO_CTRL_RFR                     (0x04)
#define IO_CTRL_CTS                     (0x10)
#define IO_CTRL_RI                      (0x40)
#define IO_CTRL_DCD                     (0x80)

#define DRV_GPIO_USB_SWITCH     200
#define DRV_GPIO_HIGH           1
#define DRV_GPIO_LOW            0

#define AT_USB_SWITCH_SET_VBUS_INVALID          2

/* 设备枚举最大端口个数 */
#define DYNAMIC_PID_MAX_PORT_NUM        17

/* NV项50091结构，代表设备将要枚举的端口形态 */
typedef struct
{
    unsigned int ulStatus;
    unsigned char aucFirstPortStyle[DYNAMIC_PID_MAX_PORT_NUM];  /* 设备切换前端口形态 */
    unsigned char aucRewindPortStyle[DYNAMIC_PID_MAX_PORT_NUM]; /* 设备切换后端口形态 */
    unsigned char reserved[22];
}DRV_DYNAMIC_PID_TYPE_STRU;
/**@todo 桩接口，待删除*/
typedef void(* USB_NET_DEV_SWITCH_GATEWAY)(void);
/**@todo 桩接口，待删除*/
static inline int DRV_OS_STATUS_SWITCH(int enable)
{
    (void)enable;
    return 0;
}
/**@todo 桩接口，待删除*/
static inline int DRV_GET_LINUXSYSTYPE(void)
{
    return -1;
}
/**@todo 桩接口，待删除*/
static inline unsigned int DRV_SET_PORT_QUIRY(DRV_DYNAMIC_PID_TYPE_STRU *pstDynamicPidType)
{
    (void)pstDynamicPidType;
    return 0;
}
/**@todo 桩接口，待删除*/
static inline int DRV_USB_NAS_SWITCH_GATEWAY_REGFUNC(USB_NET_DEV_SWITCH_GATEWAY switchGwMode)
{
    (void)switchGwMode;
    return 0;
}
/**@todo 桩接口，待删除*/
static inline unsigned int DRV_GET_U2DIAG_DEFVALUE(void)
{
    return 0;
}
/**@todo 桩接口，待删除*/
static inline int DRV_GET_DIAG_MODE_VALUE(unsigned char *pucDialmode,
                                          unsigned char *pucCdcSpec)
{
    (void)pucDialmode;
    (void)pucCdcSpec;
    return 0;
}
/**@todo 桩接口，待删除*/
static inline int DRV_USB_PHY_SWITCH_SET(unsigned char ucValue)
{
    (void)ucValue;
    return 0;
}
/**@todo 桩接口，待删除*/
static inline int DRV_USB_PORT_TYPE_VALID_CHECK(unsigned char *pucPortType,
                                                unsigned int ulPortNum)
{
    (void)pucPortType;
    (void)ulPortNum;
    return 0;
}
/**@todo 桩接口，待删除*/
static inline int DRV_UDIAG_VALUE_CHECK(unsigned int DiagValue)
{
    (void)DiagValue;
    return 0;
}
/**@todo 桩接口，待删除*/
static inline int DRV_U2DIAG_VALUE_CHECK(unsigned int DiagValue)
{
    (void)DiagValue;
    return 0;
}
/**@todo 桩接口，待删除*/
static inline unsigned char DRV_GET_PORT_MODE(char*PsBuffer, unsigned int*Length)
{
    (void)PsBuffer;
    (void)Length;
    return 0;
}
/**@todo 桩接口，待删除*/
static inline int DRV_SET_PID(unsigned char u2diagValue)
{
    (void)u2diagValue;
    return 0;
}
/**@todo 桩接口，待删除*/
static inline int DRV_GPIO_SET(unsigned char ucGroup, unsigned char ucPin, unsigned char ucValue)
{
    (void)ucGroup;
    (void)ucPin;
    (void)ucValue;
    return 0;
}
/**@todo 桩接口，待删除*/
static inline void DRV_AT_SETAPPDAILMODE(unsigned int ulStatus)
{
    (void)ulStatus;
    return;
}
/*************************************************
 函 数 名   : BSP_MNTN_ProductTypeGet
 功能描述   : 返回当前产品类型
 输入参数   : 无
 输出参数   : 无
 返 回 值   :0:STICK
             1:MOD
             2:E5
             3:CPE
*************************************************/
/**@todo 桩接口，待删除*/
static inline unsigned int BSP_MNTN_ProductTypeGet(void)
{
	return 0;
}
/**@todo 桩接口，待删除*/
#define DRV_PRODUCT_TYPE_GET()   BSP_MNTN_ProductTypeGet()
/**@todo 桩接口，待删除*/
static inline int DRV_SDMMC_GET_STATUS(void)
{
    return 0;
}

/*om use begin*/
/*VERSIONINFO_I数据结构中版本字符串最大有效字符长度*/
/**@todo 桩接口，待删除*/
#define VER_MAX_LENGTH                  30
/**@todo 桩接口，待删除*/
typedef struct
{
    unsigned char CompId;              /* 组件号：参见COMP_TYPE */
    unsigned char CompVer[VER_MAX_LENGTH+1];         /* 最大版本长度 30 字符+ \0 */
}VERSIONINFO_I;
/**@todo 桩接口，待删除*/
typedef enum{
    ADDRTYPE8BIT,
    ADDRTYPE16BIT,
    ADDRTYPE32BIT
}ENADDRTYPE;
/**@todo 桩接口，待删除*/
typedef struct tagBSP_OM_NET_S
{
    unsigned int u32NetRxStatOverFlow;       /* 接收FIFO溢出统计计数 */
    unsigned int u32NetRxStatPktErr;         /* 接收总错包计数 */
    unsigned int u32NetRxStatCrcErr;         /* 接收CRC错包计数 */
    unsigned int u32NetRxStatLenErr;         /* 接收无效长度包计数 */
    unsigned int u32NetRxNoBufInt;           /* 接收没有BUFFER中断计数 */
    unsigned int u32NetRxStopInt;            /* 接收停止中断计数 */
    unsigned int u32NetRxDescErr;            /* 接收描述符错误 */

    unsigned int u32NetTxStatUnderFlow;      /* 发送FIFO下溢统计计数 */
    unsigned int u32NetTxUnderFlowInt;       /* 发送FIFO下溢中断计数 */
    unsigned int u32NetTxStopInt;            /* 发送停止中断计数 */
    unsigned int u32NetTxDescErrPs;          /* 发送描述符错误(Ps) */
    unsigned int u32NetTxDescErrOs;          /* 发送描述符错误(Os) */
    unsigned int u32NetTxDescErrMsp;         /* 发送描述符错误(Msp) */

    unsigned int u32NetFatalBusErrInt;      /* 总线错误*/
}BSP_OM_NET_S;

/*om use end*/

/*taf used*/
/**@todo 桩接口，待删除*/
enum SECURE_ENABLE_STATUS_I
{
    SECURE_DISABLE = 0,
    SECURE_ENABLE = 1
};
/**@todo 桩接口，待删除*/
static inline int DRV_FILE_GET_DISKSPACE(const char *path,unsigned int *DskSpc,unsigned int *UsdSpc,  unsigned int *VldSpc)
{
    (void)path;
    (void)DskSpc;
    (void)UsdSpc;
    (void)VldSpc;

    return 0;
}
/*liuyi--*/

extern unsigned int omTimerGet(void);
/**@todo 桩接口，待删除*/
#define DRV_GET_SLICE() omTimerGet()

/*****************************************************************************
*  Function:  DRV_OLED_CLEAR_WHOLE_SCREEN
*  Description: oled clear *
*  Called By:AP
*  Table Accessed:
*  Table Updated:
*  Input:
*         N/A
*  Output:
*         N/A
*  Return:
*         N/A
*****************************************************************************/
/**@todo 桩接口，待删除*/
extern void DRV_OLED_CLEAR_WHOLE_SCREEN(void);

/*****************************************************************************
*  Function:  DRV_OLED_UPDATE_STATE_DISPLAY
*  Description: oled display right or not right  *
*  Called By:AP
*  Table Accessed:
*  Table Updated:
*  Input:
*         N/A
*  Output:
*         N/A
*  Return:
*         N/A
*****************************************************************************/
/**@todo 桩接口，待删除*/
extern  void DRV_OLED_UPDATE_STATE_DISPLAY(int UpdateStatus);

/*****************************************************************************
 函 数 名  : DRV_OLED_UPDATE_DISPLAY
 功能描述  : SD卡烧片版本升级，OLED显示
 输入参数  : BOOL UpdateStatus
 输出参数  : 无。
 返回值：   无
 其它:  升级成功，NV备份完成后调用该函数打勾
        升级失败后调用该函数打叉
*****************************************************************************/
/**@todo 桩接口，待删除*/
extern void DRV_OLED_UPDATE_DISPLAY(int UpdateStatus);

/*****************************************************************************
 函 数 名  : DRV_OLED_STRING_DISPLAY
 功能描述  : oled字符串显示函数
 输入参数  :
 *           要显示字符串的起始坐标
 *           要显示的字符串
 输出参数  : none
 返 回 值  : void
*****************************************************************************/
/**@todo 桩接口，待删除*/
extern void DRV_OLED_STRING_DISPLAY(unsigned char ucX, unsigned char ucY, unsigned char *pucStr);
/**@todo 桩接口，待删除*/
#define BSP_NVE_NAME_LENGTH          8       /*NV name maximum length*/
#define BSP_NVE_NV_DATA_SIZE        104     /*NV data maximum length*/
/**@todo 桩接口，待删除*/
#define BSP_NV_READ                 1       /*NV read  operation*/
#define BSP_NV_WRITE                0       /*NV write operation*/
/**@todo 桩接口，待删除*/
typedef struct nve_info_stru {
	unsigned int nv_operation;              /*0-write,1-read*/
	unsigned int nv_number;                 /*NV number you want to visit*/
	char nv_name[BSP_NVE_NAME_LENGTH];
	unsigned int valid_size;
	unsigned char nv_data[BSP_NVE_NV_DATA_SIZE];
}NVE_INFO_S;

/*****************************************************************************
* 函 数 名  : DRV_NVE_ACCESS
*
* 功能描述  : 访问LINUX NVE
*
* 输入参数  : nve  ID
*
* 输出参数  : 无
*
* 返 回 值  : OK& ERROR
*
* 修改记录 :  2013年6月27日  v1.00  yuanqinshun  创建
*****************************************************************************/
/**@todo 桩接口，待删除*/
int DRV_NVE_ACCESS(NVE_INFO_S *nve);


/**@todo 桩接口，待删除*/
extern int DRV_COPY_NVUSE_TO_NVBACKUP ( void );



/**@todo 桩接口，待删除*/
extern int DRV_COPY_NVUPGRADE_TO_NVUSE( void );
/**@todo 桩接口，待删除*/
typedef enum tagMSP_PROC_ID_E
{
    OM_REQUEST_PROC = 0,
    OM_ADDSNTIME_PROC = 1,
    OM_PRINTF_WITH_MODULE = 2,
    OM_PRINTF = 3,
    OM_PRINTF_GET_MODULE_IDLEV = 4,
    OM_READ_NV_PROC = 5,
    OM_WRITE_NV_PROC = 6,
    OM_MNTN_ERRLOG = 7, 
    MSP_PROC_REG_ID_MAX
}MSP_PROC_ID_E;
/**@todo 桩接口，待删除*/
typedef void (*BSP_MspProc)(void);
#define EXCH_CB_NAME_SIZE           (32)
typedef struct
{
    char   aucName[EXCH_CB_NAME_SIZE];
    unsigned char *pucData;
    unsigned int  ulDataLen;
}cb_buf_t;
/**@todo 桩接口，待删除*/
typedef int  (*exchCBReg)(cb_buf_t *);

/*liuyi++ 该接口在文档中未体现 ++*/
/*******************************************************************************
 函 数 名: DRV_EXCH_CUST_FUNC_REG
 功能描述: 临终遗言模块提供对外接口，供上层注册回调，用于复位时保存上层指定信息
 输入参数: cb 上层指定回调接口
 输出参数: 无
 返 回 值: 0-添加成功，其他-添加失败
 说明    :  pucData :指定记录信息地址，请注意不要指定栈等临时数据区，堆空间数据，
                     使用者自己维护，包括空间使用完，自己释放。建议全局变量。
            aucName :名称
            ulDataLen: 数据长度

返回值列表:
    CUST_REG_OK = 0,                        添加成功
    CUST_REG_PARM_NULL,                     入参为空
    CUST_REG_EXCH_NOT_READY,                EXCH模块尚未初始化
    CUST_REG_EXCH_REBOOT_CONTEXT,           已经进入复位流程
    CUST_REG_EXCH_QUEUE_NOT_INIT,           注册队列尚未初始化
    CUST_REG_EXCH_QUEUE_FULL,               队列已满
    CUST_REG_EXCH_ALREADY_IN_QUEUE,         重复注册
    CUST_REG_EXCH_MUTEX_REJECT,             MUTEX REJECT
    CUST_REG_EXCH_QUEUE_IN_ERR              入队列失败

请通过返回值定位解决错误

*******************************************************************************/
extern int DRV_EXCH_CUST_FUNC_REG(exchCBReg cb);
/*liuyi++ 该接口在文档中未体现 --*/

/*liuyi++ 该接口在文档中未体现*/
/*****************************************************************************
* 函 数 名  : DRV_MSP_PROC_REG
*
* 功能描述  : DRV提供给OM的注册函数
*
* 输入参数  : MSP_PROC_ID_E eFuncID, BSP_MspProc pFunc
* 输出参数  : NA
*
* 返 回 值  : NA
*
* 其它说明  : 可维可测接口函数
*
*****************************************************************************/
/**@todo 桩接口，待删除*/
#define DRV_MSP_PROC_REG(eFuncID, pFunc)
/*liuyi++ 该接口在文档中未体现--*/


/************************* 底软不用结构体(后续删除) ***************************/

/**
 * @brief TCP/IP协议栈可维可测捕获的消息标识
 * @todo 桩接口，待删除
 */
enum IPS_MNTN_TRACE_IP_MSG_TYPE_ENUM
{
    /* IP 数据包可维可测上报 */
    ID_IPS_TRACE_IP_ADS_UL                  = 0xD030,
    ID_IPS_TRACE_IP_ADS_DL                  = 0xD031,
    ID_IPS_TRACE_IP_USB_UL                  = 0xD032,
    ID_IPS_TRACE_IP_USB_DL                  = 0xD033,

    ID_IPS_TRACE_IP_MSG_TYPE_BUTT
};
/**@todo 桩接口，待删除*/
static inline unsigned long USB_ETH_DrvSetHostAssembleParam(unsigned long ulHostOutTimeout)
{
    (void)ulHostOutTimeout;
    return 0;
}
/**@todo 桩接口，待删除*/
static inline unsigned long USB_ETH_DrvSetDeviceAssembleParam(
    unsigned long ulEthTxMinNum,
    unsigned long ulEthTxTimeout,
    unsigned long ulEthRxMinNum,
    unsigned long ulEthRxTimeout)
{
    (void)ulEthTxMinNum;
    (void)ulEthTxTimeout;
    (void)ulEthRxMinNum;
    (void)ulEthRxTimeout;

    return 0;
}
/**@todo 桩接口，待删除*/
static inline unsigned int DRV_USB_GET_AVAILABLE_PORT_TYPE(unsigned char *pucPortType,
                            unsigned int *pulPortNum, unsigned int ulPortMax)
{
    (void)pucPortType;
    (void)pulPortNum;
    (void)ulPortMax;
    return 0;
}
/**@todo 桩接口，待删除*/
static inline int USB_otg_switch_get(unsigned char *pucValue)
{
    (void)pucValue;
    return 0;
}
/**
 * @todo 桩接口，待删除
 */
#define DRV_USB_PHY_SWITCH_GET(value) USB_otg_switch_get(value)
#define AT_USB_SWITCH_SET_VBUS_VALID            1
#define AT_USB_SWITCH_SET_VBUS_INVALID          2
#define USB_SWITCH_ON       1
#define USB_SWITCH_OFF      0

/**
 * @todo 桩接口，待删除
 */
typedef void (*HSIC_UDI_ENABLE_CB_T)(void);
/**
 * @todo 桩接口，待删除
 */
static inline unsigned int BSP_HSIC_RegUdiEnableCB(HSIC_UDI_ENABLE_CB_T pFunc)
{
    (void)pFunc;
    return 0;
}
/**
 * @todo 桩接口，待删除
 */
#define DRV_HSIC_REGUDI_ENABLECB(x) BSP_HSIC_RegUdiEnableCB(x)

/**
 * @todo 桩接口，待删除
 */
typedef void (*HSIC_UDI_DISABLE_CB_T)(void);
/**
 * @todo 桩接口，待删除
 */
static inline unsigned int BSP_HSIC_RegUdiDisableCB(HSIC_UDI_DISABLE_CB_T pFunc)
{
    (void)pFunc;
    return 0;
}
/**
 * @todo 桩接口，待删除
 */
#define DRV_HSIC_REGUDI_DISABLECB(x) BSP_HSIC_RegUdiDisableCB(x)
/**
 * @todo 桩接口，待删除
 */
static inline unsigned int BSP_GetHsicEnumStatus(void)
{
    return 0;
}
/**
 * @todo 桩接口，待删除
 */
#define DRV_GET_HSIC_ENUM_STATUS() BSP_GetHsicEnumStatus()


/************************* 底软不用结构体 END *********************************/

//struct sk_buff;
/**
 * @todo 桩接口，待删除
 */
typedef void (*USB_IPS_MNTN_TRACE_CB_T)(struct sk_buff *skb,unsigned short usType);
/**
 * @todo 桩接口，待删除
 */
static inline unsigned int DRV_USB_REG_IPS_TRACECB(USB_IPS_MNTN_TRACE_CB_T pFunc)
{
    if (!pFunc)
    {
        return 1;
    }

    return 0;
}
/**
 * @brief DRV_UTRACE_START
 * @par 描述:
 * DRV_UTRACE_START
 * @attention 无
 * @param 无
 * @retval 无
 * @todo 桩接口，待删除
 */
unsigned int DRV_UTRACE_START(unsigned char * pData);
/**
 * @brief DRV_UTRACE_TERMINATE
 *
 * @par 描述:
 * DRV_UTRACE_TERMINATE
 *
 * @attention 无
 *
 * @param 无
 *
 * @retval 无
 *
 * @todo 桩接口，待删除
 */
unsigned int DRV_UTRACE_TERMINATE(unsigned char * pData);

#ifdef __cplusplus
}
#endif
#endif
