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

#ifndef _BSP_PMU_H_
#define _BSP_PMU_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "product_config.h"
#include "osl_types.h"
#include <hi_pmu.h>

/*PMU������ܴ�С��3*1024*/
#define SHM_PMU_OCP_INFO_SIZE 0x10
#define SHM_PMU_NPREG_SIZE 0x10
#define SHM_PMU_OCP_INFO_ADDR ((unsigned long)SHM_BASE_ADDR + SHM_OFFSET_PMU)
#define SHM_MEM_PMU_NPREG_ADDR (SHM_PMU_OCP_INFO_ADDR + SHM_PMU_OCP_INFO_SIZE)

/*�ж�ddr�Ƿ񱻸�д��ħ����*/
#define SHM_PMU_VOLTTABLE_MAGIC_START_DATA   0xc7c7c7c7
#define SHM_PMU_VOLTTABLE_MAGIC_END_DATA     0xa6a6a6a6
#define BSP_PMU_ERROR -1
#define BSP_PMU_OK 0
#define BSP_PMU_NO_PMIC 0x2001
#define BSP_PMU_PARA_ERROR 0x2002
#define BSP_PMU_VOLTTABLE_ERROR 0x2003 /*DDR����д��table���ƻ�*/

#define PMU_STATE_OK    0
#define PMU_STATE_UNDER_VOL (1 << 0)
#define PMU_STATE_OVER_VOL  (1 << 1)
#define PMU_STATE_OVER_CUR  (1 << 2)
#define PMU_STATE_OVER_TEMP (1 << 3)
#define PMIC_VERSION_REG0 0x00
#define PMIC_VERSION_REG1 0x01
#define PMIC_VERSION_REG2 0x02
#define PMIC_VERSION_REG3 0x03
#define PMIC_VERSION_REG4 0x04
#define PMIC_VERSION_REG5 0x05

typedef void (*pmufuncptr)(void *);
typedef enum {
    PMU_CLK_ID_0 = 0,
    PMU_CLK_ID_1,
    PMU_CLK_ID_2,
    PMU_CLK_ID_3,
} pmu_clk_e;

typedef void (*PMU_OCP_FUNCPTR)(int);
int bsp_pmu_ocp_register(int volt_id, PMU_OCP_FUNCPTR func);
/*��������*/

/*****************************************************************************
 �� �� ��  : pmic_get_base_addr
 ��������  :���� pmu ����ַ
 �������  : void
 �������  :
 �� �� ֵ  : pmu ����ַ
 ���ú���  :
 ��������  : ϵͳ��ά�ɲ�
*****************************************************************************/
u32 pmic_get_base_addr(void);
/*****************************************************************************
 �� �� ��  : bsp_pmu_get_boot_state
 ��������  :ϵͳ����ʱ���pmu�Ĵ���״̬��
                ȷ���Ƿ�����pmu���������
 �������  : void
 �������  : reset.log
 �� �� ֵ  : pmu�����ok
 ���ú���  :
 ��������  :ϵͳ��ά�ɲ�
*****************************************************************************/
int bsp_pmu_get_boot_state(void);
/*****************************************************************************
 �� �� ��  : bsp_pmu_32k_clk_enable
 ��������  : ����pmu��32kʱ��
 �������  : clk_id:32kʱ��ö��ֵ
 �������  : ��
 �� �� ֵ  : �����ɹ���ʧ��
 ���ú���  :
 ��������  :
*****************************************************************************/
int bsp_pmu_clk_on(pmu_clk_e clk_id);
/*****************************************************************************
 �� �� ��  : bsp_pmu_32k_clk_disable
 ��������  : �ر�pmu��32kʱ��
 �������  : clk_id:32kʱ��ö��ֵ
 �������  : ��
 �� �� ֵ  : �رճɹ���ʧ��
 ���ú���  :
 ��������  :
*****************************************************************************/
int bsp_pmu_clk_off(pmu_clk_e clk_id);
/*****************************************************************************
 �� �� ��  : bsp_pmu_32k_clk_is_enabled
 ��������  : ��ѯpmu��32kʱ���Ƿ���
 �������  : clk_id:32kʱ��ö��ֵ
 �������  : ��
 �� �� ֵ  : ������ر�
 ���ú���  :
 ��������  :
*****************************************************************************/
int bsp_pmu_get_clk_status(pmu_clk_e clk_id);
/*****************************************************************************
 �� �� ��  : bsp_pmu_version_get
 ��������  : ��ȡusb�Ƿ���״̬
 �������  : void
 �������  : ��
 �� �� ֵ  : usb�����γ�:1:����;0:�γ�
 ���ú���  :
 ��������  : ���ػ�ģ��
*****************************************************************************/
bool bsp_pmu_usb_state_get(void);
/*****************************************************************************
 �� �� ��  : bsp_pmu_irq_callback_register
 ��������  : ע���жϴ���ص�����
 �������  : irq
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  : ������pmu�ڲ����ж�ģ��
*****************************************************************************/
void bsp_pmu_irq_mask(unsigned int irq);
/*****************************************************************************
 �� �� ��  : bsp_pmu_irq_callback_register
 ��������  : ע���жϴ���ص�����
 �������  : irq
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  : ������pmu�ڲ����ж�ģ��
*****************************************************************************/
void bsp_pmu_irq_unmask(unsigned int irq);
/*****************************************************************************
 �� �� ��  : bsp_pmu_irq_callback_register
 ��������  : ע���жϴ���ص�����
 �������  : irq
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  : ������pmu�ڲ����ж�ģ��
*****************************************************************************/
int bsp_pmu_irq_is_masked(unsigned int irq);
/*****************************************************************************
 �� �� ��  : bsp_pmu_key_state_get
 ��������  : ��ȡ�����Ƿ���״̬
 �������  : void
 �������  : ��
 �� �� ֵ  : �����Ƿ���:1:���£�0:δ����
 ���ú���  :
 ��������  : ���ػ�ģ��
*****************************************************************************/
bool bsp_pmu_key_state_get(void);
/*****************************************************************************
 �� �� ��  : bsp_pmu_irq_callback_register
 ��������  : ע���жϴ���ص�����
 �������  : irq
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  : ������pmu�ڲ����ж�ģ��
*****************************************************************************/
int bsp_pmu_irq_callback_register(unsigned int irq, pmufuncptr routine, void *data);
void *bsp_pmic_get_base_addr(void);
char *bsp_pmu_version_get(void);
int bsp_pmu_reg_write(u32 pmu_id, u32 addr, u32 value);
int bsp_pmu_reg_write_mask(u32 pmu_id, u32 addr, u32 value, u32 mask);
int bsp_pmu_reg_read(u32 pmu_id, u32 addr, u32 *value);
int bsp_pmu_reg_show(u32 pmu_id, u32 addr);

#ifdef __cplusplus
}
#endif

#endif /* end #define _BSP_PMU_H_ */
