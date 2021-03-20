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
 *  @brief   IPC模块在非融合版本上的资源枚举对外头文件
 *  @file    mdrv_ipc_enum.h
 *  @author  wangtongli
 *  @version v1.0
 *  @date    2019.11.27
 *  @note    修改记录
 *  <ul><li>v1.0|2019.11.27|支持接口自动化格式</li></ul>
 *  @since   始于DRV1.0（非融合版本）
 */
#ifndef __MDRV_IPC_ENUM_H__
#define __MDRV_IPC_ENUM_H__
#ifdef __cplusplus
extern "C"
{
#endif

/**
* @brief 支持的处理器类型
*/
/*lint --e{488,830}*/
typedef enum tagIPC_INT_CORE_E
{
    IPC_CORE_APPARM = 0x0,
    IPC_CORE_COMARM,
    IPC_CORE_ACORE = 0x0,   /*P531,V7R2*/
    IPC_CORE_CCORE,
    IPC_CORE_MCORE,
    IPC_CORE_LDSP,
    IPC_CORE_HiFi,
    IPC_CORE_BBE,           /* xdsp core id */
    IPC_CORE_NXDSP,
    IPC_CORE_NRCCPU,
    /* !!!!新增元素请添加到最后  */

     /* !!!!NRCPU CORE-ID 枚举值定义  */
    //ACORE MCORE NRCPU 的枚举值和AP 侧IPC 枚举值一致，新增HL1C、L2HAC、LL1C
    IPC_CORE_HL1C = 3,
    IPC_CORE_L2HAC,
    IPC_CORE_LL1C,

    IPC_CORE_BUTTOM = 8,
}ipc_int_core_e;

/**
 * @brief 支持的物理资源列表
 * @attention
 * <ul><li>添加新IPC资源，枚举命名格式:</li></ul>
 * <ul><li>IPC_\<目标处理器>_INT_SRC_\<源处理器>_<功能/作用></li></ul>
 */
typedef enum tagIPC_INT_LEV_E
{
    IPC_CCPU_INT_SRC_HIFI_MSG              = 0, /**<HIFI跨核消息通知*/
    IPC_CCPU_INT_SRC_MCU_MSG               = 1, /**<MCU跨核消息通知*/
    IPC_INT_DSP_HALT                       = 2, /**<DSP通知ARM睡眠*/
    IPC_INT_DSP_RESUME                     = 3, /**<DSP通知ARM完成唤醒后的恢复操作*/
                                                /* 4, 占位不允许添加其他值,该值必须与IPC_INT_DICC_USRDATA一致*/
                                                /* 5, 占位不允许添加其他值,该值必须与IPC_INT_DICC_USRDATA一致*/
    /* 定义TDS使用的邮箱IPC中断,begin */
    IPC_INT_WAKE_SLAVE                     = 6, /**<BBE16通知CCPU,唤醒GSM从模*/
    IPC_CCPU_INT_SRC_DSP_DVS,                  /* BBE16通知CCPU对BBP和dsp进行调压 */
    IPC_INT_DSP_PS_PUB_MBX                 ,/**<上行DSP->ARM,与低功耗IPC中断复用*/
    IPC_INT_DSP_PS_MAC_MBX                 ,/**<上行DSP->ARM,与低功耗IPC中断复用*/
    IPC_INT_DSP_MBX_RSD                    ,/**<上行DSP->ARM,与低功耗IPC中断复用*/
    IPC_CCPU_INT_SRC_DSP_MNTN              ,/**<CCPU->DSP DVFS模块调频调压后通知 NXP*/
    /* 定义TDS使用的邮箱IPC中断,end */
    IPC_CCPU_INT_SDR_CCPU_BBP_MASTER_ERROR ,   /**<BBE16通知C核发生了通道异常*/
    IPC_CCPU_INT_SRC_LDSP_OM_MBX,               /**<上行DSP->ARM, 通用邮箱消息到达*/
    IPC_CCPU_INT_TLMODEM1_WAKE_SLAVE,           /**< 副卡BBE16通知CCPU,唤醒GUTX从模 */
    IPC_CCPU_INT_TLMODEM1_MAC_MBX,              /**< 副卡BBE16通知CCPU, 专用邮箱 */
    IPC_CCPU_INT_SRC_ACPU_RESET            ,   /**< A/C核在C核单独复位期间核间通信(唤醒C核), v8r2走的是核间消息 */
    IPC_CCPU_SRC_ACPU_DUMP                 ,   /**< v7r2 可维可测，ACPU 发送给CCPU 的IPC中断 */
    IPC_CCPU_INT_SRC_ICC_PRIVATE           ,   /**< v7r2 ICC通信，icc专用发送消息使用,ACPU发送给CCPU的IPC中断       */
    IPC_CCPU_INT_SRC_MCPU                  ,   /**< ICC消息, MCU发送给CCPU的  IPC中断 */
    IPC_CCPU_INT_SRC_MCPU_WDT              ,   /**< 运行trace时停wdt */
    IPC_CCPU_INT_SRC_XDSP_1X_HALT          ,   /**< 1X Halt中断 */
    IPC_CCPU_INT_SRC_XDSP_HRPD_HALT        ,   /**< HRPD Halt中断 */
    IPC_CCPU_INT_SRC_XDSP_1X_RESUME        ,   /**< 1X Resume中断 */
    IPC_CCPU_INT_SRC_XDSP_HRPD_RESUME      ,   /**< HRPD Resume中断 */
    IPC_CCPU_INT_SRC_XDSP_MNTN             ,   /**< XDSP邮箱异常维测中断 */
    IPC_CCPU_INT_SRC_XDSP_PS_MBX           ,   /**< PS与X模邮箱通信使用的中断 */
    IPC_CCPU_INT_SRC_XDSP_DVS              ,   /**< CBBE16通知CCPU对dsp进行调频调压 */
    IPC_CCPU_INT_SRC_NRCCPU_BBP_WAKEUP     = 28,   /**< 5G BBP 唤醒4G CCPU 使用中断 */

    IPC_CCPU_INT_SRC_ACPU_IPC_EXTEND = 30, /**< acpu发给ccpu中断,用于扩展IPC 中断*/
    /*  austin项目和 mcu 通信使用 icc 中断号, 不能变更 */
    IPC_CCPU_INT_SRC_ACPU_ICC              = 31, /**< acpu发给ccpu中断*/

    /*  安全ICC通信使用中断号, 不能变更 */
    IPC_CCPU_INT_SRC_SECOS_ICC_IFC         = 32, /**< 对应安全IPC的0号中断，安全ICC共享通道中断 */
    IPC_CCPU_INT_SRC_SECOS_ICC_VSIM        = 33, /**< 对应安全IPC的1号中断，安全ICC天际通通道中断 */

    /*modem 内部IPC*/
    IPC_CCPU_INT_SRC_LTE0_HALT   =  64 ,   /**<LTE0 (  主卡) 给CCPU 的HALT  中断*/
    IPC_CCPU_INT_SRC_LTE1_HALT  ,             /**<LTE1 (  副卡) 给CCPU 的HALT  中断*/
    IPC_CCPU_INT_SRC_TDS_HALT  ,              /**<TDS  给CCPU的HALT中断*/
    IPC_CCPU_INT_SRC_LTE0_RESUME  ,   /**<LTE0 (  主卡) 给CCPU 的RESUME 中断*/
    IPC_CCPU_INT_SRC_LTE1_RESUME  ,   /**<LTE1 (  副卡) 给CCPU 的RESUME  中断*/
    IPC_CCPU_INT_SRC_TDS_RESUME  ,           /**< TDS 给CCPU 的RESUME  中断*/
    IPC_CCPU_INT_SRC_HISPEED_MBX  ,           /**< 快速邮箱DSP->ARM 中断*/
    IPC_CCPU_INT_SRC_LTEV_PS_MBX,               /**< LTEV_DSP->ARM, ps */
    IPC_CCPU_INT_SRC_LTEV_OM_MBX,               /**< LTEV_DSP->ARM, om */
    IPC_CCPU_INT_SRC_LTEV_MAC_MBX,               /**< 上行LTEV_DSP->ARM, MAC数据 */
    IPC_CCPU_INT_SRC_DSPV_HALT,                       /**< LTEVDSP通知ARM睡眠*/
    IPC_CCPU_INT_SRC_DSPV_RESUME,                     /**< LTEVDSP通知ARM完成唤醒后的恢复操作*/
    IPC_CCPU_INT_SRC_LTEV_HALT,                       /* LTEV模式通知ARM睡眠*/
    IPC_CCPU_INT_SRC_LTEV_RESUME,                     /**< LTEV模式通知ARM完成唤醒后的恢复操作*/
    IPC_CCPU_INT_SRC_DSPV_DVS_REQ,                  /**< BBE16通知CCPU对BBP和dsp进行调压 */
    IPC_INT_LETV_WAKE_SLAVE,                     /**< LTEV通知CCPU,唤醒LTE从模 */

    IPC_INNER_INT_TEST        = 90,

    /* 定义MCU IPC跨核消息中断源bit位置 */
    IPC_MCU_INT_SRC_ACPU_MSG             = 0,    /**< ACPU跨核消息通知 */
    IPC_MCU_INT_SRC_CCPU_MSG                ,    /**< CCPU跨核消息通知 */
    IPC_MCU_INT_SRC_HIFI_MSG                ,    /**< HIFI跨核消息通知 */
    IPC_MCU_INT_SRC_CCPU_IPF                ,    /**< IPF低功耗 */
    IPC_MCU_INT_SRC_ACPU_PD                 ,    /**< acpu power down */
    IPC_MCU_INT_SRC_HIFI_PD                 ,    /**< hifi power down */
    IPC_MCU_INT_SRC_HIFI_DDR_VOTE           ,    /**< HIFI DDR自刷新投票 */
    IPC_MCU_INT_SRC_ACPU_I2S_REMOTE_SLOW    ,
    IPC_MCU_INT_SRC_ACPU_I2S_REMOTE_SLEEP   ,
    IPC_MCU_INT_SRC_ACPU_I2S_REMOTE_INVALID ,
    /* ACPU给CCPU发送IPC中断时，需要同时告知给M3 */
    IPC_MCU_INT_SRC_ACPU_DRX                ,
    IPC_MCU_INT_SRC_CCPU_DRX                ,   /**< CCPU给ACPU发送IPC中断时，需要同时告知给M3 */
    IPC_MCU_INT_SRC_ICC_PRIVATE             ,   /**< m3 icc私有的ipc中断，高性能通道，启独立任务用 */
    IPC_MCU_INT_SRC_DUMP                    ,   /**< m3 dump中断，ACORE发送给MCU的IPC中断*/
    IPC_MCU_INT_SRC_HIFI_PU                 ,   /**< POWERUP,  HIFI发送给MCU的IPC中断 */
    IPC_MCU_INT_SRC_HIFI_DDR_DFS_QOS        , /**< HIFI对DDR调频投票 */
    IPC_MCU_INT_SRC_TEST                    ,   /**< for test a\c interact with m3 */
    IPC_MCPU_INT_SRC_ACPU_USB_PME_EN        ,  /**< acore向M3通报USB唤醒完成事件 */
    IPC_MCU_INT_SRC_NR_CCPU_START           ,  /**< 通知MCU启动NR CCPU */
    IPC_MCU_INT_SRC_LR_CCPU_START           ,
    IPC_MCU_INT_SRC_NRCCPU_PD               ,  /**< 通知MCU 给NRCCPU 下电 */
    IPC_MCU_INT_SRC_ACPU_HIFI_RESET         ,  /**< A核通知LPM3给BHIFI下电*/
    IPC_MCU_INT_SRC_ACPU_CUST               ,  /**< MBB平台分配给产品线使用的A核发送给M3的中断*/

    /* 以下3个austin修改，和AP对齐，不能修改 */
    IPC_MCU_INT_SRC_ICC                 = 29,   /**< m3 icc公用的ipc中断 */
    IPC_MCU_INT_SRC_CCPU_PD             = 30,    /**< ccpu power down */
    IPC_MCU_INT_SRC_CCPU_START          = 31,   /**< 通知MCU启动LR CCPU */
    IPC_MCU_INT_SRC_CCPU_HIFI_PD        = 32,  /**< c核通知LPM3给HIFI下电 */
    IPC_MCU_INT_SRC_CCPU_HIFI_PU        = 33,  /**< c核通知LPM3给HIFI上电 */

    /* 定义ACPU IPC跨核消息中断源bit位置 */
    IPC_ACPU_INT_SRC_CCPU_MSG             = 0,   /**< CCPU跨核消息通知 */
    IPC_ACPU_INT_SRC_HIFI_MSG             = 1,   /**< HIFI跨核消息通知 */
    IPC_ACPU_INT_SRC_MCU_MSG              = 2,   /**< ACPU跨核消息通知 */
    IPC_ACPU_INT_SRC_CCPU_NVIM            = 3,   /**< 用于NVIM模块C核与A核之间的同步*/
    IPC_INT_DICC_USRDATA                  = 4,   /**< 必须与CCPU接收中断号同时修改*/
    IPC_INT_DICC_RELDATA                  = 5,   /**< 必须与CCPU接收中断号同时修改*/
    IPC_ACPU_INT_SRC_CCPU_ICC             ,
    IPC_ACPU_INT_SRC_ICC_PRIVATE          ,   /**< v7r2 icc 私用，CCPU发送给ACPU的IPC中断 */
    IPC_ACPU_SRC_CCPU_DUMP                ,   /**< v7r2 可维可测，CCPU 发送给ACPU 的IPC中断 */
    IPC_ACPU_INT_SRC_MCPU                 ,   /**< ICC消息, MCU发送给ACPU的 IPC中断 */
    IPC_ACPU_INT_SRC_MCPU_WDT             ,  /**< 运行trace时停wdt */
    IPC_ACPU_INT_MCU_SRC_DUMP             ,  /**< dump中断?MCU发送给ACPU的IPC中断 */
    IPC_ACPU_INT_SRC_CCPU_RESET_IDLE      ,  /**< A/C核在C核单独复位期间核间通信(master in idle)，姚国才 */
    IPC_ACPU_INT_SRC_CCPU_RESET_SUCC      ,  /**< A/C核在C核单独复位期间核间通信(ccore reset ok)，姚国才 */
    IPC_ACPU_INT_SRC_CCPU_LOG             ,  /**< CCPU发送给ACPU的LOG保存中断 */
    IPC_ACPU_INT_SRC_MCU_FOR_TEST         ,  /**< test, m core to acore */
    IPC_ACPU_INT_SRC_CCPU_TEST_ENABLE     ,  /**< IPC自动化测试中断使能时使用，后续IPC中断不够使用时考虑删除 */
    IPC_ACPU_INT_SRC_MCPU_USB_PME         ,  /**< M3向acore上报USB唤醒事件 */
    IPC_ACPU_INT_SRC_HIFI_PC_VOICE_RX_DATA,  /**< hifi->acore pc voice */
    IPC_ACPU_INT_SRC_CCPU_PM_OM           ,  /**< CCPU发给ACPU的PMOM中断,触发写文件, 姚国才 */
    IPC_ACPU_INT_SRC_CCPU_IPC_EXTEND =20,
    IPC_ACPU_INT_SRC_NRCCPU_RESET_SUCC ,
    IPC_ACPU_SRC_NRCCPU_DUMP,
    IPC_ACPU_INT_SRC_NRCCPU_RESET_IDLE,
    IPC_ACPU_INT_SRC_END,

    /* 定义HIFI IPC跨核消息中断源bit位置 */
    IPC_HIFI_INT_SRC_ACPU_MSG = 0 ,  /**< ACPU跨核消息通知 */
    IPC_HIFI_INT_SRC_CCPU_MSG     ,  /**< CCPU跨核消息通知 */
    IPC_HIFI_INT_SRC_BBE_MSG      ,  /**< TDDSP跨核消息通知 */
    IPC_HIFI_INT_SRC_MCU_MSG      ,
    IPC_HIFI_INT_SRC_ACPU_ICC      ,  /**< MBB platform acore 2 hifi icc */
    IPC_HIFI_INT_SRC_END          ,

    /* 定义NXDSP IPC跨核消息中断源bit位置 */
    IPC_INT_MSP_DSP_OM_MBX = 0 ,   /**< 下行ARM->DSP */
    IPC_INT_PS_DSP_PUB_MBX     ,   /**< 下行ARM->DSP */
    IPC_INT_PS_DSP_MAC_MBX     ,   /**< 下行ARM->DSP */
    IPC_INT_HIFI_DSP_MBX       ,   /**< HIFI->DSP */
    IPC_BBE16_INT_SRC_HIFI_MSG ,   /**< 邮箱消息，HIFI发送给BBE16的IPC中断 */
    IPC_INT_MSP_DSP_LTE0_WAKE_UP  ,       /**<CCPU 给DSP 的LTE0  唤醒中断*/
    IPC_INT_MSP_DSP_LTE1_WAKE_UP  ,   /**<CCPU 给DSP 的LTE1  唤醒中断*/
    IPC_INT_MSP_DSP_TDS_WAKE_UP  ,    /**<CCPU 给DSP 的TDS  唤醒中断*/
    IPC_INT_MSP_DSP_HISPEED_MBX ,   /**< 快速邮箱下行ARM->DSP */
    IPC_BBE16_INT_SRC_END  ,


    /* 定义CBBE16 IPC跨核消息中断源bit位置 */
    IPC_XDSP_INT_SRC_CCPU_1X_WAKE =  IPC_BBE16_INT_SRC_END ,   /* 1X Wake中断 */
    IPC_XDSP_INT_SRC_CCPU_HRPD_WAKE   ,   /**< HRPD Wake中断 */
    IPC_XDSP_INT_SRC_CCPU_OM_MBX      ,   /**< C核->XDSP */
    IPC_XDSP_INT_SRC_CCPU_PUB_MBX     ,   /**< C核->XDSP */
    IPC_XDSP_INT_SRC_CCPU_1X_WAKE_UP  ,   /**<CCPU 给DSP 的1X  唤醒中断*/
    IPC_XDSP_INT_SRC_CCPU_HRPD_WAKE_UP  ,     /**<CCPU 给DSP 的HRPD  唤醒中断*/
    IPC_LTE_INT_SRC_LTEV_MBX,             /* LTE-V DSP给LTE DSP的LTE-V数据时频转换SDR任务启动中断 */
    IPC_XDSP_INT_SRC_END ,

    /* 定义NRCCPU IPC跨核消息中断源bit位置 */
    IPC_NRCCPU_INT_SRC_LRCCPU_WAKEUP  = 0 ,  /**<LTE CCPU 唤醒or 允许睡眠 NR CCPU*/
    IPC_NRCCPU_SRC_ACPU_DUMP,
    IPC_NRCCPU_INT_SRC_ACPU_RESET,
    IPC_NRCCPU_INT_SRC_HL1C_PD               = 29, /**< HL1C发给nrccpu 下电中断*/
    IPC_NRCCPU_INT_SRC_ACPU_ICC              = 31, /**< acpu发给nrccpu中断*/
    /*  安全ICC通信使用中断号, 不能变更 */
    IPC_NRCCPU_INT_SRC_SECOS_ICC_IFC         = 32, /**< 对应安全IPC的0号中断，安全ICC共享通道中断 */
    IPC_NRCCPU_INT_SRC_SECOS_ICC_VSIM        = 33, /**< 对应安全IPC的1号中断，安全ICC天际通通道中断 */

    /*5G modem 内部IPC*/
    IPC_NRCCPU_INT_SRC_LL1C_PM = 64,

    IPC_NRCCPU_INT_SRC_L2HAC_PD = 65, 			/**< L2HAC发给nrccpu 下电中断*/
    IPC_NRCCPU_INT_SRC_END ,

    /* 定义L2HAC IPC跨核消息中断源bit位置 */
    IPC_L2HAC_INT_SRC_NRCCPU_WAKEUP     = 64, /**< NRCCPU 发给L2HAC 唤醒中断*/
    IPC_L2HAC_INT_SRC_NRCCPU_HALT          = 65, /**< NRCCPU 发给L2HAC 允许睡眠中断*/
    IPC_L2HAC_INT_SRC_END ,

    IPC_LTEV_INT_SRC_CCPU_PS_MBX = 0,           /**< ARM->LTEV_DSP, ps */
    IPC_LTEV_INT_SRC_CCPU_OM_MBX,               /**< ARM->LTEV_DSP, om */
    IPC_INT_MSP_DSP_LTEV_WAKE_UP  ,             /**<CCPU 给DSP 的LTE0  唤醒中断*/
    IPC_CCPU_INT_SRC_DSPV_DVS_CNF,              /**< CCPU->DSP DVFS模块调频调压后通知 NXP */
    /* 仅解决编译问题 */
    IPC_INT_BUTTOM             = 96,
}ipc_int_lev_e;

/**
 * @brief 支持的信号量号列表
 */
typedef enum tagIPC_SEM_ID_E
{
    IPC_SEM_MEM          ,
    IPC_SEM_DICC         ,
    IPC_SEM_SYNC         ,
    IPC_SEM_SYSCTRL      ,
    IPC_SEM_BBP_RESERVED ,  /**<该值与BBE16镜像绑定，修改需通知BBE16作相应修改*/
    IPC_SEM_LBBP1        ,  /**<副卡bbp，ccore 与 tlphy 使用 */
    IPC_SEM_NVIM         ,
    IPC_SEM_1X_MODE      ,  /**< 1X模式锁，GSDR开发合入 */
    IPC_SEM_DPDT_CTRL_ANT,  /**< 添加mdrv_ipc_spin_trylock时添加 */
    IPC_SEM_BBPMASTER_0  ,
    IPC_SEM_BBPMASTER_1  ,
    IPC_SEM_BBPMASTER_2  ,
    IPC_SEM_BBPMASTER_3  ,
    IPC_SEM_BBPMASTER_5  ,
    IPC_SEM_BBPMASTER_6  ,
    IPC_SEM_BBPMASTER_7  ,
    IPC_SEM_NV           ,
    IPC_SEM_GPIO         ,
    IPC_SEM_PMU          ,
    IPC_SEM_IPF_PWCTRL   ,
    IPC_SEM_NV_CRC       ,
    IPC_SEM_PM_OM_LOG    ,
    IPC_SEM_MDRV_LOCK    ,
    IPC_SEM_CDMA_DRX     , /**< C核与XDSP使用 */
    IPC_SEM_GU_SLEEP     ,
    IPC_SEM_DPM          , /**<DPM 数据从C核通过共享内存传递给A核*/
    IPC_SEM2_IPC_TEST    , /**<IPC自动化测试使用*/
    IPC_SEM_DFS_FIX      = 31, /**<M3 DFS与HIFI互斥访问APB总线使用,仅HI6950芯片平台上使用*/

    IPC_SEM_CPM          = 64,
    IPC_SEM_CPM_1X       = 65,    /**< X模融合到nxp上新增1X核间锁 */
    IPC_SEM_BBP          = 66,

    IPC_SEM_LTEV         = 67,     /**< LTEV新增核间锁 */
    IPC_SEM_LTEVBBP      = 68,     /**< LTEV BBP新增核间锁 */

    IPC_SEM_INNER_TEST = 70,
    IPC_SEM_BUTTOM       = 96
} ipc_sem_id_e;

#ifdef __cplusplus
}
#endif

#endif
