

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

#define OAL_1151_IRQ_NAME_BUFF_SIZE 4

/* �����豸��DEVICE�� */
typedef struct {
    uint32_t ul_device_id; /* �豸��,��0x1151 */
    void *p_dev;         /* ָ���豸��ָ�� */
    uint32_t ul_irq_num;   /* �豸�жϺ� */
    uint8_t uc_chip_id;    /* PCIe�ţ���PCIe0 */
    uint8_t uc_bar_num;    /* BAR���� */
    uint8_t auc_resv[2];
    uint32_t aul_mem_start[1];
    oal_irq_dev_stru st_irq_info;
} oal_bus_dev_stru;

/* �����豸��CHIP�� */
typedef struct {
    uint8_t uc_chip_id;    /* PCIe�ţ���PCIe0 */
    uint8_t uc_device_num; /* ��оƬ��device������Ӧ��С��WLAN_DEVICE_MAX_NUM_PER_CHIP */
    uint8_t auc_resv[2];
    oal_pci_dev_stru *pst_pci_device; /* ָ������PCI�豸��ָ�� */
    void *p_pci_dbi_base;         /* ָ��PCI�豸DBI�ӿڵ�ָ�룬��������PCIE�ڲ��Ĵ��� */
    uint32_t aul_mem_start[1];

    /* device��Ϣ */
    oal_bus_dev_stru st_bus_dev[WLAN_DEVICE_MAX_NUM_PER_CHIP];
} oal_bus_chip_stru;

/* �������� */
extern uint8_t oal_bus_get_chip_num(void);
extern uint32_t oal_bus_inc_chip_num(void);
extern void oal_bus_init_chip_num(void);

#endif /* end of oal_bus_if.h */
