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

#include "PsTypeDef.h"
#include "vos.h"
#include "ps_common_def.h"
#include "AppVcomDev.h"
#include "taf_type_def.h"
#include "AtTypeDef.h"
#include "AtMntn.h"
#include "securec.h"


/*
 * 协议栈打印打点方式下的.C文件宏定义
 */
#define THIS_FILE_ID PS_FILE_ID_APP_VCOM_DEV_C

/* VCOM CTX,用于保存VCOM的全局变量 */
APP_VCOM_DevCtx g_vcomCtx[APP_VCOM_MAX_NUM];

APP_VCOM_DebugInfo g_appVcomDebugInfo;

/* 虚拟串口文件操作接口 */
static const struct file_operations g_operations_Fops = {
    .owner   = THIS_MODULE,
    .read    = APP_VCOM_Read,
    .poll    = APP_VCOM_Poll,
    .write   = APP_VCOM_Write,
    .open    = APP_VCOM_Open,
    .release = APP_VCOM_Release,
};

/*
 * APPVCOM的规格和应用
 * APPVCOM ID   缓存大小  用途           是否AT的CLIENT    ModemId   USER版本是否使用(参见g_debugAppPortIndex)
 * APPVCOM        4K      RIL(主)               是         MODEM0          Y
 * APPVCOM1       4K      RIL(呼叫)             是         MODEM0          Y
 * APPVCOM2       4K      工程菜单              是         MODEM0          Y
 * APPVCOM3       8K      生产装备(AT SERVER)   是         MODEM0          Y
 * APPVCOM4       4K      audio RIL             是         MODEM0          Y
 * APPVCOM5       4K      RIL(主)               是         MODEM1          Y
 * APPVCOM6       4K      RIL(呼叫)             是         MODEM1          Y
 * APPVCOM7       8K      生产装备(AT SERVER)   是         MODEM1          Y
 * APPVCOM8       4K      工程菜单/HIDP         是         MODEM1          Y
 * APPVCOM9      20K      AGPS                  是         MODEM0          Y
 * APPVCOM10      4K      RIL                   是         MODEM0          Y
 * APPVCOM11      4K      ISDB                  是         MODEM0          UnCertain
 * APPVCOM12     20K      AGPS                  是         MODEM1          Y
 * APPVCOM13      4K      RIL(查询)             是         MODEM0          Y
 * APPVCOM14      4K      RIL(查询)             是         MODEM1          Y
 * APPVCOM15      4K      RIL                   是         MODEM1          Y
 * APPVCOM16      4K      HIDP                  是         MODEM0          Y
 * APPVCOM17      4K      AGPS-AP               是         MODEM0          Y
 * APPVCOM18      4K      NFC                   是         MODEM0          Y
 * APPVCOM19      4K      海外运营商写simlock   是         MODEM0          Y
 * APPVCOM20      4K      RIL(主)               是         MODEM2          Y
 * APPVCOM21      4K      RIL(呼叫)             是         MODEM2          Y
 * APPVCOM22      4K      工程菜单              是         MODEM2          Y
 * APPVCOM23      8K      生产装备(AT SERVER)   是         MODEM2          Y
 * APPVCOM24      4K      AGPS                  是         MODEM2          Y
 * APPVCOM25      4K      RIL                   是         MODEM2          Y
 * APPVCOM26      4K      RIL(查询)             是         MODEM2          Y
 * APPVCOM27      4K      RIL                   是         MODEM0          Y
 * APPVCOM28      4K      RIL                   是         MODEM0          Y
 * APPVCOM29      4K      RIL                   是         MODEM1          Y
 * APPVCOM30      4K      RIL                   是         MODEM1          Y
 * APPVCOM31      4K      RIL                   是         MODEM2          Y
 * APPVCOM32      4K      RIL                   是         MODEM2          Y
 * APPVCOM33      4K      生产装备(AT SERVER)   是         MODEM1          Y
 * APPVCOM34      4K      NFC                   是         MODEM1          Y
 * APPVCOM35      4K      预留                  是         MODEM0          N
 * APPVCOM36      4K      预留                  是         MODEM0          N
 * APPVCOM37      4K      预留                  是         MODEM0          N
 * APPVCOM38      4K      预留                  是         MODEM0          N
 * APPVCOM39      4K      预留                  是         MODEM0          N
 * APPVCOM40      4K      预留                  是         MODEM0          N
 * APPVCOM41      4K      预留                  是         MODEM0          N
 * APPVCOM42      4K      预留                  是         MODEM0          N
 * APPVCOM43      4K      预留                  是         MODEM0          N
 * APPVCOM44      4K      预留                  是         MODEM0          N
 * APPVCOM45      4K      预留                  是         MODEM0          N
 * APPVCOM46      4K      预留                  是         MODEM0          N
 * APPVCOM47      4K      预留                  是         MODEM0          N
 * APPVCOM48      4K      预留                  是         MODEM0          N
 * APPVCOM49      4K      预留                  是         MODEM0          N
 * APPVCOM50      4K      预留                  是         MODEM0          N
 * APPVCOM51      4K      预留                  是         MODEM0          N
 * APPVCOM52      4K      预留                  是         MODEM0          N
 * APPVCOM53      128K    errlog                是                         Y
 * APPVCOM54      4K      T/L装备               否                         UnCertain
 * APPVCOM55      2M      log 3.5               否                         Y
 * APPVCOM56      2M      log 3.5               否                         UnCertain
 * APPVCOM57      4K      预留                  是         MODEM0          N
 * APPVCOM58      4K      预留                  是         MODEM0          N
 * APPVCOM59      4K      预留                  是         MODEM0          N
 * APPVCOM60      4K      预留                  是         MODEM0          N
 * APPVCOM61      4K      预留                  是         MODEM0          N
 * APPVCOM62      4K      预留                  是         MODEM0          N
 * APPVCOM63      4K      预留                  是         MODEM0          N
 */
#if (FEATURE_ON == FEATURE_VCOM_EXT)
static const APP_VCOM_DevConfig g_appVcomCogfigTab[] = {
    { APP_VCOM_DEV_NAME_0, APP_VCOM_SEM_NAME_0, 0x1000, 0 },   /* APPVCOM */
    { APP_VCOM_DEV_NAME_1, APP_VCOM_SEM_NAME_1, 0x1000, 0 },   /* APPVCOM1 */
    { APP_VCOM_DEV_NAME_2, APP_VCOM_SEM_NAME_2, 0x1000, 0 },   /* APPVCOM2 */
    { APP_VCOM_DEV_NAME_3, APP_VCOM_SEM_NAME_3, 0x2000, 0 },   /* APPVCOM3 */
    { APP_VCOM_DEV_NAME_4, APP_VCOM_SEM_NAME_4, 0x1000, 0 },   /* APPVCOM4 */
    { APP_VCOM_DEV_NAME_5, APP_VCOM_SEM_NAME_5, 0x1000, 0 },   /* APPVCOM5 */
    { APP_VCOM_DEV_NAME_6, APP_VCOM_SEM_NAME_6, 0x1000, 0 },   /* APPVCOM6 */
    { APP_VCOM_DEV_NAME_7, APP_VCOM_SEM_NAME_7, 0x2000, 0 },   /* APPVCOM7 */
    { APP_VCOM_DEV_NAME_8, APP_VCOM_SEM_NAME_8, 0x1000, 0 },   /* APPVCOM8 */
    { APP_VCOM_DEV_NAME_9, APP_VCOM_SEM_NAME_9, 0x5000, 0 },   /* APPVCOM9 */
    { APP_VCOM_DEV_NAME_10, APP_VCOM_SEM_NAME_10, 0x1000, 0 }, /* APPVCOM10 */
    { APP_VCOM_DEV_NAME_11, APP_VCOM_SEM_NAME_11, 0x1000, 0 }, /* APPVCOM11 */
    { APP_VCOM_DEV_NAME_12, APP_VCOM_SEM_NAME_12, 0x5000, 0 }, /* APPVCOM12 */
    { APP_VCOM_DEV_NAME_13, APP_VCOM_SEM_NAME_13, 0x1000, 0 }, /* APPVCOM13 */
    { APP_VCOM_DEV_NAME_14, APP_VCOM_SEM_NAME_14, 0x1000, 0 }, /* APPVCOM14 */
    { APP_VCOM_DEV_NAME_15, APP_VCOM_SEM_NAME_15, 0x1000, 0 }, /* APPVCOM15 */
    { APP_VCOM_DEV_NAME_16, APP_VCOM_SEM_NAME_16, 0x1000, 0 }, /* APPVCOM16 */
    { APP_VCOM_DEV_NAME_17, APP_VCOM_SEM_NAME_17, 0x1000, 0 }, /* APPVCOM17 */
    { APP_VCOM_DEV_NAME_18, APP_VCOM_SEM_NAME_18, 0x1000, 0 }, /* APPVCOM18 */
    { APP_VCOM_DEV_NAME_19, APP_VCOM_SEM_NAME_19, 0x1000, 0 }, /* APPVCOM19 */
    { APP_VCOM_DEV_NAME_20, APP_VCOM_SEM_NAME_20, 0x1000, 0 }, /* APPVCOM20 */
    { APP_VCOM_DEV_NAME_21, APP_VCOM_SEM_NAME_21, 0x1000, 0 }, /* APPVCOM21 */
    { APP_VCOM_DEV_NAME_22, APP_VCOM_SEM_NAME_22, 0x1000, 0 }, /* APPVCOM22 */
    { APP_VCOM_DEV_NAME_23, APP_VCOM_SEM_NAME_23, 0x2000, 0 }, /* APPVCOM23 */
    { APP_VCOM_DEV_NAME_24, APP_VCOM_SEM_NAME_24, 0x1000, 0 }, /* APPVCOM24 */
    { APP_VCOM_DEV_NAME_25, APP_VCOM_SEM_NAME_25, 0x1000, 0 }, /* APPVCOM25 */
    { APP_VCOM_DEV_NAME_26, APP_VCOM_SEM_NAME_26, 0x1000, 0 }, /* APPVCOM26 */

    { APP_VCOM_DEV_NAME_27, APP_VCOM_SEM_NAME_27, 0x1000, 0 }, /* APPVCOM27 */
    { APP_VCOM_DEV_NAME_28, APP_VCOM_SEM_NAME_28, 0x1000, 0 }, /* APPVCOM28 */
    { APP_VCOM_DEV_NAME_29, APP_VCOM_SEM_NAME_29, 0x1000, 0 }, /* APPVCOM29 */
    { APP_VCOM_DEV_NAME_30, APP_VCOM_SEM_NAME_30, 0x1000, 0 }, /* APPVCOM30 */
    { APP_VCOM_DEV_NAME_31, APP_VCOM_SEM_NAME_31, 0x1000, 0 }, /* APPVCOM31 */

    { APP_VCOM_DEV_NAME_32, APP_VCOM_SEM_NAME_32, 0x1000, 0 }, /* APPVCOM32 */
    { APP_VCOM_DEV_NAME_33, APP_VCOM_SEM_NAME_33, 0x1000, 0 }, /* APPVCOM33 */
    { APP_VCOM_DEV_NAME_34, APP_VCOM_SEM_NAME_34, 0x1000, 0 }, /* APPVCOM34 */
    { APP_VCOM_DEV_NAME_35, APP_VCOM_SEM_NAME_35, 0x1000, 0 }, /* APPVCOM35 */
    { APP_VCOM_DEV_NAME_36, APP_VCOM_SEM_NAME_36, 0x1000, 0 }, /* APPVCOM36 */
    { APP_VCOM_DEV_NAME_37, APP_VCOM_SEM_NAME_37, 0x1000, 0 }, /* APPVCOM37 */
    { APP_VCOM_DEV_NAME_38, APP_VCOM_SEM_NAME_38, 0x1000, 0 }, /* APPVCOM38 */
    { APP_VCOM_DEV_NAME_39, APP_VCOM_SEM_NAME_39, 0x1000, 0 }, /* APPVCOM39 */
    { APP_VCOM_DEV_NAME_40, APP_VCOM_SEM_NAME_40, 0x1000, 0 }, /* APPVCOM40 */
    { APP_VCOM_DEV_NAME_41, APP_VCOM_SEM_NAME_41, 0x1000, 0 }, /* APPVCOM41 */
    { APP_VCOM_DEV_NAME_42, APP_VCOM_SEM_NAME_42, 0x1000, 0 }, /* APPVCOM42 */
    { APP_VCOM_DEV_NAME_43, APP_VCOM_SEM_NAME_43, 0x1000, 0 }, /* APPVCOM43 */
    { APP_VCOM_DEV_NAME_44, APP_VCOM_SEM_NAME_44, 0x1000, 0 }, /* APPVCOM44 */
    { APP_VCOM_DEV_NAME_45, APP_VCOM_SEM_NAME_45, 0x1000, 0 }, /* APPVCOM45 */
    { APP_VCOM_DEV_NAME_46, APP_VCOM_SEM_NAME_46, 0x1000, 0 }, /* APPVCOM46 */
    { APP_VCOM_DEV_NAME_47, APP_VCOM_SEM_NAME_47, 0x1000, 0 }, /* APPVCOM47 */
    { APP_VCOM_DEV_NAME_48, APP_VCOM_SEM_NAME_48, 0x1000, 0 }, /* APPVCOM48 */
    { APP_VCOM_DEV_NAME_49, APP_VCOM_SEM_NAME_49, 0x1000, 0 }, /* APPVCOM49 */
    { APP_VCOM_DEV_NAME_50, APP_VCOM_SEM_NAME_50, 0x1000, 0 }, /* APPVCOM50 */
    { APP_VCOM_DEV_NAME_51, APP_VCOM_SEM_NAME_51, 0x1000, 0 }, /* APPVCOM51 */
    { APP_VCOM_DEV_NAME_52, APP_VCOM_SEM_NAME_52, 0x1000, 0 }, /* APPVCOM52 */

    { APP_VCOM_DEV_NAME_53, APP_VCOM_SEM_NAME_53, 0x20000, 0 },  /* APPVCOM53 */
    { APP_VCOM_DEV_NAME_54, APP_VCOM_SEM_NAME_54, 0x1000, 0 },   /* APPVCOM54 */
    { APP_VCOM_DEV_NAME_55, APP_VCOM_SEM_NAME_55, 0x200000, 0 }, /* APPVCOM55 */
    { APP_VCOM_DEV_NAME_56, APP_VCOM_SEM_NAME_56, 0x200000, 0 }, /* APPVCOM56 */
    { APP_VCOM_DEV_NAME_57, APP_VCOM_SEM_NAME_57, 0x1000, 0 },   /* APPVCOM57 */

    { APP_VCOM_DEV_NAME_58, APP_VCOM_SEM_NAME_58, 0x1000, 0 }, /* APPVCOM58 */
    { APP_VCOM_DEV_NAME_59, APP_VCOM_SEM_NAME_59, 0x1000, 0 }, /* APPVCOM59 */
    { APP_VCOM_DEV_NAME_60, APP_VCOM_SEM_NAME_60, 0x1000, 0 }, /* APPVCOM60 */
    { APP_VCOM_DEV_NAME_61, APP_VCOM_SEM_NAME_61, 0x1000, 0 }, /* APPVCOM61 */
    { APP_VCOM_DEV_NAME_62, APP_VCOM_SEM_NAME_62, 0x1000, 0 }, /* APPVCOM62 */
    { APP_VCOM_DEV_NAME_63, APP_VCOM_SEM_NAME_63, 0x1000, 0 }  /* APPVCOM63 */
};
#else
static const APP_VCOM_DevConfig g_appVcomCogfigTab[] = {
    { APP_VCOM_DEV_NAME_0, APP_VCOM_SEM_NAME_0, 0x1000, 0 }, /* APPVCOM */
    { APP_VCOM_DEV_NAME_1, APP_VCOM_SEM_NAME_1, 0x1000, 0 }  /* APPVCOM1 */
};
#endif

APP_VCOM_DebugCfg g_appVcomDebugCfg;

#if (FEATURE_ON == FEATURE_VCOM_EXT)
#if (FEATURE_OFF == FEATURE_DEBUG_APP_PORT)
/*
 * user版本不使用端口列表，当前只关闭保留端口，
 * 非保留端口大部分当前已经明确使用，当前不确认的端口有:
 * APPVCOM11 APPVCOM54 APPVCOM55,由于这几个端口当前不是很明确是否使用，所以user版本默认不关闭
 */
static const APP_VCOM_DevIndexUint8 g_debugAppPortIndex[] = {
    APP_VCOM_DEV_INDEX_35, APP_VCOM_DEV_INDEX_36, APP_VCOM_DEV_INDEX_37, APP_VCOM_DEV_INDEX_38, APP_VCOM_DEV_INDEX_39,
    APP_VCOM_DEV_INDEX_40, APP_VCOM_DEV_INDEX_41, APP_VCOM_DEV_INDEX_42, APP_VCOM_DEV_INDEX_43, APP_VCOM_DEV_INDEX_44,
    APP_VCOM_DEV_INDEX_45, APP_VCOM_DEV_INDEX_46, APP_VCOM_DEV_INDEX_47, APP_VCOM_DEV_INDEX_48, APP_VCOM_DEV_INDEX_49,
    APP_VCOM_DEV_INDEX_50, APP_VCOM_DEV_INDEX_51, APP_VCOM_DEV_INDEX_52, APP_VCOM_DEV_INDEX_57, APP_VCOM_DEV_INDEX_58,
    APP_VCOM_DEV_INDEX_59, APP_VCOM_DEV_INDEX_60, APP_VCOM_DEV_INDEX_61, APP_VCOM_DEV_INDEX_62, APP_VCOM_DEV_INDEX_63
};


VOS_UINT8 APP_VCOM_IsDebugAppPort(APP_VCOM_DevIndexUint8 devIndex)
{
    VOS_UINT32 i = 0;

    for (i = 0; i < APP_VCOM_ARRAY_SIZE(g_debugAppPortIndex); i++) {
        if (devIndex == g_debugAppPortIndex[i]) {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}
#endif
#endif

/*
 * 3 函数、变量声明
 */


APP_VCOM_DevCtx* APP_VCOM_GetVcomCtxAddr(VOS_UINT8 indexNum)
{
    return &(g_vcomCtx[indexNum]);
}


APP_VCOM_DevEntity* APP_VCOM_GetAppVcomDevEntity(VOS_UINT8 indexNum)
{
    return (g_vcomCtx[indexNum].appVcomDevEntity);
}


VOS_UINT32 APP_VCOM_RegDataCallback(VOS_UINT8 devIndex, SEND_UL_AT_FUNC func)
{
    APP_VCOM_DevCtx *vcomCtx = VOS_NULL_PTR;

    /* 索引号错误 */
    if (devIndex >= APP_VCOM_DEV_INDEX_BUTT) {
        return VOS_ERR;
    }

    vcomCtx = APP_VCOM_GetVcomCtxAddr(devIndex);

    /* 函数指针赋给全局变量 */
    vcomCtx->sendUlAtFunc = func;

    return VOS_OK;
}


VOS_UINT32 APP_VCOM_RegEvtCallback(VOS_UINT8 devIndex, EVENT_FUNC func)
{
    APP_VCOM_DevCtx *vcomCtx = VOS_NULL_PTR;

    /* 索引号错误 */
    if (devIndex >= APP_VCOM_DEV_INDEX_BUTT) {
        return VOS_ERR;
    }

    vcomCtx = APP_VCOM_GetVcomCtxAddr(devIndex);

    /* 函数指针赋给全局变量 */
    vcomCtx->eventFunc = func;

    return VOS_OK;
}


VOS_UINT8 APP_VCOM_GetIndexFromMajorDevId(VOS_UINT majorDevId)
{
    VOS_UINT32       loop;
    APP_VCOM_DevCtx *vcomCtx = VOS_NULL_PTR;

    for (loop = 0; loop < APP_VCOM_MAX_NUM; loop++) {
        vcomCtx = APP_VCOM_GetVcomCtxAddr(loop);

        if (vcomCtx->appVcomMajorId == majorDevId) {
            break;
        }
    }

    return loop;
}


VOS_VOID APP_VCOM_InitSpecCtx(VOS_UINT8 devIndex)
{
    APP_VCOM_DevCtx *vcomCtx = VOS_NULL_PTR;
    errno_t          memResult;

    if (devIndex >= APP_VCOM_MAX_NUM) {
        return;
    }

    vcomCtx = APP_VCOM_GetVcomCtxAddr(devIndex);

    (VOS_VOID)memset_s(vcomCtx->sendSemName, sizeof(vcomCtx->sendSemName), 0x00, sizeof(vcomCtx->sendSemName));
    (VOS_VOID)memset_s(vcomCtx->appVcomName, sizeof(vcomCtx->appVcomName), 0x00, sizeof(vcomCtx->appVcomName));

    if (VOS_StrLen(g_appVcomCogfigTab[devIndex].appVcomName) > 0) {
        memResult = memcpy_s(vcomCtx->appVcomName, sizeof(vcomCtx->appVcomName),
                             g_appVcomCogfigTab[devIndex].appVcomName,
                             VOS_StrLen(g_appVcomCogfigTab[devIndex].appVcomName));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(vcomCtx->appVcomName),
                            VOS_StrLen(g_appVcomCogfigTab[devIndex].appVcomName));
    }

    if (VOS_StrLen(g_appVcomCogfigTab[devIndex].sendSemName) > 0) {
        memResult = memcpy_s(vcomCtx->sendSemName, sizeof(vcomCtx->sendSemName),
                             g_appVcomCogfigTab[devIndex].sendSemName,
                             VOS_StrLen(g_appVcomCogfigTab[devIndex].sendSemName));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(vcomCtx->sendSemName),
                            VOS_StrLen(g_appVcomCogfigTab[devIndex].sendSemName));
    }

    vcomCtx->appVcomMajorId = APP_VCOM_MAJOR_DEV_ID + devIndex;
}


VOS_VOID APP_VCOM_Setup(APP_VCOM_DevEntity *dev, VOS_UINT8 indexNum)
{
    static struct class *comClass; /*lint !e565*/
    APP_VCOM_DevCtx     *vcomCtx;
    VOS_INT              iErr;
    dev_t                devno;

    vcomCtx = APP_VCOM_GetVcomCtxAddr(indexNum);

    devno = MKDEV(vcomCtx->appVcomMajorId, indexNum);

    cdev_init(&dev->appVcomDev, &g_operations_Fops);

    iErr = cdev_add(&dev->appVcomDev, devno, 1);
    if (iErr) {
        APP_VCOM_TRACE_ERR(indexNum, "APP_VCOM_Setup cdev_add error! ");
        return;
    }

    comClass = class_create(THIS_MODULE, vcomCtx->appVcomName);

    device_create(comClass, NULL, MKDEV(vcomCtx->appVcomMajorId, indexNum), "%s", vcomCtx->appVcomName);

    return;
}


VOS_INT APP_VCOM_Init(VOS_VOID)
{
    VOS_INT             iResult1;
    VOS_INT             iResult2;
    dev_t               devno;
    VOS_UINT32          indexNum;
    APP_VCOM_DevCtx    *vcomCtx  = VOS_NULL_PTR;
    APP_VCOM_DevEntity *vcomDevp = VOS_NULL_PTR;

    APP_VCOM_PR_LOGI("entry,%u", VOS_GetSlice());

    /* 初始化可维可测全局变量 */
    (VOS_VOID)memset_s(&g_appVcomDebugInfo, sizeof(g_appVcomDebugInfo), 0x00, sizeof(g_appVcomDebugInfo));

    (VOS_VOID)memset_s(&g_appVcomDebugCfg, sizeof(g_appVcomDebugCfg), 0x00, sizeof(g_appVcomDebugCfg));

    /* 初始化虚拟设备 */
    for (indexNum = 0; indexNum < APP_VCOM_MAX_NUM; indexNum++) {
#if (FEATURE_ON == FEATURE_VCOM_EXT)
#if (FEATURE_OFF == FEATURE_DEBUG_APP_PORT)
        if (VOS_TRUE == APP_VCOM_IsDebugAppPort(indexNum)) {
            APP_VCOM_TRACE_NORM(indexNum, "APP_VCOM_Init, DEBUG_APP_PORT not open, port is eng port, dont init. ");
            continue;
        }
#endif
#endif

        /* 初始化全局变量 */
        APP_VCOM_InitSpecCtx(indexNum);

        /* 获取全局变量指针 */
        vcomCtx = APP_VCOM_GetVcomCtxAddr(indexNum);

        /* 将设备号转换成dev_t 类型 */
        devno = MKDEV(vcomCtx->appVcomMajorId, indexNum);

        iResult1 = register_chrdev_region(devno, 1, vcomCtx->appVcomName);

        /* 注册失败则动态申请设备号 */
        if (iResult1 < 0) {
            iResult2 = alloc_chrdev_region(&devno, 0, 1, vcomCtx->appVcomName);

            if (iResult2 < 0) {
                return VOS_ERROR;
            }

            vcomCtx->appVcomMajorId = MAJOR(devno);
        }

        /* 动态申请设备结构体内存 */
        vcomCtx->appVcomDevEntity = kmalloc(sizeof(APP_VCOM_DevEntity), GFP_KERNEL);

        if (VOS_NULL_PTR == vcomCtx->appVcomDevEntity) {
            /* 去注册该设备，返回错误 */
            unregister_chrdev_region(devno, 1);
            APP_VCOM_TRACE_ERR(indexNum, "APP_VCOM_Init malloc device Entity fail. ");
            return VOS_ERROR;
        }

        /* 获取设备实体指针 */
        vcomDevp = vcomCtx->appVcomDevEntity;

        (VOS_VOID)memset_s(vcomDevp, sizeof(APP_VCOM_DevEntity), 0x00, sizeof(APP_VCOM_DevEntity));

        if (APPVCOM_STATIC_MALLOC_MEMORY(indexNum)) {
            vcomDevp->appVcomMem = kmalloc(g_appVcomCogfigTab[indexNum].appVcomMemSize, GFP_KERNEL);

            if (VOS_NULL_PTR == vcomDevp->appVcomMem) {
                /* 去注册该设备，返回错误 */
                unregister_chrdev_region(devno, 1);
                APP_VCOM_TRACE_ERR(indexNum, "APP_VCOM_Init malloc device buff fail. ");
                kfree(vcomCtx->appVcomDevEntity);
                vcomCtx->appVcomDevEntity = VOS_NULL_PTR;
                return VOS_ERROR;
            }
        }

        init_waitqueue_head(&vcomDevp->readWait);
        (VOS_VOID)memset_s(vcomDevp->wakeLockName, sizeof(vcomDevp->wakeLockName), 0x00, sizeof(vcomDevp->wakeLockName));
        scnprintf(vcomDevp->wakeLockName, APP_VCOM_RD_WAKE_LOCK_NAME_LEN, "appvcom%d_rd_wake", indexNum);
        vcomDevp->wakeLockName[APP_VCOM_RD_WAKE_LOCK_NAME_LEN - 1] = '\0';
        wakeup_source_init(&vcomDevp->rdWakeLock, vcomDevp->wakeLockName);

        mutex_init(&vcomDevp->mutex);

        APP_VCOM_Setup(vcomDevp, indexNum);

        /* 创建信号量 */
        sema_init(&vcomDevp->msgSendSem, 1);
        sema_init(&vcomDevp->wrtSem, 1);
    }

    APP_VCOM_PR_LOGI("eixt,%u", VOS_GetSlice());

    return VOS_OK;
}


int APP_VCOM_Release(struct inode *inode, struct file *filp)
{
    VOS_UINT         devMajor;
    VOS_UINT8        indexNum;
    APP_VCOM_DevCtx *vcomCtx = VOS_NULL_PTR;
#if (FEATURE_ON == FEATURE_VCOM_EXT)
    APP_VCOM_DevEntity *vcomDevp = VOS_NULL_PTR;
#endif

    if (VOS_NULL_PTR == inode || VOS_NULL_PTR == filp) {
        return VOS_ERROR;
    }

    /* 获取主设备号 */
    devMajor = imajor(inode);

    /* 根据主设备号得到设备在全局变量中的索引值 */
    indexNum = APP_VCOM_GetIndexFromMajorDevId(devMajor);

    if (indexNum >= APP_VCOM_MAX_NUM) {
        APP_VCOM_ERR_LOG(indexNum, "APP_VCOM_Release ucIndex is error. ");
        return VOS_ERROR;
    }

    /* 获取VCOM全局变量 */
    vcomCtx = APP_VCOM_GetVcomCtxAddr(indexNum);

    if (VOS_NULL_PTR == vcomCtx->appVcomDevEntity) {
        APP_VCOM_TRACE_ERR(indexNum, "APP_VCOM_Release VcomDevEntity is NULL. ");
        return VOS_ERROR;
    }

#if (FEATURE_ON == FEATURE_VCOM_EXT)
    if (APPVCOM_DYNAMIC_MALLOC_MEMORY(indexNum)) {
        vcomDevp = vcomCtx->appVcomDevEntity;

        down(&vcomDevp->msgSendSem);

        if (VOS_NULL_PTR != vcomDevp->appVcomMem) {
            kfree(vcomDevp->appVcomMem);
            vcomDevp->appVcomMem = VOS_NULL_PTR;
            APP_VCOM_TRACE_INFO(indexNum, "APP_VCOM_Release free memory is ok. ");
        }

        up(&vcomDevp->msgSendSem);
    }
#endif

    /* 将设备结构体指针赋值给文件私有数据指针 */
    filp->private_data = vcomCtx->appVcomDevEntity;

    APP_VCOM_TRACE_INFO(indexNum, "APP_VCOM_Release enter. ");

    if (NULL != vcomCtx->eventFunc) {
        (VOS_VOID)(vcomCtx->eventFunc(APP_VCOM_EVT_RELEASE));
    }

    vcomCtx->appVcomDevEntity->isDeviceOpen = VOS_FALSE;
    /*lint -save -e455 */
    __pm_relax(&vcomCtx->appVcomDevEntity->rdWakeLock);
    /*lint -restore */

    return VOS_OK;
}


int APP_VCOM_Open(struct inode *inode, struct file *filp)
{
    VOS_UINT         devMajor;
    VOS_UINT8        indexNum;
    APP_VCOM_DevCtx *vcomCtx = VOS_NULL_PTR;
#if (FEATURE_ON == FEATURE_VCOM_EXT)
    APP_VCOM_DevEntity *vcomDevp = VOS_NULL_PTR;
    VOS_UINT32          memSize;
#endif

    if (VOS_NULL_PTR == inode || VOS_NULL_PTR == filp) {
        return VOS_ERROR;
    }

    /* 获取主设备号 */
    devMajor = imajor(inode);

    /* 根据主设备号得到设备在全局变量中的索引值 */
    indexNum = APP_VCOM_GetIndexFromMajorDevId(devMajor);

    if (indexNum >= APP_VCOM_MAX_NUM) {
        APP_VCOM_ERR_LOG(indexNum, "APP_VCOM_Open ucIndex is error. ");
        return VOS_ERROR;
    }

    /* 获取VCOM全局变量 */
    vcomCtx = APP_VCOM_GetVcomCtxAddr(indexNum);

    if (VOS_NULL_PTR == vcomCtx->appVcomDevEntity) {
        APP_VCOM_TRACE_ERR(indexNum, "APP_VCOM_Open VcomDevEntity is NULL. ");
        return VOS_ERROR;
    }

#if (FEATURE_ON == FEATURE_VCOM_EXT)
    if (APPVCOM_DYNAMIC_MALLOC_MEMORY(indexNum)) {
        /* 获取设备实体指针 */
        vcomDevp = vcomCtx->appVcomDevEntity;

        down(&vcomDevp->msgSendSem);

        if (VOS_NULL_PTR == vcomDevp->appVcomMem) {
            memSize              = TAF_MIN(g_appVcomCogfigTab[indexNum].appVcomMemSize, KMALLOC_MAX_SIZE);
            vcomDevp->appVcomMem = kmalloc(memSize, GFP_KERNEL);

            if (VOS_NULL_PTR == vcomDevp->appVcomMem) {
                APP_VCOM_TRACE_ERR(indexNum, "APP_VCOM_Open alloc memory is err. ");
                up(&vcomDevp->msgSendSem);
                return VOS_ERROR;
            }
            APP_VCOM_TRACE_INFO(indexNum, "APP_VCOM_Open alloc memory is ok. ");
        }

        up(&vcomDevp->msgSendSem);
    }
#endif
    /* 将设备结构体指针赋值给文件私有数据指针 */
    filp->private_data = vcomCtx->appVcomDevEntity;

    APP_VCOM_TRACE_INFO(indexNum, "APP_VCOM_Open enter. ");

    if (NULL != vcomCtx->eventFunc) {
        (VOS_VOID)(vcomCtx->eventFunc(APP_VCOM_EVT_OPEN));
    }

    vcomCtx->appVcomDevEntity->isDeviceOpen = VOS_TRUE;

    return VOS_OK;
}


ssize_t APP_VCOM_Read(struct file *filp, char __user *buf, size_t count, loff_t *ppos)
{
    VOS_UINT            devMajor;
    VOS_UINT8           indexNum;
    struct cdev        *cdev    = VOS_NULL_PTR;
    APP_VCOM_DevEntity *vcomDev = VOS_NULL_PTR;
    APP_VCOM_DevCtx    *vcomCtx = VOS_NULL_PTR;
    errno_t             memResult;

    if ((filp == VOS_NULL_PTR) || (buf == VOS_NULL_PTR) || (ppos == VOS_NULL_PTR)) {
        return APP_VCOM_ERROR;
    }

    /* 获得设备结构体指针 */
    vcomDev = filp->private_data;

    /* 获得设备主设备号 */
    cdev     = &(vcomDev->appVcomDev);
    devMajor = MAJOR(cdev->dev);

    /* 获得设备在全局变量中的索引值 */
    indexNum = APP_VCOM_GetIndexFromMajorDevId(devMajor);

    if (indexNum >= APP_VCOM_MAX_NUM) {
        return APP_VCOM_ERROR;
    }

    vcomCtx = APP_VCOM_GetVcomCtxAddr(indexNum);

    APP_VCOM_TRACE_INFO(indexNum, "APP_VCOM_Read, read count:%u, current_len:%u. ", count,
                        vcomDev->currentLen); /*lint !e559 */
    APP_VCOM_TRACE_INFO(indexNum, "APP_VCOM_Read, f_flags:%d. ", filp->f_flags);
    APP_VCOM_TRACE_INFO(indexNum, "APP_VCOM_Read, wait_event 111, flag:%d. ", vcomDev->readWakeUpFlg);

    if (filp->f_flags & O_NONBLOCK) {
        return APP_VCOM_ERROR;
    }

#if (VOS_OS_VER == VOS_WIN32)

#else
    /*lint -e730 ;cause:two thread will write global variables */
    if (wait_event_interruptible(vcomDev->readWait, (vcomDev->currentLen != 0))) {
        return -ERESTARTSYS;
    }
    /*lint +e730 ;cause:two thread will write global variables */
#endif

    if (0 == vcomDev->currentLen) {
        g_appVcomDebugInfo.readLenErr[indexNum]++;
    }

    APP_VCOM_TRACE_INFO(indexNum, "APP_VCOM_Read, wait_event 222,flag:%d. ", vcomDev->readWakeUpFlg);

    /* 获取信号量 */
    down(&vcomCtx->appVcomDevEntity->msgSendSem);

    if (APPVCOM_DYNAMIC_MALLOC_MEMORY(indexNum) && (VOS_NULL_PTR == vcomDev->appVcomMem)) {
        up(&vcomCtx->appVcomDevEntity->msgSendSem);
        return APP_VCOM_ERROR;
    }

    if (count > vcomDev->currentLen) {
        count = vcomDev->currentLen;
    }

    if (copy_to_user(buf, vcomDev->appVcomMem, (VOS_ULONG)count)) {
        APP_VCOM_TRACE_ERR(indexNum, "APP_VCOM_Read, copy_to_user fail. ");

        /* 释放信号量 */
        up(&vcomCtx->appVcomDevEntity->msgSendSem);
        return APP_VCOM_ERROR;
    }

    if ((vcomDev->currentLen - count) > 0) {
        /* FIFO数据前移 */
        /*lint -save -e661*/
        memResult = memmove_s(vcomDev->appVcomMem, g_appVcomCogfigTab[indexNum].appVcomMemSize,
                              vcomDev->appVcomMem + count, vcomDev->currentLen - count);
        TAF_MEM_CHK_RTN_VAL(memResult, g_appVcomCogfigTab[indexNum].appVcomMemSize, vcomDev->currentLen - count);
        /*lint -restore +e661*/
        APP_VCOM_TRACE_INFO(indexNum, "APP_VCOM_Read, FIFO move. ");
    }

    /* 有效数据长度减小 */
    vcomDev->currentLen -= count;

    APP_VCOM_TRACE_INFO(indexNum, "APP_VCOM_Read, read %u bytes, current_len:%u. ", count,
                        vcomDev->currentLen); /*lint !e559 */

    if (0 == vcomDev->currentLen) {
        APP_VCOM_TRACE_NORM(indexNum, "APP_VCOM_Send: read all data. ");
        /*lint -save -e455 */
        __pm_relax(&vcomDev->rdWakeLock);
        /*lint -restore */
    }

    /* 释放信号量 */
    up(&vcomCtx->appVcomDevEntity->msgSendSem);

    return (ssize_t)count;
}


ssize_t APP_VCOM_Write(struct file *filp, const char __user *buf, size_t count, loff_t *ppos)
{
    VOS_INT             iRst;
    VOS_UINT            devMajor;
    VOS_UINT8           indexNum;
    struct cdev        *cdev    = VOS_NULL_PTR;
    VOS_UINT8          *dataBuf = VOS_NULL_PTR;
    APP_VCOM_DevEntity *vcomDev = VOS_NULL_PTR;
    APP_VCOM_DevCtx    *vcomCtx = VOS_NULL_PTR;

    /* 获得设备结构体指针 */
    vcomDev = filp->private_data;

    /* 获得设备主设备号 */
    cdev     = &(vcomDev->appVcomDev);
    devMajor = MAJOR(cdev->dev);

    /* 获得设备在全局变量中的索引值 */
    indexNum = APP_VCOM_GetIndexFromMajorDevId(devMajor);

    if (indexNum >= APP_VCOM_MAX_NUM) {
        APP_VCOM_ERR_LOG(indexNum, "APP_VCOM_Write, ucIndex fail. ");
        return APP_VCOM_ERROR;
    }

    if (NULL == buf) {
        APP_VCOM_TRACE_ERR(indexNum, "APP_VCOM_Write, buf is null ");
        return APP_VCOM_ERROR;
    }

    if ((count > APP_VCOM_MAX_DATA_LENGTH) || (0 == count)) {
        APP_VCOM_TRACE_ERR(indexNum, "APP_VCOM_Write, count is %d, it is error", count);
        return APP_VCOM_ERROR;
    }

    /* 获得全局变量地址 */
    vcomCtx = APP_VCOM_GetVcomCtxAddr(indexNum);

    /* 申请内存 */
    dataBuf = kmalloc(count, GFP_KERNEL);
    if (VOS_NULL_PTR == dataBuf) {
        APP_VCOM_TRACE_ERR(indexNum, "APP_VCOM_Write, kmalloc fail. ");

        return APP_VCOM_ERROR;
    }

    /* buffer清零 */
    (VOS_VOID)memset_s(dataBuf, count, 0x00, (VOS_SIZE_T)count);

    if (copy_from_user(dataBuf, buf, (VOS_ULONG)count)) {
        APP_VCOM_TRACE_ERR(indexNum, "APP_VCOM_Write, copy_from_user fail. ");

        kfree(dataBuf);
        dataBuf = VOS_NULL_PTR;
        return APP_VCOM_ERROR;
    }

#if (FEATURE_ON == FEATURE_VCOM_EXT)
    if (VOS_TRUE == APP_VCOM_ProcAgpsPortCache_InWrite(dataBuf, count, indexNum)) {
        return (ssize_t)count; /*lint !e429*/
    }
#endif

    /* 调用回调函数处理buf中的AT码流 */
    if (VOS_NULL_PTR == vcomCtx->sendUlAtFunc) {
        APP_VCOM_TRACE_ERR(indexNum, "APP_VCOM_Write, pSendUlAtFunc is null. ");
        kfree(dataBuf);
        dataBuf = VOS_NULL_PTR;
        return APP_VCOM_ERROR;
    }

    mutex_lock(&vcomDev->mutex);

    iRst = vcomCtx->sendUlAtFunc(indexNum, dataBuf, (VOS_UINT32)count);
    if (VOS_OK != iRst) {
        g_appVcomDebugInfo.atCallBackErr[indexNum]++;
        APP_VCOM_TRACE_ERR(indexNum, "APP_VCOM_Write, AT_RcvCmdData fail. ");
        mutex_unlock(&vcomDev->mutex);
        kfree(dataBuf);
        dataBuf = VOS_NULL_PTR;

        return APP_VCOM_ERROR;
    }

    mutex_unlock(&vcomDev->mutex);

    APP_VCOM_TRACE_INFO(indexNum, "APP_VCOM_Write, write %d bytes, AT_RcvCmdData Success.", count);

    /* 释放内存 */
    kfree(dataBuf);
    dataBuf = VOS_NULL_PTR;

    return (ssize_t)count;
}


unsigned int APP_VCOM_Poll(struct file *fp, struct poll_table_struct *wait)
{
    APP_VCOM_DevEntity *vcomDev = VOS_NULL_PTR;
    unsigned int        mask    = 0;

    struct cdev *cdev;
    VOS_UINT     devMajor;
    VOS_UINT8    indexNum;

    vcomDev = fp->private_data;

    cdev     = &(vcomDev->appVcomDev);
    devMajor = MAJOR(cdev->dev);
    indexNum = APP_VCOM_GetIndexFromMajorDevId(devMajor);

    poll_wait(fp, &vcomDev->readWait, wait);

    if (0 != vcomDev->currentLen) {
        mask |= POLLIN | POLLRDNORM;
    }

    APP_VCOM_TRACE_INFO(indexNum, "APP_VCOM_Poll, mask = %d. ", mask);

    return mask;
}


VOS_UINT32 APP_VCOM_Send(APP_VCOM_DevIndexUint8 devIndex, VOS_UINT8 *data, VOS_UINT32 uslength)
{
    APP_VCOM_DevEntity *vcomDev = VOS_NULL_PTR;
#if (FEATURE_ON == FEATURE_VCOM_EXT)
    APP_VCOM_DevCtx *vcomCtx = VOS_NULL_PTR;
#endif
    errno_t memResult;

    if (devIndex >= APP_VCOM_MAX_NUM) {
        g_appVcomDebugInfo.devIndexErr++;
        APP_VCOM_ERR_LOG(devIndex, "APP_VCOM_Send, enDevIndex is error. ");
        return VOS_ERR;
    }

#if (FEATURE_ON == FEATURE_VCOM_EXT)
    vcomCtx = APP_VCOM_GetVcomCtxAddr(devIndex);
    if (VOS_NULL_PTR == vcomCtx) {
        APP_VCOM_TRACE_ERR(devIndex, "APP_VCOM_Send, pstVcomCtx is null. ");
        return VOS_ERR;
    }

#if (FEATURE_OFF == FEATURE_DEBUG_APP_PORT)
    if (VOS_TRUE == APP_VCOM_IsDebugAppPort(devIndex)) {
        APP_VCOM_TRACE_NORM(devIndex, "APP_VCOM_Send, DEBUG_APP_PORT not open, port is eng port, not send. ");
        return VOS_OK;
    }
#endif
#endif

    /* 获得设备实体指针 */
    vcomDev = APP_VCOM_GetAppVcomDevEntity(devIndex);
    if (VOS_NULL_PTR == vcomDev) {
        g_appVcomDebugInfo.vcomDevErr[devIndex]++;
        APP_VCOM_TRACE_ERR(devIndex, "APP_VCOM_Send, pstVcomDev is null. ");
        return VOS_ERR;
    }

#if (FEATURE_ON == FEATURE_VCOM_EXT)
    APP_VCOM_ProcAgpsPortCache_InSend(devIndex);
#endif

    APP_VCOM_TRACE_INFO(devIndex, "APP_VCOM_Send, uslength:%d, current_len:%d. ", uslength, vcomDev->currentLen);

    /* 获取信号量 */
    down(&vcomDev->msgSendSem);

    if (APPVCOM_DYNAMIC_MALLOC_MEMORY(devIndex) && (VOS_NULL_PTR == vcomDev->appVcomMem)) {
        up(&vcomDev->msgSendSem);
        return VOS_ERR;
    }

    /* 队列满则直接返回 */
    /*lint -e661*/
    if (g_appVcomCogfigTab[devIndex].appVcomMemSize == vcomDev->currentLen) {
        /*lint +e661*/
        if (VOS_TRUE == vcomDev->isDeviceOpen) {
            APP_VCOM_ERR_LOG(devIndex, "APP_VCOM_Send: VCOM MEM FULL. ");
        }

        /*lint -e661*/
        g_appVcomDebugInfo.memFullErr[devIndex]++;
        /*lint +e661*/

        up(&vcomDev->msgSendSem);
        return VOS_ERR;
    }

    /* 发送数据大于剩余Buffer大小 */
    /*lint -e661*/
    if (uslength > (g_appVcomCogfigTab[devIndex].appVcomMemSize - vcomDev->currentLen)) {
        /*lint +e661*/
        APP_VCOM_TRACE_NORM(devIndex, "APP_VCOM_Send: data more than Buffer. ");

        if (VOS_TRUE == vcomDev->isDeviceOpen) {
            APP_VCOM_ERR_LOG(devIndex, "APP_VCOM_Send: VCOM MEM FULL. ");
        }

        /*lint -e661*/
        uslength = g_appVcomCogfigTab[devIndex].appVcomMemSize - (VOS_UINT32)vcomDev->currentLen;
        /*lint +e661*/
    }

    /* 复制到BUFFER */
    if (uslength > 0) {
        /*lint -save -e661*/
        memResult = memcpy_s(vcomDev->appVcomMem + vcomDev->currentLen,
                             g_appVcomCogfigTab[devIndex].appVcomMemSize - (VOS_UINT32)vcomDev->currentLen,
                             data,
                             uslength);
        TAF_MEM_CHK_RTN_VAL(memResult, g_appVcomCogfigTab[devIndex].appVcomMemSize - (VOS_UINT32)vcomDev->currentLen,
                            uslength);
        /*lint -restore +e661*/
    }
    vcomDev->currentLen += uslength;

    APP_VCOM_TRACE_INFO(devIndex, "APP_VCOM_Send, written %d byte(s), new len: %d. ", uslength, vcomDev->currentLen);

    APP_VCOM_TRACE_INFO(devIndex, "APP_VCOM_Send, IsDeviceOpen: %d. ", vcomDev->isDeviceOpen);
    if (VOS_TRUE == vcomDev->isDeviceOpen) {
        __pm_wakeup_event(&vcomDev->rdWakeLock, APP_VCOM_READ_WAKE_LOCK_LEN);
    }

    /* 释放信号量 */
    up(&vcomDev->msgSendSem);
    wake_up_interruptible(&vcomDev->readWait);

    if (0 == vcomDev->currentLen) {
        /*lint -e661*/
        g_appVcomDebugInfo.sendLenErr[devIndex]++;
        /*lint +e661*/
    }

    APP_VCOM_TRACE_INFO(devIndex, "APP_VCOM_Send, wakeup. ");
    return VOS_OK;
}

#if (FEATURE_ON == FEATURE_VCOM_EXT)

VOS_UINT32 APP_VCOM_ProcAgpsPortCache_InWrite(VOS_UINT8 *dataBuf, size_t count, VOS_UINT8 indexNum)
{
    APP_VCOM_DevCtx *vcomCtx;

    vcomCtx = APP_VCOM_GetVcomCtxAddr(indexNum);

    if ((APP_VCOM_DEV_INDEX_9 == indexNum) || (APP_VCOM_DEV_INDEX_12 == indexNum)) {
        down(&vcomCtx->appVcomDevEntity->wrtSem);

        if (VOS_NULL_PTR != vcomCtx->appVcomDevEntity->wrtBuffer) {
            APP_VCOM_TRACE_NORM(indexNum, "APP_VCOM_Write: free buff. ");

            kfree(vcomCtx->appVcomDevEntity->wrtBuffer);
            vcomCtx->appVcomDevEntity->wrtBuffer    = VOS_NULL_PTR;
            vcomCtx->appVcomDevEntity->wrtBufferLen = 0;
        }

        if (VOS_NULL_PTR == vcomCtx->sendUlAtFunc) {
            APP_VCOM_TRACE_NORM(indexNum, "APP_VCOM_Write: save buff. ");

            vcomCtx->appVcomDevEntity->wrtBuffer    = dataBuf;
            vcomCtx->appVcomDevEntity->wrtBufferLen = count;
            up(&vcomCtx->appVcomDevEntity->wrtSem);

            return VOS_TRUE;
        }

        up(&vcomCtx->appVcomDevEntity->wrtSem);
    }

    return VOS_FALSE;
} /*lint !e429*/


VOS_VOID APP_VCOM_ProcAgpsPortCache_InSend(APP_VCOM_DevIndexUint8 devIndex)
{
    APP_VCOM_DevEntity *vcomDev;
    APP_VCOM_DevCtx    *vcomCtx;

    vcomCtx = APP_VCOM_GetVcomCtxAddr(devIndex);

    /* 获得设备实体指针 */
    vcomDev = APP_VCOM_GetAppVcomDevEntity(devIndex);

    if ((APP_VCOM_DEV_INDEX_9 == devIndex) || (APP_VCOM_DEV_INDEX_12 == devIndex)) {
        down(&vcomDev->wrtSem);

        if (VOS_NULL_PTR != vcomDev->wrtBuffer) {
            if (VOS_NULL_PTR != vcomCtx->sendUlAtFunc) {
                APP_VCOM_TRACE_NORM(devIndex, "APP_VCOM_Send: handle buff. ");

                (VOS_VOID)vcomCtx->sendUlAtFunc(devIndex, vcomDev->wrtBuffer, vcomDev->wrtBufferLen);

                kfree(vcomDev->wrtBuffer);
                vcomDev->wrtBuffer    = VOS_NULL_PTR;
                vcomDev->wrtBufferLen = 0;
            }
        }

        up(&vcomDev->wrtSem);
    }
}
#endif


VOS_VOID APP_VCOM_ShowDebugInfo(VOS_VOID)
{
    int i;
    PS_PRINTF_INFO("App Vcom Debug Info:\n");
    PS_PRINTF_INFO("Index Err: %d\n", g_appVcomDebugInfo.devIndexErr);

    for (i = 0; i < APP_VCOM_MAX_NUM; i++) {
        PS_PRINTF_INFO("AppVcom[%d] Callback Function Return Err Num: %d\n", i, g_appVcomDebugInfo.atCallBackErr[i]);
        PS_PRINTF_INFO("AppVcom[%d] Mem Full Num:                     %d\n", i, g_appVcomDebugInfo.memFullErr[i]);
        PS_PRINTF_INFO("AppVcom[%d] Read Data Length = 0 Num:         %d\n", i, g_appVcomDebugInfo.readLenErr[i]);
        PS_PRINTF_INFO("AppVcom[%d] Send Data Length = 0 Num:         %d\n", i, g_appVcomDebugInfo.sendLenErr[i]);
        PS_PRINTF_INFO("AppVcom[%d] Get App Vcom Dev Entity Err Num:  %d\n", i, g_appVcomDebugInfo.vcomDevErr[i]);
    }
}

#if (VOS_WIN32 == VOS_OS_VER)

VOS_VOID APP_VCOM_FreeMem(VOS_VOID)
{
    APP_VCOM_DevCtx    *vcomCtx;
    VOS_UINT32          indexNum;
    APP_VCOM_DevEntity *vcomDevp;

    vcomCtx  = VOS_NULL_PTR;
    vcomDevp = VOS_NULL_PTR;

    for (indexNum = 0; indexNum < APP_VCOM_MAX_NUM; indexNum++) {
#if (FEATURE_ON == FEATURE_VCOM_EXT)
#if (FEATURE_OFF == FEATURE_DEBUG_APP_PORT)
        if (VOS_TRUE == APP_VCOM_IsDebugAppPort(indexNum)) {
            continue;
        }
#endif
#endif

        vcomCtx = APP_VCOM_GetVcomCtxAddr(indexNum);

        vcomDevp = vcomCtx->appVcomDevEntity;

        if (APPVCOM_STATIC_MALLOC_MEMORY(indexNum)) {
            if (VOS_NULL_PTR != vcomDevp->appVcomMem) {
                free(vcomDevp->appVcomMem);
                vcomDevp->appVcomMem = VOS_NULL_PTR;
            }
        }

        if (VOS_NULL_PTR != vcomCtx->appVcomDevEntity) {
            free(vcomCtx->appVcomDevEntity);
            vcomCtx->appVcomDevEntity = VOS_NULL_PTR;
        }
    }

    return;
}
#endif


VOS_VOID APP_VCOM_SendDebugNvCfg(VOS_UINT32 portIdMask1, VOS_UINT32 portIdMask2, VOS_UINT32 debugLevel)
{
    g_appVcomDebugCfg.portIdMask1 = portIdMask1;
    g_appVcomDebugCfg.portIdMask2 = portIdMask2;
    g_appVcomDebugCfg.debugLevel  = debugLevel;
}


VOS_VOID APP_VCOM_MNTN_LogPrintf(VOS_UINT32 lvl, VOS_CHAR *pcFmt, ...)
{
    VOS_CHAR   acBuf[APP_VCOM_TRACE_BUF_LEN] = {0};
    VOS_UINT32 printLength                   = 0;

    /* 格式化输出BUFFER */
    /*lint -e713 -e507 -e530*/
    APP_VCOM_LOG_FORMAT(printLength, acBuf, APP_VCOM_TRACE_BUF_LEN, pcFmt);
    /*lint +e713 +e507 +e530*/

    if (APP_VCOM_TRACE_LEVEL_ERR == lvl) {
        APP_VCOM_PR_LOGE("%s", acBuf);
    } else if ((APP_VCOM_TRACE_LEVEL_INFO == lvl) || (APP_VCOM_TRACE_LEVEL_NORM == lvl)) {
        APP_VCOM_PR_LOGI("%s", acBuf);
    } else {
    }

    return;
}

#if (VOS_LINUX == VOS_OS_VER)
#if (FEATURE_ON == FEATURE_DELAY_MODEM_INIT)
#ifndef CONFIG_HISI_BALONG_MODEM_MODULE
module_init(APP_VCOM_Init);
#endif
#endif
#endif

