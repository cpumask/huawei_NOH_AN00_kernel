/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description:  this file define info or valiable for smmuv3 tbu
 * Author:  chenweiyu
 * Create:  2019-07-30
 */
#ifndef _IVP_SMMU_V3_H_
#define _IVP_SMMU_V3_H_

#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/device.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <linux/clk.h>
#include <linux/mm.h>
#include <linux/iommu.h>
#include <linux/hisi-iommu.h>
#include <dsm/dsm_pub.h>

#define   SMMU_TCU_CTRL_SCR                 0x90
#define   TCU_UARCH_NON_SEC                 0
#define   TCU_UARCH_SEC                     1

#define   SMMU_TCU_SCR                      0x8E18
#define   TCU_INIT_UARCH_SEC                (1<<3 + 1<<0)

#define   SMMU_TCU_LP_REQ                   0x0000
#define   TCU_QREQN_CG_LEAVE                (1<<0)
#define   TCU_QREQN_PD_LEAVE                (1<<1)

#define   SMMU_TCU_LP_ACK                   0x0004
#define   TCU_AQCCEPTN_CG_NOT               (1<<0)
#define   TCU_AQCCEPTN_PD_NOT               (4<<0)

// tbu reg definition
#define   SMMU_TBU_SCR                      0x1000
#define   TBU_BYPASS                        (1<<3)
#define   TBU_MTLB_HITMP_DIS                (1<<2)
#define   TBU_HAZARD_DIS                    (1<<1)
#define   TBU_UARCH_NON_SEC                 (1<<0)
#define   TBU_NS_UARCH_MASK                 (1<<0)

#define   SMMU_TBU_CR                       0x0000
#define   MAX_TOK_TRANS_IVP                 15
#define   TBU_EN_REQ_MASK                   (1<<0)
#define   TBU_EN_REQ_ENABLE                 (1<<0)

#define   SMMU_TBU_CRACK                    0x0004
#define   TBU_EN_ACK                        (1<<0)
#define   TBU_CONNECTED_MASK                (1<<1)
#define   TBU_CONNECTED                     (1<<1)
#define   TBU_DISCONNECTED                  (0<<1)
#define   TBU_TOK_TRANS_MASK                (0xFF<<8)
#define   TBU_TOK_TRANS_SHIFT               8

#define   MAX_CHECK_TIMES                   1000

#define   PREFSLOT_FULL_LEVEL               16
#define   PREFSLOT_FULL_LEVEL_OFFSET        24
#define   FETCHSLOT_FULL_LEVEL              16
#define   FETCHSLOT_FULL_LEVEL_OFFSET       16

enum smmu_state {
	SMMU_STATE_DISABLE,    // SMMU bypass mode
	SMMU_STATE_ENABLE,    // SMMU enable
};

typedef void(*smmu_err_handler_t)(void);

struct ivp_smmu_dev {
	struct device  *dev;
	void __iomem   *reg_base;
	unsigned long   reg_size;
	spinlock_t      spinlock;
	irq_handler_t   isr;
	unsigned int    version;
	unsigned int    irq;
	unsigned int    state;
	smmu_err_handler_t  err_handler;
};

extern struct dsm_client *client_ivp;

void ivp_dsm_error_notify(int error_no);

int  ivp_smmu_trans_enable(struct ivp_smmu_dev *smmu_dev);
int  ivp_smmu_trans_disable(struct ivp_smmu_dev *smmu_dev);

struct ivp_smmu_dev *ivp_smmu_get_device(unsigned long select);


#endif /* _IVP_SMMU_V3H_  */
