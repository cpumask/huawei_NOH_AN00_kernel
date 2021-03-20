

#ifndef __OAL_LINUX_MEM_H__
#define __OAL_LINUX_MEM_H__

/* �궨�� */
#define OAL_TX_CB_LEN    48
#define OAL_MAX_CB_LEN   19
#define MAX_MAC_HEAD_LEN WLAN_MAX_MAC_HDR_LEN

/* ����enhanced���͵�����ӿ����ͷŽӿڣ�ÿһ���ڴ�鶼����һ��4�ֽڵ�ͷ���� */
/* ����ָ���ڴ�����ṹ��oal_mem_struc�������ڴ��Ľṹ������ʾ��           */
/*                                                                           */
/* +-------------------+------------------------------------------+---------+ */
/* | oal_mem_stru addr |                    payload               | dog tag | */
/* +-------------------+------------------------------------------+---------+ */
/* |      4/8 byte       |                                          | 4 byte  | */
/* +-------------------+------------------------------------------+---------+ */
#ifdef CONFIG_ARM64
/* when 64 os, c pointer take 8 bytes */
#define OAL_MEM_INFO_SIZE 8
#else
#define OAL_MEM_INFO_SIZE 4
#endif

#define OAL_IORESOURCE_IO  IORESOURCE_IO
#define OAL_IORESOURCE_MEM IORESOURCE_MEM
#ifdef IORESOURCE_REG
#define OAL_IORESOURCE_REG IORESOURCE_REG
#else
#define OAL_IORESOURCE_REG 0x00000300
#endif

#endif /* end of oal_mm.h */
