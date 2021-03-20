#ifndef _CANCER_XLOADER_PARTITION_H_
#define _CANCER_XLOADER_PARTITION_H_

#include "hisi_partition.h"
#include "xloader_partition_def.h"

static const struct partition partition_table_emmc[] =
{
  {PART_XLOADER,           0,           2*1024,    EMMC_BOOT_MAJOR_PART},
  {PART_RESERVED1,         7*1024,      1024,      EMMC_USER_PART},/* reserved1        1024K  p3*/
  {PART_VRL,               8704,        512,       EMMC_USER_PART},/* VRL              512K   p5*/
  {PART_VRL_BACKUP,        9*1024,      512,       EMMC_USER_PART},/* VRL backup       512K   p6*/
  {PART_NVME,             18*1024,      5*1024,    EMMC_USER_PART},/* nvme              5M    p8*/
  {PART_DDR_PARA,        285*1024,      1*1024,    EMMC_USER_PART},/* DDR_PARA          1M    p17*/
  {PART_LOWPOWER_PARA,   286*1024,      1*1024,    EMMC_USER_PART},/* lowpower_para     1M    p18*/
  {PART_BATT_TP_PARA,    287*1024,      1*1024,    EMMC_USER_PART},/* batt_tp_para      1M    p19*/
  {PART_DFX,             397*1024,     16*1024,    EMMC_USER_PART},/* dfx              16M    p24*/
  {PART_HISEE_IMG,       544*1024,      4*1024,    EMMC_USER_PART},/* part_hisee_img_a  4M    p31*/
  {PART_HISEE_FS,        552*1024,      8*1024,    EMMC_USER_PART},/* hisee_fs          8M    p33*/
  {PART_FASTBOOT,        560*1024,     12*1024,    EMMC_USER_PART},/* fastboot         12M    p34*/
  {PART_VECTOR,          572*1024,      4*1024,    EMMC_USER_PART},/* vector_a          4M    p35*/
#ifdef FILTER_AVS
  {PART_SYSTEM,         2736*1024,   5848*1024,    EMMC_USER_PART},/* system         5848M    p65*/
#endif
  {PART_HIBENCH_IMG,   11632*1024,    128*1024,    EMMC_USER_PART},/* hibench_img     128M    p69*/
#ifdef FACTORY_VERSION
  {PART_HIBENCH_LOG,   12784*1024,     32*1024,    EMMC_USER_PART},/* HIBENCH_LOG      32M     p72*/
#endif
#ifdef CONFIG_HISI_DEBUG_FS
  {PART_FTTEST,                24*1024*1024,        192*1024,    EMMC_USER_PART},/* fttest           192M    p66*/
#endif
#ifdef XLOADER_DDR_TEST
  {PART_DDRTEST,         PART_DDRTEST_START,        48*1024,     EMMC_USER_PART},/* ddrtest            48M    p67*/
#endif
  {"0", 0, 0, 0},
};

static const struct partition partition_table_ufs[] =
{
  {PART_XLOADER,                0,        2*1024,        UFS_PART_0},
  {PART_RESERVED1,         7*1024,          1024,        UFS_PART_2},/* reserved1          1M     p3*/
  {PART_VRL,                  512,           512,        UFS_PART_3},/* VRL                512K   p1*/
  {PART_VRL_BACKUP,          1024,           512,        UFS_PART_3},/* VRL backup         512K   p2*/
  {PART_NVME,             10*1024,        5*1024,        UFS_PART_3},/* nvme               5M     p4*/
  {PART_DDR_PARA,        277*1024,        1*1024,        UFS_PART_3},/* ddr_para           1M     p13*/
  {PART_LOWPOWER_PARA,   278*1024,        1*1024,        UFS_PART_3},/* lowpower_para      1M     p14*/
  {PART_BATT_TP_PARA,    279*1024,        1*1024,        UFS_PART_3},/* batt_tp_para       1M     p15*/
  {PART_DFX,             389*1024,       16*1024,        UFS_PART_3},/* dfx               16M     p20*/
  {PART_HISEE_IMG,       536*1024,        4*1024,        UFS_PART_3},/* part_hisee_img_a   4M     p27*/
  {PART_HISEE_FS,        544*1024,        8*1024,        UFS_PART_3},/* hisee_fs           8M     p29*/
  {PART_FASTBOOT,        552*1024,       12*1024,        UFS_PART_3},/* fastboot          12M     p30*/
  {PART_VECTOR,          564*1024,        4*1024,        UFS_PART_3},/* avs vector         4M     p31*/
#ifdef FILTER_AVS
  {PART_SYSTEM,         2728*1024,     5848*1024,        UFS_PART_3},/* system          5848M     p61*/ //������xloader�¼���ɸѡ�����ľ��񣬸���system����
#endif
  {PART_HIBENCH_IMG,   11624*1024,      128*1024,        UFS_PART_3},/* hibench_img      128M     p65*/
#ifdef FACTORY_VERSION
  {PART_HIBENCH_LOG,   12776*1024,       32*1024,        UFS_PART_3},/* HIBENCH_LOG      32M      p68*/
#endif
#ifdef CONFIG_HISI_DEBUG_FS
  {PART_FTTEST,                24*1024*1024,        192*1024,    UFS_PART_3},/* fttest           192M    p62*/
#endif
#ifdef XLOADER_DDR_TEST
  {PART_DDRTEST,         PART_DDRTEST_START,         48*1024,    UFS_PART_3},/* ddrtest            48M    p67*/
#endif
  {"0", 0, 0, 0},
};

#endif
