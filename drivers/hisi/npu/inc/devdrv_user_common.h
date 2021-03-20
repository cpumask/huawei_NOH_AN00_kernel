/*
 * devdrv_user_common.h
 *
 * about devdrv user common
 *
 * Copyright (c) 2012-2019 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */
#ifndef __DEVDRV_USER_COMMON_H
#define __DEVDRV_USER_COMMON_H

#include "devdrv_user_common_adapter.h"
#include "devdrv_user_common_model.h"

#define UINT64  uint64_t
#define UINT32  uint32_t
#define UINT16  uint16_t
#define UINT8   uint8_t

#define DEVDRV_SHM_MAPS_SIZE   (4 * 1024 * 1024)

#define DEVDRV_MAX_CQ_SLOT_SIZE 128

/* both sizeof(devdrv_rt_hwts_task_t) sizeof(devdrv_ts_comm_sqe_t) size is 64B */
#define DEVDRV_RT_TASK_SIZE			64
#define DEVDRV_MAX_STREAM_PRIORITY	7
#define UINT16_MAX					65535
#define NPU_BIT_MASK(width)			(((uint64_t)1 << width) - 1)

#define DEVDRV_CACHELINE_OFFSET	6
#define DEVDRV_CACHELINE_SIZE   64
#define DEVDRV_CACHELINE_MASK   (DEVDRV_CACHELINE_SIZE - 1)

#define DEVDRV_MAX_DAVINCI_NUM	1

#define DEVDRV_FUNCTIONAL_SQ_FIRST_INDEX  112
#define DEVDRV_FUNCTIONAL_CQ_FIRST_INDEX  116

#define DEVDRV_MAX_FUNCTIONAL_SQ_NUM    4
#define DEVDRV_MAX_FUNCTIONAL_CQ_NUM    10

#define DEVDRV_MAILBOX_MAX_FEEDBACK  16
#define DEVDRV_MAILBOX_STOP_THREAD   0x0FFFFFFF
#define DEVDRV_BUS_DOWN     0x0F00FFFF

#define DEVDRV_HCCL_NAME_SIZE       64
#define DEVDRV_HCCL_MAX_NODE_NUM    128

#define DEVDRV_MAX_MEMORY_DUMP_SIZE (4 * 1024 * 1024)

#define DEVDRV_BB_DEVICE_ID_INFORM  0x66020004
#define CHIP_BASEADDR_PA_OFFSET 0x200000000000ULL

#define DEVDRV_VALID_ION            1
#define DEVDRV_INVALID_ION          0

enum devdrv_ts_status {
	DEVDRV_TS_WORK = 0x0,
	DEVDRV_TS_SLEEP,
	DEVDRV_TS_DOWN,
	DEVDRV_TS_INITING,
	DEVDRV_TS_BOOTING,
	DEVDRV_TS_FAIL_TO_SUSPEND,
	DEVDRV_TS_SEC_WORK, // secure power up
	DEVDRV_TS_MAX_STATUS
};

typedef struct devdrv_ts_sq_info {
	u32 head;
	u32 tail;
	u32 credit;
	u32 index;
	int pid;
	u32 stream_num;
	u8 *vir_addr;
	u64 send_count;
	u64 resv;
} devdrv_ts_sq_info_t;

struct devdrv_ts_cq_info {
	u32 head;
	u32 tail;
	volatile u32 count_report;
	u32 index;
	u32 phase;
	u32 int_flag;
	int pid;
	u32 stream_num;
	u8 *vir_addr;
	u64 receive_count;
	u64 resv;
	u8 slot_size;
	u8 rsv[3];
	u32 communication_stream_num; // only use in v200
};

typedef struct devdrv_stream_info {
	int id;
	u32 devid;
	// used for non-sink stream or sink stream
	u32 cq_index;
	u32 sync_cq_index;
	u32 sq_index;
	u32 resv0;
	u64 resv1;
	int pid;
	u32 strategy;
	u32 create_tid;
	// used for sink stream
	u16 smmu_substream_id;
	u8 priority;
	u8 resv[1];
} devdrv_stream_info_t;

#define DEVDRV_MESSAGE_PAYLOAD 64
struct devdrv_mailbox_user_message {
	u8 message_payload[DEVDRV_MESSAGE_PAYLOAD];
	int message_length;
	int feedback_num;
	u8 *feedback_buffer;	/*
				 * if a sync message need feedback, must alloc buffer for feedback data.
				 * if a async message need feedback, set this to null,
				 * because driver will send a callback parameter to callback func,
				 * app has no need to free callback parameter in callback func.
				 */
	int sync_type;
	int cmd_type;
	int message_index;
	int message_pid;
};

struct devdrv_mailbox_feedback {
	void (*callback)(void *data);
	u8 *buffer;
	int feedback_num;
	int process_result;
};

struct devdrv_svm_to_devid {
	u32 src_devid;
	u32 dest_devid;
	unsigned long src_addr;
	unsigned long dest_addr;
};

struct devdrv_channel_info_devid {
	char name[DEVDRV_HCCL_NAME_SIZE];
	u32 handle;
	u32 event_id;
	u32 src_devid;
	u32 dest_devid;
	void *dest_doorbell_pa;
	void *ack_doorbell_pa;
	void *dest_mailbox_pa;
	/* for ipc event query */
	u32 status;
	u64 timestamp;
};

struct devdrv_trans_info {
	u32 src_devid;
	u32 dest_devid;
	u8 ways;
};

struct devdrv_hardware_inuse {
	u32 devid;
	u32 ai_core_num;
	u32 ai_core_error_bitmap;
	u32 ai_cpu_num;
	u32 ai_cpu_error_bitmap;
};

struct devdrv_manager_hccl_devinfo {
	u8 env_type;
	u32 dev_id;
	u32 ctrl_cpu_ip;
	u32 ctrl_cpu_id;
	u32 ctrl_cpu_core_num;
	u32 ctrl_cpu_endian_little;
	u32 ts_cpu_core_num;
	u32 ai_cpu_core_num;
	u32 ai_core_num;
	u32 ai_cpu_bitmap;
	u32 ai_core_id;
	u32 ai_cpu_core_id;
	u32 hardware_version;  /* mini, cloud, lite, etc. */
	u32 num_dev;
	u32 devids[DEVDRV_MAX_DAVINCI_NUM];
};

enum devdrv_arch_type {
	ARCH_BEGIN = 0,
	ARCH_V100 = ARCH_BEGIN,
	ARCH_V200,
	ARCH_END
};

enum devdrv_chip_type {
	CHIP_BEGIN = 0,
	CHIP_LITE_PHOENIX = CHIP_BEGIN,
	CHIP_LITE_ORLANDO,
	CHIP_LITE_DENVER,
	CHIP_LITE_BALTIMORE = 5,
	CHIP_END,
};

enum devdrv_version {
	VER_BEGIN = 0,
	VER_NA = VER_BEGIN,
	VER_ES,
	VER_CS,
	VER_CS2,
	VER_END
};

#define PLAT_COMBINE(arch, chip, ver) (((arch) << 16) | ((chip) << 8) | (ver))
#define PLAT_GET_ARCH(type) (((type) >> 16) & 0xffff)
#define PLAT_GET_CHIP(type) (((type) >> 8) & 0xff)
#define PLAT_GET_VER(type)  ((type) & 0xff)

enum devdrv_hardware_version {
	DEVDRV_PLATFORM_LITE_PHOENIX_CS = PLAT_COMBINE(ARCH_V100, CHIP_LITE_PHOENIX, VER_CS),
	DEVDRV_PLATFORM_LITE_PHOENIX_CS2 = PLAT_COMBINE(ARCH_V100, CHIP_LITE_PHOENIX, VER_CS2),
	DEVDRV_PLATFORM_LITE_ORLANDO = PLAT_COMBINE(ARCH_V100, CHIP_LITE_ORLANDO, VER_NA),
	DEVDRV_PLATFORM_LITE_DENVER = PLAT_COMBINE(ARCH_V100, CHIP_LITE_DENVER, VER_NA),
	DEVDRV_PLATFORM_LITE_BALTIMORE_ES = PLAT_COMBINE(ARCH_V200, CHIP_LITE_BALTIMORE, VER_ES),
	DEVDRV_PLATFORM_LITE_BALTIMORE_CS = PLAT_COMBINE(ARCH_V200, CHIP_LITE_BALTIMORE, VER_CS),
	DEVDRV_PLATFORM_END
};

struct devdrv_occupy_stream_id {
	u32 sqcq_strategy;
	u16 count;
	u16 id[DEVDRV_MAX_STREAM_ID];
};

struct devdrv_module_status {
	u8 lpm3_start_fail;
	u8 lpm3_lost_heart_beat;
	u8 ts_start_fail;
	u8 ts_lost_heart_beat;
	u8 ts_sram_broken;
	u8 ts_sdma_broken;
	u8 ts_bs_broken;
	u8 ts_l2_buf0_broken;
	u8 ts_l2_buf1_broken;
	u8 ts_spcie_broken;
};

/* The info memory of reserved memory order: 1.SQ_INFO, 2.CQ_INFO, 3.STREAM_INFO */
/* 4.HWTS_SQ_INFO, 5.HWTS_CQ_INFO, 6.MODEL_INFO 7.PROFILINGL_INFO 8.TS_STAUS_INFO */

/* reserved memory info size */
#define DEVDRV_SQ_INFO_SIZE   sizeof(struct devdrv_ts_sq_info)
#define DEVDRV_CQ_INFO_SIZE   sizeof(struct devdrv_ts_cq_info)
#define DEVDRV_STREAM_INFO_SIZE   sizeof(struct devdrv_stream_info)

#define DEVDRV_SQ_INFO_OCCUPY_SIZE \
	(DEVDRV_SQ_INFO_SIZE * DEVDRV_MAX_SQ_NUM)
#define DEVDRV_CQ_INFO_OCCUPY_SIZE \
	(DEVDRV_CQ_INFO_SIZE * DEVDRV_MAX_CQ_NUM)
#define DEVDRV_STREAM_INFO_OCCUPY_SIZE \
	(DEVDRV_STREAM_INFO_SIZE * DEVDRV_MAX_STREAM_ID)

#define DEVDRV_MAX_INFO_SIZE \
	(DEVDRV_INFO_OCCUPY_SIZE + sizeof(u32))
#define DEVDRV_MAX_INFO_ORDER   (get_order(DEVDRV_MAX_INFO_SIZE))


/* add necessary dfx function if you need */
enum devdrv_dfx_cmd {
	DEVDRV_DFX_QUERY_STATUS,
	DEVDRV_DFX_MAX_CMD
};


/*
|___SQ___|____INFO_____|__DOORBELL___|___CQ____|
*/
#define DEVDRV_VM_BLOCK_OFFSET  (32 * 1024 * 1024)
#define DEVDRV_VM_CQ_QUEUE_SIZE  (64 * 1024 * 1024)
#define DEVDRV_VM_CQ_SLOT_SIZE  (128 * 1024)

#define DEVDRV_VM_MEM_START  0xFFC0000000ULL
#define DEVDRV_VM_MEM_SIZE  (DEVDRV_VM_BLOCK_OFFSET * 3 + DEVDRV_VM_CQ_QUEUE_SIZE)

/* *custom ioctrl* */
typedef enum {
	DEVDRV_IOC_VA_TO_PA,
	DEVDRV_IOC_GET_SVM_SSID,
	DEVDRV_IOC_GET_CHIP_INFO,
	DEVDRV_IOC_ALLOC_CONTIGUOUS_MEM,
	DEVDRV_IOC_FREE_CONTIGUOUS_MEM,
	DEVDRV_IOC_REBOOT,
	DEVDRV_IOC_LOAD_MODEL_BUFF,
	DEVDRV_IOC_RESERVED
} devdrv_custom_ioc_t;

typedef struct {
	u32 version;
	u32 cmd;
	u32 result;
	u32 reserved;
	u64 arg;
} devdrv_custom_para_t;

typedef struct tag_stream_buffer_info {
	uint32_t stream_id;
	uint32_t mod_id;
	uint64_t len;
	uint64_t *addr;
	uint8_t  priority;
	uint8_t  reserve[7];
} devdrv_stream_buff_info_t; // total 32 Bytes

/* just for ai core bypass mode */
#define AI_PAGE_SIZE  4096
#define AI_PAGE_MASK  (~(AI_PAGE_SIZE - 1))

struct davinci_area_info {
	unsigned long va;
	unsigned long pa;
	unsigned long len;
};

struct process_info {
	pid_t vpid;
	UINT64 ttbr;
	UINT64 tcr;
	int pasid;
	UINT32 flags;
};

struct devdrv_chip_info {
	UINT32 l2_size;
	UINT32 reserved[3];
};

#define STREAM_STRATEGY_NONSINK    0
#define STREAM_STRATEGY_SINK       (0x1 << 0)
#define STREAM_STRATEGY_LONG       (0x1 << 1)
#define STREAM_STRATEGY_MAX        (0x1 << 2)

enum {
	SQCQ_PROCESSED_BY_USER_DRIVER = 0,
	SQCQ_PROCESSED_BY_KERNEL = 1
};

enum {
	EVENT_STRATEGY_NONSINK = 0,
	EVENT_STRATEGY_SINK = 1,
	EVENT_STRATEGY_TS = 2,
	EVENT_STRATEGY_HWTS = 3,
	EVENT_STRATEGY_MAX
};

typedef struct devdrv_stream_alloc_ioctl_info {
	u16 strategy;
	u16 priority;
	int stream_id;
	u32 devid;
	u32 sqcq_strategy;
} devdrv_stream_alloc_ioctl_info_t;

struct devdrv_stream_free_ioctl_info {
	int stream_id;
	u32 sqcq_strategy;
};

typedef struct npu_event_alloc_ioctl_info {
	u16 strategy;
	int event_id;
	u32 devid;
} npu_event_alloc_ioctl_info_t;

typedef struct npu_event_free_ioctl_info {
	u16 strategy;
	int event_id;
} npu_event_free_ioctl_info_t;

// for custom ioctl power up&down
typedef struct npu_work_mode_info {
	uint32_t work_mode;
	uint32_t flags;
} npu_work_mode_info_t;

struct devdrv_power_down_info {
	npu_work_mode_info_t secure_info;
	u32 sqcq_strategy;
};

struct devdrv_limit_time_info {
	u32 type;
	u32 time_out;
};

struct devdrv_report_sync_wait_info {
	u64 report_addr;
	u16 stream_id;
	u16 task_id;
	u32 timeout;
	s32 result;
};

typedef struct {
	int ionfd;
	int result;    /* 0 -- DEVDRV_INVALID_ION; 1 -- DEVDRV_VALID_ION */
} npu_check_ion_t;

struct devdrv_iova_ioctl_info {
	int fd;
	int prot;
	u64 vaddr;
};

#endif
