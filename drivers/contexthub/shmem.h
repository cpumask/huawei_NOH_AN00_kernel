/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Contexthub share memory driver
 * Author: Huawei
 * Create: 2016-04-01
 */
#ifndef __HISI_CONTEXTHUB_SHMEM_H__
#define __HISI_CONTEXTHUB_SHMEM_H__

#ifdef CONFIG_INPUTHUB_30
#include <linux/hisi/contexthub/iomcu_shmem.h>
#else

#include "protocol.h"

#include <iomcu_ddr_map.h>

#define HISI_RESERVED_CH_BLOCK_SHMEM_PHYMEM_BASE       DDR_SHMEM_LARGE_BLK_BASE_AP
#define HISI_RESERVED_CH_BLOCK_SHMEM_PHYMEM_SIZE       DDR_SHMEM_LARGE_BLK_SIZE
#endif
/*
 * sharemem��Ϣ���ͽӿڣ������ݸ��Ƶ�DDR�У�������IPC֪ͨcontexthub����
 */
extern int shmem_send(obj_tag_t module_id, const void *usr_buf, unsigned int usr_buf_size);

/*
 * Contexthub sharemem������ʼ��
 */
extern int contexthub_shmem_init(void);

/*
 * ��sharemem���е����ݴ�DDR��ȡ��������Ǵ����ݰ���Ϣͬʱ����workqueue�ظ�IPCȷ��
 */
extern const pkt_header_t *shmempack(const char *buf, unsigned int length);

/*
 * ���sharemem���ݷ��͵�size����
 */
extern unsigned int shmem_get_capacity(void);

/*
 * sharemem��Ϣ���ͺ��յ�contexthub�Ļظ�ȷ�Ϻ�up�ź���
 */
extern int shmem_send_resp(const pkt_header_t *head);

#endif
