

#ifndef __PLAT_FIRMWARE_H__
#define __PLAT_FIRMWARE_H__

/* ͷ�ļ����� */
#include "plat_type.h"
#include "oal_net.h"
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include "plat_exception_rst.h"
#endif

/* string */
#define os_mem_cmp(dst, src, size)  memcmp(dst, src, size)
#define os_str_cmp(dst, src)        strcmp(dst, src)
#define os_str_len(s)               strlen(s)
#define os_str_chr(str, chr)        strchr(str, chr)
#define os_str_str(dst, src)        strstr(dst, src)

/* memory */
#define os_mem_kfree(p)      kfree(p)
#define os_kmalloc_gfp(size) kmalloc(size, (GFP_KERNEL | __GFP_NOWARN))
#define os_kzalloc_gfp(size) kzalloc(size, (GFP_KERNEL | __GFP_NOWARN))
#define os_vmalloc_gfp(size) vmalloc(size)
#define os_mem_vfree(p)      vfree(p)

#define READ_MEG_TIMEOUT      2000  /* 200ms */
#define READ_MEG_JUMP_TIMEOUT 15000 /* 15s */

#define FILE_CMD_WAIT_TIME_MIN 5000 /* 5000us */
#define FILE_CMD_WAIT_TIME_MAX 5100 /* 5100us */

#define SEND_BUF_LEN 520
#define RECV_BUF_LEN 512
#define VERSION_LEN  64

#define READ_CFG_BUF_LEN 2048

#define DOWNLOAD_CMD_LEN      32
#define DOWNLOAD_CMD_PARA_LEN 800

#define HOST_DEV_TIMEOUT 3
#define INT32_STR_LEN    32

#define SHUTDOWN_TX_CMD_LEN 64

#define CMD_JUMP_EXEC_RESULT_SUCC 0
#define CMD_JUMP_EXEC_RESULT_FAIL 1

#define WIFI_MODE_DISABLE 0
#define WIFI_MODE_2G      1
#define WIFI_MODE_5G      2
#define WIFI_MODE_2G_5G   3

#define SOFT_WCPU_EN_ADDR  "0x50000c00"
#define SOFT_BCPU_EN_ADDR  "0x50000c04"
#define BCPU_DE_RESET_ADDR "0x50000094"

/* 1103,38.4M ��ʱ��������� */
#define STR_REG_PMU_CLK_REQ "0x50000350"

/* ����device������cfg�ļ��еĵ�һ���ַ�  ��: 2,0x50002a00,0x0 */
#define HOST_TO_DEVICE_CMD_HEAD 2

/* �����Ƿ���device����Ĺؼ��� */
#define VER_CMD_KEYWORD          "VERSION"
#define BUCK_MODE_CMD_KEYWORD    "BUCK_MODE"
#define JUMP_CMD_KEYWORD         "JUMP"
#define FILES_CMD_KEYWORD        "FILES"
#define SETPM_CMD_KEYWORD        "SETPM"
#define SETBUCK_CMD_KEYWORD      "SETBUCK"
#define SETSYSLDO_CMD_KEYWORD    "SETSYSLDO"
#define SETNFCRETLDO_CMD_KEYWORD "SETNFCRETLDO"
#define SETPD_CMD_KEYWORD        "SETPD"
#define SETNFCCRG_CMD_KEYWORD    "SETNFCCRG"
#define SETABB_CMD_KEYWORD       "SETABB"
#define SETTCXODIV_CMD_KEYWORD   "SETTCXODIV"
#define RMEM_CMD_KEYWORD         "READM"
#define WMEM_CMD_KEYWORD         "WRITEM"
#define QUIT_CMD_KEYWORD         "QUIT"

/* ���������ֲ��ᷢ��device�����ڿ��Ƽ������̣����ǻ�������cfg�ļ��� */
#define SLEEP_CMD_KEYWORD          "SLEEP"
#define CALI_COUNT_CMD_KEYWORD     "CALI_COUNT"
#define CALI_BFGX_DATA_CMD_KEYWORD "CALI_BFGX_DATA"
#define CALI_DCXO_DATA_CMD_KEYWORD "CALI_DCXO_DATA"
#define SHUTDOWN_WIFI_CMD_KEYWORD  "SHUTDOWN_WIFI"
#define SHUTDOWN_BFGX_CMD_KEYWORD  "SHUTDOWN_BFGX"

/* ������device������ִ�гɹ����صĹؼ��֣�host�յ�һ�¹ؼ���������ִ�гɹ� */
#define MSG_FROM_DEV_WRITEM_OK "WRITEM OK"
#define MSG_FROM_DEV_READM_OK  ""
#define MSG_FROM_DEV_FILES_OK  "FILES OK"
#define MSG_FROM_DEV_READY_OK  "READY"
#define MSG_FROM_DEV_JUMP_OK   "JUMP OK"
#define MSG_FROM_DEV_SET_OK    "SET OK"
#define MSG_FROM_DEV_QUIT_OK   ""

/* ������cfg�ļ���������Ĳ���ͷ��һ���Ϸ���cfg�����ʽΪ:����ͷ+����ؼ���(QUIT�������) */
#define FILE_TYPE_CMD_KEY "ADDR_FILE_"
#define NUM_TYPE_CMD_KEY  "PARA_"

#define COMPART_KEYWORD ' '
#define CMD_LINE_SIGN   ';'

#define CFG_INFO_RESERVE_LEN 8

#define HI1103_ASIC_MPW2  0
#define HI1103_ASIC_PILOT 1
#define HI1105_FPGA       2
#define HI1105_ASIC       3
#define HI1106_FPGA       4
#define HI1106_ASIC       5

#define BFGX_AND_WIFI_CFG_HI1103_PILOT_PATH  "/vendor/firmware/hi1103/pilot/bfgx_and_wifi_cfg"
#define WIFI_CFG_HI1103_PILOT_PATH           "/vendor/firmware/hi1103/pilot/wifi_cfg"
#define BFGX_CFG_HI1103_PILOT_PATH           "/vendor/firmware/hi1103/pilot/bfgx_cfg"
#define RAM_CHECK_CFG_HI1103_PILOT_PATH      "/vendor/firmware/hi1103/pilot/ram_reg_test_cfg"
#define RAM_BCPU_CHECK_CFG_HI1103_PILOT_PATH "/vendor/firmware/hi1103/pilot/reg_bcpu_mem_test_cfg"

#define HI1103_PILOT_BOOTLOADER_VERSION "Hi1103V100R001C01B083 Dec 16 2017"

#define HI1105_PILOT_BOOTLOADER_VERSION "Hi1105V100R001C01B083 Dec 16 2019"

/* hi1105 fpga cfg file path */
#define BFGX_AND_WIFI_CFG_HI1105_FPGA_PATH  "/vendor/firmware/hi1105/fpga/bfgx_and_wifi_cfg"
#define WIFI_CFG_HI1105_FPGA_PATH           "/vendor/firmware/hi1105/fpga/wifi_cfg"
#define BFGX_CFG_HI1105_FPGA_PATH           "/vendor/firmware/hi1105/fpga/bfgx_cfg"
#define RAM_CHECK_CFG_HI1105_FPGA_PATH      "/vendor/firmware/hi1105/fpga/ram_reg_test_cfg"
#define RAM_BCPU_CHECK_CFG_HI1105_FPGA_PATH "/vendor/firmware/hi1105/fpga/reg_bcpu_mem_test_cfg"

/* hi1105 asic cfg file path */
#define BFGX_AND_WIFI_CFG_HI1105_ASIC_PATH  "/vendor/firmware/hi1105/pilot/bfgx_and_wifi_cfg"
#define WIFI_CFG_HI1105_ASIC_PATH           "/vendor/firmware/hi1105/pilot/wifi_cfg"
#define BFGX_CFG_HI1105_ASIC_PATH           "/vendor/firmware/hi1105/pilot/bfgx_cfg"
#define RAM_CHECK_CFG_HI1105_ASIC_PATH      "/vendor/firmware/hi1105/pilot/ram_reg_test_cfg"
#define RAM_BCPU_CHECK_CFG_HI1105_ASIC_PATH "/vendor/firmware/hi1105/pilot/reg_bcpu_mem_test_cfg"

/* hi1106 fpga cfg file path */
#define BFGX_AND_WIFI_CFG_HI1106_FPGA_PATH  "/vendor/firmware/hi1106/fpga/bfgx_and_wifi_cfg"
#define WIFI_CFG_HI1106_FPGA_PATH           "/vendor/firmware/hi1106/fpga/wifi_cfg"
#define BFGX_CFG_HI1106_FPGA_PATH           "/vendor/firmware/hi1106/fpga/bfgx_cfg"
#define RAM_CHECK_CFG_HI1106_FPGA_PATH      "/vendor/firmware/hi1106/fpga/ram_reg_test_cfg"
#define RAM_BCPU_CHECK_CFG_HI1106_FPGA_PATH "/vendor/firmware/hi1106/fpga/reg_bcpu_mem_test_cfg"

/* hi1106 asic cfg file path */
#define BFGX_AND_WIFI_CFG_HI1106_ASIC_PATH  "/vendor/firmware/hi1106/pilot/bfgx_and_wifi_cfg"
#define WIFI_CFG_HI1106_ASIC_PATH           "/vendor/firmware/hi1106/pilot/wifi_cfg"
#define BFGX_CFG_HI1106_ASIC_PATH           "/vendor/firmware/hi1106/pilot/bfgx_cfg"
#define RAM_CHECK_CFG_HI1106_ASIC_PATH      "/vendor/firmware/hi1106/pilot/ram_reg_test_cfg"
#define RAM_BCPU_CHECK_CFG_HI1106_ASIC_PATH "/vendor/firmware/hi1106/pilot/reg_bcpu_mem_test_cfg"

#define FILE_COUNT_PER_SEND          1
#define MIN_FIRMWARE_FILE_TX_BUF_LEN 4096
#define MAX_FIRMWARE_FILE_TX_BUF_LEN (512 * 1024)

/* dump the device cpu reg mem when panic,24B mem header + 24*4 reg info */
#define CPU_PANIC_MEMDUMP_SIZE (24 + 24 * 4)

/* ö�ٶ��� */
enum return_type {
    SUCC = 0,
    EFAIL,
};

enum FIRMWARE_CFG_CMD_ENUM {
    ERROR_TYPE_CMD = 0,     /* ��������� */
    FILE_TYPE_CMD,          /* �����ļ������� */
    NUM_TYPE_CMD,           /* �������ò��������� */
    QUIT_TYPE_CMD,          /* �˳����� */
    SHUTDOWN_WIFI_TYPE_CMD, /* SHUTDOWN WCPU���� */
    SHUTDOWN_BFGX_TYPE_CMD  /* SHUTDOWN BCPU���� */
};

/*
 * 1.�״ο���ʱ��ʹ��BFGN_AND_WIFI_CFG������״ο���У׼��host����У׼���ݣ���ɺ�device�µ�
 * 2.deivce�µ�״̬����wifi��ʹ��BFGN_AND_WIFI_CFG
 * 3.deivce�µ�״̬����bt��ʹ��BFGX_CFG
 * 4.bt�������ٿ�wifi��ʹ��WIFI_CFG
 * 5.wifi�������ٿ�bt��ͨ��sdio�⸴λBCPU
 */
enum FIRMWARE_CFG_FILE_ENUM {
    BFGX_AND_WIFI_CFG = 0, /* ����BFGIN��wifi fimware����������index��ִ�ж���У׼ */
    WIFI_CFG,              /* ֻ����wifi firmware����������������ִ�ж���У׼ */
    BFGX_CFG,              /* ֻ����bfgx firmware������������������ִ�ж���У׼ */
    RAM_REG_TEST_CFG,      /* ���߲���mem reg����ʹ�� */

    CFG_FILE_TOTAL
};

enum FIRMWARE_SUBSYS_ENUM {
    DEV_WCPU = 0,
    DEV_BCPU,
    DEV_CPU_BUTT,
};

/* ȫ�ֱ������� */
extern uint8 **g_cfg_path;
extern uint8 *g_hi1103_pilot_cfg_patch_in_vendor[CFG_FILE_TOTAL];
extern uint8 *g_hi1105_asic_cfg_patch_in_vendor[CFG_FILE_TOTAL];

/* STRUCT ���� */
typedef struct cmd_type_st {
    int32 cmd_type;
    uint8 cmd_name[DOWNLOAD_CMD_LEN];
    uint8 cmd_para[DOWNLOAD_CMD_PARA_LEN];
} CMD_TYPE_STRUCT;

typedef struct firmware_globals_st {
    int32 al_count[CFG_FILE_TOTAL];            /* �洢ÿ��cfg�ļ���������Ч��������� */
    CMD_TYPE_STRUCT *apst_cmd[CFG_FILE_TOTAL]; /* �洢ÿ��cfg�ļ�����Ч���� */
    uint8 auc_CfgVersion[VERSION_LEN];         /* �洢cfg�ļ������õİ汾����Ϣ */
    uint8 auc_DevVersion[VERSION_LEN];         /* �洢����ʱdevice���ϱ��İ汾����Ϣ */
} FIRMWARE_GLOBALS_STRUCT;

typedef struct firmware_number_type_st {
    uint8 *key;
    int32 (*cmd_exec)(uint8 *Key, uint8 *Value);
} FIRMWARE_NUMBER_STRUCT;

typedef struct file OS_KERNEL_FILE_STRU;
extern FIRMWARE_GLOBALS_STRUCT g_cfg_info;

/* �������� */
extern uint8 *delete_space(uint8 *string, int32 *len);
extern int32 number_type_cmd_send(uint8 *Key, const char *Value);
extern int32 read_msg(uint8 *data, int32 len);
extern uint32 get_hi110x_asic_type(void);
extern int32 firmware_read_cfg(const char *puc_CfgPatch, uint8 *puc_read_buffer);
extern int32 firmware_parse_cmd(uint8 *puc_cfg_buffer, uint8 *puc_cmd_name, uint32 cmd_name_len,
                                uint8 *puc_cmd_para, uint32 cmd_para_len);
extern int32 firmware_download(uint32 ul_index);
extern int32 firmware_cfg_init(void);
extern int32 firmware_get_cfg(uint8 *puc_CfgPatch, uint32 ul_index);
extern int32 firmware_cfg_clear(void);
extern int32 wifi_device_mem_dump(struct st_wifi_dump_mem_info *pst_mem_dump_info, uint32 count);
extern int32 read_device_reg16(uint32 address, uint16 *value);
extern int32 write_device_reg16(uint32 address, uint16 value);
extern int32 is_device_mem_test_succ(void);
extern int32 get_device_ram_test_result(int32 is_wcpu, uint32 *cost);
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
extern int32 get_device_test_mem(bool is_wifi);
#endif
#endif /* end of plat_firmware.h */
