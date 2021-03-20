/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: VENC Driver Common Structure
 * Create: 2019-12-31
 */
#ifndef __DRV_COMMON_H__
#define __DRV_COMMON_H__

#include <linux/platform_device.h>
#include "securec.h"
#include "hi_type.h"
#include "drv_venc_osal.h"
#include "drv_venc_ioctl.h"

#define MAX_OPEN_COUNT 4
#define ENCODE_TIME_MS 100
#define INTERRUPT_TIMEOUT_MS 300
#define ENCODE_DONE_TIMEOUT_MS 800
#define WAIT_CORE_IDLE_TIMEOUT_MS 1000
#define FPGA_WAIT_EVENT_TIME_MS 1000000

#define MAX_CHANNEL_NUM 16
#define MAX_RING_BUFFER_SIZE (MAX_CHANNEL_NUM * MAX_SUPPORT_CORE_NUM)
#define ALIGN_UP(val, align) (((val) + ((align)-1)) & ~((align)-1))

enum {
	VEDU_H265 = 0,
	VEDU_H264 = 1
};

typedef enum {
	VENC_CORE_0,
	VENC_CORE_1,
	MAX_SUPPORT_CORE_NUM,
} venc_core_id_t;

typedef enum {
	VENC_POWER_OFF,
	VENC_IDLE,
	VENC_BUSY,
	VENC_TIME_OUT,
} venc_ip_status_t;

enum {
	CMDLIST_DISABLE = 0,
	CMDLIST_DUMP_DATA,
	PRINT_ENC_COST_TIME,
};

#ifdef VENC_DPM_ENABLE
enum {
	DP_MONIT_MOUDLE,
	SMMU_PRE_MODULE,
	MAX_INNER_MODULE,
};
#endif

struct venc_fifo_buffer {
	spinlock_t *lock;
	vedu_osal_event_t event;
	DECLARE_KFIFO_PTR(fifo, struct encode_done_info);
};

/* VENC IP Context */
struct venc_context {
	spinlock_t lock;
	venc_ip_status_t status;
	struct venc_fifo_buffer *buffer;
	struct channel_info  channel;
	HI_U32  *reg_base;
	HI_U32  irq_num_normal;
	HI_U32  irq_num_protect;
	HI_U32  irq_num_safe;
	HI_BOOL first_cfg_flag;
	HI_BOOL is_block;
	HI_BOOL is_protected;
	venc_timer_t timer;

	HI_U64 start_time;
};

struct encode_ops {
	HI_S32 (*init)(HI_VOID);
	HI_VOID (*deinit)(HI_VOID);
	HI_S32 (*encode)(struct encode_info*, struct venc_fifo_buffer*);
	HI_VOID (*encode_done)(struct venc_context*);
	HI_VOID (*encode_timeout)(unsigned long);
};

typedef struct {
	dev_t dev;
	struct device *venc_device;
	struct cdev cdev;
	struct class *venc_class;
	struct semaphore hw_sem; /* hardware lock */
	struct venc_context ctx[MAX_SUPPORT_CORE_NUM];
	vedu_osal_event_t event;
	struct encode_ops ops;
	HI_U32 debug_flag;
} venc_entry_t;

struct platform_device *venc_get_device(HI_VOID);
HI_S32 venc_check_coreid(HI_S32 core_id);
irqreturn_t venc_drv_encode_done(HI_S32 irq, HI_VOID *dev_id);
HI_S32 venc_drv_get_encode_done_info(struct venc_fifo_buffer *buffer, struct encode_done_info *encode_done_info);

#endif // __DRV_COMMON_H__

