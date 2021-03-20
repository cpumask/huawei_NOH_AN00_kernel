

#ifndef __OAL_BUS_IF_H__
#define __OAL_BUS_IF_H__

/* ����ͷ�ļ����� */
#include "oal_mem.h"
#include "oal_schedule.h"
#include "oal_util.h"
#include "oal_workqueue.h"
#include "oal_sdio_comm.h"

/* �궨�� */
#define OAL_MAX_BAR_NUM 6 /* ÿ���豸���BAR���� */

#define OAL_1151_IRQ_NAME_BUFF_SIZE    4
/* STRUCT���� */
/* �����豸��DEVICE�� */
typedef struct {
    oal_uint32 ul_device_id; /* �豸��,��0x1151 */
    oal_void *p_dev;         /* ָ���豸��ָ�� */
    oal_uint32 ul_irq_num;   /* �豸�жϺ� */
    oal_uint8 uc_chip_id;    /* PCIe�ţ���PCIe0 */
    oal_uint8 uc_bar_num;    /* BAR���� */
    oal_uint8 auc_resv[2];
    oal_uint32 aul_mem_start[1];
    oal_irq_dev_stru st_irq_info;
} oal_bus_dev_stru;

/* �����豸��CHIP�� */
typedef struct {
    oal_uint8 uc_chip_id;    /* PCIe�ţ���PCIe0 */
    oal_uint8 uc_device_num; /* ��оƬ��device������Ӧ��С��WLAN_DEVICE_MAX_NUM_PER_CHIP */
    oal_uint8 auc_resv[2];
    oal_pci_dev_stru *pst_pci_device; /* ָ������PCI�豸��ָ�� */
    oal_void *p_pci_dbi_base;         /* ָ��PCI�豸DBI�ӿڵ�ָ�룬��������PCIE�ڲ��Ĵ��� */
    oal_uint32 aul_mem_start[1];
    /* device��Ϣ */
    oal_bus_dev_stru st_bus_dev[WLAN_DEVICE_MAX_NUM_PER_CHIP];
} oal_bus_chip_stru;

/* �������� */
extern oal_uint8 oal_bus_get_chip_num(oal_void);
extern oal_uint32 oal_bus_inc_chip_num(oal_void);
extern oal_void oal_bus_init_chip_num(oal_void);

#endif /* end of oal_bus_if.h */
