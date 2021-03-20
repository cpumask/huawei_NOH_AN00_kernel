/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Description: Contexthub share memory driver
 * Author: Huawei
 * Create: 2016-04-01
 */
#ifndef __IOMCU_BOOT_H__
#define __IOMCU_BOOT_H__
#include <linux/types.h>
#include  <iomcu_ddr_map.h>

#define STARTUP_IOM3_CMD 0x00070001
#define RELOAD_IOM3_CMD 0x0007030D

#define IOMCU_CONFIG_SIZE (DDR_CONFIG_SIZE * 3 / 8)
#define IOMCU_CONFIG_START DDR_CONFIG_ADDR_AP

#define IOMCU_SMPLAT_SCFG_START (IOMCU_CONFIG_START + DDR_CONFIG_SIZE * 3 / 8)
#define IOMCU_SMPLAT_SCFG_SIZE (DDR_CONFIG_SIZE * 5 / 8)

#define IOMCU_SMPLAT_DDR_MAGIC 0xabcd1243

#define PA_MARGIN_NUM           2 /* PA_HCI:0 PA_NBTI:1 */
#define NPU_AVS_DATA_NUN        14
#define NPU_SVFD_DATA_NUN       28
#define NPU_SVFD_PARA_NUN       2

typedef enum DUMP_LOC {
	DL_NONE = 0,
	DL_TCM,
	DL_EXT,
	DL_BOTTOM = DL_EXT,
} dump_loc_t;
enum DUMP_REGION {
	DE_TCM_CODE,
	DE_DDR_CODE,
	DE_DDR_DATA,
	DE_BOTTOM = 16,
};

typedef struct {
	u8 loc;
	u8 reserved[3];
} dump_region_config_t;

typedef struct {
	u64 dump_addr;
	u32 dump_size;
	u32 reserved1;
	u64 ext_dump_addr;
	u32 ext_dump_size;
	u8 enable;
	u8 finish;
	u8 reason;
	u8 reserved2;
	dump_region_config_t elements[DE_BOTTOM];
} dump_config_t;

typedef struct {
	u64 syscnt;
	u64 kernel_ns;
} timestamp_kernel_t;

typedef struct {
	u64 syscnt;
	u64 kernel_ns;
	u32 timer32k_cyc;
	u32 reserved;
} timestamp_iomcu_base_t;

typedef struct {
	u64 log_addr;
	u32 log_size;
	u32 rsvd;
} bbox_config_t;

struct npu_cfg_data {
	u32 pa_margin[PA_MARGIN_NUM];
	u32 avs_data[NPU_AVS_DATA_NUN];
	u32 svfd_data[NPU_SVFD_DATA_NUN];
	u32 svfd_para[NPU_SVFD_PARA_NUN];
};

struct plat_cfg_str {
	u64 magic;
	timestamp_kernel_t timestamp_base;
	timestamp_iomcu_base_t timestamp_base_iomcu;
	bbox_config_t bbox_conifg;
	dump_config_t dump_config;
	struct npu_cfg_data npu_data;
};

extern uint32_t need_reset_io_power;
extern uint32_t need_set_3v_io_power;
extern uint32_t need_set_3_1v_io_power;
extern uint32_t need_set_3_2v_io_power;
void write_timestamp_base_to_sharemem(void);
void inputhub_init_before_boot(void);
void inputhub_init_after_boot(void);
extern struct plat_cfg_str *g_smplat_scfg;

#endif /* __LINUX_INPUTHUB_CMU_H__ */
