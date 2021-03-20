

#ifndef __BFGX_EXCEPTION_RST_H__
#define __BFGX_EXCEPTION_RST_H__

/* 其他头文件包含 */
#include "plat_exception_rst.h"
#include "hw_bfg_ps.h"
#include "oal_workqueue.h"

/* plat cfg cmd */
#define PLAT_CFG_IOC_MAGIC                   'z'
#define PLAT_DFR_CFG_CMD                     _IOW(PLAT_CFG_IOC_MAGIC, PLAT_DFR_CFG, int)
#define PLAT_BEATTIMER_TIMEOUT_RESET_CFG_CMD _IOW(PLAT_CFG_IOC_MAGIC, PLAT_BEATTIMER_TIMEOUT_RESET_CFG, int)
#define PLAT_BFGX_CALI_CMD                   _IOW(PLAT_CFG_IOC_MAGIC, PLAT_BFGX_CALI, int)

#define PLAT_EXCEPTION_ENABLE  1
#define PLAT_EXCEPTION_DISABLE 0

#define HI110X_DFR_WAIT_SSI_IDLE_MS 5000

#ifdef HISI_TOP_LOG_DIR
#define BFGX_DUMP_PATH HISI_TOP_LOG_DIR "/hi110x/memdump"
#define WIFI_DUMP_PATH HISI_TOP_LOG_DIR "/wifi/memdump"
#else
#define BFGX_DUMP_PATH "/data/memdump"
#define WIFI_DUMP_PATH "/data/memdump"
#endif

#define PLAT_BFGX_EXCP_CFG_IOC_MAGIC 'b'
#define DFR_HAL_GNSS_CFG_CMD         _IOW(PLAT_BFGX_EXCP_CFG_IOC_MAGIC, DFR_HAL_GNSS, int)
#define DFR_HAL_BT_CFG_CMD           _IOW(PLAT_BFGX_EXCP_CFG_IOC_MAGIC, DFR_HAL_BT, int)
#define DFR_HAL_FM_CFG_CMD           _IOW(PLAT_BFGX_EXCP_CFG_IOC_MAGIC, DFR_HAL_FM, int)
#define DFR_HAL_IR_CFG_CMD           _IOW(PLAT_BFGX_EXCP_CFG_IOC_MAGIC, DFR_HAL_IR, int)
#define PLAT_BFGX_DUMP_FILE_READ_CMD _IOW(PLAT_BFGX_EXCP_CFG_IOC_MAGIC, PLAT_BFGX_DUMP_FILE_READ, int)
#define PLAT_ME_DUMP_FILE_READ_CMD   _IOW(PLAT_BFGX_EXCP_CFG_IOC_MAGIC, PLAT_ME_DUMP_FILE_READ, int)

typedef enum PLAT_BFGX_EXCP_CMD_e {
    DFR_HAL_GNSS = 0,
    DFR_HAL_BT = 1,
    DFR_HAL_FM = 2,
    DFR_HAL_IR = 3,
    PLAT_BFGX_DUMP_FILE_READ = 4,
    PLAT_ME_DUMP_FILE_READ = 5,

    PLAT_BFGX_EXCP_CMD_BOTT,
} plat_bfgx_excp_cmd_m;

#define PLAT_WIFI_EXCP_CFG_IOC_MAGIC 'w'
#define PLAT_WIFI_DUMP_FILE_READ_CMD _IOW(PLAT_WIFI_EXCP_CFG_IOC_MAGIC, PLAT_WIFI_DUMP_FILE_READ, int)

typedef enum PLAT_WIFI_EXCP_CMD_e {
    DFR_HAL_WIFI = 0,
    PLAT_WIFI_DUMP_FILE_READ = 1,
} plat_wifi_excp_cmd_m;

typedef struct memdump_driver_s {
    struct sk_buff_head quenue;
    int32 is_open;
    int32 is_working;
    wait_queue_head_t dump_type_wait;
    struct sk_buff_head dump_type_queue;
} memdump_info_t;

extern memdump_info_t g_bcpu_memdump_cfg;
extern memdump_info_t g_gcpu_memdump_cfg;
extern memdump_info_t g_wcpu_memdump_cfg;

enum PLAT_CFG {
    PLAT_DFR_CFG = 0,
    PLAT_BEATTIMER_TIMEOUT_RESET_CFG = 1,
    PLAT_CFG_BUFF,

    PLAT_DUMP_FILE_READ = 100,
    PLAT_DUMP_ROTATE_FINISH = 101,
    PLAT_BFGX_CALI = 102,
};

enum BFGX_DUMP_TYPE {
    BFGX_PUB_REG = 0,
    BFGX_PRIV_REG = 1,
    BFGX_MEM = 2,

    SDIO_BFGX_MEM_DUMP_BOTTOM,
};

extern void plat_dfr_cfg_set(uint64 arg);
extern void plat_beat_timeout_reset_set(uint64 arg);
extern int32 bfgx_store_stack_mem_to_file(void);
extern int32 plat_get_excp_total_cnt(void);
extern int32 plat_get_dfr_sinfo(char *buf, int32 index);
extern int32 is_dfr_test_en(enum excp_test_cfg_em excp_cfg);
extern void set_excp_test_en(enum excp_test_cfg_em excp_cfg);
extern void bfgx_memdump_finish(struct ps_core_s *ps_core_d);
extern void wifi_memdump_finish(void);
extern int32 wifi_notice_hal_memdump(void);
extern int32 bfgx_notice_hal_memdump(struct ps_core_s *ps_core_d);
extern int32 bfgx_memdump_enquenue(struct ps_core_s *ps_core_d, uint8 *buf_ptr, uint16 count);
extern int32 wifi_memdump_enquenue(uint8 *buf_ptr, uint16 count);
extern oal_workqueue_stru *wifi_get_exception_workqueue(oal_void);
#endif
