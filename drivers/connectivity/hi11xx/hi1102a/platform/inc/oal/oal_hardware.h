

#ifndef __OAL_HARDWARE_H__
#define __OAL_HARDWARE_H__

/* ����ͷ�ļ����� */
#include "oal_types.h"
#include "oal_pci_if.h"
#include "arch/oal_hardware.h"

/* �궨�� */
#define OAL_CONST const

#define OAL_IRQ_NUM 5

#define PCI_ANY_ID (~0)

#define OAL_IRQ_INIT_MAC_DEV(_dev, _irq, _type, _name, _arg, _func) \
    do {                                                            \
        (_dev).ul_irq = (_irq);                                     \
        (_dev).l_irq_type = (_type);                                \
        (_dev).pc_name = (_name);                                   \
        (_dev).p_drv_arg = (_arg);                                  \
        (_dev).p_irq_intr_func = (_func);                           \
    } while (0)

#define OAL_PCI_GET_DEV_ID(_dev) ((_dev)->device);

#define MAX_NUM_CORES 2

/* TIMER�ⲿ���ƼĴ��� */
typedef struct {
    volatile oal_uint32 *pul_sc_ctrl;
} oal_hi_timer_ctrl_reg_stru;

#endif /* end of oal_hardware.h */
