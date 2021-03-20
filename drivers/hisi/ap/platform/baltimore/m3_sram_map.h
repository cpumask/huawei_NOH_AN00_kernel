#ifndef __M3_SRAM_MAP_H__
#define __M3_SRAM_MAP_H__ 
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
#include <soc_lpmcu_baseaddr_interface.h>
#include <soc_acpu_baseaddr_interface.h>
#include <m3_ddr_map.h>
#define __PRINT_MACRO(x) #x
#define PRINT_MACRO(x) #x"="__PRINT_MACRO(x)
#ifndef SOC_LPMCU_LP_RAM_BASE_ADDR
#define SOC_LPMCU_LP_RAM_BASE_ADDR 0x20000
#endif
#ifndef SOC_ACPU_LP_RAM_BASE_ADDR
#define SOC_ACPU_LP_RAM_BASE_ADDR 0xFFF50000
#endif
#if defined(__FASTBOOT__) || defined(CONFIG_BL2_LOAD_LPMCU)
#define M3_SRAM_BASE (LPMCU_FIRMWARE_BACKUP_ADDR)
#else
#define M3_SRAM_BASE (SOC_LPMCU_LP_RAM_BASE_ADDR)
#endif
#define M3_SRAM_SIZE (LPMCU_RAM_SIZE)
#define M3_SRAM_AO_SIZE (0x30000)
#define LPMCU_VECTOR_TABLE_BASE (M3_SRAM_BASE)
#define LPMCU_VECTOR_TABLE_SIZE (0x80)
#define LPMCU_GEN_DATA_SEC_BASE (LPMCU_VECTOR_TABLE_BASE + LPMCU_VECTOR_TABLE_SIZE)
#define LPMCU_GEN_DATA_SEC_SIZE (0x2A0)
    #define RUNTIME_VAR_BASE (LPMCU_GEN_DATA_SEC_BASE)
    #define RUNTIME_VAR_SIZE (0x2A0)
#if ((RUNTIME_VAR_BASE + RUNTIME_VAR_SIZE) > (LPMCU_GEN_DATA_SEC_BASE + LPMCU_GEN_DATA_SEC_SIZE))
    #error "lpmcu general data section overflow!!!"
#endif
#define LPMCU_GEN_MNTN_BASE (LPMCU_GEN_DATA_SEC_BASE + LPMCU_GEN_DATA_SEC_SIZE)
#define LPMCU_GEN_MNTN_SIZE (0xC00)
 #define EXC_SPECIAL_SAVE_BASE (LPMCU_GEN_MNTN_BASE)
 #define EXC_SPECIAL_SAVE_SIZE (0x40)
 #define INT_TRACK_BASE (EXC_SPECIAL_SAVE_BASE + EXC_SPECIAL_SAVE_SIZE)
 #define INT_TRACK_SIZE (0x2C0)
 #define TASK_TRACK_BASE (INT_TRACK_BASE + INT_TRACK_SIZE)
 #define TASK_TRACK_SIZE (0x100)
 #define REG_BACKUP_SRAM_BASE (TASK_TRACK_BASE + TASK_TRACK_SIZE)
 #define REG_BACKUP_SRAM_SIZE (0x70)
 #define STACK_BACKUP_SRAM_BASE (REG_BACKUP_SRAM_BASE + REG_BACKUP_SRAM_SIZE)
 #define STACK_BACKUP_SRAM_SIZE (0x300)
 #define DMA_REG_BACKUP_BASE (STACK_BACKUP_SRAM_BASE + STACK_BACKUP_SRAM_SIZE)
 #define DMA_REG_BACKUP_SIZE (0x60)
 #define SVFD_VOLT_DROP_BASE (DMA_REG_BACKUP_BASE + DMA_REG_BACKUP_SIZE)
 #define SVFD_VOLT_DROP_SIZE (0x10)
 #define REG_PMU_BASE (SVFD_VOLT_DROP_BASE + SVFD_VOLT_DROP_SIZE)
 #define REG_PMU_SIZE (0x40)
 #define REG_BACKUP_BASE (REG_PMU_BASE + REG_PMU_SIZE)
 #define REG_BACKUP_SIZE (0x1A0)
 #define NOC_DUMP_BASE (REG_BACKUP_BASE + REG_BACKUP_SIZE)
 #define NOC_DUMP_SIZE (0x14)
 #define PLL_MNTN_BASE (NOC_DUMP_BASE + NOC_DUMP_SIZE)
 #define PLL_MNTN_SIZE (0x150)
#if ((PLL_MNTN_BASE + PLL_MNTN_SIZE) > (LPMCU_GEN_MNTN_BASE + LPMCU_GEN_MNTN_SIZE))
     #error "lpmcu general mntn section overflow!!!"
#endif
#define INIT_PARA_BASE (LPMCU_GEN_MNTN_BASE + LPMCU_GEN_MNTN_SIZE)
#define INIT_PARA_SIZE (0x170)
#define PMU_MDM_PARA_BASE (INIT_PARA_BASE + INIT_PARA_SIZE)
#define PMU_MDM_PARA_SIZE (0x60)
#define PERI_AVS_PARA_BASE (PMU_MDM_PARA_BASE + PMU_MDM_PARA_SIZE)
#define PERI_AVS_PARA_SIZE (0x200)
#define LPMCU_THERMAL_PARA_BASE (PERI_AVS_PARA_BASE + PERI_AVS_PARA_SIZE)
#define LPMCU_THERMAL_PARA_SIZE (0x110)
#define LPMCU_MODEM_SEC_BASE (LPMCU_THERMAL_PARA_BASE + LPMCU_THERMAL_PARA_SIZE)
#define LPMCU_MODEM_SEC_SIZE (0x2400)
    #define AON_STORAGE_SOCP_BASE (LPMCU_MODEM_SEC_BASE)
    #define AON_STORAGE_SOCP_SIZE (0x1390)
    #define BOARD_TYPE_BASE (AON_STORAGE_SOCP_BASE + AON_STORAGE_SOCP_SIZE)
    #define BOARD_TYPE_SIZE (0x10)
    #define MODEM_PARA_BASE (BOARD_TYPE_BASE + BOARD_TYPE_SIZE)
    #define MODEM_PARA_SIZE (0x60)
    #define MODEM_MNTN_BASE (MODEM_PARA_BASE + MODEM_PARA_SIZE)
    #define MODEM_MNTN_SIZE (0x1000)
#if ((MODEM_MNTN_BASE + MODEM_MNTN_SIZE) > (LPMCU_MODEM_SEC_BASE + LPMCU_MODEM_SEC_SIZE))
    #error "lpmcu modem section overflow!!!"
#endif
#define LPMCU_TELE_MNTN_SEC_BASE (LPMCU_MODEM_SEC_BASE + LPMCU_MODEM_SEC_SIZE)
#define LPMCU_TELE_MNTN_SEC_SIZE (0x1400)
    #define LPMCU_TELE_MNTN_DATA_BASE (LPMCU_TELE_MNTN_SEC_BASE)
    #define LPMCU_TELE_MNTN_DATA_SIZE (0x400)
  #define LPMCU_TELE_MNTN_DATA_TICKMARK_BASE (LPMCU_TELE_MNTN_DATA_BASE)
  #define LPMCU_TELE_MNTN_DATA_TICKMARK_SIZE (0x150)
    #define LPMCU_TELE_MNTN_BUF_BASE (LPMCU_TELE_MNTN_DATA_BASE + LPMCU_TELE_MNTN_DATA_SIZE)
    #define LPMCU_TELE_MNTN_BUF_SIZE (0x1000)
#if ((LPMCU_TELE_MNTN_BUF_BASE + LPMCU_TELE_MNTN_BUF_SIZE) > (LPMCU_TELE_MNTN_SEC_BASE + LPMCU_TELE_MNTN_SEC_SIZE))
    #error "lpmcu tele mntn section overflow!!!"
#endif
#define LPMCU_DDR_DATA_SEC_BASE (LPMCU_TELE_MNTN_SEC_BASE + LPMCU_TELE_MNTN_SEC_SIZE)
#define LPMCU_DDR_DATA_SEC_SIZE (0xC800)
 #define DDR_REG_BACKUP_BASE (LPMCU_DDR_DATA_SEC_BASE)
 #define DDR_REG_BACKUP_SIZE (0x700)
 #define DDR_SEC_LOCK_REG_BASE (DDR_REG_BACKUP_BASE + DDR_REG_BACKUP_SIZE)
 #define DDR_SEC_LOCK_REG_SIZE (0x100)
 #define DDR_TRAINNING_BASE (DDR_SEC_LOCK_REG_BASE + DDR_SEC_LOCK_REG_SIZE)
 #define DDR_TRAINNING_SIZE (0x8C00)
 #define DDR_PROFILE_BASE (DDR_TRAINNING_BASE + DDR_TRAINNING_SIZE)
 #define DDR_PROFILE_SIZE (0xF00)
 #define DMSS_PROFILE_BASE (DDR_PROFILE_BASE + DDR_PROFILE_SIZE)
 #define DMSS_PROFILE_SIZE (0x200)
 #define DDR_IO_PROFILE_BASE (DMSS_PROFILE_BASE + DMSS_PROFILE_SIZE)
 #define DDR_IO_PROFILE_SIZE (0x200)
 #define DDR_POLICY_BASE (DDR_IO_PROFILE_BASE + DDR_IO_PROFILE_SIZE)
 #define DDR_POLICY_SIZE (0x200)
 #define DDR_POLICY_LT_BASE (DDR_POLICY_BASE + DDR_POLICY_SIZE)
 #define DDR_POLICY_LT_SIZE (DDR_POLICY_SIZE)
 #define DDR_DBC_BASE (DDR_POLICY_LT_BASE + DDR_POLICY_LT_SIZE)
 #define DDR_DBC_SIZE (0x180)
 #define DDR_DMSS_GLB_BASE (DDR_DMSS_AXI_BASE + DDR_DMSS_AXI_SIZE)
 #define DDR_DMSS_GLB_SIZE (0x180)
 #define DDR_DMSS_AXI_BASE (DDR_DBC_BASE + DDR_DBC_SIZE)
 #define DDR_DMSS_AXI_SIZE (0xA00)
 #define DDR_DMSS_BYP_BASE (DDR_DMSS_GLB_BASE + DDR_DMSS_GLB_SIZE)
 #define DDR_DMSS_BYP_SIZE (0x60)
 #define DDR_NUM_CNT_BASE (DDR_DMSS_BYP_BASE + DDR_DMSS_BYP_SIZE)
 #define DDR_NUM_CNT_SIZE (0x20)
  #define DDR_TRAIN_NUMBER_BASE (DDR_NUM_CNT_BASE)
  #define DDR_PROFILE_NUMBER_BASE (DDR_NUM_CNT_BASE + 0x004)
  #define DMSS_AXI_CFG_NUMBER_BASE (DDR_NUM_CNT_BASE + 0x008)
  #define DMSS_GLOBAL_CFG_NUMBER_BASE (DDR_NUM_CNT_BASE + 0x00C)
  #define DBC_CFG_NUMBER_BASE (DDR_NUM_CNT_BASE + 0x010)
  #define TRAINIG_RESULT_IDX_MAP_SIZE_BASE (DDR_NUM_CNT_BASE + 0x014)
  #define DMSS_PROFILE_NUMBER_BASE (DDR_NUM_CNT_BASE + 0x018)
 #define DDR_VOL_BITMAP_BASE (DDR_NUM_CNT_BASE + DDR_NUM_CNT_SIZE)
 #define DDR_VOL_BITMAP_SIZE (0x40)
 #define DDR_FEATURE_BASE (DDR_VOL_BITMAP_BASE + DDR_VOL_BITMAP_SIZE)
 #define DDR_FEATURE_SIZE (0x10)
 #define DMSS_FEATURE_BASE (DDR_FEATURE_BASE + DDR_FEATURE_SIZE)
 #define DMSS_FEATURE_SIZE (0x10)
 #define DDR_TARGET_INFO_BASE (DMSS_FEATURE_BASE + DMSS_FEATURE_SIZE)
 #define DDR_TARGET_INFO_SIZE (0x140)
 #define UCE_CTRL_CFG_BASE (DDR_TARGET_INFO_BASE + DDR_TARGET_INFO_SIZE)
 #define UCE_CTRL_CFG_SIZE (0x8)
 #define DDR_INIT_INFO_BASE (UCE_CTRL_CFG_BASE + UCE_CTRL_CFG_SIZE)
 #define DDR_INIT_INFO_SIZE (0xA0)
 #define DMSS_INIT_INFO_BASE (DDR_INIT_INFO_BASE + DDR_INIT_INFO_SIZE)
 #define DMSS_INIT_INFO_SIZE (0xA0)
 #define DDR_LATSTAT_DATA_BASE (DMSS_INIT_INFO_BASE + DMSS_INIT_INFO_SIZE)
 #define DDR_LATSTAT_DATA_SIZE (0xA0)
#if ((DDR_LATSTAT_DATA_BASE + DDR_LATSTAT_DATA_SIZE) > (LPMCU_DDR_DATA_SEC_BASE + LPMCU_DDR_DATA_SEC_SIZE))
 #error "lpmcu ddr data section overflow!!!"
#endif
#define AON_SYS_BASE (LPMCU_DDR_DATA_SEC_BASE + LPMCU_DDR_DATA_SEC_SIZE)
#define AON_SYS_RESERVED_SIZE (0x0)
#define AON_SYS_SIZE (0x14000 - AON_SYS_RESERVED_SIZE)
#if ((AON_SYS_BASE + AON_SYS_SIZE + AON_SYS_RESERVED_SIZE) > (M3_SRAM_BASE + M3_SRAM_AO_SIZE))
    #pragma message(PRINT_MACRO(AON_SYS_BASE + AON_SYS_SIZE + AON_SYS_RESERVED_SIZE))
    #error "lpmcu ao region overflow!!!"
#endif
#define SYS_CODE_SEC_BASE (AON_SYS_BASE + AON_SYS_SIZE + AON_SYS_RESERVED_SIZE)
#define SYS_CODE_SEC_RESERVED_SIZE (0x0)
#define SYS_CODE_SEC_SIZE (0x27F40-SYS_CODE_SEC_RESERVED_SIZE)
#define SYS_BSS_SEC_BASE (SYS_CODE_SEC_BASE + SYS_CODE_SEC_SIZE + SYS_CODE_SEC_RESERVED_SIZE)
#define SYS_BSS_SEC_RESERVED_SIZE (0x0)
#define SYS_BSS_SEC_SIZE (0x7600-SYS_BSS_SEC_RESERVED_SIZE)
#define SYS_DATA_SEC_BASE (SYS_BSS_SEC_BASE + SYS_BSS_SEC_SIZE + SYS_BSS_SEC_RESERVED_SIZE)
#define SYS_DATA_SEC_RESERVED_SIZE (0x0)
#define SYS_DATA_SEC_SIZE (0x4C00-SYS_DATA_SEC_RESERVED_SIZE)
#define ONSYS_AVS_RESERVED_BASE (SYS_DATA_SEC_BASE + SYS_DATA_SEC_SIZE + SYS_DATA_SEC_RESERVED_SIZE)
#define ONSYS_AVS_RESERVED_SIZE (0x600)
#define LITTLE_CLUSTER_PROFILE_BASE (ONSYS_AVS_RESERVED_BASE + ONSYS_AVS_RESERVED_SIZE)
#define LITTLE_CLUSTER_PROFILE_SIZE (0x330)
#define MIDDLE_CLUSTER_PROFILE_BASE (LITTLE_CLUSTER_PROFILE_BASE + LITTLE_CLUSTER_PROFILE_SIZE)
#define MIDDLE_CLUSTER_PROFILE_SIZE (0x330)
#define BIG_CLUSTER_PROFILE_BASE (MIDDLE_CLUSTER_PROFILE_BASE + MIDDLE_CLUSTER_PROFILE_SIZE)
#define BIG_CLUSTER_PROFILE_SIZE (0x330)
#define L3_CLUSTER_PROFILE_BASE (BIG_CLUSTER_PROFILE_BASE + BIG_CLUSTER_PROFILE_SIZE)
#define L3_CLUSTER_PROFILE_SIZE (0x330)
#define CPU_BASE_PROFILE_BASE(n) (LITTLE_CLUSTER_PROFILE_BASE + (n) * LITTLE_CLUSTER_PROFILE_SIZE)
#define GPU_BASE_PROFILE_BASE (L3_CLUSTER_PROFILE_BASE + L3_CLUSTER_PROFILE_SIZE)
#define GPU_BASE_PROFILE_SIZE (0x330)
#define GPU_TOP_BASE_PROFILE_BASE (GPU_BASE_PROFILE_BASE + GPU_BASE_PROFILE_SIZE)
#define GPU_TOP_BASE_PROFILE_SIZE (0xD0)
#define SYS_STACK_SEC_BASE (GPU_TOP_BASE_PROFILE_BASE + GPU_TOP_BASE_PROFILE_SIZE)
#define SYS_STACK_SEC_SIZE (0x400)
#if ((SYS_STACK_SEC_BASE + SYS_STACK_SEC_SIZE) != (M3_SRAM_BASE + M3_SRAM_SIZE))
    #pragma message(PRINT_MACRO(SYS_STACK_SEC_BASE + SYS_STACK_SEC_SIZE))
    #error "lpmcu ram overflow!!!"
#endif
#define CFGBUS_REG_BASE (SYS_STACK_SEC_BASE + SYS_STACK_SEC_SIZE)
#define CFGBUS_REG_SIZE (4*1024)
 #define CFGBUS_REG_SAVE_SIZE (1*1024)
 #define CFGBUS_PERI_CRG_ADDR (CFGBUS_REG_BASE)
 #define CFGBUS_PERI_CRG_SIZE (CFGBUS_REG_SAVE_SIZE)
 #define CFGBUS_SCTRL_ADDR (CFGBUS_PERI_CRG_ADDR + CFGBUS_PERI_CRG_SIZE)
 #define CFGBUS_SCTRL_SIZE (CFGBUS_REG_SAVE_SIZE)
 #define CFGBUS_PMCTRL_ADDR (CFGBUS_SCTRL_ADDR + CFGBUS_SCTRL_SIZE)
 #define CFGBUS_PMCTRL_SIZE (CFGBUS_REG_SAVE_SIZE)
 #define CFGBUS_PCTRL_ADDR (CFGBUS_PMCTRL_ADDR + CFGBUS_PMCTRL_SIZE)
 #define CFGBUS_PCTRL_SIZE (CFGBUS_REG_SAVE_SIZE)
#define LPMCU_DDR_MNTN_BASE (SYS_STACK_SEC_BASE + SYS_STACK_SEC_SIZE)
#define LPMCU_DDR_MNTN_SIZE (0x1400)
 #define DDRC_SAVE_TMP_BASE (LPMCU_DDR_MNTN_BASE)
 #define DDRC_SAVE_TMP_SIZE (0x400)
 #define DDRC_SAVE_DMC_RINT (DDRC_SAVE_TMP_BASE + DDRC_SAVE_TMP_SIZE)
 #define DDRC_SAVE_DMC_RINT_SIZE (0x10)
 #define DDRC_SAVE_AVS_PA_BASE (DDRC_SAVE_DMC_RINT + DDRC_SAVE_DMC_RINT_SIZE)
 #define DDRC_SAVE_AVS_PA_SIZE 0x8
 #define DDRC_SAVE_BASE (DDRC_SAVE_AVS_PA_BASE + DDRC_SAVE_AVS_PA_SIZE)
 #define DDRC_TRACKING_BASE (DDRC_SAVE_BASE)
 #define DDRC_SAVE_SIZE (0x900)
 #define DDRC_TRACKING_SIZE (DDRC_SAVE_SIZE)
#if ((DDRC_SAVE_BASE + DDRC_SAVE_SIZE) > (LPMCU_DDR_MNTN_BASE + LPMCU_DDR_MNTN_SIZE))
 #pragma message(PRINT_MACRO(DDRC_SAVE_BASE + DDRC_SAVE_SIZE))
 #error "lpmcu ddr mntn section size overflow!!!"
#endif
#if ((LPMCU_DDR_MNTN_BASE + LPMCU_DDR_MNTN_SIZE) > (M3_SRAM_BASE + LPMCU_RAM_PHY_SIZE))
 #pragma message(PRINT_MACRO(LPMCU_DDR_MNTN_BASE + LPMCU_DDR_MNTN_SIZE))
 #error "lpmcu ddr mntn section end overflow!!!"
#endif
#define M3_SRAM_ADDR_OFFSET(addr) ((addr) - M3_SRAM_BASE)
#ifndef __FASTBOOT__
#define LPRAM_ADDR_TO_AP_ADDR(addr) ((addr - SOC_LPMCU_LP_RAM_BASE_ADDR) + SOC_ACPU_LP_RAM_BASE_ADDR)
#endif
#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
#endif
