

#ifndef __OAL_LINUX_PCI_IF_H__
#define __OAL_LINUX_PCI_IF_H__

/* ����ͷ�ļ����� */
#include "oal_types.h"
#include "oal_util.h"
#include "oal_hardware.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_OAL_PCI_IF_H

/* �궨�� */
#ifndef PCI_EXP_DEVCTL2_LTR_EN
#define PCI_EXP_DEVCTL2_LTR_EN 0x0400 /* Enable LTR mechanism */
#endif

/* indicate 1106 pci0&pci1
 * 0xFFC~0xFFF */
#define PCI_EXP_DEVICE2_DUAL_FLAG   0xFFC

/* �������� */
extern void (*arm_pm_restart)(char str, const char *cmd);

/*
 * �� �� ��  : oal_pci_register_driver
 * ��������  : PCI����ע��
 * �������  : pst_drv: ָ��Ҫע�������
 * �� �� ֵ  : Linux������
 */
OAL_STATIC OAL_INLINE int32_t oal_pci_register_driver(oal_pci_driver_stru *pst_drv)
{
    return pci_register_driver(pst_drv);
}

/*
 * �� �� ��  : oal_pci_register_driver
 * ��������  : PCI����ȥע��
 * �������  : pst_drv: ָ��Ҫȥע�������
 * �� �� ֵ  : Linux������
 */
OAL_STATIC OAL_INLINE void oal_pci_unregister_driver(oal_pci_driver_stru *pst_drv)
{
    pci_unregister_driver(pst_drv);
}

/*
 * �� �� ��  : oal_pci_enable_device
 * ��������  : ʹ��PCI�豸
 * �������  : pst_dev: PCI�豸
 * �� �� ֵ  : Linux������
 */
OAL_STATIC OAL_INLINE int32_t oal_pci_enable_device(oal_pci_dev_stru *pst_dev)
{
    return pci_enable_device(pst_dev);
}

/*
 * �� �� ��  : oal_pci_disable_device
 * ��������  : ��ֹPCI�豸
 */
OAL_STATIC OAL_INLINE void oal_pci_disable_device(oal_pci_dev_stru *pst_dev)
{
    pci_disable_device(pst_dev);
}

/*
 * �� �� ��  : oal_pci_read_config_byte
 * ��������  : �����ÿռ��ȡһ���ֽ�
 * �������  : pst_dev : ָ��PCI�豸
 *             ul_where: ���üĴ�����ַ
 * �������  : puc_val: ��ȡ��ֵ
 * �� �� ֵ  : Linux������
 */
OAL_STATIC OAL_INLINE int32_t oal_pci_read_config_byte(oal_pci_dev_stru *pst_dev, int32_t ul_where,
                                                       uint8_t *puc_val)
{
    return pci_read_config_byte(pst_dev, ul_where, puc_val);
}

/*
 * �� �� ��  : oal_pci_read_config_word
 * ��������  : �����ÿռ��ȡһ����
 * �������  : pst_dev : ָ��PCI�豸
 *             ul_where: ���üĴ�����ַ
 * �������  : pus_val : ��ȡ��ֵ
 * �� �� ֵ  : Linux������
 */
OAL_STATIC OAL_INLINE int32_t oal_pci_read_config_word(oal_pci_dev_stru *pst_dev, int32_t ul_where,
                                                       uint16_t *pus_val)
{
    return pci_read_config_word(pst_dev, ul_where, pus_val);
}

/*
 * �� �� ��  : oal_pci_read_config_dword
 * ��������  : �����ÿռ��ȡ˫��
 * �������  : pst_dev : ָ��PCI�豸
 *             ul_where: ���üĴ�����ַ
 * �������  : pul_val : ��ȡ��ֵ
 * �� �� ֵ  : Linux������
 */
OAL_STATIC OAL_INLINE int32_t oal_pci_read_config_dword(oal_pci_dev_stru *pst_dev, int32_t ul_where,
                                                        uint32_t *pul_val)
{
    return pci_read_config_dword(pst_dev, ul_where, pul_val);
}

/*
 * �� �� ��  : oal_pci_write_config_byte
 * ��������  : �����ÿռ�дһ���ֽ�
 * �������  : pst_dev : ָ��PCI�豸
 *             ul_where: ���üĴ�����ַ
 *             uc_val  : Ҫд��ֵ
 * �� �� ֵ  : Linux������
 */
OAL_STATIC OAL_INLINE int32_t oal_pci_write_config_byte(oal_pci_dev_stru *pst_dev, int32_t ul_where,
                                                        uint8_t uc_val)
{
    return pci_write_config_byte(pst_dev, ul_where, uc_val);
}

/*
 * �� �� ��  : oal_pci_write_config_word
 * ��������  : �����ÿռ��дһ����
 * �������  : pst_dev : ָ��PCI�豸
 *             ul_where: ���üĴ�����ַ
 *             us_val  : Ҫд��ֵ
 * �� �� ֵ  : Linux������
 */
OAL_STATIC OAL_INLINE int32_t oal_pci_write_config_word(oal_pci_dev_stru *pst_dev, int32_t ul_where,
                                                        uint16_t us_val)
{
    return pci_write_config_word(pst_dev, ul_where, us_val);
}

/*
 * �� �� ��  : oal_pci_write_config_dword
 * ��������  : �����ÿռ�дһ��˫��
 *             ul_where: ���üĴ�����ַ
 *             ul_val  : Ҫд��ֵ
 * �� �� ֵ  : Linux������
 */
OAL_STATIC OAL_INLINE int32_t oal_pci_write_config_dword(oal_pci_dev_stru *pst_dev, int32_t ul_where,
                                                         uint32_t ul_val)
{
    return pci_write_config_dword(pst_dev, ul_where, ul_val);
}

/*
 * �� �� ��  : oal_pci_resource_start
 * ��������  : ��ȡPCI I/O�ռ��׵�ַ
 * �������  : pst_dev: PCI�豸
 *             l_bar  : PCI�豸����ַ�Ĵ�����ţ�ȡֵ0~5
 */
OAL_STATIC OAL_INLINE oal_uint oal_pci_resource_start(oal_pci_dev_stru *pst_dev, int32_t l_bar)
{
    return pci_resource_start(pst_dev, l_bar);
}

/*
 * �� �� ��  : oal_pci_resource_end
 * ��������  : ��ȡPCI I/O�ռ����һ�����õ�ַ
 * �������  : pst_dev: PCI�豸
 *             l_bar  : PCI�豸����ַ�Ĵ�����ţ�ȡֵ0~5
 */
OAL_STATIC OAL_INLINE oal_uint oal_pci_resource_end(oal_pci_dev_stru *pst_dev, int32_t l_bar)
{
    return pci_resource_end(pst_dev, l_bar);
}

/*
 * �� �� ��  : oal_pci_resource_len
 * ��������  : PCI I/O�ռ��С
 * �������  : pst_dev: PCI�豸
 *             l_bar  : PCI�豸����ַ�Ĵ�����ţ�ȡֵ0~5
 */
OAL_STATIC OAL_INLINE uint32_t oal_pci_resource_len(oal_pci_dev_stru *pst_dev, int32_t l_bar)
{
    return pci_resource_len(pst_dev, l_bar);
}

OAL_STATIC OAL_INLINE uint64_t oal_pci_bus_address(oal_pci_dev_stru *pst_dev, int32_t l_bar)
{
#ifdef CONFIG_PCI
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 14, 0))
    return (uint64_t)pci_bus_address(pst_dev, l_bar);
#else
    struct pci_bus_region region;

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 13, 0))
    pcibios_resource_to_bus(pst_dev, &region, &pst_dev->resource[l_bar]);
#else
    pcibios_resource_to_bus(pst_dev->bus, &region, &pst_dev->resource[l_bar]);
#endif

    return (uint64_t)region.start;
#endif
#else
    return 0;
#endif
}

OAL_STATIC OAL_INLINE unsigned long oal_pci_resource_flags(oal_pci_dev_stru *pst_dev, int32_t l_bar)
{
    return pci_resource_flags(pst_dev, l_bar);
}

/*
 * �� �� ��  : oal_pci_save_state
 * ��������  : PCI���ÿռ����ݱ���
 * �������  : pst_dev: PCI�豸
 */
OAL_STATIC OAL_INLINE int32_t oal_pci_save_state(oal_pci_dev_stru *pst_dev)
{
    return pci_save_state(pst_dev);
}

/*
 * �� �� ��  : oal_pci_restore_state
 * ��������  : PCI���ÿռ����ݻָ�
 * �������  : pst_dev: PCI�豸
 */
OAL_STATIC OAL_INLINE int32_t oal_pci_restore_state(oal_pci_dev_stru *pst_dev)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 10, 44))
    pci_restore_state(pst_dev);
    return 0;
#else
    return pci_restore_state(pst_dev);
#endif
}

/*
 * �� �� ��  : oal_pci_set_drvdata
 * ��������  : ����PCI�豸˽������
 * �������  : pst_dev: ָ���豸
 *             p_data : ����
 */
OAL_STATIC OAL_INLINE void oal_pci_set_drvdata(oal_pci_dev_stru *pst_dev, void *p_data)
{
    pci_set_drvdata(pst_dev, p_data);
}

/*
 * �� �� ��  : oal_pci_get_drvdata
 * ��������  : ��ȡPCI�豸˽������
 */
OAL_STATIC OAL_INLINE void *oal_pci_get_drvdata(oal_pci_dev_stru *pst_dev)
{
    return pci_get_drvdata(pst_dev);
}

/*
 * �� �� ��  : oal_pci_find_capability
 * ��������  : ����cap_id��ȡpci���ÿռ��ָ��capabilityλ��
 * �������  : cap id
 * �������  : ���ÿռ��ƫ��λ��
 */
OAL_STATIC OAL_INLINE int32_t oal_pci_find_capability(oal_pci_dev_stru *pst_dev, int32_t cap)
{
    return pci_find_capability(pst_dev, cap);
}

/*
 * pci_find_ext_capability - Find an extended capability
 * @dev: PCI device to query
 * @cap: capability code
 * Returns the address of the requested extended capability structure
 * within the device's PCI configuration space or 0 if the device does
 * not support it.  Possible values for @cap:
 *  %PCI_EXT_CAP_ID_ERR        Advanced Error Reporting
 *  %PCI_EXT_CAP_ID_VC        Virtual Channel
 *  %PCI_EXT_CAP_ID_DSN        Device Serial Number
 *  %PCI_EXT_CAP_ID_PWR        Power Budgeting
 * %PCI_EXT_CAP_ID_L1SS    L1 PM Sub state
 */
OAL_STATIC OAL_INLINE int32_t oal_pci_find_ext_capability(oal_pci_dev_stru *pst_dev, int32_t cap)
{
    return pci_find_ext_capability(pst_dev, cap);
}

OAL_STATIC OAL_INLINE int32_t oal_pci_pcie_cap(oal_pci_dev_stru *pst_dev)
{
    int pos = pci_find_capability(pst_dev, PCI_CAP_ID_EXP);
    return pos;
}

#endif /* end of oal_pci_if.h */
